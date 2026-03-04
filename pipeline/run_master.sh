#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
# shellcheck disable=SC1091
source "$ROOT_DIR/pipeline/config.sh"

DO_CREATE_EVLS=1
DO_LAMBDA_TREE=1
DO_KAON_TREE=1
DO_GEN_TREE=1
DO_HADD=1
DO_ANALYSIS=0
DO_WAIT=1
MINIMAL=1
DRY_RUN=0
POLL_SECONDS=30
TIMEOUT_SECONDS=0

log() { printf '[%s] %s\n' "$(date +'%F %T')" "$*"; }
warn() { log "WARNING: $*"; }
fail() { log "ERROR: $*"; exit 1; }

usage() {
  cat <<USAGE
Usage: $0 [options]

Options:
  --do-create-evls <0|1>
  --do-lambda-tree <0|1>
  --do-kaon-tree <0|1>
  --do-gen-tree <0|1>
  --do-hadd <0|1>
  --do-analysis <0|1>
  --do-wait <0|1>
  --minimal <0|1>           Submit only Queue 1 from each submit file (default: 1)
  --dry-run <0|1>           Print commands without executing (default: 0)
  --poll-seconds <int>      condor_q polling interval (default: 30)
  --timeout-seconds <int>   0 means no timeout
  -h, --help
USAGE
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --do-create-evls) DO_CREATE_EVLS="$2"; shift 2 ;;
    --do-lambda-tree) DO_LAMBDA_TREE="$2"; shift 2 ;;
    --do-kaon-tree) DO_KAON_TREE="$2"; shift 2 ;;
    --do-gen-tree) DO_GEN_TREE="$2"; shift 2 ;;
    --do-hadd) DO_HADD="$2"; shift 2 ;;
    --do-analysis) DO_ANALYSIS="$2"; shift 2 ;;
    --do-wait) DO_WAIT="$2"; shift 2 ;;
    --minimal) MINIMAL="$2"; shift 2 ;;
    --dry-run) DRY_RUN="$2"; shift 2 ;;
    --poll-seconds) POLL_SECONDS="$2"; shift 2 ;;
    --timeout-seconds) TIMEOUT_SECONDS="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *) fail "Unknown argument: $1" ;;
  esac
done

run_cmd() {
  local cmd="$1"
  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: $cmd"
  else
    eval "$cmd"
  fi
}

prepare_minimal_submit() {
  local infile="$1"
  local outfile="$2"
  awk '
    BEGIN { done = 0 }
    /^\s*[Qq][Uu][Ee][Uu][Ee](\s|$)/ {
      if (!done) {
        print "Queue 1"
        done = 1
        next
      }
    }
    { print }
  ' "$infile" > "$outfile"
}

submit_file() {
  local submit_rel="$1"
  local submit_abs="$ROOT_DIR/$submit_rel"
  [[ -f "$submit_abs" ]] || fail "Missing submit file: $submit_rel"

  if [[ "$DRY_RUN" != "1" ]] && ! command -v condor_submit >/dev/null 2>&1; then
    warn "condor_submit not found; cannot submit $submit_rel"
    return 10
  fi

  local target="$submit_abs"
  local tmp=""
  if [[ "$MINIMAL" == "1" ]]; then
    tmp="$(mktemp)"
    prepare_minimal_submit "$submit_abs" "$tmp"
    target="$tmp"
  fi

  local output
  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: condor_submit $target"
    output=""
  else
    output="$(condor_submit "$target" 2>&1)" || {
      [[ -n "$tmp" ]] && rm -f "$tmp"
      warn "Submission failed for $submit_rel"
      warn "$output"
      return 11
    }
    log "$output"
  fi

  [[ -n "$tmp" ]] && rm -f "$tmp"

  if [[ "$DRY_RUN" == "1" ]]; then
    return 0
  fi

  local cluster
  cluster="$(printf '%s' "$output" | sed -n 's/.*cluster \([0-9][0-9]*\).*/\1/p' | tail -n1)"
  if [[ -n "$cluster" ]]; then
    SUBMITTED_CLUSTERS+=("$cluster")
  else
    warn "Could not parse cluster id from condor_submit output for $submit_rel"
  fi
}

submit_group() {
  local name="$1"; shift
  local files=("$@")
  [[ "${#files[@]}" -gt 0 ]] || { warn "No submit files configured for $name"; return 0; }

  log "Submitting group: $name"
  for f in "${files[@]}"; do
    submit_file "$f" || return $?
  done
}

wait_for_clusters() {
  [[ "${#SUBMITTED_CLUSTERS[@]}" -gt 0 ]] || { log "No clusters submitted; skipping wait"; return 0; }

  if ! command -v condor_q >/dev/null 2>&1; then
    warn "condor_q not found; cannot monitor jobs"
    return 20
  fi

  local start
  start="$(date +%s)"

  log "Waiting for clusters: ${SUBMITTED_CLUSTERS[*]}"
  while true; do
    local active=0
    local details=()
    for c in "${SUBMITTED_CLUSTERS[@]}"; do
      local count
      count="$(condor_q "$c" -autoformat ClusterId ProcId 2>/dev/null | wc -l | tr -d ' ')"
      if [[ "$count" -gt 0 ]]; then
        active=$((active + count))
        details+=("$c:$count")
      fi
    done

    if [[ "$active" -eq 0 ]]; then
      log "All tracked clusters completed"
      return 0
    fi

    log "Still running ($active jobs): ${details[*]}"

    if [[ "$TIMEOUT_SECONDS" -gt 0 ]]; then
      local now elapsed
      now="$(date +%s)"
      elapsed=$((now - start))
      if [[ "$elapsed" -ge "$TIMEOUT_SECONDS" ]]; then
        warn "Timeout reached while waiting for jobs"
        return 21
      fi
    fi

    sleep "$POLL_SECONDS"
  done
}

run_optional_commands() {
  local phase="$1"; shift
  local cmds=("$@")
  local cmd
  for cmd in "${cmds[@]}"; do
    [[ -z "$cmd" ]] && continue
    log "Running $phase command: $cmd"
    run_cmd "$cmd" || return 1
  done
}

SUBMITTED_CLUSTERS=()

log "Pipeline start (minimal=$MINIMAL dry_run=$DRY_RUN)"

if [[ "$DO_CREATE_EVLS" == "1" ]]; then
  submit_group "data:create_evls" "${PIPELINE_DATA_EVL_SUBMITS[@]}" || fail "Failed in data create_evls stage"
  submit_group "mc:create_evlsMC" "${PIPELINE_MC_EVL_SUBMITS[@]}" || fail "Failed in MC create_evls stage"
fi

if [[ "$DO_LAMBDA_TREE" == "1" ]]; then
  submit_group "data:lambdaTree" "${PIPELINE_DATA_LAMBDA_SUBMITS[@]}" || fail "Failed in data lambdaTree stage"
  submit_group "mc:lambdaTreeMC" "${PIPELINE_MC_LAMBDA_SUBMITS[@]}" || fail "Failed in MC lambdaTreeMC stage"
fi

if [[ "$DO_KAON_TREE" == "1" ]]; then
  submit_group "data:kaonTree" "${PIPELINE_DATA_KAON_SUBMITS[@]}" || fail "Failed in data kaonTree stage"
  submit_group "mc:kaonTreeMC" "${PIPELINE_MC_KAON_SUBMITS[@]}" || fail "Failed in MC kaonTreeMC stage"
fi

if [[ "$DO_GEN_TREE" == "1" ]]; then
  submit_group "mc:genMCTree" "${PIPELINE_MC_GEN_SUBMITS[@]}" || fail "Failed in MC genMCTree stage"
fi

if [[ "$DO_WAIT" == "1" ]]; then
  wait_for_clusters || warn "Job monitoring reported issues"
fi

if [[ "$DO_HADD" == "1" ]]; then
  run_optional_commands "hadd" "${PIPELINE_HADD_COMMANDS[@]}" || warn "hadd phase encountered issues"
fi

if [[ "$DO_ANALYSIS" == "1" ]]; then
  run_optional_commands "analysis" "${PIPELINE_ANALYSIS_COMMANDS[@]}" || warn "analysis phase encountered issues"
fi

log "Pipeline complete"

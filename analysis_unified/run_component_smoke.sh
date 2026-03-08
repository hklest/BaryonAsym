#!/usr/bin/env bash
set -euo pipefail

# Component-by-component smoke workflow (no Condor):
# 1) create one steering for event-list step
# 2) run one create_evls/create_evlsMC
# 3) create one tree steering from produced event-list
# 4) run one lambdaTree/kaonTree (+ MC genMCTree)
# 5) run one lightweight analysis check (entry counts + optional unified packaging)

BASE="${BARYONASYM_BASE:-/data/dust/group/h1/klesthen/BaryonAsym}"
PROC="${PROC_INDEX:-0}"
DO_DATA=1
DO_MC=1
DO_PACKAGE=1
DRY_RUN=0

usage() {
  cat <<USAGE
Usage: $0 [--proc <index>] [--do-data 0|1] [--do-mc 0|1] [--do-package 0|1] [--dry-run 0|1]
USAGE
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --proc) PROC="$2"; shift 2 ;;
    --do-data) DO_DATA="$2"; shift 2 ;;
    --do-mc) DO_MC="$2"; shift 2 ;;
    --do-package) DO_PACKAGE="$2"; shift 2 ;;
    --dry-run) DRY_RUN="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown argument: $1" >&2; usage; exit 2 ;;
  esac
done

log() { printf '[%s] %s\n' "$(date +'%F %T')" "$*"; }
warn() { log "WARNING: $*"; }
fail() { log "ERROR: $*"; exit 1; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
RUNNER="$REPO_ROOT/scripts/run_h1_executable.sh"

DATA_BIN_EL9="$BASE/bin/x86_64-el9-gcc14-opt"
DATA_BIN_C7="$BASE/bin/x86_64-centos7-gcc9-opt"

pick_exe() {
  local name="$1"
  if [[ -x "$DATA_BIN_EL9/$name" ]]; then
    echo "$DATA_BIN_EL9/$name"
  elif [[ -x "$DATA_BIN_C7/$name" ]]; then
    echo "$DATA_BIN_C7/$name"
  else
    echo "$DATA_BIN_C7/$name"
  fi
}

run_cmd() {
  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: $*"
  else
    "$@"
  fi
}

ensure_dir() {
  local d="$1"
  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: mkdir -p $d"
  else
    mkdir -p "$d"
  fi
}

write_file() {
  local path="$1"
  local content="$2"
  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: write $path"
  else
    mkdir -p "$(dirname "$path")"
    printf '%s\n' "$content" > "$path"
  fi
}

verify_root_key() {
  local root_file="$1"
  local key_pattern="$2"
  if [[ "$DRY_RUN" == "1" ]]; then
    return 0
  fi
  if ! command -v root >/dev/null 2>&1; then
    warn "ROOT not available for key check: $root_file"
    return 0
  fi
  root -l -b -q "$REPO_ROOT/analysis_unified/verify_root_key.C(\"$root_file\",\"$key_pattern\")" >/tmp/verify_root_key.log 2>&1 || {
    cat /tmp/verify_root_key.log >&2
    fail "ROOT key check failed for $root_file (pattern: $key_pattern)"
  }
}

# ------------------ Data component ------------------
run_data_component() {
  log "=== DATA component smoke (proc=$PROC) ==="

  local i4
  i4=$(printf "%04d" "$PROC")

  local evls_steer="$BASE/H1LambdaC/Steering/2006/SelectedEvents06_${PROC}.steer"
  local evls_out="$BASE/H1LambdaC/EventLists/2006/component_06_evls_${PROC}.root"
  local tree_steer="$BASE/H1LambdaC/Steering/2006/Tree06_${PROC}.steer"
  local lambda_out_prefix="$BASE/H1LambdaC/LambdaTree/2006/ComponentLambdaTree06"
  local kaon_out_prefix="$BASE/H1LambdaC/KaonTree/2006/ComponentKaonTree06"

  write_file "$evls_steer" "H1SteerTree() {
   // auto-generated component smoke steering (data)
   fHatFiles  = \"/pnfs/desy.de/dphep/online/h1/data/oo-4.0/06/hat.4.0.6.10dst7.c06${i4}.root\";
   fModsFiles = \"/pnfs/desy.de/dphep/online/h1/data/oo-4.0/06/mods.4.0.6.10dst7.c06${i4}.root\";
}"

  ensure_dir "$(dirname "$evls_out")"
  run_cmd "$RUNNER" "$(pick_exe create_evls)" "$evls_steer" "${evls_out%.root}" "$PROC"

  write_file "$tree_steer" "H1SteerTree() {fEventListFiles =  \"$evls_out\";}"

  ensure_dir "$(dirname "$lambda_out_prefix")"
  ensure_dir "$(dirname "$kaon_out_prefix")"

  run_cmd "$RUNNER" "$(pick_exe lambdaTree)" "$tree_steer" "$lambda_out_prefix" "$PROC"
  run_cmd "$RUNNER" "$(pick_exe kaonTree)" "$tree_steer" "$kaon_out_prefix" "$PROC"

  if [[ "$DRY_RUN" != "1" ]]; then
    [[ -s "$evls_out" ]] || fail "Missing/empty data event-list output: $evls_out"
    [[ -s "${lambda_out_prefix}_${PROC}.root" ]] || fail "Missing/empty data lambda output"
    [[ -s "${kaon_out_prefix}_${PROC}.root" ]] || fail "Missing/empty data kaon output"
  fi

  verify_root_key "$evls_out" "mylist"
  verify_root_key "${lambda_out_prefix}_${PROC}.root" "Lambda"
  verify_root_key "${kaon_out_prefix}_${PROC}.root" "K0"
}

# ------------------ MC component ------------------
run_mc_component() {
  log "=== MC component smoke (proc=$PROC) ==="

  local evls_steer="$BASE/H1LambdaC_MC/Steering/RAPGAP31/Rad/2006/RAPGAP31_06RAD_${PROC}.steer"
  local evls_out="$BASE/H1LambdaC_MC/EventLists/RAPGAP31/2006/component_06_evlsMC_RAPGAP31_Rad_${PROC}.root"
  local tree_steer="$BASE/H1LambdaC_MC/Steering/RAPGAP31/Rad/2006/TreeMC_RAPGAP31_06_RAD_${PROC}.steer"
  local lambda_out_prefix="$BASE/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/ComponentLambdaTreeMC_RAPGAP31_06"
  local kaon_out_prefix="$BASE/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006/ComponentKaonTreeMC_RAPGAP31_06"
  local gen_out_prefix="$BASE/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006/ComponentGenMCTree_06_RAPGAP31_Rad"

  write_file "$evls_steer" "H1SteerTree() {
   // auto-generated component smoke steering (mc)
   fDirectoryNames=\"/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/rapgap31/\";
   fLoadHAT  = true;
   fLoadMODS = true;
   fLoadODS  = false;
   fLoadUserTree = false;
}"

  ensure_dir "$(dirname "$evls_out")"
  run_cmd "$RUNNER" "$(pick_exe create_evlsMC)" "$evls_steer" "${evls_out%.root}" "$PROC"

  write_file "$tree_steer" "H1SteerTree() {fEventListFiles =  \"$evls_out\";}"

  ensure_dir "$(dirname "$lambda_out_prefix")"
  ensure_dir "$(dirname "$kaon_out_prefix")"
  ensure_dir "$(dirname "$gen_out_prefix")"

  run_cmd "$RUNNER" "$(pick_exe lambdaTreeMC)" "$tree_steer" "$lambda_out_prefix" "$PROC"
  run_cmd "$RUNNER" "$(pick_exe kaonTreeMC)" "$tree_steer" "$kaon_out_prefix" "$PROC"
  run_cmd "$RUNNER" "$(pick_exe genMCTree)" "$evls_steer" "$gen_out_prefix" "$PROC"

  if [[ "$DRY_RUN" != "1" ]]; then
    [[ -s "$evls_out" ]] || fail "Missing/empty mc event-list output: $evls_out"
    [[ -s "${lambda_out_prefix}_${PROC}.root" ]] || fail "Missing/empty mc lambda output"
    [[ -s "${kaon_out_prefix}_${PROC}.root" ]] || fail "Missing/empty mc kaon output"
    [[ -s "${gen_out_prefix}_${PROC}.root" ]] || fail "Missing/empty mc gen output"
  fi

  verify_root_key "$evls_out" "mylist"
  verify_root_key "${lambda_out_prefix}_${PROC}.root" "Lambda"
  verify_root_key "${kaon_out_prefix}_${PROC}.root" "K0"
  verify_root_key "${gen_out_prefix}_${PROC}.root" "Lambda"
}

run_packaging_check() {
  [[ "$DO_PACKAGE" == "1" ]] || return 0

  local out="$BASE/H1LambdaC/AnalysisResultFiles/UnifiedTrees_component_${PROC}.root"
  local data_rec="$BASE/H1LambdaC/LambdaTree/2006/ComponentLambdaTree06_${PROC}.root"
  local mc_rec="$BASE/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/ComponentLambdaTreeMC_RAPGAP31_06_${PROC}.root"
  local mc_gen="$BASE/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006/ComponentGenMCTree_06_RAPGAP31_Rad_${PROC}.root"

  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: root -l -b -q \"$REPO_ROOT/analysis_unified/build_unified_treefile.C(\\\"$data_rec\\\",\\\"$mc_rec\\\",\\\"$mc_gen\\\",\\\"$out\\\")\""
    return 0
  fi

  if ! command -v root >/dev/null 2>&1; then
    warn "ROOT not found; skipping unified packaging check"
    return 0
  fi

  root -l -b -q "$REPO_ROOT/analysis_unified/build_unified_treefile.C(\"$data_rec\",\"$mc_rec\",\"$mc_gen\",\"$out\")"
  [[ -s "$out" ]] || fail "Unified packaging output missing/empty: $out"
  verify_root_key "$out" "data"
  verify_root_key "$out" "mc"
}

if [[ "$DO_DATA" == "1" ]]; then run_data_component; fi
if [[ "$DO_MC" == "1" ]]; then run_mc_component; fi
run_packaging_check

log "Component smoke workflow completed successfully"

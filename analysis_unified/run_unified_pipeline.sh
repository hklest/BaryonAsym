#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

DO_SUBMIT=1
DO_PACKAGE=1
DO_ANALYSIS=0
MINIMAL=1
DRY_RUN=0

usage() {
  cat <<USAGE
Usage: $0 [--do-submit 0|1] [--do-package 0|1] [--do-analysis 0|1] [--minimal 0|1] [--dry-run 0|1]
USAGE
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --do-submit) DO_SUBMIT="$2"; shift 2 ;;
    --do-package) DO_PACKAGE="$2"; shift 2 ;;
    --do-analysis) DO_ANALYSIS="$2"; shift 2 ;;
    --minimal) MINIMAL="$2"; shift 2 ;;
    --dry-run) DRY_RUN="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown argument: $1" >&2; usage; exit 2 ;;
  esac
done

log() { printf '[%s] %s\n' "$(date +'%F %T')" "$*"; }
warn() { log "WARNING: $*"; }

if [[ "$DO_SUBMIT" == "1" ]]; then
  log "Running unified submit orchestration"
  "$REPO_ROOT/pipeline/run_master.sh" \
    --minimal "$MINIMAL" \
    --dry-run "$DRY_RUN" \
    --do-analysis 0
fi

if [[ "$DO_PACKAGE" == "1" ]]; then
  # shellcheck disable=SC1091
  source "$REPO_ROOT/analysis_unified/config.sh"

  ROOT_CMD="$REPO_ROOT/analysis_unified/build_unified_treefile.C(\"$UNIFIED_DATA_REC_FILE\",\"$UNIFIED_MC_REC_FILE\",\"$UNIFIED_MC_GEN_FILE\",\"$UNIFIED_OUT_FILE\")"
  if [[ "$DRY_RUN" == "1" ]]; then
    log "DRY-RUN: root -l -b -q "$ROOT_CMD""
  else
    if ! command -v root >/dev/null 2>&1; then
      warn "ROOT not available; skipping packaging step"
    else
      root -l -b -q "$REPO_ROOT/analysis_unified/build_unified_treefile.C(\"$UNIFIED_DATA_REC_FILE\",\"$UNIFIED_MC_REC_FILE\",\"$UNIFIED_MC_GEN_FILE\",\"$UNIFIED_OUT_FILE\")"
    fi
  fi
fi

if [[ "$DO_ANALYSIS" == "1" ]]; then
  warn "Unified analysis macro execution hook not yet wired; plug your AnalysisQ2/Analysis.C call here."
fi

log "Unified pipeline script completed"

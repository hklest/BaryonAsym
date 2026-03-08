#!/usr/bin/env bash
set -euo pipefail

# Unified stage runner for data/MC production.
# Keeps existing H1 interfaces (same binaries) but exposes one mode-based entrypoint.

usage() {
  cat <<USAGE
Usage: $0 --mode <data|mc> --stage <evls|lambda|kaon|gen> --steering <file.steer> --output-prefix <path/prefix> --chain <id>

Notes:
  - stage=gen is only valid with --mode mc
  - executable paths can be overridden via env vars below

Env overrides:
  BARYONASYM_DATA_BIN
  BARYONASYM_MC_BIN
USAGE
}

MODE=""
STAGE=""
STEERING=""
OUTPUT_PREFIX=""
CHAIN=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --mode) MODE="$2"; shift 2 ;;
    --stage) STAGE="$2"; shift 2 ;;
    --steering) STEERING="$2"; shift 2 ;;
    --output-prefix) OUTPUT_PREFIX="$2"; shift 2 ;;
    --chain) CHAIN="$2"; shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown argument: $1" >&2; usage; exit 2 ;;
  esac
done

[[ -n "$MODE" && -n "$STAGE" && -n "$STEERING" && -n "$OUTPUT_PREFIX" && -n "$CHAIN" ]] || {
  usage
  exit 2
}

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
RUNNER="$REPO_ROOT/scripts/run_h1_executable.sh"

DATA_BIN="${BARYONASYM_DATA_BIN:-/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-centos7-gcc9-opt}"
MC_BIN="${BARYONASYM_MC_BIN:-/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-centos7-gcc9-opt}"

EXEC=""
case "$MODE:$STAGE" in
  data:evls) EXEC="$DATA_BIN/create_evls" ;;
  data:lambda) EXEC="$DATA_BIN/lambdaTree" ;;
  data:kaon) EXEC="$DATA_BIN/kaonTree" ;;
  mc:evls) EXEC="$MC_BIN/create_evlsMC" ;;
  mc:lambda) EXEC="$MC_BIN/lambdaTreeMC" ;;
  mc:kaon) EXEC="$MC_BIN/kaonTreeMC" ;;
  mc:gen) EXEC="$MC_BIN/genMCTree" ;;
  data:gen)
    echo "stage=gen is only valid for mode=mc" >&2
    exit 2
    ;;
  *)
    echo "Unsupported mode/stage: $MODE/$STAGE" >&2
    exit 2
    ;;
esac

"$RUNNER" "$EXEC" "$STEERING" "$OUTPUT_PREFIX" "$CHAIN"

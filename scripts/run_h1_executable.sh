#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 4 ]]; then
  echo "Usage: $0 <executable> <steering> <output_prefix> <chain>" >&2
  exit 2
fi

EXECUTABLE="$1"
STEERING="$2"
OUTPUT_PREFIX="$3"
CHAIN="$4"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/common_h1_env.sh"

AUTO_REBUILD_EXECUTABLES="${AUTO_REBUILD_EXECUTABLES:-1}"

maybe_rebuild_executable() {
  local exe="$1"
  local target
  target="$(basename "$exe")"

  [[ "$AUTO_REBUILD_EXECUTABLES" == "1" ]] || return 0

  local module_dir
  case "$target" in
    *MC|*MCree|genMCTree|kaonTreeMC|lambdaTreeMC|create_evlsMC)
      module_dir="$REPO_ROOT/H1LambdaC_MC"
      ;;
    *)
      module_dir="$REPO_ROOT/H1LambdaC"
      ;;
  esac

  if [[ ! -d "$module_dir" ]]; then
    echo "WARNING: module dir not found for rebuild: $module_dir" >&2
    return 0
  fi

  echo "[run_h1_executable] rebuilding target '$target' in $module_dir"
  if ! make -C "$module_dir" "$target"; then
    echo "WARNING: rebuild failed for $target (continuing with existing executable path)" >&2
  fi
}

maybe_rebuild_executable "$EXECUTABLE"

echo " ------------------------------------------------------------------------ "
echo "Executable Inputs:"
echo "  EXECUTABLE: $EXECUTABLE"
echo "  STEERING:   $STEERING"
echo "  OOUTPUT:    $OUTPUT_PREFIX"
echo "  CHAIN:      $CHAIN"
echo " ------------------------------------------------------------------------ "

"$EXECUTABLE" -f "$STEERING" -o "${OUTPUT_PREFIX}_${CHAIN}.root"

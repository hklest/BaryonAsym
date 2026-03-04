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
# shellcheck disable=SC1091
source "$SCRIPT_DIR/common_h1_env.sh"

echo " ------------------------------------------------------------------------ "
echo "Executable Inputs:"
echo "  EXECUTABLE: $EXECUTABLE"
echo "  STEERING:   $STEERING"
echo "  OOUTPUT:    $OUTPUT_PREFIX"
echo "  CHAIN:      $CHAIN"
echo " ------------------------------------------------------------------------ "

"$EXECUTABLE" -f "$STEERING" -o "${OUTPUT_PREFIX}_${CHAIN}.root"

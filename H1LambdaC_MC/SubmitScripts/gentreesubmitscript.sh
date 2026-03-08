#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
"$REPO_ROOT/scripts/run_h1_executable.sh" \
  "/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-centos7-gcc9-opt/genMCTree" \
  "$1" "$2" "$3"

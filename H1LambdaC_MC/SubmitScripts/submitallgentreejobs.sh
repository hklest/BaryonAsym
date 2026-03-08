#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
"$REPO_ROOT/pipeline/run_master.sh" \
  --do-create-evls 0 \
  --do-lambda-tree 0 \
  --do-kaon-tree 0 \
  --do-gen-tree 1 \
  --do-hadd 0 \
  --do-analysis 0

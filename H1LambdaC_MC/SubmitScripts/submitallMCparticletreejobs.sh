#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
"$REPO_ROOT/pipeline/run_master.sh" \
  --do-create-evls 0 \
  --do-lambda-tree 1 \
  --do-kaon-tree 1 \
  --do-gen-tree 0 \
  --do-hadd 0 \
  --do-analysis 0

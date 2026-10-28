#!/usr/bin/env bash
# build_wpa6_job.sh - WP-A6 S1 payload builder worker.  Usage: build_wpa6_job.sh <chain> <split> <out>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
CAMPAIGN=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/condor/restart_2026-10-27/wp-a6/build
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a6/build_payload_wpa6.py
CHAIN=$1; SPLIT=$2; OUT=$3; TAG=${SPLIT}_${CHAIN}
export PYTHONDONTWRITEBYTECODE=1
echo "=== build_wpa6 [$TAG] started at $(date) on $(hostname) ==="
python3 -c "import numpy, scipy" >/dev/null 2>&1 || { echo "ERROR: python3 numpy/scipy not importable" >&2; exit 1; }
mkdir -p "$(dirname "$OUT")" "$CAMPAIGN/status"
md5sum "$SCRIPT" > "$CAMPAIGN/status/md5_$TAG.txt"
python3 "$SCRIPT" --chain "$CHAIN" --split "$SPLIT" --out "$OUT"
RC=$?
echo "=== build_wpa6 [$TAG] rc=$RC at $(date) ==="
echo "$RC" > "$CAMPAIGN/status/rc_$TAG.txt"
exit $RC

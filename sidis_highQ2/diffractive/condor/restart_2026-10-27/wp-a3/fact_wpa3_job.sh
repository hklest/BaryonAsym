#!/usr/bin/env bash
# WP-A3 factorisation analysis wrapper: one job over the 17 skims.
# Usage: fact_wpa3_job.sh [outname] [extra args...]   (run 1: no args -> fact_results.json)
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a3/fact_wpa3.py
NAME=${1:-fact_results}; shift || true
OUT=$BASE/data/restart_2026-10-27/wp-a3/fact/$NAME.json
echo "=== fact_wpa3 start $(date) on $(hostname) args: $* ==="; md5sum "$SCRIPT"
python3 "$SCRIPT" --out "$OUT" "$@"; RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

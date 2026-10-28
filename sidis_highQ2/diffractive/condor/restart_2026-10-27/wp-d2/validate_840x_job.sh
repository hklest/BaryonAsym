#!/usr/bin/env bash
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
echo "=== validate_840x start $(date) on $(hostname) ==="
python3 -c "import uproot, numpy" >/dev/null 2>&1 || { echo "ERROR: uproot/numpy missing" >&2; exit 1; }
python3 -B $BASE/scripts/restart_2026-10-27/wp-d2/validate_840x.py
RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

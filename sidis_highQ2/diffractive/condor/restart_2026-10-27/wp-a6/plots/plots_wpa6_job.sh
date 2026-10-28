#!/usr/bin/env bash
# WP-A6 spectra plots (reads the 17 WP-A3 skims). Usage: plots_wpa6_job.sh <split>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a6/plots_wpa6.py
export PYTHONDONTWRITEBYTECODE=1
echo "=== plots_wpa6 start $(date) on $(hostname) split: $1 ==="; md5sum "$SCRIPT"
python3 -c "import matplotlib, numpy; print('matplotlib', matplotlib.__version__)" || exit 3
python3 "$SCRIPT" --split "$1" --part spectra; RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

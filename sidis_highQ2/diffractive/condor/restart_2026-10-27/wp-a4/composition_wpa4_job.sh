#!/usr/bin/env bash
# WP-A4 kinematic-scaling analysis wrapper: one job over the 17 skims in data/restart_2026-10-27/wp-a4/skims.
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a4/composition_wpa4.py
OUT=$BASE/data/restart_2026-10-27/wp-a4/composition.json
echo "=== composition_wpa4 start $(date) on $(hostname) ==="; md5sum "$SCRIPT"
n=$(ls $BASE/data/restart_2026-10-27/wp-a4/skims/*.npz 2>/dev/null | wc -l); [ "$n" = 17 ] || { echo "ERROR: $n/17 skims" >&2; exit 2; }
python3 "$SCRIPT" --out "$OUT"; RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

#!/usr/bin/env bash
# WP-A5 edge-3.0 re-run: propagation at 3.0 and a 3.2 reproduction check against the frozen g1pp_a_table.csv.
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SD=$BASE/scripts/restart_2026-10-27/wp-a5/edge30; DD=$BASE/data/restart_2026-10-27/wp-a5
export PYTHONDONTWRITEBYTECODE=1
echo "=== wpa5 edge30 start $(date -u +%FT%TZ) on $(hostname) ==="
md5sum "$SD/propagate_wpa5_edge30.py"; python3 -c "import numpy; print('numpy', numpy.__version__)" || exit 3
python3 "$SD/propagate_wpa5_edge30.py" --edge 3.0 --out "$DD/edge30"; RC=$?
python3 "$SD/propagate_wpa5_edge30.py" --edge 3.2 --out "$DD/edge30/check_edge32" > "$DD/edge30/check_edge32.log" 2>&1; RC2=$?
echo "rc30=$RC rc32=$RC2"
cmp "$DD/g1pp_a_table.csv" "$DD/edge30/check_edge32/g1pp_a_table_edge32.csv" && echo "CHECK32: byte-identical"
echo "=== done $(date -u +%FT%TZ) ==="; exit $((RC+RC2))

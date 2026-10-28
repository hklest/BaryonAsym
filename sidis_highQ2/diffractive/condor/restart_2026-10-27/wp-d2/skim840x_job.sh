#!/usr/bin/env bash
# WP-D2: WP-A3 skim (skim_wpa3.py) for one 840x merged file, then wpc_build.py for the same file. Usage: skim840x_job.sh <label>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
L=$1; IN=$BASE/data/t2_detector/mc840x_merged_2026-10-28/$L.root
TAB=$BASE/data/restart_2026-10-27/wp-d2/scale_table_840x.csv
echo "=== skim840x [$L] start $(date) on $(hostname) ==="
[ -f "$IN" ] || { echo "missing $IN" >&2; exit 2; }
python3 -B $BASE/scripts/restart_2026-10-27/wp-a3/skim_wpa3.py --kind mc --label $L --input $IN --out $BASE/data/restart_2026-10-27/wp-a3/skims/$L.npz --scale-table $TAB || exit 3
python3 -B $BASE/scripts/restart_2026-10-27/wp-c/study/wpc_build.py --label $L --input $IN --out $BASE/data/restart_2026-10-27/wp-c/study/s$L --table $TAB; RC=$?
echo "=== rc=$RC $(date) ==="; exit $RC

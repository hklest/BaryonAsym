#!/usr/bin/env bash
# WP-A3 skim wrapper (restart 2026-10-27). One merged input per job. Usage: skim_wpa3_job.sh <label> <kind> <input>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a3/skim_wpa3.py
OUT=$BASE/data/restart_2026-10-27/wp-a3/skims
V3TABLE=$BASE/docs/mc/composite_scale_table_v3_2026-10-20.csv
SIDECAR=$BASE/docs/mc/composite_scale_table_v3_nd4cat_sidecar_2026-10-20.csv
RUNLIST=$BASE/data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv
V3SHA=b7609559db490e6d3559feb3e7463e09dc30ddba35ff6ce9b00457a1e97b209f
LABEL=$1; KIND=$2; INPUT=$3
echo "=== skim_wpa3 [$LABEL/$KIND] start $(date) on $(hostname) ==="
python3 -c "import uproot, numpy" >/dev/null 2>&1 || { echo "ERROR: uproot/numpy missing" >&2; exit 1; }
[ -f "$INPUT" ] || { echo "ERROR: missing input $INPUT" >&2; exit 2; }
[ "$(sha256sum "$V3TABLE" | awk '{print $1}')" = "$V3SHA" ] || { echo "ERROR: v3 table sha mismatch" >&2; exit 3; }
mkdir -p "$OUT"; md5sum "$SCRIPT"
if [ "$KIND" = "data" ]; then ARGS="--runlist $RUNLIST"
else case "$LABEL" in 7094|7096|7097) ARGS="--scale-table $SIDECAR";; *) ARGS="--scale-table $V3TABLE";; esac; fi
python3 "$SCRIPT" --kind "$KIND" --label "$LABEL" --input "$INPUT" --out "$OUT/$LABEL.npz" $ARGS
RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

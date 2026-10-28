#!/usr/bin/env bash
# CJC-containment skim wrapper (restart 2026-10-28). One merged input per job. Usage: cjc_skim_job.sh <label> <kind> <input>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/cjc_containment/cjc_skim.py
OUT=$BASE/data/restart_2026-10-27/cjc_containment/skims
V3TABLE=$BASE/docs/mc/composite_scale_table_v3_2026-10-20.csv
RUNLIST=$BASE/data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv
V3SHA=b7609559db490e6d3559feb3e7463e09dc30ddba35ff6ce9b00457a1e97b209f
LABEL=$1; KIND=$2; INPUT=$3
echo "=== cjc_skim [$LABEL/$KIND] start $(date) on $(hostname) ==="
export PYTHONDONTWRITEBYTECODE=1
[ -f "$INPUT" ] || { echo "ERROR: missing input $INPUT" >&2; exit 2; }
[ "$(sha256sum "$V3TABLE" | awk '{print $1}')" = "$V3SHA" ] || { echo "ERROR: v3 table sha mismatch" >&2; exit 3; }
mkdir -p "$OUT"; md5sum "$SCRIPT"
if [ "$KIND" = "data" ]; then ARGS="--runlist $RUNLIST"; else ARGS="--scale-table $V3TABLE"; fi
python3 -B "$SCRIPT" --kind "$KIND" --label "$LABEL" --input "$INPUT" --out "$OUT/$LABEL.npz" $ARGS
RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

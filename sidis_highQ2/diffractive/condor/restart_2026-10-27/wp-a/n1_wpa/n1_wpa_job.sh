#!/usr/bin/env bash
# WP-A n1_wpa worker wrapper (restart 2026-10-27). One merged input per job.
# Usage: n1_wpa_job.sh <label> <kind> <input> <split> <outdir> <noise_table|-> <chain|-> <extra|->
#   extra: '-' or a single token among: thrT (e.g. thr10) | noplug
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
CAMP=$BASE/condor/restart_2026-10-27/wp-a/n1_wpa
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a/n1_wpa.py
LABEL=$1; KIND=$2; INPUT=$3; SPLIT=$4; OUTDIR=$5; NT=$6; CHAIN=$7; EXTRA=$8
V3TABLE=$BASE/docs/mc/composite_scale_table_v3_2026-10-20.csv
SIDECAR=$BASE/docs/mc/composite_scale_table_v3_nd4cat_sidecar_2026-10-20.csv
RUNLIST=$BASE/data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv
V3SHA=b7609559db490e6d3559feb3e7463e09dc30ddba35ff6ce9b00457a1e97b209f
TAG=${LABEL}_${SPLIT}_$(basename $OUTDIR)
echo "=== n1_wpa [$LABEL/$KIND/$SPLIT] start $(date) on $(hostname) ==="
python3 -c "import uproot, numpy" >/dev/null 2>&1 || { echo "ERROR: uproot/numpy missing" >&2; exit 1; }
[ -f "$INPUT" ] || { echo "ERROR: missing input $INPUT" >&2; exit 2; }
SHA=$(sha256sum "$V3TABLE" | awk '{print $1}')
[ "$SHA" = "$V3SHA" ] || { echo "ERROR: v3 table sha mismatch $SHA" >&2; exit 3; }
mkdir -p "$OUTDIR" "$CAMP/status"
md5sum "$SCRIPT" > "$CAMP/status/md5_${TAG}.txt"
if [ "$KIND" = "data" ]; then ARGS="--runlist $RUNLIST"
else case "$LABEL" in 7094|7096|7097) ARGS="--scale-table $SIDECAR";; *) ARGS="--scale-table $V3TABLE";; esac; fi
[ "$NT" != "-" ] && ARGS="$ARGS --noise-table $NT --chain $CHAIN"
case "$EXTRA" in thr*) ARGS="$ARGS --plug-threshold ${EXTRA#thr}";; noplug) ARGS="$ARGS --no-plug-veto";; esac
[ "$SPLIT" != "all" ] && ARGS="$ARGS --write-keys"
python3 "$SCRIPT" --kind "$KIND" --label "$LABEL" --input "$INPUT" --split "$SPLIT" --outdir "$OUTDIR" $ARGS > "$OUTDIR/stdout.json"
RC=$?
echo "=== worker rc=$RC $(date) ==="
[ $RC -ne 0 ] && exit $RC
python3 - "$OUTDIR/partial.json" "$LABEL" "$SPLIT" <<'PY'
import json,sys
p=json.load(open(sys.argv[1])); w=p["wpa"]
ok=p["label"]==sys.argv[2] and w["split"]==sys.argv[3] and p["conservation"]["checked_in_worker"] and w["control_test_key_overlap"]==0
print(json.dumps({"label":p["label"],"split":w["split"],"n_kept_pre":w["n_kept_pre"],"unique":w["run_event_unique"],"ok":ok}))
sys.exit(0 if ok else 4)
PY
exit $?

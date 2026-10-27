#!/usr/bin/env bash
# fit_wpa_job.sh — WP-A section 7.1 two-free-template fit worker (one alt/binning/scope per job).
# Usage: fit_wpa_job.sh <alt> <nd_labels> <fac_overrides|-> <binning> <scope> <payload> <outdir> <n_toys>
# nd_labels and fac_overrides accept ',' or ':' separators (use ':' inside queue files); fac_overrides '-' = none. No set -u.
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
CAMPAIGN=$BASE/condor/restart_2026-10-27/wp-a/fit_wpa
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a/fit_7p1_wpa.py
ALT=$1; NDL=$(echo "$2" | tr ':' ','); FACS=$3; BINNING=$4; SCOPE=$5; PAYLOAD=$6; OUTDIR=$7; NTOYS=$8
TAG=${ALT}_${BINNING}_${SCOPE}
echo "=== fit_wpa [$TAG] started at $(date) on $(hostname) ==="
python3 -c "import numpy, scipy" >/dev/null 2>&1 || { echo "ERROR: python3 numpy/scipy not importable" >&2; exit 1; }
[ -f "$PAYLOAD" ] || { echo "ERROR: missing payload $PAYLOAD" >&2; exit 2; }
mkdir -p "$OUTDIR" "$CAMPAIGN/status" "$CAMPAIGN/logs"
md5sum "$SCRIPT" > "$CAMPAIGN/status/md5_${TAG}.txt"
FACARGS=""
if [ "$FACS" != "-" ]; then
    for kv in $(echo "$FACS" | tr ',:' '  '); do FACARGS="$FACARGS --fac-override $kv"; done
fi
python3 "$SCRIPT" --payload "$PAYLOAD" --alt "$ALT" --nd-labels "$NDL" $FACARGS \
    --binning "$BINNING" --scope "$SCOPE" --n-toys "$NTOYS" --outdir "$OUTDIR"
RC=$?
echo "=== fit_wpa [$TAG] rc=$RC at $(date) ==="
echo "$RC" > "$CAMPAIGN/status/rc_${TAG}.txt"
exit $RC

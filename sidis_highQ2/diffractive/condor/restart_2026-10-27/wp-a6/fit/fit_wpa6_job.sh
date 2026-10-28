#!/usr/bin/env bash
# fit_wpa6_job.sh — WP-A6 BB-lite fit worker: one (chain, scope, M_X class, variant) per job.
# Usage: fit_wpa6_job.sh <chain> <scope> <mx> <eta_lo> <eta_hi> <xip none|0.04> <bb|nobb> <payload> <outdir> <n_toys> <vtag>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
CAMPAIGN=$BASE/condor/restart_2026-10-27/wp-a6/fit
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a6/fit_bblite_wpa6.py
CHAIN=$1; SCOPE=$2; MX=$3; ELO=$4; EHI=$5; XIP=$6; BB=$7; PAYLOAD=$8; OUTDIR=$9; NTOYS=${10}; VTAG=${11}
SPLIT=$(basename "$(dirname "$OUTDIR")")
KEY=${SPLIT}_${VTAG}_${CHAIN}_${SCOPE}_${MX}
export PYTHONDONTWRITEBYTECODE=1
echo "=== fit_wpa6 [$KEY] started at $(date) on $(hostname) ==="
python3 -c "import numpy, scipy" >/dev/null 2>&1 || { echo "ERROR: python3 numpy/scipy not importable" >&2; exit 1; }
[ -f "$PAYLOAD" ] || { echo "ERROR: missing payload $PAYLOAD" >&2; exit 2; }
mkdir -p "$OUTDIR" "$CAMPAIGN/status"
md5sum "$SCRIPT" > "$CAMPAIGN/status/md5_${KEY}.txt"
sha256sum "$PAYLOAD" > "$CAMPAIGN/status/payload_sha256_${KEY}.txt"
BBARG=""; [ "$BB" = "nobb" ] && BBARG="--no-bb"
python3 "$SCRIPT" fit --payload "$PAYLOAD" --outdir "$OUTDIR" --chain "$CHAIN" --scope "$SCOPE" --mx-class "$MX" \
    --eta-lo "$ELO" --eta-hi "$EHI" --xip-cut "$XIP" $BBARG --n-toys "$NTOYS" --tag "$VTAG"
RC=$?
echo "=== fit_wpa6 [$KEY] rc=$RC at $(date) ==="
echo "$RC" > "$CAMPAIGN/status/rc_${KEY}.txt"
exit $RC

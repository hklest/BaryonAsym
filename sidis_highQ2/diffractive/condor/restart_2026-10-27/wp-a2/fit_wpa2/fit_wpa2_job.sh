#!/usr/bin/env bash
# fit_wpa2_job.sh — WP-A2 section 7.1 fit worker (fork of wp-a/fit_wpa/fit_wpa_job.sh): one alt/binning/scope per job.
# Usage: fit_wpa2_job.sh <alt> <nd_labels> <fac_overrides|-> <binning> <scope> <payload> <outdir> <n_toys> <templates> <tag>
# nd_labels and fac_overrides accept ',' or ':' separators; templates 2|3 (3 = bounded PDISS+Reggeon split, [0.5,2]).
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
CAMPAIGN=$BASE/condor/restart_2026-10-27/wp-a2/fit_wpa2
SCRIPT=$BASE/scripts/restart_2026-10-27/wp-a2/fit_7p1_wpa2.py
ALT=$1; NDL=$(echo "$2" | tr ':' ','); FACS=$3; BINNING=$4; SCOPE=$5; PAYLOAD=$6; OUTDIR=$7; NTOYS=$8; NTPL=$9; JTAG=${10}
TAG=${JTAG}_${ALT}_${BINNING}_${SCOPE}
echo "=== fit_wpa2 [$TAG] started at $(date) on $(hostname) ==="
python3 -c "import numpy, scipy" >/dev/null 2>&1 || { echo "ERROR: python3 numpy/scipy not importable" >&2; exit 1; }
[ -f "$PAYLOAD" ] || { echo "ERROR: missing payload $PAYLOAD" >&2; exit 2; }
mkdir -p "$OUTDIR" "$CAMPAIGN/status"
md5sum "$SCRIPT" > "$CAMPAIGN/status/md5_${TAG}.txt"
sha256sum "$PAYLOAD" > "$CAMPAIGN/status/payload_sha256_${TAG}.txt"
FACARGS=""
if [ "$FACS" != "-" ]; then
    for kv in $(echo "$FACS" | tr ',:' '  '); do FACARGS="$FACARGS --fac-override $kv"; done
fi
python3 "$SCRIPT" --payload "$PAYLOAD" --alt "$ALT" --nd-labels "$NDL" $FACARGS \
    --binning "$BINNING" --scope "$SCOPE" --n-toys "$NTOYS" --templates "$NTPL" --outdir "$OUTDIR"
RC=$?
echo "=== fit_wpa2 [$TAG] rc=$RC at $(date) ==="
echo "$RC" > "$CAMPAIGN/status/rc_${TAG}.txt"
exit $RC

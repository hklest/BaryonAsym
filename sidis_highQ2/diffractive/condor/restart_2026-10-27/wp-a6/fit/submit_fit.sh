#!/usr/bin/env bash
# submit_fit.sh <queue file> <tag> — WP-A6 fit submission: write submit_log_<tag>.txt, verify range_frozen, submit.
# Tag `odd` (or any queue with /odd/ outdirs) aborts unless range_frozen.json exists and matches its .sha256 sidecar.
set -u
[ $# -eq 2 ] || { echo "usage: $0 <queue file> <tag>" >&2; exit 1; }
Q=$(readlink -f "$1"); TAG=$2
D=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
F=$D/condor/restart_2026-10-27/wp-a6/fit
LOG=$F/submit_log_${TAG}.txt
SUB=$F/fit_wpa6.sub
FROZEN=$D/data/restart_2026-10-27/wp-a6/range_frozen.json
CMD="condor_submit QFILE=$Q TAG=$TAG $SUB"
[ -f "$Q" ] || { echo "missing queue $Q" >&2; exit 1; }
mkdir -p "$F/logs/$TAG"
{
  echo "WP-A6 fit submit log (tag $TAG)"
  echo "utc_prepared: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "command: $CMD"
  echo "queue_rows: $(wc -l < "$Q")"
  echo "md5 script: $(md5sum $D/scripts/restart_2026-10-27/wp-a6/fit_bblite_wpa6.py)"
  echo "md5 wrapper: $(md5sum $F/fit_wpa6_job.sh)"
  echo "md5 sub: $(md5sum $SUB)"
  echo "md5 queue: $(md5sum $Q)"
  echo "payload sha256:"
  cut -d, -f8 "$Q" | sort -u | while read -r p; do sha256sum "$p"; done
} > "$LOG"
NEEDS=0; { [ "$TAG" = "odd" ] || grep -q '/odd/' "$Q"; } && NEEDS=1
if [ -f "$FROZEN" ]; then
  got=$(sha256sum "$FROZEN" | awk '{print $1}'); exp=$(awk '{print $1}' "$FROZEN.sha256" 2>/dev/null)
  echo "range_frozen sha256 $FROZEN = $got (sidecar $exp)" >> "$LOG"
  if [ "$got" != "$exp" ]; then echo "ABORT: range_frozen.json sha256 mismatch; not submitted" | tee -a "$LOG"; exit 3; fi
  echo "range_frozen check: PASS" >> "$LOG"
elif [ "$NEEDS" = 1 ]; then
  echo "ABORT: odd-split submission needs range_frozen.json; not submitted" | tee -a "$LOG"; exit 4
else
  echo "range_frozen: absent (allowed for tag $TAG)" >> "$LOG"
fi
echo "utc_submit: $(date -u +%Y-%m-%dT%H:%M:%SZ)" >> "$LOG"
$CMD 2>&1 | tee -a "$LOG"
rc=${PIPESTATUS[0]}
echo "condor_submit rc=$rc" >> "$LOG"
exit $rc

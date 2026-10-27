#!/usr/bin/env bash
# submit_s4.sh - WP-A2 S4 G1-prime fit sweep (odd frozen payloads): verify hashes, write submit_log_s4.txt, submit.
set -u
D=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
F=$D/condor/restart_2026-10-27/wp-a2/fit_wpa2
LOG=$F/submit_log_s4.txt
Q=$F/queue_s4.txt
SUB=$F/fit_wpa2.sub
REC=$D/data/restart_2026-10-27/wp-a2/s3_corr/s3_correction_record.json
PLAN=$D/docs/restart_2026-10-27/wp-a2/wp-a2_plan.md
S1=$D/docs/restart_2026-10-27/wp-a2/s1_definition_audit.md
EXP_REC=4b2b5e269a1f8d07f22d34a24c9416742eba53ebb6da23b85014bcf3bf79d719
EXP_PLAN=c553d2007df95fbd78d386a96636328d6434cf21c1171b717dd9a67eb2225b0d
EXP_S1=bb95183d9a12e8baf49157e25e4c72f1df9a2c1372eb63053d35f09a6d34bd23
mkdir -p "$F/logs/s4"
got_rec=$(sha256sum "$REC" | awk '{print $1}')
got_plan=$(sha256sum "$PLAN" | awk '{print $1}')
got_s1=$(sha256sum "$S1" | awk '{print $1}')
CMD="condor_submit QFILE=$Q TAG=s4 $SUB"
{
  echo "WP-A2 S4 fit sweep submit log"
  echo "utc_prepared: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "command: $CMD"
  echo "queue_rows: $(wc -l < "$Q")"
  echo "sha256 record $REC = $got_rec (expected $EXP_REC)"
  echo "sha256 plan $PLAN = $got_plan (expected $EXP_PLAN)"
  echo "sha256 s1   $S1 = $got_s1 (expected $EXP_S1)"
  echo "md5 script: $(md5sum $D/scripts/restart_2026-10-27/wp-a2/fit_7p1_wpa2.py)"
  echo "md5 wrapper: $(md5sum $F/fit_wpa2_job.sh)"
  echo "md5 sub: $(md5sum $SUB)"
  echo "md5 queue: $(md5sum $Q)"
  echo "payload sha256 (3):"
  cut -d, -f6 "$Q" | sort -u | while read -r p; do sha256sum "$p"; done
} > "$LOG"
if [ "$got_rec" != "$EXP_REC" ] || [ "$got_plan" != "$EXP_PLAN" ] || [ "$got_s1" != "$EXP_S1" ]; then
  echo "ABORT: hash mismatch; not submitted" | tee -a "$LOG"; exit 3
fi
echo "hash check: PASS" >> "$LOG"
echo "utc_submit: $(date -u +%Y-%m-%dT%H:%M:%SZ)" >> "$LOG"
$CMD 2>&1 | tee -a "$LOG"
rc=${PIPESTATUS[0]}
echo "condor_submit rc=$rc" >> "$LOG"
exit $rc

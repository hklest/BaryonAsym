#!/usr/bin/env bash
# submit_build.sh - WP-A6 S1 payload build: write submit_log_build.txt, submit.
set -u
D=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
F=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/condor/restart_2026-10-27/wp-a6/build
LOG=$F/submit_log_build.txt; Q=$F/queue_build.txt; SUB=$F/build_wpa6.sub
CMD="condor_submit QFILE=$Q $SUB"
{
  echo "WP-A6 S1 payload build submit log"
  echo "utc_prepared: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "command: $CMD"
  echo "queue_rows: $(wc -l < "$Q")"
  echo "md5 script: $(md5sum $D/scripts/restart_2026-10-27/wp-a6/build_payload_wpa6.py)"
  echo "md5 wrapper: $(md5sum $F/build_wpa6_job.sh)"
  echo "md5 sub: $(md5sum $SUB)"
  echo "md5 queue: $(md5sum $Q)"
  echo "skim sha256 (17):"
  sha256sum $D/data/restart_2026-10-27/wp-a3/skims/*.npz
  echo "utc_submit: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
} > "$LOG"
$CMD 2>&1 | tee -a "$LOG"
rc=${PIPESTATUS[0]}
echo "condor_submit rc=$rc" >> "$LOG"
exit $rc

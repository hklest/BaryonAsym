#!/usr/bin/env bash
# Submit the WP-A6 track builds; append UTC stamp, command and md5s to submit_log_trk.txt.
C=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/condor/restart_2026-10-27/wp-a6/trk
L=$C/submit_log_trk.txt
{ echo "== $(date -u +%Y-%m-%dT%H:%M:%SZ)"; echo "condor_submit $C/trk_wpa6.sub"; md5sum $C/trk_wpa6.sub $C/trk_wpa6_job.sh $C/queue_trk.txt /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-a6/wpc_build_bkg_wpa6.py; } >> $L
condor_submit $C/trk_wpa6.sub 2>&1 | tee -a $L

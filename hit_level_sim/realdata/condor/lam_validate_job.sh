#!/bin/bash
# WP1 validation over the 38 written lam_q2e5 files: schema parity vs MC0, overlays, counter aggregation.
D=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata
H=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim
S=$H/production/realdata_2007ep/lam_q2e5/hits
MC0=$H/production/cov_loQ2/loQ2/hits/hits_part_0.root
desygroupset h1 >/dev/null 2>&1; source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; type desygroupset >/dev/null 2>&1 && desygroupset h1 >/dev/null 2>&1
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
export PYTHONDONTWRITEBYTECODE=1
python3 $D/validation/check_schema.py --mc $MC0 --data "$S/hits_part_c07*.root" --out-json $D/validation/schema_report_lam.json
python3 $D/validation/overlay_data_mc.py --mc $MC0 --data "$S/hits_part_c07*.root" --plots-dir $D/validation/plots/wp1_overlays \
  --out-json $D/validation/wp1_overlay_summary.json
python3 $D/validation/aggregate_counters.py "$S/hits_part_c07*.root" $D/validation/wp1_counters_lam.json

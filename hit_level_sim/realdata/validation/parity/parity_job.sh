#!/bin/bash
# WP0 parity, one Condor job. Usage: parity_job.sh data <lamdir-name> | compare | data_t0 <lamdir-name> | compare_t0
P=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/validation/parity
MC0=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/production/cov_loQ2/loQ2/hits/hits_part_0.root
desygroupset h1 >/dev/null 2>&1; source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; type desygroupset >/dev/null 2>&1 && desygroupset h1 >/dev/null 2>&1
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
export PYTHONDONTWRITEBYTECODE=1
set -e
case "$1" in
  data)    python3 $P/parity_cjc.py data --lam-dir /data/dust/group/h1/klesthen/rawhit_probe/lam/$2 \
             --runlist $P/runs_selectedruns_lam.txt --out $P/work/$2.npz ;;
  compare) python3 $P/parity_cjc.py mc --hits $MC0 --out $P/work/mc_hits_part_0.npz
           python3 $P/parity_cjc.py compare --mc $P/work/mc_hits_part_0.npz --data $P/work/c07*.npz \
             --out $P/parity.json --plots $P/plots ;;
  data_t0) mkdir -p $P/work/t0
           python3 $P/parity_cjc.py data --lam-dir /data/dust/group/h1/klesthen/rawhit_probe/lam/$2 \
             --runlist $P/runs_selectedruns_lam.txt --t0 hat --det cjc --out $P/work/t0/$2.npz ;;
  compare_t0) python3 $P/parity_cjc.py compare_t0 --mc $P/work/mc_hits_part_0.npz --data-wp0 $P/work/c07*.npz \
             --data-t0 $P/work/t0/c07*.npz --out $P/parity_t0.json --plots $P/plots ;;
  *) echo "usage: $0 data <dir> | compare | data_t0 <dir> | compare_t0"; exit 2 ;;
esac

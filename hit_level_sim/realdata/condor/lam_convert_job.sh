#!/bin/bash
# WP1 hits writer, one Condor job per lam/ dir. Usage: lam_convert_job.sh <lam-dir-name> [extra write_hits.py args]
D=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata
OUT=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/production/realdata_2007ep/lam_q2e5/hits
desygroupset h1 >/dev/null 2>&1; source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; type desygroupset >/dev/null 2>&1 && desygroupset h1 >/dev/null 2>&1
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
export PYTHONDONTWRITEBYTECODE=1
mkdir -p $OUT
F=$1; shift
python3 $D/writer/write_hits.py --lam-dir /data/dust/group/h1/klesthen/rawhit_probe/lam/$F \
  --runlist-table $D/writer/runlist/runlist_lam.txt --out $OUT/hits_part_$F.root "$@"

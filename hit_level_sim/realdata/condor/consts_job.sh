#!/bin/bash
# WP3: per-run NDB constants for one mODS file with calibprobe_consts (first usable event of each run).
# usage: consts_job.sh <period: 2005|2006|07low|07med> <file stem cYYNNNN> [hat selection, default Q2e>5] [max events]
# output: $D/consts/<period>/by_file/<file>/{consts_<run>.json, runs.tsv, hat_runs.json, job.log, DONE}
period=$1; f=$2; SEL=${3:-Q2e>5}; N=$4
case $period in 2005) y=05;; 2006) y=06;; 07low) y=07low;; 07med) y=07med;; *) echo "bad period $period"; exit 2;; esac
R=/data/dust/group/h1/klesthen/rawhit_probe; D=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata
P=/pnfs/desy.de/dphep/online/h1/data/oo-4.0/$y
O=$D/consts/$period/by_file/$f; mkdir -p $O; rm -f $O/DONE
W=$(mktemp -d ${_CONDOR_SCRATCH_DIR:-${TMPDIR:-/tmp}}/consts_$f.XXXXXX)
desygroupset h1 >/dev/null 2>&1; source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; type desygroupset >/dev/null 2>&1 && desygroupset h1 >/dev/null 2>&1
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
cat > $W/s.steer <<S
H1SteerTree() {
  fHatFiles  = "$P/hat.4.0.6.10dst7.$f.root";
  fModsFiles = "$P/mods.4.0.6.10dst7.$f.root";
  fHatSelection = "$SEL";
}
S
cd $W
$R/calib_consts/bin/calibprobe_consts -f s.steer ${N:+-n $N} > job.log 2>&1
echo "calibprobe_consts exit $?" >> job.log
for c in calib_*.txt; do
  [ -e "$c" ] || continue
  r=${c#calib_}; r=${r%.txt}
  python3 -I $R/calib_consts/py/make_consts.py . $r > /dev/null 2>> job.log || echo "make_consts failed run $r" >> job.log
done
python3 $D/consts/scripts/hat_runs.py $P/hat.4.0.6.10dst7.$f.root "$SEL" > hat_runs.json 2>> job.log
python3 -I $D/consts/scripts/file_summary.py . $f > runs.tsv 2>> job.log
cp -p consts_*.json $O/ 2>/dev/null; cp -p job.log runs.tsv hat_runs.json s.steer $O/
cd / && rm -rf $W
echo done > $O/DONE

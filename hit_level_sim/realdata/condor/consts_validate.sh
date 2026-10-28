#!/bin/bash
# WP3 period validation dump (D16): unchanged bin/calibprobe, full dump of the first N Q2e>5 events of one mODS file.
# usage: consts_validate.sh <period> <file> [N=1500] [hat selection, default Q2e>5] -> $D/consts/<period>/validation/<file>/{events.txt,consts_<run>.json,job.log}
period=$1; f=$2; N=${3:-1500}; SEL=${4:-Q2e>5}
case $period in 2005) y=05;; 2006) y=06;; 07low) y=07low;; 07med) y=07med;; *) echo "bad period"; exit 2;; esac
R=/data/dust/group/h1/klesthen/rawhit_probe; D=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata
P=/pnfs/desy.de/dphep/online/h1/data/oo-4.0/$y; O=$D/consts/$period/validation/$f; mkdir -p $O; rm -f $O/DONE; cd $O
desygroupset h1 >/dev/null 2>&1; source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; type desygroupset >/dev/null 2>&1 && desygroupset h1 >/dev/null 2>&1
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
cat > s.steer <<S
H1SteerTree() {
  fHatFiles  = "$P/hat.4.0.6.10dst7.$f.root";
  fModsFiles = "$P/mods.4.0.6.10dst7.$f.root";
  fHatSelection = "$SEL";
}
S
$R/calib_consts/bin/calibprobe -f s.steer -n $N 2>&1 | head -c 20000000 > job.log
for c in calib_*.txt; do r=${c#calib_}; r=${r%.txt}; python3 -I $R/calib_consts/py/make_consts.py . $r > /dev/null 2>&1; done
rm -f banks_*.txt calib_*.txt
echo done > DONE

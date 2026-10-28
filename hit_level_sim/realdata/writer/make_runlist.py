#!/usr/bin/env python3
"""Dump the D17 run-list and run-quality table for a set of runs to text (PyROOT + H1OO libs; run once).

Usage (after `desygroupset h1` and thish1.sh):  make_runlist.py --lam-root <dir with c07*/> --out runlist/runlist_lam.txt
Columns: run selected quality
  selected = H1RunList::IsSelected(run) of SelectedRuns.root (oolumi product, good+medium runs, CJC/CIP/LAr/ToF/Lumi/BDC HV)
  quality  = H1RunQuality::ERunQual from the NDB snapshot (0 unknown, 1 good, 2 medium, 3 poor; oolumi.C:300-313)
The writer reads this table, so it never loads the H1OO libraries.
"""
import argparse, glob, os, subprocess
import ROOT

RUNLIST_ROOT = '/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym/SelectedRuns.root'

ap = argparse.ArgumentParser()
ap.add_argument('--lam-root', default='/data/dust/group/h1/klesthen/rawhit_probe/lam')
ap.add_argument('--out', required=True)
a = ap.parse_args()

ROOT.gSystem.Load('libEG')
ldd = subprocess.run(['bash', '-c', 'ldd $(which oolumi)'], capture_output=True, text=True).stdout
for l in ldd.splitlines():
    if '/h1oo/lib/' in l and '=>' in l:
        ROOT.gSystem.Load(l.split()[2])
rl = ROOT.TFile.Open(RUNLIST_ROOT).Get('H1RunList')
db = ROOT.H1DBManager.Instance(False, False)
rq = db.GetDBEntry(ROOT.H1RunQuality.Class())
runs = sorted({int(os.path.basename(p)[7:-5]) for p in glob.glob(f'{a.lam_root}/c07*/calib/consts_*.json')})
with open(a.out, 'w') as fo:
    fo.write(f'# run selected quality  (selected: H1RunList::IsSelected of {RUNLIST_ROOT}; '
             f'quality: H1RunQuality::ERunQual 0 unknown 1 good 2 medium 3 poor)\n')
    for r in runs:
        db.StartRun(r)
        fo.write(f'{r} {int(bool(rl.IsSelected(r)))} {int(rq.Quality(r))}\n')
print('runs', len(runs), 'rejected', [r for r in runs if not rl.IsSelected(r)])

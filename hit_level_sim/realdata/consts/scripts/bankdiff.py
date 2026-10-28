#!/usr/bin/env python3
"""Which raw_ndb_banks vary across the runs of one period (model: research §3), and the C0J8 Lorentz word.
usage: python3 -I bankdiff.py <period>  -> writes $D/consts/<period>/bankdiff.txt"""
import sys, os, json, glob
period = sys.argv[1]
base = f'/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/consts/{period}'
fs = sorted(glob.glob(os.path.join(base, 'consts_*.json')), key=lambda p: int(os.path.basename(p)[7:-5]))
ref, refrun = None, None
nvar, nmiss, present = {}, {}, {}
lor = []
cozempty = []   # COZ set-up absent in the dumped event (coz TEND all 0); seen with C3Z8 missing
for p in fs:
    run = int(os.path.basename(p)[7:-5]); j = json.load(open(p))
    b = j['raw_ndb_banks']; a = j['cjc']['ARC0J8']
    lor.append((run, a[5], a[5] - a[29], a[5] + a[29]))   # ARC0J8(6) and ARC0J8(6) -/+ ARC0J8(30)
    if not any(j['coz'].get('TEND', [])): cozempty.append((run, 'C3Z8' in b))
    if ref is None:
        ref, refrun = b, run
    for n in set(ref) | set(b):
        present[n] = present.get(n, 0) + (n in b)
        if n not in b or n not in ref: nmiss[n] = nmiss.get(n, 0) + 1
        elif b[n]['words'] != ref[n]['words']: nvar[n] = nvar.get(n, 0) + 1
lo = min(x[1] for x in lor); hi = max(x[1] for x in lor)
with open(os.path.join(base, 'bankdiff.txt'), 'w') as o:
    o.write(f'# period {period}: {len(fs)} runs, reference run {refrun}\n')
    o.write(f'# banks differing from the reference run (count of runs):\n')
    for n in sorted(nvar): o.write(f'VARY {n} {nvar[n]}\n')
    o.write(f'# banks missing in some runs (runs where presence differs from reference):\n')
    for n in sorted(nmiss): o.write(f'MISSING {n} {nmiss[n]} present_in {present[n]}\n')
    o.write(f'# identical in all runs: {" ".join(sorted(n for n in present if n not in nvar and n not in nmiss))}\n')
    o.write(f'# C0J8 Lorentz word ARC0J8(6): min {lo:.5f} max {hi:.5f} rad; within 0.60-0.64: {sum(0.60 <= x[1] <= 0.64 for x in lor)}/{len(lor)}\n')
    ring = [v for x in lor for v in x[2:]]
    o.write(f'# ring Lorentz angles ARC0J8(6)-/+ARC0J8(30), both rings: min {min(ring):.5f} max {max(ring):.5f} rad\n')
    o.write(f'# COZ constants empty (coz TEND all 0): {len(cozempty)}/{len(fs)} runs, of which C3Z8 present: {sum(x[1] for x in cozempty)}\n')
    for x in cozempty: o.write(f'COZ_EMPTY {x[0]} C3Z8 {int(x[1])}\n')
    out = [x for x in lor if not 0.60 <= x[1] <= 0.64]
    for x in out[:50]: o.write(f'LORENTZ_OUT {x[0]} {x[1]:.5f}\n')
print(open(os.path.join(base, 'bankdiff.txt')).read()[:3000])

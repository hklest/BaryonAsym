#!/usr/bin/env python3
"""D16 constants-chain regression for one WP3 validation dump: exact path h1hits CJC.hit3d (ICOR=3) vs h1rec CJXR,
logic of rawhit_probe/validation/consts_periods/validate_xy.py, constants per run from $D/consts/<period>/consts_<run>.json
(the WP3 product; falls back to the dump's own json, recorded per run). Pass: pooled fraction |dxy| < 10 um >= 0.90.
usage (LCG python, not -I): validate_period.py <period> <file>  -> validation/<file>/validate.json and
$D/consts/plots/validation_<period>_<file>.png"""
import sys, os, math, json
sys.dont_write_bytecode = True
sys.path.insert(0, '/data/dust/group/h1/klesthen/rawhit_probe/calib_consts/py')
from h1hits import CJC, load, cjfsxy
from events import read_events, xr_dict, trkp
import numpy as np
import matplotlib; matplotlib.use('Agg'); import matplotlib.pyplot as plt
period, f = sys.argv[1], sys.argv[2]
B = '/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/consts'
d = os.path.join(B, period, 'validation', f)
cjs, src = {}, {}
def getcj(run):
    if run not in cjs:
        p = os.path.join(B, period, f'consts_{run}.json'); src[run] = 'wp3'
        if not os.path.exists(p): p = os.path.join(d, f'consts_{run}.json'); src[run] = 'local'
        cjs[run] = CJC(load(p)) if os.path.exists(p) else None
        if cjs[run] is None: src[run] = 'missing'
    return cjs[run]
per = {}; nev = {}
for ev in read_events(os.path.join(d, 'events.txt')):
    if not ev['XR']: continue
    cj = getcj(ev['run'])
    if cj is None: continue
    nev[ev['run']] = nev.get(ev['run'], 0) + 1
    H = ev['H']; bywire = {}
    for i, h in enumerate(H): bywire.setdefault(h[0], []).append(i)
    for r in ev['XR']:
        x = xr_dict(r)
        if x['kdr'] <= 0: continue
        row = H[x['kdr'] - 1]; lst = bywire[row[0]]; jh = lst[0]
        for kk in lst[1:]:
            if H[kk][1] == row[1]: jh = kk
        pos = lst.index(jh)
        dtprv = H[lst[pos - 1]][1] - row[1] if pos > 0 else -9999.9
        dtnxt = H[lst[pos + 1]][1] - row[1] if pos < len(lst) - 1 else 9999.9
        t = trkp(ev, x['itr'], cjfsxy)
        t0 = t['t0'] if t['t0'] != 0.0 else ev['cjcc_t0']
        hexp = {'drift': x['DRFE'], 'tlen': x['TLEN'], 'cosb': x['COSB'], 'sinb': x['SINB'], 'slen': x['SLEN'],
                'dcor': x['DCOR'], 'zcor': x['ZCOR'], 'bz': x['BZ']}
        h = cj.hit3d(row, hexp, t, t0, ilng=None, dtprv=dtprv, dtnxt=dtnxt, icor=3)
        per.setdefault(ev['run'], []).append(math.hypot(h['x'] - x['XHIT'], h['y'] - x['YHIT']) * 1e4)
allv = np.concatenate([np.array(v) for v in per.values()]) if per else np.array([])
frac = lambda v, thr: float(np.mean(np.asarray(v) < thr)) if len(v) else float('nan')
res = {'period': period, 'file': f, 'events_with_xr': sum(nev.values()), 'hits': int(len(allv)),
       'median_um': float(np.median(allv)) if len(allv) else None, 'frac_lt_1um': frac(allv, 1), 'frac_lt_10um': frac(allv, 10),
       'frac_lt_100um': frac(allv, 100), 'threshold': 0.90, 'pass': bool(len(allv) and frac(allv, 10) >= 0.90),
       'per_run': {str(r): {'events': nev[r], 'hits': len(v), 'frac_lt_10um': frac(v, 10), 'consts': src[r],
                            'ivrsqt': 3 if r >= 452777 else 2} for r, v in sorted(per.items())},
       'missing_consts': sorted(r for r, s in src.items() if s == 'missing')}
json.dump(res, open(os.path.join(d, 'validate.json'), 'w'), indent=1)
os.makedirs(os.path.join(B, 'plots'), exist_ok=True)
png = os.path.join(B, 'plots', f'validation_{period}_{f}.png')
fig, ax = plt.subplots(1, 2, figsize=(11, 4.2))
if len(allv):
    s = np.sort(np.clip(allv, 1e-3, None)); ax[0].plot(s, np.arange(1, len(s) + 1) / len(s), color='#1f5fa8')
ax[0].set_xscale('log'); ax[0].axvline(10, color='0.4', ls='--'); ax[0].axhline(0.90, color='#c0392b', ls=':')
ax[0].set_xlabel('|Δxy| h1hits exact path − CJXR [µm]'); ax[0].set_ylabel('cumulative fraction of hits')
ax[0].set_title(f'{period} {f}: {len(allv)} hits, f(<10 µm) = {res["frac_lt_10um"]:.3f}')
rr = sorted(per); fr = [frac(per[r], 10) for r in rr]
ax[1].bar(range(len(rr)), fr, color='#1f5fa8'); ax[1].axhline(0.90, color='#c0392b', ls=':', label='threshold 0.90')
ax[1].set_xticks(range(len(rr))); ax[1].set_xticklabels([str(r) for r in rr], rotation=60, fontsize=7)
ax[1].set_ylim(0, 1.0); ax[1].set_ylabel('fraction |Δxy| < 10 µm'); ax[1].set_xlabel('run'); ax[1].legend(loc='lower right')
ax[1].set_title('per run (hits ≥ 1)')
fig.tight_layout(); fig.savefig(png, dpi=110)
print(json.dumps({k: v for k, v in res.items() if k != 'per_run'}), png)

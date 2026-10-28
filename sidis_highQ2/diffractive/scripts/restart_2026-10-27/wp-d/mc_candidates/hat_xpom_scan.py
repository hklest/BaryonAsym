#!/usr/bin/env python3
"""WP-D round 3 (h1-mc-expert 2026-10-28): HAT-only full-range x_IP / M_Y / |t| / beta histograms at Q2Gki>20, 0.05<y<0.8
(inside both generated phase spaces) to test for Reggeon or p-dissociation content: 840x vs 9012.
Usage: hat_xpom_scan.py <label> <hatdir> <out.json>"""
import sys, os, json, glob
import numpy as np, uproot
label, hatdir, out = sys.argv[1:4]
files = sorted(glob.glob(os.path.join(hatdir, 'hat.*.root')))
XB = [0, 0.001, 0.003, 0.01, 0.02, 0.03, 0.05, 0.07, 0.1, 0.15, 0.2, 0.3]; MYB = [0, 1.0, 1.2, 1.6, 2, 3, 5, 10, 30, 1e9]
TB = [0, 0.1, 0.2, 0.4, 0.7, 1.0, 2.0, 5.0, 100]; BB = [0, 0.01, 0.04, 0.1, 0.2, 0.4, 0.65, 0.9, 1.0001, 10]
H = {'n': 0, 'n_sel': 0, 'xpom': np.zeros(len(XB) - 1), 'xpom_R': np.zeros(len(XB) - 1), 'my': np.zeros(len(MYB) - 1), 'my_R': np.zeros(len(MYB) - 1),
     't': np.zeros(len(TB) - 1), 'beta': np.zeros(len(BB) - 1), 'my_gt1p6_xpomlt0p03': 0, 'ny_proton_like': 0, 'n_R': 0,
     'xpom_my_lt1p6': np.zeros(len(XB) - 1), 'xpom_my_ge1p6': np.zeros(len(XB) - 1)}
for f in files:
    t = uproot.open(f)['HAT']
    b = {k: t[k].array(library='np').astype(np.float64) for k in ['Q2Gki', 'YGki', 'XpomGki', 'TGki', 'MYGen', 'XGki', 'MXGen']}
    H['n'] += len(b['Q2Gki'])
    s = (b['Q2Gki'] > 20) & (b['YGki'] > 0.05) & (b['YGki'] < 0.8); H['n_sel'] += int(s.sum())
    R = (b['Q2Gki'] > 150) & (b['YGki'] > 0.1) & (b['YGki'] < 0.9) & (b['XpomGki'] < 0.03); H['n_R'] += int(R.sum())
    xp = b['XpomGki']; my = b['MYGen']; beta = np.where(xp > 0, b['XGki'] / np.maximum(xp, 1e-9), -1)
    H['xpom'] += np.histogram(xp[s], bins=XB)[0]; H['xpom_R'] += np.histogram(xp[R], bins=XB)[0]
    H['my'] += np.histogram(my[s], bins=MYB)[0]; H['my_R'] += np.histogram(my[R], bins=MYB)[0]
    H['t'] += np.histogram(np.abs(b['TGki'][s]), bins=TB)[0]; H['beta'] += np.histogram(beta[R], bins=BB)[0]
    H['xpom_my_lt1p6'] += np.histogram(xp[s & (my < 1.6)], bins=XB)[0]; H['xpom_my_ge1p6'] += np.histogram(xp[s & (my >= 1.6)], bins=XB)[0]
    H['my_gt1p6_xpomlt0p03'] += int((s & (my >= 1.6) & (xp < 0.03)).sum()); H['ny_proton_like'] += int((s & (np.abs(my - 0.938) < 0.02)).sum())
    print(label, os.path.basename(f), H['n'], flush=True)
H = {k: (v.tolist() if isinstance(v, np.ndarray) else v) for k, v in H.items()}; H.update({'label': label, 'files': len(files), 'XB': XB, 'MYB': MYB, 'TB': TB, 'BB': BB})
json.dump(H, open(out, 'w'), indent=1); print('done', label)

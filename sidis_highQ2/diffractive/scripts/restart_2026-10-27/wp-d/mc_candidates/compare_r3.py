#!/usr/bin/env python3
"""WP-D round 3 (h1-mc-expert 2026-10-28): 840x (gens 5643/5644) vs 9012 (gen 4489) gen-level comparison in
R = {Q2>150, 0.1<y<0.9, x_IP<0.03} and F = R & {4<M_X<40, M_Y<1.6, |t|<1} from the McCandidateXScan trees.
Outputs: tables (json + txt) in data/.../mc_candidates/r3/, 4 PNG + captions.md in plots/.../mc_candidates/."""
import json, glob, os, sys
import numpy as np, uproot
import matplotlib; matplotlib.use('Agg'); import matplotlib.pyplot as plt
B = '/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive'
R3 = B + '/data/restart_2026-10-27/wp-d/mc_candidates/r3/'; PL = B + '/plots/restart_2026-10-27/wp-d/mc_candidates/'; os.makedirs(PL, exist_ok=True)
SIG = {'9012': 22972.115, '8405': 30209.441, '8403': 30275.195}; NGEN = {'9012': 1199996, '8405': 9999998, '8403': 9999998}   # gen-log Tot x-sect [pb], DB N
LEQ = {k: NGEN[k] / SIG[k] for k in SIG}                                                                                       # pb^-1
COL = {'9012': 'k', '8405': 'r', '8403': 'b', '8402': 'orange', '8404': 'c'}; LAB = {'9012': '9012 (gen 4489, 06/07 e+)', '8405': '8405 (gen 5644, 06/07 e+)', '8403': '8403 (gen 5643, 04/05 e-)', '8402': '8402 (gen 5644, 03/04 e+)', '8404': '8404 (gen 5643, 2006 e-)'}
EV = ['run', 'event', 'w', 'genrad', 'q2', 'y', 'x', 'xpom', 't', 'mx', 'my', 'beta', 'pt2hat', 'etamaxhat', 'vtxz', 'mxR', 'etaX0', 'etaX04', 'etaX08', 'etaX15', 'nX', 'nY', 'nchX', 'nchXfid', 'hasCharm', 'nIsr', 'nH']
HD = ['hQ', 'hEta', 'hPt', 'hXf', 'hPt2s', 'hZs']
def load(req):
    fs = sorted(glob.glob(R3 + 'xscan_%s_p*.root' % req)); d = {k: [] for k in EV + HD}
    for f in fs:
        t = uproot.open(f)['xscan']; a = t.arrays(EV + HD, library='np')
        for k in EV + HD: d[k].append(a[k])
    d = {k: np.concatenate(v) for k, v in d.items()}; d['files'] = len(fs); return d
D = {r: load(r) for r in ['9012', '8405', '8403', '8402', '8404']}
for r, d in D.items(): d['F'] = (d['mx'] > 4) & (d['mx'] < 40) & (d['my'] < 1.6) & (np.abs(d['t']) < 1); d['Fnot'] = (d['mx'] > 4) & (d['mx'] < 40) & (d['my'] < 1.6)
out = {}; txt = []
def P(*a): s = ' '.join(str(x) for x in a); print(s); txt.append(s)
P('# Round 3 tables (h1-mc-expert 2026-10-28). L_eq [pb^-1]:', {k: round(v, 3) for k, v in LEQ.items()})
# ---- T1 sigma_R, sigma_F
out['T1'] = {}
for r in ['9012', '8405', '8403']:
    d = D[r]; nR = len(d['mx']); nF = int(d['F'].sum()); nFn = int(d['Fnot'].sum())
    out['T1'][r] = dict(nR=nR, sigR=nR / LEQ[r], nF=nF, sigF=nF / LEQ[r], nF_not=nFn, sigF_not=nFn / LEQ[r], files=d['files'])
    P('T1 %s files=%d N_R=%d sigma_R=%.2f pb  N_F=%d sigma_F=%.2f pb  (F w/o |t|<1: N=%d sigma=%.2f)' % (r, d['files'], nR, nR / LEQ[r], nF, nF / LEQ[r], nFn, nFn / LEQ[r]))
for r in ['8405', '8403']:
    for reg in ('R', 'F'):
        a, b = out['T1'][r]['n' + reg], out['T1']['9012']['n' + reg]; q = (a / LEQ[r]) / (b / LEQ['9012']); e = q * np.sqrt(1 / a + 1 / b)
        out['T1'][r]['ratio_' + reg] = [q, e]; P('   sigma_%s(%s)/sigma_%s(9012) = %.4f +- %.4f' % (reg, r, reg, q, e))
# ---- T2 ratio vs variables (sigma-normalised) in F, chi2 vs flat(global) and vs 1
VARS = {'beta': [0, 0.04, 0.1, 0.2, 0.35, 0.5, 0.65, 0.8, 1.0], 'xpom': [0, 0.003, 0.006, 0.01, 0.015, 0.02, 0.025, 0.03], 'q2': [150, 200, 250, 300, 400, 550, 800, 1500, 5000],
        'y': [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9], 'mx': [4, 6, 8, 10, 13, 16, 20, 25, 30, 40], 't': [0, 0.05, 0.1, 0.2, 0.3, 0.45, 0.7, 1.0]}
XL = {'beta': r'$\beta = x/x_{IP}$', 'xpom': r'$x_{IP}$', 'q2': r'$Q^2$ [GeV$^2$]', 'y': r'$y$', 'mx': r'$M_X$ [GeV]', 't': r'$|t|$ [GeV$^2$]'}
def var(d, v): return np.abs(d['t']) if v == 't' else d[v]
def hist(d, v, m, bins): return np.histogram(var(d, v)[m], bins=bins)[0].astype(float)
out['T2'] = {}
for r in ['8405', '8403']:
    out['T2'][r] = {}; g = out['T1'][r]['ratio_F'][0]
    for v, bins in VARS.items():
        a = hist(D[r], v, D[r]['F'], bins); b = hist(D['9012'], v, D['9012']['F'], bins); q = (a / LEQ[r]) / np.maximum(b, 1) * LEQ['9012']; e = q * np.sqrt(1 / np.maximum(a, 1) + 1 / np.maximum(b, 1))
        ok = (a > 0) & (b > 0); c2g = float(np.sum(((q - g) / e)[ok] ** 2)); c2one = float(np.sum(((q - 1) / e)[ok] ** 2)); ndf = int(ok.sum())
        # linear slope test on ratio/g vs bin centre (normalised to range)
        xc = 0.5 * (np.array(bins[1:]) + np.array(bins[:-1])); xn = (xc - xc.min()) / (xc.max() - xc.min()); W = 1 / e[ok] ** 2; X = xn[ok]; Y = (q / g)[ok]
        Sw, Sx, Sy, Sxx, Sxy = W.sum(), (W * X).sum(), (W * Y).sum(), (W * X * X).sum(), (W * X * Y).sum(); den = Sw * Sxx - Sx ** 2; slope = (Sw * Sxy - Sx * Sy) / den; eslope = np.sqrt(Sw / den)
        out['T2'][r][v] = dict(bins=bins, n840=a.tolist(), n9012=b.tolist(), ratio=q.tolist(), err=e.tolist(), chi2_flat=c2g, chi2_one=c2one, ndf=ndf, slope=[float(slope), float(eslope)])
        P('T2 %s vs %-5s ratio/global: %s  chi2(flat)/ndf=%.1f/%d  chi2(=1)/ndf=%.1f/%d  slope=%.3f+-%.3f' % (r, v, ' '.join('%.3f' % x for x in q / g), c2g, ndf, c2one, ndf, slope, eslope))
# ---- T3 (beta, xpom) reweight table 9012/840x in F
BB = [0, 0.1, 0.3, 0.6, 1.0]; XB = [0, 0.006, 0.012, 0.02, 0.03]; out['T3'] = {}
for r in ['8405', '8403']:
    a = np.histogram2d(D[r]['beta'][D[r]['F']], D[r]['xpom'][D[r]['F']], bins=[BB, XB])[0]; b = np.histogram2d(D['9012']['beta'][D['9012']['F']], D['9012']['xpom'][D['9012']['F']], bins=[BB, XB])[0]
    w = (b / LEQ['9012']) / np.maximum(a / LEQ[r], 1e-9); ew = w * np.sqrt(1 / np.maximum(a, 1) + 1 / np.maximum(b, 1)); out['T3'][r] = dict(BB=BB, XB=XB, w_9012_over_840x=w.tolist(), err=ew.tolist(), n840=a.tolist(), n9012=b.tolist())
    P('T3 %s reweight w = sigma_9012/sigma_%s per (beta row x x_IP col) cell:' % (r, r))
    for i in range(len(BB) - 1): P('    beta[%.1f,%.1f): ' % (BB[i], BB[i + 1]) + '  '.join('%.3f+-%.3f' % (w[i, j], ew[i, j]) for j in range(len(XB) - 1)))
# ---- T4 eta_max, multiplicities per M_X bin
MXB = [4, 10, 20, 40]; out['T4'] = {}
for r in ['9012', '8405', '8403']:
    d = D[r]; m = d['F']; o = {}
    for thr in ('etaX0', 'etaX04', 'etaX08', 'etaX15', 'etamaxhat'):
        o[thr] = dict(mean=float(d[thr][m].mean()), f_lt3p2=float((d[thr][m] < 3.2).mean()), f_lt3p3=float((d[thr][m] < 3.3).mean()), f_lt2p5=float((d[thr][m] < 2.5).mean()))
    o['etaX0_minus_hat_frac_gt0p1'] = float(((d['etaX0'] - d['etamaxhat'])[m] > 0.1).mean())
    o['per_mx'] = []
    for i in range(3):
        mm = m & (d['mx'] >= MXB[i]) & (d['mx'] < MXB[i + 1]); o['per_mx'].append(dict(mx=[MXB[i], MXB[i + 1]], n=int(mm.sum()), nchX=float(d['nchX'][mm].mean()), nchXfid=float(d['nchXfid'][mm].mean()), etaX04=float(d['etaX04'][mm].mean()), f_eta04_lt3p2=float((d['etaX04'][mm] < 3.2).mean()), charm=float(d['hasCharm'][mm].mean())))
    out['T4'][r] = o
    P('T4 %s F: <etaX04>=%.3f f(<3.2)=%.3f f(<3.3)=%.3f | <etaX08>=%.3f f(<3.2)=%.3f | <EtaMaxGen>=%.3f f(<3.2)=%.3f | etaX0-hat>0.1: %.3f | per-MX nchX/nchXfid/<etaX04>/f(<3.2)/charm: %s' % (
        r, o['etaX04']['mean'], o['etaX04']['f_lt3p2'], o['etaX04']['f_lt3p3'], o['etaX08']['mean'], o['etaX08']['f_lt3p2'], o['etamaxhat']['mean'], o['etamaxhat']['f_lt3p2'], o['etaX0_minus_hat_frac_gt0p1'],
        ['%.2f/%.2f/%.2f/%.3f/%.3f' % (x['nchX'], x['nchXfid'], x['etaX04'], x['f_eta04_lt3p2'], x['charm']) for x in o['per_mx']]))
# ---- T5 hadron spectra in F (hadron fiducial |eta|<1.6, 0.15<pT<15), per-event normalised
XFB = np.linspace(-1, 1, 21); PTB = [0, 0.1, 0.25, 0.5, 1, 2, 4, 8, 16]; ZB = [0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.6, 1.0]
def hadrons(d, m):
    nH = d['nH']; ev = np.repeat(m, nH); h = {k: np.concatenate(d[k])[ev] for k in HD}; fid = (np.abs(h['hEta']) < 1.6) & (h['hPt'] > 0.15) & (h['hPt'] < 15); return {k: v[fid] for k, v in h.items()}, int(m.sum())
out['T5'] = {}
for r in ['9012', '8405', '8403']:
    h, nev = hadrons(D[r], D[r]['F']); pos = h['hXf'] > 0; o = dict(nev=nev, nh=len(h['hXf']), nh_per_ev=len(h['hXf']) / nev, f_xf_pos=float(pos.mean()), f_plus=float((h['hQ'] > 0).mean()),
        xf_hist=np.histogram(h['hXf'], bins=XFB)[0].tolist(), pt2_pos=np.histogram(h['hPt2s'][pos], bins=PTB)[0].tolist(), pt2_neg=np.histogram(h['hPt2s'][~pos], bins=PTB)[0].tolist(),
        z_pos=np.histogram(h['hZs'][pos], bins=ZB)[0].tolist(), z_neg=np.histogram(h['hZs'][~pos], bins=ZB)[0].tolist(), mean_pt2_pos=float(h['hPt2s'][pos].mean()), mean_pt2_neg=float(h['hPt2s'][~pos].mean()), mean_z_pos=float(h['hZs'][pos].mean()), mean_z_neg=float(h['hZs'][~pos].mean()), mean_xf=float(h['hXf'].mean()))
    o['per_mx'] = []
    for i in range(3):
        mm = D[r]['F'] & (D[r]['mx'] >= MXB[i]) & (D[r]['mx'] < MXB[i + 1]); hh, nn = hadrons(D[r], mm); o['per_mx'].append(dict(nh_per_ev=len(hh['hXf']) / max(nn, 1), f_xf_pos=float((hh['hXf'] > 0).mean()), xf_hist=np.histogram(hh['hXf'], bins=XFB)[0].tolist()))
    out['T5'][r] = o; D[r]['_h'] = h
    P('T5 %s F: n_h/ev=%.3f f(x_F>0)=%.3f f(h+)=%.3f <x_F>=%.4f <pT*2>(+/-)=%.3f/%.3f <z*>(+/-)=%.3f/%.3f | per-MX n_h/ev=%s' % (r, o['nh_per_ev'], o['f_xf_pos'], o['f_plus'], o['mean_xf'], o['mean_pt2_pos'], o['mean_pt2_neg'], o['mean_z_pos'], o['mean_z_neg'], ['%.2f' % x['nh_per_ev'] for x in o['per_mx']]))
for r in ['8405', '8403']:
    for k in ('xf_hist', 'pt2_pos', 'pt2_neg', 'z_pos', 'z_neg'):
        a = np.array(out['T5'][r][k]) / out['T5'][r]['nev']; b = np.array(out['T5']['9012'][k]) / out['T5']['9012']['nev']; q = a / np.maximum(b, 1e-12); e = q * np.sqrt(1 / np.maximum(out['T5'][r][k], 1) + 1 / np.maximum(out['T5']['9012'][k], 1))
        ok = np.array(out['T5']['9012'][k]) > 0; c2 = float(np.sum(((q - 1) / e)[ok] ** 2)); out['T5'][r]['chi2_' + k] = [c2, int(ok.sum())]; P('   %s %s per-event ratio to 9012: chi2(=1)/ndf=%.1f/%d  max|r-1|=%.3f' % (r, k, c2, ok.sum(), np.abs(q - 1)[ok].max()))
# ---- T6 twins: event identity and z-vtx / run
def key(d): return set(zip(d['event'].tolist(), np.round(d['q2'], 2).tolist(), np.round(d['mx'], 2).tolist()))
out['T6'] = {}
for a, b in (('8402', '8405'), ('8404', '8403')):
    ka, kb = key(D[a]), key(D[b]); f = len(ka & kb) / max(len(ka), 1); out['T6'][a + '_in_' + b] = dict(n_a=len(ka), n_a_in_b=len(ka & kb), frac=f); P('T6 %s R-events found event-identical (event#, Q2, M_X) in %s: %d/%d = %.4f' % (a, b, len(ka & kb), len(ka), f))
for r in ['9012', '8402', '8403', '8404', '8405']:
    d = D[r]; q = np.quantile(d['vtxz'], [0.05, 0.16, 0.5, 0.84, 0.95]); out['T6'][r] = dict(vtxz_q=q.tolist(), vtxz_mean=float(d['vtxz'].mean()), vtxz_rms=float(d['vtxz'].std()), run=[int(d['run'].min()), int(d['run'].max())], nrun=int(len(np.unique(d['run']))))
    P('T6 %s vtxz mean/rms=%.2f/%.2f q(5,16,50,84,95)=%s  runs [%d,%d] (%d distinct)' % (r, d['vtxz'].mean(), d['vtxz'].std(), np.round(q, 1).tolist(), d['run'].min(), d['run'].max(), len(np.unique(d['run']))))
# ---- T7 raw-event expectation per M_X bin in F (truth) and x 0.65 [P round-1 reco/truth]
out['T7'] = {}
for r in ['9012', '8405', '8403']:
    d = D[r]; n = np.histogram(d['mx'][d['F']], bins=MXB)[0]; out['T7'][r] = dict(truth=n.tolist(), x0p65=(0.65 * n).round().astype(int).tolist()); P('T7 %s raw truth events in F per M_X bin %s = %s ; x0.65 = %s' % (r, MXB, n.tolist(), (0.65 * n).round().astype(int).tolist()))
json.dump(out, open(R3 + 'compare_r3_tables.json', 'w'), indent=1); open(R3 + 'compare_r3_summary.txt', 'w').write('\n'.join(txt) + '\n')
# ================= figures =================
def panel(ax, axr, bins, series, xlabel, ylabel, logy=False, logx=False, ref='9012', xticks=None):
    bins = np.array(bins, float); xc = 0.5 * (bins[1:] + bins[:-1]); wd = np.diff(bins); nref, sref = series[ref]
    for r, (n, s) in series.items():
        yv = n / s / wd; ey = np.sqrt(n) / s / wd
        if r == ref: ax.fill_between(bins, np.r_[yv, yv[-1]] - np.r_[ey, ey[-1]], np.r_[yv, yv[-1]] + np.r_[ey, ey[-1]], step='post', color='0.75', alpha=0.6, lw=0); ax.step(bins, np.r_[yv, yv[-1]], where='post', color='k', lw=1.2, label=LAB[r])
        else: ax.errorbar(xc, yv, ey, fmt='o', ms=3, color=COL[r], label=LAB[r], lw=1)
        if r != ref:
            q = (n / s) / np.maximum(nref / sref, 1e-12); eq = q * np.sqrt(1 / np.maximum(n, 1) + 1 / np.maximum(nref, 1)); axr.errorbar(xc, q, eq, fmt='o', ms=3, color=COL[r], lw=1)
    rb = np.sqrt(np.maximum(nref, 1)) / np.maximum(nref, 1); axr.fill_between(bins, np.r_[1 - rb, 1 - rb[-1]], np.r_[1 + rb, 1 + rb[-1]], step='post', color='0.75', alpha=0.6, lw=0); axr.axhline(1, color='k', lw=0.8)
    axr.set_ylim(0.7, 1.4); axr.set_xlabel(xlabel); ax.set_ylabel(ylabel); axr.set_ylabel('840x / 9012'); ax.tick_params(labelbottom=False)
    if logy: ax.set_yscale('log')
    if logx: ax.set_xscale('log'); axr.set_xscale('log')
    ax.set_xlim(bins[0], bins[-1]); axr.set_xlim(bins[0], bins[-1])
def grid(nrow, ncol, figsize):
    fig = plt.figure(figsize=figsize); gs = fig.add_gridspec(nrow * 3, ncol, height_ratios=[3, 1.3, 0.7] * nrow, hspace=0.1, wspace=0.3); axs = []
    for i in range(nrow):
        for j in range(ncol): axs.append((fig.add_subplot(gs[3 * i, j]), fig.add_subplot(gs[3 * i + 1, j])))
    for k, (a, b) in enumerate(axs):
        if k > 0: pass
    return fig, axs
SAMP = ['9012', '8405', '8403']
# Fig 1: event kinematics, sigma-normalised (pb per unit) in F
fig, axs = grid(2, 3, (15, 10.5))
for (ax, axr), v in zip(axs, ['beta', 'xpom', 'q2', 'y', 'mx', 't']):
    ser = {r: (hist(D[r], v, D[r]['F'], VARS[v]), LEQ[r]) for r in SAMP}; panel(ax, axr, VARS[v], ser, XL[v], r'd$\sigma$/d(var) [pb / unit], F', logy=(v in ('q2', 't')), logx=(v == 'q2'))
    for r in ['8405', '8403']: axr.axhline(out['T1'][r]['ratio_F'][0], color=COL[r], ls='--', lw=0.8)
axs[0][0].legend(fontsize=8, loc='upper right'); fig.suptitle(r'Round 3: $\sigma$-normalised truth yields in F = {Q$^2$>150, 0.1<y<0.9, x$_{IP}$<0.03, 4<M$_X$<40, M$_Y$<1.6, |t|<1}; dashed = global $\sigma_F$ ratio; grey = 9012 MC-stat band', fontsize=10)
fig.savefig(PL + 'r3_fig1_event_kinematics.png', dpi=130, bbox_inches='tight'); plt.close(fig)
# Fig 2: eta_max and multiplicities, per-event normalised shapes in F
fig, axs = grid(2, 3, (15, 10.5)); EB = np.linspace(-1, 6, 29); NB = np.arange(-0.5, 30.5, 1)
for (ax, axr), (v, bins, xl) in zip(axs, [('etaX04', EB, r'$\eta_{max}$(X), E>0.4 GeV'), ('etaX08', EB, r'$\eta_{max}$(X), E>0.8 GeV'), ('etamaxhat', EB, r'HAT EtaMaxGen (gap-adjacent X particle)'), ('nchX', NB, r'$n_{ch}$(X), all'), ('nchXfid', NB, r'$n_{ch}$(X), |$\eta$|<1.6, 0.15<p$_T$<15, $\pi$/K/p'), ('nX', np.arange(-0.5, 60.5, 2), r'$n_{stable}$(X)')]):
    ser = {r: (np.histogram(D[r][v][D[r]['F']], bins=bins)[0].astype(float), float(D[r]['F'].sum())) for r in SAMP}; panel(ax, axr, bins, ser, xl, '(1/N) dn/d(var), F')
axs[0][0].legend(fontsize=8, loc='upper left'); fig.suptitle(r'Round 3: X-system $\eta_{max}$ (most forward stable GTR particle of X in lab $\eta$, thresholds) and multiplicities, per-event normalised, F', fontsize=10)
fig.savefig(PL + 'r3_fig2_etamax_multiplicity.png', dpi=130, bbox_inches='tight'); plt.close(fig)
# Fig 3: X-frame hadron spectra, per-event normalised, hadron fiducial
fig, axs = grid(2, 3, (15, 10.5))
def hser(k, sel=None, bins=None):
    s = {}
    for r in SAMP:
        h = D[r]['_h']; m = np.ones(len(h['hXf']), bool) if sel is None else sel(h); s[r] = (np.histogram(h[k][m], bins=bins)[0].astype(float), float(out['T5'][r]['nev']))
    return s
specs = [('hXf', None, XFB, r'$x_F$ (Pomeron-positive), all M$_X$', False), ('hPt2s', lambda h: h['hXf'] > 0, PTB, r'$p_T^{*2}$ [GeV$^2$], $x_F>0$ (Pomeron hemisphere)', True), ('hPt2s', lambda h: h['hXf'] < 0, PTB, r'$p_T^{*2}$ [GeV$^2$], $x_F<0$ (photon hemisphere)', True),
         ('hZs', lambda h: h['hXf'] > 0, ZB, r'$z^* = 2E^*/M_X$, $x_F>0$', True), ('hZs', lambda h: h['hXf'] < 0, ZB, r'$z^* = 2E^*/M_X$, $x_F<0$', True), ('hXf', None, XFB, r'$x_F$, h$^+$ only', False)]
specs[5] = ('hXf', lambda h: h['hQ'] > 0, XFB, r'$x_F$, h$^+$ only', False)
for (ax, axr), (k, sel, bins, xl, lg) in zip(axs, specs): panel(ax, axr, bins, hser(k, sel, bins), xl, '(1/N$_{ev}$) dn/d(var), F', logy=lg)
axs[0][0].legend(fontsize=8, loc='upper left'); fig.suptitle(r'Round 3: charged $\pi$/K/p of X (|$\eta$|<1.6, 0.15<p$_T$<15 GeV) in the X rest frame, axis = Pomeron direction (D6), per-event normalised, F', fontsize=10)
fig.savefig(PL + 'r3_fig3_xframe_hadrons.png', dpi=130, bbox_inches='tight'); plt.close(fig)
# Fig 4: (beta, x_IP) reweight map, ratio vs beta split in x_IP, z-vtx and run number per period
fig = plt.figure(figsize=(15, 9)); gs = fig.add_gridspec(2, 3, hspace=0.3, wspace=0.3)
for j, r in enumerate(['8405', '8403']):
    ax = fig.add_subplot(gs[0, j]); w = np.array(out['T3'][r]['w_9012_over_840x']); ew = np.array(out['T3'][r]['err']); im = ax.imshow(w.T, origin='lower', cmap='coolwarm', vmin=0.8, vmax=1.1, aspect='auto')
    for i in range(w.shape[0]):
        for jj in range(w.shape[1]): ax.text(i, jj, '%.3f\n±%.3f' % (w[i, jj], ew[i, jj]), ha='center', va='center', fontsize=8)
    ax.set_xticks(range(len(BB) - 1)); ax.set_xticklabels(['[%g,%g)' % (BB[i], BB[i + 1]) for i in range(len(BB) - 1)]); ax.set_yticks(range(len(XB) - 1)); ax.set_yticklabels(['[%g,%g)' % (XB[i], XB[i + 1]) for i in range(len(XB) - 1)])
    ax.set_xlabel(r'$\beta$'); ax.set_ylabel(r'$x_{IP}$'); ax.set_title(r'w = $\sigma_F$(9012) / $\sigma_F$(%s) per ($\beta$, $x_{IP}$) cell' % r, fontsize=10); fig.colorbar(im, ax=ax)
ax = fig.add_subplot(gs[0, 2])
for r in ['8405', '8403']:
    for lo, hi, mk in ((0, 0.012, 'o'), (0.012, 0.03, 's')):
        a = np.histogram(D[r]['beta'][D[r]['F'] & (D[r]['xpom'] >= lo) & (D[r]['xpom'] < hi)], bins=VARS['beta'])[0]; b = np.histogram(D['9012']['beta'][D['9012']['F'] & (D['9012']['xpom'] >= lo) & (D['9012']['xpom'] < hi)], bins=VARS['beta'])[0]
        q = (a / LEQ[r]) / np.maximum(b / LEQ['9012'], 1e-9); e = q * np.sqrt(1 / np.maximum(a, 1) + 1 / np.maximum(b, 1)); xc = 0.5 * (np.array(VARS['beta'][1:]) + np.array(VARS['beta'][:-1]))
        ax.errorbar(xc + (0.01 if r == '8403' else -0.01), q, e, fmt=mk, ms=4, color=COL[r], label='%s, x_IP in [%g,%g)' % (r, lo, hi), lw=1)
ax.axhline(1, color='k', lw=0.8); ax.set_ylim(0.6, 1.6); ax.set_xlabel(r'$\beta$'); ax.set_ylabel(r'$\sigma_F$(840x)/$\sigma_F$(9012)'); ax.legend(fontsize=7); ax.set_title(r'ratio vs $\beta$, two $x_{IP}$ slices', fontsize=10)
ax = fig.add_subplot(gs[1, 0]); VB = np.linspace(-50, 50, 41)
for r in ['9012', '8402', '8403', '8404', '8405']: n = np.histogram(D[r]['vtxz'], bins=VB)[0]; ax.step(VB, np.r_[n, n[-1]] / n.sum(), where='post', color=COL[r], label='%s: mean %.1f, rms %.1f cm' % (r, D[r]['vtxz'].mean(), D[r]['vtxz'].std()), lw=1.2)
ax.set_xlabel('simulated z-vertex [cm] (HAT VtxZ), events in R'); ax.set_ylabel('fraction'); ax.legend(fontsize=7); ax.set_title('z-vtx per simulated period', fontsize=10)
ax = fig.add_subplot(gs[1, 1])
for r in ['9012', '8402', '8403', '8404', '8405']: ax.hist(D[r]['run'], bins=60, histtype='step', color=COL[r], label='%s: runs %d-%d' % (r, D[r]['run'].min(), D[r]['run'].max()), lw=1.2, density=True)
ax.set_xlabel('simulated run number (events in R)'); ax.set_ylabel('density'); ax.legend(fontsize=7); ax.set_title('run-number spread (run-dependent conditions)', fontsize=10)
ax = fig.add_subplot(gs[1, 2])
for r in ['8405', '8403']:
    a = np.histogram(D[r]['mx'][D[r]['F']], bins=VARS['mx'])[0]; b = np.histogram(D['9012']['mx'][D['9012']['F']], bins=VARS['mx'])[0]; g = out['T1'][r]['ratio_F'][0]
    q = (a / LEQ[r]) / np.maximum(b / LEQ['9012'], 1e-9) / g; e = q * np.sqrt(1 / np.maximum(a, 1) + 1 / np.maximum(b, 1)); xc = 0.5 * (np.array(VARS['mx'][1:]) + np.array(VARS['mx'][:-1])); ax.errorbar(xc, q, e, fmt='o', ms=4, color=COL[r], label='%s / 9012, shape (÷ %.3f)' % (r, g), lw=1)
ax.axhline(1, color='k', lw=0.8); ax.set_ylim(0.8, 1.2); ax.set_xlabel(r'$M_X$ [GeV]'); ax.set_ylabel('shape ratio'); ax.legend(fontsize=8); ax.set_title(r'$M_X$ shape ratio after global normalisation', fontsize=10)
fig.suptitle(r'Round 3: ($\beta$, $x_{IP}$) reweight map, $\beta$ dependence, per-period z-vtx / run simulation, $M_X$ shape', fontsize=10)
fig.savefig(PL + 'r3_fig4_reweight_periods.png', dpi=130, bbox_inches='tight'); plt.close(fig)
print('figures written to', PL)

#!/usr/bin/env python3
"""WP0 data/MC representation parity for the D5 CJC coordinates (plan 2026-10-27-fm4npp-h1-real-data-hits).

Same residual on both samples: the nearer geometric mirror (wire +- |d| * drift direction, no track-angle
term) to the reconstructed CJKR track circle, binned in |cos beta| and arc length, and the charge-division z
(cjc_zcd) against the CJKR helix z at the chosen mirror. MC: cjc_xp/yp/xm/ym/zcd of extract_hits.C
($MC0). Data: CJC.simple() mirrors with the per-run NDB constants of consts_<run>.json (never the MC
geometry of extract_hits.C) and the cjavt0 event T0, on the lam/ dumps; D17 selection (SelectedRuns.root
run list AND H1DetectorStatus::IsOn bits for CJC1, CJC2, COZ from the HAT) applied per event.

Subcommands (run after `desygroupset h1` and thish1.sh; never python3 -I):
  runlist --out <txt>                       run, selected for the lam runs (PyROOT + H1OO libs, once)
  mc      --hits <hits.root> --out <npz>
  data    --lam-dir <dir> --runlist <txt> --out <npz> [--max-events N]
  compare --mc <npz> --data <npz...> --out <parity.json> --plots <dir>
Library use (WP1 validate_data_cjc.py): mc_residuals(hits_file), data_residuals(lam_dirs, runlist), compare(mc, data).
"""
import argparse, glob, json, os, subprocess, sys
import numpy as np

PY = '/data/dust/group/h1/klesthen/rawhit_probe/calib_consts/py'
RUNLIST_ROOT = '/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym/SelectedRuns.root'
LAM = '/data/dust/group/h1/klesthen/rawhit_probe/lam'
# D17 bits (H1DetectorStatus.C setters): CJC1 HV0 + CAEN0; CJC2 HV1 + CAEN1,2; COZ HV3 + CAEN4-6; CJC BBL3 0
HV_BITS, CAEN_BITS, BBL3_BITS = (0, 1, 3), (0, 1, 2, 4, 5, 6), (0,)
COSB_EDGES = np.round(np.arange(0.0, 1.0001, 0.1), 3)
SLEN_EDGES = np.arange(0.0, 150.001, 10.0)
CDF_MM = (0.5, 1.0, 2.0, 5.0)
MIN_HITS = 500
ZCODES = (-191, -181, 181, 191, 201, 301)   # charge-division code values (cjfraw.F / cjfh3d.F); category = index + 1


def zcat(zcd):
    cat = np.zeros(zcd.shape, dtype=np.int8)
    for i, c in enumerate(ZCODES):
        cat[np.abs(zcd - c) < 0.5] = i + 1
    return cat


def pack(r, side_ok, sel=None):
    """Per-hit arrays kept for the compare step (float16 where 1e-3 relative precision suffices)."""
    d = dict(res_mm=r['res_mm'].astype(np.float32), cosb=r['cosb'].astype(np.float16),
             slen=r['slen'].astype(np.float16), dz=r['dz'].astype(np.float32), zcat=zcat(r['zcd']), side_ok=side_ok,
             z130=np.isfinite(r['dz']) & (np.abs(r['zcd']) > 130.0))   # MC codes |z| > 130 cm as +-191, data does not
    if sel is not None:
        d['sel'] = sel
    return d


def residuals(xp, yp, xm, ym, zcd, kappa, phi, theta, dca, z0):
    """Shared geometry for both samples (all numpy, cm). Track circle: n = (-sin phi, cos phi),
    PCA = -dca n, centre = (1/kappa - dca) n (H1 CJKR convention, as in validation/t0/t0_compare.py).
    Returns dict: res_mm (nearer mirror to circle), plus (nearer mirror == '+' mirror), |cos beta|,
    arc length s from the PCA [cm], dz = zcd - (z0 + s cot theta) [cm], zcode (zcd is a code value)."""
    nx, ny = -np.sin(phi), np.cos(phi)
    R = 1.0 / kappa
    cx, cy = (R - dca) * nx, (R - dca) * ny
    aR = np.abs(R)
    dp = np.abs(np.hypot(xp - cx, yp - cy) - aR)
    dm = np.abs(np.hypot(xm - cx, ym - cy) - aR)
    plus = dp <= dm
    px, py = np.where(plus, xp, xm), np.where(plus, yp, ym)
    ux, uy = xp - xm, yp - ym                     # drift direction = difference of the two mirrors
    un = np.hypot(ux, uy)
    ok = un > 1e-6
    un = np.where(ok, un, 1.0); ux, uy = ux / un, uy / un
    rx, ry = px - cx, py - cy
    rn = np.hypot(rx, ry); rn = np.where(rn > 0, rn, 1.0)
    tx, ty = -ry / rn, rx / rn                    # circle tangent at the hit
    cosb = np.where(ok, np.abs(tx * uy - ty * ux), np.nan)
    ax, ay = -dca * nx - cx, -dca * ny - cy
    dang = np.arctan2(ry, rx) - np.arctan2(ay, ax)
    dang = (dang + np.pi) % (2 * np.pi) - np.pi
    s = np.abs(dang) * aR
    zt = z0 + s / np.tan(theta)
    zcode = np.abs(zcd) > 180.0
    dz = np.where(zcode, np.nan, zcd - zt)
    return {'res_mm': np.minimum(dp, dm) * 10.0, 'plus': plus, 'cosb': cosb,
            'slen': s, 'dz': dz, 'zcode': zcode, 'zcd': zcd, 'ztrk': zt}


def robust_sigma(v):
    v = v[np.isfinite(v)]
    if v.size == 0:
        return float('nan')
    m = np.median(v)
    return float(1.4826 * np.median(np.abs(v - m)))


# ----------------------------------------------------------------------------- run list
def cmd_runlist(a):
    import ROOT
    ROOT.gSystem.Load('libEG')
    out = subprocess.run(['bash', '-c', 'ldd $(which oolumi)'], capture_output=True, text=True).stdout
    for l in out.splitlines():
        if '/h1oo/lib/' in l and '=>' in l:
            ROOT.gSystem.Load(l.split()[2])
    f = ROOT.TFile.Open(RUNLIST_ROOT)
    rl = f.Get('H1RunList')
    runs = sorted({int(os.path.basename(p)[7:-5]) for p in glob.glob(f'{LAM}/c07*/calib/consts_*.json')})
    with open(a.out, 'w') as fo:
        fo.write(f'# run selected  (H1RunList::IsSelected of {RUNLIST_ROOT}; lam/ runs)\n')
        for r in runs:
            fo.write(f'{r} {int(bool(rl.IsSelected(r)))}\n')
    print('runs', len(runs), 'rejected', [r for r in runs if not rl.IsSelected(r)])


def read_runlist(fn):
    return {int(l.split()[0]): int(l.split()[1]) for l in open(fn) if l.strip() and not l.startswith('#')}


# ----------------------------------------------------------------------------- MC
def mc_residuals(hits_file):
    """hits tree (extract_hits.C schema) -> (per-hit arrays of pack(), info). Hits with cjc_reco_trk >= 0."""
    import uproot
    t = uproot.open(hits_file)['hits']
    br = ['cjc_xp', 'cjc_yp', 'cjc_xm', 'cjc_ym', 'cjc_zcd', 'cjc_reco_trk', 'cjc_reco_side',
          'cjkr_kappa', 'cjkr_phi', 'cjkr_theta', 'cjkr_dca', 'cjkr_z0']
    e = t.arrays(br, library='ak')
    # flatten per event with explicit indexing (small file: 249 events)
    cols = {k: [] for k in ('xp', 'yp', 'xm', 'ym', 'zcd', 'side', 'kappa', 'phi', 'theta', 'dca', 'z0')}
    for ev in range(len(e)):
        tr = np.asarray(e.cjc_reco_trk[ev]); m = tr >= 0
        if not m.any():
            continue
        for k, b in (('xp', 'cjc_xp'), ('yp', 'cjc_yp'), ('xm', 'cjc_xm'), ('ym', 'cjc_ym'), ('zcd', 'cjc_zcd'),
                     ('side', 'cjc_reco_side')):
            cols[k].append(np.asarray(e[b][ev])[m])
        for k in ('kappa', 'phi', 'theta', 'dca', 'z0'):
            cols[k].append(np.asarray(e['cjkr_' + k][ev], dtype=np.float64)[tr[m]])
    c = {k: np.concatenate(v).astype(np.float64) for k, v in cols.items()}
    good = np.abs(c['kappa']) > 1e-6
    c = {k: v[good] for k, v in c.items()}
    r = residuals(c['xp'], c['yp'], c['xm'], c['ym'], c['zcd'], c['kappa'], c['phi'], c['theta'], c['dca'], c['z0'])
    side_ok = r['plus'] == (c['side'] == 0)
    return pack(r, side_ok), {'file': hits_file, 'events': len(e), 'hits_on_track': int(good.size), 'used': int(good.sum())}


def cmd_mc(a):
    arr, info = mc_residuals(a.hits)
    np.savez_compressed(a.out, **arr, info=json.dumps(info))
    print('MC events', info['events'], 'on-track hits used', info['used'], 'side match %.4f' % arr['side_ok'].mean())


# ----------------------------------------------------------------------------- data
def read_hat_status(hat, keys):
    """(run, evt) -> (Mhv1, Mhv2, Mbbl3) for the keys present in the dumps (uproot on the HAT)."""
    import uproot
    t = uproot.open(hat)['HAT']
    d = t.arrays(['RunNumber', 'EventNumber', 'Mhv1', 'Mhv2', 'Mbbl3'], library='np')
    k = d['RunNumber'].astype(np.int64) * 10**8 + d['EventNumber'].astype(np.int64)
    pos = {int(x): i for i, x in enumerate(k)}
    out = {}
    for (r, ev) in keys:
        i = pos.get(r * 10**8 + ev)
        if i is not None:
            out[(r, ev)] = (np.asarray(d['Mhv1'][i]), np.asarray(d['Mhv2'][i]), np.asarray(d['Mbbl3'][i]))
    return out


def det_on(st):
    hv, caen, bbl3 = st
    return not (any(hv[b] == 1 for b in HV_BITS) or any(caen[b] == 1 for b in CAEN_BITS)
                or any(bbl3[b] == 1 for b in BBL3_BITS))


def data_residuals(lam_dirs, runlist, max_events=10**9):
    """lam/ dumps -> (per-hit arrays of pack() with 'sel' = D17, list of per-dir info). Constants per run from
    <lam_dir>/calib/consts_<run>.json (the plan's consts_dir is each dir's calib/)."""
    if isinstance(lam_dirs, str):
        lam_dirs = [lam_dirs]
    parts = [_data_dir(d, runlist, max_events) for d in lam_dirs]
    return {k: np.concatenate([p[0][k] for p in parts]) for k in parts[0][0]}, [p[1] for p in parts]


def _data_dir(lam_dir, runlist, max_events):
    sys.path.insert(0, PY)
    from h1hits import CJC, cjavt0, load
    from events import read_events, read_dataprobe, xr_dict, flt
    d = lam_dir.rstrip('/')
    steer = open(os.path.join(d, 'calib/s.steer')).read()
    hat = steer.split('fHatFiles')[1].split('"')[1]
    evs = read_events(os.path.join(d, 'calib/events.txt'))[:max_events]
    raw = read_dataprobe(os.path.join(d, 'cjc/cjcprobe.txt'))
    rl = read_runlist(runlist)
    status = read_hat_status(hat, [(e['run'], e['evt']) for e in evs])
    cj = {}
    cnt = dict(seen=0, rejected=0, rej_runlist=0, rej_status=0, no_hat=0, empty=0, empty_after_sel=0,
               nmismatch=0, map_bad=0, no_consts=0, t0_cjavt0=0, t0_default=0, xr_hits=0,
               selected_with_xr=0)
    cols = {k: [] for k in ('xp', 'yp', 'xm', 'ym', 'zcd', 'side', 'kappa', 'phi', 'theta', 'dca', 'z0',
                            'COSB', 'SLEN', 'ZEXP', 'sel')}
    for ev in evs:
        run, en = ev['run'], ev['evt']
        cnt['seen'] += 1
        rsel = rl.get(run, 0) == 1
        st = status.get((run, en))
        if st is None:
            cnt['no_hat'] += 1
        don = st is not None and det_on(st)
        sel = rsel and don
        if not rsel: cnt['rej_runlist'] += 1
        if not don: cnt['rej_status'] += 1
        if not sel: cnt['rejected'] += 1
        rw = raw.get((run, en), {'HIT': []})['HIT']
        if not rw:
            cnt['empty'] += 1
            if sel: cnt['empty_after_sel'] += 1
            continue
        H = ev['H']
        if len(rw) != len(H):
            cnt['nmismatch'] += 1; continue
        if run not in cj:
            fn = os.path.join(d, f'calib/consts_{run}.json')
            cj[run] = CJC(load(fn)) if os.path.exists(fn) else None
        c = cj[run]
        if c is None:
            cnt['no_consts'] += 1; continue
        prep = c.prepare_event(rw)
        tt = [(flt(r[26]), abs(flt(r[27]))) for r in ev['CJKR'] if flt(r[26]) != 0.0 and flt(r[27]) != 0.0]
        if tt:
            t0 = cjavt0([x for x, y in tt], [y for x, y in tt])[0]; cnt['t0_cjavt0'] += 1
        else:
            t0 = c.A0[0]; cnt['t0_default'] += 1
        mir = {}
        n0 = cnt['xr_hits']
        for r in ev['XR']:
            x = xr_dict(r)
            if x['kdr'] <= 0 or x['itr'] < 1 or x['itr'] > len(ev['CJKR']):
                continue
            k = x['kdr'] - 1
            if k >= len(H) or prep[k]['row'] != H[k]:
                cnt['map_bad'] += 1; continue
            if k not in mir:
                mir[k] = (c.simple(prep[k], t0, 1.0), c.simple(prep[k], t0, -1.0))
            p, m = mir[k]
            cr = ev['CJKR'][x['itr'] - 1]
            cnt['xr_hits'] += 1
            for key, v in (('xp', p['x']), ('yp', p['y']), ('xm', m['x']), ('ym', m['y']), ('zcd', p['z']),
                           ('side', 0 if x['DRFE'] > 0 else 1), ('kappa', flt(cr[0])), ('phi', flt(cr[1])),
                           ('theta', flt(cr[2])), ('dca', flt(cr[3])), ('z0', flt(cr[4])), ('COSB', x['COSB']),
                           ('SLEN', x['SLEN']), ('ZEXP', x['ZEXP']), ('sel', sel)):
                cols[key].append(v)
        if sel and cnt['xr_hits'] > n0:
            cnt['selected_with_xr'] += 1
    c = {k: np.asarray(v, dtype=np.float64) for k, v in cols.items()}
    good = np.abs(c['kappa']) > 1e-6
    c = {k: v[good] for k, v in c.items()}
    r = residuals(c['xp'], c['yp'], c['xm'], c['ym'], c['zcd'], c['kappa'], c['phi'], c['theta'], c['dca'], c['z0'])
    side_ok = r['plus'] == (c['side'] == 0)
    ad = lambda u, v: np.abs(u - v)
    xc = {'n': int(good.sum()),
          'cosb_absdiff_median': float(np.median(ad(r['cosb'], np.abs(c['COSB'])))) if good.any() else None,
          'cosb_absdiff_p95': float(np.percentile(ad(r['cosb'], np.abs(c['COSB'])), 95)) if good.any() else None,
          'slen_absdiff_median_cm': float(np.median(ad(r['slen'], c['SLEN']))) if good.any() else None,
          'slen_absdiff_p95_cm': float(np.percentile(ad(r['slen'], c['SLEN']), 95)) if good.any() else None,
          'ztrk_zexp_absdiff_median_cm': float(np.median(ad(r['ztrk'], c['ZEXP']))) if good.any() else None}
    info = {'lam_dir': d, 'hat': hat, 'runs': sorted(cj), 'counters': cnt, 'xr_crosscheck': xc}
    return pack(r, side_ok, c['sel'] > 0.5), info


def cmd_data(a):
    arr, infos = data_residuals([a.lam_dir], a.runlist, a.max_events)
    np.savez_compressed(a.out, **arr, info=json.dumps(infos[0]))
    print(json.dumps(infos[0]))


# ----------------------------------------------------------------------------- compare
def load_npz(fn):
    z = np.load(fn)
    return {k: z[k] for k in z.files if k != 'info'}, json.loads(str(z['info']))


def binned(s, key, edges):
    out = []
    for lo, hi in zip(edges[:-1], edges[1:]):
        m = (s[key] >= lo) & (s[key] < hi) if hi < edges[-1] or key != 'cosb' else (s[key] >= lo) & (s[key] <= hi)
        v = s['res_mm'][m]
        out.append({'lo': float(lo), 'hi': float(hi), 'n': int(v.size),
                    'median_mm': float(np.median(v)) if v.size else float('nan')})
    return out


def summarize(s):
    res = s['res_mm']
    dz = s['dz']
    zc = s['zcat']
    codes = {str(c): float(np.mean(zc == i + 1)) for i, c in enumerate(ZCODES)}
    return {'n': int(res.size), 'median_mm': float(np.median(res)),
            'cdf': {str(x): float(np.mean(res < x)) for x in CDF_MM},
            'cosb': binned(s, 'cosb', COSB_EDGES), 'slen': binned(s, 'slen', SLEN_EDGES),
            'z_robust_sigma_cm': robust_sigma(dz), 'z_median_cm': float(np.nanmedian(dz)),
            'z_n': int(np.isfinite(dz).sum()), 'zcode_fraction': float(np.mean(zc > 0)), 'zcode_by_value': codes,
            'zcd_abs_gt130_fraction_of_z_n': float(s['z130'].sum() / max(1, np.isfinite(dz).sum())),
            'side_match': float(np.mean(s['side_ok']))}


def verdict(mc, da):
    tests = {}
    for key in ('cosb', 'slen'):
        rows, ok = [], True
        for bm, bd in zip(mc[key], da[key]):
            if bm['n'] >= MIN_HITS and bd['n'] >= MIN_HITS:
                ratio = bd['median_mm'] / bm['median_mm']
                p = 0.5 <= ratio <= 2.0
                ok &= p
                rows.append({'lo': bm['lo'], 'hi': bm['hi'], 'ratio': ratio, 'pass': bool(p)})
        tests['median_vs_' + key] = {'pass': bool(ok and rows), 'bins_tested': len(rows), 'bins': rows,
                                     'ratio_min': min((r['ratio'] for r in rows), default=float('nan')),
                                     'ratio_max': max((r['ratio'] for r in rows), default=float('nan'))}
    diffs = {x: da['cdf'][x] - mc['cdf'][x] for x in mc['cdf']}
    tests['cdf'] = {'pass': bool(all(abs(v) <= 0.05 for v in diffs.values())), 'diff': diffs}
    zr = da['z_robust_sigma_cm'] / mc['z_robust_sigma_cm']
    tests['zcd_width'] = {'pass': bool(0.5 <= zr <= 2.0), 'ratio': zr}
    tests['mc_side_check'] = {'pass': bool(mc['side_match'] >= 0.90), 'fraction': mc['side_match']}
    v = 'PASS' if all(t['pass'] for t in tests.values()) else 'FAIL'
    return v, tests


def compare(mc, data):
    """mc = (arrays, info) from mc_residuals; data = list of (arrays, info-or-list) from data_residuals or the
    per-dir npz files (arrays without 'sel' count as selected). Returns (parity dict, summaries, dz arrays)."""
    mc_arr, mcinfo = mc
    infos = []
    for p in data:
        infos += p[1] if isinstance(p[1], list) else [p[1]]
    arrs = [dict(p[0], sel=p[0].get('sel', np.ones(p[0]['res_mm'].size, bool))) for p in data]
    alld = {k: np.concatenate([p[k] for p in arrs]) for k in arrs[0]}
    seld = {k: v[alld['sel']] for k, v in alld.items()}
    S = {'mc': summarize(mc_arr), 'data_d17': summarize(seld), 'data_nosel': summarize(alld)}
    v_sel, t_sel = verdict(S['mc'], S['data_d17'])
    out = {'verdict': v_sel, 'verdict_sample': 'data with D17 selection (SelectedRuns.root AND IsOn CJC1, CJC2, COZ)',
           'tests': t_sel, 'summary': S, 'mc_info': mcinfo, 'n_lam_dirs': len(infos),
           'tolerances': {'median_ratio': [0.5, 2.0], 'min_hits_per_bin': MIN_HITS, 'cdf_abs_diff': 0.05,
                          'zcd_width_ratio': [0.5, 2.0], 'mc_side_check_min': 0.90}}
    if infos and 'counters' in infos[0]:
        cnt = {}
        for i in infos:
            for k, x in i['counters'].items():
                cnt[k] = cnt.get(k, 0) + x
        n = max(1, cnt['seen'])
        out['d17'] = {'counters': cnt, 'per_dir': {os.path.basename(i['lam_dir']): i['counters'] for i in infos},
                      'fractions': {'run_list': cnt['rej_runlist'] / n, 'status_bits': cnt['rej_status'] / n,
                                    'union': cnt['rejected'] / n,
                                    'status_bits_in_accepted_runs': (cnt['rejected'] - cnt['rej_runlist'])
                                    / max(1, cnt['seen'] - cnt['rej_runlist'])}}
        xcs = [i['xr_crosscheck'] for i in infos if i['xr_crosscheck']['n'] > 0]
        xc = {k: {'median_over_dirs': float(np.median([x[k] for x in xcs])), 'max_over_dirs': float(max(x[k] for x in xcs))}
              for k in (xcs[0] if xcs else {}) if k != 'n'}
        xc['data_side_match_nosel'] = float(np.mean(alld['side_ok']))
        xc['data_side_match_d17'] = float(np.mean(seld['side_ok'])) if seld['side_ok'].size else float('nan')
        out['data_geometry_crosscheck_vs_CJXR'] = xc
    return out, S, {'mc': mc_arr['dz'], 'data_d17': seld['dz'], 'data_nosel': alld['dz']}


def cmd_compare(a):
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    out, S, DZ = compare(load_npz(a.mc), [load_npz(f) for f in sorted(a.data)])
    json.dump(out, open(a.out, 'w'), indent=1)
    plots(S, DZ, a.plots, plt)
    print('verdict', out['verdict'])
    for k, t in out['tests'].items():
        print(' ', k, 'PASS' if t['pass'] else 'FAIL', {x: y for x, y in t.items() if x not in ('bins', 'pass')})


STY = {'mc': dict(color='#1f77b4', marker='o', ls='-', label='MC ($MC_0$, extract_hits.C)'),
       'data_d17': dict(color='#d62728', marker='s', ls='-', label='Data lam/, D17 selection'),
       'data_nosel': dict(color='#ff9896', marker='', ls='--', label='Data lam/, no D17 selection')}


def _median_plot(S, key, xlabel, fn, plt):
    fig, (ax, rx) = plt.subplots(2, 1, figsize=(7, 6.5), sharex=True, gridspec_kw={'height_ratios': [2.2, 1]})
    mcb = S['mc'][key]
    for s in ('mc', 'data_d17', 'data_nosel'):
        b = S[s][key]
        xc = np.array([0.5 * (r['lo'] + r['hi']) for r in b]); y = np.array([r['median_mm'] for r in b])
        ok = np.array([r['n'] >= MIN_HITS for r in b])
        ax.plot(xc[ok], y[ok], **STY[s])
        if s != 'mc':
            both = ok & np.array([r['n'] >= MIN_HITS for r in mcb])
            ratio = y / np.array([r['median_mm'] for r in mcb])
            rx.plot(xc[both], ratio[both], **{k: v for k, v in STY[s].items() if k != 'label'})
    lo = np.array([r['lo'] for r in mcb]); hi = np.array([r['hi'] for r in mcb]); m = np.array([r['median_mm'] for r in mcb])
    okm = np.array([r['n'] >= MIN_HITS for r in mcb])
    if okm.any():
        ax.fill_between(np.r_[lo[okm], hi[okm][-1:]], np.r_[0.5 * m[okm], 0.5 * m[okm][-1:]], np.r_[2 * m[okm], 2 * m[okm][-1:]],
                        step='post', color='#1f77b4', alpha=0.15, label='D16 tolerance: MC median ×[0.5, 2]')
    ax.set_yscale('log'); ax.set_ylabel('median residual, nearer mirror to CJKR circle [mm]')
    ax.legend(fontsize=8); ax.grid(alpha=0.3, which='both')
    rx.axhspan(0.5, 2.0, color='#1f77b4', alpha=0.15); rx.axhline(1, color='k', lw=0.7)
    rx.set_yscale('log'); rx.set_ylim(0.25, 4); rx.set_ylabel('data / MC median'); rx.set_xlabel(xlabel)
    rx.grid(alpha=0.3, which='both')
    fig.tight_layout(); fig.savefig(fn + '.png', dpi=130); fig.savefig(fn + '.pdf'); plt.close(fig)


def plots(S, DZ, pdir, plt):
    os.makedirs(pdir, exist_ok=True)
    _median_plot(S, 'cosb', r'$|\cos\beta|$ (track angle w.r.t. drift-direction normal)', os.path.join(pdir, 'rphi_median_vs_cosb'), plt)
    _median_plot(S, 'slen', 'arc length from the track PCA [cm]', os.path.join(pdir, 'rphi_median_vs_slen'), plt)
    # CDF
    fig, (ax, rx) = plt.subplots(2, 1, figsize=(7, 6.5), sharex=True, gridspec_kw={'height_ratios': [2.2, 1]})
    xs = np.array(CDF_MM)
    for s in ('mc', 'data_d17', 'data_nosel'):
        y = np.array([S[s]['cdf'][str(x)] for x in CDF_MM]); ax.plot(xs, y, **STY[s])
        if s != 'mc':
            rx.plot(xs, (y - np.array([S['mc']['cdf'][str(x)] for x in CDF_MM])) * 100,
                    **{k: v for k, v in STY[s].items() if k != 'label'})
    ym = np.array([S['mc']['cdf'][str(x)] for x in CDF_MM])
    ax.fill_between(xs, ym - 0.05, ym + 0.05, color='#1f77b4', alpha=0.15, label='D16 tolerance: MC ± 5 pp')
    ax.set_xscale('log'); ax.set_ylabel('fraction of on-track hits with residual < x')
    ax.set_ylim(0, 1); ax.legend(fontsize=8); ax.grid(alpha=0.3, which='both')
    rx.axhspan(-5, 5, color='#1f77b4', alpha=0.15); rx.axhline(0, color='k', lw=0.7)
    rx.set_ylabel('data − MC [pp]'); rx.set_xlabel('residual threshold x [mm]'); rx.set_xticks(xs); rx.set_xticklabels([str(x) for x in CDF_MM])
    rx.grid(alpha=0.3)
    fig.tight_layout(); fig.savefig(os.path.join(pdir, 'rphi_cdf.png'), dpi=130); fig.savefig(os.path.join(pdir, 'rphi_cdf.pdf')); plt.close(fig)
    # zcd width
    fig, (ax, rx) = plt.subplots(1, 2, figsize=(11, 4.5), gridspec_kw={'width_ratios': [2.2, 1]})
    bins = np.linspace(-100, 100, 201)
    for s in ('mc', 'data_d17', 'data_nosel'):
        dz = DZ[s]; dz = dz[np.isfinite(dz)]
        ax.hist(dz, bins=bins, density=True, histtype='step', color=STY[s]['color'], ls=STY[s]['ls'],
                label=f"{STY[s]['label']}: robust σ = {S[s]['z_robust_sigma_cm']:.2f} cm")
    ax.set_yscale('log'); ax.set_xlabel(r'$z_\mathrm{cd} - z_\mathrm{track}$ [cm]'); ax.set_ylabel('hits / cm (normalised)')
    ax.legend(fontsize=7); ax.grid(alpha=0.3)
    rx.axhspan(0.5, 2.0, color='#1f77b4', alpha=0.15, label='D16 tolerance [0.5, 2]'); rx.axhline(1, color='k', lw=0.7)
    for i, s in enumerate(('data_d17', 'data_nosel')):
        rx.plot([i], [S[s]['z_robust_sigma_cm'] / S['mc']['z_robust_sigma_cm']], color=STY[s]['color'], marker='s', ls='')
    rx.set_xticks([0, 1]); rx.set_xticklabels(['D17 sel.', 'no sel.']); rx.set_xlim(-0.7, 1.7)
    rx.set_yscale('log'); rx.set_ylim(0.25, 4); rx.set_ylabel('robust σ(data) / robust σ(MC)'); rx.legend(fontsize=7)
    rx.grid(alpha=0.3, which='both')
    fig.tight_layout(); fig.savefig(os.path.join(pdir, 'zcd_width.png'), dpi=130); fig.savefig(os.path.join(pdir, 'zcd_width.pdf')); plt.close(fig)



def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest='cmd', required=True)
    p = sp.add_parser('runlist'); p.add_argument('--out', required=True)
    p = sp.add_parser('mc'); p.add_argument('--hits', required=True); p.add_argument('--out', required=True)
    p = sp.add_parser('data'); p.add_argument('--lam-dir', required=True); p.add_argument('--runlist', required=True)
    p.add_argument('--out', required=True); p.add_argument('--max-events', type=int, default=10**9)
    p = sp.add_parser('compare'); p.add_argument('--mc', required=True); p.add_argument('--data', nargs='+', required=True)
    p.add_argument('--out', required=True); p.add_argument('--plots', required=True)
    a = ap.parse_args()
    {'runlist': cmd_runlist, 'mc': cmd_mc, 'data': cmd_data, 'compare': cmd_compare}[a.cmd](a)


if __name__ == '__main__':
    main()

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
  data    --lam-dir <dir> --runlist <txt> --out <npz> [--max-events N] [--t0 wp0|hat] [--det cjc_coz|cjc]
  compare --mc <npz> --data <npz...> --out <parity.json> --plots <dir>
  compare_t0 --mc <npz> --data-wp0 <npz...> --data-t0 <npz...> --out <parity_t0.json> --plots <dir>
--t0 hat (T0 confound check): HAT T0_CJC (t0_src 0) when joined and nonzero (the HAT holds 0.0 when DMIS has
no TZERO_M), else cjavt0 (2), else run default ARC0J8(1) (1). --det cjc: amended D17, det_on = IsOn with
SetCJCOn() (CJC1 + CJC2 incl. BBL3 0); COZ, CST, CIP become per-event flags (counters only).
Library use (WP1 validate_data_cjc.py): mc_residuals(hits_file), data_residuals(lam_dirs, runlist), compare(mc, data).
"""
import argparse, glob, json, os, subprocess, sys
import numpy as np

PY = '/data/dust/group/h1/klesthen/rawhit_probe/calib_consts/py'
RUNLIST_ROOT = '/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym/SelectedRuns.root'
LAM = '/data/dust/group/h1/klesthen/rawhit_probe/lam'
# D17 bits (H1DetectorStatus.C setters): CJC1 HV0 + CAEN0; CJC2 HV1 + CAEN1,2; COZ HV3 + CAEN4-6; CJC BBL3 0
HV_BITS, CAEN_BITS, BBL3_BITS = (0, 1, 3), (0, 1, 2, 4, 5, 6), (0,)
# amended D17: H1DetectorStatus::SetCJCOn() = SetCJC1On (HV0, CAEN0) + SetCJC2On (HV1, CAEN1,2) + BBL3 0;
# IsOn (H1DetectorStatus.C:833-861) checks every selected bit (run > 1994, not CJC-OR mode)
SEL_CJC = {'hv': (0, 1), 'caen': (0, 1, 2), 'bbl3': (0,)}
FLAG_BITS = {'coz': {'hv': (3,), 'caen': (4, 5, 6)}, 'cst': {'hv': (36,)}, 'cip': {'hv': (7,), 'caen': (11,)},
             'cjc1': {'hv': (0,), 'caen': (0,)}, 'cjc2': {'hv': (1,), 'caen': (1, 2)}, 'cjc_bbl3': {'bbl3': (0,)}}
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
    d = t.arrays(['RunNumber', 'EventNumber', 'Mhv1', 'Mhv2', 'Mbbl3', 'T0_CJC'], library='np')
    k = d['RunNumber'].astype(np.int64) * 10**8 + d['EventNumber'].astype(np.int64)
    pos = {int(x): i for i, x in enumerate(k)}
    out = {}
    for (r, ev) in keys:
        i = pos.get(r * 10**8 + ev)
        if i is not None:
            out[(r, ev)] = (np.asarray(d['Mhv1'][i]), np.asarray(d['Mhv2'][i]), np.asarray(d['Mbbl3'][i]),
                            float(d['T0_CJC'][i]))
    return out


def bits_off(st, sel):
    w = dict(zip(('hv', 'caen', 'bbl3'), st[:3]))
    return any(w[k][b] == 1 for k, bs in sel.items() for b in bs)


def det_on(st, mode='cjc_coz'):
    if mode == 'cjc':
        return not bits_off(st, SEL_CJC)
    hv, caen, bbl3 = st[:3]
    return not (any(hv[b] == 1 for b in HV_BITS) or any(caen[b] == 1 for b in CAEN_BITS)
                or any(bbl3[b] == 1 for b in BBL3_BITS))


def data_residuals(lam_dirs, runlist, max_events=10**9, t0_mode='wp0', det_mode='cjc_coz'):
    """lam/ dumps -> (per-hit arrays of pack() with 'sel' = D17, list of per-dir info). Constants per run from
    <lam_dir>/calib/consts_<run>.json (the plan's consts_dir is each dir's calib/)."""
    if isinstance(lam_dirs, str):
        lam_dirs = [lam_dirs]
    parts = [_data_dir(d, runlist, max_events, t0_mode, det_mode) for d in lam_dirs]
    return {k: np.concatenate([p[0][k] for p in parts]) for k in parts[0][0]}, [p[1] for p in parts]


def _data_dir(lam_dir, runlist, max_events, t0_mode='wp0', det_mode='cjc_coz'):
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
               selected_with_xr=0, t0_hat=0, hat_t0_zero=0, hat_t0_notrack=0)
    cnt.update({f'flag_{k}_off': 0 for k in FLAG_BITS})
    t0cmp = []                                   # (HAT T0_CJC, cjavt0) on events that have both (HAT nonzero)
    cols = {k: [] for k in ('xp', 'yp', 'xm', 'ym', 'zcd', 'side', 'kappa', 'phi', 'theta', 'dca', 'z0',
                            'COSB', 'SLEN', 'ZEXP', 'sel', 't0_src', 't0_src_wp0')}
    for ev in evs:
        run, en = ev['run'], ev['evt']
        cnt['seen'] += 1
        rsel = rl.get(run, 0) == 1
        st = status.get((run, en))
        if st is None:
            cnt['no_hat'] += 1
        don = st is not None and det_on(st, det_mode)
        if st is not None:
            for k, b in FLAG_BITS.items():
                cnt[f'flag_{k}_off'] += bits_off(st, b)
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
            t0 = cjavt0([x for x, y in tt], [y for x, y in tt])[0]; src_wp0 = 2
        else:
            t0 = c.A0[0]; src_wp0 = 1
        hat_t0 = st[3] if st is not None else None
        if hat_t0 is not None and hat_t0 == 0.0:
            cnt['hat_t0_zero'] += 1
        elif hat_t0 is not None:
            if tt: t0cmp.append((hat_t0, t0))
            else: cnt['hat_t0_notrack'] += 1
        src = src_wp0
        if t0_mode == 'hat' and hat_t0:
            t0, src = hat_t0, 0
        cnt[{0: 't0_hat', 1: 't0_default', 2: 't0_cjavt0'}[src]] += 1
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
                           ('SLEN', x['SLEN']), ('ZEXP', x['ZEXP']), ('sel', sel), ('t0_src', src),
                           ('t0_src_wp0', src_wp0)):
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
    info = {'lam_dir': d, 'hat': hat, 'runs': sorted(cj), 'counters': cnt, 'xr_crosscheck': xc,
            't0_mode': t0_mode, 'det_mode': det_mode, 't0_hat_vs_cjavt0': t0cmp}
    out = pack(r, side_ok, c['sel'] > 0.5)
    out['t0_src'] = c['t0_src'].astype(np.int8); out['t0_src_wp0'] = c['t0_src_wp0'].astype(np.int8)
    return out, info


def cmd_data(a):
    arr, infos = data_residuals([a.lam_dir], a.runlist, a.max_events, a.t0, a.det)
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



# ----------------------------------------------------------------------------- T0 confound check
T0STY = {'mc': STY['mc'],
         'data_wp0': dict(color='#d62728', marker='s', ls='-', label='Data, WP0 T0 (cjavt0, else run default)'),
         'data_hat': dict(color='#2ca02c', marker='^', ls='--', label='Data, HAT T0_CJC (else cjavt0, else run default)'),
         'wp0_cjavt0': dict(color='#d62728', marker='s', ls='-', label='Data WP0, events with cjavt0 (t0_src 2)'),
         'wp0_default': dict(color='#9467bd', marker='v', ls='--', label='Data WP0, events with run-default T0 (t0_src 1)')}


def compare_t0(mc, wp0, t0):
    """mc = (arrays, info); wp0, t0 = lists of per-dir (arrays, info) in the same lam-dir order. The --t0 hat
    run walks the same events and hits as WP0 (T0 changes no hit inclusion), so its per-hit t0_src_wp0 labels
    the WP0 hits; alignment is checked per dir (hit count, zcd category, |cos beta|)."""
    mc_arr = mc[0]
    align, wsplit = {}, []
    for (wa, wi), (ta, ti) in zip(wp0, t0):
        name = os.path.basename(wi['lam_dir'])
        assert name == os.path.basename(ti['lam_dir']), (name, ti['lam_dir'])
        n_ok = wa['res_mm'].size == ta['res_mm'].size
        a = {'n_wp0': int(wa['res_mm'].size), 'n_t0': int(ta['res_mm'].size), 'n_equal': bool(n_ok)}
        if n_ok and a['n_wp0']:
            a['zcat_equal'] = float(np.mean(wa['zcat'] == ta['zcat']))
            dc = np.abs(wa['cosb'].astype(np.float32) - ta['cosb'].astype(np.float32))
            a['cosb_absdiff_p99'] = float(np.nanpercentile(dc, 99))
        align[name] = a
        if not n_ok or (a['n_wp0'] and a['zcat_equal'] != 1.0):
            raise SystemExit(f'alignment failed for {name}: {a}')
        wsplit.append(dict(wa, t0_src_wp0=ta['t0_src_wp0'], sel_new=ta['sel']))
    W = {k: np.concatenate([p[k] for p in wsplit]) for k in wsplit[0]}
    T = {k: np.concatenate([p[0][k] for p in t0]) for k in t0[0][0]}
    sub = lambda A, m: {k: v[m] for k, v in A.items()}
    A = {'mc': mc_arr,
         'data_wp0': sub(W, W['sel_new']),                        # WP0 T0 handling, amended D17 selection
         'data_hat': sub(T, T['sel']),                            # HAT T0_CJC, amended D17 selection
         'wp0_cjavt0': sub(W, W['sel'] & (W['t0_src_wp0'] == 2)),  # split on the WP0 verdict sample (WP0 D17)
         'wp0_default': sub(W, W['sel'] & (W['t0_src_wp0'] == 1))}
    S = {k: summarize(v) for k, v in A.items()}
    V = {k: dict(zip(('verdict', 'tests'), verdict(S['mc'], S[k]))) for k in S if k != 'mc'}
    infos = [p[1] for p in t0]
    cnt = {}
    for i in infos:
        for k, x in i['counters'].items():
            cnt[k] = cnt.get(k, 0) + x
    n = max(1, cnt['seen'])
    acc = max(1, cnt['seen'] - cnt['rej_runlist'])
    pairs = np.array([x for i in infos for x in i['t0_hat_vs_cjavt0']], dtype=np.float64).reshape(-1, 2)
    df = pairs[:, 0] - pairs[:, 1]
    t0chk = {'n_events_both': int(df.size), 'median_diff_ticks': float(np.median(df)) if df.size else None,
             'robust_sigma_diff_ticks': robust_sigma(df), 'frac_absdiff_lt_1tick': float(np.mean(np.abs(df) < 1)) if df.size else None,
             'p95_absdiff_ticks': float(np.percentile(np.abs(df), 95)) if df.size else None,
             'median_ratio': float(np.median(pairs[:, 0] / pairs[:, 1])) if df.size else None,
             'note': 'event counts below are over events reaching T0 assignment (non-empty, hit count matched, constants)',
             'hat_t0_zero_events_reaching_t0': cnt['hat_t0_zero'], 'hat_t0_nonzero_notrack_events_reaching_t0': cnt['hat_t0_notrack'],
             'events_reaching_t0_by_src_hat_run': {'hat_0': cnt['t0_hat'], 'default_1': cnt['t0_default'], 'cjavt0_2': cnt['t0_cjavt0']}}
    hits_src = {str(k): int(np.sum(W['t0_src_wp0'][W['sel']] == k)) for k in (1, 2)}
    out = {'verdict_hat_t0': V['data_hat']['verdict'],
           'verdict_samples': {'data_hat': 'HAT T0_CJC data, amended D17 (SelectedRuns AND IsOn CJC1+CJC2 incl. BBL3 0)',
                               'data_wp0': 'WP0 per-hit npz, amended D17 mask from the t0 run',
                               'wp0_cjavt0 / wp0_default': 'WP0 per-hit npz, WP0 D17 mask (as parity.json), split by WP0 t0 source'},
           'verdicts': V, 'summary': S, 't0_hat_vs_cjavt0': t0chk,
           'split_hits_by_wp0_t0_src_in_wp0_sample': hits_src,
           'd17_amended': {'counters': cnt, 'fractions': {
               'run_list': cnt['rej_runlist'] / n, 'status_bits_cjc_only': cnt['rej_status'] / n, 'union': cnt['rejected'] / n,
               'status_bits_in_accepted_runs': (cnt['rejected'] - cnt['rej_runlist']) / acc},
               'flag_off_fractions_of_seen': {k: cnt[f'flag_{k}_off'] / n for k in FLAG_BITS}},
           'alignment_wp0_vs_t0_run': align, 'mc_info': mc[1], 'n_lam_dirs': len(infos)}
    return out, S, {k: v['dz'] for k, v in A.items()}


def _overlay(S, DZ, series, sty, suffix, pdir, plt):
    """Same four D16 figures as plots(), for an arbitrary list of data series against MC (series[0] = 'mc')."""
    for key, xl, base in (('cosb', r'$|\cos\beta|$ (track angle w.r.t. drift-direction normal)', 'rphi_median_vs_cosb'),
                          ('slen', 'arc length from the track PCA [cm]', 'rphi_median_vs_slen')):
        fig, (ax, rx) = plt.subplots(2, 1, figsize=(7, 6.5), sharex=True, gridspec_kw={'height_ratios': [2.2, 1]})
        mcb = S['mc'][key]
        for s in series:
            b = S[s][key]
            xc = np.array([0.5 * (r['lo'] + r['hi']) for r in b]); y = np.array([r['median_mm'] for r in b])
            ok = np.array([r['n'] >= MIN_HITS for r in b]); ax.plot(xc[ok], y[ok], **sty[s])
            if s != 'mc':
                both = ok & np.array([r['n'] >= MIN_HITS for r in mcb])
                rx.plot(xc[both], (y / np.array([r['median_mm'] for r in mcb]))[both], **{k: v for k, v in sty[s].items() if k != 'label'})
        ax.set_yscale('log'); ax.set_ylabel('median residual, nearer mirror to CJKR circle [mm]'); ax.legend(fontsize=7)
        ax.grid(alpha=0.3, which='both')
        rx.axhspan(0.5, 2.0, color='#1f77b4', alpha=0.15, label='D16 tolerance [0.5, 2]'); rx.axhline(1, color='k', lw=0.7)
        rx.set_yscale('log'); rx.set_ylim(0.25, 4); rx.set_ylabel('data / MC median'); rx.set_xlabel(xl); rx.legend(fontsize=7)
        rx.grid(alpha=0.3, which='both')
        fig.tight_layout(); fn = os.path.join(pdir, base + suffix)
        fig.savefig(fn + '.png', dpi=130); fig.savefig(fn + '.pdf'); plt.close(fig)
    fig, (ax, rx) = plt.subplots(2, 1, figsize=(7, 6.5), sharex=True, gridspec_kw={'height_ratios': [2.2, 1]})
    xs = np.array(CDF_MM); ym = np.array([S['mc']['cdf'][str(x)] for x in CDF_MM])
    for s in series:
        y = np.array([S[s]['cdf'][str(x)] for x in CDF_MM]); ax.plot(xs, y, **sty[s])
        if s != 'mc':
            rx.plot(xs, (y - ym) * 100, **{k: v for k, v in sty[s].items() if k != 'label'})
    ax.set_xscale('log'); ax.set_ylabel('fraction of on-track hits with residual < x'); ax.set_ylim(0, 1)
    ax.legend(fontsize=7); ax.grid(alpha=0.3, which='both')
    rx.axhspan(-5, 5, color='#1f77b4', alpha=0.15, label='D16 tolerance ± 5 pp'); rx.axhline(0, color='k', lw=0.7)
    rx.set_ylabel('data − MC [pp]'); rx.set_xlabel('residual threshold x [mm]'); rx.set_xticks(xs)
    rx.set_xticklabels([str(x) for x in CDF_MM]); rx.legend(fontsize=7); rx.grid(alpha=0.3)
    fig.tight_layout(); fn = os.path.join(pdir, 'rphi_cdf' + suffix)
    fig.savefig(fn + '.png', dpi=130); fig.savefig(fn + '.pdf'); plt.close(fig)
    fig, ax = plt.subplots(figsize=(8, 4.5)); bins = np.linspace(-100, 100, 201)
    for s in series:
        dz = DZ[s]; dz = dz[np.isfinite(dz)]
        r = S[s]['z_robust_sigma_cm'] / S['mc']['z_robust_sigma_cm']
        ax.hist(dz, bins=bins, density=True, histtype='step', color=sty[s]['color'], ls=sty[s]['ls'],
                label=f"{sty[s]['label']}: robust σ = {S[s]['z_robust_sigma_cm']:.2f} cm" + ('' if s == 'mc' else f' (×{r:.2f} MC)'))
    ax.set_yscale('log'); ax.set_xlabel(r'$z_\mathrm{cd} - z_\mathrm{track}$ [cm]'); ax.set_ylabel('hits / cm (normalised)')
    ax.legend(fontsize=7); ax.grid(alpha=0.3)
    fig.tight_layout(); fn = os.path.join(pdir, 'zcd_width' + suffix)
    fig.savefig(fn + '.png', dpi=130); fig.savefig(fn + '.pdf'); plt.close(fig)


def cmd_compare_t0(a):
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    key = lambda f: os.path.basename(f)
    wp0 = [load_npz(f) for f in sorted(a.data_wp0, key=key)]
    t0 = [load_npz(f) for f in sorted(a.data_t0, key=key)]
    assert [key(f) for f in sorted(a.data_wp0, key=key)] == [key(f) for f in sorted(a.data_t0, key=key)]
    out, S, DZ = compare_t0(load_npz(a.mc), wp0, t0)
    json.dump(out, open(a.out, 'w'), indent=1)
    os.makedirs(a.plots, exist_ok=True)
    _overlay(S, DZ, ('mc', 'data_wp0', 'data_hat'), T0STY, '_t0', a.plots, plt)
    _overlay(S, DZ, ('mc', 'wp0_cjavt0', 'wp0_default'), T0STY, '_split_t0', a.plots, plt)
    print('t0 check', json.dumps(out['t0_hat_vs_cjavt0']))
    print('d17 amended', json.dumps(out['d17_amended']['fractions']), json.dumps(out['d17_amended']['flag_off_fractions_of_seen']))
    for k, v in out['verdicts'].items():
        print(k, v['verdict'], {t: ('PASS' if x['pass'] else 'FAIL', {y: z for y, z in x.items() if y not in ('bins', 'pass')})
                                for t, x in v['tests'].items()})


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest='cmd', required=True)
    p = sp.add_parser('runlist'); p.add_argument('--out', required=True)
    p = sp.add_parser('mc'); p.add_argument('--hits', required=True); p.add_argument('--out', required=True)
    p = sp.add_parser('data'); p.add_argument('--lam-dir', required=True); p.add_argument('--runlist', required=True)
    p.add_argument('--out', required=True); p.add_argument('--max-events', type=int, default=10**9)
    p.add_argument('--t0', choices=('wp0', 'hat'), default='wp0'); p.add_argument('--det', choices=('cjc_coz', 'cjc'), default='cjc_coz')
    p = sp.add_parser('compare'); p.add_argument('--mc', required=True); p.add_argument('--data', nargs='+', required=True)
    p.add_argument('--out', required=True); p.add_argument('--plots', required=True)
    p = sp.add_parser('compare_t0'); p.add_argument('--mc', required=True); p.add_argument('--data-wp0', nargs='+', required=True)
    p.add_argument('--data-t0', nargs='+', required=True); p.add_argument('--out', required=True); p.add_argument('--plots', required=True)
    a = ap.parse_args()
    {'runlist': cmd_runlist, 'mc': cmd_mc, 'data': cmd_data, 'compare': cmd_compare, 'compare_t0': cmd_compare_t0}[a.cmd](a)


if __name__ == '__main__':
    main()

#!/usr/bin/env python3
"""Data vs MC overlay of the kept branches of the `hits` tree (real-data writer vs extract_hits.C MC).

  overlay_data_mc.py --mc <MC0.root> --data <glob|file ...> --plots-dir <dir> --out-json <json> [--events-per-file 150]

Reads with uproot only. Kept = every MC branch except the truth-only / empty-by-D7 ones (str_*, svx_*, n_str_all,
n_svx_all, cjc_truth_*, cjc_ntruth, crjt_*, crvt_*, crzt_*, crmt_*, coz_str, csts_*, cip_*, cop_*, cjcu_*) and the scalars
run/event. Writes multi-panel PNG (dpi 130) + PDF figures and a JSON summary (NaN -> null).
Run after `desygroupset h1` + thish1.sh (LCG python; never python3 -I).
"""
import argparse, glob, json, math, os, re
import numpy as np
import awkward as ak
import uproot
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

EXCL = re.compile(r'^(str_|svx_|n_str_all$|n_svx_all$|cjc_truth_|cjc_ntruth$|crjt_|crvt_|crzt_|crmt_|coz_str$|csts_|cip_|cop_|cjcu_|run$|event$)')
DATA_ONLY_VEC = ['cjc_trk_x', 'cjc_trk_y', 'cjc_trk_z']
DATA_ONLY_SCAL = ['cjc_t0', 'cjc_t0_src']
MULT = ['cjc_wire', 'cjkr_kappa', 'cjkt_kappa', 'cjkv_x', 'coz_wire']
UNITS = {'cjc_drift': 'cm', 'cjc_drift_raw': 'cm', 'cjc_zcd': 'cm', 'cjc_wire_x': 'cm', 'cjc_wire_y': 'cm',
         'cjc_xp': 'cm', 'cjc_yp': 'cm', 'cjc_xm': 'cm', 'cjc_ym': 'cm', 'coz_drift': '0.001 cm',
         'cjkr_kappa': '1/cm', 'cjkt_kappa': '1/cm', 'cjkr_dca': 'cm', 'cjkr_z0': 'cm', 'cjkr_rad': 'cm',
         'cjkr_rend': 'cm', 'cjkr_phi': 'rad', 'cjkr_theta': 'rad', 'cjkt_phi': 'rad', 'cjkt_theta': 'rad',
         'cjkv_x': 'cm', 'cjkv_y': 'cm', 'cjkv_z': 'cm', 'cjc_trk_x': 'cm', 'cjc_trk_y': 'cm', 'cjc_trk_z': 'cm',
         'cjc_t0': 'ticks'}
GROUPS = [('cjc_digis', r'^cjc_(wire|ring|cell|layer|drift|drift_raw|qplus|qminus|zcd)$'),
          ('cjc_geometry_xy', r'^cjc_(wire_x|wire_y|xp|yp|xm|ym)$'),
          ('cjc_reco_links', r'^cjc_reco_'),
          ('cjkr', r'^cjkr_'), ('cjkt_cjkv', r'^(cjkt_|cjkv_)'), ('coz', r'^coz_')]
COL = {'MC': '#0072B2', 'Data': '#D55E00'}   # Okabe-Ito blue / vermillion


def jnum(x):
    if x is None:
        return None
    if isinstance(x, (list, tuple)):
        return [jnum(v) for v in x]
    if isinstance(x, dict):
        return {k: jnum(v) for k, v in x.items()}
    if isinstance(x, (bool, int, np.integer)):
        return int(x)
    try:
        x = float(x)
    except (TypeError, ValueError):
        return x
    return None if (math.isnan(x) or math.isinf(x)) else x


def label(b):
    return '%s [%s]' % (b, UNITS[b]) if b in UNITS else b


def read_sample(files, branches, nmax, notes, name):
    """-> dict branch -> list of ak arrays (per file), n events, n files."""
    out = {b: [] for b in branches}
    nev = 0
    for f in files:
        try:
            t = uproot.open(f)['hits']
        except Exception as e:
            notes.append('%s: cannot open %s (%s)' % (name, f, str(e)[:80]))
            continue
        have = [b for b in branches if b in t.keys()]
        arr = t.arrays(have, entry_stop=nmax, library='ak')
        nev += len(arr)
        for b in have:
            out[b].append(arr[b])
    return {b: (ak.concatenate(v) if v else None) for b, v in out.items()}, nev


def flat(a):
    if a is None:
        return np.array([])
    if a.ndim > 1:
        a = ak.flatten(a, axis=None)
    return np.asarray(ak.to_numpy(a), dtype=float)


def counts(a):
    return np.asarray(ak.to_numpy(ak.num(a, axis=1)), dtype=float) if a is not None else np.array([])


def stats(v, cnt=None):
    if v.size == 0:
        d = dict(n=0, mean=None, median=None, p1=None, p99=None, frac_zero=None)
    else:
        d = dict(n=int(v.size), mean=float(v.mean()), median=float(np.median(v)), p1=float(np.percentile(v, 1)),
                 p99=float(np.percentile(v, 99)), frac_zero=float(np.mean(v == 0)))
    if cnt is not None:
        d['hits_per_event_mean'] = float(cnt.mean()) if cnt.size else None
        d['hits_per_event_median'] = float(np.median(cnt)) if cnt.size else None
    return d


def bins_for(vs, is_int):
    pooled = np.concatenate([v for v in vs if v.size])
    pooled = pooled[np.isfinite(pooled)]
    lo, hi = np.percentile(pooled, 0.5), np.percentile(pooled, 99.5)
    full_lo, full_hi = pooled.min(), pooled.max()
    clipped = (lo > full_lo) or (hi < full_hi)
    if is_int:
        lo, hi = math.floor(lo), math.ceil(hi)
        if hi - lo <= 200:
            return np.arange(lo - 0.5, hi + 1.5, 1.0), clipped
        step = math.ceil((hi - lo) / 100.0)
        return np.arange(lo - 0.5, hi + step + 0.5, step), clipped
    if hi <= lo:
        lo, hi = lo - 0.5, hi + 0.5
    return np.linspace(lo, hi, 101), clipped


def panel(ax, xlabel, samples, is_int):
    """samples: ordered dict name -> flat values."""
    vs = [v for v in samples.values() if v.size]
    if not vs:
        ax.text(0.5, 0.5, 'empty', transform=ax.transAxes, ha='center'); ax.set_xlabel(xlabel, fontsize=8); return
    bins, clipped = bins_for(vs, is_int)
    for nm, v in samples.items():
        if v.size == 0:
            continue
        ax.hist(np.clip(v, bins[0], bins[-1]) if False else v[(v >= bins[0]) & (v <= bins[-1])], bins=bins,
                density=True, histtype='step', lw=1.4, color=COL[nm.split()[0]],
                label='%s (n=%d)' % (nm, v.size))
    ax.set_yscale('log')
    ax.set_xlabel(xlabel + (' (0.5-99.5% range)' if clipped else ''), fontsize=8)
    ax.set_ylabel('density', fontsize=8)
    ax.tick_params(labelsize=7)
    ax.legend(fontsize=6, frameon=False)


def save_fig(items, name, plots, written, ncol=3):
    if not items:
        return
    n = len(items)
    nr = int(math.ceil(n / ncol))
    fig, axs = plt.subplots(nr, min(ncol, n), figsize=(4.2 * min(ncol, n), 3.2 * nr), squeeze=False)
    for ax in axs.ravel():
        ax.set_visible(False)
    for ax, (xl, samples, is_int) in zip(axs.ravel(), items):
        ax.set_visible(True)
        panel(ax, xl, samples, is_int)
    fig.suptitle(name, fontsize=10)
    fig.tight_layout()
    for ext in ('png', 'pdf'):
        p = os.path.join(plots, 'overlay_%s.%s' % (name, ext))
        fig.savefig(p, dpi=130)
        written.append(p)
    plt.close(fig)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--mc', required=True)
    ap.add_argument('--data', nargs='+', required=True)
    ap.add_argument('--plots-dir', required=True)
    ap.add_argument('--out-json', required=True)
    ap.add_argument('--events-per-file', type=int, default=150)
    a = ap.parse_args()
    os.makedirs(a.plots_dir, exist_ok=True)
    dfiles = sorted(set(f for p in a.data for f in (glob.glob(p) or [p])))
    notes, written = [], []

    mt = uproot.open(a.mc)['hits']
    kept, isint = [], {}
    for b, br in mt.items():
        if EXCL.match(b):
            continue
        tn = br.typename
        if '[' in tn or 'vector' in tn:
            kept.append(b)
            isint[b] = ('int' in tn.lower() and 'float' not in tn.lower())
    mc, mc_nev = read_sample([a.mc], kept + [], 10**9, notes, 'MC')
    extra = DATA_ONLY_VEC + DATA_ONLY_SCAL + ['cjc_reco_trk']
    dt, d_nev = read_sample(dfiles, list(dict.fromkeys(kept + extra)), a.events_per_file, notes, 'Data')
    nm_mc, nm_dt = 'MC', 'Data'
    summary = {}
    flats = {}
    for b in kept:
        row = {}
        fl = {}
        for nm, src in ((nm_mc, mc), (nm_dt, dt)):
            arr = src.get(b)
            if arr is None or len(arr) == 0:
                notes.append('%s: branch %s missing' % (nm, b)); fl[nm] = np.array([]); row[nm] = stats(np.array([])); continue
            v = flat(arr)
            fl[nm] = v
            if v.size == 0:
                notes.append('%s: branch %s empty (no entries)' % (nm, b))
            row[nm] = stats(v, counts(arr))
        summary[b] = row
        flats[b] = fl

    # figures
    assigned = set()
    for gname, rx in GROUPS:
        items = []
        for b in kept:
            if re.match(rx, b) and b not in assigned:
                assigned.add(b)
                f = flats[b]
                if sum(v.size > 0 for v in f.values()) == 0:
                    continue
                items.append((label(b), {'MC': f['MC'], 'Data': f['Data']}, isint[b]))
        save_fig(items, gname, a.plots_dir, written)
    rest = [b for b in kept if b not in assigned and any(v.size for v in flats[b].values())]
    if rest:
        save_fig([(label(b), flats[b], isint[b]) for b in rest], 'other', a.plots_dir, written)

    # multiplicity
    mult_items = []
    mult_stats = {}
    for b in MULT:
        cs = {}
        for nm, src in (('MC', mc), ('Data', dt)):
            cs[nm] = counts(src.get(b))
        mult_stats[b] = {nm: dict(mean=(float(c.mean()) if c.size else None), median=(float(np.median(c)) if c.size else None),
                                  n_events=int(c.size)) for nm, c in cs.items()}
        if any(c.size for c in cs.values()):
            mult_items.append(('hits (rows) per event: ' + b, cs, True))
        else:
            notes.append('multiplicity %s empty in both samples' % b)
    save_fig(mult_items, 'multiplicity', a.plots_dir, written)

    # data-only
    do_items, do_stats = [], {}
    sel = dt.get('cjc_reco_trk')
    for b in DATA_ONLY_VEC:
        arr = dt.get(b)
        if arr is None:
            notes.append('Data: data-only branch %s missing' % b); continue
        try:
            if sel is not None and ak.all(ak.num(arr) == ak.num(sel)):
                v = flat(arr[sel >= 0])
            else:
                notes.append('%s: length differs from cjc_wire-aligned cjc_reco_trk; used all entries (not filtered by reco_trk>=0)' % b)
                v = flat(arr)
        except Exception as e:
            notes.append('%s: filter failed (%s); used all entries' % (b, str(e)[:80])); v = flat(arr)
        do_stats[b] = stats(v)
        if v.size:
            do_items.append((label(b) + ' (reco_trk>=0)', {'Data': v}, False))
    for b in DATA_ONLY_SCAL:
        arr = dt.get(b)
        if arr is None or len(arr) == 0:
            notes.append('Data: scalar %s missing' % b); continue
        v = np.asarray(ak.to_numpy(arr), dtype=float)
        do_stats[b] = stats(v)
        do_items.append((label(b) + ' (per event)', {'Data': v}, b == 'cjc_t0_src'))
    save_fig(do_items, 'data_only', a.plots_dir, written)

    hpe = {}
    for key, br in (('cjc', 'cjc_wire'), ('coz', 'coz_wire'), ('cjkr', 'cjkr_kappa')):
        for nm, tag in (('Data', 'data'), ('MC', 'mc')):
            c = mult_stats[br][nm]
            hpe['%s_%s_mean' % (key, tag)] = c['mean']
            hpe['%s_%s_median' % (key, tag)] = c['median']
    out = dict(mc_file=a.mc, data_files_found=len(dfiles), events_per_file=a.events_per_file,
               samples={'MC': dict(n_events=int(mc_nev), n_files=1), 'Data': dict(n_events=d_nev, n_files=len(dfiles))},
               hits_per_event=hpe, branches=summary, multiplicity=mult_stats, data_only=do_stats,
               figures=written, notes=notes)
    with open(a.out_json, 'w') as fh:
        json.dump(jnum(out), fh, indent=1, allow_nan=False)
    print('wrote %d figure files, %s' % (len(written), a.out_json))


if __name__ == '__main__':
    main()

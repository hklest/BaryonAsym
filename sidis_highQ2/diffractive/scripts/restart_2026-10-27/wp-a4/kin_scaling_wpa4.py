#!/usr/bin/env python3
"""WP-A4 kinematic-scaling test of the forward-region data/MC mismatch (one Condor job over the
17 skims of skim_wpa4.py).  GRID AND TESTS FROZEN IN THIS DOCSTRING BEFORE ANY OUTPUT WAS READ.

Physics question: does the data/MC mismatch scale with event kinematics (physics-modelling signature:
forward hadronic activity / remnant flow grows with W) or is it flat across kinematics (detector
simulation / reconstruction / overlay-noise signature)?  Plus MC reco-vs-truth eta_max response at the LAr edge.

Sample ("validation chain" sample): A_pre (skim) && finite 35<E-pz<70, NO veto, NO gap cut, all EventNumber
parities.  MC = DIFF8 + ND chain, chains 7071, 7091, nd4cat (frozen stack C: weights = skim signed-v3 w x
W.fac_used/W.table_fac exposure rescale, exactly fact_wpa3.py `base`).  Data = 4 periods, unit weight.
Errors: data Poisson, MC sum w^2.  Sentinel etaMaxClus08 < -900 = no cluster (kept in "gap" regions, outside eta bins).

KINEMATIC GRID (6 cells K0..K5): Q2s in [150,300), [300,700), [700,inf)  x  x = Q2s/(4*27.6*920*Ys) below / above the
median of x of pooled DATA events of the sample within that Q2 bin (median computed from DATA only; same cut for MC).
Cell index = 2*qbin + (x>=median).  Per-cell predictors of expected forward activity: <W> from W^2 = Q2 (1-x)/x
(data, per cell) and <hatEtaMaxGen> (MC, weighted, over events with hatEtaMaxGen>-900, chain-specific).

(A) LAr edge.  Bins of etaMaxClus08: [2.0,2.5) [2.5,3.0) [3.0,3.3) [3.15,3.35) [3.40,3.55).  R = (D_bin/D_norm)/(M_bin/M_norm),
two normalisations: N1 = events with 1.0<=eta<2.0 in the same cell; NALL = all events of the cell (incl. no-cluster).  Per cell, per chain.
Also a "2.75" point: bin [2.5,3.0) is the 2.75 bin.  Slope test per (bin, chain, norm): R vs cell predictor.
(B) Plug.  Regions B1: finite etaMaxClus08>=3.3 ; B2: etaMaxClus08<2.6 (incl. no-cluster).  f0 = frac(ePlug<0.1) and
f7 = frac(ePlug>7) over events with finite ePlug in the region.  R = f_data/f_MC per cell.
(C) FMD/FTS fail fractions: f(!selFmd), f(!selFts) in B1 and B2.  R = f_data/f_MC.
Flatness for every R-series over the 6 cells: chi2 around the weighted mean (ndf=5) and p; linear-trend: weighted
least-squares R = a + b (t - <t>) vs t in {<W>, <hatEtaMaxGen>}; report b, sigma_b, z=b/sigma_b.
Interpretation (pre-registered): B2/C flat (chi2 p>0.05, |z|<2) => noise/overlay/detector signature; |z|>=3 or p<0.01
correlated with predictor => kinematic (physics-modelling) dependence.
(D) MC only (9012, 7091, same sample, weights w): response matrix reco etaMaxClus08 [3.0,3.6) in 0.1 bins vs truth hatEtaMaxGen
(rows: reco; columns: truth <3.0, 3.0..3.6 in 0.1 bins, >=3.6, none(-999)), same vs etaMaxGen; fraction of reco [3.40,3.55)
(and every 0.05 reco bin 3.0-3.7) with truth >3.6 (hat and etaMaxGen); mean truth in those reco bins.
Output: --out JSON + printed text table."""
import argparse, json, math, sys
from pathlib import Path
import numpy as np
from scipy.stats import chi2 as CHI2
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a"))
import wpa_common as W

SK = W.D / "data/restart_2026-10-27/wp-a4/skims"
ALTS = ("7071", "7091", "nd4cat")
QED = [150., 300., 700., 1e9]
ABINS = [(2.0, 2.5), (2.5, 3.0), (3.0, 3.3), (3.15, 3.35), (3.40, 3.55)]
NK = 6
KEYS = ("ec08", "ecrecomp08", "eplug", "empz", "w", "q2s", "x", "ys", "etagen", "hatetagen")


def load(l):
    z = np.load(SK / (l + ".npz")); o = {k: z[k] for k in z.files}
    for k in KEYS:
        o[k] = o[k].astype(np.float64)
    return o


def clean(o):
    if isinstance(o, dict): return {k: clean(v) for k, v in o.items()}
    if isinstance(o, (list, tuple)): return [clean(v) for v in o]
    if isinstance(o, (np.floating, float)): return None if not math.isfinite(o) else float(o)
    if isinstance(o, np.integer): return int(o)
    if isinstance(o, np.ndarray): return clean(o.tolist())
    return o


def sample(e):
    return np.isfinite(e["empz"]) & (e["empz"] > 35) & (e["empz"] < 70)


def cells(e, med):
    qb = np.clip(np.searchsorted(QED, e["q2s"], side="right") - 1, 0, 2)
    return 2 * qb + (e["x"] >= np.asarray(med)[qb]).astype(int)


def wsum(m, w=None):
    return float(m.sum()) if w is None else float(w[m].sum())


def ratio(dn, dd, mn, mv, md, mdv):
    """R = (dn/dd)/(mn/md); counts and MC sums with variances (mv, mdv); returns (R, err)."""
    if min(dn, dd, mn, md) <= 0: return float("nan"), float("nan")
    r = (dn / dd) / (mn / md)
    rel = math.sqrt(1. / dn + 1. / dd + mv / mn ** 2 + mdv / md ** 2)
    return r, r * rel


def frac_ratio(dp, dt, mp, mpv, mt, mtv):
    """R = fD/fMC, fD=dp/dt binomial, fMC = mp/mt with variance from sum w^2 split."""
    if dt <= 0 or mt <= 0 or dp <= 0 or mp <= 0: return float("nan"), float("nan"), float("nan"), float("nan")
    fd = dp / dt; fm = mp / mt
    vd = fd * (1 - fd) / dt
    vm = (mpv * (1 - fm) ** 2 + (mtv - mpv) * fm ** 2) / mt ** 2
    r = fd / fm
    return r, r * math.sqrt(vd / fd ** 2 + vm / fm ** 2), fd, fm


def flat_trend(R, E, preds):
    R = np.asarray(R, float); E = np.asarray(E, float); ok = np.isfinite(R) & np.isfinite(E) & (E > 0)
    out = {"n": int(ok.sum())}
    if ok.sum() < 3: return out
    w = 1 / E[ok] ** 2; r = R[ok]; mean = float((w * r).sum() / w.sum()); c2 = float((w * (r - mean) ** 2).sum()); nd = int(ok.sum()) - 1
    out.update(mean=mean, mean_err=float(w.sum() ** -.5), chi2=c2, ndf=nd, p=float(CHI2.sf(c2, nd)))
    for pn, t in preds.items():
        t = np.asarray(t, float)[ok]
        if not np.all(np.isfinite(t)): out["trend_" + pn] = None; continue
        tb = (w * t).sum() / w.sum(); den = (w * (t - tb) ** 2).sum()
        if den <= 0: out["trend_" + pn] = None; continue
        b = float((w * (t - tb) * r).sum() / den); sb = float(den ** -.5)
        out["trend_" + pn] = {"slope": b, "sigma": sb, "z": b / sb}
    return out


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True)
    ap.add_argument("--skim-dir"); ap.add_argument("--smoke", action="store_true", help="degenerate run on whatever npz exist in --skim-dir")
    a = ap.parse_args()
    global SK
    if a.skim_dir: SK = Path(a.skim_dir)
    labs = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    avail = lambda l: (SK / (l + ".npz")).exists()
    if not a.smoke:
        miss = [l for l in W.DATA_LABELS + labs if not avail(l)]
        if miss: raise SystemExit("FATAL: missing skims %s" % miss)
    E = {l: load(l) for l in W.DATA_LABELS + labs if avail(l)}
    S = {l: sample(e) for l, e in E.items()}
    dat = [l for l in W.DATA_LABELS if l in E]
    # frozen grid: x median per Q2 bin from DATA only (MC fallback only in --smoke)
    src = dat if dat else list(E)
    med = []
    for q in range(3):
        xs = np.concatenate([E[l]["x"][S[l] & (E[l]["q2s"] >= QED[q]) & (E[l]["q2s"] < QED[q + 1])] for l in src])
        med.append(float(np.median(xs)) if xs.size else float("nan"))
    CELL = {l: cells(E[l], med) for l in E}
    res = {"grid": {"q2_edges": QED[:3] + ["inf"], "x_median_per_qbin": med, "cell_index": "2*qbin + (x>=median)"},
           "smoke": a.smoke, "inputs": {l: W.sha256(SK / (l + ".npz")) for l in E}, "script_sha256": W.sha256(__file__), "chains": {}}
    # data per-cell counts and <W>
    dc = {"n": [], "W_mean": []}
    for k in range(NK):
        n = 0; ws = 0.
        for l in dat:
            m = S[l] & (CELL[l] == k); n += int(m.sum()); ws += float(np.sqrt(E[l]["q2s"][m] * (1 - E[l]["x"][m]) / E[l]["x"][m]).sum())
        dc["n"].append(n); dc["W_mean"].append(ws / n if n else float("nan"))
    res["data_cells"] = dc
    EPS = {"B1": lambda e: np.isfinite(e["ec08"]) & (e["ec08"] >= 3.3), "B2": lambda e: e["ec08"] < 2.6}
    table = []
    for ch in ALTS:
        comps = [l for l in W.DIFF8 + W.CHAINS[ch]["nd"] if l in E]
        rs = {l: W.fac_used(l, ch) / W.table_fac(l) for l in comps}
        Wt = {l: rs[l] * E[l]["w"] for l in comps}
        def D_(fn):  # data sum of cells
            return np.array([sum(float(fn(E[l], S[l] & (CELL[l] == k)).sum()) for l in dat) for k in range(NK)])
        def M_(fn):  # (sum w, sum w^2) per cell
            s = np.zeros(NK); v = np.zeros(NK)
            for k in range(NK):
                for l in comps:
                    m = fn(E[l], S[l] & (CELL[l] == k)); s[k] += Wt[l][m].sum(); v[k] += (Wt[l][m] ** 2).sum()
            return s, v
        # predictors
        th = np.full(NK, np.nan)
        for k in range(NK):
            num = sum((Wt[l] * E[l]["hatetagen"])[S[l] & (CELL[l] == k) & (E[l]["hatetagen"] > -900)].sum() for l in comps)
            den = sum(Wt[l][S[l] & (CELL[l] == k) & (E[l]["hatetagen"] > -900)].sum() for l in comps)
            th[k] = num / den if den > 0 else np.nan
        preds = {"W": dc["W_mean"], "hatEtaMaxGen": th}
        o = {"predictors": {"W_mean_data": dc["W_mean"], "mean_hatEtaMaxGen_mc": th}, "A": {}, "B": {}, "C": {}}
        allc = lambda e, m: m
        inr = lambda lo, hi: (lambda e, m: m & (e["ec08"] >= lo) & (e["ec08"] < hi))
        dN1, (mN1, vN1) = D_(inr(1.0, 2.0)), M_(inr(1.0, 2.0)); dAll, (mAll, vAll) = D_(allc), M_(allc)
        for (lo, hi) in ABINS:
            db = D_(inr(lo, hi)); mb, vb = M_(inr(lo, hi))
            for nn, (dd, mm, vv) in (("N1", (dN1, mN1, vN1)), ("NALL", (dAll, mAll, vAll))):
                R = []; Er = []
                for k in range(NK):
                    r, er = ratio(db[k], dd[k], mb[k], vb[k], mm[k], vv[k]); R.append(r); Er.append(er)
                key = "%.2f-%.2f|%s" % (lo, hi, nn)
                o["A"][key] = {"R": R, "err": Er, "n_data_bin": db.tolist(), **flat_trend(R, Er, preds)}
                table.append((ch, "A " + key, R, Er, o["A"][key]))
        for rn, rf in EPS.items():
            reg = lambda e, m, rf=rf: m & rf(e) & np.isfinite(e["eplug"])
            dt = D_(reg); mt, vt = M_(reg)
            defs = {"f0": lambda e: e["eplug"] < 0.1, "f7": lambda e: e["eplug"] > 7.0}
            for fn, f in defs.items():
                dp = D_(lambda e, m, f=f, reg=reg: reg(e, m) & f(e)); mp, vp = M_(lambda e, m, f=f, reg=reg: reg(e, m) & f(e))
                rr = [frac_ratio(dp[k], dt[k], mp[k], vp[k], mt[k], vt[k]) for k in range(NK)]
                R = [x[0] for x in rr]; Er = [x[1] for x in rr]
                key = "%s|%s" % (rn, fn); o["B"][key] = {"R": R, "err": Er, "f_data": [x[2] for x in rr], "f_mc": [x[3] for x in rr], "n_region_data": dt.tolist(), **flat_trend(R, Er, preds)}
                table.append((ch, "B " + key, R, Er, o["B"][key]))
            reg2 = lambda e, m, rf=rf: m & rf(e)
            dt = D_(reg2); mt, vt = M_(reg2)
            for fn, g in (("failFMD", lambda e: ~e["fmd"]), ("failFTS", lambda e: ~e["fts"])):
                dp = D_(lambda e, m, g=g: reg2(e, m) & g(e)); mp, vp = M_(lambda e, m, g=g: reg2(e, m) & g(e))
                rr = [frac_ratio(dp[k], dt[k], mp[k], vp[k], mt[k], vt[k]) for k in range(NK)]
                R = [x[0] for x in rr]; Er = [x[1] for x in rr]
                key = "%s|%s" % (rn, fn); o["C"][key] = {"R": R, "err": Er, "f_data": [x[2] for x in rr], "f_mc": [x[3] for x in rr], "n_region_data": dt.tolist(), **flat_trend(R, Er, preds)}
                table.append((ch, "C " + key, R, Er, o["C"][key]))
        res["chains"][ch] = o
    # (D) MC response
    tb = np.concatenate([[-998.5], np.round(np.arange(3.0, 3.61, .1), 4), [1e9]])  # truth edges (<3.0 | 3.0..3.6 | >=3.6)
    rb = np.round(np.arange(3.0, 3.61, .1), 4)
    Dres = {}
    for l in ("9012", "7091"):
        if l not in E: continue
        e = E[l]; m = S[l] & np.isfinite(e["ec08"]); w = e["w"]
        r = {}
        for tn in ("hatetagen", "etagen"):
            t = e[tn]; mats = {}
            tcol = np.where(t < -900, 8, np.where(t < 3.0, 0, np.where(t >= 3.6, 7, 1 + np.clip(((t - 3.0) / .1).astype(int), 0, 5))))
            rrow = np.digitize(e["ec08"], rb) - 1  # 0..5 inside, -1 below, 6 above
            M = np.zeros((6, 9)); N = np.zeros((6, 9))
            for i in range(6):
                for j in range(9):
                    s = m & (rrow == i) & (tcol == j); M[i, j] = w[s].sum(); N[i, j] = s.sum()
            r[tn] = {"truth_cols": ["<3.0"] + ["%.1f-%.1f" % (3 + .1 * i, 3.1 + .1 * i) for i in range(6)] + [">=3.6", "none(-999)"],
                     "reco_rows": ["%.1f-%.1f" % (3 + .1 * i, 3.1 + .1 * i) for i in range(6)], "weighted": M.tolist(), "raw_counts": N.tolist(),
                     "row_normalised": (M / np.maximum(M.sum(1, keepdims=True), 1e-300)).tolist()}
            fine = []
            for lo in np.round(np.arange(3.0, 3.651, .05), 4):
                s = m & (e["ec08"] >= lo) & (e["ec08"] < lo + .05); sw = w[s].sum(); good = s & (t > -900)
                fine.append({"reco": "%.2f-%.2f" % (lo, lo + .05), "n": int(s.sum()), "sumw": float(sw),
                             "frac_truth_gt_3.6": float(w[s & (t > 3.6)].sum() / sw) if sw > 0 else None,
                             "frac_truth_none": float(w[s & (t < -900)].sum() / sw) if sw > 0 else None,
                             "mean_truth": float((w[good] * t[good]).sum() / w[good].sum()) if w[good].sum() > 0 else None})
            r[tn]["fine_reco_bins"] = fine
            s = m & (e["ec08"] >= 3.40) & (e["ec08"] < 3.55); sw = w[s].sum()
            r[tn]["reco_3.40_3.55_frac_truth_gt_3.6"] = float(w[s & (t > 3.6)].sum() / sw) if sw > 0 else None
            r[tn]["reco_3.40_3.55_n"] = int(s.sum())
            tr = m_t = S[l] & (t >= 3.0) & (t < 3.6); sw2 = w[tr].sum()
            r[tn]["truth_3.0_3.6_frac_reco_same_0.1bin_or_in_3.0_3.6"] = float(w[tr & np.isfinite(e["ec08"]) & (e["ec08"] >= 3.0) & (e["ec08"] < 3.6)].sum() / sw2) if sw2 > 0 else None
        Dres[l] = r
    res["D_mc_response"] = Dres
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(clean(res), open(a.out, "w"), indent=1)
    # text table
    print("grid x-medians per Q2 bin:", med, " data n per cell:", dc["n"])
    print("%-7s %-22s %s  | chi2/ndf  p     z(W)   z(hatEta)" % ("chain", "series", " ".join("   K%d      " % k for k in range(NK))))
    for ch, name, R, Er, f in table:
        zw = (f.get("trend_W") or {}).get("z"); zh = (f.get("trend_hatEtaMaxGen") or {}).get("z")
        print("%-7s %-22s %s  | %s %s %s %s" % (ch, name, " ".join("%5.3f+-%.3f" % (r, er) if math.isfinite(r) else "    nan     " for r, er in zip(R, Er)),
              ("%.1f/%d" % (f["chi2"], f["ndf"])) if "chi2" in f else "-", ("%.3f" % f["p"]) if "p" in f else "-",
              ("%+.1f" % zw) if zw is not None else "-", ("%+.1f" % zh) if zh is not None else "-"))
    for l, r in Dres.items():
        print("D %s: reco[3.40,3.55) frac truth(hatEtaMaxGen)>3.6 = %s (n=%d); with etaMaxGen = %s" % (l, r["hatetagen"]["reco_3.40_3.55_frac_truth_gt_3.6"], r["hatetagen"]["reco_3.40_3.55_n"], r["etagen"]["reco_3.40_3.55_frac_truth_gt_3.6"]))
    print(json.dumps({"out": a.out}))


if __name__ == "__main__":
    main()

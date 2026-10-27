#!/usr/bin/env python3
"""WP-A4 follow-up: cocktail composition vs detector response in the forward regions (one Condor job over the 17 skims).
DESIGN FROZEN IN THIS DOCSTRING BEFORE ANY OUTPUT WAS PRODUCED.  MC only except where data fractions are quoted.

Sample, six-cell grid (data-median x split per Q2 bin), signed-v3 weights x exposure rescale, chains 7071/7091/nd4cat,
sentinel conventions: identical to kin_scaling_wpa4.py (imported: load, sample, cells, QED).
Component classes: EL = 9012,9024,9033 ; PD = 9155,9157,9158 ; RG = 6671,6677 ; ND = chain ND labels.
Regions: B1 = finite etaMaxClus08>=3.3 ; B2 = etaMaxClus08<2.6 (incl. no-cluster).
Series: f7 = f(ePlug>7), f0 = f(ePlug<0.1) (both over finite-ePlug events of the region), failFMD = f(!selFmd), failFTS = f(!selFts)
(over all events of the region).

(1) Per region, chain, cell: class share_k = sum w_k / sum w over the region (denominator matches the series: finite-ePlug
events for f7/f0, all events for FMD/FTS); class fraction f_k with sum-w^2 error sqrt(sum_pass w^2 (1-f)^2 + sum_fail w^2 f^2)/sum w_k;
total MC fraction, data fraction (binomial), difference data-MC with combined error.
(2) Fixed-composition tests per (region, chain, series), over the 6 cells, with sigma_c^2 = sigma_data^2 + sigma_MC(total)^2:
 (a) additive: d_c = fMC_c + c, one constant c (weighted mean of d-m), chi2/ndf (ndf=5), p.
 (b) per-class scale: d_c = sum_k a_ck s_k with a_ck = sum_pass w_k / sum w (class contribution to fMC), one s_k per class (4 classes),
     weighted linear LSQ (pinv), s_k errors from the covariance, chi2/ndf (ndf=2), p.
 Rule (pre-registered): p>0.01 for (a) or (b) => kinematic dependence explained by composition plus a detector term, not evidence for generator flow.
(3) Column-normalised MC response, 9012, 7071, 7091 separately (weights w; same sample): P(reco in R | truth hatEtaMaxGen in T) with
 T = [3.0,3.3), [3.3,3.6), [3.6,4.0), >=4.0 (denominator all sample events with truth in T incl. no-cluster reco); R = [3.40,3.55) (main),
 plus extra reco bins <2.6 or none, [2.6,3.3), [3.3,3.4), [3.55,inf).  Errors sqrt(sum_R w^2 (1-P)^2 + sum_notR w^2 P^2)/sum w.
Output: --out JSON + printed table."""
import argparse, json, math, sys
from pathlib import Path
import numpy as np
from scipy.stats import chi2 as CHI2
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE)); sys.path.insert(0, str(HERE.parent / "wp-a"))
import wpa_common as W
import kin_scaling_wpa4 as K

NK = K.NK
CLS = {"EL": ["9012", "9024", "9033"], "PD": ["9155", "9157", "9158"], "RG": ["6671", "6677"]}
REG = {"B1": lambda e: np.isfinite(e["ec08"]) & (e["ec08"] >= 3.3), "B2": lambda e: e["ec08"] < 2.6}
SER = {"f7": (True, lambda e: e["eplug"] > 7.0), "f0": (True, lambda e: e["eplug"] < 0.1),
       "failFMD": (False, lambda e: ~e["fmd"]), "failFTS": (False, lambda e: ~e["fts"])}




def fit(d, vd, a, vm):
    """d,vd: data fractions and variance per cell; a: (NK, ncls) class contributions; vm: MC total variance."""
    ok = np.isfinite(d) & np.isfinite(vd) & (vd > 0) & np.all(np.isfinite(a), 1) & np.isfinite(vm)
    out = {"n_cells": int(ok.sum())}
    if ok.sum() < 4: return out
    m = a.sum(1); s2 = vd + vm; w = 1 / s2[ok]; r = (d - m)[ok]
    c = float((w * r).sum() / w.sum()); c2 = float((w * (r - c) ** 2).sum()); nd = int(ok.sum()) - 1
    out["additive"] = {"c": c, "c_err": float(w.sum() ** -.5), "chi2": c2, "ndf": nd, "p": float(CHI2.sf(c2, nd))}
    A = a[ok]; y = d[ok]; Wm = np.diag(w); cov = np.linalg.pinv(A.T @ Wm @ A); s = cov @ A.T @ Wm @ y
    c2b = float((w * (y - A @ s) ** 2).sum()); ndb = int(ok.sum()) - A.shape[1]
    out["per_class_scale"] = {"s": s.tolist(), "s_err": np.sqrt(np.diag(cov)).tolist(), "classes": ["EL", "PD", "RG", "ND"],
                              "chi2": c2b, "ndf": ndb, "p": float(CHI2.sf(c2b, ndb)) if ndb > 0 else None}
    return out


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True); ap.add_argument("--skim-dir"); ap.add_argument("--smoke", action="store_true")
    a = ap.parse_args()
    if a.skim_dir: K.SK = Path(a.skim_dir)
    labs = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    avail = lambda l: (K.SK / (l + ".npz")).exists()
    if not a.smoke:
        miss = [l for l in W.DATA_LABELS + labs if not avail(l)]
        if miss: raise SystemExit("FATAL: missing skims %s" % miss)
    E = {l: K.load(l) for l in W.DATA_LABELS + labs if avail(l)}
    S = {l: K.sample(e) for l, e in E.items()}; dat = [l for l in W.DATA_LABELS if l in E]; src = dat if dat else list(E)
    med = [float(np.median(np.concatenate([E[l]["x"][S[l] & (E[l]["q2s"] >= K.QED[q]) & (E[l]["q2s"] < K.QED[q + 1])] for l in src]))) for q in range(3)]
    CELL = {l: K.cells(E[l], med) for l in E}
    res = {"grid_x_median": med, "smoke": a.smoke, "script_sha256": W.sha256(__file__), "chains": {}}
    lines = []
    for ch in K.ALTS:
        cl = dict(CLS); cl["ND"] = W.CHAINS[ch]["nd"]
        comps = [l for k in cl for l in cl[k] if l in E]
        Wt = {l: W.fac_used(l, ch) / W.table_fac(l) * E[l]["w"] for l in comps}
        o = {"composition": {}, "fits": {}}
        for rn, rf in REG.items():
            for sn, (needplug, sf) in SER.items():
                def mask(l, k):
                    m = S[l] & (CELL[l] == k) & rf(E[l])
                    return m & np.isfinite(E[l]["eplug"]) if needplug else m
                rows = []; d = np.full(NK, np.nan); vd = np.full(NK, np.nan); A = np.full((NK, 4), np.nan); vm = np.full(NK, np.nan)
                for k in range(NK):
                    nt = sum(int(mask(l, k).sum()) for l in dat); npass = sum(int((mask(l, k) & sf(E[l])).sum()) for l in dat)
                    if nt > 0 and 0 < npass:
                        d[k] = npass / nt; vd[k] = d[k] * (1 - d[k]) / nt
                    sw = {c: sum(Wt[l][mask(l, k)].sum() for l in cl[c] if l in E) for c in cl}; tot = sum(sw.values())
                    row = {"cell": k, "data_f": d[k], "data_n": nt, "classes": {}}
                    fm = (sum(Wt[l][mask(l, k) & sf(E[l])].sum() for l in comps) / tot) if tot > 0 else np.nan
                    v = 0.
                    for ci, c in enumerate(cl):
                        t = sw[c]; p = sum(Wt[l][mask(l, k) & sf(E[l])].sum() for l in cl[c] if l in E)
                        p2 = sum((Wt[l][mask(l, k) & sf(E[l])] ** 2).sum() for l in cl[c] if l in E); t2 = sum((Wt[l][mask(l, k)] ** 2).sum() for l in cl[c] if l in E)
                        fk = p / t if t > 0 else np.nan
                        ek = math.sqrt(p2 * (1 - fk) ** 2 + (t2 - p2) * fk ** 2) / t if t > 0 else np.nan
                        row["classes"][c] = {"share": t / tot if tot > 0 else np.nan, "f": fk, "f_err": ek}
                        A[k, ci] = p / tot if tot > 0 else np.nan
                        # variance of class contribution to total
                        v += (p2 * (1 - fm) ** 2 + (t2 - p2) * fm ** 2) / tot ** 2 if tot > 0 else 0.
                    vm[k] = v; row["mc_f"] = fm; row["mc_f_err"] = math.sqrt(v); row["diff_data_minus_mc"] = d[k] - fm
                    row["diff_err"] = math.sqrt(vd[k] + v) if np.isfinite(vd[k]) else np.nan; rows.append(row)
                key = "%s|%s" % (rn, sn); o["composition"][key] = rows; fr = fit(d, vd, A, vm); o["fits"][key] = fr
                if "additive" in fr:
                    lines.append("%-7s %-11s additive c=%+.4f chi2/ndf=%.1f/%d p=%.3g | per-class s=%s chi2/ndf=%.1f/%s p=%s | shares(K0)=%s" % (
                        ch, key, fr["additive"]["c"], fr["additive"]["chi2"], fr["additive"]["ndf"], fr["additive"]["p"],
                        np.round(fr["per_class_scale"]["s"], 2).tolist(), fr["per_class_scale"]["chi2"], fr["per_class_scale"]["ndf"],
                        ("%.3g" % fr["per_class_scale"]["p"]) if fr["per_class_scale"]["p"] is not None else "-",
                        {c: round(float(row0["share"]), 3) for c, row0 in rows[0]["classes"].items()}))
        res["chains"][ch] = o
    # (3) response
    TE = [(3.0, 3.3), (3.3, 3.6), (3.6, 4.0), (4.0, 1e9)]
    RB = {"3.40-3.55": lambda r: (r >= 3.40) & (r < 3.55), "<2.6|none": lambda r: ~(r >= 2.6), "2.6-3.3": lambda r: (r >= 2.6) & (r < 3.3),
          "3.3-3.4": lambda r: (r >= 3.3) & (r < 3.4), ">=3.55": lambda r: r >= 3.55}
    resp = {}
    for l in ("9012", "7071", "7091"):
        if l not in E: continue
        e = E[l]; w = e["w"]; r = {}
        for lo, hi in TE:
            s = S[l] & (e["hatetagen"] >= lo) & (e["hatetagen"] < hi); sw = w[s].sum(); rr = {"n_truth": int(s.sum()), "sumw": float(sw)}
            for rn, fn in RB.items():
                m = s & fn(e["ec08"]); p = w[m].sum() / sw if sw > 0 else np.nan
                rr[rn] = {"P": float(p), "err": float(math.sqrt((w[m] ** 2).sum() * (1 - p) ** 2 + ((w[s] ** 2).sum() - (w[m] ** 2).sum()) * p ** 2) / sw) if sw > 0 else None}
            r["truth_%g-%g" % (lo, hi)] = rr
            lines.append("RESP %s truth[%g,%g) n=%d  P(reco 3.40-3.55)=%.4f+-%.4f  P(3.3-3.4)=%.4f P(>=3.55)=%.4f" % (l, lo, hi, rr["n_truth"], rr["3.40-3.55"]["P"], rr["3.40-3.55"]["err"] or 0, rr["3.3-3.4"]["P"], rr[">=3.55"]["P"]))
        resp[l] = r
    res["response"] = resp
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(K.clean(res), open(a.out, "w"), indent=1)
    print("\n".join(lines)); print(json.dumps({"out": a.out}))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""WP-A5 diagnostics (A5-D8 (ii)-(iv), A5-D9) from veto_corr_hists.json and veto_correction.json.  Login node.
 odd_closure   pull (q_odd - q_even) / hypot(sigma_stat_even, sigma_stat_odd) per detector x (M_X bin, D cell)
 flatness      chi2 of q about its weighted mean (plot_veto_eff.flat) over the 9 (M_X, cell) cells, over the D cells
               (all M_X) and over the M_X bins (ALL cells), per detector and split
 period        deep-gap f_data per period (ep0304, em0405, em06, ep0607) per detector and parity, chi2 about the
               weighted mean (ndf 3); a period > 3 sigma from the mean is an open item, not a gate
 chain_spread  q(7091) - q(7071), q(nd4cat) - q(7071) per cell (even and odd)
 comb_closure  deep-gap COMB (no veto, fail = any), chain 7071 full MC: f' = 1 - (pass_ND + sum pass_DIFF8 x prod_v(1-q_v)) / N
               against f_data per split and D cell; |f' - f_data| > 2 sigma is an open item
 xip04         q (even) with 0 < x_IP < 0.04 added to the deep-gap view vs nominal, pull per cell (parent step (1) window)
 qa_flags      cells where sigma_stat_even > sigma_q (Open Q-A)"""
import json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent; sys.path.insert(0, str(HERE))
from scipy.stats import chi2 as CHI2
import derive_veto_corr as DV

OD = DV.OD; C = json.load(open(DV.FULL)); h = DV.Hists()
VIEWS = DV.VIEWS; MX3 = ("4-10", "10-20", "20-40"); CELLS = DV.CELLS; PER = ("ep0304", "em0405", "em06_true", "ep0607_true")


def flat(vals):
    d = np.array([x[0] for x in vals]); e = np.array([x[1] for x in vals]); w = 1 / e ** 2
    m = np.sum(w * d) / np.sum(w); c = float(np.sum(w * (d - m) ** 2)); n = len(d) - 1
    return {"weighted_mean": float(m), "chi2": c, "ndf": n, "p": float(CHI2.sf(c, n))}


def main():
    out = {"source": str(DV.FULL), "odd_closure": {}, "flatness": {}, "period": {}, "chain_spread": {}, "comb_closure": {}, "xip04": {}, "qa_flags": []}
    for v in VIEWS:
        cv = C["cells"][v]
        out["odd_closure"][v] = {k: {"q_even": e["q"], "q_odd": e["q_odd"], "pull": (e["q_odd"] - e["q"]) / np.hypot(e["sigma_stat_even"], e["sigma_stat_odd"])}
                                 for k, e in cv.items()}
        pulls = [out["odd_closure"][v]["%s|%s" % (m, z)]["pull"] for m in MX3 for z in CELLS]
        out["odd_closure"][v]["chi2_9cells"] = {"chi2": float(np.sum(np.square(pulls))), "ndf": 9, "p": float(CHI2.sf(np.sum(np.square(pulls)), 9))}
        out["flatness"][v] = {}
        for sp, qk, sk in (("even", "q", "sigma_stat_even"), ("odd", "q_odd", "sigma_stat_odd")):
            g = lambda ks: flat([(cv[k][qk], cv[k][sk]) for k in ks])
            out["flatness"][v][sp] = {"cells_9": g(["%s|%s" % (m, z) for m in MX3 for z in CELLS]),
                                      "D_cells": g(["ALL|%s" % z for z in CELLS]), "MX_bins": g(["%s|ALL" % m for m in MX3])}
        out["chain_spread"][v] = {k: {sp: {ch: e["chain_q_" + sp][ch] - e["chain_q_" + sp]["7071"] for ch in ("7091", "nd4cat")} for sp in ("even", "odd")}
                                  for k, e in cv.items()}
        out["xip04"][v] = {k: {"q_nominal": cv[k]["q"], "q_xip04": x["q"], "n_data_xip04": x["n_data"], "n_data_nominal": cv[k]["even"]["n_data"],
                               "diff": x["q"] - cv[k]["q"]} for k, x in C["diag_xip04_cells"][v].items()}
        for k, e in cv.items():
            if e["sigma_stat_even"] > e["sigma_q"]: out["qa_flags"].append({"view": v, "cell": k, "sigma_stat_even": e["sigma_stat_even"], "sigma_q": e["sigma_q"]})
        # period stability (data only)
        out["period"][v] = {}
        for p in ("even", "odd"):
            rows = {per: h.data(v, p, [0, 1, 2], DV.DEEP, DV.MXK["ALL"], src=per) for per in PER}
            fl = flat([(r[0], r[1]) for r in rows.values()])
            dev = {per: (r[0] - fl["weighted_mean"]) / r[1] for per, r in rows.items()}
            out["period"][v][p] = {"f_data": {per: {"f": r[0], "err": r[1], "n": r[2]} for per, r in rows.items()}, "chi2": fl["chi2"], "ndf": fl["ndf"],
                                   "p": fl["p"], "weighted_mean": fl["weighted_mean"], "dev_sigma": dev, "gt_3sigma": [k for k, x in dev.items() if abs(x) > 3]}
    # COMB closure
    W = np.ones((4, 6))  # (cell, hist mx bin) -> prod_v (1 - q_v), nominal
    for v in VIEWS:
        cv = C["cells"][v]
        for c in range(3):
            for im in range(6):
                m = {1: "4-10", 2: "10-20", 3: "20-40"}.get(im, "ALL"); W[c, im] *= 1 - cv["%s|%s" % (m, CELLS[c])]["q"]
    for p in ("even", "odd"):
        out["comb_closure"][p] = {}
        for z in CELLS + ("ALL",):
            cells = DV.cidx(z); fd, ed, n = h.data("COMB", p, cells, DV.DEEP, DV.MXK["ALL"])
            ps = fl = s2p = s2f = 0.0; ps_raw = 0.0
            for c in cells:
                sd, sn = h.SC["7071"][CELLS[c]]
                for g, s in (("signal", sd), ("reggeon", sd), ("nd", sn)):
                    a = h.arr("COMB", p, "7071", g)[c][DV.DEEP].sum(axis=0); a2 = h.arr("COMB", p, "7071", g, "sumw2")[c][DV.DEEP].sum(axis=0)
                    corr = W[c] if g != "nd" else np.ones(6)
                    ps += s * float((a[:, 0] * corr).sum()); ps_raw += s * float(a[:, 0].sum()); fl += s * float(a[:, 1].sum())
                    s2p += s * s * float(a2[:, 0].sum()); s2f += s * s * float(a2[:, 1].sum())
            N = ps_raw + fl; f0 = fl / N; fp = 1 - ps / N
            em = float(np.sqrt(max(s2f * (1 - f0) ** 2 + s2p * f0 * f0, 0))) / N; sg = float(np.hypot(ed, em))
            out["comb_closure"][p][z] = {"f_data": fd, "err_data": ed, "n_data": n, "f_mc_uncorr": f0, "f_mc_corr": fp, "err_mc": em,
                                         "pull_uncorr": (f0 - fd) / sg, "pull_corr": (fp - fd) / sg, "gt_2sigma": bool(abs(fp - fd) > 2 * sg)}
    json.dump(out, open(OD / "diagnostics.json", "w"), indent=1, default=float)
    print(json.dumps({"comb": {p: {z: round(x["pull_corr"], 2) for z, x in d.items()} for p, d in out["comb_closure"].items()},
                      "period_gt3": {v: {p: out["period"][v][p]["gt_3sigma"] for p in ("even", "odd")} for v in VIEWS},
                      "period_chi2": {v: {p: round(out["period"][v][p]["chi2"], 2) for p in ("even", "odd")} for v in VIEWS},
                      "odd_chi2_9": {v: (round(out["odd_closure"][v]["chi2_9cells"]["chi2"], 2), round(out["odd_closure"][v]["chi2_9cells"]["p"], 3)) for v in VIEWS},
                      "qa_flags": [(f["view"], f["cell"]) for f in out["qa_flags"]]}, indent=0))


if __name__ == "__main__":
    main()

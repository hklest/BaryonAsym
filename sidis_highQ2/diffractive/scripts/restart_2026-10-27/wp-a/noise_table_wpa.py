#!/usr/bin/env python3
"""WP-A S2a: remedy-1 noise table from the S1 CONTROL partials only (A4/A5, D2).

Per chain (cocktail = 8 diffractive components + the chain's ND labels, signed
v3 weights, stack C) and per data period k:
  f_data(k) = 1 - N(ctrl & FMD & Plug<T & FTS)/N(ctrl)       (data period k)
  f_MC(k)   = same for the MC cocktail PROJECTED onto period k: component c
              enters with weight exposure1 * L_k/351000 if k is one of the data
              periods c represents (composite-scale projection; nd4cat 7071 ->
              em06 only), i.e. signed * (L_k/351000)/fac_table(c).
  p_naive   = f_data - f_MC                       (A4 leading-order excess)
  q         = max(0, (f_data - f_MC)/(1 - f_MC))  removal probability for a
              veto-passing MC event: exact control-region closure of the
              independent-noise model f' = 1-(1-f_MC)(1-q) (EPJC 72 (2012) 2074)
  excess_pdf(k): data minus MC (MC normalised to the data control count) N-1
              ePlug spectrum in ctrl & FMD & FTS, bins with lower edge >= T plus
              overflow, negatives clipped to 0 (a noise-hit event fails the veto
              by construction).
Diagnostics only: per-detector fail excesses, eta_max strata, and the
sim-period variant (MC grouped by own RunNumber period).
Inputs are ONLY s1_control partials (test no_test_tuning); output JSON carries
every input sha256.
"""
import argparse, json, math
from pathlib import Path
import numpy as np
import wpa_common as W


def ratio_err(F, P, VF, VP):
    """f = F/(F+P) with independent weighted sums F, P (variances VF, VP)."""
    N = F + P
    if N <= 0:
        return float("nan"), float("nan")
    return F / N, math.sqrt(max(P * P * VF + F * F * VP, 0.0)) / N ** 2


def stat(o):
    return o["signed"], o["sumw2"]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--control-dir", default=str(W.OUT / "s1_control/partials/control"))
    ap.add_argument("--chains", default="7071,7091,nd4cat,meps3")
    ap.add_argument("--threshold", type=float, default=7.0)
    ap.add_argument("--out", default=str(W.OUT / "s2_remedy1/noise_table.json"))
    a = ap.parse_args()
    cdir = Path(a.control_dir)
    if "s1_control" not in str(cdir):
        raise SystemExit("FATAL: noise table must be derived from s1_control partials only")
    L = W.period_lumi()
    labels = sorted({l for ch in a.chains.split(",") for l in W.DIFF8 + W.CHAINS[ch]["nd"]})
    parts = {l: W.load_partial(cdir, l) for l in labels + W.DATA_LABELS}
    for l, p in parts.items():
        if p["wpa"]["split"] != "control" or p["wpa"]["plug_threshold"] != a.threshold or p["wpa"]["removal"]["applied"]:
            raise SystemExit("FATAL: %s is not an uncorrected control partial at T=%s" % (l, a.threshold))
    inputs = {l: {"path": str(cdir / l / "partial.json"), "sha256": W.sha256(cdir / l / "partial.json")} for l in parts}
    edges = np.asarray(parts["ep0304"]["wpa"]["control_stats"]["ALL"]["n1_hists"]["ePlug"]["edges"], float)
    above = np.r_[edges[:-1] >= a.threshold - 1e-9, True]          # + overflow slot

    def hist(cs, w="signed"):
        h = cs["n1_hists"]["ePlug"]
        return np.r_[np.asarray(h[w], float), h["overflow_" + w]]

    # ---- data per period ----
    data = {}
    for k in W.PERIODS:
        cs = parts[k]["wpa"]["control_stats"][k]
        N, P = cs["ctrl"]["raw"], cs["ctrl_stack_pass"]["raw"]
        f = 1 - P / N
        o = {"n_ctrl": N, "n_pass": P, "f": f, "f_err": math.sqrt(f * (1 - f) / N),
             "eplug_hist": hist(cs, "raw"),
             "det": {d: cs["ctrl_%s_fail" % d]["raw"] / N for d in ("plug", "fmd", "fts")},
             "strata": {s: (1 - v["ctrl_stack_pass"]["raw"] / v["ctrl"]["raw"] if v["ctrl"]["raw"] else None, v["ctrl"]["raw"])
                        for s, v in cs["strata"].items()}}
        data[k] = o
    # sub-period diagnostic (ep0607 halves)
    sub = {}
    for g in ("ep0607_true|fall-06", "ep0607_true|2007"):
        cs = parts["ep0607_true"]["wpa"]["control_stats"][g]
        N, P = cs["ctrl"]["raw"], cs["ctrl_stack_pass"]["raw"]
        f = 1 - P / N; sub[g] = {"n_ctrl": N, "f": f, "f_err": math.sqrt(f * (1 - f) / N)}

    chains = {}
    for ch in a.chains.split(","):
        comps = W.DIFF8 + W.CHAINS[ch]["nd"]
        rep = {l: W.represented(l, ch) for l in comps}
        out = {"cocktail": comps, "components": {}, "q": {}, "p_naive": {}, "per_period": {}, "excess_pdf": {}}
        for l in comps:
            out["components"][l] = {"represented_periods": rep[l],
                                    "period_probs": [L[k] / sum(L[j] for j in rep[l]) for k in rep[l]],
                                    "fac_used": W.fac_used(l, ch), "fac_table": W.table_fac(l)}
        for k in W.PERIODS:
            F = P = VF = VP = 0.0; det = {d: 0.0 for d in ("plug", "fmd", "fts")}; Nw = 0.0
            h = np.zeros(len(edges)); strata = {}
            members = [l for l in comps if k in rep[l]]
            for l in members:
                cs = parts[l]["wpa"]["control_stats"]["ALL"]
                r = (L[k] / W.L_DATA) / W.table_fac(l)
                n, vn = stat(cs["ctrl"]); pp, vp = stat(cs["ctrl_stack_pass"])
                P += r * pp; VP += r * r * vp; F += r * (n - pp); VF += r * r * (vn - vp); Nw += r * n
                for d in det:
                    det[d] += r * cs["ctrl_%s_fail" % d]["signed"]
                h += r * hist(cs)
                for s, v in cs["strata"].items():
                    t = strata.setdefault(s, [0.0, 0.0]); t[0] += r * v["ctrl"]["signed"]; t[1] += r * v["ctrl_stack_pass"]["signed"]
            fm, fme = ratio_err(F, P, VF, VP)
            dk = data[k]
            pn = dk["f"] - fm; pn_err = math.hypot(dk["f_err"], fme)
            q = max(0.0, pn / (1 - fm)); q_err = pn_err / (1 - fm)
            exc = np.clip(dk["eplug_hist"] - h * (dk["n_ctrl"] / Nw), 0, None) * above
            out["q"][k] = q; out["p_naive"][k] = pn
            out["excess_pdf"][k] = (exc / exc.sum()).tolist() if exc.sum() > 0 else (dk["eplug_hist"] * above / (dk["eplug_hist"] * above).sum()).tolist()
            out["per_period"][k] = {
                "members": members, "f_data": dk["f"], "f_data_err": dk["f_err"], "n_ctrl_data": dk["n_ctrl"],
                "f_mc": fm, "f_mc_err": fme, "mc_ctrl_signed": Nw, "data_over_mc_ctrl": dk["n_ctrl"] / Nw,
                "p_naive": pn, "p_err": pn_err, "q": q, "q_err": q_err,
                "excess_counts_above_T": float(exc.sum()), "excess_pdf_fallback": bool(exc.sum() <= 0),
                "per_detector_excess": {d: dk["det"][d] - det[d] / Nw for d in det},
                "eta_strata_excess": {s: (dk["strata"][s][0] - (1 - t[1] / t[0]) if t[0] > 0 and dk["strata"][s][0] is not None else None)
                                      for s, t in strata.items()},
                "eta_strata_n_data": {s: dk["strata"][s][1] for s in strata}}
        out["inapplicable"] = all(out["per_period"][k]["p_naive"] < out["per_period"][k]["p_err"] for k in W.PERIODS)
        # sim-period diagnostic: MC grouped by own RunNumber period, signed weights as stored
        simp = {}
        for k in W.PERIODS:
            F = P = 0.0
            for l in comps:
                cs = parts[l]["wpa"]["control_stats"].get(k)
                if cs and cs["ctrl"]["raw"]:
                    P += cs["ctrl_stack_pass"]["signed"]; F += cs["ctrl"]["signed"] - cs["ctrl_stack_pass"]["signed"]
            simp[k] = {"f_mc_simperiod": (F / (F + P) if F + P > 0 else None),
                       "p_naive_simperiod": (data[k]["f"] - F / (F + P) if F + P > 0 else None)}
        out["diagnostic_sim_period"] = simp
        chains[ch] = out
    res = {"schema": "wpa-noise-table-v1", "threshold": a.threshold, "eplug_edges": edges.tolist(),
           "pdf_layout": "len(edges)-1 grid bins + 1 overflow slot (drawn value 75 GeV)",
           "period_lumi_nb": L, "inputs": inputs, "data": {k: {kk: (vv.tolist() if isinstance(vv, np.ndarray) else vv)
                                                              for kk, vv in v.items()} for k, v in data.items()},
           "ep0607_subperiods": sub, "chains": chains}
    o = Path(a.out); o.parent.mkdir(parents=True, exist_ok=True)
    json.dump(res, open(o, "w"), indent=1)
    sha = W.sha256(o); (o.parent / (o.name + ".sha256")).write_text(sha + "  " + o.name + "\n")
    # bounds test
    ok = all(0 <= c["q"][k] <= 1 and c["per_period"][k]["p_err"] > 0 for c in chains.values() for k in W.PERIODS)
    print(json.dumps({"out": str(o), "sha256": sha, "noise_table_bounds": "PASS" if ok else "FAIL",
                      "q": {ch: {k: [round(c["per_period"][k]["f_data"], 4), round(c["per_period"][k]["f_mc"], 4),
                                     round(c["q"][k], 4), round(c["per_period"][k]["q_err"], 4)] for k in W.PERIODS}
                            for ch, c in chains.items()},
                      "inapplicable": {ch: c["inapplicable"] for ch, c in chains.items()}}, indent=1))
    raise SystemExit(0 if ok else 1)


if __name__ == "__main__":
    main()

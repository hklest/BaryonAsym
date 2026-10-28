#!/usr/bin/env python3
"""WP-D2 tier 1: SW77 -> SW84 detector-level check.  8405 (06/07 e+, SW77, 840x) vs 9012 (06/07 e+, SW84) on the WP-A3 skims
(A_pre: passRecoEnv, Q2s>150, 0.1<Ys<0.9, |vtxz|<35).  Counts are unweighted (every event of one sample carries one constant weight).
Stages: --stage hist (npz -> json), --stage plot (json -> figures + tables).
Selections: S0 = A_pre; S1 = S0 & xIp<0.04 & mx>3 (LRG-like region).  N-1 definitions = fact_wpa3.masks without parity."""
import argparse, json, sys
from pathlib import Path
import numpy as np
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
SK = D / "data/restart_2026-10-27/wp-a3/skims"; OUT = D / "data/restart_2026-10-27/wp-d2"; PL = D / "plots/restart_2026-10-27/wp-d2"
A, B = "8405", "9012"
ETAE = [-1.0, 1.0, 2.0, 2.5, 3.0, 3.3]; ENAME = ["[-1,1)", "[1,2)", "[2,2.5)", "[2.5,3.0)", "[3.0,3.3)", "other"]
SIGQ = {"Plug": 0.034, "FTS": 0.024, "FMD": 0.043}            # sigma_q ALL cell, g1pp_a_verdict.md:144-146
QALL = {"Plug": 0.069, "FTS": 0.048, "FMD": -0.006}            # deep-gap q ALL|ALL (even), same lines
# name: (array key, edges, transform) -- last bin of each variable also collects overflow, first collects underflow
VARS = {"etaMax08": ("ec08", np.arange(-1.0, 4.51, 0.25)), "etaMax04": ("ec04", np.arange(-1.0, 4.51, 0.25)),
        "ePlug": ("eplug", np.r_[np.arange(0, 20.1, 1.0), 30, 60, 120]), "nFmu12": ("nfmu12", np.arange(-0.5, 10.6, 1.0)),
        "nFts28": ("nfts28", np.arange(-0.5, 5.6, 1.0)), "xIp": ("xip", np.geomspace(2e-3, 0.04, 13)),
        "mx": ("mx", np.r_[np.arange(3, 30, 3.0), 40, 60, 100]), "vtxz": ("vz", np.arange(-35, 35.1, 5.0))}


def load(l):
    z = np.load(SK / (l + ".npz")); return {k: z[k] for k in z.files}


def sel(e, s):
    m = np.ones(e["xip"].size, bool)
    if s == "S1": m = (e["xip"] < 0.04) & (e["mx"] > 3)
    return m


def hist(x, ed):
    x = np.clip(np.asarray(x, float), ed[0] + 1e-9, ed[-1] - 1e-9); return np.histogram(x, ed)[0]


def vetoes(e):
    plg = e["eplug"] < 7.0; ec = e["ec08"]; etc = (ec < 3.3) | (ec < -900)
    return {"Plug": (etc & e["fmd"] & e["fts"], ~plg), "FMD": (etc & plg & e["fts"], ~e["fmd"]), "FTS": (etc & e["fmd"] & plg, ~e["fts"]),
            "COMB": (etc, ~plg | ~e["fmd"] | ~e["fts"])}


def ebin(x):
    i = np.searchsorted(ETAE, x, side="right") - 1
    return np.where(np.isfinite(x) & (x >= ETAE[0]) & (x < ETAE[-1]), i, len(ETAE) - 1)


def do_hist():
    res = {"inputs": {l: str(SK / (l + ".npz")) for l in (A, B)}, "n": {}, "dist": {}, "veto": {}, "etabins": ETAE}
    for l in (A, B):
        e = load(l); res["n"][l] = {"S0": int(sel(e, "S0").sum()), "S1": int(sel(e, "S1").sum())}
        for s in ("S0", "S1"):
            m = sel(e, s)
            for v, (k, ed) in VARS.items():
                res["dist"].setdefault(s, {}).setdefault(v, {"edges": ed.tolist()})[l] = hist(e[k][m], ed).tolist()
            # eta_max(0.4) after all three vetoes (the fit-template quantity), same binning
            pas = m & (e["eplug"] < 7.0) & e["fmd"] & e["fts"]
            res["dist"][s].setdefault("etaMax04_vetoed", {"edges": VARS["etaMax04"][1].tolist()})[l] = hist(e["ec04"][pas], VARS["etaMax04"][1]).tolist()
            res["dist"][s].setdefault("etaMax08_vetoed", {"edges": VARS["etaMax08"][1].tolist()})[l] = hist(e["ec08"][pas], VARS["etaMax08"][1]).tolist()
            for vn, (base, fail) in vetoes(e).items():
                b = m & base; ib = ebin(e["ec08"]); arr = np.zeros((len(ETAE), 2, 4))      # [eta bin, fail, cell(D0..D2,out)]
                c = np.where(e["cell"] >= 0, e["cell"], 3)
                np.add.at(arr, (ib[b], fail[b].astype(int), c[b]), 1)
                res["veto"].setdefault(s, {}).setdefault(vn, {})[l] = arr.tolist()
    OUT.mkdir(parents=True, exist_ok=True); json.dump(res, open(OUT / "tier1_hists.json", "w"))
    print(json.dumps(res["n"]))


def chi2(a, b):
    from scipy.stats import chi2 as C
    a = np.asarray(a, float); b = np.asarray(b, float); na, nb = a.sum(), b.sum()
    ok = (a + b) > 0
    # two-sample chi2 for unequal totals (Numerical Recipes): sum (sqrt(nb/na) a - sqrt(na/nb) b)^2/(a+b)
    x = ((np.sqrt(nb / na) * a - np.sqrt(na / nb) * b) ** 2 / np.where(ok, a + b, 1))[ok].sum(); n = int(ok.sum()) - 1
    return float(x), n, float(C.sf(x, n))


def binom(k, n):
    if n <= 0: return np.nan, np.nan
    f = k / n; kk = min(max(k, 0.5), n - 0.5)
    return f, float(np.sqrt(kk * (1 - kk / n)) / n)


def do_plot():
    import matplotlib; matplotlib.use("Agg"); import matplotlib.pyplot as plt
    R = json.load(open(OUT / "tier1_hists.json")); PL.mkdir(parents=True, exist_ok=True)
    plt.rcParams.update({"font.size": 9, "axes.grid": True, "grid.alpha": .25, "figure.dpi": 130})
    cA, cB = "#1b6ca8", "#d1495b"
    summ = {"n": R["n"], "shape": {}, "veto": {}, "q_shift": {}}
    figs = {"fig1_etamax": ["etaMax08", "etaMax04", "etaMax08_vetoed", "etaMax04_vetoed"], "fig2_forward": ["ePlug", "nFmu12", "nFts28"],
            "fig3_kin": ["xIp", "mx", "vtxz"]}
    for s in ("S1", "S0"):
        for fn, names in figs.items():
            fig, ax = plt.subplots(2, len(names), figsize=(3.6 * len(names), 4.6), gridspec_kw={"height_ratios": [3, 1.2], "hspace": .05}, squeeze=False)
            for j, v in enumerate(names):
                H = R["dist"][s][v]; ed = np.asarray(H["edges"]); a = np.asarray(H[A], float); b = np.asarray(H[B], float)
                pa, pb = a / a.sum(), b / b.sum(); ea, eb = np.sqrt(a) / a.sum(), np.sqrt(b) / b.sum()
                w = np.diff(ed); ctr = .5 * (ed[1:] + ed[:-1]); x2, nd, p = chi2(a, b)
                summ["shape"].setdefault(s, {})[v] = {"chi2": x2, "ndf": nd, "p": p, "n_A": int(a.sum()), "n_B": int(b.sum())}
                ax[0, j].stairs(pa / w, ed, color=cA, label=f"8405 SW77 (n={int(a.sum())})"); ax[0, j].stairs(pb / w, ed, color=cB, label=f"9012 SW84 (n={int(b.sum())})")
                ax[0, j].errorbar(ctr, pb / w, eb / w, fmt="none", ecolor=cB, alpha=.6, lw=.8)
                ax[0, j].set_yscale("log" if v in ("ePlug", "nFmu12", "nFts28") else "linear"); ax[0, j].set_title(f"{v}  chi2/ndf={x2:.1f}/{nd}", fontsize=9)
                ax[0, j].set_xticklabels([]); ax[0, j].set_ylabel("fraction / unit" if j == 0 else "")
                r = np.where(pb > 0, pa / np.where(pb > 0, pb, 1), np.nan); er = r * np.sqrt(np.where(a > 0, 1 / np.where(a > 0, a, 1), 0) + np.where(b > 0, 1 / np.where(b > 0, b, 1), 0))
                ax[1, j].cla(); ax[1, j].grid(alpha=.25)
                band = np.where(b > 0, 1 / np.sqrt(np.where(b > 0, b, 1)), np.nan)          # 9012 MC-stat band around 1
                ax[1, j].fill_between(ctr, 1 - band, 1 + band, step="mid", color=cB, alpha=.2, lw=0)
                ax[1, j].errorbar(ctr, r, er, fmt="o", ms=3, color=cA, lw=.8); ax[1, j].axhline(1, color="k", lw=.6)
                ax[1, j].set_ylim(0.4, 1.6); ax[1, j].set_xlabel(v); ax[1, j].set_ylabel("8405 / 9012" if j == 0 else "")
                if v == "ePlug": ax[1, j].set_xscale("linear")
            ax[0, 0].legend(fontsize=7, frameon=False)
            fig.suptitle(f"SW77 vs SW84, 06/07 e+, selection {s} ({'A_pre' if s == 'S0' else 'A_pre, xIp<0.04, mx>3'}); shaded: 9012 MC-stat band", fontsize=9, y=.99)
            fig.savefig(PL / f"{fn}_{s}.png", bbox_inches="tight"); fig.savefig(PL / f"{fn}_{s}.pdf", bbox_inches="tight"); plt.close(fig)
    # veto fail fractions
    for s in ("S0", "S1"):
        fig, ax = plt.subplots(2, 4, figsize=(14, 5.4), gridspec_kw={"height_ratios": [3, 1.3], "hspace": .08}, squeeze=False)
        for j, vn in enumerate(("Plug", "FMD", "FTS", "COMB")):
            f = {}
            for l in (A, B):
                arr = np.asarray(R["veto"][s][vn][l]).sum(axis=2)            # [eta, fail]
                f[l] = [binom(arr[i, 1], arr[i].sum()) + (int(arr[i].sum()),) for i in range(len(ETAE))]
            fa = np.array([x[0] for x in f[A]]); ea = np.array([x[1] for x in f[A]]); fb = np.array([x[0] for x in f[B]]); eb = np.array([x[1] for x in f[B]])
            xs = np.arange(len(ETAE))
            ax[0, j].errorbar(xs - .08, fa, ea, fmt="o", color=cA, label="8405 SW77"); ax[0, j].errorbar(xs + .08, fb, eb, fmt="s", color=cB, label="9012 SW84")
            ax[0, j].set_title(f"{vn} fail fraction (N-1)" if vn != "COMB" else "any veto fails (no veto applied)", fontsize=9); ax[0, j].set_xticklabels([])
            d = fa - fb; ed = np.hypot(ea, eb); ax[1, j].errorbar(xs, d, ed, fmt="o", color="k", ms=3)
            if vn in SIGQ: ax[1, j].axhspan(-SIGQ[vn], SIGQ[vn], color="gold", alpha=.3, label="sigma_q (ALL)")
            ax[1, j].axhline(0, color="k", lw=.6); ax[1, j].set_xticks(xs); ax[1, j].set_xticklabels(ENAME, rotation=30, fontsize=7); ax[1, j].set_xlabel("etaMaxClus08 bin")
            ax[1, j].set_ylabel("f(8405) - f(9012)" if j == 0 else "")
            tot = {}
            for l in (A, B):
                arr = np.asarray(R["veto"][s][vn][l]).sum(axis=2); tot[l] = arr
            rows = {}
            for tag, idx in (("deep_gap[-1,2.5)", [0, 1, 2]), ("all_eta<3.3", [0, 1, 2, 3, 4, 5]), ("eta[2.5,3.3)", [3, 4])):
                ra = tot[A][idx].sum(0); rb = tot[B][idx].sum(0)
                (pa_, sa_), (pb_, sb_) = binom(ra[1], ra.sum()), binom(rb[1], rb.sum())
                rows[tag] = {"f_8405": pa_, "e_8405": sa_, "n_8405": int(ra.sum()), "f_9012": pb_, "e_9012": sb_, "n_9012": int(rb.sum()), "delta": pa_ - pb_, "e_delta": float(np.hypot(sa_, sb_))}
            per_bin = [{"bin": ENAME[i], "f_8405": f[A][i][0], "e_8405": f[A][i][1], "n_8405": f[A][i][2], "f_9012": f[B][i][0], "e_9012": f[B][i][1], "n_9012": f[B][i][2]} for i in range(len(ETAE))]
            # per D cell, deep gap
            cells = {}
            for c in range(3):
                ra = np.asarray(R["veto"][s][vn][A])[:3, :, c].sum(0); rb = np.asarray(R["veto"][s][vn][B])[:3, :, c].sum(0)
                (pa_, sa_), (pb_, sb_) = binom(ra[1], ra.sum()), binom(rb[1], rb.sum())
                cells["D%d" % c] = {"f_8405": pa_, "f_9012": pb_, "delta": pa_ - pb_, "e_delta": float(np.hypot(sa_, sb_)), "n_9012": int(rb.sum())}
            summ["veto"].setdefault(s, {})[vn] = {"integrated": rows, "per_eta_bin": per_bin, "deep_gap_per_cell": cells}
            if vn in SIGQ and s == "S0":
                r = rows["deep_gap[-1,2.5)"]
                dq = -r["delta"] / (1 - r["f_9012"])                      # q = (f_data - f_MC)/(1 - f_MC); template 9012 -> 8405 moves f_MC by +delta (P_sig <= 1)
                summ["q_shift"][vn] = {"delta_f_deep": r["delta"], "e_delta_f": r["e_delta"], "dq_upper": dq, "e_dq": r["e_delta"] / (1 - r["f_9012"]),
                                       "sigma_q_ALL": SIGQ[vn], "q_ALL": QALL[vn], "ratio_dq_over_sigma_q": abs(dq) / SIGQ[vn],
                                       "pull_dq": dq / (r["e_delta"] / (1 - r["f_9012"]))}
        ax[0, 0].legend(fontsize=8, frameon=False); ax[1, 0].legend(fontsize=7, frameon=False)
        fig.suptitle(f"Forward-veto fail fractions per etaMaxClus08 bin, SW77 (8405) vs SW84 (9012), 06/07 e+, selection {s}", fontsize=9)
        fig.savefig(PL / f"fig4_veto_fail_{s}.png", bbox_inches="tight"); fig.savefig(PL / f"fig4_veto_fail_{s}.pdf", bbox_inches="tight"); plt.close(fig)
    # eta04 < 3.0 template fraction after the three vetoes
    for s in ("S0", "S1"):
        H = R["dist"][s]["etaMax04_vetoed"]; ed = np.asarray(H["edges"]); k = ed[1:] <= 3.0 + 1e-9
        out = {}
        for l in (A, B):
            h = np.asarray(H[l], float); out[l] = binom(h[k].sum(), h.sum())
        summ.setdefault("eta04_lt_3p0", {})[s] = {"f_8405": out[A][0], "e_8405": out[A][1], "f_9012": out[B][0], "e_9012": out[B][1],
                                                  "delta": out[A][0] - out[B][0], "e_delta": float(np.hypot(out[A][1], out[B][1]))}
    json.dump(summ, open(OUT / "tier1_summary.json", "w"), indent=1)
    print(json.dumps({"q_shift": summ["q_shift"], "eta04": summ["eta04_lt_3p0"]}, indent=1))


if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("--stage", choices=("hist", "plot"), required=True); a = ap.parse_args()
    do_hist() if a.stage == "hist" else do_plot()

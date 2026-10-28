#!/usr/bin/env python3
"""Veto-efficiency view figures (fig6, fig7) and table veto_eff.json.  Login node: reads stored JSON only
(veto_eff_hists.json from veto_eff_hist.py on Condor, and the WP-A2 S4 fine two-template fit results).
MC "full" = s_diff(cell) x DIFF8 + s_ND(cell) x ND(chain) with the G1'(c) fitted scales per D cell
(fine binning; same weight basis as hist_ge.py, checked); "unit" = both scales 1 (raw prediction).
ALL = D0+D1+D2.  Fail fraction f = fail/(pass+fail); data binomial stat; MC stat from sumw2:
var f = (S2_fail (1-f)^2 + S2_pass f^2) / N^2.  Purity = DIFF8 share of the full-MC N-1 sample.
Pass-efficiency ratio = (1 - f_data)/(1 - f_MC): the factor a per-event-normalised spectrum sees if it varies with kinematics.
COMB = no veto applied, fail = any of the three.  Remedy-1 (WP-A S2, excluded as a correction; shown for reference):
f' = f + qbar (1 - f) with qbar = luminosity-weighted mean of the per-period q of the chain (noise_table.json);
applied to COMB, and to the Plug N-1 under the A5 attribution (whole stack excess into Plug)."""
import json, sys
from pathlib import Path
import numpy as np
from scipy.stats import chi2 as CHI2
HERE = Path(__file__).resolve().parent; sys.path.insert(0, str(HERE))
import ge_style as S
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
VJ = D / "data/restart_2026-10-27/gate_evidence/veto_eff_hists.json"
FITS = D / "data/restart_2026-10-27/wp-a2/s4_g1p/fits"
OUT = D / "plots/restart_2026-10-27/gate_evidence"; TAB = D / "data/restart_2026-10-27/gate_evidence/veto_eff.json"
CHAINS = ("7071", "7091", "nd4cat"); CELLS = ("D0", "D1", "D2"); VETOS = ("N1_PLUG", "N1_FMD", "N1_FTS", "COMB")
VLAB = {"N1_PLUG": "Plug: fail if $E_{Plug}\\geq$7 GeV or none", "N1_FMD": "FMD: fail if not selFmd", "N1_FTS": "FTS: fail if nFts28 > 0", "COMB": "combined: fail if any of the three"}
BLAB = ["[-1,1)", "[1,2)", "[2,2.5)", "[2.5,3.0)", "[3.0,3.3)"]
H = json.load(open(VJ)); EE = np.asarray(H["eta_edges"])
SC = {ch: {z: (lambda r: (r["s_diffractive"], r["s_ND"]))(json.load(open(FITS / f"section_7_1_twofree_fit_results_{ch}_fine_{z}.json"))["fine"][ch][z])
           for z in CELLS} for ch in CHAINS}
NT = json.load(open(D / "data/restart_2026-10-27/wp-a/s2_remedy1/noise_table.json")); LUM = NT["period_lumi_nb"]
QBAR = {ch: sum(LUM[k] * NT["chains"][ch]["q"][k] for k in LUM) / sum(LUM.values()) for ch in CHAINS}


def arr(v, p, src, g, k="sumw"):
    return np.asarray(H["arrays"]["|".join((v, p, src, g))][k], float)   # (cell4, eta6, mx4, fail2)


def red(a, cells, eta, mx):
    """sum over chosen cells (list of idx), eta bins (list), mx bins (list) -> (pass, fail)."""
    return a[np.ix_(cells, eta, mx, [0, 1])].sum(axis=(0, 1, 2))


def frac_data(v, p, cells, eta, mx):
    ps, fl = red(arr(v, p, "data", "data"), cells, eta, mx); n = ps + fl
    f = fl / n if n > 0 else np.nan
    return f, np.sqrt(f * (1 - f) / n) if n > 0 else np.nan, n


def mc_sums(v, p, ch, cells, eta, mx, which, unit=False):
    """which: 'full' | 'diff' | 'nd'.  Returns (pass, fail, S2pass, S2fail) with per-cell scales."""
    o = np.zeros(4)
    for c in cells:
        sd, sn = (1.0, 1.0) if (unit or c > 2) else SC[ch][CELLS[c]]
        for g, s in (("signal", sd), ("reggeon", sd), ("nd", sn)):
            if which == "diff" and g == "nd": continue
            if which == "nd" and g != "nd": continue
            o += np.concatenate([s * red(arr(v, p, ch, g), [c], eta, mx), s * s * red(arr(v, p, ch, g, "sumw2"), [c], eta, mx)])
    return o


def frac_mc(v, p, ch, cells, eta, mx, which, unit=False):
    ps, fl, s2p, s2f = mc_sums(v, p, ch, cells, eta, mx, which, unit); n = ps + fl
    if n <= 0: return np.nan, np.nan
    f = fl / n; return f, np.sqrt(max(s2f * (1 - f) ** 2 + s2p * f * f, 0)) / n


def cell_idx(z): return [0, 1, 2] if z == "ALL" else [CELLS.index(z)]


ALLMX = [0, 1, 2, 3]


def entry(v, p, cells, eta, mx):
    fd, ed, n = frac_data(v, p, cells, eta, mx)
    o = {"data": {"f": fd, "err": ed, "n": n}}
    for ch in CHAINS:
        ff, ef = frac_mc(v, p, ch, cells, eta, mx, "full"); fdf, edf = frac_mc(v, p, ch, cells, eta, mx, "diff")
        fu, eu = frac_mc(v, p, ch, cells, eta, mx, "full", unit=True)
        ps, fl, _, _ = mc_sums(v, p, ch, cells, eta, mx, "full"); pd, fld, _, _ = mc_sums(v, p, ch, cells, eta, mx, "diff")
        o[ch] = {"f_full": ff, "err_full": ef, "f_diff8": fdf, "err_diff8": edf, "f_full_unitscale": fu, "err_full_unitscale": eu,
                 "purity_diff8": (pd + fld) / (ps + fl) if ps + fl > 0 else np.nan,
                 "delta_data_minus_full": fd - ff, "err_delta": float(np.hypot(ed, ef)), "ratio_data_over_full": fd / ff if ff > 0 else np.nan,
                 "delta_data_minus_diff8": fd - fdf, "err_delta_diff8": float(np.hypot(ed, edf)),
                 "pass_eff_ratio_data_over_full": (1 - fd) / (1 - ff), "err_pass_eff_ratio": (1 - fd) / (1 - ff) * float(np.hypot(ed / (1 - fd), ef / (1 - ff)))}
        if v in ("COMB", "N1_PLUG"):
            fr = ff + QBAR[ch] * (1 - ff)
            o[ch].update({"f_full_remedy1": fr, "delta_data_minus_full_remedy1": fd - fr, "err_delta_remedy1": float(np.hypot(ed, (1 - QBAR[ch]) * ef))})
    return o


def flat(vals):
    """chi2 of deltas about their weighted mean; returns mean, chi2, ndf, p."""
    d = np.array([x[0] for x in vals]); e = np.array([x[1] for x in vals]); w = 1 / e ** 2
    m = np.sum(w * d) / np.sum(w); c = float(np.sum(w * (d - m) ** 2)); n = len(d) - 1
    return {"weighted_mean": float(m), "chi2": c, "ndf": n, "p": float(CHI2.sf(c, n))}


def main():
    T = {"source": str(VJ), "source_script_sha256": H["script_sha256"], "eta_bins": BLAB, "eta_other_bin": "eta08 < -1 or sentinel",
         "mx_edges_GeV": H["mx_edges"], "fit_scales_fine": SC, "remedy1_qbar_lumiweighted": QBAR,
         "per_eta_index": BLAB + ["other (eta08<-1 or sentinel)", "whole N-1 sample", "[-1,3.3) (bins 0-4)"], "per_eta": {}, "deep_gap_eta_lt_2p5": {}, "kinematics": {}, "checks": {}}
    for p in ("odd", "even"):
        for v in VETOS:
            for z in ("ALL",) + CELLS:
                T["per_eta"]["|".join((v, p, z))] = [entry(v, p, cell_idx(z), [i], ALLMX) for i in range(5)] + \
                    [entry(v, p, cell_idx(z), [5], ALLMX), entry(v, p, cell_idx(z), list(range(6)), ALLMX), entry(v, p, cell_idx(z), list(range(5)), ALLMX)]
                T["deep_gap_eta_lt_2p5"]["|".join((v, p, z))] = entry(v, p, cell_idx(z), [0, 1, 2], ALLMX)
            kin = {z: entry(v, p, cell_idx(z), [0, 1, 2], ALLMX) for z in CELLS}
            kin.update({"MX%d" % (j + 1): entry(v, p, [0, 1, 2], [0, 1, 2], [j]) for j in range(3)})
            kin["flatness"] = {ch: {ax: flat([(kin[k][ch]["delta_data_minus_full"], kin[k][ch]["err_delta"]) for k in keys])
                                    for ax, keys in (("D_cells", CELLS), ("MX_bins", ("MX1", "MX2", "MX3")))} for ch in CHAINS}
            kin["flatness_pass_eff_ratio"] = {ch: {ax: flat([(kin[k][ch]["pass_eff_ratio_data_over_full"], kin[k][ch]["err_pass_eff_ratio"]) for k in keys])
                                    for ax, keys in (("D_cells", CELLS), ("MX_bins", ("MX1", "MX2", "MX3")))} for ch in CHAINS}
            T["kinematics"]["|".join((v, p))] = kin
    # consistency: integrated odd Plug N-1 f_fail per D cell vs fig2 (summary.json) and hist_ge checks
    SM = json.load(open(D / "data/restart_2026-10-27/gate_evidence/summary.json"))
    for ch in CHAINS:
        T["checks"]["fig2_" + ch] = {z: {"data_here": T["per_eta"]["N1_PLUG|odd|" + z][6]["data"]["f"], "data_fig2": SM["fig2_" + ch][z]["f_fail_data"],
                                          "mc_unit_here": T["per_eta"]["N1_PLUG|odd|" + z][6][ch]["f_full_unitscale"], "mc_fig2": SM["fig2_" + ch][z]["f_fail_mc"],
                                          "data_plug_eta_m1_3p3": T["per_eta"]["N1_PLUG|odd|" + z][7]["data"]["f"],
                                          "data_comb_eta_m1_3p3": T["per_eta"]["COMB|odd|" + z][7]["data"]["f"],
                                          "mc_unit_comb_eta_m1_3p3": T["per_eta"]["COMB|odd|" + z][7][ch]["f_full_unitscale"]}
                                     for z in CELLS}
        T["checks"]["hist_ge_delta_match_" + ch] = H["checks"][ch]["delta_matches_hist_ge"]
    T["checks"]["all_match_1e-9"] = all(abs(c["data_here"] - c["data_fig2"]) < 1e-9 and abs(c["mc_unit_here"] - c["mc_fig2"]) < 1e-9
                                        for k, d in T["checks"].items() if k.startswith("fig2_") for c in d.values())
    T["checks"]["outside_cell_events_odd"] = {v: float(arr(v, "odd", "data", "data")[3].sum()) for v in VETOS}
    T["checks"]["fts_flag_mismatch_vs_nfts28"] = H["fts_flag_mismatch_vs_nfts28"]
    json.dump(T, open(TAB, "w"), indent=1, default=float)

    # ---------------- fig6: fail fraction vs eta_max, ALL cells, one column per veto (N-1) + combined
    x = np.arange(5); off = {"7071": -0.18, "7091": 0.0, "nd4cat": 0.18}
    fig, ax = S.grid(4, 1, w=4.3, h=3.6)
    for j, v in enumerate(VETOS):
        a, b = ax[0][j]; L = T["per_eta"]["|".join((v, "odd", "ALL"))][:5]
        for ch in CHAINS:
            f = np.array([q[ch]["f_full"] for q in L]); e = np.array([q[ch]["err_full"] for q in L])
            fd8 = np.array([q[ch]["f_diff8"] for q in L])
            a.bar(x, 2 * e, bottom=f - e, width=0.8, color=S.C[ch], alpha=0.25, lw=0)
            a.hlines(f, x - 0.4, x + 0.4, color=S.C[ch], lw=1.4, label="DIFF8 + " + ch + " (fitted scales)")
            a.hlines(fd8, x - 0.4, x + 0.4, color=S.C[ch], lw=1.0, ls=":", label="DIFF8 only (" + ch + " fit)")
            dl = np.array([q[ch]["delta_data_minus_full"] for q in L]); de = np.array([q["data"]["err"] for q in L])
            b.bar(x + off[ch], 2 * e, bottom=-e, width=0.16, color=S.C[ch], alpha=0.35, lw=0)
            b.errorbar(x + off[ch], dl, yerr=de, fmt="o", ms=2.5, color=S.C[ch], lw=0.8)
        fd = np.array([q["data"]["f"] for q in L]); ed = np.array([q["data"]["err"] for q in L])
        a.errorbar(x, fd, yerr=ed, xerr=0.4, fmt="o", ms=3, color="black", lw=0.9, label="data", zorder=5)
        pur = [L[i]["7071"]["purity_diff8"] for i in range(5)]
        a.set_ylim(0, max(1.05 * np.nanmax(fd + ed), 0.1) * 1.3); a.set_xlim(-0.5, 4.5); a.yaxis.set_major_locator(MaxNLocator(5, prune="lower"))
        b.axhline(0, color="0.3", lw=0.7); b.set_xticks(x); b.set_xticklabels(BLAB, fontsize=7.5)
        b.set_xlabel("$\\eta_{max}$ (etaMaxClus08) bin"); b.set_ylabel("data $-$ MC"); a.set_ylabel("fail fraction")
        S.note(a, "D0+D1+D2, odd; %s%s\nDIFF8 share (7071 fit): %s" % (VLAB[v], "" if v == "COMB" else " (N$-$1)", " ".join("%.2f" % q for q in pur)), loc="ul")
        if j == 0: S.figlegend(fig, a, ncol=4)
    fig.legends[0].set_bbox_to_anchor((0.5, 0.0)); fig.subplots_adjust(bottom=0.3)
    S.save(fig, OUT / "fig6_veto_eff_vs_etamax.png", "Forward-veto fail fraction vs $\\eta_{max}$: each veto N$-$1 (other two applied) and the combined veto, odd split")

    # ---------------- fig7: deep-gap (eta < 2.5) fail fraction vs D cell and M_X bin, rows = vetoes
    me = H["mx_edges"]; KL = {"cells": list(CELLS), "mx": ["$M_X$<%.1f" % me[0], "%.1f-%.1f" % (me[0], me[1]), "$\\geq$%.1f GeV" % me[1]]}
    fig, ax = S.grid(2, 2, w=4.6, h=3.3)
    for r, v in enumerate(("COMB", "N1_PLUG")):
        K = T["kinematics"][v + "|odd"]
        for j, keys in enumerate((list(CELLS), ["MX1", "MX2", "MX3"])):
            a, b = ax[r][j]; x3 = np.arange(3)
            for ch in CHAINS:
                f = np.array([K[k][ch]["f_full"] for k in keys]); e = np.array([K[k][ch]["err_full"] for k in keys])
                a.bar(x3, 2 * e, bottom=f - e, width=0.8, color=S.C[ch], alpha=0.25, lw=0)
                a.hlines(f, x3 - 0.4, x3 + 0.4, color=S.C[ch], lw=1.4, label="DIFF8 + " + ch + " (fitted scales)")
                a.hlines([K[k][ch]["f_diff8"] for k in keys], x3 - 0.4, x3 + 0.4, color=S.C[ch], lw=1.0, ls=":", label="DIFF8 only (" + ch + " fit)")
                dl = np.array([K[k][ch]["delta_data_minus_full"] for k in keys]); de = np.array([K[k]["data"]["err"] for k in keys])
                b.bar(x3 + off[ch], 2 * e, bottom=-e, width=0.16, color=S.C[ch], alpha=0.35, lw=0)
                b.errorbar(x3 + off[ch], dl, yerr=de, fmt="o", ms=2.5, color=S.C[ch], lw=0.8)
            fd = np.array([K[k]["data"]["f"] for k in keys]); ed = np.array([K[k]["data"]["err"] for k in keys])
            a.errorbar(x3, fd, yerr=ed, xerr=0.4, fmt="o", ms=3, color="black", lw=0.9, label="data", zorder=5)
            a.set_ylim(0, max(np.nanmax(fd + ed), 0.1) * 1.6); a.set_xlim(-0.5, 2.5); a.yaxis.set_major_locator(MaxNLocator(5, prune="lower"))
            b.axhline(0, color="0.3", lw=0.7); b.set_xticks(x3); b.set_xticklabels(KL["cells" if j == 0 else "mx"], fontsize=8)
            b.set_ylabel("data $-$ MC"); a.set_ylabel("fail fraction" + ("" if v == "COMB" else " (N$-$1)"))
            b.set_xlabel("kinematic cell" if j == 0 else "$M_X$ bin (all cells)")
            fl = K["flatness"]
            S.note(a, "%s, $\\eta_{max}$ < 2.5, odd\nflatness of data$-$MC, $\\chi^2$/2 (p):\n%s" % (VLAB[v], "  ".join(
                "%s %.1f (%.2f)" % (ch, fl[ch]["D_cells" if j == 0 else "MX_bins"]["chi2"], fl[ch]["D_cells" if j == 0 else "MX_bins"]["p"]) for ch in CHAINS)), loc="ul")
            if r == 0 and j == 0: S.figlegend(fig, a, ncol=3)
    fig.legends[0].set_bbox_to_anchor((0.5, 0.0)); fig.subplots_adjust(bottom=0.17)
    S.save(fig, OUT / "fig7_plug_eff_kinematics.png", "Deep-gap ($\\eta_{max}$ < 2.5) fail fraction vs kinematics: combined veto and Plug N$-$1, odd split")
    print(json.dumps(T["checks"], default=lambda q: round(float(q), 4))[:1500])


if __name__ == "__main__":
    main()

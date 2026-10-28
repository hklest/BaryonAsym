#!/usr/bin/env python3
"""WP-A5 figures (login node, stored JSON only): fig1 q per (M_X bin, D cell), even (frozen, sigma_q band) and odd, per
detector; fig2 eta-shape delta_v per eta08 bin, even and odd; fig3 syst_veto_rel per measurement bin, frames A and B,
with the 0.05 line (open markers: D7-masked bins)."""
import json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent; sys.path.insert(0, str(HERE.parent / "gate_evidence"))
import ge_style as S
import matplotlib.pyplot as plt

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
OD = D / "data/restart_2026-10-27/wp-a5"; PD = D / "plots/restart_2026-10-27/wp-a5"
C = json.load(open(OD / "veto_correction.json")); T = json.load(open(OD / "g1pp_a_table.json"))["rows"]
VIEWS = ("N1_PLUG", "N1_FTS", "N1_FMD"); VL = {"N1_PLUG": "Plug", "N1_FTS": "FTS", "N1_FMD": "FMD"}
MX = ("4-10", "10-20", "20-40"); CE = ("D0", "D1", "D2")


def fig1():
    fig, ax = plt.subplots(1, 3, figsize=(13, 3.8), sharey=False)
    keys = ["%s|%s" % (m, c) for m in MX for c in CE]; x = np.arange(len(keys))
    for a, v in zip(ax, VIEWS):
        cv = C["cells"][v]; q = np.array([cv[k]["q"] for k in keys]); s = np.array([cv[k]["sigma_q"] for k in keys])
        a.bar(x, 2 * s, bottom=q - s, width=0.7, color=S.C["signal"], alpha=0.25, lw=0, label=r"even $q\pm\sigma_q$ (frozen)")
        a.errorbar(x - 0.12, q, yerr=[cv[k]["sigma_stat_even"] for k in keys], fmt="o", ms=3.5, color=S.C["signal"], lw=0.9, label="even (stat)")
        a.errorbar(x + 0.12, [cv[k]["q_odd"] for k in keys], yerr=[cv[k]["sigma_stat_odd"] for k in keys], fmt="s", ms=3, color=S.C["7071"], lw=0.9, label="odd (stat)")
        a.axhline(0, color="0.4", lw=0.7)
        for j in (2.5, 5.5): a.axvline(j, color="0.7", lw=0.6, ls=":")
        a.set_xticks(x); a.set_xticklabels([k.replace("|", "\n") for k in keys], fontsize=7)
        a.set_title(VL[v] + " N-1, deep gap " + r"$\eta_{max}(0.8)\in[-1,2.5)$", fontsize=9); a.set_ylabel(r"$q=\Delta f/(1-f_{MC})$")
    ax[0].legend(fontsize=7, loc="upper left"); fig.tight_layout(); fig.savefig(PD / "fig1_q_per_cell.pdf"); fig.savefig(PD / "fig1_q_per_cell.png", dpi=130); plt.close(fig)


def fig2():
    fig, ax = plt.subplots(1, 3, figsize=(13, 3.6))
    ks = list(C["eta_shape"]["N1_PLUG"]["bins"].keys()); x = np.arange(len(ks))
    for a, v in zip(ax, VIEWS):
        for blk, off, col, mk, lab in (("eta_shape", -0.1, S.C["signal"], "o", "even (used)"), ("eta_shape_odd", 0.1, S.C["7071"], "s", "odd")):
            b = C[blk][v]["bins"]
            a.errorbar(x + off, [b[k]["delta"] for k in ks], yerr=[b[k]["sigma_stat"] for k in ks], fmt=mk, ms=3.5, color=col, lw=0.9, label=lab)
        a.axhline(0, color="0.4", lw=0.7); a.axvspan(-0.5, 2.5, color="0.9", zorder=0)
        a.set_xticks(x); a.set_xticklabels(ks, fontsize=7.5, rotation=30); a.set_title(VL[v] + r": $\delta_v=q_{ALL}(\eta\ \mathrm{bin})-q_{ALL}$(deep gap)", fontsize=9)
        a.set_xlabel(r"$\eta_{max}(0.8)$ bin (shaded: deep gap)")
    ax[0].legend(fontsize=7); ax[0].set_ylabel(r"$\delta_v$"); fig.tight_layout()
    fig.savefig(PD / "fig2_eta_shape.pdf"); fig.savefig(PD / "fig2_eta_shape.png", dpi=130); plt.close(fig)


def fig3():
    fig, ax = plt.subplots(2, 2, figsize=(13, 6.5), sharey=True)
    for i, fr in enumerate("AB"):
        for j, axn in enumerate(("xF", "pt2")):
            a = ax[i][j]; xt = []; xl = []; pos = 0
            for c, col in (("h+", S.C["signal"]), ("h-", S.C["7071"])):
                for m in MX:
                    rr = [r for r in T if r["frame"] == fr and r["axis"] == axn and r["charge"] == c and r["mx_bin"] == m]
                    xs = pos + np.arange(len(rr)); y = np.array([r["syst_veto_rel"] for r in rr]); mk = np.array([r["masked_D7"] for r in rr])
                    a.plot(xs[~mk], y[~mk], "o", ms=4, color=col); a.plot(xs[mk], y[mk], "o", ms=4, mfc="none", color=col)
                    a.plot(xs, np.abs([r["R_eta"] for r in rr]), "_", ms=7, color="0.3")
                    xt.append(pos + (len(rr) - 1) / 2); xl.append("%s\n%s" % (c, m)); pos += len(rr) + 1
            a.axhline(0.05, color="black", ls="--", lw=0.9); a.set_xticks(xt); a.set_xticklabels(xl, fontsize=7.5)
            a.set_title("frame %s, %s bins (filled: not D7-masked; bar: |R_eta|)" % (fr, "x_F" if axn == "xF" else "p_T*^2"), fontsize=9)
            a.set_ylabel(r"$\sigma_{rel}^{veto}$")
    fig.tight_layout(); fig.savefig(PD / "fig3_syst_per_bin.pdf"); fig.savefig(PD / "fig3_syst_per_bin.png", dpi=130); plt.close(fig)


if __name__ == "__main__":
    PD.mkdir(parents=True, exist_ok=True); fig1(); fig2(); fig3(); print("written", sorted(p.name for p in PD.iterdir()))

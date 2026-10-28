#!/usr/bin/env python3
"""WP-C C1 slice 3: <= 6 figures under D/plots/restart_2026-10-27/wp-c/c1/ with captions.md.  Reads the C1 tables only."""
import csv, json, sys
from pathlib import Path
import numpy as np
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
sys.path.insert(0, str(Path(__file__).resolve().parent))
import c1_lib as C

P = C.D / "plots/restart_2026-10-27/wp-c/c1"
COL = {"h+": "#1f77b4", "h-": "#d95f02"}


def rd(name): return list(csv.DictReader(open(C.W / name)))


def main():
    P.mkdir(parents=True, exist_ok=True)
    g = json.load(open(C.W / "grid_c1.json"))["cells"]; cj = json.load(open(C.W / "cells_c1.json"))
    A = rd("binstats_c1_A.csv"); B = rd("binstats_c1_B.csv"); V = rd("veto_syst_c1.csv")
    sel = lambda r: r["axis"] == "z" or r["grid"] == g[r["cell_name"]]["pt2_grid"]
    ch = [r for r in A if sel(r)]
    cap = []
    # 1 purity / stability on the frozen grid
    fig, ax = plt.subplots(2, 4, figsize=(14, 6), sharey=True)
    for i, cn in enumerate(C.CELLS):
        for j, key in enumerate(("purity", "stability")):
            a = ax[j, i]
            for c in ("h+", "h-"):
                for axn, mk in (("z", "s"), ("pt2", "o")):
                    rr = [r for r in ch if r["cell_name"] == cn and r["charge"] == c and r["axis"] == axn]
                    a.plot([int(r["bin"]) + (0 if axn == "pt2" else 0) for r in rr], [float(r[key]) for r in rr], mk, color=COL[c], mfc="none" if axn == "z" else COL[c], label="%s %s" % (c, axn))
            a.axhline(0.35, color="k", ls="--", lw=0.8); a.set_title("%s (%s)" % (cn, g[cn]["pt2_grid"]) if j == 0 else ""); a.set_xlabel("bin"); a.set_ylabel(key if i == 0 else "")
    ax[0, 0].legend(fontsize=7); fig.tight_layout(); fig.savefig(P / "fig1_purity_stability.png", dpi=110); plt.close(fig)
    cap.append("fig1_purity_stability.png: purity (top) and stability (bottom) per gen bin on the frozen grid, per cell, h+ and h-, z* (open squares) and p_T*^2 (dots) axes, stand-in f_ND; dashed = 0.35 floor.")
    # 2 flip fractions
    fig, ax = plt.subplots(1, 4, figsize=(14, 3.6), sharey=True)
    for i, cn in enumerate(C.CELLS):
        rr = [r for r in ch if r["cell_name"] == cn and r["charge"] == "h+"]; x = np.arange(len(rr))
        for k, (key, lab) in enumerate((("flip_hemi", "hemisphere"), ("flip_beta", "beta"), ("intercell_mig", "either"), ("xf_small_share", "gen |xF|<0.02"))):
            ax[i].bar(x + 0.2 * k - 0.3, [float(r[key]) for r in rr], 0.2, label=lab)
        ax[i].set_xticks(x); ax[i].set_xticklabels(["%s%d" % (r["axis"][0], int(r["bin"])) for r in rr], fontsize=7); ax[i].set_title(cn); ax[i].axhline(0.30, color="k", ls=":"); ax[i].axhline(0.5, color="r", ls="--", lw=0.8)
    ax[0].legend(fontsize=7); ax[0].set_ylabel("fraction of gen-bin tracks"); fig.tight_layout(); fig.savefig(P / "fig2_flip_fractions.png", dpi=110); plt.close(fig)
    cap.append("fig2_flip_fractions.png: per gen bin (h+), fraction of matched tracks whose reco hemisphere / beta bin / either differs, and the share with gen |x_F| < 0.02; dotted 0.30 (WP-H flag), dashed 0.5 (C1-7 gate).")
    # 3 M_X projection
    M = rd("mx_projection_c1.csv"); fig, ax = plt.subplots(1, 2, figsize=(11, 3.8), sharey=True)
    for k, gen in enumerate(("G2", "G1")):
        for c, m in enumerate(("4-10", "10-20", "20-40")):
            rr = [r for r in M if r["gen"] == gen and r["mx_bin"] == m]
            ax[k].plot(range(len(rr)), [float(r["purity"]) for r in rr], "o-", ms=3, label="M_X %s" % m)
        ax[k].axhline(0.35, color="k", ls="--", lw=0.8); ax[k].set_title("gen frame %s" % gen); ax[k].set_xlabel("frozen-grid bin (cell, axis, bin order)")
    ax[0].set_ylabel("purity"); ax[0].legend(fontsize=7); fig.tight_layout(); fig.savefig(P / "fig3_mx_projection.png", dpi=110); plt.close(fig)
    cap.append("fig3_mx_projection.png: purity of each frozen-grid bin restricted to reco M_X classes, with the G2 and G1 proxy gen frames (D20 continuity); denominators include the stand-in ND and Reggeon.")
    # 4 failures A vs B
    fig, ax = plt.subplots(figsize=(7, 3.8)); w = 0.38
    labs = ["%s|%s" % (cn, gn) for cn in C.CELLS for gn in C.GRIDS]
    for k, (fr, T) in enumerate((("A", A), ("B", B))):
        n = [sum(1 for r in T if r["axis"] == "pt2" and r["cell_name"] == l.split("|")[0] and r["grid"] == l.split("|")[1] and r["pass_D7plus"] != "True") for l in labs]
        ax.bar(np.arange(len(labs)) + (k - 0.5) * w, n, w, label="frame " + fr)
    ax.set_xticks(range(len(labs))); ax.set_xticklabels(labs, rotation=70, fontsize=6); ax.set_ylabel("failing bins (h+ and h-)"); ax.legend(); fig.tight_layout(); fig.savefig(P / "fig4_frameAB_failures.png", dpi=110); plt.close(fig)
    cap.append("fig4_frameAB_failures.png: number of p_T*^2 bins failing D7+ (both charges) per cell and candidate grid, frame A (nominal) and frame B on the frame-A edges (cross-check, D21).")
    # 5 precision and veto
    fig, ax = plt.subplots(1, 2, figsize=(11, 3.8)); Pz = rd("precision_c1.csv")
    ax[0].plot([float(r["delta_rel"]) for r in Pz if r["delta_rel"] not in ("nan", "")], "o", ms=3); ax[0].set_ylabel("1/sqrt(n_data) (pre-unfolding)"); ax[0].set_xlabel("frozen-grid bin")
    ax[1].plot([float(r["sigma_rel"]) for r in V], "o", ms=3); ax[1].axhline(0.05, color="r", ls="--"); ax[1].set_ylabel("WP-A5 veto sigma_rel"); ax[1].set_xlabel("frozen-grid bin")
    fig.tight_layout(); fig.savefig(P / "fig5_precision_veto.png", dpi=110); plt.close(fig)
    cap.append("fig5_precision_veto.png: expected relative data precision per bin (UG frame-A counts) and the WP-A5 veto systematic per bin; red dashed = 0.05 acceptance.")
    # 6 beta bin
    fig, ax = plt.subplots(figsize=(4.5, 3.6)); bp = cj["beta_bin_event"]
    for k, key in enumerate(("purity", "stability")): ax.bar(np.arange(2) + 0.2 * (2 * k - 1) / 1, [bp[b][key] for b in ("0", "1")], 0.2, label=key)
    ax.set_xticks([0, 1]); ax.set_xticklabels(["beta < 0.523", "beta >= 0.523"]); ax.axhline(0.8, color="k", ls=":"); ax.legend(); ax.set_ylabel("event level"); fig.tight_layout(); fig.savefig(P / "fig6_beta_bins.png", dpi=110); plt.close(fig)
    cap.append("fig6_beta_bins.png: event-level purity and stability of the two beta bins (cocktail signal events, informational); dotted 0.8 = the plan's expectation.")
    (P / "captions.md").write_text("\n".join("- " + c for c in cap) + "\n")
    print("figures", len(cap))


if __name__ == "__main__":
    main()

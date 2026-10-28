#!/usr/bin/env python3
"""Figure: 9012 raw signal tracks (reco, gen-valid) per user-grid cell, frame B, both charges, D7 count threshold 50."""
import csv
import numpy as np
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/"
R = list(csv.DictReader(open(D + "data/restart_2026-10-27/wp-c/user_grid_2026-10-28/user_grid_tables.csv")))
fig, ax = plt.subplots(1, 2, figsize=(13, 4.6), sharey=True)
col = {"h+": "#0072B2", "h-": "#D55E00"}
panels = [("z", ("zstar_med", "zstar_0.2"), 2, "z* = 2E*/M_X  (2 bins per hemisphere x beta bin)", ("split at data median", "split at 0.2")),
          ("pt", ("pt2s_q5", "pt2s_f5"), 5, "p_T*^2  (5 bins per hemisphere x beta bin)", ("data-quantile edges", "fixed 0-.25-.5-1-2-5 GeV^2"))]
for a, (g, vars_, nb, title, labs) in zip(ax, panels):
    for vi, (v, mk, lab) in enumerate(zip(vars_, ("o", "s"), labs)):
        for ch in ("h+", "h-"):
            xs, ys = [], []
            for r in R:
                if r["frame"] == "B" and r["variant"] == v and r["charge"] == ch:
                    h = 0 if r["hemi"].startswith("G") else 1
                    x = (h * 2 + int(r["beta_bin"])) * (nb + 1) + int(r["bin"]) + 0.12 * (vi - .5) + (0.06 if ch == "h-" else -0.06)
                    xs.append(x); ys.append(max(int(r["raw_9012_sig"]), 0.5))
            a.scatter(xs, ys, marker=mk, s=26, color=col[ch], facecolors=col[ch] if vi == 0 else "none", label="%s, %s" % (ch, lab))
    a.axhline(50, color="k", ls="--", lw=1); a.set_yscale("log"); a.set_title(title, fontsize=10)
    for k in range(1, 4): a.axvline(k * (nb + 1) - 1, color="0.8", lw=.8)
    a.set_xticks([(k * (nb + 1)) + (nb - 1) / 2 for k in range(4)])
    a.set_xticklabels(["photon side\n(xF<0), low beta", "photon side\n(xF<0), high beta", "Pomeron side\n(xF>0), low beta", "Pomeron side\n(xF>0), high beta"], fontsize=8)
    a.grid(axis="y", alpha=.3)
ax[0].set_ylabel("9012 raw reco tracks (gen-valid) per bin"); ax[1].legend(fontsize=7, loc="upper right", ncol=1)
ax[0].text(0.01, 0.04, "dashed: D7 threshold, 50", transform=ax[0].transAxes, fontsize=8)
fig.suptitle("User grid, frame B, eta_max(0.8)<3.0, x_IP<0.03, 4<=M_X<=40, Q^2>150 integrated", fontsize=10)
fig.tight_layout(); fig.savefig(D + "plots/restart_2026-10-27/wp-c/user_grid_2026-10-28/fig_user_grid_counts.png", dpi=130)

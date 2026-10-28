#!/usr/bin/env python3
"""WP-A5 edge-3.0 fig3 equivalent (login node, single CSV input): syst_veto_rel per measurement bin, frames A and B,
D7-retained (filled) vs D7-masked (open), 0.05 line; grey crosses overlay the frozen edge-3.2 values."""
import csv, sys
from pathlib import Path
import numpy as np
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
sys.path.insert(0, str(D / "scripts/restart_2026-10-27/gate_evidence"))
import ge_style as S
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
A5 = D / "data/restart_2026-10-27/wp-a5"; PD = D / "plots/restart_2026-10-27/wp-a5/edge30"
rd = lambda p: list(csv.DictReader(open(p)))
T30 = rd(A5 / "edge30/g1pp_a_table_edge30.csv"); T32 = rd(A5 / "g1pp_a_table.csv")
MX = ("4-10", "10-20", "20-40")
f = lambda r, k: float(r[k]); b = lambda r, k: r[k] == "True"
sel = lambda T, fr, axn, c, m: sorted([r for r in T if r["frame"] == fr and r["axis"] == axn and r["charge"] == c and r["mx_bin"] == m], key=lambda r: int(r["bin"]))
PD.mkdir(parents=True, exist_ok=True)
fig, ax = plt.subplots(2, 2, figsize=(13, 6.5), sharey=True)
for i, fr in enumerate("AB"):
    for j, axn in enumerate(("xF", "pt2")):
        a = ax[i][j]; xt = []; xl = []; pos = 0
        for c, col in (("h+", S.C["signal"]), ("h-", S.C["7071"])):
            for m in MX:
                r30 = sel(T30, fr, axn, c, m); r32 = sel(T32, fr, axn, c, m); xs = pos + np.arange(len(r30))
                y = np.array([f(r, "syst_veto_rel") for r in r30]); mk = np.array([b(r, "masked_D7") for r in r30])
                a.plot(xs, [f(r, "syst_veto_rel") for r in r32], "x", ms=5, color="0.55", label="edge 3.2 (frozen)" if pos == 0 else None)
                a.plot(xs[~mk], y[~mk], "o", ms=4, color=col); a.plot(xs[mk], y[mk], "o", ms=4, mfc="none", color=col)
                a.plot(xs, np.abs([f(r, "R_eta") for r in r30]), "_", ms=7, color="0.3")
                xt.append(pos + (len(r30) - 1) / 2); xl.append("%s\n%s" % (c, m)); pos += len(r30) + 1
        a.axhline(0.05, color="black", ls="--", lw=0.9); a.set_xticks(xt); a.set_xticklabels(xl, fontsize=7.5)
        a.set_title("frame %s, %s bins, edge 3.0 (filled: D7-retained; open: D7-masked; bar: |R_eta|; x: edge 3.2)" % (fr, "x_F" if axn == "xF" else "p_T*^2"), fontsize=8)
        a.set_ylabel(r"$\sigma_{rel}^{veto}$")
ax[0][0].legend(fontsize=7, loc="upper left")
fig.tight_layout(); fig.savefig(PD / "fig3_syst_per_bin_edge30.pdf"); fig.savefig(PD / "fig3_syst_per_bin_edge30.png", dpi=130)
open(PD / "captions.md", "w").write("""# WP-A5 edge-3.0 figure captions

**fig3_syst_per_bin_edge30** (`D/plots/restart_2026-10-27/wp-a5/edge30/fig3_syst_per_bin_edge30.{pdf,png}`). Relative veto
systematic sigma_rel^veto = sqrt(max(|R_up|,|R_down|)^2 + R_eta^2) per measurement bin of (1/N_D) dn/dx_F (top row) and
(1/N_D) dn/dp_T*^2 (bottom row), frames A (upper) and B (lower), grouped by charge and M_X bin, for the event selection
etaMaxClus08 < 3.0 (D15). Filled circles: bins kept by the frozen D7 mask; open circles: D7-masked bins; short bars:
|R_eta|; grey crosses: the frozen edge-3.2 values. Dashed line: the 0.05 acceptance of G1''(a). The q, sigma_q and
eta-shape inputs are the frozen veto_correction.json; the eta-shape bins [3.0,3.2) and [3.2,3.3) are unpopulated at this edge.
""")
print("ok")

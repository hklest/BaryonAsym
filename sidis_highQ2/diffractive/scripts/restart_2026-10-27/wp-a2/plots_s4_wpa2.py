#!/usr/bin/env python3
"""WP-A2 S4 plots (login node): odd-split frozen-definition §7.1 fit, fine ALL, DATA vs
prediction and per-bin deviance, per ND alternative.  Output D/plots/restart_2026-10-27/wp-a2/s4/."""
import json
from pathlib import Path
import numpy as np
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
F = D / "data/restart_2026-10-27/wp-a2/s4_g1p/fits"; O = D / "plots/restart_2026-10-27/wp-a2/s4"; O.mkdir(parents=True, exist_ok=True)
fig, ax = plt.subplots(2, 3, figsize=(14, 6), sharex=True, gridspec_kw={"height_ratios": [3, 1]})
for j, ch in enumerate(("7071", "7091", "nd4cat")):
    r = json.load(open(F / ("section_7_1_twofree_fit_results_%s_fine_ALL.json" % ch)))
    rec = r["fine"][ch]["ALL"]; E = np.asarray(r["edges"]); nb = len(E) - 1
    y = np.asarray(rec["data"]); m = np.asarray(rec["prediction"]); dc = np.asarray(rec["deviance_contribution"])
    x = np.arange(len(y))
    ax[0, j].step(x, y, where="mid", color="k", label="DATA (odd)"); ax[0, j].step(x, m, where="mid", color="C1", label="fit")
    ax[0, j].set_yscale("log"); ax[0, j].set_title("%s: D/ndf %.0f/%d, toy p %.4f (1000 toys)" % (ch, rec["deviance"], rec["ndf"], rec["template_statistics_aware_gof"]["toy_p_ge_observed"]), fontsize=9)
    for k in (1, 2):
        ax[0, j].axvline(k * nb - 0.5, color="grey", lw=0.5); ax[1, j].axvline(k * nb - 0.5, color="grey", lw=0.5)
    ax[1, j].step(x, dc, where="mid", color="C3"); ax[1, j].set_xlabel("bin (D0 | D1 | D2; η_max(0.4 GeV) < 3.4 + [3.4,5))")
    ax[0, j].legend(fontsize=8)
ax[1, 0].set_ylabel("deviance")
fig.tight_layout(); fig.savefig(O / "g1prime_c_fit_fine_ALL.png", dpi=110); print(O / "g1prime_c_fit_fine_ALL.png")

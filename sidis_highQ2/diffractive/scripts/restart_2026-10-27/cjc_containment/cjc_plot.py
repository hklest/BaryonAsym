#!/usr/bin/env python3
"""CJC-containment figures from cjc_containment.json (one file; login node).  Nominal definition TRK, both parities."""
import json, sys
from pathlib import Path
import numpy as np

H = Path(__file__).resolve().parent
sys.path.insert(0, str(H.parent / "gate_evidence"))
import ge_style as S
plt = S.plt
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
J = json.load(open(D / "data/restart_2026-10-27/cjc_containment/cjc_containment.json"))
OUT = D / "plots/restart_2026-10-27/cjc_containment"; OUT.mkdir(parents=True, exist_ok=True)
HS = J["hists"]; ED = J["hist_edges"]; DN = sys.argv[1] if len(sys.argv) > 1 else "TRK"


def mc(c, g, v, ch, p="both"):
    parts = [HS["|".join((DN, c, p, g, v, ch, n))] for n in ("signal", "reggeon", "nd")]
    return [np.asarray(x["sumw"]) for x in parts], np.sum([np.asarray(x["sumw2"]) for x in parts], 0)


# fig_c1: eta_max for contained / not contained / all, data vs MC (7071 stack, 7091 total line)
fig, ax = S.grid(3)
ed = np.asarray(ED["ec08"])
for i, (g, t) in enumerate((("C", "current hemisphere CJC-contained"), ("N", "not contained"), ("ALL", "full LRG sample"))):
    a, r = ax[0][i]; d = np.asarray(HS["|".join((DN, "ALL", "both", g, "ec08", "data"))])
    p71, v71 = mc("ALL", g, "ec08", "7071"); p91, v91 = mc("ALL", g, "ec08", "7091")
    S.panel(a, r, ed, d, [("signal (DIFF8 RAPGAP+ODS)", p71[0], S.C["signal"]), ("Reggeon", p71[1], S.C["reggeon"]), ("ND 7071", p71[2], S.C["nd"])],
            v71, density=True, ylabel="events / unit $\\eta_{max}$", xlabel="$\\eta_{max}$ (etaMaxClus08)", rlim=(0.4, 1.8),
            mc_lines=[("DIFF8 + 7091 total", np.sum(p91, 0), v91, S.C["7091"], "--")], qual=d >= 10)
    S.vline(a, r, 3.0, "LAr edge 3.0"); S.note(a, "%s\ndata %d events" % (t, int(d.sum())))
S.figlegend(fig, ax[0][0][0], ncol=6)
S.save(fig, OUT / "fig_c1_etamax_contained.png", "LRG sample (fitted G1'(c) scales), Q$^2$>150, x$_{IP}$<0.03, 4<M$_X$<40 GeV; definition %s" % DN)

# fig_c2: occupancy of contained data events
fig, axs = plt.subplots(1, 2, figsize=(11, 4)); fig.subplots_adjust(wspace=0.45)
ml = np.asarray(ED["mx_log"]); xl = np.asarray(ED["xip_log"])
for a, k, ye, yl in ((axs[0], "mx_xip", xl, "$x_{IP}$ (reco)"), (axs[1], "mx_ec08", ed, "$\\eta_{max}$")):
    h = np.asarray(HS["2d|%s|C|%s" % (DN, k)]).T; hm = np.ma.masked_where(h <= 0, h)
    m = a.pcolormesh(ml, ye, hm, cmap="viridis", norm=plt.matplotlib.colors.LogNorm(vmin=1, vmax=max(h.max(), 2)))
    a.set_xscale("log"); a.set_xlabel("$M_X$ (reco, GeV)"); a.set_ylabel(yl); fig.colorbar(m, ax=a, label="data events")
    if k == "mx_xip": a.set_yscale("log"); a.set_ylim(1.5e-3, 0.03)
    for x in (10, 20): a.axvline(x, color="w", lw=0.8, ls=":")
S.save(fig, OUT / "fig_c2_occupancy_contained.png", "CJC-contained LRG data events (both parities, definition %s)" % DN)

# fig_c3: veto N-1 fail fractions and purity, contained subset vs full sample
V = J["veto"]; vs = ("PLUG", "FTS", "FMD", "COMB")
fig, axs = plt.subplots(1, 2, figsize=(11, 4))
for j, (sub, mk, off) in enumerate((("ALL", "s", -0.15), ("C", "o", 0.15))):
    x = np.arange(4) + off; o = [V["|".join((DN, sub, v, "both", "all"))] for v in vs]
    lab = "full sample" if sub == "ALL" else "CJC-contained"
    axs[0].errorbar(x - 0.05, [q["f_data"] for q in o], yerr=[q["err_data"] for q in o], fmt=mk, color="k", mfc="k" if sub == "C" else "w", label="data, " + lab)
    axs[0].errorbar(x + 0.05, [q["f_full_7071"] for q in o], yerr=[q["err_full_7071"] for q in o], fmt=mk, color=S.C["7071"], mfc=S.C["7071"] if sub == "C" else "w", label="MC DIFF8+7071, " + lab)
    axs[0].plot(x + 0.05, [q["f_full_7091"] for q in o], "_", ms=12, color=S.C["7091"], label="MC DIFF8+7091, " + lab if j else None)
    axs[1].plot(x, [q["purity_7071"] for q in o], mk, color=S.C["7071"], mfc=S.C["7071"] if sub == "C" else "w", label="7071, " + lab)
    axs[1].plot(x, [q["purity_7091"] for q in o], mk, color=S.C["7091"], mfc=S.C["7091"] if sub == "C" else "w", label="7091, " + lab)
for a, yl in ((axs[0], "N$-$1 fail fraction"), (axs[1], "DIFF8 purity of the N$-$1 sample")):
    a.set_xticks(range(4)); a.set_xticklabels(["Plug", "FTS", "FMD", "any (COMB)"]); a.set_ylabel(yl); a.legend(fontsize=7)
axs[1].set_ylim(0, 1.05)
S.save(fig, OUT / "fig_c3_veto_purity_contained.png", "Forward vetoes N$-$1 (x$_{IP}$<0.03, 4<M$_X$<40, $\\eta_{max}$<3.3), both parities, definition %s" % DN)
print("wrote", sorted(p.name for p in OUT.glob("fig_c*.png")))

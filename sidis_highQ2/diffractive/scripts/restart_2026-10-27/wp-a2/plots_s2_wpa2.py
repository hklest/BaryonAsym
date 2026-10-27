#!/usr/bin/env python3
"""WP-A2 S2 plot set (login node; even-split partials and s2_analysis_even.json):
eta_max edge overlays per threshold (H1), delta-eta (H1/H6), fine ePlug for
eta_max > 3.5 (H5), Plug-tagged edge sample (H4), FMD/FTS per-layer control hit
fractions (H2).  Output D/plots/restart_2026-10-27/wp-a2/s2/*.png."""
import json, sys
from pathlib import Path
import numpy as np
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "wp-a"))
import wpa_common as W

R = W.D / "data/restart_2026-10-27/wp-a2/s2_diag"; O = W.D / "plots/restart_2026-10-27/wp-a2/s2"; O.mkdir(parents=True, exist_ok=True)
P = {k: json.load(open(R / "partials" / k / "even" / "partial.json")) for k in W.DATA_LABELS + sorted(set(W.DIFF8 + ["7071", "7091"]))}
A = json.load(open(R / "analysis" / "s2_analysis_even.json"))
E = np.linspace(-1, 5, 121); LO = E[:-1]; C = 0.5 * (E[1:] + E[:-1]); CELLS = ("D0", "D1", "D2")
rs = lambda l, ch: W.fac_used(l, ch) / W.table_fac(l)
cock = lambda ch: W.DIFF8 + W.CHAINS[ch]["nd"]


def proj(l, ch, g):  # per-period projection by represented-period luminosity share (as s2_analyze_wpa2.proj_share)
    rp = W.represented(l, ch); L = W.period_lumi()
    return L[g] / sum(L[k] for k in rp) if g in rp else 0.0
COL = {"7071": "#1f77b4", "7091": "#d62728"}


def get(block_fn, labels, key, ch=None):
    return sum((rs(l, ch) if ch else 1.0) * np.asarray(block_fn(P[l]), float) for l in labels)


# 1. eta_max edge, ALL cells, per threshold
fig, ax = plt.subplots(1, 2, figsize=(11, 4), sharey=True)
for a_, br in zip(ax, ("c08", "c04")):
    nm = "cutremoved_" + br
    d = sum(get(lambda p, z=z: p["wpa2"]["dcell_eta_variants"][nm][z]["raw"], W.DATA_LABELS, "raw") for z in CELLS)
    nrm = (LO >= 3.5 - 1e-9) & (LO < 5 - 1e-9); sel = (LO >= 2.5 - 1e-9) & (LO < 4.0 - 1e-9)
    for ch in ("7071", "7091"):
        m = sum(get(lambda p, z=z: p["wpa2"]["dcell_eta_variants"][nm][z]["signed"], cock(ch), "signed", ch) for z in CELLS)
        m = m * d[nrm].sum() / m[nrm].sum()
        a_.step(C[sel], (d / m)[sel], where="mid", color=COL[ch], label="DATA / %s cocktail" % ch)
    a_.axhline(1, color="k", lw=0.5); a_.axvspan(3.15, 3.35, color="g", alpha=0.1); a_.axvspan(3.40, 3.55, color="r", alpha=0.1)
    a_.set_title("η_max %s GeV, cut-removed, even split, MC norm. 3.5≤η<5" % ("0.8" if br == "c08" else "0.4"), fontsize=9)
    a_.set_xlabel("η_max (cluster)"); a_.set_ylim(0, 3); a_.legend(fontsize=8)
ax[0].set_ylabel("DATA / MC")
fig.tight_layout(); fig.savefig(O / "h1_etamax_edge_threshold.png", dpi=110); plt.close(fig)
# 2. delta-eta
de = np.asarray(P["ep0304"]["wpa2"]["deta_vtx"]["edges"]); dc = 0.5 * (de[1:] + de[:-1])
fig, a_ = plt.subplots(figsize=(6, 4))
h = np.asarray(P["ep0607_true"]["wpa2"]["deta_vtx"]["all"]["raw"], float); a_.step(dc, h / h.sum(), where="mid", label="DATA ep0607")
h = np.asarray(P["9012"]["wpa2"]["deta_vtx"]["all"]["signed"], float); a_.step(dc, h / h.sum(), where="mid", label="9012")
a_.set_xlim(-0.1, 0.1); a_.set_xlabel("η_max(vtx) − η_max(clus), 3.0 ≤ η_clus < 3.6"); a_.legend(); a_.set_title("H1/H6 Δη, even split", fontsize=9)
fig.tight_layout(); fig.savefig(O / "h1_h6_delta_eta.png", dpi=110); plt.close(fig)
# 3. fine ePlug, eta_max > 3.5
ef = np.asarray(P["ep0304"]["wpa2"]["eplug_fine"]["edges"]); fc = 0.5 * (ef[1:] + ef[:-1])
fig, ax = plt.subplots(1, 3, figsize=(13, 4))
for a_, g in zip(ax, ("em0405", "em06_true", "ep0607_true")):
    d = sum(np.asarray(P[k]["wpa2"]["eplug_fine"]["eta35"].get(g, {"raw": np.zeros(100)})["raw"], float) for k in W.DATA_LABELS)
    a_.step(fc, d / d.sum(), where="mid", color="k", label="DATA")
    for ch in ("7071", "7091"):
        m = sum(rs(l, ch) * proj(l, ch, g) * np.asarray(P[l]["wpa2"]["eplug_fine"]["eta35"]["ALL"]["signed"], float) for l in cock(ch))
        if m.sum() > 0:
            a_.step(fc, m / m.sum(), where="mid", color=COL[ch], label=ch)
    a_.set_yscale("log"); a_.set_xlabel("ePlug [GeV]"); a_.set_title("H5: A_pre ∧ η_max>3.5, %s" % g, fontsize=9); a_.legend(fontsize=8)
fig.tight_layout(); fig.savefig(O / "h5_eplug_fine_eta35.png", dpi=110); plt.close(fig)
# 4. H4 edge sample
fig, ax = plt.subplots(1, 3, figsize=(13, 4), sharey=True)
for a_, z in zip(ax, CELLS):
    for ch in ("7071", "7091"):
        r = A["H4"][ch][z]["edge_data_over_mc"]
        a_.step(np.arange(3.025, 3.6, 0.05)[:len(r)], [np.nan if v is None else v for v in r], where="mid", color=COL[ch], label=ch)
    a_.axhline(1, color="k", lw=0.5); a_.set_title("H4 edge: ePlug>20 GeV, %s (MC norm. per cell)" % z, fontsize=9); a_.set_xlabel("η_max (cluster)")
    a_.set_ylim(0, 4); a_.legend(fontsize=8)
ax[0].set_ylabel("DATA / MC")
fig.tight_layout(); fig.savefig(O / "h4_edge_plugtag.png", dpi=110); plt.close(fig)
# 5. per-layer control hit fractions
fig, ax = plt.subplots(2, 3, figsize=(13, 6))
for i, det in enumerate(("fmd", "fts")):
    for a_, g in zip(ax[i], ("em0405", "em06_true", "ep0607_true")):
        for ch in ("7071", "7091"):
            r = A["H2"]["layers"].get("%s|%s|%s" % (ch, det, g))
            if not r:
                continue
            x = np.arange(len(r["data_frac"]))
            if ch == "7071":
                a_.plot(x, r["data_frac"], "ko", label="DATA")
            a_.plot(x, r["mc_frac"], "s", color=COL[ch], mfc="none", label=ch)
        a_.set_title("H2 %s control (deep gap), %s" % (det.upper(), g), fontsize=9); a_.set_xlabel("layer / station index"); a_.legend(fontsize=7)
fig.tight_layout(); fig.savefig(O / "h2_layer_station_rates.png", dpi=110); plt.close(fig)
print(sorted(str(p) for p in O.glob("*.png")))

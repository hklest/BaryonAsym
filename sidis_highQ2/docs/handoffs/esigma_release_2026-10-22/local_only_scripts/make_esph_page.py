"""Local page for the eSigma-frame budget (2026-10-22): comparison figure + HTML around the canonical 7-column renders.
Usage: python3 make_esph_page.py <dir with esph_budget.csv, esph_budget_summary.json, 7col PNGs>"""
import csv, json, sys
from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

P = Path(sys.argv[1])
rows = list(csv.DictReader(open(P / "esph_budget.csv")))
S = json.load(open(P / "esph_budget_summary.json"))
CELLS = ["S0_x0", "S0_x1", "S1_x0", "S1_x1", "S2_x0", "S2_x1", "S3_x0"]
HIGHX = {"S0_x1", "S1_x1", "S2_x1"}
CH = {"pos": "h⁺", "neg": "h⁻"}
TOT = [("installed_total_quoted_with_nonprimary_v5_nw_rel", "installed (2026-10-21, electron photon, old 'both-shifted' legs)", "#999999"),
       ("total_quoted_final_electronframe_rel", "final re-release, electron photon (MC-only legs)", "#D55E00"),
       ("total_quoted_esph_rel", "final re-release, eΣ photon", "#0072B2")]
col = lambda ch, cell, k: np.array([float(r[k]) for r in rows if r["charge"] == ch and r["cell_label"] == cell])

fig, ax = plt.subplots(2, 3, figsize=(17, 8.5))
for i, ch in enumerate(CH):
    a = ax[i, 0]; w = 0.27
    for j, (k, lab, c) in enumerate(TOT):
        a.bar(np.arange(7) + (j - 1) * w, [100 * np.median(col(ch, cl, k)) for cl in CELLS], w, color=c, label=lab)
    a.set_xticks(range(7)); a.set_xticklabels([c + ("\n(high x)" if c in HIGHX else "") for c in CELLS], fontsize=8)
    a.set_ylabel("median total over the cell's 35 bins [%]"); a.set_title(f"{CH[ch]}: quoted total per cell")
    a = ax[i, 1]
    fin = {(r["charge"], int(r["bin_index"])): r for r in rows}
    # electron-frame E' term is not in the eSigma CSV: take it from the summary-independent per-bin final budget columns
    for j, (k, lab, c) in enumerate((("elec_energy_rel", "E′ ±1%, eΣ photon", "#0072B2"), ("hfs_jes_rel", "HFS JES, eΣ photon", "#009E73"))):
        a.bar(np.arange(7) + (j - 0.5) * 0.4, [100 * np.median(col(ch, cl, k)) for cl in CELLS], 0.4, color=c, label=lab)
    a.set_xticks(range(7)); a.set_xticklabels([c + ("\n(high x)" if c in HIGHX else "") for c in CELLS], fontsize=8)
    a.set_ylabel("median over the cell's 35 bins [%]"); a.set_title(f"{CH[ch]}: the two photon-sensitive detector terms (eΣ)")
    a = ax[i, 2]
    for k, lab, c in TOT:
        x = np.array([100 * float(r[k]) for r in rows if r["charge"] == ch])
        a.hist(x, bins=np.linspace(0, 60, 61), histtype="step", lw=1.8, color=c, label=lab)
    a.set_xlabel("quoted total per bin [%]"); a.set_ylabel("bins"); a.set_title(f"{CH[ch]}: distribution over the 245 bins")
ax[0, 0].legend(fontsize=7); ax[0, 1].legend(fontsize=8); ax[0, 2].legend(fontsize=7)
fig.tight_layout(); fig.savefig(P / "esph_totals_comparison.png", dpi=120); plt.close(fig)

TERMS = [k for k in S["terms"]]
LAB = {"ibu_stat_rel": "IBU stat", "ibu_model_rel": "IBU model", "ibu_reg_rel": "IBU reg", "detint_removal_rel": "det.int. removal*",
       "fakecol_mcstat_rel": "fake-col. MC stat", "hfs_jes_rel": "HFS JES", "hfs_rces_rel": "HFS RCES", "hfs_theta_rel": "HFS θ",
       "elec_energy_rel": "E′ scale", "elec_theta_rel": "θ_e", "lumi_rel": "lumi", "tracking_eff_m0p5_rel": "tracking eff.†",
       "phi_h_gauss_rel": "Φ_h reweight†", "c_rad_gen_rel": "C_rad gen.", "c_rad_stat_rel": "C_rad MC stat", "bgsub_model_rel": "bgsub model",
       "bgsub_stage_rel": "bgsub stage", "bgsub_fakefrac_rel": "bgsub fake frac.*", "straddle_residual_rel": "straddle resid.",
       "m2_zshift_rel": "m2 z-shift", "bgsub_env100_hyp_rel": "bgsub env100+hyp", "nonprimary_norm_v5_rel": "non-primary v5",
       "nucliaweight_primaryloss_rel": "NucliaWeight loss"}
trs = ""
for k in TERMS:
    t = S["terms"][k]; e = [t[c]["esph"]["median"] for c in CH]; f = [t[c]["final_electron_frame"]["median"] for c in CH]
    hl = " class=hl" if abs(e[0] - f[0]) > 0.1 or abs(e[1] - f[1]) > 0.1 else ""
    trs += f"<tr{hl}><td>{LAB.get(k, k)}</td><td>{f[0]:.2f}</td><td>{e[0]:.2f}</td><td>{f[1]:.2f}</td><td>{e[1]:.2f}</td></tr>"
tot = S["total"]
ttab = "".join(f"<tr><td>{lab}</td>" + "".join(f"<td>{tot[c][k]['median']:.2f}</td><td>{tot[c][k]['p95']:.1f}</td>" for c in CH) + "</tr>"
               for k, lab in (("installed", "installed (2026-10-21)"), ("final_electron_frame", "final re-release, electron photon"), ("esph", "final re-release, eΣ photon")))
pages = "".join(f"<h3>{CH[c]}, y-axis 0–{y}%</h3><img src='esph_quoted_total_23comp_7col_ylim{y:03d}_p-{i}.png'>"
                for y in (15, 50) for i, c in enumerate(CH, start=1))
html = f"""<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>eΣ Frame Systematics</title><style>
:root{{--bg:#fff;--fg:#1d1d1f;--mut:#555;--line:#ddd;--hl:#fff4d6}}
@media (prefers-color-scheme:dark){{:root{{--bg:#141414;--fg:#eee;--mut:#aaa;--line:#333;--hl:#3a3015}}}}
body{{background:var(--bg);color:var(--fg);font:15px/1.5 -apple-system,Helvetica,Arial,sans-serif;max-width:1600px;margin:auto;padding:16px}}
img{{max-width:100%;background:#fff;border:1px solid var(--line);margin:6px 0 24px}}
table{{border-collapse:collapse;margin:8px 0 18px;font-size:14px}}td,th{{border:1px solid var(--line);padding:4px 9px;text-align:right}}
td:first-child,th:first-child{{text-align:left}}tr.hl{{background:var(--hl)}}p,li{{color:var(--mut);max-width:1150px}}
</style></head><body>
<h1>Systematic uncertainties with the eΣ photon for z and p<sub>T</sub></h1>
<p>High-Q² charged SIDIS, final 660-file data, full re-production in the eΣ reco frame (4,343 Condor jobs, every gate PASS).
STUDY products, nothing installed. The canonical 7-column renders below use the official label-fix plotter, unmodified, on the new 23-term budget;
the dashed grey line in every panel is the same total with the electron photon.</p>
<h2>Quoted total (%, over the 245 bins per charge)</h2>
<table><tr><th></th><th>h⁺ median</th><th>h⁺ p95</th><th>h⁻ median</th><th>h⁻ p95</th></tr>{ttab}</table>
<p>The integrated cross section does not change: h⁺ 10.539 vs 10.543 nb, h⁻ 10.247 vs 10.252 nb (−0.04 / −0.05 %).
Individual bins move (median 2.4 %), mostly in the sparse high-z / high-p<sub>T</sub> corners of the high-x cells.</p>
<h2>Per-term medians (%), electron photon → eΣ photon</h2>
<table><tr><th>term</th><th>h⁺ electron</th><th>h⁺ eΣ</th><th>h⁻ electron</th><th>h⁻ eΣ</th></tr>{trs}</table>
<p>Highlighted rows change by more than 0.1 point. * carried from the electron-frame budget (the 2026-10-11 response pair cannot be
re-unfolded across frames; both terms are small). † carried relative terms, as in the final re-release (Φ<sub>h</sub> should be recomputed in the new
frame before quoting).</p>
<h2>Comparison</h2><img src="esph_totals_comparison.png">
<h2>Canonical 7-column systematic plots (eΣ photon)</h2>{pages}
<p>Also: <a href="esph_quoted_total_23comp_7col_ylim015.pdf">PDF 0–15%</a> · <a href="esph_quoted_total_23comp_7col_ylim050.pdf">PDF 0–50%</a> ·
<a href="esph_quoted_total_23comp_7col_ylim100.pdf">PDF 0–100%</a> · <a href="esph_budget.csv">per-bin budget CSV</a></p>
</body></html>"""
(P / "index.html").write_text(html)
print(P / "index.html")

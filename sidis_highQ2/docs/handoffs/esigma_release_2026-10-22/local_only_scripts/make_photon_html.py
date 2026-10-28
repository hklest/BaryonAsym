"""Build a local HTML page for the photon-definition comparison. Usage: python3 make_photon_html.py <plot_dir>"""
import json, sys
from pathlib import Path
P = Path(sys.argv[1])
S = json.load(open(P / "photon_options_summary.json"))
V, M = S["validation_optA_vs_official_median_pct"], S["summary_median_pct"]
NAME = {"A": "A: electron photon (current)", "B": "B: eΣ photon everywhere",
        "C": "C: eΣ in the 3 high-x cells, electron elsewhere", "D0.2": "D: eΣ if y<0.20", "D0.25": "D: eΣ if y<0.25",
        "D0.3": "D: eΣ if y<0.30", "D0.35": "D: eΣ if y<0.35", "D0.4": "D: eΣ if y<0.40",
        "K0": "K: kinematic fit, beam fixed (σ_ISR = 0)", "K1": "K: kinematic fit, σ_ISR = 1 GeV",
        "K3": "K: kinematic fit, σ_ISR = 3 GeV", "Kinf": "K: kinematic fit, beam free (σ_ISR = ∞)",
        "Ks": "Ks: best kinematic fit, scale uncertainties in the weights"}


def row(o, ch):
    m = M[o][ch]
    cells = [m["E"], m["J"], m["stat"], m["comb"], m["comb_highx"], m["comb_lowx"], m["purity"], m["central_shift_vs_A_integrated"]]
    best = o == min(M, key=lambda k: M[k][ch]["comb"])
    return (f"<tr{' class=best' if best else ''}><td>{NAME[o]}</td>" +
            "".join(f"<td>{c:+.2f}</td>" if i == 7 else f"<td>{c:.2f}</td>" for i, c in enumerate(cells)) + "</tr>")


tables = ""
for ch, lab in (("pos", "h+"), ("neg", "h−")):
    tables += (f"<h3>{lab}</h3><table><tr><th>option</th><th>E′ term</th><th>JES term</th><th>stat</th><th>combined</th>"
               "<th>combined, high-x cells</th><th>combined, low-x cells</th><th>purity</th><th>integrated central shift vs A</th></tr>"
               + "".join(row(o, ch) for o in M) + "</table>")
val = "".join(f"<tr><td>{'h+' if ch == 'pos' else 'h−'}</td><td>{v['E_simplified']:.2f}</td><td>{v['E_official']:.2f}</td>"
              f"<td>{v['J_simplified']:.2f}</td><td>{v['J_official']:.2f}</td></tr>" for ch, v in V.items())
figs = [("po_1_validation_optA_vs_official.png", "1. Validation", "The simplified unfolding used for this comparison, run with the current definition (A), against the official production numbers. The comparison between options is only meaningful where these agree."),
        ("po_2_per_cell_pos.png", "2. Per cell, h+", "Median over each cell's 35 bins: E′-scale term, JES term, unfolded statistical error, their quadrature sum, purity and stability."),
        ("po_2_per_cell_neg.png", "2b. Per cell, h−", ""),
        ("po_3_vs_z_comb.png", "3. Combined uncertainty vs z, every cell", "Median over the 5 pT bins (log scale)."),
        ("po_3_vs_z_E.png", "3b. E′-scale term vs z, every cell", ""),
        ("po_4_threshold_scan.png", "4. Where should a y-switch sit?", "Option D (eΣ photon below a y threshold) as a function of the threshold; horizontal lines are options A, B, C."),
        ("po_5_central_value_vs_A.png", "5. Does the choice move the result?", "Unfolded data with each option divided by option A, bin by bin. Ideally ~0 within statistics: large structured shifts mean the MC does not model the resolution of one of the definitions."),
        ("po_6_kinfit_diagnostics.png", "6. Kinematic fit: what it does", "Left two panels: z and pT resolution against the generator values, vs generator y (signal tracks, all cells). Third: the weight the fit gives the measured electron energy when it estimates the true one (1 = electron only, as in A). Right: the beam-energy constraint scan (how much initial-state radiation the fit allows); the option with the best high-x z resolution is used as K.")]
fig_html = "".join(f"<h2>{t}</h2><p>{c}</p><img src='{f}'>" for f, t, c in figs if (P / f).exists())
html = f"""<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Photon Definition Comparison</title><style>
:root{{--bg:#fff;--fg:#1d1d1f;--mut:#555;--line:#ddd;--hl:#e8f5e9}}
@media (prefers-color-scheme:dark){{:root{{--bg:#141414;--fg:#eee;--mut:#aaa;--line:#333;--hl:#1e3a24}}}}
body{{background:var(--bg);color:var(--fg);font:15px/1.5 -apple-system,Helvetica,Arial,sans-serif;max-width:1500px;margin:auto;padding:16px}}
img{{max-width:100%;background:#fff;border:1px solid var(--line);margin:6px 0 24px}}
table{{border-collapse:collapse;margin:8px 0 18px;font-size:14px}}td,th{{border:1px solid var(--line);padding:4px 8px;text-align:right}}
td:first-child,th:first-child{{text-align:left}}ul.bl{{max-width:1100px}}tr.best{{background:var(--hl);font-weight:600}}p{{color:var(--mut);max-width:1100px}}
</style></head><body>
<h1>Which photon for the hadron z and pT?</h1>
<p>High-Q² charged SIDIS, final 660-file data, RAPGAP response (73 files, ~20% of the production MC), simplified IBU (4 iterations,
no background subtraction) — identical machinery for every option, so the comparison between options is what counts.
The E′ term and the JES term are the symmetric half-differences of ±1% shifts (MC-only definition).
"Combined" = E′ ⊕ JES ⊕ unfolded statistical error. Numbers are medians over bins, in %. Best combined row highlighted.</p>
<h2>Bottom line</h2><ul class=bl>
<li><b>eΣ photon in the high-x cells (C, your hybrid):</b> it captures most of the gain where it matters. The E′ term in the
high-x cells drops from ~8.5% to ~3.4–4.3%, and purity rises from ~35% to ~44%. Using eΣ everywhere (B), or below y≈0.4 (D),
is slightly better still, because the low-x cells also gain a little.</li>
<li><b>Kinematic fit (K):</b> per event, a least-squares fit of the true scattered-electron energy to the measured E′, the
hadronic Σ and the hadronic pT, with the beam energy fixed. Resolutions and calibration come from MC in bins of y.
<ul><li>It gives the best resolution everywhere: the z width at y≈0.1 is 11% (electron 22%, eΣ 13%), and the pT width is 65 MeV
(electron 127 MeV, eΣ 77 MeV) (Fig. 6).</li>
<li>It gives the best purity: ~50% in the high-x cells (eΣ 44%, electron 35%) and ~62% in the low-x cells (~59%).</li>
<li>At y≈0.1 it uses the electron energy only ~20% and the hadrons ~80%. It uses the electron energy more and more as y rises,
reaching ~80% at y≈0.65 (Fig. 6, third panel). That is exactly the "eΣ at low y, electron at high y" idea, done smoothly per event.</li>
<li>Its E′ term is about the same as eΣ's: overall 2.4/2.5% vs 2.6/2.8%; in the high-x cells 3.2% vs 3.4/4.4%. Its JES term is about
half of eΣ's in the high-x cells (0.5% vs 1.0–1.5%).</li>
<li>Its combined E′ ⊕ JES ⊕ stat is on a par with B. The version with the ±1% scale uncertainties folded into the per-event
weights (Ks) is the best overall: 5.2/5.5% vs B 5.1/5.7% and A 6.8/7.3%.</li></ul></li>
<li><b>Caveats for the fit.</b>
<ul><li>It moves individual bins more than eΣ does: median |shift| vs A is 3.5/2.8% (B 2.4%). The integrated value is unchanged (−0.01%),
and the movement is mostly in the sparse high-z/high-pT corners (Fig. 5).</li>
<li>It leans on the MC description of the hadronic final state. That covers the hadronic Σ scale (MC says 0.94 of truth), its
resolution, and the Σ–pT correlation (ρ≈0.55). It also leans on the radiative tail, because the beam is fixed: allowing initial-state
radiation in the fit made the resolution worse (scan, Fig. 6 right).</li>
<li>Before adopting it, check data vs MC for the fit pulls and χ², and add an HFS-resolution systematic.</li></ul></li>
<li><b>Any of B, C, D or K</b> changes the central-value definition, so the response and detector legs must be re-produced.</li></ul>
<h2>Validation of the simplified chain (option A vs the official production, median %)</h2>
<table><tr><th>charge</th><th>E′ (this study)</th><th>E′ (official)</th><th>JES (this study)</th><th>JES (official)</th></tr>{val}</table>
<h2>Summary (median over bins, %)</h2>{tables}
{fig_html}
<p>Code: NAF scripts/unfolding/final_rerelease_2026-10-22/elecE_study/ (photon_options_study.py, photon_options_analyze.py).</p>
</body></html>"""
(P / "index.html").write_text(html)
print(P / "index.html")

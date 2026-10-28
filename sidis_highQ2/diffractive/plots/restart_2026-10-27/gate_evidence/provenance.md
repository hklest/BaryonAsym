# Gate-evidence figures: provenance

Produced 2026-10-28. No gate was re-evaluated and no stored gate output was changed. The figures re-plot the stored
fit payloads (fig1) and histogram the stored WP-A3 skims (fig2-5) with the gates' own selection, weight and map code.

## Scripts

All in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/gate_evidence/`:
- `hist_ge.py`: Condor job over the 17 skims. It imports `fact_wpa3.py` (masks, Plug-state classes, fine-class
  setting) and `wpa_common.py` (cocktails, exposure rescale) and writes per-group sumw/sumw2 histograms. It re-runs the
  `skim_identity_s4` test (PASS) and reproduces the WP-A3 run-2 f_fail deltas, unmapped and XA-mapped, to 1e-9 for 7071,
  7091 and nd4cat (`checks` block of the output).
- `hist_ge.sub` and `hist_ge_job.sh`: the submit file and wrapper. Cluster 5919647 ran 37 s wall, 733 MiB, exit 0.
  Cluster 5919466 failed only at the final JSON dump (numpy bool) and was superseded.
- `plot_ge.py`: login node, reads only the JSON files below. It writes the PNGs and
  `data/restart_2026-10-27/gate_evidence/summary.json` (the ratio numbers quoted in `captions.md`).
- `ge_style.py`: the shared main-plus-ratio panel drawing.

## Inputs (absolute paths)

fig1 (stored G1′(c) fits; nothing re-fitted):
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a2/s4_g1p/payloads/odd_frozen_{7071,7091,nd4cat}.json`
  (sha256 prefixes c619a1fc02dd18b3, 6718b915e337947c, 2757c2db34798ba0): per-component raw and exposure1 templates, edges.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a2/s4_g1p/fits/section_7_1_twofree_fit_results_{7071,7091,nd4cat}_{fine,coarse}_{D0,D1,D2}.json`:
  fitted s_diff and s_ND, coarse groups, deviance, toy GoF. `plot_ge.py` asserts that the restacked prediction equals the
  stored `prediction`.

fig2-5 (histograms re-derived from skims, because the stored N−1 partials carry neither per-cell histograms nor sumw2,
and no stored mapped histograms exist):
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a3/skims/{ep0304,em0405,em06_true,ep0607_true,9012,9024,9033,9155,9157,9158,6671,6677,7071,7091,7094,7096,7097}.npz`
  (sha256 of each recorded in `hists_ge.json` → `inputs`).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a2/s4_g1p/n1/{data,mc}/<label>/partial.json`
  (identity reference only).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a3/fact/fact_results_fine.json`
  (cross-check reference only).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-a3/fact_wpa3.py`
  (sha256 prefix 50452ee5b61f3f20, identical to the version that wrote `fact_results_fine.json`), and
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-a/wpa_common.py`
  (63b9008f6bda93cc), together with the v3 scale table and `period_map.csv` that it reads.

Intermediate output:
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/gate_evidence/hists_ge.json`
  (sha256 prefix 31614fc1baea2e0e, 908 histograms).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/gate_evidence/summary.json`.

## Definitions used (as in the gates)

- N−1 samples (fig2, fig3, fig5 left): stack C, `fact_wpa3.masks` N1_PLUG / N1_FMD / N1_FTS, odd split.
- FIT (fig1, fig4c, fig5 right): selFmd ∧ E_Plug < 7 ∧ selFts, no η_max cut.
- H4 (fig4a): 35 < E−p_z < 70, no veto. H4E (fig4b): H4 ∧ E_Plug > 20 GeV. Both use the even split, as WP-A2 S2 does.
- Map (fig5): WP-A3 run 2, fine Plug classes, x-set XA, derived on CTRL even.
- MC-stat band: √Σw² from the skims (fig2-5). For fig1 it is Σ(s·w)²/raw per component, the conversion the gate toys use.

## Veto-efficiency view (fig6, fig7), added 2026-10-28

- `veto_eff_hist.py` (sha256 prefix 307ef430b5ce7446): Condor job over the same 17 WP-A3 skims, the same
  `fact_wpa3.py` (50452ee5b61f3f20), `wpa_common.py`, and hist_ge.py base weights (signed v3 × fac_used/table_fac, no
  veto map). It writes pass/fail sumw/sumw2 per cell × η08 bin × M_X bin × parity for data, and for DIFF8 signal,
  Reggeon and ND per chain (7071, 7091, nd4cat). It covers the N1_PLUG / N1_FMD / N1_FTS masks and the combined veto
  (COMB: parity ∧ η08 < 3.3|sentinel, fail = any veto; added before the job started). Built-in check: the odd Plug
  N−1 f_fail deltas per D cell equal `hists_ge.json` to 1e-9 for all three chains (PASS). The selFts flag equals
  nFts28 == 0 in all skims (0 mismatches).
- `veto_eff_hist.sub` and `veto_eff_hist_job.sh`: the wrapper sets
  `OUT=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/gate_evidence/veto_eff_hists.json`.
  Cluster 5925288 was submitted 02:01 and started 03:38 on batch1189. It ran 20 s wall, 269 MiB, exit 0. The logged
  script md5 is 4b6709e1ae08785ac68482eb9a51d6d6 (the COMB version).
- `plot_veto_eff.py` (29ccd635e10ab10c): login node, reads JSON only. Inputs: `veto_eff_hists.json` (f76a00fb007a7547),
  `wp-a2/s4_g1p/fits/section_7_1_twofree_fit_results_{7071,7091,nd4cat}_fine_{D0,D1,D2}.json` (fitted s_diff, s_ND),
  `wp-a/s2_remedy1/noise_table.json` (f93a6dd270f56ec1; per-period q and luminosity for the remedy-1 reference), and
  `gate_evidence/summary.json` (fig2 consistency check, PASS to 1e-9). It writes fig6, fig7 and
  `data/restart_2026-10-27/gate_evidence/veto_eff.json` (12ed5f75ecac0b0f).
- M_X edges 10.24 / 16.14 GeV: tertiles of 11557 finite-M_X data events in the deep-gap Plug N−1 sample (D cells,
  both parities), fixed in the histogram job.
- Splits: every quantity is stored for the odd (gate test) and even EventNumber parity; the figures and captions show odd,
  and `veto_efficiency_view.md` quotes even alongside odd. Pass-efficiency ratios (1−f_data)/(1−f_MC) and their flatness χ²
  (`kinematics` → `flatness_pass_eff_ratio`) were added to `plot_veto_eff.py` before the final run that wrote the JSON and PNGs.
- Record: `docs/restart_2026-10-27/gate_evidence/veto_efficiency_view.md` (80 lines).

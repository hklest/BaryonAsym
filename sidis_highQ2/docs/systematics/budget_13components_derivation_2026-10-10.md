# Canonical 13-component z005 systematic budget — component derivations and background-uncertainty status (2026-10-10)

Reference record from a read-only audit; companion to the plotted-band comparison
`data/systematics/plotted_band_vs_budget_2026-10-10.csv` (audit of 2026-10-10).
Scope: v6/z005 coherent-prior lineage, 490 bins (245 h+ / 245 h−), IBU k=4,
`L_data = 351000.0 nb^-1`. All components are per-bin RELATIVE (fractional);
the budget is invariant under the C_rad Born flip (only absolute-nb products rescale).

## Budget identity and supersession state

- Audited package (user-named canonical):
  `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/`
  — 13 components; `total_with_crad13_rel = sqrt(sum of 13 components^2)` (quadrature closure
  exact, max dev 0.0); totals h+ median 5.7125% / max 160.626%, h− median 6.1448% / max 84.773%.
- **Same-day supersession (STATE.md 2026-10-09 addendum item E):** the current canonical
  budget is `.../total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/`.
  Only `c_rad_gen_rel` changed (RG31 NONRAD e− exposure repair, +53,773.7 nb^-1): medians
  h+ 0.5505% → 0.5844%, h− 0.5166% → 0.5804%; totals h+ 5.7093% / h− 6.1523%. The other
  12 components are byte-identical (0/14,700 mismatches). The audited package carries
  `README_SUPERSEDED_2026-10-09.md` and is retained read-only as provenance. All statements
  below apply to both packages except where marked [emfix].
- The 11 non-C_rad components were carried byte-identical through both crad13 rebuilds
  (verified 0 mismatches in 29/30 carried columns × 490 rows, `validation_checks.json`).

## Component register — derivation verified from build machinery/provenance

| # | Component | Derivation (verified source) | Median h+ / h− |
|---|---|---|---|
| 1 | `ibu_stat_rel` | 1000-toy data-statistical IBU: each toy Poisson-fluctuates the rounded measured physical-reco vector, applies the fixed per-bin fake fraction `d_sig=d(1−f)`, unfolds at fixed k=4 with fixed RAPGAP response + coherent `body+miss` prior; per-bin toy spread (cluster 5380017, seeds h+ 12345 / h− 22345; refreshed into the release sidecar) | 3.5900 / 3.8314 % |
| 2 | `ibu_model_rel` | Response cross-closure `0.5·|IBU(R_DJ)−IBU(R_RG)|/nominal` (both branches with own `body+miss` priors, cluster 5379991), then per-bin B1 floor `max(base, b1_floor_coherent)`; B1 floor = per-bin spread of 100 response-bootstrap replicas (Poisson-resampled response cells, `n_eff=sumw²/sumw2`, seeds 2026100400+r, replica-own prior, cluster 5380196) | 1.4006 / 1.4255 % |
| 3 | `ibu_reg_rel` | IBU regularisation k-scan: `0.5·|IBU(k=5)−IBU(k=3)|/nominal` around nominal k=4 | 0.5769 / 0.5662 % |
| 4 | `hfs_jes_rel` | HFS hadronic energy scale (JES), one-sided shift-DOWN full-chain rerun (shifted response + shifted data vector, shifted-response `body+miss` prior, k=4); `|shifted−nominal|/nominal`. Frozen final = paired ±1% in-jet only (kT R=1.0, pT>5 GeV, −1.5<η<2.75) — current one-sided value PROVISIONAL | 0.3052 / 0.3498 % |
| 5 | `hfs_rces_rel` | Same method, one-sided shift-UP, HFS energy scale outside the JES jets (RCES). Frozen final = paired ±1% out-of-jet; PROVISIONAL | 0.2410 / 0.2446 % |
| 6 | `hfs_theta_rel` | Same method, one-sided shift-UP, HFS polar angle (frozen ±1 mrad). Two-sided HFS-theta response is quarantined (even/quadratic); PROVISIONAL | 0.2406 / 0.2783 % |
| 7 | `elec_energy_rel` | Same method, one-sided scattered-electron energy +1% (`elecE_p1pct`). Frozen final = paired ±0.5% per each of five LAr wheels — the +1% vector is ~2× the frozen amplitude; PROVISIONAL | 1.3537 / 1.6635 % |
| 8 | `elec_theta_rel` | Same method, one-sided scattered-electron polar angle +1 mrad (`elecTheta_p1mrad`). Frozen final adds three-region `eElecTh` + co-shifted matched-track `eElecTrTh`; PROVISIONAL | 0.4524 / 0.5000 % |
| 9 | `lumi_rel` | Flat 2.7% luminosity, identical in all 490 bins; H1-standard absolute-normalisation uncertainty for this dataset (Andreev 2024 §6.3 precedent); fully correlated rank-1 across bins and charges; excluded from multiplicities and A_h | 2.70 / 2.70 % |
| 10 | `tracking_eff_m0p5_rel` | Random −0.5% tracking-efficiency track-drop ensemble, 16 seeds 314159–314174, one unfold per seed; coherent-lineage regeneration (16/16 valid). Distinct from the frozen analytic 1.0%⊕0.2% central-tracking prescription; `seed_rms`/`seed_mean_unc` are QA-only | 0.1806 / 0.1828 % |
| 11 | `phi_h_gauss_rel` | φ_h flat-reweight sensitivity: per generator (DJANGOH14, RAPGAP31) `delta=(flat-unfold−nominal)/nominal`; Gaussian-smooth `|delta|` in each 5-bin pT row (σ=1.0 pT bin, edge-truncated renormalised); component = 0.5·max over generators of smoothed `|delta|`, floored `max(smoothed, 0.85·unsmoothed half-envelope)`. Diagnostic; includes fork/prior + generator-model contamination (pure flat-vs-unity ≈0.6%) | 1.0482 / 1.0550 % |
| 12 | `c_rad_gen_rel` | C_rad cross-generator spread, V3 recipe: `0.5·GaussSmooth_pTrow(|rel|)`, `rel=(C_rad_RG31−C_rad_DJ14)/C_rad_DJ14` on the releasebin pair, kernel `exp(−0.5Δ²)`, σ=1.0 pT-bin, edge-truncated renormalised per (charge,(Q2,x) cell,z) 5-point pT row, on `|δ|` not signed; independent recompute max dev 6.94e-18. [emfix: medians 0.5844 / 0.5804 %] | 0.5505 / 0.5166 % |
| 13 | `c_rad_stat_rel` | The APPLIED DJANGOH14 releasebin table's own `relative_stat_unc` column (DJ14 leg only, NOT the combined `u_diff`); matches the table exactly (max dev 0.0). Rank-1 covariance in `covariance_cradstat/` is a 100%-coherence envelope assumption, not a measured correlation | 0.4272 / 0.4429 % |

Component maxima (audited package): h+ — ibu_stat 31.34%, ibu_model 17.83%, ibu_reg 29.77%,
elec_energy 141.12%, elec_theta 65.95%, phi_h 16.09%, c_rad_gen 5.87%, c_rad_stat 4.41%,
hfs_jes 2.96%, lumi 2.70%, tracking 2.81%, hfs_theta 2.07%, hfs_rces 1.69%;
h− — ibu_stat 55.70%, ibu_model 18.64%, elec_energy 61.27%, phi_h 15.35%, ibu_reg 13.64%,
elec_theta 14.71%, c_rad_stat 6.04%, c_rad_gen 5.70%, hfs_jes 3.90%, lumi 2.70%,
hfs_theta 5.10%, hfs_rces 6.62%, tracking 2.58%.

## Provenance-thinness flags

1. **lumi 2.7%:** budget provenance records only the flat assignment (`lumi_rel = 0.027 for
   all 490 bins`); no in-workspace derivation artifact exists. The value is externally
   anchored (H1 publication standard for the same 351 pb⁻¹ dataset, Andreev 2024 §6.3 as
   cited in the definition spec). Thinner than the other components' in-workspace chains.
2. **Detector legs 4–8 are one-sided historical shifts**, not the frozen paired ±
   prescriptions; elec_energy +1% is ~2× the frozen ±0.5%-per-wheel amplitude; all five are
   labelled PROVISIONAL in `FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` §1/§2.2.
3. **phi_h_gauss_rel** is a diagnostic, "not an approved systematic" (its own package README).
4. **B1 floor:** `b1_aggregation_status: FAIL` (4 supported bins exceed the 3-SEM
   replica-mean working-point check) is carried as a report-only flag.
5. **c_rad_stat_rel covariance** asserts 100% bin-to-bin and cross-charge coherence — an
   envelope, not a measurement; C_rad exposure provenance is FILE-SET-RECONCILED
   (generator-log-open), C_rad still NOT publication-grade.
6. The budget as a whole is NOT the final publication budget: the spec's 24-row register
   counts IMPLEMENTED 4 / PROVISIONAL 6 / BLOCKED 9 / DROPPED 5, and the 13-component total
   excludes all BLOCKED rows (below).

## Background uncertainty — verdict: ABSENT from the budget

- **No component of the 13 is a background / fake-fraction / residual-background
  uncertainty.** The definition-spec register lists `background_rel` (row 17) as **BLOCKED**:
  the former retirement is not final — its huge-bin rationale was voided by the 2026-09-25
  fLumi pb⁻¹ correction, and the track-level construction lacks `overlap_980` and
  `n_raw_980`; P-ALPHA remains open. `diffvm_rel` (row 18) is BLOCKED (DIFFVM was an assumed
  zero; rho was assessed 2026-10-07 — expected 306.31±6.45 tracks, integrated fraction
  0.0074% — but phi/omega/proton-dissociative samples remain unassessed).
  `gammap_bkg_30pct_rel` (row 19) is BLOCKED (no gamma-p template is subtracted; the spec
  requires documenting no subtraction rather than emitting zero).
- **What the chain does contain:** the central release is explicitly **raw/no-bgsub** — no
  background template is subtracted, electrons and V0s are vetoed in the selection, and the
  only background-related handling is the fixed **response fake-fraction subtraction**
  `d_sig = d·(1−f_i)` with `f = 10.7141%` (h+) / `10.4019%` (h−), applied as a correction
  with **no propagated uncertainty** (the stat toys hold it fixed; the B1 replicas resample
  the signal response). At Born level the remaining unsubtracted track-level background is
  multiplied by C_rad exactly like signal (Born release README). The `cRad_bgfracsub`
  variant stays a separate derived product, never merged into the nominal.
- **Carried open items (STATE.md / RUN_STATUS.md, both 2026-10-09):** P-ALPHA
  (alpha-veto/background response-consistency validation) OPEN; alpha0p2 near-electron,
  reco-background and all-fake response products diagnostic/provisional; track-level
  background systematic BLOCKED on missing `overlap_980`/`n_raw_980` in the alpha0p2
  aggregates; every promotion's "does not cure" list carries "raw/no-bgsub with response
  fake-fraction subtraction only (10.7141%/10.4019%)" plus P-ALPHA; an A-F3
  charge-asymmetry systematic is mandatory before any A_h quote.
- **Consequence for quoting:** the 13-component `total_with_crad13_rel` must be quoted as
  excluding background uncertainty (and the BLOCKED register rows generally); it is the
  current canonical *budget basis*, not a complete publication-grade error. This absence
  compounds the plotted-band audit finding: neither the plotted release-native band nor the
  13-component budget includes a background term.

## Provenance files consulted (primary evidence)

- Budget package: `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/` — `README.md`, `README_SUPERSEDED_2026-10-09.md`, `metadata`-level `provenance.json`, `validation_checks.json`, `rebuild_crad13_releasebin_budget.py`, `crad_overlap_audit.json`
- emfix successor: `.../total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/README.md`
- Stat toys: `H1ChargedSidis_HighQ2/output_v6/systematics/stat_toys_coherentprior_2026-10-07/README.md`
- Model/reg: `H1ChargedSidis_HighQ2/output_v6/systematics/ibu_native_coherentprior_2026-10-07/README.md`
- B1 floor: `H1ChargedSidis_HighQ2/output_v6/studies/p1_replica_floors_coherentprior_2026-10-07/README.md`
- Detector shifts: `H1ChargedSidis_HighQ2/output_v6/systematics/detector_shifted_coherentprior_2026-10-07/README.md`; base build `_run_total_uncertainty_summary.py` (2026-06-13) and `total_uncertainty_z005_2026-06-13_with_electron_lumi/provenance.json` (lumi flat 2.7%)
- Tracking: `H1ChargedSidis_HighQ2/output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/ensemble_ibu_data4p_v2_20260703/tracking_eff_m0p5_ensemble_systematic.json`; coherent regeneration per RUN_STATUS (16/16)
- phi_h: `H1ChargedSidis_HighQ2/output_v6/systematics/phi_h_coherentprior_2026-10-06/README.md`; adoption `docs/systematics/flatphi_phih_systematic_adoption_2026-10-02.md`
- Definitions/status: `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md`; `STATE.md` (2026-10-09, incl. emfix addendum items B–F); `docs/RUN_STATUS.md` (releasebin promotion section); `context-handoff.md`
- Release side: `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/README.md` (sidecar refresh, 100-toy retention in `total_unc_*`); driver `run_bbb_ibu_release_coherentprior.py` line 115 (release-native total = stat⊕model⊕reg); Born release `born_crad_dj14relbin_..._2026-10-09/README.md` + `metadata.json` (open item: C_rad terms not folded into `total_unc_nb`; fake-fraction numbers)

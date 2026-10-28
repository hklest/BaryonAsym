# phi_h Flat-Rewrite Systematic — Budget Adoption Record (2026-10-02)

**Decision (user, 2026-10-02):** the Gaussian rolling-window phi_h flat-reweight systematic is adopted into the z005 uncertainty budget as an 11th component (`phi_h_gauss_rel`), with a new total `total_with_phih_rel`. Status remains **PROVISIONAL INTERNAL DIAGNOSTIC**: not physics-approved; the central release remains raw/no-background-subtraction/no-`C_rad`/not Born level.

## Definition

`phi_h_gauss_rel` = 0.5 x max over {DJANGOH14, RAPGAP31} of the Gaussian rolling-window smoothed per-generator |sigma_flat - sigma_nominal| / sigma_nominal.

- Kernel: exp(-0.5*((i-j)/sigma)^2), sigma = 1.0 pT bin, truncated at row edges and renormalized; applied within each (charge, (Q2,x) cell, z) 5-bin pT row.
- Applied to |delta|, NOT the signed delta: signed smoothing leaves zero-crossing dips near zero and does not fix the unphysically small pointwise uncertainties.
- Nominal = central release IBU k=4 cross sections (`bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/ibu/cross_sections.csv`).
- Flat = iteration-1 generator-level flat-phi_h study unfolds (`output_v6/studies/flatphi_genreweight_z005_20260925/ibu/{djangoh14,rapgap31}_flat/`).
- New total: `total_with_phih_rel = sqrt(total_with_tracking_rel^2 + phi_h_gauss_rel^2)`.

## Values

| Quantity | h+ | h- |
|---|---:|---:|
| `phi_h_gauss_rel` median / max | 1.376% / 16.264% | 1.345% / 11.072% |
| `total_with_phih_rel` median / max | 6.047% / 162.887% | 6.356% / 89.168% |

## Method-selection record

- Unsmoothed half-envelope vs central nominal: median 1.360% / 1.365%, jumpy bin-to-bin.
- Noise-weighted local-linear tricube smoothing (h=2.01) with >2-sigma halfA-noise excursion guard: barely changes medians; does not lift zero-crossing dips (18 h+ / 16 h- bins; median 0.475% / 0.396% at dips).
- Gaussian rolling window (sigma=1.0) on |delta|: lifts zero-crossing dips to median 1.766% (h+) / 0.820% (h-); chosen by the user.
- Proposal doc: `docs/systematics/flatphi_unc_smoothing_proposal_2026-10-02.md`.

## Caveats (must travel with any use of this source)

1. The envelope is taken against the central nominal, so `phi_h_gauss_rel` includes fork/prior (stored-prior provenance) and DJ-vs-RG generator-model (~1.6% median, already covered by `ibu_model_rel`) contributions. Pure phi_h flat-vs-unity sensitivity is ~0.6% median. Decomposition sidecar: `plots/systematics/flatphi_env_7panel_2026-10-02/flatphi_env_decomposition.csv`.
2. No noise guard: the Gaussian smoothing suppresses 10/37 (h+) and 13/29 (h-) of the >2-sigma halfA-noise significant excursions.
3. Diagnostic only; the publication-canonical 7-column systematic plotter remains unresolved (`P-PLOTS`).

## Artifacts

- Budget (11 components): `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_with_phih_2026-10-02/` (`total_uncertainty_summary_with_phih.csv`, 490 rows; `provenance.json`; `README.md`; build script).
- Plot package (forked expanded 7-panel plotter, all sources + phi_h, totals excl./incl. phi_h): `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/canonical_7panel_with_phih_2026-10-02/` (6 one-page PDFs, 6-page review PDF, 6 PNGs, `source_table_with_phih.csv`, manifest).
- Test-plot lineage: `plots/systematics/flatphi_env_7panel_2026-10-02/` (v1 envelope), `plots/systematics/flatphi_env_halfdiff_7panel_2026-10-02/` (v2 half-difference; `smoothing_demo/` holds the method comparison and per-bin sidecars).
- Underlying study: `docs/systematics/flatphi_gen_reweight_sensitivity_2026-09-25.md`.

## Validation (PASS)

490 rows in every merged table; exact (charge, bin_index) key and label alignment; all values finite and non-negative; 245 h+ + 245 h-; `total_with_phih_rel >= total_with_tracking_rel` in all bins; review PDF and PNG page counts 6/6; no existing artifact or central product overwritten.

## Addendum — 85% Floor Adoption (2026-10-03)

**Decision (user, 2026-10-03):** the phi_h systematic now carries an 85% floor: `phi_h_gauss_rel = max(Gaussian rolling-window smoothed value, 0.85 x unsmoothed half-envelope)` per bin. Rationale: the plain Gaussian suppressed excursion bins (floor binds 77/245 h+ and 86/245 h- bins, median gauss/raw ratio ~0.72 at those bins); the floor keeps every bin at >= 85% of the unsmoothed value while retaining the zero-crossing fix.

**Floored values:** h+ median 1.485% / max 16.264%; h- median 1.415% / max 15.743%. New total `total_with_phih_rel`: h+ median 6.047% / max 163.245%; h- median 6.389% / max 89.243%.

**Current artifacts (floored):**
- Budget: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_with_phih_floor_2026-10-03/`
- Expanded plot: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/canonical_7panel_with_phih_floor_2026-10-03/`
- 4-sources-per-page vs pT: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/canonical_7panel_with_phih_floor_4src_2026-10-03/`
- 4-sources-per-page vs z: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/canonical_with_phih_floor_4src_vsZ_2026-10-03/`
- Floor impact assessment: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/flatphi_env_halfdiff_7panel_2026-10-02/smoothing_demo/phi_h_gauss_85floor_impact_2026-10-03.json`

**Superseded:** all unfloored 2026-10-02 budget/plot packages and the unfloored `canonical_with_phih_4src_vsZ_2026-10-03/`; do not quote their phi_h or total values. Status remains PROVISIONAL INTERNAL DIAGNOSTIC.

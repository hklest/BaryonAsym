# Pipeline touch-point map — ep cross section (CREATED / CONSUMED / LABELLED) for a gamma*p post-processing derivative

**Date:** 2026-10-15
**Scope:** READ-ONLY inventory of every place in the current high-Q2 inclusive charged-hadron SIDIS chain where the ep cross section is created, consumed, or labelled, in preparation for a derived gamma*p presentation

```
sigma^{gamma*p}(bin) = sigma^{ep}(bin) / Gamma(bin)      [Gamma = Hand-convention virtual photon flux]
```

applied as a **post-processing derivative of the existing Born-level release** (no re-unfolding, no response rebuild).

**Parent products inventoried (current central, cone-basis, since the 2026-10-13/14 promotion):**

- Born release: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (54 files; IBU h+ 13.026987 / h- 12.697511 nb)
- Raw parent: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ 11.412233 / h- 11.123626 nb)
- Canonical 15-component budget (superseded same-day): `.../data_release/systematics_cone_s2prime_15comp_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/`
- **LEG-ENVELOPE successor (current canonical budget):** `.../data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/` (22 files; `total_with_cone15_rel` medians h+ 5.8032% / h- 6.1919%; install record `INSTALL_RECORD_shiftedlegs_legenv_budget_2026-10-14.json`)

**Headline structural fact (governs the whole design):** the release-table `cross_section_nb` is the **BIN-INTEGRATED ep cross section in nb** (`N_Unf / L_data`). **No bin-width division happens at release level.** The 4D differential quantity `d4sigma/dQ2 dx dz dpT,h` is computed only downstream, independently, in each plotter/table generator — with **two different x-width conventions** in live use (see §2.3 and §6-R6). **[SUPERSEDED 2026-10-16 — P-XVOL: the canonical plotter was patched on 2026-10-16; a single published x-width convention is now in live use, `dx = 1.0 − x_min` (S3_x0: `dx = 0.979`); the internal container edge `x_max = 1.5` exists only to absorb migrations; it is not a published bin and there is no truth-level support above `x = 1`.]**

---

## 1. PRODUCERS — where `cross_section_nb` is created

### 1.1 Yield → ep cross section (the raw release step)

**Canonical driver (produced the current raw parent):**
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_2026-10-11/run_bbb_ibu_release_cone_s2prime.py` (md5 `d1cb6779102f02dfbf53effb22ad4229`)

- `LUMI=351000.0` — line 195.
- `write_method(method, results, toys)` — def line 608; per-row assembly line 628. Exact implemented formula:

```
cross_section_nb      = float(vec[i] / LUMI)          # vec = IBU N_corr at k=4 (ibu["N_corr"][sel], line 613) or BBB N_corr
stat_unc_nb           = stat_for_total[i] / LUMI
analytic_stat_unc_nb  = err[i] / LUMI
model_unc_nb / reg_unc_nb / toy_stat_unc_nb / total_unc_nb / total_unc_nb_diag = <yield-unit counterpart> / LUMI
total_unc_rel         = |total[i] / vec[i]|
yield_unfolded        = float(vec[i])                  # raw unfolded counts, no C_rad ever
```

- **No acceptance/efficiency factor at this step**: detector effects are inside the unfolding (`ibu_unfold`/`bbb_unfold`, lines 601-603, coherent `body+miss` prior, per-reco-bin variant `f_i` fake-fraction subtraction inside the response, line 600). `acceptance_rapgap` / `prior_gen_rapgap` are shipped as sidecar columns only.
- **No bin-width division** (no ΔQ² Δx Δz ΔpT_h anywhere in the driver): `cross_section_nb` is bin-integrated nb. In the raw release `yield_unfolded = cross_section_nb × L_data` holds exactly.
- Outputs: `ibu/cross_sections.csv` + `.json`, `bbb/...`, `summary_totals.json` (`ibu_total_nb = vec.sum()/LUMI`, line 200 of the snapshot twin), `bin_map.csv/json` (edges only — **no y, no bin centers**).

**Lineage twins of the same formula (superseded bases, provenance only):**
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/run_bbb_ibu_release_s2prime_stub.py` — `LUMI=351000.0` line 104; row assembly line 340 (no-cone S2' release).
- Canonical-snapshot registry copy: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/_run/run_bbb_ibu_release.py` — `LUMI=351000.0` line 26; row assembly line 105 (superseded plain-prior 2026-06-10 release). The **local snapshot** `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/` contains NO release driver — only the two Born plotters in `release/scripts/`.
- The raw parent release carries **no `scripts/`**; its driver source is archived in the Born release at `provenance/release_driver_source_cone_s2prime.py`.

### 1.2 Raw → Born ep cross section (the Born release step)

**Builder (in the Born release `scripts/` dir):**
`.../born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/scripts/build_born_candidate_cone_s2prime.py` (md5 `58182da07e8d3f051503869b3a6d16fe`; wrappers `run_born_candidate_cone_s2prime.sh`, `submit_born_candidate_cone_s2prime.sub`; cluster 5570514)

- `sigma_Born(bin) = C_rad(bin) × Unfold(data; R)` — post-unfold, per-bin multiplication.
- Applied primary table: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (sha256 `cf810be4...`; integrated h+ 1.1415172920054786 / h- 1.1410367377289135). Formal alternative: `.../rapgap_crad_releasebin_emfix_2026-10-09/crad_v3_..._490rows.csv` → `alternative_scheme_rapgap31_relbin_emfix/` subproduct.
- Column scope: `ABSOLUTE_NB_COLUMNS` (lines 329-331) = `cross_section_nb, stat_unc_nb, analytic_stat_unc_nb, model_unc_nb, reg_unc_nb, toy_stat_unc_nb, total_unc_nb, total_unc_nb_diag`, all × per-bin C_rad **bitwise** under the `float_precision='round_trip'` exact-text convention; arithmetic authority = VERBATIM archived canonical merge block `provenance/canonical_merge_block_extracted.py` (block1 lines 6-29, block2 lines 32-101; executed via `run_canonical_merge`, line 838; factor map `load_factor_map`, line 879; independent recomputation `independent_crad_apply`, line 896).
- Yields stay byte-identical (YIELD POLICY CHOICE A): **the relation `yield = cross_section_nb × L_data` is DELIBERATELY BROKEN in the Born release** (documented in README and in the `raw_yield_vs_born_cross_section_warning` key). Exceptions: `toy_stat_unc_yield` := 1000-toy sidecar value; `toy_stat_unc_nb` := (sidecar/L_data) × C_rad.
- Per-bin applied factors shipped: `provenance/crad_applied_per_bin_djangoh14_relbin.csv`, `..._rapgap31_relbin_emfix.csv`.

**Earlier Born builders (same pattern, provenance only):** `sidis_highQ2/scripts/radiative/born_candidate_release_2026-10-07/build_born_candidate.py` (xFcut basis), `sidis_highQ2/scripts/radiative/born_candidate_release_2026-10-09/build_born_candidate.py` (releasebin basis).

**Whole-chain implemented formula, one line:**
`sigma_ep,Born(bin) [nb] = C_rad(bin) × N_Unf(bin) / 351000.0`, bin-integrated; `d4sigma/dQ2 dx dz dpT,h = sigma_bin / (dQ2·dx·dz·dpT_h)` computed only downstream.

---

## 2. LABELS / TEXT — where the observable is described as the ep cross section / d4sigma

### 2.1 Release metadata (Born release dir; keys in `metadata.json` / `summary_totals.json` / `manifest.json` / README)

| File | Key / location | Wording |
|---|---|---|
| `README.md` | "Definition and arithmetic" | "`sigma_Born = C_rad x Unfold(data; R)` — post-unfold, per-bin multiplication" |
| `README.md` | "Products that carry absolute cross sections (nb) — scaled" | per-product scaling table |
| `README.md` | "Yield semantics — CHOICE A" WARNING | "in the parent (raw) release, `yield = cross_section_nb x L_data` ... DELIBERATELY BROKEN" |
| `metadata.json` | `born_definition` | "sigma_Born = C_rad x Unfold(data; R)" |
| `metadata.json` | `born_integrated_cross_sections_nb` | per-charge Born totals (nb) |
| `metadata.json` | `L_data` / `lumi_nb` | "351000.0 nb^-1" / 351000.0 |
| `metadata.json` | `column_scaling_policy.absolute_nb_columns_scaled` | lists `cross_section_nb` first |
| `metadata.json` | `caveat_verbatim`, `products_scaled`, `c_rad_table_definition` | Born-level provisional caveats; `sigma_proxy = sum(weight1*weight2)/L_MC` |
| `metadata.json` | `open_items` | "**multiplicities blocked: no DIS denominator**"; "absolute hadron cross-section normalisation chain remains provisional" |
| `summary_totals.json` | `lumi_nb`; `charges.{pos,neg}.{ibu,bbb}_total_nb`; `*_total_yield`; `raw_yield_vs_born_cross_section_warning`; `c_rad_applied`; `c_rad_table_key` | nb totals + the CHOICE-A warning text |
| `manifest.json` | `schema: manifest_v1`, `variant` | release-name labelling only |
| `SELECTION.md` | caveat block | "raw/no-bgsub, no C_rad, not Born-level ... multiplicities blocked (no DIS denominator)" (raw-parent wording) |
| `alternative_scheme_rapgap31_relbin_emfix/{ibu,bbb,summary_totals.json}` | same schema | RG31-emfix Born alternative |

### 2.2 Canonical Born plotters (both registry copies md5-identical; the ep observable's public face)

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (local snapshot copy: `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/...`)
  - **Y-axis label (lines 524-527, all 7 panels):** `$d^4\sigma^{h±}/dQ^2\,dx\,dz\,dp_{T,h}$ [nb/(GeV$^2$ GeV)]`
  - **Density construction `add_density()` (lines 391-405):** `vol = (q2_max-q2_min)(x_max-x_min)(z_max-z_min)(pT_max-pT_min)`; `density = cross_section_nb / vol`; `density_err = density × total_with_crad13_rel` (budget band, line 100-102: emfix 13-comp budget, documented deviation from native `total_unc_nb`). **NO x-cap on the volume** — the S3_x0 bookkeeping `x_max = 1.5` enters `dx`; only the display text is capped (lines 452-453). **[SUPERSEDED 2026-10-16 — P-XVOL: the plotter was patched on 2026-10-16; the density division now uses the capped width `dx = 1.0 − x_min = 0.979`; the internal container edge `x_max = 1.5` exists only to absorb migrations; it is not a published bin and there is no truth-level support above `x = 1`.]**
  - Provenance JSON keys: `crad_formula` ("sigma_corrected = sigma_uncorrected * C_rad"), `density` ("(cross_section_nb * C_rad) / (dQ2 * dx * dz * dpT_h)"), `lumi_nb`, `lumi_note`.
  - **`BORN_ROOT` (line 94) still points at the SUPERSEDED pre-S2' Born release `born_crad_dj14relbin_..._2026-10-09`** — the cone-basis ep render itself has NOT been regenerated (PARKED; plotter table-registration decision required).
  - C_rad table role: provenance/bin-alignment gate only (lines 336-377) — no multiplication (input is already Born).
- `.../make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py` — the derived `cRad_bgfracsub` twin (same label machinery, ylabel at line 590; separate derived variant, never merged into nominal; `RELEASE_DIR` at line 61 points at the coherent-prior raw release for its bgsub vector).

### 2.3 LaTeX cross-section table generator

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/tables/latex_cross_section_tables_2026-10-09/generate_latex_cross_section_tables.py` (v1.2.1) + `table_config.json` (+ `table_config_public.json` public-caption overlay)

- **Already ships TWO conventions:** `withZ` (full EW Born, factor column C_rad, values = Born release `cross_section_nb`) and `noZ` (`sigma_Born_gamma = C_gamma_born × sigma_raw`, photon-exchange-only **ep** Born, factor column C_gamma_born, cross-checked `= sigma_Born_withZ × F_EWgamma` — gates at lines 947-982). **This is the closest existing precedent for a per-bin-factor derived product.**
- Caption templates (`captions.withZ` / `captions.noZ`, internal + `captions.public.*`): "Born-level inclusive charged-hadron SIDIS cross sections ... WITH $\gamma^\ast/Z$ exchange"; "Photon-exchange-only Born-level ... WITHOUT $Z$ exchange"; public profile: "Cross sections for inclusive production of ... in deep-inelastic scattering".
- `sigma_form_sentence`: `binned` = "cross section in nb integrated over the corresponding four-dimensional bin"; `density` = "nb~GeV$^{-3}$" with `density.x_cap = 1.0` and a hard `bin_volume_consistency` gate that no volume uses the uncapped width (S3_x0 `x_max = 1.5`).
- Headers: `$\sigma$ (nb)` / stat / syst (nb) + both factor columns + bin-volume column; labels `tab:sidis-crosssections-{mode}-{charge}-{cell}`.
- Config pointers (currently one step behind the cone basis): `inputs.release_born.release_dir` = S2' Born `..._2026-10-10`; `inputs.release_raw.release_dir` = S2' raw `..._2026-10-09`; budget = 15-comp S2' budget; expected withZ integrals 13.064758/12.736523 nb (IBU). Output: `sidis_highQ2/data/tables/highQ2_z005_s2prime_born_public_2026-10-11b/` (+ sidecars `numbers_withZ.csv`, `numbers_noZ.csv`, which are themselves re-consumed as `inputs.previous_product` consistency references). Repointing at a new release is a **CONFIG EDIT ONLY** by design.

### 2.4 Analysis note

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/`

- `sections/observables.tex` (lines 8-12) — **the defining equation**: `dσ/dx dQ² dz dpT,h = N_Unf × c_rad / (L × Δx ΔQ² Δz ΔpT,h)`, `L = 351 pb⁻¹`. (Note: writes the differential form, while the release stores the binned nb form — the Δ-division happens downstream; also states phi_h/moments are defined in the γ*p CM frame, but the cross section itself is ep.)
- `sections/results.tex` (lines 48-50) — Eq. `d⁴σ/dQ² dx dz dpT,h = σ_bin/(ΔQ² Δx Δz ΔpT,h)` in nb GeV⁻³; data-side level statements (Born release + per-bin C_rad); "The highest-x bin remains capped with Δx = 1.0 − x_min" (line ~110); **known stale text around line 204 ("For the cross section the radiative correction is no...") and `sections/systematics.tex:25` — note update PENDING.**
- `sections/appendix_cross_section_tables.tex` — "cross sections for producing ... charged hadrons in **deep-inelastic electron–proton scattering**"; "Two table sets ... first retains the interference with the Z boson ... second gives the pure photon-exchange contribution. The two conventions must not be mixed"; inputs the generated blocks.
- `sections/radiative_corrections.tex` — C_rad definition and budget treatment.

### 2.5 Comparison-plotter labels (consumers that also label)

- `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py` — ylabels: `d⁴σ/dQ² dx dz dpT,h [nb GeV⁻³]` (lines 328, 480), `σ_bin [nb]` (line 332), `dσ/dpT,h [nb/GeV]` (line 330); manifest strings `data_cross_section_lumi_convention: release-stored luminosity (351000 nb^-1)` (lines 539, 557).
- `sidis_highQ2/scripts/plotting/bincenter_7panel_2026-10-15/bincenter_7panel.py` — title "Born $d^4\sigma$ charged-hadron SIDIS — cone+S2'" + the same d⁴σ ylabel (lines 44, 69).

---

## 3. CONSUMERS — everything that reads `cross_section_nb` / the release CSVs

**Primary (physics-product) consumers:**

1. **Canonical Born plotters** (§2.2, 2 scripts × 2 registry copies) — read `<BORN_ROOT>/ibu/cross_sections.csv`.
2. **Canonical data-vs-MC comparison plotter** `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py` (md5 `b7cb52997227febfe387682af30d3c15`; registry snapshot `.../canonical_highQ2_sidis_2026-06-30/MC/compare_data_vs_mc_z005_4generator_born.py`) — reads `<release-dir>/ibu/cross_sections.json` (`load_data_rows`, line 100); `--differentiate-4d` divides by `bin_volume_4d` **with the highest-x bin capped at x=1.0** (lines 79, 153, 537-538); `--lumi-pb 351.0`; validated current instance: `sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/` whose data side since 2026-10-13 is the **NoZ photon-Born sidecar** `data_side_noz_photonborn/`.
3. **5-prediction package builders** (package-local, `.../data_vs_mc_z005_5generator_born_releasebin_2026-10-13/`):
   - `build_data_side_noz_photonborn_2026-10-13.py` — **THE derived-sidecar precedent**: `sigma_photonBorn(bin) = C_gamma_born(bin) × sigma_raw(bin)`; scales `cross_section_nb` + every `*_unc_nb` (+ `total_unc_nb_diag`), replaces `caveat`, everything else byte-identical to RAW; integrity + total gates.
   - `build_nonrad_truth_predictions_releasebin_2026-10-13.py` (MC NORAD curves), `build_maptmd_predictions_2026-10-13.py` (MAPTMD24 provider CSV → bin-integrated with `dx = 1.0 − x_min` cap), `make_finalcandidate_7panel_and_xQ2_maptmd_overlay_2026-10-13.py` (canonical-style 7-panel + DATA/MAPTMD24 ratio; `dx_cap_rule: when x_max > 1, use dx = 1.0 - x_min`; data side = photon-Born sidecar JSON; MAPTMD24 is photon-level, 230/490 coverage).
4. **Systematics budget chain** — `H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_shiftedlegs_2026-10-14/run_cone_systematics_envelope_successor_env.py` (+ `run_bbb_ibu_shiftedlegs_stage3.py`, `run_bbb_ibu_signsym_minus_stage3.py`, `run_cone_systematics_rebase_shiftedlegs_stage3.py`); budget CSV `.../systematics_cone_s2prime_15comp_shiftedlegs_legenv_..._2026-10-14/total_uncertainty_summary_cone_variant_15comp.csv` carries `nominal_cross_section_nb` (raw-level ep) and absolute-nb columns (`tracking_eff_m0p5_abs_nb`); **all 15 uncertainty components are RELATIVE** — a per-bin flux factor cancels in every one of them.
5. **LaTeX table generator** (§2.3, also a consumer): gates `identity_born_eq_raw_times_crad` (Born σ = raw σ × C_rad, line 947), `budget_internal_consistency` (budget `nominal_cross_section_nb` == raw parent σ, tol 1e-9 IBU), noZ construction from raw σ.
6. **Charge asymmetry** — `sidis_highQ2/scripts/charge_asymmetry_1d/make_charge_asymmetry_1d.py` (lines 93, 108: per-bin `cross_section_nb/yield_unfolded` factor; integrated `sum(cross_section_nb)` per charge) and `make_charge_asymmetry_pt_in_z.py`. A flux division cancels exactly in (h+−h−)/(h++h−) (Γ depends only on DIS kinematics, not hadron charge).
7. **Bincentering (newest consumers)** — `sidis_highQ2/scripts/bincentering/bincenter_averages_2026-10-14/bincenter_averages.py` (reads BOTH the cone raw and cone Born `ibu/cross_sections.csv`, lines 171-180; parse helper lines 1108-1145) and `sidis_highQ2/scripts/plotting/bincenter_7panel_2026-10-15/bincenter_7panel.py` (cone Born CSV, line 14; density line 33).

**Secondary/diagnostic consumers (ep-σ reading, not physics-release critical):** ptmax scan study (`H1ChargedSidis_HighQ2/scripts/studies/ptmax_scan_z005_2026-10-08/*`), TUnfold cross-check chains (`tunfold_s2prime_z005_v3/v4/v5_*`, `tunfold_official_v6_native_miss_z005/*`), bgsub-setr builders (`sidis_highQ2/scripts/systematics/bgsub_setr_build_2026-10-10/*`), `signal_vs_noise_2026-10-03`, `smoothing_s2m*`, `background_candidates_comparison_2026-10-03`, crad-envelope derived-table scripts (`sidis_highQ2/scripts/cross_section/finalcandidate_crad_envelope_2026-10-02/*`), superseded `compare_data_vs_mc_z005.py` + `*_xfpos_*` variants, flatphi study plotters, driver validators (`validate_driver_clone_cone_s2prime.py:473`, `postcheck_release_cone_s2prime.py:72` — negative-σ checks).

**Explicit NON-consumers (checked):**
- **phi_h moments / AziAsym forward-fold** (`sidis_highQ2/scripts/forwardfold_phih_harmonic_v2_cache_2026-06-29/`): yield/acceptance-template-level fits; no `cross_section_nb` normalization involved (moments normalize per bin, so a flux factor would cancel anyway).
- **BSA side study** (`v6bsa` fork): acceptance-free raw asymmetries.
- **Multiplicity / DIS-denominator workflow: DOES NOT EXIST — multiplicities are BLOCKED** (no DIS denominator; `metadata.json open_items`, `SELECTION.md`). The gamma*p flux does not create a DIS denominator.
- **HEPData-style exports: none found** anywhere in either workspace (grep for "hepdata" over all script/doc trees: zero hits). The LaTeX sidecars (`numbers_withZ.csv` / `numbers_noZ.csv`, roundtrip-precision) are the only export-shaped artifacts.

---

## 4. COVARIANCE — units verdict

**Files (Born release):**
- `.../born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/covariance/ibu_toy_covariance_pos.json`
- `.../covariance/ibu_toy_covariance_neg.json`
- (+ dimensionless twins `ibu_toy_correlation_{pos,neg}.json`, and pdf/png visualisations)

**Verdict: YIELD units, not cross-section units.** Each JSON carries (QA C4b keys, added inside the file; matrix content bit-identical to the `stat_sidecar_1000toy` source, cluster 5550195):
- `"units": "unfolded_yield_counts"`
- `"unit_warning"`: "Elements of 'covariance' ... are in UNFOLDED-YIELD units (raw data-level counts), NOT cross-section units. This file is NOT Born-level: no C_rad and no luminosity conversion has been applied"
- `"born_covariance_recipe"`: `Cov_Born[i][j] = C_i * D_ij * C_j` with `C_i = C_rad(bin_i)/L_data` (charge-local bin_index)
- `"correlation_note"`: correlation dimensionless, unchanged by the Born transform.

The raw parent release ships the same yield-unit 1000-toy matrices (`covariance/`, `stat_sidecar_1000toy/`).

**Consequence for gamma*p:** a per-bin multiplicative 1/Γ rescale is a **trivial diagonal congruence** — `Cov_gammap[i][j] = (1/Γ_i) · Cov_ep[i][j] · (1/Γ_j)`, or in one step from the sidecar `C'_i = C_rad(bin_i)/(L_data · Γ_i)`. **No toy re-derivation, no re-unfolding, no re-solve needed; the correlation matrix is unchanged.** The derived product merely needs to document the extended recipe (exactly the `born_covariance_recipe` pattern). The 13-comp crad13 budget's `covariance_cradstat/` rank-1 + diagonal sidecars are RELATIVE-unit and carry over unchanged.

---

## 5. MINIMAL-CHANGE ASSESSMENT — injection point for the flux factor

### Option A (rejected): edit the release driver in place
Adding `cross_section_gammap = vec/(LUMI*Gamma)` inside `run_bbb_ibu_release_cone_s2prime.py` or the Born builder would (i) modify protected installed releases or force a full re-release, (ii) entangle a QED-flux division with the QED C_rad step inside one product, (iii) break the exact-text/bitwise inheritance gates (`canonical_arithmetic_diff`, `crad_table_inheritance`, CHOICE-A gates) that make the current chain auditable, and (iv) create yet another level flag inside the *same* files rather than a new product. Against the workspace rule "do not move, rename, or overwrite the installed release" and the precedent that every level change so far (raw → Born → noZ) was a **separate dated derived product**.

### Option B (recommended): sidecar / derived-release, per the noZ precedent
Two precedents already implement exactly the needed machinery:
1. `build_data_side_noz_photonborn_2026-10-13.py` — derived sidecar: per-bin factor × raw σ, all `*_nb` columns scaled, caveat replaced, rest byte-identical, gates on totals.
2. `build_born_candidate_cone_s2prime.py` — exact-text (`float_precision='round_trip'`) column scaling, per-bin factor provenance CSVs, validation.json gate suite.

**Recommended design:**
1. **Freeze a per-bin flux-factor table FIRST**, as an independent dated artifact, e.g. `sidis_highQ2/data/flux/hand_gammap_flux_releasebin_<date>/gamma_flux_<convention>_490rows.csv` with `1/Gamma(bin)` (or Gamma and divide in the builder), its sha256, integrated values per charge, and recorded conventions (Hand flux variant, R = σ_L/σ_T treatment, y convention). The MC campaign machinery already produces y-resolved per-bin factor grids — see `sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` (y_index/y_min/y_max columns; 6 y bins × 490) — so an MC-integrated Γ per (release bin) is a natural campaign product; an analytic bin-center-y table is the cheaper fallback (see risk R4).
2. **New standalone builder** in a dated category dir (e.g. `sidis_highQ2/scripts/cross_section/gammap_derived_<date>/build_gammap_derived.py`) that reads the chosen Born release `ibu/cross_sections.csv` (+ `bbb/`, + raw parent for the identity gate) **read-only**, scales the 8 absolute-nb columns by 1/Γ(bin) under the round_trip exact-text convention, replaces `caveat`, keeps yields byte-identical, and writes a **NEW derived directory outside `output_v6/central/data_release/`** with `metadata.json` keys: `level: "gamma*p reduced (derived)"`, `parent_release` + sha, `flux_table_path/sha256`, `flux_convention: "Hand"`, `R_treatment`, `y_convention`, `born_covariance_recipe` extension (`C_i = C_rad(bin_i)/(L_data·Γ_i)`), and a `raw_yield_vs_gammap_cross_section_warning` (CHOICE-A analogue). Alternative-scheme handling mirrors the release (RG31-emfix parent if a scheme spread is wanted).
3. **Pros:** ep release untouched; explicit level flag; reusable gates; relative systematics and correlation carry unchanged; covariance is a documented diagonal congruence; the LaTeX generator takes a gamma*p mode as a **CONFIG EDIT ONLY**; the comparison plotter accepts the derived dir via `--release-dir`; MAPTMD24 (photon-level) finally gets a level-consistent data side.
   **Cons:** one more level in the quote-rule space (see R1); a NEW provenance surface (the flux table needs its own exposure/convention discipline — the C_rad emfix saga is the cautionary tale); plotter/table regeneration each need a recorded registration decision; picking the parent basis is unavoidable (see R2).

### What must be regenerated (and what need not)
| Product | Regeneration | Note |
|---|---|---|
| 7-panel/x-Q2 gamma*p plots | NEW render (new plotter instance or recorded repoint of the canonical plotter) | the live canonical plotter still reads the 2026-10-09 pre-cone Born release; the ep cone render itself is PARKED — decide the parent basis first |
| LaTeX tables | config edit + new mode/factor column + re-derived expected integrals | generator is config-driven by contract; `inputs.previous_product` consistency gate needs the new sidecars registered |
| data-vs-MC comparison package | rerun with `--release-dir` at the derived dir + level statement | MAPTMD24 overlay is the natural first consumer |
| Systematics budget | **NOT needed for relative totals** (flux cancels); optional derived sidecar only if absolute-nb budget columns (`nominal_cross_section_nb`, `tracking_eff_m0p5_abs_nb`) are wanted in gamma*p units | |
| Covariance | no toy rerun; document the extended recipe | §4 |
| Charge asymmetry | none (flux cancels) — but label if quoting gamma*p | |
| Release tables / response / unfolding | **NOT touched** | the entire point |

---

## 6. RISKS — concrete failure modes specific to this chain

- **R1 — Level-flag collision (the quote-rule extension).** The chain already enforces prior-basis + Born-vs-raw + observable-basis (three-basis rule). gamma*p adds a fourth axis with THREE easily-confused neighbours: Born-ep **withZ** (13.03/12.70 nb), noZ photon-only **ep** Born (11.91/11.61 nb — still an ep cross section, only the Z exchange removed), and gamma*p **reduced** (÷Γ, a different observable with different units convention). Quoting a gamma*p number without the flux convention (Hand variant), R treatment, and parent-basis flags is exactly the ambiguity class that is already FORBIDDEN for Born-vs-raw. The analysis-note appendix's "two conventions must not be mixed" sentence must become three.
- **R2 — Parent-basis fragmentation.** The live consumers currently read FOUR different bases: canonical plotter → 2026-10-09 releasebin Born (pre-S2'); LaTeX config → 2026-10-10 S2' Born/raw; comparison package → NoZ sidecar on S2' raw; current release → 2026-10-13 cone Born. A gamma*p product must declare exactly one parent (recommend the current cone Born release) and every regenerated plot/table must state it; otherwise cross-basis quotes (FORBIDDEN) and inconsistent integrals.
- **R3 — C_rad/flux interaction and double-counting.** `C_rad = σ_nonrad/σ_rad` is an **ep-level QED ratio** — it contains no flux division, so applying 1/Γ after C_rad is order-consistent (both per-bin multiplicative; they commute) and is NOT a double count. BUT: (i) the C_rad convention caveats carry into gamma*p — mixed-beam only ("not valid for any per-beam or e- Born target"), xFGen<0, EMPZ>45, releasebin binning, P-CRAD-LUMI still not publication-grade; (ii) the NoZ `C_gamma_born` table is NOT a flux — conflating "photon-exchange-only ep" with "gamma*p reduced" would under/over-correct by the full flux factor; (iii) if Γ is derived from the same MC legs as C_rad, exposure-provenance defects (the emfix e- exposure repair shifted the RG31 table by ~0.5%) leak directly into the flux — the flux table needs independent provenance and its own validation gates.
- **R4 — No y in the release tables.** Γ(Q²,x,y) needs y; `cross_sections.csv` and `bin_map.csv` carry only Q²/x/z/pT edges (selection 0.1<y<0.7). A y convention must be frozen: analytic bin-center y = Q²_center/(x_center·s) vs MC-gen-level y-integrated Γ per bin. These differ at the %-level in high-Q² low-x bins (the y-resolved 2940-row campaign table exists precisely because per-bin y structure matters). An unstated y convention is a silent systematic.
- **R5 — R = σ_L/σ_T dependence.** The Hand-convention flux (and any Γ variant: Hand vs Bertone–Guichaoua vs MVFG; R=0 vs (1+R) forms) directly rescales the gamma*p central values. R is not measured in this analysis; the choice must be a declared convention, ideally carried as an explicit uncertainty component (mirroring how `c_rad_gen_rel` carries the DJ14/RG31 spread).
- **R6 — x_max = 1.5 bookkeeping (density convention split) — RESOLVED 2026-10-16 (P-XVOL).** The former canonical-plotter division by the uncapped volume (dx = 1.5 − x_min for S3_x0) was ruled a defect and patched on 2026-10-16: the internal container edge `x_max = 1.5` exists only to absorb migrations; it is not a published bin and there is no truth-level support above `x = 1`. The published top-x cell is `S3_x0: 0.021 <= x < 1.0`; every density (canonical plotter, comparison plotter, MAPTMD overlay, LaTeX density form) must use `dx = 1.0 − x_min = 0.979` for S3_x0. A gamma*p density render must use the same published cap; the binned (nb) form sidesteps it entirely (recommend binned for the derived tables, as the current published tables do).
- **R7 — CHOICE-A broken yield relation extends.** In the Born release `yield ≠ cross_section_nb × L_data`; in a gamma*p derived release `yield ≠ cross_section_nb × L_data × Γ` either. Replicate the `raw_yield_vs_*_warning` key; any consumer back-computing "yields" from gamma*p cross sections is triply wrong (C_rad × Γ × L).
- **R8 — Absolute-normalization provisional status.** `metadata.json open_items`: "absolute hadron cross-section normalisation chain remains provisional" — the gamma*p product inherits this (and the raw/no-bgsub status with response fake-fraction subtraction only); it must not be presented as more final than its ep parent.
- **R9 — Multiplicities remain blocked.** A gamma*p reduced cross section is NOT a step toward multiplicities (no DIS denominator exists or is created); do not present it as one.

---

## 7. Quick-reference: the 5 most important paths

1. `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_2026-10-11/run_bbb_ibu_release_cone_s2prime.py` — the yield→σ producer (LUMI=351000.0 line 195; `cross_section_nb = vec[i]/LUMI` line 628).
2. `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` — the Born ep release to be divided (ibu/bbb CSVs, covariance yield-unit sidecars, metadata/summary label keys).
3. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/tables/latex_cross_section_tables_2026-10-09/generate_latex_cross_section_tables.py` (+ `table_config.json`) — the table product with the existing withZ/noZ derived-factor precedent (config-edit repointing).
4. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/build_data_side_noz_photonborn_2026-10-13.py` — the derived-sidecar pattern to copy for gamma*p.
5. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/observables.tex` (+ `sections/results.tex`, `sections/appendix_cross_section_tables.tex`) — the observable definitions and level-convention text that a gamma*p presentation extends.

*Inventory produced read-only; no files outside this report were modified.*

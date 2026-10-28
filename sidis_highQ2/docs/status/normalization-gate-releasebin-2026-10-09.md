# Normalization Gate — releasebin C_rad promotion (2026-10-09)

- **Gate:** luminosity / exposure / normalization semantics of the releasebin C_rad candidate set (read-only verification).
- **Gating authority:** `h1-normalization` (skills loaded: `sidis-hq2-normalization`, `sidis-hq2-radiative`; `STATE.md` read first per AGENTS.md).
- **Date:** 2026-10-09.
- **Scope (candidate set, all read-only):**
  - `sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/` (DJANGOH14 releasebin table + provenance)
  - `sidis_highQ2/data/radiative/rapgap_crad_releasebin_2026-10-09/` (RAPGAP31 releasebin table + provenance)
  - `H1ChargedSidis_HighQ2/output_v6/central/data_release_candidates/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/` (Born candidate)
  - `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/` (budget candidate)
  - Old xFcut campaigns `djangoh14_crad_final_xfcut_2026-10-05/`, `rapgap_crad_final_xfcut_2026-10-05/` (comparison baseline)

## VERDICT: **PASS WITH CAVEAT**

All five checks pass. No normalization defect found. The caveats are (1) a per-file exposure-manifest provenance regression in the new campaign directories and (2) carried-open exposure-provenance blockers that travel with any acceptance. This gate covers normalization semantics only; it is not the baseline review and not publication clearance.

## Caveat list

1. **Per-file exposure manifest regression (provenance completeness, non-blocking).** The old xFcut campaigns ship a 735-line per-file `input_output_exposure_manifest.csv` (per-input HAT/MODS paths, exposure columns, `radmode`). The new DJANGOH14 releasebin dir carries only a 1-line stub (`release-safe rederivation 2026-10-09`, likewise `manifest.txt`), and the RAPGAP31 releasebin dir has no per-file exposure manifest at all. The per-leg totals are correct and recorded in four places per campaign (`crad_*_releasebin.json`, `campaign_manifest.json`, `crad_v3_summary.json` (DJ), `finalize.json` (RG)) and are identical to the old campaign; the per-file record now lives only in the 2026-10-05 campaign dirs. Recommendation: if the releasebin tables become the long-term applied basis, copy or reference the 2026-10-05 per-file manifest from the releasebin provenance.
2. **Carried-open exposure/normalization blockers (inherited, unchanged; the candidates correctly do NOT claim closure).** `P-CRAD-LUMI` remains `DIAGNOSTIC_MANIFEST_LEVEL` (C_rad not publication-grade); the releasebin baseline is PROVISIONAL pending the promotion review (this gate is one input, not the review); the EMPZ>45 convention decision stays open (a future flip supersedes the applied table); the DJANGOH e- RAD `4.67%` count shortfall is bounded but unresolved; the correction is mixed-beam only; the release stays raw/no-bgsub with response fake-fraction subtraction only (`10.7141%` h+ / `10.4019%` h-). Absolute normalization of any Born quote inherits the manifest-level exposure uncertainty until `P-CRAD-LUMI` closes.

### Non-caveat observations (no action required)

- Campaign-level `c_rad_applied: false` flags in `crad_v3_summary.json` / worker config describe table-production-time status (same convention as the accepted 2026-10-07 promotion, whose xFcut campaign summary also says `false`); the application status `c_rad_applied: true` correctly lives in the Born candidate metadata/summary.
- The prebeam low-y closure control `sigma_rad` moved by `5.3e-7` (ep) / `1.3e-7` (em) relative vs the old campaign; `sigma_nonrad` is bit-identical. This is the documented release-safe grid-boundary population effect (F1: 164,038 RAD enter/leave-grid transitions = `0.03971%` of selected RAD rows; combined NONRAD only 11), i.e. a consequence of the binning-variable change, not a normalization change.
- Budget absolute-nb diagnostic columns (`nominal_cross_section_nb`, `tracking_eff_m0p5_*_abs_nb` anchors) are raw-level carry-overs inherited byte-identical from the accepted 2026-10-07 crad13 budget structure; the budget is relative (all 13 components `_rel`) and its totals do not depend on them.

## Check results

### 1. C_rad ratio semantics and per-leg luminosity bookkeeping — **PASS**

- Definition confirmed in code and metadata: `C_rad = sigma_nonrad/sigma_rad`, `sigma_proxy = sum(weight1*weight2)/L_MC` per leg. Worker line 118: `w = weight1*weight2` (finite-checked); DJ finalizer: `sn += BEAM[b]*nonrad_sumw/NONRAD_lumi`, `sr += BEAM[b]*rad_sumw/RAD_lumi`; RG finalizer: `sr += BEAM[b]*rad_sw/Lr`, `sn += BEAM[b]*non_sw/Ln`.
- **`L_data` does NOT enter the ratio.** No `L_data`/`351000` token anywhere in the campaign scripts; only per-leg MC luminosities and the dimensionless beam mixture `0.5140930272/0.4859069728` (identical to old campaign) enter.
- Worker diff vs the frozen xFcut worker (`crad_v3_worker_xfcut.py`): exactly one semantic change — `bq,bx = Q2esGen,XesGen -> Q2esGen,x_es_gen` — plus docstring wording and the `binning` metadata string (`framework_esigma -> release_safe_Q2esGen_x_es_gen`). All selection config (DIS gate, RAD-only `Q2Gki>100`, safe-xF current-hemisphere cut, hadron pool, V0/Mother2 vetoes, z/pT, y booking, weights, npz format) identical. The two generators' releasebin workers are byte-identical to each other. Matches `F1_CROSSCHECK_ROOT_CAUSE_2026-10-09.md` branch A.
- **Per-leg luminosities old vs new: IDENTICAL (zero differences), field `luminosity_nb_inv`:**
  - DJANGOH14: RAD ep `16206393.0` / em `13534017.0`; nonrad_old ep `12920115.855` / em `12841572.442`; nonrad_new ep `1717376.821` / em `1699702.771`; nonrad_combined ep `14637492.676` / em `14541275.213` nb^-1.
  - RAPGAP31: RAD ep `17536474.0` / em `14461067.0`; NONRAD ep `13954768.0` / em `13416991.0` nb^-1 (old campaign: same values as `nonrad_old`=`nonrad_combined`; `nonrad_new` was 0.0 — same convention).
  - Beam mixture and `DIAGNOSTIC_MANIFEST_LEVEL` exposure label identical.
- Input counts identical to the old campaign: DJ rad `{731 ep, 642 em}`, nonrad_old `{260,260}`, nonrad_new `{530,530}` (2953 histogrammed; 1200 reused + 1753 fresh partials); RG rad `{691, 582}`, nonrad `{256, 248}` (1777; 16 degenerate C0000032 chunk trees excluded exactly as before; 1054 reused + 723 fresh).
- Integrated C_rad shifts are the expected binning-boundary effect, not normalization changes: DJ h+ `1.1418534312056339 -> 1.1415172920054786` (`-0.0294%`), h- `1.1413755529189475 -> 1.1410367377289135` (`-0.0297%`); RG h+ `1.1368044552974204 -> 1.1364508941898332` (`-0.0311%`), h- `1.1362373358660744 -> 1.1358814928541603` (`-0.0313%`). DJ shifts match the F1-documented formal-vs-common-support integrated difference (`-0.02945%`/`-0.02951%`). Cross-generator spread of the new pair: `+0.4458%` (h+) / `+0.4539%` (h-) (old pair `+0.4441%`/`+0.4522%`).
- Independent recomputation from the shipped tables reproduces the documented integrated values exactly (rel diff `0.0` for all four): `sum(sigma_nonrad)/sum(sigma_rad)` per charge.
- `490/490` finite C_rad in both tables; `relative_stat_unc` and `rad_n`/`nonrad_n` columns present.

### 2. `L_data = 351000.0 nb^-1` consistency — **PASS**

- Born candidate `metadata.json`: `"L_data": "351000.0 nb^-1"`, `"lumi_nb": 351000.0`; `summary_totals.json` `lumi_nb: 351000.0` (primary and alternative-scheme subproduct both); raw parent `lumi_nb: 351000.0`; parent `metadata.json` `lumi_nb: 351000.0`.
- Budget candidate: relative-only product; `lumi_rel = 0.027` (2.7% flat) carried byte-identical; `L_data` does not enter any budget arithmetic. No rescaling of relative products anywhere in the candidate set (regime-B invariance held).
- `lumi_rel` and `c_rad_stat_rel` remain disjoint; `L_data` remains outside the C_rad ratio (verified in check 1).

### 3. Born candidate absolute-nb scaling — **PASS**

- Exactly the 8 nb columns scaled: `cross_section_nb, stat_unc_nb, analytic_stat_unc_nb, model_unc_nb, reg_unc_nb, toy_stat_unc_nb, total_unc_nb, total_unc_nb_diag` (canonical plotter's seven `*_nb` + `total_unc_nb_diag`, an absolute-nb uncertainty outside the plotter's suffix scope, scaled for Born self-consistency with documented rationale).
- **Independent verification (this gate, not the builder's own):** all `490 rows x 8 columns x {ibu, bbb}` = 7840 scaled cells satisfy `float(candidate) == float(parent) x float(C_rad(bin))` **bitwise** — 0 mismatches, worst relative deviation `0.0`. The `bin_index % 245` charge-local remap is thereby validated (a wrong remap could not produce bitwise products since C_rad varies per bin).
- **All unscaled columns byte-identical to the parent CSV text** (my full-column text comparison: 0 differing cells in every non-nb, non-caveat column, both ibu and bbb), including all `*_yield` columns, `total_unc_rel`, `total_unc_yield(_diag)`, `acceptance_rapgap`, `prior_gen_rapgap`, kinematic edges and all string columns. The `caveat` column is the single intentional change (490/490 rows, documented QA C3).
- Yields unchanged (YIELD POLICY CHOICE A): integrated yields identical to parent — ibu `4030663.5889414307`/`3927824.3328629597`, bbb `4035219.256343863`/`3932166.601754886`; the deliberately broken `yield = sigma_Born x L_data` relation is documented in `metadata.json` and the `raw_yield_vs_born_cross_section_warning` field of `summary_totals.json`.
- **Parent untouched:** integrated IBU verified independently by summing the parent CSV: h+ `11.483372048266192` nb / h- `11.190382714709282` nb (canonical values `11.483372`/`11.190383`; `summary_totals.json` `11.483372048266185`/`11.190382714709287`). Parent `cross_sections.csv` mtimes (2026-10-06/07) predate the campaign and the 2026-10-08T23:22Z candidate build; only doc/metadata files show earlier Oct-8 daytime touches (before the build), with sound content (`lumi_nb 351000.0`, `prior_basis body+miss`).
- Applied-table identity: sha256 verified on disk — DJANGOH14 `cf810be44a59ed790ba78e0476eb8f59df96358357cc39e258e034ed31b22ef5`, RAPGAP31 `3860a1005c4394e88ba9d152a1f6e513a9fb1ef03204e59916831e007f10d403` — both match the candidate metadata and budget provenance claims.
- Born integrated cross sections (ibu): h+ `13.107258377622092` / h- `12.77278197828459` nb (measured Born/raw scale `1.1414119757272072`/`1.1414070728336494`); bbb `13.089458939808843`/`12.759120412771287` (`1.1385750800653374`/`1.1389271407991803`). Alternative-scheme subproduct (RAPGAP31_relbin) verified: `lumi_nb 351000.0`, yields parent-identical, ibu totals `13.040167304170634`/`12.703023492730678`.
- Builder validation (`validation/validation.json`, rev 4) independently consistent: `canonical_arithmetic_diff` 0.0 on 3430 cells x 4 tables; unscaled byte-identity; CSV/JSON agreement 0 disagreeing cells; parse-convention residue vs the plotter's pandas-default universe `<= 9.72e-13` relative (documented residue, not an arithmetic difference).

### 4. Exposure provenance status and forbidden-lumi traps — **PASS**

- **No P-CRAD-LUMI closure claim.** Born candidate `caveat_verbatim` and `open_items` retain "exposure provenance DIAGNOSTIC_MANIFEST_LEVEL, not publication-grade"; candidate README: "NOT canonical and not promoted", no supersession of the installed 2026-10-07 Born release declared. DJ campaign `crad_v3_summary.json` retains `exposure_label: DIAGNOSTIC_MANIFEST_LEVEL`. Budget README: "C_rad MC-stat exposure provenance remains DIAGNOSTIC_MANIFEST_LEVEL (P-CRAD-LUMI open), inherited unchanged from the rederivation campaign; the binning fix does not change exposure accounting"; budget status is CANDIDATE until the promotion is accepted.
- **No DjBkgd/7091/7110 traps.** Semantic sweep of the full candidate set (json/md/py/tsv/txt/sh/sub): zero references to `merged_7091.root`, `djangoh14/7091`, `7110`, DjBkgd `15813.4 pb^-1`, `4379935.0`, scales `0.08013890218154498` / `22.2017` / `0.0221963651`, or superseded data lumis `297657.8` / `356613` / `126880`. (Residual grep hits are coincidental digit substrings inside covariance float payloads and CSV noise, e.g. `0.022195863...`, `0.080136532...`; the covariance matrices are inherited yield-unit parent products, `products_not_scaled`.) The C_rad legs are DJANGOH14 RAD/NONRAD and RAPGAP31 RAD/NONRAD only.

### 5. Unit sanity (fLumi pb-vs-nb conventions) — **PASS**

- New provenance luminosities live in the field `luminosity_nb_inv` (explicit nb^-1) — same field name, same values, same oomclumi-derived origin ("Exposures verbatim from the FINAL (blind) DJANGOH14 finalizer" — hardcoded identically in the new DJ finalizer; RG values CLI-passed and recorded identically in `finalize.json`/`campaign_manifest.json`/`crad_rapgap31_releasebin.json`) as the old xFcut campaign. Units consistent with the old campaign by construction (values identical).
- Magnitudes are oomclumi-scale MC exposures: `1.3e7-1.8e7 nb^-1` per leg (13k-18k pb^-1); a pb^-1 misread would be 1000x off. The 2026-09-25 fLumi trap (`H1SteerTree::fLumi` is pb^-1) is not in play: zero `fLumi`/`SetMCLumi`/`fFileLumis` usage in the campaign scripts; exposures come from oomclumi manifest accounting, not steering `fLumi`.
- The ratio is per-leg `L_MC`-normalized with identical units on both legs, so `C_rad` is dimensionless and unit-consistent; the beam-mixture weights are dimensionless fractions.

## Budget candidate (supporting verification)

- 13 components, 490 rows; `c_rad_gen_rel` recomputed from the releasebin pair (medians h+ `0.5505%` / h- `0.5166%`; was `0.5298%`/`0.5009%`); `c_rad_stat_rel` = applied DJ releasebin table's `relative_stat_unc` (medians h+ `0.4272%` / h- `0.4429%`; was `0.4272%`/`0.4480%`; written values match the table with max dev `0.0`).
- 29 carried columns byte-identical to the 2026-10-07 crad13 budget (verified independently: only the crad13-derived total columns differ, as expected); quadrature closure exact on the written CSV; totals h+ median `5.712%` (was `5.713%`), h- `6.145%` (was `6.150%`); deltas `-0.0004`/`-0.0057` pp; per-bin max |delta| `0.0642`/`0.0455` pp.
- Central products, canonical scripts, existing budgets and C_rad tables untouched (`central_products_modified: false`).

## Blockers (carried OPEN; this gate does not close them)

1. `P-CRAD-LUMI` exposure provenance `DIAGNOSTIC_MANIFEST_LEVEL` — C_rad NOT publication-grade.
2. Releasebin baseline PROVISIONAL — promotion review (of which this gate is one input) is not publication clearance.
3. EMPZ>45 convention decision open — a future flip supersedes the applied table.
4. DJANGOH e- RAD `4.67%` count shortfall (bounded, unresolved).
5. Raw/no-bgsub release with response fake-fraction subtraction only (`10.7141%` h+ / `10.4019%` h-).
6. Per-bin definition sensitivity in the z<0.05 row; mixed-beam-only validity; no Born-vs-RAD-level-MC comparison without the same correction; `cRad_bgfracsub` stays a separate variant.
7. `P-PLOTS`, `P-ALPHA`, `P-METHOD` open, unchanged.

## Evidence

- Campaign provenance: `crad_djangoh14_releasebin.json`, `crad_rapgap31_releasebin.json`, both `campaign_manifest.json`, DJ `crad_v3_summary.json`, RG `finalize.json`, `F1_CROSSCHECK_ROOT_CAUSE_2026-10-09.md`, `scripts/{crad_v3_worker_releasebin.py, finalize_releasebin.py}` (both campaign dirs).
- Old-campaign comparison: `djangoh14_crad_final_xfcut_2026-10-05/{crad_djangoh14_xfcut.json, crad_v3_summary.json, input_output_exposure_manifest.csv}`, `rapgap_crad_final_xfcut_2026-10-05/{crad_rapgap31_xfcut.json, crad_v3_summary.json}`.
- Born candidate: `metadata.json`, `summary_totals.json`, `validation/validation.json`, `README.md`, `status.json`, `alternative_scheme_rapgap31_relbin/summary_totals.json`; raw parent `bbb_ibu_coherentprior_..._2026-10-06/{summary_totals.json, metadata.json, ibu/cross_sections.csv, bbb/cross_sections.csv}`.
- Budget candidate: `provenance.json`, `README.md`, `validation_checks.json`, `crad_overlap_audit.json`, `total_uncertainty_summary_with_phih_coherentprior_b1floor_crad13_releasebin.csv`.
- Independent verification scripts (this gate): `/tmp/opencode/normgate_check_born.py`, `/tmp/opencode/normgate_check_born2.py` (read-only; bitwise product check, full-column byte-identity check, table sha256, integrated-value recomputation).

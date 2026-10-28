# Loop 1 Plotting-Script Audit — H1 High-Q2 Inclusive Charged-Hadron SIDIS (v6/z005)

**Date:** 2026-10-19
**Critic loop:** 1 of 3 (plotting scripts), read-only line-by-line code audit
**Current FINAL basis at audit time:** cone+S2'+Set-R' background-subtracted clean release, installed 2026-10-17 (`INSTALL_RECORD_bgsub_cleanrelease_2026-10-17.json`)

## VERDICT

**FATAL (audit FAIL).** The registered canonical Born 7-panel/x-Q2 plotter still renders the superseded raw/no-bgsub cone basis (2026-10-13 Born release) with the superseded `total_with_cone15_rel` budget (2026-10-14), while the declared final basis is the 2026-10-17 clean release with `total_with_bgsub19_rel` as the ONLY quotable total. Separately, the registered bgsub-diagnostic variant plotter contains a demonstrated bin-width normalization defect (missing highest-x cap) that understates every S3_x0 panel point by a factor 0.979/1.479 ≈ 0.662 (−33.8%) whenever it runs. No script in the audited set can currently produce final-basis (2026-10-17) plotting products.

## Material reviewed

1. `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (1008 lines, read in full) — registered canonical Born 7-panel/x-Q2 plotter.
2. `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py` (612 lines, read in full) — canonical data-vs-MC comparison plotter.
3. `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_bgsub_lowq2_djangoh.py` (755 lines, read in full) — plot-time bg-subtracted diagnostic variant.
4. `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/plotting/regen_minimal_7col_plots.py` (534 lines) and `plotting/plot_perbin_7col.py` (411 lines) — both present, read in full.

Supporting evidence: `INSTALL_RECORD_bgsub_cleanrelease_2026-10-17.json` (basis_statement, public_quoting_rule, installed_products); `sidis_highQ2/docs/RUN_STATUS.md` (2026-10-17 update); release-tree listings under `output_v6/central/data_release/`; cone Born release `ibu/cross_sections.csv` (S3_x0 row: x_min=0.021, x_max=1.5); 15comp shiftedlegs and 19comp cleandenom budget CSV headers; clean-release `ibu/cross_sections.json` schema (lumi_nb 351000.0; rows contain charge/bin_index/global_bin_index/cross_section_nb/stat_unc_nb/total_unc_nb/supported); the June-2026 reco-backgrounds study summary JSON and its data-vector JSON.

---

## CRITICAL

### C1. Registered canonical Born plotter reads the superseded basis and the superseded uncertainty budget
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:106-114`
- **Quote:** `BORN_ROOT = ".../born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13"`; `BUDGET_DIR = ".../systematics_cone_s2prime_15comp_shiftedlegs_..._2026-10-14"`; `BUDGET_COLUMN = "total_with_cone15_rel"`
- **Reasoning:** The 2026-10-17 install record declares the FINAL canonical basis = cone+S2'+Set-R' background-subtracted clean release and states: "Public quoting = the FINAL subtracted release (this install) + `total_with_bgsub19_rel` ONLY ... all prior totals (`total_with_cone15_rel` and predecessors) are superseded as quoted numbers". The current Born-level release is `born_crad_bgsub_setRprime_conebase_..._2026-10-15` (Born h+ 11.996558 / h− 11.663956 nb); the current budget is `systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17` (`total_with_bgsub19_rel`, both columns verified present in the respective CSV headers). The plotter instead reads the 2026-10-13 cone Born release (raw/no-bgsub; Born h+ 13.026987 / h− 12.697511 nb) and joins the 2026-10-14 cone15 budget. `RUN_STATUS.md` item 6 already records: "the live canonical 7-panel plotter still reads the cone Born release and now points at a SUPERSEDED budget column `total_with_cone15_rel`" (P-PLOTS registration USER-GATED). This audit confirms it at code level.
- **Impact:** Any figure regenerated today is a superseded-basis, superseded-budget product carrying the registered-canonical name: central values differ from the final basis by ≈8% integrated (B_R' = 7.2767% of D_cone) and the error bars are the superseded cone15 totals — non-quotable under the 2026-10-17 public quoting rule. Analysis-killing for final-basis plotting until repointed. Repoint note: the clean Born release (2026-10-15) has NO `alternative_scheme_rapgap31_relbin_emfix/` subdirectory (unlike the 2026-10-13 root), so the `CRAD_TABLE_KEY=RAPGAP31_relbin` branch of the `IBU_CSV` construction (line 108-111) must be handled as part of the repoint.

### C2. bgsub-diagnostic plotter omits the highest-x cap in the 4D density normalization — every S3_x0 panel point understated by ×0.662 (−33.8%)
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_bgsub_lowq2_djangoh.py:255` (density at 261-262)
- **Quote:** `df['dx'] = df['x_max'] - df['x_min']`
- **Reasoning:** The release-table S3_x0 `x_max = 1.5` is an internal migration-catch container edge, not a published bin edge (verified: cone Born CSV row `pos,210,...,S3_x0,...,0.021,1.5`). The canonical convention — implemented in the canonical plotter (`np.minimum(df['x_max'], 1.0) - df['x_min']`, line 414), in the MC comparison plotter (`x_width_with_last_bin_capped`, lines 76-85), and mandated in RUN_STATUS ("cap the highest-x bin at Δx = 1.0 − x_min when x_max > 1.0") — gives dx = 0.979. This script uses dx = 1.479, so `density` and `density_bgsub` for all 35 S3_x0 bins per charge are low by 0.979/1.479 ≈ 0.662. The same panel's kinematic text simultaneously displays the capped edge (line 315: `xmax_disp = min(xmax_t, 1.0)` → "x ∈ [0.021, 1.000]"), so one figure states the published support while normalizing with the container edge — internally contradictory.
- **Impact:** Demonstrated wrong physics content in the S3 panel (both nominal and bg-subtracted series) whenever the script runs. Analysis-killing for that panel; fix is one line (apply the same min(x_max, 1.0) cap).

---

## MAJOR

### M1. Canonical plotter's manifest describes a computation the code no longer performs (stale pre-repoint strings)
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:229, 232, 199, 699, 716-717, 46, 665`
- **Quotes:** `"plotted_yerr_source": "density_err = density * total_with_crad13_rel"` (229); `"the full 13-component releasebin budget"` (232); header `# ── 13-component budget band ...` (199); `"crad_formula": "sigma_corrected = sigma_uncorrected * C_rad; all absolute *_nb uncertainties scale by C_rad"` (699); `"density": "(cross_section_nb * C_rad) / (dQ2 * dx * dz * dpT_h)"`, `"ibu_yerr": "(total_unc_nb * C_rad) / (dQ2 * dx * dz * dpT_h)"` (716-717); `"yerr = total_unc_nb / 4D volume"` (46 and 665).
- **Reasoning:** Since the 2026-10-14 cone repoint the code plots the Born release values directly with no C_rad multiplication (lines 392-397: "absolute nb values must not be multiplied by C_rad again") and computes `density_err = density * BUDGET_COLUMN` = `density * total_with_cone15_rel` (line 419), with 15 components in `BUDGET_COMPONENTS`. The manifest — the provenance record shipped with every figure — claims C_rad is multiplied at plot time, names the wrong budget column (`total_with_crad13_rel`) in the same JSON block that also carries the correct `budget_column`, states the wrong component count (13 vs 15), and the header/inherited-style blocks repeat the superseded `total_unc_nb`-based yerr rule.
- **Impact:** The plotted numbers are traceable via `BUDGET_COLUMN`, but the self-contradictory uncertainty disclosure misdirects any auditor reconciling the error bars (wrong column, wrong formula, wrong count). For a canonical plotter whose products are meant to be quoted, this is a provenance-integrity defect, not cosmetics.

### M2. CRAD_APPLIED_INFO role/decision/caveat strings mislabel the applied primary scheme on every default run
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:222-224, 219-221, 756-763, 992-999`
- **Quote:** `"role": ("PRIMARY" if CRAD_TABLE_KEY == "DJANGOH14_xFcut" else "FORMAL ALTERNATIVE-SCHEME variation" if CRAD_TABLE_KEY == "RAPGAP31_xFcut" else "QUARANTINED/AUDIT-ONLY (do not apply)")` (222-224); decision string naming "DJANGOH14 xFGen<0 xFcut (EMPZ>45) applied as PRIMARY; RAPGAP31 xFcut is the FORMAL ALTERNATIVE-SCHEME variation" (219-221).
- **Reasoning:** The default and applied primary is `DJANGOH14_relbin` (lines 141-162; RUN_STATUS: "APPLIED PRIMARY (since 2026-10-09): DJANGOH14 releasebin"; the xFcut tables are superseded-for-application, as the script's own candidate dict states at lines 137 and 145). Under this role mapping a default run records role = "QUARANTINED/AUDIT-ONLY (do not apply)" for the applied primary, and reserves "PRIMARY" for a table marked SUPERSEDED-FOR-APPLICATION. The 2026-10-09/10-14 repoints updated the candidate tables and default key but not the role/decision/caveat strings.
- **Impact:** Every manifest emitted by the registered plotter misstates which C_rad scheme underlies the Born release. Provenance corruption in the quoting chain; follow-up only (string rewrite), no plotted-value impact.

### M3. xQ2 manifest names a release that is not the input read
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:948`
- **Quote:** `"release": "bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06"`
- **Reasoning:** `IBU_CSV` reads the 2026-10-13 cone Born release (lines 106-111), but the xQ2 manifest's `input.release` hard-codes the doubly-superseded 2026-10-06 coherent-prior release. The 7-panel manifest records the actual paths; only the xQ2 manifest carries the stale hard-coded name.
- **Impact:** Anyone tracing the xQ2 figure provenance is directed to the wrong release tree. Follow-up fix (derive the string from the actual input path).

### M4. bgsub-diagnostic plotter is dead as instantiated in the canonical snapshot
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_bgsub_lowq2_djangoh.py:56-59, 70-72`
- **Quote:** `RELEASE_DIR = os.path.dirname(SCRIPT_DIR)  # scripts/ is one level below release root`; `IBU_CSV = os.path.join(RELEASE_DIR, "ibu", "cross_sections.csv")`; pre-flight `sys.exit(f"[ABORT] Required input missing: {p}")`
- **Reasoning:** In the canonical snapshot the script lives in `canonical_highQ2_sidis_2026-06-30/release/scripts/`; its resolved `RELEASE_DIR` (`.../canonical_highQ2_sidis_2026-06-30/release/`) contains only `_run/, plots/, provenance/, scripts/` — no `ibu/`, no `metadata.json`, no `backgrounds/` (verified by directory listing). Every pre-flight input is missing, so the script always aborts. The canonical plotter received an absolute-`BORN_ROOT` repoint for exactly this reason (its lines 88-93 document it); this variant never did.
- **Impact:** The registered diagnostic cannot run from its registered location and its outputs cannot be regenerated in place. Dead registration; follow-up (repoint to an absolute release path) — but note C2 must be fixed in the same pass, otherwise the repoint would resurrect a script that renders wrong S3 densities.

### M5. bgsub-diagnostic x-Q2 map draws the S3 cell out to the container edge x=1.5
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_bgsub_lowq2_djangoh.py:644-645, 682-684`
- **Quote:** `x_hi = float(row["x_max"])` (no cap; contrast canonical plotter line 830: `x_hi = min(float(row["x_max"]), 1.0)`); `ax_xq2.axvline(x=1.0, ...)` + `"x=1"` label.
- **Reasoning:** The canonical plotter documents that x_max=1.5 is a migration-catch container edge, "neither displayed nor used", and draws the S3 rectangle only over the published support. This script draws the rectangle to x=1.5 and then draws an x=1 marker line that falls INSIDE the rectangle — visually asserting bin support above x=1, where no truth-level support exists.
- **Impact:** On-canvas misstatement of the published binning, inconsistent with the sibling plotter and with the script's own density-panel display text. Physics-content misrepresentation on a diagnostic figure.

### M6. plot_perbin_7col.py: laptop-only paths and a conditional h+/h− label inversion
- **File/lines:** `plot_perbin_7col.py:28-35, 152-159`
- **Quotes:** `INPUT_CSV = Path('/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/...')` (also OUT_DIR, PDF/PNG outputs, analysis-note copy); `def charge_label(charge): c = str(charge).lower(); if c.startswith(('p', 'pos', '+')): return r'$h^{+}$'; return r'$h^{-}$'`
- **Reasoning:** (a) The script cannot run on NAF: the input CSV and all output paths live on a laptop; no `reco_electron_opening_angle_veto_2026-06-23/` directory exists under `sidis_highQ2/plots/` (verified). A canonical-snapshot template whose data and products are off-workspace is untraceable. (b) `charge_label('h+')` does not start with 'p'/'pos'/'+' → returns h−. If the input CSV uses the h+/h− convention (the convention of the sibling `regen_minimal_7col_plots.py` metadata and the 980-bin MC family), every h+ panel is labeled h−. If it uses pos/neg, labels are correct. The sibling script handles 'h+' correctly (`"+" in c or c.startswith("p")`), so the two canonical templates disagree on the same convention.
- **Impact:** (a) dead template, broken provenance; (b) potential charge mislabeling — the exact trap class this loop hunts. Unverifiable here because the input is off-workspace; must be fixed or the input charge convention certified before any reuse.

### M7. Data-vs-MC chain: the validated current instance is superseded-basis; no 2026-10-17-basis instance exists
- **Evidence:** The script itself is CLI-parameterized and contains no stale release path (verified — `--release-dir` required, no hard-coded release). But per RUN_STATUS, the validated CURRENT instance is the 2026-10-13 5-prediction package `plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/`, whose data side is the NO-CONE S2' RAW release — superseded-basis since the 2026-10-13 cone promotion (regeneration PARKED) and now doubly superseded by the 2026-10-17 bgsub clean release.
- **Reasoning/impact:** Every existing rendered comparison package plots superseded-basis data (no-cone, unsubtracted) against Born-level MC curves; using it as "current" would compare MC against the wrong observable basis. The clean release IS loadable by this script (verified: `ibu/cross_sections.json`, schema `sidis_highq2_bbb_ibu_release_method_v2`, rows contain charge/bin_index/global_bin_index/cross_section_nb/stat_unc_nb/total_unc_nb/supported, lumi_nb 351000.0), but the clean RAW release is NOT Born-level, so a regenerated comparison must either use Born-release tables or be explicitly labeled raw-data-vs-Born-MC. Flagging as stale-relative-to-the-2026-10-17-clean-release per the audit brief.

---

## MINOR

### m1. No runtime guard against superseded-for-application C_rad keys (canonical plotter)
- **Lines:** `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:136-165`
- `CRAD_TABLE_KEY` can select `DJANGOH14_xFcut`, `RAPGAP31_xFcut`, or the quarantined tables (files exist; the exists-assert passes); only comments mark them superseded. Since C_rad is a provenance/alignment gate only (no multiplication), impact is a mislabeled gate, not a double correction. Add a refuse-list for superseded/quarantined keys.

### m2. 0-suppression disclosed off-canvas only (canonical plotter)
- **Lines:** 496-510 (skip `y <= 0` or non-finite), 620-621 (stdout counts), plotted_values.csv `reason='nonpositive_or_nan'`.
- Nonpositive IBU bins are omitted from log-scale panels and recorded in the CSV/manifest/stdout, but the rendered panels carry no on-canvas indication that points were suppressed. Acceptable under the no-on-canvas-caveat style rule; noted because log-scale omission is invisible to a reader of the figure alone.

### m3. Data-vs-MC plotter: dead stale caveat strings; no data-side level field in the manifest
- **Lines:** `compare_data_vs_mc_z005_4generator_born.py:32-38`
- `CAVEAT_LINES` claims "data are Born-level (per-bin post-unfold C_rad from the DJANGOH14 xFGen<0 xFcut EMPZ>45 table)" (xFcut superseded-for-application since 2026-10-09) and "the release remains raw/no-bgsub with response fake-fraction subtraction only" (false since 2026-10-17 — the central release IS background-subtracted). Verified dead code: `CAVEAT`/`RAPGAP_CAVEAT` are referenced nowhere else, and the emitted manifest contains no data-side Born-vs-raw level field — the mandatory level disclosure is left entirely to the caller. No rendered product is contaminated today; would become MAJOR if ever wired into a README. Keep dead or rewrite.

### m4. Data-vs-MC plotter: off-scale ratio points hidden without per-panel disclosure
- **Lines:** 363-364 (percentile-based `upper` for panel ratio ylim), 477 (`rx.set_ylim(0, 3)` in the combined PDF).
- Only the global-bin figure counts clipped points (`global_bin_ratio_clipped_points`, display-only, disclosed). Per-panel ratio figures and the combined PDF can hide points above their visible range with no count. Display-level, no table impact.

### m5. regen_minimal_7col_plots.py: fixed ylim (0, 0.25) with no off-scale disclosure; step-bridging over undefined bins; unrecorded denominator-key choice
- **Lines:** 473 (`ax.set_ylim(0.0, 0.25)`), 468-471 (step edges built from valid indices only, bridging NaN gaps), 96-108 (denominator loader prefers `n_subtracted` with no manifest record of the chosen key).
- The integrated background fraction is 8.11% (study summary JSON), so high-z/low-pT tail bins plausibly exceed 0.25 and would be silently cut; no clipped-point counter exists. For the current input the denominator semantics are correct (the data-vector JSON's own warning states `n_subtracted` holds RAW counts in this stopgap file, so the plotted ratio is bg/raw-data), but the silent key preference is unprovenanced and would mis-semantics on a genuinely subtracted vector.

### m6. plot_perbin_7col.py: forced ylim (0, 0.1) with partial disclosure; phantom SCRIPT_OUT
- **Lines:** 173-174 (`final = (0.0, 0.1)` forced), 33/345 (manifest declares a script copy at OUT_DIR that the script never writes).
- `pre_ylim` is recorded per panel in `y_limits_check`, so exceedance is detectable from the manifest but not from the figure. The declared `SCRIPT_OUT` artifact is never produced.

### m7. Integrated C_rad gate tolerance has almost no headroom over serialization residues
- **Lines:** `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:359-364`
- `abs(_got - _want) <= 1e-12 * _want` accommodates the documented parse residues (up to ~9.716e-13 relative ibu) with <3% headroom. Not a present defect (current residues pass), but a future re-serialization could false-alarm the gate. Robustness note only.

---

## INFO

- **i1.** Canonical plotter line 422: `assert np.max(np.abs(df['density_err'] - df['density'] * df[BUDGET_COLUMN])) == 0.0` is tautological — `density_err` was computed as exactly that product at line 419; the gate verifies nothing.
- **i2.** Canonical plotter lines 108-111: `os.path.join(BORN_ROOT, "" , "ibu", ...)` relies on empty-string join semantics for the default key; obscure but correct.
- **i3.** Data-vs-MC plotter: MC statistical uncertainties are loaded, scaled, and written to the ratio table but never drawn on the ratio curves (only data-side total uncertainty is shown; disclosed in `panel_ratio_data_uncertainty_note`). `--use-covariance` is accepted and honestly reported as not implemented.
- **i4.** bgsub-diagnostic plotter: the plot-time linearized subtraction (corr_factor = cross_section_nb / yield_unfolded per bin) is explicitly self-labeled non-rigorous ("the rigorous central method is to subtract ... before acceptance correction and unfolding"); the sidecar lumi rescale (356613 → release lumi, factor ≈0.98426) is correctly directioned and documented. The method disclosure is honest; the defects are C2/M4/M5, not the linearization itself.
- **i5.** plot_perbin_7col.py: the independent-Poisson sigma for r=k/n, sqrt(k/n² + k²/n³), matches its disclosed assumption; if the numerator were a subset of the denominator the binomial variance would be smaller, so the formula is conservative, not underestimating.
- **i6.** regen_minimal_7col_plots.py targets the June-2026 reco-backgrounds study tree (20260625) — historical diagnostic context, not a current-basis product; its "supersedes" notes are accurate and its component convention (electron/V0 generator averages + lowQ2) matches the study design.

---

## Explicitly ruled out (checked, no defect)

1. **d⁴σ bin-width normalization (canonical plotter):** all four factors present — `density = cross_section_nb / (dq2 * dx * dz * dpT)` with the highest-x cap implemented (`np.minimum(df['x_max'], 1.0) - df['x_min']` → S3 dx = 0.979); unit label nb/(GeV²·GeV) is dimensionally correct. (Script 3 is the exception — see C2.)
2. **Charge mapping (canonical plotter):** 'pos' → h⁺ and 'neg' → h⁻ superscripts, and per-charge output files, are consistently assigned; no swap. Data-vs-MC plotter enforces pos/neg 245+245 on both data and prediction sides.
3. **Ratio-panel orientation (data-vs-MC):** `ratio_mc_over_data` matches the "MC / Data" axis labels; the data reference at y=1.0 with yerr = total/data (fractional) is the correct first-order propagation for MC/Data. No Data/MC inversion.
4. **4D-mode x-width (data-vs-MC):** `x_width_with_last_bin_capped` caps x_max > 1.0 at 1.0; only S3_x0 (1.5) is affected, equivalent to the highest-bin cap; both sides are divided by the same volume, so ratios are cap-invariant anyway.
5. **C_rad double application (canonical plotter):** ruled out — the code explicitly does not multiply Born-release values by C_rad (lines 392-397); the table serves as a provenance/bin-alignment gate with per-bin kinematic-edge asserts (atol 1e-12) and an integrated-value provenance gate.
6. **Budget join integrity (canonical plotter):** one-to-one merge on (charge, bin_index), 490/490 match asserted, nonnegative and finite asserted; budget charge labels are release-normalized pos/neg.
7. **Hard-coded luminosity:** none wrong. Canonical plotter manifests 351000.0 nb⁻¹ only; data-vs-MC takes `--lumi-pb` and records the release-stored 351000 convention; bgsub-diagnostic reads lumi from release metadata and rescales the sidecar from its own 356613 normalization (documented stopgap, correct direction). No 126,880 nb⁻¹ and no 356613-as-data-lumi abuse anywhere.
8. **Log/linear axis misuse:** canonical plotter uses log-y with a linear fallback only when a panel has no positive values; nonpositive bins are skipped and recorded, not clipped onto the axis. Data-vs-MC ratio panels are linear with an explicit 1.0 reference line.
9. **Marker-value serialization residues:** the integrated C_rad gate tolerance (1e-12 relative) accommodates the documented parse residues (~9.716e-13 relative ibu / ~9.103e-13 bbb); the kinematic-edge asserts compare exact decimal edge values. No residue-induced false physics differences are possible at the current tolerance (headroom noted as m7).
10. **Pair-summed pT display bins (regen):** pairs (0,1),(2,3),(4,5),(6,7),(8,9) over native edges [0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0] reproduce exactly the canonical [0,0.5],[0.5,1.0],[1.0,2.0],[2.0,4.0],[4,8] display bins; z edges match the canonical 7-bin schema.
11. **Supported-flag handling:** no `supported=False` rows exist in the cone Born release CSV (grep verified), so plotting all bins without a supported-filter is not currently dropping or adding anything; the data-vs-MC loader carries the flag through without gating on it (harmless today).

## Missing evidence

- **plot_perbin_7col.py input CSV charge-column convention** (h+/h− vs pos/neg): the file is on a laptop path, not in the workspace; needed to decide whether M6(b) is an active mislabel or a latent trap.
- **Per-bin maximum of `frac_total`** in `background_fractions_vs_data_table_7col.csv` (regen study): needed to confirm whether the fixed 0.25 ylim actually clips tail bins (m5).
- **Born-level clean-release data-vs-MC plan** (M7): whether the regeneration will use the Born release tables or the raw clean release with an explicit raw-vs-Born label — no such instance exists today.

## Recommended next check

Repoint the registered canonical Born plotter (script 1) to the final basis — `BORN_ROOT` → `born_crad_bgsub_setRprime_conebase_..._2026-10-15`, `BUDGET_*` → `systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17` with `total_with_bgsub19_rel` — resolving the missing `alternative_scheme_rapgap31_relbin_emfix/` subdir for the RAPGAP31_relbin branch, and in the same pass fix the one-line x-cap in script 3 (line 255). The stale-basis repoint (C1) and the x-cap (C2) are the two defects that materially corrupt physics content of final figures; the manifest-string fixes (M1-M3) should ride along so the first final-basis figures do not ship with self-contradictory provenance.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 1 of 3 (plotting scripts), read-only; no analysis script was modified.
- Files audited: 5 (4 targets + both plotting/ scripts present). All line numbers refer to the current on-disk copies.

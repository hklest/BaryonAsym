# Loop 2 Plotting-Script Audit — H1 High-Q² Inclusive Charged-Hadron SIDIS (v6/z005)

**Date:** 2026-10-19
**Critic loop:** 2 of 3 (plotting scripts), read-only; independent re-derivation of loop-1 claims + expanded coverage
**FINAL basis at audit time:** cone+S2′+Set-R′ background-subtracted clean release, installed 2026-10-17; public quote = this release + `total_with_bgsub19_rel` ONLY
**Tooling notes:** the `sidis-hq2-plotting` skill failed to load (tool error); the audit proceeded from on-disk code, `context-handoff.md`, `RUNLIST.md`, and release artifacts. Loop-1 line numbers were treated as claims, not evidence; every verdict below is re-derived from fresh reads.

## VERDICT

**FATAL (audit FAIL).** Loop-1 C1 and C2 are independently CONFIRMED at code level: the registered canonical Born plotter still reads the superseded 2026-10-13 cone Born release joined to the superseded `total_with_cone15_rel` budget, and the bgsub-diagnostic variant normalizes every S3_x0 panel point with the uncapped container edge (dx = 1.479 instead of the published 0.979), understating that panel by ×0.662 (−33.8%) whenever it runs, while its own panel text displays the capped edge. M1–M7 all CONFIRMED. Expanded coverage finds no new CRITICAL physics defect — the covariance clean-release build chain and the MAPTMD overlay chain are clean on every audited trap class — but one new MAJOR registry-integrity defect: the canonical RUNLIST/SEVEN_CATEGORY index declares two ibu_crossclosure plotters that exist nowhere in the workspace, so the canonical snapshot is incomplete relative to its own registry and those declared scripts are unauditable.

## Material reviewed

Part A (independent re-derivation):
1. `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` — 1008 lines, read in full.
2. `.../release/scripts/make_finalcandidate_7panel_and_xQ2_bgsub_lowq2_djangoh.py` — paths/pre-flight (40–160), density/bg-subtraction (230–349), x-Q² map (625–699) read in full.
3. `.../plotting/plot_perbin_7col.py` (1–80, 140–179) and `.../plotting/regen_minimal_7col_plots.py` (charge_label region).
4. `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py` (CLI/release handling) + the 2026-10-13 package manifest and data-side sidecar provenance.
5. Supporting: cone Born release `ibu/cross_sections.csv` S3_x0 row; canonical `release/` directory listing; RUNLIST.md registry overrides.

Part B (expanded coverage):
6. `H1ChargedSidis_HighQ2/scripts/systematics/covariance_cleanrelease_2026-10-17/` — `common_covariance_cleanrelease.py` (495 lines, full) and `s09_build_covariance_package.py` (~1290 lines, full); trap-class sweep of all stage scripts (no matplotlib anywhere in the set).
7. `H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_covlegs_2026-10-17/run_bbb_ibu_covlegs_stage3_2026-10-17.py` — 731 lines, full.
8. `.../canonical_highQ2_sidis_2026-06-30/plotting/plot_covariance_z005_2026-09-23.py` — 129 lines, full.
9. `.../plotting/ibu_crossclosure/plot_ibu_crossclosure_7col_combined_percharge_z005.py` + fullscale variant — **NOT ON DISK** (B-M1).
10. `sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/`: `make_finalcandidate_7panel_and_xQ2_maptmd_overlay_2026-10-13.py` (full), `build_maptmd_predictions_2026-10-13.py` (full), `build_data_side_noz_photonborn_2026-10-13.py` (scaling/logic regions), package manifest + sidecar.

## Part A — independent verification of loop-1 claims

| Item | Loop-1 claim | Verdict | My independent evidence |
|---|---|---|---|
| C1 | Canonical Born plotter reads superseded basis + superseded budget | **CONFIRMED** | Lines 106 (`BORN_ROOT = ".../born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13"`), 112 (`BUDGET_DIR = ".../systematics_cone_s2prime_15comp_shiftedlegs_..._2026-10-14"`), 113–114 (`BUDGET_COLUMN = "total_with_cone15_rel"`); `BUDGET_COMPONENTS` = exactly 15 entries (115–120). Superseded by the 2026-10-17 clean release (`born_crad_bgsub_setRprime_conebase_..._2026-10-15`; budget `systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17` / `total_with_bgsub19_rel` per RUNLIST.md:273–287 and the handoff). Physics impact: central values ≈8% high vs final basis (B_R′ = 7.2767% of D_cone), error bars non-quotable under the 2026-10-17 public quoting rule. |
| C2 | bgsub plotter uncapped dx understates every S3_x0 point by ~0.662× | **CONFIRMED** | Line 255 `df['dx'] = df['x_max'] - df['x_min']` (no cap); `vol` from this dx feeds `density` (259) and `density_bgsub`/`density_err_bgsub` (261–262); line 315 `xmax_disp = min(xmax_t, 1.0)` displays the capped edge in the same panel. Cone Born CSV line 212: `pos,210,210,S3_x0,S3,1500.0,20000.0,0,0.021,1.5,...` → dx = 1.479 vs canonical 0.979 (canonical plotter line 414) → 0.979/1.479 = 0.6619 (−33.8%), all 35 S3_x0 bins per charge, both series. |
| M1 | Stale manifest strings describe a computation the code no longer performs | **CONFIRMED** | Line 229 `"plotted_yerr_source": "density_err = density * total_with_crad13_rel"`; line 232 "the full 13-component releasebin budget"; line 199 header "13-component budget band"; lines 46 and 665 "yerr = total_unc_nb / 4D volume"; line 699 `crad_formula` "sigma_corrected = sigma_uncorrected * C_rad; all absolute *_nb uncertainties scale by C_rad"; lines 716–717 density/`ibu_yerr` formulas containing "* C_rad"; line 951 repeats the stale `crad_formula` in the xQ2 manifest. Actual code: no C_rad multiplication (392–397), `density_err = density * BUDGET_COLUMN` (419), 15 components (115–120). |
| M2 | CRAD role/decision/caveat strings mislabel the applied primary on every default run | **CONFIRMED** | Lines 219–221 decision names "DJANGOH14 xFGen<0 xFcut (EMPZ>45) applied as PRIMARY"; lines 222–224 role map returns "QUARANTINED/AUDIT-ONLY (do not apply)" for every key except the two xFcut keys; the default and applied key is `DJANGOH14_relbin` (line 162) → every default run labels its applied primary QUARANTINED; caveats at 756–763 and 992–999 repeat the xFcut-as-PRIMARY wording. |
| M3 | xQ2 manifest names a release that is not the input read | **CONFIRMED** | Line 948 `"release": "bbb_ibu_coherentprior_..._2026-10-06"` while `IBU_CSV` (108–111) reads the 2026-10-13 cone Born release. |
| M4 | bgsub plotter dead as instantiated (RELEASE_DIR resolution) | **CONFIRMED** | Lines 56–57 `RELEASE_DIR = os.path.dirname(SCRIPT_DIR)`; line 59 `IBU_CSV = RELEASE_DIR/ibu/cross_sections.csv`; lines 70–72 `sys.exit` on any missing pre-flight input. Directory listing of `canonical_highQ2_sidis_2026-06-30/release/`: only `_run/, plots/, provenance/, scripts/` — no `ibu/`, no `metadata.json`, no `backgrounds/`. Always aborts from its registered location. |
| M5 | bgsub x-Q² map draws the S3 rectangle to the container edge x=1.5 with an x=1 line inside it | **CONFIRMED** | Lines 644–645 `x_hi = float(row["x_max"])` (no cap); lines 682–684 `axvline(x=1.0, ...)` + "x=1" label. Canonical contrast: line 830 `x_hi = min(float(row["x_max"]), 1.0)` with the container-edge documentation at line 749. |
| M6 | plot_perbin_7col.py laptop-only paths + h+/h− label inversion | **CONFIRMED** | Lines 28–35: input CSV, outputs, manifest, and analysis-note copy all under `/Users/hklest/Desktop/opencode/H1/...`; no `reco_electron_opening_angle_veto_2026-06-23/` under `sidis_highQ2/plots/` (glob verified). Lines 156–159: `charge_label('h+')` fails `startswith(('p','pos','+'))` → returns h⁻ (both h+ and h− inputs map to h⁻ under the h+/h− convention); sibling `regen_minimal_7col_plots.py:347–349` handles "+" correctly — the two canonical templates disagree. Input CSV off-workspace → active-vs-latent remains undecidable (as loop 1 stated). |
| M7 | Data-vs-MC chain: no current-basis instance; the validated instance is superseded-basis | **CONFIRMED** | Script is CLI-parameterized with no hard-coded release (`--release-dir` required, line 58). Package manifest `manifest_data_vs_mc_z005_5pred_releasebin_20261013.json` line 4: `release_dir = .../data_side_noz_photonborn`; sidecar `data_side_noz_photonborn/ibu/cross_sections.json` line 15: `raw_base_release = .../bbb_ibu_detints2prime_S3xmerged_..._2026-10-09` — the no-cone S2′ RAW release, doubly superseded (cone 2026-10-13, bgsub clean release 2026-10-17). Stale caveat at compare-script line 35 ("the release remains raw/no-bgsub with response fake-fraction subtraction only") confirmed false since 2026-10-17. |

**Part A result: 9/9 CONFIRMED. No refutations, no partials. Loop-1 severity assignments stand.**

## Part B — expanded coverage findings

### CRITICAL — none new

### MAJOR

**B-M1. Canonical registry declares two plotting scripts that do not exist anywhere in the workspace**
- **Files/lines:** `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/RUNLIST.md:185-186`; `SEVEN_CATEGORY_SCRIPT_INDEX.md:52-53`
- **Quote:** "`plotting/ibu_crossclosure/plot_ibu_crossclosure_7col_combined_percharge_z005.py`" / "`..._fullscale_z005.py`" — RUNLIST Phase 7: "Note-ready diagnostic plotting scripts included here".
- **Evidence:** remote snapshot `plotting/` contains only 4 files (`plot_covariance_z005_2026-09-23.py`, `plot_perbin_7col.py`, `plot_uncertainty_pth_panels.py`, `regen_minimal_7col_plots.py`); the local mirror's `plotting/` contains only `plot_covariance_z005_2026-09-23.py`; no `ibu_crossclosure/` directory in either snapshot (glob `**/*crossclosure*` in both → nothing); the declared origin package `sidis_highQ2/plots/closure/ibu_z005_crossclosure_7col_2026-07-02/` does not exist; filename grep across `H1ChargedSidis_HighQ2/scripts` returns only unrelated scripts. SEVEN_CATEGORY_SCRIPT_INDEX records the origin as `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/closure/...` (laptop).
- **Why it matters:** RUNLIST.md is mandatory pre-reproduction reading; it asserts these scripts are "included here" when they are absent. The canonical snapshot is therefore incomplete relative to its own registry; the declared cross-generator-closure diagnostics cannot be reproduced, regenerated, or audited from the workspace; and any bin-width/charge/limit defect inside them is invisible — the exact trap class of this audit loop (cf. M6's laptop-origin template, which did contain a latent charge inversion).
- **Impact:** registry-integrity/reproducibility defect, not a wrong-plotted-number defect (no on-disk code to be wrong). The brief's B4 targets could not be audited because they do not exist. Follow-up: restore both scripts into `plotting/ibu_crossclosure/` from a verified source and submit them to loop-3 review, or strike/annotate the registry entries as NOT INCLUDED.

### MINOR

**B-m1. The only on-disk covariance plotter targets the superseded v4 provisional package; no plotter exists for the current clean-release covariance package**
- **File/lines:** `plot_covariance_z005_2026-09-23.py:12-15, 19-20`
- Reads `data/covariance/z005_provisional_v4_2026-09-23` + `z005_supplement_modelreg_2026-09-23` (coherent-prior era; footer: "PROVISIONAL: C_rad unapplied; … h+/h− statistical cross-block ZERO BY ASSUMPTION … response-MC statistical covariance ABSENT"). The current covariance deliverable is `H1ChargedSidis_HighQ2/data/covariance/z005_covariance_cleanrelease_2026-10-19/` (s09 product), which has no plotting driver. The script is dated and honestly footed, so no wrong number is produced today; but a regeneration presented as "the covariance package" would silently revert to the superseded basis and to the zero-cross-block/no-MCstat limitations the new package explicitly closed. Follow-up only (dated clone for the 2026-10-19 package).

**B-m2. plot_covariance diagonal panels floor plotted values at 1e-12 without on-canvas disclosure**
- **File/line:** `plot_covariance_z005_2026-09-23.py:98` — `np.maximum(d[:245],1e-12)` (and `d[245:]`); zero/under-floor components drawn at 1e-12 on the log axis. Zero-variance bins are counted off-canvas (`zeros`, lines 73/127). Display-only.

**B-m3. MAPTMD overlay ratio panels: fixed ylim (0, 3.1) can clip covered-bin points; disclosure is off-canvas only**
- **File/lines:** `make_finalcandidate_7panel_and_xQ2_maptmd_overlay_2026-10-13.py:61-64, 108, 124` — fixed limits per user decision 2026-10-14; the validation sidecar discloses the policy, retains `ratio_max_covered`/`ratio_max_errorbar_inclusive`, and names the clipped h+ excursion, but no per-panel clipped-point count exists. Same display class as loop-1 m4; here the exceedance is at least recorded in the sidecar. Display-only.

### INFO

- **B-i1.** s09 bin_map.csv carries x_max=1.5 verbatim — correctly disclosed. `s09_build_covariance_package.py:970-987` writes release-table axes verbatim (S3_x0 x_max=1.5); the package README (line 1136) states the Top-x cell ruling: published 0.021 ≤ x < 1.0, Δx = 0.979, tabulated 1.5 is an internal migration-catch container edge. No normalization uses it. Correct labeling; no action.
- **B-i2.** run_bbb_ibu_covlegs_stage3_2026-10-17.py: no plotting; output logic clean. Charge-major slicing (pos 0–489 / neg 490–979; lines 155–161, 215), Set-R′ guard-law bitwise checks (325–334), nominal reproduction gate vs official_reunfold with the clean-release integrated pins 10.50881083667206/10.21750255073882 (121–122, 424–451), component convention `|IBU_leg_sub − IBU_nom_sub|/IBU_nom_sub`, RG governs (575–577), plus-leg machinery gate bitwise vs the installed 19-comp detector cache (624–652). LUMI = L_DATA_NB (351000.0 convention; s09 G3 independently asserts LUMI == 351000.0). No defect in the audited classes.
- **B-i3.** covariance_cleanrelease_2026-10-17 set contains no matplotlib plotters (grep over all 17 scripts: zero hits). `common_covariance_cleanrelease.py`: LUMI = 351000.0 (line 86), current-basis paths (REL19 = 19-comp cleandenom 2026-10-17, lines 70–76), integrated anchors = clean-release values (line 101), keyed (charge, bin_index) joins with explicit row-order independence (388–404). No stale lumi/budget constants anywhere in the set (no 297657.8 / 356613 / 126880 / `total_with_cone15` / `crad13`).
- **B-i4.** MAPTMD overlay chain is unit- and cap-consistent. Builder: provider densities (nb/GeV³) × volume with dx_eff = 1.0 − x_min cap for the S3 cell (324–332), band ordering gated (371–384), expected integrated totals gated to 1e-4 (137–140, 491–508), charge map h+→pos / h−→neg (81). Plotter: re-derives the same capped volume on both data and prediction sides (lines 37, 44) and divides the bin-integrated prediction by it (line 46) — no double normalization; ratio = data density / MAPTMD midpoint with data-only relative error propagated (104–106), label `data/MAPTMD24` matches; pos→h⁺ / neg→h⁻ (107, 115). Data-side builder scales all nb-valued fields by C_gamma_born and leaves relative fields unchanged (91–94, 201–225), consistent with the plotter's `total_unc_nb/vol` error bars and the "uncertainties are already photon-Born-scaled" disclosure.
- **B-i5.** The overlay package inherits the M7 superseded data side. DATA_JSON = the `data_side_noz_photonborn` sidecar (raw_base = the 2026-10-09 no-cone S2′ RAW release) — the same doubly-superseded basis as M7; regeneration PARKED; the package is dated 2026-10-13 and its manifests disclose the photon-Born level and the known ~9% data-vs-MC level inconsistency. No new defect; folds into M7's regeneration decision. Note the overlay itself is level-consistent (photon-Born data vs photon-only MAPTMD24).
- **B-i6.** plot_covariance quadrant labels and relative/absolute usage verified correct. h+/h+, h+/h−, h−/h+, h−/h− quadrants match the charge-major 0–244/245–489 layout under imshow origin='upper' semantics (lines 56–60); the diagonal comparison uses the `rel_cov_*` twins and is labeled "relative uncertainty" (84–100).

## Explicitly ruled out (Part B, checked, no defect)

1. **Missing highest-x cap:** not present in the MAPTMD chain (builder dx_eff cap + plotter caps both sides) nor anywhere in the covariance set (no density normalization exists there). The only uncapped instances remain C2/M5 in the loop-1 set.
2. **Ratio denominator inversions:** none. s09 relative twins `C/outer(sigma,sigma)` and correlation construction are standard; overlay ratio is data/MAPTMD24 as labeled; plot_covariance plots sqrt(diag(rel_cov)) as relative uncertainty.
3. **Charge mislabeling:** none in Part B files. Charge-major conventions asserted in code (s09 `keys_of` pos 0–244 / neg 245–489; stage3 `charge_slice`; overlay pos/neg → hplus/hminus outputs).
4. **Hard-coded luminosity:** LUMI = 351000.0 everywhere it appears (common:86, s09:139); s09 additionally asserts the stale 297657.8 string is absent from the release/budget payloads (469–475); no 356613 or 126880 anywhere in the audited set.
5. **Covariance diagonal-vs-total confusion:** none. G2 gates diag(C_total) attribution against the 19-column budget quadrature with 0 unattributed bins and five named convention causes; per-source sqrt(diag(C_k))/sigma vs |published signed column| semantics handled explicitly (756–770); `mag_ibu_stat` implemented by the C_stat diagonal with a no-double-count guard (198–203); detint split guard (787–789).
6. **Relative-vs-absolute unit mixing:** none. nb² and counts² matrices separated by /L² twins with per-file units fields; the nb rule (nb = rel × sigma) re-asserted on every consumed column (519–525).
7. **Stale release/budget paths in the Part B set:** none. s09 reads the 2026-10-17 clean release CSV, the 19-comp cleandenom budget (sha-pinned), and the 2026-10-15 Born bin_map; the budget comparison column is `total_with_bgsub19_rel` (751).
8. **Bin-width/density errors in stage3:** not applicable (yield/cross-section tables only; no densities computed).
9. **MAPTMD double normalization:** ruled out (B-i4).
10. **Eigen-floor twin misuse:** the floored C_MCstat twin is published only as an inversion twin and is provably NOT in C_total (P2, 602–653); rank deficiency and the downward-bias direction are disclosed in the README.

## Missing evidence

- The two declared ibu_crossclosure plotter files (absent from disk; needed to audit their bin-width/charge/limit handling) — B-M1.
- `plot_perbin_7col.py` input CSV charge-column convention (laptop; unchanged from loop 1).
- A clean-release covariance plotting driver (none exists for the 2026-10-19 package; B-m1).
- The M7/overlay regeneration plan (Born clean-release tables vs explicitly labeled raw clean-release side) — unchanged from loop 1.

## Recommended next check

The decisive physics fix remains loop-1's: repoint the registered canonical Born plotter (C1: `BORN_ROOT` → `born_crad_bgsub_setRprime_conebase_..._2026-10-15`, `BUDGET_*` → `systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17` with `total_with_bgsub19_rel`, resolving the missing `alternative_scheme_rapgap31_relbin_emfix/` subdir for the RAPGAP31_relbin branch) and apply the one-line x-cap in the bgsub variant (C2, line 255), with the M1–M3 manifest-string rewrites riding along. For the new finding, the decisive follow-up is the registry repair in the same pass: restore the two declared crossclosure plotters into `plotting/ibu_crossclosure/` from a verified source and submit them to loop-3 review, or annotate RUNLIST.md / SEVEN_CATEGORY_SCRIPT_INDEX.md that they are NOT INCLUDED, so the canonical registry stops asserting provenance it cannot deliver.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 2 of 3 (plotting scripts), read-only; no analysis script was modified.
- Files audited: 10 targets (2 absent from disk). All line numbers refer to current on-disk copies.
- Tooling notes: `sidis-hq2-plotting` skill load failed; no file-write tool available to this agent — this report persisted by the delegating supervisor session.

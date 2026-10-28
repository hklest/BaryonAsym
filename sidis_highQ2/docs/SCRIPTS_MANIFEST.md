# SCRIPTS_MANIFEST.md — H1 high-Q² charged-particle SIDIS

**Date:** 2026-05-28
**Scope:** Every active script in `H1/sidis_highQ2/scripts/` (local) and `H1ChargedSidis_HighQ2/scripts/` (NAF). Quarantined scripts are NOT listed here (see `quarantine/README.md`).
**Companion document:** `docs/REPRODUCING_THE_ANALYSIS.md` (step-by-step recipe in canonical run order).

**Quarantine note (2026-05-28):** 10 stale docs moved to `quarantine/docs/` — 2 handoffs
(`decision-binning-amendment-HQv5fr2.md`, `handoff-HQv5fr2-rebuild-on-naf.md`), and 8 methodology
docs (`crad_v5f_methodology_FROZEN.md`, `session_state_2026-05-25.md`,
`TUnfold_uniform_low_diagnosis_2026-05-24.md`, `TUnfold_scan_2026-05-25.md`, plus 4 intermediate
binning study docs). Only `binning_adoption_HQ-v5g-Bprime_2026-05-24.md` and
`binning_freeze_2026-05-24.md` are retained as durable decision records.

**Headline:** ~43 active scripts. **10 are truly load-bearing** for the historical v3/v3p1 chain;
**5 additional v3final drivers** and **5 v3p2 drivers** complete the canonical v3final chain. The
rest are diagnostic, plotting, helpers, smoke tests, or one-shot binning extractors that produced
FROZEN artifacts (don't re-run unless you're re-doing that frozen study).

---

## Stage definitions

| Stage | Meaning | Where it runs |
|---|---|---|
| Extract | Per-track / per-event CSV extraction from sidistree ROOT | NAF Condor (multi-file) |
| Response | Build 480×480 R matrix from merged CSVs | Local |
| Unfold | BBB / IBU / TUnfold on R + data marginal | Local |
| UnfoldSyst | The 4 internal unfolding systematics (σ_stat from TUnfold cov, σ_reg, σ_MCstat, σ_model) | Local |
| Systematics | The 6 detector + 3 normalization + noise-floor sources | NAF Condor for shifted-response builds; local for analytic sources and re-unfolds |
| Combine | `combine_systematics.py` → master JSON | Local |
| Trust | Trust mask + catastrophic-bin diagnostic | Local |
| Release | Assemble `data_release_v1p2_final/` package | Local (`scripts/v3final/build_data_release_v1p2_final.py`) |
| Diagnostic | Useful but not load-bearing | Local |
| Plot | Figures only | Local |
| Helper | Imported module, not invoked directly | Local |

## Status definitions

| Status | Meaning |
|---|---|
| FROZEN-CANONICAL | Current production-blessed script. Re-run as needed. |
| FROZEN-ONE-SHOT | Produced an artifact that is itself frozen; do NOT re-run unless re-doing that frozen study. |
| OPTIONAL | Useful diagnostic or plot; can be skipped without breaking the chain. |
| GUARDRAIL | Pre-submit safety check (binary-identity, smoke-gate). |
| HELPER | Imported by a vital script; never run standalone. |
| SUPERSEDED-on-disk | A `_v2` or `_v3` predecessor of a current `_v3` or `_v3p1`; quarantine candidate. |
| ORPHAN | Claims a canonical output but no such output exists on disk, OR not imported by any vital script. |

---

## 1. Local scripts: `H1/sidis_highQ2/scripts/`

### 1.1 Top-level (`scripts/*.py`)

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `compute_crad_v3_HQv5gBprime.py` | UnfoldSyst (radiative) | SUPERSEDED/HISTORICAL-AUDIT only; do **not** use for current z005 `C_rad` | `pertrack_mc_v3_gen_djangoh14_{,norad_}merged.csv.gz`, `pertrack_mc_v3_disevent_djangoh14_{,norad_}merged.csv.gz` | Historical `data/crad_v3_HQv5gBprime_2026-05-24.json`, `plots/v5g_crad_4D_v3/v5g_crad_4D_v3.pdf`; current provisional baseline is `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/full_crad_v1_summary.json` | Never for current z005. Retain only to audit the superseded Scheme-B′/v3 calculation. |
| `diag_catastrophic_4D_bins.py` | Trust | FROZEN-CANONICAL | All `unfolded_4d_v3_*.json`, both `*_covariance.npz`, response NPZ, sliced unfold JSONs (audit), `xsec_v3_*.json` | `data/v5g_catastrophic_4D_bins_diag.json`, `data/v5g_4d_trust_mask.json`, PDF | Every time the nominal unfolds change (after τ change, after binning change). |
| `perbin_LAr_HFS_beam_shifts.py` | Systematics (NAF-side helper) | SUPERSEDED-on-disk — claims HQ-v5f-r1 binning in header; not used by current 4D pipeline | NAF ROOT trees (read-only) | `/tmp/perbin_LAr_HFS_beam.json` | Never under current pipeline. Quarantine candidate. |
| `_beam_energy_utils.py` | Helper | HELPER | — | (module) | — |
| `_binary_guard.py` | Guardrail | GUARDRAIL | `binary_identity_manifest.json` | (raises on mismatch) | Before any production submission. |
| `_smoke_gate.py` | Guardrail | GUARDRAIL | (SSH NAF) | (raises on smoke-test failure) | Before any production submission. |
| `_extract_y0p7_v5g_4period_perevent.py` | Diagnostic (binning study input) | FROZEN-ONE-SHOT | NAF sidistree | `/tmp/y0p7_v5g_4period_perevent.csv` → `data/y0p7_v5g_4period_perevent.csv` | Only if re-doing the Scheme B′ binning study. Binning is FROZEN; do not re-run. |
| `_extract_y0p7_v5g_perevent.py` | Diagnostic | SUPERSEDED-on-disk (3-period variant; superseded by `_4period_` sibling) | NAF sidistree | `/tmp/y0p7_v5g_perevent.csv` | Never under current 4-period policy. Quarantine candidate. |
| `binary_identity_manifest.json` | Guardrail data | FROZEN-CANONICAL (v1.6) | — | (manifest) | When binaries are deliberately rebuilt. |

### 1.2 `scripts/unfolding/`

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `_unfold_4d_common.py` | Helper | HELPER (imported by all 4D unfolders) | — | (module: bin indexing, L-operator builders, loaders) | — |
| `build_response_4d.py` | Response | **VITAL — FROZEN-CANONICAL** | `data/pertrack_v5g_fixedFrame_v3_merged.csv.gz` (data reco), `data/mc_v3/pertrack_mc_v3_reco_djangoh14_merged.csv.gz`, `…_rapgap31_merged.csv.gz`, `…_gen_djangoh14_merged.csv.gz`, `…_gen_rapgap31_merged.csv.gz` | `data/response_4d_djangoh.npz`, `data/response_4d_rapgap.npz` | (a) sample, (b) binning, (d) fiducial. NOT τ-dependent. |
| `unfold_BBB_4d.py` | Unfold | **VITAL — FROZEN-CANONICAL** (baseline cross-check) | `data/response_4d_djangoh.npz` | `data/unfolded_4d_v3_BBB.json` | (a) sample, (b) binning. NOT τ-dependent. |
| `unfold_IBU_4d.py` | Unfold | **VITAL — FROZEN-CANONICAL** (primary positive-definite) | `data/response_4d_djangoh.npz` | `data/unfolded_4d_v3_IBU.json`, `data/unfolded_4d_v3_IBU_covariance.npz` | (a) sample, (b) binning. NOT τ-dependent (k auto-picked). |
| `unfold_TUnfold_4d.py` | Unfold | **VITAL — FROZEN-CANONICAL** (central method, τ=0.10) | `data/response_4d_djangoh.npz` | `data/unfolded_4d_v3_TUnfold.json`, `data/unfolded_4d_v3_TUnfold_covariance.npz` | (a) sample, (b) binning, (c) τ. |
| `_prep_mc_arrays_for_bootstrap.py` | UnfoldSyst (preprocess) | **VITAL — FROZEN-CANONICAL** | MC reco + gen CSVs | `data/_bootstrap_arrays_djangoh.npz`, `data/_bootstrap_arrays_djangoh_gen.npz` | (a) sample, (b) binning. One-shot preprocessor for `sigma_MCstat_TUnfold_4d.py`. |
| `sigma_MCstat_TUnfold_4d.py` | UnfoldSyst (σ_MCstat-response) | **VITAL — FROZEN-CANONICAL** | `_bootstrap_arrays_*.npz`, `response_4d_djangoh.npz`, `v5g_4d_trust_mask.json`, `sigma_reg_TUnfold_4d.json` | `data/sigma_MCstat_TUnfold_4d.json`, `data/mcstat_replicas_tau0p1/replica_{0..99}.json`, QA PDF | (a) sample, (b) binning, (c) τ. |
| `sigma_reg_TUnfold_4d.py` | UnfoldSyst (σ_reg) | **VITAL — FROZEN-CANONICAL** | `response_4d_djangoh.npz` | `data/sigma_reg_TUnfold_4d.json`, QA PDF | (a) sample, (b) binning, (c) τ (the grid is recentered on τ*). |
| `unfold_data_4d_DJ_vs_RG.py` | UnfoldSyst (σ_model) | **VITAL — FROZEN-CANONICAL** | `pertrack_v5g_fixedFrame_v3_merged.csv.gz`, `response_4d_{djangoh,rapgap}.npz`, `v5g_4d_trust_mask.json`, all nominal unfold JSONs (DJ reference) | `data/unfolded_4d_v3_data_DJvsRG.json`, QA PDF | (a) sample, (b) binning, (c) τ. Imports `TAU_FIXED` from `unfold_TUnfold_4d.py`. |
| `closure_4d_crossMC.py` | Diagnostic | OPTIONAL | both responses | `data/unfolded_4d_v3_closure_crossMC.json` | When response or τ changes; used as input to `diag_catastrophic_4D_bins.py`. |
| `closure_IBU_crossMC.py` | Diagnostic (sliced legacy) | ORPHAN — imports `unfold_IBU_v3` (sliced, quarantined); not part of 4D chain | (NAF / sliced inputs) | sliced closure PDF/JSON | Never under current 4D pipeline. Quarantine candidate. |
| `closure_TUnfold_crossMC.py` | Diagnostic (sliced legacy) | ORPHAN — imports `unfold_TUnfold_v3` (sliced, quarantined); not part of 4D chain | (NAF / sliced inputs) | sliced closure PDF/JSON | Never under current 4D pipeline. Quarantine candidate. |
| `plot_unfold4d_QA.py` | Plot | OPTIONAL | all 4D unfold JSONs + closure + compare JSONs | `plots/unfold4d_QA/*.pdf` | When unfolds or τ change. |

### 1.3 `scripts/systematics/`

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `emit_nominal_uncertainty_reference.py` | Systematics (bin grid bootstrap) | **VITAL — FROZEN-CANONICAL** | `data/xsec_v3_HQv5gBprime_2026-05-24.json` | `data/systematics_sources/source_00_nominal.json` | (b) binning. Run ONCE per binning change to seed the source pool. |
| `emit_source_lumi.py` | Systematics (normalization) | FROZEN-CANONICAL (CONCERN: `source_id="lumi"` should be `"luminosity"` per combiner) | `source_00_nominal.json`, `xsec_v3_*.json` | `data/systematics_sources/source_lumi.json` | (a) sample / when L_data value changes. Not τ-dependent. |
| `emit_source_trigger.py` | Systematics (normalization) | FROZEN-CANONICAL (placeholder 0.5%) | `source_00_nominal.json`, `xsec_v3_*.json` | `data/systematics_sources/source_trigger.json` | (a) sample. Not τ-dependent. |
| `emit_source_NucliaWeight.py` | Systematics (normalization) | FROZEN-CANONICAL (CONCERN: ±100% envelope decision pending) | nominal source + per-track CSV | `data/systematics_sources/source_NucliaWeight.json` | (a) sample, (b) binning. Not τ-dependent. |
| `source_tracking_chargedep.py` | Systematics (analytic detector) | **VITAL — FROZEN-CANONICAL** | `xsec_v3_*.json`, `source_00_nominal.json` | `data/systematics_sources/source_tracking_chargedep.json`, `cov_tracking_chargedep.npz` | (a) sample, (b) binning. Not τ-dependent (analytic). |
| `noise_floor_bootstrap.py` | Systematics (noise floor — per replica) | **VITAL — FROZEN-CANONICAL** (path bug fixed 2026-05-26) | `response_4d_djangoh.npz`, `_bootstrap_arrays_*.npz` (for `--mode mc`), data CSV (for `--mode data`) | One JSON per invocation: `data/noise_floor_replicas/noise_floor_{data,mc}_<r>.json` | (a) sample, (b) binning, (c) τ. Drive via Condor: 20 data + 20 mc = 40 invocations. |
| `compute_noise_floor.py` | Systematics (noise floor — aggregate) | **VITAL — FROZEN-CANONICAL** (path bug fixed 2026-05-26) | `data/noise_floor_replicas/noise_floor_{data,mc}_*.json` | `data/systematics_sources/noise_floor.json`, `cov_noise.npz` | After bootstrap replicas are produced. |
| `apply_noise_correction.py` | Systematics (de-double-count) | FROZEN-CANONICAL (path bug fixed 2026-05-26); has NOT yet been run at τ=0.10 (target dir `systematics_sources/corrected/` is empty) | Any `source_*.json` + `cov_*.npz` + `noise_floor.json` | `data/systematics_sources/corrected/source_*_corrected.json` + `cov_*_corrected.npz` | Once per source, after that source is produced at current τ. |
| `combine_systematics.py` | Combine | **VITAL — FROZEN-CANONICAL** | All `data/systematics_sources/source_*.json` | `data/systematics_combined_v3_HQv5gBprime.json` | After any source changes. **OUTSTANDING: must be re-run at τ=0.10 per handoff §10.1.** |
| `test_combine_smoke.py` | Test | OPTIONAL (CI-style smoke) | (creates fake sources, runs combiner, verifies math) | (assertion) | After any combiner change. |
| `submit_noise_floor.condor` | NAF submit file | SUPPORT | — | (Condor submission) | When dispatching `noise_floor_bootstrap.py` × 40. |
| `wrap_noise_floor.sh` | NAF wrap script | SUPPORT | — | (per-job env setup) | — |

### 1.4 `scripts/plotting/`

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `make_tau0p1_priority_plots.py` | Plot | **VITAL — FROZEN-CANONICAL** (the canonical 5-plot budget figure set) | all systematic source JSONs + nominal unfold JSONs + trust mask | `plots/sigma_per_bin_full_budget_tau0p1/*.pdf` (5 plots) | After any source or τ changes. |

### 1.5 `scripts/v3p2/` (v3p2 drivers — partially canonical)

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `diag_catastrophic_4D_bins_v3p2.py` | Trust | **CURRENT — FROZEN-CANONICAL** | All `unfolded_4d_v3final_*.json`, both `*_covariance.npz`, response NPZ | `data/v5g_4d_trust_mask_v3final.json` (367/480 trusted) | Every time nominal unfolds change. This produces the v3final trust mask. |
| `make_v3p2_priority_plots.py` | Plot | **CURRENT — FROZEN-CANONICAL** | Systematic source JSONs + nominal unfold JSONs + v3final trust mask | `plots/v3final_priority/*.pdf` | After any source or τ changes. |
| `build_data_release_v1p1.py` | Release | SUPERSEDED (by `scripts/v3final/build_data_release_v1p2_final.py`) | v3p2-era xsec, yields, trust mask, per-source covs | `data_release_v1p1/` | Do not rerun; v1.1 superseded by v1.2_final. |
| `build_prior_gen_full.py` | Diagnostic | DIAGNOSTIC — **wrong convention; do not use in canonical chain** | MC gen CSV | `data/prior_gen_full.npz` | Never. The prior IS the fiducial gen distribution; this script used a mistaken convention (full gen without Option C). `prior_gen_full.npz` is NOT used in the canonical chain. |
| `plot_quality_corrected_prior.py` | Plot | DIAGNOSTIC — **wrong convention; plots quarantined** | `data/prior_gen_full.npz` | `plots/prior_gen_full/` (quarantined) | Never. Companion to `build_prior_gen_full.py`; same mistaken convention. |

### 1.6 `scripts/v3final/` (v3final canonical drivers — all FROZEN-CANONICAL)

These 5 scripts are the new load-bearing chain for the canonical v3final result and data release
v1.2_final. They diverge from the generic `scripts/systematics/combine_systematics.py` contract
because the v3final source JSONs in `data/systematics_sources_v3final/` use an updated schema.

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `combine_systematics_v3final.py` | Combine | **FROZEN-CANONICAL** | 6 source JSONs + 4 unfolding-budget JSONs from `data/systematics_sources_v3final/` and `data/` | `data/systematics_combined_v3final.json` | When any source changes. |
| `apply_noise_correction_v3final.py` | Combine | **FROZEN-CANONICAL** | `data/systematics_combined_v3final.json` + `data/systematics_sources_v3final/noise_floor.json` | `data/systematics_combined_v3final_noise_corrected.json` | When noise floor or combined changes. σ_sys_corr = sqrt(max(σ_sys² − 2σ_noise², 0)). |
| `compute_xsec_v3final.py` | Release | **FROZEN-CANONICAL** | `data/unfolded_4d_v3final_TUnfold.json` + `data/response_4d_djangoh_v3final.npz` (bin widths) + L = 351,000.0 nb⁻¹ (current canonical four-period quote; supersedes the older direct-monitor 356,613 nb⁻¹) | `data/xsec_v3final.json` | When unfolded yields change. Units: nb·GeV⁻³. |
| `compute_Ah_v3final.py` | Release | **FROZEN-CANONICAL** | `data/unfolded_4d_v3final_TUnfold.json` + per-source covariance NPZs (cancels_in_A_h logic) | `data/Ah_v3final.json` (240 bins total; 181 in published set) | When unfolded yields or systematics change. |
| `build_data_release_v1p2_final.py` | Release | **FROZEN-CANONICAL** | `data/xsec_v3final.json`, `data/Ah_v3final.json`, `data/v5g_4d_trust_mask_v3final.json`, `data/systematics_combined_v3final_noise_corrected.json`, per-source cov NPZs, schema/examples from `data_release_v1p1/` | `data_release_v1p2_final/` (21 files, 41 MB) | When any input changes. |

---

## 2. NAF scripts: `H1ChargedSidis_HighQ2/scripts/`

### 2.1 Top-level (NAF)

The NAF top-level dir contains a large number of v5f-era extractors and submit wrappers that are SUPERSEDED but were not moved to a NAF-side quarantine (the 2026-05-26 cleanup only moved the local copies). Treat the top-level NAF dir as legacy unless a script appears in the table below.

| Script | Stage | Status | Notes |
|---|---|---|---|
| `submit_full_production.py` | Production driver (Stage 1 + Stage 2) | **VITAL — FROZEN-CANONICAL** | The current production wrapper. Embeds the per-period GRL dispatch fix and the year-06 chunk boundary at 220. Run only when re-doing Stage 1+2 production from raw HAT inputs. |
| `submit_sidistree_highQ2_batch.sh` | Production helper | FROZEN-CANONICAL (called by Stage 2 dispatch) | — |
| `merge_data_highQ2.sh`, `merge_mc_highQ2.sh` | Production merge | FROZEN-CANONICAL | Per-period merge of Stage 2 sidistree ROOT files. |
| `_beam_energy_utils.py`, `_binary_guard.py`, `_smoke_gate.py`, `binary_identity_manifest.json` | Helper / Guardrail | Same as local copies | Synced copies. |
| `verify_beam_energy_usage.py` | Guardrail (audit) | OPTIONAL | One-shot audit script for the beam-energy policy. |
| Everything else in NAF top-level (`abc_dis_*`, `acceptance_v5f_*`, `analyze_sidis_*`, `assemble_*`, `binning_v5f_*`, `compute_data_hadron_yields_v5f.py`, `compute_highQ2_crad_*.py`, `em06_postrecovery_extract.py`, `extract_data_yields_v5f_*`, `extract_phi*`, `extract_yields_v5f_3period.py`, `migration_matrices_v5f.py`, `n_dis_data_v5f_highQ2.py`, `parse_britzger_norad_lumi.py`, `phi_gap_diag_extract.py`, `plot_resolution_sliced_HQv5fr2.py`, `pull_results.sh`, `study_*`, `submit_crad_extract_condor.py`, `submit_em06_g15_recovery.py`, `submit_highQ2_sidis_data.py`, `submit_norad_charged_sidis_highQ2.{py,sub}`, `submit_phi_gap_diag_condor.py`, `submit_phi_yields_v5f_condor.py`, `systematics_v5f_nuclia_proton_omission.py`, `task_W_multiplicity_highQ2.py`, `_unfold_compare_v5f_extract.py`, `crad_strategy2/`, `scan_TUnfold/`) | Various v5f extractors / analyzers | SUPERSEDED-on-disk | All pre-v5g / sliced / pre-Scheme-B′. Should be moved to `quarantine/scripts_naf_legacy/`. |
| `perbin_LAr_HFS_beam_shifts.py` (NAF copy) | (orphan) | ORPHAN | Same status as local copy; HQ-v5f-r1 binning. |
| `binning_proposal_2026-05-24.py` | Binning study | FROZEN-ONE-SHOT | Produced Scheme B′ proposal; do not re-run. |

### 2.2 `scripts/extract_pertrack_v5g_fixedFrame/`  (data per-track extractor)

| Script | Stage | Status | Notes |
|---|---|---|---|
| `extract_pertrack_v5g_fixedFrame_v3.py` | Extract (data) | **VITAL — FROZEN-CANONICAL** | Current canonical per-track CSV extractor. Adds γ\*+p CM-frame `phi_h` (and `pT_h` from v2 baseline). Output schema = 18 columns. This produces the `pertrack_v5g_fixedFrame_v3_merged.csv.gz` that is the local input to `build_response_4d.py`. |
| `merge_pertrack_v5g_fixedFrame_v3.py` | Extract (merge) | **VITAL — FROZEN-CANONICAL** | Stream-merges per-file CSVs into `pertrack_v5g_fixedFrame_v3_merged.csv.gz`. |
| `submit_pertrack_v5g_fixedFrame_v3.condor`, `wrap_pertrack_v5g_fixedFrame_v3.sh`, `periods/` | NAF dispatch | FROZEN-CANONICAL | Condor dispatch for the v3 extractor. |
| `extract_pertrack_v5g_fixedFrame_v3p1.py` + matching `merge_*_v3p1.py` + `submit_*_v3p1.condor` + `wrap_*_v3p1.sh` | Extract (v3p1) | OPTIONAL — additive 8-column extension of v3 (`scatEX, scatEY, scatEZ, scatEE, Phi_lab, BeamElecE, BeamProtP, Empz`); required ONLY for the migration-affecting electron-shift systematic on NAF (`source_electron_shift.py`) | Use v3p1 when the analysis chain needs the scattered-electron 3-vector at per-track granularity. Otherwise v3 is sufficient. |
| `extract_pertrack_v5g_fixedFrame.py`, `extract_pertrack_v5g_fixedFrame_v2.py` (+ matching `merge_*`, `submit_*`, `wrap_*`) | Extract (v1, v2) | SUPERSEDED-on-disk | v1 = lab-frame (broken); v2 = pT_h fixed but phi_h still broken. Both retained for audit history only. Quarantine candidate. |

### 2.3 `scripts/extract_pertrack_mc_v3/`  (MC per-track extractors)

| Script | Stage | Status | Notes |
|---|---|---|---|
| `extract_pertrack_mc_reco_v3p1.py` | Extract (MC reco) | **VITAL — FROZEN-CANONICAL** | Additive 8-column extension of v3. v3p1 is the live target; v3 is superseded. |
| `extract_pertrack_mc_gen_v3p2.py` | Extract (MC gen, Option C fiducial) | **FROZEN-CANONICAL** — Option C v3final production complete 2026-05-28 (501 djangoh + 364 rapgap files merged; 4.9+4.1 GB gen CSVs on NAF; symlinked as v3final). | Drops GenPt>0.05, widens |GenEta|<4.0, adds xFGen>0 (Trento convention sign-flipped) and 35<GenEmpz<70 cuts. |
| `extract_pertrack_mc_disevent_v3p1.py` | Extract (MC disevent) | **VITAL — FROZEN-CANONICAL** | Additive 4-column extension of v3 with scatE\* branches. |
| `merge_pertrack_mc_v3p1.py` | Extract (merge) | **VITAL — FROZEN-CANONICAL** | 4 outputs: `pertrack_mc_v3p1_{reco,disevent}_{djangoh14,rapgap31}_merged.csv.gz`. |
| `submit_pertrack_mc_v3p1.condor`, `wrap_pertrack_mc_v3p1.sh` | NAF dispatch | FROZEN-CANONICAL | — |
| `extract_pertrack_mc_reco_v3.py`, `extract_pertrack_mc_gen_v3.py`, `extract_pertrack_mc_disevent_v3.py`, `merge_pertrack_mc_v3.py`, `submit_pertrack_mc_v3.condor`, `wrap_pertrack_mc_v3.sh` | Extract (v3) | SUPERSEDED-on-disk | Replaced by v3p1 (reco/disevent) and v3p2 (gen, in progress). However: the **current `data/mc_v3/` merged CSVs are still v3 outputs**, so v3 remains "live in the artifact pool" until the v3p1/v3p2 merges land. Do not quarantine until that switchover. |

### 2.4 `scripts/unfold4d/`  (NAF mirror of local 4D unfold tree)

| Script | Stage | Status | Notes |
|---|---|---|---|
| `build_response_4d.py`, `_unfold_4d_common.py`, `unfold_BBB_4d.py`, `unfold_IBU_4d.py`, `unfold_TUnfold_4d.py`, `closure_4d_crossMC.py`, `plot_unfold4d_QA.py` | Mirrors of local files | FROZEN-CANONICAL (mirror) | These exist on NAF so the systematics drivers (which run on NAF) can `import` them. Keep in sync with `H1/sidis_highQ2/scripts/unfolding/`. The NAF `unfold_TUnfold_4d.py.tau0p01_backup` is the pre-τ-change archive copy. |
| `compare_4d_vs_sliced.py` | Diagnostic (one-shot) | FROZEN-ONE-SHOT (sliced is gone; pure audit) | Produces `data/unfold4d_vs_sliced_comparison.json`. Do not re-run. |
| `run_build_response_4d.sh`, `submit_build_response_4d.condor` | Dispatch helpers | OPTIONAL | Use when building R on Condor instead of locally. |
| `tau_scan_diag/` | Diagnostic | FROZEN-ONE-SHOT | The extended τ-scan that motivated τ=0.10. |

### 2.5 `scripts/systematics/` (NAF — shifted-response + electron + tracking)

| Script | Stage | Status | Inputs | Outputs | When to rerun |
|---|---|---|---|---|---|
| `source_tracking_efficiency.py` | Systematics (build shifted response) | **VITAL — FROZEN-CANONICAL** | MC reco CSV | shifted `response_tracking_eff_{up,down}.npz` on NAF | (a) sample, (b) binning, (d) fiducial. Not τ-dependent. |
| `source_tracking_resolution.py` | Systematics (build smeared response) | **VITAL — FROZEN-CANONICAL** | MC reco CSV | smeared `response_tracking_res.npz` (one seed) | (a) sample, (b) binning, (d) fiducial. |
| `source_electron_shift.py` | Systematics (shifted CSV) | **VITAL — FROZEN-CANONICAL** | v3p1 per-track CSV (data + MC reco) | 6 shifted CSVs (E up/down, θ up/down, φ up/down) | (a) sample, (d) fiducial. Requires v3p1 (needs scatE* and Empz). |
| `run_tracking_efficiency_systematic.py` | Systematics (re-unfold driver) | **VITAL — FROZEN-CANONICAL** | nominal unfolded JSON + up/down shifted response NPZ | `source_tracking_efficiency.json`, `cov_tracking_efficiency.npz` (consumed locally) | (a) sample, (b) binning, (c) τ, (d) fiducial. |
| `run_tracking_resolution_systematic.py` | Systematics (re-unfold driver) | **VITAL — FROZEN-CANONICAL** | nominal unfolded JSON + smeared response NPZ | `source_tracking_resolution.json`, `cov_tracking_resolution.npz` | (a) sample, (b) binning, (c) τ, (d) fiducial. |
| `run_electron_systematic.py` | Systematics (re-unfold driver, 3-in-1) | **VITAL — FROZEN-CANONICAL** | nominal unfolded JSON + up/down shifted unfold outputs | `source_electron_{E,Theta,Phi}.json`, matching `cov_*.npz` | (a) sample, (b) binning, (c) τ, (d) fiducial. |
| `noise_floor_bootstrap.py`, `compute_noise_floor.py`, `apply_noise_correction.py` | Same as local copies | FROZEN-CANONICAL (mirror) | — | — | The NAF copies are the ones Condor actually executes; sync local ↔ NAF on edits. |
| `source_tracking_chargedep.py` | Systematics (analytic, local-runnable) | FROZEN-CANONICAL (mirror of local copy) | — | — | Either copy works; analytic source. |
| `submit_*.condor`, `wrap_*.sh` (`tracking_efficiency`, `tracking_resolution`, `electron_systematic`, `noise_floor`) | Dispatch | FROZEN-CANONICAL | — | — | — |
| `*.tau0p01_backup` | Archive | ARCHIVE | — | — | Do not invoke. Pre-τ-change snapshots, retained for audit. |

---

## 3. Gaps and orphans flagged for the supervisor

### 3.1 Orphans (claim a canonical role but evidence does NOT support it)

1. **`scripts/unfolding/closure_IBU_crossMC.py`** — imports `unfold_IBU_v3` (sliced, in `quarantine/scripts_unfolding_sliced/`). Cannot run under the current 4D pipeline.
2. **`scripts/unfolding/closure_TUnfold_crossMC.py`** — imports `unfold_TUnfold_v3` (sliced, quarantined). Same problem.
3. **`scripts/perbin_LAr_HFS_beam_shifts.py`** (and NAF copy) — header says HQ-v5f-r1 binning; current pipeline uses HQ-v5g-Bprime. Output `data/perbin_LAr_HFS_beam.json` is pre-v5g.
4. **`scripts/_extract_y0p7_v5g_perevent.py`** — 3-period sibling of the 4-period extractor; the 4-period extractor superseded it.

### 3.2 Release builder (gap closed for v1.2_final)

**`scripts/v3final/build_data_release_v1p2_final.py` now exists** and produced
`data_release_v1p2_final/` (21 files, 41 MB). The earlier v1.0 `data_release/` package was
assembled ad hoc and has no builder; the v1.1 builder is `scripts/v3p2/build_data_release_v1p1.py`
(now SUPERSEDED). The gap noted in the previous version of this document is resolved for v1.2_final
and later. The v1.0 assembly is not reproducible from scripts in the repo.

### 3.3 Pending items (Phase 2 systematics / v3final follow-on)

The v3final chain (§1.6) is complete and canonical. Remaining open items are Phase 2 systematics
for the next release:

- `combine_systematics.py` (generic combiner) — still has the `source_id="lumi"` vs.
  `"luminosity"` schema mismatch; not relevant to the v3final chain (which uses
  `combine_systematics_v3final.py`) but will need fixing before Phase 2 sources are added.
- HFS source not yet emitted (research memo exists; script does not). Phase 2 follow-on.
- Trigger and lumi-norm systematics deferred to Phase 2.

---

## 4. The irreplaceable chain (v3final canonical)

For the **canonical v3final result**, the load-bearing scripts are the 10 below plus the 5 v3final
drivers in §1.6 and the v3p2 trust-mask script in §1.5. If you only had time to read these, you
have the full chain:

1. NAF `extract_pertrack_v5g_fixedFrame/extract_pertrack_v5g_fixedFrame_v3p2.py` (+ merge) → data CSV
2. NAF `extract_pertrack_mc_v3/extract_pertrack_mc_{gen_v3p2,reco_v3p1,disevent_v3p1}.py` (+ merge) → 6 MC CSVs
3. Local `scripts/unfolding/build_response_4d.py` → `response_4d_{djangoh,rapgap}_v3final.npz`
4. Local `scripts/unfolding/unfold_TUnfold_4d.py` (central τ=0.10) → `unfolded_4d_v3final_TUnfold.{json,covariance.npz}`
5. Local `scripts/unfolding/unfold_IBU_4d.py` (cross-check) → `unfolded_4d_v3final_IBU.{json,covariance.npz}`
6. Local `scripts/unfolding/unfold_BBB_4d.py` (baseline) → `unfolded_4d_v3final_BBB.json`
7. Local `scripts/unfolding/sigma_reg_TUnfold_4d.py` + `sigma_MCstat_TUnfold_4d.py` + `unfold_data_4d_DJ_vs_RG.py` → 4-term unfolding budget
8. NAF `scripts/systematics/{source_tracking_efficiency,source_tracking_resolution,source_electron_shift}.py` + `run_*_systematic.py` → 6 detector source JSONs + covariance NPZs
9. Local `scripts/systematics/{noise_floor_bootstrap,compute_noise_floor}.py` + **`scripts/v3final/combine_systematics_v3final.py`** + **`scripts/v3final/apply_noise_correction_v3final.py`** → noise floor + master combined + noise-corrected JSON
10. Local **`scripts/v3p2/diag_catastrophic_4D_bins_v3p2.py`** → `data/v5g_4d_trust_mask_v3final.json` (367/480 trusted)
11. Local **`scripts/v3final/compute_xsec_v3final.py`** → `data/xsec_v3final.json`
12. Local **`scripts/v3final/compute_Ah_v3final.py`** → `data/Ah_v3final.json`
13. Local **`scripts/v3final/build_data_release_v1p2_final.py`** → `data_release_v1p2_final/` (21 files, 41 MB)

Everything else is either a helper imported by one of these, a one-shot binning/τ study whose
output is frozen, a diagnostic, or a plotting script.

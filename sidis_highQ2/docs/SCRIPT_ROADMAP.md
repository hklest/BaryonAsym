# Script roadmap — H1 high-Q² charged-particle SIDIS

**Last updated:** 2026-06-24

This is the human-readable map through the high-Q² SIDIS script forest. It separates the **current analysis chain** from diagnostic, exploratory, and superseded scripts. If a fresh agent needs to rerun the current analysis, start here and `docs/RUN_STATUS.md`.

> **Current status note:** The authoritative current release state is in `docs/RUN_STATUS.md`; as of 2026-06-24 the working z005 release is the IBU/BBB finalcandidate chain and DJANGOH14 gen-only `C_rad` v1 is the stable provisional radiative-correction baseline, while final release remains blocked by sparse-bin policy, publication-grade DJ MC-lumi provenance, and RAPGAP quarantine resolution/decision. The standard TUnfold entry point for z005 diagnostics/candidate comparisons is native ROOT/TUnfold v6 miss-underflow at `scripts/unfolding/tunfold_official_v6_native_miss_z005/`. Older TUnfold v4/v5 and rev6 AREA/final-bin text in this roadmap is retained as historical/audit context unless explicitly re-promoted. RooUnfoldSvd/TSVDUnfold is quarantined for the high-Q² charged-SIDIS z005 analysis because its 1D SVD regularization is not topology-safe for our multidimensional flattened bin array; do not use SVD outputs/scripts for method comparisons or validation.

---

## 0. Read-first documents

| File | Purpose |
|---|---|
| `docs/RUN_STATUS.md` | current canonical status, open blockers, output locations |
| `docs/LESSONS_LEARNED.md` | structural traps and policy lessons |
| `docs/results/final_binning_candidate_2026-06-03.md` | current 157-bin pT-merged release candidate |
| `docs/results/tunfold_area_210bin_release_2026-06-02.md` | 210-bin TUnfold AREA release construction |
| `docs/critiques/critique-finalbin-candidate-2026-06-03.md` | final-bin critic review |

---

## 1. Current production / unfolding chain

> **Diagnostic/development note:** the active TUnfold diagnostic machinery for high-Q² z005 starts from the native ROOT/TUnfold v6 miss-underflow package, not from v4/v5. Treat all TUnfold outputs as diagnostics unless a run-status or release note explicitly promotes a specific product; central z005 remains IBU/BBB finalcandidate pending nominal-method choice.

### 1.1 Stage-2 tree source (H1OO)

| Path | Role | Status |
|---|---|---|
| `src/ChargedSidisTreeBuilderHighQ2_v5g.C` (NAF) | current stage-2 sidistree builder | canonical input trees already produced |
| `scripts/audit/audit_tree_branches_v5g.py` | branch-size audit | current diagnostic; supports future v5h trimming |
| `scripts/audit/build_audit_markdown.py` | converts branch audit JSON to Markdown | current diagnostic |

**Do not re-run stage-2 unless changing tree schema/systematics production.** For current unfolding, use existing `output/mc/` and `output/data/` sidistrees.

### 1.2 Response matrix / data input build

| Script | Role | Current? | Notes |
|---|---|---|---|
| `scripts/unfolding/tunfold_official_v3_percharge/binning_v3.py` | Scheme B′ base binning: 240 gen, 960 reco | **current base** | 8 cells × 6z × 5pT gen; reco split 2×2 in z,pT |
| `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_NWfix_v2.py` | current response builder with F3–F7 chain-audit fixes | **current** | V0-not-fake, GenPt>0.15, zGen range, 8-dec match key, gen charge for matched |
| `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_NWfix_v2.py` | current merge to `migration_inputs_v3_percharge_NWfix_v2.root` | **current** | includes NWeff priors |
| `scripts/unfolding/tunfold_official_v3_percharge/submit_build_migration_per_charge*.condor` / `wrap_*` | Condor build wrappers | current variants exist | use `_NWfix_v2` variant for any rebuild |

Canonical response ROOT:
```text
output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge_NWfix_v2.root
```

### 1.3 TUnfold diagnostics / candidate comparisons

| Script / directory | Role | Current? | Notes |
|---|---|---|---|
| `scripts/unfolding/tunfold_official_v6_native_miss_z005/` | native ROOT/TUnfold v6 miss-underflow package | **standard TUnfold entry point** | use for high-Q² z005 diagnostics/candidate comparisons; diagnostic-only, not promoted |
| `build_native_miss_inputs_v6.py` | builds native miss-underflow inputs | **current TUnfold diagnostic** | matched response in ordinary 490×245 bins; misses in ROOT reco-axis underflow |
| `validate_native_miss_inputs_v6.py` | validates native geometry/input package | **current TUnfold diagnostic** | geometry PASS; ensure no measured miss bin and no fake truth column |
| `unfold_native_miss_single_tau_v6.C` | real ROOT `TUnfoldDensity` single-tau macro | **current TUnfold diagnostic** | `SetInput` receives 490 physical reco bins; status must be 0 |
| `compare_native_tau0_vs_ibu_v6.py` | compares native tau0 output to central IBU | **current TUnfold diagnostic** | no post-hoc `1/eps`; tau0/IBU medians are near unity |
| `produce_phase2_native_diagnostics_v6.py`, `finalize_native_phase2.py`, `regenerate_tau0_native_roots.py` | phase-2 diagnostics, packaging, tau0 ROOT regeneration | **current TUnfold diagnostic** | use with LCG_109a ROOT 6.38.04 |
| `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C` | older known-working TUnfold macro | audit/reference | not the z005 standard entry point |
| `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau_rev6.C` | older rev6 AREA macro | historical/audit | superseded for z005 TUnfold entry by native v6 miss-underflow |

Important native v6 outputs:
```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v6_native_miss_z005_20260621/
/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/tunfold_v6_native_miss_z005_2026-06-21/
```

Core convention: use `TUnfoldDensity`; fill misses in reco/input-axis underflow for `kHistMapOutputVert`; do not pass a miss bin to `SetInput`; do not apply post-hoc `1/eps` to central values; keep v5 external fake-subtraction covariance caveat visible.

Historical outputs retained for audit only:
```text
output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/
output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/
output/tunfold_tree_v3_percharge/tau_scan_rev6/
output/tunfold_tree_v3_percharge/tau_scan_rev6_fine_1em6_1em2/
output/tunfold_tree_v3_percharge/closure_rev6_area/
output/tunfold_tree_v3_percharge/closure_rev6_area_tauopt/
```

### 1.4 S3 rescue and final binning

| Script | Role | Current? | Notes |
|---|---|---|---|
| `scripts/analysis/build_210bin_tunfold_release.py` | builds 210-bin TUnfold AREA release | current candidate generator | cells 0–5 + S3 x-merged |
| `scripts/analysis/build_finalbin_release_ptmerge.py` | pT-only merge/drop workflow; builds final-bin candidate | **current final-binning driver** | produced 157-bin common candidate |
| `scripts/analysis/build_cell_sliced_S3_tunfold_inputs.py` | individual S3a/S3b slice inputs | diagnostic | used for S3 rescue tests |
| `scripts/analysis/build_S3merged_tunfold_inputs.py` | S3a+S3b x-merged input | diagnostic/current for S3 design |
| `scripts/analysis/finalize_cell_sliced_S3.py` | S3 cell-sliced finalization | diagnostic |
| `scripts/analysis/finalize_S3merged.py` | S3merged finalization | diagnostic/current |
| `scripts/analysis/compare_S3_area_vs_none.py` | S3 AREA vs None comparison | diagnostic; supports Area preference |
| `scripts/analysis/select_S3_tau_multiobjective.py` | multi-objective S3 tau selector | **current S3 tau policy** | stat/reg/model/rho/negative-bin score |
| `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau_cellsliced.C` | official TUnfold macro for 30-bin S3/cell slices, no area | current diagnostic |
| `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau_cellsliced_area.C` | official TUnfold macro for 30-bin S3/cell slices, area | current diagnostic |

Current release outputs:
```text
data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/
data_release_rev6_TUnfold_AREA_210bin_INTERNAL_PRELIMINARY/
data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/
plots/release_210_filtered/
plots/final_binning_candidate/
```

### 1.5 Uncertainty and systematics diagnostics

| Script | Role | Current? | Notes |
|---|---|---|---|
| `scripts/analysis/per_bin_sigma_tunfold_rev6_area.py` | TUnfold AREA σ_stat/σ_reg/σ_model/σ_total | **current** | central uncertainty table for AREA branch |
| `scripts/analysis/build_data_side_systematics.py` | data-side shifted input vectors | current diagnostic/stress-test | elecE is posthoc-frame-only, provisional |
| `scripts/diagnostics/_stat_audit_dump_remote.py` | NAF dump for stat audit | current diagnostic |
| `scripts/diagnostics/build_stat_uncertainty_audit.py` | builds stat uncertainty audit PDF/JSON | current diagnostic |
| `scripts/plotting/plot_uncertainty_pth_panels.py` | pT panels of data-side + unfolding uncertainties | current QA |
| `scripts/plotting/plot_unfolding_uncertainty_sources_sidis.py` | Lambda-style uncertainty source plots | current QA |
| `scripts/unfolding/tunfold_official_v3_percharge/finalbin_toys_djangoh/toy_unfold_rev6_area_djangoh.C` | 100-toy DJANGOH TUnfold toy macro | current diagnostic |
| `scripts/unfolding/tunfold_official_v3_percharge/finalbin_toys_djangoh/aggregate_and_plot_toys.py` | aggregates/plots toy distributions | current diagnostic |

### 1.6 historical v4/v5 TUnfold diagnostic chains

> **SVD quarantine:** RooUnfoldSvd/TSVDUnfold is quarantined for the high-Q² charged-SIDIS z005 analysis because its 1D SVD regularization is not topology-safe for our multidimensional flattened bin array; do not use SVD outputs/scripts for method comparisons or validation. Any SVD-related scripts or outputs are historical/audit artifacts only and should be treated as **QUARANTINED / DO NOT USE** for active work.

This section documents the historical role of the high-Q² v4/native-fine and v5 physical/fake-sub workflows. Do **not** use them as the current TUnfold entry point; use native v6 miss-underflow instead. v4 ordinary fake/miss-bin encoding is invalid, and v5 is a visible-TUnfold plus external `x/eps` hybrid retained for audit.

#### High-level historical v4 flow

1. Build per-file migration matrices from sidistrees.
2. Build native fine-reco data vectors directly on the fine reco axis.
3. Merge response + data into a single ROOT input with Encoding-B semantics.
4. Run TUnfold nominal probes, tau scans, and closure summaries.
5. Run BBB / IBU / TUnfold cross-checks from the merged response and data inputs.
6. Derive model uncertainty from cross-generator closure or method-comparison products.
7. Build shifted-response or shifted-data-vector systematics.
8. Finalize diagnostic releases, comparison plots, and per-source deltas.

#### Core percharge workflow (`scripts/unfolding/tunfold_official_v4_percharge/`)

| Script | Role | Notes |
|---|---|---|
| `build_migration_th2_per_charge_v4.py` | Builds per-file per-charge migration matrices from sidistrees. | Nominal v4 response builder. |
| `build_data_vector_v4.py` | Builds the native fine-reco data vector. | Use native fine reco; do not uniformly spread one coarse-bin value over fine reco children. |
| `merge_migration_per_charge_v4.py` | Merges migration fragments and injects the data vector into the combined ROOT. | Historical Encoding-B body + ordinary fake column + ordinary miss row; now invalid for current ROOT/TUnfold entry. |
| `unfold_tunfold_single_tau_v4.C` | Runs one TUnfold solve for a chosen tau and charge. | Base executable for smoke tests, tau probes, and wrappers. |
| `wrap_tau_scan_v4.sh` / `submit_tau_scan_v4.condor` | Launches tau scans on Condor. | Wrapper-configured paths are preferred over hardcoded ad hoc destinations. |
| `summarize_tau_scan_v4.py` | Summarizes tau-scan outputs into diagnostic summaries and plots. | Use for tau-choice diagnostics, not by itself as physics approval. |
| `crosscheck_unfold_methods_v4.py` | Compares BBB, IBU, and TUnfold from a common merged response/data input. | BBB and IBU remain diagnostic cross-checks unless separately validated for promotion. |
| `shifted_electron_kinematics_v4.py` | Builds shifted-electron reco-side inputs for electron-kinematic systematics. | Post-hoc reco-side response rebuild path, not a full new central production. |
| `build_shifted_response_data_vector_v4.py` | Produces shifted response/data-vector inputs consumed by systematic reruns. | Used for systematic deltas from shifted reco kinematics. |
| `finalize_scalar_q_systematics_v4.py` | Finalizes scalar-q and related systematic deltas into summary products. | Scalar-q is a diagnostic alternate photon construction, not a default central observable definition. |

#### Supported variant directories

These sibling directories reuse the same script roles with variant-specific binning or acceptance choices:

| Directory | Variant purpose |
|---|---|
| `tunfold_official_v4_z2tmd_2xpt_percharge/` | z2 TMD-style diagnostic variant with 2× reco pT refinement. |
| `tunfold_official_v4_z2tmd_4x_percharge/` | z2 TMD-style diagnostic variant with 4× reco refinement. |
| `tunfold_official_v4_z2tmd_lowpt_2xpt_percharge/` | Low-pT diagnostic emphasis for z2 TMD studies. |
| `tunfold_official_v4_zlt0p7_4x_percharge/` | z < 0.7 diagnostic variant. |
| `tunfold_official_v4_z2tmd_lowpt_2xpt_eta1p6_radlen5_nhit5_percharge/` | Stricter diagnostic low-pT / track-quality study variant. |

#### Important historical v4/v5 conventions

- **Native fine-reco data vectors were canonical within the v4 workflow** but do not make v4 a valid current TUnfold implementation.
- **Ordinary fake/miss-bin encoding is invalid for current ROOT/TUnfold.** v4's body + fake-column + miss-row convention is retained only to interpret archived files; future TUnfold work must put misses in ROOT reco-axis underflow and keep fakes out of the truth/output axis.
- **v5 physical/fake-subtracted diagnostics are audit-only.** They fixed the ordinary-bin pathology but solved visible/matched scale and used external `x_visible/eps`; do not reuse them as the standard TUnfold entry point.
- **Path policy:** central nominal inputs live under the configured `central` roots; systematic outputs should be routed under `output_v6/systematics/...` or the wrapper-configured systematic root rather than mixed into central output areas.
- **Reco-method side-study policy:** for electron/IΣ z005 studies, follow `docs/systematics/reco_method_side_study_runbook_2026-06-20.md`; both `build_migration_th2_per_charge_v4.py` and `build_data_vector_v4.py` or their study copies must pass the selected `--reco-method` into their worker functions and recompute reco `z`, `pT_h`, and `xF` from the method-specific q.
- **Diagnostic caveat:** many current v4 products are raw, not background-subtracted, and may not include final `C_rad`; say this explicitly when consuming them.
- **Shifted-electron studies are reco-side post-hoc variations.** They are useful systematic probes but do not replace a full rederived central production.
- **Scalar-q products are alternate-construction diagnostics.** Do not present them as the default release unless a later decision note says so.

### 1.7 Publication/release plots

| Script | Role | Current? |
|---|---|---|
| `scripts/plotting/plot_tunfold_rev6_tauopt_model_compare.py` | RG vs DJ pT comparison at tauopt | current QA |
| `scripts/plotting/plot_uncertainty_pth_panels.py` | uncertainty pT panels | current QA |
| `scripts/plotting/plot_unfolding_uncertainty_sources_sidis.py` | uncertainty source / dominant source plots | current QA |
| ad-hoc script inside `build_finalbin_release_ptmerge.py` | final-bin pT overlay and status maps | current release plots |

---

## 2. Diagnostic-only or exploratory scripts

These are useful but not part of the current production chain.

| Script | Purpose | Status |
|---|---|---|
| `scripts/analysis/compute_bbb_rev6_v2.py` | BBB diagnostic with rev6_v2 inputs | diagnostic only; BBB not central |
| `scripts/analysis/compute_bbb_matrix_v2.py` | matrix-BBB / EM-n=0 diagnostic | diagnostic only; not headline |
| `scripts/analysis/method_compare_rev6.py` | method comparison plots | diagnostic only |
| `scripts/analysis/closure_rev6_v2.py` | IBU/BBB closure suite | diagnostic; not central |
| `scripts/analysis/ibu_sigma_reg_scan.py`, `tunfold_sigma_reg_scan.py`, `reg_strength_comparison.py` | reg-strength historical studies | diagnostic/history |
| `scripts/analysis/tau_selection_metrics.py`, `per_bin_systematics_at_tau.py` | rev5/rev6 transition studies | diagnostic/history |
| `scripts/analysis/plot_pT_h_per_cell.py` | old pT panel plotter | diagnostic/history; use final-bin release plots now |
| `scripts/diagnostics/charge_confusion_v3.py` | charge confusion diagnostic | useful for future σ_chargeconf |

---

## 3. Superseded / historical scripts

Do not use these for current central results unless explicitly reviving a historical comparison.

| Area | Scripts / dirs | Why superseded |
|---|---|---|
| pre-rev6 4D unfolding | `scripts/unfolding/build_response_4d.py`, `unfold_IBU_4d.py`, `unfold_TUnfold_4d.py`, `unfold_BBB_4d.py`, `closure_4d_crossMC.py`, etc. | replaced by TUnfold AREA 210-bin workflow |
| old TUnfold official v1/v2 | `scripts/unfolding/tunfold_official_v2/` | replaced by v3_percharge/NWfix_v2 |
| v3final/v3p2 release scripts | `scripts/v3final/`, `scripts/v3p2/` | pre-rev6 historical release path |
| IBU production scripts | `scripts/ibu/` | diagnostic only now; do not use for central |
| OmniFold | `scripts/omnifold/` | experimental; not current central |
| superseded QA plotters | `scripts/analysis/_quarantine/2026-06-01_superseded_unfolding_QA/`, `scripts/scan_TUnfold/_quarantine/2026-06-01_superseded_unfolding_QA/` | replaced by `postprocess_single_tau.py` diagnostic view |
| invalid q2 proxy | `scripts/analysis/_quarantine/2026-06-01_invalid_q2sliced_python_proxy/` | invalid Python/Tikhonov proxy; do not use |

---

## 4. Scripts needed for a cold rerun of the current chain

Minimal order (assuming v5g sidistrees already exist):

1. **Build response**
   ```bash
   condor_submit scripts/unfolding/tunfold_official_v3_percharge/submit_build_migration_per_charge_NWfix_v2.condor
   python3 scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_NWfix_v2.py ...
   ```
2. **Run TUnfold diagnostics/candidate comparisons**
   ```bash
   # Use the native v6 miss-underflow package, with LCG_109a ROOT 6.38.04.
   # Do not start from v4 ordinary fake/miss bins or v5 visible+external-eps diagnostics.
   scripts/unfolding/tunfold_official_v6_native_miss_z005/
   ```
3. **Run closure tau optimization / promotion gates**
   ```bash
   python3 scripts/unfolding/tunfold_official_v3_percharge/summarize_closure_tauopt.py ...
   ```
   (If missing locally, recover from NAF; this script created `closure_tauopt_summary.json`.)
4. **Build TUnfold per-bin sigma**
   ```bash
   python3 scripts/analysis/per_bin_sigma_tunfold_rev6_area.py ...
   ```
5. **Build 210-bin and final-bin release**
   ```bash
   python3 scripts/analysis/build_210bin_tunfold_release.py ...
   python3 scripts/analysis/build_finalbin_release_ptmerge.py ...
   ```
6. **Make final plots**
   Current outputs live in `plots/release_210_filtered/`, `plots/final_binning_candidate/`, and `plots/unfolding_uncertainty_sources_sidis/`.

---

## 5. Branch trimming note

`docs/results/tree_branch_audit_v5g_2026-06-03.md` identifies ~47–54% file-size savings for future systematic production. If producing a trimmed v5h builder, keep all branches required by:
- `build_migration_th2_per_charge_NWfix_v2.py`
- `build_data_side_systematics.py`
- planned track/electron systematics

Do not drop diagnostic/systematics branches until `h1-systematics` confirms they are unnecessary.

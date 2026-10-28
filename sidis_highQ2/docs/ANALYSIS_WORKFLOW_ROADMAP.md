# Analysis workflow roadmap — H1 high-Q² charged-particle SIDIS

**Last updated:** 2026-06-24

This file maps **all** major pieces of the high-Q² charged-particle SIDIS analysis, not just unfolding. It is intended to prevent future agents from re-learning the workflow or using superseded scripts.

**Current status note:** The authoritative current release state is in `docs/RUN_STATUS.md`; as of 2026-06-24 the working z005 release is the IBU/BBB finalcandidate chain and DJANGOH14 gen-only `C_rad` v1 is the stable provisional radiative-correction baseline, while native ROOT/TUnfold v6 miss-underflow is the standard TUnfold entry point for diagnostics/candidate comparisons only. Final release remains blocked by sparse-bin C_rad policy, publication-grade DJ MC-lumi provenance, RAPGAP quarantine resolution/decision, and nominal-method/final-plot decisions. Older TUnfold v4/v5 and rev6 AREA/final-bin text in this roadmap is retained as diagnostic/audit context unless explicitly re-promoted. RooUnfoldSvd/TSVDUnfold is quarantined for the high-Q² charged-SIDIS z005 analysis because its 1D SVD regularization is not topology-safe for our multidimensional flattened bin array; do not use SVD outputs/scripts for method comparisons or validation.

---

## Read-first order

1. `docs/RUN_STATUS.md` — current state, blockers, canonical release paths.
2. `docs/SCRIPT_ROADMAP.md` — script inventory: current vs diagnostic vs superseded.
3. `docs/ANALYSIS_WORKFLOW_ROADMAP.md` — this file; end-to-end analysis map.
4. `docs/LESSONS_LEARNED.md` — pitfalls from rev3→rev6.
5. `context-handoff.md` — historical long-form handoff; treat older paragraphs as audit history unless the top banner says current.

---

## 1. Production inputs and treebuilder

### Current data/MC tree sources

| Item | Path | Status |
|---|---|---|
| Data sidistrees | `naf:output/data/*/sidistree_*.root` | current input, 4-period v5g |
| RAPGAP MC sidistrees | `naf:output/mc/rapgap31/*/sidistree_*.root` | current input |
| DJANGOH MC sidistrees | `naf:output/mc/djangoh14/*/sidistree_*.root` | current input |
| Low-Q² background/PHP trees | `naf:output/pertrack_mc_v3p2_bg/` and bg-sub JSON | bg subtraction already applied to nominal data JSON |

### Stage-2 builder

| File | Role | Status |
|---|---|---|
| `naf:src/ChargedSidisTreeBuilderHighQ2_v5g.C` | current stage-2 builder for `ChargedTot`, `ChargedTotGen`, `DisTot` | produced current trees |
| `scripts/audit/audit_tree_branches_v5g.py` | branch-use/size audit | current diagnostic |
| `docs/results/tree_branch_audit_v5g_2026-06-03.md` | audit report | current |

**Treebuilder cut policy:** Stage-2 tree is intentionally open-acceptance. It stores broad track/gen content; physics cuts are applied downstream in the response builder.

**Future systematic-production recommendation:** implement a trimmed v5h treebuilder after finalizing systematics branch needs. Branch audit suggests 47–54% file-size reduction by dropping legacy/unused branches and `DisTot`, while retaining electron/track/matching diagnostic branches needed for systematics.

---

## 2. Nominal response and data-vector build

### Current response chain

| Step | Script | Output | Status |
|---|---|---|---|
| Build per-period migration ROOTs | `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_NWfix_v2.py` + Condor wrappers | per-period ROOTs in `output/tunfold_tree_v3_percharge/per_file_migration_NWfix_v2/` | current |
| Merge per-period matrices | `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_NWfix_v2.py` | `migration_inputs_v3_percharge_NWfix_v2.root` | current |
| Base binning module | `scripts/unfolding/tunfold_official_v3_percharge/binning_v3.py` | Scheme B′ 240 gen / 960 reco definitions | current base |

### Important response-builder fixes already applied

| Fix | Meaning |
|---|---|
| F3 | V0-matched reco tracks excluded from fakes |
| F4 | `GenPt > 0.15` added to gen fiducial |
| F5 | zGen range symmetrized to `[0,1]` |
| F6 | consumed-key rounding 5→8 decimals |
| F7 | matched-fill charge uses gen charge |
| CC1 | ~~NWeff prior handles NucliaWeight asymmetry~~ **SUPERSEDED 2026-09-09: NWeff prior is physically backwards; ordinary unweighted gen prior is correct. See docs/results/nucliaweight_fix_2026-05-31.md banner.** |

### Current canonical response ROOT

```text
naf:output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge_NWfix_v2.root
```

---

## 3. Background subtraction

### Current status

The nominal data vector used by unfolding comes from:

```text
naf:output/data_subtracted_v3final_REAL_LUMI_filtered.json
```

This is the upstream bg-subtracted data JSON consumed by the merge script. It includes the high-Q² SIDIS background-subtraction policy frozen before rev6.

### Key scripts/docs

| File | Role | Status |
|---|---|---|
| `scripts/background_subtraction/subtract_low_q2_background_v3final.py` | applies low-Q²/PHP background subtraction to data JSON | current historical input; not rerun in rev6_v2 |
| `scripts/background_subtraction/README.md` | details bg components and expected inputs | current reference |
| `docs/research/research-bg-subtraction-final-decision-2026-05-29.md` | canonical bg-subtraction decision history | current reference |
| `docs/handoffs/background_mc_samples_handoff.md` | background sample bookkeeping | current reference |

### Known caveats

- DIFFVM is not present in current `data_subtracted_v3final_REAL_LUMI_filtered.json` data-side systematics component list.
- A Q²<60 background policy is in place; verify double-count risk before final note.
- Current data-side systematic study implements ±50% variations for available bg components (DJ low-Q² and Pythia low-Q²) and fakeScale ±20%.

---

## 4. Radiative corrections (`C_rad`)

### Current status

DJANGOH14 gen-only `C_rad` v1 is the current **stable provisional baseline** for high-Q² charged-SIDIS z005. It supersedes the older multiplicity-ratio / Scheme-B′ roadmap below, which is retained only as historical context. Final publication still requires sparse-bin treatment, publication-grade DJANGOH MC-luminosity provenance, and RAPGAP quarantine resolution or explicit exclusion.

Current study root:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/
```

Current builder and runner:

```text
studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v1.C
bin/x86_64-el9-gcc14-opt/RC_treebuilder_genonly_v1
studies/radiative_correction_genonly_v1/scripts/genonly_condor/run_genonly_entry_direct.sh
```

Current outputs:

```text
output/tables/full_crad_v1/
output/tables/full_crad_v1/full_crad_v1_summary.json
output/tables/validation/validation_rc_genonly_v1.json
output/tables/full_crad_v1/djangoh14_empty_nonrad_recovery_stability_20260623.json
```

### Important status details

| Topic | Current state |
|---|---|
| Formula | `C_rad = σ_nonrad / σ_rad`; `σ ∝ Σ(weight1*weight2)/L_MC` |
| Data luminosity | Does not enter gen-only `C_rad` |
| DJANGOH MC luminosities used | rad `4,108,957 nb^-1`; nonrad `3,560,233 nb^-1`; trace to MCDB/generator logs before publication |
| DJANGOH result | Integrated h+ `1.055329789`, h− `1.055413655`; finite `478/490`; median all charges `1.078836633` |
| Sparse-bin status | Median relative uncertainty <0.5%, but p95 about `30–33%` and max about `104–110%`; define marking/merging/smoothing policy before final correction |
| RAPGAP31 | Quarantined/blocked; anomalous integrated `≈1.405` per charge and median `≈1.478`; do not use for nominal or generator-envelope systematic pending provenance/lumi/sample-definition review |
| Alpha0p2 | Reco-only cone cut; must not be applied to gen-only `C_rad`; alpha0p2 gen-electron path canceled/not adopted |
| Publication status | Do not claim final Born-level values until sparse-bin, DJ MC-lumi provenance, and RAPGAP decision blockers are closed |

### Files/docs to read

| File | Role |
|---|---|
| `docs/RUN_STATUS.md` | current authoritative C_rad status and blockers |
| `context-handoff.md` | detailed gen-only v1 production, validation, and plot paths |
| `studies/radiative_correction_genonly_v1/scripts/histogramming/full_crad_finalize.py` | current finalize script carrying MC-lumi constants into summary JSON |
| `scripts/compute_crad_v3_HQv5gBprime.py` | historical pre-gen-only Scheme-B′ computation script; do not use as current baseline |
| `docs/research/research-crad-for-charged-sidis.md` | background on charged SIDIS C_rad method |
| `docs/references/reference-highQ2-crad-availability.md` | availability/provenance reference |
| `docs/research-crad-v5f-attempt.md` | older attempt; historical |

### What not to do

- Do not copy V0 C_rad constants directly.
- Do not set C_rad=1 in final tables unless explicitly making detector-level internal plots.
- Do not treat the old multiplicity-ratio Scheme-B′ path as the current high-Q² z005 baseline.
- Do not average RAPGAP31 with DJANGOH14 or use RAPGAP31 as a symmetric generator-envelope systematic until the RAPGAP provenance/lumi/sample-definition issue is resolved.
- Do not impose the reco-only `alpha < 0.2` scattered-electron cone cut at gen level.

---

## 5. Unfolding and final binning

### Native v6 TUnfold workflow (standard diagnostic entry point, not central by default)

> **SVD quarantine:** RooUnfoldSvd/TSVDUnfold is quarantined for the high-Q² charged-SIDIS z005 analysis because its 1D SVD regularization is not topology-safe for our multidimensional flattened bin array; do not use SVD outputs/scripts for method comparisons or validation. Historical SVD artifacts may remain on disk for audit provenance only.

For future TUnfold diagnostics and candidate comparisons, start from native ROOT/TUnfold v6 miss-underflow, not v4/v5. Central z005 remains IBU/BBB finalcandidate until the user/physics decision chooses native v6 TUnfold or IBU as nominal.

| Item | Path / convention |
|---|---|
| Script root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v6_native_miss_z005/` |
| Output root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v6_native_miss_z005_20260621/` |
| Local plots | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/tunfold_v6_native_miss_z005_2026-06-21/` |
| ROOT convention | `TUnfoldDensity`, matched 490×245 physical bins, misses in reco/input-axis underflow for `kHistMapOutputVert`, 490-bin `SetInput`, no post-hoc `1/eps` central-value correction |

Validated save points: geometry and `SetInput` PASS; tau0/IBU medians are near 1 without epsilon post-scaling; negative bins and high-refold/high-correlation diagnostics remain blockers. Required pre-promotion gates are Poisson-fluctuated closure, fake/background covariance or `SubtractBackground()` test, hard tau-selection gates, response/MC systematics, and investigation of high refold χ², negative bins, tails, and high-ρ bins.

#### Historical v4/v5 workflow structure

| Stage | What it does | Typical scripts |
|---|---|---|
| Response build | Build per-file per-charge migration matrices from sidistrees. | `build_migration_th2_per_charge_v4.py` and variant copies |
| Data vector build | Build reco-space data vectors directly at native fine reco granularity. | `build_data_vector_v4.py` |
| Merge | Combine response fragments and data vectors into merged ROOT inputs. | `merge_migration_per_charge_v4.py` |
| Nominal unfolding | Run single-tau TUnfold probes and broader tau scans. | `unfold_tunfold_single_tau_v4.C`, `wrap_tau_scan_v4.sh`, `submit_tau_scan_v4.condor`, `summarize_tau_scan_v4.py` |
| Cross-check unfolding | Compare TUnfold with BBB and IBU on common inputs. | `crosscheck_unfold_methods_v4.py` |
| Closure / model tests | Use closure and cross-generator comparisons to estimate model sensitivity. | TUnfold tau/closure products plus method-comparison outputs |
| Systematic variations | Rebuild shifted reco-side responses or data vectors for systematic deltas. | `shifted_electron_kinematics_v4.py`, `build_shifted_response_data_vector_v4.py` |
| Diagnostic finalization | Package scalar-q or shifted-systematic deltas and release-style diagnostic summaries. | `finalize_scalar_q_systematics_v4.py` |

#### Conventions that future agents should preserve

- **Native fine-reco data vectors:** the historical v4 workflow uses a reco-axis data vector defined directly on the fine reco bins. Do not describe it as a coarse-bin value spread uniformly across children.
- **Ordinary fake/miss-bin encoding:** historical v4 merged ROOT inputs used body + fake-column + miss-row terminology; preserve it only when interpreting archives. Do not use this convention for current ROOT/TUnfold diagnostics.
- **v5 physical/fake-sub diagnostics:** v5 fixed the ordinary-bin pathology but solved visible/matched scale and applied external `x_visible/eps`; retain as audit only, not as the standard TUnfold entry point.
- **Central vs systematic outputs:** nominal central inputs belong under the configured central roots. Systematic reruns should write under `output_v6/systematics/...` or wrapper-defined systematics roots, not overwrite central nominal products.
- **Reco-method side studies:** electron/IΣ z005 reco-method studies must follow `docs/systematics/reco_method_side_study_runbook_2026-06-20.md`; branch audit, method-specific q reconstruction, migration-builder switching, data-vector switching, one-file nonzero-effect smoke, and study-local paths are mandatory.
- **Method status:** native v6 TUnfold is the current TUnfold diagnostic/candidate-comparison implementation, but it is not promoted. Central z005 remains IBU/BBB finalcandidate until a nominal-method decision is made between native v6 TUnfold and IBU.
- **Current diagnostic caveat:** several v4 products are raw detector-level or otherwise not yet bg-subtracted and not yet final-`C_rad` corrected. They are suitable for workflow validation and relative deltas, not automatically for final measurement tables.
- **Shifted-electron systematics:** these are post-hoc reco-side response rebuilds, useful for systematic envelopes but not equivalent to a full end-to-end central reprocessing.
- **Scalar-q branch:** scalar-q is an alternate photon-construction diagnostic, not the default published observable definition.

#### Variant families

The same build → data-vector → merge → unfold → cross-check pattern is reused in variant directories, especially:

- `tunfold_official_v4_percharge/`
- `tunfold_official_v4_z2tmd_2xpt_percharge/`
- `tunfold_official_v4_z2tmd_4x_percharge/`
- `tunfold_official_v4_z2tmd_lowpt_2xpt_percharge/`
- `tunfold_official_v4_zlt0p7_4x_percharge/`

Treat these as diagnostic/development variants unless a specific release memo says otherwise.

### Current central candidate / nominal-method decision

```text
output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

Central z005 remains IBU/BBB finalcandidate. Native v6 TUnfold is the standard TUnfold diagnostic/candidate-comparison implementation but is not nominal. The next physics decision is to choose native v6 TUnfold or IBU as nominal, then make final plots for the analysis note.

Historical TUnfold AREA release-generation scripts are audit-only unless explicitly revived:

| Script | Role |
|---|---|
| `scripts/analysis/per_bin_sigma_tunfold_rev6_area.py` | historical per-bin TUnfold σ_stat/σ_reg/σ_model/σ_total |
| `scripts/analysis/build_210bin_tunfold_release.py` | historical 210-bin S3 x-merged TUnfold release |
| `scripts/analysis/build_finalbin_release_ptmerge.py` | historical final pT-only merged release candidate |
| `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau_rev6.C` | historical rev6 area TUnfold macro |
| `scripts/unfolding/tunfold_official_v3_percharge/postprocess_single_tau.py` | historical TUnfold QA plotter |

### Current final-binning status

- Candidate final bin count: 157 bins/charge.
- pT-only merging.
- No z merging.
- Common binning across charges.
- Central z005 remains IBU/BBB finalcandidate.
- Native v6 TUnfold is diagnostic/candidate-comparison only; nominal-method choice versus IBU is pending.

### Key reports

| File | Purpose |
|---|---|
| `docs/results/final_binning_candidate_2026-06-03.md` | final-bin workflow summary |
| `docs/critiques/critique-finalbin-candidate-2026-06-03.md` | critic review of final-bin candidate |
| `docs/results/tunfold_area_210bin_release_2026-06-02.md` | 210-bin release construction |
| `docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md` | closure tau optimization |

### Final-binning still-open issues

- Apply user decision B cleanly in final release metadata (S3 z=0.5–0.7 warning/keep policy).
- Add blocker rows explicitly to release JSON/mask.
- Add `systematic_coverage` block listing pending Tier-2 sources.
- Write response to older rev6 tau-scan FATAL items, classifying them as resolved/deferred/accepted.

---

## 6. Systematics

### Current systematics status

| Source | Status |
|---|---|
| TUnfold σ_stat | from TUnfold covariance diagonal; toy DJANGOH study exists as cross-check |
| σ_reg | tau-neighbor / S3 multi-objective scans available |
| σ_model | RG/DJ half-spread; cross-closure tails need explicit treatment |
| data-side fakeScale/bg | data-side JSONs exist; elecE is stress-test only |
| electron energy | posthoc frame-only study exists; not final |
| electron angle | posthoc study exists; small; not final |
| HFS | ignored for now; would require rerun |
| track quality/efficiency | not finalized |
| charge confusion | pending |
| MC response stat | pending |
| C_rad | DJANGOH14 gen-only v1 provisional baseline produced; final sparse-bin policy, DJ MC-lumi provenance, and RAPGAP quarantine decision pending |

### Key scripts/docs

| File | Role |
|---|---|
| `scripts/analysis/build_data_side_systematics.py` | data-side shifted histograms (elecE/θ/bg/fakeScale) |
| `scripts/diagnostics/build_stat_uncertainty_audit.py` | statistical uncertainty audit |
| `scripts/plotting/plot_uncertainty_pth_panels.py` | uncertainty pT panels |
| `scripts/plotting/plot_unfolding_uncertainty_sources_sidis.py` | Lambda-style uncertainty source plots |
| `scripts/systematics/*` | older v3final systematics scripts; largely historical |
| `docs/results/tree_branch_audit_v5g_2026-06-03.md` | branch trimming for future systematic production |

### Important caveat

The data-side electron-energy uncertainty is **not final** because it uses `posthoc_frame_only`. It is useful as a stress test but not as a final systematic until full event-kinematic recomputation is implemented.

---

## 7. Normalization and cross sections

### Luminosity

For high-Q² SIDIS current 4-period sample:

```text
L_data = 351,000.0 nb^-1 = 351 pb^-1
```

Do not use the superseded pre-rescale luminosity nb^-1 for this current high-Q² SIDIS 4-period release unless explicitly comparing to shared BaryonAsym samples. Do not use 126,880 nb^-1.

### Current release values

Most current release tables are unfolded **yields**, not finalized Born cross sections. Before final cross sections:

1. decide final binning/mask;
2. apply the DJANGOH14 gen-only v1 `C_rad` baseline after sparse-bin policy is defined and MC-lumi provenance is publication-grade;
3. keep RAPGAP31 out of nominal/systematic use unless its quarantine is resolved;
4. divide by luminosity and bin widths;
5. add luminosity uncertainty separately (correlated);
6. validate absolute normalization.

### Scripts

| File | Status |
|---|---|
| `scripts/v3final/compute_xsec_v3final.py` | historical pre-rev6; do not use directly |
| `scripts/v3final/compute_multiplicities_v3final.py` | historical pre-rev6 |
| new final-release xsec script | **not yet written** |

---

## 8. MC comparisons and plots

Current plot directories:

| Directory | Purpose |
|---|---|
| `plots/release_210_filtered/` | current 210-bin pT overlays and status maps |
| `plots/final_binning_candidate/` | final-bin candidate plots |
| `plots/unfolding_uncertainty_sources_sidis/` | uncertainty source / dominant source plots |
| `plots/finalbin_toys_djangoh/` | 100-toy DJANGOH unfolded-yield distributions |
| `plots/stat_uncertainty_audit/` | stat audit |
| `plots/S3_tau_multiobjective/` | S3 tau selection |
| `plots/S3_area_vs_none/` | S3 AREA vs None |
| `plots/S3merged_tunfold_official/` | standalone S3 x-merged test |

MC overlay/data-MC ratio release is still pending.

---

## 9. Superseded pipelines to avoid

| Pipeline | Status |
|---|---|
| `scripts/unfolding/*4d*` | pre-rev6 / historical |
| `scripts/v3final/`, `scripts/v3p2/` | historical releases |
| `scripts/ibu/` | diagnostic only; not central |
| `scripts/omnifold/` | experimental; not central |
| `scripts/scan_TUnfold/_quarantine/` | superseded QA scripts |
| invalid Q²-sliced Python proxy | quarantined; never use |

---

## 10. What a future agent should do next

1. Keep central z005 as IBU/BBB finalcandidate until the nominal-method decision is made.
2. If running TUnfold diagnostics/candidate comparisons, use native v6 `tunfold_official_v6_native_miss_z005/`; do not revive v4 ordinary fake/miss bins or v5 visible+external-eps as current entry points.
3. Resolve native v6 TUnfold promotion blockers: Poisson-fluctuated closure, fake/background covariance or `SubtractBackground()` test, hard tau gates, response/MC systematics, high refold χ², negative bins, tails, and high-ρ bins.
4. Close C_rad release blockers: sparse-bin marking/merging/smoothing policy, publication-grade DJ MC-lumi provenance, and RAPGAP31 quarantine resolution or explicit exclusion.
5. Decide between native v6 TUnfold and IBU as nominal.
6. Make final plots and update the analysis note only after the nominal-method and C_rad decisions are complete.

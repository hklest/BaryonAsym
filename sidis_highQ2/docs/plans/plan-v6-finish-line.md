# Plan: v6 Finish Line — H1 High-Q² Charged-Particle SIDIS

> **SUPERSEDED 2026-10-27:** replaced by the eΣ campaign plans (`esigma_xf_bg_revision_2026-10-24.md`, `closure_uncertainty_revision_2026-10-26.md`) and the program plan `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/plans/2026-10-27-sidis-highq2-finalization.md`; frozen edition `esigmaphotonxf_2026-10-27b`. History only.

**Created:** 2026-06-06
**Status:** Active — dependency-ordered remaining work from v6 production to final plots.
**Audience:** h1-sidis-supervisor and specialist agents picking up this analysis.
**Prerequisites:** Read `context-handoff.md` (v6 banner) and `docs/RUN_STATUS.md` first.

---

## Overview

This plan covers all remaining steps from the current v6 central production state to
publication-ready results. Steps are listed in strict dependency order. Each step
carries: script / action, inputs, outputs, owner agent, and gates / stop conditions.

Serial vs parallel notes are marked **[SERIAL]** or **[PARALLEL where noted]**.

---

## Central Input Contract (v6 only)

> **ALLOW — all steps in this plan must use only these v6 inputs:**
> - Sources: `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`, `src/ChargedSidisTreeBuilderHighQ2_v6.C`
> - Response builder: `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_v6.py`
> - Condor root: `condor_v6/central/` — output root: `output_v6/central/`
> - EVLS pattern: `evls_v6_*.root`
>
> **DENY — do not use for any step in this plan:**
> - `output/` v5g products; `condor/` v5g products
> - `condor_v6_central/` (deprecated — not the same as `condor_v6/central/`)
> - `evls_highQ2_*.root` (deprecated EVLS pattern)
> - `migration_inputs_v3_percharge_NWfix_v2.root` and any pre-v6 response/migration product
> - All `data_release_rev6_*` directories
>
> Only reference DENY items if a step is **explicitly labeled** audit / history / comparison / provenance.

---

## Step 0: Production Completion Audit  [COMPLETE — 2026-06-06]

**Status:** **COMPLETE.** Stage 2 data production is 1523/1523. The previously-missing
file `sidistree_em0405_d05_0088.root` is now present at
`output_v6/central/sidistree/data/em0405/sidistree_em0405_d05_0088.root`. Proceed to Step 1.

| Field | Value |
|-------|-------|
| Script / action | (Historical) Audit + resubmission of the single missing Condor job for `sidistree_em0405_d05_0088.root` and verification of output |
| Input | `condor_v6/central/` wrapper scripts; NAF `output_v6/central/sidistree/data/em0405/` |
| Output | `sidistree_em0405_d05_0088.root` present (verified 2026-06-06); audit log at `condor_v6/central/audits/2026-06-06_wrapper_output_audit/` |
| Owner agent | h1-naf-operator |
| Gate / stop | **PASSED.** Audit confirms 1523/1523 data + all MC files present, each non-empty. |

---

## Step 1: Schema Validation  [SERIAL — after Step 0]

**What:** Formal v6 sidistree schema audit. Verify branch presence, types, and sentinel
behaviour (especially `TrackLength` sentinel −999 on MC, absence of `StartRadius`/`EndRadius`).

| Field | Value |
|-------|-------|
| Script / action | h1-qa-specialist schema audit on one data file and one MC file from `output_v6/central/` |
| Input | 1 data sidistree + 1 MC sidistree from v6 production |
| Output | Schema validation report; confirmed branch list; sentinel guard requirements documented |
| Owner agent | h1-qa-specialist |
| Gate / stop | All expected branches present; `StartRadius`/`EndRadius` absent; `TrackLength` sentinel verified. Fail → h1-builder-engineer to patch Stage 2 source before continuing. |

---

## Step 2: v6 Response Build  [SERIAL — after Step 1]

**What:** Build the v6 migration matrix (TH2) per charge using the validated v6 trees.

| Field | Value |
|-------|-------|
| Script / action | `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_v6.py` via Condor |
| Input | All v6 MC sidistrees in `output_v6/central/` |
| Output | `output_v6/central/migration_inputs_v6.root` (or per-period partials + merge) |
| Owner agent | h1-unfolder (dispatches via h1-naf-operator) |
| Gate / stop | Migration QA smoke passes: gen-only count ≈ 7000–8000 per charge; reco-only count ≈ 800–900; zGen < 0 count ≤ 100; no NaN/Inf. If counts deviate by > 20% from smoke baseline in `RUN_STATUS.md`, halt and investigate before merging. |
| Notes | Builder applies NO per-bin gen cuts (no GenPt, GenEta, xF, final-Q2/y/Empz). Final fiducial is imposed downstream. zGen finite filter is the only builder-level filter on gen hadrons. |

---

## Step 3: Data and Background Vectors  [SERIAL — after Step 2; PARALLEL sub-tasks]

**What:** Construct the data marginal vector and background-subtracted data vector
from v6 data sidistrees. These feed TUnfold as the "data to unfold."

| Field | Value |
|-------|-------|
| Script / action | Adapt existing `scripts/background_subtraction/subtract_low_q2_background_v3final.py` for v6 paths and v6 branch names; run on v6 data trees |
| Input | v6 data sidistrees (`output_v6/central/data/`); bg MC sidistrees (unchanged from v5g: `output/mc_bg/`); L_MC_bg from `output/lumi_mc_bg_authoritative.json` |
| Output | `output_v6/central/data_marginal_v6.json`; `output_v6/central/data_subtracted_v6.json` |
| Owner agent | h1-physics-analyst |
| Gate / stop | Data marginal total within 15% of v5g baseline (6.0 × 10⁶ ± 20%); no negative cells after subtraction in trusted bins. If bg fraction > 20% integrated, escalate to supervisor before applying. |
| Notes | Sub-task A (data marginal) and sub-task B (bg subtraction) can run in parallel once v6 data trees are confirmed complete. |

---

## Step 4: Purity / Stability Maps  [SERIAL — after Step 2; PARALLEL with Step 3]

**What:** Derive per-bin purity and stability (migration fractions) from the v6 response.

| Field | Value |
|-------|-------|
| Script / action | Adapt `scripts/analysis/per_bin_sigma_rev6_v2.py` or equivalent; read v6 migration ROOT file |
| Input | `output_v6/central/migration_inputs_v6.root` |
| Output | `data/purity_stability_v6.json`; `plots/purity_stability_v6/` |
| Owner agent | h1-physics-analyst |
| Gate / stop | Purity ≥ 0.5 in ≥ 70% of bins across the Scheme B′ 4D grid; bins below 0.5 flagged for exclusion from published set. Stability ≥ 0.5 in same set. |

---

## Step 5: Finalbin Revalidation  [SERIAL — after Steps 3 and 4]

**What:** Re-derive the final-binning scheme on v6 statistics. The rev6/finalbin candidate
(157 bins per charge, two z < 0.7 blockers) is historical — must be redone from v6 purity/stability.

| Field | Value |
|-------|-------|
| Script / action | `scripts/analysis/build_finalbin_release_ptmerge.py` (or successor); iterate binning over v6 purity/stability maps |
| Input | `data/purity_stability_v6.json`; Scheme B′ bin definitions |
| Output | `data/finalbin_v6_candidate.json`; `plots/final_binning_v6/` |
| Owner agent | h1-physics-analyst with h1-analysis-critic review |
| Gate / stop | No published bins with purity < 0.5; no bins with N_data < 10 in published set; h1-analysis-critic returns SOUND or CONCERN (not FATAL) on binning choices. Resolve any FATAL before proceeding. |

---

## Step 6: Tau Scans and Closure Tests  [SERIAL — after Step 5]

**What:** Run TUnfold tau scan and closure tests on the v6 response at the revalidated
finalbin. Select canonical (τ_pos, τ_neg) pair by per-bin closure quality score.

| Field | Value |
|-------|-------|
| Script / action | `scripts/unfolding/tunfold_official_v3_percharge/` Condor tau scan; `scripts/analysis/closure_rev6_v2.py` (adapted for v6) |
| Input | `output_v6/central/migration_inputs_v6.root`; data vector from Step 3; finalbin from Step 5 |
| Output | `output_v6/central/closure_v6_tauopt/`; `data/closure_v6_tauopt/closure_tauopt_summary.json`; selected τ pair |
| Owner agent | h1-unfolder |
| Gate / stop | Per-bin closure score: selfRG/selfDJ p50 < 5%, p90 < 25% at selected τ. Data L-curve kink location consistent with selected τ. Refold χ²/ndf < 5 at selected τ. If any gate fails, iterate τ selection; do not promote to canonical until all three gates pass. |

---

## Step 7: Per-Bin Sigma Components  [SERIAL — after Step 6]

**What:** Compute σ_stat, σ_reg, σ_model for all published v6 bins at the canonical
(τ_pos, τ_neg).

| Field | Value |
|-------|-------|
| Script / action | `scripts/analysis/per_bin_sigma_rev6_v2.py` (adapted for v6); Condor for σ_model (RG vs DJ cross-closure) |
| Input | v6 response; data vector; τ pair from Step 6 |
| Output | `data/per_bin_sigma_v6/{sigma_stat,sigma_reg,sigma_model}_{pos,neg}.json` |
| Owner agent | h1-systematics |
| Gate / stop | σ_stat median < 15% in trusted bins; σ_model / σ_stat < 3 in ≥ 90% of bins; σ_reg / σ_stat < 0.5 in ≥ 90% of bins. Flag outlier bins. h1-analysis-critic reviews the budget summary. |

---

## Step 8: C_rad Cross-Section-Ratio Correction  [SERIAL — after Step 7; PARALLEL with data release prep]

**What:** Re-derive radiative correction `C_rad(bin) = σ_nonrad(bin) / σ_rad(bin)` on v6 trees.
For the current SIDIS radiative-correction work, `c_rad` / `C_rad` is the ratio of cross sections, not a multiplicity ratio.

| Field | Value |
|-------|-------|
| Script / action | Adapt `scripts/` C_rad pipeline (see `docs/methodology/crad_v5f_methodology_FROZEN.md` for method); run NORAD vs RAD MC on v6 trees |
| Input | v6 MC sidistrees (RAD and NORAD samples); per-bin sigma from Step 7 |
| Output | `data/crad_v6_cross_section_ratio.json`; `plots/crad_v6/` |
| Owner agent | h1-radiative-correction |
| Gate / stop | C_rad central values within 20% of v5f-era audit range (0.755–0.865 per-Q²); generator spread systematic ≤ 5% per bin. h1-analysis-critic reviews. Absolute cross sections remain blocked until this step passes. |

---

## Step 9: MCEG Release Rebuild  [SERIAL — after Step 8; or PARALLEL with final plots if blocked]

**What:** Rebuild the MC generator comparison release (RAPGAP vs DJANGOH predictions
vs unfolded data) on v6 trees and binning.

| Field | Value |
|-------|-------|
| Script / action | Adapt `scripts/analysis/` MCEG comparison scripts; read v6 unfolded data + MC predictions on v6 finalbin |
| Input | v6 unfolded data (from Step 6); v6 MC predictions at finalbin |
| Output | `data_release_v6_MCEG/`; comparison plots |
| Owner agent | h1-physics-analyst |
| Gate / stop | MC/data ratio within 2× across ≥ 80% of published bins; no systematic shape anomalies flagged by h1-analysis-critic. |

---

## Step 10: Final Publication Plots  [SERIAL — after Steps 8 and 9]

**What:** Build the final publication-quality plot set: pT_h distributions per (Q², x, z, charge),
A_h asymmetry panels, C_rad panels, MC/data overlay panels.

| Field | Value |
|-------|-------|
| Script / action | h1-plotting-naf remote headless plot production from v6 finalbin data release |
| Input | v6 finalbin data release JSON/CSV; per-bin sigma; C_rad; MCEG comparison |
| Output | `plots/publication_v6/`; packaged PDF bundle |
| Owner agent | h1-plotting-naf |
| Gate / stop | h1-plot-reviewer returns no BLOCKING anomalies (zero-content panels, axis mislabels, wrong frame). All pT_h computed in γ*+p CM frame. All phi_h in Trento convention. |

---

## Step 11: Data Release Build  [SERIAL — after Steps 8 and 10]

**What:** Build the formal v6 data release package (replaces all `data_release_rev6_*` directories).

| Field | Value |
|-------|-------|
| Script / action | Adapt `scripts/v3final/build_data_release_v1p2_final.py` for v6 paths and schema |
| Input | v6 unfolded results; per-bin sigma; C_rad; trust mask; publication plots |
| Output | `data_release_v6_FINAL/` with README, CHANGELOG, MANIFEST.sha256, schema/, measurement/, trust/ |
| Owner agent | h1-physics-analyst |
| Gate / stop | MANIFEST.sha256 all entries verified; 0 negative xsec in trusted set; C_rad applied to all multiplicity bins; h1-qa-specialist confirms schema. |

---

## Dependency Graph (summary)

```
Step 0 (production audit)
  └─ Step 1 (schema validation)
       └─ Step 2 (v6 response build)
            ├─ Step 3 (data/bg vectors)      [parallel with Step 4]
            └─ Step 4 (purity/stability)     [parallel with Step 3]
                 └─ Step 5 (finalbin revalidation)
                      └─ Step 6 (tau scans / closure)
                           └─ Step 7 (per-bin sigma)
                                ├─ Step 8 (C_rad mapping)
                                └─ (Step 9 can start after Step 6 if Step 8 is slow)
                                     └─ Step 10 (final plots)     [after Steps 8+9]
                                          └─ Step 11 (data release)
```

---

## Documentation Quarantine / Refactor Guidance

> **Do not move files now.** The following is a list for future quarantine action
> once v6 results are validated. Review before any cleanup session.

### Category A — Candidate for quarantine after v6 finalbin is validated

| Item | Reason |
|------|--------|
| `data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/` | Superseded by v6 |
| `data_release_rev6_TUnfold_AREA_210bin_INTERNAL_PRELIMINARY/` | Superseded |
| `data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/` | Superseded |
| `data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` | Superseded by v6 finalbin |
| `data_release_rev6_TUnfold_AREA_INTERNAL_PRELIMINARY/` | Superseded |
| `data_release_rev6_v2_INTERNAL_PRELIMINARY/` | Superseded |

### Category B — Keep but mark HISTORICAL in-file

| Item | Action |
|------|--------|
| `docs/plans/plan-finalization-r1.md` | Add HISTORICAL banner; references pre-v6 finalization |
| `docs/plans/plan-highQ2-sidis-next-steps.md` | Add HISTORICAL banner; pre-v6 |
| `docs/plans/plan-adaptive-lowpt-scan-2026-06-03.md` | Add HISTORICAL banner |
| `docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md` | Retain as audit; add note that tau selection is superseded pending v6 re-run |
| `docs/results/final_binning_candidate_2026-06-03.md` | Retain as audit; two blockers are moot pending v6 finalbin |

### Category C — Retain unchanged (durable references)

| Item | Reason |
|------|--------|
| `docs/methodology/crad_v5f_methodology_FROZEN.md` | Method still canonical; only numbers stale |
| `docs/methodology/structural_separation_2026-05-23.md` | Still accurate |
| `docs/handoffs/memo-mc-truth-policy-charged-sidis.md` | Species convention unchanged |
| `docs/critiques/rev6_audit_2026-05-31/SYNTHESIS.md` | Still the authoritative open-issues list |
| `docs/LESSONS_LEARNED.md` | Structural insights still valid |

---

## Relationship To v4 Native-Fine Diagnostic Machinery

The workspace also contains a `tunfold_official_v4*` native-fine-reco diagnostic chain used for response studies, BBB/IBU/TUnfold comparisons, shifted-electron reco-side systematics, scalar-q alternate-construction diagnostics, and variant shootouts (`z<0.7`, `z2tmd`, low-pT diagnostics).

That machinery is **allowed for development, QA, cross-checks, and systematic prototyping**, but it is **not** the central contract for this v6 finish-line plan unless a step explicitly calls for a diagnostic comparison. In particular:

- do not replace v6 central inputs with v4 diagnostic products;
- do not treat v4/native-fine outputs as final release artifacts by default;
- do use the v4 chain when a step needs method cross-checks, closure diagnostics, or reco-side shifted-response systematic studies that are not yet folded into the v6 central release.

## Notes for Supervisor

- **Do not promote any pre-v6 data release as current.** All `data_release_rev6_*` directories
  are historical until Step 11 is complete.
- **C_rad blocks absolute cross sections.** Multiplicities and A_h can be released
  before C_rad is complete; absolute cross sections cannot.
- **Three-iteration critic cycle** applies at Steps 5, 6, 7, 8, and 10. Do not
  skip critic review for any step that produces physics-facing outputs.
- **SSH policy:** Check control socket before any NAF action. All multi-file
  production runs go through Condor; no production on the login node.
- **Binary identity:** Before any recompile or production run, verify
  `scripts/binary_identity_manifest.json` reflects the v6 binary pair.

# Plan: HQ-v5f-r2 Binning-vs-Resolution Audit for High-Q² Charged-Particle SIDIS

## 0. Plan metadata

- **Date:** 2026-05-22
- **Supervisor of record:** `h1-sidis-supervisor`
- **Plan scope:** implement a dedicated high-Q² binning-vs-resolution audit for the frozen `HQ-v5f-r2` grid, producing residual, resolution, migration, purity, stability, generator-comparison, and alternative-binning sensitivity outputs for downstream finalization use.
- **Plan status:** draft for approval before implementation.
- **Annotation cycle iteration:** 1
- **Workspace:** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/`
- **Primary analysis regime:** `Q2 > 150 GeV^2`
- **Frozen nominal binning anchor:** `HQ-v5f-r2` from `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_v5f_frozen.json`
- **Primary output plan target:** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/plan-binning-vs-resolution-HQv5fr2.md`
- **Research / authoritative inputs used:**
  - `/data/dust/group/h1/klesthen/BaryonAsym/CONTEXT.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/context-handoff.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/plan-finalization-r1.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md`
  - User-provided authoritative context embedded in the request, including frozen edges, existing JSON products, schema caveat, beam-energy policy, cuts, and required outputs.

## 1. Motivation

The frozen `HQ-v5f-r2` binning is already the approved analysis grid for the high-Q² charged-particle SIDIS program. This study is therefore not a bin-design exercise and must not reopen the binning discussion. Its role is narrower and more operationally important: audit whether the already-frozen 4D cells are sufficiently wide relative to the reco-gen resolution, identify where migration is modest enough for bin-by-bin intuition, identify where unfolding remains mandatory, and package the resulting numbers so they can be consumed directly by the finalization and systematic-rigor workflows.

Low-Q² already established the relevant methodology in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/study_binning_purity.py` and `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/tmd_resolution_acceptance.py`. The high-Q² version must mirror the same study logic and figure family closely enough that reviewers can compare regimes directly, but it must not blindly copy low-Q² implementation assumptions. In particular, the high-Q² schema differs on `ChargedTot`: the gen scattered-electron four-vector cannot be reconstructed from `GenElecE` and `GenElecTheta` there because those polar branches are not available in the required form for this path, and the study must instead use the already-frozen inverse electron-method helper route from `(Q2sGen, YsGen, xGen, GenElecPhi, GenBeamElecE, GenBeamProtP)`.

A successful audit will do five things at once: (1) certify whether each populated `HQ-v5f-r2` bin satisfies the `bin width >= 2 × resolution RMS` check, (2) quantify 1D/2D/4D migration structure in the same vocabulary already used for r2 migration and unfolding decisions, (3) compare RAPGAP31 and DJANGOH14 to expose model sensitivity in the response itself, (4) measure how much the headline purity picture moves under narrower and wider `z` / `pT_h` variants without proposing a rebin, and (5) hand downstream consumers a stable JSON plus narrative memo plus figure set that can be cited in the per-bin systematics rigor table and the finalization critic gates.

## 2. Requirements Summary

### 2.1 Confirmed requirements

| Category | Confirmed requirement |
|---|---|
| Purpose | Audit and certify the frozen `HQ-v5f-r2` binning; do **not** propose bin changes. |
| Study family | Mirror the methodology and plot style proven in low-Q² precedent scripts: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/study_binning_purity.py` and `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/tmd_resolution_acceptance.py`. |
| Frozen grid | Use the exact `HQ-v5f-r2` edges: `Q2=[150,300,700,2000,20000]`, `x=[0.003,0.01,0.03,0.1,0.5]`, `z=[0.0,0.1,0.2,0.3,0.5,0.7,1.0]`, `pTh=[0.0,0.5,1.0,2.0,4.0,8.0]`. |
| Existing comparison anchor | Must compare against `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/migration_matrices_v5f_r2_default.json`, including the known `purity_4d_median_over_populated_bins = 0.626` reference value. |
| Required outputs | Produce residual distributions for `{Q2, x, z, pTh, phi_h}`, per-variable bias and RMS, RMS-vs-value curves, bin-width-vs-`2*RMS` checks, 1D purity/stability, 2D purity slices, 4D heatmap projection, alternative-binning scans, RAPGAP-vs-DJANGOH comparison, JSON, narrative MD, and figure set. |
| Output paths | JSON at `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_resolution_HQv5fr2.json`, narrative at `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/results/binning_resolution_HQv5fr2.md`, plots under `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/`. |
| Figure names | Must include `fig_r01_resolutions.png`, `fig_r02_resolution_vs_value.png`, `fig_r03_binning_vs_resolution.png`, `fig_r04_purity_stability_1d.png`, `fig_r05_purity_2d_Q2_pTh.png`, `fig_r06_purity_2d_z_pTh.png`, `fig_r07_4d_purity_heatmap.png`, plus alternative-binning scan plots and a small sqrt(s) sanity plot. |
| Samples | Read RAPGAP31 and DJANGOH14 high-Q² ROOT outputs from `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_rapgap31_*.root` and `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_djangoh14_*.root`. |
| Trees | Use `ChargedTot` as matched reco numerator source and `ChargedTotGen` as gen denominator source. |
| Critical schema caveat | On `ChargedTot`, reconstruct the gen scattered-electron four-vector using `gen_scat_electron_from_kin(Q2sGen, YsGen, xGen, GenElecPhi, GenBeamElecE, GenBeamProtP)` style logic already present in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/extract_phi_moments_data.py` and related scripts. |
| Gen-tree electron path | On `ChargedTotGen`, prefer stored Cartesian `GenScatEX/Y/Z/E` for gen-side denominator / truth work. |
| Beam policy | All kinematics must use per-event beam energies from `BeamElecE` / `BeamProtP` and `GenBeamElecE` / `GenBeamProtP` via `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/_beam_energy_utils.py`; `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/verify_beam_energy_usage.py` must show `0 FAIL` before NAF launch. |
| Event cuts | Apply frozen high-Q² cuts: `Q2 > 150`, `0.1 < y < 0.6`, `35 < E-pz < 70`, `|VtxZ| < 35`, S67 trigger on data only. |
| Track cuts | Apply `Pt > 0.15`, `|eta| < 1.3`, `NHitDedx >= 10`, `RadLength > 10`, `|DcaPrime/cosh(eta)| < 2`. |
| Gen fiducial | Apply `GenPt > 0.15`, `|GenEta| < 1.3`. |
| Matched-MC selection | Use `BASE_CUT & IsMatched=1 & gen_fid & ParticleSource != 4 & charge_agreement`. |
| sqrt(s) sanity | Reproduce the per-event `sqrt(s)` span `[318.1, 319.2] GeV` with mean `318.78 GeV` from MC and include one small check plot. |
| Pass criteria | Encode exact thresholds given by the user for resolution, purity, alt-binning sensitivity, and generator disagreement. |
| Implementation separation | This file plans the work only; it does not implement the script. |

### 2.2 Inferred assumptions to make explicit

| Topic | Inferred assumption | Why it matters |
|---|---|---|
| Main implementation vehicle | The high-Q² study will most likely be implemented as a new dedicated script under `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/`, rather than patching low-Q² scripts in place. | Keeps high-Q² schema-specific logic isolated and avoids accidental low-Q² regressions. |
| JSON granularity | The JSON should carry both aggregated summary numbers and per-bin/per-axis detailed tables, because downstream consumers need machine-readable thresholds and flags. | Prevents narrative-only output that cannot feed later systematics tables. |
| Weighting basis | Purity, stability, and residual summaries should use the nominal matched-MC event weights already standard in the high-Q² pipeline, applied consistently across generators and combined outputs. | Affects reproducibility relative to existing migration products. |
| Populated-bin handling | All headline medians and pass fractions should be computed over populated bins only, while the JSON should still retain explicit empty-bin bookkeeping. | Needed to compare fairly to the existing `0.626` 4D median anchor. |
| phi_h treatment | `phi_h` enters resolution-only products and not the 4D frozen bin audit, because `phi_h` is not a frozen bin axis in `HQ-v5f-r2`. | Avoids confusing a diagnostic variable with a nominal bin axis. |
| 2D purity slices | The 2D heatmaps are projections / aggregations derived from the 4D response and not a separate denominator definition. | Needed for numerical consistency with the 4D audit. |
| Alt-binning variants | “Narrower-by-half” and “wider-by-2x” for `z` and `pT_h` will be operationalized as explicit alternative edge sets defined inside the new study script / config, not by mutating the frozen nominal JSON. | Prevents accidental scope creep into bin-freeze files. |

### 2.3 Acceptance criteria

The plan is implementation-ready only if it enables an implementer to produce, from the authoritative MC inputs, a reproducible artifact set containing:

1. Residual distributions and summary statistics for `Q2`, `x`, `z`, `pTh`, and `phi_h`.
2. RMS-vs-value curves for each variable, including log-space forms for `Q2` and `x`.
3. Bin-width-vs-`2*RMS` audit outputs with pass/fail flags for every populated nominal bin along each axis.
4. 1D purity/stability summaries and 2D/4D purity products that numerically reconcile with the existing `migration_matrices_v5f_r2_default.json` headline median or explain any difference.
5. RAPGAP31-vs-DJANGOH14 per-bin disagreement tables with explicit `>0.05` absolute flags.
6. Alternative `z` / `pT_h` sensitivity tables demonstrating whether `delta-purity < 0.10` absolute under half-width scans.
7. A machine-readable JSON, a narrative markdown note, and a low-Q²-parallel figure set.
8. A NAF rerun procedure with smoke-test instructions and beam-policy verification guard.

## 3. Open Questions

This plan is largely implementation-ready from the user’s brief. The only open items are operational, not conceptual.

1. **Combined-vs-per-generator combined headline JSON ownership:** the plan assumes the JSON will contain both per-generator blocks and a combined nominal block; if the supervisor wants only generator-separated JSON plus a derived summary elsewhere, that should be stated before implementation.
2. **Alternative-binning exact edge construction:** the user specified the minimum requirement (“narrower-by-half and wider-by-2x `z` and `pT_h` edges”). This plan assumes implementers should construct deterministic alternative edge lists centered on the frozen nominal structure and clip them to the same physical domain endpoints. If the supervisor prefers a different exact convention, it should be frozen before implementation to prevent post-hoc reinterpretation.
3. **Whether the small sqrt(s) plot gets its own numbered `fig_r0x` file or is emitted as an appendix / auxiliary figure:** this plan includes it as an auxiliary figure so the required r01–r07 parallel naming remains intact.

## 4. Approach overview

The study should be executed as a single coherent audit pipeline rather than as loosely coupled one-off notebooks. The implementation should read the matched reco tree and gen tree from both RAPGAP31 and DJANGOH14, derive the necessary per-event reco and gen kinematics with the high-Q² schema-specific electron reconstruction path, apply the frozen matched-MC selection and fiducial logic, and then emit a unified result package containing residual metrics, migration metrics, threshold flags, and figure products.

The preferred structure is one dedicated high-Q² script plus minimal helper reuse from existing utilities. The script should be written so that the nominal r2 audit, generator-separated views, combined summaries, and alternative-binning scans all flow through one common event-selection and binning engine. That design minimizes the risk of silently comparing non-identical selections or weight prescriptions across sections of the study.

The low-Q² precedent files should be treated as methodological anchors rather than source templates:

- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/study_binning_purity.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/tmd_resolution_acceptance.py`

The implementer should also cross-reference the high-Q² helper / policy anchors:

- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/extract_phi_moments_data.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/_beam_energy_utils.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/verify_beam_energy_usage.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/context-handoff.md` (P9 beam-energy policy, schema notes, and r2 status)
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/plan-finalization-r1.md` (finalization anchor for why this audit matters)

## 5. Planned file touch list

## 5.1 Files expected to be created

| Path | Role |
|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/study_binning_resolution_HQv5fr2.py` | Main study driver for residuals, migration metrics, alt-binning scans, JSON, markdown, and plots. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_resolution_HQv5fr2.json` | Machine-readable output consumed by downstream review, systematics, and finalization tasks. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/results/binning_resolution_HQv5fr2.md` | Narrative result memo summarizing methods, findings, flags, and reconciliation to existing migration JSON. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r01_resolutions.png` | Residual distribution panel figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r02_resolution_vs_value.png` | RMS-vs-value figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r03_binning_vs_resolution.png` | Bin-width vs `2*RMS` audit figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r04_purity_stability_1d.png` | 1D purity/stability figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r05_purity_2d_Q2_pTh.png` | 2D purity slice figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r06_purity_2d_z_pTh.png` | 2D purity slice figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r07_4d_purity_heatmap.png` | 4D purity projection figure. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r08_alt_binning_scan_z.png` | `z` alternative-binning sensitivity plot. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r09_alt_binning_scan_pTh.png` | `pT_h` alternative-binning sensitivity plot. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_aux01_sqrt_s_sanity.png` | Small auxiliary `sqrt(s)` sanity-check plot. |

## 5.2 Files expected to be read but not modified

| Path | Role in planning / implementation |
|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_v5f_frozen.json` | Source of truth for `HQ-v5f-r2` edges and historical `r1` scheme storage. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/migration_matrices_v5f_r2_default.json` | Existing r2 purity/stability anchor; must be reproduced or differences explained. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/unfolding_compare_v5f_HQv5fr2.json` | Existing unfolding-method comparison anchor to align interpretation of flagged cells. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/unfolding_compare_v5f_HQv5fr2_analyzed.json` | Existing analyzed unfolding summary. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/references/reference-beam-energy-policy.md` | Beam-energy policy and acceptable fallbacks. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/references/reference-highQ2-charged-sidis-analysis-cuts.md` | Frozen event and track cuts. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/decision-binning-amendment-HQv5fr2.md` | Freeze decision and caveat context. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md` | Existing r2 rebuild context for operational consistency. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/study_binning_purity.py` | Low-Q² precedent for purity / migration workflow. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/scripts/tmd_resolution_acceptance.py` | Low-Q² precedent for residual and resolution workflow. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/extract_phi_moments_data.py` | Existing high-Q² helper anchor for inverse electron-method reconstruction path. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/_beam_energy_utils.py` | Beam-energy helper to be reused directly. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/verify_beam_energy_usage.py` | Guard script to verify no hard-coded beam energy leakage. |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_rapgap31_*.root` | RAPGAP31 per-run inputs (723 files). |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_djangoh14_*.root` | DJANGOH14 per-run inputs (486 files). |

## 6. Inputs

### 6.1 MC input files on NAF

| Generator | Glob | Expected file count | Role |
|---|---|---:|---|
| RAPGAP31 | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_rapgap31_*.root` | 723 | Main generator sample for matched reco and gen truth. |
| DJANGOH14 | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_djangoh14_*.root` | 486 | Generator-variation sample for matched reco and gen truth. |

The implementation should preserve generator separation throughout the pipeline and only form a combined “nominal” block after each generator has its own complete derived metrics. That is required to support the per-generator disagreement audit and to keep reconciliation with the existing migration JSON transparent.

### 6.2 Trees and sample semantics

| Tree | Semantics in this study | Notes |
|---|---|---|
| `ChargedTot` | Reco-side matched numerator sample and source of reco observables plus matched-gen companion fields stored on the reco tree. | This is where the high-Q² schema caveat matters for gen electron reconstruction. |
| `ChargedTotGen` | Gen-side denominator / truth sample for denominator-oriented checks and preferred source of stored Cartesian gen scattered-electron four-vector. | Use for truth-only or denominator-like projections where appropriate. |

### 6.3 Required branch families by purpose

| Purpose | Reco-tree branches on `ChargedTot` | Gen-tree branches on `ChargedTotGen` |
|---|---|---|
| Reco kinematics | `Q2`, `x`, `z_SIDIS`/high-Q² nominal `z`, `pT_h`, `phi_h`, `Pt`, `Eta`, `VtxZ`, `Empz`, event weight branches, `BeamElecE`, `BeamProtP` | N/A for reco quantities |
| Matched-gen companion kinematics on reco tree | `Q2sGen`, `YsGen`, `xGen`, matched hadron truth branches, `GenElecPhi`, `GenBeamElecE`, `GenBeamProtP`, charge-truth consistency fields | Required for per-event reco-vs-gen residuals built from the matched sample. |
| Truth denominator kinematics | N/A | `GenScatEX`, `GenScatEY`, `GenScatEZ`, `GenScatEE`, hadron truth variables, `GenBeamElecE`, `GenBeamProtP`, `GenPt`, `GenEta` |
| Matching / source filters | `IsMatched`, `ParticleSource`, charge / matched-species agreement branches | `GenIdx` and truth-class branches if denominator bookkeeping needs them |
| Track quality cuts | `NHitDedx`, `RadLength`, `DcaPrime`, `Eta`, `Pt` | N/A |

### 6.4 Nominal high-Q² binning under audit

| Axis | Edges | n_bins | Bin widths to audit |
|---|---|---:|---|
| `Q2` [GeV²] | `[150, 300, 700, 2000, 20000]` | 4 | `[150, 400, 1300, 18000]` in linear space; compare in log space for the criterion plots. |
| `x` | `[0.003, 0.01, 0.03, 0.1, 0.5]` | 4 | `[0.007, 0.02, 0.07, 0.4]` in linear space; compare in log space for the criterion plots. |
| `z` | `[0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` | 6 | `[0.1, 0.1, 0.1, 0.2, 0.2, 0.3]`. |
| `pT_h` [GeV] | `[0.0, 0.5, 1.0, 2.0, 4.0, 8.0]` | 5 | `[0.5, 0.5, 1.0, 2.0, 4.0]`. |

### 6.5 Existing anchors this study must reconcile to

| Existing artifact | Key known content | Required relationship to the new study |
|---|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/migration_matrices_v5f_r2_default.json` | 4D purity/stability already computed; 320 populated bins out of 480; `purity_4d_median_over_populated_bins = 0.626`. | New study must reproduce this headline median within expected numerical tolerance or explain any difference in selection, weighting, projection, or empty-bin handling. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/unfolding_compare_v5f_HQv5fr2.json` | BBB vs D'Agostini vs SVD comparison. | New audit must use flags from the resolution/migration study to identify unfolding-required cells, not to reopen binning. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/unfolding_compare_v5f_HQv5fr2_analyzed.json` | Analyzed method-comparison summary. | Narrative memo should cross-reference whether flagged low-purity cells align with existing unfolding sensitivity. |

## 7. Methodology

### 7.1 Stage 0: ingestion, canonical selection, and common derived columns

Before any stage-specific outputs are computed, the implementation should establish one canonical event-loading and selection block shared by all later sections.

#### 7.1.1 Core ingestion rules

| Rule | Required behavior |
|---|---|
| Multi-file reading | Iterate over the NAF per-run ROOT files directly; do not require merged ROOT files as an input dependency. |
| Generator separation | Keep RAPGAP31 and DJANGOH14 logically separate until summary-combine time. |
| Tree pairing | Read `ChargedTot` and `ChargedTotGen` as distinct sources with explicit branch allow-lists. |
| Weight consistency | Use the same nominal matched-MC weighting prescription as the current high-Q² pipeline and document it explicitly in the JSON metadata. |
| Memory strategy | Stream / chunk over files to avoid forcing all matched events into memory at once. |
| Empty-file robustness | Skip unreadable or empty trees with explicit counters and surface them in the JSON metadata. |

#### 7.1.2 Canonical matched reco selection

The matched reco sample is the primary basis for residuals and reco-indexed purity.

Required logical selection:

- event-level frozen cuts: `Q2 > 150`, `0.1 < y < 0.6`, `35 < E-pz < 70`, `|VtxZ| < 35`
- track-level frozen cuts: `Pt > 0.15`, `|eta| < 1.3`, `NHitDedx >= 10`, `RadLength > 10`, `|DcaPrime/cosh(eta)| < 2`
- matched-MC gate: `IsMatched == 1`
- gen fiducial gate: `GenPt > 0.15`, `|GenEta| < 1.3`
- source veto: `ParticleSource != 4`
- charge agreement gate: implementation must use the frozen high-Q² charge-consistency logic already used in the current charged SIDIS pipeline.

The script should expose this selection as a named block in the JSON metadata and repeat it verbatim in the markdown narrative so that later reviewers can reconcile it to the migration and unfolding products.

#### 7.1.3 Canonical gen denominator selection

For denominator-oriented truth summaries and gen-indexed stability calculations, the implementation should construct a gen-fiducial truth sample from `ChargedTotGen` with:

- gen event-level DIS cuts mapped to the frozen high-Q² definitions
- gen hadron fiducial cuts `GenPt > 0.15`, `|GenEta| < 1.3`
- truth species / source filtering consistent with the charged SIDIS truth policy
- beam and scattered-electron quantities derived from preferred gen-side branches (`GenScatEX/Y/Z/E` path)

The plan intentionally separates reco-indexed matched residuals from gen-denominator stability bookkeeping because the branch availability and optimal electron-reconstruction path differ between `ChargedTot` and `ChargedTotGen`.

### 7.2 Stage 1: residuals

Stage 1 reproduces the low-Q²-style kinematic resolution study for the high-Q² schema and must output both headline single-number summaries and value-dependent curves.

#### 7.2.1 Variables to study

| Variable | Reco quantity | Gen quantity | Residual form |
|---|---|---|---|
| `Q2` | reco nominal high-Q² `Q2` | matched gen `Q2` derived on the reco sample or from truth sample as appropriate | `Q2_reco - Q2_gen`; plus `log10(Q2_reco / Q2_gen)` |
| `x` | reco nominal `x` | matched gen `x` | `x_reco - x_gen`; plus `log10(x_reco / x_gen)` |
| `z` | reco nominal `z` | matched gen `z` | `z_reco - z_gen` |
| `pT_h` | reco nominal `pT_h` | matched gen `pT_h` | `pTh_reco - pTh_gen` |
| `phi_h` | reco nominal `phi_h` | matched gen `phi_h` | wrapped angular residual in the principal interval, e.g. `(-pi, pi]` |

#### 7.2.2 Residual summary statistics

For each variable, the script should compute at minimum:

| Statistic | Meaning |
|---|---|
| `bias` | Weighted mean of the residual distribution. |
| `resolution_rms` | Weighted RMS of the residual distribution. |
| `resolution_rms_robust` | Optional robust cross-check (e.g. central quantile RMS or IQR-derived sigma) to diagnose outlier tails; useful but not the headline criterion. |
| `n_events` | Effective selected matched count. |
| `sumw` | Total selected weight. |
| `underflow_overflow_fraction` | Fraction outside plotted window, if bounded histogram windows are used. |

For `Q2` and `x`, the script must also report:

- `bias_log10_ratio = mean(log10(reco/gen))`
- `resolution_rms_log10_ratio = RMS(log10(reco/gen))`

Those log-space forms are not optional because the user explicitly requires the bin-width-vs-resolution comparison in log space for `Q2` and `x`.

#### 7.2.3 RMS vs variable-value curves

For each variable, the script should bin the matched sample in the generator value and compute the residual RMS in each slice.

Recommended policy:

| Variable | Slice axis | Default x-axis scale | Curve quantity |
|---|---|---|---|
| `Q2` | generator `Q2` | log | RMS of `Q2_reco - Q2_gen` and RMS of `log10(Q2_reco/Q2_gen)` |
| `x` | generator `x` | log | RMS of `x_reco - x_gen` and RMS of `log10(x_reco/x_gen)` |
| `z` | generator `z` | linear | RMS of `z_reco - z_gen` |
| `pT_h` | generator `pT_h` | linear | RMS of `pTh_reco - pTh_gen` |
| `phi_h` | generator `phi_h` | linear/circular | RMS of wrapped angular residual |

The JSON should store both the slice-level curves and a reduced form aligned to the nominal bin edges so Stage 2 can compare per-bin widths to local `2*RMS` values directly.

#### 7.2.4 Small sqrt(s) sanity check

Because P9 requires per-event beam-energy handling, the script should also derive `sqrt(s)` using the helper-backed per-event beam energies and record:

- min / max / mean / RMS of `sqrt(s)` for each generator and combined sample
- a small histogram figure or inset plot to demonstrate the expected `[318.1, 319.2] GeV` span with mean near `318.78 GeV`

This is a methodological sanity check rather than a binning metric, but it is essential because a hidden regression to fixed nominal beam energies would directly bias the kinematics and the resolution study.

### 7.3 Stage 2: bin widths vs `2*RMS`

This stage turns the Stage 1 curves into an audit of the frozen bin widths.

#### 7.3.1 General policy

For each nominal axis and each populated nominal bin, compute:

- the local nominal bin width
- the local resolution RMS representative of that bin
- the comparison metric `width / (2 * RMS)`
- a boolean pass/fail flag for `width >= 2 * RMS`

The implementation should evaluate this in the space relevant to the variable:

| Axis | Width space | RMS space | Criterion interpretation |
|---|---|---|---|
| `Q2` | `Delta log10(Q2)` | RMS of `log10(Q2_reco/Q2_gen)` | Pass if log-width exceeds `2*RMS_log10_ratio`. |
| `x` | `Delta log10(x)` | RMS of `log10(x_reco/x_gen)` | Pass if log-width exceeds `2*RMS_log10_ratio`. |
| `z` | linear | RMS of `z_reco - z_gen` | Pass if `Delta z >= 2*RMS`. |
| `pT_h` | linear | RMS of `pTh_reco - pTh_gen` | Pass if `Delta pTh >= 2*RMS`. |
| `phi_h` | N/A for nominal frozen audit | RMS of wrapped angular residual | Report separately as a diagnostic; do not convert into a frozen-bin pass/fail because `phi_h` is not a nominal axis here. |

#### 7.3.2 Representative RMS per nominal bin

The script should derive a per-nominal-bin RMS for each axis using one of the following, with the chosen rule frozen in metadata:

1. default: RMS measured from matched events whose generator value lies inside that nominal bin;
2. optional cross-check: worst RMS among finer sub-slices fully contained in the bin;
3. optional conservative summary: max of generator-weighted RMS and reco-weighted RMS if asymmetry is noticeable.

The narrative should state clearly that any fail flag is an input to unfolding/systematics treatment, not a trigger to alter `HQ-v5f-r2`.

#### 7.3.3 Stage-2 output tables

The JSON should expose, per axis:

| Key family | Contents |
|---|---|
| `bin_widths_nominal` | Array of nominal widths in the relevant comparison space. |
| `resolution_rms_per_nominal_bin` | Array of local RMS values. |
| `two_rms_per_nominal_bin` | `2*RMS` array. |
| `width_over_two_rms` | Ratio array. |
| `pass_mask_populated` | Boolean array restricted to populated bins. |
| `n_populated_bins` | Count of populated bins contributing to the axis pass fraction. |
| `n_failed_bins` | Count of populated bins failing the criterion. |
| `failed_bin_labels` | Human-readable labels for each failed nominal interval. |

### 7.4 Stage 3: 1D and 2D purity / stability

This stage provides the familiar migration diagnostics that bridge residual-level intuition and full 4D cell behavior.

#### 7.4.1 1D response construction

For each axis (`Q2`, `x`, `z`, `pT_h`), build a weighted response matrix projected on that axis while integrating over the others under the same matched-MC selection. The implementation should compute:

- 1D purity per bin
- 1D stability per bin
- median / mean purity across populated bins
- median / mean stability across populated bins
- fraction of populated bins above `0.5`

The figure should mirror the low-Q² style: purity and stability drawn together against bin centers, with uncertainty bars where meaningful and with fail / caution thresholds visually marked.

#### 7.4.2 2D purity slice construction

Two dedicated 2D purity products are required:

1. `(Q2, pT_h)`
2. `(z, pT_h)`

The script should define and document precisely how each 2D purity value is formed. The preferred policy is:

- project the full matched 4D response onto the requested two axes by summing weights over the other axes;
- purity in each 2D cell = diagonal projected weight / total reco-projected weight in that 2D cell.

This avoids inconsistencies between separately built 2D samples and the 4D response. The markdown memo should say explicitly that these are projected 2D purities, not independent two-axis analyses.

#### 7.4.3 Stability treatment in 2D products

The user explicitly requires 2D purity heatmaps, not 2D stability heatmaps. The JSON should still store 2D stability tables for completeness if computationally cheap, but only purity needs to be a required plot deliverable.

### 7.5 Stage 4: 4D per-cell purity table + heatmap projections

This stage is the heart of the audit because the frozen `HQ-v5f-r2` decision lives in 4D.

#### 7.5.1 Nominal 4D cell indexing

The script should define a deterministic index convention for all 480 cells:

| Axis order | Recommended order |
|---|---|
| Index flattening order | `Q2 -> x -> z -> pT_h` |
| Human-readable label | `Q2[i], x[j], z[k], pTh[l]` interval strings |
| Charge handling | If charge-separated summaries are desired later, keep charge as metadata, not as part of the 480-cell nominal grid count used here. |

The JSON must make the indexing explicit so downstream scripts cannot misread the flattened arrays.

#### 7.5.2 Required 4D metrics per populated cell

For every populated nominal 4D cell, store:

- cell index and interval labels
- reco count / gen count / diagonal count (weighted and, if useful, unweighted)
- purity
- stability
- axis-local `width_over_two_rms` values for `Q2`, `x`, `z`, `pT_h`
- per-axis pass/fail flags
- overall “migration dominates” flag if purity < `0.5` or any required axis fails `2*RMS`
- “unfolding required” flag, semantically aligned to the finalization plan’s usage
- generator-separated purity values and absolute difference

This table must be rich enough that downstream systematics / finalization tasks do not need to re-open the ROOT files to classify cells.

#### 7.5.3 4D purity headline summaries

The JSON and markdown should report at minimum:

| Summary | Requirement |
|---|---|
| `n_total_cells` | Must be 480. |
| `n_populated_cells` | Expected to reconcile to the current 320 populated cells anchor unless the selection definition differs. |
| `fraction_populated` | `n_populated / 480`. |
| `purity_4d_median_over_populated_bins` | Must be compared directly to the existing `0.626` anchor. |
| `stability_4d_median_over_populated_bins` | Report and interpret. |
| `n_cells_purity_below_0p5` | Required for unfolding-needed classification. |
| `n_cells_fail_two_rms_any_axis` | Required for migration-dominance classification. |
| `n_cells_flagged_for_unfolding` | Union flag used by downstream consumers. |

#### 7.5.4 4D heatmap projection figure

Because a true 4D heatmap is impossible to visualize directly, `fig_r07_4d_purity_heatmap.png` should be a structured projection that preserves the 4D cell story. Recommended design:

- panel grid over `(Q2, x)` macro-cells;
- within each panel, show a `z × pT_h` heatmap of 4D purity values;
- annotate empty cells distinctly;
- visually mark cells with purity `<0.5`, failed `2*RMS`, or generator disagreement `>0.05` using overlays / hatch / glyphs.

This produces a reviewer-friendly figure without redefining the 4D metric itself.

### 7.6 Stage 5: per-generator comparison

The user requires a direct RAPGAP31-vs-DJANGOH14 response comparison.

#### 7.6.1 Required quantities

For each nominal 4D populated cell, compute:

- `purity_rapgap31`
- `purity_djangoh14`
- `stability_rapgap31`
- `stability_djangoh14`
- `delta_purity_abs = abs(purity_rapgap31 - purity_djangoh14)`
- `delta_stability_abs` (recommended secondary metric)
- `generator_disagreement_flag = delta_purity_abs > 0.05`

#### 7.6.2 Generator-comparison summaries

The narrative should include:

- number and fraction of populated cells with `delta_purity_abs > 0.05`
- worst offending cells with interval labels
- whether disagreement clusters in low-`z`, high-`pT_h`, edge-`Q2`, or sparse-`x` regions
- whether disagreement aligns with already-low purity or reveals otherwise-hidden model dependence

#### 7.6.3 How this stage informs later work

Any flagged cells are not grounds to reopen `HQ-v5f-r2`. Instead, they become explicit migration-model-sensitivity inputs for unfolding choice and systematics interpretation. The markdown memo should say this directly to avoid review drift.

### 7.7 Stage 6: alternative-binning sensitivity scan

This stage is a robustness demonstration, not a replacement study.

#### 7.7.1 Minimum required variants

At minimum, implement the following alternative schemes:

| Variant ID | Axis modified | Required intent |
|---|---|---|
| `z_halfwidth` | `z` | Narrower-by-half relative to nominal local widths, clipped to `[0.0, 1.0]`. |
| `z_doublewidth` | `z` | Wider-by-2× relative to nominal local widths, while preserving domain endpoints. |
| `pTh_halfwidth` | `pT_h` | Narrower-by-half relative to nominal local widths, clipped to `[0.0, 8.0]`. |
| `pTh_doublewidth` | `pT_h` | Wider-by-2× relative to nominal local widths, preserving domain endpoints. |

The exact edge-building recipe must be deterministic and stored in the JSON under each variant block.

#### 7.7.2 Quantities to compare

For each variant, compute at minimum:

- 1D purity distributions on the modified axis
- projected 4D or hybrid purity summary consistent with the chosen variant definition
- change relative to nominal: `delta_purity = purity_variant - purity_nominal`
- max absolute delta across populated bins
- median absolute delta across populated bins
- pass/fail versus the user threshold `delta_purity < 0.10` absolute for half-width variants

#### 7.7.3 Interpretation rule

The narrative should explicitly frame this as a robustness scan showing whether the nominal response picture is unusually fragile. A failure means “response sensitivity must be documented and propagated,” not “reopen the frozen grid.”

## 8. Schema map

The implementation must freeze the branch-to-quantity map explicitly because this is where high-Q² differs from low-Q².

| Quantity family | Source tree | Branches / path | Usage in this study | Caveat |
|---|---|---|---|---|
| Reco beam energies | `ChargedTot` | `BeamElecE`, `BeamProtP` | All reco-side kinematic derivations and `sqrt(s)` sanity checks | Must go through `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/_beam_energy_utils.py`; no scalar hardcode. |
| Gen beam energies on matched reco sample | `ChargedTot` | `GenBeamElecE`, `GenBeamProtP` | Matched-sample gen electron reconstruction and gen-side kinematics used for residuals | Required by P9. |
| Gen beam energies on truth tree | `ChargedTotGen` | `GenBeamElecE`, `GenBeamProtP` | Truth-side denominator and `sqrt(s)` summaries | Use helper-backed normalization / fallbacks only. |
| Reco event kinematics | `ChargedTot` | Nominal reco `Q2`, `x`, `y`, `Empz`, `VtxZ` fields | Event selection and reco residual side | Must be the same nominal high-Q² baseline used elsewhere in the analysis. |
| Reco track kinematics | `ChargedTot` | `Pt`, `Eta`, `NHitDedx`, `RadLength`, `DcaPrime` | Track selection | Use frozen F5/F6 policy. |
| Matched gen hadron kinematics on reco tree | `ChargedTot` | `xGen`, `Q2sGen`, `YsGen`, matched `zGen`, matched `pThGen`, matched `phi_h_gen` or equivalent truth branches available in the tree | Residual comparisons on the matched sample | Exact branch names must be verified during implementation; selection logic should not assume low-Q² names blindly. |
| Gen scattered-electron azimuth on reco tree | `ChargedTot` | `GenElecPhi` | Input to inverse electron-method reconstruction on the matched sample | Must raise / fail loudly if non-finite in selected events. |
| Gen scattered-electron 4-vector on reco tree | `ChargedTot` | Reconstruct via inverse electron method from `(Q2sGen, YsGen, xGen, GenElecPhi, GenBeamElecE, GenBeamProtP)` | High-Q² matched-sample gen electron path | **Critical caveat:** do **not** use the low-Q² `gen_electron_4vec_from_polar(GenElecE, GenElecTheta, GenElecPhi)` path on `ChargedTot`. |
| Stored gen scattered-electron 4-vector on gen tree | `ChargedTotGen` | `GenScatEX`, `GenScatEY`, `GenScatEZ`, `GenScatEE` | Preferred gen-side denominator / truth path | Preferred whenever operating directly on `ChargedTotGen`. |
| Matching gate | `ChargedTot` | `IsMatched` | Restricts residual / purity numerator to truth-linked reco tracks | Mandatory in the matched-MC selection. |
| Source veto | `ChargedTot` | `ParticleSource` | Exclude `ParticleSource == 4` | Must be explicit in metadata and narrative. |
| Charge agreement | `ChargedTot` | pipeline-standard reco/gen charge comparison fields | Ensure matched reco/gen hadron charge consistency | Must use frozen high-Q² logic; do not improvise a new rule. |
| Truth fiducial | `ChargedTot` and `ChargedTotGen` | `GenPt`, `GenEta` | Apply `GenPt > 0.15`, `|GenEta| < 1.3` | Needed both in matched sample and truth denominator path. |
| Generator ID | file-group metadata | RAPGAP31 or DJANGOH14 inferred from input file pattern | Generator-separated summaries | Keep explicit in every output block. |

## 9. Definitions

The implementation and result memo should repeat these definitions verbatim.

| Quantity | Definition |
|---|---|
| `purity(i)` | `sumw(reco in i AND gen in i) / sumw(reco in i)` |
| `stability(i)` | `sumw(reco in i AND gen in i) / sumw(gen in i)` |
| `resolution_RMS(v)` | RMS of `(v_reco - v_gen)` over matched events |
| `bias(v)` | Mean of `(v_reco - v_gen)` over matched events |
| `resolution_RMS_log10_ratio(Q2)` | RMS of `log10(Q2_reco / Q2_gen)` over matched events |
| `resolution_RMS_log10_ratio(x)` | RMS of `log10(x_reco / x_gen)` over matched events |
| `two_rms_threshold(v)` | `2 * resolution_RMS(v)` or `2 * resolution_RMS_log10_ratio(v)` in the comparison space |
| `migration_dominates` | Cell-level diagnostic flag set if `purity < 0.5` or any nominal-axis `width < 2*RMS` for the populated cell |
| `unfolding_required` | Cell-level downstream flag meaning the audit recommends unfolding-aware treatment instead of a bin-by-bin assumption |
| `delta_purity_abs` | Absolute difference in purity between RAPGAP31 and DJANGOH14 for the same populated nominal bin |

For `phi_h`, the script should define the residual using a wrapped difference, e.g.:

- `delta_phi = atan2(sin(phi_reco - phi_gen), cos(phi_reco - phi_gen))`

so the residual statistics are not polluted by the `2*pi` branch cut.

## 10. Pass criteria

The script, JSON, and markdown memo should all carry the same threshold table.

| Criterion family | Threshold | Interpretation |
|---|---|---|
| Resolution adequacy per populated nominal bin | `bin width >= 2 * RMS_resolution` | Main audit of whether the frozen bins are wider than twice the local reco-gen resolution. |
| 4D per-cell purity | `purity >= 0.5` or else flagged | Cells below `0.5` are explicitly flagged for unfolding-required / migration-dominant treatment. |
| Alternative-binning sensitivity | `delta-purity < 0.10` absolute for half-width scans | Demonstrates robustness of the nominal response picture. |
| Generator agreement | `abs(purity_RAPGAP - purity_DJANGOH) < 0.05` | Cells above `0.05` are explicitly flagged for generator-response disagreement. |
| Beam-energy policy gate | `verify_beam_energy_usage.py` reports `0 FAIL` | Required before full NAF launch. |
| Numerical sanity | purity and stability in `(0,1]` for populated bins, finite RMS values, finite `sqrt(s)` distribution, no impossible count signs | Required for critic gates. |

A failure against these criteria must be reported as a flag in the audit outputs and propagated to downstream systematics / unfolding decisions; it must **not** be framed as a reason to revise the frozen `HQ-v5f-r2` grid inside this workstream.

## 11. Outputs

### 11.1 JSON output contract

Primary file:

- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_resolution_HQv5fr2.json`

The JSON should contain the following top-level keys.

| Top-level key | Required contents |
|---|---|
| `metadata` | script version, timestamp, git commit if available, workspace path, input file globs, file counts, tree names, chunking config, weighting summary, and selection strings. |
| `analysis_scope` | explicit statement that this is an audit of frozen `HQ-v5f-r2`, not a rebinning proposal. |
| `nominal_binning` | exact edges for `Q2`, `x`, `z`, `pTh`, total nominal cell count, flattening order, and references to frozen-bin source file. |
| `beam_energy_policy` | helper path, verification command, verification status, reco/gen branch names used, and `sqrt(s)` summary numbers. |
| `input_samples` | RAPGAP31 and DJANGOH14 file counts, processed file counts, skipped file counts, selected-event totals, and effective sumw totals. |
| `definitions` | the formulas from Section 9. |
| `residuals` | per-variable global hist summaries and per-generator blocks. |
| `resolution_vs_value` | per-variable slice curves, bin centers, RMS values, bias values, counts, and generator-separated variants. |
| `binning_vs_resolution` | per-axis nominal widths, local RMS, `2*RMS`, ratios, populated masks, fail lists, and pass fractions. |
| `purity_stability_1d` | per-axis purity and stability arrays, medians, means, counts, and threshold masks. |
| `purity_2d` | projected `(Q2,pTh)` and `(z,pTh)` purity tables, optional stability tables, labels, and population masks. |
| `purity_4d` | per-cell arrays / records, populated-cell summaries, headline medians, fail counts, and heatmap-ready projected blocks. |
| `generator_comparison` | per-cell RAPGAP-vs-DJANGOH differences plus summary statistics and worst-case lists. |
| `alternative_binning_scan` | exact alt edge sets, variant-wise purity results, delta tables, summary pass/fail, and sensitivity headline numbers. |
| `reconciliation_with_existing_artifacts` | direct comparison to `migration_matrices_v5f_r2_default.json` and optionally to `unfolding_compare_v5f_HQv5fr2*.json`. |
| `flags` | lists of cells failing `2*RMS`, cells below purity `0.5`, cells with generator disagreement `>0.05`, and union flagged cells. |
| `outputs_written` | absolute paths of plots and markdown memo written by the run. |

The JSON should further include the following nested keys under each major block.

#### `metadata`

- `plan_reference`
- `frozen_scheme_name`
- `analysis_regime`
- `created_utc`
- `script_path`
- `input_globs`
- `tree_names`
- `weight_formula_description`
- `selection_summary`
- `generator_labels`
- `notes`

#### `residuals`

For each variable `Q2`, `x`, `z`, `pTh`, `phi_h`:

- `global_bias`
- `global_rms`
- `global_mean_abs`
- `n_events`
- `sumw`
- `histogram_range`
- `histogram_bin_edges`
- `histogram_counts_weighted`
- `per_generator`
- and for `Q2` and `x` additionally:
  - `global_bias_log10_ratio`
  - `global_rms_log10_ratio`
  - `histogram_log10_ratio`

#### `binning_vs_resolution`

For each nominal axis:

- `comparison_space`
- `nominal_edges`
- `nominal_widths`
- `resolution_rms_per_bin`
- `two_rms_per_bin`
- `width_over_two_rms`
- `populated_mask`
- `pass_mask`
- `failed_bins`
- `pass_fraction_populated`
- `worst_ratio`
- `per_generator`

#### `purity_4d`

- `n_total_cells`
- `n_populated_cells`
- `fraction_populated`
- `purity_4d_median_over_populated_bins`
- `stability_4d_median_over_populated_bins`
- `per_cell`
  - `flat_index`
  - `q2_bin`
  - `x_bin`
  - `z_bin`
  - `pth_bin`
  - `label`
  - `is_populated`
  - `sumw_reco`
  - `sumw_gen`
  - `sumw_diag`
  - `purity`
  - `stability`
  - `two_rms_axis_flags`
  - `purity_flag_below_0p5`
  - `migration_dominates`
  - `unfolding_required`
  - `purity_rapgap31`
  - `purity_djangoh14`
  - `delta_purity_abs`

### 11.2 Markdown result memo structure

Primary file:

- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/results/binning_resolution_HQv5fr2.md`

Required section list:

1. `Title / metadata`
2. `Purpose and scope`
3. `Frozen HQ-v5f-r2 binning under audit`
4. `Input samples and selections`
5. `Schema map and high-Q² gen-electron caveat`
6. `Residual-resolution results`
7. `Bin-width vs 2*RMS audit`
8. `1D purity and stability`
9. `2D purity slices`
10. `4D cell audit and flagged regions`
11. `RAPGAP31 vs DJANGOH14 comparison`
12. `Alternative-binning sensitivity`
13. `Reconciliation to migration_matrices_v5f_r2_default.json`
14. `Impact on unfolding-required and systematics bookkeeping`
15. `Figure inventory`
16. `Headline acceptance statement`

The narrative must include a short “do not reinterpret as rebinning” statement in the scope section.

### 11.3 Plot inventory

Required plots and intended contents:

| Filename | Required contents |
|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r01_resolutions.png` | Multi-panel residual histograms for `Q2`, `x`, `z`, `pTh`, `phi_h`, with bias and RMS annotations. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r02_resolution_vs_value.png` | RMS-vs-value curves for all variables; `Q2` and `x` shown in log-aware form. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r03_binning_vs_resolution.png` | For each nominal axis, overlay bin widths and `2*RMS`, with failed bins highlighted. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r04_purity_stability_1d.png` | 1D purity/stability curves for `Q2`, `x`, `z`, `pTh`. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r05_purity_2d_Q2_pTh.png` | 2D projected purity heatmap in `(Q2,pTh)`. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r06_purity_2d_z_pTh.png` | 2D projected purity heatmap in `(z,pTh)`. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r07_4d_purity_heatmap.png` | Panelized 4D purity projection over `(Q2,x)` with internal `(z,pTh)` heatmaps and flag overlays. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r08_alt_binning_scan_z.png` | `z` nominal-vs-alt purity sensitivity visualization, including half-width and double-width variants. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_r09_alt_binning_scan_pTh.png` | `pT_h` nominal-vs-alt purity sensitivity visualization. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/fig_aux01_sqrt_s_sanity.png` | Small per-event `sqrt(s)` sanity-check histogram / comparison plot. |

## 12. Implementation design details

### 12.1 Preferred script interface

The new script should expose a CLI approximately of the following form (illustrative interface contract, not code):

| Argument | Purpose |
|---|---|
| `--generator {rapgap31,djangoh14,combined}` | Control sample selection and whether to run one generator or both. |
| `--input-glob-rapgap <glob>` | Override default RAPGAP input glob if needed. |
| `--input-glob-djangoh <glob>` | Override default DJANGOH input glob if needed. |
| `--output-json <path>` | Primary JSON output path. |
| `--output-md <path>` | Primary markdown output path. |
| `--plot-dir <path>` | Plot output directory. |
| `--max-files N` | Smoke-test limiter. |
| `--chunk-size N` | Chunk size for uproot iteration. |
| `--skip-alt-binning` | Optional speed shortcut for debugging. |
| `--skip-plots` | Optional headless mode for JSON-only testing. |
| `--compare-existing-json <path>` | Existing migration JSON for reconciliation checks. |
| `--strict-schema` | Fail fast on missing critical branches. |

### 12.2 Internal script modules / functions to isolate

Even if the implementer writes one script file, the logic should be organized into stable internal sections.

| Function block | Responsibility |
|---|---|
| `input discovery` | Resolve file lists, count files, and tag generator origin. |
| `schema validation` | Confirm required branches are present before heavy processing. |
| `beam-energy validation hook` | Run or at minimum document the required scanner check before full launch. |
| `kinematic derivation` | Compute reco and gen observables, including the high-Q²-specific gen electron reconstruction path. |
| `selection masks` | Build base reco, gen fiducial, matched, charge-agreement, and populated-bin masks. |
| `response builders` | Construct 1D, 2D, and 4D response arrays with consistent weighting. |
| `residual statistics` | Compute bias, RMS, log-ratio RMS, and RMS-vs-value slices. |
| `alt-binning engine` | Generate alternative edges and recompute selected purity summaries. |
| `generator comparison` | Compare RAPGAP31 and DJANGOH14 outputs cell-by-cell. |
| `artifact writers` | Emit JSON, markdown, and figures using a single in-memory result object. |

### 12.3 Data-flow order

Recommended run order:

1. discover files and validate branch presence on one file per generator;
2. validate or at least pre-run the beam-energy scanner on the new script path;
3. load matched reco chunks and derive Stage-1 residual quantities;
4. compute per-axis RMS-vs-value and local RMS tables;
5. build 1D / 2D / 4D responses and purity / stability metrics;
6. compute generator-separated summaries;
7. compute alternative-binning variants;
8. compare to existing migration JSON and unfolding JSON anchors;
9. write JSON;
10. write markdown memo from the JSON-derived summary object;
11. write plots from the same object;
12. run post-write sanity checks on finite outputs and path inventory.

This order ensures that all rendered outputs are driven by the same numbers and that the markdown memo cannot diverge from the JSON.

## 13. NAF rerun procedure

This section describes the implementation-run procedure the eventual implementer should follow after plan approval.

### 13.1 SSH preflight

Per project safety rules, start every remote dispatch with:

```bash
```


### 13.2 Code sync / preflight steps on NAF

Assuming the approved script has been implemented under the high-Q² workspace, the operator should:

1. ensure the script exists under the active code root mirrored on NAF;
2. source the H1OO environment if any helper import chain requires it;
3. run the beam-energy scanner against the new script;
4. run a local import / `--help` smoke check;
5. run a one-file-per-generator smoke test before any full production launch.

Representative command sequence the plan expects the implementer to support:

```bash
ssh naf-hone.desy.de "source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh && cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2 && /usr/bin/python3 scripts/verify_beam_energy_usage.py scripts/study_binning_resolution_HQv5fr2.py"
ssh naf-hone.desy.de "cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2 && /usr/bin/python3 scripts/study_binning_resolution_HQv5fr2.py --help"
ssh naf-hone.desy.de "cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2 && /usr/bin/python3 scripts/study_binning_resolution_HQv5fr2.py --max-files 1 --output-json docs/data/binning_resolution_HQv5fr2_smoke.json --output-md docs/results/binning_resolution_HQv5fr2_smoke.md --plot-dir plots/binning_resolution_HQv5fr2_smoke"
```

### 13.3 Smoke-test requirement

The smoke test must use:

- exactly 1 RAPGAP31 file
- exactly 1 DJANGOH14 file

Smoke-test success criteria:

| Check | Required outcome |
|---|---|
| Script import / CLI | exits cleanly |
| Beam-energy scanner | `0 FAIL` |
| Residual histograms | non-empty for all five variables |
| RMS values | finite for all five variables |
| 1D purity arrays | contain at least one populated bin per nominal axis |
| 4D response | contains at least one populated cell |
| Output artifacts | JSON, markdown, and at least key plots written |

### 13.4 Full launch mode

Because this is an analysis script, not a heavy builder production, the full run may be feasible interactively if optimized and chunked. However, the plan should assume Condor if wall-clock exceeds ~10 minutes or if memory use is non-trivial across >1200 files total.

Recommended full-launch options:

| Mode | Use when | Suggested approach |
|---|---|---|
| Interactive NAF run | Script completes comfortably under ~10 minutes and memory stays modest | one-shot execution on login node may be acceptable, but only after smoke confirmation. |
| Condor split-by-generator | If each generator can be processed independently then combined | submit one RAPGAP and one DJANGOH job, then merge JSON summaries locally / in a follow-up step. |
| Condor split-by-file-chunk | If per-generator run time is still too long | process fixed file chunks into temporary partial JSON fragments, then reduce them in a deterministic combine step. |

If Condor is used, request the standard profile:

- `+RequestRuntime = 10800`
- `RequestMemory = 2000`
- `RequestCpus = 1`

### 13.5 Expected wall-clock guidance

The plan cannot lock an exact runtime without implementation, but it should set operational expectations:

| Phase | Expected scale |
|---|---|
| 1+1 file smoke test | a few minutes at most |
| Full per-generator scan without alt-binning | likely tens of minutes if purely Python / uproot and unoptimized |
| Full scan with alt-binning and full plotting | potentially up to order 1 hour unless chunked / cached efficiently |
| Condor chunked run | dominated by I/O and chunk-reduction overhead rather than CPU-only histogramming |

The script should be written to avoid re-reading the ROOT inputs separately for each stage wherever possible.

## 14. Trade-offs and design considerations

### 14.1 New dedicated high-Q² script vs reusing low-Q² scripts directly

| Option | Benefit | Cost | Planned choice |
|---|---|---|---|
| Patch low-Q² scripts in place | Maximum code reuse | High risk of low-/high-Q² schema entanglement, especially around gen electron reconstruction and beam policy | No |
| New high-Q² dedicated script with targeted helper reuse | Clean high-Q² schema handling; easier beam-policy audit; clearer reviewer ownership | Some duplicated orchestration logic | **Yes** |

### 14.2 Matched-sample residuals vs gen-tree-only truth derivation

| Choice | Rationale |
|---|---|
| Use matched reco sample for residuals | Residuals are inherently reco-vs-gen comparisons and therefore should live on the matched sample. |
| Use `ChargedTotGen` preferred electron path for truth-only denominators and consistency checks | This uses the most reliable gen-side 4-vector available on the truth tree. |

### 14.3 Projected 2D purity vs independent 2D response builds

Projected 2D purity from the 4D response is preferred because it keeps the 2D plots numerically anchored to the actual 4D binning under audit. Independent 2D responses would be easier to compute but risk drifting from the 4D numbers later cited in unfolding / systematics discussions.

### 14.4 Combined nominal summary vs per-generator only

The plan assumes both are needed:

- per-generator outputs are required for the disagreement audit;
- a combined nominal block is required because downstream reviewers usually want one headline table.

The combined block must be transparently documented so it cannot be confused with a third independent sample.

### 14.5 Half-width / double-width alternative-binning scan complexity

The alternative-binning stage can become combinatorially large if implemented too generally. The plan therefore limits the minimum required variants to `z` and `pT_h`, each with half-width and double-width scans. The implementer should resist turning this into a generic grid-search framework unless specifically requested later.

## 15. Risk register

| Risk ID | Risk | Why it matters | Mitigation in plan |
|---|---|---|---|
| R1 | High-Q² reco-tree gen-electron reconstruction uses the wrong low-Q² helper path | Would silently bias gen kinematics and therefore residuals, `phi_h`, and migration metrics | Freeze the schema map and require the inverse electron-method helper path from `(Q2sGen, YsGen, xGen, GenElecPhi, GenBeamElecE, GenBeamProtP)`. |
| R2 | Per-event beam energies are accidentally replaced by global `27.6/920` constants | Violates P9 and corrupts `sqrt(s)` and kinematics at a subtle level | Require helper use everywhere and scanner `verify_beam_energy_usage.py` with `0 FAIL` before full launch. |
| R3 | `ParticleSource` handling differs from the existing migration JSON | Could prevent reconciliation to the `0.626` median anchor | Explicitly freeze `ParticleSource != 4` in the matched-MC selection and require reconciliation section in the result memo. |
| R4 | Non-finite `GenElecPhi` or malformed inverse-electron inputs survive into selected events | Could create NaNs in `phi_h` and truth kinematics silently | Fail loudly on non-finite critical branches in the selected sample and surface counts in JSON metadata. |
| R5 | Empty cells inflate or distort 4D medians | Would make comparison to the existing migration JSON meaningless | Compute all headline medians over populated bins only and store empty-bin bookkeeping separately. |
| R6 | Generator-comparison mismatch is driven by inconsistent selection rather than true model difference | Could overstate model dependence | Use one shared selection and response engine for both generators. |
| R7 | Alternative-binning variants are defined ambiguously | Could make `delta-purity` results non-reproducible | Store explicit alternative edge arrays and construction notes in JSON. |
| R8 | Circular `phi_h` residual is treated linearly | Would produce fake large residual tails at the branch cut | Define wrapped residual explicitly in the definitions and implementation contract. |
| R9 | Interactive full run is too slow / memory-heavy | Could stall the workflow or produce partial outputs | Require a smoke test and allow generator- or chunk-split Condor fallback. |
| R10 | Existing `migration_matrices_v5f_r2_default.json` differs for legitimate but undocumented reasons (e.g. weighting, sample, empty-bin policy) | Could trigger false alarms in CG-6 | Require a dedicated reconciliation block that compares selection, weights, and population definitions before calling it a discrepancy. |

## 16. Critic gates

### CG-1: plan-critic review pre-implementation

Before any code is written, dispatch a planning critic against this plan to verify that the output contract, schema caveat handling, frozen-grid scope guard, and NAF procedure are sufficiently explicit for implementation without reopening requirements.

### CG-2: script imports cleanly and beam-energy scanner shows `0 FAIL`

After implementation but before any full run, the new script must import / expose `--help` without runtime errors and `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/verify_beam_energy_usage.py` must report `0 FAIL` on the script.

### CG-3: smoke test produces non-empty residual histograms with finite RMS

A one-RAPGAP plus one-DJANGOH smoke run must emit non-empty residual outputs for `Q2`, `x`, `z`, `pTh`, and `phi_h`, with finite bias and RMS values and at least one populated nominal cell in the 4D response.

### CG-4: full NAF outputs are numerically sane

The full-stat run must yield purity and stability values in `(0,1]` for populated bins, finite resolution metrics, a plausible `sqrt(s)` envelope consistent with P9, and flag counts that are logically consistent with the populated-bin counts.

### CG-5: plot-reviewer sign-off on figure set

After figure production, dispatch plot review on the full `fig_r01`–`fig_r09` family plus the auxiliary `sqrt(s)` check to confirm the plots are visually sane, threshold annotations are readable, and no panel suggests pathological bin bookkeeping.

### CG-6: analysis-critic sign-off on reconciliation to `migration_matrices_v5f_r2_default.json`

The final memo must be reviewed specifically for whether the new study reproduces the existing `purity_4d_median_over_populated_bins = 0.626` anchor or explains any difference in a way the analysis critic judges physically and procedurally acceptable.

## 17. Acceptance

Required one-line headline acceptance statement format for supervisor sign-off:

> `HQ-v5f-r2 binning audit PASS/CONDITIONAL: the frozen high-Q² charged-SIDIS grid was audited against per-event reco-gen resolution and migration using matched MC; [N_pass/N_populated] populated bins satisfy width >= 2*RMS on all nominal axes, the 4D populated-cell median purity is [value], [N_flagged] cells are flagged for unfolding-required treatment, and no finding in this study reopens the frozen grid.`

## 18. Implementation checklist notes for the future implementer

The eventual implementation should preserve the following anchor decisions exactly:

1. do not modify `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_v5f_frozen.json`;
2. do not route the reco-tree gen electron through low-Q² `GenElecE/Theta` logic;
3. do not hardcode nominal beam energies outside allowed helper fallbacks;
4. do not interpret Stage-2 failures as a bin-redesign recommendation;
5. do not compute headline medians over empty cells;
6. do not compare RAPGAP and DJANGOH using different masks or weight formulas;
7. do not let the markdown memo handwave discrepancies against the existing migration JSON without an explicit selection / weighting reconciliation table.

## 19. Todo

- [ ] Create `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/study_binning_resolution_HQv5fr2.py` as a dedicated high-Q² audit driver rather than patching low-Q² scripts in place.
- [ ] Validate required branch availability on one RAPGAP31 and one DJANGOH14 file before implementing full logic, with explicit attention to reco-tree matched-gen branches and gen-tree `GenScatEX/Y/Z/E` availability.
- [ ] Reuse `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/_beam_energy_utils.py` for all reco/gen beam handling and document the exact helper functions used.
- [ ] Reuse the inverse electron-method helper pattern from `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/extract_phi_moments_data.py` for reco-tree gen scattered-electron reconstruction from `(Q2sGen, YsGen, xGen, GenElecPhi, GenBeamElecE, GenBeamProtP)`.
- [ ] Freeze and document the matched-MC selection string: event cuts, track cuts, `IsMatched == 1`, gen fiducial, `ParticleSource != 4`, and charge agreement.
- [ ] Implement generator-separated file discovery and chunked multi-file reading for RAPGAP31 and DJANGOH14.
- [ ] Build common derived reco/gen observables for `Q2`, `x`, `z`, `pTh`, `phi_h`, and `sqrt(s)` using per-event beams.
- [ ] Compute global residual histograms and weighted bias/RMS summaries for `Q2`, `x`, `z`, `pTh`, and `phi_h`.
- [ ] Compute `log10(reco/gen)` bias and RMS for `Q2` and `x`.
- [ ] Compute RMS-vs-generator-value curves for all five variables and store slice-level tables in the JSON.
- [ ] Derive per-nominal-bin local RMS values for `Q2`, `x`, `z`, and `pTh` using a frozen representative-within-bin rule.
- [ ] Compute nominal bin widths in the correct comparison spaces, including log-space widths for `Q2` and `x`.
- [ ] Build Stage-2 per-axis `width / (2*RMS)` tables, pass masks, and failed-bin label lists.
- [ ] Derive the small `sqrt(s)` sanity-check distribution and write `fig_aux01_sqrt_s_sanity.png`.
- [ ] Build 1D response matrices for `Q2`, `x`, `z`, and `pTh` and compute purity/stability arrays.
- [ ] Build projected 2D purity tables for `(Q2,pTh)` and `(z,pTh)` from the same matched response basis used for 4D results.
- [ ] Build the nominal 4D response on the `Q2 -> x -> z -> pTh` flattening order and compute per-cell purity/stability over all 480 cells.
- [ ] Mark populated vs empty cells explicitly and compute headline medians over populated cells only.
- [ ] Add per-cell flags for `purity < 0.5`, any-axis `width < 2*RMS`, `migration_dominates`, and `unfolding_required`.
- [ ] Compute RAPGAP31 and DJANGOH14 per-cell purity and stability separately, then derive `delta_purity_abs` and generator-disagreement flags.
- [ ] Implement deterministic alternative edge sets for `z_halfwidth`, `z_doublewidth`, `pTh_halfwidth`, and `pTh_doublewidth` without modifying the frozen nominal binning JSON.
- [ ] Recompute the required purity summaries for each alternative-binning variant and store `delta-purity` relative to nominal.
- [ ] Compare the resulting 4D populated-cell median purity and populated-cell count to `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/migration_matrices_v5f_r2_default.json` and record exact reasons for any mismatch.
- [ ] Cross-reference flagged low-purity / migration-dominant cells against `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/unfolding_compare_v5f_HQv5fr2.json` and `_analyzed.json` in the narrative memo.
- [ ] Write `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/data/binning_resolution_HQv5fr2.json` with the full key structure defined in this plan.
- [ ] Write `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/results/binning_resolution_HQv5fr2.md` with the required section list and headline acceptance statement.
- [ ] Render `fig_r01` through `fig_r09` plus the auxiliary `sqrt(s)` plot under `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/binning_resolution_HQv5fr2/`.
- [ ] Run `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/verify_beam_energy_usage.py` on the new script and confirm `0 FAIL` before any full NAF run.
- [ ] Perform a smoke test on exactly one RAPGAP31 file and one DJANGOH14 file, confirming non-empty histograms and finite RMS outputs.
- [ ] If full-stat runtime is too long interactively, split the run by generator or file chunk on Condor using the NAF standard job profile.
- [ ] Dispatch CG-5 plot review on the final figure set.
- [ ] Dispatch CG-6 analysis-critic review specifically on the reconciliation to the existing `0.626` 4D median purity anchor.

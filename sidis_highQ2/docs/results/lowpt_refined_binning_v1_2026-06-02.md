# Low-pT-refined gen-binning TUnfold exploratory study (v1)

**Date:** 2026-06-02
**Status:** EXPLORATORY — does NOT supersede the canonical 210-bin TUnfold AREA release.

## Question

Can we refine the gen pT_h binning in the low-pT_h region (where data
statistics are large) to provide finer-grained TMD inputs, without
destabilizing TUnfold?

## Design

Per-charge layout (does not modify canonical `binning_v3.py`):

- 7 logical cells = 6 normal Scheme B′ cells (S0a, S0b, S1a, S1b, S2a, S2b)
  + 1 S3merged cell (x-integrated over S3 stratum), mirroring the
  210-bin S3merged AREA release design.
- Gen axis:
  - z edges = `[0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` (6 bins, unchanged)
  - pT_h edges = `[0.0, 0.25, 0.5, 0.75, 1.0, 2.0, 4.0, 8.0]` (7 bins,
    REFINED low-pT)
  - Per-cell: 6 × 7 = 42 gen bins; total per charge = 7 × 42 = **294**.
- Reco axis (kept identical to canonical v3):
  - z edges = `[0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6,
    0.7, 0.85, 1.0]` (12 bins)
  - pT_h edges = `[0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0,
    8.0]` (10 bins)
  - Per-cell: 12 × 10 = 120 reco bins; total per charge = 7 × 120 = **840**.
- Reco / gen ratio = 840 / 294 = 2.857 (over-determined).

Reco→gen pT child map (per cell):
- pT bins 0..3 (gen pT < 1 GeV): 1 reco child each (1:1 — identical edges)
- pT bins 4..6 (gen pT in [1, 8) GeV): 2 reco children each.
- z direction: uniform 2:1 split for all gen z bins.

Trusted-bin mask: high-z slice of S3merged (z ≥ 0.5) is conservatively
masked → 14 of 294 bins masked → 280 trusted gen bins.

## TUnfold setup

- Official ROOT **TUnfold V17.9** (ROOT 6.38.04, LCG 109a). No proxy.
- Encoding B per charge: response TH2 shape (841, 295); reco-overflow
  row at i=840 absorbs miss; gen-overflow column at j=294 absorbs fake.
- Custom L matrix with `kRegModeNone` + explicit
  `RegularizeCurvature(iL, iC, iR)` triplets along intra-cell physical
  directions:
  - pT direction: 5 triplets per (cell, iz), 6 iz, 7 cells = **210** triplets
  - z direction: 4 triplets per (cell, ipt), 7 ipt, 7 cells = **196** triplets
  - **Total = 406 triplets** (verified at runtime in the macro).
- Both `kEConstraintArea` and `kEConstraintNone` runs, scanned over 20
  log-spaced tau values in `[1e-7, 1e-2]`. Total 80 TUnfold runs.

## Pipeline & artifacts

| Component | Path |
|---|---|
| Binning module (DO NOT modify canonical) | `scripts/unfolding/tunfold_official_v3_percharge/binning_lowpt_refined_v1.py` |
| Response builder (preserves rev6 F3-F7 fixes via patched v2 builder) | `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_lowpt_refined_v1.py` |
| Merger | `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_lowpt_refined_v1.py` |
| TUnfold macro | `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau_lowpt_refined_v1.C` |
| Tau-scan summary | `scripts/unfolding/tunfold_official_v3_percharge/summarize_tau_scan_lowpt_refined_v1.py` |
| Compare to canonical 210 | `scripts/unfolding/tunfold_official_v3_percharge/compare_lowpt_refined_v1.py` |
| Diagnostic plots | `scripts/unfolding/tunfold_official_v3_percharge/plot_lowpt_refined_v1.py` |

Cluster IDs (NAF):
- Build: cluster **3588710** (8 jobs, all SUCCESS, ~ 2-15 min each)
- Tau scan: cluster **3588836** (80 jobs, all SUCCESS, ~ 1-3 min each)

NAF outputs root:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/lowpt_refined_v1/`

Contents:
- `migration_inputs_lowpt_refined_v1.root` (response, 22 MB)
- `migration_inputs_lowpt_refined_v1.root.binmap_lowpt_refined_v1.json`
- `tau_scan_Area/`   40 (root + json) pairs
- `tau_scan_None/`   40 (root + json) pairs
- `tau_scan_summary.json`
- `compare_lowpt_refined_v1_tau2p07em6_Area.json`
- `compare_lowpt_refined_v1_tau2p07em6_None.json`
- `lowpt_refined_v1_diagnostics_Area_tau2p07em6.pdf`
- `lowpt_refined_v1_diagnostics_None_tau2p07em6.pdf`

Local mirrors:
- `H1/sidis_highQ2/data/lowpt_refined_v1/` (JSONs)
- `H1/sidis_highQ2/plots/lowpt_refined_v1/` (PDFs)

## Build quality / MC stats

After full 4 Rapgap + 4 Djangoh periods:
- Reco efficiency per charge: **0.60** for both h+ and h- (consistent
  with canonical NWfix_v2 ~0.60).
- MC stat count audit per gen bin: **`n_mc_count_lt_50 = 0/294`** for
  both pos and neg in the Rapgap-as-nominal sample. Every refined gen
  bin has ≥ 50 unweighted MC tracks. This is the key MC-stat sanity
  check passing — the refined design is NOT limited by MC stats.
- Data yield conservation rebinning canonical-240 → refined-840: exact
  to machine precision (`|diff| / total < 1e-7`).

## Tau scan summary

| constraint | charge | tau plateau | n_neg (best) | chi2A/dof | med rel err |
|---|---|---|---|---|---|
| Area | pos | 1e-7…2e-6 | 34-35 / 294 | ~310 | 0.050 |
| Area | neg | 1e-7…2e-6 | 45 / 294    | ~309 | 0.046 |
| None | pos | 1e-7…2e-6 | 35-36 / 294 | ~280 | 0.060 |
| None | neg | 1e-7…2e-6 | 45 / 294    | ~279 | 0.057 |

A representative tau for this exploratory study is **tau = 2.07e-6**
(within the stable plateau, ~10× above floor, before the rapid n_neg
ramp at tau > 1e-5). The L-curve corner / IBU-consistency study that
the canonical 210-bin design used to pick its tau was NOT redone here —
this is exploratory.

`chi2A/dof ≈ 300` is much larger than 1 and reflects that the
(294-bin gen) × (840-bin reco) problem is highly under-constrained by
the data Poisson statistics in many fine-pT bins. The Cholesky
decomposition fails inside TUnfold and the eigenvalue fallback is
invoked, with a **rank deficit** (rank ≈ 259 vs 294 unknowns), meaning
~12% of gen directions are degenerate with our data + custom L.

## Pathology: large overflow magnitude

A critical finding: under both Area and None constraints, the
inner-294 unfolded vector sums to **3-4× the data total**, with the
single-bin reco-overflow compensating with a value of roughly the
negative of that excess:

| constraint | charge | inner_sum (294) | overflow bin | tot_unf reported |
|---|---|---|---|---|
| Area | pos | 5.73e6 | -4.25e6 | 1.476e6 (= data) |
| Area | neg | 6.41e6 | -4.98e6 | 1.426e6 (= data) |
| None | pos | 4.62e6 | -3.44e6 | 1.179e6 |
| None | neg | 5.17e6 | -4.03e6 | 1.139e6 |

This is gen-axis "internal oscillation": adjacent gen bins flip sign
to push amplitude into the overflow channel. The custom intra-cell L
matrix smooths within each cell but does NOT couple across cells, so
TUnfold can flip whole cells without paying a regularization penalty
proportional to cell-to-cell transitions. Combined with our modest
reco/gen ratio (2.857), this is the source of the rank deficit.

## Per-bin diagnostics at tau = 2.07e-6, Area

Trusted region (280 bins, excludes 14 S3merged z≥0.5):

| metric | h+ | h- |
|---|---|---|
| n_neg / 280 | 29 | 36 |
| median σ/N | 0.049 | 0.044 |
| median σ/N in low-pT (160 bins, pT_h < 1 GeV) | **0.083** | **0.066** |
| median σ/N in high-pT (120 bins, pT_h >= 1 GeV) | 0.037 | 0.033 |
| max σ/N in low-pT trusted | **24.5×** | 7.1× |
| n_neg in low-pT trusted (160 bins) | 21 (13%) | 31 (19%) |
| n_neg in high-pT trusted (120 bins) | 8 (7%) | 5 (4%) |

**Per-bin pathology is concentrated in low-pT**: ~13-19% of low-pT bins
are negative or have rel err > 100%, vs 4-7% in high-pT. This is the
EXACT opposite of the desired outcome — the refined design was intended
to gain TMD impact in low-pT, but the unfolding itself produces the
worst per-bin uncertainties there.

## Reaggregation to canonical 5-pT (210-bin layout)

Yield conservation refined→canonical-210 is exact by construction.
Median rel err after reaggregation:
- h+: 0.041 (vs 0.049 raw 294) — modest improvement from bin merging
- canon-pt vs refined-pt rel err inflation factor at low-pT: ~1.7-2.0×

So aggregating the refined low-pT pair back into one canonical pT bin
recovers a similar per-bin uncertainty to the canonical design, BUT
**at the cost of losing the low-pT resolution that was the whole point
of the refinement.**

## Verdict

**The proposed refinement (7 pT_h gen bins, low-pT refined) is NOT
useful as a TMD-impact upgrade in its current form.** Specifically:

1. MC statistics are NOT the limiting factor — every refined gen bin
   has ≥50 unweighted MC tracks.
2. The limiting factor is **TUnfold conditioning of the response
   matrix under the intra-cell-only L**: rank deficit ~12% and inner-
   vector oscillation absorbed by an unphysical overflow bin.
3. Per-bin uncertainties and negative-bin density are worse in the
   low-pT refined region (target region) than in the unchanged high-pT
   region.
4. Reaggregating the refined result back to the canonical 5-pT layout
   gives a result similar to (but slightly noisier than) the direct
   canonical unfold — confirming the refinement is not adding usable
   information.

## Recommendations

Further iterations that may make the refinement viable:

1. **Stronger L coupling across cells in the same (z, pT) gen bin**: add
   curvature triplets across adjacent (cell, z, pT) bins so the L
   matrix penalizes cell-to-cell jumps. This would address the inner-
   oscillation pathology.
2. **Inject MC-predicted miss yield into the data-overflow row** so
   that kEConstraintArea is physically meaningful (per the canonical
   210-bin design). Currently both Area and None constraints give
   pathological overflow values.
3. **Re-extract data at refined-840 reco binning** from the per-track
   CSVs rather than uniform-spreading the canonical 240-bin
   bg-subtracted JSON. The uniform-spread rebin is a strong assumption
   that distorts the low-pT reco spectrum precisely where we want to
   gain resolution.
4. **Less aggressive refinement** — for instance keep gen pT
   `[0, 0.5, 1, 2, 4, 8]` (5 bins, canonical) but go from 6 to 8 z
   bins, focusing the additional binning where TMD impact is highest.
5. **Run the full L-curve / closure / rho-correlation diagnostic suite**
   (as the canonical 210-bin design does) before drawing final
   conclusions. The chi2A/dof ~ 300 and Cholesky failure already
   suggest the matrix is ill-conditioned, but the full diagnostic
   would quantify whether a different tau or L weighting could rescue
   it.

For the current data release the **canonical 210-bin TUnfold AREA
release remains the recommended TMD-input product**. This exploratory
v1 is RETAINED on disk under `output/tunfold_tree_v3_percharge/lowpt_refined_v1/`
for audit history and as a starting point for the next refinement
iteration.

## Validation log

- Official ROOT TUnfold V17.9 confirmed in macro startup (every per-tau
  job logs `TUnfold version: V17.9`). No matrix-inversion proxy used.
- 294-bin output vectors non-empty (every tau, every charge).
- All 8 build jobs SUCCESS; all 80 tau-scan jobs SUCCESS.
- Smoke test on 1 rapgap file + 1 djangoh file passed before submitting
  full Condor.
- Reco efficiency 0.60 matches canonical NWfix_v2 (so the F3-F7 chain-
  audit fixes are preserved by the patched v2 builder).
- Data yield refined-840 == canonical-240 to machine precision.

## File path inventory

NAF:
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/lowpt_refined_v1/migration_inputs_lowpt_refined_v1.root`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/lowpt_refined_v1/tau_scan_{Area,None}/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/lowpt_refined_v1/tau_scan_summary.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/lowpt_refined_v1/compare_lowpt_refined_v1_tau2p07em6_{Area,None}.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/lowpt_refined_v1/lowpt_refined_v1_diagnostics_{Area,None}_tau2p07em6.pdf`
- per-file migration: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/per_file_migration_lowpt_refined_v1/{rapgap31,djangoh14}/mig_lowpt_refined_v1_*.root`

Local:
- `H1/sidis_highQ2/data/lowpt_refined_v1/tau_scan_summary.json`
- `H1/sidis_highQ2/data/lowpt_refined_v1/compare_lowpt_refined_v1_tau2p07em6_{Area,None}.json`
- `H1/sidis_highQ2/data/lowpt_refined_v1/unfolded_{pos,neg}_tau2_069e-06.json` (Area)
- `H1/sidis_highQ2/data/lowpt_refined_v1/unfolded_{pos,neg}_tau2_069e-06_None.json` (None)
- `H1/sidis_highQ2/data/lowpt_refined_v1/migration_inputs_lowpt_refined_v1.root.binmap_lowpt_refined_v1.json`
- `H1/sidis_highQ2/plots/lowpt_refined_v1/lowpt_refined_v1_diagnostics_{Area,None}_tau2p07em6.pdf`

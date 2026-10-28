# Research: Reco-Level Binning Scan Design for High-Q² SIDIS TUnfold

**Date:** 2026-06-01
**Author:** h1-physics-analyst
**Scope:** Design (no code, no runs) for a TUnfold rev5 reco-axis binning
scan on the per-charge high-Q² SIDIS pipeline. The goal is to test whether
the canonical 960-bin fine reco binning is driving the unfolded result, by
varying reco granularity while holding gen FIXED at the 240 published bins.
**Out of scope:** changes to gen binning, changes to the F1 / overflow
convention (rev5 `kEConstraintNone`, `data_overflow_bin = 0` stays canonical
for every scan point), changes to fiducial cuts, changes to MC matching.

---

## 1. Anchored Baseline (rev5 canonical)

| Quantity | Value |
|---|---|
| Gen bins per charge (PUBLISHED, FROZEN) | NB_G = 240 (8 cells × 6 z × 5 pT) |
| Reco bins per charge (CURRENT) | NB_R = 960 (8 cells × 12 z × 10 pT) |
| Reco→Gen aggregation factor | 4 (2 in z × 2 in pT_h) |
| Reco indexing | `i_reco = cell × 120 + iz_reco × 10 + ipt_reco` |
| Gen indexing | `i_gen = cell × 30 + iz_gen × 5 + ipt_gen` |
| Reco→Gen rule | `cell_gen = cell_reco`, `iz_gen = iz_reco // 2`, `ipt_gen = ipt_reco // 2` |
| TUnfold mode | `kEConstraintNone`, custom L (304 triplets) |
| Data overflow bin | 0 (no F1 hack — rev5 convention) |
| Canonical τ | 1.44 × 10⁻⁶ (rev5, h+; needs re-quoted per scheme) |
| Source of truth | `scripts/unfolding/tunfold_official_v3_percharge/binning_v3.py` |
| Merged response ROOT | `output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge.root` |

Per-charge totals at the rev5 anchor: `data_phys ≈ 1.476 × 10⁶` (h+),
`reco_eff_rapgap ≈ 0.546`, expected acceptance-corrected scale
`data/reco_eff ≈ 2.71 × 10⁶`. TUnfold gives 3.19 × 10⁶, BBB 2.77 × 10⁶,
IBU 1.47 × 10⁶ (IBU acceptance broken — orthogonal issue, do not block on it).

## 2. Why a reco-axis-only scan

- Gen is the published differential observable. Changing it changes the
  measurement definition; that is a different study.
- TUnfold solves an over-determined system when NB_R > NB_G. The current
  ratio is 4:1. The scan tests whether that 4:1 is "enough", "too much"
  (degenerate / variance-dominated rows), or "barely enough" (so the answer
  was being driven by regularization rather than data).
- If results are stable across reco binnings ⇒ 960 is a defensible choice
  and any future paper can quote it as a non-sensitivity. If results swing
  ⇒ we have evidence that reco granularity matters and we must justify the
  pick from first principles (refold χ², condition number, MC-stat per row).

## 3. Proposed Scan Schemes

All schemes hold **gen = 240 bins, fixed**. They differ only in how the
existing 960 fine reco bins are aggregated to a coarser reco axis before
TUnfold sees the response, the data, and the L matrix.

Naming: `RX-<NB_R>-<tag>`.

| Tag | NB_R | Per-cell reco grid (z × pT_h) | Reco↔Gen ratio | Aggregation rule on the 960 fine reco bins |
|---|---:|---|---:|---|
| `RX-240-1x` | 240 | 6 × 5 | 1.0 | Sum each group of 4 fine reco bins that share `(cell, iz_reco//2, ipt_reco//2)`. Reco axis becomes identical to gen axis (square response, square L). |
| `RX-480-z2` | 480 | 12 × 5 | 2.0 | Sum each pair of fine reco bins that share `(cell, iz_reco, ipt_reco//2)`. Splits z only, keeps pT_h coarse. |
| `RX-480-p2` | 480 | 6 × 10 | 2.0 | Sum each pair of fine reco bins that share `(cell, iz_reco//2, ipt_reco)`. Splits pT_h only, keeps z coarse. |
| `RX-960-4x` | 960 | 12 × 10 | 4.0 | **BASELINE (rev5 canonical).** No aggregation. |
| `RX-1920-8x` (optional / stretch) | 1920 | 24 × 10 or 12 × 20 | 8.0 | Would require a NEW finer build pass on the per-event MC; cannot be produced by aggregation. Defer unless 4× shows residual sensitivity. |

The four schemes `RX-240-1x`, `RX-480-z2`, `RX-480-p2`, `RX-960-4x` are
fully derivable from the existing rev5 merged ROOT without re-running the
Stage-1/Stage-2 builder. That is the design discipline.

`RX-1920-8x` is NOT in the first pass. It would need
`binning_v3.py::_split_linear_each(..., k=4)` and a fresh
`build_migration_th2_per_charge.py` pass. List it only as a follow-up if the
1×/2×/4× spread is non-trivial.

### 3.1 Why exactly these four

- **1×** is the floor: response is square, system is exactly determined,
  L matrix degenerates to a coarse one. This is the worst-case for TUnfold
  but the best-case for BBB. If 1× and 4× agree at the per-bin level we
  have very strong evidence that the binning is not driving the answer.
- **2× split z only** isolates whether resolving z-migration finer matters
  (z is steep and is the variable BBB is most differential-blind in).
- **2× split pT only** isolates pT_h-migration sensitivity (especially
  relevant given the 2026-05-24 frame fix; pT_h structure is sharpest in
  the low-pT bins where bin 1 [0.5, 1.0] crosses the Sivers / Cahn peak).
- **4×** is the current canonical.

The two 2× schemes carry the same NB_R but probe different physics axes;
disagreement between them is itself a diagnostic.

## 4. How each transform acts on the existing rev5 ROOT objects

All transforms below are pure linear operators on the rev5 histograms in
`migration_inputs_v3_percharge.root`. Per charge `<chg> ∈ {pos, neg}` and
per MC `<mc> ∈ {rapgap, djangoh, nominal}`. We define a single aggregation
map per scheme:

```
A[i_reco_fine] = i_reco_coarse        (integer in [0, NB_R_coarse))
```

For `RX-240-1x`: `A = build_binmap_reco_to_gen()` (already exists; length
960, image in [0, 240)).

For `RX-480-z2`: `A[i_reco_fine] = cell*60 + iz_reco*5 + (ipt_reco//2)`
where `(cell, iz_reco, ipt_reco) = _decompose_reco(i_reco_fine)`.

For `RX-480-p2`: `A[i_reco_fine] = cell*60 + (iz_reco//2)*10 + ipt_reco`.

For `RX-960-4x`: `A` is the identity.

Every scheme satisfies the invariant: each coarse reco bin receives exactly
`960 / NB_R_coarse` fine reco bins, and the aggregation is consistent with
the gen partition (no coarse reco bin spans more than one gen parent in
either z or pT_h). This is essential: it preserves the property that the
response matrix's non-zero structure stays block-diagonal across cells.

### 4.1 Response matrix `R` (inner 960 × 240 part)

For each scheme:

```
R_coarse[i_R_c, j_gen] = Σ_{i_R_f : A[i_R_f] = i_R_c} R_fine[i_R_f, j_gen]
R_coarse_w2[i_R_c, j_gen] = Σ R_fine_w2[i_R_f, j_gen]   (variance adds)
```

This is a row-aggregation: project the fine reco axis onto the coarse reco
axis by summation. Column structure (gen axis) is untouched. Absolute MC
weight semantics (`weight1 × weight2 × NucliaWeight` for matched entries)
are preserved.

### 4.2 Miss row (reco-overflow row, index NB_R in the rev5 layout)

The miss row encodes gen events with no reco match (acceptance loss). It
lives at the OVERFLOW reco index, NOT inside the NB_R × NB_G block. Under
reco aggregation it is invariant:

```
R_coarse[NB_R_coarse, j_gen] = R_fine[960, j_gen]
R_coarse_w2[NB_R_coarse, j_gen] = R_fine_w2[960, j_gen]
```

This is correct because the miss row is indexed by gen, not by reco; the
reco-axis aggregation has nothing to project. The total acceptance per gen
bin (column sum of the full 961 × 240 response) is preserved exactly under
any reco aggregation.

### 4.3 Fake column (gen-overflow column, index NB_G in rev5)

The fake column encodes reco events without a valid gen partner (within
fiducial). Under reco aggregation:

```
R_coarse[i_R_c, NB_G] = Σ_{i_R_f : A[i_R_f] = i_R_c} R_fine[i_R_f, NB_G]
R_coarse_w2[i_R_c, NB_G] = Σ R_fine_w2[i_R_f, NB_G]
R_coarse[NB_R_coarse, NB_G] = R_fine[960, NB_G]     (corner cell, usually 0)
```

This is the same row-aggregation as the inner block, applied to the
overflow gen column.

### 4.4 Data histogram `h_data_bgsub_<chg>` (length 961 in rev5)

```
data_coarse[i_R_c] = Σ_{i_R_f : A[i_R_f] = i_R_c} data_fine[i_R_f]
data_coarse_w2[i_R_c] = Σ data_fine_w2[i_R_f]
data_coarse[NB_R_coarse] = 0                       (rev5 convention preserved)
data_coarse_w2[NB_R_coarse] = 0
```

The rev5 convention `data_overflow_bin = 0` is preserved for every scheme.
No F1 hack.

### 4.5 Auxiliary per-MC histograms

- `h_prior_gen_<chg>_<mc>` (length 240, gen): **unchanged across all schemes.**
  These are column sums on the gen axis only.
- `h_matched_reco_marginal_<chg>_<mc>` (length 960): row-aggregate the same
  way as the response, optional and only needed if the post-processor wants
  per-scheme matched marginals for plotting.
- `h_mc_count_<chg>{,_djangoh}` (length 240, gen): **unchanged.**
- `h_binmap_gen_to_reco` (the legacy mis-named map): regenerate per scheme
  as `binmap_reco_coarse_to_gen[i_R_c] = i_gen` where every fine reco bin
  in the preimage maps to the same gen bin (true by construction since A
  respects the gen partition). Use for BBB.

### 4.6 L (regularization) matrix

The custom L matrix in `unfold_tunfold_single_tau.C` is built on the GEN
axis (240). It is UNCHANGED across schemes. This is correct: regularization
is a property of the published differential observable, not of the reco
projection.

### 4.7 τ rescaling

Our τ uses L-operator pre-rescaling by `1/√N_eff` with `N_eff = Σ data`.
Since data is preserved under reco aggregation (row-sums commute with the
total), `N_eff` is INVARIANT across reco schemes. Therefore the same numeric
τ value has the same physical meaning across all four schemes and direct
τ-equality comparisons (e.g. all four schemes at τ = 1.44 × 10⁻⁶) are
defensible. A per-scheme τ-scan is still required for full diagnostics —
the implicit-regularization effect of finer/coarser reco binning is exactly
what we are trying to measure.

## 5. Comparison Plan

Anchor: `RX-960-4x` at `τ = τ_canonical = 1.4384498883 × 10⁻⁶`.

### 5.1 Per-bin diagnostics, per scheme, per charge

For each `(scheme, charge)`:

1. Unfolded gen vector `n_unf[j]` (length 240).
2. Per-bin σ_stat[j] from TUnfold's analytic covariance diagonal.
3. Per-bin σ_reg[j] from `max(|n(τ × 2) − n(τ)|, |n(τ / 2) − n(τ)|)`.
4. Per-bin σ_model[j] from `|n_unf(rapgap response) − n_unf(djangoh response)|`.
5. σ_total[j] = √(σ_stat² + σ_reg² + σ_model²).

### 5.2 Pair-wise per-bin ratio vs canonical

The headline diagnostic:

```
r_scheme[j] = n_unf_scheme[j] / n_unf_960-4x[j]
δ_scheme[j] = (n_unf_scheme[j] − n_unf_960-4x[j]) / σ_total_960-4x[j]
```

Report per scheme: median |r − 1|, p90 |r − 1|, count of bins with
|δ| > 1 (1σ-shifted bins), count with |δ| > 3, separated for trusted and
untrusted bins.

**Acceptance criterion (binning-non-sensitivity claim, primary):**
median |r − 1| ≤ 5 % on trusted bins, p90 ≤ 15 %, bins with |δ| > 1
≤ 5 % of trusted bins. If 1× passes this, the canonical 4× is empirically
unforced. If 1× fails, the next test is whether 2× partners (z2, p2) bracket
4×; if so, 4× is defensible.

### 5.3 pT_h distributions per (cell, z) page

Produce `plots/tunfold_tree_v3_percharge/binning_scan/pT_h_per_cell_<charge>.pdf`
overlays of the four schemes on the same 4-page layout already used by
`scripts/analysis/plot_pT_h_per_cell.py`. Use trust mask. Use Okabe-Ito
colors with `RX-960-4x` as the gray reference band and the three others as
distinct markers.

Visual acceptance criterion: no scheme should produce a pT_h shape that
differs from `RX-960-4x` by more than its own σ_total band in any trusted
bin. If `RX-240-1x` smooths a peak that 4× preserves, that is direct
evidence that 4× resolves real migration structure.

### 5.4 Closure tests

For each scheme, run all four closure modes that already exist:
`selfRG`, `selfDJ`, `crossRG`, `crossDJ`. Acceptance criterion: per-bin
self-closure deviation (n_unf − truth) / σ_stat distributed as standard
normal on trusted bins; cross-closure deviation flat in τ. If finer reco
schemes show better cross-closure (smaller residuals at the same τ), the
4× scheme is justified by closure performance.

### 5.5 ρ matrix and effective rank

For each scheme:

- Per-bin global correlation ρ[j] = √(1 − 1 / (cov · cov⁻¹)_jj).
  Report median ρ and p90 ρ.
- Effective rank of `Rᵀ V⁻¹ R` (using `numpy.linalg.matrix_rank` with
  default tolerance and an explicit `1e-10 × σ_max` tolerance).
- Condition number κ(R) (singular-value max / min on the inner block).

Expectation: κ and median ρ increase monotonically as NB_R decreases.
The point at which ρ_median exceeds ~0.95 is where TUnfold has effectively
no information beyond regularization and the scheme should be rejected.

### 5.6 Refold residuals

For each scheme, compute `R · n_unf` and compare to the input data
histogram. Per-bin pull:

```
pull_refold[i_R] = (data[i_R] − (R · n_unf)[i_R]) / √V_data[i_R]
```

Report mean and std of pulls per scheme. A well-fitted unfold gives
`mean ≈ 0`, `std ≈ 1`. Significant deviations (std > 1.5 or |mean| > 0.3)
indicate either misregularization (try τ-down) or an inconsistency between
data and the migration model that the scheme can no longer absorb.

### 5.7 τ-scans, per scheme

Run the standard 20-point τ grid {10⁻⁹ … 10} per scheme per MC for h+
and h-. Produce summary plots:

- L-curve overlay (one panel per scheme on the same axes).
- χ²/ndf vs τ per scheme.
- n_neg vs τ per scheme.
- IBU-consistency vs τ per scheme (note: requires the IBU acceptance fix
  before being meaningful; can be deferred).

Cross-reference each scheme's τ-optimum against the canonical
τ_canonical = 1.44 × 10⁻⁶ and report whether the optimum drifts.

## 6. Expected Outcome and Predicted Best Scheme

**Prior expectation (to be falsified by the scan):**

`RX-960-4x` is the right choice for the current sample and stays canonical.
The reasoning:

1. The 4:1 over-determination is conventionally recommended for TUnfold
   when migration is non-trivial and MC statistics support it. The rev5
   inventory shows MC priors of ~6.7 × 10⁷ per charge per generator, so
   each fine reco row carries ~7 × 10⁴ MC events on average — well above
   the threshold where MC-stat-on-the-row dominates the variance.
2. The pT_h frame fix (2026-05-24) sharpened the low-pT structure; the 2×
   pT-only scheme is most likely to resolve this. We expect `RX-480-p2`
   to track 4× more tightly than `RX-480-z2`.
3. `RX-240-1x` is expected to be most different. The exactly-determined
   system has no protection against detector resolution leaking real
   physics into the wrong gen bin; the regularization will be the only
   thing absorbing finite migration. This is the textbook "BBB collapse"
   limit for TUnfold.
4. We do NOT expect `RX-1920-8x` to be necessary; if 4× already shows the
   marginal-return knee versus 2×, going further is variance-without-bias.

**Pre-registered falsifiers:**

- If `RX-240-1x` agrees with `RX-960-4x` within σ_total on > 95 % of
  trusted bins: the scan PROVES 4× is empirically unforced and we should
  consider whether the simpler 1× scheme is preferable for downstream
  consumers.
- If `RX-960-4x` shows worse closure or worse refold than `RX-480-p2`:
  4× is over-fine; revert to the 2× scheme that performs best.
- If `RX-480-z2` and `RX-480-p2` straddle 4× with the same NB_R but
  noticeably different per-bin answers: the splitting axis matters and
  the scan reveals a directional sensitivity (z vs pT_h) that must be
  studied per cell.

## 7. Implementation-Ready Run Plan

### 7.1 Inputs (no rebuild needed)

The merged rev5 ROOT and all per-MC structures already on NAF:

```
/data/dust/.../output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge.root
```

### 7.2 New code to add (file-level, no logic written here)

1. `scripts/unfolding/tunfold_official_v3_percharge/binning_v3.py`
   - Add `RECO_SCHEMES = {'1x', 'z2', 'p2', '4x'}` registry plus per-scheme
     `NB_R_coarse`, `_split_z`, `_split_pt` accessors. Reuse the existing
     `_decompose_reco`, `_split_linear_each` machinery.
   - Add `build_aggregator(scheme) -> np.int32[960]` returning A.
2. `scripts/unfolding/tunfold_official_v3_percharge/aggregate_response_per_scheme.py`
   - Read the rev5 ROOT once. For each scheme in
     `['1x', 'z2', 'p2', '4x']`, project R, R_w2, the inner block, the
     miss row, the fake column, `h_matched_reco_marginal_*`, and
     `h_data_bgsub_*` onto the coarse reco axis using A. Write
     `migration_inputs_v3_percharge_RX-<tag>.root` next to the rev5 file.
     Keep all gen-axis histograms byte-identical to the source.
3. `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C`
   - Add a `--scheme` argument that picks the input ROOT. The L matrix
     construction is unchanged (gen-axis only). Per-scheme NB_R is read
     from the input ROOT.
4. `scripts/analysis/binning_scan_compare.py`
   - Reads per-scheme per-charge per-MC unfolded JSONs, produces:
     - per-bin ratio + δ diagnostics CSV
     - per-cell pT_h overlay PDF
     - L-curve overlay + χ²/ndf overlay PDF
     - ρ-histogram + κ-vs-scheme table
     - refold-pull histogram per scheme

### 7.3 Submission shape on NAF

Per (`scheme ∈ {1x, z2, p2, 4x}`, `mc ∈ {rapgap, djangoh}`, `charge ∈ {pos, neg}`):

- 1 aggregation job (local; sub-minute).
- 1 single-τ job at τ_canonical (Condor; existing
  `submit_single_tau.condor` pattern; 4 × 2 × 2 = 16 jobs).
- 1 τ-scan job × 20 τ points (existing `submit_tau_scan.condor` pattern;
  4 × 2 × 2 × 20 = 320 jobs). Split into one cluster per scheme.
- 1 closure job set (4 modes × 4 schemes × 2 charges = 32 jobs).

Total: ~370 jobs. Same order of magnitude as the existing rev5 τ-scan.

### 7.4 Output naming convention

Mirror the existing per-rev convention:

```
data/tunfold_tree_v3_percharge/binning_scan/
  inputs/migration_inputs_v3_percharge_RX-<tag>.root
  single_tau/RX-<tag>/<mc>/<charge>/unfold_tau1p44em6.json
  tau_scan/RX-<tag>/<mc>/<charge>/unfold_tau<idx>.json
  closure/RX-<tag>/<mode>/<charge>/closure_tau<idx>.json
  compare/per_bin_ratio_<charge>.csv
  compare/refold_pulls_<charge>.csv
  compare/rho_kappa_summary.json
plots/tunfold_tree_v3_percharge/binning_scan/
  pT_h_per_cell_<charge>.pdf
  per_bin_ratio_overlay_<charge>.pdf
  Lcurve_overlay_<charge>.pdf
  chi2_vs_tau_overlay_<charge>.pdf
  closure_residual_overlay_<charge>.pdf
  refold_pulls_<charge>.pdf
  rho_distribution_<charge>.pdf
```

Final summary doc: `docs/results/binning_scan_results_2026-06-<DD>.md`
that pins the per-scheme summary table, the verdict (canonical scheme),
and the per-scheme σ-budget delta vs `RX-960-4x`.

### 7.5 Execution order

1. Land `binning_v3.py` registry + `build_aggregator`. Unit-test.
2. Land `aggregate_response_per_scheme.py`. Validate invariants:
   - row-sum preservation: per-charge per-MC, `Σ R_coarse[inner] + Σ miss + Σ fake = Σ R_fine[inner] + Σ miss + Σ fake` exactly.
   - column-sum preservation: per gen bin, column sums equal across schemes (gen axis untouched).
   - data total preservation: `Σ data_coarse = Σ data_fine = data_phys`.
3. Submit single-τ × 16. Inspect per-bin ratios vs `RX-960-4x` for sanity.
4. Submit τ-scan × 320. Build L-curve / χ²/ndf overlays.
5. Submit closure × 32. Build closure overlays.
6. Run `binning_scan_compare.py`. Land the results memo.
7. If `RX-960-4x` is confirmed: state explicitly in the canonical handoff
   that the 4× reco binning passed a 4-scheme stability scan and is
   adopted on evidence, not just convention.
8. If `RX-960-4x` is NOT confirmed: open a focused decision memo and
   propose either the new winning scheme or `RX-1920-8x` as a stretch
   target with a new build pass.

## 8. Risks, Open Questions, Gaps

- **IBU acceptance bug** (handoff Issue A) is orthogonal but means the
  IBU-consistency leg of the τ-selection plot is not informative until
  fixed. Document this caveat on every binning-scan plot that overlays IBU.
- **Charge-symmetry** is excellent at rev5 (h+/h- differ by ≤ 1 % on
  medians). The scan should preserve this. If any scheme breaks charge
  symmetry by > 2 %, that is a red flag for an aggregation-map bug.
- **MC statistics per row** at finer reco binning: for `RX-960-4x` the
  median row carries ~7 × 10⁴ events but the per-row spread is wide; some
  S3b high-z high-pT rows have < 100 MC events. The miss row is unchanged,
  but row variance V_R[i, j] (`R_w2` diagonal) grows on the coarse axes
  for those rows. The scan will measure but not solve this. Going to
  `RX-1920-8x` would worsen it; this is the empirical argument against
  the 8× stretch.
- **L matrix coupling**: the existing L is constructed on the gen axis
  with 304 triplets including z-neighbor, pT-neighbor, and an x-boundary
  term. It is unchanged across schemes. We are NOT testing L-matrix
  sensitivity here; that is a separate study (`research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`).
- **Background subtraction** is already baked into `h_data_bgsub_<chg>`
  per the v3final FINAL bg-subtraction disposition (2026-05-29). The scan
  inherits that; no re-subtraction needed.
- **Per-scheme τ-optimum drift**: if τ_optimum drifts substantially
  across schemes (say more than a factor 3), the comparison at fixed
  τ_canonical may be misleading. Mitigate by also reporting per-scheme
  results at each scheme's own L-curve corner and IBU-consistency
  minimum once IBU is fixed.
- **What this scan does NOT test**: (1) sensitivity to the gen binning
  (forbidden by scope), (2) sensitivity to L matrix structure, (3)
  sensitivity to the kEConstraintNone / data_overflow=0 convention
  (forbidden by handoff §9 Forbidden actions). These belong to separate
  research artifacts.

## 9. Verdict Template (for the future results memo)

The downstream results memo should fill exactly this template:

```
Verdict: RX-<tag> is canonical, replacing RX-960-4x   |   RX-960-4x stays canonical.
Evidence:
  - median |r − 1| trusted bins:    {1x: …, z2: …, p2: …, 4x: 0.0}
  - p90    |r − 1| trusted bins:    {…}
  - bins with |δ| > 1 (count, trusted): {…}
  - cross-closure deviation std:    {…}
  - median ρ:                       {…}
  - κ(R):                           {…}
  - refold pull mean / std:         {…}
  - per-scheme τ-optimum:           {…}
σ_total budget impact at τ_canonical (median per-bin):
  - {scheme: (σ_stat, σ_reg, σ_model, σ_total)}
```

## 10. Cross-references

- Rev5 canonical handoff: `H1/sidis_highQ2/docs/handoffs/next_session_unfolding_tuning_2026-05-31.md`
- Binning module: `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/binning_v3.py`
- TUnfold macro: `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C`
- F1 / overflow research (rev4 → rev5 transition): `H1/sidis_highQ2/docs/research/research-F1-area-constraint-normalization-2026-05-31.md`
- Schmitt-paper TUnfold-internals research: `H1/sidis_highQ2/docs/research/research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`
- Frozen `(Q², x)` binning (Scheme B′) and frozen z + pT_h companion axes:
  `H1/sidis_highQ2/docs/methodology/binning_freeze_2026-05-24.md`
- Rev3 critic verdict (now resolved): `H1/sidis_highQ2/docs/critiques/critique-tunfold-v3-percharge-2026-05-31.md`
- Methodology correction on integrated-total vs visual diagnostics:
  `H1/sidis_highQ2/context-handoff.md` § "Methodology correction (2026-06-01)"

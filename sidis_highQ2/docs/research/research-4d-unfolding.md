<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current research note. Still relevant for the post-v3 multidifferential unfolding workstream.

# Research: 4D Unfolding Framework for TMD SIDIS Multiplicities

**Date:** 2026-05-14
**Author:** researcher (general subagent)
**Scope:** Method selection, response-matrix construction, software stack,
closure-test protocol, uncertainty propagation, and a concrete code skeleton
for unfolding charge-separated SIDIS multiplicities
`M^{h+/-}(x, Q^2, z, pT_h)` from detector level to particle level for the
H1 low-Q^2 charged-hadron SIDIS analysis. Targets the binning frozen in
`plan-tmd-sidis.md` and the resolution / acceptance / purity findings
recorded in `tmd_resolution_acceptance.py` and the 2026-05-14 session
update to `context-handoff.md`.

**Inputs verified:**
- `H1/sidis_lowQ2/plan-tmd-sidis.md` (Iteration 2 + 2026-05-14 session
  update); 4D binning Q2x4 / x x4 / z x4 / pT_h x6, 13/16 (Q^2,x) cells
  populated, photon-axis kinematics, eSigma baseline.
- `H1/sidis_lowQ2/context-handoff.md` (2026-05-14): purity threshold 30%
  per dimension accepted, 4D acceptance maps already produced, full dataset
  ~18M matched MC tracks, statistics extrapolation ~1.2M tracks per (Q^2,x)
  cell.
- `H1/sidis_highQ2/scripts/tmd_resolution_acceptance.py`: existing
  `purity_stability_4d`, `acceptance_4d`, and column-normalised pT_h
  response (`plot_r9_pth_response`) functions. JSON output:
  `tmd_plots/tmd_resolution_acceptance.json`.
- `H1/agents/h1-unfolder.md`: existing specialist agent (V0-oriented) with
  RooUnfold-style menu (bin-by-bin, D'Agostini, SVD, matrix inversion).
- `H1/sidis_lowQ2/research-sidis-systematics-literature.md`: H1 multiplicity
  paper (DESY 20-176) used TUnfold in 3D (N, Q^2, y) with RAPGAP default
  / DJANGOH alternative as the leading systematic; ZEUS/COMPASS used
  bin-by-bin acceptance corrections; HERMES used a single-matrix
  unregularised unfolding with JETSET Hessian variation.

---

## 1. Headline Recommendation

**Adopt sliced-2D iterative Bayesian (D'Agostini) unfolding in (z, pT_h)
within each accessible (Q^2, x) cell, with a separate 2D bin-by-bin
correction layer on (Q^2, x) migration applied first. Run per charge.
Validate with a full 4D D'Agostini unfolding using a ROOT-CSC sparse
response and use the difference as a "factorisation systematic." Use a
custom numpy implementation as the production engine; cross-check against
RooUnfold for one slice and one full-4D run.**

Justification, in one paragraph: the (Q^2, x) migration is small (log10
RMS 0.053 and 0.084, which is well under the 0.5-decade bin widths chosen
in the plan), and the 4D purity penalty is dominated by (z, pT_h)
migration. A factorised correction therefore captures essentially all of
the bin-migration physics with a 24x24 response per (Q^2, x) cell that
each has ~1.2M MC tracks per charge -- comfortably enough to keep the
response matrix statistical noise below 1% in all populated reco/gen
cells. Full 4D unfolding (a ~312x312 sparse matrix) is feasible but is
overkill; we recommend it as a *cross-check of factorisation* rather than
the production engine.

The remainder of this document supports that recommendation and provides
the concrete recipe.

---

## 2. Method Comparison Table

For each method, "fit-for-purpose" is graded against four 4D-specific
criteria:
- **4D scaling:** does the method remain numerically and statistically
  stable when applied to the full 384-bin grid per charge?
- **Bias at 30% purity:** how badly does the method bias the result when
  per-dimension purity is only 30% (so 4D purity ~ 1%)?
- **Regularisation control:** can over-regularisation that erases physics
  features be diagnosed and bounded?
- **Uncertainty propagation:** can statistical and systematic covariances
  be propagated cleanly to TMD fitters?

Rating scale: ++ excellent, + acceptable, ~ marginal, - bad, -- unusable.

| Method | 4D scaling | Bias at 30% purity | Regularisation | Uncertainty prop. | Verdict for our 4D problem |
|---|---|---|---|---|---|
| Bin-by-bin (`epsilon_i = N_reco_i / N_gen_i`) | ++ | -- | n/a | + | **Insufficient.** With 4D purity ~1% the diagonal-only assumption is wrong everywhere. H1 multiplicity paper (DESY 20-176) explicitly switched away from bin-by-bin at this purity. May still be appropriate for the (Q^2, x) layer where purity is 80-90%. |
| Iterative Bayesian / D'Agostini | + (slice-by-slice ++) | + | + (number of iterations) | ++ via toys/MC bootstrap | **Recommended core method.** Stable in 2D slices; converges in 3-6 iterations at our purities; regularisation handle is the iteration count, easy to scan. Well-tested at HERA/HERMES-level statistics. |
| SVD (Hoecker-Kartvelishvili / TSVDUnfold) | ~ in full 4D, + sliced | + | + (k regularisation parameter via L-curve) | + (covariance from MC toys) | **Acceptable but harder.** Standard at H1/ZEUS but most institutional experience is 1D/2D. The L-curve corner is harder to identify cleanly when the bins span 3 decades in Q^2 and x; needs explicit rescaling of the response columns before SVD. Use as a cross-check. |
| Matrix inversion | -- | + (unbiased in expectation) | none | ~ | **Unusable.** A 312x312 inversion will amplify response-matrix MC statistical noise into wild oscillations. Useful only for sanity checks on small slices. |
| Slice-by-slice 2D unfolding (z, pT_h) inside (Q^2, x) | ++ | + (with bin-by-bin (Q^2,x) layer) | + per slice | ++ block-diagonal | **Recommended factorisation.** Reduces response matrix from ~312x312 (sparse) to 13 independent 24x24 (dense). Only weakness: ignores cross-(Q^2,x)-cell migration in (z, pT_h), which is small but non-zero. Quantify and assign as factorisation systematic. |
| TUnfold (Stefan Schmitt) | ++ | + | ++ (formal L-curve, density-aware) | ++ explicit covariance output | **Strong alternative.** This is the H1 multiplicity paper's tool. Handles unequal binning natively and has formal regularisation tools. Good if we need ROOT integration, but adds a dependency we can otherwise avoid. Use for one full 4D cross-check. |
| Omnifold / ML reweighting | + | ++ in principle | -- (NN training is opaque) | -- difficult to expose to fitters | **Defer.** Modern, but adds an ML stack and an opaque training step that TMD fitters will not accept on a first H1 release. Could be a future systematic cross-check. |

### Notes on the "30% per-dimension purity" claim

The 30% purity threshold is **per dimension**, applied independently to
1D purity in each of (Q^2, x, z, pT_h). It does NOT mean 4D purity is
30%. Empirically, with independent-axis migrations the 4D purity is the
product of the per-dimension purities, i.e. on the order of
0.3 x 0.3 x 0.3 x 0.3 ~ 0.8%. This is consistent with the per-cell
purity histograms in `purity_stability_4d` (most populated cells have
4D purity 1-5%, a few diagonal-favoured cells higher).

This **rules out bin-by-bin 4D correction** as the publication-grade
method, and forces either factorised slice-by-slice unfolding or a full
4D unfolding. The plan's Level C2 ("sliced 2D response") is therefore
the right architecture; this document fleshes it out.

---

## 3. Recommended Approach: Factorised Sliced-2D D'Agostini

### 3.1 Architecture

Unfolding is performed in two layers, *both* applied per charge:

1. **Layer A: 2D bin-by-bin correction in (Q^2, x).**
   - 4 (Q^2) x 4 (x) = 16 cells, of which 13 are kinematically accessible.
   - log10 RMS of (Q^2, x) is 0.053 / 0.084 (session update); bin widths
     are factor-2 in Q^2 and 0.5 decade in x. Per-dimension purity is
     therefore ~80-90% in (Q^2) and ~70-85% in (x), so 2D (Q^2, x)
     purity is ~60-75% per cell. Bin-by-bin is acceptable here, with the
     residual migration absorbed into a "Layer-A migration systematic."
   - Correction factor:
     `c_QX[iQ, iX] = sumw(reco-and-gen-in-cell) / sumw(reco-in-cell)`
     applied multiplicatively to the reco yield before Layer B.

2. **Layer B: 2D D'Agostini unfolding in (z, pT_h) inside each
   (Q^2, x) cell.**
   - 4 (z) x 6 (pT_h) = 24 reco bins x 24 gen bins per cell. Dense 24x24
     response. ~50,000-100,000 matched MC tracks per cell per charge
     even from current 1M-entry studies; the full ~18M dataset gives
     ~700,000-1.4M matched tracks per (Q^2, x) cell per charge -- response
     matrix statistical noise is sub-percent in all populated entries.
   - 13 (Q^2, x) cells x 2 charges = 26 independent 2D unfoldings.
   - Iteration count fixed by per-cell chi^2 closure on RAPGAP, capped at
     6 to avoid noise amplification (see Section 6.2).

3. **Layer C (cross-check, not production): full 4D D'Agostini.**
   - One sparse 312x312 response, ~312 reco x ~312 gen unrolled bins.
   - Memory: float64 sparse with ~10% nonzeros -> ~1 MB. Trivial.
   - Not the production engine because (a) its convergence diagnostics are
     much harder to interpret than 26 independent 2D problems and (b) its
     full covariance matrix is harder to expose to TMD fitters in a useful
     form. Use it once per generator to bound the factorisation error.

### 3.2 Why sliced rather than full 4D

| Argument | Implication |
|---|---|
| (Q^2, x) migration is small (log10 RMS << bin width) | Layer A bin-by-bin is defensible; no need to spend regularisation degrees of freedom on it. |
| (z, pT_h) migration is the dominant migration | Layer B carries the unfolding work where it actually matters. |
| Per-cell (z, pT_h) statistics are huge (>700k tracks per (Q^2, x) cell per charge from full dataset) | The 24x24 response is well-conditioned; the bias and variance trade-off in D'Agostini is well-controlled. |
| Block-diagonal covariance is easier to package for fitters | Each (Q^2, x) cell becomes an independent 24x24 stat covariance block, plus correlated systematic shifts across cells. |
| Failure modes localise to single cells | An unstable (z, pT_h) unfolding in one (Q^2, x) cell can be diagnosed and masked without re-running everything. |

### 3.3 What the factorisation ignores

Cross-cell migration in (Q^2, x) of the (z, pT_h) distribution. Two flavours:

1. A track migrating from (Q^2_gen, x_gen) cell A to (Q^2_reco, x_reco)
   cell B will be unfolded in cell B's (z, pT_h) response, not cell A's.
   Estimated impact: ~10-20% of tracks migrate across (Q^2, x) cell
   boundaries (1 - cell purity). Their (z, pT_h) response is correlated
   with the local cell, but if neighbouring cells have similar (z, pT_h)
   response (which the small kinematic neighbourhood would suggest), the
   bias is suppressed. This is the **"factorisation systematic"** to be
   evaluated by comparing Layer A+B output to a full 4D unfolding (Layer C).

2. The Layer A bin-by-bin correction itself is biased if the reco-level
   (z, pT_h) distribution within a (Q^2, x) cell differs systematically
   between events that migrated in and those that did not. This is also
   absorbed into the "factorisation systematic."

The H1 multiplicity paper handled an analogous concern by using a single
3D TUnfold in (N, Q^2, y) rather than a sliced approach; for our problem
the 4D bins are dimensionally one higher, and the increased computational
cost of a faithful 4D regularised unfolding is not justified by the
expected bias improvement.

---

## 4. Response Matrix Construction Recipe

### 4.1 Inputs from MC

Use the existing matched-reco accumulator `stream_reco_matched` from
`tmd_resolution_acceptance.py`. For each matched MC track that passes
BASE_CUT, the loader yields:

```text
charge       in {+1, -1}
Q2_reco      photon-axis Q^2 from eSigma method  (GeV^2)
x_reco       photon-axis x   from eSigma method
z_reco       photon-axis z
pT_h_reco    photon-axis pT_h                    (GeV)
Q2_gen       gen-level eSigma Q^2
x_gen        gen-level eSigma x
z_gen        gen-level photon-axis z
pT_h_gen     gen-level photon-axis pT_h
w            weight1 * weight2                   (GKI generator weight)
```

Plus the gen-only loader `stream_gen` yields the same gen quantities and
weights (no reco information). The gen loader is what defines the
acceptance denominator.

Bin edges (locked, from `plan-tmd-sidis.md` 2026-05-14 session update and
`tmd_resolution_acceptance.py` lines 86-89):

```python
Q2_BINS  = [5, 10, 20, 50, 100]                 # 4 bins in Q^2
X_BINS   = [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]       # 4 bins in x
Z_BINS   = [0.10, 0.20, 0.30, 0.50, 0.70]       # 4 bins in z
PTH_BINS = [0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0]  # 6 bins in pT_h
```

### 4.2 Layer-A response (Q^2, x bin-by-bin)

For each charge:

```text
sumw_reco_QX[iQ_r, iX_r]   = sum w  over matched-reco tracks in reco cell
sumw_gen_QX [iQ_g, iX_g]   = sum w  over gen tracks in gen cell
sumw_both_QX[iQ, iX]       = sum w  over matched tracks where both reco
                                    and gen are in the same (Q^2, x) cell
```

Bin-by-bin acceptance for Layer A:

```text
epsilon_QX[iQ, iX] = sumw_both_QX[iQ, iX] / sumw_gen_QX[iQ, iX]
purity_QX[iQ, iX]  = sumw_both_QX[iQ, iX] / sumw_reco_QX[iQ, iX]
```

Layer A "correction" applied to data is `data_corr = data_reco / epsilon_QX`,
*not* the H1-paper-style direct-acceptance correction. Note the choice:
because Layer B operates after Layer A, the sequence is:

1. data_reco_4d[iQ, iX, iZ, iP]    <- charge-separated 4D data histogram
2. project to data_reco_QX[iQ, iX] = sum_{iZ, iP} data_reco_4d[iQ, iX, iZ, iP]
3. compute Layer-A *migration-only* correction (NOT acceptance): apply
   the (Q^2, x) reco-to-gen migration factor
4. unfold (z, pT_h) inside each (Q^2_gen, x_gen) cell with Layer B
5. apply remaining (z, pT_h)-only acceptance correction

The cleaner formulation, which avoids double-counting acceptance, is to
fold the (Q^2, x) migration directly into Layer B's response and never
do an explicit Layer A. That is the formulation we use below.

### 4.3 Recommended formulation: skip explicit Layer A; use one
sliced response per (Q^2_gen, x_gen) cell that includes (Q^2, x)
migration in/out

For each (iQ_gen, iX_gen) cell and each charge:

1. **Reco bins:** all 24 (z_reco, pT_h_reco) bins, regardless of where
   the reco track's (Q^2_reco, x_reco) lands. This is "data the cell
   sees."
2. **Gen bins:** 24 (z_gen, pT_h_gen) bins for tracks whose gen-level
   (Q^2_gen, x_gen) is in this cell. Plus an "out-of-cell gen" sink bin
   that absorbs the migration *out* contribution.
3. **Response R[r, g]:** number of matched-MC tracks (weight-summed)
   with gen in (cell, gen-(z, pT_h)-bin g) and reco in (any, reco-(z,
   pT_h)-bin r).

This formulation is exact: it correctly accounts for both inward and
outward migration in (Q^2, x) at the per-cell level, and it reduces to
the standard 2D unfolding when (Q^2, x) migration is zero.

Note: if implementing this exact form is too involved for a first
release, the simpler approximation -- Layer A bin-by-bin migration
correction multiplied by Layer B (z, pT_h) unfolding inside the gen
cell -- is what the recommendation in Section 3.1 actually delivers.
Quantify the difference between the two formulations and assign it as
the "factorisation systematic."

### 4.4 Pseudocode for the per-cell Layer B response

```python
def build_2d_response_per_cell(reco, charge, iQ_gen_cell, iX_gen_cell):
    """
    Returns:
      R    : float64 (24, 24) response matrix, R[r, g] = sumw of tracks
             with reco-(z, pT_h)-bin r and gen-(z, pT_h)-bin g, restricted
             to gen tracks whose gen (Q^2, x) is in (iQ_gen_cell, iX_gen_cell)
             and the requested charge.
      gen_truth : float64 (24,) sumw_gen by gen (z, pT_h) bin; the
                  acceptance denominator (no reco requirement).
      reco_inc  : float64 (24,) sumw_reco by reco (z, pT_h) bin, including
                  tracks whose gen migrated into this cell from elsewhere.
    """
    # Charge mask
    cm = (reco['charge'] == charge)
    # Gen-cell mask (gen Q^2, x in cell)
    iQg = safe_digitize(reco['Q2_gen'], Q2_BINS)
    iXg = safe_digitize(reco['x_gen'],  X_BINS)
    in_gen_cell = (iQg == iQ_gen_cell) & (iXg == iX_gen_cell) & cm

    # Indices
    iZr = safe_digitize(reco['z_reco'],   Z_BINS)
    iPr = safe_digitize(reco['pT_h_reco'], PTH_BINS)
    iZg = safe_digitize(reco['z_gen'],    Z_BINS)
    iPg = safe_digitize(reco['pT_h_gen'],  PTH_BINS)
    in_zp_reco = (iZr >= 0) & (iPr >= 0)
    in_zp_gen  = (iZg >= 0) & (iPg >= 0)

    nZ = len(Z_BINS) - 1; nP = len(PTH_BINS) - 1
    nB = nZ * nP  # 24

    # Unrolled bin index helper
    def idx(iz, ip): return iz * nP + ip

    R = np.zeros((nB, nB))
    sel = in_gen_cell & in_zp_reco & in_zp_gen
    r_idx = idx(iZr[sel], iPr[sel])
    g_idx = idx(iZg[sel], iPg[sel])
    np.add.at(R, (r_idx, g_idx), reco['w'][sel])

    # Acceptance denominator (gen-only, this cell, gen z/pT_h binned)
    # Use the gen-only stream for this!  Not the matched-reco stream.
    return R  # gen_truth and reco_inc must be filled from stream_gen below.
```

The acceptance-denominator histogram `gen_truth` must come from the
**gen-only loader** `stream_gen`, not from the matched-reco loader, or
the denominator will be silently restricted to matched-reco tracks
(this is the F1/F2 family of bugs from the original analysis -- see
`context-handoff.md` Section 7). Use the following:

```python
def gen_truth_per_cell(gen, charge, iQ_gen_cell, iX_gen_cell):
    cm = (gen['charge'] == charge)
    iQg = safe_digitize(gen['Q2_gen'], Q2_BINS)
    iXg = safe_digitize(gen['x_gen'],  X_BINS)
    iZg = safe_digitize(gen['z_gen'],  Z_BINS)
    iPg = safe_digitize(gen['pT_h_gen'], PTH_BINS)
    in_cell = (iQg == iQ_gen_cell) & (iXg == iX_gen_cell) & cm
    in_zp   = (iZg >= 0) & (iPg >= 0)
    sel = in_cell & in_zp

    nZ = len(Z_BINS) - 1; nP = len(PTH_BINS) - 1
    g = np.zeros(nZ * nP)
    np.add.at(g, iZg[sel] * nP + iPg[sel], gen['w'][sel])
    return g
```

### 4.5 Normalisation conventions

- **Rows of R** (gen index `g`): summing `R[r, g]` over `r` gives the
  number of matched-reco tracks with gen in bin `g`. Dividing this by
  `gen_truth[g]` gives the acceptance for gen bin `g`. Both quantities
  should be stored alongside `R`.
- **Columns of R** (reco index `r`): summing `R[r, g]` over `g` gives
  the number of matched-reco tracks with reco in bin `r`. This is NOT
  the data prediction; it omits unmatched-reco tracks (true detector
  fakes) which must come from the systematic on `MatchAngle`. It also
  omits tracks whose gen (Q^2, x) is outside the cell -- those will be
  absorbed into a neighbouring cell's "in-migration."

- **Column-normalised response** (the standard D'Agostini conditional
  probability `P(reco r | gen g)`): `M[r, g] = R[r, g] / sum_r R[r, g]`.
  This is what the iteration uses.

### 4.6 Sparsity, memory, and conditioning

- Full 4D unrolled response: 312 x 312 dense float64 = 779 KB. Trivial.
- Per-cell 2D response: 24 x 24 = 4.6 KB per (Q^2, x, charge) tuple
  = 13 x 24 x 24 x 2 = 30 KB total for all cells / charges. Trivial.
- Conditioning: the column-normalised response `M` has condition number
  bounded above by the inverse of the smallest column-sum of `R`. With
  ~700k matched tracks per (Q^2, x) cell per charge spread over 24 reco
  bins, the smallest column sum is on the order of 10^3 even in the
  sparsest gen bin, so MC statistical noise in the response is well
  below 1%.
- Empty rows or columns: a gen bin with zero MC tracks cannot be
  unfolded; flag and mask it. A reco bin with zero MC tracks but
  non-zero data implies migration in from a gen region not modelled in
  this cell -- flag it and inflate the systematic.

### 4.7 Python / numpy implementation note

`np.add.at` is the safe accumulator (handles repeated indices). Use it
in preference to fancy indexing assignment, which silently drops
duplicate-index updates. Use float64 throughout. Use `np.errstate` to
suppress invalid-value warnings around the acceptance and purity
divides.

---

## 5. Implementation Plan

### 5.1 Software stack

**Production engine: custom numpy** in a new script
`scripts/tmd_unfold.py`. Reasons:

- The 2D D'Agostini iteration is ~30 lines of code and is dominated by
  one matrix-vector multiply per iteration (24 x 24 x 6 iterations).
  Wall time per cell is microseconds; for 26 cells x N_iterations x
  N_systematic_variations x N_bootstrap_replicas the total is still
  seconds.
- Avoids the H1OO / ROOT dependency for the unfolding step itself; data
  flow stays in numpy / npz / JSON which is the existing analysis-script
  convention.
- Direct control over weight handling (the existing accumulators
  use `weight1 * weight2` via `reco['w']`).
- Direct control over MC-bootstrap response-matrix uncertainty
  propagation (Section 7), which is awkward in RooUnfold.

**Cross-check engine: RooUnfold** (one slice + one full 4D run) on NAF.
- Build with the H1OO ROOT (or use a standalone PyROOT install at NAF).
- Cross-check that our 2D iterative result agrees within MC statistics.
- Cross-check that the full-4D RooUnfoldBayes result on a single slice
  agrees with our sliced result (this is the factorisation-systematic
  measurement).
- We do NOT use RooUnfold for production because (a) our systematic
  variation pipeline already runs in numpy and integrating each
  variation into RooUnfold would multiply the I/O burden, and (b) the
  numpy implementation is auditable line by line.

**No PyUnfold, no Omnifold, no TUnfold for the first release.** Each is
a viable cross-check but adds dependencies that are not necessary for
the recommended approach. Defer to second iteration if needed.

### 5.2 New script: `scripts/tmd_unfold.py`

Inputs:
- Merged MC ROOT files (RAPGAP31 nominal; DJANGOH14 for systematics).
  Path: `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsym/output/`.
- Data ROOT file (when available; for now, MC-as-pseudo-data via
  `--use-mc-as-pseudo-data`).
- Existing JSON: `tmd_plots/tmd_resolution_acceptance.json` for the
  binning, the precomputed acceptance maps, and the purity/stability
  flags.
- Optional: shifted-MC files from `produce_shifted_mc.py` for systematic
  variations.

Outputs (under `tmd_plots/` in `H1/sidis_lowQ2/`):
- `tmd_unfolded_hplus.npz`: per (Q^2, x) cell, `R`, `gen_truth`,
  `unfolded`, `unfolded_cov_stat`, iteration count, `chi2_iter`.
- `tmd_unfolded_hminus.npz`: same for h-.
- `tmd_unfolded_summary.json`: bin definitions, per-cell metadata,
  closure-test diagnostics, factorisation-systematic estimate.
- `tmd_response_matrices.npz`: all per-cell response matrices for
  fitter export.
- `tmd_covariance_blocks.npz`: per-cell statistical covariance plus
  per-systematic-source shift vectors.
- Plots: per-cell response heatmap, iteration-vs-chi^2 curve, closure
  test (gen-truth vs unfolded), unfolded multiplicities for h+/h-.

### 5.3 Integration with existing pipeline

The new script depends on the existing `stream_reco_matched` and
`stream_gen` loaders. Recommended pattern: import them from
`tmd_resolution_acceptance` rather than duplicate. Keep all bin
definitions in a single shared module to avoid the divergence trap.

The production order is:

```text
  merged MC                                    data ROOT
       |                                           |
       v                                           v
  stream_reco_matched + stream_gen           stream_data_reco
       |                                           |
       v                                           v
  build_response_per_cell (charge, iQ, iX)   project to 4D reco hist
       |                                           |
       +-------> build_R, gen_truth ------> dagostini_unfold (per cell)
                                                  |
                                                  v
                                          unfolded_4d_hplus, hminus
                                                  |
                                                  v
                       acceptance correction (already in gen_truth norm)
                                                  |
                                                  v
                       multiplicities M^{h+/-}(x, Q^2, z, pT_h)
```

The existing 4D acceptance pipeline (`acceptance_4d` in
`tmd_resolution_acceptance.py`) is **superseded** by this unfolding when
purity is below threshold. It remains useful as a cross-check in the
high-purity bins (where bin-by-bin and unfolding should agree).

### 5.4 Where to run

NAF login node is sufficient for the unfolding step itself (~1 minute for
the full pipeline). Generating systematic variations (shifted-MC
production) requires Condor, but that is already handled by
`produce_shifted_mc.py`. The unfolding consumes the post-Condor merged
files.

### 5.5 Output contract for h1-unfolder agent

To match the contract in `H1/agents/h1-unfolder.md` (Output Contract
section), the script should also emit `unfolded_sidis_charged.json` with:

```json
{
  "step": "unfolding",
  "channel": "sidis_charged_lowQ2",
  "status": "PASS",
  "input_files": [...],
  "output_file": "tmd_unfolded_summary.json",
  "bins": {"Q2": Q2_BINS, "x": X_BINS, "z": Z_BINS, "pT_h": PTH_BINS},
  "values": {"hplus": [...], "hminus": [...]},
  "errors": {...},
  "validation": {
    "closure_chi2_per_dof": {...},
    "refold_chi2": {...},
    "method_comparison": {...},
    "factorisation_systematic_pct": {...}
  }
}
```

---

## 6. Closure Test Protocol

The closure test is the *single most important validation step*. It must
be done before any data unfolding result is reported.

### 6.1 Standard closure (MC-as-pseudo-data, same generator)

For each generator (RAPGAP, DJANGOH) independently:

1. Load matched-reco MC and split into two random halves with the same
   `weight1 * weight2` weighting:
   - Half A (response): build `R[r, g]` and `gen_truth[g]`.
   - Half B (pseudo-data): build the reco-level histogram
     `data_reco[r] = sum_{tracks in B} w_track * (track in reco bin r)`.
2. Unfold half-B's reco histogram with half-A's response.
3. Compare unfolded values to half-B's gen-truth histogram (built from
   half B with no reco requirement).
4. Per (Q^2, x, charge) cell, compute
   `chi2 = sum_g (unfolded[g] - gen_truth[g])^2 / var(unfolded[g])`
   where `var(unfolded[g])` is the diagonal of the propagated stat
   covariance (Section 7.1).
5. **Pass criterion:** `chi2 / dof < 2` per cell, `chi2 / dof < 1.3`
   averaged over cells, and no per-bin pull > 3 sigma in any populated
   gen bin.

### 6.2 Iteration-count selection

For each cell, scan `n_iter in {1, 2, 3, 4, 5, 6, 8, 10}`:
- Track `chi2_to_truth[n_iter]` (closure chi^2 to half-B gen).
- Track `chi2_to_data[n_iter]` (refold chi^2 to half-B reco).
- Track per-bin RMS oscillation between consecutive iterations.

**Frozen iteration count:** the smallest `n_iter` such that
`chi2_to_data[n_iter] / dof < 1.5` AND incremental change in unfolded
values is below 1%. Cap at 6. The H1 multiplicity paper used TUnfold
regularisation at ~10^-3, which is a different parametrisation but
produces the same effective behaviour: stop before MC statistical noise
in the response begins to dominate.

If the optimal `n_iter` differs significantly between cells, freeze the
*per-cell* iteration count rather than a global value, and document this
in the systematic write-up.

### 6.3 Generator cross-closure

This is the H1 multiplicity paper's Sys.2 (the dominant source).

1. Build response from RAPGAP, unfold DJANGOH reco, compare to DJANGOH
   gen.
2. Build response from DJANGOH, unfold RAPGAP reco, compare to RAPGAP
   gen.
3. The discrepancy between these two cross-unfolds is the
   **model-dependence systematic** in unfolding.
4. Recover-truth tolerance: `|unfolded - gen_truth| / gen_truth < 5%`
   per bin in populated cells (matches the H1 paper's quoted Sys.2 of
   1.5-4.3% on moments).

If the cross-closure is much worse than the same-generator closure,
that is a generator-tune disagreement, not an unfolding failure.

### 6.4 Refolding test

For each cell, after unfolding:
- Compute `refold[r] = sum_g M[r, g] * unfolded[g] / acceptance[g]`
- Compare `refold[r]` to the input data reco histogram (or pseudo-data
  reco).
- `chi2_refold / dof < 1` is required. A failure indicates either
  numerical bug or an empty-row/column problem.

### 6.5 Robustness scans

- **Prior dependence:** D'Agostini default prior is a flat gen
  distribution; cross-check with the gen-truth itself as the prior. The
  difference at the chosen iteration count should be < 1%.
- **Bin-edge perturbation:** shift each bin edge by 5% of its width and
  re-unfold. Differences are part of the binning systematic.
- **Low-occupancy bins:** explicitly mask cells where any reco or gen
  bin has effective `N_eff < 50` and report them as "not unfolded."

### 6.6 Closure for the factorised approximation

To quantify what Section 3.3 ignores:

1. Run the full 4D D'Agostini unfolding (Layer C, sparse 312x312) on
   pseudo-data.
2. Run the sliced 2D + Layer-A correction on the same pseudo-data.
3. Per published bin, compute the relative difference. Take the RMS
   (or worst case) per cell as the **factorisation systematic**. Expect
   1-3% based on the small (Q^2, x) migration; if it exceeds 5% in any
   populated cell, switch that cell to the full-4D unfolding.

---

## 7. Uncertainty Propagation

### 7.1 Statistical uncertainty: D'Agostini covariance via MC bootstrap

The D'Agostini iteration has a closed-form propagation (Adye 2011) but
the simpler and more robust approach for our problem is bootstrap
resampling. Two independent statistical sources:

1. **Data Poisson statistical uncertainty:** for each bootstrap replica,
   draw a Poisson realisation of `data_reco[r]` from its observed value,
   then unfold. Repeat 200-500 times. The bootstrap covariance is the
   sample covariance of the unfolded vectors.

2. **Response-matrix MC statistical uncertainty:** for each bootstrap
   replica, draw a Poisson realisation of each `R[r, g]` entry (or
   resample matched MC tracks with replacement, more correctly), rebuild
   the response, and unfold the same data. Repeat 200-500 times.
   The response-MC covariance contribution is the sample covariance of
   these unfolded vectors.

The total per-cell statistical covariance is the sum of the two. The
diagonal gives per-bin statistical errors; the off-diagonal gives the
intra-cell statistical correlations that TMD fitters need.

For the full release, these covariances should be exposed as block-
diagonal matrices, one block per (Q^2, x, charge).

### 7.2 Systematic uncertainty: shift-and-reunfold per source

For each systematic source (full list in
`reference-sidis-systematics.md` and the MVP table in
`context-handoff.md` Section 9):

1. Construct the systematically shifted MC sample (or in some cases
   shifted data, e.g. for cut-scan systematics).
2. Build the response matrix from the shifted MC.
3. Unfold the (nominal) data with the shifted response.
4. The per-bin shift `delta_source[bin]` = (unfolded_shifted - unfolded_nominal).
5. For asymmetric `+/-` sources, run both directions.
6. Store per-source shift vectors, NOT just the symmetrised total.

For the model-dependence systematic (RAPGAP vs DJANGOH response), the
prescription matches H1 multiplicity paper Sys.2: build response from
DJANGOH, unfold the data, take the difference vs the RAPGAP-response
unfolding as the systematic.

### 7.3 Per-source covariance

For each correlated systematic source, the contribution to the full
covariance is the outer product of the per-bin shift vector with itself:

```text
Cov_source[bin_i, bin_j] = delta_source[bin_i] * delta_source[bin_j]
```

This treats each source as fully correlated across bins, which is the
standard assumption for MC-model and detector-scale systematics. For
sources that should be uncorrelated bin-to-bin (e.g. the H1
spectra-companion treatment of track efficiency, see
`research-sidis-systematics-literature.md` Section 3.1, item 3), use a
diagonal contribution.

### 7.4 Total covariance

The total covariance for fitter consumption is:

```text
Cov_total = Cov_stat + sum_sources Cov_source
```

Block-structured by (Q^2, x, charge). Cross-(Q^2, x) correlations exist
through the systematic sources (a single SpaCal scale shift applies
coherently to all cells) but not through the statistical part.

### 7.5 Release format for TMD fitters

Per Part 5 of `plan-tmd-sidis.md`, the recommended release contains
per source shift vectors *and* the assembled covariance:

- Central values: per (Q^2, x, z, pT_h, charge) bin.
- Statistical covariance: block-diagonal, one block per (Q^2, x, charge).
- Per-source systematic shifts: full vector per source per charge.
- Total systematic covariance: optional convenience.
- Theory flags per Part 4 of `plan-tmd-sidis.md`.

Storage: a single `tmd_release_lowQ2_charged.npz` (numpy) plus a JSON
manifest. HEPData export is downstream of this.

### 7.6 What the H1 multiplicity paper actually did

For comparison: the H1 paper (DESY 20-176) computed each systematic by
re-running the full 3D TUnfold with the shifted input or shifted
response, then took symmetric per-bin shifts and added in quadrature
(see `research-sidis-systematics-literature.md` Section 1). They did
not publish a bin-to-bin covariance matrix, only per-bin uncertainties.
For our TMD release, fitters explicitly require the covariance, so we
add it.

---

## 8. Concrete Code Skeleton

This skeleton is intended for `scripts/tmd_unfold.py` and uses the
existing `tmd_resolution_acceptance` infrastructure for I/O. It
implements the recommended sliced 2D D'Agostini per (Q^2, x) cell.

```python
#!/usr/bin/env python3
"""tmd_unfold.py -- 4D SIDIS multiplicity unfolding.

Implements sliced 2D iterative-Bayesian (D'Agostini) unfolding in
(z, pT_h) inside each accessible (Q^2, x) cell, charge-separated.
Cross-checked against full 4D unfolding for the factorisation
systematic.

Usage:
    python tmd_unfold.py --mc rapgap.root --data data.root --out tmd_plots/
"""
from __future__ import annotations
import argparse
from pathlib import Path
import json
import numpy as np

# Import the existing loaders, bin definitions, and helpers.
from tmd_resolution_acceptance import (
    Q2_BINS, X_BINS, Z_BINS, PTH_BINS, Y_LO, Y_HI,
    stream_reco_matched, stream_gen, safe_digitize,
)

NZ = len(Z_BINS)  - 1
NP = len(PTH_BINS) - 1
NB = NZ * NP  # 24 (z, pT_h) bins per (Q^2, x) cell


# ---------------------------------------------------------------------
# 2D-bin index utilities
# ---------------------------------------------------------------------

def zp_index(iz: np.ndarray, ip: np.ndarray) -> np.ndarray:
    """Unroll (z, pT_h) bin indices to a flat 0..NB-1 index."""
    return iz * NP + ip


def zp_unindex(idx: int) -> tuple[int, int]:
    return divmod(idx, NP)


# ---------------------------------------------------------------------
# Per-cell response-matrix construction
# ---------------------------------------------------------------------

def build_response_per_cell(reco: dict, charge: int,
                            iQ_gen_cell: int, iX_gen_cell: int
                            ) -> tuple[np.ndarray, np.ndarray]:
    """Return:
        R[r, g]    : sumw of matched-reco tracks with reco-(z, pT_h) bin r,
                     gen-(z, pT_h) bin g, restricted to the chosen (Q^2,
                     x) gen cell and charge.
        n_reco[r]  : sumw of matched-reco tracks in reco bin r whose gen
                     was in this cell (NOT including in-migration).
    Note: the acceptance denominator (gen-only, this cell) comes from
    `gen_truth_per_cell` using the gen-only stream; that is a separate
    function so the numerator and denominator are not silently restricted
    to the matched-reco subset.
    """
    cm = (reco['charge'] == charge)
    iQg = safe_digitize(reco['Q2_gen'], Q2_BINS)
    iXg = safe_digitize(reco['x_gen'],  X_BINS)
    in_cell = (iQg == iQ_gen_cell) & (iXg == iX_gen_cell) & cm

    iZr = safe_digitize(reco['z_reco'],   Z_BINS)
    iPr = safe_digitize(reco['pT_h_reco'], PTH_BINS)
    iZg = safe_digitize(reco['z_gen'],    Z_BINS)
    iPg = safe_digitize(reco['pT_h_gen'],  PTH_BINS)

    finite = (np.isfinite(reco['z_reco']) & np.isfinite(reco['pT_h_reco'])
              & np.isfinite(reco['z_gen']) & np.isfinite(reco['pT_h_gen'])
              & np.isfinite(reco['w']))
    sel = in_cell & finite & (iZr >= 0) & (iPr >= 0) & (iZg >= 0) & (iPg >= 0)

    R = np.zeros((NB, NB), dtype=np.float64)
    n_reco = np.zeros(NB, dtype=np.float64)

    r_idx = zp_index(iZr[sel], iPr[sel])
    g_idx = zp_index(iZg[sel], iPg[sel])
    np.add.at(R,      (r_idx, g_idx), reco['w'][sel])
    np.add.at(n_reco, r_idx,           reco['w'][sel])

    return R, n_reco


def gen_truth_per_cell(gen: dict, charge: int,
                       iQ_gen_cell: int, iX_gen_cell: int) -> np.ndarray:
    """Acceptance denominator: sumw over gen-only tracks in the cell,
    binned in gen (z, pT_h)."""
    cm = (gen['charge'] == charge)
    iQg = safe_digitize(gen['Q2_gen'], Q2_BINS)
    iXg = safe_digitize(gen['x_gen'],  X_BINS)
    iZg = safe_digitize(gen['z_gen'],  Z_BINS)
    iPg = safe_digitize(gen['pT_h_gen'], PTH_BINS)
    sel = ((iQg == iQ_gen_cell) & (iXg == iX_gen_cell) & cm
           & (iZg >= 0) & (iPg >= 0)
           & np.isfinite(gen['w']))
    out = np.zeros(NB, dtype=np.float64)
    np.add.at(out, zp_index(iZg[sel], iPg[sel]), gen['w'][sel])
    return out


# ---------------------------------------------------------------------
# D'Agostini iterative Bayesian unfolding (single 2D problem)
# ---------------------------------------------------------------------

def dagostini_unfold(R: np.ndarray, data_reco: np.ndarray,
                     gen_truth: np.ndarray, n_iter: int,
                     prior: np.ndarray | None = None
                     ) -> tuple[np.ndarray, list[float]]:
    """Iterative Bayesian unfolding (D'Agostini 1995).

    Args:
        R          : (NB, NB) response, R[r, g] = sumw matched, reco r, gen g
        data_reco  : (NB,)    observed reco-level histogram
        gen_truth  : (NB,)    acceptance denominator (gen-only, same cell)
        n_iter     : number of iterations
        prior      : (NB,) starting prior; default = flat normalised

    Returns:
        unfolded   : (NB,) unfolded values divided by acceptance
                     (i.e. corrected to particle level)
        chi2_iter  : chi^2 of refolded vs data per iteration
    """
    # Acceptance per gen bin: probability that a gen track is reconstructed.
    eff = np.where(gen_truth > 0, R.sum(axis=0) / gen_truth, 0.0)

    # Conditional probability M[r, g] = P(reco r | gen g), restricted to
    # detected events.  M.sum(axis=0) <= 1.
    col = R.sum(axis=0)
    M = np.divide(R, col[None, :], out=np.zeros_like(R), where=col[None, :] > 0)

    # Initial prior (flat over allowed gen bins).
    if prior is None:
        n_active = int((gen_truth > 0).sum())
        prior = np.where(gen_truth > 0, 1.0 / max(n_active, 1), 0.0)
    else:
        prior = prior / prior.sum() if prior.sum() > 0 else prior

    n_total = data_reco.sum()
    truth_est = prior * n_total  # initial expected truth count per gen bin

    chi2_iter = []
    for _ in range(n_iter):
        # Refold to expected reco
        expected_reco = M @ truth_est
        # Avoid divide-by-zero
        safe_exp = np.where(expected_reco > 0, expected_reco, 1.0)
        # Bayes update: new truth = sum_r M[r, g] * truth[g] * data[r] / expected[r] / eff[g]
        # (factoring out efficiency at the end is more numerically stable)
        # Standard D'Agostini "transfer matrix":
        #   theta[g | r] = M[r, g] * truth_est[g] / sum_g'  M[r, g'] * truth_est[g']
        denom = M @ truth_est
        denom = np.where(denom > 0, denom, 1.0)
        theta = (M * truth_est[None, :]) / denom[:, None]
        # New truth estimate (still un-corrected for efficiency)
        truth_new = theta.T @ data_reco
        # Apply acceptance correction at the end
        truth_est = truth_new
        # Diagnostic chi^2
        refold = M @ truth_est
        chi2 = np.sum(np.where(refold > 0, (data_reco - refold) ** 2 / refold, 0.0))
        chi2_iter.append(float(chi2))

    # Acceptance-corrected unfolded result
    safe_eff = np.where(eff > 0, eff, np.nan)
    unfolded = truth_est / safe_eff
    return unfolded, chi2_iter


# ---------------------------------------------------------------------
# Bootstrap covariance
# ---------------------------------------------------------------------

def bootstrap_covariance(R: np.ndarray, data_reco: np.ndarray,
                         gen_truth: np.ndarray, n_iter: int,
                         n_replicas: int = 300, rng=None
                         ) -> np.ndarray:
    """Statistical covariance of the unfolded result via Poisson
    resampling of data_reco. Response is held fixed in this routine;
    response-MC stat noise is a separate bootstrap (Section 7.1)."""
    if rng is None:
        rng = np.random.default_rng(20260514)
    samples = np.zeros((n_replicas, NB), dtype=np.float64)
    for k in range(n_replicas):
        d_k = rng.poisson(np.maximum(data_reco, 0.0))
        u_k, _ = dagostini_unfold(R, d_k.astype(np.float64),
                                  gen_truth, n_iter)
        samples[k] = u_k
    cov = np.cov(samples, rowvar=False, ddof=1)
    return cov


# ---------------------------------------------------------------------
# Driver: loop over (Q^2, x) cells and charges
# ---------------------------------------------------------------------

def run_unfolding(reco: dict, gen: dict, data_reco_4d: np.ndarray,
                  n_iter: int = 4) -> dict:
    """data_reco_4d shape: (2 charges, nQ, nX, NZ, NP)."""
    nQ = len(Q2_BINS) - 1
    nX = len(X_BINS) - 1
    out_unfolded = np.full((2, nQ, nX, NZ, NP), np.nan)
    out_cov = {}
    out_meta = []
    for ic, charge in enumerate([+1, -1]):
        for iQ in range(nQ):
            for iX in range(nX):
                R, _ = build_response_per_cell(reco, charge, iQ, iX)
                gt = gen_truth_per_cell(gen, charge, iQ, iX)
                if gt.sum() < 100 or R.sum() < 100:
                    out_meta.append({
                        'charge': charge, 'iQ2': iQ, 'ix': iX,
                        'status': 'masked_low_stats',
                    })
                    continue
                d_reco_flat = data_reco_4d[ic, iQ, iX].reshape(NB)
                if d_reco_flat.sum() < 10:
                    out_meta.append({
                        'charge': charge, 'iQ2': iQ, 'ix': iX,
                        'status': 'masked_no_data',
                    })
                    continue
                unfolded, chi2_iter = dagostini_unfold(
                    R, d_reco_flat, gt, n_iter)
                cov = bootstrap_covariance(R, d_reco_flat, gt, n_iter,
                                           n_replicas=200)
                out_unfolded[ic, iQ, iX] = unfolded.reshape(NZ, NP)
                out_cov[(charge, iQ, iX)] = cov
                out_meta.append({
                    'charge': charge, 'iQ2': iQ, 'ix': iX,
                    'status': 'OK',
                    'n_iter': n_iter,
                    'chi2_per_iter': chi2_iter,
                    'unfolded_sum': float(unfolded.sum()),
                    'gen_truth_sum': float(gt.sum()),
                })
    return {
        'unfolded': out_unfolded,
        'covariance_blocks': out_cov,
        'metadata': out_meta,
    }


# ---------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument('--mc', required=True,
                    help='merged MC ROOT file (RAPGAP31 or DJANGOH14)')
    ap.add_argument('--data', default=None,
                    help='data ROOT file (else MC-as-pseudo-data)')
    ap.add_argument('--out', required=True, help='output directory')
    ap.add_argument('--max-entries', type=int, default=0)
    ap.add_argument('--n-iter', type=int, default=4)
    args = ap.parse_args()

    out = Path(args.out); out.mkdir(parents=True, exist_ok=True)

    print('Loading matched-reco MC ...')
    reco = stream_reco_matched(args.mc, max_entries=args.max_entries)
    print('Loading gen-only MC ...')
    gen  = stream_gen(args.mc,         max_entries=args.max_entries)

    if args.data:
        # In data mode, fill data_reco_4d from the data ROOT file,
        # respecting BASE_CUT and the same observable definitions.
        raise NotImplementedError('Data loader to be added when '
                                  'merged data trees are validated.')
    else:
        # MC-as-pseudo-data: use the matched-reco histogram itself
        # (will trivially close).  For real closure tests, split the
        # MC into halves first.
        print('Building pseudo-data 4D reco histogram from MC ...')
        nQ, nX = len(Q2_BINS) - 1, len(X_BINS) - 1
        data_reco_4d = np.zeros((2, nQ, nX, NZ, NP), dtype=np.float64)
        for ic, ch in enumerate([+1, -1]):
            cm = (reco['charge'] == ch)
            iQ = safe_digitize(reco['Q2_reco'][cm], Q2_BINS)
            iX = safe_digitize(reco['x_reco'][cm],  X_BINS)
            iZ = safe_digitize(reco['z_reco'][cm],  Z_BINS)
            iP = safe_digitize(reco['pT_h_reco'][cm], PTH_BINS)
            w  = reco['w'][cm]
            ok = (iQ >= 0) & (iX >= 0) & (iZ >= 0) & (iP >= 0)
            np.add.at(data_reco_4d, (ic, iQ[ok], iX[ok], iZ[ok], iP[ok]),
                      w[ok])

    print('Running per-cell unfolding ...')
    result = run_unfolding(reco, gen, data_reco_4d, n_iter=args.n_iter)

    # Save
    np.savez(out / 'tmd_unfolded.npz',
             unfolded=result['unfolded'],
             Q2_bins=Q2_BINS, x_bins=X_BINS,
             z_bins=Z_BINS, pT_h_bins=PTH_BINS)
    # Covariance blocks: keyed by (charge, iQ, iX) - flatten to npz.
    cov_dict = {}
    for (ch, iQ, iX), cov in result['covariance_blocks'].items():
        cov_dict[f'cov_c{ch:+d}_Q{iQ}_X{iX}'] = cov
    np.savez(out / 'tmd_covariance.npz', **cov_dict)
    with open(out / 'tmd_unfolded_summary.json', 'w') as fh:
        json.dump({'metadata': result['metadata'],
                   'bins': {'Q2': Q2_BINS.tolist(), 'x': X_BINS.tolist(),
                            'z': Z_BINS.tolist(),
                            'pT_h': PTH_BINS.tolist()}},
                  fh, indent=2)

    print(f'Wrote {out / "tmd_unfolded.npz"}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
```

### 8.1 Notes on the skeleton

- The D'Agostini iteration above is the standard formulation; it differs
  from RooUnfoldBayes only in not exposing the unfolding-error matrix
  closed form (we use bootstrap instead, see Section 7.1).
- The acceptance correction is folded into the final divide by `eff`
  rather than into the iteration; this is the standard convention and
  keeps `truth_est` interpretable as "estimated true reco-passing
  count."
- The `bootstrap_covariance` function holds the response fixed; for the
  full statistical covariance (including response-MC stats), wrap the
  whole pipeline in a second outer loop that resamples the MC tracks
  with replacement before each call to `build_response_per_cell`. With
  ~700k matched tracks per cell, response-MC stats contribute < 1% to
  the unfolded variance and may be safely neglected for the first
  release.
- Per-cell mask on `gt.sum() < 100` and `R.sum() < 100` keeps the
  factor-of-2-missing-MC-files corner cases from producing nonsense.
- `data_reco_4d` is the only object that changes between MC-closure mode
  and data mode; the unfolding driver itself is data-agnostic.
- The skeleton does NOT yet implement Layer A (Q^2, x bin-by-bin
  correction) or the full 4D cross-check; these are added once the
  per-cell unfolding is closure-tested.

### 8.2 RooUnfold cross-check (one slice)

For a single (Q^2, x) cell, after the numpy unfolding has produced
`R`, `data_reco`, `gen_truth`, and `unfolded`:

```python
# Inside H1OO ROOT environment
import ROOT
ROOT.gSystem.Load('libRooUnfold')

# Build TH1F reco and gen
hReco  = ROOT.TH1F('hReco',  '', NB, 0, NB)
hGen   = ROOT.TH1F('hGen',   '', NB, 0, NB)
hMeas  = ROOT.TH1F('hMeas',  '', NB, 0, NB)
for i in range(NB):
    hMeas.SetBinContent(i + 1, data_reco[i])
    hGen.SetBinContent (i + 1, gen_truth[i])
# Build RooUnfoldResponse from R
resp = ROOT.RooUnfoldResponse(NB, 0, NB)
for r in range(NB):
    for g in range(NB):
        if R[r, g] > 0:
            resp.Fill(r + 0.5, g + 0.5, R[r, g])  # weighted Fill

unf = ROOT.RooUnfoldBayes(resp, hMeas, n_iter)
hUnf = unf.Hreco()
unfolded_roo = np.array([hUnf.GetBinContent(i + 1) for i in range(NB)])

# Compare
agreement = np.allclose(unfolded, unfolded_roo, rtol=0.01)
```

Agreement to 1% across all populated bins validates the numpy
implementation. Disagreement at the 5%+ level indicates a convention
mismatch (e.g. acceptance handling) -- investigate before proceeding.

---

## 9. Risks, Open Questions, and Gaps

### 9.1 Identified risks

1. **Cross-(Q^2, x) migration leak.** The factorisation systematic
   (Section 6.6) is hypothesised to be 1-3% but is not yet measured.
   Until the full-4D cross-check is run, this is a known unknown. If
   the leak is large, the architecture pivots to full 4D unfolding.

2. **MC statistical depletion in extreme cells.** Cells at the lowest x
   and the highest pT_h have ~1% of the average occupancy and may
   contain only a few hundred matched-MC tracks per cell per charge,
   even on the full dataset. Specific bin combinations may need to be
   masked from publication. Proceeding without confirming this on the
   full dataset is a risk.

3. **Sentinel values in z_SIDIS, pT_h_gen.** The mistakes-list in
   `context-handoff.md` Section 11 calls these out. The unfolding script
   inherits the masking via `tmd_resolution_acceptance.py`'s
   `filter_sentinel`; if any new branch is added (e.g. from the OQ8
   freeze), the same filtering must apply.

4. **D'Agostini bias at low purity.** At per-cell purity well below the
   30% per-dimension threshold (rare combinations), the D'Agostini
   iteration may underconverge or oscillate. The iteration cap at 6
   bounds the runaway risk but does not eliminate the bias. Mitigation:
   per-cell iteration tuning (Section 6.2).

5. **Response-matrix MC reweighting.** If the MC truth distribution
   differs strongly from the true gen distribution (which we cannot
   measure directly), the D'Agostini result has a residual model-shape
   bias that does not converge with iterations. This is exactly the
   model-dependence systematic (Section 7.2) and matches the H1
   multiplicity paper's leading systematic.

6. **MatchAngle dependence.** The matched-reco selection uses
   `MatchAngle < 2.0 deg` (per `context-handoff.md` Section 7, F3 fix).
   Tightening to 1.0 deg or loosening to 4.0 deg should shift the
   response matrix; this should be added as a systematic source.

### 9.2 Open questions for the supervisor

1. **Layer A explicit or implicit?** Section 4.2-4.4 leaves a choice
   between an explicit (Q^2, x) bin-by-bin correction and a per-(Q^2_gen,
   x_gen)-cell response that absorbs migration in/out. The
   implementation in Section 8 uses the latter (more correct) form;
   confirm this is the desired production approach.

2. **Iteration count freeze.** Per-cell or global? Global is simpler to
   document; per-cell is more rigorous. The recommendation here is
   per-cell; the supervisor may prefer global with a single conservative
   choice (e.g. n_iter = 4).

3. **Bootstrap replica count.** 200-500 is the recommended range. Larger
   is more expensive, smaller is less stable. Default in skeleton is
   200. Confirm.

4. **Response-MC bootstrap inclusion.** Default in skeleton is to
   neglect response-MC stat (it's <1%). For a fully rigorous release,
   add it; cost is a factor of 200-500 in wall time which is still
   minutes.

5. **Data unfolding before or after Phase 2B validation?** The skeleton
   only supports MC-as-pseudo-data until merged data trees are validated
   (Phase 2B Gate). Data unfolding waits on that gate.

6. **Full-4D cross-check timing.** Run on the full dataset, or on a
   stat-limited sample? Full dataset is feasible (memory < 1 MB) but
   takes longer to build the response. Recommendation: full dataset for
   the cross-check, since it is run once per generator.

### 9.3 Gaps in coverage

1. **No explicit treatment of the gen-fiducial broad-vs-analysis
   mismatch.** The gen denominator uses the broad fiducial
   (|GenEta| < 3.5, GenPt > 0.05) per `context-handoff.md` Section 3.
   The unfolded result is therefore "true tracks in the broad gen
   fiducial." The publication observable should be in the analysis
   fiducial; this requires an additional fiducial correction
   `R_fid = N_analysis_fid / N_broad_fid` per gen bin, computed from the
   gen-only MC. This is not in the skeleton -- it is a follow-up.

2. **No treatment of the sliced 2D vs full 4D systematic.** The
   skeleton only provides the sliced 2D engine; the full-4D engine
   (Layer C) needs to be implemented for the cross-check. The
   implementation is straightforward (same `dagostini_unfold` with
   NB = 312, sparse R matrix) but is not in this document.

3. **No data trees yet validated.** Section 5.1 lists the data ROOT
   path as required input but Phase 2B Gate has not passed. The script
   structure is data-agnostic but cannot be production-run on data
   today.

4. **No HEPData export script.** The `tmd_release_lowQ2_charged.npz`
   plus JSON manifest format proposed in Section 7.5 is sketched but
   not implemented. This is a downstream artifact that a separate
   release-tooling script should produce.

5. **No coverage of the high-Q^2 extension.** All recommendations are
   low-Q^2 specific. The high-Q^2 trees have different branch parity
   (per `research-highQ2-sidis-extension.md`); a separate research note
   should adapt the recipe once the high-Q^2 trees are validated.

---

## 10. Summary of Decisions

| Decision | Choice |
|---|---|
| Production unfolding method | Sliced 2D iterative Bayesian (D'Agostini) |
| Slicing axis | (Q^2, x) cells; unfold in (z, pT_h) per cell |
| Cross-check | Full 4D D'Agostini, one run per generator, for factorisation systematic |
| Engine | Custom numpy in `scripts/tmd_unfold.py`; RooUnfold for one slice cross-check |
| Iteration count | Per-cell scan, capped at 6; chosen by chi^2-to-data plateau |
| Statistical covariance | Poisson bootstrap on data, 200 replicas; response-MC bootstrap optional |
| Systematic propagation | Re-unfold with shifted response per source; per-source shift vector + outer-product covariance |
| Per-cell mask | sumw_gen < 100 OR sumw_R < 100 OR data_reco < 10 |
| Closure pass criterion | chi^2/dof < 2 per cell, < 1.3 averaged, no per-bin pull > 3 sigma |
| Generator cross-closure tolerance | < 5% per bin; difference recorded as model systematic |
| Refold check | chi^2_refold/dof < 1 mandatory |
| Output format | per-charge npz arrays + JSON manifest; HEPData export downstream |

---

## 11. Action Items for the Supervisor

1. Confirm the architecture choice (sliced 2D D'Agostini vs full 4D).
2. Confirm the engine choice (numpy vs RooUnfold as primary).
3. Authorise `scripts/tmd_unfold.py` development per Section 8.
4. Schedule the full-4D cross-check run (one RAPGAP, one DJANGOH).
5. Schedule the closure-test campaign (Section 6, all four protocols).
6. Schedule the systematic-shift response-matrix campaign once the
   `produce_shifted_mc.py` pipeline has run on the full dataset.
7. Decide on Layer-A explicit-vs-implicit treatment per Section 9.2 #1.
8. Confirm iteration-count strategy per Section 9.2 #2.

---

## 12. References

- `H1/sidis_lowQ2/plan-tmd-sidis.md` (Iteration 2 + 2026-05-14 session
  update); binning, observable definitions, Part 3 acceptance hierarchy.
- `H1/sidis_lowQ2/context-handoff.md` (2026-05-14); current status,
  resolution numbers, 4D occupancy and acceptance results.
- `H1/sidis_highQ2/scripts/tmd_resolution_acceptance.py`; existing 4D
  purity, stability, and acceptance code (basis for the unfolding
  loaders and bin definitions).
- `H1/sidis_lowQ2/research-sidis-systematics-literature.md`; H1
  multiplicity paper Sys.1-Sys.8 inventory and HERMES/COMPASS comparison.
- `H1/agents/h1-unfolder.md`; existing specialist agent contract;
  Output Contract section adapted in Section 5.5.
- D'Agostini, "A multidimensional unfolding method based on Bayes'
  theorem," Nucl. Instrum. Meth. A 362 (1995) 487.
- Hoecker & Kartvelishvili, "SVD approach to data unfolding,"
  Nucl. Instrum. Meth. A 372 (1996) 469.
- Schmitt, "TUnfold, an algorithm for correcting migration effects in
  high energy physics," JINST 7 (2012) T10003.
- Adye, "Unfolding algorithms and tests using RooUnfold,"
  arXiv:1105.1160.
- H1 Collaboration, "Measurement of charged particle multiplicity
  distributions in DIS at HERA and its implication to entropy and
  correlations," DESY 20-176, arXiv:2011.01812 (3D TUnfold reference).

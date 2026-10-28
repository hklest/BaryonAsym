# Critic Report: research-critic on rev5 unfolding methodology

**Date:** 2026-05-31 (post-rev5)
**Scope:** Defensibility of rev5 methodology choices vs published literature.

---

## Fatal Methodology Concerns

### M1. `kEConstraintNone` is not standard for inclusive multiplicity unfolding.

Schmitt 2012 (JINST 7, T10003) §3 explicitly motivates `kEConstraintArea` as the default constraint for measurements where the integrated event count carries physical meaning (cross-sections, multiplicities). Removing the constraint entirely is appropriate for shape-only measurements or for spectra where the absolute normalization is independently fixed downstream. For inclusive SIDIS multiplicities, the integrated total IS the physical observable (multiplicity = N_h / N_DIS, both at gen level).

The rev5 choice was driven by an algebra error in the rev4 F1 formula. The correct response to the F1 bug is NOT to drop the constraint, it is to fix F1. Specifically:
- rev3 used `prior_total` in the denominator → undercount by reco_eff factor.
- rev4 used `matched_total` in the denominator → produced 2-D scaling artifact (5.06e6 = data/reco_eff²) because of NucliaWeight asymmetry between matched-reco-fill (`w1·w2·NW`) and miss-row-fill (`w1·w2`).
- The minimal fix is **harmonize the NucliaWeight handling between matched-reco and miss-row fills in the build stage**, then restore `kEConstraintArea` with the rev4 matched_total denominator. That eliminates the +21% inflation and recovers the physically-motivated constraint.

Until either the constraint is restored with corrected F1 OR a published reference for `kEConstraintNone` in inclusive multiplicity unfolding is cited, the choice is not defensible in review.

**Reference:** Schmitt, JINST 7 (2012) T10003, §3.2 "Auxiliary conditions". Britzger TUnfold v17 user manual, §4.4 "Area constraints".

### M2. Per-bin τ-selection by "N_good gate-counting" is unpublished methodology.

Standard TUnfold τ-selection methods are:
- **L-curve kink (Tikhonov)**: `ScanLcurve` — Hocker & Kartvelishvili 1996, NIM A 372, 469.
- **Min global correlation**: `ScanTau` with `kEScanTauRhoAvg` — Schmitt 2012 §4.3.
- **Min refold χ²/ndf** with closure check.

The rev5 "N_good = number of bins passing all 4 gates (bias, variance, correlation, closure)" metric is a custom construction. With 8/240 = 3.3% pass rate, the gate thresholds are restrictive enough that the metric is effectively counting outliers, not selecting τ in the usual sense. Composite-metric τ disagrees (1.62e-5 vs 1.44e-6 by ~10×) — symptomatic of an unstable selection.

Cite at least one of:
- L-curve τ from `ScanLcurve` as cross-check.
- ρ_avg τ from `ScanTau` as cross-check.
- Closure-based τ choice with documented bias-variance plot.

Without an L-curve or ρ_avg cross-check, the τ choice is not defensible.

**Reference:** Hocker & Kartvelishvili, NIM A 372 (1996) 469. Schmitt 2012 §4.

---

## Defensibility Concerns

### D1. Per-charge unfolding at 0.35% charge confusion needs explicit precedent.

Joint pos+neg unfolding with charge as a response dimension is the textbook approach for charge-asymmetric observables when charge confusion is non-negligible. At 0.35% integrated confusion the per-charge approach is plausibly OK, but:
- The decision should cite a HERA-era precedent for the per-charge factorization at sub-1% confusion. ZEUS charged-current cross-section papers used joint unfolding with charge as a response axis; H1 inclusive ep cross-section likewise.
- If no precedent, the σ_chargeconf systematic must explicitly bound the bias: typical convention is 2× confusion = 0.7% per-bin systematic, propagated as fully correlated between pos and neg per published bin.

**Reference:** Britzger TUnfold note §5 examples; H1 / ZEUS charged-current papers (e.g., H1 Collab., EPJ C71 (2011) 1579, §4.3).

### D2. 4× reco/gen ratio is at the upper end of Schmitt's recommendation.

Schmitt 2012 §4 recommends n_reco ≥ 2·n_gen for stable inversion. The rev5 layout uses 4× — fine in principle, but with NB_R = 960 and NB_G = 240, the per-fine-reco bin statistics shrink by 4× relative to the published binning. For low-statistics corners (S3b high-z), this halves the effective stat power per response-matrix cell and inflates the small-eigenvalue tail of the singular value spectrum, which is exactly what drives the high σ_reg observed at small τ.

A 2× ratio (NB_R = 480) is a more standard choice. Either:
- Document a stability comparison showing 4× vs 2× produces consistent τ choice and per-bin σ.
- Reduce to 2× as the production layout.

**Reference:** Schmitt 2012 §4.2 "Number of bins".

### D3. Custom L matrix (intra-cell only, no cross-cell smoothing) is unconventional.

Standard TUnfold practice is full 2D curvature on the flattened bin index, OR block-diagonal curvature with explicit cross-block penalties at boundaries. The rev5 choice — intra-(cell, z) pT smoothing and intra-(cell, pT) z smoothing with NO cross-cell terms — risks under-regularization at cell boundaries (where the unfolded solution can oscillate freely between cells with no penalty) and over-regularization within cells (where intra-cell smoothness is enforced more strongly than the data warrant).

Defensible alternative: use TUnfold's `RegularizeDistribution` with `kRegModeCurvature` and a TUnfoldBinning that explicitly declares the 4D (Q², x, z, pT) topology, which gives boundary-aware curvature for free.

**Reference:** Britzger TUnfold manual §6 TUnfoldBinning examples.

### D4. σ_model with RAPGAP as denominator is asymmetric.

`σ_model = |N_RG − N_DJ| / N_RG` is biased toward RAPGAP. Standard alternatives:
- Symmetric: `|N_RG − N_DJ| / ((N_RG + N_DJ)/2)`
- Half-spread: `|N_RG − N_DJ| / 2 / max(N_RG, N_DJ)`
- Quadrature half-spread: `|N_RG − N_DJ| / (2·N_central)`

For a 16.8% integrated diff, the convention choice shifts the headline by ~1.5% (not large) but the per-bin medians can differ more. Pick the symmetric convention and document it.

**Reference:** H1 SIDIS multiplicities at low-Q² (e.g., H1 Coll., NPB 485 (1997)) used symmetric half-spread; ZEUS analogous.

### D5. Diagonal-only σ_total for regularized unfolding is reviewer-blocking.

Every modern HERA unfolding publication quotes the full covariance matrix or at minimum publishes the per-bin σ AND the off-diagonal correlation matrix. Examples:
- H1 inclusive jet cross-sections (e.g., EPJ C77 (2017) 215) quote bin-bin correlation matrices.
- ZEUS dijet (PRD 85 (2012) 052008) provides covariance.
- Both H1 and ZEUS multi-differential SIDIS (multiplicity) papers since 2010 propagate the covariance through to plotted error bars (showing diagonal-only) AND tabulate the off-diagonal correlations in HepData.

Publishing diagonal-only σ at the headline level WILL be flagged by referees. The covariance must be propagated through all derived observables (integrated multiplicities, asymmetries, projection-marginalizations).

**Reference:** H1 EPJ C77 (2017) 215. HepData entry conventions for unfolded data.

### D6. Background subtraction Q²gen < 60 MC-based without data closure.

Subtracting MC-predicted events with Q²gen < 60 from the data reco distribution is standard ONLY when the MC describes the low-Q² spillover at the percent level. The gap 60 < Q²gen < 150 is not addressed in rev5. For Q²gen < 60, the subtraction relies on:
- Correct MC luminosity scaling (verify with `h1-normalization`).
- Correct MC modeling of the Q² migration from low-Q² to high-Q² reco — this is a fundamentally hard region to model because the photoproduction-to-DIS transition involves resolved vs direct contributions that PYTHIA-like generators handle differently than DJANGOH.
- Data-MC closure in a tagged Q²_reco < 150 control region (no evidence in rev5 evidence pack).

**Reference:** H1 Coll., EPJ C71 (2011) 1769 for photoproduction subtraction in inclusive DIS; H1 Coll., NIM A 393 (1997) for the standard DIS event selection in high-Q² with photoproduction-bgsub.

---

## Citation Gaps

### G1. Schmitt 2012 not cited in any rev5 code or doc.

The rev5 design references "Schmitt requirement n_reco ≥ n_gen" verbally but no source pointer in code comments or docs. Add citation to the binning_v3.py header docstring.

### G2. Britzger Encoding B miss-row choice not cited.

Encoding B is a specific Britzger convention from the TUnfold manual; the alternative (Encoding A: miss row on gen-axis instead) has different algebraic properties. The rev5 choice should be cited and the consequences documented.

### G3. D'Agostini IBU paper not cited.

`ibu_core.py` references "D'Agostini 1995, NIM A 362, 487" in the docstring (good) but the regularization choice (number of iterations as proxy for regularization strength) is from D'Agostini's later 2010 update (arXiv:1010.0632). Cite both.

### G4. No reference to recent HERA unfolding papers as methodological anchor.

H1 inclusive jet (EPJ C77, 2017, 215), H1 single hadrons in DIS (forthcoming?), ZEUS dijets (PRD 85, 052008) all use TUnfold with detailed methodology documentation. None are cited as comparators in rev5 docs.

---

## Alternative Methodologies Not Considered

### A1. Joint pos+neg unfolding (not just per-charge).

A 2 × 240 = 480 gen-axis with charge as the leading dimension would give automatic charge-confusion correction in the response. The rev5 per-charge architectural choice cuts this off without explicit comparison.

### A2. OmniFold / iterative reweighting.

The handoff mentions OmniFold is in-flight as a fourth method (clusters 3559797, 3559798). Good — this should provide an ML-based cross-check. But OmniFold needs to be compared against the same per-bin σ_model decomposition, not just integrated agreement, to be informative.

**Reference:** Andreassen et al., PRL 124 (2020) 182001.

### A3. Multifold / dimensionality reduction.

A 4D measurement with only 240 published bins and 1.47e6 total data events has ~6000 events per bin on average but min ~100 in S3b corners. A reduced 3D measurement (collapse one of Q², x, z, pT) would give better stat power for the published binning and is the standard fallback when 4D bins are statistics-limited.

---

## Verified-Defensible Choices

- **Per-charge separation** at 0.35% confusion is borderline-defensible IF a σ_chargeconf systematic is added (see D1).
- **Inverted reco-fine / gen-coarse layout** is correct per Schmitt 2012 (modulo the ratio choice in D2).
- **Custom L matrix** is a defensible departure from default IF justified by physics arguments about cell-boundary independence; needs a documented basis.
- **Trust mask at reco_eff < 5%** is consistent with standard practice of excluding bins where the response is statistically untrustable.
- **Closure cross-checks** (selfRG, selfDJ, crossRG, crossDJ modes in the macro) are correctly constructed and properly handle the pseudo-data overflow per the rev3 §F-C fix.

---

## Recommended Next Research Step

**Single most decisive next action:** Run the L-curve scan (`ScanLcurve`) and the ρ_avg scan (`ScanTau` with `kEScanTauRhoAvg`) on the rev5 response and compare the τ choices to the current "N_good" winner. If the three methods (L-curve, ρ_avg, N_good) converge to within a factor of 3, the rev5 τ = 1.44e-6 is defensible and the N_good metric serves as additional context. If they diverge by more than a factor of 10, the τ choice is unstable and the entire single-tau-canonical concept needs reconsideration (likely needs a stability-band cross-section quoting τ-induced variation as a systematic).

Companion action: cite Schmitt 2012, Britzger v17 manual, and at least one HERA-II multi-differential unfolding paper in `binning_v3.py` and `merge_migration_per_charge.py` headers so methodology defensibility is documented in code, not just in handoff notes.

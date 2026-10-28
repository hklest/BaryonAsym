# TUnfold rev5 physics-correctness critic — relentless attack

> 📍 **See [SYNTHESIS.md](SYNTHESIS.md) for the cross-critic prioritized action list.** This is one of four critic reports; do not act on it in isolation.

---


**Verdict:** **CONCERN** (3 fatal physics issues, 4 major, 3 methodology defensibility gaps)
**Material reviewed:** unfold_tunfold_single_tau.C, merge_migration_per_charge.py, build_migration_th2_per_charge.py

---

## Fatal Physics Issues

### TP1. **kEConstraintNone removes physical normalization anchor — multiplicity meaningless**

For inclusive charged-hadron SIDIS multiplicities, the headline observable is
`M^h = N_h_unfolded / N_DIS_unfolded / (Δz · ΔpT)`.
The integrated `N_h_unfolded` is a physical observable in its own right (the charged hadron yield per DIS event). Without `kEConstraintArea`, TUnfold's integrated `N_h_unfolded` is not tied to data; it floats with regularization choice.

The rev5 result `N_h_unfolded ≈ 3.19e6` happens to equal `data / reco_eff_MC` (the natural target), so the user-facing number looks fine. **But this is empirical, not enforced.** A different L matrix or different τ in the same scan would give a different integrated total without violating any internal consistency.

**Severity:** **The headline cross-section number has no provable normalization.** This is the single most reviewer-blocking issue in the rev6 audit.

### TP2. **F1 algebra was dropped without an alternative normalization mechanism**

Rev3 had F1 with `data/prior * miss_MC` (wrong denominator → 21% bias).
Rev4 had F1 with `data/matched * miss_MC` (algebra fixed but produced 5.06e6 = data/reco_eff² because NucliaWeight asymmetry between matched and miss fills).
Rev5 dropped F1 → set overflow to 0 → kEConstraintArea → kEConstraintNone.

**The rev5 choice was a workaround for the NucliaWeight asymmetry, NOT for the F1 algebra.** The correct fix was to harmonize NucliaWeight, restore F1, and keep kEConstraintArea. Three rounds of critiques have flagged this:
- Rev3 critic-1 §F-A: "NucliaWeight asymmetry deferred to rev5".
- Rev5 critic-2 IB3: "NucliaWeight asymmetry deferred to rev5 but never fixed".
- Rev6 IBU critic-1 IB3: same issue.

The rev5 TUnfold "fix" of removing the constraint papers over the underlying bug rather than solving it.

**Severity:** Tied to TP1; together they say the rev5/rev6 TUnfold has no physically-anchored normalization.

### TP3. **Custom L matrix shape is unverified vs alternatives**

The 304-triplet intra-cell L matrix encodes a strong prior: "within each (Q², x) cell, the multiplicity is smooth in z and pT; across cells, no smoothness". The physics motivation is reasonable for SIDIS where cells differ kinematically.

**But:** No comparison run shows that:
- This L gives a smaller bias than e.g. TUnfoldBinning auto-curvature (which would couple cell boundaries via the binning declaration).
- This L gives a smaller σ_reg than a simpler global curvature.
- The L-curve kink at the user-chosen τ=1.4e-6 actually corresponds to the kink of THIS L matrix (vs the default L that ScanLcurve would use).

In Schmitt 2016's terminology, the L matrix is "the regularization conditions" and its choice is **subjective scientific prejudice**. Subjective is OK; **untested subjective is dangerous**.

**Severity:** Cell-boundary bins (the 8 boundaries at every multiple of 30) could be the most biased or most uncertain bins in the output. Has not been tested.

**Fix:** Run a comparison study: rev6 L matrix vs default global curvature vs TUnfoldBinning-auto-curvature, all at the same τ. Quote the spread of per-bin σ_reg as a "L matrix choice systematic".

---

## Major Concerns

### TP-M1. Background subtraction at reco level, then TUnfold internal fake handling — double-counting risk

The rev5 pipeline:
1. Upstream: subtract Q²gen<60 spillover from `data_bgsub`.
2. TUnfold uses the response matrix that includes fake column (R[:, NB_G] from build_migration).
3. With `kEConstraintNone` and `data_overflow=0`, TUnfold internally treats the data as fake-free and the fake column as MC-only.

**Question:** Is the rev5 implicitly assuming the upstream bgsub removes the same events that the fake column would catch? If yes, no double-count. If no, partial overlap → over-subtraction at 1-3%.

The IBU rev6 path explicitly subtracts fakes via `_fake_subtract.py`, then uses fake-free response. TUnfold rev5 leaves the fake column in the response but data has no fake-overflow data → effectively the fake column is unused.

**Severity:** Depends on what upstream bgsub does. Need to confirm with `h1-normalization` or background_subtraction code.

### TP-M2. Per-charge unfolding loses charge confusion

Same as IBU critic IP-M2. Per-charge TUnfold has the same 0.35% integrated charge confusion as a missing systematic. Documented but not propagated.

### TP-M3. Bin-by-bin migration mismatched with τ-stability window

At rev5 canonical τ=1.4e-6, bin migration is partially regularized away. At τ=1e-7 (the plateau center per the tuning analysis), migration is much less regularized. The per-bin σ_model = |unf_RG − unf_DJ|/2 differs between the two τ choices because the regularization shape interacts with the bias differently.

**Severity:** σ_model at canonical τ is NOT the same as σ_model at the "physically natural" small-τ point. If we publish σ_model from the canonical τ as the "model uncertainty", we are conflating model uncertainty with regularization choice.

**Fix:** Quote σ_model at the τ-stability-plateau center (where regularization is minimal), not at the canonical τ.

### TP-M4. Refold χ²/ndf computation appears anomalous

From rev5 single-tau JSON for h+ at τ=1.4e-6: `refold_chi2_per_ndf` value not in current evidence pack but typically reported. If it's ~1 per data bin, OK. If >>1 (as the IBU refold at 758 suggests), TUnfold may have similar issues.

**Severity:** Open. Need to inspect actual TUnfold refold χ²/ndf number.

---

## Methodology Defensibility Gaps

### TP-D1. Tikhonov τ chosen by ad-hoc multi-metric gate, not L-curve

Rev5 picked τ=1.44e-6 via "N_good gate-counting" (8/240 bins pass all 4 gates). This is not a published methodology. Schmitt 2012/2016 recommends L-curve scan (`ScanLcurve`) or global-correlation scan (`ScanTau` with `kEScanTauRhoAvg`).

**Defensibility:** Reviewers will ask "what is the L-curve τ for this analysis?" — and currently we don't have one. The rev6 ScanLcurve macro is drafted but not run.

### TP-D2. σ_reg quoted via "narrow neighbor" definition with inconsistent step size between IBU and TUnfold

Earlier in the session we found TUnfold's narrow-neighbor σ_reg = 0.11% vs IBU's 0.35%, but the τ grid was 4× per step while IBU N grid was 1.67× per step. Apples-to-apples (stability-window) gives ~0.3% for both. The σ_reg headline number depends on the scan grid spacing — must be standardized.

**Fix:** Quote σ_reg with explicit definition AND scan spacing parameters.

### TP-D3. No comparison to ZEUS or H1 published multiplicities for sanity check

The integrated unfolded number 3.19e6 / the superseded pre-rescale luminosity nb⁻¹ → ~10.7 charged hadrons per nb of integrated luminosity. Per DIS event (with `N_DIS` ≈ few × 10⁵), this is a few charged hadrons per event — physically reasonable but not yet compared to ZEUS NPB 776 (2007) 1 or H1 NPB 485 (1997) hadron multiplicities.

**Fix:** Publish ratio to a reference HERA SIDIS multiplicity in similar Q² range.

---

## Recommended Next Physics Check

**Single most decisive next action:** Run ScanLcurve in the rev6 macro AND restore kEConstraintArea with the rev6 fake-handling logic. Then:

1. Confirm the L-curve kink τ agrees with the manual canonical τ to within 3×.
2. Confirm the integrated total at the L-curve τ equals data/reco_eff_MC (the natural target) to <1%.
3. Re-derive σ_model at the L-curve τ (which should be at smaller τ than the manual canonical, in the regularization-minimal plateau).

If all three pass, TUnfold rev6 is publication-ready. If any fail, the headline normalization is broken and the multiplicity result cannot be quoted.

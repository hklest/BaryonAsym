# TUnfold rev5 implementation critic — relentless file:line audit

> 📍 **See [SYNTHESIS.md](SYNTHESIS.md) for the cross-critic prioritized action list.** This is one of four critic reports; do not act on it in isolation.

---


**Verdict:** **CONCERN** (4 fatal, 6 major, 5 latent)
**Files audited:** `unfold_tunfold_single_tau.C` (1330 lines), `merge_migration_per_charge.py` (652 lines)

---

## Fatal Bugs

### TB1. `unfold_tunfold_single_tau.C:439` — `kEConstraintNone` chosen as canonical against Schmitt 2016 recommendation

```cpp
TUnfoldDensity unfold(h_R,
                      TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeNone,
                      TUnfold::kEConstraintNone,  // REV5: no area constraint
                      TUnfoldDensity::kDensityModeNone);
```

**Bug:** Schmitt 2016 (JINST 7, T10003) §3.2 explicitly recommends `kEConstraintArea` as the default for inclusive cross-section measurements where the integrated total is itself a physical observable. Multiplicity is exactly such a measurement (`M^h = N_h_corr / N_DIS_corr / D`). Removing the constraint was a workaround for the rev4 F1 algebra bug; the real fix was to repair F1, not to drop the constraint.

**Severity:** Without area constraint, TUnfold's integrated total is not pinned to data — it floats with τ in a way that doesn't have a physical reference point. Current rev5 canonical τ=1.4e-6 gives integrated 3.19e6, which AGREES with data/reco_eff (the natural target). But the agreement is **coincidental**; with a different L matrix or different τ neighborhood, the integrated total drifts unpredictably.

**Fix:** Restore `kEConstraintArea`, AND fix the F1 area-target calculation in `merge_migration_per_charge.py` (currently sets `miss_yield_data_scaled = 0.0` at line 407, killing F1 entirely). The rev6 path I drafted (`unfold_tunfold_scan_lcurve.C`) supports `use_area_constraint` parameter, but the canonical macro still uses `kEConstraintNone`.

### TB2. `merge_migration_per_charge.py:407` — F1 area target hard-coded to 0 (dead F1 logic)

```python
miss_yield = 0.0                              # REV5: no data at overflow
ne[NB_R] = miss_yield
```

**Bug:** The diagnostic prints below at lines 415-426 still compute and report `matched_total`, `miss_row_mc`, `reco_eff`, `data_total/reco_eff` as if F1 were active. **The dead computations invite an accidental re-enable in future edits, AND the diagnostic output is misleading** (suggests F1 is being computed when it's being overridden).

**Severity:** Maintenance trap. Future edit could re-enable F1 without realizing rev5 dropped it.

**Fix:** Either delete the dead F1 computation block entirely, OR wrap in `if F1_ENABLED:` with feature flag at top of file.

### TB3. `unfold_tunfold_single_tau.C:959-993` — score function `area_residual_rel` term is dead-weight at rev5

```cpp
double miss_yield_overflow = h_data->GetBinContent(NB_R_EXT);  // = 0 at rev5
double dtot_with_overflow  = dtot + miss_yield_overflow;       // = dtot at rev5
double area_residual_rel   = std::fabs(tot_unf_pub - dtot_with_overflow)
                           / std::max(dtot_with_overflow, 1.0);
score_tau = ... + 1.0 * area_residual_rel;
```

**Bug:** With `kEConstraintNone`, `tot_unf_pub` is NOT supposed to equal `dtot`. The natural target is `dtot / reco_eff ≈ 2.7e6`, not `dtot = 1.5e6`. So `area_residual_rel ≈ |3.19 − 1.5|/1.5 ≈ 1.13` per τ. The `+ 1.0 × 1.13` adds a roughly constant ~1.13 to every τ's score, distorting the user-facing score interpretation.

**Severity:** Argmin discrimination preserved (the term is τ-independent at this precision), but the per-τ "score = ~1.5 + ..." appears alarming. Users may misread as "13% deviation" when it is structural.

**Fix:** Disable the term when `kEConstraintNone` is detected:
```cpp
bool area_constraint_active = (cstr == TUnfold::kEConstraintArea);
score_tau = ... + (area_constraint_active ? 1.0 * area_residual_rel : 0.0);
```

### TB4. `unfold_tunfold_single_tau.C:447-482` — custom L matrix excludes cell-boundary smoothness, undocumented physics motivation

```cpp
// Smoothness along pT_h within (cell, iz)
for (int c = 0; c < NC; ++c)
  for (int iz = 0; iz < NZ; ++iz)
    for (int ipt = 0; ipt + 2 < NPT; ++ipt) {
      int iL = c*30 + iz*NPT + ipt;
      ...
      unfold.RegularizeCurvature(iL, iC, iR);
    }
// Smoothness along z within (cell, ipt) — same pattern
```

**Bug:** The L matrix has 144 + 160 = 304 triplets, all intra-cell (no smoothing across Q² or x). Physics motivation: SIDIS multiplicity is expected to vary smoothly within (Q², x) cells but to differ between cells (since the kinematics are very different). Sounds defensible.

**But:** No documented test of whether the 8 cell-boundary bins (240 = 8 cells × 30 inner bins; each cell has 30 bins; boundaries are sharp at every multiple of 30) suffer from over- or under-fitting due to lack of cross-cell coupling. The L-curve cross-check (Schmitt's `ScanLcurve`) is NOT in the canonical macro — only the manual τ scan is.

**Severity:** Custom L may be appropriate but is unproven. Cell-boundary bins could be the most-correlated or most-suspect bins in the output.

**Fix:** Run `ScanLcurve` (now drafted in `unfold_tunfold_scan_lcurve.C`) and confirm L-curve kink-τ agrees with the manual canonical τ=1.4e-6. If they differ by >2×, the L matrix shape needs revisiting.

---

## Major Implementation Concerns

### TM1. `unfold_tunfold_single_tau.C:540` — h_zsigma_gt10_pub gate threshold "0" is too aggressive

```cpp
bool gate_n_zsigma     = (n_zsigma_gt10_pub > 0  );
```

Any single bin with |unf/σ| > 10 fails the gate. With 240 bins, the false-positive rate from Poisson noise alone is non-negligible (~few percent of toy runs). Trust mask should exclude S3b corners before this gate triggers.

### TM2. `merge_migration_per_charge.py:540` — `h_binmap_gen_to_reco` histogram name MISLEADING

The histogram has length NB_R (960) and contains reco→gen aggregation. Its name preserves rev2 backward-compat but is exactly the reverse of what its name suggests. **Future consumer reading the file with assumption of gen→reco will be wrong by transposition.**

**Severity:** Already flagged in earlier critique. STILL not fixed.

**Fix:** Write both names; deprecate the misleading one.

### TM3. `unfold_tunfold_single_tau.C:540` — `n_neg_eig` gate uses TUnfold's eigendecomposition output, but TUnfold normalizes covariance differently per version

The gate `n_neg_eigenvalues_pub > 0` flags any negative eigenvalue. As noted in TM4 below, numerical tolerance threshold is added at line 977-983. Good. But the tolerance is `-1e-10 * |λ_max|` — for very small covariance matrices the tolerance is too tight (1e-10 of max is below double precision floor for small dynamic range). Edge case: condition number > 1e10 already gets hard-rejected, but the eigenvalue gate also fires.

### TM4. `unfold_tunfold_single_tau.C:583-598` — fallback aggregation path uses transposed binmap unsafely

The "legacy layout" path at line 583 expects `binmap[g]` to give reco bin. If misread under inverted layout where binmap[r] gives gen bin, the aggregation silently produces wrong results. Lines 254 detects inverted_layout correctly so this path is dead code, but it's a future-edit footgun.

### TM5. Data overflow bin set to zero but variance to median (`merge_migration_per_charge.py:409-411`)

```python
v_pos = v_f[v_f > 0]
ve[NB_R] = float(max(np.median(v_pos), 1.0)) if v_pos.size > 0 else 1.0
```

Variance of a bin we set to ZERO is set to the median variance of the physical bins. This is a placeholder — the TUnfold internals will use this as the Poisson variance of the overflow constraint, which is wrong scale (off by ~3 orders of magnitude). Since rev5 also drops the constraint at line 439, this never affects the result. But if rev6 restores `kEConstraintArea`, the variance scale matters.

### TM6. `merge_migration_per_charge.py:243-249` — REV2 §F4 variance scaling fix is fragile

```python
n_fine[fine_idx] = n_coarse[parent] / float(N_RECO_PER_GEN)
v_fine[fine_idx] = var_coarse[parent] / float(N_RECO_PER_GEN)  # Poisson 1/4
```

The variance scaling assumes Poisson; for bg-subtracted data the variance is non-Poisson. The 1/N_RECO_PER_GEN scaling is correct for Poisson, generally wrong for non-Poisson. This is the same FALLBACK comment that's been documented since rev2 — proper fix is re-extraction at 960-bin fine reco binning from per-track CSV. Still not done.

---

## Latent Risks

### TL1. `unfold_tunfold_single_tau.C:436-440` — `kRegModeNone` chosen because custom L is registered next

The constructor uses `kRegModeNone` because the custom L is added later via `RegularizeCurvature`. **If anyone removes the custom-L block (lines 447-482) without changing kRegModeNone → kRegModeCurvature, TUnfold runs with ZERO regularization — matrix inversion semantics.** No safeguard.

### TL2. `unfold_tunfold_single_tau.C:735-754` — closure metric uses `prior` for both response prior and "truth" target

For self-closure (response_mc == pdata_mc), the prior == truth, so this is correct. For cross-closure (mode "crossDJ"), the macro at line 286-298 loads `h_prior_gen_<chg>_<pdata_mc>` separately from the response prior. This is correct. But the implementation depends on the histogram naming convention `h_prior_gen_<chg>_<mc>` which must match what `merge_migration_per_charge.py` writes. If naming drifts, silent crash.

### TL3. `merge_migration_per_charge.py:587, 600` — `meta_f1_formula_version` string is stale

```python
"f1_formula_version": "rev5_kEConstraintNone_no_overflow",
```

This metadata claims F1 is "rev5" but the actual algebra was dropped. The string is decorative, no downstream consumer parses it, but future audits will be confused.

### TL4. `unfold_tunfold_single_tau.C:1064-1070` — `h_emat_pub` is written WITHOUT propagating the full covariance through aggregation

For inverted layout (lines 572-580), the aggregation is trivial direct-copy of the gen-axis cov. For legacy layout (lines 581-598), `T*cov*T^T` is computed by summing per-pair, which is mathematically correct but doesn't preserve negative correlations from regularization. Should add a positive-semi-definite check on the resulting cov_pub.

### TL5. The macro writes both `h_unfolded_pub` (240 bins) and `h_unfolded` (240+1 = 241 if write_cov=true). Downstream consumers must know which to read.

The bug is subtle: `h_unfolded` is the raw TUnfold output, which has NB_G+1 bins (last is fake column). Reading `h_unfolded` as 240 bins would correctly drop bin 241, but no documentation in the JSON schema.

---

## Verified-Correct Components

- `merge_migration_per_charge.py:444`: `binmap` correctly built via `build_binmap_reco_to_gen()`.
- `unfold_tunfold_single_tau.C:447-482`: 304 triplets computed correctly (144 pT + 160 z, verified by counter).
- `unfold_tunfold_single_tau.C:286-340`: closure pseudo-data overflow computation `closure_overflow = prior - matched` is consistent with the gen-pool conservation law.
- TUnfold internals (`SetInput`, `DoUnfold(tau)`, `GetEmatrixTotal`): trusted black-box from arXiv:1205.6201, version 17.6+.
- Per-charge processing: no cross-charge histogram leaks in either build or merge or macro.

---

## Recommended Next Implementation Step

**Single most decisive fix:** Run TUnfold ScanLcurve (the rev6 macro I drafted) and compare its kink-τ to the manual canonical τ=1.4e-6. If they agree to within a factor of 3, the manual τ choice is defensible. If they disagree by >10×, the τ-selection methodology needs revisiting before publication.

Then in priority order:
1. **TB1+TB2+TB3 jointly**: restore kEConstraintArea, fix F1 to use proper data-scaled miss yield (matched-total denominator per rev4 algebra), disable area_residual_rel score term when kEConstraintArea is NOT active.
2. **TM2**: rename `h_binmap_gen_to_reco` → `h_binmap_reco_to_gen` with alias for backward compat.
3. **TL1**: add assert at line 482 that if no custom-L triplets registered, kRegModeCurvature is used as default.

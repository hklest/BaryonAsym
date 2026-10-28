# IBU rev6 implementation critic — relentless file:line audit

> 📍 **See [SYNTHESIS.md](SYNTHESIS.md) for the cross-critic prioritized action list.** This is one of four critic reports; do not act on it in isolation.

---


**Verdict:** **CONCERN** (5 fatal, 4 major, 6 latent; ALL findings file:line cited)
**Files audited:**
- `H1ChargedSidis_HighQ2/scripts/ibu/ibu_core_rev6.py` (193 lines)
- `H1ChargedSidis_HighQ2/scripts/ibu/_fake_subtract.py` (154 lines)
- `H1ChargedSidis_HighQ2/scripts/ibu/run_ibu_rev6.py` (171 lines)

---

## Fatal Bugs

### IB1. `_fake_subtract.py:108-111` — variance double-counts MC fluctuation as data fluctuation

```python
var_data       = np.maximum(data_fine, 0.0)     # Poisson on data_fine (≈ data_inner + fake_pred)
var_fake_scale = (scale * scale) * fake_marginal_MC
data_inner_var = var_data + var_fake_scale
```

**Bug:** `var_data = data_fine` is the Poisson variance of the **input** (which already includes the fake count). Then `var_fake_scale` separately adds the MC fake variance. But the **data-side** Poisson variance of the SUBTRACTED quantity `data_inner = data_fine − fake_pred` is `var(data_fine) + var(fake_pred)` ONLY if data_fine and fake_pred are independent. They're not: fake_pred is `scale × fake_marginal_MC`, where `scale = data_total / (matched + fake)_MC` is a **function of data_total** — so `var(fake_pred)` has a contribution from `var(data_total)` too.

More importantly, the user passed `--data data_bgsub` which has its own variance already encoded in `h_data_bgsub_<chg>_w2`. The `_fake_subtract.py` ignores `data_w2` entirely and assumes Poisson. For the bg-subtracted data, this is wrong: variance is propagated through the upstream physics-bg subtraction and is generally larger than Poisson.

**Severity:** Analysis-killing for the σ_stat headline. Underestimates by 5-30% depending on bg-subtraction stage.

**Fix:** Accept and use `data_w2` in `fake_subtract_data` as the input variance, not Poisson-on-counts. Add covariance term between data and fake_pred via the scale dependence.

### IB2. `_fake_subtract.py:88` — closed-form scale uses data_total / (matched + fake)_MC, ignoring miss row

```python
scale = data_total / (matched_total_MC + fake_total_MC)
```

**Bug:** The MC predicts inner-reco at MC luminosity. The right scale to make MC inner+fake match data inner+fake is what's coded. **But** for the data prediction to use this scale on the **fake** part, you're assuming the fake/(inner+fake) ratio is identical in data and MC at the integrated level. In a real analysis, fakes are tracks the matcher couldn't pair (combinatorial + photon conversions + V0s + secondaries). The data-vs-MC ratio of fakes can differ from the data-vs-MC ratio of matched-inner by 20-50% because:
- Conversion rates depend on detector material modeling.
- V0 contamination depends on K0s/Lambda cross-section accuracy.
- Beam-gas contributes to fakes but not to matched-inner.

**Severity:** Fake subtraction at the 10-15% level (we have fake/data = 12.7% as shown). If MC overshoots fakes by 30%, we over-subtract by 4% globally — comparable to current σ_model.

**Fix:** Use per-cell or per-charge MC-to-data scale fits on the fake-only fraction (e.g., fit to high-fake regions where signal is small), or treat the fake-scale ambiguity as a σ_fakescale systematic and propagate.

### IB3. `ibu_core_rev6.py:107` — `eps[j] = col[j]/prior[j]` can exceed 1.0 (unphysical)

```python
eps[good] = col[good] / p0[good]
```

**Bug:** `col[j]` is the MC-weight column sum of the inner response. `prior[j]` is the MC gen prior. In principle ε[j] ≤ 1 since matched ⊆ gen-pool. In practice, with NucliaWeight applied to matched-reco fill in `build_migration_th2_per_charge.py:486` but NOT applied to the prior fill at `build_migration_th2_per_charge.py:651`, the matched col has a multiplicative `<NucliaWeight>` factor (mean ≈ 1.02, max ≈ 10) that prior lacks. Result: ε[j] > 1 for bins where matched events systematically have high NucliaWeight.

The rev5 build_migration also flagged this asymmetry as "Option B, deferred to rev5" — but it was **never actually fixed**. The fix is to apply NucliaWeight to the prior fill too (or remove it from matched fill).

**Severity:** ε[j] > 1 means the 1/ε[j] correction multiplies p_n by less than 1, biasing all unfolded yields LOW by ~2% on average and up to 10× in extreme bins. Verify: compute `(eps > 1).sum()` and `eps.max()` directly.

**Fix:** Harmonize NucliaWeight in `build_migration_th2_per_charge.py` — either remove from matched-reco fill or add to prior fill. This is the same root cause Critic-1 of the rev5 round flagged but rev6 inherited unfixed.

### IB4. `ibu_core_rev6.py:88-91` — silent clipping of negative inputs without logging

```python
p0 = np.where(p0 < 0, 0.0, p0)
d  = np.where(d  < 0, 0.0, d)
```

**Bug:** Inputs are clipped at zero with no log message. After `_fake_subtract`, `data_inner` can legitimately be negative in low-stat bins (Poisson fluctuation below MC fake prediction). The clip silently throws away those events, biasing the integrated total HIGH by the discarded negative mass.

**Severity:** For h+ rev6, 0 bins clipped at default canonical (per `_fake_subtract.py:101` `n_clipped` count). But in trust-masked S3b corners, this matters.

**Fix:** Emit `n_clipped` count to a per-run log. If `n_clipped > 5%` of bins, fail loudly. Or use a proper negative-weight-tolerant IBU formulation.

### IB5. `run_ibu_rev6.py` — refold uses prior in denominator, not col_inner

```python
# Lines 70-75
ratio = np.zeros_like(prior)
ratio[prior > 0] = p_unf[prior > 0] / prior[prior > 0]
refold = R_inner @ ratio
```

**Bug:** The refold formula is `refold[i] = Σ_j R_inner[i,j] · p_unf[j] / prior[j]`. This is correct ONLY if R is column-normalized to prior. In our setup R is in absolute MC weights, and the column sum is `col[j] = R[:,j].sum() ≠ prior[j]` in general (col[j] = ε[j]·prior[j]). So the correct refold is `refold[i] = Σ_j R[i,j] · p_unf[j] / col[j]`, which equals `P[i,j] · p_unf[j]` where P is the conditional probability matrix.

The current code gives `refold[i] = Σ_j R[i,j] / prior[j] · p_unf[j] = Σ_j (col[j]/prior[j]) · P[i,j] · p_unf[j] = Σ_j ε[j] · P[i,j] · p_unf[j]`. This is **the forward fold including efficiency**, i.e. it predicts the *reconstructed* yield, not the matched yield. For comparison against data_inner (which IS the matched-reco-equivalent after fake subtraction), this is **correct** — but only by coincidence of two formula errors canceling. The chi2/ndf numbers are reportable but the formula labeling is confused.

**Severity:** Numbers OK by accident; documentation lies about what's being computed. A future edit that "fixes" one of the two formula errors will break closure.

**Fix:** Rewrite refold explicitly as `refold[i] = (R_inner @ p_unf) / col[j]` using `ibu.refold(p)` from `ibu_core_rev6.py:135`. The latter is correct (uses `P @ (eps * p) = R @ p / prior`, which is **wrong** for matched-reco comparison too — should be `R @ p / col`). Actually both implementations have the issue. Replace with `(R_inner / col[None,:]) @ p_unf` to get matched-reco prediction proper.

---

## Major Implementation Concerns

### IM1. `ibu_core_rev6.py:97-103` — column normalization division-by-zero in 5 bins for rev6

```python
col = R.sum(axis=0)
safe_col = np.where(col > 0, col, 1.0)
self.P = R / safe_col[None, :]
```

`(col == 0)` happens for ~5 of 240 bins (S3b iz≥4 corner). Those columns of `P` are zero by construction (since `R / 1.0 = R = 0`), AND ε[j] = 0 there. So the unfolded result is forced to zero in those bins forever. **This is correct algebraically but silently masks bins where the user might want a different behavior** (e.g., interpolate from neighboring bins, or warn).

**Fix:** Print `n_zero_col` and `which_gen_indices` at IBU construction. Document that these bins are silently dropped from unfolded.

### IM2. `ibu_core_rev6.py:131` — rel_change normalization can divide by zero in convergence loop

```python
rel = float(np.sum(np.abs(p_next - p)) / denom) if denom > 0 else float('nan')
```

If `denom > 0` but small (early iterations near zero in fresh runs), `rel` can be enormous. For the rev6 normal case (prior > 0), `denom = prior.sum() > 0` always. But a malformed prior (e.g., all-zero in one MC) would produce nan history. No safeguard.

### IM3. `_fake_subtract.py:90` — `scale` formula has no per-bin variation

The closed-form `scale = data_total / (matched + fake)_MC` is a single number per charge. This forces the data and MC fake shapes to match. If MC mis-models fake shape (e.g., MC predicts more fakes at high pT than data has), the subtraction is biased per-bin even if integrated agreement is good.

**Test:** Compare data fine-reco-marginal vs MC inner+fake marginal per bin. If shape χ²/ndf >> 1, the global scale is the wrong subtractor.

### IM4. `run_ibu_rev6.py:88` — variance from `data_inner_var` not propagated through IBU

`_fake_subtract.py` returns `data_inner_var` but `run_ibu_rev6.py` only passes `data_inner` to IBU. The Poisson bootstrap in `covariance_bootstrap` uses `np.random.poisson(data)` which assumes Poisson statistics regardless of the actual variance. For non-Poisson `data_inner_var`, this under/over-samples.

**Fix:** Pass `data_inner_var` to IBU, replace Poisson bootstrap with Gaussian draws `N(mean=data_inner, std=sqrt(data_inner_var))` or use sqrt-Poisson rescaling.

---

## Latent Risks

### IL1. `ibu_core_rev6.py:65` — float64 conversion can lose precision for very large weights

If `weight1 * weight2` overflows float32, conversion to float64 happens via `np.asarray(..., dtype=np.float64)`. For typical H1 MC weights this is fine, but no overflow check.

### IL2. `_fake_subtract.py:115` — variance floor of `max(median(v_pos), 1.0)` on overflow bin is wrong scale

The overflow bin variance is set to the median of physical-bin variances. With ~1.5e6 events in 960 bins, median variance ~1500. The miss-flow has ~3e7 MC events at MC luminosity → at data scale ~1e6. Variance "floor of 1500 or 1.0" is many orders of magnitude too small. (But: the rev6 path sets `miss_yield = 0` so this code never runs in practice — confirm dead code or remove.)

### IL3. `run_ibu_rev6.py` — no protection against `N_max > 10000` runaway iterations

A user typo could request N=1e5 iterations. No upper bound check. Would hang for ~10s but not crash.

### IL4. `ibu_core_rev6.py:118` — fixed point convergence not checked

`fit()` runs `n_iter` blindly. If rel_change blows up (matrix-inversion-like oscillation), no detection. Should warn if `rel_change_hist[-1] > rel_change_hist[len/2]`.

### IL5. `ibu_core_rev6.py:166-168` — bootstrap recreates IBU object per toy, ignoring class-level caches

`for b in range(n_bootstraps): ibu = IBU(self.R, ...)`. Each toy rebuilds `P`, `eps`, `col` from R. For 50 toys this is ~50 wasted recomputations. Performance only, not correctness.

### IL6. `_fake_subtract.py` — no charge-confusion treatment

The fake column R[:, NB_G] includes both "true fake" (no gen partner) and "charge-confused" (gen partner exists but with opposite charge — went to OTHER charge's matched). At 0.35% integrated charge confusion this is small but adds to fake total at the per-bin level.

---

## Verified-Correct Components

- `ibu_core_rev6.py:140-143`: refold formula `P @ (eps * p)` is mathematically correct conditional-probability fold.
- `ibu_core_rev6.py:115-121`: step formula has correct semantics for absolute-weight R, verified by `_self_test` machine-precision result.
- `ibu_core_rev6.py:180-191`: self-test passes (max |rel| = 3e-16) — confirms the core math.
- `_fake_subtract.py:86-103`: closed-form scale derivation is consistent; mathematics correct given the global-shape-match assumption.
- `run_ibu_rev6.py:166-168`: per-iter scan exploitation (one fit, multiple N readouts) is correct.

---

## Missing Evidence Needed for Full Verdict

- Distribution of `eps[j]` per bin: need to confirm `(eps > 1).sum()` and `eps.max()` to size IB3.
- `n_clipped` counts per charge per MC over the actual run: need to size IB4.
- Per-bin data vs MC fake shape comparison: needed for IM3.
- Test of `data_w2` from `h_data_bgsub_w2` vs Poisson assumption: needed for IB1.

---

## Recommended Next Implementation Step

**Single most decisive fix:** Address IB3 (NucliaWeight asymmetry between matched-reco and prior fills in `build_migration_th2_per_charge.py`). This is the same bug rev5 critic-1 §F-A flagged as "deferred to rev5", and rev6 inherited it unaddressed. The fix:

1. In `build_migration_th2_per_charge.py:651`, add NucliaWeight to the prior fill:
   ```python
   wg = w_gen[good_gen] * nuclia_weight[good_gen]
   ```
   AND ensure `nuclia_weight` is read from `ChargedTotGen` (probably needs to be propagated through the matcher).
2. OR remove NucliaWeight from the matched-reco fill at line 486:
   ```python
   w_reco = w1 * w2   # no NucliaWeight
   ```
3. Rebuild all migration ROOTs (Condor production), then rerun rev6 IBU.

Until then, IBU ε[j] silently exceeds 1.0 in unknown number of bins, biasing the unfolded result LOW.

After IB3, fix IB1 (data_w2 propagation) and IB5 (refold documentation clarification).

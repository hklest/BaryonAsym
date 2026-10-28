# Area 5 / Critic A — Unfolding semantics + propagation (implementation-critic)

**Verdict:** **CONCERN** — 3 fatal + 4 major + 2 latent
**Files reviewed:** ibu_core_rev6.py (193 lines), run_ibu_rev6_NWfix.py (171 lines), unfold_tunfold_single_tau.C, _fake_subtract.py interaction

## Fatal bugs

### C5F1. **TUnfold rev5 and IBU rev6 use DIFFERENT fake-handling semantics**

| Method | Fake handling |
|---|---|
| TUnfold rev5 | Uses R_full with fake column R[:NB_R, NB_G]; TUnfold internally subtracts fakes via the encoded fake column. Data overflow = 0 (rev5 dropped F1). |
| IBU rev6 | Uses R_inner only (slices off fake column); fakes pre-subtracted from data via `_fake_subtract.py`. |

**Bug:** When the method comparison plot shows "TUnfold-rev5 3.19e6 vs IBU-rev6 3.20e6 vs BBB 2.37e6", these are computed with DIFFERENT fake-handling. Comparing them directly is misleading because:
- TUnfold's "data_total" = 1.5e6 (raw bg-sub data, including fakes).
- IBU's "data_inner total" = 1.29e6 (post-fake-subtraction).
- Ratio TUnfold/data = 2.13×, IBU/data_inner = 1.94× — close but not equal (because TUnfold also accounts for fakes via the fake column).

When the user looks at "method comparison" and sees TUnfold and IBU within 0.5%, they're comparing apples to oranges. The agreement is COINCIDENTAL given the different fake treatments.

**Severity:** Misleading reported "method agreement". For a publishable comparison, both methods must use the SAME fake handling (either both pre-subtract OR both leave to internal handling).

**Fix:** Either (a) move IBU rev6 to use R_full and skip the explicit fake subtraction, OR (b) update TUnfold rev6 macro to use R_inner only and consume `data_inner` from `_fake_subtract.py`. The latter is the cleaner choice given the rev6 audit's TB1 recommendation to restore kEConstraintArea + use explicit fakes.

### C5F2. `ibu_core_rev6.py:131` — convergence metric `rel = |p_next − p|_1 / |p|_1` is normalized by L1 norm of CURRENT iterate

```python
rel = float(np.sum(np.abs(p_next - p)) / denom) if denom > 0 else float('nan')
```

**Bug:** If `p` shrinks rapidly across iterations (e.g., from prior to converged), the same absolute change appears as a growing rel_change. For our case prior_sum ~ 6.7e7 and converged ~ 6.7e7 (similar scales), this is OK. But for cases where one charge has prior_sum < matched_sum (rare), the convergence diagnostic is unreliable.

**Severity:** Convergence-loop self-check is misleading in edge cases. Doesn't affect unfolded result.

### C5F3. `run_ibu_rev6_NWfix.py:96-103` — bootstrap covariance uses `np.cov(samples, rowvar=False)` which subtracts the BOOTSTRAP mean, not the central-value unfolded result

```python
mean = np.nanmean(samples, axis=0)
std = np.nanstd(samples, axis=0, ddof=1)
cov = np.cov(samples, rowvar=False)
```

(Actually this is `ibu_core_rev6.py:166-181` — `covariance_bootstrap` method.)

**Bug:** `np.cov` computes `E[(X − ⟨X⟩)(Y − ⟨Y⟩)]` where `⟨X⟩, ⟨Y⟩` are bootstrap means. The proper σ_stat is `E[(X − μ)²]` where μ is the central-value unfolded (not bootstrap mean). The two differ when the bootstrap mean has a bias (bootstrap bias is a known property of IBU at finite iterations).

**Severity:** σ_stat is slightly under-estimated (the bootstrap bias absorbs some of the variance). For 50 toys, bootstrap bias is ~1-2% of the value → σ_stat under-estimate ~1-2%.

**Fix:** Compute σ_stat as `sqrt(mean((samples - p_central)², axis=0))` where p_central is the unfolded value from the un-fluctuated fit.

---

## Major concerns

### C5M1. **IBU bootstrap is over Poisson-fluctuated DATA only, ignoring response-matrix MC stat uncertainty**

`ibu_core_rev6.py:170`: `d_toy = rng.poisson(d_clip).astype(np.float64)`. Response and prior held fixed across toys. **No propagation of MC stat uncertainty on R.**

**Concern:** The MC stat uncertainty on R[i,j] is O(1/sqrt(N_MC_per_bin)). For NB_G = 240 bins with ~150k matched MC each, σ_MC_stat per cell is ~0.25%. Propagated through IBU, this adds ~1-3% per published bin (typical for HERA SIDIS unfolding).

**Severity:** σ_MC_stat is a SEPARATE source from σ_stat (data Poisson). Missing from current budget.

**Fix:** Two-tier bootstrap: outer loop over R (Poisson on each cell using MC counts), inner loop over data. Quote σ_MC_stat separately.

### C5M2. `_fake_subtract.py:114-118` — variance treatment of `var_fake_scale = scale² × fake_marginal_MC`

This is the variance of `fake_pred = scale × fake_marginal_MC` assuming `scale` is fixed. **But `scale = data_total / (matched + fake)_MC` is itself a derived quantity that depends on data_total.** Its variance propagates through the chain.

**Severity:** Missing covariance between data_total and fake_pred. Per-bin σ_stat under-estimated by ~5-10%.

**Fix:** Add `cov(data, fake_pred)` term in the variance formula.

### C5M3. **Trust mask is hard-coded; no test for trust-mask boundary stability**

Trust mask: `cnt_MC < 50 OR (cnt_MC < 500 AND reco_eff < 0.05)`. The 50 and 500 thresholds are documented but never varied to check stability of headline σ.

**Fix:** Vary trust thresholds (e.g., 50→100, 500→1000) and check headline σ shifts. If headlines move by >1%, the trust choice is part of the systematic budget.

### C5M4. `unfold_tunfold_single_tau.C:540` — `h_zsigma_gt10_pub > 0` gate triggers if ANY single bin has |unf/σ| > 10. With trust mask, untrusted bins should be excluded BEFORE this gate.

**Severity:** Cosmetic, the gate is informational only.

---

## Latent risks

### C5L1. `ibu_core_rev6.py:75-87` — `prior` and `data` clipped silently to ≥0 at construction. No log message. Same issue as IB4 in rev6 audit, still not fixed.

### C5L2. IBU bootstrap uses `np.random.default_rng(seed)`. Bootstrap seeds are not documented or versioned. Reproducibility hazard for any per-bin sigma published.

---

## Recommended next implementation check

**Most decisive:** Address **C5F1 (fake-handling semantics mismatch)**. Either move IBU to R_full handling OR move TUnfold rev6 to R_inner handling. Without this fix, the "method comparison" tables are not apples-to-apples and the integrated agreement claims are coincidental.

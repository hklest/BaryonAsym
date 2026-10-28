# Area 4 / Critic A — Weights + normalization (implementation-critic)

**Verdict:** **CONCERN** — 3 fatal + 3 major + 2 latent
**Files reviewed:** _fake_subtract.py (154 lines), build_migration NW handling, merge

## Fatal bugs

### C4F1. `_fake_subtract.py:88` — closed-form `scale = data_total / (matched_inner_total + fake_total)` ignores `data_w2`

`data_inner = data_fine − scale × fake_marginal_MC`. The `data_fine` carries per-bin variance via `h_data_bgsub_<chg>_w2`. The fake_subtract utility receives only the values, ignores the w2 histogram.

**Bug:** The σ_stat propagation through fake subtraction assumes Poisson on `data_fine`. For bg-subtracted data, the actual variance is `data_w2 ≠ data_fine` (because upstream bg-sub introduces correlated additional variance). Using Poisson under-counts σ_stat by 10-30% per bin in regions with significant bg subtraction.

**Severity:** σ_stat headline underestimated by 10-30%. **Affects every downstream uncertainty in the entire analysis** — IBU bootstrap, TUnfold σ_stat, BBB σ_stat.

**Fix:** Modify `fake_subtract_data()` to accept `data_w2` and use it as variance instead of Poisson on `data_fine`. Wire through from `run_ibu_rev6_NWfix.py` and `compute_bbb_rev6_NWfix.py`.

### C4F2. `_fake_subtract.py:104-107` — silent clipping of negative `data_inner` to 0

```python
n_clipped = int((data_inner < 0).sum())
if n_clipped:
    data_inner = np.where(data_inner < 0, 0.0, data_inner)
```

**Bug:** Same issue as flagged in IB4 of the previous rev6 audit, persists in NWfix. `n_clipped` is computed but never logged or returned in `diag`. The downstream consumer (run_ibu, compute_bbb) doesn't know how many bins were clipped.

**Severity:** For h+ rev6 NWfix at canonical N=100, smoke test showed 0 clipped bins. But at low-stat bins after multiple iterations, can grow.

**Fix:** Add `n_clipped` to `diagnostics` dict; raise if `n_clipped / NB_R > 0.05`.

### C4F3. `build_migration_th2_per_charge_NWfix.py:482-483` — NucliaWeight clip `np.clip(wn, 0.0, NUCLIA_CLIP)` clips negatives to 0

```python
wn = np.where(np.isfinite(wn), wn, 1.0)
wn = np.clip(wn, 0.0, NUCLIA_CLIP)
```

**Bug:** NucliaWeight on real data has `min = −2.58` (verified earlier). These negative weights are clipped to 0 → the event has `w_reco = w1·w2·0 = 0` → contributes nothing to col_inner AND nothing to matched_marginal. BUT: it still contributes 1 to `n_matched` counter (line 687: `acc.n_matched += 1` regardless of wr value).

Now in the prior_NWeff fill (step 8, line 766): `consumed_keys[key] = _nw_partner = wr / w12 = 0 / w12 = 0`. So the gen partner gets `w_eff = w12 * 0 = 0` in prior_NWeff. Consistent — both col and prior_NWeff are 0 for this event.

In the OLD prior fill (step 5, kept for audit): w_gen = w1·w2 → the gen partner contributes `w12` to prior (unweighted). col/prior_unweighted for this event: `0 / w12 = 0` → ε contribution is 0. **Correct.**

**Net:** clipped-NW events are silently dropped from BOTH numerator and denominator of ε[j]_NWeff. They DO appear in the prior_unweighted (legacy) histogram. **Asymmetric: prior_unweighted has them, prior_NWeff doesn't.** This is the CORRECT physics behavior for NWeff (matches the col_inner scale) but creates confusion if any consumer uses the legacy `h_prior_gen_<chg>` instead of `h_prior_gen_<chg>_NWeff`.

**Severity:** Risk of accidentally using the wrong prior. Confirmed: per_bin_sigma_rev6 originally read `h_prior_gen_<chg>_<mc>` (unweighted) — fixed via _NWfix variants. Any new consumer must read `_NWeff`.

**Fix:** Deprecate the unweighted prior histograms after NWeff transition is complete, OR add asserts in IBU/BBB drivers to require `_NWeff` suffix.

---

## Major concerns

### C4M1. `_fake_subtract.py:90` — scale formula assumes data is at SIDIS-specific data luminosity 356,613 nb⁻¹

The scale `data_total / (matched + fake)_MC` is dimensionless: it converts MC weights (no explicit lumi) to the data scale. **Implicit assumption**: the MC weights `w1*w2` are such that integrated MC at the data luminosity equals what's predicted.

**Concern:** MC luminosity is NOT explicitly in `weight1 * weight2`. Per H1OO convention, `weight1 * weight2` gives the MC-event weight at unit luminosity; the data luminosity multiplier is applied downstream (typically in cross-section formulas). For IBU rev6 the closed-form scale FORCES the data/MC ratio to match without needing the explicit lumi. **This works.**

But: if a downstream cross-section formula re-applies the lumi factor (assuming MC is at MC lumi), the IBU unfolded would be DOUBLE-corrected. This is exactly the pre-rev3 normalization confusion.

**Fix:** Explicit assertion at the top of any cross-section computation: "unfolded yield is at data luminosity 356,613 nb⁻¹; do NOT re-divide by lumi when forming σ".

### C4M2. `merge_migration_per_charge_NWfix.py` — Rapgap and Djangoh files merged with EQUAL WEIGHT into nominal aliases

Lines 297-298:
```python
prior_p_dj, prior_p_dj_w2 = _sum_TH1(dj_files, "h_prior_gen_pos", NB_G)
```

The "nominal" alias is just `prior_<chg>_rapgap`. Djangoh is treated as alternative. **No weighted combination — Rapgap-only is the canonical truth model.**

**Concern:** If RG and DJ disagree on the gen prior shape (they do — σ_model 7% trusted-median), the choice of Rapgap-only as nominal introduces a hidden model dependence. Should this be a 50/50 average?

**Severity:** Model dependence is what σ_model captures. The choice of Rapgap as nominal is conservative (matches H1 convention historically). Documented choice — not a bug.

### C4M3. `_fake_subtract.py:90` — closed-form scale is identical for all reco bins

Per the rev6 audit IB2: "global per-charge scale assumes data and MC fake fractions match globally, not per-bin". If MC underpredicts fakes in bin X but overpredicts in bin Y, the global scale gets the integrated total right but the per-bin subtraction is wrong.

**Severity:** Per-bin fake-shape ambiguity, not captured by current σ_total. Need σ_fakescale per bin.

---

## Latent risks

### C4L1. `_fake_subtract.py:121-125` — `diag` dict doesn't include the n_clipped counter (see C4F2). Missing observability.

### C4L2. NWeff prior_pos_djangoh is read from djangoh files, not rapgap. If a future build cycle generates ONLY rapgap (e.g., partial Condor run), `prior_pos_djangoh_NWeff` would be missing → merge crashes. Add fallback.

---

## Recommended next implementation check

**Most decisive:** Fix **C4F1 (data_w2 propagation)**. This affects every downstream σ_stat number. Plumb `data_w2` through fake_subtract → IBU → BBB → σ tables. Re-run NWfix σ decomposition and quantify the σ_stat shift.

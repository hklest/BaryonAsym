# Critic Report: h1-analysis-critic on rev5 TUnfold + IBU + BBB

**Date:** 2026-05-31 (post-rev5)
**Verdict:** **FATAL**
**Material reviewed:** rev5 source code under `H1ChargedSidis_HighQ2/scripts/{unfolding/tunfold_official_v3_percharge,ibu,analysis}/`
  - `binning_v3.py`, `merge_migration_per_charge.py`, `unfold_tunfold_single_tau.C`
  - `ibu/ibu_core.py`, `ibu/run_ibu.py`
  - `analysis/compute_bbb_and_compare.py`
  - Numerical headlines from prior session (TUnfold pos unfolded 3.19e6; IBU pos 1.47e6; BBB pos 2.77e6; expected 2.71e6).

---

## Fatal Issues

### F1. BBB formula is wrong (`compute_bbb_and_compare.py:32-71`).

**Claim criticized:** "BBB[j] = data_at_gen[j] · (prior_MC[j] / matched_at_gen[j])", with `matched_at_gen[j]` constructed by aggregating the **matched_reco_marginal** (length 960) via `binmap_reco_to_gen` into 240 cells (lines 53-58).

**Evidence:**
```python
matched_fine = f[f'h_matched_reco_marginal_{charge}_{mc}'].values()[:NB_R]  # length 960
for r in range(NB_R):
    j = binmap[r]                                                # reco→gen aggregation
    if 0 <= j < NB_G:
        matched_at_gen[j] += matched_fine[r]
...
ratio = np.where(matched_at_gen > 0, prior / matched_at_gen, 0.0)
bbb = data_at_gen * ratio
```

`matched_reco_marginal[r]` is the sum over **reco bin r** of MC weights for matched reco-gen pairs whose reco lands in bin r. Aggregating to a published bin j via `binmap_reco_to_gen` does NOT give the matched count of events whose **gen** lands in j; it gives the matched count of events whose **reco** lands in the 4 fine children of j. These two are NOT equal: a matched event in cell (g, r) with reco bin in fine-children-of-j contributes to `matched_at_gen[j]` here, but the corresponding gen bin may be a different published bin g.

**Physical mechanism of bias:** The correct BBB factor is the per-gen-bin acceptance × purity ratio:
- C_BBB[j] = (truth in j with reco-in-fid) / (reco-in-fid coming from gen-in-j)
- The standard formulation: C_BBB[j] = N_gen_MC[j] / N_reco_MC[j_paired], where `N_reco_MC[j_paired]` is the column-sum of the response matrix R restricted to gen bin j and inner-reco rows.

What `compute_bbb_and_compare.py` computes is approximately right WHEN the response is near-diagonal (small migration) AND when the per-cell aggregation of reco-marginal closely tracks the per-gen-bin matched count. Under the inverted 4-to-1 layout (and Schmitt 2012 §2 which warns this assumption is only true with negligible migration), the implementation systematically biases BBB whenever migration is non-trivial. This explains the suspicious closeness of BBB to the naive expectation: it is partially the right answer for the wrong reason.

**Severity:** Analysis-killing. BBB is currently the "clean" cross-check whose 2.2% integrated agreement with the naive expectation is used to validate TUnfold. That validation is invalid until the BBB formula is corrected to use the response-matrix column sums (R[:NB_R, j].sum() per gen j) for the denominator.

### F2. IBU acceptance correction is structurally broken at rev5 (`ibu_core.py:95-106`, interplay with `merge_migration_per_charge.py:407` and `run_ibu.py:114`).

**Claim criticized:** "ε[j] = column_sum[j] / prior[j]" where `col = R.sum(axis=0)` includes the miss row at reco index NB_R.

**Evidence:**
- `run_ibu.py:114`: `R = Rfull[:, :NB_G].copy()` — keeps the miss row (full 961 rows × 240 gen cols), drops only the fake column.
- `ibu_core.py:100-103`: `col = R.sum(axis=0); eps[good] = col[good] / p0[good]`.
- `merge_migration_per_charge.py:407`: `miss_yield = 0.0` at rev5 → data[NB_R] = 0.
- In `ibu_core.step()` line 130: `denom = R @ p_n` produces a 961-vector; line 133: `weight = data / safe_denom` gives weight[NB_R] = 0/denom[NB_R] = 0. So the miss row contributes ZERO to the iteration update.
- Line 137: `update = p_n * (R.T @ weight)` — miss row row in R.T multiplies weight[NB_R] = 0 → no contribution.
- Line 141: `p_next[m] = update[m] / eps[m]` — but eps[j] includes the MC miss-row weight in its numerator (col[j] = matched_col[j] + miss_row[j]).

**Net effect:** The update numerator `R.T @ weight` only includes matched-row contributions (because weight[NB_R] = 0), but the denominator eps[j] is inflated by the MC's miss-row weight. The iteration therefore systematically scales DOWN p_n by a factor of (matched_col[j] / col[j]) = reco_eff_per_gen_bin[j] every iteration. At fixed point, this drives the unfolded total to equal data_phys (since the 1/eps multiplicative correction is fully neutralized by the missing miss-row data weight).

**Reproducible signature:** IBU integrated unfolded = data_phys = 1.47e6 for both Rapgap and Djangoh responses, exactly as observed. The agreement across MC responses is a tell that IBU has become a trivial identity transformation, not a real unfolding.

**Severity:** Analysis-killing. IBU at rev5 is not unfolding — it is reproducing data_phys at the gen pool with bin-by-bin geometric reshuffling, biased by the prior shape. All IBU rev5 numbers in the per-bin σ_model comparison are meaningless until fixed.

**Recommended fix:** Either
- (a) Construct the IBU response with the miss row excluded AND replace ε[j] with col_inner[j] / prior[j] (mathematically equivalent to the proper "fraction matrix" IBU with acceptance correction). The miss row is structurally not part of the IBU forward operator unless the data side also carries the miss flow.
- (b) Restore the rev4 F1 area constraint exclusively in the IBU data-loading path: replace data[NB_R] with `miss_row_MC.sum() · (data_phys / matched_total_MC)`. This is the rev4 algebra and is consistent with the standard IBU acceptance treatment.

### F3. Uncertainty model treats regularized TUnfold output as bin-independent.

**Claim criticized:** `σ_stat ⊕ σ_reg ⊕ σ_model in quadrature per bin` is reported as the final uncertainty, with no covariance retained between published gen bins.

**Evidence:** Rev5 TUnfold writes `h_emat_pub` (240×240 covariance) and `h_rho_pub`, with measured `mean_abs_rho_pub_offdiag` and `max_abs_rho_pub_offdiag` per τ. The single-tau macro at lines 1064-1070 stores the full cov, but the headline σ in the per-bin systematics report uses only `sigma_pub[r] = sqrt(cov_pub(r,r))`.

**Physical mechanism of bias:** Tikhonov regularization at τ = 1.44e-6 introduces bin-to-bin anti-correlations (negative off-diagonals on the curvature-smoothed neighbors) that, when integrated over published bins, can either enhance or partially cancel the diagonal sum depending on the projection. Any downstream integrated cross-section, asymmetry, or projection-marginalization that uses these per-bin σ in quadrature is wrong by a covariance-induced factor — typically 30-50% of the diagonal contribution for moderate regularization.

**Severity:** Analysis-killing for any downstream integrated observable. Diagonal-only error bars on per-bin plots are acceptable for visualization, but the final published table must propagate the 240×240 covariance.

---

## Major Concerns

### M1. Charge-confusion systematic missing from per-charge budget.

If per-charge unfolding is the architectural choice (justified by 0.35% integrated charge confusion being "DOCUMENT-band"), then the charge confusion that WOULD have been corrected by a joint pos+neg unfolding is now an unaccounted systematic. The σ budget contains σ_stat, σ_reg, σ_model. There is no σ_chargeconf. For a 0.35% migration, the effect on per-bin asymmetry A_h = (N+ − N−)/(N+ + N−) at typical bins is up to ~2×0.35% = 0.7% per published bin, larger in low-statistics corners. Add to budget or document why omitted.

### M2. `area_residual_rel` term in τ-selection score still active despite `kEConstraintNone`.

`unfold_tunfold_single_tau.C:959-993` still computes and adds `1.0 * area_residual_rel` to `score_tau` unless `hard_reject`. With rev5 `kEConstraintNone` and `miss_yield_overflow = 0`, `dtot_with_overflow = dtot = 1.47e6`, but `tot_unf_pub ≈ 3.19e6` (the natural total when constraint is dropped). So `area_residual_rel ≈ |3.19−1.47|/1.47 ≈ 1.17` for every τ, contributing a roughly constant +1.17 penalty to every score. Argmin discrimination is preserved across τ (the term is τ-independent at this precision) BUT:
- The composite metric values reported in `tau_selection_metrics.json` are dominated by this constant, distorting the score-level interpretation.
- The user-facing "score = 1.17 + ..." may be misread as 17% deviation when it is actually structural (data doesn't have the miss flow, by design).

**Recommended fix:** Disable the `area_residual_rel` term entirely when `inverted_layout && constraint_mode == kEConstraintNone`, OR compute area_residual_rel against `dtot / reco_eff_MC = 2.71e6` (the natural area target absent the constraint).

### M3. Trust mask `cnt_rap < 50` is asymmetric in MC samples.

The trust mask uses only `h_mc_count_pos` / `h_mc_count_neg` (Rapgap counts) at `merge_migration_per_charge.py:549-550`. Djangoh-only bins or bins where Rapgap underpopulates but Djangoh has support are still untrusted, then σ_model is computed from |RG-DJ|. This double-penalizes regions where Rapgap is anomalously sparse, and prevents recovery via the Djangoh response. Add Djangoh-count-based trust criterion.

### M4. Background subtraction Q²gen < 60 gap.

Rev5 carries forward MC-based subtraction of Q²gen < 60 GeV². But the gap 60 < Q²gen < 150 (between the low-Q² subtraction threshold and the high-Q² fiducial lower edge) is NOT addressed in the rev5 evidence supplied. If MC events with 60 < Q²gen < 150 reconstruct into the Q²reco > 150 fiducial, they are spillover into the analysis sample but neither subtracted nor properly handled in the response matrix (since gen-out-of-fiducial events show up as miss flow in the response, but the response is built only from MC truth-matched gen in fiducial). Clarify.

### M5. Closure pseudo-data uses Rapgap prior also for Djangoh response (`unfold_tunfold_single_tau.C:228-231`).

In mode `dataDJ` the prior histogram name is set to `h_prior_gen_<chg>_djangoh`. Correct. In mode `data` the prior is `h_prior_gen_<chg>` which aliases Rapgap (`merge_migration_per_charge.py:483-486`). When the nominal (Rapgap) unfolding is run, the prior used as TUnfold's `bias_scale = 1.0` reference is Rapgap, AND the model-uncertainty Djangoh unfolding uses Djangoh prior. This asymmetry is fine, BUT the σ_model = |N_RG − N_DJ|/N_RG conflates two effects: (i) different response matrices, (ii) different priors. The "pure response model uncertainty" requires either fixing the prior across both unfoldings or showing that the prior contribution is small via a side study.

---

## Non-Blocking Checks Confirmed

- The `binning_v3.py` reco/gen layouts are consistent and the 4-to-1 aggregation is correctly encoded (`build_binmap_reco_to_gen` lines 273-284 verify uniform 4-fold coverage).
- F1 area constraint dropping at rev5 is documented in `merge_migration_per_charge.py:394-427` with diagnostic prints retained, satisfying the audit trail.
- Custom L matrix (304 triplets, intra-cell pT and z only) is correctly constructed at `unfold_tunfold_single_tau.C:447-474` and matches the no-cross-cell-smoothing intent.
- Per-charge processing keeps pos and neg fully independent via separate histograms and the `chg` argument throughout — no charge-mixing bug observed.

---

## Missing Evidence

- The full 240×240 covariance matrix `h_emat_pub` at the canonical τ = 1.44e-6: no per-pair correlation summary table was supplied, only mean and max. Needed for M3 propagation cross-check.
- The actual numeric content of `h_mc_count_pos` and `h_mc_count_neg` per published bin: needed to verify F1 BBB fix when implemented (i.e., compare matched-column-sum vs marginal-aggregation per bin).
- Q²gen ∈ [60, 150] event count in MC after fiducial cut: needed to size M4.

---

## Recommended Next Check

**Single most decisive follow-up:** Compute and tabulate, for each published gen bin j, the three quantities
1. `matched_via_marginal[j]` = sum over fine reco children of (matched_reco_marginal[r])
2. `matched_via_column[j]`   = R[:NB_R, j].sum()
3. `gen_pool[j]`              = prior_MC[j]

Then plot (1)/(2) per bin. If the ratio differs from 1.0 by more than ~10% in any retained bin, F1 is confirmed and the corrected BBB MUST replace the current implementation before any downstream comparison is trusted. The bins where this ratio deviates most are the bins where the response is non-diagonal — exactly the bins where BBB should not be trusted as a method, but the current code masks that fact by producing artificially clean ratios.

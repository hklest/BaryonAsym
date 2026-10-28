# Critic Report: implementation-critic on rev5 unfolding pipeline

**Date:** 2026-05-31 (post-rev5)
**Scope:** File:line audit of the implementation of the rev5 design intent.

---

## Fatal Bugs

### B1. `compute_bbb_and_compare.py:53-60` — wrong matched denominator for BBB.

**Code:**
```python
matched_fine = f[f'h_matched_reco_marginal_{charge}_{mc}'].values()[:NB_R]
matched_at_gen = np.zeros(NB_G, dtype=np.float64)
for r in range(NB_R):
    j = binmap[r]
    if 0 <= j < NB_G:
        matched_at_gen[j] += matched_fine[r]
ratio = np.where(matched_at_gen > 0, prior / matched_at_gen, 0.0)
```

**Bug:** `matched_reco_marginal[r]` is the **reco-side marginal** of the response matrix: sum over j of R[r, j]. Aggregating it via `binmap_reco_to_gen` (which maps reco bin r to its corresponding published cell) does NOT produce the **per-gen-bin matched count** that BBB needs. The correct denominator is the per-gen-bin column sum of the matched response: `R[:NB_R, j].sum()` for each j.

Concretely, the marginal `matched_fine[r] = Σ_j R[r, j]` includes events from MULTIPLE gen bins j (whenever the reco distribution at r came from gen bins outside the parent cell of r). Aggregating these to `matched_at_gen[j]` via `binmap[r]==j` therefore counts events whose gen-bin is NOT j when migration crosses cell boundaries.

**Fix:** Replace the matched-marginal loading with a column-sum read of the response TH2:
```python
R_th2 = f[f'h_R_{charge}_{mc}'].values()  # (961, 241)
matched_per_gen = R_th2[:NB_R, :NB_G].sum(axis=0)  # length 240
```

### B2. `ibu_core.py:100-103` and `run_ibu.py:114` together produce a degenerate IBU at rev5.

**Code:**
```python
# run_ibu.py:114
R = Rfull[:, :NB_G].copy()    # shape (961, 240) — KEEPS miss row
# ibu_core.py:100-103
col = R.sum(axis=0)            # includes miss row contributions
eps[good] = col[good] / p0[good]
```

**Bug:** At rev5 `data[NB_R] = 0` (set in `merge_migration_per_charge.py:407`). In the IBU iteration step (`ibu_core.py:128-141`):
```python
denom = self.R @ p_n             # length 961
weight = self.data / safe_denom  # weight[NB_R] = 0 / something = 0
update = p_n * (self.R.T @ weight)  # miss-row terms contribute 0 (weight=0)
p_next[m] = update[m] / self.eps[m]
```
The numerator `update[j]` only receives contributions from inner reco rows (because weight[NB_R] = 0 kills the miss-row contribution), but the denominator `eps[j] = (matched_col[j] + miss_row[j]) / prior[j]` is inflated by the MC miss-row weight. Net: every iteration multiplies p_n[j] by `(matched_col[j] · weight_sum_over_matched) / (matched_col[j] + miss_row[j])`. At fixed point this drives integrated unfolded to data_phys, killing the acceptance correction.

**Fix (option A, recommended):** In `run_ibu.py:114`, slice the miss row OUT for IBU:
```python
R = Rfull[:NB_R, :NB_G].copy()   # shape (960, 240) — inner matched only
# and slice data:
data = data[:NB_R]                # length 960, no overflow
```
Then `ibu_core.eps[j] = R.sum(axis=0)[j] / prior[j]` correctly equals reco_eff_per_gen_bin from inner matched only, and IBU's 1/ε correction works as intended.

**Fix (option B, alternative):** Restore the rev4 F1 overflow inside IBU's data input only (do NOT change merge), e.g. set `data[NB_R] = R[NB_R, :].sum() · (data[:NB_R].sum() / R[:NB_R, :].sum())` inside `run_ibu.py` after loading.

Option A is cleaner and makes IBU mathematically the standard D'Agostini construction.

---

## Major Implementation Concerns

### C1. Histogram name `h_binmap_gen_to_reco` is misleading (`merge_migration_per_charge.py:540`).

The histogram has length NB_R (960) and values in [0, NB_G) — it is the **reco-to-gen** map, not gen-to-reco. The name is kept for back-compat with the TUnfold macro (which reads `h_binmap_gen_to_reco` at `unfold_tunfold_single_tau.C:235`), but the macro adapts via `inverted_layout` detection at line 254. Three risks:
- Any new consumer reading `h_binmap_gen_to_reco` and assuming gen→reco semantics will silently produce wrong results.
- The merge writes a comment at line 537-539 explaining the inversion, but the histogram name itself is the documentation, not the comment.
- `compute_bbb_and_compare.py:39` already correctly reads it as reco→gen, but only because the author knew. New code is at risk.

**Fix:** Write the histogram under TWO names, `h_binmap_gen_to_reco` (legacy alias) and `h_binmap_reco_to_gen` (correct name), and migrate the macro to read the correct name. Deprecate the alias after migration.

### C2. `merge_migration_per_charge.py:587, 600` — version string is stale relative to logic.

The `meta_f1_formula_version` is set to `"rev5_kEConstraintNone_no_overflow"`, but the F1 OVERFLOW CALCULATION (variables `matched_total`, `miss_row_mc`, `prior_total`, `reco_eff`) is still computed in `_extend_data_fine` (lines 401-405) even though `miss_yield = 0.0` is hard-coded at line 407. The diagnostic prints (lines 415-426) still mention F1 as if it were active. This invites accidental re-enabling of F1 in a future edit. Either delete the dead F1 computation, or wrap it in `if F1_ENABLED:` and add a feature-flag at the top of the file.

### C3. `ibu_core.py:115-143` — vectorized step does not protect against floating-point underflow at deeply-nested zero migration.

When prior[j] = 0 AND col[j] > 0 (a gen bin with no MC prior but has MC matched-reco — should not happen in practice, but possible if `prior` and `R` come from different MC selections), `eps[j] = 0` per `ibu_core.py:101`. Then `p_next[m]` mask at line 141 silently drops the bin. Add explicit logging:
```python
if (~m).any():
    print(f"[IBU] masked {(~m).sum()} bins with eps=0 (no MC support)")
```
At S3b iz≥4 corners with reco_eff = 0%, this is exactly the failure mode — the 10-15 bins where rev5 trust mask says "don't trust" are precisely where IBU silently drops them.

### C4. `unfold_tunfold_single_tau.C:959-993` — score-function area_residual_rel is dead-weight at rev5.

```cpp
double miss_yield_overflow = h_data->GetBinContent(NB_R_EXT);  // = 0 at rev5
double dtot_with_overflow  = dtot + miss_yield_overflow;       // = dtot
double area_residual_rel   = std::fabs(tot_unf_pub - dtot_with_overflow)
                           / std::max(dtot_with_overflow, 1.0);
...
score_tau = ... + 1.0 * area_residual_rel;
```

With rev5 `kEConstraintNone`, `tot_unf_pub ≠ dtot` is BY DESIGN (the natural total is data/reco_eff ≈ 2.71e6, not data = 1.47e6). The score therefore receives a roughly constant +1.17 penalty for every τ. Argmin selection is preserved (the penalty is τ-independent at fixed unfolded total), but the reported score values become uninterpretable. Disable when `kEConstraintNone` is active, or replace `dtot_with_overflow` with `dtot / reco_eff_MC`.

### C5. `unfold_tunfold_single_tau.C:447` — `inverted_layout && NB_G == NB_PUB` gate excludes legitimate inverted layouts where NB_G < NB_PUB.

The check excludes layouts where a future binning has a coarser publication axis than the gen-axis output. Not a current bug (since NB_G == NB_PUB = 240 at rev5), but a latent restriction. Document the assumption or generalize.

### C6. `run_ibu.py:96-97` — mc_tag logic for prior selection is inconsistent.

```python
h_prior_name = f"h_prior_gen_{charge}"
if mc_tag not in ("nominal",):
    h_prior_name = f"h_prior_gen_{charge}_{mc_tag}"
```

The nominal alias is `h_prior_gen_{charge}` (no `_rapgap` suffix), set by `merge_migration_per_charge.py:483-486`. When mc_tag = "rapgap" (not "nominal"), the code looks for `h_prior_gen_pos_rapgap` which DOES exist (line 475). When mc_tag = "djangoh", looks for `h_prior_gen_pos_djangoh` which DOES exist. OK in practice, but the conditional is fragile if "nominal" is renamed. Use explicit alias map.

---

## Latent Risks

### R1. `compute_bbb_and_compare.py:39` reads binmap from a TH1I without checking its size matches NB_R.

```python
binmap = f['h_binmap_gen_to_reco'].values().astype(int)
assert binmap.size == NB_R, f"binmap size {binmap.size} != NB_R {NB_R}"
```

The assert exists — good. But if a future merge writes binmap with length NB_G (the legacy semantics), this would fail loudly. Cross-link the assertion text to the `h_binmap_reco_to_gen` rename in C1.

### R2. `ibu_core.py:225-250` — bootstrap covariance uses `np.cov(samples, rowvar=False)` which subtracts the bootstrap mean, not the central-value unfolded result.

For unbiased σ_stat that's correct. But if downstream code reads `cov` and expects MSE around the central value (`unfolded_pub`), there will be a bias term proportional to `(mean - unfolded_pub)^2`. Document the convention.

### R3. `merge_migration_per_charge.py:217` — uniform-spread of data 240→960 assumes flat distribution within each parent cell.

```python
n_fine[fine_idx] = n_coarse[parent] / float(N_RECO_PER_GEN)
```

For SIDIS multiplicities, z and pT distributions are not flat within the rev5 published bin widths (z 0.0→0.1 spans an order of magnitude in cross-section; pT 0→0.5 has a sharp peak). The uniform spread biases the data input by up to ~30% within a parent cell. The docstring documents this as a FALLBACK and asks for proper re-extraction. Re-extraction from per-track CSV should be next priority after F1 BBB and F2 IBU fixes.

### R4. `unfold_tunfold_single_tau.C:481` — legacy non-inverted-layout fallback uses global curvature, not the custom L.

```cpp
unfold.RegularizeBins(0, 1, NB_G, TUnfold::kRegModeCurvature);
```

If anyone ever reverts to a non-inverted layout for cross-checks, the regularization is unintentionally global and cross-cell. Either remove the fallback or wire the custom L for both layouts.

### R5. Trust mask at `(cnt_rap<500 AND reco_eff<0.05)` is sample-shape-dependent.

S3b iz≥4 bins have cnt_rap < 500 due to RAPGAP's specific z spectrum. DJANGOH samples might populate those bins more (different fragmentation tune). Trust mask should consider max(cnt_rap, cnt_dj) or require both to be sparse. Cross-link to C1 of critic-1.

---

## Verified-Correct Components

- **`binning_v3.py`** — geometric consistency verified by the `_self_test` (lines 329-419) including a 1000-bin random-sample geometric-center cross-check. Reco/gen index conventions match across all consumers. PASS.
- **Custom L matrix triplet construction** (`unfold_tunfold_single_tau.C:447-482`) — 144 + 160 = 304 triplets as designed, no cross-cell smoothing, computed at runtime from the cell × z × pT structure. The triplets are unique by construction (pT triplets vary `ipt`, z triplets vary `iz`, both fix the orthogonal direction; no triplet is registered twice). PASS.
- **Closure overflow** (`unfold_tunfold_single_tau.C:286-340`) — `closure_overflow = prior_total_pdata - matched_total_pdata` is the correct MC miss-flow expression in self-closure AND cross-closure modes. PASS.
- **Tolerance-based negative-eigenvalue gate** (`unfold_tunfold_single_tau.C:971-983`) — `numerical_neg_threshold = -1e-10 * |λ_max|` is a reasonable numerical tolerance and matches standard practice for TMatrixDSymEigen. PASS.
- **Per-charge separation** — no cross-charge histogram reads, no shared output buffers. PASS.

---

## Missing Evidence Needed for Full Verdict

- The `wrap_ibu_rev5_mctag.sh` script content (not pulled in this audit). The wrapper passes mc_tag through to `run_ibu.py`. Need to confirm the wrapper does not override the slicing in B2.
- The `submit_ibu_rev5.condor` content. Need to confirm `--ibu-rapgap-dir` and `--ibu-djangoh-dir` are pointed at the correct per-MC IBU outputs (i.e., `out_dir` differs per MC).
- The `per_bin_systematics_at_tau.py` content (referenced in handoff, not pulled). Need to confirm σ_stat propagation for BBB and σ_model convention for IBU when IBU is degenerate.

---

## Recommended Next Implementation Step

**Single most decisive fix:** Implement Option A of B2 in `run_ibu.py`:
1. Change line 114 from `R = Rfull[:, :NB_G].copy()` to `R = Rfull[:NB_R, :NB_G].copy()` (slice miss row OUT).
2. Change line 123-126 to slice data: `data = data[:NB_R]` so the data vector is length 960.
3. Re-run IBU × 4 (pos/neg × rapgap/djangoh).
4. Confirm IBU integrated total now equals `data_phys / mean_reco_eff` ≈ 2.71e6 (pos), NOT 1.47e6.
5. Re-run `compute_bbb_and_compare.py` AFTER fixing B1 (BBB column-sum denominator).

Without this fix, IBU and BBB rev5 results are both meaningless and the three-method comparison is uninformative.

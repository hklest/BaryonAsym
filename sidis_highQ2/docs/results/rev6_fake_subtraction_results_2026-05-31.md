# Rev6: Fake-subtraction + acceptance-correction results

> 📍 **Entry-point docs:** [RUN_STATUS.md](../RUN_STATUS.md) (what's current) · [LESSONS_LEARNED.md](../LESSONS_LEARNED.md) (structural insights) · [rev6_audit/SYNTHESIS.md](../critiques/rev6_audit_2026-05-31/SYNTHESIS.md) (open issues).

---


**Date:** 2026-05-31 (post-rev5, post-3-critics)
**Directive:** User 2026-05-31: "Rec tracks without a gen partner are background and should be subtracted. Gen tracks without a rec match should be corrected for as acceptance loss. We shouldn't just drop unmatched particles."

## Pipeline change

The merged ROOT `migration_inputs_v3_percharge.root` already carries a 4-cell response matrix per (charge, MC):
- **Inner matched** R[:NB_R, :NB_G]: 3.66e7 weighted MC entries (h+ Rapgap). Gen-in-fid AND reco-in-fid AND matched.
- **Fake column** R[:NB_R, NB_G]: 5.32e6 = **background** = reco-in-fid with no matched gen-in-fid. **12.7%** of (matched_inner + fake_total).
- **Miss row** R[NB_R, :NB_G]: 3.07e7 = **acceptance loss** = gen-in-fid with no matched reco-in-fid. **45.6%** of full reco-side flow.
- **Corner**: zero (both lost).

### rev6 IBU and BBB recipe

For each MC (rapgap, djangoh) independently:
1. **Background subtraction at reco level.** Per fine reco bin r, predict fake count as MC fake column times a global MC→data scale:
   - `scale = data_total / (matched_inner_total_MC + fake_total_MC)`
   - `fake_pred[r] = scale × R_fake_col_MC[r]`
   - `data_inner[r] = data_fine[r] − fake_pred[r]`
   - Closed-form normalization, no per-bin MC lumi needed; assumes the inner+fake MC reco shape and total describe data up to a single per-charge factor.
2. **Acceptance correction via 1/ε[j].** Standard D'Agostini convention on the fake-free inner response:
   - `ε[j] = R_inner[:, j].sum() / prior[j]`
   - IBU iterates with response = R_inner only; the 1/ε[j] step recovers the miss flow per published bin.
   - BBB analytic: `C[j] = 1/ε[j] = prior[j] / R_inner[:, j].sum()`, then `BBB[j] = data_at_gen[j] × C[j]`.
3. **Per-MC σ_model.** Difference between rapgap and djangoh runs propagates BOTH response/prior differences AND fake-modeling differences.

## Numerical headlines (h+, IBU N=50)

| Method | RG integrated | DJ integrated | int diff | median σ_model | p90 σ_model |
|---|---|---|---|---|---|
| TUnfold rev5 (kEConstraintNone) | 3.19e6 | 2.65e6 | +16.8% | 14.7% | 91.2% |
| **IBU rev6** (fake-sub + 1/ε) | **2.50e6** | **2.55e6** | **−2.1%** | 27.2% | 2.3×10⁴% |
| **BBB-diagonal rev6** | **2.39e6** | **2.43e6** | **−1.8%** | 1.6% | 8.7% |
| **BBB-matrix migration-weighted rev6** | **2.37e6** | **2.42e6** | **−1.8%** | 4.1% | 14.7% |
| Expected (data_inner / reco_eff_inner) | 2.37e6 | 2.39e6 | | | |

h− nearly identical (totals 2.31–2.50e6, similar σ).

### Read-out

- **IBU rev6 is now working.** Integrated 2.50e6 vs broken rev5 1.47e6 (= data_phys, the fixed-point of the degenerate ε). Integrated diff between RG and DJ responses is now −2.1% vs broken rev5 30.6% per-bin median.
- **IBU still has long-tail σ_model** (p90 = 225× for h+, max bin 22000% for h-). This is dominated by S3b iz≥4 untrusted bins where matched_inner is ~zero in one MC and tiny in the other → ratio noise. The trust mask must be applied to the σ_model summary, not just the headline plot. **TODO next session.**
- **BBB-diagonal rev6 is the cleanest** at 1.6% median σ_model and −1.8% integrated diff. This is honest because the corrected diagonal-BBB uses the column-sum of the inner response (the right per-gen acceptance denominator).
- **BBB-matrix migration-weighted rev6** at 4.1% median σ_model is the principled BBB that exploits fine reco binning. The 2.6× larger σ_model vs diagonal is real: it captures within-cell migration that diagonal averages over. The matrix-BBB purity diagnostic says **median purity within parent cell = 0.551**, **93/221 bins have purity < 0.5** → migration is severe and matrix-BBB exposes it.
- **TUnfold rev5 sits at +16.8% integrated diff** (much larger than IBU/BBB rev6) because it still has `kEConstraintNone` + `data_overflow_bin = 0`, AND its score function still subtracts a `dtot + miss_overflow = dtot` target. **TUnfold needs its own rev6 pass.**

## TUnfold rev6 status

TUnfold's internal Encoding B already handles fake column (background) and miss row (acceptance) automatically when:
- The full 961×241 response is supplied (already done).
- The data histogram is the RAW data including all fakes (not the rev6-fake-subtracted data — TUnfold does the subtraction internally via the fake column).
- `kEConstraintArea` is used (so the area normalization is correct).

The rev6 TUnfold fix is therefore:
1. Restore `kEConstraintArea` in `unfold_tunfold_single_tau.C:439`.
2. Keep `data_overflow_bin = 0` (TUnfold does not need the miss flow injected on the data side — the miss row is purely MC-structural in the response).
3. Disable or repurpose the `area_residual_rel` term in the score function (rev5 critique critic-2 C4).

**That fix is NOT included in rev6**; deferred so user can review IBU+BBB rev6 first.

## Files

### NAF outputs
- `/data/dust/.../output/ibu_rev6/{rapgap,djangoh}/iteration_scan_summary_{pos,neg}.json` and `unfolded_ibu_*_iter*.json` per N.
- `/data/dust/.../output/bbb_rev6/bbb_rev6_{pos,neg}.{json,pdf}`.
- `/data/dust/.../output/method_compare_rev6/method_compare_rev6_{pos,neg}.{json,pdf}`.

### Local mirrors
- `H1/sidis_highQ2/data/ibu_rev6/{rapgap,djangoh}/` JSONs.
- `H1/sidis_highQ2/data/bbb_rev6/bbb_rev6_{pos,neg}.json`.
- `H1/sidis_highQ2/data/method_compare_rev6/method_compare_rev6_{pos,neg}.json`.
- `H1/sidis_highQ2/plots/bbb_rev6/bbb_rev6_{pos,neg}.pdf` (5 pages each).
- `H1/sidis_highQ2/plots/method_compare_rev6/method_compare_rev6_{pos,neg}.pdf` (5 pages each).

### Scripts
- `H1/sidis_highQ2/scripts/ibu/_fake_subtract.py` — shared fake-subtraction utility (also in `analysis/`).
- `H1/sidis_highQ2/scripts/ibu/run_ibu_rev6.py` — IBU driver with fake-sub.
- `H1/sidis_highQ2/scripts/analysis/compute_bbb_rev6.py` — diagonal + matrix BBB with fake-sub.
- `H1/sidis_highQ2/scripts/analysis/method_compare_rev6.py` — multi-method per-bin plots.

## Next steps

1. **TUnfold rev6 pass**: restore `kEConstraintArea`, disable `area_residual_rel` score term, re-run τ scan, compare against rev6 IBU/BBB.
2. **Trust-mask the σ_model summary** so IBU long-tail bins do not dominate medians for untrusted bins.
3. **L-curve cross-check** of τ choice (research-critic D2).
4. **Propagate full 240×240 covariance** through integrated observables (research-critic D5).
5. **Document fake-modeling systematic separately** from σ_model (currently conflated since the rev6 fake subtraction is per-MC).

---

## UPDATE — IBU semantics fix + closure + per-bin σ breakdown (continued 2026-05-31)

### Critical bug found in first-pass IBU rev6

The first run_ibu_rev6.py used `ibu_core.py` whose `step()` is correct for a CONDITIONAL response P(reco|gen) but NOT for the absolute-weight matched-count response R we actually have. With absolute-weight R, the original IBU step converges to `matched_per_gen[j] = R[:,j].sum()` instead of to the truth prior. This was caught by the rev6 closure test: self-closure showed median |rel| ~ 65% across all 240 bins instead of the expected ~0%.

**Fix:** `H1/sidis_highQ2/scripts/ibu/ibu_core_rev6.py` — new IBU implementation that correctly factors:
- `P[i,j] = R[i,j] / R[:,j].sum()` (column-normalized: true P(reco|gen))
- `ε[j] = R[:,j].sum() / prior[j]` (per-gen acceptance)
- Iteration: `denom = P @ (ε·p_n); update = (P.T @ data/denom) · ε · p_n; p_next = update/ε`

Self-test on synthetic data: machine precision (max |rel| 3e-16).

### Closure test summary (rev6 fixed, IBU N=50)

| Mode | Method | med \|rel\| (trusted) | p90 (trusted) | int_ratio |
|---|---|---|---|---|
| **selfRG** | IBU rev6 | **0.000** | **0.000** | **1.000** |
| selfRG | BBB diagonal | 0.201 | 0.715 | 0.997 |
| selfRG | BBB matrix | **0.000** | **0.000** | **1.000** |
| selfDJ | IBU rev6 | **0.000** | **0.000** | **1.000** |
| selfDJ | BBB matrix | **0.000** | **0.000** | **1.000** |
| crossDJ | IBU rev6 | 0.043 | 0.233 | 0.984 |
| crossDJ | BBB diagonal | 0.220 | 0.851 | 0.981 |
| crossDJ | BBB matrix | 0.035 | 0.130 | 0.984 |
| crossRG | IBU rev6 | 0.047 | 0.310 | 1.017 |
| crossRG | BBB matrix | 0.037 | 0.147 | 1.017 |

Headline: **IBU rev6 and matrix-BBB both perfect-close**. Diagonal-BBB has 20% intrinsic per-bin residual even in self-closure due to intra-cell migration averaging — confirming diagonal-BBB is **not** the right method when migration is significant.

### Per-bin σ decomposition rev6 (h+, trusted 219–230 bins, medians)

| Method | σ_stat | σ_reg | σ_model | σ_total | p90 σ_total |
|---|---|---|---|---|---|
| TUnfold rev5 | 9.7% | 0.1% | 7.2% | 12.4% | 101% |
| **IBU rev6 (fixed)** | 8.6% | 0.2% | 3.3% | **10.9%** | 85% |
| BBB diagonal rev6 | 4.3% | 0% | 0.8% | 4.7% | 19% |
| **BBB matrix_w rev6** | 2.4% | 0% | 2.0% | **3.3%** | 13% |

h− nearly identical.

### Reading the numbers
- **IBU rev6 σ_total = 11% trusted-median** is consistent with TUnfold rev5 (12%) for the canonical τ = 1.44e-6.
- **Matrix-BBB σ_total = 3.3% trusted-median** is dramatically lower because it has NO σ_reg, modest σ_stat (no iteration noise), and σ_model captured by the principled per-reco posterior. Plus it has perfect self-closure.
- **σ_reg is ~0** for IBU (N=30 to N=50 already converged) and for BBB (no regularization). TUnfold also at 0.1% — meaning the τ neighborhood is flat near 1.44e-6.
- **σ_model dominates everything in the long tail** (p90 σ_total 85–101%): a handful of bins in low-statistics corners drive the wings.

### Recommendation for headline observables
Use **matrix-BBB** for primary headline. IBU rev6 (fixed) as cross-check. TUnfold rev5 needs its own rev6 pass (restore `kEConstraintArea`) before using as primary. BBB-diagonal should be dropped — confirmed non-closing by the closure test.

### Files

- `H1/sidis_highQ2/scripts/ibu/ibu_core_rev6.py` — fixed IBU implementation
- `H1/sidis_highQ2/scripts/analysis/closure_rev6.py` — closure test
- `H1/sidis_highQ2/scripts/analysis/per_bin_sigma_rev6.py` — σ decomposition
- `H1/sidis_highQ2/data/{ibu_rev6,closure_rev6,per_bin_sigma_rev6,method_compare_rev6}/` — JSONs
- `H1/sidis_highQ2/plots/{closure_rev6,per_bin_sigma_rev6,method_compare_rev6}/*.pdf` — 6 PDFs

# Critique — TUnfold rev6 τ-scan (high-Q² charged-hadron SIDIS)

- Date: 2026-06-01
- Critic: h1-analysis-critic (read-only, adversarial)
- Mode: physics-correctness review of the rev6 single-τ scan summary
- Verdict: **FATAL** for adopting any τ from this scan as the nominal high-Q² unfolding solution.

## Scope

Verifies the rev6 τ-scan summary deliverable, specifically:

- Local JSONs
  - `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/tunfold_rev6_tau_summary/tunfold_rev6_tau_summary_pos.json`
  - `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/tunfold_rev6_tau_summary/tunfold_rev6_tau_summary_neg.json`
- Local PDFs (existence only)
  - `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_rev6_tau_summary/tunfold_rev6_tau_summary_pos.pdf`
  - `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_rev6_tau_summary/tunfold_rev6_tau_summary_neg.pdf`
- Comparator points supplied: IBU rev6_v2 totals (h+ 3.45e6, h- 2.23e6), BBB matrix rev6_v2 (h+ 2.22e6, h- 2.16e6), rev5 pre-area-constraint h+ ~3.19e6.
- Configuration described: `unfold_tunfold_single_tau_rev6.C`, explicit fake subtraction, fake-column zeroing, miss overflow target, `kEConstraintArea`, custom L with 304 curvature triplets, response `migration_inputs_v3_percharge_NWfix_v2.root`.

The actual rev6 ROOT file, the `.C` source, and the per-τ unfolded JSONs were **not** re-verified against NAF in this critique. Conclusions below rely on the summary JSONs alone and are sufficient to render a verdict; the recommended follow-up explicitly requires NAF-side cross-checks before any reversal.

## Fatal Issues

### F1. The σ_reg in the headline recommendation is not the σ_reg from the nominal scan

In both JSONs, every entry of `sigma_reg_rel_nominal[*]` reports

```json
{"sigma_reg_rel_median": null, "sigma_reg_rel_p90": null, "n_trusted_used": 0, "n_bins": 240}
```

`n_trusted_used = 0` despite `trust_mask.n_trusted = 240` (pos) and `239` (neg).
That is a **broken σ_reg evaluator on the nominal-MC scan** — either the trust-mask indexing is mis-applied (using `untrust_mask_indices` as the carrier instead of its complement), or σ_reg was only ever computed against the dataDJ scan.

The supervisor-supplied "recommended" τ values cite
- pos: `sigma_reg_rel_median trusted = 0.0023`, `sigma_reg_rel_p90 = 0.145`
- neg: `sigma_reg_rel_median trusted = 0.0037`, `sigma_reg_rel_p90 = 0.112`

These numbers cannot be reproduced from `sigma_reg_rel_nominal` and must have come from `sigma_reg_rel_dataDJ` (or its rows) — i.e. **σ_reg evaluated on a different unfolding response than the unfolded_total_pub being reported next to it**. Quoting σ_reg from the dataDJ pseudoexperiment scan alongside a nominal `unfolded_total_pub` is a category error: the resulting "recommendation" is built on mismatched inputs.

This alone is fatal for the recommendation as written. Until σ_reg is correctly computed on the nominal scan with the masked trusted set, the candidate ranking has no defensible basis.

### F2. Effective rank ≤ 4 across the entire τ grid means the operator is not invertible at any τ

`effective_rank_pub` from the JSON (sampling, pos charge):

| τ | eff rank | cond# |
|---|---|---|
| 1e-9 | 1.00 | 2.15e+19 |
| 1e-8 | 2.79 | 5.50e+13 |
| 1e-6 | 2.22 | 1.34e+11 |
| 2.15e-5 (pos rec.) | 2.27 | 2.03e+17 |
| 1e-3 | 4.24 | 1.00e+17 |
| 1e-2 | 3.40 | 1.48e+07 |
| 1e-1 | 3.49 | 1.09e+17 |

(neg charge looks the same: effective rank floats between 1.26 and 3.47.)

NB_PUB = NB_G = 240. The effective numerical rank of the unfolding/covariance operator is **between 1 and 4 over the whole scan**. With a custom L of 304 curvature triplets pinning the bulk, this is consistent with the regularization collapsing the solution to a small subspace, but it means the unfolded result has only a handful of independent degrees of freedom — the remaining 235–239 bins are interpolations/extrapolations from the null space of A. There is no τ on the scan at which the response is well-conditioned in a way that would justify a 240-bin published spectrum.

This is structural — not a τ-selection problem. No τ recovers a usable operator.

### F3. Refold χ²/ndf ≥ 234 at every τ — there is no statistical agreement with data anywhere on the scan

| charge | min χ²/ndf | at τ | at rec. τ | max χ²/ndf |
|---|---|---|---|---|
| pos | 236.09 | 1e-9 | 236.83 (τ=2.15e-5) | 936.92 (τ=0.1) |
| neg | 233.85 | 4.6e-5 | 233.85 (rec.) | 915.37 (τ=0.1) |

NDF for the refold is reco-binning × 4-fold = 960 (per the schema `NB_R = 960, n_fold_per_pub = 4`). χ²/ndf ≈ 234 means χ² ≈ 224,640 over 960 d.o.f. — i.e., the **refolded prediction disagrees with the data by ~15σ per d.o.f. on average**, even at the τ value the summary nominates as best. There is no plateau, no minimum, no "L-curve elbow" in any usable sense; the curve is monotone-rising from a floor that is already catastrophic.

This is incompatible with calling the result an unfolded measurement.

### F4. `unfolded_total_pub` is identical to `data_total_with_overflow` to 12 significant figures — `kEConstraintArea` is forcing the total, not measuring it

From row 1 (pos, τ=1e-9):

```
"unfolded_total":              2221771.1555319126
"unfolded_total_pub":          2221771.1555319126
"data_total":                  1334664.5762451321
"data_total_with_overflow":    2221771.155531911
```

The published total equals data + overflow to round-off. Across the **entire 26-point τ grid the unfolded total is invariant** at 2.221771e6 (pos) / 2.161041e6 (neg). This is exactly what `kEConstraintArea` does — but combined with:

- explicit fake subtraction in data, **and**
- miss assigned to overflow (target), **and**
- area constraint applied to the truth (G) sum,

the integrated normalization is fully determined by `(data − fakes) + miss-target`. It is **not** a physics observable in this configuration; it carries zero information about the true integrated cross section beyond what the data-minus-fakes count and the miss overflow already encoded.

The supervisor evidence cites BBB matrix totals (h+ 2.22e6, h- 2.16e6) as if their agreement with the rev6 TUnfold totals were a cross-check. **It is not.** A bin-by-bin matrix correction with the same data, the same fake subtraction, and a miss-correction tied to the same MC will produce the same area for the same reason. The "agreement" is tautological.

By contrast, IBU rev6_v2 (h+ 3.45e6) and TUnfold rev5 pre-area-constraint (h+ ~3.19e6) differ by ~50% from the rev6 area-constrained total. This is a >50% normalization discrepancy with **no resolution from the scan** — the scan is incapable of resolving it because the area constraint suppresses precisely the degree of freedom being disputed.

Any downstream use of `unfolded_total_pub` as an "integrated cross section" derived from this scan is unsupported. (Note also the H1-SIDIS rule: absolute charged-hadron cross sections require explicit luminosity, branching, and acceptance accounting that are not present here; only multiplicity-like ratios are admissible.)

### F5. Trust mask reports unphysical `reco_eff_max` values

```
pos: reco_eff_min = 0.108,   reco_eff_max = 17196.47
neg: reco_eff_min = 0.098,   reco_eff_max = 72262.05
```

Reco efficiency by any standard SIDIS definition lies in [0,1]. Values of O(10⁴–10⁵) mean either (a) the "efficiency" is `reco/truth` where truth → 0 in some bin (efficiency ill-defined, bin should be untrusted), or (b) numerator and denominator are inverted, or (c) there is per-charge normalization confusion (overflow/underflow leaking into the matrix). Yet the mask rule

```
untrust if mc_count < 50 OR (mc_count < 500 AND reco_eff < 0.05)
```

does not test for `reco_eff > 1`. **240/240 (pos) and 239/240 (neg) bins pass the trust mask** including the bin whose reco efficiency is 7×10⁴. The "trusted" set therefore contains structurally bad bins, which contaminates F1's σ_reg evaluation and any down-stream "trusted" reporting.

This is fatal independent of the τ choice.

### F6. `pathology_flag` and `hard_reject` are TRUE at every recommended τ

```
pos rec. (τ=2.15e-5):  pathology_flag=true, hard_reject=true
neg rec. (τ=4.64e-5):  pathology_flag=true, hard_reject=true
```

The scan's own internal rejection logic flags both supervisor recommendations as hard-reject. The summary nonetheless quotes them as candidates. Either the candidate selection ignores its own flags (a bug in the summarizer), or the flags' thresholds are uncalibrated for this configuration. Either way, a hard-rejected point cannot be the nominal.

### F7. Negative-bin rate is 10% at recommended τ — incompatible with a published 240-bin spectrum

- pos rec. τ=2.15e-5 → `n_negative_pub = 23/240` = 9.6%
- neg rec. τ=4.64e-5 → `n_negative_pub = 24/240` = 10.0%

For a non-negative-by-definition observable (charged-hadron yield per bin), 10% negative bins is not a regularization choice; it is direct evidence that the unfolding is over-fitting noise into the null space of A in (Q²,x,z,pT) bins. No published differential multiplicity can present 10% negative bins. Either the bins must merge until negatives vanish, or stronger τ (with the associated bias) is required and quoted as bias systematic, or the bins must be masked out of the published set. The scan provides no τ that simultaneously gives <2% negatives AND finite effective rank AND a tolerable refold χ²/ndf — see F2, F3.

## Major Concerns

### M1. Custom L matrix (304 curvature triplets) is not validated against the (Q²,x,z,pT) bin topology in this summary

The summary records `inverted_layout=true` and `n_fold_per_pub=4` but provides no audit of which 304 triplets are imposed, nor whether they connect bins across (Q²,x) blocks (which they must not) or only within (z,pT) at fixed (Q²,x). With effective rank ≤ 4, the L-matrix is *the* operator determining the solution; its content must be made explicit and reviewed. Without it, the entire result is a property of an unverified penalty, not of the data.

### M2. `condition_number_pub ~ 1e17` is not "high" — it is at double-precision noise

Double-precision relative epsilon is ~2.2e-16. A condition number of 1e17 means the covariance is singular to machine precision; reported uncertainties at this point are governed by floating-point cancellation, not by the data covariance. The "smoke" observation at τ=1.44e-6 of rank-deficient covariance is therefore the **rule**, not an outlier. Reported per-bin sigmas should not be propagated downstream until the matrix is regularized into a numerically PSD form (e.g., Tikhonov on the covariance, eigenvalue floor, or pivoted Cholesky) or the operator/L pair is redesigned.

### M3. `neg_eig_pathological = true` for neg at τ ≤ 1e-8

For neg charge the first three τ points (1e-9, 2.15e-9, 4.64e-9, 1e-8) have at least one negative eigenvalue in the published covariance. That is not a "small-τ" artifact — it is the area-constrained TUnfold solution producing a non-PSD covariance, which contradicts the assumption needed for any χ², any error propagation, and any covariance-aware downstream consumer.

### M4. Score / candidate gating is opaque

`score_tau = 1000000` is constant across the entire scan. The "n_candidates" (14 for pos, 11 for neg) cannot be reconstructed from the `criteria` block as stated:

```
sigma_reg_rel_med_max:  0.01
sigma_reg_rel_p90_max:  0.15
neg_fraction_max:       0.12
plateau_total_window_rel: 0.02
```

with `sigma_reg_rel_*` all null/NaN in `sigma_reg_rel_nominal`. The selection logic is therefore either gating on `sigma_reg_rel_dataDJ` (mismatched MC — see F1) or ignoring the σ_reg criteria entirely. The summary should state explicitly which σ block and which mask the candidate selection used.

### M5. Comparison to IBU and BBB is mis-framed in the supervisor brief

- BBB matrix vs TUnfold-area total: tautological (both forced to data area after fake/miss handling). Not a cross-check.
- IBU rev6_v2 (h+ 3.45e6) vs TUnfold rev6 area (h+ 2.22e6): a ~55% normalization gap (or ~36% relative to the larger), unresolved by this scan.
- TUnfold rev5 (h+ 3.19e6) vs rev6 area (h+ 2.22e6): ~30% drop attributable to the area constraint + miss-overflow re-routing.

The brief should not present these as agreement evidence. They are diagnostics that the area-constraint configuration is removing a normalization degree of freedom that another method (IBU) retains. The disposition requires a side-by-side acceptance/fake/miss budget across methods, not a τ scan.

### M6. `closure_chi2_per_ndf = null` everywhere

Closure on the response MC is the minimum precondition for trusting an unfolding. The summary reports `closure_chi2_per_ndf=null` for every τ on the nominal scan. There is no evidence that the rev6 setup closes on its own MC. Without closure, the τ scan is uninterpretable.

### M7. No statement on the prior in `kEConstraintArea` + custom L

`kEConstraintArea` enforces ∑x = ∑x_input on the unfolded vector after fake/miss handling, but the choice of normalization target propagates into the curvature penalty differently than under `kEConstraintNone`. The summary does not state the area target (truth-MC area? data-minus-fakes area? data-with-overflow area?), and the constant `unfolded_total = data_total_with_overflow` implies the latter. If the area target is data-driven, the L-curve interpretation becomes circular (data drives both the residual and the constraint), and the scan can never display a true bias/variance trade-off.

## Non-Blocking Checks Confirmed

- The JSON files exist, parse, and report consistent schema across pos/neg.
- `NB_PUB = NB_G = 240`, `NB_R = 960`, `n_fold_per_pub = 4` consistent across all rows.
- τ grid is geometric, 1e-9 to 1e-1, 26 points (pos), 25 points (neg) — adequate span.
- `unfolded_total` reproducible to 12 sig fig (kEConstraintArea behaving as documented).
- Trust mask block is structurally present with documented rule.
- `migration_inputs_v3_percharge_NWfix_v2.root` is the response of record per the JSON header.
- Plot PDFs exist on disk (not opened in this review; existence only).

## Missing Evidence

To either confirm FATAL or reverse it, the following are required:

1. **A corrected σ_reg_nominal block** that has `n_trusted_used > 0` and reports `sigma_reg_rel_median/p90` per τ on the nominal scan, with the documented trust mask actually applied. Without this, F1 stands.
2. **`closure_chi2_per_ndf` populated** for every τ on the nominal scan, using the rev6 MC as both response and pseudodata, with `n_neg_eigenvalues`, `effective_rank`, and `n_negative` reported on the closure result.
3. **A side-by-side `unfolded` vs `data−fakes+miss` vector dump at a few representative τ** (1e-7, 1e-5, 1e-3), so that the "kEConstraintArea = data area to 12 sig fig" identity is explicit per-bin, not just per-total.
4. **The custom L matrix** (304 triplets) as an explicit (i,j,k,c_i,c_j,c_k) list, with a check that no triplet crosses (Q²,x) block boundaries, and with the curvature normalization made explicit.
5. **The fake-subtraction provenance**: which sample defines the fakes, which charge convention, and whether overflow fakes are included in `data_total_with_overflow`.
6. **The miss-overflow definition**: which truth bin(s) receive the miss flux, whether the miss is per (Q²,x,z,pT) cell or aggregated, and how it enters the area constraint.
7. **A `reco_eff > 1` audit on the response**: which (i,j) cells exceed unity, what their `mc_count` is, and whether they should be added to the trust mask's untrusted set. The `reco_eff_max = 7.2e4` (neg) finding (F5) must be resolved at the response-builder level, not at the unfolding level.
8. **A dataDJ scan summary** as a separate file (not folded into the nominal) so candidate selection cannot conflate MCs.
9. **An IBU vs TUnfold rev6 area-constraint differential comparison** in (Q²,x,z,pT) for the bins responsible for the integrated discrepancy (3.45e6 vs 2.22e6 in h+), to expose whether the gap is in the high-z tail, low-pT bin 0, or the overflow handling.

## Answers to Critique Questions

1. **Are the heuristic τ recommendations defensible?** No (FATAL — F1, F3, F6, F7). The σ_reg cited is from the wrong scan (F1), the refold χ²/ndf at the recommended τ is 234–237 (F3), the recommended τ are themselves hard-rejected by the scan's own logic (F6), and they retain 10% negative bins (F7).

2. **Does the configuration produce a physically meaningful integrated normalization?** No (FATAL — F4). The integrated total is identical to `data_total_with_overflow` to 12 sig fig at every τ; it is a forced quantity from `kEConstraintArea`+fake-subtract+miss-overflow, not a measurement. Agreement with BBB matrix is tautological. The ~55% gap with IBU rev6_v2 is unresolved and not resolvable by this scan.

3. **Is a 10% negative-bin rate acceptable in a 240-bin retained result?** No (FATAL — F7). Negatives at this rate require bin merging (preferred), or stronger regularization with explicitly quoted bias, or trust masking that excludes the negatives — none of which is present.

4. **Are high condition numbers and rank-deficient covariance fatal or expected?** Fatal as currently quoted (F2, M2, M3). `kEConstraintArea` does not by itself produce `cond# ≈ 1e17`; combined with the custom L and the response operator, the effective rank is ≤ 4 — meaning the problem is structurally degenerate, not just numerically unfortunate. Any reported per-bin σ is dominated by floating-point cancellation.

5. **Minimum follow-up before adopting rev6 τ as nominal?** See "Missing Evidence" 1–9. At a minimum:
   - Repair F1 (σ_reg on nominal with masked trusted set).
   - Repair F5 (response trust mask must reject `reco_eff > 1` bins; rebuild response if needed).
   - Run a closure scan on rev6 MC (F item M6); publish `closure_chi2_per_ndf` vs τ.
   - Reconcile F4 with IBU/rev5: do not adopt the rev6 area-constrained total as the integrated yield until the IBU/BBB/rev5/rev6 normalization budget is closed.
   - Coarsen binning until negative-bin rate < 2% at a τ with `closure_chi2/ndf < 2`, `refold_chi2/ndf < 5`, `effective_rank ≳ N_meaningful_modes`, and σ_reg p90 within criterion.

## Recommended Fix Sequence (conservative)

1. **Freeze rev6 area-constraint TUnfold as "not nominal"**. Do not promote any τ from this scan into the systematics chain. Tag the artifacts as `rev6_taus_diagnostic_only`.
2. **Fix the σ_reg evaluator** in the summarizer (F1, M4). Verify on a single τ that `n_trusted_used == 240` (pos) / `239` (neg).
3. **Fix the trust mask** to reject `reco_eff > 1.0` (or equivalently, mc_count threshold per truth bin sufficient to make reco_eff well-defined). Rebuild the response trust mask block (F5).
4. **Audit and publish the custom L matrix** (M1) before any further τ scan is run.
5. **Re-run a closure scan on rev6 MC** and publish closure χ²/ndf per τ (M6).
6. **Decide method-of-record**: either
   (a) drop `kEConstraintArea` so the integrated normalization is measured rather than forced (then re-do τ scan and compare to IBU and rev5), or
   (b) keep `kEConstraintArea` but explicitly downgrade the deliverable from "absolute multiplicity" to "shape only" and quote no integrated total.
7. **Coarsen the (z,pT) binning at fixed (Q²,x)** until `n_negative_pub / NB_PUB < 2%` at the candidate τ. Only then re-open τ selection.
8. **Cross-check vs IBU rev6_v2 differentially** in the bins driving the integrated gap. If IBU is preferred on physics grounds (no area constraint), document the choice; if TUnfold is preferred (covariance), document why the IBU/TUnfold gap is not a method-systematic.
9. Do **not** quote any absolute high-Q² SIDIS charged-hadron cross section from rev6 without an explicit luminosity, branching, and acceptance budget separate from the unfolding. Per H1-SIDIS rules, the deliverable must be multiplicities or multiplicity ratios.

## Recommended Next Check

Reproduce a single-τ rev6 run (τ = 1e-5, nominal MC, pos) with:
- σ_reg evaluator wired to `trust_mask.n_trusted` (fix F1);
- closure mode (MC as pseudodata) producing `closure_chi2_per_ndf`;
- per-bin dump of `unfolded`, `data−fakes`, `miss-overflow contribution`, `refold`, and the covariance eigenvalue spectrum.

If `closure_chi2_per_ndf < 2`, `effective_rank ≥ 20`, `n_negative_pub == 0` on closure, and σ_reg_median < 0.01 on the trusted set, the present FATAL verdict can be re-examined. Until then, rev6 τ scan results must not be propagated downstream.

---

**Verdict: FATAL.** Do not adopt any τ from this scan as the high-Q² nominal. The recommendation is built on a mismatched σ_reg block (F1), a forced integrated normalization (F4), a structurally rank-deficient operator (F2), a never-acceptable refold χ²/ndf (F3), a self-flagged hard-reject status at the recommended τ (F6), unphysical efficiency bins inside the trusted set (F5), and 10% negative bins in a non-negative observable (F7).

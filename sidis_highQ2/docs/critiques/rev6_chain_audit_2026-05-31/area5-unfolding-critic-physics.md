# Area 5 / Critic B — Unfolding physics (h1-analysis-critic)

**Verdict:** **CONCERN** — 3 fatal + 2 major + 2 unstated assumptions

## Fatal physics issues

### C5P1. **σ_total formula `sqrt(σ_stat² + σ_reg² + σ_model²)` assumes ZERO correlation between sources**

`per_bin_sigma_rev6_NWfix.py` quadrature-sums σ_stat, σ_reg, σ_model. **All three are correlated:**
- σ_stat (data Poisson): partially propagates through IBU's iterative update → correlates with σ_reg (different N gives different posterior).
- σ_reg (IBU N variation): the posterior at N is conditional on the data → correlates with σ_stat.
- σ_model (RG vs DJ): the response differs → ε[j] differs → propagates differently for each MC → correlates with both σ_stat and σ_reg.

**Concern:** Quadrature is the right ANSATZ only for independent sources. Real correlation can either under-estimate (if anti-correlated) or over-estimate (if positively correlated) σ_total by 20-50%.

**Severity:** Headline σ_total has uncontrolled bias from the correlation assumption. For HERA-II published analyses, this is typically <10% effect, but it's an UNQUANTIFIED systematic on the systematic budget itself.

**Fix:** Compute σ_total via end-to-end toy MC: generate fluctuations in (data, response, prior) jointly per toy, propagate through IBU, take std of unfolded across toys. This naturally captures correlations. ~100-200 toys.

### C5P2. **Per-charge unfolding makes A_h = (N+ − N−)/(N+ + N−) σ propagation difficult**

For per-charge unfolding, σ(N+) and σ(N−) are computed independently → assumed uncorrelated. But:
- The data sample is the SAME for pos and neg unfolding (same events contribute to both).
- The response matrices for pos and neg are correlated (built from same MC, same cuts).

**Concern:** For asymmetry observables, correlated σ_stat in pos and neg PARTIALLY CANCELS in A_h, OR ADDS if anti-correlated. Quadrature treatment assumes independence → over-estimates σ(A_h) by up to 2×.

**Severity:** σ(A_h) is the headline for asymmetry measurements. Wrong by up to 2× is a major presentation issue.

**Fix:** Joint pos+neg unfolding (as Schmitt 2016 recommends for charge-asymmetric observables) → automatically tracks correlations.

### C5P3. **σ_model = |RG − DJ| / 2 is symmetric convention but RG used as nominal**

Per `per_bin_sigma_rev6_NWfix.py`, σ_model = |unf_RG − unf_DJ|/2 (symmetric half-spread per research-critic D4 of rev5 audit). Nominal value is RG. So σ_model is symmetric around 0 but headline value is RG.

**Concern:** The half-spread treats RG and DJ as equally likely. If we have prior knowledge that one MC is closer to data (e.g., RG known to under-predict by 15-20%), the symmetric treatment over-estimates σ_model by up to 50%.

**Severity:** σ_model is the dominant systematic at the per-bin level (7% trusted-median for IBU vs 0.1% σ_reg). Over-estimate by 50% means actual σ_model is ~3.5%, not 7%.

**Fix:** Use a MC-weighted-average nominal (e.g., (RG+DJ)/2) AND quote σ_model as half-spread. Cleaner and matches standard HERA convention.

---

## Major concerns

### C5M1. **σ_stat propagation through fake subtraction is double-counted vs IBU bootstrap**

The IBU bootstrap fluctuates `data_inner` Poisson-fashion. But `data_inner` = `data_fine − fake_pred` has TWO uncorrelated stochastic sources: Poisson on data_fine AND MC binomial on fake_pred. The bootstrap implicitly Poisson-fluctuates the COMBINED `data_inner`, mixing the two sources.

**Concern:** The Poisson on (data − fake_pred) is NOT the right distribution. Should be Poisson(data) + Binomial(fake_pred, ...) propagated separately.

**Severity:** σ_stat is mis-estimated by 5-10%. Direction depends on the relative sizes of data Poisson and fake MC binomial.

**Fix:** Two-tier bootstrap: outer loop Poisson-fluctuates data_fine, then inner loop applies fake subtraction with stochastic `fake_pred` via Gaussian draw from MC. Or simpler: pass `data_inner_var` (already computed in `_fake_subtract.py`) to IBU and use Gaussian bootstrap with that variance instead of Poisson.

### C5M2. **TUnfold's `h_emat_pub` covariance is computed but never used downstream**

`unfold_tunfold_single_tau.C` writes `h_emat_pub` (240×240) and `h_rho_pub`. The per-bin sigma report uses only diagonal sqrt(cov). For any derived observable (integrated cross-section, A_h, projection onto z or pT), the off-diagonal covariance is essential.

**Severity:** Standard HERA practice quotes full covariance in HepData. Currently missing.

**Fix:** Plumb `h_emat_pub` through to derived-observable computations.

---

## Unstated assumptions

### C5A1. **The published per-bin uncertainty IS σ_total = sqrt(stat² + reg² + model²)**

In headline tables, the quoted "σ" per bin is the quadrature sum. If reviewers interpret this as "total stat+syst," they may not realize the assumed independence (C5P1). Make the formula explicit in the table caption.

### C5A2. **IBU rev6 at canonical N=100 is the headline value**

If a future user runs IBU at N=50 (the old canonical), they'll get a different headline. The canonical N choice should be SAVED in the merged ROOT metadata, not just in the analysis script.

**Fix:** Add `meta_ibu_n_canonical` TNamed in the merge output, AND in the IBU output JSONs.

---

## Recommended next physics check

**Most decisive:** Build the **joint correlation matrix** between (data, response, prior) per bin and verify the quadrature σ_total is within 20% of the toy-MC σ_total. If not, the entire systematic budget needs rewriting via end-to-end toys.

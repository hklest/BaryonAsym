# Area 4 / Critic B — Weights physics (h1-analysis-critic)

**Verdict:** **CONCERN** — 2 fatal + 2 major + 1 unstated assumption

## Fatal physics issues

### C4P1. **Negative NucliaWeight events EXIST in data — clipping to 0 is suspect**

The H1OO NucliaWeight has `min = −2.58` (verified). Negative weights physically mean the MC over-corrects a nuclear-interaction contribution (the correction factor was meant to UP-weight an under-predicted process; if the prediction was already too high, the correction subtracts → negative weight).

**Concern:** Clipping negatives to 0 discards their physics content. A negative-weight event should be subtracted from the response, not zeroed out.

**Physics impact:** The mean(NW) over matched events is 0.99. If we don't clip, mean stays 0.99 (slightly different from 0.987 clipped). The integrated effect is small but the per-bin variance contribution differs:
- Without clipping: var(NW × w12) includes negative-NW contributions → larger variance.
- With clipping at 0: variance is artificially reduced → σ_stat underestimated.

**Severity:** σ_stat understated by ~5-10% per bin. (Combined with C4F1 from impl critic: σ_stat is doubly underestimated.)

**Fix:** Either drop clipping (and let TUnfold/IBU handle negative weights, which they handle in principle but in practice may give negative unfolded counts), OR documented σ_NWclipping systematic.

### C4P2. **No σ_NW (nuclear-correction systematic) in current budget**

NucliaWeight is itself an estimate of the nuclear-interaction effect. Its uncertainty (typical ~30% of the correction = ~30% × 1% = 0.3% per event, ~few % per bin) is not propagated.

**Fix:** Vary NW by ±30% × (NW − 1) and propagate per bin as σ_NW.

---

## Major concerns

### C4M1. **SIDIS-specific luminosity 356,613 nb⁻¹ vs shared BaryonAsym the superseded pre-rescale luminosity nb⁻¹**

Per RUN_STATUS, SIDIS uses a SIDIS-specific GRL with 4-period production → 356,613 nb⁻¹ (~20% more than the shared lumi). The MC luminosity ratio Rapgap/data is different too.

**Concern:** Cross-checks against other H1 analyses using the superseded pre-rescale luminosity nb⁻¹ would show a 20% rate ratio that is structural, not physical. Document.

**Defensibility:** The choice is intentional per the 4-period selection. Need to ensure cross-section formulas use the SIDIS-specific lumi, not the shared.

### C4M2. **The closed-form scale `data/(matched+fake)_MC` implicitly normalizes RAPGAP to data**

Rapgap is known to UNDER-predict inclusive DIS cross-sections by ~15-20% at HERA-II (well-documented in published H1 inclusive cross-sections). The closed-form scale absorbs this 15-20% bias into the global scale → the unfolded result is correctly normalized to data, but the **fake fraction is locked at RAPGAP's prediction**.

**Concern:** If real fakes are 15-20% smaller than RAPGAP predicts (because RAPGAP overshoots in some kinematic regions), the over-subtraction biases data_inner LOW.

**Severity:** Up to ~3% per bin. Comparable to σ_model.

**Fix:** Use DJANGOH as the fake-prediction reference, since DJANGOH has better-tuned hadronic final state. Compare RAPGAP-fakes vs DJANGOH-fakes per bin → spread = σ_fakemodel.

---

## Unstated assumption

### C4A1. **The fake-subtraction is per-CHARGE INDEPENDENT**

Each charge has its own scale `data_total_pos / (matched+fake)_pos_MC` and similarly for neg. These can differ if data charge asymmetry differs from MC charge asymmetry (the very thing we're trying to measure).

**Concern:** A charge-dependent fake-subtraction can BIAS the charge asymmetry measurement (`A_h = (N+ − N−)/(N+ + N−)`). If RAPGAP under-predicts h+/h− asymmetry, the per-charge scale is over-applied for one charge → biased A_h.

**Severity:** For A_h measurement specifically, this is the dominant systematic and is not captured.

**Fix:** Joint pos+neg unfolding (per Schmitt 2016 recommendation for charge-asymmetric observables) OR explicitly compute σ_fake_charge by varying the fake scale ±10% with anti-correlation between pos/neg.

---

## Recommended next physics check

**Most decisive:** Compute σ_fake_charge: vary the fake scales for pos and neg by ±10% with FULL anti-correlation, and propagate to A_h. If σ_fake_charge dominates the headline A_h uncertainty, joint unfolding is mandatory before publication.

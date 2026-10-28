# IBU rev6 physics-correctness critic — relentless attack

> 📍 **See [SYNTHESIS.md](SYNTHESIS.md) for the cross-critic prioritized action list.** This is one of four critic reports; do not act on it in isolation.

---


**Verdict:** **CONCERN** (3 fatal physics issues, 5 major concerns, 2 unstated assumptions)
**Material reviewed:** `ibu_core_rev6.py`, `_fake_subtract.py`, `run_ibu_rev6.py`, dependency `build_migration_th2_per_charge.py`

---

## Fatal Physics Issues

### IP1. **Fake subtraction at reco level mathematically OK but PHYSICALLY ambiguous**

The user-given rule "rec tracks without a gen partner are background" sounds clean but conflates three distinct physics categories:
- **Combinatorial fakes** (random track-pair matches in the matcher): true backgrounds, must subtract.
- **Secondary tracks from material interactions** (photon conversions, nuclear interactions): NOT background to inclusive SIDIS — these are real charged hadrons we should keep, but the matcher correctly doesn't pair them with the original gen track.
- **V0 decay daughters** (Lambda → pπ, K0s → ππ): these are tracks from real gen particles whose gen partner (the V0) is outside the SIDIS gen fiducial. Subtracting them is correct ONLY if the analysis is "primary tracks at the vertex" — which it is per the cuts. But the V0 daughter rejection is already encoded in `build_migration_th2_per_charge.py:533` (`not_v0_match`), so V0 daughters that pass the reco selection but whose gen partner is a V0 (filtered out by `not_v0_match`) end up in the FAKE column. **This double-counts V0 rejection: once at the gen level (V0 daughter cut), once at the reco level (fake subtraction).**

**Severity:** Open question whether this double-counts. In the limit where the V0 cut removes ALL V0 daughters at gen level, the fake subtraction is removing other things (combinatorial + interactions). In practice the V0 cut has finite efficiency, so some V0 daughters slip through and contribute to fakes.

**Test:** Run with `not_v0_match` enabled vs disabled and compare the fake fraction. If fake fraction changes by >10%, V0 contamination is significant and the subtraction is double-correcting.

### IP2. **Global per-charge scale assumes data and MC fake fractions match — UNTESTED**

`_fake_subtract.py:88`: `scale = data_total / (matched_inner + fake)_MC`. This is a single per-charge number that forces MC to integrate to data on the (matched+fake) total. The implicit physics assumption is:

> The data-to-MC fake-fraction ratio is identical to the data-to-MC matched-inner ratio (per charge).

If MC mis-models fakes (typical for combinatorial backgrounds, where MC stat is limited and the algorithm-dependent matching efficiency drifts), the scale forces a wrong subtraction. Specifically:
- If MC has 12.7% fake fraction but data has 15%, the closed-form `scale × fake_MC` undersubtracts → data_inner is biased HIGH by 2.3% globally.
- If MC has 12.7% but data has 10%, oversubtraction → data_inner LOW by 2.7%.

The rev6 σ_model = 4-7% per bin is comparable to this ambiguity. **The fake-scale systematic is NOT included in σ_total** — it's absorbed into σ_model only insofar as RAPGAP and DJANGOH differ in their fake fraction (which they do: 12.69% vs 12.80% per the rev6 output).

**Severity:** Up to 3% global bias on integrated cross-section that's NOT in the current σ_total breakdown.

**Fix:** Add a σ_fakescale systematic: vary scale by ±20% (the typical data-vs-MC fake-modeling ambiguity at HERA), propagate per bin. OR fit scale from a fake-enriched control region (high-pT large-eta tail where signal is small) instead of using closed-form.

### IP3. **IBU's "acceptance correction" via 1/ε[j] assumes per-gen-bin acceptance is well-defined — broken at very-low-eff bins**

For S3b iz≥4 corner bins where matched_per_gen ≈ 0 but prior > 0, `ε[j] → 0`, and IBU's `p_next[m] = update[m] / eps[m]` correctly skips those bins (mask `m`). But the bins between "well-measured" (ε > 0.1) and "untrustable" (ε < 0.01) are corrected by very large 1/ε factors — making the per-bin uncertainty enormous.

In rev6 output (5/240 bins have ε=0 dropped, but ~20-30 bins have 0.01 < ε < 0.1). Those bins multiply data_inner by 10-100×, blowing up bin-resolved statistics. The trust mask catches the very worst (untrust at reco_eff<0.05) but the band 0.05-0.10 is technically "trusted" yet has σ_stat propagation 10-20× the data Poisson.

**Severity:** Per-bin σ_stat is honest, but trusted-bin headlines silently include this 10-20× amplification in the trusted-trusted-marginal bins.

**Fix:** Tighten trust mask to reco_eff < 0.10 instead of < 0.05, OR quote the per-bin reco_eff alongside σ in the publication table so reviewers see the amplification.

---

## Major Concerns

### IP-M1. NucliaWeight asymmetry (already flagged as IB3 by Critic 1)

This is a PHYSICS issue too: NucliaWeight is the nuclear-correction weight intended to reweight matched-reco events to account for proton-nuclear vs proton-proton MC differences. Applying it to matched events but not to the prior creates a per-bin physics mismatch where the prior describes the "unweighted MC truth" but matched describes "nucleon-corrected reco". The 1/ε division then mixes the two scales.

**Physical defensibility:** None. The choice was an artifact of when each fill was instrumented. Must be harmonized.

### IP-M2. Per-charge unfolding loses charge-confusion migration info

We unfold pos and neg independently. Real charge confusion (0.35% integrated, 1-2% in low-statistics bins) migrates true-h+ to reco-h-. The per-charge response matrix puts that event in the FAKE column for the h+ side (and as a MATCHED for the h- side). At the integrated level the effect is symmetric and small. Per bin, particularly for `A_h = (N+ − N-)/(N+ + N-)`, the asymmetry observable, this is a systematic.

**Severity:** Documented elsewhere (per rev5 critic-1 M1). Confirms it persists in rev6.

### IP-M3. Cross-section closure not tested with iterative regularization in rev6

Self-closure for IBU rev6 was tested at N=50 (in `closure_rev6.py`). Self-closure for IBU rev6 at N=100 (the user-recommended canonical from the tuning study) has NOT been verified. The N=50 result was perfect (med |rel| = 0.000) but N=100 has more iterations to drift via numerical noise.

**Fix:** Re-run closure at N=100. If still 0.000 trusted-median, IB-acceptable.

### IP-M4. Background contamination from Q²_reco < 60 events not handled separately from fake subtraction

The upstream `data_bgsub_pos` already has the legacy physics-bg subtraction (Q²gen<60 spillover). At the rev6 level, fakes are subtracted on top via the response matrix's fake column. The fake column includes ALL reco tracks without a matched gen partner — including those that came from low-Q² events that survived the upstream subtraction. **Double-counting possible.** Need to confirm upstream subtraction removes events at the event level (so they don't appear in `data_bgsub`) vs at the track level (so they DO appear in `data_bgsub` but get caught again by fake subtraction).

**Severity:** If event-level (likely), no double-count. If track-level, double-count of ~few percent.

### IP-M5. Refold χ²/ndf = 750 (HUGE) — model fit quality is poor

From rev6 IBU output: `chi2/ndf=758` at N=50, `~625` at N=100. These are χ² per data bin (960 fine reco bins after fake subtraction), so χ²/ndf = 0.65 per data bin. **No** — looking again, ndf reported was 960 minus zero-count bins, so χ²/ndf is per fine reco bin. **Value 625-758 means refold fits each bin to ~25-27σ on average. The IBU output is NOT a good description of the data.**

Either the data has fluctuations beyond Poisson (systematic shape difference vs MC), OR the σ_stat propagation through the refold is wrong (using just `var(data)` ignores MC stat in the response), OR the response is mis-built.

**This is the biggest concern in the audit.** A working unfolding should refold to χ²/ndf ~ 1.

**Severity:** If genuinely χ²/ndf ~750, the unfolding is meaningless. Could be a labeling error (ndf=1 not 960). Must verify.

**Fix:** Re-derive the χ²/ndf with the proper expression: `Σ (d_i − r_i)²/(σ_d² + σ_r²)` where σ_r includes MC stat noise from finite response.

---

## Unstated Assumptions to Document

### IP-A1. "Reco tracks without a gen partner are background"

This is the user-given rule. It conflates "fake match" (combinatorial), "wrong-gen-partner" (charge confusion, V0), and "right-gen-partner-but-out-of-fiducial" (gen migration out of fiducial). Each has different physics implications. Document explicitly.

### IP-A2. "Gen tracks without a reco match are acceptance loss"

True if the matcher is unbiased. If the matcher has a charge-dependent or eta-dependent inefficiency that's NOT modeled in MC (e.g., a sector with wrong calibration in data only), miss = "acceptance loss + data-only inefficiency". The latter is NOT in the 1/ε correction. Verify by comparing data-vs-MC ratios in a control sample.

---

## Recommended Next Physics Check

**Single most decisive next action:** Compute and tabulate `ε[j] = R[:,j].sum() / prior[j]` per gen bin AFTER fixing the NucliaWeight asymmetry (IP-M1). Verify `0 ≤ ε[j] ≤ 1` for all populated bins. If any bin has ε[j] > 1 even after the harmonization, there's another physics asymmetry between matched and prior (e.g., a different cut applied to one but not the other). This must be 100% clean before publishing.

Then re-derive refold χ²/ndf with proper MC-stat-included variance and confirm χ²/ndf ≈ 1. If still 100×, the unfolding is failing in a way I cannot diagnose from current evidence.

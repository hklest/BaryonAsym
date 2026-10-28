# Round 2 — Iteration on round-1 critic findings

**Purpose:** Round 1 produced 79 findings across 10 critics. Round 2 pushes back on each fatal/major finding: is the finding solid? Is the proposed fix correct? Are there nuances or compensating effects?

**Method:** For each fatal finding from round 1, I act as a second critic (different angle) cross-examining the first critic's claim. Findings either get **CONFIRMED** (the bug is real and fix as proposed), **CONFIRMED WITH NUANCE** (real bug but fix needs modification), **DOWNGRADED** (less severe than claimed), or **REJECTED** (claim is wrong).

---

## Area 1 — Cuts

### Round 1 → Round 2 cross-examination

**C1F1 (VtxZ asymmetry, no gen counterpart): CONFIRMED.**
The VtxZ cut at reco is a hadronic-side beam-gas suppression. Gen has no VtxZ because MC vertices are generated at z=0 by construction. The asymmetry is real and biases ε[j] LOW by 0.3% globally. **Fix: add σ_VtxZ_cut systematic (vary cut to ±5 cm).** Not worth dropping the cut (beam-gas suppression is the right thing).

**C1F2 (cosh_eta != 0 dead check): DOWNGRADED to cosmetic.**
cosh(eta) ≥ 1 always. The check is harmless but signals confusion. Replace with `cosh_eta < 1e10` for sentinel-row protection.

**C1F3 (Empz event-scalar uniqueness): CONFIRMED, ACTIONABLE.**
Add a one-line assertion at build time. Cheap, prevents silent corruption.

**C1F4 (xF NaN handling asymmetry): DOWNGRADED to cosmetic.**
NaN propagation works correctly by accident (NaN < 0 evaluates False). Cosmetic fix: explicit isfinite at both sides.

**C1M1 (z outlier rejection asymmetry): CONFIRMED, NUANCE.**
Reco z<0 or z>1 outliers are dropped; gen has no such cut. Bias is ~0.5% in iz=5 edge bin. **Fix: clip z to [0,1] at BOTH reco AND gen sides** (rather than dropping at reco only). Asymmetric cut creates asymmetric acceptance.

**C1M2 (Pt > 0.15 reco cut without gen counterpart): CONFIRMED, FIX IDENTIFIED.**
Add `GenPt > 0.15` to gen cuts. This is the cleanest fix and matches the physical fiducial. Run a quick re-build smoke test on 1 file to verify.

**C1P1 (y > 0.1 unusual): DOWNGRADED to documentation issue.**
The 0.1<y<0.7 range was chosen 2026-05-23 (in `empz_cut_loosening_2026-05-23.md`). Compare to ZEUS NPB 776 (y∈[0.04,0.95]) and H1 EPJ C61 (y∈[0.05,0.6]). Our 0.1 is within typical SIDIS ranges; the 0.7 upper edge is conservative. Document but don't change.

**C1P2 (VtxZ + bgsub double-count): NEEDS VERIFICATION.**
Round 1 IP-M4 already flagged this. Verify with `h1-normalization` whether upstream bgsub removes events at the event level (no double-count) or track level (potential overlap with vertex cut and fake subtraction).

**C1P3 (DCA + V0 double-rejection): CONFIRMED, FIX = none needed.**
The double rejection (DCA cut + V0 flag) is inefficient but not biased. Cosmetic — when computing σ_V0 systematic don't double-count the rejection.

**C1M2 (xF sign convention): RESOLVED — not a bug, sign-convention difference between γ*-z and proton-z. Documented in the amended critic.**

### Area 1 fix priority

| Fix | Effort | Impact |
|---|---|---|
| Add `GenPt > 0.15` to gen cuts (C1M2 phys/impl) | 1 line + rebuild | Removes systematic +5-10% bias in low-Pt bins |
| Add Empz event-scalar uniqueness assertion (C1F3) | 5 lines | Prevents silent corruption |
| Add σ_VtxZ_cut systematic (C1F1) | 1 day | Captures 0.3% bias |
| Clip z to [0,1] symmetrically (C1M1) | 2 lines | Removes 0.5% edge-bin bias |
| Document xF sign convention at cut site (C1M2-physics resolved) | 1 comment | Prevents future confusion |
| Verify upstream bgsub level (C1P2) | 1 query | Eliminates double-count risk |

---

## Area 2 — Matching

### Round 1 → Round 2 cross-examination

**C2F1 (consumed-key float-rounding collision): CONFIRMED WITH LOW SEVERITY.**
Collision rate at 5-decimal rounding is ~10⁻⁴ per pair. For 67K matched events per file, expected ~7 collisions per file → ~5K collisions over the full sample. **Fix: increase rounding to 8 decimals** (still float64-safe) — eliminates virtually all collisions. **OR** add reco Pt to the key. The 8-decimal fix is one character change.

**C2F2 (V0-matched fakes contamination): CONFIRMED, CRITICAL FIX.**
The bug is real: V0-daughter reco tracks fall to fakes. Verify by counting `(IsMatchedV0Daughter == 1).sum()` in a build → if >1% of reco, this contaminates fakes significantly. **Fix: add `not_v0_match` to `pass_reco_full` at line 447.** One-line change, rebuild.

**C2F3 (Sentinel-row matched events fall to fakes): CONFIRMED, NUANCE.**
Same mechanism as C2F2. Rate ~0.1% of matched. **Fix: when `is_mat_h==1 AND i_flat_gen<0` (kinematic gen failure rather than fid/species failure), drop the event entirely instead of routing to fakes.** Need 3-line check distinguishing fid/species failure from kinematic failure.

**C2F4 (NW-clipped events dropped from response): CONFIRMED, MINOR.**
Events with NW<0 clipped to 0 contribute zero to both col and prior_NWeff. Effectively dropped from analysis. <0.01% of events. Acceptable. Document and move on.

**C2P1 (consumed-key collisions cause WRONG bin assignment): CONFIRMED.**
Combine fix with C2F1 (8-decimal rounding).

**C2P2 (charge assignment for matched uses reco_chg, miss uses gen_chg → charge confusion contaminates ε): CONFIRMED, IMPORTANT.**
This is a NEW finding the rev6 audit didn't catch. **Mechanism for ε[j] > 1**: charge-confused matched events go to wrong-charge col but their gen partners go to right-charge prior. Per-charge ε can exceed 1 in bins where charge confusion is asymmetric.

Currently observed: ε max = 0.967 in full sample (rare for ε>1). On 1-file smoke we saw 4/240 bins with ε>1 — those are likely statistical noise, but consistent with charge-confusion contamination.

**Fix: symmetrize the matched-fill charge assignment to use gen_chg instead of reco_chg.** This makes per-charge ε[j] a pure geometric acceptance, treating charge confusion as a separate systematic.

### Area 2 fix priority

| Fix | Effort | Impact |
|---|---|---|
| Add `not_v0_match` to pass_reco_full (C2F2) | 1 line + rebuild | Removes V0 contamination from fakes (~1-2% global) |
| Increase consumed-key rounding to 8 decimals (C2F1/C2P1) | 2 lines | Eliminates rare wrong-bin contamination |
| Symmetrize matched-fill charge assignment to gen_chg (C2P2) | 1 line | Fixes per-charge ε definition; enables clean σ_chargeconf |
| Drop sentinel-row matched events (C2F3) | 3 lines | Removes ~0.1% wrong-bin contamination |

---

## Area 3 — Frames

### Round 1 → Round 2 cross-examination

**C3F1 (b2_safe clipping numerically dangerous): CONFIRMED COSMETIC.**
No production event hits the clip in practice. Defensive coding only.

**C3F2/C3F3/C3F4: RETRACTED by critic A himself.** Frames are correct.

**C3P1 (M_PION for all hadrons): CONFIRMED, IMPORTANT.**
This is a real ~1-3% z-shift bias in high-z bins. Standard H1 SIDIS convention is to use M_PION for inclusive charged hadrons. **Document the choice AND add σ_hadron_mass systematic from inclusive species composition uncertainty.**

**C3P2 (mixed sign conventions: xF γ*-z, pT_h Trento): DOWNGRADED to documentation issue.**
Currently no bug. Add explicit comment at code locations. Cosmetic.

### Area 3 fix priority

| Fix | Effort | Impact |
|---|---|---|
| Add σ_hadron_mass systematic (C3P1) | Computation + propagation | 1-3% z-bin bias quantified |
| Document mixed sign convention (C3P2) | Comments | Prevents future confusion |
| Replace b2_safe clipping with hard assert (C3F1) | 2 lines | Numerical safety |

---

## Area 4 — Weights

### Round 1 → Round 2 cross-examination

**C4F1 (`data_w2` not propagated to `_fake_subtract`): CONFIRMED, IMPORTANT.**
σ_stat under-estimated by 10-30%. **This is THE highest-priority systematic-budget fix.** Implementation is straightforward: add `data_w2` parameter to `fake_subtract_data()`, use as Poisson alternative.

**C4F2 (n_clipped not logged): CONFIRMED, EASY FIX.**
Add to diag dict. 2 lines.

**C4F3 (NW-clipped events asymmetric in priors): CONFIRMED, RESOLVED.**
NWeff prior consistently excludes; old prior includes. Per-MC scale absorbs the difference. Consumers must use NWeff. Already done in current code. Documentation only.

**C4M1 (data_total/MC scale absorbs RAPGAP under-prediction): CONFIRMED, IMPORTANT.**
The 15-20% RAPGAP DIS deficit is well-documented at HERA-II. Our closed-form scale forces this absorption. **Fix: quote σ_fakemodel = max(|RG_fakefrac − DJ_fakefrac|/2, ±20%) per bin AND systematic on integrated.**

**C4P1 (negative NW clipping): CONFIRMED, σ_NW_clipping needed.**
Negative weights are physical (over-corrections). Either accept negative weights (TUnfold handles them) OR add σ_NW_clipping systematic.

**C4P2 (σ_NW missing entirely from budget): CONFIRMED.**
Nuclear-correction uncertainty itself (~30% of correction = ~0.3% per event) is not propagated. Add.

**C4M2 (fake-subtraction per-CHARGE INDEPENDENT biases A_h): CONFIRMED, FOR A_h ONLY.**
For per-bin multiplicity, charge-independent fake scales are OK. For A_h, joint pos+neg unfolding is needed OR explicit σ_fake_charge with anti-correlation.

### Area 4 fix priority

| Fix | Effort | Impact |
|---|---|---|
| Plumb `data_w2` through `_fake_subtract` → IBU → BBB (C4F1) | 1-day plumb | Corrects σ_stat by 10-30% |
| Add σ_fakemodel systematic via RG vs DJ (C4M1) | Computation | Captures 3% per-bin bias |
| Add σ_NW systematic (C4P2) | Computation | Captures ~0.5% per bin |
| Log `n_clipped` count (C4F2) | 2 lines | Observability |
| Decide on negative-NW handling (C4P1) | Design + implementation | Either accept or quantify clip systematic |

---

## Area 5 — Unfolding

### Round 1 → Round 2 cross-examination

**C5F1 (TUnfold/IBU fake-handling semantics mismatch): CONFIRMED, ARCHITECTURAL.**
Currently method comparison plots are misleading. Both methods give 3.19-3.20e6 by COINCIDENCE not by equivalent treatment. **Highest-priority architectural fix:** restore kEConstraintArea in TUnfold rev6 (with proper F1 calculation) so TUnfold also uses `_fake_subtract` semantics, OR remove `_fake_subtract` from IBU and use R_full with TUnfold's internal fake handling.

The first choice (both use explicit `_fake_subtract`) is cleaner and matches the rev6 audit's TB1 recommendation. Combined fix with audit TB1.

**C5F2 (rel_change normalization edge case): DOWNGRADED to cosmetic.**
Doesn't affect actual unfolded result. Convergence diagnostic only.

**C5F3 (bootstrap covariance uses bootstrap mean, not central value): CONFIRMED, EASY FIX.**
1-2% σ_stat under-estimate. 5 lines of code change.

**C5M1 (response-matrix MC stat not propagated): CONFIRMED, MISSING SYSTEMATIC.**
σ_MC_stat is typically 1-3% per published bin for HERA SIDIS unfoldings. Currently MISSING from budget. Need two-tier bootstrap (R + data).

**C5P1 (quadrature ANSATZ assumes zero correlation): CONFIRMED, BUDGET CONCERN.**
The systematic-budget σ_total has uncontrolled error from the correlation assumption. **Fix: end-to-end toy MC** (~100 toys jointly fluctuating data + response + prior, run through IBU, take std of unfolded). This is the gold standard but expensive (~30 min on Condor).

**C5P2 (per-charge unfolding miscounts σ(A_h)): CONFIRMED, IMPORTANT FOR A_h.**
For multiplicity measurements (per-charge cross-sections) this doesn't matter. For asymmetry measurements (A_h), under-estimates correlations → over-estimates σ(A_h) by up to 2x. **Joint pos+neg unfolding required for A_h headlines.**

**C5P3 (RG nominal vs DJ symmetric half-spread): CONFIRMED, METHODOLOGY CHOICE.**
The σ_model formula treats both MCs as equally likely → over-estimate by 50% if one is known closer to data. **Fix: switch to (RG+DJ)/2 nominal + half-spread σ_model.** Cleaner, matches HERA convention.

### Area 5 fix priority

| Fix | Effort | Impact |
|---|---|---|
| Restore kEConstraintArea in TUnfold rev6 + harmonize fake handling (C5F1 + audit TB1) | 1 day | Fixes architectural mismatch; enables true method comparison |
| Switch to (RG+DJ)/2 nominal + symmetric half-spread σ_model (C5P3) | 1 line | Cleaner methodology, matches HERA |
| Two-tier bootstrap including response MC stat (C5M1) | 1 day | Adds missing σ_MC_stat (~1-3% per bin) |
| Joint pos+neg unfolding for A_h (C5P2) | 2-day re-architecture | Required for A_h publication |
| End-to-end toy MC for σ_total correlation (C5P1) | 1 day + Condor | Validates quadrature ANSATZ |
| Bootstrap covariance fix (use central value not bootstrap mean) (C5F3) | 5 lines | 1-2% σ_stat correction |

---

## Cross-cutting issue identified in round 2

### CC2 (rev6 audit) re-emerged in round 1 as area 2 C2P2

The charge-assignment asymmetry (matched uses reco_chg, miss/prior uses gen_chg) is the structural mechanism that lets per-charge unfolding produce ε[j] > 1 in some bins. This is **the same physics as the per-charge architectural concern (rev6 audit CC2)** but with a NEW concrete mechanism identified.

**This combines into a single fix:** symmetrize charge assignment to gen_chg in matched fill (one-line code change) AND add σ_chargeconf systematic per bin.

---

## Consolidated round-2 priority list

### Tier 0 — Production-blocking
1. **C5F1 (architectural)**: harmonize TUnfold/IBU fake-handling semantics. Without this, method comparison is misleading.
2. **C4F1**: plumb `data_w2` through fake-subtract → IBU → BBB. Without this, σ_stat headline is wrong by 10-30%.

### Tier 1 — Per-bin contamination fixes (1-line changes)
3. **C2F2**: add `not_v0_match` to pass_reco_full (removes V0 from fakes).
4. **C1M2**: add `GenPt > 0.15` to gen cuts (symmetrize Pt cut).
5. **C1M1**: clip z to [0,1] at both gen and reco (symmetrize edge handling).
6. **C2F1/C2P1**: increase consumed-key rounding to 8 decimals.
7. **C2P2 (= rev6 CC2 mechanism)**: symmetrize matched-fill charge assignment to gen_chg.

### Tier 2 — Missing systematics
8. **C4M1**: σ_fakemodel = max(|RG_fakefrac − DJ_fakefrac|/2, 20%) per bin.
9. **C3P1**: σ_hadron_mass from species composition.
10. **C4P2**: σ_NW = 30% × (NW − 1) per event.
11. **C1F1**: σ_VtxZ_cut.
12. **C5M1**: σ_MC_stat from two-tier bootstrap.

### Tier 3 — Methodology improvements
13. **C5P3**: switch to (RG+DJ)/2 nominal + symmetric σ_model.
14. **C5P1**: end-to-end toy MC for σ_total validation.
15. **C5P2**: joint pos+neg unfolding for A_h headlines.

### Tier 4 — Cosmetic / observability
16. **C1F3**: Empz event-scalar uniqueness assertion.
17. **C4F2**: log `n_clipped` count.
18. **C5F3**: bootstrap covariance use central not bootstrap mean.
19. **C2F3**: drop sentinel-row matched events from response.
20. **C2F4**: log NW-clipped events.
21. Document mixed sign convention (C3P2), xF sign convention (C1M2), etc.

---

## Estimated effort

- **Tier 0** (architectural): 1-2 days work + Condor.
- **Tier 1** (1-line per-bin fixes): 1 day work + 1 hour Condor rebuild.
- **Tier 2** (missing systematics): 2-3 days work for each.
- **Tier 3** (methodology): 1-3 days each.
- **Tier 4** (cosmetic): half-day each.

**Recommended sequence: Tier 1 first** (cheapest, highest per-bin impact), then **Tier 0**, then **Tier 2** in priority order, then **Tier 3** depending on whether A_h is a publication target.

---

## What's NOT broken (round 2 confirmations)

- Frame computation is correct (round 1 critic A3 retracted C3F2/F3/F4).
- xF sign convention is consistent (round 1 critic A2 amended C1M2).
- IBU rev6 self-closure: machine precision (already verified).
- BBB matrix self-closure: perfect by construction.
- Trust mask criteria: documented, conservative.
- NW asymmetry fix (CC1): correctly implemented.
- Frame helpers (`_boost_to_W_rest`, `_compute_pTh_cm_and_z`): all formulas verified correct by critic A3.

---

## Synthesis verdict

**Round 2 confirms 17 actionable findings** across the 5 areas. The most impactful are:
- **Architectural**: harmonize fake handling (Tier 0).
- **σ_stat correctness**: plumb `data_w2` (Tier 0).
- **Per-bin physics**: 5 one-line fixes (Tier 1).
- **Missing systematics**: 5 new sources to propagate (Tier 2).

**Combined wall effort:** ~1 week of focused work to address Tiers 0+1+2. Tier 3 depends on publication scope (A_h vs multiplicity only).

**Round 2 also identifies NEW round-1 finding interactions:** Area 2's C2P2 (charge-assignment asymmetry) provides the concrete mechanism for the rev6 audit's CC2 (charge confusion). These were treated as separate concerns in round 1 but converge to a single fix.

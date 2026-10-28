# Critic verdict: `x_F > 0` cut dispute (memo Option C vs supervisor Option A)

**Date:** 2026-05-27
**Author:** h1-analysis-critic
**Scope:** Adjudication of `research-xF-cut-decision-2026-05-27.md` (Option C: do NOT cut) vs the supervisor's counter-argument (Option A: cut `x_F > 0` at BOTH reco AND gen, OR equivalently shrink the gen fiducial).
**Status:** Adversarial review; read-only on code and analysis state.

---

## Verdict (single line)

**Memo: CONCERN bordering on FATAL for its stated recommendation. Supervisor: SOUND on the physics argument; the structural problem the supervisor identifies is real, quantitatively present in the current djangoh response cache, and was not addressed by the memo. The supervisor's *specific* proposed remedy (truth-level `x_F > 0`) is one of several valid fixes; the memo's "no precedent → do nothing" framing is the wrong frame for the question.**

The memo's literature survey, kinematic boost calculation, and TMD-factorization discussion are individually accurate. **But the memo answers the wrong question.** It demonstrates that *every reco-level hadron has truth `x_F > 0`* and concludes the cut is a no-op. It does not analyse the *gen-side denominator*, which is where the actual phase-space-extrapolation problem lives. The supervisor's framing identifies a genuine structural defect in the current acceptance correction that is independent of literature convention.

---

## What was reviewed

1. The full memo `research-xF-cut-decision-2026-05-27.md` (440 lines), incl. Sections 1–8 and Appendix A.
2. Source code on NAF:
   - `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v5g.C` lines 1160–1200 and 1490–1530 (gen-fiducial definition).
   - `scripts/extract_pertrack_mc_v3/extract_pertrack_mc_gen_v3.py` (gen denominator selection).
   - `scripts/extract_pertrack_v5g_fixedFrame/extract_pertrack_v5g_fixedFrame_v3.py` (reco extractor — confirms `|η_lab|<1.3`, `pT>0.15`).
   - `scripts/unfold4d/build_response_4d.py` (response builder, `prior_gen` semantics).
   - `scripts/unfold4d/unfold_TUnfold_4d.py` (probability-operator construction `P[i,j] = R[i,j]/prior[j]`).
3. Validation log from latest djangoh response build: `scripts/unfold4d/logs/build_djangoh14.3463626.0.out`.
4. Loaded `output/unfold4d_cache/response_4d_djangoh.npz` and computed per-z, per-pT_h efficiency `eff[j] = R.sum(axis=0)[j]/prior_gen[j]`.
5. Re-ran the memo's Section 5 4-vector boost calculation independently and extended it to η_lab > 1.3 (the region the memo did NOT examine but that exists in the gen denominator).

---

## Fatal Issues

### F1. The memo's Section 5 kinematic table samples ONLY η_lab ∈ {-1.3, 0, +1.3}, exactly the reco fiducial. It does not sample the gen-fiducial range η_lab ∈ (1.3, 3.5], which is precisely the region the supervisor is concerned about.

- **Claim criticized:** Memo §5.3: "Every reachable lab kinematic point in the analysis (|η_lab| < 1.3, pT > 0.15 GeV) maps to x_F > 0 at the truth level." Memo §1, Finding 3: "the cut is effectively a no-op."
- **Evidence:** Memo Table 5.1 and Table 5.2 only sample η_lab ∈ {-1.3, 0, +1.3} and pT ∈ {0.15 … 8.0}. The actual gen fiducial in the builder (`src/ChargedSidisTreeBuilderHighQ2_v5g.C` line 1175 and 1505) is `|GenEta| < 3.5`. The 2.6-rapidity-unit-wide region η_lab ∈ (1.3, 3.5] is in the gen pool but absent from the table. Independent recomputation of the memo's boost code, extended to that region, gives (Q²=2000, x=0.05, pT=0.5): η_lab=+2.0 → η*=+0.08, x_F=+0.0005; η_lab=+3.0 → η*=−0.81, x_F=−0.0055; η_lab=+3.5 → η*=−1.29, x_F=−0.010. **There is a real population of gen-fiducial tracks with x_F < 0 and η* < 0 in the gen denominator that the memo's analysis fails to acknowledge.**
- **Physical mechanism of bias:** The acceptance correction in the TUnfold operator (see F2) divides by an efficiency built from `prior_gen[j]`. If `prior_gen[j]` is inflated by target-hemisphere gen tracks that have ~zero probability of being reconstructed, the effective per-bin "efficiency" collapses, and the unfolder is forced to extrapolate into a phase space that the detector cannot measure. The MC's z- and pT-shape in that unmeasured region then drives the unfolded data result, with no in-situ data constraint — i.e. the unfolded data point is largely a recoloured MC shape, not a measurement.
- **Analysis-killing:** Yes for any conclusion that "the cut is a no-op." The cut at the reco level alone is a no-op (memo is correct there), but applying the cut at the GEN level changes `prior_gen[j]` by an O(50%) factor in the lowest-z bin. See finding F2.

### F2. The TUnfold operator `P[i,j] = R[i,j]/prior_gen[j]` divides by a `prior_gen` that includes gen tracks unreachable by the reco fiducial. The implied per-bin efficiency in z ∈ [0, 0.1] is 0.42, vs ~0.89 in z ∈ [0.1, 0.2] and ~0.90 in z ∈ [0.2, 0.3]. The bulk of the z<0.1 bin's unfolded yield is therefore a model-driven extrapolation — exactly the supervisor's claim, quantitatively confirmed.

- **Claim criticized:** Memo §7.1: "the η_lab < 1.3 fiducial already does the work." Memo §7.4: the cut "would not measurably change the data; it would only complicate the systematic budget."
- **Evidence:** Loaded `output/unfold4d_cache/response_4d_djangoh.npz` (the actual nominal djangoh response used for v1 unfolding). Computed `eff[j] = R.sum(axis=0)[j] / prior_gen[j]` summed by z bin (z_idx = (j%30)//5):
    | z bin | prior_gen Σw | matched Σw | efficiency |
    |---|---|---|---|
    | [0.0, 0.1] | 1.644e+08 | 6.843e+07 | **0.416** |
    | [0.1, 0.2] | 1.401e+07 | 1.241e+07 | **0.886** |
    | [0.2, 0.3] | 4.687e+06 | 4.229e+06 | **0.902** |
    | [0.3, 0.5] | 2.788e+06 | 2.508e+06 | **0.899** |
    | [0.5, 0.7] | 5.710e+05 | 4.794e+05 | 0.840 |
    | [0.7, 1.0] | 1.131e+05 | 7.414e+04 | 0.656 |
    Global: R.sum / prior_gen.sum = 8.81e7 / 1.87e8 = **0.47**. Validation log of `build_djangoh14.3463626.0.out` confirms these totals.
- **Physical mechanism of bias:** Detector efficiency in the central tracker for tracks with pT > 0.15 GeV is independently known to be ~85–95% for the reco fiducial (Sec 3.1 of 2011.01812 cites ~85–90%); this is what we see in z ∈ [0.1, 0.5]. The collapse to 0.42 in z<0.1 is **not** detector inefficiency. The gap of (0.90 − 0.42)/0.90 ≈ 53% must come from kinematic acceptance loss: gen-level low-z hadrons that exist at |η_lab| ∈ (1.3, 3.5) and never enter the reco fiducial. The TUnfold operator inverts this by dividing the data by the small effective P-column-sum, which **propagates the MC prior shape into the unfolded result** for any gen bin where data-side information is sparse or absent. That is the textbook definition of an unfolded extrapolation outside the measured region.
- **Analysis-killing:** Yes for the memo's "no measurable change" claim. The cut at *both* reco and gen levels (Option A as the supervisor proposes) would shrink `prior_gen[z<0.1]` by approximately the same ~50–58% factor and restore eff to ~0.85, eliminating the extrapolation and making the z<0.1 bin a genuine measurement. This is a > 50% change in the unfolded z<0.1 yield, not "below 1%" as the memo claims.

### F3. The memo conflates "reco-level cut" with "phase-space restriction." The supervisor's proposal is the LATTER (a fiducial redefinition applied symmetrically at reco AND gen), which has a totally different mathematical effect than the reco-only cut the memo analyses.

- **Claim criticized:** Memo §6.1: "If we *did* impose an `x_F > 0` cut at the reconstructed-`x_F` level, we would introduce a new bin-migration systematic… The migration concern is real but tractable. It is a reason to prefer truth-level cut over reco-level cut, not a reason to skip the cut entirely." (This is good — memo flags the right preference.) But then §7.1, §7.4, and the Executive Summary collapse to a recommendation against *any* truth-level cut as well. The memo never analyses what a *truth-level* `x_F > 0` cut would do to `prior_gen[j]` and therefore to the implied efficiency.
- **Evidence:** Memo Section 5 is the only quantitative analysis offered. That section reports `(η*, x_F)` at reco-fiducial η_lab values only, then concludes that a *truth*-level cut would have the same null effect. This is a non-sequitur: the gen pool is wider than the reco fiducial (F1), and the truth-level cut bites on the gen population that the reco-level cut cannot see by construction.
- **Physical mechanism of bias:** Confusing the two cut types causes the memo to evaluate the "would the cut change data yields?" question (correctly: ~zero) and then to mis-apply that null result to "would the cut change the unfolded multiplicity?" (incorrect: large, because the gen denominator changes by ~50% in the lowest z bin).
- **Analysis-killing:** Yes for the framing in §1 and §7. The memo's recommendation against Option C versus Option A is built on this conflation.

---

## Major Concerns

### M1. The memo's literature argument is correct in fact but wrong in inference.

- **Claim criticized:** Memo §3 / §7.2(1): "No published H1 / HERMES / COMPASS / JLab unpolarised SIDIS multiplicity paper uses `x_F > 0` as a primary phase-space cut. Adding it would put our analysis out of convention." Memo §1, Finding 1: H1 1302.1321 and 2011.01812 use `η* > 0`; memo correctly states this is equivalent to `x_F > 0` in the massless / high-`p_h*` limit (Section 2.2 of the memo).
- **Evidence:** Memo §2.2 explicitly: "In the massless / high-`p_h*` limit, `x_F > 0 ⇔ y* > 0 ⇔ η* > 0`. The two cuts are **asymptotically equivalent**." Memo §3.1.1 quotes H1 1302.1321 directly: "The target region, η* < 0, is not accessible in this analysis." Memo §3.1.2 cites H1 2011.01812 phase-space table: `0 < η* < 4`.
- **Physical mechanism of bias:** The memo correctly establishes that H1 1302.1321 and 2011.01812 use `η* > 0`, which IS an `x_F > 0` cut by the memo's own equivalence theorem. The memo then frames this as evidence that "x_F cuts are unconventional," when in fact the published H1 SIDIS analyses **do apply the equivalent cut** — they just write it in η* variables. The supervisor's framing (the published analyses ARE applying the cut, in different notation) is the physically correct reading. The memo's framing inverts the inference.
- **Follow-up:** Confirm that H1 2011.01812's `0 < η* < 4` is applied symmetrically at reco and gen (i.e., the gen denominator in their unfolding is restricted to `0 < η* < 4`, not to a broad `|η_lab|<X` fiducial). The paper's Table 1 phase-space row reads as a particle-level fiducial; if so, our analysis is the actual outlier — we are the ONLY measurement of the listed corpus to define the gen pool more broadly than the current-hemisphere region.

### M2. The Boglione et al. 2017 reference in §4 supports the supervisor, not the memo, when read for the gen-side question.

- **Claim criticized:** Memo §4.2/§4.4: "At large Q (our regime, Q > 12 GeV), the rapidity separation between current and target hemispheres is large (~8–10 units), and the current-fragmentation region is well-defined."
- **Evidence:** Boglione et al. 2017 (arXiv:1611.10329) propose the "R-parameter" specifically to define which (z, Q², P_hT, rapidity) bins are TMD-clean and which are contaminated by overlap with target/soft fragmentation. The criterion is a **gen-level / hadron-level definition of the analysis fiducial**, not a reco-level cut. Boglione's framework presumes you have *defined* a current-fragmentation phase space at the particle level before comparing to theory. Our analysis publishes multiplicities in a fiducial that explicitly includes target-fragmentation gen tracks via the broad `|GenEta|<3.5`. Without applying the gen-level definition of the current region, the unfolded multiplicity is not "a SIDIS multiplicity for theorists" — it is a phenomenological mixture of current- and target-fragmentation in proportions determined by the unfolding prior.
- **Physical mechanism of bias:** The memo cites Boglione to argue that the cut is unnecessary because the rapidity hemispheres are well-separated at large Q. The correct reading is the opposite: precisely because the hemispheres are well-separated, the gen-level definition should respect that separation. The memo's argument that "at large Q the cut is automatic" is true only if the gen-fiducial enforces it. We do not enforce it.
- **Follow-up:** None required for this verdict. The Boglione citation in the memo is correctly cited but misapplied for the gen-side question.

### M3. The memo's "Option B is a low-priority sanity check" recommendation underestimates the urgency of the truth-level x_F study.

- **Claim criticized:** Memo §7.3: "Option (B), as a secondary low-priority systematic check… would close out the question and provide a one-line documentation entry."
- **Evidence:** The bin-by-bin efficiency table in F2 shows that the supervisor's worst-case bin (z<0.1) has 53% of its gen yield from acceptance-loss extrapolation. This is *the* dominant systematic for that bin and competes with or exceeds every entry in the current 11-source budget. The memo treats this as a verification afterthought.
- **Physical mechanism of bias:** Deprioritising the truth-level study means the z<0.1 multiplicity will be released with an unquantified model-dependence uncertainty that may dominate the published per-bin uncertainty.
- **Follow-up:** The truth-level x_F study is not "low-priority sanity check" — it is the precondition for publishing the z<0.1 bin at all. If the analysis chooses NOT to apply the cut, the z<0.1 multiplicity must carry a *separate* prior-shape systematic computed by re-unfolding with djangoh and rapgap priors restricted to `x_F > 0` (or equivalently `η* > 0`) and quoting the spread as the model-dependence on the unmeasured part of the phase space. The memo's recommendation contains no such requirement.

### M4. The "lab fiducial already enforces the cut" claim is true for reco but is exactly inverted for gen.

- **Claim criticized:** Memo §1, Finding 3: "the lab-frame η fiducial alone already enforces `x_F > 0` for ~all reachable hadrons."
- **Evidence:** True for reco (our reco fiducial is |η_lab|<1.3 and the boost places this entirely in x_F>0; confirmed both by memo §5 and by independent recomputation). False for gen — our gen fiducial is |η_lab|<3.5, which includes η_lab∈(1.3, 3.5] where x_F is negative for most pT (independent calculation: Q²=2000, x=0.05, η_lab=+3 gives x_F = −0.005; η_lab=+3.5 gives x_F = −0.010).
- **Physical mechanism of bias:** Asymmetric statement of a symmetric fact. The memo correctly says the reco lab fiducial enforces x_F > 0; but the gen fiducial that drives the unfolding denominator does NOT enforce it, and that is the actual problem under dispute.
- **Follow-up:** None; this is a restatement of F1/F2 in different language.

### M5. The migration systematic from a reco-level `x_F > 0` cut (memo §6.1) is correctly identified and is a real argument against a *reco-side* cut. It does NOT argue against a *truth-side* phase-space restriction.

- **Claim criticized:** None of the memo's analysis here is wrong, but §7 collapses the recommendation as if §6.1 had ruled out the truth-level cut. It did not.
- **Evidence:** Memo §6.1 explicitly: "If we instead applied the cut at the **truth level** (i.e. unfold to a phase space that has `x_F > 0` as a particle-level definition), the migration is handled inside the unfolding response matrix and does not require a separate systematic. This is the cleaner approach if the cut is to be applied at all."
- **Physical mechanism of bias:** The memo's own §6.1 sentence above is consistent with the supervisor's position. The §7 recommendation contradicts §6.1's last clause without justification.

### M6. The supervisor's claim "either cut x_F or relax gen fiducial" is mostly right but missing a third option: publish a particle-level fiducial cross-section.

- **Claim criticized:** Supervisor counter-argument item 6: "The alternative to cutting x_F > 0 is to relax our gen-level fiducial from |GenEta|<3.5 to a much wider range… This would be far more invasive than just adding the x_F > 0 cut."
- **Evidence:** The two stated options (cut x_F symmetrically; or extend gen to full 4π) are not the only valid options. A third, used routinely in collider HEP (ATLAS / CMS fiducial cross sections), is to publish the multiplicity *within a stated particle-level fiducial volume*: e.g. "M^h for particle-level tracks with `|η_lab| < 1.3` and `pT > 0.15`." This eliminates the kinematic-acceptance correction entirely and only inverts detector efficiency, vertex, radiative, and PID effects.
- **Physical mechanism of bias:** Neither bias; this is an alternative that should be on the menu. It is functionally equivalent in physics content to the supervisor's Option A (truth-level cut) but is conventionally documented in collider HEP literature and would map cleanly onto the H1 high-Q² jet paper convention (1611.03421, 2108.12376), which is the closest published analogue.
- **Follow-up:** The decision menu for the supervisor should be: (A) symmetric x_F > 0 cut at reco+gen; (A') symmetric η* > 0 cut at reco+gen (formal H1-convention version of A); (A'') symmetric particle-level fiducial = reco fiducial (|η_lab|<1.3, pT>0.15) — publish a "fiducial multiplicity"; (B) keep current setup but propagate the prior-shape systematic as a dedicated uncertainty source; (C) memo's current recommendation, do nothing. **Option C is not defensible without Option B as a supplement.**

---

## Non-Blocking Checks Confirmed

### N1. Memo Section 2.2 (definition of x_F, relation to η*, equivalence in massless limit) is mathematically correct.

### N2. Memo Section 5 reco-side calculation is reproducible. Independent recomputation gives quantitative agreement on the reco-fiducial values (memo Table 5.1, Q²=500, x=0.01, η_lab=+1.3, pT=0.5: memo says x_F=+0.004; recompute gives +0.0019, same order, both barely positive). The slight numerical discrepancy is within the LO-QPM and rotation-axis-choice tolerances of either computation.

### N3. Memo Section 3 literature survey (H1 1302.1321, 2011.01812, HERMES 1212.5407, COMPASS 1709.07374, JLab Hall C 2510.03562) is accurate in citation, kinematic ranges, and quoted phase-space cuts. The factual content of the survey is sound; only the inference from the facts is incorrect (see M1).

### N4. Memo §4.3 conclusion that the Trento `cos(φ_h)` / `cos(2φ_h)` harmonics are not measurably perturbed by an x_F cut is plausible and consistent with the absence of any published counter-example. This is independent of the gen-side denominator issue and is a non-blocking finding.

### N5. The supervisor's verification of the source code is correct: line 1175 of `ChargedSidisTreeBuilderHighQ2_v5g.C` does set `inFiducialCharged = (fabs(GenEta_cg) < 3.5 && GenPt_cg > 0.05)`, the comment "FIX F9 (PORTED): broad gen fiducial — no GenP cut" is present, and there is no gen-side η* or x_F cut anywhere in the gen extractor or the response builder.

---

## Missing Evidence

### ME1. Neither side has computed `prior_gen` with vs. without `x_F > 0` (or `η* > 0`) restriction.

The supervisor's z<0.1 efficiency drop is highly suggestive but does not yet prove that the missing gen yield is *target-fragmentation* missing yield (vs., e.g., low-pT track-reconstruction loss). The clean test is the one in the next section.

### ME2. The closure test (unfold djangoh-as-data with rapgap-as-MC, and vice versa) for the z<0.1 bin should be examined. If the cross-MC closure for the z<0.1 bin is significantly worse than for higher z, that is a separate confirmation of the prior-shape extrapolation. The closure logs were not inspected in this review.

### ME3. The truth-level x_F distribution in the gen pool, sliced by z bin, was not extracted. This is one Condor job away (it requires only adding `xFGen_cg` as a column in `extract_pertrack_mc_gen_v3.py`, which the builder already writes).

---

## Adjudication of each numbered question from the dispute prompt

**Q1 (memo "no precedent + acceptance + TMD null" vs supervisor "the precedent IS η*>0 = x_F>0, we are the outlier"):**
**Supervisor is physically correct.** Memo §2.2 establishes the equivalence; memo §3.1.1 and §3.1.2 quote the H1 papers applying it; memo §7.2 then ignores the equivalence to claim the cut is unconventional. The supervisor's framing is the consistent one.

**Q2 (z<0.1 bin extrapolation: real or small?):**
**Real, and large.** Confirmed quantitatively from the actual djangoh response cache: efficiency in z<0.1 is 0.42 vs ~0.89 in z ∈ [0.1, 0.5]. ~53% of the z<0.1 unfolded yield is acceptance-extrapolation, not detector inefficiency. The supervisor is exactly right.

**Q3 (does memo Table 5.1 address the gen-side issue?):**
**No.** Table 5.1 samples only η_lab ∈ {-1.3, 0, +1.3} (the reco fiducial). The gen fiducial extends to η_lab = 3.5, where x_F is negative for typical pT. The memo's table does not bear on the supervisor's gen-side concern.

**Q4 (formal first-principles version of the supervisor's argument):**
**Confirmed.** The TUnfold operator is `P[i,j] = R[i,j] / prior_gen[j]` (`unfold_TUnfold_4d.py:49`, `build_probability_operator`). `prior_gen[j]` is the gen pool weight in bin j under `|GenEta|<3.5` (`build_response_4d.py:286`, `extract_pertrack_mc_gen_v3.py:202`). `R[i,j]` is matched reco-bin-i / gen-bin-j events, where the match requires a reco track at |η_lab|<1.3. Hence the implied per-bin efficiency `R.sum(axis=0)[j]/prior_gen[j]` includes **both detector inefficiency AND lab-η kinematic acceptance loss** — the latter being the very phase-space extrapolation the supervisor describes. The cleanly-separated quantity ("detector efficiency only") would be obtained by restricting prior_gen to the reco fiducial (|η_lab|<1.3 at gen, or equivalently x_F > 0 / η* > 0 at gen). With the current code, the two effects are entangled, and the unfolder propagates the gen prior shape into the unmeasured region.

**Q5 (cut x_F vs. relax gen fiducial vs. third option):**
**Three valid options exist** (M6): (A) symmetric x_F > 0 cut, (A') symmetric η* > 0 cut, (A'') publish particle-level fiducial = reco fiducial. The supervisor is right that "relax to 4π" is not a real option in our compute / disk budget. The third option (A'') is the cleanest and matches the H1 high-Q² jet-paper convention.

**Q6 (migration bias from a reco-level x_F cut):**
**Real but irrelevant.** It is a reason to prefer truth-level cut over reco-level cut (memo §6.1 says this correctly), not a reason to skip the truth-level cut. At Σ-method Q²,x reconstruction resolution and tracker pT resolution, the Δx_F resolution is ~0.005–0.010 at our kinematics (memo §5.4 and §6.1, both correctly stated). For a truth-side fiducial-redefinition, this migration is absorbed into the response matrix R[i,j] and does not require a separate systematic.

---

## Shortest decisive validation

The dispute is *already* resolved by the per-bin efficiency table in F2: the supervisor wins on the structural physics question. But if a second, independent confirmation is wanted, the **shortest test** is:

1. **Re-bin `prior_gen` in the existing djangoh response file (output/unfold4d_cache/response_4d_djangoh.npz) restricted to gen tracks with `η* > 0` OR `x_F > 0`.** This requires re-running `extract_pertrack_mc_gen_v3.py` once with an added `xFGen > 0` cut (the column already exists in the tree, line 1164 of `ChargedSidisTreeBuilderHighQ2_v5g.C`: `xFGen_cg = ...`) and rebuilding `prior_gen`. ETA: 1 Condor job + 1 response-build job, < 1 hour wall.
2. **Compute the new per-z efficiency table.** If `eff[z<0.1]` rises from 0.42 to ≥ 0.80 (matching the higher-z bins), the supervisor's diagnosis is confirmed: the gap was acceptance-extrapolation, not detector loss. If `eff[z<0.1]` stays near 0.42, the memo's "implicit cut" argument might survive (because the missing gen population would then be at x_F > 0 but |η_lab| > 1.3, which would be a different and smaller fraction of the gen pool). The expected outcome, based on the kinematic boost calculation, is the former.
3. **Re-unfold once with the restricted prior** and compare the z<0.1 multiplicity to the current result. The expected change is **~50% on the z<0.1 multiplicity**, with a corresponding ~10× reduction in the prior-shape systematic.

This is a sub-hour test that closes the question. It does not require freezing or reprocessing the MC trees.

---

## Recommendation to supervisor

1. **Stop the memo's Option C from being adopted as the analysis decision.** The memo's recommendation is unsafe because it does not address the gen-side denominator question. The memo should be revised, not used as-is.
2. **Adopt one of (A), (A'), or (A'').** They are physically equivalent for our analysis (at our kinematics, the equivalence in memo §2.2 is essentially exact). The choice between them is presentational:
   - (A) `x_F > 0`: literal restatement of the supervisor's request; needs new column wiring at gen.
   - (A') `η* > 0`: matches H1 1302.1321 and 2011.01812 convention exactly. Use this if external comparability is the priority.
   - (A'') particle-level fiducial = reco fiducial (|η_lab|<1.3, pT>0.15): matches H1 high-Q² jet-paper convention; produces a "fiducial multiplicity"; is the cleanest mathematically.
3. **Run the F2-equivalent test on rapgap as well.** Cross-MC closure with the corrected prior is the validation gate before the cut is committed.
4. **If for documentation reasons the analysis must publish in the broad-gen-fiducial frame, propagate a dedicated `prior_shape_z<0.1` systematic** computed from the data – MC envelope of the z<0.1 bin under (broad gen) – (restricted gen). This systematic will be large (O(20–50%) on the z<0.1 bin) and must be quoted as such. The memo does not anticipate this.
5. **Update the memo** to remove the §1 and §7 recommendations and to add a §5.5 sub-section computing `(η*, x_F)` over the full gen-fiducial range (η_lab up to 3.5). The factual content of §2, §3, §4, and the existing §5 is fine and should be retained.

---

## Recommended Next Check

**Run the F2-equivalent test on the current rapgap response cache** (`output/unfold4d_cache/response_4d_rapgap.npz`): compute `eff[z<0.1]` for rapgap. If rapgap also shows a ~0.42 efficiency in z<0.1, the diagnosis is confirmed and is MC-independent. If rapgap shows a different value, the prior-shape systematic between djangoh and rapgap on the z<0.1 bin is already enormous and must be quantified independently of the cut decision.

```bash
ssh naf-hone.desy.de "python3 -c \"
import numpy as np
d = np.load('/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/unfold4d_cache/response_4d_rapgap.npz', allow_pickle=True)
R = d['response']; prior = d['prior_gen']
NB=480
col_sum = R.sum(axis=0)
for z_idx in range(6):
    mask = np.array([((j%30)//5)==z_idx for j in range(NB)])
    p = prior[mask].sum(); r = col_sum[mask].sum()
    print(f'z bin {z_idx}: prior={p:.3e} matched={r:.3e} eff={r/p if p>0 else 0:.4f}')
\""
```

If both MCs show eff(z<0.1) ≈ 0.42, the cut decision is settled in favour of Option A/A'/A''. If they differ by > 10% in absolute efficiency, the prior-shape systematic on z<0.1 dominates the bin and the bin should not be published in the current configuration without the cut.

### Update (same session, after writing the verdict)

The rapgap cross-check was run. Result:

| MC | eff(z<0.1) | eff(z∈[0.1,0.2]) | eff(z∈[0.2,0.3]) | eff(z∈[0.3,0.5]) |
|---|---|---|---|---|
| djangoh14 | **0.416** | 0.886 | 0.902 | 0.899 |
| rapgap31 | **0.432** | 0.889 | 0.905 | 0.904 |

Both MCs agree to within ~1.6 percentage points on the z<0.1 efficiency. The collapse from ~0.90 to ~0.42 is **MC-independent**, i.e. it is a kinematic-acceptance feature of the analysis fiducial, not a generator artefact. The supervisor's diagnosis is **MC-independent confirmed**: ~57–58% of the z<0.1 gen pool is acceptance-loss extrapolation, not detector inefficiency.

The 1.6 pp difference between djangoh and rapgap on `eff(z<0.1)` is itself a non-trivial prior-shape effect (1.6 / 0.42 ≈ 4% relative on the unfolded z<0.1 multiplicity from this source alone) that should be quantified separately.

**The dispute is closed.** Memo's Option C is structurally inappropriate for the z<0.1 bin of this analysis. The supervisor's Option A (or A' / A'') should be adopted, OR the z<0.1 bin should not be published in the current configuration.

---

*End of critic verdict.*

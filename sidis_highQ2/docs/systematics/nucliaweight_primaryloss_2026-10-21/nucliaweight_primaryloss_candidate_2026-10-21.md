# NucliaWeight primary-loss uncertainty — DIAGNOSTIC candidate (2026-10-21, NOT installed)

**Status:** diagnostic candidate. It is NOT installed and NOT quotable. It does not change STATE.md's quoted budget
(`total_quoted_with_nonprimary_v5_rel`). Installing it needs a user decision plus critic review.

## What was varied
- `NucliaWeight` (`H1NuclIACor::GetWeight = 1 − ani_ineff`, K. Daum; based on a ρ analysis from 2005-11-16; the routine
  documents NO uncertainty) is an MC-only per-track loss correction, applied as `w_reco = w1*w2*wn` on body and fake legs.
- Variant: the deficit is scaled, `wn → 1 − (1+κ)(1−wn)`, with κ = ±1 (±100%, the house fiat convention as for env100/int).
- **Adopted form, "loss-to-miss":** a changed loss rate turns lost tracks into misses, so B → B − κD_B, F → F − κD_F and
  miss_j → miss_j + κ Σ_i D_B[i,j]. Here D = R(κ=0) − R(κ=+1) is exact because the response is linear in κ.
- Builder: `scripts/unfolding/nucliaweight_variant_2026-10-21/build_migration_nucliaweight_variant.py`. It is a copy of
  the trackclass builder (md5 c9af59df…) with a 3-line diff (NW_KAPPA; meta key). Condor 5815153 (174 shards,
  κ=+1, null-config classes) and term job 5815199.
- Unfold: lean IBU k=4 on the official data_sub′ (it reproduces the official OPT-A release to < 1e-12, asserted).

## Validation
- κ=0 builder vs the official OPT-A shard rapgap31_0000: bit-identical in 68/68 histograms.
- Linearity: R(0.5) − (R(0)+R(1))/2 = 2.6e-15 relative. All 174 variant shards carry meta nucliaweight_kappa = 1.0.
- The miss row is independent of NW (asserted exactly 0). Total reproduction max abs = 0. The term is finite and nonnegative.

## Results (RAPGAP31 nominal response; DJANGOH14 response in brackets)
| | h+ | h− |
|---|---|---|
| NW deficit, share of body | 1.036% [1.013%] | 1.046% [1.022%] |
| integrated shift κ=+1 / κ=−1 | +1.179% / −1.147% | +1.183% / −1.151% |
| per-bin term median / p95 / max | 0.76 / 2.06 / 4.59% [0.73 / 2.06 / 4.71] | 0.78 / 2.33 / 5.56% [0.74 / 2.36 / 5.78] |
| MC stat on the shift (audit only), integrated / per-bin median | 0.0013% / 0.006% | 0.0014% / 0.006% |
| of which fake-leg (f_i) part | +0.148% | +0.144% |
| quoted total median v5 → v5+NW | 7.4475 → 7.4848% | 7.7992 → 7.8604% |
| total increment median / p95 / max | +0.036 / 0.131 / 0.239 pp | +0.036 / 0.137 / 0.251 pp |

- Charge-symmetric: h+/h− integrated ±0.004% (per-bin median 0.009%); A_h ±0.00002. Generator spread is ≤ 0.03% integrated.
- Engine-native form (miss row unchanged): only 0.61× the loss-to-miss shift, because the engine prior
  P = colsum(B)+miss moves with B. It is reported as `_info` and not adopted.

## Central-value finding (NOT an uncertainty; not ours to fix)
The nominal NW deficit is not booked into the miss row, and the engine uses P = colsum(B) + miss. So the IBU efficiency is
`nC/(nC+M)`, which is the form the 2026-09-09 supersession of `docs/results/nucliaweight_fix_2026-05-31.md` rules
physically wrong ("biases results LOW by ~0.45%"). The 2026-10-05 coherent-prior adoption (prior := response column marginal)
re-introduced it. Moving the already-applied deficit to the miss row shifts the OPT-A nominal by
**+0.453% (h+) / +0.453% (h−)** integrated (DJ response +0.448%), matching the predicted 0.4502%/0.4515%.
It is charge-symmetric, so it cancels in h+/h− and A_h. Per-bin values are in column `nucliaweight_coherence_centralshift_rel_info`.
Related: the 2026-10-20 removal-check audit's "expected O(0.45%)" NW effect is this partial-propagation number.

## Open decisions (user)
κ (100% fiat as here, 50% H1-style, or derived); correlation with `nonprimary_norm_v5` int (treated as independent here,
which is conservative); overlap with `tracking_eff_m0p5_rel`; (anti)proton absorption (NW = 1 for protons); whether to
adopt the loss-to-miss coherence fix in the nominal.

## Products
- Candidate budget: `H1ChargedSidis_HighQ2/output_v6/studies/nucliaweight_variant_2026-10-21/nucliaweight_term/`
  (`budget_v5_plus_nucliaweight_candidate.csv`: v5 columns verbatim + NW columns; `summary.json`).
- Plots: `sidis_highQ2/plots/systematics/nucliaweight_primaryloss_overlay_2026-10-21/` (binmap labels; y 0–5% and 0–15%).

## Degeneracy check against the v5 budget (2026-10-21)
Scripts: `degeneracy_check.py`, `degeneracy_check_lsq.py` (local `scripts/nucliaweight/`). Signed vectors come from the
covariance edition's `delta_k_columns.csv` and from the budget's signed columns.

- **`bgsub_fakefrac_rel`: DEGENERATE with the fake-leg piece (same mechanism).** That term is `|1 − r_min|·f_i`, where
  r_min = 0.9817 is the template (no NW) / fake-column (with NW) ratio, attributed to NucliaWeight in its spec. So it
  already books the NW deficit on fake legs at ~100%. The candidate's fake-leg piece (κ·D_F) has |fake-leg| vs fakefrac
  r = 0.64 and a median ratio of 0.50. **Recommendation: drop the fake-leg piece (body + miss only).**
  - Linear split: per-bin median 0.76/0.78% → 0.65/0.67%; integrated ≈ 1.18% → 1.03%.
  - Total median 7.4848/7.8604% → 7.4728/7.8524%.
  - The exact body-only re-unfold is pending.
- **`tracking_eff_m0p5_rel`: no shape relation; possible conceptual overlap.**
  - Profile correlation r = 0.04. The term is booked data-side and bin-uncorrelated (diagonal cov), so the two cannot
    trade off in the covariance.
  - The H1 0.5%/central-track number (DESY 20-176; 1% in 1302.1321) is a generic tag-and-probe data/MC efficiency
    uncertainty. Whether it was derived after the H1NuclIACor correction is not documented in this workspace.
  - If it was, keeping both is a conservative partial double count of ≤ 0.5% coherent.
- **`nonprimary_norm_v5` int: related driver (material-interaction rate), different legs.**
  - Lost primaries vs produced secondaries: not a double count once the fake-leg piece is dropped.
  - A common rate change moves them in opposite directions, so treating them as independent is conservative.
    Shape-only cosine 0.27.
- **lumi / env100_hyp: no physical degeneracy.**
  - 70% of |NW|² is flat (normalization-like). Raw cosines of 0.84 / 0.68 come entirely from that, and shape-only
    cosines are 0.000 / 0.006.
  - Caveat: in a nuisance-parameter fit to absolute cross sections, the flat part of NW and lumi are indistinguishable
    (both are normalizations). This does not matter for the quadrature budget.
- **ibu_model, detint_removal_model (DJ vs RG): no overlap.** Both generators share NW; the NW term with the DJ response
  equals the RAPGAP one to ≤ 0.03%.
- **Joint fit on all 13 v5 signed vectors:** shape-only R² = 0.30, with no single cosine above 0.42 (c_rad_gen, coincidental).
  The raw R² of 0.78 is the flat part absorbed by lumi.

## User decision (2026-10-21, after the degeneracy check)
"Let's leave both in to be conservative": the term is kept **as built** (body + miss + fake-leg piece).
`bgsub_fakefrac_rel` and `tracking_eff_m0p5_rel` are also kept unchanged.
- This is a **documented conservative double count.** The fake-leg piece (per-bin median ≈ 0.10%; integrated +0.15%)
  overlaps `bgsub_fakefrac_rel`, and the term may partly overlap the H1 per-track tracking number (≤ 0.5% coherent).
  The precedent is the STR-μ / ibu_model overlap in v5.
- Candidate column unchanged: `nucliaweight_primaryloss_rel`. Total `total_quoted_with_nonprimary_v5_nw_rel`, medians
  7.4848% / 7.8604%.
- Next, per house practice: a critic review, then a covariance edition (rank-1, ρ = 1 across charges), then the install,
  which needs user sign-off.

## Critic round 1 (2026-10-21): implementation PASS-WITH-CONDITIONS, physics SOUND-WITH-CONDITIONS, no blockers
Reviews: `nucliaweight_critic_implementation_2026-10-21.md` and `nucliaweight_critic_physics_2026-10-21.md` (this directory).
The implementation critic reproduced every number independently to ≤ 2e-15, and found all 98 v5 columns byte-identical.

**Corrections and additions to this record (supersede the wording above where they differ):**
- **Convention invariance (physics F1):** the term does not depend on the nominal's NW convention. Re-evaluated around a
  nominal with the deficit moved into the miss row, it agrees per bin to 6e-15; this follows from y_j ∝ P_j·h_j(B, d) in
  the engine. Adopting the +0.453% fix later does NOT require regenerating the term (the implementation critic's MINOR-3
  expected < 0.01%; measured 6e-15).
- **The term does NOT cover the +0.453% nominal low bias.** That bias is a separate open central-value defect (F2).
- **Ratio and A_h (impl MINOR-2, physics F3):** the NW ratio/A_h figures (h+/h− ±0.004%, A_h ±0.00002) are
  **zero by construction**, not an uncertainty on those observables.
  - `ani_ineff` is charge-blind (L44-67), and protons/antiprotons get NW = 1.
  - The charge-asymmetric part of primary loss (p̄/K− absorbed more than p/K+; house estimate 0.1–0.5% at low pT, and
    O(0.001–0.003) on A_h from the proton omission) is NOT covered.
  - Cross-charge ρ = 1 is justified only for this charge-blind correction. The v5 int term uses ρ = 0 because its
    source content is charge-split.
- **Overlap wording (physics F7), replacing "DEGENERATE" and "not documented":**
  - `bgsub_fakefrac_rel` is a fc0 common-model proxy that carries an NW caveat. It also covers e/other/det.int. model
    dependence, and |1 − r_min| = 1.83% is 155% of the 1.18% NW fake-leg deficit. Overlap with the fake-leg piece:
    **partial, unproven**.
  - The house 1302.1321 tracking source states its 1.0% covers "central tracking / vertex / weak-decay /
    nuclear-interaction". The installed `tracking_eff_m0p5_rel` is a different construction (random-drop ensemble,
    diagonal, ρ = 0). Overlap: **partial, unproven**; "≤ 0.5% coherent" overstated it.
  - The user's keep-both decision stands.
- **Known omissions:**
  - The subtracted B_R′ templates exclude NW by convention, so they do not move with κ. The missing piece has the same
    sign as the term, ≲ 0.07% integrated and a few tenths of a percent in the highest-background bins (physics F8).
  - Rank-1 with a single κ is normalization-like and does not model p/θ shape errors of the per-track correction. Its
    flat part is degenerate with lumi in a fit (F9).
- **Clip before scale (impl MINOR-1, physics F10):** wn is clipped to [0, 10] before scaling, so wn < 0.5 gives negative
  weights at κ = +1. 1–2 negative body cells survive; capping them changes the term by ≤ 1.7e-5. Clip after scaling in
  any future rebuild.
- **Linearity (impl MINOR-4):** 8.7e-15 over all non-w2 histograms (2.6e-15 was a subset).
- **Covariance convention (physics F6):** use a rank-1 vector sign(s(+1))·term, so its diagonal equals the budget
  column, with ρ = 1 across bins and charges. max(|s+|, |s−|) is conservative; |s+ + s−| has a median of 0.011%.
- **κ context (physics F5):** ±100% is a fiat choice. At ±50% the integrated term is ≈ 0.59% and the median total
  increment ≈ +0.01 pp. The geometry table in `ani_ineff` is valid up to run 436888.

**Conditions still open (need the user):**
1. Explicitly ratify κ (±100% or ±50%).
2. Log the +0.453% bias in STATE and the register as an open decision.
3. For the charge-asymmetric/proton primary loss: book a charge-split piece, or register an open gap for h+/h−/A_h.

**At install:** build the covariance edition (sign(s+1)·term, gates re-run), then update the register entries
(`unpropagated_sources.json` L81-84, v5 addendum L107, STATE, RUN_STATUS, current-release) via sha-guarded edits with
fresh re-reads.

## User decisions on the critic conditions (2026-10-21)
1. **κ = ±100% ratified.** Physics condition F5 is closed.
2. **The +0.453% nominal bias is not logged in STATE or the register.** The user judges it negligible against the 6–7%
   total. It stays documented in this record only (as a central-value finding, not an uncertainty).
3. **Charge-asymmetric / proton primary loss.** The user notes that the measurement is charge-split. Assessment:
   - **Per-charge cross sections: covered.**
     - Central value: the responses are per charge (GEN-charge dispatch), and H1SIM/GEANT simulates species- and
       charge-dependent hadronic interactions (p̄ annihilation, K−/K+). So the asymmetric absorption is already in each
       charge's efficiency; NW is a charge-blind data/MC correction on top of it.
     - Uncertainty: the term assigns each charge its own ±1.18%, which bounds an asymmetric mis-modelling of that size.
   - **Not covered by the charge split:**
     - (a) Derived h+/h− and A_h. With ρ = 1 the term cancels, so a GEANT charge-asymmetric mis-modelling is not
       represented. That is a correlation statement, which the per-charge split cannot supply.
     - (b) Protons: NW ≡ 1, so they get zero variation.
   - **Proton bound** (one file, DJ14 7072 em06_0000, script `proton_bound.py`): protons with the kaon deficit at the
     same (z, lab pT).

     | | h+ | h− |
     |---|---|---|
     | proton share of body | 4.79% | 4.30% |
     | integrated | 0.033% | 0.029% |
     | cell median | 0.056% | 0.047% |
     | cell max (lowest-pT cells) | 0.40% | 0.67% |
     | cell max ×1.5 | 0.60% | 1.0% |

     Negligible in quadrature against the totals.
   - Disposition: no extra term. A caveat goes with any h+/h−/A_h use of the covariance (the NW contribution there is
     zero by construction).

## Covariance candidate edition (2026-10-21, NOT installed)
`H1ChargedSidis_HighQ2/output_v6/studies/nucliaweight_variant_2026-10-21/covariance_candidate/z005_covariance_optA_nonprimary_v5_nw_2026-10-21/`
- An edition of the installed v5 edition: C_total_new = C_total_v5 + `cov_nucliaweight_rank1`.
  - d_i = sign(s_i(+1)) · term_i · σ_i: rank-1, ρ = +1 across bins and charges. No bin has a negative sign; the pos/neg
    vector cosine is 0.99998.
  - The other 98 files are byte-identical copies.
  - The unpropagated-sources addendum records the scope caveats (charge-asymmetric, protons, B_R′ templates,
    normalization-like shape, and the kept double counts).
- Builder: `build_nw_covariance_edition.py` (Condor 5816021).
  - A first run (5816020) crashed on a numpy-bool JSON print after writing; that output was deleted and rebuilt.
- **Gates: 9/9 PASS.**
  - g0: 0.0 / 0.0
  - g1: rank 1, PSD
  - g2: diagonal change == budget change to 2.2e-16
  - g3: 1.6e-15
  - g4: 2.2e-16
  - g5: 98 protected inputs unchanged
  - g6: 0.0
  - g7: ρ = +1
  - ga10: no bitwise duplicates; top |cos| = lumi 0.84, env100_hyp 0.68, int 0.53 (all normalization-driven; see the
    degeneracy check)
- **Headline C_total medians with lumi:** h+ 7.3655 → **7.4016%**; h− 7.5999 → **7.6525%**.
- Budget quoted-total medians (candidate CSV): 7.4475 → 7.4848% / 7.7992 → 7.8604%.

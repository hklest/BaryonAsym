# Path A fitnorm_2026-10-17 — hostile physics-correctness review (read-only)

Reviewer: h1-analysis-critic (session ses_eb58d2e68ffeGrJYZC5IbSSE8A). Persisted verbatim by supervisor 2026-10-17.

**VERDICT: CONCERN.** Artifact integrity PASSES (one reported mismatch against the review brief), but the fitted mu's are **not valid measurements of the cone-kept (Set-R') e/V0 template normalizations**, and the delivered systematic term is **not acceptable as-is**. The in-control-region to kept-region transfer is **(c) unjustified extrapolation**, not an upper bound and not a measurement. Decision forced on the supervisor: **REJECT-AND-REMEASURE** (named re-measurement below), with **NEED-CONDITIONAL-VERDICT** for any interim use.

- Reviewed: observable = per-class (e, V0) template-normalization factors mu and the projected per-bin systematic term; channel = high-Q2 charged-hadron SIDIS, cone+S2'+Set-R' clean release; family = sidis_highQ2 backgrounds/systematics.
- Evidence: spec `bgsub_fitnorm_spec_2026-10-17.json`, `STATUS.md`, the four scripts, results JSON, mu-table CSV, term CSV + summary JSON, merged `fitnorm_validation.json`, condor joblogs, released vectors CSV (`bgsub_setrprime_vectors_980.csv`), `alpha_cone_baseline_spec_2026-10-11.md`, Set-R' cone spec 2026-10-15, `STATE.md` 2026-10-17 Clean-Release Promotion Override. The plots PDF was NOT opened (existence only, per review instruction).
- Reviewer toolset is read-only text (no shell): filesystem mtimes could not be stat'ed; ordering is established from in-file timestamps only.

## 1. Artifact integrity — PASS, one mismatch reported

- All named artifacts exist (spec, results JSON, mu table, term CSV, term summary; plots PDF present; merged validation JSON also present).
- Ordering (in-file evidence): spec sealed 2026-10-17 16:32:18 +0200 (STATUS) precedes Condor cluster 5732026 first submission 16:41:49 (joblog `fitnorm_data_data_shard_000.log`); the fit consumes the merged partials of that cluster; the term CSV is the rebuild whose log (`logs_term_rebuild_2026-10-17.log`) medians match the summary JSON. Consistent with spec -> extraction -> fit -> term. Direct mtime statting was impossible for this reviewer; flagged as a residual (minor) gap.
- mu table vs results JSON: all 8 groups match to full precision (mu_e, mu_V0, sigma_e/V0 minus/plus, chi2, ndf, flags).
- mu table vs term CSV: all 16 group factors equal hypot(|mu-1|, sigma(mu)) exactly (hand-verified for all 8 groups).
- Term spot-recomputation against the released vectors CSV (e_avg, v0_avg, data_sub_prime), by hand:
  - h-,349: 0.574361617693128*1.8085677267649674/2.1077611963954253 + 1.000909293685814*0.08367107683960755/2.1077611963954253 = 0.492832 + 0.039733 = 0.532565 vs CSV 0.5325646214540062 OK (hottest delivered bin)
  - h+,0: 0.118621 vs CSV 0.11862145621716144 OK
  - h-,0: 0.090123 vs CSV 0.09012323632901137 OK
  - h+,68: 0.162251 vs CSV 0.16225082276221567 OK
  - h+,349: 0.206254 vs CSV 0.20625367463875052 OK
  - NaN-guard h-,79: released vectors row is all-zero (n_raw_cone=0, data_sub_prime=0.0, empty_bin_guard=True) -> NaN term with factors retained OK
- 980 term rows; 14 NaN guards (6 h+ / 8 h-), all with empty_bin_guard=True OK; medians h+ 3.164% / h- 2.546% OK; all 16 nominal |mu-1| > 2 sigma flags true OK.
- **MISMATCH REPORTED:** the review brief's "hottest f=31.858%" appears nowhere in the delivered artifacts (grep over the whole study directory). The actual hottest term_total_rel is **53.256% (h-,349)** — an n_raw_cone=4 near-empty bin; the next hottest is 20.626% (h+,349, n=9). The delivered maxima live in near-empty bins where the guard (data_sub_prime <= 0) does not protect against tiny-denominator inflation.

## 2. Transfer validity — UNSOUND as a measurement; category (c)

- The spec contains **no transfer assumption**. `control_population.rationale` justifies only signal-depletion; the `quoting_note` *asserts* the mu are corrections to the packaged cone-applied templates — an assertion, never demonstrated.
- The brief's premise ("mu measured on the cone-VETOED population") is contradicted by the artifacts' own diagnostics: the control population is 394,956 = **371,023 cone-ACCEPTED |DcaPrime|>0.25 tracks (93.9%)** + **23,933 cone-rejected (6.1%)** (merged validation diagnostics; also visible in the fitted data counts: h+_S0 DcaPrime bins below 0.25 hold ~5.2k tracks vs ~116k above).
- The mu's are therefore a **blend** of (i) the out-of-cone Dca tail (where V0 daughters live) and (ii) an in-cone component that the cone baseline spec itself documents as carrying a **x2.3-2.6 data/MC excess** attributed to "electron-induced data tracks (unmatched fragments etc.) that the MC templates/response do not describe at the measured rate" (alpha_cone_baseline_spec Sec 0.2). The population actually being corrected — out-of-cone, small-|DcaPrime| (prompt) tracks that dominate e_avg/v0_avg — enters the fit region only through its Dca tail.
- Applying sqrt(|mu-1|^2 + sigma^2) to the out-of-cone class fraction is thus **neither (a) a demonstrated upper bound** (nothing establishes alpha- or Dca-flatness of the data/MC ratio; where the ratio IS known, in-cone, it is x2.3-2.6, not 1.4-1.6) **nor (b) a valid measurement**; it is **(c) an unjustified extrapolation**.
- The alternative — fitting the out-of-cone |DcaPrime|>0.25 tail only (kept-region / subtracted class) — was **neither pre-registered nor precluded with a reason**; the spec simply does not consider it. Note the out-of-cone tail is already 94% of the delivered control region, so the re-measurement is a redesign of the same data, not new data taking.

## 3. Template soundness — inputs verified; photoproduction NOT removed (explicit ruling)

- Luminosity scales: `run_fitnorm_fit.py` hard-codes RG31 0.03843251824641288 (L_MC 9132891.0) and DJ14 0.03346843073584302 (L_MC 10487495.0) to L=351000.0, sum-then-scale-once on pooled sums. Grep of the entire study directory for forbidden values (0.0221963651 / 22.1963651 / 15813.4 / 0.08013817556653238 / 297657.8 / 356613.0): the ONLY hit is the spec's own `forbidden_values` list. **No STOP-level defect.** (The djangoh14 input list legitimately contains 7091 FILES; the 7091 SCALE is not used.)
- lowQ2: fixed at template, not fitted, factor 0 in the term (spec `classes.lowq2`; build script). Caveat: the lowQ2 class is entirely absent from the fit templates (N_CLS=3), so any control-region lowQ2 content is absorbed by the mu's — negligible in the kept region (0.0054% of D_cone), unmodeled in the control region.
- **Photoproduction/beam-gas ruling: NOTHING removes it by construction.** The extractor applies only DIS event cuts (Q2es in [150,20000], y in [0.1,0.7], Empz>45, |PVz|<35) and track cuts; templates are RG31/DJ14 DIS only; the data control histogram is the TOTAL control population; the fit has no photo/beam-gas template. IsFromPrimary==1 + PVz suppress vertexless beam-gas tracks, but quasi-real/photoproduction events with a good vertex pass into the control region and are absorbed into mu_e/mu_V0 or the misfit. The released chain itself acknowledges this class is unquantified (register rows 18/19 DIFFVM/photoproduction BLOCKED, systematic-only; `gammap_bkg_30pct_rel` never included). **Therefore the delivered mu's are upward-biased compound absorptions of e/V0 misdescription + non-DIS contamination + signal-tail misdescription; they cannot be read as e/V0 normalization measurements.** The data-excess *finding* survives; the *attribution* does not.

## 4. Interpretation — only (ii) is demonstrable; the term omits its own dominant systematic leg

- Distinguishability from the delivered artifacts: **(ii) template-shape mismatch forcing normalization up is demonstrably present** — nominal chi2/ndf = 31.4 / 13.3 / 4.2 / 0.88 (h+ S0..S3) and 12.4 / 5.2 / 1.6 / 0.67 (h- S0..S3) with shapes fixed; of the 48 variation fits, at least 19 hit the mu=0 bound and NHitDedx fits reach chi2 ~1878 — several "variations" are not valid alternative fits at all. **(iii) contamination** is not testable inside the artifacts but externally supported (cone spec's in-cone x2.3-2.6 fake excess; blocked photoproduction register). **(i) genuine V0 underdescription** cannot be separated from (ii)+(iii). **(iv) truth-flag misassignment** cannot be tested (no per-PDG breakdown delivered). Conclusion: the 16/16 flags prove "the fixed-shape model is inconsistent with the control data", NOT "the e/V0 template normalizations are underestimated by these factors".
- Spread accounting: sigma(mu) (profile, MC-stat-penalized, nominal AVG fit) does **not** contain the template-variation spread; sigma and spread are disjoint — **no double-count**. The delivered factor hypot(|mu-1|, sigma) **omits the spread leg entirely**. The pre-registered XOR term rule (factor = |mu-1| if flagged else sigma) also omits it: spec rule (ii) defines a spread that no delivered term formula consumes — an internal inconsistency of the pre-registration, inherited by the delivery. The delivered formula deviates from the sealed XOR (documented supersession in STATUS stage-4 note and the build-script header; slightly more conservative than XOR because all 16 flags are true). Either way the delivered medians 3.164%/2.546% are **not an envelope of the fit's own systematics**: folding in the delivered spreads (spread_V0 1.16-2.99, spread_e 0.74-1.40) would raise the factors by ~1.5-3x (e.g., h+_S0 e: 0.536 -> 1.50; h-_S3 V0: 2.037 -> 3.61). Counter-caveat: because many variation fits are pathological (bound hits, chi2~10^3), the spread is itself not a clean systematic — the honest statement is that **this fit does not determine the e/V0 normalizations at any usable precision**.

## 5. Consequence audit — decision forced

- If mu were a valid measurement, the correct action would be a **central re-derivation, not a term**: applying charge-averaged mu_e ~ 1.46 and mu_V0 ~ 2.25 to the e 3.8543% / V0 3.4166% shares would move B_R' from 7.2767% to ~13.3% of D_cone, i.e. data_sub' down ~6.5% relative -> Born h+ 11.996558 -> ~11.2 nb, h- 11.663956 -> ~10.9 nb. **RE-DERIVE-CENTRAL is NOT supported** (Sections 2-4).
- **REJECT-AND-REMEASURE is the forced primary decision**, with the named re-measurement: an **out-of-cone-only |DcaPrime|>0.25 tail fit per group** (drop the in-cone component) on the kept-region/subtracted class, **with the signal-tail degeneracy confronted** (float the signal normalization or validate the MC signal Dca tail against data) **and a photoproduction sideband bound** (y/Empz or non-fiducial-Q2 sideband) to close interpretation (iii).
- The **flat-30% fallback is indeed not demonstrably conservative** (the brief's 0.948%/1.067% figures are not in the fitnorm artifacts): the released `bgsub_model_rel` leg carries only the generator half-spreads (medians 0.202%/0.228% including lowQ2 +/-30%; Set-R' spec `uncertainty_components`; the Rev-2 transfer rescale was explicitly RETIRED — "NO transfer correction is consumed for e/V0"), an order of magnitude below any reading of this fit. **All available options likely understate the e/V0 normalization uncertainty**; the released e/V0 legs should be treated as suspect pending the re-measurement.
- **NEED-CONDITIONAL-VERDICT** for any interim use of the delivered term. Conditions:
  - C1: re-fit on the out-of-cone tail only; require mu stability against the delivered blend.
  - C2: signal-template normalization floated or externally validated; require mu stability under it.
  - C3: a quantified photoproduction/beam-gas bound on the control region.
  - C4: goodness-of-fit acceptance (chi2/ndf of order 1) for whichever fit is finally quoted.
  - C5: the term formula re-pre-registered to include (or explicitly justify excluding) the template-variation leg, consistent between spec and delivered code.

## Non-blocking checks confirmed

- Extraction reproduces the canonical selection and the released vectors: V1 980/980 exact (total 3,865,073), V2/V3 template reproduction <= 3e-16, V4 658/364/501 complete; `cone_rejected_of_base` 28,582 matches the cone baseline spec's 28,582 — strong cross-validation of the extractor.
- e-and-V0 overlap-ignored convention is consistent between fit and application (both templates double-count the overlap; released overlap 0.0029% of D_cone).
- Group map (within-charge bin//70 -> cells {0,1}=S0, {2,3}=S1, {4,5}=S2, {6}=S3) matches the spec; 14 guarded NaN bins excluded from medians; the per-class term projection matches the spec's `term_rule.total` intent.
- Poisson chi2 with Barlow-Beeston-lite MC-stat penalties implemented as pre-registered; scales applied once on pooled sums; no 7110/DjBkgd or 7091 scale anywhere in code.

## Missing evidence (for any future SOUND ruling)

- Per-population split of the control region (in-cone vs out-of-cone tail), fitted separately — the single most decisive missing artifact.
- Signal-class DcaPrime-tail validation (data vs MC) or a floating-signal fit variant.
- Any photoproduction/beam-gas estimate for the control region.
- Direct filesystem mtime stat (not possible with this reviewer's toolset; in-file timestamps only).

**Recommended next check:** the out-of-cone-only |DcaPrime|>0.25 re-fit per group with floated signal normalization (C1+C2). If mu_V0 stays ~2-3 there under a floated signal with a bounded photoproduction sideband, a V0 re-scaling decision becomes defensible; if it collapses toward 1, the delivered mu's are confirmed as blend/shape artifacts and the released half-spread legs stand pending a re-derived envelope.

---

# APPENDIX — Path A R2 follow-up review (same reviewer session ses_eb58d2e68ffeGrJYZC5IbSSE8A, persisted 2026-10-17)

**VERDICT: CONCERN** — R2 executed and reported correctly, but it is a FAILED MEASUREMENT, not a validation: it supports no central-value change and leaves the e/V0 templates unvalidated. All headline numbers verified (mu table, results JSON, photo note, term CSV/summary, rerun log, STATE.md).

1. DEGENERACY: model unidentifiable by construction. In |DcaPrime|, mu_V0 pinned at 0 bound (sigma 0.046-0.458) while mu_e 2.5-3.5 still leaves chi2/ndf=4.98; RG31/DJ14 agree (generator-independent). RadLength/NHitDedx give interior solutions at catastrophic chi2/ndf 30-217. C2 signal-float flips decomposition even in gate-passing h+_S3 (mu_e 1.27->0, mu_V0 2.81->6.31). No consistent decomposition across observables; no two-class Dca-tail fit separates e from V0.
2. PHOTO: consistent with all four observations, not demonstrated. Sideband bound 4549.7 = 2.85x fitted e+V0 sideband 1599.1 (loud 8/8) but covers only 8.5% of the 53,812 fitted excess: junk-like photo cannot explain the bulk. Ceiling on unmodeled tail content ~54k tracks ~1.4% of D_cone; junk floor 0.12%. Equally consistent: DIS displaced-track mis-modeling (mu_s=1.02-1.16, prompt class fine, displaced component off). Artifacts cannot discriminate.
3. B_R': reading (c) for the central value, bounded residual (b). Misfit confined to >0.25 cm tail; kept sample 90.4% small-Dca. Gate-passing groups consistent with mu=1 within errors. R1's 1.4-3.2 does not reproduce; the feared -6.5% central shift is DEAD. Residual: ~1.4% unexplained tail excess, composition unknown — potential completeness issue, not proven background.
4. DECISION — primary (iv): keep central values and 19-comp structure; do NOT merge the R2 term CSV (700/980 NaN exactly where the misfit lives; largest terms in S3 where mu=1 within 1sigma — backwards as coverage); RAISE e/V0 normalization uncertainty to a FIAT +/-100% envelope on (e_avg+v0_avg) content, quadrature with retained half-spreads (term medians ~3.2%/3.6%; totals 5.98%->~6.8%, 6.30%->~7.2%). Factor 2 = smallest round number covering every admitted estimate (R1 blend 1.4-3.2; R2 ensembles mu_e 0.28-3.60, mu_V0 0-2.44). 30% has no demonstrated coverage. R3-as-proposed REJECTED (floated 4th component in Dca-only fit unidentifiable; would need simultaneous multi-observable redesign with external photo/junk template). Path B can settle whether published values move when the tail is excluded; it cannot validate e/V0 normalization, identify the excess, or bound photo.
5. INTERIM QUOTING SENTENCE: "The e/V0 (Set-R') subtraction templates are MC-derived and remain unvalidated by data — dedicated control-region fits could not measure their normalizations (5/8 groups fail the pre-registered goodness gate; the remainder are consistent with unity within large errors) — so their normalization uncertainty is carried as a +/-100% fiat envelope on the e/V0 content, included in the quoted total; an unexplained |DcaPrime|>0.25 cm tail excess (<~1.4% of the cone sample, composition unknown) is under investigation and implies no central-value change."

Recommended next check: Path B (|DcaPrime|<0.3 rebuild) stability vs the demotion criterion in ruling 4.

# Hot adversarial review: coherent-prior IBU magnitude study and proposed central replacement

**Review mode:** read-only, adversarial. No ROOT was opened interactively, no code was rerun, and no analysis product was changed.

## Verdicts

- **V1 — implementation fidelity: SOUND (for the reported numerical study, not for release certification).** The baseline is independently compared bin-by-bin with the frozen release CSV, rather than merely with a locally generated baseline: maximum relative discrepancy is `3.311839017904824e-15` (`comparison/summary.json:3-13`). The driver reads the central response/data paths, fixes `LUMI=351000.0` and `K=4` (`run_coherent_prior_study.py:8-15`), uses tagged RAPGAP `M`, `P`, and data inputs (`:47-53`), and records the central response checksum and unchanged-before/after state (`validation/validation.json:7-14,83-88`). The four variants differ only in `(p_init,p_eff)` (`run_coherent_prior_study.py:53-60`); its response shape, 490-row, finite, and non-negative gates pass (`validation/validation.json:15-82`).

- **V2 — correctness of `prior := body+miss`: INSUFFICIENT EVIDENCE to call it the builder-intended canonical fix.** It is the only response-internally coherent post-hoc prior: the study defines `truth = B.sum(0)+miss` (`run_coherent_prior_study.py:49-53`), hence its IBU normalization has a response column marginal by construction. Moreover, the builder defines `w_gen=w1*w2` (`build_migration_th2_per_charge_v6.py:464-470`) and its intended `prior_gen_NWeff` weights a matched truth row by `w_gen*NW_partner` but a miss by `w_gen` (`:877-900`), matching the *weight rule* of the body plus miss marginal. But the only direct canonical object that would settle the equivalence, `h_prior_gen_*_NWeff`, is absent from the consumed ROOT (`comparison/prior_variants_comparison.json:53-58`), and there is no central-file proof that the key-based consumed/miss assignment is one-to-one and makes NWeff exactly equal to this merged `body+miss`. The builder’s own commentary names NWeff—not an inferred post-hoc marginal—as canonical for IBU/TUnfold and as the route to a “pure geometric acceptance” (`build_migration_th2_per_charge_v6.py:314-322,1008-1021`).

- **V3 — replacement decision: NO.** The single decisive missing item is a **complete coherent-response re-release**: rebuild the response from the immutable inputs, store `prior_gen_NWeff`, demonstrate binwise `prior_gen_NWeff == body+miss` under the exact production merge, then regenerate and validate every central-derived systematic/release artifact against that new nominal. A ROOT-only post-hoc swap cannot establish that the proposed prior is the canonical builder quantity, and it cannot preserve the validity of products whose nominal is explicitly the old plain-prior release.

## Fatal Issues

### 1. The proposal replaces a named builder-level canonical quantity with an unverified surrogate

**Claim attacked:** Because `w_gen=w1*w2`, `body+miss` is automatically the builder’s canonical `prior_gen_NWeff`, so the post-hoc swap is design-conform and can replace central.

**Evidence:** The favorable part of the claim is real: `w_gen` is explicitly `w1*w2` (`build_migration_th2_per_charge_v6.py:464-470`); matched response entries use `w_reco` (`:818-826`), with the matched partner NW extracted as `wr/(w1*w2)` (`:837-844`); misses use `w_gen` (`:852-875`); and the NWeff accumulator applies `w_gen*NW_partner` to a consumed key, otherwise `w_gen` (`:877-900`). This is the intended weight-level construction behind a truth marginal.

**Why that does not close the attack:** Equality requires more than the displayed scalar weights. It requires that every body contribution and exactly one NWeff truth-row contribution share the same resolved key, charge, fiducial decision, and merge treatment, and that no duplicate/collision or reconstruction-side pairing ambiguity changes the consumed-key map. The code stores `consumed_keys` as a dictionary keyed by rounded run/event/PDG/kinematics (`:790-795,829-844`); the review package supplies no central production audit demonstrating the necessary one-to-one property. The central ROOT does not contain the intended NWeff histogram, so the decisive binwise comparison cannot be made (`prior_variants_comparison.json:53-58`). The independent mismatch investigation correctly recommends a coherent rebuild or explicit retained NWeff, not an in-place/post-hoc release patch (`response_prior_mismatch_investigation_2026-10-04.md:46-50`).

**Physical mechanism:** The physical efficiency must normalize the matched weighted response by the same weighted truth ensemble. `body+miss` enforces response algebra, but it is not proven to be the historical builder’s intended truth ensemble without the direct NWeff object or a reproducible rebuild. Calling it “pure geometric acceptance” before that identity is verified risks silently changing the intended treatment of NucliaWeight in the missed population.

**Disposition:** Analysis-killing for promotion/replacement; not a criticism of the side-study numerical delta.

### 2. A new nominal invalidates the completed nominal-relative systematic/release chain unless it is regenerated

**Claim attacked:** The coherent result can replace only the central IBU numbers while retaining the existing shifted releases, tracking ensemble, phi_h budget, and provenance.

**Evidence:** The current state explicitly says central IBU/BBB and completed systematics, including tracking, are done (`STATE.md:14`). The tracking ensemble was validated by comparing each shifted response/release to the existing nominal central release (`context-handoff.md:69-108`). The adopted phi_h systematic likewise defines its nominal as the central release IBU k=4 (`STATE.md:53-57`; `context-handoff.md:140-145`). The coherent study changes 125/245 h+ and 129/245 h− bins by more than 1%, with maxima 3.583% and 3.463%, overwhelmingly from changing the efficiency prior (`comparison/summary.json:14-107`). It contains no systematic-product comparison, re-expression, covariance propagation, or nominal-reference update (and its JSON inventory has no such evidence).

**Physical mechanism:** Systematic shifts and their relative/absolute covariance are functions of the nominal estimator. Reusing old differences relative to a different estimator changes both central values and relative uncertainties; the phi_h envelope is especially exposed because the state record says it includes fork/prior contamination. A small integrated +0.1240% shift is irrelevant to binwise systematic consistency.

**Disposition:** Analysis-killing for a central replacement now.

## Major Concerns

### 3. Efficiency semantics are algebraically coherent for the proposed marginal, but “pure geometric” and no-double-counting claims remain unvalidated

**Claim attacked:** `epsilon = col_inner/(body+miss)` necessarily removes an unwanted NW reweighting and cannot double-count it.

**Evidence:** In the side study, `B` contains matched entries, and `eps=sum(B/truth)` because `A=B/p_eff` with `p_eff=truth` (`run_coherent_prior_study.py:25-32,49-56`). That is a well-defined efficiency for the response actually supplied. The builder describes the same intended NWeff denominator as removing the matched/miss NW asymmetry (`build_migration_th2_per_charge_v6.py:314-320,877-882`). However, no supplied per-bin NWeff/body+miss comparison, no unweighted efficiency control, and no truth/reco NW ledger establishes that the actual merged central matrix realizes this description. The central ordinary prior mismatch is 245/245 bins per charge (`response_prior_mismatch_investigation_2026-10-04.md:20-34`), demonstrating that apparently related accumulators cannot be presumed interchangeable after production.

**Physical mechanism:** With matched legs weighted `w1*w2*NW` and misses weighted `w1*w2`, a plain denominator makes the correction depend on the matched fraction and NW distribution. A coherent denominator is the right algebraic repair for this matrix; whether it is the intended *physics* efficiency depends on the exact NW role and production provenance, which this study does not establish.

**Disposition:** Follow-up required before V2 can become SOUND.

### 4. The baseline gate is strong, but it is not a full independent release replay

**Claim attacked:** `3.31e-15` proves every relevant central choice is reproduced.

**Evidence:** It does prove agreement to the frozen IBU CSV through all 490 bins (`comparison/summary.json:3-13`) and hashes the data-vector input (`validation/validation.json:3-6`). It also uses the same fake-fraction expression, `d=raw*(1-fake/(body+fake))` (`run_coherent_prior_study.py:49-53`), the release’s stated fake treatment (`_run/run_bbb_ibu_release.py:74-82`), and correct charge-major loops. Thus the data vector, charge mapping, k, luminosity, response path, and baseline numerical result attacks fail on the supplied record.

**Remaining limitation:** The hard-coded expected data hash has no shown immutable release-manifest provenance, and the study is an independent compact implementation rather than invocation of the frozen driver. Exact agreement makes an accidental charge swap, changed k, different fake subtraction, or self-only comparison implausible; it does not validate omitted uncertainty/covariance products or future release provenance.

**Disposition:** Follow-up only; it does not overturn V1.

### 5. Central-release contract and provenance cannot be rewritten by a study directory

**Claim attacked:** A read-only study output can become the central release merely because it corrects an identified defect.

**Evidence:** The actual frozen driver consumes the tagged plain prior in `ibu_unfold` (`_run/run_bbb_ibu_release.py:74-82`), and the current central release contract remains the dated plain-prior path (`STATE.md:3-14`). The study labels itself a “PROVISIONAL INTERNAL DIAGNOSTIC,” says no central product was modified, and omits background subtraction, C_rad, and Born-level claims (`README.md:1-9`; `comparison/summary.json:127`). The response-prior investigation requires a new named coherent response/prior, merge-time conservation assertion, and release QA (`response_prior_mismatch_investigation_2026-10-04.md:46-50`).

**Physical mechanism:** Release identity is part of reproducibility. A central replacement changes the nominal estimator and needs immutable inputs, exact script/provenance, regenerated releases/covariances/systematics, and validation—not a relabel of a derived CSV.

**Disposition:** Analysis-killing for a replacement now.

## Non-Blocking Checks Confirmed

- **Wrong-file/naming-trap attack failed.** The study’s ROOT/data paths match the frozen release driver’s `VAR`, output root, and migration ROOT (`run_coherent_prior_study.py:8-12`; `_run/run_bbb_ibu_release.py:10-29`). Its response checksum is recorded before and after (`validation/validation.json:7-10,83-88`).
- **Causal delta attack failed.** The variants share response, raw data, fake handling, k, luminosity, and mapping; only initialization/efficiency prior assignments change (`run_coherent_prior_study.py:53-60`). The attribution is therefore informative: initialization-only medians are 0.0330%/0.0318%, while efficiency-only medians are 1.052%/1.156% (`comparison/summary.json:71-107`).
- **Acceptance-cancellation escape failed.** This does not affect the release BBB acceptance, which is separately `body/(body+miss)`, as documented by the prior-mismatch investigation (`response_prior_mismatch_investigation_2026-10-04.md:36-38`). It does affect IBU because its response normalization and initialization consume the prior.
- **Intentional-design escape failed for the *plain* prior.** Builder comments explicitly call ordinary `prior_gen` backward-compatible/unweighted and NWeff canonical (`build_migration_th2_per_charge_v6.py:314-322,1008-1021`); the frozen release nevertheless consumes the tagged ordinary prior. That supports fixing the defect, but does not prove that the post-hoc substitute is release-ready.

## Missing Evidence

1. A rebuild from immutable production inputs that writes `h_prior_gen_{pos,neg}_rapgap_NWeff`, has an immutable checksum/manifest, and proves binwise equality to `body+miss` (including duplicate-key, charge, and merge audits).
2. Response-conservation, support/efficiency, fake, and covariance QA for that rebuilt coherent response—not only finite/non-negative outputs.
3. Regenerated coherent-nominal central IBU/BBB release, tracking shifts, all response/model/regularization products, phi_h envelope/budget, and their covariance/provenance; binwise comparison with the old release.
4. Formal re-release decision and new release contract. The old plain-prior contract must remain immutable/auditable rather than be overwritten.

## Recommended Next Check

Do **not** swap the central CSV. Produce one controlled coherent-response re-release candidate: rebuild and store NWeff, require `max_j |NWeff_j-(body+miss)_j|` within a predefined floating-point merge tolerance for each charge, then rerun the complete nominal-dependent systematic chain and compare all 490 central values and uncertainty components before any replacement decision.

# Final hot review: proposed coherent-prior central replacement

**Scope:** read-only adversarial review. No ROOT was opened interactively, no script was rerun, and no central/canonical product was changed.

## Verdicts

- **V1 (fidelity): CONCERN.** The compact IBU study is numerically faithful to the existing plain-prior release, and the isolated rebuild reproduces its body, miss, and ordinary prior to floating precision. But the supplied source review explicitly qualifies the rebuild as faithful to canonical NWeff fill semantics, **not byte-for-byte identical to the full canonical response configuration** (`docs/mc/prior_weight_semantics_2026-10-06.md:74-85`). Thus the study/rebuild is strong diagnostic evidence, not yet the claimed corrected central re-release.
- **V2 (fix correctness): SOUND for the already-built weighted response.** For the matrix actually unfolded, `T_response(g)=sum_r R_body(r,g)+R_miss(g)` is the sole column marginal that counts every response contribution once and with its actual weight (`prior_weight_semantics_2026-10-06.md:87-105`). The direct NWeff comparison demonstrates that NWeff is not that marginal in the presence of many-reco-to-one-truth bookkeeping.
- **V3 (replacement decision): NO.** The single decisive missing item is the **actual corrected central re-release artifact and its complete nominal-dependent downstream regeneration/validation**. The evidence supplied remains study-only; no new dated central release directory, release manifest/RUNLIST, regenerated tracking/phi_h/covariance/tables, or comparison package exists in the inspected output trees.

## Fatal Issues

### 1. The object described as a completed corrected re-release does not exist in the supplied filesystem evidence

**Claim attacked:** The fix has already been executed as a new dated central release with full revalidation and downstream regeneration, so the decision is only whether to relabel it central.

**Evidence:** The actual rebuild README says, unambiguously, “**PROVISIONAL INTERNAL DIAGNOSTIC**” and “no central product modified” (`output_v6/studies/coherent_prior_response_rebuild_z005_20261006/README.md:1-15`). The study tree contains a rebuilt response, diagnostics, scripts, and shard artifacts; it does not contain a release. A search of `output_v6/central/data_release/` found no 2026-10-06 release directory. The only coherent cross-section CSVs found are the four side-study variants under `output_v6/studies/coherent_prior_ibu_z005_20261005/`, not a dated central release. `STATE.md:67-68` and `RUN_STATUS.md:5` still explicitly say central adoption is pending.

**Physical/release mechanism:** A change of IBU denominator and initialization changes the estimator binwise by up to 3.58%, not merely a label. A study CSV cannot serve as a reproducible central product without frozen release inputs, release driver, exact response identity, QA/status manifest, and output contract.

**Disposition:** Analysis-killing for V3 now.

### 2. Nominal-dependent systematic and covariance products have not been demonstrated to be regenerated

**Claim attacked:** All downstream products required by the round-2 critique have been regenerated consistently with the coherent nominal.

**Evidence:** The old tracking ensemble is explicitly a comparison to the existing nominal central release (`context-handoff.md:69-108`), and the adopted floored phi_h budget explicitly defines the nominal as central IBU k=4 (`context-handoff.md:140-145`). The coherent shift affects 125/245 h+ and 129/245 h− bins by over 1% (`coherent_prior_ibu_z005_20261005/comparison/attribution_corrected_20261006.json:15-33,47-65`). Yet the coherent rebuild tree has no downstream systematics, tracking, phi_h, covariance, release-table, or manifest outputs; its only reported NWeff IBU calculation is itself a diagnostic (`comparison/nweff_ibu_summary.json:1-38`). `STATE.md:68` reports new response/audit evidence but not a regenerated systematics/release package.

**Physical mechanism:** Tracking shifts, model/regularization shifts, the floored phi_h envelope, statistical/response covariance, and tables are all referenced to a nominal estimator. Keeping their old absolute/relative values after changing that estimator creates a hybrid result with an unknown uncertainty model. This is especially material because the previous phi_h envelope was known to include fork/prior contamination.

**Disposition:** Analysis-killing for V3 now.

## Major Concerns

### 3. The instrumented audit establishes the NWeff defect, but does not independently validate the response fill itself

**Claim attacked:** Instrumented reproduction proves both the NWeff classification and the absence of any response-side lossiness independently of the fill code.

**Evidence:** The audit reports exact NWeff reproduction and body/miss observer agreement at <2e-15 relative (`audit/instrumented/audit_summary.md:3-7`; `instrumented_audit_final.json:5-39`). It is compelling for the statement that its observers reproduce the merged study response. But the observers instrument the same production-path implementation; a common selection/fill defect would reproduce in both arrays. The source memo concedes a prior independent reimplementation failed reproduction at max deviation 0.0106519 in the task history, and it further labels the rebuild copy not byte-for-byte canonical (`prior_weight_semantics_2026-10-06.md:74-85`). The machine report’s per-bin decomposition also displays sizeable named `unattributed_residual` terms (e.g. `instrumented_audit_final.json:49-67`), even though the separate summary reports a final observer identity at <=7.5e-9. That requires a clear algebraic definition to prevent readers from mistaking the intermediate term for a closed physical decomposition.

**Physical mechanism:** Shared-code reproduction is an implementation-consistency check, not an independent proof that the accepted gen pool, response body, and key accounting reflect the intended detector/truth mapping. It is sufficient to reject NWeff as a response marginal once the observed matrix is accepted; it is insufficient to certify an altered production response by itself.

**Disposition:** Follow-up required for a release QA package; does not defeat the response-algebra case for `body+miss`.

### 4. The external-prior escape fails for efficiency, but must be documented as a deliberate IBU prior policy

**Claim attacked:** IBU should use the full generated pool, so response-derived `body+miss` is necessarily an illegitimate prior.

**Evidence:** A generator model can be an external initialization prior, but the efficiency denominator is not free: it must normalize the matrix columns used in the likelihood/update. Here body entries are weighted by reconstructed-track NucliaWeight while misses are weighted with the event factor; therefore the plain full gen pool is not the marginal of the weighted matrix. The source review shows event-level `weight1*weight2` identity across trees, NucliaWeight reco-only, and the exact algebra `epsilon=col_inner/[body+miss]` (`prior_weight_semantics_2026-10-06.md:5-15,45-70,87-105`). The plain mismatch is 245/245 bins, whereas the rebuilt response body/miss reproduces central at 6.86e-15/9.42e-15 and miss exactly (`validation/validation.json:3-40`).

**Physical mechanism:** An external MC shape can be used only as a prior-shape choice after response normalization is made coherent. It cannot replace the efficiency denominator without breaking column conservation. The corrected re-release must state separately that initialization is also set to the response marginal as the chosen k=4 regularization prior; it must not claim that every possible IBU initialization is forbidden.

**Disposition:** Follow-up/documentation requirement only; V2 remains SOUND.

### 5. NWeff is a real competing numerical output, but not a competing physically valid denominator

**Claim attacked:** The 0.4% NWeff variant makes the coherent choice ambiguous.

**Evidence:** NWeff differs from body+miss in 240/243 bins over 0.1%, with 108/116 over 1%, and has a distinct k=4 shift (`validation/validation.json:4-40`; `comparison/nweff_ibu_summary.json:1-38`). The source-level cause is explicit: body fills every accepted reco row, while NWeff retains only one NW value for a rounded truth key; preferred truth links have no one-to-one guard (`prior_weight_semantics_2026-10-06.md:59-70`).

**Physical mechanism:** Because NWeff omits or retains only one of multiple body-leg contributions, it cannot be the matrix truth marginal. Its numerical existence is useful as a diagnostic/systematic provenance record, not an equally admissible nominal efficiency denominator.

**Disposition:** Attack failed as a reason to reject `body+miss`; retain NWeff as a documented diagnostic, not central.

## Non-Blocking Checks Confirmed

- **Attribution correction passes.** The corrected file preserves baseline/coherent CSV hashes and correctly separates the tiny initialization-only integrated shifts (+0.000115%/+0.000591%) from efficiency-only and coherent shifts (`attribution_corrected_20261006.json:2-77`). The old integrated-attribution bug is repaired.
- **The baseline and coherent study gate remains strong.** The earlier baseline agreement to `3.31e-15`, fixed k=4/data hash/response identity, and 490 finite non-negative outputs remain documented in the study validation. No charge-swap, fake-treatment, or self-comparison escape is evidenced.
- **Rebuild reproduction passes within its stated scope.** Rebuilt ordinary prior is exact, body differences are <=9.42e-15, miss differences zero, dimensions are `(491,246)`, and no acceptance exceeds one (`validation/validation.json:1-71`).
- **NWeff is decisively rejected as the fix.** Its ratio to body+miss spans 0.968–1.033, while the retained-key mechanism is source-supported and instrumented in the production path. This resolves the round-2 NWeff ambiguity in favor of the response marginal.
- **Release-contract attack succeeds.** A new dated directory with immutable manifest, frozen command/runlist, checksums, status/QA, and regenerated descendants would satisfy the round-2 requirement. The proposed definition is appropriate; the artifact itself is absent.

## Missing Evidence

One integrated corrected central re-release package, in a new dated `output_v6/central/data_release/` directory, containing:

1. immutable response/data/script checksums, coherent-prior formula, frozen driver/RUNLIST, 490-row IBU and BBB outputs, and binwise QA;
2. an explicit unchanged-BBB/acceptance validation, plus coherent IBU k=1–10 and data/response-statistical covariance revalidation;
3. regenerated tracking shifted releases/ensemble, all response/model/regularization shifts, the floored phi_h budget and its provenance, covariance sidecars, release tables/plots, and 490-bin old-versus-new comparison manifests;
4. an auditable declaration that the old plain-prior dated release remains preserved/superseded rather than overwritten.

## Recommended Next Check

Create and validate that one new dated coherent-prior re-release package before voting on promotion. Re-run all nominal-referenced systematic descendants from that package; then a final critic can evaluate an actual release rather than infer one from a diagnostic response rebuild.

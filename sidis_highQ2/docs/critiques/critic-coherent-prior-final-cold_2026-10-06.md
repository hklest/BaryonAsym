# Cold final critique: coherent-prior baseline replacement

## Verdicts

- **Overall: SOUND for the replacement decision, conditional on the specified corrected re-release.** This is not approval of a post-hoc central-file swap and does not claim that a replacement release already exists.
- **V1 (fidelity): YES.** The original controlled IBU study reproduces the 490-row central release at maximum relative deviation `3.31e-15`, with the fixed response/data/fake prescription, `k=4`, and `L=351000 nb^-1` documented in `docs/unfolding/coherent_prior_fix_results_2026-10-05.md:57-81`. The rebuild independently reproduces central body at `6.8645386853132736e-15` (h+) / `9.420397652729519e-15` (h-), miss exactly, and the plain prior exactly (`.../validation/validation.json:4-40`). Its `5342813` job record is `73` completed, `0` failed/held (`condor/cluster_5342813_status.json:2-9`). The instrumented production-path audit reproduces merged NWeff exactly and body/miss at <=`1.8504352532147086e-15` relative (`audit/instrumented/instrumented_audit_final.json:5-39`); its `5343475` record has 73 normal terminations and no nonzero exit (`audit/instrumented/merged/cluster_5343475_status.json:2-10`).
- **V2 (fix correctness): YES.** The response truth marginal is explicitly `sum_r R_body(r,g)+R_miss(g)=body+miss`, hence the only documented exact response-conserving IBU efficiency denominator (`docs/mc/prior_weight_semantics_2026-10-06.md:87-104`). The same memo documents that `weight1/weight2` are shared event-level variables in both trees (`:27-37`), while the response body retains every accepted reco leg but NWeff retains one partner per truth key (`:49-70`). The instrumented audit quantitatively resolves the former ambiguity: high/low outliers are 105/135 h+ and 110/133 h-, exclusively matched-key retention versus body-leg accounting, with no negative, fallback, or retained-outgrid contributor (`audit/instrumented/audit_summary.md:3-7`); exact decomposition residuals are <=`7.450580596923828e-09` and pass `1e-08` (`instrumented_audit_final.json:8781-8806`).
- **V3 (replacement decision): YES — replace through a new corrected central re-release only.** NWeff is a third tested prior, but not an unresolved alternative: it differs from body+miss in all 245 bins per charge, with ratios h+ `0.9681332799/0.9958820376/1.0334613577` and h- `0.9675814656/0.9977532347/1.0303854894` (`comparison/nweff_vs_bodymiss_summary.json:3-42`), and produces a distinct k=4 result (median `0.410435%` h+ / `0.409908%` h-, integrated `-0.451506%` / `-0.451447%` versus baseline; `comparison/nweff_ibu_summary.json:3-38`). That is direct evidence against selecting NWeff as the coherent denominator, not ambiguity about the response marginal.

## Major Concerns

### Replacement must remain a corrected release, not a retrospective substitution

- **Claim:** a decision YES authorizes changing the baseline method, not modifying existing release artifacts.
- **Evidence:** the study and rebuild are explicitly `PROVISIONAL INTERNAL DIAGNOSTIC` and state no central product was modified (`coherent_prior_fix_results_2026-10-05.md:1-13`; rebuild `README.md:1-15`). The documented adoption procedure requires builder-level rebuild, merge-time column-conservation assertion, regenerated central release and QA/iteration check, downstream shifted/model/floor/smoothing/plot regeneration, and refreshed review (`coherent_prior_fix_results_2026-10-05.md:159-183`).
- **Physical mechanism:** nominal-dependent systematics and covariance products otherwise retain the old plain-prior central vector.
- **Severity:** analysis-killing if a post-hoc CSV/histogram replacement is attempted; follow-up required for the approved corrected re-release.

## Non-Blocking Checks Confirmed

- **Round-2 attribution blocker resolved.** `attribution_corrected_20261006.json` is `PASS`; init-only medians are `0.0003297414`/`0.0003176155`, efficiency-only medians `0.0105244729`/`0.0115627598`, and init-only integrated shifts `1.1465435e-06`/`5.9086560e-06` (`:2-65`). Baseline/coherent SHA-256 values are identical before/after and `baseline_coherent_unchanged=true` (`:69-77`).
- **Round-2 requested NWeff equality test is resolved negatively, not missing.** The rebuild validation records `gate4: FAIL_NOT_EQUAL`, while the central-reproduction gates pass (`validation/validation.json:43-48`). The negative result is explained by the instrumented accounting evidence above.
- **Builder citations support the event-level-weight claim.** The memo identifies shared Stage-2 variables and branch citations at `src/ChargedSidisTreeBuilderHighQ2_v6.C:524-529, 745-746, 812-853, 1027-1050, 1374-1903` (`prior_weight_semantics_2026-10-06.md:27-37`).

## Missing Evidence

- No corrected central re-release, regenerated nominal-dependent systematic/covariance artifacts, or refreshed validation manifest exists yet in the reviewed record. This does **not** reverse V3 because V3 explicitly approves doing those steps; it prohibits declaring the old release replaced before they are completed.

## Recommended Next Check

Execute the documented corrected-re-release chain with `prior=body+miss` for both IBU initialization and efficiency; require the merge-time `|body+miss-prior|<=1e-6` assertion, then submit the complete regenerated release, downstream systematics/covariance manifest, and release QA for fresh independent review.

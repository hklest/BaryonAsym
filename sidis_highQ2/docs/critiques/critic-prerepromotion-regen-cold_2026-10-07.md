# Cold documentary QA — coherent-prior pre-promotion regeneration

**Verdict V1 (campaign fidelity): CONCERN.** The recorded base reruns consistently identify `body+miss` from each source response as the consumed initialization and efficiency prior, and all completed branches have the expected 490-row structure. However, the IBU-native family explicitly omits the legacy B1 per-bin response-bootstrap floor, so the documented family is not complete as the budget-grade native-model lineage required for promotion.

**Verdict V2 (budget/supplement correctness): CONCERN.** The assembled 490-bin table, definitions, source paths, alignment checks, quadrature closure, and covariance schema are internally consistent with the recorded legacy contract. It is nevertheless explicitly a B1-incomplete provisional budget, not a promotion-complete uncertainty product.

**Verdict V3 (promotion readiness): NO.** The single decisive missing item is a recorded decision and, if retained, coherent-prior rerun of the B1 per-bin response-bootstrap model floor. The plan makes this a promotion gate; neither a flag nor an unfloored replacement establishes its disposition.

## Fatal Issues

None demonstrated from the inspected documentary artifacts.

## Major Concerns

### 1. The B1 model-floor gate remains open

**Claim.** The regenerated native-model/budget chain cannot be called promotion-ready while the old B1 per-bin response-bootstrap floor has neither been regenerated nor formally removed by decision.

**Evidence.** `ibu_native_coherentprior_2026-10-07/summary.json:45` says “NOT RERUN” and requires a separate decision/campaign before budget promotion; its README:6 specifies 100 B1 replicas. The assembled budget repeats `model_b1_floor_pending=true` and says the legacy floor is non-transferable because it used fixed published priors (`.../total_uncertainty_z005_with_phih_coherentprior_2026-10-07/provenance.json:60-61`). The plan classifies B1 certification as required before promotion (`docs/systematics/nominal_dependent_regeneration_plan_2026-10-06.md:53,59`).

**Physical mechanism.** A per-bin model-floor contribution derived under a different prior can change after the efficiency/prior replacement; carrying it forward or silently omitting it changes the model uncertainty without a coherent basis.

**Impact.** Promotion-blocking, but not evidence that the completed base model calculation is numerically wrong.

### 2. The large model and JES reductions are reproduced but their causal attribution is not demonstrated

**Claim.** The artifacts support the numerical reductions, not a documentary proof that the reductions arise specifically from removal of plain-prior contamination rather than an unrecorded calculation difference.

**Evidence.** The model source records the prescribed formula `0.5*abs(sideA-sideB)/new_nominal` and changes h+ median `1.729799% -> 0.838786%`, h− `1.538071% -> 0.801353%` (`ibu_native.../summary.json:24-42`). The JES source records all 490 rows per tag, finite/nonnegative output, own-response `body+miss` validation below `8.32e-16`, and the h+/h− median reduction `0.761407/0.813337% -> 0.305240/0.349761%` (`detector_shifted.../summary.json:8-58`; comparison table:46-56). The budget imports those exact named CSVs with checksums (`.../provenance.json:19-45`). No inspected summary supplies an old/new same-source decomposition isolating only the old plain-prior term.

**Physical mechanism.** A response/prior-consistency correction can cancel a source shift that was formerly dominated by mismatched efficiency or initialization. But the identical observed direction also could result from an untracked consumer/configuration change; the listed summaries establish inputs and closure, not this exclusive causal decomposition.

**Impact.** Follow-up only: preserve these values as provisional and require the refreshed reviewer to inspect an explicit old-prior/coherent-prior same-source attribution before physics interpretation. No calculation defect is demonstrated.

### 3. The phi_h residual-generator-model explanation is stated only as an expectation, not demonstrated binwise

**Claim.** The partial phi_h reduction is documented; attribution of the residual above the approximately 0.6% pure-flat scale to generator-model contamination is not established by an identified residual decomposition.

**Evidence.** The re-derivation gives the unchanged procedure, coherent nominal, 490-row alignment, and medians `1.048153%` (h+) and `1.055001%` (h−) from `1.485371%` and `1.415276%` (`phi_h.../summary.json:2-19,39-45,53-86`). It records only `pure_flat_vs_unity_expectation_median_percent_approx: 0.6` (`:86`), while the README defines an envelope over DJANGOH/RAPGAP flat results (`README.md:11-15`). No inspected per-bin generator-model subtraction or covariance attribution shows that the remaining difference is generator-model rather than another term in the legacy envelope.

**Physical mechanism.** A two-generator envelope necessarily contains generator-response/model spread in addition to a pure flat-phi response effect; that is compatible with, but does not prove, the proposed explanation.

**Impact.** Follow-up only. The documented component is reproducible under the approved old prescription; its physical interpretation remains qualified.

## Non-Blocking Checks Confirmed

- **Candidate baseline record.** The task-supplied release record reports 245/245 truth conservation per charge, IBU cross-checks `3.31e-15/3.66e-15`, finite output, and BBB unchanged to `1e-15`; the downstream packages all point to that dated candidate as nominal (e.g. detector summary:5-6; budget provenance:2-4).
- **Detector family (five tags).** Every tag records `job_status=OK`, 490 rows (245/charge), no negative cross sections, and own-shifted-response `body+miss` prior use for both initialization and efficiency; recorded max deviations are `5.05e-16` to `8.32e-16` (detector summary:8-27,61-80,114-133,167-186,220-239). README:56 records the same 490-row/finiteness result for all tags. The supplied cluster completion is 5379915, 5/5.
- **IBU-native base branches.** README:3-7 records own-response `body+miss` for k=3/k=5 and each generator response, prescribed half-difference definitions, Condor cluster 5379991 completion 4/4, and exit 0. DJANGOH metadata:3-12 independently records its response tag, k=4, 351000 nb^-1, own-response prior, and 490 rows.
- **Statistical toys.** The campaign records cluster 5380017 completion 2/2, 1,000 toys/charge, legacy seeds 12345/22345, `body+miss` for initialization/efficiency, 490 finite nonnegative components, and symmetric positive-diagonal 245x245 covariance for both charges (`stat_toys.../summary.json:4-40`). Candidate-nominal reproduction is below `6.97e-16`; the documented prior failed cluster is explicitly reconciled as producing no adopted numerical output (`:7-9,33-34`).
- **Tracking ensemble.** The manifest records all seeds 314159–314174 included, none excluded, candidate nominal, and own-seed-response `body+miss` prior (`ensemble_manifest.json:22-47`). The summary records PASS alignment/finiteness/lumi/nonnegative/row-count checks, 16 included, 245 rows/charge, and medians `0.180642123%` h+ and `0.182832532%` h− (`summary.json:8-35`). The supplied clusters are 5380048/5380058, 16/16.
- **phi_h regeneration.** The source documents direct runner consumption of `h_prior_gen`, builder writing that histogram as `body+miss`, k=4, shared 980-vector, L=351000, 490 aligned rows and finite nonnegative values (`phi_h summary:11-19,39-51`; README:7-15).
- **Budget construction.** It preserves the 11 legacy component-column names (`total summary:5-29`), explicitly defines `total_quadrature_rel`, `total_with_tracking_rel`, and `total_with_phih_rel` (`:30-34`; provenance:62-65), and records 490 unique aligned keys for every component, finite/nonnegative values, total-at-least-component, and exact quadrature closure (`validation_checks.json:2-96`). The reported totals are h+ median/max `5.633107/160.613079%` and h− `5.975659/83.518185%` (`:97-116`). The old/new comparison table matches the campaigns’ recorded component medians, including statistics, model, JES, tracking, and phi_h (`comparison_vs_legacy...json:7-173`).
- **Covariance supplement.** The package declares the legacy schema match, candidate/body+miss basis, exactly 490 aligned rows, the same signed half-difference covariance forms, finite/symmetric/positive-diagonal matrices, and PASS (`covariance manifest:2-28,80-122`). The README confirms no statistical charge cross-block is claimed from independent toy loops (`README.md:16-20`).

## Missing Evidence

- An old-versus-coherent same-source attribution table sufficient to demonstrate, rather than infer, that the model and JES reductions are caused by removal of the plain-prior inconsistency.
- A binwise residual decomposition establishing the phi_h residual’s generator-model contribution.
- B1 decision/campaign output. The budget correctly carries the pending flag, but a flag is not the required uncertainty-source disposition.

## Promotion gates

**Satisfied for the regenerated base chain:** dated coherent candidate consumed; detector tags; k-scan and base model branches; 1,000-toy covariances; 16-seed tracking ensemble; phi_h floor refresh; 490-row aligned budget/validation; model/reg covariance supplement. The completed campaign packages consistently remain labelled pre-promotion/provisional.

**Open nominal-dependent gate:** B1 floor decision and, if retained, coherent 100-replica regeneration.

**Open independent blockers that promotion must surface, not cure:** frozen paired Stage1+Stage2 HFS/electron campaigns and noise qualification; response-MC/fake/background covariance; alpha/background consistency and track-level inputs; C_rad/Born approval; nominal-method decision; charge cross-block/charge-dependent tracking for asymmetry applications; unresolved canonical publication plotter. These are listed as independent blockers in the plan (`nominal_dependent_regeneration_plan_2026-10-06.md:61`) and are not resolved by this review.

## Recommended Next Check

Record the B1 disposition. If retained, run and validate the 100 coherent-prior B1 response-bootstrap replicas, rebuild `ibu_model_rel`, the budget, and the covariance supplement; if formally retired, record the approving rationale and revised component definition before the refreshed promotion review.

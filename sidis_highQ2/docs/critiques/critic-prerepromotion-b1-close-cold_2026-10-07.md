# Cold documentary QA — B1 close-out before promotion

**Verdict V1 (B1 fidelity): SOUND.** The coherent B1 campaign implements the approved per-bin response-bootstrap floor with 100/100 valid replicas, `n_eff=sumw²/sumw2` Poisson cell resampling, IBU `k=4`, and the replica-owned resampled `body+miss` marginal for both initialization and cross-closure bias reference. The four 3-SEM working-point excursions are report-only extreme-value candidates, not exclusions from the floor or budget.

**Verdict V2 (budget correctness): SOUND.** The B1-floored 490-bin budget applies `max(ibu_model_rel_base,b1_floor_coherent)` bin by bin, recomputes all totals, and passes finiteness, component-domination, and quadrature-closure checks. The corrected certification is downstream diagnostic metadata only; it did not enter the floor array or budget combiner.

**Verdict V3 (promotion readiness): YES.** The prior review's sole nominal-dependent pre-promotion gap—an approved B1 disposition followed by a coherent rerun and rebuilt budget—is closed. This endorses starting the 10-step baseline-update promotion declaration; it does **not** close the independent physics/release blockers that the declaration must carry forward.

## Fatal Issues

None demonstrated.

## Major Concerns

None promotion-blocking from the B1 close-out.

## Non-Blocking Checks Confirmed

### 1. Coherent B1 method, replica completeness, and traceability

**Claim.** The B1 campaign faithfully uses the approved coherent-prior definition rather than transferring the old plain-prior floor.

**Evidence.** The campaign README records seeds `2026100400+r`, `n_eff` Poisson resampling from `_w2`, IBU `k=4`, `L=351000.0 nb^-1`, and the replica-owned `body+miss` initialization and bias-reference rule. `submission_status.json` identifies full Condor cluster `5380196`. The aggregation reads 100 PASS replica validation records and reports `n_included=n_expected=100`, with 490/490 supported finite floors. The driver constructs each replica truth marginal as matched body plus miss, passes that marginal as the unfolding prior, and evaluates each bias against the same replica-owned marginal.

**Physical mechanism.** The response bootstrap must fluctuate the response, its reco marginal, and the IBU truth/efficiency basis coherently; otherwise the floor would retain the plain-prior inconsistency which motivated this regeneration.

**Impact.** Pass. The resulting floor has median `1.1811%`, p95 `7.8278%`, and maximum `16.4381%`, so it is a materially bin-resolved replacement rather than an integrated proxy.

### 2. Corrected certification has removed the SEM inflation

**Claim.** The corrected B1 signal-to-floor classification uses the per-replica floor, not the uncertainty on its estimated mean.

**Evidence.** `b1_floor_aggregation.py` explicitly computes `sem=floor/sqrt(n)` only for the working-point reproduction gate, then separately computes certification as `abs(d_base)/floor`. The corrected JSON/CSV define exactly this rule and retain `floor_sem_rel` only as a distinct field. Direct row consistency is visible, e.g. h+ bin 0 has `0.00693097/0.000627399=11.0472`, matching its stored certification z, whereas division by its SEM would have given approximately 110.5. The complete corrected class counts are h+ `216/19/10` and h− `215/20/10` for NOISE/MIXED/REAL, i.e. `88.1633/7.7551/4.0816%` and `87.7551/8.1633/4.0816%`.

**Physical mechanism.** The floor is the MC-response fluctuation scale of one model estimator. Dividing by the SEM instead tests precision of the estimated 100-replica mean and artificially multiplies the z values by 10.

**Impact.** Pass. The corrected predominately-NOISE classification is compatible with the relative scales: the base model medians are about `0.839%`/`0.801%`, below the combined median B1 floor `1.181%`. No remaining SEM use appears in the certification calculation.

### 3. Four working-point gate excursions are correctly non-destructive

**Claim.** The aggregation's `FAIL` label does not establish a failure of the B1 floor or of the floored budget.

**Evidence.** The only recorded aggregation problems are one h+ and three h− replica-mean deviations exceeding the 3-SEM reproduction threshold. Their computed dev/threshold ratios are `1.005`, `1.022`, `1.149`, and `1.259`; all four are classified NOISE under the corrected, independent signal-to-floor metric. The legacy Section 9.1 treatment identifies the same kind of near-threshold 3-SEM excursion as an expected multiple-bin extreme and retains its raw floor unmodified in outputs, with only certification handling/reporting affected. The new budget provenance preserves the four-gate record, while the numerical budget validation remains PASS.

**Physical mechanism.** Across 490 binwise stochastic checks, a handful of just-over-3-SEM excursions are expected even for an unbiased replica ensemble. They test reproduction of the unresampled working point, not whether the bootstrap standard deviation is finite or usable.

**Impact.** Follow-up only. The status label should not be misreported as a campaign-production failure; preserve the four bin identifiers and ratios in the promotion record. The floor values remain included, as approved.

### 4. Per-bin flooring and total recomputation are mechanically and numerically coherent

**Claim.** The certification defect cannot have contaminated the adopted B1-floored model array or total uncertainties.

**Evidence.** The budget combiner loads only `floor_rel`, computes `new=max(old,floors[key])`, and never consumes certification z, class, or SEM. It then recalculates the ten-base-component quadrature, tracking total, and phi_h total. The output has 490 rows (245/charge), finite non-negative components/totals, total no smaller than every component, and exact stated closure. The floor binds in 150 h+ and 163 h− bins; model medians change `0.8388% -> 1.4006%` (h+) and `0.8014% -> 1.4255%` (h−). The resulting totals are h+ median/max `5.6838%/160.6131%` and h− `6.0950%/84.7483%`.

**Physical mechanism.** `max(base,floor)` is conservative per bin and avoids lowering any coherent cross-closure model estimate. Recomputing rather than reusing totals prevents an inconsistent total after the affected component rises.

**Impact.** Pass. The correction changes only a diagnostic classification; the reviewed budget arrays are unaffected by construction.

## Missing Evidence

No additional evidence is required to close the prior B1 pre-promotion gate.

A minor documentation hygiene item remains: the top-level descriptive `definition` field in `b1_ibu_model_floors.json` still contains an inherited statement about fixed published priors/truths, while the executable driver and campaign README correctly implement and describe replica-owned resampled `body+miss` priors/truths. This is a stale descriptive string, not evidence of a numerical mismatch, because the per-replica driver, working-point file, campaign README, provenance, and output values all identify the coherent basis. Correct it during promotion documentation so no later reader mistakes it for the executed method.

The prior round's model/JES causal-attribution and phi_h residual-generator-model decomposition gaps remain follow-up-only: the regenerated values and budget interfaces are traceable, but those explanations should not be elevated to demonstrated physics causes without their requested same-source/binwise decompositions.

Independent blockers that promotion must surface—not cure—remain: the method decision, C_rad/Born review and provenance, alpha/background response consistency and covariance, response-MC/fake/background covariance, paired detector/noise qualifications, charge-correlation considerations for asymmetries, and the canonical publication plotter.

## Recommended Next Check

Proceed with the 10-step baseline-update promotion declaration, explicitly recording the B1 floor rule, cluster `5380196`, the corrected certification definition, and the four report-only 3-SEM excursions; then route the independent blockers through their designated reviews before any physics-final or publication claim.

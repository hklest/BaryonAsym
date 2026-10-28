# Central response stored-prior mismatch investigation

> SUPERSEDED-BY (2026-10-06): the recommended fix is implemented and validated in the coherent-prior fork study; the resolved state, k=4 numbers, and adoption-pending requirements are in `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`. This investigation stands as root-cause provenance.

**Date:** 2026-10-04  
**Scope:** read-only audit of the central v6/z005 RAPGAP response and its IBU k=4 consumer. No central or release artifact was changed.

## Verdict

The central ROOT file's tagged RAPGAP prior is not the truth marginal of its stored response: the canonical builder fills the ordinary prior from every accepted `ChargedTotGen` row with `w_gen=weight1*weight2` (lines 769--780; definition lines 464--470), but fills a matched body entry from a `ChargedTot` row with `w_reco=weight1*weight2*NucliaWeight` (lines 591--603 and 818--826), while an unconsumed miss uses `w_gen` (lines 852--875). Thus the ordinary prior and `body+miss` can differ whenever the matched-leg selection/weight provenance differs; the central stored product lacks the explicitly designed NW-effective prior. This agrees with the internally conserving 2026-09-25 fork, rather than implicating matrix mechanics.

## Reproduction

The one-file audit used, for each gen bin `j`,

```
prior[j] / (sum_i=0..489 R[i,j] + R[490,j]).
```

`R[490,j]` is the miss row **for the same column**. The central file has `(491,246)` response histograms, so the physical response is rows `0..489`, columns `0..244`; the final column is the fake column and is excluded.

| charge | tagged response / prior | bins non-unit | stored-prior sum | body+miss sum | truth-prior / prior | `(body+miss)/prior` min / median / max |
|---|---|---:|---:|---:|---:|---:|
| h+ | `h_R_pos_rapgap` / `h_prior_gen_pos_rapgap` | 245/245 | 91,784,225.0791 | 91,879,281.4817 | +0.1035651% | 0.964168 / 0.997185 / 1.028029 |
| h- | `h_R_neg_rapgap` / `h_prior_gen_neg_rapgap` | 245/245 | 90,185,163.1419 | 90,271,538.7361 | +0.0957758% | 0.965375 / 0.994197 / 1.028519 |

These reproduce the STATE.md values (the report table uses the `truth/prior` orientation reported there). The audit lists all ROOT keys in `summary.json`; relevant additional keys include djangoh and nominal response/prior pairs, `h_matched_reco_marginal_{pos,neg}_{rapgap,djangoh}`, count histograms, data histograms, bin map, and metadata. No `h_prior_gen_*_NWeff` object is present.

## Fill-path comparison

* **Plain stored prior:** `good_gen=(i_gen>=0)` and charge selection, then `prior_gen += w_gen`; it is a full accepted gen pool, not only matched objects (builder lines 769--782). `i_gen` requires broad generator event/numerical, pool, V0, charge, and coarse-bin checks (362--470), not reco selection.
* **Response body:** a `ChargedTot` row must have a valid reco bin and pass full reco selection (797--805), an inlined matched generator must pass the mirrored broad gen/pool/V0/bin requirements (629--693), and is filled with `w_reco=w1*w2*NW` (591--603, 818--826). It is therefore conditional on a reco-side row, matching/key resolution, and reco selection, with NW.
* **Miss row:** every accepted gen-pool row not found in `consumed_keys` is placed in row 490 with `w_gen` (852--875). Misses have no reco-side NW factor.
* **NW-effective variant:** the snapshot defines `prior_gen_NWeff` specifically to align the matched leg: accepted gen rows matched in `consumed_keys` use `w_gen*NW_partner`; misses use `w_gen` (877--900). The writer calls it canonical for IBU/TUnfold and writes `h_prior_gen_{tag}_NWeff` (1008--1021); the accumulator commentary distinguishes this from ordinary backward-compatible `prior_gen` (304--320).

Consequently, different matched-vs-miss weights, reco/matching eligibility, key collisions or unmatched partners, and independently merged per-shard arrays can separate plain `prior_gen` from the response truth marginal. The central file is an audited merged object with tag-suffixed names and no NW-effective histogram, so it demonstrably does not expose the corrective object described in the later canonical snapshot.

## Which prior central IBU actually consumes

The release driver fixes `KSEL=4` (release `_run/run_bbb_ibu_release.py:30`), loads the RAPGAP result (74--82), and passes `ld["prior_gen"]` to `ibu_unfold` (78). Its loader requests `h_prior_gen_{charge}_{response_tag}` before untagged fallback (the deployed `crosscheck_unfold_methods_v4.py:391--403`), records the resolved name (427--445), and the release calls the tag `rapgap` (89--105, 212--215). Thus central IBU k=4 consumes **`h_prior_gen_pos_rapgap` and `h_prior_gen_neg_rapgap`**, not an NW-effective prior. Acceptance itself is computed independently from body plus miss, `A_j=sum_i B_ij/(sum_i B_ij+M_j)` (516--563).

## Cheap ROOT-only hypothesis test

The leading testable hypothesis is a matched-leg/reco-side weight provenance effect, rather than a binning or a universal normalization error. The ROOT file cannot test the direct NW prediction because it has no NWeff object. Nevertheless, its signed pattern is incompatible with a single scale: `prior/(body+miss)` spans h+ `0.972735--1.037164` (median `1.002823`) and h- `0.972272--1.035867` (median `1.005837`). Correlations of that ratio with response acceptance are only `+0.220` (h+) and `+0.255` (h-), with gen-z center `+0.102/+0.159` and gen-pT center `-0.033/-0.044`. This weak geometry dependence is consistent with a track/event weight or matched-leg population effect, not a simple z/pT bin-edge defect. The prior raw-event-weight spread in the pre-existing audit (`1.0--3.76`) makes an event/track-weight provenance mechanism credible, but does not identify its exact historical merge/fill stage from ROOT contents alone.

The flat-phi fork is the decisive control: its truth-coherent policy puts truth weights on prior/miss and the matched partner (flat-phi note lines 11--17), and all four responses satisfy body+miss/prior=`1/1/1`, with zero bins beyond `1e-6` (118--122). It proves coherent filling can close exactly.

## Fix and impact

**Recommended fix:** rebuild the response and stored prior coherently, defining the consumed IBU prior as the exact column marginal `body+miss` (or equivalently retain/write and explicitly consume a truth-coherent NWeff construction with matched `w_gen*NW_partner` and miss `w_gen`). Require a merge-time column-conservation assertion at `1e-6`, retain both named variants with unambiguous metadata, and rerun release QA before any use. Do not patch central histogram bins in place.

The local discrepancy is about 2.8% peak-to-peak in the truth/prior ratio. In D'Agostini IBU it perturbs the initial prior and the response normalization used in the iteration; at iteration 1 the natural scale is therefore a few percent per affected bin. Four iterations suppress purely prior-driven differences when migration constrains the bin, but migration can propagate them, so a defensible pre-rerun order-of-magnitude is **sub-percent to few-percent per bin at k=4**, not an integrated 0.1% normalization shift. The exact k=4 shift requires the coherent response/prior rerun; it must not be inferred from the small integrated total mismatch.

## Artifacts

* Reproduction data, script copy, log, and status: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/audits/response_prior_mismatch_2026-10-04/`
* This report: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md`

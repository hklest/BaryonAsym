# Statistical coverage of the eSigma IBU release (2026-10-26)

**Scope.** This study asks whether the per-bin uncertainties quoted for the release
`bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-24`
have correct frequentist coverage. It quantifies any under-coverage and proposes a treatment that can be published.

- The release uses IBU with k=4, 490 presented bins, and 2 reco bins per presented bin.
- This is an investigation only. The release, the budget, the covariance and the documents are unchanged.

**Paths.**
- Root: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`
- Scripts: `scripts/studies/stat_coverage_2026-10-26/`
- Outputs: `output_v6/studies/stat_coverage_2026-10-26/`
- Abbreviations: S = `scripts/unfolding`; E = S/esigma_final_2026-10-24; RUN = E/release/run_bgsub_setrprime_reunfold_cone_optA.py; ENG = S/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py; TOY = S/tunfold_official_v4_percharge/ibu_toy_covariance_v4.py

## Summary

1. **The quoted stat uncertainty has the right size for N≥3.** There, the 1000-toy RMS matches the true spread of the k=4 estimator: the median quoted/true ratio is 0.96–1.00. A 1-track bin at 50% instead of the bin-by-bin ~110% is not an error. IBU pulls sparse bins toward the RAPGAP prior, which genuinely cuts the variance. The price is a bias.
   - At N≤2 the per-toy σ is itself unreliable. Pseudo-experiments that see 0 events quote σ≈1. Even with the truth equal to the prior, coverage is only 0.61–0.66 and quoted/true is 0.91–1.01. Between 0.3% and 12% of pseudo-experiments have zero raw counts in these bins.
2. **For N≥3, coverage fails because the bias is not quoted.** This is not specific to sparse bins.
   - With the truth equal to the prior, ±σ_stat covers 0.66–0.68.
   - Pooled over six plausible-to-stressed truths, it covers 0.54–0.59.
   - The IBU part of the budget (stat ⊕ ibu_model ⊕ ibu_reg) covers 0.61–0.66.
   - Coverage tracks bias/σ: 0.67 when |b|/σ<0.1, 0.35 for 1–2, and 0.07 for ≥2.
   - Even bins with N≥500 under-cover (0.59 pooled). There σ_stat is ~2.2%, and the response-model and shape bias is 0.2–0.6 σ.
3. **Response-MC statistics are handled correctly by the covariance.** The pull width grows only ×1.00–1.04 when the response is fluctuated. The existing `cov_mcstat` term restores coverage, but it is not in the per-bin quoted total.
4. **None of the other levers fixes it.** No single number of iterations gives 0.68 everywhere. Toy-percentile intervals cover worse than ±σ. IBU neighbour correlations in the quoted toy covariance are faithful.
5. **Recommendation (R):**
   - Add a per-bin **unfolding closure term**, quoted as a new budget source on top of ibu_model and ibu_reg, with a rank-2 covariance. No new toys or events are needed.
   - Flag bins with N<10 in the tables.
   - Keep k=4 and symmetric errors.
   - R is tested with whole truth families held out of the closure training. It covers 0.67–0.70 pooled over all six truths for N≥4, 0.70–0.73 on the four plausible truths, and 0.69–0.78 for N≤3.
   - It still under-covers the corner-stressed truth (0.48–0.66) and the DJANGOH-response truth at N≥500 (0.62).
   - Cost: see §3.

## 1. Audit: how the quoted uncertainty is built

All items in this section were verified against the code.

| Item | Finding | Citation |
|---|---|---|
| Producer chain | esph_downstream → run_release_final → RUN; installed `ibu/cross_sections.csv` byte-identical to `downstream/release/esph660/ibu/` | E/esph_downstream.py:137-150; S/final_rerelease_2026-10-22/run_release_final.py:20-44 |
| Quoted stat | RMS of 1000 Poisson toys of the bg-subtracted vector data_sub'; seeds 12345/22345; k=4 | RUN:1046, 878, 177, 173; common_bgsub_setr_cone_optA.py:219; TOY:210-211, 304-305 |
| Fixed per toy | response, prior, f_i, background | TOY:317 |
| Spread reference | toy mean (np.cov), so the RMS **cannot contain bias with respect to the truth** | TOY:325, 332; RUN:976-981 |
| Analytic D'Agostini error | written but not quoted (frozen M of iteration 10) | ENG:704, 716 |
| Response MC stat | not in the stat or the 26-term quoted total; in C_total as `cov_mcstat` (s03, K=200 replicas) | E/phaseD s03_…optA_2026-10-20.py:87-141 |
| ibu_model (budget) | max(0.5\|DJ−RG\|/RG, B1 floor) | E/budget/components_groupA.py:358-360 |
| ibu_reg | 0.5\|k5−k3\| | components_groupA.py:367; ENG:805 |
| Release-CSV model | full \|DJ−RG\|, no floor (the CSV `total_unc_*` is not the quoted total) | RUN:971, 982 |
| Covariance | stat block = 1000-toy covariance; ibu_model and ibu_reg rank-1 signed | E/phaseD/s08s09/s08_…py:141-182; s09_…py:36-41 |
| Sparse-bin rules | only `supported=(prior>0)&(eff>0)`, which all bins pass; no minimum-N rule | RUN:940; `summary_totals.json` |

ibu_model and ibu_reg compare two regularised answers. They cannot bound the bias relative to the truth where the prior dominates.

**Evidence the user raised (confirmed).**
- N is the raw count of tracks summed over the 2 feeding reco bins, before subtraction. The minimum N is 1, so this release has no 0-track bins.
- IBU stat / BBB stat for the 1-track bins: h+ 109 is 50.0% / 110.1%; h− 109 is 57.8% / 112.9%; h− 240 is 34.1% / 100.8%.
- Exactly 10 bins have stat < 0.5/√N: h+ 135, 200, 205, 206, 235 and h− 170, 205, 235, 240, 241. All lie in the high-z, low-pT corner.
- Per-bin table: `output…/evidence/evidence_table.csv`.

## 2. Coverage study

### Method (verified)

**Harness.** The harness imports the production engine unmodified. It reproduces the release yields, model, reg and toy stat to ≤3.5e-15 relative.

**Truths** (`run_coverage_toys.py`, `prepare_inputs.py`):

| Truth | Definition |
|---|---|
| rapgap | the RAPGAP prior |
| djangoh | folded with the RG response |
| djangoh_djresp | folded with the DJ response |
| datalike | the nominal unfolded result |
| stressA | ×2 at z<0.05 & pT>2 GeV, ×0.5 at z>0.7 & pT<1 GeV |
| stressB | the reverse of stressA |

**Procedure.** Each truth is normalised to the data total. It is folded with the nominal response and fakes, the background B_R' is added, and the raw sum is Poisson-fluctuated at the data statistics. The result is subtracted as in the nominal, then unfolded with IBU at k=4 and the RAPGAP prior. For every pseudo-experiment, the quoted stat is recomputed with 1000 inner replicas, exactly as the release does.

**Modes.**
- `data`
- `respmc`: the RG response is replaced by an s03 replica.
- `respmc_only`: Asimov data with a replica response.

**Statistics.** 1000 toys per truth and charge in the `data` and `respmc` modes. The `respmc_only` mode has 200 toys for 2 truths. In total that is 24,800 toys. The work ran as Condor clusters 5889802 (toys, 248/248 OK), 5889898 (merge), 5889853 (exact inner-percentile reruns), 5889892 (analysis) and 5889945 (treatment evaluation).

**Plots** (`output…/plots/`):
- `pull_width_vs_N.{pdf,png}`
- `coverage_vs_N.{pdf,png}`
- additional: `iteration_scan.*`, `correlation_vs_N.*`

### Results by N class

Data mode, pooled over 6 truths and both charges. Source: `analysis/coverage_analysis.json`, `fixes_by_Nclass.csv`.

| N | bins (h+ + h−) | robust pull width | cov ±σ_stat | stat+IBU budget | RAPGAP truth only |
|---|---|---|---|---|---|
| 1 | 3 | 1.04 | 0.661 | 0.702 | 0.655 |
| 2 | 3 | 1.13 | 0.645 | 0.676 | 0.614 |
| 3 | 1 | 1.09 | 0.538 | 0.606 | 0.657 |
| 4–5 | 6 | 1.22 | 0.590 | 0.623 | 0.659 |
| 6–9 | 3 | 1.23 | 0.558 | 0.607 | 0.678 |
| 10–19 | 16 | 1.25 | 0.573 | 0.618 | 0.678 |
| 20–49 | 23 | 1.31 | 0.554 | 0.606 | 0.675 |
| 50–99 | 33 | 1.24 | 0.582 | 0.621 | 0.668 |
| 100–499 | 95 | 1.22 | 0.593 | 0.634 | 0.675 |
| ≥500 | 307 | 1.22 | 0.592 | 0.658 | 0.672 |

- Binomial errors are 0.0004–0.006.
- Adding the full 26-term budget gives 0.70–0.94. That is only an upper bound, because the other 23 terms widen the intervals but never shift the toys.

**Per truth.**
- stressA covers 0.23–0.57; it is worst at N=3 and N=6–9.
- djangoh_djresp at N≥500 covers 0.51 with pull width 1.47, a response-model bias.

**Tails at N≤5.** The plain pull std is 4–34. In h+ bin 179 (z<0.05, pT 4–8 GeV, N=2) under stressB, toys that see no events quote σ≈1 against a truth of 35. The inner replicas resample the toy's own rounded data, so an empty toy cannot know its true rate. (This explanation is an inference.)

**Worst bins** (pooled coverage / minimum over truths):

| bin | Q² (GeV²) | z | pT (GeV) | N | coverage |
|---|---|---|---|---|---|
| h+ 203 | 700–1500 | 0.5–0.7 | 2–4 | 304 | 0.23 / 0.01 |
| h+ 57 | 150–300 | 0.3–0.5 | 1–2 | 6001 | 0.28 / 0.07 |
| h− 62 | 150–300 | 0.5–0.7 | 1–2 | 1191 | 0.28 / 0.00 |
| h− 57 | 150–300 | 0.3–0.5 | 1–2 | 5374 | 0.28 / 0.01 |
| h+ 244 | 1500–20000 | 0.7–1.0 | 4–8 | 56 | 0.31 / 0.10 |
| h+ 125 | 300–700 | 0.3–0.5 | 0–0.5 | 948 | 0.32 / 0.00 |

Full list: `analysis/per_bin_data_pooled.csv`.

**Response MC statistics.**
- In respmc mode the pull width is ×1.00–1.04 that of data mode.
- For the RAPGAP truth, coverage drops from 0.667–0.678 to 0.649–0.667. Adding diag C_MCstat brings it to 0.666–0.693.
- The pure response-MC spread over √diag C_MCstat is 0.93–1.01 for N≥50 and 0.5–0.95 for N<50, so C_MCstat is adequate or conservative.

### Diagnostics

**Iterations.** The scan uses k = 1, 2, 3, 4, 5, 6, 8, 10, 50 and 100; k=7 and k=9 were not run. σ is scaled by spread_k/spread_4, a proxy because σ is stored only at k=4.
- Coverage rises with k: for N≥500 it is 0.25 at k=1, 0.59 at k=4 and 0.65 at k=50–100.
- No k reaches 0.68±0.03 in every class; N=2 and N=10–19 never reach it.
- In the large-k limit, the bias vanishes only in dense bins (N≥500: 0.40% → 0.08%). In sparse bins it grows (N=1: 21% at k=4, 41% at k=100).
- The spread reaches 1.5–3× the BBB stat, as for a matrix inversion.
- The DJ-response bias does not depend on k: even oracle coverage stays at 0.66–0.67.

**Neighbour correlations.** The median correlations from the pseudo-experiment ensemble and from the quoted toys agree within 0.02 in every class. The release values differ from them by up to 0.04. Examples: −0.03 for N≥500, +0.50 for N=3, +0.36 for N=6–9. They depend on the bin topology, not on N alone.

**Interval shape at N<10.**
- The unfolded distribution has skewness 1.27 at N=1 and 1.25 at N=2, and 0.3–0.4 above that.
- Exact inner-replica 16/84% percentile intervals cover worse than Gaussian ±σ (0.51–0.62 against 0.54–0.66), because the replicas inherit the pull toward the prior.
- Garwood asymmetric intervals cover 0.57–0.76.
- Asymmetric intervals do not fix the bias.

## 3. Candidate fixes

Coverage per N class. F0–F4 are tested leave-one-truth-out, against all 6 truths.

| fix | 1 | 2 | 3 | 4–5 | 6–9 | 10–19 | 20–49 | 50–99 | 100–499 | ≥500 |
|---|---|---|---|---|---|---|---|---|---|---|
| F0 current stat | .661 | .645 | .538 | .590 | .558 | .573 | .554 | .582 | .593 | .592 |
| current stat+IBU budget | .702 | .676 | .606 | .623 | .607 | .618 | .606 | .621 | .634 | .658 |
| F1 inflate by pull width | .687 | .698 | .576 | .681 | .654 | .678 | .679 | .684 | .683 | .684 |
| F2 stat ⊕ bias (all truths) | .843 | .709 | .835 | .729 | .785 | .720 | .706 | .682 | .680 | .676 |
| F3 Garwood asymmetric | .716 | .760 | .569 | .675 | .608 | .615 | .583 | .607 | .608 | .597 |
| F4 best k (proxy σ) | .681 | .643 | .683 | .656 | .645 | .641 | .657 | .659 | .665 | .650 |
| **R** stat ⊕ ibu_model ⊕ ibu_reg ⊕ closure, family held out | .782 | .693 | .752 | .668 | .670 | .695 | .680 | .670 | .683 | .694 |
| R, plausible truths only (rapgap, djangoh, djresp, datalike) | .798 | .688 | .764 | .700 | .717 | .718 | .731 | .711 | .714 | .707 |
| R without ibu_model | .756 | .666 | .718 | .648 | .640 | .673 | .653 | .645 | .659 | .646 |
| R′ stat ⊕ closure′ (DJ, DJ-resp, datalike; model/reg dropped) | .748 | .667 | .705 | .649 | .651 | .664 | .649 | .640 | .656 | .676 |

**Definitions.**
- closure = per-bin rms over {djangoh with RG response, datalike} of mean(unfolded_k4 − truth).
- Held-out test. Truths fall into families: DJ = {djangoh, djangoh_djresp}, DL = {datalike, stressA, stressB} and RG = {rapgap}. The stressed truths are datalike × corner factors; their bias correlates with datalike at r≈0.82–0.85. Each test truth uses a closure trained only outside its own family: DJ tests use datalike, and DL tests use djangoh, whose bias correlates with datalike at r≈0.005. These correlations come from a read-only review computation over per-bin bias_rel in `analysis/per_bin_all_groups.csv` (data mode, 490 bins); `eval_treatment_v2.py` does not output them.
  - A first version held out only the identically named truth. It gave 0.68–0.70 for N≥4, which is close to the family-held-out values.
- R per truth (family held out), range over N classes: rapgap .66–.84, djangoh .68–.81, djangoh_djresp .62–.80 (lowest, 0.62, at N≥500), datalike .62–.77, stressB .62–.85, stressA .48–.72.
- Using the max instead of the rms over training truths (R2) changes coverage by ≤0.01, except for rapgap.
- Source: `analysis/treatment_R_v2.json`; script `eval_treatment_v2.py`.

**Overlap with ibu_model.** On the nominal release:
- The ratio closure/ibu_model is 0.82 at the median and 2.88 at p90.
- |closure_DJ| and ibu_model correlate at r=0.65. The signed correlation is −0.45, opposite in sign by convention.
- So the two terms partly measure the same DJ-vs-RG prior dependence, and R double-counts part of it.
- Dropping ibu_model ("R without ibu_model") still costs 0.02–0.05 in coverage and leaves N≥500 at 0.65. On these numbers the overlap is acceptable rather than harmful. That judgement is an inference.

**Effect of R on the nominal release.**
- closure/σ_stat: median 0.32 over all bins; median 0.71 and range 0.06–1.82 for N<10.
- IBU part of the uncertainty, new/old: median ×1.04, p90 ×1.35, max ×2.16; median ×1.20 at N<10.
- Adding Garwood on top of R for N<10 over-covers (0.71–0.82) and adds nothing.
- Source: `analysis/treatment_R.json`.

**Cost and covariance per fix.** These estimates come from the budget trace; rerun times are taken from the logs.

| fix | stages rerun | covariance | cost |
|---|---|---|---|
| F1 inflate | sidecar → groupA → assemble → s08/s09 → install | C_stat → D·C·D | ~1 day |
| R (bias/closure term) | closure vectors from existing toys → new budget source → s08/s09 → s15/s16 → tables/plots | new rank-2 term ½Σ_t b_t b_tᵀ | ~2–3 days |
| F3 asymmetric | release writer and tables | none; display only, χ² users stay Gaussian | small–medium |
| F4 change k | every unfolded leg, all of phaseD, install | everything rebuilt | ~1 week |
| bin merging | full campaign (binning pinned at prod) | rebuilt | largest |
| flag N<10 | flag column; plotters (which assert all bins supported) | unchanged | small |

## 4. Practice in published papers

- **H1 OmniFold** (arXiv:2108.12376, 2303.13620): bootstrap of the data with the MC fixed. Djangoh-vs-Rapgap model and closure (Djangoh pseudo-data unfolded with Rapgap) are separate systematics. The number of iterations is chosen so that closure does not dominate.
- **H1 multiplicities** (arXiv:2011.01812): TUnfold; wide bins at large N "to avoid large negative bin-to-bin correlations".
- **H1 and ZEUS strange hadrons** (arXiv:0810.4036, hep-ex/0612023, 1111.3526): bin-by-bin correction, with model bias from CDM vs MEPS. ZEUS drops bins with acceptance < 0.05.
- **HERMES** (arXiv:1212.5407): direct smearing-matrix inversion with an analytic covariance; MC-model variations as systematics.
- **COMPASS** (arXiv:1604.02695): acceptance correction; RICH matrix varied within its errors.
- None of these papers shows a coverage or pull study, uses Poisson or asymmetric intervals for unfolded points, or states a minimum-count rule.
- **Method literature:**
  - Adye (arXiv:1105.1160): D'Agostini errors are underestimated without the full propagation; toys are the reference.
  - Kuusela and Panaretos (arXiv:1505.04768): regularised intervals "may suffer from severe undercoverage" because of bias.
  - PDG 2024 §40.2.5: discusses the bias-variance trade-off and recommends publishing the folding ingredients.

A closure term quoted separately from the model term therefore follows H1 practice. A coverage study like this one goes beyond published practice.

## 5. Recommendation

**Adopt R.**

1. **Closure term.** Add a per-bin **unfolding closure** source b = rms over {DJANGOH through the RAPGAP response, data-like reweighted truth} of mean(unfolded − truth) at k=4.
   - Keep ibu_model and ibu_reg as well. They overlap partly with b (r=0.65), but dropping ibu_model loses 0.02–0.05 coverage (§3).
   - Expected coverage with families held out: 0.67–0.70 pooled at N≥4, and 0.70–0.73 on plausible truths.
2. **Covariance.** Use ½Σ_t b_t b_tᵀ over h+⊕h− with the same truth for both charges. This is rank 2, and its diagonal equals the quoted term.
   - The shared bias partly cancels in the ratio and asymmetry columns.
   - This conclusion is an inference; it has not been computed.
3. **Toys.** Compute b from the toys already made. No new events or toys are needed.
4. **Sparse bins.** Keep k=4 and symmetric errors. Flag N<10 bins in the tables and in the note, with the statement that their errors carry a prior-dependence term. The 4-event bins can then be published with an error that covers.
5. **Stressed truths.** Report the stressed truths as a robustness test, not as a quoted term.

**Code and cost.**
- The change touches about 8 files in `esigma_final_2026-10-24/`: the budget, s08/s09, s15/s16 and assemble. The term counts go from 26 to 27 quoted and from 28 to 29 in the covariance.
- Cost is in §3: about 2–3 days, with a new release tag.
- Gate: before the rebuild, rerun `eval_treatment_v2.py` on the vectors as built.

**Runner-up R′.** Make one closure envelope over {djangoh, djangoh_djresp, datalike} and drop ibu_model and ibu_reg.
- It costs the same.
- It under-covers slightly: 0.64–0.68 at N≥4, family held out.
- It is preferable only if the user wants a single unfolding systematic.

**What R does not cover.**
- stressA: 0.48–0.62 for N=3–499 (0.66 at N≥500).
- The DJ-response truth at N≥500: 0.62.
- R is calibrated on plausible truths, not on arbitrary shape distortions.
- If the true spectrum departs from RAPGAP as strongly as stressA, these bins under-cover.

## 6. Accepted harness deviations and their likely effect

| Deviation | Effect on the conclusions |
|---|---|
| Background template fixed | √varB/σ_stat ≤ 0.016 (median): negligible. The release's inner toys omit the background's Poisson variance, so pull inflation is √(N_raw/N_sub) = 1.01–1.10. That is real, already inside the measured widths, and small. |
| Other 23 budget terms held fixed | They widen the intervals but never shift the toys, so full-budget coverage (0.70–0.94) is an upper bound. The stat+IBU conclusions are unaffected. |
| Negative toy vectors unfolded, not refused | 0.3–16% of toys are affected. Excluding them moves sparse coverage by ≤0.004. The 101 non-finite analytic-error toy-bins are unused. |
| Only the RG response varied in respmc | The DJ legs' own MC noise is comparable to ibu_model (ratio 0.8–1.7) and is untested. This may slightly understate the ibu_model noise (inference). |
| k=7 and k=9 not run in the scan | Expected to be negligible: the coverage trend from k=6 to 10 is smooth in `iteration_scan.png`, though not checked bin by bin (inference). |
| Per-toy inner seeds | 2.2% σ noise, which changes coverage by <0.005 (estimated). |
| datalike truth = nominal unfolded result | It is biased toward the prior. Its shape still differs from the prior by 9–46%, more than DJANGOH, so this does not make the study optimistic for these truths. The real truth could lie further away (inference). |
| σ stored only at k=4 | The iteration-scan coverage uses a proxy σ, so the k-scan conclusions are approximate. |

## 7. Open questions and gaps

- **User choices:** whether to accept R (closure split from model, H1 style) or R′ (single term), and whether to flag N<10 bins in the letter's tables.
- **Not checked:** ZEUS Bayesian-unfolding papers, and the per-paper systematics tables of the H1 OmniFold papers.
- **Not computed:** R's effect on the asymmetry and ratio columns, and on the χ² of the theory comparisons.

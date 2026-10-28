# Decision-Ready Unfolding Closure Plan — v6/z005, 490-bin Contract

**Scope and decision context.** This plan covers only the high-Q2 inclusive charged-hadron SIDIS v6/z005 contract: 245 published truth bins per charge, 490 published charge-major rows, 490 reco bins per charge, a `(491,246)` response per charge, and a 980-element combined reco vector. It is an audit and validation plan; it does not authorize replacement of the central release. The current central candidate is IBU k=4 with BBB as a diagonal cross-check. Native v6 miss-underflow TUnfold remains diagnostic-only.

> STATUS 2026-10-06: the Section 1 controlled coherent-prior k=4 comparison is COMPLETE; see `docs/unfolding/coherent_prior_fix_results_2026-10-05.md` for the validated fix, its k=4 impact, and the adoption-pending requirements (COHERENT-PRIOR lineage, not yet adopted in central).

## 1. Response Truth Conservation Failure (highest priority)

**Problem.** The central RAPGAP response fails truth-side conservation in every published truth bin. The response is oriented as `M[reco,truth]`: its 491 rows are 490 physical reco bins `i=0..489` plus the miss row `i=490`; its 246 columns are 245 physical truth bins `j=0..244` plus the fake column `j=245`. Thus, for each physical truth column, the only correct conservation check is

`T_response[j] = sum_{i=0}^{489} M[i,j] + M[490,j]`.

Equivalently, sum the physical body **down the reco axis** (`axis=0`) and add the one miss-row element in that same truth column. One must not sum the miss row row-wise; that produces a scalar with no truth-bin meaning. The stored `h_prior_gen_{pos,neg}` should equal this truth marginal if it is the prior used to normalize the same response.

**Current evidence and numerical severity.** The supervisor's column-wise audit finds the following for the central RAPGAP response, all 245 bins in each charge failing at a tolerance of `1e-6`:

| charge | `sum(body)+miss` | stored prior | total difference | `(body+miss)/prior`: min / median / max | bins off |
|---|---:|---:|---:|---|---:|
| h+ | 91,879,281.4817 | 91,784,225.0791 | +0.103565% | 0.964168 / 0.997185 / 1.028029 | 245/245 |
| h− | 90,271,538.7361 | 90,185,163.1419 | +0.095776% | 0.965375 / 0.994197 / 1.028519 | 245/245 |

The total offset is small but not the relevant diagnostic: the per-bin nonconstant scatter is approximately ±2.8%. The audit also found a finite, non-negative `(491,246)` matrix, efficiencies h+ `[0.0254083,0.7647282]` and h− `[0.0260073,0.7644560]`, and fake fractions h+ `[0,0.3224391]`, h− `[0,0.3067053]`. Therefore this is not an empty-bin or invalid-response explanation.

**Authoritative fill-path audit.** The release response is the finalcandidate-specific builder, not the broad-envelope helper with the similar name:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`.

Its decisive paths are:

* `compute_gen_bin()` defines the gen-pool selection and `w_gen = weight1*weight2`; the stored prior is filled from eligible `ChargedTotGen` rows in lines 873–886 (`prior_gen += w_gen`). It is written as `h_prior_gen_{tag}` by the ROOT writer.
* The matched body is filled from `ChargedTot` after reco and inlined truth selection, in lines 888–929. Its matched weight is `w_reco = weight1*weight2*NucliaWeight` (the corresponding documented production logic is the same as the canonical helper at `scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py:591–600`); the fill is `R[ir,ig] += w_reco`.
* The non-consumed truth row is sent to the miss row using `w_gen`, i.e. `R[490,ig] += weight1*weight2` (same canonical helper lines 852–875).
* The intended coherent alternative is the NW-effective prior: a matched truth row gets `w_gen*NW_partner`, while a miss gets `w_gen`; see the canonical helper lines 877–900 and writer lines 1008–1021. In the finalcandidate layout this is the object that should equal body plus miss, subject only to consumed-key matching correctness.
* The central release producer explicitly loads `prior_gen` and passes it as IBU `P` in `_run/run_bbb_ibu_release.py:74–82`; the IBU implementation initializes `n^(0)=P` and builds `A_ij=B_ij/P_j` in `crosscheck_unfold_methods_v4.py:623–681`.

**Most likely explanation.** A **weight mismatch** is the best-supported cause: matched physical-body entries include `NucliaWeight`, whereas the stored legacy prior and the miss row use only `weight1*weight2`. This predicts a bin-dependent, rather than constant, ratio because the matched fraction and the NucliaWeight distribution vary across truth bins. It is also precisely the issue for which `prior_gen_NWeff` was introduced. A selection mismatch remains a secondary possibility because the body uses inlined matched truth while the prior/miss use `ChargedTotGen`; a consumed-key collision or mismatch could also produce a bin structure. Double counting is disfavoured by the small signed total difference and the internally coherent flat-phi fork. A genuine truth-fiducial difference is not supported: the finalcandidate matched-side cuts were explicitly designed to mirror the gen-pool fiducial selection, and the flat-phi fork preserves exact truth conservation.

The independent flat-phi_h fork is important discriminating evidence, not merely a side result. With its coherent truth-leg policy (prior and miss at gen phi_h; matched reco at the matched truth partner phi_h; fakes unity), every unity/flat response has `(body column sum + miss)/stored study prior = 1/1/1` with zero bins off `1e-6`. It also reproduces central acceptance at median difference zero. This points to the stored-prior provenance/weighting in central products, rather than a generic failure of response construction or the matrix orientation.

**Does this bias central IBU k=4?** Yes: the central IBU response normalization and initial prior are internally inconsistent, so the k=4 values are not presently validated as unbiased. The issue is not repaired by the small integrated (+0.10%) difference.

A precise first-iteration statement is possible. With `P_stored` used both as the starting prior and as the column denominator, IBU iteration 1 carries the factor `P_stored/(body-column sum)` after the data-driven Bayes assignment. Replacing it by the truth-conserving marginal changes that local factor by `1/r-1`, where `r=(body+miss)/P_stored`. The audited ratios imply an exact first-iteration local normalization change spanning **−2.727% to +3.717%** (h+: −2.727% to +3.716%; h−: −2.772% to +3.587%). This is an appropriate scale and a hard demonstration that the problem is binwise, not an integrated 0.1% nuisance.

It is **not valid** to call ±3.72% a rigorous k=4 bound: later IBU iterations couple truth bins through the response and can attenuate or amplify the change. No existing artifact isolates the corrected-prior versus legacy-prior k=4 delta, so a numerical k=4 bound has not been established. Until the required controlled comparison is made, the only defensible statement is: central k=4 may have a binwise bias of order the observed 2.8% response/prior mismatch, with an exactly demonstrated iteration-1 envelope of −2.77% to +3.72%.

**Is BBB acceptance affected?** The currently used diagonal acceptance is `A_j = sum_i B[i,j]/(sum_i B[i,j]+M[j])`, derived solely from body plus miss (`crosscheck_unfold_methods_v4.py:535–562`). It is therefore internally response-conserving and is **not altered by which standalone prior histogram is stored**. The same statement applies to a response-derived acceptance only if it is calculated from body+miss; any downstream acceptance implementation that divides by `h_prior_gen` is affected and must be searched/audited. BBB remains subject to its independent migration limitations and does not validate IBU.

**Exact discriminating test and existing script path.** Run a noncentral, isolated reproduction of the finalcandidate builder while instrumenting the existing finalcandidate script above. For each `(charge,truth bin)`, save four marginals from the same event loop: `(i)` legacy `w_gen` prior, `(ii)` `w_gen*NW_partner` for consumed/matched truth and `w_gen` for misses (NW-effective prior), `(iii)` body column sum, and `(iv)` miss. Compare `body+miss` to both priors at absolute and relative precision, and additionally count/report the categories that differ between `ChargedTotGen` and inlined matched truth selection. The existing directly relevant run path is the finalcandidate builder named above; the authoritative reference implementation for NW-effective filling is `scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py:769–900`. The release driver to use for an isolated corrected-prior-only IBU comparison is `output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/run_bbb_ibu_release.py`.

**Acceptance criterion.** For every charge and all 245 truth bins, require `abs((body+miss)/P_coherent - 1) < 1e-6` (or a documented floating-point aggregation tolerance no weaker than `1e-8` relative where weights permit), no non-finite/negative entries, `0 <= A_j <= 1`, and an explicit stored reco marginal satisfying `body row sum + fake` where defined. The controlled corrected-prior k=4 run must report all 490 per-bin changes, integrated changes, changes in the k=3/4/5 regularization variation, and self/cross closure. Release acceptance requires every retained bin to be covered by the repaired closure result; large shifts may not be hidden in an integrated number.

**Recommended default.** Stop release promotion, preserve the existing central product as an audit reference, and repair the stored IBU prior to the coherent NW-effective truth marginal (or equivalently make all matched body weights and the truth marginal use one documented weight convention). Do not merely rescale the stored prior globally. Rebuild/merge the response in a new study/release candidate, rerun the response conservation audit, then rerun IBU k=1–10 and all closure/refold tests before any central replacement. BBB acceptance can be retained only after confirming it is calculated from body+miss and after the repaired response passes the stated audit.

## 2. Nominal Method Decision (P-METHOD)

**Problem.** The analysis must decide whether the nominal migration correction remains the existing IBU k=4/BBB finalcandidate or whether native v6 `TUnfoldDensity` with misses on the reco-axis underflow is promoted. This decision is downstream of Section 1: neither method can be promoted on an internally inconsistent truth normalization.

**Current evidence.** Native v6 TUnfold uses the technically correct current geometry: 490 physical reco by 245 truth bins per charge; physical matched entries in ordinary bins; truth misses in reco/input underflow; only physical reco bins passed to `SetInput`; `GetOutput()` read on the full truth-fiducial scale. Geometry and `SetInput` status pass. The unregularized native/IBU median ratios were 0.99175 (h+ RAPGAP), 0.99273 (h+ DJANGOH), 0.99129 (h− RAPGAP), and 0.99062 (h− DJANGOH), without an invalid post-hoc efficiency correction.

However, every physics-promotion gate is either failed or incomplete:

| promotion gate | current status and evidence |
|---|---|
| Poisson-fluctuated closure with bias, variance, coverage | **MISSING.** Existing native cross-generator closure is explicitly `deterministic_arithmetic_closure_only_not_poisson_validation` at tau `1e-8`; it is not a statistical closure. |
| Fake/background covariance or `SubtractBackground()` | **MISSING.** Phase 1/2 uses external fake treatment; no complete fake/background covariance or registered `SubtractBackground()` treatment is demonstrated. The central background audit also finds no fake-column, response-MC, or background scale/shape covariance. |
| physics-valid tau choice | **FAIL.** The scan's diagnostic selections are interior but have refold chi2/ndf 607–2267 and 7–9 negative bins. The selection rule minimizes negative bins first, then distance to 1, and is explicitly diagnostic-only—not a validated L-curve/global-rho/closure selection. |
| response/MC systematic propagation | **MISSING.** A response swap exists as a diagnostic, but native TUnfold response-statistical covariance and complete response/fake/background systematic propagation are not established. |
| data refold chi2 | **FAIL.** Across 80 scan points, chi2/ndf minimum/median/p90/max is 3.85/9.77/1415/2829. The selected points are much worse: 607–2267. |
| negative bins | **FAIL.** The scan has 7–24 negatives (median 11). The tau `1e-8` cross closure has 2–4 negatives per charge/direction. |
| tails and high-rho bins | **FAIL/INCOMPLETE.** At tau `1e-8`, 30.2–32.7% of bins have absolute cross-closure residual above 5%, 9.4–11.0% above 25%, and maximum global rho is 0.988–0.998. No accepted tail/high-rho masking or merge policy is defined. |
| covariance PSD, weighted-input variance and background accounting | **MISSING.** No published native covariance validation demonstrates all three. |

The deterministic tau `1e-8` closure is useful only as an algebra/smoke test: it has `SetInput=0` and `DoUnfold=0`, but refold chi2/ndf is 57.0–65.3, 2–4 negatives remain, and it cannot establish coverage. It must not be relabelled a pass.

**Candidate explanations.** The main possibilities are not a simple TUnfold API failure—the native miss-underflow encoding and input dimensions pass—but insufficient model/covariance treatment, a response/data mismatch in fake/background treatment, an unsuitable curvature regularization/topology, and low-support tail bins. The very large refold chi2 indicates that changing tau alone is not a credible remedy.

**Exact discriminating test and existing script path.** The current native suite is at

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v6_native_miss_z005/`

with `build_native_miss_inputs_v6.py`, `validate_native_miss_inputs_v6.py`, `unfold_native_miss_single_tau_v6.C`, `produce_phase2_native_diagnostics_v6.py`, and `finalize_native_phase2.py`. The minimum discriminating study is a statistically independent response/pseudo-data split for each generator, at least 100 Poisson toys per charge and response/pseudodata direction, using the same fake/background convention once and its covariance. For each tau, report bias, RMS, pull mean/width, 68% coverage, refold chi2/ndf, negative bins, covariance PSD, rho distribution, and tail-bin failures. Select tau only from points surviving all hard gates; then repeat with swapped RAPGAP/DJANGOH response.

**Acceptance criterion.** A candidate tau must be non-endpoint and survive predeclared gates: no negative published retained bins, covariance PSD within numerical tolerance, accepted background/fake covariance treatment, data and toy refold chi2/ndf compatible with the stated variance model, no unaddressed high-rho/tail failures, Poisson-toy pull mean compatible with zero and pull width/68% coverage compatible with one within toy statistics, and response/MC variations propagated. If no tau survives, TUnfold remains a diagnostic.

**Recommended default.** Keep **IBU k=4 nominal with BBB as a diagonal cross-check; retain native v6 TUnfold as a qualified cross-check, not a promotion project.** This is the lower-risk decision near completion because native TUnfold currently fails refolding and negativity gates and lacks Poisson closure/covariance, while the Section 1 repair is mandatory for the existing central result. Promotion would require a materially new validation campaign with no evidence yet that a valid tau exists.

For a credible published cross-check—not nominal—minimum work is: repair Section 1 first; freeze the fake/background convention and covariance; run split-MC Poisson closure including both generator directions; issue a tau scan with refold/PSD/negative/rho diagnostics; document the selected diagnostic tau or explicitly state that no physics-valid tau exists; and publish a comparison only over retained bins with failures separately flagged. Do not add method spread as a systematic.

## 3. Regularization and k=4 Justification

**Problem.** The central release selects IBU `k=4`, but the analysis note currently asserts broad stability for `N_iter<10` without documenting a quantitative, reproducible stopping rule. Given the Section 1 prior inconsistency, the pre-repair k=4 rationale cannot be treated as final.

**Current evidence with numbers.** The central release saved IBU iterations 1–10 and selects `KSEL=4`; its regularization component is explicitly the k=3/k=5 variation in the release producer. The release metadata reports median k=3/5 regularization uncertainty of 8.4929 unfolded-yield units for h+ and 8.5923 for h− (these are not a stopping criterion). The analysis note says IBU is stable in most bins below 10 iterations and claims cross-generator closure qualitatively, but it supplies neither a numerical iteration-to-iteration convergence table nor Poisson coverage/pull evidence. The central IBU implementation agrees with RooUnfoldBayes to floating precision (maximum relative difference `2.6e-15`), which validates implementation equivalence but not selection of k=4.

**Candidate explanations.** k=4 may be a sensible empirical compromise between residual prior bias at low k and noise amplification/model sensitivity at high k. But a visual iteration scan, a k±1 envelope, and code agreement cannot by themselves demonstrate that compromise. The observed flat-phi response sensitivity is amplified by IBU (median 3.07x DJANGOH and 3.41x RAPGAP relative to acceptance-only), reinforcing the need for an explicit bias–variance trade-off rather than an inherited iteration count.

**Exact discriminating test and existing script path.** After the Section 1 repair, use the current release driver

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/run_bbb_ibu_release.py`

and the reusable IBU core/closure path

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/ibu/run_ibu_closures.py`

(or a finalcandidate-geometry adaptation; the checked-in generic script is historical 960x240 and must not be run unchanged against z005). Perform k=1–10 split-MC Poisson toys for same-generator and cross-generator closure. For each k, report per-bin and aggregate bias, RMS/variance, pull mean/width, 68% coverage, negative bins, data refold chi2, and change from k−1. Define the final test in the 245-bin-per-charge geometry, not a superseded 240-bin result.

**Acceptance criterion.** Promote k=4 only if it lies on a documented plateau: k=4 and adjacent accepted k values have statistically compatible closure bias and refold performance; k=4 has acceptable coverage; and increasing k produces no material bias reduction relative to its variance/model-sensitivity increase. The note must state the metric, toy count, statistical independence, response/prior/pseudodata generators, bin mask, and the numerical k=1–10 table. If k=4 is not on that plateau, select the predeclared best passing k and propagate the selection change through the release.

**Recommended default.** Retain k=4 provisionally after the response repair only as the current candidate, not as a defended final choice. The immediate objective is a short, reproducible k=1–10 bias–variance and coverage scan; it is substantially cheaper and more decision-relevant than promoting TUnfold.

## 4. Closure and Null Tests Inventory

**Problem.** Existing tests are useful but heterogeneous: some are implementation/algebra checks, some are deterministic generator comparisons, and some are response-shape sensitivity studies. They must not be presented as a completed statistical closure suite.

**Current inventory of completed tests.**

| test | status | result and limitation |
|---|---|---|
| IBU implementation equivalence | PASS | In-house IBU reproduces RooUnfoldBayes to maximum relative difference `2.6e-15`; this is a code check, not physics closure. |
| central response finite/range audit | PARTIAL PASS | No negative/NaN/Inf entries; efficiencies and fake fractions are within [0,1], but truth conservation fails in 245/245 bins per charge. |
| IBU cross-generator deterministic closure | PARTIAL / NOT SUFFICIENT | The analysis note has RAPGAP↔DJANGOH cross-closure figures. They are deterministic pseudo-data diagnostics and do not establish toy coverage, independent response/pseudodata closure, or corrected-prior validity. |
| native TUnfold deterministic cross closure | FAIL for promotion | At tau `1e-8`, refold chi2/ndf 57.0–65.3, 2–4 negatives, 30–33% bins above 5% residual, high rho; explicitly not Poisson validation. |
| central data IBU statistics | PARTIAL PASS | 1000 data-Poisson toys per charge replace the former 100-toy covariance; response and fake fraction are fixed, hence response-MC and fake/background covariance are absent. |
| flat generator-level phi_h response sensitivity | PASS as diagnostic | Exact truth-conserving fork; IBU k=4 has median absolute flat/unity shift 0.5372% DJANGOH14 and 0.5364% RAPGAP31, zero negative outputs. |
| flat-phi per-bin refinement / half-sample null | PASS as interpretation control | Fine per-bin apparent shifts are noise dominated: DJANGOH median perbin/halfA/cell 1.0183%/1.0011%/0.5372%; RAPGAP 0.9589%/0.9737%/0.5364%. No hidden large z/pT dependence is established. |
| acceptance-only versus IBU flat-phi comparison | PASS as diagnostic | BBB median shift 0.1752%/0.1572%, versus IBU 0.5372%/0.5364%, so IBU amplification is 3.07x/3.41x (DJANGOH/RAPGAP). |

**Missing tests before release.**

1. **MC self-closure:** response and pseudo-data must be statistically independent (split MC or independent samples), with RAPGAP and DJANGOH each used in both roles where available. The current same-sample/deterministic tests are insufficient, and the Section 1 mismatch invalidates a claim of full central self-closure.
2. **Half-sample/split-MC closure:** required to expose response statistical effects and prevent the same MC fluctuations from appearing in both response and pseudo-data. It must run at k=1–10 and include coverage.
3. **Reweighted-truth closure:** the flat-phi study establishes response sensitivity, not closure to an injected non-nominal truth. Build a pseudo-data sample with known truth modulation/reweighting, use an independent nominal response, and test recovery. Its defensible pre-existing sensitivity target is iteration-1 cell-level median `|delta|=0.5372%` DJANGOH14 and `0.5364%` RAPGAP31; the fine per-bin apparent 1% shifts are not a validated model signal because the half-sample null reproduces them.
4. **Data refold chi2:** calculate and publish a consistent covariance-aware refold chi2 for central IBU k=4 after repair. The release does not contain an accepted central data-refold result. Use the fake-subtracted reco vector and the exact fake/background covariance convention once.
5. **Pull tests:** run at least 100 Poisson toys (preferably more) for each closure direction; report pull mean, width, 68% coverage, failed bins, and separate data-Poisson from response-MC fluctuations.
6. **Response-prior conservation regression:** this is a release gate, not optional QA: assert column-wise equality before any closure run and preserve the marginal histograms in the artifact.

**Model-systematic disposition.** The existing nominal model component is the absolute difference of data unfolded with RAPGAP versus DJANGOH response. The flat-phi result is approximately one third of the reported unity generator-to-generator median spread (1.61%) and is therefore plausibly encompassed numerically at the median. But **it is not demonstrated to be covered bin-by-bin**, and the study is explicitly diagnostic/not systematic. Do not silently declare it covered and do not add an additional source yet. The required reweighted-truth closure must compare the flat-phi shift vector against the already-defined model vector and its uncertainty convention: if every retained-bin injected-truth bias is covered, document it as coverage validation; if not, define a non-overlapping additional response-model source with a correlation prescription through the systematics workflow. Method spread remains a diagnostic, not a quadrature source.

**Exact discriminating tests and existing script paths.** The existing IBU closure driver is `scripts/ibu/run_ibu_closures.py`, but it has hard-coded historical 960x240 dimensions and a pseudo-data miss-overflow convention; it is a template, not directly runnable for the 490x245 finalcandidate contract. The finalcandidate release driver is the correct geometry reference. The flat-phi response and IBU scripts are at

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/flatphi_genreweight_z005_20260925/`

with the response construction under `response/` and IBU sensitivity driver under `unfolding/run_ibu_flatphi_sensitivity.py`. Native TUnfold's corresponding current scripts are in the Section 2 path. New closure production belongs under `output_v6/studies/`, never under the central release.

**Acceptance criterion.** For each closure direction and retained bin, recover truth within statistical uncertainty at the predeclared coverage rate; no coherent bin-group bias may remain. Require pull mean consistent with zero, pull width and 68% coverage consistent with nominal within toy uncertainty, no uninvestigated negative values, and a covariance-aware refold chi2/ndf compatible with the adopted data model. Report failing tail bins explicitly; merge/drop only with an approved publication-bin policy. A deterministic `R·truth` recovery is recorded only as an arithmetic smoke PASS and cannot satisfy any of these gates.

**Recommended default.** Treat completed tests as diagnostic evidence only. Following the Section 1 repair, prioritize a single finalcandidate-geometry split-MC IBU closure framework that delivers self/cross closure, reweighted closure, refold, and pulls in one artifact. This is the shortest path to defensible IBU k=4 validation and a clean analysis-note update.

## 5. Dependency-Ordered Task List

| order | task and deliverable | blocks / dependency | release gate or parallel work |
|---:|---|---|---|
| 1 | Freeze an audit copy of the central response/release hashes and write the column-wise marginal regression test. | Nothing; prevents accidental reinterpretation or overwrite. | **Release gate.** |
| 2 | Instrument/rebuild the finalcandidate response in an isolated study to compare legacy, NW-effective, body, and miss marginals; trace selection-category differences if NW-effective is not exactly conservative. | Requires the actual finalcandidate builder, not the broad-envelope helper. | **Highest-priority release gate.** |
| 3 | Select and implement the coherent convention (expected: NW-effective stored IBU prior), create a new response candidate, and validate truth/reco marginals, axes, finiteness, acceptance, fake fractions, and sumw2 persistence. | Depends on the root-cause result from task 2. | **Release gate.** |
| 4 | Run the controlled corrected-prior IBU k=1–10 comparison and quantify the exact central k=4 shift, including total and per-bin shifts. | Depends on task 3. | **Release gate.** This closes the currently unquantified k=4 bias. |
| 5 | Build finalcandidate-geometry split-MC toy closure: self/cross generator directions, k scan, pulls, coverage, refold, response-MC toys, and reweighted-truth injection. | Depends on tasks 3–4; uses the repaired response. | **Release gate.** |
| 6 | Select/defend k with predeclared bias–variance/coverage metrics and update the analysis-note unfolding claims only with results from task 5. | Depends on task 5. | **Release gate.** |
| 7 | Make P-METHOD decision: retain IBU/BBB nominal unless native TUnfold independently satisfies its full promotion gates. | IBU decision depends on tasks 3–6; native TUnfold promotion also needs a coherent repaired response. | **Release gate for method declaration; not a reason to delay IBU repair.** |
| 8 | Minimum credible native-TUnfold cross-check: fake/background covariance, split-MC Poisson closure, tau/rho/refold/PSD/negative diagnostics, and response-systematic study. | Can design in parallel, but final inputs depend on task 3. | **Parallel; not required to keep IBU nominal.** |
| 9 | Compare the injected flat-phi closure shift vector with the existing RAPGAP–DJANGOH model vector and decide/document coverage versus an additional non-overlapping source. | Depends on task 5 and the systematics workflow. | **Release gate for claiming model coverage; parallel with task 8 after task 3.** |
| 10 | Package immutable validation tables/JSON, update the analysis note, and conduct final reviewer sign-off. | Depends on all relevant release gates. | **Final release gate.** |

The governing decision is intentionally asymmetric: repair and validate the nominal IBU chain first. Native TUnfold promotion is optional and should proceed only if its tests reveal an actually promotable solution, not merely because an implementation exists.

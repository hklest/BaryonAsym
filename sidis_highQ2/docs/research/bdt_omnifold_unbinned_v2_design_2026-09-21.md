# BDT OmniFold design for `unbinned_v2` (2026-09-21)

## Decision

**Recommendation — CONDITIONAL ADOPTION AS A DIAGNOSTIC-ONLY BDT cross-check; do not adopt the upstream class directly and do not promote it to the nominal result.**  Retain the clean-state streaming implementation as the operational starting point, but replace its neural classifier with a purpose-built, streaming BDT backend only after the gates below pass.  The upstream `RooUnfoldOmnifold` package and the v1 paper are useful algorithmic references, but their unbinned interface cannot directly express this analysis's weighted, separate-reco/separate-gen measure, reco-only fake, event-grouped five-fold out-of-fold (OOF) contract.

This recommendation is **BLOCKED** from physics interpretation or nominal-method consideration.  It does not change the current v6/z005 release status: IBU/BBB is the central finalcandidate while the IBU/BBB-versus-native-v6-TUnfold decision remains open, and unbinned/OmniFold is diagnostic-only ([`STATE.md:14,24-30`](../../STATE.md); [`context-handoff.md:203-211`](../../context-handoff.md)).

## Scope and authority

* **Verified:** This design is confined to high-`Q²` inclusive charged-hadron SIDIS.  The current contract is 245 truth bins per charge and separate charge-major handling; it must not be merged with other BaryonAsym analyses ([`context-handoff.md:39-54`](../../context-handoff.md)).
* **Verified:** The requested unbinned-v2 feature vector is exactly
  `logQ2, logx, z, pth, sin_phi_h, cos_phi_h`; `y` is deliberately excluded ([`scripts/unbinned_v2/streaming_omnifold.py:12`](../../scripts/unbinned_v2/streaming_omnifold.py), [`config/streaming_campaign_manifest.json:12-20`](../../scripts/unbinned_v2/config/streaming_campaign_manifest.json)).  Preserve all six features identically in reco and gen measures.
* **Verified:** The prepared campaign is explicitly labelled “diagnostic only,” uses independent DJANGOH and RAPGAP responses, 658 DATA / 501 DJANGOH / 364 RAPGAP shards, and forbids a monolithic merge/full copy ([`streaming_campaign_manifest.json:2-29`](../../scripts/unbinned_v2/config/streaming_campaign_manifest.json)).
* **Verified:** `unbinned_v2` passed an **input-shard validation**, not an unfolding/closure validation.  Its closure status is `BLOCKED_UNVALIDATED` ([`streaming_campaign_manifest.json:31-38`](../../scripts/unbinned_v2/config/streaming_campaign_manifest.json)).

## Paper and upstream package: what is usable

### Algorithmic content to retain

**Verified — paper Secs. 2.2/2.2.1.** OmniFold alternates two likelihood-ratio fits: Step 1 estimates the data/reco-MC density ratio `ω(x)` in detector variables; Step 2 estimates the reweighted-gen/prior-gen ratio `ν(z)` in truth variables.  Paired MC is the bridge from the Step-1 reco weight to truth.  This is the correct conceptual basis for a six-dimensional MultiFold application.  [Milton *et al.*, arXiv:2503.09720v1, Eqs. (5–6), Sec. 2.2](https://arxiv.org/html/2503.09720v1#S2.SS2).

**Verified — paper Sec. 2.2.1.** The BDT implementation uses `sklearn.ensemble.GradientBoostingClassifier`; it is presented as suitable for relatively low-dimensional tabular features, with no required feature standardisation.  This supports a BDT trial for six high-level observables, but is not evidence that it is validated for this H1 response.  [Paper Sec. 2.2.1](https://arxiv.org/html/2503.09720v1#S2.SS2.SSS1).

**Verified — paper Sec. 3.** The jet illustration uses six simultaneous high-level observables, five OmniFold iterations, and states explicitly that settings were not extensively optimized and must be optimized per application.  Therefore **do not inherit `n=5` as an H1 default**; current `unbinned_v2`’s two-iteration/one-epoch scan is a deliberately conservative diagnostic start, not a physics choice ([paper Sec. 3.2](https://arxiv.org/html/2503.09720v1#S3.SS2); [`streaming_campaign_manifest.json:25-29`](../../scripts/unbinned_v2/config/streaming_campaign_manifest.json)).

**Verified — Appendix B and code.** The examples are ordinary in-memory Python examples, not a distributed OOF workflow.  The upstream README documents ROOT >=6.10, scikit-learn, NumPy, a CMake-built RooUnfold fork, and use via C++/PyROOT ([upstream README](https://github.com/rymilton/unbinned_unfolding)).  No upstream example establishes safety for the H1 streaming/memmap route or event-grouped cross-fitting.

### Exact BDT default specification

For an **upstream-parity baseline only**, use separately instantiated scikit-learn `GradientBoostingClassifier` objects for every `(charge, iteration, fold, step)`:

```text
loss='log_loss' (paper calls this cross-entropy)
learning_rate=0.1
n_estimators=100
subsample=1.0
criterion='friedman_mse'
min_samples_split=2
min_samples_leaf=1
max_depth=3
min_impurity_decrease=0.0
max_features=None
random_state=<recorded deterministic seed>
max_leaf_nodes=None
warm_start=False
validation_fraction=0.1 (irrelevant unless early stopping is enabled)
n_iter_no_change=None  # early stopping OFF
```

**Verified:** Sec. 2.2.1 gives max depth 3, 100 boosting stages, learning rate 0.1, cross-entropy, and says early stopping was not used for Sec. 3/Appendix-A results.  [Paper Sec. 2.2.1](https://arxiv.org/html/2503.09720v1#S2.SS2.SSS1).  Remaining entries are the corresponding scikit-learn `GradientBoostingClassifier` defaults and must be version-pinned/recorded rather than silently assumed.  **Inferred design choice:** Fix `random_state`; cross-fit makes this mandatory for reproducibility.

For truth-only miss propagation, the paper/upstream uses `GradientBoostingRegressor` with the **same tree defaults** (100 estimators, depth 3, learning rate 0.1, squared-error default loss) to learn the conditional mean Step-1 weight from gen features for reco-failing truth events.  [Paper Sec. 2.2.1](https://arxiv.org/html/2503.09720v1#S2.SS2.SSS1); [`python/omnifold.py`, `use_regressor` branch](https://raw.githubusercontent.com/rymilton/unbinned_unfolding/main/python/omnifold.py).  This must be a separately labelled **miss-model variant**, not an unnoticed substitution for the current Step-2 treatment.

## Direct-compatibility assessment

| Required H1 property | Upstream direct support | finding / adaptation |
|---|---|---|
| Six tabular features | **Yes** | Arrays are arbitrary-dimensional tabular input; retain fixed ordering and transformations. |
| h+/h− separate | **No automatic separation** | Caller must run independent charge jobs; never add charge as a seventh feature because that lets class balance/model leakage couple the physical samples. |
| Weighted data, reco MC, gen MC | **Partial** | Upstream accepts `MCgen_weights`, `MCreco_weights`, `measured_weights`, but its unbinned algorithm assumes aligned MC rows.  Weight support is not the required distinct-measure semantics by itself. |
| Matched events | **Yes, aligned only** | It can associate one reco and one gen row by shared MC-array index. |
| Reco-only fakes | **No** | It first filters both MC arrays by `MC_pass_truth_mask`, so a row failing truth selection is discarded before Step 1.  Paper explicitly says detector-only background / det-pass truth-fail accounting is not implemented unbinned. |
| Truth-only misses | **Partial** | It learns a BDT regressor on matched events and assigns `E[ω(x)|z]` to reco-failing truth rows.  It cannot prove that this is suitable for our weighted/multi-source miss population. |
| Independent reco/gen measures | **No** | Its `weights_pull/push` vectors share one index-aligned MC population.  Current H1 code keeps separate `weight_reco` and `weight_gen` memmap measures. |
| Five-fold event-grouped OOF | **No** | No cross-fitting API/split manager; needs an external orchestrator. |
| Streaming/full-shard safety | **No** | Upstream transfers RDataFrames to Python / builds in-memory NumPy arrays; no shard iterator or out-of-core state. |

**Bottom line (verified from code): upstream is not direct-drop-in.** `RooUnfoldOmnifold::UnbinnedOmnifold` accepts three RDataFrames plus pass masks and optional three weight vectors, but invokes a Python routine after materialising the data ([`src/RooUnfoldOmnifold.cxx`, `UnbinnedOmnifold`](https://raw.githubusercontent.com/rymilton/unbinned_unfolding/main/src/RooUnfoldOmnifold.cxx)).  In `omnifold.py`, all MC arrays are sliced by `MC_pass_truth_mask`; Step 1 uses only `MC_pass_reco_mask`, and fakes outside truth acceptance have already been removed ([upstream `python/omnifold.py`](https://raw.githubusercontent.com/rymilton/unbinned_unfolding/main/python/omnifold.py)).

## Required H1 semantics and proposed BDT implementation contract

### Independent measures and exact routing

Maintain two mutable, base-weighted measures indexed by the source record, never one shared event weight:

* `μ_R^0`: reco measure = `weight_reco` on `has_reco` rows;
* `μ_G^0`: gen measure = `weight_gen` on `has_gen` rows.

Per charge `c ∈ {+1,−1}` and OOF fold `f`, define `M = has_reco ∧ has_gen`, `F = has_reco ∧ ¬has_gen`, and `U = has_gen ∧ ¬has_reco`.

1. **Step 1 (reco):** fit `D_reco(c,f)` (class 1) versus `MC_reco(c,f)=M∪F` (class 0), weighted by their *current reco measures*.  Update `μ_R` for both `M` and `F`.  This is the only place fakes enter.
2. **Bridge:** copy the Step-1 updated **matched** reco measure to the matched truth partner only.  Do **not** copy fake weights to gen; do **not** overwrite `μ_G(U)`.
3. **Miss treatment:** for each `U`, construct `\hat ω_U(z)=E[ω(x)\mid z]` with a separately fitted Step-1-weight regressor trained only on `M` using gen features and current Step-1 ratios.  The Step-2 target truth measure is `μ_G × \hatω` on `U` and matched-pushed reco measure on `M`.
4. **Step 2 (gen):** fit original/current `μ_G` (class 0) against target/pushed `μ'_G` (class 1), both in the gen feature space; multiply only OOF gen records by the resulting `ν(z)`.

This differs from the currently prepared PyTorch route, which intentionally leaves misses unchanged by the Step-1 push and lets Step 2 update them ([`streaming_omnifold.py:89-101`](../../scripts/unbinned_v2/streaming_omnifold.py)).  That current behavior is internally coherent as a diagnostic variant, but **not paper-parity miss handling**.  Retain both variants and compare them; do not silently relabel one as the other.

### Five-fold cross-fitting

**Required adaptation:** Hash the immutable `(run,event)` pair once, assign each event (and therefore every associated track/record) to exactly one of five folds, and rotate:

```text
holdout f      = inference/test fold
validation f+1 = early-stopping/model-selection fold (unused for exact-default baseline)
training       = remaining three folds
```

The existing clean-state function already implements this 3/1/1 rotation and OOF test inference ([`streaming_omnifold.py:15-17, 85-98`](../../scripts/unbinned_v2/streaming_omnifold.py)).  It should become the fixed splitter for BDT adoption.  Do not use the stale TensorFlow adapter’s `grouped_train_mask`: it returns only hash fold 0 and is not its claimed rotating 3/1/1 contract ([`weighted_omnifold_adapter.py:14-20`](../../scripts/unbinned_v2/weighted_omnifold_adapter.py)).

**Critical inference:** Cross-fitting must be executed at every iteration and every Step 1/Step 2 fit.  A model may score only its OOF event group.  Mixing tracks from one event between training and test is forbidden because event-level kinematics and weights leak.

### Class-prior/density-ratio correction

For classifier output `s(x)=P_train(Y=1|x)`, the physical ratio is

```text
r_1/0(x) = [s(x)/(1-s(x))] × [W_0/W_1],
W_k = sum of effective sample weights of class k supplied to that fit.
```

This follows Bayes’ theorem for the weighted training distributions.  It is mandatory whenever class totals are not equal.  The upstream `reweight()` returns only `s/(1-s)` ([upstream `python/omnifold.py`](https://raw.githubusercontent.com/rymilton/unbinned_unfolding/main/python/omnifold.py)); it therefore relies on implicit/equal class priors rather than exposing this correction.

**Verified:** the current streaming implementation normalizes each class to total weight 0.5 before BCE, so `W0/W1=1` by construction and `exp(logit)=s/(1-s)` is correct ([`streaming_omnifold.py:55-67`](../../scripts/unbinned_v2/streaming_omnifold.py)).  **Required BDT adaptation:** either retain exactly that per-fit half/half normalization, or record raw `W0,W1` and multiply by `W0/W1`.  Never both.  Log raw totals, normalized totals, correction factor, fraction clipped, and effective sample size `(Σw)^2/Σw²` per `(charge, fold, iteration, step)`.

## Resource design and safe parallelism

**Verified:** full-shard streaming currently avoids `merged.npz`, makes float64 measure memmaps per shard, streams batches, and reports peak RSS ([`streaming_omnifold.py:18-19, 38-44, 76-83, 102`](../../scripts/unbinned_v2/streaming_omnifold.py)).  This is the right storage contract.

**Inferred resource consequence:** stock `GradientBoostingClassifier.fit` requires the selected training arrays in RAM; it is not a streaming/partial-fit estimator.  Thus adopting upstream BDTs means a gather per `(response, charge, iteration, fold, step)` even if input reading remains streamed.  The peak allocation is at least feature matrix + labels + sample weights + tree working memory.  With float32 features and 6 columns this is roughly `24 N` bytes before labels/weights/copies; realistic scikit-learn copies and tree state make a budget of **>=100 N bytes** prudent.  Memmap state does not make the BDT fit out-of-core.

**Safe plan:**

1. Perform a read-only inventory estimate of selected rows and `N_eff` first; route that validation to `h1-data-validator`/`h1-root-analyst`.
2. Use one process/thread per model (`OMP_NUM_THREADS=1`, `MKL_NUM_THREADS=1`, `OPENBLAS_NUM_THREADS=1`); classic sklearn GradientBoosting itself has no useful `n_jobs` training knob.
3. Parallelize **only independent response × charge jobs** at first (maximum four simultaneous jobs), not folds within a charge and not iterations within a response.  Each job needs isolated state/output roots.
4. Folds are sequential inside a charge because the next iteration requires the full OOF-composed measure.  Step 2 follows Step 1; iterations follow iterations.
5. Request memory only after a one-fold production-like measurement establishes peak RSS.  Do not claim the existing `3 GB/8 CPU` request is adequate merely because the PyTorch streaming job uses it ([`submit_streaming_production2_20260921.sub:1-12`](../../scripts/unbinned_v2/condor/submit_streaming_production2_20260921.sub)).  Route any Condor deployment to `h1-production`/`h1-naf-operator`.

## Validation gates (all required before any interpretation)

1. **Schema/selection gate — BLOCKED:** independently audit six reco/gen feature definitions, finite ranges, event IDs, charge consistency, and fixed fold assignment.  Require no event overlap among 3/1/1 sets and no paired record crossing folds.
2. **Measure-conservation gate — BLOCKED:** for every fit, document `M/F/U` raw counts, `Σw`, `Σw²`, `N_eff`; prove fakes appear only in Step 1, misses only in the miss prescription/Step 2, and matched propagation is once-only.  Require non-negative finite weights and no unaccounted selection flow.
3. **Prior-ratio gate — BLOCKED:** unit-test deliberately unequal class totals; verify exact `W0/W1` recovery, and verify equivalence to `s/(1-s)` under half/half normalization.  Audit ratio clipping as a declared regularisation variation, never hidden.
4. **Paper-parity miss gate — BLOCKED:** on a controlled sample, compare H1 regressor-miss `E[ω|z]` against upstream BDT output using equal measures/no fakes.  Then quantify the difference from current “miss unchanged until Step 2” routing.  No preference without Poisson closure.
5. **Fake/background gate — BLOCKED:** the paper limitation applies directly: no unbinned detector-only background/fake implementation exists upstream.  First close the high-Q² alpha-veto/background response-consistency blocker and define a single covariance-carrying fake treatment; never pre-subtract and retain fakes in Step 1 simultaneously ([`context-handoff.md:229-235`](../../context-handoff.md)).
6. **Statistical closure gate — BLOCKED:** ≥100 statistically independent, event-Poisson pseudo-experiments, with event multipliers shared across all tracks in an event; split response/pseudo-data statistically where feasible.  Report per-bin bias, variance, coverage, pulls, correlations, refold residuals, and tail failures for both charges and both response/pseudodata generator directions.  The current `streaming_closure.py` is explicitly an identity streamed-measure check, “not a replacement for a full unfolded covariance calculation” ([`streaming_closure.py:1-8,35-37`](../../scripts/unbinned_v2/streaming_closure.py)); it cannot pass this gate.
7. **Regularisation gate — BLOCKED:** scan BDT capacity (`n_estimators`, depth, leaf size), iterations, clipping, and miss model.  Select before looking at data comparison; demonstrate stability versus H1 binned IBU `k=4`, not agreement of an integral alone.
8. **Systematics/covariance gate — BLOCKED:** generate data-stat and MC-response toys with weighted input variance; evaluate DJANGOH/RAPGAP swap, detector shifts, fakes/background scale/shape, miss prescription, BDT hyperparameters and seeds.  Method spread is a diagnostic, not a quadrature systematic ([unfolding skill, “Traps”]).
9. **Status gate — BLOCKED:** keep all output below `output_v6/studies/`, retain `diagnostic_only=true`, and require explicit supervisor method decision after the existing `C_rad`, response-generator coherence, and background gates close.

## Specialist handoff

* **Implementation only after approval:** `h1-builder-engineer` / `h1-script-runner` should implement the external BDT backend, correct cross-fit splitter use, and provenance capture; they must not replace central code.
* **Input and flow audit:** `h1-data-validator` or `h1-root-analyst`.
* **Fake/background semantics:** `sidis-hq2-backgrounds` / `h1-systematics`.
* **Generator provenance and response meaning:** `h1-mc-db` then `h1-mc-expert`.
* **Closure/covariance/method decision:** `h1-unfolder`, `h1-systematics`, and the SIDIS supervisor.
* **Operations:** `h1-production` or `h1-naf-operator`; no execution is requested by this report.

## Sources

1. R. Milton *et al.*, “Tools for Unbinned Unfolding,” arXiv:2503.09720v1 (2025): [Sec. 2.2.1](https://arxiv.org/html/2503.09720v1#S2.SS2.SSS1), [Sec. 3](https://arxiv.org/html/2503.09720v1#S3), [Appendix B](https://arxiv.org/html/2503.09720v1#A2).
2. Upstream source, commit tree queried 2026-09-21: [`RooUnfoldOmnifold.cxx`](https://raw.githubusercontent.com/rymilton/unbinned_unfolding/main/src/RooUnfoldOmnifold.cxx), [`python/omnifold.py`](https://raw.githubusercontent.com/rymilton/unbinned_unfolding/main/python/omnifold.py), [README](https://github.com/rymilton/unbinned_unfolding).
3. Local current authority: [`STATE.md`](../../STATE.md), [`context-handoff.md`](../../context-handoff.md), [`docs/RUN_STATUS.md`](../RUN_STATUS.md).
4. Local implementation reviewed: [`streaming_omnifold.py`](../../scripts/unbinned_v2/streaming_omnifold.py), [`weighted_omnifold_adapter.py`](../../scripts/unbinned_v2/weighted_omnifold_adapter.py), [`streaming_closure.py`](../../scripts/unbinned_v2/streaming_closure.py), and [`streaming_campaign_manifest.json`](../../scripts/unbinned_v2/config/streaming_campaign_manifest.json).

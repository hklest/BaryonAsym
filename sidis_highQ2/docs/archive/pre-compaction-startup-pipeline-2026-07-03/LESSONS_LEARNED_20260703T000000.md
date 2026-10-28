# Lessons learned — H1 high-Q² charged-particle SIDIS

**Purpose:** Single distilled file capturing structural insights from the rev3 → rev4 → rev5 → rev6 unfolding iteration. Read this before touching the response matrix, IBU, TUnfold, or any other downstream consumer. Pointers below cite source documents for full detail.

**Status: living document.** Update when a new structural insight or cautionary trap is found.

> **Methodology correction (2026-06-01):** Do **NOT** treat integrated unfolded totals or integrated cross sections as the primary evidence that an unfolding method is healthy. Integrated totals can be distorted by a small number of pathological bins and should be used only as a coarse sanity check. Required diagnostics are **L-curve, closure tests, rho/correlation matrix, refold residuals, negative-bin pattern, effective rank/eigenvalue spectrum, and per-bin pathology diagnostics.** For **TUnfold specifically**, assess "working / not working" only after looking at the **L-curve, closure, and rho/correlation plots together**. This **supersedes earlier wording** in these notes where integrated totals or "integrated stability" were treated too strongly as evidence.

---

## Cardinal rule

**Every per-track weight applied to the matched-reco fill must be matched in the gen prior fill.** Failure to do this produces an `ε[j] = col_inner / prior` that is silently biased by `<weight_asymmetry>`, propagating through every downstream method (IBU, TUnfold, BBB). The asymmetry is hard to detect at the integrated level (typically ~1%) but breaks normalization-anchored unfolding completely (rev3→rev4 TUnfold cascade).

See: [docs/critiques/rev6_audit_2026-05-31/SYNTHESIS.md](critiques/rev6_audit_2026-05-31/SYNTHESIS.md) CC1, [docs/results/nucliaweight_fix_2026-05-31.md](results/nucliaweight_fix_2026-05-31.md).

---

## The NucliaWeight asymmetry (CC1) — three rounds of critics, finally fixed 2026-05-31

**The trap:** `build_migration_th2_per_charge.py` applied NucliaWeight to `w_reco = w1·w2·NW` (matched-reco fill) but not to `w_gen = w1·w2` (gen prior fill). NucliaWeight is a per-track DOWN-weight (mean 0.99, max 1.00, occasional negative tail clipped to 0). The asymmetry causes:

| Method | Direction of bias |
|---|---|
| IBU 1/ε correction | Unfolded result inflated by ~1% per bin |
| TUnfold kEConstraintArea (rev4) | Area target inflated by `1/<NW>² ≈ 1/0.97 ≈ 3%` → integrated 5.06e6 instead of 2.7e6 |
| BBB diagonal | Same ~1% bias as IBU |

**Why it took 3 rounds to fix:**
- Rev3 critic-1 §F-A: flagged as "Option B, deferred to rev5". Not fixed.
- Rev5 critic-2 IB3: flagged again as "deferred from rev5 but never fixed". Not fixed.
- Rev6 audit synthesis CC1: flagged a third time. **Fixed 2026-05-31** via `prior_gen_NWeff` (Option F: NW-weighted prior using per-matched-event NW lookup).

**The lesson:** A "deferred" critic finding is not addressed unless verified by code change AND empirical re-test. The NW asymmetry was on the to-do list for 3 sessions and slipped each time because the bias was "only ~1%". For TUnfold with area constraint, "only 1%" became "5×" because of the squared denominator in the F1 formula.

**Empirical magnitude (h+ Rapgap, full production sample):**
- `prior_NWeff / prior_unweighted = 0.9947`
- ε per-bin median shift: +0.3%
- Integrated unfolded shift (expected): +1% on headline

**Where the fix lives:**
- `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_NWfix.py` (1039 lines, +57 vs original)
- `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_NWfix.py` (+30 lines)
- Original files preserved unmodified for audit reference.

---

## The rev3 → rev4 → rev5 → rev6 TUnfold cascade

A textbook example of fixing the symptom instead of the cause.

| Rev | Choice | What broke | Real cause |
|---|---|---|---|
| rev3 | F1 area target = `data/prior_total × miss_MC` | Under-counted overflow by `reco_eff ~ 0.55`, biased integrated by −45% | Wrong denominator (`prior` instead of `matched`) |
| rev4 | F1 area target = `data/matched_total × miss_MC` | Integrated 5.06e6 = `data/reco_eff²` instead of `data/reco_eff` | NucliaWeight asymmetry: matched has NW, miss-row fill has no NW |
| rev5 | Drop area constraint entirely (`kEConstraintNone`), set overflow = 0 | Integrated ~3.19e6 by accident agrees with `data/reco_eff` | Workaround for rev4 symptom; root cause (NW asymmetry) untouched |
| rev6 | Add explicit fake subtraction (`_fake_subtract.py`), keep `kEConstraintNone` | Headline numerically OK but no physical normalization anchor | Same root cause, plus an architectural shift to per-method handling |

**The lesson:** When TWO consecutive bug fixes (rev3 → rev4) don't fix the problem, the root cause is probably upstream of the patched code. The NW asymmetry in `build_migration_th2_per_charge.py` was upstream of all the F1 algebra — three rounds of fixing F1 couldn't fix it.

**Detection pattern:** Whenever the headline result depends sensitively on the algebra of a normalization formula (F1, area constraint, miss-yield injection), suspect a weight asymmetry between fill paths in the upstream response matrix.

See: [docs/research/research-F1-area-constraint-normalization-2026-05-31.md](research/research-F1-area-constraint-normalization-2026-05-31.md), [docs/critiques/rev5_iteration_2026-05-31/](critiques/rev5_iteration_2026-05-31/).

---

## IBU rev5 → rev6 — the absolute-weight-response bug

**The trap:** Stock `ibu_core.py` (rev5) was D'Agostini's formula assuming `R[i,j]` is a CONDITIONAL probability matrix `P(reco|gen)`. Our `R` is in ABSOLUTE WEIGHT counts (`R[i,j] = sum of MC weights for events with reco=i, gen=j`). With absolute-weight R, the rev5 IBU step converges to `matched_per_gen[j] = R[:,j].sum()` **instead of** to the truth prior. Symptom: self-closure gave median |rel| = 0.66 across 240 bins (should be ~0).

**The fix (rev6):** `ibu_core_rev6.py` factors R correctly:
- `P[i,j] = R[i,j] / R[:,j].sum()` (column-normalized: true `P(reco|gen)`)
- `ε[j] = R[:,j].sum() / prior[j]` (per-gen acceptance)
- Iteration: `denom = P @ (ε·p); update = (P.T @ data/denom) · ε · p; p_next = update/ε`

**Verification:** Self-test on synthetic data gives max |rel| = 3e-16 (machine precision). Self-closure on real data: med |rel| = 0.000 trusted-median.

**The lesson:** When borrowing a formula from a textbook (D'Agostini 1995, Schmitt 2016), CHECK that your response matrix is in the same units the formula assumes. Conditional probability matrix vs absolute count matrix differ by a column-normalization factor that's easy to overlook.

See: [docs/critiques/rev6_audit_2026-05-31/critic-ibu-1-implementation.md](critiques/rev6_audit_2026-05-31/critic-ibu-1-implementation.md), [docs/results/rev6_fake_subtraction_results_2026-05-31.md](results/rev6_fake_subtraction_results_2026-05-31.md).

---

## Matrix-BBB ≠ novel; ≈ D'Agostini at n=0 iterations

**What it looks like:** A clever closed-form that uses per-fine-reco-bin response info instead of per-cell aggregation. Distributes data across gen bins via `f_rj = R[r,j] / Σ_k R[r,k]`, multiplies by `C[j] = prior/col`.

**What it actually is:** **D'Agostini's IBU at n=0 iterations starting from a flat prior** (or equivalently Shepp-Vardi 1982 EM at zero iterations). Schmitt 2016 (arXiv:1611.01927) §4.5 Eq. 15 IS this formula. Schmitt explicitly tests it (Table 1, EM n=0) and **categorically recommends against using it as an unfolding method**: "the results obtained for n=0 iterations... are very poor... all bins have positive correlations, corresponding to a smearing rather than unfolding".

**Why self-closure is perfect (by construction, not by validation):** In self-closure the response IS the model that generated the pseudo-data, so the n=0 step trivially reproduces the input. This is a closure FEATURE, not a closure VALIDATION.

**Useful as:** Migration-severity diagnostic (purity-within-parent-cell metric).
**NOT useful as:** Headline unfolding method.

**The lesson:** When you invent a "new" method, search arXiv before publishing the name. The unfolding literature is mature; most clean closed forms have been tried and characterized.

See: [docs/results/rev6_fake_subtraction_results_2026-05-31.md](results/rev6_fake_subtraction_results_2026-05-31.md).

---

## σ_reg depends on definition AND scan spacing

Quoting σ_reg without specifying both the **definition** (narrow neighbor / stability window / rms-over-full-scan) and the **scan grid spacing** is meaningless. Equal numbers from different definitions can hide a 60× actual gap. Equal definitions on different scan grids can fake a 60× gap.

| Definition | h+ IBU N=50 | h+ TUnfold τ=1.4e-6 |
|---|---|---|
| Narrow neighbor (step in own scan grid) | 0.35% | 0.11% |
| Stability window (factor 2 in N / decade in τ) | 0.37% | 0.32% |
| RMS over full scan range | 6.94% | 42.8% |

**Headline σ_reg should be quoted with definition AND grid stated.** Recommendation: narrow neighbor on a documented log-spaced grid. Quote the stable plateau width as a separate robustness metric.

See: [docs/results/reg_strength_tuning_2026-05-31.md](results/reg_strength_tuning_2026-05-31.md).

---

## TUnfold has a wider stable plateau than IBU; canonical operating points matter

**TUnfold:** stable plateau (σ_reg < 1%, integrated stable < 2%) spans 9 decades in τ for h+ and 4 decades for h-.
**IBU:** stable plateau is just 1 factor of ~2 in N for h+ and a single point (N=130) for h-.

**Implication:** TUnfold canonical τ choice is forgiving; small mis-choice does not matter. IBU was studied as a diagnostic; its iteration-count behavior helped expose regularization sensitivity but it is not central.

**Historical note:** This lesson predates the z005 IBU/BBB finalcandidate and native v6 TUnfold work. Current z005 central status is IBU/BBB finalcandidate; native ROOT/TUnfold v6 miss-underflow is the standard TUnfold diagnostic/candidate-comparison implementation but not promoted.

See: [docs/results/reg_strength_tuning_2026-05-31.md](results/reg_strength_tuning_2026-05-31.md).

---

## Native ROOT/TUnfold miss handling for z005 (2026-06-21)

**The trap:** v4 encoded misses and fakes as ordinary TUnfold bins: a measured/input miss row with zero data and a truth/output fake column. ROOT/TUnfold documentation and z005 diagnostics confirm this is wrong. It made TUnfold fit non-physical bins and produced O(100) negative bins.

**Correct native ROOT convention:** for `TUnfold::kHistMapOutputVert`, put matched events in the physical response block and missed generated events in the reconstructed/input-axis underflow (or overflow) bins. For z005 per charge this means:

| Object | Placement |
|---|---|
| Matched response | ordinary `490 reco × 245 truth` physical bins |
| Missed truth events | ROOT reco/input-axis underflow bin 0 for the relevant truth bin |
| `SetInput()` data | 490 physical measured reco bins only; no miss bin |
| Reco-only fakes | background/fake subtraction, preferably `SubtractBackground()` or equivalent covariance; not a truth/output column |
| Output | `GetOutput()` is full truth-fiducial scale directly; no post-hoc `1/eps` central-value correction |

**Current implementation:** use `scripts/unfolding/tunfold_official_v6_native_miss_z005/` and outputs under `output_v6/studies/tunfold_official_v6_native_miss_z005_20260621/`. It uses LCG_109a ROOT 6.38.04 with real `TUnfoldDensity`. Geometry and `SetInput` validation pass; tau0/IBU medians are near 1 without epsilon post-scaling.

**v5 disposition:** v5 physical/fake-subtracted diagnostics removed the ordinary fake/miss bins but solved a visible/matched-scale TUnfold problem and then applied external `x_visible/eps`. This is useful audit history, not the current TUnfold entry point.

**Promotion caveat:** native v6 TUnfold remains diagnostic-only until Poisson closure, fake/background covariance, hard tau-selection gates, response/MC systematics, high refold χ², negative bins, tails, and high-ρ bins are addressed. Central z005 remains IBU/BBB finalcandidate until the nominal-method decision.

See: [docs/research/research-tunfold-native-miss-handling-z005-2026-06-21.md](research/research-tunfold-native-miss-handling-z005-2026-06-21.md).

---

## Trust mask is essential for σ summaries

Untrusted bins (5-15 of 240 per charge, mostly S3b iz≥4 corner with reco_eff < 5%) contribute long-tail outliers that distort median and p90 σ summaries by 10-100×. ALL σ headlines must be quoted "trusted-median" (NOT raw median) with the trust mask criteria explicitly stated.

**Current trust mask:** `cnt_MC < 50 OR (cnt_MC < 500 AND reco_eff < 0.05)`.

See: [docs/results/rev6_fake_subtraction_results_2026-05-31.md](results/rev6_fake_subtraction_results_2026-05-31.md).

---

## SSH ControlMaster fragility (operational lesson)

Mid-session SSH socket death happens whenever:
- A subagent issues `sleep` in the same bash call as an `ssh` invocation.
- A subagent runs `ssh -O exit/stop` or `pkill ssh`.
- A subagent opens a parallel `ssh -fNM` connection.

**Hard rule** (in all 21 H1 agents): one `ssh -O check` per dispatch; if it fails, stop and ask user to reconnect; never retry; never sleep+ssh in one bash call.

The `FOREIGN KEY constraint failed` bug in the subagent dispatcher (also persistent this session) is unrelated to SSH but has the same workaround: do tasks directly in the main agent's context when dispatch fails, do not retry.

---

## Open structural concerns (not yet addressed)

The rev6 audit synthesis flagged these as still-pending after CC1:

| ID | Issue | Status |
|---|---|---|
| TB1/TP1 | TUnfold uses `kEConstraintNone` (non-standard for inclusive multiplicity) | Open |
| TP-D1 | TUnfold τ chosen by ad-hoc gate, not L-curve scan | Open (ScanLcurve macro drafted, not run) |
| IP-M5 | IBU refold χ²/ndf ≈ 750 — verify formula labeling | Open |
| IB2 | BBB closed-form fake scale has no associated systematic | Open |
| CC2 | 0.35% charge confusion as unaccounted systematic in per-charge architecture | Open |
| IP-M4 | Q²_reco<60 spillover double-count risk between upstream bgsub and fake subtraction | Open |

See: [docs/critiques/rev6_audit_2026-05-31/SYNTHESIS.md](critiques/rev6_audit_2026-05-31/SYNTHESIS.md) for full priority list.

---

## Tau selection from per-bin closure score, not L-curve alone (2026-06-02)

**Context:** After enabling `kEConstraintArea` with the NWfix_v2 response
matrix, a 30-point log-spaced tau grid (1e-6 … 1e-2) was scanned over 4
closure modes × 2 charges = 240 Condor jobs.

**What the scan showed:**

1. **Self-closure is extremely good at low tau.** At τ = 1e-6, selfRG and
   selfDJ give p50 ≈ 1%, p90 ≈ 5%, refold χ²/ndf < 1e-4 — essentially
   machine-precision closure within one MC model.
2. **Cross-closure is significantly worse and mode-dependent.** At the same
   τ, crossDJ/crossRG give p90 ≈ 68–88%, driven by a handful of high-z,
   high-pT corner bins where the DJANGOH and RAPGAP models differ strongly.
   This is expected physics: the model-dependence is real, not a bug. The
   majority of bins close at <10% even in cross-mode.
3. **The score plateau is wide.** For h+, all tau values from 1e-6 to
   3.3e-5 pass the quality gate and change the score by < 7%. The tau
   selection is not sensitive: moving the canonical point by ±1 grid step
   changes the result negligibly.
4. **Always-bad bins are very few.** Only 2 bins per charge (0.8%) fail
   closure at ALL tau values and ALL modes (>200%). These are structurally
   unresolvable and must be masked before any publication claim.

**The lesson:** When choosing a regularization parameter for inclusive
multiplicity unfolding in a multi-dimensional 4D space, per-bin closure
quality over a full tau scan is more informative than the L-curve kink
position alone. The L-curve gives you the "safe interior" of the plateau;
per-bin closure tells you which specific bins are problematic at which tau,
and whether they are intrinsically unresolvable vs. tau-tunable.

A combined workflow of (1) per-bin closure scan to find the plateau and
identify always-bad bins, followed by (2) L-curve validation on data at the
chosen tau, is more robust than either diagnostic alone.

See: [docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md](results/tunfold_rev6_closure_tauopt_2026-06-02.md)

---

## pT-only merge policy: aggregate-vs-per-bin gate semantics (2026-06-03)

When defining a "group is acceptable" predicate for a merged pT group, the
`data_count > 0` check must be applied to the **aggregated** group sum, not
to each base bin. After background subtraction the per-bin `data_count` can
go slightly negative in low-statistics tails (purely a stochastic
subtraction artifact). A per-base-bin `data_count > 0` gate would
unnecessarily fail otherwise-healthy rows whose group sum is comfortably
positive and whose aggregated central + relative uncertainties are within
tolerance.

A second related lesson: **inherited trust masks from a previous release
can become a design constraint at merging time.** The conservative
S3-merged mask in the 2026-06-02 210-bin release zero-trusts all S3 z ≥ 0.5
bins. When the pT-merge policy was applied on top, the S3 z=4 row failed
the "at least one trusted base bin" gate even though numerical relative
uncertainties at scheme E were perfectly acceptable
(rel σ_total = 0.58 pos / 0.85 neg). This is a real design conflict
between the conservative-inherited mask and the merge policy and was
correctly flagged as a BLOCKER for user/critic decision rather than
auto-resolved.

See: [docs/results/final_binning_candidate_2026-06-03.md](results/final_binning_candidate_2026-06-03.md)

---

## File pointers for the curious

| Topic | Authoritative doc |
|---|---|
| Current state of pipeline | [docs/RUN_STATUS.md](RUN_STATUS.md) |
| Rev6 tau selection and always-bad mask | [docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md](results/tunfold_rev6_closure_tauopt_2026-06-02.md) |
| Final-binning pT-merge candidate (157 bins / charge) | [docs/results/final_binning_candidate_2026-06-03.md](results/final_binning_candidate_2026-06-03.md) |
| Rev6 fake subtraction details | [docs/results/rev6_fake_subtraction_results_2026-05-31.md](results/rev6_fake_subtraction_results_2026-05-31.md) |
| Reg strength tuning IBU vs TUnfold | [docs/results/reg_strength_tuning_2026-05-31.md](results/reg_strength_tuning_2026-05-31.md) |
| NW asymmetry fix details | [docs/results/nucliaweight_fix_2026-05-31.md](results/nucliaweight_fix_2026-05-31.md) |
| Full rev6 audit | [docs/critiques/rev6_audit_2026-05-31/SYNTHESIS.md](critiques/rev6_audit_2026-05-31/SYNTHESIS.md) |
| Schmitt 2016 unfolding methods reference | arXiv:1611.01927 (the foundational reference for matrix-BBB ≠ novel) |
| F1 area-constraint algebra derivation | [docs/research/research-F1-area-constraint-normalization-2026-05-31.md](research/research-F1-area-constraint-normalization-2026-05-31.md) |

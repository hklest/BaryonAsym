# Critique: v6 Stage1+Stage2 OR-Envelope Migration Coverage and Gen-Cut Bias

**Date:** 2026-06-05
**Critic:** h1-analysis-critic (read-only physics-correctness review)
**Mode:** Adversarial review. No file edits, no production, no remote runs.
**Scope:** High-Q² charged-SIDIS v6 OR-envelope Stage1 keep-list logic, Stage2
gen-fill behavior, and the v6 response-builder gen leg, evaluated against the
single smoke MC file
`sidistree_v6_djangoh14_7072_em06_0007_smoke.root`
(djangoh14, run 7072, e⁻ pol −, file 0007) and its migration QA JSON at
`H1/sidis_highQ2/plots/v6_migration_smoke_2026-06-05/v6_migration_smoke_sidistree_v6_djangoh14_7072_em06_0007_smoke.json`.

**Question put to the critic:** Does the latest v6 implementation capture all
four migration directions (gen∈/gen∉ × reco∈/reco∉), and does it avoid biasing
unfolding via residual gen-level cuts, given that the final kinematic
selection is imposed only at the TUnfold/truth-selection stage?

---

## 1. Verdict

**Overall verdict: CONCERN (not FATAL, not SOUND).** The v6 OR-envelope is a
strict superset of the prior production keep-list and demonstrably opens the
gen-only migration channel that was previously closed (50,712 gen-only events
in this one smoke file). For the three observable migration quadrants
(gen∈/reco∈, gen∈/reco∉, gen∉/reco∈) the implementation behaves as designed in
the smoke and is consistent across the Stage1 keep-counts, the Stage2 tree
populations, and the migration-QA recount. However, three residual gen-side
selections survive in the OR-envelope and three more survive in the v6
response-builder; each of these can still bias the unfolded result if the
**published fiducial** is wider than the envelope on any dimension. These are
not theoretical: at least one (`Q2_gen > 60` vs published `Q2 > 150`) is
trivially safe, but two (the broad y window and the `zGen ∈ [0,1]` numerical
filter as currently scoped) require an explicit decision and a documented
written justification before full production. The migration-QA smoke is also a
single file using an any-track event-level approximation, which is sufficient
for category-existence but insufficient as the final closure evidence.

Per-claim verdicts are given inline below using SOUND / CONCERN / FATAL /
INSUFFICIENT EVIDENCE.

---

## 2. Implementation Summary as Reviewed

The implementation under review (as stated in the user prompt and corroborated
by `docs/methodology/v6_production_changes_2026-06-04.md`,
`docs/methodology/migration_qa_script_guide_2026-06-05.md`, and the QA JSON):

- **Stage 1 MC keep-list:** `pass_reco_envelope OR pass_gen_envelope`.
- **Stage 1 data keep-list:** `pass_reco_envelope` only (no gen leg in data).
- **Reco envelope (loose):** `Q2_reco > 100`, `0.05 < y_reco < 0.9`,
  `30 < Empz_reco < 75`, plus the standard quality/trigger/electron/background
  gates. This is intentionally wider than the published reco fiducial
  (`Q² > 150`, `0.1 < y < 0.7`, `35 < Empz < 70`) to provide migration
  headroom on the reco side.
- **Gen envelope (loose, post-patch):** `Q2_gen > 60`, `0.01 < y_gen < 0.99`,
  `25 < GenEmpz < 75`. No `xF` cut. No gen-hadron requirement. No `GenPt` or
  `GenEta` cut.
- **Stage 2 behavior:** For all MC events kept by the Stage 1 OR, fill
  `ChargedTotGen`. For events that fail `pass_reco_envelope`, skip the reco
  fill so `ChargedTot` contains only reco-passing events.
- **v6 response-builder gen leg:** No active gen Q², gen y, gen Empz cut for
  the migration build; no `xF`, `GenPt`, or `GenEta`. Only finite-`zGen`,
  species pool `|PDG| ∈ {211, 321, 2212}`, prompt/GTR via `IsV0DaughterGen==0`
  V0-daughter veto, and broad sanity. Final fiducial is imposed at
  TUnfold / truth-selection downstream.
- **Smoke counts (this file):**
  - Stage 1 kept total = 76,140 (`reco_only=45 + gen_only=50,712 + both=25,383`).
  - Stage 2 `ChargedTot`: 252,201 track entries across 25,048 unique events.
  - Stage 2 `ChargedTotGen`: 1,720,321 track entries across 76,140 unique events.
  - Data smoke: `ChargedTot = 16,889`, no `ChargedTotGen`, no FPACK abort.

Note: the Stage 1 totals reported by the user (`45 + 50,712 + 25,383 = 76,140`)
and the Stage 2 unique-event totals (`ChargedTot = 25,048` ≈ `45 + 25,383 =
25,428` and `ChargedTotGen = 76,140` = full kept set) are consistent within
the ±400 discrepancy expected from event-level any-track classification vs
strict per-track inlined matching used by Stage 1. This is flagged below.

---

## 3. Per-Quadrant Migration-Coverage Evaluation

The four migration quadrants, evaluated event-level (RunNumber, EventNumber)
against the **published signal fiducial without z** (`Q² > 150`,
`0.1 < y < 0.7`, `35 < Empz < 70`, `xF < 0`). Counts are from
`categories.signal_fiducial_no_z` in the QA JSON unless stated otherwise.

### 3.1 gen∈ → reco∈ (both) — SOUND for existence; INSUFFICIENT EVIDENCE for sufficiency

- **Smoke count:** 17,578 events.
- **Interpretation:** Core measurement population. Largest of the three
  observable categories under the published fiducial, as expected. Stage 1
  keep-list and Stage 2 fill both write these events. Both `ChargedTot` and
  `ChargedTotGen` contain them.
- **Risk:** None at the envelope-coverage level. Statistical adequacy of
  17.6k events from one smoke file is irrelevant to coverage; for full
  production, the sample must be the full em06+ep05+ep06+ep07 stack across
  all DIS MC, which is established practice. **No new risk introduced by v6.**

### 3.2 gen∈ → reco∉ (gen_only under signal fiducial) — SOUND for existence; CONCERN for completeness above the gen envelope

- **Smoke count:** 7,262 signal-fiducial gen-only events (and 51,092 gen-only
  with no fiducial applied at all).
- **Interpretation:** Detector-inefficiency and resolution-loss column of the
  response. Previously suppressed by the rev6 / NWfix_v2 reco-only keep-list,
  which discarded the gen tracks of events that failed the reco envelope. v6
  now retains them because the Stage1 OR matches on `pass_gen_envelope` and
  Stage2 unconditionally fills `ChargedTotGen` for kept events. This is the
  category whose absence was the dominant pre-v6 unfolding-bias risk.
- **VERDICT:** **The category exists and is populated** — verdict
  **SOUND** for existence.
- **Residual completeness concern:** events with `Q2_gen < 60` or
  `y_gen ∉ (0.01, 0.99)` or `GenEmpz ∉ (25, 75)` that nonetheless lie inside
  the published gen fiducial (`Q² > 150`, `0.1 < y < 0.7`, `35 < Empz < 70`)
  are **logically impossible** by inclusion for `Q2_gen > 60` ⊃ `Q² > 150`,
  for `0.01 < y < 0.99` ⊃ `0.1 < y < 0.7`, and for `25 < GenEmpz < 75` ⊃
  `35 < GenEmpz < 70`. **So as long as the published fiducial stays inside
  the v6 envelope on every dimension, no gen∈/reco∉ event is dropped by the
  v6 envelope.** This is the operative correctness claim.
- **CONCERN:** the safety of this claim depends on the published fiducial
  never being widened during analysis-stage tuning. If any future fiducial
  loosening (e.g., dropping the y > 0.1 floor) is contemplated, the v6 gen
  envelope's `y_gen > 0.01` floor must be revisited. Today the margin is
  large (0.1 vs 0.01) and operationally adequate; the bound must be tracked
  alongside the published-fiducial freeze.

### 3.3 gen∉ → reco∈ (reco_only under signal fiducial) — SOUND for existence

- **Smoke count:** 841 signal-fiducial reco-only events; 0 reco-only events
  under the no-fiducial diagnostic (because every event in `ChargedTot` is
  by construction also in `ChargedTotGen` once it is kept by the Stage 1
  OR and Stage 2 fills gen).
- **Interpretation:** Smear-in migration: events whose generator kinematics
  fall outside the published signal fiducial but whose reconstructed
  kinematics fall inside it. Required for correct unfolding of resolution
  smearing. v6 retains these because (a) `pass_reco_envelope` keeps the
  event and (b) gen tracks are still written. This is the column the prior
  pipeline did retain, and v6 preserves it.
- **VERDICT:** **SOUND** for existence and population.
- **Sub-claim under no-fiducial counting:** `reco_only=0` is correct by
  construction. Any event that triggers Stage 1 via the reco leg also gets
  its gen leg written by Stage 2, so it lives in both trees by `(Run,
  Event)`. This is a healthy schema-level invariant: there should be no
  Stage 2 reco events without a matching `ChargedTotGen` row in MC. The
  smoke file satisfies this. No issue.

### 3.4 gen∉ → reco∉ (both outside published fiducial) — INSUFFICIENT EVIDENCE by construction (acceptable)

- **Smoke count:** Not directly observable. As explicitly documented in
  `docs/methodology/migration_qa_script_guide_2026-06-05.md` §2, events that
  satisfy neither published fiducial are not written to either tree by
  construction and cannot be counted from the Stage 2 output.
- **Physics relevance:** For TUnfold AREA-normalised response matrices the
  (out, out) quadrant is not required as a populated bin; it is absorbed
  into the off-fiducial integral that the analysis does not measure.
  Standard practice. The smoke-QA's inability to count it is by design.
- **CONCERN (residual):** an event with `Q2_gen < 60`, `y_gen < 0.01`,
  `y_gen > 0.99`, or `GenEmpz < 25` or `GenEmpz > 75` AND reco-failing is
  excluded from the Stage 1 keep. This is the correct behavior so long as
  the published fiducial stays well inside the envelope on each axis.
- **VERDICT:** **SOUND by design**; verifying the quadrant populationally
  is neither possible from the Stage 2 output nor required for unfolding
  correctness, provided the envelope contains the fiducial on all
  dimensions (it does).

### 3.5 Quadrant summary

All three observable quadrants are populated and counted in the smoke. The
fourth quadrant is non-observable by construction and not required. The v6
OR-envelope therefore captures **the migration directions that physics
requires for an unbiased response matrix**, subject to the gen-cut bias
concerns enumerated in §4.

---

## 4. Residual Gen-Level Cuts and Their Unfolding-Bias Risk

The user explicitly asked the critic to evaluate **each** remaining gen-side
selection that could bias unfolding. Each is graded.

### 4.1 `Q2_gen > 60` (Stage 1 gen envelope) — SOUND

- Published gen fiducial: `Q² > 150 GeV²`. Envelope floor 60 is 2.5× below
  the fiducial floor in absolute scale, ~1.5σ below in `log Q²` units for
  typical resolution `σ(Q²)/Q² ~ 10–15%`. Even a 5σ down-smear of an event
  with true `Q² = 150` lands near `Q² ~ 75`, comfortably inside the
  envelope.
- The QA `per_variable_single_cut.Q2` column shows the Q² fiducial is the
  largest single-variable driver of `gen_only` (16,294) and `reco_only`
  (303) under the signal fiducial; these events are real migration partners
  and the v6 envelope retains them.
- **No unfolding-bias risk under the current published fiducial.** Document
  this margin alongside the binning freeze so any future Q² fiducial
  loosening triggers a Stage-1 re-evaluation.
- **Verdict: SOUND.**

### 4.2 `0.01 < y_gen < 0.99` (Stage 1 gen envelope) — CONCERN

- Published gen fiducial: `0.1 < y < 0.7`. Envelope is `(0.01, 0.99)`, so
  the lower margin is `0.1 − 0.01 = 0.09` in y (10× tighter on the floor
  than on Q²) and the upper margin is `0.99 − 0.7 = 0.29`.
- y resolution at low y is poor (`σ(y)/y ~ 20–30%` for `Σ`-method below
  `y ~ 0.1`). A true `y_gen = 0.05` event can smear up to `y_reco ~ 0.06–0.08`
  and miss the published reco floor; that is the correct gen-leg migration
  to capture. The envelope `y_gen > 0.01` admits these events. ✓
- **However:** below `y ~ 0.01` the `Σ`-method becomes pathological and the
  v6 gen envelope correctly truncates. The truncation is far below the
  published floor and unlikely to bias the analysis, BUT this is a
  judgement call rather than a derivation. The `per_variable_single_cut.y`
  row shows y is the second-largest gen_only driver (28,311 no-fiducial
  gen-only events come from the y dimension), and a fraction of those at
  `y_gen ∈ (0.005, 0.01)` would migrate into `y_reco > 0.05` after
  resolution. These are dropped by the v6 envelope.
- **Risk magnitude:** likely small (the truncated range is narrow and the
  population there is low because of the `1/y² σ_red` weighting in the
  cross-section), but it has **not been quantified for this file** and
  there is no Stage 1 audit that scans the (y_gen, y_reco) plane below
  the floor.
- **Verdict: CONCERN.** Mitigation: either (a) lower `y_gen` floor to
  e.g. `1e-3` and re-run the smoke to verify the additional gen population
  is negligible vs the signal-fiducial both/gen_only counts, or (b)
  produce a 2D `(y_gen, y_reco)` migration plot from the smoke and
  document that the missing strip below `y_gen = 0.01` contributes
  `<1%` of the signal-fiducial response matrix integral. Document the
  decision in the gen-fiducial methodology file.

### 4.3 `25 < GenEmpz < 75` (Stage 1 gen envelope) — SOUND

- Published gen fiducial: `35 < GenEmpz < 70`. Envelope margins:
  `35 − 25 = 10 GeV` on the low side; `75 − 70 = 5 GeV` on the high side.
- Empz resolution is dominated by HFS energy resolution, conservatively
  `σ(Empz) ~ 2–3 GeV` at HERA-II in the SIDIS regime. Both margins are
  several σ, so the envelope contains the fiducial in the migration sense.
- The QA `per_variable_single_cut.Empz` is also benign (only 27
  no-fiducial `gen_only` events differ between Empz alone and full
  fiducial after Empz is added).
- **Verdict: SOUND.**

### 4.4 `zGen` finite-only filter in the v6 response builder — CONCERN

- The user's prompt and the QA JSON's `z_gen_range` block confirm: only the
  finiteness of `zGen` is required; `zGen < 0` count = 72, `zGen > 1` count
  = 3, and `z_min = −0.86`, `z_max = 12.44` in the smoke file.
- The 72 `zGen < 0` and 3 `zGen > 1` entries are **per-track**, not
  per-event, and arise from tracks where the gen-hadron longitudinal
  momentum fraction along the photon axis is computed for hadrons in the
  current-fragmentation veto region or from numerical edge cases. These
  values are unphysical in the SIDIS definition `z = P·p_h / P·q ∈ [0, 1]`.
- **If** the response builder propagates them into TH2 bin computations
  without an explicit `0 ≤ zGen ≤ 1` truncation, they end up in an
  underflow/overflow bin or — worse — in the lowest/highest physical bin
  if the indexer wraps. The current v6 builder header documented in
  `v6_production_changes_2026-06-04.md` §"Change #1" lists
  `zGen finite in [0,1] (numerical sanity, not a fiducial cut)`. The
  conflict between this written claim (`zGen finite in [0,1]`) and the
  smoke filter as implemented (`finite only`) is unresolved by source
  inspection — the C++/Python code lives on NAF.
- **Verdict: CONCERN.** Either the response builder enforces `0 ≤ zGen
  ≤ 1` and the documentation is correct (then the 75 outliers are dropped
  cleanly; verify by adding an explicit count to the next smoke), or the
  builder only filters `isfinite(zGen)` and the 75 outliers can leak into
  the response (potential per-charge `~10^-5` bias, likely negligible but
  unaudited). Either way, **the explicit form of the cut must be confirmed
  by reading the v6 response-builder source** before production launch,
  and the QA smoke must report a `zGen ∈ [0,1]` count post-builder, not
  just pre-builder.

### 4.5 Species pool `|GenPDG| ∈ {211, 321, 2212}` — SOUND

- This is a **species definition**, not a kinematic fiducial. The observable
  is "charged π + K + p", consistent with the published charged-hadron
  fiducial. Removing electrons, muons, and anti-matter equivalents handled
  via charge sign is the correct truth-level definition of the measured
  cross-section.
- **Verdict: SOUND** as a definitional choice. Document explicitly in the
  cross-section observable definition so consumers (TMD global fits) know
  which species are included.

### 4.6 Prompt / GTR via `IsV0DaughterGen==0` (V0-daughter veto) — SOUND with caveat

- This veto removes feed-down decay daughters of `K⁰_S`, `Λ`, `Λ̄`, and
  other long-lived gen-level V0s from the prompt-hadron observable. Without
  it, the "prompt charged hadron" definition is contaminated by ~5–10% of
  daughter tracks that decayed in flight and are matched as primary by the
  reco-side classifier.
- This is the **standard H1 SIDIS truth definition** and matches the
  matched-reco veto via `IsMatchedV0Daughter==0`. The two vetos are paired
  and symmetric across the response axes, which is the correct treatment.
- **Caveat:** the V0-daughter classifier `H1OO ProtonUtil::classifySourceGTR`
  + `findVetoAncestorPDG` must produce consistent flags between Stage 1 and
  Stage 2 (Stage 1 sets the keep, Stage 2 fills the branch consumed by the
  response builder). If the Stage 1 keep used a different ancestor-walk
  depth than the Stage 2 branch, the gen tracks written for kept events
  would not match the response-builder's view. **Not verified in this
  audit.** Standard H1 practice is for both to call the same H1OO
  classifier; assume so unless the source on NAF says otherwise.
- **Verdict: SOUND**, conditional on the Stage1/Stage2 V0-daughter flag
  consistency being verified once on a non-smoke sample.

### 4.7 No `xFGen` cut in the gen envelope or response builder — SOUND

- The published gen fiducial uses `xF_Trento < 0` (target-region SIDIS
  definition), so the gen tracks with `xFGen ≥ 0` will be discarded by the
  final TUnfold truth-selection regardless. Removing the cut from the
  envelope is correct because (a) `xF_reco` resolution near 0 is poor and
  a reco-side `xF < 0` event can correspond to a gen-side `xFGen ≥ 0`
  event, which is exactly the smear-in migration that must be retained
  for unbiased unfolding.
- The QA `per_variable_single_cut.xF` row confirms a modest but non-zero
  reco_only signal (30 events) for the xF dimension alone, consistent
  with this migration mechanism.
- **Verdict: SOUND.** xF retention in the envelope is the correct v6
  decision and matches the documented research conclusion in
  `docs/research/research-xF-cut-decision-2026-05-27.md`.

### 4.8 No `GenPt` or `GenEta` cut in the gen envelope or response builder — SOUND

- Per `docs/methodology/v6_production_changes_2026-06-04.md` §"Change #1",
  the v6 response builder explicitly removed `GenPt > 0.15` and
  `|GenEta| < 4`. These were geometric symmetrizations with the reco-side
  acceptance and are now correctly recovered through the response matrix
  rather than the gen denominator. Removing them widens the gen
  denominator and lowers the per-bin efficiency uniformly, which is the
  intended effect.
- **Verdict: SOUND.** This is the correct direction; the prior denominator
  was biased toward higher apparent efficiency.

### 4.9 No gen-hadron requirement in the Stage 1 gen envelope — SOUND

- Stage 1 keeping an event purely on the gen DIS-event envelope, even with
  zero gen hadrons in the published fiducial, is the correct behavior
  because: (a) Stage 2 fills `ChargedTotGen` per track, so a kept event
  with zero in-fiducial gen tracks contributes 0 entries to the gen
  numerator and ~1 entry to the gen DIS denominator (via the
  unique-event-key reconstruction documented in
  `plan-v6sys-treebuilder.md`); (b) requiring a gen hadron at Stage 1
  would bias the gen-side DIS-event count and silently break the
  multiplicity normalization.
- **Verdict: SOUND.**

---

## 5. Consistency Checks on the Smoke Numbers

These are sanity checks the critic ran against the QA JSON to look for
arithmetic or schema inconsistencies.

### 5.1 Stage 1 keep totals vs Stage 2 unique events — SOUND with a small discrepancy noted

- Stage 1 reports `45 reco-only + 50,712 gen-only + 25,383 both = 76,140` kept.
- Stage 2 `ChargedTotGen.n_unique_events = 76,140`. ✓ exact match.
- Stage 2 `ChargedTot.n_unique_events = 25,048`.
- QA JSON `categories.no_fiducial.both = 25,048` and `gen_only = 51,092`,
  `reco_only = 0`.
- **Discrepancy:** Stage 1's `both` (25,383) > QA no-fiducial `both`
  (25,048), by 335. Stage 1's `gen_only` (50,712) < QA's `gen_only`
  (51,092), by 380. Net is approximately neutral (335 ≈ 380 within
  rounding). The QA script attributes this to the event-level any-track
  approximation: an event whose reco tracks all fail offline track-quality
  cuts at the QA level (the QA uses just track-presence; Stage 1 uses the
  loose Pt > 0 + IsFromPrimary + IsCentralTrk gate) will be classified
  differently between Stage 1 and the QA. **335 / 25,000 ≈ 1.3%** of the
  reco-side events shift category between the two definitions, which is
  the expected order of magnitude.
- **Verdict: SOUND**, with the standing caveat that the any-track event
  classifier in the QA is not the per-track matched response and these
  ~1–2% migrations are not informative about correctness; they are
  informative about the diagnostic's resolution.

### 5.2 `reco_only = 0` under no-fiducial — SOUND (invariant satisfied)

- Every reco event in `ChargedTot` should also be in `ChargedTotGen` for MC,
  because Stage 2 fills the gen leg for every Stage 1 kept event including
  reco-only ones. The smoke confirms this invariant.
- For data, `ChargedTotGen` is correctly absent.

### 5.3 `TrackLength` sentinel in MC and absent `Start/End` — INSUFFICIENT EVIDENCE for current-review purposes

- The user reports `TrackLength sentinel` and `Start/End absent` in this
  smoke MC. The migration-coverage critique does not depend on these
  branches; they are tracking-quality diagnostics evaluated downstream
  per Change #2 in `v6_production_changes_2026-06-04.md`. Out of scope for
  this critique. Flagged here only so the next dispatch knows the Lee-West
  diagnostics are not yet populated in this smoke and the v6sys
  `TrackQualityPayloadVersion = "v6sys-trackqual-2026-06-04"` should be
  checked on a fresh smoke after the C++ binary is rebuilt.
- **Verdict: INSUFFICIENT EVIDENCE** (and out of scope for migration
  coverage).

### 5.4 Data FPACK abort absent — SOUND

- `ChargedTot = 16,889` data tracks, no abort, no `ChargedTotGen`. As
  expected for v6 data path (`pass_reco_envelope` only, no gen leg).

---

## 6. Validation Plots / Tables Required Before Full Production Launch

The smoke is sufficient to demonstrate **category existence and schema-level
plumbing correctness**. It is not sufficient to demonstrate **unfolding
unbiasedness on the published binning**. The following are required (not
optional) before launching the full v6 production:

### 6.1 Mandatory — block production until done

1. **Per-charge response-matrix closure on a multi-file MC sample.** Build
   the full TH2 migration matrix per charge from the v6 response-builder
   on at least one full DIS MC period (e.g., djangoh14 em06), apply it
   to the same MC's reco-level distribution, and compare the unfolded
   result to the MC truth on the published 210-bin scheme. Acceptable
   closure: per-bin relative residual <2% statistical-only, no systematic
   trend with z, Q², or x. This is the actual unfolding-bias test; a
   single-file smoke cannot perform it.
2. **2D `(Q²_gen, Q²_reco)` migration plot per charge.** Verify that the
   distribution outside the published fiducial reaches all the way to the
   `Q2_gen = 60` floor with non-trivial event content and shows no edge
   pile-up. Edge pile-up would indicate the v6 envelope is still cutting
   into the published-fiducial smear-out tail.
3. **2D `(y_gen, y_reco)` migration plot per charge.** Same as above for y.
   Specifically inspect whether the `y_gen < 0.01` strip contributes
   non-negligibly to the `y_reco > 0.1` integral. If it does (above ~1% of
   the total response in any bin), the y_gen envelope floor must be
   lowered.
4. **2D `(GenEmpz, Empz)` migration plot per charge.** Same for Empz, with
   focus on the `GenEmpz < 35` strip and its contribution to the
   `35 < Empz < 70` slice.
5. **zGen post-builder count.** A QA pass that reports the number of
   `zGen` values in `(−∞, 0)`, `[0, 1]`, and `(1, ∞)` **after** the v6
   response builder ingests the tree. If non-zero outside `[0, 1]`, the
   builder must apply an explicit `0 ≤ zGen ≤ 1` clip or be modified to do
   so. The 75 outliers in the smoke are an upper-bound estimate of the
   leakage size; even at this scale the bias is `<<1%` per bin, but the
   leak should be closed by code or proven absent by audit.
6. **Stage1/Stage2 V0-daughter flag consistency check.** A single QA call
   confirming `IsV0DaughterGen` and `IsMatchedV0Daughter` come from the
   same H1OO classifier call (or call paths that produce bit-identical
   results) at both Stage 1 and Stage 2, so that the gen events kept by
   Stage 1's veto contain gen tracks consistent with the response
   builder's veto.

### 6.2 Recommended — produce before production but not blocking

7. **xF migration 2D plot per charge.** Quantify smear-in across `xF = 0`
   and confirm the no-xF-cut envelope is necessary (we expect ≥5% of
   reco-fiducial events to have `xFGen ≥ 0`; if fewer, the envelope
   widening is operationally redundant but not harmful).
8. **GenEmpz upper-edge audit.** The published fiducial top is 70 GeV; the
   envelope top is 75 GeV. Confirm `GenEmpz > 75` event count is
   negligible (the typical truth `GenEmpz` distribution peaks at ~55 GeV
   for ep DIS at HERA-II, so this should be cosmetic).
9. **Event-classifier alignment.** Either upgrade the migration-QA script
   to use the per-track matched response on its event-level
   classification or add a written note quantifying the ~1.3% any-track
   vs Stage 1 classifier disagreement seen above as the diagnostic's
   intrinsic floor.

### 6.3 Already covered by existing documentation

- Stage 1 / Stage 2 schema definition: `v6_production_changes_2026-06-04.md`
- Migration-QA methodology: `migration_qa_script_guide_2026-06-05.md`
- Gen fiducial decision history: `gen_fiducial_acceptance_2026-05-24.md`,
  `research-xF-cut-decision-2026-05-27.md`
- v6 response-builder change log: `v6_production_changes_2026-06-04.md`
  §"Change #1"

---

## 7. Summary Matrix

| Item | Verdict | Action |
|---|---|---|
| gen∈/reco∈ migration captured | SOUND | none |
| gen∈/reco∉ migration captured | SOUND for existence; SOUND for completeness inside published fiducial | document that envelope ⊃ published fiducial on every axis; freeze together |
| gen∉/reco∈ migration captured | SOUND | none |
| gen∉/reco∉ unobservable | SOUND by design | none |
| `Q2_gen > 60` floor | SOUND | document margin to `Q² > 150` fiducial |
| `0.01 < y_gen < 0.99` floor | **CONCERN** | quantify `y_gen ∈ (0, 0.01)` contribution to fiducial response; lower floor if non-negligible |
| `25 < GenEmpz < 75` window | SOUND | document margins |
| `zGen` finite-only filter | **CONCERN** | confirm response-builder enforces `0 ≤ zGen ≤ 1`; report post-builder count |
| Species pool `{π, K, p}` | SOUND | document in observable definition |
| V0-daughter prompt/GTR veto | SOUND with caveat | one-time consistency check between Stage 1 and Stage 2 flag generation |
| No `xFGen` cut | SOUND | none |
| No `GenPt`, `GenEta` cuts | SOUND | none |
| No gen-hadron requirement | SOUND | none |
| Single-smoke-file basis | **CONCERN** | run §6.1 closure on a full DIS-MC period before production |

---

## 8. Bottom-Line Recommendation

- The v6 OR-envelope **captures all three observable migration directions**
  in the smoke evidence provided and **avoids the dominant gen-cut biases**
  (`GenPt`, `GenEta`, `xFGen`) that the prior pipeline carried.
- It does **not yet** demonstrate unfolding closure on the published
  binning; this requires §6.1 items 1–6.
- **Two non-fatal but explicit risks remain** before production:
  (a) the `0.01` floor on `y_gen` needs a quantitative defense against the
  published `0.1` reco floor, and (b) the `zGen` filter in the response
  builder must be confirmed to be `[0, 1]` (not just finite).
- **Recommended next dispatch:** h1-physics-analyst + h1-naf-operator to
  run the §6.1 mandatory closure on one full djangoh14 em06 period; in
  parallel, h1-builder-engineer or h1-software to confirm the response-
  builder zGen clip and the Stage 1 vs Stage 2 V0-daughter consistency.
- **Do not absolutize the assurance** as requested by the user. Migration
  coverage is provably correct in the smoke at the schema and category
  level; it is not provably unbiased on the 210-bin published TH2 until
  closure is demonstrated on a sufficient MC sample. The smoke is a
  necessary, not sufficient, gate.

---

## 9. Open Questions / Gaps in This Critique

- The v6 Stage 1 source code (`TreeBuilderHighQ2_chargedsidis_v6sys.C` or
  the v6-relabeled equivalent) and the v6 response-builder
  (`build_migration_th2_per_charge_v6.py`) were not read directly during
  this critique; both live on NAF and were not transferred. The critic's
  verdicts rely on the user's stated semantics, the QA JSON, and the
  documentation files in `H1/sidis_highQ2/docs/methodology/`. If the live
  source diverges from the stated semantics in any of the listed cuts
  (especially the `zGen` clip and the Stage 1 V0-daughter flag origin),
  the verdicts in §4.4 and §4.6 must be revisited.
- The data-side audit is limited to confirming `ChargedTot = 16,889` and
  the absence of `ChargedTotGen`. Data-side fiducial behavior is not
  checked here because it is identical to v5g by construction (only the
  reco envelope changed) and the data file did not produce an FPACK abort.
- The audit is on a single em06 file (run 7072, file 0007). All numerical
  category counts cited are file-local. Per-period and per-sample
  variation is expected; the §6.1 closure must use the full production
  stack.

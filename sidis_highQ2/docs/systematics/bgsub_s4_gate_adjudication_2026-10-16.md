# Cone-basis bgsub Set-R′ chain — S4 gate QA adjudication (2026-10-16)

**Author:** h1-qa-specialist (read-only QA adjudication session).
**Date of adjudication:** 2026-10-16, at the request of the H1 charged-SIDIS
supervisor, **prior to the supervisor adoption decisions** of the same date.
**Date of persistence:** 2026-10-16 (this document is the persisted record of
that adjudication; it is cited by the spec `adoption_addenda` and by the dated
gate record as "QA adjudication 2026-10-16").

**Chain tree (v6/z005 high-Q² inclusive charged-hadron SIDIS, Q² > 150 GeV²,
h+/h− without PID, 490-bin release grid):**

- Study tree: `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/stage2_setrprime_2026-10-15/`
- Gate records (retained 2026-10-15 edition + dated 2026-10-16 re-run edition):
  `.../stage2_setrprime_2026-10-15/gate/bgsub_gate_26items_official.json`,
  `.../stage2_setrprime_2026-10-15/gate/bgsub_gate_26items_official_2026-10-16.json`
- Stage measurement: `.../stage2_setrprime_2026-10-15/stage_rel/bgsub_stage_rel_measurement_official.json`
  (+ sibling signed-difference CSV; dated 2026-10-16 edition alongside).
- Candidate: `.../stage2_setrprime_2026-10-15/born_release_candidate/born_crad_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-15/`
- Campaign ledger: `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/RUN_SHEET.md`
- Condor budget (P5, spent 2026-10-15): clusters **5658879** (job 1,
  SMOKE-ONE throwaway) and **5658880** (job 2, OFFICIAL chain, exit 0).

**Status:** ADJUDICATION ONLY — read-only. No production was launched, no
Condor jobs were submitted, and no gate record, spec, RUN_SHEET, candidate, or
campaign product was modified by the adjudication. At persistence time every
key number below was re-verified against the on-disk gate records (both
editions); **no measured number changed in the 2026-10-16 re-run edition**
(it re-evaluates the verdict only, on the same measured numbers — no
re-measurement). This document is the only write (additive new document).

**Scope:** QA adjudication of the three non-PASS items of the S4 26-item
official gate (2026-10-15 edition: 23 PASS / 2 FAIL / 1 HELD,
`all_25_pass_for_promotion` false) — CP-1, PM-3, PM-4 — including a soundness
assessment of the 2026-10-15 CP-1 physics ruling
(`bgsub_palpha_cp1_ruling_2026-10-15.md`), ahead of the supervisor's adoption
decisions.

---

## 1. CP-1 — stage-consistency item

### 1.1 Item definition (as gated)

Per-bin

```
|pre-unfold − coneBorn·(1 − f_res′)| / pre-unfold
```

with **operands = the canonical cone Born release**
(`born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13`;
unsubtracted cone Born totals h+ 13.026987 / h− 12.697511 nb). Thresholds:
per-bin ≤ 1%; sparse bins ≤ 2% with written justification; integrated ≤ 0.5%.
Sparse criterion fixed a priori: `toy_stat_rel ≥ 0.05` (the bin's own released
statistical uncertainty at least 5× the per-bin window).

### 1.2 Recorded FAIL detail (2026-10-15 official edition)

- **Integrated PASSED** on both charges: 1.8666e-4 (h+) / 2.2198e-4 (h−) ≤ 5e-3
  (≈1.87e-4 / 2.22e-4; identical to the RU-3 integrated deviations
  −0.0187%/−0.0222%; post-vs-coneD anchor cross-checks 0.0/0.0).
- **11 non-sparse bins > 1%**: h+ 4 (106, 176, 181, 211); h− 7 (59, 106, 175,
  176, 181, 195, 211). All 11 are non-sparse by the a-priori criterion, so the
  pre-registered sparse exception path cannot cover any of them.
- **Bin 176 exceeds the 2% sparse window on BOTH charges while non-sparse and
  above its own toy stat**: h+ 2.084% / h− 2.290% vs candidate toy_stat
  1.389%/1.420%.
- Sparse path itself behaved exactly as pre-registered (8 h+ / 10 h− >2%
  sparse bins, all below own stat; 7+7 sparse (1,2]% bins below own stat).
- The gate correctly recorded FAIL rather than reinterpreting pinned criteria
  at execution time (genuine STOP condition; RUN_SHEET CHAIN STOP AT S4).

### 1.3 Soundness assessment of the 2026-10-15 ruling (adjudicated 2026-10-16)

The ruling's EXPECTATION-REVISION finding ((a): the adjudicated sparse-only
CP-1 expectation was wrong; the chain is not defective) was assessed SOUND on
the official products, on the following evidence, all re-derived read-only:

- **Deterministic deviations:** against the physically correct
  subtraction-uncertainty null (σ_Δ combining template-MC statistics via
  `bgsub_stat_rel` with O(f) toy-sidecar terms), **z_sub = 2.9–25.1σ** over the
  11 failing rows (bin 176: 23.0σ h+ / 25.1σ h−). The data-Poisson term that
  dominates `toy_stat_rel` **cancels bitwise between the two operands** (CP-2:
  `data_sub′ + B_R′_cone == D_cone` bitwise), so what remains — template-MC
  statistics at full size — is far too small to produce 1–2.3% deviations in
  these bins. Against naive own-stat resolution nothing resolves
  (z_own 0.12–1.61; Bonferroni over the non-sparse multiplicity p = 1.00 both
  charges), which is exactly why the correct null matters.
- **Charge-correlated:** Pearson r of the signed relative stage deviations =
  **0.912** on the 182 matched bins where the subtraction actually acts
  (f_res′ ≥ 2%; 0.839 on all 245; sign agreement 90.1%). **All 7 union bins
  {59, 106, 175, 176, 181, 195, 211} deviate with the same sign in both
  charges**, including the three single-charge failures; the four
  doubly-failing matched bins (106, 176, 181, 211) have nearly equal
  per-bin signed magnitudes across independent samples (e.g. bin 211:
  +0.0011100 vs +0.0011116 nb, 0.14% apart).
- **Spatially structured:** the field is a **z0 pT0/pT1 dipole** — all 14
  z0-pT1 bins under-removed (signed positive; unfolded removal 19–37% below
  the diagonal B_R′ in the failing cells, deficit −1.0…−2.2 pp), 10 of 14
  z0-pT0 partners over-removed (largest f_res′ in the field, 10–14.4%, on the
  over-removal leg) — consistent with pT-boundary migration leakage of the
  cone-applied (low-pT-enriched) templates through the S2′ variant response.
- **Integral-closing:** the net field closes in integral
  (−0.0187%/−0.0222%), i.e. the dipole redistributes rather than adds.
- **Machinery bitwise-verified:** every defect-indicating item passed exactly
  on the official products (RU-1 graded/bitwise reproduction of the canonical
  cone RAW release; CP-2 add-back closure bitwise 0.0; RU-2 cone-variant f_i
  byte-identical with integrated removal 11.3063%/10.9326% in windows; RU-4
  C_rad bitwise, table sha `cf810be4…` == spec pin; RU-5 physicality +
  exact-text serialization max dev 0.0; SA-1..SA-5 and IV-1..IV-7 exact vs
  pins).
- **Normalization channel excluded:** both CP-1 operands consume the same
  B_R′ field, so a global template-normalization error cancels to first
  order; attributing the integrated deviation to a global B error would
  require |δ| ≈ 0.26% of B, while the failing bins show *local*
  unfolded-removal mismatches of 19–37% of the local B. No normalization-error
  channel can produce the observed dipole.
- **Expectation-revision = pre-S2′ extrapolation corrected:** the sparse-only
  expectation was an extrapolation from the pre-S2′ no-cone Set-R chain, where
  the same 14 bins showed stage differences of only 0.04–0.30% with zero
  non-sparse bins >1%. The cone-applied templates roughly doubled f_res′ in
  exactly the affected cells (bin 176: 2.98%→5.99% h+, 2.92%→6.03% h−; bin
  175: 3.57%→13.66% / 3.66%→14.38%), so the response-mediated non-locality —
  which scales with the subtracted fraction, not with the released statistical
  resolution — grew correspondingly. The expectation, not the chain, was
  wrong.
- **κ ≤ 0.5 post-hoc calibration noted, tolerable, no number changes:** the
  proposed non-sparse exception path's bound κ = |signed_diff|/(born·f_res′)
  ≤ 0.5 was calibrated on the realized field (κ percentiles over the 487 bins
  with f_res′ > 0: p50 0.069, p75 0.162, p90 0.329, p95 0.501; all 11
  non-sparse exceptions κ ≤ 0.365). Assessed tolerable because the path is
  evidence-gated (same sign in both charges AND κ ≤ 0.5), non-absorbing
  (every exception carries its **full measured difference** in
  `bgsub_stage_rel`; the 19-component budget and `total_with_bgsub19_rel`
  require no re-derivation), original thresholds are kept verbatim, any bin
  exceeding 1% that satisfies neither path remains a FAIL, and the
  original-criterion FAIL verdict is retained verbatim in the dated records.

### 1.4 CP-1 verdict

**SOUND — no substantive gate-weakening.** The ruling's physics attribution
(deterministic, charge-correlated, spatially structured, integral-closing,
bitwise-verified-machinery, normalization-excluded) is correct on the official
products, and the proposed criterion revision keeps every original threshold,
adds only a physically-motivated recorded-exception path, and changes no
measured number. Adopting it converts the realized measurement to
PASS_WITH_RECORDED_EXCEPTIONS without weakening the gate's STOP discipline.

---

## 2. PM-3 — sidecar-hygiene item

### 2.1 Item definition (as gated)

Sidecar hygiene: the sha256 input ledger extended with the cone campaign
artifacts and carried inputs (cone DV JSON, cone S2′ response ROOTs/npz, cone
RAW/Born release IBU CSVs, f_i crosscheck CSVs, alpha-QA sums, Phase-F rerun
record, guard adjudication, carried electron template npz's, C_rad table); no
stale blind C_rad sidecars.

### 2.2 Recorded FAIL detail (2026-10-15 official edition)

- **Sole failure:** `ledger_extension_markers.AGGREGATED_djangoh14_lowq2 =
  false` — the lowQ2 template npz
  (`AGGREGATED_djangoh14_lowq2_djbkgd_L351000_finalcandidate.npz`, sha256
  `c40227f0…`) is absent from the candidate's 37-entry
  `provenance/inputs_sha256_ledger.json`.
- **All ledger pin equalities MATCH** (ledger == fresh == pin on every one;
  11 equalities in the gate record), manifest 33/33 good, 0
  blind/quarantine files; the applied C_rad table `cf810be4…` equals the spec
  pin (the older adjudication-text value `205e8218…` is STALE and superseded
  in the gate evidence).
- **No provenance actually lost:** the lowQ2 npz is a consumed, spec-pinned
  chain input (B_R′ = e + V0 + lowQ2; IV-1 lowQ2 total 208.3795 verified) and
  its sha256 is recorded in the S1 builder's
  `vectors/bgsub_setrprime_vectors.json` `sha256_inputs` — re-verified fresh
  against the file: identical. Root cause: the packager's `ledger_paths` block
  adds only the two electron template npz's; the gate hard-codes the lowQ2
  marker among its 14 required markers (stricter than the spec text).

### 2.3 PM-3 verdict and recommended resolution

**Packaging/provenance gap, not a product defect.** The finding has no physics
content and is independent of CP-1. **Option B recommended** (packager
`ledger_paths` extension + repackage): the parent scheme's PM-3 intent is a
sha256 ledger of **every input**; the gate marker is faithful to that intent,
and the packager omission is the defect — so the fix belongs in the packager,
not in a gate-marker amendment. Option B is deterministic, login-node,
0 Condor jobs, and leaves the physics content bit-identical (the ledger
carries no physics arrays); its cost is candidate-directory churn (new ledger
+ manifest), requiring a full 26-item gate re-run against the repackaged
candidate. (Option A, record-only annotation, would leave the marker false on
any re-run and would require a gate adjustment at promotion time — a weaker
disposition.)

---

## 3. PM-4 — state-rule item

PM-4 (`HELD_PENDING_GATE_PASS`) enforces the state rule: no merge into the
nominal chain until items 1–25 PASS on the OFFICIAL products (25×
pass-or-exception expected, with CP-1's expected terminal state
PASS_WITH_RECORDED_EXCEPTIONS and PM-4 READY_FOR_USER_DECISION). The realized
count was 23 — **the shortfall is exactly CP-1 + PM-3; no third blocker
exists.** PM-4 is **content-independent** of the CP-1 ruling: the promotion
content it guards is already adjudged on its own merits (CP-5 budget closure
PASS: 15 components byte-identical to the installed leg-envelope package,
21,070 byte-carry cells, 0 mismatches; 19-component quadrature identity max
dev 0.0 ≤ 1e-12; `bgsub_stage_rel` already carries the full measured per-bin
differences). **PM-4 unblocks via the normal state rule through a post-fix
gate re-run** — no further physics work of any kind is implied by either
resolution.

---

## 4. Path-to-completion finding

- **0 further Condor jobs needed.** The P5 2-job budget was already spent
  (clusters 5658879, 5658880, both 2026-10-15); both resolutions and the gate
  re-run are deterministic login-node operations (the re-run measured ~2 s).
- **Products reusable as-is.** The candidate stays byte-frozen; the CP-1
  resolution is verdict-only (same measured numbers, no re-measurement); the
  PM-3 fix touches only the provenance ledger (physics content bit-identical);
  the 19-component budget and `total_with_bgsub19_rel` require no
  re-derivation under either resolution.
- **No risk to the 23 PASS items.** The resolutions change classifications
  and annotations, not values — every previously-PASS item evaluates on
  bit-identical content. (Subsequently verified in the re-run: all 23
  previously-PASS items reproduced PASS; status changes confined to
  CP-1/PM-3/PM-4, all in the expected direction.)

---

## 5. Subsequent recorded facts (2026-10-16, post-adjudication)

Recorded here as facts with their on-disk record, not as adjudication content:

- **Supervisor adoptions (both 2026-10-16, both citing this adjudication as
  "verified sound"):**
  1. **CP-1 revised criterion** — adoption of the 2026-10-15 ruling
     EXPECTATION-REVISION (`bgsub_palpha_cp1_ruling_2026-10-15.md` §5 (a)):
     original thresholds KEPT (≤1% per bin, ≤2% sparse-justified, ≤0.5%
     integrated) PLUS the non-sparse charge-correlated non-locality exception
     path (same sign in both charges AND κ ≤ 0.5). Recorded as a DATED
     ADDENDUM in the machine spec `adoption_addenda`; original criterion text
     NOT rewritten.
  2. **PM-3 Option B** — packager `ledger_paths` extension + repackage, per
     the rationale of §2.3 above; recorded in the same spec addendum.
- **PM-3 fix executed** (login node, deterministic, 0 Condor jobs):
  repackaged ledger `.../stage2_setrprime_2026-10-15/pm3_ledger_repackage_2026-10-16/inputs_sha256_ledger.json`
  (38 entries = 37 original byte-identical + the lowQ2 npz) +
  `repackage_record_2026-10-16.json`; official candidate 34 files
  sha256+mtime UNCHANGED (physics content bit-identical).
- **Dated stage-rel edition** (revised criterion, SAME measured numbers — no
  re-measurement; dated signed-difference CSV byte-identical to the retained
  official CSV; original FAIL verdict retained verbatim inside the dated
  JSON).
- **Gate re-run outcome** (official dated edition
  `gate/bgsub_gate_26items_official_2026-10-16.json`; prior
  `bgsub_gate_26items_official.json` RETAINED superseded-not-modified):
  **24 PASS + CP-1 PASS_WITH_RECORDED_EXCEPTIONS (11 non-sparse exceptions,
  κ ≤ 0.3652, all same-sign both charges; sparse exceptions unchanged 8+10) +
  PM-3 PASS (evaluated against the repackaged 38-entry ledger; all pin
  equalities MATCH; all 14 markers TRUE incl. AGGREGATED_djangoh14_lowq2) +
  PM-4 READY_FOR_USER_DECISION**; n_fail 0, n_held 0;
  `all_25_pass_for_promotion` true; `matches_expected_terminal_states` true;
  adoption records embedded (`adoption_records_2026_10_16`). S4 acceptance
  met; S5 remains USER-GATED (P5_RECORD, STATE.md P-ALPHA closure entry,
  candidate promotion, quoting switch — all user decisions; nothing canonical
  touched).

---

## 6. Persistence-time verification note

All key numbers in this document were re-verified on 2026-10-16 against both
on-disk gate editions (`bgsub_gate_26items_official.json`,
`bgsub_gate_26items_official_2026-10-16.json`), the stage-rel measurement, the
CP-1 ruling, and the campaign RUN_SHEET. The 2026-10-16 re-run edition changed
no measured number (verdict-only re-evaluation: identical per-charge blocks,
identical exception bins and κ values); the only changes are the
classification statuses recorded in §5. The integrated CP-1 deviations are
1.8666e-4 / 2.2198e-4 precisely (≈1.87e-4 / 2.22e-4). No corrections to the
adjudication's findings were required at persistence time.

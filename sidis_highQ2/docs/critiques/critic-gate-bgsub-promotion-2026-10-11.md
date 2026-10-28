# Critic Gate Record — Background-Subtracted (Set R) Born Chain Promotion
**Review type:** Physics-correctness critic gate review (read-only; critique authority only)
**Candidate:** `born_crad_bgsub_setR_centralbase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/`
(bgsub set-R pre-unfold subtraction chain, central pre-S2' base)
**Date:** 2026-10-11
**Verdict:** **FATAL — promotion blocked (PM-4 must be resolved as DO-NOT-PROMOTE as-is)**

## Material reviewed

- Governing spec: `docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md` (+ JSON sidecar), full.
- Authoritative background accounting: `docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`, §1–§12 full
  (ESTABLISHED/PROVISIONAL register, builder-predicate evidence, per-category ledger, exactly-once audit).
- S2' variant spec: `docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md` (§0–§3, incl. the §3 exactly-once
  ledger and the "full future chain" definition).
- Basis status: `STATE.md` (S2' promotion override), `docs/RUN_STATUS.md`, `docs/status/current-release.md`,
  `docs/status/radiative-corrections.md`, `docs/status/alpha0p2-background.md` (S2' sections).
- Verification: `docs/status/normalization-verification-bgfracsub-2026-10-10.md` (PASS WITH CAVEAT, §4–§7);
  `docs/critiques/publication-grade-baseline-review-2026-10-10.md` (C-1/L1).
- Build products: `output_v6/studies/bgsub_setr_build_2026-10-10/` — RUN_SHEET, 24-item gate JSON, budget_13to17
  (README + CSV header + JSON), stage_rel official JSON, official re-unfold/repro/addback records, vectors builder and
  re-unfold driver scripts, born_release_candidate (README, validation.json, summary_totals.json).
- G9/S1b audit: `output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/validation/template_vs_fakecolumn_report.json`, full.
- Observable: high-Q2 charged-hadron SIDIS Born cross-sections (h±, 490 release bins), background-subtracted chain.
  Evidence types: spec text, ledger (code-level, ESTABLISHED), gate JSON, per-bin budget CSV, template reports,
  arithmetic re-verification of headline numbers.
- Tooling note: the `sidis-hq2-backgrounds` skill failed to load in this session (two attempts); all findings rest on
  primary artifacts read directly.

## Fatal Issues

### F-1 (FATAL, analysis-killing): Set R omits the residual-electron class — the spec's fiducial adjudication contradicts the ESTABLISHED ledger; the candidate's cross-sections are biased high by ≈3.8–4.1% integrated, per-bin up to +27%.

**Claim criticized:** Spec §2.1 row "Non-V0 electrons | 100% fake column | h+ 3.749% / h− 3.963% (alpha0p2 D) | Already
removed by the response fake fraction — must NOT be subtracted again", and §2.2/§3.1 built on it (Set B rejected as
"fiducially wrong" for double-counting electrons; Set R adopted as "exactly the fiducial residual").

**Evidence:**
1. The 2026-10-07 consistency ledger (the document the same-day normalization verification names "authoritative
   background accounting") establishes the opposite, at builder-code level: the central response reco space is
   `pass_reco_full = evt & trk & not_e & not_v0_match_top` (builder lines 647–659) — MC-flagged electrons are vetoed
   out of the response **entirely, signal AND fake columns**; "the fake column contains no MC-flagged electrons and no
   MC-flagged V0 daughters" (§3); ESTABLISHED register items 3–4: the fake column contains only MC-unflagged unmatched
   reco tracks, and `f_i` (10.7141% h+ / 10.4019% h−) removes that class **only**.
2. Triple corroboration: the S2' spec §3 ledger (residual electrons: "nothing removes them (mask no-op in data;
   excluded from response) — under-subtracted"; external vector YES) and the 2026-10-10 normalization verification
   caveat 1 ("these figures quantify the MC-unflagged reco-fake class… they do **not** include the surviving residual
   categories: residual electrons 3.8355% cone-applied [≈4.1441% no-cone, PROVISIONAL]").
3. Internal impossibility within the spec's own §2.1: it asserts V0 daughters are "dropped from the response entirely"
   while electrons are "100% fake column" — but a single builder predicate (`not_e & not_v0_match_top`) treats the two
   classes symmetrically. Asymmetric handling cannot arise from this code path.
4. Provenance of the error: the "100% fake column" property is the **alpha0p2-convention** fact (ledger §5: alpha0p2
   builder has "no electron truth veto"; electrons 99.92% fake col there), and the spec's own magnitude tag is
   "(alpha0p2 D)". The only quantitative support the spec cites for fake-column content, the G9/S1b audit, is an
   allfake-chain artifact: its caveat states the content equality is exact "(S0/G2)" for the **allfake** fake column,
   where e/V0/det.int. are fake-column categories *by construction* (rerouted). It cannot prove the central response's
   fake column contains electrons. The spec never cites the 2026-10-07 ledger anywhere (verified by search).

**Physics impact:** Nothing in the candidate chain removes data electron contamination: B_R = V0+lowQ2 only
(h+ 67,211.631 = V0 67,092.201 + lowQ2 119.431), `f_i` cannot touch the class (excluded from response), and the
re-unfold maps the contamination onto signal. The omitted class is ≈148,976 template tracks = **3.8355%** of the
central data vector (cone-applied; **≈4.1441%** no-cone PROVISIONAL via the measured cone survival 7.3027% RG /
7.587% DJ), concentrated at low-z/low-pT with worst populated bins **26.95%** (h+ b349, S2x0, z∈[0.7,1), pT∈[6,8),
d=9) and **15.67%** at d≥100 (h− b350). First-order bias on the published Born totals: ≈ **+3.7% to +4.1% relative**
(≈0.47–0.52 nb on h+ 12.6235 nb), and the omission is **charge-asymmetric** (h+ 3.749% vs h− 3.963%, alpha-D) —
≈ **+0.21 pp** of charge-asymmetry contamination flowing into any h+/h− ratio observable. The candidate's own
precision claims (RU-3 −0.0283%, new components ≤0.19% medians) are an order of magnitude below the omitted class.
The ledger-prescribed published set under the central response is **e + V0 + lowQ2 = 281,250.09 tracks = 7.2411%**
(§6, §8: `d' = d − B(e) − B(V0) − B(lowQ2)`, e∩V0 overlap handled once); the candidate subtracts 132,273.656 = 3.41%,
i.e. **47% of the prescribed set**. Adding the electron vector is exactly-once-safe (ledger §7.1: fake column excludes
flagged e/V0, so external e/V0 vectors touch disjoint classes — no double-subtraction with `f_i`).

**Note:** Set B was rejected for a false reason (electrons are *not* already removed), but Set B would also have been
wrong: it includes det.int., whose external subtraction is FORBIDDEN under the central response (13.82% already in
`f_i`; 86.18% is response-modeled signal; ledger §7.2). The correct set is Set B minus det.int. = e+V0+lowQ2.

### F-2 (FATAL for promotion as-is): Observable-basis conflict — the candidate is built on the superseded pre-S2' base while S2' is the canonical basis by standing user decision.

**Claim criticized:** Promoting `born_crad_bgsub_setR_centralbase_..._2026-10-10` as the published background-subtracted
Born chain.

**Evidence:** The candidate's basis is pre-S2' throughout: central response sha256 `53234e21…`, central `f_i`
(integrated removal 10.7128%/10.4005% on the subtracted vector), repro target `bbb_ibu_coherentprior_..._2026-10-06`
(11.483372/11.190383 nb), budget parent = the 13-component emfix budget. STATE.md/RUN_STATUS.md record the S2'
detint-fake promotion (user decision 2026-10-10, documentation 2026-10-11) as canonical: RAW
`bbb_ibu_detints2prime_..._2026-10-09` (11.446174/11.158554 nb), Born `born_crad_dj14relbin_detints2prime_..._2026-10-10`
(13.064758/12.736523 nb), canonical budget = the 15-component S2' budget; the spec, build, and 24-item gate are all
S2'-blind, and STATE.md forbids quoting pre-S2' values without an observable-basis flag.

**Physics impact:** The S2' promotion changed the observable (det.int. Class 2 removed response-internally; variant
`f_i` data-weighted 11.3577%/10.9768%); the pre-S2'→S2' raw shift is **−0.324%/−0.284%** integrated — ~10× the CP-1
stage differences and ~2× the largest new budget component. A pre-S2'-basis chain cannot be promoted as the published
chain without an explicit user re-adjudication of the basis. Under S2', Set R's V0/lowQ2 vectors remain the correct
external classes (S2' spec §3: e/V0/lowQ2 external, unchanged) — so the cure is a re-base, not a scheme redesign.

## Major Concerns

- **M-1 (structural, follow-up mandatory in any re-run):** The 24-item gate (IV-1…5, SA-1…5, RU-1…5, CP-1…5, PM-1…4;
  22 PASS, CP-1 PASS_WITH_RECORDED_EXCEPTIONS, PM-4 READY_FOR_USER_DECISION) contains **no set-composition item**. It
  validates implementation-vs-spec; the spec-vs-physics layer failed at §2.1 and passed 24/24. Any re-run must add a
  gate item re-deriving the fiducial-residual set against the 2026-10-07 ledger before the chain is rebuilt.
- **M-2 (concern, material under the cure):** Template cone-survival transfer is unclosed. The electron no-cone
  rescale is quantified (÷0.9241/÷0.9270 → ≈4.1441% PROV) but the packaged templates are cone-applied; **V0/low-Q2
  cone-survival counters do not exist** (ledger §9: no-cone magnitudes unbounded). The spec's −0.011 pp bound covers
  the denominator swap only, not the numerator cone-survival deficit. The cure must apply the electron rescale (or
  produce central-selection templates — none exist, ledger §11.4) and close or explicitly carry the V0 gap.
- **M-3 (concern):** The 13-component budget parent is pre-S2' provenance (canonical is now the 15-component S2'
  budget, `total_with_s2prime15` medians 8.1864%/8.4079%, adding `detint_removal_rel` + `fakecol_mcstat_rel`). The
  correct re-based budget is 15 + re-derived bgsub components (→19), with the electron template's variance and
  RG/DJ spread folded into `bgsub_stat_rel`/`bgsub_model_rel`.
- **M-4 (documentation hygiene, follow-up):** The S2' promotion record `docs/promotions/detints2prime_promotion_2026-10-10.md`
  is cited by at least five status documents (RUN_STATUS.md, current-release.md, alpha0p2-background.md,
  radiative-corrections.md, STATE.md) but **does not exist** in `docs/promotions/` (contains only two other files).
  The substance survives in RUN_STATUS/STATE, but the standalone record must be restored.
- **M-5 (documentation hygiene, follow-up):** RUN_SHEET.md's status block is stale ("OFFICIAL production … NOT
  launched") although official artifacts exist (clusters 5486182/5486183).

## Non-Blocking Checks Confirmed (these materially narrow the root cause)

- **Chain machinery is spec-faithful and internally sound:** pre-unfold stage; correct ordering
  (`data_fine = data_sub`, then `d_sig = data_sub×(1−f_i)`); `f_i` sidecar byte-identical to the repro; E1b
  exactly-once Poisson (`w2 = data_sub`, asserted equal; template variance carried once, in `bgsub_stat_rel` — no
  double-count); re-unfold config unchanged (IBU k=4, L=351,000 nb⁻¹, same 980→490 mapping); C_rad bitwise
  (max dev 0.0); RU-3 −0.0283%/−0.0271% vs the dilution anchors; CP-2 addback bitwise; toy lineage bitwise.
  The defect is confined to the set definition and the basis — not the implementation.
- **Budget 13→17 internals correct:** the 13 frozen components (ibu_stat, ibu_model, ibu_reg, hfs_jes, hfs_rces,
  hfs_theta, elec_energy, elec_theta, lumi, tracking_eff_m0p5, phi_h_gauss, c_rad_gen, c_rad_stat) carry no
  bgsub/e/V0 heritage — no double-count; 35 parent columns byte-identical (17,150 cells, 0 mismatches); quadrature
  closure max dev 0.0; totals17 5.7159%/6.1586% vs totals13 5.7093%/6.1523%; `bgsub_fakefrac_rel` correctly
  propagates r_min = 0.9816719 (|1−r| = 0.0183281) with the integrated f_release reproducing RU-2 to ≤1e-12; spec
  median anchors explained as drafting-stage product-of-medians (`spec_anchor_note`).
- **CP-1 sparse-bin exceptions adjudicated: ACCEPTED.** 11 bins >1% (pos 34, 69, 109, 179, 205; neg 170, 206, 209,
  240, 243, 244), all sparse-declared a priori (toy_stat_rel ≥ 0.05); 5 exceed the 2% cap (pos 34: 6.111%, pos 179:
  2.228%, neg 170: 2.286%, neg 240: 2.401%, neg 244: 2.476%); all lie below their own statistical resolution and are
  carried full-size in `bgsub_stage_rel`. Bins 109/179/240 are the empty-D migration-inflow bins. Legitimate and
  conservative; not a blocker.
- **Empty-bin convention preserved:** SA-2 deviation from spec anchors equals exactly the guarded content
  (0.248 tracks, 6.6e-8 rel, within 1e-6), consistent with the spec's literal formula and SA-1's zero-negative rule.
- **Set-B arithmetic cross-check:** the spec's Set-B 7.9858% reproduces the ledger four-template sum exactly
  (308,655.8946 = 7.9467% central; 0.0798577141 alpha-D), and 308,655.89 − 281,250.09 = 27,405.80 = the det.int.
  class — confirming the spec's arithmetic is faithful and isolating the error to the set *choice*.

## C-1 reconciliation (mandated adjudication)

The publication-grade review C-1/L1's framing of the 10.7% fake fraction as *unsubtracted* background is adjudicated
**wrong in mechanism, right in existence**. The 10.7141%/10.4019% fake fraction IS subtracted in the central chain
(`d_sig = data×(1−f_i)`), reproduced exactly by the normalization verification and the candidate's RU-2. The genuine
unsubtracted fiducial residual under the central response is **7.2411%** (e+V0+lowQ2), not 10.7% — and not 3.40%. The
bgsub spec correctly rebutted C-1's number but then over-corrected into F-1: by declaring electrons "already removed,"
Set R subtracts only 47% of the true residual. The candidate therefore does not resolve C-1's underlying finding; it
relocates it. C-1's "~0.3 pp charge-symmetry" statement is verified accurate (0.3122 pp).

## P-ALPHA disposition (mandated adjudication)

**PASS WITH CONDITIONS as scoped by the gate — and the conditions become mandatory under the cure.** The published
chain uses the central response, not the alpha0p2 response, so P-ALPHA's original scope (alpha0p2
response-consistency validation) is not gate-blocking; the alpha0p2 *template* heritage is bounded for set R
(denominator swap −0.011 pp, carried in `bgsub_stage_rel`). Conditions: (i) the electron vector added under the cure
must apply the quantified cone-survival rescale (≈4.1441% no-cone PROV) or be produced under the central selection
(no such production exists); (ii) the V0/low-Q2 cone-survival gap (no counters; unbounded) must be closed or carried
as an explicit uncertainty component. Under the F-1/F-2 rebuild both conditions are automatically in scope.

## Conditions for a promotable re-build (all mandatory)

1. Extend the subtraction set to the ledger-prescribed **e + V0 + lowQ2** (B_R' = B(e)+B(V0)+B(lowQ2); e∩V0 overlap
   per user decision 2026-10-08 [ignored, 0.0364% accepted] or ledger §7.4 overlap-once — record the choice).
2. Re-base onto the canonical **S2' variant** (response/`f_i`/prior per `bbb_ibu_detints2prime_..._2026-10-09`) —
   this is exactly the S2' spec §3 "full future chain" (`d' = d − B(e) − B(V0) − B(lowQ2)`, then unfold with the
   variant response and variant `f_i`) — or obtain an explicit user re-adjudication to publish on the pre-S2' basis
   with observable-basis flagging (not recommended).
3. Apply the electron cone-survival rescale (or produce no-cone templates); close or explicitly carry the V0
   cone-survival gap.
4. Re-derive anchors and the budget on the new basis (15 + re-derived bgsub → 19 components, electron template
   stat/model terms included); re-run the 24-item gate **plus** a new set-composition item re-deriving §2.1 against
   the 2026-10-07 ledger.
5. Hygiene (non-blocking): restore the missing `detints2prime_promotion_2026-10-10.md` record; fix the RUN_SHEET
   status block.

## Recommended Next Check

Rebuild the chain as the S2' "full future chain": B(e)+B(V0)+B(lowQ2) vectors (electron cone-survival rescale applied),
pre-unfold subtraction, S2' variant response + variant `f_i` + coherent prior — the single build that discharges both
fatals. Decisive first sub-check: the integrated subtraction must rise from 3.41% to ≈7.2% (cone-applied templates;
≈7.5% with the e rescale), and the integrated Born totals must shift by an additional ≈ −3.7% to −4.1% relative
versus the present candidate, with the h+/h− asymmetry of that shift ≈ 0.2 pp.

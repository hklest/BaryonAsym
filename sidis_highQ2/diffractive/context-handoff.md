# Context Handoff: H1 Diffractive High-Q² Charged-SIDIS Extension (v6dif)

**Date:** 2026-10-20
**Status:** **RESTARTED 2026-10-27** (see "Current Status — RESTARTED"; PARKED record kept below as history)

This workspace is the diffractive (LRG) extension of the high-Q² inclusive charged-hadron
SIDIS analysis (`sidis_highQ2/`, Q² > 150 GeV²). It is self-contained: never mix code,
outputs, or results with other analyses. The parent inclusive analysis is unaffected by
every diffractive-side failure found to date and proceeds to publication (see
`../context-handoff.md` and `../STATE.md`).

## Goal

Diffractive charged-hadron cross sections on the DECIDED 45-bin/charge grid
(3 D-cells D0/D1/D2 × 3 z × 5 pT; `docs/binning/diffractive_binning_baseline_2026-10-17.md`)
with LRG selection (x_IP < 0.03, M_Y < 1.6 GeV, |t| < 1 GeV², M_X > 2 GeV), ND leakage
constrained per plan §7.1, forward-detector validation per §7.2, and unfolding through
the parent IBU machinery (task chain T0–T9, `H1DiffractiveHighQ2SIDISPlan.md`).

## Current Status — RESTARTED (2026-10-27)

- Restarted from scratch on user decision, 2026-10-27, under the program plan
  `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`.
- User decisions: (1) the Plug probe runs first, under kill gate G1; (2) observables are
  x_F and p_T*² in the X rest frame, in M_X bins, Q² > 150 GeV², per-event-normalised;
  (3) all prior decisions are re-evaluated and none is binding.
- Research input: `docs/research/revival_assessment_2026-10-27.md`.
- Output root convention: `sidis_highQ2/diffractive/{docs,data,scripts,condor,plots}/restart_2026-10-27/<wp>/`.
- 2026-10-28 WP-A6 (G2, G1″(c)): BB-lite η04 < 3.0 refit, odd split, RULING PASS with masks — M_X [4, 10) retained (nd4cat f_ND 0.048 ± 0.040), [10, 20) and [20, 40] masked_G2; record `docs/restart_2026-10-27/wp-a6/g2_verdict.md`, table `data/restart_2026-10-27/wp-a6/f_nd_per_mx.csv`, mask `data/restart_2026-10-27/wp-a6/purity/`.
- The PARKED status below is kept as history of the 2026-10-20 end state.

## Current Status — PARKED (2026-10-20)

- **Verdict:** no unfolded diffractive SIDIS result is defensible from the current
  evidence register under any mask scenario. Unfolding ruled NOT PERMISSIBLE under the
  plan's own gates (§§7.1/7.2/8.2/10); systematic-viability stop condition TRIGGERED;
  final path adjudication verdict FATAL (`docs/t4/final_path_adjudication_2026-10-20.md`).
- **Leakage-campaign end state (user-accepted):** ENVELOPE-ONLY — the ±100% correlated
  leakage envelope stands, no central value, no nominal ND shape. Neither RAPGAP31-7071
  nor DJANGOH14-7091 is a defensible nominal (corrected symmetric spread ~1.31×, RAPGAP
  higher; both fail template-statistics-aware GOF, toy p ≤ 0.0033 everywhere).
- **Why:** every pre-registered leakage-fit design failed at shape level on 79,008
  cut-removed full-range events (§7.1 two-free-template fine D/ndf 5925.8/358 and
  6457.8/358; DF-SYS-01 nuisance refit fails and fails closed in D2; five-scale
  per-component fit fails with degenerate templates; photoproduction excluded at a 1.75%
  E-pz ceiling). Full audit chain and numbers: **the internal feasibility note
  `docs/diffractive_feasibility_note_2026-10-20.md`** (start there).
- **Masks:** NOT ratified. Binding pre-registered masks h⁺ 16 / h⁻ 14 stand unchanged;
  scenario B (+ full D2) is the recorded leading candidate for any revival, to be
  re-derived from GOF-passing inputs (`docs/t4/t7_mask_review_proposal_2026-10-20.md`).
- **Stopped:** task execution toward T6–T9 — no unfolding, no further leakage-fit
  designs, no diffractive systematics production. The evidence register is CLOSED. No
  response-improvement campaign funded. Revival preconditions: adjudication §3(ii)
  items 1–5 + §7 missing-evidence list (reproduced in the feasibility note §5).
- **Period-matched generator-pair record closure** (7094-vs-7091 ep0607; 7072-vs-7071
  em06, gendump-style): **DELIVERED 2026-10-20** — record
  `docs/mc/period_matched_pair_record_2026-10-20.md`; feasibility note §7 filled;
  CHANGELOG entry appended. Truth-level rates (per read event): 7071 1.010e-4,
  7094 0.970e-4, 7072 2.393e-4, 7091 2.499e-4; period-matched DJANGOH/RAPGAP ratios
  2.58 (ep0607) / 2.37 (em06) vs the un-matched 2.48x; the particle-vs-detector sign
  flip survives period matching (detector level: RAPGAP 1.49x per tree entry, 1.307x
  corrected per-nb) — ENVELOPE-ONLY reinforced; no gate or verdict changes.

## Revival Assets — still-valid operational facts (do not rebuild)

| Asset | Location / value |
|---|---|
| v6dif data trees (true periods) | `output_v6dif/data/{ep0304,em0405,em06,ep0607}/` (e.g. em06 312 files) |
| v6dif MC trees | `output_v6dif/mc/{6671,6677,7071,7091,9012,9024,9033,9155,9157,9158,…}` |
| Certified forward-detector run list | `data/t2_detector/certified_runlist_2026-10-19/` (subtractive-only; 20,788 runs screened, 18,052 ACCEPT; certified lumi **317,212.803710 nb⁻¹** exact) |
| Exposure convention | certified per-period luminosity weighting (equal-period ×4 rejected as a 4.43× double-count); `docs/mc/exposure_convention_ruling_2026-10-19.md` |
| Composite scale table v3 (SIGNED, binding) | `docs/mc/composite_scale_table_v3_2026-10-20.csv`, sha256 `b7609559db490e6d3559feb3e7463e09dc30ddba35ff6ce9b00457a1e97b209f`; v2 retained as provenance; 7071 row corrected (scale_to_data 0.23166084324546941; defect factor 6.027730001; ten-row audit 9 PASS / 7071-only FAIL) |
| Composite response v3 | `data/t4/composite_response_v3_2026-10-20/` (only the 7071 ND template re-keyed h⁺ 24,221.95 / h⁻ 22,130.82; signal/reggeon/7091/priors/diss-fraction exactly v2-invariant; v2 at `data/t4/composite_response_v2_2026-10-19/` superseded, retained) |
| §7.1 cut-removed full-range payload | `data/t4/datamc_cutremoved_eta_7p1_2026-10-20/assembled_v3/cutremoved_fullrange_eta_templates_7p1.json` (G1–G5 PASS; 79,008 events, D-cells 44,583/23,947/10,404; sha256 `341e5a1e…`; v2-bound `assembled/` retained as provenance) |
| Leakage-fit / datamc products | `data/t4/leakage_fit_2026-10-19/` (v3 addendum, dfsys01_refit), `data/t4/datamc_comparison_2026-10-19/` (ruled_v3), `data/t4/mask_review_2026-10-20/` |
| Systematics ledger (DF-SYS-01/02, registered UNEVALUATED) | `docs/systematics/diffractive_systematics_ledger.md` + `systematics_diffractive.json`; total diffractive systematic NOT quotable; five-column assembly policy binding for any T8 |
| Mixture nominal (frozen) | enforced flat PDISS-uds ×5.3611, ±7.2% dissociation/elastic ratio-normalisation systematic; 83:17 elastic:dissociative (17.0001%; 16.97% under the ruled exposure convention) |
| Grid + masks (DECIDED, binding) | `docs/binning/diffractive_binning_baseline_2026-10-17.md` (45 gen / 90 reco bins per charge; h⁺ 16 / h⁻ 14 masked; 11 structural empties per charge mixture-invariant) |

## Known Issues / Traps

- **Never quote the 4.61× ND-generator spread or 19.76/101.94 events** — superseded by
  the 7071 scale-row re-ruling (6.027730× under-normalisation; corrected ~1.31×, RAPGAP
  higher). Never compare b_ND(7071) with b_ND(7091) on the old v2 basis (the fallacy
  re-enters through the fit).
- The ±100% correlated leakage envelope is NOT a coverage claim on the rejected shape
  and has no admissible central value; it never constrained a passing fit.
- FI-1 guardrail: a "fixed" template base must be verified against the binding layout —
  the 2026-10-19 cluster likelihood fixed Reggeon-only (120.39, 1.8%) where signal+
  reggeon was binding, mislabelling signal as leakage (87.079%/79.499%).
- DF-SYS-01 (ODS η_max, mixed-sign +32% D0 / −27% D2, both branches) and DF-SYS-02
  (FMD nFmu12, shape FAIL under every cocktail 0.366–0.459) are UNEVALUATED, never
  zero; two-way interdiction against the leakage envelope is binding.
- **B5 CLOSED 2026-10-21 (operations sign-off executed):** the certified run list is
  **FROZEN** — B5 sign-off delivered (`docs/t2/b5_operations_signoff_2026-10-21.md`,
  9/9 checks PASS; machine record
  `data/t2_detector/certified_runlist_2026-10-19/b5_operations_signoff_2026-10-21.json`;
  certified lumi 317,212.803710 nb⁻¹ recomputed exact; subtractive-only and rescan
  no-change md5-verified). Freeze remains CONDITIONAL on the list as delivered
  2026-10-19: any later verdict change requires recomputing the frozen lumi table.
  (Supersedes the former "B5 open formality / NOT frozen" wording.)
- MC noise overlay is present: random-trigger data events are overlaid on every v6dif MC
  sample (`RT_NOISE=Y` for every component ReqID; FMD, FTS/PRT, PLUG and other subdetectors
  covered), verified at production-configuration level in
  `docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md`. The quantitative fidelity
  of the overlaid noise rates versus data is not yet verified. (Supersedes the former "no
  random-trigger stream / policy blocker B3" wording.)

## Priority Actions

1. **(Passive.)** Keep the extension parked; do not run unfolding, leakage fits, or
   diffractive systematics production. The evidence register is closed.
2. **(DONE 2026-10-20.)** Period-matched generator-pair comparison record-closed
   (7094-vs-7091 ep0607; 7072-vs-7071 em06); feasibility note §7 filled; CHANGELOG
   entry appended; record at `docs/mc/period_matched_pair_record_2026-10-20.md`.
3. **(DONE 2026-10-21.)** B5 operations sign-off EXECUTED — certified run list FROZEN
   (operations); record `docs/t2/b5_operations_signoff_2026-10-21.md` (9/9 PASS).
   Consumer caveats updated in the localization verdict, the feasibility note, and the
   certification policy §10.4.
4. **(Only on an explicit revival decision.)** Re-open via the adjudication §3(ii)
   preconditions — localization diagnostics first (§10.1 N−1 distributions,
   truth-M_Y-resolved cluster-eta tables), attribution to a named simulable defect,
   noise-stream resolution, plan amendment + sign-off, budget realism. Any mask change
   requires fresh user ratification from GOF-passing inputs.
5. **(Parent.)** Parent inclusive analysis proceeds to publication independently; see
   `../context-handoff.md` and `../STATE.md`.

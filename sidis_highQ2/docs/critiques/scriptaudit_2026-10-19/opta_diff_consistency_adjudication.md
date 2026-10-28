# OPT-A rebuild impact diff — physics-consistency adjudication (2026-10-20)

**Auditor:** physics-correctness critic, read-only. No analysis script modified. Skill `sidis-hq2-unfolding` load failed (recorded precedent); adjudication on file evidence.

**VERDICT: CONSISTENT** — the OPT-A per-cell IBU/Born shift pattern is physically consistent with the N1-removal (Q2esGen body-cell harmonization) mechanism at all three layers: the intended mechanism is explicit (B.1/C.2), the implementation change-scope is audited by primary artifacts, and the resulting per-cell numbers are traceably consistent with the prediction. **ADOPT-WITH-CONDITIONS** as the response-rebuild candidate; it is NOT yet a quotable release.

**Material reviewed** — Observable: per-cell IBU k=4 raw and Born (C_rad-applied) cross-section shifts, OPT-A (2026-10-19) vs the 2026-10-17 clean release. Channel: high-Q2 charged-hadron SIDIS, v6/z005, Q2>150 GeV^2, cone+S2'+Set-R' bgsub basis, h+/h-, 245 published gen bins/charge, lumi 351,000 nb^-1. Evidence: the full 490-row per-cell/per-charge delta CSV (every row read; `rel_delta_raw == rel_delta_born` verified to machine precision in every row); `optA_crad_diff_summary.json` + run log; merge manifest + `official_reunfold/run_record_sub.json` (change scope, audit counters, disabled/kept pins); 3-shard mismatch forensics; the single-slice straddle quantification (rapgap31_7071_em06_0060); audit docs `crad_truth_contract_adjudication.md` (B.1/C.2), `loop3_treebuilder.md`, `loop3_systematics.md` (C1), `CROSS_AREA_CONSOLIDATED_VERDICT.md`; STATE.md anchors (clean-release raw IBU h+ 10.508811 / h- 10.217503 nb; Born 11.996558 / 11.663956 nb; releasebin C_rad tables book by (Q2esGen, x_es_gen)).

## Decomposition identity (needed by findings 1-3)

The diff applies the SAME C_rad releasebin table (sha `cf810be4...`, unchanged) to both sides, so `rel_delta_born == rel_delta_raw` per bin. The observed shift is therefore **A - B**, where A = net Born->radiative re-binning straddle flux per cell (undiluted; net receivers +, net donors -) and B = the clean release's efficiency-split bias (diluted, f_matched-scaled; same per-cell sign as A per the C.2 ruling — empirically supported: every receiver-cell median sits below its straddle rate, as same-sign partial self-cancellation requires). In the released clean Born values the two components had OPPOSITE signs (total bias = B - A); removing both gives observed = A - B. Consequences: observed signs track the net straddle flux A, and observed magnitudes are bounded by the straddle rate itself, not by f_matched x straddle.

## Finding 1 — Sign/magnitude pattern vs the two-component prediction: CONSISTENT

Per-cell medians (h+/h-) vs prediction: S1_x0 `+1.9%/+1.9%` (all positive) — net receiver (rate 2.76%); S2_x0 `+2.5%/+2.6%` (all positive, max populated-bin +3.4-3.6%) — net receiver (8.74%); S3_x0 `+1.35%/+1.4%` — net receiver (6.21%); S0_x0 `-0.33%/-0.32%` — small net donor; S0_x1 `-1.4%/-1.3%` bulk all-negative — net donor — the slice table's "S0 = 0%" is the RECEIVER-side (prior-cell) rate only; S0 cells still DONATE (their out-flux is exactly what appears as the S1_x0 2.76% / S1_x1 2.34% receiver rates), so "S0 expected clean" is a misreading and bulk-negative S0_x1 is the expected donor-side A-flux; S1_x1 `-0.1%/-0.02%` (mixed, small) — near-balanced; S2_x1 `+0.14%/+0.25%` — near-balanced slight net receiver (1.84%). h+/h- symmetric throughout, as required for a charge-blind gen-Q2 operand change. No sign violation, no unexplained spike or oscillation in any populated bin.

## Finding 2 — Magnitude bound: PASSES, with a bound-semantics correction

The stated bound "f_matched x straddle rate (slice 1.84-8.74%)" applies to component B only. The observed diff (A - B) is bounded by the straddle rate itself (A is undiluted). Against that bound: the largest well-populated shift is ~ +3.4-3.6% (S2_x0), inside its 8.74% slice rate; S1_x0 (+1.9% vs 2.76%) and S3_x0 (+1.35-1.4% vs 6.21%) likewise pass; all donor-side bulk magnitudes sit below their out-flux scale. Caveat carried, not a violation: the rates come from ONE MC slice (rapgap31_7071_em06_0060; 100% upward straddle; Q2 strata [150,300,700,1500,20000]) — indicative priors, not full-sample per-cell measurements. No populated bin exceeds its slice-indicative bound.

## Finding 3 — S0_x1 pos b39 -4.83%: small-denominator corner-bin amplification; NOT a mechanism anomaly

Hypothesis adjudication: (a) coherent N1-removal failure in S0_x1 — REJECTED: the S0_x1 bulk is the expected donor flux (Finding 1) and b39 is isolated. (b) Small-denominator statistical amplification — SUPPORTED: b39 is a low-z/high-pT corner bin with sparse truth content. (c) IBU k=4 prior redistribution in a sparse corner — SUPPORTED (same family); decisive discriminator is charge behavior: pos -4.83% vs neg +2.85% in the SAME bin (neg b69 -3.99% nearby) — a coherent efficiency/binning change is charge-symmetric, so opposite-sign excursions between charges are the signature of noise-driven prior redistribution, not a systematic efficiency change. (d) Implementation artifact — REJECTED by the audit layer: 0 Q2esGen lookup misses; 3/58,768,556 body-vs-pool mismatches (rapgap31; djangoh14: 0/69,695,815), all certified benign pT-edge bit flips (|dpT| <= 1.7e-7 GeV at the 0.5 GeV edge, cell and iz agreeing); Q2esGen event-stable (0 variation rows). Follow-up only. Same adjudication covers pos b109 (S1_x1, -6.95%) and pos b179 (S2_x1, -35.5%; clean value 7.5e-5 nb -> remains positive ~4.8e-5 nb) — near-empty, zero-bgsub-budget corner bins where relative shifts are unconstrained and no physics claim lives.

## Finding 4 — The 56 budget-exceeding cells: component-level statement only; NOT a total-budget failure; treatment = regenerate the budget on the OPT-A nominal

56 bins exceed the per-bin bgsub component budget; max ratio 2.58 (neg b168, S2_x0: +2.67% vs 1.04%). ZERO bins exceed the 19-comp total (max ratio 0.86) or the 21-comp env100 total (max 0.70; worst populated bin pos b152 S2_x0 +3.41% vs 3.94%/4.88%). C1 factored: the env100 detint double-count INFLATES the 21-comp total by <= ~171 ppm and never deflates, so the C1-corrected total shrinks by <=1.71e-4 relative — the max total-ratio moves 0.70 -> ~0.7001; "0 exceed totals" is robust to the C1 fix in the conservative direction. Physics framing: the N1-removal shift is a one-time CENTRAL-VALUE correction (removal of a demonstrated binning bias), not an uncertainty to fold in; what needs a systematic home is the RESIDUAL model dependence (MC-vs-data straddle-rate mismatch), small and bounded by the same straddle logic. The comparison is also transitional screening: the compared budget (env100, sha `ba9ede68...`) was built on clean-release central values; after adoption the affected components (ibu_stat 1000-toy, fakecol Jacobian, detint legs, five detector legs) are re-derived on the OPT-A nominal anyway, and the C1 fix must land in that regeneration. Not adoption-blocking.

## Finding 5 — Integrated conservation: HOLDS

Raw integrated +0.0240% (h+) / +0.0231% (h-); Born +0.0074% / +0.0064%. A pure re-binning redistribution must approximately conserve the total; O(2e-4) residuals are consistent with IBU fixed-point re-regularization plus the body/pool boundary population gaining partners (`n_opta_body_no_pool_partner` = 41,390 rapgap / 49,878 djangoh). No normalization leak (a luminosity-like or fiducial-side leak would appear at percent level); both charges agree with each other, charge-symmetric as required. The Born residual being smaller than raw is consistent with the C_rad-convention component partially cancelling at Born level. No red flag.

## Finding 6 — Verdict and adoption

Mechanism verdict: **CONSISTENT** — sign pattern, magnitudes, charge symmetry, integrated near-conservation, and the audited change scope (only the matched-mirror BODY gen-Q2 cell operand changed: rad-safe Q2_es_gen -> calculator Q2esGen; gen fiducial stays on rad-safe Q2_es_gen/Y_es_gen; z, pT, x, cone, S2', matching, and the entire reco side unchanged; RU-1 bit-identity intentionally broken) all agree with N1-removal, and the harmonization target convention is externally anchored (STATE.md: releasebin C_rad tables book by (Q2esGen, x_es_gen)).

**Adopt recommendation: ADOPT-WITH-CONDITIONS** — adopt as the OPT-A response/central-value candidate; do NOT promote to a quotable release until:
1. Budget regeneration on the OPT-A nominal, including the per-bin treatment decision for the 56 component-exceeding cells and a systematic home for the straddle-rate residual (Finding 4).
2. The pre-existing cross-area quoting blockers land: env100 C1 detint double-count fix + M1 builder reconstruction (loop3_systematics C.3), N2 HFS-Empz channel, M2 z-shift systematic (B.2 / CROSS_AREA_CONSOLIDATED_VERDICT.md).
3. OPT-A-specific gates close: RU-2 removal-window re-derivation for the OPT-A f_i (currently disabled; measurement recorded: integrated removal 0.11280/0.10907, in the old window informationally) and the two-stage SHA re-pin (spec response shas TO-BE-ESTABLISHED per `opta_disabled_pins`).
4. Registry discipline: RUNLIST.md / SEVEN_CATEGORY_SCRIPT_INDEX.md override entries for the OPT-A campaign (the n1 lesson — do not repeat the env100 freeze's registry bypass).

## Minimal open items

- Full-sample per-cell straddle counters (closes the single-slice caveat; quantitatively confirms the b39/b109/b179 amplification).
- Per-cell f_matched (separates A from B explicitly; currently only B's existence, dilution, and sign alignment are established).
- Per-cell breakdown of the 1,357,250 (rapgap31) / 1,574,230 (djangoh14) body-cell changes.

## Missing Evidence

1. Full-sample per-cell straddle rates — only the single MC slice exists; needed to close Finding 2's bound quantitatively and Finding 3's amplification claim per cell.
2. Per-cell f_matched values — needed to quantify component B separately.
3. Per-cell changed-count breakdown of `n_opta_body_cell_changed_vs_radsafe` (merge manifest records totals only).
4. (Context note, not verdict-blocking: the tree-builder code diff line itself — relied on here via loop3_treebuilder.md's code-level review plus the primary-artifact run-record operand description and audit counters.)

## Recommended Next Check

A full-sample (all-shard, Condor) per-cell straddle-counter + f_matched pass over the OPT-A campaign, mapped onto the 490 published bins — the single check that closes the quantitative bound (Finding 2) and the corner-bin amplification (Finding 3) simultaneously; hand the resulting table to the sidis supervisor alongside the budget-regeneration decision (Finding 4, adoption condition 1).

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic (OPT-A diff consistency), read-only; no analysis script was modified.
- Report persisted by the delegating supervisor session (critic toolset read-only), 2026-10-20.
- Input artifacts: output_v6/studies/optA_response_rebuild_2026-10-19/crad_diff/ (per-cell delta CSV, summary JSON), stage2_setrprime_optA_2026-10-19/official_reunfold/run_record_sub.json, merge manifest + forensics (sha 9955dc8d...6e247f5).

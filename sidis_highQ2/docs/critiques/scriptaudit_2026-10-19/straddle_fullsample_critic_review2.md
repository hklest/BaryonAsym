# CRITIC REVIEW 2 of 2 — Final Adjudication: OPT-A Baseline Promotion Gate (straddle + f_matched, full sample), 2026-10-20

Auditor: physics-correctness critic, read-only (independent verification pass; retry after an empty first attempt). No analysis script modified. All numbers recomputed independently from the artifacts; none taken from Review 1.

**Verdict: PASS-WITH-CONDITIONS** (critic verdict on the study physics: SOUND — the flag set is complete, all bounds hold, closure holds by construction, and observed shifts trace exactly to the delta CSV; the conditions are six corrections to the Critic Review 1 record, none blocking, none changing the physics conclusion or the gate outcome).

## Per-Claim Adjudication

| # | Claim | Result |
|---|-------|--------|
| 1 | Flagged set complete (48; distribution; no violation outside) | VERIFIED — conclusion holds; Review 1's decisive argument REFUTED (C1) |
| 2 | Named spot bins end-to-end | VERIFIED — incl. new field-level delta-CSV trace |
| 3 | A-B closure (B = A - obs positive) | VERIFIED structurally; quantitative characterizations CORRECTED (C3, C4) |
| 4 | Bounds and quoted maxima | Bounds VERIFIED everywhere; 3 of 4 quoted maxima CORRECTED (C2) |
| 5 | Corner bins b39/b109/b179 | VERIFIED verbatim |
| 6 | Slice reconciliation | VERIFIED (690/690; 1.9381%/1.5727%) |
| 7 | Manifest headline rates | VERIFIED exact arithmetic both legs |
| 8 | Coverage (masking fix, mismatches, no-pool-partner, dj sensitivity) | 8a/8b/8c VERIFIED; 8d REFUTED as stated (C6) |

**Claim 1 — completeness.** Grep for flagged rows returns exactly 48 (26 pos, 22 neg); distribution S2_x1 31 / S1_x1 13 / S1_x0 2 / S3_x0 1 / S0_x1 1 exact; all flagged rows populated_rg = populated_dj = 1; raw == born flags coincident on all 490 rows (rel_delta_raw == rel_delta_born to machine precision). Mechanical completeness proof reproduced: grep for unflagged rows with ratio >= 1 on the raw leg returns ZERO matches -> all 442 unflagged bins satisfy |obs| <= |A_rg| by construction, and B = A - obs is same-sign as A on every unflagged bin.

**Claim 2 — spot bins.** All match: pos b70 +1.479/3.804, b152 +3.406/5.126, b158 +3.847/5.301, b106 (ratio 108, gross 5.35%), b189 (+0.498%, dj -0.206%), b194 +0.898%, b0 (obs -0.22693%, body -65,567.29/15,832,930.5 = -0.414%), b220 +1.255/2.385; neg b70-73 +1.443/+1.879/+1.971/+1.615%. Delta-CSV trace: pos,158 rel_delta_raw = 0.0384685379125517; pos,189 = 0.004983742334442509; neg,39 (global bin 284 = 245+39) = 0.02851700395647324; pos,0 = -0.0022693062179537414 — all matching the join to machine precision; aggregator hard cell_label check guarantees the join key.

**Claim 3 — closure.** B = A - obs is positive and same-sign as A on all unflagged bins — holds by construction from the Claim-1 mechanical result. Quantitative claims do not hold (C3, C4). Load-bearing use — dilution on flagged near-balanced cells, e.g. pos b189 |obs| = 0.498% << 0.452 x 13.34% ~ 6.0% — remains valid.

**Claim 4 — bounds/maxima.** No bound violation anywhere outside the flagged set; within it the flag fires as designed (max ratio 204.98, neg b132, near-zero A). Corrected maxima: pos S1_x0 true max b83 +2.8610% (not b78 +2.7797%); pos S3_x0 true max b217 +2.1712% (not b223 +2.1137%); neg S3_x0 max b236 +2.26% (ratio 0.909); pos S2_x0 max b158 +3.8469% (ratio 0.726). The overall largest populated shift is neg b159 +4.2051% vs A +4.9191% (ratio 0.855, mc 7,052), exceeding the quoted "+3.85% largest well-populated". All values remain within bounds.

**Claim 5 — corners.** Verified verbatim: pos b39 obs -4.829% vs A -6.537% (ratio 0.739, dj -7.167%, f 0.0206, mc 4,754, outflux 397.4466/6,079.97 = 6.54%); b109 ratio 1.8516 (f = 97/3,533); b179 ratio 8.7972 (f 0.035196, mc 1,790); corner ratio range 1.063-1.819; charge-opposite pairs -6.95/+1.99 and -35.5/+4.36.

**Claim 6 — slice.** Smoke 690/690 bins, 1.9381% (rg) / 1.5727% (dj) — matches the verdict file.

**Claim 7 — manifest.** Exact: rg 2,292,226/129,231,130 = 1.7738%; dj 2,677,248/154,083,280 = 1.7371%; n_straddle_rows_ingrid + n_safe_out = n_straddle_rows exact both legs (2,247,882 + 44,344; 2,624,307 + 52,941); n_changed 1,357,250/1,574,230; all ten reconciliation entries (5 counters x 2 legs) pass:true; n_shards 73/101 = the 174 shard JSONs scanned.

**Claim 8 — coverage.**
- 8a masking fix: verified in code (L235-238 FIX masking LHS to both; hard SystemExit on any z/pt flip in pool; n_safe_out = int((~_both).sum()) L244; pool (NB_G, NB_G+1) with OUT_IDX=245; hard assert pool row-sum == builder mc_count_per_gen). Shard scan: 173/174 shards n_safe_out > 0 (only rapgap31_0012 = 0) — consistent with the 0.034% global rate, no anomaly.
- 8b mismatch rows: rg-leg n_opta_body_vs_pool_mismatch = 3 of 58,768,556 checked (5.1e-8), dj leg 0 of 69,695,815; audit-counted only, leg-vs-manifest reconciliation pass:true both legs. Benign confirmed.
- 8c no-pool-partner: rg 41,390 (0.0704% of matched), dj 49,878 (0.0716%); excluded from the pool by construction — bounded effect.
- 8d dj-vs-rg flag sensitivity: REFUTED as stated. Recomputed on all 48 rows: 14/48 would NOT flag under dj rates — pos b106 (0.909), b177 (0.999), b183 (0.915), b186 (0.882), b188 (0.558), b191 (0.945), b197 (0.718), b240 (0.953); neg b106 (0.929), b126 (0.573), b181 (0.857), b192 (0.720), b193 (0.570), b194 (0.822). The rg-leg flag basis (nominal response) remains valid, and cell-level rg/dj agreement stands, but the flag set is not generator-invariant and sub-bin net rates in near-balanced cells disagree at O(1) relative scale.
- Also verified: "11 flagged cells with |obs| > 1%, all sparse except neg b189" — exact list pos b100/b109/b138/b179/b209/b240, neg b69/b102/b179/b189/b207; neg b189 mc 8,884, next largest pos b109 mc 3,533.

## Major Concerns (all record-level; none changes the physics conclusion)

- C1 — Review 1's "decisive completeness argument" is false and must be removed. "sign(obs) != sign(A) mathematically implies |obs| > |A|" is not a theorem, and "the other 442 bins are same-sign" is empirically false: unflagged sign-flipped receiver bins exist — pos b174 (obs -0.1776% vs A +3.1595%, mc 286, ratio 0.056), pos b244 (-0.3897% vs +2.3166%, mc 518, ratio 0.168), neg b170 (0.0015), neg b235 (0.050), neg b240 (0.408), neg b244 (0.031), plus neg b39 (+2.852% vs -7.251%) and neg b109 (+1.990% vs -4.304%) which Review 1 itself quotes — an internal contradiction. Why it matters: a false proof in the promotion record would be reused as a scanning theorem elsewhere. The completeness conclusion itself survives via the mechanical grep (zero unflagged rows with ratio >= 1), independently reproduced. Follow-up: record correction only.
- C2 — wrong maxima quoted. pos S1_x0 max is b83 +2.8610% (not b78); pos S3_x0 max is b217 +2.1712% (not b223); the largest populated shift overall is neg b159 +4.2051% (ratio 0.855, mc 7,052), not pos b158 +3.85%. All values remain within bounds. Follow-up: record correction only.
- C3 — "B at 0.3-0.7 x |A|" is false. B/|A| on populated receiver bins spans ~0.005 (neg b165) to ~1.06 (pos b174); sampled: 0.005, 0.136 (neg b84), 0.145 (neg b159), 0.234 (pos b79), 0.824 (pos b74), 1.056 (pos b174). The structural statement (B positive, same-sign as A) is what actually holds. Follow-up: record correction only.
- C4 — "B ~ f_matched x GROSS" is not a per-bin relation. B/(f x gross) spans ~0.004-1.2 (e.g. pos b158 B = 1.454% vs f x gross = 5.17%). Valid only as an order-of-magnitude dilution statement on flagged near-balanced cells. Follow-up: record correction only.
- C5 — f_matched range understated/misstated. S3_x0 receiver bulk is ~0.30-0.51 (not 0.41-0.77); ip=4 edges (b74/b144/b214) ~0.20-0.25. The true range implies MORE severe dilution in parts of S3_x0 than the record suggests. Follow-up: record correction only.
- C6 — flag set is not generator-robust. 14/48 flagged rows would not flag under dj rates (list above). Any future dj-based cross-check would produce a different flagged set; the record must not claim invariance. The rg-nominal decision is unaffected. Follow-up: record correction + carry the O(1) sub-bin net-rate disagreement between legs as a known limitation.

## Non-Blocking Checks Confirmed

- Mechanical completeness: zero unflagged rows with |obs| > |A_rg| (raw and born legs; flags coincide because rel_delta_raw == rel_delta_born to machine precision — multiplicative C_rad cancels in the ratio).
- Closure by construction: B = A - obs same-sign as A on all 442 unflagged bins.
- Traceability: observed shifts trace field-level to optA_vs_cleanrelease_per_cell_delta.csv (4 spot bins to the last digit) under the aggregator's hard cell_label check; neg global-bin offset 245 confirmed.
- Manifest arithmetic exact both legs; straddle = ingrid + safe_out exact; all reconciliation blocks pass; 174-shard safe-out scan clean; conventions block independently documents the pool semantics (influx row-offdiag, outflux col-offdiag, rates normalized to content_rad = row sum incl. safe-side OUT column, net = in - out).
- Safe-out semantics settled and immaterial: 44,344/129,231,130 = 0.0343% (rg), 52,941/154,083,280 = 0.0344% (dj); out-flux to rad-OUT structurally invisible, bounded by no-pool-partner ~0.07% of matched.
- Cross-projection consistency: per-bin tables reproduce the join to the last digit (pos b158 in/out/net; pos b39 outflux; pos S3_x0 cell rate 135,538.1/3,829,850.5 = 3.539% ~ 3.54%).

## Missing Evidence

- The weighted headline rate (1.48%/1.48%) is not directly recomputed in the aggregate; rests on the hard-assert chain (pool row-sum == builder prior_gen per bin) plus the smoke run's exact weighted reconciliation. Acceptable as a provenance note; non-blocking.
- Per-event detail of the 3 rg body-vs-pool mismatches (which shards/bins) is not in the manifest — counts and reconciliation only. At 5.1e-8 of checked rows this is provenance completeness, not physics.

## Promotion-Record Conditions

The gate passes PROVIDED the promotion record (erratum attached as the record of correctness): (1) removes the false sign-flip completeness argument and cites the mechanical grep, naming the unflagged sign-flip bins; (2) corrects the maxima to b83 / b217 / b236 / neg b159 +4.2051%; (3) replaces "B at 0.3-0.7 x |A|" with the measured span ~0.005-1.06; (4) restricts "B ~ f_matched x GROSS" to an order-of-magnitude dilution statement on flagged near-balanced cells; (5) corrects the f_matched range to ~0.30-0.51 bulk / ~0.20-0.25 ip=4 edges; (6) records the 14/48 dj-flag sensitivity and the O(1) sub-bin net-rate disagreement between legs; and (7) carries the weighted-rate provenance note.

## Recommended Next Check

Hand the seven record conditions to the h1-sidis-supervisor to fold into the promotion record before sign-off (the erratum edit itself is an exact, fully specified change suitable for h1-mechanical-runner). No further physics validation is required for the OPT-A baseline promotion gate; the only optional residual is a one-line shard grep naming the 3 rg mismatch events for provenance completeness.

--- audit metadata ---
- Report persisted by the delegating supervisor session (critic toolset read-only), 2026-10-20.
- Retry note: first attempt returned an empty response; this is the retry with mandatory-final-summary instruction, per the recorded retry precedent.

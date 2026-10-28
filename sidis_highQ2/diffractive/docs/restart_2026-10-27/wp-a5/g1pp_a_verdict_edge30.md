# WP-A5 gate G1″(a) re-run at the LRG edge etaMaxClus08 < 3.0

Date 2026-10-28. Companion to `D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md` (frozen, edge 3.2); `D` =
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Trigger: user decision D15 (selection edge 3.0).

**Result: PASS (mechanical).** Every D7-retained bin has σ_rel^veto ≤ 0.05 in both frames: max 0.0378 (frame A) and 0.0248 (frame B).

## 1. Gate text
Unchanged: verbatim in the original §1 (G1″(a), acceptance σ_rel ≤ 0.05 per bin of the WP-C start grid, bin above it masked).

## 2. Inputs and provenance
- Script: `D/scripts/restart_2026-10-27/wp-a5/edge30/propagate_wpa5_edge30.py` (sha256 ff7e14eef816d12726b8bc8b8e0b3ada8bfa2424859adafccbbe75c95145f202). It imports the frozen
  `propagate_wpa5.main()` unchanged and wraps `sample()`: keeps events with ec08 < 3.0 or sentinel (< -900), drops their tracks,
  remaps `t_eglob`. Asserts: no kept event with ec08 ≥ 3.0; array lengths; track weight = event weight.
- Table: `D/data/restart_2026-10-27/wp-a5/edge30/g1pp_a_table_edge30.{csv,json}` (csv sha256 06748120c1333824a40cd8ed85bf7cb85940da4aa1fabed7da1e67f473072bb0).
- Reused: `D/data/restart_2026-10-27/wp-a5/veto_correction.json` (sha256 cff5c00ad579546b0a144830e361f2acb250d384669fe3226f13deae53b9dea7), `cells_<label>.npz`, `veto_weight.py`,
  the D7 masks `wp-c/mask/mask_start_frame{A,B}_G2_D7.json`. Frozen `g1pp_a_table.csv` sha256 3cc1ac125cbef0e175e4cc719668dc7fb2e952c9d491c292c2918ebaa0d9d1cb verified unchanged.
- Condor cluster 5951896 (`D/condor/restart_2026-10-27/wp-a5/edge30/`), executed 12 s wall, rc 0.
- Events 22 753 → 17 362; tracks 158 507 → 119 627 (per label events kept: 9012 1989/2503, 9024 4281/5970, 9033 2933/4176,
  9155 3352/4161, 9157 1681/2108, 9158 3126/3835).
- Reproduction check: the same script with `--edge 3.2` gave a CSV byte-identical to the frozen `g1pp_a_table.csv` (`cmp`); check output deleted.

## 3. Per-frame summary, edge 3.0 against 3.2
| | A 3.0 | A 3.2 | B 3.0 | B 3.2 |
|---|---|---|---|---|
| bins / D7-masked / D7-retained | 60 / 23 / 37 | 60 / 23 / 37 | 60 / 26 / 34 | 60 / 26 / 34 |
| `masked_veto` (all / not D7) | 1 / 0 | 3 / 0 | 2 / 0 | 2 / 0 |
| retained after both | 37 | 37 | 34 | 34 |
| max σ_rel, D7-retained (bin) | 0.0378 (h+ 10-20 x_F 5) | 0.0491 (same) | 0.0248 (h− 10-20 x_F 4) | 0.0346 (h+ 10-20 x_F 4) |
| median σ_rel, D7-retained | 0.0028 | 0.0031 | 0.0027 | 0.0031 |
| max abs(R_eta), D7-retained | 0.0374 | 0.0489 | 0.0248 | 0.0346 |
| `veto_eta_fragile`, D7-retained | 4 | 6 | 2 | 3 |
| Q-B: abs(R_eta) alone > 0.05 | 0 | 1 (D7-masked) | 1 (h+ 20-40 x_F 5, D7-masked) | 1 (same) |
| Q-B: masked, η-stepped form | 1 (h− 20-40 x_F 5, empty) | 1 (same) | 1 (same) | 1 (same) |

The threshold bin A h+ 10-20 x_F 5 is 0.0378 ± 0.0038 (bootstrap) on 32 raw tracks (0.0491 at 3.2).

## 4. Mask lists
- Frame A `masked_veto` at 3.0: h− 20-40 x_F 5 (empty bin), already D7-masked. At 3.2 also h+ 20-40 x_F 5 (0.055) and h− 10-20 x_F 5 (0.051).
- Frame B `masked_veto` at 3.0: h+ 20-40 x_F 5 (0.0538) and h− 20-40 x_F 5 (empty), both D7-masked (unchanged bins; 0.0663 at 3.2).
- Fragile, D7-retained, 3.0. Frame A: h+ 4-10 x_F 5; h+ 10-20 x_F 4 and 5; h− 10-20 x_F 4. Frame B: h+ 10-20 x_F 4; h− 10-20 x_F 4.
  Dropped from the 3.2 list: A h+ 20-40 x_F 4, A h− 4-10 x_F 5, B h+ 20-40 x_F 0. The binding downstream rule of the original §8 applies to the remaining bins.

## 5. Reused and changed
Reused: q, σ_q, η-shape δ_v (frozen, not re-derived, even/odd untouched), cell assignment, fine G1′(c) scales, weights, bootstrap seed,
all table columns. Changed: only the event selection (ec08 < 3.0), which removes 5 391 events (1 689 of them in label 9024).
Because the η-shape bins [3.0,3.2) and [3.2,3.3) are unpopulated by construction, the large δ_v of the η_max ≥ 3.0 region no longer enters R_eta.

## 6. Deviations
1. The D7 masks are the frozen start-grid masks; D7 was not re-evaluated at 3.0. The reduced MC statistics could move bins across the D7 threshold.
2. Event weights, N_D denominators and the per-event normalisation are recomputed on the reduced sample (the original code path), not rescaled.
3. Reggeon omission and the other items of the original §7 are unchanged.

## 7. Ruling
Mechanical re-evaluation by the lead of the pre-registered criterion (every D7-retained bin σ_rel ≤ 0.05): PASS in frame A
(37 of 37, max 0.0378) and frame B (34 of 34, max 0.0248). This is not an `h1-analysis-critic` ruling and no systematics or
code review was run on the edge-3.0 outputs. Open: D7 re-evaluation at 3.0 (WP-C), and the carried items 3 and 5 of the original §8.

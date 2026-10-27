# WP-A S3 record: §7.1 η_max two-free-template GOF on the remedy-1 payload

Date 2026-10-27. Plan: `wp-a_plan.md` (A7, S3); gate: A0 G1(c).
`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`; `R` = `D/data/restart_2026-10-27/wp-a`.

## Pre-registered inputs

The ND alternatives were fixed in the child plan Log before any fit:

- **Primary alternatives:** 7071 (RAPGAP ME+PS, verified by h1-mc-db, `../wp-d/mc3_7071_cascade_mode.md`), 7091 (DJANGOH CDM) and 7094 (RAPGAP ME+PS, same period and SW as 7091; added by the coordinator).
- **Fallback:** nd4cat (7071 + 7094 + 7096 + 7097, the four-catalogue convention), run because all three primary alternatives failed (A7).
- **Pass scope (supervisor):** every D cell and ALL, coarse and fine.

## Payload and fit

- **Payload:** `payload_wpa.py` builds the payload from the S2 test-split partials (`dcell_eta.cutremoved`: A_pre ∧ FMD ∧ Plug<7 ∧ FTS, η cut removed, cluster η, 121-edge grid). Output: `R/s3_gof/payloads/payload_<chain>.json`, 76,619 DATA events in D cells.
  - This replaces the plan's `eta_builder_wpa.py` and v3 assembler. The basis is identical (t4 cross-checks X1/X2), and the v3 assembler's control cross-comparison cannot accept corrected partials.
  - Identity is re-proven on split=all against assembled_v3: DATA raw exact, MC raw exact, exposure1 max relative difference 1.3e-7 (`R/s3_gof/payload_identity/`).
- **Test `payload_asserts`:** `pass` and `conservation.all_pass` are true for every payload.
- **Fit:** `fit_7p1_wpa.py`, a fork of `run_section_7_1_twofree_v3_2026-10-20.py`. The fit, deviance, multistart, coarse grouping and toy recipe are unchanged. Only the ND membership is generalised.
- **Test `baseline_reproduced`** (2026-10-20 assembled_v3 payload, alt 7071, fine ALL, 300 toys):
  - Result: D = 5925.826, s_diff 0.90452, s_ND 1.32511, toy p 0.0033223.
  - Toy p is bit-identical. Scales agree to 1e-8, and the original script's own `fit()` gives the same in this scipy 1.15 environment.
  - The rebuilt split=all payload gives D = 5925.826 and p 0.0033223.
- **Test `toy_floor`:** all 64 fits (S3 + S4) report n_toys = 1000 and p_floor = 1/1001.
- **Condor:** clusters 5902092 (24 fits) and 5902094 (8 nd4cat fits), all rc 0. Table of all fits: `R/s3_gof/g1c_fit_table.csv`.

## Results (D/ndf, template-statistics-aware toy p; floor 0.0010)

**Remedy 1 (S3)**

| alt | binning | D0 D/ndf, p | D1 | D2 | ALL | s_diff, s_ND (ALL) |
|---|---|---|---|---|---|---|
| 7071 | fine | 3365/118, 0.0010 | 1628/118, 0.0030 | 1078/118, 0.0110 | 6117/358, 0.0010 | 1.132, 1.564 |
| 7071 | coarse | 3063/53, 0.0010 | 1254/36, 0.0010 | 497/22, 0.0010 | 5130/115, 0.0010 | 1.146, 1.541 |
| 7091 | fine | 3382/118, 0.0010 | 1852/118, 0.0010 | 1152/118, 0.0020 | 6530/358, 0.0010 | 1.109, 2.033 |
| 7091 | coarse | 3029/53, 0.0010 | 1490/36, 0.0010 | 647/22, 0.0010 | 5619/115, 0.0010 | 1.127, 1.994 |
| 7094 | fine | 3217/118, 0.0010 | 1760/118, 0.0010 | 1254/118, 0.0010 | 6272/358, 0.0010 | 1.174, 4.405 |
| 7094 | coarse | 2876/53, 0.0010 | 1273/36, 0.0010 | 541/22, 0.0010 | 4980/115, 0.0010 | 1.194, 4.319 |
| nd4cat | fine | 3264/118, 0.0010 | 1774/118, 0.0010 | 806/118, 0.0030 | 5902/358, 0.0010 | 1.097, 1.688 |
| nd4cat | coarse | 2894/53, 0.0010 | 1284/36, 0.0010 | 517/22, 0.0010 | 5052/115, 0.0010 | 1.113, 1.661 |


The 2026-10-20 baseline (full sample, uncorrected) for comparison:

| alt | fine ALL | coarse ALL | toy p |
|---|---|---|---|
| 7071 | D/ndf 5926/358 | 5248/132 | 0.0033 (300-toy floor) |
| 7091 | — | 5832/132 | 0.0033 |

## Ruling against A0 G1(c)

FAIL for every alternative.

- 28 of 32 fits sit at the 1000-toy floor. The largest p is 0.011 (7071 fine D2).
- Every ALL-scope fit, coarse and fine, is at p = 0.0010.
- The remedy-1 payload does not move the fit toward acceptance. Fine ALL D/ndf is 6117/358 for 7071, against the baseline 5926/358. This was expected (plan Risks): an η-independent removal cannot repair a shape mismatch at D/ndf ≈ 17.

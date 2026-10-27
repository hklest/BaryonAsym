# WP-A2 S4: G1′ evaluation on the test split (odd EventNumber)

Date 2026-10-27. Frozen correction set `D/data/restart_2026-10-27/wp-a2/s3_corr/s3_correction_record.json`, sha256
`4b2b5e269a1f8d07f22d34a24c9416742eba53ebb6da23b85014bcf3bf79d719`. Both S4 submit logs quote this hash, verified
before submission (`D/condor/restart_2026-10-27/wp-a2/{diag_wpa2,fit_wpa2}/submit_log_s4.txt`).
`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.

## 1. What ran (each queue once; no resubmission)

- n1 pass: Condor cluster 5911827, 17 jobs, all exit 0. Flags: `--split odd --eta-cut 3.3`, no noise table (remedy-1
  is excluded). Partials are in `D/data/restart_2026-10-27/wp-a2/s4_g1p/n1/{data,mc}/<label>/`.
- Payloads: `g1prime_eval_wpa2.py prepare` built the frozen-definition odd payloads `s4_g1p/payloads/odd_frozen_{7071,7091,nd4cat}.json`:
  `etaMaxClus04`, η < 3.4 plus [3.4, 5), 89 bins per cell.
- Fits: cluster 5912792, 24 jobs (3 alternatives × {fine, coarse} × {D0, D1, D2, ALL}, 1000 toys), all exit 0.
- Evaluation: `g1prime_eval_wpa2.py evaluate` (login node) runs the unchanged WP-A `g1_eval_wpa.py` for (a) and (b)
  and reads the fits for (c). Output: `D/data/restart_2026-10-27/wp-a2/s4_g1p/g1prime_table.csv` (sha256
  `611d5140fba1716587015c5d347b261f2b684022d05a72f54abdb4a9f0f0b4c3`). Plot:
  `D/plots/restart_2026-10-27/wp-a2/s4/g1prime_c_fit_fine_ALL.png`.

## 2. Numbers (odd split, stack C, signed v3, η_max(0.8 GeV) < 3.3)

| clause | limit | 7071 | 7091 | nd4cat |
|---|---|---|---|---|
| (a) Plug N−1 unit-area max \|MC/DATA−1\| | ≤ 0.10 | 0.982 | 0.965 | 0.953 |
| (a) Δf_fail D0 / D1 / D2 | \|·\| ≤ 0.02 | −0.210 / −0.217 / −0.224 | −0.214 / −0.225 / −0.238 | −0.210 / −0.218 / −0.230 |
| (b) FMD / FTS N−1, nominal | — | 2.16 / 0.71 | 1.70 / 0.71 | 2.14 / 0.68 |
| (b) best admissible joint (FMD, FTS) | < 0.25 | 0.775 (0.768, 0.775) | 0.744 (0.738, 0.744) | 0.763 (0.746, 0.763) |
| (c) max toy p over 8 fits | ≥ 0.05 in all 8 | 0.015 (fine D2) | 0.013 (fine D2) | 0.018 (fine D1) |
| (c) ALL fine D/ndf, p | | 3456/265, 0.0010 floor | 2767/265, 0.0010 floor | 2624/265, 0.0010 floor |
| (c) ALL coarse D/ndf, p | | 1778/76, 0.0010 floor | 1827/76, 0.0010 floor | 1715/76, 0.0010 floor |
| verdict | | (a) FAIL, (b) FAIL, (c) FAIL | (a) FAIL, (b) FAIL, (c) FAIL | (a) FAIL, (b) FAIL, (c) FAIL |

- DATA f_fail is 0.272 / 0.300 / 0.337, against MC 0.058-0.113.
- Of the 24 fits, 17 sit at the toy floor 1/1001. The 7 off-floor values are 0.002-0.018, all in single-cell fits.

## 3. Tests

- `single_evaluation`: PASS (both queues submitted once, no resubmission).
- `clauses_complete`: PASS. Per alternative: (a) shape plus 3 D cells, (b) best joint, and (c) 8 toy-p rows with
  D/ndf.
- `toy_floor_reported`: PASS (each p carries `n_toys` and `at_floor`).
- `tally_within_cap`: PASS. Total 0.06585 Condor-days of 5; 0.377 calendar days of 7 since the first WP-A2 submission
  (2026-10-27T04:30:47Z). Source: `D/data/restart_2026-10-27/wp-a2/condor_tally.csv`, kept by `h1-naf-operator`.

The S4 queue regeneration (no per-chain MC jobs after the remedy-1 exclusion) happened before any odd-split job.

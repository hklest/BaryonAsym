# WP-A S4 record: remedies 2 (threshold scan) and 3 (no Plug veto)

Date 2026-10-27. Plan: `wp-a_plan.md` (A8, A9, A10, S4). S4 ran because S2 and S3 failed G1.
`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`; `R` = `D/data/restart_2026-10-27/wp-a`.

## Remedy 2: threshold scan (A8, control region only)

- **Script:** `s4_threshold_choice_wpa.py`. It reads only `R/s1_control` partials (sha256 values in the output), so the test `threshold_choice_control_only` passes.
- **Method:** the thresholds 3/5/7/10/15 GeV are bin edges of the 0.5 GeV control ePlug N−1 histogram, so each fail rate is exact without new jobs. The T = 7 pass count reproduces the stored stack-pass count in all 17 files.
- **Criterion (A8, "Plug fail rates"):** control Plug N−1 fail fraction f(T) = 1 − N(ctrl ∧ FMD ∧ FTS ∧ ePlug < T)/N(ctrl ∧ FMD ∧ FTS), data vs the projected chain cocktail, 2σ in every period.

Chain 7071, f_data / f_MC (z):

| T [GeV] | ep0304 | em0405 | em06_true | ep0607_true |
|---|---|---|---|---|
| 3.0 | 0.069 / 0.023 (3.2) | 0.092 / 0.025 (5.5) | 0.154 / 0.032 (6.3) | 0.093 / 0.023 (6.5) |
| 5.0 | 0.053 / 0.010 (3.5) | 0.069 / 0.013 (5.4) | 0.100 / 0.019 (5.1) | 0.059 / 0.010 (5.9) |
| 7.0 | 0.036 / 0.007 (2.9) | 0.051 / 0.011 (4.3) | 0.070 / 0.013 (4.2) | 0.043 / 0.007 (5.1) |
| 10.0 | 0.033 / 0.005 (3.0) | 0.037 / 0.004 (4.7) | 0.052 / 0.006 (4.1) | 0.027 / 0.005 (3.8) |
| 15.0 | 0.019 / 0.003 (2.2) | 0.017 / 0.003 (2.8) | 0.035 / 0.002 (3.5) | 0.016 / 0.003 (2.7) |

- **Other chains:** 7091 and 7094 give the same picture (|z| at 15 GeV between 2.2 and 3.5).
- **Diagnostic:** the stack fail rate disagrees by 4.5-10.9σ at every threshold. This is expected, because the FMD and FTS excess does not depend on the Plug threshold.
- **Test `threshold_scan_monotone`:** the data control Plug fail fraction decreases monotonically with T in every period (table above). The Stage-B N−1 per-cell fail fraction is monotone by construction, because the N−1 histogram does not depend on T.
- **Ruling:** no pre-registered threshold satisfies A8. Data stays 5-17 times the MC rate up to 15 GeV, so the excess is not a near-threshold effect.
  - Remedy 2 has no nominal threshold. Per A8, G1 is evaluated at no other threshold (look-elsewhere).
  - The G1(a) shape clause could not pass under remedy 2 in any case: the Plug N−1 histogram is threshold-independent and stays at 0.977.

## Remedy 3: no Plug veto (A9)

- **Runs:**
  - Condor cluster 5902093: 17 test-split N−1 jobs with `--no-plug-veto`, uncorrected MC, all rc 0.
  - Worker n1_wpa.py md5 b59d11738f319df67ff5cd0de3ed8607 (status files `md5_<label>_test_<label>.txt`, all 17 identical).
  - Fits: clusters 5902095 (3 alternatives, 24 fits) and 5902096 (nd4cat fallback after all three failed, 8 fits), 1000 toys, all rc 0.
  - Outputs: `R/s4_noplug/{partials,payloads,analysis,fits}`. The payload has 124,740 DATA events in D cells.
- **A9 reading (supervisor decision 3):** G1(a) = FMD and FTS N−1 max deviation ≤ 0.10, plus a Stage-B data/MC yield ratio within 1 ± 0.10 in each D cell after the PDISS M_Y > 1.6 GeV migration term.
- **Migration term:** signed PDISS (9155/9157/9158) Stage-B content with truth mirMY ≥ 1.6 GeV, enveloped by ±50%.
- **h1-mc-expert S4d validation:** YES-WITH-CONDITIONS (`s4d_mc_expert_migration.md`).
  - The expert's open check found that mirMY and hatMYGen are not identical on 9157: 70% of events differ by more than 1e-3 GeV.
  - Only 280 of 86,769 events (0.3%) cross 1.6 GeV, which is negligible against a 1-2% term.

| chain | FMD N−1 | FTS N−1 | yield ratio D0 / D1 / D2 (±50% migration envelope) | migration fraction D0/D1/D2 | G1(a)-A9 |
|---|---:|---:|---|---|---|
| 7071 | 2.304 | 0.729 | 1.213 / 1.309 / 1.507 (1.20-1.22, 1.30-1.32, 1.50-1.51) | 0.019 / 0.016 / 0.008 | FAIL |
| 7091 | 1.835 | 0.731 | 1.237 / 1.406 / 1.961 | 0.019 / 0.017 / 0.011 | FAIL |
| 7094 | 1.070 | 0.757 | 1.333 / 1.568 / 2.358 | 0.021 / 0.019 / 0.013 | FAIL |
| nd4cat | 2.332 | 0.699 | 1.209 / 1.320 / 1.553 | 0.019 / 0.016 / 0.009 | FAIL |

- **Test `noplug_migration_reported`:** PASS (table above).
- **Plug N−1 without a Plug veto:** unchanged from the uncorrected run, 0.97-0.99.
- **G1(b) under remedy 3:** the best admissible joint value is 0.710-0.920. FAIL.
- **Context:** the Stage-B data/MC yield ratio is already 1.21-1.43 (7071) with the Plug veto and remedy 1 (`s2_remedy1/analysis`), so removing the veto does not create the deficit.

G1(c) on the no-Plug payload:
**Remedy 3, no Plug veto (S4)**

| alt | binning | D0 D/ndf, p | D1 | D2 | ALL | s_diff, s_ND (ALL) |
|---|---|---|---|---|---|---|
| 7071 | fine | 4590/118, 0.0010 | 2105/118, 0.0010 | 1298/118, 0.0010 | 8609/358, 0.0010 | 0.977, 2.285 |
| 7071 | coarse | 4381/55, 0.0010 | 1819/38, 0.0010 | 717/24, 0.0010 | 7569/122, 0.0010 | 0.983, 2.277 |
| 7091 | fine | 4825/118, 0.0010 | 2629/118, 0.0010 | 1339/118, 0.0010 | 8884/358, 0.0010 | 0.956, 2.849 |
| 7091 | coarse | 4608/55, 0.0010 | 2357/38, 0.0010 | 1041/24, 0.0010 | 8231/122, 0.0010 | 0.965, 2.834 |
| 7094 | fine | 4197/118, 0.0010 | 2086/118, 0.0010 | 1372/118, 0.0010 | 8135/358, 0.0010 | 0.931, 6.318 |
| 7094 | coarse | 3949/55, 0.0010 | 1801/38, 0.0010 | 720/24, 0.0010 | 7029/122, 0.0010 | 0.938, 6.291 |
| nd4cat | fine | 4097/118, 0.0010 | 2037/118, 0.0010 | 926/118, 0.0010 | 7651/358, 0.0010 | 0.940, 2.420 |
| nd4cat | coarse | 3880/55, 0.0010 | 1762/38, 0.0010 | 693/24, 0.0010 | 6972/122, 0.0010 | 0.948, 2.409 |

All 32 remedy-3 fits sit at the 1000-toy floor (p = 0.0010). Fine ALL D/ndf is 7651-8884 over 358, worse than remedy 1.

## Ruling against A0 (A9 reading)

Remedy 3 fails G1(a) and G1(c) for every ND alternative, and G1(b) also fails. No remedy is a systematic leg, because none passes.

# WP-A S2 record: remedy 1 (control-region noise removal) and G1(a)/(b)

Date 2026-10-27. Plan: `wp-a_plan.md` (A4-A6, S2); gate text: plan A0, quoted from the parent plan.
`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`; `R` = `D/data/restart_2026-10-27/wp-a`.

## Noise table (S2a)

- File: `R/s2_remedy1/noise_table.json`, sha256 `f93a6dd270f56ec1832efb1435877842ea77672667c4f870f7c7872d543fbcaf`, produced by `noise_table_wpa.py`.
- Inputs: only the 17 `R/s1_control/partials/control/*` partials. Their sha256 values are inside the JSON, and the script refuses any non-`s1_control` input. Test `no_test_tuning`: PASS.
- Test `noise_table_bounds`: PASS.
- Removal probability: q = (f_data − f_MC)/(1 − f_MC). This is the exact control closure of the independent-noise model.
  - A4's p = f_data − f_MC is recorded as `p_naive`.
  - h1-mc-expert S2d: correct, and documented as an extension of A4.

Chain 7071 (8 diffractive + 7071, projected per period).

- Chains 7091 and nd4cat differ from 7071 by ≤ 0.003 in q.
- Chain 7094 differs: 7094 represents ND only in ep0607, so the cocktail projected onto ep0304, em0405 and em06 has no ND member. Its q there is inflated to 0.232 / 0.258 / 0.245 (ep0607: 0.191). This drives the 7094 D0 overshoot below, but no PASS/FAIL changes.

| period | N_ctrl data | f_data | f_MC | p_naive | q ± σ |
|---|---:|---:|---:|---:|---:|
| ep0304 | 569 | 0.390 | 0.278 | 0.112 | 0.155 ± 0.032 |
| em0405 | 1160 | 0.387 | 0.252 | 0.135 | 0.181 ± 0.024 |
| em06_true | 603 | 0.380 | 0.254 | 0.126 | 0.169 ± 0.031 |
| ep0607_true | 1480 | 0.416 | 0.278 | 0.138 | 0.191 ± 0.024 |

- q > 1σ in every period, so remedy 1 applies.
- Per-detector excess (data − MC fail fraction, Plug/FMD/FTS):

  | period | Plug | FMD | FTS |
  |---|---:|---:|---:|
  | ep0304 | .069 | .054 | .057 |
  | em0405 | .071 | .063 | .073 |
  | em06 | .075 | .065 | .051 |
  | ep0607 | .064 | .101 | .036 |

  The stack excess is therefore only about 1/3 Plug.
- Excess counts above 7 GeV: 12-35 events per period. The excess-spectrum shape is statistics-limited.

## h1-mc-expert sign-off (S2d)

YES-WITH-CONDITIONS (`s2d_mc_expert_signoff.md`). Conditions honoured here:

- The seed is fixed: 20261027 + crc32(label|chain), stored per partial.
- The spectrum is stored in the noise table.
- The 12-35-event excess statistics and the absolute-lumi normalisation variant (data/MC control yield 0.92-1.01) are systematics for WP-J.
- A5 attributes the whole stack excess to Plug, so the Plug N−1 below is **stack-attributed**. It over-shifts the Plug by about 3× and leaves FMD/FTS uncorrected. The expert asks for a per-detector q variant, which is a `user:` item (see verdict).

## Corrected run (S2b)

- Condor cluster 5902091: 43 jobs, all rc 0, 0.0044 Condor-days.
- Worker n1_wpa.py md5 b59d11738f319df67ff5cd0de3ed8607, reported by h1-naf-operator from the per-job status files at the S2 drain.
  - The per-job md5 files were later overwritten by the S4 run (the TAG in `n1_wpa_job.sh` collides across runs; the wrapper is left as it ran).
  - The file has not changed since: its mtime is 02:14 CET, before the S2 queue was written at 02:15, and the S4 jobs recorded the same md5.
- The DATA test partials equal the S1 test partials in every count.
- MC: test split, `--noise-table` above, chains 7071 / 7091 / 7094 / nd4cat.
- Evaluator: `g1_eval_wpa.py`, outputs `R/s2_remedy1/analysis/g1_<chain>.json`.
- Test `removal_rate_closure`: 77 (component, period) rows; pulls have mean −0.09 and s.d. 1.11.
  - One row, 7071|ep0304, has a pull of −3.26, so the literal per-row 3σ rule fails for chain 7071.
  - For 77 binomial rows, P(max |pull| ≥ 3.26) ≈ 0.08. The rate offset is 0.4% of q and has no effect on any ruling.

## G1(a) after remedy 1 (stack C, signed v3, test sample)

Plug N−1 fail fraction, data vs cocktail, and |Δ|:

| chain | D0 data / MC / Δ | D1 | D2 | max \|Δ\| | Plug N−1 max dev | G1(a) |
|---|---|---|---|---:|---:|---|
| uncorrected (7071) | 0.219 / 0.043 | 0.236 / 0.059 | 0.285 / 0.094 | 0.191 | 0.977 | FAIL |
| 7071 | 0.219 / 0.214 / −0.004 | 0.236 / 0.229 / −0.008 | 0.285 / 0.247 / −0.037 | 0.037 | 4.035 | FAIL |
| 7091 | / 0.210 / −0.008 | / 0.223 / −0.014 | / 0.239 / −0.046 | 0.046 | 6.355 | FAIL |
| 7094 | / 0.257 / +0.039 | / 0.253 / +0.017 | / 0.297 / +0.012 | 0.039 | 6.801 | FAIL |
| nd4cat | / 0.213 / −0.006 | / 0.232 / −0.005 | / 0.242 / −0.043 | 0.043 | 6.086 | FAIL |

How the rate and shape clauses fail:

- **Rate clause:** remedy 1 brings D0 and D1 within 0.02 for three of the four chains. D2 stays 0.037-0.046 low for those three, and 7094 overshoots in D0.
- **Shape clause:** this clause fails worse after the correction.
  - The driver is bin 78 (39-39.5 GeV), where the spiky, statistics-limited excess spectrum places replaced events.
  - Uncorrected, the shape failure sits below threshold. In 1-7 GeV the MC/DATA unit-area ratio is 0.29-0.44 (0.71 at 0.5 GeV), and at 0 GeV it is 1.35.
  - MC therefore lacks the broad sub-threshold Plug energy that data shows. Removing events cannot create that, so the shape clause is out of reach for any event-removal remedy.

## G1(b) after remedy 1

FMD and FTS N−1 max deviation. The nominal cocktail is signed v3. The admissible scan covers 1,953,125 cocktails: 9 multipliers (8 components + one common ND) in {0.5, 0.75, 1, 1.25, 1.5}, with both observables on the same cocktail.

| chain | FMD nominal | FTS nominal | best joint max (FMD, FTS) | best FMD-only | best FTS-only | G1(b) |
|---|---:|---:|---|---:|---:|---|
| 7071 | 1.858 | 0.727 | 0.759 (0.739, 0.759) | 0.530 | 0.601 | FAIL |
| 7091 | 1.248 | 0.731 | 0.709 (0.706, 0.709) | 0.490 | 0.589 | FAIL |
| 7094 | 0.728 | 0.758 | 0.680 (0.645, 0.680) | 0.519 | 0.617 | FAIL |
| nd4cat | 1.837 | 0.693 | 0.738 (0.728, 0.738) | 0.522 | 0.551 | FAIL |

The best joint cocktails sit on the grid boundary: PDISS at 1.5, ND at 0.5. No admissible cocktail comes below 0.25.

## η_max > 3.5 forward-energy-scale diagnostic (S1 decision 4; no gate role)

From the S2 partials (`eta35_eplug`, ePlug before replacement); table `R/s2_remedy1/analysis/eta35_eplug_7071.json`.

| period | N data | ⟨ePlug⟩ data [GeV] | ⟨ePlug⟩ MC (7071 cocktail) | MC/DATA | f(ePlug ≥ 7) data / MC |
|---|---:|---:|---:|---:|---|
| ep0304 | 4710 | 17.18 | 14.28 | 0.83 | 0.742 / 0.693 |
| em0405 | 8589 | 15.73 | 14.27 | 0.91 | 0.715 / 0.693 |
| em06_true | 4802 | 15.77 | 14.27 | 0.91 | 0.728 / 0.693 |
| ep0607_true | 10749 | 16.64 | 14.28 | 0.86 | 0.742 / 0.692 |

- In the ND-dominated region the MC Plug energy is 9-17% low, more so in the e⁺ periods. This is consistent with a Plug energy-scale or response deficit, not threshold noise alone.
- The 7094 chain is not comparable outside ep0607, because 7094 represents only ep0607.

## Ruling against A0

After remedy 1, both G1(a) clauses fail for every ND alternative: the shape metric gives 4.0-6.8 against a limit of 0.10, and max |Δf_fail| is 0.037-0.046 against 0.02. G1(b) fails too, with a best of 0.68 against < 0.25. Remedy 1 is not a nominal pass.

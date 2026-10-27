# WP-A S1 record: period split and control region

Date 2026-10-27. Plan: `wp-a_plan.md` (A1-A4, S1). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.

## Inputs and provenance

- Inputs: 17 merged files. These are the 14 files of the t4 queue plus 7094/7096/7097 from `D/data/t2_detector/mc_merged_nd4cat_2026-10-20/`, added for the S3 ND alternatives.
- Run list: the frozen certified run list (ACCEPT only), with the scale tables v3 (sha256 b7609559…) and the nd4cat sidecar.
- Worker: `D/scripts/restart_2026-10-27/wp-a/n1_wpa.py`, a fork of `D/scripts/t4/build_localization_n1_mytbl_nd4cat_2026-10-20.py`.
  - S1 jobs used md5 8fde88ab574c7bd752ecaefc02cdaadd.
  - One additive block was added later (the η_max > 3.5 diagnostic), and identity was re-verified after it.
- The t4 originals' md5s are in `D/data/restart_2026-10-27/wp-a/t4_originals_md5.txt`.
- Condor: cluster 5902090, 51 jobs (17 files × splits all/test/control), all rc 0, 0.0088 Condor-days.
- Period map: `D/data/restart_2026-10-27/wp-a/period_map.csv` (`make_period_map_wpa.py`).
- Checks: `D/data/restart_2026-10-27/wp-a/s1_split/analysis/s1_checks.json` (`s1_analyze_wpa.py`).

## Tests

| test | result |
|---|---|
| `fork_default_identity` (9155, default flags vs t4 partial: counts, hists, dcell_eta, my_tables) | PASS: 0 differences, raw exact |
| `fail_fraction_recomputed` (split all, stack C) | PASS: 0.2070 / 0.2297 / 0.2831, identical to t4 |
| t4 N−1 maxima reproduced (plus_reg_7071, stack C, signed) | Plug 0.978, FMD 1.773, FTS 0.729 (t4: 0.978 / 1.773 / 0.729) |
| `control_disjoint` ((Run, Event) keys control ∩ test, per file) | PASS: 0 shared keys in 17/17 files; test + control = A_pre count |
| `split_complete` (control + test = all, every mask and D cell) | PASS 17/17 |
| `period_map_total` | PASS: 317,212.804 nb⁻¹ |
| (Run, Event) uniqueness | unique in every file, so the parity basis is `EventNumber` |

- MC events in run gaps between periods (unmapped, A_pre): 0-97 per file, e.g. 97 of 4.66 M for 7091.
- These events do not affect the remedy, because MC periods are assigned by projection (below).

## Period map and the MC period convention

| period | run range | ACCEPT lumi [nb⁻¹] |
|---|---|---:|
| ep0304 | 357160-392213 | 43,833.59 |
| em0405 | 399629-436893 | 98,087.05 |
| em06_true | 444312-466997 | 51,216.11 |
| ep0607_true fall-06 | 468532-492541 | 79,108.01 |
| ep0607_true 2007 | 492560-500611 | 44,968.04 |

Each MC component carries one sim-period overlay pool (`s1c_mc_expert_lookup.md`):

| sim-period pool | components |
|---|---|
| 06_07e+ | 9012, 9024, 9158, 6677, 7091, 7094 |
| 04_05e− | 9033, 9155, 6671, 7096 |
| 2006e− | 9157, 7071 |
| 03_04e+ | 7097 |

- Most components represent more data periods than they were simulated for (composite scale v3).
- The remedy therefore assigns each MC event a represented data period, k ∈ P_c, with probability L_k/L(P_c).
  - P_c is the unique decomposition of the component's exposure_factor into certified period lumis.
  - Examples: 9012, 7071 and 7091 represent all four periods; 9024/9158/6677 represent ep0304 + ep0607; 9033/6671 represent em0405 + em06; 7094 represents ep0607 only.
- Grouping MC by its own RunNumber confounds composition. For example, em06 then contains only 9157 + 7071, giving a control f_MC of 0.75.
- The sim-period mismatch is a systematic leg (h1-mc-expert), not a nominal input.

## Data Plug N−1 fail fraction per period (stack C, raw; ±binomial; N in parentheses)

| period | D0 | D1 | D2 |
|---|---|---|---|
| ep0304 | 0.202 ± 0.006 (3912) | 0.236 ± 0.010 (1909) | 0.268 ± 0.016 (786) |
| em0405 | 0.209 ± 0.004 (8567) | 0.221 ± 0.006 (4503) | 0.279 ± 0.010 (1909) |
| em06_true | 0.222 ± 0.006 (4606) | 0.251 ± 0.009 (2350) | 0.296 ± 0.014 (1109) |
| ep0607_true | 0.201 ± 0.004 (10467) | 0.225 ± 0.006 (5229) | 0.286 ± 0.010 (2031) |
| ep0607 fall-06 | 0.198 ± 0.005 | 0.215 ± 0.007 | 0.281 ± 0.013 |
| ep0607 2007 | 0.206 ± 0.006 | 0.240 ± 0.010 | 0.295 ± 0.017 |

- The deficit is present in every period with similar size, so it is not a single bad run range.
- In D0 the period spread (0.201-0.222) is about 3σ, mostly em06.

MC per component (split all, stack C, signed):

| component | D0 | D1 | D2 |
|---|---|---|---|
| 9012 | 0.020 | 0.027 | 0.053 |
| 9024 | 0.014 | 0.023 | 0.036 |
| 9033 | 0.017 | 0.018 | 0.029 |
| 9155 | 0.019 | 0.021 | 0.021 |
| 9157 | 0.028 | 0.023 | 0.069 |
| 9158 | 0.023 | 0.017 | 0.028 |
| 6671 | 0.097 | 0.094 | 0.085 |
| 6677 | 0.088 | 0.076 | 0.111 |
| 7071 | 0.139 | 0.134 | 0.126 |
| 7091 | 0.125 | 0.123 | 0.108 |
| 7094 | 0.137 | 0.131 | 0.119 |
| 7096 | 0.137 | 0.133 | 0.122 |
| 7097 | 0.145 | 0.138 | 0.122 |

Cocktail plus_reg_7071: 0.040 / 0.059 / 0.094 on the full sample.

## Control region (A2): counts and fail fractions (raw)

Definition: A_pre ∧ (etaMaxClus08 < 1.5 or sentinel) ∧ xIp < 0.04 ∧ mx > 3 GeV ∧ EventNumber even.

| sample | N_ctrl | f_stack | f_Plug | f_FMD | f_FTS |
|---|---:|---:|---:|---:|---:|
| data ep0304 | 569 | 0.390 | 0.132 | 0.274 | 0.149 |
| data em0405 | 1160 | 0.387 | 0.137 | 0.262 | 0.152 |
| data em06_true | 603 | 0.380 | 0.143 | 0.260 | 0.133 |
| data ep0607_true | 1480 | 0.416 | 0.126 | 0.321 | 0.129 |
| elastic 9012 / 9024 / 9033 | 302 / 358 / 217 | 0.113 / 0.115 / 0.051 | ≤ 0.007 | 0.02-0.09 | 0.02-0.04 |
| PDISS 9155 / 9157 / 9158 | 1245 / 617 / 1266 | 0.56-0.57 | 0.10-0.12 | 0.43-0.44 | 0.19-0.25 |
| Reggeon 6671 / 6677 | 45 / 49 | 0.24 / 0.35 | 0.11 / 0.16 | 0.20 / 0.33 | 0.02 / 0.06 |
| ND 7071 / 7091 / 7094 / 7096 / 7097 | 1380-3805 | 0.84-0.87 | 0.35-0.41 | 0.78-0.82 | 0.21-0.25 |

- The control region is noise-dominated only for elastic events.
- PDISS and ND fail the forward stack physically even at η_max < 1.5. This is the leakage named in the plan Risks.
- The noise excess is therefore measured against the full projected cocktail (S2a), not against elastic MC alone.
- The excess in each η_max stratum ([−1,0), [0,1), [1,1.5)) is 0.09-0.18, i.e. it does not vanish at the deepest gaps. This points to detector-side activity that the overlay misses.
- The ep0607 halves give control f_stack 0.422 ± 0.016 (fall-06) and 0.406 ± 0.021 (2007). They differ by < 2σ, so A1 keeps 4 period values.

## h1-mc-expert lookup (S1c, `s1c_mc_expert_lookup.md`)

- The RT pools on `/pnfs/desy.de/dphep/online/h1/noise/` are readable. They are LAr/Spacal/CPC pools; there are no Plug/FMD/FTS pool files.
- RT_NOISE=Y adds Plug energy noise at reconstruction (`pscale`, OSRN word 9, VT-45/49), so it enters `ePlug`. FMD noise is hit-level. Confidence: medium-high.
- The overlay uses 2-4 RT runs per period, a snapshot that does not track beam- or luminosity-dependent forward activity.
- The projection is sound for luminosity weighting.

## η_max > 3.5 energy-scale diagnostic

Assigned to S1 as a table with no gate role. It is filled from the S2 data and MC partials (block `eta35_eplug`, ePlug before any remedy). See `s2_remedy1_record.md`, section "η_max > 3.5".

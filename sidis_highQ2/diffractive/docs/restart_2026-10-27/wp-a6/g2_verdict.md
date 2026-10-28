# WP-A6 gate G2 verdict (and G1″(c))

Date 2026-10-28. Package WP-A6 of `.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`; child plan
`.supervisor/working/plans/2026-10-28-wp-a6-leakage-refit.md`. `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Fit details and the even-split tables:
`D/docs/restart_2026-10-27/wp-a6/fit_record.md`.

**Ruling: PASS with masks** — M_X [4, 10) retained, [10, 20) and [20, 40] masked by G2 (`h1-analysis-critic`, last section).

## Pre-registered gate text (parent plan, verbatim)

  (c) **Leakage (WP-A6).** The two-template η_max fit on `etaMaxClus04` is confined to η_max < 3.0 (0.05 bins,
  no merged edge bin; the LAr-edge 3.15-3.4 region is excluded as simulation response, WP-A4 §1.1); toy GOF
  on the restricted range is reported (≥ 1000 toys) as a diagnostic, not gated; the ND-generator spread over
  7071 / 7091 / nd4cat is the systematic; the gate is G2.

- **G2 (WP-A6 on the existing skims, re-confirmed by WP-F on the new production; leakage ceiling).** Per
  retained M_X bin, ND leakage fraction f_ND ≤ 0.30 under the largest of the three ND chains, with
  uncertainty = fit statistical ⊕ generator spread (max |f_ND(7091 or nd4cat) − f_ND(7071)|; nominal 7071 per
  the WP-D/MC-3 verification), fit range η_max(0.4) < 3.0 (G1″(c)). Amended 2026-10-28 (lead, Open Q7): the
  former GOF-dependent branch ("when G1(c) holds … without a GOF pass f_ND ≤ 0.10 with a ±100% correlated
  envelope") is retired, because GOF over the LAr edge is no longer a gate and the generator spread replaces
  the envelope. Bins above the ceiling are masked; if every M_X bin is masked
  the restart stops. Justification: the ceiling caps the leakage term at ≈ 10% per bin (0.30 × a
  combined uncertainty ≤ 1/3 of f_ND), the size of the parent's whole detector-side total
  (median 7.4-7.9%, `sidis_highQ2/docs/RUN_STATUS.md:7`), so one background term cannot dominate
  the budget before the G5 ceiling.

Readings applied (child plan Decisions 6-7, recorded before the odd fit): the ceiling is applied to the central f_ND of the
chain with the largest f_ND in the bin; the uncertainty σ_G2 = fit-statistical ⊕ generator spread is quoted beside it, not
added (Open Q1 option (a), as briefed); the edge-band term δ_edge is reported beside it with a flip flag.

## Per-M_X table (odd split, ruling)

Selection SEL = `etaMaxClus08` < 3.3 (frozen WP-A2 `SB` mask) ∧ Plug/FMD/FTS vetoes ∧ x_IP < 0.04 ∧ M_X > 3 GeV, D cells
D0-D2. f_ND = Σ_cell s_ND · N_ND^SEL / N_data^SEL; in-fit = η04 < 3.0, band = η04 ≥ 3.0 (extrapolated with the fitted s_ND).
σ_fit = profile σ(s_ND) ⊕ band MC-stat ⊕ data Poisson; spread = max |f_ND(7091 or nd4cat) − f_ND(7071)|; σ_G2 = σ_fit ⊕ spread;
δ_edge = Σ_cell f_ND^band · ρ, ρ = max |data/prediction − 1| over the [3.0, 3.4) 0.05 bins with ≥ 20 data in the unit.
`flip with δ_edge` is per chain row: (f_ND ≤ 0.30) ≠ (f_ND + δ_edge ≤ 0.30). Source `D/data/restart_2026-10-27/wp-a6/f_nd_per_mx.csv`.

| M_X bin | chain | f_ND | in-fit | band | σ_fit | spread | σ_G2 | δ_edge | f_Reg | N_data^SEL | largest | > 0.30 | masked_G2 | flip with δ_edge |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| MX4_10 | 7071 | 0.015 | 0.009 | 0.006 | 0.023 | 0.033 | 0.041 | 0.024 | 0.032 | 1817 | nd4cat | False | False | False |
| MX4_10 | 7091 | 0.042 | 0.026 | 0.016 | 0.039 | 0.033 | 0.052 | 0.008 | 0.045 | 1817 | nd4cat | False | False | False |
| MX4_10 | nd4cat | 0.048 | 0.023 | 0.025 | 0.021 | 0.033 | 0.040 | 0.013 | 0.044 | 1817 | nd4cat | False | False | False |
| MX10_20 | 7071 | 0.131 | 0.033 | 0.098 | 0.083 | 0.242 | 0.255 | 0.076 | 0.054 | 6950 | 7091 | False | True | False |
| MX10_20 | 7091 | 0.372 | 0.094 | 0.278 | 0.074 | 0.242 | 0.253 | 0.113 | 0.046 | 6950 | 7091 | True | True | False |
| MX10_20 | nd4cat | 0.240 | 0.061 | 0.179 | 0.084 | 0.242 | 0.256 | 0.111 | 0.049 | 6950 | 7091 | False | True | True |
| MX20_40 | 7071 | 0.283 | 0.032 | 0.250 | 0.124 | 0.114 | 0.168 | 0.108 | 0.086 | 6732 | nd4cat | False | True | True |
| MX20_40 | 7091 | 0.169 | 0.025 | 0.143 | 0.096 | 0.114 | 0.149 | 0.037 | 0.087 | 6732 | nd4cat | False | True | False |
| MX20_40 | nd4cat | 0.332 | 0.041 | 0.291 | 0.134 | 0.114 | 0.176 | 0.116 | 0.080 | 6732 | nd4cat | True | True | False |

G2 by M_X bin (largest chain, central value):

| M_X bin | largest chain | f_ND | σ_G2 | δ_edge | G2 | ruling flips with δ_edge |
|---|---|---|---|---|---|---|
| [4, 10) | nd4cat | 0.048 | 0.040 | 0.013 | pass (retained) | no |
| [10, 20) | 7091 | 0.372 | 0.253 | 0.113 | masked_G2 | no (already masked) |
| [20, 40] | nd4cat | 0.332 | 0.176 | 0.116 | masked_G2 | no (already masked) |

No bin's ruling flips with δ_edge. Row-level flags are set on nd4cat [10, 20) and 7071 [20, 40], which are not the largest
chain in their bin. The even split gives the same ruling for [4, 10) and [20, 40] and passes [10, 20) (largest nd4cat 0.201).
The [10, 20) ruling therefore differs between the splits (see Where the result sits).

## Per-unit scales (odd split, diagnostic toy p)

| chain | cell | M_X class | s_diff | s_ND (−/+) | dev/ndf | dev/ndf excl. zero-MC bins | toy p (1000) | scales used for f_ND |
|---|---|---|---|---|---|---|---|---|
| 7071 | D0 | MX4_10 | 1.103 | 0.000 (−0.000/+0.725) | 110.5/78 | 110.5/78 | 0.704 | fallback_all |
| 7071 | D0 | MX10_20 | 0.758 | 0.706 (−0.706/+0.905) | 270.5/78 | 110.7/75 | 0.449 | unit |
| 7071 | D0 | MX20_40 | 0.878 | 1.522 (−0.889/+1.028) | 718.7/78 | 54.6/73 | 0.312 | unit |
| 7071 | D0 | MXout | 6.920 | 1.371 (−0.965/+2.068) | 426.5/78 | 100.4/74 | 0.985 | fallback_all |
| 7071 | D0 | all | 0.836 | 0.374 (−0.374/+0.708) | 179.5/78 | 179.5/78 | 0.031 | unit |
| 7071 | D1 | MX4_10 | 2.018 | 0.891 (−0.545/+0.613) | 477.8/78 | 92.8/75 | 0.639 | fallback_all |
| 7071 | D1 | MX10_20 | 0.761 | 1.728 (−0.898/+0.885) | 1283.7/78 | 68.8/68 | 0.012 | unit |
| 7071 | D1 | MX20_40 | 1.138 | 1.208 (−0.561/+0.615) | 591.0/78 | 49.0/71 | 0.310 | unit |
| 7071 | D1 | MXout | 8.795 | 2.358 (−0.829/+1.075) | 1797.1/78 | 51.7/58 | 0.792 | fallback_all |
| 7071 | D1 | all | 1.152 | 0.000 (−0.000/+0.116) | 562.4/78 | 162.3/77 | 0.227 | unit |
| 7071 | D2 | MX4_10 | 5.658 | 1.322 (−0.478/+0.611) | 206.7/78 | 46.9/75 | 0.974 | fallback_all |
| 7071 | D2 | MX10_20 | 3.085 | 1.260 (−0.307/+0.287) | 581.5/78 | 43.3/70 | 0.427 | unit |
| 7071 | D2 | MX20_40 | 0.335 | 2.458 (−0.299/+0.300) | 461.0/78 | 28.3/72 | 0.014 | unit |
| 7071 | D2 | MXout | 0.000 | 2.049 (−0.355/+0.402) | 872.8/78 | 17.9/63 | 0.001 | fallback_all |
| 7071 | D2 | all | 1.907 | 1.344 (−0.324/+0.290) | 612.8/78 | 70.9/71 | 0.244 | unit |
| 7091 | D0 | MX4_10 | 1.103 | 0.000 (−0.000/+0.571) | 110.5/78 | 110.5/78 | 0.693 | unit |
| 7091 | D0 | MX10_20 | 0.673 | 2.417 (−0.783/+0.754) | 264.0/78 | 104.2/75 | 0.280 | unit |
| 7091 | D0 | MX20_40 | 0.999 | 0.000 (−0.000/+0.777) | 771.3/78 | 54.0/72 | 0.176 | unit |
| 7091 | D0 | MXout | 5.305 | 1.717 (−0.626/+0.904) | 367.7/78 | 94.8/75 | 0.930 | unit |
| 7091 | D0 | all | 0.855 | 0.000 (−0.000/+0.542) | 179.7/78 | 179.7/78 | 0.022 | unit |
| 7091 | D1 | MX4_10 | 1.817 | 1.143 (−0.510/+0.568) | 380.1/78 | 104.4/76 | 0.327 | unit |
| 7091 | D1 | MX10_20 | 0.537 | 3.935 (−0.678/+0.684) | 749.0/78 | 89.4/72 | 0.012 | unit |
| 7091 | D1 | MX20_40 | 0.863 | 2.938 (−1.313/+1.341) | 591.4/78 | 49.4/71 | 0.141 | unit |
| 7091 | D1 | MXout | 2.722 | 6.850 (−1.573/+1.635) | 1003.5/78 | 84.2/66 | 0.474 | unit |
| 7091 | D1 | all | 0.757 | 2.607 (−0.558/+0.560) | 184.9/78 | 184.9/78 | 0.295 | unit |
| 7091 | D2 | MX4_10 | 0.537 | 1.393 (−0.544/+0.309) | 106.1/78 | 52.8/77 | 0.473 | fallback_all |
| 7091 | D2 | MX10_20 | 2.742 | 2.216 (−0.398/+0.393) | 265.2/78 | 52.1/74 | 0.301 | unit |
| 7091 | D2 | MX20_40 | 0.468 | 4.304 (−0.562/+0.665) | 93.1/78 | 39.8/77 | 0.694 | unit |
| 7091 | D2 | MXout | 13.333 | 1.265 (−0.317/+0.385) | 677.1/78 | 35.2/67 | 0.002 | fallback_all |
| 7091 | D2 | all | 0.715 | 2.610 (−0.296/+0.288) | 284.6/78 | 65.0/76 | 0.109 | unit |
| nd4cat | D0 | MX4_10 | 1.103 | 0.000 (−0.000/+0.383) | 110.5/78 | 110.5/78 | 0.688 | unit |
| nd4cat | D0 | MX10_20 | 0.738 | 1.160 (−0.985/+0.906) | 269.6/78 | 109.8/75 | 0.436 | unit |
| nd4cat | D0 | MX20_40 | 0.844 | 1.695 (−1.003/+1.121) | 722.2/78 | 58.1/73 | 0.207 | unit |
| nd4cat | D0 | MXout | 6.218 | 1.822 (−1.028/+1.197) | 428.3/78 | 102.1/74 | 0.901 | unit |
| nd4cat | D0 | all | 0.855 | 0.000 (−0.000/+0.353) | 179.7/78 | 179.7/78 | 0.031 | unit |
| nd4cat | D1 | MX4_10 | 1.740 | 2.218 (−0.704/+0.781) | 326.0/78 | 107.4/76 | 0.440 | unit |
| nd4cat | D1 | MX10_20 | 0.565 | 3.643 (−0.522/+0.523) | 505.5/78 | 119.4/75 | 0.036 | unit |
| nd4cat | D1 | MX20_40 | 1.043 | 1.746 (−0.674/+0.726) | 545.2/78 | 56.5/72 | 0.233 | unit |
| nd4cat | D1 | MXout | 8.457 | 3.053 (−0.772/+0.885) | 1495.6/78 | 72.5/63 | 0.684 | unit |
| nd4cat | D1 | all | 0.984 | 1.172 (−0.336/+0.378) | 217.7/78 | 217.7/78 | 0.328 | unit |
| nd4cat | D2 | MX4_10 | 0.398 | 2.027 (−0.079/+0.420) | 107.1/78 | 53.9/77 | 0.297 | fallback_all |
| nd4cat | D2 | MX10_20 | 2.167 | 1.856 (−0.260/+0.267) | 115.6/78 | 62.4/77 | 0.531 | unit |
| nd4cat | D2 | MX20_40 | 0.162 | 2.749 (−0.289/+0.275) | 202.8/78 | 43.1/75 | 0.018 | unit |
| nd4cat | D2 | MXout | 0.000 | 2.289 (−0.823/+0.266) | 624.6/78 | 36.0/68 | 0.001 | fallback_all |
| nd4cat | D2 | all | 0.738 | 2.035 (−0.256/+0.238) | 200.0/78 | 93.5/76 | 0.101 | unit |

## Edge diagnostic

Per-bin data, prediction and ratio over [3.0, 3.4) for every (chain, cell, class): `D/data/restart_2026-10-27/wp-a6/edge_diag_odd.csv`.
The band is not described: ρ is 0.24-2.0 in the [10, 20) and [20, 40] units (largest in D0), and 0.9-7.6 in the sparse [4, 10) units, where
the band holds little ND (f_ND^band ≤ 0.025). The post-fit stacks are in `D/plots/restart_2026-10-27/wp-a6/odd/stacks/`.

## Stability (even split)

Table in `fit_record.md` (`D/data/restart_2026-10-27/wp-a6/stability_even.csv`). Flags (shift above the nominal σ_G2): `lo1p0` and
`nobb` in [4, 10) and [10, 20) for all three chains (shifts +0.10 to +0.37; in [4, 10) the `lo1p0` f_ND rises to 0.145-0.279),
`hi2p8` 7071 [4, 10), `shared` 7091 and nd4cat [10, 20). `xip04` is unflagged. None of the variants is added to σ_G2.

## Where the result sits

- The band dominates: 75-90 % of f_ND in [10, 20) and [20, 40] comes from η04 ≥ 3.0, outside the fit, where data/MC is 1.2-1.5
  for every chain (LAr-edge response, `D/docs/restart_2026-10-27/wp-a2/s2_diagnostic_record.md`). This is the plan's first risk.
- The per-unit fits are poorly constrained. s_diff spans 0-13 and s_ND 0-6.9 across units. In D2 the fit trades DIFF8 against
  ND (s_diff 0.16-0.47 against s_ND 2.5-4.3 in [20, 40]), and the even/odd values differ by more than the chain spread.
- The retained [4, 10) bin is the one with the least ND and the most fallback units (7071 falls back in all three cells). Its pass
  depends on the low-η04 bins: the `lo1p0` and `nobb` variants raise its f_ND to 0.15-0.29 while keeping it under 0.30.

### `h1-mc-expert` forward-flow reading (spread 0.242 > 3 σ_fit of 7091 in [10, 20), 0.221; Decision 13 trigger)

Most of the chain spread comes from the fitted normalisation s_ND, not from differences in the ND generators' gap probability.
In M_X 10-20 (odd split) the band/in-fit ratio is almost the same for all three chains (7071 3.0, 7091 2.9, nd4cat 3.0). So the
three ND tails have nearly the same shape across eta04 2.6-3.3, and the factor 2.8 spread in f_ND (0.13-0.37) is a spread in s_ND
alone. A different gap model would show up as different shapes. Only in M_X 20-40 do the ratios differ (7.7 / 5.6 / 7.1), which
leaves a modest generator-shape effect there. Two more signs point away from the generator. First, the odd/even swings are as
large as or larger than the chain spread (7091: 0.37 to 0.09; 7071 at 20-40: 0.28 to 0.06), and they match the s_diff/s_ND
degeneracy in D2: the fit trades DIFF8 against ND. Second, about 75-90% of f_ND sits in the 3.0-3.3 band. That band is outside
the fit, so it is extrapolated, not measured, and it carries the common data/MC excess of 1.2-1.5 from the LAr-edge simulation.
The identity of the 7091 generator is not checked (PROVISIONAL). Credible upper bound: nd4cat. It covers every period, its f_ND
is large in both splits (10-20: 0.24/0.20; 20-40: 0.33/0.42) without the split-dependent swings of 7071 and 7091, and it is the
largest chain in most rows.

## Purity table and mask (WP-C proposal grid: frame B, x_IP < 0.03, D7+)

`D/data/restart_2026-10-27/wp-a6/purity/binstats_wpa6_D7plus_B_xip030.csv`, from 7071 ND and 6671/6677 Reggeon track builds
(`D/data/restart_2026-10-27/wp-a6/trk/`) scaled with the odd-split resolved scales. purity + f_mig + f_fake + f_ND + f_Reg = 1 per bin
(max deviation 1.1e-8). Of 30 bins (2 charges × 2 axes): 24 masked by G2 (every [10, 20) and [20, 40] bin), 19 by D7+ on the new
columns (WP-C table: 20), 26 in the union. Retained: [4, 10) x_F [−1, 1] and p_T*² [0, 0.75] for both charges (purity
0.427-0.433, f_ND 0.013-0.015, f_Reg 0.023-0.025, f_fake 0.207-0.211). Median purity 0.362 against 0.450 in the WP-C table. Mask
`purity/mask_wpa6_D7plus_B_xip030_2026-10-28.json` (D7+, via `make_blind_mask.py`) and the G2 overlay
`purity/mask_wpa6_g2_overlay_2026-10-28.json` (final list = D7+ ∪ G2). WP-C files unchanged (sha256 checked). Caveat: the track
build selects E−p_z 35-70 and `etaMaxClus08` < 3.2 (`wpc_build.py:97,100-101`), the event-level f_ND uses 3.3; harmonisation is
WP-B item W2. Tracks of events outside D0-D2 (5-11 %) take the ALL-scope scale of their M_X class.

## Deviations from the pre-registration

1. Selection edge 3.3 (frozen WP-A2 `SB` mask, brief) instead of the WP-A6 block's η_max(0.8) < 3.2 (Decision 6); WP-F re-derives
   f_ND on WP-B's final edge.
2. Purity keeps WP-C's D7 migration definition, so the identity closes with an f_mig term rather than purity = 1 − f_ND − f_Reg −
   f_fake (Decision 9).
3. The fit set adds the per-cell all-M_X units and ALL-scope (cells concatenated) units per class: 60 units per split instead of
   36. The all-M_X units are the Decision 3 fallback; the ALL-scope units only scale tracks of events outside the D cells
   (Decision 9 names no rule for them).
4. Zero-MC bins are reported (deviance with and without them); no rule was pre-registered.
5. The Condor tally was compiled by the lead from `condor_history` (`D/data/restart_2026-10-27/wp-a6/condor_tally.csv`), not by
   `h1-naf-operator`.
6. ρ fallback not registered in Decision 6: in units with no [3.0, 3.4) bin holding ≥ 20 data (D1 and D2 [4, 10), all three
   chains; 6 units), ρ = |Σ data / Σ prediction − 1| over the unit's 8 bins (the script's log calls it "class-summed"). It enters
   δ_edge of [4, 10) only and changes no flip flag (largest-chain f_ND + δ_edge = 0.061).
7. The six signal track rebuilds agree with the WP-C npz exactly in every integer/boolean column and to ≤ 1.8e-6 relative in
   floats (CPU-dependent float32 arithmetic); the bit-exact check passes for four of six labels.

## Budget

Total 0.033 Condor-days (2812 job-seconds, 390 jobs, including 12 jobs of two discarded build submissions), of which the track
builds took 0.005. Budget ≤ 0.7 (≤ 0.5 for the track builds).

## Records (sha256 at verdict time)

| file (under `D/data/restart_2026-10-27/wp-a6/`) | sha256 |
|---|---|
| `range_frozen.json` | `6eb1405e3e6457665924df4085d201198378d2ca920d98af990a4b277f97b7b4` |
| `f_nd_per_mx.csv` | `6268013032511409bd0a7cc08539391f72f2180effe6115ceaf02a6f0f19a160` |
| `stability_even.csv` | `c93ea365602061f8b6c83a870e504c33c3567cd63342df872e8c7118f9cd5cbd` |
| `unit_table_odd.csv` | `5430ccacc1231ea70fb7244780d31c7e41c99d09dd5e1e0661a369895f5effe0` |
| `unit_table_even.csv` | `2e4974f5e5b20e004d95b2d1d20eb030f46dcf201876d2676dbb3e7400b17688` |
| `scales_resolved_odd_nominal.csv` | `22c7cb9e0087382007504a730bfa9f763d3aa633e91e702593d8b87f2a63f3da` |
| `purity/binstats_wpa6_D7plus_B_xip030.csv` | `614bce1d148e367896945c82b114187f83fae906f2191532c284a51c0d4ec34a` |
| `purity/mask_wpa6_D7plus_B_xip030_2026-10-28.json` | `4baa819e160d7d548eb18efe8071265b3857af8acdd402f6156b85af88c34904` |
| `purity/mask_wpa6_g2_overlay_2026-10-28.json` | `a86087f070fe1bfb5f6eb9c89e0b8258e38daad475dd169e8b13a7e35b714394` |
| `condor_tally.csv` | `c2b160b5ed0ad5b061a5a0e746c736d3e8aa8ad30d5c85601720998411cbf87f` |
| `payloads/payload_sha256.txt` | `cf388f74534c9c3e4f1ddc2499bb1c3b31cb8295e6e387c4c02dbced1690a165` |

## Open items

- G2 with the band term: WP-B's remedy (selection edge at η04 < 3.0 or a tighter x_IP window) would remove the extrapolated band
  that carries 75-90 % of f_ND in [10, 20) and [20, 40]. Open Q1 option (b) for WP-F applies if a flip flag is set on the largest
  chain; none is set here.
- The [10, 20) ruling is split-dependent (even pass, odd masked); WP-F re-confirms on the new production.
- The retained [4, 10) bin rests on fallback scales and the thin η04 < 1 bins (`lo1p0`, `nobb` shifts).
- 7091 generator identity not verified (`h1-mc-expert`, PROVISIONAL).

## Ruling (`h1-analysis-critic`)

RULING: PASS with masks. Under the pre-registered G2 reading (ceiling 0.30 on the central f_ND of the largest ND chain, odd
split, σ_G2 = fit ⊕ generator spread quoted beside it), M_X [4, 10) is retained (nd4cat 0.048 ± 0.040, δ_edge 0.013); [10, 20)
(7091 0.372 ± 0.253) and [20, 40] (nd4cat 0.332 ± 0.176) are masked_G2. No ruling flips with δ_edge and at least one M_X bin is
retained, so the restart proceeds. (`h1-analysis-critic`, 2026-10-28)

- WP-F re-confirms G2 on the new production and WP-B's final selection edge; if any largest-chain flip flag is set there, Open Q1(b)
  (δ_edge added to σ_G2) applies.
- WP-F shows [4, 10) stays ≤ 0.30 without BB-lite and with lower edge 1.0 (here 0.15-0.29, all 7071 [4, 10) units on fallback
  scales); any variant above 0.30 masks the bin. σ_G2/f_ND ≈ 0.83 there, so only the absolute cap of the G2 justification holds.
- Purity and leakage budgets for retained bins use the largest-chain f_ND (nd4cat) or the 7071 value with the full relative spread,
  not the bare 7071 value (0.013-0.015) of the purity table.
- [10, 20) stays masked unless WP-F unmasks it on the new production with the 7091 identity confirmed by `h1-mc-expert`; the
  even-split pass is not grounds to unmask.
- WP-B item W2 aligns the 3.2 (track build) and 3.3 (event-level) selection edges before WP-F re-rules.
- Final mask = D7+ ∪ G2 (`purity/mask_wpa6_g2_overlay_2026-10-28.json`); retained: x_F [−1, 1] and p_T*² [0, 0.75], both
  charges, M_X [4, 10).

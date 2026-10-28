# WP-A5 gate G1″(a) verdict: forward-veto signal-loss correction

Date 2026-10-28. Package WP-A5 of `.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`; child plan
`.supervisor/working/plans/2026-10-28-wp-a5-veto-signal-loss-correction.md` (decisions A5-D1…D11). `D` =
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. The structure follows `D/docs/restart_2026-10-27/wp-a/g1_verdict.md`.

**Result: PASS.** All 37 (frame A) and 34 (frame B) measurement bins that the D7 rule keeps have σ_rel^veto ≤ 0.05.
The bins above 0.05 are already D7-masked: three in frame A, two in frame B, one of them empty in each frame. The largest
retained value is 0.049 in frame A (h+, M_X 10-20, x_F bin 5) and 0.035 in frame B. The ruling by `h1-analysis-critic` is in §8: PASS in both frames, with nine bins flagged `veto_eta_fragile`.

## 1. Pre-registered gate text (verbatim, parent plan lines 205-219)

> - **G1″ (WP-A5 and WP-A6; replaces G1′ as the campaign gate, user 2026-10-28, D12-D14).** Efficiency-based, on
>   the existing WP-A3 skims, signed-v3 weights and the fine G1′(c) scales:
>   (a) **Veto signal loss (WP-A5).** On the deep-gap N−1 view (`etaMaxClus08` < 2.5, the other two vetoes
>   applied; DIFF8 purity 0.83-0.97 across the three η_max bins), the data−MC fail-probability difference per
>   detector and per (M_X bin, D cell) enters as the MC removal probability q of D14 with systematic
>   max(|q|/2, odd/even spread). Acceptance: the propagated relative systematic on (1/N_D) dn/dx_F and
>   (1/N_D) dn/dp_T*² is ≤ 0.05 in each measurement bin on the WP-C start grid; a bin above it is masked. Validation: the integrated
>   Plug/FTS/FMD N−1 fail fractions reproduce fig2 and `veto_eff.json` (odd Plug D0/D1/D2 data 0.2724 /
>   0.2996 / 0.3365; deep-gap Plug Δf +0.065…+0.067) to 1e-6.
>   (b) FMD and FTS N−1 shapes: no longer gated; their fail-probability differences are part of (a).
>   (c) **Leakage (WP-A6).** The two-template η_max fit on `etaMaxClus04` is confined to η_max < 3.0 (0.05 bins,
>   no merged edge bin; the LAr-edge 3.15-3.4 region is excluded as simulation response, WP-A4 §1.1); toy GOF
>   on the restricted range is reported (≥ 1000 toys) as a diagnostic, not gated; the ND-generator spread over
>   7071 / 7091 / nd4cat is the systematic; the gate is G2.
>   (d) The MC-statistics gate (D7 mask rule, WP-C) and the ≥ 5× signal-MC request (Open Q6) are unchanged.

Open user questions run with the plan defaults (supervisor brief 2026-10-28):
- **Q-A:** σ_q = max(|q|/2, |q_even − q_odd|) (D14). σ_stat is reported per cell and does not enter σ_q.
- **Q-B:** the nominal q is constant in η_max. Where the η-shape term alone exceeds 0.05, both forms are tabulated and the
  bin is masked under the constant nominal.

## 2. Inputs and provenance

- Sample: the 17 WP-A3 skims `D/data/restart_2026-10-27/wp-a3/skims/<label>.npz`. Masks are `fact_wpa3.masks`, applied
  N−1 with the other two vetoes. The deep gap is etaMaxClus08 ∈ [−1, 2.5). The selection is A_pre with no x_IP cut,
  as in `gate_evidence/veto_eff_hist.py` (see §7, item 1). MC uses signed-v3 weights × fac_used/table_fac. MC "full" is
  s_diff(cell)·DIFF8 + s_ND(cell)·ND(chain), with the fine G1′(c) scales from `veto_eff.json` `fit_scales_fine`. The
  nominal chain is 7071.
- Definition: q_v = (f_data − f_MC,full)/(1 − f_MC,full) for each veto v ∈ {Plug, FTS, FMD}. A passing MC signal event
  gets the weight Π_v(1 − q_v). The D14 rule gives σ_q = max(|q_even|/2, |q_even − q_odd|).
- Histogram job: Condor cluster 5931280, 31 s wall, 1 CPU, 733 MB peak, request 2 GB. It integrates to
  `gate_evidence/veto_eff_hists.json` with a maximum relative deviation of 2.0e-12 over 152 arrays.
- Cell-attach jobs: cluster 5931281, six jobs, 19 s wall in total. All invariants hold: ec08 < 3.2 or sentinel,
  (run, event) unique, `DiffTot.mx[e_idx] == e_mx`.
- **Budget:** two clusters, 50 s × 1 CPU = 5.8e-4 Condor-days. The plan estimated ≈ 0.01, and the stop limit was 0.1.
- Freeze: `D/docs/restart_2026-10-27/wp-a5/freeze_record.md`. The even JSON was written at 06:17:41Z, frozen at
  06:17:56Z and made read-only. The odd split was first read at 06:17:59Z. The even sha256 is
  `c026604e29a02f854c10dd2c773f47534d5b807c3a10eccdfa10687327f52dad`.
- Downstream interface: `D/scripts/restart_2026-10-27/wp-a5/veto_weight.py`, with `load()` and
  `weights(mx, cell, ec08, variant, detectors)`. It reads `veto_correction.json` and returns the same weight for
  frames A and B. Fallbacks: cell −1 takes the ALL-cell q of its M_X bin, M_X outside [4,40] takes the ALL-M_X q of its own cell, and
  an event with both takes ALL|ALL. An unknown detector suffix in a variant name raises an error.

Skim sha256 (as recorded by the histogram job):

| label | sha256 |
|---|---|
| ep0304 | `89b1930ff0cba2239372444dc8d2b54ba3d72eb363ca3c94db7521a857867e02` |
| em0405 | `93ae0e927c3a4cf8b84db8e26d28e949fb4818e06f7f7be4d77f419a0ee7cd18` |
| em06_true | `9425ef3f709def0aa9cf9c45d330ad1aa6a8f22af80f3b6491c79aeca6da5b28` |
| ep0607_true | `a631d2a2830bac0347f7ff64781d91767db524e96475e955a7147eee422eb948` |
| 6671 | `812b8f8feacd0d0122054fc77a5f54bdde37a1145011cf8250780819b762155b` |
| 6677 | `efd9fa772f8fe1dba20272e07f333a46f2a61da9faf446c340b4cf600e9fbd0f` |
| 7071 | `e8fdaaef71e9698dce6280b290985101b7e899af70546e0a90e9c29e143984b2` |
| 7091 | `6f8532af78cdc0c43f09edebbadcb47107235698ae6686b9a602169587b61a27` |
| 7094 | `203e23dde018f09a9982116b1bf6e006d2684e6539268d61e0d3d6fc44bcbcd6` |
| 7096 | `b1fbe9c3cc4cea6e450218bc6fb13b763239429f087fc083fd1a003c33e689bb` |
| 7097 | `51dc743103b1cde62d28c8a482c094e837520cb844f99f75836806542c09e13e` |
| 9012 | `d62838e9ade1853c04c524e887cb159ab32a0577e295b34459f7721a7a38f3f9` |
| 9024 | `a0f6facf0d73dbc7b802abe1a8863d8945c3b5271144b6ce853cda48a5c06a93` |
| 9033 | `387ba32ccae224ac7a25275afcdfa9f3e23c78acf17d37d32a2a54421e7d55bd` |
| 9155 | `1a815a6dc7fffbe3d3b845b27159612a0261c18b4693bfde2fab92383b410fc9` |
| 9157 | `00e1a51c8403300b856540a91855b39476e53f259afd1ccc81d107fa2147bbae` |
| 9158 | `9868d0fe2c47bf0e18300a197ac4a7f8bf09484f0ede5151884a26ca2c7d0173` |

Scripts, Condor files and outputs (sha256 at the time of writing):

| file | sha256 |
|---|---|
| `D/scripts/restart_2026-10-27/wp-a5/attach_cells.py` | `55ad2d2628cb211e47dd69b29807beaf6845697b4f05d17ce2de8939eb812dd4` |
| `D/scripts/restart_2026-10-27/wp-a5/derive_veto_corr.py` | `eaabc9b199a1d96947c3d78cb92922a69b2d91facc45abac8e14273127dcdb70` |
| `D/scripts/restart_2026-10-27/wp-a5/diagnostics_wpa5.py` | `acac0562fa97f4204822987764a30f5577d8cb5bf7e1fd486c24c09e39200b7a` |
| `D/scripts/restart_2026-10-27/wp-a5/plot_wpa5.py` | `721a75cf7a5debefabcd0441ce4e12cf2ee6e7f054b43aaee5c79b2d4ae412ae` |
| `D/scripts/restart_2026-10-27/wp-a5/propagate_wpa5.py` | `ffd7873cfe5704a8b9ac19a45e3b00f8dba197acea8525724c2efc3873e6998c` |
| `D/scripts/restart_2026-10-27/wp-a5/tests_wpa5.py` | `e0fb3065e5ecd5b6163d5de841c4b5a429d9477c95927631f5f2656c9a4553de` |
| `D/scripts/restart_2026-10-27/wp-a5/veto_corr_hist.py` | `feb40020fdb39590a57e0415fa8f564c3d5fbebd45bc4b24bea1c305e3310772` |
| `D/scripts/restart_2026-10-27/wp-a5/veto_weight.py` | `d9693af0d2331e3dca7d34feb455cfb1e8161ab3cadf2a5bfa2400ac3f75010d` |
| `D/condor/restart_2026-10-27/wp-a5/attach_cells.sub` | `b05060ff171e2ce0d5f88d5bc54798430848919df73d77eb11fa6fd00a218c0d` |
| `D/condor/restart_2026-10-27/wp-a5/veto_corr_hist.sub` | `dee5ba0975b172a1b00ee3fd79ef262369f838d85c0c98548b97f896ae522721` |
| `D/condor/restart_2026-10-27/wp-a5/wpa5_job.sh` | `d98327526425e16c9d5c52225807da84dd3a5138917a880003c5a12bc7a06763` |
| `D/data/restart_2026-10-27/wp-a5/veto_corr_hists.json` | `6b3e0e7825cb2ce049b77f0cfe5fc46384f0036c76ae4627eceacdbebc41a418` |
| `D/data/restart_2026-10-27/wp-a5/veto_correction_even.json` | `c026604e29a02f854c10dd2c773f47534d5b807c3a10eccdfa10687327f52dad` |
| `D/data/restart_2026-10-27/wp-a5/veto_correction.json` | `cff5c00ad579546b0a144830e361f2acb250d384669fe3226f13deae53b9dea7` |
| `D/docs/restart_2026-10-27/wp-a5/freeze_record.md` | `d6ec620024b4e3e27244ad88434e0d806fca1e40c0d3a0e79027065966ecb1c7` |
| `D/data/restart_2026-10-27/wp-a5/g1pp_a_table.csv` | `3cc1ac125cbef0e175e4cc719668dc7fb2e952c9d491c292c2918ebaa0d9d1cb` |
| `D/data/restart_2026-10-27/wp-a5/g1pp_a_table.json` | `6d4258fc69f118b44b6511f502a649bcafd89f838f2863316f52323eacfc8a70` |
| `D/data/restart_2026-10-27/wp-a5/diagnostics.json` | `6cb98e03e7faab6abb8ae5a618adce62e9fb99d0d86546e2fd34b872d3f7cac1` |
| `D/data/restart_2026-10-27/wp-a5/cells_9012.npz` | `c432ffb158ea0a1e7e0cb3aa9363961cfa2c10e22eb00255abb4b5e574bfffa3` |
| `D/data/restart_2026-10-27/wp-a5/cells_9024.npz` | `d84aa6798e60f4d3be163190388abe982f2efec926e35e9b562b6b994f577213` |
| `D/data/restart_2026-10-27/wp-a5/cells_9033.npz` | `a08def19dbfe33c2f00134adaf5fc21a4ac06ff9986dcf73a497a45daa22e499` |
| `D/data/restart_2026-10-27/wp-a5/cells_9155.npz` | `4fd31a725152ab84bb7ab328c3eeb3ede00e5a78883a14de6c101998c4deb0a5` |
| `D/data/restart_2026-10-27/wp-a5/cells_9157.npz` | `a1e226bec552fcdfb5b080f3d462bfa9065bc7ca91be12ebb26ab011ee8b74a1` |
| `D/data/restart_2026-10-27/wp-a5/cells_9158.npz` | `16a30dea4f956e05edc62b50799967a4c572de1e06812fa9ab3c1636fa8a0609` |

## 3. Correction table

This table gives the deep-gap N−1 values per detector × M_X bin × D cell. The nominal q is from the even split and is
frozen. σ_q is the D14 systematic. The purity is the DIFF8 share of the 7071 full MC. The q values for 7091 and nd4cat
are the chain spread and are diagnostic only (A5-D2).

| detector | M_X [GeV] | cell | q (even, nominal) | q odd | spread | abs(q)/2 | σ_q | σ_stat even | n_data even | purity | q 7091 | q nd4cat |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| PLUG | 4-10 | D0 | +0.049 | +0.059 | 0.010 | 0.024 | **0.024** | 0.009 | 1242 | 0.96 | +0.046 | +0.048 |
| PLUG | 4-10 | D1 | +0.065 | +0.073 | 0.009 | 0.032 | **0.032** | 0.023 | 329 | 0.87 | +0.060 | +0.060 |
| PLUG | 4-10 | D2 | +0.137 | +0.080 | 0.058 | 0.069 | **0.069** | 0.067 | 43 | 0.49 | +0.142 | +0.134 |
| PLUG | 10-20 | D0 | +0.060 | +0.068 | 0.008 | 0.030 | **0.030** | 0.006 | 1903 | 0.93 | +0.059 | +0.059 |
| PLUG | 10-20 | D1 | +0.079 | +0.065 | 0.015 | 0.040 | **0.040** | 0.013 | 776 | 0.88 | +0.082 | +0.080 |
| PLUG | 10-20 | D2 | +0.176 | +0.105 | 0.071 | 0.088 | **0.088** | 0.043 | 175 | 0.68 | +0.174 | +0.172 |
| PLUG | 20-40 | D0 | +0.089 | +0.065 | 0.025 | 0.045 | **0.045** | 0.017 | 608 | 0.89 | +0.088 | +0.088 |
| PLUG | 20-40 | D1 | +0.066 | +0.086 | 0.020 | 0.033 | **0.033** | 0.019 | 320 | 0.84 | +0.076 | +0.071 |
| PLUG | 20-40 | D2 | +0.115 | +0.120 | 0.005 | 0.057 | **0.057** | 0.039 | 129 | 0.50 | +0.149 | +0.126 |
| FTS | 4-10 | D0 | +0.037 | +0.008 | 0.029 | 0.018 | **0.029** | 0.013 | 1307 | 0.96 | +0.028 | +0.035 |
| FTS | 4-10 | D1 | +0.011 | +0.057 | 0.046 | 0.005 | **0.046** | 0.027 | 333 | 0.85 | -0.005 | +0.011 |
| FTS | 4-10 | D2 | +0.013 | -0.252 | 0.265 | 0.007 | **0.265** | 0.068 | 40 | 0.48 | -0.054 | -0.019 |
| FTS | 10-20 | D0 | +0.079 | +0.069 | 0.010 | 0.039 | **0.039** | 0.010 | 2042 | 0.93 | +0.072 | +0.074 |
| FTS | 10-20 | D1 | +0.047 | +0.055 | 0.009 | 0.023 | **0.023** | 0.020 | 797 | 0.88 | +0.044 | +0.038 |
| FTS | 10-20 | D2 | +0.053 | +0.048 | 0.006 | 0.027 | **0.027** | 0.051 | 167 | 0.66 | +0.053 | +0.036 |
| FTS | 20-40 | D0 | +0.029 | +0.081 | 0.052 | 0.014 | **0.052** | 0.026 | 613 | 0.86 | +0.060 | +0.044 |
| FTS | 20-40 | D1 | +0.029 | +0.103 | 0.074 | 0.015 | **0.074** | 0.026 | 328 | 0.81 | +0.050 | +0.033 |
| FTS | 20-40 | D2 | -0.044 | +0.049 | 0.092 | 0.022 | **0.092** | 0.074 | 126 | 0.49 | +0.008 | -0.029 |
| FMD | 4-10 | D0 | +0.015 | -0.071 | 0.086 | 0.008 | **0.086** | 0.022 | 1478 | 0.88 | -0.021 | +0.014 |
| FMD | 4-10 | D1 | -0.073 | -0.063 | 0.009 | 0.036 | **0.036** | 0.050 | 404 | 0.70 | -0.182 | -0.116 |
| FMD | 4-10 | D2 | -0.096 | -0.624 | 0.528 | 0.048 | **0.528** | 0.172 | 64 | 0.35 | -0.257 | -0.326 |
| FMD | 10-20 | D0 | +0.047 | +0.047 | 0.001 | 0.023 | **0.023** | 0.019 | 2404 | 0.83 | +0.039 | +0.042 |
| FMD | 10-20 | D1 | +0.007 | -0.052 | 0.059 | 0.004 | **0.059** | 0.032 | 1002 | 0.69 | +0.049 | +0.009 |
| FMD | 10-20 | D2 | -0.108 | -0.291 | 0.183 | 0.054 | **0.183** | 0.088 | 232 | 0.42 | -0.002 | -0.170 |
| FMD | 20-40 | D0 | +0.006 | -0.060 | 0.067 | 0.003 | **0.067** | 0.038 | 777 | 0.72 | +0.070 | -0.019 |
| FMD | 20-40 | D1 | -0.122 | -0.190 | 0.068 | 0.061 | **0.068** | 0.056 | 403 | 0.63 | -0.003 | -0.160 |
| FMD | 20-40 | D2 | -0.353 | -0.292 | 0.062 | 0.177 | **0.177** | 0.125 | 183 | 0.24 | +0.028 | -0.359 |

Fallback rows (ALL cells or ALL M_X, even split, q ± σ_q):
- Plug: 4-10|ALL +0.054±0.027, 10-20|ALL +0.072±0.036, 20-40|ALL +0.086±0.043, ALL|ALL +0.069±0.034.
- FTS: 4-10|ALL +0.031±0.020, 10-20|ALL +0.067±0.034, 20-40|ALL +0.022±0.062, ALL|ALL +0.048±0.024.
- FMD: 4-10|ALL −0.009±0.084, 10-20|ALL +0.018±0.016, 20-40|ALL −0.072±0.053, ALL|ALL −0.006±0.043.

The η-shape variant (A5-D4) uses δ_v = q_v,ALL(η bin) − q_v,ALL(deep gap), from ALL cells and all M_X. The even row is
the one applied. Events in "other" (η < −1 or sentinel) use δ[−1,1). The DIFF8 purity per bin for Plug runs
0.97/0.91/0.84/0.67/0.50/0.37; for FMD it runs 0.92/0.77/0.62/0.39/0.23/0.14.

| detector | [-1,1) | [1,2) | [2,2.5) | [2.5,3.0) | [3.0,3.2) | [3.2,3.3) | other |
|---|---|---|---|---|---|---|---|
| Plug even | -0.039 | +0.006 | +0.024 | +0.079 | +0.165 | +0.219 | -0.013 |
| Plug odd | -0.029 | -0.001 | +0.024 | +0.097 | +0.157 | +0.223 | +0.070 |
| FTS even | +0.007 | -0.006 | +0.001 | +0.016 | +0.015 | -0.012 | +0.007 |
| FTS odd | -0.018 | +0.009 | +0.008 | +0.022 | -0.008 | -0.018 | -0.004 |
| FMD even | +0.058 | -0.003 | -0.034 | -0.194 | -0.358 | -0.540 | +0.197 |
| FMD odd | +0.032 | +0.031 | -0.037 | -0.124 | -0.343 | -0.525 | +0.202 |

Figures: `D/plots/restart_2026-10-27/wp-a5/fig1_q_per_cell.{pdf,png}`, `fig2_eta_shape.{pdf,png}` and
`fig3_syst_per_bin.{pdf,png}`. Captions are in `D/plots/restart_2026-10-27/wp-a5/captions.md`.

## 4. Validation tests

`python3 D/scripts/restart_2026-10-27/wp-a5/tests_wpa5.py --slice all` passes all tests (exit 0).

| test | result | numbers |
|---|---|---|
| `fig2_consistency` | PASS | odd Plug N−1 data f_fail D0/D1/D2 = 0.2724/0.2996/0.3365, within 1e-6 of `summary.json` |
| `deep_gap_reproduced` | PASS | Plug ALL odd Δ(7071) = 0.065389, n = 5755. The 96 deep-gap Δ values (4 views × 2 parities × 4 cells × 3 chains) agree with `veto_eff.json` to 1e-15 |
| `even_frozen_first` | PASS | even 06:17:41Z ≤ freeze 06:17:56Z < odd 06:17:59Z (verification re-run 06:22:55Z); sha256 matches |
| `syst_floor` | PASS | every one of the 48 cells has σ_q ≥ abs(q)/2 and σ_q ≥ spread |
| `no_control_1p5` | PASS | no η_max < 1.5 cut and no remedy-1 term in any wp-a5 script |
| `join_keys` | PASS | six labels: 2503/5970/4176/4161/2108/3835 events, all unique, all with ec08 < 3.2 or sentinel |
| `zero_variant_identity` | PASS | the zero variant equals the unweighted spectrum exactly (118 non-empty bins). Σ w per M_X = 3765.5/9606.3/5840.1, equal to the skim e_w·s_diff sums |
| `weight_total` | PASS | the 19 variants are finite for synthetic edge cases (cell −1, M_X NaN/2/50, sentinel and NaN η) and for all six skims |
| `g1pp_a_table` | PASS | 120 rows (2 frames × 2 charges × 3 M_X × 10 bins) |
| `period_table` | PASS | 4 periods × 3 detectors × 2 parities, χ² with ndf 3 |
| `comb_closure` | PASS (written) | see §6; outside 2σ, so this is an open item |
| `verdict_sections` | PASS | the eight A5-D11 sections and both mask lists are present |
| verification re-run | PASS | `derive_veto_corr.py --split odd` and `propagate_wpa5.py` re-run: `veto_correction.json` is identical apart from `meta.timestamp`, and `g1pp_a_table.csv` is byte-identical |

## 5. Per-bin table and mask list

The table is `D/data/restart_2026-10-27/wp-a5/g1pp_a_table.csv`, with a JSON copy holding meta and summary. It uses the
row keys `charge, mx_bin, axis, bin` plus `frame, S_nom, R_up, R_down, R_eta, syst_veto_rel, boot_sigma, masked_D7,
masked_veto`, and joins to `mask_start_frame{A,B}_G2_D7.json`. Extra columns:
- `R_nom_vs_uncorr`: the size of the nominal correction itself.
- `syst_veto_rel_etaform`, `masked_veto_etaform`: the Q-B alternative.
- `syst_veto_rel_eta_quad`, `masked_veto_eta_quad`: a diagnostic, see §6.
- `veto_eta_fragile`: a flag, not a mask. It marks a bin that passes the gate but exceeds 0.05 under the quadrature
  form or under any single-detector η term (`h1-systematics` review). Downstream packages carry
  `syst_veto_rel_eta_quad` next to `syst_veto_rel` for these bins.
- Per-detector `R_up_/R_down_/R_eta_/R_nominal_<det>`.
- `n_tracks_raw`.

The propagation sample is the six WP-C signal skims: 22 753 events, event weight e_w × s_diff(cell, 7071).
σ_rel = sqrt(max(|R_up|, |R_down|)² + R_eta²).

| | frame A | frame B |
|---|---|---|
| bins / D7-masked / retained by D7 | 60 / 23 / 37 | 60 / 26 / 34 |
| `masked_veto` (all / not already D7-masked) | 3 / **0** | 2 / **0** |
| retained by D7 and G1″(a) | 37 | 34 |
| max σ_rel in D7-retained bins | 0.0491 (h+ 10-20 x_F bin 5, boot 0.0037) | 0.0346 (h+ 10-20 x_F bin 4) |
| median σ_rel in D7-retained bins | 0.0031 | 0.0031 |
| max coherent ±σ_q term (D7-retained) | 0.0067 | 0.0072 |
| max abs(R_eta) (D7-retained) | 0.0489 | 0.0346 |
| max σ_rel, p_T*² axis (D7-retained) | 0.0044 | 0.0050 |
| max abs(nominal correction) (D7-retained) | 0.030 | 0.018 |
| `veto_eta_fragile`, D7-retained | 6 | 3 |
| Q-B: bins with abs(R_eta) alone > 0.05 | 1 (h+ 20-40 x_F 5, −0.052, D7-masked) | 1 (h+ 20-40 x_F 5, −0.066, D7-masked) |
| Q-B: masked under the η-stepped form | 1 (h− 20-40 x_F 5, empty) | 1 (h− 20-40 x_F 5, empty) |

**Mask list, frame A** (`masked_veto`): h+ 20-40 x_F bin 5 (σ_rel 0.055); h− 10-20 x_F bin 5 (0.051); h− 20-40
x_F bin 5 (empty bin). All three are already D7-masked.

**Mask list, frame B** (`masked_veto`): h+ 20-40 x_F bin 5 (0.066); h− 20-40 x_F bin 5 (empty bin). Both are already
D7-masked.

Under Q-B, the one bin per frame where the η-shape term alone exceeds 0.05 is D7-masked, so the choice between the two
forms does not change the mask in either frame. The η-stepped form gives σ_rel ≤ 0.033 in every non-empty bin.

**Fragile bins** (`veto_eta_fragile`, D7-retained):
- Frame A: h+ 4-10 x_F 5; h+ 10-20 x_F 4 and 5; h+ 20-40 x_F 4; h− 4-10 x_F 5; h− 10-20 x_F 4.
- Frame B: h+ 10-20 x_F 4; h− 10-20 x_F 4; h+ 20-40 x_F 0.

The frame-A bin h+ 10-20 x_F 5 passes at 0.0491 ± 0.0037 (bootstrap) on 32 raw tracks, so it is statistically at the
threshold.

The ±σ_q variations shift q by a constant within each (M_X, cell). Within an M_X bin that changes only the D-cell mix, and
in a per-event-normalised spectrum the change largely cancels. The coherent same-sign shift is not strictly the
conservative choice; the `h1-systematics` review gives the measured bounds. In D7-retained bins the ±σ_q term is
0.0067/0.0072 coherent (A/B), 0.0047/0.0111 per-detector in quadrature and 0.0069/0.0137 per-detector linear. The
review estimates at most ≈ 0.015 for opposite-sign shifts across cells. So the term is ≤ 0.014 under every
detector combination that was evaluated. All the larger values come from the η-shape term, at
x_F > 0.2 for M_X 10-40.

## 6. Diagnostics

`D/data/restart_2026-10-27/wp-a5/diagnostics.json`.

- **Odd closure.** The pull is (q_odd − q_even)/σ_stat over the 9 (M_X, cell) cells.
  - Plug: χ² 6.1/9 (p 0.73), max |pull| 1.4.
  - FTS: χ² 15.5/9 (p 0.08), max |pull| 2.1.
  - FMD: χ² 15.2/9 (p 0.09), max |pull| 2.5.
- **Flatness.** χ² of q about the weighted mean over the 9 cells.
  - Plug: even 16.8/8 (p 0.03), odd 4.8/8. Over the D cells, even 11.2/2 (D2 high).
  - FTS: even 14.1/8, odd 22.5/8 (p 0.004).
  - FMD: even 22.7/8, odd 45.2/8.
  - q is not flat in (M_X, cell), which supports the 3 × 3 step function (A5-D3).
- **Period stability.** Deep-gap data f_fail per period (ep0304 / em0405 / em06 / ep0607), all cells and all M_X:

| detector | parity | ep0304 | em0405 | em06 | ep0607 | χ²/ndf | p | > 3σ |
|---|---|---|---|---|---|---|---|---|
| Plug | even | 0.068±0.009 | 0.089±0.007 | 0.102±0.010 | 0.084±0.006 | 7.4/3 | 0.06 | — |
| Plug | odd | 0.090±0.010 | 0.081±0.007 | 0.093±0.009 | 0.088±0.006 | 1.2/3 | 0.74 | — |
| FTS | even | 0.131±0.011 | 0.128±0.008 | 0.114±0.010 | 0.119±0.007 | 1.9/3 | 0.59 | — |
| FTS | odd | 0.142±0.012 | 0.128±0.008 | 0.125±0.010 | 0.118±0.007 | 3.1/3 | 0.37 | — |
| FMD | even | 0.277±0.014 | 0.250±0.009 | 0.237±0.012 | 0.305±0.009 | 27.9/3 | 3.8e-06 | ep0607 |
| FMD | odd | 0.254±0.014 | 0.258±0.009 | 0.244±0.013 | 0.299±0.008 | 18.8/3 | 3.0e-04 | ep0607 |

  The FMD fail rate in ep0607 is above the other periods in both parities. Under A5-D8(iii) this is an open item and not
  a gate. No q is derived per period (D14).
- **Chain spread.** The largest |q(7091 or nd4cat) − q(7071)| over the 9 cells (even) is 0.034 for Plug, 0.067 for FTS and
  0.381 for FMD (D2 × 20-40, where 7091 gives +0.03 against −0.35 from 7071). FMD q depends on the ND model wherever the
  purity is low. This is reported only and does not enter σ_q (A5-D2).
- **COMB closure (A5-D9).** Deep-gap COMB (fail = any veto), 7071 full MC; f′ is the corrected MC.

| split | cell | f_data | f_MC uncorr. | f′ corrected | pull uncorr. | pull corr. |
|---|---|---|---|---|---|---|
| even | D0 | 0.474±0.006 | 0.400 | 0.480 | −7.1 | +0.5 |
| even | D1 | 0.529±0.009 | 0.536 | 0.566 | +0.4 | +2.3 |
| even | D2 | 0.671±0.015 | 0.711 | 0.719 | +1.7 | +2.0 |
| even | ALL | 0.506±0.005 | 0.474 | 0.531 | −3.9 | +3.1 |
| odd | D0 | 0.474±0.006 | 0.425 | 0.503 | −4.7 | +2.7 |
| odd | D1 | 0.548±0.009 | 0.559 | 0.586 | +0.7 | +2.4 |
| odd | D2 | 0.657±0.015 | 0.749 | 0.747 | +4.0 | +3.9 |
| odd | ALL | 0.510±0.005 | 0.499 | 0.553 | −1.4 | +5.2 |

  After the correction the MC overshoots the combined data fail fraction by 2-5σ. This is an **open item**. The
  multiplicative combination Π_v(1 − q_v) treats the three vetoes as independent. If the Plug and FTS excesses come from
  the same forward activity, the product over-removes. The COMB purity is 0.58 and its D1/D2 MC is already above the data
  before any correction, so this is a diagnostic (A5-D9). It does not bias the gate quantity, because every variant R uses
  the same product form. It does bias the nominal correction: about 45 % over-correction on the even split (ALL) and
  about 80 % on the odd split. In retained bins this is bounded by |R_nom_vs_uncorr| ≤ 0.030. Open item for WP-E/F/H,
  where absolute pass rates and the N_D normalisation enter and the product rule no longer cancels.
- **η-shape cancellation.** The joint η variant applies all three δ_v together, so the Plug term (positive at high x_F)
  and the FMD term (negative) partly cancel. In h+ 10-20 x_F bin 5 (frame A), R_eta is +0.073 for Plug alone, −0.127 for
  FMD alone and −0.049 for all three. If the per-detector η terms are added in quadrature instead
  (`syst_veto_rel_eta_quad`), six D7-retained bins exceed 0.05 in frame A and three in frame B. All of them are
  x_F bins 4-5 at M_X 4-40, except B h+ 20-40 x_F bin 0 (0.051). The FMD δ at η_max ≥ 2.5 rests on DIFF8 purity
  0.39-0.14, which is the regime the plan's A5-D4 rationale already calls unattributable. This is the main judgement
  call for the ruling.
- **x_IP window (parent step (1)).** Adding 0 < x_IP < 0.04 to the deep-gap view keeps 92 % of the Plug data. ALL|ALL
  q changes by −0.008 (Plug), +0.007 (FTS) and +0.025 (FMD). Per cell, the largest changes are in D2: Plug −0.106 at
  10-20|D2 and FMD +0.285 at 20-40|D2, against σ_q of 0.088 and 0.177.
- **Q-A flags.** σ_stat,even exceeds σ_q in four cells: FTS 10-20|D2 (0.051 vs 0.027), FTS ALL|D2 (0.035 vs 0.011), FMD
  4-10|D1 (0.050 vs 0.036) and FMD 10-20|ALL (0.016 vs 0.016). The plan's flagged cell, Plug D2 × [4,10), has
  σ_stat 0.067 against σ_q 0.069, so it is covered only because the spread happens to be large.

## 7. Deviations from the pre-registration

1. **No x_IP cut in the derivation sample.** Parent WP-A5 step (1) lists x_IP < 0.04 and M_X > 3 GeV. The child plan
   keeps the `veto_eff_hist.py` masks, which have no x_IP cut, so that the 1e-6 validations against `veto_eff.json`
   can hold. M_X > 3 is implied by the [4,40] grid. The x_IP-window q is reported as a diagnostic in §6.
2. **Reggeon omitted from the propagation** (A5-D6). Reggeon (6671/6677) has no WP-C track skim. Parent step (4) names
   the DIFF8 reco events. The correction is still defined for all of DIFF8 downstream, through `veto_weight.py`.
3. **Additions to the histogram job.** The job also writes per-period data arrays (input to A5-D8(iii), so that
   diagnostics do not read the skims on the login node) and `|xip04` copies (item 1). Neither is used for q.
4. **Condor files.** The `.sub` files and the wrapper are in `D/condor/restart_2026-10-27/wp-a5/`, with logs in
   `logs/`, as the child plan says. One wrapper `wpa5_job.sh` serves both clusters.
5. **Extra table columns** beyond the A5-D11 set: `R_nom_vs_uncorr`, the Q-B `*_etaform` pair, the `*_eta_quad`
   diagnostic and the per-detector R columns. The plan's `R_zero` reading ("R == 0") is implemented as an identity
   check: the zero variant against an independently weighted spectrum.

None of these changes q, σ_q or the gate quantity as the child plan defines them.

## 8. Ruling

Ruling: PASS for G1″(a) in both frames (`h1-analysis-critic`, 2026-10-28). Frame A keeps 37 of 37 D7-retained bins
(max σ_rel 0.0491) and frame B keeps 34 of 34 (max 0.0346). The `masked_veto` bins are already D7-masked: three in
frame A and two in frame B, one of them empty in each. Every M_X bin keeps retained bins, so the kill condition is not met.

**Systematic review (`h1-systematics`, 2026-10-28): acceptable with recorded caveats.**
- (1) The coherent ±σ_q shift is not strictly conservative. It is bounded at ≤ ≈ 0.014 under every detector combination
  evaluated (§5).
- (2) Concern: the joint η variant passes in x_F bins 4-5 only because the Plug and FMD terms cancel. These bins are
  flagged `veto_eta_fragile` (§5).
- (3) The COMB over-correction does not bias the gate ratio. It biases the nominal correction by ≤ 0.030 (§6).
- (4) The threshold bin A h+ 10-20 x_F 5 sits at 0.0491 ± 0.0037.

The code critic returned PASS. Its one SHOULD finding (variant-name validation in `veto_weight.py`) and three NITs are fixed.

The ruling rests on these points:
- The criterion is applied exactly. A5-D5/A5-D4 define σ_rel with the joint q_η variant, so the joint form is the gate
  quantity and the quadrature form is a diagnostic.
- Q-B changes no mask: the one bin per frame where |R_eta| alone exceeds 0.05 is D7-masked.
- Q-A cannot change a mask, because the ±σ_q term is ≤ 0.007 in retained bins.
- The PASS assumes the frozen q, σ_q and the table are correct as implemented. That rests on the code-critic PASS and the
  byte-identical verification re-run. The critic reviewed the verdict, the plans and the CSV. It did not recompute §3/§6
  from `veto_correction.json` or `diagnostics.json`.

Binding downstream:
1. **Fragile bins.** These pass only because the Plug (+) and FMD (−) η terms cancel:
   - Frame A: h+ 4-10 x_F 5; h+ 10-20 x_F 4 and 5; h+ 20-40 x_F 4; h− 4-10 x_F 5; h− 10-20 x_F 4.
   - Frame B: h+ 10-20 x_F 4; h− 10-20 x_F 4; h+ 20-40 x_F 0.

   Downstream reports both forms for these bins. It never quotes a veto uncertainty below `syst_veto_rel_eta_quad` in any
   combination that undoes the cancellation.
2. **Threshold bin.** A h+ 10-20 x_F 5 is re-evaluated on any new production (WP-F) or re-frozen C1 frame, and is not
   grandfathered.
3. **COMB closure.** The independence assumption of Π_v(1 − q_v) is resolved or budgeted wherever absolute pass rates or
   the N_D normalisation enter (WP-E/F/H).
4. **A5-D5 wording.** "Coherent is the conservative choice" is replaced by the measured bound of ≤ ≈ 0.014.
5. **Carried, not gated.** The FMD ND-chain spread (up to 0.381) and the FMD ep0607 period offset go to WP-A6/G2 as
   context.

Nothing goes to the user now. The fragile-bin list and caveat 1 are stated explicitly in the Q4 campaign-authorisation
return. Recommended next check, in WP-A6 (`h1-systematics`): whether the FMD δ_v at η_max ≥ 2.5 holds across 7071, 7091
and nd4cat. If it moves by the chain spread, the fragile bins are masked.

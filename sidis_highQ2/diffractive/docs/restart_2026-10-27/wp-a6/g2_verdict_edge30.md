# WP-A6 gate G2 at the selection edge etaMaxClus08 < 3.0 (user D15)

Date 2026-10-28. Wave 1d of `.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`; child plan `.supervisor/working/plans/2026-10-28-wp-a6-leakage-refit.md`. `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Original verdict: `D/docs/restart_2026-10-27/wp-a6/g2_verdict.md` (gate text, readings, definitions of σ_fit, spread, σ_G2, δ_edge and the flip flag; all unchanged here).

**Mechanical result: G2 passes in all three M_X bins (odd split, and the even split too).** This is the lead's re-evaluation under the pre-registered reading, not an `h1-analysis-critic` ruling.

## Reused and changed

- Reused, no refit: the frozen odd and even nominal unit fits (`fits/<split>/nominal/`, `range_frozen.json` 6eb1405e…) with the Decision 3 resolution (`fnd_wpa6.resolve_all`). Also reused: `fnd_wpa6.fnd`/`edge` and the frozen payloads' `cluster` mode for ρ (the fit sample is unchanged, so ρ per unit is unchanged), the 7071 nominal chain, and the 0.30 ceiling.
- Changed: SEL = SB30 = `SB` ∧ (ec08 < 3.0 or sentinel), with the other SB terms (Plug/FMD/FTS vetoes) and x_IP < 0.04, M_X > 3, D0-D2 kept. In-fit is ec04 < 3.0. The residual band is ec08 < 3.0 ∧ ec04 ≥ 3.0, which is now the only extrapolated piece.
- New SEL payloads: `D/data/restart_2026-10-27/wp-a6/edge30/payloads/<split>_<chain>_sel30.json` (`build_sel30_wpa6.py`, which reuses `build_payload_wpa6.block/close`). **Closure:** the SB block rebuilt there reproduces the frozen `f_nd_per_mx.csv` exactly (max relative difference 0, both splits, every column).

## Per-M_X table (odd split, SB30)

| M_X bin | chain | f_ND | in-fit | band | σ_fit | spread | σ_G2 | δ_edge | f_Reg | N_data^SB30 | largest | > 0.30 | masked_G2 | flip with δ_edge |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| MX4_10 | 7071 | 0.013 | 0.010 | 0.003 | 0.019 | 0.027 | 0.033 | 0.011 | 0.028 | 1683 | nd4cat | False | False | False |
| MX4_10 | 7091 | 0.037 | 0.029 | 0.008 | 0.032 | 0.027 | 0.042 | 0.004 | 0.039 | 1683 | nd4cat | False | False | False |
| MX4_10 | nd4cat | 0.040 | 0.025 | 0.015 | 0.017 | 0.027 | 0.032 | 0.008 | 0.038 | 1683 | nd4cat | False | False | False |
| MX10_20 | 7071 | 0.092 | 0.053 | 0.040 | 0.054 | 0.151 | 0.160 | 0.029 | 0.037 | 4305 | 7091 | False | False | False |
| MX10_20 | 7091 | 0.243 | 0.152 | 0.091 | 0.044 | 0.151 | 0.157 | 0.037 | 0.031 | 4305 | 7091 | False | False | False |
| MX10_20 | nd4cat | 0.169 | 0.098 | 0.071 | 0.052 | 0.151 | 0.159 | 0.042 | 0.033 | 4305 | 7091 | False | False | False |
| MX20_40 | 7071 | 0.185 | 0.088 | 0.097 | 0.071 | 0.059 | 0.093 | 0.041 | 0.048 | 2482 | nd4cat | False | False | False |
| MX20_40 | 7091 | 0.126 | 0.069 | 0.057 | 0.057 | 0.059 | 0.082 | 0.015 | 0.048 | 2482 | nd4cat | False | False | False |
| MX20_40 | nd4cat | 0.221 | 0.111 | 0.110 | 0.080 | 0.059 | 0.099 | 0.043 | 0.045 | 2482 | nd4cat | False | False | False |

G2 by M_X bin (largest chain), with the edge cost and the residual band:

| M_X bin | N_data SB30 / SB | odd: chain, f_ND ± σ_G2, δ_edge | odd G2 (3.3 → 3.0) | even: chain, f_ND ± σ_G2 | even G2 (3.3 → 3.0) | band data frac (odd / even) | band MC frac, DIFF8 / ND (odd) |
|---|---|---|---|---|---|---|---|
| [4, 10) | 1683 / 1817 (0.93) | nd4cat 0.040 ± 0.032, 0.008 | pass → pass | 7091 0.051 ± 0.119 | pass → pass | 0.098 / 0.114 | 0.07 / 0.23-0.37 |
| [10, 20) | 4305 / 6950 (0.62) | 7091 0.243 ± 0.157, 0.037 | masked → pass | nd4cat 0.127 ± 0.062 | pass → pass | 0.237 / 0.238 | 0.13-0.14 / 0.37-0.43 |
| [20, 40] | 2482 / 6732 (0.37) | nd4cat 0.221 ± 0.099, 0.043 | masked → pass | nd4cat 0.269 ± 0.230 | masked → pass | 0.349 / 0.343 | 0.26 / 0.45-0.53 |

Even-split N_data SB30 / SB is 1693/1842, 4336/6911 and 2533/6647. The band fractions are data events (or scaled MC) in ec04 ≥ 3.0 divided by all SB30 events in the bin. The band carries 23-53 % of f_ND, compared with 75-90 % at 3.3 in [10, 20) and [20, 40]. No ruling flips with δ_edge: the largest f_ND + δ_edge is 0.280 in odd [10, 20) and 0.299 in even [20, 40] (nd4cat, 0.001 below the ceiling). The flag `g2_plus_sigma_flag` (f_ND + σ_G2 > 0.30) is set for the largest chain in odd [10, 20) and [20, 40] and in even [20, 40].

## Purity and the D7+ ∪ G2 mask (WP-C proposal grid, frame B, x_IP < 0.03)

The track builds (cut at ec08 < 3.2) were cut at ec08 < 3.0 or sentinel, with ec08 attached per event and joined on e_idx. Signal ec08 comes from `wp-a5/cells_<label>.npz`. Reggeon/ND ec08 was read from DiffTot by `attach_ec08_wpa6.py` (63 parts; all checks hold: ec08 < 3.2, M_X matches). No rebuild was needed. Events kept: signal 17 362/22 753, 6671 952/1842, 6677 902/1757, 7071 4482/8790. Scales are the frozen `scales_resolved_odd_nominal.csv`; G2 input is `edge30/f_nd_per_mx_SB30.csv`. Both edges are now at 3.0, which resolves the WP-B W2 caveat for this edge.

| edge | D7+ pass | G2-masked M_X | retained (D7+ ∪ G2) | purity of retained |
|---|---|---|---|---|
| 3.3 (frozen) | 11 / 30 | [10, 20), [20, 40] | 4: M_X [4, 10), x_F and p_T*² bin 0, both charges | 0.427-0.433 |
| 3.0 | 18 / 30 | none | 18: [4, 10) 4; [10, 20) 6; [20, 40] 8 | 0.365-0.511 (median 0.421 over all 30 bins) |

Retained bins at 3.0 are x_F bin 0 at [4, 10) and [10, 20), x_F bins 1-2 at [20, 40], p_T*² bin 0 at [4, 10), p_T*² bins 0-1 at [10, 20), and p_T*² bins 1-2 at [20, 40], each for both charges. Files are under `D/data/restart_2026-10-27/wp-a6/purity/`: `binstats_wpa6_D7plus_B_xip030_edge30.{csv,json}` (685e866b…, fbf8c49b…), `mask_wpa6_D7plus_B_xip030_2026-10-28_edge30.json` (0db7e78d…), and `mask_wpa6_g2_overlay_2026-10-28_edge30.json` (839b4e95…), each with a `.sha256` sidecar.

## Records, budget, open items

- Scripts are in `D/scripts/restart_2026-10-27/wp-a6/edge30/`. Condor files are in `D/condor/restart_2026-10-27/wp-a6/edge30/` (clusters 5952214 with 69 jobs and 5952587 with 1 job, all rc 0, 278 job-seconds = 0.0032 Condor-days). Tables: `edge30/f_nd_per_mx_edge30.csv` (SB and SB30, both splits, band columns). Figure: `D/plots/restart_2026-10-27/wp-a6/edge30/fnd_per_mx_edge30.{pdf,png}`. The frozen WP-A6 files are unchanged (sha256 re-checked).
- The original ruling's binding conditions are not lifted by this mechanical pass: "[10, 20) stays masked unless WP-F unmasks it … with the 7091 identity confirmed". The supervisor or `h1-analysis-critic` decides whether D15 supersedes that condition. 7091 is the largest chain in odd [10, 20).
- σ_G2/f_ND is 0.65 (odd [10, 20)) and 0.45 (odd [20, 40]). The chain spread dominates in [10, 20). The [4, 10) caveats (fallback scales, thin η04 < 1 bins) carry over.

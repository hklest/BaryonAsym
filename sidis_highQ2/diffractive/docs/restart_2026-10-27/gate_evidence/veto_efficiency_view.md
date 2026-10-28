# Forward-veto efficiency view

Date 2026-10-28. No gate, plan, handoff or verdict is changed. `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.
Figures: `D/plots/restart_2026-10-27/gate_evidence/fig6_veto_eff_vs_etamax.png` and `fig7_plug_eff_kinematics.png`.
Every number: `D/data/restart_2026-10-27/gate_evidence/veto_eff.json` (odd and even, ALL and per cell).
Question: the spectra are normalised per event, so an overall veto inefficiency cancels. What matters is how much more often each veto rejects a true large-gap event in data than in MC, and whether that difference depends on kinematics.

## Method
- Samples: the 17 WP-A3 skims, with no new skimming (Condor cluster 5925288, `veto_eff_hist.py`). Each veto is N−1 with the gate masks (`fact_wpa3.masks`: A_pre, etaMaxClus08 < 3.3 or sentinel, the other two vetoes pass). COMB uses the same sample with no veto applied; an event fails if any veto fails.
- Fail definitions. Plug: E_Plug ≥ 7 GeV or no measurement. FMD: not selFmd. FTS: nFts28 > 0, which equals not selFts in all skims.
- MC full = s_diff·DIFF8 + s_ND·ND(chain) per D cell, using the fine G1′(c) scales: for 7071 D0/D1/D2, s_diff 0.80/0.85/0.91 and s_ND 1.61/1.40/1.21.
  These use the same weight basis as `hist_ge.py`; the template sums equal the stored fit inputs. DIFF8-only is the signal + PDISS + Reggeon prediction. Unit-scale values are kept as `f_full_unitscale`. ALL = D0+D1+D2; the 46/51/32/118 odd events outside the cells (Plug/FMD/FTS/COMB) are excluded.
- Errors. Data: binomial. MC: Σw², with σ² = (S2_fail(1−f)² + S2_pass f²)/N². Purity = the DIFF8 share of the full-MC sample in the bin. Pass-efficiency ratio = (1−f_data)/(1−f_MC). This is what a per-event-normalised spectrum sees if it varies with kinematics.
- M_X bins: data tertiles of the deep-gap Plug N−1 sample (both parities, fixed before any fraction): < 10.2, 10.2–16.1, ≥ 16.1 GeV. Flatness: χ² of the three values about their weighted mean (ndf 2). Caveat: the MC errors leave out the scale uncertainty, and the G1′ fits are poor (7071 D0 D/ndf 1290/87).

## Consistency check (acceptance): PASS
Plug N−1, odd, integrated over η_max: data f_fail is 0.2724 / 0.2996 / 0.3365 (D0/D1/D2), and unit-scale MC is 0.062 / 0.083 / 0.113 (7071).
Both equal fig2 (`summary.json`) to 1e-9 for all three chains. The data−MC deltas equal the `hists_ge.json` checks.
The 22 events at η08 < −1 or sentinel are included; restricting to [−1,3.3) changes only D0 data, to 0.2725. With the fitted scales, whole-sample MC is 0.087–0.092.

## Per η_max bin, ALL cells, odd (Δ = data − MC full for 7071 / 7091 / nd4cat; MC, DIFF8-only and purity for 7071)
| veto | bin | f data | f MC | f DIFF8 | Δ (±σ) | data/MC | purity |
|---|---|---:|---:|---:|---|---:|---:|
| Plug | [−1,1) | 0.049 ± 0.005 | 0.011 | 0.009 | +0.038 / +0.036 / +0.038 (0.006) | 4.4 (3.7–4.5) | 0.97 |
| Plug | [1,2) | 0.085 ± 0.006 | 0.020 | 0.013 | +0.065 / +0.068 / +0.064 (0.008) | 4.2 (4.0–5.0) | 0.92 |
| Plug | [2,2.5) | 0.119 ± 0.007 | 0.030 | 0.015 | +0.088 / +0.090 / +0.090 (0.008) | 3.9 (3.9–4.2) | 0.83 |
| Plug | [2.5,3.0) / [3.0,3.3) | 0.210 / 0.354 | 0.054 / 0.127 | 0.024 / 0.068 | +0.155–0.161 / +0.226–0.229 | 3.9 / 2.8 | 0.68 / 0.43 |
| FTS | [−1,1) | 0.105 ± 0.007 | 0.074 | 0.068 | +0.031 / +0.030 / +0.031 (0.010) | 1.41 | 0.96 |
| FTS | [1,2) | 0.134 ± 0.008 | 0.079 | 0.064 | +0.055 / +0.051 / +0.052 (0.011) | 1.70 | 0.91 |
| FTS | [2,2.5) | 0.134 ± 0.007 | 0.080 | 0.044 | +0.054 / +0.056 / +0.052 (0.010) | 1.67 | 0.81 |
| FTS | [2.5,3.0) / [3.0,3.3) | 0.167 / 0.183 | 0.101 / 0.151 | 0.046 / 0.055 | +0.054–0.066 / +0.019–0.032 | 1.66 / 1.21 | 0.66 / 0.41 |
| FMD | [−1,1) | 0.188 ± 0.009 | 0.203 (to 0.223) | 0.157 | −0.014 / −0.035 / −0.016 (0.016) | 0.93 (0.84–0.93) | 0.92 |
| FMD | [1,2) | 0.263 ± 0.009 | 0.276 | 0.123 | −0.014 / −0.002 / −0.009 (0.014) | 0.95 | 0.76 |
| FMD | [2,2.5) | 0.337 ± 0.009 | 0.390 (0.358–0.390) | 0.151 | −0.052 / −0.021 / −0.049 (0.014) | 0.87 | 0.61 |
| FMD | [2.5,3.0) / [3.0,3.3) | 0.436 / 0.559 | 0.519 / 0.707 | 0.155 / 0.238 | −0.046 to −0.090 / −0.135 to −0.151 | 0.84 / 0.79 | 0.40 / 0.17 |
| COMB | [−1,1) / [1,2) / [2,2.5) | 0.369 / 0.498 / 0.597 | 0.330 / 0.467 / 0.597 | 0.24 / 0.21 / 0.22 | +0.039 / +0.030 / −0.000 (7091 +0.006 / +0.050 / +0.040) | 1.12 / 1.07 / 1.00 | 0.85 / 0.62 / 0.44 |

Deep gap (η_max < 2.5), all three chains, odd split, then even split:
- Plug: data 0.087 ± 0.004 against MC 0.020–0.021. Δ is +0.065 to +0.067 ± 0.005 (ratio 4.0–4.3); even +0.067 to +0.068. Purity 0.88–0.90.
- FTS: data 0.125 ± 0.004 against MC 0.078–0.080. Δ is +0.045 to +0.047 ± 0.006 (ratio 1.6); even +0.042 to +0.044. Purity 0.86–0.89.
- FMD: data 0.272 ± 0.005 against MC 0.292–0.306. Δ is −0.020 to −0.034 ± 0.009 (ratio 0.89–0.93); even −0.012 to +0.004, consistent with zero. Purity is 0.73, and the MC fail rate is mostly PDISS and ND physics (DIFF8-only 0.14).
- COMB: data 0.510 against MC 0.476–0.502. Δ is +0.009 to +0.034; even +0.018 to +0.043. Purity 0.58.

## Kinematic dependence, η_max < 2.5 (7071 values; p for 7071 / 7091 / nd4cat)
| veto, split | quantity | D0 | D1 | D2 | M_X < 10.2 | 10.2–16.1 | ≥ 16.1 | p (D cells) | p (M_X) |
|---|---|---:|---:|---:|---:|---:|---:|---|---|
| Plug, odd | Δ | +0.061 ± 0.005 | +0.066 ± 0.010 | +0.095 ± 0.025 | +0.053 | +0.068 | +0.075 | 0.40 / 0.09 / 0.32 | 0.11 / 0.01 / 0.09 |
| Plug, odd | pass-eff. ratio | 0.938 | 0.932 | 0.898 ± 0.026 | 0.946 | 0.931 | 0.923 | 0.30 / 0.05 / 0.23 | 0.08 / 0.008 / 0.07 |
| Plug, even | Δ | +0.059 | +0.072 | +0.138 ± 0.025 | +0.048 | +0.070 | +0.083 | 0.006 / 0.001 / 0.004 | 0.003 / 0.0002 / 0.0008 |
| Plug, even | pass-eff. ratio | 0.940 | 0.926 | 0.854 ± 0.025 | 0.951 | 0.928 | 0.915 | 0.002 / 0.0004 / 0.001 | 0.002 / 0.0001 / 0.0006 |
| FTS, odd / even | pass-eff. ratio | 0.950 / 0.943 | 0.937 / 0.969 | 0.99 / 0.98 | 0.984 / 0.964 | 0.931 / 0.936 | 0.933 / 0.956 | odd ≥ 0.06; even ≥ 0.07 | odd ≤ 0.002; even 0.17 / 0.02 / 0.31 |
| FMD, odd | pass-eff. ratio | 1.009 | 1.081 | 1.37 ± 0.09 | 1.10 | 0.98 | 1.09 | ≤ 0.012 | < 0.001 |
| COMB, odd | pass-eff. ratio | 0.914 | 1.025 | 1.37 ± 0.11 | 1.075 | 0.871 | 1.009 | ≤ 0.034 | < 0.001 |
- D2 is less clean: Plug purity there is 0.51–0.58, against 0.92–0.94 in D0 and 0.83–0.86 in D1. The D2 Plug Δ is +0.095 / +0.117 / +0.097 across the chains. In D2 the ND share is 70–84% for FMD and COMB. The 7091 chain differs from the others there by up to 0.09.

## Remedy-1 control-region removal probability (quoted, not recomputed)
Source: `D/docs/restart_2026-10-27/wp-a/s1_period_split_record.md` (control-region definition), `s2_remedy1_record.md` and `g1_verdict.md`.
Control region: A_pre ∧ η_max(0.8) < 1.5 or sentinel ∧ x_IP < 0.04 ∧ M_X > 3 GeV ∧ EventNumber even.
Removal probability: q = (f_data − f_MC)/(1 − f_MC) of the full stack (any veto fails), chain 7071.

| period | N data | f_data | f_MC | q ± σ | data − MC: Plug / FMD / FTS |
|---|---:|---:|---:|---:|---|
| ep0304 | 569 | 0.390 | 0.278 | 0.155 ± 0.032 | .069 / .054 / .057 |
| em0405 | 1160 | 0.387 | 0.252 | 0.181 ± 0.024 | .071 / .063 / .073 |
| em06 | 603 | 0.380 | 0.254 | 0.169 ± 0.031 | .075 / .065 / .051 |
| ep0607 | 1480 | 0.416 | 0.278 | 0.191 ± 0.024 | .064 / .101 / .036 |
- After remedy 1, on the G1 test sample (data f_fail 0.219 / 0.236 / 0.285), MC f_fail and max |Δ| are: 7071 0.214 / 0.229 / 0.247 (0.037),
  7091 0.210 / 0.223 / 0.239 (0.046), nd4cat 0.213 / 0.232 / 0.242 (0.043). All three fail G1(a).
- As a reference only, the luminosity-weighted q̄ = 0.178–0.180 was applied here as f′ = f + q̄(1 − f). With the A5 attribution of the whole stack excess to Plug, it overshoots the deep-gap Plug data by 0.11 and the COMB data by 0.06–0.08 (`f_full_remedy1`).

## Answer
- **Plug.** In the deep-gap sample (η_max < 2.5; 0.88–0.90 diffractive over all cells, 0.51–0.58 in D2), Plug fires more often in data than in MC by
  +0.065 to +0.067 absolute: 0.087 against 0.021, a ratio of 4.0–4.3 (even split +0.067). Kinematically, the excess rises with M_X
  (+0.053 → +0.075) and toward D2 on both splits. On the odd split this is compatible with flat (p 0.01–0.40). On the even split it is not (p ≤ 0.006).
  The pass-efficiency ratio data/MC varies by about 5% on the odd split (0.946 → 0.898) and about 10% on the even split (0.951 → 0.854).
- **FTS.** It fires +0.045 to +0.047 more often: 0.125 against 0.078, a ratio of 1.6 (even +0.043). Its ratio is flat across D cells. Across M_X it is
  not flat on odd (p ≤ 0.002) but is compatible with flat on even.
- **FMD.** It fires 0.020–0.034 less often on odd (ratio 0.89–0.93) and is consistent with MC on even (−0.012 to +0.004).
- **FMD and COMB.** Both are not flat (D2 pass-efficiency ratio about 1.37 on odd). Their data−MC difference follows the ND and PDISS composition and the
  ND chain, not the signal-event noise alone.

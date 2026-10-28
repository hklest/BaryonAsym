# Low-Q² binning study: synthesis and recommendation (2026-10-22), FOR USER APPROVAL

Inputs:
- Plan: `docs/lowQ2_binning_study_plan_2026-10-22.md`
- Gates: `docs/lowQ2_binning_gates_precedent_2026-10-22.md`
- Results:
  - WS2 (resolution and candidates): `data/lowQ2_binning_ws2_2026-10-22/` (also on NAF)
  - WS3 (occupancy): `data/lowQ2_binning_ws3_2026-10-22/occupancy/`
  - WS4 (unfolding): `data/lowQ2_binning_ws4_2026-10-22/SUMMARY.md`

## What limits the binning
1. **Resolution does not limit it.** σ68:
   - ln Q² 0.045;
   - log10 x 0.053 (0.03 at high y up to 0.085 at low y);
   - z ≈ 0.005, growing ∝ z;
   - pT_h 0.016 GeV, growing with z.
2. **(Q², x) migration limits purity.** Even at width ≥ 2σ the x bands are one atom (0.13 decade) wide, and the 4D diagonal is ≈0.65 in (Q², x). Purity failures concentrate in the last-x (low-y) band of each Q² stratum.
3. **Acceptance shapes the stability definition.** The gen fiducial has no detector cuts (TMD-correct, user decision D2), so 57% of gen-fid hadrons are missed.
   - Stability with the full gen denominator measures acceptance (median 0.25–0.31).
   - Migration-only stability S_mig has median 0.47 (k2p0_q4_ptz) and 0.61 (template).
4. **A kinematic corner is empty.** The first x band (high y) × z ≥ 0.5 × low pT_h has zero acc×eff, which makes R rank-deficient for the global-pT candidates (cond 1e16–1e19).
5. **Statistics:**
   - Data stat per cell is only 0.7–2.6% (median), so data never limits.
   - MC is the limit: Kish n_eff ≈ 2× data. The "MC ≥ 10× data" gate is unattainable. Pull widths of ~1.15–1.2 come from response-MC statistics.
6. **Model dependence:** DJ↔RG cross-closure bias is 1.3–2.3σ (median, 1.7–3.3% relative), far above data stat. It is a systematic (as planned), not a gate.
7. **Convergence:** IBU needs k = 5–8 at data statistics under the 0.2σ convergence rule. The high-Q² nominal k = 4 is not converged here.

## Candidate ranking (h+; h− within ±2%)

| candidate | n_gen | k | purity med | S_mig med | same-gen closure ≤0.5σ | cells failing (S_mig gates) |
|---|---|---|---|---|---|---|
| template | 280 | 8 | 0.51 | 0.61 | 100% | 18% |
| **k2p0_q4_ptz** | 1620 | 5 | 0.40 | 0.47 | 100% | 26%, all structural and local |
| k2p0_q4 | 1134 | 6 | 0.40 | 0.46 | 99% | 33% (rank-deficient corner) |
| candB (old) | 480 | 6 | 0.41 | 0.49 | 100% | 45% |
| k1.5 / k1.0 families | 2048–3960 | 6 | 0.22–0.34 | ≤0.39 | 93–99% | 46–72% |

## Recommendation
**k2p0_q4_ptz**, with local merges:
- **Q² strata:** 4 (5–10–20–50–100). x bands per stratum at ≥ 2σ.
- **z:** first edge at 0.05; reco 1:1 with gen.
- **pT_h:** edges chosen per gen z bin (`_ptz`); reco 2:1 finer than gen.
- **Merges:**
  1. the last-x band into its neighbour in each Q² stratum (218 of the 301 rescued cells);
  2. the empty corner (first-x × z ≥ 0.5 × low pT_h), merged or cut;
  3. a few pT (50) and z (33) pair merges.
- **Status:** 88 of 389 failures remain after one-pair merges. A second iteration (WS2 → WS4 on the merged candidate) is needed before freezing.
- **Why not the template:** it is the cleanest, but it is 6× coarser, and its data refold χ²/ndf ≈ 94 (vs ≈ 10) shows the coarse bins average over MC-shape mismatch.

## Decisions needed (user)
1. **Stability gate definition:** gate on migration-only stability S_mig ≥ 0.3 plus a minimum acc×eff (proposed ≥ 0.05, with cells below that merged or cut), or on the full-denominator stability as written in the precedent?
2. **MC-stat gate:** drop it as a gate and propagate the response-MC statistics as a systematic (the high-Q² `fakecol_mcstat` analogue)? Or request more MC?
3. **IBU k:** choose k by the convergence rule (5 for the recommended candidate), departing from the high-Q² k = 4 for this analysis?
4. **Corner handling:** merge the zero-acceptance corner, or add a documented gen-level kinematic restriction?

## Follow-ups (not binning-blocking)
- **Data/MC normalisation:** F = 0.83 / 0.88 / 0.93 by year (2005/06/07), the same for both generators. Data yield per nb⁻¹ rises across the years while MC is flat, and the MC period mix differs (2005: 26% of MC vs 36% of data). Check the data lumi and S61 trigger efficiency per period, and reweight the MC period mix.
- **Builder:** MC `z_SIDIS_es` uses the truth mass. Fix it at the next rebuild (analysis uses the offline z_comp).
- **Storage:** the h1 group quota is full. The WS3/WS4 outputs are local only until space is freed.

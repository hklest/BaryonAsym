# DIFFVM elastic rho0: the large-fraction bins (2026-10-25)

**Scope.** The final eSigma release of the high-Q² charged-hadron SIDIS measurement, with outputs under `H1ChargedSidis_HighQ2/output_v6/studies/esigma_final_2026-10-24/`. In that release the DIFFVM elastic ρ⁰ sample (diffvm11001, reqIDs 8445–8448, genIDs 5679–5682) is scaled by L_data/L_MC = 350,891/2,581,730 nb⁻¹ = 0.135913. It is not subtracted. It enters only as a ±100% rank-one term (`diffvm_rho_rel`, `diffvm_rho_signed_rel`). This note asks which bins are large, whether that is physical, whether data support the DIFFVM normalisation, and what to do. No release file or document was changed.

Path shorthand: `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`, `R` = `H/output_v6/studies/esigma_final_2026-10-24/downstream`.

## Short answer

- The figure ">40%" is a **reco-level** fraction, and it holds in only **2 of 980 reco bins**. One of those two bins contains a single data track. No unfolded bin exceeds 40%. The largest unfolded `diffvm_rho_rel` is **34.6%**, in h⁻ gen bin 30. Only 2 of 490 output bins exceed 20%.
- The location is physical. Longitudinally polarised ρ⁰ → π⁺π⁻ sends one pion to z ≈ 0.9–0.97 and the other to z ≈ 0.04–0.1, both at low pT_h, in the lowest-Q² cell. The large bins sit exactly there.
- **The data do not support the DIFFVM normalisation.** We selected events with two opposite-charge tracks and z₁+z₂ ≈ 1. The data show **no ρ peak**: the excess is −0.5 ± 11.9 events, where DIFFVM predicts 67.4 ± 3.0. The data therefore contain at most about 0.35× the predicted exclusive ρ⁰ (2σ, all z), and at most about 0.55× in the high-z, low-Q² corner where the large bins sit. The likely cause is that DIFFVM overestimates the yield at Q² > 150 GeV². A data-only filter that removes clean two-track events is not yet excluded.
- **Recommendation:** keep the nominal result unsubtracted. Keep the ±100% term; it is conservative and needs no rerun. Add a footnote for gen bin 30 (both charges), and record this data test as the reason for not subtracting. Details are in §5.

## 1. Which bins

The bin layout is defined in `H/scripts/unfolding/*/cone_s2prime_2026-10-11/binning_v3.py`.

- **Reco bins:** 980 = 2 charges × 7 (Q², x) cells × 7 z bins × 10 pT_h bins. The index is charge×490 + cell×70 + iz×10 + ipt.
- **Gen (output) bins:** 245 per charge, with gen pT edges 0, 0.5, 1, 2, 4, 8 GeV.
- **Data counts:** D = `per_bin.n_raw` in `R/vectors/data_vector_bgsub_setrprime_esph660.json`.
- **DIFFVM counts:** N = `counts_980` and T = 0.135913·N, both in `R/diffvm/diffvm_reco980_esigmaphotonxf_2026-10-24.npz`. In total N = 2196 and T = 298.47.

Reco bins with T/D > 0.40 (verified):

| reco bin | cell | z | pT_h (GeV) | charge | N | T ± δT | D ± √D | T/D | gen bin | unfolded `diffvm_rho_rel` |
|---|---|---|---|---|---|---|---|---|---|---|
| 551 | S0_x0: Q² 150–300, x 0.0021–0.0071 | 0.7–1.0 | 0.25–0.50 | h⁻ | 54 | 7.34 ± 1.00 | 15 ± 3.9 | 0.49 ± 0.14 | h⁻ 30 | 0.346 |
| 690 | S1_x0: Q² 300–700, x 0.0042–0.016 | 0.7–1.0 | 0–0.25 | h⁻ | 3 | 0.41 ± 0.24 | 1 ± 1.0 | 0.41 ± 0.47 | h⁻ 100 | 0.112 |

How many bins are large:

- **Reco level:** 7 bins exceed 20%. Five of them lie between 0.20 and 0.27 (bins 200, 620, 550, 60, 760). All seven have z 0.7–1.0, pT_h < 0.5, and the lowest x in their Q² stratum.
- **Unfolded:** 14 of 490 output bins exceed 5%, 2 exceed 20%, and none exceed 40%.
  - The largest values are h⁻ bin 30 (0.346), h⁺ bin 30 (0.220), bin 31 (0.14 for both charges) and h⁻ bin 65 (0.13).
  - In h⁻ bin 30 the DIFFVM term carries 76% of the variance in the quoted total uncertainty. The total is 39.7%, of which 16.1% is statistical. The unfolded 34.6% rests on 25 background-subtracted data tracks (T = 9.9).
- **Sources:** the budget column is in `R/budget_groupC_v2/groupC_new_columns.csv` and `R/budget_v2/esph_budget_v2.csv`. The term is built in `H/scripts/unfolding/esigma_final_2026-10-24/budget/components_groupC.py:181-187`. That code re-unfolds d ± T with 4 IBU iterations and takes max(|r(−T)|, |r(+T)|).
- **Not propagated (verified):** the DIFFVM MC statistical variance `var_980` is written but never used.

## 2. Is the location physical?

**Verified facts:**

- **z and pT_h.** z = P·h/P·q; pT_h is taken relative to q in the γ*p frame (`H/scripts/studies/ptmax_scan_z005_2026-10-08/build_data_vector_ptmax.py:255-281`).
- **Where the DIFFVM tracks fall in z:**

  | z bin | tracks | share |
  |---|---|---|
  | 0.7–1.0 | 881 | 40% |
  | 0.05–0.2 | 628 | 29% |
  | 0.3–0.7 | 342 | 16% |

- **pT_h and Q² of the DIFFVM tracks.** 57% have pT_h < 0.5 GeV, and 77% are in the lowest Q² stratum (150–300 GeV²).

**Interpretation (h1-mc-expert):**

- **Pion z.** z₁+z₂ = z_ρ ≈ 1. Each pion has z ≈ (1 + β cos θ*)/2 with β ≈ 0.93. At high Q² the ρ⁰ is mostly longitudinal, which gives a cos²θ* decay distribution. The pions therefore pile up near z ≈ 0.04–0.1 and z ≈ 0.9–0.97, not at 0.3–0.7. Only the transverse part fills z ≈ 0.5.
- **pT_h.** pT_h is small, roughly 0.35 GeV from the decay and 0.37 GeV from t.
- **Q² dependence.** σ(γ*p → ρ⁰p) falls roughly as (Q²+M²)^−2.4, much faster than inclusive σ(γ*p), so it concentrates in the lowest Q² cell.
- **Low-x cells.** These are the high-W cells (W ≈ 145–380 GeV in S0_x0), where the elastic cross section is largest.
- **Conclusion.** The pattern is exactly the expected one: large DIFFVM fractions arise where inclusive SIDIS is small (2–25 tracks) and exclusive ρ⁰ is concentrated.

## 3. Data-driven test

**Setup.**

- **Inputs.** Existing v6 trees (`ChargedTot`): 658 data files (7.5 M track rows) and the 404 DIFFVM files.
- **Job.** Condor, 23 jobs, under 1 core-hour.
- **Code and outputs.** Scripts are in `H/scripts/studies/diffvm_mpipi_2026-10-25/`. Outputs are in `H/output_v6/studies/diffvm_mpipi_2026-10-25/`, namely `mpipi_summary.json` and `mpipi_merged.npz`.
- **Event cuts.** The analysis cuts: 150 < Q²_es, 0.1 < y_es < 0.7, E−p_z > 45 GeV, |z_vtx| < 35 cm.
- **Track cuts.** Primary-vertex tracks with pT > 0.15 GeV and |η| < 1.6. An analysis-cut variant agrees with this within 1–2 events.
- **Selections.** A: exactly 2 opposite-charge tracks. B: A plus 0.9 < z₁+z₂ < 1.1, the "exclusive proxy". C: B plus Q² < 300 GeV² and z_max > 0.7, i.e. the large-DIFFVM corner.
- **Mass and background.** M(ππ) uses the pion mass. The peak window is 0.6–0.95 GeV. The background is linear, from sidebands at 0.40–0.55 and 1.05–1.30 GeV.

**Results (verified):**

| selection | data in window | sideband background | data excess | DIFFVM prediction | excess / DIFFVM |
|---|---|---|---|---|---|
| A | 3513 | 3481 ± 64 | 32 ± 87 | 103.4 ± 3.8 | not usable* |
| B | 65 | 65.5 ± 8.7 | −0.5 ± 11.9 | 67.4 ± 3.0 | −0.01 ± 0.18 |
| C | 35 | 27.2 ± 5.6 | 7.8 ± 8.1 | 42.8 ± 2.4 | 0.18 ± 0.19 |

\*In A the inclusive two-track background is large. A flat background instead of a linear one changes the excess from 32 to 632, so A cannot separate a peak.

**What B shows:**

- The DIFFVM events in B peak sharply at 0.69–0.84 GeV. For 492 of the 496 events in the window, both tracks come from a true ρ⁰.
- The data in B are flat to falling, with no structure near 0.77 GeV.
- Even if every data event in the B window were a ρ⁰, the 65 events would only just equal the DIFFVM prediction.
- The 2-track selection is efficient for DIFFVM: 86% of its events that pass the event cuts have exactly 2 selected tracks.

**Inference:**

- **Content bound.** At 2σ, the data contain at most about 0.35× the DIFFVM prediction (B, linear background). With a flat background the B limit is about 0.45×; in the corner selection C it is about 0.55× (C: 0.18 ± 0.19). If DIFFVM is the cause, it is high by a factor of about 2–3 or more.
- **Expected cause.** This fits what h1-mc-expert expected. The ρ⁰ tune of this DIFFVM sample is not recorded. The φ sample (diffvm11000, `exclu_phi/docs/research-diffvm-evaluation.md`) uses a Q² fall-off exponent of 2.0, whereas measurements give about 2.4. Extrapolated from Q² of a few GeV² to Q² > 150 GeV², an exponent like that overestimates by roughly ×4 (provisional).

**Caveats:**

- No HFS-energy or forward-detector veto is possible: these branches are not stored. B requires exactly 2 tracks, so exclusive events with an extra central track are not bounded.
- DIFFVM is used with unit weight; trigger and MC weights are ignored.
- The cause of the overestimate is inferred, not verified. One unchecked possibility is that a data-only stage-1 filter (background finders, trigger) removes clean two-track events.
- A proper exclusive selection needs new tree branches, at about 1060 jobs and roughly 1 hour of wall time.

## 4. How other experiments treat it

| experiment | Q² range | treatment | size and uncertainty |
|---|---|---|---|
| HERMES multiplicities, PRD 87 (2013) 074029, arXiv:1212.5407, Sec. III.4 | > 1 GeV² | Subtracted bin by bin with a PYTHIA VMD model tuned to HERMES ρ⁰ data. "The final tabulation includes data with and without this correction." The highest-z bin is dropped from the figures because of the large VM contribution. | Up to 50% of charged pions near z → 1. 1σ variation of the ρ⁰ cross-section parametrisation. |
| COMPASS, PLB 764 (2017) 001, arXiv:1604.02695, Sec. 3.4 | > 1 GeV², 0.2 < z < 0.85 | Subtracted with HEPGEN ρ⁰, normalised to a phenomenological cross section. Values with and without the correction are public. | About 30% on the cross section gives at most 30% on the correction. |
| COMPASS proton, arXiv:2410.12005, Sec. 3 | > 1 GeV² | Subtracted with HEPGEN ρ⁰ and φ, normalised to the data missing-energy spectrum. | The VM contribution "exceeds the SIDIS cross section itself" at the highest z. About 20% on the VM cross section gives up to 23% on the multiplicity. |
| H1, EPJC 73 (2013) 2406, arXiv:1302.1321; H1, arXiv:2108.12376 and arXiv:2111.11364 (Q² > 150 GeV²) | 5–100 GeV² and > 150 GeV² | No explicit VM treatment was found. The correction uses Djangoh/Rapgap, so the exclusive VM signal is effectively left in the data. | Not quoted. |
| ZEUS scaled momentum, JHEP 2010, arXiv:1001.4026 | > 10 GeV² | No treatment found (keyword search only). | Not quoted. |

The H1 and ZEUS entries come from a keyword search of the full text, not a close read.

## 5. Options

Proton-dissociative ρ⁰ is absent from DIFFVM. Measurements give a proton-dissociative/elastic ratio of about 0.5–0.8 (provisional, from memory). φ, ω, ρ′ and J/ψ are also absent. In principle these raise the true exclusive yield above DIFFVM-elastic. Proton-dissociative ρ⁰ sends the dissociated system beyond |η| < 1.6, so it passes the §3 two-track selection in the same way. The data limit, about 0.35–0.55× DIFFVM-elastic, therefore applies to the combined ρ⁰-like yield, and the upward risk is already covered.

| option | effect | what must be rerun | interaction with the release (installed, not frozen) |
|---|---|---|---|
| **(a) Keep as is (systematic only)** | The nominal includes +T, a bias of one σ of this term (up to 35% in h⁻ bin 30). The data bound the true bias at about 0.35–0.55·T. The ±100% term is therefore conservative. | Nothing. | None. Matches the 2026-10-24 sign-off. |
| **(b) Subtract DIFFVM nominally, ±X%** | Not supported by the data test: it would remove a yield the data do not show, and it would likely over-correct the high-z corner by about 3×. A data-scaled subtraction (at most 0.55·T, ±100%) would be small. | Set `n_subtracted` to `n_subtracted` − kT. Then rerun the release unfolding, groups A, B and C, and the assemble step. No production rerun is needed. The post-hoc form y0·(1 + r(−T)) is exact only for k = 1, because IBU is nonlinear. | Changes the installed release: a new edition and a new user decision. |
| **(c) Exclude or flag the bins** | Flag gen bin 30 (h⁺ and h⁻), where DIFFVM exceeds 20%. Optionally also flag bins 31 and 65, which are above 10%. | Tables and footnotes only. | Documentation only. No numbers change. |
| **(d) Quote with and without** | This is the HERMES/COMPASS practice. Here the "without" column would be the over-corrected DIFFVM subtraction. | Store the r(−T) vector, which is already computed at `components_groupC.py:184`. Needs a group C rerun plus the assemble step. | Adds a published column. Cheap, but it publishes a number the data disfavour. |
| (a′) Reduce the term to the data limit (about ±55% in the corner) | Tighter uncertainty in bins 30, 31 and 65. | Rescale T by about 0.55, then run group C plus the assemble step (budget only, no unfolding of the nominal). | A change to the signed-off budget: needs a user decision. A proper exclusive-selection study should come first. |

**Recommendation:** (a) + (c).

- Keep the nominal unsubtracted and the ±100% term unchanged; no rerun is needed.
- Footnote gen bin 30 (both charges) as dominated by possible exclusive ρ⁰ contamination.
- Record the §3 null result as the reason. In the corner bins the data limit the exclusive ρ⁰ content to at most about half of the DIFFVM prediction.

This leaves the release untouched and stays conservative. Option (a′) is worthwhile only if the high-z corner matters physically, and then only after a proper exclusive selection with forward and HFS vetoes.

## Verified vs inferred

File-backed numbers are marked verified in each section. Inferred or provisional items: the polarisation explanation, the DIFFVM ρ⁰ tune, the proton-dissociative and other VM ratios, the cause of the missing data peak, and the H1/ZEUS literature rows.

## Open questions

1. Rule out the data-only filter (see §3 caveats), e.g. by comparing DIFFVM trigger flags with data subtriggers.
2. Ask h1-mc-db for the DIFFVM ρ⁰ steering (Q² exponent, W dependence, t slope) and whether the inclusive Djangoh/Rapgap MC contains any exclusive ρ⁰.
3. Full exclusive selection with vetoes (§3 caveats).

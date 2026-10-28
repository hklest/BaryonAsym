# Promotion record: NucliaWeight primary-loss uncertainty (2026-10-21)

## 1. Authorization
- **Initial direction:** the user asked to build the column (κ = ±100%, independent of the non-primary int term).
- **Degeneracy check:** the user ruled to keep the overlapping terms ("leave both in to be conservative").
- **Critic gate:** implementation critic PASS-WITH-CONDITIONS; physics critic SOUND-WITH-CONDITIONS. No blockers.
- **User decisions:**
  1. κ = ±100% ratified.
  2. The +0.453% nominal bias is not logged (judged negligible against the 6–7% totals).
  3. Charge-asymmetric/proton loss: per-charge results are covered by the charge split; h+/h−/A_h is zero by
     construction (caveat only).
- **Install:** the user signed off on the covariance edition and the install ("go ahead").

## 2. Installed (additive; sources untouched)
| product | path |
|---|---|
| quoted budget | `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_2026-10-21/` |
| covariance edition | `H1ChargedSidis_HighQ2/data/covariance/z005_covariance_optA_nonprimary_v5_nw_2026-10-21/` |
| install record | `H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_nucliaweight_2026-10-21.json` |
| study package | `H1ChargedSidis_HighQ2/output_v6/studies/nucliaweight_variant_2026-10-21/` |
| code | `H1ChargedSidis_HighQ2/scripts/unfolding/nucliaweight_variant_2026-10-21/` |
| plots (overlay, binmap labels; rendered at candidate stage, identical content) | `sidis_highQ2/plots/systematics/nucliaweight_primaryloss_overlay_2026-10-21/` |
| records + critic reviews | `sidis_highQ2/docs/systematics/nucliaweight_primaryloss_2026-10-21/` |

## 3. Definition
- NucliaWeight (`H1NuclIACor::GetWeight = 1 − ani_ineff`) is the MC-only per-track nuclear-interaction loss correction.
  The deficit is scaled: wn → 1 − (1+κ)(1−wn), κ = ±1.
- **Loss-to-miss form:** B → B − κD_B, F → F − κD_F, miss_j → miss_j + κ Σ_i D_B[i,j]. D comes from one exact
  κ = +1 pass (174 shards); the response is linear in κ.
- IBU k=4 on the official data_sub′. Term = max(|s(+1)|, |s(−1)|).
- Covariance: rank-1, d = sign(s(+1))·term·σ, ρ = +1 across bins and charges.
- The term does not depend on the nominal's NW convention (physics critic F1: 6e-15).

## 4. Numbers
- Term: integrated ±1.18% (h+ +1.179/−1.147%; h− +1.183/−1.151%).
  - Per-bin median 0.76/0.78%; p95 2.06/2.33%; max 4.59/5.56% (sparse S3 high-z bins).
  - The DJANGOH response agrees to ≤ 0.03%. MC statistics 0.001%.
- **Quoted total** `total_quoted_with_nonprimary_v5_nw_rel` medians: h+ 7.4475 → **7.4848%**; h− 7.7992 → **7.8604%**.
  - Increment median +0.036 pp; p95 +0.131/+0.137 pp; max +0.239/+0.251 pp.
- **C_total with lumi** medians: 7.3655 → **7.4016%** / 7.5999 → **7.6525%**. Gates 9/9 PASS.

## 5. Caveats carried
- h+/h− and A_h: the NW contribution is zero by construction (charge-blind correction).
  - Charge-asymmetric primary loss is not covered beyond GEANT's per-charge simulation.
  - Protons have NW ≡ 1. Bound: 0.03% integrated, per-bin median 0.05%, max 0.40/0.67%.
- Kept conservative double counts: the fake-leg piece vs `bgsub_fakefrac_rel`; a possible partial overlap with
  `tracking_eff_m0p5_rel`.
- The B_R′ templates exclude NW and do not move with κ (≲ 0.07%).
- The single-κ term is normalization-like; its flat part is degenerate with lumi in a nuisance fit.
- Clip-before-scale gives a few negative weights at κ = +1 (impact ≤ 1.7e-5).
- Central-value finding, NOT covered and not logged (user decision): the nominal's NW deficit is not booked in the
  miss row (efficiency nC/(nC+M)), which leaves the nominal low by about 0.453%, charge-symmetric.
- QUOTING STILL BLOCKED: M1, N2, plotter re-pointing, note update.

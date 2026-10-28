# Exclusive vector-meson background: precedent and options (2026-10-26)

**Scope.** This note covers the high-Q² (Q² > 150 GeV²) charged-hadron SIDIS cross sections, in the eΣ final release, unfolded with IBU. The DIFFVM elastic ρ⁰ sample is scaled to luminosity (×0.135913). It is not subtracted, and it carries a ±100% systematic. The note asks three things: how published measurements handled exclusive vector-meson (VM) contamination, how the other workspace analyses handled DIFFVM, and which option fits our case. It builds on `sidis_highQ2/docs/backgrounds/diffvm_large_bins_2026-10-25.md` ("the 10-25 note"), including its two-track m_ππ test, which is not redone here. Research only: no code, jobs, note or plan changes.

Path shorthand: `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`, `R` = `H/output_v6/studies/esigma_final_2026-10-24/downstream`.

## Short answer

- **The usual fixed-target answer:** HERMES and COMPASS subtract the exclusive VM yield bin by bin using a VM Monte Carlo. Both also keep z below about 0.8–0.85, or drop the highest-z bin from the figures. Both publish values with and without the correction. Since 2024, COMPASS has normalised the VM MC to data through the missing-energy spectra.
- **H1 and ZEUS have no precedent that matches our case.** No H1 track or hadron paper subtracts or cuts exclusive VM events. One H1 paper (2021) varies its diffractive MC by a factor 2 as a systematic. ZEUS applies a rapidity-gap cut (η_max > 3.2), but only below Q² = 160 GeV².
- **The large "contamination" is a prediction, not an observation.** The data show no ρ peak. DIFFVM is anchored at photoproduction (Q² = 0) and extrapolated with a fixed model. The ZEUS ρ⁰ data up to Q² = 160 GeV² fall increasingly steeply with Q². Both facts point the same way: DIFFVM probably overestimates the exclusive yield at Q² > 150 GeV², by a factor 2 or more (inferred).
- **Recommended:** replace the ±100% term with a data-constrained one, at most 0.55× DIFFVM in the corner, and flag generator bin 30 (both charges). As a fallback, drop or merge that bin. Do not subtract the full DIFFVM. Do not build an exclusivity cut for the publication.

## 1. Published precedent

V = verified in the arXiv full text, by worker close reading. U = unverified or abstract-only.

| Measurement | Treatment | Size of contamination | MC normalisation and uncertainty | Data vs VM MC |
|---|---|---|---|---|
| HERMES multiplicities, PRD 87 (2013) 074029, arXiv:1212.5407, Sec. "Exclusive vector-meson contribution" (V) | Bin-by-bin MC subtraction in both the SIDIS and the DIS yields. The highest z bin (0.8–1.1) is omitted from the figures "because of the large contribution from exclusive vector meson production". "The final tabulation includes data with and without this correction." | "up to 50% of the charged pions" from ρ⁰ near z → 1. The φ meson gives under 10% of kaons. The DIS correction is about 4%. | PYTHIA VMD model tuned to HERMES exclusive ρ⁰ data. Uncertainty from a 1σ variation of the ρ⁰ cross-section parametrisation. | Not discussed |
| COMPASS h±, PLB 764 (2017) 001, arXiv:1604.02695, Sec. 3.4–3.5 (V) | HEPGEN ρ⁰ subtraction, about 25% of it target-dissociative. A 0.2 < z < 0.85 cut also "excludes the region with large diffractive contributions". HEPData has both versions. | Correction factor between 1.02 and 0.55 for pions, largest at small Q² and high z | Normalised to a phenomenological cross section. About 30% on that cross section gives at most 30% on the correction. | Not discussed |
| COMPASS kaons, PLB 767 (2017) 133, arXiv:1608.06760 (V) | Same method, for ρ⁰ and φ | φ correction between 0.1% and 22% | 30% on σ_φ, which gives under 1.5% on average | Not discussed |
| COMPASS pT-dependent h±, PRD 97 (2018) 032006, arXiv:1709.07374, Sec. 3.4 (V) | Same method. The 0.2 < z < 0.8 cut partly excludes the ρ⁰ region. HEPData has both versions. | π from ρ⁰ is 40–50% near z → 1. The correction is about 25% at low P_hT² for 0.6 < z < 0.8. | 30% on σ, which gives up to 5–6% on the correction | Not discussed |
| COMPASS proton target, arXiv:2410.12005, Sec. 3.4, 3.6 (V) | HEPGEN ρ⁰ and φ subtraction. The "missing energy [spectra] obtained from HEPGEN and LEPTO are normalised to the data". 0.2 < z < 0.85. | At high z the ρ⁰ contribution "exceeds the SIDIS cross section itself" | The data normalisation brings the uncertainty down to about 20%, which is up to 23% on the multiplicity. The scale factor is not stated in the text. | Rescaled to data; the closest analogue to our case |
| COMPASS K⁻/K⁺ at high z, arXiv:1802.00584 (V) | No correction | The VM contribution is judged "negligible" | none | n/a |
| JLab Hall C π±, PRC 85 (2012) 015202, arXiv:1103.1649 (V) | Diffractive ρ and the exclusive-π radiative tail are both subtracted bin by bin. The fits are redone without the ρ subtraction as a check. | ρ is a few % at low z, rising to about 15% at z = 0.6. The exclusive tail exceeds 50% at z ≳ 0.9. | PYTHIA (HERMES tune) with a CLAS-motivated t slope. Parameters are varied by ±10%, giving up to 2.5%. | Agrees with an independent CLAS code to about 10% |
| CLAS π⁺, PRD 80 (2009) 032004, arXiv:0809.1153 (V) | No ρ⁰ treatment. A missing-mass cut M_X > 1.08 GeV removes exclusive π⁺n. | n/a | n/a | n/a |
| CLAS12 (U) | Talk-level only: a ρ⁰ MC scaled to data. No paper found. | | | |
| H1 charged-particle spectra, EPJC 73 (2013) 2406, arXiv:1302.1321 (V) | None. PHOJET photoproduction includes VM production, but that is the photoproduction background only. | not quoted | Djangoh correction, Rapgap as the systematic | n/a |
| H1 scaled momenta, PLB 654 (2007) 148, arXiv:0706.2456, 100 < Q² < 20000 (V) | None; standard E−p_z and vertex cuts only | not quoted | | n/a |
| H1 multiplicity distributions, EPJC 81 (2021) 212, arXiv:2011.01812, 5 < Q² < 100 (V) | Systematic only. "Sys.7 – Diffractive contributions: variation of the diffractive MC contribution by a factor of 2". No gap cut. | The systematic is 1–5% on P(N) | Factor-2 variation; the generator is not named | n/a |
| H1 ML-unfolded, Q² > 150: arXiv:2108.12376 (PRL 128 (2022) 132002), 2111.11364, 2303.13620, 2403.10109 (V) | None. The jet papers require a jet with pT > 10 GeV, which removes exclusive events (inference). 2111.11364 and 2403.10109 are inclusive and do not discuss the issue. | not quoted | Djangoh/Rapgap | n/a |
| H1 event shapes, EPJC 46 (2006) 343, hep-ex/0512014 (V) | No η_max or gap cut | | | n/a |
| ZEUS scaled momenta, JHEP 06 (2010) 009, arXiv:1001.4026, Sec. 2 (V) | An η_max > 3.2 cut "removes diffractive events", applied to the 10 < Q² < 160 GeV² sample only. Systematic from varying the cut by ±0.2. | "for Q² > 100 GeV² the contamination with diffractive events is well below 5%". Up to 10% in the lowest Q² bin. | Systematic +2/−1% | n/a |

This table supersedes §4 of the 10-25 note. In particular, its ZEUS row ("no treatment") was wrong.

**Pattern.** Where the VM share reaches tens of percent, every experiment that published z near 1 did two things. It subtracted using a tuned MC, and it either dropped the top z bin or published with and without the subtraction. Nobody published a bin with a large uncorrected VM share as if it were clean. COMPASS 2024 is the only case that tuned the VM MC to data.

## 2. The ρ⁰ rate at Q² > 150 GeV² (DIFFVM steering verified, extrapolation inferred)

- **DIFFVM steering (verified).** Our sample is diffvm11001, genIDs 8445–8448. It has the same physics settings as diffvm11000:
  - propagator with Λ = 0.77 GeV and a constant n = 2.4;
  - a longitudinal-to-transverse ratio R_LT with ξ = 0.30 and χ = 0.07, which rises with Q² and then saturates, so the effective fall-off of the total cross section is not a pure n = 2.4;
  - normalisation σ_γp = 15 µb at W = 90 GeV, i.e. at Q² = 0;
  - b = 6 GeV⁻²;
  - elastic only.

  Source: `sidis_lowQ2/docs/research/research-diffvm-rho-LMC-database-2026-05-26.md:182, 261-267`. This replaces the provisional "n = 2.0" in the 10-25 note §3, which was taken from the φ sample.
- **Measured Q² fall-off (verified).**
  - H1, JHEP 05 (2010) 032, arXiv:0910.5831, covers 2.5–60 GeV². A constant fit gives n = 2.37 ± 0.06. A running form n = 2.09 + 0.0073·(Q²+M²) fits better (χ²/ndf 17/24 against 40/25).
  - ZEUS, PMC Phys. A 1 (2007) 6, arXiv:0708.1478, covers 2–160 GeV². A constant n does not fit: "the data appear to favour an n value which increases with Q²". ZEUS gives σ = 0.36 ± 0.07 nb at Q² = 60 GeV² and 0.05 ± 0.03 nb at 100 GeV², both at W = 90 GeV.
- **Inference.**
  - The local slope from the ZEUS points rises from about 2.4 (Q² of 12–24 GeV²) to about 3.5–3.9 (Q² of 40–100 GeV²). H1's constant fit, n = 2.37 over 2.5–60 GeV², agrees with DIFFVM's 2.4. The case for an overestimate at Q² > 150 GeV² therefore rests on the ZEUS high-Q² points and on our data.
  - Reviewer estimate, not run through the generator: evaluating the printed DIFFVM formula at W = 90 GeV gives about 2.4 nb at Q² = 60 GeV² and about 0.8 nb at 100 GeV². ZEUS measures 0.36 ± 0.07 and 0.05 ± 0.03 nb, so DIFFVM would be roughly 7–16× high there.
  - This is consistent with the data limit of at most 0.35–0.55× DIFFVM. That limit is statistics-limited, so the true ratio may be far smaller.
- **Proton-dissociative ρ⁰.** This adds about 0.56× the elastic yield (H1, for M_Y < 5 GeV). It is absent from DIFFVM. It raises the true yield, but it is already inside the two-track data bound.

## 3. Workspace precedent (verified, file:line)

| Analysis | Treatment of DIFFVM |
|---|---|
| sidis_lowQ2 | **Subtracted** ρ/ω/φ before unfolding, with luminosity scaling (`context-handoff.md` ~2917, ~2945-2972). φ was rescaled ×0.03984 after a data m_KK check (`context-handoff.md:3076-3086`). A ρ⁰ overprediction of about 46× was "not independently confirmed" and never applied (`context-handoff.md:2965`). It predates the luminosity and double-counting fix. Systematic "DIFFVM ±3 nb" (`docs/v3p3_final_headline_2026-05-29.md:182`). No exclusivity cut. |
| exclu_phi | DIFFVM φ is the signal there, normalised to data with k = 0.767 (`docs/research/diffvm_phi_normalization_audit.md:277-289`). The MC ρ/ω background overshoots the data (`docs/research/bkg_composition_omega_rho.md:1-14`). |
| lambda (low-Q²) | DIFFVM is 0.16% of the data, carried as ±3% in the budget. Subtraction was proposed but never done (`docs/research_PHP_lowQ2_subtraction_from_SIDIS.md:181-204, 340, 390`). |
| proton_HiQ | Plan only: a data-driven rapidity-gap estimate, plus a systematic covering the full fraction in the z > 0.6 bin (`docs/plan-sidis-tmd-final.md:814, 1069`). |
| proton, lambda_HiQ, xi | Not used |

No analysis in the workspace has validated the DIFFVM ρ⁰ normalisation against data. The two that checked a DIFFVM channel against data (sidis_lowQ2 for φ, exclu_phi) both found that the MC is too high.

## 4. Affected bins (verified)

Sources:
- reco fraction: T/D with T = `T_980` in `R/diffvm/diffvm_reco980_esigmaphotonxf_2026-10-24.npz` and D = `per_bin.n_raw` in `R/vectors/data_vector_bgsub_setrprime_esph660.json`. Using `n_subtracted` instead changes the ratio by less than 0.01.
- unfolded fraction: `diffvm_rho_rel`, total and stat uncertainties from `R/budget_v2/esph_budget_v2.csv`.

All listed bins have z = 0.7–1.0. Cells:
- S0_x0: Q² 150–300 GeV², x 0.0021–0.0071
- S0_x1: Q² 150–300 GeV², x 0.0071–0.030
- S1_x0: Q² 300–700 GeV², x 0.0042–0.016
- S1_x1: Q² 300–700 GeV², x 0.016–0.069

**Unfolded (published) bins: 7 above 10%, 2 above 20%, 0 above 40% (of 490).**

| charge | gen bin | cell | pT_h (GeV) | DIFFVM share | total unc. | stat unc. |
|---|---|---|---|---|---|---|
| h⁻ | 30 | S0_x0 | 0–0.5 | **0.346** | 0.397 | 0.161 |
| h⁺ | 30 | S0_x0 | 0–0.5 | **0.220** | 0.285 | 0.144 |
| h⁺ | 31 | S0_x0 | 0.5–1 | 0.144 | 0.213 | 0.103 |
| h⁻ | 31 | S0_x0 | 0.5–1 | 0.138 | 0.211 | 0.099 |
| h⁻ | 65 | S0_x1 | 0–0.5 | 0.132 | 0.224 | 0.103 |
| h⁻ | 100 | S1_x0 | 0–0.5 | 0.112 | 0.256 | 0.183 |
| h⁺ | 65 | S0_x1 | 0–0.5 | 0.104 | 0.201 | 0.092 |

**Reco bins: 12 above 10%, 7 above 20%, 2 above 40% (of 980).** The ">40%" figure is reco-level only.

| reco bin | charge | cell | pT_h (GeV) | T | D | T/D | gen bin |
|---|---|---|---|---|---|---|---|
| 551 | h⁻ | S0_x0 | 0.25–0.5 | 7.34 | 15 | **0.489** | 30 |
| 690 | h⁻ | S1_x0 | 0–0.25 | 0.41 | 1 | **0.408** | 100 |
| 620 | h⁻ | S0_x1 | 0–0.25 | 1.90 | 7 | 0.272 | 65 |
| 200 | h⁺ | S1_x0 | 0–0.25 | 0.27 | 1 | 0.272 | 100 |
| 550 | h⁻ | S0_x0 | 0–0.25 | 2.58 | 10 | 0.258 | 30 |
| 60 | h⁺ | S0_x0 | 0–0.25 | 1.77 | 8 | 0.221 | 30 |
| 760 | h⁻ | S1_x1 | 0–0.25 | 0.82 | 4 | 0.204 | 135 |
| 61 | h⁺ | S0_x0 | 0.25–0.5 | 6.25 | 37 | 0.169 | 30 |
| 62 | h⁺ | S0_x0 | 0.5–0.75 | 5.98 | 39 | 0.153 | 31 |
| 552 | h⁻ | S0_x0 | 0.5–0.75 | 5.03 | 35 | 0.144 | 31 |
| 131 | h⁺ | S0_x1 | 0.25–0.5 | 3.94 | 33 | 0.119 | 65 |
| 621 | h⁻ | S0_x1 | 0.25–0.5 | 3.67 | 33 | 0.111 | 65 |

Bins 690 and 200 contain one data track each.

**Merge estimates (reco level only; an unfolded number needs a re-unfold):**
- Merging z 0.7–1.0 with z 0.5–0.7 at the same pT_h brings every flagged bin to 5% or below (0.7–4.9%). The ρ⁰ pions sit at z ≈ 0.95 and z ≈ 0.05, so z 0.5–0.7 adds data but little ρ⁰.
- Merging the two lowest pT_h bins leaves h⁻ S0_x0 at 39.7%.
- Merging the two charges leaves h⁻ S0_x0 at 25.6%, because the ρ⁰ contributions of the two charges add.
- Dropping z > 0.7, pT_h < 0.5 in S0_x0 removes 6.0% of the DIFFVM prediction and 1.8 × 10⁻⁵ of the data tracks.

## 5. Options, ranked for our case

The two-track test gives an excess/DIFFVM ratio of −0.01 ± 0.18 overall and 0.18 ± 0.19 in the corner. The 2σ upper limits are about 0.35× and 0.55×. A data-normalised subtraction would therefore remove almost nothing. "Subtract normalised to data" and "keep with a data-constrained uncertainty" become the same option.

The scaled shares below assume the unfolded DIFFVM term scales linearly with T. This is an approximation, since IBU is nonlinear.

1. **Data-constrained uncertainty plus a flag (recommended; fits the no-peak result).**
   - *What changes:* keep the nominal unsubtracted. Alternatively, subtract the fitted 0.18·T in the corner; the effect is small either way. Replace ±100%·T by ±0.55·T, the corner 2σ limit (±0.35·T would be the all-z limit). Footnote gen bin 30 for both charges.
   - *Precedent:* COMPASS 2410.12005, which normalises the VM MC to data.
   - *Effect on bins:* the DIFFVM share in h⁻ bin 30 drops from 34.6% to about 19%, and the total uncertainty from 39.7% to about 27%. h⁺ bin 30 goes from 22% to about 12%, with total 28.5% → about 22%. Every other bin falls below 8%. No bin is removed.
   - *Cost:* rescale T, rerun the group-C budget and the assemble step; no re-unfolding of the nominal. This changes the signed-off ±100% term (`.supervisor/memory/decisions/2026-10-24-esigma-new-rank1-bg-terms-signed-off.md`), so it needs a user decision.
   - *Weak point:* the two-track test misses exclusive events with an extra central track. The DIFFVM normalisation is itself unverified.
2. **Merge the corner bins in z (z 0.5–1.0 at pT_h < 0.5 or < 1 GeV in the lowest-x cells).**
   - *Precedent:* in spirit, the HERMES and COMPASS upper z cuts.
   - *Effect on bins:* removes the problem by construction; every merged bin is at 5% or below at reco level.
   - *Cost:* a new generator binning with irregular bins, a response rebuild from the existing trees, a full re-unfold and budget, and a re-validation of the binning. It also loses the z → 1 corner.
3. **Drop gen bin 30 (both charges), optionally also 31, 65 and 100.**
   - *Precedent:* HERMES, which drops the top z bin from its figures.
   - *Effect on bins:* 2 of 490 bins (or up to 7) leave the main table; they could stay in an auxiliary table.
   - *Cost:* tables only, no rerun.
   - *Weak point:* it treats a background the data do not show as real, and throws away about 70 data tracks across both charges (about 52 after the DIFFVM estimate).
4. **Keep the ±100% term (status quo).**
   - *Precedent:* H1 2011.01812, factor-2 variation.
   - *Effect on bins:* none; this is what the user calls unpublishable. It overstates the uncertainty by about 2× given the data.
   - *Cost:* none.
5. **Subtract the full luminosity-scaled DIFFVM (the HERMES/COMPASS-2017 style).**
   - *What changes:* would move h⁻ bin 30 by about −35%.
   - *Weak point:* disfavoured by the data. The no-peak result and the measured Q² slope both say it over-subtracts by 2× or more. Avoid it, including as a "without correction" column.
6. **Exclusivity cut at reco and gen level (track multiplicity, rapidity gap or η_max).**
   - *Precedent:* ZEUS only below Q² = 160 GeV². No H1 SIDIS precedent.
   - *What changes:* it changes the definition of the measured cross section (non-diffractive SIDIS), which complicates theory comparison.
   - *Cost:* HFS-energy and forward-detector branches are not in the trees, so it needs about 1060 production jobs. It also needs a new gen-level definition and a full re-unfold.
   - *Possible use:* worthwhile only as a cross-check. One cheap form needs no new branches: veto events with exactly two opposite-charge tracks and z₁+z₂ ≈ 1, and see whether bin 30 moves. This reuses the two-track selection from the 10-25 note.

## Verified vs inferred

- **Verified:** the full-text literature rows marked V; the DIFFVM steering; the bin tables and merge numbers, recomputed from the listed files; and the workspace precedents, with file:line.
- **Inferred:**
  - that the H1 jet requirements remove exclusive events;
  - the size of the DIFFVM overestimate (reviewer estimate 7–16× at Q² of 60–100 GeV², not run through the generator);
  - the linear scaling of the unfolded DIFFVM term in option 1;
  - that COMPASS's rescale implies its MC was off (the factor is not stated).

## Risks, open questions, gaps

- The two-track bound misses exclusive or proton-dissociative topologies with a third central track, and events outside |η| < 1.6. A data-only stage-1 filter removing clean two-track events is not ruled out (10-25 note, §3).
- Not read:
  - HEPData for the COMPASS 2024 scale factor;
  - a published CLAS12 paper (none found);
  - H1prelim-25-031 ("Towards unfolding all particles in high-Q² DIS"), whose server was unreachable;
  - ZEUS fragmentation and V0 papers beyond 1001.4026.

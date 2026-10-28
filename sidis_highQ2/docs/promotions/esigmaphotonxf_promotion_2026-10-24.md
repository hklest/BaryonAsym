# Promotion record, eSigma final release with eSigma x_F and the true generator photon (tag esigmaphotonxf_2026-10-24)

Status: INSTALLED 2026-10-25 by s15_install_esigmaphotonxf_2026-10-24.py (Condor cluster 5888201): 106 files, verbatim copies, bitwise equal to
MANIFEST.sha256; 49 of 49 hard gates True; 1705 files of 46 older editions and install records sha-swept before and after, unchanged.
Package: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_final_2026-10-24/phaseD/package/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_2026-10-24
(MANIFEST.sha256 sha 1c55b1d3b1be10396bf1c7727812ee19d3c30e061652bab8ebbe45f7fe4b5ecc, 173 entries: 106 installable, 67 provenance and sources).
Install record: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphotonxf_2026-10-24.json
(sha256 89d07b5de7394b0e21f95ea76a7e4ab3495527f64bc897d5df0fd5f7f14b67d4). Plan: /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md.
Provenance notes (stale metadata strings, raw versus Born convention of the covariance, lineage): PROVENANCE_NOTES.md in the package.

Raw versus Born: "raw" is the unfolded value before the radiative correction C_rad; "Born" has C_rad applied (release folder born_crad_applied).
The budget is built against the raw release; relative uncertainties apply to both. The covariance files and ratio_asym_columns.csv are at raw level;
the Born-level charge ratio is the raw ratio times C_rad(h+)/C_rad(h-) per bin (integrated agreement 1.4e-4, per bin up to 8%).

## 1. What this promotion is
One final production (plan D22) under new roots, every term rebuilt from this campaign. Installed targets:
- output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-24
- output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-24
- data/covariance/z005_covariance_optA_esigmaphotonxf_2026-10-24
- sidis_highQ2/data/radiative/cgammaborn_esigmaphotonxf_2026-10-24
The 2026-10-23 editions are superseded, not deleted (D11).

## 2. What changed in plain words
- The current-hemisphere cut x_F < 0 is measured along the proton direction in the photon-proton frame, built from the eSigma photon at
  reconstruction level and from the true exchanged photon at generator level (U1).
- At generator level every variable (Q2, y, x, z, pT, phi_h, the fiducial cuts, the Q2 cell) uses the true exchanged photon, with only
  initial-state photons subtracted from the beam energy (U2). The previous formula also subtracted final-state photons; those events now return
  to the fiducial region (+6% generator truth, matched by the same rise in the radiative leg of C_rad).
- Generator pT uses the true hadron mass (U4).
- New quoted terms: DIFFVM rho0 +-100% and the low-Q2 +-30% normalisation at rho = +1 (U3); 26 quoted sources.
- C_gammaBorn rebuilt with f(e+p) = 0.5439 (D14). C_rad uses the same MC samples and exposures as the 2026-10-21 nominal (proof:
  output_v6/studies/esigma_final_2026-10-24/s3/crad_sameMC/crad_same_mc_proof.json).

## 3. Headline numbers (from the installed products)
| quantity | h+ | h- |
|---|---|---|
| integrated Born [nb] | 12.026 | 11.682 |
| integrated no-Z Born [nb] (fixed alpha(0)) | about 10.97 | about 10.66 |
| total quoted uncertainty, median over 245 bins | 8.13% | 8.92% |
| previous edition (2026-10-23), same median | 8.34% | 8.89% |
| integrated fake-removal fraction (RU-2) | 7.32% | 6.99% |
Integrated C_rad: 1.0719 (DJANGOH), 1.0680 (RAPGAP); integrated C_gammaBorn 0.9777 / 0.9775.

## 4. User decisions and rulings (stated, not reopened)
U1-U4 and D0-D24 of the plan; and the 2026-10-25 rulings recorded in the plan section "User rulings during production": benign pT_h-edge
mismatches certified for both generators (option A); truth_parity retired; same MC for C_rad; comparisons with the previous analysis are
diagnostics; the high-z Born shift (+0.6 to +2.6% at z >= 0.2) accepted as unfolding model dependence; tight-DCA kappa stays +-100% with the
exceedance bins h+ 176 (114%) and h- 129 (104%) quoted (1000 toys); TUnfold v4/v5 dropped, the TUnfold cross-check covers the nominal result only.

## 5. Critic verdicts
S2 smoke: h1-analysis-critic PASS, h1-mc-expert OK, critic PASS after fixes. Pre-install: h1-analysis-critic PASS, h1-systematics PASS;
install scripts: critic round 1 REVISE (all fixed), round 2 PASS, scope-critic TRIM (cosmetic only).

## 6. Limitations
- Three bins have a Born shift above their total quoted uncertainty (h+ bins 193, 222, 234; statistics dominated); diagnostic
  output_v6/studies/esigma_final_2026-10-24/s4/born_shift_vs_ibu_model.json.
- The TUnfold cross-check covers the nominal result only (no in-solve detector systematics).
- Several product metadata strings are stale (listed in PROVENANCE_NOTES.md); no number is affected.

## 7. Gates (hard gates all PASS; diagnostics recorded in the install record)
S2 13/13; S3-S5 summary pass; covariance s08 8/8, s09 g0-g7, ga10, gD5; cut variations 7/7; same-MC proof; RU-2 re-derived window; f_i
internal consistency; benign certification; generator phi_h manifest PASS; release lineage (no old input feeds a number).

## 8. Not done in this record
S8 figures and documents; S9 freeze (not requested).

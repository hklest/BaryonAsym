# Promotion record, eSigma reco-photon release (2026-10-23)

Status: INSTALLED 2026-10-23 by s15_install_esigmaphoton_2026-10-23.py (Condor cluster 5844143): 106 files, verbatim copies, bitwise equal to MANIFEST.sha256. Critic: round 4 REVISE (items fixed); round 5 PASS.
Package: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphoton_2026-10-23
(MANIFEST.sha256 sha 72cef8cd1ebdbff9f14a62f2f588056996b1d81cfa521f287994617f1629acbb, 138 files, all verified; covariance edition with the rank-1 detint normalization term).
Install record: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphoton_2026-10-23.json (sha256 1ab34a98bb07dfc62eadcf7fa85a761296b8c459c5397bfc4cd46239f01da06a). Plan: /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/esigma_release_plan_2026-10-22.md (section 7, F2).

Raw versus Born, used throughout: "raw" means the unfolded value before the radiative correction C_rad; "Born" means the table with C_rad applied
(release folder born_crad_applied). C_rad was applied, not rerun. All uncertainty numbers below are relative (fractions of the value), so the same
percentage holds for the raw table and the Born table; the budget is built against the raw release tables.

## 1. What this promotion is
One bundled install (user decision U5): the 660 files, L_eff, the consistent C_rad (Djangoh), the MC-only detector legs, the +-1% electron
energy scale, the unfolded env100 background term, and the new electron-Sigma (eSigma) reconstructed photon, with the swap-one table in section 3.
Planned install targets (all under /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2):
- output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphoton_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-23
- output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphoton_2026-10-23
- data/covariance/z005_covariance_optA_esigmaphoton_2026-10-23

## 2. What changed in plain words
The photon that the virtual-photon frame is built from now comes from the electron plus the hadronic final state (eSigma) instead of the
electron alone. Q2, z and pT follow the eSigma photon. The generator target is unchanged and C_rad is not rerun.
New quoted term: sigmah_smear (smear of the hadronic Sigma in simulation only). JES now uses the symmetrized (up-down)/2 shift.
The det.-int. removal gets a +-100% normalization term. No ISR/QED term was added (record line a).

## 3. Swap-one attribution (median of the relative total over 245 bins per charge; raw and Born have the same relative size)
Source: package budget/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphoton_2026-10-23/swap_one/swap_one_attribution.md (installed verbatim with the budget dir; attribution study, not a quoted column).
Method: replace one term at a time by its later-level value and read the change in the total median, in percentage points (pp).

| step | h+ total median | h- total median | main driver |
|---|---|---|---|
| installed to final electron frame | 7.485 to 9.732 % (+2.248 pp) | 7.860 to 10.502 % (+2.641 pp) | elec_energy_rel +2.248 / +2.649 pp (1.62 to 4.08 % / 1.72 to 4.23 %) |
| final electron to eSigma v1 | 9.732 to 8.200 % (-1.532 pp) | 10.502 to 8.803 % (-1.699 pp) | elec_energy_rel -1.549 / -2.028 pp (4.08 to 2.68 % / 4.23 to 2.88 %) |
| eSigma v1 to v2 (new legs) | 8.200 to 8.343 % (+0.143 pp) | 8.803 to 8.890 % (+0.087 pp) | sigmah_smear_rel +0.051 / +0.081 pp (0 to 0.44 % / 0.42 %) |
| installed to eSigma v2 (whole) | 7.485 to 8.343 % (+0.858 pp) | 7.860 to 8.890 % (+1.029 pp) | elec_energy_rel +0.752 / +0.943 pp (1.62 to 2.68 % / 1.72 to 2.88 %) |

The cross-section value itself moves by +0.33 % (h+) and +0.21 % (h-) over the whole step (median, not attributed to terms).
The sum of single swaps leaves a residual of +0.102 pp (h+) and -0.163 pp (h-) over the whole step (interaction between terms).
Budget v2 totals: 8.343 % (h+) / 8.890 % (h-) (esph_budget_v2_summary.json, sha of esph_budget_v2.csv cbac4b57...0108b9).

## 4. User decisions (stated, not reopened)
D1 to D4, 2026-10-22 (source: reference/final_production_plan_2026-10-21.md section 10 under the plan's handoff folder):
- D1: detector legs (electron E and theta, HFS JES, RCES, theta) use the MC-only definition, Delta = IBU(D_nom; M_leg) - IBU(D_nom; M_nom); data-only kept as a cross-check; budget and covariance both.
- D2: electron energy scale +-1% (the +-0.5% legs only as a linearity check).
- D3: applied C_rad stays Djangoh14 (consistent-definition tables); RapGap31 is the formal alternative; the Djangoh gen-only sigma_rad does not close to the Djangoh detector-MC truth (+1.65% integrated, z-shaped; RapGap closes to 0.2%).
- D4: env100 switches to the unfolded form (IBU k=4 re-unfold on the OPT-A chain), ending the mixed-definition caveat.

U1 to U6, 2026-10-23 (decisions in /data/dust/group/h1/klesthen/BaryonAsym/.supervisor/memory/decisions/2026-10-23-esigma-*.md):
- U1: no ISR/QED kappa study and no new ISR quoted term (not standard in H1); radiative effects stay with the applied C_rad.
- U2: add the MC-only Sigma_h smear leg only; no LAr-noise proxy.
- U3: JES symmetrized (up-down)/2 for budget and covariance.
- U4: low-Q2 parity is documented as a difference only; separate effort.
- U5: one bundled promotion with a swap-one table; release name accepted; install stays user-gated at Phase F.
- U6: the +-0.5% E' linearity legs are out of scope. The det.-int. kappa is +-100%, like env100 (same decision file as U4).

## 5. Critic verdicts
- Critic round 4 (hostile): REVISE — 1 BLOCKING (detint sign inferable; fixed by rank-1 rebuild) and SHOULD/NIT items fixed; round 5: PASS

## 6. Limitations
- Det.-int. removal is carried from the electron frame (median 0.03%, not recomputed in eSigma; a native value needs 174 shards). Its +-100% normalization term is rank-1 with a uniform inferred sign; see record line d. Record: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A7_records/A5_detint_carry.md.
- The xF cut keeps the electron-like photon: stored xF_SIDIS_es uses the electron-method Q2 and angle, not the eSigma photon. The hemisphere cut is the same in both frames; only z and pT follow eSigma. At z < 0.1 tracks near xF = 0 can fall on different sides. Record: A7_xf_cut_photon.md in the same folder.
- Low-Q2 difference: the low-Q2 analysis still uses the electron photon (U4, documented only). High-Q2 (eSigma) and low-Q2 (electron) results are not built on the same photon.
- The m2_zshift term is identically zero and its ISR residual is unquoted (line a).

## 7. Record lines a to f (condensed from record_lines in the install record; numbers verified at the cited sources)
a. U1: there is no ISR/QED leg. This corrects the plan's wording "the ISR residual belongs to the U1 leg" (plan line 123): there is no U1 leg; the m2_zshift ISR residual is unquoted. Source: .../2026-10-23-esigma-no-isr-qed-leg.md; A7_m2_zshift_rationale.md.
b. A2 ibu_model B1 floor: the same working-point check FAILs: eSigma floor 1 h+ bin (pos 34, S0_x0, z_idx 6, pT_idx 4: replica mean -1.456% vs working point +0.863%, dev 2.32% > 3*SEM 2.28%); the installed floor also FAILs this check, in 4 different bins (pos 98; neg 8/123/226); accepted as for the installed floor. Accepted by the lead 2026-10-23, no rerun. Source: phaseA/A2_b1floor/run_record.json.
c. JES: RapGap only, symmetrized (up-down)/2, since the generator test gives p = 1.3e-6 (phaseB/jes_sym/jes_sym_esph.json). This is an approximation: the Djangoh difference is stated, not added; the test ignores bin correlations; RapGap-only versus the average changes the median by under 0.02%.
d. Det.-int. +-100% term is a RANK-1 covariance term with a uniform negative sign (cov_detint_norm_rank1.npy; detint_norm_form = rank1_uniform_sign_inferred), replacing the earlier diagonal form. Evidence: the sum over bins of |inc_RG,i| (= sum of norm_i*sigma_i) is 0.032753 nb (h+) / 0.028001 nb (h-) and equals the absolute value of the signed integrated increment |-0.032760| / |-0.028003| nb (relative difference 2.2e-4 / 1.0e-4; groupA_new.json, diagnostics.detint_integrated_increment_nb), so by the triangle inequality all bins share the negative sign (A5 record lines 38-47); gate g_detint_norm_uniform_sign PASS. Effect: the integrated detint-norm uncertainty is 0.03275 / 0.02800 nb against 0.00907 / 0.00841 nb in the diagonal form (x3.6 / x3.3); per-bin diagonals and budget totals are unchanged; the h+/h- ratio relative uncertainty median moves 6.565% to 6.559% and the A_h absolute uncertainty median 0.03281 to 0.03278 (the correlated term largely cancels in ratios). Size: quoted detint_removal_rel medians 0.197% (h+) / 0.171% (h-), norm part 0.191% / 0.166%, model part 0.029% / 0.028%. Residual caveat: the per-bin signs are inferred from the integral equality, not stored; any opposite-sign content is bounded by the 2e-4 relative mismatch. The old diagonal edition is kept in _superseded_esigmaphoton_cov_diagdetint_2026-10-23.
e. Sigma_h smear leg is a single draw. Noise level: median |RapGap-Djangoh|/2 = 0.17% (h+) / 0.14% (h-) against the quoted median 0.44% / 0.42% (generator-difference proxy, not a repeat-draw spread).
f. Carried: rank-1 signs of ibu_model, ibu_reg, detint_model, c_rad_gen come from the installed edition (no eSigma signed vectors). Joint-toy clipping distortion flagged, not corrected (1000 toys, 3895 clipped bin instances, 994 toys with a clip, 42 bins). C_stat PSD minimum eigenvalue -2.5949e-06 against tolerance -2.5949e-06 (at the edge; 0 bins below). NucliaWeight h+/h- ratio column: zero by construction in the budget; the covariance-derived h+/h- ratio uncertainty is <=0.081% from unequal h+/h- magnitudes (informational); user-dispositioned.

## 8. Gates (all PASS)
- Governing runs: s08 cluster 5843553 (8/8 gates PASS, including g_detint_norm_uniform_sign) and s09 cluster 5843566 (g0 to g7, ga10, gD5 x3, and the g5 sha sweep over 156 protected files: all PASS), both on budget v2 sha cbac4b57...0108b9. Logs in package provenance/phaseD/s08s09/logs/.
- Superseded (diagonal-detint edition, kept for audit): s08 cluster 5842985, s09 cluster 5843172.
- s03 closure PASS. Diagonal toy medians 0.839% (h+) / 0.938% (h-) = 0.57 / 0.60 of the pre-OPT-A STATE bound 1.4769% / 1.5752%; pre-registered criterion "same order" (closure_report.json). Frozen reference medians 0.8531% / 0.9195% (2026-10-17, old response). Raw-level relative values.

## 9. Not done in this record
Canonical edits (STATE, RUN_STATUS, handoffs, skills) are the Phase F promote step, after user sign-off and a post-install audit critic (F3). Pre-edit copies go to docs/promotions/esigmaphoton_2026-10-23_preedit/.

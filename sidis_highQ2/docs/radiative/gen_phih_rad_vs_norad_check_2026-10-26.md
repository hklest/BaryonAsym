# Generator-level phi_h, RAD vs NONRAD: definition and difference check (2026-10-26)

Scope: the generator-level phi_h RAD/NONRAD appendix pages of the high-Q2 charged SIDIS note
(eSigma release 2026-10-24), and the generator truth used by C_rad and the unfolding. The checks
were read-only. No code, release or git changes were made, and no Condor jobs were run (none were needed).
Decided convention, not reopened: every generator kinematic uses one true gamma* with ISR-only E_eff
(`.supervisor/memory/decisions/2026-10-24-gen-all-kinematics-true-gamma-isr-only-incl-phih.md`).

Path abbreviations:
- PLOT = `H1ChargedSidis_HighQ2/scripts/plotting/phi_reco_luminorm_2026-06-12/condor_esigmaphotonxf_gen_2026-10-24/plot_phi_rad_nonrad_gen_v3_compact_esxf2.py`
- XF = `H1ChargedSidis_HighQ2/scripts/unfolding/esigma_final_2026-10-24/esigma_xf.py`
- CRADW = `H1ChargedSidis_HighQ2/scripts/radiative/final_crad_esxf_2026-10-24/crad_v3_worker_final_consistent_esxf.py`

## Short answer

No. The generator-level phi_h does not use reco phi_h, and it does not use an eSigma or electron-method photon.
- It is recomputed from generator four-vectors with the true gamma*, in the gamma*p centre-of-mass frame, using the Trento sign convention.
- The RAD and NONRAD curves are different arrays from different MC samples.
- They look almost the same because the true shape difference is below 0.4% overall, and at most 1.6% in any one bin.
- The visible difference is a normalisation offset of about 5% (RAD/NONRAD = 0.94-0.97 per cell). It is not a change of shape.

## 1. Definitions (verified)

| Item | RAD and NONRAD appendix (PLOT, one code path for both samples) | C_rad (CRADW) | Unfolding truth |
|---|---|---|---|
| gamma* | `gen_truth()` XF:183-201: q = k_eff - k'_gen, k_eff = (0,0,-E_eff,E_eff) | `gen_truth` CRADW:143 | no phi_h truth axis; the response is built in z and pT; phi_h enters only as a systematic CSV (`esph_downstream.py:43,303`) |
| E_eff | E0 - GenEnPhoton on ISR-tagged rows, E0 otherwise (XF:150-152); ISR tag XF:143-148; FSR photons not subtracted | same | n/a |
| Scattered lepton | generator record GenElecE/Theta/Phi (XF:191) | same | n/a |
| Lepton plane | k_eff and k'_gen, boosted to the gamma*p CM (PLOT:513, `compute_cm_true`) | n/a, C_rad is not binned in phi_h | n/a |
| Hadron | generator record GenPx/Py/Pz with true mass `gen_mass(GenPDG)` (PLOT:860) | same | n/a |
| phi_h | atan2((nl x nh).qhat, nl.nh) on [-pi,pi] (PLOT:315 definition text, PLOT:513 onward) | none | none |
| Selection and Q2/x cell | true gamma* (PLOT:832-833) | true gamma* (CRADW:143-145, `bindex` CRADW:166) | |

- **How the photon choice is applied.** On ISR rows the true-gamma* phi_h replaces the default (PLOT:871-873). Non-ISR rows keep the older nominal-beam path, q = k(E0) - k'_gen. This is the same vector, because E_eff = E0 on those rows.
- **Reco quantities are never binned.** The stored LAB `phi_h` and `pT_h` are read only as filters (PLOT:836-837).
- **NONRAD rows are never ISR-tagged.** `isr_tagged` = 0 in 6 of the 6 nonrad_old and nonrad_new C_rad partials sampled, covering 13.9 M fiducial rows. NONRAD therefore always uses E_eff = E0, as expected for Born.
- **Source of the release C_rad table.** The release C_rad table (`crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`, release `impact_diff/optA_crad_diff_summary.json:13`) was produced by CRADW, the true-gamma* fork; its cutflow carries the `isr_tagged` counter.

## 2. Figures against their source data (verified)

- **Figure source.** The note section is `sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/appendix_gen_phi_h.tex:10-127`. Its PDF `..._hplus_cell01.pdf` is byte-identical (`cmp`) to the page in `sidis_highQ2/plots/docfigs/esigmaphotonxf_2026-10-24/phi_h_gen/pages/`, written 2026-10-25 17:57.
- **Plot inputs.** The pages come from `.../phi_h_gen/rerun2/partials/shard_0000..0089`. These are the 90 true-gamma* shards, written 2026-10-25 17:40-17:55. They are not the older `rerun` and not the quarantined manifest. The render manifest is `.../phi_h_gen/render/manifest.json`: PASS, PRODUCTION.
- **The arrays differ.** RAD comes from djangoh14 `rad_ep_*.root`; NONRAD comes from separate nonrad files. After merging, `np.array_equal(sumw_rad, sumw_nonrad)` is False for both h+ and h-, and also for the RAPGAP31 pair. The scaled integral for h+ is 10.53 (RAD) against 11.05 (NONRAD).

## 3. Size of the difference (verified numbers; merged over 90 shards, Django, ep+em, summed over z and pT)

Shape-normalised. The moment errors are approximate (bin-centre cosines). The chi2 uses 15 degrees of freedom and statistical errors only.

| h+ cell | int RAD/NONRAD | <cos phi> RAD | <cos phi> NONRAD | <cos 2phi> RAD | <cos 2phi> NONRAD | chi2/15 |
|---|---|---|---|---|---|---|
| 1 | 0.938 | 0.0021(1) | 0.0016(1) | 0.0126(1) | 0.0124(1) | 30.9 |
| 4 | 0.972 | 0.0027(2) | 0.0002(2) | 0.0128(2) | 0.0123(2) | 150.8 |
| 6 | 0.974 | 0.0044(2) | -0.0006(2) | 0.0098(2) | 0.0084(2) | 320.6 |
| 7 | 0.955 | 0.0032(2) | -0.0004(2) | 0.0082(2) | 0.0056(2) | 278.4 |
| all | 0.953 | 0.00241(5) | 0.00118(5) | 0.01293(5) | 0.01256(5) | 305.9 |

- **h- result.** h- behaves the same way. For example, over all cells <cos phi> is 0.00288 (RAD) against 0.00168 (NONRAD).
- **Shape ratio over all cells (h+).** The shape-normalised RAD/NONRAD ratio over all cells runs from 0.998 at phi = ±pi to 1.004 near phi = 0. The error per bin is about 0.0004. In individual cells the ratio spans 0.990-1.016, with the largest spread at high Q2 (cells 5-7).
- **Inference (physics).** The expectation is met.
  - Because the true gamma* absorbs ISR into the kinematics, most of the radiative effect moves into the cell assignment and the normalisation.
  - What remains in phi_h is a statistically significant but small cos phi shift (+0.001 to +0.005) and a cos 2phi shift (-0.0001 to +0.003).
  - On a linear y-axis the two curves sit on top of each other. A ratio panel is the right display. Ratio PDFs already exist in `.../phi_h_gen/render/`.

## 4. Bugs and impact

- **No bug found.** The phi_h computation, the inputs, the figures and the C_rad table all follow the convention. The appendix photon sentence (`appendix_gen_phi_h.tex:4`, q = k_eff - k' with any initial-state photon subtracted) matches the decision.
- **C_rad has no phi_h dependence.** It is binned in (cell, z, pT) only, so no phi_h result in the release carries a phi_h C_rad.
- **Stale wording only, no effect on numbers:**
  - The manifest field `phi_cm_definition`, and the PLOT text around line 306, describe q = k_in - k'_gen without mentioning ISR-only E_eff.
  - The CRADW:37 docstring and the `finalize_crad_djangoh14_esxf.py:10` docstring still say `bindex(Q2esGen, x_es_gen, ...)`.
  - `docs/esigma_figure_audit_2026-10-24.md:89,103` describes the figures from before the rerun.

## Gaps

- The rendered PDFs were not inspected visually. The comparison was done on the arrays only.
- The RAPGAP gen phi_h was not compared.
- NONRAD `GenEnPhoton` was checked only through the ISR-tag counters in 6 partials, not by reading the branch directly.

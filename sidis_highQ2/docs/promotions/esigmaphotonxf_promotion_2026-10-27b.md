# Promotion record, eSigma release with the closure revision and without the detint norm leg (tag esigmaphotonxf_2026-10-27b)

Status: INSTALLED 2026-10-27 by s15_install_esigmaphotonxf_2026-10-27b.py (Condor cluster 5919168): 104 files, verbatim copies, bitwise equal to
MANIFEST.sha256; 55 of 55 hard gates True; 2157 protected files of older editions and install records sha-swept before and after, unchanged.
Package: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_final_2026-10-24/closure_rev3b_staging/phaseD/package/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_2026-10-27b
(MANIFEST.sha256 sha db450ae264a7a589142470f8e6334372a3168b73dbb61633d880f350142f621d, 174 entries: 104 installable, 69 provenance, 1 SOURCES.tsv).
Install record: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphotonxf_2026-10-27b.json
(sha256 f29193a5ccdc0fa340835a102d2c6d1fc63e46e0ef50364258d6ab7118348ce2). Plan: /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/closure_uncertainty_revision_2026-10-26.md (rev 3, D28).

Raw versus Born: "raw" is the unfolded value before C_rad; "Born" has C_rad applied. Relative uncertainties apply to both. Covariance files are at raw level.

## 1. What this promotion is
The 2026-10-27 edition (closure term, response-MC statistics, no DIFFVM term) with one change: the detint_removal normalisation leg is dropped. Installed targets:
- output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-27b
  (every file sha-equal to the 2026-10-24 release, 19 files)
- output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-27b
- data/covariance/z005_covariance_optA_esigmaphotonxf_2026-10-27b
- sidis_highQ2/data/radiative/cgammaborn_esigmaphotonxf_2026-10-27b (sha-equal to the 2026-10-24 table)
The 2026-10-27 and 2026-10-24 editions and their INSTALL_RECORDs are superseded for quoting, not modified or deleted.

## 2. What changed in plain words
- detint_removal_rel = hypot(model, stat). The former +-100% normalisation leg varied the whole response-removed class (material interactions and
  Sigma/Xi/Omega decays), which bgsub_env100_hyp_rel and nonprimary_int_rel already vary; it stays as an INFO column only (user 2026-10-27, "Fix it").
- cov_detint_norm_rank1 is removed: 26 quoted sources, 27 covariance terms.
- Everything else is as in the 2026-10-27 edition (promotion record esigmaphotonxf_promotion_2026-10-27.md), including the per-bin DIFFVM table
  sidis_highQ2/data/backgrounds/diffvm_perbin_esigmaphotonxf_2026-10-27/ (its rows are unchanged by this edition).

## 3. Headline numbers
| quantity | h+ | h- |
|---|---|---|
| total quoted uncertainty, median over 245 bins | 8.595% | 9.323% |
| 2026-10-27 edition, same median | 8.597% | 9.324% |
No bin's total increases (max decrease 0.59 pp). Charge-ratio and A_h uncertainties: no cell increases; median ratio 0.99998, max decrease 6.3%
(cell 118 S1_x1). Coverage gate unchanged (PASS, 0.668-0.695 per N >= 4 class). MAPTMD24 chi2 2540.9 / 230 (diagnostic).

## 4. Critic verdicts
Code critic PASS; hostile review (h1-analysis-critic) GO; h1-data-validator package 6/6 PASS.

## 5. Limitations
- In a few R/A_h cells the remaining nonprimary_int and env100_hyp terms are smaller than the removed rank-1 leg (up to 6.3% of the total); the
  removed piece reflected how the reroute assigns tracks in the response (an observable-definition choice), not a rate.
- The s4 hard gates attest the 2026-10-24 group-A definition; the detint identity of this edition is checked by s09 g2.
- Limitations of the 2026-10-27 record (one-sided closure bias in edge bins, closure / ibu_reg overlap, 4-5 class per-truth spread) still apply.

## 6. Not done in this record
Figures (P2), documents (P3), freeze (P4).

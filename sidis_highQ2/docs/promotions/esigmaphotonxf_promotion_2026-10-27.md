# Promotion record, eSigma release with the closure revision (tag esigmaphotonxf_2026-10-27)

Status: INSTALLED 2026-10-27 by s15_install_esigmaphotonxf_2026-10-27.py (Condor cluster 5911769): 106 files, verbatim copies, bitwise equal to
MANIFEST.sha256; 54 of 54 hard gates True; 2051 protected files of older editions and install records sha-swept before and after, unchanged.
Package: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_final_2026-10-24/closure_rev3_staging/phaseD/package/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_2026-10-27
(MANIFEST.sha256 sha 9078f0e4d82aca453996410a86a78b2a647005757a14732ddaa923defbec61f9, 176 entries: 106 installable, 69 provenance, 1 SOURCES.tsv).
Install record: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphotonxf_2026-10-27.json
(sha256 1980854c44ada4de6070a9d435cfa6cd8792c6a8c7a916b39bbe8d72dbde9832). Plan: /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/closure_uncertainty_revision_2026-10-26.md (rev 3).
Provenance notes: PROVENANCE_NOTES.md in the package.

Raw versus Born: "raw" is the unfolded value before C_rad; "Born" has C_rad applied. The budget is built against the raw release;
relative uncertainties apply to both. Covariance files and ratio_asym_columns.csv are at raw level.

## 1. What this promotion is
A new systematic budget and covariance on the unchanged 2026-10-24 release. Installed targets:
- output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-27
  (every file sha-equal to the 2026-10-24 release, 19 files)
- output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-27
- data/covariance/z005_covariance_optA_esigmaphotonxf_2026-10-27
- sidis_highQ2/data/radiative/cgammaborn_esigmaphotonxf_2026-10-27 (sha-equal to the 2026-10-24 table)
The 2026-10-24 editions and INSTALL_RECORD_esigmaphotonxf_2026-10-24.json are superseded for quoting, not modified or deleted.

## 2. What changed in plain words
- Central values, statistical uncertainties and Born corrections are unchanged.
- New quoted term: per-bin unfolding closure `unfold_closure_rel` (treatment R, plan D1-D3): the rms over the djangoh and datalike truths of
  the toy-mean bias of the k=4 unfolding, relative to the release yield; covariance `cov_unfold_closure` (rank 2, correlated across charges).
- Response-MC statistics enter the quoted total as `response_mcstat_rel` (plan D4), replacing `fakecol_mcstat_rel` (now INFO).
- The DIFFVM rho0 +-100% term is removed from the quoted budget and the covariance (user 2026-10-27); `diffvm_rho_rel` stays as an INFO column.
  A per-bin DIFFVM table (fraction, recommended 1 sigma, UL95, sign) is provided separately:
  sidis_highQ2/data/backgrounds/diffvm_perbin_esigmaphotonxf_2026-10-27/ and sidis_highQ2/docs/backgrounds/diffvm_perbin_table_esigmaphotonxf_2026-10-27.md.
- Term counts: 26 quoted sources, 28 covariance terms.

## 3. Headline numbers (from the installed products)
| quantity | h+ | h- |
|---|---|---|
| total quoted uncertainty, median over 245 bins | 8.60% | 9.32% |
| previous edition (2026-10-24), same median | 8.13% | 8.92% |
| unfold_closure_rel, median | 1.28% | 1.09% |
| response_mcstat_rel, median | 0.83% | 0.89% |
Per-bin total, new/old: median x1.016, p90 x1.156, max x1.88. h- bin 30 total 39.7% -> 24.5% (DIFFVM row removed; closure 14.1% added).
Closure / sigma_stat median 0.32 (0.71 for N < 10). Charge-ratio total uncertainty x1.005 median.
Coverage gate (plan D6, held-out families, each N >= 4 class >= 0.66): 0.668, 0.670, 0.695, 0.680, 0.670, 0.683, 0.694 (PASS).
MAPTMD24 chi2/ndf (diagnostic): 2513/230 (2026-10-24) -> 2533/230.

## 4. User decisions (stated, not reopened)
Treatment R, kappa = 1, new-tag install after the coverage gate and hostile review (2026-10-26); response-MC statistics in the quoted totals;
DIFFVM not subtracted and no DIFFVM uncertainty term, per-bin table instead (2026-10-27), with an explicit sign column (2026-10-27);
no N < 10 flags; install sign-off 2026-10-27 ("Install, after fixing F7-F9").

## 5. Critic verdicts
Code: critic PASS (round 1), h1-analysis-critic PASS, scope-critic TRIM. Hostile review (h1-analysis-critic): GO, no blocking finding.
h1-data-validator: package 6/6 PASS, re-check after reassembly 4/4 PASS.

## 6. Limitations
- In bins where the closure exceeds sigma_stat, both truth legs point the same way (e.g. h- bin 109, closure 105%): a one-sided bias quoted symmetrically.
- Closure and ibu_reg partly overlap (cosine -0.90 of their shift vectors); the calibration includes both.
- The 4-5 count class passes the coverage gate with a pooled 0.668; per truth it ranges from 0.54 (stressA) to 0.74 (rapgap).
- Several product metadata strings are stale (PROVENANCE_NOTES.md); no number is affected.

## 7. Gates
S0 reuse proof; closure term reproduces the study exactly; group A unchanged columns bit-identical to 2026-10-24; s08 8/8; s09 13/13;
coverage gate PASS; s15 54/54 hard gates including closure_coverage_gate_PASS and package_release_identical_to_REL24.

## 8. Not done in this record
Figures (program plan P2), documents (P3), freeze (P4).

# Plots Directory Layout Standard — sidis_highQ2/

**Date:** 2026-05-24
**Scope:** `H1/sidis_highQ2/plots/` (charged-particle SIDIS, Q² > 150 GeV²)
**Status:** Standard adopted; loose top-level PDFs migrated; producing scripts updated.

---

## Convention

Every plot **product** under `sidis_highQ2/plots/` lives in its own
subdirectory. No loose `.pdf` (or `.png`) files at the top level. This holds
even for single-PDF products: the subdirectory contains just that one PDF.

Subdirectory names use lowercase / mixed-case underscores, and follow these
patterns:

| Pattern              | Example                                       | When to use                                    |
| -------------------- | --------------------------------------------- | ---------------------------------------------- |
| `<name>`             | `acceptance_v5f/`                             | Generic product, no version/date qualifier     |
| `<name>_v<X>`        | `v5g_unfolded_IBU_v3/`                        | Versioned products (binary version + script v) |
| `<name>_<YYYY-MM-DD>`| `binning_study_2026-05-24/`                   | Dated, snapshot-style products                 |
| `diag_<name>...`     | `diag_high_pTh_2026-05-24/`                   | Diagnostic / sanity-check products             |

**Single-PDF rule:** when a producer script emits exactly one PDF named
`foo.pdf`, the destination is `plots/foo/foo.pdf` (subdir basename = file
basename without `.pdf`).

---

## Migration table — loose files → subdirs (2026-05-24)

All 15 PDFs that were previously dumped directly into `plots/` were moved
(never deleted, never re-rendered) into their own subdirectories. File
contents and byte sizes are preserved exactly.

| Source (was)                                       | Destination (now)                                                                       | Size (bytes) |
| -------------------------------------------------- | --------------------------------------------------------------------------------------- | -----------: |
| `plots/binning_finalization_HQ-v5g-Bprime.pdf`     | `plots/binning_finalization_HQ-v5g-Bprime/binning_finalization_HQ-v5g-Bprime.pdf`       |       76,403 |
| `plots/binning_finalization_HQ-v5g-Bprime_v2.pdf`  | `plots/binning_finalization_HQ-v5g-Bprime_v2/binning_finalization_HQ-v5g-Bprime_v2.pdf` |       85,607 |
| `plots/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf`  | `plots/gen_fiducial_acceptance_HQ-v5g-Bprime/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf` |      106,380 |
| `plots/gen_fiducial_acceptance_compare_DJ_RG.pdf`  | `plots/gen_fiducial_acceptance_compare_DJ_RG/gen_fiducial_acceptance_compare_DJ_RG.pdf` |      160,383 |
| `plots/v5g_crad_4D_v3.pdf`                         | `plots/v5g_crad_4D_v3/v5g_crad_4D_v3.pdf`                                               |      137,225 |
| `plots/v5g_migration_matrix_4D.pdf`                | `plots/v5g_migration_matrix_4D/v5g_migration_matrix_4D.pdf` (merged into existing empty subdir) | 406,901 |
| `plots/v5g_multiplicities.pdf`                     | `plots/v5g_multiplicities/v5g_multiplicities.pdf`                                       |      110,732 |
| `plots/v5g_multiplicities_v2.pdf`                  | `plots/v5g_multiplicities_v2/v5g_multiplicities_v2.pdf`                                 |      118,172 |
| `plots/v5g_multiplicities_v3.pdf`                  | `plots/v5g_multiplicities_v3/v5g_multiplicities_v3.pdf`                                 |      118,267 |
| `plots/v5g_phih_harmonics.pdf`                     | `plots/v5g_phih_harmonics/v5g_phih_harmonics.pdf`                                       |      224,246 |
| `plots/v5g_purity_4D_v3.pdf`                       | `plots/v5g_purity_4D_v3/v5g_purity_4D_v3.pdf`                                           |      130,478 |
| `plots/v5g_unfolded_IBU_v3.pdf`                    | `plots/v5g_unfolded_IBU_v3/v5g_unfolded_IBU_v3.pdf`                                     |      268,456 |
| `plots/v5g_unfolded_TUnfold_v3.pdf`                | `plots/v5g_unfolded_TUnfold_v3/v5g_unfolded_TUnfold_v3.pdf`                             |      286,143 |
| `plots/v5g_xsec_4D_v3.pdf`                         | `plots/v5g_xsec_4D_v3/v5g_xsec_4D_v3.pdf`                                               |      182,624 |

Operation used exclusively: `mv`. No file was deleted; no PDF was re-rendered.
The existing empty `plots/v5g_migration_matrix_4D/` subdirectory was reused
as the destination (no name collision because it was empty).

Pre-existing subdirectories that already followed the convention
(`acceptance_v5f/`, `abc_dis_*/`, `binning_study_2026-05-24/`,
`crad_v5f_strategy2_real/`, `diag_high_pTh_2026-05-24/`,
`phi_h_moments_v5f_no_ep0304_v*/`, `systematics_v5f*/`, `unfolding_v5f/`,
etc.) were left untouched. Two pre-existing empty subdirs
(`crad_v5f_strategy2/`, `diag_unfolding_BBB_normalization/`) were also
preserved as-is — empty directories are not files and are not removed
under the "never delete" rule.

---

## Script-side updates — output path strings (2026-05-24)

The following scripts had their PDF-output path strings rewritten so that
re-running them lands the PDF in the new subdirectory automatically. Only
the path strings changed; no logic was modified, and no binary was touched.

Each `mkdir(parents=True, exist_ok=True)` call already present in these
scripts will create the new subdirectory on first re-run.

| Script                                                                                | Old path (was)                                                       | New path (now)                                                                                        |
| ------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- |
| `scripts/plot_v5g_purity_4D.py`                                                       | `plots/v5g_purity_4D_v3.pdf`                                         | `plots/v5g_purity_4D_v3/v5g_purity_4D_v3.pdf`                                                         |
| `scripts/plot_v5g_phih_harmonics.py`                                                  | `plots/v5g_phih_harmonics.pdf`                                       | `plots/v5g_phih_harmonics/v5g_phih_harmonics.pdf`                                                     |
| `scripts/plot_v5g_multiplicities.py`                                                  | `plots/v5g_multiplicities.pdf`                                       | `plots/v5g_multiplicities/v5g_multiplicities.pdf`                                                     |
| `scripts/plot_v5g_multiplicities_v2.py`                                               | `plots/v5g_multiplicities_v2.pdf`                                    | `plots/v5g_multiplicities_v2/v5g_multiplicities_v2.pdf`                                               |
| `scripts/plot_v5g_multiplicities_v3.py`                                               | `plots/v5g_multiplicities_v3.pdf`                                    | `plots/v5g_multiplicities_v3/v5g_multiplicities_v3.pdf`                                               |
| `scripts/plot_v5g_migration_matrix_4D.py`                                             | `plots/v5g_migration_matrix_4D.pdf`                                  | `plots/v5g_migration_matrix_4D/v5g_migration_matrix_4D.pdf`                                           |
| `scripts/compute_xsec_v3_HQv5gBprime.py`                                              | `plots/v5g_xsec_4D_v3.pdf`                                           | `plots/v5g_xsec_4D_v3/v5g_xsec_4D_v3.pdf`                                                             |
| `scripts/compute_crad_v3_HQv5gBprime.py`                                              | `plots/v5g_crad_4D_v3.pdf` (default of `--output-pdf`)               | `plots/v5g_crad_4D_v3/v5g_crad_4D_v3.pdf`                                                             |
| `scripts/diagnose_gen_fiducial_acceptance.py`                                         | `plots/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf`                    | `plots/gen_fiducial_acceptance_HQ-v5g-Bprime/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf`               |
| `scripts/diagnose_gen_fiducial_acceptance_compare_DJ_RG.py`                           | `plots/gen_fiducial_acceptance_compare_DJ_RG.pdf`                    | `plots/gen_fiducial_acceptance_compare_DJ_RG/gen_fiducial_acceptance_compare_DJ_RG.pdf`               |
| `scripts/binning_finalization_study_HQ-v5g-Bprime.py`                                 | `plots/binning_finalization_HQ-v5g-Bprime.pdf`                       | `plots/binning_finalization_HQ-v5g-Bprime/binning_finalization_HQ-v5g-Bprime.pdf`                     |
| `scripts/binning_finalization_study_HQ-v5g-Bprime_v2.py`                              | `plots/binning_finalization_HQ-v5g-Bprime_v2.pdf`                    | `plots/binning_finalization_HQ-v5g-Bprime_v2/binning_finalization_HQ-v5g-Bprime_v2.pdf`               |
| `scripts/unfolding/unfold_IBU_v3.py`                                                  | `sidis_highQ2/plots/v5g_unfolded_IBU_v3.pdf` (default of `--out-pdf`) | `sidis_highQ2/plots/v5g_unfolded_IBU_v3/v5g_unfolded_IBU_v3.pdf`                                      |
| `scripts/unfolding/unfold_TUnfold_v3.py`                                              | `sidis_highQ2/plots/v5g_unfolded_TUnfold_v3.pdf` (default of `--out-pdf`) | `sidis_highQ2/plots/v5g_unfolded_TUnfold_v3/v5g_unfolded_TUnfold_v3.pdf`                          |

In addition, matching docstring / comment / runtime-emitted-Markdown
references inside the same script files were updated to reflect the new
paths, so any reproducibility blocks regenerated by the binning_finalization
scripts will quote the correct new locations.

---

## Re-run behavior

Re-running any of the updated scripts above will:
1. Use the new path in its `OUT_PDF` / `PDF_OUT` / `--out-pdf` / `--output-pdf` variable.
2. Hit the existing `mkdir(parents=True, exist_ok=True)` call (already in
   every script — no script logic was modified to add this).
3. Write the PDF to the correct new subdirectory location.

No further user action is needed.

---

## Hard constraints honored

- No file was deleted (only `mv` used).
- No PDF content was altered (only file path changed).
- No script logic was modified — only string assignments for output paths
  (and matching docstring / comment references).
- No agent definition files were touched.
- All edits were confined to `sidis_highQ2/plots/`, `sidis_highQ2/scripts/`,
  and this new memo under `sidis_highQ2/docs/methodology/`.

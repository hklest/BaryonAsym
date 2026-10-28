# 210-bin high-Q² charged-SIDIS TUnfold AREA release (INTERNAL / PRELIMINARY)

**Date:** 2026-06-02
**Status:** Internal candidate for inspection. Do not circulate outside the workspace.

## What this is

A 210-bin "stitched" TUnfold AREA result for the high-Q² charged-particle
SIDIS analysis, built as a postprocessing step on already-produced TUnfold
JSON outputs. Per-charge layout:

| Bins | Source | Notes |
|---|---|---|
| 0..179 | rev6 cells 0..5 from `per_bin_sigma_tunfold_rev6_area_{pos,neg}.json` | 6 cells × 30 bins, **unchanged** from the full rev6 area run |
| 180..209 | S3merged AREA TUnfold result, x integrated over S3a+S3b | 30 bins (6 z × 5 pT) at the multi-objective selected τ |
| **Total** | | **210 bins per charge** |

Selected AREA τ for the S3merged block (multi-objective scan,
`S3_tau_multiobjective_summary.json`):

- h+ : τ = 3.162 × 10⁻³
- h- : τ = 1.000 × 10⁻³

S3 stratum binning convention: Q² ∈ [1500, 20000] GeV², x ∈ [0.021, 1.5],
z bins [0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0], pT_h bins [0, 0.5, 1, 2, 4, 8]
GeV. The S3merged block is labelled `x integrated (S3a+S3b)` everywhere
and the binning table carries `x_merged: true`.

## Why this is preliminary

This is the user-and-supervisor-agreed compromise that avoids pretending S3
is x-differential while S3b remains unstable. Per the multi-objective
scan, S3a (cell 6) and S3b (cell 7) treated independently both trigger
hard-reject or persistent pathology even at their best τ; the x-merged
case is the only operating point that produces a non-rejected operating
point under the area constraint. The high-z half of the merged block
remains noisy (`n_negative_zhi > 0` in the multi-objective metrics) and
is masked conservatively here.

## Uncertainty decomposition for the S3merged block

Per-bin uncertainties for the 30 S3merged bins were computed at the
selected τ using the same definitions as the rev6 per-bin σ pipeline:

- `sigma_stat`  : `h_unfolded_pub` bin error (`sigma_pub` from the
  selected-τ scan JSON, `S3merged_tunfold_area/scan/`).
- `sigma_reg`   : 0.5 × |unf(τ_high) − unf(τ_low)| on the geometric τ grid
  (sqrt(10) spacing). For h+: τ ∈ {1.778e-3, 3.162e-3, 5.623e-3}. For
  h-: τ ∈ {5.623e-4, 1.000e-3, 1.778e-3}.
- `sigma_model` : 0.5 × |unf_RG − unf_DJ| at the selected τ, using the
  Djangoh-response scan (`S3merged_tunfold_area/scan_dataDJ/`).
- `sigma_total` : root-sum-square of the three above.

These mirror the conventions documented in
`per_bin_sigma_tunfold_rev6_area_*.json` `sigma_*_definition` fields.

No additional uncertainty components (cross-Q² migration, fake-scale,
detector-level systematics, charge confusion, …) are folded into this
release: it is a TUnfold-only product. Use it for *binning-and-yield*
inspection, not for a final uncertainty budget.

## Trust mask

- Bins 0..179 inherit the rev6 trust mask verbatim
  (`untrust if mc_count_rapgap<50 OR (mc_count_rapgap<500 AND
  reco_eff<0.05) OR reco_eff>1`).
- Bins 180..209 (S3merged) use a conservative mask: **all z ≥ 0.5 bins
  (S3merged local bins 20..29) masked outright**; z < 0.5 bins (local
  bins 0..19) kept trusted unless the central unfolded value is
  non-positive.
- `trusted_physical` = `trusted AND unfolded > 0`.

Per-charge mask counts:

| Charge | trusted | trusted_physical | S3merged trusted | S3merged masked |
|---|---|---|---|---|
| h+ | 199 / 210 | 185 / 210 | 20 / 30 | 10 / 30 |
| h- | 196 / 210 | 183 / 210 | 20 / 30 | 10 / 30 |

(All S3merged-masked counts of 10 are exactly the z ≥ 0.5 half by
construction; no zlo bin needed extra masking for either charge.)

## Files

Release directory (canonical local):
`/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_210bin_INTERNAL_PRELIMINARY/`

| File | Description |
|---|---|
| `binning_210.{json,csv}` | 210-row binning table with q²/x/z/pT ranges, source provenance, `x_merged` flag |
| `unfolded_yields_210_{pos,neg}.{json,csv}` | Per-charge 210-bin yields with σ_stat/σ_reg/σ_model/σ_total, trust flags |
| `trust_mask_210_{pos,neg}.json` | Trust mask + `trusted_physical` flag + definitions |
| `release.json` | Metadata, taus, input provenance, multi-objective selected metrics, totals (release metadata only) |
| `README.md` | Short release readme |
| `manifest.json` | File list with SHA-256 hashes |
| `plots/pth_210_results_{pos,neg}.pdf` | pT_h pages per (cell, z); 7 pages each (6 retained cells + 1 S3merged) |
| `plots/release_210_summary.pdf` | Header, integrated-totals bar chart per cell, rel σ_total histograms |
| `plots/pth_210_data_mc_context_SKIPPED.txt` | Note explaining the data/MC overlay was not produced (see below) |

Remote inputs consumed:
- `naf:output/tunfold_tree_v3_percharge/per_bin_sigma_tunfold_rev6_area/per_bin_sigma_tunfold_rev6_area_{pos,neg}.json`
- `naf:output/tunfold_tree_v3_percharge/S3merged_tunfold_area/scan/tunfold_{pos,neg}_tau{5.623e-04,1.000e-03,1.778e-03,3.162e-03,5.623e-03}.{json,root}`
- `naf:output/tunfold_tree_v3_percharge/S3merged_tunfold_area/scan_dataDJ/...` (same taus)
- `naf:output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge_NWfix.root.binmap_v3.json`
- `naf:output/tunfold_tree_v3_percharge/S3merged_tunfold_area/inputs/S3merged_inputs_summary.json`
- `naf:output/tunfold_tree_v3_percharge/S3_tau_multiobjective/S3_tau_multiobjective_summary.json`

Postprocessing script:
- `H1/sidis_highQ2/scripts/analysis/build_210bin_tunfold_release.py`
- mirrored to `naf:scripts/analysis/build_210bin_tunfold_release.py`

Plot mirror:
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/release_210/`

## What is NOT in this release

- **No data/MC overlay PDF.** The 210-bin scheme is not bin-aligned with
  any single existing MC truth product (the rev6 240-bin scheme is, but
  the S3merged 30-bin sub-block has its own merged truth from the
  S3merged inputs, and we did not aggregate the two onto the 210-bin
  grid in this postprocessing pass). A `*_SKIPPED.txt` note is shipped
  in `plots/`.
- **No detector-level systematics, no cross-Q² migration uncertainty,
  no fake-scale uncertainty, no charge-confusion uncertainty.** These
  are TUnfold-only σ_stat/σ_reg/σ_model values.
- **No merged covariance.** The rev6 cells 0..5 covariance and the
  S3merged covariance live in separate TUnfold runs; we do not block-
  diagonalise them here. Use per-bin σ only.
- **No headline integrated-cross-section claim.** Totals are exposed
  in `release.json` and on the summary PDF as release metadata only,
  consistent with the 2026-06-01 methodology correction.

## Future work (out of scope for this release)

- Merge any bins with > 100 % model or statistical relative uncertainty,
  per the supervisor's note. Not done here; just inspectable from
  `unfolded_yields_210_*.csv` (`rel_sigma_total` column).
- Add detector-level systematics, fake-scale, charge-confusion,
  cross-Q² migration uncertainty.
- Produce a single 210×210 block-diagonal covariance.
- Decide whether to include the S3merged zhi half at all in any
  downstream observable.

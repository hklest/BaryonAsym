# qT/Q purity and acceptance twin — figure specification

**Status:** PROVISIONAL INTERNAL DIAGNOSTIC specification only.  No figure is certified or to be rendered from the current merged product.

## Purpose and provenance

This is the like-for-like qT/Q counterpart to the diagnostic alpha0p2 all-fake pT,h response-metric family produced by `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/qa_acceptance/plot_purity_vs_pTh_7col_alpha0p2.py`.  It is a RAPGAP31 response diagnostic, not a central-release, background-subtracted, radiatively corrected, Born-level, or physics-approved figure.  The title/footer and README/manifest must state: `PROVISIONAL INTERNAL DIAGNOSTIC | RAPGAP31 | qT/Q = pT,h^CM/(z Q) | released z scope 0 <= z < 1`.

Input after correction/reproduction: a corrected counterpart of the current 4D merged study product
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/qt_over_Q_resolution_4d_zscope_20261007/merged/qt_over_Q_resolution_4d_zscope_rapgap31_fullpool_merged.root`
and its JSON manifest.  The current file is **not usable**: its `response4d_*` objects place fake on the gen axis and miss on the reco axis.  This is the confirmed bookkeeping bug under repair.  The corrected reproduction must instead provide a fake vector indexed by reco qT/Q bin and a miss vector indexed by gen qT/Q bin, with recorded orientation and exact closure checks.

## Common panel layout

Make one one-page PDF and PNG for each charge and metric: purity h+, purity h-, acceptance h+, acceptance h-.  Each page has a **7-column by 7-row grid (49 panels)**.  Columns are the seven released z intervals `[0,.05), [.05,.10), [.10,.20), [.20,.30), [.30,.50), [.50,.70), [.70,1.00)`; rows are the seven `(Q2,x)` cells in this order: `(150,300): [.0021,.0071), [.0071,.03)`; `(300,700): [.0042,.016), [.016,.069)`; `(700,1500): [.0099,.034), [.034,.15)`; `(1500,20000): [.021,1.0)`.

Every panel contains five step-and-marker points versus **qT/Q (dimensionless)**.  Its annotation gives `Q2`, `x`, `z`, charge, generator, and `qT/Q quantile bins from gen distribution`.  The y label is `purity` or `acceptance`; y range is `[0,1.2]`.  Use the canonical pT-family typography, dotted grid, and first-panel legend.  A side-by-side pT,h comparator has the same charge/metric/page/panel order and y range, but its x axis is `$p_{T,h}$ [GeV]` with fixed bins `[0,.5), [.5,1), [1,2), [2,4), [4,8)`; the qT/Q side uses cell--z-specific edges and labels them in its table/manifest, not a misleading shared numerical tick scheme.

## Binning, quantities, and required keys

For each `(cell,z,charge)`, derive five qT/Q intervals from the **weighted generator qT/Q marginal** of that series: edges are the 0th, 20th, 40th, 60th, 80th, and 100th percentiles of in-axis generator qT/Q, after deterministic handling of repeated quantiles (merge duplicate native intervals and record any unsupported five-bin request).  Coarsen the native body matrix and category vectors by these intervals on their respective axes.  Use the same per-series five-bin boundaries for the reco display mapping; record the reco-bin aggregation rule and any endpoint/out-of-axis exclusion.  No pooled, global, or pT-derived edges are allowed.

For coarsened bin `k`, define `D_k` as the sum of body entries whose gen and reco qT/Q values both map to `k`.  The metrics are exactly:

* `purity_k = D_k / (D_k + fake_reco_k)`;
* `acceptance_k = body_from_gen_k / (body_from_gen_k + miss_gen_k)`, where `body_from_gen_k` sums all reco destinations from gen bin `k`.

The corrected ROOT must provide `response4d_<S0_x0..S3_x0>_z<0..6>_<pos|neg>` (98 objects) with an explicitly documented body matrix, `fake_reco` vector, and `miss_gen` vector.  The corrected manifest must provide `binning.qt_gen_edges`, `binning.qt_reco_edges`, `binning.cells`, `binning.z_edges_release`, `zdifferential_4d.response_series`, and the per-series orientation/closure declaration.  The plotter must fail closed if any required object/key, 98-series count, quantile edge, finite denominator, or closure gate is missing.

## Output contract after corrected re-production

Remote study output: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/qt_over_Q_purity_acceptance_5bin_quantile_z005_<DATE>/plots/`.

Local review package: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/response_diagnostics/qt_over_Q_purity_acceptance_5bin_quantile_z005_<DATE>/`.

Required names must carry the explicit scope token `released_z_0_to_1` (never `_z005`, since `zlow005` is a path tag, not a z cut), the generator (`RAPGAP31`), charge, and metric. PDFs are the review masters; manifests and a bundle accompany them.

## Implementation status — 2026-10-07

- **Exists:** `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/analysis/qt_over_Q_resolution_2026-10-07/plot_qT_over_Q_purity_acceptance_canonical_twin.py`. It mirrors the canonical 7×7 response-metric layout, accepts corrected Encoding-B ROOT/NPZ products or the declared coarsened-table schema, supports `--bins 5` (default) / `--bins 8`, and `--paired-pth`. It fails closed unless every declared series says `fake_axis=reco` and `miss_axis=gen`.
- **Waiting:** corrected RAPGAP31 4D re-production and manifest at `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/qt_over_Q_resolution/2026-10-07/purity_acceptance_4d/` (or the corrected study ROOT plus manifest). DJANGOH14 is not present in this pool.
- **Real-figure command:**
  ```bash
  MPLBACKEND=Agg python3 /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/analysis/qt_over_Q_resolution_2026-10-07/plot_qT_over_Q_purity_acceptance_canonical_twin.py --input /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/qt_over_Q_resolution/2026-10-07/purity_acceptance_4d/qt_over_Q_resolution_4d_zscope_rapgap31_corrected.root --manifest /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/qt_over_Q_resolution/2026-10-07/purity_acceptance_4d/manifest.json --bins 5 --paired-pth --output-dir /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/qt_over_Q_resolution/2026-10-07/purity_acceptance_5bin_quantile_released_z_0_to_1_RAPGAP31/
  ```
  Use `--bins 8` only for the declared eight-bin diagnostic variant.

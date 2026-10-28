# Unfolding pipeline: FINAL (2026-05-25)

## Canonical methods
- BBB-4D, IBU-4D, TUnfold-4D, all on full 480×480 response
- Primary: IBU-4D (auto k=5, 100-replica bootstrap)
- Cross-check: TUnfold-4D (τ=0.01, L_within + L_xboundary, analytic Cov)

The sliced per-(Q²,x,charge) unfolding chain is SUPERSEDED. Full 4D unfolding is
now the only canonical migration-correction pipeline for new high-Q² SIDIS
results.

## Systematic budget (4-term, per H1 2403.10134 convention)
σ²_unfold = σ²_stat,data + σ²_model + σ²_reg + σ²_MC-stat,response

Inputs:
- σ_model: `H1/sidis_highQ2/data/unfolded_4d_v3_data_DJvsRG.json`
- σ_reg: `H1/sidis_highQ2/data/sigma_reg_TUnfold_4d.json`
- σ_MC-stat,response: `H1/sidis_highQ2/data/sigma_MCstat_TUnfold_4d.json`
- Trust mask: `H1/sidis_highQ2/data/v5g_4d_trust_mask.json`

Headline trusted-bin summaries:
- mean rel σ_model: IBU 1.87%, TUnfold 2.86%
- mean rel σ_reg: 1.50%, p90 3.82%
- mean rel σ_MC-stat,response: 7.36%
- median σ_MC/σ_stat = 0.33

## Per-cell headline numbers

| Cell | stat | model | reg | MC-stat,response | Dominant |
|------|------|-------|-----|------------------|----------|
| S0a h+ | 0.42% | 0.99% | 0.02% | 0.13% | model |
| S0b h+ | 0.56% | 2.25% | 0.02% | 0.19% | model |
| S1a h+ | 0.67% | 1.79% | 0.08% | 0.29% | model |
| S1b h+ | 0.93% | 3.12% | 0.16% | 0.42% | model |
| S2a h+ | 1.23% | 2.67% | 0.30% | 0.54% | model |
| S2b h+ | 2.50% | 3.06% | 1.83% | 1.72% | model |
| S3a h+ | 1.88% | 3.22% | 0.33% | 1.94% | model |
| S3b h+ | 6.40% | 3.88% | 2.95% | 8.04% | MC-stat,response |
| S0a h- | 0.42% | 1.00% | 0.02% | 0.18% | model |
| S0b h- | 0.57% | 2.36% | 0.03% | 0.31% | model |
| S1a h- | 0.62% | 1.80% | 0.03% | 0.22% | model |
| S1b h- | 0.91% | 3.20% | 0.05% | 0.39% | model |
| S2a h- | 1.64% | 2.86% | 0.41% | 0.75% | model |
| S2b h- | 2.36% | 3.06% | 1.42% | 1.09% | model |
| S3a h- | 1.65% | 2.98% | 0.17% | 1.62% | model |
| S3b h- | 5.79% | 2.87% | 2.67% | 5.70% | stat |

Headline: **σ_model dominates 14/16 cells. σ_MC-stat dominates only S3b h+
(high-x highest-Q²). σ_stat dominates only S3b h-.**

## Sliced products SUPERSEDED
Preserved for audit, method comparison, and historical reproducibility, but not
used in any new derivation:

- `H1/sidis_highQ2/data/unfolded_v3_IBU_HQv5gBprime.json`
- `H1/sidis_highQ2/data/unfolded_v3_TUnfold_HQv5gBprime.json`
- `H1/sidis_highQ2/data/unfolded_v3_TUnfold_HQv5gBprime_covariance.npz`
- `H1/sidis_highQ2/data/unfolded_v3_OmniFold_HQv5gBprime.json`
- `H1/sidis_highQ2/data/v5g_unfolding_IBU_closure_crossMC.json`
- `H1/sidis_highQ2/data/v5g_unfolding_TUnfold_closure_crossMC.json`
- `H1/sidis_highQ2/scripts/unfolding/unfold_IBU_v3.py`
- `H1/sidis_highQ2/scripts/unfolding/unfold_TUnfold_v3.py`
- `H1/sidis_highQ2/scripts/unfolding/unfold_OmniFold_v3.py`
- `H1/sidis_highQ2/scripts/unfolding/closure_IBU_crossMC.py`
- `H1/sidis_highQ2/scripts/unfolding/closure_TUnfold_crossMC.py`
- PDFs under `plots/v5g_unfolded_IBU_v3/`, `plots/v5g_unfolded_TUnfold_v3/`,
  `plots/v5g_unfolded_OmniFold_v3/`, `plots/v5g_TUnfold_covariance/`,
  `plots/v5g_unfolding_IBU_closure_crossMC*/`,
  `plots/v5g_unfolding_TUnfold_closure_crossMC*/`,
  `plots/v5g_unfolding_closure_ratio_summary/`,
  `plots/v5g_unfolding_comparison_3method_v3/`,
  `plots/v5g_unfolding_comparison_3method_v4/`, and `plots/TUnfold_scan_QA/`

Audit copies with explicit historical tags remain untouched:
- `unfolded_v3_TUnfold_HQv5gBprime.BEFORE-cov-export.json`
- `unfolded_v3_TUnfold_HQv5gBprime.OLD-WRONG-tau-Lcurve.json`

## OmniFold-4D: FUTURE WORK
Not yet implemented. Sliced OmniFold-v3 is preserved for audit but not part of
the canonical pipeline.

## Trust mask
353 of 480 bins (74%) pass. Mask criteria:
- N_data ≥ 10
- σ/N < 0.5 for both IBU-4D and TUnfold-4D
- purity ≥ 0.5 AND bbb_valid
- ε_h > 0
- 0.5 ≤ (4D/sliced ratio) ≤ 2.0
- N_unfold ≥ 0

127 excluded. Worst-affected cells: S3b (15-16 excluded), S2b (14), S3a (11).

## Catastrophic bin diagnosis
98 per-bin catastrophes. Mechanism summary:
- 65% (64/98) are simple data sparsity (N_data < 10)
- 15/98 with N_data ≥ 100 are TUnfold-only acceptance-smearing pathology;
  BBB and IBU-4D stable on all of these

## Cross-references
- 4D pipeline scripts: `H1/sidis_highQ2/scripts/unfolding/{build_response,unfold_{BBB,IBU,TUnfold}}_4d.py`
- σ_model: `H1/sidis_highQ2/data/unfolded_4d_v3_data_DJvsRG.json`
- σ_reg: `H1/sidis_highQ2/data/sigma_reg_TUnfold_4d.json`
- σ_MC-stat: `H1/sidis_highQ2/data/sigma_MCstat_TUnfold_4d.json`
- Trust mask: `H1/sidis_highQ2/data/v5g_4d_trust_mask.json`
- Catastrophic diag: `H1/sidis_highQ2/data/v5g_catastrophic_4D_bins_diag.json`
- 4D methodology: `H1/sidis_highQ2/docs/methodology/unfold_4d_full_2026-05-25.md`
- Research basis: `H1/sidis_highQ2/docs/research/unfolding_systematic_decomposition_2026-05-24.md`

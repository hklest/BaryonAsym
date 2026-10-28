# `nonprimary_norm_rel`: extra uncertainty for the non-primary track rate (DIAGNOSTIC, not installed)

> **WITHDRAWN 2026-10-21 (v1 classification).** A triple-check (`docs/trackclass_triplecheck_2026-10-21.md`) found real primary tracks in the v1 background classes: Tier-2 matches, P/GenP > 1.5 tracks (badly measured real particles) and part of the large-angle class. The numbers below are superseded by the v2 results; do not quote them.


Date: 2026-10-20. Builds on `docs/trackclass_impact_2026-10-20.md`.

- NAF package: `H1ChargedSidis_HighQ2/output_v6/studies/trackclass_variant_2026-10-20/nonprimary_norm_term/`.
  It contains a README, `budget_optA_c1fix_plus_nonprimary_norm.csv`,
  `nonprimary_norm_covariance.npz` and `summary.json`.
- Local copy: `data/nonprimary_norm_term/`.
- Script: `scripts/trackclass_variant/make_nonprimary_norm_term.py`, run as Condor 5803779
  (exit 0, 153 s).

## Definition

- **What it covers:** the MC-predicted rate of every non-primary class removed through the
  response fake column:
  - det.int. (S2' fc3/4/5);
  - absorbed secondaries (fc6), Tier-2 matches (fc7), the K± keeper (fc8) and split duplicates
    (fc9).
- **Per unfolded bin:**
  - rel(κ) = y(F + κC)/y(F) − 1, using IBU k=4, the coherent prior and the official `data_sub′`
    on the variant RAPGAP31 response;
  - the component is κ · max(|rel(+1)|, |rel(−1)|), with **κ = 1 (±100%)** by default;
  - it is linear in κ, so `nonprimary_norm_rel_per_unit_kappa` rescales it.
- **Correlation:** fully correlated across all 490 bins (both charges). The signed vector gives
  cov = v vᵀ. The sign is the same in 488/490 bins, so the term behaves as a coherent
  normalization.
- **Sub-components** with the same construction: `nonprimary_norm_detint_rel` and
  `nonprimary_norm_new_rel`. They are additive to 0.4%.
- **Totals:**
  - `total_with_nonprimary_rel` = hypot(`total_with_env100_c1fix_rel`, term);
  - `total_quoted_with_nonprimary_rel` = hypot(`total_quoted_rel`, term).
  The base budget cells are carried verbatim (490/490), and quadrature closure is exact.

## Results (κ = 1)

| | h+ | h− |
|---|---:|---:|
| **coherent integrated** | **±2.83%** | **±2.19%** |
| … det.int. part / new-class part | 0.84% / 1.94% | 0.75% / 1.41% |
| DJ14 cross-check (integrated) | ±3.04% | ±2.33% |
| per-bin median | 1.22% | 1.09% |
| per-bin p95 / max | 5.7% / 24.9% | 10.4% / 47.4% |
| MC-stat of the term (median) | 0.10% | 0.11% |
| bins where MC-stat > ½ term | 4 | 4 |
| DJ14 / RG31 per-bin ratio (median) | 1.045 | 1.035 |
| `total_quoted_rel` median, before → after | 7.312% → **7.492%** (+0.18 pp) | 7.624% → **7.752%** (+0.13 pp) |
| increment on total: median / p95 / max | +0.09 / +1.10 / +6.6 pp | +0.08 / +1.15 / +16.2 pp |
| bins with total > 100% (before → after) | 1 → 1 | 0 → 0 |

**Where it is large.** The term is ~0.8–1.3% per bin for z1–z5. It is **~3.1–3.2% at the
highest z (z6)**, and largest in the S3 (highest-Q²) z6 cells:

| bin | term | MC stat | dominant part | DJ | total before → after |
|---|---:|---:|---|---:|---|
| neg b244 | 47.4% | ±6.5% | new classes 41.8% | 47.6% | 61% → 77% |
| neg b243 | 29.1% | ±3.9% | | | 46% → 55% |
| pos b244 | 24.9% | ±3.4% | | | 44% → 50% |

There, a small absolute fake content is a large fraction of a tiny high-z signal. The value is
stable across generators and well above its MC stat, so it is a real sensitivity, not noise.
It rises mildly with pT (h− pT4 median 1.6%).

## Caveats

- **Transfer to the nominal.** The rate shift is computed in the variant scheme, where these
  tracks are fakes, and applied as a relative uncertainty to the OPT-A nominal, where most are in
  the body. To first order both schemes respond equally to a rate mismodelling
  (`trackclass_impact_2026-10-20.md` Sec. 2).
- **Born level.** The relative shift is assumed level-invariant (C_rad multiplicative per bin).
  Not explicitly re-derived.
- **κ is a user decision.** ±100% follows the env100 fiat convention; the term scales linearly.
- **Relation to existing terms:**
  - `detint_removal_rel` (generator spread of the S2' removal) is a different source and is
    kept; there is no double count.
  - `fakecol_mcstat_rel` (MC stat of `f_i`) is also distinct.
  - The term's own MC stat (0.10%) is not added; it can be added in quadrature if wanted.
- **Not installed.** Installing it means a budget promotion: critic review, then a user decision
  and STATE.md/skill updates via `sidis-hq2-baseline-update`. Quoting is in any case blocked
  pending the OPT-A conditions.

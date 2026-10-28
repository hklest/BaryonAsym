# Impact of the track-class reclassification on the unfolded result, and the rate-shift systematic

> **WITHDRAWN 2026-10-21 (v1 classification).** A triple-check (`docs/trackclass_triplecheck_2026-10-21.md`) found real primary tracks in the v1 background classes: Tier-2 matches, P/GenP > 1.5 tracks (badly measured real particles) and part of the large-angle class. The numbers below are superseded by the v2 results; do not quote them.


Date: 2026-10-20. **DIAGNOSTIC**: no release, budget or STATE.md entry was changed.
Companion to `docs/track_truth_classification_study_2026-10-20.md`, which defines the classes, and
`docs/detint_uncertainty_audit_2026-10-20.md`, which covers the missing rate uncertainty.

## What was run

- **Variant response.** The OPT-A builder (`cone_s2prime_optA_2026-10-19`) was copied to
  `H1ChargedSidis_HighQ2/scripts/unfolding/trackclass_variant_2026-10-20/build_migration_trackclass_variant.py`
  with extra reroutes to the fake column:
  - fc6, absorbed: GTR-linked with a charge flip, or outside 0.7 < P/GenP < 1.5, or angle ≥ 10°;
  - fc7, Tier-2 matches;
  - fc8, the S2' keeper;
  - fc9, non-best duplicate legs.

  Rerouted legs record **no consumed key**, so their gen partner falls to the miss row unless a
  clean leg consumes it. The local copy is in `scripts/trackclass_variant/`.
- **Production.** Condor cluster **5803732**: 613 jobs, all exit 0, on the frozen 2026-10-11
  shard lists. Configs are `all` (RG + DJ); `absorbed`, `duplicate`, `tier2` and `keeper` (RG);
  `loose` (0.5–2, 20°) and `tight` (0.8–1.25, 5°) (RG); and `null` (all classes off).
  Outputs: `H1ChargedSidis_HighQ2/output_v6/studies/trackclass_variant_2026-10-20/shards/`.
- **Unfolding.** `lean_unfold.py`: the official engine (read-only import), IBU k=4, coherent
  body+miss prior, official `data_sub′`.
- **Analysis.** `impact_analysis.py`, Condor cluster **5803738** (691 s). Outputs in
  `.../trackclass_variant_2026-10-20/impact/`: `impact_summary.json`, `impact_per_bin.csv` and
  `rate_shift_per_bin.csv`.

### Validation (all passed)
| check | result |
|---|---|
| lean unfold vs official OPT-A IBU (per bin) | max rel. dev. 1.0e-15 / 1.5e-15 |
| Σ official RG shards vs merged response | identical (0.0) |
| variant builder, classes off vs official shard (micro-smoke and full shard 0) | 68/68 histograms bit-identical |
| variant, classes on (smoke) | total reco weight conserved exactly; prior unchanged; pool count identity holds; miss row grows |
| impact machinery, nominal vs nominal | impact, bootstrap and toy spread all exactly 0 |

### Uncertainties on the estimates
- **MC statistics:** a paired bootstrap over shards, 500 replicas for the impact and 200 for the
  rate shifts. Nominal and variant are resampled with the same shard weights, so the correlation
  between them is handled correctly. There are 73 RG and 101 DJ shards.
- **Data statistics:** 300 paired Poisson toys of `data_sub′`, each unfolded through both
  responses.

## 1. Impact of the reclassification (variant / nominal − 1)

| | h+ RG31 (nominal response) | h− RG31 | h+ DJ14 | h− DJ14 |
|---|---:|---:|---:|---:|
| **integrated** | **−0.559% ± 0.005% (MC) ± 0.001% (data)** | **+0.095% ± 0.005% ± 0.001%** | −0.643% ± 0.004% ± 0.001% | +0.063% ± 0.005% ± 0.001% |
| per-bin median | −0.29% | −0.22% | −0.26% | −0.21% |
| per-bin p05 / p95 | −0.95 / +0.08% | −0.79 / +0.51% | −0.97 / +0.35% | −0.67 / +0.92% |
| per-bin MC-stat (median) | 0.053% | 0.057% | 0.050% | 0.056% |
| bins with \|impact\| > 1% / > 2% (of 245) | 12 / 3 | 18 / 11 | 15 / 6 | 17 / 8 |
| median \|impact\| / ibu_stat | 0.097 | 0.078 | 0.094 | 0.075 |
| median \|impact\| / total quoted unc. | 0.043 | 0.035 | 0.040 | 0.035 |
| bins with \|impact\| > ½ total unc. | 0 | 0 | 0 | 0 |
| data-weighted `f_i` | 11.28 → 12.90% | 10.91 → 12.17% | 11.43 → 13.11% | 11.05 → 12.33% |

- **Removal vs impact.** About 1.5% of the body is removed and `f_i` grows by 1.3–1.6 pp, yet the
  integrated result moves by only −0.56% / +0.10%. Removal through `f_i` and the corresponding drop
  in efficiency largely cancel, as expected when data ≈ MC.
- **Significance.** The shift is real: it exceeds 3σ (MC) in ~150–166 of 245 bins. But it is small
  compared with the uncertainties: about 4% of the total and 8–10% of the stat at the median, and
  below half the total in every bin.
- **Largest per-bin effects.** These are the known sparse corner bins (pos b179 −11.8% ± 5.5%,
  pos b109 −6.3% ± 3.4%, neg b39 +7.3% ± 2.7%, all z0/pT4). They have total uncertainties of
  31–111%, and they are the same noise-dominated corners flagged in the OPT-A promotion record.
- **Flat in z and pT.** Medians are −0.24…−0.38% (h+) and −0.18…−0.31% (h−), except h− z0 at
  +0.15%.
- **Charge asymmetry.** h+ goes down and h− goes up, so h+/h− moves by about −0.65% (RG) and −0.71%
  (DJ). The asymmetric part comes from the absorbed class (h+ −0.40%, h− +0.27%). That is
  consistent with the charge-flipped absorbed legs leaving the *gen-charge* body (nominal "F7"
  dispatch) for the *reco-charge* fake column. Not isolated yet: a check is to rerun "absorbed"
  without the charge-flip criterion.

**Per class (RG, integrated, each class alone):**

| class | h+ | h− |
|---|---:|---:|
| absorbed | −0.404% ± 0.004% | +0.271% ± 0.003% |
| duplicate | −0.256% ± 0.004% | −0.193% ± 0.003% |
| Tier-2 | +0.002% ± 0.000% | +0.001% ± 0.000% |
| keeper | −0.033% ± 0.000% | −0.028% ± 0.000% |

The classes are not exactly additive (duplicates overlap the absorbed population).

**Window systematic (RG, integrated, all classes):**

| window | h+ | h− |
|---|---:|---:|
| loose (0.5–2, 20°) | −0.561% | +0.094% |
| nominal (0.7–1.5, 10°) | −0.559% | +0.095% |
| tight (0.8–1.25, 5°) | −0.557% | +0.097% |

The window spread is 0.004%, below the MC stat of 0.005%. The window choice does not matter at
the integrated level, and per-bin only in sparse corners (tight-window max 17.6%).

## 2. Rate-shift systematic (the uncertainty the reclassification makes measurable)

Each removed class's fake-column content is shifted by κ (`F_i → F_i + κ·class_i`) and
re-unfolded. The result is quoted relative to the variant central value, per unit κ, i.e. ±100%.
It is linear in κ: the ratio of the κ = 0.5 to the κ = 1 shift is 0.500–0.510.

| group (RG) | class share of reco h+ / h− | κ = +1 integrated h+ | κ = −1 h+ | κ = +1 integrated h− | κ = −1 h− | per-bin \|.\| median h+ / h− | per-bin p95 h+ / h− |
|---|---|---:|---:|---:|---:|---|---|
| det.int. (S2', existing) | 0.73 / 0.65% | −0.824% ± 0.006% | +0.844% | −0.735% ± 0.006% | +0.750% | 0.37 / 0.29% | 1.4 / 1.3% |
| absorbed | 1.18 / 0.70% | −1.344% ± 0.012% | +1.402% | −0.799% ± 0.008% | +0.819% | 0.49 / 0.46% | 3.7 / 6.8% |
| duplicate | 0.26 / 0.31% | −0.280% ± 0.004% | +0.283% | −0.342% ± 0.006% | +0.346% | 0.08 / 0.09% | 0.19 / 0.21% |
| Tier-2 | 0.16 / 0.16% | −0.156% ± 0.006% | +0.157% | −0.160% ± 0.006% | +0.160% | 0.21 / 0.20% | 0.61 / 0.60% |
| keeper | 0.07 / 0.06% | −0.076% ± 0.001% | +0.076% | −0.067% ± 0.001% | +0.067% | 0.02 / 0.01% | 0.12 / 0.12% |
| **new classes combined** | 1.66 / 1.23% | −1.837% ± 0.016% | +1.937% | −1.354% ± 0.014% | +1.406% | 0.80 / 0.76% | 4.6 / 7.9% |
| **all non-primary (det.int. + new)** | 2.39 / 1.89% | **−2.621% ± 0.019%** | **+2.827%** | **−2.062% ± 0.018%** | **+2.186%** | **1.20 / 1.07%** | 5.1 / 8.7% |

DJ14 cross-check, all non-primary: h+ −2.801% / +3.041%, h− −2.192% / +2.334%. det.int. alone:
h+ −0.935%, h− −0.838%. These agree with RG to about 7%.

Per-bin MC-stat on the shift: median 0.05% (det.int.), 0.08% (new), 0.10% (all).

**Effect on the quoted total if added in quadrature (RG, median over bins; current
`total_with_env100_c1fix_rel` 7.308% / 7.619%):**

| term | κ = 1.0 | κ = 0.5 | κ = 0.3 |
|---|---|---|---|
| det.int. only | +0.007 / +0.003 pp | +0.002 / +0.001 | +0.001 / 0.000 |
| new classes | +0.062 / +0.076 pp | +0.016 / +0.015 | +0.006 / +0.005 |
| all non-primary | **+0.171 / +0.129 pp** | +0.027 / +0.022 | +0.010 / +0.008 |

Compare the existing det.int. term, `detint_removal_rel`, with a median of ~0.03%: the rate shift
for det.int. alone at κ = 1 is 0.37% / 0.29% per bin and −0.82% / −0.74% integrated.

## 3. Reading

- **Reclassifying is a small, well-measured central-value shift.** It is −0.56% (h+) and +0.10%
  (h−) integrated, with MC-stat ±0.005% and data-stat ±0.001%. Bins move by about −0.25% at the
  median, and the window choice doesn't matter. It amounts to a few % of the quoted uncertainty,
  and the h+/h− ratio moves by about −0.65%.
- **The uncertainty is the bigger issue.** A ±100% normalization on all non-primary classes, the
  same convention as env100 for e/V0, gives about ±2.6% (h+) / ±2.1% (h−) coherent integrated and
  ≈1.1–1.2% per bin at the median. It is up to 5–9% at p95, concentrated at low pT. In quadrature
  it adds 0.13–0.17 pp to the median total, but as a correlated normalization it is about a third
  of env100's coherent ±7.7%.
- **The size of κ is the open physics decision.** These are GEANT/H1SIM and hadronization rates,
  not fiat templates, so ±100% is conservative. A per-class justification could support smaller
  values: H1's 1302.1321 used a 1%/track tracking systematic for secondaries, and strangeness
  feed-down is known to ~10–30%. The result is linear in κ, so any choice scales the numbers above.

## 4. Status, and what would make this real

- Everything here is diagnostic. Nominal, budgets and STATE.md are untouched.
- **As a shifted variant for the budget:** add `nonprimary_norm_rel` = κ × the per-bin rate shift
  (`rate_shift_per_bin.csv`, group `nonprimary_all`, or det.int./new split). Make it fully
  correlated across bins and charges. Optionally add a small method term (the nominal-vs-variant
  difference).
- **As a new nominal:** this needs the official chain (dated builder copy, merge, SHA-pinned
  driver, gates, critic review), budget regeneration and a user promotion decision. It is best
  folded into the pending OPT-A budget regeneration.
- **Open checks:**
  1. isolate the charge-flip contribution to the h+/h− asymmetry;
  2. Born-level propagation (C_rad is multiplicative per bin, so relative shifts should carry
     over, but verify);
  3. covariance (full-correlation assumption) for the rate term.

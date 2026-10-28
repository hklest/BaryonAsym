# Triple-check of the track-class background definition, and the corrected (v2) impact and uncertainty

Date: 2026-10-21. **DIAGNOSTIC**: nothing installed; nominal, budgets and STATE.md are unchanged.

This supersedes the v1 numbers in:
- `trackclass_impact_2026-10-20.md`;
- `nonprimary_norm_term_2026-10-20.md`;
- the background part of `track_truth_classification_study_2026-10-20.md`.

**Trigger.** The user challenged whether real tracks were being put into the background class.
They were.

## 1. How the classes were checked (variables never used by the classification)

Scripts in `scripts/trackclass_variant/`:
- `triplecheck_removed_classes.py`, Condor 5803795;
- `triplecheck_curvature.py`, Condor 5803797.

Each covers 10 files per generator from two periods (about 1.3–1.6 M body-eligible tracks).

Three independent tests were used:
1. **|DCA′| mixture fit.** The shape is fitted as a·(clean-core template) + (1−a)·(interaction-born
   det.int. template). The templates are matched in reco pT **and** NHitCJC, because DCA resolution
   degrades for short tracks.
   - Closure: core 1.000 ± 0.001, interaction det.int. 0.000 ± 0.003.
2. **Curvature pull |Δ(q/pT)|/σ.** σ is measured robustly from the direction-matched
   (angle < 2°) Tier-0 GTR population in NHitCJC × pT bins (table in `sigma_kappa_*.json` on NAF).
   - Reference: 99.5% of tracks have pull < 5.
3. **Physical constraints.** A secondary from an interaction cannot carry more momentum than its
   parent, and multiple-scattering angles scale as 1/p. The core's 99.9% angle quantile is 6–9°
   at all momenta.

## 2. Verdict per v1 class (RAPGAP / DJANGOH files)

| v1 class | % of body | matched-DCA primary frac. | curvature-consistent | charge agree | verdict |
|---|---:|---:|---:|---:|---|
| core (kept) | 98.3 | 1.000 | 0.994 | 1.000 | signal |
| P/GenP < 0.7 | 0.80–0.82 | 0.03–0.04 | 0.04 | 0.64 | **background**: less momentum than the parent, wrong charge 36%, 95% are the only track of their gen particle |
| K± keeper | 0.07 | 0.00 | 0.88 | 0.98–0.99 | **background** |
| charge flip | 0.01 | 0.005 | 0.0–0.02 | 0 | background (negligible) |
| P/GenP > 1.5 | 0.05 (2% at z > 0.7) | 0.07 | 0.23 | 0.66–0.72 | **NOT background**: more momentum than the parent is impossible for a secondary, so these are badly measured real tracks. DCA can't distinguish a bad fit from displacement. |
| angle ≥ 10° (\|p\| consistent) | 0.23–0.24 | 0.07–0.10 | 0.51 | 0.87–0.88 | **ambiguous**: includes elastically scattered primaries (real particles) |
| Tier-2 | 0.20–0.24 | 0.34–0.37 | 0.98 | 1 (forced by matching) | **treated as signal**: curvature- and direction-consistent with the gen particle, but no truth link |
| duplicates | 0.32–0.33 | 0.44 | 0.95 | 1.00 | real particle, extra track: a double-count correction, not "background" |

A pull-based "absorbed" definition (pull ≥ 5 or angle ≥ 10°) was tested and **rejected**. It would
remove another 0.63% of tracks that have 100% charge agreement (likely decay-in-flight kinks of
real primaries).

## 3. v2 definition (conservative: only clear background removed)

- Tier-0 GTR-linked with P/GenP < 0.7 (fc6), the K± keeper (fc8) and det.int. (S2' fc3/4/5).
- Split-track duplicates (fc9) are a separate double-count column.
- Tier-2, P/GenP > 1.5, large-angle and charge-flip tracks **stay signal**.

**Production.** Builder env: `TC_CLASSES=absorbed,keeper,duplicate`, `TC_RLO=0.7`, `TC_RHI=1e9`,
`TC_AMAX=180`, `TC_USE_QFLIP=0`.
- Condor 5803813: 320 jobs, all exit 0. Configs `v2` (RG + DJ), plus `v2_rlo05` and `v2_rlo085`
  (RG) for the boundary.
- An earlier attempt, 5803801, failed with exit 126 because the new wrapper wasn't executable.
  Nothing was produced from it.
- Null test of the modified builder (classes off): 68/68 histograms bit-identical to OPT-A.

## 4. Corrected impact of the reclassification (variant / nominal − 1)

From Condor 5803819. Outputs: NAF `.../trackclass_variant_2026-10-20/impact/`, local copy
`data/trackclass_impact_v2/`.

| | h+ RG31 | h− RG31 | h+ DJ14 | h− DJ14 |
|---|---:|---:|---:|---:|
| **integrated, v2** | **−0.526% ± 0.005% (MC) ± 0.001% (data)** | **+0.057% ± 0.005% ± 0.001%** | −0.612% ± 0.004% | +0.023% ± 0.004% |
| boundary 0.5 / 0.85 (RG) | −0.488% / −0.539% | +0.014% / +0.074% | | |
| v1 (withdrawn), for comparison | −0.559% | +0.095% | −0.643% | +0.063% |
| per-bin median (p05 / p95) | −0.27% (−0.87 / −0.02) | −0.21% (−0.58 / +0.35) | | |
| median \|impact\| / quoted total | 0.041 | 0.031 | 0.040 | 0.033 |

- The central-value impact **barely depends on the definition**: v1 → v2 moves it by 0.03–0.04%,
  and the boundary variations span about ±0.03%. The real tracks wrongly removed in v1 hardly
  moved the central value. In a self-consistent unfolding that is expected: removal and efficiency
  change cancel.
- The large per-bin values (max 11.7% / 7.3%) are still only the known sparse z0/pT4 corner bins,
  with total uncertainties of 31–111%.

## 5. Corrected uncertainty term: `nonprimary_norm_rel` v2 (κ = 1, ±100%)

From Condor 5803820. Package: NAF `.../trackclass_variant_2026-10-20/nonprimary_norm_term_v2/`,
local copy `data/nonprimary_norm_term_v2/`.
- The OPT-A c1fix budget is carried verbatim (490/490), plus the new columns.
- `total_quoted_with_nonprimary_rel` = √(total_quoted² + nonprimary² + duplicate²); closure is
  exact.

| | h+ | h− | v1 (withdrawn) h+ / h− |
|---|---:|---:|---|
| **coherent integrated** | **±2.07%** | **±1.39%** | ±2.83 / ±2.19% |
| … det.int. / v2 absorbed + keeper | 0.85% / 1.19% | 0.75% / 0.63% | |
| duplicates (separate column), coherent | ±0.33% | ±0.40% | |
| per-bin median (p95, max) | **0.44%** (3.1%, 5.0%) | **0.38%** (2.0%, 3.5%) | 1.22 (5.7, 24.9) / 1.09 (10.4, 47.4)% |
| duplicates per-bin median | 0.15% | 0.17% | |
| MC-stat of the term (median) | 0.065% | 0.060% | |
| DJ14 coherent (cross-check) | ±2.30% | ±1.57% | |
| total_quoted: median per-bin increment (p95, max) | +0.014 pp (0.51, 1.38) | +0.012 pp (0.19, 0.47) | |
| total_quoted: median of totals, before → after | 7.312 → 7.400% | 7.624 → 7.662% | 7.49 / 7.75% |

**Where the term is largest.**
- By z: highest at z0 (0.7–0.8%) and lowest at mid z (0.3–0.35%), with z6 at about 0.5%.
- By pT: it rises mildly (0.4 → 0.57%).
- The largest bins are S2_x1 at low z (pos b175 5.0% ± 0.07%, b183 4.8%, b177 4.7%). The MC stat
  there is small, so these are robust.
- The v1 z6 blow-ups (up to 47%) are gone. They came from the real-track classes: P/GenP > 1.5
  and Tier-2.

## 6. Remaining caveats

- The ~4% primary-like content of the v2 absorbed class and the ambiguous large-angle class
  (kept as signal) bound the residual definitional uncertainty. The central-value spread is about
  ±0.03–0.04%.
- κ = ±100% is a convention choice (env100-like). The term is linear in κ; use
  `nonprimary_norm_rel_per_unit_kappa`.
- The rate shift is evaluated in the variant scheme and applied relatively to the nominal. Born
  level is assumed level-invariant. Covariance: fully correlated (`nonprimary_norm_covariance.npz`,
  including a separate duplicate vector).

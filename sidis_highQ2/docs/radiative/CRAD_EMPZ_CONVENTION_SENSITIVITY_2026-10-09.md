# C_rad EMPZ convention sensitivity — releasebin basis (2026-10-09)

**Status: completed releasebin sensitivity study.  This document provides decision evidence only; it does not change the applied table or decide the convention.**

## 1. What the convention question was

The carried blocker asked whether a truth-level `GenEmpz` condition belongs in a gen-only radiative correction, particularly because earlier documentation described an open response-truth denominator and contained inconsistent historical EMPZ statements.  The concrete alternatives are:

- **applied/reference:** `GenEmpz > 45 GeV`, no upper bound;
- **threshold alternatives:** `GenEmpz > 50`, `>55`, and diagnostic legacy windows `45<GenEmpz<70`, `35<GenEmpz<70`;
- **open-truth alternative:** no `GenEmpz` cut.

All alternatives retain the releasebin machinery: `C_rad=sigma_nonrad/sigma_rad`, release-safe `(Q2esGen,x_es_gen)` bin assignment, `xFGen<0` recomputed with nominal-q on both legs, the same DIS/RAD-domain gates, weights, per-leg exposures, and beam mixture.

The source of the prior ambiguity is documented in `CRAD_CLOSURE_PLAN_2026-09-30.md`: it described a loose/open response-truth interpretation and historical reco/comment inconsistencies, and therefore proposed no-cut as a provisional target unless the exact executable response contract proved otherwise.  It also explicitly classifies no-cut versus `>45` as a fiducial-definition closure test, not a generator systematic.

## 2. Current executable response evidence

The response/migration builder used by the release-family v4 contract explicitly applies **lower-only `Empz>45` at reco and `GenEmpz>45` at truth**, with no upper cut.  In `H1ChargedSidis_HighQ2/scripts/unfolding/.../build_migration_th2_per_charge_v4.py`, the documented contract and executable masks set `Empz_lo=45.0` for both legs and require finite `GenEmpz>45`.  The releasebin worker implements the same lower-only reference mask (`empz_gt45`).

Therefore the evidence now strongly favors `GenEmpz>45` as the release-matched definition.  The user retains the convention decision: choosing no-cut or another threshold would redefine the truth fiducial and require a replacement C_rad table (and a superseding Born release), not merely a systematic relabeling.

## 3. Sensitivity production

Study root: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/crad_empz_convention_2026-10-09/`.

| generator | accepted releasebin input trees | Condor cluster | requested modes |
|---|---:|---:|---|
| DJANGOH14 | 2,953 | 5435552 | `>45`, `>50`, `>55`, `45--70`, `35--70`, none |
| RAPGAP31 | 1,777 | 5435553 | `>45`, `>50`, `>55`, `45--70`, `35--70`, none |

Jobs use the releasebin worker copied into the study root and request 4 GB after the earlier 2-GB held-campaign audit.  The complete 4,730-input retry finished successfully (DJANGOH14 cluster `5435675`: 2,953/2,953 exit 0; RAPGAP31 cluster `5435676`: 1,777/1,777 exit 0).  The first two submissions (`5435552/5435553` and `5435584/5435585`) were terminal pre-worker wrapper failures (zero partials), not held jobs: respectively nounset `GROUPROOT` and unavailable `module` during `thish1.sh` setup.  The successful wrapper correctly omitted unnecessary H1OO setup.

Numbers below are real 490-bin releasebin reductions.  Each entry is `median / maximum` of `|Delta C_rad|/C_rad` in percent; `z<0.05` is the same statistic restricted to its 35 bins per charge; `int` is the signed integrated change relative to `>45`.

### DJANGOH14

| alternative | h+ all | h+ z<0.05 | h+ int | h− all | h− z<0.05 | h− int |
|---|---:|---:|---:|---:|---:|---:|
| `>50` | 1.663/26.734 | 4.111/8.589 | +2.711% | 1.556/21.203 | 4.080/8.816 | +2.704% |
| `>55` | 18.099/70.919 | 23.614/33.923 | +19.411% | 17.226/57.564 | 23.634/34.518 | +19.353% |
| `45--70` | 0.000/0.0004 | 0.000/0.000 | -0.000% | 0.000/0.0002 | 0.000/0.000 | -0.000% |
| `35--70` | 0.251/32.865 | 4.144/26.834 | -1.965% | 0.250/27.987 | 4.103/24.929 | -1.955% |
| none | 0.263/72.667 | 7.724/72.667 | -3.047% | 0.264/68.391 | 7.620/68.391 | -3.020% |

### RAPGAP31

| alternative | h+ all | h+ z<0.05 | h+ int | h− all | h− z<0.05 | h− int |
|---|---:|---:|---:|---:|---:|---:|
| `>50` | 1.616/27.447 | 4.294/9.791 | +2.793% | 1.657/23.906 | 4.302/9.650 | +2.785% |
| `>55` | 17.841/64.980 | 24.253/36.498 | +19.821% | 17.766/59.605 | 24.248/36.132 | +19.767% |
| `45--70` | 0.000/0.000 | 0.000/0.000 | +0.000% | 0.000/0.000 | 0.000/0.000 | +0.000% |
| `35--70` | 0.239/35.078 | 4.518/32.199 | -2.029% | 0.239/33.170 | 4.610/30.179 | -2.018% |
| none | 0.243/76.771 | 8.287/76.771 | -3.140% | 0.252/74.134 | 8.380/74.134 | -3.111% |

Machine-readable summary: `empz_sensitivity_results.json` and one 490-row CSV per generator/alternative in the study root.

The existing aggregate scan is now corroborated qualitatively: tightening the lower threshold raises C_rad, while opening the truth space lowers the integrated correction by about 3%.  The 45--70 identity shows there is effectively no accepted `GenEmpz>=70` population under the fixed releasebin gates.

## 4. Recommendation, conditional on the completed check

On the current executable response evidence, retain `GenEmpz>45` for the release definition unless the user intentionally changes the release truth fiducial.  A future flip has the following cost: regenerate matched RAD/NONRAD releasebin partials/tables, replace the applied table, regenerate the Born-level release and its C_rad-dependent budget components/plots, update provenance, and explicitly supersede the present Born release.  It must not be treated as a generator spread or silently absorbed into `c_rad_gen_rel`.

## 5. Remaining blocker

The EMPZ sensitivity calculation is complete.  The user convention decision and `P-CRAD-LUMI` remain independently open; manifest inheritance improves local traceability but does not upgrade exposure provenance.

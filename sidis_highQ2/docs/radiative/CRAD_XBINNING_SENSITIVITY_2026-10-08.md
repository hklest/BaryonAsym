# C_rad x-cell binning sensitivity — 2026-10-08

**Status: PASS — material per-bin correction-definition effect; new budget component needed before this Born correction can be publication-grade.**

## Question

The applied xFcut DJANGOH14 correction table uses `C_rad = sigma_nonrad/sigma_rad` in 490 y-projected bins (245 per charge). The frozen campaign worker gates the DIS selection with the ISR-safe `x_es_gen`, but assigns its `(Q2,x)` bin using framework eSigma `Q2esGen, XesGen`. The v6 response/release assigns its Q2 stratum with `Q2esGen` but preferentially assigns x with `x_es_gen`. This study measures the effect of making the C_rad x assignment release-consistent.

## Verified implementation and provenance

* Frozen worker: `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/crad_v3_worker_xfcut.py`.
  * Lines 120–122 set `q2=Q2_es_gen`, `y=Y_es_gen`, `x=x_es_gen` and use `150 <= q2 < 20000`, `0.1 < y < 0.7`.
  * Lines 121 and 140 book `bindex(Q2esGen, XesGen, z, pT)`; line 147 labels this `framework_esigma`.
  * Lines 124–140 apply the charged-hadron (`|PDG|=211,321,2212`, nonzero charge), `IsGtr==1`, `Mother2==-1`, V0/ancestor veto, `GenEmpz>45`, recomputed nominal-q safe-xF `<0`, and finite/bin-supported nominal `z,pT` requirements. RAD has `Q2Gki>100`.
* Release response snapshot: `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py:510–519`: `Q2_bin=Q2esGen`; `x_bin=x_es_gen` when present.
* The v6 builder obtains the safe x with `E_e_eff=GenBeamElecE-GenEnPhoton` and `x_es_gen=Q2_es_gen/(4 E_p E_e_eff Y_es_gen)` (`src/ChargedSidisTreeBuilderHighQ2_v6.C:1321–1339`).
* Inputs from `crad_djangoh14_xfcut.json`: RAD `731 ep + 642 em`; NONRAD-old `260+260`; NONRAD-new `530+530`: **2,953 trees** total.

## Execution and recovery

The first submission failed before payload execution because Condor treated the queue column named `output` as scheduler stdout, not as the worker argument, and tried to open non-existent `.npz` paths as job logs. This was repaired only in the study root: `target` is now the payload queue column, study output directories are pre-created, and the study worker also calls `os.makedirs(dirname(output), exist_ok=True)`. A one-file smoke (cluster `5429191`) wrote and read a valid NPZ. No central release, applied table, canonical snapshot, or radiative-correction campaign input was modified.

Production then completed with terminal success (all jobs `JobStatus=4`, exit code 0): `5429192` (400), `5429193` (400), `5429194` (400), `5429196` (400), `5429198` (400), `5429199` (400), `5429201` (400), and `5429202` (152), plus the smoke file = **2,953/2,953 readable partials**. There were no held jobs in the successful production chunks.

The additive study worker retains the frozen worker's weights, all selection, nominal z/pT and safe-xF construction, RAD domain, bin edges, 245-bin charge ordering, production luminosities, and beam mixture. It books the two assignments in parallel:

* framework: `(Q2esGen, XesGen)`;
* release-safe: `(Q2esGen, x_es_gen)`.

The reducer is `output_v6/studies/crad_xbinning_sensitivity_2026-10-08/scripts/reduce.py`. Its framework rebooking reproduces the applied table to a maximum relative residual `8.72e-5` (the small difference is numerical/reduction-level, not the tested assignment change).

## Processed rows

| leg | input rows | pass DIS/domain | selected hadron rows |
|---|---:|---:|---:|
| RAD | 4,199,702,557 | 1,244,742,282 | 413,087,810 |
| NONRAD-old | 1,777,852,281 | 840,397,798 | 403,429,583 |
| NONRAD-new | 235,985,250 | 111,544,156 | 53,540,050 |
| NONRAD total | 2,013,837,531 | 951,941,954 | 456,969,633 |

`selected hadron rows` is after the full worker-equivalent selection, including finite nominal z/pT but before requiring a specific `(Q2,x,z,pT)` bin. It is the denominator used for the assignment-transition rates below.

## Assignment flips

### Overall and low-z concentration

| leg | all selected: changed / total | flip rate | `z<0.05`: changed / total | flip rate |
|---|---:|---:|---:|---:|
| RAD | 7,221,832 / 413,087,810 | **1.7483%** | 5,290,055 / 299,119,544 | **1.7685%** |
| NONRAD-old | 12,491 / 403,429,583 | 0.00310% | 9,169 / 290,032,911 | 0.00316% |
| NONRAD-new | 1,447 / 53,540,050 | 0.00270% | 1,088 / 38,492,986 | 0.00283% |
| NONRAD total | 13,938 / 456,969,633 | **0.00305%** | 10,257 / 328,525,897 | **0.00312%** |

Thus the x reassignment is strongly radiative: RAD flips are about 570 times more frequent than the combined NONRAD flips. The `z<0.05` row contains 73.25% of RAD flips (`5.290/7.222` million) and 73.59% of all selected RAD hadrons, so it has a modestly higher flip rate rather than being the sole source.

### Per framework `(Q2,x)` cell — beam-combined RAD

| cell | Q2 range / x range | RAD events | changed assignment | rate |
|---:|---|---:|---:|---:|
| 0 | 150–300 / 0.0021–0.0071 | 142,370,692 | 3,166,089 | **2.2238%** |
| 1 | 150–300 / 0.0071–0.030 | 70,391,541 | 93 | 0.000132% |
| 2 | 300–700 / 0.0042–0.016 | 89,886,761 | 2,821,078 | **3.1385%** |
| 3 | 300–700 / 0.016–0.069 | 43,598,218 | 0 | 0 |
| 4 | 700–1500 / 0.0099–0.034 | 26,700,590 | 1,070,526 | **4.0094%** |
| 5 | 700–1500 / 0.034–0.150 | 18,170,636 | 8 | 0.000044% |
| 6 | 1500–20000 / 0.021–1.5 | 21,795,377 | 82 | 0.000376% |

The same RAD concentration persists for `z<0.05`: cells 0/2/4 have rates 2.2180%/3.1133%/3.9604%, while the complementary high-x cells 1/3/5/6 are effectively unchanged. Combined NONRAD cell rates are tiny: cells 0/2/4 are approximately 0.005%, 0.005%, 0.0044%, respectively; the other cells are at or below 0.0001%.

### Edge migration

RAD has 164,038 assignment transitions entering or leaving the seven-cell `(Q2,x)` grid: **0.03971%** of all selected RAD hadrons, increasing to 125,726 (**0.04203%**) in the `z<0.05` row. The combined NONRAD edge count is only 11 events (**2.41e-6%**) overall and 7 (**2.13e-6%**) at low z. Hence grid-edge migration is negligible compared with the internal RAD x-cell reshuffling.

## C_rad impact

The table below is `|C_rad(x_es_gen)-C_rad(XesGen)|/C_rad(XesGen)`, using the same y-projected 490-bin rate-ratio calculation as production.

| scope | median | mean | 95th percentile | maximum |
|---|---:|---:|---:|---:|
| all 490 bins | **3.5049%** | 3.3375% | 6.3441% | **11.3712%** |
| h+ (245) | **3.5031%** | 3.3052% | 6.1248% | **11.3712%** |
| h− (245) | **3.5138%** | 3.3699% | 6.5848% | **11.0135%** |
| `z<0.05`, all charges (70) | **3.5831%** | 4.0104% | 8.8503% | **11.3712%** |
| `z<0.05`, h+ (35) | 3.6483% | 4.0276% | 8.3152% | 11.3712% |
| `z<0.05`, h− (35) | 3.5179% | 3.9931% | 8.2609% | 11.0135% |

Despite these differential changes, the integrated correction is unchanged, because the reassignment redistributes rather than changes the weighted rate:

| charge | `C_rad(XesGen)` | `C_rad(x_es_gen)` | integrated `|delta|/C` |
|---|---:|---:|---:|
| h+ | 1.141853575 | 1.141853575 | 0 |
| h− | 1.141375704 | 1.141375704 | 0 |
| combined | 1.141616560 | 1.141616560 | `1.95e-14%` |

The zero integrated shift must **not** be used to dismiss the issue: it is the expected cancellation from a pure `(Q2,x)` reassignment under a full-grid sum, while published differential bins move by 3–11%.

## Verdict

**New budget component needed; not second-order and not absorbable in the currently applied medians.** The 3.503%/3.514% per-charge median is about 6.6/7.0 times larger than the existing `c_rad_gen_rel` medians (0.5298% h+, 0.5009% h−), and about 8.2/7.8 times the existing `c_rad_stat_rel` medians (0.4272% h+, 0.4480% h−). The maxima reach 11.37%/11.01%. The effect is a deterministic definition mismatch, not statistical noise or a DJANGOH–RAPGAP generator comparison, and should be resolved by rederiving/replacing the applied correction table with release-safe x binning (or, pending replacement, carried as a dedicated correlated C_rad x-binning-definition systematic). It cannot be absorbed by the existing generator-spread or MC-statistical median components.

## 2026-10-09 addendum — formal independent-safe rederivation cross-check

The formal release-safe campaign worker was diffed against the frozen xFcut worker. Its only semantic change is `bindex(Q2esGen, XesGen, z, pT) -> bindex(Q2esGen, x_es_gen, z, pT)`; metadata and prose labels are the only remaining differences. The RAPGAP31 release-safe worker has the same binning-only diff.

The subsequent apparent F1/formal disagreement is a **study-support convention**, not a formal-worker deviation. The F1 worker requires *both* framework and safe indices in-grid (`good = m & (io>=0) & (isf>=0)`) before booking either result, so its safe histogram is a common-support object. The formal rederivation correctly books all rows in-grid under the release-safe index, including safe-in/framework-out grid-edge migrations. The F1 study records 164,038 such RAD enter/leave-grid transitions (0.03971% of selected RAD rows) versus 11 combined NONRAD transitions. The formal-vs-F1 common-support difference is at most 0.3225988% per bin and −0.02945%/−0.02951% integrated h+/h−.

This does **not** change the F1 headline result: framework-vs-safe comparison within F1 applies the same common-support restriction to both assignments, yielding its internally consistent ~3.5% median and 11.4% maximum differential sensitivity. The conclusion that the mismatch is not second-order and cannot be absorbed by the existing ~0.5% C_rad components stands. The exact root-cause record is `data/radiative/djangoh14_crad_releasebin_2026-10-09/F1_CROSSCHECK_ROOT_CAUSE_2026-10-09.md`.

## Artifacts

* Study summary: `H1ChargedSidis_HighQ2/output_v6/studies/crad_xbinning_sensitivity_2026-10-08/results/summary.json`
* 490-row comparison: `.../results/per_bin_crad_comparison.csv`
* Diagnostic: `.../results/per_bin_relative_crad_shift.pdf` and `.png`
* Study-only implementation and recovery record: `.../README.md`

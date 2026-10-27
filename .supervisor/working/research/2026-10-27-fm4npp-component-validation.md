# Research: validation of the FM4NPP real-data hit components, and the CJC event T0

Date: 2026-10-27. Validation only: no production code was changed and nothing was submitted for
production. This follows the blockers research
(`.supervisor/working/research/2026-10-27-fm4npp-h1-real-data-blockers.md`) and the plan
(`.supervisor/working/plans/2026-10-27-fm4npp-h1-real-data-hits.md`, D5, D11, D12, D13, D16).

**Scope.** Part A checks each component the plan relies on against h1rec output as the reference:
A1 CJC coordinates, A2 COZ, A3 CST, A4 constants for 2005 and 2006, and A5 the `hits` schema.
Part B finds the real CJC event-T0 code and tests whether it improves on the `cjavt0`
approximation.

Labels: **[V]** verified here, by a command, an output file or a file:line; **[I]** inference.

Abbreviations:
- `$R` = `/data/dust/group/h1/klesthen/rawhit_probe`
- `$V` = `$R/validation`
- `$h1rec` = `/data/dust/group/h1/sschmitt/h1soft/h1rec`
- Every figure exists as `.png` and `.pdf`.

All validation scripts, Condor `.sub` files, logs, outputs and figures are under `$V` (1.1 GB). No
Condor jobs from this stage are left running.

## Headline

- **One D16 CJC threshold fails: the track-free 95% quantile** is 7.3 mm against a required
  < 2 mm. The track-free median (470 µm) passes marginally. The plan's D16 regression (0.944 within
  10 µm on the 89 c070060 Q2e>150 events) is reproduced exactly (0.9442). On the wider Q2e>5 lam/
  sample the exact path gives 0.926, which is informational, since D16 sets no lam/ threshold.
- **The track-free tail grows steeply at small |cos β| and long arc length.** `CJC.simple()`
  drops the track-angle (β) terms, which is the likely cause [I].
- **The real event T0 is on the DST and in the HAT** (DMIS `TZERO_M`; HAT `T0_CJC`). Using it
  instead of `cjavt0` changes the track-free r-φ resolution by less than 0.2%. T0 is not the
  limiting factor.
- **Constants work for 2005 and 2006 e−** (IVRSQT = 2). The exact path gives 0.937 and 0.912
  within 10 µm. IVRSQT 2 and 3 take the same branch for hit positions.
- **The COZ isochrone code matches h1rec** on 1.36M hits (median |Δt| 2e-4 ns). In this probe
  chain h1rec leaves CZZR empty, so no h1rec COZ coordinates exist to copy into `coz_trk_x/y/z`
  (D12).
- **The CST chain reproduces the DST clusters at 100%,** but that is h1rec compared with itself.
  The r-φ median threshold (< 0.4 mm) fails on one of three files (483 µm).

## Validation table

| Component | Sample | Metric | Result | Plan threshold | Verdict | Figure |
|---|---|---|---|---|---|---|
| A1 CJC exact, D16 regression | `$R/calib_consts`, c070060 Q2e>150, 89 ev, 54,062 hits | frac \|Δxy\| < 10 µm | 0.9442 | 0.944 | pass | no figure (numbers from `/data/dust/group/h1/klesthen/rawhit_probe/validation/cjc_lam/validate_cjc_regress.out`) |
| A1 CJC exact (hit3d, ICOR=3) vs CJXR | lam/, 38 files, 46 runs with hits, 57,038 ev, 21.77M hits | frac < 10 µm; median / 95% \|Δxy\| | 0.926 (per run 0.915–0.944; 1/46 ≥ 0.944); 0.42 µm / 23.9 µm | none on lam/ | info | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig1_residual_hists.png` (b, e); `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig2_per_run.png` |
| " | " | median / 95% \|Δz\| | 26 µm / 3.79 mm | — | info | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig1_residual_hists.png` (c) |
| A1 track-free, nearest mirror, `cjavt0` T0 | " | median \|Δr-φ\| | 470 µm; 6/46 runs > 0.5 mm | < 0.5 mm | pass (marginal) | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig1_residual_hists.png` (a); `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig2_per_run.png` |
| " | " | 95% \|Δr-φ\| | 7.26 mm | < 2 mm | **FAIL** | same |
| A1 track-free, run-default T0 (460) | " | median / 95% | 491 µm / 7.56 mm; 16/46 runs > 0.5 mm | < 0.5 / < 2 mm | marginal / **FAIL** | same |
| A1 track-free z | " | median / 95% \|Δz\| | 0.6 mm / 27 mm | median < 1 cm | pass | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig1_residual_hists.png` (c) |
| A1 one event by eye | run 497718, ev 27341, 328 hits | x-y overlay | exact points on CJXR; steep segments' mirrors off by cm | — | info | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig3_event_map.png` |
| A1 tail vs β, arc length | c070060, 60 ev, 26,015 hits | track-free median by \|cos β\| bin | 0.3 mm at \|cos β\| > 0.9; 12 mm at 0.1–0.2 | — | info | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig4_tail_vs_beta_slen.png` |
| A2 COZ drift time, py vs CZR2 | lam/, 37 files, 1,362,037 hits | median / p99 / max \|Δt\| | 2.0e-4 / 5.6e-4 / 3.7e-2 ns | none (ref 7e-4 ns) | pass; 7 garbage-hit tails | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/coz/coz_fig1_dt.png`; `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/coz/coz_fig5_perfile.png` |
| A2 COZ z, r from py vs h1rec isochrone parameters | " | median \|Δz\| | 0.014 µm (max 329 µm) | none | pass (isochrone step only) | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/coz/coz_fig2_dzdr.png` |
| A2 COZ z vs CJKR track | lam/, 973,881 z-matched hits | robust σ / median | 14.4 mm / −0.34 mm | none | consistent with ref 14.5 mm | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/coz/coz_fig3_ztrack.png` |
| A2 COZ φ, charge division (unvalidated path) | " | median \|φ_cd − φ_track\| | 0.26 rad (null 0.95), p95 2.4 rad | none | not usable (D12 off holds) | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/coz/coz_fig4_phi.png` |
| A3 CST DCLH cluster match | c070060, c070030, c070100, 121 ev each | match fraction | 100%, 100%, 100% | ≥ 99% | pass (h1rec vs itself) | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cst/cst_cog_raw_vs_DST.png` |
| A3 CST \|Δz\| vs CSHY | " | frac < 0.5 mm (q90) | 0.91 (0.43), 0.92 (0.15), 0.93 (0.08 mm) | 90% < 0.5 mm | pass (partly circular) | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cst/cst_dz_vs_CSHY.png` |
| A3 CST r-φ to nearest point, CJKR tracks | " | median | 253, 483, 330 µm | < 0.4 mm | **FAIL** on c070030 | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cst/cst_rphi_nearest.png` |
| A3 CST signed r-φ per layer | " | median; core width | within ±6 µm; 133–231 µm | — | info | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cst/cst_rphi_signed_per_layer.png` |
| A4 constants 2006 e− | c060250, run 452004, 23 ev, 10,190 hits | exact frac < 10 µm | 0.912 | ≥ 0.90 | pass | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/consts_periods/dxy_exact_path.png` |
| A4 constants 2005 e− | c050075, run 420922, 21 ev, 7,528 hits | exact frac < 10 µm | 0.937 | ≥ 0.90 | pass | same; z: `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/consts_periods/dz_exact_path.png` |
| A4 2007 reference, same script | lam/c070060, run 497719, 300 ev, 142,831 hits | exact frac < 10 µm | 0.923 | — | matches A1 | same |
| A5 schema | `hit_level_sim/production/cov_loQ2/loQ2/hits/hits_part_0.root` | branches | 110: 4 `Int_t`, 56 `vector<float>`, 50 `vector<int>` | 110 (D1) | confirmed | no figure (list in `$V/schema/hits_part_0_branches.txt`) |
| B CJCREC T0 on raw vs DMIS | c070060, run 497719, 131 ev | median / robust σ | 0.000 / 0.004 ticks; 17/131 > 0.1 tick | — | reproduces DMIS | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/t0/fig3_cjcrec_raw_minus_DMIS.png` |
| B DMIS − `cjavt0` | c070060, 250 ev | median / robust σ | −0.24 / 1.05 ticks | — | `cjavt0` is a good proxy | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/t0/fig2_T0_real_minus_approx.png` |
| B track-free r-φ to CJKR circles by T0 | c070060, 77 ev, 52,549 hits | robust σ | see B table | — | real T0 brings no gain | `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/t0/fig1_rphi_resid_by_T0.png` |

## A1. CJC coordinates on lam/ [V]

**Files.**
- Scripts in `$V/cjc_lam/`: `a1_file.py`, `a1_job.sh`, `a1_job.sub`, `a1_agg.*`, `a1_hist.*`,
  `a1_diag.py`, `a1_plots.py`, `a1_diag_plot.py`.
- Condor clusters 5917784 (38 jobs), 5917912 and 5917934 have all finished.
- Per-run table: `$V/cjc_lam/a1_table.txt`; full numbers in `a1_perrun.json`. Per-hit CSVs:
  `$V/cjc_lam/out/*.hits.csv.gz` (829 MB).

**Inputs.** Raw hits come from `lam/<f>/cjc/cjcprobe.txt` (CRJE). The h1rec hit bank and the
`kdr` index come from `events.txt` `H` rows. Every `consts_<run>.json` was present, and there were
no count mismatches.

**Exact path.**
- The D16 regression reproduces the README exactly. `python3 -I py/validate_cjc.py .` in
  `$R/calib_consts` gives 54,062 hits and these fractions:

  | Within | 1 µm | 10 µm | 100 µm |
  |---|---|---|---|
  | Fraction of hits | 0.8611 | 0.9442 | 0.9832 |

  The output is saved as `$V/cjc_lam/validate_cjc_regress.out`. It is identical to the stored
  `validate_cjc.out` apart from five extra "worst hit" lines.
- On lam/ (Q2e>5), runs range from 0.915 (497718) to 0.944 (498739).
- On 60 events of c070060, the fraction within 10 µm falls as tracks get longer or steeper:

  | Hits | Fraction within 10 µm |
  |---|---|
  | All | 0.911 |
  | Arc length < 30 cm | 0.997 |
  | Arc length > 60 cm | 0.753 |
  | \|cos β\| < 0.8 | 0.786 |

  This is the TP5-versus-TP11 arc-length tail (`$R/calib_consts/README.md` §4). Figure:
  `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig4_tail_vs_beta_slen.png`.
- [I] Q2e>5 events carry more long, low-pT tracks than Q2e>150, so their fraction sits lower. A4
  gets 0.923 on run 497719 of lam/ with an independent script, which agrees.

**Track-free path.**
- On 60 events of c070060, 19% of hits are off by more than 2 mm and 33% by more than 1 mm.
- The track-free median is 0.3 mm at |cos β| > 0.9 and 12 mm at |cos β| 0.1–0.2. It is lowest
  (0.23 mm) at arc length 20–30 cm and rises to 7–9 mm at 90–110 cm (few hits beyond) (`fig4`).
- [I] The cause is that `CJC.simple()` sets β = 0, which drops the term `(1/|cosβ| − 1)·DRD`
  (README §2.2). At |cos β| = 0.5 that term doubles the drift distance.
- Nearest-mirror selection picks the same side as CJXR for 98% of hits.
- The T0 choice moves the median by about 20 µm and leaves the 95% quantile unchanged. The one
  exception is run 492991, where the run-default T0 gives 650 µm against 509 µm with `cjavt0`.
- CJXR is ICOR=3 (hits pulled onto the track circle), so these residuals include the
  track-dependent terms by construction.
- D5's 634 µm is a robust σ against CJKR circles, a different metric. B measures 604 µm with it.

**Per-run anomalies.**
- **Run 492559** (c070000): 1,316 of 1,501 events have CRJE = 0, and CJXREF returns ier 2. Only
  7,292 hits remain, yet their agreement is normal (exact 0.923, track-free median 443 µm). This is
  a statistics problem, not a collapse.
- **Run 500490** (c070108): 20 events, 18 with CRJE = 0 and none with CJXR hits. It is excluded.
- **Other CRJE = 0 events:** 161 in total, 96 of them in run 495213.
- **Events without tracks or T0:** 11,626 events have no CJXR row, and 13,588 have no usable
  CJKR T0, so they fall back to T0 = 460.
- **The `# CJCC … t0` line in `events.txt`** ranges from −537 to 2900. It is calibprobe's own
  CJCC (mean `T0track`), not CJCREC's, and must not be used.

**Figure captions.**
- `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig1_residual_hists.png`: exact versus track-free |Δxy| (log-log), a µm zoom of the
  exact path, |Δz|, the signed track-free residual, and the cumulative fraction against the
  0.944 line.
- `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig2_per_run.png`: per-run exact fraction within 10 µm and the track-free median and
  95% against run number, with threshold lines and outliers labelled.
- `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig3_event_map.png`: x-y map of run 497718, event 27341, with a one-cell zoom. Exact
  points sit on CJXR, while both track-free mirrors of steep, long-path segments miss by
  centimetres.
- `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cjc/fig4_tail_vs_beta_slen.png`: exact fraction within 10 µm, track-free fraction above
  2 mm, and track-free median, binned in |cos β| and in arc length (c070060, 60 events).

## A2. COZ on-track hits [V]

**Files.** Scripts are in `$V/coz_lam/` (`a2_file.py`, `a2_job.sub`, `a2_agg.py`, `a2_plots.py`,
`a2_tail.py`). Condor clusters 5917793 and 5917924 have finished. The summary is
`$V/coz_lam/a2_summary.txt`.

**What h1rec output exists.**
- In the calibprobe dumps, h1rec COZ output is CRZE, CZR2 and CZZR.
- CZR2 holds wire, IERR (7 in every row), TIME and the isochrone parameters.
- CZZR is all zeros in every row, so the dumps hold no h1rec COZ z, r or φ. COZG was not dumped.
- The z/r agreement is therefore COZNIC evaluated on Python versus h1rec isochrone parameters. It
  tests the isochrone lookup only.

**Tail hits.**
- The 7 hits with |Δt| > 2e-3 ns all lie beyond the isochrone table (t > 810 ns).
- Five have DL near 16-bit overflow, and two carry a bogus calibprobe CJCC T0 (−537 and −54).
- The residual is float32 rounding in the extrapolation branch. A cut at t ≤ 810 ns removes all
  seven. Per-hit dumps are in `$V/coz_lam/out/*.tail.json`.

**T0 sensitivity.** Using `cjavt0` instead of the CJCC line changes z by a median of 0.036 mm.
Using 460 instead changes it by 0.053 mm. The track-residual σ stays at 14.4 mm in all three cases.

**Track-free φ.** The charge-division φ carries some information, but the tails are too large to
use (table row).

**Figure captions** (each in `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/coz/`, `.png` and `.pdf`):
- `coz_fig1_dt`: |Δt| with the 7e-4 ns and 2e-3 ns lines.
- `coz_fig2_dzdr`: |Δz| and |Δr| from Python versus h1rec parameters.
- `coz_fig3_ztrack`: z_COZ − z_CJKR with the three T0 choices overlaid.
- `coz_fig4_phi`: exploratory φ_cd − φ_track, real versus a +1 rad null.
- `coz_fig5_perfile`: per-file σ and max |Δt| against run, with outliers labelled.

## A3. CST [V]

**Files.**
- The original outputs in `$R/cst_data/` were re-derived without changes.
- Two new Condor runs of the unchanged `cstprobe` binary are in `$V/cst/run_c070030/` (run
  494756) and `$V/cst/run_c070100/` (run 500077), clusters 5917777 and 5917778.
- Scripts: `$V/cst/validate.py`, `cmp_cl.py`, `plots.py`.

**How each metric is computed.**
- **DCLH match.** The denominator is DST clusters. A match needs the same half-ladder and side
  and a cog within 0.1 strip. The cog agrees to at most 0.02 strip.
- **CSHY z.** The denominator is the 256 of 300 tracks that found an r-φ-matched point. Over all
  300 tracks the fraction is 0.77, 0.70 and 0.71.
- **r-φ to the nearest point.** This is per (track, layer), with no z requirement.

**Independence.**
- The DCLH match is h1rec compared with itself. The same CSCLUS2 code and the same NDB constants
  ran on the same SIFE, so it shows the call is reproducible and nothing more.
- The CSHY z check is partly circular, because CSHY was fitted with these CST hits. It is also a
  best-of over three ambiguity copies and several hypotheses. Against raw CJKR θ and z0 instead,
  the median |Δz| is 4.9 mm. That last number has no figure; it comes from `$V/cst/validate.py`
  output in `validate_c070060.log`.
- The r-φ check is partly independent: CJKR uses the CJC only. Rotating tracks in φ drops the
  median to about 10 mm, so the match is far above chance.
- The per-layer signed residual has no bias (median within ±6 µm).
- [I] The per-file spread of the median (253–483 µm) follows point and ghost density, so a
  per-layer residual width is a better threshold statistic than the median.

**Plan facts.** SIFE is present in GMCUT for all three files. The CSMK read was not located.

**Figure captions** (each in `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/cst/`):
- `cst_dz_vs_CSHY`: three files with the 0.5 mm line and q90 marked.
- `cst_rphi_nearest`: three files plus the rotated control, with the 0.4 mm line.
- `cst_rphi_signed_per_layer`: the signed residual per layer.
- `cst_cog_raw_vs_DST`: the cog difference, raw versus DST.

## A4. Constants for 2005 and 2006 [V]

**Files.** Run directories `c060250/`, `c050075/` and `c060000/` are in `$V/consts_periods/`,
with `setup_run.sh`, `cmp.py`, `validate_xy.py`, `dump_resid.py` and `plot_resid.py`. Runs were on
the login node, about 40 s each.

**NDB chain.**
- The NDB chain works for 2006 e− run 452004, 2005 e− run 420922 and 2006 e+ run 468531. Each
  log shows "H1 Master Database" and CALRUN ier = 0. The CALCOZ ier = −1 rate matches 2007.
- 2005 dst7 resolved through the worker's own `search/acs/data/05/dst7` symlink, because
  `H1Tree.C:634` adds the steer's mODS directory to the search path. `calibprobe.C:71` hardcodes
  `$R/mirror`, which has no 05 entry.

**IVRSQT.**
- `$h1rec/ct_cjcrec/cjrunc.F:462-467` sets IVRSQT = 2, then 1 below run 281276 and 3 from run
  452777.
- On the hit-position path, IVRSQT appears only as `IVRSQT.LE.1` in `cjfh3d.F:221` and
  `cjdedx.F:220`. The other uses found are dE/dx calibration (`ct_cjcalib/cje*ac.F`,
  `ksprim/cjdxcc.F`) and display (`ct_cjclook/cjtfig.F:255`).
- So for hit positions, IVRSQT 2 and 3 take the same branch.

**Bank differences against 2007.** The bank list is the same apart from sizes. CSF8 has 24 words
(Superfit present; its effect was not checked). C3Z8 is missing for run 468531, which is harmless
in ellipse mode (C0Z8 word 7 = 1000).

**Exact path.** The median |Δxy| is 0.415 µm (2006) and 0.429 µm (2005). The 2006 e+ test file
c060000 had no CJC hits in its 4 sampled events, so it got no hit-level check.

**Figure captions** (each in `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/consts_periods/`):
- `dxy_exact_path`: |Δxy| for 2005, 2006 and the 2007 reference, with the 10 µm line.
- `dz_exact_path`: |Δz| with the 1 mm line.

## A5. Schema [V]

`/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/production/cov_loQ2/loQ2/hits/hits_part_0.root`
(md5 `57c70330a8af36f8c66939fb85fb3def`) has one TTree, `hits`, with 249 entries and 110 branches:
- 4 `Int_t`: `run`, `event`, `n_str_all`, `n_svx_all`.
- 56 `vector<float>`.
- 50 `vector<int>`, including the `coz_*`, `csts_*` and `cip_*/cop_*` branches.

The full list is `$V/schema/hits_part_0_branches.txt`.

## B. The CJC event T0

**Where h1rec computes it [V].**
- `$h1rec/ct_cjcrec/cjcrec.F` calls CJCTGV (fast CJK4 tracks) at `:357`, CJFRAW at `:503,507`
  and CJTIME at `:519`.
- In `ct_cjcrec/cjtime.F`, each CJK4 track goes through `CJTIMA` (`:198`; the fit is in
  `cjtima.F`). CJTIMA fits T0 and Δv_drift to the hits in a road around the track.
- The per-track T0 and its error are stored in CJK5. These become the DST CJKR columns 27/28.
- The event value is `CJAVT0` over the per-track T0s (`:406`), then a robust joint T0 and v_drift
  fit `CJAVR2` (`:422`). For IRFLAG ≠ 1 it falls back to the run average when within 5 ticks.
- CJCC is booked at `:482-500`: row 1 is measured, row 2 is used, column 2 holds T0.
- It is a track-based fit over raw hits. It is not a hit-pair or mirror-hit T0.
- Inputs: raw CRJE, run constants (CJICAL, CJGEOM, CJCMAT through the NDB), CJC4 and HEAD.

**Stored on the DST and in the HAT [V].**
- `h1banks.ddl:12186-12188` (4.2.1 `h1oo/H1Banks/`) defines DMIS `59 TZERO_M`, `60 TZERO_U` and
  `61 D_TZERO`.
- `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/H1Filler/H1HatFiller.C:855` fills
  `hatEvent->fT0_CJC = dmis[0]->GetTzero_m()`.
- Events without a measured T0 have `D_TZERO` = 0 and a default T0.
- The earlier artifact and D11 said the event T0 is "not on the DST". That is true of the CJCC
  bank only.

**Calling CJCREC from the probe [V].**
- A private build in `$V/t0/` (`build_lib.sh`, `lib/`, `t0probe.C`, `bin/t0probe`) runs CJCREC up
  to CJTIME on raw CRJE for c070060 and reproduces DMIS. It needs:
  - a no-op `WDROP`, because work banks abend with "INDEX ID INCORRECT";
  - an early exit, because CJCREC later segfaults in `cjecln.F:129`.
- It is feasible but fragile, and unnecessary given DMIS.

**Does the real T0 help? [V]** Track-free robust σ against the CJKR circles, 77 events and
52,549 hits of c070060:

| Hits | Robust σ [µm] | p95 \|r−median\| [mm] |
|---|---|---|
| h1rec CJXR (reference floor) | 253 | 4.89 |
| Track-free, DMIS (real T0) | 604.8 | 7.33 |
| Track-free, `cjavt0` | 604.2 | 7.34 |
| Track-free, per-track T0 | 598.1 | 7.35 |
| Track-free, mean `T0track` | 627.4 | 7.34 |
| Track-free, one run T0 (455.2) | 637.5 | 8.20 |

- DMIS − `cjavt0` has a median of −0.24 ticks and a robust σ of 1.05 ticks (1 tick ≈ 10 µm of
  drift).
- h1rec's own CJXR reaches a p95 of 4.9 mm against the circles, so a 95% < 2 mm requirement on
  this metric is out of reach even for h1rec.

**Figure captions** (each in `/data/dust/group/h1/klesthen/rawhit_probe/validation/plots/t0/`):
- `fig1_rphi_resid_by_T0`: r-φ distance to the CJKR circles for each T0 choice overlaid, log y.
- `fig2_T0_real_minus_approx`: DMIS − `cjavt0` and DMIS − mean `T0track`.
- `fig3_cjcrec_raw_minus_DMIS`: CJCREC on raw hits minus DMIS.

## Corrections the plan must absorb

1. **D16 exact path.** The regression holds (0.9442). Record 0.926 pooled (0.915–0.944 per run)
   as the lam/ Q2e>5 reference. If a lam/ floor is wanted, about 0.91 per run fits.
2. **D16 track-free 95% < 2 mm is unattainable as defined.** The measured value is 7.3 mm, and
   h1rec's CJXR itself reaches 4.9 mm against the circles. Restate D16 as a median (or robust σ)
   threshold. Adding local β would make the path track-informed, and whether it brings the 95%
   quantile under 2 mm is untested: the README's 507 µm is a robust σ.
3. **D11 event T0.** Use DMIS `TZERO_M` (HAT `T0_CJC`) when `D_TZERO` > 0, else the run default.
   `cjc_t0_src` needs a third value. The gain is provenance, not resolution.
   - Never use the `# CJCC` line in `events.txt`.
   - The writer can join HAT `T0_CJC` by run and event. Dumping DMIS in calibprobe instead would
     be a C++ change, which D9 excludes.
4. **D12 COZ coordinates.** The probe dumps contain no h1rec COZ coordinates (CZZR is all zeros),
   so `coz_trk_x/y/z` must come from `h1hits.COZ.hit()` with the CZR2-matched track. That function
   is validated at the isochrone step only. Add a t ≤ 810 ns cut. A usable criterion: at least
   99.99% of hits with |Δt| < 2e-3 ns, measured at 99.9995% before the cut and 100% after.
5. **D13 / D16 CST.** Replace the 0.4 mm median with a per-layer r-φ residual width plus the
   rotated-φ control. Label the DCLH match as a reproducibility check.
6. **D15 / WP3.** 2005 needs a `mirror/acs/data/05` symlink. IVRSQT 2 is harmless for positions,
   and the C3Z8 absence is harmless in ellipse mode.
7. **D8 environment.** Run `desygroupset h1` before `thish1.sh`, or call the LCG python by full
   path. Otherwise `python3` is `/usr/bin/python3`, which cannot import ROOT, and `-I` drops numpy
   and ROOT.

## Open questions and gaps

- **Data versus MC.** Are data track-free points like-for-like with MC `cjc_xp/xm` from
  `extract_hits.C`, which uses a simulated drift length? Whether the MC mirror points show the same
  β-dependent offset was not checked. This is the decisive check before WP1.
- **COZ reference.** Would running COZNIC/COZG inside the probe fill CZZR and give an h1rec
  reference for COZ z, r and φ? COZNIC, COZG and CTZFIX are untested against h1rec.
- **CJCREC mismatches.** The cause of the 17 CJCREC/DMIS mismatches in B (truncated chain or a
  different CJC4 state) is open.
- **Remaining coverage.**
  - The 2006 e+ hit-level check is missing; it needs a file with the CJC on.
  - A4 covers about 30 events per period.
  - A3 covers three 2007 files at Q2e>150.

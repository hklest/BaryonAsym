# SHERPA3 chain critique — three-critic consensus (2026-10-20)

Three independent read-only critics (adversarial physics, cold implementation audit,
semantics cross-check vs SHERPA 3.0.3 manual / Rivet source) reviewed the chain
run card -> generation -> Rivet -> 10M unit-count CSV -> canonical plotter.

Trigger: SHERPA3 490-bin total 28.961821 nb vs data NoZ photon-Born 23.517167 nb (+23.2%),
while PYTHIA8 19.100458 / RAPGAP31 21.861163 / DJANGOH14 21.222921 nb are close.

## UNANIMOUS FATAL FLAW: 90-seed double-count in the 10M CSV producer (+9.006%)

`H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/data_vs_release_7col_20260629/sherpa3_10M_unitcount_xfpos/producer/run_sherpa3_10M_correctednorm.py`
- Line 17: REFERENCE_CSV = 1M CSV, which already = 100k base + seeds 910001-910090
  (manifest_sherpa3_unitcount_xfpos_1M_490rows.json).
- Lines 117-147: topup scan globs ALL 990 `seed910*` dirs -> seeds 910001-910090 enter twice.
- Numerator = 10.9M event-equivalents; denominator N_GENERATED = 10,000,000 -> x1.09 uniform.
- Manifest evidence: total_numEntries 64,543,049 = 5,923,211 (ref) + 58,619,838 (topup);
  manifest_job_count 90 vs topup_seed_count 990 mismatch visible, unacted.
- Corrected total: 28.961821 / 1.09 = 26.57 nb (reviewer-exact 26.568940 nb, implementation audit;
  consistent with 1M 26.578692 and 100k 26.498815 nb).
- Inflation bin-independent -> published ratio-panel SHAPES unaffected at this level.
- Affects SHERPA3 only; PYTHIA8/RAPGAP31/DJANGOH14 from other producers, unaffected.
- STATE.md lines 39-41 and the 2026-09-25 / 2026-10-08 / 2026-10-13 packages inherit x1.09.

## LEVEL DECOMPOSITION (all three critics agree)

28.961821 / 23.517167 = 1.2315 =
  x1.0900 (double-count) x 1.0969 (documented by-design photon-Born data vs full-Born MC
  mismatch, C_gamma_born 0.911633, STATE.md line 41) x ~1.03 residual.
At matched full-Born level corrected SHERPA3 is +3.0% vs data -> no longer an outlier;
sits at top of generator band (PYTHIA8 -26%, RAPGAP31 -15%, DJANGOH14 -18% at matched level).

## PARTIAL DISAGREEMENT (flagged, needs quantification): unit counting a Weighted MC@NLO sample

- Production card: `sherpa3_mepstest_20260628/rivet_smoke_20260629/work/proc_reuse_p_pattern_20260629/Sherpa.yaml`
  — MC@NLO MEPS DIS (NJET 1, QCUT 5, SDIS 0.4, NNPDF30_nnlo_as_0118, METS),
  BEAMS [-11,2212] e+ only, 27.6 x 920 GeV, Q2 selector [150,20000],
  EVENT_GENERATION_MODE: Weighted (negative-weight RS events present).
- Rivet routine fills unit weights; released z005 hists have sumW == numEntries, so per-bin
  event-weight information is destroyed at CSV build.
- Adversarial critic (FATAL for shape): mean event weight spans ~117,632 (Q2 150-398) to
  ~1,200-1,500 (Q2 ~5000) — unit counting understates low-Q2 bins (x~0.59) and overstates
  high-Q2 bins (x~46-58 vs naive). Predicted artifact 0.588 x 1.09 = 0.641 matches observed
  S0 ratio 0.585-0.655 with no physics input. Claims the released low-Q2-under / high-Q2-over
  pattern (S3 ratios up to 16.4) is the weight-inversion signature; corrected S3 shape UNKNOWN
  until a weighted rebuild.
- Implementation + semantics critics: same mechanism flagged as unquantified bias candidate
  (RS |weight| share ~186.67 pb of 4523.31 pb, <=~8% at event level, inflating), subdominant
  for the integrated level; did not independently verify the per-Q2 weight spread.
- Resolution requires a weighted-fill Rivet variant + sumW-based CSV; no sumW-based per-bin
  table exists for any SHERPA sample today.

## STAGES CLEARED (all three critics)

- Run card: rad-off genuinely effective (YFS: MODE None, ME_QED false; Settings_Report
  seed950001 confirms); selectors coherent; K0s/Lambda stable (Max_Proper_Lifetime 10 mm)
  consistent with routine veto. Minor flags: e+ only vs mixed e-/e+ data (~1-2% at high Q2),
  Ee 27.6 vs 27.5 GeV (~0.5%), hard-coded sigma_gen 4.48721 nb vs log XS 4434-4523 pb (~±1%).
- Generation: exactly 10M unique events (100k + 990 x 10k); plus4M campaign (950001-950400)
  correctly excluded. Open item: seed 910091 originating job untraced (no impact).
- Rivet routine (released path): cuts Q2>150, 0.1<y<0.7, xF>0, pi/K/p + long-lived-ancestor
  veto correct; single-arg fill() semantics fine for unit fills; xF>0-along-qhat ==
  xFGen<0-along-proton equivalent convention, not a bug. Doc bug: README claims "raw weighted
  hadron counts" but code fills unit weights.
- Plotter `compare_data_vs_mc_z005_4generator_born.py`: identical schema/merge/ratio for all
  generators, --lumi-pb metadata-only, dx = 1.0 - x_min cap uniform; no SHERPA-specific path.

## HISTORY (three successive normalization defects, two fixed, one live)

1. ~100x under-count (fixed, pre-correctednorm 10M producer).
2. sigma_gen confusion: 1M builder used 26.4988 nb (100k total) as sigma_gen -> 156.958 nb
   (fixed by correctednorm, sigma_gen 4.48721 nb).
3. 90-seed double-count (LIVE, this finding). Stale 2026-06-27 CSV was the opposite mixed
   convention (sigma_bin inflated x~17,043 at bin 0). Correct formula
   sigma_bin = sigma_gen * sumW_bin / sumW_EVTCOUNT never implemented.

## RECOMMENDED NEXT ACTIONS

1. Rebuild SHERPA3 10M CSV deduplicated (either 100k + all 990 seeds, or 1M ref +
   seeds 910091-910990; N = 10,000,000) -> expected total ~26.568940 nb. Mechanical fix.
2. Add producer assertions: scanned seed set disjoint from reference; Sigma n_events ==
   N_generated (would have caught 90 != 990).
3. Quantify/repair the weighted-sample shape bias: weighted-fill Rivet variant on a small
   SHERPA3 sample, then sumW-based 490-row CSV with sumw2 errors, before quoting ANY
   SHERPA3 shape statement (especially S3 high-Q2 cells).
4. Update STATE.md lines 39-41, context-handoff.md lines 26/35/39, and re-render or annotate
   the three comparison packages (SHERPA3 curves only).
5. Until rebuild: withdraw or explicitly flag the SHERPA3 curve as invalid in the
   5-prediction package; withdraw the "SHERPA over-predicts most at low-z/low-Q2/low-pT"
   narrative (observed pattern is actually low-Q2 UNDER, high-Q2 OVER — the weight-inversion
   signature).

## EVIDENCE BASE (primary artifacts)

- Producer + manifests: H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/data_vs_release_7col_20260629/sherpa3_{1M,10M}_unitcount_xfpos/producer/
- Seed accounting: H1ChargedSidis_HighQ2/MC/sherpa_rivet_z005_xfpos_to1M_20260703/condor/sherpa_to10M_seeds_20260704.txt (899 seeds), resubmit_missing_20260704_sherpa_seed910091_n10000.sub
- Card: H1ChargedSidis_HighQ2/MC/sherpa3_mepstest_20260628/rivet_smoke_20260629/work/proc_reuse_p_pattern_20260629/Sherpa.yaml
- Settings_Report: H1ChargedSidis_HighQ2/MC/sherpa_rivet_z005_xfpos_full_plus4M_20260704a/outputs/seed950001_n10000/Settings_Report/Settings_Report.md
- 100k YODA RAW: H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/data_vs_release_7col_20260629/sherpa/Analysis.yoda (_EVTCOUNT sumW 6.918515e+09 / 1.0e5 events)
- Rivet: H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/H1_2026_I_CHARGED_SIDIS_HIGHQ2.cc
- Canonical CSV: H1ChargedSidis_HighQ2/MC/plots/data_vs_mc_z005_20260704_10M_rivet_unitcount_xfpos_normfix_clean/producer_inputs_normalized/prediction_sherpa3_unitcount_xfpos_10M_correctednorm_normfix_clean_plotter.csv
- Plotter: H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py
- Sherpa 3.0.3 manual: qed-corrections.html, selectors.html, hadronization.html (semantics verified)

Full critic reports retained in session ses_ea979a722ffezLO7kh0b8kLUky (adversarial),
ses_ea979a713ffew5yuKrgX1XfOCI (implementation audit), ses_ea979a6ffffen5zhPh0gxFLc9q (semantics).

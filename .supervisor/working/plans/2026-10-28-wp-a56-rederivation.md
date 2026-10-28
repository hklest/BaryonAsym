# WP-A56′ child plan: veto correction q and ND leakage f_ND re-derived on the 840x mixture, per β bin (2026-10-28)

Parent: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(WP-A56′ block `:650-682`, D12-D25 `:169-281`, gates G1″/G2 `:311-345`, Scope Notes `:481-491`, Slices 1f `:434-437`, Q11
`:1008-1010`). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`; `S` = `D/scripts/restart_2026-10-27`;
`DT` = `D/data/restart_2026-10-27`; `DOC` = `D/docs/restart_2026-10-27`. Forked from the WP-A5 plan
(`.supervisor/working/plans/2026-10-28-wp-a5-veto-signal-loss-correction.md`) and the WP-A6 plan
(`.supervisor/working/plans/2026-10-28-wp-a6-leakage-refit.md`).
Lead: `executor`; `h1-naf-operator` (Condor, tally), `h1-mc-expert` (chain-spread reading if the > 3σ_fit trigger recurs),
`h1-systematics` (σ_q and leakage-leg review), `h1-analysis-critic` (both verdicts).

## Goal

Re-derive, under the WP-A5/WP-A6 freeze protocol (even derives, odd rules), (1) the forward-veto signal-loss correction q
per detector on the DIFF8′ mixture (840x + charm + PDISS + Reggeon, 9012 out) with σ_q = max(|q|/2, odd/even spread, σ_stat)
(D25), propagated on the user grid (D16) in frames A and B, and (2) the two-template η_max(0.4) ∈ [−1, 3.0) fit per β bin
(split 0.523, D20), Q² integrated, D cells combined (D17, D24), giving f_ND per β bin with σ_G2 = σ_fit ⊕ chain spread and δ_edge
beside; rule G1″(a) (≤ 0.05 per user-grid bin) and G2 (≤ 0.30, largest chain) per β bin; evaluate the D24 leakage target
σ_G2/(1 − f_ND) ≤ 0.10 per β bin and cost the η_max(0.4) < 3.0 variant beside any miss (Q11); write the purity table on the
user grid with largest-chain f_ND for WP-C C1; write the critic conditions 1-8 to disk (step 0, D25).

Non-goals: no new production (WP-E), no change to the fit model beyond the fixed-rest component, no D7 re-evaluation (WP-C),
no remedy-1 control region, no Plug energy-shape criterion, no relaxing of D7, no edits to the frozen WP-A5/WP-A6 files, no
parent-plan edits, no 6670 templates (D24; Decision 6), no G5 trade (WP-J; this package supplies its inputs).

Expected change size: 10 new scripts under `S/wp-a56/` (forks of the WP-A5/WP-A6 scripts plus a config module and a test
module, ≈ 1200-1600 lines in all, most of it copied), one Condor wrapper and three `.sub` files under
`D/condor/restart_2026-10-27/wp-a56/`, ≈ 12 data files and ≈ 10 figures under `DT/wp-a56/` and `D/plots/restart_2026-10-27/wp-a56/`,
four records under `DOC/wp-a56/` plus the conditions file under `DOC/wp-a6/`. Budget ≤ 0.3 Condor-days (WP-A5 + WP-A6 together
used ≈ 0.033, `DT/wp-a6/condor_tally.csv` TOTAL and `DOC/wp-a5/g1pp_a_verdict.md:47`).

## Approach

Fork, do not edit: every WP-A5/WP-A6 script that hard-codes the mixture or the unit axis gets a `wpa56` copy driven by one
mixture/unit configuration (`wpa56_common.py`), with a `legacy` configuration that reproduces the frozen 9012 results through the
new code before the mixture is switched (regression first, then re-derivation). Both corrections need event quantities the
WP-A3 skims lack (β, Q², Run/Event), so one Condor re-skim of all 21 labels precedes everything. The leakage fitter gains one
fixed component so the two 840x stream factors can be fitted against their own data periods with charm/PDISS/Reggeon fixed,
then frozen; the per-β leakage fits keep the two-parameter form. Rejected: editing the frozen scripts in place (breaks the
`even_frozen_first` and sha provenance of WP-A5/WP-A6); deriving β from Q² and M_X instead of the DiffTot `beta` branch (D20
names the branch); floating the stream factors inside each per-β fit (Decision 5); 6670 templates (Decision 6); re-skimming
the track-level npz (the cell/Run/Event attach of WP-A5 already supplies what the propagation needs).

## Decisions

1. **Mixture and weights.** DIFF8′ = {8402, 8403, 8404, 8405, 9024, 9033, 9155, 9157, 9158, 6671, 6677}; SIGNAL′ = DIFF8′ minus
   the Reggeon pair; 9012 is absent from DIFF8′ and is fitted once as the SW84 anchor cross-check. Per-label factor for 840x =
   w_period (`DT/wp-d2/period_weights.json`: 0.13241865 / 0.29696051 / 0.15505780 / 0.37482630 for 8402/8403/8404/8405) ×
   stream factor s_5644 (8402, 8405) or s_5643 (8403, 8404), initial 1.0; each 840x period represents only its own data period
   in `represented()` (`S/wp-a/wpa_common.py:50`). Charm, PDISS and Reggeon keep their v3 table factors
   (`D/docs/mc/composite_scale_table_v3_2026-10-20.csv`, `fac_used`, `wpa_common.py:68`) unchanged; the PDISS:elastic ratio
   after the stream factor is reported against R11's 1.20 ± 7.2 % (parent `:82`), not re-tuned here (WP-F). Reason: D23 (i)-(iv);
   the composition stays fixed because only the elastic stream normalisation is a free quantity of the adoption.
2. **Skim fork.** `skim_wpa56.py` = `skim_wpa3.py` (`S/wp-a3/skim_wpa3.py:30` branch list, `:70-77` output keys) plus keys
   `run`, `evno` (RunNumber, EventNumber), `q2` (Q2s), `ys`, `beta` (DiffTot `beta`, D20), same selection and weight recipe,
   output `DT/wp-a56/skims/<label>.npz` for 4 data periods + DIFF8′ (11) + 9012 + ND 7071/7091/7094/7096/7097 (21 Condor
   jobs). Reason: the WP-A3 skims carry neither β nor Q² nor an event id (`skim_wpa3.py:70-77`), and bootstraps by generated
   event (D23 iii) need EventNumber, on which the twins 8402≡8405 and 8403≡8404 match at 99.95 % / 99.94 % on the first 30,000
   entries (`DOC/wp-d2/validation_840x.md:14`; the full-sample rate comes from `twin_pairing`).
3. **Fit units.** Primary: β bin {BLO: β < 0.523, BHI: β ≥ 0.523} × chain {7071, 7091, nd4cat} × split, D cells D0+D1+D2 combined
   by summing their histograms at the natural MC weights (= "MC relative cell populations", D24), Q² integrated; cell −1 excluded
   as before. Cross-check units from the same payload: per M_X bin [4,10), [10,20), [20,40] cells combined
   (continuity with `DOC/wp-a6/g2_verdict_edge30.md:15-35`), per (β, cell), and ALL-β (the `shared` variant: ALL-β scales
   applied to both β bins). The 100/100 fallback (`S/wp-a6/fit_bblite_wpa6.py:19`) is re-checked per unit and logged; a β
   unit is not expected to fall back (odd SB30 data 1683/4305/2482 per M_X bin, `g2_verdict_edge30.md`). Reason: D17, D20, D24 (4).
4. **Fit model, samples, variants.** Two free parameters (s_diff, s_ND) per unit at the event level — one pair per (β bin,
   chain, split), shared across hemispheres, charges and hadron bins; BB-lite likelihood unchanged (`fit_bblite_wpa6.py:27-40`),
   range [−1, 3.0) in 0.05 bins (`DT/wp-a6/range_frozen.json`, reused, not re-frozen), FIT sample mask unchanged
   (`S/wp-a3/fact_wpa3.py:80`). Variants on the even split: `nobb`, `lo1p0`, `hi2p8`, `xip04`, `shared`; `nobb` additionally on
   the odd split so the BB-lite sensitivity Δ_nobb = |f_ND(nobb) − f_ND| is quantified per β bin on the ruling split (expected
   ≤ σ_fit with ≈ 27× template statistics; flagged if larger, condition 2). Toy GOF ≥ 1000 toys on the nominal β units only,
   reported as a diagnostic per G1″(c) (parent `:321-324`), not gated (D25 retired only the gated GOF branch of G2).
5. **Stream factors fixed from a first-pass fit, not floated per β unit.** `fit_bblite_wpa56.py` adds a fixed component F to the
   model (μ = s_d·D + s_n·N + F, σ² gains ΣW_F; F = 0 reduces to the WP-A6 fitter). Step (2) of D24: per stream group unit (e⁺ group:
   data ep0304 + ep0607 vs D = 8402 + 8405, F = charm + PDISS + Reggeon at their factors for those periods; e⁻ group: em0405 + em06
   vs 8403 + 8404), β and cells combined, FIT sample, chain 7071 (7091 and nd4cat reported as the spread on the factor), even
   split; s_d of that fit is the stream factor. Cross-check: the deep-gap count ratio (ec04 < 2.0 after vetoes, ND subtracted at
   the fitted s_ND) per group, expected within 2σ. One iteration: after q (step 3) the group fits are re-run with the q weights
   applied to the MC, then `stream_factors.json` + sha is frozen before any odd read. The per-β fits then float (s_diff, s_ND) with
   the frozen factors inside the DIFF8′ weights; per-β s_diff is reported as `stream_closure` (|s_diff − 1| vs max(3σ, 0.10)),
   a β-shape diagnostic, not gated (D23 i: no β reweight). Rejected: nuisance stream factors inside each per-β fit — within one
   β unit the two stream templates differ only by detector period, so the η_max likelihood constrains their sum only (the WP-A6
   `--templates 3` variant hit a bound in 18 of 24 fits, WP-A6 plan Decision 4), and a per-fit factor would differ per β bin and
   chain while the measurement needs one number per stream. f_ND is invariant to s_diff (`t_qinv`, `S/wp-a6/tests_s2_wpa6.py:43`), so the choice does not move G2.
6. **6670 dropped.** HAT/MODS carries no reco η_max and no Plug/FMD/FTS (`DOC/wp-d/mc_candidates_assessment_2026-10-28.md:40`,
   `:44`: only gen-level η_max), so it cannot enter the FIT sample (vetoes applied) nor the N−1 views; 840x already gives
   182,285 skim events vs 6,779 for 9012 (`DT/wp-a3/skims/`). Not re-openable inside this package.
7. **f_ND definitions.** f_ND(β, chain) = Σ s_ND(β, chain) · N_ND^SEL(β) / N_data^SEL(β) with SEL = the user-grid selection
   (SB30: ec08 < 3.0 or sentinel, three vetoes, x_IP < 0.03, 4 ≤ M_X ≤ 40, `S/wp-c/user_grid_2026-10-28/ug_tables.py:18`), split
   into in-fit (ec04 < 3.0) and band parts; the continuity per-M_X cross-check uses the old SEL30 (x_IP < 0.04, M_X > 3) so it
   compares like with like to `g2_verdict_edge30.md:15-35`. σ_fit, spread = max |f_ND(c) − f_ND(7071)|, σ_G2 = σ_fit ⊕ spread,
   largest chain = argmax f_ND, ceiling flag f_ND(largest) > 0.30, exactly as `S/wp-a6/fnd_wpa6.py:70-77,117`. δ_edge per β bin
   with ρ derived on the residual band (ec08 < 3.0 ∧ ec04 ≥ 3.0) per the condition-5 recipe
   (`S/wp-a6/edge30/cond25_edge30_wpa6.py:5-7`), reported beside σ_G2, never folded in (D25).
8. **Leakage target and miss handling.** Target ratio T(β) = σ_G2(β)/(1 − f_ND,largest(β)) ≤ 0.10. Propagated term per user-grid
   bin = σ_G2 · (f_ND,bin/f_ND) / (1 − f_ND,bin), with f_ND,bin the ND track fraction of that bin at the fitted scales (the relative
   uncertainty of the ND-subtracted yield). On a miss in a β bin the pre-registered variant selection η_max(0.4) < 3.0 is evaluated
   on the same payload (f_ND^var = in-fit part over in-fit data, same σ recipe, event cost = 1 − N_data(ec04 < 3.0)/N_data(SEL))
   and written beside; nothing is switched: the verdict states the Q11 default (accept under G5 if every bin's propagated term
   ≤ 0.15, else the variant) and returns Q11 to the user.
9. **q strata and σ_q.** Nominal q per detector × (M_X bin, D cell) as the parent binds (D14, D17, D24 (3), parent `:1075`),
   derived on the deep-gap N−1 view (ec08 < 2.5), even derives, odd rules, σ_q = max(|q|/2, |q_even − q_odd|, σ_stat) (D25; adds
   σ_stat to `derive_veto_corr.py:13`). The histogram job keeps the WP-A5 seven-bin η axis and the other-veto context `etc` < 3.3
   (`fact_wpa3.py:76`) so the integrated fig2/`veto_eff.json` invariant (`veto_corr_hist.py:11`) stays exact; the 3.0 edge enters
   in `derive`: the η-shape δ table is derived on the four bins [−1,1), [1,2), [2,2.5), [2.5,3.0), and `veto_correction_v2.json`
   keeps all six `ETAK` keys of `veto_weight.py:21` with δ = 0 and `beyond_sel: true` for [3.0,3.2) and [3.2,3.3), so
   `veto_weight.py` is read unchanged; with SEL at 3.0 those two entries must never be hit (asserted). A second table per detector
   × β bin, cells combined, is derived from the same histograms and propagated as the `qbeta` variant so the stratum choice is
   quantified (Open Question 1).
10. **Propagation on the user grid.** `propagate_wpa56.py` replaces the x_F/p_T*² grid of `propagate_wpa5.py:29` by hemisphere
    (sign of x_F in the frame) × β (0.523) × {z* at 0.2 (`ug_tables.py:24,27`) | p_T*² q4 edges} × charge, frames A (nominal,
    `t_xfA/t_pt2A`) and B (`t_xfB/t_pt2B`); the z* split and the q4 edges are read per frame from the `edges` block of
    `DT/wp-c/user_grid_2026-10-28/user_grid_summary.json` (written by `ug_tables.py:228-229`; its sha256 recorded in the table
    JSON) and recomputed as `ug_tables.py:89` does only when that file is absent, stated in the JSON;
    variants, σ_rel joint form, quadrature form, fragile flag and dual reporting as `propagate_wpa5.py:9-16,106-108`; masked if
    σ_rel > 0.05. Propagation sample = SIGNAL′ track skims (`DT/wp-c/study/s<label>.npz`, 840x via `LABELS_840X`,
    `S/wp-c/study/wpc_lib.py:4`) plus Reggeon from `DT/wp-a6/trk/b6671.npz, b6677.npz` (closes the WP-A5 recorded omission),
    event weight = `e_w` × per-label factor of Decision 1 × s_diff(β) of the 7071 nominal fit; cells, Run/Event and ec08 from
    `attach_cells.py` outputs run for the new labels. Bootstrap: 200 Poisson(1) replicas drawn per (stream, EventNumber) so the
    twins share a draw (D23 iii). D7 status is joined from the WP-C C1 binstats when present; otherwise all user-grid bins are
    reported and the ruling is stated on the full grid with the D7 join as a follow-up line.
11. **Purity table.** `purity_fill_wpa56.py` fills the user grid (both frames, both charges, z* and q4 p_T*²) with per-event scales
    by β bin (not by (cell, M_X class), `S/wp-a6/purity_fill_wpa6.py:62`), ND tracks from `b7071.npz` at the largest-chain
    normalisation s_ND(7071) × f_ND,largest/f_ND,7071 per β bin (condition 3), columns purity, f_mig, f_fake, f_nd, f_reg with the
    WP-A6 definitions unchanged (`purity_fill_wpa6.py:118-121`, so C1 reads them as before; they are not a partition of 1), plus
    f_nd_spread, leakage_term, g2_flag, target_flag → `DT/wp-a56/purity_user_grid.csv` for WP-C C1. The WP-C and WP-A6 files are
    not modified (`wpc_untouched`).
12. **Validation set.** (a) regression: the `legacy` configuration (9012 DIFF8, cell × M_X units, old SEL30) reproduces
    `DT/wp-a6/edge30/f_nd_per_mx_SB30.csv` 7071 rows within 1e-6 and the frozen q cells of `DT/wp-a5/veto_correction.json` within
    1e-9 from the new skims; (b) fig2/`veto_eff.json` data fail fractions within 1e-6 (data unchanged); (c) odd/even closure for q
    (spread) and f_ND (|f_even − f_odd| vs σ_fit; the > 3σ_fit trigger of `DOC/wp-a6/g2_verdict.md:135` dispatches `h1-mc-expert`,
    condition 6: odd rules); (d) period stability with four 840x periods: deep-gap f_data and f_MC per data period with its own
    840x period, χ² about the mean (ndf 3) as `S/wp-a5/diagnostics_wpa5.py:6-7`, now for MC too, plus the twin response test
    Δf(8402 − 8405), Δf(8403 − 8404) per detector (same events, different simulation periods); (e) COMB independence bound re-measured
    and budgeted (`DOC/wp-a5/g1pp_a_verdict.md:354`); (f) 9012 anchor: f_ND per β with 9012 in place of 840x within σ_G2.
13. **Records.** Step 0 writes `DOC/wp-a6/g2_edge30_critic_conditions.md` with conditions 1-8 verbatim from the Appendix of this
    plan (D25 names the path). `DOC/wp-a56/freeze_record_v2.md`, `fit_record_v2.md`, `g1pp_a_verdict_v2.md`, `g2_verdict_v2.md` mirror the WP-A5/WP-A6
    records (8 sections, mask lists, `Ruling:` line, `S/wp-a5/tests_wpa5.py:106-114`), each verdict with a conditions table
    (1-8: discharged with evidence, or carried with the owner stage) and the §8 rules carried (fragile dual reporting, threshold
    bin re-evaluated, COMB budget, FMD chain spread, `g1pp_a_verdict.md:345-357`). Inputs for condition 8 go to
    `DT/wp-a56/etacut_eff_per_beta.csv` (840x efficiency of ec08 < 3.0 per β bin and the in-fit data/MC shape χ²) for WP-J.

## Outline

- `S/wp-a56/wpa56_common.py` (new): `MIXTURES = {"diff8p": {...}, "legacy": {...}}` with label lists, per-label factor table
  (period weights × stream factor, read from `DT/wp-a56/stream_factors.json` when present, else 1.0), `represented()` override for
  840x, `UNITS = {"beta": ..., "mx": ..., "beta_cell": ..., "group": ...}`, `beta_class(beta)`, `stream_of(label)`; pattern
  `S/wp-a/wpa_common.py:16-27,50-77`. Gotcha: `table_fac` for 840x must equal the skim `scale` (w_period) so exposure1 is the raw
  event weight.
- `S/wp-a56/skim_wpa56.py` (new): `skim_wpa3.py` + five keys (Decision 2); same `--kind/--label/--input/--out/--runlist/--scale-table`;
  the 840x scale table is `DT/wp-d2/scale_table_840x.csv`, inputs `D/data/t2_detector/mc840x_merged_2026-10-28/<R>.root`
  (others as `D/condor/restart_2026-10-27/wp-a5/wpa5_job.sh:11`). Invariant: (Run, Event) unique (`skim_wpa3.py:35-37`).
- `S/wp-a56/veto_corr_hist_wpa56.py`, `derive_veto_corr_wpa56.py` (forks of `S/wp-a5/veto_corr_hist.py`, `derive_veto_corr.py`):
  `--mixture`, `--strata {mxcell,beta}`; arrays gain a β axis; `sigma_q` per Decision 9; outputs `DT/wp-a56/veto_corr_hists.json`,
  `veto_correction_v2_even.json`, `veto_correction_v2.json` with the WP-A5 schema plus `strata`, `stream_factors_sha256` and the
  `beyond_sel` flag; histogram axis and `etc` unchanged, δ table per Decision 9. `freeze_record_v2.md` replaces `derive_veto_corr.py:21`.
- `S/wp-a56/build_payload_wpa56.py` (fork of `S/wp-a6/build_payload_wpa6.py:14-19`): templates indexed
  `[w][cell][class]` with class ∈ {MX4_10, MX10_20, MX20_40, MXout, BLO, BHI}; data per period (already); `sel` blocks for both
  SEL definitions (Decision 7); `--mixture`; payload `DT/wp-a56/payloads/<split>_<chain>.json` + sha list.
- `S/wp-a56/fit_bblite_wpa56.py` (fork): `assemble()` returns (D, N, F, WD, WN, WF); `_bb`/`fit`/`prof_interval`/`toy_gof` take F;
  `--scope {ALL,D0,D1,D2,GRP_EP,GRP_EM}`, `--unit-class`, `--fixed-rest`; fit outputs under `DT/wp-a56/fits/<mixture>/<split>/`;
  `freeze-streams` writes `stream_factors.json` + sha and refuses if odd fit files exist under `fits/diff8p/` (the slice-1
  `fits/legacy/` files are exempt; pattern `fit_bblite_wpa6.py:191`). Illustrative only: with F the per-bin μ = s_d D + s_n N + F
  and τ = μ²/(s_d² W_D + s_n² W_N + W_F); everything else in `_bb` (`:27-40`) is unchanged.
- `S/wp-a56/fnd_wpa56.py` (fork of `fnd_wpa6.py:13,49-94` + `edge30/fnd_edge30_wpa6.py`, `cond25_edge30_wpa6.py:21`): units per
  Decision 3, both SEL definitions, band-ρ δ_edge, target ratio, variant costing, 9012 anchor; outputs `f_nd_per_beta.csv`
  (columns of `fnd_wpa6.py:13` + `target_ratio`, `target_flag`, `delta_nobb`, `var_f_nd`, `var_sig_g2`, `var_event_cost`),
  `f_nd_per_mx_continuity.csv`, `stability_v2.csv`, `period_fits_v2.csv`, `etacut_eff_per_beta.csv`.
- `S/wp-a56/propagate_wpa56.py` (fork of `propagate_wpa5.py:29-38,52-109` with the grid helpers of `ug_tables.py:27,89-96`):
  outputs `DT/wp-a56/g1pp_a_table_v2.{csv,json}` with columns charge, hemisphere, beta_bin, axis, bin, frame, lo, hi, S_nom, R_up,
  R_down, R_eta, syst_veto_rel, syst_veto_rel_eta_quad, R_eta_<det>, veto_eta_fragile, boot_sigma, masked_veto, masked_D7 (NaN
  until C1), q_strata variant column (`mxcell` | `qbeta`). Reads `veto_weight.py:57` unchanged. Attach jobs: reuse
  `S/wp-a5/attach_cells.py` as is (`--label --input --out`) for 8402-8405 and the Reggeon pair.
- `S/wp-a56/purity_fill_wpa56.py` (fork of `purity_fill_wpa6.py:14-18,62,144-156`): Decision 11; output `purity_user_grid.csv`.
- `S/wp-a56/tests_wpa56.py` (new): the test names below, `--slice 0|1|2|3|4|5|all`.
- `D/condor/restart_2026-10-27/wp-a56/{wpa56_job.sh, skim.sub, attach_cells.sub, fit.sub}` (the hist job queues from `fit.sub`'s
  wrapper; pattern `wp-a5/wpa5_job.sh`,
  `wp-a6/fit/`); fits queue from `fit_bblite_wpa56.py queue`; login node runs only `derive`, `fnd`, `propagate`, `purity`, tests.
- Docs: `DOC/wp-a6/g2_edge30_critic_conditions.md` (step 0); `DOC/wp-a56/{freeze_record_v2.md, fit_record_v2.md,
  g1pp_a_verdict_v2.md, g2_verdict_v2.md}`; plots under `D/plots/restart_2026-10-27/wp-a56/` (q per stratum, η_max stacks per β
  × chain, f_ND per β with target line, G1″(a) σ_rel map on the user grid).

## Slices

0. **Conditions on disk, configuration, skims** (units: `g2_edge30_critic_conditions.md`; `wpa56_common.py`; `skim_wpa56.py` +
   `skim.sub`, 21 Condor jobs). Tests:
   `conditions_file: g2_edge30_critic_conditions.md -> 8 numbered conditions, each quoting its WP-A5/WP-A6 source line`;
   `skim_superset: 9012 and 7071 new skims, old keys -> equal to the wp-a3 skims element-wise; new keys run, evno, q2, ys, beta present`;
   `twin_pairing: 8402 vs 8405 and 8403 vs 8404 evno after selection -> ≥ 99.9 % matched, matched rows equal in mx, beta, ec04`;
   `per_period_weights: Σ_R w_R L_eq,R per period -> L_data,period × stream factor within 1e-6`;
   `mixture_identity: MIXTURES["diff8p"] labels -> the 11 of Decision 1, 9012 absent; "legacy" -> DIFF8 of wpa_common.py:16`.
   Checkpoint: `python3 tests_wpa56.py --slice 0` passes; skim sha256 list written.
1. **Legacy regression through the new code** (units: `build_payload_wpa56.py`, `fit_bblite_wpa56.py`, `fnd_wpa56.py`,
   `veto_corr_hist_wpa56.py`, `derive_veto_corr_wpa56.py` in `--mixture legacy`; payload and hist jobs on Condor, 7071 odd+even
   nominal fits 24 jobs). Tests:
   `payload_identity: legacy odd 7071 payload, cells × M_X classes, bins < 3.0 -> raw and exposure1 within 1e-6 of DT/wp-a6/edge30/payloads/odd_7071_sel30.json`;
   `fixed_rest_reduces: F = 0 -> deviance equals fit_bblite_wpa6 on the same arrays within 1e-9`;
   `regression_fnd_9012: legacy 7071 f_ND per M_X (old SEL30) -> within 1e-6 of f_nd_per_mx_SB30.csv`;
   `regression_q_9012: legacy even q per (detector, M_X, cell) -> within 1e-9 of veto_correction_even.json (sha c026604e…)`;
   `beta_classes_partition: Σ BLO + BHI templates -> equal the ALL template in every bin`.
   Checkpoint: `tests_wpa56.py --slice 1` passes; regression table in `fit_record_v2.md` section "regression".
2. **Stream factors and q on the even split, frozen** (units: hist job on `diff8p`; `derive --split even`; group fits with
   `--fixed-rest` (even, 3 chains); deep-gap cross-check; iteration; `freeze-streams`; `freeze_record_v2.md`). Tests:
   `stream_closure_injected: Asimov e⁺ group with s_5644 = 1.08, s_ND = 1.3 -> recovered within 1e-3`;
   `deep_gap_crosscheck: count-ratio stream factor per group -> within 2σ of the fit value, both written`;
   `iteration_converged: stream factors before and after applying q -> |Δ| < 0.005, both written`;
   `sigma_q_d25: every stratum -> sigma_q ≥ |q|/2, ≥ sigma_stat_even (spread added in slice 3)`;
   `eta_shape_keys: eta_shape -> six ETAK keys, δ derived for the four bins below 3.0, δ = 0 and beyond_sel for the two above; zero SEL entries at ec08 ≥ 3.0`;
   `fig2_consistency: integrated odd Plug f_fail_data per cell over the seven η bins -> within 1e-6 of DT/gate_evidence/summary.json`;
   `even_frozen_first: stream_factors.json and veto_correction_v2_even.json sha256 in freeze_record_v2.md -> timestamps precede any file under fits/diff8p/odd/ and veto_correction_v2.json`.
   Checkpoint: `freeze_record_v2.md` with both shas; `tests_wpa56.py --slice 2` passes.
3. **Odd split: q ruled, per-β fits, f_ND, target, variant** (units: `derive --split odd`; per-β, per-M_X, per-(β, cell), ALL-β
   fits both splits + even variants + odd `nobb` on Condor; 9012 anchor fits; `fnd_wpa56.py`; period fits; plots). Tests:
   `freeze_before_odd: odd submit log -> quotes both frozen shas with an earlier UTC stamp`;
   `g2_table_beta: every (β bin, chain) row -> f_nd, sig_fit, spread, sig_g2, delta_edge, rho_band, above_ceiling, target_ratio, target_flag, delta_nobb written`;
   `spread_as_syst: 7071 rows -> spread == max |f_nd(7091 or nd4cat) − f_nd(7071)|`;
   `continuity_mx: per-M_X f_ND (old SEL30) on diff8p vs g2_verdict_edge30.md:15-35 -> |Δ| and σ_G2 written per bin and chain`;
   `nobb_quantified: odd nobb per β -> delta_nobb written and flagged if > sig_fit`;
   `fallback_logged: units below 100/100 -> listed with the scales used (expected none for β units)`;
   `variant_costed: any β bin with target_flag -> var_f_nd, var_sig_g2, var_event_cost written`;
   `stream_closure_per_beta: per-β s_diff (7071) -> written with the |s_diff − 1| vs max(3σ, 0.10) flag`;
   `period_stability_4: deep-gap f per data period (data and MC) and f_ND per period -> χ² ndf 3 written; twin Δf per detector written`;
   `anchor_9012: f_ND per β with 9012 for 840x -> |Δ| vs σ_G2 written`;
   `swing_trigger: any chain with |f_even − f_odd| > 3σ_fit -> listed (dispatches h1-mc-expert)`.
   Checkpoint: `fit_record_v2.md` sections "even", "odd", "target"; `veto_correction_v2.json` written; slice-3 tests pass.
4. **Propagation on the user grid and purity table** (units: `attach_cells.sub` for 8402-8405, 6671, 6677 (Condor);
   `propagate_wpa56.py` frames A and B, nominal `mxcell` strata and the `qbeta` variant; `purity_fill_wpa56.py`; plots). Tests:
   `g1pp_a_table_user_grid: 2 hemispheres × 2 β × (2 z* + 4 p_T*²) × 2 charges × 2 frames × 2 strata -> every row has both forms and the fragile flag; grid edges and the user_grid_summary.json sha256 recorded in the JSON`;
   `zero_variant_identity: zero variant -> R = 0 and Σw per β bin equals the skim sums`;
   `bootstrap_twin_shared: replica weights of matched 8402/8405 rows -> identical draws`;
   `reggeon_in_propagation: 6671 and 6677 rows -> present with cells attached; their Σw fraction written`;
   `qbeta_vs_mxcell: per bin |σ_rel(qbeta) − σ_rel(mxcell)| -> written, max reported in the verdict`;
   `fraction_identities: per user-grid bin f_nd + f_reg + nsig/ntot -> 1 ± 1e-6, and purity + f_mig -> nsig_gv/ntot ± 1e-6 (purity_fill_wpa6.py:118-121)`;
   `leakage_term_per_bin: every retained bin -> leakage_term written, max per β bin reported against 0.10 and 0.15`;
   `wpc_untouched: sha256 of DT/wp-c and DT/wp-a6 files -> unchanged`;
   `comb_closure: Π(1 − q) vs the COMB view -> finite pulls, independence bound written`.
   Checkpoint: `g1pp_a_table_v2.csv`, `purity_user_grid.csv`, figures exist; slice-4 tests pass.
5. **Verdicts** (units: `g1pp_a_verdict_v2.md`, `g2_verdict_v2.md`; `h1-systematics` review of σ_q and the leakage leg;
   `h1-analysis-critic` rulings; `h1-naf-operator` tally; one handoff line). Tests:
   `gate_text_frozen: quoted G1″(a)/G2 blocks vs the parent plan -> empty diff`;
   `numbers_traceable: every number in the verdict tables -> found in the CSV/JSON outputs (script check)`;
   `conditions_dispositioned: conditions 1-8 -> each marked discharged (with file:line) or carried (with owner stage)`;
   `section8_carried: fragile list, threshold-bin rule, COMB budget, FMD chain spread -> each has a line in g1pp_a_verdict_v2.md`;
   `budget_recorded: condor_tally.csv -> total ≤ 0.3 Condor-days`;
   `verdict_sections: both verdicts -> 8 sections, mask lists, a "Ruling:" line`.
   Checkpoint: both verdicts exist with `Ruling:` lines and the critic's verdict appended.

## Verification

After slice 5: `python3 S/wp-a56/tests_wpa56.py --slice all` passes; `DOC/wp-a56/g2_verdict_v2.md` holds per β bin f_ND (3 chains),
σ_G2, δ_edge, target ratio and flag, the variant side-by-side where flagged, the continuity table, the conditions table;
`g1pp_a_verdict_v2.md` holds the user-grid σ_rel table (both frames, both strata), fragile list, retained count; the two frozen
shas appear in both; `purity_user_grid.csv` is readable by WP-C C1; the parent's `## Log` line is returned to the supervisor.

## Risks

- The chain spread is a model term; the low-β bin may miss the 0.10 target whatever the statistics (parent Risks `:476`): the
  variant is costed, Q11 returns to the user, no lead switches it.
- The stream-group fit may be degenerate between s_d and s_ND in the e⁻ group if the ND shape mimics 840x at high η_max; the
  deep-gap count ratio is the fallback value (recorded as such) if the profile interval exceeds 0.10.
- q is data-limited in the deep gap (n = 5755 per split); 840x shrinks only the MC term, so σ_q stays dominated by |q|/2 and the
  spread; the G1″(a) pass margin on the user grid is not guaranteed by the new statistics.
- The track skims lack Run/Event; the attach step joins by `e_idx` row order (`attach_cells.py:4-7` invariants); a mismatch is
  fatal, not silent.
- The twin pairing is 99.95 %, not 100 %; unmatched twin rows get independent draws (reported).
- The WP-C C1 D7 masks may not exist when slice 4 runs; the ruling is then on the full user grid with the join as a follow-up.
- Purity uses 7071 track shapes at the largest-chain normalisation; if the largest chain is 7091 or nd4cat its track shape
  differs from 7071's (no track npz for them); the spread column carries this.

## Scope Notes

In: everything above. Out: WP-E production, WP-F re-confirmation, WP-J trade (condition 8 inputs only), D7 masks, parent edits,
memory edits, the remedy-1 η_max < 1.5 control region, any Plug energy-shape criterion, relaxing D7, re-freezing the fit range,
editing any WP-A5/WP-A6/WP-C/WP-D2 file. Binding user decisions (not reopened): D12-D16 (veto efficiency, G1″, correction +
systematic, 3.0 edge, user grid), D17 (Q² integrated), D20 (β split), D25 housekeeping defaults; the critic conditions 1-8.
Lead decisions the user may override: D21 (frame A), D23 (840x), D24 (target, Q11 default) and Decisions 5, 6, 9-11 here.

## Assumptions

- DiffTot `beta` is the reco β of D20 for data and all MC labels: checked by `skim_superset` and a one-line correlation with
  Q²/(Q² + M_X²) printed in the skim log.
- `attach_cells.py` runs unchanged on the 840x and Reggeon inputs (the Reggeon npz were built by `wpc_build_bkg_wpa6.py` with
  `e_idx`): checked by its fatal invariants.
- The FIT sample (`fact_wpa3.py:80`) has no ec08 dependence, so the 3.0 edge changes only SEL and the live δ bins: checked by
  `payload_identity`.
- Fits are deterministic (fixed starts, L-BFGS-B): checked by `regression_fnd_9012`.
- The WP-D2 record's open items (gen 4489 bookkeeping, 9.99 M vs 10 M, `adoption_840x_record.md:25-29`) do not affect per-period
  weights at the 1e-3 level: checked by `per_period_weights`.

## Open Questions

1. (user:, may override) q strata: the parent binds (M_X bin, D cell) (D14, D17, `:1075`); the brief names β bin × cells combined.
   Recommended: keep (M_X, cell) nominal and report the `qbeta` propagation beside it (slice 4, `qbeta_vs_mxcell`); switch only if
   the difference exceeds the σ_rel margin in any bin.
2. (user:, conditional) = parent Q11, handled per Decision 8; returned to the user with the slice-3 side-by-side numbers.

## Log

## Todo

- [ ] Slice 0: conditions file, `wpa56_common.py`, 21 skims, tests 0
- [ ] Slice 1: legacy regression (payload, fitter with F, f_ND, q) within tolerances
- [ ] Slice 2: even hist job, stream-group fits, q even, deep-gap cross-check, iteration, freeze (two shas)
- [ ] Slice 3: odd q, per-β/M_X/(β, cell)/ALL fits + variants + nobb odd, 9012 anchor, f_ND tables, target and variant, period and
      twin stability, swing trigger → `h1-mc-expert` if fired
- [ ] Slice 4: attach jobs, propagation (A, B; mxcell, qbeta), G1″(a) table, purity table, plots
- [ ] Slice 5: `freeze_record_v2.md`, `fit_record_v2.md`, both verdicts with conditions table; `h1-systematics` review;
      `h1-analysis-critic` rulings; Condor tally ≤ 0.3; handoff line; return Open Questions 1-2 and the Q10 inputs to the supervisor

## Appendix: `h1-analysis-critic` conditions 1-8 on the edge-30 ruling (relayed by the supervisor; step 0 writes them verbatim)

1. 7091 identity on [10,20): retention is no longer conditional (D15); the identity confirmation by `h1-mc-expert` survives as a
   condition on the quoted σ_G2 where the spread is 7091-driven (`DOC/wp-a6/g2_verdict.md:222`).
2. Robustness variants `nobb`, `lo1p0`, `shared` bind all M_X/β bins; at SB30 `nobb` [20,40] 7091 = 0.353 is flagged
   (`DOC/wp-a6/g2_edge30_conditions_2_5.md`, Condition 2).
3. Purity and leakage budgets use the largest-chain f_ND (or 7071 × the full relative spread), not bare 7071
   (`DOC/wp-a6/g2_verdict_edge30.md:15-35`).
4. δ_edge chain dependence: with ρ derived on the residual band (ec08 < 3.0 ∧ ec04 ≥ 3.0), f_ND + δ_edge crosses 0.30 in odd
   [10,20) and [20,40] (`DOC/wp-a6/g2_edge30_conditions_2_5.md`); reported beside G2 per the user's default (WP-A6 Q1, D25) and
   stated in every G2 record.
5. ρ for the band is derived on that sub-population (done in the conditions 2/5 record, `g2_edge30_conditions_2_5.md:21-28`); carried.
6. The 7091 odd/even swing exceeds σ_fit by more than 2×, so the profile σ is not faithful for 7091; "odd rules" stands
   (`DOC/wp-a6/g2_verdict.md:135`).
7. The WP-A5 §8 rules survive: fragile-bin dual reporting; the threshold bin is re-evaluated on any new production; COMB
   independence is budgeted; the FMD δ chain check is pending (`DOC/wp-a5/g1pp_a_verdict.md:345-357`).
8. The η_max < 3.0 cut efficiency does not cancel (η_max correlates with x_F); G5 must carry σ_G2/(1 − f_ND) (≈ 21 % [10,20),
   13 % [20,40] at the old statistics) and show the trade net favourable (parent D25, `:276-278`).

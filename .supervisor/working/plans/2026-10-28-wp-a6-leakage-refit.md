# WP-A6 child plan: ND leakage re-fit at η_max(0.4) < 3.0, gates G1″(c) and G2 (2026-10-28)

Parent: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(WP-A6 block, `## Gates` G1″(c) and G2, D12-D14, Open Q7). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.
Lead: `executor`; `h1-naf-operator` (Condor, tally), `h1-mc-expert` (chain-disagreement reading), `h1-analysis-critic` (verdict).

## Goal

Re-run the two-template `etaMaxClus04` fit confined to [−1, 3.0) with MC statistics in the likelihood, for the ND
chains 7071 (nominal), 7091 and nd4cat, per (D cell, M_X bin); derive the ND leakage fraction f_ND per M_X bin in
the selected sample (`etaMaxClus08` < 3.3, three vetoes, x_IP < 0.04, M_X > 3 GeV) with fit-statistical ⊕
generator-spread uncertainty; fill `f_nd`/`f_reg` and the rescaled purity on the WP-C grid; rule G2 in
`D/docs/restart_2026-10-27/wp-a6/g2_verdict.md`.

Non-goals: changing the selection (WP-B), the WP-A5 correction derivation, the G2 re-confirmation on the new
production (WP-F), new MC requests, Plug threshold scans, the σ_r^D(3) comparison (WP-G, see Decision 11),
editing WP-C outputs in place, any gate threshold.

Expected change size: about 1100 lines of new Python under `D/scripts/restart_2026-10-27/wp-a6/` (six scripts),
about 60 lines of Condor files under `D/condor/restart_2026-10-27/wp-a6/`, two records under
`D/docs/restart_2026-10-27/wp-a6/`, data and plots under the matching `wp-a6/` directories. Nothing outside
`wp-a6/` changes except one handoff line in `D/context-handoff.md`.

## Approach

Build new fit payloads from the 17 WP-A3 skims (event-level `.npz`, columns `ec04`, `ec08`, `mx`, `xip`, `cell`,
`even`, `w`, veto flags; `D/scripts/restart_2026-10-27/wp-a3/skim_wpa3.py:70-77`) with the WP-A2 FIT mask and
templates partitioned by reco M_X; fork the WP-A2 fitter with a Barlow-Beeston-lite per-bin nuisance; fit per
(cell, M_X) unit; take f_ND in the selected region from the fitted s_ND times the ND template's own count in that
region (in-fit part plus the unfitted [3.0, ∞) band reported separately); fill the WP-C table from ND and Reggeon
track-level builds; freeze on the even split, rule on the odd split.

Rejected: re-using `payload_wpa2.py` (it reads `n1_wpa2` partials that discard M_X,
`D/scripts/restart_2026-10-27/wp-a2/payload_wpa2.py:41-48`); a full χ² with MC-stat errors (bins with 0-5 raw ND
events at η < 1 break the Gaussian approximation, `odd_frozen_7071.json` 7071 raw below η = 1: 5/12/5 per cell);
letting the DIFF8 relative weights float (the `--templates 3` variant put t at its bound in 18 of 24 fits and was
not adopted, `D/docs/restart_2026-10-27/wp-a2/s3_correction_record.md:31-35`); a Condor re-skim (every needed column is
in the skims).

## Decisions

1. **Fit range and binning.** `etaMaxClus04`, edges `linspace(−1, 3.0, 81)` (80 bins of 0.05), no merged bin;
   events with η04 ≥ 3.0 are counted in two out-of-fit classes [3.0, 3.4) and [3.4, 5) plus `oor`, reported with
   data/prediction per 0.05 bin as the edge diagnostic. Thin bins at η < 1 stay in the fit: the gate text binds 0.05
   bins, the Poisson likelihood with the BB-lite term is valid at low counts, and a lower edge of 1.0 is run as a
   stability variant, not as the nominal. The lower edge −1 matches the WP-A2 grid (`n1_wpa2.py:120`), so the
   `payload_identity` test is bin-exact.
2. **Likelihood.** Poisson deviance of the WP-A2 fitter (`fit_7p1_wpa2.py` `dterms`/`fit`) plus Barlow-Beeston-lite:
   one nuisance β_b per bin scaling the total prediction μ_b = s_diff·D_b + s_ND·N_b, Gaussian-constrained with
   σ_b² = s_diff²·Σw²_D,b + s_ND²·Σw²_N,b (per-component `sumw2` at the delivered weights, the same quantity as the
   fig1 MC-stat band), profiled analytically per bin (quadratic in β_b). The deviance, the profile intervals
   (Δ = 1) and the toy GOF (parametric bootstrap as in `toy_gof`, refit with BB-lite) all use the profiled
   likelihood. Toys ≥ 1000 for the nominal units only; variants run without toys.
3. **Fit units.** s_diff and s_ND are free per (cell, M_X class) where the FIT-sample data count in [−1, 3.0) is
   ≥ 100 and the ND raw count there is ≥ 100; otherwise that unit takes the per-cell all-M_X scales (the parent's
   fallback). M_X classes: [4,10), [10,20), [20,40] (WP-C grid, `wpc_lib.py:4`) plus `MXout` (M_X < 4, ≥ 40 or
   non-finite), so the classes partition the FIT sample and the all-M_X per-cell fit equals the WP-A2 basis
   restricted to < 3.0. A shared-scale (per-cell) variant is a diagnostic.
4. **Templates.** DIFF8 = 9012 + 9024 + 9033 + 9155 + 9157 + 9158 + 6671 + 6677 with fixed relative weights
   (`exposure_factors` of the odd_frozen payloads; `wpa_common.fac_used/table_fac`), one ND chain per fit (7071,
   7091, or nd4cat with its sidecar overrides). Reggeon rides inside DIFF8 under s_diff; its fraction f_Reg per
   M_X bin in the selected region is reported for WP-B's D4 check, not fitted.
5. **Nominal split and freeze.** Develop and run every variant on the even split; freeze
   `D/data/restart_2026-10-27/wp-a6/range_frozen.json` (edges, classes, unit thresholds, BB-lite spec, script md5s)
   and quote its sha256 in the odd-split submit log before any odd-split fit (the WP-A2 `submit_s4.sh` pattern,
   `D/condor/restart_2026-10-27/wp-a2/fit_wpa2/submit_log_s4.txt`). G2 is ruled on the odd split; the even result
   is reported beside it.
6. **f_ND in the selected region.** Per M_X bin m and chain c:
   f_ND(m) = Σ_cell s_ND(cell, m) · N_ND^SEL(cell, m) / N_data^SEL(m), with SEL = `F.masks(...)["SB"]`
   (`fact_wpa3.py:79`, `etaMaxClus08` < 3.3) ∧ x_IP < 0.04 ∧ M_X > 3 and N_ND^SEL the chain's weighted count there
   (its own M_X class and cell). Selection edge: the parent WP-A6 block (line 448) and D4 say η_max(0.8) < 3.2; the
   brief and the WP-A2 frozen `SB` mask say 3.3; G2 names no edge. The ruling edge is 3.3 (brief; the frozen WP-A2
   definition), recorded as a deviation from the WP-A6 block text (Decision 13); the final edge is WP-B's and WP-F
   re-derives f_ND on it. The sum is split into the in-fit part (η04 < 3.0) and the band part (η04 ≥ 3.0); both are
   tabulated. Uncertainty components, all in `f_nd_per_mx.csv`: (i) fit statistical = profile σ(s_ND) per unit (max
   of minus/plus, which carries the in-fit template MC-stat through BB-lite) ⊕ MC-stat (√Σw²) of the band part of
   N_ND^SEL only ⊕ Poisson of N_data^SEL, in quadrature over cells; (ii) generator spread =
   max |f_ND(7091 or nd4cat) − f_ND(7071)|; (iii) edge-band term δ_edge(m) = f_ND^band(m) · ρ, ρ = max over the
   [3.0, 3.4) 0.05 bins with ≥ 20 data events of |data/prediction − 1| in the same fit unit. The G2 uncertainty is
   (i) ⊕ (ii) as pre-registered; (iii) is reported beside it and the verdict states whether any bin's ruling would
   flip with (iii) included (Open Q1). What the spread covers: the ND gap-probability model, including the
   generator-dependent slope over 3.0-3.3 (WP-A2 diagnosis). What it does not cover: the LAr-edge response
   mismatch common to all chains (`forward_response_fix_assessment.md` §1.1 for the attribution; data/MC 1.2-1.5
   over 3.0-3.3 for every chain, `D/docs/restart_2026-10-27/wp-a2/s2_diagnostic_record.md:22,55`), which is what
   δ_edge bounds.
7. **G2 reading.** "f_ND ≤ 0.30 under the largest of the three ND chains" is applied to the central value of the
   chain with the largest f_ND in that M_X bin; the uncertainty is quoted beside it, not added. Bins above the
   ceiling are flagged `masked_G2`. This is a reading of the pre-registered text, recorded here for the critic.
8. **Order relative to WP-A5.** The fit runs on uncorrected templates. The D14 correction is a per-(detector, M_X
   bin, cell) random removal applied uniformly over the selection, so inside a (cell, M_X) fit unit it is a constant
   factor on DIFF8 that s_diff absorbs exactly; s_ND, f_ND and the purity denominators are invariant (test
   `q_invariance`). Only fallback units (Decision 3) would see a mixture change, and the correction is derived on
   the same M_X grid. WP-F applies the correction before its fit on the new production, where s_diff is also a
   physics normalisation. WP-A6 therefore has no dependency on WP-A5's output.
9. **Purity table.** Track-level inputs: `wpc_build.py` run unchanged in selection on the merged
   `D/data/t2_detector/mc_merged_2026-10-18/{7071,6671,6677}.root` with the truth-fiducial block bypassed
   (every track counts as background) and an `e_cell` column added, defined exactly as the fit cells,
   `skim_wpa3.py:56-57` (x = Q2s/(4·27.6·920·Ys), boxes of `n1_wpa2.py:119`; not from `Q2es`). The WP-C npz carry
   `e_Q2es`/`e_Yes` but no `e_cell`, so the six signal labels are rebuilt with the same wrapper under `wp-a6/`. The
   fill recomputes every column of one WP-C table (the proposal set `binstats_proposal_D7plus_xip030_frameB_G2.csv`:
   frame B, x_IP < 0.03, D7+; the CLI takes `--rule --frame --xipcut` so the grid Open Q6 selects is one re-run) with
   per-event weights × s_diff(cell, m) for DIFF8 (Reggeon included) and × s_ND(cell, m) for ND, through
   `wpc_lib.pur_stab` and `cell_id` unchanged: f_nd = s_ND N_ND / N_tot, f_reg = s_diff N_Reg / N_tot, purity and
   f_fake with N_tot in the denominator, so purity + f_mig + f_fake + f_nd + f_reg = 1 per bin. Deviation from the
   WP-A6 block's `purity = 1 − f_ND − f_Reg − f_fake`: purity keeps WP-C's D7 migration definition, so the identity
   closes with the existing `p_mig` column (Decision 13 lists it). The per-bin f_nd spread uses the per-M_X relative
   spread of Decision 6. A new dated mask JSON with `.sha256` is written through `make_blind_mask.py`; WP-C files are
   not touched. Caveat recorded in the table header: the track build selects E−p_z 35-70 and `etaMaxClus08` < 3.2
   (`wpc_build.py:97,100-101`), the event-level f_ND uses 3.3; the harmonisation is WP-B's open item W2.
10. **Validation set.** `closure_injected` (Asimov from even MC, s_diff = 0.85, s_ND = 1.5, recovered within 1e-3),
    `bblite_reduces_to_poisson` (Σw² → 0 reproduces the WP-A2 deviance), `payload_identity`, `q_invariance`;
    stability variants on the even split: upper edge 2.8, lower edge 1.0, x_IP < 0.04 applied in the fit, shared
    per-cell scales; chain spread is the systematic. Each variant's f_ND per M_X bin goes
    in a stability table; a variant shift above the quoted G2 uncertainty is flagged in the verdict, not added.
11. **σ_r^D(3) figure not here.** No machine-readable EPJC 72 (2012) 2074 table and no (β, Q², x_IP) yield script
    exist in the workspace (worker check); an uncorrected reco-level comparison would not test the leakage fit.
    The sanity figures of this package are the post-fit η04 stacks per (cell, M_X) and the data vs scaled-cocktail
    M_X and x_IP spectra in the selected region.
12. **Compute.** Payload build: one Condor job per (chain, split), 6 jobs, 8 GB, minutes. Fits: one job per
    (chain, split, cell, unit), 72 nominal with toys plus about 150 even-split variant fits without toys; ≤ 0.15
    Condor-days. Track builds: 7071 (10.3 GB) split per raw file (`D/output_v6dif/mc/7071/sidistree/`, 61 files) if one
    merged job would exceed `+MaxRuntime` 10800, plus 6671, 6677 and the six signal labels (WP-C's `wpc_build.sub`
    ran them at ≤ 3000 s each); ≤ 0.5 Condor-days. Total ≤ 0.7 Condor-days, leaving WP-A5's ≈ 0.05 inside the joint
    1 Condor-day cap; tally by `h1-naf-operator` in `D/data/restart_2026-10-27/wp-a6/condor_tally.csv`. Login node:
    single-file tests only.
13. **Record format.** `g2_verdict.md` mirrors `g1prime_verdict.md`: the G1″(c) and G2 text quoted verbatim from
    the parent; the per-M_X table (f_ND per chain, in-fit/band split, fit σ, spread, δ_edge, ceiling flag, largest
    chain); the per-unit scale table with toy p (diagnostic); the edge diagnostic; the stability table; deviations
    from pre-registration (at least: selection edge 3.3 against the WP-A6 block's 3.2, Decision 6; purity keeps the
    D7 migration definition, Decision 9); budget; sha256 of `range_frozen.json`, the CSVs and the mask; open items;
    `RULING:` line by `h1-analysis-critic`. If in any M_X bin the chain spread exceeds 3× the fit σ, `h1-mc-expert`
    gives a one-paragraph forward-flow reading that the verdict quotes.

## Outline

- `D/scripts/restart_2026-10-27/wp-a6/build_payload_wpa6.py` — `--chain {7071,7091,nd4cat} --split {even,odd}
  --out`; loads skims with `fact_wpa3.load` and `masks` (`:68-80`), weights per `veto_eff_hist.py:74`; writes the
  `wpa-fit-payload-v1` layout of `odd_frozen_*.json` (`edges`, `modes.cluster.data.templates.raw[cell]`,
  `modes.cluster.components[label].templates.{raw,exposure1,signed}[cell]`, `scale_table_basis.exposure_factors`)
  extended with a new `sumw2` key beside `raw`, a `mx_class` level under each cell, `band_3p0_3p4`, `band_3p4_5`,
  `oor`, and `sel` (SEL counts per cell × class, in-fit and band, data and components, with `sumw2`). Gotcha: the
  `ec08 < −900` sentinel is kept in the mask though no skim checked carries it (slice 1 logs the count); `period` is
  −1 for MC.
- `D/scripts/restart_2026-10-27/wp-a6/fit_bblite_wpa6.py` — fork of `fit_7p1_wpa2.py` with
  `dev_bb(y, d, n, sw2_d, sw2_n, q)` (profiled deviance), `fit`, `prof_interval`, `toy_gof` calling it; CLI adds
  `--mx-class`, `--eta-lo/--eta-hi`, `--xip-cut`, `--no-bb`; outputs the WP-A2 JSON/CSV set per unit plus
  `edge_diagnostic` (per 0.05 bin in [3.0, 3.4): data, prediction, ratio). Illustrative BB-lite per bin, marked:
  with τ_b = μ_b²/σ_b², β_b solves β² τ + β(μ − τ) − y = 0 (positive root) when σ_b > 0, β_b = 1 otherwise.
- `D/scripts/restart_2026-10-27/wp-a6/fnd_wpa6.py` — reads the fit JSONs and payload `sel` blocks; writes
  `D/data/restart_2026-10-27/wp-a6/f_nd_per_mx.csv` (columns: `split, mx_bin, chain, f_nd, f_nd_infit, f_nd_band,
  sig_fit, spread, delta_edge, f_reg, n_data_sel, largest_chain, above_ceiling, flip_with_edge`) and the stability
  CSV; applies the fallback rule of Decision 3 and records which units fell back.
- `D/scripts/restart_2026-10-27/wp-a6/wpc_build_bkg_wpa6.py` — thin wrapper importing `wpc_build.py` selection code,
  `--label --input --out --gen {on,off}` (off bypasses the gen block for 7071/6671/6677; on for the six signal
  rebuilds), adds `e_cell` per `skim_wpa3.py:56-57` from the `Q2s`/`Ys` branches `wpc_build.py:25` already reads;
  `D/scripts/restart_2026-10-27/wp-a6/purity_fill_wpa6.py` — `--scales <fit summary> --frame B --xipcut 0.03
  --rule D7plus --out`; reuses `wpc_lib.cell_id/pur_stab`; writes `binstats_wpa6_<rule>_<frame>_<xip>.csv` and calls
  `D/scripts/restart_2026-10-27/wp-c/mask/make_blind_mask.py` for the mask.
- `D/scripts/restart_2026-10-27/wp-a6/plots_wpa6.py` — post-fit stacks per (cell, class) with ratio and MC-stat
  band (fig1 layout, `D/scripts/restart_2026-10-27/gate_evidence/plot_ge.py`), f_ND vs M_X with the three
  uncertainty bands, M_X and x_IP in SEL; `captions.md` beside the PNGs.
- `D/scripts/restart_2026-10-27/wp-a6/tests_wpa6.py` — the slice tests below as functions, one-file inputs only on
  the login node.
- `D/condor/restart_2026-10-27/wp-a6/{build,fit,trk}_wpa6.sub` + `_job.sh` + queue files + `submit_*.sh` writing
  `submit_log_*.txt` with the sha256 of `range_frozen.json` and the payloads (pattern `fit_wpa2.sub`,
  `submit_s4.sh`; 1 CPU, 1-8 GB, `+MyProject="h1"`, RedHat9; no environment line, LCG python inherited).
- Invariants: the odd split is read by fits only after the freeze; no script applies `etaMaxClus08 < 1.5`; the
  `[3.0, 3.4)` and `[3.4, 5)` classes never enter a likelihood.

## Slices

1. **Payloads with M_X classes and SEL counts** (unit: `build_payload_wpa6.py`, `build_wpa6.sub`). Delivers six
   payloads under `D/data/restart_2026-10-27/wp-a6/payloads/`. Tests:
   `payload_identity: odd 7071 payload, per cell summed over classes, bins < 3.0 -> raw equal and exposure1 within 1e-6 of odd_frozen_7071.json`;
   `mx_partition: Σ classes per cell -> equals the all-class template, every bin`;
   `sel_count_one_file: SEL data count for the ep0304 skim -> equals np.sum(F.masks(e, False)["SB"] & (xip<0.04) & (mx>3))`;
   `sumw2_present: every component block -> has sumw2 of the same length as raw`.
   Checkpoint: `python3 tests_wpa6.py slice1` passes; payload sha256 list written.
2. **BB-lite fitter frozen on the even split** (units: `fit_bblite_wpa6.py`; `fit_wpa6.sub` + queues). Delivers the
   even-split nominal and variant fits and `range_frozen.json`. Tests:
   `bblite_reduces_to_poisson: --no-bb on the odd_frozen payload cut at 3.0 -> deviance equals fit_7p1_wpa2 on the same arrays within 1e-6`;
   `closure_injected: Asimov even MC with (0.85, 1.5) -> recovered within 1e-3, both scales`;
   `q_invariance: DIFF8 × 0.8 in one unit -> s_ND unchanged within 1e-9, s_diff × 1.25`;
   `range_frozen: range_frozen.json -> edges [−1, 3.0) in 0.05 steps, no merged bin, sha256 sidecar written`.
   Checkpoint: `D/docs/restart_2026-10-27/wp-a6/fit_record.md` section "even split" with the scale and stability tables.
3. **Odd-split fits, f_ND per M_X, G2 table and plots** (units: `fnd_wpa6.py`, `plots_wpa6.py`; odd queue). Delivers
   `f_nd_per_mx.csv`, the stability CSV, the figures. Tests:
   `freeze_before_odd: submit_log_odd.txt -> quotes the range_frozen.json sha256 with a UTC stamp earlier than the odd condor_submit`;
   `g2_table: every (mx_bin, chain) row -> f_nd, sig_fit, spread, delta_edge, above_ceiling present`;
   `spread_as_syst: 7071 rows -> spread == max |f_nd(7091 or nd4cat) − f_nd(7071)|`;
   `parts_sum: f_nd_infit + f_nd_band -> equals f_nd within 1e-9`;
   `fallback_logged: units with data < 100 or ND raw < 100 -> listed with the per-cell scales used`;
   `plots_complete: 3 chains × 3 cells × 4 classes stacks + f_nd figure -> files exist with captions`.
   Checkpoint: `fit_record.md` section "odd split" and the per-M_X table.
4. **Purity table and mask** (units: `wpc_build_bkg_wpa6.py` + `trk_wpa6.sub` (may start after slice 1);
   `purity_fill_wpa6.py` after slice 3). Delivers `binstats_wpa6_*.csv` and `mask_wpa6_*.json` + `.sha256`. Tests:
   `nd_tracks_present: 7071 npz -> ≥ 20 raw tracks in every (charge, mx_bin)`;
   `fractions_sum: per bin purity + f_mig + f_fake + f_nd + f_reg -> 1 ± 1e-6`;
   `purity_table: both axes × both charges × 3 M_X bins -> every row has f_nd, f_reg, purity, masked`;
   `wpc_untouched: sha256 of the WP-C binstats and mask files -> unchanged`;
   `mask_sha: mask JSON sidecar -> matches, source CSV sha recorded inside`.
   Checkpoint: the fill script's summary line (bins masked by G2, by D7+, purity shift vs WP-C median).
5. **Verdict** (unit: `g2_verdict.md`; `h1-analysis-critic` ruling, `h1-mc-expert` reading if triggered;
   `h1-naf-operator` tally; one handoff line). Tests:
   `gate_text_frozen: diff of the quoted G1″(c)/G2 block vs the parent plan -> empty`;
   `numbers_traceable: every number in the verdict tables -> found in f_nd_per_mx.csv, the fit summaries or the stability CSV (script check)`;
   `budget_recorded: condor_tally.csv -> total Condor-days ≤ 0.7`.
   Checkpoint: `g2_verdict.md` exists with a `RULING:` line.

## Verification

After slice 5: `python3 tests_wpa6.py all` passes on the login node (single-file inputs); `f_nd_per_mx.csv` has
3 M_X bins × 3 chains × (even, odd); the verdict's per-M_X table reproduces from the CSV; `git status` shows changes
only under `wp-a6/` paths and the handoff line; the parent WP-A6 tests `range_frozen`, `payload_identity`,
`g2_table`, `spread_as_syst`, `purity_table` map to slices 2, 1, 3, 3, 4.

## Risks

- The band part dominates f_ND (ND η04 rises steeply toward the edge): then δ_edge can exceed the pre-registered
  uncertainty and the verdict carries a flip flag; the remedy is WP-B's (a selection edge at η04 < 3.0 or a tighter
  x_IP window), not WP-A6's.
- D2 × [20,40] or [4,10) may fall back to per-cell scales; the fallback hides an M_X dependence of s_ND there.
- The 7071 track build on one 10 GB merged file may exceed the runtime; the per-raw-file queue is the fallback and
  costs nothing but job count.
- `wpc_build.py` gen-side code on ND rows is untested; the wrapper bypasses it, but shared helpers may still assume
  `mirFound`.

## Scope Notes

In: the five slices. Out: everything under Non-goals; the remedy-1 control region (η08 < 1.5) in any step; any
Plug energy-shape criterion; changing a threshold of G1″ or G2 (user sign-off); 7091/nd4cat track builds. User
decisions not to reopen: D12-D14, G1″ re-gating, Q7's recommendation (GOF-dependent G2 branch
retired), fit basis = WP-A2 frozen set with the range changed (parent WP-A6 Scope Notes). Memory: no entry affects
`wp-a6/`; `.supervisor/memory/decisions/2026-10-26-response-mc-stat-in-quoted-totals.md` is consistent with
Decision 2.

## Assumptions

- The event-level scales s_diff, s_ND apply unchanged to track yields (checked: the fill reports the per-bin raw
  track counts per component; a data/MC track-multiplicity check is WP-G's).
- s_ND fitted without the x_IP cut holds inside x_IP < 0.04 (checked by the x_IP variant in Decision 10).
- `wpc_build.py` runs on 7071/6671/6677 with the gen block bypassed (checked by `nd_tracks_present`).
- The inherited LCG python (numpy 1.26.4, scipy 1.15.0) serves the Condor jobs as it did for WP-A2 (checked by the
  wrapper's import line).

## Open Questions

- **Q1 (user:) edge-band term in the G2 uncertainty.** Options: (a) rule with the pre-registered fit ⊕ spread and
  report δ_edge with a flip flag; (b) add δ_edge to the G2 uncertainty before ruling. Recommendation: (a) for this
  package, since the uncertainty definition is pre-registered; decide (b) for WP-F if any flip flag is set.

## Log

## Todo

- [ ] S1 payload builder, Condor build job, slice-1 tests, payload sha256 list
- [ ] S2 BB-lite fitter, even-split nominal and variant queues, `range_frozen.json`, `fit_record.md` even section
- [ ] S3 odd queue after the freeze, `fnd_wpa6.py`, `f_nd_per_mx.csv`, stability CSV, plots and captions
- [ ] S4 ND/Reggeon track builds, `purity_fill_wpa6.py`, `binstats_wpa6_*.csv`, dated mask + sha256
- [ ] S5 `g2_verdict.md`, `h1-analysis-critic` ruling, `h1-mc-expert` reading if triggered, Condor tally, handoff line

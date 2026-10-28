# WP-A5 child plan: forward-veto signal-loss correction and gate G1″(a)

Parent: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(WP-A5 block, G1″(a), D12–D14, Scope Notes). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.
Date 2026-10-28. Lead for execution: `executor`; `h1-naf-operator` submits Condor; `h1-systematics` reviews the
systematic assignment; `h1-analysis-critic` rules the gate.

## Goal

Derive, per forward veto (Plug, FTS, FMD) and per (M_X bin of the WP-C start grid, D cell), the data−MC signal-event
veto fail-probability difference from the deep-gap N−1 view on the even `EventNumber` split, freeze it, check it on the
odd split, assign the D14 systematic, propagate it as event weights through the per-event-normalised x_F and p_T*²
spectra of the signal cocktail in both WP-C interim frames, and write `D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md`
with the per-measurement-bin ≤ 0.05 table (pass or mask). Non-goals: any Plug energy-shape criterion; the remedy-1
control region (η_max(0.8) < 1.5) or a single q per period; re-fitting ND scales (WP-A6); new skims of data; a new MC
request; changing the WP-C grid or masks.

Expected change size: five new Python scripts (≈ 550 lines in total, forks of existing ones), two `.sub` files, one
wrapper `.sh`, one JSON correction file plus a frozen even-split JSON and its sha256 record, one CSV/JSON per-bin table,
three figures with captions, one verdict document. No edits to existing scripts, data or documents.

## Approach

One Condor job histograms pass/fail sums over the 17 WP-A3 skims (fork of `veto_eff_hist.py`, M_X axis changed to the
WP-C grid). A derivation script turns the even-split sums into q and freezes them; a second run after the freeze adds
the odd split and the systematic. A small Condor job attaches D cell and η_max to the existing WP-C track-level skims
(`s<label>.npz`, frames A and B already computed). A propagation script reweights those events with
Π_v(1 − q_v) under the nominal, ±σ and η-shape variants and writes the relative change per measurement bin. Diagnostics,
plots and the verdict close the package.

Rejected alternatives (one line each): a new per-track skim (the WP-C skims already hold x_F, p_T*² in both frames);
random event removal (same expectation as weights, adds MC variance, not reproducible); a parametrised q(M_X, η_max)
(stat precision per cell is already at the |q|/2 floor, so a fit would add model choice without reducing the
systematic); a scalar extrapolation term (a normalisation shift cancels in a per-event-normalised spectrum; the gate
measures sculpting); the ratio model f_data = R·f_MC as the η_max extrapolation (R falls from 4.4 to 2.8 across the η
bins and overshoots the [3.0,3.3) data, `veto_efficiency_view.md` per-η table).

## Decisions

- **A5-D1 Split protocol.** q is derived from the even split with MC even (the `veto_eff_hist.py` convention: data and MC
  of the same parity), written to `veto_correction_even.json`, and its sha256 recorded in `freeze_record.md` before any
  odd-split number is read; the odd split then gives the per-cell spread. Pattern:
  `D/docs/restart_2026-10-27/wp-a2/s3_correction_record.md:64,78` (`corrections_are_definitions`; JSON never edited,
  errata in the .md). The freeze is procedural, not blind: the gate-evidence view already published both parities at
  ALL/tertile granularity; the per-(WP-C M_X bin, cell) values have not been read.
- **A5-D2 Per detector, combined multiplicatively.** q_v from each N−1 view (Plug, FTS, FMD), nominal chain 7071 with the
  fine G1′(c) scales (`veto_eff.json` `fit_scales_fine`), q_v = Δf/(1 − f_MC,full) with Δf = f_data − f_MC,full. The MC
  weight for a passing signal event is Π_v(1 − q_v); independence is checked, not assumed silently, by the COMB closure
  diagnostic (A5-D9). FMD and FTS follow the same procedure as Plug with no special case: the even-split FMD Δf is
  consistent with zero only over ALL cells (−0.004, 7071); per cell it is D0 +0.023, D1 −0.030, D2 −0.090
  (`veto_eff.json` `deep_gap_eta_lt_2p5` `N1_FMD|even|D*`), so FMD gets a real per-cell q under the same rule, with a
  D2 weight above 1. Negative q is allowed (weight > 1). Chain spread (7091, nd4cat) is reported per cell, not added
  to σ (parent WP-A5 block, step (2)).
- **A5-D3 Binning: 3 × 3 step function, no merging, no smoothing.** M_X ∈ {[4,10), [10,20), [20,40]} × {D0, D1, D2}, per
  detector and split. Deep-gap Plug N−1 data per parity (worker count, 2026-10-28): D0 1242/1903/608, D1 329/776/320, D2
  43/175/129, so the binomial σ_q is 0.007–0.016 in D0/D1 and 0.02–0.045 in D2, below the |q|/2 ≈ 0.033 floor everywhere
  except D2 × [4,10) (±0.043; Open Q-A). Events with cell −1 or M_X outside [4,40] are not in any measurement bin; they
  receive the ALL-cells q of their M_X bin (or the all-M_X ALL q) so the weight module is total. A finer or fitted form
  is rejected (Approach).
- **A5-D4 η_max dependence: constant nominal, additive η-shape variant as the systematic.** Nominal q(M_X, cell) applies
  to every selected event (η_max(0.8) < 3.2 or sentinel), as proposed in D14. Reasons: the user binning is (M_X, cell);
  above 2.5 the excess cannot be attributed to signal events (purity 0.68/0.43) nor excluded as ND mis-normalisation;
  within the deep gap the η trend (+0.038 → +0.088) is in part a purity trend (0.97 → 0.83). The lead's "[2.5,3.0)
  extrapolation term" is implemented as a weight variant, not a scalar: q_η(η, M_X, cell) = q(M_X, cell) + δ_v(η bin),
  δ_v = q_v,ALL(η bin) − q_v,ALL(deep gap), from the even split, ALL cells, all M_X, in the six η bins [−1,1), [1,2),
  [2,2.5), [2.5,3.0), [3.0,3.2) and "other" (< −1 or sentinel, δ := δ[−1,1)); additive because the FMD ratio is
  ill-defined at Δf ≈ 0. The relative change S_η/S_nom − 1 per measurement bin is the η-shape term.
- **A5-D5 Systematic per cell and per bin.** σ_q = max(|q|/2, |q_even − q_odd|) per detector and cell (user rule). Per
  measurement bin the quoted relative systematic is σ_rel = sqrt(max(|R_up|, |R_down|)² + R_η²) with R = S_var/S_nom − 1,
  where "up"/"down" shift all three detectors coherently by +σ_q/−σ_q (same sign; Plug and FTS excesses plausibly share a
  forward-activity cause, so coherent is the conservative choice; per-detector pieces are reported). Pass if σ_rel ≤ 0.05;
  otherwise the bin is flagged `masked_veto`. Both frames A and B are evaluated (WP-C C1 has not frozen the frame); the
  flag is set per frame and the verdict reports both.
- **A5-D6 Propagation sample.** The six WP-C SIGNAL track skims `D/data/restart_2026-10-27/wp-c/study/s{9012,9024,9033,
  9155,9157,9158}.npz` (reco selection: parent env, Q²_es > 150, 0.1 < y_es < 0.7, η_max(0.8) < 3.2, three vetoes pass,
  0 < x_IP < 0.04, M_X > 3; 2503 events / 15 686 tracks for 9012), event weight `e_w` × s_diff(cell, 7071). Reggeon
  (6671/6677) has no track skim and is ≤ 0.15 per bin (parent D4); it is omitted from the propagation (a deviation from parent
  step (4) "DIFF8 reco events", recorded in the verdict's deviations section) and the correction is nonetheless defined
  for all DIFF8 downstream (D14). The spectrum per (charge, M_X bin, frame) is
  S(bin) = Σ_tracks w′ / Σ_events w′ with w′ = w·Π_v(1 − q_v); variants reweight the same events, so MC statistics
  cancel in R to first order; a 200-resample event bootstrap of R is reported per bin as a diagnostic.
- **A5-D7 Application form for downstream.** `veto_weight.py` exposes one function (Outline) that every later package
  (WP-E, WP-F, WP-H) imports; the JSON is the record, the module is the interface. Weights, not removal (Approach).
- **A5-D8 Validation set (all reported in the verdict).** (i) `fig2_consistency` and `deep_gap_reproduced` to 1e-6 against
  `summary.json`/`veto_eff.json` (requires the histogram job to keep the full η and M_X coverage, Outline); (ii) odd
  closure: pulls (q_odd − q_even)/σ_stat per cell and flatness χ² over cells and M_X bins per split (plot_veto_eff
  `flat`); (iii) period stability: deep-gap f_data per period (ep0304, em0405, em06, ep0607) per detector, both
  parities, χ² about the weighted mean (ndf 3); a > 3σ period is an open item, not a gate (MC is period-blind beyond
  exposure composition, so data alone tests period dependence; this replaces, and does not reuse, the remedy-1 single q
  per period); (iv) chain spread per cell.
- **A5-D9 COMB closure (independence check).** On the deep-gap COMB sample (no veto applied, fail = any), the MC fail
  fraction after correction, f′ = 1 − Σ_pass w·Π_v(1 − q_v)/Σ w over DIFF8 (ND unchanged), is compared with the data COMB
  fail fraction per split; |f′ − f_data| beyond 2σ is recorded as an open item (COMB purity 0.58, so a diagnostic).
- **A5-D10 Compute.** Two Condor clusters: the histogram job (fork of `veto_eff_hist.sub`; the original ran 20 s wall,
  264 MB peak, requested 8 GB — request 2 GB) and the cell-attach job (six labels, DiffTot only, < 1 min each, 2 GB).
  The derivation, propagation, diagnostics and plots read JSON/npz of a few MB and run on the login node (single-file,
  seconds). Budget ≈ 0.01 Condor-days; with WP-A6's 0.1 well inside the 1 Condor-day cap. `.sub` files under
  `D/condor/restart_2026-10-27/wp-a5/`, logs under `.../wp-a5/logs/`.
- **A5-D11 Record format.** `g1pp_a_verdict.md` mirrors `D/docs/restart_2026-10-27/wp-a/g1_verdict.md` (sections:
  pre-registered gate text verbatim; inputs and provenance with sha256 of the 17 skims, the five scripts, both JSONs and
  the freeze record; correction table; validation tests as a PASS/FAIL table; per-bin table summary with the mask list
  per frame; diagnostics; deviations from the pre-registration; ruling by `h1-analysis-critic`). The per-bin table CSV
  uses the parent's shared row keys (`charge, mx_bin, axis, bin`) plus `frame, S_nom, R_up, R_down, R_eta, syst_veto_rel,
  boot_sigma, masked_D7, masked_veto`, joinable with `mask_start_frame{A,B}_G2_D7.json`.

## Outline

All new files; nothing existing is edited. Python: the cvmfs LCG_107a `python3` the gate-evidence job used via
`getenv = True` (plain `python3 -I` has no numpy).

- `D/scripts/restart_2026-10-27/wp-a5/veto_corr_hist.py` — fork of
  `D/scripts/restart_2026-10-27/gate_evidence/veto_eff_hist.py` (keep `acc` :59, `ebin` :34, `mbin` :39, the checks
  :87). Changes: `ETAE = [-1, 1, 2, 2.5, 3.0, 3.2, 3.3]` (six bins + other, :24); M_X edges fixed
  `[-inf, 4, 10, 20, 40, inf]` (replace the tertiles :55; no quantile), array shape `(4, 7, 6, 2)`; output
  `D/data/restart_2026-10-27/wp-a5/veto_corr_hists.json` with the same `arrays` keys `"<view>|<parity>|<chain|data>|<group>"`.
  Invariant: integrating over all η and M_X bins reproduces `veto_eff_hists.json` sums exactly (same masks
  `fact_wpa3.masks` :74-80, same weight basis `hist_ge.py:89-91`). Gotcha: the 18–22 data events at −900 ≤ η < −1 are in
  "other", outside the deep gap (deep gap = [−1, 2.5), n = 5755 odd Plug ALL in `veto_eff.json`).
- `D/scripts/restart_2026-10-27/wp-a5/derive_veto_corr.py --split even|odd` — reads the hists; per view v ∈ {N1_PLUG,
  N1_FTS, N1_FMD}, cell c ∈ {D0, D1, D2, ALL}, M_X bin m ∈ {4-10, 10-20, 20-40, ALL}: `f_data, n_data, f_mc_full,
  err_mc, purity, delta, q, sigma_stat` (formulas as `plot_veto_eff.py:47,65,82,83`, scales as `mc_sums` :50-58);
  per view and η bin (ALL cells, all M_X): `q_all_eta[bin]` and `delta_eta[bin] = q_all_eta[bin] − q_all_deepgap`.
  `--split even` writes `veto_correction_even.json` and refuses to run if it exists; `--split odd` requires
  `freeze_record.md` (sha256 of the even JSON, UTC timestamp) and writes `veto_correction.json` = even block + odd block
  + per cell `spread = |q_even − q_odd|`, `sigma_q = max(|q_even|/2, spread)`, chain values for 7091/nd4cat, and
  `even_sha256`. Data shape:
  ```
  {"meta": {skims: {label: sha256}, scripts: {...}, scales: fit_scales_fine, mx_edges: [4,10,20,40],
            eta_edges: [-1,1,2,2.5,3,3.2,3.3], deep_gap: "[-1,2.5)", nominal_chain: "7071", even_sha256},
   "cells": {"N1_PLUG": {"4-10|D0": {even: {...}, odd: {...}, q: , sigma_q: , spread: , chain: {...}}, ...}},
   "eta_shape": {"N1_PLUG": {"[-1,1)": delta, ..., "other": delta}, ...}}
  ```
- `D/scripts/restart_2026-10-27/wp-a5/attach_cells.py --label L --input <root> --out <npz>` — opens `DiffTot` of
  `D/data/t2_detector/mc9012_merged_2026-10-18/9012.root` or `D/data/t2_detector/mc_merged_2026-10-18/<L>.root`,
  reads `RunNumber, EventNumber, Q2s, Ys, etaMaxClus08` for the rows `e_idx` of `s<L>.npz`
  (`wpc_build.py:187`), assigns the D cell exactly as `skim_wpa3.py:56-57` with `n1_wpa2.D` (:119), writes
  `D/data/restart_2026-10-27/wp-a5/cells_<L>.npz` with `e_idx, run, event, cell, ec08, even`. Invariant: every attached
  `ec08` is < 3.2 or < −900 (the skim's own cut, `wpc_build.py:100-102`), (run, event) unique.
- `D/scripts/restart_2026-10-27/wp-a5/veto_weight.py` — `load(path=VETO_CORRECTION_JSON) -> dict` and
  `weights(mx, cell, ec08, variant="nominal", detectors=("N1_PLUG","N1_FTS","N1_FMD")) -> np.ndarray`, variant ∈
  {`nominal`, `up`, `down`, `eta`, `zero`, `nominal_<det>`, `up_<det>`, `down_<det>`, `eta_<det>`}; returns
  Π_v(1 − q_v) per event; q lookup by digitised M_X and cell with the ALL fallbacks of A5-D3; clips 1 − q at 0.
- `D/scripts/restart_2026-10-27/wp-a5/propagate_wpa5.py` — loads the six `s<L>.npz` via
  `D/scripts/restart_2026-10-27/wp-c/study/wpc_lib.py` `load()` and `cells_<L>.npz`; weights `e_w × s_diff(cell)`;
  spectra per (frame A|B, charge h+|h−, M_X bin, axis xF|pt2, bin) on `XF_EDGES`/`PT2_EDGES` (`wpc_lib.py:5-6`), tracks
  binned with `t_xfA/t_pt2A/t_xfB/t_pt2B`; variants per A5-D5; joins `masked` from
  `D/data/restart_2026-10-27/wp-c/mask/mask_start_frame{A,B}_G2_D7.json` (`bins[]` keys `axis, bin, charge, mx_bin`);
  writes `D/data/restart_2026-10-27/wp-a5/g1pp_a_table.{csv,json}`. Per-event normalisation is per M_X bin after
  summing cells (worker note).
- `D/scripts/restart_2026-10-27/wp-a5/diagnostics_wpa5.py` — odd closure pulls, flatness (`plot_veto_eff.py:92-96`),
  chain spread, period stability from the data skims' `period` column, COMB closure (A5-D9); writes
  `D/data/restart_2026-10-27/wp-a5/diagnostics.json`.
- `D/scripts/restart_2026-10-27/wp-a5/plot_wpa5.py` + `D/plots/restart_2026-10-27/wp-a5/captions.md` (style
  `gate_evidence/ge_style.py`): fig1 q per cell, even and odd, three detectors; fig2 δ_v(η bin); fig3 σ_rel per
  measurement bin with the 0.05 line, both frames.
- `D/scripts/restart_2026-10-27/wp-a5/tests_wpa5.py` — the named tests below as functions printing PASS/FAIL; exit 1 on
  any FAIL.
- Condor: `D/condor/restart_2026-10-27/wp-a5/veto_corr_hist.sub` (pattern
  `D/scripts/restart_2026-10-27/gate_evidence/veto_eff_hist.sub`: vanilla, `getenv = True`, 1 CPU, `+MyProject "h1"`,
  RedHat9; request_memory 2048, `+MaxRuntime 3600`) and `attach_cells.sub` (`queue label in 9012 9024 9033 9155 9157
  9158`, pattern `D/condor/restart_2026-10-27/wp-c/study/wpc_build.sub`); wrapper `wpa5_job.sh` prints `md5sum` of the
  script first (as `veto_eff_hist_job.sh`).
- Docs: `D/docs/restart_2026-10-27/wp-a5/freeze_record.md`, `g1pp_a_verdict.md`.

## Slices

1. **Correction derived and frozen (even), checked (odd).** Units: (a) `veto_corr_hist.py` + `.sub` + wrapper;
   (b) `derive_veto_corr.py`; (c) `tests_wpa5.py` (tests 1–5). Order: submit (a) via `h1-naf-operator`; run (b) `even`,
   write `freeze_record.md`; run (b) `odd`. Tests:
   `fig2_consistency: integrated Plug N−1 f_fail, odd, D0/D1/D2 (all η incl. other, all M_X) -> 0.2724/0.2996/0.3365
   within 1e-6 of summary.json`; `deep_gap_reproduced: Plug ALL odd Δ, 7071, η ∈ [−1,2.5), all M_X -> equals veto_eff.json
   deep_gap_eta_lt_2p5["N1_PLUG|odd|ALL"]["7071"]["delta_data_minus_full"] within 1e-6 (also n = 5755)`;
   `even_frozen_first: veto_correction.json even_sha256 == sha256(veto_correction_even.json) and freeze_record.md
   timestamp < odd-run log timestamp -> true`; `syst_floor: every cell sigma_q ≥ |q_even|/2 and ≥ spread -> true`;
   `no_control_1p5: grep of wp-a5 scripts for an η_max < 1.5 mask or "1.5" cut and for "remedy1"/"QBAR" -> no hit`.
   Checkpoint: `python3 tests_wpa5.py --slice 1` all PASS; `veto_correction.json` exists.
2. **Propagation table with the ≤ 0.05 flag.** Units: (a) `attach_cells.py` + `.sub`; (b) `veto_weight.py`;
   (c) `propagate_wpa5.py`. Tests: `join_keys: (run, event) unique and ec08 < 3.2 or sentinel for every attached row, all
   six labels -> true`; `zero_variant_identity: variant zero -> R == 0 in every bin and Σ_events w equals the skim e_w·s_diff
   sum per M_X bin`; `weight_total: weights() returns a finite value for every event incl. cell −1 and M_X outside
   [4,40] -> true`; `g1pp_a_table: 2 frames × 2 charges × 3 M_X × (6 + 4) bins = 120 rows with syst_veto_rel and
   masked_veto -> written`. Checkpoint: `tests_wpa5.py --slice 2` PASS; table exists.
3. **Diagnostics, plots, verdict.** Units: (a) `diagnostics_wpa5.py`; (b) `plot_wpa5.py` + captions; (c) verdict draft
   from the tables, then `h1-systematics` review of A5-D5 and `h1-analysis-critic` ruling appended. Tests:
   `period_table: 4 periods × 3 detectors × 2 parities f_data with χ²/ndf 3 -> written`; `comb_closure: f′ and f_data
   per split with |Δ|/σ -> written`; `verdict_sections: the eight A5-D11 sections present; every test row PASS/FAIL;
   the mask list per frame present -> true`. Checkpoint: `g1pp_a_verdict.md` exists with the ruling line.

## Verification

Re-run `derive_veto_corr.py --split odd` and `propagate_wpa5.py` from the stored hists: the regenerated
`veto_correction.json` (excluding `meta.timestamp`) and `g1pp_a_table.csv` are byte-identical to the stored ones;
`tests_wpa5.py` (all slices) PASS; the Condor tally (two clusters, wall × CPUs) is in the verdict's budget line; every
output path in the verdict exists (absolute paths).

## Risks

- The η-shape term dominates: δ_Plug ≈ +0.10/+0.19 at [2.5,3.0)/[3.0,3.2) on a signal fraction above 2.5 that is not
  small (parent Risks) may push high-x_F bins over 0.05 and mask them; the plan reports both forms (Open Q-B).
- D2 × [4,10) has ≈ 42 data events per parity: σ_stat 0.045 > |q|/2; a lucky small spread under-states σ_q (Open Q-A).
- The deep-gap f_MC,full depends on the fitted ND scales from poor fits (D0 7071 D/ndf 1290/87); negligible for Plug
  (f_MC ≈ 0.02) but 0.02–0.03 for FMD, whose MC fail rate is mostly ND/PDISS physics; the chain spread exposes it.
- The 9012 skim has 2503 events: R is stable (same events reweighted) but the cell/η composition per bin is noisy; the
  bootstrap column shows where.
- The WP-C frame is not frozen (C1); a frame other than A or B would need `propagate_wpa5.py` re-run, nothing else.
- Hadron-kinematics dependence of the veto loss within an (M_X, cell, η_max) cell is not measurable from event-level
  skims; η_max is the proxy, and this limit is stated in the verdict.

## Scope Notes

In: the WP-A5 block steps (1)–(6). Out: WP-A6; data or MC re-skimming; editing anything under `gate_evidence/`,
`wp-a*/`, `wp-c/`; the parent plan. User decisions `executor` must not reopen: D12 (efficiency, not energy shape), D13
(G1″ gating, ≤ 0.05 acceptance), D14 (correction + systematic per (M_X, cell), σ = max(|q|/2, odd/even spread), even
derives / odd checks, no η_max(0.8) < 1.5 control, no single q per period, no threshold scan, Plug veto kept), and the
parent Scope Notes. Memory: no `.supervisor/memory/` entry affects `sidis_highQ2/diffractive/`; the parent-side lesson
"correct per-bin uncertainties rather than presentation cuts"
(`.supervisor/memory/decisions/2026-10-26-low-stat-cut-proposal-dropped.md`) is why thin cells keep their own q (A5-D3).

## Assumptions

- The six `s<label>.npz` skims were built from the same merged files as the WP-A3 skims and with the same signed-v3
  table (check: `s<label>.json` `scale_to_data`/`exposure_factor` equal `composite_scale_table_v3_2026-10-20.csv`).
- `e_idx` indexes the full `DiffTot` row order of the input file (check: `attach_cells` invariant on `ec08` and the
  skim's `e_mx` equal to `DiffTot.mx[e_idx]`).
- The LCG_107a `python3` with numpy/uproot is available on the Condor nodes through `getenv = True` (check: the
  gate-evidence job ran so; the wrapper prints `python3 -c "import numpy, uproot"` first).
- `veto_eff.json` `deep_gap_eta_lt_2p5` was computed on η ∈ [−1,2.5), not including "other" (check: n = 5755 equals the
  odd Plug deep-gap count 3915 + 1439 + 401).

## Open Questions

Batched; none blocks execution (the plan runs with the recommended answer and reports the alternative).
- **Q-A (user:)** Should the data binomial σ_stat of q enter the per-cell systematic, σ_q = max(|q|/2, spread, σ_stat)?
  Only D2 × [4,10) is affected now. Recommendation: yes; it costs nothing elsewhere and guards against a small spread by
  chance. Until answered, the plan applies the D14 rule and reports σ_stat per cell.
- **Q-B (user:)** If the η-shape term alone exceeds 0.05 in a bin, promote q_η (η-stepped) to the nominal correction with
  half the step as systematic, or keep the constant nominal and mask? Recommendation: decide on the numbers; the verdict
  tabulates both forms per bin.

## Log

(empty; `fixer` appends)

## Todo

- [ ] S1a `veto_corr_hist.py`, `veto_corr_hist.sub`, `wpa5_job.sh`; submitted by `h1-naf-operator`; hists JSON written
- [ ] S1b `derive_veto_corr.py --split even`; `veto_correction_even.json`; `freeze_record.md` with sha256 and UTC time
- [ ] S1b `derive_veto_corr.py --split odd`; `veto_correction.json`
- [ ] S1c `tests_wpa5.py --slice 1`: fig2_consistency, deep_gap_reproduced, even_frozen_first, syst_floor, no_control_1p5 PASS
- [ ] S2a `attach_cells.py`, `attach_cells.sub`; six `cells_<label>.npz`
- [ ] S2b `veto_weight.py`
- [ ] S2c `propagate_wpa5.py`; `g1pp_a_table.{csv,json}`; `tests_wpa5.py --slice 2` PASS
- [ ] S3a `diagnostics_wpa5.py`; `diagnostics.json`
- [ ] S3b `plot_wpa5.py`; fig1–fig3 + `captions.md`
- [ ] S3c `g1pp_a_verdict.md` drafted; `h1-systematics` review; `h1-analysis-critic` ruling appended; `tests_wpa5.py` all PASS
- [ ] Verification re-run byte-identical; Condor tally recorded; Open Q-A/Q-B answers requested from the user

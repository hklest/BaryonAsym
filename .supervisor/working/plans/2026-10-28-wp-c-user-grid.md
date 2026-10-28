# Plan: WP-C C1 — freeze of the measurement grid on the user grid (840x cocktail)

Child plan of `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(P), package WP-C, slice C1 (P:702-746). Binding: the WP-C block, D6, D7, D15-D25 (P:140-280), Scope Notes (P:481-490),
wave 1f (P:430-434), and the user's words: "two bins in z and possibly 5 in pth … x_F > 0 and x_F < 0 … two bins in
diffractive beta … integrate over Q²". Abbreviations: `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`;
`UG` = `D/docs/restart_2026-10-27/wp-c/user_grid_2026-10-28/user_grid_stats.md`; `BR` = `D/docs/restart_2026-10-27/wp-c/binning_record.md`;
`S` = `D/scripts/restart_2026-10-27`; `W` = `D/data/restart_2026-10-27`; `C1` = `…/restart_2026-10-27/wp-c/c1/` under
`D/{scripts,data,plots,docs,condor}`.

## Goal

Freeze the C1 grid as numbers and definitions an outside reader can re-derive: hemisphere, β, z*, p_T*², charge, fiducial
statement; the per-cell p_T*² grid by the D19 rule on the 840x cocktail; per-bin purity/stability/mask with the WP-A6 edge-30
f_ND as stand-in and a declared re-fill from WP-A56′; the migration treatment across β and hemisphere; the 840x statistics,
data precision and the WP-A5 ≤ 5 % veto-systematic acceptance per bin; the cross-checks kept (M_X projection, frame B,
definition closure); a provisional hashed mask; `grid_freeze_record.md`.

Non-goals: C2 (true gen X frame confirmation, final mask freeze, P:730-733); unfolding or responses beyond the shape check;
any data read beyond the existing UG binning tables; selection changes (WP-B); the η_max fits and q re-derivation (WP-A56′);
parent edits; commits.

Expected change size: 8 new Python scripts under `S/wp-c/c1/` (≈ 700 lines total), 2 `.sub` files, tables/JSON under
`W/wp-c/c1/`, ≤ 6 figures, one record ≤ 150 lines plus a 5-line stub. No edit to any existing script.

## Approach

Reuse the UG machinery as it stands (`S/wp-c/user_grid_2026-10-28/ug_tables.py` for the cell/edge logic, `S/wp-c/study/wpc_lib.py`
skim schema, `S/wp-a5/veto_weight.py` weights, `S/wp-a6/purity_fill_wpa6.py` for the ND/Reggeon components,
`S/wp-c/mask/make_blind_mask.py` JSON schema) through a small C1 library that owns its own label list and loaders, so no
shared file is edited (WP-A56′ owns `wp-a5/` and the `LABELS` extension, `D/docs/restart_2026-10-27/wp-d2/adoption_840x_record.md:29`).
One Condor pass attaches D cells, `etaMaxClus08` and run/event numbers to the four 840x skims with the existing
`S/wp-a5/attach_cells.py` (it already writes `run`, `event`, `even`, `:35-37`), which gives the bootstrap-by-generated-event unit
without rebuilding skims. All multi-file table work runs as short Condor jobs (login-node policy, `CLAUDE.md`).

Rejected: rebuilding the 840x skims with event numbers (attach_cells gives them); re-reading data (the D19 edges and the
per-bin data counts exist in `W/wp-c/user_grid_2026-10-28/`, and the D15 selection is already applied there, `ug_tables.py:18`);
editing `wpc_lib.LABELS` or `propagate_wpa5.sample()` (read by four other scripts, and A56′ territory); a new binning
framework.

## Decisions

- C1-1 **Frame, axis, hemisphere (restated from D6/D21, operational).** Reco p_X = stored HFS 4-vector (frame A); X rest frame;
  axis u = −q̂* with q the eSigma photon boosted into that frame, which is the Pomeron direction (BR:23-27,
  `S/wp-c/study/xframe_obs.py:87-105`); x_F = 2 p*·u/M_X, proton-positive (D6; `.supervisor/memory/decisions/2026-10-24-xf-gamma-p-frame-not-lab.md`);
  p_T*² = |p* − (p*·u)u|² (BR:29). Hemisphere P if x_F > 0, else G (x_F = 0 is measure-zero; the rule is stated so the
  partition is deterministic). Gen frame until WP-E: the G2 proxy `pX_collinear(q_gen, P, mirMX)` (`S/wp-c/study/wpc_build.py:157-158`).
- C1-2 **z* = 2E*/M_X**, E* = √((x_F M/2)² + p_T*² + m²), m = m_π at reco and data, PDG at gen; M = `mxA` (mass of the stored
  HFS vector) at reco, `mirMX` at gen (`ug_tables.py:27-30` the formula, `:56` and `:81` the frame-A mass `mxA`, `:62` the gen
  mass). Bins [0, 0.2) and [0.2, ∞), the same split in all four cells
  (D18). The top bin is open: reco resolution can give z* > 1; the fraction is measured in slice 2 and reported, not cut, so
  the parent test `zstar_range: all tracks -> 0 < z* ≤ 1` (P:736) is applied exactly to gen tracks and as a reported overflow
  at reco. Default for Q9 = option (a); options (b)/(c) stay the user's (Open Questions).
- C1-3 **p_T*² in GeV², not p_T*.** Reason: D6/D16 observable; the quantile grid makes the variable choice immaterial for
  populations, and p_T*² keeps the parent's (1/N_D) dn/dp_T*² convention. Frozen edges = the frame-A data quantiles already in
  `W/wp-c/user_grid_2026-10-28/user_grid_summary.json` (`edges["('A', 'pt2s', 'q5')"]`, `q4`, `q3`; pooled over both
  hemispheres and both β bins, D19), as stored (3 decimals), last bin open: q5 0 / 0.039 / 0.096 / 0.205 / 0.494 / ∞,
  q4 0 / 0.051 / 0.141 / 0.386 / ∞, q3 0 / 0.074 / 0.268 / ∞ GeV². The UG headline values 0/0.042/0.103/0.222/0.547 are
  frame B (UG:14). Frame B is evaluated on the frame-A edges (a systematic must
  share the bins).
- C1-4 **β.** Reco and data: DiffTot `beta`; gen: Q²_t/(Q²_t + mirMX²) (UG:10). Split at 0.523, the full-sample data event
  median at the D15 selection (`ug_tables.py:119` gives 0.5235; frozen at 3 decimals), applied identically to data, reco MC
  and gen MC. Not a round 0.5: D20 fixed the median; the 1.5 % estimator offset and the +0.054 reco bias (BR event-level
  table, ≈ :110-125) are migrations the event-level β response absorbs. C1 reports the event-level purity and stability of
  each β bin on 840x (informational; expected ≳ 0.8).
- C1-5 **Charge.** h⁺: track `Charge` > 0, h⁻: < 0 (`ug_tables.py:149`; `Charge != 0` is a track cut, `ug_data_tracks.py:55`);
  separate tables, one grid per cell for both charges (D19).
- C1-6 **Fiducial statement for the record (lead; WP-B ratifies).** Q² > 150 GeV², 0.1 < y < 0.7 (eSigma), x_IP < 0.03,
  M_Y < 1.6 GeV, |t| < 1 GeV², 4 ≤ M_X ≤ 40 GeV, Q² and M_X integrated (D17); hadrons |η_lab| < 1.6, 0.15 ≤ p_T ≤ 15 GeV,
  π/K/p from X, V0 and conversion daughters excluded (BR:18-21, D6). η_max(0.8) < 3.0 (D15) is the selection, not part of the
  fiducial definition. y < 0.7 rather than A_pre's 0.9: every WP-C, WP-A5 and WP-A6 number, the UG edges, the resolutions and
  the veto correction are at 0.7 (UG:6). WP-B runs after C1 in wave 1f; if it moves y or x_IP, slice 2 is re-run on the new
  skims and the record goes v2; the grid structure and rule do not change.
- C1-7 **Grid rule = D19 with D7+ as the pass criterion (lead; user may override to plain D7).** Per (hemisphere, β) cell
  the p_T*² grid is the finest of {q5, q4, q3} whose every bin passes, for both charges, D7 (weighted N ≥ 50, purity ≥ 0.35,
  stability ≥ 0.35) and the D7+ extras as written (BR:222-244, D22): p_mig ≥ 0.40, f_fake ≤ 0.35, ≥ 100 raw MC tracks in every
  2× reco sub-bin, width ≥ 2σ_core (σ_core per cell from slice 2), with the old bin-count cap (≤ 4 p_T*² per M_X bin) replaced by
  D16's ≤ 5 per cell and 2 z* bins; plus the `h1-unfolder` additions from its consult (2026-10-28, read-only; none relaxes
  anything): raw matched 840x tracks ≥ 500 per gen bin and ≥ 20 per used response element (the weighted ≥ 50 is a luminosity
  statement, UG:24, not an MC-precision one; the engine carries no response-statistics error), inter-cell migration fraction
  per gen bin (matched reco landing in another hemisphere or β cell) < 0.5, and width ≥ 1.5 × (σ_core + |bias|) at the bin
  centre in addition to 2σ_core. Purity and stability are evaluated at gen granularity (the 2× reco pairs merged), as the
  pre-registered numbers were. Start value q4, q5 where it passes. Reason: D22 makes D7+ the floor for unfolded bins; choosing
  the grid by D7 alone and masking by D7+ would leave holes a coarser grid would not have. Expected outcome (frame A, UG:26):
  q4 in the photon-side high-β cell, q5 elsewhere if the extras hold; slice 2 decides.
- C1-8 **Photon-side high-β cell.** Same rule, no "z*-only" variant: if q3 also fails under the stand-in f_ND, the cell keeps q3
  with its failing bins masked and reported, and the z* measurement stands; re-evaluated at the A56′ re-fill (C1-13). The user
  asked for p_T bins; masked bins are shown, not removed.
- C1-9 **Mask rule under IBU (h1-unfolder consult applied).** D7+ with the C1-7 additions is the floor and nothing is
  relaxed (D22; the consult: a purity floor below 0.35 is not justified by the reco split, which improves conditioning, not
  the prior dependence of a 0.3-purity gen bin). The diagonal fraction per gen bin (purity on the 2× reco split) is a column
  of the table, informational. Two engine facts recorded for WP-H: the engine's `valid` mask only drops columns with P_j = 0
  or ε_j = 0 (`crosscheck_unfold_methods_v4.py:678`), so the D7+ mask must be applied by the response builder, with a masked gen
  column folded into the fake column F_i (`:56`), never zeroed; the quoted IBU error is diagonal through the frozen M_ji
  (`:711-722`), so pulls need a bootstrap covariance. Ruling: `h1-unfolder` rules on purity/stability and the response
  diagnostics, `h1-analysis-critic` on the frame definition (WP-C Lead line, P:715); a disagreement goes to the supervisor.
- C1-10 **Migration across hemisphere and β: one response per charge over the full grid, no |x_F| exclusion zone (lead;
  h1-unfolder concurs).** Cells = hemisphere × β inside the response (D22), so a hemisphere or β flip is a cell-to-cell
  migration the unfolding corrects with the neighbouring cells' data; as per-cell fake/efficiency corrections they would be
  pure MC ratios. Grounds: gen x_F piles at zero (median −0.03, BR:142-143) with σ_core 0.008-0.021 (BR:80-147); β has a one-way
  bias +0.054, σ 0.062 (BR:114); flipped tracks land in the same observable bin of the other cell (z* and p_T*² barely change
  at x_F ≈ 0). No |x_F| > x_min cut in the particle-level definition: it redefines the observable, discards ≈ 30-50 % of the
  sample at x_min ≈ 0.02-0.03, and z* ≥ |x_F| makes the loss fall on the low-z* bin. Slice 2 reports per gen bin and charge
  the flip fraction and the share with |x_F| < 0.02; the inter-cell migration < 0.5 criterion (C1-7) is the only gate. Handed
  to WP-H, not C1: the |x_F| > 0.02 (gen and reco) cross-check unfold (residual ≤ 0.5 σ_data in the common phase space) and,
  if a bin's flip fraction exceeds 0.30, a reco |x_F| < 0.02 sub-bin for its lowest observable bin instead of a reco
  hemisphere split.
- C1-11 **Weights of the 840x cocktail.** Signal 8402-8405 with `e_w` as skimmed (already w_period = L_data/L_eq from
  `W/wp-d2/scale_table_840x.csv`; test `weights_840x`), stream factor 1.0 (A56′ floats it; a global
  factor leaves purity, stability and p_mig unchanged and raises N_pred by +6-8 %, noted in the record); charm 9024/9033 and
  PDISS 9155/9157/9158 with their existing scales (`wpc_build.py:72-89`); 9012 excluded (D23 iv); the WP-A5 nominal veto weight
  `veto_weight.weights(mx, cell, ec08, "nominal")` with the q record `W/wp-a5/veto_correction.json` (the edge-30 re-ruling
  changed the propagation range, not q: `W/wp-a5/edge30/` holds only `g1pp_a_table_edge30.*`) and the fine D-cell scales
  exactly as `propagate_wpa5.sample()` applies them
  (`S/wp-a5/propagate_wpa5.py:38-49`); cells for 840x from slice 1.
- C1-12 **Gen-valid, purity, stability, fakes.** As `ug_tables.py:177-193` and `wpc_lib.pur_stab` define them: purity =
  N(reco and gen in the same (cell, bin))/N(all reco-selected tracks in the reco bin, fakes and backgrounds included);
  stability = N(same)/N(gen-valid tracks of the gen bin reconstructed anywhere); p_mig = N(same)/N(gen-valid reco tracks in
  the bin); f_fake = tracks failing `t_gv` (BR §3). The gen cell uses gen β and the G2 hemisphere.
- C1-13 **f_ND stand-in and the A56′ re-fill.** ND component per bin = the WP-A6 ND track template `W/wp-a6/trk/b7071.npz`
  (edge 3.0, WP-C schema) scaled per event reco-M_X bin so that its event share equals the largest-chain odd-split SB30 f_ND of
  that bin, 0.040 / 0.243 / 0.221 for [4,10) / [10,20) / [20,40] (`W/wp-a6/edge30/f_nd_per_mx_edge30.csv`,
  `g2_verdict_edge30.md:31-33`, D25 "largest chain"); Reggeon from `b6671/b6677.npz` as in `purity_fill_wpa6.py:117-121`;
  purity + f_mig + f_fake + f_ND + f_reg = 1 per bin (`g2_verdict.md:152`). Re-fill (slice 4b): when
  `W/wp-a56/f_nd_per_beta.csv` exists (largest chain per β bin, P:650-682), the fill is re-run with f_ND mapped by reco β bin and
  the A56′ `veto_correction_v2.json`, the grid rule re-applied, a new dated mask written; the grid handed to C2 is the
  post-re-fill one, and any per-cell change against the provisional grid is reported to the supervisor (the user approves the
  rule, not the numbers). If A56′ also delivers `purity_user_grid.csv`, C1 cross-checks it row by row (|Δpurity| ≤ 0.01) rather
  than replacing its own fill.
- C1-14 **Freeze protocol.** C1 freezes, as numbers in `grid_c1.json` with a sha256 sidecar and in the record: the definitions
  (C1-1…C1-5), the edges (β 0.523, z* 0.2, frame-A p_T*² quantiles), the per-cell grid choice. The provisional mask reuses the
  `make_blind_mask.py` JSON (`:17-21`; rule, source table and its sha256, created_utc, per-bin masked/reasons/marginal) with the
  bin key (charge, hemi, beta_bin, axis, bin) in place of (charge, mx_bin, axis, bin) and new fields `grid_sha256`,
  `f_nd_source`, `data_files_opened: []`. Honest statement in the record: the UG study tabulated per-bin data counts on the
  candidate grids before any mask (binning statistics; no unfolded or normalised quantity); C1 opens no data file. No even/odd
  split in C1: there is no data fit, the edges come from the full sample per D19, and the mask is MC-only; even/odd is the
  WP-A56′ protocol (D24). Every re-derivation writes a new dated JSON, never overwrites (BR §4). The final freeze with the true
  gen X is C2.
- C1-15 **Cross-checks kept.** (i) M_X projection: per-bin counts and purity of the frozen grid projected onto [4,10), [10,20),
  [20,40] as a continuity table with the G1″/G2 strata (D20), one figure. (ii) Frame B on the frozen edges: the full D7+ table
  and failure count in the same CSV (`frame` column), reported, never nominal (D21). (iii) Definition closure on 840x: boost
  closure (`S/wp-c/study/wpc_tests.py` pattern, < 1e-6), z* identity (from (x_F, p_T*², m) against 2E*/M from the boosted
  4-vector, < 1e-6), gen 0 < z* ≤ 1 exactly, −1 ≤ x_F ≤ 1 at gen, hemisphere partition G + P = all. (iv) z* and β resolution rows
  (bias, core σ, RMS per cell) added to the T3 table format (BR:80-147), which has no z* entry.
- C1-16 **Statistics and acceptance per bin (840x).** Columns per (frame, charge, hemi, beta_bin, axis, grid, bin): n_data
  (from the UG frame-A table, no new read), N_pred weighted, raw 840x reco signal tracks, min raw per 2× reco sub-bin, purity,
  stability, p_mig, f_fake, f_ND, f_reg, σ_MC(purity) from 200 bootstraps whose unit is the generated event (stream,
  EventNumber) so the twins 8402≡8405 and 8403≡8404 move together (D23 iii; `event` from the cells files), expected data
  precision δ_rel = 1/√n_data (pre-unfolding figure; the IBU inflation is WP-H's), and the WP-A5 veto systematic σ_rel per bin
  from the up/down/eta variants of `veto_weight.weights` on the normalised per-cell spectrum, the `propagate_wpa5.spectra`
  recipe (`:55`) with the C1 bin index; σ_rel > 0.05 is a mask reason `veto_syst` (acceptance per measurement bin,
  `D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md:8-19`).
- C1-17 **Outputs and record.** Everything under `C1`; `.sub` files under `D/condor/restart_2026-10-27/wp-c/c1/`. The record
  `D/docs/restart_2026-10-27/wp-c/c1/grid_freeze_record.md` (≤ 150 lines) holds: definitions, frozen edges, the per-cell grid
  table, the D7+ summary with the full CSV linked, mask file and sha256, flip fractions and |x_F| < 0.02 shares, β-bin purity,
  resolutions, cross-checks, the statistics/precision/veto table, re-fill status, open items. A 5-line stub
  `D/docs/restart_2026-10-27/wp-c/user_grid_record.md` points to it, because P:744 and the Todo P:1099 name that path.
- C1-18 **Budget ≤ 0.03 Condor-days:** 4 `attach_cells` jobs (minutes each, the WP-D2 skims took ≈ 10 CPU-min in total,
  adoption record :30), 1 tables job, 1 statistics job. Login node: single-file tests and the record only.
- C1-19 **Reco layout and closure placement (h1-unfolder consult).** C1 freezes the gen grid of C1-1…C1-8 only. The reco
  layout stays D22 as written for the `engine_shape_c1` shape check (reco split 2× along the observable axis, 8 → 16 z*,
  ≤ 20 → 40 p_T*² per charge); the consult's recommendation to split reco β 2× as well (at 0.523 ± σ_β, track matrices and the
  event-level N_D response 2 gen × 4 reco, so the refold keeps a degree of freedom) goes to the supervisor as Q-C1-1 with
  σ_β(840x) from slice 2 as evidence. The consult's closure suite (technical, half-sample by generated event,
  reweighted-truth slopes, iteration scan with k_sel ≤ 4, 9012-as-pseudo-data model closure, ≥ 300-toy data-luminosity
  bootstrap) conditions the final freeze; it runs where the parent builds responses (C2/WP-H, Q-C1-2), not in C1, and the
  record lists it as handed over.

## Outline

`S/wp-c/c1/` (new; pattern: the WP-A6 scripts, plain scripts with `argparse`, tests as a plain script with named checks,
`S/wp-a6/tests_wpa6.py`):

- `c1_lib.py`: `LABELS_C1 = [8402, 8403, 8404, 8405, 9024, 9033, 9155, 9157, 9158]`; `BETA_SPLIT = 0.523`,
  `ZSTAR_SPLIT = 0.2`, `PT2_EDGES = {"q5": [...], "q4": [...], "q3": [...]}` read once from `user_grid_summary.json` frame A and
  then frozen as literals (test `edges_frame_A`); `load_c1(prefix="s") -> (T, E)` (the 10-line twin of `wpc_lib.load`, `:12`,
  with `t_samp`, `e_samp`, `t_eglob`); `sample_c1() -> (T, E, cell, ec, w0, g, emb, event, stream)` mirroring
  `propagate_wpa5.sample()` (`:38-49`) with the C1 cells directory; `zstar(xf, pt2, M, m)`; `hemi(xf) -> {0: G, 1: P}`;
  `cell(hemi, beta_bin) -> 0..3`; `bin_index(cell, axis, grid, value) -> flat index` (gen and 2× reco variants);
  `d7plus_row(row) -> (passes, reasons)`; `grid_rule(table, frame) -> {cell: "q5"|"q4"|"q3"}`. Gotcha: `attach_cells.py:29`
  checks `etaMaxClus08 < 3.2` and `:35` exits on failure; the 840x skims were built by `wpc_build.py` with the same pre-cut
  (`:100-102`), so it holds.
- `c1_tables.py --frame A|B --out W/wp-c/c1/`: writes `binstats_c1_<frame>.csv/json` (UG columns, `user_grid_tables.csv`
  header, plus `p_mig, f_fake, f_nd, f_reg, n_raw_840x, n_raw_subbin_min, n_raw_element_min, intercell_mig, diag_frac,
  sigma_core, bias, width_ok, pass_D7plus, reasons`), `cells_c1.json` (per cell: n_data events, N_pred, β-bin event
  purity/stability, flip fraction per gen bin, |x_F| < 0.02 share), `grid_c1.json` + `.sha256` (edges, per-cell grid, frame,
  rule string, inputs' sha256). ND/Reggeon fill: follow `purity_fill_wpa6.py:118-120` (ntot and fractions) and `:168-169`
  (template weights) with the per-M_X factor of C1-13.
- `c1_mask.py <binstats> <grid_c1.json> <out.json>`: the mask per C1-14; sidecar as `make_blind_mask.py` writes it.
- `c1_stats.py`: bootstraps (200, unit (stream, EventNumber)), `precision_c1.csv`, `veto_syst_c1.csv` (variants nominal, up,
  down, eta, eta_up, eta_down per `veto_weight.py:57`), `boot_c1.json`.
- `c1_resolution.py`: z*, β, x_F, p_T*² per cell (bias, core σ, RMS, matched tracks, G2 gen), `resolution_c1.csv`;
  `mx_projection_c1.csv`. The `engine_shape_c1` toy uses the engine `S/wp-c/engine_shape/engine_shape_test.py:5` imports
  (`H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`,
  `derive_response_blocks` `:516`, `ibu_unfold` `:623`).
- `c1_plots.py`: ≤ 6 figures under `D/plots/restart_2026-10-27/wp-c/c1/` with captions in `captions.md`.
- `tests_c1.py slice1|slice2|slice3|slice4|all`.
- `D/condor/restart_2026-10-27/wp-c/c1/attach_cells_c1.sub` (queue label in 8402 8403 8404 8405; input
  `D/data/t2_detector/mc840x_merged_2026-10-28/<label>.root`; out `W/wp-c/c1/cells/cells_<label>.npz`; resources as
  `D/condor/restart_2026-10-27/wp-c/study/wpc_build.sub`) and `c1_run.sub` (one job: tables A and B, mask, stats, resolution,
  plots, tests).

Data shapes: `grid_c1.json = {frame, rule, beta_split, zstar_split, pt2_edges{q5,q4,q3}, cells{"G_lo","G_hi","P_lo","P_hi":
{"pt2_grid": "q4", "zstar": [0, 0.2, null]}}, inputs_sha256{...}, created_utc}`. Mask bin: `{charge, hemi, beta_bin, axis,
grid, bin, lo, hi, n_sig_pred, purity, stability, p_mig, f_fake, f_nd, veto_sigma_rel, masked, reasons[], marginal}`.

Invariants: `t_w == e_w[t_eglob]` (`propagate_wpa5.py:45`); the per-cell grid is identical for h⁺ and h⁻; the gen bin index
uses gen β and the G2 hemisphere; frame B rows use the frame-A edges; no C1 script opens a data ROOT file or a data skim other
than reading `n_data` from `user_grid_tables.csv`.

## Slices

1. **840x cells and the C1 library** (units: `attach_cells_c1.sub` + run; `c1_lib.py`, `tests_c1.py slice1`). Delivers
   `W/wp-c/c1/cells/cells_840{2,3,4,5}.npz` with their check JSON, and the loader. Tests:
   `cells_840x: four check lines -> ec08_ok, run_event_unique, mx_matches_skim all true`;
   `weights_840x: e_w/(weight1·weight2) of s8402 -> 0.132419 for all events (scale_table_840x.csv)`;
   `labels_c1: LABELS_C1 -> contains 8402-8405, no 9012` (parent test `nine012_absent`);
   `edges_frame_A: PT2_EDGES literals -> equal summary.json ('A','pt2s',q5/q4/q3) to 3 decimals`;
   `zstar_identity: 10⁴ 840x tracks, z* from (x_F, p_T*², m_π) vs 2E*/M from the boosted 4-vector -> |Δ| < 1e-6`;
   `boost_closure: p_X in its own frame -> (M_X, 0, 0, 0) within 1e-6`.
   Checkpoint: `python3 tests_c1.py slice1` passes on the login node (single-file reads only).
2. **Per-bin tables, grid rule, provisional mask** (unit: `c1_tables.py`, `c1_mask.py`, `c1_resolution.py` in `c1_run.sub`).
   Delivers `binstats_c1_A.csv/json`, `binstats_c1_B.csv`, `cells_c1.json`, `grid_c1.json(.sha256)`,
   `mask_c1_<date>_standin.json(.sha256)`, `resolution_c1.csv`, `mx_projection_c1.csv`. Tests:
   `beta_split_reproduced: summary.json data median -> 0.5235 → frozen 0.523; MC unweighted median reported`;
   `grid_rule_applied: per cell the chosen grid is the finest of q5/q4/q3 with no D7+ failure in either charge -> true`;
   `same_grid_both_charges: grid_c1.json -> one grid per cell`;
   `fractions_sum: purity + f_mig + f_fake + f_nd + f_reg -> 1 ± 1e-6 every row`;
   `purity_source: f_nd column filled from f_nd_per_mx_edge30.csv largest chain odd SB30, mapped by reco M_X -> true`;
   `hemisphere_partition: n(G) + n(P) -> n(all tracks) per sample and frame`;
   `zstar_range: gen tracks -> 0 < z* ≤ 1 exactly; reco fraction z* > 1 -> written to cells_c1.json and the record (no bound)`;
   `xf_range: gen tracks -> −1 ≤ x_F ≤ 1`;
   `flip_fraction_reported: per gen bin and charge -> in cells_c1.json; any > 0.30 -> flagged in the record for WP-H (C1-10)`;
   `intercell_migration: per gen bin -> < 0.5 for every unmasked bin, else a D7+ reason`;
   `mask_hashed: sidecar sha256 -> matches the JSON; data_files_opened == []`;
   `engine_shape_c1: toy response of the C1 shapes (gen 8 z* + Σ_cells n_pt2, reco 2×) through eng.ibu_unfold k=4 ->
   closure < 1e-3 (pattern S/wp-c/engine_shape/engine_shape_test.py)`.
   Checkpoint: the files above exist; `python3 tests_c1.py slice2` passes (run inside `c1_run.sub`, log in the condor dir).
3. **Statistics, precision, veto systematic, figures** (units: `c1_stats.py`; `c1_plots.py`; same job or a second
   `c1_run.sub` queue). Delivers `boot_c1.json`, `precision_c1.csv`, `veto_syst_c1.csv`, figures and `captions.md`. Tests:
   `boot_unit: resampling unit (stream, EventNumber); twin events share one draw -> true`;
   `boot_count: 200 replicas -> σ_MC(purity), σ_MC(stability) per bin written`;
   `mc_stat_floor: raw matched 840x per gen bin ≥ 500 and per used response element ≥ 20 -> true for every unmasked bin`;
   `veto_syst_table: σ_rel per (charge, hemi, beta_bin, axis, bin) for the frozen grid -> written; rows > 0.05 listed and
   carried into the mask as reason veto_syst`;
   `precision_table: δ_rel = 1/√n_data per bin from the UG frame-A n_data -> written, no data file opened`;
   `plots_complete: ≤ 6 figures (per-cell grid with purity, flip fractions, M_X projection, frame A vs B failures, precision
   per bin, β-bin purity) -> exist with captions`.
   Checkpoint: `python3 tests_c1.py slice3` passes.
4. **Record, stub, and the A56′ re-fill** (units: `grid_freeze_record.md` + stub now (4a); `c1_tables.py --refill` when
   `W/wp-a56/f_nd_per_beta.csv` exists (4b, a second `executor` dispatch after WP-A56′)). Tests (4a):
   `record_complete: every C1-17 section present; frozen edges and per-cell grid quoted; mask sha256 quoted; the stub
   user_grid_record.md names grid_freeze_record.md`.
   Tests (4b): `refill_inputs: f_nd_per_beta.csv rows per β bin and chain -> largest chain picked`;
   `refill_new_mask: mask_c1_<date>_a56.json written; the standin JSON sha256 unchanged`;
   `grid_change_reported: per-cell grid v1 vs v2 -> diff in record §re-fill (empty allowed)`;
   `a56_purity_crosscheck: if purity_user_grid.csv exists -> |Δpurity| ≤ 0.01 per bin or the rows listed`.
   Checkpoint: `python3 tests_c1.py slice4` passes; record on disk; the supervisor is told of any grid change.

## Verification

`python3 S/wp-c/c1/tests_c1.py all` passes; `grid_freeze_record.md` quotes the frozen edges, the per-cell grid, the mask file
and sha256, the flip fractions, the β-bin purities, the resolution rows and the precision/veto table; the parent's C1 tests
(`zstar_range` at gen per C1-2, `beta_split_reproduced`, `grid_rule_applied`, `nine012_absent`, `purity_source`, P:735-738)
all map to a slice test above; no data ROOT file or data skim was opened by any C1 script (`data_files_opened == []`); `h1-unfolder` has read the
record's purity/stability table (incl. `diag_frac`) and `h1-analysis-critic` the frame definition (WP-C Lead line, P:715).

## Risks

- The stand-in f_ND at [10,20) and [20,40] (0.243, 0.221) can push low-β bins below 0.35, so the provisional grid may be
  coarser than the post-A56′ one (C1-13 re-fill).
- The G2 proxy gen frame is not the true X frame; C2 recomputes purities and may change failures (the parent reports instead of
  flipping frames, D21).
- The UG per-bin data counts exist before the mask (binning statistics only); stated in the record, not hidden.
- Fine D-cell scales and q come from 9012-era fits applied to 840x; A56′ re-derives both (slice 4b re-fill).
- The flip fraction near x_F ≈ 0 has never been measured; a bin above 0.30 is flagged for WP-H's reco sub-bin (C1-10).
- `attach_cells.py` imports `wp-a2/n1_wpa2.py` for the D cells and asserts ec08 < 3.2; a WP-A56′ edit to those files during
  wave 1f would change C1's cells silently: the record stores the sha256 of both.
- Per-cell grids with different bin counts give a block response with unequal blocks; `engine_shape_c1` checks the engine
  accepts it.
- Twins 8402≡8405 and 8403≡8404 share event numbers; the bootstrap unit is (stream, EventNumber), so a draw moves both
  periods of a stream together and never pairs across streams (`boot_unit`).

## Scope Notes

In: everything in `## Slices`. Out: C2; unfolding beyond the shape check; data reads; WP-B selection changes; WP-A56′ fits,
q re-derivation and the `LABELS`/`propagate_wpa5` extension; Reggeon/ND template changes; parent edits; memory edits
(curator). User decisions `executor` must not reopen: the user grid of D16 (two z bins, ≤ 5 p_T bins, hemispheres, two β bins,
Q² integrated); the D15 edge 3.0; x_F proton-positive in the γ*p/X frame
(`.supervisor/memory/decisions/2026-10-24-xf-gamma-p-frame-not-lab.md`); 840x as signal MC and 9012 out of the mixture (D23,
Q13 pending with the user). Lead decisions the user may override: C1-6 (y < 0.7 as the fiducial statement), C1-7 (D7+ as the
grid-rule criterion), C1-10 (no exclusion zone), besides D18/D21 from the parent. Workspace rules per `CLAUDE.md`.

## Assumptions

- The 840x skims under `W/wp-c/study/s840{2,3,4,5}.npz` keep the 9012 schema and `e_w` already carries w_period (checked:
  `weights_840x`, slice 1).
- `attach_cells.py --out` accepts a path outside `W/wp-a5/` and its `WPC` constant points at the directory holding the 840x
  skims (checked by the first Condor job's check line).
- The WP-A6 ND/Reggeon track templates `b7071/b6671/b6677.npz` carry frame-A quantities (`xfA`, `pt2A`, `mxA`) like the WP-C
  skims (checked: `fractions_sum` cannot be built otherwise; slice 2 first step prints their keys).
- `eng.ibu_unfold` accepts the unequal-block C1 shapes (checked: `engine_shape_c1`).
- `W/wp-a56/f_nd_per_beta.csv` will carry columns beta_bin, chain, f_nd (P:650-682 names the file; columns are read by name,
  slice 4b fails loudly otherwise).

## Open Questions

- **Q9 (user; default applied).** z definition: (a) z* = 2E*/M_X in the X rest frame, split at 0.2 (planned, D18, C1-2);
  (b) z* split at the per-hemisphere data median (fails P high-β low-z*, purity 0.33, UG:18,26); (c) another z the user meant.
  Recommended: (a). Non-blocking: slices 1-3 run with (a); (b) is one constant.
- **Q-C1-1 (supervisor, not the user): parent D22 reco layout.** The `h1-unfolder` consult recommends splitting reco β 2× as
  well (8 → 32 and ≤ 20 → 80 per charge, N_D 2 × 4), against D22's "observable axis only" (8 → 16, ≤ 20 → 40), because the
  +0.054 β bias makes near-edge events a pure MC-prior correction otherwise. The gen grid is unaffected. Recommended: amend D22
  accordingly when C2/WP-H builds the responses, with σ_β(840x) from slice 2 as the edge evidence; a lead decision, no user
  return needed.
- **Q-C1-3 (supervisor): parent D19 pass criterion.** D19 says "passes D7"; C1-7 uses D7+ (D22) plus the consult's
  MC-statistics, inter-cell-migration and bias-width criteria, which can coarsen a cell below the user's "possibly 5".
  Recommended: amend D19 to the C1-7 rule; the `reasons` column of `binstats_c1` shows which criterion failed each bin, so the
  plain-D7 grid stays derivable from the same table.
- **Q-C1-2 (supervisor): closure suite placement.** The consult makes the reweighted-truth closures, the k-scan, the
  9012-pseudo-data model closure and the ≥ 300-toy data-luminosity bootstrap conditions of the freeze; the parent places the
  freeze at C2 (wave 3′) and the unfolding at WP-H (wave 4′). Recommended: C2 runs them on the v6dif2 responses before hashing
  the mask (budget to be set in the C2 child plan); C1 runs only the cheap subset (C1-19).

## Log

## Todo

- [ ] Slice 1: `attach_cells_c1.sub` run (4 jobs), check JSON, `c1_lib.py`, `tests_c1.py slice1` PASS
- [ ] Slice 2: `c1_tables.py` A and B, `c1_mask.py`, `c1_resolution.py`; `grid_c1.json` + sha256; stand-in mask + sha256;
      `tests_c1.py slice2` PASS; `engine_shape_c1` PASS
- [ ] Slice 3: `c1_stats.py` (bootstraps, precision, veto σ_rel), `c1_plots.py`; `tests_c1.py slice3` PASS
- [ ] Slice 4a: `grid_freeze_record.md` and the `user_grid_record.md` stub; `h1-unfolder` and `h1-analysis-critic` reviews
      recorded
- [ ] Slice 4b (after WP-A56′): re-fill, dated mask v2, record §re-fill, grid change reported to the supervisor
- [ ] Q9 confirmed by the user; C1-6/C1-7/C1-10 overrides, if any, applied as one-constant changes

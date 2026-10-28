# WP-C C1 grid freeze record (2026-10-28, provisional until C2)

`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`, `W` = `D/data/restart_2026-10-27/wp-c/c1`, `S` = `D/scripts/restart_2026-10-27/wp-c/c1`.
Plan: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-28-wp-c-user-grid.md` (C1-1..C1-19). Cocktail: 840x signal 8402-8405
(`e_w` = w_period) + charm 9024/9033 + PDISS 9155/9157/9158, WP-A5 nominal veto weight, 9012 excluded. No data file opened (`data_files_opened == []`);
the per-bin data counts are the existing UG frame-A binning counts (`D/docs/restart_2026-10-27/wp-c/user_grid_2026-10-28/user_grid_stats.md`),
tabulated before any mask. Binning statistics only, no unfolded or normalised quantity.

## 1. Definitions (frozen, `W/grid_c1.json`, sha256 `8458fa4d...5f39`)
- Frame A: stored HFS 4-vector p_X, X rest frame, axis u = -q-hat* (photon boosted in), x_F = 2 p*.u/M_X proton-positive, p_T*^2 = |p* - (p*.u)u|^2 (D6/D21).
- Hemisphere: P if x_F > 0, else G (x_F = 0 goes to G). Gen frame until WP-E/C2: the G2 proxy `pX_collinear(q_gen, P, mirMX)`.
- z* = 2E*/M_X, E* = sqrt((x_F M/2)^2 + p_T*^2 + m^2), m = m_pi (reco, data), PDG (gen), M = `mxA` (reco), `mirMX` (gen). Bins [0, 0.2) and [0.2, inf). Q9 option (a) applied.
- beta: DiffTot `beta` (reco), Q2_t/(Q2_t + mirMX^2) (gen); split 0.523 (data median 0.52347, D20); MC unweighted median 0.5549 (reported).
- Charge: h+ / h- separate tables, one grid per cell for both charges. Cells: G_lo, G_hi, P_lo, P_hi = hemisphere x beta bin.
- Fiducial (lead statement, WP-B ratifies): Q2 > 150, 0.1 < y < 0.7 (eSigma), x_IP < 0.03, M_Y < 1.6, |t| < 1, 4 <= M_X <= 40, Q2 and M_X integrated; hadrons |eta_lab| < 1.6, 0.15 <= p_T <= 15 GeV, V0/conversion daughters excluded; eta_max(0.8) < 3.0 is the selection (D15).
- p_T*^2 unit: GeV^2, observable (1/N_D) dn/dp_T*^2.

## 2. Frozen edges (frame-A data quantiles from `user_grid_summary.json`, 3 decimals, last bin open)
| grid | edges (GeV^2) |
|---|---|
| q5 | 0 / 0.039 / 0.096 / 0.205 / 0.494 / inf |
| q4 | 0 / 0.051 / 0.141 / 0.386 / inf |
| q3 | 0 / 0.074 / 0.268 / inf |
Frame B is evaluated on the same frame-A edges. Reco 2x sub-bins (C2/WP-H layout): midpoints of finite bins, open top bins split at the weighted MC median
(z* 0.294, q5 0.843, q4 0.686, q3 0.517 GeV^2; constants in `grid_c1.json`).

## 3. Per-cell grid (rule C1-7: finest of q5/q4/q3 with every p_T*^2 bin passing D7+ and the MC-statistics, inter-cell and width criteria, both charges)
| cell | z* bins | p_T*^2 grid | failing bins q5 / q4 / q3 (h+, h-) | reasons |
|---|---|---|---|---|
| G_lo (xF<0, beta<0.523) | [0,0.2) [0.2,inf) | q3 (passes) | 5,3 / 1,1 / 0,0 | purity<0.35 only |
| G_hi (xF<0, beta>=0.523) | same | q3 (3 of 3 masked) | 5,5 / 4,4 / 3,3 | purity<0.35 (0.29-0.34) |
| P_lo (xF>0, beta<0.523) | same | q3 (passes) | 1,1 / 1,1 / 0,0 | purity<0.35 |
| P_hi (xF>0, beta>=0.523) | same | q3 (bin 0 masked) | 4,3 / 1,1 / 1,1 | purity<0.35 (0.336-0.339) |
No cell passes q5 or q4 in full; the `reasons` column of `W/binstats_c1_A.csv` (all three grids, both charges, frame A and B) keeps the plain-D7 grid derivable.
The only failure reason on any bin of any grid is purity < 0.35 (one p_mig < 0.40 in G_hi q5): the stand-in f_ND (0.040/0.243/0.221 per reco M_X, odd SB30
largest chain, `W/wp-a6/edge30/f_nd_per_mx_edge30.csv`; ND template `b7071`, Reggeon `b6671/b6677`) is the lever, hence the re-fill (section 9).
Consequence for the user's "possibly 5 in pT": q5 does not hold on the stand-in; the choice is q3 with failing bins masked (C1-8), re-decided at the re-fill.

## 4. D7+ summary (frozen grid, frame A; 40 bins = 2 charges x 4 cells x (2 z* + 3 p_T*^2))
Mask `W/mask_c1_2026-10-28_standin.json` (sha256 `cdba7f243a46b150d69533bc10c35de94db503e8065b45aa8e1da695fa2dcddd`; the pre-veto twin `..._standin_d7plus.json`,
sha256 `d56d6cc1...7ccd`): 9 of 40 masked, 17 marginal (purity or stability < 0.40), all masks purity<0.35: G_hi p_T*^2 (3 bins x 2 charges), P_hi p_T*^2 bin 0 (x2),
P_lo z* bin 1 h+ (0.347). Unmasked bins: raw 840x gen 1131-32122, raw per used response element >= 70, purity 0.35-0.46, stability 0.56-0.88,
diag_frac 0.47-0.74, intercell 0.05-0.29. Frame B on the frame-A edges: 12 failing bins (all purity), reported, never nominal (D21).
Bins failing C1-7 summary: p_T*^2 G_hi bins 0-2 (h+: 0.326/0.320/0.297, h-: 0.338/0.325/0.322), P_hi bin 0 (0.336/0.339), P_lo z* bin 1 h+.

## 5. Flip fractions (gen bin, frozen grid, h+; `W/cells_c1.json` flip_by_cell_gen_bin)
- Hemisphere flip 0-0.077 (max in z* bin 0 of G_hi/G_lo); beta flip 0.04-0.29 (beta < 0.523 cells 0.17-0.29, beta >= 0.523 cells 0.04-0.06); either < 0.30 everywhere,
  so no bin exceeds the 0.30 WP-H flag. Share with gen |x_F| < 0.02: 0.00-0.55 (max P_lo p_T*^2 bin 0 = 0.55; z* bin 1 about 0.01).
- No |x_F| exclusion zone (C1-10); the |x_F| > 0.02 cross-check unfold stays with WP-H.

## 6. beta-bin purity (event level, cocktail signal, informational)
purity / stability: beta < 0.523 0.712 / 0.737; beta >= 0.523 0.632 / 0.954 (plan expected >= 0.8; fakes and non-fiducial events are in the purity denominator).
beta resolution (reco - gen): bias +0.065 / +0.043, sigma_core 0.065 / 0.061, RMS 0.086 / 0.089 (lo / hi gen bin); all +0.054, 0.063.

## 7. Resolutions (frame A, matched gen-valid tracks, per gen cell; `W/resolution_c1.csv`)
| observable | G_lo | G_hi | P_lo | P_hi |
|---|---|---|---|---|
| z* sigma_core (bias) | 0.0098 (+0.006) | 0.016 (+0.007) | 0.011 (+0.011) | 0.024 (+0.012) |
| x_F sigma_core (bias) | 0.0092 (-0.001) | 0.015 (-0.001) | 0.011 (+0.009) | 0.022 (+0.010) |
| p_T*^2 sigma_core (bias) GeV^2 | 0.012 (+0.0007) | 0.018 (+0.001) | 0.017 (-0.0002) | 0.016 (-0.0003) |
The width criteria (2 sigma_core and 1.5 (sigma_core + |bias|)) pass for every bin of every grid; sigma_core is per gen bin in the table.

## 8. Cross-checks and statistics
- M_X projection (`W/mx_projection_c1.csv`, G2 and G1 gen frames agree to 0.001): bin purities at M_X [4,10) 0.45-0.60, [10,20) 0.23-0.50 (median 0.35), [20,40] 0.14-0.41 (median 0.26): the grid purity is carried by the low M_X classes and is poor at high M_X.
- Definition closure: boost 4e-14, z* identity 4e-15 (synthetic 4-vectors through `xframe_obs`; the skims carry no 4-vectors); hemisphere partition G + P = all (no undefined x_F);
  gen z* in [0.009, 1.115] and |x_F| max 1.069: one gen track of 262949 exceeds 1 in each (G2 proxy artefact; the parent's exact 0 < z* <= 1 test is not met by the proxy, C2 re-tests with the true gen X). Reco z* > 1: 0.00-0.07 % per cell (frame A).
- Statistics: bootstraps (200, unit (stream, EventNumber); 70464 units of 91552 events; twins share a draw; templates fixed): sigma_MC(purity) <= 0.013, sigma_MC(stability) <= 0.019 (`W/boot_c1.json`).
- Data precision 1/sqrt(n_data) per bin 0.0081-0.0388 (n_data 663-15140; `W/precision_c1.csv`). WP-A5 veto sigma_rel per bin max 0.030 (`W/veto_syst_c1.csv`); none exceeds 0.05, no `veto_syst` reason.
- Engine shape: toy response 20 gen x 40 reco (unequal blocks) through `ibu_unfold` k = 4 closes to 2e-16.

## 9. Re-fill status (slice 4b, NOT done)
Waits for WP-A56' `f_nd_per_beta.csv` (largest chain per beta bin) and `veto_correction_v2.json`; then the fill is re-run, grid rule re-applied, a new dated mask written, per-cell grid change reported
to the supervisor. The stand-in mask is not overwritten. Files hashed in `grid_c1.json` inputs_sha256 include `attach_cells.py` and `n1_wpa2.py` (WP-A56' edits to them would change the cells).

## 10. Specialist rulings
- h1-unfolder (consult 2026-10-28, read-only, no BLOCKING): keep purity >= 0.35 with ND/Reggeon in the denominator; 0.35-0.40 bins stay unmasked but marginal, re-evaluated at the re-fill;
  freeze q3 everywhere under the pre-registered rule (no hand-picked finer grid), with the q4/q5 pass/fail tables kept (section 3); masked bins fold into the fake column (C1-9);
  recommends accepting the reco beta 2x split now (event beta purity 0.63-0.71, flip 0.29); raw 840x counts adequate, add a response-statistics bootstrap and a beta/hemisphere prior-reweighting closure at C2/WP-H.
  Pass/fail is stand-in-dependent, not final. The reasons columns were read from the briefing numbers; the unfolder did not open the CSVs.
- h1-analysis-critic (consult, read-only, CONCERN, no BLOCKING): definitions coherent; u = -q-hat* in the X rest frame is the gamma*-IP (Pomeron) direction, not literally the gamma*p frame, so the record states it and the user should confirm the X-frame reading;
  x_F = 0 -> G and no exclusion zone defensible, P/G in the lowest z* bin anti-correlate through flips, so WP-H needs a bootstrap covariance; gen median x_F = -0.03 may be a G2-proxy artefact (check at C2);
  z* uses m_pi for all hadrons at reco (PDG at gen), a migration source near the 0.2 split; standard z = P.Ph/P.q is an alternative to offer to the user, not implemented; gen beta omits -t;
  the proxy ignores |t| (tilt 0.1-0.25 rad at low M_X), so sigma_core likely understates low-M_X, low-p_T*^2 migration; label the grid provisional until C2 and compare true gen X vs proxy per M_X bin there.

## 11. Open items
- Q9 (user): z* split 0.2 default applied; alternatives (b) per-hemisphere data median, (c) another z are one constant (`ZSTAR_SPLIT`).
- Lead decisions the user may override: C1-6 (y < 0.7), C1-7 (D7+ criterion; supervisor ruling Q-C1-3 accepted), C1-10 (no exclusion zone).
- Reco beta split 2x (Q-C1-1, accepted) and the unfolder closure suite (Q-C1-2, accepted) go to C2/WP-H before the mask hash; the final freeze with the true gen X is C2.
- Sub-bin and "used response element" definitions (lead choices): top-bin sub-split = weighted MC median; element used when >= 5 % of its gen-bin column.
- If WP-B moves y or x_IP, re-run slice 2 on the new skims (record v2).

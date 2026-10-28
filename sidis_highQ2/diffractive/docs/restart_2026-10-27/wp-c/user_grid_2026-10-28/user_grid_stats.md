# User-grid statistics (2026-10-28): hemisphere x beta(2) x z(2) and p_T(5)

`D`=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive. Tables: `D/data/restart_2026-10-27/wp-c/user_grid_2026-10-28/user_grid_tables.{csv,json}` (1632 rows: frame x variant x hemisphere x beta x bin x charge h+/h-/sum), `user_grid_events.csv`, `user_grid_summary.json` (edges, medians, factors). Scripts: `D/scripts/restart_2026-10-27/wp-c/user_grid_2026-10-28/` (`ug_data_tracks.py` + `ug_data.sub`, Condor cluster 5958315, 4 jobs, ~4 s each; `ug_tables.py`; `ug_plot.py`). Figure: `D/plots/restart_2026-10-27/wp-c/user_grid_2026-10-28/fig_user_grid_counts.png`.

## Definitions
- Selection: WP-C study selection (Q2es>150, 0.1<Yes<0.7, 35<Empz<70, |PVz|<35, FMD, Plug<7, FTS, release track cuts) plus eta_max(0.8)<3.0 (D15), x_IP<0.03, 4<=M_X<=40. Not the cjc `A_pre` (Ys<0.9) base. Data: certified runs, 4 files, 15,296 events, 104,459 tracks. Signal: 9012+9024+9033+9155/7/8 with `e_w` x G1'(c) cell scale x WP-A5 nominal veto weight (`propagate_wpa5.sample()`, `veto_weight.weights`); 9012 raw = 1,767 events.
- Hemisphere: x_F>0 Pomeron side (P), x_F<0 photon side (G); frame B (nominal) and frame A (both in the CSV, `frame` column).
- z*: 2E*/M_X, E* from x_F, p_T*^2 and the pion mass, M_X = frame mass; z_lab: P.p_h/P.q, computed from frame-B quantities (boost invariant; checked against the direct lab value on 104,459 data tracks, max |diff| 1.5e-8). Split per hemisphere at the data median, or at 0.2.
- p_T: `pt2s` = p_T*^2, `pts` = p_T* (fixed edges read in GeV), `ptlab` = lab p_T. Edges: `q<n>` data quantiles (equal population, all tracks, both hemispheres), `f<n>` fixed {0,.25,.5,1,2,5}; 4 bins {0,.25,.5,1,5}, 3 bins {0,.5,1,5}. Last bin open.
- beta: DiffTot `beta` for reco (median ratio to Q2es/(Q2es+M_X^2) is 0.985; gen uses Q2t/(Q2t+mirMX^2), a ~1.5% offset at the split), split at the data event median.
- D7 (record sec. 3): masked if weighted gen-valid DIFF8 count <50, purity <0.35 or stability <0.35 (weighted; purity denominator includes fakes; G2 gen proxy; gen beta from true Q^2 and mirMX). Evaluated per charge. The WP-A6 purity machinery is M_X-binned f_ND; not used.

## Medians and edges (frame B; frame A in summary.json)
beta median 0.523 (low-beta bin: M_X 5/50/95% = 13.2/19.6/31.7; high-beta bin: 5.2/10.4/17.5 GeV). z* median G 0.094, P 0.083; z_lab median G 0.077, P 0.013. p_T*^2 quantile edges (GeV^2) 0/0.042/0.103/0.222/0.547/inf (4 bins 0.055/0.152/0.424, 3 bins 0.079/0.291); lab p_T 0/0.322/0.545/0.914/1.75/inf.

## Counts (frame B; cells in bin order; data h+h- summed | 9012 raw gen-valid h+/h- | D7 failure)
Events (data / DIFF8 weighted / 9012 raw): beta-low 7,648 / 5,978 / 812; beta-high 7,648 / 6,490 / 955. With >=1 track: G beta-low 7,479, beta-high 7,340; P 6,949 and 6,929 (data).
z* median split (2 bins): G lo-beta 20345,14487 | 632/616, 590/595; G hi-beta 10495,16354 | 411/385, 835/822; P lo-beta 15082,8232 | 469/461, 286/318; P hi-beta 6307,13157 | 221/243, 623/615. Fails: P hi-beta z*<median (purity 0.33).
z* split 0.2: data G 29409/5423, 19665/7184; P 21654/1660, 14457/5007; 9012 min 57 (P lo-beta z>0.2). No D7 failure.
p_T*^2 quantile (5): data G lo 7659,6245,6233,6604,8091; G hi 6814,5087,4915,5076,4957; P lo 2833,5347,5248,5055,4831; P hi 3412,4357,4522,4152,3021. 9012 min 82 (P lo bin 0). Fails: G hi-beta bins 1-4, purity 0.23-0.33 (8 of 40 per-charge bins).
p_T*^2 fixed (5): data G lo 21082,5110,3779,2464,2397; G hi 17576,3881,2685,1591,1116; P lo 14181,3877,2581,1474,1201; P hi 12972,3147,1844,929,572. 9012 min 11 (P hi top bin). Fails: G hi bins 1-4 and P hi top, purity 0.10-0.28 (10 of 40; one also stability 0.33).

## D7 and 9012 factors (per charge, frame B; "N" = weighted count >=50 criterion)
- The N criterion is met in every primary bin (min weighted 73): the 9012 factor for N is 1.0 for z* (both splits) and p_T*^2 (q5, f5, q4, f4, q3, f3). The N criterion is set by luminosity (n_sig_pred is the yield at data luminosity), so more MC does not change it; CSV column `f9012_weighted` is a yield-growth factor, not an MC factor (frame A f5: 1.46, one bin).
- Raw 9012 >=50 per bin (the plan's stat-limiting reading): factor 1.0 for z*, pt2s q5/q4/q3 (min 82); 4.55 for pt2s f5; 1.43 for f4 and f3; frame A f5 10.0, f4/f3 2.0, q grids 1.0. p_T* fixed f5: 25. z_lab 0.2 split: not reachable (P-side z_lab>0.2 holds 0-4 tracks).
- Purity/stability failures are not statistical: 8 (q5), 10 (f5), 5 (q4), 7 (f4), 1 (q3), 5 (f3) of 40/32/24 per-charge bins in frame B, in G hi-beta (p_T*^2) or P hi-beta (z*, f5 top bin); z*: 2 of 16 (median split), 0 (0.2 split). Frame A: q5 5, q4 0, q3 0, f5 8. Multiplying 9012 does not change them.
- Reducing 5 to 4 or 3 p_T bins therefore changes the raw-9012 factor (f5 4.55 -> 1.43) and the purity failures (q5 8 -> q4 5 -> q3 1), not the weighted-count factor.

## Not computable / caveats
- Data counts are binning statistics only; the D7 mask uses MC only (plan D7). Stability and purity use the G2 proxy, not the true WP-E X frame.
- 9012 raw counts here are reco gen-valid tracks of the cocktail's elastic sample; the other samples use their own weights, so the only MC multiplication factor is the raw-9012 one (`f9012_raw50`).
- Q^2 bins and D cells are not used in the bins; D-cell scales enter only through the MC weights.

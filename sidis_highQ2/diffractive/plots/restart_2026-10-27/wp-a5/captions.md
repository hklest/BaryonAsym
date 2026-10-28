# WP-A5 figure captions

Made by `D/scripts/restart_2026-10-27/wp-a5/plot_wpa5.py`, which reads only stored JSON.

- **fig1_q_per_cell.{pdf,png}.** Forward-veto signal-loss correction q = (f_data − f_MC,full)/(1 − f_MC,full). It is
  measured on the deep-gap N−1 view (etaMaxClus08 ∈ [−1, 2.5), with the other two vetoes applied), with chain 7071 and
  the fine G1′(c) scales, for each detector (Plug, FTS, FMD) and each cell (M_X bin × D cell). Blue circles are the even
  split, which is frozen and nominal. Red squares are the odd split. Both carry binomial and MC statistical errors. The
  shaded band is q_even ± σ_q, with σ_q = max(|q|/2, |q_even − q_odd|) (D14).
- **fig2_eta_shape.{pdf,png}.** η-shape term δ_v = q_v,ALL(η_max bin) − q_v,ALL(deep gap), for ALL cells and all M_X,
  even (applied) and odd. The shaded area is the deep gap. The "other" bin (η < −1 or sentinel) is shown, but events in
  it use δ[−1,1) (A5-D4). The DIFF8 purity falls from 0.97 to 0.14 across these bins (verdict §3).
- **fig3_syst_per_bin.{pdf,png}.** Propagated relative systematic σ_rel = sqrt(max(|R_up|,|R_down|)² + R_eta²) on the
  per-event-normalised x_F and p_T*² spectra. The rows are frames A and B; the groups are charge × M_X bin. Filled
  markers are bins kept by the WP-C D7 rule and open markers are D7-masked bins. Grey bars are |R_eta|. The dashed line
  is the 0.05 acceptance of G1″(a).

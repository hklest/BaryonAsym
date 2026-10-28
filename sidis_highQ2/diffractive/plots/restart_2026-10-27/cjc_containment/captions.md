# CJC-containment figures: captions

All figures: H1 HERA-II, A_pre (Q² > 150 GeV², 0.1 < y_Σ < 0.9, |z_vtx| < 35 cm), D cells D0+D1+D2, reco 0 < x_IP < 0.03 and
4 ≤ M_X ≤ 40 GeV, η_max = etaMaxClus08 < 3.3 (or sentinel). Data are the four certified periods. MC = s_diff·DIFF8 + s_ND·ND
with the G1′(c) fine scales per D cell (as `gate_evidence/fig6`); 7071 stacked, 7091 as a dashed total. Odd and even
EventNumber parities are summed (per-parity numbers are in the JSON). "Contained" is the nominal TRK definition of
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/docs/restart_2026-10-27/cjc_containment/cjc_containment_record.md`
(≥ 90 % of the current-hemisphere track energy at 20° < θ < 160°). Numbers: `D/data/restart_2026-10-27/cjc_containment/cjc_containment.json`.

## fig_c1_etamax_contained.png — η_max, contained / not contained / all
What is plotted: η_max in 0.25 bins up to 3.3 (first bin includes underflow and the sentinel), LRG sample (all three
vetoes pass), per unit η. Ratio panel: data/MC with data Poisson errors and the MC-stat band; bins with < 10 data events
are not drawn in the ratio.
What to look at: the contained panel (21 665 events) is the full-sample panel (22 878) minus 5 %; the shapes are the same.
Data/MC in the contained subset runs 0.60–0.82 at η_max < −0.2, 0.71–0.89 over 1.05–2.8 and 1.08 / 1.17 (7071) in the two
top bins; the MC-stat band is ±9–11 % below η_max = 0.3 and ±2–4 % above 2.0, so the low-η deficit and the edge excess are
both significant. 37 % of the contained data events sit at 3.0 ≤ η_max < 3.3. The not-contained panel (693 events) is
statistics-limited and has no distinct shape.

## fig_c2_occupancy_contained.png — where the contained events sit (data)
What is plotted: contained data events in reco M_X (log) versus x_IP (log) and versus η_max. Dotted lines: M_X bin edges
10 and 20 GeV. The empty lower-right corner is the kinematic limit x_IP ≥ (Q² + M_X²)/(ys) at Q² > 150 GeV².
What to look at: the contained events fill the whole selected plane (M_X 5–95 % quantiles 7.1–31.9 GeV, x_IP 0.0073–0.029);
η_max rises with M_X; 82 % of the contained events with η_max < 1.55 have M_X < 15.3 GeV.

## fig_c3_veto_purity_contained.png — forward-veto N−1 fail fractions and purity
What is plotted: left, the N−1 fail fraction of Plug (E ≥ 7 GeV or no measurement), FTS, FMD and any veto (COMB), data
(black) and MC (DIFF8+7071 red, DIFF8+7091 green bar), for the full sample (open squares) and the contained subset
(filled circles); method as `gate_evidence/veto_eff_hist.py` but with the x_IP and M_X cuts applied. Right: DIFF8 share
of the full MC in each N−1 sample. Error bars (binomial data, Σw² MC) are smaller than the markers.
What to look at: contained and full are indistinguishable. Plug: data 0.180 against MC 0.043 (Δ = +0.137 ± 0.003);
FTS 0.149 against 0.089 (+0.060); FMD 0.383 against 0.444 (−0.061); COMB −0.025 (7071) / −0.005 (7091).

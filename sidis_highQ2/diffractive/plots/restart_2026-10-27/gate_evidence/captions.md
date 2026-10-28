# Gate-evidence figures: captions

All figures: H1 HERA-II, Q² > 150 GeV², 0.1 < y < 0.9, |z_vtx| < 35 cm (A_pre). Data are the four certified periods
(ep0304, em0405, em06_true, ep0607_true). MC = DIFF8 (RAPGAP 9012 elastic + ODS 9024/9033/9155/9157/9158, Reggeon
6671/6677) + one ND chain, with signed-v3 weights and the chain exposure rescale, as in the gates. "odd"/"even" is the
EventNumber parity: odd is the gate test sample, even the derivation sample. Lower panels: data/MC with data
Poisson errors (points) and the ±1σ MC-stat band (shaded; per-chain colour when several MC are overlaid). Triangles
mark ratio points off the panel scale. D0/D1/D2 are the kinematic cells. Numbers quoted below are in
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/gate_evidence/summary.json`.

## fig1_etamax_fit_{7071,7091,nd4cat}_{fine,coarse}.png — η_max template fit, G1′(c)

What is plotted: the cluster η_max (etaMaxClus04) in the veto-passing sample with the η_max cut removed (selFmd,
E_Plug < 7 GeV, selFts; the FIT sample of the G1′(c) fit), odd split, one panel per cell. Fine binning is 0.05 in η up
to 3.4 plus one merged edge bin [3.4, 5) (the WP-A2 S3 fit-range correction). Coarse binning merges fine bins until
each holds ≥ 100 data events. The stack is the stored post-fit prediction: the diffractive template (DIFF8, relative
weights fixed) times the fitted s_diff, plus the ND template times s_ND. The stack reproduces the fit's stored
`prediction` array exactly. The band is the MC stat that the gate toys use (Σ (s·w)²/raw per component). y is events per
unit η. In the fine ratio panel, bins with < 10 data events are not drawn. No 3.3 line is drawn: the LRG cut
η08 < 3.3 is on etaMaxClus08 and is not applied in this sample. The dotted 3.4 line is the edge of the fit binning on
etaMaxClus04, above which all events are merged into one bin.
What to look at: below η ≈ 3 the ratio scatters around 1 inside the MC-stat band. That band is ±20–100% at η < 1, where
the MC is thin. With MC stat included, χ²/bin over the η < 3.0 bins is 1.8 / 1.5 / 1.0 (fine, D0/D1/D2, 7071;
142/80, 111/75, 55/57), similar for 7091 and nd4cat. By eye the agreement there is reasonable. Statistically it
is not perfect: 142/80 has p ~ 1e-5. The failure sits in
the highest-statistics bins at the forward edge. Over 3.0–3.4, data/MC is 1.1–1.3 in most bins and peaks at
1.31–1.48 at η = 3.30–3.35, with a dip to 0.78–1.02 at 3.10–3.15 in D1/D2. It then drops to 0.81–0.87 in the merged [3.4, 5)
bin, in every cell and for every ND chain. The three largest deviance bins carry 31–53% (fine) and 46–82% (coarse) of
the deviance. In D0 and in the coarse fits they are the 3.15–3.4 bins. In some D1/D2 fine fits, one or two of them
are thin low-η bins. The quoted D is a Poisson deviance with no MC stat. The toy median D (145–420 for 87 fine bins,
1.7–4.8× the ndf) shows how much MC stat alone inflates the expected D. The observed D is still 2.4–4.0× above the toy
median. About half of the excess sits in the 3.15–3.4 bins. The rest is spread over the range at the
χ²/bin ≈ 1.5–1.8 level. The fitted scales are not near 1 (s_diff 0.76–1.48, s_ND 0.96–1.92). The failure is a χ² statement for most of the
range. It is a visible 30–50% shape mismatch in the 3.15–3.4 bins.

## fig2_plug_n1_{7071,7091,nd4cat}_{lin,log}.png — Plug energy N−1, G1′(a)

What is plotted: E_Plug in 0.5 GeV bins, 0–10 GeV (the gate grid runs to 50 GeV), in the stack-C N−1 sample (A_pre,
etaMaxClus08 < 3.3, selFmd, selFts; Plug cut removed), odd split, per cell; linear and log versions. Normalisation is
the gate's: MC scaled to unit area relative to data over the 0–50 GeV bins with ≥ 20 data events (MC/data scale
1.33–1.89). The red line is the veto: events with E_Plug ≥ 7 GeV (or no Plug measurement) fail. The boxes give
f_fail (the fraction failing the veto) for data and MC.
What to look at: the disagreement is large and visible, not a χ² effect. The MC-stat band is ±2% in the first bin
and ±10–37% at the highest E_Plug, far below the factor 1.5–3 deviations. MC puts too many events in the first bin:
data/MC there is 0.51–0.57, i.e. MC/data 1.76–1.95. MC has too few events everywhere above: data/MC is 1.4–3.4 over
most of 1–7 GeV (minimum 0.94–1.05 in single bins in D1/D2) and 1.3–3.7 from 7 to 10 GeV. The fail fraction is 0.272 / 0.300 / 0.337 in data against 0.058–0.113 in MC. Note: the explainer's
"MC/data 1.35 at 0 GeV, 0.29–0.44 at 1–7 GeV" comes from the WP-A G1 sample (`docs/restart_2026-10-27/wp-a/g1_verdict.md`,
data f_fail 0.219/0.236/0.285). These figures show the G1′ sample (η08 < 3.3), so those two numbers are not reproduced here. On this sample
MC/data is 1.76–1.95 at 0 GeV.

## fig3_fmd_fts_n1_{7071,7091,nd4cat}.png — FMD and FTS N−1, G1′(b)

What is plotted: left, nFmu12 (FMD hit pairs, layers 1–2) in the N−1 sample without selFmd. Right, nFts28 (FTS hits,
stations 2 and 8, saturating at 4) without selFts. All cells, odd split, MC unit area on bins with ≥ 20 data events (the
gate metric). selFts is exactly nFts28 = 0. selFmd passes only for nFmu12 ≤ 1, though not every such event, because
the bit also uses other FMD information.
What to look at: both mismatches are large and coherent, not driven by MC stat (the band is barely visible). For FMD,
data/MC is 1.1–1.5 for 1–8 pairs (0.82–0.90 at 0), crosses 1 near 10, and falls steadily to 0.32–0.37 at 19, so MC has too hard an
FMD multiplicity spectrum. Only 2 of 20 bins agree within 2σ. For FTS, the veto-passing bins 0 and 1 agree
(0.92–1.04). Data/MC is 1.24–1.67 at 2–3 hits and 3.1–3.5 in the saturated bin 4 (all layers hit). The 0.74–0.78
unmapped best "admissible joint" value of G1′(b) comes from this shape.

## fig4a_h4_inclusive_eta08.png — inclusive no-veto DIS, each ND generator

What is plotted: etaMaxClus08 in the H4 sample (A_pre and 35 < E−p_z < 70 GeV, no forward veto, no gap requirement),
even split (the WP-A2 S2 H4 sample), per cell. Data are compared with DIFF8 + ND for 7071 and 7094 (RAPGAP ME+PS) and
7091 (RAPGAP CDM), each normalised to data per cell over the full range. ND is 85–93% of this sample.
What to look at: the edge pattern is visible in all three ND samples. Data/MC is 1.1–2.0 just below the LAr edge
(3.0–3.35) and 0.5–0.9 at 3.40–3.55, so in data the η_max distribution ends about one 0.05 bin earlier than in MC.
At η < 2 the three generators differ from each other by up to a factor 2–3 in the low-statistics tail (7094 runs
low, 7071 and 7091 high). That is ND-model dependence, not the edge.

## fig4b_h4_edge_eplug20_eta08.png — H4 edge sample (E_Plug > 20 GeV)

What is plotted: as fig4a, restricted to events with E_Plug > 20 GeV (guaranteed forward activity; this is the WP-A2
S2 H4 criterion sample), linear y, 2.5 ≤ η < 4.0, MC normalised to data per cell.
What to look at: this is the cleanest view of the LAr forward-edge feature. The data peak ends at η ≈ 3.5 and the MC
peak about one bin later. Over 3.0–3.35, data/MC is 1.15–1.85 (7071, 7094) and 1.43–2.84 (7091). Over 3.40–3.55 it is
0.49–0.88. The worst |MC/data − 1| is 0.67–0.75 (ME+PS) and 0.89–1.02 (CDM), matching the S2 record. The statistics
are large (thousands of events per bin), so this mismatch is plainly visible by eye.

## fig4c_fit_eta08_drift.png — where the forward drift starts

What is plotted: etaMaxClus08 in the veto-passing sample without an η_max cut (selFmd, E_Plug < 7 GeV, selFts), all
cells, even split, with DIFF8 + ND normalised to data in 1.0 ≤ η < 2.5, for 7071, 7091 and nd4cat (dashed). Dotted
lines mark 2.25 and 2.55. The red dashed line is the LRG cut at 3.3.
What to look at: from 0 to 2.25 the bin-by-bin ratio scatters widely (0.64–3.1; about half the bins within
0.8–1.2), where data and MC are thin and the MC-stat band is wide. It shows no trend: in 0.25-wide steps it is
1.01–1.02 at 1.75–2.0. Data/MC then rises steadily: 1.07–1.09 at 2.25, 1.17–1.21 at 2.5, 1.37–1.48 at 2.75,
1.67–1.84 at 3.0. The first 0.05 bin outside ±10% is at 2.55 for all three chains. The plateau is absent, and the
rise is not confined to the last bin before the cut.

## fig5_reweight_before_after_{7071,7091,nd4cat}.png — WP-A3 veto-state map

What is plotted: data vs MC before (grey dashed) and after (coloured) the WP-A3 run-2 joint veto-state map (fine Plug
classes × selFmd × selFts, derived per D cell on the even no-gap control CTRL, x-set XA, applied to odd events). Left:
Plug N−1, all cells, 0–10 GeV, MC unit area on bins with ≥ 20 data events. The box gives f_fail per cell. Right:
etaMaxClus04 in the veto-passing FIT sample, all cells, MC normalised to data over the full range.
What to look at: the map fixes most of the Plug disagreement that fig2 shows. Data/MC in the first bin goes from
0.53–0.55 to 0.94–1.01, and over 1–7 GeV from 1.2–3.0 to 0.78–1.31. f_fail closes to within 0.002–0.04 of data.
By eye the mapped Plug distribution agrees at the ±20–30% level. It still fails the pre-registered 10% shape limit,
with a 0.26–0.28 max deviation over 0–10 GeV. That deviation is driven by the 2.0–2.5 GeV bin (data/MC 0.78–0.80)
and by data/MC 1.15–1.31 at 5.5–7 GeV, just below the threshold. Read this as "reasonable but not within 10%", not as a gross failure. Because the map is fitted
on control data, its rate closure is partly by construction. For η_max the map moves the 1.5–2.8 region from data/MC
0.47–0.80 to 0.65–1.19 (0.05 bins). The forward edge stays. Over 2.8–3.4, data/MC is 0.91–1.44 after the map against
0.73–1.53 before, with the same excess at 3.15–3.35 and the same drop to 0.81–0.83 at 3.40–3.45 (summary.json
`fig5_*` → `eta_*`). The map does not remove the edge mismatch of fig1/fig4.

## fig6_veto_eff_vs_etamax.png — forward-veto fail fraction vs η_max

What is plotted: the fraction of events that fail each forward veto, in five etaMaxClus08 bins [−1,1), [1,2),
[2,2.5), [2.5,3.0), [3.0,3.3), odd split, D0+D1+D2. The panels are Plug, FMD and FTS, each N−1 (the other two vetoes
applied), and the combined veto (no veto applied; fail if any of the three fails). Black points are data with binomial
errors. Solid lines with bands are DIFF8 + ND chain (7071 red, 7091 green, nd4cat orange) with the fine G1′ fitted
scales per D cell. Dotted lines are the DIFF8 prediction alone. The lower panel gives data − MC (points: data error;
bands: MC stat). The box gives the DIFF8 share of the MC per bin (7071 fit).
What to look at: Plug fails far more often in data in every bin. The data − MC difference is +0.038, +0.065 and
+0.088 in the three deep-gap bins and +0.155 and +0.228 above η_max 2.5 (7071; the other chains are within 0.006).
MC Plug f_fail stays at 0.011–0.030 below 2.5, against data 0.049–0.119. FTS: data − MC is +0.031 to +0.066 in all
bins. FMD: MC fails more than data, by −0.014, −0.014, −0.052, −0.083 and −0.148. This tracks the falling DIFF8 share
(0.92 → 0.17) and the spread between ND chains (up to 0.04), so FMD mostly probes the ND modelling. Combined veto:
data − MC is +0.039, +0.030, −0.000, −0.024 and −0.053. It is small, but it is the sum of opposite-sign Plug/FTS and
FMD terms in a sample that is 15% DIFF8 overall. It should not be read as a good veto model. Purity in the
Plug N−1 falls 0.97, 0.92, 0.83, 0.68, 0.43. Per-cell (D0/D1/D2) and even-split values are in
`data/restart_2026-10-27/gate_evidence/veto_eff.json` → `per_eta`; the figure shows D0+D1+D2, odd.

## fig7_plug_eff_kinematics.png — deep-gap veto fail fraction vs D cell and M_X

What is plotted: as fig6, restricted to the deep gap η_max ∈ [−1,2.5), for the combined veto (top) and the Plug N−1
(bottom). Left: per D cell. Right: in three M_X bins (data tertiles: < 10.2, 10.2–16.1, ≥ 16.1 GeV), all cells. The
boxes give χ²/2 dof (p) of data − MC about its weighted mean, per chain.
What to look at: the Plug excess does not vanish in any cell and grows toward D2 and toward high M_X. Data − MC is
+0.061 / +0.066 / +0.095 ± 0.005/0.010/0.025 in D0/D1/D2 (7071; 7091 D2 +0.117) and +0.053 / +0.068 / +0.075 over M_X.
On the odd split this is compatible with flat (χ² p 0.08–0.40 over D cells, 0.01–0.11 over M_X). The even split shows the same trend
more strongly (D2 +0.138), and flatness is rejected there (p ≤ 0.006). As a pass-efficiency ratio (1−f_data)/(1−f_MC), which is what
a per-event-normalised spectrum sees, the variation is about 5% on the odd split (0.946 → 0.898) and about 10% on the even split
(0.951 → 0.854). Purity is 0.85–0.94, falling to 0.51–0.58 in D2. The combined veto is not flat. Data − MC is +0.049 / −0.011 / −0.092 over
the D cells (7071; nd4cat −0.112 in D2, 7091 −0.023) and −0.040 / +0.070 / −0.004 over M_X, with p ≤ 0.003 for every chain.
The D2 and M_X structure follows the ND share (purity 0.19–0.24 in D2) and differs between ND chains. Numbers:
`data/restart_2026-10-27/gate_evidence/veto_eff.json` → `kinematics`; discussion in
`docs/restart_2026-10-27/gate_evidence/veto_efficiency_view.md`.

# E_e' > 11 GeV reco cut: how much of the unfolded result is acceptance extrapolation (2026-10-29)

Edition: `esigmaphotonxf_2026-10-27b` (frozen; nothing in it was changed). Grid: 245 gen bins per charge (7 Q2-x cells x 35 z-pT bins).
Question: the reco cut `ELEC_E_MIN = 11 GeV` (CUT 3, `TreeBuilderHighQ2_chargedsidis_v6.C`) has no gen-level counterpart. Is the unfolding's extrapolation into the E_e'(gen) < 11 GeV corner acceptable?

## Method
- Condor cluster 5971062: 174 jobs, one per production shard list (RAPGAP31 73, DJANGOH14 101), the same file lists as the 27b responses.
- Script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/qa/elec_e11_acceptance/e11_worker.py`. It assigns each gen bin with the response builder's own `FileAccumulator.compute_gen_bin`: the full 27b gen definition (true gamma*, 150<Q2<20000, 0.1<y<0.7, GenEmpz>45, xF<0, z/pT support, w1*w2). The merge step is `e11_merge.py` in the same directory.
- Rows are split by `GenElecE` < 11 or >= 11 GeV.
- Reco acceptance here is event-level. A gen row counts as accepted if its event is in ChargedTot with 150<Q2es<20000, 0.1<Yes<0.7 and Empz>45. ChargedTot is written only for events that pass reco E_e' > 11.
- Track and cone efficiencies cancel in the ratio eps_lt/eps_ge, so they are left out.
- Metric: X = f_lt * (1 - eps_lt/eps_ge). This is the share of the bin's gen-level cross section that the MC supplies for phase space the reconstruction does not see.

Note on the gen y cut: at gen level y < 0.7. With E' = E_e(1-y) + Q2/(4E_e), the Born corner is limited to Q2 < 300 GeV², y between about 0.65 and 0.70, and x < 0.0042. ISR shifts some events outside this window.

## Results (per-bin CSV `plots/qa/elec_e11_acceptance/e11_corner_per_bin.csv`, summary JSON, plot `e11_corner_S0.png`)
| cell | f_lt RG / DJ | X RG / DJ (sigma-weighted) | max X per bin | eps_lt/eps_ge |
|---|---|---|---|---|
| S0_x0 (150-300, x 0.0021-0.0071), h+ | 0.0576 / 0.0563 | 0.0511 / 0.0498 | 0.0625 | 0.11 |
| S0_x0, h- | 0.0577 / 0.0564 | 0.0512 / 0.0499 | 0.0676 | 0.11 |
| S1_x0 (300-700, x 0.0042-0.016) | 0.0004 / 0.0003 | 0.0004 / 0.0003 | 0.0016 | 0.01-0.02 |
| S0_x1, S1_x1, S2_*, S3_x0 | 0 | 0 | 0 | - |

- Only the 35 S0_x0 bins per charge carry a share above 1%. No bin has X > 0.1, so no bin is dominated by zero-acceptance phase space (the "dominated" flag, X > 0.5, is empty).
- Within S0_x0, X ranges from 0.039 to 0.068. The median is 0.048, about 1.5 times the median total uncertainty of the bin (3.2% h+, 2.9% h-). The largest values are in the lowest z bin (0-0.05) at high pT.
- About 21% of the corner rows carry an ISR tag. The rest is Born kinematics.
- Integrated over all 245 bins, X is 1.57% (h+) and 1.59% (h-).
- Model dependence in S0_x0, X(RG) - X(DJ):
  - median |dX| is 0.15% (h+) and 0.10% (h-); the maximum is 1.3% and 0.6%;
  - the mean is +0.10% and +0.07%;
  - this is roughly 1-2 times the MC statistical precision of dX.
  - The quoted `ibu_model` term in the same bins has a median of 1.1%, so dX is about 10 times smaller. That term is the full RG-vs-DJ response swap, which already contains this corner difference.

## Recommendation
**OK as is; no new systematic.** The corner sits entirely in S0_x0 and is about 5% (at most 7%) of the cross section there. Its acceptance is small (about 0.11 of the accepted region's, from E resolution migration) but not zero. No bin's corrected yield is dominated by unseen phase space.

The MC extrapolation is fixed mainly by Born kinematics within a few hundredths in y at known (Q2, x). It is model-stable: RAPGAP31 and DJANGOH14 differ by about 0.1% of the cross section, an order of magnitude below the `ibu_model` term already quoted, and that term covers this difference by construction.

The ISR part (about 21% of the corner, about 1% of the S0_x0 cross section) depends on QED modelling. Both generators use HERACLES-type QED, so that part is not independently tested here; it falls under the existing C_rad treatment.

Restricting the gen phase space with E_e' > 11 GeV would remove a 5% extrapolation from one cell but add a non-standard, detector-motivated cut to the cross-section definition. Theory comparisons (MAPTMD24) would then need it too. This is not warranted for an effect of this size and this stability.

Suggested action: one sentence in the note's phase-space or unfolding discussion stating the S0_x0 extrapolation share (about 5%) and its model stability (about 0.1%). Per instructions the note was not edited here.

Caveat: the acceptance is event-level (electron-side cuts only), not the full response body. The eps ratio is what drives X, and the hadron-side efficiencies cancel in it to first order.

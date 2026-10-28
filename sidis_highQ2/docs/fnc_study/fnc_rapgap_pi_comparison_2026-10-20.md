# RAPGAP-π 9134 FNC comparison

**Date:** 2026-10-20  
**Status:** validation PASS; all plots are uncorrected exploratory, unit-area shape comparisons. No luminosity scaling, FNC/track efficiency, acceptance, or radiative correction is applied.

## RAPGAP-π tree validation

The new ReqID 9134 tree has **393,344 rows** and **46,292 events**. All 16 required FNC branches are present; FNC values are event-constant (zero nonconstant events in every checked branch). `v6fnc_stage2_version` is `ChargedSidisTreeBuilderHighQ2_v6fnc/2026-10-20`. `FncPresent` = **34.518%**; `FncGenNFound` = **100.000%**; finite non-sentinel Part-A projection among gen-found events = **100.000%**.

The explicit stored-branch high-Q2 selection is `Q2es>150 GeV2`, `0.1<Yes<0.7`, `Empz>45 GeV`, and `abs(PVz)<35 cm`; **64.465%** (29,842 events) passes. Q2es 10% / median / 90% quantiles are **136 / 248 / 742 GeV2**, documenting substantial lower-Q2 reach.

## Comparison conventions

Track selection is `IsFromPrimary==1`, `(IsCentralTrk==1 OR IsCombinedTrk==1)`, finite `Pt>0`, finite `Eta/Phi`. All phi residuals use `atan2(sin Δphi, cos Δphi)`. The sim-cone page applies reco `FncThetaN<0.75 mrad`, the established region where MC FNC response is trustworthy.

Associated reco–truth pairs use the exact prior-study exploratory proxy/association cuts. RAPGAP-π gives E ratio mean/rms **1.0270/0.0791**, dtheta mean/rms **0.0111/0.0696 mrad**, dphi mean/rms **-0.0028/0.1788 rad** (N=10,440); DJANGOH control is **1.0525/0.1309**, **-0.0294/0.0917 mrad**, **0.0518/0.2095 rad** (N=2,731).

## Artifacts

- PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_rapgap_pi_comparison_2026-10-20.pdf`
- Numeric sidecar: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_rapgap_pi_comparison_2026-10-20_numbers.json`
- Script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/fnc_study/analyse_fnc_rapgap_pi_comparison_2026-10-20.py`


## 2026-10-21 Q²-scope correction

**Correction to the 2026-10-20 wording.** The 46,292-event `ChargedTot` tree is **not** an unbiased representation of the ReqID-9134 generated `Q²>3 GeV²` sample. The reported `64.465%` is correctly calculated, but it is the fraction of the **46,292-event Stage-2 charged-track-bearing tree** passing the stated stored-branch final selection; it must not be read as a fraction of the generated sample. Likewise, the `Q2es` quantiles `136.255 / 247.963 / 742.098 GeV²` describe that selected tree, not the generator population. The phrase “substantial lower-Q² reach” is withdrawn: this tree only has a lower edge/tail relative to the final `Q2es>150` cut and cannot support a lower-Q² study without a dedicated low-Q² production.

Stage 1 uses the v6 MC OR admission: a loose **reconstruction envelope** (`Q2es>100 GeV²`, `0.05<Yes<0.9`, `30<Empz<75 GeV`) passing its event-quality gates, **or** a generator envelope (`Q2sGen>60 GeV²`, `0.01<YsGen<0.99`, `25<GenEmpz<75 GeV`). Thus it rejects the overwhelming low-Q² part of the Q²>3 generator sample before Stage 2. Across the 101 inputs, Stage 1 retained `260,195 / 9,999,998 = 2.60195%` events; only `46,292 / 9,999,998 = 0.462920%` appear in the merged Stage-2 charged-track tree (the remaining Stage-1 events have no selected `ChargedTot` row). The 9,090,000 figure is not the actual input count: logs show 99 files of 100,000 events, one of 90,000, and one of 9,998.

`Q2es` is correctly labelled as the reconstructed **eΣ** estimator: it is filled with `H1CalcKine::GetQ2es()`, not the electron-method `GetQ2e()`. In the selected 46,292-event tree the HAT generator-Σ branch `Q2sGen` has 10%/50%/90% quantiles `127.536 / 242.764 / 723.715 GeV²` (82.844% above 150), consistent with the reconstructed `Q2es` distribution `136.255 / 247.963 / 742.098 GeV²` (85.371% above 150). The low tail arises after the loose Stage-1 envelope and reconstruction migration; it does not contradict the original generated Q² spectrum.

A one-file raw-HAT check (100,000 generated events) uses the generated-electron estimator `HAT.Q2eGen` for the generator-level Q² corresponding to the MCDB `Q²>3 GeV²` statement: its 10%/50%/90% quantiles are `3.381 / 6.162 / 25.089 GeV²`, and `0.8860%` is above `Q2eGen>150 GeV²` (binomial statistical uncertainty about `0.0296` percentage points). For estimator continuity, `HAT.Q2sGen` in the same file gives `3.220 / 5.867 / 23.557 GeV²` and `0.7640%` above 150. These are the relevant small generated high-Q² fractions and are fully compatible with the Stage-1/Stage-2 retention figures.

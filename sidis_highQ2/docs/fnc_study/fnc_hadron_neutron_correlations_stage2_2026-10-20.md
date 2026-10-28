# Per-track v6fnc Stage-2 charged-hadron / forward-neutron correlation study

**Date:** 2026-10-20  
**Status:** uncorrected exploratory shapes only. No FNC/track efficiency, acceptance, radiative, trigger/luminosity, or physics-model correction has been applied.

## Inputs and track convention

- DATA ep0607: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6fnc/ep0607_study/stage2/merged/chargedsidis_v6fnc_d07_ep0607_merged.root` (1,034,208 rows; the d07_0029 central chunk is pre-existing absent: one of 112 2007 chunks).
- MC DJANGOH14/7091 10-file merge: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6fnc/mc7091_study/stage2/merged/chargedsidis_v6fnc_djangoh14_7091_mc10_merged.root` (3,115,335 rows).
- Exact per-track branches used: `Phi`, `Charge`, `Pt`, `Eta`, `IsFromPrimary`, `IsCentralTrk`, `IsCombinedTrk`; FNC: `FncPresent`, `FncGoodRun`, `FncE`, `FncThetaN`, `FncPhiN`, `FncClType`, `FncHtype`. The production-contract selection was re-applied: `IsFromPrimary==1`, `IsCentralTrk==1 OR IsCombinedTrk==1`, finite `Pt>0`, finite `Eta/Phi`. No additional z cut is appropriate for this inclusive exploratory correlation; sentinel `z_SIDIS` values are not used.
- `DeltaPhi=atan2(sin(phi_track-phi_n),cos(phi_track-phi_n))` and all reco--truth phi residuals use the same wrap to `[-pi,pi]`.

## FNC phi coverage

For `FncPresent && FncGoodRun`, the distribution is restricted/nonuniform rather than a full-azimuth observable. The empirical MC 99%-occupancy shortest covered arc is start **6.142 rad**, span **6.037 rad**, ending (wrapped) **5.896 rad**. Numeric 1--99% quantiles are DATA [-3.020,3.046] rad and MC [-3.101,3.104] rad. This coverage is detector geometry and must not be interpreted as a physical phi distribution.

## Fiducial reco--truth validation (MC)

The Part-A projection is used. The reproducible empirical detector-fiducial proxy is `FncGenNFound`, projected `r=sqrt(FncGenNXfnc^2+FncGenNYfnc^2)` in **[0.20,31.17] cm** (the 0.1--99.9% reco-theta envelope at z=10550 cm), and projection phi inside the MC 99%-coverage arc above. This is necessarily an exploratory fiducial proxy because Stage-2 contains no reco FNC x/y nor calibrated FNC aperture map.

A leading-neutron/reco-cluster pair is accepted only if the fiducial truth neutron and reco cluster coexist and satisfy `|Delta theta|<0.4 mrad`, `|wrapped Delta phi|<0.5 rad`, and `0.3<Ereco/Egen<2.0`. There is one stored FNC cluster and one leading stored neutron per event, so this is one-to-one within the tree's content. Results: **N(fid truth)=59079**, **N(associated)=2730**, fiducial-gen-to-associated-reco efficiency **4.621%**, and reco-cluster-to-fiducial-associated-truth fraction **13.681%**.

Matched scale: mean `Ereco/Egen` = 1.0525  (RMS 0.1309 ; N=2730); `Delta theta` = -0.0294 mrad (RMS 0.0917 mrad; N=2730); wrapped `Delta phi` = 0.0518 rad (RMS 0.2095 rad; N=2730). The prior un-fiducialized candidate figures (median ratio 1.020, RMS 0.503; theta RMS 0.558 mrad; phi RMS 1.328 rad) are not resolution measurements. The explicitly fiducial/associated selection is narrower by construction and yields the numbers above; it should not be marketed as a detector calibration. All associated events have `FncGenNVtxMode=1` ({'1': 2730}): the event simulated-vertex proxy. Its residual effect cannot be isolated because no true per-neutron vertex/mode-2 sample exists.

## Charged-hadron correlations

DATA has **23285** selected FNC-good events and **206102** selected associated tracks (h+ 104498, h- 101604). Delta-phi summary: all 0.0092 rad (RMS 1.8131 rad; N=206102); h+ 0.0050 rad (RMS 1.8131 rad; N=104498); h- 0.0136 rad (RMS 1.8132 rad; N=101604). Histogram DATA uncertainties in the PDF are 200-replica track bootstrap standard deviations; MC/DATA overlays are each unit-normalized, with no luminosity scaling.

The energy-split page uses DATA FNC energy below/at-or-above 300 GeV. Results are descriptive only; any apparent same-side/away-side pattern remains convolved with the restricted FNC phi acceptance. FNC E/theta and phi overlays are also unit-area, uncorrected shape controls.

## Limitation

The missing d07_0029 chunk reduces data statistics and may cause a small period-sampling dependence; it is not corrected or imputed. The FNC finder retains one cluster (cluster 0) and E>10 GeV at production. No absolute rate/cross section or acceptance-corrected correlation is claimed.

## Artifacts

- PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_hadron_neutron_correlations_stage2_2026-10-20.pdf`
- Numeric sidecar: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_hadron_neutron_correlations_stage2_2026-10-20_numbers.json`

# Detailed high-Q² DATA vs RAPGAP-π FNC comparison

**Date:** 2026-10-21  
**Status:** uncorrected exploratory, shape-level study. The standard stored-tree selection is `Q2es>150 GeV²`, `0.1<Yes<0.7`, `Empz>45 GeV`, and `|PVz|<35 cm`; exact branches verified: `Q2es`, `Yes`, `Xes`, `Empz`, `PVz`.

## Essential scope and caveat
RAPGAP-π 9134 is a pure pion-exchange leading-neutron sample: every generated event contains a leading neutron. Its absolute tag rate is therefore not like-for-like with inclusive data. All overlays are unit-area shapes; no luminosity, FNC/track efficiency, acceptance, or radiative correction is applied. A parallel normalization task is establishing the effective MC luminosity for future rate-level work. MC FNC response is credible only inside reco `FncThetaN<0.75 mrad`; both MC generators also lack the data proton-beam-tilt pattern.

## Event summary
|sample|high-Q² events|tagged|tag fraction|in-cone tagged|in-cone fraction|
|---|---:|---:|---:|---:|---:|
|DATA ep0607|73,738|14,899|20.205%|4,455|6.042%|
|RAPGAP-$\pi$ 9134|29,842|10,324|34.596%|9,076|30.414%|
|DJANGOH14/7091|222,335|14,633|6.582%|10,260|4.615%|

## RAPGAP-π high-Q² reco–truth associated subset
Prior-study fiducial/proxy and association cuts were retained. `Ereco/Egen` = **1.0266 ± 0.0782** (RMS), `Δtheta` = **0.0112 ± 0.0698 mrad**, and wrapped `Δphi` = **-0.0047 ± 0.1782 rad** (N=6681). Full-sample reference values are 1.0270 ± 0.0791, 0.0696 mrad, and 0.1788 rad.

## Artifacts
- PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_rapgap_pi_highQ2_detailed_2026-10-21.pdf`
- JSON: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_rapgap_pi_highQ2_detailed_2026-10-21_numbers.json`
- Script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/fnc_study/analyse_fnc_rapgap_pi_highQ2_detailed_2026-10-21.py`

Track convention: primary central-or-combined tracks with finite `Pt>0`, `Eta`, and `Phi`; z uses `z_SIDIS` after finite/sentinel filtering. Δφ is wrapped using `atan2(sin Δφ, cos Δφ)`.

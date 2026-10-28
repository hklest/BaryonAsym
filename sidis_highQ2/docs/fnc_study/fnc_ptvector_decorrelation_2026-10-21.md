# FNC transverse-vector decorrelation and hadron-neutron back-to-back study

**Date:** 2026-10-21  
**Status:** uncorrected, exploratory. This is a shape-level DATA/RAPGAP-pi comparison plus an MC-internal resolution study; it is not an acceptance correction or a rate measurement.

## Observable and selections
The small-angle neutron transverse vector is `theta_x=theta cos(phi)`, `theta_y=theta sin(phi)`, `p_x=E theta_x`, `p_y=E theta_y`. This is the small-angle approximation. The back-to-back observable is `Psi=wrap(atan2(p_y,p_x)-Phi_track)`. Algebraically this is an azimuthal difference, but its error is evaluated in neutron `pT=E theta` bins, rather than interpreting an ill-conditioned phi at vanishing transverse radius. MC matching follows the prior study exactly: first tree row/event; data-face+slot+phi-arc truth fiducial; `FncPresent`, `FncGenNFound`, `|Delta theta|<1 mrad`, `|wrapped Delta phi|<1 rad`; no energy-ratio or MC GoodRun cut.

For the back-to-back plots the stored-tree high-Q2 selection is `Q2es>150 GeV2`, `0.1<Yes<0.7`, `Empz>45 GeV`, `|PVz|<35 cm`. Tracks are primary central-or-combined tracks with finite `Pt>0`, `Eta`, `Phi`. DATA uses `FncPresent && FncGoodRun`; RAPGAP-pi uses `FncPresent` without MC GoodRun.

## Resolution result: azimuth blow-up and vector components
All numbers below are RMS ± Gaussian RMS SE, all matched pairs, full trees (no high-Q2 selection). Theta bins are mrad.

### RAPGAP-pi: sigma(wrapped Delta phi) vs theta_true
`0-0.2: 0.354±0.004 (N=3702); 0.2-0.4: 0.277±0.003 (N=5083); 0.4-0.6: 0.241±0.003 (N=2575); 0.6-0.8: 0.313±0.009 (N=623); 0.8-99: 0.630±0.050 (N=79)`

### DJANGOH14: sigma(wrapped Delta phi) vs theta_true
`0-0.2: 0.477±0.012 (N=735); 0.2-0.4: 0.348±0.008 (N=1008); 0.4-0.6: 0.293±0.007 (N=965); 0.6-0.8: 0.304±0.008 (N=706); 0.8-99: 0.565±0.027 (N=224)`

The component resolutions `Delta theta_x,y` and `Delta p_x,y` stay finite in the corresponding PDF panels while the coordinate azimuth has the expected small-radius instability. The energy panels overlay the **published-detector reference** `sqrt((10 cm/sqrt(E))^2+(0.6 cm)^2)/(sqrt(2)*106 m)` per Cartesian angular component; this is a descriptive position-to-angle translation, not a fit or a validation of the current FNC MC.

## Vector-direction result
The angle between reco and true neutron transverse vectors is shown versus true `pTn=Etheta`; the 0.3-rad guide is descriptive. The sidecar gives every bin, count and uncertainty for both generators. The practical analysis split is `pTn=0.5 GeV`: use >=0.5 GeV for the trustworthy-direction display and retain <0.5 GeV as an explicitly ill-conditioned control.

## Back-to-back shape comparison
All Psi overlays have unit area and ratio panels. No luminosity scaling is used: RAPGAP-pi is pure pion exchange, so absolute tag rates are not comparable.

|sample|high-Q2 events|tagged|tagged pTn>=0.5|fraction of tagged|
|---|---:|---:|---:|---:|
|DATA ep0607|73,738|14,899|239|1.60%|
|RAPGAP-pi 9134|29,842|10,324|169|1.64%|

### trustworthy_ptn_ge_0p5
|sample|track class|N|RMS Psi [rad]|back-to-back fraction |Psi-pi|<pi/4|
|---|---|---:|---:|---:|
|DATA ep0607|all|2,284|1.840±0.027|25.088%|
|DATA ep0607|hplus|1,178|1.844±0.038|24.448%|
|DATA ep0607|hminus|1,106|1.835±0.039|25.769%|
|RAPGAP-pi 9134|all|1,490|1.738±0.032|21.275%|
|RAPGAP-pi 9134|hplus|745|1.725±0.045|21.342%|
|RAPGAP-pi 9134|hminus|745|1.750±0.045|21.208%|

### ill_conditioned_ptn_lt_0p5
|sample|track class|N|RMS Psi [rad]|back-to-back fraction |Psi-pi|<pi/4|
|---|---|---:|---:|---:|
|DATA ep0607|all|136,670|1.810±0.003|25.013%|
|DATA ep0607|hplus|69,114|1.809±0.005|24.956%|
|DATA ep0607|hminus|67,556|1.811±0.005|25.071%|
|RAPGAP-pi 9134|all|89,395|1.805±0.004|24.579%|
|RAPGAP-pi 9134|hplus|45,469|1.802±0.006|24.505%|
|RAPGAP-pi 9134|hminus|43,926|1.808±0.006|24.655%|

The leading-track `phi(pTtrack+pTn)` and `|pTtrack+pTn|` pages are descriptive balance controls only; neither is a momentum-conservation test in inclusive DIS.

## Limitations
The oo-4.0 MC FNC response is known to collapse outside approximately theta<0.75 mrad, and both MC samples use an ideal-axis neutron pattern rather than the data proton-beam tilt. No current-branch analysis correction repairs either issue. Thus these figures support an error-model-aware exploratory comparison, not an FNC-tagged measurement.

## Artifacts
- PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_ptvector_decorrelation_2026-10-21.pdf`
- JSON sidecar: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_ptvector_decorrelation_2026-10-21_numbers.json`
- Script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/fnc_study/analyse_fnc_ptvector_decorrelation_2026-10-21.py`

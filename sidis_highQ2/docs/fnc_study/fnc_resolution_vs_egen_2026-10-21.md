# FNC response versus true neutron energy: RAPGAP-pi and DJANGOH14

**Date:** 2026-10-21  
**Status:** uncorrected exploratory MC-internal detector-response study. **Primary scope: all matched pairs from the full MC trees; no high-Q² selection.** The in-cone (`FncThetaN<0.75 mrad`) result is a required cross-check, because the documented oo-4.0 FNC simulation response is trustworthy only in that cone.

## Matching and fiducial definition
The first `ChargedTot` row of each `(RunNumber,EventNumber)` is used because FNC branches are event-constant. Truth requires `FncGenNFound=1` and the data-derived face `-30<=FncGenNXfnc<=30 cm`, `-20<=FncGenNYfnc<=28 cm`, excluding the pipe-slot `21<=y<=27.5 cm`, plus the observed reco-phi arc `[6.142, 6.142+6.037]` rad modulo `2pi`. The tree stores one cluster (cluster 0) and one leading gen neutron, so nearest-cluster association is one-to-one within tree content by construction. The loose, tail-preserving association is `FncPresent=1`, `|Delta theta|<1.0 mrad`, and `|wrapped Delta phi|<1.0 rad`; **there is no energy-ratio cut and no MC GoodRun cut.**

## Counts
|sample|events|fiducial gen neutrons|all matched pairs|in-cone matched pairs|match/fiducial truth|
|---|---:|---:|---:|---:|---:|
|RAPGAP-pi 9134|46,292|43,824|12,062|11,771|27.52%|
|DJANGOH14/7091|307,948|53,595|3,638|3,353|6.79%|

The prior cut-based full-sample references were RAPGAP-pi `10,440` and DJANGOH `2,730` pairs, using tighter angular cuts and `0.3<Ereco/Egen<2`. The present loose, no-energy-cut count is intentionally not a like-for-like efficiency change; its change versus those references is reported in the JSON/PDF as a tail-systematic context.

## Per-bin primary results (all matched pairs)
Each entry is `mean ± RMS [core68]`; RMS/core uncertainties are bootstrap SEs in the JSON. `dtheta` is mrad and `dphi` rad. N<50 is flagged sparse.
|Egen [GeV]|N RG/DJ|scale RG ; DJ|dtheta RMS RG ; DJ|dphi RMS RG ; DJ|
|---|---:|---|---|---|
|10-100|0/11|NA ; 3.870 ± 3.459 [2.417]|NA ; 0.364|NA ; 0.565|
|100-200|15/239|1.099 ± 0.260 [0.103] ; 1.211 ± 0.545 [0.173]|0.236 ; 0.240|0.328 ; 0.387|
|200-300|128/488|1.023 ± 0.248 [0.082] ; 1.082 ± 0.277 [0.111]|0.233 ; 0.253|0.366 ; 0.375|
|300-450|1240/863|1.013 ± 0.193 [0.076] ; 1.016 ± 0.219 [0.092]|0.160 ; 0.226|0.325 ; 0.365|
|450-650|5115/1228|1.001 ± 0.164 [0.065] ; 0.967 ± 0.231 [0.095]|0.130 ; 0.230|0.299 ; 0.375|
|650-920|5564/809|1.008 ± 0.126 [0.055] ; 0.955 ± 0.229 [0.096]|0.096 ; 0.229|0.297 ; 0.373|

## Reference and caveats
The RMS-scale panel overlays the descriptive published H1 FNC hadronic reference `sigma(E)/E = 63%/sqrt(E[GeV]) ⊕ 3%` (arXiv:1001.0532); it is not a claim about this MC. Both generators have the documented ideal-axis versus proton-beam-tilt frame mismatch. Residual distributions by true-energy bin (including RAPGAP-pi tail pages) and FncClType secondary checks are in the PDF.

## Artifacts
- PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_resolution_vs_egen_2026-10-21.pdf`
- JSON: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_resolution_vs_egen_2026-10-21_numbers.json`
- Script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/fnc_study/analyse_fnc_resolution_vs_egen_2026-10-21.py`

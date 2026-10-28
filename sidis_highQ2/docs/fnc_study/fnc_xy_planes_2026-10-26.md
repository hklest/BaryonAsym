# FNC face x–y planes

**Date:** 2026-10-26  
**Status:** uncorrected, exploratory; validation PASS

## Purpose and inputs

This study renders the FNC face occupancy in the high-Q² charged-SIDIS `v6fnc` Stage-2 `ChargedTot` trees only.  One event-constant FNC record is retained per `(RunNumber, EventNumber)`; `ChargedTotGen` is not used.  The delivered panels are DATA ep0607 reco, RAPGAP-π 9134 reco, DJANGOH14/7091 reco, the two stored MC truth projections, and one truth–reco contour overlay per MC.

## Failed attempts and gate correction

1. **Attempt 1 (Condor 5891572):** aborted because it assumed `FncX/FncY` existed in `ChargedTot`.  They do not.
2. **Attempt 2 (Condor 5891593):** correctly formed angular coordinates but incorrectly required the DATA 1–99% population quantiles to equal `x=[-30,+22]` cm and `y=[-20,+28]` cm.

The second gate was wrong, not the coordinate derivation.  Those bounds are the data-derived FNC face/aperture edges (including its population hard edges), rather than the expected 1–99% extent of a flux-weighted cluster population.  A physical population should lie within them.

A branch audit found no stored reconstructed FNC position in any `ChargedTot` tree.  The apparent extra position-like fields are DIS variables (`Xe`, `Ye`, `Xda`, `Yda`, `Xes`, `Yes`, `Xh`, `Yh`, `Ys`, `YsGen`, `Y_es_gen`) or scattered-electron variables (`scatEX`, `scatEY`, `scatEZ`).  The MC-only `FncGenNXfnc/FncGenNYfnc` are stored truth projections, not reco positions.

## Coordinate and truth definitions

Reconstructed coordinates therefore use the inverse of the Stage-1 vertex-corrected angular convention,

\[
x=(z_{\rm FNC}-PVz)\tan(\theta_n)\cos\phi_n,\qquad
y=(z_{\rm FNC}-PVz)\tan(\theta_n)\sin\phi_n,
\]

with `z_FNC=10584 cm`, the raw-MODS mean cluster z.  The `PVz` lever arm is retained: its magnitude is only tens of cm compared with 10584 cm, but it is the geometrically consistent inverse for the stored, vertex-corrected angles.  Repeating at 10550 cm changes positions by at most 0.1 cm over this population.

Truth uses stored `FncGenNXfnc/FncGenNYfnc` after `FncGenNFound==1`.  It is the builder envelope for the leading GTR neutron (`E>10 GeV`, `theta<10 mrad`), **not** a detector fiducial.

The DATA cluster median is `(-1.276, +10.654) cm`; the positive y displacement is consistent with the approximately +1 mrad proton-beam tilt present in data and absent in the ideal-axis MC.  It is displayed, not corrected.

## Validation

Reference first-row event/count gates all reproduce exactly: DATA `FncPresent&&FncGoodRun=23285`; RAPGAP reco `15979`, truth `46292`; DJANGOH reco `19954`, truth `107942`.

The corrected DATA containment validation passes.  Its reco 1–99% quantiles are `x=[-16.415,+15.024] cm`, `y=[-9.139,+27.827] cm`; all lie within the face using a 1 cm per-edge tolerance.  Only `3/23285 = 0.0129%` are outside the nominal rectangle, below the 2% ceiling.

The independent DJANGOH raw-position check also passes.  Tree-derived 1–99% quantiles are `x=[-13.984,+9.714] cm`, `y=[-9.796,+13.139] cm`, compared with raw-MODS FNCPart `[-13.6,+10.0]` and `[-9.2,+13.2]` cm.  The four derived-minus-raw edge shifts are `(-0.384,-0.286,-0.596,-0.061) cm`, all below the 2 cm tolerance.

## Artifacts

- Plot PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_xy_planes_2026-10-26.pdf` (7 pages).
- Page PNGs: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_xy_planes_2026-10-26/page_01.png` through `page_07.png` (7; PDF parity PASS).
- Machine-readable counts, branch audit, quantiles, radial fractions, and validation: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_xy_planes_2026-10-26_numbers.json`.
- Producer: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/fnc_study/plot_fnc_xy_planes_2026-10-26.py`; batch job `5891922` completed successfully.

The face and pipe-slot overlays are geometric references only.  In particular, the oo-4.0 FNC MC response remains limited to approximately `r<8–10 cm`; this exploratory rendering is not an acceptance correction.

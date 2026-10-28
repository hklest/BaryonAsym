# v6fnc reco--truth and exploratory correlation study

**Date:** 2026-10-20  
**Inputs:** one DJANGOH14/7091 MC smoke EVL tree (95,021 events) and merged ep0607 data EVL tree (119,483 events).  
**Status:** uncorrected exploratory event-level study; no efficiency, luminosity, radiative, or acceptance correction has been applied.

## Executive outcome

The apparent `97.8%` reco--truth overlap is not supported by the v6fnc output.  In the MC smoke tree,

- `FncPresent=1`: 5,981 events;
- `FncGenNFound=1`: 33,771 events;
- raw same-event intersection: **2,312/5,981 = 38.66%**.

The raw-file cross-check quoted in the task gives 2,443/6,268 = 38.98%, consistent with the smoke result.  The former 97.8% statement has been corrected by a dated erratum appended to the builder record.

## Matching definition and its limitation

`FncGenNFound` is **not** a detector-fiducial truth-match flag.  It selects only the leading GTR neutron satisfying `E>10 GeV` and `theta<10 mrad`.  It is an intentionally broad builder envelope.  The correct analysis-level criterion for a reco-vs-truth comparison is:

1. require a truth neutron in a documented FNC geometrical fiducial (projected FNC position and azimuthal acceptance, including beamline geometry), and
2. use an explicit one-to-one reco--truth association in angle/position and energy.

This cannot be completed from the event-level v6fnc EVL branches alone: there is no stored FNC truth projection, no detector fiducial/map, and no reco--truth association identifier.  Thus, **38.66% is the corrected raw-overlap number, not an analysis match efficiency**.  A defensible FNC-fiducial matched-overlap number is blocked pending a dedicated matching product.  No builder filling bug is established and no change to the existing branch filling is required; the issue is the former analysis-level interpretation/criterion.

For illustration only, using the builder envelope with progressively narrower *polar-angle-only* truth cuts changes the raw reco overlap as follows.  These are not FNC acceptance definitions and must not be treated as matches:

| truth theta upper cut | reco ∩ truth | fraction of 5,981 reco FNC events |
|---:|---:|---:|
| 10 mrad | 2,312 | 38.66% |
| 1.0 mrad | 1,490 | 24.91% |
| 0.8 mrad | 1,393 | 23.29% |
| 0.75 mrad | 1,341 | 22.42% |

The existing documentation's simple `theta_n<0.8 mrad` acceptance shorthand also cannot be applied literally to this sample: the reconstructed FNC theta distribution extends above 0.8 mrad and the stored truth and reconstructed directions are not matched.  A projected detector fiducial is required.

## Reco--truth candidate diagnostics (MC only)

The PDF contains descriptive same-event candidate-pair plots.  To avoid presenting an unvalidated pair as a truth match, the plotted candidate subset is the raw intersection constrained only to the central 0.1--99.9% reconstructed-theta range (`N=1,759`).  Its figures are diagnostic only; the numerical values below are **not calibrated FNC resolutions or energy-scale corrections**.

| diagnostic, candidate subset | result |
|---|---:|
| mean / median `FncE/FncGenNE` | 0.916 / 1.020 |
| RMS of ratio | 0.503 |
| mean / RMS `FncThetaN-FncGenNTheta` | -0.117 / 0.558 mrad |
| mean / RMS wrapped `FncPhiN-FncGenNPhi` | +0.0069 / 1.328 rad |
| vertex mode | `FncGenNVtxMode=1` for all candidates |

For the azimuth residual, wrapping is `DeltaPhi = ((phi_reco-phi_gen+pi) mod 2pi)-pi`, in `[-pi,pi)`.  The broad delta-phi distribution, together with the absence of a detector-fiducial projected truth association, demonstrates why these values must not be promoted to a resolution measurement.

The `FncGenNVtxMode=1` truth vertex is only the event-level simulated-vertex proxy.  No per-neutron production vertex is stored, so its separate angle effect cannot be quantified from the supplied tree.  It is an additional systematic limitation for any eventual angle-resolution claim.

Candidate composition is predominantly neutron/hadron-like: `FncClType=3:1556`, `2:199`, `1:4`; corresponding HAT codes are `FncHtype=1:963`, `2:792`, `3:2`, `4:2`.  The 4,222 reco FNC events outside the diagnostic theta-range candidate set have mean reco energy 223.2 GeV.  They are not rejected physics objects; they are unmatched/ambiguous under the unavailable fiducial association.

## FNC azimuthal coverage and data event-level observables

The FNC reconstructed azimuth is not an approximately uniform full-azimuth detector observable.  In the supplied event-level samples its numeric range spans the wrapped interval because of sparse entries at the boundary, but the distributions are strongly nonuniform and centered in a limited detector coverage sector:

| sample | selected FNC events | phi 1--99% quantile [rad] | median [rad] |
|---|---:|---:|---:|
| MC smoke, `FncPresent` | 5,981 | -3.107 to +3.104 | +1.462 |
| data ep0607, `FncPresent && FncGoodRun` | 23,820 | -3.021 to +3.046 | +1.599 |

These wrapped quantiles are a conservative numeric description; the supplied PDF shows the actual restricted/nonuniform coverage.  It must be retained when viewing any uncorrected FNC phi shape.

The merged data tree has 119,483 events, 23,860 `FncPresent` events, and `FncGoodRun=1` for 95.6973% of all events.  Applying the required FNC-good-run filter retains **23,820** FNC-present data events (40 FNC-present events are removed).  The PDF includes uncorrected unit-area data--MC overlays for reco `FncPhiN`, `FncThetaN`, and `FncE`; they are shape-only overlays and MC is not luminosity-scaled.

## Charged-hadron--FNC delta-phi request: blocker

Neither supplied v6fnc tree contains a charged-track tree, per-track arrays, or branches for charged-track azimuth, charge, `pT`, eta, or z.  The requested all-track / h+ / h- `DeltaPhi(phi_track-phi_n)` distributions therefore cannot be constructed without silently mixing a non-entry-aligned external track product.  Accordingly no data charged-hadron delta-phi result is claimed.  This is the primary blocker for the requested correlation study, and an explicit blocker page is included in the PDF.

A valid follow-up requires an FNC-aware Stage-2 track-tree fork that copies the event FNC variables with the charged tracks (or a proven event identifier/entry-aligned join product).  It should retain `FncGoodRun` on all FNC-related data distributions.

## Artifacts

- PDF: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_reco_true_and_correlations_2026-10-20.pdf`
- Numeric audit sidecar: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/fnc_study/fnc_study_numbers_2026-10-20.json`
- Builder-record erratum: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/fnc_study/v6fnc_builder_change_and_smoke_plan_2026-10-20.md`

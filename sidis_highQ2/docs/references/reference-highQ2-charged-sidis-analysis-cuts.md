# Reference: H1 high-Q^2 charged-particle SIDIS analysis cuts

**Status:** Authoritative cut list for the v5f analysis on the HQ-v5f-r1 (and successor) frozen binning. Supersedes the conflicting cut numbers in any older handoff or memo. This document is the source-of-truth for what cuts were applied to produce every multiplicity, cross-section, asymmetry, and phi_h moment in `H1/sidis_highQ2/docs/data/`.

**Date created:** 2026-05-21 by `h1-sidis-supervisor`.
**Workspace:** `H1/sidis_highQ2/`.

## Two-stage selection

The analysis is two-stage. The UPSTREAM stage is baked into the tree builder (`ChargedSidisTreeBuilderHighQ2.C`) and applies an evls pre-selection that filters events at tree-build time. The DOWNSTREAM stage is applied at analysis-extraction time by the per-script analysis cuts. Downstream values are authoritative; upstream values describe the parent sample from which the downstream analysis is drawn.

## 1. Event-level cuts

### 1.1 Upstream evls pre-selection (tree-level)

| Cut | Value | Applied |
|---|---|---|
| `Q2 > 150 GeV^2` | required | builder |
| `y in [0.1, 0.9]` | required | builder |
| `E - p_z in [45, 65] GeV` | required | builder |
| `|VtxZ| < 35 cm` | required | builder |
| Bunch type | nominal e±p crossings | builder |
| Background finders | standard H1 NBG suite | builder |
| Anti-QED-Compton | rejection | builder |
| Electron detector | LAr (`4° < θ_e < 154°`) | builder |
| FidVolCut | standard `H1ElecFidVolCut` LAr fiducial | builder |

### 1.2 Downstream analysis cuts (authoritative for v5f)

| Cut | Value | Notes |
|---|---|---|
| `Q2 > 150 GeV^2` | analysis | `Q2e` (electron method) = `Q2es` at high Q^2 by H1CalcKine design |
| `Q2 < 20,000 GeV^2` | binning | HQ-v5f-r1 upper edge |
| `0.1 < y < 0.6` | analysis | TIGHTER than upstream's `< 0.9`. Suppresses photoproduction at high y. |
| `35 < E - p_z < 70 GeV` | analysis | WIDER than upstream's `[45, 65]`. Recovers events on both sides. |
| `|VtxZ| < 35 cm` | analysis | unchanged from upstream |
| **Trigger (data only)** | S67: `(L1L2L3ac[2] & (1<<3)) != 0` | data only; MC uses `TrigWeightAC` (no trigger cut) |
| Electron detector | LAr (`4° < θ_e < 154°`) | inherited from upstream |

## 2. Track-level reco cuts (per hadron candidate)

| Cut | Value | Notes |
|---|---|---|
| `pT > 0.15 GeV` (lab) | required | track pT in lab frame, not γ*+p frame |
| `|η| < 1.3` | required | central tracker fiducial |
| `NHitDedx ≥ 10` | required | track quality (dE/dx hit count). Retained as quality cut even though v5f removed dE/dx PID branches. |
| `RadLength > 10 cm` | required | track radial length in CTD |
| `|DcaPrime / cosh(η)| < 2.0 cm` | required | signed DCA in cm, η-corrected |
| `IsFromPrimary == 1` | required | primary-vertex association |
| Track quality | `IsCentralTrk == 1` OR `TrackType in {1, 2}` | central or combined-track classes |

## 3. Gen-level fiducial cuts (matched-gen partner of a reco hadron)

| Cut | Value | Notes |
|---|---|---|
| `GenPt > 0.15 GeV` | required | matches reco fiducial |
| `|GenEta| < 1.3` | required | matches reco fiducial |
| `inFiducialCharged` | true | broad gen fiducial from builder; the analysis cuts above replace it for matched-gen selection |

## 4. Gen-level pool (F9, denominator of the acceptance)

| Cut | Value |
|---|---|
| Species | `|GenPDG| in {211, 321, 2212}` (π±, K±, p/pbar only) |
| V0 daughter veto | `IsV0DaughterGen == 0` |
| Fiducial | as in Section 3 |
| Generator pool | RAPGAP31 + DJANGOH14, averaged for the nominal; the per-bin spread is the model-dependence systematic |

## 5. Reco hadron mask (F11, "Option H")

Composite per-track mask applied bin-by-bin in every data and MC analysis script:

```
IsMatched & IsMatchedHadron
       & ~IsMatchedV0Daughter
       & ~IsMatchedConvOrDalitz
       & ~IsMatchedElectron
       & ~IsMatchedMuon
       & (ParticleSource != 4)
```

`ParticleSource != 4` explicitly excludes the secondary-particle source class.

## 6. Symmetric event-cut prescription (F3)

For acceptance and response numerators, an event contributes only if BOTH the reco event cuts AND the matched-gen event cuts are satisfied. No asymmetric numerator is allowed. Enforced in every acceptance / response / unfolding script.

## 7. Weighting (NOT cuts, but applied per event/track)

| Weight | Where | Value |
|---|---|---|
| MC track weight (F5, F7) | MC only | `weight1 × weight2 × NucliaWeight_clipped` |
| MC trigger weight | NOT applied | MC explicitly does NOT use `TrigWeightAC` |
| Data event weight | data | `TrigWeightAC` only |
| NucliaWeight clip | data + MC | clipped to `[0, 1]` with default 1.0; protons get 1.0 by builder design |
| Per-run beam energy (F13) | data + MC | from Z9 lookup table per run |

## 8. Bin frame and kinematics (F1, F15)

| Variable | Definition |
|---|---|
| Q² | `Q2e` (electron method) = `Q2es` (eSigma) at high Q^2 |
| x | `Xes` (eSigma) |
| z | `z_SIDIS = (P·p_h) / (P·q)` in γ*+p (HCM) frame |
| pT_h | `|p_h × q̂|` in the γ*+p frame, NOT lab pT |
| φ_h | Trento convention |

## 9. Provenance

These cuts are implemented in the following scripts (all under `H1/sidis_highQ2/scripts/`):

- `extract_yields_v5f_3period.py` -- main data yield extractor
- `em06_postrecovery_extract.py` -- em06 data yield extractor
- `abc_dis_2d_response_v5f.py` -- A_BC^DIS 2D extractor
- `extract_data_yields_v5f_HQv5fr1.py` -- 4D data yield extractor
- `acceptance_v5f_charged.py` -- per-bin acceptance
- `assemble_multiplicity_cross_section_v5f.py` -- multiplicity / cross-section assembly
- `compute_asymmetries_v5f.py` -- A_h / A_BC computation

All cuts are mirrored in the `cuts_used` block of the output JSONs:
- `docs/data/multiplicity_v5f_data_HQv5fr1.json`
- `docs/data/cross_section_v5f_data_HQv5fr1.json`
- `docs/data/acceptance_v5f_charged.json`

The upstream builder cuts are documented in the header comment of:
- `source_cache/ProtonTreeBuilderHighQ2.C` (the cached snapshot of the v5b builder source)

## 10. Discrepancies vs older documentation

The handoff `context-handoff.md`, in the P5 frozen-prescription bullet and the kinematic-region section, historically stated `0.1 < y < 0.9` and `E - p_z in [45, 65]`. Those numbers describe the upstream evls pre-selection only. The downstream analysis values (this document, Section 1.2) supersede them for every quoted multiplicity / cross-section / asymmetry / phi_h number in this workspace.

If you find an older memo or plan citing the upstream values as the analysis values, treat the upstream values as the parent-sample selection and the values in this document as the analysis selection. The two are not in conflict; the upstream is a superset of the downstream.

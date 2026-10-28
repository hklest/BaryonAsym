# Side-by-side diff: TreeBuilderHighQ2.C vs g1j_ES AnalysisBase::DoBasicCutsRec

**Date:** 2026-05-22
**Author:** code-editor (Stream C NORAD blocker investigation)
**Status:** INVESTIGATION ONLY -- no source edits applied. See companion
memo `docs/results/g1j_ES_port_v1.md` for the verdict and supervisor
escalation.

## Sources compared

- **Current TreeBuilderHighQ2:**
  `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C`
  function `stage_evls`, lines 240-612 (md5 of file `e1c1d7070333d3feb1bf24cd7cac6d93`).
- **Canonical g1j_ES (Britzger Event Shapes):**
  `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/AnalysisBase.C`
  function `DoBasicCutsRec`, lines 485-772.
- Supporting g1j_ES files inspected:
  `AnalysisBase.h`, `elecCut.{C,h}`, `FidVolCut.{C,h}` (top-level),
  `H1BestKinematics.h`, `DISKinematics.h`, `main.C`.

## Cut-by-cut comparison

| # | Cut | TreeBuilderHighQ2 `stage_evls` (current) | g1j_ES `DoBasicCutsRec` (canonical) | Material delta? |
|---|-----|------------------------------------------|--------------------------------------|-----------------|
| 1 | Run list (data only) | `goodRunList->IsSelected(run)` (lines 373-378) | `fRunList->IsSelected(run)` AND `fDectStatus->IsOn()` AND `*VtxZ > -35 && < 35` (line 549, data branch only) | g1j_ES combines run+detstatus+VtxZ in one boolean for data; TreeBuilderHighQ2 splits them. SAME effective selection (data gets all three; MC skips run+detstatus). NOT material. |
| 2 | Detector status (data only) | `detectorStatus->IsOn()` (lines 380-385) | Folded into the data branch above (line 549) | Same. NOT material. |
| 3 | Trigger S67 (data only) | `gH1Calc->Trig()->GetL1ac(67)` for `!isMC` (lines 388-392) | `TrigCuts = true` -- **S67 trigger cut is DISABLED in this snapshot** (line 582-583, commented out); the commented form is `( GetL1ac(67) \|\| IsMC )` | **CURRENT IS STRICTER.** g1j_ES has the cut disabled in this snapshot version. The TreeBuilderHighQ2 policy (S67 for data only) is the documented H1 NC DIS policy. KEEP CURRENT. |
| 4 | Electron type | -- (not checked) | `gH1Calc->Elec()->GetType() == 1` i.e. LAr (line 561) | **g1j_ES has an extra check** that the scattered electron is in LAr (type 1). On NORAD HATs where the electron is missing, this also fails. Adding it would not save NORAD. Recommended addition for robustness but does NOT fix the NORAD blocker. |
| 5 | Electron E | `eE < 11.0` reject (line 397; `PhysConst::ELEC_E_MIN = 11.0` constant -- NOT year-dependent) | `gH1Calc->Elec()->GetFirstElectron().E() >= 11.0` (line 565, literal 11.0, NOT year-dependent) | **IDENTICAL.** No year lookup in either. The supervisor's diagnosis "H1Constants::Year(1) -> 0 collapses threshold to zero" is FACTUALLY INCORRECT against the current source. |
| 6 | Electron theta max | `eTheta >= 2.7 rad` reject (lines 402-407) | `theta < 2.7` cut is COMMENTED OUT in g1j_ES (line 566) | CURRENT IS STRICTER. The 2.7 rad upper bound is the documented LAr acceptance edge and is standard. KEEP CURRENT. |
| 7 | E-pz | `empz < 45 \|\| empz > 65` reject (lines 410-413) | `Empz > 45 && < 65` (lines 574-575) | IDENTICAL. |
| 8 | Vertex type | `vtOptimalNC \|\| (theta < 30 deg && vtCJC)` (lines 416-425) | `OptimalVtx \|\| (FwdTheta && CJCVtx)` with `FwdTheta = theta < 30 deg` (lines 599-603) | IDENTICAL. |
| 9 | Vertex Z | `\|vtxZ\| < 35` (lines 427-431) | `Vertex()->GetZ() > -35 && < 35` AND `GetHatPrimaryVertexZ() > -35 && < 35` (lines 606-609) | g1j_ES additionally checks `GetHatPrimaryVertexZ()` (HAT-bank stored Z). Minor robustness; not material for non-pathological events. |
| 10 | Zimpact | `(zimp > -190 && < 15) \|\| (zimp > 25)` (lines 433-439) | `(Zimpact > -190 && < 15) \|\| (Zimpact > 25)` (lines 646-648) | IDENTICAL. |
| 11 | Phi octant crack | `(phiOctant > 2 deg && < 43 deg) \|\| (zimpact > 300)` (lines 441-449) | `(PhiOctant > 2 deg && < 43 deg) \|\| (Zimpact > 300)` (lines 653-655) | IDENTICAL. |
| 12 | FidVolCut | `fidVolCut->FiducialVolumeCut()` using `FidVolCut("FidVolCut_HERA2")` and `FidVolCut.steer` (lines 293, 451-455) | `fFidVolCut->FiducialVolumeCut()` using `FidVolCut("FidVolCut_HERA2")` (line 660) | IDENTICAL. The `FidVolCut.{C,h,steer}` files in `H1BaryonAsym_HighQ2/src/` are byte-for-byte the same g1j_ES FidVolCut. (Diff confirmed: line counts 178/46/246 match.) |
| 13 | Q2 | `q2 < 150 \|\| q2 > 20000` reject (lines 457-465) | NOT applied in DoBasicCutsRec; applied later in AnalysisEventShapes::DoAnalysisCutsRec | CURRENT applies in stage_evls. Standard event-list builder pattern. KEEP CURRENT. |
| 14 | y | `y < 0.1 \|\| y > 0.9` reject (lines 466-469) | NOT applied in DoBasicCutsRec (applied analysis-side) | Same as 13. KEEP CURRENT. |
| 15 | ye < 0.94 | `ye >= 0.94` reject (lines 471-476) | `Ye < 0.94` (line 689) | IDENTICAL. |
| 16 | Bunch type | `Event()->GetBunchType() == 3` for `!isMC` with try/catch and static-disable on bank-missing (lines 478-499) | `Event()->GetBunchType() == 3` applied UNCONDITIONALLY (line 693) | CURRENT IS LOOSER on MC (skips); g1j_ES applies on MC too. For NORAD with Bunch=0, the g1j_ES form would reject all events at this cut instead of at the Elec-E cut. Either way NORAD is doomed in any reco-cut path. NOT MATERIAL for NORAD; weak material delta for data (try/catch wrapper saves data runs where the LH1R NDB bank is absent). KEEP CURRENT (more robust). |
| 17 | BG finders | finders {0,1,5,6,7} with HadPt/ElecPt ratio gates (lines 501-523) | Same finders, same ratio gates (lines 700-718) | IDENTICAL. |
| 18 | QED Compton | nEmParts>=2 AND E1>4 AND E2>4 AND (E1+E2)>18 AND -cos(dphi)>0.95 (lines 525-536) | Same logic (lines 733-751) | IDENTICAL. |

## Findings on the year-dependence claim

The supervisor's diagnostic stated that NORAD fails because `H1Constants::Year(1)` returns 0 and "the year-dependent threshold collapses to zero, so electron energy is below threshold." Inspection of the current TreeBuilderHighQ2.C source shows:

- **There is NO year-dependent electron-E threshold in TreeBuilderHighQ2.C.** The threshold is `PhysConst::ELEC_E_MIN = 11.0` (a `constexpr double` in `PhysicsConstants.h` line 72), used as a literal `eE < 11.0` cut.
- **There is NO call to `H1Constants::Year(...)` in `stage_evls`.**
- The `Calibration.h` class `SpacalCalibration` has year-dependent constants (`setDataConstants(year)`) but it is NOT instantiated or called from `stage_evls`. It is only relevant to a SpaCal electron path that is dead code in the high-Q^2 LAr builder.
- The `ElecCut` class in `Calibration.h` (a reimplementation of g1j_ES `elecCut`) is also SpaCal-only and is NOT called from `stage_evls`. It IS instantiated in `stage_lambda` (line 649) and `stage_kaon` (line 1623) but those stages are SpaCal V0 stages, not the high-Q^2 NC DIS event-list stage.

**The supervisor's framing is factually incorrect.** The "Year(1) -> 0" path does not exist in the current source.

## Findings on the real NORAD blocker

Direct ROOT inspection of one NORAD HAT
(`/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000.root`,
HAT tree, 154,913 events) shows:

```
RunNumber : RunType : Q2s : Q2sGen : Ys : YsGen : VtxZ : Epz : ElecE : ElecTheta
        1 :       1 :   0 : 224.08 :  0 : 0.0067 :  0  :   0 :    0  :    0
        1 :       1 :   0 : 173.96 :  0 : 0.0650 :  0  :   0 :    0  :    0
        1 :       1 :   0 : 153.88 :  0 : 0.1081 :  0  :   0 :    0  :    0
        1 :       1 :   0 : 100.82 :  0 : 0.0455 :  0  :   0 :    0  :    0
        1 :       1 :   0 : 178.72 :  0 : 0.8581 :  0  :   0 :    0  :    0
```

**Every reco-side branch is zero on every event.** `ElecE = 0`, `Q2s = 0`,
`VtxZ = 0`, `Epz = 0`, `ElecTheta = 0`. Only the gen-side branches
(`Q2sGen`, `YsGen`, `GenEnElec`, `GenThElec`, `GenPhElec`) carry real
information. This is by NORAD's production design: NORAD samples are
generator-level radiative-correction reference samples, produced without
detector simulation / reconstruction (or with reco data stripped to save
disk).

**Therefore:**
- No re-tuning of any reco cut threshold can rescue NORAD events through `stage_evls`.
- The "Electron E rejected: 154913/154913" diagnostic is a consequence of `ElecE = 0` for all events, not of a wrong threshold value.
- Even if the Electron-E cut were removed, the Q2 cut (`q2 < 150`), the y cut, the vertex cut (`!OptimalVtx && !FwdRegion`), and several others would all reject NORAD events for the same reason (their reco values are all 0).
- The cutflow only reports the FIRST failing cut, which happens to be Electron E in the current cut order.

## Canonical g1j_ES handling of NORAD

`/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/just_run_norad.sh` and `run_test_norad_ep0304.sh` show that Britzger's framework runs ALL NORAD chains with the `-g` (gen-only) command-line flag:

```bash
../bin/x86_64-centos7-gcc9-opt/EventShapes -f Steering/es.NoRadDjango_em06_All.steer -c DjNorad_Eminus06_5 -o testrun.root -n 18000 -g
```

In `main.C` the `-g` flag sets `cmd_gen_only = true`, and at line 167 the entire reco-side block is bypassed (`if ( !cmd_gen_only ) { ... }`). The gen-side observables (`DoCrossSectionObservablesGen`) are filled unconditionally. The reco cuts in `DoBasicCutsRec` are evaluated but their failure is irrelevant when `cmd_gen_only` is true.

This is the architecturally correct pattern for NORAD: bypass reco, compute gen-only observables. NORAD HATs simply do not carry the inputs that any reco-cut path needs.

## Cuts that differ in g1j_ES but should NOT be ported

- **Cut 3 (trigger):** g1j_ES has it disabled in this snapshot. TreeBuilderHighQ2's `S67 for data only` is the documented H1 NC DIS policy.
- **Cut 6 (theta < 2.7):** g1j_ES has it commented out. TreeBuilderHighQ2's enforcement is the documented LAr acceptance edge.
- **Cut 16 (bunch type on MC):** g1j_ES applies unconditionally. TreeBuilderHighQ2's data-only application with try/catch on missing LH1R bank is more robust on real production data.
- **Cuts 13-14 (Q2/y):** g1j_ES defers these to the analysis layer. TreeBuilderHighQ2's pre-selection at the evls stage is the standard event-list-builder pattern.

## Cuts that ARE different in g1j_ES and could be ported safely

- **Cut 4 (electron type == LAr):** Add `gH1Calc->Elec()->GetType() == 1` as the first electron cut. Low risk on data (LAr electron is already implied by S67 + theta cuts). Doesn't fix NORAD.
- **Cut 9 (HatPrimaryVertexZ):** Add `gH1Calc->Vertex()->GetHatPrimaryVertexZ() in [-35, 35]` alongside the existing reco-VtxZ cut. Negligible additional rejection on physical events.

Neither of these would change the NORAD outcome.

## Conclusion

The g1j_ES `DoBasicCutsRec` and TreeBuilderHighQ2 `stage_evls` are functionally equivalent on the cuts that matter for high-Q^2 NC DIS selection (electron E, theta, E-pz, vertex, Zimpact, phi crack, FidVol, ye, BG finders, anti-QED-Compton). Where they differ, TreeBuilderHighQ2 is the more conservative / production-ready of the two (S67 enforcement, theta upper bound, robust bunch-type handling, Q2/y pre-selection).

The NORAD 0/154913 failure is NOT caused by any cut threshold being wrong, NOT by year-dependence (which doesn't exist in the current source), and NOT by a delta versus g1j_ES. It is caused by NORAD HATs having empty reco-side branches by design. The fix is an architectural one: either (a) add a `--gen-only` mode to TreeBuilderHighQ2.C `stage_evls` that bypasses the reco loop and stores only `Q2sGen`/`YsGen`/gen-electron, OR (b) route NORAD HATs through a different upstream stage that consumes only gen-level information. The latter matches the g1j_ES pattern (`-g` flag).

This decision is OUT OF SCOPE for a focused-code-edit task and is being returned to the supervisor for design direction.

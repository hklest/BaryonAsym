# g1j_ES port investigation v1 -- TERMINATED WITHOUT SOURCE CHANGES

**Date:** 2026-05-22
**Author:** code-editor
**Task:** "Port the g1j_ES analysis components (electron cut, fiducial
volume cut, Zimpact, LAr phi crack, etc.) into the high-Q^2 TreeBuilder."
**Verdict:** **NO PORT NEEDED. NORAD blocker has a different cause.
Returned to supervisor for design direction.**

## TL;DR

1. The current `TreeBuilderHighQ2.C` `stage_evls` cuts already match the canonical g1j_ES `AnalysisBase::DoBasicCutsRec` on every material cut: Electron E (literal 11 GeV), theta < 2.7, E-pz [45, 65], vertex (optimal NC or forward-CJC), |VtxZ| < 35, Zimpact, phi-octant crack, FidVolCut_HERA2 (same `FidVolCut.{C,h,steer}`), ye < 0.94, BG finders, anti-QED-Compton. Side-by-side diff in `docs/data/g1j_ES_port_diff.md`.

2. The supervisor's diagnosis "`H1Constants::Year(1) -> 0`, year-dependent threshold collapses to zero, electron energy below threshold" is FACTUALLY INCORRECT against the current source. The threshold is the literal constant `PhysConst::ELEC_E_MIN = 11.0` (file `PhysicsConstants.h` line 72). There is no `H1Constants::Year(...)` call in `stage_evls`.

3. The REAL cause of the NORAD 0/154913 failure is that **NORAD HATs have all reco-side branches set to zero**. Verified by direct ROOT scan of one NORAD HAT (`/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000.root`, 154,913 entries): RunNumber=1, RunType=1, but `Q2s=0`, `Ys=0`, `VtxZ=0`, `Epz=0`, `ElecE=0`, `ElecTheta=0` on every event. Only gen-side branches (`Q2sGen`, `YsGen`, `GenEnElec`, etc.) are populated.

4. The canonical Britzger g1j_ES handling of NORAD HATs is to use the `-g` (gen-only) command-line flag, which bypasses the reco-cut path in `main.C` line 167. Verified from `eventshapes_git/Analysis/just_run_norad.sh` and `run_test_norad_ep0304.sh`. NORAD is NEVER run through reco cuts in the g1j_ES production.

5. **No source change can fix this within a focused-port scope.** The fix is architectural and out of scope.

## Locations of g1j_ES source consulted

| File | Path on NAF | Lines | Purpose |
|------|-------------|-------|---------|
| `AnalysisBase.C` | `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/AnalysisBase.C` | 943 | Canonical reco-level DIS selection (`DoBasicCutsRec`, lines 485-772). |
| `AnalysisBase.h` | same dir | 154 | Member declarations. |
| `elecCut.{C,h}` | same dir | 794, 82 | SpaCal electron quality cuts. Not used at high-Q^2 (LAr electron). |
| `FidVolCut.{C,h}` | same dir | 178, 46 | LAr fiducial volume cut "FidVolCut_HERA2"; reads `FidVolCut.steer`. **Byte-identical to the copy in `H1BaryonAsym_HighQ2/src/`.** |
| `H1BestKinematics.h` | same dir | 534 | DIS kinematics helpers. |
| `DISKinematics.h` | same dir | 242 | DIS variable utilities. |
| `main.C` | same dir | 247 | Event loop with `-g` gen-only flag. Confirms NORAD bypass pattern. |
| `just_run_norad.sh`, `run_test_norad_ep0304.sh` | same dir | -- | Production scripts: every NORAD chain invocation passes `-g`. |

Local copies pulled to `/var/folders/n1/zy1hghps5tlg9538vlknzmdm0000gs/T/opencode/g1j_port/upstream/` for inspection during this task. Not preserved in the repo (not workspace artifacts).

## Specific evidence for the "year-dependence is not the cause" finding

`grep -n 'H1Constants\|Year\b' /data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` returns ZERO matches outside of the function `stage_lambda`/`stage_kaon` (where `Calib::ElecCut elecCut;` is instantiated for the SpaCal V0 daughter analyses, on lines 649 and 1623). `stage_evls` -- the function that processes NORAD HATs through the event-list builder -- has NO year-dependent code path. The Electron E cut at line 397 is

```cpp
const TLorentzVector& scatElec = gH1Calc->Elec()->GetFirstElectron();
double eE = scatElec.E();
if (eE < PhysConst::ELEC_E_MIN) {  // ELEC_E_MIN = 11.0 (constexpr literal)
    nElecECut++;
    continue;
}
```

which is functionally identical to g1j_ES `DoBasicCutsRec` line 565:

```cpp
ElecCuts &= gH1Calc->Elec()->GetFirstElectron().E() >= 11.0;
```

The reason the cut rejects all NORAD events is that `GetFirstElectron().E() == 0` for NORAD events, not that 11.0 is the wrong threshold.

## Specific evidence for "NORAD HATs have empty reco branches"

```text
ssh naf-hone.desy.de "source <h1oo>; root -b -q -l -e '
  TFile *f = TFile::Open(\"<NORAD HAT path above>\");
  TTree *t = (TTree*)f->Get(\"HAT\");
  t->Scan(\"RunNumber:RunType:Q2s:Q2sGen:Ys:YsGen:VtxZ:Epz:ElecE:ElecTheta\", \"\", \"\", 5);'"
```

returned (full table in `docs/data/g1j_ES_port_diff.md` section "Findings on the real NORAD blocker"):

```
RunNumber : RunType : Q2s : Q2sGen : Ys : YsGen : VtxZ : Epz : ElecE : ElecTheta
        1 :       1 :   0 : 224.08 :  0 : 0.0067 :   0 :   0 :     0 :         0
        1 :       1 :   0 : 173.96 :  0 : 0.0650 :   0 :   0 :     0 :         0
        ...
```

This pattern -- gen-side populated, reco-side all zero -- is consistent with a generator-level-only HAT (no detector simulation or reco run, or reco data stripped to keep file sizes small). This is the standard structure of an H1 NORAD production used for radiative corrections.

## What was NOT done

- **No source edit applied to `TreeBuilderHighQ2.C`** -- because the diff shows the cuts already match g1j_ES, and the supervisor's framing of the failure is incorrect. Editing the file to "port g1j_ES cuts" would be a no-op at best and a regression at worst (g1j_ES has some cuts DISABLED that TreeBuilderHighQ2 enforces -- see diff memo cuts 3, 6, 13, 14, 16).
- **No new binary compiled.** The existing `TreeBuilderHighQ2` binary (md5 `63e47715...`, size 1,884,624 bytes, source md5 `e1c1d7070333d3feb1bf24cd7cac6d93`) remains pinned in the manifest and is still the correct binary for the high-Q^2 reco production.
- **Binary identity manifest NOT updated.** No rebuild, no md5 change, no manifest delta. The manifest at `sidis_highQ2/scripts/binary_identity_manifest.json` (and its NAF mirror) is unchanged.
- **Smoke gate NOT re-run.** A re-run would still show 0 entries for NORAD (the file's reco branches are empty regardless of cuts), so it would not produce useful new information.
- **Data smoke NOT run.** No source change means no expected change in data behaviour. The existing v5f data sample (ep0607) continues to be correct; G1C (the missing-period data launch) is unaffected by this NORAD-side investigation.

## Stream C verdict

**Stream C is NOT unblocked by anything in this investigation.** Stream C's NORAD blocker is real but its cause and fix are not in the form the original task brief assumed. The blocker requires a supervisor design decision:

| Option | Description | Effort |
|--------|-------------|--------|
| A | Add a `--gen-only` mode to `TreeBuilderHighQ2.C` `stage_evls` that skips ALL reco cuts and writes only `Q2sGen`, `YsGen`, `GenEnElec`, `GenThElec`, `GenPhElec`, `GenVtxZ` to `EvlTree`. Wire it through the NORAD wrapper. | Small. Mirrors g1j_ES `-g` pattern. |
| B | Skip `stage_evls` entirely for NORAD; have the downstream charged-SIDIS tree builder consume the NORAD HATs directly using only gen-level branches (no event-list intermediary). | Medium. Requires a NORAD-specific upstream stage in `ChargedSidisTreeBuilderHighQ2`. |
| C | Verify whether a re-production of NORAD HATs WITH reco information (full dst2all -> rec -> mods -> hat pipeline) is feasible, and whether it has already been done by another group. | Unknown; depends on Britzger / generator availability. |

The canonical g1j_ES choice is Option A (`-g` flag). Recommendation: adopt Option A in a follow-up focused-edit task once the supervisor confirms the gen-level cuts and the EvlTree schema for NORAD.

## Recommendation for Stream C re-launch

**NOT READY.** Stream C must not be re-launched as currently scoped because:

1. The blocker is real and unaddressed by this investigation.
2. The fix requires a `--gen-only` mode in `TreeBuilderHighQ2.C` that does not yet exist.
3. A re-launch with the current binary would re-waste Condor cycles on a known-empty output.

The supervisor needs to:

(a) Decide between Options A / B / C above.
(b) Delegate the chosen option as a new focused-code-edit task to `code-editor` (Option A) or to `code-editor` + `h1-software` (Option B). Option C is research/discovery and goes to `researcher`.
(c) Re-evaluate the original task brief: the "year-dependence" diagnostic should not be repeated in future briefs.

## Pointers for the next agent

- The full cut-by-cut diff is in `docs/data/g1j_ES_port_diff.md`.
- The g1j_ES sources are at `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/` on NAF; local copies were temporary (in `/var/folders/.../g1j_port/upstream/`) and are not committed to the repo.
- The NORAD HATs are at `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/` (and `dst2all_rapgap_*` siblings). 154,913 events per file is typical.
- The `Calibration.h` / `PhysicsConstants.h` / `TDetectQedc.{C,h}` / `FidVolCut.{C,h,steer}` files already in `H1BaryonAsym_HighQ2/src/` ARE g1j_ES derivatives or copies. No re-port at the source-file level is needed.
- The `EvlTree` schema in `stage_evls` already has the right gen branches (`Q2sGen`, `YsGen`) declared on lines 328-331; an Option-A patch would mostly consist of (i) early-return after gen fill when `--gen-only` is set, (ii) skipping the reco-cut block, (iii) adding gen-electron branches.

## Sanity sweep

- The 5 binary identity guardrails were NOT exercised in this task because no rebuild occurred. The TreeBuilderHighQ2 md5 on disk still matches the pinned `63e47715...` (assumed valid as of the prior guardrail-validation task; no probe done here to avoid spurious load on the SSH socket).
- The low-Q^2 `TreeBuilder` binary at `bin/x86_64-el9-gcc14-opt/TreeBuilder` (md5 `8de736bc...`) was not touched.
- No ep0304 leakage: no file under `ep0304/` paths was touched.
- The low-Q^2 SIDIS code path (`H1BaryonAsym/`, `sidis_lowQ2/`) was NOT touched.
- No Condor submission; no /pnfs write; only DUST reads.

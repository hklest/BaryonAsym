# Research: H1 Forward Neutron Calorimeter (FNC) Data Availability in H1OO 4.2.1 and the SIDIS High-Q2 Input

- **Date:** 2026-10-19
- **Author:** researcher subagent (FNC availability investigation)
- **Scope:** Whether FNC data is (a) accessible in the H1OO framework used by this analysis (relvol10 / 4.2.1), and (b) actually present and non-empty in the current high-Q2 charged-particle SIDIS (Q2 > 150 GeV^2) input data (HERA-II 2005-2007 + ep0304, `oo-4.0` hat/mods production over DST7), including trigger information, and what code changes would be needed to use it.
- **Method:** H1OO 4.2.1 source reading on AFS + direct single-chunk inspection of the production input files on the NAF login node (plain ROOT 6.40.04, streamer-info emulation; each check a single input file, well under 5 min wall).
- **Supersedes:** nothing; first FNC availability record in this workspace.

---

## 1. Executive verdict

| Question | Verdict |
|---|---|
| FNC support in H1OO 4.2.1? | **YES — complete.** Banks **FNCL** (cluster) and **FNCE** (cell) with C++ accessor classes; reconstruction chain `H1FindFNC` -> `H1PartFNC` -> MODS branch `FNCPart`; HAT branch `PartFNC`; calculator `H1CalcFnc` via `H1Calculator::Fnc()`; run-quality class `H1FNCQuality`; detector-status bit `H1DetectorStatus::SetFNCOn()`. There is **no bank named "FNCK"**. |
| FNC present in the SIDIS high-Q2 input files? | **YES, at MODS/HAT level, for FNC-good runs.** The online `oo-4.0` hat/mods production already ran the FNC finder. In the inspected FNC-good chunk (`c070005`, ep0607, runs from 492991) the MODS `FNCPart` array is non-empty in **139,529 / 752,213 events (18.55%)**, with exactly one FNC particle per event, mean energy ~215 GeV. The HAT `PartFNC` type code agrees exactly (same 18.55% nonzero). |
| FNC absent anywhere? | **YES for `H1FNCQuality`-excluded runs.** Chunk `c070000` (runs 492559-492577, entirely inside the excluded range 492559-492832, "BR10/FNC DAQ problems or FNC not in readout") has `FNCPart` empty in **100%** of 682,562 events and `PartFNC == 0` everywhere. |
| Raw FNCL/FNCE bank access from the current inputs? | **NO.** The current steering loads only HAT+MODS (`fLoadODS = false`); the raw banks exist only in the DST7 files, and H1OO cannot read BOS DST directly (ROOT-format ODS files do not exist for this data). This is **not needed** for FNC particle access — the MODS `FNCPart` already carries the finder output. |
| FNC trigger bits in the input? | **Raw trigger words: YES.** The HAT contains all 256 raw L1 trigger-element bits (`Il1te`), all 128 subtrigger raw/actual bits (`Il1rw`, `Il1ac`, `ITrigac`), L2/L3/L4 words — the same mechanism the analysis already uses for S67 (`GetL1ac(67)`). **No FNC-specific trigger code or element names exist in H1OO 4.2.1**; identifying the FNC trigger element/subtrigger numbers requires the H1 trigger database (run-dependent). FNC-trigger live-time luminosity is **not** in the input files. |
| Current builder opens FNC? | **No** — neither Stage 1 nor Stage 2 references FNC at all. Access is a small additive change at Stage 1 (see §7). |

**Bottom line:** FNC reconstructed-neutron/photon information is available in H1OO and is already inside the current SIDIS high-Q2 input MODS files (for FNC-good runs) — no DST reprocessing is required to read it. What is *not* available from these inputs: multi-cluster FNC events (only cluster 0 kept), FNC clusters below 10 GeV (finder cut applied at production), cell-level FNCE information, and FNC-specific trigger/luminosity handling.

---

## 2. H1OO release and reference source location

- The analysis builds against **H1OO relvol10 / 4.2.1**: `source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh` (workspace `AGENTS.md`; confirmed — the script sets `H1CURRENT=4.2.1`, LCG_107a, `PLATFORM=x86_64-el9-gcc14-opt`, ROOT 6.34.04).
- Reference source tree used for this research: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/` (readable directly on AFS).
- **Note:** the `h1oo-master/` path named in the workspace-root `AGENTS.md` **does not exist**. `/data/dust/group/h1/klesthen/BaryonAsym/h1oo-build/` contains only `bin/` and `lib/` (no sources). The AFS release tree above is the actual reference source.
- Compiled H1OO libs (used by the package build): `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/lib/x86_64-el9-gcc14-opt/` (prepended to `LD_LIBRARY_PATH` by `thish1.sh`).

### Operational caveat discovered (login node)

On this NAF login node the H1OO stack **cannot currently be loaded interactively**:

- LCG ROOT 6.34.04 binary fails: `root: /lib64/libstdc++.so.6: version 'GLIBCXX_3.4.31' not found (required by root)` (system libstdc++ is gcc 11 / EL9).
- The H1OO shared libraries fail to load into system ROOT 6.40.04: `GLIBCXX_3.4.32 not found (required by .../libH1OOBanks.so, libH1Mods.so)`.
- Also, `thish1.sh` only proceeds when `H1CURRENT=current` is pre-set and `GROUPROOT`/`H1_ROOT` are non-empty (`desygroupset` is not on the non-interactive PATH; manual `export GROUPROOT=/afs/desy.de/group/h1; export H1_ROOT=/afs/desy.de/group/h1; export H1CURRENT=current` works).
- Workaround used here: **plain system ROOT 6.40.04 + streamer-info emulation** (`TClonesArray` reads and `TTree::Draw("FNCPart.fFncEne")`-style member access work without H1OO dictionaries because the MODS files carry `TStreamerInfo` for `H1PartFNC`). Condor jobs are unaffected (they run the full H1OO environment).

---

## 3. FNC in the H1OO framework (exact names, paths, quantities)

### 3.1 Banks

| Bank | Content | H1OO accessor / row class | Notes |
|---|---|---|---|
| **FNCL** | FNC **cluster** bank ("data 2001-20xx") | `H1OOBanks/H1FnclBankPtr.{h,C}`; row class `H1FnclRow` in `H1OOBanks/ODS/H1Fncl.h` | Columns (getters): `Cltype` (I, 1-3), `Ecl` (F, cluster energy), `Xcl`, `Ycl`, `Zcl` (F, position; Zcl ~ 10550-10650 cm), `Timetof`, `Timecell`, `Ecra` (cluster radius), `Corene` (core-energy fraction E(4)/E), `Secmom`, `Est4` (energy in main calorimeter), `Quality` (I), `Calocell`, `Preshcell` |
| **FNCE** | FNC **cell** bank ("data 2001-20xx") | `H1OOBanks/H1FnceBankPtr.{h,C}`; row class `H1FnceRow` in `H1OOBanks/ODS/H1Fnce.h` | Columns: `Eelm`, `Ehad`, `Time`. Continuous cell numbering rows 1-32: MainCalo 1-8, modules I-IV 9-32 |
| **FNHT** | FNC cluster bank, **1998-2000 (HERA-I) setup only** | no generated row class; generic `H1BankPtr fnht("FNHT")` (used in `H1FindFNC::Find1998_2000`) | DDL exists at `H1Banks/fnc/fnht.ddl` (git-lfs pointer, not checked out on AFS) |
| **FNCR** | FNC raw/other bank | none (DDL only: `H1Banks/fnc/fncr.ddl`) | listed in the ODS/RED bank lists, no C++ class |
| ~~FNCK~~ | **does not exist** | — | grep over the full 4.2.1 tree finds no "FNCK" |

Bank-list membership (standard DST content as far as H1OO is concerned):

- `H1Ods/H1SteerOdsEvent.C:27-32` — default ODS bank list includes `FNHT, FNCR` and `FNCE, FNCL` (i.e. H1OO expects these in standard DSTs; they are read whenever ODS content is loaded from DST).
- `H1Red/h1red.steer:10` — DST bank list for the RED/BOS chain includes `FNHT, FNCR, FNCL` (not FNCE).
- The farm reconstruction output bank definitions (`H1Banks/farm/*.ddl`) contain **no FNC banks** — FNC reconstruction is not part of the central farm DST banks; the FNCL/FNCE banks are FNC-specific additions present in the DST data stream.

### 3.2 Reconstruction chain

- **Finder:** `H1Finder/H1FindFNC.{h,C}` (author A. Uraev 2002; HERA-II update 2007-09-25).
  - HERA-II path `Find2000_20xx` (`H1FindFNC.C:69-109`): reads `H1BankPtr fncl("FNCL")`, takes **only cluster row 0**, maps columns: `fClType=col0, fEnergy=Ecl, fFncX/Y/Z=Xcl/Ycl/Zcl, fTimeToF=Timetof, fTime=Timecell, fEcra=Ecra, fCorEne=Corene, fSecMom=Secmom, fEMainCalo=Est4, fQuality=col11`; applies a single cut **`fEnergy > 10 GeV`**; charge extracted as `fQuality & 1`.
  - HERA-I path `Find1998_2000` (`H1FindFNC.C:112-201`): reads FNHT, loops over clusters, shifts x/y (`fncx-1.6`, `fncy+4.8`), cuts `E in (10,1200) GeV`, `charge==0`, fiducial `|y-2.5|<7`, `|x+2.5|<8`.
- **Particle class:** `H1Mods/H1PartFNC.{h,C}` — members `fFncX, fFncY, fFncZ, fFncEne, fClType` (comment: `2,3 - Neutron, 1 - Photon`), `fQuality`, `fTimeToF`, `fTime`, `fTail` (HERA-I), `fCorEne, fEcra, fSecMom, fEMainCalo` (HERA-II). Methods: `GetE()`, `GetEPrime()` (= raw `fFncEne`), `GetMass()` (0 for ClType==1, else neutron mass via TDatabasePDG), `GetP() = sqrt(E^2-M^2)`, `GetTheta() = sqrt(x^2+y^2)/10550` (FNC at z = 105.5 m), `GetPt()`, `GetPx/Py/Pz()`, `GetPhi()`, plus the estimator getters.
- **Array pointer:** `H1Mods/H1PartFNCArrayPtr.{h,C}`, default branch name **`"FNCPart"`**.
- **MODS schema:** `H1Mods/H1ModsEvent.C:60-71` — the MODS tree's particle branches are `Particles, SelectedTracks, EmParticles, Muons, MCParticles, FNCPart, FPSPart` (FNCPart is a standard MODS branch, `TClonesArray` of `H1PartFNC`).
- **MODS filler:** `H1Filler/H1ModsFiller.C:139` calls `FillFNC(modsevent)` **unconditionally** in `Fill()`; `FillFNC` (`:296-303`) runs `H1FindFNC`. So every hat/mods production (including `oo-4.0`) attempted FNC reconstruction.

### 3.3 Calculator (the easy user-facing access)

- `H1Calculator_and_Weights/H1CalcFnc.{h,C}` (constructed in `H1Calculator.C:94`; accessor `H1Calculator.h:149` — `H1CalcFnc* Fnc()`).
- Reads `H1PartFNCArrayPtr fFncArray("FNCPart")` (`H1CalcFnc.C:28`) — i.e. it connects to the MODS `FNCPart` branch; **works with hat+mods input, no ODS needed**.
- Getters: `GetIndex()` (0 = one FNC particle present; -1 = none), `GetMass(), GetZ(), GetPt(), GetP(), GetPhi(), GetTheta(), GetTail(), GetTime(), GetQuality(), GetClType(), GetCorEne(), GetEPrime(), GetE(), GetEcra(), GetNeutron()` (TLorentzVector), `GetClusterPosition()` (TVector3), and `GetHtypeFNC()` which reads the HAT `PartFNC` word.
- Generic interface wrappers: `H1Calculator_and_Weights/H1CalcGenericInterface.h:40-47` (`GetFnc_Index`, `GetFnc_Mass`, ...).

### 3.4 HAT-level FNC flag

- `H1Hat/H1HatEvent.h:202` — `Short_t fPartFNC; // FNC particle`; branch `PartFNC` written at `H1HatEvent.C:1190`, read at `:261-264`.
- Semantics (from the filler, `H1Filler/H1HatFiller.C:1136-1141`):
  - `0` = no FNC particle;
  - `1` = ClType>1 (hadron/neutron-like) and Quality==0 (good);
  - `2` = ClType>1 and Quality!=0 (bad hadronic);
  - `3` = ClType==1 (EM/photon-like) and Quality==0 (good EM);
  - `4` = ClType==1 and Quality!=0 (bad EM).
  (Matches the `H1CalcFnc::GetHtypeFNC()` comment.)

### 3.5 Run-level FNC quality and detector status

- `H1Geom/H1FNCQuality.{h,C}` (V. Dodonov, 2008): "runs with problems in BR10/FNC DAQ or with FNC not included in readout. **Only HERA-II runs provided.**" — 63 excluded run *ranges* + 68 individually excluded runs, covering run numbers 335574 through 505238 (i.e. the full HERA-II range used by this analysis, including ep0304 through ep0607). Interface: `H1FNCQuality::IsRunGood(int runnr)`.
  - Example ranges relevant to the inspected data: `492559-492832`, `494518-494520`, `497802-497807`, `505155-505238`.
- `H1Geom/H1DetectorStatus.C:675-682` — `SetFNCOn()` selects the FNC HV bit 37 / CAEN channel 26 for detector-status (per-run good/bad) selection.

### 3.6 Trigger model (generic; no FNC-specific code)

- `H1Calculator_and_Weights/H1CalcTrig.{h,C}`: arrays `fL1te[256]` (raw L1 trigger **elements**), `fL1rw[128]` / `fL1ac[128]` (subtrigger raw / actual), `fL2te[96]`, `fL2rw[96]`, `fL3rw[48]`, `fL1L2L3ac/ov[128]`, `fTrigac[128]`, `fClass[32]`, `fL4dec`; getters `GetL1te(i)`, `GetL1rw(i)`, `GetL1ac(i)`, `GetTrigac(i)`, `GetPrescaleWeight()`, etc. Enable-then-read pattern.
- In the HAT file these are the branches `Il1te[256], Il1rw[128], Il1ac[128], Il2te[96], Il2rw[96], Il1l2rw[128], Il3rw[48], Il1l3rw[128], Il1l2l3ac[128], Il1l2l3ov[128], ITrigac[128], L4dec, Iclass[32]` (`H1Hat/H1HatEvent.C:1145-1162`).
- Run-dependent subtrigger metadata (prescales, enabled subtriggers, trigger phase, L4 mode): `H1Geom/H1TrigInfo.{h,C}` (from the H1 NDB; `H1_DBPAR` set by `thish1.sh`).
- **There is no FNC-specific trigger element name, subtrigger constant, or weight class anywhere in H1OO 4.2.1** (searched `etfnc`, `fnc` in all trigger code, `H1CalibTrigger/`, `H1CaloTrigger/`, trigger-weight steers). The published H1 forward-neutron measurements used dedicated FNC triggers; their element/subtrigger numbers are run-dependent and live in the H1 trigger database, not in the framework code.

---

## 4. The SIDIS high-Q2 input data model (v6 production)

- Workspace: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`; Stage 1 source `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`; Stage 2 `src/ChargedSidisTreeBuilderHighQ2_v6.C`.
- Stage 1 steering (event-list production), e.g. `steering_v6/central/evls/data/07/evls_v6_d07_0000.steer`:
  ```
  H1SteerTree() {
     fHatFiles  = "/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/hat.4.0.6.10dst7.c070000.root";
     fModsFiles = "/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/mods.4.0.6.10dst7.c070000.root";
     fLoadODS   = false;
  }
  ```
  i.e. the input is **HAT+MODS pairs** from the official H1 OO production `oo-4.0` (reconstruction `4.0.6.10dst7`), **no ODS/bank-level input**.
- Period directories and EVLS steer-file counts: `03` (20), `04` (63), `05` (152), `06` (313), `07` (112) — 660 steer files. SIDIS period mapping: ep0304 = 03+04 (83 files), em0405 = 05 (152), em06 + ep0607 = 06 + 07 (handoff counts: em06 100, ep0607 323; 658 data files total per `context-handoff.md`). The `06` OO directory mixes the 2006 e- and e+ chunks; `07` is the 2007 e+ data. (`07low`/`07med` pnfs directories are further luminosity-splits of the same production, chunks c070111-c070348.)
- Stage 1 trigger requirement: **CUT 2 "Trigger S67"** — `gH1Calc->Trig()->GetL1ac(PhysConst::TRIGGER_L1_BIT)` with `TRIGGER_L1_BIT = 67` (`src_stage1/PhysicsConstants.h:79`; cut at `TreeBuilderHighQ2_chargedsidis_v6.C:694-696`).
- Stage 2 consumes **only** the Stage-1 EVL files (`fEventListFiles`) — it never re-opens hat/mods, so any FNC content must enter at Stage 1.
- Neither builder contains any FNC reference today (`grep -i "fnc|neutron"` over both sources: zero hits).

---

## 5. Direct inspection of the input files (login-node checks)

All checks with plain ROOT 6.40.04. MODS tree name is `MODS`; HAT tree name is `HAT`. File pair = one input chunk.

### 5.1 Chunk c070000 (ep0607, 2007 e+) — FNC-EXCLUDED runs

- Files: `mods.4.0.6.10dst7.c070000.root` (1,237,609,311 B), `hat.4.0.6.10dst7.c070000.root` (327,685,405 B), both under `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/`.
- MODS tree: 682,562 entries; top-level branches include `FNCPart` (TClonesArray of `H1PartFNC`, 1 leaf, not split; branch filled 682,562 times; `TotBytes=44,372,188`, `ZipBytes=1,783,152` — the per-event empty-array overhead).
- **`FNCPart` non-empty in 0 / 682,562 events.** Control on the same file: `Particles` non-empty in 1,985 / 2,000 scanned events (technique validated).
- HAT `PartFNC` (Short_t): **0 for all 682,562 events**.
- Run composition (MODS `RunNumber` branch): 14 runs, 492559-492577 — **entirely inside the `H1FNCQuality` excluded range 492559-492832** ("FNC not included in readout / BR10 DAQ problems"). This chunk is therefore *not* representative for FNC availability; its emptiness is the expected behavior for FNC-bad runs, not evidence of missing FNC support.

### 5.2 Chunk selection for FNC-good runs

First-entry `RunNumber` probes (one GetEntry per file, metadata-level):

| chunk | first run | FNC status vs `H1FNCQuality` |
|---|---|---|
| c070005 | 492991 | good (past excluded range 492559-492832) |
| c070010 | 493266 | good |
| c070020 | 493921 | good |
| c070030 | 494811 | good |
| c070040 | 496362 | good |
| c070050 | 497172 | good |

### 5.3 Chunk c070005 (ep0607, runs from 492991) — FNC-GOOD runs: **FNC DATA PRESENT**

- File: `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/mods.4.0.6.10dst7.c070005.root`, MODS tree 752,213 entries.
- **`FNCPart` non-empty in 139,529 / 752,213 events = 18.55%**; exactly one FNC particle per event (`entries with >1 FNC particle: 0` — consistent with the finder storing only cluster 0).
- Member data read via streamer-info emulation (`TTree::Draw("FNCPart.<member>")`, 139,529 values each):

| member | mean | RMS | meaning |
|---|---|---|---|
| `fFncEne` | 215.4 GeV | 234.7 | FNC particle energy (finder cut E > 10 GeV; histogram 0-1200 GeV has 0 underflow) |
| `fFncX` | -0.98 cm | 6.06 | hit x |
| `fFncY` | 9.87 cm | 8.67 | hit y |
| `fFncZ` | 10,601 cm | 39 | ~106 m from IP — the FNC position (cf. `GetTheta()` uses 10550 cm) |
| `fClType` | 2.19 | 0.60 | 1=photon-like, 2/3=neutron-like |
| `fQuality` | 3.17 | 2.23 | reconstruction quality word |
| `fCorEne` | 0.208 | 0.293 | core-energy fraction E(4)/E (HERA-II n/gamma estimator) |
| `fEcra` | 14.4 | 352.7 | cluster radius (outliers present — needs QA before physics use) |
| `fTime` | 223.3 | 21.6 | cluster time |
| `fEMainCalo` | 136.6 GeV | 168.4 | energy in FNC main calorimeter |

- HAT cross-check, `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/hat.4.0.6.10dst7.c070005.root` (752,213 entries), `PartFNC` distribution:
  - `0` (no FNC): 612,684
  - `1` (good hadronic): 28,377
  - `2` (bad hadronic): 96,941
  - `3` (good EM): 8,477
  - `4` (bad EM): 5,734
  - nonzero fraction **0.185491 — exactly matches** the MODS `FNCPart` non-empty fraction (139,529/752,213).

### 5.4 What the presence of FNCPart proves about the DST

The `oo-4.0` production demonstrably ran the **HERA-II** FNC finder reading the **FNCL** bank (the stored HERA-II estimators `fCorEne/fEcra/fSecMom/fEMainCalo` are only filled by `H1FindFNC::Find2000_20xx`). Hence FNCL was present and readable in the DST7 input for FNC-good chunks. Direct DST-level FNCL verification was **not** performed (BOS tools not practical inside the login-node budget); it is also not needed for the availability question at MODS level.

---

## 6. Trigger and luminosity information relevant to FNC

- The full raw trigger content needed to evaluate **any** HERA-II trigger decision is in the input HAT: `Il1te[256]` (all raw L1 trigger elements), `Il1rw/Il1ac[128]` (subtrigger raw/actual), `ITrigac[128]` (L1xL2xL3xL4), `Il2te/Il2rw`, `Il3rw`, `L4dec`, `Iclass[32]`. The analysis already reads `GetL1ac(67)` this way.
- Therefore, if the FNC trigger **element bit number(s)** / **subtrigger number(s)** for the relevant runs are known (H1 trigger database; the published H1 forward-neutron analyses' trigger definitions), the FNC trigger decisions are recoverable from the current input with no reprocessing — e.g. `gH1Calc->Trig()->GetL1ac(n)` / `GetL1te(bit)` after `EnableL1acTrigger(n)`.
- **Not available in the input files:** FNC-trigger live times / per-subtrigger luminosity. That information lives in the H1 luminosity system (run-list/lumi DB, cf. `LumiFiles/` usage in this workspace and `H1Geom/H1TrigInfo` prescales); any FNC-trigger-normalized measurement would need a dedicated luminosity derivation.
- **No FNC-specific trigger code exists in H1OO 4.2.1** (verified by search; §3.6).

---

## 7. Code changes needed to access FNC from the sidis_highQ2 analysis

**Nothing at file/steering level is required to make the data readable** — the MODS `FNCPart` branch is already part of every loaded MODS file, `H1Calculator` already constructs `H1CalcFnc`, and `fLoadODS` stays `false`. The only changes are additive Stage-1 code + EVL branches, e.g. (following the existing `t_TrigS67` branch pattern at `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:465-489`):

```cpp
#include "H1Calculator/H1CalcFnc.h"   // or use the already-linked H1Calculator::Fnc()

// per event (inside the reco loop, after the existing gH1Calc reads):
Int_t   fncIdx     = gH1Calc->Fnc()->GetIndex();     // 0 = FNC particle present, -1 = none
Float_t fncE       = fncIdx >= 0 ? gH1Calc->Fnc()->GetE()       : -1.;
Float_t fncX       = fncIdx >= 0 ? gH1Calc->Fnc()->GetX()       : -999.;
Float_t fncY       = fncIdx >= 0 ? gH1Calc->Fnc()->GetY()       : -999.;
Int_t   fncClType  = fncIdx >= 0 ? gH1Calc->Fnc()->GetClType()  : -1;
Int_t   fncQuality = fncIdx >= 0 ? gH1Calc->Fnc()->GetQuality() : -1;
Float_t fncCorEne  = fncIdx >= 0 ? gH1Calc->Fnc()->GetCorEne()  : -1.;
Float_t fncEcra    = fncIdx >= 0 ? gH1Calc->Fnc()->GetEcra()    : -1.;
// also available: GetZ(), GetTime(), GetTheta(), GetNeutron(), GetClusterPosition(), GetEMainCalo()...

evlTree->Branch("fncIdx", &fncIdx, "fncIdx/I");  // ... etc.
```

- The HAT type code is alternatively available as `gH1Calc->Fnc()->GetHtypeFNC()` (reads `PartFNC`).
- **Recommended vehicle:** an additive Stage-1 fork (the proven `v6bsa` pattern — forked source, separate binary, read-only reuse of steering) rather than touching central `v6`; Stage 2 then reads the new EVL branches. Re-running Stage 1 over the 658 data chunks is a Condor campaign.
- **Run-quality guard:** apply `H1FNCQuality::IsRunGood(runnr)` (H1Geom) or, per event, simply treat `fncIdx == -1` as "no FNC information" (FNC-excluded runs produce exactly this). For MC, expect `fncIdx == -1` everywhere unless the MC hat/mods carry FNC simulation (not checked — see §9).
- **If raw-bank information (multi-cluster FNCL, sub-10-GeV clusters, FNCE cell energies) were ever needed:** that would require a new ODS/hat/mods production from the DST7 files with FNCL/FNCE in the bank list (H1OO cannot read BOS DST directly; `H1SteerTree` supports only ROOT-format HAT/MODS/ODS/USER/EventList files — `H1Skeleton/H1SteerTree.h:120-127`). This is a major production campaign and is *not* needed for standard leading-neutron tagging.

### Content limitations inherited from the production-time finder (important for physics use)

1. **Only FNCL cluster 0 is stored** (`H1FindFNC.C:73-91` reads `fncl[0]` only) — multi-cluster events are truncated to the first cluster; published FNC analyses that loop over clusters cannot be exactly reproduced from MODS.
2. **Hard E > 10 GeV cut at production time** — lower-energy FNC clusters are irrecoverable from hat/mods.
3. **FNC cell-level (FNCE) information is absent** from MODS/HAT entirely.
4. **`H1FNCQuality`-excluded runs have no FNC content** (verified on chunk c070000).
5. The `fEcra` member shows large outliers (RMS 352 vs mean 14) in the checked chunk — estimator QA needed before cuts are built on it.

---

## 8. Files and paths searched / inspected (audit trail)

H1OO 4.2.1 source (all under `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`):
`H1OOBanks/H1FnclBankPtr.{h,C}`, `H1OOBanks/H1FnceBankPtr.{h,C}`, `H1OOBanks/ODS/H1Fncl.{h,C}`, `H1OOBanks/ODS/H1Fnce.{h,C}`, `H1Banks/fnc/{fncl,fnce,fncr,fnht}.ddl`, `H1Finder/H1FindFNC.{h,C}`, `H1Mods/H1PartFNC.{h,C}`, `H1Mods/H1PartFNCArrayPtr.{h,C}`, `H1Mods/H1ModsEvent.C`, `H1Calculator_and_Weights/H1CalcFnc.{h,C}`, `H1Calculator_and_Weights/H1Calculator.{h,C}`, `H1Calculator_and_Weights/H1CalcGenericInterface.h`, `H1Calculator_and_Weights/H1CalcTrig.{h,C}`, `H1Hat/H1HatEvent.{h,C}`, `H1Filler/H1HatFiller.C`, `H1Filler/H1ModsFiller.C`, `H1Geom/H1FNCQuality.{h,C}`, `H1Geom/H1DetectorStatus.{h,C}`, `H1Geom/H1TrigInfo.{h,C}`, `H1Ods/H1SteerOdsEvent.C`, `H1Red/h1red.steer`, `H1Skeleton/H1SteerTree.h`, `H1Banks/farm/`, `H1Banks/dst/`.

SIDIS workspace: `sidis_highQ2/context-handoff.md`, `sidis_highQ2/docs/RUN_STATUS.md`, `sidis_highQ2/STATE.md`, `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`, `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`, `H1ChargedSidis_HighQ2/src_stage1/PhysicsConstants.h`, `H1ChargedSidis_HighQ2/steering_v6/central/evls/data/{03,04,05,06,07}/evls_v6_*.steer`, `H1ChargedSidis_HighQ2/steering_v6/central/sidistree/data/*`, `H1ChargedSidis_HighQ2/Makefile`.

Input data inspected (single-chunk checks, login node):
- `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/mods.4.0.6.10dst7.c070000.root` (FNCPart full scan + Particles control + RunNumber scan)
- `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/hat.4.0.6.10dst7.c070000.root` (PartFNC scan)
- `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/mods.4.0.6.10dst7.c070005.root` (FNCPart full scan + member-value readout)
- `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/hat.4.0.6.10dst7.c070005.root` (PartFNC scan)
- First-entry RunNumber probes: mods chunks `c070005/c070010/c070020/c070030/c070040/c070050`
- File-listing only: `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/{00,03,04,05,06,07,07low,07med,96,97}/`, `/pnfs/desy.de/dphep/online/h1/data/07/{dst7,gmcut}/`, `dst2ods`/`H1Bos2oop` binary inventory, `fplist10619 -s/-r` on `CDST7.C0700001` (83,307 RUNEVENT records; first event 492559).

---

## 9. Open questions and gaps in coverage

1. **Only ep0607 chunks were file-checked** (c070000 FNC-bad, c070005 FNC-good). The other periods (ep0304 dirs 03/04, em0405 dir 05, em06 dir 06) use the identical `oo-4.0` production, so the same structure is expected — but per-period FNC occupancy is unmeasured. A Condor sweep reading only `RunNumber` + `PartFNC` (or the EVL-level `fncIdx` after a Stage-1 fork) over the 660 chunks would quantify occupancy and cross-check `H1FNCQuality` against actual data.
2. **FNC in the MC hat/mods** (rapgap31 / djangoh14 / DjBkgd): not checked. Standard H1 MC MODS almost certainly have empty `FNCPart` (no FNC GEANT simulation in the standard chain) — any forward-neutron SIDIS measurement would need an FNC-specific MC treatment. This is the largest open item for any physics use.
3. **FNC trigger element / subtrigger numbers** for the 2003-2007 runs (needed to select FNC-triggered events and to reproduce published FNC trigger definitions): to be taken from the H1 trigger database or the published FNC analysis notelets; not in H1OO code.
4. **FNC-trigger live-time / luminosity**: not in the input; needs H1 lumi-DB work.
5. **`fEcra` outlier cleanup** and general FNC-variable QA (time, ToF, estimator distributions) before physics cuts.
6. **ep0304 (dirs 03/04) FNC content**: the FNCL bank comment ("data 2001-20xx") and the `H1FNCQuality` coverage (from run 335574) suggest HERA-II-wide availability, but the 2003/04 e- chunks were not inspected.
7. Whether any *alternative* OO productions with richer FNC content exist (none found: `oo-4.0` is the only production under `/pnfs/desy.de/dphep/online/h1/data/`, containing only `hat.*` and `mods.*` files; no ODS ROOT files exist for this data).

---

## 10. Risks / traps for downstream work

- **Do not conclude "FNC absent" from an arbitrary single chunk** — chunks inside `H1FNCQuality` excluded ranges (e.g. c070000, runs 492559-492832) are 100% empty by construction. Always check the chunk's runs against `H1FNCQuality` first.
- **`PartFNC == 0` is ambiguous only in isolation** — it means "no FNC particle", including both "no neutron in acceptance" and "FNC not in readout for this run". Use `H1FNCQuality::IsRunGood()` to separate the cases.
- **The MODS `FNCPart` is the production-time finder output**, not the raw FNCL bank: single cluster, E > 10 GeV, no cell info. Quote it as such.
- **Login node cannot load H1OO libs right now** (GLIBCXX mismatch, §2) — interactive FNC probing must use plain ROOT + streamer emulation, or run on Condor.
- The `PartFNC` HAT word is a *type/quality code* (0-4), **not** an index — do not use it as a pointer into `FNCPart`.

---

*End of artifact.*

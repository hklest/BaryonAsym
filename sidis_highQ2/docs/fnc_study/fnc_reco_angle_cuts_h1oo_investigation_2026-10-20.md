# FNC reconstruction angle/radius cuts in H1OO: does the reconstruction deliberately keep only small-angle neutrons?

- **Date:** 2026-10-20
- **Author:** H1OO framework reference subagent (source-level investigation)
- **Question (user hypothesis):** the FNC reconstruction may intentionally restrict neutrons to small angles / small projected radii (to control shower leakage and improve energy resolution), which would explain the MC response cliff at r ~ 8-10 cm; or the cliff is a simulation-geometry effect.
- **Method:** full source reading of every FNC-touching class in the authoritative H1OO release used by the analysis (`relvol10 / 4.2.1`, AFS source tree), complete-release grep for angular/radial/leakage cuts, and a bounded inspection of the upstream MC simulation source (`h1sim` GEANT3 car export) that produces the FNC content found in the MC DSTs.
- **Status:** all H1OO claims verified against source (file:line quoted). Claims about the upstream MC production chain are marked PROVISIONAL where the decisive inputs (detector-database geometry banks, the MC-side FNCL producer) are not locally inspectable.
- **Preceded by:** `fnc_h1oo_availability.md` (bank/class map), `fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` (measured response cliff).

---

## 1. Executive verdict

**The user's hypothesis is REFUTED for the HERA-II FNC reconstruction, and the MC response cliff is confirmed to be a simulation-side effect, not a reconstruction cut.**

| Claim | Verdict |
|---|---|
| H1OO FNC reconstruction keeps only small-angle / small-radius neutrons | **FALSE.** The HERA-II finder (`H1FindFNC::Find2000_20xx`) applies exactly one physics cut — `E > 10 GeV` — plus "cluster row 0 only". There is **no radial, angular, fiducial, or leakage-motivated cut anywhere** in the finder, the particle class, the calculator, the fillers, or any calibration/weight code of H1OO 4.2.1. |
| A small-radius fiducial exists somewhere in the FNC chain | **Only in dead HERA-I code.** The 1998-2000 finder path has a fiducial window `|y-2.5| < 7 cm && |x+2.5| < 8 cm` (H1FindFNC.C:184) — this path is unreachable for all HERA-II runs (routing at H1FindFNC.C:35). |
| The published `theta_n < 0.75 mrad` cut is encoded in H1OO defaults | **FALSE.** No angular cut of any value exists anywhere in the release (searched: `mrad`, `0.75`, theta/radius/angle co-occurrence with FNC, leakage). It is purely analysis-level (user code). |
| The MC r < 8-10 cm response cliff is a reconstruction cut | **FALSE.** H1OO contains **no FNC detector simulation at all** (no GEANT, no beamline, no response parametrization); it only reads the FNCL bank produced upstream. The cliff is produced upstream in the MC DST chain: the GEANT FNC calorimeter model in `h1sim` is **full-face** (x ± 30 cm), so the cliff must originate in the simulated beamline transport / MC response chain between the IP and the calorimeter — not in any reconstruction code. PROVISIONAL as to the exact upstream mechanism (DB-driven geometry, not locally readable). |
| Small-angle restriction exists anywhere | **Yes, in exactly two places, neither of them the HERA-II reconstruction:** (a) the analysis level (published leading-neutron analyses' `theta_n < 0.75 mrad` selection, applied by users); (b) de facto in the MC simulation's effective response cone (upstream of H1OO, empirical). |

**Implication for MC acceptance correction:** the current oo-4.0 MC hat/mods FNC content cannot be used as a full-acceptance FNC simulation — independently confirmed at code level: since the (cut-free) reconstruction stores whatever the simulation produces, the absence of MC clusters beyond r ~ 10 cm is a property of the simulated transport, and no code-level switch can widen it. The MC is usable only inside its own response cone (r < ~8 cm, theta < ~0.75 mrad), consistent with the efficiency diagnosis (18.55% matched efficiency inside the cone vs 5.09% on the full face).

---

## 2. Source basis

- Authoritative release (per workspace `AGENTS.md` and confirmed by `thish1.sh`): **`/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`** — readable on AFS; this tree is the reference used below.
- The workspace path `/data/dust/group/h1/klesthen/BaryonAsym/h1oo-master` **does not exist** (verified 2026-10-20; consistent with `fnc_h1oo_availability.md` §2); `/data/dust/group/h1/klesthen/BaryonAsym/h1oo-build/` contains only `bin/` and `lib/`. All findings below are from the AFS release tree, which is authoritative.
- Upstream simulation source (supplementary): `/afs/desy.de/group/h1/h1sim/h1sim.car31003.fullhistory_from30804.gz` (CMZ car text export, 2005-era version 31003; HERA-II-era FNC code present).
- Upstream bank definitions: `/afs/desy.de/group/h1/h1banks/H1Banks/fnc/{fncl,fnce,fncr,fnht}.ddl` (identical FNCL text to the H1OO copy).

---

## 3. Every cut, threshold, and selection in the H1OO FNC chain (complete inventory)

The complete FNC-referencing file list in the release was enumerated (case-insensitive grep over `*.h *.C *.cxx *.ddl *.steer`); after removing false positives (`fNCells`, `fNCuts`, `GetStrFncCon`, NC-trigger weights, `FNCLOS` Fortran names), **every** FNC file was read. The chain is: FNCL bank (read) -> `H1FindFNC` -> `H1PartFNC` -> MODS branch `FNCPart` -> `H1CalcFnc`; HAT word `PartFNC`; run-quality via `H1FNCQuality` / `H1DetectorStatus`.

### 3.1 The finder — `H1Finder/H1FindFNC.{h,C}` (the ONLY selecting code)

Constructor (`H1FindFNC.C:24-27`):
```cpp
H1FindFNC::H1FindFNC()
{
// default cuts: none
}
```

Routing (`H1FindFNC.C:34-41`): HERA-II runs (`H1Constants::Instance()->IsHeraII(runNumber)`, defined as `Year(run) > fgLastYearHeraI`, `H1Geom/H1Constants.C:645-650`) go to `Find2000_20xx`; only pre-HERA-II runs `>= FirstRun(1998)` go to `Find1998_2000`. **All 2005-2007 data and the MC run numbers (468531-472427 era) take the HERA-II path.**

**HERA-II path `Find2000_20xx` (`H1FindFNC.C:69-109`) — the complete selection logic:**

1. `H1FindFNC.C:73-75` — reads the FNCL bank; `if (nClusters == 0 ) return 0;`
2. `H1FindFNC.C:78-91` — reads **only cluster row 0** (`fncl[0]->GetInt(0)` ... `fncl[0]->GetInt(11)`), mapping the 12 FNCL columns to `fClType, fEnergy, fFncX/Y/Z, fTimeToF, fTime, fEcra, fCorEne, fSecMom, fEMainCalo, fQuality`.
3. `H1FindFNC.C:93` — `fCharge = fQuality & 1 ; // Extract Charge from Quality word` — **extracted but never used as a cut** in this path.
4. `H1FindFNC.C:97-108` — the complete cut:
```cpp
    // -- Apply final selection citeria
    if (fEnergy > 10. ) {
      // ... create H1PartFNC ...
      return partFNC;
    }
    // -- Final selection failed -> return 0
    return 0;
```

**That is all.** No radial cut, no theta cut, no fiducial box, no leakage rejection, no ClType requirement, no quality requirement, no time cut. Clusters anywhere on the FNC face (the data show x in [-30,+22] cm, y in [-20,+28] cm) pass untouched as long as `Ecl > 10 GeV`.

**HERA-I path `Find1998_2000` (`H1FindFNC.C:112-201`) — dead code for HERA-II, quoted for completeness:**
- Loops over FNHT clusters, keeps the max-energy one (`:163`), applies survey shifts `fFncX = fncx - 1.6; fFncY = fncy + 4.8;` (`:165-166`).
- `H1FindFNC.C:182-184` — the complete cuts:
```cpp
    if (fClType > 0 && fCharge == 0)
        if (fEnergy > 10. && fEnergy < 1200.)
            if (TMath::Abs(fFncY - 2.5) < 7. && TMath::Abs(fFncX + 2.5) < 8.) {
```
  i.e. a neutral hadronic cluster, 10 < E < 1200 GeV, inside a **±7 cm (y) x ±8 cm (x) fiducial window** around the HERA-I FNC center — the only small-radius restriction that exists anywhere in H1OO, and it is unreachable for every HERA-II run.

### 3.2 The particle class — `H1Mods/H1PartFNC.{h,C}`: kinematics only, no cuts

- `GetTheta()` (`H1PartFNC.C:71-76`): `return TMath::Sqrt( fFncX*fFncX + fFncY*fFncY)/10550.;` — the FNC z position is **hardcoded 10550 cm** (comment at `:30-31`: "the z-position is set to be the front face of the FNC at z=10550"). Note the upstream GEANT model puts the front face at z = 10590.5 cm (§5.2), a ~0.4% theta-scale detail, not a cut.
- `GetE()` (`H1PartFNC.C:51-57`): returns the raw energy — `Float_t Corr = 1;` with the HERA-I tail-catcher leakage correction **commented out**: `// if ( fTail>30. ) Corr = 655/(350+85*TMath::Log(fTail));`. **No leakage correction is applied anywhere in H1OO** (the string "leakage" does not occur in the release at all).
- `GetMass()` (`:43-48`): `fClType == 1 ? 0 : neutron mass` (photon vs neutron interpretation of the stored ClType).
- All other methods (`GetP/GetPt/GetPx/GetPy/GetPz/GetPhi`, estimator getters `GetCorEne/GetEcra/GetSecMom/GetEMainCalo/GetTime/GetTimeToF/GetTail/GetQuality/GetClType`) are pure accessors. `fClType` comment (`H1PartFNC.h:24-26`): `2,3 - Neutron, 1 - Photon`. `SetFourVector` (`H1PartFNC.C:27-40`) exists but is called nowhere in the framework.

### 3.3 The calculator — `H1Calculator_and_Weights/H1CalcFnc.{h,C}`: pure accessors, no cuts

Connects to the MODS `FNCPart` branch (`H1CalcFnc.C:28`, `H1PartFNCArrayPtr fFncArray("FNCPart")`). `GetIndex()` (`:89-105`): 0 if one FNC particle present, -1 if none ("Nothing ... or One cluster ... assumed always" — a direct consequence of the cluster-0-only finder). All getters (`GetMass/GetZ/GetPt/GetP/GetPhi/GetTheta/GetTail/GetTime/GetQuality/GetClType/GetCorEne/GetEPrime/GetE/GetEcra/GetClusterPosition/GetNeutron`, and `GetHtypeFNC`) are lazy-cached pass-throughs with no selection. The duplicate copy under `H1Calculator/` is byte-identical (verified by diff). The generic interface wrappers (`H1CalcGenericInterface.h:40-47`) add no logic. **No FNC weight exists**: `H1CalcWeight` has no FNC term (its `fNC*` members are NC-trigger/QCD weights — unrelated).

### 3.4 Fillers and event model

- `H1Filler/H1ModsFiller.C:139` calls `FillFNC(modsevent)` **unconditionally** inside `Fill()`; `FillFNC` (`:296-304`) instantiates `H1FindFNC fncFinder; fncFinder.Find(tracks, fnc);` — no configuration, no steering hooks, no cut parameters. Whatever the finder accepts is stored.
- `H1Filler/H1HatFiller.C:1135-1141` — HAT `PartFNC` is a type/quality **flag**, not a cut: `0` none; `1` ClType>1 (hadron) + Quality==0; `2` ClType>1 + Quality!=0; `3` ClType==1 (EM) + Quality==0; `4` ClType==1 + Quality!=0.
- `H1Mods/H1ModsEvent.C:32,70,169` registers `FNCPart` (`TClonesArray` of `H1PartFNC`) as a standard MODS branch.

### 3.5 Run-level quality (not event cuts)

- `H1Geom/H1FNCQuality.{h,C}` (V. Dodonov 2008): static excluded-run-range / excluded-run lists ("problems in BR10/FNC DAQ or with FNC not included in readout. Only HERA-II runs provided"), interface `IsRunGood(int runnr)`. Run-level rejection only; says nothing about angles.
- `H1Geom/H1DetectorStatus.C:675-682` — `SetFNCOn()` selects HV bit 37 / CAEN channel 26 for per-run detector-status selection.
- `H1Tools/oolumi.C:468-470` — when the FNC option is set, runs failing `H1FNCQuality::IsRunGood` are rejected from the luminosity list ("FNC DAQ problems - reject!"). Run-level only.

### 3.6 Bank-level thresholds documented (produced upstream, not by H1OO)

FNCL DDL (`H1OOBanks/h1banks.ddl:787-848`, identical to `/afs/desy.de/group/h1/h1banks/H1Banks/fnc/fncl.ddl`):
- `ClType` comment (`:800-804`): a Preshower/MainCalo "signal" requires the cell-summed energy (elm. scale, noise cuts applied) to exceed **4 GeV (Preshower) / 10 GeV (MainCalo)** — these are the upstream reconstruction's cluster-presence thresholds, not H1OO cuts.
- `Quality` bit 0 = charge (0 neutral, 1 charged = coincidence of both FNC Veto Counters); **"for the meaning of other bits see FNC home-page"** — i.e. the full Quality-word semantics are NOT in H1OO and NOT in any local source; users applying Quality cuts do so from external documentation.
- `CorEne` (core energy E(4)/E) is range-checked to [0,1] by the generated row class (`H1OOBanks/ODS/H1Fncl.h:74-79`) — a bank validity check, not a selection.
- `Ecra` is an **estimator** (cluster radius, stored), never cut on in the framework.
- Bank-list plumbing only: `H1Ods/H1SteerOdsEvent.C:28-30` (ODS bank list incl. `FNHT,FNCR,FNCE,FNCL`), `H1Red/h1red.steer:10-11`, `H1UserCim/*.steer` (commented lists).

### 3.7 Absence proofs (searched the full release)

- `mrad`/`millirad`: only unrelated hits (FTT track-matching constants in `H1UserFtt/trkMatch.C:392-395`).
- `leakage`: **zero** hits in the entire release.
- FNC co-occurring with theta/angle/radius: only the `GetTheta` accessor definitions and generic-interface lookups.
- `0.75` in FNC-relevant directories: only unrelated constants (EM-cluster cone R=0.75 in eta-phi, `H1CreatePartEm.C:182`; track pT>0.75 in a trigger-weight comment).
- No FNC trigger element/subtrigger constant anywhere (confirmed `fnc_h1oo_availability.md` §3.6).
- No FNC entries in `H1Selection/`, `H1Cuts/` (not in the FNC file list at all), no FNC reweight class, no FNC luminosity tool beyond the run-quality rejection in oolumi.

---

## 4. Where the FNCL bank comes from (the producer is NOT in H1OO)

- H1OO **reads** FNCL/FNCE (bank pointers `H1OOBanks/H1FnclBankPtr.{h,C}`, `H1FnceBankPtr.{h,C}` — generated, logic-free accessors) from DST-derived ODS content; it contains **no code that writes or reconstructs FNCL**. The farm bank definitions in H1OO (`H1Banks/farm/*.ddl`) contain no FNC banks — the FNCL/FNCE banks enter the DST stream from the FNC-local reconstruction in the H1 offline chain (the DDL's "see FNC home-page" is the only pointer). The data-side producer was not found in the accessible AFS trees (`h1rec7`/`h1rec9` car files contain no FNCL producer).
- **MC side:** the H1 GEANT simulation (`h1sim`) does contain the FNC — but it does **not** write FNCL either. It produces hit/digit banks: `FNCDIGI` creates **FRCE** (channel + energy in MeV) and **FRCX** (track-digit relations) from the FRCT hit bank (deck `FNCDIGI`, car line ~248796). The FNCL banks found in the MC DSTs are therefore produced by a further downstream step in the MC DST production (FNC reconstruction run on the MC digits). **That producer's code and parametrization were not found in the accessible trees and are not locally inspectable** — PROVISIONAL; provenance routing belongs to `h1-mc-db` / `h1-mc-expert`.

---

## 5. The data-vs-MC asymmetry in light of the code

### 5.1 The cliff cannot be a reconstruction cut

The measured facts (`fnc_mc_matching_efficiency_diagnosis_2026-10-20.md`): data clusters populate x in [-30,+22] cm, y in [-20,+28] cm (theta up to ~2.7-3.4 mrad), while the MC per-neutron FNC response is 54.6% at projected r<2 cm, 9.6% at 6-8 cm, ~0 beyond 10 cm.

Since the HERA-II finder imposes no position/angle condition whatsoever (§3.1) and the fillers store whatever passes (§3.4), the OO chain transmits clusters at any radius. The data prove this: the same cut-free chain delivers large-radius clusters in real data. The MC deficit is therefore created **before** the FNCL bank exists — in the simulation — or in the MC-side digit-to-FNCL step. **It is not, and cannot be, an H1OO reconstruction selection.**

### 5.2 What the OO chain does and does not simulate

- **H1OO simulates nothing.** It is a read/analysis framework: no GEANT code, no beamline geometry, no sensitive-volume definitions, no detector response (the release's only "GEANT" strings are code comments and a generated bank-class description; `h1hepmc` is generator-event HepMC I/O, `h1mcutil` is production utility scripts). The FNCL content of both data and MC DSTs is made upstream.
- **The upstream GEANT FNC calorimeter model is full-face — the cliff is not the calorimeter geometry.** From the `h1sim` car export (version 31003):
  - `+KEEP,FNCMAP` (car ~:8990-9006): FNC z span `ZFNCMIN/10590.5/ ... ZFNCMAX/10836.0/`; face `XFNCMIN/-30.00/, XFNCMAX/+30.00/` with `X0/-1.58/`; `YFNCMIN/-48.00/, YFNCMAX/+12.00/` with `Y0/+20.3135/` (plus step offsets `DXSTEP1-3 = +26/+20/+26.10`, `DYSTEP1-3 = +20/+20/+26.10` describing the segmented construction). This is a ~60 cm-wide face — consistent with the data face — **not** an r ~ 10 cm detector.
  - `+DECK,FNCGEOM` (car ~:247628): "Purpose: to put FNC into volume JDX4, which itself is put into volume FBL3 belonging FWFE volume. Called by: GUGEOM for LCONFG>=10" — the FNC is embedded in the forward-beamline volume hierarchy, and all FNC geometry/material/media parameters are read from **database banks** (`FNMA, FNMX, FNME, FNRO, FNVO, FNF1, FNF2, FNTP`).
  - The forward beamline itself is database-driven too: "DEFINE GEOMETRY OF FORW. BEAMLINE (IN PLUG,FWFE) FROM **OGDJ** BOS BANK" (car ~:24142 etc.).
  - Fast-sim pickup in "outside space" (car ~:61383, ~:61553-61556): `DATA ZMINFNCA/10590.5/,ZMAXFNCA/10836./,RMAXFNCA/110./` and `IF(Z.GE.ZMINFNCA.AND.Z.LT.ZMAXFNCA.AND.R.LT.RMAXFNCA) ... CALL AUFNCA(...)`; spot deposition via `SPODEP` -> `MOLD.EQ.14` -> `FNCDEPO` (car ~:60541, `IF(ISRBNK(7).EQ.1)CALL FNCDEPO(...)`).
- **Where the cliff most plausibly originates (PROVISIONAL):** the simulated transport between the interaction point and z ~ 105.9 m — i.e. the OGDJ-defined beam-pipe/beamline apertures and whatever material model stands between the central detector and the FNC — and/or the MC digit-to-FNCL response step (§4). Both are database/production-side and not locally readable. The observed **smooth within-cone gradient** (54.6% -> 27.1% -> 15.2% -> 9.6% across r = 0-8 cm) is characteristic of progressive geometric interception (aperture/transport), not of a hard reconstruction threshold; a reconstruction cut would produce a sharp all-or-nothing edge at the cut value with ~full efficiency inside.
- The published H1 leading-neutron measurements treated the real beamline with a **dedicated beamline-transport + FNC simulation** rather than relying on the standard chain (PROVISIONAL — general H1 knowledge, not locally verified; route to `h1-mc-expert`/`h1-mc-db`). This is consistent with the standard-chain MC being acceptance-limited exactly as measured.
- The second defect noted in the diagnosis — generators emitting neutrons azimuthally symmetric around the ideal +z axis while the data flux follows the proton-beam tilt — is likewise entirely outside H1OO: the framework performs no beam-direction transformation anywhere in the FNC chain (`GetTheta()`/`GetPhi()` are plain coordinate computations from the stored cluster x, y).

### 5.3 Reconciliation with the "theta < 0.75 mrad" literature number

The approximate edge of the MC response cone (r ~ 7.9-10 cm at z = 105.5 m -> theta ~ 0.75-0.95 mrad) numerically coincides with the published analyses' `theta_n < 0.75 mrad` analysis cut. The code shows these are two different things: the published cut is an **analysis-level** choice (made because the real FNC acceptance and resolution degrade at larger angles and require the dedicated beamline simulation to describe), while the MC cliff is the **standard-chain simulation's effective aperture**. The numerical coincidence is expected — both reflect the same physical beamline aperture — but the mechanism is simulation geometry on one side and deliberate analysis selection on the other. Neither is a reconstruction cut.

---

## 6. Verdict on the user hypothesis

**REFUTED in its reconstruction form; the true small-angle restrictions live elsewhere.**

1. **Reconstruction (H1OO, HERA-II): NO small-angle/small-radius restriction exists.** The complete selection is `E > 10 GeV` (H1FindFNC.C:98) + cluster-0-only (H1FindFNC.C:74-91). No leakage control, no position-dependent calibration, no shower-start logic, no quality/type cut is applied by the framework. The framework also applies **no** energy leakage correction (the only one ever written is commented out, H1PartFNC.C:55).
2. **Historical HERA-I finder:** a genuine small-window fiducial exists (|y-2.5|<7 cm, |x+2.5|<8 cm, H1FindFNC.C:184) — dead code for all HERA-II runs. This is the only grain of truth for "the reconstruction restricts to small radii", and it does not apply to this analysis.
3. **Analysis level:** the published `theta_n < 0.75 mrad` cut is purely user-level; H1OO has no default, no constant, no predefined FNC selection helper for it.
4. **Simulation (upstream, not H1OO):** the effective MC response cone (r < ~8-10 cm) is the source of the data-vs-MC asymmetry. It is a simulation-geometry/transport effect: the GEANT FNC calorimeter model is full-face, so the limitation is in the beamline transport / MC response chain between the IP and the calorimeter (DB-driven, not locally inspectable — PROVISIONAL on the exact mechanism).

**Implication for MC acceptance correction:** because the reconstruction is cut-free, the MC's missing large-radius response cannot be "recovered" by any framework-side change; the deficit is in what the simulation delivers into FNCL. The current MC is valid as an FNC acceptance correction only inside its own response cone (theta_n <~ 0.75 mrad / r <~ 8 cm, where the response is calorimeter-like, E_reco/E_gen ~ 1.03-1.06, and the RAPGAP-pi purity is high); on the full face it underestimates the acceptance by the measured factor ~15. Full-face use requires a dedicated FNC/beamline MC or external acceptance maps (route: `h1-mc-db` for availability, `h1-mc-expert` for physics interpretation) — consistent with §8 of the efficiency diagnosis.

---

## 7. Audit trail

H1OO 4.2.1 (all under `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`), every file read in full unless noted:
`H1Finder/H1FindFNC.{h,C}`; `H1Finder/H1Find.h`; `H1Mods/H1PartFNC.{h,C}`; `H1Mods/H1PartFNCArrayPtr.{h,C}`; `H1Mods/H1ModsEvent.C` (FNC lines); `H1Calculator_and_Weights/H1CalcFnc.{h,C}` (full) and `H1Calculator/H1CalcFnc.{h,C}` (diff-verified identical); `H1Calculator_and_Weights/H1CalcGenericInterface.{h,C}` (FNC lines); `H1Calculator_and_Weights/H1CalcWeight.{h,C}` (FNC-mention check: false positives only); `H1Filler/H1ModsFiller.C` (FillFNC), `H1Filler/H1HatFiller.C` (PartFNC block), `H1Filler/H1CopyBankFiller.C` (comment mention); `H1Hat/H1HatEvent.h` (fPartFNC); `H1Geom/H1FNCQuality.{h,C}` (header full, .C excluded-run data), `H1Geom/H1DetectorStatus.C` (SetFNCOn), `H1Geom/H1Constants.{h,C}` (IsHeraII); `H1Tools/oolumi.C`, `H1Tools/H1SteerOolumi.C` (FNC lines); `H1OOBanks/H1FnclBankPtr.{h,C}`, `H1OOBanks/ODS/H1Fncl.h`, `H1OOBanks/ODS/H1Fnce.{h,C}` (bank classes); `H1OOBanks/h1banks.ddl` (FNCL/FNCE tables); `H1Ods/H1SteerOdsEvent.C`, `H1Red/h1red.steer`, `H1UserCim/*.steer` (bank lists); `H1OOBanks/ODS/H1Evcl.C`, `H1OOBanks/ODS/H1Vrce.C`, `H1Mods/H1PartEm.*`, `H1MuonFinder/H1MuonCandidate.C`, `H1QCDFunc/*`, `H1NonepBgFinder/*`, `H1Analysis/*`, `H1UserDstar/*`, `H1UserLifetime/*`, `H1Examples/clusters_ods.C` (false-positive verification).

Release-wide searches: `grep -ril 'fnc'` over `*.h *.C *.cxx *.ddl *.steer` (file list enumerated, all resolved); `mrad|millirad`, `leakage`, FNC+theta/angle/radius co-occurrence, `0.75` in FNC-relevant directories, `GEANT` mentions; release top-level directory listing (no simulation package present).

Upstream (supplementary): `/afs/desy.de/group/h1/h1banks/H1Banks/fnc/fncl.ddl` (read); `/afs/desy.de/group/h1/h1sim/h1sim.car31003.fullhistory_from30804.gz` (greps + targeted deck extractions: `FNCMACRO/FNCMAP`, `FNCGEOM`, `FNCVOLEM` header, `FNCDEPO`, `FNCDIGI`, SPODEP FNC dispatch, outside-space pickup, OGDJ beamline comment); `/afs/desy.de/group/h1/h1rec7/h1rec771906.car` (FNC grep: FNCA H1FAST defaults only, no FNCL producer); `h1rec9` car/header scan (no FNCL producer found); `/afs/desy.de/group/h1/` and `/afs/desy.de/group/h1/cvs/` listings (no FNC-named package or module found).

Workspace checks: `h1oo-master` absent (verified); `sidis_highQ2/STATE.md` read (no FNC-related state); `.opencode/skills/` empty at check time.

## 8. Limitations / provisional items

1. The `h1sim` car inspected is version 31003 (2005-era). The DST7 MC production used later database content; the FNC code structure (LCONFG>=10, DB-driven geometry) is expected unchanged, but the production's actual OGDJ/FNVO geometry values were not read. The FNC-face conclusion rests on the code constants of `FNCMAP`, which are hard-coded, plus the data-side face evidence.
2. The exact origin of the r ~ 8-10 cm cliff (beam-pipe aperture vs magnet apertures vs MC response parametrization) is **not locally resolvable** — the decisive inputs are detector-database banks (OGDJ/FNVO/FNF*) and the MC-side FNCL producer, neither found in readable form. PROVISIONAL; route to `h1-mc-db` (provenance) / `h1-mc-expert` (physics interpretation).
3. FNC `Quality`-word semantics beyond bit 0 (charge) are not in any local source ("see FNC home-page"); any Quality-based analysis cuts require external documentation.
4. `H1PartFNC::GetTheta()` uses a fixed z = 10550 cm while the GEANT front face is at 10590.5 cm and data `Zcl` scatters around ~10,601 cm — a sub-percent theta-scale convention, noted but irrelevant to the cut question.

*End of artifact.*

# Research: v5g scattered-electron track association, gen-method identity, and quarantine status

**Date:** 2026-05-23
**Scope:** Five concrete pre-implementation verification tasks for the v5g
high-Q² charged-particle SIDIS tree-builder spec
(`docs/specs/spec-v5g-builder-scat-elec-veto-and-rad-x.md`).
**Read-only:** no edits performed to any source or builder file.
**Sources:** local H1OO snapshot at `H1/h1oo-master/`, local builder cache at
`H1/sidis_highQ2/source_cache/`, NAF authoritative source at
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`.

> **Important up-front finding.** The local builder cache filename
> `ProtonTreeBuilderHighQ2.C` is *stale and renamed on NAF*. The
> authoritative NAF source is already named
> `ChargedSidisTreeBuilderHighQ2.C` (84 563 bytes vs. local 62 509 bytes),
> and the corresponding utils header is also renamed
> (`ChargedSidisUtils_highQ2.h`). MD5 hashes differ. This affects how
> Task C should be interpreted: the *NAF tree* is properly quarantined and
> already renamed; the *local source_cache* has not been refreshed since
> 14 May 2026. See §C for details.

---

## A. Gen electron-method vs gen eSigma identity check

### A.1 H1OO definitions (algebraic from source)

The H1OO `H1MakeKine` class defines the per-method kinematics. From
`h1oo-master/H1PhysUtils/H1MakeKine.h:56-78` and
`h1oo-master/H1PhysUtils/H1MakeKine.C`:

**Electron method (`MakeElec`, H1MakeKine.C:26-44):**
```cpp
fYe  = 1 - ((eu / EBEAM) * (1 - cos(thu))/2);
fQ2e = 4 * EBEAM * eu * ((1 + cos(thu))/2);
fSS  = 4*EBEAM*PBEAM;
// (Xe accessed inline as fQ2e/(fYe*fSS), H1MakeKine.h:57)
```

**Sigma method (`MakeSig`, H1MakeKine.C:64-82):**
```cpp
Float_t denom = sig + (eu * (1 - cos(thu)));
fYs = sig / denom;
fQ2s = (eu * sin(thu)) * (eu * sin(thu)) / (1 - fYs);
// (Xs accessed inline as fQ2s/(fYs*fSS), H1MakeKine.h:61)
```
where `sig = E_h - p_z,h` (hadronic Σ) and `eu = E'`, `thu = θ'` are the
scattered-electron energy and polar angle.

**eSigma method (inline getters, H1MakeKine.h:72-74):**
```cpp
Float_t  GetQ2es() const {return GetQ2e();}                            // = Q²_e
Float_t  GetXes()  const {return GetYs()*fSS>0 ? GetQ2s()/(GetYs()*fSS) :0. ;}   // x_es ≡ x_s
Float_t  GetYes()  const {return GetXs()*fSS>0 ? GetQ2e()/(GetXs()*fSS) :0. ;}   // y_es = Q²_e / (x_s · s)
```
So eSigma uses Q² from the electron method, but *y* and *x* are reshuffled
from sigma-method results. Equivalently:
- `y_es = Q²_e / (x_s · s)`
- `x_es = Q²_s / (y_s · s) ≡ x_s`
- `Q²_es = Q²_e`

### A.2 H1Calculator gen-level wiring

`H1CalcKine::CallSigGen()` (H1CalcKine.C:854-870) builds the gen sigma
inputs from the gen scattered electron and the gen HFS-less-electron:
```cpp
const TLorentzVector& elec = Elec->GetFirstElectronGen();
const TLorentzVector& had  = Fs->GetAllFsLessElectronGen();
fMakeKine->MakeSig(elec.E(), elec.Theta(), had.E()-had.Pz(), EBEAM, PBEAM);
fQ2sGen = fMakeKine->GetQ2s();
fYsGen  = fMakeKine->GetYs();
fXsGen  = fMakeKine->GetXs();
```
Then `GetQ2esGen()` returns `GetQ2eGen()` (H1CalcKine.C:399-403);
`GetYesGen()` returns `GetQ2eGen()/(GetXsGen()*fSS)` (H1CalcKine.C:406-411);
`GetXesGen()` returns `GetXsGen()` (H1CalcKine.C:414-418). So the eSigma-gen
values are wired exactly to the formulas in §A.1, but with `Elec.E/Theta` =
*gen* electron and `had.E - had.Pz` = *gen* HFS-less-electron.

### A.3 Algebraic identity at closed gen level

If the gen final state is 4-momentum balanced — i.e. all final-state
particles (including any radiated photon) are present in the gen list, and
`E_beam_e` is the effective beam energy seen by the hard scattering — then
4-momentum conservation gives:

```
(E_h - p_z,h)_gen + (E' - p_z')_gen = 2 · E_beam_e
=> Σ_gen + E'(1 - cos θ') = 2 · E_beam_e
=> Σ_gen = 2 · E_beam_e − E'(1 - cos θ')
```
Plugging into the sigma-method formula for `y_s`:
```
y_s = Σ_gen / (Σ_gen + E'(1-cos θ')) = (2·E_beam_e − E'(1-cos θ')) / (2·E_beam_e)
    = 1 − E'(1-cos θ')/(2·E_beam_e)
    = y_e    (compare to MakeElec: fYe = 1 − (E'/E_beam_e)·(1-cos θ')/2)
```
So **y_s,gen = y_e,gen** under closed 4-momentum conservation.

Then since `Q²_es ≡ Q²_e` by *definition* (H1MakeKine.h:72), and since
`Q²_s = (E' sin θ')² / (1−y_s) = (E' sin θ')²/(1−y_e) = Q²_e` only when
`y_s = y_e` AND `Q²_e` is rewritten in this form, which is the case here.
Therefore:

- `Q²_es,gen = Q²_e,gen`  (by definition, always)
- `y_es,gen  = Q²_e,gen / (x_s · s)` with `x_s = Q²_s/(y_s · s)`,
  so `y_es = Q²_e · y_s / Q²_s`. With `Q²_s = Q²_e` and `y_s = y_e`
  (closed gen), `y_es = y_e`. ✓
- `x_es,gen  = x_s = Q²_s/(y_s · s) = Q²_e/(y_e · s) = x_e`. ✓

**Conclusion:** at the truth level with full 4-momentum conservation in the
gen list, **gen electron-method and gen eSigma method are algebraically
identical** for all three of Q², y, x.

### A.4 Caveat — ISR photon

In DJANGOH NC events the ISR photon (if any) is included in the gen list.
The HAT branch `GenEnPhoton` carries its energy; the v5g spec uses an
*effective* beam energy `E_beam_e − E_γ_ISR` when computing
`x_es_gen / y_es_gen / Q²_es_gen`. The H1OO `Elec->GetFirstElectronGen()`
(line 820 of H1CalcKine.C) and `Fs->GetAllFsLessElectronGen()` (line 821)
already use a topology-dependent gen-electron 4-vector
(`UseGenElecUncombined` toggle, H1CalcElec.h:462-466) — by default the FSR
photon is combined back into the electron. In that case (the default), the
identity at closed 4-momentum reduces to the standard form above. If
`UseGenElecUncombined(true)` were called the user takes responsibility for
the photon book-keeping; this is not the default and not what the v5g spec
uses.

The spec's chosen prescription
("gen electron-method using `genElec + GenEnPhoton` for effective E_beam")
is therefore numerically identical, at closed gen level, to a true gen
eSigma method that uses `Σ_h` from the gen HFS. **No accuracy is lost by
choosing the simpler electron-method form.**

### A.5 H1OO numerical test

No numerical regression test specifically comparing gen-eSigma to
gen-electron-method was located in `h1oo-master/H1Examples/` or in the
`H1Calculator/` directory. The closest is the unit-level comment in
`H1MakeKine.h:72` stating `GetQ2es() == GetQ2e()` by construction. The
algebraic equivalence above is the H1OO position.

---

## B. H1OO scattered-electron track + cluster association

### B.1 Scattered-electron class

**Class:** `H1PartEm` (header: `H1Mods/H1PartEm.h`, impl: `H1Mods/H1PartEm.C`).
Inherits `H1PartID` → which carries a pointer back to its parent
`H1PartCand` via `H1PartID::GetParticle()` (returns `const H1PartCand*`,
`H1Mods/H1PartID.h:64-67`).

The "*the* scattered electron" is the H1PartEm that has the bit
`H1PartEm::kIsScatElec = BIT(14)` set (`H1Mods/H1PartEm.h:115`):
```cpp
inline Bool_t IsScatElec() const { return TestBit(kIsScatElec); }
```
**Crucially**, `H1PartEm::SetIsScatElec()` (H1PartEm.h:310-313) also
**propagates the bit down to the parent H1PartCand**:
```cpp
inline void SetIsScatElec() {
    SetBit(kIsScatElec);
    const_cast<H1PartCand*>(this->GetParticle())->SetIsScatElec();
}
```
This is the key API hook for v5g (see §B.5).

`H1PartCand::kIsScatElec = BIT(23)` is defined separately on H1PartCand
(`H1Mods/H1PartCand.h:77`), and `H1PartCand::IsScatElec()` is implemented as
a direct bit test for efficiency (`H1Mods/H1PartCand.C:832-839`):
```cpp
Bool_t H1PartCand::IsScatElec() const {
    return TestBit(kIsScatElec);
}
```
Note the comment in H1PartCand.C:834-838 explicitly states that the bit
test is preferred over `GetIDElec()->IsScatElec()` for speed.

**Selection of the scat-elec H1PartEm** is done by
`H1CreatePartEm::SelectScatElec()` (`H1PartEmFinder/H1CreatePartEm.C:97-167`).
After running both the LAr and SpaCal electron finders, electron candidates
are Pt-sorted (line 119, `ecand->Sort()`) and the highest-Pt isolated
candidate with E > 8 GeV is flagged as scattered (lines 127-134):
```cpp
for(Int_t i=0; i<ecand->GetEntriesFast(); i++) {
    H1PartEm *escat = (H1PartEm*) ecand->UncheckedAt(i);
    if(escat->GetE() > disThreshold && escat->IsIsolatedLepton()) {
        escat->SetIsScatElec();
        break;
    }
}
```
At most one H1PartEm per event carries the `kIsScatElec` flag.

### B.2 Associated track accessor

The H1PartEm class **does not** carry an `H1PartSelTrack*` pointer
directly. Inspection of `H1Mods/H1PartEm.h` shows it has only *track
parameters* copied in from the matching algorithm:
```
fTrType, fTrTheta, fTrPhi, fTrMom, fTrDtheta, fTrDphi, fTrDpt,
fTrDedx, fTrNhit, fDTrack, fTrClusDca, fTrDcaVtx, fNbMatchingTr ...
```
(H1PartEm.h:34-49). These are populated by the matching algorithms in
`H1CreatePartEm.C` (`FindMatchingDTRATrack`, `FindMatchingDTNVTrack`,
`FindMatchingLWTrack`). They are not a reverse pointer.

However, the **parent H1PartCand** *does* hold the link:

- `H1PartCand::GetTrack()` returns the matched ODS `H1Track*`
  (H1PartCand.h:173, impl in H1PartCand.C).
- `H1PartCand::GetIDTrack()` returns the matched **`H1PartSelTrack*`**
  (H1PartCand.h:181, impl H1PartCand.C:700-706):
  ```cpp
  const H1PartSelTrack* H1PartCand::GetIDTrack() const {
      return static_cast<const H1PartSelTrack*>(this->GetPartID(H1PartSelTrack::Class()));
  }
  ```
- `H1PartCand::GetIDElec()` symmetrically returns the H1PartEm
  (H1PartCand.h:182, impl H1PartCand.C:709-716).

These accessors traverse the H1ArrayPointer of attached `H1PartID`
objects. The H1OO docs (H1PartCand.C:681-690) explicitly warn that this
"demands a lot of data to be loaded" and recommends the pointer-equality
shortcut described in §B.5 below.

### B.3 Associated cluster accessor

- `H1PartCand::GetClusters()` returns the `TObjArray*` of associated
  `H1Cluster` ODS objects (H1PartCand.h:174).
- `H1PartEm` carries the cluster centre coordinates directly:
  `GetXClus()`, `GetYClus()`, `GetZClus()` (H1PartEm.h:152-154 / .C),
  populated by the LAr / SpaCal finders.

### B.4 High-Q² matching algorithm

For the LAr (barrel/forward) electron finder used at high Q²:

- The LAr finder entry point is `H1CreateLArEm::Find()`
  (`H1PartEmFinder/H1CreateLArEm.{h,C}`), invoked from
  `H1CreatePartEm::Find()` (`H1CreatePartEm.C:77-78`).
- Track-cluster matching is performed by three sister functions in
  `H1CreatePartEm.C`:
  - `FindMatchingDTRATrack(cluster, trackEvent, NbTracks, TrClDca)`
    (H1CreatePartEm.C:286-436) — matches against fitted ODS tracks
    (`H1CombinedFittedTrack`, `H1CentralFittedTrack`,
    `H1ForwardFittedTrack`).
  - `FindMatchingDTNVTrack(...)` (H1CreatePartEm.C:440-554) — non-vertex
    fitted (DTNV) tracks (`H1CentralTrack`, `H1ForwardTrack`).
  - `FindMatchingLWTrack(...)` (H1CreatePartEm.C:561-695) — Lee-West /
    H1PartSelTrack tracks.

The matching criterion in all three is **DCA < 12 cm** between the
extrapolated track position at the LAr surface and the cluster barycenter,
with an azimuthal opening-angle cut `Alpha < 2·3·π/8`
(H1CreatePartEm.C:394, 520, 654), implemented in
`H1CreatePartEm::CalcDCA()` (H1CreatePartEm.C:700-751). Extrapolation uses
the helix `GetArcLengthAtCalo()` (lines 369, 491, 629).

The functions return the chosen track based on the highest-Pt **primary**
track in the matching cone. The track-cluster matching parameters
(`fTrType`, `fTrClusDca`, `fNbMatchingTr`, `fTrTheta`, `fTrPhi`, `fTrMom`,
...) are then copied into the H1PartEm.

When the matched track happens to be an `H1PartSelTrack` (i.e. it
participated in the LW collection), `H1CreatePartEm::FindMatchingLWTrack`
iterates the H1PartSelTrack array (`modsEvent->GetArray(H1PartSelTrack::Class())`,
H1CreatePartEm.C:596) and the matched track's parent `H1PartCand` is the
same H1PartCand as the H1PartEm. The line that performs the EM/non-EM
filtering on selected tracks (H1CreatePartEm.C:600-601) is the canonical
H1OO idiom:
```cpp
if (LWtracks->GetParticle()->IsEm() == kTRUE) continue;
track = const_cast<H1Track*> (LWtracks->GetParticle()->GetTrack());
```

**Reference algorithm name / paper:** the LAr electron finder algorithm is
generally cited as the standard H1 NC DIS electron identification — the
nearest internal note is H1 NC DIS analyses (e.g. H1 inclusive NC HERA-II
paper, JHEP 09 (2012) 061 = DESY 12-107 = H1prelim-12-041); the H1OO source
header in `H1CreatePartEm.C:1-11` only credits "Emmanuel Sauvan, Judith
Katzy, 2001/11/26". No specific paper citation appears in the H1OO
sources.

### B.5 Actionable: how to query "is this track the scat-elec track?" from inside the builder

**The official H1OO idiom is pointer equality on the parent H1PartCand.**
Documented directly in `H1Mods/H1PartCand.C:686-688`:
```cpp
// E.g., if you want to check whether an H1PartMuon and an H1PartSelTrack
//        are 'the same', do
//   const H1PartCand *pCandMu    = muon->GetParticle();
//   const H1PartCand *pCandTrack = selTrack->GetParticle();
//   if (pCandMu == pCandTrack) ...
```

**For v5g — preferred recipe (cheapest, no traversal):**
Inside the track loop, simply test the propagated bit on the track's
parent H1PartCand:
```cpp
// inside: for (i = 0; i < nPSTTracks; i++) { H1PartSelTrack* trk = PST[i]; ... }
if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;
```
This works because `H1PartEm::SetIsScatElec()` (H1PartEm.h:310-313)
**always** propagates the bit to the parent `H1PartCand`. Since both the
scattered-electron H1PartEm and the matched H1PartSelTrack share the same
parent H1PartCand when (and only when) the LAr cluster has a matched
Lee-West track in the selected-track collection, the bit test on the
track's `GetParticle()` is the correct test.

`H1PartCand::IsScatElec()` is a pure `TestBit()` call
(H1PartCand.C:832-839) — O(1), no array dereference, no I/O.

**Alternative explicit recipe (pointer equality):**
```cpp
H1CalcElec* elecCalc = gH1Calc->Elec();
H1PartEmArrayPtr emArr;
Int_t scatIdx = elecCalc->GetIndex(0);     // index of scat elec on em array
if (scatIdx < 0) continue;                  // no scat elec selected
const H1PartCand* pCandScat = emArr[scatIdx]->GetParticle();

for (Int_t i = 0; i < nPSTTracks; i++) {
    H1PartSelTrack* trk = PST[i];
    if (trk->GetParticle() == pCandScat) continue;  // same particle → skip
    // ...
}
```
The H1CalcElec::GetIndex implementation that returns the scat-elec index is
in `H1CalcElec.C:441-541`: it sorts the EM array such that the
`kIsScatElec`-flagged H1PartEm is at index 0 (line 487).

**Recommendation for v5g:** use the bit-test recipe (§B.5 preferred). It
is the cheapest, requires no extra includes beyond
`H1Mods/H1PartSelTrack.h` (already included), and matches what
`H1CreatePartEm.C:600` itself does for the symmetric IsEm() case.

**Defensive caveat:** the bit propagation in H1PartEm.h:310-313 requires
that `H1PartEm::SetIsScatElec()` was called by H1OO (i.e. the
`H1CreatePartEm::SelectScatElec` step actually ran). For data and MC reco
in standard H1OO reads, this is always the case (the OO production runs
the EM finder). The defensive check `if (!trk->GetParticle()) continue;`
guards against the null-particle pathology only.

**Edge case to test:** when no H1PartSelTrack is matched to the scat-elec
cluster (no LW track in the matching cone), `trk->GetParticle()->IsScatElec()`
will never fire for any track. This is the *desired* behaviour: in that
case no charged track in PST belongs to the electron, so no track should be
vetoed. This matches the high-Q² physics — backward LAr clusters near the
barrel/forward boundary are often LAr-energy only with no matched primary
CJC track.

---

## C. Workspace quarantine status

### C.1 NAF source path and listing

SSH check executed: `ssh -O check naf-hone.desy.de` → `Master running
(pid=21568)` ✓.

NAF package root: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`
```
drwxr-xr-x. 14 klesthen h1 4096 May 22 18:01 .
... drwxr-xr-x.  3 klesthen h1 4096 May 21 00:40 bin
... -rw-r--r--.  1 klesthen h1 7036 May 22 18:44 binary_identity_manifest.json
... drwxr-xr-x.  6 klesthen h1 4096 May 22 17:04 condor
... drwxr-xr-x.  3 klesthen h1 4096 May 22 03:46 condor_workdir
... drwxr-xr-x.  2 klesthen h1 4096 May 21 00:45 LumiFiles
... -rw-r--r--.  1 klesthen h1 4322 May 21 00:43 Makefile
... drwxr-xr-x.  2 klesthen h1 4096 May 21 00:45 metadata
... drwxr-xr-x.  6 klesthen h1 4096 May 22 23:45 output
... drwxr-xr-x.  3 klesthen h1 4096 May 21 20:15 recovery
... drwxr-xr-x.  4 klesthen h1 4096 May 23 19:34 scripts
... drwxr-xr-x.  6 klesthen h1 4096 May 22 19:18 smoke_gate_work
... drwxr-xr-x.  2 klesthen h1 4096 May 21 05:00 src
... drwxr-xr-x.  4 klesthen h1 4096 May 22 17:04 steering
... drwxr-xr-x.  2 klesthen h1 4096 May 21 05:00 x86_64-el9-gcc14-opt
```
`src/` contents (NAF):
```
-rw-r--r--. 1 klesthen h1 84563 May 21 05:00 ChargedSidisTreeBuilderHighQ2.C
-rw-r--r--. 1 klesthen h1 69351 May 21 05:00 ChargedSidisTreeBuilderHighQ2.C.bak_v5b_pre_v5f_2026-05-19
-rw-r--r--. 1 klesthen h1  8945 May 21 05:00 ChargedSidisUtils_highQ2.h
-rw-r--r--. 1 klesthen h1  6013 May 21 05:00 ChargedSidisUtils_highQ2.h.bak_v5b_pre_v5f_2026-05-19
-rw-r--r--. 1 klesthen h1  6975 May 21 00:42 PhysicsConstants_chargedsidis_highQ2.h
```
The package is structurally separate from `H1ProtonAsym/`, `H1LambdaAsym/`,
`H1SIDIS_HighQ2/`, `H1SIDIS_HighQ2_v5{c,d,e,f}/`, `H1BaryonAsym/`,
`H1BaryonAsym_HighQ2/`, `H1XiAsym/`, `H1PhiMeson/` — all of which exist as
independent top-level directories under `BaryonAsym/`. No symlinks, no
shared `src/` directory.

### C.2 Source name (ProtonTree* vs ChargedSidisTree*)

The NAF source name has **already been renamed** to
`ChargedSidisTreeBuilderHighQ2.C`. The accompanying utils header is
`ChargedSidisUtils_highQ2.h`. Both `.bak_v5b_pre_v5f_2026-05-19` backups
also use the renamed names — i.e., the rename predates the v5f port.

The NAF source-file header (lines 1-11) explicitly documents the rename:
```
// ============================================================================
// ChargedSidisTreeBuilderHighQ2.C
//
// Renamed from ProtonTreeBuilderHighQ2.C during Phase 4 workspace migration
// (2026-05-19). Builder logic UNCHANGED. Only the filename, header
// includes (PhysicsConstants_chargedsidis_highQ2.h, ChargedSidisUtils_highQ2.h),
// and three banner cout strings were edited. ...
```
**No rename is needed for v5g; it is already done on NAF.** The action
implied is to refresh the local source_cache (see §C.3).

### C.3 Local snapshot freshness

The local cache is **stale by several revisions**:

| file | local cache | NAF authoritative |
|------|-------------|-------------------|
| name | `ProtonTreeBuilderHighQ2.C` | `ChargedSidisTreeBuilderHighQ2.C` |
| size | 62 509 B (1 279 lines)        | 84 563 B (~1 700+ lines) |
| mtime | May 14 23:34 (local)        | May 21 05:00 (NAF) |
| md5 | `6bfa3839f0bb7b179c3862e2897309eb` | `ee4e3c118d1ddcad160f96479837b1e1` |
| utils | `ProtonUtils_highQ2.h` (5 285 B, Apr 28) | `ChargedSidisUtils_highQ2.h` (8 945 B) |

The NAF source header (lines 12-25) lists v5f-era ports the local cache
does NOT have:
> v5f port (2026-05-19): TRUTH-LINK MATCHER + V0/conv-Dalitz veto + DIF-muon
> flags + STR-provenance diagnostic ported in one pass from the
> H1SIDIS_HighQ2_v5f reference workspace (Phases A+B+C+D = v5c V0-veto,
> v5d convenience flags + DIF-muon, v5e gen-denominator IsGtr() restore,
> v5f truth-link matcher with Tier 0 H1PartCand::GetPartMC() preferred /
> Tier 2 greedy 2-deg angular fallback). New branches on ChargedTot:
> VetoAncestorPDG, IsMatchedV0Daughter, IsMatchedConvOrDalitz, IsStr,
> IsMatchedK0LDaughter, IsMatchedDIFMuon, MatchTier, IsMatchedElectron,

The v5g spec was drafted against the stale `ProtonTreeBuilderHighQ2.C`
text. **Before writing v5g**, the local source_cache must be refreshed
from NAF so the diff base is correct. (This is a recommendation, not an
edit performed in this research.)

### C.4 Cross-package contamination check

Grep for foreign-package references in NAF `src/`
(`ChargedSidisTreeBuilderHighQ2.C`):
```
15:// H1SIDIS_HighQ2_v5f reference workspace (Phases A+B+C+D = v5c V0-veto,
43://   preselected by H1BaryonAsym_HighQ2 stage_evls through fEventListFiles.
123:// CLI helpers (mirroring H1BaryonAsym TreeBuilder.h::TreeUtil)
157:<< "responsibilities (stage_evls from H1BaryonAsym_HighQ2)." << endl;
```
All four hits are **comments only** — no `#include "../H1.../foo.h"`, no
relative-path source compilation, no shared headers across packages. The
references are documentary (citing upstream pre-selection by
`H1BaryonAsym_HighQ2/stage_evls`).

A grep with `\.\./` (relative path includes that would reach sibling
packages) returned no source-level matches. The package is
**source-quarantined**.

### C.5 Makefile / build target check

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/Makefile`
(4 322 B), salient excerpts:
```make
MODULENAME = H1ChargedSidisHighQ2Standalone
SLIB       = libChargedSidisHighQ2User.so
OBJS =                      # no shared-library objects
BIN  = ChargedSidisTreeBuilderHighQ2

EXTRA_INCLUDEDIRS = src     # ONLY src/ added to include path
vpath %.C src               # source search only in src/
```
Library set (lines below): exclusively the public H1OO library set
(`-lH1Geom -lH1Ods -lH1Mods -lH1Hat -lH1Calculator -lH1PhysUtils ...`) +
ROOT + Centauro (from `$(FJCONTRIB__HOME)`) + LHAPDF + FORTRAN libs. **No
`-I../H1ProtonAsym/src`, no `-L../H1.../lib`, no foreign source files in
`vpath`.** The link target is:
```make
ChargedSidisTreeBuilderHighQ2: outputdirs $(BUILDDIR)/ChargedSidisTreeBuilderHighQ2.o
    $(LOADER) $(BUILDDIR)/ChargedSidisTreeBuilderHighQ2.o $(ALL_LIBS) -o $(BINDIR)/$@
```
Only the one `.o` from this package's `src/` is linked.

**One asymmetry to note (not a leak, but worth flagging):** the Makefile
header (lines 31-43) documents that `BINDIR` resolves to the *shared*
`BaryonAsym/bin/x86_64-el9-gcc14-opt/` directory (because `BINDIR =
$(MAKEDIR)/../bin/$(SYSTEM)` per H1Basic.mk). The new binary
`ChargedSidisTreeBuilderHighQ2` lands in the same output directory as the
legacy `ProtonTreeBuilderHighQ2` binary, but they are *distinct*
executable files (different names). This is intentional per the Makefile
comment. The local `H1ChargedSidis_HighQ2/x86_64-el9-gcc14-opt/` directory
exists (May 21 05:00) and holds only the build objects, not the binary.

### C.6 Summary

The NAF `H1ChargedSidis_HighQ2/` package **is properly quarantined** at
the source, header, Makefile, and library levels. The only cross-package
references are comments documenting the upstream event-list dependency.
The local `source_cache/` is stale (still using pre-rename
`ProtonTreeBuilderHighQ2.C`, and missing the v5f ports). The action item
is a local cache refresh, not a NAF rename.

---

## D. H1Calculator::ScatElectron documentation

The current builder accesses the scattered electron through HAT pointers:
```cpp
H1FloatPtr hatElecTheta("ElecTheta");    // line ~216-218 of cache
H1FloatPtr hatElecPhi("ElecPhi");
H1FloatPtr hatElecE("ElecE");
```
These HAT branches are filled by `H1HatFiller::Fill()`
(`H1Filler/H1HatFiller.C:912-923`):
```cpp
hatEvent->fElecE = 0;
hatEvent->fElecTheta = 0;
hatEvent->fElecPhi = 0;
for (Int_t i = 0; i < emParts.GetEntries(); i++) {
    if (emParts[i]->GetParticle()->IsScatElec()) {
        hatEvent->fElecE     = emParts[i]->GetE();
        hatEvent->fElecTheta = emParts[i]->GetTheta();
        hatEvent->fElecPhi   = emParts[i]->GetPhi();
    }
}
```
i.e. these HAT branches are nothing more than projections of the same
`kIsScatElec`-flagged H1PartEm that the EM finder
(H1CreatePartEm::SelectScatElec) selected. The HAT branches are
*equivalent* to the H1Calculator path, just pre-projected to scalars.

**Richer accessors in H1Calculator** (live, in-memory, with object link):

- `H1Calculator::Instance()->Elec()` → `H1CalcElec*` (declared
  `H1Calculator/H1Calculator.h:141`).
- `H1CalcElec::GetFirstElectron()` → `const TLorentzVector&`
  (H1CalcElec.h:376). This is the calibrated 4-vector of the scat elec.
- `H1CalcElec::GetFirstElectronCluster()` → `const TLorentzVector&`
  (H1CalcElec.h:388) — uncalibrated cluster 4-vector.
- `H1CalcElec::GetFirstElectronGen()` → `const TLorentzVector&`
  (H1CalcElec.h:391) — gen-level scat elec 4-vector.
- `H1CalcElec::GetIndex(Int_t n=0)` → `Int_t` (H1CalcElec.h:305 / impl
  H1CalcElec.C:441-541) — returns the index of the scat-elec H1PartEm on
  the H1PartEmArray. With this:
  ```cpp
  H1PartEmArrayPtr emArr;
  Int_t idx = H1Calculator::Instance()->Elec()->GetIndex(0);
  H1PartEm* scatEm = (idx >= 0) ? emArr[idx] : nullptr;
  const H1PartCand* scatCand = scatEm ? scatEm->GetParticle() : nullptr;
  ```
- `H1CalcElec::GetClusterPosition(Int_t n)` → `const TVector3&` (h:415).
- `H1CalcElec::GetCharge(Int_t)`, `GetType(Int_t)`, `GetXClus/YClus/ZClus`,
  `GetDeltaXTrCl/DeltaYTrCl/DeltaZTrCl`, etc.

**v5g implication:** the builder *could* (optionally) switch from HAT
pointers to the live H1Calculator path. This would give simultaneous
access to the 4-vector AND the H1PartEm AND the parent H1PartCand. The
current HAT-pointer code in the builder (cache lines 216-218, 634-640) is
*functionally equivalent* and slightly more efficient (no array
dereferencing), so a switch is optional. However, if v5g wants the
scat-elec H1PartCand pointer for the track-veto in §B.5, it should add the
`H1CalcElec::GetIndex(0)` + `H1PartEmArrayPtr` lookup, OR rely on the
bit-propagation idiom `trk->GetParticle()->IsScatElec()`. The latter is
the simplest path.

---

## E. Q3 clarification: writing the flag NOT to ChargedTot

### E.1 DisTot per-event scat-elec content

There is **no separate `DisTot` tree** in the current builder. The
`scatEX/EY/EZ/EE` per-event four-vector is written as **branches on each
of the existing per-track trees**:

- `ProtonTot`: declared as `protonTree->Branch("scatEX", ...)` etc., at
  lines 385-388 of the cache. Filled once per event (lines 637-640) and
  carried as the same value on every ProtonTot row of that event.
- `ChargedTot`: declared at lines 488-491, same per-event semantics.

So the scat-elec four-vector is already available per-event — it just
happens to be denormalised into every ProtonTot / ChargedTot row.
Downstream analyses read it as `event[scatEX]` of the first (or any) row
in the event. **No extra event-level branch or DisTot tree is required to
preserve the scat-elec info if the track row is removed.**

### E.2 ChargedTotGen gen-electron filter check

The ChargedTotGen loop is at lines 855-895 of the cache. The species
filter is at line 862:
```cpp
for (int ig = 0; ig < gtrdata.GetEntries(); ig++) {
    H1PartMC* gp = gtrdata[ig];
    // Fix 3: restrict to pi, K, p — exclude leptons and hyperons
    if (!ProtonUtil::isChargedHadron(gp->GetPDG())) continue;
    if (!gp->IsGtr()) continue;  // GTR only
    ...
}
```
The helper `ProtonUtil::isChargedHadron(int pdg)` (defined in
`source_cache/ProtonUtils_highQ2.h`, also referenced in the v5f-renamed
`ChargedSidisUtils_highQ2.h` per the NAF naming) restricts to
`|PDG| ∈ {211, 321, 2212}` (pi±, K±, p/pbar). **Gen electrons (PDG ±11) are
already excluded.** No parallel fix is required on the gen side.

The `IsGtr()` filter (line 863) further restricts to genuine final-state
GTR particles, and the `gp->GetMother2() != -1` filter (line 868) excludes
secondary-interaction products. The gen-side denominator is therefore
clean of gen electrons by construction.

### E.3 v5g change recipe (per Q3 interpretation)

In the H1PartSelTrack loop (cache lines 944-1259), **add at the top
(after the H1PartSelTrack pointer extraction at line 945)**:
```cpp
H1PartSelTrack* trk = PST[i];
// v5g: skip the track associated with the scattered electron.
// (Bit set by H1PartEm::SetIsScatElec via parent H1PartCand propagation.)
if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;
```
The skip applies to **both** ChargedTot AND ProtonTot fills (both share
this loop body in the current builder). The per-event scat-elec
four-vector (`scatEX/EY/EZ/EE`) remains written on the surviving rows of
each event, fully preserving the downstream interface. No row is written
for the scat-elec track, which is exactly the Q3 clarification.

**Gen side:** no change needed (see §E.2).

---

## Headline answers (1 paragraph each)

### Task A — gen electron-method ≡ gen eSigma method at closed gen level?
**Identical.** From the H1OO `H1MakeKine` source itself
(`H1PhysUtils/H1MakeKine.h:72-74`), `Q²_es ≡ Q²_e` by definition, and at
gen level with 4-momentum conservation in the gen final state
`Σ_h = 2·E_beam_e − E'(1−cos θ')`, which makes `y_s = y_e` and therefore
`x_es = x_s = x_e` and `y_es = y_e`. The v5g spec's chosen
electron-method-only form for `x_es_gen / Y_es_gen / Q²_es_gen` (using
`genElec + GenEnPhoton`) is numerically equivalent to a true gen eSigma
method that uses the gen HFS Σ. No accuracy is lost. No H1OO test code or
internal note was found that compares them numerically; the algebraic
identity from the source is the H1OO position.

### Task B — H1OO API path + how to query "is THIS track the scat-elec?"
The scattered-electron H1PartEm is selected by
`H1CreatePartEm::SelectScatElec()` (`H1PartEmFinder/H1CreatePartEm.C:97-167`)
and flagged with `H1PartEm::kIsScatElec = BIT(14)`. The setter
`H1PartEm::SetIsScatElec()` (H1PartEm.h:310-313) propagates the bit to the
parent `H1PartCand` (`kIsScatElec = BIT(23)`). The **actionable query** for
v5g is the single-line bit test on the track's parent particle:
```cpp
if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;
```
`H1PartCand::IsScatElec()` (H1PartCand.C:832-839) is a pure `TestBit()` —
O(1), no array dereference. The track and the scat-elec H1PartEm share
their parent H1PartCand whenever the LAr cluster has a Lee-West-matched
selected track (the high-Q² LAr matching is `FindMatchingLWTrack` in
`H1CreatePartEm.C:561-695`, DCA < 12 cm). If no track is matched, the
test simply never fires for any PST track — desired behaviour.

### Task C — quarantined Y/N + any leaks
**Yes, the NAF package is properly quarantined.** Source under
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/` is
self-contained; the Makefile (`MODULENAME = H1ChargedSidisHighQ2Standalone`,
`vpath %.C src`, `EXTRA_INCLUDEDIRS = src`) compiles only files in `src/`;
no `-I../H1.../src` flags, no `vpath` to foreign trees. Cross-package grep
returned only comment-level references to `H1BaryonAsym_HighQ2`
(documenting upstream event-list dependency). The NAF source is **already
renamed** to `ChargedSidisTreeBuilderHighQ2.C` (84 563 B, May 21) — the
rename happened on 2026-05-19. **The local `H1/sidis_highQ2/source_cache/`
is stale** (still named `ProtonTreeBuilderHighQ2.C`, 62 509 B, May 14, MD5
mismatch, missing v5f ports). Recommendation: refresh the local
source_cache from NAF before drafting the v5g implementation, so the diff
base is the v5f-era code, not the pre-rename v5b code.

### Task D — alternate accessors found
The HAT branches `ElecE/ElecTheta/ElecPhi` are filled by
`H1HatFiller::Fill()` (H1HatFiller.C:912-923) by projecting the
`kIsScatElec`-flagged H1PartEm. The richer alternatives in H1Calculator
are `H1Calculator::Instance()->Elec()->GetFirstElectron()` (4-vector,
H1CalcElec.h:376), `GetFirstElectronCluster()` (uncalib cluster 4-vec,
h:388), `GetFirstElectronGen()` (gen-level 4-vec, h:391),
`GetIndex(0)` to retrieve the H1PartEm array index (h:305, impl
H1CalcElec.C:441-541), plus full cluster-position, charge, type, and
track-cluster delta accessors. The v5g builder does NOT need to switch
away from the HAT pointers (they are equivalent and slightly cheaper);
the scat-elec H1PartCand pointer for the track veto is obtained more
cheaply via the bit-propagation idiom (Task B answer) rather than the
H1CalcElec lookup.

### Task E — confirmed / changes needed
**Confirmed.** No new event-level tree (DisTot) is needed — the per-event
`scatEX/EY/EZ/EE` four-vector is already written on every row of
ProtonTot and ChargedTot (cache lines 385-388, 488-491, 637-640), so
removing the scat-elec track's row from ChargedTot/ProtonTot loses no
information. The gen-side ChargedTotGen loop (cache lines 855-895) already
filters to charged hadrons (`|PDG| ∈ {211, 321, 2212}`) via
`ProtonUtil::isChargedHadron()` at line 862, so gen electrons cannot appear
in ChargedTotGen — no parallel fix is required on the gen side. The v5g
implementation reduces to inserting one `continue` after the
H1PartSelTrack pointer extraction in the reco loop.

---

## Blockers / risks / surprises

- **Local source_cache is stale.** The local `H1/sidis_highQ2/source_cache/`
  is two days and one major port behind NAF. The v5g spec was drafted
  against the stale code, so cross-references in the spec (file paths,
  line numbers) may point to the wrong line in the NAF source. Refresh
  the cache from NAF before writing v5g.
- **`source_cache` filename mismatch.** Local file is
  `ProtonTreeBuilderHighQ2.C` (pre-rename name); NAF file is
  `ChargedSidisTreeBuilderHighQ2.C`. Update both the local cache name
  and any spec references when refreshing.
- **No DisTot tree exists.** The v5g spec wording about "writing flag
  not to ChargedTot" is satisfied by the existing per-event scat-elec
  branches on each row of ChargedTot and ProtonTot — no new tree
  required.
- **No H1OO numerical test of gen-eSigma vs gen-electron.** The
  identity claim rests on the algebraic identity in H1MakeKine. A
  one-off numerical check on a few MC events is recommended but not
  blocking.
- **High-Q² LAr-only events with no matched LW track:** the bit-test
  veto correctly does nothing in this case (no PST track carries the
  scat-elec bit). Verify in MC truth that this is the desired behaviour
  — for high-Q² LAr scat elec, the dominant case will be no PST track
  match (LAr cluster only), in which case the veto is a no-op and no
  charged hadron yield is biased.

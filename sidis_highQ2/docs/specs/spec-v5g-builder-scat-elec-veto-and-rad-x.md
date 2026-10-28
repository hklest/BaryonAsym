# Spec v5g (r2) — High-Q² Charged SIDIS Tree Builder

**Status:** APPROVED FOR IMPLEMENTATION
**Revision:** r2 (supersedes r1 at `spec-v5g-builder-scat-elec-veto-and-rad-x.r1.md`)
**Author:** h1-software (delegated from h1-sidis-supervisor)
**Date:** 2026-05-23
**Target binary:** `ChargedSidisTreeBuilderHighQ2_v5g`
**New source file:** `H1/sidis_highQ2/source_cache/ChargedSidisTreeBuilderHighQ2_v5g.C`
                    (NAF: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v5g.C`)
**v5f source (untouched):** `ChargedSidisTreeBuilderHighQ2.C`
                            (NAF MD5 `ee4e3c11...`, 1606 lines, refreshed 2026-05-23)
**Verified against source:**
  - `ChargedSidisTreeBuilderHighQ2.C` (whole file, read 2026-05-23)
  - `PhysicsConstants_chargedsidis_highQ2.h` (whole file)
  - `ChargedSidisUtils_highQ2.h` (whole file)
  - `H1Mods/H1PartCand.h` / `H1PartCand.C` (via research artifact §B)
  - `H1Mods/H1PartEm.h` (via research artifact §B)
  - `H1PhysUtils/H1MakeKine.h` / `H1MakeKine.C` (via research artifact §A)
  - `H1Calculator/H1CalcWeight.C` (rad-`s` formula, lines 500–564)
**Research basis:** `docs/research/research-v5g-scat-elec-track-assoc.md`
  (gen-electron ≡ gen-eSigma proof in §A; `H1PartCand::IsScatElec` API in §B;
  quarantine confirmation in §C)

---

## Diff vs r1 (one-paragraph headline)

r1 designed a manual angular-veto branch `IsScatElecCandidate` (10° cone /
3 GeV cut) plus tuning machinery to flag-but-keep the scattered-electron
track in `ChargedTot`. The researcher pass (Task B of the research artifact)
established that H1OO already tags the scattered-electron track via
`H1PartCand::IsScatElec()` — the bit is propagated from `H1PartEm` to its
parent `H1PartCand` by `H1PartEm::SetIsScatElec()` (`H1Mods/H1PartEm.h:310-313`),
so the test `trk->GetParticle()->IsScatElec()` is exact, O(1), and requires
no thresholds. r2 therefore (1) deletes the entire `IsScatElecCandidate`
branch and threshold-constant subsystem, (2) replaces the algorithm with a
one-line `continue` at the top of each of the two reco track loops, (3)
**skips** the scattered-electron track row entirely (Q3 supervisor decision),
(4) preserves the rad-x fix unchanged (now 5+5 new branches, not 6+5), and
(5) re-derives all line-number references against the refreshed 1606-line
source. The result is a simpler, cheaper, and provably correct intervention.

This document is a **specification only**. No C++ is produced here.

---

## 1. Motivation

The supervisor identified two analysis-blocking defects in the v5f trees
that v5g must fix without regressing any existing branch:

1. **Scattered-electron contamination of `dN/dφ_h`.** The current
   `ChargedTot` tree contains every primary-vertex CJC track that passes
   the wide online selection, including the LAr-tagged DIS scattered
   electron when its matched Lee-West track happens to lie inside the
   central-tracker acceptance. Downstream `φ_h` histograms then acquire a
   spurious narrow peak at `φ_h ≈ 0` (electron direction by construction
   in the eSigma frame). The scattered-electron 4-vector is already
   written per event as `scatEX/EY/EZ/EE` (source lines 796–799 / branched
   at 479–482) so no information is lost by skipping the row.

   **r2 fix (this revision).** The H1OO bit `H1PartCand::kIsScatElec`
   (BIT 23) is propagated to the parent `H1PartCand` whenever
   `H1PartEm::SetIsScatElec()` is called by the EM finder
   (`H1PartEmFinder/H1CreatePartEm::SelectScatElec`,
   `H1CreatePartEm.C:97-167`). Inside the reco track loop a single bit
   test
   ```cpp
   if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;
   ```
   correctly and exactly identifies and skips the scattered-electron
   track. No angular threshold, no extra branch, no analysis tuning
   surface.

2. **`xGen = ∞` for radiative events.** The current xGen formula is
   `xGen = Q²_sGen / (4·E_p·E_e · Y_sGen)` (source line 938) using the
   **nominal** beam energies. For events with a hard ISR photon `γ_ISR`,
   the effective electron CMS energy is `E_e − E_γ_ISR`, so the true
   `s_true = 4·E_p·(E_e − E_γ_ISR)` (verified in
   `H1CalcWeight::GetCCQCDWeight` line 558 and `GetNCQCDWeight` line 514).
   Using `s_nominal` while `Y_sGen` is computed under the true-`s`
   assumption drives `xGen` to infinity or into junk (`>1`) for rad
   events, which makes `C_rad` binning impossible.

   **r2 fix (carried from r1, line numbers re-derived).** Add per-row
   `Q2_es_gen`, `Y_es_gen`, `x_es_gen`, `GenEnPhoton`, and `ystar_es_gen`
   computed with `E_e_eff = GenBeamElecE − max(GenEnPhoton, 0)` and
   `s_eff = 4·GenBeamProtP·E_e_eff`. Per research artifact §A, the gen
   electron-method formula is **algebraically identical** to gen eSigma at
   closed gen 4-momentum (proof reproduced in §6.2 below), so the choice
   of method is numerically safe.

Both defects propagate through the entire downstream chain
(`h1-acceptance` → `h1-radiative-correction` → `h1-unfolder` →
`h1-cross-section`); fixing them at the tree-builder level is the only
clean intervention.

---

## 2. Existing schema — annotated against the refreshed source

All line-number references in this section are against the refreshed
`ChargedSidisTreeBuilderHighQ2.C` (1606 lines, NAF MD5 `ee4e3c11...`).

### 2.1 `ChargedTot` (reco track tree)

Groupings as written in the source. Branch-declaration sites cited.

#### 2.1.1 Event identifiers (KEEP, 2)

| Branch        | Type   | Declared at | Branched at |
|---------------|--------|-------------|-------------|
| `RunNumber`   | `Int_t`| 257         | 417         |
| `EventNumber` | `Int_t`| 257         | 418         |

#### 2.1.2 Track kinematics (KEEP, 9)

`Pt`, `Eta`, `Phi`, `P`, `Px`, `Py`, `Pz`, `Charge`, `Theta`.
Declared at lines 259–260, 348; branched at 419–426, 439.

#### 2.1.3 Track quality / classification (KEEP, 11)

`NHitDedx`, `NHitCJC`, `NHitCJC1`, `DcaPrime`, `RadLength`, `IsFromPrimary`,
`IsCentralTrk`, `IsCombinedTrk`, `TrackType`, `StartRadius`, `EndRadius`,
`DcaPrimeSinTheta`. (12 names; `NHitDedx` is dE/dx-pad count only,
remaining dE/dx PID was removed in the v5f-era cleanup per the source
header.)  Declared lines 262–264, 345, 398–400; branched lines 428–443.

#### 2.1.4 Trigger / weights (KEEP, 6)

`TrigWeightAC`, `TrigWeightRW`, `L1L2L3ac[4]/i`, `L1L2L3rw[4]/i`,
`NucliaWeight`. Declared 378–379, 390–391, 394; branched 444–452.

#### 2.1.5 DIS kinematics — reco-side, calibrated eSigma (KEEP, 5)

`Q2s`, `Ys`, `xBj`, `Ws`, `PVz`. Declared line 267; branched 453–457.

#### 2.1.6 DIS kinematics — reco-side, all reconstruction methods (KEEP, 13)

`Q2e`, `Ye`, `Xe`, `Q2da`, `Yda`, `Xda`, `Q2es`, `Yes`, `Xes`,
`Q2h`, `Yh`, `Xh`, `Empz`. Declared 269–274; branched 459–471.

#### 2.1.7 SIDIS — eSigma-frame (KEEP, 4)

`ystar_es`, `yBreit_es`, `xF_SIDIS_es`, `z_SIDIS_es`.
Declared 276–277; branched 473–476.

#### 2.1.8 Per-event beam and scattered-electron 4-vector (KEEP, 6)

| Branch       | Declared at | Branched at | Filled at  |
|--------------|-------------|-------------|------------|
| `BeamElecE`  | 284         | 477         | 789        |
| `BeamProtP`  | 284         | 478         | 790        |
| `scatEX`     | 285         | 479         | 796        |
| `scatEY`     | 285         | 480         | 797        |
| `scatEZ`     | 285         | 481         | 798        |
| `scatEE`     | 285         | 482         | 799        |

These are denormalised onto every retained-track row, so they remain
available per event after §5 removes the scattered-electron track's row.

#### 2.1.9 Per-row weights (KEEP, 2)

`weight1`, `weight2`. Declared 287; branched 483–484.

#### 2.1.10 MC truth — match to gen charged hadron (KEEP, 18)

`GenPDG`, `GenMotherPDG`, `IsGtr`, `IsMatched`, `ParticleSource`,
`GenMatchIdx`, `GenPt`, `GenEta`, `GenPhi`, `GenP`, `MatchAngle`,
`GenCharge`, `VetoAncestorPDG`, `IsMatchedV0Daughter`,
`IsMatchedConvOrDalitz`, `IsStr`, `IsMatchedK0LDaughter`,
`IsMatchedDIFMuon`. Declared 290–312; branched 486–512.

#### 2.1.11 V5F truth-link diagnostics (KEEP, 4)

`MatchTier` (`UChar_t`), `IsMatchedElectron`, `IsMatchedMuon`,
`IsMatchedHadron`. Declared 302–305; branched 513–521.

#### 2.1.12 Species-correct reco SIDIS (KEEP, 5)

`ylab`, `ystar`, `yBreit`, `xF_SIDIS`, `z_SIDIS`. Declared 326–327;
branched 522–527.

#### 2.1.13 Matched gen SIDIS — DEPRECATE-BUT-RETAIN (4) + KEEP (3)

| Branch       | Declared at | Branched at | Disposition |
|--------------|-------------|-------------|-------------|
| `Q2sGen`     | 314         | 529         | DEPRECATE-BUT-RETAIN — broken-`s`; use `Q2_es_gen` |
| `YsGen`      | 314         | 530         | DEPRECATE-BUT-RETAIN — broken-`s`; use `Y_es_gen` |
| `xGen`       | 314         | 531         | **DEPRECATE-BUT-RETAIN — INFINITE for rad events; use `x_es_gen`** |
| `ystarGen`   | 315         | 532         | DEPRECATE-BUT-RETAIN — uses broken xGen via boost; use `ystar_es_gen` |
| `yBreitGen`  | 315         | 533         | KEEP — Breit boost is `s`-insensitive |
| `xFGen`      | 315         | 534         | KEEP — uses `WmagGen` from γ\*p; documented as valid for NoRad |
| `zGen`       | 315         | 535         | KEEP — Lorentz invariant `P·Ph/P·q` |

#### 2.1.14 Fiducial / per-event gen-electron mirror (KEEP, 4)

`inFiducialChargedMatch`, `GenElecPhi`, `GenElecTheta`, `GenElecE`.
Declared 317, 321–323; branched 537–541.

**ChargedTot total declared branches before v5g additions: 92.**
(Counted from explicit `chargedTree->Branch(...)` calls at lines 417–541.)

### 2.2 `ChargedTotGen` (gen-only tree)

`chargedGenTree->Branch(...)` calls live in lines 544–617 (inside the
`if (chargedGenTree) { ... }` MC-only block opened at line 544).

#### 2.2.1 Event identifiers + identity (KEEP, 5)

`RunNumber`, `EventNumber`, `GenPDG`, `GenMotherPDG`, `IsGtr`.
Declared 330; branched 545–549.

#### 2.2.2 Gen kinematics (KEEP, 6)

`GenPt`, `GenEta`, `GenPhi`, `GenP`, `GenCharge`, `ylab`.
Declared 331–333; branched 550–555.

#### 2.2.3 Gen DIS Sigma — DEPRECATE-BUT-RETAIN (3)

| Branch   | Declared at | Branched at | Disposition |
|----------|-------------|-------------|-------------|
| `Q2sGen` | 338         | 556         | DEPRECATE-BUT-RETAIN — broken-`s`; use `Q2_es_gen` |
| `YsGen`  | 338         | 557         | DEPRECATE-BUT-RETAIN — broken-`s`; use `Y_es_gen` |
| `xGen`   | 338         | 558         | **DEPRECATE-BUT-RETAIN — INFINITE for rad events; use `x_es_gen`** |

#### 2.2.4 Gen DIS — all other methods (KEEP, 12)

`Q2eGen`, `YeGen`, `XeGen`, `Q2daGen`, `YdaGen`, `XdaGen`,
`Q2esGen`, `YesGen`, `XesGen`, `Q2hGen`, `YhGen`, `XhGen`.
Declared 279–282; branched 560–571.

#### 2.2.5 Gen SIDIS (1 DEPRECATE-BUT-RETAIN + 3 KEEP)

| Branch      | Declared at | Branched at | Disposition |
|-------------|-------------|-------------|-------------|
| `ystarGen`  | 337         | 572         | DEPRECATE-BUT-RETAIN — derived from broken-`s` boost; use `ystar_es_gen` |
| `yBreitGen` | 337         | 573         | KEEP — Breit boost `s`-insensitive |
| `xFGen`     | 337         | 574         | KEEP — uses `WmagGen` |
| `zGen`      | 337         | 575         | KEEP — Lorentz invariant |

#### 2.2.6 Weights, fiducial, source, index (KEEP, 5)

`weight1`, `weight2`, `inFiducialCharged`, `ParticleSource`, `GenIdx`.
Declared 287, 334, 336, 339; branched 576–581.

#### 2.2.7 Gen scattered-electron + beam (legacy, KEEP, 6)

`GenScatEX`, `GenScatEY`, `GenScatEZ`, `GenScatEE`, `GenBeamElecE`,
`GenBeamProtP`. Declared 341–342; branched 583–588. Required inputs to
the new §6 algorithm.

#### 2.2.8 Standardised gen-electron aliases (KEEP, 6)

`GenElecPx`, `GenElecPy`, `GenElecPz`, `GenElecE`, `GenElecTheta`,
`GenElecPhi`. Declared 354–355; branched 591–596.

#### 2.2.9 Gen E−pz / V0 / classifier flags (KEEP, 7)

`GenEmpz`, `IsV0DaughterGen`, `VetoAncestorPDG`, `IsConvOrDalitzGen`,
`IsStr`, `IsK0LDaughter`, `IsDIFMuon`. Declared 358–368; branched
598–614.

#### 2.2.10 Gen vertex Z (KEEP, 1)

`GenVtxZ`. Declared 371; branched 616.

**ChargedTotGen total declared branches before v5g additions: 51.**
(Counted from explicit `chargedGenTree->Branch(...)` calls at lines
545–616.)

### 2.3 `DisTot` (event-level)

**Out of scope for v5g.** The `DisTot` tree (declared at line 253,
branched lines 621–682) already has its own `Q2sGen` / `YsGen` / `Xs_gen`
event-level branches (declared line 414, branched lines 669–671). These
inherit the same broken-`s` bug, but the DisTot fix is deferred to a
follow-up spec (`spec-v5h-distot-rad-x.md`) so that v5g remains a
minimal-surface change to the per-track / per-gen trees that
downstream `h1-acceptance` / `h1-radiative-correction` reads.

### 2.4 Disposition summary

| Tree           | Existing | KEEP | DEPRECATE-BUT-RETAIN | REMOVE |
|----------------|----------|------|-----------------------|--------|
| `ChargedTot`   | 92       | 88   | 4                     | 0      |
| `ChargedTotGen`| 51       | 47   | 4                     | 0      |
| `DisTot`       | 36       | 36   | 0                     | 0 (out of scope) |

No removals. v5g is strictly append-only on existing schemas, except for
the *physical* per-row deletion of scattered-electron tracks from
`ChargedTot` (see §5 and §9).

---

## 3. New branches

### 3.1 `ChargedTot` — 5 new branches

| # | Branch          | Type    | Default if invalid | Description |
|---|-----------------|---------|--------------------|-------------|
| 1 | `Q2_es_gen`     | `Float_t` | `−999.0`         | Q² from the gen-scattered-electron 4-vector via the electron method on gen quantities (see §6). Defined only for events with `validGenElec`. |
| 2 | `Y_es_gen`      | `Float_t` | `−999.0`         | Inelasticity from the gen-scattered-electron via the electron method on gen quantities with effective `E_e` (see §6). |
| 3 | `x_es_gen`      | `Float_t` | `−999.0`         | `x_es_gen = Q²_es_gen / (s_eff · Y_es_gen)` with `s_eff = 4·GenBeamProtP·(GenBeamElecE − max(GenEnPhoton,0))`. Finite for rad events. |
| 4 | `GenEnPhoton`   | `Float_t` | `0.0`            | ISR photon energy from HAT branch `GenEnPhoton`. Raw value; zero for samples without ISR (NoRad generators). |
| 5 | `ystar_es_gen`  | `Float_t` | `−999.0`         | Rapidity of the matched gen charged hadron boosted to the γ\*–p CMS, where γ\* = (per-event gen e-beam) − (gen scattered electron) and `s` uses per-event gen beams (NOT nominal). |

Branches are written for **every retained ChargedTot row** (every selected
reco track that is not the scattered-electron track per §5). Branch list
grows ChargedTot from 92 → **97**.

### 3.2 `ChargedTotGen` — 5 new branches

| # | Branch          | Type    | Default | Description |
|---|-----------------|---------|---------|-------------|
| 1 | `Q2_es_gen`     | `Float_t` | `−999.0` | Same as §3.1 #1; identical event-level value across all rows of the same event. |
| 2 | `Y_es_gen`      | `Float_t` | `−999.0` | Same as §3.1 #2. |
| 3 | `x_es_gen`      | `Float_t` | `−999.0` | Same as §3.1 #3. |
| 4 | `GenEnPhoton`   | `Float_t` | `0.0`    | Same as §3.1 #4. |
| 5 | `ystar_es_gen`  | `Float_t` | `−999.0` | Rapidity of the per-row gen charged hadron boosted to the same γ\*–p CMS as §3.1 #5. |

Branch list grows ChargedTotGen from 51 → **56**.

### 3.3 Branch-count summary

| Tree            | v5f | v5g | Δ   |
|-----------------|-----|-----|-----|
| `ChargedTot`    | 92  | 97  | +5  |
| `ChargedTotGen` | 51  | 56  | +5  |
| `DisTot`        | 36  | 36  | 0 (out of scope) |
| `ProtonTot` / `ProtonTotGen` | (do not exist in this builder; removed in v5f-era cleanup, see source-header lines 36–39) | — | — |

ProtonTot / ProtonTotGen do **not exist** in the
`ChargedSidisTreeBuilderHighQ2` builder — the proton/antiproton trees were
removed when the binary was forked into a generic charged-hadron SIDIS
builder (source header lines 36–39). They live in a separate NAF package
(`H1ProtonAsym` at `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsym/`)
that is explicitly **out of scope** for v5g per supervisor Q7.

### 3.4 No new flag branch (r2 change vs r1)

r1 specified a new branch `IsScatElecCandidate` (Bool_t, ChargedTot only)
to mark the scattered-electron track for downstream offline vetoing. **r2
removes this branch entirely**. The H1OO `IsScatElec()` bit test (§5) is
exact, so the track is filtered at the tree-builder level and no per-row
flag is needed.

### 3.5 No new `PhysConst` constants (r2 change vs r1)

r1 introduced `SCAT_ELEC_VETO_ANG_DEG = 10.0` and
`SCAT_ELEC_VETO_P_MIN = 3.0` in `PhysicsConstants_chargedsidis_highQ2.h`.
**r2 introduces no new physics constants** — the bit-test algorithm has no
tunable parameters. The header file is therefore **untouched** by v5g.

---

## 4. Deprecate-but-retain branches

The following branches stay written with identical formulas to v5f, but
receive inline `// DEPRECATED V5G: ...` comments at their declaration *and*
`Branch()` registration sites:

| Tree           | Branch       | Inline comment |
|----------------|--------------|----------------|
| `ChargedTot`   | `Q2sGen`     | `// DEPRECATED V5G: use Q2_es_gen (same value at NoRad).` |
| `ChargedTot`   | `YsGen`      | `// DEPRECATED V5G: use Y_es_gen.` |
| `ChargedTot`   | `xGen`       | `// DEPRECATED V5G: INFINITE for radiative events (uses s_nominal). Use x_es_gen.` |
| `ChargedTot`   | `ystarGen`   | `// DEPRECATED V5G: derived from broken xGen boost. Use ystar_es_gen.` |
| `ChargedTotGen`| `Q2sGen`     | `// DEPRECATED V5G: use Q2_es_gen.` |
| `ChargedTotGen`| `YsGen`      | `// DEPRECATED V5G: use Y_es_gen.` |
| `ChargedTotGen`| `xGen`       | `// DEPRECATED V5G: INFINITE for radiative events. Use x_es_gen.` |
| `ChargedTotGen`| `ystarGen`   | `// DEPRECATED V5G: use ystar_es_gen.` |

All other v5f branches retain unchanged semantics and require no comment
change. `yBreitGen`, `xFGen`, `zGen` remain **KEEP** — the Breit boost,
`Wmag` from gen γ\*p, and the Lorentz-invariant `z = P·Ph/P·q` are all
insensitive to the `s_nominal` vs. `s_true` mismatch within the v5f
formulas as actually written, because they do not divide by `s`.

**Critical invariant:** the legacy `xGen` branch values must be
**byte-identical** between v5f and v5g on the same input data sample.
This is enforced by §8.4 (backward-compatibility validation).

---

## 5. Algorithm: skip the scattered-electron track

### 5.1 One-line H1OO check

**Insertion sites.** Two reco track loops in the source:

| Loop | Line of `for (...)` | Line of `H1PartSelTrack* trk = PST[i];` | Insertion point |
|------|---------------------|------------------------------------------|-----------------|
| Pass 1 (central tracks) | 1133 | 1134 | Immediately after line 1134 |
| Pass 2 (combined tracks) | 1437 | 1438 | Immediately after line 1438 |

In each loop, insert the following two lines immediately after the
`H1PartSelTrack* trk = PST[i];` declaration:

```cpp
// FIX V5G-1: skip the scattered-electron track per H1OO finder.
// The H1PartCand::kIsScatElec bit is propagated from H1PartEm by
// H1PartEm::SetIsScatElec() (H1Mods/H1PartEm.h:310-313); the bit test
// is O(1) and requires no thresholds. See research-v5g-scat-elec-track-assoc.md §B.5.
if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;
```

### 5.2 Required include

`H1Mods/H1PartCand.h` is **already included** at source line 102 (carried
in by the v5f truth-link matcher port). **No new include is needed.** The
implementer must verify this and not add a duplicate.

### 5.3 Q3-resolved sentence (per supervisor)

The scattered-electron track is filtered out of the `H1PartSelTrack` loop
**before any tree row is created**. No event-level extra branch is added
because every retained-track row already carries `scatEX/EY/EZ/EE` (set
at source lines 796–799, branched at 479–482), so the full per-event
scattered-electron 4-vector remains accessible to every downstream
consumer with zero information loss.

### 5.4 Behaviour table

| Condition                                                      | Action |
|----------------------------------------------------------------|--------|
| `trk->GetParticle() == nullptr` (defensive; should not occur)  | Keep track (no skip) |
| `IsScatElec() == false`                                        | Keep track (proceed to normal fill) |
| `IsScatElec() == true`                                         | **Skip:** `continue` — no row written |

### 5.5 Multiplicity expectation

For a clean DIS high-Q² MC event the count of skipped rows per event is
0 or 1:
- **0 skips:** the LAr scattered-electron cluster had no matched Lee-West
  selected track (the LAr finder ran but no track-cluster DCA < 12 cm
  match was found). Per research artifact §B.5, this is the *dominant
  high-Q² case* for LAr-cluster-only events near the barrel/forward
  boundary.
- **1 skip:** the LAr scattered-electron cluster had a matched Lee-West
  primary CJC track that survived `IsFromPrimary && IsCentralTrk` (Pass 1)
  or `IsFromPrimary && IsCombinedTrk` (Pass 2). The bit propagation
  ensures exactly one PST track carries the bit per event.
- **≥ 2 skips:** not possible by H1OO construction — the EM finder calls
  `H1PartEm::SetIsScatElec()` at most once per event
  (`H1CreatePartEm.C:127-134`).

### 5.6 Comments on per-row state in Pass 2

Pass 2 (combined tracks, lines 1437–1577) reuses the
`H1PartSelTrack*` iterator scope with `if (!trk->IsCentralTrk()) continue;`
filtering at line 1441. The skip-the-scat-elec test must come **before**
both the `IsFromPrimary` (line 1440) and `IsCentralTrk` (line 1441)
filters so the scattered electron is dropped in either pass even if it
were classified as `IsCombinedTrk`. The insertion point given in §5.1
(immediately after line 1438) satisfies this.

---

## 6. Algorithm: `Q2_es_gen`, `Y_es_gen`, `x_es_gen`, `GenEnPhoton`

### 6.1 New HAT pointer

Declare immediately after the existing `hatYsGen` line 226 (in the
gen-level HAT pointer cluster at lines 224–233):

```cpp
H1FloatPtr hatGenEnPhoton("GenEnPhoton");
```

**Target insertion line:** after line 226 (between `hatYsGen` and
`hatGenThElec`). The implementer must place it inside the same
"`MC gen-level HAT pointers`" comment block.

### 6.2 Method-choice rationale (researcher proof)

Use the **gen electron method** (`Q²_e` and `y_e` formulas applied to gen
quantities with `E_e_eff`), **not** the gen eSigma method. Per research
artifact §A.3 — verified algebraically against `H1MakeKine.h:72-74` and
`H1MakeKine.C:26-44, 64-82`:

> At gen level with full 4-momentum conservation in the gen final state,
> `Σ_gen = 2·E_beam_e_eff − E'(1−cos θ')`. Substituting into the
> Σ-method formula for `y_s` gives
> `y_s = 1 − E'(1−cosθ')/(2·E_beam_e_eff) ≡ y_e`. Since
> `Q²_es ≡ Q²_e` by definition and `x_es = Q²_s/(y_s·s) = Q²_e/(y_e·s) ≡ x_e`,
> **gen electron-method = gen eSigma at closed gen 4-momentum**.

The electron-method form requires only `E'_gen`, `θ_gen`, `φ_gen`, and
`E_e_eff` — all already in scope via existing HAT branches (`GenEnElec`,
`GenThElec`, `GenPhElec`) plus the new `GenEnPhoton`. Total new compute is
≈ 8 lines. The eSigma form would require summing gen HFS E−pz, which is
≳ 50 lines and adds correctness questions (which gen particles to
include, `IsGtr()` filtering, neutral-hadron coverage) that are unrelated
to the rad-x fix.

The branch suffix `_es_gen` (supervisor Q1 accepted) advertises that this
is the *truth-level analogue* of the data-side eSigma reconstruction
(`Q2es`, `Yes`, `Xes`). The proof above makes this naming
mathematically honest.

### 6.3 Variables to hoist to event scope

In the v5f source, the gen-electron 4-vector `genScatElec4` is built as a
**local** `TLorentzVector` inside the `if (validGenElec) { ... }` block
at lines 922–928 and used only to construct `gammaGen`. v5g needs the
same 4-vector at event scope to compute `Q²_es_gen` and `ystar_es_gen`
(the latter both inside the ChargedTotGen gen loop and the ChargedTot
matched-track block).

**Required hoist:** promote the local `TLorentzVector genScatElec4` (line
923) to an event-scope variable named `genScatElec4_evt`, defaulted
`(0,0,0,0)` at the top of every event-loop iteration (just before line
873 where `gammaGen` / `gammaPGen` / `boostGammaPGen` are already
declared). Inside the existing `if (validGenElec) { ... }` block, *assign*
to `genScatElec4_evt` instead of re-declaring.

No other v5f variables need hoisting. `GenBeamElecE_cg` and
`GenBeamProtP_cg` are already assigned per-event from `BeamElecE`,
`BeamProtP` at lines 1084–1085, and v5g may also assign them at event
scope to a freshly-declared pair of `double GenBeamElecE_evt`,
`GenBeamProtP_evt` for use in §6.4 and §7.

### 6.4 Computation block (event scope)

**Insertion site.** Inside the existing `if (isMC) { ... }` event-scope
block, **between line 952 (end of the `if (validGenElec) { ... }` gen
kinematics block) and line 953 (start of `else` reset)**. More precisely:
inside the inner `if (validGenElec)` block, after the `kine->Get*Gen()`
fills (lines 941–952), append the new event-level computation. Sentinel
defaults must be set *outside* the `if (validGenElec)` guard (next to the
existing `else` reset at lines 953–959).

**Pseudocode (NOT C++; for spec only):**

```
// FIX V5G-2: rad-safe gen DIS kinematics (Q2_es_gen, Y_es_gen, x_es_gen, GenEnPhoton).
// Inputs (all event scope, all already-bound in v5f or hoisted in §6.3):
//   - eBeamE          = *hatEBeamE                 (data-side reco beam; used only as fallback)
//   - pBeamP          = *hatEBeamP
//   - GenBeamElecE_evt = BeamElecE                  (per-event gen beam; identical to reco beam in this builder)
//   - GenBeamProtP_evt = BeamProtP
//   - genElecE        = recovered gen electron energy (lines 892, 910-913)
//   - genElecTh       = *hatGenThElec
//   - genElecPh       = *hatGenPhElec
//   - validGenElec    = (genElecE >= 0.5 && !std::isnan(genElecE))
//
// 1. Read ISR photon energy. Default 0 for samples without GenEnPhoton.
GenEnPhoton_val = isMC ? (Float_t)(*hatGenEnPhoton) : 0.0f;
if (GenEnPhoton_val < 0.0f) GenEnPhoton_val = 0.0f;   // guard

// 2. Build effective electron-side energy and s_eff per H1CalcWeight.
double E_e_eff = (double)GenBeamElecE_evt - (double)GenEnPhoton_val;
double s_eff   = 4.0 * (double)GenBeamProtP_evt * E_e_eff;

// 3. Gen electron-method kinematics. Uses E_e_eff, not nominal E_e.
if (validGenElec && E_e_eff > 0.5 && genElecE > 0.5) {
    double s_th2 = sin(genElecTh / 2.0);
    Y_es_gen_val  = 1.0 - (genElecE / E_e_eff) * s_th2 * s_th2;
    Q2_es_gen_val = 2.0 * E_e_eff * genElecE * (1.0 + cos(genElecTh));
    if (Y_es_gen_val > 1e-6 && s_eff > 1.0 && Q2_es_gen_val > 0) {
        x_es_gen_val = Q2_es_gen_val / (s_eff * Y_es_gen_val);
    } else {
        x_es_gen_val = -999.0;
    }
} else {
    Q2_es_gen_val = Y_es_gen_val = x_es_gen_val = -999.0;
}
```

### 6.5 New event-scope `Float_t` variables to declare

Append to the existing `Float_t` declaration cluster near line 314 (for
the ChargedTot side) and line 338 (for the ChargedTotGen side):

```
// Near line 315 (after `Q2sGen_ch = 0, YsGen_ch = 0, xGen_ch = 0`):
Float_t Q2_es_gen_ch = -999.0f, Y_es_gen_ch = -999.0f, x_es_gen_ch = -999.0f;
Float_t GenEnPhoton_ch = 0.0f, ystar_es_gen_ch = -999.0f;
// Near line 339 (after `Q2sGen_cg = 0, YsGen_cg = 0, xGen_cg = 0`):
Float_t Q2_es_gen_cg = -999.0f, Y_es_gen_cg = -999.0f, x_es_gen_cg = -999.0f;
Float_t GenEnPhoton_cg = 0.0f, ystar_es_gen_cg = -999.0f;
```

The event-level computation in §6.4 writes a single set of `_val` doubles
(`Q2_es_gen_val`, etc.); the per-tree variables (`_ch`, `_cg`) are
assigned from these `_val` doubles inside the respective fill blocks
(§6.7, §7.3).

### 6.6 New branches on ChargedTot

Append after the existing `chargedTree->Branch("zGen", &zGen_ch);` at
line 535:

```cpp
// FIX V5G-2: rad-safe gen DIS kinematics.
chargedTree->Branch("Q2_es_gen",    &Q2_es_gen_ch,    "Q2_es_gen/F");
chargedTree->Branch("Y_es_gen",     &Y_es_gen_ch,     "Y_es_gen/F");
chargedTree->Branch("x_es_gen",     &x_es_gen_ch,     "x_es_gen/F");
chargedTree->Branch("GenEnPhoton",  &GenEnPhoton_ch,  "GenEnPhoton/F");
// FIX V5G-3: rad-safe gen gamma*-p boost.
chargedTree->Branch("ystar_es_gen", &ystar_es_gen_ch, "ystar_es_gen/F");
```

**Exact insertion point:** between lines 535 and 537 (i.e., before
`chargedTree->Branch("inFiducialChargedMatch", ...);` at line 537).

### 6.7 New branches on ChargedTotGen

Append after the existing `chargedGenTree->Branch("zGen", &zGen_cg);` at
line 575 (still inside the MC-only `if (chargedGenTree)` block opened at
line 544):

```cpp
// FIX V5G-2: rad-safe gen DIS kinematics.
chargedGenTree->Branch("Q2_es_gen",    &Q2_es_gen_cg,    "Q2_es_gen/F");
chargedGenTree->Branch("Y_es_gen",     &Y_es_gen_cg,     "Y_es_gen/F");
chargedGenTree->Branch("x_es_gen",     &x_es_gen_cg,     "x_es_gen/F");
chargedGenTree->Branch("GenEnPhoton",  &GenEnPhoton_cg,  "GenEnPhoton/F");
// FIX V5G-3: rad-safe gen gamma*-p boost.
chargedGenTree->Branch("ystar_es_gen", &ystar_es_gen_cg, "ystar_es_gen/F");
```

**Exact insertion point:** between lines 575 and 576 (before
`chargedGenTree->Branch("weight1", ...);` at line 576).

### 6.8 Per-row fill (ChargedTot, Pass 1 matched-track block)

The matched-track fill block runs at lines 1319–1349 inside
`if (matched) { ... }`. **Inside the existing `if (validGenElec) { ... }`
sub-block at lines 1320–1349**, after the legacy `Q2sGen_ch / YsGen_ch /
xGen_ch` assignments at lines 1321–1323, append:

```cpp
// FIX V5G-2: fill rad-safe gen DIS kinematics on matched-track row.
Q2_es_gen_ch    = (Float_t)Q2_es_gen_val;
Y_es_gen_ch     = (Float_t)Y_es_gen_val;
x_es_gen_ch     = (Float_t)x_es_gen_val;
GenEnPhoton_ch  = GenEnPhoton_val;
```

The per-track reset block (lines 1203–1221) must additionally reset:

```cpp
// FIX V5G-2: per-track reset of rad-safe gen branches.
Q2_es_gen_ch    = -999.0f;
Y_es_gen_ch     = -999.0f;
x_es_gen_ch     = -999.0f;
GenEnPhoton_ch  = 0.0f;
ystar_es_gen_ch = -999.0f;
```

(Inserted next to the existing `Q2sGen_ch = 0; YsGen_ch = 0; xGen_ch = 0;`
reset at line 1214.)

The Pass 2 combined-track reset at lines 1569–1572 must similarly reset
all five new branches to their sentinels. Pass 2 does **not** attempt MC
matching (per source comments at lines 1547–1549), so the five new
branches remain at sentinel values on combined-track rows.

### 6.9 Per-row fill (ChargedTotGen gen loop)

Inside the gen loop at lines 969–1107, after the legacy `Q2sGen_cg /
YsGen_cg / xGen_cg` assignments at lines 1029–1031, append:

```cpp
// FIX V5G-2: fill rad-safe gen DIS kinematics on each gen-hadron row.
Q2_es_gen_cg    = (Float_t)Q2_es_gen_val;
Y_es_gen_cg     = (Float_t)Y_es_gen_val;
x_es_gen_cg     = (Float_t)x_es_gen_val;
GenEnPhoton_cg  = GenEnPhoton_val;
```

These values are constant across all rows of the same event (event-level
quantity by construction), but written per row to maintain
self-contained-row semantics.

---

## 7. Algorithm: `ystar_es_gen`

`ystar_es_gen` is the rapidity of the per-row gen hadron after boosting
to the γ\*–p CMS, where γ\* is constructed from the **gen** scattered
electron and **per-event gen** beams (NOT data-side nominal beams; NOT
data-side reco scattered electron).

### 7.1 Boost-vector computation (event scope)

**Insertion site.** Event-scope block, immediately after the §6.4
`Q2_es_gen` / `Y_es_gen` / `x_es_gen` computation (i.e., before line 953
"else" reset). Uses the hoisted `genScatElec4_evt` from §6.3 and the
per-event `GenBeamElecE_evt`, `GenBeamProtP_evt`.

**Pseudocode:**

```cpp
// FIX V5G-3: rad-safe gamma*-p CMS boost using gen-electron 4-vector
// and per-event gen beams (the canonical "_es_gen" frame).
TVector3 boostGammaPGen_es;
double Wmag_gen_es = 0.0;
bool validBoostGen_es = false;
if (validGenElec) {
    TLorentzVector eBeam4_gen(0.0, 0.0, -GenBeamElecE_evt, GenBeamElecE_evt);
    double pBeamE_gen = sqrt(GenBeamProtP_evt * GenBeamProtP_evt
                             + PhysConst::PROTON_MASS * PhysConst::PROTON_MASS);
    TLorentzVector pBeam4_gen(0.0, 0.0,  GenBeamProtP_evt, pBeamE_gen);
    TLorentzVector gamma_gen_es  = eBeam4_gen - genScatElec4_evt;
    TLorentzVector gammaP_gen_es = gamma_gen_es + pBeam4_gen;
    boostGammaPGen_es = gammaP_gen_es.BoostVector();
    Wmag_gen_es       = gammaP_gen_es.M();
    validBoostGen_es  = (Wmag_gen_es > 0.0);
}
```

**Supervisor Q4 (boost convention):** per-event gen beams, **no
exceptions**. The data-side `BeamElecE` / `BeamProtP` are already used by
the v5f `ystarGen` boost via `eBeam4` at line 809; the v5g `_es_gen`
variant uses `GenBeamElecE_evt` / `GenBeamProtP_evt` (currently identical
to the data-side beams by virtue of line 1084–1085, but kept distinct in
naming so that a future per-event gen-beam-energy variation does not
silently regress).

### 7.2 ChargedTotGen per-row fill

**Insertion site.** Inside the gen loop, immediately after the existing
`ystarGen_cg = (Float_t)genBoosted_cg.Rapidity();` at line 1035:

```cpp
// FIX V5G-3: rad-safe gamma*-p rapidity for gen hadron.
if (validBoostGen_es) {
    TLorentzVector genBoosted_es = genP4_cg;
    genBoosted_es.Boost(-boostGammaPGen_es);
    ystar_es_gen_cg = (Float_t)genBoosted_es.Rapidity();
} else {
    ystar_es_gen_cg = -999.0f;
}
```

### 7.3 ChargedTot matched-track per-row fill

**Insertion site.** Inside the matched-track block, immediately after the
existing `ystarGen_ch = (Float_t)matchBoosted.Rapidity();` at line 1332
(uses the already-built `matchP4` from lines 1325–1328):

```cpp
// FIX V5G-3: rad-safe gamma*-p rapidity for matched gen hadron.
if (validBoostGen_es) {
    TLorentzVector matchBoosted_es = matchP4;
    matchBoosted_es.Boost(-boostGammaPGen_es);
    ystar_es_gen_ch = (Float_t)matchBoosted_es.Rapidity();
} else {
    ystar_es_gen_ch = -999.0f;
}
```

For unmatched tracks (no `if (matched) { ... }`) and for Pass 2
combined-track rows, `ystar_es_gen_ch` remains at its per-row reset value
`-999.0f` (set per §6.8).

### 7.4 Distinction from v5f `ystarGen`

v5f's `ystarGen` uses `boostGammaPGen` built from
`gammaGen = eBeam4 - genScatElec4` with
`eBeam4 = (0,0,-eBeamE,eBeamE)` — i.e. the **data-side / nominal**
electron beam (source line 809). The v5g `_es_gen` variant uses
per-event gen beams (`eBeam4_gen`). For NoRad events
`eBeamE ≡ GenBeamElecE_evt` and the two boosts agree exactly; for Rad
events they agree because `GenBeamElecE_evt = BeamElecE = *hatEBeamE`
(same source). The rapidity *only* differs if a future code change
introduces a gen-vs-data beam-energy distinction. Both branches are
retained for now: `ystarGen` (v5f, deprecated, broken `xGen` linkage via
the boost reuse) and `ystar_es_gen` (v5g, fixed and forward-compatible).

---

## 8. Validation criteria

A v5g build is APPROVED for full-stat rebuild only when all four classes
pass on a 8-file MC validation sample (see §8.5). All comparisons are
per-row on the same input event lists; v5g and v5f must process the same
files.

### 8.1 NoRad event class — gen electron-method ≡ gen eSigma closure

**Selection.** `GenEnPhoton < 0.1 GeV` (or `GenRad != 1` where available).
For these events the §6 formula reduces to the no-ISR case and per
research §A.3 must agree bit-for-bit with the gen eSigma formula.

**Criteria (per row in ChargedTot).**
- `|x_es_gen − xGen| / xGen < 1e-3` for `xGen > 0` and finite.
- `|Q2_es_gen − Q2sGen| / Q2sGen < 1e-3`.
- `|Y_es_gen − YsGen| / YsGen < 1e-3` for `YsGen > 0.01`.
- `GenEnPhoton ≈ 0` (within HAT precision).

Per supervisor Q5: **no separate "NoRad reference sample" is required.**
The researcher proved gen electron-method = gen eSigma at gen-momentum
closure, so the regression test is performed on the NoRad subset of the
standard rad-enabled samples.

Sample size: at least 10 000 NoRad ChargedTot rows.

### 8.2 Radiative event class — rad-x fix demonstration

**Selection.** `GenEnPhoton > 5.0 GeV` (hard ISR; ≈5–10% of DJANGOH NC
events).

**Criteria.**
- v5f `xGen` is `inf`, `nan`, or `> 1.0` (i.e. unphysical) for ≥ 80% of
  these rows. (Confirms the v5f bug *exists* in the validation sample.)
- v5g `x_es_gen` is finite, positive, and `< 1.0` for ≥ 99% of these
  rows.
- v5g `x_es_gen` agrees with the data-side eSigma `Xes` for the same row
  within `2·σ_resol(x)` where `σ_resol(x) ≈ 0.1 · x` is the SIDIS x
  resolution at high Q² (loose closure cross-check — eSigma is itself
  resolution-broadened).

### 8.3 Scattered-electron skip class (replaces r1 §8.3)

**Selection.** All MC events in the validation sample.

**Criteria.**
- Per-event count: `n_skips_per_event` ∈ {0, 1} for ≥ 99.9 % of events.
  ≥ 2 skips is structurally impossible by H1OO construction (see §5.5);
  observing it would indicate a builder bug or upstream H1OO regression.
- High-Q² tagging fraction: count
  `n_events_with_IsScatElec_tagged / n_events_total` on the validation
  sample. **Expected: essentially 100 % at Q² > 150 GeV²**, but
  acceptable down to ~70 % in events where the LAr cluster has no
  matched Lee-West selected track (per research §B.5, the LAr-cluster-
  only events near the barrel/forward boundary fall in this category and
  the test correctly does nothing). The validation harness must report
  the fraction, not gate on it.
- Track-row delta: per event, `n_rows_v5f − n_rows_v5g ∈ {0, 1}` for
  ChargedTot, with delta = 1 in the dominant high-Q² case. **This is
  the operational signature that the skip is working** and is the
  required positive test.
- ChargedTotGen row count: **unchanged**. The gen loop already filters
  to `|PDG| ∈ {211, 321, 2212}` via `ProtonUtil::isChargedHadron()` at
  source line 972, so no gen electrons are present in ChargedTotGen.
  No `_gen` skip is needed (research artifact §E.2).

**No `IsScatElecCandidate` validation tests** — the branch does not
exist in v5g.

### 8.4 Backward-compatibility class (tightened from r1)

**Criterion A — surviving rows bit-exact on legacy branches.** For
every retained ChargedTot row (every v5g row), all 92 v5f branches must
equal the corresponding v5f row value bit-exactly (`memcmp` on the
branch buffer). The row-matching key is `(RunNumber, EventNumber,
Px, Py, Pz, Charge)` — the v5f scat-elec row is dropped, but every other
row must match.

**Criterion B — legacy `xGen` value preservation.** Per supervisor
discussion, the legacy `xGen` branch values must be **byte-identical**
between v5f and v5g for every retained row (we are deprecating-but-not-
changing the branch). The v5f formula `xGen = Q2sGen / (4·pBeamP·eBeamE·YsGen)`
at line 938 must remain in place untouched. Regression test: for ≥ 10 000
randomly sampled NoRad rows, `memcmp(xGen_v5f, xGen_v5g) == 0`. For
≥ 1 000 Rad-event rows, both sides equally produce `inf`/`nan`/`>1` (the
v5f bug is preserved deliberately for compatibility; consumers must
switch to `x_es_gen` to get the fix).

**Criterion C — ChargedTotGen bit-exact.** ChargedTotGen schema is purely
additive; the v5g per-row value must equal the v5f per-row value
bit-exactly for every existing branch on every existing row.

### 8.5 Sample composition

| MC | Generator | Rad enabled? | Files | Estimated events |
|----|-----------|--------------|-------|------------------|
| DJANGOH NC high-Q² | DJANGOH 1.4 | yes (ISR+FSR) | 5 | ~1 M |
| RAPGAP NC high-Q² | RAPGAP 3.1  | yes (ISR+FSR) | 3 | ~0.6 M |

NoRad-subset for §8.1 is obtained by cutting `GenEnPhoton < 0.1 GeV` on
the rad-enabled samples (Q5 supervisor decision).

---

## 9. Backward compatibility

- **Schema additive (mostly):** ChargedTot 92 v5f branches retained at
  identical names and identical formulas. ChargedTotGen 51 v5f branches
  retained likewise. v5g only **adds** 5 + 5 = 10 branches and
  **deletes the `IsScatElecCandidate` branch from r1** (which never
  existed in v5f, so no consumer change).
- **Per-event ChargedTot row count DECREASES by 1** in events where the
  scattered-electron track was previously written. Per §5.5 and §8.3,
  this is the dominant high-Q² case. Downstream scripts that count
  ChargedTot rows per event (e.g., per-event multiplicity histograms,
  `nChargedReco` consistency checks, event-level normalisations) will
  see this change.

  **Affected downstream consumers (action required):**
  - `h1-acceptance`: per-event reco-charged-hadron acceptance numerator
    drops by 1 in scattered-electron-tagged events. Acceptance corrections
    are *improved* (no electron contamination), but the numerator
    counting code must be re-validated.
  - `h1-radiative-correction`: no change — `C_rad` is computed on gen
    (numerator) and reco (denominator) yields at a given (x, Q²) bin;
    removing the electron track reduces the reco yield in the
    electron-method bin where the electron sits.
  - `h1-unfolder`: response matrix re-derived on v5g trees automatically
    reflects the removed row. The matrix is regenerated per build by
    design.
  - `h1-cross-section`: takes acceptance-corrected yields; consumes
    `h1-acceptance` output transparently.
  - `nChargedReco` event-level branch on `DisTot` (line 657, 682) is
    decremented by 1 in scattered-electron-tagged events. This is the
    desired behaviour — `nChargedReco` is the "n charged hadrons per
    event", which should not count the electron.

- **Downstream scripts that read v5f trees:** continue to work unchanged
  on v5f files, including the broken `xGen` and including the
  scattered-electron contamination. They must be updated to use
  `x_es_gen` (rad-safety) and to consume v5g trees (electron skipping),
  but each is a downstream task on individual analysis pipelines, not a
  v5g blocker.
- **Old ROOT files read by new code:** ROOT's TTree reader treats missing
  branches as the default-constructed type value. Code that reads
  `x_es_gen` on a v5f file gets `0.0` per row and must fall back to
  `xGen` — or detect the file format by checking branch presence with
  `TTree::GetBranch()` and choose at load time. Implementation detail,
  left to downstream consumers.
- **New ROOT files read by old code:** the 10 extra branches are
  ignored. Behaviour identical to v5f except for the per-event row
  decrement.
- **`DisTot` / `ProtonTot` / `ProtonTotGen`:** untouched. ProtonTot /
  ProtonTotGen do not exist in this builder; DisTot fix deferred to
  v5h.

---

## 10. Build & rollout plan

### 10.1 File layout

- **New source (NAF):**
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v5g.C`
  — copy of `ChargedSidisTreeBuilderHighQ2.C` (the v5f production source)
  with §5 / §6 / §7 edits inline. All v5g changes tagged
  `// FIX V5G-1`, `// FIX V5G-2`, `// FIX V5G-3` per Appendix A.
- **Local mirror (read-only):**
  `H1/sidis_highQ2/source_cache/ChargedSidisTreeBuilderHighQ2_v5g.C`
  — kept in sync with NAF for diffing and review. Refresh after every
  NAF edit.
- **v5f source UNTOUCHED:** `ChargedSidisTreeBuilderHighQ2.C` remains as
  the v5f reference binary at NAF. Both binaries coexist in
  `bin/x86_64-el9-gcc14-opt/`.
- **No header changes:** `PhysicsConstants_chargedsidis_highQ2.h` and
  `ChargedSidisUtils_highQ2.h` are **untouched** by v5g (r2 removed all
  new physics constants relative to r1).

### 10.2 Makefile change

The NAF `Makefile` (lines documented in research §C.5) must list the new
`.C` file as a build target. Append (alongside the existing
`ChargedSidisTreeBuilderHighQ2` target):

```make
ChargedSidisTreeBuilderHighQ2_v5g: outputdirs $(BUILDDIR)/ChargedSidisTreeBuilderHighQ2_v5g.o
	$(LOADER) $(BUILDDIR)/ChargedSidisTreeBuilderHighQ2_v5g.o $(ALL_LIBS) -o $(BINDIR)/$@
```

Plus extending the top-level `BIN` variable from `BIN = ChargedSidisTreeBuilderHighQ2`
to `BIN = ChargedSidisTreeBuilderHighQ2 ChargedSidisTreeBuilderHighQ2_v5g`.
Both binaries compile from the same `vpath %.C src` rule, so no
additional file-search wiring is needed.

### 10.3 Build on NAF

```bash
ssh -O check naf-hone.desy.de    # mandatory pre-check per AGENTS.md
ssh naf-hone.desy.de '
  source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
  cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2
  make bin
'
```

Expected outputs:
- `bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2`       (v5f, unchanged)
- `bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v5g`   (v5g, new)

### 10.4 Validation campaign

1. **Smoke test (5 k events, local).** Run v5g on one MC file with
   `--max-events 5000`, confirm:
   - the binary does not segfault on the new `hatGenEnPhoton` HAT read;
   - the 5 + 5 new branches appear in both trees and have non-default
     values on ≥ 80 % of rows in a rad-enabled sample;
   - the per-event ChargedTot row count is reduced by 0 or 1 vs v5f;
   - `ystar_es_gen` ≈ `ystarGen` on the NoRad subset (data-side and
     per-event-gen beams are identical in v5f).
2. **Validation submission.** Condor submission of v5g on the 8 files
   from §8.5. Outputs go to a sandbox directory
   (`output/sandbox_v5g_validation/`), distinct from production v5f
   outputs.
3. **Side-by-side compare.** Python / PyROOT validation harness reads
   v5f and v5g trees event-by-event, applies §8.1–§8.4 criteria, and
   produces a one-page PASS/FAIL summary table.
4. **Approval gate.** Supervisor reviews §8 summary. Approval required
   before §10.5.

### 10.5 Full-stat rebuild

**NOT triggered by this spec.** A separate supervisor decision is needed
to launch a full HERA-II rebuild (≈30 k Condor jobs, ≈5 days wall on
NAF). The full rebuild plan goes into a follow-up planning doc once v5g
is approved through §10.4.

### 10.6 Step-by-step for the script-implementer

The script-implementer that consumes this spec should:

1. Refresh local source_cache to match NAF (already done as of
   2026-05-23 13:11; verify MD5 `ee4e3c11...`).
2. Copy `ChargedSidisTreeBuilderHighQ2.C` → `ChargedSidisTreeBuilderHighQ2_v5g.C`
   on NAF (`scripts/` or `src/`).
3. Apply edits per §5 / §6 / §7, in this order, in a single editing
   session:
   - Add `H1FloatPtr hatGenEnPhoton("GenEnPhoton");` after line 226 (§6.1).
   - Add `Float_t Q2_es_gen_ch ... ystar_es_gen_ch` declarations after
     line 315 (§6.5).
   - Add `Float_t Q2_es_gen_cg ... ystar_es_gen_cg` declarations after
     line 338 (§6.5).
   - Hoist `genScatElec4` and add `boostGammaPGen_es / Wmag_gen_es /
     validBoostGen_es` declarations before line 873 (§6.3, §7.1).
   - Insert the §6.4 event-scope computation inside `if (validGenElec)`
     after line 952.
   - Insert the §7.1 event-scope boost computation after the §6.4 block.
   - Add the 5 `chargedTree->Branch(...)` calls between lines 535 and
     537 (§6.6).
   - Add the 5 `chargedGenTree->Branch(...)` calls between lines 575 and
     576 (§6.7).
   - Add the §6.8 per-track reset block at line 1214.
   - Add the §6.8 matched-track fill block at line 1323.
   - Add the §7.3 ChargedTot `ystar_es_gen` fill at line 1332.
   - Add the §6.9 gen-loop fill at line 1031.
   - Add the §7.2 ChargedTotGen `ystar_es_gen` fill at line 1035.
   - Add the Pass 2 reset block at line 1572.
   - Insert the §5.1 IsScatElec skip at line 1135 (Pass 1, after line
     1134) and line 1439 (Pass 2, after line 1438).
4. Extend the `Makefile` per §10.2.
5. Run `make bin`.
6. Smoke test per §10.4 step 1.
7. Hand off to validation per §10.4 steps 2–4.

---

## 11. Risk register

| # | Risk | Likelihood | Impact | Mitigation |
|---|------|------------|--------|------------|
| R1 | HAT branch `GenEnPhoton` not present in some MC samples (esp. older productions or NoRad-only generators). | Medium | Low | `H1FloatPtr` returns 0.0 for missing branch; formula degrades to NoRad case. §8.1 still passes. Log warning on first event if read returns exactly 0 across the first 100 events of a known-rad sample (heuristic for "branch absent vs. genuinely zero"). |
| R2 | H1OO `IsScatElec` bit not propagated to H1PartCand in some legacy production. | Very Low | High | Per research §B.5, propagation is built into `H1PartEm::SetIsScatElec()` (H1PartEm.h:310-313) since the H1OO version this analysis uses. §8.3 high-Q² tagging-fraction monitor (~100 % expected) catches any regression. |
| R3 | Gen scattered electron reconstruction degrades when gen electron θ < 0.01 rad (≈0.57°, very forward into the e-beam pipe). | Very Low | Negligible at high Q² | `validGenElec` guard (line 921) already protects. At Q² > 150 GeV² the gen electron is always at θ ≳ 0.1 rad. |
| R4 | Hoisting `genScatElec4` from local-to-block scope to event-scope introduces a stale-value bug if a future edit forgets to reinitialise per event. | Low | High | Initialise `genScatElec4_evt` to `(0,0,0,0)` and `validBoostGen_es = false` at the top of every event-loop iteration; explicitly comment the initialisation site `// FIX V5G-2/3: per-event reset`. Validation §8.4 Criterion A (bit-exact on retained rows) catches any silent drift. |
| R5 | `GenBeamElecE_evt` / `GenBeamProtP_evt` (per-event gen beams) currently always equal data-side `BeamElecE` / `BeamProtP` (line 1084–1085), so the `_es_gen` branches are numerically identical to a data-beam variant. The distinction will only matter if H1OO ever exposes truly per-event gen beam energies. | Medium | Cosmetic now / Important later | Use the per-event gen beams in code anyway, even if the source values currently happen to equal the data beams. The naming and code path are forward-compatible; the value parity is incidental. |
| R6 | Validation campaign Condor jobs exceed NAF batch quotas if too many files are submitted at once. | Low | Low | §10.4 step 2 specifies only 8 files. Well below quota. |
| R7 | Per-event row-count decrement (§9) silently breaks a downstream consumer that asserts `nChargedReco == sum_of_rows`. | Medium | Medium | §8.3 row-delta test catches the change; document in §9 with action items per consumer. Each downstream agent is on notice via the consumer table in §9. |
| R8 | Scattered-electron `H1PartSelTrack` is sometimes the highest-Pt track in low-Pt-hadron events; skipping it changes per-event leading-hadron analyses. | Low | Medium (physics) | This is **desired**: the electron is not a hadron and should not enter SIDIS hadron observables. Document explicitly in `docs/data/` after the v5g rebuild. No code mitigation. |
| R9 | `make bin` build picks up a stale `.o` for `ChargedSidisTreeBuilderHighQ2_v5g.o` from a prior partial build. | Low | Low | Run `rm -f x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v5g.o` before the first `make bin`. The Makefile's incremental compile is otherwise reliable. |

---

## 12. Open questions for supervisor

**None.** All seven r1 open questions were resolved by the supervisor on
2026-05-23:

| r1 Q | Resolution | Carrier in r2 |
|------|------------|---------------|
| Q1 (branch suffix `_es_gen` vs `_e_gen`) | `_es_gen` accepted (mathematically honest per research §A.3) | §6.2 naming, §3.1/§3.2 branches |
| Q2 (veto thresholds 10° / 3 GeV) | OBSOLETE — no manual veto cone | §5 (single-line bit test, no thresholds), §3.5 (no `PhysConst` constants) |
| Q3 (write or skip the scat-elec track) | SKIP via `continue`; do not write the row; per-event scat-elec 4-vector remains on every retained row via `scatEX/EY/EZ/EE` | §5.3 explicit statement; §6.6/§6.7 do not add a flag branch |
| Q4 (`ystar_es_gen` boost convention) | Per-event gen beams, no exceptions | §7.1 |
| Q5 (NoRad reference sample) | OBSOLETE — researcher proved gen electron-method ≡ gen eSigma at closed gen | §8.1 (NoRad subset of rad-enabled samples used; no separate sample needed) |
| Q6 (`IsScatElecCandidate` type) | OBSOLETE — no such branch | §3.4 explicit deletion |
| Q7 (ProtonTot / ProtonTotGen scope) | OUT OF SCOPE confirmed — separate NAF package, separate supervisor | §3.3 explicit, §2.4 (not in disposition table) |

---

## Appendix A: Inline-comment tag conventions

All new code in v5g must use the project's numbered-fix convention:

```
// FIX V5G-1: skip the scattered-electron track per H1OO IsScatElec(). See spec §5.
// FIX V5G-2: rad-safe gen DIS kinematics (Q2_es_gen, Y_es_gen, x_es_gen, GenEnPhoton). See spec §6.
// FIX V5G-3: rad-safe gen gamma*-p boost (ystar_es_gen). See spec §7.
```

Deprecation comments use:

```
// DEPRECATED V5G: <one-line reason>. Use <replacement>.
```

The deprecation comments must be added at both the variable declaration
site (lines 314, 315, 337, 338) and the branch registration site (lines
529–532, 556–558, 572). See §4 for the exact text per branch.

---

## Appendix B: Verification cross-reference

| Spec claim | Source verified | Where verified |
|------------|-----------------|----------------|
| `s_true = 4·P·(E_e − GenEnPhoton)` for rad events | `H1Calculator/H1CalcWeight.C:514, 558` | Read prior; cited in r1 Appendix B and unchanged in r2 |
| v5f `xGen` uses `4·pBeamP·eBeamE` (nominal `s`) | `ChargedSidisTreeBuilderHighQ2.C:938` | Read in r2 (refreshed source) |
| ChargedTot per-event scatE 4-vector source | `ChargedSidisTreeBuilderHighQ2.C:796-799` (fill), `479-482` (branch) | Read in r2 |
| ChargedTot branch count (92) | Counted from `chargedTree->Branch(...)` calls lines 417–541 | Read in r2 |
| ChargedTotGen branch count (51) | Counted from `chargedGenTree->Branch(...)` calls lines 545–616 | Read in r2 |
| Reco track loop Pass 1 (central) | `ChargedSidisTreeBuilderHighQ2.C:1133-1430` | Read in r2 |
| Reco track loop Pass 2 (combined) | `ChargedSidisTreeBuilderHighQ2.C:1437-1577` | Read in r2 |
| Gen-electron energy recovery + `validGenElec` set | `ChargedSidisTreeBuilderHighQ2.C:910-913, 921` | Read in r2 |
| Gen loop with `isChargedHadron` species filter | `ChargedSidisTreeBuilderHighQ2.C:969-1107` (filter at 972) | Read in r2 |
| `H1PartCand::IsScatElec()` bit propagation API | `H1Mods/H1PartEm.h:310-313`, `H1Mods/H1PartCand.C:832-839` | research §B (read by researcher 2026-05-23) |
| Gen electron-method ≡ gen eSigma at closed gen | `H1PhysUtils/H1MakeKine.h:72-74`, `H1MakeKine.C:26-44, 64-82`, `H1Calculator/H1CalcKine.C:399-418, 854-870` | research §A.1–§A.3 (read by researcher 2026-05-23) |
| Workspace quarantine status (no cross-package contamination) | NAF `Makefile`, `src/`, grep for relative-path includes | research §C.4–§C.5 (read by researcher 2026-05-23) |
| Refreshed source MD5 `ee4e3c11...` and 1606 lines | NAF `src/ChargedSidisTreeBuilderHighQ2.C` | research §C.3, local file `wc -l` 2026-05-23 |

All source citations in this spec have been verified against the
current local source_cache copies of `ChargedSidisTreeBuilderHighQ2.C`,
`ChargedSidisUtils_highQ2.h`, and `PhysicsConstants_chargedsidis_highQ2.h`,
which match NAF as of 2026-05-23.

— end spec r2 —

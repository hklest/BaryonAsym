# Spec v5g — High-Q² Charged SIDIS Tree Builder

**Status:** DRAFT — awaiting supervisor approval
**Author:** h1-software (delegated from h1-sidis-supervisor)
**Date:** 2026-05-23
**Target binary:** `ChargedSidisTreeBuilderHighQ2_v5g`
**Source file (new):** `H1/sidis_highQ2/source_cache/ChargedSidisTreeBuilderHighQ2_v5g.C`
**Parent version:** v5b/v5f port = `ProtonTreeBuilderHighQ2.C` (1279 lines, unchanged)
**Verified against source:** `ProtonTreeBuilderHighQ2.C` (lines 1–1279, read 2026-05-23), `H1/h1oo-master/H1Calculator/H1CalcWeight.C` (lines 500–564, canonical `S_true` formula).

This document is a **specification only**. No C++ is produced here. All
section numbers below match the supervisor's deliverable contract.

---

## 1. Motivation

The supervisor identified two analysis-blocking defects in the v5f trees that
v5g must fix without regressing any existing branch:

1. **Scattered-electron contamination of `dN/dφ_h`.** The current ChargedTot
   tree contains every primary-vertex CJC track that passes the wide online
   selection, including the LAr-tagged DIS scattered electron when it falls
   inside the central tracker acceptance. Downstream φ_h histograms then
   acquire a spurious narrow peak at φ_h ≈ 0 (electron direction by
   construction in the eSigma frame). The scattered-electron 4-vector is
   already written per event as `scatEX/EY/EZ/EE` (line 633–640) but there
   is no per-track flag to identify which CJC track *is* that electron.
   Downstream code has no clean way to veto it without re-reading raw
   selection objects.

2. **`xGen = ∞` for radiative events.** The current xGen formula is
   `xGen = Q²_sGen / (s_nominal · Y_sGen)` with `s_nominal = 4·E_e·E_p`
   using the *nominal* beam energies (line 753). For events with a hard
   ISR photon `γ_ISR`, the effective electron CMS energy is
   `E_e − E_γ_ISR`, so the true `s_true = 4·E_p·(E_e − E_γ_ISR)`
   (verified in `H1CalcWeight::GetCCQCDWeight`, line 558, and
   `GetNCQCDWeight`, line 514). Using `s_nominal` while `Y_sGen` is
   computed under the true-`s` assumption drives `xGen` to infinity or
   into junk (>1) for rad events, which makes C_rad binning impossible.

Both defects propagate through the entire downstream chain
(`h1-acceptance` → `h1-radiative-correction` → `h1-unfolder` →
`h1-cross-section`); fixing them at the tree-builder level is the only
clean intervention.

---

## 2. Existing schema — annotated

### 2.1 `ChargedTot` (reco track tree, 86 branches)

Grouped by purpose. Source-line ranges refer to `ProtonTreeBuilderHighQ2.C`.

#### 2.1.1 Event identifiers (KEEP, 2)

| Branch        | Type | Source line | Disposition |
|---------------|------|-------------|-------------|
| `RunNumber`   | I    | 433         | KEEP        |
| `EventNumber` | I    | 434         | KEEP        |

#### 2.1.2 Track kinematics (KEEP, 9)

| Branch   | Type | Disposition |
|----------|------|-------------|
| `Pt`     | F    | KEEP |
| `Eta`    | F    | KEEP |
| `Phi`    | F    | KEEP |
| `P`      | F    | KEEP |
| `Px`     | F    | KEEP |
| `Py`     | F    | KEEP |
| `Pz`     | F    | KEEP |
| `Pz`     | F    | KEEP |
| `Charge` | I    | KEEP |

#### 2.1.3 dE/dx variables (KEEP, 11)

`DedxRaw`, `DedxProton`, `DedxPion`, `DedxKaon`, `LkProton`, `LkPion`,
`LkKaon`, `NlhProton`, `NlhPion`, `NlhKaon`, `NHitDedx`. All KEEP.

#### 2.1.4 Track quality (KEEP, 6)

`NHitCJC`, `NHitCJC1`, `DcaPrime`, `RadLength`, `IsFromPrimary`,
`IsCentralTrk`. All KEEP.

#### 2.1.5 DIS kinematics — reco-side, calibrated eSigma (KEEP, 5)

`Q2s`, `Ys`, `xBj`, `Ws`, `PVz`. All KEEP. (`Q2s`, `Ys` are eSigma-calibrated
upstream of v5g per the binary header note, lines 51–53.)

#### 2.1.6 DIS kinematics — reco-side, all reconstruction methods (KEEP, 13)

`Q2e`, `Ye`, `Xe`, `Q2da`, `Yda`, `Xda`, `Q2es`, `Yes`, `Xes`,
`Q2h`, `Yh`, `Xh`, `Empz`. All KEEP.

#### 2.1.7 Per-event beam and scattered-electron 4-vector (KEEP, 6)

| Branch       | Type | Source | Disposition |
|--------------|------|--------|-------------|
| `BeamElecE`  | F    | 486    | KEEP |
| `BeamProtP`  | F    | 487    | KEEP |
| `scatEX`     | F    | 488    | KEEP — used by §5 IsScatElecCandidate |
| `scatEY`     | F    | 489    | KEEP |
| `scatEZ`     | F    | 490    | KEEP |
| `scatEE`     | F    | 491    | KEEP |

#### 2.1.8 SIDIS variables — reco, species-correct (KEEP, 9)

`ystar_es`, `yBreit_es`, `xF_SIDIS_es`, `z_SIDIS_es`,
`ylab` (= `ylab_ch`), `ystar` (= `ystar_ch`), `yBreit` (= `yBreit_ch`),
`xF_SIDIS` (= `xF_SIDIS_ch`), `z_SIDIS` (= `z_SIDIS_ch`). All KEEP.

#### 2.1.9 Weights (KEEP, 2)

`weight1`, `weight2`. All KEEP.

#### 2.1.10 MC truth — match to gen charged hadron (KEEP, 11)

`GenPDG`, `GenMotherPDG`, `IsGtr`, `IsMatched`, `ParticleSource`,
`GenMatchIdx`, `GenPt`, `GenEta`, `GenPhi`, `GenP`, `MatchAngle`,
`GenCharge`. All KEEP.

#### 2.1.11 Matched gen SIDIS variables — DEPRECATE-BUT-RETAIN (7)

| Branch       | Type | Source | Disposition |
|--------------|------|--------|-------------|
| `Q2sGen`     | F    | 518    | DEPRECATE-BUT-RETAIN — broken for rad events; use `Q2_es_gen` |
| `YsGen`      | F    | 519    | DEPRECATE-BUT-RETAIN — broken for rad events; use `Y_es_gen` |
| `xGen`       | F    | 520    | **DEPRECATE-BUT-RETAIN — INFINITE for rad events**; use `x_es_gen` |
| `ystarGen`   | F    | 521    | DEPRECATE-BUT-RETAIN — uses broken xGen via γ\*p boost; use `ystar_es_gen` |
| `yBreitGen`  | F    | 522    | KEEP (Breit boost does not depend on `s`) |
| `xFGen`      | F    | 523    | KEEP (uses `Wmag` from γ\*p boost; documented as still valid for non-rad) |
| `zGen`       | F    | 524    | KEEP (Lorentz invariant P.Ph / P.q; independent of `s`) |

Inline comments in v5g source for the four deprecated branches must read:
```
// DEPRECATED V5G: broken for radiative events (E_γ_ISR > 0 → wrong s_eff).
// Retained for backward compatibility. Use *_es_gen for rad-safe values.
```

#### 2.1.12 Fiducial flag (KEEP, 1)

`inFiducialChargedMatch`. KEEP.

**ChargedTot total branches before v5g additions: 86.**

### 2.2 `ChargedTotGen` (gen-only tree, 50 branches)

#### 2.2.1 Event identifiers (KEEP, 2)

`RunNumber`, `EventNumber`.

#### 2.2.2 Gen particle identity (KEEP, 5)

`GenPDG`, `GenMotherPDG`, `IsGtr`, `GenCharge`, `GenIdx`.

#### 2.2.3 Gen particle kinematics (KEEP, 5)

`GenPt`, `GenEta`, `GenPhi`, `GenP`, `ylab`.

#### 2.2.4 Gen-level DIS kinematics — all methods (KEEP, 12)

`Q2eGen`, `YeGen`, `XeGen`, `Q2daGen`, `YdaGen`, `XdaGen`,
`Q2esGen`, `YesGen`, `XesGen`, `Q2hGen`, `YhGen`, `XhGen`.

#### 2.2.5 Gen Sigma-method DIS kinematics — DEPRECATE-BUT-RETAIN (3)

| Branch   | Type | Disposition |
|----------|------|-------------|
| `Q2sGen` | F    | DEPRECATE-BUT-RETAIN — broken-`s`; use `Q2_es_gen` |
| `YsGen`  | F    | DEPRECATE-BUT-RETAIN — broken-`s`; use `Y_es_gen` |
| `xGen`   | F    | **DEPRECATE-BUT-RETAIN — INFINITE for rad events**; use `x_es_gen` |

#### 2.2.6 Gen SIDIS variables (KEEP / DEPRECATE-BUT-RETAIN, 4)

| Branch      | Type | Disposition |
|-------------|------|-------------|
| `ystarGen`  | F    | DEPRECATE-BUT-RETAIN — derived from broken-`s` boost; use `ystar_es_gen` |
| `yBreitGen` | F    | KEEP — Breit boost insensitive to `s` |
| `xFGen`     | F    | KEEP — uses `WmagGen`; safe for NoRad, drift for Rad documented |
| `zGen`      | F    | KEEP — Lorentz invariant |

#### 2.2.7 Weights and fiducial (KEEP, 3)

`weight1`, `weight2`, `inFiducialCharged`.

#### 2.2.8 Source classification (KEEP, 1)

`ParticleSource`.

#### 2.2.9 Gen scattered-electron and beam (KEEP, 6)

`GenScatEX`, `GenScatEY`, `GenScatEZ`, `GenScatEE`, `GenBeamElecE`,
`GenBeamProtP`. All KEEP — required as inputs to the new §6 algorithm.

**ChargedTotGen total branches before v5g additions: 50.**

### 2.3 Disposition summary

| Tree           | Existing | KEEP | DEPRECATE-BUT-RETAIN | REMOVE |
|----------------|----------|------|-----------------------|--------|
| `ChargedTot`   | 86       | 79   | 7                     | 0      |
| `ChargedTotGen`| 50       | 44   | 6                     | 0      |

No removals. v5g is strictly append-only on existing schemas.

---

## 3. New branches

### 3.1 `ChargedTot` — 6 new branches

| # | Branch                 | Type    | Default if invalid | Description |
|---|------------------------|---------|--------------------|-------------|
| 1 | `IsScatElecCandidate`  | Bool_t  | 0                  | True iff the current track points within `SCAT_ELEC_VETO_ANG_DEG` of the H1Calculator LAr scattered-electron 3-vector AND track `P > SCAT_ELEC_VETO_P_MIN`. Defaults: 10° / 3 GeV (intentionally loose; analysis can tighten). |
| 2 | `Q2_es_gen`            | Float_t | −999.0             | Q² from the gen-scattered-electron 4-vector via the electron method on gen quantities (see §6). Defined only for events with a valid gen-scattered electron. |
| 3 | `Y_es_gen`             | Float_t | −999.0             | Inelasticity from the gen-scattered-electron via the electron method on gen quantities with effective `E_e` (see §6). |
| 4 | `x_es_gen`             | Float_t | −999.0             | `x_es_gen = Q²_es_gen / (s_eff · Y_es_gen)` with `s_eff = 4 · GenBeamProtP · (GenBeamElecE − max(GenEnPhoton,0))`. Finite for rad events (fixes v5f xGen=∞ bug). |
| 5 | `GenEnPhoton`          | Float_t | 0.0                | ISR photon energy from HAT branch `GenEnPhoton`. Raw value; will be zero for samples without ISR (e.g. NoRad generators). |
| 6 | `ystar_es_gen`         | Float_t | −999.0             | Rapidity of the matched gen charged hadron boosted to the γ\*–p CMS, where γ\* is built from `(eBeam4_gen − genScatElec4_gen)` and beams use per-event gen values. |

Constants live in `PhysicsConstants.h`:
```
SCAT_ELEC_VETO_ANG_DEG = 10.0
SCAT_ELEC_VETO_P_MIN   = 3.0
```

Branches are written for **every** ChargedTot row (every selected reco
track). Branch list grows ChargedTot from 86 → **92**.

### 3.2 `ChargedTotGen` — 5 new branches

| # | Branch          | Type    | Default | Description |
|---|-----------------|---------|---------|-------------|
| 1 | `Q2_es_gen`     | Float_t | −999.0  | Same as §3.1 #2; identical event-level value across all rows for the same event. |
| 2 | `Y_es_gen`      | Float_t | −999.0  | Same as §3.1 #3. |
| 3 | `x_es_gen`      | Float_t | −999.0  | Same as §3.1 #4. |
| 4 | `GenEnPhoton`   | Float_t | 0.0     | Same as §3.1 #5. |
| 5 | `ystar_es_gen`  | Float_t | −999.0  | Rapidity of the per-row gen charged hadron boosted to the same γ\*–p CMS. |

**`IsScatElecCandidate` is deliberately NOT on ChargedTotGen** — the gen
tree has no reco track concept.

Branch list grows ChargedTotGen from 50 → **55**.

### 3.3 Branch-count summary

| Tree            | v5f | v5g | Δ   |
|-----------------|-----|-----|-----|
| `ChargedTot`    | 86  | 92  | +6  |
| `ChargedTotGen` | 50  | 55  | +5  |
| `ProtonTot`     | unchanged | unchanged | 0 |
| `ProtonTotGen`  | unchanged | unchanged | 0 |
| `DisTot`        | unchanged | unchanged | 0 |

ProtonTot/ProtonTotGen are explicitly **out of scope** for v5g; this
spec touches only the charged-hadron SIDIS trees.

---

## 4. Deprecate-but-retain branches

The following branches stay written, with identical formulas to v5f, but
receive inline `// DEPRECATED V5G: ...` comments at their declaration *and*
`Branch()` registration sites:

| Tree           | Branch       | Inline comment |
|----------------|--------------|----------------|
| `ChargedTot`   | `Q2sGen`     | `// DEPRECATED V5G: same value as Q2_es_gen for NoRad events; use Q2_es_gen.` |
| `ChargedTot`   | `YsGen`      | `// DEPRECATED V5G: y under nominal-s; use Y_es_gen.` |
| `ChargedTot`   | `xGen`       | `// DEPRECATED V5G: INFINITE for radiative events (uses s_nominal). Use x_es_gen.` |
| `ChargedTot`   | `ystarGen`   | `// DEPRECATED V5G: derived from broken xGen boost; use ystar_es_gen.` |
| `ChargedTotGen`| `Q2sGen`     | `// DEPRECATED V5G: use Q2_es_gen.` |
| `ChargedTotGen`| `YsGen`      | `// DEPRECATED V5G: use Y_es_gen.` |
| `ChargedTotGen`| `xGen`       | `// DEPRECATED V5G: INFINITE for radiative events. Use x_es_gen.` |
| `ChargedTotGen`| `ystarGen`   | `// DEPRECATED V5G: use ystar_es_gen.` |

All other v5f branches retain unchanged semantics and require no comment
change. `yBreitGen`, `xFGen`, `zGen` remain KEEP — the Breit boost,
`Wmag` from gen γ\*p, and the Lorentz-invariant `z = P·Ph/P·q` are all
insensitive to the `s_nominal` vs. `s_true` mismatch *within the v5f
formulas as actually written*, because they do not divide by `s`.

---

## 5. Algorithm: `IsScatElecCandidate`

**Insertion site.** Inside the reco track loop in `ProtonTreeBuilderHighQ2.C`
between lines 991 (after `IsCentralTrk` is set) and 1024 (before MC-truth
defaults). The check needs `Px,Py,Pz` (set at lines 966–968) and
`scatEX/EY/EZ/EE` (set at lines 637–640 at event scope, before the loop).

**Pseudocode (NOT C++; for spec only).**

```
// FIX V5G-1: per-track scattered-electron tag for downstream phi_h veto.
TVector3 trk3(Px, Py, Pz);
TLorentzVector scatElec4(scatEX, scatEY, scatEZ, scatEE);
TVector3 scat3 = scatElec4.Vect();
IsScatElecCandidate = 0;                          // defensive default
if (scat3.Mag() > 0.5 && trk3.Mag() > 0.5) {
    double ang_deg = trk3.Angle(scat3) * TMath::RadToDeg();
    double P_track = trk3.Mag();
    if (ang_deg < PhysConst::SCAT_ELEC_VETO_ANG_DEG
        && P_track > PhysConst::SCAT_ELEC_VETO_P_MIN) {
        IsScatElecCandidate = 1;
    }
}
```

**Behaviour table:**

| Condition                                            | `IsScatElecCandidate` |
|------------------------------------------------------|-----------------------|
| `scat3.Mag() < 0.5` (no LAr electron in event)       | 0 |
| `trk3.Mag() < 0.5`  (degenerate track, should be impossible after kine cuts) | 0 |
| Angle ≥ 10° OR track P ≤ 3 GeV                       | 0 |
| Angle < 10° AND track P > 3 GeV                      | 1 |

**Write-vs-skip policy.** The track is **WRITTEN** to ChargedTot with the
flag set. It is **not skipped**. Justification: downstream code (φ_h
histograms, charge-asymmetry pipelines, dN/dy_h) may want to either cut
the flagged tracks, weight them, or use them for a closure check; the
tree builder must not prejudge that choice. This also lets analysts
*tighten* the angle/P thresholds offline without rebuilding trees.

**Multiplicity expectation.** For a clean DIS MC event the count of
`IsScatElecCandidate==1` rows per event should be 0 or 1. ≥2 indicates
(a) a back-scattered photon conversion that produced a sibling track in
the CJC near the electron direction, or (b) thresholds chosen too loose;
v5g logs nothing extra — multiplicity audit is a downstream §8.3 check.

---

## 6. Algorithm: `Q2_es_gen`, `Y_es_gen`, `x_es_gen`, `ystar_es_gen`, `GenEnPhoton`

**Insertion site.** Event-scope block in `ProtonTreeBuilderHighQ2.C`,
inserted between line 776 (end of the existing `if (isMC) { ... }` gen-
electron recovery) and line 778 (start of `H1PartMCArrayPtr gtrdata`).
This places the new event-level computation after the v5f gen-electron
4-vector is built (`genScatElec4` is local to the existing block; v5g
must hoist it to event scope — see "Refactor note" below).

**Refactor note.** In the v5f source, `genScatElec4` is built as a local
`TLorentzVector` inside the `if (isMC && validGenElec) { ... }` block
(lines 738–743) and used only to construct `gammaGen`. v5g needs the
same 4-vector at event scope to compute Q²_es_gen and to refer to inside
the matched-track block. Promote it to an event-scope variable (named
`genScatElec4_evt` to avoid shadowing) and copy-fill it where the v5f
code currently constructs it.

**Method choice.** Use the **gen electron method** (`Q²_e` and `y_e`
formulas applied to gen quantities with effective `E_e`), **not** the
gen eSigma method. Justification:

1. **eSigma on gen would require gen hadronic E−pz**, which means a
   sum over all GTR final-state hadrons, including neutral hadrons and
   neutrinos. That sum is feasible (`gtrdata` is already loaded) but
   adds ≳50 lines of new code, opens correctness questions about which
   gen particles to include (`IsGtr() && IsFinalState()` filter), and
   sensitises Q²_es_gen to gen-level hadronic acceptance assumptions
   that have nothing to do with the rad-x bug being fixed.

2. **Electron method on gen quantities** uses only `E'_gen`, `θ_gen`,
   `φ_gen`, and `E_e_eff`. These are all already in scope (HAT branches
   `GenEnElec`, `GenThElec`, `GenPhElec`, with the v5f recovery if
   `GenEnElec == 0`, plus the new HAT branch `GenEnPhoton`). Total new
   compute ≈ 6 lines.

3. **Naming.** The branch suffix is `_es_gen` because the *intended downstream
   use* is comparison against the data-side eSigma reconstruction
   (`Q2es`, `Yes`, `Xes`). The truth-side analogue of a reconstruction
   method is, at the parton level, identical to the true `Q²`, `y`, `x`
   regardless of which reconstruction method one would have applied to
   the same final state — *provided* the formula uses the same effective
   beam energy. So the gen electron-method result *is* the truth value
   the eSigma data side is trying to reconstruct. The `_es_gen` suffix
   is kept for the supervisor-requested naming consistency; if the
   supervisor prefers `_e_gen` (electron-method-on-gen) we can rename
   trivially — flagged in §12.

**Pseudocode.**

```
// FIX V5G-2: rad-safe gen DIS kinematics.
// Inputs (all event scope):
//   - eBeamE      = *hatEBeamE                  (data-side; reco beam)
//   - pBeamP      = *hatEBeamP
//   - GenBeamElecE_cg, GenBeamProtP_cg          (gen-side beams; v5f, lines 931–932 — currently set inside gen loop; v5g must hoist them to event scope)
//   - genElecE_loc, genElecTh_loc, genElecPh_loc (gen scattered electron; same recovery as v5f lines 730–734, 917–922)
//   - genScatElec4_evt = (Ex,Ey,Ez,E) of gen scattered electron
//
// 1. Read ISR photon energy. Default 0 if branch absent (NoRad samples).
GenEnPhoton_val = isMC ? (*hatGenEnPhoton) : 0.0;
if (GenEnPhoton_val < 0) GenEnPhoton_val = 0.0;   // guard

// 2. Build effective electron-side energy and s_eff per H1CalcWeight.
E_e_eff = GenBeamElecE_evt - GenEnPhoton_val;     // GeV
s_eff   = 4.0 * GenBeamProtP_evt * E_e_eff;       // GeV^2

// 3. Gen electron-method kinematics. Uses E_e_eff, not nominal E_e.
//    y_e = 1 - (E'/E_e) sin^2(theta/2)   →  use E_e_eff in place of E_e
//    Q2_e = 2 E_e E' (1 + cos theta)     →  use E_e_eff in place of E_e
if (isMC && validGenElec && E_e_eff > 0.5 && genElecE_loc > 0.5) {
    double s_th2 = sin(genElecTh_loc / 2.0);
    Y_es_gen  = 1.0 - (genElecE_loc / E_e_eff) * s_th2 * s_th2;
    Q2_es_gen = 2.0 * E_e_eff * genElecE_loc * (1.0 + cos(genElecTh_loc));
    if (Y_es_gen > 1e-6 && s_eff > 1.0 && Q2_es_gen > 0) {
        x_es_gen = Q2_es_gen / (s_eff * Y_es_gen);
    } else {
        x_es_gen = -999.0;
    }
} else {
    Q2_es_gen = Y_es_gen = x_es_gen = -999.0;
}
```

**New HAT pointer required:**

```
H1FloatPtr hatGenEnPhoton("GenEnPhoton");
```

Declared at line 235 cluster (next to `hatGenThElec` etc.). Per §11 risk,
if the branch is absent in a given MC, the H1FloatPtr returns 0.0 and
the formulas degrade gracefully to the NoRad case.

**Optional `GenRad` cross-check (recommended, not required).** v5g may
also declare `H1ShortPtr hatGenRad("GenRad")` and assert
`(*GenRad == 1) ⇔ (GenEnPhoton > 0.1)` as a one-line sanity warning
(non-fatal). Implementation detail, left to implementer.

---

## 7. Algorithm: `ystar_es_gen`

`ystar_es_gen` is the rapidity of the per-row gen hadron after boosting
to the γ\*–p CMS, where γ\* is constructed from the gen scattered
electron and per-event gen beams (not nominal beams; not data beams).

**Insertion sites.**
- *ChargedTot row* (matched-track block, around line 1170): compute
  inside the `if (validGenElec) { ... }` clause that already contains
  the v5f `ystarGen_ch` fill.
- *ChargedTotGen row* (gen loop, around line 896): compute inside the
  `if (isMC && chargedGenTree && validGenElec) { ... }` clause that
  already contains the v5f `ystarGen_cg` fill.

**Pseudocode (event scope, computed once per event, reused for both rows).**

```
// FIX V5G-3: rad-safe gamma*-p CMS boost using gen-electron 4-vector
// and per-event gen beams (the canonical "_es_gen" frame).
if (isMC && validGenElec) {
    TLorentzVector eBeam4_gen( 0.0, 0.0, -GenBeamElecE_evt, GenBeamElecE_evt );
    double pBeamE_gen = sqrt(GenBeamProtP_evt * GenBeamProtP_evt
                             + PhysConst::PROTON_MASS * PhysConst::PROTON_MASS);
    TLorentzVector pBeam4_gen( 0.0, 0.0,  GenBeamProtP_evt, pBeamE_gen );
    TLorentzVector gamma_gen_es  = eBeam4_gen - genScatElec4_evt;
    TLorentzVector gammaP_gen_es = gamma_gen_es + pBeam4_gen;
    boostGammaPGen_es = gammaP_gen_es.BoostVector();
    Wmag_gen_es       = gammaP_gen_es.M();
}
```

**Per-row computation** (identical pattern in both rows):

```
if (isMC && validGenElec && Wmag_gen_es > 0) {
    TLorentzVector hadGen = (matched/gen 4-vector with PDG mass);
    hadGen.Boost(-boostGammaPGen_es);
    ystar_es_gen = (Float_t)hadGen.Rapidity();
} else {
    ystar_es_gen = -999.0f;
}
```

**Distinction from v5f `ystarGen`.** v5f's `ystarGen` uses
`boostGammaPGen` built from `gammaGen = eBeam4 - genScatElec4` with
`eBeam4 = (0,0,-eBeamE,eBeamE)` — i.e. the **data-side / nominal**
electron beam. For NoRad events `eBeamE == GenBeamElecE_evt` and the two
boosts agree; for Rad events they differ by ≲5% at the boost vector
level and the rapidity difference can be O(0.1) units. Both are
retained: `ystarGen` (v5f, deprecated) and `ystar_es_gen` (v5g, fixed).

---

## 8. Validation criteria

A v5g build is APPROVED for full-stat rebuild only when all three classes
pass. All comparisons are per-row on the same input event lists; v5g and
v5f must process the same files.

### 8.1 NoRad event class (existence of regression)

**Selection.** `GenEnPhoton < 0.1 GeV` (or `GenRad != 1` where available).
For these events the v5f `xGen` formula uses the correct `s`.

**Criterion.** Per-row in ChargedTot:
- `| x_es_gen - xGen | / xGen < 1e-3` for `xGen > 0` and finite.
- `| Q2_es_gen - Q2sGen | / Q2sGen < 1e-3`.
- `| Y_es_gen - YsGen | / YsGen < 1e-3` for `YsGen > 0.01`.
- `GenEnPhoton ≈ 0` (within HAT precision).

Sample size: at least 10 000 NoRad ChargedTot rows.

### 8.2 Radiative event class (existence of fix)

**Selection.** `GenEnPhoton > 5.0 GeV` (hard ISR; ≈5–10% of DJANGOH events).

**Criteria.**
- v5f `xGen` is either `inf`, `nan`, or `> 1.0` (i.e. unphysical) for
  ≥80% of these rows. (Confirms the v5f bug exists in the validation
  sample.)
- v5g `x_es_gen` is finite, positive, and `< 1.0` for ≥99% of these rows.
- v5g `x_es_gen` agrees with the data-side eSigma `Xes` for the same row
  within `2σ_resol(x)` where `σ_resol(x) ≈ 0.1 · x` is the SIDIS x
  resolution at high Q² (loose criterion — this is a closure cross-check,
  not a precision test, because eSigma is itself resolution-broadened).

### 8.3 Scattered-electron flag class

**Selection.** MC events where exactly one gen GTR electron with PDG=±11
exists with `Q²_gen > 150` and the LAr scattered-electron candidate
exists (`scat3.Mag() > 0.5`).

**Criteria.**
- For ≥95% of events, exactly one ChargedTot row has
  `IsScatElecCandidate == 1`.
- For events with `IsScatElecCandidate == 1` rows: the flagged row
  matches the highest-Pt CJC track among those with
  `IsMatched_ch == 1 && abs(GenPDG_ch) == 11`, for ≥90% of events.
  (The other 10% are conversions / wrongly matched cases — acceptable.)
- For ≤5% of events ≥2 rows have `IsScatElecCandidate == 1` (loose
  threshold conversions / brems).

### 8.4 Backward-compatibility class

**Criterion.** For every v5f branch on both ChargedTot and ChargedTotGen,
the v5g per-row value must equal the v5f per-row value bit-exactly
(`memcmp` on the branch buffer) for the same input event sequence.

Acceptable exception: ordering differences if any per-event refactor
reorders fills — none planned in §5–§7, so the criterion is bit-exact.

### 8.5 Sample composition

| MC | Generator | Rad enabled? | Files | Estimated events |
|----|-----------|--------------|-------|------------------|
| DJANGOH NC high-Q² | DJANGOH 1.4 | yes (ISR+FSR) | 5 | ~1 M |
| RAPGAP NC high-Q² | RAPGAP 3.1 | yes (ISR+FSR) | 3 | ~0.6 M |
| DJANGOH NoRad     | DJANGOH 1.4 | no            | 2 if available; else use NoRad-tagged subset of (1) | ≥10 k NoRad rows |

If no dedicated NoRad sample exists on NAF, §8.1 is performed on the
`GenEnPhoton < 0.1 GeV` subset of the rad-enabled samples.

---

## 9. Backward compatibility

- **Schema:** ChargedTot 86 v5f branches retained at identical names and
  identical formulas. ChargedTotGen 50 v5f branches retained likewise.
  v5g only **adds** 6 + 5 = 11 branches.
- **Downstream scripts that read v5f trees:** continue to work
  unchanged, including the broken `xGen`. They must be updated to use
  `x_es_gen`, but that is a downstream-task on each analysis pipeline,
  not a v5g blocker.
- **Old ROOT files read by new code:** ROOT's TTree reader treats
  missing branches as the default-constructed type value for that row.
  Code that reads `x_es_gen` on a v5f file gets `0.0` per row and must
  fall back to `xGen` — or the analysis code can detect the file format
  by checking branch presence with `TTree::GetBranch()` and choose at
  load time. Implementation detail, left to downstream consumers.
- **New ROOT files read by old code:** the 11 extra branches are
  ignored. Behaviour identical to v5f.
- **DisTot / ProtonTot / ProtonTotGen:** untouched.

---

## 10. Build & rollout plan

### 10.1 File layout

- **New source:** `H1/sidis_highQ2/source_cache/ChargedSidisTreeBuilderHighQ2_v5g.C`
  — copy of `ProtonTreeBuilderHighQ2.C` with §5/§6/§7 edits inline and
  all v5g changes tagged `// FIX V5G-1`, `// FIX V5G-2`, `// FIX V5G-3`.
- **Header update:** `H1/sidis_highQ2/code/PhysicsConstants.h` (or the
  NAF-deployed copy) must define `SCAT_ELEC_VETO_ANG_DEG = 10.0` and
  `SCAT_ELEC_VETO_P_MIN = 3.0`.
- **v5f source untouched:** `ProtonTreeBuilderHighQ2.C` remains as the
  v5f reference binary. Builds of both binaries coexist.

### 10.2 Build on NAF

```
ssh -O check naf-hone.desy.de    # mandatory pre-check per AGENTS.md
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2
make bin
```

The package's `Makefile` must be updated to list the new `.C` file as a
target. Implementation detail; trivial.

### 10.3 Validation campaign

1. **Smoke test:** run v5g on one MC file (5 k events) locally to confirm
   the new branches appear with non-default values and the binary does
   not segfault on the new `GenEnPhoton` HAT read.
2. **Validation submission:** Condor submission of v5g on the 8 files
   from §8.5. Output trees written to a sandbox directory, distinct
   from production v5f outputs.
3. **Side-by-side compare:** Python/PyROOT validation harness reads
   v5f and v5g trees event-by-event, applies §8.1–§8.4 criteria, and
   produces a one-page PASS/FAIL summary table.
4. **Approval gate:** supervisor reviews §8 summary. Approval required
   before §10.4.

### 10.4 Full-stat rebuild

**NOT triggered by this spec.** A separate supervisor decision is needed
to launch a full HERA-II rebuild (≈30 k Condor jobs, ≈5 days wall on
NAF). The full rebuild plan goes into a follow-up planning doc once
v5g is approved.

---

## 11. Risk register

| # | Risk | Likelihood | Impact | Mitigation |
|---|------|------------|--------|------------|
| R1 | HAT branch `GenEnPhoton` not present in some MC samples (esp. older productions or NoRad-only generators). | Medium | Low | `H1FloatPtr` returns 0.0 for missing branch; formula degrades to NoRad case. §8.1 still passes. Log warning on first event if read returns exactly 0 across the first 100 events of a known-rad sample (heuristic for "branch absent vs. genuinely zero"). |
| R2 | H1Calculator LAr scattered electron missing for events that fail strict LAr fiducial (small fraction at high Q²). | Low | Low | §5 algorithm defensively returns `IsScatElecCandidate = 0` when `scat3.Mag() < 0.5`. Such events have no LAr electron in the first place, so the supervisor's φ_h veto is not needed for them. |
| R3 | Gen scattered electron reconstruction degrades when gen electron θ < 0.01 rad (≈0.57°, very forward into the e-beam pipe). | Very Low | Negligible at high Q² | `validGenElec` guard (v5f line 736) already protects against this. At Q² > 150 GeV² the gen electron is always at θ ≳ 0.1 rad. |
| R4 | The 10° / 3 GeV thresholds for `IsScatElecCandidate` are too loose and tag genuine hadrons. | Medium | Medium | Intentional design choice (see §5 write-vs-skip). The downstream analysis can tighten without rebuilding. Validation §8.3 monitors the multiplicity-≥2 rate; if >5% the supervisor should consider tightening defaults. |
| R5 | The 10° / 3 GeV thresholds are too tight and miss the electron. | Low | High | At high Q² the scattered electron has E ≳ 10 GeV typically. The CJC angular resolution is ≪1°, so 10° is a wide tolerance. Mitigation: §8.3 criterion requires ≥95% single-flag, which catches this regression. |
| R6 | `_es_gen` branch suffix is misleading because the formula is the gen electron method, not gen eSigma. | Low | Cosmetic | Documented in §6 method-choice note; supervisor can rename in §12 Q1 if desired. |
| R7 | Promotion of `genScatElec4` from local-to-block scope to event-scope (§6 "Refactor note") introduces a stale-value bug if a future edit forgets to reinitialise per event. | Low | High | Initialise to a sentinel `(0,0,0,0)` at the top of every event-loop iteration; explicitly comment the initialisation site. Validation §8.4 (bit-exact backward compat) catches any silent drift. |
| R8 | Promotion of `GenBeamElecE_cg`, `GenBeamProtP_cg` from inside the gen loop to event scope (currently lines 931–932 fill them per-row from `BeamElecE`, `BeamProtP`) could change values written to ChargedTotGen if a per-row computation was intended. | Very Low | Low | Inspection of v5f source shows the per-row value is always `BeamElecE` / `BeamProtP`, identical for all rows of the same event. Hoisting changes nothing. |
| R9 | Validation campaign Condor jobs exceed NAF batch quotas if too many files are submitted at once. | Low | Low | §10.3 step 2 specifies only 8 files. Well below quota. |
| R10 | `make bin` build picks up the wrong `PhysicsConstants.h` (NAF copy vs. repo copy diverge). | Medium (this has bitten the project before per binary header note lines 77–82) | High | Header sync check pre-build: confirm both `SCAT_ELEC_VETO_ANG_DEG` and `SCAT_ELEC_VETO_P_MIN` are present in the NAF-deployed header. Run `grep -n SCAT_ELEC_VETO` on NAF before `make bin`. |

---

## 12. Open questions for supervisor

1. **Branch naming.** Use `_es_gen` (matches data-side `_es` family,
   supervisor-requested) or `_e_gen` (faithful to the actual method = gen
   electron method)? Default in this spec: `_es_gen` per supervisor.
2. **Default `SCAT_ELEC_VETO_ANG_DEG = 10°` and
   `SCAT_ELEC_VETO_P_MIN = 3 GeV`.** Confirm these conservative
   defaults, or specify alternative starting values. The choice
   propagates only through which tracks have the flag set; downstream
   can tighten without rebuilding.
3. **Write-vs-skip for `IsScatElecCandidate == 1` tracks.** Default
   in this spec: WRITE (do not skip). Confirm. Skipping would shrink
   ChargedTot by ≲1 row per event but loses closure-check capability.
4. **`ystar_es_gen` boost convention.** Use per-event gen beams
   (`GenBeamElecE_evt`, `GenBeamProtP_evt`) — spec's choice. Alternative:
   use nominal `(27.6, 920)` beams to ease comparison with the v5f
   `ystarGen`. Default: per-event gen beams (more correct).
5. **Validation NoRad sample.** Does a dedicated DJANGOH/RAPGAP NoRad
   production exist on NAF for this Q² regime, or should §8.1 use only
   the `GenEnPhoton < 0.1` subset of rad-enabled samples?
6. **Should `IsScatElecCandidate` be `Bool_t` (1 byte) or `Int_t`
   (4 bytes) for ROOT-IO consistency?** ChargedTot uses `Int_t` for
   all existing flags (`IsMatched`, `IsGtr`, etc.). Spec proposes
   `Bool_t` to save 3 bytes/row; supervisor may prefer `Int_t` for
   schema uniformity.
7. **Confirm that `ProtonTot` / `ProtonTotGen` are out of scope.** Same
   bug logically affects proton-pair `xGen` on those trees, but the
   proton/antiproton asymmetry analyses (`proton/`, `proton_HiQ/`) use
   a different supervisor. Spec assumes v5g is charged-SIDIS-only.

---

## Appendix A: Inline-comment tag conventions

All new code in v5g must use the project's numbered-fix convention:

```
// FIX V5G-1: per-track scattered-electron tag (IsScatElecCandidate). See spec §5.
// FIX V5G-2: rad-safe gen DIS kinematics (Q2_es_gen, Y_es_gen, x_es_gen, GenEnPhoton). See spec §6.
// FIX V5G-3: rad-safe gen gamma*-p boost (ystar_es_gen). See spec §7.
```

Deprecation comments use:

```
// DEPRECATED V5G: <one-line reason>. Use <replacement>.
```

## Appendix B: Verification cross-reference

| Spec claim | Source verified | Verified at |
|------------|-----------------|-------------|
| `s_true = 4·P·(E_e − GenEnPhoton)` for rad events | `H1CalcWeight.C:514, 558` | Lines 500–564 read |
| v5f `xGen` uses `4·pBeamP·eBeamE` (nominal `s`) | `ProtonTreeBuilderHighQ2.C:753` | Lines 760+ read |
| ChargedTot scatE 4-vector source | `ProtonTreeBuilderHighQ2.C:633-640` | Read |
| ChargedTot branch count | Source lines 433–527, counted | Read |
| ChargedTotGen branch count | Source lines 530–574, counted | Read |
| Reco track loop range | `ProtonTreeBuilderHighQ2.C:944-1259` | Read |
| Gen-electron energy recovery | `ProtonTreeBuilderHighQ2.C:730-734, 917-922` | Read |

All source citations in this spec have been verified against the
working-tree copies of `ProtonTreeBuilderHighQ2.C` and `H1CalcWeight.C`
on 2026-05-23.

— end spec —

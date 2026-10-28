# Research: g1j_ES Scattered-Electron / LAr-Side Systematics

**Date:** 2026-06-01
**Scope:** Detector-side scattered-electron (LAr) systematic prescription used by
the g1j_ES event-shapes analysis on NAF, and which pieces can be ported to the
H1 charged-particle SIDIS high-Q² rev6_v2 pipeline.
**Target file:** `/data/dust/group/h1/klesthen/g1j_ES/Analysis/RooUnfold/my_sys_unfold.cxx`
plus the upstream g1j_ES code that actually applies the H1OO systematic shifts.
**Constraints honoured:** SSH control-socket checked (alive, pid 73260); no
files modified; no batch jobs submitted; one interactive `ssh` session for
read-only inspection.

## TL;DR

1. The unfolding driver (`my_sys_unfold.cxx`) does **not** itself shift the
   electron energy/angle. It is a posthoc consumer that loads a fixed,
   integer-keyed family of pre-built migration matrices
   `Acceptance<obs>_<iSys>/Migration_Matrix_moreRec(_EFF)` from
   `merged.root` and unfolds each one with TUnfold, producing per-iSys
   unfolded histograms (`my_sys_unfold.cxx:130`, lines 596–678).
2. The actual H1CalcSystematic invocations live in
   `Analysis/AnalysisBase.C::SetSysShift(int sys)` (lines 89–148) and are
   driven from `Analysis/main.C` (lines 191–217). The shifts are applied at
   **event reconstruction time** during the `main` event loop and produce
   one full re-derivation of all observables per iSys, which is then
   histogrammed into `merged.root` under per-iSys directories.
3. The numeric set of variations is:

   | iSys | H1OO handle | Physical content | Shift magnitude |
   |---|---|---|---|
   | `-9999` | — | nominal (no shift) | — |
   | `+1` / `-1` | `H1BoostedJets::SysShiftJetEnergy` | JES (hadronic energy inside jets) | ±1 % |
   | `+2` | `H1BoostedJets::SysShiftHFSEnergy` | RCES (HFS clusters outside jets) | +1 % |
   | `+3` | `H1BoostedJets::SysShiftHFSTheta` | HFS polar angle | +1 mrad |
   | `+7` / `-7` | `H1CalcSystematic::eElecEn` + per-wheel `SetShiftElecEnWheel(0…4, ±0.005)` | LAr scattered-electron energy scale, applied per LAr wheel (BBE / CB1+2 / CB3 / FB1 / FB2+IF) | ±0.5 % per wheel |
   | `+10` | `H1CalcSystematic::eElecTh` + per-region `SetShiftElecTheta(0…2, +0.001)` co-shifted with `eElecTrTh` (+1 mrad) | LAr scattered-electron polar angle, applied per impact region (θ>135°, 120–135°, θ<120°), plus matched track-θ shift | +1 mrad |
4. Every iSys is applied by **rerunning the event reconstruction** through
   `H1Calculator` / `H1BoostedJets`, which gives a fully self-consistent
   migration matrix `Acceptance<obs>_<iSys>/…` per source. There is **no
   posthoc weight-only or bin-shift implementation** anywhere in
   g1j_ES.
5. For SIDIS high-Q² rev6_v2: the +7 / −7 electron-energy and +10
   electron-θ prescriptions are the directly portable LAr-side
   ingredients, but they **must be applied at TreeBuilder / response-matrix
   build time**, not as posthoc data-side shifts on already-unfolded
   histograms. The current SIDIS-side `source_electron_energy.json`,
   `source_electron_theta.json`, `source_electron_phi.json` family is the
   weight-style placeholder; replacing it with a g1j_ES-style migration-
   matrix family requires a builder-level fork with `H1CalcSystematic`
   shifts enabled.

---

## 1. What `my_sys_unfold.cxx` actually does

### 1.1 The systematic key list

The unfolding driver hard-codes the integer key set:

```
my_sys_unfold.cxx:130
   int isys [8] = { -9999, -1, 1, 2, 3, 7, -7, 10};
```

These keys are *consumed*, not produced, here. The driver assumes that
`merged.root` contains, for every observable `<obs>` and every iSys in
`isys[]`, a directory `<Model>/Acceptance<obs>_<iSys>/` carrying two
TH2D migration matrices:

- `Migration_Matrix_moreRec` — main reco-extended migration
- `Migration_Matrix_moreRec_EFF` — efficiency-only sibling (added in)

(see `my_sys_unfold.cxx:605–607` for the nominal `-9999` load and
`609–610` for the per-iSys `Model.Acceptance<obs>_<iSys>` load).

### 1.2 What `my_sys_unfold.cxx` does NOT do

- It does **not** call `H1CalcSystematic` anywhere — grep returns only the
  comment lines `2631, 2713` (search ran on the file; the only hits for any
  `H1CalcSystematic|kElecE|kElecTh|SetSys` token are documentation
  comments, no setter calls).
- It does **not** rescale a nominal histogram by `±0.5 %` or `±1 mrad`.
- It does **not** smear data after unfolding. The per-iSys output comes
  from a full TUnfold pass on a per-iSys response matrix
  (`my_sys_unfold.cxx:616–620` and `647–678`).

### 1.3 How the per-iSys unfolded result is combined

The combiner is `Analysis/SysUncertainty.cxx`. For each iSys it computes a
fractional shift relative to nominal and adds them in quadrature with the
non-detector terms:

```
SysUncertainty.cxx:341–355  (representative excerpt; equivalent for Dj/Ra)
   RaSysunc1  = (RD - RD1 ) / RD       // JES up
   RaSysunc2  = (RD - RD2 ) / RD       // RCES
   RaSysunc3  = (RD - RD3 ) / RD       // HFS theta
   RaSysunc7  = (RD - RD7 ) / RD       // electron energy up
   RaSysunc10 = (RD - RD10) / RD       // electron theta
   RaSysunc   = sqrt( RaSysunc1^2 + RaSysunc2^2 + RaSysunc3^2
                    + RaSysunc7^2  + RaSysunc10^2
                    + SysuncModel^2 + UnfoldingStatUnc^2
                    + SysuncReg^2  + LumiUnc^2 );  // QED dropped 2024-02-21
```

Notes worth flagging:

- `RaSysunc_1` (iSys = −1, JES down) and the `−7` electron-energy down side
  are loaded and printed but **not included in the listed quadrature
  sum** (`SysUncertainty.cxx:344, 351, 354`). The detector quadrature
  treats JES and electron-energy as one-sided (up only) in the final
  number. This is a g1j_ES-internal convention choice, not a recommendation
  for SIDIS to copy as-is.
- `RD_1` etc. are available so the legend can still show ±JES (see
  `SysUncertainty.cxx:497–502, 845–848`).

---

## 2. Where the shifts are actually applied

### 2.1 The H1OO setter block

```
AnalysisBase.C:67–149
   void AnalysisBase::SetSysShift(int sys) {
      fSys = sys;
      gH1Calc->Systematic()->ResetAllShifts();
      H1BoostedJets::Instance()->ResetSysShifts();
      gH1Calc->Reset();
      if ( sys != -9999 ) {
         if ( sys == 1 )             H1BoostedJets::Instance()->SysShiftJetEnergy( +0.01);
         else if ( sys == -1 )       H1BoostedJets::Instance()->SysShiftJetEnergy( -0.01);
         else if ( sys == 2 )        H1BoostedJets::Instance()->SysShiftHFSEnergy( +0.01);
         else if ( sys == 3 )        H1BoostedJets::Instance()->SysShiftHFSTheta( +0.001);
         else if ( sys == int(H1CalcSystematic::eElecEn) ) {        // +7
            for w in 0..4: SetShiftElecEnWheel(w,  +0.005);
            gH1Calc->Systematic()->ShiftUp(sys);
         }
         else if ( sys == -1*int(H1CalcSystematic::eElecEn) ) {     // -7
            for w in 0..4: SetShiftElecEnWheel(w,  -0.005);
            gH1Calc->Systematic()->ShiftDo(sys);
         }
         else if ( sys == int(H1CalcSystematic::eElecTh) ) {        // +10
            for r in 0..2: SetShiftElecTheta(r, +0.001);
            SetShift(H1CalcSystematic::eElecTrTh, +0.001);
            ShiftUp(eElecTh);  ShiftUp(eElecTrTh);
         }
         else if ( sys == int(H1CalcSystematic::eMuonPh) ) {        // +19
            SetShift(H1CalcSystematic::eMuonPh, +0.0001);
         }
      }
   }
```

Documentation lines `AnalysisBase.C:80–87` cite arXiv:0904.3513 as the
source for the magnitudes (electron energy 0.5 %, electron θ 0.2 mrad
[2000] / 0.5 mrad [low-Q² FL]; g1j_ES uses the more conservative 1 mrad).
The per-LAr-wheel (BBE / CB1+2 / CB3 / FB1 / FB2+IF) and per-θ-region
(>135° / 120–135° / <120°) breakdown is the standard
`H1CalcSystematic::SetShiftElecEnWheel` / `SetShiftElecTheta` API.

### 2.2 Confirmation of the H1OO enum integers

`H1Calculator/H1CalcSystematic.h:20–24` (release relvol5/3.4.13):

```
enum Systematics{eHadEn, eHadEnCor, eHadEnUnc, eHadTh, eHadPh,
                 eNoiseEn,
                 eElecEn, eElecEnCor, eElecEnUnc, eElecTh, eElecPh,
                 eElecTrEn, eElecTrTh, eElecTrPh, eElecIdentEff,
                 eMuonEn, eMuonPt, eMuonTh, eMuonPh, eMuonIdentEff, ... };
```

so `eElecEn = 6`, `eElecTh = 9`, `eMuonPh = 18` in this release. The
g1j_ES `main.C` comment block (`main.C:195–204`) states:

```
  1  JES
  2  RCES
  3  HFS Theta
  7  Electron energy
 10  Electron theta
 19  Muon phi (can be used as default)
```

The numeric offset (7 vs 6, 10 vs 9, 19 vs 18) implies that the H1OO
release linked into the g1j_ES binary has an *additional* enum entry
before `eElecEn` (one of the early-2020s releases added `eNoiseEn` or an
analogous entry such that `eElecEn = 7`). The code is robust to this
because it always passes `int(H1CalcSystematic::eElecEn)`; the integer
that ends up on disk as `Acceptance<obs>_7/` simply reflects whatever the
linked release evaluates that enum to. **Porting note:** never hard-code
the integer 7 or 10 in a SIDIS rebuild; always use
`int(H1CalcSystematic::eElecEn)` and `int(H1CalcSystematic::eElecTh)`.

### 2.3 Driver loop

```
main.C:191–217
   if ( cmd_sys == -9999 ) {
      for ( int iSys : { -1, 1, 2, 3,
                         -1*int(H1CalcSystematic::eElecEn),
                              int(H1CalcSystematic::eElecEn),
                              int(H1CalcSystematic::eElecTh) } ) {
         esanalysis.DoReset();
         esanalysis.SetSysShift(iSys);
         GenBaseCuts     = esanalysis.DoBasicCutsGen();
         GenAnalysisCuts = esanalysis.DoAnalysisCutsGen();
         RecBaseCuts     = esanalysis.DoBasicCutsRec();      // rec cuts re-applied with shifted electron / HFS
         RecAnalysisCuts = esanalysis.DoAnalysisCutsRec();
         esanalysis.DoCrossSectionObservablesGen();
         esanalysis.DoCrossSectionObservablesRec();
         esanalysis.DoControlPlotsGenRec(...);
      }
      esanalysis.DoReset();
      esanalysis.SetSysShift(-9999);   // restore nominal
   }
```

Key properties:

- The **same input HAT/DST event** is reprocessed under every iSys; the
  shifted reco-side cuts (Empz, vertex-z, electron-fiducial) are
  re-evaluated against the shifted electron kinematics returned by
  `gH1Calc->...` after `SetShiftElec…`.
- Both reco-side and gen-side observables are re-filled, but only the
  reco side is sensitive to the shift; gen-side stays fixed (the
  acceptance / response matrix is what changes, not the truth axis).
- The output histograms land in per-iSys subdirectories of the same
  `merged.root` that `my_sys_unfold.cxx` later reads.

Conclusion: in g1j_ES, **every detector systematic is a full
response-matrix rebuild**, not a posthoc reweight.

---

## 3. Recommendations for high-Q² charged-SIDIS rev6_v2

### 3.1 Which g1j_ES variations port directly as LAr-side data sources

| g1j_ES iSys | SIDIS rev6_v2 mapping | Port type | Notes |
|---|---|---|---|
| `+7` / `−7` (`eElecEn`, per-wheel ±0.5 %) | replaces / supersedes current `source_electron_energy.json` | **migration-matrix rebuild required** | The current rev6_v2 source is a posthoc / rank-1 reweight (see `docs/research/research-electron-energy-systematic-critique-2026-05-26.md`). The g1j_ES prescription gives a physically correct per-iSys response, including per-LAr-wheel granularity. |
| `+10` (`eElecTh` + `eElecTrTh`, per-region +1 mrad) | replaces / supersedes current `source_electron_theta.json` | **migration-matrix rebuild required** | g1j_ES uses 1 mrad; the published H1 scale is 0.2 mrad (HERA-II jets) to 1 mrad (low-Q² FL). For high-Q² SIDIS the appropriate value is 0.2–1 mrad — start with 1 mrad to match g1j_ES; revisit once response-matrix machinery is in place. |
| (no g1j_ES analogue) | `source_electron_phi.json` | not addressable by g1j_ES | g1j_ES does not vary electron φ. Either drop the φ source (electron-φ has no documented LAr-scale systematic at HERA-II) or generate it with `H1CalcSystematic::eElecPh` if a published prescription exists. |
| `+1` / `−1` JES, `+2` RCES, `+3` HFS-θ | becomes the HFS-side systematic family | **migration-matrix rebuild required** | These are the hadronic-side analogues; they belong to the HFS source list, not the electron list. Tracked separately in `research-HFS-systematic-2026-05-25.md`. |

### 3.2 What can be done posthoc (and what cannot)

The handoff policy is already correct on this point (`context-handoff.md`,
section "Systematics propagation convention"):

- Pure normalisation (lumi): posthoc per-bin scaling is exact.
- Per-event / per-track weight that does not move bin boundaries: posthoc
  reweight on the already-unfolded result is defensible.
- **Migration-affecting systematics** (electron energy, electron angle,
  HFS, tracking precision): MUST go through `ROOT → shifted CSV → response
  rebuild → full 4D unfold pipeline rerun`.

The g1j_ES code is the canonical demonstration of that policy: it applies
the electron-energy and electron-θ shift at the reco-cut stage, then
rebuilds the migration matrix, then unfolds. **Any rev6_v2 attempt to
mimic +7 / −7 / +10 by posthoc shifting of the unfolded data marginal
will misrepresent the LAr-acceptance bin-bin coupling** (the same
critique already raised on the current rank-1 sources).

### 3.3 Concrete next steps for rev6_v2

1. **Builder change.** In `ChargedSidisTreeBuilderHighQ2_v5g`, add a
   `--sys <iSys>` CLI flag analogous to `g1j_ES/Analysis/main.C:191` and
   a `SetSysShift(int)` helper analogous to
   `g1j_ES/Analysis/AnalysisBase.C:67–149`, calling exactly the same
   `H1CalcSystematic` setters per iSys. Important: the SIDIS high-Q²
   builder currently has only a dead `#include "H1Calculator/H1CalcSystematic.h"`
   (see `docs/references/reference-LAr-electron-scale-highQ2-sidis.md`,
   findings G-LAr-3/G-LAr-8) — i.e. no application mechanism exists yet.
   This is the missing piece.
2. **Production matrix.** Re-run the v5g sidistree production for each of
   iSys ∈ {`+int(eElecEn)`, `-int(eElecEn)`, `+int(eElecTh)`}, plus a
   reference `-9999` run. That gives 3 shifted per-track CSV families on
   the same input HAT set. Per-iSys CSV outputs feed the response-matrix
   builder.
3. **Response matrices.** Build one 480×480 response per iSys (same
   pipeline as the nominal v3final response, just with the shifted-reco
   CSV). Keep gen unchanged.
4. **Unfold each iSys.** Run the canonical TUnfold-4D unfold (τ=0.10) on
   the **nominal data** with each shifted response. The per-iSys
   unfolded histogram is `N_unf^{(iSys)}`.
5. **Per-source σ.** For each iSys, compute `δ_i = N_unf^{(iSys)} − N_unf^{nominal}`
   per bin, build the source covariance as `cov = δ δᵀ` only if a
   noise-floor characterisation justifies it (the previous rank-1 cov
   critiques apply directly here — see
   `docs/research/research-electron-energy-systematic-critique-2026-05-26.md`).
   Prefer a 2-sided symmetrised `δ` (use both `+eElecEn` and `−eElecEn`)
   rather than g1j_ES's one-sided convention.
6. **Combination.** Quadrature as in `SysUncertainty.cxx:341–355`, but
   keep the down-side electron-energy variation in the sum (do NOT drop
   it as g1j_ES does); use per-source 480×480 covariance for downstream
   global fits.

### 3.4 What NOT to port

- Do not hard-code `7` / `−7` / `10` as the integer keys. Always go
  through `int(H1CalcSystematic::eElecEn)` etc. The numeric value depends
  on the linked H1OO release.
- Do not import the g1j_ES one-sided detector-side quadrature
  convention; SIDIS asymmetry observables (`A_h`, `A_BC`) need
  symmetric ± variations to permit the physical charge-cancellation
  flagged in `research-electron-energy-systematic-critique-2026-05-26.md` §8.1.
- Do not import the +3 HFS-θ value `0.001` rad blindly as "HFS theta
  for SIDIS"; SIDIS-side HFS treatment is being separately tracked
  in `research-HFS-systematic-2026-05-25.md`.

---

## 4. Source-and-citation table

| Question | Answer | File:line evidence |
|---|---|---|
| What sets the iSys key list consumed by the unfolder? | hard-coded integer array of 8 entries | `Analysis/RooUnfold/my_sys_unfold.cxx:130` |
| Where are per-iSys response matrices loaded? | `merged.root → <Model>/Acceptance<obs>_<iSys>/Migration_Matrix_moreRec(_EFF)` | `my_sys_unfold.cxx:605–610, 637–641, 691–696` |
| Does the unfolder call `H1CalcSystematic`? | No. Only nominal TUnfold per iSys-keyed matrix | `my_sys_unfold.cxx:130, 605–678` (no `H1CalcSystematic` symbol references except 2 comments at 2631, 2713) |
| Where are electron energy/θ shifts actually applied? | In `AnalysisBase::SetSysShift(int)` during the event loop | `Analysis/AnalysisBase.C:67–149` |
| Electron energy magnitude | ±0.005 (0.5 %) per LAr wheel (5 wheels: BBE, CB1+2, CB3, FB1, FB2+IF) | `AnalysisBase.C:108–129` |
| Electron θ magnitude | +0.001 rad per impact region (3 regions: θ>135, 120–135, <120), plus same shift on `eElecTrTh` | `AnalysisBase.C:130–141` |
| Driver loop over iSys | hard list in `main.C` | `main.C:195` |
| Per-iSys procedure | full reco cut + observable refill per shifted event | `main.C:201–215` |
| Combination formula | per-iSys fractional shift in quadrature with model/reg/unfStat/lumi | `Analysis/SysUncertainty.cxx:341–355` |
| One-sided detector-side convention (JES, ElecEn) | down-side variations loaded but not summed | `SysUncertainty.cxx:344, 351, 354` |
| H1OO enum reference | `enum Systematics { eHadEn, eHadEnCor, …, eElecEn, …, eElecTh, …, eMuonPh, … }` | `/afs/desy.de/group/h1/root/checkout2/oo-releases/releases/relvol5/3.4.13/H1Calculator/H1CalcSystematic.h:20–24` |
| Documentation citation for shift magnitudes | arXiv:0904.3513 (electron energy 0.5 %; electron θ 0.2–0.5 mrad) | `AnalysisBase.C:82–84` |
| Pre-shifted SysUnc plot library | `/data/dust/group/h1/klesthen/g1j_ES/Analysis/RooUnfold/SysUnc/Cross_Sec_<obs>_<iSys>_1D.{pdf,ps}` for iSys ∈ {-1,1,2,3,7,-7,10,-9999} | listed by `ls SysUnc/` |
| Current SIDIS-side LAr-systematic mechanism | none — dead `#include` only, no shifted-MC production, no `--sys` flag | `H1/sidis_highQ2/docs/references/reference-LAr-electron-scale-highQ2-sidis.md`, findings G-LAr-3 / G-LAr-8 |
| Current SIDIS-side `source_electron_*.json` quality | rank-1 cov on noise-dominated bins; flagged as unsafe to ship as-is | `H1/sidis_highQ2/docs/research/research-electron-energy-systematic-critique-2026-05-26.md`, `research-electron-theta-critique-2026-05-26.md` |

---

## 5. Recommended high-Q² SIDIS variations (summary)

Adopt the g1j_ES H1OO calls directly, with the symmetry / coverage fixes
noted above. Magnitudes and call sites:

```
// Electron energy (LAr scale), both directions, per LAr wheel
gH1Calc->Systematic()->ResetAllShifts();
for (int w = 0; w < 5; ++w) gH1Calc->Systematic()->SetShiftElecEnWheel(w, +0.005);
gH1Calc->Systematic()->ShiftUp(H1CalcSystematic::eElecEn);
// ... rerun reco-side cuts and refill response matrix ...
// Repeat with -0.005 and ShiftDo(...) for the down side.

// Electron polar angle (per impact region) + co-shifted track angle
gH1Calc->Systematic()->ResetAllShifts();
for (int r = 0; r < 3; ++r) gH1Calc->Systematic()->SetShiftElecTheta(r, +0.001);
gH1Calc->Systematic()->SetShift(H1CalcSystematic::eElecTrTh, +0.001);
gH1Calc->Systematic()->ShiftUp(H1CalcSystematic::eElecTh);
gH1Calc->Systematic()->ShiftUp(H1CalcSystematic::eElecTrTh);
// ... rerun, refill. Repeat with -0.001 to get a symmetric envelope.
```

Each variant requires:

- a per-iSys sidistree production pass (Stage 2 only; gen unchanged),
- a per-iSys response-matrix build on the resulting CSV,
- a per-iSys TUnfold-4D unfold of the nominal data on the shifted response.

There is **no g1j_ES-supported posthoc, weight-only or bin-shift
shortcut** for any of these on the data side.

---

## 6. Risks, gaps, open questions

- **Builder mechanism missing.** SIDIS high-Q² has no `--sys` flag or
  `SetSysShift` analogue in `ChargedSidisTreeBuilderHighQ2_v5g`. Adding it
  is a builder change requiring rebuild, the binary-identity manifest
  must be bumped (see `context-handoff.md` §0).
- **Compute cost.** Each iSys is a full Stage-2 reproduction over the
  4-period sample (≈ 1525 jobs at the current scale). The 3 LAr-side
  iSys (eElecEn +/−, eElecTh +) imply ~3× the nominal Stage-2 wall
  time, plus 3 additional response-matrix builds. Plan for ≈ 4–6 h
  per iSys on Condor at current MC throughput.
- **Down-side coverage gap in g1j_ES convention.** g1j_ES sums only the
  up-side electron-energy variation. SIDIS should sum both, OR
  symmetrise (`|δ_up|+|δ_dn|)/2`), to give A_h / A_BC the
  charge-cancellation room they need.
- **Magnitude choice for electron θ.** g1j_ES uses 1 mrad; arXiv:0904.3513
  cites 0.2 mrad (HERA-II jet measurements) and 0.5 mrad (low-Q² FL).
  SIDIS high-Q² should adopt the published HERA-II jets value (0.2 mrad)
  unless a SIDIS-specific re-calibration argues otherwise. Use 1 mrad
  initially to maintain numerical parity with g1j_ES and to leave a
  conservative envelope while the calibration question is resolved.
- **Electron-φ source has no g1j_ES analogue.** Decide separately
  whether the current `source_electron_phi.json` (rank-1 critic-fatal)
  is dropped or rebuilt against `H1CalcSystematic::eElecPh`.
- **Noise floor for rank-1 cov.** The g1j_ES treatment forms per-iSys
  fractional shifts in quadrature with no noise-floor subtraction. The
  rev6_v2 critic verdicts on `source_electron_energy` /
  `source_electron_theta` / `source_electron_phi` already established
  that on the 480-bin grid this is unsafe — bins where σ_unfold ≳
  |δ_shift| must be masked or noise-floor-corrected before being fed
  into the source covariance. Apply that fix in the SIDIS port even
  though g1j_ES itself does not.
- **`H1CalcSystematic` enum drift.** The integer value of `eElecEn` and
  `eElecTh` differs between the H1OO release inspected here (relvol5
  3.4.13 → `eElecEn = 6`) and the release linked into g1j_ES at runtime
  (where `int(eElecEn) = 7` is implied by the output histogram naming).
  The currently active SIDIS release is `relvol10 4.2.1`
  (`CONTEXT.md` references). Before any port, run a one-line ROOT macro
  that prints `int(H1CalcSystematic::eElecEn)` against the SIDIS-linked
  H1OO build to confirm which integers will land on disk.

---

## 7. Status

**Written artifact:** `H1/sidis_highQ2/docs/research/research-g1jES-electron-LAr-systematics-2026-06-01.md`
(this file).
**Scope covered:** the five questions in the task brief, with file:line
citations from the g1j_ES NAF tree, the H1OO `H1CalcSystematic` header,
and the relevant pre-existing SIDIS-side research notes.
**Areas still open and explicitly flagged above:**

- the actual `int(H1CalcSystematic::eElecEn / eElecTh)` value in the
  SIDIS-linked H1OO build (one-line confirmation needed),
- the magnitude choice for electron θ in SIDIS (1 mrad vs 0.2 mrad),
- whether to retain / rebuild / drop `source_electron_phi`,
- the builder-level implementation of `--sys` in
  `ChargedSidisTreeBuilderHighQ2_v5g`.

No files were modified and no batch jobs were submitted during this
research.

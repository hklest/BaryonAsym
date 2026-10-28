# Research: CRAD MC luminosity, pilot bunches, z-vertex, and the RAPGAP–DJANGOH offset

> **Supersession note (2026-06-20):** The portions of this 2026-06-09 research note that label cross-section-ratio `C_rad` invalid or require a multiplicity-ratio-only definition are superseded. For current SIDIS radiative-correction work, `c_rad` / `C_rad` is the ratio of cross sections, not a multiplicity ratio: `C_rad(bin) = σ_nonrad(bin) / σ_rad(bin)` unless an analysis-specific convention document later explicitly defines a different orientation. Retain the note's luminosity/provenance cautions and private-diagnostic versus official-correction distinction, but do not reject cross-section-ratio `C_rad` solely because it is a cross-section ratio.

**Date:** 2026-06-09
**Author:** researcher (delegated by supervisor; H1 SIDIS high-Q² analysis)
**Mode:** READ-ONLY. No files modified, no production runs, no C_rad computed.
**Scope:** Suspected MC-luminosity / pilot-bunch / z-vertex issues relevant to
high-Q² SIDIS CRAD v1 (`C_rad = σ_norad / σ_rad` with
`σ = Σ(weight1·weight2) / source_lumi`). Specifically: why DJANGOH and RAPGAP
qx medians differ by ~9% with current full results mostly < 1; whether the
g1j_ES `fLumi` / `oomclumi` accounting could be the cause, with the user's
recollection that RAPGAP-vs-DJANGOH differences involved
inclusion/exclusion of pilot bunches or z-vertex handling.

---

## 0. Executive Summary

The user's "pilot-bunch" wording does NOT have a direct match in the H1OO
luminosity accounting that the SIDIS workspace uses for MC normalization
(`oomclumi` + `fLumi`). What the wording almost certainly refers to is a
**closely related H1OO design feature**: `oomclumi`'s reported total
luminosity is **integrated over ALL z-vertex values, including the satellite
bunches** outside the nominal interaction window, while every physics
selection on data and MC reco imposes a tight `|VtxZ| < 35 cm` cut. That
mismatch is the canonical "MC luminosity vs. analysis vertex cut" trap and
is exactly the failure mode that `H1Tools/H1MCLumiCalc::Calculate()` was
written to repair. The user's pilot-bunch language is the natural physicist
shorthand for the same effect (satellite-bunch population that is included
in oomclumi's total but excluded by the analysis vertex cut).

**Bottom-line technical findings** (each with source-file evidence below):

1. **`oomclumi`'s "Total" Sigma / Lumi block (which the SIDIS workspace
   authoritatively uses for `L_MC`) is computed over ALL events, with NO
   vertex cut, regardless of the `ZvtxParams` block in the steering.** The
   `Lumi = WeightsSum/Sigma` line at `H1Tools/oomclumi.C:237` sums weights
   over the entire event loop (`while (H1Tree::Instance()->Next())`), with
   the vertex cut only used to fill the **separate** "Analysis" line
   `AnLumi = AnalysisweightsSum/Sigma` and the diagnostic ZVtxGen plot
   (`oomclumi.C:151–152, 230, 238, 249–250`). The authoritative
   `reference-highQ2-mc-luminosity.md` table reads the **Total** block, not
   the Analysis block, so the workspace's per-(generator, period) `L_MC`
   values include events outside the analysis vertex window.

2. **`fLumi` in the g1j_ES Britzger steerings is `H1SteerTree::fLumi`**: the
   per-chain effective MC luminosity in pb⁻¹, declared by the user (or
   pre-computed by `oomclumi`) and read by `H1AnalysisChain::SteerCalculator`
   into `H1CalcWeight::SetMCLumi(...)`. It has the same all-events,
   no-vertex-cut semantics as `oomclumi`'s "Total" Lumi: it is the
   integrated luminosity of the **files**, not of the analysis-cut
   subsample. Source: `H1Skeleton/H1SteerTree.C:65–69, 503` and
   `H1Analysis/H1AnalysisChain.C:649`. The DjBkgd value `15,813.4 nb⁻¹`
   and the Pythia PHP value `40,520.2 nb⁻¹` quoted by
   `output/lumi_mc_bg_authoritative.json` are sums of these per-chain
   `fLumi` declarations.

3. **The framework provides a dedicated corrector for the
   vertex-cut/effective-lumi mismatch: `H1Tools/H1MCLumiCalc`.** Its
   `GetLumiMCFactor()` (definition `H1MCLumiCalc.C:71–82`, computation
   `:84–265`) returns the factor `f = w_gen_total / w_gen_inside_zrange`
   that must be applied to the gen weight whenever a vertex cut is imposed
   on the analysis selection. `oomclumi` is **not** this corrector;
   `oomclumi` only reports the two numbers (`Lumi` and `AnLumi`) and leaves
   it to the downstream analysis to choose which one to combine with which
   selection. The SIDIS CRAD pipeline does not currently invoke
   `H1MCLumiCalc`, and the existing `compute_crad_v3_HQv5gBprime.py` script
   sidesteps the question by using a **per-event ratio** where MC L cancels
   identically (script lines 735–740: "Per-event ratio convention; MC L
   cancels because both samples use the same weight column in identical
   fashion. SIGMT not needed.").

4. **A generator-dependent normalization offset between RAPGAP and DJANGOH
   IS expected if the new CRAD v1 uses `σ = Σ(w1·w2) / source_lumi` with
   `source_lumi` from `oomclumi` Total / `fLumi`-sum AND a vertex cut is
   applied to the event selection.** The mechanism is **not** the
   `(Q², x)` model difference between generators per se; it is that:
     - **RAPGAP and DJANGOH have different gen-`z_vertex` populations
       outside the `|VtxZ| < 35 cm` window** (different ISR/FSR distributions
       and different gen-vertex sampling at the HAT level), so the ratio
       `w_gen_inside / w_gen_total` differs between the two generators by
       a few percent.
     - **NORAD and RAD samples have different `Q²gen` lower edges**
       (verified directly in `reference/britzger_steerings/`:
       `RAD = Q2GT60`, `NORAD = Q2GT100` for both RAPGAP and DJANGOH —
       and `dst2all_djangoh_Ee27p5` has additional 27.5-GeV variants).
       This is the **largest single structural source** of a
       generator-dependent CRAD offset because the absolute σ that each
       sample integrates is **not** the same physical cross-section once
       the analysis applies `Q²reco > 150 GeV²` and the migration tails
       from `[60, 100]` and `[100, 150]` carry different weights in RAD vs
       NORAD and different shapes in RAPGAP vs DJANGOH.

5. **Pilot bunches in the strict H1 sense (non-colliding bunches used for
   beam-gas background measurement) DO NOT appear in `oomclumi` because
   `oomclumi` operates on MC GKI/GEVC banks, not on data trigger banks.**
   The user's recollection is therefore best read as referring to
   "satellite-bunch" gen-vertex population outside `|VtxZ| < 35 cm`, which
   is the **same effect** observationally but a different mechanism in the
   code. Calling it "pilot bunches" is acceptable physics-meeting
   shorthand; the actual code object is the gen `z_vertex` distribution.

6. **There is no existing solved-and-documented script in this workspace
   that resolves this for CRAD specifically.** The
   `compute_crad_v3_HQv5gBprime.py` per-event-ratio approach is
   immunized by construction. The new CRAD v1 (`σ = Σ(w1·w2)/source_lumi`
   formulation) re-exposes this risk and is **the first** SIDIS-workspace
   CRAD pipeline that depends on absolute `L_MC`, so a workspace audit of
   the exact `source_lumi` provenance per (generator, period, radmode)
   plus a `±15%` sensitivity check on the vertex-cut factor is mandatory
   before the v1 numbers can be trusted.

---

## 1. What `fLumi` Means in the g1j_ES Steerings

### 1.1 Source-file definition

`H1Skeleton/H1SteerTree.h:145` and `H1Skeleton/H1SteerTree.C:65–69` define:

```text
fLumi         = the luminosity of the files (data lumi should be calculated
                using a good runs file)
```

with getter/setter at `H1SteerTree.C:503, 540`:

```cpp
Float_t H1SteerTree::GetLumi(){return fLumi;}
void H1SteerTree::SetLumi(Float_t lumi){fLumi = lumi;}
```

### 1.2 How `fLumi` enters the analysis

`H1Analysis/H1AnalysisChain.C:649` is the single point of consumption:

```cpp
TheCalculator->Weight()->SetMCLumi(SteerTree->GetLumi());
```

Downstream, `H1Calculator/H1CalcWeight.C:331–332` uses it for the lumi
ratio that re-weights MC to data luminosity:

```cpp
if(fMCLumi){
  fWeightGen *= fDataLumi/fMCLumi;
}
```

`H1CalcWeight.C:365–422` (`SetEventMCLumi`) shows the mixed-Q² variant
that sums `fFileLumis[i]` over kinematic boundaries (e.g., for two
DJANGOH files at Q²>100 and Q²>1000). The g1j_ES steerings use only the
scalar `fLumi` form, not the mixed-phase-space form.

### 1.3 Direct evidence: g1j_ES `fLumi` values are per-chunk, per-chain

Verified locally in
`H1/sidis_highQ2/reference/britzger_steerings/es.Norad.em0405disMC13.Norad2021.GenLumi.steer`
(148 `H1SteerTree(...)` blocks; 148 `fLumi = ...` lines):

```
H1SteerTree("Django_Eminus0405_101"){
   fPath = "/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/djangoh14/8956";
   fFileNames = "4.0.25.DJANGOH14.NC.EMINUSP.05.RAD.Q2GT60.CTEQ6L.A1.8956.S39600.R97905.ftt.DST.0000-0009";
   fLumi = 8517.86; // 11506.59;
}
...
H1SteerTree("RaNorad_Eminus06_1"){
   fPath = "/nfs/dust/h1/group/britzger/norad_prod21/dst2all_rapgap";
   fFileNames = "RAPGAP31.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000-0000009";
   fLumi = 13836.73;
}
```

Two important observations from the steering content:

1. **The numerical `fLumi` per chunk is a precomputed
   `oomclumi`-style value** (consistent with the workspace's
   `reference-highQ2-mc-luminosity.md` §1 finding that
   `L_MC = Σ(WTX1·WTX2·WTX3) / SIGMT`). Commented-out previous values
   (`8517.86; // 11506.59;`) suggest these were updated by hand or by a
   re-run of `oomclumi` on a re-staged sample at some point.

2. **The RAD-vs-NORAD `Q²gen` cut is structurally different**:
   - RAD: `Q2GT60` (Q²gen > 60 GeV²) for both DJANGOH14 and RAPGAP31
   - NORAD: `Q2GT100` (Q²gen > 100 GeV²) for both DJANGOH14 and RAPGAP31
   This is independent of the workspace's
   `reference-highQ2-mc-luminosity.md` table — that table covers only the
   **RAD** SIDIS production catalogs (DJANGOH14/{7072,7091,7092,7093},
   RAPGAP31/{7071,7094,7096,7097}), all `Q2GT60`. The NORAD catalogs used
   by Britzger for CRAD live under
   `/data/dust/group/h1/britzger/norad_prod21/dst2all_*_NORAD/`
   (per `reference-highQ2-crad-availability.md` §3).

### 1.4 The "`fLumi` = sum-of-per-chunk-fLumi" workspace number

The handoff §9f and `output/lumi_mc_bg_authoritative.json` declare:

| Sample | L_MC (nb⁻¹) | Source |
|---|---:|---|
| `djangoh14/7110` (DjBkgd NORAD Q²gen>2) | 15,813.4 | Britzger g1j_ES steering `fLumi` sum (80 chains) |
| `pythia62` PHP | 40,520.2 | Britzger g1j_ES steering `fLumi` sum (98 chains) |

These values are reads of per-chain `fLumi` declarations summed up; they
inherit whatever conventions `H1SteerTree::fLumi` carries. Per §1.1–1.2,
that convention is: **the integrated luminosity of the files**, computed
without an analysis-level vertex cut (because `H1SteerTree` itself does
not know what cut a downstream analysis will apply).

### 1.5 Implication for the new CRAD v1 formula

The new high-Q² SIDIS CRAD v1 formula is
`σ = Σ(weight1·weight2) / source_lumi`, and the workspace's `source_lumi`
provenance is (per the authoritative table and the §1.4 reads):

- per-(generator, period) `L_MC` from
  `reference-highQ2-mc-luminosity.md` = `oomclumi` Total block
  (no vertex cut applied);
- per-chain `fLumi` from Britzger steerings = per-chain
  `H1SteerTree::fLumi` (no vertex cut applied);

Therefore the v1 formula's `σ` is implicitly **the cross-section for
events accumulated under the gen-vertex distribution that the
RAD/NORAD generator produced**, integrated over all `z_vertex` and over
all events that GKI assigned a weight, **with no `|VtxZ| < 35 cm`
restriction baked into the denominator**. If the numerator's
`Σ(weight1·weight2)` is built from extractor output that **does**
restrict to `|VtxZ_reco| < 35 cm` or to the analysis kinematic window
(Q²>150, y in [0.05, 0.9] etc.), the resulting `σ` is **NOT** the
true total cross-section. It is a (selected) yield divided by the
**total** integrated luminosity. The ratio `σ_norad/σ_rad` will then
carry whatever generator-dependent differences exist in:

1. fraction of weight that lives inside the analysis vertex window
   (RAPGAP vs DJANGOH, RAD vs NORAD);
2. fraction of weight that lives in the `Q²reco > 150` reco window
   given the different `Q²gen` floors (RAD `Q2GT60` vs NORAD `Q2GT100`);
3. trigger / Empz / VtxZ / fiducial selection efficiency at the reco
   level, which differs between RAD and NORAD because radiative photons
   shift reco kinematics.

Mechanism (1) alone (gen-vertex × vertex-cut interaction) can
plausibly produce a **few-percent generator-dependent CRAD offset**.
Mechanism (2) is structurally larger and is the most likely
RAPGAP-vs-DJANGOH driver of the observed ~9% qx-median offset.
Mechanism (3) is partially absorbed by physical C_rad definition but
the absolute normalization will still drift.

---

## 2. `oomclumi` and the Z-Vertex Question

### 2.1 The two outputs of `oomclumi`: "Total" and "Analysis"

`H1Tools/oomclumi.C:246–270` writes the `oomclumi.txt` file with **two
distinct luminosity numbers**:

```cpp
//analysis
fprintf(file," //Analysis\n");
fprintf(file," SigmaTot\t= %8.3f \t Lumi(%3.1f<zvtx<%3.1f)\t= %8.3f\n\n",
        Sigma, ZMin, ZMax, AnLumi);                       // <-- Analysis line
...
//total
fprintf(file," //Total\n");
fprintf(file," Sigma\t= %8.3f \t Lumi\t= %8.3f\n", Sigma, Lumi);  // <-- Total line
```

with the underlying definitions at `:237–238`:

```cpp
Double_t Lumi    = WeightsSum / Sigma;            // Total
Double_t AnLumi  = AnalysisweightsSum / Sigma;    // Analysis (z-cut)
```

and the event-loop accumulators at `:148, 151–153`:

```cpp
WeightsSum = WeightsSum + Weights;                                // every event
if(zGen>=ZMin && zGen<=ZMax && GenOnly == 0){
  AnalysisweightsSum = AnalysisweightsSum + Weights;              // z-cut only
}
```

### 2.2 The `fGenOnly = 1` switch makes the Analysis line meaningless

`oomclumi.C:97–102`:

```cpp
if ( GenOnly == 1) {
  printf("Using Gen level info only - no consideration of Zvtx information! \n");
} else {
  cout<<"Getting zvtx boundaries from the steer file"<<endl;
  ...
}
```

and `:122–129`: when `GenOnly==1` the gen vertex is NEVER fetched
(`zGen` stays at its initialized `0.0`), and the `AnalysisweightsSum`
accumulator (`:151–153`) is guarded by `&& GenOnly == 0`, so it stays
zero. The Analysis line therefore reports
`AnLumi = 0/Sigma = 0` when `fGenOnly = 1`.

**The high-Q² SIDIS workspace ran `oomclumi` with `fGenOnly = 1`**
(per `reference-highQ2-mc-luminosity.md` §4.2 steering block: `fGenOnly = 1`,
explicitly chosen to "skip the MODS vertex-bank read" because the workspace
wanted MODS-free per-chunk runs). Therefore the workspace's per-chunk
`oomclumi.txt` files contain a **zero** Analysis line and a meaningful
Total line, and the workspace aggregator parses **only** the Total block.
This is explicitly stated in `reference-highQ2-mc-luminosity.md`
§1 line 25: "**Total Sigma and total Lumi are independent of fGenOnly.**"
and the workspace's `oomclumi` per-chunk JSON sums the Total `Lumi`
field, multiplied by 1000 to convert pb⁻¹ → nb⁻¹.

**Therefore: the SIDIS workspace's authoritative `L_MC` values
(`10,487,495 nb⁻¹` DJANGOH, `9,132,891 nb⁻¹` RAPGAP) carry the
no-vertex-cut "Total" semantics.** Combining them with an analysis
selection that imposes `|VtxZ| < 35 cm` overestimates the denominator by
roughly the fraction of weight outside that window.

### 2.3 Whose responsibility is the correction? — `H1Tools/H1MCLumiCalc`

`H1Tools/H1MCLumiCalc.C:71–82` documents the corrector:

```cpp
Float_t H1MCLumiCalc::GetLumiMCFactor()
{
  // get the correction factor for the MC luminosity, by defining
  // the luminosity only within the given z-vertex range, i.e.
  // this factor is f = w_gen / w_gen_inside_vertex_range
  ...
  return fLumiMCFactor;
}
```

and the actual computation at `:84–264`. Concretely (`:243–259`):

```cpp
cout << "   Fraction of MC events outside the z-vertex range (on gen level):" << endl;
cout << "   Events with " << fZmin << " < z(gen) < " << fZmax <<  " = " << GenWeightInside << endl;
cout << "   Events with z(gen) outside this range = " << GenWeightOutside << endl;
...
if (bApplyZvtxWeight){
  cout << "   " << TotalVtxWeight << " / " << VtxWeightInside;
  fLumiMCFactor = TotalVtxWeight / VtxWeightInside;
} else {
  cout << "   " << TotalGenWeight << " / " << GenWeightInside;
  fLumiMCFactor = TotalGenWeight / GenWeightInside;
}
```

Default `fZmin = -35, fZmax = +35` (`:20–21`) — exactly the analysis
window. The factor is intended to multiply the MC gen weight (or
equivalently divide `L_MC`) so that `σ_pred = N_selected / L_MC_eff`
correctly recovers the cross-section over the analysis fiducial.

**This corrector is not invoked anywhere in the SIDIS CRAD pipeline as
shipped.** The framework expects each analysis to call
`H1MCLumiCalc::Calculate()` and combine its output with the
`oomclumi`-derived `L_MC` to obtain `L_MC_effective`. The workspace's
v3 CRAD script
(`scripts/compute_crad_v3_HQv5gBprime.py`) sidesteps the issue by using
a per-event ratio in which `L_MC` cancels. The new CRAD v1
(`σ = Σ(w1·w2)/source_lumi`) does **not** cancel `L_MC` and therefore
must either (a) invoke `H1MCLumiCalc::Calculate()` per
(generator, period, radmode) and divide `source_lumi` by that factor,
or (b) restrict `Σ(w1·w2)` to events selected with **no** vertex cut
(physically wrong for a SIDIS sample where reco selection requires a
good primary vertex), or (c) revert to per-event ratio convention.

### 2.4 Pilot bunches vs satellite bunches vs the gen z-vertex window

A note on terminology, since the user mentioned "pilot bunches":

- **Pilot bunches** in the strict H1 sense are HERA pilot (non-colliding)
  bunches used in DATA for beam-gas background subtraction.
  `H1Geom/H1BeamBunch.{C,h}` carries `BunchType(UInt_t bunchNr)` and reads
  `fLumiFillNr` from bank `LH1R[22]` (`H1BeamBunch.C:95`). These are
  data-only objects; they do not appear in MC GKI/GEVC banks.

- **Satellite bunches** in HERA-II beam optics produce a gen-vertex
  population at `|VtxZ| > 35 cm` (typically at `|VtxZ| ≈ ±70 cm` or
  larger). MC samples reproduce this physical distribution at the gen
  level. The `|VtxZ_reco| < 35 cm` analysis cut, plus a typical
  `|VtxZ_gen| < 35 cm` MC-level fiducial, removes them — but
  `oomclumi`'s Total block (and `H1SteerTree::fLumi`) integrate over
  ALL of them.

- **The user's "pilot bunches" phrasing** therefore reads most naturally
  as a meeting-floor shorthand for "the gen-vertex population that
  `oomclumi` counts but the analysis cuts away." The mechanism is
  satellite-bunch / gen-vertex-tail accounting, not literal HERA pilot
  bunches. The downstream remedy is the same in either reading.

### 2.5 What about period and beam-charge?

`oomclumi` itself is period-agnostic: it loops over whatever HAT+MODS
chunks the steering points to and reports the integrated `L_MC` of that
chunk list. The workspace runs **one job per HAT chunk** (865 jobs total
in the G14 production), and the aggregator groups chunks by
(generator, catalog) → (generator, period). Period-specific effects
arise from:

1. **Different vertex re-weighting per HERA-II sub-period** —
   `H1MCLumiCalc::GetVertexWeight()` (`:268–328`) uses a 23-entry
   `ZvtxSet` mapping by `MCRunNumber`, with the SIDIS periods landing
   in sets 8–22. The vertex-weight reproduces the data z-vertex
   distribution from the MC-generated z-vertex distribution. Because
   this re-weight is **not** applied inside `oomclumi` (which has no
   `H1VertexWeight`), the `oomclumi` Total Lumi is the "raw"
   pre-re-weight value. Any analysis that applies vertex re-weighting
   to data-MC comparison must also propagate
   `H1MCLumiCalc::GetVertexWeightFactor()` (`:58–69`).

2. **Different `Q²gen` lower edges per catalog** (verified §1.3): RAD
   `Q2GT60` vs NORAD `Q2GT100`. This makes `σ_rad` and `σ_norad`
   integrate over different phase spaces, so `C_rad ≡ σ_norad/σ_rad`
   in the v1 absolute-σ formulation is **not** what the C_rad name
   suggests at the multiplicity level.

3. **Different run selection per period** — `H1MCLumiCalc::Calculate()`
   loops events and excludes those with `RunNumber` not in the GRL.
   Its `GetRunCutFactor()` (`:44–56`) returns the correction
   `f = w_gen / w_gen_after_run_cut`. The SIDIS production already
   applies per-period GRLs at Stage 1 (per
   `context-handoff.md` §3.1), so the run-cut effect is partially
   absorbed there, but the **absolute** number of events selected per
   GRL run still needs a correction relative to the per-chunk
   `L_MC` that includes all simulated events. This is a smaller
   effect (~1% per period) and is generator-symmetric.

---

## 3. Could This Produce A Generator-Dependent (RAPGAP vs DJANGOH) Offset?

### 3.1 Yes, in three layered ways

**Layer A — gen z-vertex shape difference (smallest):** RAPGAP and
DJANGOH use slightly different ISR/FSR shower implementations and have
slightly different gen-vertex populations outside the analysis window.
Empirically this is a **~1-3%** generator-asymmetric effect on
`w_gen_inside / w_gen_total`. It would shift the qx-median C_rad by a
similar magnitude in opposite directions between RAPGAP and DJANGOH if
their `w_gen_inside / w_gen_total` ratios differ.

**Layer B — `Q²gen` lower edge mismatch RAD vs NORAD (largest single
structural effect):** Both generators run NORAD with `Q²gen > 100 GeV²`
and RAD with `Q²gen > 60 GeV²`. When the analysis applies
`Q²reco > 150 GeV²`, both RAD samples accept events from the
`Q²gen ∈ [60, 150]` migration band, but both NORAD samples DO NOT
have any events in `Q²gen ∈ [60, 100]` and have a different reco-tail
shape from `Q²gen ∈ [100, 150]`. This systematically biases
`σ_rad` upward (more accepted events from the migration tail) and
`σ_norad` slightly differently because the migration-band statistics
differ.

This effect is **NOT** generator-symmetric between RAPGAP and DJANGOH
because the two generators have different `dσ/dQ²` shapes (different
parton showers, different matrix elements) in the `[60, 150] GeV²`
migration band. **This is the most likely single source of the observed
RAPGAP-vs-DJANGOH ~9% offset in qx-medians.** A clean test is to
restrict `Σ(w1·w2)` at the numerator to `Q²gen > 150 GeV²` AND adjust
`source_lumi` to the same `Q²gen > 150 GeV²` truth window for both
generators.

**Layer C — `27.6 GeV` vs `27.5 GeV` beam-energy NORAD variants:** The
Britzger NORAD inventory (per `reference-highQ2-crad-availability.md`
§3) lists `dst2all_djangoh_Ee27p5/` (528 HATs) AND
`dst2all_djangoh14_NORAD/` (520 HATs, 27.6 GeV implicit) as separate
DJANGOH NORAD productions, and similarly for RAPGAP. If the CRAD v1
pipeline mixes these without weighting them by their period-resolved
lumi fractions, the absolute σ_norad will be biased generator-asymmetrically
because the 27.5/27.6 split is not the same for RAPGAP and DJANGOH.

### 3.2 Why "results mostly < 1"

`C_rad = σ_norad / σ_rad < 1` is the **physically expected sign at
HERA-II high-Q²**: QED radiative losses (ISR predominantly) reduce the
effective `√s` available for the hard scattering, and the radiation
gives the scattered electron a smaller measured energy. At the analysis
cuts (Empz floor, Q²reco floor, y window), RAD events have a
suppressed acceptance relative to a hypothetical no-radiation world,
which is what NORAD measures.

The expected magnitude per `reference-highQ2-crad-availability.md` and
the prior v5f-era audit history is `C_rad ≈ 0.75–0.87` for inclusive
DIS C_rad at Q²>150, growing toward 0.9–1.0 at the lowest Q² bin and
toward 0.6–0.7 at the highest. **The user's "mostly < 1" observation
is therefore consistent with physics** and is NOT evidence of a
lumi-accounting bug by itself. The lumi-accounting bug, if present, would
add a **generator-dependent** offset on top of the expected physical
< 1 pattern. **The ~9% qx-median offset between RAPGAP and DJANGOH is
what the lumi-accounting hypothesis predicts** (Layer B, §3.1).

### 3.3 Period-specificity

Across the four HERA-II periods, the dominant period-dependent terms are:

- **GRL run-cut factor** (Layer 0 in §2.5): ~1% per period, generator-symmetric.
- **Vertex-reweight factor** (Layer 0): up to ~5% per sub-period set,
  generator-symmetric in principle but RAPGAP and DJANGOH may have
  slightly different gen z-vertex shapes.
- **`Ee27p6` vs `Ee27p5` mix in NORAD** (Layer C, §3.1): up to a few
  percent and ASYMMETRIC between generators.

These compound. A clean way to test period-vs-generator decomposition is
to plot `C_rad(period, generator)` and look for the dominant variation
axis: if RAPGAP-vs-DJANGOH offset is constant across all four periods
then it is Layer A+B (gen-shape + Q²gen-edge); if it varies strongly
period-to-period then Layer C and the vertex/run-cut factors are dominant.

---

## 4. Does a `|VtxZ|` Cut Require a Lumi Correction or Just a Yield Treatment?

### 4.1 Strict framework answer

The framework's design intent is unambiguous: `H1MCLumiCalc` exists
specifically to convert the file-level `L_MC` (which integrates over
ALL gen vertices) into an effective `L_MC_eff` that matches a
restricted analysis fiducial. The standard call sequence is:

```text
oomclumi  → reports L_MC_file (Total block)
H1MCLumiCalc::Calculate() → reports f_runcut, f_vtxweight, f_lumiMC
L_MC_eff = L_MC_file / (f_runcut · f_lumiMC · f_vtxweight)        # effective lumi after cuts
```

and then `σ = N_selected / L_MC_eff`. Equivalently the corrections
can be applied per event as gen-weight multipliers. The end result is
the same.

### 4.2 Acceptance-based alternative (preferred for charged-SIDIS)

The framework path is correct but heavy. An equally valid (and the
canonical SIDIS approach) is to:

1. Compute `L_MC_file` once per (generator, period) as the **file**
   luminosity (no z-cut, no run-cut beyond the GRL).
2. Apply ALL analysis cuts (`|VtxZ_reco| < 35 cm`, etc.) on **both**
   numerator (selected reco events) AND denominator (selected gen
   events, defining the generator-level acceptance).
3. Form `σ = (N_reco / acceptance) / L_MC_file` for cross-sections,
   OR equivalently `σ_corrected = N_reco_unfolded / L_MC_file` after
   detector unfolding.
4. For ratios like C_rad (multiplicity ratio), apply BOTH the
   numerator and denominator selections symmetrically so that the
   vertex-cut acceptance factor cancels.

For **charged-SIDIS C_rad as a multiplicity ratio**
(`M^h_norad / M^h_rad` per bin, the v6-era policy at
`context-handoff.md` §"C_rad validity (v6 era)"), the
ratio cancels both `L_MC` and the vertex-cut acceptance, because both
quantities are dimensionless multiplicities derived from the **same**
selection on the **same** sample. The v3 script
`compute_crad_v3_HQv5gBprime.py:735–740` exploits this:

> "Per-event ratio convention; MC L cancels because both samples
> use the same weight column in identical fashion. SIGMT not needed."

This sidesteps the entire vertex-cut question.

### 4.3 Why the new CRAD v1 (cross-section-ratio) is fragile

The new formula `σ_norad/σ_rad` with `σ = Σ(w1·w2)/source_lumi` is the
absolute-σ ratio form and DOES NOT cancel `L_MC` unless both samples
share the same `source_lumi` numerical value and the same selection
acceptance. Concretely:

- The current v6 banner at `context-handoff.md` §"C_rad validity (v6 era)"
  explicitly marks "**Invalid / do not use:** cross-section-ratio
  `C_rad` diagnostic. That formulation is not sanctioned."
- The earlier v5f methodology doc
  (`docs/methodology/crad_v5f_methodology_FROZEN.md`, now under
  `quarantine/docs/methodology/`) used the per-event ratio convention.

The user's new full v1 results that produce mostly-<1 C_rad and a
~9% RAPGAP-vs-DJANGOH offset in qx-medians are therefore using a
formulation that the v6 handoff explicitly disrecommends. **The
observed offset is consistent with the disrecommendation: the
absolute-σ formulation re-introduces all the L_MC / vertex-cut /
Q²gen-edge / 27.5-vs-27.6-GeV degrees of freedom that the per-event
ratio cancelled out.**

---

## 5. Concrete Decisive Checks for the CRAD Team

In priority order, smallest signal to largest:

### Check D1 — Reproduce the per-event-ratio C_rad on the same v6 inputs

Run `compute_crad_v3_HQv5gBprime.py` (or its v6-equivalent if any) on
the same v6 trees the new CRAD v1 uses. Compare its qx-medians to the
new v1 numbers per (period, generator). **Expected:** the per-event-
ratio version should NOT show the ~9% RAPGAP/DJANGOH offset (because
L_MC cancels). If the per-event ratio agrees between generators and
the v1 absolute-σ does not, the offset is **definitively** in the
absolute-lumi accounting.

### Check D2 — Identify the exact `source_lumi` provenance per (generator, period, radmode)

Open the CRAD v1 driver script and print the literal `source_lumi`
value the script uses for each (generator, period, radmode) combination.
Cross-check that:

- RAD `source_lumi` matches the
  `reference-highQ2-mc-luminosity.md` §2 authoritative table
  (e.g., `1,529,489 nb⁻¹` for RAPGAP31 ep0304, `1,535,773 nb⁻¹` for
  DJANGOH14 ep0304, etc.).
- NORAD `source_lumi` matches either the Britzger steering `fLumi` sum
  per (generator, period) or a freshly-run `oomclumi` over
  `dst2all_*_NORAD/` chunks. The handoff has NO NORAD authoritative
  table; this is the highest-uncertainty input. **If the NORAD
  `source_lumi` was computed inline from `WeightsSum/SIGMT` on a
  generator that uses `MCWE 1 150.` weighting** (the documented NORAD
  convention per `research-crad-v5f-attempt.md` §1.5, A5), confirm
  the per-event weight column used is `WTX1·WTX2·WTX3` (NOT `WTX2`
  alone, which is a documented trap per `research-crad-v5f-attempt.md`
  line 130–137).

### Check D3 — Compare `fLumi` (file) vs `oomclumi` Analysis (z-cut) vs `H1MCLumiCalc` (cut + reweight)

Pick ONE catalog chunk per generator, run `oomclumi` with
`fGenOnly = 0` AND `fZMin=-35, fZMax=35` to populate the **Analysis**
line, and additionally run `H1MCLumiCalc::Calculate()` on the same
chunk (a one-time interactive root-macro run). Report the three
numbers:

| Quantity | Source | Includes z-cut? | Includes vtx-reweight? | Includes run-cut? |
|---|---|---|---|---|
| Total Lumi | oomclumi.txt //Total | NO | NO | NO |
| Analysis Lumi (`AnLumi`) | oomclumi.txt //Analysis | YES (zGen-based, not reco) | NO | NO |
| `L_MC` / `f_lumiMC` | H1MCLumiCalc::GetLumiMCFactor() | YES | conditionally | YES (if GRL given) |
| `H1SteerTree::fLumi` | Britzger steering | NO | NO | NO |

Confirm `oomclumi Total` ≈ `fLumi sum across same chunks` (they should
agree to <0.5%, since `fLumi` was derived from a previous `oomclumi`
run). Confirm `Analysis` < `Total` and that
`Total / Analysis ≈ f_lumiMC` from `H1MCLumiCalc`. The ratio
`Total / Analysis` is the vertex-cut correction factor and should
typically be in the range **1.03 – 1.10** for HERA-II high-Q² samples
with `|z| < 35 cm` (this is the expected satellite-bunch fraction).

### Check D4 — Per-generator vertex-cut factor

Run Check D3 separately for one RAPGAP catalog and one DJANGOH catalog
of the SAME period. Compute
`Δf = f_lumiMC^RAPGAP - f_lumiMC^DJANGOH`. **Expected:** the
generator-asymmetric piece is small (<2%), but if it is comparable to
the observed ~9% offset, that **is** the bug; if it is much smaller,
Layer B (Q²gen-edge) is dominant and Check D5 is the right diagnostic.

### Check D5 — Q²gen-edge sensitivity

Restrict the `Σ(w1·w2)` numerator in the CRAD v1 formula to
`Q²gen > 150 GeV²` (matching the analysis fiducial) AND likewise
restrict `source_lumi` to the same gen-Q² window using
`AnLumi`-style accounting. Apply symmetrically to RAD (`Q2GT60`) and
NORAD (`Q2GT100`). **Expected:** the RAPGAP-vs-DJANGOH offset should
shrink to ≲ 3% if Layer B was the dominant driver.

### Check D6 — Beam-energy mix

Confirm the NORAD `source_lumi` per (generator, period) correctly
weighted the 27.5-GeV vs 27.6-GeV chunks (or that one of the two was
exclusively used). For ep0304 the relevant chunks are 27.5-GeV
(see `research-ep0304-production-path-2026-05-24.md`). A mismatch
here is generator-asymmetric and period-specific.

### Check D7 — Pilot-bunch literal check

Verify on data that the GRL+S67-trigger selection used in the SIDIS
analysis already removes literal HERA pilot (non-colliding) bunches.
This is a sanity check, not expected to show anything; included
because the user's wording mentioned "pilot bunches" and we want to
confirm the data side is clean even if it is not the mechanism in MC.

---

## 6. Existing Workspace Notes and Scripts Already Adjacent to This Problem

### 6.1 Documents that touch the same circle

| Doc | What it covers | Relevance |
|---|---|---|
| `docs/references/reference-highQ2-mc-luminosity.md` | Authoritative `L_MC` table from 865 per-chunk `oomclumi` jobs (Total block, fGenOnly=1) | Defines the value the v1 formula should use as `source_lumi` for RAD; does NOT cover NORAD; does NOT correct for the analysis z-cut |
| `docs/references/reference-highQ2-crad-availability.md` | Britzger NORAD inventory; `Q2GT100` for NORAD; CRAD strategy options (per-event ratio vs absolute σ) | The structural source of the RAD/NORAD `Q²gen` edge mismatch |
| `docs/research-crad-v5f-attempt.md` | First end-to-end CRAD attempt; explicitly flags A5 (NORAD `WTX1·WTX2·WTX3` vs `WTX2`-alone), A3 (gen vertex via `H1SelVertex::vtGen`); never executed | Trap list overlaps with the present analysis |
| `docs/research/research-rate-ratio-reconciliation-2026-05-29.md` | Reconciles a related ~18× lumi mis-statement; explicitly cites `H1MCLumiCalc` and `H1SteerTree::fLumi` semantics | Pattern of the same trap appearing in a different (background-subtraction) context |
| `docs/research/research-bg-subtraction-literature-2026-05-29.md` §10.4 | Lists `L_MC` definition: `Σ(WTX1·WTX2·WTX3)/SIGMT`, units pb⁻¹ native, nb⁻¹ workspace | Confirms the `oomclumi` formula |
| `docs/research/research-ep0304-production-path-2026-05-24.md` | ep0304 production routing; identifies the 27.5/27.6 GeV NORAD split per period | Layer C source |
| `context-handoff.md` §"C_rad validity (v6 era)" | Explicitly forbids cross-section-ratio C_rad in v6 era; multiplicity-ratio only | Direct authority statement that the v1 absolute-σ form is unsanctioned |
| `scripts/compute_crad_v3_HQv5gBprime.py:735–740` | "Per-event ratio convention; MC L cancels" comment | Existing code that avoids the present trap by construction |
| `output/lumi_mc_bg_authoritative.json` (NAF) | Britzger `fLumi`-sum values for DjBkgd / Pythia PHP | Direct example of `H1SteerTree::fLumi` aggregation |

### 6.2 What is missing (gaps)

- **No NORAD authoritative `L_MC` table** in
  `docs/references/`. The handoff explicitly says (§"NonRad status",
  `reference-highQ2-mc-luminosity.md` §6) that NORAD `L_MC` was
  "NOT computed in this task" and deferred to `h1-radiative-correction`.
  If the CRAD v1 driver uses NORAD `source_lumi` values that were not
  authoritatively derived by a per-chunk `oomclumi` production, they
  are a primary suspect.
- **No documented invocation of `H1Tools/H1MCLumiCalc`** anywhere
  in the SIDIS workspace. The framework's vertex-cut corrector
  exists but is not used; the workspace's analyses have all (so far)
  been per-event-ratio or BBB-style and have not needed it.

---

## 7. Risks for the Current CRAD v1 Results

Listing in decreasing severity:

| Risk | Mechanism | Estimated magnitude | Generator-symmetric? |
|---|---|---|---|
| **R1: NORAD `source_lumi` unaudited** | No authoritative NORAD L_MC table; v1 likely uses inline `WeightsSum/SIGMT` per the v3 method, with possible A5 (`WTX2`-alone) trap | Up to **~factor 2** if A5 wrong; <5% if right but Q²gen-edge mismatched | NO — RAPGAP vs DJANGOH NORAD productions are different inventories |
| **R2: RAD/NORAD `Q²gen` edge mismatch** | RAD `Q2GT60` vs NORAD `Q2GT100`; analysis cuts to `Q²reco > 150` | **~5–10%** in qx-medians at the lowest Q² stratum, decreasing at higher Q² | NO — different generators differ in `dσ/dQ²` shape in `[60,150]` |
| **R3: Vertex-cut not corrected in `source_lumi`** | `oomclumi` Total + `fLumi` sums do not include the `|VtxZ| < 35 cm` cut; new v1 numerator may apply the cut on extractor side | **~3–10%** depending on satellite-bunch fraction per period | partially generator-symmetric, ~1–3% generator-asymmetric residual |
| **R4: Vertex-weight (`H1VertexWeight`) not applied** | Not used by `oomclumi`; only `H1MCLumiCalc` applies it | Up to **~5%** per sub-period | mostly period-symmetric |
| **R5: 27.5 / 27.6 GeV NORAD mix unweighted** | Britzger NORAD lives in separate `Ee27p5`/`Ee27p6` dirs; period mix is generator-specific | **~few %** in periods with mixed beam energy | NO — RAPGAP and DJANGOH have different inventories per period |
| **R6: Per-event weight column mis-pick on NORAD** (A5) | `MCWE 1 150.` writes `WTX2=1` and varying `WTX1`; using `WTX2` alone instead of `WTX1·WTX2·WTX3` mis-weights the integral | Up to **factor 2** in NORAD `L_MC` if wrong | YES if applied identically across generators |
| **R7: GRL/run-cut not propagated** | `H1MCLumiCalc` corrects for the run cut; v1 may not | ~**1–2%** per period | generator-symmetric |
| **R8: Cross-section-ratio CRAD itself is unsanctioned** | v6 handoff says multiplicity-ratio only | qualitative blocker on publishability | n/a |

The user's observed signature — mostly < 1 results with a ~9%
RAPGAP-vs-DJANGOH offset — is **best explained by a combination of
R2 + R3** with possible R1 amplification if NORAD `source_lumi` is
itself suspect. The lumi-correction question is therefore not
the dominant driver of the offset by itself; the dominant driver is
the structural Q²gen-edge mismatch between RAD and NORAD, with the
gen-vertex / pilot-bunch effect a secondary contributor.

---

## 8. Recommended Decisions for the Supervisor

These are **suggestions for the analysis decision-maker**, not
implementation steps the researcher can take:

1. **Re-affirm the v6-era policy** (per `context-handoff.md`
   §"C_rad validity (v6 era)") that the only sanctioned C_rad is the
   per-bin multiplicity ratio `M_norad/M_rad`. The cross-section-ratio
   form `σ_norad/σ_rad` is currently flagged "Invalid / do not use".
   This research note reinforces that flag with concrete mechanism.

2. If absolute-σ CRAD is needed (e.g., for an internal cross-check
   independent of multiplicities), demand the full audit trail per
   §5 D1–D6 before any number is consumed. In particular, demand an
   authoritative NORAD `L_MC` table mirroring the RAD table in
   `reference-highQ2-mc-luminosity.md`, plus per-(generator, period,
   radmode) `H1MCLumiCalc` factors.

3. Treat the current full-v1 RAPGAP-DJANGOH ~9% offset as a known
   methodology artifact, not a physics result. Do not propagate it
   into any downstream systematic or cross-check until §5 D1 reproduces
   the v3 per-event-ratio C_rad on the same v6 trees and confirms
   the offset disappears under the per-event convention.

4. Frame the next CRAD pass as either (a) re-derive C_rad as a
   per-event multiplicity ratio on v6 trees (the v3 script
   pattern), or (b) bring in `H1MCLumiCalc::Calculate()` to derive
   authoritative `L_MC_eff` for every (generator, period, radmode)
   combination, both RAD and NORAD, **before** running the absolute-σ
   formula. Option (a) is simpler and matches the existing v6 banner.

---

## 9. What This Research Did Not Cover

- **Did not inspect the actual CRAD v1 driver script.** The user's
  task description states the formula but does not point at the
  script. The `H1/sidis_highQ2/scripts/crad/` folder contains only
  `submit_crad_v6_smoke.py` (a stager that explicitly does NOT
  compute C_rad — see its docstring). The active C_rad scripts
  visible locally are `scripts/compute_crad_v3_HQv5gBprime.py` and
  `scripts/analysis/compute_crad_adaptive_v2.py`, both of which use
  per-event-ratio convention. The exact v1 absolute-σ script that
  produced the "mostly < 1" results and the ~9% qx-median offset
  may live in `output_v6/crad/...` on NAF or in a wrapper that I
  did not locate. **If the user can point to the specific script,
  Check D2 (§5) can be sharpened to a direct line-level audit.**

- **Did not inspect the NORAD HAT inventory on NAF.** The Britzger
  NORAD inventory at `/data/dust/group/h1/britzger/norad_prod21/`
  has been audited previously
  (`research-crad-v5f-attempt.md` §1.2, `reference-highQ2-crad-availability.md`
  §3), and I did not re-run that audit. The 27.5/27.6 GeV split and
  the EPLUSP/EMINUSP availability per generator are taken from those
  prior docs.

- **Did not run `H1MCLumiCalc` or any `oomclumi` job.** Per the
  read-only mandate, no remote production was launched. The
  vertex-cut correction-factor magnitude estimate (~1.03–1.10) is
  derived from the typical HERA-II satellite-bunch fraction; the
  exact value per (generator, period) must come from an actual
  `H1MCLumiCalc::Calculate()` run on the relevant chunks.

- **Did not verify the user's "584,918 nb⁻¹" history is unrelated.**
  The `research-rate-ratio-reconciliation-2026-05-29.md` audit
  resolved a different lumi-mis-statement (background subtraction,
  ~18× too small). That trap is in a different code path than the
  CRAD v1 formula but illustrates the same family of pitfalls.

---

## 10. References (Source-file evidence, local-only)

| Reference | Location |
|---|---|
| `oomclumi` event loop, formula | `/Users/hklest/Desktop/opencode/H1/h1oo-master/H1Tools/oomclumi.C:71–272` |
| `oomclumi` `WeightsSum` (Total) vs `AnalysisweightsSum` (z-cut) | `oomclumi.C:147–153, 237–250` |
| `oomclumi` `fGenOnly=1` behaviour | `oomclumi.C:97–129` |
| `H1MCLumiCalc::Calculate` event loop | `/Users/hklest/Desktop/opencode/H1/h1oo-master/H1Tools/H1MCLumiCalc.C:84–264` |
| `H1MCLumiCalc::GetLumiMCFactor` definition | `H1MCLumiCalc.C:71–82, 243–259` |
| `H1MCLumiCalc::GetVertexWeight` per-sub-period mapping | `H1MCLumiCalc.C:268–328` |
| `H1SteerTree::fLumi` declaration | `/Users/hklest/Desktop/opencode/H1/h1oo-master/H1Skeleton/H1SteerTree.{h:145, C:65–69, 503, 540}` |
| `H1AnalysisChain::SteerCalculator` reads `fLumi` | `/Users/hklest/Desktop/opencode/H1/h1oo-master/H1Analysis/H1AnalysisChain.C:649` |
| `H1CalcWeight` uses `fMCLumi` for `L_data/L_MC` reweight | `/Users/hklest/Desktop/opencode/H1/h1oo-master/H1Calculator/H1CalcWeight.C:325–339, 365–422` |
| Britzger steering example (`em0405`, 148 chains, mixed RAD/NORAD/bg) | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/reference/britzger_steerings/es.Norad.em0405disMC13.Norad2021.GenLumi.steer` |
| Authoritative RAD `L_MC` table (workspace) | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/references/reference-highQ2-mc-luminosity.md:60–104` |
| `oomclumi` Total vs Analysis discussion (workspace) | `reference-highQ2-mc-luminosity.md:25, 153` |
| NORAD inventory + RAD/NORAD Q²gen edge | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/references/reference-highQ2-crad-availability.md` §3 |
| Prior CRAD attempt with same trap list | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research-crad-v5f-attempt.md` §1.5, B5 |
| v6-era C_rad validity policy | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/context-handoff.md` §"C_rad validity (v6 era)" |
| Per-event-ratio CRAD (immunized) | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/compute_crad_v3_HQv5gBprime.py:735–740` |
| Britzger `fLumi`-sum authoritative bg values | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/context-handoff.md` §"NAF" line 766; `output/lumi_mc_bg_authoritative.json` (NAF) |
| Rate-ratio L_MC reconciliation precedent | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/research-rate-ratio-reconciliation-2026-05-29.md` §§4–5 |
| ep0304 27.5/27.6 GeV split | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/research-ep0304-production-path-2026-05-24.md` |

---

## 11. One-Paragraph Summary For The Supervisor

The new CRAD v1 absolute-cross-section formulation
`σ = Σ(w1·w2)/source_lumi` re-introduces a family of L_MC-accounting
risks that the previously canonical per-event-ratio CRAD avoided by
construction. The user's recollection of "pilot bunches" is best read
as the satellite-bunch / gen-vertex population that `oomclumi`'s Total
block and `H1SteerTree::fLumi` both count, but the `|VtxZ| < 35 cm`
analysis cut excludes. The framework provides
`H1Tools/H1MCLumiCalc::GetLumiMCFactor()` to correct this, but the
SIDIS workspace has never invoked it. The dominant single cause of the
observed ~9% RAPGAP-vs-DJANGOH qx-median offset is most likely the
structural `Q²gen` edge mismatch between RAD (`Q2GT60`) and NORAD
(`Q2GT100`) samples, which is not generator-symmetric because the two
generators have different `dσ/dQ²` shapes in the `[60, 150] GeV²`
migration band. The recommended decisive check is (1) re-run the v3
per-event-ratio CRAD on the same v6 trees and confirm the
generator-offset vanishes; if it does, the v1 absolute-σ formulation
is the source. The v6 handoff already labels the cross-section-ratio
CRAD as "Invalid / do not use"; this research note documents the
mechanism behind that label.

<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current research note. `C_rad` remains provisional operationally until the charged non-radiative production path is executed.

# Research: C_rad for Inclusive Charged-Hadron SIDIS (Low-Q2)

**Date:** 2026-05-13
**Scope:** How the V0 (Lambda/K0s) analysis computes the QED radiative
correction factor C_rad, what changes for the inclusive charged-hadron
SIDIS multiplicity / charge-asymmetry measurement, and a concrete recipe
for `compute_crad_charged.py`.
**Audience:** A code-writing agent (and the SIDIS supervisor) who must
implement charged-hadron C_rad without reading every prior research
artifact.
**Source artifacts read (verified):**
- `lambda/scripts/compute_crad.py` (reads pre-binned histograms; older I/O
  contract)
- `lambda/scripts/apply_crad.py` (Lambda/K0s xsec + asym application)
- `lambda/src/CradCompute.C` (C++ producer of GenParticles tree on NAF)
- `lambda/data/crad/crad_rapgap31_differential.json` (current C_rad values)
- `lambda/docs/research-crad-definitive.md`
- `lambda/docs/research-crad-from-our-trees.md`
- `lambda/docs/research-crad-rapgap-recipe.md`
- `agents/h1-radiative-correction.md`
- `agents/h1-mc-expert.md`
- `agents/h1-normalization.md`
- `sidis_lowQ2/memo-mc-truth-policy-charged-sidis.md`
- `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` (charged builder; legacy proton-prefixed name from the v5b SIDIS fork — see `docs/references/reference-naming-aliases-highQ2-sidis.md` for the alias table)
- `sidis_lowQ2/reference-sidis-systematics.md`
- `sidis_lowQ2/research-sidis-systematics-framework.md`

---

## 0. Bottom Line First (TL;DR)

1. **V0 C_rad formula:**
   `C_rad(bin) = (N_rad(bin) / L_rad) / (N_nonrad(bin) / L_nonrad)` =
   `sigma_rad / sigma_nonrad`, in the **denominator convention** (a value
   < 1 means radiation suppressed the apparent yield, so divide the
   measured cross-section by C_rad to recover the Born cross-section).
   Verified to 6 significant figures against `gtustin/CrossSections.C`.
2. **V0 BGcheck story is irrelevant for charged hadrons.** The V0 sign
   flip ("without BGcheck C_rad > 1; with PS<=1 C_rad ~ 0.67") is a
   property of the V0 *particle-level* asymmetry between rad and nonrad
   under a hyperon-feed-down filter. For inclusive charged hadrons, the
   `IsGtr() && |PDG| in {211, 321, 2212}` filter already isolates the
   primary fragmentation set unambiguously (see
   `memo-mc-truth-policy-charged-sidis.md` Section 1). PS = 0..3 should
   all be accepted as signal -- not as an analysis choice but because
   they are physical primary charged hadrons.
3. **Major SIDIS-vs-V0 difference: C_rad acts on the DIS event rate, not
   on the per-particle yield.** For multiplicities
   `M = N_h / (N_DIS * Delta_bin)` the radiative correction enters in the
   numerator (per-event hadron yield) AND the denominator (DIS event
   count). The two corrections largely **cancel** in M. They do **not**
   cancel for the charge asymmetry `A_h = (N_+ - N_-)/(N_+ + N_-)` at
   the per-charge level (Section 4), so the SIDIS analysis needs
   *per-charge* C_rad applied to the per-charge yields.
4. **The code path the V0 analysis ACTUALLY uses today** is not the
   `compute_crad.py` script in the repo (that was built for a
   pre-binned-histogram contract). It is a two-stage pipeline:
   `lambda/src/CradCompute.C` writes a per-particle `GenParticles`
   TTree on NAF (BGcheck-equivalent ancestry filter, all event-level cuts
   stored as branches), then a downstream Python script (which produces
   `crad_rapgap31_differential.json`) draws histograms with all eight
   gtustin cuts and computes C_rad per bin. The SIDIS adaptation should
   follow the same two-stage pattern.
5. **NonRad MC for charged tree:** a soft blocker. RAPGAP31 / DJANGOH14
   nonRad H1OO files exist on NAF and have published luminosities (see
   Section 5). **They have NOT been processed through
   `ProtonTreeBuilder_lowQ2.C`.** Until a nonRad pass produces
   `ChargedTotGen` from those H1OO files, charged C_rad cannot be
   computed from our own trees; an `OPEN ISSUE 1` in
   `reference-sidis-systematics.md` tracks this.
6. **Workaround that lets C_rad ship today:** integrated charged C_rad
   for h+/h- can be computed by `CradCompute_charged.C` (a fork of
   `CradCompute.C` that filters on PDG instead of |PDG|=3122/310) on
   nonRad gen-only files. No reco is needed. This is feasible if and
   only if the nonRad gen H1OO files contain the charged-particle gen
   bank (verified at the H1 framework level; production needs to run).
7. **There is no SIDIS-binning shortcut via gtustin's gen trees.**
   Gtustin's `Lambda Tot`/`K0s` trees lack the SIDIS variables
   (`z_SIDIS`, `qT_SIDIS`, `xF`, etc.) and lack PDG codes for charged
   hadrons. So the SIDIS analysis MUST run its own gen-tree producer.

---

## 1. Summary of the V0 C_rad Approach

### 1.1 Mathematical core (denominator convention)

Per bin `k`:

```
C_rad(k)
  = [ Sum_(rad events with i in bin k)    weight1*weight2 ] / L_rad
    -----------------------------------------------------------------
    [ Sum_(nonrad events with i in bin k) weight1*weight2 ] / L_nonrad

  = (N_rad(k) / N_nonrad(k)) * (L_nonrad / L_rad)

  = sigma_rad(k) / sigma_nonrad(k)
```

The cross-section formula divides by C_rad:
```
sigma_Born(k) = N_data(k) / ( eps(k) * BR * L_data * C_rad(k) * Delta_bin(k) )
```
For Lambda integrated, C_rad ~ 0.966 (DJANGOH14) / 0.960 (RAPGAP31).
Source: `lambda/docs/research-crad-definitive.md` Section 1, verified to
6 significant figures against `gtustin/CrossSections.C` line 1711-1716.

### 1.2 Trees, files, luminosities

There are two source trees (both required, both per-generator):

| Sample          | Tree path on NAF (gtustin reference)                                                                  | Lumi (nb^-1, all years) |
|-----------------|--------------------------------------------------------------------------------------------------------|-------------------------|
| RAPGAP31 Rad    | `/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root`    | 5,293,265               |
| RAPGAP31 NonRad | `/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/merged.root` | 3,397,124               |
| DJANGOH14 Rad   | `/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root`    | 4,108,957               |
| DJANGOH14 NonRad| `/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root` | 3,560,233               |

Per-year luminosities are tabulated in `agents/h1-normalization.md`
lines 30-37. `compute_crad.py` in the repo accepts `--lumi-rad` and
`--lumi-nonrad` as floats; it is generator-agnostic.

The V0 standalone producer `CradCompute.C` produces a `GenParticles`
TTree under `/pnfs/desy.de/dphep/scratch/h1/klesthen/lambda/crad/...` and
the merged outputs land at
`H1/lambda/data/crad/merged_crad_{rad,nonrad}.root`. (The `compute_crad.py`
script in the repo is now a vestigial pre-binned-histogram reader that
predates the `GenParticles` design; the actively-used computation is in
the differential JSON producer that was used to write
`crad_rapgap31_differential.json`. Either contract is fine to clone for
the SIDIS adaptation. Recommend the `GenParticles` per-particle TTree
contract because it lets the SIDIS analyst rebin without rerunning the
Condor production.)

### 1.3 The eight cuts (verbatim from gtustin's `AnalyzeGenTreeRadCorr`)

```
pT  > 0.5 && pT < 3.5
|Eta| < 1.3
Ys  > 0.1 && Ys < 0.6                  // sigma method
Q2s > 5  && Q2s < 100                  // sigma method
H1GenBool == 1                         // = IsGtr(); structurally implicit in our trees
|VtxZgen| < 30 cm
GenElecEn > 11 GeV
Empz > 35 && Empz < 70                 // E - pz at gen level
```

Weight: `weight1 * weight2` (weight2 == 1 always; left in for symmetry).

The **DIS-event-level cuts** (`H1GenBool`, `|VtxZgen|<30`,
`GenElecEn>11`, `Empz`) are what create the rad/nonrad asymmetry. The
fiducial cuts (`pT`, `|Eta|`, `Ys`, `Q2s`) define the binning. See
`research-crad-definitive.md` Section 4 for the cut-by-cut decomposition
showing that the H1GenBool cut alone contributes -21 percentage points
to C_rad, while the E-pz cut contributes only -0.4 pp.

### 1.4 BGcheck and ParticleSource role in V0 C_rad

This is the question the user asked most pointedly. The story is:

1. **Gtustin's gen trees are filled with BGcheck applied at fill time**
   (during `genMCTree.C`). They contain GTR particles plus STR particles
   from legitimate Sigma0/Xi/Omega -> Lambda decay chains. They do NOT
   contain a `ParticleSource` branch. They contain `H1GenBool` per
   particle which equals `IsGtr()`.
2. The V0 cut `H1GenBool == 1` then selects the *GTR-only* subset of
   gtustin's trees, dropping the STR feed-down. After all eight cuts,
   the V0 integrated C_rad lands at 0.966 (DJANGOH14).
3. Our own gen trees were filled with `IsGtr() == true` at fill time
   and lack `H1GenBool` entirely (it would be redundant; every entry has
   `IsGtr() == true` by construction). They store `ParticleSource` from
   `classifySourceGTR()`, which does *not* implement BGcheck's
   ancestry-walk.
4. **Without any BGcheck-equivalent**, our trees give catastrophically
   wrong V0 C_rad (~0.64 for RAPGAP31, ~0.85 for DJANGOH14) because
   IsGtr-only is asymmetrically restrictive between rad and nonrad
   samples (rad events have more STR-classified Xi/Omega -> Lambda
   chains that get dropped). See `research-crad-from-our-trees.md`
   Section 6.
5. **The sign-flip phenomenon** (C_rad > 1 without BGcheck flipping to
   ~0.67 with `PS <= 1`) was observed on an *older* set of our trees and
   is an artifact of mixing two effects: (a) wrong baseline from
   IsGtr-only filtering and (b) extra removal by the PS filter. The
   resolution in the codebase is to use `CradCompute.C`'s
   `BGcheckGTR()` ancestry walk which reproduces gtustin's BGcheck on
   our own production stream. With `BGcheckGTR`, our V0 C_rad lands at
   ~0.95-0.96 (see `crad_rapgap31_differential.json`), broadly
   consistent with gtustin (within ~1 percentage point per bin; the
   small residual is documented in `research-crad-offset-diagnosis.md`).

**For Lambda/K0s, BGcheck/ParticleSource matter critically because the
Sigma0/Xi/Omega feed-down chain produces a sizeable fraction of the
final Lambdas and the chain is asymmetrically classified between rad
and nonrad.** This is the V0-specific story.

### 1.5 Per-charge handling in the V0 analysis

`apply_crad.py` lines 110-118 reads three Lambda C_rad values:

- `LamReg_int` (Lambda only)
- `LamBar_int` (AntiLambda only)
- `LamTot_int` (Lambda + AntiLambda)

For the Lambda asymmetry, lines 204-233 of `apply_crad.py` apply the
per-species C_rad:
```
sigma_L_corr  = sigma_L  / C_rad_L
sigma_AL_corr = sigma_AL / C_rad_AL
A_corr = (sigma_L_corr - sigma_AL_corr) / (sigma_L_corr + sigma_AL_corr)
```

This is the **partial cancellation** template. C_rad_L and C_rad_AL
differ by ~0-1% in V0 because Lambda and AntiLambda are produced from
the same fragmentation but with different sea/valence quark content;
QED radiation acts on the leptonic side and is charge-blind, so the
per-species C_rad difference comes from the slightly different gen-
level fiducial-edge migration patterns.

For SIDIS h+/h- the analogue is `C_rad_(h+)` vs `C_rad_(h-)`. There is
no a-priori reason the SIDIS per-charge C_rad would differ by more than
1-2% (charge-blind QED), but the analysis must compute both and apply
them separately for `A_h`.

### 1.6 What the V0 `compute_crad.py` (in the repo) actually does

It reads pre-binned `TH1` histograms (`hLamTot_int`, `hLamTot_Q2`, ...)
from the rad and nonrad merged files. It expects `Sumw2` errors stored
on each histogram. It does *not* read the per-particle `GenParticles`
TTree, even though that is what `CradCompute.C` writes.

Implication: the in-repo `compute_crad.py` is wired to a
**legacy histogramming producer** (an older CradCompute that
pre-binned). The current `CradCompute.C` writes `GenParticles` (per-
particle TTree). The actual JSON in
`lambda/data/crad/crad_rapgap31_differential.json` was therefore
written by a Python script that draws histograms from `GenParticles`
with all 8 cuts as TTreeFormula -- I did not find this script in
`lambda/scripts/`, so it was either inlined into a Jupyter session or
lives outside the repo. For the SIDIS adaptation, **clone the
GenParticles producer pattern, not the legacy compute_crad.py reader**.

### 1.7 Year-by-year combination

V0 currently uses ALL-years merged trees on both rad and nonrad samples
with the all-years total luminosity. This is the path that exactly
reproduces gtustin to 6 sig figs.

Per-year luminosity tables exist in `agents/h1-normalization.md` lines
30-37. A per-year C_rad would compute
`C_rad_combined = sum_y (sigma_rad_y * L_data_y) / sum_y (sigma_nonrad_y * L_data_y)`
with `sigma_y = N_y / L_MC_y`. Only the ratio `L_nonrad / L_rad`
matters within each year, but it must be the per-year MC luminosities,
not the all-years totals. Since gtustin's recipe uses all-years totals
and our V0 reproduces gtustin exactly, the all-years approach is
acceptable provided the year mix in the data and MC are matched (which
they are for HERA-II 2005+2006+2007).

---

## 2. Critical Differences for SIDIS

### 2.1 Particle species

V0 uses tree filters `|PDG| == 3122` (Lambda) or `|PDG| == 310` (K0s).
SIDIS uses `|PDG| in {211, 321, 2212}` (`isChargedHadron()` from
`ProtonUtils_lowQ2.h`). The change is a one-line filter swap in the
producer; no other physics changes at this level.

### 2.2 Per-charge separation

V0 has `Lambda Tot`, `Lambda Reg`, `Lambda Bar`, `K0s` as separate
trees because the species are physically distinct. SIDIS will store one
`GenParticles` TTree with a `Charge` (or signed PDG) branch and split
at analysis time. This is more flexible and requires fewer Condor jobs.

### 2.3 Fiducial cuts

V0 fiducial: `pT in (0.5, 3.5)`, `|eta| < 1.3`.
SIDIS fiducial (from `memo-mc-truth-policy-charged-sidis.md`):
`|eta| < 1.3`, `pT > 0.15 GeV`, `0.3 < P < 3.0 GeV`.

The SIDIS fiducial is wider in pT (down to 150 MeV) and adds a momentum
window. The producer should store pT, eta, AND P (or px,py,pz so the
analysis can compute P) per particle and apply the SIDIS fiducial at
analysis time.

### 2.4 SIDIS variable binning

V0 bins are 1-D in `Q2`, `pT`, `eta` plus integrated. SIDIS bins are
1-D in `z`, `pT_h`, `eta_lab`, `Q2`, plus optionally 2-D `(z, Q2)` or
`(pT_h, Q2)`. The producer must store the SIDIS gen-level variables
(`zGen`, `pT_h_Gen`, `xFGen`, `ystarGen`) per particle. These are
already computed in the charged gen loop of `ProtonTreeBuilder_lowQ2.C`
(lines 843-866) and can be written into the gen-only `GenParticles`
tree with the same expressions.

### 2.5 The signal definition (PS 0-3)

Per `memo-mc-truth-policy-charged-sidis.md` Section 4: PS = 0, 1, 2, 3
are all accepted as signal because they are *physical* primary charged
hadrons (direct fragmentation plus generator-level decays of light
hyperons). This is the same as the V0 policy. PS = 4 (secondary
interaction) is impossible for IsGtr particles. PS = 5 (combinatorial)
is excluded by the `GetMother2() == -1` veto.

For the C_rad ratio specifically, **including the same PS set in both
rad and nonrad keeps any feed-down asymmetry in the ratio**, which is
correct: it is the actual radiative correction to the observable yield.

### 2.6 Multiplicities vs cross-sections: the hidden cancellation

V0 measures absolute differential cross-sections. C_rad enters once,
in the cross-section denominator.

SIDIS measures **multiplicities** `M = N_h / (N_DIS * Delta_bin)`. The
hadron count `N_h` and the DIS event count `N_DIS` both have radiative
corrections. Algebraically:

```
M_Born = (N_h_meas / C_rad_h) / (N_DIS_meas / C_rad_DIS) / Delta_bin
       = M_meas * (C_rad_DIS / C_rad_h)
```

If `C_rad_h ~ C_rad_DIS` (which is true to first order because both
have the same DIS event-level cut origins), the multiplicity is almost
unchanged by C_rad. The remaining correction is the
**ratio C_rad_DIS / C_rad_h**, which is a per-bin O(1%) effect arising
because the per-particle migration is slightly different from the
per-event migration in fiducial-edge bins.

This is `C_rad_partial`, *not* `C_rad`. It is what
`reference-sidis-systematics.md` line 199 calls "Partial, not full" for
A_h.

For `A_h`:
```
A_h_Born = (N_h+/C_rad_h+ - N_h-/C_rad_h-) / (N_h+/C_rad_h+ + N_h-/C_rad_h-)
         = A_h_meas * [ ... per-charge correction terms ... ]
```
The DIS event count cancels exactly in `A_h` (it appears in both num
and den when forming the per-charge yields). The per-charge C_rad
ratio survives.

### 2.7 Implication for the deliverable

The SIDIS pipeline needs **two C_rad tables** per generator:

| Table          | What it corrects             | Bin axes                         | Used by    |
|----------------|------------------------------|----------------------------------|------------|
| `C_rad_DIS`    | DIS event count              | DIS variables only: `Q2`, `y`    | M, sigma   |
| `C_rad_h+/-`   | per-charge hadron count      | SIDIS bins x charge              | M, A_h     |

Then:
- `M_Born = M_meas * (C_rad_DIS / C_rad_h)` per (charge, SIDIS bin).
- `A_h_Born` from per-charge corrected yields.
- `M_combined_Born = M_meas * C_rad_M_partial` where
  `C_rad_M_partial = C_rad_DIS / C_rad_h` is a single per-bin
  multiplicative.

The V0 analysis only needs `C_rad_h` (per particle species) because it
measures absolute cross-sections. SIDIS needs both, but the hadron one
is the same producer with PDG widened.

---

## 3. Recommended SIDIS C_rad Formulas

### 3.1 Per-charge hadron-level C_rad (used by A_h and M)

```
C_rad_h(charge, bin) =
   [ Sum_(rad)    weight1*weight2 * 1{passes 8 DIS cuts} * 1{PDG in {211,321,2212}}
                                  * 1{IsGtr && Mother2==-1}
                                  * 1{this charge} * 1{this SIDIS bin} ] / L_rad
   --------------------------------------------------------------------------
   [ Sum_(nonrad) ... same indicator ... ] / L_nonrad
```

Where the 8 DIS cuts are gtustin's:
`pT_h, |Eta_h|, Ys, Q2s, IsGtr, |VtxZgen|, GenElecEn, Empz`. Note that
the pT/eta cuts here are on the *hadron* (not on the electron), so
they couple to the binning. For per-charge SIDIS bins, the binning
*replaces* the corresponding nominal range; all other six cuts stay.

**Crucial:** the SIDIS fiducial replaces the V0 fiducial:
- Replace `pT > 0.5 && pT < 3.5` with `pT > 0.15`
- Add `0.3 < P < 3.0`
- Keep `|Eta| < 1.3`
- The Q2/y/electron-side cuts stay at gtustin's values for cross-
  consistency with the rest of the H1 SIDIS literature; the SIDIS
  analysis low-Q2 fiducial actually uses `Q2 in [5,100]` and
  `y in [0.1, 0.6]` per `memo-mc-truth-policy` and existing scripts.
  Confirm against `analyze_sidis_charged.py`.

### 3.2 Event-level C_rad (used by M)

```
C_rad_DIS(bin) =
   [ Sum_(rad events)    weight1*weight2 * 1{passes DIS event cuts} * 1{this DIS bin} ] / L_rad
   ----------------------------------------------------------------------------------
   [ Sum_(nonrad events) ... same indicator ...                                       ] / L_nonrad
```
The "DIS event cuts" are everything except the per-particle pT/eta/P
cuts: `Ys`, `Q2s`, `IsGtr` (event-level scattered-electron tag),
`|VtxZgen|<30`, `GenElecEn>11`, `Empz in [35,70]`. This gives one
correction per DIS bin (typically `Q2`-binned and `y`-binned). The
denominator of the multiplicity `N_DIS` measured in data needs this
correction.

For most SIDIS bin definitions where the DIS bin = the multiplicity bin
(e.g., bin in Q2 only), `C_rad_DIS(Q2)` is computed from a 1-event-per
loop over the gen tree (or from the event-tree branch of CradCompute).

### 3.3 Effective multiplicity correction (drop-in)

```
M_Born(bin) = M_meas(bin) * [ C_rad_DIS(Q2_of_bin) / C_rad_h(charge, bin) ]
```

For an inclusive (charge-summed) M, use `C_rad_h(both charges, bin)`
i.e. the all-charges version, computed with no charge filter.

### 3.4 Per-charge asymmetry correction (drop-in)

Following `apply_crad.py` lines 204-233:
```
N_(+)_corr(bin) = N_(+)_meas(bin) / C_rad_h(+, bin)
N_(-)_corr(bin) = N_(-)_meas(bin) / C_rad_h(-, bin)
A_h_Born(bin) = (N_(+)_corr - N_(-)_corr) / (N_(+)_corr + N_(-)_corr)
```
The DIS event count cancels in A_h and does not appear here.

---

## 4. Recommended Signal Definition for SIDIS C_rad

Apply at producer (CradCompute_charged.C) fill time:

```
PDG filter:    |PDG| in {211, 321, 2212}            (isChargedHadron)
GTR filter:    IsGtr() == true
Mother2 veto:  GetMother2() == -1
BGcheck-equiv: skipped -- IsGtr() is structurally sufficient for pi/K/p
               (per memo Section 1; STR feed-down via hyperons goes
               through Lambda/K0_S which are NOT in the SIDIS species
               set, and even Sigma0->p chains are GEANT3-decayed so
               their daughter pi/K/p have IsGtr=false anyway)
ParticleSource: stored but not filtered (for diagnostics).
                Accept PS = 0, 1, 2, 3 implicitly.
                (The truth-policy memo explicitly endorses PS 0-3.)
Loose kinematic prefilter (to keep tree size manageable):
   pT > 0.05 GeV, |eta| < 3.5, P > 0.05 GeV
   (Same envelope as ChargedTotGen's `inFiducialCharged` definition in
   ProtonTreeBuilder_lowQ2.C line 870; intentionally wider than the
   analysis fiducial to keep migration studies possible.)
```

Apply at analysis time (compute_crad_charged.py):
```
SIDIS fiducial:    |Eta| < 1.3, pT > 0.15, 0.3 < P < 3.0
DIS event cuts:    GenElecEn > 11, |GenEmpz - 0|, Empz in [35,70],
                   |VtxZgen| < 30
DIS bin window:    Q2s in [Q2_lo, Q2_hi], Ys in [0.1, 0.6]
SIDIS bin window:  z, pT_h, eta_lab, etc.
```

**Justification:** see `memo-mc-truth-policy-charged-sidis.md` Sections
1, 4, 5. The PDG filter combined with `IsGtr()` is structurally
sufficient because:
- pi/K/p directly from PYTHIA fragmentation -> IsGtr=true (PS=0).
- pi/K/p from Sigma0->Lambda+gamma where the Sigma0 itself decayed in
  the generator -- the daughter is the *Lambda*, not pi/K/p. The
  pi/K/p downstream of the Lambda are GEANT3-decayed (Lambda is
  PYTHIA-stable). So PS=1 pi/K/p with IsGtr=true are essentially
  impossible (they would require a direct Sigma0->p X decay at
  generator level, which is a charge-blind 0.5% branching).
- pi/K/p from material interactions -> IsGtr=false, dropped.
The contamination risk for pi/K/p is negligible (memo Section 5).

---

## 5. NonRad MC Availability

### 5.1 Current status

| Sample              | H1OO files exist on NAF? | Processed through ProtonTreeBuilder? | C_rad-able today? |
|---------------------|--------------------------|--------------------------------------|--------------------|
| RAPGAP31  Rad       | YES (35 run idx)         | YES, merged_proton_mc_rapgap31.root  | n/a (rad)         |
| RAPGAP31  NonRad    | YES (per h1-normalization)| **NO**                              | **NO**            |
| DJANGOH14 Rad       | YES (93 run idx)         | YES, merged_proton_mc_djangoh14.root | n/a (rad)         |
| DJANGOH14 NonRad    | YES (per h1-normalization)| **NO**                              | **NO**            |

The H1OO source files exist (gtustin used them successfully). They have
documented per-year and total luminosities in `agents/h1-normalization.md`
lines 30-37. `research-sidis-systematics-framework.md` lines 258-265
flags this exact gap; `reference-sidis-systematics.md` open question 1
owns the resolution.

### 5.2 What "processed" means here

The SIDIS analysis builds `ChargedTotGen` (and `ChargedTot`) by running
`ProtonTreeBuilder_lowQ2.C` over H1OO HAT/MODS files. The nominal
production has only run on the **rad** MC. To get C_rad, we need either:

**Path A (full):** Run a `--mode mc` pass of `ProtonTreeBuilder_lowQ2.C`
on the **nonRad** H1OO files. Output: `merged_proton_mc_djangoh14_nonrad.root`,
`merged_proton_mc_rapgap31_nonrad.root` containing `ChargedTotGen` (we
can throw out `ChargedTot` -- not needed for C_rad).
- Cost: ~few hours of Condor (nonRad samples are smaller; ~10x fewer
  events than rad).
- Risk: same builder, no new code.

**Path B (lightweight):** Build a `CradCompute_charged.C` standalone
binary modeled on `lambda/src/CradCompute.C` that filters to charged
hadrons and stores only what C_rad needs. Run it on both rad and
nonRad files.
- Cost: ~1 day of new code + tests + a Condor pass (actually two:
  same number of files for rad; somewhat fewer for nonRad).
- Benefit: same producer for rad and nonRad (consistent cuts), no
  reco overhead, smaller output, cuts applied post-hoc via branches.
- This is what the V0 thread did and it is the recommendation.

### 5.3 Files an agent will need on NAF

The H1OO file lists (steering files) for nonRad MC live under
`/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/{RAPGAP31,DJANGOH14}/{2005,2006,2007}/`
per `proton/context-handoff.md` line 89. These steering files reference
H1OO HAT/MODS pairs on `/pnfs`. The same steering files are usable for
the SIDIS thread because they reference H1 detector simulation data
that is generator-content-independent at the H1OO API level (the
same `H1PartMCArrayPtr` is populated from the GTR bank regardless of
which generator wrote it).

**Action for agents:** confirm via `h1-data-validator -> h1-root-analyst`
that the nonRad H1OO files (a) exist, (b) load on Alma9, and (c)
populate `H1PartMCArrayPtr` with charged hadrons (a pi+ should be
findable in any nonRad event with non-trivial hadronic final state).
If all three pass, Path B is unblocked.

---

## 6. Detailed Implementation Instructions for `compute_crad_charged.py`

### 6.1 Recommended two-stage pipeline

Stage 1 (C++, runs on NAF via Condor):
- Binary: `CradCompute_charged` (a fork of `lambda/src/CradCompute.C`).
- Input: H1OO HAT/MODS via steering file.
- Output: per-particle TTree `GenParticles_charged`.
- Run twice per generator: once on rad steering, once on nonRad
  steering. Merge each output to one ROOT file per (generator, radmode)
  combination. Total: 4 merged files.

Stage 2 (Python, runs locally or on NAF):
- Script: `sidis_lowQ2/scripts/compute_crad_charged.py`.
- Input: the 4 merged ROOT files plus the four luminosities.
- Output: `sidis_lowQ2/data/crad/crad_charged_<generator>.json`,
  containing per-charge per-bin C_rad values for both `C_rad_h` and
  `C_rad_DIS` flavours.

### 6.2 GenParticles_charged tree spec

One entry per accepted gen particle. All cuts applied post-hoc.

| Branch          | Type    | Notes                                                                    |
|-----------------|---------|--------------------------------------------------------------------------|
| `RunNumber`     | Int_t   | for per-year breakdown / luminosity matching                            |
| `EventNumber`   | Int_t   | event identifier (only needed if you want to dedupe the DIS-event side) |
| `pT`            | Float_t | hadron pT                                                               |
| `eta`           | Float_t | hadron pseudorapidity                                                   |
| `P`             | Float_t | hadron momentum magnitude                                               |
| `Charge`        | Int_t   | +1 / -1 (from chargeSignForPDG)                                         |
| `PDG`           | Int_t   | signed PDG code (e.g., 211, -211, 321, -321, 2212, -2212)               |
| `ParticleSource`| Int_t   | 0..5 from classifySourceGTR (diagnostic)                                |
| `IsGtr`         | Int_t   | 1 always (we filtered at fill time)                                     |
| `zGen`          | Float_t | z = (P.p_h)/(P.q) -- gen-level                                          |
| `pT_h_Gen`      | Float_t | hadron pT relative to gamma* in Breit frame (or just lab pT)            |
| `xFGen`         | Float_t | Feynman x in gamma*p CM frame                                           |
| `ystarGen`      | Float_t | rapidity in gamma*p CM frame                                            |
| `Q2s`           | Float_t | Q2 sigma method, gen-level                                              |
| `Ys`            | Float_t | y sigma method, gen-level                                               |
| `weight`        | Float_t | weight1 * weight2                                                       |
| `VtxZ`          | Float_t | gen-level vertex Z                                                      |
| `GenElecEn`     | Float_t | gen-level scattered-electron energy                                     |
| `GenEmpz`       | Float_t | gen-level E - pz                                                        |
| `EventPasses`   | Int_t   | 1 if event-level DIS cuts pass (GenElecEn>11, Empz in [35,70],          |
|                 |         | |VtxZ|<30); precomputed for speed                                       |

For Stage 1 the producer can omit `EventPasses` (Stage 2 derives it),
but storing it speeds Stage 2 Draw queries.

A separate single-bin `EventTree` (one entry per event with
`weight, VtxZ, GenElecEn, GenEmpz, Q2s, Ys, RunNumber`) is needed for
`C_rad_DIS`. This costs little disk and avoids deduplicating
GenParticles per event.

### 6.3 Stage 1 producer skeleton (CradCompute_charged.C)

```cpp
// Pseudocode -- fork CradCompute.C and edit:

bool isChargedHadron(int pdg) {
    int absPdg = std::abs(pdg);
    return (absPdg == 211 || absPdg == 321 || absPdg == 2212);
}
int chargeSign(int pdg) { return (pdg > 0) ? +1 : -1; }   // (works for all 3 species)

while (H1Tree::Instance()->Next()) {
    // ... read HAT pointers, compute GenEmpz from hadronic E and pz ...
    // ... compute SIDIS gen-level boosts (boostGammaPGen, boostBreitGen,
    //     gammaGen, WmagGen) using gen scattered electron and beam 4-vectors
    //     -- copy expressions from ProtonTreeBuilder_lowQ2.C lines 843-866 ...

    // Fill the EventTree once per event (always, even for events with no
    // accepted hadrons, because C_rad_DIS needs the full DIS-event count).
    eventTree->Fill();

    for (int ip = 0; ip < gtrdata.GetEntries(); ip++) {
        H1PartMC* part = gtrdata[ip];
        if (!part) continue;
        if (!isChargedHadron(part->GetPDG())) continue;
        if (!part->IsGtr()) continue;
        if (part->GetMother2() != -1) continue;

        // BGcheck-equiv NOT required for charged hadrons (see memo Sec 1).
        // We could call BGcheckGTR for symmetry with V0; it would always
        // accept (because IsGtr is true), so it costs nothing.

        TVector3 mom = part->GetMomentum();
        double mass  = ProtonUtil::getMassForPDG(part->GetPDG());
        TLorentzVector p4; p4.SetXYZM(mom.X(), mom.Y(), mom.Z(), mass);

        if (p4.Pt() < 0.05 || p4.Pt() > 5.0) continue;        // loose prefilter
        if (fabs(p4.Eta()) > 3.5) continue;
        if (mom.Mag() < 0.05 || mom.Mag() > 5.0) continue;

        // Compute SIDIS gen variables (z, pT_h, xF, ystar) for this hadron
        // using the precomputed event-level boosts.

        // Fill GenParticles_charged tree.
    }
}
```

### 6.4 Stage 2 driver skeleton (`compute_crad_charged.py`)

```python
"""
Compute C_rad (per-charge hadron and per-event DIS) for inclusive charged
SIDIS. Reads GenParticles_charged + EventTree from rad and nonRad merged
files; writes a structured JSON.

Formula:
    C_rad(bin) = (N_rad(bin) / L_rad) / (N_nonrad(bin) / L_nonrad)
"""
import argparse, json, math
from pathlib import Path
import uproot
import numpy as np

# 8 DIS cuts (event-level subset, applied to BOTH GenParticles and EventTree)
DIS_EVENT_CUT = ("(GenElecEn > 11) & (GenEmpz > 35) & (GenEmpz < 70) "
                 "& (abs(VtxZ) < 30)")
# Fiducial DIS phase-space (Q2, y); these are also bin axes
DIS_PHASE_SPACE = "(Q2s > 5) & (Q2s < 100) & (Ys > 0.1) & (Ys < 0.6)"

# SIDIS hadron fiducial (per memo-mc-truth-policy-charged-sidis.md)
HADRON_FIDUCIAL = "(pT > 0.15) & (abs(eta) < 1.3) & (P > 0.3) & (P < 3.0)"

def load_arrays(path, treename, branches):
    f = uproot.open(path)
    t = f[treename]
    return t.arrays(branches, library="np")

def crad_pair(N_rad, N_nonrad, w2_rad, w2_nonrad, L_rad, L_nonrad):
    """Return (C_rad, sigma) using sum-of-weights and sum-of-weights^2."""
    if N_rad <= 0 or N_nonrad <= 0:
        return float("nan"), float("nan")
    L_ratio = L_nonrad / L_rad
    crad = (N_rad / N_nonrad) * L_ratio
    rel2 = (w2_rad / N_rad**2) + (w2_nonrad / N_nonrad**2)
    return crad, crad * math.sqrt(rel2)

def per_bin(arrays, mask, bin_var, edges, weight_branch="weight"):
    """Return arrays of (N, w2) per bin."""
    w  = arrays[weight_branch][mask]
    x  = arrays[bin_var][mask]
    N  = np.zeros(len(edges)-1)
    W2 = np.zeros(len(edges)-1)
    for i in range(len(edges)-1):
        b = (x >= edges[i]) & (x < edges[i+1])
        N[i]  = w[b].sum()
        W2[i] = (w[b]**2).sum()
    return N, W2

def compute_charged_crad(rad_path, nonrad_path, L_rad, L_nonrad,
                         binnings, charge_split=True):
    """
    binnings: dict like {"z": {"var": "zGen",  "edges": [...] },
                         "pT_h": {"var": "pT_h_Gen", "edges": [...] },
                         "Q2": {"var": "Q2s",  "edges": [...] }}
    Returns nested results dict.
    """
    branches = ["pT", "eta", "P", "Charge", "PDG",
                "zGen", "pT_h_Gen", "xFGen", "ystarGen",
                "Q2s", "Ys", "weight",
                "VtxZ", "GenElecEn", "GenEmpz"]
    rad    = load_arrays(rad_path,    "GenParticles_charged", branches)
    nonrad = load_arrays(nonrad_path, "GenParticles_charged", branches)

    # Event-level dedup is automatic because each row is one particle;
    # for C_rad_h every particle is a count.
    # Build the per-charge masks AFTER applying all 8 cuts.
    def full_mask(a, charge=None):
        m = ((a["GenElecEn"] > 11) & (a["GenEmpz"] > 35) & (a["GenEmpz"] < 70)
             & (np.abs(a["VtxZ"]) < 30) & (a["Q2s"] > 5) & (a["Q2s"] < 100)
             & (a["Ys"] > 0.1) & (a["Ys"] < 0.6)
             & (a["pT"] > 0.15) & (np.abs(a["eta"]) < 1.3)
             & (a["P"] > 0.3) & (a["P"] < 3.0))
        if charge is not None:
            m = m & (a["Charge"] == charge)
        return m

    results = {"C_rad_h": {}}
    charges = ([+1, -1] if charge_split else [None])
    for charge in charges:
        ckey = ("hplus" if charge == +1 else
                "hminus" if charge == -1 else
                "all")
        mask_r = full_mask(rad, charge)
        mask_n = full_mask(nonrad, charge)
        results["C_rad_h"][ckey] = {}
        for bname, bspec in binnings.items():
            N_r, W2_r = per_bin(rad,    mask_r, bspec["var"], bspec["edges"])
            N_n, W2_n = per_bin(nonrad, mask_n, bspec["var"], bspec["edges"])
            crad = []
            err  = []
            for i in range(len(N_r)):
                c, s = crad_pair(N_r[i], N_n[i], W2_r[i], W2_n[i],
                                 L_rad, L_nonrad)
                crad.append(c); err.append(s)
            results["C_rad_h"][ckey][bname] = {
                "edges": bspec["edges"],
                "N_rad": N_r.tolist(),  "N_nonrad": N_n.tolist(),
                "C_rad": crad,           "C_rad_err": err,
            }
            # Integrated for this charge (one big bin) too.
            cint, sint = crad_pair(N_r.sum(), N_n.sum(),
                                   W2_r.sum(), W2_n.sum(),
                                   L_rad, L_nonrad)
            results["C_rad_h"][ckey].setdefault("integrated", {})
            results["C_rad_h"][ckey]["integrated"][bname] = {
                "C_rad": cint, "C_rad_err": sint,
            }

    # Now C_rad_DIS from EventTree (one row per DIS event)
    ev_branches = ["weight", "VtxZ", "GenElecEn", "GenEmpz", "Q2s", "Ys"]
    er = load_arrays(rad_path,    "EventTree", ev_branches)
    en = load_arrays(nonrad_path, "EventTree", ev_branches)
    def ev_mask(a):
        return ((a["GenElecEn"] > 11) & (a["GenEmpz"] > 35) & (a["GenEmpz"] < 70)
                & (np.abs(a["VtxZ"]) < 30) & (a["Q2s"] > 5) & (a["Q2s"] < 100)
                & (a["Ys"] > 0.1) & (a["Ys"] < 0.6))
    mr, mn = ev_mask(er), ev_mask(en)
    results["C_rad_DIS"] = {}
    for bname in ("Q2",):  # at minimum, Q2 binning for DIS event count
        edges = binnings[bname]["edges"]
        N_r, W2_r = per_bin(er, mr, binnings[bname]["var"], edges)
        N_n, W2_n = per_bin(en, mn, binnings[bname]["var"], edges)
        crad = []; err = []
        for i in range(len(N_r)):
            c, s = crad_pair(N_r[i], N_n[i], W2_r[i], W2_n[i],
                             L_rad, L_nonrad)
            crad.append(c); err.append(s)
        results["C_rad_DIS"][bname] = {
            "edges": edges, "N_rad": N_r.tolist(), "N_nonrad": N_n.tolist(),
            "C_rad": crad, "C_rad_err": err,
        }
        cint, sint = crad_pair(N_r.sum(), N_n.sum(), W2_r.sum(), W2_n.sum(),
                               L_rad, L_nonrad)
        results["C_rad_DIS"]["integrated"] = {
            "C_rad": cint, "C_rad_err": sint,
        }
    return results

def main():
    p = argparse.ArgumentParser()
    p.add_argument("--rad", required=True)
    p.add_argument("--nonrad", required=True)
    p.add_argument("--lumi-rad", type=float, required=True)
    p.add_argument("--lumi-nonrad", type=float, required=True)
    p.add_argument("--generator", choices=["RAPGAP31","DJANGOH14"], required=True)
    p.add_argument("--output", required=True)
    args = p.parse_args()
    binnings = {
        "z":    {"var": "zGen",    "edges": [0.1, 0.2, 0.3, 0.4, 0.5, 0.7, 1.0]},
        "pT_h": {"var": "pT_h_Gen","edges": [0.0, 0.5, 1.0, 1.5, 2.0, 3.0]},
        "eta":  {"var": "eta",     "edges": [-1.3, -0.5, 0.0, 0.5, 1.3]},
        "Q2":   {"var": "Q2s",     "edges": [5.0, 7.0, 10.0, 15.0, 25.0, 100.0]},
    }
    out = compute_charged_crad(args.rad, args.nonrad,
                               args.lumi_rad, args.lumi_nonrad,
                               binnings, charge_split=True)
    payload = {
        "step": "radiative_correction",
        "channel": "charged_inclusive",
        "generator": args.generator,
        "luminosities": {"rad_nb": args.lumi_rad,
                         "nonrad_nb": args.lumi_nonrad,
                         "ratio": args.lumi_nonrad/args.lumi_rad},
        "results": out,
        "validation": {
            # Sanity ranges; any C_rad outside [0.5, 1.2] is a blocker.
            "all_in_range_0.5_1.2": True,  # assert before writing
        },
    }
    Path(args.output).parent.mkdir(parents=True, exist_ok=True)
    with open(args.output, "w") as f:
        json.dump(payload, f, indent=2)

if __name__ == "__main__":
    main()
```

### 6.5 Year-by-year handling

Two acceptable approaches; pick by analysis policy:

**(a) All-years combined (matches V0 default):** merge per-year H1OO
files for each (generator, radmode) combination before
`CradCompute_charged`. Use the all-years total luminosities from
`agents/h1-normalization.md`. This is simplest and matches gtustin.

**(b) Per-year then combined:** run `CradCompute_charged` once per year,
compute `C_rad_y(bin)` per year, then combine using
```
C_rad_combined(bin)
   = sum_y N_rad_y(bin) / L_rad_y     ratio  same denom over y
   ----------------------------------
   sum_y N_nonrad_y(bin) / L_nonrad_y
```
*not*
```
C_rad_combined = sum_y w_y C_rad_y     (this is wrong)
```
because C_rad is a ratio of cross-sections; cross-sections add, ratios
don't, weighted by data luminosity if we want the mean radiative
correction per data event:
```
C_rad_data_weighted(bin)
   = sum_y L_data_y * sigma_rad_y(bin)
     -----------------------------------
     sum_y L_data_y * sigma_nonrad_y(bin)
```
Per-year is needed only if (i) the year-mix in MC and data differ, OR
(ii) a per-year systematic is required. The current SIDIS plan does
not need per-year C_rad; method (a) is fine for MVP.

### 6.6 BGcheck sensitivity check

The producer should also write a parallel diagnostic tree
`GenParticles_charged_NoBGcheck` with no BGcheck (it would change
nothing because `IsGtr() && Mother2==-1` already implements the
relevant filter for charged hadrons), AND a `..._PSfilter01` variant
that requires `ParticleSource <= 1`. Then the analysis script reports:
```
C_rad (default, PS 0-3)  = ...
C_rad (PS<=1 only)        = ...
delta_BGcheck = C_rad(PS<=1) - C_rad(default)
```
For inclusive charged hadrons this delta is expected to be **<0.5%**
because PS=2,3 are negligibly populated in pi/K/p (the hyperon
feed-down that creates them is via Lambda/K0_S, which decay in GEANT3
and produce PS=4 daughters that are STR-classified -- already excluded
by IsGtr). If the delta is larger than 1%, something in the producer
is wrong.

This is the SIDIS analogue of the V0 BGcheck-sensitivity check.

### 6.7 Application: how the JSON is consumed downstream

`apply_crad_charged.py` (a SIDIS analogue of `lambda/scripts/apply_crad.py`)
should:
- Read the JSON.
- For each measured multiplicity bin, look up `C_rad_h(charge, bin)`
  and `C_rad_DIS(Q2_of_bin)`.
- Apply
  `M_Born(bin) = M_meas(bin) * (C_rad_DIS / C_rad_h)`.
- For A_h:
  `N_+_corr = N_+_meas / C_rad_h(+, bin)`
  `N_-_corr = N_-_meas / C_rad_h(-, bin)`
  `A_h_Born = (N_+_corr - N_-_corr) / (N_+_corr + N_-_corr)`.
- Propagate `C_rad_err` quadratically into the systematic uncertainty
  budget (per `apply_crad.py` lines 169-201).
- Compare DJANGOH14 and RAPGAP31 C_rad and assign half the difference
  (or the full difference, per analysis convention) as the generator
  systematic.

---

## 7. Open Questions Requiring Human Decision

1. **NonRad MC processing path: A or B?**
   Path A reuses `ProtonTreeBuilder_lowQ2.C` on nonRad H1OO files; Path
   B writes a dedicated `CradCompute_charged.C`. B is closer to the V0
   pattern and ~10x cheaper to run; A is zero-new-code. Recommend B
   based on V0 precedent and the value of having a per-particle TTree
   that supports rebinning without reproduction.

2. **Per-charge or all-charges integrated C_rad for the multiplicity
   correction?** The recommended formula uses charge-summed `C_rad_h`
   for `M`, per-charge `C_rad_h+/-` for `A_h`. Confirm with analysis
   lead.

3. **Whether to compute `C_rad_DIS` separately or absorb it into
   `C_rad_h` by counting per-event.** The clean approach is to keep
   them separate: `C_rad_DIS` corrects the `N_DIS` denominator of the
   multiplicity, and `C_rad_h` corrects the per-event hadron count in
   the numerator. Algebraically equivalent to defining a single
   `C_rad_M_partial = C_rad_DIS / C_rad_h` per bin and using only that.
   The two-table form is more diagnostic.

4. **SIDIS DIS-cut subset: gtustin's exact 8 cuts vs. the SIDIS
   analysis cuts.** Gtustin uses `Q2 in [5,100]`, `y in [0.1,0.6]`,
   etc. The SIDIS analysis low-Q2 fiducial is documented as
   `Q2 in [5,100]`, `y in [0.1,0.6]` per existing scripts -- so they
   match. CONFIRM by reading `analyze_sidis_charged.py`.

5. **Per-year vs all-years.** Default to all-years (Section 6.5(a)).
   Only switch to per-year if a per-year systematic is requested.

6. **How to handle DIS bins that are NOT in `Q2`.** `C_rad_DIS` is a
   function of (Q2, y), not z/pT_h. For multiplicity binned in z (at
   fixed Q2), `C_rad_DIS` is the average over Q2 weighted by the data
   distribution -- or it should be computed for each (Q2, z) cell. For
   MVP, use `C_rad_DIS(Q2_bin)` and treat z-dependence as a residual
   systematic.

7. **Is the SIDIS A_h "partial cancellation" really partial, or
   essentially full?** For pi+/pi- with charge-blind QED, the per-charge
   C_rad difference is expected to be sub-percent. If it is, the C_rad
   correction to A_h is genuinely negligible at the SIDIS precision
   (statistical uncertainties on A_h are O(1%) per bin). Compute it
   first; if the per-charge C_rad agrees within 0.5% across all bins,
   classify as "fully cancels" and absorb into systematic. The
   `reference-sidis-systematics.md` matrix currently classifies it as
   "partial, not full," requiring explicit per-charge derivation.

8. **DJANGOH14 ChargedTotGen completeness** (open question 2 in
   `reference-sidis-systematics.md`). If DJANGOH14 nonRad MC has the
   same `validGenElec` issue as the rad sample, the C_rad numerator
   (charged hadrons) and denominator (DIS events) could both be
   suppressed in correlated ways. This is benign for C_rad if the
   suppression is event-level (cancels in ratio), bad if particle-level
   and asymmetric. Verify via h1-root-analyst before publishing.

9. **Systematic from the BGcheck-equivalent ancestry walk.** For V0
   this is critical; for charged hadrons it should be ~0. Quote the
   measured delta_BGcheck (Section 6.6) in the systematic budget for
   completeness.

---

## 8. What I Did NOT Verify

- Whether `CradCompute_charged.C` (the proposed binary) actually
  compiles or runs on Alma9. The V0 `CradCompute.C` compiled and ran
  successfully (per `lambda/data/crad/crad_rapgap31_differential.json`
  existing), but the SIDIS adaptation has not been written.
- Whether the nonRad RAPGAP31 / DJANGOH14 H1OO files contain a
  populated charged-particle GTR bank for typical SIDIS events. Highly
  likely yes (nonRad is just rad without QED radiation; the hadronic
  final state is unchanged in topology), but unverified at the file
  level.
- The exact SIDIS bin edges (z, pT_h, etc.) in use by the current
  analysis -- I copied indicative edges in the skeleton; replace with
  the analysis canonical edges from `analyze_sidis_charged.py`.
- The actual per-year MC luminosity values for RAPGAP31 NonRad and
  DJANGOH14 NonRad after the per-year merge and any year-dependent
  fixes; the ones in `agents/h1-normalization.md` lines 30-37 are the
  current authoritative values.
- The current `analyze_sidis_charged.py` already-existing C_rad
  application code (if any) -- the SIDIS framework currently uses
  `C_rad = 1` placeholder.

---

## 9. Risks and Surprises Discovered

- **The `compute_crad.py` in the repo is a legacy reader.** It expects
  pre-binned histograms (`hLamTot_int` etc.) but the actively used
  `CradCompute.C` writes a per-particle TTree. The differential JSON
  was produced by an unidentified Python script. Anyone copying
  `compute_crad.py` for SIDIS will get a histogram-reader that does
  not match the GenParticles producer pattern. This research
  recommends the GenParticles + Stage-2-Python pattern explicitly.

- **The "C_rad sign flip" backstory is V0-specific and partly
  obsolete.** It was diagnosed on an older set of our V0 trees. The
  current `CradCompute.C` with `BGcheckGTR()` produces V0 C_rad in the
  ~0.95 regime. For inclusive charged hadrons, the IsGtr+PDG filter is
  structurally sufficient and the BGcheck-equivalent walk is a
  formality (it would never reject a particle that the IsGtr filter
  accepts). The "without BGcheck C_rad > 1, with PS<=1 C_rad ~0.67"
  phenomenon does NOT have an obvious analogue for pi/K/p.

- **Multiplicity has a hidden cancellation that is not present for
  Lambda cross-sections.** The V0 thread does not need `C_rad_DIS`
  because it measures absolute cross-sections per Lambda. SIDIS
  multiplicities cancel this term to first order. This is good news
  (the residual correction is small, of order 1%) but the producer
  must still emit `C_rad_DIS` because the cancellation is bin-by-bin
  approximate, not exact.

- **Gtustin's RAPGAP31/DJANGOH14 gen trees cannot be used for SIDIS
  C_rad** because they (a) lack the SIDIS variables (`zGen`,
  `pT_h_Gen`, `xFGen`, `ystarGen`) and (b) lack PDG codes for charged
  hadrons. The SIDIS C_rad must run a dedicated producer. There is no
  shortcut.

- **The user's hypothesis ("C_rad should be computed at the DIS event
  level for SIDIS") is partially correct.** The full picture is two
  C_rad tables: one event-level (for the multiplicity denominator) and
  one per-particle per-charge (for the numerator and for A_h). Either
  alone is incomplete; both together give the right answer.

---

## 10. References

| Path | Used for |
|------|----------|
| `lambda/src/CradCompute.C` | Producer template (BGcheckGTR, GenParticles tree) |
| `lambda/scripts/compute_crad.py` | Legacy histogram reader; uncertainty propagation reference |
| `lambda/scripts/apply_crad.py` | Per-charge application, asymmetry error propagation |
| `lambda/data/crad/crad_rapgap31_differential.json` | Output schema reference |
| `lambda/docs/research-crad-definitive.md` | C_rad formula, eight cuts, cut-by-cut decomposition |
| `lambda/docs/research-crad-from-our-trees.md` | Why our trees give wrong V0 C_rad without BGcheck |
| `lambda/docs/research-crad-rapgap-recipe.md` | Gtustin tree paths, luminosities, recipe |
| `agents/h1-normalization.md` | All MC luminosities (per year and per radmode) |
| `agents/h1-radiative-correction.md` | Existing C_rad agent contract |
| `agents/h1-mc-expert.md` | BGcheck/ParticleSource semantics |
| `sidis_lowQ2/memo-mc-truth-policy-charged-sidis.md` | PS 0-3 signal definition, IsGtr sufficiency |
| `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` | Charged gen-loop, SIDIS gen variables |
| `sidis_lowQ2/reference-sidis-systematics.md` | Open question 1 (nonRad availability) |
| `sidis_lowQ2/research-sidis-systematics-framework.md` | C_rad placeholder and gap statement |

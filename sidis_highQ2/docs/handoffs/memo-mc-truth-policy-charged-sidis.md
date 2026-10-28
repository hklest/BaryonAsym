**Status:** Current handoff/reference memo. The truth-species decision `{pi, K, p}` is now frozen and should be treated as settled.

# MC Truth-Policy and Defect-Closure Guidance Memo

## Inclusive Charged-Hadron SIDIS (Low-Q2)

**Date:** 2026-05-12
**Scope:** Generator-level truth denominator, matching policy, and
defect-closure guidance for the `ChargedTotGen` and `ChargedTot` trees
produced by `ProtonTreeBuilder_lowQ2.C` (the v5b SIDIS low-Q2 builder
source; the proton-prefixed filename is legacy from the v5b SIDIS fork —
see `docs/references/reference-naming-aliases-highQ2-sidis.md`).
**Observable:** Multiplicities M^{h+/-} and charge asymmetry
A_h = (N_{h+} - N_{h-}) / (N_{h+} + N_{h-}).
**Species:** pi+/- (211), K+/- (321), p/pbar (2212). No PID applied at
reco level; the truth denominator defines the measurement target.
**Fiducial:** |eta| < 1.3, pT > 0.15 GeV. The earlier `0.3 < P < 3.0 GeV` window has since been dropped from the charged-hadron SIDIS measurement definition (frozen decision, 2026-05-16).

---

## 1. Truth Denominator Policy: IsGtr + Species Filter Sufficiency

### Q1 Answer

`IsGtr()` on an `H1PartMC` object tests whether the particle originates from
the GTR (generator-truth) bank as opposed to the STR (GEANT3 simulation)
bank. In the H1OO MC framework, the `H1PartMCArrayPtr` returns a merged
array containing both GTR and STR entries. The GTR bank contains particles
written by the event generator (RAPGAP31 or DJANGOH14) as final-state
products of the hard ep interaction. The STR bank contains particles produced
by GEANT3 during detector simulation: nuclear interactions in material,
electromagnetic showers, delta rays, and decay products of generator-stable
particles that GEANT3 decays (e.g., charged hyperons, K0_S, Lambda).

For the species set {pi+/-, K+/-, p/pbar}:

- **Primary fragmentation pi/K/p** are written to the GTR bank by the
  generator. They have `IsGtr() == true`.
- **Secondary pi/K/p from material interactions** are produced by GEANT3 and
  live exclusively in the STR bank. They have `IsGtr() == false`. A proton
  knocked out of a calorimeter absorber, or a pion from hadronic
  re-interaction in the beam pipe, is an STR particle.
- **Decay-daughter pi/K/p from hyperon decays** (Sigma -> p pi, Xi -> Lambda
  pi -> p pi pi, Omega -> Lambda K) are GEANT3-decayed because PYTHIA treats
  charged hyperons as stable. These daughters also live in the STR bank with
  `IsGtr() == false`.

Therefore, `IsGtr() == true` combined with the `isChargedHadron()` PDG
filter (abs(PDG) in {211, 321, 2212}) is **structurally sufficient** to
exclude all secondary-interaction products from the charged gen denominator.
No particle with `IsGtr() == true` and PDG in {211, 321, 2212} can be a
GEANT3 material secondary. The code review's assessment in Section A.6
(lines 100-106) that "IsGtr is structurally sufficient per researcher
Section 1.3" is **correct**.

An explicit `classifySourceGTR != kSecondaryInteraction` veto is redundant
for the gen denominator, because `classifySourceGTR` can never return
`kSecondaryInteraction` for a particle that has `IsGtr() == true` -- the
function's secondary-interaction classification relies on mother-chain
tracing that would only fire for STR particles. However, storing
`ParticleSource` in `ChargedTotGen` is recommended for defense-in-depth
diagnostics (see Section 5).

**Verdict: IsGtr + isChargedHadron is sufficient. No additional
kSecondaryInteraction veto is physically required in the gen loop.**

---

## 2. GetMother2 Recommendation

### Q2 Answer

`GetMother2()` returns the index of a second mother particle. In the H1OO
MC model, `GetMother2() != -1` indicates a particle with multiple mothers,
which arises from:

1. **String fragmentation bookkeeping artifacts** in PYTHIA/RAPGAP, where
   the LUND string endpoints are recorded as two "mothers." These are
   typically intermediate partons, not final-state particles, and the
   final-state hadrons produced by string fragmentation have
   `GetMother2() == -1`.

2. **GEANT3 re-scattering or annihilation processes** where two particles
   interact to produce the particle in question. These are STR-bank
   entries and are already excluded by `IsGtr()`.

3. **Rare generator-level processes** where the event record genuinely
   assigns two mothers to a final-state particle. In practice, for
   pi/K/p from RAPGAP31 and DJANGOH14, this is exceedingly rare at the
   GTR level. The PYTHIA fragmentation model produces final-state hadrons
   with a single mother string reference.

The ztudesy reference analysis does not check `GetMother2()` -- it relies on
`status==0` filtering, which is structurally equivalent to `IsGtr()` for
H1OO MC. The current low-Q2 charged gen loop
(`ProtonTreeBuilder_lowQ2.C:661-720`) also does not check `GetMother2()`.

The `classifySourceGTR()` function does check `GetMother2()` and classifies
multi-mother particles as `kCombinatorial` (`ProtonUtils_lowQ2.h:54`), but
this is called only in the proton gen loop and the reco matching loop, not
in the charged gen loop.

For the pi/K/p-only species set with `IsGtr()` filtering, the
`GetMother2() == -1` veto would exclude generator-level particles that
have two mothers in the PYTHIA event record. The practical population of
such entries among IsGtr pi/K/p is negligibly small -- these would be
PYTHIA bookkeeping artifacts where the mother indices happen to record
string endpoints. The ztudesy analysis, which is the published reference,
does not apply this veto and produces correct results.

**Recommendation: Add `GetMother2() == -1` as a defense-in-depth veto.**
The physics impact is negligible (expected to remove O(0) or O(few) entries
per million events for pi/K/p), but it provides:

- Consistency with the `classifySourceGTR()` logic already used elsewhere
  in the codebase.
- Protection against any future change in generator configuration that
  might populate multi-mother entries for final-state hadrons.
- Alignment with the Team A researcher's recommendation (Section 3 rule 2:
  `IsGtr() && GetMother2()==-1`).

The veto should be applied to both the `ChargedTotGen` gen loop and the
matching candidate pool in the reco loop.

---

## 3. Scattered-Electron Exclusion

### Q3 Answer

The ztudesy reference excludes the scattered electron from the gen
denominator using an `isElectron` set (`ForwardMultAnalyzer.C:767`). The
current low-Q2 builder does not perform this exclusion.

For the charged SIDIS measurement with the `isChargedHadron()` species
filter restricting to abs(PDG) in {211, 321, 2212}: **a gen-level electron
can never pass the filter.** The scattered electron has PDG = 11, which is
not in the set {211, 321, 2212}. The exclusion is therefore **physically
redundant** for this analysis.

The same applies to:
- Beam-remnant electrons (PDG 11)
- Radiative photons (PDG 22, charge 0 -- fail both PDG and charge filters)
- Muons from photoproduction (PDG 13 -- not in the PDG set)

There is no physics process in RAPGAP31 or DJANGOH14 that would assign
PDG 211/321/2212 to the scattered electron.

**Defense-in-depth argument:** Weak for this specific analysis. The PDG
filter provides a stronger species gate than the electron-index exclusion.
Adding the electron exclusion would protect against a hypothetical future
change where the species set is broadened to include leptons (as in the
ztudesy multiplicity paper definition), but that is a different analysis.
For the current pi/K/p-only measurement, the explicit electron exclusion
adds code complexity for zero physics benefit.

**Recommendation: Do not add explicit scattered-electron exclusion to the
remediated builder for the pi/K/p species set.** If the species set is
ever broadened to match the H1 multiplicity paper convention (adding e+/-,
mu+/-, charged hyperons), the electron exclusion must be added at that time.
Document this decision so future maintainers understand the scope
dependency.

---

## 4. ParticleSource Policy for Inclusive Charged Hadrons

### Q4 Answer

The `ParticleSource` enum in `ProtonUtils_lowQ2.h:29-36` classifies
generator-level particles by their production mechanism:

| PS value | Name | Origin |
|----------|------|--------|
| 0 | kPrimarySignal | Direct production in hard scatter fragmentation |
| 1 | kSigma0Decay | Daughter of Sigma0 (3212) or Sigma* (3214, 3224, 3114) |
| 2 | kXiDecay | Daughter of Xi (3312, 3322) or Xi* (3314, 3324) |
| 3 | kOmegaDecay | Daughter of Omega (3334) |
| 4 | kSecondaryInteraction | Material interaction (STR only; impossible for IsGtr particles) |
| 5 | kCombinatorial | Multiple mothers |

For pi/K/p from DIS fragmentation in RAPGAP31 and DJANGOH14:

- **PS = 0 (kPrimarySignal):** The dominant population. Pions, kaons, and
  protons directly from string fragmentation. These are the primary
  measurement target.

- **PS = 1 (kSigma0Decay):** Protons from Sigma0 -> Lambda gamma -> p pi
  gamma chains. Also pions from Sigma* -> Lambda pi or Sigma -> N pi
  decay chains where PYTHIA treats the Sigma as unstable (note: Sigma+/-
  are PYTHIA-stable with ctau ~ 2.4 cm, so their direct decay is handled
  by GEANT3 in STR; but Sigma0 is PYTHIA-unstable with ctau ~ 7e-11 cm
  and decays within the generator). A Sigma0 -> Lambda + gamma decay
  occurs at the generator level, and the subsequent Lambda -> p pi- is
  also generator-level if the Lambda is generator-unstable (Lambda ctau =
  7.89 cm; PYTHIA treats it as stable, GEANT3 decays it). Therefore,
  PS=1 protons from Sigma0 typically arise when `classifySourceGTR` traces
  the mother chain to a Sigma0 that decayed at the generator level. In
  practice, for IsGtr pi/K/p, PS=1 entries are rare because Lambda is
  PYTHIA-stable and its decay daughters are STR.

  **Clarification:** `classifySourceGTR` checks only the immediate mother,
  not the full chain. If a GTR proton has a GTR Sigma0 mother, PS=1 is
  assigned. But since Lambda is PYTHIA-stable (IsGtr Lambda exists),
  and protons from Lambda decay are STR (IsGtr=false), the GTR proton
  with a Sigma0 mother is actually the Lambda itself being misidentified
  -- this cannot happen because the function checks `abs(PDG) == 3212`
  for the mother. In practice: very few GTR pi/K/p will have PS=1
  because the feed-down chains go through GEANT3-decayed particles.

- **PS = 2 (kXiDecay) and PS = 3 (kOmegaDecay):** Same reasoning. Xi and
  Omega are PYTHIA-stable; their decays are GEANT3-handled. GTR pi/K/p
  with Xi or Omega mothers are possible only if the immediate GTR mother
  of the pi/K/p is a Xi or Omega -- which would require the pi/K/p to be
  a direct strong-decay product at the generator level. This is
  essentially impossible for the listed PDG codes.

- **PS = 4 (kSecondaryInteraction):** Impossible for IsGtr particles, as
  established above.

- **PS = 5 (kCombinatorial):** Triggered by `GetMother2() != -1`. As
  discussed in Section 2, negligibly rare for IsGtr pi/K/p.

**Policy for inclusive charged hadrons:**

For an inclusive charged-hadron measurement (multiplicities and charge
asymmetry), the signal definition should include **all primary
fragmentation products that reach the detector as pi/K/p**. This means:

- **PS = 0: Include.** This is the core signal.
- **PS = 1, 2, 3: Include.** These are physically real pi/K/p from
  hyperon feed-down chains. An inclusive measurement should count them --
  they are genuine charged hadrons produced in the ep interaction. Excluding
  them would require a model-dependent feed-down subtraction that is not
  part of the standard SIDIS multiplicity definition.
- **PS = 4: Exclude.** These are not from the ep interaction (and are
  impossible for IsGtr particles anyway).
- **PS = 5: Exclude via the GetMother2 veto** (see Section 2).

The proton analysis includes PS 0-3 as signal. **This is appropriate for
inclusive charged hadrons.** The proton analysis excludes PS=4
(`kSecondaryInteraction`) in its gen loop via an explicit veto
(`ProtonTreeBuilder_lowQ2.C:596`), which is correct but redundant given
IsGtr filtering.

**Recommendation: Accept PS = 0, 1, 2, 3 as signal. The gen loop should
apply `IsGtr() && GetMother2()==-1 && isChargedHadron(PDG)` without any
ParticleSource-based veto. Store ParticleSource in ChargedTotGen for
offline diagnostics only.**

---

## 5. Species Contamination Assessment

### Q5 Answer

With `isChargedHadron` restricting to abs(PDG) in {211, 321, 2212}, the
contamination risks from non-ep-interaction sources are:

**Beam-gas events:** RAPGAP31 and DJANGOH14 are pure ep DIS generators.
They do not simulate beam-gas interactions or pile-up. Beam-gas events
are a data-only background rejected by the DIS event selection (E-pz cut,
vertex quality, trigger requirements). At the generator level in MC, there
are no beam-gas particles -- every GTR particle originates from the
simulated ep interaction. **No risk from beam-gas in MC truth.**

**Pile-up artifacts:** H1 MC simulation includes overlay of minimum-bias
events for pile-up modeling, but these overlay events are handled by the
detector simulation (STR bank). Their particles have `IsGtr() == false`.
**No risk from pile-up in the GTR-filtered truth denominator.**

**GEANT3 PDG code reassignment:** GEANT3 uses its own particle numbering
(GEANT3 particle codes) internally, but the H1OO framework translates back
to PDG codes when writing the STR bank. For GTR particles, the PDG code
is written directly by the event generator and is never modified by GEANT3.
**No risk of PDG code reassignment for GTR particles.**

**GEANT3 creating GTR entries:** GEANT3 does not write to the GTR bank.
The GTR bank is frozen after the generator step and before detector
simulation begins. **No risk of STR-originated particles appearing with
IsGtr == true.**

**Edge cases in generator output:** Both RAPGAP31 and DJANGOH14 can
produce pi/K/p from:
- Beam-remnant fragmentation (proton remnant breakup): These are genuine
  ep-interaction products and should be in the denominator.
- Initial-state radiation recoil: The proton remnant after ISR is still an
  ep-interaction product.
- Diffractive dissociation: Particles from the diffractive system are
  generator-level products with IsGtr=true.

All of these are legitimate ep-interaction products and should be counted.

**Risk assessment: NEGLIGIBLE.** No physics process in RAPGAP31 or DJANGOH14
can produce GTR-bank particles with PDG codes 211/321/2212 that are not
from the primary ep interaction. The `IsGtr() && isChargedHadron()` filter
is a clean and complete species gate.

**Recommended guard (defense-in-depth only):** Store `GenMotherPDG` in
`ChargedTotGen` (already done at line 679-681) to allow offline inspection
of the mother-particle spectrum. If unexpected mothers appear (PDG codes
outside the expected set of quarks, diquarks, strings, and resonances),
investigate.

---

## 6. Charge-Matching Recommendation

### Q6 Answer

The code review (Section B.9) recommends requiring
`gp->GetCharge() == trk->GetCharge()` in the matching candidate scan.
From the MC physics perspective:

**Physics processes that could produce charge-inconsistent matches:**

1. **Charge-exchange scattering in material:** A gen pi+ enters the
   detector, undergoes charge-exchange (pi+ n -> pi0 p) in detector
   material, and the outgoing proton is reconstructed as a reco track.
   The reco track has positive charge (proton), and the gen particle is
   pi+ (also positive charge) -- but the PDG code changes. With angle-only
   matching, the reco proton from charge exchange would match to the
   original gen pi+ by direction. Since the charge sign is preserved in
   this example, this would not be caught by a charge veto. However, the
   reco track is a material secondary (STR) and should not be in the
   numerator regardless.

2. **Genuine charge-exchange at the generator level:** The process
   pi- p -> pi0 n is a strong interaction that conserves isospin. At the
   generator level in RAPGAP/DJANGOH, this does not happen -- the
   generators produce final-state hadrons via string fragmentation and do
   not simulate hadronic rescattering. **No generator-level charge
   exchange.**

3. **Detector-level charge confusion:** Extremely rare for CJC tracks in
   the H1 momentum range (P < 3 GeV). The magnetic field provides
   unambiguous charge determination. **Negligible.**

4. **Chance geometric alignment:** A reco h+ from a secondary interaction
   (no gen counterpart) happens to be within 5 degrees of an unrelated
   gen h-. Without the charge veto, this match is accepted. With the
   charge veto, it is rejected. This is the **primary benefit** of the
   charge veto -- it halves the combinatorial background for fake matches.

**Expected rate of genuine charge exchange lost by charge matching:**
Effectively zero at the generator level. RAPGAP31 and DJANGOH14 do not
simulate hadronic final-state interactions. A gen pi+ produces a reco pi+
(if it reaches the tracker without material interaction). The only
mechanism for a gen pi+ to produce a reco pi- is through detector
processes (charge exchange in material followed by GEANT3 tracking the
secondary), which are STR-bank processes that should be rejected anyway.

**Assessment:** Requiring charge consistency is **strongly recommended**.
It provides:

- A factor ~2 reduction in combinatorial fake-match background (the
  dominant contamination pathway identified in Section D of the code
  review as "Path 3").
- Zero physics signal loss (no genuine generator-level process converts
  a gen h+ into a reco h- within the same particle trajectory).
- Reduction of the charge-asymmetric matching artifact identified in
  code review Section B.9: material protons (h+) mismatching to
  unrelated gen pi- (h-) create a spurious asymmetric inefficiency that
  mimics a physics A_h signal.

**Recommendation: Add `gp->GetCharge() == trk->GetCharge()` to the
matching candidate scan in both the charged matching loop
(ProtonTreeBuilder_lowQ2.C:898-910) and ideally the proton matching loop
(834-845). This is a mandatory fix for the charge-asymmetry measurement.**

---

## 7. DJANGOH14 ChargedTotGen Completeness

### Q7 Answer

Open question OQ2 asks whether the low-Q2 DJANGOH14 merged MC file
suffers the same incomplete `ChargedTotGen` problem as RAPGAP31.

**What can be determined from physics knowledge alone:**

The `ChargedTotGen` tree is filled by the charged gen loop at
`ProtonTreeBuilder_lowQ2.C:661-720`. This loop iterates over
`gtrdata.GetEntries()` for every event and fills one entry per IsGtr
charged hadron. The completeness of `ChargedTotGen` depends on:

1. Whether the merged MC file contains all events from all batch jobs.
2. Whether the H1OO `H1PartMCArrayPtr` correctly reads the GTR bank from
   DJANGOH14 MC ntuples.
3. Whether the `validGenElec` guard at line 661 passes for DJANGOH14 events.

Point 3 is the most likely failure mode. The `validGenElec` flag requires
`genElecE >= 0.5` (line 563). DJANGOH14 handles radiative photons
differently from RAPGAP31 -- DJANGOH14 includes QED radiative corrections
natively, while RAPGAP31 uses HERACLES/SOPHIA. If the DJANGOH14 HAT
pointers `GenEnElec`, `GenThElec`, `GenPhElec` have different population
semantics (e.g., returning the pre-radiation electron energy instead of
post-radiation, or returning 0 for certain event topologies), the
`validGenElec` guard could suppress the entire gen loop for those events.

Additionally, the fallback recovery at line 558-561:
```cpp
if (genElecE < 0.5 && Q2sGenVal > 0 && genElecTh > 0.01) {
    double denom = 2.0 * eBeamE * (1.0 + cos(genElecTh));
    if (denom > 0) genElecE = Q2sGenVal / denom;
}
```
uses the electron-method formula Q2 = 2 E_e E_e' (1 + cos theta_e) to
recover the scattered electron energy. This is valid for non-radiative
events but can give incorrect results for hard-ISR events in DJANGOH14
where Q2sGen is the true (Born-level) Q2 but the scattered electron angle
includes the radiative kick.

**What requires file inspection:**

This question cannot be answered definitively from physics knowledge alone.
The following checks must be performed by `h1-root-analyst` on the actual
merged DJANGOH14 MC file:

1. **Entry count comparison:** Compare `ChargedTotGen->GetEntries()` to
   `DisTot->GetEntries()`. If `nChargedGen` is 0 for a significant
   fraction of events, the `validGenElec` guard is likely failing.

2. **Per-event gen multiplicity:** Plot `DisTot.nChargedGen` for DJANGOH14
   and RAPGAP31 separately. If DJANGOH14 shows a spike at
   `nChargedGen == 0` that RAPGAP31 does not, the gen loop is being
   suppressed.

3. **GenEnElec population:** Check the distribution of `GenEnElec` HAT
   pointer values for DJANGOH14 events. If a significant fraction are
   < 0.5 GeV (before the fallback recovery), those events have no gen
   denominator.

4. **Q2sGen population:** Check whether `Q2sGen` is populated for
   DJANGOH14 events. If `Q2sGen == 0` for some events, the entire gen
   block (both gen loop and gen SIDIS variables) is affected.

5. **Branch-level cross-check:** Verify that `ChargedTotGen.GenPDG`
   contains the expected PDG code distribution (dominated by 211, with
   subdominant 321 and 2212) and that no unexpected codes appear.

**Recommendation: Defer this question to h1-root-analyst for empirical
verification. Do not assume DJANGOH14 completeness based on RAPGAP31
behavior.**

---

## 8. Summary Recommendations

The remediated `ProtonTreeBuilder_lowQ2.C` and analysis script should
enforce the following policies:

### Gen denominator (ChargedTotGen loop)

- [x] `IsGtr() == true` (already applied, line 665)
- [ ] Add `GetMother2() == -1` veto (defense-in-depth)
- [x] `isChargedHadron(PDG)` restricting to {211, 321, 2212} (already
  applied, line 664)
- [x] No `kSecondaryInteraction` veto needed (IsGtr is sufficient)
- [ ] No scattered-electron exclusion needed (PDG filter is sufficient)
- [ ] Store `ParticleSource` in `ChargedTotGen` for offline diagnostics
- [ ] Update `inFiducialCharged` to use analysis-level cuts
  (|eta| < 1.3, pT > 0.15 GeV) instead of tree-level cuts
- [x] Accept PS = 0, 1, 2, 3 as signal (all primary fragmentation
  products)

### Matching candidate pool (reco loop inner gen scan)

- [x] `IsGtr() == true` (already applied, line 902)
- [ ] Add `GetMother2() == -1` veto (consistency with gen denominator)
- [x] `isChargedHadron(PDG)` (already applied, line 901)
- [ ] **Add charge-consistency requirement:**
  `gp->GetCharge() == trk->GetCharge()` (mandatory for A_h measurement)
- [ ] Update `inFiducialChargedMatch` to use analysis-level cuts

### Analysis script (acceptance computation)

- [ ] **BLOCKER: Add gen-fiducial gate to numerator** --
  `abs(GenEta)<1.3 && GenPt>0.15 && GenP>0.3 && GenP<3.0` in `reco_cut`
- [ ] **BLOCKER: Remove silent epsilon cap at 1.0** -- emit failed bins
  to a diagnostic report; refuse to produce cross-section if any bin
  has raw epsilon > 1
- [ ] Add `ParticleSource!=4` to `reco_cut` (defense-in-depth)
- [ ] Add `MatchAngle < 1.0` to `reco_cut` (tighten from 5 degrees)
- [ ] Consider binning numerator in gen variable (not reco) for
  consistent bin-by-bin acceptance definition, or implement response
  matrix unfolding

### DJANGOH14 verification (pre-reprocessing)

- [ ] Run `h1-root-analyst` to check ChargedTotGen completeness
- [ ] Verify `GenEnElec` and `Q2sGen` population for DJANGOH14 events
- [ ] Compare per-event `nChargedGen` distributions between generators

### Priority ordering for reprocessing

1. Charge-consistency in matching (eliminates dominant A_h artifact)
2. Gen-fiducial gate in numerator (eliminates acceptance > 100%)
3. Remove epsilon cap (exposes remaining bugs)
4. GetMother2 veto in gen loop and matching (defense-in-depth)
5. Tighten match angle threshold (reduces combinatorial fakes)
6. DJANGOH14 completeness verification (required before production)
7. ParticleSource storage in ChargedTotGen (diagnostic capability)
8. Fiducial flag correction (tree-level to analysis-level cuts)

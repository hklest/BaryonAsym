<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Review: MC-Physics Critique of the Gen->Reco Matching Pool (Charged-Hadron SIDIS, low-Q2)

**Date:** 2026-05-19
**Role:** `h1-mc-expert` (authoritative on generator semantics, GTR/STR
banks, H1PartMC, BGcheck/ParticleSource, C_rad).
**Mode:** Read-only critique. No code modified.
**Scope:** Critique of the charged-hadron matching block in
`ProtonTreeBuilder_lowQ2.C` lines 1060-1170, with the matching pool
defined by `ProtonUtils::isChargedHadron`, `H1PartMC::IsGtr`, and
`H1PartMC::GetMother2`. Anchored to the MC-physics infrastructure
described in `H1/CONTEXT.md` and the analysis-level decisions in
`sidis_lowQ2/context-handoff.md` Sections 4, 7, and Session End State.
**Audience:** `h1-sidis-supervisor`, `h1-acceptance`, `h1-systematics`,
`h1-analysis-critic`, `h1-unfolder`, `code-editor`.

---

## 0. The matching pool, as built

For each reco track that has already passed `BASE_CUT` and reaches the
charged matching block (`ProtonTreeBuilder_lowQ2.C:1078-1169`), the
builder iterates `gtrdata = H1PartMCArrayPtr` and accepts a candidate
gen particle `gp` only if all of the following hold:

```cpp
!genUsedCharged[ig]                              // one-to-one claim
ProtonUtil::isChargedHadron(gp->GetPDG())        // |PDG| in {211,321,2212}
gp->IsGtr()                                       // (fStrIdx == -1)
gp->GetMother2() == -1                            // defense-in-depth
gp->GetCharge() == trk->GetCharge()              // charge-consistent
|gp.GetMomentum().Eta()| < 3.5                   // broad gen eta
gp.GetMomentum().Pt() > 0.05 GeV                 // broad gen pT
```

The best (smallest 3D opening angle) candidate is accepted if
`bestAngle_ch < CHARGED_MATCH_ANGLE = 2.0 deg`
(`PhysicsConstants_lowQ2.h:44`). The matched gen object then sets
`GenPDG_ch`, `GenCharge_ch`, `ParticleSource_ch =
classifySourceGTR(matched, gtrdata)`, and the matched-gen SIDIS variables
boosted into the gen `gammaGen` photon frame.

The gen denominator (`ChargedTotGen`) at lines 814-902 applies the
identical pool filter plus the broad fiducial. So the denominator and
the matching pool are **structurally consistent** with one another --
but the question this review answers is: is that consistent definition
physically meaningful for an absolute charged-hadron cross-section, and
where does it break?

---

## 1. GTR-only matching pool

### MC-physics statement

`H1PartMC::IsGtr()` is implemented as `(fStrIdx == -1)`
(`h1oo-master/H1Mods/H1PartMC.C:193-197`). The GTR bank holds the
PYTHIA/JETSET (RAPGAP31) or LEPTO/JETSET (DJANGOH14) generator-level
event record AFTER the generator's own decay step but BEFORE GEANT3
detector simulation. Long-lived neutral hyperons (Lambda, K0_S, K0_L,
Xi^0) and the charged hyperons (Sigma+, Sigma-, Xi-, Omega-) are
PYTHIA-stable: the generator writes the parent to the GTR bank with
`fDaughter1 = -1` and leaves decay to GEANT3. The decay products
therefore appear ONLY in the STR bank, with `fStrIdx >= 0`.

Concretely, the matching pool excludes:

| Channel | BR | Stage of decay | Daughter location |
|---------|-----|---------------|-------------------|
| Lambda -> p pi- | 64% | GEANT3 (PYTHIA-stable) | STR only |
| AntiLambda -> p-bar pi+ | 64% | GEANT3 | STR only |
| K0_S -> pi+ pi- | 69% | GEANT3 | STR only |
| K0_L -> pi+/- e/mu nu | ~67% | GEANT3 | STR only |
| Sigma+ -> p pi0 | 52% | GEANT3 | STR only |
| Sigma+ -> n pi+ | 48% | GEANT3 | STR only |
| Sigma- -> n pi- | 99.85% | GEANT3 | STR only |
| Xi- -> Lambda pi- | 100% | GEANT3 cascade | STR only |
| Omega- -> Lambda K- | 68% | GEANT3 cascade | STR only |
| K+/- -> mu+/- nu | 64% | GEANT3 (in-flight DIF) | STR only |
| pi+/- -> mu+/- nu | ~100% asymptotic | GEANT3 DIF | STR only |
| D, B hadrons | varies | GENERATOR PYTHIA-decayed | GTR primary, GTR daughters |

The last entry is important: charm and beauty mesons are PYTHIA-decayed
at the generator level, so D -> K pi and B -> X are typically GTR/GTR
chains. Whether the daughter D-decay pion has `Mother1` pointing to the
D meson (still in GTR) and whether `GetMother2() != -1` for any of them
depends on the PYTHIA event-record bookkeeping (see Sec. 3).

### Physical correctness for charged-hadron SIDIS

The frozen analysis decision (handoff Sec. 3 and the truth-policy memo)
is to define the measured species as {pi+/-, K+/-, p/p-bar} ONLY, with
hyperons NOT counted in the multiplicity. With that definition:

- The GTR-only matching pool is correct for the *signal* class: primary
  pi/K/p are in GTR and the matching reaches them.
- BUT a reco track from a Lambda-decay proton (`Lambda -> p pi-`) is a
  REAL central, primary-vertex-consistent reco hadron (Lambda c-tau =
  7.89 cm, much of the decay volume is inside the beam pipe / CST /
  inner CJC). Its gen counterpart is the STR proton. It is NOT in the
  matching pool. The track will therefore be:
    - rejected by `IsMatched_ch == 0`, contributing to "unmatched"; OR
    - accepted but pulled toward an unrelated GTR primary pi/K/p within
      2 degrees, then rejected by `GetCharge` consistency if the charge
      sign differs (good), or accepted as a fake match if it does not
      (bad).

This is the textbook V0-feeddown problem. The current pool design
relies on the second outcome being rare and the first outcome being
counted as numerator background that must be subtracted.

### Quantitative population at HERA-II low-Q2 (5 < Q2 < 100 GeV2)

These are taken from H1-published expectations and from the
specifically-cited DESY 20-176 systematics catalog
(`research-background-sources-migration.md` Sec. 2.5):

- K0_S daughter contamination in primary tracks: ~0.2-1% of charged
  tracks (1302.1321 estimate; DESY 20-176 Sys.6 assigns 50% systematic
  uncertainty on the K0_S subtraction).
- Lambda + AntiLambda daughter contamination in primary tracks: O(0.5%),
  dominated by the proton daughter (the pi- daughter spectrum overlaps
  the primary pi- spectrum).
- Sigma+/- charged-track contribution (the parent itself, since charged
  Sigma have ct = 2.4 cm / 4.4 cm and can hit the CST/CJC before
  decaying): 0.1-0.5% (handled separately in Sec. 2 below).
- Total V0/hyperon-decay contamination of primary tracks: ~1-2% of the
  numerator after BASE_CUT.

For an absolute cross-section measurement these are large effects.
DESY 20-176 explicitly subtracts the K0_S contribution (Sys.6) and
quotes a residual systematic; our analysis has IsV0DaughterGen on
ChargedTotGen as of v5b (handoff Sec. 12 / Session End State) but the
matching pool itself does not use it, and no V0-daughter subtraction
script currently exists.

### Severity

**CONCERN** for charged-h multiplicities (1-2% effect, charge-mostly-
symmetric for K0_S, partially asymmetric for Lambda/AntiLambda); **INFO**
for A_h (the Lambda/AntiLambda asymmetry, which IS the central
observable of `H1/lambda/`, can leak into A_h at the ~0.1% level per the
background research note Sec. 2.6). For absolute cross-section
publication this is a FATAL-class gap unless an explicit V0/hyperon
feed-down subtraction is added.

---

## 2. `isChargedHadron` restriction to {211, 321, 2212} in the matching pool

### MC-physics statement

`ProtonUtils::isChargedHadron` (`ProtonUtils_lowQ2.h:81-84`) is hard-coded
to `|PDG| in {211, 321, 2212}`. The contrast with
`isChargedStable` (lines 86-99) is informative: the broader definition
of "charged stable" (c-tau > 10 mm) used by the DESY 20-176 H1
multiplicity paper additionally includes e+/-, mu+/-, Sigma+ (3222),
Sigma- (3112), Xi- (3312), and Omega- (3334).

When a reco track is genuinely a primary charged Sigma+ that survived to
the CJC (ct = 2.4 cm for Sigma+, 4.4 cm for Sigma-; many decays occur
beyond the beam pipe and produce a measurable track segment), its gen
counterpart in GTR has PDG 3222 or 3112 and is REJECTED by
`isChargedHadron`. The reco track therefore becomes unmatched, is
classified as a fake, and is effectively rejected by any analysis cut
on `IsMatched`.

The gen denominator (line 820) applies the same `isChargedHadron`
filter, so the gen denominator also excludes Sigma+/-. The
consistency (Sigma+/- absent from numerator and denominator) means the
ratio N_h_reco / N_h_gen is correct in expectation IF the reco
acceptance for Sigma+/- is identical to that for pi/K/p in the same
kinematics. It is not: the Sigma+/- nuclear interaction cross-sections,
decay-in-flight probabilities, and tracking efficiencies differ.

### Quantitative expectation at HERA-II low-Q2

PYTHIA 6 production of Sigma+/- in DIS fragmentation at H1 kinematics
is small. From PYTHIA's strangeness suppression and isospin counting,
charged Sigma production is approximately:
N(Sigma+ or Sigma-) / N(charged hadrons) ~ 0.3-0.5%.

This is the same order of magnitude as the Lambda/AntiLambda
contamination but with a different topology: charged Sigma are
themselves charged, so they leave a real track in the CTD until they
decay; if the decay happens beyond the silicon vertex detector they are
indistinguishable from primary pi/K/p at the analysis-cut level.

There is no published H1 measurement of Sigma+/- inclusive
electroproduction at HERA-II that I can cite to pin the number, but the
order of magnitude is consistent with the < 0.5% range. Generator
differences between RAPGAP31 (PYTHIA 6) and DJANGOH14 (JETSET 7.4)
could be at the factor-of-two level here because both use similar
parton-shower and string-fragmentation parameters but with different
tunes; this is captured by the existing `mc_model_swap` systematic to
the extent that the analysis takes the generator difference as the
uncertainty.

### Severity

**INFO** for inclusive charged-hadron multiplicities (sub-percent
effect, mostly canceling between numerator and denominator). **CONCERN**
for absolute cross-section because the species-definition exclusion of
Sigma+/- is non-standard relative to the published H1 multiplicity
paper (DESY 20-176) and the cross-section should be quoted with explicit
reference to the species set.

The handoff Sec. 3 documents the species choice as a frozen analysis
decision. That is acceptable but needs to be quantified: a one-time
PYTHIA-truth study should be run on RAPGAP31 and DJANGOH14 to count
how many primary GTR Sigma+/- with |eta| < 1.3 and pT > 0.15 GeV pass
through (`abs(PDG) == 3222 || 3112 && IsGtr && decay_radius > 30 cm`)
versus primary pi/K/p. That number is the unmatched-fake fraction
attributable to species exclusion.

---

## 3. `GetMother2() == -1` veto: generator-specific behavior

### MC-physics statement

`H1PartMC::fMother2` is the second-parent index in the PYTHIA/JETSET
event record. The semantics differ slightly between PYTHIA 6 (used by
RAPGAP31) and JETSET 7.4 (used by DJANGOH14 fragmentation):

- **PYTHIA 6 / RAPGAP31:** Final-state hadrons from string fragmentation
  have a single string parent. `K(I,4)` and `K(I,5)` ("mothers" in
  the H1PartMC convention) are populated such that fragmented hadrons
  have `Mother1 = string index`, `Mother2 = 0` (treated as -1 in the
  H1OO writeback, see H1Mods conversion). The Mother2 != -1 case fires
  for:
    - Resonance daughters where multiple resonances feed the same
      particle index (rare, mostly junk indices in the PYTHIA history).
    - String junctions in baryon production (the rare three-quark
      junction topology in baryon-number rearrangement).
    - Particles that arose from joining two color singlets in the
      RAPGAP partonic interface (multi-parton interaction artifacts;
      RAPGAP 3.1 has MPI off by default).

- **DJANGOH14 / LEPTO / JETSET 7.4:** Similar string-fragmentation
  semantics, but LEPTO's partonic event record can attach the QED
  radiation diagram differently. ISR/FSR photons themselves can have
  Mother2 populated when LEPTO marks a hadron as the kinematic
  recoil partner of a radiated photon (this is a documentation/history
  flag, not a physics process). DJANGOH14 also sometimes writes
  history particles with two mothers for the HERACLES radiative-photon
  vertex.

The empirical population fraction of `IsGtr && |PDG| in {211,321,2212}
&& GetMother2() != -1` particles is expected to be O(10^-4) to O(10^-3)
in both generators, but is NOT guaranteed to be the same between
RAPGAP31 and DJANGOH14. The truth-policy memo (Sec. 2) states "the
practical population of such entries among IsGtr pi/K/p is negligibly
small" and recommends the veto as defense-in-depth.

### Where the asymmetry bites

For the model systematic (RAPGAP vs DJANGOH), the matching efficiency
denominator includes the same `GetMother2() == -1` cut applied to the
gen denominator (line 826). If RAPGAP31 has 0.05% of GTR pi/K/p with
Mother2 != -1 and DJANGOH14 has 0.2%, then:

- Both numerators lose those entries equally (the matching pool
  excludes them).
- Both denominators lose those entries equally (the gen loop excludes
  them).
- Acceptance ratio is unchanged because the cut is applied
  consistently.

BUT: the *physical* meaning of "Mother2 != -1" is different in the two
generators. In RAPGAP this is likely a string-junction artifact (no
real physics impact). In DJANGOH this could correlate with QED
radiation, which means the radiative-MC and non-radiative-MC samples
inside DJANGOH may have different `Mother2` populations. That
correlation would propagate into C_rad as a small generator-internal
systematic.

### Severity

**INFO** for the matching-pool consistency itself (negligible
population). **CONCERN** for the RAPGAP-vs-DJANGOH systematic
interpretation if the absolute Mother2-population difference exceeds
~0.5%, because the model-swap delta would then include a
bookkeeping-driven component that has nothing to do with physics
modeling.

The concrete next check (see action item M3 below) is to count, on the
existing low-Q2 v5b RAPGAP31 and DJANGOH14 MC trees, the fraction of
`IsGtr && |PDG| in {211,321,2212} && GetMother2() != -1` particles per
kinematic bin (Q2, x). This requires extending the gen loop to write
a `GetMother2()` diagnostic branch on `ChargedTotGen`, OR running a
one-time H1OO macro that reads `H1PartMCArrayPtr` directly and counts.

---

## 4. Beam protons and the broad eta=3.5 admit window

### MC-physics statement

The pool admits `|GenEta| < 3.5` and `GenPt > 0.05`. Beam-remnant
protons from string-fragmentation of the proton remnant have:

- eta ~ 4-7 (most of them; the proton beam direction at H1 corresponds
  to eta -> +infinity for outgoing forward particles in the
  electron-incoming convention).
- pT ~ 0.05 - 0.5 GeV (soft remnant fragmentation).

The `|eta| < 3.5` cut excludes the bulk of the beam-remnant population.
What survives is the tail of the proton-remnant pT spectrum and any
proton-fragmentation product that landed at eta ~ 3.0-3.5. This is
documented in the handoff Sec. 6 as a 4.7% population of GTR entries.

### Geometric overlap with central reco tracks

The angle 2 deg is in 3D opening angle, not pseudorapidity. Using
theta(eta) = 2 atan(exp(-eta)):

- theta(eta=1.3) ~ 33.5 deg
- theta(eta=3.5) ~ 3.4 deg
- theta(eta=2.0) ~ 15.4 deg
- theta(eta=2.5) ~ 9.4 deg

A central reco track at eta=1.3 cannot match a gen proton at eta=3.5
within 2 deg (gap is 30 deg). The risk window is when the reco track
is at the *high-eta edge* of |eta| < 1.3 and the gen beam-remnant
proton happens to be at the *low-eta edge* of the pool admission (close
to 3.5). The minimum 3D opening angle between (eta_reco, eta_gen) =
(1.3, 3.5) is bounded below by |theta_reco - theta_gen| = 30.1 deg if
phi is identical, so the geometric overlap is **zero** for any reco
track inside the analysis fiducial.

If the analysis fiducial were extended to |eta| < 2.0 (Scheme A+ or
combined-track studies), the gap shrinks to ~12 deg, still well above
the 2 deg threshold. For Scheme A+ at |eta| < 2.5, the gap is ~6 deg,
still safe.

### Where the contamination DOES bite

Forward gen protons at eta = 3.0-3.5 are eligible to claim a one-to-one
slot via `genUsedCharged[ig] = true` even if no reco track matches them.
This is irrelevant because the matching loop iterates over reco tracks,
not over gen particles, so an unclaimed eta=3.5 gen proton simply
remains unclaimed. **No bias from this mechanism.**

The real cost of admitting eta-3.5 gen protons is in the gen
denominator (line 884: `inFiducialCharged = (|GenEta| < 3.5 && GenPt >
0.05)`). The gen denominator written to `ChargedTotGen` includes
beam-remnant protons. Downstream analysis scripts must filter on
`|GenEta| < 1.3 && GenPt > 0.15` to get the analysis fiducial; the
v5b extraction script does this (the handoff explicitly verified that
the GenElecPhi numerator/denominator inconsistency was fixed and
median efficiency dropped to 0.82 with all bins <= 1).

### Severity

**INFO** for the matching pool (geometric overlap is zero for the
current |eta| < 1.3 fiducial; bounded for any extension up to |eta| <
2.5). **CONCERN** only if the analysis fiducial is ever widened toward
the forward region without also re-checking the angular geometry.

---

## 5. Radiative vs non-radiative MC: matching frame invariance

### MC-physics statement

RAPGAP31 contains NO QED radiation in its standard run mode. DJANGOH14
in radiative mode includes ISR and FSR via HERACLES. The "non-radiative"
DJANGOH14 sample is Born-level.

The matching uses `recoDir = TVector3(Px, Py, Pz)` (lab momentum of the
reco track) and `genMom = gp->GetMomentum()` (lab momentum of the GTR
particle, AFTER QED radiation in the radiative case, BEFORE in the
non-radiative case). The 3D opening angle `recoDir.Angle(genMom)` is a
lab-frame quantity. It does not care about the gen DIS frame
construction.

### Frame consistency check

The matched-gen SIDIS variables (`ystarGen_ch`, `yBreitGen_ch`,
`xFGen_ch`, `zGen_ch`) are computed via `boostGammaPGen` and
`boostBreitGen` (lines 1147-1161). These boosts are built per-event
from the gen scattered-electron 4-vector and the gen proton beam. In
the radiative case, the gen scattered-electron 4-vector is the
post-FSR electron (the visible scattered electron); the photon 4-vector
`gammaGen` is `k_in - k_out_post_FSR`, which is the standard "true"
virtual photon kinematics seen by the hadronic final state. So the
boost is event-self-consistent: hadron 4-vector and boost both live in
the same post-radiation frame.

For C_rad, the relevant comparison is rad/nonrad at the gen-binning
level for the DIS event count. The matched-gen SIDIS variables in the
radiative sample correctly use the post-radiation `gammaGen`. The
non-radiative sample uses Born `gammaGen`. The acceptance epsilon =
N_matched(reco) / N_gen(gen) is rad/nonrad-comparable because the
matching is lab-frame and the gen-level binning uses the SAME boost
construction per sample.

### Where this could quietly bias

If the C_rad computation ever attempts to do gen-level binning of the
matched-gen variables IN one sample and the gen-only variables IN the
other (cross-sample), the boosts come from different events and the
comparison is wrong. The current C_rad design
(`research-crad-for-charged-sidis.md`) explicitly uses per-bin gen-only
counts, so this trap is avoided.

### Severity

**INFO**. The matching itself is rad/nonrad-insensitive at the angular-
pairing level. The downstream C_rad construction must continue to use
gen-only counts per generator/sample, which the existing plan does.

---

## 6. Response matrix generator choice

### MC-physics statement

The current charged-hadron SIDIS MC samples on v5b are RAPGAP31 (35
jobs) and DJANGOH14 (93 jobs at low-Q2). RAPGAP31 has NO QED radiation;
DJANGOH14 in default mode has ISR/FSR enabled. Data has real QED
radiation. Therefore:

- A response matrix R(reco|gen) built from RAPGAP31 MC is missing the
  reco-side smearing induced by QED radiation. Applying it to data
  (which has QED radiation) leaves a radiative bias in the unfolded
  result that C_rad cannot fully repair, because C_rad acts on the gen
  DIS event count, not on the per-bin response.
- A response matrix built from DJANGOH14 MC includes QED radiation on
  both axes, matching data, and is the physically defensible choice
  for unfolding.

### What the handoff says

The "First Physics Results" block in the Session End State reports that
multiplicities and A_h were extracted using RAPGAP31 for acceptance.
The handoff does not explicitly state which generator is used for
response-matrix construction in the planned 4D unfolder. The current
acceptance map epsilon(Q2, x, z, pT_h) is built from RAPGAP31 per
`tmd_resolution_acceptance.py` (Sec. 6B).

### Implication for absolute cross-section

For absolute multiplicities (the central deliverable per handoff Sec.
1), using a non-radiative generator (RAPGAP31) for the response matrix
combined with a separately computed C_rad correction is the LEGACY
H1 approach (1302.1321 uses RAPGAP for acceptance and a separate
HECTOR/HERACLES C_rad). This is defensible if:

- C_rad is computed at the event-DIS level, not per hadron.
- The hadronic-side smearing inside the response matrix is dominated by
  HFS resolution, not by the radiative photon kinematics, so the
  no-QED RAPGAP response is "good enough."

Neither assumption is automatic. For SIDIS multiplicities at low Q2 and
high y, the radiative ISR can shift the reconstructed Q2 by a factor of
2+ and the per-track kinematic placement in (z, pT_h) bins is affected.
A DJANGOH14-radiative response matrix would be more conservative.

The truly correct construction is:

- Build R from DJANGOH14 radiative MC.
- Compute C_rad as (gen-fiducial non-rad / gen-fiducial rad) from
  the DJANGOH14 nonrad / DJANGOH14 rad pair.
- Apply R^-1 to data to unfold to gen-radiative truth.
- Multiply by C_rad to get Born-level truth.

The current MVP plan uses a placeholder for C_rad
(handoff Sec. 9 "crad_model_swap: NonRad charged-tree MC unavailable")
and treats the model swap RAPGAP <-> DJANGOH as the systematic. This
underestimates the QED-related component of the response.

### Severity

**FATAL** for absolute cross-section publication if the response matrix
is RAPGAP31-only. **CONCERN** for the current MVP because the analysis
publishes multiplicities and A_h (multiplicity is approximately a
ratio of two cross-sections that share the QED-radiative correction at
leading order, so the radiative bias partially cancels; A_h cancels it
to first order). For an absolute charged-h cross-section release, the
unfolding response matrix MUST be sourced from DJANGOH14 radiative MC
or from RAPGAP31 with a per-bin radiative correction applied at the
response-matrix level.

---

## 7. Decays in flight (DIF) of K+/- and pi+/-

### MC-physics statement

Kaon DIF: `K+ -> mu+ nu` (BR 63.5%), `K+ -> pi+ pi0` (BR 20.7%), c-tau
= 3.71 m. At pT = 0.5 GeV and the central rapidity region, the
relativistic decay length beta gamma c-tau ~ 5-10 m, but the CJC
tracker outer radius is ~85 cm and the CST/CIZ is at ~5-20 cm. The
probability of decaying inside the active tracker volume for a typical
SIDIS-kinematic K+/- is approximately 1 - exp(-L_track / (beta gamma
c-tau)) ~ 10-20% per kaon.

GEANT3 simulates the kaon decay in flight, producing an STR muon (or
STR pi+ pi0) at the decay vertex. The H1OO H1PartSelTrack
reconstruction sees a curved-track segment up to the decay point and a
second segment (or no second segment, if the daughter is below the pT
threshold). The reco outcomes:

1. **Track-merger:** the H1 pattern recognition extends the segments
   into a single helix with kinked momentum. The reco track has the
   parent-K direction at production and approximately the parent-K
   momentum (with a small systematic shift); IT MATCHES the GTR K+/-
   correctly via the 2 deg angular pool. This is the dominant outcome
   for K -> mu nu when the kink angle is small.
2. **Track-split:** two reco tracks are produced. The upstream segment
   has the K direction and is short (low NHitDedx, fails BASE_CUT). The
   downstream segment is the muon track, with no GTR counterpart (the
   STR muon is not in the pool). It becomes an UNMATCHED FAKE.
3. **Downstream-only:** the upstream segment is too short to be
   reconstructed; only the downstream daughter track is reconstructed.
   Same outcome as case 2 (unmatched fake).

Pion DIF: `pi+ -> mu+ nu`, c-tau = 7.8 m, so beta gamma c-tau is ~30 m
for pT ~ 1 GeV. The probability of decaying inside the tracker is
~1-3% per pion, far less than for kaons. The same three outcomes apply
in proportion.

### Quantitative expectation

For inclusive charged SIDIS at low Q2 with the standard BASE_CUT,
fast tracking studies in H1 (and the equivalent ZEUS / EIC studies)
quote:

- K+/- DIF-induced fake rate: 1-3% of the reco K+/- numerator.
- pi+/- DIF-induced fake rate: 0.1-0.3% of the reco pi+/- numerator.

The H1NuclIACor correction (handoff: "NucliaWeight added in v5b") is
specifically the per-track multiplicative weight that compensates the
MC overestimate of pion/kaon nuclear ABSORPTION cross-sections; it
does not specifically handle DIF fake tracks.

### What the current pool does

For the merged-track outcome the matching is correct (GTR K+/- claimed
correctly). For the split / downstream-only outcomes the reco track is
unmatched. The unmatched-fake population enters the numerator only if
the analysis does NOT cut on `IsMatched`. The current acceptance
construction (handoff Sec. 6B) uses matched-gen numerator and gen
denominator, so unmatched fakes are NOT in the numerator. They are,
however, in the data (which has no matching information). The
acceptance map therefore underestimates the data yield by the
unmatched-fake fraction.

### Severity

**CONCERN** for absolute K+/- multiplicity (1-3% downward bias if no
explicit DIF-fake correction). **INFO** for pi+/- multiplicity (sub-
percent). **INFO** for A_h (DIF is charge-symmetric to within ~0.1%
because the K+/- decay BRs are CP-symmetric).

For an absolute cross-section, a DIF-correction term should be added
to the kaon acceptance, derived from a one-time MC truth study of
"GTR K+/- in fiducial that did NOT produce a matched reco track in the
CJC due to DIF inside the tracker."

---

## 8. STR-only daughter tracks (Lambda, K0_S, Xi, Omega decays)

### MC-physics statement (extension of point 1)

Reco tracks from `Lambda -> p pi-` decay inside the tracker volume
have:

- A primary-vertex DCA close to zero (`DcaPrime`) if the Lambda decayed
  near the primary vertex (Lambda c-tau = 7.89 cm; ~30% of Lambdas
  decay within the first 5 cm).
- A daughter pT distribution that extends down to 0.15 GeV (analysis
  threshold).
- A reco track flag `IsFromPrimary == 1` if the Lambda decayed before
  the silicon vertex detector (~5 cm), which is roughly the same 30%
  fraction.

These tracks pass BASE_CUT and enter the reco numerator. The matching
pool does not contain their gen counterparts (STR pi-, STR p), so the
pool matching either:

- Returns `IsMatched_ch = 0` (no GTR candidate within 2 deg), classifying
  the track as unmatched and contributing to the unmatched-fake
  population. The analysis script's `reco_cut` requires `IsMatched ==
  1 && Charge == GenCharge` (per the handoff and standard analysis
  flow), so these tracks are dropped.
- Returns a spurious match to an unrelated primary pi/K/p within 2 deg
  with the correct charge (very rare given the small 2 deg cone, ~0.1%
  combinatorial rate).

### Why the gen denominator does NOT include them

The gen denominator gen loop (lines 814-902) applies the same
`isChargedHadron && IsGtr && GetMother2 == -1` filter. Lambda-decay
products are STR and not in the gen denominator. So the species
mismatch IS consistent.

### The asymmetry that bites

For DATA, the analysis has no matching information. The data numerator
INCLUDES the Lambda-decay daughter tracks (they pass IsFromPrimary
because they decay near the primary vertex). The MC-derived acceptance
denominator EXCLUDES them.

The corrected data multiplicity is:

M_data_corrected = N_data_reco_in_bin / epsilon_MC

where `epsilon_MC = N_matched_in_fid_gen_bin /
N_gen_in_fid_gen_bin`. Because both numerator and denominator of
epsilon exclude V0 daughters, epsilon is the acceptance for primary
pi/K/p ONLY. Dividing data (which includes V0 daughters) by an
acceptance that excludes them OVERESTIMATES the multiplicity by the
V0-daughter contamination fraction.

This is exactly the case for which DESY 20-176 publishes a 50%
systematic on the K0_S subtraction (Sys.6 in `research-h1-1302-1321
-charged-multiplicity.md`).

### v5b infrastructure

The handoff records that `IsV0DaughterGen` was added to ChargedTotGen
in v5b. This is the correct hook: a flag on the gen tree indicating
"this gen particle is a daughter of a V0." But the current matching
pool does NOT use it (cannot - V0 daughters are STR, not GTR, so they
are not in the matching pool in the first place). The flag must be
used by a downstream subtraction script that:

1. Reads ChargedTot for the data-side numerator.
2. Reads ChargedTotGen for the MC-truth V0-daughter fraction.
3. Subtracts the V0-daughter contribution from the data numerator
   BEFORE dividing by the acceptance.

This subtraction script does not currently exist (the handoff Sec. 12
"Next Steps" do not include it; the migration-background script in
`research-background-sources-migration.md` is a different background).

### Severity

**CONCERN** for absolute multiplicity (1-2% bias upward without
subtraction). **INFO** for A_h (the Lambda-daughter proton and the
AntiLambda-daughter antiproton enter opposite charges with mostly
cancelling kinematics; the K0_S daughter pi+ pi- pair is exactly
charge-symmetric).

For absolute cross-section publication this MUST be subtracted, and
the subtraction MUST use the v5b `IsV0DaughterGen` truth flag on a per-
bin basis. A 50% systematic on the subtraction itself is the
H1-standard treatment.

---

## 9. Heavy-flavor decays (D, B)

### MC-physics statement

Charm and beauty hadrons in RAPGAP31 and DJANGOH14 are produced
according to the PYTHIA matrix elements + parton shower. PYTHIA
decays charm and beauty hadrons at the GENERATOR level (D, B are NOT
PYTHIA-stable). So:

- The primary D or B meson is in GTR.
- Its decay products (D -> K pi pi, D -> K* pi, B -> D X) are also in
  GTR with `Mother1` pointing back to the parent D/B.
- `GetMother2()` is -1 for the decay products (single string parent
  in the cascade sense).

This is DIFFERENT from the Lambda/K0_S case: D and B decay products
ARE in the GTR matching pool. They have `IsGtr == true`, pass
`isChargedHadron`, satisfy `GetMother2 == -1`, and have the correct
charge for the underlying physics.

### Implication for the matching pool

A reco K+ from D+ -> K- pi+ pi+ (BR 9.4%) at H1 kinematics enters the
matching pool as a regular GTR K+. Its gen counterpart in GTR is the
K+ daughter of the D+. The matching works correctly. The
`classifySourceGTR` function on this particle would check `Mother1 =
D+ (PDG 411)`, which is NOT in the {3212, 3214, 3224, 3114, 3312,
3322, 3314, 3324, 3334} feed-down list, so `ParticleSource = 0
(kPrimarySignal)`.

This means D/B-decay charged hadrons are classified as primary signal
and are included in the numerator and the denominator. For an
inclusive charged-hadron measurement this is the correct physics:
D-decay pions and kaons ARE genuine charged hadrons produced in the
ep interaction, and should be counted.

### Quantitative expectation

Charm contribution to inclusive charged hadrons at HERA-II low Q2:
~3-5% (PYTHIA + RAPGAP charm production fraction in DIS). Beauty
contribution: ~0.5-1%. Both are correctly captured by the matching
pool as `ParticleSource = 0`.

### Subtle issue: charm decay vertex inside the tracker

D mesons have c-tau ~ 100-300 microns. At pT ~ 2 GeV, beta gamma c-tau
~ 1-3 mm. The decay happens well inside the beam pipe. Reco tracks
from D-decay have `IsFromPrimary == 1` (or, for fast secondary-vertex
finders, may be flagged as displaced). For the inclusive charged-hadron
analysis with `IsFromPrimary == 1` cut, D-decay tracks are included.
This is consistent with their inclusion in the gen denominator. No
bias.

### Severity

**INFO**. The matching pool correctly handles heavy-flavor decay
products because PYTHIA decays them at the generator level.

---

## 10. Material-interaction secondaries

### MC-physics statement

Hadronic interactions in the inner detector material (beam pipe, CST,
CJC inner wall) produce secondary charged hadrons (recoil protons from
nuclear interactions, secondary pions from p N -> p N pi, etc.).
GEANT3 tracks these and writes them to the STR bank. They have
`IsGtr == false` and `ParticleSource == 4 (kSecondaryInteraction)` in
the `classifySourceGTR` semantics — except `classifySourceGTR` ONLY
returns 4 if `part == nullptr`, never for valid GTR particles
(`ProtonUtils_lowQ2.h:51`). For STR-only particles `classifySourceGTR`
would not even be called because the matching pool excludes them.

### Mechanism that survives BASE_CUT

The BASE_CUT cuts (NHitDedx >= 10, RadLength > 10, DcaPrime cut,
IsFromPrimary, IsCentralTrk) reject most material secondaries. The
ones that survive are:

- Secondary tracks from a very-small-radius interaction (within the
  first beam pipe wall) where the primary-vertex extrapolation is
  consistent with the primary.
- Recoil protons from quasi-elastic scattering of a primary track
  inside the silicon vertex detector. These get IsFromPrimary == 1
  because their helical extrapolation passes through the primary
  vertex within the DCA window.

These tracks have no GTR counterpart (they are STR), so the matching
pool returns `IsMatched_ch = 0`. They are dropped by the analysis
script's `IsMatched == 1` cut on the MC side, but they ARE in the data
(which has no matching information), so the same V0-daughter-style
bias applies.

### Population fraction

For the BASE_CUT + IsFromPrimary + IsCentralTrk selection, residual
material secondaries are typically 0.5-1% of the reco numerator
(H1 tracking studies; consistent with the DESY 20-176 estimate).

### Severity

**CONCERN** for absolute multiplicity (sub-percent to 1% bias).
**INFO** for A_h (material secondaries from p N interactions are
charge-asymmetric because the underlying p N cross-section is, but
the magnitude is < 0.5%).

Mitigation: this is part of the unmatched-fake background that should
be subtracted by a per-bin MC-truth study of "data-numerator minus
matched-primary-numerator," giving the cumulative fake fraction from
ALL non-matching causes (V0 daughters + DIF + material secondaries +
conversions).

---

## 11. Photon conversions and Dalitz pairs

### MC-physics statement

Gen photons (PDG 22) from pi0 decay and direct radiation pass through
the material. GEANT3 simulates conversion `gamma -> e+ e-` with the
material-dependent probability ~7% per radiation length (for soft
photons in the analysis kinematic range, the conversion length is a
few percent of the inner-detector material budget). The e+/e- pair
is written to the STR bank.

The reco e+/e- tracks typically have low pT and large opening angle
relative to the gen photon direction. They may pass BASE_CUT if their
pT > 0.15 GeV, but the IsCentralTrk cut and the requirement of a
primary-vertex-consistent helix usually reject them.

### Matching outcome

The pool excludes the STR e+/e- pair. A surviving conversion track
becomes unmatched-fake. Same population as material secondaries:
typically 0.5% per DESY 20-176 Sys.6.

### Severity

**INFO** for inclusive charged-hadron multiplicity (sub-percent;
already part of the < 1% unmatched-fake budget). **INFO** for A_h
(conversion e+ and e- are exactly charge-symmetric in production
kinematics, so they cancel in the asymmetry to ~0.01%).

---

## 12. Fixed 2 deg matching window vs momentum-dependent multiple scattering

### MC-physics statement

The angular resolution of a CJC track in the H1 magnetic field is
dominated by:

- **Position resolution** of the CJC wires: sigma_phi ~ sigma_z ~ 0.1-
  0.3 mrad for high-pT tracks.
- **Multiple scattering** in the inner-detector material: sigma_MS
  ~ (13.6 MeV / (beta c p)) sqrt(x / X_0) (1 + 0.038 log(x / X_0))
  where x / X_0 is the integrated radiation length up to the
  measurement point. For H1 inner tracker x / X_0 ~ 0.05-0.10
  (handoff RadLength branch), so:
    - At p = 0.15 GeV, sigma_MS ~ 13.6/150 * sqrt(0.1) ~ 30 mrad ~ 1.7 deg.
    - At p = 1 GeV, sigma_MS ~ 13.6/1000 * sqrt(0.1) ~ 4 mrad ~ 0.25 deg.
    - At p = 3 GeV, sigma_MS ~ 13.6/3000 * sqrt(0.1) ~ 1.4 mrad ~ 0.08 deg.

The matching 2 deg cut is therefore:

- **Loose** at p > 0.5 GeV (~10 sigma_MS).
- **Tight** at p ~ 0.15 GeV (~1.2 sigma_MS).

### Bias implication

At the lowest pT bin the matching efficiency for a genuine pi/K/p can
be artificially low because the reco direction has wandered out of
the 2 deg cone relative to the gen direction. This translates into:

- Lower numerator at low pT.
- Same denominator at low pT.
- Lower acceptance epsilon at low pT.
- HIGHER corrected multiplicity at low pT (because data / epsilon goes
  up when epsilon drops).

This is the standard "matching efficiency mis-modeling" bias. It does
not affect A_h to leading order (it is charge-symmetric: pi+ and pi-
suffer the same multiple scattering at the same momentum). It does
affect absolute multiplicities at the lowest pT bin by a few percent.

### Quantitative check

The handoff Sec. 6B reports pT_h resolution of 0.18-0.39 GeV across
the analysis range. This is the kinematic-projection resolution
(post-boost into photon-axis frame), not the lab-frame angular
resolution. For the actual lab-frame angular resolution check, a
momentum-binned histogram of the matched `MatchAngle_ch` distribution
on the v5b RAPGAP31 MC would reveal whether the 2 deg cut is biting
at low pT.

If the median MatchAngle_ch at pT = 0.15-0.20 GeV exceeds 0.5 deg
(consistent with the 1.7 deg sigma_MS estimate, even when most
candidates are inside the cone), then a fraction of true matches are
being lost at the cone edge. The fraction is bounded above by the
fraction of MatchAngle_ch values that pile up against the 2 deg cut
(visible as a sharp upper edge in the MatchAngle_ch histogram in the
lowest-pT bin).

### Severity

**CONCERN** for absolute pT-differential multiplicity at the lowest
pT bin (pT < 0.25 GeV; a few-percent bias). **INFO** for A_h
(charge-symmetric). **INFO** for higher-pT bins where the cone is
loose.

Mitigation options:

- Replace the fixed 2 deg cut with `max(2 deg, 5 * sigma_MS(p, eta))`
  in a future builder revision.
- Or: store `MatchAngle_ch` (already done as of v5b) and apply a
  momentum-dependent reweighting in the downstream analysis script.
- Or: tighten the cut to 1 deg at high pT and loosen to 5 deg at low
  pT, then quote the difference as a matching-window systematic in
  the `compute_systematics_charged.py` driver.

---

## 13. classifySourceGTR vs gtustin classifier (98.5% agreement)

### MC-physics statement

`ProtonUtils::classifySourceGTR` (`ProtonUtils_lowQ2.h:49-74`) is a
restricted, batch-node-safe version of the full classifier that:

- Checks `GetMother2() != -1` -> returns `kCombinatorial`.
- Inspects only `Mother1` (single mother), not the full mother chain.
- Limits feed-down detection to Sigma0/Sigma*, Xi/Xi*, Omega (the
  hyperon resonance set explicitly enumerated in lines 61-67).

The full gtustin classifier additionally:

- Walks the full mother chain (recursive ancestor lookup), not just
  one level.
- Inspects the STR bank for material-secondary classification (which
  is impossible for GTR particles, but the STR check matters when
  classifying tracks linked to STR rather than to GTR).
- Uses BGcheck enumerations from H1OO's `H1BGcheck` semantics with
  additional ParticleSource values for cascade chains
  (e.g., Xi -> Lambda -> p, where the proton's IMMEDIATE Mother1 is
  Lambda, but the chain origin is Xi).

The handoff Sec. (CONTEXT.md key MC-physics anchors block) cites the
98.5% agreement between the two classifiers. The 1.5% disagreement is
concentrated in:

- Multi-step cascades: `Xi -> Lambda -> p` where Lambda is in STR (so
  the proton has no GTR mother chain). `classifySourceGTR` returns
  `kPrimarySignal` because the immediate Mother1 lookup fails or
  points to a non-feed-down particle. gtustin walks the chain and
  finds the Xi.
- Combinatorial cases where `GetMother2 != -1` is true for a STR
  bookkeeping reason (LEPTO QED-radiation history): `classifySourceGTR`
  returns `kCombinatorial`, gtustin returns `kPrimarySignal`.

### Impact on the matching pool

The matching pool already enforces `GetMother2 == -1`, so the
combinatorial-disagreement class is already excluded from the pool.
The cascade-chain disagreement only affects the assignment of
`ParticleSource_ch` on the OUTPUT side of the matched particle; it
does not change which particles enter the matching pool.

For the analysis script, the `ParticleSource_ch != 4` veto is a no-op
(both classifiers always return non-4 for IsGtr particles). The
`ParticleSource_ch in {0, 1, 2, 3}` acceptance follows the
truth-policy memo (Sec. 4) and includes all primary fragmentation
products including Sigma0/Xi/Omega cascade descendants. The
classifySourceGTR-vs-gtustin disagreement therefore affects only the
breakdown of the numerator across `ParticleSource_ch` categories, not
the total numerator.

### Where it bites

For absolute cross-section, the per-`ParticleSource` breakdown is used
to:

- Assign systematic uncertainties to individual feed-down channels
  (e.g., Sigma0 normalization uncertainty propagates to the cross-
  section error budget).
- Subtract the Xi/Omega feed-down to obtain a "primary-only" cross-
  section if that observable is wanted.

A 1.5% misclassification means the assigned feed-down fraction has a
1.5%-of-the-fraction uncertainty (NOT a 1.5%-of-the-numerator
uncertainty). For Sigma0-feeddown protons at the ~5% level of all
protons, the resulting error budget contribution is ~0.075% of the
proton multiplicity. **Sub-permille.**

### Severity

**INFO**. The matching pool itself is unaffected by the
classifySourceGTR-vs-gtustin disagreement (the disagreement is on the
labelling of the matched particle, not on its presence in the pool).
For absolute cross-section the residual error is sub-permille and
well-below other systematics. The classifySourceGTR choice can be
retained without escalation.

If the final analysis ever switches to a primary-only cross-section
quote (excluding Sigma0/Xi/Omega cascades from the numerator), the
gtustin classifier should be re-evaluated for the full cascade chain.
Until then, classifySourceGTR is adequate.

---

## Summary: severity ratings

| # | Issue | Severity for absolute cross-section |
|---|-------|---|
| 1 | GTR-only pool excludes V0/hyperon daughters | CONCERN (FATAL without explicit V0-daughter subtraction) |
| 2 | isChargedHadron drops Sigma+/- | CONCERN (species definition must be documented; ~0.5% unmatched-fake) |
| 3 | GetMother2 == -1 veto, generator-dependent | INFO/CONCERN (CONCERN if Mother2 population differs >0.5% between RAPGAP31 and DJANGOH14) |
| 4 | Beam protons at GenEta < 3.5 admit window | INFO (geometric overlap zero for |eta_reco|<1.3) |
| 5 | Rad/non-rad matching invariance | INFO (matching is lab-frame; OK) |
| 6 | Response matrix from RAPGAP31 (no QED) vs data (has QED) | FATAL for absolute cross-section if not switched to DJANGOH14 rad or per-bin radiative-corrected R |
| 7 | K+/- and pi+/- DIF | CONCERN (1-3% on K, sub-percent on pi) |
| 8 | STR-only daughter tracks contaminate data numerator | CONCERN (1-2% upward bias on multiplicity without V0 subtraction) |
| 9 | Heavy-flavor (D, B) decays | INFO (correctly handled in GTR by PYTHIA-level decay) |
| 10 | Material-interaction secondaries | CONCERN (0.5-1% unmatched-fake) |
| 11 | Photon conversions / Dalitz | INFO (small, already partly cut) |
| 12 | Fixed 2 deg matching window vs MS | CONCERN at low pT (lowest pT bin can be biased a few percent) |
| 13 | classifySourceGTR vs gtustin (98.5%) | INFO (sub-permille on per-channel feed-down) |

The dominant issues for an absolute charged-h cross-section are
**#1, #6, #8** (V0/hyperon feed-down without explicit subtraction;
response-matrix QED mismodeling; data-side unmatched-fake bias from
STR daughter tracks). Next-tier: **#7, #10, #12**.

---

## MC-physics action items

These are concrete, testable next checks. They are pre-requisites for
calling the matching defensible for an absolute charged-hadron
cross-section.

### Mandatory before absolute cross-section release

- **M1: V0/hyperon daughter subtraction script.** Use the v5b
  `IsV0DaughterGen` branch on `ChargedTotGen` to build a per-bin
  MC-truth V0-daughter fraction f_V0(Q2, x, z, pT_h, charge). Implement
  the subtraction `N_data_corrected = N_data_measured *
  (1 - f_V0(bin))` BEFORE applying the acceptance correction. Assign a
  50% systematic on f_V0 per DESY 20-176 Sys.6 precedent. Owner:
  `h1-acceptance` and `code-editor`. Pre-requisite: confirm the v5b
  builder writes `IsV0DaughterGen` correctly (count the fraction of
  ChargedTotGen entries with the flag set per kinematic bin; expected
  ~0 because the flag is intended for STR daughters that are NOT in
  ChargedTotGen — verify the flag semantics first).

- **M2: Switch response-matrix generator to DJANGOH14 radiative.**
  Build the 4D response matrix R(Q2_reco, x_reco, z_reco, pT_h_reco |
  Q2_gen, x_gen, z_gen, pT_h_gen) from DJANGOH14 radiative MC. Keep
  RAPGAP31 for cross-check and for the `mc_model_swap` systematic.
  Pre-requisite: DJANGOH14 v5b ChargedTotGen completeness verification
  (handoff OQ2 was reported resolved but the response-matrix usage adds
  a new requirement: full GTR/STR consistency at the per-event level,
  not just gen-loop completeness).

- **M3: RAPGAP31 vs DJANGOH14 Mother2 population diagnostic.** Count
  `IsGtr && |PDG| in {211,321,2212} && GetMother2 != -1` as a fraction
  of `IsGtr && |PDG| in {211,321,2212}` in each (Q2, x) cell of v5b
  RAPGAP31 and DJANGOH14 MC. If the absolute difference is above 0.5%
  in any cell, escalate to a new systematic
  `mother2_veto_modeling`. Owner: `h1-mc-expert` + `h1-root-analyst`
  via a one-off macro that reads `H1PartMCArrayPtr` directly (the
  current builder does not store Mother2 on the gen trees; either add
  a diagnostic branch or run the macro standalone).

- **M4: Unmatched-fake budget per kinematic bin.** Construct, on v5b
  MC, the per-bin ratio
  N_unmatched(bin) / N_data(bin) and decompose it into:
    - V0/hyperon daughter contribution (from M1).
    - DIF-fake contribution (from M5 below).
    - Material-secondary contribution (residual).
  This gives the full data-side bias and validates the M1 subtraction.

### Recommended before publication

- **M5: K+/- and pi+/- DIF-fake correction.** Run a one-time MC truth
  study on v5b RAPGAP31 and DJANGOH14: for each GTR K+/- and pi+/-
  passing the gen fiducial, classify whether the GEANT3 simulation
  decayed it inside the CJC volume (radius < 85 cm). Count the
  fraction that produces a matched reco track vs. fraction that does
  not. The latter fraction, after applying BASE_CUT to the daughter
  track, is the DIF-fake contribution. Assign a 30% systematic on the
  correction (consistent with H1 tracking-systematics practice).

- **M6: Sigma+/- inclusion or explicit species systematic.** Either:
  (a) Extend `isChargedHadron` to include Sigma+/- (3222, 3112) in
  both the matching pool AND the gen denominator. This matches the
  DESY 20-176 multiplicity-paper definition and lifts the species
  systematic. Validate by checking that classifySourceGTR and the
  matching pool handle the new species correctly (Sigma+ has a
  one-step decay via PYTHIA, but the parent is in GTR with a finite
  decay vertex). OR
  (b) Quote a `species_definition` systematic equal to the integrated
  Sigma+/- fraction in GTR, with the {pi,K,p} restriction documented
  prominently.

- **M7: Momentum-dependent matching window.** Replace the fixed 2 deg
  cone with `max(2 deg, 5 * sigma_MS(p, eta))` in a future builder
  revision, OR add a downstream MatchAngle_ch reweighting step. Quote
  the difference between fixed-2-deg and momentum-dependent variants
  as a `matching_window` systematic. Particularly important for the
  lowest pT bin where the current cut is at the multiple-scattering
  edge.

- **M8: classifySourceGTR vs gtustin per-bin validation.** Re-run the
  98.5% agreement check on v5b RAPGAP31 and DJANGOH14, broken down by
  ParticleSource category. If any category shows agreement below 95%,
  the per-channel feed-down systematic needs to be revisited. (Likely
  no escalation needed; the 1.5% disagreement is sub-permille on the
  multiplicity error budget.)

### Already-resolved or low-priority (for the record)

- The `GetMother2 == -1` veto itself is correctly applied and the
  population is bounded; only the cross-generator stability check (M3)
  remains.
- The beam-proton admission at `|GenEta| < 3.5` does not bias the
  matching at the current `|eta_reco| < 1.3` analysis fiducial.
- Heavy-flavor decay products are correctly in the matching pool with
  `ParticleSource = 0`.
- Photon conversions and material secondaries are bounded at < 1% and
  fold into the M4 unmatched-fake budget.
- Rad/non-rad matching invariance at the lab-frame angular pairing
  level holds; rad/non-rad differences enter only via the SIDIS
  variables (boost frame), which the current code constructs per-event
  per-sample correctly.

---

## Gaps and risks in this critique

- The Mother2 population fractions in RAPGAP31 vs DJANGOH14 are quoted
  qualitatively; the actual per-bin numbers require running M3.
- The expected Sigma+/- production fraction (~0.5%) is an order-of-
  magnitude estimate from PYTHIA strangeness suppression; the actual
  RAPGAP31/DJANGOH14 numbers may vary by a factor of two depending on
  the JETSET tune. M6's diagnostic count would pin it.
- The K+/- DIF-fake fraction (1-3%) is from H1 tracking lore and
  external SIDIS literature, not a direct measurement on v5b MC. M5
  would pin it for this analysis.
- The IsV0DaughterGen flag was added in v5b but its exact semantic
  (does it flag GTR particles whose STR daughters survive into the
  detector? or does it flag the STR daughters themselves, which are
  NOT in ChargedTotGen?) needs to be verified before M1 can be
  implemented. The handoff Sec. 12 lists it as "flag for gen V0 decay
  daughters" but the gen tree only writes GTR particles, so the flag
  must be on the parent V0 (Lambda, K0_S, ...) indicating its decay
  daughters are real-tracker-visible. This is a SOURCE-CODE question
  that must be resolved before M1 can be written.
- The current critique assumes the v5b matching code is identical to
  the `source_cache/ProtonTreeBuilder_lowQ2.C` lines 1060-1170 read
  above. v5b added several branches but the matching block itself
  may have been minimally modified. The pool semantics
  (`isChargedHadron && IsGtr && GetMother2 == -1 && charge-consistent
  && |eta|<3.5 && pT>0.05 && 2 deg`) are believed unchanged.
- The C_rad infrastructure (whether the response matrix is built from
  RAPGAP31 or DJANGOH14) is inferred from the handoff text, not from
  direct reading of the unfolding script (`tmd_unfold.py` exists per
  handoff Sec. 12 but was not read for this critique). M2 is therefore
  a recommended cross-check rather than a verified gap.

---

## Provenance

- `sidis_lowQ2/context-handoff.md` — Sections 3, 4, 6, 6B, 7, 9, 12, and
  the Session End State (frozen decisions, v5b schema, IsV0DaughterGen,
  98.5% classifier agreement).
- `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` — lines 814-902
  (gen loop) and 1060-1170 (charged matching block).
- `sidis_lowQ2/source_cache/ProtonUtils_lowQ2.h` — full file
  (isChargedHadron, isChargedStable, classifySourceGTR, ParticleSource
  enum).
- `sidis_lowQ2/source_cache/PhysicsConstants_lowQ2.h` line 44
  (CHARGED_MATCH_ANGLE = 2.0 deg).
- `sidis_lowQ2/docs/handoffs/memo-mc-truth-policy-charged-sidis.md` —
  full file (truth-policy decisions, charge-matching mandate,
  ParticleSource semantics).
- `sidis_lowQ2/docs/research/research-background-sources-migration.md`
  — Sec. 2.5, 2.6 (V0, conversion, material backgrounds).
- `h1oo-master/H1Mods/H1PartMC.h` — class definition (fMother2,
  fStrIdx, IsGtr, IsStr declarations).
- `h1oo-master/H1Mods/H1PartMC.C` lines 187-197 (IsGtr/IsStr
  implementation: `fStrIdx != -1`).
- `H1/CONTEXT.md` — MC-physics anchors block (GTR/STR semantics,
  BGcheck/ParticleSource conventions, RAPGAP31/DJANGOH14 QED handling).

---

## Status

This critique is **standalone-complete** for MC-physics review of the
matching pool. It does not modify any code. The list of action items
(M1-M8) defines the work needed to defend the matching for an absolute
charged-hadron cross-section measurement. Of these, **M1, M2, M3, M4
are mandatory** and **M5, M6, M7 are strongly recommended** before
publication. M8 is a low-priority cross-check.

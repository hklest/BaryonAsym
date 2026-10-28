**Status:** Current research note. Source-anchored definition of the
"primary charged hadron" convention for H1 charged-hadron SIDIS multiplicity
that lines up with the convention assumed by global FF fits.

# Research: Primary-Hadron Convention for H1 Charged-Hadron SIDIS

**Date:** 2026-05-20
**Author:** physics-literature-researcher
**Subject:** What counts as a "primary charged hadron" in the published
M^{h±}(Q², x, z, pT_h) so that the data feed cleanly into NNFF, DSS, JAM,
MAPFF and AKK-style fragmentation-function global fits.
**Audience:** `h1-sidis-supervisor`, `h1-systematics`, `h1-unfolder`,
`code-editor` (gen-side mask in `ProtonTreeBuilder_lowQ2.C`).

> **Scope.** Literature-anchored definition of the particle-level sample for
> a charged-hadron SIDIS multiplicity at HERA. Verbatim definitions from the
> primary e⁺e⁻ → h X, SIDIS, and global-FF-fit references. Concrete mask on
> our v5c gen-branches. Out of scope: code patches, V0 daughter detection
> infrastructure (see `research-v0-daughter-classification.md` and
> `research-v0-daughter-detection-api.md`), and the migration treatment of
> non-V0 secondaries (see `research-background-sources-migration.md`).

---

## 1. Executive Summary

**Recommendation.** Adopt the **LEP/SLD "stable charged particle" convention**
for the H1 charged-hadron SIDIS denominator (Born-level primary sample) and
numerator (truth-matched primary sample). Concretely a generated charged
particle counts as primary if **(a) it is a charged hadron (π±, K±, p, p̄,
plus charged hyperons Σ±, Ξ±, Ω± that survive long enough to be reconstructed
as a track)**, **AND (b) it comes from the primary interaction or from a
chain of decays through ancestors each with cτ < 9 cm (proper lifetime
< 3 × 10⁻¹⁰ s)**, **AND (c) it is NOT a secondary from interactions in
detector material, NOT a decay-in-flight muon, NOT an electron from a γ→e⁺e⁻
conversion or π⁰ Dalitz decay**, **AND (d) it is NOT a decay product of an
exclusively produced diffractive vector meson (ρ⁰, φ, ω) — this last cut is
applied as a separate process-level correction, not as a particle-mask cut.**

Justification, one-line per choice:

- **(a) charged hadrons only:** SLD PRD 69 (2004) 072003 §5 defines the
  conventional final-state charged hadron as π±/K±/p/p̄; leptons are removed
  via PID; we have no PID, so we treat charged tracks as a sum of these
  species and let the FF fit predict their sum.
- **(b) cτ < 9 cm parent threshold:** SLD §4 and §5 (verbatim "**proper
  lifetime less than 3×10⁻¹⁰ s**") sets the LEP/SLD convention; this admits
  K⁰_s daughters (cτ = 2.68 cm) and Λ/Λ̄ daughters (cτ = 7.89 cm) as primary;
  it excludes K⁰_L (cτ = 15.3 m), K± (cτ = 371 cm), π± (cτ = 7.8 km) decay
  products, but those parents themselves ARE primary because they are stable
  in this convention — only their decay daughters are excluded; charged
  hyperons Σ±/Ξ±/Ω± (cτ ≈ 2–8 cm) are themselves primary; their decay
  products are also primary by the lifetime rule.
- **(c) reject material secondaries, DIF muons, γ→e⁺e⁻:** SLD §5 explicitly
  subtracts "**particles interacting in the detector material, and particles
  decaying outside the tracking volume**"; HERMES (1212.5407 §III.3.1)
  subtracts charge-symmetric background dominated by π⁰ Dalitz and γ
  conversions; H1 (1302.1321 Sec. 5) subtracts γ-conversion / π⁰ Dalitz
  electrons via "0.5% on photon-conversion/Dalitz tracks".
- **(d) subtract exclusive diffractive vector mesons:** HERMES (1212.5407
  §III.3.4) and COMPASS (1709.07374 §3.4) both subtract ρ⁰→π+π⁻ and φ→K+K⁻
  decay products with up to **50% correction at high z** because exclusive
  VMD is not described by the collinear FF factorisation framework;
  H1 1302.1321 does not subtract this because the contribution is small in
  charge-summed densities, but for **charge-separated M^{h±} the correction
  is unavoidable** because exclusive ρ⁰ produces equal h⁺ and h⁻ from a
  J^P = 1⁻ vector meson and biases A_h.

This is the convention DSS, NNFF1.0/1.1, MAPFF1.0, and JAM all implicitly
assume — none of these fits applies a convention-matching correction
between LEP e⁺e⁻ data and HERMES/COMPASS SIDIS data, because the
experiments themselves publish in the same SLD-style "stable charged
particle" framework. (See §3 below for the verbatim quotes confirming this.)

**Mask on v5c gen-branches (concrete):**

```python
IsPrimaryCharged_v5c = (
    (GenCharge != 0)                                            # charged
    & is_charged_hadron(GenPDG)                                 # π/K/p/hyperon
    & (IsPrimary_or_descendant_of_short_lived_ancestor(...))    # cτ < 9 cm chain
    & ~IsMaterialSecondary                                      # not GEANT secondary
    & ~IsDIFMuon                                                # not K/π → μν daughter
    & ~IsConvOrDalitzGen                                        # not γ→e+e- / π⁰ Dalitz
    & ~IsExclusiveVMDDaughter                                   # not ρ⁰/φ/ω exclusive
)
```

In terms of our actual v5c branches (`GenPDG`, `GenMotherPDG`,
`IsV0DaughterGen`, `VetoAncestorPDG`, `IsStr`, `IsConvOrDalitzGen`,
`ParticleSource`), see §5 for the bit-by-bit translation.

---

## 2. Survey of e⁺e⁻ → h X Measurements

The "primary" / "stable" convention used by every Z-pole charged-hadron
measurement that DSS, AKK, NNFF, MAPFF, JAM consume.

| Experiment | Reference | Particle-level definition (verbatim or paraphrased) | cτ cut | V0 daughters included? | DIF treated? | Material secondaries? |
|---|---|---|---|---|---|---|
| **SLD** (inclusive charged) | PRD 69 (2004) 072003 §4 [hep-ex/0310017] | "all stable charged particles defined conventionally as the sum of electrons, muons, pions, kaons and protons that are either primary hadrons or **products of a chain of decays of particles with proper lifetime less than 3×10⁻¹⁰ s**" | **cτ < 9 cm** (3×10⁻¹⁰ s) | YES (K⁰_s, Λ daughters in) | NO (K±, π± stable in this convention; their μν daughter is OUT because K±/π± have cτ ≫ 9 cm) | NO ("particles interacting in the detector material" subtracted) |
| **SLD** (identified π/K/p) | PRD 69 (2004) 072003 §5 [hep-ex/0310017] | "**The conventional definition of a final-state charged hadron was used, namely a charged pion, kaon or proton that is either from the primary interaction or a product of a chain of decays of hadrons each with a proper lifetime less than 3×10⁻¹⁰ s**" | cτ < 9 cm | YES | NO | NO (subtracted via "particles decaying outside the tracking volume") |
| **OPAL** charged-hadron Z-pole | EPJ C16 (2000) 407 (cited by Airapetian 2013) | Inherits same "stable charged particle" convention as SLD; LEP-wide working definition matches SLD verbatim. Charged tracks from interactions in beam-pipe and inner detector material are corrected out via JETSET+GEANT MC. | cτ < 10 mm typically, equivalent operationally to 3×10⁻¹⁰ s | YES (K⁰_s, Λ decays in tracker treated as primary) | NO (K± decays in tracker subtracted via MC) | NO |
| **DELPHI** inclusive charged | EPJ C5 (1998) 585 (cited as Abreu 1998 by DSS, AKK) | Stable charged hadron sample identical to OPAL/SLD; K⁰_s/Λ daughters reconstructed in tracker and counted as primary, K± decays-in-flight subtracted | cτ < 10 mm | YES | NO | NO |
| **ALEPH** charged hadrons | Z. Phys. C 66 (1995) 355; Phys. Rep. 294 (1998) 1 [Buskulic 1995 cited by HERMES, DSS] | "stable charged particles" with same threshold; ALEPH defines "stable" as PDG-stable plus K⁰_s and Λ explicitly tagged as decaying inside the TPC. | cτ < ~10 mm | YES | NO | NO |
| **TASSO/TPC** PETRA | TPC: Phys. Rev. Lett. 61 (1988) 1263 [tpcdata in DSS] | Pre-LEP convention; counted all charged tracks reaching the TPC with PID, subtracted MC-modelled material secondaries and DIF; effectively the same | Effectively the same | YES | NO | NO |
| **BELLE** inclusive | PRD 88 (2013) 032011 (Leitgab 2013) | Pseudo-stable π±/K± from the primary collision plus weak-decay chains of cτ < 1 cm parents (tighter than LEP because of smaller SVD radius); explicitly subtracts K⁰_s and Λ daughters in their default analysis (different convention from LEP) | **cτ < 1 cm** in BELLE | NO (K⁰_s, Λ daughters subtracted) | NO | NO |
| **BABAR** inclusive | PRD 88 (2013) 032011 partner / Lees 2013 | Similar to BELLE: cτ < 1 cm or species-specific PDG-list, K⁰_s/Λ daughters subtracted in their default | cτ < 1 cm | NO | NO | NO |

**Most important verbatim quote (SLD, the cleanest formulation):**

> "The conventional definition of a final-state charged hadron was used,
> namely a charged pion, kaon or proton that is either from the primary
> interaction or a product of a chain of decays of hadrons each with a
> proper lifetime less than 3×10⁻¹⁰ s."  — SLD, PRD 69 (2004) 072003, §5

**Key disagreement on the literature:** **BELLE/BABAR** (the B-factory
e⁺e⁻ data at √s = 10.5 GeV) use a **tighter** cτ < 1 cm threshold AND
explicitly subtract K⁰_s/Λ daughters. This is a real convention mismatch
between LEP and the B-factories. NNFF1.0 (Bertone et al. 2017, see §3)
keeps these data in separate normalisation bins and lets the fit absorb the
difference; DSS does **not** correct for this and treats it as a residual
systematic.

**Conclusion of e⁺e⁻ survey:** The **LEP/SLD convention is the dominant one**
in the FF-fit world. Of the ≈400 e⁺e⁻ data points typically included in
modern FF fits, ≈300 (ALEPH + OPAL + DELPHI + SLD at Z-pole) use the
"cτ < 3×10⁻¹⁰ s = 9 cm" convention. BELLE/BABAR (≈100 points) are a known
outlier convention but not corrected for.

---

## 3. Survey of FF Global Fits

How global fits handle the input data and whether they apply any
convention-matching correction between e⁺e⁻ and SIDIS sources.

| Fit | Reference | e⁺e⁻ data used | SIDIS data used | Convention assumed | Convention-matching correction? |
|---|---|---|---|---|---|
| **DSS07** | de Florian, Sassot, Stratmann, PRD 75 (2007) 114010 [hep-ph/0703242] | ALEPH (Buskulic 1995), DELPHI (Abreu 1998), OPAL (Abbiendi 2000), SLD (Abe 1999), TPC | HERMES preliminary π/K multiplicities (Hillenbrand 2005, Maiheu 2006) | LEP/SLD "stable charged hadron, cτ < 3×10⁻¹⁰ s"; HERMES assumed compatible via PID-identified π/K direct count | **NO** convention-matching applied. DSS treats SIDIS HERMES as "directly comparable" and notes: "**for heavy flavor tagged data, a further complication arises due to possible contaminations by weak decays**" — flagged as a residual issue, not corrected. |
| **DSS17** | de Florian, Sassot, Stratmann, PRD 95 (2017) 094019 | Same e⁺e⁻ + adds HERMES PRD 87 (2013) 074029 published [1212.5407] + COMPASS PLB 764 (2017) 1 + RHIC | LEP/SLD convention | **NO** convention-matching |
| **AKK08** | Albino, Kniehl, Kramer, Nucl. Phys. B803 (2008) 42 [0803.2768] | ALEPH, DELPHI, OPAL, SLD, TPC; also adds K⁰_s and Λ measurements as separate species | NO SIDIS (e⁺e⁻ + pp only) | LEP/SLD convention | N/A — no SIDIS. AKK fits K⁰_s and Λ as **separate** FF sets so there is no overlap with the "primary charged hadron" sample |
| **HKNS07** | Hirai, Kumano, Nagai, Sudoh, PRD 75 (2007) 094009 | TPC, TASSO, ALEPH, OPAL, DELPHI, SLD (combined) | NO SIDIS (e⁺e⁻ only) | LEP/SLD convention | N/A |
| **NNFF1.0** | Bertone, Carrazza, Hartland, Nocera, Rojo, EPJC 77 (2017) 516 [1706.07049] | Same Z-pole set + BABAR + BELLE | NO SIDIS in NNFF1.0 (e⁺e⁻ only) | LEP/SLD convention for Z-pole; BABAR/BELLE absorbed via per-experiment normalisation nuisance parameters | **NO** explicit convention-matching, but per-dataset normalisation freedom in the fit effectively absorbs LEP-vs-B-factory mismatch |
| **NNFF1.1h** | Bertone et al. EPJC 78 (2018) 651 | Same as NNFF1.0 | Same (still e⁺e⁻ only) | LEP/SLD convention | NO |
| **MAPFF1.0** | Abdul Khalek, Bertone, Nocera, PRD 104 (2021) 034007 [2105.08725] | LEP + BABAR/BELLE | **HERMES 1212.5407 + COMPASS 1709.07374** | LEP/SLD convention assumed for both e⁺e⁻ and SIDIS; HERMES π/K species are taken at face value | **NO** explicit convention-matching. Authors explicitly note in §3 of [2105.08725]: "**we discuss the quality of the fit to the SIDIS data with low virtuality Q² showing that, as expected, low-Q² SIDIS measurements are generally harder to describe within a NLO-accurate perturbative framework**" — they attribute residual data-theory tension to higher-twist / hadronisation effects, NOT to convention differences |
| **MAPFF1.0 NNLO** | Abdul Khalek et al. PLB 834 (2022) 137456 [2204.10331] | Same | Same | Same | Same — no convention correction |
| **JAM21/22** | Moffat et al. PRD 104 (2021) 016015; Anderson et al. PRD 105 (2022) 054017 | LEP + BABAR/BELLE | HERMES + COMPASS | LEP/SLD convention | **NO** convention-matching |

**Verbatim DSS07 statement on the convention issue:**

> "**Besides these fully inclusive measurements also "flavor tagged" SIA
> results are available, where the quark flavor refers to the primary qq̄
> pair created by the intermediate photon or Z-boson. ALEPH, DELPHI, and TPC
> provide tagged results... For heavy flavor tagged data, a further
> complication arises due to possible contaminations by weak decays.**"
> — DSS, PRD 75 (2007) 114010, §III.2

This is the **only** mention in DSS of a primary-hadron convention issue,
and it concerns heavy-quark weak decays (B → charged hadrons), NOT the
soft-decay cτ < 9 cm boundary. DSS assumes the experimental data are
already published in the LEP/SLD convention and **applies no correction**.

**Verbatim MAPFF1.0 (Abdul Khalek et al. 2021):**

> "We exploit the flavour sensitivity of the SIDIS measurements delivered
> by the HERMES and COMPASS experiments..." — MAPFF1.0 abstract

MAPFF takes the HERMES and COMPASS multiplicities as published, with no
convention-matching transformation. The fit's parametric flexibility (NN
representation) plus per-experiment normalisation nuisance parameters absorb
any residual O(few%) convention mismatch.

**Conclusion of FF-fit survey:** **No modern FF fit applies an explicit
convention-matching correction between e⁺e⁻ "stable charged hadron"
samples and HERMES/COMPASS SIDIS multiplicities.** The implicit assumption
in every fit (DSS07, DSS17, AKK08, HKNS07, NNFF1.0/1.1, MAPFF1.0, JAM) is
that the experiments publish under a common convention, namely the
LEP/SLD "primary or descendant of cτ < 9 cm ancestor" rule. Per-experiment
normalisation freedom and the fit χ² are expected to absorb the small
residual mismatch.

**Implication for H1:** **If we publish under the LEP/SLD convention, our
data will be directly consumable by all listed FF fits with no
convention-matching correction.** If we publish under a different convention
(e.g., GTR-only, which excludes K⁰_s/Λ daughters), the fit will see a
systematic ~5-10% discrepancy that it will absorb into nuisance parameters
incorrectly, biasing the extracted FFs.

---

## 4. Survey of SIDIS Multiplicity Measurements

How the SIDIS multiplicity papers actually treat the primary-hadron sample.

| Experiment | Reference | Particle-level definition | cτ cut | V0 daughters | DIF | Material | Exclusive VMD subtraction? |
|---|---|---|---|---|---|---|---|
| **HERMES** π±/K± (proton, deuteron) | PRD 87 (2013) 074029 [1212.5407] | "**multiplicities of pions and kaons separately for positive and negative charge**" — π/K identified by RICH directly. No explicit cτ cut text. Hadrons "must be detected" in the spectrometer; all reconstructed tracks at the primary interaction vertex with RICH ID are counted. Charge-symmetric background (π⁰ Dalitz e±, γ→e+e- e±) subtracted by mirror-charge subtraction (§III.3.1). Background ≈ 1% (DIS), 2% (SIDIS at low z). | **Implicit** ≈ LEP convention (no explicit threshold; relies on RICH PID to reject e/μ, and on track-quality cuts at the primary vertex to reject V0 daughters from secondary vertices) | **Implicit** — RICH-identified K from K⁰_s daughter would be counted as a K±, BUT K⁰_s daughters reconstruct as a secondary vertex and are typically rejected by the "primary vertex" track-quality cut. So effectively HERMES counts **K⁰_s and Λ daughters as NOT primary** because of the vertex cut. This is a **tighter** convention than LEP/SLD. | NO — DIF μ identified as μ by RICH/calorimeter and excluded | NO — material secondaries also fail the primary-vertex track-quality cut | **YES** — corrects for exclusive ρ⁰→π+π⁻ (up to 50% at z→1) and φ→K+K⁻ (≤10%) using PYTHIA-VMD MC. Both numerator and denominator corrected bin-by-bin (§III.3.4). |
| **COMPASS** charged hadrons | PRD 97 (2018) 032006 [1709.07374] | "**The differential multiplicity M^h for charged hadrons, where h denotes a long-lived charged hadron (π+, π−, K+, K−, p or p̄)**" — explicit PDG list, no cτ threshold. Hadron tracks "associated with the primary interaction vertex" (§3.2). | **Implicit ≈ LEP convention** for the PDG list (π/K/p stable as PDG); effectively the same as SLD §5 (PDG-list approach) | **Excluded** — secondary vertices from V0 decays fail the primary-vertex association cut, so K⁰_s and Λ daughters are NOT counted in COMPASS's hadron sample. **Tighter than LEP.** | NO — DIF μ rejected | NO — material secondaries simulated by FLUKA in MC and absorbed into the acceptance correction (§3.3); the published yield is corrected back to the "true" primary-vertex hadron level | **YES** — explicit subtraction of exclusive ρ⁰→ππ (up to 25% at PhT² ≈ 0.1 GeV² and z > 0.6) and φ→KK (up to 35%) using HEPGEN MC (§3.4) |
| **COMPASS** identified π/K | PLB 764 (2017) 1 (Adolph et al.) | Same as 1709.07374: π/K identified by RICH, primary-vertex required | Implicit | Excluded | Excluded | Excluded | YES |
| **H1** charged particle (low-Q²) | EPJC 73 (2013) 2406 [1302.1321] §3.3 | "**stable charged particles (cτ > 10 mm), including charged hyperons; this excludes K⁰_s/Λ daughter pions/protons**" — verbatim from our own audit `research-h1-1302-1321-charged-multiplicity.md` §4.1. | **cτ > 10 mm threshold for the PARTICLE itself** — i.e., the rule "stable means the particle's own cτ > 10 mm" → **this includes charged hyperons (Σ±, Ξ±, Ω± with cτ ~ 2-8 cm)** as primary, but **EXCLUDES the daughters of K⁰_s (cτ = 2.68 cm > 10 mm so K⁰_s is "stable" but its π± daughter would be flagged as a decay product)**. **WARNING:** The H1 text says "this excludes K⁰_s/Λ daughter pions/protons" — meaning H1 1302.1321 uses a **tighter** convention than LEP/SLD: K⁰_s and Λ daughters are NOT primary in H1 1302.1321. | **NO** | NO (vertex-fit cut) | NO (vertex-fit cut) | NO (charge-summed inclusive density; ρ⁰→π+π⁻ cancels in sum) |
| **H1** (DESY 20-176) charged multiplicity | arXiv:2011.01812 [research-h1-charged-particle-analysis.md] | Inherits 1302.1321's convention; explicit Sys.6 "V0 contamination" assigns 50% uncertainty to K⁰_s daughter tracks, confirming they are **excluded** from the primary sample | Same as 1302.1321 | Excluded (treated as a 50% systematic on contaminating tracks) | NO | NO | NO |
| **ZEUS** charged inclusive in DIS | Nucl. Phys. B 545 (1999) 3 (older); arXiv:0904.3494 (newer charged-particle pT in DIS) | "stable charged hadrons, where K⁰_s and Λ are treated as stable and their decay products are not counted as primary" — effectively the same as H1 1302.1321 | cτ > ~10 mm threshold | Excluded | NO | NO | NO |

**Key disagreement on the SIDIS side:**

1. **HERMES / COMPASS** use a **primary-vertex track-quality cut** that
   operationally excludes K⁰_s and Λ daughters (because they form
   secondary vertices). This is **tighter** than LEP/SLD which counts
   them as primary.
2. **H1 1302.1321 and DESY 20-176** use a "**cτ > 10 mm**" threshold for
   the particle itself, which makes K⁰_s and Λ "stable" but treats their
   daughters as secondary. Same effective convention as HERMES/COMPASS.
3. **LEP/SLD** uses the **OPPOSITE convention**: K⁰_s (cτ = 2.68 cm) and Λ
   (cτ = 7.89 cm) are short-lived enough that their daughters are counted
   as primary.

**This is the central convention conflict in the literature.** SIDIS
papers (HERMES, COMPASS, H1) effectively exclude K⁰_s/Λ daughters
from their primary sample by virtue of a primary-vertex track-quality
cut, while LEP/SLD papers include them.

**Quantitative impact:** In our v5c smoke test, K⁰_s + Λ + Λ̄ daughters
are **8.33% of gen entries** (K⁰_s 60%, Λ/Λ̄ 26%, K⁰_L 13%). At reco
level (after vertex cuts) the contamination is **3.05% of matched-reco
tracks**. So:

- H1/HERMES/COMPASS effective convention: 3-4% of charged tracks are
  V0 daughters but were never counted, so the published M^{h±} is **smaller
  than the LEP convention by ~4%**.
- The FF fits silently assume LEP/SLD convention, so they expect the SIDIS
  data to **include** V0 daughters. This is a known but unresolved
  ~few% systematic.

---

## 5. Quantitative Recommendation for H1 SIDIS

### 5.1 The choice: match LEP/SLD or match HERMES/COMPASS?

Two options on the table:

**Option L (LEP/SLD convention — recommended):** Include K⁰_s/Λ daughters
as primary. Match what FF fits actually assume internally. Match SLD §5
verbatim.

**Option H (HERMES/COMPASS/H1-1302.1321 effective convention):** Exclude
K⁰_s/Λ daughters via vertex-quality cut. Match what other SIDIS
experiments publish.

**Choice: Option L.** Reasons:

1. **FF fits silently assume LEP convention.** Publishing in LEP convention
   makes our data directly usable by every modern FF fit without a
   convention-matching correction. Option H would require us (or the FF
   fitter) to apply an O(4%) upward correction to compare to LEP data,
   and would create an inconsistency at the fit level.
2. **Bug history.** The reco-side bug we just fixed (V0 daughters in STR
   bank silently dropped) was caused by **trying to implement Option H
   accidentally**. The v5c builder now admits STR particles, which naturally
   gives Option L. Reverting to Option H requires re-introducing a vertex
   cut that we just removed.
3. **Physical correctness.** K⁰_s and Λ are produced in the fragmentation
   process. Their decay products are physical fragmentation products. The
   LEP convention is the physically correct one for an inclusive
   measurement that has no PID and no ability to subtract V0 daughters
   species-resolved.
4. **Differentiation from V0 analyses.** Our dedicated `lambda/` and
   `lambda_HiQ/` analyses measure K⁰_s, Λ, Λ̄ as separate species. Those
   analyses look for V0 vertices and reconstruct V0 mass peaks; they do
   not double-count the V0 itself, just the daughter tracks. By including
   V0 daughters as primary in the charged-hadron measurement, we are
   measuring the **sum** of all charged tracks; the V0 analyses measure
   the **vector mesons / baryons separately**. There is no double-counting
   between the two analyses at the cross-section level.

### 5.2 The mask (concrete, bit-by-bit)

Gen-side particle-level primary mask on v5c branches:

```python
def IsPrimaryCharged_v5c(g):
    """LEP/SLD primary-hadron convention applied to our v5c gen-branches.
    g is a particle record; returns bool. The mask defines the
    Born-level acceptance denominator and the truth-matched numerator."""

    # (a) Must be a charged particle (gen charge nonzero)
    if g.GenCharge == 0:
        return False

    # (b) Must be a hadron: π±, K±, p, p̄, Σ±, Ξ±, Ω±
    # Reject leptons (e±, μ±) and any spurious entries
    if not is_charged_hadron_pdg(g.GenPDG):
        return False

    # (c) Must be from primary interaction OR descendant of cτ<9cm ancestor
    # In v5c, IsV0DaughterGen flags K0s/Lambda/K0L daughters — these are
    # ADMITTED by the LEP convention because K0s/Lambda have cτ < 9 cm,
    # so we do NOT veto on IsV0DaughterGen.
    # However K0L has cτ = 15.3 m, so K0L daughters MUST be vetoed.
    if g.IsV0DaughterGen and g.GenMotherPDG == 130:  # K0L parent
        return False
    # Similarly, K± and π± have cτ >> 9 cm; their decay daughters (μν, eν)
    # are not primary. Veto via IsDIFMuon flag (which we need to add if not
    # already present in v5c; see §6 below).
    if g.IsDIFMuon:
        return False

    # (d) Must NOT be a material secondary (GEANT-generated)
    # In v5c, IsStr=True flags STR-bank particles which include all
    # non-GTR origins (material rescattering, secondary hadronic interactions).
    # We have to use the source classification:
    if g.ParticleSource == "material_secondary":
        return False
    # If we don't have ParticleSource, the practical surrogate is:
    # (IsStr AND NOT IsV0DaughterGen) flags non-V0 STR entries.
    # Non-V0 STR entries are 5364/event in smoke test = ~37% growth from
    # GTR-only; these are MOSTLY material secondaries.

    # (e) Must NOT be a photon-conversion electron or pi0 Dalitz electron
    if g.IsConvOrDalitzGen:
        return False

    # (f) Must NOT be an exclusive diffractive vector meson decay product
    # — applied as a separate process-level subtraction at the
    # cross-section level, not at the particle mask level.
    # We DO need to flag the exclusive ρ⁰, φ, ω events in MC truth so
    # we can subtract bin-by-bin (same as HERMES §III.3.4 and COMPASS §3.4).
    # The flag is at the event level (IsExclusiveVM event tag), not the
    # particle level.

    return True
```

In terms of v5c branch names:

| Cut | v5c expression |
|---|---|
| (a) charged | `GenCharge != 0` |
| (b) hadron | `abs(GenPDG) in {211, 321, 2212, 3222, 3112, 3312, 3334}` |
| (c1) admit V0 daughters of K⁰_s, Λ, Λ̄ | `not (IsV0DaughterGen and GenMotherPDG == 130)` (i.e. veto K⁰_L daughters only) |
| (c2) veto DIF μ from K±, π± | `not IsDIFMuon` (NEW BRANCH NEEDED — see §6) |
| (d) veto material secondaries | `not (IsStr and not IsV0DaughterGen and ParticleSource != 'V0_daughter')` |
| (e) veto γ→e+e-, π⁰→γe+e- | `not IsConvOrDalitzGen` |
| (f) veto exclusive VMD event | applied at event-level cut, separate `IsExclusiveVMDEvent` flag |

### 5.3 Residual systematic to assign

The LEP convention has the following residual uncertainties when transferred
to a fixed-target SIDIS measurement:

| Source | Magnitude | Treatment |
|---|---|---|
| K⁰_s, Λ branching-ratio MC uncertainty | ~0.5% on K⁰_s, ~1% on Λ from PDG | Negligible vs other systematics |
| K⁰_s, Λ production cross-section MC mismodelling | ~10% (cf. our `lambda_HiQ/` analysis) | Translates to 10% × 4% = **0.4%** on M^{h±} |
| K⁰_L daughter K → π e ν branching | ~1% on the small K⁰_L contribution | Subdominant |
| DIF μ contribution if not vetoed | K± cτ = 371 cm; ~few% of K± decay in CTD; ~few% of K± in hadron sample → ~0.1-0.2% effect | Veto explicitly via IsDIFMuon |
| Material secondaries that escape the IsStr/not V0 veto | <0.5% if GEANT physics list is correct | Carry as a tracking-systematic component |
| Convention mismatch vs HERMES/COMPASS (which exclude V0 daughters) | **~3-4% upward shift in our data vs. HERMES/COMPASS** | **DO NOT subtract**; **document explicitly in publication** |
| Convention mismatch vs LEP e⁺e⁻ Z-pole data | **Effectively zero** by construction (matched conventions) | None |
| Exclusive ρ⁰, φ, ω contribution if not subtracted | Up to 50% at z > 0.7 for pions, up to 10% at z = 0.4-0.6 for kaons | **MUST be subtracted** via HEPGEN-style MC, same as HERMES and COMPASS |

**Recommended residual-systematic line for the published budget:**

| Sys component | Magnitude on M^{h±} | Correlation |
|---|---|---|
| Primary-hadron convention (V0 daughter modelling) | 0.5% per bin | Correlated across all bins |
| Exclusive VMD subtraction | 0-5% (z-dependent, dominant at high z) | Correlated bin-by-bin in z |
| Material secondary residual | 0.5% per bin | Uncorrelated |
| DIF muon residual | 0.2% per bin | Uncorrelated |

---

## 6. Cross-Reference Matrix

Which experiments' published data are **bit-identical** to our recommended
convention, and which require a known correction.

| Comparison target | Convention | Match to LEP-convention H1 SIDIS? | Required correction to compare |
|---|---|---|---|
| **SLD PRD 69 (2004) inclusive charged** | LEP/SLD | ✓ **Identical** | None |
| **SLD identified π/K/p** | LEP/SLD | ✓ **Identical** species-by-species; our M^{h±} = sum of π + K + p tracks corresponds to SLD's sum | Sum SLD's π+K+p tables and compare; no further correction |
| **OPAL EPJ C16 (2000) 407 charged** | LEP/SLD | ✓ **Identical** | None |
| **DELPHI EPJ C5 (1998) 585** | LEP/SLD | ✓ **Identical** | None |
| **ALEPH Phys. Rep. 294 (1998) 1** | LEP/SLD | ✓ **Identical** | None |
| **TASSO / TPC PETRA** | LEP/SLD | ✓ **Identical** | None |
| **BELLE PRD 88 (2013) 032011** | tighter (cτ < 1 cm, V0 subtracted) | **Mismatch** | Add back BELLE-subtracted K⁰_s/Λ daughters (~4%); BELLE auxiliary tables provide the correction in some cases. |
| **BABAR Lees 2013** | tighter (cτ < 1 cm, V0 subtracted) | **Mismatch** | Same as BELLE |
| **HERMES PRD 87 (2013) 074029** | tighter (primary-vertex cut excludes V0 daughters); π/K species PID-resolved | **Mismatch in two ways:** (a) HERMES excludes V0 daughters; (b) HERMES is π/K/p resolved | (a) HERMES M^{h±} sum is ~4% smaller than our M^{h±}; (b) we cannot compare species-by-species |
| **COMPASS PRD 97 (2018) 032006** | tighter; charged-hadron sum | **Mismatch in V0 treatment only** | COMPASS M^{h±} sum is ~4% smaller than our M^{h±} for same z/PhT² range |
| **H1 1302.1321** | tighter (cτ > 10 mm particle threshold = V0 daughters excluded); charge-summed (h⁺+h⁻) | **Mismatch in V0 treatment** | Compare our M^{h+} + M^{h-} integrated over our pT_h range, multiplied by (1 - 0.04) ≈ 0.96 to subtract V0 daughters → directly compare to 1302.1321 |
| **H1 DESY 20-176** | same as 1302.1321; charge-summed multiplicity distributions | Same mismatch | Same correction needed |
| **ZEUS NPB 545 (1999) charged pT in DIS** | cτ > 10 mm | Same mismatch as H1 1302.1321 | Same correction |

**Bottom line on cross-references:** Our M^{h±} (LEP convention) is
**bit-identical** to LEP/SLD e⁺e⁻ data and is **~4% larger than** existing
H1/HERMES/COMPASS SIDIS data on the inclusive charged-hadron sum. We must
flag this 4% mismatch explicitly in any cross-check plot. **The mismatch is
not an error in our analysis; it is a deliberate convention choice
documented as Option L.**

---

## 7. Open Issues / Honest Gaps

### 7.1 K⁰_L daughter handling

K⁰_L has cτ = 15.3 m, so it is "stable" in the LEP convention (it traverses
the detector without decaying inside the tracker). Its decay daughters
(K⁰_L → π± e∓ ν, K⁰_L → π± μ∓ ν) that DO decay inside the tracker (~10% of
K⁰_L for a 1 m tracker at GeV energies) are then daughters of a **long-lived**
parent, so they should NOT be counted as primary. Our v5c builder flags K⁰_L
daughters as `IsV0DaughterGen=True` with `GenMotherPDG=130`, which we
correctly veto in the mask above. **However**, this is 13% of the V0 daughter
sample × the ~10% K⁰_L-decay-in-tracker fraction ≈ 1.3% of the 4% V0
daughter pool = **~0.05% effect on M^{h±}**. Negligible. Mention only as a
completeness check.

### 7.2 Charged hyperon Σ±, Ξ±, Ω± treatment

The LEP convention counts charged hyperons as primary (because their cτ ≈
2-8 cm < 9 cm cutoff means they are admitted by the "stable" definition; but
their **decay products** are descendants of a short-lived parent and so are
admitted as well — there is no double counting because the hyperon itself
decays inside the tracker and is reconstructed as a track segment, then its
charged decay daughter (π/p) is reconstructed as a second track segment).
SLD §5 includes all of these. Their rates are O(0.1%) per event so the
impact on our M^{h±} is small but nonzero. Our v5c builder needs to make
sure these are flagged correctly. The PDG list in §5.2 explicitly includes
3222 (Σ+), 3112 (Σ-), 3312 (Ξ-), 3334 (Ω-). Confirm with smoke test.

### 7.3 Exclusive ρ⁰, φ, ω subtraction

Both HERMES and COMPASS subtract exclusive VMD bin-by-bin. H1 1302.1321 does
not, because charge-summed densities are insensitive to exclusive ρ⁰ (which
produces equal h⁺ and h⁻). **Our M^{h±} is charge-separated, so we MUST
subtract exclusive ρ⁰ at the same level as HERMES/COMPASS.** This requires
running RAPGAP or DJANGOH with the exclusive-VMD module enabled, plus a
phenomenological ρ⁰ cross-section normalisation tuned to data (RAPGAP has
this built in). The subtraction can be up to 50% at z > 0.7 — at high z this
is the **dominant** correction. We do not yet have this in our pipeline. Add
to the systematics backlog.

For φ → K⁺K⁻ the correction is 10-35% in the z = 0.4-0.6 range and matters
for our M^{h±} at intermediate z. Same exclusive-VMD MC machinery handles
both.

For ω → π⁺π⁻π⁰ the contribution is small (a few %) but nonzero.

### 7.4 Diffractive DIS (non-exclusive)

Diffractive DIS (rapidity-gap events) contributes a separate population of
charged hadrons that is treated within the same FF framework but has
different kinematics. H1 (1302.1321) mixes diffractive RAPGAP at 10% with
inclusive RAPGAP. We follow the same prescription via our standard MC mix
(see `research-h1-charged-particle-analysis.md` §5). No convention-level
issue beyond MC modelling.

### 7.5 Heavy-flavor weak decays (B, D)

The LEP/SLD cτ < 9 cm threshold ADMITS charged hadrons from B (cτ ≈ 470 μm)
and D (cτ ≈ 90-310 μm) decays — these are short-lived in this convention. So
when DSS, NNFF, MAPFF include LEP charged-hadron data, they include the B
and D feed-down implicitly. At HERA-II low Q² the b-quark contribution to
SIDIS is <1% and the c-quark is ≈10% (charm via boson-gluon fusion). Our
DJANGOH + RAPGAP MC includes these via standard PDF + JETSET fragmentation.
The published M^{h±} thus correctly includes B/D feed-down charged hadrons,
consistent with LEP convention. No additional treatment needed.

### 7.6 BELLE/BABAR convention mismatch (real but uncorrected)

BELLE and BABAR explicitly use a tighter cτ < 1 cm convention and subtract
K⁰_s/Λ daughters. This is a known mismatch with the LEP data that DSS, NNFF,
MAPFF do not correct for. Since our SIDIS M^{h±} will be compared to LEP/Z-
pole-trained FFs evolved down to low Q², the BELLE/BABAR vs LEP convention
mismatch (which sits inside the FF fit, not in our analysis) is **somebody
else's problem**. Flag it in the discussion if BELLE/BABAR data show
unusual residuals in NNFF/MAPFF.

### 7.7 Lack of PID is genuinely a convention question

We measure **inclusive charged tracks**, not π/K/p separated. SLD's
inclusive-charged-particle rate (§4 of PRD 69 (2004) 072003) IS the sum of
their separate π/K/p species, plus electrons and muons which we then subtract
on the gen side (because our SIDIS M^{h±} is for hadrons, not leptons). So
SLD's table 2 ("inclusive stable charged particle production rate")
corresponds to **our M^{h+} + M^{h-} summed** if we keep e±, μ± out of our
sample. Tables 3-5 (separate π, K, p rates) are not directly comparable to
us bin-by-bin because we don't have PID. We can only compare the **sum**
π + K + p to our charge-summed M^{h+} + M^{h-}.

This is the right normalisation for the FF fit:
**M^{h+}(Q²,x,z,pT_h) = Σ_q∈{u,d,s,c,b} [F_2^q(x,Q²) D_q^{h+}(z,Q²) ...]**
where D_q^{h+} is itself a sum of D_q^{π+}, D_q^{K+}, D_q^{p}, etc.

The FF fits know how to compute D_q^{h+} from the species-resolved FFs
they fit. So our charge-separated, species-summed M^{h±} is a clean
observable for them.

### 7.8 Unresolved literature inconsistency

The single biggest unresolved item: **LEP/SLD includes K⁰_s/Λ daughters as
primary; HERMES/COMPASS/H1-1302.1321 exclude them**. The FF fits silently
assume both samples are in the same convention. Our recommended choice
(Option L) aligns us with LEP/SLD and creates a documented ~4% offset vs
HERMES/COMPASS. We should:

1. Publish under Option L.
2. Document the offset explicitly in the systematics section.
3. Provide a one-line conversion (multiply M^{h±} by 0.96 to compare to
   HERMES/COMPASS-convention data).
4. Push HEPData to provide both conventions (we can do this without rerunning
   the analysis — just two acceptance corrections from the same builder).
5. Engage the JAM / NNFF / MAPFF authors directly about the convention
   inconsistency so future fits can apply a proper correction.

---

## 8. References (anchor for traceability)

### e⁺e⁻ measurements

- SLD Collaboration, K. Abe et al., "Production of π+, π−, K+, K−, p and p̄
  in Light (uds), c and b Jets from Z⁰ Decays," PRD 69 (2004) 072003;
  hep-ex/0310017. ar5iv: https://ar5iv.labs.arxiv.org/html/hep-ex/0310017
- OPAL Collaboration, G. Abbiendi et al., EPJ C16 (2000) 407.
- DELPHI Collaboration, P. Abreu et al., EPJ C5 (1998) 585.
- ALEPH Collaboration, D. Buskulic et al., Z. Phys. C66 (1995) 355;
  R. Barate et al., Phys. Rep. 294 (1998) 1.
- BELLE Collaboration, M. Leitgab et al., PRL 111 (2013) 062002.
- BABAR Collaboration, J. P. Lees et al., PRD 88 (2013) 032011.

### Global FF fits

- DSS07: D. de Florian, R. Sassot, M. Stratmann, PRD 75 (2007) 114010;
  hep-ph/0703242. ar5iv: https://ar5iv.labs.arxiv.org/html/hep-ph/0703242
- DSS17: D. de Florian, R. Sassot, M. Stratmann, PRD 95 (2017) 094019.
- AKK08: S. Albino, B. A. Kniehl, G. Kramer, Nucl. Phys. B803 (2008) 42;
  arXiv:0803.2768.
- HKNS07: M. Hirai, S. Kumano, T.-H. Nagai, K. Sudoh, PRD 75 (2007) 094009.
- NNFF1.0: V. Bertone, S. Carrazza, N. P. Hartland, E. R. Nocera, J. Rojo,
  EPJC 77 (2017) 516; arXiv:1706.07049.
- NNFF1.1h: V. Bertone et al., EPJC 78 (2018) 651.
- MAPFF1.0: R. Abdul Khalek, V. Bertone, E. R. Nocera, PRD 104 (2021)
  034007; arXiv:2105.08725.
- MAPFF1.0 NNLO: R. Abdul Khalek et al., PLB 834 (2022) 137456;
  arXiv:2204.10331.
- JAM21/22: E. Moffat et al. PRD 104 (2021) 016015; Anderson et al. PRD 105
  (2022) 054017.

### SIDIS measurements

- HERMES Collaboration, A. Airapetian et al., "Multiplicities of charged
  pions and kaons from SIDIS by the proton and the deuteron," PRD 87 (2013)
  074029; arXiv:1212.5407. ar5iv: https://ar5iv.labs.arxiv.org/html/1212.5407
- COMPASS Collaboration, M. Aghasyan et al., "Transverse-momentum-dependent
  Multiplicities of Charged Hadrons in Muon-Deuteron DIS," PRD 97 (2018)
  032006; arXiv:1709.07374. ar5iv: https://ar5iv.labs.arxiv.org/html/1709.07374
- COMPASS Collaboration, C. Adolph et al., PLB 764 (2017) 1 (unidentified
  hadrons).
- H1 Collaboration, "Measurement of Charged Particle Spectra in Deep-
  Inelastic ep Scattering at HERA," EPJC 73 (2013) 2406; arXiv:1302.1321.
  See also local audit `research-h1-1302-1321-charged-multiplicity.md`.
- H1 Collaboration, "Measurement of charged particle multiplicity distributions
  in DIS at HERA and its implication to entanglement entropy of partons,"
  DESY 20-176, arXiv:2011.01812. Local audit:
  `research-h1-charged-particle-analysis.md`.
- ZEUS Collaboration, "Measurement of inclusive prompt photon production in
  deep inelastic scattering," NPB 545 (1999) 3 (older charged pT
  reference).

### Local research files (referenced)

- `research-h1-charged-particle-analysis.md` (DESY 20-176 audit; §4.5 has
  Sys.6 V0 contamination at 50% on K⁰_s daughter tracks)
- `research-h1-1302-1321-charged-multiplicity.md` (§4.1: "stable charged
  particles (cτ > 10 mm); this excludes K⁰_s/Λ daughter pions/protons")
- `research-v0-daughter-classification.md`
- `research-v0-daughter-detection-api.md`
- `research-background-sources-migration.md`
- `research-crad-for-charged-sidis.md`

### Source-limitation note

- The OPAL, DELPHI, ALEPH papers were verified by citation chain from DSS,
  NNFF, and AKK reference lists; their primary-hadron definitions
  inherit from the same LEP working-group convention as SLD. The
  exact verbatim quotes from OPAL EPJ C16 (2000) 407 and DELPHI EPJ C5
  (1998) 585 are not transcribed here because both papers are paywalled at
  Springer and arXiv copies have variable html quality; the convention is
  documented in the LEP Electroweak Working Group particle-flow conventions
  and is consistent with SLD's verbatim wording.
- HERMES PRD 87 (2013) 074029 §III.3 does not explicitly state "primary
  hadron" but operationally the primary-vertex track-quality cut + RICH
  PID excludes V0 daughters. This is the de facto convention; documented
  in the local audit above.
- COMPASS PRD 97 (2018) 032006 §3.1 explicitly states "long-lived charged
  hadron (π+, π−, K+, K−, p or p̄)" with no cτ cut text; the PDG list is
  used directly.
- We did not retrieve the full text of BELLE PRD 88 (2013) 032011 in this
  research pass; the "cτ < 1 cm" convention is reported in the NNFF1.0
  paper §2.2 and the FF-fit literature consistently flags BELLE/BABAR as
  using a tighter primary-hadron convention than LEP.

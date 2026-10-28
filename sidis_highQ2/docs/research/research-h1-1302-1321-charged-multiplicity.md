**Status:** Current research note. New literature-grounding document for the frozen low-Q2 event and track-selection choices.

# Research: H1 Charged Particle Spectra in Low-Q² DIS (arXiv:1302.1321)

**Date:** 2026-05-16
**Author:** physics-literature-researcher
**Paper:** H1 Collaboration, "Measurement of Charged Particle Spectra in
Deep-Inelastic ep Scattering at HERA"
**Identifier:** arXiv:1302.1321 [hep-ex], DESY 13-012, submitted to EPJC
(published as Eur. Phys. J. C 73 (2013) 2406)
**Source surface used:** ar5iv HTML (`https://ar5iv.labs.arxiv.org/html/1302.1321`),
metadata cross-checked against `https://arxiv.org/abs/1302.1321`.
**Audience:** SIDIS low-Q² and high-Q² analysis teams; consumed by
`h1-sidis-supervisor`, `h1-systematics`, `h1-unfolder`, and `code-editor`.

> **Scope.** This report extracts what the 2013 H1 charged-particle spectra
> paper measures, how it does it, and what aspects are directly transferable
> to our charged-hadron SIDIS multiplicity / charge-asymmetry / azimuthal-
> modulation analyses on the 2005-2007 HERA-II dataset. The paper is the
> primary published reference for low-Q² inclusive charged-particle tracking
> at H1 within the kinematic region most of our low-Q² analysis sits inside.
> It also predates and explicitly motivates DESY 20-176 (`arXiv:2011.01812`),
> which is treated in `research-h1-charged-particle-analysis.md` and is not
> re-derived here. Both should be read together: 1302.1321 is the spectra
> companion (CTD+FTD, charge-summed multiplicities), DESY 20-176 is the
> multiplicity-distribution and entanglement-entropy follow-up. Our SIDIS
> measurement extends both into pT_h, z, phi_h, and charge separation.

---

## 1. Quick Summary

| Field | Value |
|---|---|
| Dataset | 2006 e+p, HERA-II, single year |
| Beams | E_e = 27.6 GeV, E_p = 920 GeV, sqrt(s) = 319 GeV |
| Integrated lumi | 88.6 pb^-1 (≈30% of our 2005-2007 sample, 297.7 pb^-1) |
| Q² range | 5 < Q² < 100 GeV² (12 GeV² avg geometry of SpaCal acceptance) |
| x range | 1e-4 < x < 1e-2 |
| y range | 0.05 < y < 0.6 |
| Frame | Hadronic centre-of-mass (HCM, virtual γ-p rest frame) |
| Observables | (1/N) dn/dη*, (1/N) dn/dpT*, η* in [0,5], pT* in [0,10] GeV |
| Differential bins | 8 (x,Q²) intervals (see Sec. 5 of this report) |
| Track types | CTD central + CTD/FTD combined |
| Track η (lab) | -2 < η < 2.5 |
| Track pT cut | central > 150 MeV (lab), combined p > 0.5 GeV |
| Correction | Bin-by-bin (Djangoh nominal, Rapgap alt = model systematic) |
| Default MC | Djangoh (CDM/Ariadne, CTEQ6L LO PDF, Heracles for radiative effects) |
| Total syst. | < 2.5 % for single-differential bins; up to 8% in marginal (x,Q²) bins |
| Charge separation | None (charge-summed multiplicities only) |

The headline physics finding is that **Djangoh (Colour Dipole Model) provides
the best, but still imperfect, description of charged-particle densities in
the low-Q², low-x DIS regime**. Rapgap (DGLAP+PS) severely underdescribes the
data at high pT* and small η* (current/central region overlap), while Cascade
(CCFM) overshoots over most of phase space. Herwig++ undershoots everywhere.
This is the H1 paper to cite for "low-Q² DIS DGLAP-shower MC is broken at high
pT*", which is a direct input to MC systematic choices in our analysis.

---

## 2. DIS Event Selection

Reconstructed from Section 3.2 (eΣ method) and 3.3 (data selection) of the
paper. Trigger details from Section 3.3.

| Cut | Value | Notes |
|---|---|---|
| Trigger | SpaCal EM cluster triggers | Inefficiency "negligible" in analysis phase space |
| Scattered e' energy | E'_e > 12 GeV | Defines the most energetic SpaCal cluster as the lepton candidate |
| Q² range | 5 < Q² < 100 GeV² | Set by SpaCal geometric acceptance |
| y range | 0.05 < y < 0.6 | Upper cut suppresses photoproduction; lower cut from resolution |
| x range | 10^-4 < x < 10^-2 | Derived from Q², y, s |
| Kinematic method | eΣ-method (eq. 1) | Combines scattered e' info with Σ = Σ(E_i - p_z,i) over HFS; optimal kinematic resolution + low sensitivity to QED ISR |
| z-vertex | |z_vtx - z_nom| < 35 cm | Standard non-ep / cosmic / beam-gas rejection |
| Empz | 35 < Σ(E_i - p_z,i) < 75 GeV | Includes both HFS particles AND the scattered electron (full sum). Suppresses ISR and reduces γp background to ≈ 0.5 % |
| Residual γp | ≈ 0.5 % | After Empz cut; flagged separately as a syst |

**Notes for our analysis.**

- The trigger choice (SpaCal EM cluster) is correct for the low-Q² region
  where the scattered electron lands in the SpaCal. We use the same family
  of triggers (S0, S2, S3) in the low-Q² SIDIS analysis. No new trigger
  efficiency study is implied by this paper; they take it as 100 % and put
  no syst on it.
- E'_e > 12 GeV is a hard cut. Some H1 low-Q² analyses use 11 GeV. The
  12 GeV value matches the F2 measurements and HERAPDF inputs and is a safe
  default; going below 12 GeV introduces γp contamination and SpaCal
  triggering inefficiencies that they did not have to worry about with this
  cut.
- The **Empz window** is on the **full sum including the electron**, not on
  the HFS alone. Our `ProtonTreeBuilder_lowQ2` builders must use the same
  convention — confirm in code review (see also
  `research-h1-charged-particle-analysis.md` Sec. 4 for DESY 20-176 cuts).
- y > 0.05 is non-negotiable in this regime — below that the eΣ-method
  resolution degrades. Our SIDIS bin edges should not cross y = 0.05 from
  below in any (x, Q²) cell.

---

## 3. Track Selection

Reconstructed from Section 3.3 and from Table 1.

### 3.1 Track collections used

| Collection | Polar angle | Quality |
|---|---|---|
| Central (CTD) | 20° < θ < 165° (η ≈ [-1.74, +1.74]) | Vertex-fitted, full CTD reconstruction |
| Combined (CTD+FTD) | Reconstructed from short CTD segments + FTD hits in overlap region 6° < θ < 25° (η ≈ +1.5 ... +2.95) | Extends forward acceptance |

Both collections are required to:
- Originate from the primary event vertex (suppresses K0s, Λ, photon
  conversion, hadronic interaction secondaries).
- Be linked to silicon-vertex hits (CST/BST/FST) where available, to improve
  primary-vertex spatial resolution.
- Pass unspecified "further quality cuts" cited as ref [41] (PhD thesis,
  inaccessible via this read). DESY 20-176 (see related research file)
  defines these explicitly: number of CTD hits ≥ 10, DCA, fit chi²/ndf,
  start radius, length-in-r constraints. The 2013 paper inherits these.

### 3.2 Kinematic cuts on tracks

| Cut | Central tracks | Combined tracks |
|---|---|---|
| Transverse momentum (lab) | pT > 150 MeV | p > 500 MeV (i.e. cut is on total momentum, not pT; this guarantees the track has enough momentum to cross the CJC end-wall and reach the FTD) |
| Lab pseudorapidity | -2 < η_lab < 2.5 | -2 < η_lab < 2.5 (forward end fixed by FTD geometry) |
| HCM pseudorapidity | 0 < η* < 5 | same |
| HCM pT | 0 < pT* < 10 GeV | same |

### 3.3 Detector resolution quoted in the paper

| Quantity | Resolution |
|---|---|
| CTD track pT | σ(pT)/pT ≈ 0.2 % × pT/GeV ⊕ 1.5 % |
| SpaCal EM energy (e') | σ(E)/E ≈ 7.1 %/√(E/GeV) ⊕ 1 % |
| LAr hadronic energy | σ(E)/E ≈ 50 %/√(E/GeV) ⊕ 2 % |
| SpaCal angular | 1 mrad (used as the syst variation in this analysis) |

---

## 4. Observable Definitions

Section 3.4.

### 4.1 Densities, not multiplicities

The published observables are normalised per DIS event but **not** integrated
over an extended kinematic variable beyond η* or pT*:

```
(1/N_DIS) dn / dη*       (charged-particle pseudorapidity density)
(1/N_DIS) dn / dpT*      (charged-particle transverse momentum density)
```

dn is the count of stable charged particles (cτ > 10 mm, including charged
hyperons; this excludes K0s/Λ daughter pions/protons), and N_DIS is the
number of DIS events selected in the same (x, Q²) bin.

### 4.2 Comparison to TMD multiplicities M^{h±}

Our M^{h±}(Q², x, z, pT_h) is conceptually similar (dN_h / N_DIS per
phase-space cell), but:

- **Per charge sign**, not summed.
- Differential in `z = P·p_h / (P·q)` and `pT_h` (HCM, not lab).
- We typically restrict to the current hemisphere (η* > 0) or apply a z > 0.2
  cut to enforce current fragmentation. 1302.1321 takes ALL particles in
  0 < η* < 5 with no z cut.
- We integrate over (Q², x) only at the binned-cross-section level. They
  define the spectra differentially in (Q², x) directly.

The acceptance correction logic, however, is essentially the same: yields per
bin divided by selected DIS events, corrected per bin for detector effects.
1302.1321 just does the simpler 2D (η*, pT*) version.

### 4.3 "Central" vs "current" naming convention — WARNING

Section 3.4 defines two HCM regions and uses non-standard names:

| Region (this paper) | η* range | Conventional name in our group |
|---|---|---|
| "central" | 0 < η* < 1.5 | This is the **target side relative to current**, what we usually call the **proton-direction / central region** |
| "current" | 1.5 < η* < 5 | What we usually call the **photon-direction / current region** |

This is the H1 convention in low-x studies: "central" means central in
the lab (low |η_lab|) which maps to low η* in HCM at low x. **DO NOT
confuse this with the Breit-frame current/target hemispheres** of our TMD
work. The 1302.1321 "current" region maps to where the highest-z, hardest
particles concentrate in low-x DIS — i.e., toward the proton-remnant /
forward direction in the lab. Add a note to any direct M^{h} comparison.

### 4.4 Frame

HCM only. Boost is reconstructed from Q² and y (eq. before sec. 3.3); for
us, exactly the same boost is computed in `H1PartHCMS` from the H1OO
electron and proton four-vectors. The Breit frame, the natural TMD frame,
is not used in 1302.1321.

---

## 5. Differential Binning

From Tables 4-9, the 8 (x, Q²) bins are:

| Q² bin (GeV²) | x bin | n_eta_bins (in pT*<1) | n_eta_bins (in 1<pT*<10) |
|---|---|---|---|
| 5 - 10 | 0.0001 - 0.00024 | 7 (0..3.7 in 0.5 + 0.7 + 1.3 chunks) | same |
| 5 - 10 | 0.00024 - 0.0005 | 6 | 6 |
| 5 - 10 | 0.0005 - 0.002 | 5 | 5 |
| 10 - 20 | 0.0002 - 0.00052 | 7 | 7 |
| 10 - 20 | 0.00052 - 0.0011 | 6 | 6 |
| 10 - 20 | 0.0011 - 0.0037 | 5 | 5 |
| 20 - 100 | 0.0004 - 0.0017 | 7 | 7 |
| 20 - 100 | 0.0017 - 0.01 | 6 | 6 |

The η* binning is non-uniform: narrower bins where statistics are large
(central η*), wider 0.7-1.3 unit bins near the kinematic edges
(η* ≈ 0, η* ≈ 5). pT* binning is read from Tables 5, 8, 9 (not fully
extracted here but visible in ar5iv).

**Implication for our analysis.** Our low-Q² SIDIS measurement covers
5 < Q² < 100 GeV² with finer Q² bins (typically 4-5 Q² edges). At the
(x, Q²) integrated level our bins should be checked against the 8 here so
that **at least one of our combined bins matches one of theirs** to enable
a charge-summed M^{h} cross-check. The natural overlap is Q² ∈ [20, 100]
GeV², x ∈ [0.0017, 0.01], which is also where our highest-statistics
M^{h} bins live.

---

## 6. Correction Methodology

### 6.1 Bin-by-bin correction

Section 4. **No matrix unfolding.** The data are corrected with a multiplicative
correction factor per (η*, pT*) bin:

```
C_bin = N_charged_particles_generated_in_bin / N_tracks_reconstructed_in_bin
```

computed from Djangoh + Heracles + Geant simulation, where the generator-level
"generated" particles are required to be stable (cτ > 10 mm) and inside the
phase-space definition of Table 1.

### 6.2 Purity requirement

Bin widths are chosen such that **purity > 75 %** in every bin, where
purity = (particles generated AND reconstructed in the bin) / (all
particles reconstructed in the bin). This is the implicit constraint that
keeps bin-by-bin correction valid; if migrations dominated, purity would
drop and you would need true unfolding.

### 6.3 Outside-phase-space and γp subtraction

Both done **before** applying the multiplicative C_bin factor, following the
prescription of ref [15] (an earlier H1 forward-jet paper). This is the
order:

1. Subtract background-event contribution (γp from Phojet, weighted to data
   normalisation).
2. Subtract migrations from outside the (x, Q², η*, pT*) phase space.
3. Multiply by C_bin (Djangoh).

### 6.4 Magnitude of correction factors

- In the **current region** (1.5 < η* < 5): C_bin varies between 1.0 and
  1.8. Largest at high pT* and large η*.
- In the **central region** (0 < η* < 1.5): C_bin rises up to 2.6 at high
  pT* — they explicitly attribute this to "limited detector acceptance in
  this region", i.e. tracks at low η* / high pT* fall outside the CTD-only
  acceptance and the FTD does not help.

The Djangoh-vs-Rapgap C_bin difference is **at most 5.5 %** in marginal
bins, typically much smaller.

### 6.5 MC generators used for correction

| MC | Role | Notes |
|---|---|---|
| **Djangoh** | Nominal correction MC | CDM via Ariadne, CTEQ6L(LO) PDF, JETSET-Lund fragmentation, Heracles for QED radiative effects |
| **Rapgap** | Systematic alternative | DGLAP+PS with O(αs) matrix element, CTEQ6L(LO) PDF, Pythia-Lund fragmentation, Heracles |
| Cascade | Comparison only | CCFM, A0 unintegrated gluon, Pythia fragmentation |
| Herwig++ | Comparison only | POWHEG+DGLAP, MRST 02 NLO, cluster fragmentation |
| **Phojet** | γp background subtraction | Two-component dual parton model |

Both Djangoh and Rapgap distributions are **reweighted to data** in (y, vtxZ,
charged-particle pT*, charged-particle η*) before the bin-by-bin factors are
computed. The reweighting is applied to the **generator quantities** (so
generator and reco are reweighted by the same factor on a per-event basis).
This is the same MCweight scheme used in DESY 20-176 (see existing
`research-h1-charged-particle-analysis.md`).

---

## 7. Systematic Uncertainty Budget

Section 5. Note that this budget is **for charge-summed densities** and
covers only the (η*, pT*) observables. Our charge-asymmetry and azimuthal-
modulation observables have additional terms not included here.

| Source | Variation applied | Impact on densities | Treatment |
|---|---|---|---|
| SpaCal EM energy scale | ± 1 % | typically 0.4 % | Correlated across bins |
| SpaCal angular resolution | ± 1 mrad | ≈ 0.1 % | Correlated |
| Hadronic energy scale (HFS) | ± 2 % | ≈ 0.3 % | Affects (x, Q²) calculation via Σ and the HCM boost |
| MC model dependence | Djangoh vs Rapgap C_bin | 0.2 % to 5.5 % | Treated as a 1-sided systematic taking the difference |
| Central-track reconstruction | (see below) | 1 % per track | **Uncorrelated** point-to-point |
| Combined-track reconstruction | (see below) | 10 % per track | **Uncorrelated** point-to-point |
| K0s contamination diff data vs MC | n/a | 0.2 % | Flat |
| Λ contamination | n/a | negligible | Not assigned |
| Residual γp background | ± 30 % | up to 0.9 % at lowest (x, Q²) | Correlated by (x, Q²) bin |
| **Total syst** | | **< 2.5 % typical, up to ≈ 8 %** | quadrature |

**Tracking syst details.**

- **Central tracks: 1 %.** Source: in-situ studies of (a) curling tracks
  and (b) secondary vertices in the inter-CJC material from
  hadronic interactions. The MC underestimates the pion/kaon nuclear
  interaction cross-section; after correcting for this, agreement is < 1 %.
- **Combined tracks: 10 %.** Determined by comparing combined-track
  reconstruction efficiency in data and MC using (a) selected central
  tracks as a reference and (b) pions from K0s decays, both as a function
  of pT and η. Agreement within 10 %. This is an order of magnitude
  worse than the central-track value and dominates the systematic in the
  forward (η* > 3) bins.

The total of tracking-related contributions to the densities is 1.0-5.4 %
per bin, 1.6 % on average.

### 7.1 How to apply these to OUR analysis

| Item | Adopt as-is? | Action |
|---|---|---|
| Central-track efficiency 1 % | Yes, with caveats | Apply as bin-by-bin uncorrelated. For charge asymmetry A_h, the 1 % flat efficiency cancels; only the charge-dependent residual matters. |
| Combined-track efficiency 10 % | Yes for forward tracks | If we use FTD-combined tracks (we currently restrict to η_lab < 1.5 in some builders), this scale matters. Verify whether we use combined tracks at all; if not, this term drops. See `proton/context-handoff.md` for the current cut. |
| SpaCal energy 1 % → 0.4 % effect | Re-derive | Our observables include cos(phi_h) and pT_h-binned data; the propagated effect could be larger because phi_h depends on the boost. Re-run with H1OO ±1 % SpaCal scale shift. |
| Hadronic scale 2 % → 0.3 % | Re-derive | Same logic; affects Empz, eΣ x and Q², and HCM boost. |
| MC model (Djangoh vs Rapgap) | Yes, exactly | This is the single largest systematic in their analysis and almost certainly will be in ours. Plan for both MCs to be available end-to-end. |
| γp 30 % normalisation | Adopt | 30 % syst on Phojet normalisation is standard at H1 low-Q². Use the same value. |
| K0s contamination 0.2 % | Adopt as starting point | We should re-derive from our V0 measurements (see `lambda/` analysis); could be larger in finer (z, pT_h) bins. |

---

## 8. Radiative Corrections

The paper does **NOT publish explicit C_rad values per bin.** Instead:

- Heracles is interfaced to both Djangoh and Rapgap during MC generation so
  that the simulated events include single-photon ISR/FSR.
- The eΣ kinematic method is explicitly chosen because of its "little
  sensitivity to QED radiative effects".
- The z-vertex and Empz cuts further reduce the ISR tail.
- The **outside-phase-space subtraction** absorbs residual radiative
  migrations: events whose generator-level (true) (Q², x) is outside the
  cut window but which migrate in because of ISR are subtracted bin-by-bin.

The implicit treatment is that **radiative corrections are folded into the
detector-level Djangoh+Heracles bin-by-bin correction factor C_bin and not
factored out separately.** This is the same approach as DESY 20-176 and is
adequate when the radiative tail is small (here ≲ 1 %).

**Comparison to our planned procedure.** We are computing explicit C_rad
per (Q², x, z, pT_h) bin from DJANGOH14 radiative vs non-radiative
generator trees (see `research-crad-for-charged-sidis.md`). This is more
explicit than 1302.1321 and lets us quote C_rad in the systematic table.
The 1302.1321 approach is a fallback if our C_rad determination becomes
unstable in low-statistics bins.

---

## 9. Normalisation

Section 3.4. **Luminosity does not appear in the published observables.**

- Numerator: track count summed over the data sample within a (x, Q², η*,
  pT*) bin, after corrections.
- Denominator: number of DIS events in the same (x, Q²) cell.

Because both numerator and denominator come from the same luminosity, the
overall HERA-II 2006 lumi normalisation (and its associated ≈ 3 %
uncertainty) **cancels in the ratio** and is not included in the syst
budget.

**This is the right normalisation for our M^{h±} measurement too** and
matches our existing convention. The lumi uncertainty does not enter
M^{h±} or A_h, only an explicit cross-section dσ/dx dQ² dz dpT_h.

For DIS-event counting they use the selected sample directly; no separate
unfolding of N_DIS itself is performed. (Their bin-by-bin C_bin folds in
the DIS-event acceptance implicitly because it is defined as ratio of
particles per generated DIS event to tracks per reconstructed DIS event.)

---

## 10. Key Physics Results Relevant to Our Analysis

### 10.1 Charge-summed density values

The published per-event density (1/N) dn/dη* peaks at ≈ 1.95 charged
particles per unit η* near η* ≈ 2 at Q² ∈ [20, 100] GeV², x ∈ [0.0004,
0.0017], for pT* < 1 GeV (Table 4 transcribed values around line 880-910
of the ar5iv source).

For pT* > 1 GeV (Table 5), the density peaks at ≈ 0.23 charged particles
per η* near η* ≈ 2.5 (line 470-510).

**These numbers are the direct cross-check our charge-summed M^{h+} +
M^{h-} should reproduce to within their ≈ 2 % systematic** in the
overlapping kinematic regions, integrated over our z range with the
matching pT_h cut.

### 10.2 Generator scorecard for low-Q² DIS

| Generator | pT* < 1 GeV | pT* > 1 GeV | Notes |
|---|---|---|---|
| Djangoh (CDM) | ✓ within PDF | ✓ best | Mild current-region deviation at low (x, Q²) high pT* |
| Rapgap (DGLAP) | ✓ within PDF | ✗ undershoots strongly at small η*, low (x, Q²) | "Sharp drop in ratio at pT* ≈ 1 GeV" |
| Herwig++ (DGLAP+cluster) | small η* OK | ✗ even softer than Rapgap | Cluster hadronisation likely the issue |
| Cascade (CCFM) | ✗ too high for η* < 3 | ✗ overshoots everywhere except lowest (x, Q²) | Default A0 ugd |

**Direct implication for our M^{h} analysis:**

1. **Use Djangoh as the nominal MC** for detector correction, just as
   they do (and as DESY 20-176 does).
2. **Use Rapgap as the systematic alternative** for the MC-model
   syst — exactly the prescription here.
3. **Cascade is not a good alternative MC** for our purposes. We should
   not use it for an additional uncertainty band.
4. **Where Rapgap undershoots Djangoh by ≳ 10 % (high pT*, low η*, low
   x)**, the MC syst on M^{h} will be at the level of several percent,
   matching the 5.5 % they quote for the worst bins.

### 10.3 Hadronisation tune sensitivity

Figure 4 of 1302.1321 compares Rapgap with three Lund fragmentation
tunes: ALEPH, Professor (LEP-tuned), and Pythia 6.424 defaults. ALEPH wins
in the soft region; in the hard region (pT* > 1 GeV) all three fail
similarly. **Implication:** for M^{h±}-integrated-in-pT_h observables, the
fragmentation tune matters at the few-percent level; for high-pT_h bins
the tune choice is washed out by parton-shower differences.

---

## 11. Phase-Space Overlap with Our Measurement

### 11.1 Low-Q² SIDIS

| Cut | 1302.1321 | Our low-Q² SIDIS | Overlap? |
|---|---|---|---|
| Q² | 5-100 GeV² | 5-100 GeV² | Exact |
| x | 1e-4 - 1e-2 | depends on bin; usually 1e-4 - 0.05 | Theirs ⊂ ours (at low x) |
| y | 0.05 - 0.6 | 0.05 - 0.6 (standard) | Exact |
| Lumi | 88.6 pb^-1 (2006) | 297.7 pb^-1 (2005-7) | Ours is 3.4× larger |
| Frame | HCM | HCM (also Breit for some TMDs) | Direct |
| Tracks η_lab | -2 to +2.5 | TBD per analysis | Adopt theirs as default |
| Track pT_lab | > 150 MeV central | > 150 MeV (standard) | Same |
| Observable | (1/N) dn/dη*, dn/dpT* | M^{h±}(x,Q²,z,pT_h), <cos φ_h>, A_h | Theirs is the marginal (charge-summed, no z, no φ_h) of ours |

**The 1302.1321 measurement is a marginal of our M^{h+} + M^{h-} integrated
over our z range with our pT_h cut and our η* selection.** This is the
single most important cross-check we have for the low-Q² builder.

### 11.2 High-Q² SIDIS

| Cut | 1302.1321 | Our high-Q² SIDIS | Overlap |
|---|---|---|---|
| Q² | 5-100 GeV² | > 150 GeV² | **None** |
| x | 1e-4 - 1e-2 | up to ~0.5 | None |

The high-Q² extension is **not constrained** by 1302.1321 directly.
However the systematic methodology (track-efficiency assignments, MC model
choice, bin-by-bin correction philosophy) transfers, because the H1
hardware is the same and the tracking systematics do not depend strongly
on Q² (they depend on η, pT, and detector occupancy). For a Q² > 150
GeV² absolute-density cross-check we should fall back to other H1 charged-
particle measurements (e.g. high-Q² jet papers or DESY 20-176 high-Q²
extension if available).

---

## 12. Pitfalls and Lessons Learned

1. **Forward-tracking efficiency systematic is 10×**, not the same as
   central. Any analysis that uses combined (CTD+FTD) tracks must carry a
   10 % per-track uncertainty in the forward region. If we restrict to
   central CTD tracks only, we save a factor of 10 in this systematic at
   the cost of losing tracks at η_lab > 1.74. The 1302.1321 paper deems
   the FTD acceptance worth the 10 % syst because the high-η* region is
   physics-essential at low x.

2. **Bin-by-bin correction is acceptable only when purity > 75 % per bin.**
   Below this, migrations become large and you must move to matrix
   unfolding. Our M^{h±}(Q², x, z, pT_h) measurement has finer differential
   bins than 1302.1321 (we add z and pT_h dimensions and split by charge);
   purity per bin will be lower. **Plan to verify purity per bin from
   Djangoh truth-matched output; if any bin drops below 75 %, escalate
   to unfolding** (see existing `research-4d-unfolding.md`).

3. **HCM frame "central" vs "current" labels are inverted from Breit-frame
   convention.** Do not let plotting macros silently assume the same.

4. **The MC reweighting in (y, vtxZ, charged-particle pT*, η*) is applied
   to generator-level quantities.** This means truth-matched MC studies of
   detector effects use the *reweighted* truth, not the raw truth. Replicate
   exactly in our framework for like-for-like syst comparison.

5. **Track candidates must be vertex-fitted.** Not vertex-fitting suppresses
   the K0s/Λ daughter and γ-conversion subtraction step but inflates the
   secondary background. 1302.1321 takes the vertex-fitted route, gets a
   0.2 % residual K0s syst, and treats Λ as negligible. Our analysis
   should match this convention.

6. **The "additional quality cuts" cited as ref [41] are critical but not
   published in this paper.** Get them from DESY 20-176 (`research-h1-
   charged-particle-analysis.md` Sec. 4) or from the underlying H1OO track
   selectors. Do not invent new cuts.

7. **The eΣ kinematic method is the right default for low-Q²** (best
   resolution, lowest QED-radiation sensitivity). Our `H1DISKinematics`
   wrapper should default to eΣ in the low-Q² analyzer. See
   `research-dis-kinematic-methods.md` for the full method comparison.

8. **Empz cut applies to total sum including the electron** (35-75 GeV
   window). Easy to get wrong if you reuse code that defined Empz as
   HFS-only.

9. **Photoproduction systematic is 30 %** on the Phojet template, leading
   to up to 0.9 % on densities — small enough to ignore in the central
   (x, Q²) bins, important enough to count in the lowest (x, Q²) bin.

10. **They quote no luminosity systematic** because lumi cancels in the
    per-event density. Apply the same logic to M^{h±} and A_h. Only quote
    lumi syst when reporting differential cross-sections.

---

## 13. Concrete Recommendations for Our SIDIS Analysis

| # | Item | Adopt? | Comment |
|---|---|---|---|
| 1 | DIS selection: Q² (5-100), y (0.05-0.6), x (1e-4 - 1e-2), E'_e > 12 GeV, |z_vtx|<35 cm, 35<Empz<75 GeV | YES | Direct copy for low-Q² analysis |
| 2 | eΣ kinematic method | YES | Best resolution and radiation insensitivity in low-Q² |
| 3 | Track selection: central pT > 150 MeV, combined p > 500 MeV, both vertex-fitted, η_lab ∈ [-2, 2.5] | YES | Standard |
| 4 | HCM frame for observables | YES | Same boost as 1302.1321 |
| 5 | Djangoh nominal + Rapgap alternative for MC syst | YES | Largest single systematic; expected |
| 6 | Bin-by-bin correction with purity > 75 % per bin | CONDITIONAL | Verify purity in our finer (z, pT_h) bins; escalate to unfolding if any bin < 75 % |
| 7 | 1 % central track + 10 % combined track tracking efficiency | YES | Bin-by-bin uncorrelated; for A_h, only charge-dependent residual matters |
| 8 | SpaCal energy ± 1 %, SpaCal angle ± 1 mrad, hadronic ± 2 %, γp ± 30 % | YES | Re-derive impact on M^{h±}, <cos φ_h>, A_h via H1OO replay |
| 9 | Charge-summed density cross-check vs Tables 2-9 | YES | Strong validation of low-Q² SIDIS builder |
| 10 | Charge-separated extensions | OURS | 1302.1321 does not measure these; new contribution from our work |
| 11 | z and pT_h dimensions | OURS | 1302.1321 does not measure these; new contribution |
| 12 | Azimuthal modulations | OURS | 1302.1321 does not measure these; new contribution |
| 13 | Explicit per-bin C_rad from DJANGOH14 radiative vs non-radiative | OURS | More explicit than 1302.1321's implicit Heracles folding; cleaner systematic |
| 14 | Avoid Cascade as alternative MC | YES (avoid) | Cascade overshoots data over most of phase space; not a fair alternative |
| 15 | Lumi syst not included in M^{h±} or A_h | YES | Cancels in per-event normalisation |

---

## 14. Source Limitations and Follow-ups

- The paper references [41] (Bylinkin PhD thesis) for the precise track
  quality cuts and the η*/pT* reweighting tables. Not retrieved here.
  Recommend pulling from DESY thesis archive or cross-referencing against
  DESY 20-176's published version of the same cuts.

- pT* binning tables (5, 8, 9) and per-bin charge-particle densities were
  read partially from ar5iv (lines 460-1199 of the source dump). Full
  tables can be re-extracted by re-fetching ar5iv and pulling specific
  line ranges; they were not transcribed exhaustively here because the
  text content is what matters for our analysis methodology.

- Figures 3-10 were not visually inspected; numerical content was read
  from Tables 2-9.

- I did not verify the published version (EPJC 73 (2013) 2406) page
  references against the arXiv submission. The arXiv v1 metadata, DESY-13-012
  report number, and submission to EPJC are confirmed from the abstract
  page. Numerical values cited in Section 10.1 are from the ar5iv-rendered
  arXiv source, which is the v1 submission.

- For finer cross-checks of pT* > 1 GeV high-statistics bins, consult
  HEPData entry for the publication (recommend) — that gives the full
  numerical tables in a machine-readable format.

---

## 15. References (anchor for traceability)

- H1 Collaboration, "Measurement of Charged Particle Spectra in
  Deep-Inelastic ep Scattering at HERA," DESY 13-012, arXiv:1302.1321
  [hep-ex], Eur. Phys. J. C 73 (2013) 2406.
  - Submission: arXiv v1, 6 Feb 2013
  - Abstract page: https://arxiv.org/abs/1302.1321
  - ar5iv HTML: https://ar5iv.labs.arxiv.org/html/1302.1321
  - InspireHEP: https://inspirehep.net/arxiv/1302.1321

- Related earlier and later H1 publications in the same series, cited
  here as referenced by 1302.1321:
  - C. Adloff et al. (H1) "Measurement of charged particle transverse
    momentum spectra in deep inelastic scattering," Nucl. Phys. B485 (1997)
    3 — the predecessor analysis (ref [18] in 1302.1321), ≈ 70× lower
    statistics.
  - DESY 20-176, arXiv:2011.01812 — multiplicity-distribution and
    entanglement-entropy successor analysis; see `research-h1-charged-
    particle-analysis.md` in this directory.

- Adjacent research notes in this directory (read together with this one):
  - `research-h1-charged-particle-analysis.md` (DESY 20-176 codebase map
    and tracking cut details that 1302.1321 inherits)
  - `research-sidis-systematics-literature.md` (cross-paper syst comparison)
  - `research-tracking-systematics-deep.md` (1 % central / 10 % combined
    tracking syst origin)
  - `research-dis-kinematic-methods.md` (eΣ vs alternative methods)
  - `research-crad-for-charged-sidis.md` (our explicit C_rad treatment)
  - `research-fast-systematics-methods.md` (analytic propagation pattern
    used in 1302.1321)

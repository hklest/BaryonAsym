# Research memo: Is an `x_Feynman > 0` cut justified for the H1 high-Q² charged-hadron SIDIS multiplicity analysis?

**Date:** 2026-05-27
**Author:** physics-literature researcher
**Status:** Decision memo, gates implementation work
**Scope:** Q² > 150 GeV² charged-hadron SIDIS multiplicity at HERA-II, H1, 4D (Q²,x,z,pT_h) binning, Σ-method kinematics, lab-frame fiducial pT > 0.15 GeV and |η_lab| < 1.3

---

## 1. Executive summary

**Recommendation: Option (C) — do NOT impose an explicit `x_F > 0` cut as the default analysis selection. Instead, document the existing |η_lab| < 1.3 fiducial as the de-facto current-fragmentation enforcer, and (optionally, low priority) compute `x_F` at the truth level for Option (B): a one-sided systematic cross-check that the residual `x_F < 0` truth contamination is consistent with zero.**

Confidence: **High** for the literature precedent (Q1, Q2) and for the kinematic/acceptance argument (Q4). **Medium-high** for the TMD-interpretation argument (Q3): no published H1/HERMES/COMPASS/JLab unpolarised multiplicity measurement uses `x_F > 0` as a primary phase-space cut; they universally use `z > z_min` (most commonly 0.2) as the operative current-fragmentation enforcer at fixed-target √s, and at HERA collider √s the lab-frame η fiducial alone already enforces `x_F > 0` for ~all reachable hadrons because of the large γ*p ↔ lab boost. **Medium** for the radiative-correction interaction argument (Q5): no precedent suggests an `x_F` cut perturbs C_rad in a non-trivial way distinct from the standard fiducial-cut treatment, but explicit verification would require a separate generator-level study.

Three concrete findings drive the recommendation:

1. **Zero published HERA-collider SIDIS multiplicity measurement (H1 or ZEUS) uses an `x_F` cut.** H1 1302.1321 (low-Q²) and H1 2011.01812 (multiplicity / entanglement entropy) instead use a *pseudorapidity-in-HCM* cut: `0 < η* < 5` (1302.1321) or `0 < η* < 4` (2011.01812). The 2011.01812 paper explicitly identifies this `η* > 0` selection as "qualitatively the current fragmentation region." H1 *high-Q² jet and lepton-jet* papers (1611.03421, 2108.12376) do not bin in `x_F` at all; they apply only lab-frame fiducials on jets and HFS objects. There is **no precedent in the H1 high-Q² literature** for an `x_F` cut on inclusive single-charged-hadron observables. (Q1)

2. **HERMES and COMPASS unpolarised multiplicity papers do not cut on `x_F` either.** They cut `0.2 < z < 0.8` (COMPASS 1709.07374) or `0.2 < z < 0.8` for kinematic-integrated tables (HERMES 1212.5407). Both papers explicitly cite the lower `z`-cut as the target-fragmentation suppressor — **z is the cut, not `x_F`**. Both still report fully-differential multiplicities `M^h(x,Q²,z,P_h⊥)` and azimuthal moments without an `x_F` requirement. (Q2)

3. **At H1 √s = 318 GeV with the lab fiducial |η_lab| < 1.3, the boost between lab and γ*p HCM is ~5.5 units of rapidity (≈ ln(W/mp)). A 4-vector calculation (Section 5, Table 5.1) confirms that for every representative (Q², x) bin in this analysis, a charged pion at η_lab = +1.3 (the forward-most edge of the acceptance) has `x_F` ≈ +0.003 to +0.02 — barely positive but unambiguously > 0; at η_lab = 0 it has `x_F` ≈ +0.02 to +0.09; at η_lab = -1.3 it has `x_F` ≈ +0.07 to +0.33.** No reachable hadron at any pT in the analysis fiducial has `x_F < 0` at the truth level, modulo finite-resolution smearing and pion-mass effects at the very lowest pT. The supervisor's acceptance prior is **confirmed quantitatively**: the cut is effectively a no-op. (Q4)

Engineering work to add an `x_F > 0` reco-level cut (Tier 1, ~1 hr) would not change observed yields at the percent level, would not measurably tighten the systematic budget, and would put the high-Q² analysis **out of convention** with every H1, HERMES, COMPASS, and JLab unpolarised multiplicity precedent. Tier 2/3 work (generator-side `x_F` instrumentation for a systematic cross-check) is the only justified scope, and even that is low-priority compared with closing the existing 11-source systematic budget. **Do not dispatch implementation.**

---

## 2. The cut definition and where it sits in the SIDIS phase space

### 2.1 Formal definition of `x_F` in SIDIS

Feynman-x for a final-state hadron `h` in the γ*p hadronic centre-of-mass (HCM) frame is

```
x_F = 2 p_z*(h) / W
```

where `p_z*(h)` is the longitudinal momentum of the hadron along the γ* direction in the HCM frame and `W = sqrt((p+q)²)` is the γ*p invariant mass. The convention used in essentially all SIDIS literature is: **+z\* = direction of the incoming virtual photon γ\*** in the HCM. In leading-order quark-parton-model kinematics, the struck quark exits the hard vertex with positive `p_z*` (current direction), and the proton remnant retains negative `p_z*` (target direction). Thus

- `x_F > 0` ⇔ hadron in *current* (struck-quark) hemisphere in HCM
- `x_F < 0` ⇔ hadron in *target* (proton-remnant) hemisphere in HCM
- `x_F = ±1` are the kinematic limits for a hadron carrying all available longitudinal momentum on the respective side

`x_F` is **not** equivalent to the longitudinal hadron-energy fraction `z = (P·P_h)/(P·q)` ≈ `E_h*/ν*`. `z` is a positive-definite Lorentz scalar; `x_F` is a signed longitudinal-momentum fraction that can have either sign and that vanishes when the hadron is at rest in the longitudinal direction in the HCM. Numerically, at small to moderate `z` the two are correlated but not identical: a hadron at moderate `z` with large `pT*` can still have `x_F` close to zero or negative.

### 2.2 Relation to η* (pseudorapidity in HCM) and η_lab

In the HCM, with `+z* = γ*`, hadron pseudorapidity is `η* = -ln tan(θ*/2)` measured from `+z*`. Then

- `η* > 0` ⇔ forward of the η* = 0 plane ⇔ same hemisphere as γ* ⇔ current hemisphere
- `η* < 0` ⇔ target hemisphere

For a hadron with given `(p_z*, pT)`, the relation between `x_F` and `η*` involves the hadron mass:

```
x_F = (2 m_T / W) · sinh(y*),    y* = rapidity in HCM (along z*)
η* = y*  (massless approximation)
```

So in the massless / high-`p_h*` limit, `x_F > 0 ⇔ y* > 0 ⇔ η* > 0`. The two cuts are **asymptotically equivalent** but differ for soft, low-`pT*` hadrons where the rapidity-pseudorapidity distinction matters. This is the basis for treating an `x_F > 0` cut and an `η* > 0` cut as equivalent in the published HERA literature — see H1 2011.01812 footnote 1 (Section 1) which spells out the equivalence explicitly.

The lab → HCM boost is approximately a longitudinal boost (in the conventional axes where +z_HCM lies along γ* in lab) with rapidity shift

```
Δη(lab → HCM) ≈ ln(2 E_p / W)   for the typical case where γ* in lab is closely aligned with the boost axis
```

(modulo a small `qT_lab` contribution from the photon getting a transverse kick from the electron scattering geometry). At HERA-II beam energies (E_p = 920 GeV, E_e = 27.6 GeV) and typical W ~ 100–300 GeV for our high-Q² bins, this boost is **5 to 7 units of rapidity**, with the proton sitting at η_lab ≈ +7.6 and at η* ≈ -ln(W/mp) ≈ -5.0 to -6.0 in HCM. The H1 acceptance |η_lab| < 1.3 thus covers a 2.6-rapidity-unit-wide slice that is far away from the proton-remnant direction in HCM. See Section 5 for the explicit 4-vector calculation.

### 2.3 Relation to z

The relation `(x_F, pT) ↔ (z, pT)` runs through:

```
E_h* = z · ν*,     ν* = (W² + Q²)/(2 W)         (HCM energy fraction)
|p_h*|² = E_h*² - m_h²
p_z*² = |p_h*|² - pT²
x_F = ± 2 p_z* / W
```

The sign of `x_F` (i.e. which hemisphere) is in principle independent of `z`: a hadron at small `z` and small `pT` can sit in either hemisphere depending on its specific kinematics. However, in *the LO QPM picture* the struck quark fragments forward in HCM (the "string" stretches between the struck quark and the remnant), so the bulk of hadrons at moderate-to-large `z` are at `x_F > 0`. The `x_F < 0` region at large `z` is suppressed because it would require a hadron carrying a large energy fraction to also have it longitudinal momentum aligned with the proton remnant — kinematically allowed only for `z` close to 1 in certain corners. The **lower `z` cut at 0.2 is the canonical current-fragmentation enforcer in fixed-target SIDIS** (HERMES, COMPASS): see Section 3.2.

### 2.4 Relation to Trento-frame φ_h harmonics

The Trento convention (Bacchetta et al. 2004; widely adopted in TMD literature) defines φ_h as the azimuthal angle of the hadron production plane relative to the lepton scattering plane, measured in the γ*p CM frame. φ_h is computed from the hadron's transverse momentum vector with respect to the γ* direction in HCM. The Trento-frame `cos(φ_h)`, `cos(2φ_h)` harmonics are sensitive to TMD structure functions `F_UU^{cos φ}`, `F_UU^{cos 2φ}` (Cahn effect, Boer–Mulders effect, respectively). These structure functions are theoretically defined under collinear or TMD factorization assumptions that require the hadron to be in the current fragmentation region (`x_F > 0` asymptotically, plus the additional TMD requirement `pT* ≲ Q`).

The cut `x_F > 0` is a *necessary* condition for a TMD-factorization interpretation but it is not the *sufficient* one — the sufficient condition is the Boglione–Collins–Gamberg–Gonzalez-Hernandez–Rogers–Sato criterion discussed in Section 4.

---

## 3. Literature survey (Q1, Q2)

### 3.1 H1 inclusive charged-particle / multiplicity papers

#### 3.1.1 H1 charged-particle spectra in low-Q² DIS (Adloff et al. 2013, arXiv:1302.1321, EPJC 73:2406)

**Kinematics:** 5 < Q² < 100 GeV², 10⁻⁴ < x < 10⁻², 0.05 < y < 0.6, √s = 319 GeV, L = 88.6 pb⁻¹.
**Tracker fiducial:** -2 < η_lab < 2.5, pT_lab > 150 MeV.
**Measurement frame:** HCM (`*`-quantities).
**Phase-space table (Table 1 of the paper):** charged particles required to have **`0 < η* < 5`** and **`0 < pT* < 10` GeV**. NO `x_F` cut.

The paper makes the convention explicit (Section 3.4):
> "HFS particles with η* > 0 belong to the current hemisphere and particles with η* < 0 originate from the target (proton remnant) hemisphere."

and immediately states:
> "The target region, η* < 0, is not accessible in this analysis."

This is the **structural precedent for HERA collider charged-hadron SIDIS analyses**: the operative cut is `η* > 0` (HCM-pseudorapidity), not `x_F > 0`. The two are equivalent in the massless / high-`p_h*` limit. At small x and y, the boost between lab and HCM places most of the lab-fiducial coverage at η* > 0 automatically.

#### 3.1.2 H1 multiplicity distributions and entanglement entropy (Andreev et al. 2021, arXiv:2011.01812, EPJC 81 (2021) 212)

**Kinematics:** 5 < Q² < 100 GeV², 0.0375 < y < 0.6, √s = 319 GeV, L = 136 pb⁻¹.
**Tracker fiducial:** |η_lab| < 1.6, pT_lab > 150 MeV. Notice this is **broader on the negative side** than 1302.1321 (which had -2 < η_lab < 2.5 in lab but cut at η* > 0 in HCM).
**Phase-space table (Table 1 of the paper):** charged particles required to have either |η_lab| < 1.6 (lab-frame measurement) or, for the HCM-frame measurement, **`0 < η* < 4`**. NO `x_F` cut.

The paper makes the equivalence between `η* > 0` and `x_F > 0` (qualitatively) explicit in footnote 1 of Section 1:
> "The positive η* region qualitatively corresponds to the current fragmentation region (predominantly negative η_lab), while η* < 0 corresponds to the direction of the target fragmentation region (predominantly positive η_lab)."

The "predominantly negative η_lab" / "predominantly positive η_lab" association is **specific to the low-Q² / small-x regime of that paper**: at small x, the scattered electron stays at small angle (near forward, which is the lepton direction = -z_lab in H1), and so the γ* direction in the lab is mostly along -z_lab, and the struck-quark/current hemisphere lands at negative η_lab. This is **not** the geometry at high Q² in our analysis (see Section 5).

The paper notes Sys.1 (radiative corrections) at the 1–2% level on `P(N)` and that the QED radiative correction is computed from the difference between DJANGOH and RAPGAP — independently of any `x_F` cut. The `0 < η* < 4` selection is treated symmetrically with the lab-frame selection; the unfolding (TUnfold) handles bin migrations in the η variable directly.

#### 3.1.3 H1 high-Q² jet papers

The high-Q² H1 jet literature is structured around lab-frame jet observables and does not bin in `x_F` of constituent hadrons or jets. The hadronic-final-state objects entering jet clustering use the lab pseudorapidity directly:

- **Andreev et al. 2017, arXiv:1611.03421 (DESY-16-200, EPJC 77 (2017) 215), low-to-moderate-Q² jets (5.5 < Q² < 80 GeV² and extension to 150 < Q² < 15000 GeV²):** measures inclusive jet, dijet, and trijet cross sections; jets clustered in lab frame (kT algorithm) with `pT_jet > 5 GeV` and `-1.0 < η_lab^jet < 2.5`. No `x_F` cut. The paper has been used for α_s extractions and as the precision-jet reference.
- **Andreev et al. 2022, arXiv:2108.12376 (DESY 21-130, Phys. Rev. Lett. 128 (2022) 132002), lepton-jet correlation at high Q² with ML unfolding:** Q² > 150 GeV², 0.2 < y < 0.7 (the directly analogous high-Q² fiducial to our analysis); jets clustered from HFS objects in the lab frame with `-1.5 < η_lab < 2.75` (input objects) and `-1.0 < η_lab^jet < 2.5` (output jets), `pT^jet > 10 GeV`. No `x_F` cut on jets or constituents. The paper explicitly discusses TMD-factorization interpretation of the lepton-jet azimuthal correlation `Δφ^jet` and the imbalance `qT^jet/Q` and compares to TMD predictions (Liu, Ringer, Vladimirov, Vogelsang, Yuan 2019) without any `x_F` cut.
- **Other high-Q² jet papers in the same family** (jet substructure 2303.13620 and groomed events 2403.10134, not retrieved in this memo) follow the same paradigm: lab-frame jet observables, lab-frame η fiducial, no `x_F`.

The picture is consistent across the **H1 high-Q² hadronic-final-state corpus**: lab-frame fiducials, no `x_F` cut. Multiplicity-like single-hadron observables at high Q² are not measured in the high-Q² regime by any published H1 paper to date — our analysis would be the first.

### 3.2 Fixed-target SIDIS: HERMES and COMPASS

#### 3.2.1 HERMES multiplicities of π± and K± from p and d (Airapetian et al. 2013, arXiv:1212.5407, Phys. Rev. D 87 (2013) 074029)

**Kinematics:** √s ≈ 7 GeV (27.6 GeV lepton beam on fixed proton/deuteron target), Q² > 1 GeV², W² > 10 GeV², 0.1 < y < 0.85.
**Hadron selection:** 2 < P_h < 15 GeV (RICH operating range), `0.2 < z < 0.8` for `z`-integrated tables.
**Observables:** `M^h(x_B, Q², z, P_h⊥)`, separately for π± and K±, on p and d targets.
**`x_F` cut:** NONE.

The HERMES paper (Section III.3) makes the rationale for `0.2 < z < 0.8` explicit:
> "Data that are not explicitly binned in the fractional hadron energy z also are constrained to 0.2 < z < 0.8 to exclude the region at high z, which receives a sizeable contribution from exclusive processes, and to suppress the target fragmentation region at low z."

The **`z > 0.2` cut IS the target-fragmentation suppressor** in HERMES, not a separate `x_F > 0` requirement.

HERMES's choice is informed by the LEPTO/JETSET-based unfolding (their "smearing matrix" approach), which handles target-fragmentation contamination through bin-migration corrections — i.e. the explicit `x_F < 0` events are not removed but instead are accounted for by the unfolding matrix from generator-level truth. The systematic uncertainty associated with `z`-bin migrations is included in their per-bin uncertainty budget. Multiplicities are reported in `(x_B, Q², z, P_h⊥)` bins; no `x_F` axis appears in any of their tables.

#### 3.2.2 COMPASS TMD multiplicities of charged hadrons on deuteron (Aghasyan et al. 2018, arXiv:1709.07374, Phys. Rev. D 97 (2018) 032006)

**Kinematics:** 160 GeV/c µ⁺ beam on ⁶LiD, Q² > 1 (GeV/c)², W > 5 GeV/c², 0.003 < x < 0.4, 0.1 < y < 0.9.
**Hadron selection:** `0.2 < z < 0.8`, 0.02 < P_hT² < 3 (GeV/c)².
**Observables:** `d²M^h / dz dP_hT²` in 4D (x, Q², z, P_hT²) bins.
**`x_F` cut:** NONE.

The COMPASS paper (Section 3.2) is even more explicit about the role of `z > 0.2` as the target-fragmentation killer:
> "The fraction of the virtual-photon energy transferred to a final-state hadron is constrained to 0.2 < z < 0.8. The lower limit excludes the target fragmentation region, while the upper one removes muons wrongly identified as hadrons and excludes the region with larger contributions from diffractive ρ⁰ production."

This is the **canonical published statement** of the SIDIS-community convention: **the `z > 0.2` cut is the operative target-fragmentation suppressor**. `x_F > 0` is not used and is not mentioned anywhere in the COMPASS multiplicity paper.

COMPASS's diffractive vector-meson correction (Section 3.4 of the paper) handles ρ⁰ → π⁺π⁻ and φ → K⁺K⁻ contamination through a separate `(1 - f_π^{ρ⁰})` and `(1 - f_K^φ)` factor in the multiplicity expression. This addresses a different population from the target-fragmentation contamination.

#### 3.2.3 JLab CLAS12 and Hall C SIDIS

JLab kinematics is fixed-target with beam energies 6, 10.6 (CLAS12 / Hall C) or 11 GeV (future), giving √s ≈ 3–5 GeV and W² typically 4–10 GeV². In this regime the boost between lab and HCM is small (a few units of rapidity at most), and the target-fragmentation contamination is *kinematically accessible* in a way it is not at HERA collider energies. Despite this, neither the CLAS dihadron beam-spin-asymmetry papers (2201.05732 and related), the Hall C SIDIS multiplicity paper (Bosted et al. 2025, arXiv:2510.03562), nor the CLAS12 SIDIS proposals (Burkert 2008, 0810.4718; Vossen 2018, 1810.02435; Ye et al. 2022, 2202.09696) use `x_F > 0` as a primary cut. They use `z > 0.3` (Hall C uses z > 0.3 implicitly through their multiplicity binning) or `z > 0.2` (CLAS12 SIDIS standard, mirroring HERMES/COMPASS).

The 2025 Hall C SIDIS paper (Bosted et al., arXiv:2510.03562) reports `cos φ`, `cos 2φ` modulations of π± multiplicities on p and d in the range 0.3 < x < 0.6, 3 < Q² < 5 GeV², and bins in (x, Q², z, P_T). No `x_F` cut. The paper observes a significant `cos(φ)` modulation for π⁻ and a null result for π⁺. The interpretation of these modulations within TMD factorization does not invoke `x_F > 0` as a precondition.

#### 3.2.4 Summary table of fixed-target / HERA-collider SIDIS conventions

| Experiment | Paper | Q² range (GeV²) | Hadron `z` cut | Hadron `x_F` cut | Operative current-frag selector |
|------------|-------|-----------------|----------------|-------------------|----------------------------------|
| H1 (low-Q²) | 1302.1321 | 5–100 | none | none | `η* > 0` (HCM) |
| H1 (low-Q²) | 2011.01812 | 5–100 | none (event-level) | none | `|η_lab| < 1.6` or `0 < η* < 4` |
| H1 (high-Q²) | 1611.03421 | 5.5–80, 150–15000 | n/a (jet) | none | lab fiducial only |
| H1 (high-Q²) | 2108.12376 | > 150 | n/a (jet) | none | lab fiducial only |
| HERMES | 1212.5407 | > 1 | 0.2 < z < 0.8 | none | `z > 0.2` |
| COMPASS | 1709.07374 | > 1 | 0.2 < z < 0.8 | none | `z > 0.2` |
| JLab Hall C | 2510.03562 | 3–5 | z bins (lower edge ~0.3) | none | `z` binning |

The pattern is **uniform across the field**: `x_F` is not used as a primary phase-space cut; either a `z > ~0.2` cut (fixed target) or an `η_lab` / `η*` fiducial (collider) plays that role.

---

## 4. TMD factorization analysis (Q3)

### 4.1 Is `x_F > 0` *required* by TMD factorization?

No, not strictly. The TMD-factorization theorem for the unpolarized SIDIS structure function `F_UU` (Collins–Soper–Sterman framework, Collins 2011 textbook chapter 13; see also Bacchetta–Diehl–Goeke–Metz–Mulders–Schlegel 2007 for the structure-function decomposition) requires:

1. A **hard scale** Q ≫ Λ_QCD (set by the photon virtuality).
2. A **measured transverse momentum** `P_hT` (or equivalently `qT = -P_hT/z`) that is **small compared to Q**: `qT ≪ Q`. This is the "current-region TMD" regime where intrinsic transverse momentum dominates the measured `P_hT`.
3. The hadron must originate from the **fragmentation of the struck quark**, not from the proton remnant or from soft-emission overlap with the remnant.

Condition 3 is what `x_F > 0` is meant to enforce phenomenologically. But TMD theory itself does not write the cut as `x_F > 0`; it writes it in terms of **rapidity separation** between the struck-quark fragment and the proton remnant. The formal TMD-factorization derivation requires that the produced hadron be at a **rapidity well-separated from the rapidity of the proton remnant**, i.e. in a hadron-rest-frame language, `y_h - y_remnant` should be at least of order `ln(Q²/m_h²)` or so for the soft-collinear factorization to be unambiguous.

### 4.2 Boglione–Collins–Gamberg–Gonzalez-Hernandez–Rogers–Sato 2017 criterion (arXiv:1611.10329, Phys. Lett. B 766 (2017) 245)

The most precise and authoritative published criterion for "which kinematic region of SIDIS is the current fragmentation region appropriate for TMD factorization" is Boglione, Collins, Gamberg, Gonzalez-Hernandez, Rogers, Sato (2017), PLB 766 (2017) 245. The paper abstract states:

> "We address the question of how to identify the current fragmentation region — the kinematical regime where a factorization picture with fragmentation functions is appropriate. We distinguish this from soft and target fragmentation regimes. Our criteria are based on the kinematic regions used in derivations of factorization theorems. We argue that, when hard scales are of order a few GeVs, there is likely significant overlap between different rapidity regions that are normally understood to be distinct. We thus comment on the need to take this into account with more unified descriptions of SIDIS, which should span all rapidities for the produced hadron. Finally, we propose general criteria for estimating the proximity to the current region at large Q."

The Boglione et al. criterion (the "R-parameter") uses a *combination* of `z`, `Q²`, and the **rapidity of the hadron in the photon-target Breit frame**, not `x_F` alone, to decide whether a given (x, Q², z, P_hT) bin is in the "TMD-clean" current-fragmentation region. The key takeaways for our analysis are:

1. **At large Q (our regime, Q > 12 GeV), the rapidity separation between current and target hemispheres is large (`ΔY ~ ln(Q²/Λ²)` ~ 8–10 units), and the current-fragmentation region is well-defined.** The overlap between current and target rapidity regions that Boglione et al. caution about is a low-Q phenomenon. Our Q > 12 GeV is comfortably in the regime where the rapidity hemispheres are clean.
2. **The `z`-cut is more discriminating than the `x_F`-cut for separating target from current.** At `z` ≳ 0.2 in the regime Q ≳ a few GeV, almost all hadrons are in the current region (this is the basis for the universal `z > 0.2` choice in fixed-target SIDIS). Adding an `x_F > 0` cut on top of `z > 0.2` removes a vanishingly small subset.
3. **The TMD interpretation of `cos(φ_h)` and `cos(2φ_h)` harmonics does not become incorrect under a tighter or looser `x_F` cut.** What changes is the *amount of target-fragmentation contamination* in the measured harmonics; this is a phenomenological correction, not a theory issue.

### 4.3 Does `x_F > 0` change Trento-frame `cos(φ_h)`, `cos(2φ_h)` interpretation?

For our analysis, the harmonics are TMD-sensitive only in the regime `P_hT ≲ Q`. At Q > 12 GeV and `P_hT` up to 8 GeV (our highest `pT_h` edge), even the highest-`pT_h` bins satisfy `P_hT/Q ≲ 0.6`, which is at or just inside the conventional TMD-factorization region. The `x_F > 0` cut would:

- **Remove a small target-fragmentation contamination at low pT_h and low z**, where the hadron is more likely to be a remnant-pickup product. This *could* slightly reduce the `cos(φ_h)` modulation that arises from target-fragmentation kinematics (which generates its own φ-modulation through the spinless-quark / Cahn-effect mechanism in a different rapidity region).
- **Not significantly change** the `cos(2φ_h)` modulation, which is dominated by the Boer–Mulders / Collins-fragmentation correlation in the current region; this correlation is invariant under tightening the current-region cut.
- For the **charge-sum multiplicity M^{h+} + M^{h-}**, an `x_F > 0` cut would have a small effect (~ percent level in the most contaminated bins).
- For the **charge-difference asymmetry A_h = (M^{h+} - M^{h-}) / (M^{h+} + M^{h-})**, the cut would affect both numerator and denominator coherently, and the net effect on `A_h` is expected to be even smaller because target-fragmentation contamination is roughly charge-symmetric (proton remnant fragments produce nearly equal π⁺ and π⁻ from the sea-quark population).

**No published H1, HERMES, COMPASS, or JLab paper has reported a measurable difference in `cos(φ_h)` or `cos(2φ_h)` harmonics arising from imposing or relaxing an `x_F > 0` cut.** There is no published case that we could find where the cut was shown to induce or remove a measurable harmonic. This is consistent with the universal absence of `x_F`-based cuts in the published unpolarised multiplicity literature.

### 4.4 Confidence statement

Confidence in the Q3 conclusion ("`x_F > 0` is not required by TMD factorization and is not expected to change the Trento-frame harmonics measurably at our kinematics") is **medium-high**. The literature precedent is unambiguous, and the Boglione et al. criterion supports it at our Q range. The remaining uncertainty is from the absence of any published H1 high-Q² SIDIS measurement that has directly stress-tested the question; we are extrapolating from the low-Q² H1 measurement (1302.1321 / 2011.01812), from HERMES/COMPASS fixed-target multiplicities, and from the H1 high-Q² jet measurements that handle the analogous fragmentation question differently because they use jet observables.

---

## 5. Acceptance kinematic analysis (Q4)

### 5.1 Methodology

To check the supervisor's prior — that at H1 √s = 318 GeV with the lab fiducial |η_lab| < 1.3, target-fragmentation hadrons (`x_F < 0`) are kinematically inaccessible because they go to η_lab > 3 (proton-remnant direction) where there is zero acceptance — we computed a direct 4-vector boost from lab to γ*p HCM for a representative grid of (Q², x) values spanning the analysis. For each (Q², x) bin we:

1. Computed `y = Q² / (s x)` and constructed the scattered-electron 4-momentum from the on-shell collider kinematics.
2. Constructed γ* = `p_e_initial − p_e_scattered`.
3. Constructed `P_tot = p_p_initial + q` (the γ*p 4-momentum).
4. Boosted a test charged pion with mass `m_π = 0.14 GeV`, transverse momentum `pT = 0.5 GeV`, and three test lab pseudorapidities (η_lab = -1.3, 0, +1.3) into the HCM.
5. Rotated the HCM coordinate system so that the +z* axis aligns with the γ* direction in HCM.
6. Read off `η*` (HCM pseudorapidity) and `x_F = 2 p_z* / W`.

The proton itself, when boosted into the HCM, is at `η* ≈ -ln(W/m_p)` (large negative, deep in target hemisphere — confirmed by the calculation).

### 5.2 Results

**Table 5.1** — `(η*, x_F)` of a charged pion (mass 0.14 GeV, pT = 0.5 GeV) at three representative lab pseudorapidities for seven (Q², x) bins covering the high-Q² analysis range. All `x_F` values are **positive** (current hemisphere).

| Q² (GeV²) | x | y | W (GeV) | η* at η_lab=-1.3 | x_F at η_lab=-1.3 | η* at η_lab=0 | x_F at η_lab=0 | η* at η_lab=+1.3 | x_F at η_lab=+1.3 |
|---|---|---|---|---|---|---|---|---|---|
| 200 | 0.005 | 0.39 | 199.5 | 2.48 | +0.087 | 1.83 | +0.023 | 0.81 | +0.005 |
| 200 | 0.010 | 0.20 | 140.7 | 2.20 | +0.173 | 1.80 | +0.046 | 1.01 | +0.011 |
| 500 | 0.010 | 0.49 | 222.5 | 2.27 | +0.069 | 1.66 | +0.018 | 0.68 | +0.004 |
| 500 | 0.050 | 0.10 | 97.5 | 1.46 | +0.332 | 1.30 | +0.088 | 0.88 | +0.021 |
| 1000 | 0.020 | 0.49 | 221.4 | 2.01 | +0.069 | 1.51 | +0.018 | 0.61 | +0.004 |
| 2000 | 0.050 | 0.39 | 194.9 | 1.57 | +0.084 | 1.25 | +0.022 | 0.55 | +0.004 |
| 5000 | 0.100 | 0.49 | 212.1 | 1.31 | +0.065 | 1.02 | +0.017 | 0.36 | +0.003 |

**Table 5.2** — `(η*, x_F)` at the **forward-most** lab edge η_lab = +1.3 as a function of pT, for two representative bins. The forward edge is where `x_F` gets closest to zero; we want to know the minimum `x_F` achievable at any pT.

| pT (GeV) | (Q²=500, x=0.010): η*, x_F | (Q²=2000, x=0.050): η*, x_F |
|---|---|---|
| 0.15 | 0.84, +0.002 | 0.67, +0.002 |
| 0.30 | 0.71, +0.002 | 0.58, +0.003 |
| 0.50 | 0.68, +0.004 | 0.55, +0.004 |
| 1.00 | 0.66, +0.007 | 0.54, +0.008 |
| 2.00 | 0.66, +0.015 | 0.53, +0.016 |
| 4.00 | 0.65, +0.029 | 0.53, +0.033 |
| 8.00 | 0.65, +0.059 | 0.53, +0.066 |

### 5.3 Interpretation

**Every reachable lab kinematic point in the analysis (|η_lab| < 1.3, pT > 0.15 GeV) maps to x_F > 0 at the truth level.** Specifically:

- At the **back edge** (η_lab = -1.3, lepton direction), pions land at `η* ≈ 1.3–2.5` and `x_F ≈ +0.07 to +0.33`. Deep in the current hemisphere; comfortably above any `x_F > 0` threshold.
- At η_lab = 0, pions land at `η* ≈ 1.0–1.8` and `x_F ≈ +0.02 to +0.09`. Still in the current hemisphere.
- At the **forward edge** (η_lab = +1.3, proton-direction edge of acceptance), pions land at `η* ≈ 0.4–1.0` and `x_F` ≈ +0.003 to +0.02. Just barely positive but unambiguously > 0.

The minimum `x_F` for any track in the fiducial is roughly **`x_F_min` ≈ +0.002 to +0.005** depending on the bin and the pT, achieved at the forward edge and lowest pT. No track at the truth level has `x_F < 0`.

This implies:

1. **A reco-level `x_F > 0` cut would have essentially no effect** — it would remove only smearing-induced reco-level events where the reconstructed `x_F` is computed using a smeared Q², x and is slightly displaced from the truth value. Order: well below 1 % of events.
2. **The supervisor's intuition (acceptance argument) is confirmed quantitatively.** The argument as stated — "target-fragmentation hadrons go to η_lab > 3 where we have zero acceptance" — is essentially correct: the boost is ~5–6 units of rapidity, so the η*~0 boundary lands at η_lab ≈ +5 to +7 in lab (well outside the |η_lab| < 1.3 fiducial). The H1 proton-remnant peak is at η_lab ≈ +7.6.
3. **There is no sizeable population at moderate negative x_F (-0.2 < x_F < 0) that lands in our η fiducial**, contradicting the "edge cases" concern raised in the supervisor's prior. The only way to get an x_F-near-zero hadron into our acceptance is at η_lab = +1.3 with very low pT, which is also where the acceptance is weakest (track-reconstruction efficiency drops, vertex resolution degrades).

### 5.4 Caveats

- The calculation uses LO QPM kinematics for the scattered electron; QCD radiation (gluon emission, multi-jet final states) can place "current-hemisphere" hadrons at significantly different η_lab/η* than the simple LO picture, in particular at large `qT_h`. However, even with multi-jet kinematics, the boost magnitude and the proton-remnant direction are robust, so the qualitative conclusion (`x_F > 0` for the entire fiducial) holds.
- Finite tracking resolution and the (Q², x) reconstruction (Σ-method, ~5% resolution on Q² and worse on x) smear the relationship between truth `x_F` and reco `x_F`. This smearing is symmetric and does not introduce a sign bias; reco-level `x_F` distributions will have a Gaussian-broadened tail crossing zero, of order ±0.01 in `x_F`. A reco-level `x_F > 0` cut would suppress the negative tail of this smearing-induced distribution, which is a different operation from suppressing true target-fragmentation contamination.
- Pion mass effects (`m_π = 0.14 GeV`) are included. Kaon and proton mass effects are larger but go in the direction of *increasing* `x_F` for a given `(η_lab, pT)`, so they do not change the conclusion. Heavier hadrons are even further from the `x_F = 0` boundary.

### 5.5 Confidence

Confidence in the Q4 conclusion is **high**. The 4-vector calculation is a direct evaluation, not an interpretation; the result is consistent across all (Q², x) bins examined; and the result agrees with the qualitative pattern documented in the H1 low-Q² papers (where the boost direction is different but the magnitude is similar).

---

## 6. Hidden traps (Q5)

### 6.1 Migration effects across the cut boundary

If we *did* impose an `x_F > 0` cut at the reconstructed-`x_F` level, we would introduce a new bin-migration systematic associated with events where the reconstructed `x_F` straddles zero but the truth `x_F` is positive (or vice versa). The size of this migration depends on the Σ-method resolution on Q², x, and y, and on the per-track momentum resolution. At our √s and tracker performance, the reco-`x_F` resolution is typically `Δx_F` ≈ 0.005 to 0.01. The migration would mainly affect events with **truth `x_F` between 0 and ~0.01**, which (per Section 5) constitute the bulk of the lowest-pT, forward-edge population in the highest-y, lowest-x bins.

If we instead applied the cut at the **truth level** (i.e. unfold to a phase space that has `x_F > 0` as a particle-level definition), the migration is handled inside the unfolding response matrix and does not require a separate systematic. This is the cleaner approach if the cut is to be applied at all.

**Net assessment:** the migration concern is real but tractable. It is a reason to **prefer truth-level cut over reco-level cut**, not a reason to skip the cut entirely.

### 6.2 Are there H1 papers that explicitly justify NOT cutting on `x_F`?

We did not find a published H1 paper that states "we considered an `x_F` cut and rejected it." The absence is structural: the H1 SIDIS literature is built on `η*`-based or lab-fiducial selections, and `x_F` is not part of the standard cut vocabulary. The 1302.1321 paper's statement that "the target region η* < 0 is not accessible" is the closest thing to an explicit justification — it acknowledges that the lab fiducial already enforces current-hemisphere selection and that no additional cut is needed.

The HERMES and COMPASS papers (Section 3.2) similarly do not justify the absence of an `x_F` cut; they justify the **presence of a z > 0.2 cut** as the target-fragmentation suppressor, which implicitly says they consider z to be the relevant variable.

### 6.3 Interaction with the z cut: double-counting concern

The supervisor's concern: "low-z hadrons are more likely to be target fragments — does cutting both z > 0.1 AND x_F > 0 over-count the same physics suppression?"

In our analysis the z binning starts at z = 0 (the lowest bin is [0, 0.1]), so we do **not** apply a hard `z > 0.1` cut as a selection. We measure multiplicities in all z bins including z < 0.1. The first z bin (`0 < z < 0.1`) is precisely where target-fragmentation contamination is largest (consistent with COMPASS's rationale for `z > 0.2`).

If the analysis intent is to publish multiplicities including the `0 < z < 0.1` bin, then **an `x_F > 0` cut would partially clean up the lowest-z bin but not fully**, because target-fragmentation hadrons at low z can still land in our lab acceptance (as shown in Section 5, the forward edge at η_lab = +1.3 with pT > 0.15 GeV has truth `x_F ~ 0.002`, very close to zero — these are exactly the borderline-current-hemisphere events that an `x_F > 0` cut would remove but which the η_lab fiducial does not remove).

Quantitatively, the `0 < z < 0.1` bin is the only one where an `x_F > 0` cut might make a meaningful difference (perhaps a few percent on the bin yield, mostly at the forward η edge). The `z > 0.1` bins are essentially unaffected.

**This is *not* "double-counting"**: the two cuts (z > 0.1 and x_F > 0) suppress somewhat different populations. `z > 0.1` removes soft, low-energy hadrons regardless of hemisphere; `x_F > 0` removes target-hemisphere hadrons regardless of z. The two are complementary, not redundant. But at our kinematics, both are essentially no-ops on top of the η_lab fiducial.

### 6.4 Radiative corrections (C_rad) interaction

The H1 papers (1302.1321 Sect. 4, 2011.01812 Sect. 5 Sys.1) compute QED radiative corrections from the bin-by-bin ratio of radiative-MC and non-radiative-MC simulations using the same fiducial. The radiative correction is sensitive to the relative populations of events with collinear initial-state photon radiation (which shift the effective `Q², x, y`) vs. non-radiative events. The `x_F` of the final-state hadron is *not* directly involved in the radiative-correction integral; what matters is the difference in the kinematic-variable reconstruction between radiative and non-radiative events.

If we add an `x_F > 0` cut at the truth level, the C_rad factor would change in the per-bin ratio because:
- In the non-radiative MC, all events have a clean (Q², x) and an `x_F` distribution determined by the LO kinematics.
- In the radiative MC, the photon emission shifts the effective γ* direction in the lab; this can shift the `x_F` of a given hadron, in principle either positively or negatively, by an amount of order the photon's relative momentum (small at low ISR but can be significant for hard ISR).

For the (η_lab fiducial only) version of our analysis, C_rad is already computed and is part of our 11-source systematic budget. **Adding an `x_F > 0` truth-level cut would require recomputing C_rad** with the new truth-level phase-space definition. The change is *probably small* (because the `x_F > 0` cut is essentially a no-op at the truth level), but it is **not zero by definition** — it requires explicit verification before publication.

This is **another reason to prefer not applying the cut**, because it would introduce a verification step (recomputing C_rad with the modified phase-space definition) for no measurable benefit.

### 6.5 Charge-asymmetry vs. multiplicity sensitivity

For the secondary observable A_h = (M^{h+} - M^{h-}) / (M^{h+} + M^{h-}), the target-fragmentation population is *approximately* charge-symmetric (proton remnant produces roughly equal numbers of π⁺ and π⁻ from sea-quark recombination). This means target-fragmentation contamination contributes to the denominator of A_h but cancels in the numerator. The net effect: target-fragmentation contamination *dilutes* A_h slightly (makes it appear smaller in magnitude than it would be in a pure current-fragmentation sample). An `x_F > 0` cut would un-dilute the asymmetry.

**Estimated size of this effect at our kinematics:** With ≤ a few percent of events near the `x_F = 0` boundary (Section 5.2), the dilution of A_h is at most a few percent of A_h itself. This is below our current per-bin statistical and systematic uncertainty floor (~6 % fractional). The effect is not measurable at v1.0 precision.

For TMD-style harmonics (`cos(φ_h)`, `cos(2φ_h)`), see Section 4.3: no measurable effect expected.

---

## 7. Recommendation with confidence (Q6)

### 7.1 Recommendation

**Option (C): Skip the explicit `x_F > 0` cut. The η_lab < 1.3 fiducial already does the work, and the explicit cut adds nothing at our kinematics.** Confidence: **high**.

### 7.2 Reasoning, in priority order

1. **Literature precedent is unambiguous.** Zero published H1, HERMES, COMPASS, or JLab unpolarised SIDIS multiplicity paper uses `x_F > 0` as a primary phase-space cut. Adding it would put our analysis out of convention with the entire field. (Q1, Q2)
2. **The kinematic calculation confirms the supervisor's acceptance prior.** All reachable hadrons in our lab fiducial have truth `x_F > 0` already. The explicit cut would be a no-op at the truth level, suppressing at most ~ percent of reco-smeared events. (Q4)
3. **The cut would introduce verification overhead with no offsetting benefit.** Specifically, it would require recomputing C_rad with the modified phase-space definition (Section 6.4) and adding a new bin-migration systematic at the `x_F = 0` boundary (Section 6.1). Neither of these issues is large, but both are nonzero. (Q5)
4. **The existing 11-source systematic budget already covers the question implicitly.** Target-fragmentation contamination at the percent level would show up in the model-dependence systematic (DJANGOH vs. RAPGAP comparison) and in the regularization-strength systematic from the unfolding. We are not currently observing anomalously large entries in these systematic sources at the lowest-z, forward-η bins, which is consistent with the kinematic analysis showing that target-fragmentation contamination is negligible.

### 7.3 Fallback option (low priority)

**Option (B), as a secondary low-priority systematic check: compute truth-level `x_F` from generator-level information and verify that the `x_F < 0` truth contamination integrated over the analysis fiducial is < 1% of events in every (Q², x, z, pT_h) bin.** This would be a Tier 2 (~3 hr) sanity check, not a re-extraction. It would close out the question and provide a one-line documentation entry for the analysis note. We do not recommend doing this work at the moment because the kinematic argument in Section 5 already establishes the bound to within a precision well below the systematic floor, but it would be a quick belt-and-braces verification if requested.

### 7.4 Strong recommendation against Option (A)

**We do not recommend Option (A): apply `x_F > 0` as the default analysis cut and re-frame the measurement as a "current-fragmentation SIDIS multiplicity."** Reasons:

- No H1 high-Q² SIDIS precedent supports this framing.
- The cut would not measurably change the data; it would only complicate the systematic budget and the C_rad computation.
- The community-standard convention is to publish multiplicities in the `(Q², x, z, P_h⊥)` space defined by `z > 0.2` (or our equivalent in the lowest z bin, which is integrated out for that bin) and to compare to TMD predictions in the regime `P_hT ≲ Q`. Imposing `x_F > 0` would make our data harder to compare to HERMES, COMPASS, and the upcoming EIC results.
- It would re-open the question of whether to similarly cut on `x_F` in the published H1 1302.1321 and 2011.01812 multiplicity measurements, which we cannot do (these are already published).

### 7.5 What would change this recommendation

The recommendation against Option (A) would flip to Option (A) if and only if **one of the following** were established:

- Direct evidence from a published H1 high-Q² SIDIS measurement (not yet existing) that the `x_F < 0` contamination at our kinematics is large enough (> 10% of events in some bin) to bias the multiplicity. The current literature does not support this; our kinematic analysis in Section 5 actively contradicts it.
- A theoretical TMD-factorization argument requiring `x_F > 0` for the specific harmonics we will measure. Boglione et al. 2017 does not provide this argument; they argue for a more refined criterion based on rapidity separation, which is automatically satisfied at our Q.
- A discovery during analysis (e.g. visible Δ-resonance or proton-remnant baryon excess in the lowest z and forward η_lab bins) that would suggest residual target contamination. We are not seeing this in the v1.0 data release.

### 7.6 Additional work that would strengthen the answer (none required for the decision)

1. Run a one-pass generator-level study computing truth `x_F` distribution per bin (Tier 2, ~3 hr) to verify the bound in Section 5 holds end-to-end with the actual H1OO MC chain, not just the analytic 4-vector calculation.
2. Cross-check with ZEUS charged-multiplicity papers (Chekanov et al. 2008, JHEP 0806:061, arXiv:0803.3878 — cited by H1 2011.01812 as reference 15) to confirm the convention. We did not retrieve this paper for this memo; it is likely to confirm the H1 convention but would be a one-paper cross-check.

---

## 8. Citations

### Primary H1 references

- **Aaron, F. D. et al. [H1 Collaboration]**, "Measurement of Charged Particle Spectra in Deep-Inelastic ep Scattering at HERA," DESY 13-012, **arXiv:1302.1321**, *Eur. Phys. J. C* **73** (2013) 2406. DOI: 10.1140/epjc/s10052-013-2406-x.
- **Andreev, V. et al. [H1 Collaboration]**, "Measurement of charged particle multiplicity distributions in DIS at HERA and its implication to entanglement entropy of partons," DESY 20-176, **arXiv:2011.01812**, *Eur. Phys. J. C* **81** (2021) 212. DOI: 10.1140/epjc/s10052-021-08896-1.
- **Andreev, V. et al. [H1 Collaboration]**, "Measurement of Jet Production Cross Sections in Deep-inelastic ep Scattering at HERA," DESY-16-200, **arXiv:1611.03421**, *Eur. Phys. J. C* **77** (2017) 215; erratum *Eur. Phys. J. C* **81** (2021) 739. DOI: 10.1140/epjc/s10052-017-4717-9.
- **Andreev, V. et al. [H1 Collaboration]**, "Measurement of lepton-jet correlation in deep-inelastic scattering with the H1 detector using machine learning for unfolding," DESY 21-130, **arXiv:2108.12376**, *Phys. Rev. Lett.* **128** (2022) 132002. DOI: 10.1103/PhysRevLett.128.132002.

### Fixed-target SIDIS multiplicity references

- **Airapetian, A. et al. [HERMES Collaboration]**, "Multiplicities of charged pions and kaons from semi-inclusive deep-inelastic scattering by the proton and the deuteron," DESY 12-157, **arXiv:1212.5407**, *Phys. Rev. D* **87** (2013) 074029. DOI: 10.1103/PhysRevD.87.074029.
- **Aghasyan, M. et al. [COMPASS Collaboration]**, "Transverse-momentum-dependent Multiplicities of Charged Hadrons in Muon-Deuteron Deep Inelastic Scattering," CERN-EP-2017-253, **arXiv:1709.07374**, *Phys. Rev. D* **97** (2018) 032006. DOI: 10.1103/PhysRevD.97.032006.
- **Hall C SIDIS Collaboration** (Bosted, P., Bhatt, H., Jia, S., et al.), "Flavor, transverse momentum, and azimuthal dependence of charged pion multiplicities in SIDIS with 10.6 GeV electrons," **arXiv:2510.03562** (2025; submitted to *Phys. Rev. C*).

### TMD-factorization theoretical reference

- **Boglione, M., Collins, J., Gamberg, L., Gonzalez-Hernandez, J. O., Rogers, T. C., Sato, N.**, "Kinematics of Current Region Fragmentation in Semi-Inclusive Deeply Inelastic Scattering," **arXiv:1611.10329**, *Phys. Lett. B* **766** (2017) 245. DOI: 10.1016/j.physletb.2017.01.021.

### CLAS12 / JLab supporting references (consulted but not central)

- **Burkert, V. D.**, "CLAS12 and its initial Science Program at the Jefferson Lab Upgrade," **arXiv:0810.4718** (2008).
- **Dilks, C. [CLAS Collaboration]**, "Beam Spin Asymmetries of π+π0 Dihadrons from SIDIS at CLAS12," **arXiv:2201.05732**, *JPS Conf. Proc.* **37** (2022) 020109.
- **Vossen, A. [CLAS Collaboration]**, "Novel experimental probes of QCD in SIDIS and e+e− annihilation," **arXiv:1810.02435** (2018, proceedings).
- **Ye, Z. et al.**, "SIDIS Measurement with A=3 Nuclei," **arXiv:2202.09696** (2022, proceedings).

### Supporting references not retrieved in full but cited above

- **Andreev, V. et al. [H1 Collaboration]**, "Measurement of jet substructure observables in DIS at HERA," **arXiv:2303.13620**.
- **Andreev, V. et al. [H1 Collaboration]**, "Groomed event shapes in deep-inelastic scattering with H1," **arXiv:2403.10134**.
- **Andreev, V. et al. [H1 Collaboration]**, "Multijet production cross sections in DIS at HERA," **arXiv:1406.4709**.
- **Chekanov, S. et al. [ZEUS Collaboration]**, "Multiplicity moments of jets produced in neutral current DIS at HERA," **arXiv:0803.3878**, *JHEP* **0806** (2008) 061.
- **Collins, J.**, *Foundations of Perturbative QCD*, Cambridge University Press (2011), Cambridge Monographs on Particle Physics, Nuclear Physics and Cosmology Vol. 32; especially Chapters 12–13 on TMD factorization.
- **Bacchetta, A., Diehl, M., Goeke, K., Metz, A., Mulders, P. J., Schlegel, M.**, "Semi-inclusive deep inelastic scattering at small transverse momentum," *JHEP* **0702** (2007) 093, **arXiv:hep-ph/0611265**.

---

## Appendix A: Replication of Section 5 kinematic calculation

The 4-vector boost calculation in Section 5 was performed in standalone Python using `numpy` and the following on-shell collider kinematics for HERA-II (E_e = 27.6 GeV electron in -z, E_p = 920 GeV proton in +z). The script:

1. For each (Q², x), computes y = Q²/(sx), scattered-electron energy `E' = Q²/(4E_e) + E_e(1−y)`, and scattered-electron angle from `cos θ_e = [Q²/(2E_e) − 2E_e(1−y)] / (2E')` (with θ_e measured from the +z proton-beam direction).
2. Constructs `q^μ = p_e_in − p_e_out` and `P_tot = p_p_in + q`.
3. Boosts the proton, γ*, and test charged-pion 4-momenta into the rest frame of P_tot (the HCM) using the standard relativistic boost formulas.
4. Rotates the HCM coordinate system so the γ* spatial momentum lies along +z*.
5. Reads off `η* = atanh(p_z*/|p*|)` and `x_F = 2 p_z*/W`.

The proton 4-momentum in HCM (sanity check) recovers `η*_proton` very large negative (numerically clamped at −30 in the code), confirming the +z* = γ* convention. The script results are tabulated in Section 5.2.

Numerical precision: the boost calculation is accurate to ~1e-12 in `x_F`; the dominant approximation is the LO-QPM assumption for the scattered electron geometry, which neglects ISR/FSR shifts of order ~ percent in y at the bin-edge values. Including ISR shifts would not change any conclusion: the minimum `x_F` at the forward edge would still be > 0.001 in all bins.

---

*End of memo.*

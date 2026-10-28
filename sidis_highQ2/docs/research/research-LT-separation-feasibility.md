<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Feasibility of a SIDIS Rosenbluth-style L/T separation with the H1 HERA-II low-Q² dataset

**Scope.** Quantitative feasibility study for measuring the unpolarized SIDIS
structure functions F_UU,T and F_UU,L (and the ratio R_SIDIS = F_UU,L/F_UU,T)
from charged-hadron production in the H1 low-Q² fiducial
(5 < Q² < 100 GeV², 0.1 < y < 0.6, |η_lab| < 1.3, p_T^lab > 0.15 GeV,
0.1 < z < 0.7, 0 < p_T^h < 2 GeV) by exploiting the three HERA-II proton
beam energies (E_p = 920, 575, 460 GeV).

**Bottom line in advance.** Statistically marginal but not impossible at the
inclusive (case a) and (z,Q²)-binned (case b) levels; the differential
TMD case (c) is statistics-limited at HER and severely so at MER/LER. The
fit is dominated by point-to-point normalization systematics: a single 2%
relative-luminosity / efficiency offset between HER and LER inflates
σ(R) by an order of magnitude at the expected SIDIS R values
(R ≲ 0.1). The largest practical risk is that MER (5.9 pb⁻¹) and LER
(12.2 pb⁻¹) SIDIS-builder samples and the corresponding MC do not exist
in the H1 production system and would need to be produced from scratch.
Sources: arXiv:1012.4355, arXiv:1312.4821, arXiv:1302.1321,
arXiv:hep-ph/0611265, arXiv:1212.5407.

---

## 1. Physics formalism

### 1.1 One-photon-exchange unpolarized SIDIS cross section

Following Bacchetta, Diehl, Goeke, Metz, Mulders, Schlegel,
JHEP 0702:093 (2007) [arXiv:hep-ph/0611265], the unpolarized
SIDIS cross section in the one-photon-exchange approximation (eq. 2.7,
restricted to the unpolarized-beam, unpolarized-target sector) is

```
dσ / [ dx_B dy dψ dz_h dφ_h d(P_hT)² ]
  =  α² / (xy Q²) · y² / (2(1-ε)) · ( 1 + γ²/(2x) )
      × {  F_UU,T
         + ε F_UU,L
         + √( 2 ε (1+ε) ) cos(φ_h) F_UU^{cos φ_h}
         + ε cos(2 φ_h) F_UU^{cos 2φ_h}
        }
```

with γ = 2Mx/Q (negligible at HERA collider kinematics, γ² ≪ 1), dψ ≈ dφ_S
absent for unpolarized target, and the structure functions
F_UU,T(x,z,P_hT,Q²), F_UU,L(x,z,P_hT,Q²) etc. defined in eqs. 2.8-2.13 of
Bacchetta et al. The two φ_h-modulated terms (Cahn × DSS, Boer-Mulders ×
Collins) integrate to zero in the φ_h-integrated cross section.

The longitudinal photon polarization parameter (Hand convention, with
the small-y γ² correction neglected) is

```
ε(y) = (1 - y - γ² y²/4) / (1 - y + y²/2 + γ² y²/4)
     ≈ (1 - y) / (1 - y + y²/2)             [γ ≪ 1, collider limit]
```

### 1.2 Tabulated ε at the three HERA-II proton-beam energies

Centre-of-mass energies used: HER √s = 318.7 GeV, MER √s = 252.0 GeV,
LER √s = 225.4 GeV. y = Q²/(s·x).

Representative points (small x ≈ 10⁻³ and moderate x ≈ 10⁻²) at Q² = 10
and Q² = 30 GeV²:

| Q² | x | y_HER | y_MER | y_LER | ε_HER | ε_MER | ε_LER | Δε (HER−LER) | All in 0.1 < y < 0.6? |
|---:|---:|------:|------:|------:|------:|------:|------:|-------------:|:--:|
| 10 | 1·10⁻³ | 0.098 | 0.157 | 0.197 | 0.995 | 0.985 | 0.976 | **0.019** | no (HER below 0.1) |
| 10 | 1·10⁻² | 0.010 | 0.016 | 0.020 | 1.000 | 1.000 | 1.000 | 0.000 | no |
| 30 | 1·10⁻³ | 0.295 | 0.472 | 0.590 | 0.942 | 0.825 | 0.701 | **0.241** | **yes** |
| 30 | 3·10⁻³ | 0.098 | 0.157 | 0.197 | 0.995 | 0.985 | 0.976 | **0.019** | no (HER below 0.1) |
| 30 | 1·10⁻² | 0.030 | 0.047 | 0.059 | 1.000 | 0.999 | 0.998 | 0.001 | no |

The lever arm Δε scales rapidly with y; at the upper-y edge it can
approach 0.25–0.3. At y < 0.2 it is essentially zero. The
moderate-x bins (x ≳ 10⁻²) yield no useful separation across the H1 low-Q²
fiducial because y is too small at all three energies.

### 1.3 Rosenbluth-like separation

At fixed (x, Q², z, P_hT) the φ_h-integrated reduced SIDIS cross section
(after dividing out the y-prefactor, α², Q², radiative corrections, and
acceptance) is **linear in ε**:

```
σ̃_SIDIS(x,Q²,z,P_hT; ε)  =  F_UU,T(x,Q²,z,P_hT) + ε · F_UU,L(x,Q²,z,P_hT)
```

A three-point linear fit over (ε_LER, ε_MER, ε_HER) yields intercept
F_UU,T and slope F_UU,L. R_SIDIS = F_UU,L / F_UU,T.

### 1.4 Region of triple-energy overlap inside the fiducial

A bin is usable for a 3-point Rosenbluth fit only if 0.1 < y < 0.6 at
**all three** energies. Combined with 5 < Q² < 100 GeV² this requires:

- the LER constraint y_LER < 0.6 → x > Q²/(s_LER · 0.6) = Q²/30483
- the HER constraint y_HER > 0.1 → x < Q²/(s_HER · 0.1) = Q²/10157

i.e. **Q²/30483 < x < Q²/10157**, or equivalently **0.333 < y_HER < 1**
combined with the s_LER/s_HER ≈ 0.50 factor compressing the usable y
window. Concretely (full Q² × x sweep):

| Q² [GeV²] | x window | y_HER range | y_MER range | y_LER range | Δε(HER−LER) at midpoint |
|-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
|  10 | none in fid                           |  –  |  –  |  –  | – |
|  15 | 5·10⁻⁴ … 1.5·10⁻³                     | 0.10 – 0.30 | 0.16 – 0.48 | 0.20 – 0.59 | ≈ 0.05 |
|  20 | 7·10⁻⁴ … 2·10⁻³                       | 0.10 – 0.30 | 0.16 – 0.47 | 0.20 – 0.59 | ≈ 0.09 |
|  30 | 1·10⁻³ … 3·10⁻³                       | 0.10 – 0.30 | 0.16 – 0.47 | 0.20 – 0.59 | 0.05–0.24 |
|  50 | 1.6·10⁻³ … 5·10⁻³                     | 0.10 – 0.31 | 0.16 – 0.50 | 0.20 – 0.62 | 0.02–0.15 |
| 100 | 3·10⁻³ … 1·10⁻²                       | 0.10 – 0.33 | 0.16 – 0.52 | 0.20 – 0.66 | 0.01–0.10 |

**The triple-overlap region is a narrow diagonal band in the (x, Q²)
plane** with x ≈ Q²/15000 ± a factor of ~2. Inside it, Δε ranges from
near zero (lower-y edge) to ~0.25 (upper-y edge). **There are at most
~5 simultaneously useful (x, Q²) cells** with the SIDIS handoff's
nominal binning, and they sit at the high-y / low-x corner where
statistics shrink and radiative corrections are largest.

---

## 2. Luminosity and statistics

### 2.1 Recorded integrated luminosities (primary sources)

All values are H1 SpaCal-trigger low-Q² selections (2003–2007 collection
period). The 575/460 GeV programs ran only in 2007.

| Sample | √s [GeV] | E_p [GeV] | Year | L [pb⁻¹] | Normalization unc. | Source |
|:------:|:--------:|:---------:|:----:|---------:|:------------------:|:------:|
| HER (low-Q²) | 318.7 | 920 | 2003–06 | **103.5**  | 3% | arXiv:1012.4355 (Table 1, Section 3) |
| MER          | 252.0 | 575 | 2007    | **5.9**    | 4% | arXiv:1012.4355 (Table 1) |
| LER          | 225.4 | 460 | 2007    | **12.2**   | 4% | arXiv:1012.4355 (Table 1) |

For cross-check, the H1 high-Q² LAr-trigger FL paper [arXiv:1312.4821,
Table 1] quotes 11.8 pb⁻¹ (LER) and 5.4 pb⁻¹ (MER) for the high-Q²
selection; the small differences are HV/run-quality cuts.

The predecessor charged-particle low-Q² paper [arXiv:1302.1321] used
**88.6 pb⁻¹** of 2006-only HER 920 GeV data — same fiducial we propose,
but only one beam energy. **No L/T separation was attempted there.**

The combined HERA-II low-Q² SIDIS sample on disk at NAF (per AGENTS.md
note that the workspace lumi is the superseded pre-rescale luminosity nb⁻¹ ≈ 297.7 pb⁻¹) reflects
the full HERA-II inclusive low-Q² catalogue including the dedicated
high-y subsets used by H1 in the FL analysis. The 103.5 / 5.9 / 12.2
pb⁻¹ split is what is relevant for a 920/575/460 partitioning.

### 2.2 Inclusive DIS rate per pb⁻¹

A simple but defensible anchor: 1302.1321 collected its full
5 < Q² < 100, 0.05 < y < 0.6, 10⁻⁴ < x < 10⁻² sample at 88.6 pb⁻¹ HER.
The published bin yields and the binning (their Tables 2–9, 8 (x,Q²)
bins × ~20 (η*,p_T*) cells, with per-cell densities of O(10⁻¹) per
event) imply an order of magnitude of ~1.5–2 × 10⁶ selected DIS events
in this fiducial, i.e. **~20,000 DIS events per pb⁻¹**. The per-event
cross section depends weakly on √s at low x (the dominant Q² dependence
1/Q⁴ is shared by all energies; only the s-dependent y-acceptance and
the (Y_+ − y² F_L) factor differ at the few-per-cent level at low x),
so I take the same rate per pb⁻¹ at MER and LER for an order-of-
magnitude yield estimate.

The hadron-to-event multiplicity in the narrower analysis fiducial
|η_lab| < 1.3, p_T > 0.15 GeV is ≈ 2–3 charged hadrons per DIS event
(extrapolated from the 1302.1321 η*-pT* tables, restricted to the
central CTD acceptance and the |η|<1.3 cut).

### 2.3 Raw yields (order of magnitude)

| Sample | L [pb⁻¹] | N_DIS in fid (5 < Q² < 100) | N_charged_h (|η|<1.3, p_T>0.15) |
|:------:|---------:|----------------------------:|------------------------------:|
| HER | 103.5 | 2.07 × 10⁶ | 5.2 × 10⁶ |
| MER |   5.9 | 1.18 × 10⁵ | 3.0 × 10⁵ |
| LER |  12.2 | 2.44 × 10⁵ | 6.1 × 10⁵ |

In the triple-overlap (x, Q²) corner (≈ 5 cells out of ~16 in
(x, Q²) — i.e. a fraction ≈ 0.25–0.4 of the full sample lives in that
diagonal band), distributed over 4 z bins × 5 p_T^h bins × 2 charges =
40 cells:

| Sample | Cells in overlap × z × p_T^h × charge | N_h per cell (overlap fraction = 0.25) | Stat. unc. per cell |
|:------:|:-----:|---------:|----:|
| HER | 5 × 40 = 200 cells | ~6,500 | 1.2% |
| MER | 200 cells | ~370 | 5.2% |
| LER | 200 cells | ~760 | 3.6% |

**MER is the bottleneck.** Below ~100 events per cell the linear fit
becomes ill-conditioned regardless of lever arm.

### 2.4 Bins that have enough events for a meaningful ε-fit

Adopting "≥ 100 events per cell at every beam energy" as the floor for
a meaningful ε-fit:

- **(a) Inclusive (Q² only, integrated over x,z,p_T^h):** all
  Q² bins in 5–100 GeV² have ≫ 100 hadrons at MER and LER. Statistics
  not the limiter; **systematics are**.
- **(b) (z, Q²) integrated over x and p_T^h:** OK at MER for Q² ≤ 30
  GeV² with the wider z bins; marginal in the highest-z bin at MER.
- **(c) Differential in (z, p_T^h) at fixed (x, Q²):** OK only at HER;
  MER drops below 100 events per cell in roughly half the cells; LER
  is marginal in the highest p_T^h bins. **Not feasible** for the
  full 40-cell TMD grid at MER without bin coarsening.

---

## 3. Statistical precision of the L/T fit

### 3.1 Toy 3-point linear fit

For each yield scenario I solve a weighted linear fit
σ̃_i = F_T + ε_i F_L over i ∈ {LER, MER, HER}, propagating
Poisson per-bin uncertainties on the measured σ̃_i. ε values are
taken at Q² = 30, x = 10⁻³ where the lever arm is largest in the
triple-overlap region: ε = (0.701, 0.825, 0.942) for (LER, MER, HER).
F_T is normalized to 1; R_true = F_L/F_T is varied.

#### Statistical-only (no systematics)

| Case | N(LER) | N(MER) | N(HER) | σ(R)/R, R=0.05 | σ(R)/R, R=0.10 | σ(R)/R, R=0.20 |
|:----:|------:|------:|------:|----:|----:|----:|
| (a) Inclusive R per Q² (Q²=20–50, integ. x,z,p_T^h, both charges) | 36,600 | 17,700 | 310,500 | **48%** | **26%** | **15%** |
| (b) R per (z, Q²) bin (4 z bins) | 9,200 | 4,400 | 77,600 | 96% | 52% | 30% |
| (c) R per (z, p_T^h) cell at fixed (x,Q²), per charge (40 cells) | 920 | 440 | 7,760 | 303% | 164% | 95% |

#### With ±2% point-to-point normalization systematic added in quadrature

| Case | σ(R)/R, R=0.05 | σ(R)/R, R=0.10 | σ(R)/R, R=0.20 |
|:----:|----:|----:|----:|
| (a) Inclusive | 259% | 140% | 81% |
| (b) (z, Q²)   | 273% | 147% | 85% |
| (c) (z, p_T^h)| 398% | 215% | 125% |

### 3.2 Interpretation

- The **statistical precision of the inclusive case (a)** for R ≈ 0.1
  is ~26%, comparable to or better than the inclusive F_L precision
  achieved by H1 in the same energy scan (the H1 2010 paper
  arXiv:1012.4355 reports σ(F_L) ≈ 0.02–0.06 with R ≈ 0.2–0.3,
  i.e. σ(R)/R ≈ 10–30%). This is a useful sanity check that the
  SIDIS L/T separation is at least conceivable.
- **A single 2% point-to-point normalization systematic destroys the
  measurement**: σ(R)/R balloons from O(25%) to O(140%) at R=0.1.
  This is the central conclusion of the feasibility study: the
  experiment is systematics-, not statistics-limited.
- The (z, p_T^h) differential TMD case (c) is statistics-limited at
  MER and not viable in its naive form. Coarsening to 2 z bins × 3
  p_T^h bins would bring per-cell yields to ~75 (MER) and would still
  give σ(R)/R > 100% at R=0.1.

### 3.3 Inclusive-FL sanity check

H1 2010 (arXiv:1012.4355) and 2014 (arXiv:1312.4821) achieved
σ(F_L)_inclusive ≈ 0.02–0.06 in the range 1.5 < Q² < 800 GeV² using
exactly the (HER, MER, LER) energy scan, with R_inclusive ≈ 0.2–0.3
(consistent with the value R = 0.26 explicitly assumed in the H1 2010
combination of cross sections). The SIDIS extension scales the per-bin
yields by the hadron multiplicity (×2–3) but partitions them across
~40 (x, Q², z, p_T^h) cells per charge: the gain on per-cell
statistics is at most ~2× for the inclusive integral (case a) and a
~10–20× loss for the differential cells (case c). The systematic
budget is also harder for SIDIS because tracking and HFS-reconstruction
uncertainties enter on top of the SpaCal-electron scale.

---

## 4. Systematics that DO NOT cancel in the L/T fit

A Rosenbluth-style separation is exquisitely sensitive to
**any uncertainty that differs between the three energy points**.
Sources that fully cancel between HER, MER, LER are NOT a concern.
Sources that do NOT cancel and dominate the error budget are:

### 4.1 Catalogue of point-to-point systematics

| Source | Per-energy uncertainty | Correlated across energies? | Net point-to-point uncertainty |
|:-------|:----------------------:|:---------------------------:|:------------------------------:|
| Integrated luminosity | 3% (HER), 4% (MER, LER) [arXiv:1012.4355 Sec. 3] | partial (HER uses 2003–06 system, MER/LER use 2007 system) | **2–3%** |
| SpaCal electromagnetic energy scale | 1% [arXiv:1302.1321 Sec. 5; arXiv:1012.4355] | partial (stability across 2003–07) | **0.5–1%** |
| LAr hadronic energy scale | 2% [arXiv:1302.1321 Sec. 5] | partial | 0.3–0.5% on σ̃ |
| SpaCal trigger efficiency | < 1% (negligible in 1302.1321 fiducial) | mostly correlated | ≲ 0.5% |
| Central tracking efficiency | 1% (central tracks, 1302.1321) | mostly correlated (same CTD); 2007 alignment changes possible | 0.5–1% |
| Radiative corrections C_rad | 1% on σ_inclusive [1012.4355 Sec. 3.5]; up to 5% in high-y SIDIS cells | **uncorrelated by construction** (C_rad differs per E_p) | **1–3%** |
| MC model dependence (Rapgap vs Djangoh) for acceptance | 0.2–5.5% in 1302.1321 | partially correlated if same generators used | 1–2% |
| MC reweighting for y, p_T spectra | included above | – | – |
| Photoproduction subtraction (30%) | up to 0.9% at low x,Q² | correlated in mechanism, but rate differs with s | 0.3–0.5% |

**Total point-to-point envelope: 2–4%**, dominated by relative
luminosity and energy-dependent radiative corrections. The 2% number
used in §3 is therefore on the optimistic side.

**Important caveat — broken luminosity cancellation.** The current H1
SIDIS workspace's predecessor analysis (arXiv:1302.1321 and the H1
low-Q² multiplicity work documented in
`H1/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md`)
quotes **no luminosity systematic** because the observable is a
per-event ratio (track count / DIS event count) in which L cancels.
**That cancellation is broken in a Rosenbluth-style L/T separation**:
the reduced SIDIS cross section σ̃_SIDIS = F_T + ε F_L is an
absolutely-normalized cross section that must be compared across
HER/MER/LER, so the per-energy absolute lumi enters directly. The
proposed L/T measurement therefore inherits the *full* H1 inclusive
luminosity uncertainty (3% HER, 4% MER/LER) rather than the
multiplicity-style "no luminosity systematic" of the predecessor.
This is a fundamental difference between the multiplicity workstream
and an L/T workstream.

### 4.2 Energy-dependent radiative corrections

C_rad must be computed separately per beam energy using DJANGOH14 with
HERACLES interfaced ISR/FSR; the high-y, low-x SIDIS fiducial is
precisely where radiative corrections to the Born cross section can
reach 10% (arXiv:1012.4355 Sec. 3.5) and locally exceed 50% in extreme
high-y bins. Even a 1% mis-modelling of C_rad at the upper-y edge of
LER directly biases ε_LER → ε_LER × (1 ± 0.01) → biases F_L
extraction.

### 4.3 Quantified impact of a 2% norm uncertainty on σ(R)

Already tabulated in §3.1: at R = 0.1 (the most relevant value for
SIDIS at HERA), a 2% point-to-point normalization scaling **raises
σ(R) from 0.026 to 0.140 in the inclusive case (a)** — an order of
magnitude inflation. For the inclusive case the SIDIS L/T separation
becomes a 1.5σ measurement of R rather than a 4σ measurement.

To realise the statistical precision implied by case (a), the
HER/MER/LER point-to-point normalization (lumi × trigger × tracking ×
radiative) must be controlled to **better than 1%** — a tighter
requirement than was achieved in the H1 inclusive-FL papers.

---

## 5. SIDIS-specific complications

### 5.1 Expected size of R_SIDIS at H1 kinematics

For the inclusive structure function H1 itself uses R_inclusive ≈ 0.25
(with weak Q² dependence) in the relevant low-Q² regime
(arXiv:1012.4355 fits). For SIDIS:

- **HERMES proton/deuteron multiplicity data** [arXiv:1212.5407]:
  HERMES did not directly publish R_SIDIS, but the φ_h-integrated
  multiplicities are consistent with a small F_UU,L contribution
  (R_SIDIS ≲ 0.05–0.10 at z ≳ 0.2, P_hT ≲ 1 GeV) and a leading
  F_UU,T from collinear DGLAP-factorized PDF×DSS-FF. HERMES operated
  at √s ≈ 7.3 GeV, very different from HERA collider kinematics.
- **JLab fixed-target experiments** have measured R_SIDIS at much
  lower Q² (typically Q² ≈ 1–5 GeV² at JLab 6 and 12 GeV) and find
  R_SIDIS that is small but with significant z- and p_T-dependence
  consistent with subleading-twist enhancement at low Q². These
  cannot be extrapolated cleanly to HERA collider kinematics where
  Q² ≫ ⟨k_T²⟩ and the TMD-evolved F_UU,L is power-suppressed.
- **pQCD/TMD expectations**: in the TMD framework F_UU,L at leading
  power is generated by the twist-3 quark-gluon correlator (Cahn-like
  contributions) plus the NLO α_s correction to the partonic
  coefficient function; the latter dominates at moderate Q² and gives
  R_SIDIS ~ α_s(Q²)/π × O(1) ≈ 0.03–0.10.

**Best estimate of R_SIDIS in the H1 low-Q² fiducial: 0.05–0.15**,
with substantial z- and p_T-dependence (rising with p_T from the
F_UU,L suppression at low p_T).

This is small enough that the inflation of σ(R) by point-to-point
systematics (§4.3) becomes dominant: a 140% σ(R)/R measurement at
R=0.1 is not publishable.

### 5.2 φ_h-dependent terms and strategy

The φ_h-dependent terms are themselves linear in ε (cos2φ_h) or in
√(2ε(1+ε)) (cosφ_h). Two practical strategies:

1. **φ_h-integrate the cross section** to project out F_UU,T + ε F_UU,L.
   Requires uniform φ_h acceptance (or careful unfolding of the
   φ_h-acceptance variations) and explicit verification that the
   acceptance-corrected φ_h distribution is flat. This is the cleanest
   approach but discards information.
2. **Simultaneous Fourier fit** of the φ_h-binned cross section to
   extract F_UU,T, F_UU,L, F_UU^{cosφ_h}, F_UU^{cos2φ_h} at each
   (x,Q²,z,p_T^h) bin, with the ε-dependence of each Fourier
   coefficient providing the L/T separation. This requires ~4× more
   cells (φ_h binning), pushing MER/LER statistics below the floor in
   case (c) and marginal in case (b). **Not recommended** at H1
   statistics.

Recommended strategy: φ_h-integration, with a separate analysis of
the cosφ_h and cos2φ_h moments per beam energy at the HER 920 GeV
sample only (Cahn / Boer-Mulders), and treat the residual
φ_h-acceptance non-uniformity as a systematic on the L/T fit.

### 5.3 p_T^h dependence — the most informative bins

F_UU,L is power-suppressed at low p_T^h (it scales as
p_T^h²/Q² × leading-twist + α_s × log-enhanced terms in the small-
p_T^h TMD regime, and vanishes as p_T^h → 0 to leading-twist
accuracy in the collinear factorization at NLO). The lever arm in
R is therefore largest at **moderate-to-high p_T^h, p_T^h ≳ 0.5 GeV**.

The two upper p_T^h bins (1.0–1.5 and 1.5–2.0 GeV in the SIDIS
handoff binning) carry the bulk of the R signal but also the worst
statistics at MER and LER (cross section falls ~exponentially in
p_T^h). Recommended: integrate the lowest p_T^h bin (0–0.25 GeV) with
the next (0.25–0.5) for the L/T fit, keep the high-p_T^h bins as
finely as statistics permit.

---

## 6. Highest-impact deliverable

### 6.1 Comparison of three deliverables

| Deliverable | Stat precision (R = 0.1) | Systematics-driven precision | Novelty | TMD/higher-twist pull |
|:-----------|:------------------------:|:----------------------------:|:-------:|:---------------------:|
| (a) R_SIDIS(Q²) inclusive in (x,z,p_T^h) | σ(R)/R ≈ 26% | ≈ 140% (with 2% norm) → 50–70% (with 1% norm) | **First R_SIDIS measurement at HERA collider energies** | Moderate; tests overall normalization of F_UU,L vs pQCD |
| (b) R_SIDIS(z, Q²) integrated in (x, p_T^h) | 50% | 150% | First z-dependent R_SIDIS at collider energies | Constrains z-dependence of NLO coefficient + twist-3 |
| (c) R_SIDIS(z, p_T^h) differential at fixed (x, Q²) | > 100% per cell | > 200% per cell | Would be first TMD-relevant L/T separation at collider energies, but only viable if cells are aggressively coarsened | Strongest TMD pull (p_T^h dependence directly probes F_UU,L power-suppression) |

### 6.2 Which is first-of-its-kind?

**All three would be first-of-its-kind at HERA collider energies.**
HERMES (√s ≈ 7 GeV) has measured cos φ_h and cos 2φ_h moments
(arXiv:1212.5407 and the polarized-target SIDIS literature) but
no clean F_UU,L extraction. JLab measurements (CLAS, HMS/SHMS) sit
at much lower Q² and have done R_SIDIS in fixed-target kinematics but
not at low-x. **A HERA-collider R_SIDIS measurement would fill a
unique low-x, moderate-Q² regime** that no other facility can probe
with the same lever arm — the EIC will eventually probe this regime
but is years away.

### 6.3 Highest-impact recommendation

**Case (a) — inclusive R_SIDIS(Q²)** is the best risk-adjusted
deliverable:
- Statistically robust (case (a) has 26% stat precision at R=0.1).
- Achievable in 3–5 Q² bins covering 5 < Q² < 50 GeV² with √s overlap.
- Provides the first benchmark of F_UU,L at HERA-collider kinematics.
- Sets up case (b) for a publication-quality follow-up if the
  systematics work pans out.
- Has the largest pull on absolute SIDIS normalization
  (LHAPDF-style global fits of fragmentation functions, e.g. DSS, are
  presently sensitive to whether F_UU,L is or is not negligible at
  this kinematic regime).

Case (b) is a stretch goal at HERA-II statistics; case (c) is not
achievable without aggressive bin coarsening and is not recommended
as a primary deliverable.

---

## 7. Practical blockers

### 7.1 Availability of MER/LER datasets in the H1 SIDIS production framework

The current SIDIS production at NAF
(`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_*`) is built only
for HER 920 GeV (per the H1 AGENTS.md note that the workspace lumi
is the superseded pre-rescale luminosity nb⁻¹ for the nominal 2005–2007 HER dataset). The v5e
SIDIS builder schema has not, to my knowledge, been run over the
MER 575 GeV or LER 460 GeV ODS samples.

**Required production work (estimated):**

| Item | Estimate |
|:-----|:--------:|
| Run v5e builder over LER (12.2 pb⁻¹ data) | ~500 Condor job-hours; ~50 GB output |
| Run v5e builder over MER (5.9 pb⁻¹ data)  | ~300 Condor job-hours; ~25 GB output |
| Locate/validate ODS files for MER/LER on dCache | unknown; needs check of H1 catalogue |
| RAPGAP31 + DJANGOH14 MC at Ep = 575 GeV at v5e | **may not exist; major blocker if not — see §7.3** |
| RAPGAP31 + DJANGOH14 MC at Ep = 460 GeV at v5e | same |
| Per-energy lumi calibration / re-derivation | already provided in arXiv:1012.4355 (5.9 and 12.2 pb⁻¹) |
| Per-energy C_rad recomputation (HERACLES in DJANGOH) | ~weeks for proper RC tables |

### 7.2 Production cost (data side, conservative)

- ~800 Condor job-hours total for MER+LER v5e data builder over the
  17.1 pb⁻¹ combined sample.
- ~75 GB additional disk on DUST.
- 1–2 weeks of expert effort to validate (entry counts, kinematic
  closure, merge chain, normalization).

### 7.3 The hard blocker: MC samples

The H1 SIDIS-relevant MC chain on NAF
(RAPGAP31 + DJANGOH14, processed through the H1OO v5e SIDIS builder)
was produced **for the 920 GeV running configuration only** as of the
state captured in the local docs/data/*.json artifacts.

**For the L/T separation to be possible, RAPGAP31 and DJANGOH14
samples must exist (or be produced) at E_p = 460 and 575 GeV, in
adequate statistics (typically 5–10× data), and be passed through the
same H1OO simulation/reconstruction chain that produced the 920 GeV
SIDIS MC.**

If no such MC exists, the practical blockers are:
1. **MC generation campaign**: H1 SIDIS MC for the 460/575 GeV
   configurations was produced for the H1 inclusive FL analyses, but
   only for the inclusive-DIS observables (no SIDIS-builder pass).
   Re-running the H1OO chain with the v5e SIDIS builder over the
   existing 460/575 GeV inclusive MC samples is possible IF the
   underlying ODS files include hadronic-final-state and track
   information at v5e schema level. Needs explicit check by the H1
   software/production teams.
2. **If no usable MC exists**, the campaign requires regenerating the
   460/575 GeV samples through the full H1 simulation chain
   (RAPGAP31 → H1SIM → H1REC), which is a multi-month effort and
   would require H1 collaboration approval and computing allocation.

### 7.4 Single hard blockers that would kill the measurement

The measurement is **killed entirely** if any of the following is true:

- (B1) **The H1OO ODS files for MER/LER do not contain SIDIS-relevant
  v5e-compatible HFS information**, and re-reconstruction is not
  available. → Unrecoverable without full re-reconstruction.
- (B2) **No RAPGAP31/DJANGOH14 MC at E_p = 460/575 GeV exists with
  full GEANT detector simulation at sufficient statistics**, and the
  collaboration does not approve a new MC campaign. → Cannot
  unfold/acceptance-correct → no L/T separation.
- (B3) **Relative luminosity between HER and MER/LER cannot be
  controlled below 2%** at the SIDIS-builder level (lumi recalibration
  inconsistent across years). → σ(R) > 100% even in case (a) and the
  measurement loses publishable significance.

If none of B1–B3 hold, the measurement is feasible at the precision
quoted in §3.

---

## 8. Bottom-line recommendation

**Feasibility: marginal but worth pursuing for the inclusive deliverable
(case a) only.** Statistical precision on R_SIDIS at the inclusive Q²
level is ~26% at R = 0.1 (stat-only), competitive with the inclusive
F_L precision H1 has achieved with the same energy scan
(arXiv:1012.4355, arXiv:1312.4821). However, the measurement is
**dominated by point-to-point normalization systematics**: a 2%
HER-vs-LER relative-luminosity / efficiency offset inflates σ(R)/R to
~140%, rendering the measurement unpublishable. The differential TMD
case (R(z, p_T^h)) is not viable at HERA-II statistics. The
highest-impact achievable deliverable is **R_SIDIS(Q²) inclusive in
(x, z, p_T^h), in 3–5 Q² bins across 5 < Q² < 50 GeV², as the first
SIDIS L/T separation at HERA-collider energies**. Timeline contingent
on availability of MER/LER MC at v5e SIDIS-builder schema (the
dominant practical risk): 6 months if MC exists, 1.5–2 years if a new
MC campaign is required. Recommend a one-week scoping task at
NAF to (i) verify presence of v5e-compatible MER/LER ODS and MC, and
(ii) reproduce H1 inclusive F_L cross-section measurements at the
SIDIS-fiducial level to bound the achievable HER↔MER↔LER relative
normalization control. If both checks come back positive,
authorise a 6-month feasibility build of case (a) only.

---

## Open questions / gaps in this study

1. **Per-bin yields are first-order estimates.** A proper feasibility
   note would re-derive them from the H1 2010 FL reduced-cross-section
   tables (Table 10 ff. of arXiv:1012.4355) folded with the
   SIDIS-fiducial geometric acceptance and a measured hadron
   multiplicity from arXiv:1302.1321 Table 8/9, rather than the
   rough 20,000 DIS/pb⁻¹ × 2.5 hadrons/event scaling used here.
2. **The φ_h-acceptance uniformity** of the H1 detector in the
   |η|<1.3 fiducial has not been verified for the SIDIS-builder
   output — assumed flat-after-correction here. This matters for
   the φ_h-integration strategy in §5.2.
3. **R_SIDIS theory prediction** at H1 low-Q² collider kinematics
   has not been computed in this note; cited values are
   extrapolations from HERMES/JLab fixed-target data and the
   leading-order α_s estimate. A dedicated TMD calculation (e.g.,
   using the JAM3D or Pavia15 framework) would be a useful
   companion to this feasibility study.
4. **Existence of MER/LER MC at v5e** is not verified by this study
   and must be checked on NAF before any production-cost estimate
   in §7.2 is reliable.
5. **Relative luminosity uncertainty between HER and MER/LER** in
   the H1 production system is quoted from inclusive-DIS papers
   (3% / 4%), but the SIDIS-builder-level lumi-weighting consistency
   has not been audited.

## Risks summary

- (R1) **MC blocker.** v5e-compatible MER/LER SIDIS MC may not
  exist. → Measurement dead unless MC campaign approved.
- (R2) **Point-to-point normalization control.** Must reach better
  than 1% to make a publishable case (a) result. Currently quoted
  ~3–4%.
- (R3) **Radiative corrections at high y (LER edge of overlap).**
  Up to 10% on σ_inclusive; need dedicated DJANGOH C_rad tables
  per beam energy.
- (R4) **MER statistics.** 5.9 pb⁻¹ is the smallest of the three;
  drives the conditioning of the 3-point fit.

## Primary sources

- arXiv:1012.4355 — H1 Collaboration, *Measurement of the Inclusive
  e±p Scattering Cross Section at High Inelasticity y and of the
  Structure Function F_L*, Eur.Phys.J.C71:1579,2011. **Provides
  L_HER=103.5, L_MER=5.9, L_LER=12.2 pb⁻¹ for the low-Q² SpaCal
  selection; norm uncertainties 3%/4%/4%.**
- arXiv:1312.4821 — H1 Collaboration, *Measurement of Inclusive ep
  Cross Sections at High Q² at √s = 225 and 252 GeV and of the
  Longitudinal Proton Structure Function F_L at HERA*,
  Eur.Phys.J.C74. **Provides high-Q² LAr-trigger lumis 11.8 (LER)
  and 5.4 (MER) pb⁻¹; companion to 1012.4355.**
- arXiv:1302.1321 — H1 Collaboration, *Measurement of Charged
  Particle Spectra in Deep-Inelastic ep Scattering at HERA*.
  **The direct predecessor low-Q² SIDIS-fiducial paper: 88.6 pb⁻¹
  HER 920 GeV only, 5 < Q² < 100 GeV², 10⁻⁴ < x < 10⁻². Same
  fiducial we want to extend.**
- arXiv:hep-ph/0611265 — A. Bacchetta et al., *Semi-inclusive deep
  inelastic scattering at small transverse momentum*, JHEP 0702:093.
  **Defines F_UU,T, F_UU,L and the SIDIS cross-section decomposition
  (eq. 2.7) used throughout this study.**
- arXiv:1212.5407 — HERMES Collaboration, *Multiplicities of charged
  pions and kaons from semi-inclusive deep-inelastic scattering by
  the proton and the deuteron*, Phys.Rev.D87:074029. **HERMES SIDIS
  multiplicity benchmark; no L/T separation but constrains R_SIDIS
  indirectly to be small.**

## Files referenced in workspace

- `H1/AGENTS.md` — workspace conventions, the superseded pre-rescale luminosity nb⁻¹ HER lumi.
- `H1/sidis_highQ2/docs/data/acceptance_highQ2_charged_mc_prototype.json`,
  `H1/sidis_highQ2/docs/data/validation_highQ2_mc_species_matching.json`
  — confirm v5b SIDIS builder schema and MC pool for the **high-Q²**
  analysis (Q² in [150, 20000]).
- `H1/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md`
  — workspace's predecessor-paper digest. Critical note for this
  study: it documents that the per-event-multiplicity observable
  (track count / DIS-event count) used in 1302.1321 and adopted by
  the current H1 SIDIS workspace **explicitly cancels luminosity in
  the ratio**, so the published H1 multiplicity result quotes no
  luminosity systematic. **This cancellation does NOT survive in a
  Rosenbluth-style L/T separation**, because the reduced
  SIDIS cross section (σ̃_SIDIS = F_T + ε F_L) needs the
  per-energy absolute normalization to compare across √s. The L/T
  measurement reintroduces the full lumi systematic that the
  predecessor avoided. **This is an important consequence: the
  measurement proposed here is harder than the existing H1
  multiplicity analysis precisely because the lumi cancellation is
  broken.**
- `H1/sidis_highQ2/docs/research/research-beam-polarization-charge.md`
  — explicitly documents the 2007 low-energy run periods
  `eEplus460LH1` (500918–507824, ~12k subruns) and `eEplus575LH1`
  (507843–511079, ~6k subruns) and **flags them as "NOT in our
  SIDIS sample"**. This is the workspace-internal confirmation of
  blocker B1/B2 from §7.4: the current H1 production pipeline at
  NAF has not processed these run periods through the SIDIS builder.
- `H1/sidis_highQ2/docs/research/research-crad-for-charged-sidis.md`
  — the existing C_rad pipeline runs on RAPGAP31/DJANGOH14
  Rad/NonRad samples at 920 GeV only (paths
  `/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/...`).
  Equivalent samples at 460 and 575 GeV would have to be located or
  produced. This is the concrete instantiation of blocker B2 in §7.4.
- `H1/agents/h1-normalization.md` — per-year HER luminosities for
  the current 920 GeV SIDIS analysis. Does not currently include
  MER/LER calibrations.
- `H1/sidis_lowQ2/context-handoff.md` — does **not exist** at the
  time of this study. The luminosity numbers cited in this study for
  MER and LER come from arXiv:1012.4355 Table 1 directly, not from
  any workspace handoff.

# Why a high-Q^2 charged-SIDIS `C_rad` can be large, and what eSigma can (and cannot) cure

**Scope.** Research-only assessment for the H1 high-Q^2 inclusive charged-hadron SIDIS analysis (`Q2 > 150 GeV2`).  This is not a recommendation to alter the current release.  The governing state was read: the central v6/z005 release is raw (no `C_rad` applied), and its DJANGOH `C_rad` candidate remains provisional because of exposure/provenance, sparse-bin, and method gates.

**Status labels:** **Verified** = algebra or directly cited source; **Inferred** = consequence of the verified definition; **Unresolved** = requires a controlled study or truth-record audit; **Speculative** = a plausible mechanism not established for these samples.

## Executive answer

**Verified.** Collinear ISR changes the incoming hard lepton from `k` to `rk`, with `r=1-x_gamma`.  If the nominal `k` is nevertheless used in `q=k-k'`, then

\[
 Q^2_{e,\rm nom}=Q^2_{\rm hard}/r,\qquad
 y_{e,\rm nom}=x_\gamma+r y_{\rm hard},
\]

and hence tracks migrate to too-high `Q2`, generally too-high `y`, too-low `z`, and a wrong `gamma* p` boost/axis.  This is a real, potentially strong bin-migration mechanism near steep `Q2`, `x`, `z`, and low-`pT_h` bin edges.  It is not merely an overall QED rate correction.

**Verified correction to a common premise.** eSigma does *not* make the electron-Q2 component ISR-safe: in the standard eSigma definition, `Q2_eSigma = Q2_e`, evaluated with the nominal beam energy.  It makes the **inelasticity and x reconstruction** substantially ISR-robust through the hadronic `Sigma` balance.  A virtual photon reconstructed from `(Q2_e, y_Sigma, phi_e)` uses **more than just the electron azimuth**: `Q2` and `y` fix its energy, longitudinal component, and transverse magnitude; `phi_e` fixes the remaining transverse orientation.

**Inference.** Replacing a nominal-electron `q` in `z` and `pT_h` by a self-consistent eSigma photon should remove the *large ISR-induced y/denominator and photon-axis error* but cannot remove ISR sensitivity contained in `Q2_e`.  A substantial decrease in the high positive tail of `C_rad` is expected only if the present SIDIS variables really use nominal-electron `q`; it is not a prediction for the already eSigma-tagged v6 convention without a matched RAD/NONRAD calculation.

---

## 1. Conventions and ISR algebra

Take the proton along `+z`, incoming electron along `-z`, neglect lepton/proton masses where stated:

\[
 k=(E_e,0,0,-E_e),\quad P\simeq(E_p,0,0,E_p),\quad
 k_{\rm hard}=rk,\quad r=1-x_\gamma .
\]

A strictly collinear ISR photon is `k_gamma=x_gamma k`.  The physical hard photon is

\[
q_{\rm hard}=rk-k',\qquad q_{\rm nom}=k-k'=q_{\rm hard}+x_\gamma k.\tag{1}
\]

Equation (1) is exact in the collinear, massless approximation and is the clearest statement of the error: it is a light-cone error, not a generic detector smearing.

### Electron method

With HERA polar angle measured from `+z`,

\[
Q_e^2=4E_e E'_e\cos^2(\theta_e/2),\qquad
 y_e=1-\frac{E'_e}{2E_e}(1-\cos\theta_e).
\]

For fixed measured `k'` and a hard interaction at energy `rE_e`:

\[
Q^2_{e,\rm nom}=Q^2_{\rm hard}/r,
\qquad y_{e,\rm nom}=1-r(1-y_{\rm hard})=x_\gamma+r y_{\rm hard}.\tag{2}
\]

Thus, to leading order in `x_gamma`,

\[
\frac{\delta Q^2}{Q^2}=x_\gamma+O(x_\gamma^2),\qquad
\delta y_e=x_\gamma(1-y_{\rm hard}),\qquad
\frac{\delta y_e}{y_{\rm hard}}=x_\gamma(1-y_{\rm hard})/y_{\rm hard}.
\]

**Verified.** The relative `y` error is especially large at low true `y`; `Q2` is biased upward at all `y`.  This is the standard reason the electron method is radiatively fragile.  Bassler--Bernardi explicitly introduced the Sigma method for small radiative corrections and discuss the QED-radiation effect on reconstruction [R1].

### Consequence for SIDIS `z`

For `z=(P.p_h)/(P.q)` and collinear beams, `P.q=2E_e y` in the nominal reconstruction and `P.q_hard=2rE_e y_hard`.  Therefore

\[
\frac{z_{\rm nom}}{z_{\rm hard}}
 =\frac{r y_{\rm hard}}{x_\gamma+r y_{\rm hard}}
 =1-\frac{x_\gamma}{y_{\rm hard}}+O(x_\gamma^2).\tag{3}
\]

**Verified.** ISR moves `z` downward.  The leading expansion is only valid for `x_gamma << y`; the exact form in (3), not the expansion, must be used for hard radiation.  Example: `y_hard=0.20`, `x_gamma=0.10` gives `z_nom/z_hard=0.18/0.28=0.643`, a 36% downward migration.

### Consequence for `pT_h`

**Verified.** The ISR photon has zero transverse momentum, so `q_T=-k'_T` is unchanged; however Eq. (1) changes `q^0` and `q^z` by `(x_gamma E_e,-x_gamma E_e)`.  Hence it changes both the lab photon direction and the boost `B(q+P)` used to reach the gamma-star--proton CM.  There is no universal scalar multiplicative bias in `pT_h` because the effect depends on the hadron momentum and its angle to the displaced axis.

At fixed `q_T`, a useful leading directional estimate is

\[
\delta\hat q\sim {x_\gamma E_e\over |\vec q|}\,
 [\hat z-(\hat z\!\cdot\!\hat q)\hat q],
\qquad
\delta p_{T,h}\sim-\delta\hat q\!\cdot\!\vec p_h^{\,*},\tag{4}
\]

with the additional CM-boost contribution of the same order.  This is **inferred** from (1), not a calibrated resolution formula.  It predicts the greatest fractional effect in the lowest `pT_h` bins and at `z`/hemisphere boundaries, where a modest axis rotation can cross an edge.

The H1 workspace defines `z` as `P.p_h/P.q` and `pT_h` transverse to `q` in the gamma-star--proton CM [L1, lines 41--64].  A recent diagnostic confirms that its eSigma photon is used for the CM boost and axis [L2, lines 15--19].

---

## 2. Sigma and eSigma: exact definitions and the reconstructed photon

Define

\[
\Sigma_h=\sum_{\rm HFS}(E_i-p_{z,i}),\qquad
\Sigma_e=E'_e(1-\cos\theta_e).
\]

The standard Sigma inelasticity is

\[
y_\Sigma={\Sigma_h\over \Sigma_h+\Sigma_e}.\tag{5}
\]

The usual Sigma method also uses

\[
Q^2_\Sigma={(E'_e\sin\theta_e)^2\over 1-y_\Sigma},\qquad
x_\Sigma={Q^2_\Sigma\over s y_\Sigma}.\tag{6}
\]

These equations are documented in the analysis coordinate convention [L1, lines 22--36] and originate with Bassler--Bernardi [R1].

The standard **eSigma** hybrid instead is

\[
 y_{e\Sigma}=y_\Sigma,\qquad Q^2_{e\Sigma}=Q^2_e
 =4E_eE'_e\cos^2(\theta_e/2),\qquad
 x_{e\Sigma}={Q^2_e\over s y_\Sigma}.\tag{7}
\]

**Verified, with nomenclature caveat.** Published/H1 software labels can be confusing: the local H1CalcKine evidence says `GetQ2es()=GetQ2e()` by construction, while `GetYes()` is the eSigma y [L3, lines 171--177; L4, lines 52--75].  Thus the prompt's expression for `Q2_eSigma` is correct under the HERA angle convention.  The electron **energy and polar angle** enter `Q2_e`; `E'_e` and `theta_e` also enter `Sigma_e`; the hadronic final state enters `Sigma_h`; and `phi_e` is not required for scalar `(Q2,y,x)`.

### Explicit `q(Q2,y,phi)` reconstruction

Given nominal beam four-vectors and a massless photon with `(Q2,y,phi_q)`, `P.q=2E_eE_p y` and `q^2=-Q2` yield

\[
q=(q^0,q_T\cos\phi_q,q_T\sin\phi_q,q^z),\tag{8}
\]
\[
q_T=\sqrt{Q^2(1-y)},\quad
q^0=E_e y-{Q^2\over4E_e},\quad
q^z=-E_e y-{Q^2\over4E_e}.\tag{9}
\]

For a measured lepton plane, transverse momentum conservation gives `phi_q=phi_{e'}+pi` (modulo `2pi`).  Equations (8)--(9) are **verified algebraically** from `P.q` and `q^2`; they show precisely why “only `phi` enters” is false.  `phi_e` fixes only the azimuthal orientation.  `Q2` and `y` fix the magnitude and longitudinal/energy components, and therefore the photon axis and gamma-star--proton boost.

A commonly used alternative implementation keeps measured `theta_e,phi_e` and obtains an effective energy from

\[
E'_{e\Sigma}={Q^2_e\over4E_e\cos^2(\theta_e/2)}
={Q^2_e\over2E_e(1+\cos\theta_e)}.\tag{10}
\]

It then builds `k'=(E'_{eSigma},E'_{eSigma}\sin\theta_e\cos\phi_e,E'_{eSigma}\sin\theta_e\sin\phi_e,E'_{eSigma}\cos\theta_e)` and `q=k-k'`.  The local diagnostic documentation uses the second expression in (10) [L2, line 17].

**Verified constraint.** `(Q2,y,phi)` fixes a massless `q` through (8)--(9), whereas `(Q2,theta_e,phi_e)` fixes it through (10).  In a hybrid eSigma estimator, `y_Sigma` and measured `theta_e` need not obey the massless electron-method relation exactly after reconstruction/calibration; one cannot impose all of `(Q2_e,y_Sigma,theta_e,phi_e)` as independent exact constraints.  The analysis must therefore state which construction defines its SIDIS axis.  The definitive v6 gen-only `C_rad` convention is **unresolved** here; this potential difference must be controlled before interpreting any eSigma-variant shift.

---

## 3. Why Sigma quantities self-correct collinear ISR

For an ideal contained DIS event with a lost ISR photon down the electron (`-z`) pipe,

\[
\Sigma_h+\Sigma_e=\sum_{\rm measured}(E-p_z)=2(E_e-E_\gamma)=2rE_e.\tag{11}
\]

The unobserved photon itself has `E-p_z=2E_gamma`; including it restores `2E_e`.  Thus the measured Sigma balance directly measures the reduced incoming energy:

\[
E_e^{\rm eff}={\Sigma_h+\Sigma_e\over2}=rE_e,
\qquad s_{\rm eff}=r s.\tag{12}
\]

**Verified.** Equation (11) proves the ISR robustness of (5): both numerator and denominator track the reduced hard energy, so `y_Sigma` remains approximately `y_hard`.  The statement assumes containment of the noncollinear HFS and ignores detector losses; it is not immunity to calibration or acceptance effects.

**Important limitation (verified).** Since eSigma retains `Q2_e`, (2) still applies to its Q2 if it uses nominal `E_e`.  It is therefore accurate to say eSigma is ISR-robust chiefly in `y` and `x` (through `y` and `s_eff` conventions), not that it entirely reconstructs the hard `q` without ISR bias.  The full Sigma `Q2_Sigma` has a different trade-off.

Primary references are U. Bassler and G. Bernardi, *On the kinematic reconstruction of deep inelastic scattering at HERA: the Sigma method*, NIM A **361** (1995) 197--208, DOI `10.1016/0168-9002(95)00173-5`, arXiv:hep-ex/9412004 [R1]; and the cited follow-up eSigma discussion, NIM A **426** (1999) 583 [R2].  **Unresolved bibliographic detail:** this review did not obtain the follow-up paper text/DOI from an authoritative accessible copy, so readers should verify its exact title/DOI before bibliography insertion.

---

## 4. Size expectations, the `E-pz` window, and interpretation of 1.15--1.5

### What is firmly established for this analysis

**Verified local result.** The earlier DJANGOH14 gen-only v1 construction (`C_rad=sigma_nonrad/sigma_rad`) gives integrated `1.05533` (h+) and `1.05541` (h-), all-bin median `1.07884`, with the nominal gen selection including `35 < GenEmpz < 70` [L5, lines 5--29].  This is the best currently documented, internally normalized magnitude reference, but remains non-publication-grade.

**Verified local result.** The later 2026-09-21 candidate gives integrated `1.11557` per charge, but only `479/490` finite values, `114` values outside `[0.5,1.2]`, and tail statistical uncertainties up to 109.5%; it is explicitly unapplied [L6, lines 24--41].  It cannot support a physics conclusion about a mean of 1.15 or tail 1.5.

**Verified local warning.** A former RAPGAP aggregate-luminosity result around 1.405 is quarantined; the alternate fLumi diagnostic gives integrated 1.0248 but is also provisional only [L5, lines 31--51].  Neither is an allowed benchmark.

### Literature-backed qualitative expectation

**Verified.** Bassler--Bernardi's abstract states that the Sigma method provides small radiative corrections and specifically treats QED-radiation effects [R1].  **Unresolved.** In the available primary literature reviewed here, no published H1 charged-hadron high-Q2 four-dimensional table was located that licenses a universal numerical claim such as “electron method = A%, eSigma = B%” after the analysis's exact SIDIS definitions and cuts.  Such a number is intrinsically observable-, binning-, and cut-dependent.  It would be misleading to invent a universal 1--2% number from inclusive DIS and apply it to per-hadron `(Q2,x,z,pT_h)` cross sections.

The defensible numerical summary is therefore:

| reconstruction/selection | evidence-supported expectation |
|---|---|
| nominal electron `q`, no hard-ISR rejection | **Inferred:** 10--50% local effects are physically plausible in steep or edge bins because (2)--(4) induce event and hadron-variable migration; not necessarily a 10--50% inclusive normalization effect. |
| Sigma/eSigma `y/x` plus self-consistent photon | **Verified qualitative:** reduced ISR migration in `y`, `x`, `z` denominator and axis relative to a nominal-electron construction; **unresolved numerical** residual because eSigma retains `Q2_e`. |
| `35 < E-pz < 70` GeV with `E_e=27.6` GeV | **Verified:** ideal collinear ISR surviving the lower cut obeys `r>35/(2E_e)=0.63406`, hence `x_gamma<0.36594`; the upper 70 GeV bound is not an ISR bound. |

Thus the cut rejects photons with more than 36.6% of the beam energy in the ideal fully measured case, but admits all softer ISR.  It cannot by itself prove that residual 15% corrections are impossible.  **Inference:** a stable 15% average after this cut is plausible through remaining soft/moderate ISR plus migration in steep distributions; a pervasive 50% correction in well-populated central bins would be a warning sign for a definition, RAD/NONRAD pairing/normalization, FSR-lepton, or sparse-bin problem, not evidence to accept without closure.

---

## 5. Generator-level FSR: what is “the scattered electron”?

**Verified physics statement.** With final-state radiation, there are two physically distinct lepton four-vectors: the lepton at the hard DIS vertex before FSR (`k'_Born/hard`) and the post-FSR lepton (`k'_post`, after `k'_hard -> k'_post + gamma_FSR`).  They define different photons:

\[
q_{hard}=k_{hard}-k'_{hard},\qquad q_{post}=k_{hard}-k'_{post}=q_{hard}+k_{\gamma,FSR}.\tag{13}
\]

Using `q_post` changes `Q2`, `y`, `z`, and the photon axis exactly as a changed scattered-electron energy/direction would.  Collinear FSR often makes the post-FSR electron lower energy and shifts electron-defined `Q2` downward; noncollinear FSR can also rotate the axis.

**Unresolved for current DJANGOH ntuples.** The truth-record status code, ancestry, and H1 interface mapping must establish whether `GenElec*` is pre-FSR, post-FSR, a dressed lepton, or a selection-dependent proxy.  The NONRAD sample is conceptually unambiguous in the absence of QED emission, but its stored record still requires a schema check.  RAD is not unambiguous merely because a branch is named “electron.”

**Inference.** If RAD uses post-FSR `k'` but NONRAD uses a hard/pre-FSR electron (or the converse), their bin migrations differ by construction and `sigma_nonrad/sigma_rad` can be artificially enhanced.  This can mimic a radiative correction, especially in edge bins.  It is not a cancellation that weights or luminosities can repair.

**Required follow-up (not performed here):** route truth-ancestry and HERACLES/DJANGOH semantics to `h1-mc-expert`; route RAD/NONRAD request, period, and truth-product provenance to `h1-mc-db`; route a bounded branch/record audit to `h1-root-analyst` or `h1-data-validator`.

---

## 6. Prediction and discriminating tests

### Conditional prediction

If the present RAD and NONRAD `z,pT_h` use `q_nom=k_nom-k'_measured` while event cuts/binning use eSigma, then a variant that builds both hadron variables from the self-consistent eSigma photon of (8)--(10) should:

1. **Inference:** reduce the positive `C_rad` tail and likely lower an integrated factor above the v1 `1.055--1.079` range, because ISR events no longer receive the large downward `z` migration of (3) and the spurious axis/boost displacement of (4).
2. **Inference:** change most strongly at low `y`, high `z`, lowest `pT_h`, and `(Q2,x)` cell boundaries, where the relative errors in (2)--(4) and spectrum slopes are largest.
3. **Unresolved:** leave a nonzero residual because `Q2_eSigma=Q2_e` remains ISR-sensitive and because genuine QED radiation changes the physical cross section, not only the reconstruction.

No reliable percentage reduction can be assigned from algebra alone.  The fraction of a putative mean `~1.15` or tail `~1.5` that is a definition artifact is **unresolved**.  It requires the same weighted RAD/NONRAD samples, selections, lepton convention, bins, and luminosity proxy, differing in one and only one `q` definition.

### Ranked research options

| priority | action and owner | discriminant |
|---|---|---|
| 1 | `h1-mc-expert` + `h1-mc-db`: certify pre/post/dressed FSR electron and paired RAD/NONRAD truth provenance | removes the largest semantic failure mode in (13) |
| 2 | `h1-physics-analyst` (routine variant generation) with `h1-normalization` review: calculate matched `C_rad` for electron-q and eSigma-q, retaining per-bin sumw/sumw2 | directly measures the definitional component |
| 3 | `h1-root-analyst`: read-only branch audit of `GenElec*`, photon records, and eSigma q construction | makes the truth convention auditable |
| 4 | `h1-systematics`: if retained, define a reconstruction-convention uncertainty only after the controlled comparison | prevents absorbing a definition mismatch as physics |

No execution, data processing, or central-release modification was performed for this artifact.

## Sources

### Primary literature

- **[R1]** U. Bassler and G. Bernardi, “On the kinematic reconstruction of deep inelastic scattering at HERA: The Sigma method,” *Nucl. Instrum. Meth. A* **361** (1995) 197--208, DOI: https://doi.org/10.1016/0168-9002(95)00173-5; arXiv: https://arxiv.org/abs/hep-ex/9412004; INSPIRE record https://inspirehep.net/literature/381117.  The abstract explicitly states the aim of small radiative corrections and discussion of QED-radiation effects.
- **[R2]** U. Bassler and G. Bernardi, follow-up eSigma reference cited in the request: *Nucl. Instrum. Meth. A* **426** (1999) 583.  **Unresolved:** verify bibliographic metadata from the publisher/library before formal use; it was not independently retrieved in this review.

### Analysis-local authority and implementation evidence

- **[L1]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data_release/schema/coordinate_conventions.md`, lines 22--64: Sigma equations and charged-SIDIS `z,pT_h` definitions.
- **[L2]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/diagnostics/highx_highz_lowpt_onefile_2026-09-22.md`, lines 15--19: eSigma photon/CM construction (explicitly a bounded non-`C_rad` diagnostic).
- **[L3]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/research-dis-kinematic-methods.md`, lines 171--177: H1CalcKine relationship `GetQ2es=GetQ2e` and `GetYes`.
- **[L4]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/research-v5g-scat-elec-track-assoc.md`, lines 52--75: reco/gen getter evidence.
- **[L5]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/status/radiative-corrections.md`, lines 5--51: DJANGOH v1 magnitude and RAPGAP quarantine.
- **[L6]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/SESSION_HANDOFF_crad_resolution_2026-09-21.md`, lines 24--41: candidate magnitude, sparsity, and release gates.
- **[L7]** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/context-handoff.md`, lines 225--231: current `C_rad` status and unapplied-release rule.

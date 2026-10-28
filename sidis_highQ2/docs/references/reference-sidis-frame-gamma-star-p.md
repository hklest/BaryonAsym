<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Reference: Frozen SIDIS frame -- gamma*+p (Hadronic CMS) for high-Q2 SIDIS

**Status:** FROZEN. The low-Q2 code is verified to compute pT_h, z, and
phi_h in the **gamma*+p frame** (= Hadronic CMS = HCM = virtual photon -
proton rest frame). The high-Q2 charged-particle multiplicity, asymmetry,
phi_h, and C_rad analyses inherit this convention verbatim.

**Scope:** This document is the single source of truth for the SIDIS frame
convention used by every high-Q2 charged-particle observable
(M^{h+/-}(Q2,x,z,pT_h), A_h, A_BC, < cos(phi_h) > / < cos(2 phi_h) >, and
the C_rad correction factors). It is to be cited verbatim by every
multiplicity / acceptance / phi_h / C_rad / A_BC gate in
`plan-highQ2-sidis-next-steps.md`.

---

## 1. Executive summary

1. The low-Q2 pipeline -- `tmd_analysis.py`, `tmd_resolution_acceptance.py`,
   `extract_multiplicities.py`, `extract_azimuthal_moments.py`,
   `extract_phi_moments_data.py`, `azimuthal_acceptance.py` -- computes
   `pT_h`, `z`, and `phi_h` from lab-frame 4-vectors using formulas that
   are **manifestly Lorentz-invariant under any boost along the virtual
   photon direction q̂**. The numerical answer is therefore identical
   in the H1 lab, in the Hadronic CMS (gamma*+p rest frame = HCM), and
   in the Breit frame. The published interpretation is the **gamma*+p
   frame**, which is the standard TMD frame.
2. This frame is what HERMES, COMPASS, JLab and the 2013 H1 charged-
   multiplicity paper (arXiv:1302.1321) and DESY 20-176 all use, under
   the synonyms HCM / hadronic CMS / virtual gamma-p rest frame.
3. Task W (`task_W_multiplicity_highQ2.py`) and Task R
   (`compute_highQ2_crad_charged.py` + `.C`) both use the same
   `pT_h = |p_had x q̂|` and `z = (P.p_had)/(P.q)` formulas. **Frame is
   identical.** No frame fix is required for Task W or Task R.
4. The dM/dpT_h CONCERN flagged on the Task W prototype is **NOT** a
   frame bug. It is consistent with computing pT_h correctly in the
   gamma*+p frame. The likely cause is the Task W binning mismatch:
   Task W's `PTH_EDGES = [0, 0.25, 0.5, 1.0, 1.5, 2.0]` and
   `X_EDGES = [0.01, 0.03, 0.1, 0.3, 1.0]` do **not** match frozen
   Scheme HQ-A (`pT_h: [0, 0.5, 1.0, 2.0, 4.0, 8.0]`,
   `x: [0.003, 0.01, 0.03, 0.1, 0.5]`). Task W must be re-run with
   frozen HQ-A before G9 can close.
5. The boost magnitude into the gamma*+p frame is **per-event** (driven
   by the per-event electron 4-momentum). The proton-side 4-momentum is
   **per-run** (E_p = 920 GeV, identical across HERA-II periods; per-run
   E_e from `BeamElecE`).

---

## 2. The frozen SIDIS frame: gamma*+p (Hadronic CMS)

> **FROZEN STATEMENT for high-Q2 and low-Q2 SIDIS observables**
>
> All SIDIS observables `(pT_h, z, phi_h)` are defined in the
> **gamma*+p rest frame** (a.k.a. Hadronic CMS, a.k.a. virtual
> photon - proton rest frame, a.k.a. HCM, a.k.a. gamma*-N CMS).
> The z-axis of this frame is the virtual photon 3-momentum direction;
> the x-axis lies in the lepton plane.
>
> Because the implemented formulas
> `pT_h = |p_had × q̂|`, `z = (P · p_had)/(P · q)`, and the Trento
> `phi_h` expression based on 4-vector cross products are all
> Lorentz-invariant under boosts along q̂, they yield the **identical
> numerical answer in the lab, the Hadronic CMS, and the Breit frame**.
> The H1 BaryonAsym code therefore evaluates them directly from
> lab-frame 4-vectors; no explicit boost is applied. The physical
> interpretation is the gamma*+p frame.
>
> Reference: Bacchetta, Diehl, Goeke, Metz, Mulders, Schlegel,
> arXiv:hep-ph/0410050 ("Trento conventions"), Section 2 and Eq. (2.4).
> H1 precedent: arXiv:1302.1321 (HERA-II low-Q2 charged-particle
> multiplicities) and DESY 20-176 both define their multiplicity
> observables in the HCM, called "the virtual gamma*-p rest frame".

---

## 3. Definitions and formulas

All quantities below are evaluated from lab-frame 4-vectors in the
standard H1 lab convention: proton along +z, incoming lepton along -z.
**No explicit boost** is performed; the formulas are invariant under any
boost along q̂ so they give the gamma*+p frame value directly.

### 3.1 Beam 4-vectors

Source: `tmd_analysis.py:203-213`, `compute_highQ2_crad_charged.C:390-392`.

```
k_beam = (0, 0, -E_e, E_e)             # incoming lepton, -z direction
P_beam = (0, 0, +|P_p|, sqrt(P_p^2 + M_p^2))   # incoming proton, +z direction
```

E_e is per-event from `BeamElecE` (~27.5 GeV for ep0304, ~27.6 GeV for
em0405/em06/ep0607). P_p = 920 GeV across all HERA-II periods. M_p =
0.93827 GeV.

### 3.2 Virtual photon 4-vector

Source: `tmd_analysis.py:259`, `compute_highQ2_crad_charged.C:395`.

```
q = k_beam - k_scat                    # incoming minus outgoing electron
q3 = q.spatial                         # 3-vector part
q_hat = q3 / |q3|                      # unit virtual-photon direction
```

VERIFIED: Identical at every site:
- `tmd_analysis.py:259`
- `tmd_resolution_acceptance.py:280` (via the same `compute_pth_phih_z`)
- `extract_multiplicities.py:179`
- `extract_azimuthal_moments.py:251`
- `extract_phi_moments_data.py:262`
- `compute_highQ2_crad_charged.py:261`
- `compute_highQ2_crad_charged.C:257`
- `task_W_multiplicity_highQ2.py:83-88` (inline `compute_pTh_phot`)

### 3.3 pT_h definition (frozen)

Source: `tmd_analysis.py:269-271`.

```
pT_h = | p_had × q̂ |
```

This is the magnitude of the hadron 3-momentum component perpendicular
to q̂. It is identical to `sqrt(p_had^2 - (p_had · q̂)^2)`, which is
what `task_W_multiplicity_highQ2.py:89-94` computes (mathematically
equivalent expression). Because `q̂` is a unit 3-vector evaluated in
the lab, but the perpendicular component of a 3-vector with respect to
the q axis is unchanged under any pure boost along q, **the numerical
value of pT_h is identical in the lab, HCM, gamma*+p frame, and Breit
frame**. The physical interpretation is "hadron transverse momentum in
the gamma*+p frame, measured relative to the virtual photon axis".

VERIFIED at every site (lines as above for q definition; immediately
after q_hat construction in each).

### 3.4 z definition (frozen)

Source: `tmd_analysis.py:288-291`.

```
z = (P · p_had) / (P · q)
```

This is the Lorentz-invariant Bjorken-y-like z, also known as `z_pq` in
COMPASS and `z` in Bacchetta et al. In the gamma*+p rest frame this
reduces to `z = E_h / nu` where E_h is the hadron energy in that frame
and nu = E_gamma* is the virtual photon energy. **Computing
`(P.p_h)/(P.q)` in the lab gives the gamma*+p-frame `E_h / nu` value
exactly**, because P and q are 4-vectors and the dot product is
Lorentz-invariant.

Task W deviates here: it uses `zGen` read directly from the
`ChargedTot.zGen` branch (`task_W_multiplicity_highQ2.py:290`). The
comment at line 280 states "zGen IS per-track on ChargedTot (validated
against photon-frame)" -- which asserts the builder's `zGen` is the
same `(P · p_had) / (P · q)` quantity. **VERIFIED PROVISIONAL**: needs
a one-time numerical closure on v5b high-Q2 MC comparing
`compute_pth_phih_z` z against the stored `zGen`. If they differ by
more than ~1e-5 in median, the builder's zGen is using a different
definition (e.g., the eSigma-method photon q, or a different
denominator) and Task W is silently using a slightly different z than
the rest of the pipeline.

### 3.5 phi_h definition (frozen, Trento)

Source: `tmd_analysis.py:273-286`.

```
l = k_beam × k_scat                    # lepton-plane normal
h = q × p_had                          # hadron-plane normal
cos(phi_h) = (l · h) / (|l| |h|)
sin(phi_h) = ((l × h) · q̂) / (|l| |h|)
phi_h = atan2(sin, cos)  ∈  (-π, π]
```

This is the Trento phi_h per Bacchetta et al, arXiv:hep-ph/0410050
Eq. (2.4). The expression is invariant under boosts along q̂
(the cross products and triple product all transform consistently),
so it gives the gamma*+p frame phi_h directly. The sign of phi_h is
fixed by the H1 lab z-orientation, which is identical for e+p and
e-p periods (incoming lepton always along -z).

See `H1/sidis_highQ2/docs/reviews/critique-phih-convention-freeze-highQ2-sidis.md`
for the full sign and orientation audit and for the phi_h-specific
output-metadata block.

---

## 4. Cross-script consistency status (low-Q2)

**VERIFIED: all five Python scripts and the one C++ macro use bit-for-bit
identical formulas for (pT_h, z, phi_h).**

| Script | `compute_pth_phih_z` location | Formula match | Boost? |
|--------|-------------------------------|---------------|--------|
| `tmd_analysis.py` | lines 251-297 | canonical reference | none (lab-frame) |
| `tmd_resolution_acceptance.py` | lines 269-298 | identical | none |
| `extract_multiplicities.py` | lines 178-209 | identical | none |
| `extract_azimuthal_moments.py` | lines 230-270 | identical | none |
| `extract_phi_moments_data.py` | lines 251-281 | identical | none |
| `azimuthal_acceptance.py` | inherits via `tmd_resolution_acceptance` | identical | none |
| `compute_highQ2_crad_charged.py` | lines 252-275 (no phi_h) | identical pT_h + z | none |
| `compute_highQ2_crad_charged.C` | lines 252-266 (no phi_h) | identical pT_h + z | none |
| `task_W_multiplicity_highQ2.py` | inline `compute_pTh_phot` lines 66-94 | algebraically identical pT_h; **z from tree** | none |

Every script:
- Reads `k_beam = (0,0,-E_e, E_e)` with E_e from `BeamElecE` per event
  (or per-event default 27.6 GeV).
- Reads `P_beam = (0, 0, +P_p, sqrt(P_p^2 + M_p^2))` with P_p from
  `BeamProtP` per event (or per-event default 920.0 GeV).
- Computes `q = k_beam - k_scat` from the per-event scattered electron
  (reco or gen depending on context).
- Computes pT_h = `|p × q̂|` and z = `(P · p)/(P · q)`.

No script applies an explicit Lorentz boost. None is needed.

---

## 5. Task W and Task R alignment with the frozen frame

### 5.1 Task W (`task_W_multiplicity_highQ2.py`)

**Frame: SAME as low-Q2 pipeline. No frame fix required.**

`compute_pTh_phot` (lines 66-94) constructs q from the gen scattered
electron 4-vector and the lab-frame incoming electron 4-vector, then
returns `|p - (p · q̂) q̂|` which is algebraically identical to
`|p × q̂|`. The frame is the gamma*+p / HCM frame.

z comes from the `ChargedTot.zGen` branch (line 290). VERIFIED
PROVISIONAL pending a numerical closure test against
`compute_pth_phih_z` z.

**Binning mismatch (separate from the frame question, but blocking
G9):**

| Axis | Task W code | Frozen HQ-A | Match? |
|------|-------------|-------------|--------|
| Q2 | [150, 300, 700, 2000, 20000] | [150, 300, 700, 2000, 20000] | YES |
| x | [0.01, 0.03, 0.1, 0.3, 1.0] | [0.003, 0.01, 0.03, 0.1, 0.5] | **NO** |
| z | [0.1, 0.2, 0.3, 0.5, 0.7] | [0.1, 0.2, 0.3, 0.5, 0.7] | YES |
| pT_h | [0, 0.25, 0.5, 1.0, 1.5, 2.0] | [0, 0.5, 1.0, 2.0, 4.0, 8.0] | **NO** |

Required fix for Task W (does NOT involve the frame):

```python
Q2_EDGES = np.array([150.0, 300.0, 700.0, 2000.0, 20000.0])
X_EDGES  = np.array([0.003, 0.01, 0.03, 0.1, 0.5])
Z_EDGES  = np.array([0.1, 0.2, 0.3, 0.5, 0.7])
PTH_EDGES= np.array([0.0, 0.5, 1.0, 2.0, 4.0, 8.0])
```

The frozen pT_h edges go up to 8 GeV (sensible at sqrt(s)=319 GeV with
Q up to ~140 GeV), and the lowest x edge is 0.003 (not 0.01).

### 5.2 Task R (`compute_highQ2_crad_charged.py` + `.C`)

**Frame: SAME as low-Q2 pipeline. No frame fix required.**

The Python driver's `_compute_pth_z_photonaxis` (lines 252-275) is a
direct slice of `tmd_analysis.compute_pth_phih_z` with phi_h dropped,
keeping `pT_h = |p × q̂|` and `z = (P · p)/(P · q)`.

The C++ macro's `compute_pth_z` (lines 252-266) implements the same
formulas with `TLorentzVector`:
```cpp
TLorentzVector q = k_beam - k_scat;
TVector3 q_hat = q.Vect().Unit();
pT_h = (p_had.Vect().Cross(q_hat)).Mag();
z_phot = P_beam.Dot(p_had) / P_beam.Dot(q);
```

Beam 4-vectors are built identically: `k_beam = (0, 0, -E_e, E_e)` and
`P_beam = (0, 0, +P_p, sqrt(P_p^2 + M_p^2))`. The C++ uses per-period
defaults (E_e = 27.6, P_p = 920) with a `TODO` comment (lines 386-389)
to read per-event values from the HAT GHD bank when Task S confirms the
branch.

---

## 6. TMD-literature cross-check

### 6.1 H1 1302.1321 (HERA-II charged-particle multiplicities, 2013)

Source:
`H1/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md`
lines 41, 128-129, 195-198.

> "Frame | Hadronic centre-of-mass (HCM, virtual γ-p rest frame)"

> "HCM only. Boost is reconstructed from Q² and y (eq. before sec. 3.3);
> for us, exactly the same boost is computed in `H1PartHCMS` from the
> H1OO electron and proton four-vectors. The Breit frame, the natural
> TMD frame, is not used in 1302.1321."

H1 1302.1321 publishes `(1/N) dn/dη*` and `(1/N) dn/dpT*` in the HCM.
The "*" superscript denotes HCM kinematics. The HCM is identical to the
gamma*+p rest frame. The boost in H1OO is implemented by `H1PartHCMS`
from the electron and proton 4-vectors.

### 6.2 DESY 20-176 (charged-particle analysis, ForwardMult)

Source:
`H1/sidis_highQ2/docs/research/research-h1-charged-particle-analysis.md`
lines 519-557.

The output tree branches `ptStarREC, etaStarREC, phiStarREC` are
defined explicitly as "HCM (electron-method boost)" and
`ptStar2REC, etaStar2REC, phiStar2REC` as "HCM (e-Sigma method boost)".
The same HCM-frame conventions are used at gen level
(`ptStarMC[], etaStarMC[], phiStarMC[]`). The starred (*) notation is
the H1 standard for HCM-frame kinematics.

### 6.3 Bacchetta et al (Trento conventions)

Source: arXiv:hep-ph/0410050, Eq. (2.4). Frame: gamma*+p rest frame
(equivalent to the gamma*-N CMS). pT_h is the hadron transverse
momentum perpendicular to q in this frame; z = `(P · p_h) / (P · q)`;
phi_h is the azimuth of p_h about q with phi_h = 0 when the hadron
lies in the lepton plane on the same side as the scattered lepton.

### 6.4 Verdict

The H1 BaryonAsym low-Q2 code, the Task W prototype, and the Task R
C_rad code all compute in **the same frame** as H1 1302.1321 and
DESY 20-176. That frame is the **gamma*+p rest frame = HCM**. It is
the standard TMD frame.

**The Breit frame is NOT used by H1 or by us.** Although Bacchetta et
al sometimes use the Breit frame as a conceptual reference for TMD
factorization, all measured H1 SIDIS multiplicities, asymmetries, and
phi_h moments are reported in the gamma*+p / HCM frame.

---

## 7. High-Q2-specific considerations

There is **no reason** for the high-Q2 analysis to use a different
frame than low-Q2. Specifically:

1. **Boost magnitude.** At high Q^2, the gamma*+p boost magnitude
   along z grows (the virtual photon takes a larger fraction of the
   incoming-electron 4-momentum). This changes the *correspondence*
   between lab-frame variables and gamma*+p-frame variables, but
   does NOT change the gamma*+p-frame variables themselves. Since
   the formulas are Lorentz-invariant under that boost, no special
   high-Q2 handling is needed.

2. **pT_h coverage.** At higher Q^2 the kinematically accessible
   pT_h range is larger because pT_h <= sqrt(Q^2 (1-z) z) plus
   intrinsic-kT smearing. The frozen HQ-A pT_h edges go up to
   8.0 GeV (vs ~2 GeV at low-Q2 frozen binning), which reflects this.

3. **Lepton detector change** (SpaCal at low-Q2 → LAr at high-Q2).
   This changes the resolution of the scattered-electron 4-vector
   `(scatEX, scatEY, scatEZ, scatEE)` but does NOT change the frame
   definition. The formula `q = k_beam - k_scat` is identical.

4. **TMD validity envelope.** The TMD region `pT_h << Q` is more
   comfortably accessible at high-Q2 (Q starts at 12.2 GeV, with the
   HQ-A pT_h ceiling at 8.0 GeV; so pT_h / Q <= 0.7 even in the highest
   pT_h bin). At low-Q2 the same ratio constraint forces a tight
   `pT_h < min(1.5 GeV, Q/2)` cut. The frame is identical in both
   regimes; only the bin envelope is different.

5. **No HERA-specific shift.** H1 used the HCM in 1302.1321 (low-Q2)
   and DESY 20-176 (full range). There is no precedent for switching
   conventions between low-Q2 and high-Q2 within a single H1
   measurement.

**Conclusion:** high-Q2 uses the **identical** gamma*+p / HCM frame
convention as low-Q2, with no modification.

---

## 8. Per-run beam-energy interface

Once per-run beam-energy is available on the v5b trees (planned Z9 work
on storing `BeamElecE` and `BeamProtP` per event):

| Quantity | Source | Per-event or per-run? |
|----------|--------|------------------------|
| E_e (incoming electron energy) | `ChargedTot.BeamElecE` | per-event in v5b; per-period otherwise (ep0304: 27.5; em0405/em06/ep0607: 27.6) |
| P_p (incoming proton momentum) | `ChargedTot.BeamProtP` | per-event in v5b; constant across HERA-II = 920.0 GeV |
| k_beam | `(0, 0, -E_e, E_e)` | per-event (driven by E_e) |
| P_beam | `(0, 0, +P_p, sqrt(P_p^2 + M_p^2))` | effectively per-period; numerically identical across periods |
| k_scat | `(scatEX, scatEY, scatEZ, scatEE)` (reco) or built from `GenElecE, GenElecTheta, GenElecPhi` (gen) | per-event |
| q = k_beam - k_scat | derived | per-event |
| Boost into gamma*+p frame | (E_beam + P_beam in z direction along q̂) | per-event in magnitude, but never explicitly applied because formulas are invariant |

For the high-Q2 C_rad C++ macro (`compute_highQ2_crad_charged.C`):
- The `TODO` comment at lines 386-389 states the macro currently uses
  per-period defaults `E_e = 27.6`, `P_p = 920.0`. Task S should
  confirm whether the HAT GHD bank exposes per-event beam energies
  to override these defaults.
- Even with per-period defaults, **the gamma*+p frame is correctly
  realized** -- the formulas only require the relative direction of
  k_beam and the proton 4-vector, not exact magnitudes. A 0.1 GeV
  shift in E_e produces a ~0.4% shift in Q^2 but does not change the
  frame; it only changes the kinematic mapping.

The boost magnitude into the gamma*+p frame is therefore **per-event**
in principle (it depends on k_beam and k_scat both), but in practice
the H1 BaryonAsym code never performs the boost explicitly: every
SIDIS observable is computed from Lorentz-invariant combinations and
thus inherits the per-event variation of k_scat automatically. The
proton 4-vector enters only via the dot products `P · p_had` and
`P · q` in the z definition, where any per-period/per-event variation
in P_p contributes <1e-4 relative shift in z and is negligible.

---

## 9. The Task W dM/dpT_h CONCERN -- frame is not the cause

The CONCERN flagged on rising `dM/dpT_h` in the Task W prototype was
attributed in chat to "HQ-A photon-axis frame feature". **This audit
confirms the frame is correct.** The rising shape is therefore one of:

1. **Binning mismatch (most likely).** Task W uses
   `PTH_EDGES = [0, 0.25, 0.5, 1.0, 1.5, 2.0]`. Frozen Scheme HQ-A is
   `PTH_EDGES = [0, 0.5, 1.0, 2.0, 4.0, 8.0]`. The Task W binning
   misses the natural high-pT_h falloff and biases the visible shape
   toward the lower-pT_h end where TMD physics has its peak. Task W
   must be re-run with frozen HQ-A edges before G9 can close.

2. **x-binning mismatch.** Task W's x-edges (`[0.01, 0.03, 0.1, 0.3,
   1.0]`) place the lowest bin at the frozen-HQ-A boundary and omit
   the [0.003, 0.01] bin. This restricts the analyzed phase space
   to higher-x than the frozen HQ-A; combined with the truncated
   pT_h ceiling, the visible dM/dpT_h shape is not directly
   comparable to the frozen HQ-A target.

3. **Physical effect.** At fixed (Q^2, x, z), the differential
   `dM/dpT_h` in the gamma*+p frame is expected to be approximately
   Gaussian in pT_h^2 (Cahn-style TMD shape) with a peak at low
   pT_h. The integrated `M(Q^2, x, z)` is dominated by the low-pT_h
   region. A rising visible shape with width-normalized bins in
   [0, 2.0] is suspicious **only if** the bin widths increase
   faster than the underlying density falls. Task W's PTH_EDGES has
   widths `[0.25, 0.25, 0.5, 0.5, 0.5]` -- the bin-width factor
   increases monotonically, so a rising raw count is partly a
   bin-width artifact and partly physics. Frozen HQ-A widths
   `[0.5, 0.5, 1.0, 2.0, 4.0]` also increase monotonically; the
   rising tail is expected at low pT_h coverage but should reverse
   in the highest pT_h bin.

**Recommendation:** re-run Task W with the corrected binning
(Section 5.1) and re-evaluate dM/dpT_h. If still rising in the
[0, 2 GeV] subrange with frozen-HQ-A binning, the shape is the
combination of detector acceptance (which can rise with pT_h) and
the TMD distribution. This is NOT a frame bug and NOT a code bug.

---

## 10. VERIFIED vs PROVISIONAL items

### VERIFIED (read directly from source)
- [V1] `tmd_analysis.py:251-297` `compute_pth_phih_z` implements
  `pT_h = |p × q̂|`, `z = (P · p)/(P · q)`, Trento phi_h via
  `atan2((l×h)·q̂, l·h)`. No explicit boost.
- [V2] Identical formulas in `tmd_resolution_acceptance.py:269-298`,
  `extract_multiplicities.py:178-209`,
  `extract_azimuthal_moments.py:230-270`,
  `extract_phi_moments_data.py:251-281`,
  `compute_highQ2_crad_charged.py:252-275`,
  `compute_highQ2_crad_charged.C:252-266`.
- [V3] `task_W_multiplicity_highQ2.py:66-94` `compute_pTh_phot` uses
  the algebraically equivalent `|p - (p · q̂) q̂|` expression with q
  built from the gen scattered-electron 4-vector. Same frame.
- [V4] All scripts use `k_beam = (0,0,-E_e,E_e)` (lepton -z) and
  `P_beam = (0,0,+P_p, sqrt(P_p^2 + M_p^2))` (proton +z).
- [V5] H1 1302.1321 uses HCM (gamma*-p rest frame) per
  `research-h1-1302-1321-charged-multiplicity.md` lines 41, 128, 195-198.
- [V6] DESY 20-176 / ForwardMult uses HCM per
  `research-h1-charged-particle-analysis.md` lines 519-557 (starred
  branches).
- [V7] Trento phi_h convention metadata is stamped at
  `tmd_analysis.py:79-110` and references arXiv:hep-ph/0410050.
- [V8] Frozen Scheme HQ-A binning per
  `plan-highQ2-sidis-next-steps.md` lines 393-397.
- [V9] Task W binning at `task_W_multiplicity_highQ2.py:38-41` does
  NOT match frozen HQ-A in x and pT_h axes.
- [V10] C++ Crad macro uses per-period defaults E_e=27.6, P_p=920
  with `TODO` for per-event GHD lookup
  (`compute_highQ2_crad_charged.C:384-389`).

### PROVISIONAL (needs a one-time check)
- [P1] Numerical closure test: on a v5b high-Q2 MC sample, compare
  per-track `compute_pth_phih_z` z against the stored
  `ChargedTot.zGen` branch used by Task W. If median residual exceeds
  ~1e-5, the builder's `zGen` uses a different photon convention (e.g.,
  eSigma-method q) and Task W is silently using a slightly different
  z. The Task W comment at line 280 asserts the agreement but does
  not cite the validation script.
- [P2] phi_h sign closure at high-Q2: confirm that for v5b high-Q2 MC,
  `phi_h(reco)` vs `phi_h(gen)` has unit-slope diagonal (not -1
  slope), to guarantee the LAr `scatEX/Y/Z/E` sign convention matches
  the SpaCal one. See `critique-phih-convention-freeze-highQ2-sidis.md`
  ME-4.
- [P3] LAr azimuthal resolution at high-Q2: the
  `wrap(phi_h_reco - phi_h_gen)` RMS has not been measured for high-Q2.
  See `critique-phih-convention-freeze-highQ2-sidis.md` ME-1 and ME-2.
- [P4] Task S deliverable: confirm whether the HAT GHD bank exposes
  per-event beam energies for the Crad macro
  (`compute_highQ2_crad_charged.C:386-389`).

### NOT APPLICABLE (no change required)
- Frame conversion code: not needed; formulas are Lorentz-invariant.
- Explicit boost: not needed.
- Separate high-Q2 frame definition: identical to low-Q2.

---

## 11. Citation block for downstream consumers

When citing this convention in plan documents, output JSON metadata,
or paper drafts, use:

```
SIDIS frame: gamma*+p rest frame (Hadronic CMS, HCM).
   z-axis: virtual photon 3-momentum direction in the gamma*+p frame.
   x-axis: in the lepton plane.
Definitions (Lorentz-invariant; evaluated from lab-frame 4-vectors):
   pT_h  = |p_had × q̂|     with q̂ = q3 / |q3|, q = k_in - k_out.
   z     = (P · p_had) / (P · q).
   phi_h = atan2((l × h) · q̂, l · h),
           l = k_in × k_out, h = q × p_had.
   phi_h ∈ (-π, π], Trento convention
   (Bacchetta, Diehl, Goeke, Metz, Mulders, Schlegel,
    arXiv:hep-ph/0410050 Eq. (2.4)).
Beam 4-vectors (H1 lab):
   k_beam = (0, 0, -E_e, E_e),  E_e per-event from BeamElecE.
   P_beam = (0, 0, +P_p, sqrt(P_p^2 + M_p^2)),  P_p = 920 GeV.
   Convention identical for e+p and e-p HERA-II periods.
Implementation: shared across the low-Q2 charged-particle SIDIS
   pipeline and the high-Q2 Task W (multiplicity prototype) and
   Task R (C_rad). See
   H1/sidis_lowQ2/docs/references/reference-sidis-frame-gamma-star-p.md.
```

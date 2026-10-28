# Area 3 / Critic B — Frames physics (h1-analysis-critic)

**Verdict:** **SOUND** with documentation gaps — 0 fatal + 2 major + 2 documentation issues

## Major concerns

### C3P1. **Hadron mass assumption: M_PION for ALL charged hadrons**

`build_migration_th2_per_charge_NWfix.py:129-130`:
```python
M_PION = 0.13957
M_PROTON = 0.93827
```

Reco hadron mass is set to M_PION at line 162 (`E_h = sqrt(Px²+Py²+Pz²+M_PION²)`). Gen hadron mass also M_PION (the `_compute_pTh_cm_and_z` helper is shared).

**Concern:** For inclusive charged-hadron SIDIS, the hadron is pi, K, or p. The TRUE mass is generally NOT M_PION:
- pi: M_PION (correct, ~62% of charged hadrons at high-Q²)
- K: M_KAON = 0.4937 (3.5x heavier)
- p: M_PROTON = 0.9383 (6.7x heavier)

Using M_PION for all hadrons means E_h is **underestimated by 0.5-1% for kaons and 1-3% for protons** at typical hadron energies of order Q.

**Physics impact:** z = P·h / P·q where P·h = Ep*E_h − E_p*Pz. The underestimated E_h → underestimated z → events migrate to LOWER z bin than physical. Systematic shift in z distribution of order 1-3% in the highest-z bins (where protons dominate).

**Severity:** Per-bin shift of 1-3% in high-z bins, smaller in low-z bins. Comparable to σ_model.

**Fix:** Either (a) compute E_h with the proper mass per gen (use abs(GenPDG) to lookup mass at the matched-fill step), or (b) document the M_PION-approximation choice and propagate as σ_hadron_mass systematic.

**This is exactly the kind of "hidden in plain sight" systematic that needs documentation.** The H1 SIDIS multiplicity literature universally uses M_PION (inclusive). H1's own published charged-hadron multiplicity papers use this approximation. Defensible BUT needs an explicit σ_mass systematic.

### C3P2. **Different sign conventions for xF and pT_h depending on which frame**

`xF_SIDIS_es` (computed in C++ stage 2) uses LAB-z convention → flipped sign vs Trento.
`pT_h_cm` (computed in Python build_migration) uses γ* convention → standard.

**Concern:** Mixing two sign conventions in one analysis invites systematic errors. Future edits could swap a sign without noticing the dual convention.

**Severity:** Latent. Currently no bug because the analysis only uses |pT_h| (sign-agnostic) and `xF < 0` cut (one-sided).

**Fix:** Unify all kinematics to γ*p (Trento) convention OR clearly document the mixed-frame setup.

---

## Documentation gaps

### C3D1. **No explicit statement of which frame defines `pT_h_cm` in the methodology doc**

The methodology doc says "pT_h in γ*-p CM frame" but the code computes it via boost-to-W-rest where W is the γ*p system. **These ARE the same frame** (γ*+p rest frame = γ*p CM rest frame) but the naming is inconsistent across docs.

### C3D2. **z formula choice: P·h / P·q vs Eh/ν**

Code uses `z = P·h / P·q` (Bjorken z, Lorentz-invariant). The methodology doc doesn't specify which z. For inclusive multiplicity these typically agree at <1% in the high-Q² regime. Document the choice.

---

## Recommended next physics check

**Most decisive:** Quantify **C3P1 (M_PION approximation)** by computing z for a sample of matched events using both M_PION and the correct gen-PDG mass. Per-bin difference quantifies the systematic. If >2% in any retained bin, add σ_hadron_mass to the systematic budget.

# Why a 1% electron-energy shift moves the unfolded bins by ~4% (median) — study 2026-10-22

Question (user): with the MC-only leg definition and a ±1% E′ scale, `elec_energy_rel` has a median of 4.1% (h+) /
4.2% (h−) and pushes the quoted median total from 7.5% to 9.7%. Why is it so large?

Code: NAF `scripts/unfolding/final_rerelease_2026-10-22/elecE_study/` (local `scripts/final_rerelease/elecE_study/`).
Outputs and plots: NAF `output_v6/studies/final_rerelease_2026-10-22/elecE_study/`, local
`plots/elecE_scale_study_2026-10-22/`. Nothing canonical was touched.

## Method
- `elecE_sensitivity_study.py` imports the UNMODIFIED shifted F1 data-vector builder and runs its own selection, cone
  and binning at 0 and ±1%. It also runs them with only one group of shifted variables switched on: cell (Q²_eΣ, x_eΣ),
  selection (y_eΣ, E−pz), z, pT, or xF + electron cone.
- Data: all 660 files. MC: 9 RAPGAP files with per-track nominal, +1% and generator values.
- Gates PASS (bitwise): the summed nominal histogram equals `dv_nominal_660`, and the full ±1% histograms equal the
  production `dv_elecE_{p,m}1pct_660`. The unfolded nominal equals the final660 release to <1e-9.
- Attribution uses the data-only form, IBU(D_variant − B; M_nom) / IBU(D_nom − B; M_nom) − 1. It mirrors the
  MC-only systematic: the full-shift medians are 4.9 / 4.8% data-only vs 3.6 / 3.8% MC-only at +1%.

## Findings
1. **The hadron z and pT are computed with the photon taken from the scattered electron alone**:
   q = k − k′ in `_compute_pTh_cm_and_z`. So P·q ∝ y_e = 1 − (E′/E_e) sin²(θ/2).
   - The event variables use the eΣ method (Q², x, y cuts), and xF uses an eΣ photon. z and pT do not.
2. **y_e is a small difference of large numbers at low y.**
   - δy_e/y_e = −(1−y)/y · δE′/E′, which is −9% at y = 0.1 for a 1% E′ shift. The measured per-track values follow this
     curve exactly (Fig. 1).
   - Hence δz/z = +(1−y)/y × 1%: +11% at y_e ≈ 0.09, +5% at 0.17, +1.3% at 0.45.
   - pT (w.r.t. γ*, in the γ*p frame) inherits the same amplification through the frame definition: up to ~1 GeV
     shifts for high-z hadrons at low y.
   - By contrast, the eΣ variables move by only Q² +1%, x +1.5 to +2.8%, y_eΣ −0.5 to −1.7%.
3. **Half of the cells live at low y.** The selection is 0.1 < y_eΣ < 0.7, and the high-x cells (S0_x1, S1_x1, S2_x1)
   have median y_eΣ ≈ 0.18–0.20, piled up against the y = 0.1 cut. The low-x cells have median y ≈ 0.43–0.45.
4. **Steep z and pT spectra turn the shift into a large yield change.** A 5–10% z shift moves the falling z spectrum.
   Per bin, the result is about −50% to +120% in the sparse high-z, high-pT corners of the low-y cells (Fig. 3).
5. **Attribution (Fig. 4/5, median |Δ| per cell at +1%):**
   - In the high-x cells, z alone gives 10–11% and pT alone 5–9%. They partly cancel in the full shift (7–8%).
   - The cell assignment (Q², x) contributes 1.4–4%, the y/E−pz cuts 0.6–1.6%, and the xF and electron cone about 0.
   - In the low-x cells everything is about 2%.
   - By z bin, the effect grows from ~1% at z < 0.05 to ~13% at z > 0.7.
6. **Resolution consequence (Fig. 2).** The electron-photon z resolution degrades from ~4–6% at y > 0.4 to ~25% at
   y ≈ 0.1, and the pT resolution from ~25 MeV to ~260 MeV. An eΣ-based photon (comparison only) gives ~16% and ~180 MeV
   there, and an E′ sensitivity of δz/z = 1.8% instead of 11% at y≈0.09 (equal for y ≳ 0.35).
7. **The old ("both-shifted") definition hid this.** Shifting data and MC together cancels the migration to first
   order (installed median 1.6%), so the effect was never visible before.

## Conclusion
The size is real for the present z/pT definition: it is not a bug and not an artifact of the leg machinery. RAPGAP and
DJANGOH agree, data-only and MC-only agree, and the response is linear (±0.5% gives half). It is a lever arm:
1% in E′ → up to 10% in z and pT at low y → steep spectra. Options for the user (not decided):
- (a) Keep it. It is a correct uncertainty for an electron-photon z/pT at ±1%.
- (b) Revisit the scale magnitude.
- (c) Reconstruct z/pT with an eΣ photon, consistent with Q², x and xF. This is a central-value methodology change
  that needs a full response/leg re-production.

## Follow-up: which photon for z/pT? (user question, 2026-10-22)
Script: `photon_options_study.py` (per-track inputs, Condor 5836906: 660 data files and 73 RAPGAP files) and
`photon_options_analyze.py` (Condor 5836959). Every option goes through the same simplified IBU k=4, with no background
subtraction. Gate: the data bin_prod histogram equals dv_nominal_660. Option A agrees with the official MC-only E′/JES
terms per cell (E′ 3.9/4.2% vs 4.5/4.1% official). Page: `plots/photon_options_2026-10-22/index.html`.

| median % (h+ / h−) | E′ | JES | stat | E′⊕JES⊕stat | high-x E′ | high-x purity |
|---|---|---|---|---|---|---|
| A electron photon (current) | 3.95 / 4.21 | 0.58 / 0.50 | 3.70 / 3.98 | 6.79 / 7.26 | 8.38 / 8.65 | 36 / 35 |
| B eΣ everywhere | 2.57 / 2.75 | 0.83 / 0.84 | 3.69 / 4.11 | 5.15 / 5.75 | 3.38 / 4.39 | 45 / 43 |
| C eΣ in high-x cells (user hybrid) | 2.96 / 3.32 | 0.80 / 0.80 | 3.73 / 4.02 | 5.36 / 6.21 | 3.43 / 4.32 | 45 / 43 |
| D eΣ if y_eΣ < 0.4 | 2.54 / 2.81 | 0.79 / 0.81 | 3.69 / 4.05 | 5.10 / 5.79 | 3.37 / 4.40 | 45 / 43 |

The integrated central value is unchanged (−0.02%). Per bin, the median |shift| vs A is 0.9% for C and 2.4% for B. The
largest shifts are in the sparse high-z/high-pT corners of the high-x cells. Not decided; adopting any option requires
re-producing the response and detector legs.

### Kinematic-fit photon (K), added 2026-10-22 on request
- Inputs: `photon_fit_inputs.py` (Condor 5837078, row-aligned with the photon-options inputs; alignment gate PASS).
  Analysis: `photon_options_analyze.py` (Condor 5837853; v1 backed up as `photon_options_analyze.v1_2026-10-22.py`).
- Fit, per event: a linear least-squares fit of (E′_t, E_eff) to the measured E′, Σ_h = 2·E_e·Yh and
  pT_h = sqrt(Q2h(1−Yh)). The model is Σ_h = 2E_eff − E′_t(1−cosθ) and pT_h = E′_t sinθ, with the prior E_eff = E_e ± σ_ISR.
  - Calibration (median reco/gen) and robust resolutions come from RAPGAP in 12 bins of y_eΣ: Σ_h scale 0.91–0.94,
    pT_h scale 0.95–0.97, ρ(Σ_h, pT_h) ≈ 0.5–0.58.
  - The photon is k − k′(E′_t, θ, φ) with the nominal beam, the same convention as the generator-level target.
- σ_ISR scan (z resolution in the high-x cells): 0 → 8.6%, 1 GeV → 10.2%, 3 GeV → 12.3%, ∞ → 12.9% (A 13.0%, B 11.4%).
  The fixed beam is best, so K = K0.
- The fitted E′ uses the measured E′ with weight ~0.2 at y≈0.1, rising to ~0.8 at y≈0.65: a smooth, per-event version of the hybrid.

| median % (h+ / h−) | E′ | JES | stat | E′⊕JES⊕stat | high-x E′ | high-x JES | high-x purity | low-x purity | median shift vs A |
|---|---|---|---|---|---|---|---|---|---|
| K0 fit | 2.44 / 2.52 | 0.68 / 0.72 | 3.88 / 4.14 | 5.26 / 5.69 | 3.18 / 3.21 | 0.52 / 0.55 | 51 / 50 | 62 / 62 | 3.5 / 2.8 |
| Ks fit + scale unc. in weights | 2.32 / 2.52 | 0.66 / 0.63 | 3.78 / 4.18 | 5.21 / 5.46 | 3.04 / 3.46 | 0.53 / 0.52 | 51 / 50 | 62 / 62 | 3.3 / 2.7 |

- Resolution at y_gen≈0.1: z 11% (A 22%, B 13%); pT 65 MeV (A 127, B 77).
- The integrated central value is unchanged (−0.01%).
- Caveats: larger per-bin redistribution, and reliance on the MC description of the HFS (scale, resolution, correlation) and of the
  ISR tail. Data/MC fit-pull and χ² checks plus an HFS-resolution systematic would be needed before adoption.

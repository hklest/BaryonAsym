**Status:** Current research note. Authoritative catalog of background
sources for the charged-hadron SIDIS measurement and the design specification
for the kinematic-boundary migration study script
(`scripts/study_migration_background.py`).

# Research: Background Sources and Phase-Space Migration for Charged-Hadron SIDIS

**Date:** 2026-05-17
**Audience:** `h1-sidis-supervisor`, `h1-systematics`, `h1-unfolder`,
`code-editor`. Read together with `reference-sidis-systematics.md`,
`research-h1-1302-1321-charged-multiplicity.md`,
`research-h1-charged-particle-analysis.md`, and
`research-crad-for-charged-sidis.md`.
**Scope:** Catalog of all relevant background sources for the inclusive
charged-hadron SIDIS measurement at H1 in the low-Q2 regime
(5 < Q2 < 100 GeV2, 0.1 < y < 0.6) with first-class treatment of the
dominant background (migration across the DIS kinematic boundary), plus the
design spec for a migration-study script that runs on a single per-run
RAPGAP31 MC file.

> The user's explicit prior is that **migration across the DIS kinematic
> boundary is the largest background**: events generated outside the
> analysis (Q2, y) window that reconstruct inside it because of detector
> resolution. The migration study script designed here makes that estimate
> quantitative for the first time on H1 charged-SIDIS MC.

---

## 1. Why Migration is the Dominant Background

### 1.1 Resolution scales versus kinematic windows

From `context-handoff.md` Section 6B (resolutions measured on
RAPGAP31 low-Q2 MC):

| Variable | RMS resolution | Cut window | Width / RMS |
|----------|----------------|------------|-------------|
| log10(Q2) | 0.053 | log10(Q2) in [0.70, 2.0] | ~25 |
| log10(x)  | 0.084 | log10(x) varies          | many |
| y (implicit, via Sigma method) | not directly tabulated; Sigma method amplifies electron resolution by 1/(1-y) | y in [0.1, 0.6] | small at the y = 0.6 edge |
| pT_h | 0.18-0.39 GeV | 0-2 GeV | 5-11 |
| z | 0.075 | 0.1-0.7 | 8 |
| phi_h | 0.104 rad | 2 pi | 60 |

The Q2 resolution is comfortable on a *log* axis but **events very near
the kinematic edges of Q2 in [5, 100] GeV2 still migrate**, particularly
at the lower edge (5 GeV2) where the relative resolution sigma(Q2)/Q2 is
worst. The y-axis is the more dangerous boundary because the Sigma-method
Q2 has a 1/(1-y) amplification (cited in `context-handoff.md` Section 6,
issue "Sigma-Method Q2 Amplification"), so events with true y close to
0.6 routinely reconstruct with y > 0.6 and vice versa.

### 1.2 Steeply falling spectra amplify migration in

DIS cross-sections fall steeply with Q2 (roughly 1/Q4) and with 1-y at
fixed x. The number of events that **could** migrate in from outside the
window (Q2 < 5 or y < 0.1) is *much* larger than the number sitting just
inside the window. Even a small migration probability per event therefore
produces a non-negligible per-bin contamination at the cut edges. This is
the standard "feeddown from outside the phase space" mechanism that
1302.1321 explicitly subtracts (`research-h1-1302-1321-charged-multiplicity.md`
Section 6.3 step 2; the published paper subtracts the outside-of-phase-space
contribution *before* the multiplicative bin-by-bin correction).

### 1.3 Why this is "background" rather than "smearing"

For multiplicities and A_h:

- **Smearing** (events migrating between *internal* analysis bins) is
  corrected by acceptance (bin-by-bin) or unfolding (matrix).
- **Migration in** (events from outside the DIS window reconstructing
  inside it) is an *additive contamination* of the data sample that is
  not in the unfolding response matrix when the response matrix is built
  only from in-phase-space MC. It must be either (a) subtracted as a
  background or (b) handled by extending the response-matrix gen-side
  fiducial to include the outside-phase-space region.

The 1302.1321 paper uses option (a). Our existing 4D acceptance
framework (`scripts/tmd_resolution_acceptance.py`) follows option (a) by
construction: the denominator is gen-level events passing the gen DIS
fiducial, the numerator is matched reco tracks passing the same gen
fiducial. The migration-in contamination is therefore a **systematic on
the data sample**, not a correction factor; the MVP plan must either
subtract it or assign a systematic.

---

## 2. Catalog of Background Sources

This catalog covers all known background sources for the inclusive
charged-hadron SIDIS measurement. Sources are ordered by expected size
in the low-Q2 regime.

### 2.1 DIS kinematic boundary migration (dominant)

| Attribute | Value |
|-----------|-------|
| Generators | RAPGAP31 (rad) and DJANGOH14 (rad) -- both have the migration |
| Mechanism | Reco DIS kinematics differ from gen DIS kinematics due to SpaCal energy/angle resolution and HFS energy resolution; events with gen-level (Q2, y) outside the analysis window reconstruct inside it |
| Boundaries crossed | Q2 low (gen < 5, reco >= 5); Q2 high (gen > 100, reco <= 100); y low (gen < 0.1, reco >= 0.1); y high (gen > 0.6, reco <= 0.6) |
| Expected size | A few percent overall; potentially 10-30% in the lowest-Q2 / highest-y reco bins |
| Affects | Multiplicities (numerator and denominator), A_h via charge-symmetric population shift |
| Mitigation | Subtract using MC truth (subtract migration-in contamination from data N_DIS and N_h); or extend gen fiducial in the response matrix to absorb the migration in the unfolding |
| Systematic | "outside-phase-space" subtraction with a generator-dependent systematic (RAPGAP vs DJANGOH); inherits the same model-dependence treatment as the C_rad model swap |

Per `research-h1-1302-1321-charged-multiplicity.md` Section 6.3, this is
exactly what 1302.1321 does: step 2 of their three-step correction is to
**subtract migrations from outside the (x, Q2, eta*, pT*) phase space**
*before* the bin-by-bin acceptance factor is applied. Our analysis must
do the same.

### 2.2 Photoproduction (gamma-p)

| Attribute | Value |
|-----------|-------|
| Mechanism | Q2 ~ 0 photoproduction event where a hadron in the SpaCal acceptance fakes the scattered electron |
| Suppression | (a) Electron ID (cluster shape, Ecra < 4 cm, HAD fraction < 0.15); (b) Empz in [35, 70] GeV cut suppresses ISR and gamma-p simultaneously; (c) E'_e > 12 GeV cut |
| Expected size | < 0.5% in the analysis phase space per 1302.1321 (`research-h1-1302-1321-charged-multiplicity.md` Section 2, residual gamma-p); up to ~0.9% on the densities in the lowest (x, Q2) bin |
| Affects | Multiplicities and A_h (charge-symmetric to first order) |
| Mitigation | Subtract Phojet template normalized to data; assign 30% systematic on the Phojet normalization |
| Status | Not yet evaluated locally; placeholder in `reference-sidis-systematics.md` source #9 (low-Q2) |

For our analysis the gamma-p contribution is expected to be small (sub-percent
on multiplicities, sub-permille on A_h because it is charge-symmetric).
This source can be deferred but cannot be quoted as zero without a
verification step.

### 2.3 Diffractive DIS

| Attribute | Value |
|-----------|-------|
| Mechanism | Diffractive DIS events (rapidity-gap topology) are included in inclusive DIS but have soft hadronic-final-state production characteristics that differ from non-diffractive DIS |
| Expected fraction | ~5-10% of inclusive DIS events at low x; ~10% of the cross-section per `research-crad-for-charged-sidis.md` Section 5.1 ("Diffractive RAPGAP is scaled down by an extra 0.1") |
| Affects | Multiplicities (different per-event hadron yield); A_h to a lesser degree (charge symmetric for rho0/phi-dominated diffractive final states) |
| Mitigation | RAPGAP includes diffractive component natively; DJANGOH14 does not. The standard H1 method is to scale diffractive MC normalization by 2 (factor-of-2 method) and take the difference as the systematic |
| Status | Listed as source #4 (low-Q2) in `reference-sidis-systematics.md` with "Executable today: No" (diffractive MC not yet processed through `ProtonTreeBuilder_lowQ2.C`); critic line 79 |

For inclusive charged-hadron multiplicities at the per-event level, the
diffractive component is a real physics contribution to the inclusive
DIS cross-section, not a background per se -- but the per-event hadron
yield is lower in diffractive events, which biases M^{h+/-} downward.
For A_h, the diffractive contribution is approximately charge-symmetric
(rho0/phi/Pomeron exchange does not prefer h+ over h-), so the bias on
A_h is much smaller.

### 2.4 Non-DIS backgrounds (cosmic, beam-gas, halo, QED Compton)

| Attribute | Value |
|-----------|-------|
| Mechanism | (a) Cosmic muons projecting through the detector; (b) beam-gas interactions; (c) beam halo; (d) QED-Compton scattering (e + p -> e' + gamma + p, with the gamma faking a hadron or vice versa) |
| Suppression | (a) vertex Z window |z| < 35 cm; (b) ibg background-finder bits 0-6 vetoed at event level (DESY 20-176 standard); (c) HFS hermeticity Empz in [35, 70] GeV; (d) QED-Compton flagged at MC level and at reco level via sub-leading SpaCal cluster |
| Expected size | < 0.1% on inclusive DIS rate after the above vetoes |
| Affects | Negligible for multiplicities; can bias A_h if any one of these has a charge-dependent topology (cosmic muons in particular are charge-dependent at the source) |
| Mitigation | Already vetoed; no additional handling required for MVP |
| Status | Considered fully mitigated by event selection; no dedicated systematic |

### 2.5 Track-level backgrounds

| Attribute | Value |
|-----------|-------|
| Material-interaction secondaries (ParticleSource = 4) | Already vetoed by `ParticleSource != 4` in the analysis numerator; per `memo-mc-truth-policy-charged-sidis.md`, PS = 4 is impossible for IsGtr-tagged tracks but the explicit filter is retained for safety |
| Weak decays (K0_S -> pi+ pi-, Lambda -> p pi-) | Tracks reconstructed as primary because the displaced vertex is near the beamline. Standard H1 V0 contamination: 0.2% from K0_S (`research-h1-1302-1321-charged-multiplicity.md` Section 7); negligible from Lambda |
| Photon conversions (gamma -> e+ e-) | 0.5% from conversion / Dalitz pairs reconstructed as primary tracks (DESY 20-176 Sys.6) |
| Hyperon feed-down to p/p-bar | Sigma+, Sigma-, Xi-, Omega- decays produce primary p/p-bar that are physically valid primary hadrons; do not subtract |
| Affects | Multiplicities (1-2%); A_h via the K0_S contribution being charge-symmetric (mostly cancels) and Lambda/AntiLambda being charge-asymmetric (does not cancel) |
| Mitigation | Apply nucliaREC weight (H1NuclIACor::GetWeight) to compensate MC overestimate of pi/K nuclear absorption -- NOT YET IMPLEMENTED in our builder per `research-h1-charged-particle-analysis.md` Section 8.4 |
| Systematic | 50% on K0_S daughters, 0.5% on conversions per DESY 20-176 Sys.6 |

The combined K0_S/Lambda/conversion track-level background is the
**second largest** source after kinematic-boundary migration. It is
already partially handled by the IsFromPrimary cut and the
ParticleSource != 4 veto in our framework.

### 2.6 Charge-specific backgrounds (A_h-only)

Backgrounds that affect h+ and h- differently and therefore bias A_h
without affecting the charge-summed multiplicity:

| Source | Sign of asymmetry | Magnitude estimate |
|--------|-------------------|---------------------|
| Lambda -> p pi- | Adds h+ (proton) and h- (pi-), but with different kinematic distributions; the proton is faster and may cluster in different (z, pT_h) bins than the pi- | Small (~0.1%); Lambda fraction in primary p sample is < 5% (see `lambda/` analysis) |
| AntiLambda -> p-bar pi+ | Adds h- (p-bar) and h+ (pi+); same magnitude as Lambda but with charges flipped | Mostly cancels with Lambda for the inclusive A_h, but at the per-bin level the Lambda/AntiLambda asymmetry (which is itself a measured quantity in `lambda/`) feeds through into A_h |
| Material interactions: pi+ p -> X vs pi- p -> X | pi- have a larger inelastic cross-section in low-Z material than pi+ at H1 momenta (~few percent difference) | < 0.5% per `reference-sidis-systematics.md` (low-Q2 source #2: 0.2% charge-dependent residual placeholder) |
| Charge-dependent tracking efficiency | Charge-dependent CTD efficiency, charge-dependent track-quality cuts | < 0.5% (DESY 20-176 Sys.5 split between common 0.5% and the charge-dependent residual, which is the part that does not cancel in A_h) |
| Period composition (e+p 2005-06 vs e-p 2006-07) | Different beam charge produces different scattered-electron kinematics; the per-period h+/h- ratios may differ | Up to 1-2% (HERMES precedent: never larger than 2%) per `reference-sidis-systematics.md` (low-Q2 source #8) |

The dominant charge-specific source is **material interactions**. This
is captured separately in the systematic budget; for the migration study
it is irrelevant because kinematic-boundary migration is charge-symmetric
to within ~0.1%.

### 2.7 Summary table

| # | Source | Expected size on M^{h+/-} | Expected size on A_h | Status |
|---|--------|---------------------------|----------------------|--------|
| 1 | DIS kinematic boundary migration | A few percent overall; 10-30% in edge bins | < 0.5% (charge-symmetric) | **TO BE QUANTIFIED BY THIS WORK** |
| 2 | Photoproduction | < 0.5% | < 0.1% | Placeholder; verify |
| 3 | Diffractive DIS | A few percent on multiplicity; differs by Q2 and x | < 0.5% | Blocked (no charged diffractive MC) |
| 4 | Non-DIS (cosmic/beam-gas/QC) | < 0.1% | < 0.1% | Mitigated by selection |
| 5 | Weak decays (K0_S, Lambda) | 0.2-1% on multiplicity | 0.1-0.5% on A_h | DESY 20-176 Sys.6 |
| 6 | Photon conversions / Dalitz | 0.5% | < 0.1% | DESY 20-176 Sys.6 |
| 7 | Material interactions (pi+- asymmetric) | Negligible | < 0.5% | A_h-specific |
| 8 | Charge-dependent tracking | Negligible | < 0.5% | A_h-specific |
| 9 | Period composition (e+p / e-p) | Negligible | < 1-2% | A_h-specific |

---

## 3. Migration Study: Design

This section specifies the migration study script that quantifies source
#1 above on a single per-run RAPGAP31 MC file.

### 3.1 Three classes of tracks

For every track on the `ChargedTot` tree that passes BASE_CUT (track
quality), is matched, and has charge agreement with its gen counterpart,
classify based on whether the reco-level event passes the DIS cuts and
whether the gen-level event passes the gen DIS cuts:

| Class | Reco DIS pass | Gen DIS pass | Physics meaning |
|-------|---------------|--------------|-----------------|
| **Signal** | Yes | Yes | The standard "in-phase-space" class; what acceptance corrects |
| **Migration-in** | Yes | No | Reco-only: the dominant background under study |
| **Migration-out** | No | Yes | Gen-only: lost signal (corrected by the acceptance denominator) |
| (Outside) | No | No | Not in the analysis at any level; ignored |

Reco-level DIS cuts (eSigma baseline; identical to Sigma method for Q2es
per OQ8 resolution):

```
5 < Q2s < 100
0.1 < Ys < 0.6
```

Gen-level DIS cuts (matching set on the gen DIS scalars in
`ChargedTotGen` and on the reco tree as `Q2sGen`, `YsGen`):

```
5 < Q2sGen < 100
0.1 < YsGen < 0.6
```

### 3.2 Migration fraction definition

For each kinematic variable v (Q2, x, z, pT_h, y) and each bin k:

```
f_mig_in(v, k) = N_mig_in(v, k) / [ N_signal(v, k) + N_mig_in(v, k) ]
```

where the counts are **weight1 * weight2** weighted and the variables in
`v` are reconstructed at reco level (because that is what the bin index
of the *contaminated* sample uses). The migration-in fraction is a
per-bin contamination on the reco distribution.

For completeness the script also stores `N_mig_out` (gen-binned) so the
denominator-side migration effect on acceptance is visible.

### 3.3 Boundary-crossing decomposition

Each migration-in track is classified by *which* boundary was crossed at
gen level:

| Tag | Gen-side condition | Comment |
|-----|---------------------|---------|
| `Q2_low`  | `Q2sGen < 5`   | Steeply rising spectrum -> dominant edge in low-Q2 |
| `Q2_high` | `Q2sGen > 100` | Falling spectrum -> sub-dominant |
| `y_low`   | `YsGen < 0.1`  | y resolution near edge; Sigma method amplification |
| `y_high`  | `YsGen > 0.6`  | Trigger turn-on edge |
| `multi`   | Two or more of the above | Joint boundary crossings |

The boundary-crossing decomposition is reported as a stacked histogram
versus reco Q2 and versus reco x.

### 3.4 Bin choices

Use the analysis canonical bins from `tmd_resolution_acceptance.py`:

```
Q2:   [5, 10, 20, 50, 100] GeV2
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]
z:    [0.1, 0.2, 0.3, 0.5, 0.7]
pT_h: [0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0] GeV
y:    [0.1, 0.2, 0.3, 0.45, 0.6]
```

For the 2D map use (Q2, x) with the standard 4 x 4 grid.

### 3.5 Variable computation policy

- **Q2, x, y**: read directly from the reco tree as `Q2s`, `xBj`, `Ys`
  for reco-level; `Q2sGen`, `xGen`, `YsGen` for gen-level. Both are
  on the `ChargedTot` tree (per `context-handoff.md` Section 4).
- **z and pT_h**: photon-axis definitions; compute on the fly from the
  reco scattered electron, the hadron 4-vector, and the beam 4-vectors
  using the same `compute_pth_phih_z` helper as in
  `tmd_resolution_acceptance.py`. For gen-level z and pT_h, reuse the
  reco scattered electron as the proxy for the gen electron (the
  approximation has phi_h RMS = 0.104 rad and is documented as
  acceptable; see `context-handoff.md` Section 6).
- **phi_h**: also computed from the photon axis; not central to the
  migration study but included as a sixth diagnostic distribution.

### 3.6 Plots produced

The script emits exactly six PNG plots plus a JSON summary:

| File | Content |
|------|---------|
| `migration_vs_Q2.png` | Migration-in fraction f_mig_in(Q2_reco) with statistical error bars and an inset stacked bar showing the boundary-crossing decomposition per bin |
| `migration_vs_x.png` | Migration-in fraction f_mig_in(x_reco) with the same boundary-crossing decomposition |
| `migration_vs_z.png` | Migration-in fraction f_mig_in(z_reco) |
| `migration_vs_pTh.png` | Migration-in fraction f_mig_in(pT_h_reco) |
| `migration_vs_y.png` | Migration-in fraction f_mig_in(y_reco) with the boundary-crossing decomposition |
| `migration_2d_Q2_x.png` | 2D migration-in fraction f_mig_in(Q2_reco, x_reco) as a `pcolormesh` |
| `migration_summary.json` | Per-bin counts, fractions, and boundary-crossing breakdowns for all variables |

### 3.7 Migration-out reporting

The migration-out class is computed on the gen side and reported as
text/JSON only (no plot in the six-plot deliverable). The migration-out
*fraction* per gen bin is the complement of the standard acceptance
restricted to the gen DIS window:

```
f_mig_out(v_gen, k) = N(reco fails DIS | gen passes DIS) / N(gen passes DIS)
```

For internal sanity it should equal 1 - epsilon(v_gen, k) up to a small
correction from track-level cuts. The JSON includes integrated values
for documentation.

### 3.8 Per-track vs per-event counting

The migration-in fraction is computed **per track** (one entry per
hadron in `ChargedTot`). This is the right quantity for multiplicity
backgrounds because multiplicities are per-bin track counts. The event
classification is identical for all tracks within the same event by
construction; therefore the per-track fraction equals the per-event
fraction when integrated over hadrons.

A separate per-event count is also written to the JSON for cross-check
with the DIS event-level `DisTot` denominator.

### 3.9 Charge-dependent migration check

The script also reports the migration-in fraction split by hadron charge
(h+ and h-). If `f_mig_in(h+, k) - f_mig_in(h-, k)` is below 0.5% in
every bin, the migration background is charge-symmetric and the A_h
correction is negligible. If the difference exceeds 0.5%, the analysis
must apply a per-charge migration subtraction.

### 3.10 Use of weights

All counts are weighted by `weight1 * weight2`. The `weight2` field is
always 1.0 (per `context-handoff.md` Section 3 "MC weights"), so the
weighting is in practice `weight1`. The sum-of-weights-squared is
tracked per bin for statistical error bars.

### 3.11 Limitations of the single-file pilot

Running on a single per-run RAPGAP31 file (~50k DIS events typical)
gives statistically significant migration-in fractions in the dominant
edge bins (Q2 in [5, 10], y in [0.1, 0.2] and [0.45, 0.6]) but may be
statistics-limited in interior bins. The script is designed so that the
JSON output can be merged across per-run files for the full-statistics
production run later.

---

## 4. Expected Findings (Predictions)

These predictions guide validation of the script output. If the actual
output differs by more than a factor of two from these ranges, the
script should be re-checked for bugs before any interpretation:

1. **Integrated migration-in fraction**: 3-8% of all matched in-window
   reco tracks. The 1302.1321 outside-of-phase-space subtraction is at
   the few-percent level integrated.
2. **Lowest Q2 bin** (5 < Q2_reco < 10): 10-25% migration-in fraction,
   dominated by `Q2_low` boundary crossings (gen Q2 < 5).
3. **Highest y bin** (0.45 < y_reco < 0.6): 5-15% migration-in, dominated
   by `y_high` boundary crossings.
4. **Lowest y bin** (0.1 < y_reco < 0.2): 5-15% migration-in, dominated
   by `y_low` boundary crossings.
5. **Interior bins** (Q2 in [20, 50], y in [0.2, 0.45]): < 2%
   migration-in.
6. **Charge symmetry**: |f_mig_in(h+) - f_mig_in(h-)| < 0.3% in every
   bin.
7. **z and pT_h dependence**: nearly flat in z, slight rise at high pT_h
   because high-pT_h hadrons correlate with hard scattering (larger
   true Q2) and therefore with the Q2_low edge being less populated.
8. **Migration-out fraction** (gen bins): same order of magnitude as
   migration-in, by construction in a well-conditioned migration matrix.

---

## 5. How the Migration Number Enters the Analysis

Once `f_mig_in(Q2, x, z, pT_h)` is known per bin and per charge, the
correction to the measured multiplicity is multiplicative:

```
N_h_corrected(bin) = N_h_measured(bin) * (1 - f_mig_in(bin))
```

equivalently the migration-in count is subtracted before the acceptance
correction is applied. The acceptance is then computed from in-phase-space
gen and matched-reco tracks only, exactly as in
`tmd_resolution_acceptance.py`.

For A_h, the per-charge correction is:

```
N_h+_corr = N_h+_measured * (1 - f_mig_in(h+, bin))
N_h-_corr = N_h-_measured * (1 - f_mig_in(h-, bin))
A_h_corr  = (N_h+_corr - N_h-_corr) / (N_h+_corr + N_h-_corr)
```

If the charge-symmetry test in Section 3.9 passes (< 0.5% difference),
the `(1 - f_mig_in)` factor is the same for h+ and h- and cancels in
A_h. The migration background then only affects multiplicities, not the
asymmetry.

The systematic uncertainty on the migration correction is the difference
between RAPGAP31 and DJANGOH14 (when the DJANGOH14 migration study is
also run). Until DJANGOH14 is available, assign 100% of the correction
as the systematic, consistent with the C_rad model-swap practice in
`research-crad-for-charged-sidis.md` Section 9.

---

## 6. Relation to Existing Pipeline

| Existing artifact | Relation |
|-------------------|----------|
| `tmd_resolution_acceptance.py` | Computes acceptance with both numerator and denominator inside the gen DIS fiducial. The migration-in background is OUT OF SCOPE of this script by construction; the migration study fills the gap |
| `compute_systematics_charged.py` | Will need a new `migration_subtraction` source once the migration fraction is computed. Conceptually similar to `crad_model_swap` |
| `reference-sidis-systematics.md` | Add a new low-Q2 source after #11 ("Wide-bin extrapolation"): `kinematic_boundary_migration`. Owner: `h1-acceptance` (or `h1-unfolder` if unfolding is adopted). Authoritative analog: 1302.1321 step-2 subtraction |
| `research-crad-for-charged-sidis.md` | Migration in the DIS event count is implicit in `C_rad_DIS`; the per-particle migration is a separate effect captured by this study |
| `research-h1-1302-1321-charged-multiplicity.md` Section 6.3 | 1302.1321 subtracts the outside-of-phase-space contribution before the bin-by-bin correction. This is the published precedent for the procedure |

---

## 7. Implementation Notes for `study_migration_background.py`

Implementation choices match the existing low-Q2 analysis-script conventions:

1. **Input file**: a single per-run RAPGAP31 ROOT file containing
   `ChargedTot` with `Q2s`, `Ys`, `Q2sGen`, `YsGen`, the reco
   scattered-electron 4-vector branches (`scatEX`, `scatEY`, `scatEZ`,
   `scatEE`), beam energies (`BeamElecE`, `BeamProtP`), hadron
   kinematics (`Px`, `Py`, `Pz`, `Pt`, `Eta`, `Charge`, `GenPDG`), gen
   hadron kinematics (`GenPt`, `GenEta`, `GenPhi`), matching/quality
   branches (`IsMatched`, `GenCharge`, `IsFromPrimary`, `NHitDedx`,
   `RadLength`, `DcaPrime`, `ParticleSource`, `TrackType`), and weights
   (`weight1`, `weight2`).
2. **Streaming**: read in chunks via `uproot.iterate` to avoid loading
   the entire file into memory.
3. **Selections applied**:
   - BASE_CUT (track-type-aware) -- mirror `tmd_resolution_acceptance.py`
     `make_base_mask` exactly: central tracks get the standard CJC quality
     cuts, combined tracks get the relaxed cuts; both require
     `Pt > 0.15`, `IsFromPrimary == 1`, and the type-appropriate
     pseudorapidity bound.
   - Matched: `IsMatched == 1` and `Charge == GenCharge`.
   - Species: `|GenPDG| in {211, 321, 2212}`.
   - ParticleSource != 4 (already implicit but explicit for safety).
4. **No gen-fiducial pre-cut** in the migration study (unlike the
   acceptance study). The migration study needs to see tracks whose gen
   event is OUTSIDE the DIS window, which are dropped by the gen-fid
   mask.
5. **Photon-axis kinematics**: use the same helpers as
   `tmd_resolution_acceptance.py` (`compute_pth_phih_z`,
   `hadron_4vec`, `beam_4vectors`, `scattered_electron_4vec`).
6. **No file writes outside the requested output dir**: the script must
   accept `--output-dir` and write all six PNGs plus
   `migration_summary.json` into it. No writes anywhere else.
7. **Plot style**: Okabe-Ito palette as defined in
   `tmd_resolution_acceptance.py` lines 99-135 -- the project's
   plotting-style skill. No grids. Mathtext for variable labels. DPI 200.
8. **Stat error bars**: use the standard sum-of-weights / sqrt(sum-of-weights^2)
   binomial-style propagation for the migration-in fraction.

---

## 8. Open Questions and Gaps

1. **Should we also run on DJANGOH14?** Yes, eventually -- the
   generator-difference is the systematic. For the pilot script on one
   per-run RAPGAP31 file the DJANGOH14 comparison is deferred to a
   full-dataset production rerun. The script is generator-agnostic by
   construction.
2. **Per-charge migration study**: the script reports per-charge
   migration-in fractions and the h+/h- difference. If the difference
   exceeds 0.5% in any bin, escalate; otherwise the A_h is unaffected
   by this background.
3. **Migration in versus diffractive contribution**: diffractive events
   pass the DIS cuts at both gen and reco level, so they are
   classified as "signal" in this study. The diffractive contamination
   is a separate background (Section 2.3) and is not captured by the
   migration fraction. Do not interpret f_mig_in as a total background
   estimate; it is the kinematic-boundary contribution only.
4. **Migration in versus photoproduction**: gamma-p events have gen-level
   Q2 ~ 0 (always outside the DIS window) and reco-level fake-electron
   kinematics that may or may not pass the DIS cuts. RAPGAP DIS MC does
   NOT contain photoproduction; this study therefore captures only the
   DIS-internal Q2/y boundary migration, NOT the photoproduction
   contamination. The two backgrounds are additive.
5. **DisTot interaction**: the DIS event count used by multiplicities
   comes from `DisTot`, not `ChargedTot`. The per-event migration-in
   correction on N_DIS may differ from the per-track migration-in
   correction on N_h. The script reports per-event counts derived from
   the unique event identifiers in `ChargedTot` for a first estimate;
   a follow-up version should read `DisTot` and apply the same
   classification to N_DIS directly.
6. **Reco-electron-as-gen-electron proxy** for gen-level pT_h and phi_h
   introduces a small bias in the gen kinematics. For Q2 and y (which
   are read directly from the gen DIS scalars) there is no proxy
   involved, so the boundary-crossing classification is exact.

---

## 9. Risks and Failure Modes

| Risk | Mitigation |
|------|------------|
| Per-run file does not contain `Q2sGen`/`YsGen` branches | Script must check branch presence at startup and fail loudly with the missing branches listed |
| Per-run file is the v3 builder output before proton-tree removal (still contains `ProtonTot`) | Harmless; the script reads `ChargedTot` only and ignores `ProtonTot` |
| Statistics in extreme bins (e.g. Q2 in [50, 100], y in [0.45, 0.6]) are < 100 weighted tracks; migration fraction noisy | Report stat errors honestly; flag bins with N_signal + N_mig_in < 100 in the JSON output |
| `ChargedTot` has the `_ch` alias suffix on some data branches | Not relevant for MC; the MC file does not use the alias scheme |
| The reco electron 4-vector branches are zero / sentinel for some events | Skip those events in the per-track loop; report the skip count in the JSON |
| Plot style departs from the Okabe-Ito convention | Use the same rcParams block as `tmd_resolution_acceptance.py` (lines 99-135) verbatim |

---

## 10. Provenance

- `context-handoff.md` (Sections 4, 6, 6B) -- tree schema, resolutions, known issues.
- `research-h1-1302-1321-charged-multiplicity.md` Sections 6.3, 7 -- outside-phase-space subtraction precedent.
- `research-h1-charged-particle-analysis.md` Sections 3, 8 -- DESY 20-176 event-selection cuts and tracking systematics.
- `research-crad-for-charged-sidis.md` Sections 1, 2 -- C_rad / migration relationship for DIS event count.
- `reference-sidis-systematics.md` Sections "Low-Q2 Ordered Catalog" #1, #4, #9 -- where this new source slots in.
- `scripts/tmd_resolution_acceptance.py` -- canonical implementations of BASE_CUT, photon-axis kinematics, plotting style.

---

## 11. Status

This research note is **standalone-complete** for the design of
`study_migration_background.py`. The script can be implemented from this
document plus `tmd_resolution_acceptance.py` without consulting other
sources. The expected output (Section 4) provides validation predictions.

Once the script runs on the user-specified per-run RAPGAP31 file at
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2/output/mc/rapgap31/condor_07_evlsMC_RAPGAP31_Rad_1_proton.root`,
the resulting JSON should be cross-checked against Section 4
predictions. If predictions are confirmed, the next step is to:

1. Add `kinematic_boundary_migration` as source #12 in the low-Q2
   catalog of `reference-sidis-systematics.md`.
2. Promote the per-bin migration fractions into
   `compute_systematics_charged.py` as a new source `migration_subtraction`.
3. Re-run the full-dataset migration study on the merged low-Q2 v3 MC
   when it is available, and again on DJANGOH14 for the model
   systematic.

The pilot study on a single per-run file will already provide the
order-of-magnitude estimate needed to plan the systematic-budget
allocation for this source.

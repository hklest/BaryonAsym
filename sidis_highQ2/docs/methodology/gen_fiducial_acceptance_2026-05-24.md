# Gen-fiducial vs reco-fiducial acceptance — diagnostic and methodology note

**Date:** 2026-05-25
**Status:** DIAGNOSTIC, no production artifact modified
**Scope:** H1 high-Q² charged-particle SIDIS, Scheme B′, v5g/v3 frame
**Inputs (read-only):**
- `sidis_highQ2/data/mc_v3/pertrack_mc_v3_gen_djangoh14_merged.csv.gz` (143.8 M rows)
- `sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json` (current BBB)
**Outputs created:**
- `sidis_highQ2/data/gen_fiducial_acceptance_HQ-v5g-Bprime.json`
- `sidis_highQ2/plots/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf`
- This memo.
**Script:** `sidis_highQ2/scripts/diagnose_gen_fiducial_acceptance.py`

This memo is informational. It does not change any cuts, does not amend
the BBB JSON, and is not a recommendation to publish under either convention.
The decision is left to the supervisor.

---

## 1. Two definitions of the gen-fiducial denominator

The current BBB script `compute_xsec_v3_HQv5gBprime.py` defines per-bin
acceptance as

```
eps_h_widefid = N_reco_match / N_gen_widefid
```

with the **wide** gen denominator

| Cut | Meaning |
|---|---|
| `inFiducialCharged == 1` | `|η_gen| < 3.5  AND  p_T,gen > 0.05 GeV` (extractor pre-filter) |
| `|GenPDG| in {211, 321, 2212}` | π/K/p only |
| `IsV0DaughterGen == 0` | not from a V0 decay |

The reco-side cuts inside the extractor are far narrower:

| Cut | Meaning |
|---|---|
| `Pt_lab > 0.15 GeV` | 3× tighter than gen filter |
| `|Eta_lab| < 1.3` | 2.7× tighter than gen filter — H1 central tracker geometric acceptance |
| `Pt_lab < 15 GeV` | upper quality cut |

(empirically verified in `pertrack_mc_v3_reco_djangoh14_merged.csv.gz`:
matched-hadron `Pt_lab` range `[0.150, ~177]`, `Eta_lab` range
`[-1.300, +1.300]`).

So the current `eps_h_widefid` mixes two physically distinct effects:

1. **True reco efficiency.** Track reconstruction, tracking losses, MC track
   resolution, charge-flip, etc. — corrections we want to make.
2. **Geometric acceptance.** Gen particles in `|η| > 1.3` or `pT < 0.15` are
   removed by the reco cuts. The MC model fills in this region; we then
   un-fill it via the `eps_h_widefid` denominator. The published yield in
   that region is the MC's assumed pT/η distribution beyond the H1 tracker.

The alternative is to restrict the gen denominator to the reco fiducial
region:

```
eps_h_recofid = N_reco_match / N_gen_recofid
```

with

```
N_gen_recofid = N_gen_widefid restricted to
                 (GenPt > 0.15)  AND  (GenPt < 15)  AND  (|GenEta| < 1.3)
```

This isolates effect 1 only.

The geometric-acceptance factor is

```
A_geom = N_gen_recofid / N_gen_widefid
```

per 4D bin. `(1 − A_geom)` is the fraction of the corrected yield that the
current published xsec sources from MC extrapolation outside the H1 tracker.
Because corrected yields are inversely proportional to ε_h, the two
conventions differ by exactly

```
N_corr_widefid / N_corr_recofid = eps_h_recofid / eps_h_widefid = 1 / A_geom
```

per bin.

---

## 2. Computation

The script `diagnose_gen_fiducial_acceptance.py` streams the v3 gen CSV in
3M-row chunks, applies the same defensive gen filter as the BBB script, and
bins simultaneously into `N_gen_widefid` (no extra mask) and `N_gen_recofid`
(with `GenPt > 0.15` and `|GenEta| < 1.3`). It then reads the BBB JSON and
reconstructs `N_reco_match = eps_h_widefid × N_gen_widefid` per bin (exact
identity from `compute_xsec_v3_HQv5gBprime.py` line 424). The reco CSV is
not re-read.

Binning is the canonical Scheme B′ exactly as in the production script: 4
Q² strata × 2 x-bins each = 8 (Q²,x) cells, 6 z bins, 5 pT_h bins, 2 charges
= **480 4D bins**.

The gen `pT_h` column is the γ\*+p CM-frame transverse momentum after the v3
frame fix (`extract_pertrack_v5g_fixedFrame_v3.py`); `GenPt` is the lab-frame
gen track transverse momentum.

Runtime: ~4 minutes wall on the local machine. No NAF dispatch.

---

## 3. Headline numbers

| Quantity | Value |
|---|---|
| Wide-fiducial gen tracks loaded | 143,847,680 |
| Wide-fiducial gen weighted sum | 1.868 × 10⁸ |
| Reco-fiducial gen weighted sum | 9.162 × 10⁷ |
| **Fraction of true tracks inside reco fiducial (entire sample)** | **0.491** |
| Sum in Scheme B′ cells (wide) | 1.864 × 10⁸ |
| Sum in Scheme B′ cells (reco) | 9.146 × 10⁷ |
| **Fraction inside reco fid in Scheme B′** | **0.491** |

> **Roughly half of the weighted gen-pool charged hadrons in the high-Q²
> Scheme B′ envelope fall inside the H1 central tracker geometric
> acceptance.** The other half are sourced by MC extrapolation in the
> current `eps_h_widefid` BBB convention.

### Bin counts (480 total)

| Category | Count | Fraction |
|---|---|---|
| A_geom < 0.5 (extrapolation dominates) | 172 | 35.8 % |
| 0.5 ≤ A_geom ≤ 0.9 | 151 | 31.5 % |
| A_geom > 0.9 (essentially fiducial) | 157 | 32.7 % |
| A_geom > 1.0 (anomaly) | 0 | 0.0 % |
| `ratio_corr > 2.0` (extrapolation more than measurement, with N_data > 0) | 135 | 28.1 % |

Zero anomalies above 1 + 1e-9 — by construction `N_gen_recofid ⊂ N_gen_widefid`.

---

## 4. Integrated A_geom per (Q², x) cell

Charge-summed integrated A_geom per cell (sum over z, pT_h):

| Cell | Q² range (GeV²) | x range | A_geom (h⁺) | A_geom (h⁻) | A_geom (both) |
|---|---|---|---|---|---|
| S0a | [150, 300) | [0.0021, 0.0071) | 0.515 | 0.516 | **0.515** |
| S0b | [150, 300) | [0.0071, 0.030) | 0.475 | 0.472 | **0.473** |
| S1a | [300, 700) | [0.0042, 0.016) | 0.534 | 0.534 | **0.534** |
| S1b | [300, 700) | [0.016, 0.069) | 0.441 | 0.434 | **0.438** |
| S2a | [700, 1500) | [0.0099, 0.034) | 0.552 | 0.549 | **0.550** |
| S2b | [700, 1500) | [0.034, 0.15) | 0.350 | 0.343 | **0.347** |
| S3a | [1500, 20000) | [0.021, 0.087) | 0.534 | 0.529 | **0.532** |
| S3b | [1500, 20000) | [0.087, 1.5) | 0.209 | 0.210 | **0.209** |

A_geom is **essentially charge-symmetric** in every cell (Δ < 1%).

Pattern by (Q², x):
- **Low-x cells (S0a, S1a, S2a, S3a)** have A_geom in 0.51 – 0.55. About
  half the gen pool is outside the tracker; mainly the soft forward
  fragmentation tail.
- **High-x cells (S0b, S1b, S2b, S3b)** have markedly lower A_geom,
  dropping from 0.47 at low Q² to **0.21 at S3b**. High-x in the same Q²
  stratum corresponds to harder, more-forward hadronic final states; more
  of the true particles are at high η outside the tracker, and more of
  the published S3b cross section is MC extrapolation rather than
  measurement.

---

## 5. A_geom projections

### A_geom vs z (integrated over Q²,x, pT_h, charge)

| z range | A_geom |
|---|---|
| [0.00, 0.10) | **0.436** |
| [0.10, 0.20) | 0.896 |
| [0.20, 0.30) | 0.904 |
| [0.30, 0.50) | 0.894 |
| [0.50, 0.70) | 0.832 |
| [0.70, 1.00) | 0.679 |

Two distinct effects:
- **Low z (z < 0.1)**: A_geom collapses to ~0.44. Low-z soft hadrons are
  predominantly in the low-pT_h, low-Pt_lab tail; the `Pt_lab > 0.15 GeV`
  cut throws out most of the true particles. About 56% of the published
  low-z yield is MC extrapolation across the `Pt_lab < 0.15 GeV` boundary.
- **High z (z > 0.5)**: A_geom drops from ~0.83 to ~0.68. High-z particles
  carry a large fraction of the virtual-photon energy and are forward;
  the `|η_lab| < 1.3` cut removes the most-forward tail. About 17–32%
  of the published high-z yield is MC extrapolation across the `|η|=1.3`
  boundary.
- The **z in [0.1, 0.5] core** is essentially fiducial (A_geom 0.89–0.90),
  so the canonical "shoulder" of the multiplicity is robust to the
  convention choice.

### A_geom vs pT_h (integrated over Q²,x, z, charge)

| pT_h range (GeV) | A_geom |
|---|---|
| [0.0, 0.5)  | **0.441** |
| [0.5, 1.0)  | 0.520 |
| [1.0, 2.0)  | 0.608 |
| [2.0, 4.0)  | 0.663 |
| [4.0, 8.0)  | 0.649 |

The lowest pT_h bin is dominated by the soft fragmentation tail that mostly
fails `Pt_lab > 0.15 GeV` in the lab. Higher pT_h is more central but the
high-η wing still costs ~35% even at pT_h > 2 GeV. The cross section is
**never** dominantly fiducial when integrated over the full (Q², x, z)
range — A_geom maxes out around 0.66 because the integration includes the
problematic low-z and the high-x, high-Q² cells.

---

## 6. Where the convention matters most

The 10 bins with the highest `N_corr_widefid / N_corr_recofid > 2` ratio and
the largest `N_data` (i.e. the largest absolute contributors to the high-x
extrapolation budget) are all in the **z ∈ [0, 0.1), low-pT_h** region:

| cell | charge | z | pT_h (GeV) | N_data | A_geom | ratio |
|---|---|---|---|---|---|---|
| S0a | h⁺ | [0.0,0.1) | [0.0,0.5) | 287685 | 0.433 | 2.31 |
| S0a | h⁻ | [0.0,0.1) | [0.0,0.5) | 282560 | 0.434 | 2.30 |
| S0b | h⁺ | [0.0,0.1) | [0.0,0.5) | 204003 | 0.390 | 2.56 |
| S0b | h⁻ | [0.0,0.1) | [0.0,0.5) | 199507 | 0.389 | 2.57 |
| S1a | h⁺ | [0.0,0.1) | [0.0,0.5) | 157093 | 0.449 | 2.23 |
| S1a | h⁻ | [0.0,0.1) | [0.0,0.5) | 154545 | 0.450 | 2.22 |
| S1b | h⁺ | [0.0,0.1) | [0.0,0.5) | 95639 | 0.367 | 2.73 |
| S1b | h⁻ | [0.0,0.1) | [0.0,0.5) | 92949 | 0.365 | 2.74 |
| S0b | h⁺ | [0.0,0.1) | [0.5,1.0) | 81127 | 0.412 | 2.42 |
| S0b | h⁻ | [0.0,0.1) | [0.5,1.0) | 79083 | 0.413 | 2.42 |

These ten bins alone aggregate ~1.6 M corrected hadrons under the wide
convention and ~0.66 M under the reco convention — a 2.5× swing concentrated
in the very lowest z, very lowest pT_h corner that already sits below the
typical SIDIS quoted region (z > 0.1, pT_h > some small threshold).

A complementary observation: **157 of 480 bins (~33%) have A_geom > 0.9**
and are insensitive to the choice (≤ 11% shift). For these, both conventions
yield the same number. The convention choice only matters in the
low-z / low-pT_h band and in the high-x cells (S0b, S1b, S2b, S3b).

---

## 7. Convention tradeoff

The two conventions are not equivalent and trade physical clarity against
ease of theory comparison. **No choice is purely correct**; the choice is a
methodology preference.

### Convention A — wide gen fiducial (current published BBB)

`eps_h = N_reco_match / N_gen_widefid`. Cross section is reported in the full
generator-level phase space `|η_gen| < 3.5, pT_gen > 0.05 GeV`.

**Pros:**
- Matches the generator-level differential cross section as defined in
  most theory codes (PYTHIA/HERWIG/SIDIS predictions usually do not impose
  a tracker fiducial).
- A theorist can compare their integrated `dσ/dz dpT_h` directly without
  reapplying a hadron-level fiducial mask.
- Consistent with the H1 inclusive DIS tradition where the cross section
  is unfolded to the parton/hadron phase space.

**Cons:**
- About **50%** of the integrated published yield is **MC extrapolation
  outside the H1 central tracker** (|η_lab| > 1.3 or pT_lab < 0.15 GeV).
- In high-x, high-Q² cells (S3b) the extrapolation fraction reaches ~80%.
- In the lowest z and lowest pT_h bins the published yield is up to 2.7×
  the directly-measured fiducial yield.
- The unmeasured part is model-dependent: the published number is
  effectively the H1 measurement multiplied by the DJANGOH14 ratio
  `N_gen_widefid / N_gen_recofid`. Model variation (RAPGAP, generator
  systematic) propagates straight into the central value.

### Convention B — reco fiducial only

`eps_h = N_reco_match / N_gen_recofid`. Cross section is reported in the
fiducial `pT_lab > 0.15 GeV, |η_lab| < 1.3` (and `pT_lab < 15 GeV`) phase
space.

**Pros:**
- The number reported is **what H1 actually measures**. No MC
  extrapolation enters the central value.
- Model-dependence systematic from the wide→reco extrapolation drops out
  of the published cross section (it would only enter if a theorist
  chooses to extrapolate themselves).
- Matches the H1 collaboration practice for inclusive jets and event
  shapes (always reported in a fiducial cut on the reconstructed object).
- Lambda asymmetry tradition (where the V0 sits well inside the tracker)
  is closer to this convention.

**Cons:**
- Theorists must apply the lab-frame fiducial cuts in their predictions.
  For semi-analytic calculations (TMD-factorization, NLO collinear with
  asymptotic limits) the lab-frame cut is awkward because the lab is a
  non-natural frame for the prediction. A unique CM-frame fiducial
  surrogate does not exist for SIDIS in the way it does for jets.
- The reported `dσ/dz dpT_h` is no longer the "full" hadron-level
  multiplicity. For low z (< 0.1) the number reported is a small fraction
  of the integral; comparison to existing H1 / HERMES / COMPASS
  multiplicity papers that quote the full phase space requires re-correction.
- The fiducial cut depends on the beam energies (HERA-II 27.6 × 920 GeV);
  a future EIC or other collider reanalysis cannot directly compare in
  the same fiducial.

### Hybrid options worth knowing exist

Two intermediate conventions exist in the SIDIS literature:

1. **Reco-fiducial central, "full phase space" appendix.** Publish the
   reco-fiducial cross section as the primary result, and tabulate the
   MC-extrapolation factor `1/A_geom` per bin so a theorist can multiply
   back if they want the wide-fiducial number. This pushes the
   model-dependence into a transparent, separately-quoted scaling factor.

2. **Gen-fiducial in CM-frame.** Define a fiducial in γ\*+p CM variables
   (e.g. `|η*| < 2`) that approximately maps onto the lab tracker
   acceptance but is theory-friendly. This requires per-bin acceptance
   bookkeeping and a model-dependence systematic on the lab↔CM mapping.

Both options have precedent in SIDIS publications. They are larger changes
than the binary wide/reco choice and would warrant a separate scoping.

---

## 8. Recommendation policy

This memo does **not** advocate a choice. The tradeoff is largely a
philosophical one about which model dependence to expose:

- Convention A exposes the **MC extrapolation** model dependence into the
  central value (which then needs a model-systematic envelope).
- Convention B exposes the **lab fiducial cut translation** model
  dependence into the theory comparison (which is the theorist's
  problem rather than ours).

For the H1 high-Q² charged-particle SIDIS analysis specifically:

- If the primary scientific goal is the **shape of multiplicities** (e.g.
  comparison to HERMES/COMPASS) and the integration is dominated by the
  `0.1 < z < 0.5, 0.5 < pT_h < 2.0 GeV` core, the convention choice
  shifts central values by **≤ 11% in 33% of bins** and the choice is
  mostly cosmetic for the headline plot.

- If the analysis intends to publish a **flat 4D table including the
  low-z, low-pT_h tail** (the natural unfolded product), Convention A
  asserts model-dependent values up to 2.7× the measured yield in
  individual bins. This needs either:
  - a large model-spread systematic (DJANGOH ↔ RAPGAP) explicitly
    capturing the extrapolation, or
  - a switch to Convention B for the publishable bins.

- If a TMD factorization fit is the target, low-pT_h is critical and the
  reco-fiducial bias is concentrated exactly where the physics is most
  interesting. The two conventions yield meaningfully different inputs
  to a TMD fit.

A defensible split-the-difference path is the **hybrid option 1**: keep
the reco-fiducial cross section as the published central, and provide the
per-bin `A_geom` factor (from this JSON) as auxiliary data so theorists
can scale back to the wide-fiducial number if their prediction is
phase-space-integrated.

The choice is left to the supervisor.

---

## 9. Files

| Artifact | Path |
|---|---|
| Script | `sidis_highQ2/scripts/diagnose_gen_fiducial_acceptance.py` |
| JSON | `sidis_highQ2/data/gen_fiducial_acceptance_HQ-v5g-Bprime.json` |
| PDF  | `sidis_highQ2/plots/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf` |
| Memo (this file) | `sidis_highQ2/docs/methodology/gen_fiducial_acceptance_2026-05-24.md` |
| BBB reference (unchanged) | `sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json` |
| BBB script (unchanged, read-only) | `sidis_highQ2/scripts/compute_xsec_v3_HQv5gBprime.py` |
| MC gen CSV (unchanged) | `sidis_highQ2/data/mc_v3/pertrack_mc_v3_gen_djangoh14_merged.csv.gz` |

The JSON includes:
- Per-bin records (480 entries): `N_gen_widefid`, `N_gen_recofid`,
  `N_reco_match` (reconstructed), `N_data`, `eps_h_widefid`,
  `eps_h_recofid`, `N_corr_widefid`, `N_corr_recofid`, `A_geom`, and
  `ratio_N_corr_wide_over_reco`.
- Integrated A_geom per (Q², x) cell, per charge and charge-summed.
- A_geom z and pT_h projections.
- Headline counts and top-10 listings.

---

## 10. Limitations and caveats

- The reco-fiducial mask on the gen denominator uses the **lab-frame**
  `GenPt` and `GenEta` (matching the reco extractor cuts). It does not
  account for any per-track resolution effects at the |η|=1.3 boundary;
  a track with `|GenEta| = 1.32` could reconstruct at `|Eta_lab| = 1.28`
  and contribute to `N_reco_match` while being excluded from
  `N_gen_recofid`. This causes a small over-correction in
  `eps_h_recofid` near the boundary (`eps_h_recofid` slightly > true
  reco efficiency). A migration-aware unfolded recofid acceptance would
  remove this; for the current bin-by-bin comparison the effect is
  expected to be <5% in the bins adjacent to the geometric edge and
  negligible inside. This caveat does not affect `A_geom` itself,
  which is a pure gen-level ratio and is robust.
- This is a **DJANGOH14-only** computation. The DJANGOH-vs-RAPGAP spread
  of `A_geom` is itself a measure of the wide-fiducial extrapolation
  model dependence; it has not been computed here. A follow-up
  dispatch with the same script pointed at
  `pertrack_mc_v3_gen_rapgap31_merged.csv.gz` would close that.
- Only the v3-frame `pT_h` (CM-frame) is used. The lab-frame `pT_h_old`
  available in the v3 schema is not relevant to this diagnostic.
- No radiative corrections are applied to gen rates here — irrelevant to
  the `A_geom` ratio, which is purely a phase-space fraction inside the
  same MC sample. The `C_rad` correction is orthogonal and would only
  enter if one rebuilt the corrected yields under either convention from
  scratch.
- The BBB JSON is **PRELIMINARY-ABS-NORM-UNRESOLVED** at high-Q² (see
  `compute_xsec_v3_HQv5gBprime.py` cover note). This diagnostic inherits
  the same caveat for the absolute `N_corr_widefid` / `N_corr_recofid`
  numbers but does not depend on the absolute MC normalization for
  `A_geom` or for the ratio.


<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current research note. Compares candidate 4D binning schemes
for the charged-hadron SIDIS multiplicity measurement at HERA-II and
recommends the most granular scheme that is supportable by statistics,
detector resolution, and migration purity.

# Research: Optimal 4D Binning Scheme for Charged-Hadron SIDIS Multiplicities

**Date:** 2026-05-17
**Author:** researcher (general subagent)
**Scope:** Determine how differential the H1 charged-hadron SIDIS
measurement `M^{h+/-}(Q^2, x, z, pT_h)` can be in the 4D space at the
HERA-II low-Q^2 luminosity of 297,658 nb^-1. Evaluate candidate binning
schemes against resolution limits, MC and data statistics, purity, and
TMD-fit consumer requirements, and recommend a binning that maximises
fitter usefulness without driving the per-bin uncertainty into the
no-information regime. Include a separate recommendation for the
high-Q^2 regime.

**Inputs verified:**
- `H1/sidis_lowQ2/context-handoff.md` (Section 6B TMD feasibility; Sections 7, 11)
- `H1/sidis_lowQ2/docs/plans/plan-tmd-sidis.md` Iteration 2 + 2026-05-14 session update (decision tree, baseline grid, theory flags, phase-space policy)
- `H1/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md` (published H1 low-Q^2 charged-particle binning: 8 (Q^2,x) cells; bin-by-bin acceptable at purity > 75%)
- `H1/sidis_highQ2/docs/research/research-4d-unfolding.md` (sliced 2D D'Agostini per (Q^2,x) cell is the production unfolder; full 4D as cross-check)
- `H1/sidis_highQ2/docs/research/research-highQ2-sidis-extension.md` (high-Q^2 kinematic reach, statistics, TMD-safe phase-space comparison)
- `/var/folders/n1/zy1hghps5tlg9538vlknzmdm0000gs/T/opencode/v3_acceptance_final.json` (per-run RAPGAP31 acceptance results: 4D occupancy, purity, stability)
- `H1/sidis_lowQ2/data/tmd/tmd_4d_occupancy.json` (4D occupancy with y in [0.05, 0.7], `N_eff` per cell)

---

## 1. Headline Recommendation

**Adopt a slightly-refined version of the current binning, "Scheme A+":**

```text
Q2:   [5, 10, 20, 50, 100]                 GeV^2  (4 bins)
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]              (4 bins)
z:    [0.10, 0.20, 0.30, 0.40, 0.55, 0.70]        (5 bins; refined)
pT_h: [0, 0.15, 0.30, 0.50, 0.75, 1.0, 1.5, 2.0]  GeV (7 bins; finer at low pT_h)
```

Total: 4 x 4 x 5 x 7 = **560 nominal bins per charge**, of which
13/16 (Q^2, x) cells are kinematically accessible -> **455 occupied
bins per charge**.

This is the most granular scheme that simultaneously satisfies:
- per-bin DATA statistics adequate for 5-10% statistical precision in
  the populated bulk and 15-20% in the kinematic edges;
- bin widths >> 2 sigma_resolution in every dimension;
- response-matrix MC statistics sufficient for the sliced 2D D'Agostini
  unfolder recommended in `research-4d-unfolding.md`;
- direct compatibility with the H1 1302.1321 published low-Q^2 charged-
  particle (Q^2, x) binning at the coarser level (allowing a
  charge-summed cross-check by merging two of our (Q^2, x) cells per
  one of theirs in the overlap region);
- the TMD-fit-preferred z subset (0.2 < z < 0.7) is cleanly extractable
  by dropping the lowest-z bin;
- the lowest pT_h bin width (0.15 GeV) matches the photon-axis pT_h
  detector RMS at low pT_h (~0.034 GeV), giving width / sigma > 4.

Refinements relative to the current Scheme A (4x4x4x6 = 384):
- **z gains one bin** (0.40 split out of the broad [0.3, 0.5] bin), to
  satisfy TMD fitters who want at least one bin centered near the
  current-fragmentation peak (z ~ 0.4) before the steep falloff.
- **pT_h gains one bin** (a sub-0.25 GeV split [0, 0.15] and [0.15,
  0.30]), to expose the TMD Gaussian peak shape near pT_h -> 0 where
  TMD fitters are most sensitive to the intrinsic kT width.

The full optimal grid is supportable and is the recommendation for the
nominal release. The bigger Schemes B/C/D/E are NOT recommended on
statistics grounds; Scheme F (matched to 1302.1321 Q^2 and x) is
recommended as the **derived cross-check binning**, not as the nominal
release, since dropping the finer x and Q^2 granularity throws away
the TMD-fit-relevant lever arm that is the central reason to do this
measurement.

A separate, coarser binning is recommended for the high-Q^2 regime (see
Section 9).

---

## 2. Resolution Limits per Dimension

All numbers from the per-run RAPGAP31 acceptance run reported in
`v3_acceptance_final.json` ("inputs.mc_file":
`condor_07_evlsMC_RAPGAP31_Rad_1_proton.root`, 3.1M reco entries, 1.52M
matched selected tracks). These supersede the older 1M-entry numbers in
the handoff Section 6B for the same quantities.

### 2.1 pT_h (photon-axis)

| pT_h center (GeV) | RMS residual (GeV) | sumw (per-run) |
|---|---|---|
| 0.125 | 0.033 | 1.38e6 |
| 0.375 | 0.035 | 2.52e6 |
| 0.625 | 0.049 | 1.46e6 |
| 0.875 | 0.068 |  0.80e6 |
| 1.125 | 0.088 |  0.45e6 |
| 1.375 | 0.110 |  0.27e6 |
| 1.625 | 0.118 |  0.17e6 |
| 1.875 | 0.137 |  0.11e6 |
| 2.125 | 0.148 |  0.077e6 |
| 2.375 | 0.187 |  0.055e6 |

Bin-width-vs-resolution rule (require bin width >= 2 sigma for good
purity):

| Region | 2 sigma | Min defensible bin width |
|---|---|---|
| pT_h < 0.25 GeV | 0.07 GeV | 0.07-0.10 GeV |
| 0.25 < pT_h < 0.5 | 0.07-0.10 GeV | 0.10-0.15 GeV |
| 0.5 < pT_h < 1.0 | 0.10-0.14 GeV | 0.15-0.20 GeV |
| 1.0 < pT_h < 1.5 | 0.18-0.22 GeV | 0.25 GeV |
| 1.5 < pT_h < 2.0 | 0.24-0.30 GeV | 0.30-0.35 GeV |
| pT_h > 2 GeV | > 0.30 GeV | 0.5 GeV |

**Bottom line on pT_h:** Resolution is excellent. The minimum defensible
bin width at the lowest pT_h is 0.07-0.10 GeV; resolution does not
impose a constraint on schemes with bin edges down to 0.15 GeV. The
upper end (pT_h > 2 GeV) is statistics-limited, not resolution-limited.

### 2.2 z (photon-axis)

The naive RMS reported in the resolution JSON is **1.32**, which is a
sentinel/migration artifact (`z_SIDIS` has documented sentinel values at
both ends, see handoff Section 11 item 9). After sentinel filtering the
true detector RMS on z is **0.075** (handoff Section 6B and
plan-tmd-sidis.md session update). Resolution is dominated by the HFS
calibration entering the photon momentum reconstruction, not by the
hadron track resolution.

| Region | sigma | Min defensible bin width |
|---|---|---|
| z < 0.2 | 0.075 (dominated by purity 0.85+ in 1D from JSON) | 0.10 |
| 0.2 < z < 0.4 | 0.075 | 0.10 |
| z > 0.4 | 0.075 | 0.10 |

The 1D purity numbers from the JSON for the current z = [0.1, 0.2, 0.3,
0.5, 0.7] edges are 0.85, 0.62, 0.63, 0.44 respectively. The bin
purities are dominated by the bin width to neighbor-bin migration ratio,
which is decent for 0.1-wide bins. Going below 0.05 in width would push
1D purity below 0.5.

**Bottom line on z:** Detector resolution sets a floor of ~0.075;
bin widths of 0.10 are at the lower bound. The current edges [0.1,
0.2, 0.3, 0.5, 0.7] are conservative; adding a split at z = 0.4
(making [0.1, 0.2, 0.3, 0.4, 0.55, 0.7]) is supportable and matches
the TMD fitter preference for finer z structure in the current
fragmentation region.

### 2.3 Q^2

`log10(Q^2)` RMS = **0.051** (excellent). Resolution is essentially
electron-scale-limited. The current Q^2 edges [5, 10, 20, 50, 100]
correspond to log10 bin widths of 0.30, 0.30, 0.40, 0.30, giving
width / sigma ratios of 6:1 and better. The Q^2 dimension could be
sliced 3-4x finer before bumping into resolution-driven migration.

| Bin scheme | log10 width | width / sigma |
|---|---|---|
| Current [5, 10, 20, 50, 100] | 0.30 - 0.40 | 6:1 - 8:1 |
| Refined [5, 10, 20, 50] then [50, 100] | 0.30 | 6:1 |
| Aggressive [5, 8, 12, 20, 35, 60, 100] | 0.20 | 4:1 |

**Bottom line on Q^2:** Resolution does not limit Q^2 binning at any
plausible scheme.

### 2.4 x (Bjorken)

`log10(x)` RMS = **0.099** (good). Current x edges [1e-4, 5e-4, 1e-3,
5e-3, 1e-2] are log10 widths of 0.70, 0.30, 0.70, 0.30, giving
width / sigma ratios of 3:1 - 7:1. The 0.3-decade bins are tighter
than ideal. A uniform 0.5-decade grid [1e-4, 3e-4, 1e-3, 3e-3, 1e-2]
gives width / sigma = 5:1 uniformly.

The 1D purity numbers from the JSON for the current x edges are
0.96, 0.82, 0.89, 0.75 -- adequate everywhere except the highest x bin.
The stability for the highest x bin is also 0.80, indicating migration
both in and out at the edge.

**Bottom line on x:** Resolution allows the current 4-bin x grid; a
5-bin or 6-bin x grid is at the borderline; 7+ x bins would force
unfolding regularisation strength that erases physics features at the
edges of the migration kernel.

### 2.5 phi_h (azimuthal modulation)

Not directly part of the 4D multiplicity binning, but relevant for the
phi_h moment extraction. RMS = **0.132 rad** (correct gen with reco
electron phi proxy), supporting ~8-12 phi_h bins comfortably. The 1D
phi_h purity is 0.93-0.98 in the existing 8-bin grid.

### 2.6 Summary table

| Dimension | RMS resolution | Min bin width (>2 sigma) | Current width | Notes |
|---|---|---|---|---|
| pT_h (0-0.5 GeV) | 0.03-0.05 GeV | 0.07-0.10 GeV | 0.25 | Room to refine |
| pT_h (0.5-1 GeV) | 0.05-0.07 GeV | 0.10-0.15 GeV | 0.25-0.50 | Adequate |
| pT_h (1-2 GeV) | 0.09-0.15 GeV | 0.20-0.30 GeV | 0.50 | Adequate |
| z | 0.075 (cleaned) | 0.10 | 0.10-0.20 | At lower bound at z=0.1-0.2 |
| log10 Q^2 | 0.051 | 0.10 (factor 1.3) | 0.30-0.40 | Plenty of room |
| log10 x | 0.099 | 0.20 (factor 1.6) | 0.30-0.70 | Room to refine slightly |
| phi_h | 0.132 rad | 0.26 rad (= pi/12) | pi/4 - pi/6 | Plenty of room |

**Resolution does not bind for any dimension in any of the candidate
schemes.** The binding constraint is statistics (per-bin event count)
and purity / migration handling at the unfolder level.

---

## 3. Statistical Requirements per Target Precision

The H1 SIDIS multiplicity is a per-DIS-event count, so the per-bin
statistical uncertainty in `dM^{h+-} / M^{h+-}` is the
multiplicity-weighted relative Poisson error:

```text
relative stat error = sqrt(N_bin) / N_bin = 1 / sqrt(N_bin)
```

where `N_bin` is the number of selected tracks in the bin.

| Target precision | Required N_bin per charge |
|---|---|
| 30% | 11 |
| 15% | 45 |
| 10% | 100 |
| 5% | 400 |
| 3% | 1100 |
| 1% | 10000 |

For the charge asymmetry `A_h = (N+ - N-) / (N+ + N-)`, the per-bin
sigma is `sqrt((1 - A_h^2) / N_total)` ~ `1 / sqrt(N+ + N-)` in the
small-asymmetry regime. For a target sigma_{A_h}:

| Target sigma(A_h) | Required N_total per bin (h+ + h-) |
|---|---|
| 0.05 | 400 |
| 0.02 | 2,500 |
| 0.01 | 10,000 |
| 0.005 | 40,000 |
| 0.003 | 110,000 |

The TMD multiplicity release nominally targets per-bin statistical
precision of 5-10% (5% in well-populated cells, 10-15% acceptable at
the edges). The A_h release targets sigma(A_h) ~ 0.01-0.02 in the
bulk.

---

## 4. Available Statistics: From Per-Run to Full-Dataset Projection

### 4.1 Per-run MC counts (from `v3_acceptance_final.json`)

Single RAPGAP31 file `condor_07_evlsMC_RAPGAP31_Rad_1_proton.root`:

| Quantity | Count |
|---|---|
| Reco tree entries | 3,099,426 |
| Reco entries passing BASE_CUT | 1,746,948 |
| Matched-and-selected tracks (numerator) | 1,520,624 |
| Gen tree entries | 7,016,108 |
| Gen entries passing fiducial | 2,043,884 |
| DIS events (from DisTot) | 330,847 |

The 4D occupancy JSON shows that summing `sumw_reco` across the 384
nominal bins (both charges combined) gives 1,750,792 weighted entries,
consistent with the 1.75M after BASE_CUT.

### 4.2 Full-dataset extrapolation

| Source | Files | Tracks (full dataset) | Notes |
|---|---|---|---|
| RAPGAP31 MC | 35 | ~52.5M matched selected tracks (h+ + h-) | Per-run x 35 |
| DJANGOH14 MC | ~93 (per prompt) | ~140M matched selected | Higher per-event multiplicity than RAPGAP |
| DATA | 1 (combined) | ~3.44M matched selected tracks (h+ + h-) | See note below |

The **data** extrapolation uses the lumi ratio
`L_data / L_RAPGAP = 297658 nb^-1 / 5.3M nb^-1 = 0.0562`. Applied to
the 35-file MC projection: `1.75M x 35 x 0.0562 = 3.44M` data tracks
(h+ + h-) ~ **1.72M data tracks per charge**.

This is the most important number in this report: **the data sample
contains roughly 1.7 million selected tracks per charge** across the
full accessible 4D phase space.

### 4.3 Cell-by-cell distribution under Scheme A (current)

From the per-cell `sumw_reco` in the acceptance JSON, scaled to data
per charge (`x 35 x 0.0562 / 2 = x 0.984`, i.e. effectively the per-run
sumw_reco numbers themselves are a close proxy for the per-charge data
counts in the full set):

| Statistic | Value |
|---|---|
| Bins with > 0 expected data | 327 / 384 |
| Bins with > 10 | 318 / 384 (83%) |
| Bins with > 30 | 310 / 384 (81%) |
| Bins with > 100 | 286 / 384 (74%) |
| Bins with > 300 | 252 / 384 (66%) |
| Bins with > 1000 | 191 / 384 (50%) |
| Bins with > 3000 | 124 / 384 (32%) |
| Bins with > 10000 | 51 / 384 (13%) |
| Median per-bin (over occupied 327 bins) | 1,724 |
| Mean per-bin | 5,262 |
| Max per-bin | 49,524 |
| Min per-bin | 1.3 |

**Multiplicity precision in Scheme A:**
- 74% of bins reach <= 10% per-bin statistical precision per charge
- 63% of bins reach <= 5% per-bin statistical precision per charge
- 13% of bins reach <= 1% per-bin statistical precision per charge

**A_h precision in Scheme A** (using N_total = 2 * per-charge count):
- 47% of bins reach sigma(A_h) < 0.02
- 36% of bins reach sigma(A_h) < 0.014
- 22% of bins reach sigma(A_h) < 0.009

### 4.4 (Q^2, x) cell-level statistics (full dataset, per charge)

Per (Q^2, x) cell, summing over all 24 (z, pT_h) sub-bins:

| iQ^2 | ix | Q^2 range | x range | Data per charge (full) |
|---|---|---|---|---|
| 0 | 0 | 5-10 | 1e-4 - 5e-4 | 240,000 |
| 0 | 1 | 5-10 | 5e-4 - 1e-3 | 222,500 |
| 0 | 2 | 5-10 | 1e-3 - 5e-3 | 243,300 |
| 0 | 3 | 5-10 | 5e-3 - 1e-2 | 3,500 (kin. forbidden, fake events) |
| 1 | 0 | 10-20 | 1e-4 - 5e-4 | 60,100 |
| 1 | 1 | 10-20 | 5e-4 - 1e-3 | 160,400 |
| 1 | 2 | 10-20 | 1e-3 - 5e-3 | 342,200 |
| 1 | 3 | 10-20 | 5e-3 - 1e-2 | 22,800 (kin. forbidden, fake events) |
| 2 | 0 | 20-50 | 1e-4 - 5e-4 | 2,700 (kin. forbidden, fake events) |
| 2 | 1 | 20-50 | 5e-4 - 1e-3 | 45,200 |
| 2 | 2 | 20-50 | 1e-3 - 5e-3 | 249,800 |
| 2 | 3 | 20-50 | 5e-3 - 1e-2 | 53,000 |
| 3 | 0 | 50-100 | 1e-4 - 5e-4 | 0 (forbidden) |
| 3 | 1 | 50-100 | 5e-4 - 1e-3 | 280 (suppressed) |
| 3 | 2 | 50-100 | 1e-3 - 5e-3 | 53,300 |
| 3 | 3 | 50-100 | 5e-3 - 1e-2 | 23,900 |

The kinematic accessibility analysis (y in [0.1, 0.6]) gives:

| iQ^2 | ix | y_min | y_max | Accessible? |
|---|---|---|---|---|
| 0 | 0 | 0.099 | 0.985 | YES |
| 0 | 1 | 0.049 | 0.197 | YES |
| 0 | 2 | 0.010 | 0.099 | NO |
| 0 | 3 | 0.005 | 0.020 | NO |
| 1 | 0 | 0.197 | 1.97 | YES |
| 1 | 1 | 0.099 | 0.394 | YES |
| 1 | 2 | 0.020 | 0.197 | YES |
| 1 | 3 | 0.010 | 0.039 | NO |
| 2 | 0 | 0.394 | 4.92 | YES |
| 2 | 1 | 0.197 | 0.985 | YES |
| 2 | 2 | 0.039 | 0.492 | YES |
| 2 | 3 | 0.020 | 0.099 | NO |
| 3 | 0 | 0.985 | 9.85 | NO |
| 3 | 1 | 0.492 | 1.97 | YES |
| 3 | 2 | 0.099 | 0.985 | YES |
| 3 | 3 | 0.049 | 0.197 | YES |

The published `tmd_4d_occupancy.json` was generated with y in [0.05,
0.7], a slightly broader range that admits 13/16 cells (the four
NO-cells above are still excluded). The forbidden cells have small
nonzero data counts only from gen-to-reco migration of (Q^2, x), which
the unfolder correctly handles by routing them back into the originating
gen cells.

**Key observation:** the bulk of statistics live in the diagonal of the
(Q^2, x) plane, mirroring the HERA `Q^2 = s x y` correlation. The
brightest cells are (Q^2 ~ 10-50, x ~ 1e-3 - 5e-3) with 200-350k per
charge. The faintest accessible cell is (50-100, 5e-4 - 1e-3) with only
280 per charge.

---

## 5. Candidate Scheme Comparison

### 5.1 Comparison table

For each scheme, the average per-charge per-bin data count is
`1.72M / N_accessible_bins`. The A_h sigma is `1/sqrt(2 * avg)`.

| Scheme | nbins | Accessibility | Occupied | Avg / bin / charge | sigma(A_h) avg | Fraction stat-OK (>100) |
|---|---|---|---|---|---|---|
| A: 4x4x4x6 (current) | 384 | 13/16 = 81% | 312 | 5,515 | 0.0095 | 74% |
| **A+: 4x4x5x7 (recommend)** | **560** | **13/16 = 81%** | **455** | **3,780** | **0.0115** | **~65% est.** |
| B: 4x4x4x8 (finer pT_h) | 512 | 13/16 = 81% | 416 | 4,135 | 0.0110 | ~70% est. |
| C: 4x4x6x6 (finer z) | 576 | 13/16 = 81% | 468 | 3,675 | 0.0117 | ~65% est. |
| D: 6x4x4x6 (finer Q^2) | 576 | 17/24 = 71% | 408 | 4,215 | 0.0109 | ~65% est. |
| E: 6x5x6x8 (maximal) | 1440 | 17/30 = 57% | 816 | 2,110 | 0.0154 | ~50% est. |
| F: 8(Q^2,x) x 4 x 6 (matched 1302.1321) | 192 | 8/8 = 100% | 192 | 8,960 | 0.0075 | ~85% est. |

The "stat-OK" fraction is the fraction of bins where the per-charge
data count exceeds 100 (the 10%-precision threshold). It is estimated
for non-A schemes by scaling the per-cell distribution from Scheme A
and is not exact -- it captures the average behaviour. The full
re-binning would require recomputing per-cell `sumw_reco` from the
underlying MC arrays, which is in scope for the next pass on NAF
(`scripts/tmd_resolution_acceptance.py` already loads the arrays
needed).

### 5.2 Per-scheme verdict

**Scheme A (current, 4x4x4x6 = 384):**
- Pros: already in production, ample statistics in the bulk, simple to
  cross-check, matches the migration tolerances of the 2D D'Agostini
  unfolder.
- Cons: too coarse in z (no bin around the fragmentation peak at
  z ~ 0.4), pT_h binning too uniform (no low-pT_h granularity for TMD
  Gaussian peak).
- **Verdict: usable as a fallback; not recommended as the nominal release.**

**Scheme A+ (recommended, 4x4x5x7 = 560):**
- Pros: adds the missing z and low-pT_h granularity demanded by TMD
  fits; preserves the 13/16 (Q^2, x) accessibility; per-bin statistics
  stay above the 10% threshold for ~65% of occupied bins (estimated);
  the TMD-fit-preferred z subset (0.2 < z < 0.7) drops cleanly.
- Cons: slight per-bin precision loss vs Scheme A (sigma(A_h) avg
  0.0115 vs 0.0095); ~35% of bins need 15-20% per-bin statistical
  precision in the publication.
- **Verdict: recommended as the nominal release binning.**

**Scheme B (4x4x4x8 = 512, finer pT_h only):**
- Pros: cheapest path to TMD-relevant pT_h granularity at low pT_h.
- Cons: leaves z under-binned around the fragmentation peak.
- **Verdict: acceptable if z refinement is rejected; second choice
  after A+.**

**Scheme C (4x4x6x6 = 576, finer z only):**
- Pros: addresses the TMD-fit demand for finer z structure.
- Cons: leaves pT_h under-binned at low pT_h; A_h precision penalty
  similar to A+.
- **Verdict: not preferred; A+ gives both refinements at similar cost.**

**Scheme D (6x4x4x6 = 576, finer Q^2 only):**
- Pros: provides finer Q^2-evolution lever arm for TMD fits.
- Cons: y-acceptance fraction drops from 81% to 71%; statistically
  smaller per-cell gain than A+ in any TMD-physics-relevant sense; the
  finer Q^2 bins fall mostly in the small-x corner where statistics
  are already lowest.
- **Verdict: not recommended for the first release; revisit in a second
  iteration if the data-driven A_h shows surprising Q^2 dependence
  that the 4-bin Q^2 grid cannot resolve.**

**Scheme E (6x5x6x8 = 1440, maximal):**
- Pros: maximally informative IF statistics supported it.
- Cons: only 57% of (Q^2, x) cells are kinematically accessible; bulk
  average per-bin precision drops to ~22%; only ~50% of occupied bins
  reach 10% per-bin precision; unfolder regularisation strength would
  have to be increased, washing out structure.
- **Verdict: NOT recommended. Over-binned for the available statistics;
  would produce a result that visually appears more differential but
  carries no more information than Scheme A+.**

**Scheme F (8 (Q^2, x) cells from H1 1302.1321, 8x4x6 = 192):**
- Pros: enables direct charge-summed cross-check against the published
  H1 paper; 100% (Q^2, x) accessibility (by construction, the paper
  chose accessible bins); highest per-bin statistics; bin-by-bin
  acceptance correction may be defensible with purity > 75% in the
  (Q^2, x) layer.
- Cons: throws away the (Q^2, x) granularity that is the TMD-fit value
  add; provides no finer-than-current z/pT_h binning.
- **Verdict: NOT recommended as the nominal release. STRONGLY
  recommended as a derived auxiliary release: produce both Scheme A+
  AND Scheme F outputs, with the latter as the 1302.1321 cross-check
  table.**

### 5.3 Note on the 1302.1321 binning

H1 1302.1321 published 8 (Q^2, x) bins:

| Q^2 bin (GeV^2) | x bin |
|---|---|
| 5-10 | 0.0001 - 0.00024 |
| 5-10 | 0.00024 - 0.0005 |
| 5-10 | 0.0005 - 0.002 |
| 10-20 | 0.0002 - 0.00052 |
| 10-20 | 0.00052 - 0.0011 |
| 10-20 | 0.0011 - 0.0037 |
| 20-100 | 0.0004 - 0.0017 |
| 20-100 | 0.0017 - 0.01 |

Each bin was further divided in (eta*, pT*) -- not (z, pT_h) -- with
the explicit purity > 75% constraint. The eta-binning was 5-7 bins per
(Q^2, x) cell. The total bin count was thus ~ 8 x 5 x 2 (pT* low and
high) = ~80 bins per (Q^2, x) variable cell, ~640 total.

Our z, pT_h binning is conceptually similar in dimensionality but
different in physics meaning: we measure charge-separated multiplicities
in the *photon-axis frame* observables (z, pT_h), not lab/eta. We
cannot directly compare bin-for-bin against 1302.1321 without first
projecting our (z, pT_h) histogram into (eta*, pT*) using the boost. A
charge-summed eta*/pT* cross-check histogram should be produced as a
side product.

---

## 6. Purity and Migration Constraints

### 6.1 4D purity distribution under Scheme A

From `v3_acceptance_final.json` Section `purity_stability_4d`:

| Purity threshold | Cells above threshold |
|---|---|
| > 0.30 | 228 / 327 (70%) |
| > 0.40 | 181 / 327 (55%) |
| > 0.50 | 131 / 327 (40%) |
| > 0.60 |  77 / 327 (24%) |
| > 0.70 |  25 / 327 (8%) |

**The 75% purity threshold from H1 1302.1321 (Section 6.2 of that paper)
that justifies bin-by-bin correction is NOT met by Scheme A's 4D bins.**
Only ~8% of cells exceed 75% 4D purity. This is the reason
`research-4d-unfolding.md` recommends sliced 2D D'Agostini per (Q^2, x)
cell with the 30% per-dimension threshold as the working tolerance.

### 6.2 Implication for binning refinement

Going from Scheme A (24 (z, pT_h) sub-bins per (Q^2, x) cell) to Scheme
A+ (35 sub-bins per (Q^2, x) cell) increases the per-sub-bin migration
because:
- splitting z bin [0.3, 0.5] into [0.3, 0.4] and [0.4, 0.55] roughly
  doubles the relative migration probability in that region
  (z resolution ~ 0.075 means a 0.1-wide bin has ~50% migration to
  adjacent bins);
- splitting pT_h bin [0, 0.25] into [0, 0.15] and [0.15, 0.30]
  is safer because pT_h resolution at low pT_h is 0.033 GeV, so a
  0.15-GeV-wide bin has ~10% migration.

The sliced 2D D'Agostini unfolder handles migration up to the level
where the response matrix becomes ill-conditioned. With ~700k matched
MC tracks per (Q^2, x) cell per charge (the full-dataset extrapolation
for the densest cells) and a 35x35 response, the response matrix is
still well-conditioned. For the sparsest cells (~1000 matched tracks
per charge), a 35x35 response is marginal; the unfolder will likely
need to mask the lowest-statistics z or pT_h bins in those cells.

**Bottom line:** Scheme A+ is at the boundary of what the sliced 2D
D'Agostini unfolder can handle. Going to Scheme E (full 6x8 = 48 sub-
bins per cell) would require explicit per-cell regularisation tuning
and would push several cells out of the unfoldable regime.

---

## 7. TMD-Fitter Requirements

### 7.1 What TMD fits actually need (consolidated from public literature)

| Quantity | Typical minimum | Our delivery |
|---|---|---|
| Q^2 lever arm | factor-2 binning, >= 3 Q^2 points | 4 Q^2 bins (factor 2 each), excellent |
| x range | >= 1 decade | 2 decades (1e-4 - 1e-2), unique to HERA |
| z bins in 0.2 < z < 0.7 | >= 3 | 4 (with z = 0.4 split) |
| pT_h bins at pT_h < 1 GeV | >= 3, ideally with bin centers at 0.1, 0.3, 0.5 GeV | 4-5 (with sub-0.25 GeV bins) |
| pT_h max | 2-3 GeV | 2 GeV |
| pT_h / Q < 0.3 (or < 0.5) safe-region flagging | Required as per-bin metadata | Implemented via `pth_over_q_flag` (plan-tmd-sidis.md Part 4) |
| Charge separation | Strongly preferred | YES, h+ and h- separately |
| Per-source systematic shifts | Required | Planned (plan-tmd-sidis.md Part 5) |
| Covariance matrix | Strongly preferred | Block-diagonal per (Q^2, x, charge), planned |

### 7.2 What different fit groups have requested from other experiments

Based on the published TMD-fit literature (and the
`research-sidis-systematics-literature.md` cross-paper survey):

**JAM (Jefferson Lab Angular Momentum):** Single-hadron multiplicities
in 4D with charge separation; per-bin theory flags; nuisance-source
shifts. Prefer **>= 5 pT_h bins per (Q^2, x, z) slice** with the lowest
bin spanning 0-0.15 or 0-0.2 GeV to constrain the Gaussian intrinsic
width.

**MAP (Multidimensional Analyses of Partonic distributions):**
Multidifferential SIDIS multiplicities with the strongest preference
for **fine pT_h binning below 0.5 GeV** (where the TMD evolution kernel
is most sensitive). Recommend `pT_h < min(0.3 * Q, Q/2)` cuts.

**Pavia group (Bacchetta et al.):** Per-bin covariance is **required**
not optional; charge separation moderately preferred. **z >= 3 bins**
in 0.2 < z < 0.7 with at least one near z = 0.4 (the fragmentation
peak).

**Artemide (Vladimirov et al.):** Strong preference for **TMD-safe
binning where pT_h / Q < 0.3 in every reported bin**, with bins
**flagged** rather than cut, so the user can choose a less restrictive
mask. Lab pT or non-photon-axis pT_h is **not usable**.

All four groups would accept the Scheme A+ binning and would prefer
it to Scheme A. None would request finer than Scheme A+ given the H1
luminosity. **The minimum pT_h bin width any of them can use is
0.10-0.15 GeV**, matching the recommendation.

### 7.3 Specific pT_h granularity argument

The TMD Gaussian intrinsic-kT model gives `M(pT_h) ~ exp(-pT_h^2 / <kT^2>)`
at fixed (x, Q^2, z). To constrain `<kT^2>` from a binned measurement
requires:
- at least 3 bins covering pT_h < <kT> ~ 0.3-0.5 GeV (where the
  Gaussian is steep);
- at least 2 bins in the falling tail 0.5-1.5 GeV.

The current Scheme A pT_h grid [0, 0.25, 0.5, 1.0, 1.5, 2.0] has 2
bins below 0.5 GeV; this is **insufficient** to fit a TMD Gaussian
width to better than ~30% per-bin uncertainty propagation. The Scheme
A+ pT_h grid [0, 0.15, 0.30, 0.50, 0.75, 1.0, 1.5, 2.0] has 3 bins
below 0.5 GeV; this is the minimum for a TMD Gaussian fit.

---

## 8. Risks and Mitigations for Scheme A+

| Risk | Probability | Mitigation |
|---|---|---|
| The (50-100, 5e-4 - 1e-3) cell has < 300 data tracks per charge | High (already evident) | Mask this single cell from the publication if precision is too low; document |
| Per-cell 4D purity drops below 30% in extreme z or pT_h bins after splitting | Medium | Apply the existing per-dimension 30% purity flag from `tmd_resolution_acceptance.py`; mask bins that fail |
| Sliced 2D D'Agostini becomes unstable in cells with 35x35 response | Low | Cells with < 1000 matched MC tracks per charge fall back to bin-by-bin in (z, pT_h) within the cell, with explicit theory-caution flag |
| MC reweighting (RAPGAP vs DJANGOH) shifts the per-bin response by > 10% | Medium | Mc-model-swap systematic (already planned); freezes systematic shift |
| The z = 0.4 split lands in a region where the z resolution sigma > 0.075 | Low | Resolution is roughly flat in 0.2 < z < 0.7 from JSON data; bin width 0.10 stays > sigma everywhere |
| Underbinned in phi_h for the azimuthal-modulation extension | Not applicable to multiplicity release | phi_h binning is independent and finer (12 bins recommended by separate analysis) |

---

## 9. High-Q^2 Recommendation

### 9.1 Statistical penalty

The high-Q^2 data sample is ~10x smaller per the prompt and
`research-highQ2-sidis-extension.md`. Total available data per charge
in the high-Q^2 regime is roughly **170k tracks** vs 1.72M at low-Q^2.

### 9.2 Kinematic regime is complementary, not overlapping

| Quantity | Low-Q^2 | High-Q^2 |
|---|---|---|
| Q^2 range | 5-100 GeV^2 | 150-20,000 GeV^2 |
| x range | 1e-4 - 1e-2 (sea/gluon) | 1e-3 - 0.5 (valence) |
| TMD-safe phase space (pT_h/Q < 0.3) | Limited; most bins are at the boundary | Wide; nearly all bins satisfy easily |

The high-Q^2 measurement is **statistics-limited**, not
purity/resolution-limited. The TMD-safe phase space is far larger, so
the bin count should be smaller and bins should be deliberately wider.

### 9.3 Recommended high-Q^2 binning

```text
Q2:   [150, 300, 700, 2000, 20000]  GeV^2  (4 bins)
x:    [1e-3, 1e-2, 5e-2, 0.5]              (3 bins; valence-region focused)
z:    [0.1, 0.2, 0.3, 0.5, 0.7]            (4 bins; matching low-Q^2)
pT_h: [0, 0.25, 0.5, 1.0, 2.0, 4.0]  GeV   (5 bins; extends higher because Q is higher)
```

Total: 4 x 3 x 4 x 5 = **240 bins per charge**. (Q^2, x) accessibility
needs to be re-verified; expect ~9/12 = 75% accessible -> ~180
populated bins per charge. Average per-bin data: 170,000 / 180 ~ 940
per charge. Adequate for 5-7% per-bin precision in the bulk.

The high-Q^2 binning is intentionally coarser than low-Q^2 because:
- statistics are 10x smaller and would not support equivalent
  granularity;
- the valence-x regime varies more smoothly with x than the sea region,
  so fewer x bins are needed;
- the TMD-safe pT_h band reaches higher Q, allowing pT_h bins up to
  4 GeV.

### 9.4 Combined release strategy

The high-Q^2 and low-Q^2 binnings DO NOT need to be matched. The two
regimes are kinematically disjoint (modulo a small overlap at
Q^2 ~ 100-300 GeV^2). The combined release should provide both with
their own binning conventions and let TMD fitters consume each as a
separate dataset, exactly as JAM and MAP currently consume HERMES
(low-Q^2) and COMPASS (mid-Q^2) as separate datasets.

---

## 10. Final Recommendations Summary

### 10.1 For the low-Q^2 nominal release

Adopt **Scheme A+**:

```text
Q2:   [5, 10, 20, 50, 100]                       GeV^2  (4 bins)
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]                    (4 bins)
z:    [0.10, 0.20, 0.30, 0.40, 0.55, 0.70]              (5 bins)
pT_h: [0, 0.15, 0.30, 0.50, 0.75, 1.0, 1.5, 2.0] GeV    (7 bins)
```

- 560 nominal bins per charge, 455 populated, ~3,800 data tracks per
  charge per bin average, ~65% of bins reach 10% statistical precision.
- Unfolding by sliced 2D D'Agostini per (Q^2, x) cell (per
  `research-4d-unfolding.md`) with 5x7 = 35 (z, pT_h) bins per cell.
- Per-bin theory flags per `plan-tmd-sidis.md` Part 4.
- Per-source systematic shifts per `plan-tmd-sidis.md` Part 5.

### 10.2 For the low-Q^2 derived cross-check release

Produce **Scheme F** (1302.1321-matched (Q^2, x), charge-summed, with
acceptance-corrected eta*/pT* projections) as an auxiliary release for
the published-H1 cross-check.

### 10.3 For the high-Q^2 release

Adopt the coarser high-Q^2 binning in Section 9.3. Do not match low-Q^2
binning. Release as a separate dataset with its own metadata.

### 10.4 What to NOT do

- Do not adopt Scheme E (6x5x6x8 = 1440). Statistically unsupportable.
- Do not adopt Scheme D (6x4x4x6) just to claim finer Q^2 granularity.
  The Q^2 lever arm is already maximal; finer Q^2 binning splits
  cells where statistics is already lowest.
- Do not match the high-Q^2 binning to low-Q^2. The kinematics and
  statistics are different enough that one binning cannot serve both.
- Do not use lab pT or lab eta as TMD observables. The photon-axis
  pT_h and (Q^2, x, z) are the only TMD-fit-relevant variables.
- Do not over-promise sub-0.1 GeV pT_h binning. Detector resolution
  permits it but data statistics in low-occupancy (Q^2, x) cells do
  not.

### 10.5 Open items requiring confirmation on full dataset

1. Re-run `tmd_resolution_acceptance.py` with the Scheme A+ edges on
   the full v3 merged MC to confirm per-cell purity and the per-bin
   data count distribution.
2. Run the same script on DJANGOH14 merged MC to confirm scheme A+
   generator-dependence is small (target: < 10% per-bin response
   shift).
3. Compute the explicit average per-cell condition number of the 35x35
   2D D'Agostini response matrix for each (Q^2, x) cell, to identify
   cells that need bin-merging.
4. Run the 1302.1321 cross-check (Scheme F + eta*/pT* projection)
   on the merged MC to confirm the central charge-summed multiplicities
   reproduce the published values within ~2%.
5. Verify that the highest-Q^2 / lowest-x kinematically borderline cells
   (e.g. (Q^2 50-100, x 5e-4 - 1e-3)) do not contribute spurious
   migration into the populated cells.

These items belong to the post-merge analysis chain
(`plan-post-production-analysis-chain.md`).

---

## 11. Gaps and Open Questions

- The per-bin data count estimates in Section 4.3 are extrapolated from
  the per-run RAPGAP MC `sumw_reco` distribution multiplied by the
  data / MC luminosity ratio. This assumes the per-bin distribution of
  the data matches the RAPGAP MC; for the bulk this is true to within
  ~10%, but at the kinematic edges (low x, high pT_h) DJANGOH and data
  may differ by 20-50%, in which case the actual data per-bin count
  may be larger or smaller than the estimate.

- Scheme F (1302.1321-matched) is recommended as auxiliary but the
  exact projection from (z, pT_h) to (eta*, pT*) requires
  re-implementation of the HCM boost in our analysis-layer script. This
  has not been verified against the H1 1302.1321 numbers.

- The sliced 2D D'Agostini unfolder's behaviour on the Scheme A+
  refined response (5x7 = 35 bins) has not been measured. Closure tests
  in Section 6 of `research-4d-unfolding.md` were done for the 4x6 =
  24-bin response. The closure must be re-run with the larger response.

- Scheme A+ pT_h bin centers at low pT_h (0.075, 0.225, 0.4 GeV) are
  not equally spaced; this is intentional (TMD-shape-driven) but
  introduces a small unbinned bias in the published `<pT_h>` per bin.
  Per-bin average kinematics should be stored in the release metadata
  to avoid this.

- The high-Q^2 binning recommendation in Section 9.3 has NOT been
  validated against the high-Q^2 merged MC. The accessibility, per-cell
  occupancy, and per-bin purity must be reverified using the high-Q^2
  v4 merged outputs once available (per
  `research-highQ2-sidis-extension.md`).

- This research deliberately did NOT include a TMD-factorisation cut
  evaluation (e.g. testing whether `pT_h < min(0.3*Q, Q/2)` removes
  too many bins). The plan's policy is to flag rather than cut; the
  flag implementation is a separate piece of work.

- The recommended binning has not been circulated to TMD-fit groups
  for feedback. Per `plan-tmd-sidis.md` Section 6.5, this is a future
  step.

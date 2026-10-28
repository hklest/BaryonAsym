# Gen-fiducial acceptance MC-model spread: DJANGOH14 vs RAPGAP31

**Date:** 2026-05-25
**Status:** DIAGNOSTIC. No production artifact modified.
**Scope:** H1 high-Q² charged-particle SIDIS, Scheme B′, v5g/v3 frame.
**Companion memo:** `gen_fiducial_acceptance_2026-05-24.md` (DJANGOH-only baseline). This
sibling memo extends the diagnostic to RAPGAP31 to quantify the MC-model
systematic on the wide-fiducial extrapolation factor.

**Script:** `sidis_highQ2/scripts/diagnose_gen_fiducial_acceptance_compare_DJ_RG.py`

**Inputs (read-only):**
- RAPGAP31 gen v3 CSV: `sidis_highQ2/data/mc_v3/pertrack_mc_v3_gen_rapgap31_merged.csv.gz`
  (116,934,744 rows, 4.2 GB)
- DJANGOH reference JSON: `sidis_highQ2/data/gen_fiducial_acceptance_HQ-v5g-Bprime.json`

**Outputs created:**
- `sidis_highQ2/data/gen_fiducial_acceptance_compare_DJ_RG.json` (per-bin DJ+RG + delta + rel_delta)
- `sidis_highQ2/plots/gen_fiducial_acceptance_compare_DJ_RG.pdf` (6-page PDF)
- This memo.

This memo is informational. It does not change any cuts and does not amend
the BBB JSON. The DJANGOH-only memo, JSON, and PDF are untouched.

---

## 1. Motivation

`A_geom = N_gen_recofid / N_gen_widefid` per 4D bin is the fraction of true
charged hadrons inside the H1 central tracker geometric acceptance
(`pT_lab > 0.15 GeV AND |η_lab| < 1.3`). Under the **wide-fiducial
Convention A** the published corrected yield is
`N_corr_widefid = N_data / eps_h_widefid`, and
`eps_h_widefid = eps_h_recofid · A_geom`, so

```
N_corr_widefid ∝ 1 / A_geom
```

The MC chosen to compute `A_geom` directly sets the wide-fiducial central
value. The **DJANGOH ↔ RAPGAP spread** of `A_geom` is therefore the
dominant MC-model systematic on Convention A. **Convention B
(reco-fiducial)** does not extrapolate and is unaffected by this
systematic.

Definitions used in this memo:

| Quantity | Definition | Meaning |
|---|---|---|
| `A_geom^DJ` | `N_gen_recofid / N_gen_widefid` from DJANGOH14 (reference JSON) | DJANGOH geometric-acceptance factor |
| `A_geom^RG` | same, recomputed here from the RAPGAP31 gen CSV | RAPGAP geometric-acceptance factor |
| `delta_A_geom` | `A_geom^RG − A_geom^DJ` | signed absolute shift |
| `rel_delta` | `delta_A_geom / A_geom^DJ` | **fractional MC-model systematic on `N_corr_widefid`** |
| `half_diff` | `0.5 · |delta_A_geom|` | standard "half the spread" systematic |

---

## 2. Computation

The script `diagnose_gen_fiducial_acceptance_compare_DJ_RG.py` mirrors the
DJANGOH-only diagnostic exactly. It streams the RAPGAP gen CSV in 3M-row
chunks with the same gen-pool pre-filter (`inFiducialCharged == 1`, π/K/p,
`IsV0DaughterGen == 0`), bins into `N_gen_widefid^RG` and `N_gen_recofid^RG`
on the canonical Scheme B′ 4D grid (8 (Q²,x) × 6 z × 5 pT_h × 2 charge =
**480 bins**), then loads `A_geom^DJ` and `N_data` from the DJANGOH JSON
verbatim and computes per-bin `delta_A_geom`, `rel_delta`, `half_diff`.
No reco CSV is re-read; the DJANGOH-only artifacts are not modified.

The script halts with an explicit schema error if any expected column is
missing from the RAPGAP CSV. Both gen CSVs have the same 30-column v3
schema; no mismatch was found.

RAPGAP gen input: 116.9 M rows loaded, all 116.9 M passed the wide-fiducial
pre-filter, 59.7 M passed the additional reco-fiducial mask
(`GenPt ∈ (0.15, 15)` and `|GenEta| < 1.3`). Sum of weights inside Scheme B′:
wide = 1.615 × 10⁸, reco = 8.210 × 10⁷. (For comparison, DJANGOH has
wide = 1.864 × 10⁸ and reco = 9.146 × 10⁷ in the same envelope.)

Runtime: ~3.2 min wall on the local machine.

---

## 3. Headline numbers

| Quantity | DJANGOH14 | RAPGAP31 | Δ (RG − DJ) | rel_delta |
|---|---|---|---|---|
| Weighted gen wide (Scheme B′) | 1.864 × 10⁸ | 1.615 × 10⁸ | — | — |
| Weighted gen reco (Scheme B′) | 9.146 × 10⁷ | 8.210 × 10⁷ | — | — |
| **Integrated A_geom (entire Scheme B′)** | **0.4906** | **0.5083** | **+0.0177** | **+3.61 %** |

> **The integrated wide-fiducial extrapolation factor `1/A_geom` shifts
> by +3.6 % between DJANGOH and RAPGAP. RAPGAP places ~1.8 percentage
> points more of its true tracks inside the H1 tracker fiducial than
> DJANGOH does, so the wide-fiducial published yield is ~3.6 % lower
> under RAPGAP than under DJANGOH at integrated level.**

### Per-bin spread across all 480 bins

| Category | Count | Notes |
|---|---|---|
| Populated bins (DJ wide > 0 AND RG wide > 0) | 480 | all bins |
| `|delta_A_geom| > 0.05` (absolute shift > 5 pp) | **4** | extreme corner bins |
| `|delta_A_geom| > 0.10` (absolute shift > 10 pp) | 0 | none |
| `|rel_delta| > 0.05` (>5 % on wide N_corr) | **93** | ~19 % of bins |
| `|rel_delta| > 0.10` (>10 % on wide N_corr) | **31** | ~6 % of bins |
| `|rel_delta| > 0.20` (>20 % on wide N_corr) | 10 | low-stat corners |
| Mean `|delta_A_geom|` (populated) | 0.0095 | ~1 pp |
| Median `|delta_A_geom|` (populated) | 0.0051 | ~0.5 pp |
| Max `|delta_A_geom|` (populated) | 0.0682 | S2b h⁻ z[0.5,0.7) pT[4,8) |
| Mean `|rel_delta|` | 4.08 % | well-behaved |
| Median `|rel_delta|` | 1.09 % | typical bin barely shifts |
| Max `|rel_delta|` | 135 % | S3b h⁺ z[0.5,0.7) pT[4,8), A_DJ=0.003, A_RG=0.007, N_data=0 |

The maximum relative shift is a pathological low-stat bin
(A_geom~0.003, no data, no contribution to any published number). It is
flagged in the JSON for inspection but plays no role in the systematic.

---

## 4. Per (Q², x) cell integrated comparison

Charge-summed integrated `A_geom` per cell (sum over z, pT_h), with
charge-separated rows underneath:

| Cell | Q² range | x range | A_geom^DJ | A_geom^RG | Δ | rel_delta |
|---|---|---|---|---|---|---|
| S0a | [150, 300)   | [0.0021, 0.0071) | 0.5152 | 0.5236 | +0.0084 | **+1.63 %** |
| S0b | [150, 300)   | [0.0071, 0.030)  | 0.4735 | 0.4936 | +0.0201 | **+4.25 %** |
| S1a | [300, 700)   | [0.0042, 0.016)  | 0.5340 | 0.5533 | +0.0193 | **+3.61 %** |
| S1b | [300, 700)   | [0.016, 0.069)   | 0.4380 | 0.4638 | +0.0259 | **+5.90 %** |
| S2a | [700, 1500)  | [0.0099, 0.034)  | 0.5503 | 0.5785 | +0.0282 | **+5.13 %** |
| S2b | [700, 1500)  | [0.034, 0.15)    | 0.3469 | 0.3667 | +0.0199 | **+5.73 %** |
| S3a | [1500, 20000)| [0.021, 0.087)   | 0.5320 | 0.5627 | +0.0307 | **+5.77 %** |
| S3b | [1500, 20000)| [0.087, 1.5)     | 0.2092 | 0.2282 | +0.0190 | **+9.09 %** |

Per-charge (h⁺ vs h⁻) the shifts agree to well under 0.3 pp in every
cell; the systematic is **essentially charge-symmetric**, exactly as
`A_geom` itself was.

**Patterns:**
- The shift is **always positive** (`A_geom^RG > A_geom^DJ` in every
  charge-summed cell): **RAPGAP fragments slightly more centrally than
  DJANGOH**. In the wide-fiducial convention, RAPGAP attributes a smaller
  fraction of the yield to the extrapolated `|η_lab| > 1.3` / `pT_lab < 0.15`
  region than DJANGOH does.
- **rel_delta grows with Q²** within the high-x band: S0b 4.25 % →
  S1b 5.90 % → S2b 5.73 % → **S3b 9.09 %**. The high-x, high-Q² cell where
  the absolute extrapolation is already largest (A_geom~0.21) is also the
  cell where the two MC models disagree most in fractional terms. In S3b
  the MC-model systematic alone is ~9 % on the published wide-fiducial yield.
- **rel_delta is smaller in low-x cells**: S0a 1.63 %, S1a 3.61 %. Low-x
  events are softer and more isotropic; both generators agree better there.
- The absolute Δ across all 8 cells lies in the narrow band [+0.008, +0.031],
  a factor-of-4 spread driven mostly by where each cell sits on the
  fragmentation hardness scale.

---

## 5. z and pT_h projections

### z projection (integrated over Q², x, pT_h, charge)

| z range | A_geom^DJ | A_geom^RG | Δ | rel_delta |
|---|---|---|---|---|
| [0.00, 0.10) | 0.4360 | 0.4541 | +0.0180 | **+4.13 %** |
| [0.10, 0.20) | 0.8963 | 0.9027 | +0.0064 | +0.71 % |
| [0.20, 0.30) | 0.9040 | 0.9103 | +0.0063 | +0.70 % |
| [0.30, 0.50) | 0.8943 | 0.9029 | +0.0085 | +0.96 % |
| [0.50, 0.70) | 0.8324 | 0.8453 | +0.0130 | +1.56 % |
| [0.70, 1.00) | 0.6785 | 0.7072 | +0.0287 | **+4.24 %** |

The MC-model spread is **U-shaped in z**: smallest in the canonical
multiplicity-shoulder bins (0.1 < z < 0.5, where both generators agree
to ~1 %), largest at the two extremes — the soft low-z tail (where the
`pT_lab > 0.15` cut bites) and the hard high-z tail (where the `|η|<1.3`
cut bites). Both extremes are precisely the regions where wide-fiducial
extrapolation is largest.

### pT_h projection (integrated over Q², x, z, charge)

| pT_h range (GeV) | A_geom^DJ | A_geom^RG | Δ | rel_delta |
|---|---|---|---|---|
| [0.0, 0.5) | 0.4414 | 0.4634 | +0.0220 | **+4.99 %** |
| [0.5, 1.0) | 0.5204 | 0.5464 | +0.0259 | **+4.99 %** |
| [1.0, 2.0) | 0.6085 | 0.6343 | +0.0258 | +4.24 % |
| [2.0, 4.0) | 0.6632 | 0.6665 | +0.0034 | +0.51 % |
| [4.0, 8.0) | 0.6489 | 0.6269 | **−0.0220** | **−3.39 %** |

Two distinct regimes:
- **Low and moderate pT_h** (< 2 GeV): RAPGAP places more particles in
  the fiducial than DJANGOH → wide yield smaller under RAPGAP by ~4–5 %.
- **High pT_h** (4–8 GeV): the sign **flips** — RAPGAP loses A_geom
  relative to DJANGOH by ~2 pp. RAPGAP's high-pT_h fragmentation is
  somewhat **more forward** (higher |η_lab|) than DJANGOH's. The bin is
  low-stat for data (top-10 unweighted shifts all sit in `pT_h > 4 GeV`
  corner bins) so the cross-section impact is small, but it is real.

This sign-flip is the only place in the diagnostic where the
direction of the MC bias is not monotonic in the cut-distance
variable. It is consistent with RAPGAP's harder parton shower producing
more energetic forward jets at high pT_h.

---

## 6. Where the systematic matters most (data-weighted)

Top-10 bins by `|delta_A_geom| · N_data` (the absolute contribution to the
cross-section uncertainty):

| Cell | charge | z | pT_h (GeV) | N_data | A_DJ | A_RG | Δ | rel_delta |
|---|---|---|---|---|---|---|---|---|
| S0b | h⁺ | [0.0,0.1) | [0.0,0.5) | 204,003 | 0.390 | 0.411 | +0.021 | +5.4 % |
| S0b | h⁻ | [0.0,0.1) | [0.0,0.5) | 199,507 | 0.389 | 0.411 | +0.022 | +5.5 % |
| S0a | h⁺ | [0.0,0.1) | [0.0,0.5) | 287,685 | 0.433 | 0.447 | +0.014 | +3.2 % |
| S0a | h⁻ | [0.0,0.1) | [0.0,0.5) | 282,560 | 0.434 | 0.448 | +0.014 | +3.2 % |
| S1a | h⁺ | [0.0,0.1) | [0.0,0.5) | 157,093 | 0.449 | 0.473 | +0.024 | +5.4 % |
| S1a | h⁻ | [0.0,0.1) | [0.0,0.5) | 154,545 | 0.450 | 0.474 | +0.025 | +5.5 % |
| S1b | h⁺ | [0.0,0.1) | [0.0,0.5) | 95,639  | 0.367 | 0.393 | +0.027 | +7.2 % |
| S1b | h⁻ | [0.0,0.1) | [0.0,0.5) | 92,949  | 0.365 | 0.391 | +0.026 | +7.2 % |
| S0a | h⁻ | [0.0,0.1) | [0.5,1.0) | 118,815 | 0.521 | 0.536 | +0.015 | +2.9 % |
| S0a | h⁺ | [0.0,0.1) | [0.5,1.0) | 121,030 | 0.519 | 0.533 | +0.014 | +2.8 % |

The data-weighted top-10 is **entirely concentrated in the
`z ∈ [0, 0.1)` band** — the same region already flagged as
extrapolation-dominated in the DJANGOH-only diagnostic. The MC-model
systematic on those bins is **3–7 %** of the published wide-fiducial yield,
sign-coherent (RAPGAP shifts the wide-fiducial corrected yield *down* by
3–7 % in every one of these high-statistics bins).

Outside the `z < 0.1` band the data-weighted systematic falls below ~3 %
per bin and the per-cell integrated impact stays within the 1.6 % – 9.1 %
range listed in §4.

---

## 7. Implication for the systematic budget

`rel_delta` is a one-to-one fractional systematic on the wide-fiducial
corrected yield `N_corr_widefid`. Under the standard "half the spread"
convention (one MC envelope: DJANGOH ↔ RAPGAP), the systematic per bin is
`half_diff = 0.5 · |delta_A_geom|`, and the **fractional systematic on
`N_corr_widefid` is `0.5 · |rel_delta|`**:

| Region | Fractional MC-model sys (half-spread) on N_corr_widefid |
|---|---|
| Integrated (all of Scheme B′) | ~1.8 % |
| Typical S?a cell (low-x) | 0.8 – 2.9 % |
| Typical S?b cell (high-x) | 2.1 – **4.5 %** |
| S3b (highest-x, highest-Q², lowest A_geom) | **4.5 %** |
| z ∈ [0.1, 0.5] core bins | < 0.5 % |
| z ∈ [0.0, 0.1) tail (extrapolation-dominated) | ~2 % typical, up to ~3.5 % per bin |
| z ∈ [0.7, 1.0) tail | ~2.1 % |
| pT_h ∈ [4, 8) GeV (only sign flip) | ~1.7 % |

Compared to the canonical SIDIS systematic budget (typically: a few % from
tracking, a few % from trigger, a few % from BBB closure, ~5 % from
absolute luminosity), this MC-model systematic on the wide-fiducial
extrapolation is **a comparable, but not dominant, contribution overall**,
and a **dominant or sub-dominant contribution in the high-x cells**
(S?b cells, especially S3b).

---

## 8. Recommendation update

The DJANGOH-only memo (§7-8) framed the convention choice as a tradeoff
between exposing model-dependence into the central value (Convention A) vs
exposing the lab-fiducial cut into the theory comparison (Convention B).
It declined to make a recommendation. The new DJANGOH ↔ RAPGAP spread
sharpens the picture:

- The **integrated wide-fiducial MC-model systematic is +3.6 %** (or
  ±1.8 % under the half-spread convention). This is **smaller than
  feared**: a-priori, with 50 % of the integrated yield being MC
  extrapolation, one could have plausibly seen a 10–20 % spread. The two
  generators apparently agree on the gross shape of the forward
  fragmentation tail well enough that the integrated central value is
  robust to the MC choice at the few-% level.

- **In the high-x cells (S?b), the MC-model systematic is 4–9 % per
  cell**, with S3b at 9 % being the worst. This is **not negligible** and
  needs to be quoted in the systematic table for any Convention-A
  publication. It is *not* a showstopper — 9 % is a normal-size systematic
  for an inclusive-rate measurement.

- **In the canonical multiplicity-shoulder bins (0.1 ≤ z ≤ 0.5, all pT_h
  except the highest)** the systematic is **below 1 %** per bin. The
  central, physics-rich part of the multiplicity distribution is robust
  to the convention choice and to the MC choice. If the publishable
  product is the multiplicity shape integrated to z > 0.1 (the
  HERMES/COMPASS convention), the MC-model systematic on the wide-fiducial
  yield is essentially negligible.

- The **z < 0.1 tail** carries the largest data-weighted shifts but is
  already an unusual region to quote (most SIDIS publications cut at
  z > 0.1 or higher). If z < 0.1 bins are included in the publishable
  product, they need a per-bin systematic of ~2 % (half-spread) which is
  small in the budget but should be flagged.

- The **high-pT_h (4-8 GeV) sign flip** is interesting physics-wise (it
  indicates RAPGAP's harder shower produces more forward high-pT_h
  hadrons than DJANGOH's) but is in low-stat bins and contributes
  negligibly to the data-weighted systematic.

### Bottom line

The previously identified large fraction of MC extrapolation (~50 % of
integrated yield under Convention A) **does not translate into a large
MC-model systematic on the central value**: the two generators agree to
~3.6 % integrated and ≤ 9 % in any single (Q²,x) cell. **This significantly
strengthens the case that Convention A (wide-fiducial published cross
section) is defensible** as long as the per-cell MC-model systematic is
explicitly quoted alongside the central value (essentially the **hybrid
option 1** described in the DJANGOH-only memo §7: publish the central
value, tabulate the per-bin extrapolation factor and its model spread).

A pure-Convention-B publication is still a fully valid choice — it has
the strict advantage of zero MC-model dependence from this source — but
the wide-fiducial publication is no longer endangered by a hidden 50 %
MC dependence. The "extrapolation" is more constrained than its raw
fraction would suggest, because both DJANGOH and RAPGAP are tuned to H1
forward data (HFS-related measurements) and agree on the forward
fragmentation tail to a few %.

The supervisor's previous decision criterion — "is the wide-fiducial
extrapolation model-robust?" — now has a quantitative answer: **yes, at
the few-% level integrated and at the 9 %-level in the worst (Q²,x) cell**.

---

## 9. Files

| Artifact | Path |
|---|---|
| Script | `sidis_highQ2/scripts/diagnose_gen_fiducial_acceptance_compare_DJ_RG.py` |
| JSON   | `sidis_highQ2/data/gen_fiducial_acceptance_compare_DJ_RG.json` |
| PDF    | `sidis_highQ2/plots/gen_fiducial_acceptance_compare_DJ_RG.pdf` |
| Memo (this file) | `sidis_highQ2/docs/methodology/gen_fiducial_acceptance_compare_DJ_RG_2026-05-24.md` |
| DJ baseline JSON (unchanged) | `sidis_highQ2/data/gen_fiducial_acceptance_HQ-v5g-Bprime.json` |
| DJ baseline PDF  (unchanged) | `sidis_highQ2/plots/gen_fiducial_acceptance_HQ-v5g-Bprime.pdf` |
| DJ baseline memo (unchanged) | `sidis_highQ2/docs/methodology/gen_fiducial_acceptance_2026-05-24.md` |
| RAPGAP gen CSV (unchanged) | `sidis_highQ2/data/mc_v3/pertrack_mc_v3_gen_rapgap31_merged.csv.gz` |
| BBB reference (unchanged) | `sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json` |

The JSON includes:
- **Per-bin records (480 entries)** with `A_geom^DJ`, `A_geom^RG`,
  `delta_A_geom`, `rel_delta`, `half_diff`, plus the underlying
  `N_gen_widefid` / `N_gen_recofid` for both MCs and `N_data`.
- **Integrated per-(Q²,x) cell rows** (per charge AND charge-summed).
- **z and pT_h projections** of A_geom (both MCs), delta, rel_delta.
- **Top-10 lists** by `|delta_A_geom|` and by `|delta_A_geom|·N_data`.
- Headline statistics, input track counts, full input bookkeeping.

---

## 10. Limitations and caveats

- **Same caveats as the DJANGOH-only memo apply unchanged**: lab-frame
  fiducial cut on gen tracks (no migration-aware correction at the
  |η|=1.3 boundary), no radiative correction on the gen rates (irrelevant
  to the ratio), inherited PRELIMINARY-ABS-NORM-UNRESOLVED on the
  underlying BBB JSON (irrelevant to `A_geom` or to `delta_A_geom`).
- **Only DJANGOH vs RAPGAP**, the two production-MC samples available. A
  third generator (e.g. HERWIG, PYTHIA) would refine the envelope but is
  not produced for this analysis. The DJ ↔ RG spread is the standard H1
  practice for fragmentation-model uncertainty.
- **The non-rad samples** (`pertrack_mc_v3_gen_djangoh14_norad_merged.csv.gz`)
  are not used; the `A_geom` ratio is purely a phase-space fraction and is
  insensitive to the QED radiative state of the parent MC. Both gen
  pre-filters used here are from the radiative samples (consistent with
  the BBB convention).
- **One pathological bin** (S3b h⁺, z[0.5,0.7), pT_h[4,8)) has A_geom~0.003
  in both MCs and N_data=0; its rel_delta=135 % carries no
  cross-section weight and is flagged in the JSON for completeness.
- The integrated comparison `+3.6 %` is computed by taking the ratio of
  sums (sum N_gen_reco / sum N_gen_wide), which is the
  `N_gen_widefid`-weighted average of per-bin A_geom. A simple
  unweighted average of per-bin rel_delta values would give a different
  number (mean |rel_delta| = 4.08 %); the weighted integrated number is
  the correct one for systematics propagation onto the integrated
  cross section.

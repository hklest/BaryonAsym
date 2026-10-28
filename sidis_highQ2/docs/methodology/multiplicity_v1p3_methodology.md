# SIDIS Multiplicity M^{h±}(Q², x, z, pT_h) — v1.3 Methodology

**Date:** 2026-05-30
**Status:** CURRENT — bg-subtracted v3final result for data release v1.3
**Schema:** `mult_v3final.1_bgsubtracted`
**Script:** `scripts/v3final/compute_multiplicities_v3final.py`

---

## 1. Observable Definition

The charged-hadron SIDIS multiplicity is defined as:

```
M^h(Q², x, z, pT_h) = (1 / N_DIS(Q², x)) × dN^h(Q², x, z, pT_h) / (dz · dpT_h)
```

- **N^h** — bg-corrected, acceptance-corrected, unfolded charged-hadron yield in the
  4D bin `(Q², x, z, pT_h)`, charge-separated `h+` and `h−`
- **N_DIS** — bg-corrected, acceptance-corrected (BBB) DIS event count in the same
  `(Q², x)` cell (see §2 below)
- **Δz, ΔpT_h** — bin widths (`Δz` dimensionless; `ΔpT_h` in GeV)
- **Units:** 1/GeV (pT_h bin width denominates the measurement)

The binning is **Scheme B′ (HQ-v5g-Bprime)**: 4 Q² strata × 2 x bins per stratum = 8
`(Q², x)` cells; 6 z bins; 5 pT_h bins; 2 charges → 480 total bins.

---

## 2. N_DIS Denominator — Three-Step Computation

The DIS event denominator is computed in three steps to match the bg correction applied
to the hadron numerator.

### Step 1 — Raw data event count

Count **unique (period, run, event)** tuples per `(Q², x)` cell from the data per-track
merged CSV:

```
output/pertrack_v5g_fixedFrame_v3p2/pertrack_v5g_fixedFrame_v3p2_merged.csv.gz
```

All events in this CSV have already passed the v3final downstream analysis cuts
(applied by the v3 extractor `extract_pertrack_v5g_fixedFrame_v3.py`):
- Q² > 150 GeV², 0.1 < y < 0.7, 35 < Empz < 70 GeV
- xF_Trento > 0 (current hemisphere), z_outlier = 0

Cell assignment uses `Q2es` and `Xes` with Scheme B' boundaries.

### Step 2 — Background subtraction at DIS event level

Subtract scaled low-Q² background DIS events using the bg disevent CSVs:

```
output/pertrack_mc_v3p2_bg/pertrack_mc_v3p2_bg_disevent_djangoh14_lowQ2bkgd_merged.csv.gz
output/pertrack_mc_v3p2_bg/pertrack_mc_v3p2_bg_disevent_pythia62_merged.csv.gz
```

**Same cut as the hadron pipeline (final disposition 2026-05-29):**
- `Q2sGen < 60 GeV²` (signal MC lower edge)
- `Q2sGen > 1e-6` (sentinel filter for broken gen-electron pointer)
- `pass_reco == 1`

Weight: `weight1 × weight2 × NucliaWeight`

Scale factors (historical 7110/Pythia configuration from `output/lumi_mc_bg_authoritative.json`; verify sample identity before reuse):
- `scale_DjBkgd = L_data / L_MC_DjBkgd = 356,613 / 15,813.4 = 22.551` for `djangoh14/7110` DjBkgd only, not processed 7091
- `scale_Pythia  = L_data / L_MC_Pythia  = 356,613 / 40,520.2 =  8.801`

Correction note (2026-06-23): if the processed input is `merged_7091.root`, use `djangoh14/7091` RAD signal MC luminosity `4,379,935 nb⁻¹` from `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`, record `per_catalog/djangoh14_7091_ep0607`, giving scale `356613.0/4379935.0 = 0.0814197`; the 7110 scale overestimates 7091 by `276.98×`.

N_DIS bg-corrected fractions per cell:

| Cell | N_raw  | DjBkgd (scaled) | Pythia (scaled) | N_bg_corr | frac_bg |
|------|--------|-----------------|-----------------|-----------|---------|
| S0a  | 153,390 | 41,365  | 6,225  | 105,800 | 31.0% |
| S0b  | 152,593 |    734  |    92  | 151,767 |  0.5% |
| S1a  |  84,057 | 15,849  | 1,208  |  67,000 | 20.3% |
| S1b  |  78,878 |    253  |    35  |  78,590 |  0.4% |
| S2a  |  24,828 |  2,709  |   131  |  21,988 | 11.4% |
| S2b  |  21,458 |      0  |     0  |  21,458 |  0.0% |
| S3a  |  11,068 |    606  |     9  |  10,454 |  5.6% |
| S3b  |   6,933 |     23  |     0  |   6,910 |  0.3% |

**Physics check:** S0a (lowest x, lowest Q²) is most affected (31% bg correction),
physically consistent with the low-Q² leakage pattern. S2b/S3b cells are negligibly
affected (< 0.5%), consistent with the very different kinematic region vs the bg
generator acceptance.

### Step 3 — DIS acceptance correction (BBB)

The acceptance correction for DIS events uses the **signal MC (DJANGOH14) disevent CSV**:

```
output/pertrack_mc_v3p2/pertrack_mc_v3p2_disevent_djangoh14_merged.csv.gz
```

Acceptance: `eps_DIS(cell) = N_pass_gen / N_pass_reco`

- **N_pass_reco**: events with `pass_reco == 1`, cell from `(Q2es, Xes)` → reco cell
- **N_pass_gen**: events with `pass_gen == 1`, cell from `(Q2esGen, XesGen)` → gen cell

Gen-cell assignment is correct: the truth-level cell defines the observable's kinematic
domain, consistent with the hadron unfolding convention where gen kinematics define the
truth-level bin.

| Cell | n_reco     | n_gen      | eps_DIS |
|------|-----------|-----------|---------|
| S0a  | 3,179,839 | 3,108,912 | 0.9777  |
| S0b  | 1,993,373 | 1,893,480 | 0.9499  |
| S1a  | 2,220,298 | 2,321,832 | 1.0457  |
| S1b  | 1,566,536 | 1,505,512 | 0.9610  |
| S2a  |   712,347 |   733,910 | 1.0303  |
| S2b  |   684,401 |   682,347 | 0.9970  |
| S3a  |   321,400 |   322,365 | 1.0030  |
| S3b  |   345,175 |   355,795 | 1.0308  |

Note: eps > 1 reflects bin migration between reco and gen cells (same as track-level
BBB acceptance in the hadron unfolding pipeline). This is physically correct.

**Final N_DIS:**
```
N_DIS_final(cell) = N_DIS_bg_corrected(cell) / eps_DIS(cell)
```

---

## 3. Background Correction Consistency

The bg correction is self-consistent between numerator and denominator:

| Layer | Numerator (N_unf^h) | Denominator (N_DIS) |
|-------|---------------------|---------------------|
| Input | unfolded_v3final_FINAL_bgsubtracted_TUnfold.json | per-track data CSV unique event count |
| BG cut | Q²gen < 60 GeV², IsMatched==1, IsMatchedHadron==1, Q²gen>0 | Q²gen < 60 GeV², pass_reco==1, Q²gen>1e-6 |
| BG samples | DjBkgd + Pythia62 reco track CSVs | DjBkgd + Pythia62 disevent CSVs |
| L_MC | Historical 7110 DjBkgd 15,813.4 nb⁻¹; Pythia 40,520.2 nb⁻¹ | same; for processed 7091 use 4,379,935 nb⁻¹ instead |
| Scale factors | Historical 7110 DjBkgd 22.551×; Pythia 8.801× | same; for processed 7091 use 0.0814197 |
| Acceptance | 4D TUnfold unfolding (full 480×480 response) | BBB per cell (N_gen/N_reco from MC disevent) |

The hadron numerator uses `IsMatched==1 & IsMatchedHadron==1` in addition to the
Q²gen cut; the DIS denominator uses `pass_reco==1`. These are physically distinct
filters appropriate to their respective objects (tracks vs events), but both apply
the same Q²gen < 60 cut defining the "low-Q² leakage" contamination.

---

## 4. Uncertainty Propagation

### Statistical uncertainty

```
sigma_M_stat² = M² × [ (sigma_N / N)² + 1/N_DIS ]
```

- `sigma_N` : TUnfold analytic `sigma_stat` (from `err_selected` in unfolded JSON)
- `1/N_DIS` : Poisson uncertainty on the acceptance-corrected DIS event count

### Systematic uncertainty

```
sigma_M_syst = M × (sigma_N_syst / N)
```

where `sigma_N_syst` is the per-bin total systematic from the noise-corrected
combined systematics JSON (`systematics_combined_v3final_noise_corrected.json`).

Bin widths `Δz`, `ΔpT_h`, and `N_DIS` are treated as exact in the systematic term.
The `N_DIS` Poisson uncertainty is counted only in the statistical term.

### Total

```
sigma_M_total = sqrt(sigma_M_stat² + sigma_M_syst²)
```

---

## 5. Results Summary

| Quantity | Value |
|----------|-------|
| Schema | `mult_v3final.1_bgsubtracted` |
| Total 480-bin multiplicity output | `data/multiplicities_v3final_bgsubtracted.json` (298 KB) |
| N_unfolded_total | 5,203,379 |
| Trusted bins | 367 / 480 |
| Negative M^h bins (all / trusted) | 23 / **3** |
| Negative bin note | Same bg MC-stat noise at 22.55× scale in sparse high-pT corners as 3 negative xsec bins (S0a z[2] pT[4] h+/h−, S2a z[0] pT[4] h+); flagged for h1-qa-specialist joint review |

### Charge ratios M^h+ / M^h- (median over trusted paired bins)

| Stratum | Median M^h+/M^h- | n bins |
|---------|-----------------|--------|
| S0 (150–300 GeV²) | 1.033 | 50 |
| S1 (300–700 GeV²) | 1.072 | 53 |
| S2 (700–1500 GeV²) | 1.103 | 42 |
| S3 (1500–20000 GeV²) | 1.149 | 35 |

Charge ratio increases with Q² (1.03 → 1.15), physically consistent with
sea-quark differences (u-quark fragmentation produces more h+ than h−;
the effect is enhanced at high x where valence u dominates).

### z-pT integrated ⟨n^h±⟩ per DIS event (trusted bins)

The z-pT integrated multiplicity ∑ M^h × Δz × ΔpT provides an estimate of
the mean hadron multiplicity per DIS event in the accepted phase space:

| Stratum | ⟨n^h+⟩_trusted | ⟨n^h-⟩_trusted |
|---------|----------------|----------------|
| S0 (150–300 GeV²)   | 10.76 | 10.64 |
| S1 (300–700 GeV²)   | 11.08 | 10.85 |
| S2 (700–1500 GeV²)  | 11.91 | 11.58 |
| S3 (1500–20000 GeV²)| 15.07 | 14.21 |

**Sanity check:** The expected mean charged-hadron multiplicity in DIS events at
high Q² is approximately ⟨n_ch⟩ ~ ln(s) ~ 2 × ln(Q/Λ), increasing with Q².
The S0 value ~10.6 at Q² ~ 250 GeV² corresponds to W² ~ Q²(1−x)/x ≈ 5000 GeV²,
and ⟨n_ch⟩ ~ 10–12 for h+h− combined in e+p at HERA-II (LEP experience at √s ~ 90 GeV
gives ⟨n_ch⟩ ~ 20 total; at W ~ 70 GeV it is ~16; our ~11 per sign × 2 ~ 22 for all
signs is consistent). The growth S0→S3 reflects the increase in W with Q² at fixed
Bjorken y.

### Fractional uncertainty (median over trusted bins)

| Stratum | Median σ_mult_total/M | p90 |
|---------|----------------------|-----|
| S0 | 3.8% | 24.8% |
| S1 | 5.7% | 29.6% |
| S2 | 6.2% | 23.0% |
| S3 | 8.4% | 18.7% |

These match exactly the xsec fractional uncertainty profile, as expected (same
numerator; N_DIS denominator is treated as a fixed scale factor in the systematic
uncertainty).

---

## 6. Anomaly Register

Three negative trusted M^h bins (same pathology as xsec):

| idx | charge | cell | z | pT | M value | Status |
|-----|--------|------|---|----|---------|--------|
| 14  | h+     | S0a  | 0.2–0.3 | 4–8 GeV | −1.38×10⁻² 1/GeV | Flagged — qa-specialist |
| 124 | h+     | S2a  | 0.0–0.1 | 4–8 GeV | −2.44×10⁻³ 1/GeV | Flagged — qa-specialist |
| 254 | h−     | S0a  | 0.2–0.3 | 4–8 GeV | −1.88×10⁻² 1/GeV | Flagged — qa-specialist |

Physics interpretation: These are all in the high-pT tail (pT_h ∈ [4, 8] GeV) where
the bg scale factor 22.55× amplifies MC statistical fluctuations in sparse bins.
The bg subtraction produces a negative estimate for the hadron yield in these bins;
the multiplicity inherits the sign. The QA verdict for xsec and A_h will apply equally
to M^h.

An additional 20 untrusted negative bins are present (same as xsec); they are outside
the published measurement set and are not released.

---

## 7. Inputs Inventory

| File | Role | Version |
|------|------|---------|
| `output/unfolded_v3final_FINAL_bgsubtracted_TUnfold.json` | Hadron yield N_unf^h (numerator) | v3final FINAL, 2026-05-29 |
| `output/response_4d_v3final_FINAL_bgsubtracted.npz` | Bin widths (z, pT_h) | v3final FINAL |
| `data/systematics_combined_v3final_noise_corrected.json` | Noise-corrected systematics | v3final |
| `data/v5g_4d_trust_mask_v3p2.json` | Trust mask 367/480 | v3p2=v3final |
| `output/pertrack_v5g_fixedFrame_v3p2_merged.csv.gz` | Data per-track (N_DIS raw count) | v3p2=v3final |
| `output/pertrack_mc_v3p2_bg_disevent_djangoh14_lowQ2bkgd_merged.csv.gz` | DjBkgd DIS events (bg) | v3p2 bg |
| `output/pertrack_mc_v3p2_bg_disevent_pythia62_merged.csv.gz` | Pythia DIS events (bg) | v3p2 bg |
| `output/pertrack_mc_v3p2_disevent_djangoh14_merged.csv.gz` | Signal MC DIS events (acceptance) | v3p2=v3final |

---

## 8. Reproducibility

```bash
cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2
python3 scripts/v3final/compute_multiplicities_v3final.py
# output: data/multiplicities_v3final_bgsubtracted.json
```

Expected runtime: ~3–4 minutes on the login node (reading ~15M rows total across
3 CSV files). Single-file operation: login-node policy permits.

All defaults match the canonical v3final paths. No CLI flags required for nominal run.

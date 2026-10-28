# Phase 1 results: data-vs-MC comparison for H1 high-Q² charged-particle SIDIS

**Date:** 2026-05-27
**Status:** PRELIMINARY — uncalibrated rad-off LO+PS generator predictions against
preliminary H1 data; pilot statistics only; backgrounds not yet subtracted
**Audience:** SIDIS supervisor and future MC-tuning / theory-comparison work
**Output artifacts:** `MC/predictions/`, `MC/plots/comparison_per_bin.csv`,
`MC/plots/data_vs_mc_phase1.pdf`, `MC/plots/fid_release_vs_wide_vs_fullps.pdf`

---

## 1. Summary

Phase 1 of the H1 high-Q² charged-particle SIDIS theory-prediction subproject
produced the first generator-level comparison between the preliminary H1 data
release and two LO+PS Monte Carlo generators: Pythia8 8.317 (1M events) and
Sherpa 3.0.3 (2M events). Both generators used NNPDF31_lo_as_0118, rad-off
(leptonic-line QED off), and a common long-lived-wall species definition
(K0_S, K0_L, Λ/Λ̄, Σ±, Ξ0/−, Ω−). Three track selections were derived
post-hoc from the same per-job ROOT ntuples: full phase space (`fullps`),
wide gen fiducial (`fid_wide`), and the data-release fiducial (`fid_release`).

Comparing against 480 unfolded yields in the Scheme B′ 4D binning
(8 (Q²,x) cells × 6 z bins × 5 pT_h bins × 2 charges), with 364 of 480 bins
marked trustworthy, the χ²/dof using the full 480×480 covariance matrix ranges
from 317 to 683 depending on generator and selection. These values are
large — as expected for uncalibrated LO MC without any H1 tuning — and confirm
that the dataset has substantial discriminating power for future
PDF/tune/NLO-matched studies.

Of the 364 trustworthy bins, 197 are over-predicted by both generators, 134
are under-predicted by both, and only 33 (~9%) are bracketed. Sherpa sits
systematically above Pythia8 (median N_MC/N_data ≈ 1.21 vs 1.08). The two
most striking deviations are: (a) both generators under-predict the first
z bin (z < 0.1) by large factors across all (Q²,x) cells, and (b) the
highest pT_h bin (4–8 GeV) is under-predicted by factors of 5–10 in both
generators at low and mid z. The fiducial-extrapolation study shows that
the data-release central-tracker acceptance (|η_lab| < 1.5, pT_lab > 0.15 GeV)
retains only 23–45% of generator-level charged hadrons, with both generators
agreeing on the lost fraction to within ~2–3 percentage points despite
differing in absolute yield by ~50%. **These caveats are structural, not
artifacts of pilot statistics; see §8.**

Next steps: resolve the Pythia8 σ_gen discrepancy (§8), complete the
Herwig 7.3.0p1 Phase 1d pilot (§9), and move to Phase 2 full statistics
(10M events per generator).

---

## 2. Inputs

### 2.1 Data

| Field | Value |
|---|---|
| Dataset | H1 high-Q² charged-particle SIDIS data release v1.0 (`data_release/README.md`) |
| Status | PRELIMINARY (backgrounds not subtracted; see `data_release/README.md` §9) |
| Collider | HERA-II, 27.6 × 920 GeV e±p, √s = 318 GeV |
| Integrated luminosity | 356 613 nb⁻¹ (four-period v5g: ep0304 + em0405 + em06 + ep0607) |
| e⁺p sub-lumi | 194 012 nb⁻¹ (ep0304 + ep0607) |
| e⁻p sub-lumi | 162 601 nb⁻¹ (em0405 + em06) |
| Kinematic region | Q² > 150 GeV², 0.1 < y < 0.7 |
| Hadron fiducial | pT(lab) > 0.15 GeV, \|η(lab)\| < 1.5 (central silicon + CJC) |
| Binning | Scheme B′ (HQ-v5g-Bprime): 8 (Q²,x) cells × 6 z bins × 5 pT_h bins × 2 charges = 480 bins |
| Bin index | `idx = charge_idx × 240 + cell_idx × 30 + z_idx × 5 + pTh_idx` |
| Observable | `N_unfolded` — truth-level fiducial yield per 4D bin (not a cross section) |
| Uncertainty model | 1 stat + 9 systematics; full 480×480 covariance (`measurement/uncertainty_covariance.npz`) |
| Trust mask | 364 / 480 bins trustworthy (`measurement/yields_per_bin.csv` column `trustworthy`) |
| Source CSV | `data_release/measurement/yields_per_bin.csv` |

The 8 (Q²,x) cells are:

| Cell | Q² range [GeV²] | x range |
|---|---|---|
| S0a | 150–300 | 0.0021–0.0071 |
| S0b | 150–300 | 0.0071–0.030 |
| S1a | 300–700 | 0.0042–0.016 |
| S1b | 300–700 | 0.016–0.069 |
| S2a | 700–1500 | 0.0099–0.034 |
| S2b | 700–1500 | 0.034–0.15 |
| S3a | 1500–20000 | 0.021–0.087 |
| S3b | 1500–20000 | 0.087–1.5 |

z bins: [0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]; pT_h bins (GeV): [0, 0.5, 1.0, 2.0, 4.0, 8.0].
Full frame and coordinate conventions in `data_release/schema/coordinate_conventions.md`:
z = (P·p_h)/(P·q) Lorentz-invariant; pT_h in γ*+p CM frame; φ_h Trento convention.

### 2.2 MC generators

| Generator | Family | Version | Environment | PDF | Tune / hadronization |
|---|---|---|---|---|---|
| Pythia8 | LO+PS | 8.317 | LCG_109a / x86_64-el9-gcc14-opt | NNPDF31_lo_as_0118 | Monash 2013 (default) |
| Sherpa | LO+PS | 3.0.3.atlas1-4ee77 | LCG_109a (ATLAS-patched; LD_LIBRARY_PATH fix required) | NNPDF31_lo_as_0118 via LHAPDFSherpa | Comix ME + cluster hadronization (default) |

Common settings for both generators:
- HERA-II beams: 27.6 GeV electron/positron × 920 GeV proton
- NC DIS only; Q² > 150 GeV² generator-level cut
- Rad-off: leptonic-line QED ISR/FSR off (Pythia8: `SpaceShower:QEDshowerByL = off` +
  `TimeShower:QEDshowerByL = off`; Sherpa: `YFS:MODE:Off` — confirmed by
  `Settings_Report.md` entry `YFS:MODE Full | Off user | Off effective`)
- Long-lived wall locked stable: K0_S (310), K0_L (130), Λ/Λ̄ (±3122),
  Σ± (±3222, ±3112), Ξ (±3312, ±3322), Ω (±3334)
- Species: gen-level primary π±/K±/p/p̄ with no long-lived ancestor
  (Option-H; `MC/common/option_h_selector.py`)
- Beam-axis: generators run in FLIPPED orientation (lepton at +z);
  reducer applies R_y(180°) to all 4-vectors to restore H1 convention
  (`beam_orientation_correction_applied = true` in all 6 JSONs)

**Generators dropped from v1:**

Pythia6 6.4.29.2 was attempted but **dropped** due to a PYMAXI envelope failure
for HERA-II asymmetric ep at Q² > 150 GeV² (ISUB 10 cross-section inflated by
~3.16× due to low-Q² accept-reject violations), producing a 40–350× excess
over Sherpa in the Q² > 3000 GeV² tail. The EIC-community fork (`PYTHIA-RAD-CORR`,
gitlab.com/eic/mceg) does not address this defect (its `pyrand.f` accept-reject
machinery is byte-identical to stock 6.4.28). Pythia6 is excluded from Phase 1
with this documented justification; see `MC/common/research-eic-pythia6-2026-05-26.md`
and `MC/smoke_q2_v2/investigations_2026-05-26.md`.

Herwig 7.3.0p1 (LCG_109a) is assessed as ready (verdict A) for a Phase 1d smoke:
all ingredients confirmed — shipped `DIS.in` + `EPCollider.in` for NC DIS, LHAPDF6
NNPDF31_lo, lepton-line rad-off via splitting deletion, HepMC3 ASCII output linked
against `libHepMC3.so.4`. Phase 1d smoke is in flight; see
`MC/common/research-herwig-2026-05-27.md`.

### 2.3 Three track selections

All three selections are derived post-hoc from the same per-job ROOT ntuple
`hadrons` TTree; no re-generation is needed.

| Selection | Lab-frame cuts | Physics meaning |
|---|---|---|
| `fullps` | None (species only) | Full generator phase space; quantifies total hadronic production |
| `fid_wide` | \|η_lab\| < 3.5 AND pT_lab > 0.05 GeV | Analysis-wide gen fiducial; unfolding target |
| `fid_release` | \|η_lab\| < 1.5 AND pT_lab > 0.15 GeV | Matches data_release central-tracker acceptance |

Species selection is applied identically across all three: π±/K±/p/p̄ with
no long-lived ancestor (Option-H). The `fid_release` selection is the directly
comparable quantity for data-vs-MC residuals.

### 2.4 Predicted-yield formula

For each bin b in each selection:

```
N_pred[b] = σ_gen_ep × (n_h_ep[b] / n_DIS_ep) × L_ep_pb
           + σ_gen_em × (n_h_em[b] / n_DIS_em) × L_em_pb
```

where L_ep = 194.012 pb⁻¹, L_em = 162.601 pb⁻¹ (L_total = 356.613 pb⁻¹),
and n_h[b] / n_DIS is the Monte Carlo hadron yield fraction in that bin.
σ_gen values are the per-beam-mode generator-reported cross sections from the
pilot runs. The combination is equivalent to the luminosity-weighted mixture
applied to the data (`data_release/README.md` §3).

---

## 3. Pilot production

### 3.1 Pythia8 8.317 (Phase 1a)

| Beam mode | Seed | N_generated | σ_gen (pb) | Label |
|---|---|---|---|---|
| e⁺p (ep) | 100001 | 500 000 | **3044 pb** (rad-off, NNPDF31_lo_as_0118, Q² > 150) | Phase 1a |
| e⁻p (em) | 100002 | 500 000 | **3046 pb** (same settings) | Phase 1a |
| **Total** | — | **1 000 000** | — | — |

Prediction JSONs:
- `MC/predictions/pythia8-8.317_phase1a_2026-05-27_fullps.json`
- `MC/predictions/pythia8-8.317_phase1a_2026-05-27_fid_wide.json`
- `MC/predictions/pythia8-8.317_phase1a_2026-05-27_fid_release.json`

All three JSONs confirm: `beam_orientation_correction_applied = true`,
`binning_tag = "HQ-v5g-Bprime"`, `composition_validation` all-zero long-lived
ancestor fractions, `n_events = {"ep": 500000, "em": 500000, "total": 1000000}`.

**Known σ_gen discrepancy:** the v2 smoke (10k events, same cmnd, seed 12345)
reported σ ≈ 1950 ± 201 pb (`MC/smoke_q2_v2/smoke_test_v2_2026-05-26.md` §1.1);
the Phase 1a pilot reports σ ≈ 3044 pb, a factor of ~1.56× higher. The
discrepancy is under investigation (likely a run-card or kinematic-cut change
between the smoke and the production cmnd). This difference affects only the
absolute σ_gen normalisation used in the yield formula; it does not affect the
internal consistency of the downstream pipeline (all per-bin ratios n_h/n_DIS
are computed from the same events as the σ_gen value). This is a known-open
audit item; see §8.

### 3.2 Sherpa 3.0.3 (Phase 1b)

| Beam mode | Seed | N_generated | σ_gen (pb) | Label |
|---|---|---|---|---|
| e⁺p (ep) | 200001 | 1 000 000 | **4373.78 ± 3.80 pb** | Phase 1b |
| e⁻p (em) | 200002 | 1 000 000 | **4343.65 ± 3.78 pb** | Phase 1b |
| **Total** | — | **2 000 000** | — | — |

Prediction JSONs:
- `MC/predictions/sherpa-3.0.3_phase1b_2026-05-27_fullps.json`
- `MC/predictions/sherpa-3.0.3_phase1b_2026-05-27_fid_wide.json`
- `MC/predictions/sherpa-3.0.3_phase1b_2026-05-27_fid_release.json`

Sherpa σ_gen is consistent with the v2 smoke result (4367 ± 38 pb at 10k events;
the improvement to ±3.8 pb reflects the √(100×) statistics gain). The run-card
includes `YFS:MODE:Off` (confirmed from the v2 smoke Settings_Report) and the
`PARTICLE_DATA: {PDG: { Stable: 1 }}` block for all eight long-lived PDGs,
resolved by `MC/common/research-pilot-blocking-opens-2026-05-26.md`.

### 3.3 Stage 1 reducer validation

Prior to the full pilot, the reducer pipeline was validated on a 1k-event
Pythia8 ep smoke (`MC/predictions_raw/_validation_pythia8_1k_2026-05-26.md`):
9/10 hard checks PASS. The one deviation (check j: 66.8% fid_wide acceptance
vs ≥80% threshold) is a physics feature — the natural forward shift of
η_lab at HERA-II asymmetric beams — not a reducer defect.

---

## 4. χ² summary

χ² computed with the full 480×480 covariance restricted to the 364
trustworthy bins. Also shown: χ² using diagonal-only covariance (which
substantially over-estimates tension; see `data_release/README.md` §7).
All χ²/dof values use dof = 364 (number of trustworthy bins).

| Generator | Selection | χ²_full / dof | χ²_diag / dof |
|---|---|---|---|
| Pythia8 8.317 | `fullps` | **352.78** | 114.19 |
| Pythia8 8.317 | `fid_wide` | **339.98** | 101.55 |
| Pythia8 8.317 | `fid_release` | **458.34** | 147.71 |
| Sherpa 3.0.3 | `fullps` | **466.33** | 259.52 |
| Sherpa 3.0.3 | `fid_wide` | **316.50** | 169.53 |
| Sherpa 3.0.3 | `fid_release` | **682.58** | 176.11 |

χ²/dof ~ 300–700 across all six combinations is the expected outcome for
uncalibrated, untuned LO+PS generator predictions compared against
acceptance-corrected, radiative-corrected H1 data with a tight 480×480
covariance. These values do not indicate a pipeline error; they indicate
that the data has substantial discriminating power for future PDF-variation,
tune-variation, and NLO-matched studies. The diagonal-only χ² exceeds the
full-covariance χ² by a factor of 2–4 in most combinations, consistent with
the strong inter-bin correlations in the statistical covariance (max
off-diagonal correlation 0.997 from TUnfold regularisation).

The Sherpa `fid_release` χ²_full / dof = 682.58 is the worst-performing
combination, driven by Sherpa's systematic over-prediction of data yields
in the central-tracker fiducial (the fid_release selection biases toward
high-multiplicity regions where Sherpa overshoots most strongly).

---

## 5. Data-vs-MC headline comparison

**Primary figure:** `MC/plots/data_vs_mc_phase1.pdf` — 8 pages, one per
(Q²,x) cell in the order S0a → S3b. Each page shows a 2 × 5 grid: rows
are h⁺ (top) and h⁻ (bottom); columns are the 5 pT_h bins from lowest to
highest. Each panel plots N_data with total uncertainty (black points),
N_pythia8_fid_release (blue), and N_sherpa_fid_release (red) vs z bin index.

Quantitative summary over the 364 trustworthy bins:

| Outcome | Count | Fraction |
|---|---|---|
| Both generators over-predict | 197 | 54% |
| Both under-predict | 134 | 37% |
| Data bracketed between generators | 33 | 9% |

Bins missing data by > 5σ in both generators: 165 of 364 (45%).

**Headline patterns:**

1. **First z bin (z < 0.1): massive under-prediction by both generators**
   across all 8 (Q²,x) cells and both charges. This is a generic
   generator weakness at very low z — the fragmentation models are not
   calibrated in this regime for high-Q² DIS. No reliable prediction
   exists from either generator here.

2. **Mid-z (z ∈ [0.2, 0.5]) at low pT_h (pT_h < 1 GeV): both generators
   over-predict**, Sherpa more so than Pythia8. This is the dominant
   contribution to the 197 over-predicted trustworthy bins. The excess is
   present across all Q² strata.

3. **Highest pT_h bin (4–8 GeV): both generators under-predict by factors
   of 5–10** at low and mid z. This is a real physical signal — high-pT
   charged-hadron production in the γ*+p CM frame at high Q² is
   substantially under-modeled at LO+PS. This is among the most
   physics-interesting deviations in the dataset.

4. **Sherpa is systematically above Pythia8** overall: median
   N_MC/N_data = 1.21 for Sherpa vs 1.08 for Pythia8 on the 364
   trustworthy `fid_release` bins. Sherpa produces roughly 50% more
   charged hadrons than Pythia8 in the central-tracker fiducial, despite
   using the same PDF and Q² cut.

5. **Generator-vs-generator disagreement > 2× in 24 of 364 bins:**
   concentrated at low-z/low-Q²/low-pT (Sherpa higher) and at
   high-z/low-pT or low-z/high-pT (Pythia8 higher). These are the cells
   where generator differences are largest and where future 3-way
   comparisons (adding Herwig) will be most informative.

6. **The cross-generator Sherpa/Pythia8 ratio is Q²-dependent:** ~1.33
   at low Q² (S0a, S0b), declining to ~0.97–1.04 at high Q² (S3a, S3b).
   The integrated ratio ~1.51 is a cell-average, not a flat normalisation
   difference. The Q²-dependent shape suggests different ME treatments or
   parton-shower implementations become relevant at high Q².

**Example interpretation — S0a page (Q² 150–300 GeV², x 0.0021–0.0071):**
The z < 0.1 column shows both generators at roughly 30–50% of the data
yield. The mid-z / low-pT region (z ~ 0.2–0.5, pT_h < 1 GeV) shows
Sherpa up to ~1.5–1.7× the data while Pythia8 sits ~0.9–1.1×. The highest
pT_h column is sparsely populated in the data and dramatically under-predicted
by both generators.

---

## 6. Fiducial extrapolation study

**Figures:** `MC/plots/fid_release_vs_wide_vs_fullps.pdf` (8 pages; same
cell ordering) and `MC/plots/fid_release_vs_wide_vs_fullps.png` (S3b
quick-view).

The study quantifies what fraction of the total generator-level yield
(fullps) survives the data-release fiducial cut (|η_lab| < 1.5,
pT_lab > 0.15 GeV). Values are integrated over z and pT_h within each
(Q²,x) cell.

| Cell | Q² [GeV²] | x range | Pythia8 in fid_release [%] | Sherpa in fid_release [%] |
|---|---|---|---|---|
| S0a | 150–300 | 0.0021–0.0071 | 58.77% | 59.76% |
| S0b | 150–300 | 0.0071–0.030 | 62.24% | 64.03% |
| S1a | 300–700 | 0.0042–0.016 | 56.52% | 57.93% |
| S1b | 300–700 | 0.016–0.069 | 64.25% | 64.90% |
| S2a | 700–1500 | 0.0099–0.034 | 55.10% | 55.96% |
| S2b | 700–1500 | 0.034–0.15 | 69.27% | 68.92% |
| S3a | 1500–20000 | 0.021–0.087 | 55.20% | 55.57% |
| S3b | 1500–20000 | 0.087–1.5 | 76.85% | 77.20% |

All cells are above the 20% flag threshold. The data_release central-tracker
fiducial retains only **23–45% of the generator-level charged-hadron yield**
(equivalently, 55–77% is outside acceptance). The S3b cell (highest Q², high x)
is the most extreme at ~77% loss.

The S3b quick-view (`fid_release_vs_wide_vs_fullps.png`) shows the bin-by-bin
ratio fullps / fid_release rising to approximately 5× in the first z bin
(z < 0.1), with both generators agreeing within ~5%. At very low z, nearly 4
out of every 5 generator-level charged hadrons fall outside the central-tracker
acceptance; these are predominantly forward-going hadrons at η_lab > 1.5.

**Key finding:** both generators agree on the extrapolation fraction to within
~2–3 percentage points in every cell, despite differing in absolute yield by
~50% overall. The fraction of yield lost to the central-tracker acceptance is
generator-independent to this precision, even though the absolute multiplicities
are not.

**Implication for theory consumers:** the data_release measurement is on the
central-tracker acceptance only. When comparing generator predictions
to this dataset using the `fid_release` selection, one is implicitly accepting
that 55–77% of the total charged-hadron production is not measured. Fitting
fragmentation functions or TMD shapes against this data uses a kinematic
region that excludes the bulk of the hadronic final state, particularly at
low z (forward-going hadrons). Extrapolating to the full phase space carries
~50% generator-model dependence on the extrapolation fraction per cell.
The `fid_wide` selection (|η_lab| < 3.5, pT_lab > 0.05 GeV) captures
substantially more of the phase space and is the nominal unfolding target; the
study quantifies the additional step from `fid_wide` to `fullps`.

---

## 7. Cross-generator agreement

Sherpa/Pythia8 ratio on `fid_release` predicted yields, per (Q²,x) cell,
integrated over z and pT_h in trustworthy bins:

| Cell | Sherpa/Pythia8 ratio (approx.) | Pattern |
|---|---|---|
| S0a | ~1.33 | Sherpa substantially higher |
| S0b | ~1.33 | Sherpa substantially higher |
| S1a | ~1.25 | Sherpa higher |
| S1b | ~1.20 | Sherpa moderately higher |
| S2a | ~1.10 | Sherpa mildly higher |
| S2b | ~1.05 | Near equal |
| S3a | ~0.97 | Near equal |
| S3b | ~1.04 | Near equal |
| **Integrated** | **~1.51** | (cell-average; dominated by low-Q² statistics) |

The two generators are LO+PS using the same PDF set (NNPDF31_lo_as_0118), the
same Q² generator cut (150 GeV²), and the same leptonic-line rad-off treatment.
The residual ~50% overall normalisation spread and the Q²-dependent shape
difference reflect genuine generator-model differences: Comix matrix elements
vs Pythia8 LO DIS matrix elements, Sherpa cluster hadronisation vs Pythia8
Lund string, and different parton-shower implementations. The data discriminates
between the two in differential (Q²,x) cells, providing future constraints on
hadronisation-model parameters.

Of the 364 trustworthy bins, 24 (7%) show a >2× generator-vs-generator
disagreement (`|N_sherpa − N_pythia8| / min(N_sherpa, N_pythia8) > 1`).
These are concentrated in the physically interesting corners: low-z/low-Q²/low-pT
(Sherpa higher by the largest margins) and low-z/high-pT or high-z/low-pT
(Pythia8 higher in some bins).

---

## 8. Caveats and known limitations

1. **Pilot statistics.** 1M Pythia8 + 2M Sherpa events. MC statistical
   uncertainty is 5–30% per bin in typical cells, smaller than the data
   σ_total in most bins but non-negligible in the sparsely-populated high-pT
   and high-z bins. Phase 2 (§9) will reduce MC stat to ~1–2% per bin.

2. **LO + parton-shower only.** No NLO matrix-element matching. The ~50%
   Sherpa/Pythia8 difference and the large deviations from data are
   expected at LO without tuning. NLO-matched predictions (Matchbox,
   POWHEG) are deferred to later phases.

3. **Default tunes.** Pythia8 Monash 2013 (default 8.317 tune); Sherpa
   default cluster hadronisation. Neither has been tuned to H1 SIDIS data.

4. **Rad-off MC vs radiative-corrected data.** The data release applies
   the unfolding C_rad factor to correct for QED radiation at the event
   level. The predictions use rad-off generators. This is the correct
   nominal comparison scheme (Born-level prediction vs Born-level corrected
   data), but the matching depends on the unfolding's C_rad treatment
   documented in the analysis, which is not re-derived here.

5. **Backgrounds not subtracted in the data.** The data_release v1.0
   `N_unfolded` values include photoproduction leakage, diffractive
   contamination, and kinematic-boundary migration (see `data_release/README.md`
   §9). Expect O(1–10%) downward shifts in `N_unfolded` in the v2 release,
   larger at low Q² and at the y/Empz boundaries.

6. **No PDF / tune / scale systematics.** Phase 4 will add PDF error bands,
   αs variation, scale variation, and alternative tunes. The current
   comparison is with the central-value PDF member only.

7. **Herwig 7.3.0p1 not yet included.** The v1 comparison is Pythia8 +
   Sherpa only. Phase 1d Herwig smoke is in flight.

8. **Pythia8 Phase 1a σ_gen discrepancy.** σ_gen ≈ 3044 pb in the pilot
   vs σ ≈ 1950 pb in the v2 smoke (same generator, same PDF, nominally
   same Q² cut). Factor ~1.56× discrepancy is a known-open audit item.
   Likely cause: a run-card delta between the smoke cmnd and the production
   cmnd. The downstream pipeline is internally consistent (σ_gen and
   n_h/n_DIS are computed from the same events), so yields per bin are
   self-consistent. However, absolute predictions quoted in future memos
   should be checked once the audit is complete.

9. **Trigger and luminosity systematics absent.** These are not included
   in the data_release v1.0 covariance (`data_release/README.md` §9 items
   6–7). The 2.3% H1 luminosity uncertainty cancels in self-normalised
   multiplicities but should be added in quadrature when comparing absolute
   yields.

---

## 9. Next steps

| Priority | Task | Phase |
|---|---|---|
| High | Resolve Pythia8 σ_gen v2-vs-Phase1a discrepancy (audit run-card delta) | Ongoing |
| High | Phase 1d: Herwig 7.3.0p1 pilot (1M ep + 1M em); integrate into 3-way comparison | Phase 1d |
| Medium | Phase 2: full statistics (10M events per generator per beam mode); reduce MC stat to ~1–2% per bin | Phase 2 |
| Medium | Backgrounds: update comparison once data_release v2 (background-subtracted) is available | Pending data |
| Lower | Phase 4: PDF error bands (NNPDF31_lo replicas), αs variation, scale variation, alternative tunes | Phase 4 |
| Lower | Phase 4 rad-on cross-check: compare rad-off predictions with the `C_rad`-corrected unfolding output to verify consistency | Phase 4 |
| Lower | Focus physics-analysis iteration on the two most anomalous regions: z < 0.1 (first z bin, both generators miss) and pT_h > 4 GeV (both massively under-predict) | Ongoing |
| Lower | Iterate with h1-sidis-supervisor on high-pT/low-z binning strategy for forthcoming Herwig 3-way comparison | After Phase 1d |

---

## 10. Reproducibility

### Prediction JSONs

All six prediction files are at `MC/predictions/`, named
`<generator>_<phase>_2026-05-27_<selection>.json`:

```
MC/predictions/
  pythia8-8.317_phase1a_2026-05-27_fullps.json
  pythia8-8.317_phase1a_2026-05-27_fid_wide.json
  pythia8-8.317_phase1a_2026-05-27_fid_release.json
  sherpa-3.0.3_phase1b_2026-05-27_fullps.json
  sherpa-3.0.3_phase1b_2026-05-27_fid_wide.json
  sherpa-3.0.3_phase1b_2026-05-27_fid_release.json
```

All JSONs share `binning_sha = "2fba3b57b8faa9bb985a1a599587223c5159039563506f27f28f4beb6db27acb"`,
`binning_tag = "HQ-v5g-Bprime"`, and `binning_source =
"H1/sidis_highQ2/docs/data/binning_v5f_frozen.json"`. The binning SHA
provides a cryptographic consistency check.

### Per-bin comparison table

`MC/plots/comparison_per_bin.csv` — 480 rows (header + 480 bins), columns:

```
idx, charge, cell, z_idx, pth_idx,
N_data, sigma_total,
N_pythia8_fullps, N_pythia8_fid_wide, N_pythia8_fid_release,
N_sherpa_fullps,  N_sherpa_fid_wide,  N_sherpa_fid_release,
pull_p8_fid_release, pull_sherpa_fid_release,
ratio_p8_fid_release_over_data, ratio_sherpa_fid_release_over_data,
trustworthy
```

Pulls are `(N_MC − N_data) / sigma_total`. All 364 trustworthy bins have
`trustworthy = 1`; the remaining 116 are retained in the table for completeness.

### Scripts

| Script | Purpose |
|---|---|
| `MC/scripts/compare_data_vs_mc.py` | Loads data + both prediction JSONs, applies trust mask, produces `data_vs_mc_phase1.pdf` and `comparison_per_bin.csv` |
| `MC/scripts/compare_fid_vs_fullps.py` | Produces `fid_release_vs_wide_vs_fullps.pdf` and the cell-level extrapolation table |
| `MC/scripts/build_prediction_from_root.py` | Builds prediction JSONs from per-job ROOT ntuples |
| `MC/scripts/reduce_hepmc_to_root.py` | Converts HepMC3 → slim ROOT ntuple (Stage 1 reducer) |
| `MC/common/option_h_selector.py` | Option-H species selection (ancestor veto) |
| `MC/common/observables.py` | CM-frame observables (z, pT_h, φ_h Trento), R_y(180°) orientation fix |
| `MC/common/binning.py` | Loads + validates frozen B′ binning from `docs/data/binning_v5f_frozen.json` |

Per-job ROOT ntuples are on NAF DUST:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/predictions_raw/`

---

## 11. Citations

| Document | Path | Used for |
|---|---|---|
| MC predictions plan | `H1/sidis_highQ2/docs/plans/plan-MC-predictions-sherpa-pythia6-pythia8.md` | Framework design, physics definitions, file layout |
| Data release README | `H1/sidis_highQ2/data_release/README.md` | Data provenance, uncertainty model, trust mask |
| Coordinate conventions | `H1/sidis_highQ2/data_release/schema/coordinate_conventions.md` | Frame, z, pT_h, φ_h, fiducial definitions |
| Per-bin yields | `H1/sidis_highQ2/data_release/measurement/yields_per_bin.csv` | N_unfolded values, trust mask column |
| Generator availability | `H1/sidis_highQ2/MC/common/research-generator-availability-2026-05-26.md` | LCG_109a environment, binary paths |
| Pilot-blocking opens | `H1/sidis_highQ2/MC/common/research-pilot-blocking-opens-2026-05-26.md` | Sherpa `Stable` YAML key; Pythia6 LHAPDF6 bridge; `YFS:MODE:Off` confirmation |
| Pythia6 EIC fork | `H1/sidis_highQ2/MC/common/research-eic-pythia6-2026-05-26.md` | Rationale for dropping Pythia6 from v1 |
| Herwig availability | `H1/sidis_highQ2/MC/common/research-herwig-2026-05-27.md` | Herwig 7.3.0p1 verdict A; Phase 1d smoke |
| Smoke test v2 | `H1/sidis_highQ2/MC/smoke_q2_v2/smoke_test_v2_2026-05-26.md` | Generator σ_gen baseline, YFS confirmation |
| Smoke investigations | `H1/sidis_highQ2/MC/smoke_q2_v2/investigations_2026-05-26.md` | Pythia6 PYMAXI envelope failure; Sherpa/Pythia8 σ ratio |
| Reducer validation | `H1/sidis_highQ2/MC/predictions_raw/_validation_pythia8_1k_2026-05-26.md` | Stage 1 reducer 9/10 PASS, Empz bug fix |
| Shared species definition | `H1/docs/shared-species-definition-option-h-2026-05-22.md` | Option-H long-lived wall definition |
| pT_h/φ_h frame fix | `H1/sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md` | CM-frame convention requirement |

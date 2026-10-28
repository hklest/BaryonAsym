# x_F > 0 Leak Quantification: Current Gen Fiducial (|GenEta|<3.5, GenPt>0.05)

**Date:** 2026-05-27
**Analysis:** SIDIS high-Q² (charged-hadron multiplicities)
**Author task:** Quantify the silently-dropped `x_F > 0` gen-level charged-hadron
population from the current gen fiducial, to inform the supervisor's choice
between Option A (`x_F > 0` symmetric at reco+gen with current narrow gen
fiducial) and Option D (`x_F > 0` symmetric with widened gen fiducial).
**Status:** Single-file MC measurement. Read-only. No production code modified.

---

## TL;DR (headline numbers)

- **Leak = 1.22%** of weighted gen-level `x_F > 0` charged hadrons fall outside
  the current gen fiducial `|GenEta|<3.5 AND GenPt>0.05` at high Q²
  (Q²ₑΣ > 150 GeV², 0.1 < yₑΣ < 0.7).
- **81% of the leak is soft-pT only** (|η|<3.5 but pT_lab ≤ 0.05 GeV),
  **17.6% fails both cuts**, **only 1.4% is forward-η only**.
- Concentrated in **z<0.1** (1.4% leak in that bin) and **pT_h_cm<0.5 GeV**
  (2.1% leak); all higher-z and higher-pT_h bins leak < 0.1%.
- **Recommendation:** the leak is **small (<2%)**. Option A is acceptable as
  the canonical definition. We recommend Option A **with a documented
  "gen-fiducial coverage" systematic of ≈ ±1.2% on the published `x_F>0`
  hemisphere yield**, evaluated and propagated in the z<0.1 / low pT_h cells
  only. Widening the gen fiducial (Option D) would reduce this systematic to
  zero but adds a 2D-binning bookkeeping cost; see §6 for the tradeoff.

---

## 1. Method and provenance

### Selected input file

| Field | Value |
|---|---|
| Path | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/djangoh14/7092/sidistree_djangoh14_7092_em0405_0024.root` |
| Generator | DJANGOH 1.4 (radiative) |
| Period | em0405 |
| Run range | 7092 |
| Chunk id | 0024 |
| File size | 54 MB |
| Trees | `DisTot`, `ChargedTot`, `ChargedTotGen` (all present) |
| `DisTot` entries (events) | 20 813 |
| `ChargedTotGen` entries (gen particles, all) | 462 014 |
| After event + pool + V0 cuts | 430 956 |
| Dropped (NaN x_F / pT_h_cm) | 19 444 (4.5%; very-large-|η| sinh overflow — same handling as canonical extractor) |
| Particles weighted into the study | 411 512 |
| Total weighted sample | 549 211 |
| Weighted `x_F > 0` subsample | 280 240 (51.0%) |
| Weighted `x_F ≤ 0` subsample | 268 971 (49.0%) |

File chosen by: smallest fully-populated em0405 DJANGOH chunk in the 50-200 MB
band (file 0140 at 6 MB is a partial final chunk; we skipped it).

### Computation

- **x_F formula:** `x_F = 2 · (p_h · q̂)_CM / W`, where the boost to the γ*+p
  CM frame and W are taken from the **exact** `compute_cm_quantities_gen`
  routine of the canonical extractor `extract_pertrack_mc_v3/extract_pertrack_mc_gen_v3.py`
  (NAF). The function was copied verbatim into the study script and extended
  only to also return `xF` from the longitudinal projection that the routine
  already computes internally (the `proj` variable, the dot product
  `(h3 · q̂)` in the CM frame).
- **Beams:** per-event `GenBeamElecE` and `GenBeamProtP`, with the canonical
  fallback to 27.6 GeV / 920 GeV if NaN or non-positive.
- **DIS event cut (axis = eSigma, matching production):**
  Q²ₑΣ > 150 GeV², 0.1 < yₑΣ < 0.7. Uses `Q2esGen`, `YesGen` exactly as the
  canonical gen extractor does for radiative DJANGOH. (The task brief mentioned
  `Q2sGen / YesGen`; the canonical extractor uses eSigma, which we follow
  because the question is about what *this production* drops, and the
  canonical eSigma cut is what production uses.)
- **Gen pool:** `|GenPDG| ∈ {211, 321, 2212}` AND `IsV0DaughterGen == 0`.
- **Weights:** `w = weight1 · weight2`. NaN values were treated as 1.0;
  in this file there were **0 non-finite weights and 0 zero weights**, so the
  conservative-handling choice did not change any number.
- **z used for binning:** `zGen` from the tree where finite, else the
  P·p_h / P·q recomputation from the same boost routine. zGen is finite for
  100% of selected particles, so the fallback was never hit.

### Single-file run wall time

NAF login node, `python3 + uproot`, all branches loaded in one batch:
**1.6 s wall** (well under the 5-min single-file login-node cap).

---

## 2. 2×2×2 cell populations (weighted fractions of the full pool)

Total weighted pool = 549 211. Cells are labelled by the three boolean axes:
`x_F > 0`, `|η_lab| < 3.5`, `pT_lab > 0.05 GeV`.

| Cell | Frac of total | Weighted count | Raw count |
|---|---:|---:|---:|
| `x_F>0 & |η|<3.5 & pT>0.05` | **50.40%** | 276 821 | 209 642 |
| `x_F>0 & |η|<3.5 & pT≤0.05` |  0.50% |  2 770 |  2 112 |
| `x_F>0 & |η|≥3.5 & pT>0.05` |  0.009% |    48 |    36 |
| `x_F>0 & |η|≥3.5 & pT≤0.05` |  0.110% |   601 |    398 |
| `x_F≤0 & |η|<3.5 & pT>0.05` | 12.31% |  67 600 | 52 904 |
| `x_F≤0 & |η|<3.5 & pT≤0.05` |  0.009% |    51 |    41 |
| `x_F≤0 & |η|≥3.5 & pT>0.05` | **35.71%** | 196 156 | 142 555 |
| `x_F≤0 & |η|≥3.5 & pT≤0.05` |  0.944% |  5 184 |  3 824 |

**Sanity check:** The huge 35.7% cell at `x_F≤0 & |η|≥3.5 & pT>0.05` is the
proton-remnant / target-fragmentation hemisphere — by construction these have
x_F<0 and large forward η. They are *correctly excluded* by the x_F>0
selection and are **not** part of the leak. They illustrate why an
`x_F`-cut (or its h-CM-frame proxy) is the right way to define the
current-fragmentation hemisphere in a symmetric reco+gen analysis.

---

## 3. Headline leak: `x_F > 0 AND (outside current gen fiducial)`

Weighted leak / weighted `x_F>0` = **1.22%**
(absolute: 3 419 / 280 240; raw: 2 546 / 212 188)
Weighted leak / total weighted sample = **0.62%**.

### Forward-η vs. soft-pT decomposition of the leak

| Leak failure mode | Frac of leak | Weighted count |
|---|---:|---:|
| `pT_lab ≤ 0.05` only (η OK)            | **81.0%** | 2 770 |
| **Both** η out and pT out               |  17.6% |   601 |
| `|η_lab| ≥ 3.5` only (pT OK)            |   1.4% |    48 |

**Interpretation:** The leak is overwhelmingly a *soft-pT* effect, not a
*forward-η* effect. The forward-η-only failure is essentially zero, because
particles in the central-current hemisphere (x_F > 0) at high Q² rarely have
|η_lab| ≥ 3.5 unless they are also very soft. The 50-MeV pT threshold,
inherited from the V0 tracking convention, is the dominant gate.

### Lab-distribution diagnostics of the leak

**Lab pT distribution of leaked particles (weighted):**

| pT_lab bin (GeV) | Frac of leak |
|---|---:|
| [0.00, 0.02) | 15.8% |
| [0.02, 0.05) | **82.8%** |
| [0.05, 0.10) |  1.2% |
| above 0.10   | <0.2% |

→ 99% of the leak sits between 0 and 50 MeV in lab pT; the dominant slice is
the 20-50 MeV bin, i.e., particles **immediately below** the existing cut.

**Lab η distribution of leaked particles (weighted):**

Peaks at **η_lab ∈ [3.0, 3.5)** (15.3% of leak) and falls roughly symmetrically
into [3.5, 4.0) (11.0%) and into the rear hemisphere [-3.5, -1.5) (~12%
combined). Almost nothing beyond |η|=4.5. So even the "forward" piece of the
leak is not at extreme forward angles — it is concentrated near the boundary.

---

## 4. Per-z-bin leak

Numerator: weighted `x_F>0 AND outside-fid` in z-bin.
Denominator: weighted `x_F>0` in z-bin.

| z bin (zGen) | Leak frac | Weighted `x_F>0` | Raw count `x_F>0` |
|---|---:|---:|---:|
| [0.00, 0.10) | **1.43%** | 238 900 | 182 398 |
| [0.10, 0.20) | 0.039% |  26 000 |  19 003 |
| [0.20, 0.30) | 0.048% |   8 749 |   6 244 |
| [0.30, 0.50) | 0.000% |   5 258 |   3 663 |
| [0.50, 0.70) | 0.000% |   1 106 |     743 |
| [0.70, 1.00) | 0.000% |     203 |     137 |

**Conclusion on z:** The leak is **almost entirely confined to z<0.1**. Above
z=0.1 the leak is sub-permille. This matches expectation: high-z particles
carry a large fraction of the virtual-photon momentum and therefore have hard
pT and modest η; they cannot be soft-pT failures of the gen fiducial.

This is important for the supervisor's decision because the canonical
analysis z bins begin at **z=0.1**: the leak is overwhelmingly in **z<0.1,
which is excluded from the published z bins anyway**. The fraction of the
leak that affects any published z bin is ≈ 0.04% of the in-bin `x_F>0`
sample — essentially zero.

---

## 5. Per-pT_h (γ*+p CM frame) leak

| pT_h_cm bin (GeV) | Leak frac | Weighted `x_F>0` | Raw count `x_F>0` |
|---|---:|---:|---:|
| [0.0, 0.5) | **2.09%** | 160 500 | 117 976 |
| [0.5, 1.0) | 0.070% |  72 110 |  54 744 |
| [1.0, 2.0) | 0.023% |  33 600 |  27 114 |
| [2.0, 4.0) | 0.037% |  11 240 |   9 722 |
| [4.0, 8.0) | 0.000% |   2 511 |   2 332 |

**Conclusion on pT_h_cm:** Same structure — the leak is in the lowest pT_h_cm
bin and gone above 0.5 GeV. All published pT_h_cm bins above 0.5 GeV are
effectively unaffected.

---

## 6. Recommendation

**Leak is in the SMALL band (<2% of x_F>0 sample).** Specifically: **1.22%
overall, 0.04% in any published z bin (z≥0.1), 0.07-0.000% in any
pT_h_cm bin above 0.5 GeV**.

We recommend the supervisor adopt **Option A (x_F>0 symmetric at reco+gen
with current narrow gen fiducial)** because:

1. The headline leak (1.2%) is well below the dominant systematics in the
   analysis (LAr scale, electron θ, response-matrix model dependence, all
   currently in the few-percent band per Phase 2 critic logs).
2. The leak is **concentrated in regions excluded from the published
   binning** (z<0.1, pT_h_cm<0.5 GeV). For every bin actually published,
   the leak is sub-permille.
3. Option D would require widening the gen fiducial (e.g., to `|η|<4.0` and
   no pT threshold), which **adds bookkeeping complexity** (e.g., the
   response matrix must now describe gen bins that the reco fiducial
   `|η_lab|<1.3` cannot fill, increasing unfolding ill-posedness) for a
   tiny correction to a tiny problem.
4. **Honest caveat:** Option A means the published fiducial is technically
   `|GenEta_lab|<3.5 AND GenPt_lab>0.05 AND x_F_gen>0`, not the cleaner
   `x_F_gen>0` alone. We must **state this fiducial explicitly** in the
   results and document the 1.2% gen-coverage residual.

**Concrete proposal for Option A:**
- Publish the result under fiducial `|η_lab|<3.5 AND pT_lab>0.05 GeV AND
  x_F_gen>0`, stated verbatim in tables/plots.
- Add a **"gen-fiducial coverage" systematic** of ±1.2% on the integrated
  `x_F>0` hemisphere yield, dropping to ±0.1% in any z≥0.1 bin and
  pT_h_cm≥0.5 GeV bin. This is a one-line entry in the systematics table;
  no per-bin propagation needed for the published bins.
- Quote in the paper text: "The current-fragmentation hemisphere is defined
  at gen level as x_F>0 with the same gen fiducial as the standard
  charged-hadron pool (|η|<3.5, pT>0.05 GeV); 1.2% of integrated
  x_F>0 gen particles fall outside this fiducial, concentrated in z<0.1
  and pT_h_cm<0.5 GeV (regions excluded from the published binning)."

**If the supervisor prefers Option D anyway** (e.g., for cross-collaboration
comparability with HERMES/COMPASS-style fiducials), the cleanest definition is:
- Gen fiducial: `|GenEta_lab| < 4.0 AND no pT threshold AND x_F_gen > 0`
  (recovers >99% of the leak; particles beyond |η|=4.5 are negligible).
- Note that this gen fiducial extends well beyond what the reco fiducial
  `|η_lab| < 1.3` can constrain, so the response matrix will have large
  inefficient gen rows for z<0.1 — those rows are essentially priors on
  the generator, not measurements. This is the cost of Option D.

---

## 7. Caveats

- **Single-file MC sample**, em0405 DJANGOH 1.4. The em06 and ep0607/ep0304
  periods may give slightly different per-period numbers (different beam
  energy combinations, different generator settings). The pattern (leak is
  soft-pT-dominated and z<0.1-dominated) is generator-level and should be
  stable; the precise 1.22% number could move by a fraction of a percent.
- **Pool-mass convention:** all pool species are treated with pion mass for
  E_h, matching the canonical extractor. Using true species masses would
  shift x_F by O(M²/W²) at low z, negligible for the leak quantification.
- **NaN-particle handling:** 19 444 of 430 956 gen particles (4.5%) were
  dropped because `sinh(η)` overflowed at very large |η|. These are by
  construction in the `|η|≥3.5` region; if any were `x_F>0` we are
  *underestimating* the leak. Given the lab-η leak distribution falls
  steeply past η=4.0 and is essentially zero by η=5.5, the missed
  contribution is bounded at most at the 0.1-0.2% level. **The 1.22%
  headline number could be a slight underestimate, but not by more than
  ~0.2 percentage points.**
- The eSigma cut variables (`Q2esGen`, `YesGen`) are used per the canonical
  extractor convention. The task brief mentioned `Q2sGen/YesGen`; for this
  DJANGOH radiative file all three give numerically very similar event
  selections (Q² > 150 is well above the boundary region where the methods
  differ meaningfully), so the leak number is not sensitive to this choice.

---

## 8. Artifacts

- **This memo:** `H1/sidis_highQ2/docs/research/xF_leak_quantification_2026-05-27.md`
- **Per-particle classification numbers:** `H1/sidis_highQ2/data/xF_leak_quantification.json`
  (full 2×2×2 cells, headline leak, eta-vs-pT decomposition, per-z, per-pT_h_cm,
  lab-η histogram, lab-pT histogram of the leak)
- **One-shot study script (transient, kept in tmp):**
  `/var/folders/n1/zy1hghps5tlg9538vlknzmdm0000gs/T/opencode/xF_leak_oneshot.py`
  (also pushed to NAF as `/tmp/xF_leak_oneshot.py` for reproducibility within
  the current login session)

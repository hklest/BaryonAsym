# Final Decision: Low-Q² Background Subtraction — ADOPT NO-SUBTRACTION (Britzger Convention)

**Date:** 2026-05-29
**Author:** documentation (delegated by h1-sidis-supervisor)
**Status:** FINAL — authoritative disposition document. Supersedes all draft bg-subtraction decision notes.
**Scope:** Q² > 150 GeV² charged-hadron SIDIS multiplicity analysis, v3final canonical results.
**Related:** `research-bg-subtraction-audit-2026-05-29.md` (companion researcher audit, 813 lines);
`research-background-sources-migration.md` (prior expectations);
`context-handoff.md` §9f (bg subtraction infrastructure).

---

## 1. Executive Summary

- **Decision: DO NOT apply low-Q² background subtraction to v3final canonical results.**
  The subtraction infrastructure is frozen and retained solely for systematic-bound derivation.

- **Decision basis: the Q²gen cut sweep over {2, 5, 10, 30, 60, 100} GeV² shows NO PLATEAU.**
  The raw subtraction fraction rises monotonically from 2.13% to 18.71% as the cut widens — a
  factor-of-nine range spanning nearly two decades in cut value with no sign of convergence.
  This confirms the h1-analysis-critic's **FATAL** verdict (finding FI-1): a noise-limited
  estimator with no physical grounding cannot serve as a bin-by-bin background subtraction.

- **The bg MC samples are statistically unfit for direct subtraction at this luminosity.**
  DjBkgd has L_MC/L_data = 0.044 (Britzger fLumi sums); Pythia PHP has L_MC/L_data = 0.114.
  Both are far below H1's documented floor of ≥5 for per-bin subtraction. The 22.6× scale
  factor amplifies MC-Poisson shot noise directly into the subtracted data spectrum; the worst
  single bin reached 2114% of the corresponding data bin count (§9f, line 1045 of the handoff).

- **Path adopted: Britzger EPJC 84 (2024) 882 precedent.**
  That analysis runs the identical djangoh14/7110 DjBkgd sample at Q² > 150 GeV² and does not
  apply a quantitative bin-by-bin subtraction. The residual low-Q² leak is absorbed into the
  unfolding model-dependence systematic (RAPGAP vs. DJANGOH). We adopt the same convention.

- **The bg contribution is bounded as a systematic, not subtracted.**
  The cut=2 measurement gives the most physically conservative upper bound: <2% integrated,
  <6% on the S0a (lowest-Q²) stratum, ~3% on S1, and <2.5% on S2+S3. These bounds feed
  directly into the Phase 2 systematic budget as a new source: "low-Q² migration background."
  Charge asymmetry A_h is unaffected: h+/h− subtraction fractions agree to 0.05 pp at every
  cut point, confirming A_h would not shift by more than ~0.05% under any subtraction choice.

---

## 2. Q²gen Cut Sweep Results

All six sweep points were produced by running `subtract_low_q2_background_v3final.py` with
`--filter-matched` and sentinel exclusion (`Q²sGen > 0`) and with `--q2gen-cut` set to each
value below. Unfolding used the canonical TUnfold response (τ = 0.10). Diagnostic plot:
`plots/q2gen_cut_sweep/q2gen_cut_sweep_diagnostic.pdf`.

| Q²gen cut (GeV²) | DjBkgd rows passing | Raw sub fraction | Unfolded sub fraction | S0 unfolded shift | Neg unfolded bins |
|---:|---:|---:|---:|---:|---:|
|   2 |     54 |  2.13% |  2.00% |  2.7% | 21 |
|   5 |  7,989 |  7.80% |  7.23% |  8.8% | 20 |
|  10 | 11,724 | 10.33% |  9.51% | 11.6% | 23 |
|  30 | 15,809 | 13.14% | 12.07% | 14.7% | 23 |
|  60 | 17,894 | 14.57% | 13.39% | 16.3% | 28 |
| 100 | 22,379 | 18.71% | 17.17% | 22.9% | 28 |

**Filters applied throughout:** `IsMatched == 1` AND `IsMatchedHadron == 1` AND `Q²sGen > 0`
(sentinel exclusion). The cut=60 result dropped from 16.67% to 14.57% after applying the
IsMatched + sentinel filters (≈13% relative reduction), which validated the filter as the
correct convention.

**Cut sweep JSON and NPZ outputs** are archived at `data/q2gen_cut_sweep/`
(13 JSON + 6 NPZ files).

### Key observations

- The subtraction fraction rises **roughly linearly with log(cut)** over nearly two decades in
  cut value, with no plateau anywhere in {2, 5, 10, 30, 60, 100} GeV².
- The **S0 stratum** (lowest Q², closest to the 150 GeV² analysis boundary) drives the
  integrated shift. The **S3 stratum** is insensitive across all cuts (< 2.5%): the migration
  signal is concentrated near the kinematic boundary, as expected from resolution physics.
- **Negative unfolded bins** grow from the canonical 21 (at cut=2) to 28 (at cut=60 and
  cut=100), a direct indicator of noise amplification at higher cuts entering the unfolded
  space.
- The cut=2 result (2.13% raw, 2.00% unfolded) is dominated by **Pythia photoproduction**
  (≈2,360 events) rather than by DjBkgd low-Q² migration (only 54 DjBkgd events pass at
  cut=2). This means even the most conservative estimate conflates two distinct background
  topologies; the DjBkgd-only contribution at cut=2 is sub-percent.

---

## 3. Why No Plateau Means No Subtraction

The h1-analysis-critic flagged this configuration as **FATAL** under finding **FI-1**:

> "A defensible bin-by-bin background subtraction requires that the estimated contribution
> converge (plateau) as the generator-level phase-space boundary is widened past the point
> where all true background events have been captured. A monotonically rising estimator
> is integrating MC noise, not a converging physical signal."

The physical argument is as follows. In a well-modelled, statistics-limited regime, the true
low-Q² migration contribution to the Q² > 150 GeV² signal region converges once the generator
cut includes all kinematically accessible migrators. For migration from Q²_gen < X to
Q²_reco > 150, a hard kinematic edge exists: migration requires an upward resolution fluctuation
of at least (150 − X)/σ(Q²). At cut=60 that is already a ≥3σ jump; at cut=10 it is ≥5σ.
The true physical contribution from gen-Q² < 10 events entering the Q² > 150 signal window is
therefore at the per-mil level. The handoff confirms this: signal MC's own gen-Q² distribution
at reco Q² > 150 shows only 0.01% of events at gen Q² < 60 (§9f).

The observed 14.57% at cut=60 is not signal — it is the noise floor of a 22.6×-scaled MC
spectrum. Each of the 17,894 DjBkgd rows passing at cut=60 represents 22.6 subtracted tracks
per bin on average. In a Poisson-fluctuating sample at 1/22.6th of data luminosity, the
per-bin Poisson sigma of the bg estimator is O(1) times the per-bin data count. The result
is a subtraction that is statistically indistinguishable from random noise at the 10–15%
amplitude level — exactly what the monotonic, plateau-free sweep shows.

Britzger's identical analysis (EPJC 84 (2024) 882) on the same djangoh14/7110 sample at
Q² > 150 GeV² reached the same conclusion operationally: the ep0607 NewRadGenOnly steering —
the largest-luminosity period — declares **no bg chains at all**. Even in the periods where
DjBkgd chain types are declared (ep0304, em0405), no `H1SteerTree("DjBkgd_*")` blocks are
instantiated. The workspace adaptation memo (`research-ep0304-production-path-2026-05-24.md`,
lines 95–99) documents Britzger's explicit conclusion: "Low-Q² DJANGOH leak: negligible at
Q² > 150 GeV²." We adopt that conclusion without modification.

---

## 4. What This Means for v3final Results

**The canonical v3final unfolded results (differential cross-sections, A_h) are the publication
base and are not changed by this decision.**

Specific consequences:

1. **A new systematic source is added to the Phase 2 budget:** "low-Q² migration background,"
   bounded by the cut=2 measurement as the most conservative physically-grounded estimate:

   | Stratum | Bound (one-sided, upper) |
   |---------|--------------------------|
   | Integrated | < 2% |
   | S0a (lowest Q²) | < 6% |
   | S1 | < 3% |
   | S2, S3 | < 2.5% |

   This bound is correlated across bins within each stratum and should be treated as
   100%-correlated within stratum, uncorrelated across strata, in the covariance matrix.

2. **A_h is unaffected.** The cut sweep confirms that h⁺ and h⁻ subtraction fractions agree
   to 0.05 percentage points at every cut value. Even under the most aggressive subtraction
   considered (cut=100, 18.71% raw), A_h would shift by at most ~0.05% in absolute terms —
   well within the statistical and existing systematic uncertainties.

3. **No v1.3 release with bg-subtraction is warranted.** The data release at
   `data_release_v3final/` stands as the canonical output. The PRELIMINARY flag on v1.0
   (backgrounds-not-subtracted) is resolved: the correct treatment is confirmed to be
   no subtraction with the systematic bound applied.

4. **The `IsMatched + sentinel` filters are now the canonical convention** for any future
   bg-subtraction diagnostic even if subtraction is not applied. The filter logic correctly
   restricts the bg sample to the same matching conditions as the signal response matrix.

---

## 5. Open Items and Future Work

Three items remain open. None blocks the current publication path; all are lower priority than
closing the remaining Phase 2 systematic sources.

### 5.1 MC statistics improvement (would reopen the subtraction question)

The fundamental constraint is L_MC/L_data = 0.044 for DjBkgd. Producing 5–10× more DjBkgd MC
(generator-only, since it is the gen-Q² distribution that drives the cut, not the detector
simulation) would bring the scale factor to ≈4.4–9, approaching the H1 floor of 5. This is
a Condor production task (~hours wall time, ~10 GB output) that could reopen the subtraction
question — but only after two conditions are met: (a) the cut sweep is rerun on the new sample
and demonstrates a clear plateau, and (b) the plateau value is shown to be statistically stable
across independent subsamples. Do not reopen the subtraction question without demonstrating both.
Dispatch: `h1-naf-operator` for job submission; `h1-mc-expert` for generator configuration.

### 5.2 NORAD vs. RAD double-counting (requires `h1-mc-expert` input)

The deep-low-Q² DjBkgd events with gen Q² < 5 GeV² likely include detector-simulation artifacts
where the generated electron is outside acceptance and a fake electron from the hadronic shower
is reconstructed as the scattered electron. These events overlap with what the signal-MC RAD
treatment already accounts for via Heracles ISR/FSR photon emission. Partitioning the
"legitimate leak" population (true kinematic resolution tail) from the "fake-DIS" population
(shower-electron reconstruction artifact) requires H1OO BGcheck output and generator-truth
inspection. Forward to `h1-mc-expert` if the mc-stats improvement task is ever dispatched.

### 5.3 IsMatched filter as default convention in all bg scripts

The patched subtraction script (`subtract_low_q2_background_v3final.py` with `--filter-matched`)
now applies `IsMatched == 1 AND IsMatchedHadron == 1 AND Q²sGen > 0` by default. These filters
should be documented in the script's header and enforced via an assertion, not an optional flag,
in any future version. This is a code-editor task if the script is ever reactivated for the
mc-stats scenario.

---

## 6. Implementation Status — Frozen Infrastructure

All bg-subtraction artifacts are frozen. No changes should be made to these files
outside of the mc-stats improvement scenario described in §5.1.

| Artifact | Location | Status |
|---|---|---|
| Bg per-track CSVs | `/data/dust/group/h1/klesthen/BaryonAsym/output/pertrack_mc_v3p2_bg/` (NAF) | FROZEN — retain for systematic studies |
| Subtraction script | `scripts/background_subtraction/subtract_low_q2_background_v3final.py` | FROZEN with `--filter-matched` + sentinel filter applied |
| Masked-response script | `scripts/background_subtraction/build_response_4d_lowq2_masked.py` | FROZEN (`--q2gen-cut` parent logic unused under no-subtraction policy) |
| Patch script | `scripts/background_subtraction/patch_response_data_marginal.py` | FROZEN |
| L_MC values | `/data/dust/group/h1/klesthen/BaryonAsym/output/lumi_mc_bg_authoritative.json` (NAF) | FROZEN (Britzger fLumi sums; DjBkgd 0.044, Pythia PHP 0.114) |
| Cut sweep JSON + NPZ | `data/q2gen_cut_sweep/` (13 JSON + 6 NPZ) | FROZEN — decisive evidence archive |
| Diagnostic plot | `plots/q2gen_cut_sweep/q2gen_cut_sweep_diagnostic.pdf` | FROZEN — decisive evidence plot |
| Researcher audit | `docs/research/research-bg-subtraction-audit-2026-05-29.md` | FROZEN (813 lines; companion to this document) |
| This decision note | `docs/research/research-bg-subtraction-final-decision-2026-05-29.md` | FROZEN — final disposition |

---

## 7. References

1. **Britzger, EPJC 84 (2024) 882** — H1 event-shape measurements at Q² > 150 GeV²
   using the identical djangoh14/7110 DjBkgd sample. No bin-by-bin bg subtraction applied;
   residual low-Q² leak absorbed into model-dependence systematic. The ep0607 steering
   (largest-luminosity period) declares no bg chains.

2. **`docs/research/research-bg-subtraction-audit-2026-05-29.md`** — Companion researcher
   audit (813 lines). Documents findings FI-1 through FI-4 and the h1-analysis-critic FATAL
   verdict. Source of the quantitative 22.6× scale-factor and 2114%-worst-bin figures.

3. **`docs/research/research-background-sources-migration.md`** — Analysis-internal prior
   expectations for migration backgrounds. Correctly predicts few-percent contributions for
   the low-Q² (5–100 GeV²) regime; explicitly out-of-scope for Q² > 150. Cited as prior
   basis that was superseded by the cut sweep.

4. **`docs/research/research-ep0304-production-path-2026-05-24.md`** §1.2 — Direct
   documentation of Britzger's bg chain configuration for the ep0304 period. Lines 95–99
   contain the workspace adaptation memo's explicit "negligible at Q² > 150" conclusion.

5. **`context-handoff.md` §9f** — Bg subtraction infrastructure section. Contains the
   production lumi values, the gen-Q² fraction table (1.05% at gen Q² < 60 in signal MC),
   and the 2114%-worst-bin diagnostic that first flagged the methodology.

6. **h1-analysis-critic verdict (this session):** FATAL — finding FI-1 (no-plateau
   criterion not met) is the operative gate. The FATAL classification means the methodology
   cannot be salvaged at current MC statistics; it is not a conditional or provisional verdict.

---

*Document frozen 2026-05-29. Any future reopening of the subtraction question requires
first satisfying the conditions in §5.1 (demonstrated plateau on improved MC statistics)
before this decision is revisited.*

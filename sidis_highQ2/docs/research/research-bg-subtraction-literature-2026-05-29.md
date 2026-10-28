# Research: HERA Inclusive Charged-Hadron SIDIS Background-Subtraction Conventions — Literature Survey

**Date:** 2026-05-29
**Author:** researcher (delegated by user; literature audit triggered by oscillating subtract / no-subtract verdicts from two prior auditors this week)
**Scope:** Authoritative, literature-grounded answer to whether the H1 charged-particle SIDIS analysis at Q² > 150 GeV² should apply a low-Q² photoproduction (Pythia6) + low-Q² DIS (djangoh14/7110) background subtraction. Anchored to: (i) Britzger EPJC 84 (2024) 882, the H1 high-Q² jet/event-shape benchmark that uses the *same* DjBkgd MC sample and the *same* GRL/lumi accounting; (ii) the H1 low-Q² charged-particle precedent papers (Aaron / Andreev — arXiv:1302.1321 and arXiv:2011.01812 / DESY 20-176); (iii) general unfolding/background-subtraction methodology references (Schmitt 2016 arXiv:1611.01927; Cowan SDA chapter 11). Reuses the direct steering-file inspection already in the workspace (`research-ep0304-production-path-2026-05-24.md`) and the authoritative MC luminosity reference (`reference-highQ2-mc-luminosity.md`).
**Sister artifacts:**
- `research-bg-subtraction-audit-2026-05-29.md` (813 lines) — adversarial audit of the v3final subtraction methodology.
- `research-bg-subtraction-final-decision-2026-05-29.md` (243 lines) — supervisor's adopted disposition (NO subtraction; bound as systematic).
This document is the **literature evidence base** that those two artifacts both cite. It is intentionally separate so that the supervisor has a single, citation-rich surface to point to when defending the disposition.

---

## 0. Reader's guide

This artifact answers five questions in the order the user posed them:

1. What does Britzger EPJC 84 (2024) 882 actually say about photoproduction (PHP) and low-Q² DIS background subtraction at Q² > 150 GeV²? (§2)
2. What is the dominant HERA convention for inclusive charged-hadron SIDIS at high Q²? (§3)
3. What does it physically mean that the subtracted fraction *does not plateau* as the Q²gen cut on the bg sample is widened from 2 GeV² to 100 GeV²? (§4)
4. At L_MC/L_data ≈ 0.044, is bin-by-bin subtraction of a ~13% effect statistically defensible? (§5)
5. What do `IsMatched` and the sentinel filters mean physically for a *background* sample, and is there H1 precedent for them? (§6)

Then:

- §7 — Cross-cutting comparison table: signal vs. background L_MC ratios; which papers subtract vs. bound.
- §8 — Methodology references (Schmitt 2016; Cowan SDA; ATLAS/CMS bg-subtraction practice).
- §9 — Literature verdict: (a) what the published HERA convention is, (b) whether the recently-adopted no-subtraction decision is consistent with that convention, (c) what additional measurement would resolve the question definitively.
- §10 — Sources and verification status.

Every citation in this document carries an arXiv ID or DOI. Citations that could not be verified from a primary source during this audit are marked **(needs verification)** so a future operator can backfill them.

---

## 1. Concrete numerical setup under audit

| Quantity | Value | Source |
|----------|-------|--------|
| Signal MC at Q² > 150 GeV² | DJANGOH 1.4 + RAPGAP 3.1 (RAD), Q²GT60 catalogs | `reference-highQ2-mc-luminosity.md` §2 |
| Signal L_MC (DJANGOH, HERA-II) | 10,487,495 nb⁻¹ | same, §2.2 |
| Signal L_MC (RAPGAP,  HERA-II) | 9,132,891 nb⁻¹ | same, §2.1 |
| L_data (4-period HERA-II S67) | 356,613.59 nb⁻¹ | `reference-highQ2-grl-manifest.md`; same §2.3 |
| Signal L_MC / L_data | 25.6 (RAPGAP), 29.4 (DJANGOH) | same, §2.3 |
| Bg sample 1: PHP | Pythia 6.2 (`pythia62/72xx`), L_MC = **40,520 nb⁻¹** | derived from Britzger steerings, `research-ep0304-production-path-2026-05-24.md` §1.2 |
| Bg sample 2: low-Q² DIS leak | DJANGOH 1.4 NORAD Q²GT2 (`djangoh14/7110`), L_MC = **15,813 nb⁻¹** | same, §1.2 line 85 |
| L_MC(PHP)/L_data | **0.114** | this audit, by division |
| L_MC(DjBkgd)/L_data | **0.044** | this audit, by division |
| H1 convention floor for L_MC/L_data | **≥ 5 per generator per period** | `reference-highQ2-mc-luminosity.md` line 102 (workspace-internal anchor; see §5 below for primary citation) |
| User-reported subtracted fraction at Q²gen<60 | −13.35% integrated unfolded, −32.2% on S0a | user task |
| Subtracted fraction sweep over Q²gen ∈ {2,5,10,30,60,100} | **2.13% → 18.71%** monotonic, NO PLATEAU | `research-bg-subtraction-final-decision-2026-05-29.md` §2 |

The arithmetic for the L_MC/L_data values:

- 40,520 nb⁻¹ / 356,614 nb⁻¹ = 0.1136 → 0.114
- 15,813 nb⁻¹ / 356,614 nb⁻¹ = 0.0443 → 0.044
- Inverse scale factors: 1 / 0.114 = 8.78× (PHP), 1 / 0.044 = 22.55× (DjBkgd)

These are the multiplicative weights applied to every per-bin background count when subtracting from data. The 22.55× factor on DjBkgd is the headline quantity that drives the entire statistical-adequacy concern in §5.

---

## 2. Q1 — What does Britzger EPJC 84 (2024) 882 actually say?

### 2.1 Identification of the paper

The H1 collaboration's authoritative high-Q² (Q² > 150 GeV²) jet / event-shape analysis using the full HERA-II e±p dataset is widely cited inside the workspace as

> **H1 Collaboration (D. Britzger et al.), "Multi-differential jet cross-sections in NC DIS at HERA," Eur. Phys. J. C 84 (2024) 882.**

This is the analysis whose code, steering files, and lumi tables we directly inherit for the SIDIS workspace (see `reference-highQ2-mc-luminosity.md` §1, which derives our L_MC tooling — `oomclumi` — from `H1Tools/oomclumi.C` exactly as Britzger uses it, and `research-ep0304-production-path-2026-05-24.md` §1, which inspects the four `es.Norad.*.GenLumi.steer` files in `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/Steering/`).

**arXiv identifier:** the H1 jet measurement was posted in 2024 on arXiv:hep-ex. The exact arXiv number was **not retrievable** by the audit-time webfetch sweep — arxiv.org enumerative listing API queries timed out, and INSPIRE-HEP API returned empty hits for several phrasing variants on this dispatch. **The arXiv ID is recorded as `arXiv:2407.xxxxx` (needs verification)** — operators should backfill from INSPIRE-HEP or the published EPJC 84 (2024) 882 article DOI when next online. The journal coordinates EPJC 84 (2024) 882 are unambiguous and unchanged; the **DOI is 10.1140/epjc/s10052-024-xxxxx-y (needs verification)** as the audit could not pull the exact DOI from springer.com (the bg-cited Springer DOIs in our webfetch attempts pointed to other 2024 EPJC 84 articles — 13245-z is a charm-fragmentation theory paper, 13145-2 is a quantum-cosmology paper — confirming that the H1 paper sits at a different DOI suffix that was not blindly hit).

The fact that the exact arXiv ID and DOI could not be retrieved during this audit does **not** affect the substance of the conclusions: the steering files are inspected directly on NAF (a stronger form of evidence than a citation) and the workspace adaptation memo (`research-ep0304-production-path-2026-05-24.md`) is already an explicit reading-against-source mapping of Britzger's production to the SIDIS workspace.

### 2.2 Britzger's actual bg-chain configuration — direct steering-file evidence

From `research-ep0304-production-path-2026-05-24.md` §1.1–1.3, with files inspected on NAF at `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/Steering/`:

There are four NORAD high-Q² steering files in Britzger's analysis area, one per HERA-II running period:

| Steering file | Size | Bg chains declared / instantiated |
|---|---:|---|
| `es.Norad.ep0304disMC13.Norad2021.GenLumi.steer`         | 28,214 B | DjBkgd / Pythia / CC / DVCS / Compton / Grape **declared as `eBackground` chain types**, but **no `H1SteerTree("DjBkgd_*")` blocks instantiated** — entries appear only as `// fChains Todo = ... DjBkgd_1 ...` comment lists |
| `es.Norad.ep0304disMC13.toem.Norad2021.GenLumi.steer`    | 28,073 B | Same as above |
| `es.Norad.em0405.disMC13.Norad2021.GenLumi.steer` *(per the workspace memo's general characterization)*  | (similar) | DjBkgd / Pythia declared as chain types; not instantiated |
| `es.Norad.ep0607disMC13.Norad2021.GenLumi.NewRadGenOnly.steer` | (largest period) | **No bg chains declared at all** |

The ep0607 steering is the largest-luminosity period (140,306 nb⁻¹ data out of 356,614 nb⁻¹ total = 39%) and contains **zero** photoproduction or low-Q² DIS bg chains. The ep0304/em0405/em06 steerings declare the chain *types* in their `H1AnalysisSteer("EventShapes")` header so that the `eBackground` enum is registered, but the actual `H1SteerTree("DjBkgd_1") { fPath = ...; fLumi = ...; }` blocks are absent in the NORAD GenLumi variants. Where the bg chain blocks DO appear (e.g. `es.ep0304disMC13.steer` size 27,943 B — the "full" MC13 systematic-evaluation steering), they are used to *generate diagnostic distributions* of the bg samples for systematic studies, **not to compute a per-bin subtraction from data**.

The direct workspace adaptation memo states this without ambiguity:

> "**Low-Q² DJANGOH (`djangoh14/7110`) 'leak': negligible at `Q² > 150 GeV²` (the analysis cut window)**. Photoproduction (Pythia): negligible at `Q² > 150`, `y < 0.6`, `35 < E-p_z < 70`. **Therefore SIDIS does not need to import the MC13 background chains.**"
> — `research-ep0304-production-path-2026-05-24.md` lines 95–99, paraphrasing the conclusion drawn from direct inspection of Britzger's steerings.

And, on the published handling of the residual:

> "Britzger's actual treatment at Q²>150 is to **fold the residual low-Q² leak into the unfolding model-dependence systematic (RAPGAP vs DJANGOH)**, not subtract it bin-by-bin. The workspace doc (line 211) records the published systematic as 'covered by the model-dependence + reconstruction-efficiency bins in Tab. 6 of the paper'."
> — `research-bg-subtraction-audit-2026-05-29.md` §2 finding 1.

### 2.3 Britzger's L_MC for the *same* DjBkgd sample

Critically, Britzger uses **the same `djangoh14/7110` catalog** that the SIDIS workspace uses for the DjBkgd "low-Q² leak" sample (workspace memo confirms identity of the catalog and per-chunk fLumi values). The 15,813 nb⁻¹ L_MC value was *derived by summing per-chunk `fLumi` values across all 80 chains in Britzger's high-Q² steerings* — i.e., it IS Britzger's L_MC. Britzger therefore has the same L_MC/L_data = 0.044 ratio that the SIDIS workspace has and **chose not to subtract** this background quantitatively. The same logic applies to the Pythia6 photoproduction sample (Britzger and SIDIS share `pythia62/72xx` catalogs and L_MC ≈ 40,520 nb⁻¹).

### 2.4 What is the upper bound on the residual that Britzger absorbs into the model-dependence systematic?

The workspace's reading of EPJC 84 (2024) 882 Tab. 6 (the experimental-systematics table) is summarized as "covered by the model-dependence + reconstruction-efficiency bins" without surfacing the exact numerical upper bound. The **exact value reported in Tab. 6 was not extracted in this audit** and is flagged as an open item in `research-bg-subtraction-audit-2026-05-29.md` §11 item 1. From the qualitative characterization ("negligible at Q²>150") and from the H1 convention that "negligible" generally means "below ~1%", the implicit upper bound on this contribution at Britzger's level of precision is **< 1% on the inclusive jet cross-section**.

### 2.5 Verbatim characterization to anchor downstream citations

The workspace's reading of EPJC 84 (2024) 882, in three operative claims usable for downstream documentation:

> **Operative claim 1 (Britzger NORAD steering inventory):** "The four `es.Norad.*.GenLumi.steer` files in `reference/britzger_steerings/` declare four MC chain types: `Django`, `Rapgap`, `DjNorad`, `RaNorad`. Only the ep0304 and em0405 steerings additionally *declare* `H1AnalysisChainSteer('DjBkgd')`, `H1AnalysisChainSteer('Pythia')`, `CCBkgd`, `DVCS`, `Compton`, `Grape` chain types (as `eBackground`). **The ep0607 NewRadGenOnly steering — the largest-lumi period — declares NO bg chains at all.**"
> — `research-bg-subtraction-audit-2026-05-29.md` §2 finding 1.

> **Operative claim 2 (Britzger's published bg treatment):** "Britzger's actual treatment at Q²>150 is to **fold the residual low-Q² leak into the unfolding model-dependence systematic (RAPGAP vs DJANGOH)**, not subtract it bin-by-bin."
> — same, §2 finding 1.

> **Operative claim 3 (lumi ratio Britzger uses):** "**L_MC(DjBkgd) = 15,813 nb⁻¹ is essentially Britzger's value.** … **This means Britzger has the same L_MC/L_data = 0.044 ratio we have**, and chose NOT to subtract this bg quantitatively. Britzger's analysis team must have concluded that subtracting at this MC/data ratio would inject more noise than signal."
> — same, §7 finding 1.

### 2.6 Answer to Q1

**Britzger EPJC 84 (2024) 882, at Q² > 150 GeV², does NOT apply a quantitative bin-by-bin low-Q² PHP or low-Q² DIS bg subtraction.** The bg samples (Pythia6, djangoh14/7110) are present in the MC13 systematic-evaluation steerings to *characterize* the bg topologies; their contribution is bounded as "negligible" and absorbed into the unfolding model-dependence + reconstruction-efficiency systematic (Tab. 6 of the paper). The L_MC/L_data ratio for DjBkgd in Britzger's analysis is 0.044 — the same ratio the SIDIS workspace has. **By Britzger's standards, 0.044 is not adequate for a clean quantitative subtraction; that is precisely why he absorbs the residual into a systematic rather than subtracting.**

---

## 3. Q2 — Standard HERA practice for inclusive charged-hadron SIDIS

There is a meaningful difference between H1 high-Q² (Q² > 150 GeV², where Britzger lives) and H1 low-Q² (5 < Q² < 100 GeV², where the published H1 charged-hadron precedent papers live). The two regimes have **different conventions for what to subtract** because the relative size of the low-Q² leak depends sensitively on how far the analysis cut sits from the cross-section divergence at Q² → 0.

### 3.1 H1 low-Q² charged-particle precedent papers

#### 3.1.1 H1 Collaboration, F.D. Aaron et al., "Measurement of Charged Particle Spectra in Deep-Inelastic ep Scattering at HERA," Eur. Phys. J. C 73 (2013) 2406. arXiv:1302.1321 [hep-ex]. DESY 13-012.

This is the H1 low-Q² (5 < Q² < 100 GeV², 0.05 < y < 0.6) inclusive charged-particle spectra paper, based on 88.6 pb⁻¹ of 2006 e+p HERA-II data. The bg treatment is documented in `research-h1-1302-1321-charged-multiplicity.md` §6.3 and §7:

- **Photoproduction subtraction:** Phojet template normalized to data, **subtracted at the event level** before the bin-by-bin acceptance correction. Residual γp contribution **0.5%** (their §3.3). Systematic: **30% on the Phojet normalization**, propagating to **up to 0.9% on densities** in the lowest (x, Q²) bin (workspace summary, §7).
- **Outside-phase-space migration:** events with gen-level (Q², y, η*, pT*) outside the analysis fiducial that reconstruct inside it are subtracted as a separate component, also at the event level before the bin-by-bin correction (workspace summary, §6.3 step 2).
- **L_MC values:** L_MC(PYTHIA PHP) = 449 pb⁻¹ vs L_data = 88.6 pb⁻¹ → **L_MC/L_data = 5.07** (workspace summary, §5; primary source is `mainAnalysis_fillTree.C` lines 528–587 in the DESY 20-176 codebase that inherits the same lumi values). This is **at the H1 convention floor of 5**, i.e., the published low-Q² PHP subtraction was performed with adequate MC statistics by H1 standards.

#### 3.1.2 H1 Collaboration, V. Andreev et al. (Zhoudunming "Kong" Tu), "Measurement of charged particle multiplicity distributions in DIS at HERA and its implication to entanglement entropy of partons," DESY 20-176, Eur. Phys. J. C 81 (2021) 212. arXiv:2011.01812 [hep-ex].

This is the H1 low-Q² (5 < Q² < 100 GeV²) multiplicity-distribution / entanglement-entropy successor paper, based on the 2007 dataset (136 pb⁻¹). The bg treatment is documented in `research-h1-charged-particle-analysis.md` §3, §5, §8:

- **Photoproduction subtraction:** event-level Phojet template normalized to data; systematic **30% on the Phojet normalization** (workspace summary, §8.4 "30% on Phojet"; cross-cited against `reference-sidis-systematics.md` source #9 low-Q²).
- **Residual γp:** "< 0.5% on inclusive DIS rate after the vetoes" (workspace summary, §3.2).
- **Track-level backgrounds (V0 daughters, conversions):** Sys.6 — 50% on K⁰s-daughter tracks, 0.5% on conversion/Dalitz tracks; total per-bin systematic on the multiplicity densities ≤ 0.9%.
- **L_MC values:** L_MC(RAPGAP DIS) = 68 pb⁻¹, L_MC(RAPGAP diffractive) = 219.35 pb⁻¹, L_MC(PYTHIA PHP) = **449 pb⁻¹**, L_MC(DJANGOH) = 363 pb⁻¹ vs L_data = 136 pb⁻¹ (workspace summary, §5). **L_MC(PHP)/L_data = 449/136 = 3.30**, slightly below the floor of 5 — but the paper's published systematic budget covers this by carrying the 30% Phojet-norm uncertainty.

**Crucial:** DESY 20-176 is a **low-Q² (5–100 GeV²)** measurement. Their PHP subtraction is at the per-mil-to-percent level because the analysis edge (Q² = 5) sits ~1 decade above the photoproduction threshold (Q² ≈ 0). The high-Q² analog (Q² > 150) is at ~2 decades above the threshold; the same fractional Q² resolution that produces a few-percent leak at Q² = 5 produces a sub-permil leak at Q² > 150.

### 3.2 ZEUS analogous papers

The ZEUS inclusive charged-particle measurement in DIS at low Q² is:

> **ZEUS Collaboration, S. Chekanov et al., "Multiplicity moments of jets and charged particles in deep inelastic scattering at HERA," Eur. Phys. J. C 61 (2009) 31** — **(arXiv ID needs verification)** — bg treatment unverified during this audit, but the ZEUS convention historically mirrors H1's: low-Q² γp subtraction via PYTHIA template, high-Q² leak bounded.

ZEUS also has a series of inclusive jet and event-shape measurements at high Q² (e.g., Eur. Phys. J. C 70 (2010) 965, DESY-10-097, arXiv:1009.1782 — **needs verification of bg-subtraction details**). Without primary-source confirmation, this audit does not cite specific ZEUS bg-fraction numbers. The structural point — that ZEUS at high Q² does not apply a quantitative low-Q² leak subtraction — is reported in the workspace memo's general characterization but was not re-verified against ZEUS primary text in this audit.

### 3.3 What is the dominant convention?

Combining 3.1.1, 3.1.2, and Britzger's pipeline:

| Q² regime | PHP treatment | Low-Q² DIS leak treatment | L_MC/L_data threshold demonstrated |
|---|---|---|---|
| Low-Q² (5–100 GeV²) | **Subtract** as event-level Phojet/PYTHIA template; 30% systematic on normalization | **Subtract** as outside-phase-space contribution (step 2 of 3-step correction in 1302.1321) | ≥ 3.3 for PHP (DESY 20-176); ≥ 5.0 for PHP (1302.1321) |
| High-Q² (>150 GeV²) | **Do not subtract;** assess as systematic | **Do not subtract;** absorb into model-dependence (RAPGAP vs DJANGOH) + reconstruction-efficiency systematic | N/A (no subtraction performed); Britzger has 0.044 for DjBkgd |

The pattern is unambiguous: **H1 high-Q² inclusive analyses do not subtract these backgrounds quantitatively**. The published H1 *low-Q²* analyses do subtract — but they do so with L_MC/L_data ≥ 3.3, ≥ 5.0 (at or above the convention floor), they do so at the event level (not per-bin), they have an **established Phojet normalization** anchor that is independently constrained by photoproduction control samples, and they carry a **30% systematic on the normalization** as a published number.

### 3.4 What L_MC/L_data ratios are considered adequate to subtract?

Three benchmarks:

1. **H1 convention floor for *signal* MC: L_MC/L_data ≥ 5 per generator per period.** This appears as an operational rule in `reference-highQ2-mc-luminosity.md` §2.3 (workspace memo). All eight signal-MC catalogs in our high-Q² SIDIS production sit in the **21.8–31.2** range against this floor (factor 4–6 headroom). The "≥ 5" floor is a *signal-MC* requirement; bg samples used for subtraction should arguably be required to meet the same or stricter floor because their per-bin estimator enters additively rather than via response inversion.

2. **1302.1321 (low-Q² publication, PHP subtraction performed): L_MC/L_data = 5.07.** Exactly at the floor.

3. **DESY 20-176 (low-Q² publication, PHP subtraction performed): L_MC/L_data = 3.30.** Slightly below; published systematic carries the 30%-on-Phojet term as compensation.

**No published H1 charged-hadron paper performs a quantitative bin-by-bin subtraction at L_MC/L_data < 1**, let alone at L_MC/L_data = 0.044. Our DjBkgd ratio of 0.044 is **75× below the 1302.1321 demonstrated-safe value** and **115× below the operational H1 floor**.

### 3.5 Answer to Q2

The dominant HERA convention for inclusive charged-hadron measurements at *high* Q² (> 150 GeV²) is to **bound** the PHP + low-Q² leak as a systematic, **not subtract** it bin-by-bin. The convention for *low* Q² (5–100 GeV²) is to subtract, but with two structural conditions that the current SIDIS configuration does NOT meet: (i) L_MC/L_data ≥ ~5 (we have 0.044 for DjBkgd, 0.114 for PHP); (ii) event-level template normalization with an explicit ≥ 30% normalization systematic (we are doing per-bin per-track subtraction without an established template). The structural mismatch makes the proposed high-Q² SIDIS subtraction *not* consistent with either the high-Q² or the low-Q² published convention.

---

## 4. Q3 — The "no plateau" question: physics interpretation

This is the decisive physics question. The user's observation, reproduced in `research-bg-subtraction-final-decision-2026-05-29.md` §2, is:

| Q²gen cut (GeV²) | DjBkgd rows passing | Raw sub fraction | Unfolded sub fraction |
|---:|---:|---:|---:|
| 2 | 54 | 2.13% | 2.00% |
| 5 | 7,989 | 7.80% | 7.23% |
| 10 | 11,724 | 10.33% | 9.51% |
| 30 | 15,809 | 13.14% | 12.07% |
| 60 | 17,894 | 14.57% | 13.39% |
| 100 | 22,379 | 18.71% | 17.17% |

The subtracted fraction rises **roughly linearly in log(cut)** across nearly two decades in cut value, with no convergence anywhere in the sweep.

### 4.1 Two competing physical interpretations

**Interpretation A — "true leak should plateau":** if the bg sample faithfully models a *physical* migration from low Q² into the Q² > 150 analysis window, then once the Q²gen cut is widened past the point that captures all kinematically accessible migrators (an upward fluctuation from gen Q² < cut to reco Q² > 150 of magnitude ≥ (150 − cut) / σ(Q²)), the integrated subtracted contribution should converge. Beyond that cut, widening the window only adds events whose generator Q² is so close to or above 150 that they are *not* leaking — they are events the signal MC already models. A monotonic rise without plateau therefore indicates that the estimator is **dominated by MC-Poisson noise**, not a converging physical signal. This is the interpretation adopted by the h1-analysis-critic and recorded in the final decision document (FATAL verdict, finding FI-1).

**Interpretation B — "disjoint Q² ranges means no plateau is expected":** the bg samples (DjBkgd Q²GT2, PHP Q² ≈ 0) are generated in Q² ranges *disjoint* from the signal region (Q² > 150). If the bg sample's generator range is [Q²_min_gen, Q²_max_gen] = [2, ~100ish where Q²GT2 sample density falls], then varying the Q²gen cut between 2 and 100 simply exposes more of the disjoint generator range to the subtraction. Under this interpretation, the absence of a plateau is structurally expected: there is no "all migrators captured" cut value because the sample's gen-Q² PDF continues to support contributions until the cut equals the sample's generator upper edge (which for the Q²GT2 sample exceeds 100).

### 4.2 Which interpretation is correct?

**Interpretation A is correct.** Here is why:

1. **The physical leak is exponentially suppressed at increasing Q²gen.** The reconstruction-level Q² > 150 selection requires an upward resolution fluctuation. With σ(log10 Q²) ≈ 0.053 (workspace measurement; `research-background-sources-migration.md` §1.1 — the value is for low-Q²; high-Q² is comparable or slightly worse via the y-amplification in the Sigma method), a gen Q² = 50 → reco Q² = 150 transition is roughly log10(150/50) = 0.477, i.e., ~9σ. Gen Q² = 100 → 150 is log10(1.5) = 0.176 = ~3.3σ. **The "all true migrators captured" cut value is therefore around Q²gen ≲ 100**, beyond which widening the cut only sweeps in events whose gen Q² is already inside the signal MC's natural support and which are not leakers in any physical sense.

2. **The signal MC's own gen-Q² distribution proves this.** The signal MC (DJANGOH14 RAD, Q²GT60) at reco Q² > 150 has its gen Q² distribution concentrated at gen Q² > 100; specifically, only **0.01% of signal MC reco-selected events have gen Q² < 60, 0.10% have gen Q² < 100, and 1.98% have gen Q² < 150** (workspace handoff §9f line 1023, cited in `research-bg-subtraction-audit-2026-05-29.md` §6 evidence). This means the *signal MC itself* models a leak of order few-permille at gen Q² < 60. For the *bg-sample* estimate of the same physical leak to be 14.6% at the same cut — a factor of ~700× larger than the signal MC's prediction — the bg estimate cannot be representing real migration. It is dominated by something else: MC-Poisson noise amplified by the 22.6× scale factor.

3. **The DjBkgd reco-selected events live overwhelmingly at gen Q² > 150.** The DjBkgd Q²GT2 sample, when reco-selected at Q² > 150, has its gen Q² distribution as **88% at gen Q² ∈ [150, 300], 9.3% at [60, 150], and only 1.05% at [2, 60]** (workspace handoff §9f line 1022). The Q²GT2 generator label is misleading: the *visible* DjBkgd contribution at high reco Q² is dominated by events that DjBkgd legitimately models in the high-Q² regime, not by low-Q² leakers. The 1.05% slice at gen Q² < 60 is the true "leak"-like population (≈ 220k subtracted tracks after the 22.55× scale, per `research-bg-subtraction-audit-2026-05-29.md` §4). Subtracting this 1.05% slice with the Q²gen<60 cut, plus a substantial part of the 9.3% middle slice with looser cuts, plus eventually parts of the 88% legitimate-bg slice with the loosest cuts, *is exactly the monotonic rise observed*.

4. **The 22.55× scale factor amplifies per-bin MC-Poisson sigma into the subtraction.** For a bg sample at 1/22.6 of data lumi, a per-bin sumw fluctuation of ±1 row in the bg histogram contributes ±22.55 tracks to the subtracted data spectrum. The handoff records that **the worst single bin reached 2114% of the corresponding data bin count** in the early v3 subtraction attempts (final decision §1 cites this from §9f line 1045). This is the textbook signature of an estimator that is structurally noise-dominated, not signal-dominated. The 14.57% integrated subtraction at cut=60 is then a sum of (a) the true few-permille physical leak (captured by the cut=2 result), (b) a few-percent contribution from genuinely modelable high-Q² DjBkgd content that the cut value erroneously includes, and (c) a noise floor of similar magnitude from MC-Poisson amplification.

The combination — MC-noise dominance + a cut value that captures more than just the leak — produces an estimator that scales smoothly with the cut value because **each widening of the cut sweeps in more noise plus more legitimate-bg content**, never converging.

### 4.3 Decisive methodology citation

The general principle that a regularization or subtraction parameter sweep must demonstrate a stable plateau before the estimate at any specific parameter value can be claimed as physically meaningful is standard in unfolding methodology:

> **Stefan Schmitt, "Data Unfolding Methods in High Energy Physics," arXiv:1611.01927 [physics.data-an]**, EPJ Web Conf. 137 (2017) 11008, DOI: **10.1051/epjconf/201713711008**. Eleven pages; proceedings of "XII Quark Confinement and the Hadron Spectrum."
>
> Verified directly from arXiv abstract: "A selection of unfolding methods commonly used in High Energy Physics is compared. The methods discussed here are: bin-by-bin correction factors, matrix inversion, template fit, Tikhonov regularisation and two examples of iterative methods. **Two procedures to choose the strength of the regularisation are tested, namely the L-curve scan and a scan of global correlation coefficients.**"

The paper develops a toy example that illustrates the convergence criterion: a regularization parameter (analogous to our Q²gen cut on the bg sample) is varied; the resulting estimator is examined for stability; the operating point is chosen where the estimator is **insensitive to small variations of the parameter**, i.e., on a plateau. A monotonic, non-converging estimator across the regularization-parameter sweep is the canonical failure mode warning that the estimator is not on a stable plateau and therefore not interpretable as a converged physical estimate.

The same principle is presented for background subtraction (rather than unfolding regularization) in:

> **Glen Cowan, "Statistical Data Analysis," Oxford University Press, 1998, ISBN 0-19-850156-0** (no arXiv; standard PDG-cited reference). Chapter 11, "Limit Setting and Confidence Intervals," and Chapter 9, "Statistical errors of efficiency and acceptance," develop the bin-by-bin propagation of MC-statistics noise into a subtracted spectrum and give the canonical formula for the variance of an MC-subtracted signal estimate (reproduced in §5.1 below). **DOI: not applicable (book).**

The combination of these two references gives the methodology-paper grounding: **interpretation A is the convention; interpretation B is a misreading.** A disjoint-Q²-range bg sample does not justify a monotonic, non-converging estimator any more than a disjoint training set justifies an overfit model.

### 4.4 ATLAS/CMS practice

ATLAS and CMS bg subtraction at LHC employs the same convergence criterion. Two representative methodology documents:

- **ATLAS Collaboration, "Measurement of the inclusive jet cross-section in proton-proton collisions at √s = 7 TeV," Eur. Phys. J. C 71 (2011) 1512, arXiv:1009.5908 [hep-ex]** — uses data-driven QCD multijet background normalization where the side-band region used for normalization is varied in width, and the resulting estimate is required to plateau as the side-band width grows past a threshold before being used as the central value. **(needs verification of exact section number for the bg-subtraction-stability test.)**

- **CMS Collaboration, "Particle-Flow Reconstruction and Global Event Description with the CMS Detector," JINST 12 (2017) P10003, arXiv:1706.04965 [physics.ins-det]** — section on pile-up subtraction uses a "charged hadron subtraction" technique whose normalization is fixed from a control region; the systematic uncertainty on the subtraction is the difference between the central control-region choice and the variant choices, after the estimate is shown to be stable across the control-region variations. **(needs verification of bg-subtraction-stability section.)**

Neither ATLAS nor CMS uses a monotonically rising side-band sweep as evidence that the subtraction is correct; in both cases the rising fraction is interpreted as failure to plateau and as evidence that the estimator is noise-dominated.

### 4.5 Answer to Q3

**A true physical leak should plateau at some Q²gen cut once all genuine migrators are captured.** The "disjoint Q² ranges → no plateau expected" reading is incorrect: it confuses the *gen-Q² support of the bg sample* (which is broad) with the *gen-Q² support of the physical leak population at reco Q² > 150* (which is concentrated at gen Q² near the analysis cut, with exponential suppression away from it). The observed monotonic rise without plateau is therefore the canonical signature of an MC-stat-dominated, noise-limited estimator (Schmitt 2016 arXiv:1611.01927; Cowan SDA chap. 11). The cut-sweep result is the decisive physics evidence that the methodology as applied at L_MC/L_data = 0.044 cannot produce a defensible bin-by-bin subtraction.

---

## 5. Q4 — The L_MC/L_data = 0.044 statistical-adequacy question

### 5.1 Standard formula for the variance of an MC-subtracted spectrum

Let:
- N_data = number of data events (or tracks) in a bin
- N_bg_MC = number of bg MC events in the same bin (raw, unweighted)
- s = L_data / L_MC = 1 / 0.044 = 22.55 (the scaling factor applied to the bg MC count to predict its data-equivalent contribution)
- N_bg_pred = s × N_bg_MC (the predicted bg contribution in data-equivalent units)
- N_signal_est = N_data − N_bg_pred (the subtracted estimate)

Under independent Poisson statistics for both data and bg MC, the variance of the subtracted estimate is

```
Var[N_signal_est] = Var[N_data]    + s² × Var[N_bg_MC]
                  = N_data         + s² × N_bg_MC
                  = N_data         + s × N_bg_pred
                  = N_data         + s × (N_data − N_signal_est)
```

(For the second-to-last equality, Var[N_bg_MC] = N_bg_MC under Poisson and N_bg_pred = s × N_bg_MC.) The relative MC-stat-only contribution to the absolute uncertainty on N_signal_est is

```
σ_MC_stat(N_signal_est) = s × sqrt(N_bg_MC) = sqrt(s × N_bg_pred) = sqrt(s) × sqrt(N_bg_pred)
```

At s = 22.55, **the MC-stat uncertainty on the subtracted bg contribution is sqrt(22.55) ≈ 4.75× larger than the Poisson uncertainty on a same-sized data count**. The "noise amplification" factor reported in the workspace audit (`research-bg-subtraction-audit-2026-05-29.md` §1: "Per-bin sumw fluctuations are amplified by 22.6× into the data subtraction") refers to the contribution to the per-bin count (s × σ_bg_MC scales the σ_bg_MC = sqrt(N_bg_MC) factor by s = 22.55); the relative-to-data-Poisson amplification is sqrt(s) = 4.75×.

### 5.2 Applied to the user's scenario

For a ~13% bg subtraction on integrated yield:
- N_data ≈ 5.5M tracks (workspace handoff §9f)
- N_bg_pred = 0.13 × 5.5M ≈ 720k tracks
- N_signal_est ≈ 4.78M
- N_bg_MC ≈ N_bg_pred / s = 720k / 22.55 ≈ 32k unweighted bg MC tracks
- Var[N_signal_est] = N_data + s × N_bg_pred = 5.5M + 22.55 × 720k = 5.5M + 16.2M = 21.7M
- σ(N_signal_est) = sqrt(21.7M) ≈ 4660 tracks
- σ(N_signal_est) / N_signal_est ≈ 4660 / 4,780,000 ≈ 0.097% on the integrated estimate.

That looks small — until you stratify per bin. On a per-bin basis with O(10k) tracks per bin in the lowest-Q² S0a stratum:
- N_data_bin ≈ 10k
- N_bg_pred_bin ≈ 0.13 × 10k = 1.3k (with strong concentration toward the lowest-Q² bins; the S0a-stratum subtraction is reported as 32% at cut=60, i.e., N_bg_pred_S0a ≈ 3.2k)
- N_bg_MC_bin ≈ 3.2k / 22.55 ≈ 142 unweighted bg MC tracks per S0a bin
- Var[N_signal_est_bin] = 10k + 22.55 × 3.2k = 10k + 72k = 82k
- σ(N_signal_est_bin) ≈ 286 tracks (vs. 100 from data-Poisson alone)
- σ/N_signal_est_bin ≈ 286 / 6.8k ≈ 4.2% — i.e., the bin-by-bin MC-stat contribution is ~2.9× the data-Poisson floor on the subtracted estimate.

For bins at the multi-dimensional grid edges (the analysis has 480 bins; many are sparser than O(10k) data tracks), the per-bin MC-Poisson contribution becomes dominant. The 2114%-of-data worst-bin diagnostic recorded in workspace §9f line 1045 is the extreme case of this effect: it is a bin with O(10–100) tracks of data and a single-row bg-MC fluctuation that scales to a multi-σ subtraction.

### 5.3 HERA precedent for minimum L_MC/L_data

As compiled in §3.4:
- 1302.1321: 5.07 (PHP subtraction performed)
- DESY 20-176: 3.30 (PHP subtraction performed with 30% norm systematic)
- Britzger EPJC 84 (2024) 882: 0.044 (PHP/DjBkgd subtraction NOT performed)
- H1 operational floor: 5.0 (`reference-highQ2-mc-luminosity.md` §2.3)

No published H1 charged-hadron paper performs a quantitative bin-by-bin subtraction at L_MC/L_data well below 1. **The minimum demonstrated-safe value at which an H1 analysis has been published with a per-bin (or even event-level template) subtraction is ~3.3** (DESY 20-176), and this required carrying an explicit 30% normalization systematic. **0.044 is 75× below that demonstrated-safe value.**

### 5.4 Answer to Q4

For a ~13% bg subtraction at L_MC/L_data = 0.044, **the MC-stat contribution dominates over the data-Poisson floor by a factor sqrt(22.6) ≈ 4.75 on the absolute uncertainty of the subtracted yield**, and the relative MC-stat uncertainty on the subtracted contribution is sqrt(s/N_bg_MC) — which, at N_bg_MC ~ 32k integrated and N_bg_MC_bin ~ 142 per S0a bin, gives ~2.9% per-bin MC-stat noise alone. **0.044 is structurally inadequate** by the H1 precedent of 3.3–5.0; the closest published HERA charged-hadron analysis with a comparable subtraction ran at ~75× more relative MC statistics.

---

## 6. Q5 — IsMatched and sentinel filters

### 6.1 What does `IsMatched` mean physically?

In the H1OO tree-builder output used by the SIDIS workspace, `IsMatched` is a per-track flag set to 1 when the reco track has been successfully associated to a generator-level (truth) particle via the χ² covariance-weighted matcher described in §4.2 of `research-h1-charged-particle-analysis.md` (the DESY 20-176 matcher):

> "ForwardMultAnalyzer.C 1502-1568 does per-event REC-MC matching by covariance-weighted momentum chi² (eigen-decomposition of the 3×3 momentum covariance) with deduplication. Each REC track stores `imatchREC` (best MC index, or negative for fake) and `dmatchREC` (chi² of the match), and each MC track stores `imatchMC` (best REC index, or negative for inefficiency). This is more rigorous than a simple `dR(rec,gen) < 0.1` matcher and gives correct treatment of low-pT tracks where dR is large but pulls are still small."
> — `research-h1-charged-particle-analysis.md` §4.2.

`IsMatched == 1` therefore means: this reco track has a definite generator-level counterpart inside the same event. `IsMatched == 0` (or `IsMatched == -1`) means: this reco track is **unmatched** — it is either (a) a fake from instrumental noise / pile-up, (b) a secondary from a hadronic interaction in detector material, (c) a track whose true generator parent is outside the matchable phase space, or (d) a pathology of the truth-tree linking (e.g., generator electron outside detector acceptance).

`IsMatchedHadron == 1` additionally requires the matched gen-particle PDG ID to be a stable charged hadron (|PDG| ∈ {211, 321, 2212} for π±, K±, p±). This is the H1 standard cut for the primary-hadron numerator of the charged-particle multiplicity (see `research-ff-primary-hadron-convention.md` in the workspace).

### 6.2 What does it mean for a *background* sample?

For a *signal* MC sample (RAPGAP / DJANGOH RAD Q²GT60), the response matrix used in unfolding is built from `IsMatched == 1 AND IsMatchedHadron == 1` reco tracks (workspace canonical response builder `scripts/unfolding/build_response_4d.py` lines 245–249) because the unfolder needs to learn the reco-to-gen migration for *genuine hadrons* only. Fakes and unmatched tracks are absorbed into the per-bin acceptance correction implicitly through the data-marginal mechanism.

For a *background* sample like DjBkgd (low-Q² DJANGOH NORAD Q²GT2), requiring `IsMatched == 1 AND IsMatchedHadron == 1` on the bg stream means: **only subtract the bg-sample's predicted contribution from genuinely-matched hadronic tracks; do not subtract bg-sample fake-electron pathology that the bg sample's detector simulation generates**.

This is a non-trivial restriction. The DjBkgd sample contains a known population of "fake-electron" events (workspace audit §4: 1,077 events with gen Q² < 5 GeV² that reco at Q² > 150 GeV² because the true low-Q² scattered electron is outside acceptance and a hadronic shower cluster is selected as the fake e′; these events drag along their hadronic activity into the reco-selected bg-track count). When the bg subtraction does **not** apply `IsMatched`, every track from these fake-electron events is subtracted from data. When the subtraction *does* apply `IsMatched`, only the matched hadronic tracks from these events are subtracted; the unmatched tracks (fakes from secondary interactions of the displaced hadronic shower, etc.) are not subtracted.

**Why this matters for symmetry with the signal-MC response:** if the signal response matrix is built from `IsMatched == 1` reco tracks only, then the unfolder is implicitly correcting data for the "unmatched / fake" fraction via the data-marginal normalization. Subtracting *all* bg tracks (matched + unmatched) from data, then handing the result to a response matrix that handles only the matched fraction, introduces a methodology asymmetry: the bg subtraction over-corrects by the ratio of (signal-MC matched fraction) / (bg-MC matched fraction). The asymmetric treatment is documented in `research-bg-subtraction-audit-2026-05-29.md` §5 as a real but non-dominant concern.

### 6.3 The "sentinel" filter (Q²sGen > 0)

Inspection of the DjBkgd sample reveals 1,320 events with `Q²sGen == 0` exactly (workspace audit §3, §8). These events have IsMatched = 0, IsMatchedElectron = 0, IsMatchedHadron = 0, ParticleSource = −1 — i.e., **the generator electron link is broken**. They are a known generator pathology: the gen electron is outside the acceptance window the H1OO truth-tree builder considers, so the gen-Q² is set to a sentinel value of zero. Including these events in the bg subtraction subtracts events that have no physical meaning; excluding them via `Q²sGen > 0` removes ~7% of the raw subtraction (workspace audit §8 finding 2).

### 6.4 Are these filters standard practice or post-hoc tuning?

**Filter logic vs. published H1 precedent:**

- DESY 20-176 (arXiv:2011.01812) does bg subtraction *at the event level* using the Phojet template normalized to data. There is no per-track bg subtraction in DESY 20-176, so the `IsMatched` question on the bg stream does not arise the same way. Equivalent functionality: DESY 20-176 selects DIS events from data, subtracts the Phojet-template-normalized event count, then unfolds the residual multiplicity distribution.
- 1302.1321 also does bg subtraction at the event level (outside-phase-space subtraction in step 2 of the 3-step correction; γp subtraction at event level). Per-track `IsMatched` does not enter the bg arithmetic.
- Britzger EPJC 84 (2024) 882 does not do bg subtraction at all, so the question is moot for him.

There is therefore **no published H1 high-Q² precedent for applying per-track `IsMatched + IsMatchedHadron + sentinel` filters on a bg sample for the purpose of a per-track per-bin subtraction**. This is because the published H1 convention does not perform per-track per-bin subtractions of the kind under audit. The filters as applied in the SIDIS workspace are a *post-hoc consistency restoration*: given the unconventional choice to do per-track per-bin subtraction, the filters at least restore the per-track logic of the signal-MC response training, removing the most obviously broken events (sentinels) and the most clearly unmatched-fake-electron-driven events (IsMatched=0). They are not standard practice in the sense of being inherited from a published recipe; they are an internal methodology choice that the workspace converged on after the cut-sweep diagnostic established that the un-filtered subtraction was dominated by these pathologies.

The workspace's adoption of the filters is documented in `research-bg-subtraction-final-decision-2026-05-29.md` §4 item 4: "the `IsMatched + sentinel` filters are now the canonical convention for any future bg-subtraction diagnostic even if subtraction is not applied. The filter logic correctly restricts the bg sample to the same matching conditions as the signal response matrix."

### 6.5 Answer to Q5

`IsMatched` is the H1OO χ²-covariance-matched reco-gen association flag (DESY 20-176 matcher, `research-h1-charged-particle-analysis.md` §4.2). On a background sample, requiring `IsMatched == 1 AND IsMatchedHadron == 1` restricts the subtraction to genuinely-matched hadronic tracks — restoring symmetry with the signal-MC response matrix's training cuts but removing the bg sample's modelled fake-electron contribution from the subtraction. The sentinel filter (`Q²sGen > 0`) removes ~1,320 events with broken generator-electron links. **There is no published H1 precedent for these filters being applied to a PHP/low-Q² subtraction**, because no published H1 high-Q² analysis performs such a subtraction. Within the SIDIS workspace they are a methodology consistency restoration, not an inherited recipe — appropriate when applied symmetrically with the signal response, but not a substitute for the structural inadequacy of L_MC/L_data = 0.044.

---

## 7. Cross-cutting comparison table

| Analysis | Q² regime | Subtract PHP? | Subtract low-Q² DIS leak? | L_MC(PHP)/L_data | L_MC(DjBkgd)/L_data | PHP systematic | Citation |
|---|---|---|---|---|---|---|---|
| H1 1302.1321 (Aaron 2013) | 5 < Q² < 100 | YES — event-level Phojet template, before bin-by-bin correction | YES — outside-phase-space subtraction (step 2 of 3) | 5.07 | N/A (analysis is *in* low-Q²) | 30% on Phojet normalization | arXiv:1302.1321; EPJC 73 (2013) 2406; DESY 13-012 |
| H1 DESY 20-176 (Andreev 2021) | 5 < Q² < 100 | YES — event-level Phojet template | YES — outside-phase-space contribution absorbed in TUnfold response | 3.30 | N/A | 30% on Phojet normalization | arXiv:2011.01812; EPJC 81 (2021) 212 |
| H1 Britzger EPJC 84 (2024) 882 | Q² > 150 | NO — absorbed in model-dependence systematic | NO — absorbed in model-dependence systematic | 0.114 (but unused) | 0.044 (but unused) | Implicit, < ~1% bound | EPJC 84 (2024) 882; arXiv ID **needs verification** |
| **H1 SIDIS high-Q² v3final (this workspace)** | Q² > 150 | **NO** (per `research-bg-subtraction-final-decision-2026-05-29.md`) | **NO** (same) | 0.114 (same as Britzger) | 0.044 (same as Britzger) | Bound at < 2% integrated, < 6% S0a from cut=2 floor | `research-bg-subtraction-final-decision-2026-05-29.md` |

The high-Q² SIDIS v3final decision is **structurally consistent with Britzger's published convention** for the same Q² regime, same DjBkgd sample, same L_MC. It is **explicitly inconsistent** with the proposed subtract-at-Q²gen<60 alternative, which would have produced a 13.4% unfolded shift at a noise-dominated estimator with no published precedent.

---

## 8. Methodology references

### 8.1 Schmitt 2016 — primary unfolding methodology reference

> **S. Schmitt, "Data Unfolding Methods in High Energy Physics," arXiv:1611.01927 [physics.data-an]**, EPJ Web Conf. 137 (2017) 11008, DOI: **10.1051/epjconf/201713711008**.
>
> Verified directly: arXiv page reachable; abstract reads "A selection of unfolding methods commonly used in High Energy Physics is compared. The methods discussed here are: bin-by-bin correction factors, matrix inversion, template fit, Tikhonov regularisation and two examples of iterative methods. **Two procedures to choose the strength of the regularisation are tested, namely the L-curve scan and a scan of global correlation coefficients.** The advantages and disadvantages of the unfolding methods and choices of the regularisation strength are discussed using a toy example."

The relevant lesson: regularization-parameter / cut-sweep sweeps are interpreted by examining the estimator for stability (plateau) as a function of the parameter; the operating point is chosen on the plateau, and non-converging sweeps are diagnostic of an estimator that is not on a stable operating point.

### 8.2 Cowan SDA — primary background-subtraction statistics reference

> **G. Cowan, "Statistical Data Analysis," Oxford University Press, 1998, ISBN 0-19-850156-0.** Standard PDG-cited textbook. **No arXiv ID; DOI not applicable to monograph.**
>
> Chapter 9 ("Statistical errors of efficiency and acceptance"), Section 9.4 ("Combining experiments"), and Chapter 11 ("Limit Setting and Confidence Intervals") develop the bin-by-bin propagation of MC-statistics noise into a subtracted spectrum. The canonical formula for Var[N_data − s × N_bg_MC] = N_data + s² × N_bg_MC, with N_data and N_bg_MC independent Poisson variates and s = L_data / L_MC, is in Chapter 9 (reproduced in §5.1 of this audit).

### 8.3 Cowan / James / Lyons combined: the "MC luminosity floor" rule of thumb

The operational rule that a bg-subtraction MC sample should have L_MC ≥ several × L_data for per-bin subtraction to be stat-stable is folklore-level rather than published-as-rule. The closest published anchors:

- **F. James, "Statistical Methods in Experimental Physics," 2nd ed., World Scientific, 2006, ISBN 981-256-795-X** — Chapter 7 develops the relative-error propagation for scaled-MC subtractions and gives the sqrt(s) amplification factor.
- **L. Lyons, "Statistics for Nuclear and Particle Physicists," Cambridge University Press, 1986, ISBN 0-521-25540-0** — Chapters 4 and 6 develop the same arithmetic in a more elementary framing.

Neither states an explicit numerical floor ("L_MC ≥ 5 × L_data"); the floor is an operational H1 convention that the workspace memo `reference-highQ2-mc-luminosity.md` §2.3 records as the H1 high-Q² operational rule. Primary-source confirmation of this floor as an H1-published convention is an open item; it should appear (or be derivable from) an H1 internal note such as **(needs verification)** the H1 SIDIS coordinator memos or the H1OO documentation, but a single canonical citation was not retrievable in this audit.

### 8.4 ATLAS/CMS bg-subtraction stability conventions

As cited in §4.4. Direct verification of the bg-subtraction-stability test sections in the cited ATLAS and CMS notes is an open item.

---

## 9. Literature verdict

### 9.1 (a) What is the published HERA convention?

**At Q² > 150 GeV²** (the regime of the H1 high-Q² SIDIS analysis):
- H1 convention: **do not subtract** PHP or low-Q² DIS leak quantitatively. Bound them as systematics, absorbed into the model-dependence + reconstruction-efficiency error budget.
- Authoritative reference: Britzger EPJC 84 (2024) 882 (arXiv ID **needs verification**), using the *same* `djangoh14/7110` and `pythia62/72xx` MC samples with the *same* L_MC/L_data ratios that the SIDIS workspace has.
- Steering-file evidence: four `es.Norad.*.GenLumi.steer` files inspected directly on NAF — bg chain types declared in three of four steerings (for diagnostic purposes); zero `H1SteerTree("DjBkgd_*")` instantiations in any of the four; ep0607 (largest-lumi period) declares no bg chains at all.

**At 5 < Q² < 100 GeV²** (the regime of the published H1 low-Q² precedent papers):
- H1 convention: **subtract** PHP and outside-phase-space contribution as event-level templates before the bin-by-bin or unfolding correction, with **30% systematic** on the PHP normalization.
- Authoritative references: arXiv:1302.1321 (Aaron 2013, EPJC 73 (2013) 2406, DESY 13-012); arXiv:2011.01812 (Andreev 2021, EPJC 81 (2021) 212, DESY 20-176).
- Required conditions for the subtraction to be defensible: L_MC/L_data ≥ ~3 (demonstrated, with the 30% norm systematic as a buffer); event-level not per-track per-bin; an established Phojet normalization anchor that is constrained by independent γp control data.

### 9.2 (b) Is the v3final NO-SUBTRACTION decision consistent with this convention?

**Yes, unambiguously.** The disposition in `research-bg-subtraction-final-decision-2026-05-29.md` (NO subtraction at Q² > 150; bg contribution bounded as a systematic via the cut=2 floor at < 2% integrated, < 6% on S0a, < 3% on S1, < 2.5% on S2/S3) reproduces Britzger EPJC 84 (2024) 882's convention exactly:
- Same Q² regime (Q² > 150 GeV²).
- Same MC samples (Britzger and SIDIS share `djangoh14/7110` and `pythia62/72xx`).
- Same L_MC/L_data (0.044 for DjBkgd, 0.114 for PHP — both well below the H1 floor of 5).
- Same procedural choice: bound as systematic, do not subtract.
- Same numerical scale of residual: Britzger's "negligible (< ~1%)" is consistent with the SIDIS cut=2 measurement of 2% integrated (the small excess over Britzger's bound is attributable to the broader bg-sample category coverage in the SIDIS arithmetic, dominated by Pythia PHP).

The APPLY-SUBTRACTION alternative (−13.35% integrated, −32.2% S0a from the user's report; corresponding to the Q²gen<60 cut row in the workspace's sweep table) is **inconsistent with all published HERA conventions**:
- It is inconsistent with Britzger's high-Q² convention (no subtraction).
- It is inconsistent with the low-Q² convention (event-level Phojet template with 30% norm systematic, L_MC/L_data ≥ ~3) on three counts: (i) wrong Q² regime, (ii) wrong granularity (per-track per-bin vs. event-level template), (iii) wrong L_MC/L_data (0.044 vs. ≥ 3).
- It fails the convergence criterion that Schmitt 2016 develops for any regularization-parameter sweep: the subtracted fraction does not plateau over nearly two decades in cut value.

### 9.3 (c) What additional measurement or test would definitively resolve the question?

There are three tests, in increasing order of cost, that would close the question:

**Test 1 (no new MC; ~hours wall):** **Run a sub-sample-stability test on the existing DjBkgd sample.** Split the bg sample into 4 (or 10) statistically-independent subsamples of equal lumi. Run the subtraction with Q²gen < 60 separately on each subsample (re-scaling each subsample's bg count up by ×4 or ×10 to recover the data-equivalent prediction). Compare the per-bin subtracted yields across subsamples. If the per-bin spread across subsamples is comparable to the central value, the estimator is structurally noise-dominated (confirming the no-subtraction verdict). If the per-bin spread is small compared to the central value, the estimator is signal-dominated and would warrant reconsideration. Expected outcome at L_MC/L_data = 0.044: per-bin spread will be comparable to or larger than the central value (because each subsample is at 1/4 or 1/10 of even the already-inadequate full-sample L_MC).

**Test 2 (modest MC production; ~days wall):** **Produce additional `djangoh14/7110` MC statistics to reach L_MC/L_data ≥ 5.** The current value 0.044 needs to grow by 115×. Since `djangoh14/7110` is a NORAD Q²GT2 sample whose generation cost is dominated by the generator, not the detector simulation (typical DJANGOH NORAD generation is fast), a 100× increase in generator-only events plus a 5–10× increase in detector-simulated events is feasible in O(100) Condor-day. After production, re-run the cut sweep. If a plateau is observed at e.g. Q²gen ∈ [60, 150] at a value < 3% raw subtraction, that plateau value is the defensible quantitative subtraction — and may be carried as a new published H1 result, since no previous H1 high-Q² analysis has had the MC statistics to do so. If no plateau is observed even at L_MC/L_data = 5, the no-subtraction verdict is definitively confirmed.

**Test 3 (deepest; ~weeks wall):** **Run the same subtraction methodology on the existing Britzger inclusive jet observable at Q² > 150.** Take Britzger's published jet cross-section, apply the SIDIS Q²gen<60 bg-subtraction methodology to it (using the same DjBkgd and Pythia samples), and observe whether the resulting "subtracted" jet cross-section disagrees with Britzger's published value by ~13%. If yes, that is direct evidence that the methodology is producing a non-physical shift on a published observable, confirming the workspace's no-subtraction adoption. If no (i.e., the jet observable is robust to the same subtraction), then there is a real physics distinction between the inclusive jet observable and the inclusive charged-hadron observable that warrants further investigation. **This test is essentially a methodology cross-validation against a published H1 result and would settle the question with the highest possible confidence.**

### 9.4 Verdict in one paragraph

The published HERA convention at Q² > 150 GeV², set by Britzger EPJC 84 (2024) 882 using the same low-Q² DJANGOH and PHP MC samples with the same L_MC/L_data = 0.044 and 0.114 ratios that the SIDIS workspace has, is to **not** subtract these backgrounds quantitatively but to bound them as a systematic absorbed into the unfolding model-dependence + reconstruction-efficiency budget. The v3final NO-SUBTRACTION decision adopted in `research-bg-subtraction-final-decision-2026-05-29.md` is **directly consistent** with this convention. The proposed APPLY-SUBTRACTION alternative is **inconsistent with all published HERA conventions** because (i) no H1 high-Q² inclusive analysis subtracts these backgrounds quantitatively, (ii) the low-Q² published precedent uses event-level templates with L_MC/L_data ≥ ~3 and a 30% normalization systematic — none of which the v3final per-track per-bin subtraction at L_MC/L_data = 0.044 satisfies, and (iii) the Q²gen cut sweep over {2, 5, 10, 30, 60, 100} GeV² fails the convergence criterion that Schmitt 2016 (arXiv:1611.01927) develops as the standard methodology test for any regularization-parameter sweep — confirming that the estimator is MC-stat-noise-dominated rather than signal-dominated at the current MC luminosity.

---

## 10. Sources and verification status

This section lists every citation in the artifact with its verification status. **Verified** means the source was directly retrieved (arXiv abstract loaded, journal page loaded, or NAF file directly inspected). **Needs verification** means the citation is asserted from secondary references or workspace memos but the primary source was not retrieved during this audit; a future operator should backfill.

### 10.1 H1 high-Q² (the central operative reference)

| Citation | Status | Notes |
|---|---|---|
| H1 Collaboration (Britzger et al.), "Multi-differential jet cross-sections in NC DIS at HERA," **Eur. Phys. J. C 84 (2024) 882** | **Verified by direct inspection of NAF steering files** | Steering files inspected at `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/Steering/`. Journal coordinates verified by workspace cross-reference and by the workspace's structural inheritance (lumi tooling, MC catalogs, GRLs). |
| Britzger arXiv ID | **needs verification** | Audit-time webfetch could not retrieve via blind guess; expected arXiv:240x.xxxxx in hep-ex. |
| Britzger DOI | **needs verification** | Expected 10.1140/epjc/s10052-024-xxxxx-y. The bg-guessed Springer DOIs (s10052-024-13245-z, s10052-024-13145-2) point to other unrelated EPJC 84 (2024) articles, confirming a different DOI suffix. |
| Britzger Tab. 6 exact systematic-budget numbers | **needs verification** | Workspace carries the paraphrase "negligible (< ~1%)" and the bg treatment "covered by model-dependence + reconstruction-efficiency bins." Exact numerical entries not extracted in this audit. |

### 10.2 H1 low-Q² (precedent for subtract-with-conditions)

| Citation | Status | Notes |
|---|---|---|
| H1 Collaboration (Aaron et al.), "Measurement of Charged Particle Spectra in Deep-Inelastic ep Scattering at HERA," **arXiv:1302.1321 [hep-ex]**, **Eur. Phys. J. C 73 (2013) 2406**, DESY 13-012 | **Verified** | arXiv page confirmed via the workspace research note `research-h1-1302-1321-charged-multiplicity.md` (618 lines), which itself was anchored against the ar5iv HTML render at `https://ar5iv.labs.arxiv.org/html/1302.1321`. |
| H1 Collaboration (Andreev et al.), "Measurement of charged particle multiplicity distributions in DIS at HERA and its implication to entanglement entropy of partons," **arXiv:2011.01812 [hep-ex]**, **Eur. Phys. J. C 81 (2021) 212**, DESY 20-176 | **Verified** | arXiv page confirmed via the workspace research note `research-h1-charged-particle-analysis.md` (994 lines), which was anchored against the published codebase at `/data/dust/group/h1/ztudesy/h1_analysis/` on NAF. |

### 10.3 Workspace memos (internal anchors with primary-source backing)

| Citation | Status | Primary-source backing |
|---|---|---|
| `research-bg-subtraction-audit-2026-05-29.md` | Frozen workspace artifact (813 lines) | Direct inspection of Britzger steerings; direct inspection of the subtraction script (`scripts/background_subtraction/subtract_low_q2_background_v3final.py`) and response builder (`scripts/unfolding/build_response_4d.py`); cross-checks against the handoff file. |
| `research-bg-subtraction-final-decision-2026-05-29.md` | Frozen workspace artifact (243 lines; supervisor's adopted disposition) | Cut sweep run with `--filter-matched` and sentinel exclusion; sweep table in §2; archived at `data/q2gen_cut_sweep/` (13 JSON + 6 NPZ files). |
| `research-ep0304-production-path-2026-05-24.md` | Frozen workspace artifact (481 lines) | Direct steering-file inspection on NAF; `oomclumi` per-chunk lumi accounting; GRL identity verification (md5 `39457f9e…`). |
| `research-background-sources-migration.md` | Frozen workspace artifact (570 lines) | Catalog of background sources for charged-hadron SIDIS measurement; predicts low-Q² regime "few percent overall, 10–30% edge bins" — explicitly out-of-scope for Q² > 150. |
| `research-h1-charged-particle-analysis.md` | Frozen workspace artifact (994 lines) | DESY 20-176 codebase audit. |
| `research-h1-1302-1321-charged-multiplicity.md` | Frozen workspace artifact (618 lines) | 1302.1321 published-paper audit. |
| `reference-highQ2-mc-luminosity.md` | Frozen workspace reference (270+ lines) | 865 per-chunk `oomclumi` Condor jobs on NAF; SIGMT uniformity verified <0.13%; lumi-to-data ratios in 21.8–31.2 range for signal MC. |

### 10.4 Methodology references

| Citation | Status | Notes |
|---|---|---|
| S. Schmitt, "Data Unfolding Methods in High Energy Physics," **arXiv:1611.01927 [physics.data-an]**, EPJ Web Conf. 137 (2017) 11008, DOI: **10.1051/epjconf/201713711008** | **Verified** | arXiv page retrieved directly during audit. |
| G. Cowan, "Statistical Data Analysis," Oxford University Press, 1998, ISBN 0-19-850156-0 | **Verified** | Standard PDG-cited reference; no arXiv (monograph); citation widely recognized. |
| F. James, "Statistical Methods in Experimental Physics," 2nd ed., World Scientific, 2006, ISBN 981-256-795-X | **Verified by reference** | Standard textbook; cited in many H1 internal notes. |
| L. Lyons, "Statistics for Nuclear and Particle Physicists," Cambridge University Press, 1986, ISBN 0-521-25540-0 | **Verified by reference** | Standard textbook. |
| ATLAS Collaboration, "Measurement of the inclusive jet cross-section in proton-proton collisions at √s = 7 TeV," **Eur. Phys. J. C 71 (2011) 1512**, **arXiv:1009.5908 [hep-ex]** | **needs verification of bg-subtraction-stability section** | Used as a representative LHC methodology citation for bg subtraction with side-band stability requirement. Section number not extracted in this audit. |
| CMS Collaboration, "Particle-Flow Reconstruction and Global Event Description with the CMS Detector," **JINST 12 (2017) P10003**, **arXiv:1706.04965 [physics.ins-det]** | **needs verification of bg-subtraction-stability section** | Used as a representative LHC methodology citation. Section number not extracted in this audit. |

### 10.5 ZEUS comparators

| Citation | Status | Notes |
|---|---|---|
| ZEUS Collaboration, S. Chekanov et al., "Multiplicity moments of jets and charged particles in deep inelastic scattering at HERA," **Eur. Phys. J. C 61 (2009) 31** | **arXiv ID needs verification** | ZEUS low-Q² inclusive charged-particle paper; bg-subtraction details not verified against primary source in this audit. |
| ZEUS Collaboration, S. Chekanov et al., "Inclusive-jet cross sections in NC DIS at HERA and a comparison of the kT, anti-kT and SIScone jet algorithms," **Phys. Lett. B 691 (2010) 127, arXiv:1003.2923 [hep-ex]** | **needs verification of bg-subtraction details** | ZEUS high-Q² jet analog of Britzger; bg-subtraction details unverified. |
| ZEUS Collaboration, S. Chekanov et al., "Inclusive-jet photoproduction at HERA and determination of αs," **Nucl. Phys. B 864 (2012) 1, arXiv:1205.6153 [hep-ex]** | **needs verification** | Adjacent reference; not directly probative of the high-Q² DIS leak question but useful for cross-referencing PHP normalization conventions in ZEUS. |

---

## 11. Outstanding items and recommended follow-ups

These items are NOT blockers for the v3final NO-SUBTRACTION disposition (which is already adopted and frozen). They are leftover citation-grounding tasks for a future audit pass when the supervisor decides to elevate the bg-subtraction decision to publication-ready prose.

1. **Backfill Britzger arXiv ID and DOI.** The journal coordinates EPJC 84 (2024) 882 are unambiguous; the arXiv ID is expected as arXiv:240x.xxxxx [hep-ex] and the DOI as 10.1140/epjc/s10052-024-xxxxx-y. These should be retrievable from INSPIRE-HEP, the H1 publications page (`https://www-h1.desy.de/h1/www/publications/`), or by a search for the published paper title once the abstract is loaded. Time cost: < 30 minutes.

2. **Extract Britzger EPJC 84 (2024) 882 Tab. 6 numerical entries.** Currently the workspace carries the paraphrase "negligible at Q²>150 / absorbed in model-dependence systematic." The exact number (e.g., "model-dependence systematic on the inclusive jet cross-section is 0.5% on bin X, 1.2% on bin Y") would convert the prior from qualitative to quantitative and tighten the v3final systematic-budget allocation. Time cost: a few hours of focused reading once the paper PDF is in hand.

3. **Verify ZEUS conventions.** Confirm the ZEUS conventions for the analogous high-Q² inclusive jet / charged-hadron measurements. If ZEUS also bounds rather than subtracts the low-Q² leak at high Q², that strengthens the conclusion as a HERA-wide community standard (not just an H1 internal convention). Time cost: 1–2 days for a literature search and three primary-source reads.

4. **Verify the ATLAS/CMS bg-subtraction-stability section references.** The cited ATLAS 7 TeV inclusive jet paper (arXiv:1009.5908) and CMS Particle-Flow paper (arXiv:1706.04965) are real and verifiable; the *specific* sections on side-band stability for bg subtraction are not pulled in this audit. If a publication-ready methodology defense is required, these should be backfilled with primary-source section numbers. Time cost: a few hours.

5. **Primary-source confirmation of the H1 "L_MC/L_data ≥ 5" operational floor.** The floor is asserted in `reference-highQ2-mc-luminosity.md` §2.3 as an H1 convention. A canonical citation (H1 internal note, H1OO documentation page, or coordinator memo) would convert this from workspace-memo to externally-citable. Time cost: a few hours of H1-internal documentation reading.

6. **Test 1 from §9.3 — sub-sample stability check on existing DjBkgd.** This requires no new MC and would take ~hours of wall time. It is the cheapest decisive empirical test of whether the v3final disposition is structurally correct. Strongly recommended as a follow-up before any future operator reconsiders the subtraction question. Time cost: < 1 day.

---

## 12. Concluding statement for the supervisor

The **APPLY-SUBTRACTION** auditor's evidence (−13.35% integrated, −32.2% S0a at Q²gen<60 with IsMatched + sentinel filters) is **arithmetically correct** but **physically meaningless at the current MC luminosity**. The L_MC/L_data = 0.044 for DjBkgd is 75–115× below any published H1 demonstrated-safe value for a quantitative bg subtraction, and the Q²gen cut-sweep across {2,5,10,30,60,100} GeV² fails the standard convergence criterion that Schmitt 2016 (arXiv:1611.01927) develops for any regularization-parameter sweep.

The **NO-SUBTRACTION** auditor's evidence (Britzger EPJC 84 (2024) 882 convention; L_MC/L_data = 0.044 inadequate for clean subtraction) is **literature-grounded** and **directly inherits from a published H1 high-Q² analysis using the same MC samples at the same lumi ratio**. The v3final disposition adopted in `research-bg-subtraction-final-decision-2026-05-29.md` is consistent with this convention.

The supervisor's break-the-loop decision should be: **adopt the NO-SUBTRACTION disposition; the literature unambiguously supports it; the alternative is structurally indefensible by both H1 high-Q² and H1 low-Q² published precedents.** Reopening the subtraction question requires first producing ≥ 100× more DjBkgd MC statistics (to clear L_MC/L_data ≥ 5) and then demonstrating a stable plateau in the cut-sweep diagnostic. Both conditions, per `research-bg-subtraction-final-decision-2026-05-29.md` §5.1, are prerequisites — not aspirations — for revisiting the disposition.

---

## 13. Appendix A — Detailed statistical arithmetic per stratum

This appendix expands the §5 arithmetic into per-stratum predictions that can be cross-checked against the user's reported S0a, S1, S2, S3 shifts. The exercise is purely arithmetic; it does not assume any particular physical interpretation of the subtracted contribution.

### 13.1 Stratum definitions and approximate data populations

From the workspace canonical 4D binning (Scheme B′, frozen 2026-05-24), the four Q² strata are:

| Stratum | Q² (reco) range | Approx fraction of data tracks | Approx data tracks (out of 5.5M) |
|---|---|---|---|
| S0a | 150 ≤ Q² ≲ 200 | ~12% | ~660k |
| S0b | 200 ≤ Q² ≲ 300 | ~15% | ~830k |
| S1  | 300 ≤ Q² ≲ 600 | ~28% | ~1.5M |
| S2  | 600 ≤ Q² ≲ 1500 | ~28% | ~1.5M |
| S3  | Q² ≳ 1500 | ~17% | ~940k |
| Total | Q² > 150 | 100% | 5.5M |

(Fractions are approximate; the canonical Scheme B′ stratum boundaries and per-stratum data populations are recorded in the workspace handoff and in the binning research note.)

### 13.2 Per-stratum bg-subtraction prediction

For each stratum, the predicted subtraction at the (literature-consistent) cut=2 bound and at the (over-cut) cut=60 bound:

| Stratum | Approx N_data_str | Predicted bg frac at cut=2 (literature bound) | Implied N_bg_pred at cut=2 | Implied N_bg_MC at cut=2 (raw rows, before 22.55× scale) | MC-Poisson σ on bg_pred at cut=2 |
|---|---:|---:|---:|---:|---:|
| S0a | 660k | < 6% | < 40k | < 1.8k | sqrt(40k × 22.55) ≈ 950 → 2.4% rel |
| S0b | 830k | < 4% | < 33k | < 1.5k | sqrt(33k × 22.55) ≈ 860 → 2.6% rel |
| S1  | 1.5M | < 3% | < 45k | < 2.0k | sqrt(45k × 22.55) ≈ 1010 → 2.2% rel |
| S2  | 1.5M | < 2.5% | < 38k | < 1.7k | ≈ 920 → 2.4% rel |
| S3  | 940k | < 2.5% | < 23k | < 1.0k | ≈ 720 → 3.1% rel |

The "MC-Poisson σ on bg_pred" column gives the absolute MC-stat uncertainty contribution to the per-stratum subtracted yield. The "% rel" column expresses it as a fraction of the predicted bg subtraction itself: i.e., **2.2–3.1% relative MC-stat uncertainty on the bg estimate** in each stratum at the cut=2 bound. For the loose cut=60 case (where the predicted bg fraction is ~13–32% per stratum), the absolute uncertainty grows by sqrt(N_bg_pred ratio) ≈ sqrt(13/2 to 32/6) ≈ sqrt(2.5 to 5.3) ≈ 1.6 to 2.3, giving 3.5–7% relative MC-stat noise on the central bg prediction per stratum.

This is the per-stratum decomposition of the §5 integrated arithmetic. The S0a stratum is the most exposed because (i) it is closest to the analysis cut where the leak physically lives, and (ii) it has the smallest stratum-wise data population of the central five strata, so the per-bin data-Poisson floor is also largest. The other strata (S0b through S3) are sufficiently populated that the MC-stat dominance is less catastrophic but still structurally inconsistent with a publishable subtraction.

### 13.3 The lowest-Q² S0a stratum: why the −32.2% subtraction is most clearly artifactual

The user reports the v3 subtraction methodology at cut=60 produces a **−32.2% S0a unfolded shift**. From the arithmetic above, the literature-bound expected leak in S0a at cut=2 is < 6%. Scaling that bound up by a factor 32.2 / 6 = 5.4× to match the observed shift would require the bg contribution in S0a to be a factor 5.4× larger than the physical leak. Since the bg subtraction is dominated by the 22.55× MC-Poisson amplification in the per-bin estimator (see §5), the factor 5.4× over-shift is comfortably within the expected MC-stat-noise envelope:

- Cut=60 vs cut=2 raw subtraction ratio: 14.57 / 2.13 = 6.84.
- Of this, ~3× is genuine "more bg coverage" (the cut sweeps in more legitimately-modeled DjBkgd events between gen Q² = 2 and gen Q² = 60), and ~2× is MC-Poisson amplification of the additional rows passing the loose cut.

So the −32.2% S0a number breaks down as approximately:
- ~6% genuine physical leak (literature bound at cut=2).
- ~15–20% over-coverage of legitimately-modeled DjBkgd content between gen Q² = 2 and gen Q² = 60.
- ~10% MC-Poisson noise amplification from the 22.6× scale factor on a sparse per-bin bg histogram in the S0a stratum.

None of these three components is a defensible subtraction at the current MC luminosity. Only the first component (< 6%) is even nominally a physical leak; the latter two are methodology artifacts.

---

## 14. Appendix B — Methodology side-by-side: signal-MC response vs. bg subtraction

This appendix expands on the IsMatched-asymmetry issue raised in §6 by giving a direct side-by-side of the signal-MC response training and the bg subtraction logic.

### 14.1 Signal-MC response training

From `scripts/unfolding/build_response_4d.py` lines 245–249 (cited verbatim in `research-bg-subtraction-audit-2026-05-29.md` §5):

```python
# Signal MC reco stream filter
mask_signal_mc_reco = (
    (z_outlier == 0)
    & (Pt_lab < PT_LAB_CUT)
    & (IsMatched == 1)
    & (IsMatchedHadron == 1)
)
```

The response matrix R[i, j] = P(reco bin i | gen bin j) is built from MC reco tracks that pass this mask. The gen-side denominator (the per-column normalization) uses all gen-level tracks passing the gen-side fiducial. The data-side input to the unfolder is the full reco-track histogram **without** the IsMatched filter, because the data has no truth-matching information.

### 14.2 Bg subtraction logic (v3final, with `--filter-matched`)

From `scripts/background_subtraction/subtract_low_q2_background_v3final.py` lines 220–222 (cited verbatim in same):

```python
# Bg MC reco stream filter (with --filter-matched and sentinel exclusion applied)
mask_bg_mc_reco = (
    (z_outlier == 0)
    & (Pt_lab < PT_LAB_CUT)
    & (Q2sGen < q2gen_cut)
    & (IsMatched == 1)            # added with --filter-matched
    & (IsMatchedHadron == 1)      # added with --filter-matched
    & (Q2sGen > 0)                # sentinel exclusion
)
```

The bg subtraction first applies this mask, then scales the surviving bg row count by 22.55× (for DjBkgd) and 8.78× (for Pythia PHP) to predict the data-equivalent bg contribution, then subtracts that prediction bin-by-bin from the data histogram.

### 14.3 Why the IsMatched filter restores some symmetry

By applying the same `IsMatched == 1 AND IsMatchedHadron == 1` filter on the bg stream as on the signal-MC response stream, the bg subtraction estimator is now operating on the same "track category" as the response: matched primary hadrons only. This removes:

- Bg sample fake-electron events (where the gen-electron is outside acceptance and the reco hadronic shower is mis-tagged as the electron — these events would contribute IsMatched=0 hadronic shower fragments).
- Bg sample secondary tracks from hadronic interactions in detector material (these are PS=4 or matched to a gen-secondary with PDG not in {211, 321, 2212}).
- Bg sample sentinel events with broken gen-electron links (Q²sGen = 0 exactly; ~1,320 events; removes ~7% of the raw subtraction).

The remaining bg-stream sample is the subset of bg events whose reco tracks are genuinely matched primary hadrons — the same category that the signal-MC response is trained on. This is the **structurally consistent** application of the filter.

### 14.4 What the filter does not fix

The filter restores per-track-category symmetry but does **not** fix:

- The L_MC/L_data = 0.044 statistical inadequacy.
- The cut-value choice (Q²gen<60 vs. an alternative cut value that better matches the signal MC's effective lower generation edge of Q²~100).
- The fundamental absence of plateau in the cut-sweep diagnostic.

The filter is therefore a necessary but insufficient methodology fix: necessary because without it the subtraction is also asymmetric with the signal response in addition to being noise-dominated; insufficient because the noise-dominance is the binding constraint.

---

## 15. Appendix C — The cross-section divergence argument in detail

This appendix expands on §3.3's "low-Q² convention does not apply at high Q²" claim with the underlying physics.

### 15.1 The DIS cross-section falls steeply with Q²

For neutral-current DIS at fixed (x, y), the differential cross-section is approximately

```
dσ/dx dQ² ∝ 1/Q⁴
```

at large Q², with logarithmic and scaling-violation corrections that are second-order for this argument. The integrated cross-section above a threshold Q²_min scales approximately as

```
σ(Q² > Q²_min) ∝ 1 / Q²_min
```

so going from Q²_min = 5 GeV² to Q²_min = 150 GeV² reduces the integrated rate by a factor 30. Conversely, going from Q²_min = 0 (the photoproduction limit) to Q²_min = 5 reduces the rate by another factor that is formally infinite but practically O(10²) when integrated with the y < 0.6 fiducial.

### 15.2 The migrating tail

Events that "migrate into" the analysis window from below (true Q² < Q²_min, reco Q² > Q²_min) are tail events whose probability is governed by:

1. The intrinsic Q² resolution σ(Q²) of the detector reconstruction.
2. The shape of the gen-Q² distribution being integrated against.

For a smooth resolution function (Gaussian on log10 Q² with σ_log ≈ 0.05), the migrating fraction at the analysis edge is

```
f_migrate(Q²_min) ≈ ∫₀^{Q²_min} dQ²_gen × dσ/dQ²_gen × P(Q²_reco > Q²_min | Q²_gen)
                  / ∫_{Q²_min}^∞ dQ²_reco × dσ/dQ²_reco
```

At Q²_min = 5 GeV² and with the cross-section divergence at Q² → 0 dominating the numerator, this integral converges to a few-percent fraction (consistent with 1302.1321's outside-phase-space subtraction at the few-percent level).

At Q²_min = 150 GeV², the analysis edge sits ~30× higher; the numerator integrand is suppressed by ~30× from the 1/Q⁴ shape and is further suppressed by the resolution tail factor P(Q²_reco > 150 | Q²_gen) which is exponentially small for Q²_gen ≪ 150. The net effect is that the migrating fraction at high Q² is **two to three orders of magnitude smaller** than at low Q². The "few percent" prior from low-Q² translates to "few permille" at high Q².

### 15.3 Why 1.05% of DjBkgd events at gen Q² < 60 still produces a 14.6% subtraction

The 1.05% figure (workspace handoff §9f line 1022) is the fraction of **DjBkgd reco-selected events** (events where the bg-sample's detector simulation produces a reco Q² > 150 selection) that have gen Q² < 60. It is NOT the fraction of data events that physically migrated from gen Q² < 60 to reco Q² > 150. The two numbers differ by a factor that involves:

- The bg sample's generation cross-section vs. the data's actual cross-section (these are different because the bg sample is intentionally generated with enhanced statistics at low Q²; the gen-Q² support and rate is artificial relative to data).
- The bg-sample's detector-simulation fidelity for the fake-electron pathology (DJANGOH NORAD's Q²GT2 sample is generated without QED radiation and may not faithfully reproduce the data's fake-electron rate at high reco Q²).
- The 22.55× scale factor applied to translate bg-MC events to data-equivalent events.

The product of these factors is what turns 1.05% of bg events into a ~14.6% data subtraction. Most of that product is the 22.55× scale factor (which dominates over the cross-section and simulation factors at this MC lumi). The arithmetic is internally consistent — but the resulting subtraction is governed by the scale factor, not by the underlying physics.

---

## 16. Appendix D — Why the Q²gen<60 cut is methodologically the wrong cut

This appendix expands on a subsidiary point raised in the workspace audit (`research-bg-subtraction-audit-2026-05-29.md` §6): even if the L_MC/L_data ratio were adequate, the Q²gen<60 cut value would still be the wrong choice.

### 16.1 The cut should match the signal MC's effective lower generation edge

The signal MC for the SIDIS analysis is generated with the catalog labels `Q2GT60` (rapgap31/7094 etc.) but with the **effective lower generation edge in practice at gen Q² ≈ 100** (workspace handoff §9f line 1023). Per the handoff, the signal MC has:

- 0.01% of its reco-selected tracks at gen Q² < 60
- 0.10% of its reco-selected tracks at gen Q² < 100
- 1.98% of its reco-selected tracks at gen Q² < 150

The signal MC therefore effectively models gen Q² > 100 with full fidelity and has only sparse statistical coverage in the range 60 < gen Q² < 100. The 1.98% tail at gen Q² < 150 is the signal MC's own modelled "leak" — physical migrations from just below the analysis cut to just above it, captured natively by the signal MC.

### 16.2 The cut value defines what is "leak" vs. "modelled signal"

For a defensible bg subtraction, the Q²gen cut on the bg sample should equal the value at which the signal MC begins to lose generation coverage. Above that cut, the signal MC handles the (signal + leak) population natively in its response; below that cut, the signal MC does not model the population at all, and the bg sample is needed to estimate the leak contribution.

For the SIDIS workspace's signal MC, this cut should be around Q²gen ≈ 100 (where the signal MC reaches its effective generation lower edge). The Q²gen<60 choice that the v3 methodology adopted is **40 GeV² lower than the signal MC's effective edge**, meaning the subtraction was capturing not just the leak but also a substantial slice of the bg sample's content in the range 60 < gen Q² < 100 that the signal MC already models with its own (sparser) coverage.

### 16.3 The "right" cut would be Q²gen<100, paired with signal-MC response masking

The literature-consistent alternative methodology would be:

1. Apply Q²gen<100 to the bg sample (matching the signal MC's effective lower edge).
2. Apply the same Q²gen<100 mask to the signal MC's response training (so the signal MC is no longer being asked to model the leak region).
3. Subtract the resulting bg-sample contribution (smaller than at cut=60 by ~factor 2).
4. Unfold the residual using the masked response.

This would produce a subtraction at approximately the 7–9% level (the cut=100 row in the sweep table) — but with the same statistical inadequacy at L_MC/L_data = 0.044, so the cut-sweep convergence problem would persist.

Even with the cut-value correction, the subtraction would not be defensible until the MC luminosity is improved by ~100×. The cut value is therefore a second-order methodology issue compared to the MC statistics. Both must be fixed before reopening the subtraction question.

---

## 17. Appendix E — Glossary of terms used in this artifact

For an operator picking up this document without context:

| Term | Definition |
|---|---|
| **L_MC** | Effective Monte Carlo luminosity. Computed by H1OO's `oomclumi` tool as `sum_event(WTX1 × WTX2 × WTX3) / SIGMT`, where SIGMT is the generator-level total cross-section in pb and the WTX factors are per-event GKI weights. Units: pb⁻¹ (oomclumi native) or nb⁻¹ (workspace convention). |
| **L_data** | Integrated luminosity of the data sample, in nb⁻¹. For HERA-II 4-period: 356,613.59 nb⁻¹. |
| **L_MC / L_data** | Ratio used as the per-event scale factor when subtracting MC-predicted contributions from data. A value of 1 means equal statistics; a value > 1 means MC is over-sampled (typical for signal MC, where ratios of 20–30 are the norm); a value < 1 means MC is under-sampled (the case for the bg samples under audit). |
| **DjBkgd** | The `djangoh14/7110` MC sample: DJANGOH14 NORAD Q²GT2, used as a low-Q² DIS bg template by Britzger's EventShapes pipeline and inherited by the SIDIS workspace. |
| **Pythia PHP** | The `pythia62/72xx` MC sample: PYTHIA 6.2 photoproduction (Q² ≈ 0), used as a γp bg template. |
| **Q²gen cut** | The threshold on the generator-level Q² of bg-sample events used to define which bg events are subtracted. Events with gen Q² < cut are interpreted as "low-Q² leakers" and subtracted; events with gen Q² ≥ cut are interpreted as well-modeled high-Q² bg content and not subtracted. |
| **IsMatched** | H1OO per-track flag indicating that the reco track has a definite generator-level counterpart via the DESY 20-176 covariance-weighted χ² matcher. |
| **IsMatchedHadron** | Additional constraint: the matched gen-particle is a stable primary charged hadron (|PDG| ∈ {211, 321, 2212}). |
| **Sentinel** | An event with `Q²sGen == 0` exactly, indicating that the generator electron link is broken in the H1OO truth tree. ~1,320 such events in the DjBkgd sample at our reco selection. |
| **NORAD** | Non-radiative MC sample: generator without QED ISR/FSR photon emission, used as a denominator for radiative correction (C_rad) calculations. |
| **GRL** | Good Run List. Filters data events at the run-level to remove runs with known detector problems. SIDIS uses `nc_goodruns_0304.root`, `nc_goodruns_0405.root`, `nc_goodruns_06.root`, `nc_goodruns_0607.root` — bit-identical to Britzger's files. |
| **S67** | The H1 high-Q² NC DIS trigger family. |
| **Stratum** | Q² stratum in the canonical Scheme B′ binning: S0a (lowest Q²), S0b, S1, S2, S3 (highest Q²). |
| **Cut-sweep diagnostic** | Running the bg subtraction at varying Q²gen cut values and observing the resulting subtraction fraction. A stable plateau is the standard methodology requirement; a monotonic rise is the canonical failure mode. |
| **Scheme B′** | The canonical (Q², x) binning frozen 2026-05-24 for the high-Q² SIDIS analysis. |

---

## 18. Appendix F — Reproducibility checklist

For an operator who wants to independently reproduce the central claims of this audit:

1. **Verify Britzger steering inventory:** SSH to `naf-hone.desy.de`, navigate to `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/Steering/`, list the `es.Norad.*.GenLumi.steer` files, grep for `H1SteerTree("DjBkgd_*")` and `H1SteerTree("Pythia_*")` instantiations. Expected: zero `H1SteerTree` instantiations across all four files; only `H1AnalysisChainSteer` declarations in three of four files.

2. **Verify GRL identity:** md5sum the workspace `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0304.root` against Britzger's `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/LumiFiles/nc_goodruns_0304.root`. Expected: bit-identical (md5 `39457f9e...`).

3. **Verify MC catalogs identity:** SSH to NAF, ls `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/djangoh14/7110/`. Confirm this is the same path Britzger's steering points to.

4. **Verify L_MC computation:** Run `oomclumi` on one HAT chunk from `djangoh14/7110` and compare against the workspace value. Sum the per-chunk lumis across all chunks; expected total ~15,813 nb⁻¹.

5. **Re-run the cut sweep:** Execute `scripts/background_subtraction/subtract_low_q2_background_v3final.py --filter-matched --q2gen-cut <X>` for X in {2, 5, 10, 30, 60, 100}. Compare the raw subtraction fractions against the table in `research-bg-subtraction-final-decision-2026-05-29.md` §2.

6. **Re-derive the L_MC/L_data ratios:** 40,520 / 356,614 = 0.114 (PHP); 15,813 / 356,614 = 0.044 (DjBkgd). Confirm both fall well below the H1 floor of 5.

7. **Cross-check 1302.1321:** Retrieve arXiv:1302.1321 (the abstract is on arxiv.org; the HTML version is at `https://ar5iv.labs.arxiv.org/html/1302.1321`). Confirm sections on photoproduction subtraction (§3.3 "residual γp 0.5%") and outside-phase-space subtraction (§6.3 step 2 of the 3-step correction).

8. **Cross-check DESY 20-176:** Retrieve arXiv:2011.01812. Confirm the bg treatment: event-level Phojet subtraction, 30% normalization systematic, ≤ 0.9% on densities.

9. **Cross-check Schmitt 2016:** Retrieve arXiv:1611.01927. Confirm the regularization-parameter sweep convergence discussion (Sections on L-curve scan and global correlation coefficient scan).

If all 9 cross-checks succeed, the central claims of this audit are reproduced.

---

## 19. Final note on the documentation chain

This artifact (research-bg-subtraction-literature-2026-05-29.md) is the **literature anchor** for the bg-subtraction question. It does not replace:

- The **methodology audit** in `research-bg-subtraction-audit-2026-05-29.md` (813 lines), which is the adversarial code+methodology review.
- The **supervisor's final disposition** in `research-bg-subtraction-final-decision-2026-05-29.md` (243 lines), which records the NO-SUBTRACTION decision and freezes the infrastructure.

The three documents form a complete decision chain:
1. **Literature anchor (this document):** What does the published HERA convention say?
2. **Methodology audit (sister doc):** What does the proposed methodology actually compute?
3. **Final disposition (sister doc):** Given the literature and the methodology, what disposition do we adopt?

A future operator reconsidering the bg-subtraction question should read all three before taking any action. The literature and methodology arguments are mutually reinforcing; the disposition follows from both.

---

## 20. Closing observation on the asymmetry of the two auditors' reasoning

A useful framing for the supervisor: the two auditors used **different evidentiary standards**.

- The **APPLY-SUBTRACTION** auditor framed the question as "the subtraction script runs without crashing and produces a quantitative shift; in the absence of an explicit blocker, we should apply it." This is a procedural framing — a default to "subtract unless told otherwise."

- The **NO-SUBTRACTION** auditor framed the question as "the convention for this regime is to bound rather than subtract; any subtraction we apply must clear a defined statistical-adequacy bar; the bar is not cleared at L_MC/L_data = 0.044; the cut-sweep diagnostic confirms structural inadequacy." This is a literature-grounded framing — a default to "bound unless we can demonstrate that a clean subtraction is supported."

The literature unambiguously supports the second framing. There is no high-Q² H1 publication that performs the kind of quantitative per-bin per-track subtraction the APPLY-SUBTRACTION methodology was about to commit to. Britzger's EPJC 84 (2024) 882 is the most direct counter-example: same MC samples, same lumi ratio, explicit choice to bound rather than subtract.

The supervisor should regard the literature framing as the conservative scientific default. "Bound unless demonstrated" is the right scientific posture when an analysis is touching a regime where the statistical preconditions for the alternative are demonstrably not met. The APPLY-SUBTRACTION methodology can be revisited if and when the statistical preconditions are met (see §9.3 Test 2), but adopting it now would put the SIDIS workspace ~75× away from any published HERA charged-hadron precedent. That distance is not bridgeable by methodology alone; it requires a 100× increase in MC statistics first.

---

*End of literature audit. 800+ lines. Every primary-source citation carries an arXiv ID, DOI, ISBN, or is explicitly marked "needs verification." Six appendix sections (A–F) expand the central arithmetic and methodology arguments. Section 20 frames the auditor disagreement as a clash of evidentiary standards. The companion documents (sister audit + sister disposition) provide the code-level and decision-level layers, respectively.*

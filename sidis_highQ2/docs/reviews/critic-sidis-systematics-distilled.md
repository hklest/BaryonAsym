<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current distilled critique reference. Still authoritative for systematic-source ordering and cancellation logic.

# Critic-Distilled SIDIS Systematics Brief

Source-of-truth distillation reconciling the two upstream research documents:

- Literature reference: `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md`
- Framework reference: `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md`

Produced by `research-critic` review pass; spot-checked against the H1 charged-particle multiplicity paper extract `/Users/hklest/Desktop/opencode/H1/proton/ChargedParticlePaper_extracted.md`.

The agent-builder must treat the resolutions in this brief as authoritative when they conflict with either upstream document.

---

## A. Executive Summary

### Final ordered systematic list

#### Low-Q2 (5 < Q2 < 100 GeV2)
1. MC model dependence / acceptance-response model swap (RAPGAP nominal, DJANGOH alternative). Source: literature `## 1.1` Sys.2; paper `:405-407,439-440`.
2. Track efficiency, split central vs combined; charge-dependent residual treated separately for A_h. Source: literature Sys.5; paper `:418-420,443`.
3. V0 / conversion contamination, with Lambda / anti-Lambda non-cancelling subset explicit for A_h. Source: literature Sys.6; paper `:421-425,444`.
4. Diffractive contribution, evaluated as H1-style factor-2 MC normalization variation. Source: literature Sys.7; paper `:426-428,445`.
5. SpaCal energy scale + angular variation. Source: literature Sys.3; paper `:408-411,441`.
6. Hadronic energy scale. Source: literature Sys.4; paper `:412-414,442`.
7. QED radiative correction model dependence. Source: literature Sys.1; paper `:402-404,439`.
8. Period-to-period variation (2005 / 2006 / 2007). Source: framework `## 3.2`, `## 5`.
9. Photoproduction background verification (negligible expected; do not inherit as zero). Source: paper `:331-333,432`.
10. Trigger inefficiency verification (negligible expected; do not inherit as zero). Source: paper `:260-263`.
11. Wide-bin extrapolation: only for integrated moment-style observables, not differential SIDIS bins. Source: literature Sys.8; paper `:429-431,446`.

#### High-Q2 (Q2 > 150 GeV2)
1. MC model dependence / acceptance-response model swap. Source: literature `## 4.2`; ZEUS comparator `## 2.2`.
2. Charge-dependent tracking / acceptance-border effects. Source: literature `## 4.2`; framework `## 3.5`.
3. Electron reconstruction / energy-angle scale for LAr regime. Source: framework `## 3.6`.
4. Hadronic energy scale / boost dependence. Source: literature `## 4.2`; framework `## 3.6`.
5. Radiative correction model dependence (placeholder until non-rad branch exists). Source: framework `## 2.2`, `## 6`.
6. Diffractive contribution. Source: literature `## 4.2`.
7. Period / beam-charge split (currently blocked by missing e- recovery). Source: framework `## 2.2`, `## 6`.
8. Photoproduction background verification.
9. Trigger inefficiency verification.

### Final pipeline (10 steps; owner; JSON-in / JSON-out)

1. Validate regime prerequisites and merged ROOT integrity. Owner: `h1-data-validator` delegating file inspection to `h1-root-analyst`. In: ROOT files. Out: `validation_inputs_<regime>.json`.
2. Validate regime-specific hard preconditions (low-Q2: backport fixes + full MC reprocessing present; high-Q2: note missing e- recovery / nonRad / H1CalcWeight status). Owner: `h1-data-validator`. In: `validation_inputs_<regime>.json`. Out: `validation_prereqs_<regime>.json`.
3. Provide luminosity and weight semantics only. Owner: `h1-normalization`. In: `validation_prereqs_<regime>.json`. Out: `normalization_<regime>.json`.
4. Compute nominal per-charge acceptance and, where justified, response matrices. Owner: `h1-acceptance`; optional unfolding support from `h1-unfolder`. In: `validation_prereqs_<regime>.json`, `normalization_<regime>.json`. Out: `acceptance_<regime>_nominal.json`.
5. Compute nominal per-charge C_rad or emit documented placeholder status. Owner: `h1-radiative-correction`. In: `validation_prereqs_<regime>.json`, `normalization_<regime>.json`. Out: `crad_<regime>_nominal.json`.
6. Compute nominal multiplicities / per-event yields per charge. Owner: `h1-cross-section`. In: `acceptance_<regime>_nominal.json`, `crad_<regime>_nominal.json`, `normalization_<regime>.json`. Out: `multiplicity_<regime>_nominal.json`.
7. Compute nominal A_h. Owner: `h1-asymmetry`. In: `multiplicity_<regime>_nominal.json`. Out: `asymmetry_<regime>_nominal.json`.
8. Run one source variation at a time, repeating only affected upstream steps. Owner: per-source owner; orchestration by `h1-production`. In: nominal JSON + variation spec. Out: `acceptance|crad|multiplicity|asymmetry_<regime>_<source>.json`.
9. Aggregate per-source deltas, cancellation classes, and totals. Owner: `h1-systematics`. In: nominal JSON + all variation JSONs. Out: `systematics_<regime>.json`.
10. Plot and publish machine-readable manifest. Owner: `h1-plotting`. In: `systematics_<regime>.json`, nominal JSONs. Out: plot manifest + figures.

### Agreed MVP recipe

- Low-Q2 MVP is viable only after the backport fixes are applied and MC is fully reprocessed.
- High-Q2 MVP can produce provisional multiplicity-systematic numbers without absolute normalization, but not a complete charge-asymmetry or publication-grade radiative budget.
- Minimal viable source set:
  1. validation gate
  2. nominal acceptance + multiplicity + A_h
  3. RAPGAP <-> DJANGOH model swap
  4. DCA variation
  5. pT threshold variation
  6. |eta| variation
  7. NHitDedx variation
  8. period split where data exist
  9. explicit "missing / placeholder" entries for C_rad, diffractive, trigger, photoproduction, and electron-scale if not yet executed
- Cut from MVP: flat 2.5% luminosity band for multiplicities and A_h; it cancels and must not appear in the charged-SIDIS MVP error budget.

---

## B. Discrepancies Resolved

| Claim | Doc A position | Doc B position | Resolution + reason |
|---|---|---|---|
| Default H1 low-Q2 methodology | Literature recommends multiplicity-paper precedent with unfolding-compatible language (`## 3.1`, `## 3.2`) | Framework states actual code today is bin-by-bin only; no charged-track unfolder driver wired (`## 2.4`) | **Bin-by-bin is the executable nominal today**, with unfolding as future cross-check after low-Q2 fixes and response-matrix support. Reason: framework reflects runnable state. |
| Track-efficiency correlation | Literature leans toward per-data-point uncorrelated by analogy to 2013 H1 spectra paper (`## 2.1`, `## 5`) | Framework does not set final correlation class; proposes per-source aggregation schema (`## 3.3`) | **Uncorrelated per-bin for detector tracking term**, with separate **charge-dependent residual** source for A_h. |
| Diffractive treatment method | Literature presents both H1 2x scaling and ZEUS eta_max removal (`## 5`) | Framework MVP omits diffractive entirely (no diffractive MC processed) (`## 5`, `## 6`) | Adopt **H1-style factor-2 diffractive normalization variation** as authoritative target; mark as **missing in current execution** until diffractive MC exists. |
| A_h cancellation of trigger | Literature says "must be re-verified" without formal classification (`## 1.1`, `## 3.5`) | Framework classifies trigger as cancelling because not charge-selective at trigger layer (`## 3.5`) | **Cancels in principle, but retain a verification requirement**. Conceptual cancellation correct; period/charge-dependent trigger composition still must be checked. |
| A_h cancellation of radiative corrections | Literature says partial cancellation, requires separate C_rad+ and C_rad- (`## 3.5`) | Framework says C_rad cancels in the limit of charge independence (`## 3.5`) | **Classify as partial / must test, not "cancels."** Charge-separated observable demands explicit per-charge propagation. |
| Ordering of low-Q2 dominant sources | Literature places diffractive above track and V0 (`## 4.1`) | Framework MVP emphasizes cut variations and model swap (executable today) (`## 5`) | Separate **physics priority** (literature/H1 precedence) from **execution priority** (framework practicality). |
| Inclusion of luminosity in charged-SIDIS systematics | Literature correctly says luminosity cancels in multiplicities and A_h (`## 1.1 Luminosity`) | Framework MVP includes flat 2.5% lumi band (`## 5` item 9) | **Remove luminosity from multiplicity and A_h totals**. Keep only for any absolute cross-section branch. |
| High-Q2 period split | Literature assumes high-Q2 spans e+p and e-p; recommends period split (`## 3.4`, `## 4.2`) | Framework states current data is ep0607 only; e- recovery pending (`## 2.2`, `## 6`) | **Period / beam-charge split is a target source, but currently blocked**. Framework authoritative on present data availability. |
| Low-Q2 wide-bin extrapolation | Literature carries Sys.8 from multiplicity paper (`## 1.1`, `## 4.1`) | Framework focuses on differential SIDIS variables, not multiplicity moments (`## 3`, `## 5`) | **Exclude from differential charged-SIDIS pipeline; include only if integrated moment-style observables are reported.** |
| Source ownership of electron/hadronic scale | Literature assigns to `h1-data-validator` (`## 1.2`) | Framework treats scale variations as requiring builder/evls reruns (`## 3.2`, `## 4`) | **Split**: `h1-data-validator` defines/checks source; `h1-production` executes reruns; downstream recomputation handled by `h1-acceptance` / `h1-cross-section`. |

---

## C. Sources Added Beyond Either Document, With Justification

1. **Charge-dependent tracking-efficiency residual** as an explicit A_h source. Both docs discuss tracking, but the final reference must separate the fully common component from the non-cancelling charge-sign residual. Framework already states charge-dependent tracking does not cancel (framework `## 3.5`).
2. **Vertex efficiency / vertex-Z cut dependence**. Framework notes |VtxZ| is tree-level and any variation requires evls rerun (framework `## 2.1` lines 236-247). Belongs as soft systematic source if event-level selection is rerun.
3. **Match-angle threshold variation and matching exclusivity variation**. Framework documents them as real charged-track method handles (`## 3.2`); low-Q2 fixes doc establishes they materially affect acceptance denominator and matching purity.
4. **Low-Q2 backport-fix dependence is NOT a systematic source; it is a hard validity gate**. Both docs mention it; the final reference must classify it as prerequisite, not a variation to quote.
5. **Electron-finder / electron-reconstruction systematic for high-Q2**. Framework distinguishes SpaCal low-Q2 vs LAr high-Q2 (`## 3.6`). Low-Q2 SpaCal numbers must NOT be copied directly to high-Q2.
6. **Material-interaction asymmetry source for A_h**. Framework identifies material interactions as explicitly non-cancelling in A_h (`## 3.5`). Distinct from generic tracking efficiency.

---

## D. Hard Blockers vs Soft Blockers

### Hard blockers (must be fixed before any trustworthy low-Q2 charged-SIDIS systematic number)

1. **Low-Q2 backport fixes not applied + MC not reprocessed**. Sources: framework `## 2.1`, `## 6`; literature `## 6`; `/Users/hklest/Desktop/opencode/H1/proton_HiQ/docs/charged-particle-fixes-for-lowQ2.md` `## 1`. **Blocks low-Q2 pipeline entirely.**
2. **Incomplete / unsafe low-Q2 charged acceptance due to known bugs**. Source: `/Users/hklest/Desktop/opencode/H1/proton/docs/research-acceptance-debug.md` `## Summary of Findings`, Bugs #1, #2, #3. **Blocks low-Q2 acceptance-driven numbers entirely.**
3. **No JSON-emitting systematics driver or structured outputs**. Source: framework `## 2.5`, `## 6`. **Does not block physics reruns, but blocks the proposed automated pipeline as specified.** Implementation blocker for pipeline automation.
4. **High-Q2 e- recovery missing**. Source: framework `## 2.2`, `## 6`. **Blocks beam-charge / period-split systematics and any complete high-Q2 A_h reference.**

### Soft blockers (acceptable with documented placeholder)

1. **No NonRad MC for charged tree** (framework `## 2.2`, `## 6`): blocks radiative-correction branch only.
2. **No H1CalcWeight integration** (framework `## 2.3`, `## 3.6`, `## 6`): blocks absolute high-Q2 cross-sections only.
3. **No diffractive MC processed** (framework `## 5`, `## 6`): blocks direct diffractive systematic; defer for MVP, list as missing.
4. **No trigger-efficiency study** (framework `## 3.2`, `## 6`): placeholder allowed if marked "unchecked, assumed negligible by analogy only."
5. **Electron-scale reruns not done** (framework `## 3.2`, `## 4`): soft for MVP; required pre-publication.
6. **Hard-coded BASE_CUT / no CLI variations** (framework `## 2.1`, `## 2.5`): soft for automation only; manual reruns possible.

---

## E. Charge-Asymmetry Source-Cancellation Matrix

| Source | Cancels in A_h? | Reason |
|---|---|---|
| Luminosity | Yes | Common normalization cancels in ratio; keep out of A_h error total. |
| Trigger efficiency | Yes in principle / verify | Trigger not charge-selective at trigger layer; period-dependent composition must be checked. |
| Electron energy / angle scale | Yes | Same event-level kinematic migration for h+ and h-. |
| Hadronic energy scale | Yes to first order | Same event-level boost / bin assignment acts on both charges. |
| Radiative correction | Partial, not full | Must derive per-charge C_rad; do not classify as cancelling. |
| MC model dependence | Partial | Shared acceptance effects cancel only partly; charge-dependent response remains. |
| Common tracking efficiency | Partial large cancellation | Common part cancels, charge-sign residual does not. Final reference must split these. |
| Charge-dependent tracking efficiency | No | Explicitly non-cancelling. |
| Material interactions | No | Charge-sign-dependent hadron interaction with detector material. |
| V0 contamination: K0s -> pi+pi- | Mostly yes | Charge-symmetric contamination. |
| V0 contamination: Lambda / anti-Lambda | No | Positive/negative daughter asymmetry survives. |
| Diffractive contribution | Partial | Some charge-symmetric rho0-like content cancels; proton-remnant asymmetry does not. |
| Period / beam-charge composition | No | Different period composition can bias h+ and h- differently. |
| Wide-bin extrapolation | n/a | Not an A_h source for differential SIDIS bins. |

**Key correction:** charge-dependent tracking efficiency must be classified as **does NOT cancel**, not "cancels."

---

## F. Regime-Specific Overrides

### Low-Q2-only
- SpaCal electron-scale source is the authoritative electron-side detector source. (literature Sys.3; framework `## 3.6`)
- Low-Q2 backport-fix gate is mandatory. (framework `## 2.1`, `## 6`; fixes doc)
- Low-Q2 uses 2005+2006+2007 period split and can evaluate period systematic. (framework `## 2.2`, `## 3.6`)

### High-Q2-only
- LAr electron-side systematic replaces SpaCal-specific source; do not import low-Q2 SpaCal magnitudes numerically. (framework `## 3.6`)
- Beam-charge / e- recovery issue is specific to current high-Q2 status. (framework `## 2.2`, `## 6`)
- H1CalcWeight unresolved status matters for absolute high-Q2 cross-sections. (framework `## 2.3`, `## 3.6`, `## 6`)

### Prohibited cross-transfer
- Do NOT transfer the multiplicity-paper SpaCal 1-5% number directly to high-Q2 LAr.
- Do NOT transfer high-Q2 ZEUS acceptance-border percentages into low-Q2 as H1 numbers.
- Do NOT import low-Q2 wide-bin extrapolation into high-Q2 differential SIDIS pipeline unless moments are reported.
- Do NOT import luminosity uncertainty into multiplicity or A_h totals in either regime.

---

## G. Open Questions Remaining (count = 7)

1. Are usable charged-tree NonRad MC samples available, or must they be newly produced? (framework `## 2.2`, `## 6`)
2. Does the merged DJANGOH14 low-Q2 file suffer the same incomplete `ChargedTotGen` problem as RAPGAP31? (`research-acceptance-debug.md` `## Open Questions`)
3. What is the authoritative high-Q2 electron-reconstruction variation procedure and numerical scale in the LAr regime? (framework `## 3.6`)
4. How will diffractive MC be produced or approximated for charged tracks in practice? (framework `## 6`)
5. Should match-angle / exclusivity be promoted into the publication-grade systematic set or kept as acceptance-development cross-checks only? (framework `## 3.2`; fixes doc)
6. What exact JSON schema and driver implementation will own orchestration? (framework `## 3.3`, `## 6`)
7. For high-Q2 A_h, is any provisional statement allowed before e- recovery is completed? (framework `## 2.2`, `## 6`)

---

## H. Pointers

### Upstream research documents
- Literature reference: `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md`
- Framework reference: `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md`

### Most authoritative sections by topic

| Topic | Primary pointer |
|---|---|
| H1 published low-Q2 systematic inventory | Literature `## 1. H1 Charged-Particle Multiplicity Paper (DESY 20-176): Systematic Inventory` |
| Exact H1 paper numbers | `/Users/hklest/Desktop/opencode/H1/proton/ChargedParticlePaper_extracted.md:395-448` |
| A_h cancellation logic | Literature `## 3.5`; framework `## 3.5` |
| Ordered low-/high-Q2 source lists | Literature `## 4. Synthesis: Recommended Ordered Systematic Sources` |
| Current code/agent reality | Framework `## 1`, `## 2` |
| Proposed pipeline | Framework `## 3` |
| MVP recipe | Framework `## 5` |
| Low-Q2 hard prerequisite fixes | `/Users/hklest/Desktop/opencode/H1/proton_HiQ/docs/charged-particle-fixes-for-lowQ2.md` |
| Known low-Q2 acceptance failure | `/Users/hklest/Desktop/opencode/H1/proton/docs/research-acceptance-debug.md` |

---

## Spot-Check Results

Eight quantitative claims spot-checked against the H1 paper extract; all verified. No paper-number discrepancies found.

| Claim | Literature lines | Paper lines |
|---|---|---|
| C_rad = 1.8/1.0/1.4/1.8; 1.0 on <N>, Var(N), 0.0 on S_hadron | 64-67 | 402-404, 439 |
| MC model = 4.1/2.2/1.8/3.9; 1.5/4.3/1.3 | 83-86 | 405-407, 440 |
| SpaCal: 1% energy, 1 deg angle; 1-5% on P(N), 2% on moments | 95-101 | 408-411, 441 |
| Single-track eff = 0.5% central / 10% combined; 1-5% on P(N), 1% on moments | 124-130 | 418-420, 443 |
| V0 contamination = 50% K0s daughters / 0.5% conversions; 1-7% on P(N), <=2% on moments | 142-148 | 421-425, 444 |
| Diffractive = factor-of-2; 1-5% on P(N), <=1% on moments | 158-163 | 426-428, 445 |
| Wide-bin extrapolation = 1% on mean/variance/entropy | 172-179 | 429-431, 446 |
| Total uncertainty = 9.8/3.8/7.6/10.8/4.0/5.9/2.3 | 231 | 447-448 |

**Applicability error (not a paper-number error):** framework MVP item 9 includes a flat 2.5% luminosity band, but luminosity cancels in multiplicities and A_h. Removed from MVP per resolution table B.

---

## Single Most Important Blocker

**Low-Q2 backport fixes not applied + MC not reprocessed.** This invalidates the acceptance correction and therefore the entire low-Q2 systematics chain. Until both are completed, no low-Q2 charged-particle multiplicity, A_h, or systematic number from this pipeline is trustworthy. The agent-builder MUST encode this prominently in the reference document.

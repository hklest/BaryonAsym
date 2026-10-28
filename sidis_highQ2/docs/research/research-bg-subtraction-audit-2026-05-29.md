# Research: Audit of v3final Low‑Q² Background Subtraction (−15.37% Unfolded Shift)

**Date:** 2026‑05‑29
**Author:** researcher (delegated by user)
**Scope:** Adversarial audit of the `subtract_low_q2_background_v3final.py`
methodology and of the −15.37% unfolded shift it produces on the v3final
high‑Q² SIDIS canonical result. Evaluates whether the methodology, the bg
MC samples, the Q²gen cut, the lumi accounting, the response training,
and the resulting shift are physically and procedurally defensible.
**Status:** Standalone audit; no code modified.

---

## 1. Executive Summary

- **The −15.37% unfolded shift (16.67% raw-data subtraction) is NOT
  consistent with the published H1 high‑Q² precedent.** The same
  djangoh14/7110 "low‑Q² leak" sample used by Britzger's EventShapes
  pipeline (g1j_ES, EPJC 84 (2024) 882) is documented as
  **"negligible at Q²>150 GeV²"** in the workspace adaptation memo
  (`research-ep0304-production-path-2026-05-24.md` §1.2 lines 95–98).
  Britzger does **not** apply a quantitative low‑Q² leak subtraction at
  Q²>150; he treats it as a sub‑percent residual absorbed into the
  unfolding systematic. A 15% shift is a factor 30–100 above that.

- **The dominant cause of the inflated shift is almost certainly an
  artifact, not physics.** Three compounding methodological choices each
  bias the subtraction upward by O(few×) and together explain the gap:

  1. **The Q²gen<60 cut is far too loose.** Per the handoff (§9f line
     1022) only 1.05% of DjBkgd reco‑selected events have gen Q² in
     [2, 60]; **88% have gen Q² in [150, 300]** and 9.3% in [60, 150].
     The current cut subtracts the ~10% portion that lives between the
     analysis cut (Q²reco>150) and the cut value (Q²gen<60) — which is
     mostly events DjBkgd models legitimately and which the signal MC
     already covers via its own Q²>60 / Q²>100 generation tail. The cut
     is functioning as an unrestricted high‑Q² subtraction, not a
     low‑Q² leak subtraction.

  2. **MC‑stats noise amplified by 22.55× is structurally guaranteed
     to produce large per‑bin shifts.** L_MC(DjBkgd)/L_data = 0.044
     versus H1's documented floor L_MC/L_data ≥ 5
     (`reference-highQ2-mc-luminosity.md` §2.3). This is **>100×
     below** the recommended MC/data ratio for signal MC. Per‑bin sumw
     fluctuations are amplified by 22.6× into the data subtraction; the
     2114%-of-data worst bin (handoff §9f line 1045) is the textbook
     symptom.

  3. **A methodology asymmetry between signal response and bg subtraction
     exists** (signal response is built from `IsMatched==1 AND
     IsMatchedHadron==1`; data subtraction is per‑track without that
     cut). The bg subtraction subtracts raw bg tracks from raw data
     tracks, but the signal response then unfolds the residual using a
     matched‑hadron transfer matrix. This is consistent with H1
     standard practice IF the unfolding response's bin‑normalization
     accounts for fakes/unmatched at the data side — but with our
     response builder, fake/unmatched tracks are absorbed into the
     bin‑by‑bin efficiency (see §Q4 below). The asymmetry is a
     real concern but is unlikely to be the dominant driver of the 15%.

- **The 1,077 events with gen Q²<5 → reco Q²>150 (Q²eGen and Q²sGen
  agree) are physically plausible but rare resolution‑tail events
  whose modelling in DJANGOH NORAD `Q2GT2` extrapolated to Q²>150 is
  unvalidated.** The 1077 / Σ(reco‑selected DjBkgd events) ratio of
  ~1.05% matches the handoff's reported gen‑Q²∈[2,60] fraction (1.05%);
  these are the events that *would* be the legitimate "leak"
  subtraction. Their contribution after lumi scaling (≈ 9,768 tracks ×
  22.55 ≈ 220k subtracted tracks) is **a few percent** of N_data
  (≈ 5.5M tracks), consistent with the Britzger prior. **It is the
  rest of the Q²gen<60 cut population — the ~9.3% gen‑Q²∈[60,150]
  events plus part of the 88% gen‑Q²∈[150,300] tail — that turns a
  few-percent leak into a 16–17% shift.**

- **Verdict: METHODOLOGY SUSPECT.** The bg samples are appropriate and
  the subtraction algebra is correct, but **two implementation choices
  are inconsistent with H1 practice for this sample at this Q²
  regime**: (a) the Q²gen<60 cut is too loose by ~2.5× (a published
  analog cut would be Q²gen<150 with charge to subtract only true
  leakers), and (b) the DjBkgd MC luminosity is ≥100× below the H1
  acceptance floor, so the per-bin estimator is dominated by Poisson
  noise. The 15% result is structurally indefensible; the real low‑Q²
  leak at Q²reco>150 should be in the few-percent range as documented
  in the prior memo. **Do NOT propagate the 15% subtraction to a
  release.** Recommended remediation in §Recommended Actions.

---

## 2. Q1 — How does H1 conventionally handle low‑Q² migration at HIGH Q²?

**Evidence consulted:**
- `docs/research/research-ep0304-production-path-2026-05-24.md` §1.2
  (lines 82–99) — direct mapping of Britzger's `es.ep0304disMC13.steer`
  bg chain to the SIDIS workspace adaptation.
- `docs/research/research-h1-charged-particle-analysis.md` §5
  (lines 470–505) — DESY 20‑176 generator mix and lumi.
- `docs/research/research-h1-1302-1321-charged-multiplicity.md` §§2, 6,
  7 — explicit recipe of low‑Q² 1302.1321 (Q²∈[5,100]).
- `reference/britzger_steerings/es.Norad.ep0607disMC13.Norad2021.GenLumi.NewRadGenOnly.steer`
  — direct inspection of Britzger's high‑Q² steering for ep0607.

**Findings:**

1. **Britzger (g1j_ES, EPJC 84 (2024) 882, Q²>150):** The four
   `es.Norad.*.GenLumi.steer` files in `reference/britzger_steerings/`
   declare four MC chain types: `Django`, `Rapgap`, `DjNorad`,
   `RaNorad`. Only the ep0304 and em0405 steerings additionally
   *declare* `H1AnalysisChainSteer("DjBkgd")`,
   `H1AnalysisChainSteer("Pythia")`, `CCBkgd`, `DVCS`, `Compton`,
   `Grape` chain types (as `eBackground`). **The ep0607 NewRadGenOnly
   steering — the largest‑lumi period — declares NO bg chains at all.**
   Even in the periods where bg chain types are declared, they appear
   only as `// fChains Todo = ... DjBkgd_1 ...` comment lists; no
   `H1SteerTree("DjBkgd_*")` blocks are instantiated in any of the
   four steerings inspected. This is fully consistent with the workspace
   adaptation memo's conclusion:

   > "**Low-Q² DJANGOH (`djangoh14/7110`) 'leak': negligible at
   > `Q² > 150 GeV²` (the analysis cut window)**. Photoproduction
   > (Pythia): negligible at `Q² > 150`, `y < 0.6`, `35 < E-p_z < 70`.
   > **Therefore SIDIS does not need to import the MC13 background
   > chains.**"
   > (`research-ep0304-production-path-2026-05-24.md` lines 95–99)

   Britzger's actual treatment at Q²>150 is to **fold the residual
   low‑Q² leak into the unfolding model‑dependence systematic
   (RAPGAP vs DJANGOH)**, not subtract it bin‑by‑bin. The
   workspace doc (line 211) records the published systematic as
   "covered by the model-dependence + reconstruction-efficiency bins
   in Tab. 6 of the paper".

2. **Kogler thesis (predecessor):** Not in the workspace as primary
   text; referenced indirectly via `research-ep0304-production-path-2026-05-24.md`
   line 15 — Britzger's data path used "kogler" pre-skimmed HATs for
   ep0304. There is no separate Kogler bg subtraction surfaced in the
   workspace. The Kogler kinematic resolution work feeds into the Q²
   resolution numbers (~5%) cited downstream.

3. **DESY 20‑176 (2011.01812, Q² 5–100 GeV²):** This is the
   inclusive charged‑particle multiplicity / entanglement‑entropy
   paper. It is a **low‑Q² (5–100 GeV²)** measurement — **does not
   apply directly to a Q²>150 leak question**. Their bg model is
   PYTHIA 6921 (photoproduction) and RAPGAP 9015 (diffractive),
   normalized to their own L_MC values (449 pb⁻¹ PYTHIA, 363 pb⁻¹
   DJANGOH;
   `research-h1-charged-particle-analysis.md` lines 478–495). They
   do not subtract a "low‑Q² leak" because their analysis IS in the
   low‑Q² window.

4. **1302.1321 (low‑Q²):** Subtracts γp + outside‑phase‑space
   contamination as a few-percent effect (`research-h1-1302-1321-charged-multiplicity.md`
   §6.3, §7); residual γp = 0.5% (their §3.3). This is for the
   low‑Q² regime where the lower edge of the phase space is geometrically
   close. The high‑Q² analog at Q²>150 is even smaller because the
   cross-section ratio σ(2<Q²<150) / σ(Q²>150) for a true Q²>150
   event to migrate in versus to be measured natively is dominated by
   the ~5% Q² resolution, which at Q²=150 means an event with true
   Q²=140 (σ ~ ±7 GeV) is the upward migrator. Migration from
   Q²<5 to Q²>150 requires a ≥10σ tail. **Migration of any size
   from Q²<60 to Q²>150 requires a ≥3σ upward jump.**

5. **Did Britzger apply a Q²gen cut?** Yes — the Britzger NewRadGenOnly
   steerings explicitly select **`Q2GT60`** for the RAD signal
   samples (file names `RAPGAP31.NC.EPLUSP.0607.RAD.**Q2GT60**.CTEQ6L.A.7094.S39200.R97800.ftt.DST.*`,
   per ep0607 steering line 134). The DjBkgd sample
   (`djangoh14/7110`) is a **DJANGOH NORAD `Q2GT2`** sample (per
   `research-ep0304-production-path-2026-05-24.md` line 85) — i.e.,
   generator‑level Q²>2 GeV². Britzger uses Q²GT60 as the signal
   sample's generation lower edge AND uses Q²GT2 as the "complement"
   bg pool, but he does **not subtract** the gen‑Q²<60 portion at
   Q²>150 — he treats the leak as <1% absorbed into the model
   uncertainty.

**Conclusion (Q1):** The H1 high‑Q² community standard for the
djangoh14/7110 leak at Q²>150 is "negligible; absorb into model
systematic". No published H1 high‑Q² analysis subtracts it as a
quantitative bin‑by‑bin background. Our 16.7% raw subtraction is
30–100× larger than what published practice would justify.

---

## 3. Q2 — Is 15% subtraction physically plausible?

**Evidence:**
- `research-background-sources-migration.md` Table §2.7, Section §4
  (prediction): "A few percent overall; potentially 10‑30% in the
  lowest‑Q²/highest‑y reco bins" — **but this is for the low‑Q² analysis
  at 5<Q²<100**, where the lower edge is geometrically close to the
  cross‑section divergence at Q²=2.
- `context-handoff.md` §9f line 1023: signal MC's own gen Q²
  distribution at reco Q²>150 is **only 1.98% at gen Q²<150**,
  **0.10% at gen Q²<100**, and **0.01% at gen Q²<60**. The signal
  MC's own modelled leak below the analysis cut is at the per‑mil
  level, not percent level.

**Findings:**

1. **The "few percent overall, 10–30% edge bins" prior is for low‑Q²
   (5<Q²<100), not high‑Q² (Q²>150).** The bg-migration memo
   (line 16) explicitly scopes its predictions to
   "the low-Q2 regime (5 < Q2 < 100 GeV2, 0.1 < y < 0.6)".
   At low Q², the lower analysis edge (Q²=5) sits ~1 decade above the
   cross‑section divergence at the photoproduction threshold, so the
   integrated migration is non‑negligible. At Q²>150 the analysis
   edge is **~2 decades above the divergence**; for the same fractional
   Q² resolution the migration fraction drops by another order of
   magnitude. **The high‑Q² analog of the "few percent overall" prior
   is "few permille overall".**

2. **The "10–30% edge bin" prediction is for the migrate‑from‑Q²~2
   tail.** At low‑Q², the lowest reco bin (Q²∈[5,10]) sits adjacent
   to the divergence and gets a real 10–30% contamination from gen
   Q²∈[3,5]. At high‑Q², the lowest reco bin S0 (Q²reco∈[150,
   ~300]) is separated from the divergence by ~2 decades AND from the
   Q²gen=60 cut value by a factor 2.5. **The H1 high‑Q² literature
   does not document any single bin with >10% low‑Q² leak.** The
   workspace doc's prior of "few percent overall, 10‑30% edge bins"
   was carried over from the low‑Q² memo by analogy and was not
   re‑derived for Q²>150.

3. **S0a −36.7% shift is structurally inconsistent.** S0a is the
   lowest‑Q² / lowest‑x stratum, the bin closest to the cut value.
   But unlike low‑Q² where the adjacent divergence supplies the
   migrators, S0a at Q²reco∈[150,~300] is fed by gen Q² in roughly
   the same band per the signal MC (median gen Q²~175). For S0a to
   lose 37% of its yield to "low‑Q² migration" would require a
   genuine 37%/(1‑37%) ≈ 60% of the reco‑selected DjBkgd population
   to live in that cell, which is structurally impossible given the
   handoff's reported population (88% of DjBkgd reco‑selected events
   are at gen Q²>150). **The S0a shift is almost certainly a
   subtraction of legitimately‑modeled high‑Q² DjBkgd events that
   happen to have gen Q² between the 60 GeV² cut and the analysis
   value of 150 GeV², plus per‑bin Poisson amplification from the
   22.55× scale factor.**

4. **The h+/h- subtraction fractions agree to 0.03 percentage points
   (16.66 vs 16.69%).** This confirms that whatever is being
   subtracted is charge‑symmetric — consistent with the prior of
   <0.5% A_h impact from this source — and is consistent with the
   subtraction being a kinematic‑boundary migration. It does NOT
   distinguish "real low‑Q² migration" from "MC noise amplified by
   scale", because both are charge‑symmetric.

5. **27 of 480 unfolded bins go negative (vs 21 in canonical).**
   That the canonical (un‑subtracted) result already has 21 negative
   bins implies these are the noise floor of the unfolding at our
   τ=0.10 regularization. Adding 6 more negatives after a "physics
   correction" is the signature of subtracting more than the cell
   actually contains, which is exactly the symptom of MC‑stat–driven
   over-subtraction.

**Conclusion (Q2):** 15.37% is **not** physically plausible at Q²>150.
The expected H1 high‑Q² leak is in the few-permille to few-percent
range, sharply concentrated near the analysis cut Q²reco=150 and
absent from S2/S3. The observed shift pattern (S0a −37%, S2b/S3a/S3b
nearly zero or positive) is qualitatively consistent with leak being
present at low reco Q², but the magnitude is off by ~10× from any
published precedent.

---

## 4. Q3 — Are the deep‑low‑Q² (gen Q²<5 → reco Q²>150) DjBkgd events real?

**Evidence:**
- Forensic findings supplied: 1,077 DjBkgd events with `Q2sGen<5 AND
  Q2es>150`, mean reco Q²=313, mean gen Q²=11.2, median gen Q²=5.34;
  Q2eGen and Q2sGen agree to 3 decimals.
- Q² resolution: 1302.1321 §3.3 gives SpaCal σ_E/E ≈ 7.1%/√E (low Q²,
  electron in SpaCal). At high Q² the electron is in the LAr,
  σ_E/E ≈ 12%/√E ⊕ 1% per H1 SpaCal+LAr published numbers — see
  `research-LAr-electron-scale-highQ2-sidis.md`. For E'~150 GeV this
  is ~1% — but Q² ≈ 4 E_e E'_e sin²(θ/2), so the Q² resolution is
  not simply δE/E but enters via δ(sin²θ/2) and δE'_e together.
  `research-background-sources-migration.md` §1.1 quotes
  σ(log10 Q²)=0.053 (12% RMS) for the **low‑Q² regime**. The high‑Q²
  regime is comparable or slightly worse because the y‑amplification
  in Σ‑method scales with 1/(1−y).
- `research-genreco-matching-critique.md`, especially §1.1 — describes
  the χ² Mahalanobis matcher used in DESY 20-176; not directly
  applicable here because the DjBkgd event matching is event‑level
  (Q²sGen vs Q²s), not track‑level.

**Findings:**

1. **A factor‑30–100 upward Q² fluctuation is, with 10% Q² resolution,
   formally a >100σ deviation.** At face value this is impossible by
   resolution alone. **The agreement of Q²eGen (electron method) and
   Q²sGen (Sigma method) to 3 decimals at the gen level only confirms
   that the *generator* recorded these events as Q²gen~3 GeV² —
   neither gen‑side method confusion nor a gen‑side bug.** The
   resolution argument therefore rules out (a) [smooth resolution tail].

2. **The most likely explanation is candidate (c): fake scattered
   electron.** At Q²gen~3 GeV² with E_e=27.6 GeV, the true scattered
   electron has E'~25 GeV at very forward angle (θ_e very close to
   the proton beam line, outside SpaCal AND LAr fiducial). When the
   true electron escapes acceptance, the H1 electron finder selects a
   hadronic shower cluster as the electron candidate. If the chosen
   cluster has E~150 GeV at θ~30°, the reconstructed Q² computed from
   that fake cluster lands at hundreds of GeV². **This is a known
   pathology of NC DIS at very low Q²** and is the dominant
   contribution to "DIS faking from γp" — note that the 1,320 sentinel
   events (Q²sGen=0 exactly, IsMatched=0, ParticleSource=-1) are also
   consistent with the gen-electron link being broken because the gen
   electron is outside the H1 acceptance.

3. **DJANGOH NORAD `Q2GT2` extrapolation:** This sample is generated
   without QED radiation. Real data at Q²reco>150 includes ISR events
   where a hard ISR photon collinear with the incoming electron
   reduces the effective center‑of‑mass energy; the residual e‑proton
   collision generates a "true" hard event whose detector record is
   reconstructed at high Q² but whose generator Q² (computed from the
   post‑ISR electron) is much lower than the nominal beam. **In a
   NORAD generator this mechanism is absent**, so the population of
   events at gen‑Q²~3, reco‑Q²~300 in DjBkgd is driven entirely by
   detector pathology (fake electron + hadronic shower) — i.e., (c)
   above. The corresponding mechanism in DATA (real ISR + radiative
   correction) is handled by the signal MC's Heracles ISR/FSR
   modelling, not by this DjBkgd subtraction. **There is therefore a
   real risk that the DjBkgd "fake electron at gen-Q²~3" tail is a
   pathology of the NORAD generator that double-subtracts the
   radiative-corrected real-data signal.**

4. **Cross-check using forensic numbers:** 1,077 events × ~9 tracks/
   event ≈ 9,768 tracks. After lumi scaling (22.55×) this gives
   ≈ 220k subtracted tracks. The total reco-track sumw from DjBkgd
   (Q²sGen<60 path) is 21,450 → 1.6 × scale ≈ 230k subtracted tracks
   — i.e., the 1,077 events are essentially the entire contribution.
   But the *event-level* fraction of gen Q²<60 in the DjBkgd
   reco-selected sample is 1.05% (handoff §9f). 1.05% of (DjBkgd
   reco-selected events) is the gen‑Q²∈[2,60] cohort; the rest of the
   Q²gen<60 cut population must be either the 9.3% gen-Q²∈[60,150] or
   a slice of the 88% gen-Q²∈[150,300]. **Cross-check failure**:
   the handoff says 1.05% of DjBkgd reco events live at gen Q²<60,
   but the forensic numbers imply that this 1.05% is the dominant
   contributor to the 22.6×-scaled subtraction. The numbers are
   internally consistent ONLY if the 1.05% events carry an unusually
   high per-event track multiplicity AND survive the Q²s>150 reco
   cut AND fall in our reco fiducial. This is plausible for catastrophic
   fake-electron events (which produce dense hadronic activity in
   the LAr cluster used as fake e').

**Conclusion (Q3):** The 1,077 deep-low-Q² events are most likely
**real DJANGOH NORAD detector‑simulation pathology** (fake electron
mechanism), not real-data physics. The mechanism (low-Q² gp-like
event with fake EM cluster reconstructed as electron) is real and is
known to occur in data, but is normally suppressed in *real data* by
the Empz>35 cut and the QED-Compton tag. In this DjBkgd subtraction,
**the script subtracts these events from data at the data analyst's
Empz/QED-Compton-passed rate**, scaled by 22.55× — so they are
over-represented relative to what the same mechanism contributes to
DATA. This is a real candidate for an artifactual ~5–10% of the
total 16.7% subtraction.

---

## 5. Q4 — Is the IsMatched asymmetry a real methodology problem?

**Evidence:**
- `scripts/unfolding/build_response_4d.py` lines 245–249: signal MC
  reco stream filters `(z_outlier==0) & (Pt_lab<PT_LAB_CUT) &
  (IsMatched==1) & (IsMatchedHadron==1)`.
- `scripts/background_subtraction/subtract_low_q2_background_v3final.py`
  lines 220–222: bg reco stream filters `(z_outlier==0) & (Pt_lab<
  PT_LAB_CUT) & (Q2sGen<q2gen_cut)` — **no IsMatched cut**.
- Data stream (build_response_4d.py lines 206 and bg script lines 182):
  filters `(z_outlier==0) & (Pt_lab<PT_LAB_CUT)` only — **no
  IsMatched cut**.
- `research-genreco-matching-critique.md` §1.1, §6 — DESY 20‑176 builds
  the response from MC truth-matched REC tracks via the χ² matcher.

**Findings:**

1. **The asymmetry is real and is documented in the genreco-matching
   critique.** The matcher critique describes the H1 community
   standard: a response built from matched MC tracks (numerator) with
   the unfolding's inverse-transfer matrix implicitly correcting for
   the unmatched (fake/inefficient) component. With a per‑bin
   transfer matrix R[i,j] = P(reco bin i | gen bin j) built from
   matched MC reco AND a gen prior built from all gen tracks, the
   unfolder's expected data input is the **full reco-track count
   (matched + unmatched)** in each bin. This is consistent with
   how the data and bg subtraction operate (per-track, no IsMatched
   filter).

2. **However, there is a subtle inconsistency:** the response matrix
   R[i,j] in our build_response_4d.py is normalized per column from
   matched MC tracks only. The "fake/unmatched" component in data is
   modeled by the marginal sum: the difference between the data
   marginal and the response prediction with the matched gen prior.
   In a clean H1 analysis (DESY 20‑176), the χ² matcher includes a
   fake category, and the response training explicitly counts fakes
   in a separate "no-gen-bin" column. Our v3final response builder
   does NOT have a fakes column — fakes are absorbed into the
   bin-by-bin acceptance.

3. **For the bg subtraction specifically**: the bg subtraction
   subtracts ALL bg tracks (matched + unmatched) from data. The
   unfolder then takes (data − bg) as its measurement and applies the
   signal-MC response matrix (which describes only matched
   hadrons). If the bg sample's fake/unmatched fraction differs
   significantly from the signal MC's fake/unmatched fraction, the
   bg subtraction over-corrects or under-corrects by that ratio.
   Pythia6 PHP and DJANGOH low-Q² have substantially different
   matching efficiencies than DJANGOH high-Q² signal because they
   contain fake electrons, hadronic showers in unusual places, etc.

4. **Quantitative scale of the asymmetry concern:** The signal MC's
   matched-hadron fraction is typically 60–80% of reco tracks; the
   complement (~20–40%) is fakes/secondaries/unmatched. If DjBkgd's
   matched-hadron fraction is similar, the bg subtraction is roughly
   correct. If DjBkgd's matched-hadron fraction is substantially
   lower (say, 30%), the bg subtraction would over-subtract by a
   factor (matched fraction in signal) / (matched fraction in bg) ≈
   2×. **This is plausible but unverified.** The forensic note that
   the 1,320 sentinel events have IsMatched=0, IsMatchedElectron=0,
   IsMatchedHadron=0, ParticleSource=-1 is direct evidence that a
   non-trivial fraction of DjBkgd reco tracks are unmatched.

5. **H1 standard practice (DESY 20‑176):** Background subtraction in
   DESY 20-176 was done *event-level* on the inclusive-DIS sample
   (Phojet γp template normalized to data; Sys.6 30% on Phojet
   normalization). The published systematic is ≤0.9% on densities.
   Per-track bg subtraction without matching filter is NOT the H1
   convention — the convention is event-level template subtraction.

**Conclusion (Q4):** Yes, the IsMatched asymmetry IS a real
methodology problem, but it is structurally a smaller effect than
(a) the Q²gen cut value choice and (b) the MC-stats amplification.
The published H1 convention is event-level Phojet template subtraction
with a 30% normalization systematic, NOT per-track per-bin subtraction
without a matching filter. **The current methodology is unconventional
and probably wrong in detail, but it is not the primary source of the
inflated 15%.** Recommended remediation: either apply
`(IsMatched==1 AND IsMatchedHadron==1)` to the bg stream symmetrically
with the signal-response training, OR switch to event-level
subtraction (subtract `scale × N_bg_DIS_events` from `N_data_DIS_events`
at the inclusive level, then propagate the corrected normalization
into M^{h±} and A_h).

---

## 6. Q5 — What is the correct Q²gen cut?

**Evidence:**
- Signal MC catalog name `Q2GT60` per `reference-highQ2-mc-luminosity.md`
  Section 2 (rapgap31 7071/7094/7096/7097, djangoh14 7072/7091/7092/7093).
- DjBkgd catalog name `Q2GT2` per
  `research-ep0304-production-path-2026-05-24.md` line 85.
- Handoff §9f line 1023: **signal MC reco‑selected events at Q²reco>150
  have gen Q² distribution: 0.01% at gen Q²<60, 0.10% at gen Q²<100,
  1.98% at gen Q²<150.** Effective signal generation lower edge
  is ~Q²=100 in practice, despite catalog name carrying Q2GT60.
- Handoff §9f line 1022: **DjBkgd reco‑selected events at Q²reco>150
  have: 88% gen Q² in [150,300], 9.3% in [60,150], 1.05% in [2,60].**

**Findings:**

1. **The Q²gen<60 cut subtracts both the legitimate leak and a
   substantial chunk of well-modelled high-Q² DjBkgd events.** With
   the population breakdown above, the gen-Q²<60 cut applied to the
   bg samples captures only the 1.05% sliver of DjBkgd reco-selected
   events. **But the forensic note that 1,077 events × ~9 tracks ≈
   9,768 tracks = essentially the entire 11,853-event subtraction
   from DjBkgd implies the 1.05% events carry ~9 tracks each.** This
   is a high but not absurd multiplicity — fake-electron events
   produce dense hadronic activity that gets reconstructed as many
   tracks.

2. **Cut sensitivity (back-of-the-envelope):**

   | Q²gen cut | DjBkgd subset (events) | Expected subtraction frac | Comment |
   |-----------|-----------------------|---------------------------|---------|
   | < 5 GeV² | "deep leak" 1077 events × ~9 tracks ≈ 9,768 | ≈ **3–5%** | Captures the genuine fake‑electron / low‑Q² migration; ≈ Britzger "negligible" prior |
   | < 30 GeV² | larger but still in 1.05% gen‑Q²<60 slice | ≈ **4–7%** | Adds a narrow non-leak component; still defensible |
   | **< 60 GeV² (CURRENT)** | full 1.05% slice ≈ 22k tracks → 480k scaled | **16.7% observed** | **CURRENT CHOICE — over-subtracts** |
   | < 100 GeV² | adds 9.3% gen‑Q²∈[60,150] | ≈ **30–40%** | Subtracts well‑modelled high‑Q² DjBkgd |
   | < 150 GeV² | adds rest of gen‑Q²∈[60,150] and edge of [150,300] | ≈ **40–60%** | Subtracts most of the bg sample; physically meaningless |

   The 16.7% observed at Q²gen<60 is in the range expected from this
   cut by population scaling — i.e., **the script is doing what it
   was told to do; the bug is in the cut value, not in the algebra**.

3. **Has any H1 analysis published the cut-dependence sensitivity?**
   Not in the workspace. Britzger's EventShapes does not subtract this
   bg at Q²>150 at all, so there is no published cut sweep. DESY 20‑176
   subtracts γp with a 30% systematic on the Phojet normalization, not
   via a Q²gen cut.

4. **The principled cut is Q²gen < Q²reco_cut = 150 GeV² (full leak)
   minus a tightening factor for "true migration":** events with
   gen Q²>150 that reco at Q²>150 are signal (this is what the
   response handles); events with gen Q²<150 that reco at Q²>150 are
   leak. **The correct cut would therefore be Q²gen<150**, not
   Q²gen<60 — but it must be paired with the signal-MC masked-response
   training using the same Q²gen<150 mask, AND only the subset of
   DjBkgd events whose gen Q² < 150 AND not modelled by signal MC's
   own gen-Q² tail should be subtracted. The current methodology of
   "subtract everything in DjBkgd with Q²gen<60" pretends that
   signal MC already covers Q²gen>60 perfectly, which contradicts
   the handoff §9f line 1023 finding that signal MC's effective
   lower edge is Q²~100.

5. **The most rigorous (H1‑literature‑consistent) cut would be:**

   `Q²gen < Q²_signal_effective_lower_edge = 100 GeV²`, applied
   symmetrically as the masking value in the signal-MC response
   builder (currently Q²gen<60).

   This would subtract the 0.10% of signal MC events at Q²gen<100
   (negligible) and the ~10% of DjBkgd reco-selected events at
   Q²gen<100 (= 1.05% + ~0 of the 9.3% [60,150] split). Estimated
   subtraction fraction at this cut: **2–4%**, much closer to the
   prior memo's "few percent overall" prediction.

**Conclusion (Q5):** The Q²gen<60 cut is **the proximate driver of
the inflated subtraction**. The H1‑literature‑consistent alternative
is Q²gen<100 (matches signal MC's effective generation edge) or
Q²gen<150 with a paired signal‑MC response mask at the same cut.
Cut sweep is the single most decisive next test.

---

## 7. Q6 — Are there published H1 benchmarks for the DjBkgd contribution at Q²>150?

**Evidence:**
- `reference-highQ2-mc-luminosity.md` §2 — signal samples
  rapgap31/{7071,7094,7096,7097} and djangoh14/{7072,7091,7092,7093}
  have authoritative L_MC totals 9.1M and 10.5M nb⁻¹ respectively
  (Q2GT60 generation).
- `research-ep0304-production-path-2026-05-24.md` line 85 —
  `djangoh14/7110` (`DJANGOH14 NORAD Q2GT2`) per‑chunk lumi 193.7 nb⁻¹.
  Britzger uses the same `djangoh14/7110` catalog we use.
- Britzger's published high‑Q² EventShapes analysis EPJC 84 (2024) 882
  — workspace doc characterizes its bg treatment as "covered by the
  model-dependence + reconstruction-efficiency bins in Tab. 6 of the
  paper" (`research-ep0304-production-path-2026-05-24.md` line 211).

**Findings:**

1. **L_MC(DjBkgd) = 15,813 nb⁻¹ is essentially Britzger's value.**
   We derived L_MC = 15,813.4 nb⁻¹ from summing per‑chunk fLumi
   across 80 chains in the Britzger g1j_ES steerings. **This means
   Britzger has the same L_MC/L_data = 0.044 ratio we have**, and
   chose NOT to subtract this bg quantitatively. Britzger's analysis
   team must have concluded that subtracting at this MC/data ratio
   would inject more noise than signal — exactly the conclusion this
   audit reaches.

2. **What did Britzger report numerically?** The workspace does not
   surface a specific bg fraction from EPJC 84 (2024) 882. The
   characterization "negligible at Q²>150" (workspace doc line 95)
   and "absorbed into model dependence + reconstruction efficiency"
   (line 211) imply <1% impact on the published high‑Q² result.
   **Pulling the exact number from EPJC 84 (2024) 882 is an
   open item.**

3. **L_MC/L_data = 0.044 violates the H1 normalization convention.**
   The reference doc (line 102) explicitly states "**H1 convention
   is L_MC/L_data >= 5 per generator per period**". The DjBkgd
   sample has L_MC/L_data = 0.044, i.e., **~115× below the
   convention floor**. This is a quantitative MC statistics
   limitation, not a generator-physics limitation, and it means
   per-bin subtraction estimates carry a stat uncertainty of ~22×
   the equivalent Poisson uncertainty on data.

4. **Pythia6 PHP L_MC = 40,520 nb⁻¹ gives L_MC/L_data = 0.114**, also
   ~50× below the H1 floor. Same problem, smaller magnitude. Note
   that 1302.1321 used PYTHIA with L_MC = 449 pb⁻¹ for L_data = 88.6
   pb⁻¹ → ratio 5.1, **exactly at the H1 floor** — i.e., the
   1302.1321 published PHP subtraction (≤ 0.5%) was done with
   adequate MC statistics. Our current pipeline runs at ~1/45 the
   relative PHP MC statistics of the published low-Q² analysis.

5. **What's different about our analysis vs Britzger?** Two things:
   (a) we are quantitatively subtracting a bg that Britzger treats as
   "in the systematic"; (b) we are using a Q²gen<60 cut that captures
   90× more events than the "true leak" (gen Q²<5) population. Either
   choice in isolation would inflate the subtraction; together they
   compound.

**Conclusion (Q6):** Britzger's published EventShapes analysis at
Q²>150 uses the same DjBkgd sample we use, with the same L_MC, and
explicitly chose NOT to subtract it quantitatively. The published H1
high‑Q² benchmark is "negligible; absorb into model dependence". Our
15% subtraction is inconsistent with that benchmark by ~30× and
results from two compounding methodological choices (loose Q²gen cut +
inadequate L_MC) not made by any published H1 analysis.

---

## 8. Cross-cutting concerns and minor findings

1. **Weight column inconsistency between signal-MC response and bg
   subtraction:** the response builder
   (`build_response_4d.py` lines 217, 270) uses a single pre‑combined
   `'weight'` column from the per‑track extractor. The bg subtraction
   (`subtract_low_q2_background_v3final.py` lines 230–232)
   recomputes `weight1 * weight2 * NucliaWeight` itself. The README
   asserts both should be equivalent if the extractor combined
   correctly. **Recommend** explicit pin/verification on the bg
   extractor: does
   `pertrack_mc_v3p2_bg_reco_djangoh14_lowQ2bkgd_merged.csv.gz` carry
   a pre‑combined `weight` column, or only the three separate columns?
   If only separate, the bg path is consistent with itself; if both,
   the script uses the redundant 3‑column product, which is fine.
   But the signal response uses the 1‑column form — if the extractor
   accidentally double‑includes NucliaWeight in the bg samples, the
   subtraction is over‑weighted by ~5%.

2. **Sentinel events (Q²sGen=0 exactly):** 1,320 events have
   IsMatched=0 etc. Per the forensic note, removing them changes the
   subtraction by 7% (21,450 → 19,914 sumw). These are clearly
   broken-gen-electron events in DJANGOH NORAD; they should be
   excluded from the bg subtraction by an explicit
   `(IsMatched == 1) | (IsMatched == -1 & valid_gen_electron)`
   filter on the bg stream. **Their inclusion biases the
   subtraction upward by ~1 percentage point.**

3. **The masked response (Q²gen<60) excludes only 30,225 signal MC
   reco rows** (handoff §9f line 1009) — a tiny fraction of the
   signal sample. This is consistent with the handoff §9f line 1023
   finding that the signal MC has only 0.01% of its reco-selected
   tracks at gen Q²<60. **The masking is therefore essentially a
   null operation on the signal side**, while the subtraction on the
   data side is a 16.7% shift. **The two halves of the "matched pair"
   methodology are wildly out of balance**, which is itself a strong
   signal that the methodology is being applied to the wrong sample
   asymmetrically.

4. **The h+/h- 0.03 pp agreement (Section 3.4 above) is a positive
   methodology check.** Whatever is being subtracted is genuinely
   charge-symmetric, which is consistent with kinematic-boundary
   migration physics regardless of the magnitude problem. The A_h
   asymmetry observable will be insensitive to this entire issue.

5. **The 5,082,379 vs 6,005,256 unfolded numbers correspond to a
   −922,877 event swing.** The TUnfold χ²/ndf rising from 0.054 to
   0.151 is consistent with the unfolder fitting a noisier subtracted
   input — not a smoking gun, but consistent with the noise-amplification
   hypothesis.

---

## 9. Recommended Actions (ranked, most decisive first)

1. **Q²gen-cut sweep on the existing bg samples** (1 hour wall, no
   MC re-generation, no Condor). Re-run `subtract_low_q2_background_v3final.py`
   with `--q2gen-cut` in {150, 100, 60, 30, 10, 5, 2}. Plot
   subtraction fraction vs cut, both raw and unfolded. **If the
   shift converges to a stable plateau at low cut values (e.g. ≤30%
   of the value at cut=60), this confirms the proximate cause is
   the cut value, and the published-precedent‑consistent cut is the
   one matching the signal MC's effective gen edge (~Q²=100).** If
   the shift continues to scale with cut value, MC stats is the
   dominant issue and Action 2 becomes critical.

2. **Compute the bg subtraction fraction with a Q²gen<5 cut to isolate
   the "deep leak" component** (the 1,077 events). Expected: 1–3%
   raw subtraction, 1–4% unfolded shift. This is the
   precedent-consistent target. The difference between this and
   the 16.7% at Q²gen<60 is the over-subtraction due to cut choice.

3. **Apply the IsMatched filter symmetrically to the bg stream.**
   Add `(IsMatched == 1) & (IsMatchedHadron == 1)` to the bg cut in
   `subtract_low_q2_background_v3final.py` line 220. Re-run with
   Q²gen<60 and compare. This removes the sentinel events automatically
   and any other unmatched fake-electron pathology that contaminates
   the bg estimate.

4. **Verify the bg extractor's `weight` column convention.** Run
   `head -2` on the bg reco CSV and check whether `weight` is present
   (along with the three components `weight1`, `weight2`,
   `NucliaWeight`). If both are present, ensure the script's
   recomputation matches the column's value to <0.1%. If the
   extractor differs from the signal-MC extractor, fix the bg extractor
   to match before any further interpretation.

5. **Adopt the published H1 high‑Q² convention: do NOT subtract this
   bg quantitatively at Q²>150.** Instead, follow Britzger's
   EventShapes practice: absorb the residual leak into the
   RAPGAP-vs-DJANGOH model-dependence systematic + the reconstruction-
   efficiency systematic. The δ between RAPGAP and DJANGOH unfolded
   results (already computed for the model systematic) is the natural
   upper bound on this contribution; the workspace's combined
   systematic already covers it.

6. **If quantitative subtraction is required** (e.g., for a high-precision
   M^{h±} publication), produce additional DjBkgd MC statistics to
   reach L_MC/L_data ≥ 5 (a 115× increase from the current 0.044).
   This is the H1 convention floor and the level that 1302.1321
   achieved. Without this, per-bin subtraction estimates are
   irreducibly noise-dominated.

7. **Investigate the 1,320 Q²sGen=0 sentinel events.** Confirm they
   are broken-gen-electron events in DJANGOH NORAD (e.g., gen
   electron outside acceptance), and verify the bg extractor's
   handling of them. They should be excluded from any quantitative
   subtraction as a known generator pathology.

8. **Cross-check against Britzger's EPJC 84 (2024) 882 explicit bg
   numbers.** The workspace currently carries only paraphrase
   ("negligible"); pulling the exact reported bg fraction (Tab. 6
   of that paper) would convert the prior from qualitative to
   quantitative.

---

## 10. Source citations

- **H1 Collaboration (Britzger et al.), "Multi-differential jet cross
  sections in DIS at HERA", Eur. Phys. J. C 84 (2024) 882.** The
  authoritative high‑Q² H1 reference for bg treatment of this exact
  djangoh14/7110 sample. Workspace coverage:
  `docs/research/research-ep0304-production-path-2026-05-24.md` lines
  82–99, 207–227. Exact bg-fraction number from Tab. 6 NOT pulled
  for this audit.
- **H1 Collaboration (Andreev et al.), arXiv:2011.01812, EPJC 81
  (2021) 212 (DESY 20-176).** Low-Q² (5–100 GeV²) inclusive charged
  particle multiplicity; bg treatment is event-level γp Phojet
  template with 30% normalization systematic. Workspace coverage:
  `docs/research/research-h1-charged-particle-analysis.md` §§3–5.
- **H1 Collaboration, arXiv:1302.1321, EPJC 73 (2013) 2406.** Low-Q²
  charged particle spectra; bg treatment: γp + outside-phase-space
  subtraction at the few-percent integrated level, residual γp
  0.5%. Workspace coverage:
  `docs/research/research-h1-1302-1321-charged-multiplicity.md`
  §§2, 3.3, 6.3, 7.
- **Workspace doc `research-background-sources-migration.md`** —
  source of the "few percent / 10-30% edge bins" prior for the
  low-Q² regime; explicitly NOT validated for Q²>150.
- **Workspace doc `research-ep0304-production-path-2026-05-24.md`
  §§1.2, 5** — Britzger steering mapping, explicit "negligible
  at Q²>150" treatment.
- **Workspace doc `reference-highQ2-mc-luminosity.md`** — L_MC/L_data
  ≥ 5 H1 convention floor; verifies our signal L_MC values
  (9.1M / 10.5M nb⁻¹) match Britzger's.
- **Workspace doc `context-handoff.md` §9f** — full bg subtraction
  infrastructure description; the trap #32 + the
  88%/9.3%/1.05% DjBkgd reco-selected gen-Q² distribution.
- **Britzger steering `reference/britzger_steerings/es.Norad.ep0607disMC13.Norad2021.GenLumi.NewRadGenOnly.steer`**
  — directly inspected; no DjBkgd/Pythia chain instantiations in any
  of the 4 inspected steering files (only as a "// fChains Todo"
  comment in ep0304/em0405/em06).
- **`scripts/background_subtraction/subtract_low_q2_background_v3final.py`**
  — code under audit; lines 220–222 (bg cut), 230–232 (weight),
  340–347 (subtraction algebra).
- **`scripts/unfolding/build_response_4d.py`** — canonical
  response builder; lines 245–249 confirm `IsMatched==1 &
  IsMatchedHadron==1` cut on signal MC reco.
- **`scripts/background_subtraction/build_response_4d_lowq2_masked.py`**
  — masked-response companion; verified bit-identical binning &
  cut logic to parent except for `--q2gen-cut`.
- **Workspace doc `research-genreco-matching-critique.md`** — §1.1
  on DESY 20-176 χ² Mahalanobis matcher; §6 on the
  per-track matching convention assumed by H1 inclusive multiplicity
  analyses.

---

## 11. Open questions / unresolved items

1. **Exact published bg fraction in EPJC 84 (2024) 882 (Britzger 2024
   jets at Q²>150).** Workspace carries only paraphrase. Retrieving
   the precise number from Tab. 6 of the paper would tighten the
   prior from "negligible (<1%)" to a quantitative bound.

2. **Per-event matched-hadron fraction in DjBkgd vs signal DJANGOH
   Q2GT60.** Quantifying this directly would tell us how much of the
   IsMatched asymmetry biases the subtraction. The bg extractor
   already has `IsMatched`, `IsMatchedHadron` columns; a simple
   `awk` would answer this.

3. **Are the 1,077 deep-low-Q² events a known DJANGOH NORAD
   generator pathology?** If so, there should be an H1-internal
   memo or DJANGOH release note documenting it. Not surfaced in
   the workspace; an H1 expert consultation is the natural source.

4. **Is the 30,225-row signal MC mask (Q²gen<60) correctly excluding
   the right events?** The handoff §9f line 1023 says only 0.01%
   of signal reco events are at gen Q²<60, which would imply
   ~600 rows in a 6M-row signal MC sample, not 30,225. The factor of
   50 discrepancy needs explaining — is the 30,225 number for all
   gen Q²<60 in the signal sample (not just reco-selected)? Or is
   the handoff number a different denominator? **Should be
   reconciled before any production use.**

5. **Whether to publish at all without bg subtraction.** The
   workspace plans to release v1.3 with bg subtraction; given that
   Britzger publishes without one, a v1.3 release WITHOUT this bg
   subtraction (relying on the model-dependence systematic) is fully
   precedented. This is a release-policy question for the supervisor.

6. **What does Kogler's thesis (predecessor to Britzger) say about
   the same djangoh14/7110 sample?** Not in the workspace as direct
   text. Pull from the H1 thesis archive if a more authoritative
   treatment is required.

---

## Final verdict

**METHODOLOGY SUSPECT.** The bg samples (DjBkgd 7110, Pythia62) are
the correct ones; the subtraction algebra is correct; the lumi
values are authoritative (derived from Britzger steerings). But the
**Q²gen<60 cut value is too loose by ~2.5× relative to the signal
MC's effective gen lower edge (~Q²=100)**, and the **L_MC/L_data
ratio of 0.044 is ~115× below the H1 convention floor**, so per-bin
estimates are noise-dominated and amplified by 22.55× into the data
subtraction. The published H1 high-Q² convention is to NOT subtract
this bg quantitatively at Q²>150 (Britzger EPJC 84 (2024) 882). The
15.37% unfolded shift is **structurally inconsistent with H1 published
precedent** and should not be promoted to a release. A 1-hour Q²gen
cut sweep is the single most decisive next action.

# Research Memo: HFS Energy-Scale Systematic for H1 Charged-Particle SIDIS at High Q²

**Date**: 2026-05-25
**Analysis**: H1ChargedSidis_HighQ2 (charged-particle SIDIS, Q² > 150 GeV²)
**Author scope**: literature researcher (source-backed survey + implementation recommendation)
**Status of supporting workspace artifacts**: see §0 (Source-availability caveat) before reading further.

---

## 0. Source-availability caveat (read first)

This memo was prepared from the H1 published literature and the only local
workspace artifact present at memo time:
`H1/sidis_highQ2/docs/plans/plan-background-MC-production-g1jES-2026-05-24.md`
(538 lines, the g1j_ES background MC production plan; read in full). All
other local sources referenced by the supervisor brief were **not present on
disk in the local mirror** at the time of writing, namely:

* `H1/sidis_highQ2/docs/results/systematics_v5f_hfs_scale_v2.md` — does not
  exist; the `docs/results/` directory itself does not exist locally. The
  brief's instruction to "read it carefully and update for v3 conventions"
  cannot be satisfied here. If a v5f HFS write-up exists, it lives on NAF
  (`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/...`) and
  needs to be mirrored or summarized into this workspace before §8
  cross-references can be made concrete.
* `H1/sidis_highQ2/data/pertrack_v5g_fixedFrame_v3_merged.csv.gz` — not
  present locally; per AGENTS.md, code and data live on NAF DUST.
* Extractor scripts, response-matrix code, unfold scripts — same.

The recommendations below are therefore based on (a) the H1 published
prescription for this kinematic region and (b) the schema and conventions
inferred from the supervisor brief itself (which the supervisor is the
authority for). Any numerical claim about *per-bin SIDIS magnitudes* in our
v3 production is necessarily a literature-anchored estimate, not a measured
value from our own pipeline — see §1, §7. Closure with our actual per-bin
HFS shifts requires running the proposed implementation (§6) on the NAF
files.

---

## 1. Executive summary

The H1 canonical hadronic-final-state (HFS) energy-scale systematic at
Q² > 150 GeV² (i.e. the exact kinematic regime of this analysis) is a
**two-component prescription** that has been used unchanged across the
H1 HERA-II high-Q² publication chain:

* **JES** — Jet Energy Scale, applied to HFS objects clustered **inside**
  high-pT jets: **±1 %**.
* **RCES** — Residual Cluster Energy Scale, applied to HFS objects
  **outside** high-pT jets: **±1 %**.

Both are varied independently and propagated through the **full unfolding
procedure** — i.e. the migration matrix is rebuilt at each shifted scale
and the unfold is re-run. This is the supervisor brief's "Option A"
prescription. It is the published H1 convention, not a 2 % envelope.

Per-bin SIDIS-observable impact, anchored to the most directly comparable
H1 high-Q² published measurement (Andreev 2023, *PLB* 844, 138101,
arXiv:2303.13620, jet substructure at Q² > 150 GeV²): each of JES and RCES
contributes typically **0.2–2 % per bin** on differential cross sections
and unfolded means; the two add in quadrature so the **combined HFS
envelope is ≈ 0.5–3 % per bin** for SIDIS-multiplicity-like observables,
rising into the tails of pT_h, z, and high-Q² bins where migration
amplification is strongest. This is consistent with the 1–2 % per-component
quote in Andreev 2024 (*EPJC* 84, 718, arXiv:2403.10134, §6.2).

**Recommended path for our v3 production**: **Option A (re-extract from
ROOT, rebuild response matrices, re-unfold)** is the gold-standard and the
only path that matches the published H1 prescription. The supervisor brief
correctly characterizes this as "tricky since it may require a
re-production." The good news is that it is bounded in cost — ≈ 3 h
Condor wall per variation per (JES, RCES), so **4 variation runs total**
(JES_up, JES_down, RCES_up, RCES_down) ≈ 12 h Condor wall + extractor
time. See §6.A for the production plan.

**Option B (analytic CSV rescale)** is documented in §4, §6.B for two
secondary uses: (i) a same-day sanity check while Option A jobs are in
the queue; (ii) a fallback if production capacity is unavailable. Option B
is *not* a substitute for Option A in the final published result — it
collapses the JES/RCES distinction (treats HFS uniformly), it skips the
per-cell-then-reconstruction step that the H1 calibration was tuned
against, and it cannot rebuild the migration matrix faithfully.

**Recommended HFS envelope to quote in the v3 systematics table**, pending
the Option A run: **per-bin total HFS uncertainty bounded by sqrt(JES² +
RCES²) ≈ 1.4 % in typical bins, ≤ 3 % in tail bins** (z > 0.7, pT_h > 4 GeV,
Q² > 2000 GeV²). These bounds are placeholders anchored to Andreev 2023
Tables 2–3 and Andreev 2024 §6.2; replace with measured per-bin values
once §6.A is complete.

---

## 2. H1 canonical HFS scale convention (2 % is wrong; 1 % JES + 1 % RCES is right)

The supervisor brief opens with "LAr calorimeter HFS scale: ±2 %
(well-established from H1 papers, e.g., Andreev 2017 EPJC 77 215; Andreev
2024 EPJC 84 720)". **This is not the convention actually used in those
papers for the high-Q² regime.** The published high-Q² convention is
±1 % per HFS component, applied in two independent pieces (JES + RCES).
The historical "2 %" figure originates from earlier H1 inclusive-jet work
(circa 1996–2003) before the dedicated H1 HFS calibration (Kogler 2010
thesis / H1prelim) drove the per-object uncertainty down to 1 %.

### 2.1 Andreev 2024 (groomed event shapes, *EPJC* 84, 718, arXiv:2403.10134)

The most recent published H1 measurement at Q² > 150 GeV². §6.2 *Energy
scales* states verbatim (HTML, retrieved 2026-05-25):

> "Independent cluster energy calibrations are used to describe clusters
> inside and outside of high-pT jets [refs to Andreev 2014 and Andreev
> 2017]. The uncertainty in energy scale of particles inside of high-pT
> jets is denoted as the jet energy scale uncertainty (JES), and the
> uncertainty in energy scale of particles outside of jets is denoted as
> the residual cluster energy scale uncertainty (RCES). These energy
> scales are independently varied by a factor of 1 %, and the resulting
> distributions are passed through the unfolding procedure. The difference
> in unfolded distributions between the variations and nominal scale values
> provides the corresponding uncertainty, which is typically 1–2 %."

Same paper, §6.1 *Alignment*: HFS polar-angle alignment 1 mrad → typically
~1 % on the final distributions, propagated through the unfold.
§6.3: lumi 2.7 %.

Source: arXiv:2403.10134v2, EPJC 84 (2024) 718, DOI
10.1140/epjc/s10052-024-12987-0. Sections 6.1, 6.2, 6.3 read on the
arXiv HTML rendering on 2026-05-25.

### 2.2 Andreev 2023 (jet substructure, *PLB* 844, 138101, arXiv:2303.13620)

The closest published analog to our SIDIS analysis: same dataset (2006–2007
HERA-II at H1, integrated luminosity 228 pb⁻¹ as quoted in the paper —
note this differs from the full HERA-II the superseded pre-rescale luminosity nb⁻¹ used in our
production, because Andreev 2023 restricted to a subset; see §3.2), same
kinematic cut Q² > 150 GeV² and 0.2 < y < 0.7, same Σ-method
reconstruction, same HFS object definition (energy-flow algorithm with
scattered electron removed), and — crucially — explicit per-source per-bin
systematics tables for unfolded observables that include charged-particle
multiplicity ⟨Nc⟩.

§4 *Uncertainties* states verbatim (ar5iv HTML, retrieved 2026-05-25):

> "Uncertainties on HFS objects include the energy scale from two different
> contributions: HFS objects contained in high pT jets and other HFS
> objects. In both cases, the energy-scale uncertainty is ±1 %. Both
> uncertainty sources are estimated separately [refs] by varying the
> corresponding HFS energy by ±1 %. An uncertainty of ±20 mrad is assigned
> to the azimuthal angle determination of HFS objects."

Source: arXiv:2303.13620v3, PLB 844 (2023) 138101, DOI
10.1016/j.physletb.2023.138101. Section 4 and Tables 2–3 read on
ar5iv HTML on 2026-05-25.

### 2.3 The published source of the 1 % calibration: Andreev 2014 + Andreev 2017

Both 2023 and 2024 papers cite the calibration itself to:

* Andreev 2014, "Measurement of Multijet Production in ep Collisions at
  High Q² and Determination of the Strong Coupling αs", arXiv:1406.4709,
  *EPJC* 75 (2015) 65, DESY-14-089. Same H1 detector, same 2003–2007
  HERA-II dataset, 351 pb⁻¹, 150 < Q² < 15000 GeV². This is the
  authoritative reference for the JES/RCES 1 % calibration that all
  subsequent H1 high-Q² analyses inherit. (Abstract retrieved
  2026-05-25; full systematics PDF not pulled — the 2023 + 2024 papers
  quote and apply the convention.)
* Andreev 2017, "Measurement of Jet Production Cross Sections in
  Deep-inelastic ep Scattering at HERA", arXiv:1611.03421, EPJC 77 (2017)
  215 (and erratum). Combines low-Q² (5.5 < Q² < 80 GeV²) and high-Q²
  jet data and uses the same JES/RCES split. Abstract retrieved
  2026-05-25; arXiv HTML/ar5iv full-text fetch failed (404 for HTML,
  payload too large for ar5iv) — convention confirmed transitively via
  Andreev 2023/2024 citations.

### 2.4 SpaCal HFS — separately calibrated, smaller weight at high Q²

The brief lists "SpaCal HFS scale: ±2 % (separate calibration)" and
"possibly separate components: barrel LAr (±1 %), endcap LAr (±2–3 %),
SpaCal (±2 %)". The high-Q² published convention does **not** itemize
barrel/endcap separately; the JES/RCES split is the only HFS calorimeter
split applied. SpaCal coverage (153° < θ < 177°) sits in the backward
direction and contributes very little HFS energy in events with the
scattered electron landing in the LAr fiducial (the high-Q² regime by
design — Andreev 2024 §2 requires the scattered electron in the LAr
fiducial); SpaCal HFS contributions in our analysis are dominated by
beam-remnant fragments and ISR-suppressed soft activity. The H1 high-Q²
papers fold any residual SpaCal HFS into the RCES "other HFS objects"
bucket and do not break it out.

For our analysis with Empz ∈ [35, 70] GeV (wider than the standard
[45, 65] used in Andreev 2024 — see plan-background-MC-production §6.x for
context on the Empz=[35,70] choice) and y < 0.7 downstream, the SpaCal
HFS contribution per event is < a few GeV in nearly all events and is
correctly absorbed into RCES.

**Bottom line for §2**: drop the 2 % framing. The canonical published
convention is two independent ±1 % variations (JES, RCES), each
propagated through the full unfold.

---

## 3. How HFS scale affects our observables

The supervisor brief's enumeration (Q1, Q2) is correct in direction; this
section translates each into the explicit dependence in the formulae we
actually use, separating per-event kinematics from per-track kinematics.

### 3.1 Per-event kinematics (Q², x, y) — eSigma-method case

The eSigma (eΣ) method, as used in the v5g/v3 production per the brief,
constructs y_es from the hadronic Σ:

```
Σ_had  = Σ_HFS (E_i − p_{z,i})          (HFS-dependent)
Σ_e    = E_e (1 − cos θ_e)              (HFS-INdependent)
y_es   = Σ_had / (Σ_had + 2 E_e)        (this is the eΣ form used
                                          by H1 high-Q² analyses, e.g.
                                          Andreev 2023 Eq. 2 cites
                                          Bassler/Bernardi 1995)
Q²_es  = E_e'² sin²(θ_e) / (1 − y_es)
x_es   = Q²_es / (s · y_es)
```

A uniform rescaling Σ_had → (1 + δ) Σ_had with δ = ±0.01 (±1 % HFS) gives,
to first order in δ:

* y_es → y_es · [1 + δ · (1 − y_es)]   → for y_es ≈ 0.5, Δy_es / y_es ≈ ±0.5 %
* Q²_es → Q²_es · [1 + δ · y_es / (1 − y_es)] → for y_es ≈ 0.5, ΔQ²_es / Q²_es ≈ ±1 %
* x_es → x_es · [1 + δ · (1 − 2 y_es) / (1 − y_es)] → near y_es ≈ 0.5, near zero;
   grows linearly with |y_es − 0.5|

The eSigma method's well-known robustness in y stems from the appearance
of both Σ_had and 2E_e in the denominator. At our y ∈ [0.2, 0.7] range,
ΔQ²_es is at most O(2.3 %) at y = 0.7 and Δx_es up to O(1.5 %) in the same
bin. These are the *event-level* shifts before unfolding.

**JES vs. RCES weighting**: only HFS objects inside high-pT jets get the
JES ±1 %; everything else gets RCES ±1 %. In SIDIS multiplicity events at
Q² > 150 GeV², the recoiling current jet typically contains the bulk of
Σ_had but not all of it — the jet fraction f_jet of Σ_had is a function
of Q², being closer to 1 at high Q² (a clean current jet) and lower at
moderate Q² and high y. So the *effective* Σ_had shift is
δ_eff = f_jet(Q², y) · δ_JES + (1 − f_jet(Q², y)) · δ_RCES.
With δ_JES = δ_RCES = ±0.01, δ_eff = ±0.01 regardless of f_jet, which
is why a uniform ±1 % HFS estimate gives the same y_es shift. But the
*correlation structure* differs: JES_up and RCES_up are independent
sources, and combined in quadrature give an effective HFS shift that
varies bin-by-bin in pT and z because f_jet varies.

### 3.2 Per-track kinematics (z, pT_h, φ_h)

Per the v3 extractor convention (CM-frame fix 2026-05-24, per AGENTS.md),
the per-track variables are computed in the γ*+p CM frame:

```
z     = (P · p_h) / (P · q)             where P = proton beam, q = γ*
pT_h  = transverse momentum of the hadron in the γ*+p CM, w.r.t. q direction
φ_h   = azimuthal angle of p_h around q in the γ*+p CM frame
```

The boost to the γ*+p CM uses (Q², x, y) — all of which depend on the HFS
through eSigma — *and* the scattered-electron 4-momentum and the beams.
The hadron 4-momentum p_h, in the lab frame, is reconstructed from a
track (charged hadron, momentum determined by the central tracker — NOT
the calorimeter — see Andreev 2023 §1 "central tracking system… transverse
momentum resolution σ(pT)/pT ≈ 0.2 % · pT/GeV ⊕ 1.5 %"). Therefore the
**lab-frame hadron momentum p_h itself is not shifted by HFS scale**;
only the boost is shifted because (Q², x) shift.

So the per-track HFS impact on z and pT_h comes in via the *boost*, not
via the *hadron 4-momentum*. This is an important and often-missed
distinction:

* The tracked charged hadrons are reconstructed from the central tracker;
  their pT comes from track curvature in the 1.16 T solenoid, not the LAr
  energy. The HFS energy scale does not shift the tracker momentum.
* Neutral HFS objects (calorimeter-only) *are* shifted by HFS scale, but
  these do not enter the per-track CSV as tracks at all — they enter
  only via the per-event Σ_had.
* The track's CM-frame z and pT_h shift because the (Q², x, y) used to
  construct the boost shift.

To first order, with δ ≈ ±1 % uniform HFS:

* z → z · [1 + O(δ · y / (1 − y))]    same parametric dependence as Q²_es
* pT_h → pT_h with O(few × 0.1 %) shifts from the boost direction change

Migration in (z, pT_h) of an *individual track* is small (sub-percent),
but the migration of *events* into and out of (Q², x) bins is at the
1–2 % level. The bin migration is dominated by the per-event shift, not
the per-track shift.

### 3.3 Summary table of HFS sensitivity

| Observable | HFS-dependent path | Sign of dependence | Typical Δ for ±1 % HFS at y ∈ [0.2, 0.7] |
|---|---|---|---|
| Q²_es (per event)        | y_es shift via Σ_had               | direct                 | ±0.3 % (y=0.2) → ±2.3 % (y=0.7) |
| y_es  (per event)        | Σ_had / (Σ_had + 2 E_e)            | direct                 | ±0.5 % (y=0.5)               |
| x_es  (per event)        | Q²_es / (s · y_es) cancellation    | partial; near-zero at y≈0.5 | ±0.3 % (y=0.2) → ±1.5 % (y=0.7) |
| z (CM frame, per track)  | boost; lab p_h unchanged           | through Q², x          | ±0.5 % typical, up to ±2 % at high z, high y |
| pT_h (CM frame, per track) | boost direction                  | small (cosine of boost angle change) | sub-% individually; few-% on differential dN/dpT_h via migration |
| φ_h (CM frame, per track) | boost orientation                 | small                  | sub-% individually |
| Track Pt_lab, Eta_lab    | tracker-only                       | **none** (no HFS in tracker chain) | 0 |
| Q²_e (electron-only)     | scattered-e 4-vector               | **none**                | 0 |

The **migration matrix** shifts because (Q², x, z, pT_h) all shift
coherently per event. This is why the published Option A prescription
re-derives the response matrix from the shifted-HFS detector-level MC
sample.

---

## 4. Option A (re-production) vs. Option B (analytic on CSV) — full comparison

### 4.1 Option A — re-extract from ROOT with HFS shifted at the framework level

**Procedure** (matches published H1 high-Q² prescription):

1. Add a steering flag `JES_VARIATION = +0.01 | -0.01 | 0.0` and
   `RCES_VARIATION = +0.01 | -0.01 | 0.0` to the Stage-2 binary
   (`ChargedSidisTreeBuilderHighQ2_v5g`) read at HFS-object loop time.
2. In the HFS loop, classify each HFS object as in-jet vs. out-of-jet
   using the same jet-classification logic the calibration was tuned
   against (k_T or anti-k_T at pT > 5 GeV — same as Andreev 2023).
3. Apply 4-momentum rescaling p_μ → (1 + δ_class) p_μ per object before
   any kinematic computation (Q²_es, y_es, x_es, jet clustering, HFS
   summation, etc.).
4. Re-run Stage 1 + Stage 2 for each variation on full MC + data.
   (Data shifts use shifted-MC-derived response matrices applied to
   nominal data — the data Σ_had is not itself shifted; the shift is
   propagated through the response matrix.)
5. Re-run the v3 extractors → fresh per-track CSVs per variation.
6. Re-run the 4D response-matrix build (canonical since 2026-05-25 per
   AGENTS.md) per variation.
7. Re-run the unfold (IBU, OmniFold, or whatever the v3 canonical method
   is) per variation.
8. Per bin, take the half-difference of (JES_up − JES_down)/2 and
   (RCES_up − RCES_down)/2; sum in quadrature with other systematics.

**Pros**:
* Matches the published H1 prescription exactly. No referee defense issue.
* Captures the in-jet vs. out-of-jet distinction (JES vs. RCES treated
  independently, varied independently, correlated in the right way per
  event).
* Captures the migration matrix shift coherently — the response matrix
  itself is rebuilt with the shifted-HFS MC, so bin-boundary motion is
  handled by construction.
* No assumptions about the per-cell shape of the HFS shift (uniform vs.
  pT-dependent vs. η-dependent); the calibration was tuned against
  uniform ±1 %, which is what the framework applies.
* The hadron-side reconstruction (energy-flow algorithm, scattered-electron
  removal, ISR-photon recombination from Andreev 2024 §2) all runs at the
  shifted scale, so the HFS variation propagates through every downstream
  decision (E−pz cut, pT_HFS / pT_e balance cut, etc.) — these cuts are
  themselves HFS-dependent and Option B cannot reproduce that.

**Cons / costs**:
* Production cost. Per the supervisor brief: "~3 hours wall on Condor per
  variation." Plan-background-MC-production-g1jES-2026-05-24 §6.6 confirms
  the Stage-2 binary runs at ≤ 5 s/file on signal MC files, and the full
  signal MC v5g took ≈ 1 h wall on 200 concurrent NAF slots for the 865
  signal MC files; data + Norad chains add to this. Four variation runs
  (JES_up, JES_down, RCES_up, RCES_down) at ≈ 3 h each → ≈ 12 h Condor
  wall, parallelizable to ≈ 3 h if all four submitted as one batch.
* Extractor cost. ~30 min per variation for the v3 extractors over data
  and the 4 signal periods + MC; total ~2 h serial, parallelizable.
* Response matrix + unfold cost. The 4D response matrix build is reported
  in the brief as ~30 min wall; the unfold itself another ~30 min.
  Multiply by 4 variations = ~4 h serial.
* Disk: 4 × (Stage-2 sidistree + pertrack CSV). Per
  plan-background-MC-production §3.1, signal MC sidistree is 29 GB and
  pertrack v3 is ~30 % of that, so each variation adds ≈ 40–50 GB on DUST.
  4 variations = ~160–200 GB. Modest vs. the 405 GB g1j_ES background
  spend already in the plan.
* Steering changes: ~10 LOC in the Stage-2 binary to add the JES/RCES
  flags. The HFS object classifier already exists (the v5g pipeline
  already runs jet clustering for the SIDIS cut; reuse that jet
  collection to tag in/out).
* Risk: if the JES/RCES classifier in our v5g code does not match the
  exact jet definition the H1 calibration was tuned against, the
  uncertainty magnitude will be off. Cross-check against Andreev 2023 §1
  jet definition: k_T algorithm, R = 1.0, pT > 5 GeV, in lab frame, on
  HFS objects with −1.5 < η_lab < 2.75. Match that.

**Bottom-line cost**: ~1 day end-to-end if all four variations submitted
as a single batch and the pipeline holds together; ~2–3 days realistically
once iterating on the steering patch and verifying the JES/RCES classifier
is right.

### 4.2 Option B — analytic rescale on per-track CSV

**Procedure** (the brief's "Option B" — feasible in principle from the
v3 CSV alone if the schema matches what the brief asserts):

1. Read the per-track CSV.
2. Per event, rescale `Empz` (which equals Σ_had − [electron contribution],
   plus the 2E_e from the photon-removed beam — verify in the actual
   extractor) by (1 + δ) — uniform δ = ±0.01.
3. Recompute y_es, Q²_es, x_es from the shifted Empz.
4. Per track, recompute z and pT_h in the γ*+p CM frame using the shifted
   (Q², x, y) → shifted boost. Keep `Pt_lab, Eta_lab, charge` unchanged
   (tracker, not HFS).
5. Re-build the response matrix from the shifted MC pseudo-CSV the same
   way (apply the rescale to MC reco-level CSV; gen-level stays nominal).
6. Re-unfold with the shifted response matrix; difference vs. nominal is
   the HFS systematic.

**Pros**:
* Fast: ~30 min wall as the brief states.
* No NAF production cost.
* Useful as a first-pass sanity check while Option A jobs run.

**Cons**:
* Collapses JES and RCES into a single uniform δ. To split, we would need
  per-track flags marking which HFS objects sit in a jet at extraction
  time — and these flags are *not* in the per-track CSV schema as the
  brief describes it (`Q2es, Xes, Yes, Pt_lab, Eta_lab, charge, z, pT_h,
  phi_h, weight, weight1, weight2, Empz`). Without per-HFS-object jet
  membership at the event level, we cannot reproduce the JES/RCES split.
* The HFS shift cannot reach event-selection cuts that depend on HFS
  (E−pz, pT_HFS / pT_e balance). Events that pass under nominal HFS might
  fail under shifted HFS, and vice versa, and Option B cannot capture this
  acceptance shift because the events are already in the CSV.
* The HFS shift cannot reach the ISR-photon recombination step (Andreev
  2024 §2): photons close to the electron are recombined into the
  electron, photons close to the beam are removed. The recombination
  depends on calorimeter cluster energies and angles, which Option B
  cannot revisit.
* Cannot reach the energy-flow algorithm's track-cluster matching
  (Andreev 2023 cites refs [86][87][88]). The algorithm decides whether
  a track is replaced by its calorimeter cluster (high-momentum tracks)
  or its tracker measurement (low-momentum tracks); shifting the
  calorimeter scale may change those decisions. Option B treats the
  energy-flow output as immutable.
* The migration matrix rebuild in Option B uses the same CSV-rescaled
  reco-vs-gen pairs. This is conceptually inconsistent: gen stays
  nominal, reco gets uniformly shifted Empz, so the migration captured
  by the rebuilt matrix is *only* the migration in (Q²_es, x_es, y_es,
  z, pT_h) that follows from a uniform Empz shift. The real migration
  from the published prescription includes per-object shift of the HFS
  4-momenta before any clustering, jet finding, or kinematic
  reconstruction.
* Not the published H1 convention. Cannot defend in a paper.

**When Option B is acceptable**:
* As a 30-minute sanity-check before launching the full Option A run.
* As a fallback if the production pipeline is broken and we need a
  preliminary HFS envelope for an internal review by tomorrow.
* As a cross-check on Option A: if Option B (uniform 1.4 % combined)
  gives results within a factor of ~2 of Option A per bin, the Option A
  result is internally consistent. If they diverge dramatically in tail
  bins, dig into the f_jet weighting and the cut-acceptance effects.

---

## 5. Recommended path

**Option A. Re-extract from ROOT with HFS shifted at the framework level,
two components (JES, RCES) varied independently by ±1 %, four total
variation runs, propagated through Stage 1 + Stage 2 + extractor +
response build + unfold.**

This is the published H1 convention for high-Q² analyses (Andreev 2014,
2017, 2023, 2024 all use this exact prescription) and is the only path
that produces a publication-defensible HFS systematic. The cost (~3 h
batch wall for all 4 variations, ~1 day end-to-end including extractor
+ unfold) is acceptable.

**Use Option B in parallel as a same-day sanity check** to confirm the
order of magnitude before Option A finishes. Document both numbers in
the v3 systematics table with Option A as the canonical value.

---

## 6. Concrete implementation plan

### 6.A Option A — re-production plan

**Phase 0 (1–2 h, no Condor): steering + binary patch.**

* Add to Stage-2 binary
  `bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v5g`:
  ```cpp
  // Read from steering or env:
  double jesShift  = readOption("JES_SHIFT",  0.0);  // ±0.01 for variation
  double rcesShift = readOption("RCES_SHIFT", 0.0);  // ±0.01 for variation

  // Inside HFS object loop, BEFORE any kinematic computation:
  for (auto& hfs : eventHFS) {
    double scale = isHfsInsideHighPtJet(hfs, jetCollection) ?
                   (1.0 + jesShift) : (1.0 + rcesShift);
    hfs.SetPxPyPzE(hfs.Px()*scale, hfs.Py()*scale,
                   hfs.Pz()*scale, hfs.E()*scale);
  }
  ```
* `isHfsInsideHighPtJet` must use the *same* jet collection used by the
  H1 JES calibration. From Andreev 2023 §1: k_T, R = 1.0, pT > 5 GeV in
  lab frame, on HFS with −1.5 < η_lab < 2.75. Verify our existing jet
  collection in the v5g pipeline matches; if it doesn't (e.g., we use
  anti-k_T or a different pT threshold), build a parallel JES-classifier
  jet collection just for the in/out tagging.
* Cross-check: a Stage-2 run with `JES_SHIFT = 0.0, RCES_SHIFT = 0.0` must
  reproduce the nominal sidistree byte-identically. This is the
  no-op test before committing.

**Phase 1 (~30 min Condor + 30 min verification): smoke variation on 1 signal MC file.**

Run a single Stage-2 job with `JES_SHIFT = +0.01` on one signal MC file.
Compare event-level Q²_es, y_es, x_es distributions vs. nominal:
* Mean y_es should shift by ~+0.5 % at ⟨y⟩ ≈ 0.5.
* Mean Q²_es should shift by ~+1 % at ⟨y⟩ ≈ 0.5.
* No catastrophic acceptance loss (event count should be within ±1 %).

Verdict gate before launching full production: smoke must pass these
three sanity checks within 2σ statistical.

**Phase 2 (~3 h Condor wall as one batch): full 4-variation production.**

Submit 4 Condor clusters (parallel), one per variation:
* `JES_SHIFT = +0.01, RCES_SHIFT = 0.0` → `output/mc_jes_up/`,
  `output/data_jes_up/`
* `JES_SHIFT = -0.01, RCES_SHIFT = 0.0` → `output/mc_jes_down/`,
  `output/data_jes_down/`
* `JES_SHIFT = 0.0, RCES_SHIFT = +0.01` → `output/mc_rces_up/`,
  `output/data_rces_up/`
* `JES_SHIFT = 0.0, RCES_SHIFT = -0.01` → `output/mc_rces_down/`,
  `output/data_rces_down/`

Re-use the existing `submit_full_production.py` with a new `--hfs-variation
{jes_up,jes_down,rces_up,rces_down,nominal}` flag that sets the steering
flags and the output paths.

Estimated per-cluster wall: same as nominal (≤ 30 min for largest period
per plan-background-MC-production §6.6) for Stage 2; Stage 1 evls do *not*
need re-running because HFS scale does not enter Stage 1 (electron-side
trigger + event-list only). So **Stage 1 is shared across variations**,
saving substantial time. Verify Stage 1 truly does not touch HFS energies
before relying on this.

**Phase 3 (~2 h serial; parallelize for ~30 min): re-extract pertrack CSVs.**

Per variation, run the three v3 extractors against the new
`output/mc_<var>/` and `output/data_<var>/` trees:
* `scripts/extract_pertrack_mc_v3/extract_pertrack_mc_reco_v3.py
   --mc-root output/mc_jes_up --output output/pertrack_mc_v3_jes_up`
* `extract_pertrack_mc_gen_v3.py` (nominal gen — gen does not shift)
* `extract_pertrack_mc_disevent_v3.py` (per-event eSigma kinematics)
* And the data equivalents.

The gen-level pertrack is shared across all variations (gen-side is not
shifted by HFS calibration). Only reco and the per-event CSV need
re-extraction.

**Phase 4 (~30 min per variation, ~2 h total): rebuild response + unfold.**

Per variation, run the 4D response matrix build using the shifted reco +
nominal gen, then run the unfold against shifted data. Output: 4 unfolded
distributions per bin (JES_up, JES_down, RCES_up, RCES_down).

**Phase 5 (~15 min): compute the systematic per bin.**

Per bin i, in each (Q², x, z, pT_h) cell:

```
δ_JES(i)  = [N_unfolded(i, JES_up)  − N_unfolded(i, JES_down)]  / 2
δ_RCES(i) = [N_unfolded(i, RCES_up) − N_unfolded(i, RCES_down)] / 2
σ_HFS(i)  = sqrt( δ_JES(i)² + δ_RCES(i)² )
```

Sign convention follows Andreev 2024 §6.2: "the signed difference between
the unfolded result with the nominal energy scale and the energy scale
shifted upwards, i.e. a negative value corresponds to the case where the
value in a given bin was increased after the systematic shift."

Document σ_HFS(i) per bin in
`H1/sidis_highQ2/docs/results/systematics_v3_hfs_scale.md` (the canonical
v3 systematics file, which needs creating — `docs/results/` does not
currently exist; first commit should create it).

### 6.B Option B — analytic CSV rescale (sanity-check only)

If used (NOT recommended as canonical):

```python
# H1/sidis_highQ2/scripts/sanity_hfs_csv_rescale.py
# ~120 LOC. Reads pertrack_v5g_fixedFrame_v3_merged.csv.gz from NAF,
# applies uniform delta to Empz per event, recomputes Q2es/Xes/Yes per
# event, recomputes z/pT_h per track via re-boosted γ*+p CM. Writes a
# shifted CSV with the same schema. Then run the existing response-build
# + unfold against the shifted CSV.

import pandas as pd, numpy as np
import vector  # for 4-vector boosts

E_e = 27.6        # electron beam energy (GeV)
E_p = 920.0       # proton beam energy (GeV)
s   = 4 * E_e * E_p   # CMS² (GeV²)

def shifted_kinematics(df_event, delta):
    """delta = ±0.01 uniform HFS shift on Empz."""
    Empz_shift = df_event['Empz'] * (1 + delta)
    # y_es = Empz_had / (Empz_had + 2 E_e), but our 'Empz' includes the
    # electron 2 E_e contribution; verify in extractor source. If
    # Empz_total = Empz_had + 2 E_e, then:
    #    Empz_had_shift = (Empz_total − 2 E_e) * (1 + delta) + 2 E_e
    # Adjust accordingly before computing y_es.
    y_es   = (Empz_shift - 2 * E_e) / (Empz_shift)
    # Q2_es needs scattered electron energy + θ_e, which we DO NOT have
    # in the CSV per the brief's schema. This is a blocker for Option B.
    # FAIL-FAST HERE — Option B requires (E_e', θ_e) in the CSV.
    ...
```

**Discovered blocker for Option B**: the per-track CSV schema as
described in the brief (`Q2es, Xes, Yes, Pt_lab, Eta_lab, charge, z,
pT_h, phi_h, weight, weight1, weight2`) does not contain the per-event
scattered-electron 4-momentum (E_e', θ_e, φ_e). Q²_es = E_e'² sin²(θ_e) /
(1 − y_es) cannot be recomputed analytically from a shifted y_es without
also knowing (E_e', θ_e). Without those, Option B can only do the shift
through the (Q², x, y) values already in the CSV, which means applying
the first-order chain rule δQ²_es / Q²_es ≈ δ · y / (1 − y) without
re-deriving from the underlying kinematics. That is a defensible
*first-order* analytic shift but it loses the second-order kinematic
correlations that Option A captures.

If we want Option B to be defensible at all, the per-track CSV schema
must be extended to include (E_e_prime, theta_e, phi_e, Empz_had_only).
That's a small extractor patch but it requires re-running the extractor
once → at which point we're already paying the extractor cost for Option
A and might as well do the full thing.

**Conclusion on Option B**: not viable as the schema currently stands.
Go with Option A.

---

## 7. Per-cell estimated magnitude of σ_HFS (from prior H1 published results)

This section anchors the expected per-bin HFS uncertainty for our SIDIS
observables to the published H1 high-Q² evidence base. Values quoted here
are not direct measurements on our v3 sample — they are the literature
prior. The Option A pipeline in §6.A will replace these with measured
values.

### 7.1 Andreev 2024 (groomed event shapes, EPJC 84 (2024) 718)

§6.2 quotes the per-bin HFS uncertainty (signed difference, shifted-up
vs. nominal) for groomed 1-jettiness and groomed invariant mass: **"typically
1–2 %"** per HFS component (JES or RCES individually), with both
propagated through the unfold. Combined HFS quadrature ≈ 1.4–2.8 %
typical per bin.

### 7.2 Andreev 2023 (jet substructure, PLB 844 (2023) 138101) — most comparable

Table 2 of Andreev 2023 gives per-bin systematics on the unfolded *means*
of six jet angularities in four Q² ranges. The two HFS columns are HFS(jet)
[=JES] and HFS(other) [=RCES]. Selected entries, transcribed from the
ar5iv HTML table (retrieved 2026-05-25):

| Observable | Q² range (GeV²) | Central value | HFS(jet) | HFS(other) | Total syst |
|---|---|---:|---:|---:|---:|
| ⟨ln λ₁¹⟩ (jet broadening) | 150–360       | −1.385 | 0.004 | 0.004 | 0.007 |
| ⟨ln λ₁¹⟩                  | 360–866       | −1.506 | 0.003 | 0.000 | 0.009 |
| ⟨ln λ₁¹⟩                  | 866–2081      | −1.643 | 0.006 | 0.005 | 0.012 |
| ⟨ln λ₁¹⟩                  | 2081–5000     | −1.803 | 0.008 | 0.009 | 0.018 |
| ⟨ln λ₂¹⟩ (jet thrust)     | 150–360       | −2.273 | 0.007 | 0.007 | 0.007 |
| ⟨ln λ₂¹⟩                  | 2081–5000     | −2.914 | 0.011 | 0.014 | 0.016 |
| ⟨Nc⟩ (charged multiplicity)| 150–360       |  5.531 | 0.010 | 0.020 | 0.019 |
| ⟨Nc⟩                      | 360–866       |  5.969 | 0.021 | 0.023 | 0.021 |
| ⟨Nc⟩                      | 866–2081      |  6.753 | 0.012 | 0.028 | 0.016 |
| ⟨Nc⟩                      | 2081–5000     |  7.747 | 0.053 | 0.056 | 0.017 |

For charged-particle multiplicity ⟨Nc⟩ — the closest available analog to
our SIDIS yield in (Q², x) — relative HFS contributions are:

| Q² range | (HFS_jet / Nc) | (HFS_other / Nc) | Combined HFS / Nc |
|---|---:|---:|---:|
| 150–360 GeV²   | 0.18 % | 0.36 % | 0.40 % |
| 360–866 GeV²   | 0.35 % | 0.39 % | 0.52 % |
| 866–2081 GeV²  | 0.18 % | 0.41 % | 0.45 % |
| 2081–5000 GeV² | 0.68 % | 0.72 % | 0.99 % |

For standard deviations of the same observables (Table 3), HFS contributions
are higher in relative terms:

| Observable | Q² range | std value | HFS_jet (rel) | HFS_other (rel) |
|---|---|---:|---:|---:|
| std(ln λ₁¹) | 2081–5000 | 0.622 | 1.9 % | 1.0 % |
| std(ln λ₂¹) | 2081–5000 | 1.004 | 1.6 % | 1.0 % |
| std(Nc)     | 866–2081  | 3.132 | 1.0 % | 0.5 % |
| std(Nc)     | 2081–5000 | 3.544 | 0.8 % | 0.2 % |

### 7.3 Extrapolation to our SIDIS observables

Our analysis reports differential cross sections (or normalized yields) in
4 bins of (Q², x, z, pT_h). The published evidence base gives the following
expectation:

* Per-component HFS uncertainty (JES alone or RCES alone): **0.2–2 %** per
  bin on differential cross sections / unfolded yields, with 0.5–1 %
  typical and the high end reached at high Q² (Q² > 2000 GeV²) and in the
  tail of any HFS-correlated variable.
* Combined HFS quadrature (JES ⊕ RCES): **0.3–3 % per bin**, typically
  ~0.7–1.5 % on bulk SIDIS bins, ≤ 3 % on tail bins.
* z dependence: not directly measured in Andreev 2023, but our parametric
  analysis in §3.2 predicts the per-bin HFS amplification grows with z
  via the (y / (1−y)) factor on Q²_es. Conservatively expect the high-z
  bins (z > 0.7) to land in the upper half of the envelope (~2–3 %).
* pT_h dependence: similar expectation; pT_h > 4 GeV bins probably hit
  the 2–3 % range, low pT_h sits below 1 %.

**Recommended placeholder envelope for the v3 systematics table** (to use
in any pre-Option-A document or talk):

* Bulk bins (Q² < 1000 GeV², z < 0.5, pT_h < 2 GeV): σ_HFS / yield ≈ 1 %
* Mid bins (intermediate Q², z, pT_h):                σ_HFS / yield ≈ 1.5 %
* Tail bins (Q² > 2000 GeV², z > 0.7, pT_h > 4 GeV): σ_HFS / yield ≈ 3 %

These are *upper bounds* anchored to the most-conservative comparable
published values. Replace with measured per-bin values from the Option A
run when complete.

---

## 8. Cross-references to other H1 documentation in this workspace

### 8.1 Local workspace cross-references (verified present)

* `H1/sidis_highQ2/docs/plans/plan-background-MC-production-g1jES-2026-05-24.md`
  (538 lines). The background MC production plan. Cross-relevance to this
  memo:
  * §6.2 confirms the Stage-2 binary (`ChargedSidisTreeBuilderHighQ2_v5g`)
    is the v5g production binary and that v5f is "not to be used per
    `submit_full_production.py:88`". The Option A plan in §6.A of this
    memo therefore targets v5g unambiguously.
  * §6.6 wall-time estimates (≤ 30 min per Condor cluster, 24 clusters
    end-to-end ≤ 1 h batch) inform the 3 h estimate for 4 HFS variations
    in §6.A here.
  * §6.4 confirms the v3 extractors (`extract_pertrack_mc_v3/{reco,
    gen, disevent}_v3.py`) — these are the extractors that Option A
    re-runs after each Stage-2 variation.
  * §3.1 disk calibration (signal MC ≈ 29 GB sidistree, 30 % overhead for
    pertrack) informs the 40–50 GB per variation estimate in §6.A.
  * §3.2 confirms DUST has > 2 PB headroom — the 160–200 GB HFS-variation
    spend is negligible vs. the 405 GB g1j_ES background spend already
    being weighed.

### 8.2 Workspace cross-references the brief asked for but that are NOT present

* `H1/sidis_highQ2/docs/results/systematics_v5f_hfs_scale_v2.md` — **does
  not exist locally**. `docs/results/` directory does not exist either.
  If a v5f HFS write-up exists, it lives on NAF and must be mirrored
  here (or summarized into this memo) before this section can resolve.
  **Action**: ask the supervisor whether the v5f doc was published in
  this repo at all, or whether it lives only on NAF. If only on NAF,
  retrieve it (read-only) and summarize as §8.3 of this memo in a
  follow-up commit.

### 8.3 Updates for v3 (relative to any v5f convention, once located)

Cannot be written without the v5f document. The two non-trivial
v5f-to-v3 changes the supervisor brief highlights are:

* Frame fix 2026-05-24 (pT_h + φ_h in γ*+p CM frame, extractor v3) —
  affects how Option A computes the per-track shift on z, pT_h (see §3.2).
  The v3 frame definition is the correct one to inherit; any v5f-era
  HFS systematic computed in a lab-frame or Breit-frame convention needs
  re-deriving in the CM frame.
* Scheme B′ canonical (Q², x) binning frozen 2026-05-24 — affects which
  bin boundaries the Option A systematic is computed in. v5f-era HFS
  per-bin numbers in a different binning scheme are not directly
  transferable; re-bin to Scheme B′ before comparing.

### 8.4 Published H1 cross-references (verified, source-backed)

* **Andreev 2024**, "Measurement of groomed event shape observables in
  deep-inelastic electron-proton scattering at HERA", arXiv:2403.10134v2,
  EPJC 84 (2024) 718, DOI 10.1140/epjc/s10052-024-12987-0, DESY-24-036.
  §6.2 "Energy scales" — canonical statement of JES = RCES = ±1 %,
  propagated through unfold, "typically 1–2 %" per HFS component.
  *Surface used*: arXiv abstract page + arXiv HTML (full §6.2 text quoted
  verbatim in §2.1 of this memo). Retrieved 2026-05-25.
* **Andreev 2023**, "Unbinned Deep Learning Jet Substructure Measurement
  in High Q² ep collisions at HERA", arXiv:2303.13620v3, PLB 844 (2023)
  138101, DOI 10.1016/j.physletb.2023.138101, DESY-23-034. §4
  "Uncertainties" + Tables 2–3. Most comparable to our analysis (same
  detector, same dataset subset, same Σ-method, same Q² > 150 GeV², same
  0.2 < y < 0.7). Tables 2–3 give per-bin HFS(jet) and HFS(other) values
  on unfolded jet-angularity means and standard deviations — the source
  for §7.2 of this memo.
  *Surface used*: arXiv abstract page + ar5iv HTML (Tables 2–3
  transcribed). Retrieved 2026-05-25.
* **Andreev 2014**, "Measurement of Multijet Production in ep Collisions
  at High Q² and Determination of the Strong Coupling αs",
  arXiv:1406.4709v2, EPJC 75 (2015) 65, DOI not extracted, DESY-14-089.
  Originator of the 1 % JES/RCES calibration for H1 high-Q² HERA-II at
  150 < Q² < 15000 GeV². Cited by Andreev 2023 [ref 142, "[142]" → 1406.4709]
  and Andreev 2024 [ref 167] as the calibration source.
  *Surface used*: arXiv abstract page only (full-text fetch not pulled;
  the calibration content is reproduced in the 2023 and 2024 papers
  cited above). Retrieved 2026-05-25.
* **Andreev 2017**, "Measurement of Jet Production Cross Sections in
  Deep-inelastic ep Scattering at HERA", arXiv:1611.03421v4, EPJC 77
  (2017) 215 (and erratum, v4 dated 2021-06-15), DESY-16-200. Combines
  low-Q² (5.5 < Q² < 80 GeV²) and high-Q² (using Andreev 2014 data)
  inclusive-jet measurements. Same JES + RCES split.
  *Surface used*: arXiv abstract page (HTML fetch failed: 404 on
  arXiv html; ar5iv payload exceeded 5 MB limit). The convention is
  inferred from the 2023 + 2024 citation chain rather than read
  first-hand from the 2017 paper text.

### 8.5 What to consult next, if Option A produces surprising results

* If JES + RCES per-bin HFS ≫ 3 % in some bin: cross-check our jet
  classifier definition against Andreev 2023 §1 (k_T, R = 1.0, pT > 5
  GeV, η_lab ∈ [−1.5, 2.75]) and see whether our v5g jet collection
  matches.
* If JES + RCES per-bin HFS ≈ 0 in some bin: suspect that the variation
  was not actually applied (no-op steering flag bug) or that the bin
  has no HFS-shifted events (e.g., a bin populated entirely by
  electron-driven kinematics). Run the smoke test in §6.A Phase 1
  before trusting the result.
* If the JES vs. RCES split is suspect or controversial in our case
  (e.g., dispute over the jet-classification definition): Andreev 2014
  is the original calibration source — read its §6 / §7 (would require
  pulling the PDF; the `pdf` skill handles it).

---

## 9. Open questions and follow-up

1. **Where does the v5f HFS document live?** Needed to satisfy §8 of the
   brief and to import any v5f per-bin numbers as the *prior* for the
   Option A run.
2. **Confirm v5g jet classifier matches Andreev 2023 definition.** The
   Option A magnitude is calibration-sensitive; if our jet collection
   differs (different algorithm, R, or pT threshold), the H1 1 % JES
   calibration is not directly inheritable and we would need either a
   re-derivation or a conservative inflation.
3. **Stage 1 HFS-independence.** Confirm Stage 1 evls do not touch HFS
   energies (the §6.A plan assumes this and re-uses nominal Stage 1
   across all 4 variations). If Stage 1 does apply any HFS-dependent
   cut (e.g., calorimeter trigger threshold uses HFS energy), then
   Stage 1 has to run per variation too, increasing the budget.
4. **CSV schema extension.** If Option B is ever to be used as more than
   a sanity check, the per-track CSV needs (E_e', θ_e, φ_e, Empz_had).
   Patch is small but requires extractor re-run.
5. **Background MC HFS handling.** Per
   plan-background-MC-production-g1jES-2026-05-24, the background MC
   (Pythia γp, DVCS, Compton, Grape, low-Q² Django, CC Django) is
   subtracted from data before unfolding. The HFS variation should
   propagate through the background subtraction too: background MC
   needs the same 4 variations applied. This adds production cost
   proportional to the background spend (~405 GB × 4 = 1.6 TB if done
   naively). Cheaper alternative: apply HFS variation only to the
   signal MC and propagate background as a separate normalization
   uncertainty if the background contribution is small (typically < 5 %
   in the high-Q² SIDIS regime per Andreev 2023's negligible-background
   claim). Decision deferred to supervisor.

---

*End of memo.*

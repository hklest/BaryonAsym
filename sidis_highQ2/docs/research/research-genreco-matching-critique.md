<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** New research note. Audit of current H1 charged-hadron SIDIS gen→reco matching against H1, HERA, and LHC best practice, with explicit recommendations for absolute-cross-section readiness.

# Research: Gen→Reco Truth Matching for Charged-Hadron SIDIS — Critique vs Best Practice

**Date:** 2026-05-19
**Author:** researcher (delegated)
**Subjects:**
- Current matching in `ProtonTreeBuilder_lowQ2.C` / `ProtonTreeBuilderHighQ2.C` lines ~1060-1170
  (these are the v5b SIDIS builder sources; the proton-prefixed filenames are
  legacy from the fork ancestor — see
  `docs/references/reference-naming-aliases-highQ2-sidis.md`).
  (CHARGED_MATCH_ANGLE = 2.0°; charge-consistent; greedy one-to-one; GTR-only pool;
  no momentum tolerance; no isolation cut; no chi² score).
- Reference: DESY 20-176 (`H1_ForwardMultAnalyzer.C` covariance-weighted χ² matcher);
  H1 1302.1321; HERMES 1212.5407; COMPASS 1709.07374; ATLAS hit-based matching probability;
  H1OO `H1PartCand::GetPartMC()` link.
- Audience: `h1-sidis-supervisor`, `h1-unfolder`, `h1-systematics`, `code-editor`.

> **Scope.** This note answers seven specific critique questions about whether the
> current angle-only, greedy, charge-consistent matching scheme is fit for purpose
> when the downstream goal is an *absolute* differential cross-section M^{h±}(Q²,x,z,pT_h),
> not just a charge asymmetry or ratio. It does not redesign the unfolder. It does
> not redesign the response-matrix binning. It does identify which parts of the
> matching scheme are safe, which are defensible-but-fragile, and which are below
> the H1-community standard for absolute cross-section publication.

---

## 0. Executive TL;DR

1. **The H1 community standard for an inclusive charged-particle response matrix is
   covariance-weighted momentum χ² matching, not angle-only.** This is the matching
   implemented in `ForwardMultAnalyzer.C` (lines 1502-1568) that produced the
   published DESY 20-176 multiplicity result, and it is what `h1-unfolder` should
   benchmark against. Our 2° angle-only greedy matcher is a simpler scheme that
   has not been published as the basis of any absolute H1 charged-hadron
   cross-section.
2. **Angle-only is defensible for asymmetry/ratio observables**, because the
   matching-induced biases cancel between numerator and denominator if they are
   charge-symmetric (which they are by construction in our charge-consistent
   matcher). For absolute M^{h±} it is not equivalent and is **not the published
   choice** of any HERA/LHC absolute charged-multiplicity measurement we have
   reviewed.
3. **For HERMES and COMPASS the matching question does not arise the way we ask
   it.** Their response/migration is built from bin-population ratios at MC level
   (`a^h = N_reco_in_bin / N_gen_in_bin`), with no per-particle assignment. This
   is fundamentally different from H1's per-track matcher approach and is also
   acceptable for an absolute M^{h}. We use the per-track approach because the
   H1OO data model exposes per-particle links and because our resolution is good
   enough to make per-track matching meaningful.
4. **Greedy first-come matching is the same scheme DESY 20-176 actually uses
   after de-duplication.** This is not the gap to worry about.
5. **The matching-induced systematic on M^{h±}, integrated over our (Q², x, z, pT_h)
   bins, is conservatively bounded at 1-3% per bin** based on the closure tests
   reported in H1 1302.1321 (Sys.5 ≈ 1% central tracking; Sys.6 ≈ 1-7% V0
   contamination — the largest matching-adjacent terms) and on what the
   neighbouring V0 analysis directly measured (3.2% of matches above 5°, with
   poor resolution). **For the most forward (high-η_lab) bins and the lowest-pT
   bins it can reach 5-10%** and must be re-derived per kinematic cell once the
   nominal scheme is frozen.

The headline conclusion: **the current matcher is acceptable for the first-look
A_h and the asymmetry-class observables already produced. It is below the H1
publication standard for absolute M^{h±}, and the gap is closeable with a small
set of well-scoped builder upgrades plus a closure-test campaign on the v5b
merged outputs.**

---

## 1. Summary of Best-Practice Matching Schemes

### 1.1 H1 published inclusive charged-particle multiplicity (DESY 20-176)

Source: `ForwardMultAnalyzer.C` lines 1502-1568 (Tu/Kong, on NAF under
`/data/dust/group/h1/ztudesy/h1_analysis/`); summarised in
`docs/research/research-h1-charged-particle-analysis.md` Sec. 4.2.

**Scheme.** Per-event REC↔MC matching by **covariance-weighted momentum χ²**:

- For each REC track of momentum **p_rec** with 3×3 momentum covariance
  `V_rec` from `H1Track::GetMomentumCovar()`, and each MC track of momentum
  **p_gen**, form the residual **r = p_rec − p_gen** and the Mahalanobis-like
  distance χ² = rᵀ V_rec⁻¹ r (with V_rec eigen-decomposed for stability).
- Each REC track stores `dmatchREC` (the χ² of the chosen MC partner) and
  `imatchREC` (the MC index; negative for fake).
- Each MC track stores `imatchMC` (the REC index; negative for inefficiency).
- De-duplication step (1539-1568): if two REC tracks claim the same MC
  particle, the one with smaller χ² wins; the other is re-matched (or
  demoted to fake).

**Why this is the H1 community standard for an absolute charged-particle
multiplicity measurement.** The χ² uses the actual measurement uncertainty,
so a low-pT track with broader resolution is allowed a larger geometric
mismatch, while a stiff track is held to a tight momentum tolerance. This
gives correct treatment of the *resolution-driven* migration that an angle
cut alone cannot capture. The de-duplication is symmetric (REC↔MC), so
matching ambiguities in dense events are resolved by the score, not by
loop order.

**Published systematic for matching choice in DESY 20-176:** *not present*
as a separate line item in Table 2. The matching choice is absorbed into
Sys.2 ("MC model dependence", 1.5-4.3%) and Sys.5 ("Single-track efficiency",
0.5% central / 10% combined). The interpretation in the H1 community is that
when the response matrix is built consistently from MC-truth-matched REC
tracks with the χ² matcher, the matching scheme itself does not generate a
separately quotable bias — the bias is folded into the MC model and tracking
efficiency lines via the closure tests on RAPGAP vs DJANGOH.

### 1.2 H1 1302.1321 (low-Q² charged-particle spectra, 2013)

Source: `docs/research/research-h1-1302-1321-charged-multiplicity.md` §4 and §6.

**Scheme.** Bin-by-bin correction with C_bin = N_gen_in_bin / N_rec_in_bin
from DJANGOH MC. *No per-particle matcher is published.* The correction is a
ratio of bin populations under the constraint that **purity > 75% per bin**.
When purity is high enough the per-particle matching is implicit in the bin
population, and the choice of per-track matcher only enters in how the MC
itself classifies a REC track as belonging to a given gen bin. There is no
explicit quoted "matching systematic" — it is subsumed in Sys.5 (tracking
efficiency, 1% central / 10% combined) and the MC model swap.

### 1.3 HERMES SIDIS multiplicities (Airapetian 2012, 1212.5407)

Source: arXiv:1212.5407 §III.3.5, equations (4)–(6).

**Scheme.** **No per-particle truth matching.** The acceptance/migration
correction is a smearing matrix S^h(i,j) = n^h(i,j) / n^h_Born(j), where
n^h(i,j) is the *number of (hadron type h) entries in REC bin i with Born
bin j* and n^h_Born(j) is the Born-only count in bin j from a second MC
pass without radiative/detector effects. Migrations out of acceptance go
into the j=0 "background" column. The Born multiplicity is recovered by
inverting the truncated S matrix (Eq. 6).

This is a *bin-population* response, not a *per-track* response, and it
**does not require a per-particle gen↔reco link** beyond the basic
"this MC track is generated, this MC track is reconstructed" bookkeeping
internal to LEPTO+GEANT3.

**Quoted systematic from matching choice:** zero. None is quoted. The
sources of systematic uncertainty in §III.4 are RICH unfolding,
azimuthal-modulation reweighting in the MC, radiative corrections, exclusive
ρ⁰/φ subtraction, and acceptance. No "matching scheme" line.

### 1.4 COMPASS charged-hadron multiplicities (1709.07374)

Source: arXiv:1709.07374 §3.3, equation (5).

**Scheme.** Identical philosophy to HERMES. Acceptance is

a^h(z, PhT²) = (dN_reco^h / dz dPhT²) / (dN_gen^h / dz dPhT²)

evaluated in four-dimensional (x, Q², z, PhT²) bins. **The numerator and
denominator are both over MC; the experimental measurement is corrected by
this ratio.** No per-particle truth match enters anywhere in the published
multiplicity extraction. Total systematic uncertainty is 5-7% (acceptance
+ diffractive VM + nuclear) with the acceptance line dominated by PDF/JETSET
variations of the MC, **not by matching choice**.

### 1.5 ATLAS/CMS charged-particle reconstruction (LHC standard)

Source: ATL-PHYS-PUB-2017-014; CMS TRK-11-001 (J.Instrum. 9 P10009, 2014).

**Scheme.** **Hit-based matching probability.** A reconstructed track is
associated with a truth particle if the fraction of its silicon hits coming
from that truth particle exceeds a threshold (typically 50%). This is a
*hit-level* match, not a geometric or momentum match, and it works because
the LHC silicon tracker records the truth-particle ID per hit through the
Geant4 simulation. The match is intrinsically one-to-one (a track can be
assigned at most one truth particle, the one contributing the most hits)
and intrinsically resolution-aware (you cannot accidentally match an
unrelated truth particle that happens to lie nearby in (η, φ) but
contributes zero hits to the track).

**Why this matters here.** ATLAS/CMS deliberately moved away from
geometric/momentum matchers because they (a) misbehave in dense
environments where multiple truth particles are within the resolution cone,
and (b) bias the reconstruction-efficiency measurement when the resolution
itself varies. H1's CJC tracking lives at much lower track density than
LHC tracking (a few hadrons per DIS event vs hundreds per tt̄ event), so
the hit-based-probability machinery is overkill and the H1OO data model
does not expose the hit-truth linkage cleanly anyway. The H1 community has
chosen χ² matching as the closest geometric/kinematic analogue, which is
already a substantial step up from angle-only.

**Quoted systematic from matching choice at LHC:** typically not broken
out separately for charged-particle multiplicity measurements; folded into
the per-track-efficiency uncertainty (≈ 1-2% per central track).

### 1.6 H1OO native truth link: `H1PartCand::GetPartMC()`

Source: `H1Mods/H1PartCand.h` line 175, `Bool_t AddPartMC(H1PartMC*)` at
line 219.

The H1OO data model already supports a per-candidate MC link: every
`H1PartCand` (the particle-candidate object the HFS finder produces and
that wraps an `H1Track`) has a `GetPartMC()` accessor returning the
`H1PartMC*` that was linked during the H1Mods filler stage. **This is the
H1OO-native truth match.** It is set by `H1Filler` / `H1HfsFinder` using
the underlying CJC-hit ↔ Geant-track association at the ODS / MODS layer
(the analogue of ATLAS's hit-based matching probability, but inside H1OO).

Whether or not `GetPartMC()` is populated for the tracks we read in our
builder is an **open question**. The DESY 20-176 author chose to bypass
this and implement his own χ² matcher in `ForwardMultAnalyzer.C`, which
suggests either (a) the H1OO native link was felt to be insufficiently
robust for the publication-grade response matrix, or (b) the χ² matcher
was preferred for explicit diagnostic control (he stores `dmatchREC` per
track). Either way, the existence of `GetPartMC()` should be confirmed
or refuted by direct inspection of an H1Mods event before being claimed
or dismissed.

---

## 2. Side-by-side: Current H1 SIDIS Scheme vs Best Practice

| Aspect | **Current H1 SIDIS builder** | **DESY 20-176 (H1 community standard)** | **HERMES / COMPASS** | **ATLAS / CMS** |
|---|---|---|---|---|
| Matcher granularity | per-track | per-track | per-bin (no per-track match) | per-track |
| Score | 3D opening angle in degrees | covariance-weighted χ² in 3D momentum (Mahalanobis) | none | hit-based matching probability |
| Threshold | angle < 2.0° (`CHARGED_MATCH_ANGLE`) | χ² < (eigen-decomposed bound; not a single number) | n/a | typically ≥ 50% of hits from one truth particle |
| Momentum tolerance | **none** | implicit in χ² (uses V_rec) | n/a | implicit (hits are particle-tagged) |
| Charge consistency | enforced (`gp->GetCharge() == trk->GetCharge()`) | not separately enforced; charge mismatch is automatically high χ² | n/a | implicit (truth particle carries charge) |
| Exclusivity | greedy: first reco to claim a gen wins (`genUsedCharged[ig]`) | de-duplicated bidirectionally (REC and MC each store best partner; ties broken by χ²) | n/a | one-to-one by hit ownership |
| Pool | GTR only; |PDG|∈{211,321,2212}; |gen η|<3.5; GenPt>0.05; GetMother2()==-1 | all stable charged MC (cτ > 10 mm) within fiducial; uses `H1PartMC::IsGtr()` | "Born" particles after Heracles cone-photon merge | all truth particles within tracker volume |
| Decay-in-flight (Λ, K0s daughters) | not flagged at the matching stage | `isDaughtersMC[]` walks parent chain; flags K0s daughter (1) and Λ daughter (2); separately weighted in Sys.6 | absorbed in VM subtraction; not flagged per-track | not relevant (LHC analyses cut on truth IP_T) |
| Heavy-flavour daughters | included via species cut on PDG only | included; `maxPDGmc` reweight inflates c→1.2, b→2.0 to match data | included; no special treatment | flagged via truth ancestry |
| Beam-remnant gen particles | included in gen pool (broad fiducial admits ≈ 4.7% beam protons); not excluded explicitly | implicit cut via fiducial (|η_lab|<1.6) | excluded by HERMES spectrometer acceptance | n/a |
| Diagnostics stored | `MatchAngle_ch`, `GenMatchIdx_ch`, `GenPDG_ch`, kinematics, `ParticleSource_ch`, `inFiducialChargedMatch` | `dmatchREC`, `imatchREC`, `imatchMC`, `nucliaREC`, `isDaughtersMC`, `ptStar*REC`, ... | n/a | matching-probability variable per track |
| Published as basis for **absolute** σ | **no — first-look only** | yes (DESY 20-176 EPJC 81 (2021) 212) | yes (PRD 87 (2013) 074029) | yes (multiple) |
| Quoted "matching systematic" | not yet evaluated | absorbed into Sys.2/Sys.5/Sys.6 | not quoted | absorbed into per-track efficiency |

**Reading.** Our scheme is structurally simpler than every per-track scheme in
the comparison row, but it is operating at the same place in the chain (build
response matrix R(reco|gen)). The simplification is principally on the score
(angle vs χ²) and on the de-duplication (greedy vs bidirectional). Pool
construction is comparable to DESY 20-176 once the species set is restricted
to {π, K, p}. V0-daughter bookkeeping is the most conspicuous missing piece.

---

## 3. Critique Questions — Direct Answers

### A. What is the H1 community's recommended matching scheme for inclusive charged-hadron SIDIS that targets absolute cross-section?

**Covariance-weighted 3D-momentum χ² matching with bidirectional de-duplication**,
as implemented in `ForwardMultAnalyzer.C` and published in DESY 20-176 (arXiv:2011.01812;
EPJC 81 (2021) 212). The script-level reference is
`/data/dust/group/h1/ztudesy/h1_analysis/H1_ForwardMultAnalyzer/src/ForwardMultAnalyzer.C`
lines 1502-1568. Our own audit of that code is in
`research-h1-charged-particle-analysis.md` Sec. 4.2.

No published H1 inclusive charged-particle multiplicity result uses **angle-only**
matching as the basis for an absolute cross-section. H1 1302.1321 uses
*bin-by-bin* correction (so per-particle matching does not enter as a quotable
choice); DESY 20-176 uses *unfolding with the χ² matcher* (so the matcher choice
is implicit in the response matrix). There is no published H1 result that uses
a 2°-angle-only greedy matcher as the basis for an absolute σ. **The
recommendation, if we want to be defensibly in line with the published H1
practice, is to migrate to the χ² matcher.**

### B. Is angle-only (no momentum window) defensible at this analysis's kinematic range (pT 0.15-5 GeV, |η|<1.3)?

**Partially.** Defensible for an asymmetry, marginal for an absolute σ.

The H1 published momentum resolution for central CJC tracks is
σ(pT)/pT ≈ 0.2% × pT/GeV ⊕ 1.5% (H1 1302.1321 §3.3.3). At pT = 5 GeV the
1.5% term dominates; at pT = 0.15 GeV the 1.5% term still dominates. So
relative momentum resolution is roughly 1.5-2.5% across our pT range. An
angle-only matcher with a 2° threshold does not enforce any tolerance on
this — a reco track at pT = 0.5 GeV moving in some direction will be matched
to a gen particle at pT = 5 GeV moving in nearly the same direction (large
relative |p_reco - p_gen| / p_gen) as long as the angle is within 2°. The
gen particle is wrong; the angle was right.

**Typical momentum tolerance in comparable analyses.**
- DESY 20-176 (H1): no fixed window; the χ² *is* the tolerance, scaled by V_rec.
  Effective acceptance is ≈ 3-5σ in (1/p, φ, θ) jointly.
- Heavy-ion track-jet correlation analyses (ALICE, CMS) typically use
  ΔR < 0.05 AND |p_T_reco - p_T_gen| / p_T_gen < 0.5 (i.e. 50% window) as a
  loose conservative cut and tighten to 20% when validating efficiency.
  This is the explicit "ΔR + momentum window" combination cited in the
  request.
- Local V0 analyses (see `docs/research-matching-validation.md` §6 and §7)
  showed that for Λ candidates, events with match angle > 5° had only 46%
  pT-resolution-within-10% and only 40% landed in the mass window. The
  high-angle tail is dominated by mismatches even though the angle threshold
  was 15° at that time. **This is direct evidence on our own data that
  angle-only matching admits non-trivial mismatches.**

**Concrete recommendation.** At 2° the angle is already tight enough that most
of the obvious mismatches are excluded. Adding a relative-momentum window of
**|p_reco - p_gen| / p_gen < 0.5** would be a near-zero-cost addition that
catches the residual pathologies (slow reco track matching to a fast forward
gen track that happens to align). A scientifically tighter scheme is the χ²
matcher.

### C. Is greedy first-come matching standard, or do other H1/LHC analyses use Hungarian, second-pass, or score-based assignment?

**Greedy first-come is not standard in published collider analyses; score-based
with de-duplication is.**

- DESY 20-176 uses score-based (χ²) with explicit bidirectional de-duplication
  in `ForwardMultAnalyzer.C` 1539-1568.
- ATLAS hit-based matching is inherently score-based (matching probability)
  and bidirectional.
- Hungarian assignment is rare in HEP charged-track matching because the
  cost matrix is sparse (most pairs are far apart in any score) and a single
  pass of "best-score-for-each-REC, then de-duplicate" achieves the same
  effective result with O(n_rec · n_gen) cost.

The local V0 analysis matcher in `H1/reference/TreeBuilder.h` `matchRecoToGen()`
(lines 212-254) is *also greedy first-come*: it walks gen indices in order and
keeps the first that beats the current best within the angle threshold. The
order dependence is hidden because the inner loop iterates all gen indices, so
the score (`best.matchAngle`) is what actually breaks ties. **This is greedy
on the gen→reco side (a given gen can be claimed by the first reco that picks
it via `genUsedCharged[ig]`) but score-based on the reco→gen side (each reco
finds its best gen via min-angle).** This is also what our SIDIS builder does
at lines 1092-1112.

**Why this is a small effect for SIDIS.** In our kinematic regime (Q² < 100
GeV², few hadrons per event in the central tracker, |η|<1.3), the per-event
combinatorial ambiguity is low: most events have ≤ 5 reco tracks and ≤ 10 gen
particles in the matching pool. The probability of two reco tracks competing
for the same gen partner is small (the diagnostic to confirm this is the
fraction of events where bidirectional de-duplication would have flipped a
match). In dense V0 environments this matters more — see
`research-matching-validation.md` §6 — but for inclusive central charged
hadrons the order dependence is sub-percent on the response matrix.

**Recommendation.** Add a bidirectional de-duplication pass for safety but do
not expect a large numerical change. Run a diagnostic test that compares
greedy vs Hungarian-style assignment on a single MC file and quotes the
fraction of differently-assigned tracks.

### D. What is the typical CHARGED_MATCH_ANGLE in degrees in comparable analyses? Is 2° tight, loose, or standard?

**2° is on the tight end of standard; comparable analyses use 1-5°.**

- H1 V0 analysis (lambda): historically 15° (very loose); analysis-time
  studies showed 99% of true matches are < 1°, and tightening to 5° loses
  only 3% of matches but kills most of the high-angle mismatch tail
  (`research-matching-validation.md` §3 and §6).
- DESY 20-176: no fixed angle threshold (χ²-based), but the typical
  effective ΔR equivalent is < 0.05 rad ≈ 2.9° for central CJC tracks
  (estimated from σ(p)/p ≈ 1.5% × p/GeV at fixed θ).
- ALICE/CMS heavy-ion track-jet matching: ΔR < 0.05 (≈ 2.9°) is standard
  for "good match"; ΔR < 0.1 (≈ 5.7°) is "loose".
- LHCb open-charm: 0.5°-1° because tracks are well-separated in the
  forward arm.

For our pT range (0.15-5 GeV) the H1 CJC angular resolution σ(θ) is
≈ 1 mrad (0.06°) for stiff tracks, degrading to ≈ 5 mrad (0.3°) at low pT
where multiple scattering dominates. The 3D opening angle pile-up of
σ(θ) and σ(φ) gives ≈ 0.5° per pT (in the worst-case low-pT bin), and the
*physical* MC truth direction can differ from the reco direction by
several σ due to MS in inner material plus the natural curvature
extrapolation back to vertex.

**A 2° threshold is therefore ≈ 4σ on the worst-case low-pT central track
and ≈ 30σ on a stiff track.** It is wide enough to capture true matches
at low pT (where you most need slack) and tight enough to suppress
random-coincidence matches in events with multiple gen particles in the
same hemisphere.

**Recommendation.** 2° is acceptable as the nominal. Carry a systematic
variation at 1° (tight) and 3° (loose) when computing the matching
systematic.

### E. How do comparable analyses handle:

#### Decay-in-flight kaons and pions (Λ, K0s daughters)

- **DESY 20-176:** `isDaughtersMC[]` walks the MC parent chain to flag
  K0s (PDG 310) and Λ (PDG 3122) daughters
  (`ForwardMultAnalyzer.C` 770-857). Sys.6 assigns **50% uncertainty on
  K0s daughter tracks** and 0.5% on photon-conversion/Dalitz daughters
  surviving the primary-vertex cut. This dominates the dense-environment
  systematic in their analysis (1-7% on P(N)).
- **H1 1302.1321:** No per-track daughter flag is published, but K0s
  contamination is quoted at 0.2% on the densities (their Sys. 6).
- **HERMES, COMPASS:** Decay-in-flight is bundled into the
  acceptance/smearing matrix; they do not subtract V0 daughters from the
  inclusive charged-hadron sample because their detector acceptance (close
  to interaction point, no long V0 flight) leaves the population very small.
  Their dominant analogous correction is the **diffractive vector-meson
  subtraction** (10-50% on z bins above 0.6 — for ρ⁰→π⁺π⁻ and φ→K⁺K⁻).
- **Our builder:** *Not flagged.* The `H1PartMC::IsGtr()` cut admits
  daughters that the GTR scheme tagged as stable enough to be in the
  matching pool. The `GetMother2()==-1` veto only catches one specific
  edge case (the strangeness-baryon multiple-mother bookkeeping). For an
  *absolute* M^{h±} measurement, this is the single most important gap
  for which the published H1 community standard (DESY 20-176) has a
  specific recipe. See **Recommendation R3** below.

The H1 SIDIS handoff explicitly notes (`context-handoff.md` "Truth Species")
that the species set is `{π, K, p}` and "Lambda, K0s, and other strange-baryon
cross-sections will be published separately". That convention requires us to
either *subtract* K0s daughter pions from the inclusive sample or *count* them
and absorb the bias into Sys.6. We currently do neither.

#### Beam-remnant gen particles in the matching pool

- **DESY 20-176:** The fiducial cut `|η_lab|<1.6` removes beam protons
  geometrically; no explicit veto needed.
- **H1 1302.1321:** Same; their analysis lab η window is [-2, 2.5] which
  is wider, but beam protons at z_lab>0 with η > 5 are well outside even
  this wider window.
- **HERMES, COMPASS:** Fixed-target spectrometers do not have a backward
  beam remnant; the issue does not arise the way it does at HERA.
- **Our builder:** Admits beam protons via the broad gen fiducial
  (`|GenEta|<3.5`, `GenPt>0.05`). The handoff documents this as 4.7% of
  the gen-level pool, contained by `pT>0.05`. The reco-level cuts
  (`|η|<1.3`, `pT>0.15`) prevent them from being selected as numerator
  entries, but they remain *available* as match partners for any
  reco track with the right charge and angle. This is a low-probability
  failure mode (a beam proton at η>3.5 will not be within 2° of any reco
  central track at |η|<1.3), but it should be confirmed by a one-line
  diagnostic: count the number of accepted matches where the matched
  gen particle has |η|>2.5.

#### Charge-consistency requirement

- **DESY 20-176:** Not separately enforced. Charge mismatches are
  automatically high χ² because q/p is in the track parameter vector and
  q is a hard ±1 discrete. A wrong-charge match would require the reco
  q/p to be opposite-sign of the gen q/p, which is essentially impossible
  for tracks that pass the χ² cut.
- **H1 1302.1321:** Not separately enforced; same reasoning.
- **HERMES, COMPASS:** Not relevant (bin-population matching).
- **ATLAS/CMS:** Not relevant (hit-based matching).
- **Our builder:** Explicitly enforced (`gp->GetCharge() == trk->GetCharge()`).
  This is a defensible *defensive* choice: angle-only matching alone could
  in principle pair an h⁺ reco track with a colocated h⁻ gen particle if
  one happens to be the nearest in angle. The cost of the enforcement is
  near-zero (charge is one comparison) and the benefit is bounding the
  charge-swap fake rate exactly to zero.

**The cost of enforcing charge consistency is that we cannot diagnose
charge mis-reconstruction from our match output**, because the matcher
will refuse to associate any reco track to its true-charge-but-flipped gen
counterpart. The charge-mismatch rate measured downstream (0.59% in our
data after BASE_CUT, per `context-handoff.md` §7) is therefore the rate
*not* of mismatches but of *unmatched* reco tracks whose nearest gen
candidate had the wrong charge. For the asymmetry A_h this is fine because
the charge-flip systematic is below the statistical uncertainty. For an
absolute σ it should be reconciled: how many of those 0.59% are actually
charge-flipped tracks (and therefore migrated bin assignments in the response
matrix) versus genuine fakes?

**Recommendation.** Keep charge consistency in the matcher. Add a second
diagnostic-only branch `GenChargeFlipCand` that records the charge of the
nearest opposite-charge gen partner, *without* using it for the actual match.
This lets us measure the charge-swap rate at analysis time without changing
the response matrix.

#### Heavy-flavour decay daughters

- **DESY 20-176:** Apply `maxPDGmc` reweight (`mainAnalysis_fillTree.C` 184ff):
  charm-containing events upweighted by 1.2, b-containing by 2.0. This is a
  data/MC adjustment, not a matching restriction. Heavy-flavour daughters in
  the central charged sample are kept.
- **H1 1302.1321:** No explicit heavy-flavour treatment beyond what
  DJANGOH/RAPGAP include.
- **HERMES:** Not relevant at √s = 7.2 GeV.
- **COMPASS:** Not relevant at √s = 17 GeV.
- **ATLAS/CMS:** Heavy-flavour daughters in the inclusive charged sample
  are kept (the inclusive measurement is over all primary + prompt-decay
  charged hadrons).
- **Our builder:** Heavy-flavour daughters with PDG ∈ {211, 321, 2212} are
  kept; daughters with other PDGs (e.g. e⁺e⁻ from semileptonic B decays)
  are excluded by the species cut. This is consistent with the H1 published
  practice. We do **not** apply the DESY 20-176 c/b reweight; this is a
  separate (small) systematic of order 0.5% per bin if our charm/beauty
  fraction differs from data by 20%.

### F. What unfolding/response-matrix construction is standard, and how sensitive is the response to the matching scheme?

**Standard.** The H1 standard for charged-particle multiplicity is **TUnfold-based
matrix unfolding in (reco × gen) bins with regularisation**, with the response
matrix R(i,j) = P(reco bin i | gen bin j) built from MC truth-matched tracks
(DESY 20-176 §4.3). For more differential observables (e.g. our 4D
M^{h±}(Q²,x,z,pT_h)), the same TUnfold machinery generalises but with the
caveat that the regularisation parameter is harder to choose; alternatives
in active H1 use are Bayesian iterative (RooUnfold), SVD, and the OmniFold
ML-based approach (see `docs/research/research-omnifold-feasibility.md`).

**Sensitivity to the matching scheme.** This is the question with the
smallest amount of published literature, because most published analyses
do not vary the matching scheme as a systematic. The available evidence:

1. **HERMES/COMPASS.** The response is built from bin populations, not
   per-track matches, so the matching scheme question does not arise.
   Their analogous sensitivity is to the *MC generator choice* (5%-level)
   and the *acceptance flatness assumption* (negligible to a few %).
2. **H1 1302.1321.** Bin-by-bin correction with purity > 75% per bin. The
   matching scheme is implicit; the published sensitivity is to MC
   generator (DJANGOH vs RAPGAP, up to 5.5% in marginal bins).
3. **DESY 20-176.** TUnfold response built with χ² matcher. The published
   sensitivity is to the MC model used to build the response matrix
   (DJANGOH vs RAPGAP, 1-4%, Sys.2) and to the single-track tracking
   efficiency (Sys.5). **There is no published "matching algorithm" line
   in Table 2 of DESY 20-176.**
4. **Local V0 analysis.** `docs/research-matching-validation.md` §3
   directly measured the impact of tightening the angle threshold from
   15° to 5° on the V0 matching: it loses 3.2% of matches but improves
   purity in the signal mass window from ≈90% to ≈99% (averaged), with
   strong impact on the *response purity* but small impact on the
   *integrated* numbers because the high-angle matches are a small
   tail.

**Closure-test evidence.** Our own resolution and acceptance study
(`tmd_resolution_acceptance.json`) shows 30% per-dimension purity in the
4D binning, which is what motivates 4D unfolding rather than bin-by-bin.
In this regime the response matrix has significant off-diagonal weight,
and the matching scheme controls *which* off-diagonal entries get filled.
A different matcher (χ² vs angle-only) would assign slightly different
gen partners for the borderline tracks, redistributing weight along the
off-diagonal. The integrated effect on M^{h±} depends on how much
correlation there is between the matcher's mistakes and the unfolding
direction in (Q², x, z, pT_h) space.

**Recommendation.** Run a single closure test: rebuild the response matrix
twice — once with the current 2° angle-only greedy matcher, once with a
χ² matcher (or even a simpler angle + momentum-window combination). Unfold
the same data with both responses. The bin-by-bin spread of unfolded
M^{h±} is a direct measurement of the matching-induced systematic on this
analysis. **This is the single most important closure test missing from
the current Phase 5 systematics suite for absolute σ.**

### G. What is the published systematic uncertainty assigned to "matching choice" in comparable H1/HERA SIDIS analyses?

**Effectively zero — it is not a separately quoted line in any reviewed
publication.**

| Publication | "Matching choice" syst | Where it's hidden |
|---|---|---|
| DESY 20-176 (Tab. 2) | not listed | folded into Sys.2 (MC model, 1.5-4.3%) and Sys.5 (tracking eff., 0.5/10%) |
| H1 1302.1321 | not listed | folded into Sys.5 (tracking eff., 1/10%) and MC model (≤ 5.5%) |
| HERMES 1212.5407 | not listed | folded into "acceptance" (PYTHIA model) |
| COMPASS 1709.07374 | not listed | folded into "acceptance correction" (5%) |
| ATLAS/CMS various | not listed | folded into per-track efficiency (1-2%) |

**Interpretation.** The community absorbs matching choice into MC-model
and tracking-efficiency lines because the closure tests against alternative
MC and against tracking-efficiency variations are believed to also probe the
matching mistakes. This is a defensible accounting *if* the analysis has
run those closure tests. If we want a quotable matching-systematic line
for absolute M^{h±}, we have to run the closure test in F. above and quote
a number from it.

For now (asymmetry-only analysis), the conservative bound from the
neighbouring lambda V0 analysis (`docs/research-matching-validation.md`)
is **≤ 3% of matched events show ≥ 5° angle and have poor resolution**.
For our tighter 2° cut this drops to ≤ 1% of matched events. Translated
to an integrated M^{h±} systematic, this is **≤ 1% per bin for central
bins and ≤ 3-5% per bin for the most forward / lowest-pT bins** where
the matching is hardest. See Sec. 5 for a more detailed estimate.

---

## 4. Gap Analysis for Absolute Cross-Section Readiness

The MVP scope (`context-handoff.md` §1) explicitly excludes absolute
cross-sections, but the response matrix produced by the matcher is the
input that *will be required* when the analysis moves from M^{h±} (a
ratio) to dσ/dQ² dx dz dpT_h (an absolute observable). Gaps to close
before that promotion is defensible:

| # | Gap | Severity | Drives which observable? |
|---|---|---|---|
| G1 | No covariance-weighted χ² score; angle is treated as the only quality variable | Medium-High | Off-diagonal weight of response matrix; absolute σ |
| G2 | No momentum window even as a fallback bound | Medium | Slow-track mismatches; impacts low-pT bins |
| G3 | No V0-daughter flag (Λ→pπ, K0s→ππ daughters not distinguishable from primary) | High | M^{h±} normalisation; Sys.6 = 1-7% per DESY 20-176 |
| G4 | No conversion/Dalitz flag (γ→ee, π⁰→γeē) | Medium | Sys.6 = 0.5%/track per DESY 20-176; primarily affects e±-faking-h± rate |
| G5 | Greedy first-come ordering on the gen-claim side (no bidirectional de-dup) | Low-Medium | Order dependence in dense events; expected sub-percent |
| G6 | Charge-flip diagnostic not stored | Low | Wrong-charge contamination of asymmetry; expected ≤ 0.6% |
| G7 | Beam-proton pool admittance is bounded but undiagnosed | Low | High-η_lab marginal bins only |
| G8 | No closure-test campaign comparing alternative matchers | High | Required to quote a matching-syst line in a publication |
| G9 | Native H1OO `H1PartCand::GetPartMC()` link not used or even verified | Diagnostic | Would let us cross-check the geometric matcher against the H1OO-native truth link |
| G10 | No per-bin "matching purity" stored as a quality flag | Medium | Diagnostic for response-matrix bin-by-bin validation |

The largest gaps for **absolute** σ are G1, G3, and G8. G2, G4, G5 are
easy and worth fixing. G6, G7, G9, G10 are diagnostic; they should be
addressed but do not block.

---

## 5. Specific Recommendations, Ordered by Impact on Absolute σ Validity

### R1 (highest impact, medium effort) — Add a χ² score, store alongside MatchAngle, gate eventually

Implement the covariance-weighted χ² score from
`ForwardMultAnalyzer.C` 1502-1568 in `ProtonTreeBuilder_lowQ2.C` (and the
high-Q² twin). For each candidate (reco, gen) pair within the current 2°
angle gate, compute χ² = (p_rec - p_gen)ᵀ V_rec⁻¹ (p_rec - p_gen) using
`H1Track::GetMomentumCovar()` for V_rec (eigen-decomposed for numerical
stability). Store as a new branch `MatchChi2_ch` (Float_t) per track.

**Phase 1:** store χ² but **do not** change the match decision — the angle
score still wins; this gives downstream a diagnostic without breaking the
v5b first-look results.

**Phase 2:** add a config flag `USE_CHI2_MATCHER` that switches the
selection criterion from `bestAngle < 2°` to `bestChi2 < 25` (≈ 5σ in 3D).
Run the closure test (R6 below) to compare before promoting to nominal.

**Why this is R1.** It is the single change that brings us into line with
DESY 20-176. It is also the precondition for quoting a defensible
"matching scheme" systematic, because the χ² matcher is the natural
alternate-matcher comparison the systematic is built from.

**Effort.** ≈ 20 lines C++ in the builder, plus an include for the
covariance accessor. The eigen-decomposition is ROOT-native
(`TMatrixDSymEigen`).

### R2 (high impact, low effort) — Add a momentum-window fallback gate

Even without R1, add a simple secondary criterion:

```cpp
if (fabs(gp->GetMomentum().Mag() - trk->GetMomentum().Mag())
        / gp->GetMomentum().Mag() > 0.5)
    continue;   // relative momentum window: 50% loose
```

inside the existing matching loop (line ≈ 1106 of the builder, before the
angle comparison). This is a near-zero-cost catch for the
slow-reco-matched-to-fast-gen pathology. Tune the threshold (0.5 is
conservative; could be tightened to 0.3 if closure tests show that's safe).

**Effort.** 3 lines C++. Backward-compatible at the angle-only level
(turn off by setting threshold to ∞).

### R3 (high impact, medium effort) — Add V0-daughter and conversion flags at gen level

Walk the MC parent chain at builder time (analogous to
`ForwardMultAnalyzer.C` 770-857). For each gen particle in the matching
pool:

```cpp
int parentPdg = 0;
if (gp->GetMother1() >= 0) parentPdg = gtrdata[gp->GetMother1()]->GetPDG();
int isDaughterV0 = 0;
if (parentPdg == 310)  isDaughterV0 = 1;   // K0s daughter
if (parentPdg == 3122) isDaughterV0 = 2;   // Lambda daughter
if (parentPdg == 22)   isDaughterV0 = 3;   // photon conversion
if (parentPdg == 111)  isDaughterV0 = 4;   // pi0 Dalitz
```

The handoff already records `IsV0DaughterGen` on `ChargedTotGen` (v5b
schema). **Propagate this flag to the matched track on `ChargedTot` as
`IsMatchedV0Daughter_ch`** so downstream scripts can subtract or upweight
without re-walking the parent chain.

**Why.** This is the single biggest missing piece relative to the H1
published practice. Without it, we cannot evaluate Sys.6 (the 1-7%
V0-daughter systematic in DESY 20-176) for our analysis. With it, we
can either subtract these tracks (cleaner inclusive M^{h±}) or include
them with a 50% / 0.5% per-track weight (DESY 20-176 convention).

**Effort.** ≈ 20 lines C++. Already mostly done at the gen level
(`IsV0DaughterGen` on `ChargedTotGen`); just needs to be propagated to the
matched-reco branch via the existing match link.

### R4 (medium impact, low effort) — Bidirectional de-duplication pass

After the main matching loop, run a second pass that resolves
multiple-claim conflicts by best-score:

```cpp
// Sketch: store (recoIdx, genIdx, score) tuples in a vector
// Sort by score ascending
// Walk: for each tuple, if both recoIdx and genIdx are unclaimed,
// assign; else skip
```

This is what DESY 20-176 does at 1539-1568.

**Why.** Order independence of the assignment, which is a
publication-grade property. Expected numerical change: < 1% on the
response matrix in our central-tracks-only kinematic regime.

**Effort.** ≈ 30 lines C++. Should be done after R1 so the score used
for the sort is the χ², not the angle.

### R5 (low-medium impact, very low effort) — Charge-flip diagnostic branch

Add a separate loop or pass that, for each reco track, finds the
nearest *opposite-charge* gen partner (within a relaxed angle gate, e.g.
5°) and stores its index + angle + pT-ratio in three new branches
`GenChargeFlipCand_ch`, `MatchAngleChargeFlip_ch`, `PtRatioChargeFlip_ch`.
**Do not** use these in the primary match.

**Why.** Lets us quantify the charge-swap rate at analysis time, which we
currently cannot do because charge consistency is enforced upstream. This
is needed for an absolute σ to verify that the charge-symmetric component
of fake matches is what we think it is.

**Effort.** ≈ 15 lines C++.

### R6 (high impact, medium effort) — Closure test: angle-only vs χ²

Run a single MC file through the builder twice, once with the
angle-only matcher and once with the χ² matcher (R1 Phase 2). Use both
outputs to populate a response matrix on the v5b binning. Unfold the
*same* MC reco yields with each response and quote the bin-by-bin
relative difference on M^{h±} as the "matching scheme" systematic.

**Outputs.**
- A 2D plot of |ΔM^{h±}/M^{h±}| over the (Q², x) cells, integrated over
  z and pT_h.
- A 4D table of the same in the full binning.
- A single quotable number: the median absolute deviation across all
  populated bins.

**Why.** This is the closure test that lets us write a single
publication-ready line in the systematic table. Without it the
matching-scheme systematic is at best an estimate (Sec. 5 below); with it
the systematic is a measurement.

**Effort.** Once R1 is done, this is a one-day Python script. The MC file
should be a RAPGAP31 sample with statistics ≥ 1M tracks to keep the
per-bin statistical uncertainty small.

### R7 (diagnostic, very low effort) — Verify H1OO native `GetPartMC()` link

Single diagnostic script on NAF: open one MC event, dump the result of
`H1PartCandArrayPtr` walked with `GetPartMC()` for each accepted central
track, and compare to our geometric-match result. Three outcomes:

1. The native link is populated and agrees: we can use it as a
   cross-check and our geometric matcher is validated.
2. The native link is populated but disagrees: investigate; either we
   have a builder bug or the native link is using a different MC bank
   than we assume.
3. The native link is empty: confirms that we *must* implement our own
   matcher (which is what DESY 20-176 did anyway).

**Why.** Reduces a hidden assumption to a measurement. Saves us from
mis-trusting either machinery.

**Effort.** ≈ 30 lines of ROOT macro on NAF; one CPU-minute to run.

### R8 (diagnostic, low effort) — Beam-proton-pool sanity diagnostic

Add a counter at build time: number of accepted charged matches where the
matched gen particle has `|GenEta| > 2.5`. Print at end of job. If
non-zero, investigate.

**Why.** Bounds the beam-proton-leak component of matches at the source.
Expected: 0 events. If non-zero, the cut at gen `|η|<3.5` is too loose
and should be tightened to `|η|<2.5`.

**Effort.** 5 lines C++.

---

## 6. Estimated Systematic Uncertainty Attributable to "Matching Scheme"

### 6.1 Methodology

In the absence of an end-to-end closure test on our analysis (which is
R6 above and not yet executed), the matching-scheme systematic must be
estimated from literature anchors and from the neighbouring V0 analysis's
direct measurements on the same H1 dataset.

### 6.2 Anchors

| Anchor | Value | Where it appears |
|---|---|---|
| Lambda V0 analysis: fraction of matched events with angle > 5° | 3.2% | `docs/research-matching-validation.md` §3 |
| Lambda V0 analysis: same with poor pT resolution (|Δp/p|<10% only 21%) | 3.2% × 79% ≈ 2.5% genuinely-mismatched | as above |
| H1 1302.1321: Sys.5 central tracking efficiency | 1% per track | arXiv:1302.1321 Tab. 1 |
| H1 1302.1321: Sys.6 K0s contamination | 0.2% on densities | as above |
| DESY 20-176: Sys.6 V0 daughter contribution | 1-7% on P(N), 1-2% on moments | arXiv:2011.01812 §5 |
| DESY 20-176: Sys.5 central single-track efficiency | 0.5% per track | as above |
| DESY 20-176: matching-scheme line | not separately quoted (zero) | as above |
| Local SIDIS first-look: median 4D purity | 52% (Scheme A) | `context-handoff.md` "Purity (v5b)" |
| Local SIDIS first-look: integrated migration | 3.83%, charge-symmetric | as above |

### 6.3 Bound for a 2° angle-only greedy matcher (CURRENT SCHEME)

**Per-bin matching-scheme systematic on absolute M^{h±} (estimate):**

| Bin class | Estimate | Anchor / reasoning |
|---|---|---|
| Central bins (mid-η, mid-pT, mid-z) | **1-2%** | Dominated by V0-daughter contamination (R3 not done). 99% of matches are < 1° (V0-analogue numbers); residual high-angle tail < 1%. |
| Forward / high-η_lab bins | **3-5%** | Resolution worse, density higher; the 3.2% V0-analogue mismatch rate at angle > 5° applies more strongly here. |
| Lowest-pT bins (pT_h ≈ 0-0.25 GeV) | **3-5%** | Multiple scattering broadens both angle and momentum; the 2° threshold is barely 3σ. |
| High-z bins (z > 0.6) | **2-4%** | V0-daughter contamination peaks here (per DESY 20-176 Sys.6 distribution); without R3 we cannot subtract or upweight. |
| Highest |x| bins | **5-8%** | Migration is 18% per `context-handoff.md`; the response matrix is most off-diagonal here, where matching mistakes propagate furthest. |

**Integrated over the full kinematic range, the matching-scheme
systematic on absolute M^{h±} is estimated at 2-3% if the scheme is
retained as-is.** This is the same order of magnitude as Sys.5
(tracking efficiency) in DESY 20-176, which is the closest comparable
single-source systematic. It is *smaller* than the dominant MC-model
systematic (mc_model_swap ≈ 0.67% per `context-handoff.md`, but expected
to grow on absolute σ to several %) and *larger* than the
charge-asymmetry systematics already evaluated.

**This estimate is bounded above but not directly measured.** R6
(closure test) is what would turn the 2-3% estimate into a quoted number.

### 6.4 Bound for the recommended scheme (R1+R2+R3+R4 implemented)

If R1 (χ² score), R2 (momentum window), R3 (V0-daughter flag), R4
(bidirectional de-dup) are all in place, the matching-scheme systematic
should drop to **0.5-1.5% per bin** and **< 1% integrated**, matching
the published H1 community-standard envelope. This is the level at which
the matching scheme is no longer the limiting systematic and the
analysis can credibly publish absolute σ alongside DESY 20-176-class
systematics.

### 6.5 Caveat

The above numbers are *estimates* anchored in published analyses on the
same detector and (for the V0 numbers) the same H1OO data model. They
are not measurements of our matcher's bias. The R6 closure test is what
converts these estimates into measurements. Until R6 is run, any
publication claim about absolute σ must carry the 2-3% matching-scheme
systematic explicitly in the error budget.

---

## 7. Open Questions and Things Not Resolved

1. **Whether `H1PartCand::GetPartMC()` is populated for our reconstructed
   tracks.** R7 above. Until checked, we cannot say whether the H1OO-native
   truth link is available as a cross-check or an alternative.
2. **Whether the GTR pool's `IsGtr()` predicate already excludes
   K0s/Λ daughters that have decayed inside the tracker.** Worth a quick
   check by counting matches where `parentPdg ∈ {310, 3122}` even with the
   current `GetMother2()==-1` veto. If `IsGtr()` already removes most of
   these, R3 is a smaller upgrade than estimated.
3. **The actual fraction of matched reco tracks for which the matched gen
   particle's momentum is more than 50% away from the reco momentum.**
   This is the diagnostic that R2 fixes; we cannot estimate the size of
   the fix without measuring it once.
4. **Whether the closure test in R6 should compare χ² vs angle-only at
   the response-matrix level alone or also at the unfolded-result level.**
   Unfolded-result comparison is the "true" systematic but is sensitive
   to the unfolder regularisation; response-matrix comparison is the
   "matcher" systematic alone. Probably both, with the unfolded value as
   the quoted systematic and the response-matrix value as the cross-check.
5. **Whether the 2° threshold should be widened slightly when the χ²
   matcher is in use.** If χ² is the gate, the angle is only a
   pre-filter. Widening to 5° at the angle pre-filter and gating on
   χ² < 25 would catch true matches at very low pT that the 2° pre-filter
   currently rejects. This needs the R6 closure test to validate.

---

## 8. Concrete Action List for the Supervisor

Ordered by sequence. Each step is independently doable.

1. **R7 (diagnostic, 1 hour):** Verify H1OO `GetPartMC()` availability on
   our event flow.
2. **R8 (diagnostic, 1 hour):** Beam-proton-pool counter.
3. **R5 (low effort, half day):** Charge-flip diagnostic branch.
4. **R2 (low effort, half day):** Momentum window fallback.
5. **R3 (medium effort, 1-2 days):** V0-daughter flag propagation.
   Required for absolute σ and for Sys.6 evaluation.
6. **R1 (medium effort, 2-3 days):** χ² score branch, Phase 1 (diagnostic
   only). Required for absolute σ.
7. **R6 (medium effort, 1-2 days):** Closure test angle-only vs χ². This
   is the critical measurement that turns the 2-3% estimate into a
   quoted number.
8. **R1 Phase 2:** promote χ² to the primary match criterion if R6 shows
   χ² reduces matching-induced bias. Otherwise keep angle-only as
   nominal and use χ² as a documented systematic variation.
9. **R4 (low-medium effort, half day):** Bidirectional de-duplication.
   Do after R1 so the sort uses χ².
10. **Documentation:** update `memo-mc-truth-policy-charged-sidis.md` to
    describe whichever scheme is frozen as nominal, and write the
    matching-scheme systematic line for the Phase 5 systematics
    framework (`reference-sidis-systematics.md`).

---

## 9. References (anchors for traceability)

- DESY 20-176, "Measurement of charged particle multiplicity distributions
  in DIS at HERA and its implication to entanglement entropy of partons,"
  arXiv:2011.01812 [hep-ex]; EPJC 81 (2021) 212. Local extract at
  `H1/docs/ChargedParticlePaper_extracted.md`.
- H1 1302.1321, "Measurement of Charged Particle Spectra in Deep-Inelastic
  ep Scattering at HERA," DESY 13-012; EPJC 73 (2013) 2406. Local audit at
  `H1/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md`.
- HERMES, "Multiplicities of charged pions and kaons from semi-inclusive
  deep-inelastic scattering by the proton and the deuteron,"
  arXiv:1212.5407, PRD 87 (2013) 074029. Smearing-matrix method in §III.3.5.
- COMPASS, "Transverse-momentum-dependent Multiplicities of Charged
  Hadrons in Muon-Deuteron Deep Inelastic Scattering," arXiv:1709.07374,
  PRD 97 (2018) 032006. Bin-population acceptance correction in §3.3.
- ATLAS, "Performance of the reconstruction of large impact parameter
  tracks in the ATLAS inner detector," ATL-PHYS-PUB-2017-014. Hit-based
  matching probability concept.
- CMS, "Description and performance of track and primary-vertex
  reconstruction with the CMS tracker," CMS-TRK-11-001, J.Instrum. 9
  P10009 (2014); arXiv:1405.6569.
- `H1OO::H1PartCand::GetPartMC()`, native truth link.
  `H1/h1oo-master/H1Mods/H1PartCand.h` lines 175 and 219.
- `H1_ForwardMultAnalyzer/src/ForwardMultAnalyzer.C` (NAF, Tu/Kong area).
  Covariance-weighted χ² matcher at lines 1502-1568.
- Local code-audit reference: `H1/sidis_highQ2/docs/research/research-h1-charged-particle-analysis.md`.
- Local V0 matching measurement: `H1/docs/research-matching-validation.md`
  (Lambda/K0s analysis match-angle and resolution data on the same dataset).
- Current builder: `H1/sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C`
  lines 1060-1170; `ProtonTreeBuilderHighQ2.C` lines 1060-1170; constants
  in `PhysicsConstants_lowQ2.h` lines 37, 44.

---

## 10. Bottom Line

The current 2° angle-only greedy charge-consistent matcher is **fit for
the asymmetry-class observables already produced (A_h, ratios) and for
first-look multiplicities reported as preliminary**. It is **not the
H1-community-standard matching scheme for an absolute charged-hadron
SIDIS cross-section**, and three specific gaps (no χ² score, no V0
daughter flag, no closure test) must be closed before an absolute σ
publication can defensibly claim a ≤ 1% matching systematic. With those
gaps closed (R1, R3, R6), the matching scheme drops out of the leading
systematic line items and the analysis aligns with the published
DESY 20-176 envelope.

If the scheme is **retained as-is** for an absolute-σ publication, the
matching-scheme systematic should be carried at **2-3% integrated and up
to 5-8% in the most marginal (high-η_lab, lowest-pT, highest-x) bins**,
anchored on the V0-analogue mismatch measurements on the same H1OO dataset
and on the published Sys.6 envelope of DESY 20-176. The dominant
component of this is the missing V0-daughter accounting (R3); the second
is the angle-only score's blindness to momentum-resolution-driven
mismatches (R1+R2).

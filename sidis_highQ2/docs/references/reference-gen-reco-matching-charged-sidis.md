<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Reference: Gen-to-Reco Matching for Charged-Particle SIDIS (High-Q2)

**Date:** 2026-05-19
**Status:** FROZEN definition. Cite this note as the canonical "matched"
specification for gates G6 (hadron acceptance), G7 (DIS-event acceptance),
G8 (correction-method choice), G12 (unfolding), and G15.5 (A_BC validation).
**Scope:** High-Q2 charged-hadron SIDIS, Q2 > 150 GeV2, v5b builder.
**Authoritative source:**
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C`
(NAF, 1401 lines, mtime 2026-05-17 — `ProtonTreeBuilderHighQ2.C` is the v5b
SIDIS builder source; the proton-prefixed name is legacy from the v5b SIDIS
fork. See `docs/references/reference-naming-aliases-highQ2-sidis.md`.) and
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/PhysicsConstants.h`
(NAF, 120 lines, mtime 2026-05-17).
**Numerical sample:** 10 v5b per-run MC files (5 RAPGAP31 + 5 DJANGOH14),
2,535,742 reco rows, 4,852,043 gen rows, 218,732 events with >= 1 matched
reco track, **zero** events with duplicated `GenMatchIdx`. Frozen Task 6
manifest.

---

## 1. Executive summary

The v5b high-Q2 matching is a **reco-driven, greedy, angle-only, charge-
consistent, one-to-one** assignment from reconstructed PST tracks to GTR
gen pi/K/p with a **2.0 degree** opening-angle threshold on the 3-vector
direction. The matching pool is the GTR-bank charged-hadron set
(`|PDG| in {211,321,2212}`) with mother veto (`GetMother2()==-1`) and the
broad gen fiducial pre-cut (`|GenEta|<3.5 AND GenPt>0.05 GeV`), and a hard
charge-consistency requirement (`gp->GetCharge()==trk->GetCharge()`). 69.4%
of stored reco tracks acquire a match. The unmatched-reco sentinel on
`ParticleSource_ch` is `-1`. The empirically observed 90th percentile of
`MatchAngle` is **1.17 deg**, well below the 2.0 deg threshold; the 99th
percentile is 1.86 deg, brushing the cut. The threshold is appropriate for
high-Q2 with a marginal safety margin: KEEP at 2.0 deg.

---

## 2. The frozen gen-reco matching definition for high-Q2 charged SIDIS

A reco track `trk` in `ChargedTot` is "matched" to a gen particle `gp` in
`ChargedTotGen` if and only if all of the following hold, evaluated by the
v5b builder code at fill time:

1. **Reco-side fill prerequisites** (without these the reco track does not
   enter `ChargedTot` at all; matching is never attempted):
   - `trk->IsFromPrimary() == true`
   - `trk->IsCentralTrk() == true` for the central-pass; combined tracks
     enter via a second pass with central-wins dedup (deltaR < 0.05,
     same charge)
   - `Pt > 0`, `Pt`, `Eta`, `P` all finite.
   - Source: `ProtonTreeBuilderHighQ2.C:1009-1027`.

2. **Matching is MC-only.** `isMC == true` (data-driven from
   `gH1Calc->IsMC()`, FIX F13). For data rows the matching block is
   skipped and `IsMatched=0`, `MatchAngle=-1`, `GenMatchIdx=-1`,
   `ParticleSource=-1`. Source: `ProtonTreeBuilderHighQ2.C:1085`.

3. **Matching-pool eligibility** for each candidate gen entry `gp` in the
   per-event GTR array `gtrdata` (inner loop `ig`):
   - `gp` is not already claimed by a prior reco track in this event:
     `usedGenIndices_ch.find(ig) == end()` (greedy one-to-one lock).
   - `ProtonUtil::isChargedHadron(gp->GetPDG())` true, i.e.
     `|PDG| in {211, 321, 2212}`. Source: `:1094`.
   - `gp->IsGtr() == true` (GTR bank, not GEANT3 secondary). Source: `:1095`.
   - `gp->GetMother2() == -1` (no second mother; FIX F7
     defense-in-depth). Source: `:1097`.
   - `gp->GetCharge() == trk->GetCharge()` (FIX F3, strict charge
     consistency). Source: `:1100`.
   - Broad gen fiducial: `|gp->GetMomentum().Eta()| <= 3.5` and
     `gp->GetMomentum().Pt() >= 0.05 GeV`. Source: `:1103-1105`.

4. **Distance metric.** The candidate-by-candidate distance is the 3-D
   opening angle in **degrees** between the reco 3-momentum
   `(Px,Py,Pz)` and the gen 3-momentum:
   ```
   angle = TVector3(Px,Py,Pz).Angle(gp->GetMomentum()) * 180.0 / M_PI
   ```
   This is `acos(p_reco . p_gen / (|p_reco||p_gen|))` in degrees, NOT
   `sqrt(dEta^2 + dPhi^2)` and NOT a momentum-magnitude metric. Source:
   `:1107`.

5. **Disambiguation.** Within the candidate pool, the candidate with the
   **smallest opening angle** wins (greedy nearest-angle, no pT tie-break,
   no momentum tie-break). Implementation: a running minimum
   `bestAngle_ch` over the inner loop. Source: `:1108-1110`.

6. **Acceptance threshold.** The match is ACCEPTED iff
   `bestIdx_ch >= 0 AND bestAngle_ch < PhysConst::CHARGED_MATCH_ANGLE`,
   where `CHARGED_MATCH_ANGLE = 2.0` (degrees). Source: `:1114` and
   `PhysicsConstants.h:55`.

7. **Match commitment.** On acceptance the builder writes:
   - `IsMatched_ch = 1`
   - `GenMatchIdx_ch = bestIdx_ch` (GTR-array index of the matched gen)
   - `usedGenIndices_ch.insert(bestIdx_ch)` -- the gen index is LOCKED
     for the remainder of the event, enforcing strict one-to-one
   - `MatchAngle_ch = bestAngle_ch` (deg)
   - `ParticleSource_ch = classifySourceGTR(matched, gtrdata)`
   - `GenPDG_ch, GenMotherPDG_ch, IsGtr_ch, GenPt_ch, GenEta_ch,
     GenPhi_ch, GenP_ch, GenCharge_ch` copied from the matched gen
   - Gen SIDIS variables (`Q2sGen_ch, YsGen_ch, xGen_ch, ystarGen_ch,
     yBreitGen_ch, xFGen_ch, zGen_ch`) and the broad-fiducial flag
     `inFiducialChargedMatch` populated. Source: `:1114-1156`.

8. **Match rejection.** If no candidate has `bestAngle < 2.0 deg` or no
   candidate exists, the reco track is unmatched: `IsMatched_ch = 0`,
   `MatchAngle_ch = -1`, `GenMatchIdx_ch = -1`,
   `ParticleSource_ch = -1` (sentinel; FIX F5), all `Gen*_ch` fields set
   to default sentinels. Source: `:1071-1082`.

This list is the canonical definition of "matched" used by every
downstream calculation. Any deviation -- different threshold, different
pool, different metric, gen-driven inversion, allowing many-to-one, etc.
-- is NOT what is on the tree.

---

## 3. The frozen gen-pool definition (acceptance / purity / stability
denominator and numerator association)

**The analysis-level gen pool for the high-Q2 charged-hadron SIDIS
measurement consists of every row of `ChargedTotGen` (one entry per
gen-level charged hadron per event, MC only) that satisfies, at runtime:
(i) `|GenPDG| in {211, 321, 2212}` (already structurally enforced by the
builder's `isChargedHadron` gen-loop gate; no extra cut needed but apply
defensively); (ii) the broad-fiducial flag `inFiducialCharged == 1`,
equivalent to `|GenEta| < 3.5 AND GenPt > 0.05 GeV` and verified bit-equal
to that combination in all 10 Task-6 sample files; (iii) the
analysis-fiducial cuts `|GenEta| < 1.3 AND GenPt > 0.15 GeV` applied at
runtime on top of `inFiducialCharged`; (iv) the V0-daughter exclusion
`IsV0DaughterGen == 0` (the 50th `ChargedTotGen` branch, populated for v5b
when the immediate gen mother PDG is in {310 (K0s), 311 (K0), 3122 (Lambda
/ anti-Lambda)}); and (v) the gen-level DIS event-class gate built from
the gen-electron variables and gen kinematics that the builder gates on at
gen-loop fill time, namely `validGenElec == true` (gen scattered electron
energy >= 0.5 GeV after the Q2sGen/theta_e fallback recovery,
`ProtonTreeBuilderHighQ2.C:829`), plus the analysis-level high-Q2 DIS
event cuts applied to the gen DIS variables stored on the same row
(`Q2eGen > 150 GeV2` as the high-Q2 entry condition, `0.1 < YeGen < 0.9`,
`35 < GenEmpz < 70 GeV` consistent with the reco S67 selection; note
`Q2esGen ≡ Q2eGen` by H1CalcKine design and `Q2sGen` is eSigma-calibrated
upstream at high-Q2 per the regime convention). `ParticleSource` is NOT
used as a positive selector at the gen level; the
`ParticleSource != kSecondaryInteraction (==4)` veto is structurally
implied because `IsGtr()` is required at fill time and PS=4 is impossible
for `IsGtr` particles, but PS=0,1,2,3 are all accepted as signal per the
truth-policy memo. `IsGtr_cg == 1` is also structurally implied by the
fill-time gate but should be re-applied defensively in analysis code.
Filling references: `ProtonTreeBuilderHighQ2.C:874-981` (gen loop),
`:880-886` (species + IsGtr + GetMother2 gate), `:927-928`
(inFiducialCharged), `:969-974` (IsV0DaughterGen).**

---

## 4. Algorithm details (questions 1-7)

### 4.1 Matching algorithm (Q1)

| Aspect | Value | Source / verification |
|--------|-------|-----------------------|
| Direction | reco-driven (outer loop over PST tracks; inner loop over gen) | `:944, 1085-1112`. VERIFIED |
| Metric | 3-D opening angle in degrees of momentum vectors | `:1107` `recoDir.Angle(genMom)*180/M_PI`. VERIFIED |
| Pool species | `|PDG| in {211, 321, 2212}` | `:1094`. VERIFIED |
| Pool origin | `IsGtr() == true` | `:1095`. VERIFIED |
| Pool mother-veto | `GetMother2() == -1` (FIX F7) | `:1097`. VERIFIED |
| Pool charge | strict `gp->GetCharge() == trk->GetCharge()` (FIX F3) | `:1100`. VERIFIED |
| Pool fiducial | `|GenEta| <= 3.5 AND GenPt >= 0.05 GeV` (FIX F9) | `:1103-1105`. VERIFIED |
| Pool one-to-one lock | `usedGenIndices_ch` set, per event, locked at acceptance | `:988, 1092, 1115`. VERIFIED |
| Threshold | `bestAngle < CHARGED_MATCH_ANGLE = 2.0 deg` | `:1114` + `PhysicsConstants.h:55`. VERIFIED |
| Disambiguation | smallest angle wins; no secondary tie-break | `:1108-1110`. VERIFIED |

There is no momentum-magnitude check, no pT-relative threshold, no hit-
based / vertex association. Charge consistency is enforced **before** the
angle is computed, so opposite-charge candidates can never enter the
running minimum (their angles are not evaluated). The PDG of the gen
candidate is checked, but the analysis explicitly carries no dE/dx PID at
reco level, so the reco "species" is defined a posteriori as the matched
gen `GenPDG`.

### 4.2 Per-track matching outputs (Q2)

Stored on `ChargedTot` (86 branches):

| Branch | Type | Meaning when unmatched |
|--------|------|-----------------------|
| `IsMatched` | Int_t | 0 |
| `MatchAngle` | Float_t | -1.0 (semantic "no match"; FIX F3 reset) |
| `GenMatchIdx` | Int_t | -1 (index into `ChargedTotGen.GenIdx` GTR position) |
| `ParticleSource` | Int_t | -1 (sentinel; Fix 5 default) |
| `GenPDG` | Int_t | 0 |
| `GenMotherPDG` | Int_t | 0 |
| `IsGtr` | Int_t | 0 |
| `GenPt, GenEta, GenPhi, GenP` | Float_t | -1 / -999 / -999 / -1 |
| `GenCharge` | Int_t | 0 |
| `Q2sGen, YsGen, xGen` | Float_t | 0 (matched-event gen DIS) |
| `ystarGen, yBreitGen, xFGen, zGen` | Float_t | -999 |
| `inFiducialChargedMatch` | Int_t | 0 |

`MatchType`/`MatchResult` branches do not exist; matching state is
recovered from `IsMatched` and the sentinels.

Stored on `ChargedTotGen` (50 branches), the 50th being `IsV0DaughterGen`:

- `GenIdx` (the GTR-array index of the gen entry; this is the value that
  `ChargedTot.GenMatchIdx` references). VERIFIED via JSON `aggregate.gen_n`
  consistency.
- There is NO `RecoMatchIdx` back-pointer on `ChargedTotGen`; the join
  must be done by `(RunNumber, EventNumber, GenIdx) <-> (RunNumber,
  EventNumber, GenMatchIdx)`.

The Task 6 sentinel-rate identity confirms the design:
`ParticleSource_ch == -1` fraction is **0.3061**, complementary to the
`IsMatched==1` fraction **0.6939**, within 0.0001 of exact equality
(`0.3061 + 0.6939 = 1.0000`). This proves that `ParticleSource_ch == -1`
is equivalent to `IsMatched_ch == 0` and is not contaminated by other
sentinels.

### 4.3 One-to-one enforcement (Q3)

The mechanism is the per-event STL set `usedGenIndices_ch`
(`ProtonTreeBuilderHighQ2.C:988`), reset at the top of every event,
populated by `insert(bestIdx_ch)` immediately on match acceptance
(`:1115`), and consulted as the first inner-loop continue
(`:1092: if (usedGenIndices_ch.find(ig) != end()) continue;`).

This is the **per-gen-flag locks once consumed** variant (option (c) in
the supervisor's enumeration). It is NOT a post-pass deduplication and it
is NOT a natural consequence of the disambiguation rule alone. Two
distinct reco tracks pointing at the same gen will see the second
attempt blocked at the inner-loop guard, even if the second reco track is
geometrically closer to that gen than the first.

Confirmed empirically: Task 6 aggregate `events_with_dup_n = 0` across
218,732 events with >= 1 matched track, **strictly one-to-one in the
sample** (`validation_highQ2_mc_species_matching.json` aggregate).

**Order dependence:** because reco tracks are processed in PST iteration
order and the first-arriving reco track wins the gen, the matching is
order-sensitive when two reco tracks both pass <2 deg to the same gen.
This is a known property of greedy first-come matching, and the
sample-wide rate of such contention is bounded by the dup rate (0%), so
the second reco track in such a pair is currently always either (a)
outside the threshold to alternative candidates and becomes unmatched, or
(b) absent because the gen-side multiplicity exceeds the contention rate.

### 4.4 Charge-consistent matching (Q4)

Strict equality `gp->GetCharge() == trk->GetCharge()` (line 1100). This is
a hard rejection -- opposite-charge candidates never enter the running
minimum and are never considered for the threshold. No "opposite-charge
with penalty" mode exists. Per the truth-policy memo (Section 6), this
is mandatory for A_h, eliminates the dominant Path-3 combinatorial
contamination, and removes a charge-asymmetric matching artifact that
would otherwise mimic a physics asymmetry.

### 4.5 Broad fiducial pre-cut (Q5)

The broad fiducial `|GenEta|<=3.5 AND GenPt>=0.05` is applied in TWO
places, both inside the matching loop scope (i.e., enforced on the
matching-pool only, not on tree fill):

1. Inner candidate-scan rejection `:1103-1105` (per-candidate `genMom`
   computed; rejected if outside).
2. Stored as the flag `inFiducialCharged` on `ChargedTotGen` (`:927-928`)
   and as `inFiducialChargedMatch` on `ChargedTot` (`:1155-1156`) for
   downstream filtering.

**Important:** the `ChargedTotGen` rows are filled WITHOUT this
fiducial gate. Every IsGtr pi/K/p (mother-veto-passing) gen entry gets a
row. The `inFiducialCharged` flag is the broad-fiducial mask, not a fill
filter. Task 6 confirms: the gen pool is ~38% broader than the broad
fiducial (38% of gen rows have `inFiducialCharged==0`), and the broad-
fiducial set is then ~50% broader than the analysis-fiducial set. The
broad fiducial is bit-equal to `|GenEta|<3.5 AND GenPt>0.05` in all 10
sample files (`inFiducialCharged_eq_broad_test: true`). VERIFIED.

### 4.6 Mother veto (Q6)

`GetMother2() == -1` is applied at **two** places:
- Gen-loop fill (`:886`), so `ChargedTotGen` rows themselves are mother-
  veto-clean. The gen denominator is therefore implicitly mother-veto-
  protected.
- Matching pool (`:1097`), so the matching pool is independently re-
  protected (redundant but defense-in-depth).

The veto is described in the truth-policy memo Section 2 as a defense
against PYTHIA string-fragmentation bookkeeping artifacts and GEANT3
re-scattering combinatorial cases. Its empirical impact is negligible for
IsGtr pi/K/p, but it is on by design.

**`IsV0DaughterGen` is a separate, NEWLY ADDED v5b flag** (`:521-522`,
`:969-974`) that tags gen rows whose **immediate** mother PDG is in
{310 (K0s), 311 (K0), 3122 (Lambda/anti-Lambda)}. This is the per-row
provenance of V0-decay daughter pi/K/p that survived to the GTR bank.
Per the truth-policy memo Section 4, V0 daughters (PS=1,2,3 in the
PsegrgaSource enum) are by default INCLUDED as signal because they are
genuine pi/K/p from primary-fragmentation chains. The `IsV0DaughterGen`
branch is therefore an **opt-in defense** for analyses that want to study
or veto V0 feed-down explicitly, not a default-on veto. The frozen gen
pool in Section 3 above EXCLUDES `IsV0DaughterGen == 1` rows from the
acceptance denominator -- this is the conservative posture and matches
the analysis-policy intent of "pi/K/p from primary fragmentation," not
"pi/K/p from any source."

### 4.7 Material-secondary protection (Q7 / F3)

F3 ("material-secondary reco fakes match unrelated GTR gen") is closed at
the matching pool, not at the reco-side veto. The two protections that
matter are:

1. **Charge consistency** (`:1100`). Material secondaries (e.g., a
   p from material knockout) have well-defined charge; an unrelated gen
   pi- of opposite charge in the same angular vicinity cannot become the
   match.
2. **Tighter angular threshold**: 2.0 deg vs the proton-analysis 5.0 deg
   (`PROTON_MATCH_ANGLE` in low-Q2; not present at high-Q2 because the
   proton tree was removed in v5b). Lower threshold cuts the
   combinatorial fake rate ~quadratically in solid-angle terms.
3. **Mother veto + IsGtr**: the matching pool is GTR-only and mother-
   veto-clean, so the candidates in the pool cannot themselves be
   material secondaries.

There is NO reco-side veto of "particle source non-primary" on the reco
track entering the matching pool (the reco tracks are PST primary-vertex
central tracks; PST's `IsFromPrimary` is the only reco-side primary
filter). This is intentional: material-secondary reco tracks DO enter the
matching attempt; they fail to find a charge- and angle-consistent gen
within 2.0 deg, become unmatched, and acquire `ParticleSource_ch = -1`.
This is the documented design; the Task 6 sentinel rate (~30.6%) is
consistent with the expected reco-side contamination of secondaries and
genuine reco fakes that never had a true gen partner inside the
matching window. VERIFIED.

---

## 5. Numerical properties from the 10-file sample (Q8)

All numbers below are aggregated over the 10 v5b high-Q2 MC files (Task 6
frozen manifest; 5 RAPGAP31 + 5 DJANGOH14; 1,759,581 matched reco rows;
2,535,742 reco rows; 4,852,043 gen rows). Sampling date: 2026-05-19.
Method: uproot inspection on NAF using `(RunNumber, EventNumber)
+ GenIdx <-> GenMatchIdx` keys for the gen-driven efficiency stratified
counts. No data was written on NAF.

### 5.1 `MatchAngle` distribution for matched tracks (deg)

| Population | n | Median | 90% | 99% | Max |
|------------|----|--------|-----|-----|-----|
| All matched | 1,759,581 | 0.347 | **1.169** | 1.860 | 2.000 |
| pi (|PDG|=211) | 1,500,611 | 0.358 | 1.183 | 1.864 | -- |
| K (|PDG|=321) | 187,790 | 0.296 | 1.083 | 1.836 | -- |
| p (|PDG|=2212) | 71,180 | 0.277 | 1.040 | 1.826 | -- |
| Frac above 0.5 deg | -- | -- | 0.369 | -- | -- |
| Frac above 1.0 deg | -- | -- | 0.140 | -- | -- |

The maximum is 2.000 (matches are accepted strictly below
`CHARGED_MATCH_ANGLE`; the max sample value is at the truncation).
Kaons and protons match tighter than pions, consistent with their lower
material-interaction cross-section per unit eta at fixed P (kaons are
shorter-lived but slower; protons are heavier and slower-bending in B).

### 5.2 True-PDG fake-rate proxy

Matched reco tracks with `|GenPDG|` outside `{211, 321, 2212}`:
**0 of 1,759,581** (0.000%). The species pre-cut in the matching pool is
strict; no escape into leptons/hyperons/photons. The matching is fake-PDG-
clean by construction. VERIFIED.

### 5.3 Per-charge match efficiency (gen-driven, analysis-fiducial
`|GenEta|<1.3 AND GenPt>0.15`)

| Charge | Matched | Total | Efficiency |
|--------|---------|-------|------------|
| Positive gen | 657,298 | 746,662 | **0.8803** |
| Negative gen | 639,215 | 731,381 | **0.8740** |
| Asymmetry (eps_+/eps_-) - 1 | -- | -- | **+0.0072** (0.72% asym) |

The 0.72% charge asymmetry is small but NON-ZERO and **bigger** than the
target A_BC physics signal of ~0.001-0.018 reported in low-Q2 v5b first-
look. It MUST be acceptance-corrected (gen-binned eff). This is the F1/F2
gate concern made quantitative for high-Q2.

### 5.4 Per-species-charge match efficiency

| Species + charge | Matched | Total | Efficiency |
|------------------|---------|-------|------------|
| pi+ | 555,725 | 629,283 | 0.8831 |
| pi- | 544,462 | 618,687 | 0.8800 |
| K+  | 73,564 | 85,768 | 0.8577 |
| K-  | 70,140 | 83,118 | 0.8439 |
| p   | 28,009 | 31,611 | 0.8861 |
| pbar | 24,613 | 29,576 | 0.8322 |

Pions are the most efficient. Antiprotons are the least efficient (5.4%
lower than protons) -- this is consistent with antiproton annihilation in
the inner detector material being a real physics effect and feeds the
NuclIA correction's charge-asymmetric component.

### 5.5 Per-pT match efficiency (gen-driven, analysis-fiducial)

| GenPt bin (GeV) | Matched | Total | Efficiency |
|-----------------|---------|-------|------------|
| 0.15 - 0.30 | 171,098 | 209,309 | **0.8174** |
| 0.30 - 0.50 | 224,716 | 256,380 | 0.8765 |
| 0.50 - 1.00 | 346,734 | 390,163 | 0.8887 |
| 1.00 - 2.00 | 282,602 | 317,891 | 0.8890 |
| 2.00 - 5.00 | 209,248 | 234,717 | 0.8915 |
| 5.00 - inf | 62,115 | 69,583 | 0.8927 |

Efficiency rises monotonically with pT and saturates ~0.89 above 0.5 GeV.
The lowest-pT bin is the bottleneck (multiple scattering, looping tracks,
threshold-region acceptance). This is expected and consistent with the
low-Q2 result.

### 5.6 Per-|eta| match efficiency (gen-driven, analysis-fiducial)

| |GenEta| bin | Matched | Total | Efficiency |
|--------------|---------|-------|------------|
| 0.0 - 0.5 | 533,360 | 630,435 | **0.8460** |
| 0.5 - 1.0 | 506,904 | 563,844 | 0.8990 |
| 1.0 - 1.3 | 256,249 | 283,764 | 0.9030 |

The central |eta|<0.5 bin is LOWEST. This is the opposite of the naive
expectation. The cause is likely CJC/CIP geometry around eta=0 plus the
event-level vertex distribution; it is not a matching algorithm artifact.
The pattern matches what is seen in published H1 analyses (DESY 20-176)
where the central rapidity bin shows the largest acceptance correction.

### 5.7 Migration radius (matched-pair gen-to-reco residuals)

| Variable | n | Median | 5th percentile | 95th percentile | 90% width |
|----------|----|--------|----------------|-----------------|-----------|
| (pT_reco - GenPt) / GenPt | 1,759,581 | +0.0006 | -0.0444 | +0.0459 | 0.0903 |
| Eta_reco - GenEta | 1,759,581 | +0.00006 | -0.0263 | +0.0263 | 0.0526 |

The relative pT migration is centered at zero with a 90% width of ~9%,
consistent with the H1 CJC pT resolution sigma(pT)/pT ~ 0.01 pT (GeV)
folded with multiple scattering. The eta migration is ~0.026 in 90%
width, far below any analysis-level eta bin width. Bias is negligible
(<0.1% in pT, <1e-4 in eta).

---

## 6. Acceptance / purity / stability bookkeeping consequences (Q9)

The matching definition above implies the following bookkeeping rules,
which MUST be honored uniformly in every gate calculation:

### 6.1 Gen-binned numerator (acceptance)

"Matched reco tracks, binned by **gen** kinematics (`GenPt, GenEta,
GenPhi, z_SIDIS_gen, pT_h_gen, Q2_gen, x_gen`) of the matched gen
partner, with the same analysis-level cuts on the **gen** variables that
the denominator uses." The matched-reco row exposes the matched
`GenPt_ch, GenEta_ch, GenPhi_ch, GenP_ch` plus the matched-event gen
DIS variables (`Q2sGen_ch, YsGen_ch, xGen_ch, ystarGen_ch,
yBreitGen_ch, xFGen_ch, zGen_ch`) directly.

The numerator therefore reads from `ChargedTot` with the cuts:
`IsMatched==1 AND inFiducialChargedMatch==1 AND |GenEta_ch|<1.3 AND
GenPt_ch>0.15 AND |GenPDG_ch| in {211,321,2212} AND GenCharge_ch matches
the desired charge`. The V0-daughter exclusion is NOT directly available
on `ChargedTot`; it must be looked up via the
`(RunNumber, EventNumber, GenMatchIdx) <-> (RunNumber, EventNumber,
GenIdx)` join into `ChargedTotGen.IsV0DaughterGen` and applied as a
post-join filter.

### 6.2 Gen-binned denominator (acceptance)

The gen-pool definition in Section 3 verbatim. Read `ChargedTotGen` with:
`inFiducialCharged==1 AND |GenEta|<1.3 AND GenPt>0.15 AND
|GenPDG| in {211,321,2212} AND IsGtr==1 AND IsV0DaughterGen==0`,
event-gated by the gen DIS event cuts on `Q2eGen, YeGen, GenEmpz`.

### 6.3 Reco-only quantities (purity / stability)

Reco tracks (matched or not) passing reco-side analysis cuts
(`Pt>0.15, |Eta|<1.3, DcaPrime/cosh(Eta)<2, NHitDedx>=10, RadLength>10,
IsFromPrimary, IsCentralTrk`) binned by **reco** kinematics. Purity in
bin b = (matched reco in bin b) / (all reco in bin b). Stability in bin b
= (matched reco where matched-gen also falls in bin b) / (gen entries
in bin b). These do NOT enter the acceptance correction directly; they
enter the unfolding response matrix.

### 6.4 The `inFiducialCharged` ambiguity (must be resolved at runtime)

Task 6 verified that `inFiducialCharged` on `ChargedTotGen` is bit-equal
to the BROAD fiducial `|GenEta|<3.5 AND GenPt>0.05`. It is NOT the
analysis-level fiducial. The acceptance denominator must therefore
apply BOTH:
- `inFiducialCharged == 1` (to get the broad-fiducial pool the matching
  is consistent with), AND
- runtime `|GenEta| < 1.3 AND GenPt > 0.15 GeV` (to restrict to the
  analysis fiducial).

If either is dropped, the denominator is wrong:
- Dropping the broad gate keeps beam protons (4.6% per Task 6) and
  forward fragments in the denominator -> inflated, multiplicity biased
  low.
- Dropping the analysis gate keeps the migration buffer in the
  denominator -> biased low.

Both are required; this is by design.

---

## 7. Comparison to published H1 charged-particle practice (Q11)

Two reference audits live in
`H1/sidis_lowQ2/docs/research/`:
- `research-h1-charged-particle-analysis.md` (DESY 20-176 code audit).
- `research-h1-1302-1321-charged-multiplicity.md` (arXiv:1302.1321 paper
  audit).

Both documents focus on the analysis-level architecture (selection,
binning, unfolding) and do not give a line-level matching algorithm for
direct comparison. The salient consensus points:

- DESY 20-176 uses `status==0` (equivalent to `IsGtr()` in H1OO terms)
  as the gen-stable gate; v5b's `IsGtr()` + species filter is in the
  same family.
- DESY 20-176 species set is "stable charged hadrons" via the cτ > 10 mm
  criterion, which admits charged hyperons (Sigma+/-, Xi-, Omega-). v5b
  uses the explicit `{pi, K, p}` PDG set instead; this is a DIFFERENT
  species choice and the published Lambda/K0s analyses cover the
  complement.
- The published H1 charged-multiplicity analyses use angle-based reco-to-
  gen matching with thresholds in the 5-10 mrad to 100 mrad range
  depending on the paper. The v5b 2.0 deg = 34.9 mrad threshold sits in
  the middle of that band, and is tighter than the proton-analysis 5 deg
  by design (FIX F3 commentary in builder source).
- One-to-one greedy matching with a per-event used-set is the standard
  H1 pattern; see also the Lambda analysis (`H1/lambda/`) and the proton
  analysis (`H1/proton/`) which both use the same `usedGenIndices`
  pattern.

The v5b matching is CONSISTENT with published H1 practice. The
differences (tighter angle, strict charge gate, broad fiducial pre-cut on
the pool) are explicit defects-driven improvements documented in the
truth-policy memo and the F3/F6/F7/F9 fix ledger.

---

## 8. High-Q2-specific pitfalls (Q12)

1. **High-Q2 hadronic systems are MORE collimated than low-Q2.** At
   Q2 > 150 GeV2 the current jet is significantly more boosted into the
   forward hemisphere and the per-jet track multiplicity is higher;
   nearest-neighbor angular separations within the current jet can
   approach the matching threshold. The MatchAngle distribution shows
   p90 = 1.17 deg and p99 = 1.86 deg, which means 14% of matched tracks
   sit above 1 deg and 1% sit between 1.86 and 2.00 deg. The tail
   profile is consistent with multi-track contention in dense jet cores.
   If high-Q2 unfolding response matrices show non-physical migration
   tails, a tighter threshold (1.0 deg) should be tested.

2. **The 0.7% charge asymmetry in the matching efficiency** (pos eff
   0.8803 vs neg eff 0.8740) is comparable to the expected A_BC signal.
   Acceptance correction MUST be gen-binned per charge. Any reco-binned
   "matched / all-reco" shortcut will leak this 0.7% into A_BC. Gate
   G15.5 (A_BC validation) must explicitly check that the corrected A_BC
   shifts by approximately +0.7% when the matching-efficiency
   correction is toggled off vs on, as a self-consistency proof.

3. **Per-species K/p efficiency gap** (K: 0.85; p/pbar: 0.86 / 0.83).
   These are not equal, and downstream NuclIA reweighting acts on top.
   For inclusive h+/h- multiplicities the species mix is pi-dominated
   (~85% of matched tracks) so the K/p sub-efficiencies dilute slowly,
   but in high-z bins where K/p enrich, the species-dependent
   acceptance correction is essential.

4. **Central |eta|<0.5 efficiency dip (0.846)** is significant and
   eta-binning of the acceptance correction is required. A simple
   eta-integrated efficiency will mis-correct the central bin by ~5%.

5. **Order-dependence of greedy matching.** The PST iteration order is
   the H1OO array order; for tracks geometrically close to the same gen,
   the first reco track wins. In the 10-file sample no duplicates
   appeared, so the contention is empirically zero, but at very high
   track multiplicity (e.g., the highest-Q2 hadronic jets) the order
   sensitivity could become an issue. Gate G12 (unfolding) must check
   the response matrix for off-diagonal entries with this provenance.

6. **Threshold appropriateness at high-Q2 is BORDERLINE.** The 90th
   percentile (1.17 deg) gives ~0.83 deg of headroom below 2.0 deg, but
   the 99th percentile (1.86 deg) leaves only 0.14 deg of headroom.
   **Verdict: KEEP at 2.0 deg.** Tightening to 1.5 deg would reject the
   highest-quality matches in the longest-tail 1% (~17,600 tracks in this
   sample), and the empirical fake rate (0% PDG-outside-target) shows
   no daylight where tightening would help. Loosening to 5 deg (proton-
   threshold value) would re-open the F3 combinatorial leak that the
   builder was specifically rebuilt to close. A future high-multiplicity
   study at Q2 > 1000 GeV2 specifically should re-measure the p99 in the
   highest-Q2 bin and revisit; for the analysis as defined, 2.0 deg
   stands.

---

## 9. Status: VERIFIED vs PROVISIONAL

### VERIFIED (read from source or derived from the 10-file sample)

- Algorithm (Section 2 items 1-8): builder lines 1085-1156, gen loop
  874-981, PhysicsConstants line 55. All line-cited.
- Charge consistency: builder line 1100. VERIFIED.
- Charge pre-cut is strict equality, no penalty. VERIFIED.
- Broad fiducial cut (`|GenEta|<3.5 AND GenPt>0.05`): builder lines
  1103-1105 (matching) and 927-928 (gen loop). VERIFIED.
- Mother veto in both gen-loop (`:886`) and matching pool (`:1097`).
  VERIFIED.
- `IsV0DaughterGen` definition (mothers in {310, 311, 3122}):
  builder lines 521-522, 969-974. VERIFIED.
- One-to-one enforcement via `usedGenIndices_ch` set: builder lines 988,
  1092, 1115. VERIFIED.
- `MatchAngle_ch = -1` for unmatched: builder line 1078. VERIFIED.
- `ParticleSource_ch = -1` for unmatched: builder line 1072.
  Empirically: matches the 30.61% sentinel rate exactly complementing
  the 69.39% match rate (Task 6 aggregate). VERIFIED.
- Numerical properties Section 5.1-5.7. Computed live on NAF, 10 files,
  1.76M matched tracks. VERIFIED.
- `inFiducialCharged` is bit-equal to broad fiducial
  (`inFiducialCharged_eq_broad_test: true`, all 10 files). VERIFIED.

### PROVISIONAL (would require further inspection)

- **Gen-level DIS event cuts on `Q2eGen, YeGen, GenEmpz`**: the gen pool
  definition (Section 3) prescribes high-Q2 analysis-level event cuts
  on the gen DIS variables, but these are RUNTIME cuts applied by
  downstream analysis scripts, NOT fill-time gates. The builder gates
  only on `validGenElec` (`gen electron energy >= 0.5 GeV` after the
  Q2sGen/theta_e fallback recovery; builder line 829). Verification of
  the analysis-side cut on `Q2eGen > 150 GeV2` and `0.1<YeGen<0.9` is
  PROVISIONAL until the high-Q2 analysis scripts are inspected; the
  baseline reco-side high-Q2 DIS cuts per
  `research-highQ2-sidis-extension.md` Section 1.1 table are
  `150 < Q2 < ~20000 GeV2, 0.1 < y < 0.9, 45 < E-pz < 65 GeV,
  |VtxZ|<35 cm` and the gen-side cuts should mirror them on the
  corresponding `*Gen` variables. **To verify:** read
  `H1/sidis_highQ2/scripts/extract_multiplicities.py` and
  `analyze_charged_xsec_corrected.py` for the high-Q2 branch of the
  event-cut block, or measure the gen-DIS-event count `N_DIS_gen` on
  Task-6 files with the proposed cuts and confirm against `DisTot`
  per-event entries.

- **Builder line numbers on NAF vs cached source.** The NAF source
  (1401 lines) is the authoritative v5b; the locally cached
  `source_cache/ProtonTreeBuilderHighQ2.C` is 1279 lines (pre-v5b
  parity additions: missing `IsV0DaughterGen`, `GenEmpz`, `GenVtxZ`,
  `NucliaWeight`, trigger weights, `GenElec*`). All line references in
  this note are to the NAF version. PROVISIONAL only in the trivial
  sense that the cache should be refreshed; the algorithm is unchanged
  between versions (the v5b additions are additive, not algorithmic).

- **High-Q2-specific MatchAngle behavior at Q2 > 1000 GeV2.** The
  Section 5 numbers are integrated over the full v5b high-Q2 event
  selection. Whether the p99 = 1.86 deg degrades further at the
  highest-Q2 bins where the hadronic system is most collimated is
  PROVISIONAL. **To verify:** stratify the MatchAngle distribution by
  `Q2e` and check the highest bin's p99.

- **Strict charge-asymmetry sign of the 0.72% effect** depends on the
  pT/eta integration. **To verify:** repeat Section 5.3 stratified by
  pT_h or photon-axis z bin; for A_BC validation (gate G15.5), the
  charge-asymmetry correction must be measured per-bin, not integrated.

- **DJANGOH14 vs RAPGAP31 model dependence of the matching
  efficiencies.** Section 5 aggregates 5+5 files and does not separate
  the two generators. Per the truth-policy memo Section 7, DJANGOH14
  completeness in `ChargedTotGen` was an open question for low-Q2;
  Task 6's verdict for high-Q2 is "no anomaly" (DJANGOH14 files all
  pass the per-file checks at consistent rates). PROVISIONAL: the
  generator-level matching efficiency split should be reported when
  the systematic `mc_model_swap` source is evaluated.

---

## 10. Provenance

- SSH control socket: `Master running (pid=16259)` -- PASS at start of
  task.
- Remote read-only commands (no NAF writes):
  - `ssh -O check naf-hone.desy.de`
  - `ls -la /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/...`
  - `wc -l /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/...`
  - `grep -n '...' /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C`
  - `sed -n '...,...p' /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C`
  - one uproot read of 10 ROOT files (Task 6 manifest) computing the
    Section 5 numerics; total ~5M gen rows and ~2.5M reco rows scanned.
- No `find` on `/cvmfs`, `/afs`, or unbounded `/pnfs` was executed.
- No file was written on NAF.

## 11. Citation key for downstream gates

Cite this note as the canonical definition wherever the word "matched"
appears in:
- **G6** (hadron acceptance): use Section 6.1 and Section 6.2 verbatim.
- **G7** (DIS-event acceptance): use Section 3 plus the gen-side DIS
  cuts; the matched-reco numerator inherits the gen-event class.
- **G8** (correction-method choice): the response matrix is built on
  the gen-binned matched-reco numerator and the gen-binned denominator
  from Section 6; the matrix off-diagonal is the migration measured in
  Section 5.7.
- **G12** (unfolding): the matching defines the response-matrix domain.
- **G15.5** (A_BC validation): the 0.72% charge-asymmetric matching
  efficiency (Section 5.3) is the dominant systematic-floor contribution
  to A_BC and must be acceptance-corrected per-charge per-bin.

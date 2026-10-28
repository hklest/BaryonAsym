# v2 truth / background track selection: exact specification (DIAGNOSTIC)

> **SUPERSEDED 2026-10-21 by v3** (`trackclass_v3_provenance_2026-10-21.md`). Per the user ruling, background is defined by provenance only (STR-only truth links). The P/GenP < 0.7 and duplicate classes are unfolded, not subtracted.

- Implemented in NAF
  `H1ChargedSidis_HighQ2/scripts/unfolding/trackclass_variant_2026-10-20/build_migration_trackclass_variant.py`
  (md5 `f69f3c1e5b37fe58fcb8b1d70de21572`).
- Environment for v2: `TC_CLASSES=absorbed,keeper,duplicate TC_RLO=0.7 TC_RHI=1e9 TC_AMAX=180 TC_USE_QFLIP=0`.
- Validated in `docs/trackclass_triplecheck_2026-10-21.md`.
- Applies only to MC `ChargedTot` rows, i.e. when building the response. The data vector is
  untouched; background enters data only through the response `f_i` and the external e/V0/lowQ2
  templates.

## Step 0: reco selection (identical to the OPT-A nominal)

| | cut |
|---|---|
| event | 150 < Q2es < 20000, 0.1 < Yes < 0.7, Empz > 45, \|PVz\| < 35 cm |
| track | Pt > 0.15 GeV, \|Eta\| < 1.6, NHitDedx ≥ 5, RadLength ≥ 5 cm, \|DcaPrime/cosh Eta\| < 2 cm, IsFromPrimary == 1, (IsCentralTrk == 1 or TrackType ∈ {1,2}), xF_SIDIS_es < 0 |
| cone | α(track, reco scattered e) ≥ 0.2 rad |
| guard | 0 ≤ z ≤ 1, valid fine reco bin |

## Step 1: excluded from the response (external templates subtract them from data)

| priority | predicate | class |
|---|---|---|
| 1 | IsMatchedElectron == 1 (GTR or STR) | electron → e template |
| 2 | IsMatchedV0Daughter == 1 (K0S/K0L/K0/Λ within 6 ancestor hops) | V0 → V0 template |

## Step 2: gen-candidate test

`gen_candidate_ok` = all of:
- IsMatched == 1 (Tier-0 DTRY link, or Tier-2 angular match);
- |GenPDG| ∈ {211, 321, 2212};
- IsMatchedV0Daughter == 0;
- 150 < Q2_es_gen < 20000, 0.1 < Y_es_gen < 0.7, GenEmpz > 45, xFGen < 0;
- finite GenPt/GenEta/GenPhi/zGen;
- valid gen bin (OPT-A: Q2esGen cell, zGen, nominal-beam pT).

If not `gen_candidate_ok`: **fake column** (fc0; fc4/fc5 if det.int.). This covers:
- unmatched tracks;
- matches to non-π/K/p (μ, n, Λ, Σ±, Ξ…);
- tracks whose gen event is outside the fiducial region.

## Step 3: classes among `gen_candidate_ok` legs (evaluated in this order)

| class | predicate (on top of `gen_candidate_ok`) | v2 route |
|---|---|---|
| det.int. | IsStr == 1 & IsGtr == 0 & \|GenMotherPDG\| ≠ 321 | **background**: fake column (fc3, S2') |
| K± keeper | IsStr == 1 & IsGtr == 0 & \|GenMotherPDG\| == 321 | **background**: fake column (fc8) *(v2 change; was signal in S2')* |
| absorbed secondary | MatchTier == 0 & IsGtr == 1 & P/GenP < 0.7 | **background**: fake column (fc6) |
| everything else | Tier-0 GTR with P/GenP ≥ 0.7 (any angle, any charge, including P/GenP > 1.5); Tier-2 matches | **signal candidate** |

## Step 4: one reco track per truth particle

- Key: (Run, Event, signed GenPDG, GenPt, GenEta), rounded to 1e-8.
- Among signal candidates sharing a key, keep the one with the smallest |ln(P/GenP)| + angle/10°.
- The others are routed to the fake column as **duplicates** (fc9). They are a double-count
  correction, quoted separately.

## Step 5: filling

- **Signal:** body R[reco bin, gen bin], weight w1·w2·NucliaWeight (clipped to [0,10]), booked in
  the gen-charge response. It records the gen partner's consumed key.
- **Background:** fake column, same weight. Booked under the reco charge (fc3 keeps the S2'
  gen-charge convention).
  - fc6, fc8 and fc9 record no consumed key, so their GTR partner falls to the miss row unless a
    signal leg consumed it.
  - fc3 records a key, but its STR partner is never in the GTR-only truth.

## Truth definition (unchanged)

`ChargedTotGen` rows with:
- GTR only, Mother2 == −1, IsV0DaughterGen == 0;
- |GenPDG| ∈ {π, K, p};
- the same gen fiducial and gen binning.

Each pool row is either consumed (by a signal leg) or a miss (weight w1·w2).

## Kept as signal although ambiguous (definitional residual, ±0.03–0.04% on the central value)

| class | share of body | why kept |
|---|---:|---|
| angle ≥ 10° with P/GenP ≥ 0.7 | 0.24% | includes elastically scattered primaries |
| P/GenP > 1.5 | 0.05% | a secondary can't exceed its parent's momentum; these are mismeasured real tracks |
| charge flips | 0.01% | |
| Tier-2 | 0.2% | kinematically consistent with the gen particle; no truth link |

The removed absorbed class is ~3–4% primary-like by the matched DCA′ fit.

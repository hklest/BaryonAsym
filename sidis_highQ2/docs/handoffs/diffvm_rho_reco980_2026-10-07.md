# Handoff — DIFFVM exclusive-ρ background vector in reco980 (v6 finalcandidate)

**Date:** 2026-10-07 · **Status:** complete, validated, central release untouched.
**Study:** `H1ChargedSidis_HighQ2/output_v6/studies/diffvm_rho_reco980_2026-10-07/`
**Script:** `sidis_highQ2/scripts/backgrounds/diffvm_rho_reco980_2026-10-07/extract_diffvm_rho_reco980.py`

## Verdict

**Exclusive elastic ρ is bin-localized, not globally negligible.** Integrated over the
reco980 grid it contributes 285.78 ± 6.23 scaled tracks against 3,865,073 data tracks
(alpha0p2 denominator) = **0.0074%** — negligible for the total. But it is concentrated at
high z: the z ∈ [0.7, 1.0) row carries 758/2102 accepted ρ tracks, yield 103.05 vs 4,735
data tracks = **2.18% row-level** (h+ 2.00%, h− 2.41%); the S0_x0 cell in that row is
**4.53%**; z ≥ 0.5 overall is 0.51%. Even restricted to bins with data denominator ≥ 100,
**25 bins exceed 0.2%** (max 3.65%, idx 134, S0_x1 h+, z[0.7,1.0), pT[1.0,1.5)); no bin
with denominator ≥ 1000 exceeds 0.2%. The old scoping claim ("0.5% at z≥0.7 in
150–300 GeV², 0.05% elsewhere") is **superseded**: the measured z≥0.7 row level is 2.18%,
and the z<0.5 region is 0.004% integrated (consistent with "negligible elsewhere").

**Decision implication:** if the final measurement keeps z ∈ [0.7, 1.0) bins, this vector
must enter the reco-space background subtraction (or an assigned systematic); if those bins
are masked, elastic exclusive ρ can be documented as negligible. z ∈ [0.5, 0.7) at 0.136%
row-level sits below a 0.2% threshold but should be re-evaluated once the high-z policy is
fixed. The α cone veto is irrelevant for this background: variant (b) removes **0/2102**
tracks (0 ρ pions, 0 per charge).

**Scope caveat:** reqIDs 8445–8448 are DIFFVM `diffvm11001` exclusive **elastic** ρ
(`rho_*_ela`). Proton-dissociative ρ is **not** covered by this vector.

## Inputs and normalization

- 404 `sidistree` files (101 per reqID; 8445/8446/8447/8448, all-decay ρ only; charged-only
  8407/8412/8417/8422 and φ/ω samples forbidden and absent).
- Binding scale `s_c = 351000.0/2581730.0 = 0.1359553477706809` (L_data/L_MC combined),
  SUM-THEN-SCALE-ONCE: μ = s_c·ΣN, Var = s_c²·ΣN (Poisson on raw counts). `weight1·weight2
  == 1` asserted on every accepted track; sumw/sumw2 never used.
- See `sidis_highQ2/data/normalization/diffvm_rho_v6_normalization_2026-10-07/NORMALIZATION.md`.

## Selection

- **Variant (a)** = central v6 finalcandidate: event 150 < Q2es < 20000, 0.1 < Yes < 0.7,
  Empz > 45, |PVz| < 35; track Pt > 0.15, |Eta| < 1.6, NHitDedx ≥ 5, RadLength ≥ 5.0,
  |Dca/cosh(Eta)| < 2, IsFromPrimary == 1, track-class ok, xF_SIDIS_es < 0; truth vetoes
  IsMatchedElectron == 0 & IsMatchedV0Daughter == 0. z/pT from the canonical
  `_compute_pTh_cm_and_z` (pion mass, W-rest boost, invariant z) — not stored `z_SIDIS_es`.
- **Variant (b)** = (a) + α(track, reco scattered electron) ≥ 0.2 rad, applied in addition
  per task definition. (In the live alpha0p2 package the cone replaces the electron truth
  veto; here 41 electron-truth-tagged reco-quality tracks are removed by the truth veto in
  both variants — recorded as `sel_recoquality_electron_tagged`, diagnostic only.)
- Grid: 7 cells × 7 z-bins [0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0] × 10 pT-bins (GEN
  [0, 0.5, 1, 2, 4, 8], ×2 split) × 2 charges = 980, charge-major.
- Mask caveats (campaign record): operationalized mask omits the v4 Pt < 15 upper bound and
  uses RadLength ≥ 5 (v4: strict > 5); negligible for ρ pions.

## Validation (all PASS)

| Check | Expected | Measured |
|---|---|---|
| files / per-reqID | 404 / 101×4 | 404 / 101×4 PASS |
| ChargedTot / ChargedTotGen | 3742 / 102782 | 3742 / 102782 PASS |
| campaign mask (per-sample) | 2253 (528/587/568/570) | 2253 exact PASS |
| z005 counter (stored z ≥ 0.05) | 2142 (494/558/545/545) | 2142 exact PASS |
| mask-scaled integral | 306.31 ± 6.45 | 306.3074 ± 6.4532 PASS |
| schema (len 980, h+ first, finite, ≥0) | all | all PASS |

Smokes (single-file, pre-batch): 8445_0000 CT=1, CTG=224, EvlTree=56, mask contribution 0;
8448_0001 CT=12, CTG=210, EvlTree=53, mask=7, sel_a=6. Batch: 8 shard jobs (Condor
5385894) + merge (5385898), all rc=0.

**Integrated yield:** the 980-vector in-grid sum is **285.778 ± 6.233** (h+ 1081 → 146.97,
h− 1021 → 138.81). The difference to 306.31 is 151/2253 mask tracks outside the reco980
grid under canonical z/pT (`fail_grid=151`, `fail_z_nonfinite=0`, `fail_charge_zero=0`;
111/2253 have stored z < 0.05). The exact cause split (z<0 / z≥1 / pT≥8 / cell-out) was not
recorded — optional one-job follow-up, not needed for the verdict. For reco980-space
subtraction only the in-grid vector matters.

## Per-bin results (identical for variants a and b)

- 332/980 bins populated. Bins with fraction > 0.05%: **180**; > 0.2%: **112** — all in
  the high-z / small-denominator region.
- Top bin: idx 270 (S1_x1 h+, z[0.7,1.0), pT<0.25): 6 MC counts, yield 0.816 ± 0.333,
  fraction **0.4079 ± 0.1665** — but against a data denominator of **2 tracks**; the
  fraction is noise there, the absolute yield is real. Next: idx 550 (S0_x0 h−, z[0.7,1.0),
  pT<0.25) 0.3399 ± 0.0760 (denom 8), idx 551 0.3363 ± 0.0491 (denom 19).
- Largest single-bin yield: idx 10 (S0_x0 h+, z[0.05,0.1), pT<0.25): 55 counts,
  7.478 ± 1.008, fraction 0.048%.
- Per-bin MC stat: `mc_sigma = sqrt(var_980)` per bin in CSV/NPZ; O(1–10) counts in the
  high-z bins → 15–40% relative MC-stat there.
- Corner question: high-z (z≥0.5) low-pT (<0.5) corner **reaches and far exceeds**
  0.2–0.5%: max 40.8%, mean 5.65% over the 56 corner bins — driven by tiny denominators.

## VM purity (variant a; b identical)

Tag: inlined GenPDG/GenMotherPDG on the matched ChargedTot row (ρ = |GenMotherPDG| ∈
{113, 213}); GEANT-secondary via IsStr==1 & IsGtr==0.

- **ρ⁰ decay pions: 2067/2102 = 98.33%** (1052 π⁺ + 1015 π⁻, mother 113; zero ρ±).
- Contamination 35/2102 = 1.66%: 15 GEANT-secondary pions (π←π), 5 muons from π decay,
  5 protons from π GEANT interactions, 10 unmatched fakes; 0 electrons, 0 kaons,
  0 ρ pions flagged GEANT-secondary.
- No proton-dissociative / beam-remnant tracks in the accepted spectrum (consistent with
  the elastic-only sample).
- `IsMatchedConvOrDalitz` is True on 2092/2102 clean ρ pions — unusable in v6 trees,
  diagnostic only, not used for classification.

## Field warning

`validation_summary.json`'s `integrated_fraction = 4.8699` is `Σ(yield_i/denom_i)` over
populated bins (raw-ratio sum), **not** the integrated contamination. The integrated
contamination is 285.78/3,865,073 = **7.394e-5 (0.0074%)**. See `FIELD_NOTES.md` in the
study directory.

## What remains before entering the systematics budget

1. **High-z policy decision** (subtraction vs masking vs systematic) — gates everything else.
2. **MC statistics:** 2253 mask tracks total; O(1–10) per high-z bin. If used as a
   subtraction template, merge z-rows/cells for the shape or add DIFFVM statistics.
3. **Proton-dissociative ρ:** not simulated in 8445–8448; if deemed relevant at high z,
   MC provenance/production needed (route via h1-mc-db / h1-mc-expert).
4. **Supersede the old scoping claim** in all background-fraction documentation.
5. **Response consistency:** this is a reco-space, pre-unfolding, no-C_rad vector; any
   subtraction must be applied in reco space before unfolding, consistent with the other
   background templates.
6. **Exactly-once ledger:** register this template so it is not double-counted against any
   future DIFFVM-based subtraction.

## Artifacts (absolute paths)

- Script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/backgrounds/diffvm_rho_reco980_2026-10-07/extract_diffvm_rho_reco980.py`
- Package: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/diffvm_rho_reco980_2026-10-07/`
  — `diffvm_rho_reco980_central_v6_selection.{npz,csv}`,
  `diffvm_rho_reco980_alpha0p2_veto.{npz,csv}`, `metadata.json`,
  `validation_summary.json`, `FIELD_NOTES.md`, `smoke/SMOKE_8445_0000.{json,npz}`,
  `smoke/SMOKE_8448_0001.{json,npz}`, `partials/shard_{0..7}.npz`
- Condor: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/condor_v6/diffvm_rho_reco980_2026-10-07/` (clusters 5385894 extract, 5385898 merge)
- Denominator (alpha0p2): `.../electron_cone_alpha0p2_z005_20260623/data_vector_alpha0p2_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json`

# Background-subtraction vs unfolding consistency ledger — high-Q2 charged-hadron SIDIS (v6/z005)

- **Ledger date:** 2026-10-07 (deliverable naming); assembled 2026-10-08.
- **Scope:** exactly-once accounting of track-level backgrounds in the v6/z005 high-Q2
  inclusive charged-hadron SIDIS chain, tested against artifacts. Ruling concerns
  **future background treatment only** — it does not change any installed release.
- **Release context (unchanged by this ledger):** raw parent
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`
  and Born release `born_crad_dj14xfcut_..._2026-10-07/` remain as installed.
- **Luminosity:** `L_data = 351000.0 nb^-1` everywhere in the products cited here
  (never 356613.0 or 297657.8).
- **Machine-readable sidecar:**
  `sidis_highQ2/data/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.json`.

---

## 1. Question under test

> "Is it the case that all track-level backgrounds are already subtracted at the stage
> of the unfolding? If so, we should not subtract 1-f_bg(track) but only 1-f_bg(event)."

Both halves were tested against artifacts; neither was accepted a priori.

## 2. Verdict

**No — the premise is false, and the proposed remedy is the wrong functional form.**

1. **Not all track-level backgrounds are subtracted at unfolding.** The unfolding
   fake-fraction subtraction (`f_i`) removes exactly ONE track-level class: the
   MC-unflagged reco fakes (reco tracks passing the full selection with no matched
   gen π/K/p in the pool). Integrated removal from the data vector:
   **10.7141% (h+)** and **10.4019% (h−)** (data-weighted; audit JSON).
2. **Residual electrons and V0 daughters are not subtracted anywhere in the central
   chain.** The central selection masks (`IsMatchedElectron==0`,
   `IsMatchedV0Daughter==0`) are MC-truth-link flags that are **identically zero in
   data** — verified in the builder code path AND empirically (Sec. 4). The central
   response excludes flagged electrons/V0 **entirely** (signal AND fake columns), so
   its fake column cannot remove data electron/V0 contamination. That contamination
   therefore survives the full chain and is **unfolded as if it were signal**.
   Magnitude (MC templates, cone-applied → understated): electrons **3.8355%**,
   V0 daughters **3.4002%** of the central data vector.
3. **Low-Q2 leakage and DIFFVM ρ are source-distinct** (not in signal MC, hence not in
   the response): **0.0054%** and **0.0074%** integrated. Neither is subtracted
   anywhere. DIFFVM is systematic-only by ruling (2026-10-07): per-bin subtraction
   forbidden.
4. **Detector-interaction (STR/GEANT) charged-hadron secondaries ARE covered by the
   response by design:** 86.18% are truth-matched and unfolded as physical signal
   (included in the observable under the central convention); 13.82% land in the fake
   column and are removed by `f_i`. The 86.18/13.82 magnitudes are measured under
the alpha0p2 cone and are cone-conditional; the matched/fake split itself and the
FORBIDDEN ruling are cone-independent (structural, central v4 builder).
**External det.int. subtraction is forbidden**:
   it would double-remove the 13.82% and wrongly remove the 86.18%.
5. **The remedy "only 1−f_bg(event)" is wrong for every surviving category.** The
   surviving contamination is track-level, per-bin, and kinematically correlated
   (electrons peak in high-z/high-pT sparse corners and at low z/low pT; V0 daughters
   peak at low z/low pT). An event-level multiplicative factor scales bins by event
   counts, not by the actual per-bin contamination. The low-Q2 category does have an
   event-level representation (276.5991634 events), but that is a **different object**
   from the track-level vector (208.3795 tracks) that any subtraction of the data
   vector requires.

**Correct future form (Sec. 8):** per-category, per-bin, track-level **additive**
vectors (electron, V0, low-Q2; electron∩V0 overlap once), subtracted from the data
vector **pre-unfold**, with variance `Var(d−B) = Var(d)+Var(B)` — **or** the
response-consistent alternative: all-fake response variant (e/V0/det.int. inside
`f_i`) + low-Q2 external only. No external det.int. subtraction under the central
response. DIFFVM stays a systematic envelope.

## 3. What the central fake-fraction subtraction actually removes

- **Fake column definition** (central builder
  `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`,
  lines 647–659, incl. comment 654–658): reco tracks passing
  `pass_reco_full = evt & trk & not_e & not_v0_match_top` with **no matched gen
  partner in the π/K/p pool**. Both fakes and matched signal draw from
  `pass_reco_full`.
- **`f_i` construction** (`crosscheck_unfold_methods_v4.py`, lines 321–450, 516–563;
  release driver `scripts/run_bbb_ibu_release_coherentprior.py`):
  `f_i = F_i/(Σ_j B_ij + F_i)`, `d_sig = data·(1−f_i)`, variance ×(1−f_i)².
- **Integrated removal (audit
  `sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/central_audit_numbers.json`):**
  h+ 1,975,301 → 1,763,665.127 (**10.7141%**); h− 1,908,796 → 1,710,244.719
  (**10.4019%**). MC-side fake fractions: 10.61% (h+) / 10.31% (h−); per-bin fake
  fraction median 10.10%, max 32.24% (h+). Audit gates: 0 negative subtracted bins,
  0 background-exceeds-data bins.
- **Composition:** because `pass_reco_full` already carries `not_e` and
  `not_v0_match_top`, the fake column contains **no MC-flagged electrons and no
  MC-flagged V0 daughters**. `f_i` therefore cannot remove data electron/V0
  contamination (Sec. 4–5).

## 4. The MC-only masks: central selection is a no-op on data

- **Code path:** `IsMatchedElectron`, `IsMatchedV0Daughter`, `IsMatchedHadron`,
  `IsMatched` are assigned only inside the MC-matched block of
  `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` (lines 1731–1771,
  inside `if (matched)` from H1PartMC). With no truth link there is no assignment.
  (Stage-1 `RejectTracksNearScatElec(0.2)` at line 394 of `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` affects hadronic calibration
  only, not tree admission.)
- **Empirical check (login-node, one file per invocation):**
  - `output_v6/central/sidistree/data/em0405/sidistree_em0405_d05_0000.root`,
    tree `ChargedTot`, 4541 entries: `IsMatchedElectron==1`: **0**;
    `IsMatchedV0Daughter==1`: **0**; `IsMatchedHadron==1`: **0**; `IsMatched==1`: **0**.
  - `output_v6/central/sidistree/data/ep0607/sidistree_ep0607_d06_0000.root`
    (second period), 3016 entries: all three flags **0**.
- **Counter cross-check:** central data vector `n_tracks_in_ps = 3,902,480` equals the
  alpha0p2 mask-off `alpha_pre_count = 3,902,480`; the alpha-cone (not the truth
  masks) removes 28,582 data tracks (0.733%); data
  `alpha_rejected_matched_electron_count = 0`.
- **Data-vector builder** (`build_data_vector_v4.py`, doc lines 52, 254–255) requires
  `IsMatchedElectron==0 & IsMatchedV0Daughter==0` — in data this requirement is
  vacuously satisfied by every track and removes nothing.

**Consequence (ESTABLISHED):** data electron and V0-daughter contamination enters the
central data vector, and the central response (which, in MC, never contains flagged
e/V0 reco tracks) maps it onto signal. It is not removed by selection, not contained
in the response's signal or fake columns, and not touched by `f_i`.

## 5. Response-convention dependence of background coverage

| Response convention | residual electrons (non-V0) | V0 daughters | det.int. GEANT 2ndaries | MC-unflagged fakes | low-Q2 | DIFFVM |
|---|---|---|---|---|---|---|
| **central v6 (release)** | excluded from response → **data uncovered** | excluded (F3/C2F2 fix) → **data uncovered** | 86.18% signal + 13.82% fake col (magnitudes alpha0p2-conditional; split structural) | fake col (`f_i`) | absent (source-distinct) | absent (source-distinct) |
| alpha0p2 (diagnostic, P-ALPHA open) | 99.92% fake col (cluster 3973257; 0.076% out-of-grid) | 100% excluded → uncovered | ~86/14 (measured under this selection, cluster 3971447) | fake col | absent | absent |
| all-fake (cluster 3987932; 2026-10-05 chain) | fake col | fake col | fake col | fake col | absent → external | absent → systematic |

Central builder evidence: `pass_reco_full = evt & trk & not_e & not_v0_match_top`
(line 659); V0-matched reco tracks are dropped entirely per chain-audit F3/C2F2
(lines 649–652). Alpha0p2 builder
(`studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py`):
no electron truth veto, V0 mask kept, cone α≥0.2 rad.

## 6. Per-category ledger

Classification labels: **(a)** removed by central selection veto; **(b)** removed by
unfolding fake-fraction subtraction; **(c)** partially removed; **(d)** not covered by
the response at all. Classification refers to **data-side contamination through the
central chain**. Fractions are vs the central data vector (raw, no bgsub;
Σ = 3,884,097; h+ 1,975,301 / h− 1,908,796).

| Category | Already handled by (central chain) | Classification | Residual treatment (future) | Integrated residual | Worst populated bin |
|---|---|---|---|---|---|
| MC-unflagged reco fakes | `f_i` fake-fraction subtraction at unfolding | (b) | none — already exactly once | 0% by construction (10.71%/10.40% removed) | per-bin `f_i` ≤ 32.2% |
| Residual electrons | nothing (mask no-op in data; excluded from response) | (d) | per-bin track-level vector pre-unfold, or all-fake response | **3.8355%** (cone-applied); **≈4.1441%** no-cone (PROV) | 26.95% (h+ b349, S2x0, z∈[0.7,1), pT∈[6,8), d=9); 15.67% at d≥100 (h− b350, z<0.05, pT<0.25) |
| V0 daughters | nothing (mask no-op; F3 drop from response) | (d) | per-bin track-level vector pre-unfold, or all-fake response | **3.4002%** (cone-applied) | 6.36% (h+ b0, S0x0, z<0.05, pT<0.25, d=118,226) |
| det.int. GEANT charged-hadron 2ndaries | response by design: 86.18% as signal, 13.82% via `f_i` (magnitudes alpha0p2-conditional) | (c) | **NONE** — external subtraction forbidden (double-removal + wrongful removal of the 86%) | 0% (0.7056% is in-observable by design) | 2.20% at d≥100 (h+ b420, S3x0, z<0.05, pT<0.25) |
| low-Q2 leakage | nothing (not in signal MC/response) | (d) | per-bin track-level vector pre-unfold | **0.0054%** | 15.33% (h+ b68, S0x0, z∈[0.7,1), pT∈[4,6), d=56 — near-empty); 0.41% at d≥100 |
| DIFFVM ρ | nothing (not in response) | (d) | **NO per-bin subtraction** (ruling 2026-10-07): systematic envelope only | **0.0074%** | 33.99% (h− b60, S0x0, z∈[0.7,1), pT<0.25, d=8 — noise-dominated); 3.65% at d≥100 |

Combined externally-removable set under the central response (e + V0 + low-Q2, simple
sum): **281,250.09 tracks = 7.2411%** of the central vector (h+ 7.1337%, h− 7.3521%);
for a union vector subtract the electron∩V0 scaled overlap **112.2093** once
(0.0753% of e, 0.0850% of V0, 0.0364% of the template total). All four templates
simple-summed: 308,655.8946 = 7.9467% of central (h+ 7.8796%, h− 8.0161%); the same
sum over the alpha0p2 denominator (3,865,073) is 0.0798577141, reproducing
`validation_report.json` exactly.

### Category evidence notes

- **Residual electrons.** Template mode = reco tracks truth-classified
  `IsMatchedElectron!=0` (the H1OO-tagged scattered-electron track is already excluded
  by the v6 builder, so this is the residual electron class: conversion/Dalitz plus
  any second electron candidates). Producer:
  `studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/scripts/produce_reco_background_vector_alpha0p2_v2.py`
  (truth vetoes disabled for e/V0 modes, lines 547–549, 721–728) under the alpha0p2
  cone selection. **Cone understatement is quantified:** the cone removes only
  215,731/2,954,136 = **7.3027%** (RAPGAP31) and 273,538/3,605,110 = **7.587%**
  (DJANGOH14) of matched electrons
  (`provenance/validation_alpha0p2_final_summary.json`, `.mc_alpha_counts`), so the
  central no-cone contamination is ≈ template ÷ 0.9241 (dj) / ÷ 0.9270 (rg)
  → **4.1441%** integrated (PROVISIONAL; e+V0 ≈ 7.5442%). Yield is dominated by a
  low-z/low-pT component (worst well-populated bins at z<0.05, pT<0.25), with
  scattered-electron-remnant tails in sparse high-z/high-pT corners; the composition
  split is not decomposed in existing artifacts (open item).
- **V0 daughters.** Template mode `IsMatchedV0Daughter!=0`, same producer/convention.
  Contamination is concentrated at low z/low pT (worst bins b0/b140, z<0.05,
  pT<0.25 at 5.4–6.4% on well-populated bins). No cone-survival counter exists for
  V0 daughters, so the no-cone correction is unbounded (PROVISIONAL).
- **det.int.** STR-only GEANT secondary charged π/K/p
  (`IsStr==1 && IsGtr==0 && IsMatchedHadron==1 && IsMatchedElectron==0 && IsMatchedV0Daughter==0`);
  not "pure material interactions". Partition
  `studies/.../diagnostics/detint_response_partition_20260626/detint_response_partition_summary.json`
  (cluster 3971447): 86.18% matched physical block / 13.82% fake column (fake reasons
  ≈92% gen-event Q2/y fail), measured under the alpha0p2 selection. Under the central
  convention the physical part is **inside the observable by design**; under the
  all-fake convention it is removed via `f_i`. Either is exactly-once-consistent;
  external subtraction on top of the central response is not.
- **low-Q2 leakage.** Template
  `AGGREGATED_djangoh14_lowq2_djbkgd_L351000_finalcandidate.npz` (DJANGOH-only
  low-Q2 bg sample): 208.3795 tracks integrated. Selection caveat: built with the
  alpha0p1 cone (`build_lowq2_djangoh_bg_hist_v6_alpha0p1_reco980.py`) — a selection
  mismatch vs the central no-cone selection. The event-level object
  (276.5991634 events) is a different quantity and must not be substituted for the
  track-level vector.
- **DIFFVM ρ.** `studies/diffvm_rho_reco980_2026-10-07/`, central-variant
  `diffvm_rho_reco980_central_v6_selection.npz`: 2,102 raw in-grid counts, yield
  285.7781 ± 6.2332 (scale 351000.0/2581730.0). Integrated 285.7781/3,884,097 =
  **0.00736%** (0.0074% vs the alpha0p2 denominator per FIELD_NOTES). Per-bin
  ratios up to 40.79% exist only on O(1–10)-track denominators (noise-dominated);
  worst z-row 2.18%, worst cell S0_x0 4.53%. Ruling 2026-10-07 (h1-analysis-critic):
  **systematic-only; per-bin subtraction forbidden.**

## 7. Exactly-once audit

**Today's central chain:** nothing is subtracted twice — `f_i` removes only the
MC-unflagged fake class and nothing else removes anything. The central chain's
background failure mode is **under-subtraction** (e, V0, low-Q2, DIFFVM survive),
not double-subtraction.

**Exactly-once constraints for any future treatment:**

1. **e/V0 external vectors + central response: CONSISTENT.** The fake column excludes
   flagged e/V0 by construction, so `f_i` and external e/V0 vectors touch disjoint
   classes (up to the additive-composition assumption, Sec. 9).
2. **det.int. external vector + central response: FORBIDDEN.** The 13.82% fake part
   is already inside `f_i` (double-removal) and the 86.18% physical part is signal by
   design (wrongful removal).
3. **Any external vector + all-fake response: FORBIDDEN for e/V0/det.int.** — they
   would already be inside `f_i`. With the all-fake response, only low-Q2 remains
   external and DIFFVM remains a systematic.
4. **Electron∩V0 overlap (112.2093 scaled)** must be removed once when combining the
   e and V0 vectors into a single subtraction vector.
5. **low-Q2** has no response content under any convention — external track-level
   treatment is the only option (the 2026-10-05 diagnostic chain precedent:
   `lowq2sub`).

## 8. Form of any justified residual subtraction

- **Rejected:** event-level multiplicative `1−f_bg(event)` for any category. The data
  vector is a track-level object; the surviving backgrounds are per-track, per-bin,
  and kinematically correlated. Event-level scaling would mis-scale every bin
  proportionally to event counts rather than to actual contamination. The low-Q2
  event count (276.5991634) is not the operative object.
- **Rejected:** a single global `1−f_bg(track)` multiplicative factor. Per-bin
  contamination ranges from ≲0.1% to ~34% (sparse corners); a global factor cannot
  represent it.
- **Exact form (Option A — central response retained):** per-category per-bin
  **additive** track-level vectors, subtracted from the data vector **before
  unfolding**:
  `d'_i = d_i − B_i(e) − B_i(V0) − B_i(lowQ2)` with the e∩V0 overlap taken once and
  `Var(d'_i) = Var(d_i) + Var(B_i)` (templates carry `var_980`). A per-bin
  multiplicative `1 − B_i/d_i` is equivalent at first order but is not the packaged
  convention; additive matches the template packaging (`yield_980`/`var_980`) and the
  established pre-unfold pattern (`f_i` and `lowq2sub` are both pre-unfold). Clamp
  `d'_i ≥ 0`; require 0 background-exceeds-data bins (as the `f_i` audit does).
- **Alternative (Option B — response-consistent, prior handoff recommendation):**
  all-fake response variant (cluster 3987932;
  `studies/alpha0p2_allfake_response_z005_20260627/`; exercised end-to-end by the
  diagnostic chain `studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/`):
  e/V0/det.int. are fake-column components removed by `f_i` automatically; low-Q2
  external; DIFFVM systematic-only. Then NO external per-category e/V0/det.int.
  vectors may be added.
- **Either option:** DIFFVM remains systematic-only. det.int. is never subtracted
  externally under the central response. Choosing between A and B changes the
  physical observable for det.int. (included vs removed) and is a convention
  decision for the supervisor/user, not settled by this ledger.

## 9. Assumptions that cannot be closed from existing artifacts

- `f_i` transfer to data assumes the data fake composition matches the MC-unflagged
  fake class (standard closure assumption; no data-side independent validation).
- Data e/V0 magnitudes are MC expectations (no truth in data); cone-selection transfer
  is approximate: electrons corrected by measured cone survival (7.30%/7.59%
  removed); **V0 and det.int. cone-survival counters do not exist** — their no-cone
  central magnitudes are unbounded from above by existing artifacts.
- The det.int. 86/14 partition was measured under the alpha0p2 selection; transfer to
  the central (no-cone) selection is approximate.
- The low-Q2 template selection (alpha0p1 cone) mismatches the central selection.
- Additive decomposition of data bins into signal + MC-like fakes + e + V0 + low-Q2
  (+DIFFVM) with no residual interference is assumed by any subtraction scheme.
- Electron-template composition (scattered-electron remnants vs conversion/Dalitz)
  is not decomposed; the dominant low-z/low-pT electron yield is uninterpreted.

## 10. ESTABLISHED vs PROVISIONAL register

**ESTABLISHED (artifact-backed):**
1. `IsMatchedElectron`/`IsMatchedV0Daughter`/`IsMatchedHadron`/`IsMatched` are MC-only
   and ≡ 0 in data (code path + two-period empirical check).
2. The central data vector retains data e/V0 contamination (mask no-op; counter
   equality 3,902,480).
3. The central response excludes flagged e/V0 entirely; its fake column contains only
   MC-unflagged unmatched reco tracks (builder lines 647–659).
4. `f_i` removes 10.7141% (h+) / 10.4019% (h−) of the data vector — the MC-unflagged
   fake class only.
5. det.int. is in-response with an 86.18/13.82 matched/fake split (alpha0p2
   measurement); external det.int. subtraction on the central response violates
   exactly-once.
6. low-Q2 and DIFFVM are source-distinct and absent from every response variant.
7. DIFFVM is systematic-only (0.0074% integrated; per-bin subtraction forbidden).
8. Template totals and integrated fractions vs both denominators reproduce exactly
   (308,655.8946; 7.9467% of central; 0.0798577141 of alpha0p2).
9. Event-level low-Q2 (276.5991634 events) ≠ track-level low-Q2 (208.3795 tracks).
10. L_data = 351000.0 nb^-1 throughout the cited products.

**PROVISIONAL (needs a central-selection rerun or new counters to close):**
1. No-cone (central-selection) electron magnitude ≈ 4.1441% (counter-based rescale).
2. No-cone V0 and det.int. magnitudes (no survival counters; unbounded).
3. low-Q2 magnitude under the central selection (alpha0p1-cone template).
4. All alpha0p2-derived per-bin magnitudes are diagnostic-grade (P-ALPHA: alpha0p2
   response-consistency unvalidated).
5. Any future subtraction vector's data-side effect (depends on Option A/B choice and
   on assumptions in Sec. 9).

## 11. Open decisions and blockers

1. **`background_rel` RETIREMENT REQUIRES RE-EXAMINATION** (STATE.md line 31): the
   h− `bin_index 34` value is corrected to `0.5·B/Y = 0.0055214` (0.55214%), **not**
   a 552% sparse-yield excursion; the retirement was partly justified by the void
   552% magnitude. Do not silently retain or reverse. The weight-43.1292 event
   remains an untraced outlier. This ledger's quantified residuals (Sec. 6) are the
   replacement decision input.
2. **Track-level background systematic blocked:** `overlap_980` and `n_raw_980` are
   missing from the alpha0p2 e/V0/det.int. aggregates (pre-existing blocker).
3. **P-ALPHA:** alpha0p2 response-consistency unvalidated; alpha0p2 products remain
   diagnostic-only.
4. **No central-selection (no-cone) background template production exists**; closing
   Sec. 10 PROVISIONAL items 1–3 requires one (Condor production, not attempted in
   this read-only study).
5. `sections/systematics.tex` still carries the stale 552%/"genuine sparse-yield"
   text (analysis-note follow-up, not edited here).
6. The 2026-10-05 all-fake chain's born tables carry blind C_rad needing S3 re-attach
   (unrelated pending item, recorded for context).

## 12. Provenance and reproduction

**Primary artifacts (all read-only):**
- Central data vector:
  `H1ChargedSidis_HighQ2/output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/data_vector_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json`
  (Σ 3,884,097; h+ 1,975,301 / h− 1,908,796; charge-major 0–489 h+, 490–979 h−).
- Central response builder + data-vector builder (same directory,
  `scripts/unfolding/tunfold_official_v4_finalcandidate_.../`).
- Tree builder: `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`
  (lines 1731–1771); the `RejectTracksNearScatElec(0.2)` call is at line 394 of
  `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` (hadronic calibration only).
- `f_i`: `scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`
  (lines 321–450, 516–563); release driver
  `output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/scripts/run_bbb_ibu_release_coherentprior.py`.
- Audit: `sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/central_audit_numbers.json`.
- Templates + validation:
  `H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/`
  (npz per generator/category; `validation_report.json`).
- Partitions: `.../diagnostics/detint_response_partition_20260626/detint_response_partition_summary.json`
  (cluster 3971447); `.../diagnostics/electron_v0_response_partition_20260627/electron_v0_response_partition_summary.json`
  (cluster 3973257).
- MC cone counters: `studies/electron_cone_alpha0p2_z005_20260623/provenance/validation_alpha0p2_final_summary.json`
  (`.mc_alpha_counts`).
- Alpha0p2 data vector (denominator cross-check, Σ 3,865,073):
  `studies/electron_cone_alpha0p2_z005_20260623/data_vector_alpha0p2_..._percharge.json`.
- DIFFVM: `studies/diffvm_rho_reco980_2026-10-07/` (npz both variants,
  `validation_summary.json`, `FIELD_NOTES.md`).
- All-fake alternative: `studies/alpha0p2_allfake_response_z005_20260627/` (cluster
  3987932); `studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/`.
- Skill (authoritative background facts): `sidis_highQ2/.opencode/skills/sidis-hq2-backgrounds/SKILL.md`.
- State: `sidis_highQ2/STATE.md` (line 31 for `background_rel`).

**Reproduction (login-node, single file per check):**
- ROOT flag check: open one `sidistree_*.root`, tree `ChargedTot`,
  `GetEntries("IsMatchedElectron==1")` etc. → 0 (both periods checked).
- Fraction computation: load central `per_bin.n_subtracted` (980, charge-major) and
  each template `yield_980`; ratios per bin; integrated = ΣB/Σd. Bin decode:
  490 = 7 cells (S0x0,S0x1,S1x0,S1x1,S2x0,S2x1,S3x0) × 7 z × 10 pT;
  `bin_index = cell*70 + z_idx*10 + pT_idx`; z edges
  [0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]; pT edges
  [0,0.25,0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0,8.0] (central binmap v3).

**Sidecar:** `sidis_highQ2/data/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.json`
(contains the full numbers block, per-category records, worst-bin lists with decoded
coordinates, flags, and provenance paths).

---

## S2' supersession note (2026-10-10 promotion; appended 2026-10-11)

APPEND-ONLY note — the ledger body above stays verbatim and records the PRE-S2'
realization of the chain. Since the 2026-10-10 S2' detint-fake promotion (user
decision 2026-10-10; promotion record
`docs/promotions/detints2prime_promotion_2026-10-10.md` §1; `STATE.md`
"2026-10-10 S2' detint-fake Promotion Override"):

- The canonical central release is
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`.
- The det.int. Class 2 removal is now RESPONSE-INTERNAL via the enlarged per-reco-bin
  `f_i` (integrated RG `f_i` `11.2403%` (h+) / `10.8737%` (h−); data-weighted
  convention `11.3577%` / `10.9768%`), so the fake-fraction figures above
  (`10.7141%`/`10.4019%`) are PRE-S2'-BASIS values; quoting them against S2' values
  without the observable-basis flag is FORBIDDEN.
- The exactly-once structure still holds under S2' — the central response still
  applies ONLY response fake-fraction subtraction, now the per-reco-bin variant
  `f_i` (which additionally removes the det.int. removable Class 2), and the
  charged-K DIF k_parented keeper is kept as signal.
- External det.int. subtraction is FORBIDDEN, more strongly than before: under S2'
  it would DOUBLE-REMOVE the removable class AND violate the keeper (spec Sec 3).
- The e/V0/low-Q2 residual statements, the Option-A/Option-B discussion, and the
  DIFFVM systematic-only ruling are UNCHANGED by this promotion (Option-A external
  e/V0/low-Q2 vectors remain unapplied to the central chain).

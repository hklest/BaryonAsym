# Background-subtraction scheme — S2′ re-base addendum (2026-10-11) — Rev 3 (cone-base re-derivation) + §H production-preconditions extension (2026-10-11; JSON v4)

**Status:** ADDENDUM to
`docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md` (and its
machine twin `bgsub_scheme_spec_2026-10-10.json`). ADJUDICATION + SPEC
RE-DERIVATION only: no products were built, no releases were promoted, no
runs were launched; every release, budget, table, study tree, and the
cone-base campaign products were treated READ-ONLY. Implementation and
production follow in later dispatches.

**Rev 3 (2026-10-11, cone-base re-derivation):** the user has DECIDED to
impose the cone α≥0.2 (3D opening angle to the reconstructed scattered
electron; `not_e` kept) on the central track selection, and the cone-base
chain is complete and validated (RAW + Born releases, cone S2′ responses,
cone coherent prior, cone data vector, f_i crosschecks, QA counters;
`output_v6/studies/cone_alpha_central_build_2026-10-11/`). This revision
re-derives every Set R′ anchor, gate item, and budget expectation of Rev 2
**on the cone base** (§0-bis–§F). The decisive consequence: the packaged
cone-applied templates are now **selection-consistent with the chain by
construction** — the Rev-2 electron cone-survival rescale is RETIRED and the
V0/lowQ2 cone-survival envelope is CLOSED (M-2 closes by construction;
P-ALPHA discharges; §0-bis.3). The subtraction set itself is UNCHANGED:
**Set R′ = e + V0 + lowQ2** (the Rev-2 F-1 correction stands). Rev-2
statements that were load-bearing for the superseded selection-transfer
treatment are retained below as SUPERSEDED blockquotes.

**Rev 2 (2026-10-11, same-day correction — STANDS):** corrected Rev 1 per the
critic gate `docs/critiques/critic-gate-bgsub-promotion-2026-10-11.md`
(**FATAL F-1**): MC-flagged electrons are excluded from the response entirely
(builder predicate `pass_reco_full = evt & trk & not_e & not_v0_match_top`),
so the subtraction set is e + V0 + lowQ2, not V0 + lowQ2. The full Rev-2
correction record is §0; its set-composition, disjointness, exactly-once,
and budget-structure content carries into Rev 3 unchanged in substance; its
selection-transfer items (§0.2 rescale, §0.3 envelope) are superseded by the
Rev-3 cone closure.

**Companion machine-readable spec:** `docs/systematics/bgsub_s2prime_rebase_spec_2026-10-11.json`
(schema `bgsub_s2prime_rebase_spec_2026-10-11_v4`; the §H extension is the
`production_preconditions_h` block)

**§H extension (2026-10-11, same-day; JSON v4):** the Stage-1 production
dispatch stopped at precondition verification
(`output_v6/studies/bgsub_setr_cone_build_2026-10-11/BLOCKER_RECORD_2026-10-11.md`):
the cone campaign built the core chain (data vector, central/S2′ responses,
coherent prior, f_i, IBU, Born) but NONE of the 15-comp systematics inputs,
and Rev 3 §G named the cone 15-comp budget a precondition without prescribing
its production. §H (below) supplies the complete per-component inventory, the
three missing production recipes (5 cone shifted detector data vectors; a
cone port of the tracking-drop builder + 16 seed data vectors; the cone
1000-toy sidecar), the 31-item gate/machinery re-scoping with every pinned
constant, and the ordered re-dispatch plan. **No Rev-3 anchor, window, or
budget expectation changes** (§H.0).

**Decision records (user):**
- **2026-10-11 (re-base):** the published background-subtracted chain is
  RE-BASED onto the S2′ detint-fake basis; the pre-S2′ bgsub candidate
  (`born_crad_bgsub_setR_centralbase_..._2026-10-10`, official gated values
  h+ 12.62353085 / h− 12.30219489 nb) is documented provenance only and will
  NOT be promoted.
- **2026-10-11 (cone basis):** the cone α≥0.2 is IMPOSED on the central
  selection (Grebenyuk-precedent alignment; impact study
  `docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md` adjudicated the
  background-reduction case NOT supported and the consistency case real; the
  user decision adopts the cone as a selection-convention change). The
  re-base target moves to the cone-base products. Comparison of the two
  bases: `docs/results/cone-vs-nocone-comparison-2026-10-11.md` (integrated
  −0.297%/−0.313% RAW IBU, −0.289%/−0.306% Born; all impact-study
  predictions confirmed).

**Re-base target (cone base; study-tree R9 PROVISIONAL until promotion):**
RAW `output_v6/studies/cone_alpha_central_build_2026-10-11/production/release_driver/bbb_ibu_cone_alpha_central_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-11/`
(integrated IBU h+ 11.412233 / h− 11.123626 nb); Born
`born_crad_dj14relbin_cone_alpha_central_..._2026-10-11/` (h+ 13.026987 /
h− 12.697511 nb; per-bin C_rad byte-identical to canonical, table sha256
`205e8218…`; 17/17 validation gates green). Cone data vector
`production/data_vector_cone_alpha_central_2026-10-11.json`: in-grid
**3,865,073** (h+ 1,965,700 / h− 1,899,373). Cone S2′ variant responses
`production/migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.{root,npz}`.
Cone 15-comp budget: **DOES NOT EXIST YET — precondition (§G)**. S2′
convention unchanged: det.int. Class 2 removed response-internally via the
enlarged per-reco-bin `f_i` (removable fraction 0.909920 ± 0.000269;
cone-variant response-internal RG integrated h+ 11.2335% / h− 10.8698%;
data-weighted on D_cone 11.3344% / 10.9602%); charged-K DIF k_parented keeper
KEPT as signal; external det.int. subtraction FORBIDDEN.

> **SUPERSEDED (Rev 2 re-base target):** the no-cone S2′ releases
> (`bbb_ibu_detints2prime_..._2026-10-09`, `born_crad_dj14relbin_detints2prime_..._2026-10-10`)
> and the no-cone 15-comp budget remain canonical-installed releases, but
> they are NO LONGER the re-base target after the cone decision; Rev-2
> anchors computed against them (§D of Rev 2) are superseded by the cone
> anchors below.

---

## 0. Rev-2 correction record (2026-10-11) — critic gate F-1 (STANDS)

**Trigger:** critic gate
`docs/critiques/critic-gate-bgsub-promotion-2026-10-11.md`, Fatal F-1
(PM-4 resolved DO-NOT-PROMOTE as-is), with Major M-1/M-2/M-3 and Conditions
1–5. **F-1 evidence (three-way corroborated):** (i) the builder predicate
`pass_reco_full = evt & trk & not_e & not_v0_match_top` excludes MC-flagged
electrons from the response ENTIRELY — signal AND fake columns — under the
central AND the variant response; (ii) the 2026-10-07 ledger §6 rules the
residual electrons "under-subtracted; external vector YES"; (iii) the
2026-10-10 normalization verification (caveat 1) quantifies them at 3.8355%
cone-applied (of the old no-cone D) / ≈4.1441% no-cone PROVISIONAL, not
covered by `f_i`.

**Corrections of Rev 2 that CARRY into Rev 3 unchanged:**

1. **Set composition (F-1 cure):** **Set R′ = e + V0 + lowQ2**,
   `B_R′ = B(e) + B(V0) + B(lowQ2)` (simple sum; e∩V0 overlap IGNORED per the
   standing user decision 2026-10-08). Exactly-once safety: fc1 (electron)
   and fc2 (V0) are identically zero in the variant response — re-verified on
   the CONE variant (§A).
2. **e∩V0 overlap (user 2026-10-08) — CARRIES:** an ACTIVE double-subtraction
   of **112.2093 cone-applied tracks = 0.00290% of D_cone** (≤0.042% of
   B_R′) — inside the accepted "max 0.0364% of the template total" bound.
   Decision unchanged: IGNORED. (The Rev-2 rescaled copy ≈121.2354 retires
   with the rescale.)
3. **Anchors recomputed exactly** — now on the cone base (§D).
4. **Budget: 19 components STAND**; `bgsub_fakefrac_rel` scope stays fc0-only
   (§E).
5. **Gate:** 25 items in Rev 2; Rev 3 re-derives the anchors and adds the
   cone-parity item IV-7 → **26 items** (§F).
6. **Kept (critic-confirmed sound):** disjointness evidence for the
   V0/lowQ2/keeper/det.int. rows (G1/G9/G11, parentage campaign
   `n_v0_ancestor_in_detint = 0`); template validity without re-derivation;
   the 19-component budget structure; the gate mechanics.

**Superseded Rev-2 items (replaced by the Rev-3 cone closure):**

> **SUPERSEDED (Rev 2 §0.2):** "Electron selection transfer — ADOPTED:
> per-generator cone-survival rescale" `B(e) = 0.5·(RG31_e/s_RG + DJ14_e/s_DJ)`
> with `s_RG = 2,738,405/2,954,136`, `s_DJ = 3,331,572/3,605,110`;
> rescaled totals 160,959.57 = 4.1441% of D (PROVISIONAL). On the cone base
> the packaged cone-applied template is the CORRECT same-selection object;
> the rescale is RETIRED (register CLOSED, §0-bis.3). The rescaled 4.1441%
> PROVISIONAL figure is no longer consumed anywhere.

> **SUPERSEDED (Rev 2 §0.3):** "V0/lowQ2 cone-survival gap — CARRIED as an
> explicit one-sided envelope inside `bgsub_model_rel`" (8.0442% relative;
> integrated ≈0.296%/0.297% of data_sub′). On the cone base the V0 gap
> closes by construction; the envelope term is REMOVED from
> `bgsub_model_rel` (§E). Only the lowQ2 α0p1 radius mismatch remains, as an
> explicit tiny caveat (§0-bis.3c).

---

## 0-bis. Rev-3 cone-base re-derivation record (2026-10-11)

**Trigger:** user decision 2026-10-11 (cone imposed; cone-base chain
complete). All cone-base products validated by the campaign
(`cone_alpha_central_build_2026-10-11`: production log, Born log, Phase-F
rerun guard record `qa_tools/check_phase_f_rerun_outputs.json` with E values
exact 697/700/777/845, guard adjudication
`docs/qa/cone-missrow-guard-adjudication-2026-10-11.md`, f_i crosschecks,
per-class alpha-QA counters 174/174 shards).

**Re-derivations applied (this revision, all read-only computations):**

1. **Data vector:** the chain consumes the CONE data vector D_cone
   (in-grid 3,865,073; in-PS parity `alpha_pre = 3,902,480` exact; cone
   rejections 28,582 in-PS = 14,465/14,117, 19,024 in-grid = 9,601/9,423).
   **Identity verified read-only:** the cone data vector is per-bin
   BIT-IDENTICAL to the June alpha0p2 campaign vector (max deviation 0.0,
   both charges, all 980 bins) — two independent builds of the same
   selection on the same 658 trees.
2. **Subtraction vector:** `B_R′_cone = e_cone (as packaged) + V0_genavg +
   lowQ2` — the same packaged alpha0p2-bank templates, now consumed WITHOUT
   any rescale: totals h+ 140,912.91681341422 / h− 140,337.17473977088 /
   281,250.09155318514 = **7.2767% of D_cone** (impact-study prediction
   7.2766% — confirmed to 0.0001 pp).
3. **PROVISIONAL registers CLOSE (M-2 closes by construction):**
   - (a) **Electron cone-survival rescale: CLOSED/RETIRED.** The template's
     selection (alpha0p2 = central + cone) is now the chain's selection;
     the cone-applied magnitude (148,976.4352 = 3.8543% of D_cone
     integrated; h+ 3.7494% / h− 3.9632% per charge) is used DIRECTLY.
   - (b) **V0 cone-survival envelope: CLOSED.** Same-selection argument;
     no counter is needed. The unbounded no-cone V0 caveat (ledger §9)
     closes with it.
   - (c) **lowQ2: NARROWED, not closed.** The lowQ2 template was built with
     the α<0.1 cone (radius mismatch vs the α≥0.2 selection; overcount
     direction). Bound: the α0p1→α0p2 full-chain IBU ratio mean 0.99972
     says the 0.1→0.2 rad widening is worth ≈0.03% chain-level, and the
     class is 0.0054% of D_cone → class-level effect ≪0.01%, well inside the
     lowQ2 ±30% model term. Carried explicitly (§E); optional cheap rebuild
     (single low-Q2 sample) recorded (§G).
   - (d) **P-ALPHA: both critic conditions DISCHARGE by construction**
     ((i) same-selection templates — condition (i) was "rescale or
     central-selection templates"; (ii) V0/lowQ2 gap closed or carried —
     V0 closed, lowQ2 carried as the tiny explicit (c)). The
     "alpha0p2-derived magnitudes are diagnostic-grade" caveat FALLS for
     e/V0. **A formal closure/supersession entry in STATE.md is REQUIRED**
     (recorded action, §G — the baseline-update procedure owns STATE.md).
4. **Disjointness re-verified on the CONE response** (§A): the cone changes
   the reco space (response-eligible MC rejections 162,528 = 83,782/78,746
   by class: matched_signal 77,974/74,586, detint 1,121/652, unflagged_fake
   4,687/3,508 — NO electron or V0 class appears, because `not_e`/`not_v0`
   are kept) but NOT the class structure: fc1/fc2 remain ≡ 0, the category
   partition closes exactly (2.4e-16/3.8e-16 of max F), cone f_i ∈ [0,1).
5. **All anchors, gate windows, and budget expectations recomputed on
   D_cone / the cone releases** (§C–§F). New precondition: the cone 15-comp
   budget package (§G).

---

## A. Disjointness adjudication on the S2′+SetR′ CONE chain (re-verified)

Question: the cone removed reco tracks from the response (and from the data
vector) — does the class structure of the exactly-once ledger still hold on
the cone response?

**Verdict: DISJOINT — UNCHANGED. The cone changes the reco space, not the
class taxonomy: fc1 (electron) and fc2 (V0) remain identically zero on the
cone variant response; the enlarged `f_i` still removes ONLY det.int. Class 2
non-keeper content; zero overlap with Set R′.**

### A.1 Evidence (read-only; cone-specific items new in Rev 3, rest carried)

1. **Cone variant category exports contain NO electron/V0 fake category**
   (`migration_s2prime_cone_rapgap31_2026-10-11.npz` keys): only
   `h_fake_existing_other_*` (fc0), `h_fake_detint_{rerouted,prefail,keeper_prefail}_*`
   (fc3 parts), `h_keeper_retained_body_*`, and the marginal/prior vectors.
   The builder predicate `pass_reco_full = evt & trk & not_e & not_v0_match_top
   (& alpha>=0.2)` excludes flagged e/V0 from the cone response exactly as
   from the no-cone response.
2. **Partition closure on the cone variant (re-verified exactly):**
   `fc0 + fc3_rerouted + fc3_prefail + keeper_prefail − F_fake` =
   2.448e-16 (h+) / 3.839e-16 (h−) of max F — the fake column is EXACTLY the
   four categories; nothing else entered under the cone.
3. **Cone f_i sanity:** per-bin f_i ∈ [0.000000, 0.324906] (h+) /
   [0.000000, 0.305182] (h−) — inside [0,1) for all 490 fine bins per charge.
4. **S2′ reroute conservation on the cone** (campaign crosscheck
   `s2prime_vs_central_cone`): per-row denominator `B+F` conserved to max
   rel 4.9e-16 — the reroute design (G9 identity) holds on the cone response.
5. **Per-class cone rejection counters (alpha-QA, 174/174 shards):** the
   response-eligible rejections are EXCLUSIVELY matched_signal (77,974/74,586),
   detint (1,121/652), and unflagged_fake (4,687/3,508); the matched-ELECTRON
   (244,729/244,540) and matched-V0 (4,269/3,729) in-cone counters are
   DIAGNOSTIC counts of content ALREADY EXCLUDED by the vetoes (they never
   were response content). The data vector's matched-electron
   rejection/survival counters are 0/0 (`not_e` kept).
6. **Carried (unchanged by the cone):** the e/V0 vetoes INSIDE `is_detint`
   (spec Rev 2 Sec 2.1); G1/G9/G11 gate-suite results (the reroute moves only
   det.int.-tagged non-keeper matched legs; the miss row bit-identical); the
   parentage campaign (`n_v0_ancestor_in_detint = 0` campaign-wide; keeper =
   100% charged-K mothers ±321); binmap/binning identity (NB_R = 490,
   NB_G = 245 per charge, UNCHANGED under the cone).

### A.2 Exactly-once class taxonomy on the S2′+SetR′ CONE chain

| Class | Definition | Handling on cone-S2′+SetR′ | Exactly once |
|---|---|---|---|
| Unflagged / other / unmatched reco fakes | response fake column (fc0) | removed by the cone variant `f_i` (fc0 is 93.45%/94.00% of the cone variant fake column, response-internal; the cone removed 4,687/3,508 unflagged-fake MC legs — content change, not class change) | YES — `f_i` only |
| Residual electrons | `IsMatchedElectron!=0`; absent from the response entirely (fc1 ≡ 0 on the cone variant too) | subtracted pre-unfold by the Set R′ electron vector **as packaged (cone-applied, no rescale)**; NOT removed by `f_i` | YES — Set R′ only |
| det.int. Class 2 removable (0.909920 ± 0.000269) | `is_detint` label | removed RESPONSE-INTERNALLY (rerouted matched non-keeper + pre-existing gen-fail non-keeper); cone rejections 1,121/652 legs are inside this class and stay response-internal | YES — variant `f_i` only; external det.int. subtraction FORBIDDEN |
| charged-K DIF k_parented keeper (9.01% of the block) | `is_detint & GenMotherPDG==±321` | SIGNAL (keeper legs in the body; keeper gen-fail in the fake column like ordinary signal gen-fail) | YES — signal; subtracted by nothing |
| V0 daughters | `IsMatchedV0Daughter!=0` | absent from the response (fc2 ≡ 0 on the cone variant); subtracted pre-unfold by Set R′ (packaged cone-applied template, selection-consistent) | YES — Set R′ only |
| low-Q2 leakage | DjBkgd source, source-distinct | absent from the response; subtracted pre-unfold by Set R′ (α0p1 radius caveat carried, §0-bis.3c) | YES — Set R′ only |
| DIFFVM ρ | exclusive rho, source-distinct | bounded uncovered residual (0.0074% integrated); systematic-only, never subtracted | not subtracted (register rows 18/19 BLOCKED, unchanged) |

**Ordering (unchanged):** `data_sub′ = data_cone − B_R′_cone` (980-bin
pre-unfold) → `d_sig = data_sub′·(1 − f_i_cone_variant)` → IBU k=4 on the
cone variant response with the cone coherent prior → `× C_rad` (Born). All
Set R′ classes remain absent from the response's reco content; the e∩V0
overlap (112.2093 cone-applied = 0.00290% of D_cone) remains IGNORED per the
standing user decision 2026-10-08.

## B. Template validity on the cone base

**Verdict: VALID — and now EXACTLY SELECTION-CONSISTENT (no transfer
correction of any kind is consumed for e/V0).**

1. **Selection consistency is EXACT, not approximate:** the packaged
   templates were derived under the alpha0p2 selection (central + cone
   α≥0.2) on the fixed 980-bin grid; the cone-base chain now applies exactly
   that selection. Read-only verification: the cone data vector is per-bin
   bit-identical to the June alpha0p2 campaign vector (max dev 0.0, both
   charges) — the template-derivation data denominator and the chain data
   denominator are the SAME vector. The f_res′ denominators are therefore
   the derivation-consistent denominators.
2. **The templates never touched a response** (unchanged argument):
   data-side reco-space expected-track vectors from the alpha0p2 bank
   (L351,000 scaling); S2′-independent and cone-independent inputs.
3. **Class definitions untouched** (unchanged argument): the reroute is
   V0-vetoed and e-vetoed inside `is_detint`; the parentage campaign
   confirms zero V0-heritage rows in det.int.; lowQ2 is source-distinct.
4. **What shifted relative to Rev 2:** the electron template lost its
   PROVISIONAL rescale (consumed as packaged); the V0/lowQ2 envelope term
   left `bgsub_model_rel`; the per-bin f_res′ denominators are D_cone (the
   fine-empty bin set is UNCHANGED: 6 h+/8 h− empty fine bins, same bins
   with template content as the no-cone vector — the cone removed no fine
   bin's full content); the yield-weighted integrated f_res′ is recomputed
   on the cone release values (§D).
5. **P-ALPHA:** discharged by construction (§0-bis.3d); the STATE.md formal
   closure entry is the remaining action (§G).

## C. Spec deltas — the re-based published chain (cone base)

The scheme structure is retained; the basis changes as follows:

1. **Subtraction stage (pre-unfold, 980-bin): CHANGED — the vector is the
   packaged extended Set R′ on the cone data vector:**
   `B_R′_cone = e_cone + V0_genavg + lowQ2` (all as packaged; simple sum;
   e∩V0 ignored per user 2026-10-08). Vector totals:
   h+ 140,912.91681341422 / h− 140,337.17473977088 / 281,250.09155318514 =
   **7.2767% of D_cone** (per class of D_cone: e 3.7494%/3.9632% —
   integrated 3.8543%; V0 3.4131%/3.4208%; lowQ2 0.00608%/0.00468%).
   Guards UNCHANGED (the carried dual convention, verified read-only on the
   cone base): fine-level empty-bin guard for the 980-level subtraction
   (`D_cone[i]==0 → no subtraction`), release-level empty-bin guard for the
   490-level dilution vectors (`D4==0 → f=0`). Fine-empty bins with template
   content (guarded): h+ 2 (bins 218, 358; content 0.1915, 0.1463), h− 4
   (bins 341, 410, 470, 480; content 0.0167–0.0527); total guarded-away
   **0.4663** tracks (electron part ≈0.218 + V0/lowQ2 part ≈0.248 — the
   latter bit-identical to the carried build's content in the same bins).
   Release-empty bins (f=0): h+ 109/179, h− 240 — unchanged.
   **TRUE negatives (D>0 & B>D): 0 (verified read-only)** — the SA-1
   expectation holds. E1b exactly-once Poisson variance:
   `var_sub′ = data_sub′ + varB_R′_cone` with
   `varB_R′_cone = 0.25·(varRG_e + varDJ_e) + 0.25·(varRG_V0 + varDJ_V0) +
   var_lowQ2` (NO rescale factors); toy/released statistical input
   Poisson-only `w2 = data_sub′`; double-Poisson = STOP.
   data_sub′ sums: h+ 1,824,787.4209930277 / h− 1,759,035.953778433.
2. **Fake fraction:** CHANGED — the per-reco-bin **cone S2′ variant** `f_i`
   table, derived from the pinned cone variant response
   (`migration_s2prime_cone_rapgap31_2026-10-11.root`, sha256
   `fda5466f6ef16c2cec22a6b8dc08113929a73a766a6bee4ee9271a362f77d6c6`) by
   the byte-identical derivation used for the cone RAW release. Integrated
   on-D_cone: h+ 11.3344% / h− 10.9602% (down from the no-cone variant's
   11.3577%/10.9768% — the measured cone f_i shift, comparison report §4).
   Integrated removal on `data_sub′`: expected **h+ 11.3063% / h− 10.9326%**
   (window §F, RU-2). Recomputing `f_i` or falling back to the no-cone table
   is a STOP condition.
3. **Re-unfold:** CHANGED — RAPGAP31 **cone S2′ variant** response (sha256
   above), coherent `body+miss` prior computed **FROM THE CONE VARIANT
   RESPONSE** (the campaign's Phase-D prior; R10-style repointed driver with
   the prior-provenance assertion; NO code path may read a no-cone response
   or prior), IBU k=4, `L = 351,000 nb⁻¹`, identical 980→490 release
   mapping. The DJANGOH model leg uses the cone DJ variant response
   (`migration_s2prime_cone_djangoh14_2026-10-11.root`, sha256
   `c84a8c2df4d2943d5d5626a7b267bdfe90736b831ef62de1734cb3816f0c48b5`)
   with the stored plain prior (G4/G6 stored-prior caveat carries,
   report-only). The repro target is the **cone RAW release** (integrated
   11.412233 / 11.123626 nb), not the no-cone S2′ release.
4. **C_rad:** UNCHANGED — the applied releasebin DJANGOH14 table
   (`data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`,
   integrated h+ 1.1415172920054786 / h− 1.1410367377289135), bitwise
   `C_rad × raw` identity; the cone is reco-only and does not enter gen-only
   C_rad (standing ruling). The cone Born release used the byte-identical
   table (sha256 `205e8218…`; per-bin Born shift == per-bin raw shift to
   2.2e-14; 17/17 gates green) — the same table is consumed here.
5. **Product:** a new dated study tree (e.g.
   `output_v6/studies/bgsub_setrprime_conebase_<date>/`) and a dated
   candidate release
   `born_crad_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<date>/`
   with exact-text serialization, invariants README, and extended
   validation.json. The Rev-2 candidate name
   (`..._detints2primebase_...`) is superseded for new candidates (it
   denotes the no-cone basis); the `setR` name remains RETIRED.
   Statistical lineage: 1000-toy Poisson through the cone variant
   `f_i`/response/prior.
6. **Estimator (per-bin, formal):**

```
sigma_bgsub[b] = (1/L) * IBU_k4( data_cone − B_R′_cone ,
                                 response RG31_S2prime_cone_variant,
                                 prior body+miss (FROM CONE VARIANT RESPONSE),
                                 fake fraction f_i_S2prime_cone )
                         [b] * C_rad_releasebin[b]
```

## D. Expected integrated anchors (cone base, extended set, computed read-only)

Method: per-bin dilution `Σ_b x_b·(1−f_res′,b)` on the **cone** release
values; `f_res′,b` = per-release-bin B_R′_cone/D_cone (unguarded template
pair sum; release-level empty-bin convention). **Method continuity:** the
Rev-2 method reproduced the carried build artifacts exactly (max dev 0.0);
the same machinery now runs on D_cone and the cone releases. The cone data
vector per-bin == the June alpha0p2 vector (max dev 0.0) and the packaged
V0/lowQ2 CSV columns == the bank vectors (max dev 0.0) were re-verified in
this pass.

| Quantity | h+ | h− |
|---|---|---|
| Cone RAW integrated IBU (unsubtracted, nb) | 11.412233 | 11.123626 |
| Integrated f_res′ on D_cone | 7.1686% | 7.3886% |
| Extended-set f_res′, RAW-yield-weighted | 7.9001% | 8.1267% |
| **Expected re-based integrated IBU (dilution anchor, nb)** | **10.510658** | **10.219647** |
| Cone Born integrated (unsubtracted, nb) | 13.026987 | 12.697511 |
| Extended-set f_res′, Born-weighted | 7.8928% | 8.1194% |
| **Expected re-based integrated Born (dilution anchor, nb; RU-3 center)** | **11.998797** | **11.666545** |
| Born anchor via integrated C_rad (cross-check, nb) | 11.998098 | 11.660992 |
| vs Rev-2 (no-cone) extended anchors | RAW +0.0393% / Born +0.0474% | RAW +0.0416% / Born +0.0489% |
| vs retired Set-R-only anchors | −4.6749% / −4.6673% | −4.9310% / −4.9236% |

Full precision (JSON sidecar carries repr-level values): anchors RAW
10.510657911476054 / 10.219646767689696; Born 11.998797392290179 /
11.666544718471744.

- The two Born constructions differ by +0.0058% (h+) / −0.0476% (h−) — the
  data-weighted-vs-table-integrated C_rad weighting residue (cone
  data-weighted scales 1.1414933338 / 1.1414902670 vs table-integrated
  1.1415172920 / 1.1410367377); the per-bin Born-weighted anchor is the gate
  center, the integrated-C_rad value the cross-check (same convention as
  Rev 2 and the pre-S2′ chain).
- The cone anchors are HIGHER than the Rev-2 no-cone anchors by
  +0.039%/+0.042% (RAW): the cone removed 0.49% of the data vector, of which
  the Set-R′-accounted contamination share exceeded the acceptance-recovered
  true-hadron loss — the background-subtracted result is slightly LARGER on
  the cone base, as expected when real contamination is excised.
- The additional shift vs the retired Set-R-only anchors stays dominated by
  the electron class (−4.67%/−4.93% Born) with h+/h− asymmetry **−0.2563 pp**
  (Rev-2 no-cone: −0.2576 pp) — the charge-asymmetric electron class
  (h− 3.9632% vs h+ 3.7494% of D_cone) persists under the cone, as the impact
  study predicted (alpha-D ≈0.21 pp).
- **Soft pre-unfold expectation (not a gate):** the pre-S2′ official
  non-linearity precedent (−0.0283%/−0.0271% at a 3.66% subtraction) scales
  quadratically to ≈ **−0.131% / −0.133%** at the 7.90%/8.13% input — soft
  landing ≈ **11.9830 / 11.6510 nb Born** and ≈ **10.4969 / 10.2060 nb RAW**;
  gate window ±0.5% of the dilution anchors (RU-3).
- **RU-2 (cone variant f_i):** on-D_cone h+ 11.3344% / h− 10.9602%; expected
  on data_sub′ **h+ 11.3063% / h− 10.9326%** (drift −0.028 pp); window =
  on-D_cone ± 0.12 pp = **[11.2144, 11.4544]% (h+) / [10.8402, 11.0802]% (h−)**
  — expected values INSIDE (verified read-only).
- **CP-3 (charge asymmetry):** cone Born asymmetry unsubtracted **+1.2808%**
  → **+1.4040%** at the anchors (shift **+0.1232 pp**; RAW-level +1.2807% →
  +1.4038%). The shift is a PHYSICS EFFECT (removal of a genuinely
  charge-asymmetric electron class); gate expectation-centered (§F).
  Rev-2's no-cone expectation (+0.1325 pp) and its cone reference
  (+0.1220 pp) bracket the Rev-3 value — consistency check passed.
- **SA-4 (f_res′ envelope, cone):** h+ max **31.858%** (release local bin 34
  = S0_x0, z_idx 6, pT pair 4) / h− max **23.034%** (same bin); medians
  3.0635%/3.4409%; 8 (h+) / 10 (h−) bins ≥10%; **1 bin ≥20% per charge**;
  3 zero-D release bins f=0. (Identical maxima to the Rev-2 cone REFERENCE —
  that reference is now the adopted basis.)
- **SA-5 (integrated f_res′ asymmetry):** h+ 7.1686% vs h− 7.3886% →
  expected difference **−0.2200 pp**; gate expectation-centered (§F).
- **Alpha-denominator swap:** the Rev-2 swap diagnostic is MOOT on the cone
  base — the alpha0p2 vector IS the data vector (identity exact, §0-bis.1),
  so the V0/lowQ2 denominator-swap bound is **0 by construction** (verified:
  max |f_res′(cone-D) − f_res′(alpha0p2-D)| = 0.0, both charges). The
  bgsub_stage_rel selection-coherence operand correspondingly collapses
  (§E).

## E. Budget structure — 15 + 4 = 19 components (cone base)

Base: the **cone 15-component S2′ budget — PRECONDITION: does not exist yet**
(§G; the impact study §5.2 item 8 already prescribed the full budget
re-derivation on the new nominal). The four bgsub components are added per
bin; their Rev-3 (cone-base) treatments:

| # | Component | Rev-3 (cone-base) treatment |
|---|---|---|
| 16 | `bgsub_stat_rel` | `sqrt(varB_R′_cone)/data_sub′` with the PACKAGED electron variance (NO rescale inflation — the Rev-2 1/s² factors are gone); expected medians ≈ **0.0806% / 0.0958%** (Rev-2: 0.0831%/0.0988%); sparse maxima ≈10.4%/14.5% (smoothing rule unchanged); exact values are build outputs |
| 17 | `bgsub_model_rel` | quadrature of V0 half-spread ⊕ lowQ2 ±30% ⊕ electron RG/DJ half-spread (packaged cone templates) — **the V0/lowQ2 cone-survival envelope term is REMOVED (register closed)**; the lowQ2 ±30% term absorbs the α0p1 radius-mismatch caveat (≪0.01%, §0-bis.3c); expected medians ≈ **0.2016% / 0.2279%** (Rev-2: 0.2675%/0.2985%; terms at median: v0hs 0.1339%/0.1473%, ehs 0.1202%/0.1395%, lowq2-30 ≈0); exact values are build outputs |
| 18 | `bgsub_stage_rel` | definition unchanged (pre-vs-post stage difference ⊕ selection-coherence bound); RE-MEASURED on the cone chain at gate time (CP-1 operands = the cone Born release); **the V0/lowQ2 denominator-swap bound is 0 BY CONSTRUCTION** (alpha0p2 vector == data vector, §D) and the electron selection-transfer operand is GONE (rescale retired); only the stage-difference term remains — expected SMALLER than the pre-S2′ medians (0.0909%/0.0950%), which included the swap bound |
| 19 | `bgsub_fakefrac_rel` | fc0-only scope UNCHANGED (Rev-2 correction stands): per-reco-bin `|1−r_min,standing|·f_i,standing,i` with `f_i,standing,i = F_fc0,i/(B_i+F_i)` from the CONE variant category exports (partition closure exact, §A.1.2); r_min = 0.9816719489793092 retained as the fc0 common-model PROXY (allfake-convention envelope 0.9817–0.9872, NucliaWeight caveat — it CANNOT be re-derived on the cone chain: no template-bearing category remains in the cone fake column; CP-4 re-scoped, §F); read-only cone verification: fc0 share of the cone variant fake column 93.45%/94.00% response-internal, standing data-weighted on D_cone 10.5727%/10.2889% (no-cone: 10.5966%/10.3064% — down with the cone f_i); expected medians ≈ **0.1846% / 0.1815%** (max ≈0.58%/0.55%); exact values are build outputs |

**Double-counting checks:** the Rev-2 verdict CARRIES (all pairs DISJOINT or
CLOSED) with two entries RETIRED: the "electron rescale vs bgsub_stage"
restructured-disjoint entry (the rescale no longer exists) and the
envelope-vs-stage distinction (the envelope no longer exists). No new
overlap is introduced by the cone: the cone's data/response content changes
are selection effects carried by the response/prior/f_i objects themselves,
not by any budget component.

**Answer to "do `bgsub_stat_rel`/`bgsub_model_rel` change?": YES, both
shrink.** Stat: the electron variance loses the ×1.16-ish rescale inflation
(medians 0.0831→0.0806 / 0.0988→0.0958). Model: the cone-survival envelope
term (median contribution ≈0.13%/0.13%) is REMOVED and the electron
half-spread is the packaged cone-template spread (medians 0.2675→0.2016 /
0.2985→0.2279). Fakefrac: essentially unchanged (0.1846/0.1815 vs Rev-2
0.1846/0.1820 — the cone f_i drop is small). Stage: the swap bound collapses
to 0. Expected bgsub quadrature medians (stat ⊕ model ⊕ fakefrac):
**≈ 0.285% (h+) / 0.307% (h−)**, plus the re-measured stage term — expected
total ≈ **0.29–0.32%**, DOWN from Rev-2's ≈0.35%/0.38% (the envelope closure
is the driver). The 19-comp total waits on the cone 15-comp base (§G).

Quoting policy (extends Rev 2): once the re-based gate passes, the
**19-component total** is the quoted published total; every quote states
prior basis (coherent body+miss FROM THE CONE VARIANT RESPONSE), Born-vs-raw
level, observable basis (S2′ detint-corrected), bgsub status (Set R′
e+V0+lowQ2 subtracted), AND **the cone-selection flag (α≥0.2 imposed;
Grebenyuk-precedent alignment)**. Rows 18 (`diffvm_rel`) and 19
(`gammap_bkg_30pct_rel`) of the register remain BLOCKED and unchanged.

## F. Validation gate deltas — 26 items: 5 IDENTICAL, 20 CHANGED, 1 NEW

**Identical (5):** IV-4 (binning identity — unchanged under the cone),
IV-5 (Set A machinery cross-check), IV-6 (set-composition re-derivation —
the cone changes the reco space, not the class structure; the independent
re-derivation must still yield e + V0 + lowQ2), RU-4 (C_rad bitwise,
releasebin DJ14 table sha256 ledger — byte-identical use confirmed in the
cone Born release), RU-5 (physicality/serialization).

**Changed (20) and new (1):**

| Item | Delta vs Rev 2 |
|---|---|
| IV-1 | Template totals verified AS PACKAGED (cone-applied, selection-consistent): e 148,976.4352 (h+ 73,701.28554105517 / h− 75,275.14969129545); V0 132,065.2768; lowQ2 208.3795; det.int. 27,405.8031 verified NOT subtracted; the survival rationals/rescaled totals are RETIRED from the verification (rescale no longer consumed); PLUS the selection-consistency assertion: cone data vector == June alpha0p2 vector per-bin exact |
| IV-2 | CHANGED — data vector identity: the CONE vector, totals h+ 1,965,700 / h− 1,899,373 / 3,865,073 (980 bins); alpha_pre parity 3,902,480 == central in-PS exact; cone rejections 28,582 in-PS (14,465/14,117), 19,024 in-grid (9,601/9,423); matched-electron rejection/survival counters 0/0 (`not_e` kept) |
| IV-3 | CHANGED (re-scoped) — cone-vector == alpha0p2-vector per-bin IDENTITY (max dev 0, read-only verified; the builder must reproduce it); the Rev-2 "alpha0p2 vector + cone Δ 19,024" item is superseded (the alpha0p2 vector IS the data vector on the cone base) |
| **IV-7 (NEW)** | **Cone-parity gate:** (a) data side — the IV-2/IV-3 counters exact; (b) response side — shard alpha-QA 174/174 with cone counters; response-eligible MC rejections 162,528 = 83,782/78,746 (matched_signal 77,974/74,586, detint 1,121/652, unflagged_fake 4,687/3,508; NO e/V0 class); body-diff vs the no-cone S2′ response within [0.00204, 0.00216] (guard values 0.002079/0.002101/0.002045/0.002158 per combo); Phase-F miss-row guard E values 697/700/777/845 exact in windows; (c) f_i parity — denominator drop −0.0940%/−0.0923%/−0.1011%/−0.1006% (RG h+/h−, DJ h+/h−), real-change rows 127/126/130/125, max per-row rel ≤2.56%, cone f_i ∈ [0,1), integrated shift INSIDE the impact-study band (measured read-only: response-internal −0.004…−0.007 pp; S2′-variant on-D −0.0233/−0.0166 pp, DOWN) |
| SA-1 | 0 negative bins after the fine-level guard (read-only verified: 0 TRUE negatives); fine-empty bins with content guarded: h+ 2 (218/358; 0.1915/0.1463), h− 4 (341/410/470/480; 0.0167–0.0527); release-empty bins h+ 109/179, h− 240 subtract exactly 0 |
| SA-2 | B_R′_cone sums h+ 140,912.91681341422 / h− 140,337.17473977088 / 281,250.09155318514 (7.2767% of D_cone); data_sub′ sums h+ 1,824,787.4209930277 / h− 1,759,035.953778433; the Rev-2 rescaled sums (146,841.18/146,392.04) are superseded |
| SA-3 | E1b exactly-once Poisson with `varB_R′_cone` (NO rescale factors, §C.1); double-Poisson = STOP |
| SA-4 | f_res′ envelope (cone): h+ max 31.858% (local bin 34 = S0_x0, z_idx 6, pT pair 4) / h− max 23.034% (same bin); medians 3.0635%/3.4409%; 8/10 bins ≥10%; 1 bin ≥20% per charge; 3 zero-D release bins f=0 |
| SA-5 | Expectation-centered: realized integrated f_res′ asymmetry within **−0.2200 pp ± 0.02 pp** (h+ 7.1686% vs h− 7.3886% of D_cone); the Rev-2 −0.2355 pp (rescaled basis) is superseded |
| RU-1 | Configuration identity vs the **cone RAW release**: cone S2′ variant RAPGAP31 response (sha256 `fda5466f…`), coherent `body+miss` prior FROM THE CONE VARIANT RESPONSE (campaign Phase-D prior; repointed + asserted; no no-cone-read code path), IBU k=4, L=351,000; repro mode reproduces the cone RAW release (11.412233/11.123626 nb); config diff vs cone nominal = input vector only |
| RU-2 | Fake fraction exactly once: the **cone variant** per-reco-bin `f_i` (byte-identical derivation to the cone RAW release); integrated removal on `data_sub′` within **[11.2144, 11.4544]% (h+) / [10.8402, 11.0802]% (h−)** (expected **11.3063% / 10.9326%**, drift −0.028 pp from on-D_cone 11.3344%/10.9602%); the Rev-2 windows (no-cone basis) are superseded; recomputing `f_i` or using the no-cone table = STOP |
| RU-3 | Magnitude window: integrated within **±0.5%** of the cone dilution anchors Born h+ **11.998797** / h− **11.666545** nb (RAW 10.510658/10.219647; integrated-C_rad cross-check 11.998098/11.660992); the Rev-2 anchors (11.993108/11.660841) and all earlier anchors are RETIRED for gating; soft pre-unfold landing ≈ 11.9830/11.6510 nb Born (§D) |
| CP-1 | Stage comparison operands repointed to the **cone Born release**: per-bin `|pre-unfold − coneBorn·(1−f_res′)|/pre-unfold` ≤1% (≤2% sparse, justified), integrated ≤0.5%; signed vector saved; `bgsub_stage_rel` re-measured on the cone chain (expected SMALLER — the swap bound is 0 by construction); sparse exceptions expected (worst-bin f_res′ 31.9%); the integrated ≤0.5% threshold binds and is expected to hold (soft non-linearity ≈ −0.13%) |
| CP-2 | Add-back closure reference: unfolding `data_sub′ + B_R′_cone` (bitwise == data_cone) through the **cone variant chain** reproduces the **cone RAW release** bitwise (≤1e-9 relative on all totals) |
| CP-3 | Expectation-centered: charge-asymmetry shift vs the **unsubtracted cone Born asymmetry** (+1.2808%): realized shift within **+0.1232 pp ± 0.02 pp** (expected realized asym +1.4040%; RAW-level +1.2807% → +1.4038%); the shift is a PHYSICS EFFECT (charge-asymmetric electron class); the Rev-2 +0.1325 pp center is superseded |
| CP-4 | RE-SCOPED — the fc0 standing share re-derived on the **cone variant** (expected: response-internal standing 10.4975%/10.2173%; fc0 share of F 93.45%/94.00%; standing data-weighted on D_cone 10.5727%/10.2889%); the r envelope 0.9817–0.9872 CARRIES as the allfake-convention PRIOR — it CANNOT be re-derived on the cone chain (no template-bearing category remains in the cone fake column; unflagged fakes have no template); NucliaWeight caveat carried; per-category results recorded |
| CP-5 | Budget closure: the **15** existing components byte-identical to the **CONE 15-comp package** (PRECONDITION — must be produced first, §G; the production recipe is **§H**; 0 mismatches once it exists); the **19**-component quadrature identity per bin (≤1e-12); medians reported; quoting-policy switch to `total_with_bgsub19_rel` recorded |
| PM-1 | P-ALPHA disposition UPDATED (closure by construction): the waiver cites the cone basis — (i) same-selection templates (rescale RETIRED), (ii) V0 gap closed by construction, lowQ2 α0p1 radius mismatch carried explicitly (≪0.01%, inside the lowQ2 ±30% term); the Rev-2 rescale/envelope waiver basis is superseded; **a formal closure/supersession entry in STATE.md is REQUIRED** (recorded action) |
| PM-2 | Release hygiene: new dated candidate `born_crad_bgsub_setRprime_conebase_...` (the Rev-2 `_detints2primebase_` and the `_setR_` names are superseded/retired for new candidates); the cone releases are STUDY-TREE PROVISIONAL (R9) — their promotion/install must be recorded before or with the bgsub candidate promotion; untouched (read-only): all cone campaign products, both no-cone S2′ releases, all pre-S2′ releases, and the pre-S2′ bgsub study tree |
| PM-3 | Sidecar hygiene: sha256 ledger extended with the cone campaign artifacts — cone data vector JSON (`3371f5dc…`), cone S2′ response ROOTs (`fda5466f…` / `c84a8c2d…`) + merged npz (`736fae14…`), cone RAW/Born release IBU CSVs (`ded84be1…` / `20b5c431…`), f_i crosscheck CSVs (4), alpha-QA sums (2), Phase-F rerun record, guard adjudication doc — plus the carried electron template npz's (`a04dd1a6…` / `66141a54…`) and C_rad table; no stale blind C_rad sidecars (unchanged rule) |
| PM-4 | State rule: no merge until 1–24 + IV-7 PASS; the **19-component** total becomes the quoted published total at promotion; STATE.md/RUN_STATUS.md record the **cone-S2′ + Set R′ (e+V0+lowQ2)** basis, the cone-selection flag in the quoting rule, and the P-ALPHA closure entry |

**Gate checklist count: 26 (was 25): IV 7 + SA 5 + RU 5 + CP 5 + PM 4.**

## G. Standing preconditions, blockers, and build-time measurements

- **Cone 15-comp budget package — NEW PRECONDITION (BLOCKER for CP-5 and the
  19-comp total):** the full 15-component S2′ budget must be re-derived on
  the cone base (impact study §5.2 item 8: all components, anchors,
  1000-toy sidecars, `fakecol_mcstat_rel`, shifted-response detector
  components) BEFORE or WITH the bgsub build; the bgsub chain consumes it
  byte-identically. **The complete production recipe, machinery re-scoping,
  and re-dispatch plan are §H (2026-10-11 extension).**
- **Cone-base promotion/install:** the cone RAW/Born releases are study-tree
  R9 PROVISIONAL. Promotion to canonical (and the STATE.md/RUN_STATUS
  supersession records for the no-cone S2′ releases as re-base target) is a
  recorded decision that must precede or accompany the bgsub candidate
  promotion (PM-2). Until then every cone-based value carries the
  study-tree PROVISIONAL flag.
- **P-ALPHA formal closure:** both critic conditions discharge by
  construction on the cone basis (§0-bis.3d); a formal closure/supersession
  entry in STATE.md is REQUIRED (baseline-update procedure owns the file).
- **lowQ2 radius rebuild (OPTIONAL):** the α0p1-radius mismatch is bounded
  ≪0.01% and carried inside the lowQ2 ±30% model term; a cheap single-sample
  α0p2-radius rebuild would remove even that caveat. The ledger §11.4
  central-selection template production is otherwise MOOT (achieved by the
  cone basis for e/V0).
- **Build-time measurements** (this addendum supplies expectations, not
  measurements): the pre-unfold re-unfold result (RU-3 realization vs
  anchor); `bgsub_stage_rel` re-measurement (only the stage-difference term
  remains); the fc0 re-derivation on the cone variant (CP-4); exact 19-comp
  medians; the 1000-toy sidecar on `data_sub′` through the cone chain.
- **Keeper-boundary envelope** (k_parented K-nuclear secondaries ≤0.064% of
  the data vector) remains REPORT-ONLY per the canonical budget — unchanged.
- DIFFVM remains a bounded uncovered residual (never subtracted, rows 18/19
  BLOCKED); the normalization-owner fake-fraction provenance dispatch runs
  unchanged and its outcome re-triggers IV-1..IV-7 automatically.
- Absolute hadron cross-section quoting remains governed by the existing
  normalization policy; this addendum changes only the basis of the
  background-corrected chain and the uncertainty register.
- Implementation and production (driver copy, shard/Condor mechanics,
  official chain, certification) are **later dispatches** — operational
  execution routes to `h1-script-runner` / `h1-naf-operator` per the
  standing handoff rules.

## H. Production preconditions — the cone 15-comp budget recipe (2026-10-11 extension; JSON v4)

**Trigger:** the Stage-1 production dispatch stopped at precondition
verification
(`H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/BLOCKER_RECORD_2026-10-11.md`,
executor h1-script-runner 2026-10-11; nothing was launched, nothing canonical
was touched). Its input-by-input disposition is adopted in full here.

**§H.0 — What changes and what does not.** NOTHING in §D/§E/§F changes:
every Rev-3 anchor, window, and budget expectation is UNCHANGED by this
section — none depends on the productions below (they depend only on the
EXISTING cone core chain, the packaged cone-applied templates, and carried
artifacts). The cone 15-comp component medians remain BUILD OUTPUTS; §H.1
states sanity windows only. This section adds production recipes, pinned
gate constants (read-only computed, §H.7), and the re-dispatch plan. All
inputs remain READ-ONLY except the new campaign tree (§H.6 P0).

**§H.0-bis — Exists vs missing (verified against the blocker record and the
campaign trees, 2026-10-11).**

- EXISTS (cone core chain): cone data vector
  `production/data_vector_cone_alpha_central_2026-10-11.json` (in-grid
  3,865,073 = 1,965,700/1,899,373); cone central merged ROOT
  `production/migration_inputs_cone_alpha_central_2026-10-11.root` (RG+DJ,
  embeds the cone DV + binmap); cone S2′ variant responses
  `production/migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.{root,npz}`;
  cone RAW + Born releases under `production/release_driver/`; f_i
  crosschecks; alpha-QA sums. Also EXISTS: the two cone-EDITED self-tested
  shifted-leg builders (`copied_scripts/build_shifted_response_data_vector_v4.py`
  self-test 11/11; `..._s2prime_v4.py` 13/13; cone from RAW unshifted
  branches — identical cone across all shift legs) and the cone-ported
  `copied_scripts/build_data_vector_v4.py` (the Phase-A builder itself).
- CARRIED no-cone artifacts (read-only reuse, selection-independent or
  gen-level): the 3 HFS shifted sidistree sets (660 files per tag, the same
  file sets the legacy HFS DV builds consumed); the legacy shifted/seed data
  vectors (acceptance references, §H.2/§H.3); PHICAMP, B1JSON, BUDGET_CSV,
  the C_rad tables, PARENTAGE (§H.5 carry conventions); the 658-file central
  sidistree list (byte-identical shard-list reuse per campaign precedent).
- MISSING (the three productions + the re-scoping): (1) the 5 cone shifted
  detector data vectors; (2) the 16 cone tracking-seed data vectors — and NO
  cone-ported tracking-drop builder exists anywhere; (3) the cone 1000-toy
  stat sidecar; (4) the gate/machinery re-scoping (unprescribed until §H.5).

### H.1 Per-component inventory (15 components)

| # | Component | Cone input needed | Status | Recipe (one line) |
|---|---|---|---|---|
| 1 | `ibu_stat_rel` | cone 1000-toy sidecar on the cone variant chain | **MISSING** | §H.4: repointed sidecar campaign (2 Condor jobs + assembler; seeds 12345/22345, 1000 toys/charge) |
| 2 | `ibu_model_rel` | cone coherent legs `dj_v_coh`, `ibu_v` (in-machinery) + CARRIED B1 floors | EXISTS + CARRY | no production; legs from the existing cone responses; floors carried per §H.5-bis |
| 3 | `ibu_reg_rel` | cone k-scan legs `k3_v`, `k5_v` (in-machinery) | EXISTS | no production |
| 4 | `hfs_jes_rel` | cone shifted DV over the `hfs_jes_down` shifted sidistrees | **MISSING** | §H.2-HFS: cone-ported `build_data_vector_v4.py` over the 660 legacy shifted files (1 job) |
| 5 | `hfs_rces_rel` | cone shifted DV over the `hfs_rces_up` shifted sidistrees | **MISSING** | §H.2-HFS: same, tag `hfs_rces_up` (1 job) |
| 6 | `hfs_theta_rel` | cone shifted DV over the `hfs_theta_up` shifted sidistrees | **MISSING** | §H.2-HFS: same, tag `hfs_theta_up` (1 job) |
| 7 | `elec_energy_rel` | cone shifted DV, shift `elecE_p1pct` | **MISSING** | §H.2-e: cone-edited shifted builder `--mode data-vector --shift elecE_p1pct` over the 658 central files (1 job) |
| 8 | `elec_theta_rel` | cone shifted DV, shift `elecTheta_p1mrad` | **MISSING** | §H.2-e: same, shift `elecTheta_p1mrad` (1 job) |
| 9 | `lumi_rel` | none | N/A | flat 2.7%, fully correlated — no production |
| 10 | `tracking_eff_m0p5_rel` | 16 cone seed data vectors (seeds 314159–314174) | **MISSING** (builder port too) | §H.3: cone port of the tracking-drop builder + 16 jobs + P=0 anchor job |
| 11 | `phi_h_gauss_rel` | CARRIED no-cone flat-leg absolutes re-expressed on the cone nominal | EXISTS (carry) | no production; §H.5-bis phi_h carry convention |
| 12 | `c_rad_gen_rel` | unchanged gen-level tables | EXISTS (carry) | no production (gen-only standing ruling) |
| 13 | `c_rad_stat_rel` | unchanged DJ14 releasebin table stat | EXISTS (carry) | no production |
| 14 | `detint_removal_rel` | cone legs `inc_RG`/`inc_DJ` (in-machinery) + CARRIED parentage (0.909920 ± 0.000269) | EXISTS + CARRY | no production |
| 15 | `fakecol_mcstat_rel` | cone variant npz w2 matrix (exists) | EXISTS | no production; G12 re-scoped per §H.5 |

**8 of 15 components need no new production** (2, 3, 9, 11, 12, 13, 14, 15);
5 need the shifted-DV campaign; 1 needs the seed campaign; 1 needs the
sidecar. Sanity windows (acceptance only, NOT Rev-3 anchors): each component
median within ±25% relative of its no-cone S2′ 15-comp value
(`systematics_detints2prime_15comp_..._2026-10-10/README.md`: ibu_stat
3.5902/3.8330%, ibu_model 1.3623/1.4271%, ibu_reg 0.5767/0.5669%, hfs_jes
0.3624/0.3918%, hfs_rces 0.2595/0.2794%, hfs_theta 0.2965/0.3120%,
elec_energy 4.5590/4.8422%, elec_theta 0.5091/0.5601%, tracking 0.4910/0.4929%,
phi_h 1.0494/1.0568%, c_rad_gen 0.5844/0.5804%, c_rad_stat 0.4272/0.4429%,
detint_removal 0.0296/0.0282%, fakecol 0.3443/0.3507%) — the cone changes
0.49% of data-vector content and ~0.3% of unfolded content, so no median
should move materially; exact values are build outputs.

### H.2 Cone shifted detector-DV campaign (5 legs + 1 anchor leg)

**Leg classification.** The 3 HFS legs are TREE-LEVEL shifts: the legacy
shifted sidistrees already exist
(`output_v6/systematics/hfs/<tag>/sidistree/data/{ep0304,em0405,em06,ep0607}/sidistree_*.root`,
660 files per tag) and are selection-independent (the HFS calibration shift
changes hadronic kinematics; the electron branches — hence the cone — are
unchanged). They are REUSED byte-identical; NO tree re-production. The 2
electron legs are POST-HOC shifted-electron kinematics on the nominal trees;
the cone-edited shifted builder computes the cone from the RAW (UNSHIFTED)
reco branches by construction, so the cone rejection set is IDENTICAL across
all shift legs (the systematic varies kinematics/binning only — the builder
banner/FIDUCIAL_TAG convention).

**Recipes.**

- **HFS legs** (tags `hfs_jes_down`, `hfs_rces_up`, `hfs_theta_up`): the
  campaign's cone-ported `build_data_vector_v4.py`
  (`cone_alpha_central_build_2026-10-11/copied_scripts/`, run from a dated
  copy in the new campaign tree) with
  `--data-root-files <shifted-tree globs, 660 files>` `--out-json
  <campaign>/shifted_dvs/data_vector_<tag>_cone_alpha_central_2026-10-11.json`
  `--label raw_data_no_bgsub_<tag>_cone`. One Condor job per tag (the
  `wrap_build_hfs_down_data_vector_v4.sh` + `submit_build_hfs_down_data_vector_v4.condor`
  pattern, repointed; request ≈2 GB, 3 h cap).
- **Electron legs** (shifts `elecE_p1pct`, `elecTheta_p1mrad`): the cone-edited
  `build_shifted_response_data_vector_v4.py` `--mode data-vector --shift
  <SHIFT_NAME>` over the SAME 658 central sidistree files as the cone
  Phase-A build (byte-identical shard-list reuse; the
  `submit_build_shifted_data_vector_v4.condor` pattern, repointed; one job
  per shift).
- **Zero-shift anchor leg** (REQUIRED, 1 job): `--shift zero` through the
  cone-edited shifted builder over the same 658 files → the cone zero-shift
  DV. ACCEPTANCE: its per-bin arrays must be BIT-IDENTICAL to the campaign
  cone data vector (`data_vector_cone_alpha_central_2026-10-11.json`; max
  per-bin dev 0). This validates the shifted builder's cone port before any
  shifted leg is trusted.

**Acceptance per shifted leg** (all five): (a) schema — 980 per-bin rows,
charges {0,1}, 490 bins per charge, label/cone stamps present
(`cone_campaign`, `fiducial_contract`, alpha-rejection counters > 0);
(b) per-bin SUBSET property vs the legacy no-cone shifted DV
(§H.7 table): `cone_shifted[i] <= nocone_shifted[i]` for all 980 rows
(EXACT — the cone only removes); (c) cone-removed total
`sum(nocone_shifted) − sum(cone_shifted)` within **[0.35%, 0.65%]** of the
legacy shifted-DV total (nominal removal 0.4861%/0.4937%; per-leg window
centers 0.4809–0.4859% / 0.4882–0.4934%, §H.7); (d) the leg's total change
vs the CONE nominal within ±0.05 pp of the legacy leg's change vs the
NO-cone nominal (reference changes: hfs_jes +0.0397/+0.0516%,
hfs_rces +0.3007/+0.3086%, hfs_theta +0.3036/+0.3106%,
elecE +1.0747/+1.1112%, elecTheta +0.0616/+0.0555% pos/neg).

### H.3 Tracking-drop builder cone port + 16-seed campaign

**The port.** No cone-ported tracking-drop builder exists. The port MERGES
two already-verified edits into ONE new dated script in the campaign tree
(NEITHER parent is modified):
- Parent A (drop): `output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/scripts/build_data_vector_v4.py`
  — the randdrop patch (`randdrop_tracking_eff.py` import; env
  `H1_RANDDROP_P=0.005`, `H1_RANDDROP_SEED_TAG=seed<N>`;
  `make_keep_mask` on the valid track rows; counters
  `n_reco_dropped_randdrop`; the `.randdrop_audit.json` sidecar).
- Parent B (cone): the campaign's cone-ported
  `copied_scripts/build_data_vector_v4.py` — the alpha predicate
  (`_compute_reco_electron_alpha`; `ELECTRON_ALPHA_VETO_RAD = 0.2`; NaN
  alpha rejected; applied at admission BEFORE the z sanity guard; cone QA
  counters + FIDUCIAL_TAG stamps).
The two edits sit at DISJOINT code sites (drop = valid-row thinning; cone =
admission), so the merge is both-verbatim. ORDER/SEMANTICS: the drop thins
the valid track rows FIRST (efficiency perturbation on the pre-admission
population); the cone applies at admission AFTER, identically to the
campaign port. Track identity
(`seed_tag|run|event|track_index|row_index` → sha256-uniform) is
cone-independent, so the same track receives the same drop decision as in
the legacy seeds. RELATIVE TO the legacy seed legs the cone is the ONLY
selection change; relative to the campaign cone DV the drop is the ONLY
content change.

**The campaign.** 16 jobs, seeds 314159–314174 (the legacy seed set,
reused EXACTLY), each: the ported builder over the SAME 658-file list
(`manifests/data_all.txt` pattern — byte-identical reuse), env
`H1_RANDDROP_SEED_TAG=seed<N>`, outputs
`<campaign>/tracking_cone/data_vector_cone_seed<N>_all4p.json` (+ qa +
randdrop_audit sidecar), label
`tracking_eff_m0p5_randdrop_seed<N>_cone_data4p_prod_data` (the
`run_tracking_eff_prod.sh` + per-seed `.sub` pattern, repointed; ≈8 GB,
workday flavour). PLUS one P=0 anchor job (`H1_RANDDROP_P=0.0`, any seed
tag): `should_drop_track` returns False identically, so the per-bin arrays
must be BIT-IDENTICAL to the campaign cone data vector. A packaging step
writes `<campaign>/tracking_cone/ensemble_manifest.json` (schema mirror of
`tracking_ensemble_coherentprior_2026-10-07/ensemble_manifest.json`:
`seed_inputs` → the 16 cone seed DV paths; NO re-unfold campaign is needed —
the data-side convention computes the component in-machinery).

**Acceptance per seed:** (a) schema/stamps as §H.2(a) + randdrop_audit
present with `n_reco_dropped_randdrop` ≈ 0.5% of pre-drop (legacy DV-level
drop fractions: 0.49276%/0.49349%); (b) per-bin subset vs the legacy seed
DV (EXACT, all 980 rows); (c) removed fraction vs the legacy seed DV within
**[0.35%, 0.65%]** (expected ≈ 0.4837% pos / 0.4912% neg — the nominal
removal times the drop-survival factor (1−0.0049276)/(1−0.0049349),
§H.7); (d) the P=0 anchor bit-identity (above).

### H.4 Cone 1000-toy stat sidecar

**Machinery:** dated copies of the three sidecar scripts
(`studies/detint_variant_s2prime_2026-10-09/stat_sidecar_campaign_2026-10-10/scripts/`
→ the campaign tree), repointed at the cone chain; the precedent campaign
is cluster 5481723. Repoints: `REL` → the cone RAW release;
`VARIANT_RESPONSE` → `migration_s2prime_cone_rapgap31_2026-10-11.root`;
`VARIANT_ID_EXPECTED` → `cone_alpha_central_S2prime_2026-10-11` (the
response-identity pre-flight assert); the measured data vector from the cone
central ROOT (identical to the cone DV JSON). UNCHANGED: seeds
pos 12345 / neg 22345, N_TOYS 1000 per charge, independent Poisson on the
rounded measured cone data vector, FIXED cone variant f_i + response +
coherent body+miss prior, IBU k=4, L = 351,000 nb⁻¹, the authoritative
covariance schema, and the central-column stat_component CSVs.

**Deltas vs the precedent campaign (recorded in payloads):** (1) the nominal
cross-check — the cone release `ibu/cross_sections.json` vs the recomputed
cone variant IBU at ≤1e-10 — is PRE-VALIDATED read-only this revision
(reproduction dev ≤1.8e-15, §H.7); (2) the G5-style full-product closure
re-check is reproduced VERBATIM and UNWEAKENED against the cone release
nominal (n|z|>3 ≤ 4 is the stop condition; a count > 4 STOPS the campaign
before install); (3) the precedent's bit-level cross-check against the
gate-suite G5 1000-toy product is DROPPED — no cone gate-suite toy product
exists (the same gap the cone f_i crosschecks recorded for the TUnfold
JSON); the closure re-check + nominal cross-check carry the validation.

**Install + acceptance:** the assembler clone installs ADDITIVELY at
`<cone RAW release>/stat_sidecar_1000toy/` (covariance JSONs + plots +
byte-copied stat_component CSVs + manifest recording the ORIGINAL
manifest/tarball sha256 as unchanged) and verifies isolation (the release
manifest `6ca6b4a1…` and tarball `b16424f0…` bit-identical pre/post; writes
confined to the campaign dir + the sidecar dir). Acceptance: 1000 toys per
charge complete; per-charge status PASS; stat_component CSVs 245 rows,
`nominal_yield_coherent` == the cone release yields (≤1e-9) and
`toy_stat_unc_yield/yield == ibu_stat_rel_new` (≤1e-12); the closure stop
condition not triggered; deterministic reproduction — re-running one charge
with the same seed must reproduce the stat_component CSV BITWISE (pure
numpy-seeded; no entropy sources).

### H.5 Gate/machinery re-scoping (the cone edition of `run_s2prime_systematics_rebase.py`)

The cone edition is a NEW dated copy in the campaign tree (the canonical
builder and its study-tree copy are NEVER modified). Input map repoints:
`RELEASE`/`TARBALL` → the cone RAW release + tarball;
`CENTRAL_ROOT` → `migration_inputs_cone_alpha_central_2026-10-11.root`;
`CENTRAL_DV` → the cone DV JSON; `VNPZ_RG/DJ` → the cone S2′ npz's;
`VAR_IBUT_CSV` → the cone release `ibu/cross_sections.csv`; `STAT_CSV` →
the cone sidecar; `DET_TAGS` → the 5 cone shifted DVs (§H.2);
`TRKCAMP` → the cone tracking campaign (manifest only); `OUT`/`PKG_*` → the
campaign tree's package dir. CARRIED UNCHANGED (no-cone paths, read-only):
`B1JSON`, `BUDGET`/`BUDGET_CSV`, `PHICAMP`, `EMDIR`/`EM_CSV`/`RG_EM`,
`DJ_TAB`, `PARENTAGE`, and the `SPEC_*` parentage constants. REMOVED
constants (their gates drop): `IBUNAT`, `CAND_CSV`, `VECTORS_NPZ`,
`G10_NPZ`, `GATEREPORT`, the DETCAMP/TRKCAMP campaign-CSV paths.

**The 31 named gates** (machinery names from
`run_s2prime_systematics_rebase.py`; line numbers refer to the canonical):

| # | Gate (canonical) | Cone re-scoping |
|---|---|---|
| 1 | STEP 0 candidate manifest sha (L151) | REPPOINT: cone manifest sha256 `6ca6b4a148e3b83702440c75800045e31beb6201fdb2fd48cdd0f95ae87aa8d3` |
| 2 | STEP 0 candidate tarball sha (L152) | REPPOINT: cone tarball sha256 `b16424f0f048880178ebf05573765cb094f1655d8bd4e508e35da5d4ca0c711e` |
| 3 | STEP 0 SEED_PATHS == 314159–314174 (L155) | REPPOINT: the CONE tracking campaign `ensemble_manifest.json` |
| 4 | STEP 0 GUARD_FILES existence (L156–170) | REPPOINT: the cone/carried input map above |
| 5 | STEP 0 guard_before/after tree hashes | REPPOINT: cone campaign tree + carried artifacts + cone release tree |
| 6 | STEP 1 DATA_SUMS == 1975301/1908796 (L230) | REPLACE: cone sums `{'pos': 1965700.0, 'neg': 1899373.0}` |
| 7 | STEP 1 shape/DV-identity/binmap asserts (L191–215) | UNCHANGED (hold on the cone; verified §H.7) |
| 8 | STEP 2 VECTORS_NPZ exactness ≤1e-9 (L257–268) | DROP — no-cone gate-suite npz; replaced by #12 (release reproduction) |
| 9 | STEP 2 IBUNAT/CAND campaign reproduction ≤1e-12 (L271–291) | DROP — no cone ibu_native campaign; the cone-central legs are PRIMARY; anchors #10/#13 |
| 10 | STEP 2 CENTRAL_IBMED 11.483372/11.190383 (L292–294) | REPLACE: CONE_CENTRAL_IBMED `{'pos': 11.449320, 'neg': 11.155372}` (§H.7) |
| 11 | STEP 2 central model/reg convention reproduction ≤1e-12 (L297–311) | DROP — no cone equivalents; conventions enforced by construction + the exactly-once ledger |
| 12 | STEP 2 variant release reproduction ≤1e-9 (L313–321) | REPPOINT: vs the cone release CSV — PRE-VALIDATED read-only at ≤1.8e-15 (§H.7) |
| 13 | STEP 2 VARIANT_IBMED + G10 + SHIFT (L322–330) | REPLACE: CONE_VARIANT_IBMED `{'pos': 11.412233, 'neg': 11.123626}`; CONE_VARIANT_SHIFT_INTEGRATED `{pos: −0.003239238509156861, neg: −0.002845788385763881}`; the G10 npz comparison DROPS (the cone shift vector becomes a recorded output) |
| 14 | (NEW) secondary leg anchors | NEW: the six remaining legs pinned at round-6 (§H.7: dj_c_coh 11.515830/11.227196, k3_c 11.446691/11.153025, k5_c 11.450400/11.156371, dj_v_coh 11.473250/11.190520, k3_v 11.409644/11.121270, k5_v 11.413289/11.124629) |
| 15 | STEP 3a sidecar consistency ≤1e-9/≤1e-12 (L340–353) | REPPOINT: the cone sidecar CSVs; tolerances UNCHANGED |
| 16 | STEP 3b B1 floor mapping vs BUDGET_CSV ≤1e-12 (L355–369) | UNCHANGED — both operands are the CARRIED B1JSON/BUDGET_CSV; the check pins the CARRY MAPPING (§H.5-bis) |
| 17 | STEP 3b model component construction (L370–381) | UNCHANGED LOGIC — cone legs; carried floors; binding fractions recorded |
| 18 | STEP 3d shifted-DV load + variant data-side component (L405–427) | REPPOINT: the 5 cone shifted DVs |
| 19 | STEP 3d DETCAMP machinery reproduction ≤1e-12 (L428) | DROP → REPLACED by the §H.2 acceptance re-assertions in-machinery (schema; per-bin subset vs the legacy shifted DVs, EXACT; removed-fraction window [0.35, 0.65]%) |
| 20 | STEP 3d DETCAMP nominal-vs-candidate ≤1e-12 (L429) | DROP — no cone coherent detector campaign exists or is needed (data-side convention); the cone-central legs are the internal reference; the central data-side diagnostic stays REPORT-ONLY |
| 21 | STEP 3e seed ensemble component (L444–463) | REPPOINT: the 16 cone seed DVs; ensemble-mean construction UNCHANGED |
| 22 | STEP 3e TRKCAMP machinery reproduction ≤1e-12 (L473) | DROP → REPLACED by the §H.3 acceptance re-assertions in-machinery |
| 23 | STEP 3f phi recipe machinery reproduction ≤1e-15 (L535) | UNCHANGED — both operands are the CARRIED PHICAMP CSVs; the check pins the CARRY MAPPING (§H.5-bis) |
| 24 | STEP 3f variant re-expression (L500–534) | UNCHANGED LOGIC — the carried flat-leg absolutes divided by the CONE nominal per bin (additive separability; removal shift transferred OUT) |
| 25 | STEP 3g c_rad V3 recompute + budget drift ≤1e-15 (L570, L580) | UNCHANGED — gen-level tables, cone-invariant (standing gen-only ruling) |
| 26 | STEP 4 parentage SPEC asserts ≤1e-15 (L594–598) | UNCHANGED — PARENTAGE is gen-level truth classification; cone-invariant |
| 27 | STEP 4 G9 growth reproduction vs GATEREPORT ≤1e-9 (L613–630) | REPLACE: cone-INTERNAL identities — per-row denominator invariance rowsum(B_v)+F_v == rowsum(B_c)+F_c ≤1e-8 (read-only ≤1.4e-9) and per-bin f_i identity f_i_v = f_i_c + D/(B_c+F_c) ≤1e-12 (read-only ≤1.1e-16) — PLUS the pinned cone references at ≤1e-9: RG growth +0.6245835888590197 pp (pos) / +0.5612091043738465 pp (neg); DJ +0.6863208448106516 / +0.6155459135576747; dj_over_rg 1.0988454660879141 / 1.0968209687981685 (§H.7) |
| 28 | STEP 4 G12 sumw2 cross-checks vs GATEREPORT ≤1e-6 (L703–723) | REPLACE: cone-INTERNAL w2 partition closure — the category w2 vectors (fc0+fc3r+fc3p+kpf) == the w2-matrix fake column per reco row, ≤1e-9 relative (read-only ≤1.05e-15) — PLUS pinned cone totals at ≤1e-6: fakecol_sumw2_total 14640397.30909059 (pos) / 13603616.799681988 (neg); fc3_sumw2_total 462979.3219727144 / 402267.9972327622 (§H.7) |
| 29 | STEP 4 keeper envelope (L675–700, report-only) | UNCHANGED — the cone npz carries `h_keeper_retained_body_*` |
| 30 | closure/hierarchy/finite/lumi/dominance/soft gates (L813–835, L865, L906–914) | UNCHANGED — component-level identities; the central template pinning operates on the CARRIED BUDGET_CSV |
| 31 | PKG non-existence + schema/header asserts (L176, L1079–1080) | REPPOINT: the new campaign package dir; structures UNCHANGED (490×43) |

**Count: 31 gates — 10 REPPOINT, 6 REPLACE (incl. 1 NEW anchor set), 6 DROP
(each with a named replacement or a documented void), 9 UNCHANGED** (row
#13 additionally DROPS the G10 npz comparison inside a REPLACE row). The
canonical no-cone builder remains the reference implementation; the cone
edition's diff is exactly this table plus the input map.

**§H.5-bis — the two carry conventions (prescribed; both were flagged
unspecified).**

- **B1-floor carry (gate #16):** the P1-replica B1 floors
  (`p1_replica_floors_coherentprior_2026-10-07/ensemble_b1/b1_ibu_model_floors.json`)
  are per-gen-bin model-component floors derived on the NO-cone central
  chain. They are CARRIED UNCHANGED and applied per gen bin to the cone
  model component as `max(unfloored_cone_halfdiff, floor)` — the SAME carry
  the no-cone S2′ 15-comp applied from the central chain onto the variant
  chain (no variant B1 replica campaign existed there either). A cone B1
  replica campaign is NOT required and NOT part of this production. The
  floor-mapping gate (#16) stays AS-IS: both its operands are carried
  no-cone artifacts, so it validates the carry mapping, not cone physics.
  Floor-binding fractions are recorded (no-cone S2′ binding: 61.2%/66.5%
  of bins).
- **phi_h carry (gates #23/24):** the per-generator ABS flat-leg deltas (nb)
  from `phi_h_coherentprior_2026-10-06/per_generator_deltas.csv` are
  CARRIED UNCHANGED as generator-model absolutes — the flat-vs-nominal
  cross-section difference is a generator-physics effect, and the no-cone
  S2′ 15-comp carried them the same way onto its variant nominal
  ("additive separability; removal shift transferred OUT"). On the cone
  base they are re-expressed per bin against the CONE S2′ nominal
  (`d_rel = |flat − nom_c| / v_cone`), then the recipe is applied
  IDENTICALLY (5-bin pT-row Gaussian smoothing, max over generators, half
  envelope, 0.85× unsmoothed floor). The machinery gate (#23) stays AS-IS
  (carried operands). A cone phi_h flat-leg campaign is NOT required.

**Stage-2 machinery note:** the bgsub build scripts
(`sidis_highQ2/scripts/systematics/bgsub_setr_build_2026-10-10/`) run from
dated cone-edition copies with the Rev-3 §C/§F input repoints ALREADY
specified; the ONE additional Stage-2 re-scoping prescribed here is the
budget-extender parent: `extend_budget_13to17_v1.py` hard-asserts the
no-cone CENTRAL 13-comp budget (491 lines × 35 fields) as parent — the cone
edition (`extend_budget_15to19`) must take the CONE 15-comp package CSV
(490 rows × 43 fields, FULL_HEADER) as parent, assert its structure and
the CP-5 byte-identity, and extend with the four bgsub components to the
19-comp quadrature identity (≤1e-12) per Rev-3 §E/§F. The gate runner's
CP-5 consumption follows the budget JSON produced by that edition.

### H.6 Ordered production plan (the re-dispatch)

All stages run via Condor (login-node policy unchanged); smokes per
workspace policy; every stage's acceptance is a STOP condition. Campaign
tree: `output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/`
(the blocker-record tree; subdirs `scripts/`, `shifted_dvs/`,
`tracking_cone/`, `stat_sidecar_campaign/`, `systematics_package/`).

- **P0 — scaffolding (no builds):** dated cone-edition script copies (the
  re-scoped 15-comp builder per §H.5; the §H.2 wrap/submit clones; the
  §H.3 ported tracking builder; the §H.4 sidecar trio; the §H.5-bis
  Stage-2 extender edition). ACCEPTANCE: copies exist; the cone-edition
  shifted builders re-pass their self-tests (11/11, 13/13); the ported
  tracking builder passes a single-file smoke on one central sidistree
  file (P=0 → per-bin content ⊆ the campaign cone DV's file-level
  contribution; counters stamped); the re-scoped builder's constant block
  matches §H.5/§H.7 exactly (dry-read; no run).
- **P1 — 5 cone shifted DVs + zero-shift anchor (6 jobs; independent of
  P2/P3):** ACCEPTANCE: §H.2 (a)–(d) per leg + the zero-shift anchor
  bit-identity to the campaign cone DV.
- **P2 — tracking: port validation + 16 seeds (17 jobs; independent of
  P1/P3):** ACCEPTANCE: the P=0 anchor bit-identity; §H.3 (a)–(c) per seed;
  `ensemble_manifest.json` written with seeds 314159–314174.
- **P3 — cone 1000-toy sidecar (2 jobs + assembler; independent of
  P1/P2):** ACCEPTANCE: §H.4 (1000 toys/charge; nominal cross-check ≤1e-10;
  closure stop condition untriggered; additive install isolated — manifest
  `6ca6b4a1…` / tarball `b16424f0…` unchanged; one-charge re-run bitwise).
- **P4 — the cone 15-comp budget build (1 job):** the re-scoped builder.
  ACCEPTANCE: ALL 31 §H.5 gates green; legs positive; components finite,
  non-negative; quadrature closure ≤1e-12, hierarchy 0 violations; the
  package written (490×43 CSV + JSON + contract + validation + provenance +
  the report-only comparison vs the no-cone S2′ 15-comp package); medians
  inside the §H.1 sanity windows; full-precision scalars recorded.
- **P5 — Stage 2, the Set R′ chain (the original dispatch's Stage 2):** the
  Rev-3 §C/§G productions (B_R′_cone vector build → cone-variant re-unfold
  → C_rad → candidate `born_crad_bgsub_setRprime_conebase_...` → budget
  15→19 via the §H.5-bis extender edition). ACCEPTANCE: the Rev-3 §F
  26-item gate (IV-1..IV-7 + SA-1..5 + RU-1..5 + CP-1..5 + PM-1..4), with
  CP-5 consuming the P4 package byte-identically.

P1, P2, P3 are mutually independent and may run concurrently; P4 requires
all three; P5 requires P4. Estimated totals: 24 Condor jobs + the P4 build
+ the Stage-2 chain.

### H.7 Pinned reference values (read-only computed 2026-10-11; full precision in the JSON v4 `production_preconditions_h.pinned_constants`)

- **Cone release guards:** manifest sha256
  `6ca6b4a148e3b83702440c75800045e31beb6201fdb2fd48cdd0f95ae87aa8d3`;
  tarball sha256
  `b16424f0f048880178ebf05573765cb094f1655d8bd4e508e35da5d4ca0c711e`;
  cone DATA_SUMS {pos 1965700.0, neg 1899373.0}.
- **The eight cone legs (integrated nb; the 15-comp machinery conventions —
  load_response_and_data + derive_response_blocks + body+miss prior +
  ibu_unfold):** ibu_c 11.449320/11.155372, dj_c_coh 11.515830/11.227196,
  k3_c 11.446691/11.153025, k5_c 11.450400/11.156371, ibu_v
  11.412233/11.123626, dj_v_coh 11.473250/11.190520, k3_v
  11.409644/11.121270, k5_v 11.413289/11.124629 (pos/neg; round-6). The
  variant legs reproduce the cone RAW release CSV at ≤1.8e-15 (nb and
  yield) — the re-pointed reproduction gate is PRE-VALIDATED. Cone
  integrated shift (S2′ vs central): −0.3239238509156861% /
  −0.2845788385763881% (no-cone: −0.3239331%/−0.2844285% — the detint
  removal shift is nearly cone-invariant, a physical consistency note).
- **G9/G12 cone operands:** RG growth +0.6245835888590197 pp /
  +0.5612091043738465 pp; DJ +0.6863208448106516 / +0.6155459135576747;
  dj_over_rg 1.0988454660879141 / 1.0968209687981685; denominator
  invariance ≤1.4e-9; f_i identity ≤1.1e-16; fakecol_sumw2_total
  14640397.30909059 / 13603616.799681988; fc3_sumw2_total
  462979.3219727144 / 402267.9972327622; w2 partition closure ≤1.05e-15.
- **Legacy (no-cone) shifted-DV sums (acceptance references):**
  hfs_jes_down 1,976,086/1,909,781; hfs_rces_up 1,981,241/1,914,687;
  hfs_theta_up 1,981,299/1,914,724; elecE_p1pct 1,996,529/1,930,006;
  elecTheta_p1mrad 1,976,517/1,909,855 (pos/neg; 490 rows per charge).
  Legacy total changes vs the no-cone nominal: §H.2(d). Per-leg window
  centers for the cone removal: hfs_jes 0.4859/0.4934%, hfs_rces
  0.4846/0.4921%, hfs_theta 0.4846/0.4921%, elecE 0.4809/0.4882%,
  elecTheta 0.4858/0.4934%.
- **Legacy seed-DV sums (16 seeds; acceptance references):** per-seed sums
  in the JSON; ensemble means 1,965,567.4375/1,899,376.3125; legacy drop
  fractions 0.49276%/0.49349%; expected cone-seed removed fractions
  ≈0.4837%/0.4912% (window [0.35, 0.65]%). Nominal cone removal: 9,601
  (pos) / 9,423 (neg) in-grid = 0.4861%/0.4937% of the no-cone nominal.

**Computation provenance (§H):** read-only scratch scripts
`/tmp/opencode/cone_hlegs_computation.py` (the eight legs; the pinned
engine `crosscheck_unfold_methods_v4` from
`scripts/unfolding/tunfold_official_v4_percharge`) and
`/tmp/opencode/cone_g9g12_probe.py` (G9/G12 operands) + a legacy-sums
probe; outputs retained in `/tmp/opencode/`; the load-bearing scalars are
embedded above and in the JSON v4 (the scratch copies may not survive /tmp
cleanup).

## I. Source index (primary artifacts, read-only)

- Cone-base campaign (the Rev-3 basis):
  `H1ChargedSidis_HighQ2/output_v6/studies/cone_alpha_central_build_2026-10-11/`
  — RAW release `production/release_driver/bbb_ibu_cone_alpha_central_..._2026-10-11/`;
  Born release `production/release_driver/born_crad_dj14relbin_cone_alpha_central_..._2026-10-11/`;
  data vector `production/data_vector_cone_alpha_central_2026-10-11.json`;
  cone S2′ responses `production/migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.{root,npz}`;
  f_i crosschecks `production/f_i_crosscheck_cone_*.json/csv`; QA sums
  `production/{central,s2prime}_shards_alpha_qa_sum.json`; Phase-F rerun
  record `production/qa_tools/check_phase_f_rerun_outputs.json`; logs
  `docs/PRODUCTION_LOG_2026-10-11.md`, `docs/BORN_LOG_2026-10-11.md`
- Guard adjudication: `sidis_highQ2/docs/qa/cone-missrow-guard-adjudication-2026-10-11.md`
- Cone impact study (predictions verified): `sidis_highQ2/docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md`
- Cone-vs-no-cone comparison (Deliverable 1): `sidis_highQ2/docs/results/cone-vs-nocone-comparison-2026-10-11.md`
- Critic gate record (the Rev-2 trigger): `sidis_highQ2/docs/critiques/critic-gate-bgsub-promotion-2026-10-11.md`
- Established ledger: `sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`
  (§6–§11.4) + JSON twin in `sidis_highQ2/data/backgrounds/`
- Normalization verification: `sidis_highQ2/docs/status/normalization-verification-bgfracsub-2026-10-10.md`
- S2′ spec: `sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md`
  (Rev 2 §0; §3 exactly-once ledger + "full future chain")
- S2′ gate suite: `H1ChargedSidis_HighQ2/output_v6/studies/detint_variant_s2prime_2026-10-09/validation/gate_suite_G1_G13_report_2026-10-10.md`
- No-cone S2′ releases (superseded as re-base target; canonical-installed):
  `output_v6/central/data_release/bbb_ibu_detints2prime_..._2026-10-09/`,
  `born_crad_dj14relbin_detints2prime_..._2026-10-10/`,
  `systematics_detints2prime_15comp_..._2026-10-10/`
- Electron templates (consumed as packaged): `output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/AGGREGATED_{rapgap31,djangoh14}_electron_alpha0p2_combinedL_L351000_finalcandidate.npz`
  (sha256 `a04dd1a6…` / `66141a54…`)
- B_R vectors (V0/lowQ2 part, bit-identical carry): `output_v6/studies/bgsub_setr_build_2026-10-10/vectors/bgsub_setr_vectors_{980,490}.csv`
- Parentage campaign: `output_v6/studies/detint_parentage_2026-10-08/detint_parentage_results_2026-10-09.json`
- G9/S1b category ratios (r proxy basis): `output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/validation/template_vs_fakecolumn_report.json`
- C_rad applied table: `sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`
- Pre-S2′ bgsub spec + study (provenance): `sidis_highQ2/docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md`;
  `output_v6/studies/bgsub_setr_build_2026-10-10/`
- Computation provenance (this revision): read-only scratch scripts
  `/tmp/opencode/cone_rev3_computation.py` (+ extension), outputs retained;
  engine `crosscheck_unfold_methods_v4.derive_response_blocks` (pinned),
  `common_bgsub_setr` (pinned conventions; PT_PAIRS pairing, guards);
  full-precision scalars in the JSON sidecar (§verification_provenance).
- §H extension sources (2026-10-11): Stage-1 blocker record
  `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/BLOCKER_RECORD_2026-10-11.md`;
  canonical 15-comp machinery
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_detints2prime_15comp_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/`
  (README + `scripts/run_s2prime_systematics_rebase.py`); sidecar machinery
  `H1ChargedSidis_HighQ2/output_v6/studies/detint_variant_s2prime_2026-10-09/stat_sidecar_campaign_2026-10-10/scripts/`;
  tracking study
  `H1ChargedSidis_HighQ2/output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/`
  (randdrop builder + 16 seed productions); legacy shifted-leg trees/vectors
  `H1ChargedSidis_HighQ2/output_v6/systematics/{hfs,electron}/...` and
  campaign references `{ibu_native,detector_shifted,tracking_ensemble,phi_h}_coherentprior_2026-10-0{6,7}/`;
  paired-legs protocol `sidis_highQ2/docs/systematics/paired_legs_s2prime_protocol_2026-10-11.md`
  (the P2 12-leg program is SEPARATE — the cone 15-comp mirrors the
  data-side convention and needs NO shifted responses);
  §H computations `/tmp/opencode/cone_hlegs_computation.py`,
  `/tmp/opencode/cone_g9g12_probe.py` (read-only; scalars embedded in the
  JSON v4 `production_preconditions_h.pinned_constants`).

---

## J. Clean-release single-denominator budget re-derivation (user decision 2026-10-16)

**Status:** ADDITIVE NOTE to this addendum (no prior section is rewritten).
Machine twin: the third entry of `adoption_addenda` in
`H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/stage2_setrprime_2026-10-15/bgsub_setrprime_cone_spec_2026-10-15.json`
(id `clean_release_single_denominator_rederivation`, dated 2026-10-16).

**User decision (2026-10-16):** the published 19-component budget is
re-derived as a CLEAN RELEASE on a SINGLE denominator: every component is
expressed relative to the bgsub (Set R′) subtracted nominal — the
`official_reunfold` IBU k=4 result on `data_sub′` (RAW level; C_rad is
multiplicative and bitwise per bin, so relative components are
level-invariant). This supersedes, as the quoted-budget basis, the
mixed-denominator frozen carry (`budget_15to19`: the 15 legenv cone
components carried byte-identical from the RAW-cone-basis parent package +
the 4 bgsub components). `budget_15to19` is retained
superseded-not-modified as the frozen-carry record (totals
5.8242%/6.2078%).

**Rationale (loop-1/loop-2 critic adjudications 2026-10-16):** the 15
carried components divide by RAW-cone unfolded denominators, ~×1.086 larger
than the subtracted-basis values (integrated f_res′ 7.9001%/8.1267%
RAW-yield-weighted), so the frozen-carry total understates the clean-release
total by ~+0.1–0.5 pp. Uniform re-expression brackets the totals at
h+ ≈6.2–6.3% / h− ≈6.65–6.75%; per-component re-derivation may land lower
(+0.1–0.2 pp on medians, high-f_res′ tails larger). Records:
`docs/systematics/bgsub_s4_gate_adjudication_2026-10-16.md` (loop-1
persisted record) + the in-session critic loops of 2026-10-16 (loop-2
rulings: lumi NEVER rescaled — multiplicative, denominator-invariant;
c_rad_gen/c_rad_stat carried — level-invariant, dev 0 across all prior
budgets; elecE leg-level envelope convention; the CP-5 re-definition).

**CP-5 re-definition (clean release):** budget closure verifies (a) each
RE-DERIVED component against its own re-derivation record (the
re-derivation campaign's per-component products, sha-pinned in the budget
provenance — NOT byte-identity vs the legenv package); (b) each CARRIED
component against its pinned source bit-identically; (c) the 19-component
quadrature identity per bin (≤1e-12); (d) medians reported; (e) the quoting
switch recorded. The original CP-5 text in the spec is NOT rewritten; the
budget_15to19 CP-5 closure PASS remains valid FOR THAT PACKAGE.

**Component dispositions (19).** RE-DERIVED (12): `ibu_stat` (regenerated
1000-toy sidecar on data_sub′, seeds 12345/22345, Poisson-only
w2 = data_sub′); `ibu_model` (RG vs DJ COHERENT half-diff on data_sub′;
carried central B1 floor rule); `ibu_reg` (k5/k3 half-diff on data_sub′);
`hfs_jes`/`hfs_rces`/`hfs_theta`/`elec_energy`/`elec_theta` (Set R′
subtraction applied to each existing shifted DV — same sha-pinned
cone-applied templates AS-IS, same fine-level guard, same e∩V0
overlap-ignore ruling — own shifted response + own coherent prior + own
f_i, IBU k=4; elecE leg-level envelope over plus/minus legs);
`tracking_eff_m0p5` (data-side conservative form KEPT, caveat OPEN,
re-denominated on the subtracted basis); `phi_h_gauss` (carried central ABS
deltas / subtracted-basis nominal; additive separability);
`detint_removal` (removal-increment isolation recomputed on the subtracted
chain + cone parentage stat); `fakecol_mcstat` (G12 fc3 sigma through the
IBU k=4 Jacobian on the subtracted basis). CARRIED (7): `lumi` (2.7% flat;
never rescaled); `c_rad_gen`, `c_rad_stat` (level-invariant); `bgsub_stat`,
`bgsub_model`, `bgsub_stage`, `bgsub_fakefrac` (already bgsub-denominated in
budget_15to19; carried bit-identical).

**Double-count watch (resolved):** the regenerated `ibu_stat` (unfolding
data-Poisson toy statistics, w2 = data_sub′) and `bgsub_stat` (template-MC
variance of B_R′) are DISTINCT by the SA-3 exactly-once Poisson policy —
no double-count.

**Products:** `budget_19comp_cleandenom_2026-10-16/` (single-denominator
19-comp package: 43+-col CSV mirroring the legenv/budget_15to19 layout,
JSON, contract, 29-check validation suite + physical bounds 490/490,
provenance with all cluster chains, comparison JSONs vs budget_15to19 and
vs the legenv parent); then a NEW dated 26-item gate edition evaluated
against the clean-release budget (prior editions retained
superseded-not-modified; no criterion weakened — any FAIL is a STOP).

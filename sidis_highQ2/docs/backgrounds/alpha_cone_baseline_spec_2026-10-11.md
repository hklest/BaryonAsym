# Alpha-cone (α ≥ 0.2 rad) BASELINE reco-selection specification — v6/z005 high-Q2 inclusive charged-hadron SIDIS, on the S2′ detint-fake basis

- **Review status (2026-10-11):** reviewed 2026-10-11 by the analysis critic —
  verdict **SOUND-WITH-CONDITIONS**; the three FIXABLE-NOW text edits
  (truth-conservation two-level identity with the NucliaWeight-effect warning,
  measured cone hot-bin locations replacing the low-z/low-pT claims, C2
  consumed-elsewhere hardening with the rerouted-key zero-collision assertion) are
  applied in this revision. The DEFERRED-TO-CAMPAIGN items — C4 window pinning,
  C3/C5 register pinning, DJANGOH14 501-vs-502 shard-list resolution, retired
  4.1441% figure labeling, and the cone parentage re-slice alongside the campaign —
  are execution-time register pinning, not spec text, and are to be pinned in the
  Phase 0 acceptance register at execution.
- **Spec date:** 2026-10-11. **USER-DECIDED 2026-10-11** — the lab-frame
  `alpha(track, scattered electron) ≥ 0.2 rad` cone is adopted as the **BASELINE reco
  selection** of the v6/z005 high-Q2 charged-hadron SIDIS chain, on top of the promoted
  S2′ detint-fake response basis. This spec encodes the convention, the construction, the
  exactly-once ledger, the gates, the production plan, and the quoting rule. It does not
  reopen the S2′ rulings (spec
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md`
  Rev 2 Sec 0, USER-DECIDED 2026-10-09, PROMOTED 2026-10-10).
- **Nothing was rebuilt, launched, or centrally touched by this spec.** No central or
  diagnostic product was modified. The single deliverable is this file, plus ONE
  append-only supersession pointer note added to the S2′ spec's DO-NOT-PORT list
  (Sec 0.4 below; no existing text of that spec was altered).
- **Scope:** define the cone as baseline — the exact alpha variable and threshold, its
  placement in the selection order, the response/data-vector/prior construction changes,
  the updated exactly-once ledger, the mandatory validation gates, the production plan,
  the extended quoting rule, and the open questions. Luminosity `L_data = 351000.0 nb^-1`
  throughout (never 356613.0 or 297657.8).
- **Companion execution document (operational, already written):**
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/plan_alpha_cone_central_imposition_2026-10-11.md`
  (phases, shard layout, trap list, post-promotion cascade). This spec is the
  physics/observable-definition authority; where both speak, this spec owns the
  convention and the plan owns the execution mechanics. Gate-id mapping in Sec 5.7.
- **Inputs consumed (authoritative, all read-only):**
  - S2′ spec Rev 2 (current central convention): Sec 0 rulings, Sec 2.1 construction,
    Sec 2.2 miss-fill suppression, Sec 3 exactly-once ledger, Sec 4 gates G1–G13.
  - `STATE.md` "2026-10-10 S2′ detint-fake Promotion Override" (current release facts,
    10-item remaining-work ledger).
  - Cone impact study 2026-10-11:
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md`
    (measured per-class in-cone evidence, integrated impact, rebuild chain; its §6
    recommendation is superseded by the user decision — Sec 0.2 below).
  - Alpha0p2 handoff 2026-06-28:
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`
    (cone numbers, all-fake rules, L351000 template totals).
  - Consistency ledger 2026-10-07 (+ JSON sidecar):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`
    (cone counters, per-category ledger, PROVISIONAL register).
  - Background provenance closure 2026-10-05:
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/critiques/v6_background_provenance_critic_loop_2026-10-05.md`
    (lab-frame alpha = intended physics, Grebenyuk precedent, never touched by the
    2026-05-24 frame bug).
  - Cone measurement provenance:
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/provenance/validation_alpha0p2_final_summary.json`
    (data and MC alpha counters).
  - Backgrounds skill facts:
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/.opencode/skills/sidis-hq2-backgrounds/SKILL.md`.

---

## 0. Decision record (USER-DECIDED 2026-10-11)

### 0.1 The decision

**USER DECISION 2026-10-11 (verbatim): "The cone cut must be baseline. We know we have
many fake tracks from the scattered electron."** The α ≥ 0.2 rad lab-frame
scattered-electron cone becomes the **BASELINE reco selection** of the central chain —
data vector AND response reco legs, cone-coherent by construction — on top of the
promoted S2′ detint-fake basis (enlarged per-reco-bin `f_i`; removable fraction
`0.909920 ± 0.000269`; charged-K DIF `k_parented` keeper, mother PDG ±321 only). The
S2′ construction is preserved in full; the cone is an ADDITIONAL selection-convention
(observable-basis) change.

### 0.2 Motivation and its evidence base

The motivation is the fake-track population from the scattered-electron region. The
measured evidence base (all pre-existing artifacts; nothing re-derived here):

- **Data-side in-cone removal:** 28,582 of 3,902,480 in-PS data tracks (0.7325%);
  19,024 of 3,884,097 in-grid (0.4897%) — direct measurement on the same 658 central
  data trees (`validation_alpha0p2_final_summary.json` `.data_vector`; ledger §4 counter
  cross-check `alpha_pre = 3,902,480 = central n_tracks_in_ps`, exact parity).
- **Data/MC in-cone excess:** MC predicts only ≈11.0k (RG) / ≈12.2k (DJ) lumi-scaled
  cone-rejected tracks at `L_data = 351000.0 nb^-1`; data has 28,582 — an integrated
  excess of **×2.3–2.6**, with independently validated per-bin excesses of ×1.8–2.6 in
  hot bins (cone impact study §3.3; `docs/status/alpha0p2-background.md`). The worst-bin
  MC removed population is 96–99% true e±, so the excess is plausibly electron-induced
  data tracks (unmatched fragments etc.) that the MC templates/response do not describe
  at the measured rate — exactly the user's "many fake tracks from the scattered
  electron". The cone excises this population wholesale, in the region where the `f_i`
  closure assumption (ledger §9.1) is weakest.
- **Targeted per-bin cleanup:** data track-level removal reaches ≈36.6% in the worst
  measured data-removal bin — **global bin 24 = h+ S0x0, z∈[0.2,0.3), pT∈[1.0,1.5)**
  (v3 binmap decode; measured under α<0.1, so α<0.2 removal is ≥ this; next bins
  ≈33.7/33.7/30.7%; cone impact study §2) — and a direct per-bin check of the central
  vs alpha0p2 vectors puts the worst α≥0.2 removal at **58.6% in h+ reco bin 49**
  (2026-10-11 critic correction: the hot bins are at the MEASURED locations — worst
  bin at z∈[0.2,0.3), pT∈[1.0,1.5), NOT at low-z/low-pT) — the cone is a targeted
  per-bin electron-region cleanup concentrated where the measured hot bins are.

### 0.3 Relationship to the 2026-10-11 cone impact study

The impact study's §6 recommendation ("do NOT impose the cone as a background-reduction
measure") is **SUPERSEDED by this user decision**. The study's own closing clause
governs the adoption path: "If the cone IS adopted (e.g. for Grebenyuk-precedent
alignment), it must be sold as a **selection-convention change**" with the full §5
re-derivation chain executed on the S2′ basis — which is what this spec specifies. The
study's measured magnitudes (its §2/§3) and rebuild chain (its §5.1/§5.2) remain the
authoritative quantitative inputs and are cited throughout below. The study's verdict
that the cone is NOT a dramatic integrated background reduction (≤0.49% of the data
vector; electron-class share ≈0.30 pp of a ≈4% class) stands as measured context — the
decision adopts the cone as a **selection convention** (Grebenyuk-precedent alignment +
template-consistency closure + excision of the measured data/MC-excess region), not as
the solution to the background budget.

### 0.4 Supersessions issued by this decision

1. **S2′ spec Sec 2.1 DO-NOT-PORT entry "the alpha cone (prototype `alpha_ok` /
   `ELECTRON_ALPHA_VETO_RAD`) — central is no-cone" is SUPERSEDED.** The cone becomes
   TO-PORT into the S2′ builder lineage. A dated append-only supersession pointer note
   was added 2026-10-11 at the top of that spec's DO-NOT-PORT list (the only permitted
   edit; the S2′ spec body and the original S2′ builder file
   `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
   stay untouched as provenance). The full STATE.md supersession cascade happens at
   promotion time, not by editing historical documents.
2. **The other two S2′ DO-NOT-PORT entries REMAIN IN FORCE:** (a) the e/V0 truth-veto
   disablement (prototype lines 685-693) — the central e/V0 exclusion stays
   (`not_e`, `not_v0_match_top` remain in the reco selection); (b) the miss-row fill for
   reclassified det.int. rows (prototype 1043-1066) — the S2′ miss-fill suppression
   (spec Sec 2.2, R1 SIGNED OFF) stands unchanged. Only the cone entry reverses.
3. **The 2026-10-11 impact study §6 recommendation** is superseded (Sec 0.3 above).
4. **NOT superseded:** the alpha0p1 status (comparison/provenance only — Sec 8.2), the
   DIFFVM systematic-only ruling (2026-10-07), the e∩V0 overlap-ignored decision
   (2026-10-08), the reco-only-C_rad standing ruling, and every S2′ ruling.

### 0.5 Status discipline

SPEC ONLY. The cone basis does not exist as a product until the Sec 6 campaign is
built, the Sec 5 gates pass, and a separate user promotion installs it. Until then the
current canonical basis remains the no-cone S2′ release
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
(integrated IBU h+ `11.446174 nb` / h− `11.158554 nb`; response-internal RG `f_i`
`11.2403%`/`10.8737%`, data-weighted `11.3577%`/`10.9768%`), and every interim cone
number is PROVISIONAL CANDIDATE, never "current" (Sec 7).

---

## 1. Cone definition and placement

### 1.1 The alpha variable (ESTABLISHED; provenance-closed)

- **Definition:** `alpha` is the LABORATORY-FRAME 3D opening angle between the
  reconstructed charged-track momentum and the scattered-electron direction:
  `alpha = arccos( clip( (Px·sEX + Py·sEY + Pz·sEZ) / (|p|·|p_e|), -1, 1 ) )`, computed
  downstream of the tree builder from the stored branches `Px, Py, Pz` and `scatEX,
  scatEY, scatEZ`; where either norm is 0 the reference formula yields NaN
  (`cos_alpha = np.where((trk_norm > 0) & (ele_norm > 0), ..., np.nan)`).
- **Reference implementation (authoritative, port VERBATIM):** the all-fake prototype
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_allfake.py`
  — `ELECTRON_ALPHA_VETO_RAD = 0.2` (line 387), `_compute_reco_electron_alpha`
  (lines 391-398); identical formula in
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py`
  (lines 363-371, constant line 113) and in the alpha0p2 v4 migration builder
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py`
  (constant line 386, formula lines 390-396).
- **Frame and intent (ESTABLISHED WITH STATED LIMITS, three-reviewer loop 2026-10-05):**
  the lab-frame cone is intended physics — a detector-geometry near-electron veto
  following the H1 Grebenyuk charged-particle precedent (track–scattered-electron angle
  > 0.2 rad) — and the background chain was never touched by the 2026-05-24 pT_h/phi_h
  lab-frame bug. Record:
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/critiques/v6_background_provenance_critic_loop_2026-10-05.md`.
- **Threshold:** veto tracks with `alpha < 0.2 rad`; keep `alpha ≥ 0.2 rad`.
- **NaN handling:** `alpha_ok = np.isfinite(alpha) & (alpha >= ELECTRON_ALPHA_VETO_RAD)`
  (prototype line 715). A NaN/inf alpha FAILS the cone — the track is removed. (Finite
  fraction ≈ 0 expected; asserted at the Phase 0 smoke.)

### 1.2 Placement in the selection order

In the S2′ builder lineage the reco selection is currently
`pass_reco_full = evt & trk & not_e & not_v0_match_top` (builder line 867), followed by
the z-guard (`pass_reco_full &= ~z_outlier`, lines 884-885). The cone inserts BETWEEN
the e/V0 truth vetoes and the z-guard:

```
pass_pre_alpha  = evt & trk & not_e & not_v0_match_top   # == S2' pass_reco_full, pre-cone
alpha           = _compute_reco_electron_alpha(Px, Py, Pz, sEX, sEY, sEZ)
alpha_ok        = np.isfinite(alpha) & (alpha >= 0.2)
pass_reco_full  = pass_pre_alpha & alpha_ok              # prototype lines 714-716 pattern
pass_reco_full  = pass_reco_full & (~z_outlier)          # z-guard unchanged, after the cone
```

- The cone sits **AFTER** the reco-quality/event selection and the e/V0 truth vetoes,
  and **BEFORE** the z-guard, **BEFORE** matched/fake classification, and **BEFORE**
  the S2′ det.int. reroute (`s2p_reroute` / `matched_gen_ok`, builder lines 857-860,
  1021). Conditioning `pass_reco_full` means the cone conditions ALL reco fills
  uniformly — body, fake column, and reroute — which is what makes the response
  cone-coherent with the cone data vector.
- **Cone-rejected tracks leave the reco sample ENTIRELY:** no body fill, no fake-column
  fill, no reroute, no consumed-key recording (the fill loop skips them at its
  `pass_reco_full` guard — S2′ builder lines 1152-1158; prototype lines 973-979).
- The det.int. selection counters keep their PRE-cone definition
  (`pass_pre_z = pass_pre_alpha`, parentage-campaign-comparable) and gain NEW
  post-cone twins (Sec 2.4).

### 1.3 Reco-only guard (standing, unchanged)

- **GEN side untouched:** the `ChargedTotGen` pool, the gen fiducial, the gen-grid
  membership, and all gen-side weights are unchanged. The cone is a RECO-leg cut only.
- **`C_rad` untouched:** the standing ruling (skill; `docs/status/alpha0p2-background.md`)
  — the cone must NEVER be imposed on gen-only `C_rad`. The gen-level releasebin tables
  (applied primary
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/`;
  formal alternative
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/`)
  are invariant; per-bin Born shift == per-bin raw shift (S2′ precedent, gated ≤ 1e-12).
- **Stage-1 object is NOT this cone (trap):** `RejectTracksNearScatElec(0.2)` at line 394
  of `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`
  affects the HFS/hadronic-calibration object only, NOT tree admission (ledger §4 note;
  cone impact study §1; plan trap T4). Direct proof: the central data vector's
  `n_tracks_in_ps = 3,902,480` equals the alpha0p2 mask-off `alpha_pre` exactly, and the
  cone removes 28,582 data tracks at the aggregation level — those tracks ARE in the
  trees, so no upstream veto removed them. The 2026-10-06 audit
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/alpha0p2_application_audit_2026-10-06.md`
  (PROVISIONAL INTERNAL DIAGNOSTIC) claims the 0.2-rad veto is "applied upstream at v6
  tree production" — that claim is REFUTED by the counter parity above and is superseded
  by the ledger (2026-10-07) and the impact study (2026-10-11). Do not cite it as
  evidence that the central chain already has the cone. Do NOT touch the stage-1 call.
- **No tree re-production:** the cone is computable at the aggregation level from
  branches already in the v6 trees, on data AND MC (the parity measurement is the
  evidence). Stage-1 and Stage-2 builders are untouched; the 658 central data trees and
  the central MC trees (RAPGAP31 364 files, DJANGOH14 501 files) are reused read-only.
  If any tree-level change appears required: STOP — scope change requiring the user and
  the builder engineer.

---

## 2. Construction changes

### 2.1 (a) Response builder — cone port into the S2′ lineage

- **New dated script** (never an in-place edit; the original S2′ builder stays untouched
  as provenance): a clone of
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
  plus EXACTLY ONE additional physics delta — the cone port of Sec 1.2 — plus the new
  cone diagnostics (Sec 2.4). Campaign tag `alphaveto_cone_s2prime_<launch-date>`
  (plan §3). The S2′ delta list is preserved VERBATIM: the `is_detint` classifier
  (builder lines 845-851), the `k_parented` keeper (direct-mother `GenMotherPDG` ±321
  ONLY, lines 852-858), the matched-fill gate
  `matched_gen_ok = gen_candidate_ok & ~(is_detint & ~is_kpar)` (line 1021), the
  fake-column reroute of non-keeper det.int. legs with the SAME
  `w_reco = w1·w2·NucliaWeight` and GEN-charge dispatch, the category vectors
  (fc=3 rerouted non-keeper / fc=4 pre-existing fake non-keeper / fc=5 pre-existing fake
  keeper / fc=0 unflagged), and the per-reco-bin rerouted-weight diagnostics.
- **Cone + S2′ reroute composition:** the reroute applies ONLY to cone-surviving
  det.int. legs. In-cone det.int. legs (≈0.16% of the class; parentage cone survival
  ≈0.9984) fail `pass_reco_full` and drop out entirely — body and fake both lose them;
  the removable fraction is mother-lineage-based and cone-independent at the class
  level (Sec 4.4).
- **MISS-FILL SEMANTICS FOR CONE-REJECTED LEGS — the load-bearing statement.**
  Cone-rejected matched legs' gen partners **MUST flow to the miss row via the existing
  consumed-key machinery**. This is the OPPOSITE of the S2′ reroute case:
  - *S2′ reroute case (unchanged):* a rerouted leg IS reconstructed (it passed the reco
    selection) but is fake-classified; recording its consumed key SUPPRESSES the miss
    fill so the det.int. gen content vanishes from the truth marginal — that is the
    R1-signed miss-fill suppression, and it stays.
  - *Cone case (new):* a cone-rejected leg is NOT reconstructed at all — an ordinary
    selection loss, exactly like any reco inefficiency. Its gen key is never recorded,
    so the CTG miss pass (S2′ builder lines 1280-1303) fills its gen partner
    (`w_gen = w1·w2`). **The miss row MUST grow.** Do NOT suppress it; do NOT add a
    manual miss fill for it either (the ordinary CTG pass already handles it — a manual
    fill would double-fill).
  - *Prototype verification (how the cone-active prototype handled it):* the all-fake
    prototype's fill loop skips `pass_reco_full` failures before any fill or
    consumed-key recording (prototype lines 973-979), so cone-rejected matched legs
    leave their CTG partners unconsumed and the ordinary miss pass fills them — the
    prototype's miss accounting is "accepted nonfake reco matches only"
    (alpha0p2 handoff §6). The same mechanics carry into the S2′ lineage unchanged.
  - *Precise growth statement:* only cone-rejected matched legs whose gen partner is a
    valid in-pool GTR π/K/p (i.e., legs that were BODY rows in the no-cone S2′
    response) grow the miss row. Cone-rejected det.int. legs have no CTG partners
    (their inlined gen candidates are STR records; CTG is GTR-only — the same argument
    as S2′ spec Sec 2.2). Cone-rejected gen-fail fake legs have no in-pool partners. A
    gen partner consumed by ANOTHER matched leg in the same event stays consumed (no
    miss growth for that partner).
- **Consequence for the S2′ gate structure:** the S2′ G1(a) assertion "miss row
  bit-identical to central" is FALSE on the cone basis BY DESIGN and is redefined
  (Sec 5.2, C2). Truth conservation still holds BY CONSTRUCTION, stated as the
  precise two-level identity (2026-10-11 critic correction; the former single-line
  "`body + miss` == the unchanged gen-side CTG pool marginal" was imprecise at
  weight level): **(a) COUNTS are exactly conserved vs the unchanged gen-side CTG
  pool per gen bin, 245/245 per charge** — every in-pool gen particle lands in
  exactly one of body (matched to a cone-surviving leg) or miss (unmatched, or
  matched to a cone-rejected leg whose key is never recorded); the miss growth is
  precisely the mechanism that keeps the count identity exact under a reco-side
  loss. **(b) WEIGHTS: `body + miss` == the cone build's OWN `prior_gen_NWeff`**
  (body legs carry `w1·w2·NucliaWeight`, miss legs `w1·w2`), NOT the plain pool
  marginal; the no-cone→cone weight difference per gen bin = Σ over in-cone matched
  legs of `w1·w2·(NucliaWeight−1)` (no-cone minus cone; auditable from
  `--dump-cone-removed-legs`, ≈0 expected). **WARNING — the single most dangerous
  misimplementation:** an implementer comparing `body + miss` WEIGHTS against the
  PLAIN unchanged pool marginal will see the ~0.45% NucliaWeight effect (the NW
  factors on ALL matched body legs) and must NOT "fix" it by suppressing miss
  growth; the count identity is the exact one against the unchanged pool, and the
  weight-level comparison must be run against the build's own `prior_gen_NWeff`.
- **`f_i` algebra change:** per-reco-bin `B + F` is NOT invariant under the cone (both
  the body and the fake column lose in-cone content), unlike the S2′ reroute
  (`f_i_v = f_i_c + D_i/(B_c+F_c)` with invariant denominator — S2′ spec Sec 2.5).
  The S2′ G9 identity therefore does NOT carry; `f_i` is recomputed from the cone
  response's own `B` and `F` and gated by band (Sec 5.4, C4; plan G9′).

### 2.2 (b) DATA VECTOR — hard gate: identical cone selection

- The central data vector is currently NO-CONE (in-grid Σ 3,884,097; h+ 1,975,301 /
  h− 1,908,796). **A cone response with a no-cone data vector is INVALID** — the cone
  would remove data content (the near-electron region, up to ≈36%+ per bin) that the
  response still maps onto signal: the double-rejection geometry failure mode
  (provenance closure §4.6; the P-ALPHA core concern). Selection identity between the
  data vector and the response reco space is a HARD GATE (Sec 5.1, C1).
- The cone-applied data vector already exists in validated form — the alpha0p2 campaign
  built it from the SAME 658 central data trees with exact pre-cone parity
  (`alpha_pre = 3,902,480`): in-grid **3,865,073** (h+ 1,965,700 / h− 1,899,373);
  in-PS 3,873,898. Reference:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/data_vector_alpha0p2_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json`.
- **Rebuild required anyway** (dated clone `build_data_vector_v4_alphaveto.py` from the
  CENTRAL vector builder
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_data_vector_v4.py`
  + the cone port, plan §5): the rebuild must reproduce the alpha0p2 vector EXACTLY
  (totals AND per-bin 980/980 identity — deterministic counting on the same trees and
  effective data selection; any mismatch = convention drift, STOP). Keep the CENTRAL
  truth-veto contract string (`IsMatchedElectron==0`, `IsMatchedV0Daughter==0` — inert
  on data); do NOT inherit the alpha0p2 vector's
  `disable_truth_electron_veto_for_alpha_central` MC-convention documentation (plan
  trap T9).
- **Driver guard (R10-style, NEW):** the S2′ release driver consumed the CENTRAL
  no-cone data vector. The cone driver must consume the Phase-1 cone vector and MUST
  refuse to run unless the vector label == `raw_data_no_bgsub_alphaveto_cone`,
  `electron_alpha_veto_rad == 0.2`, and the in-grid total == 3,865,073. A silent
  stale-vector unfold is the top mechanical risk (plan trap T8); no fallback path may
  read a no-cone vector.

### 2.3 (c) Prior and efficiency — recomputed from the cone+S2′ response

- `prior := body + miss` computed FROM the cone variant rapgap31 response (body legs
  `w_reco = w1·w2·NucliaWeight`; miss legs `w_gen = w1·w2`). Never reuse the no-cone
  S2′ marginal (its body still contains the in-cone matched content); never rebuild
  priors the plain `w_gen`-only way (superseded 2026-10-07).
- Efficiency `eff[j] = body[j]/(body[j]+miss[j])` drops in cone-affected bins (body
  loses in-cone matched content, miss gains the gen partners) — the acceptance
  correction then RECOVERS the lost true signal statistically (this is why the true
  π/K/p cone loss does not bias the final cross section at first order; Sec 4.3).
  `prior_gen_NWeff` is recomputed on the cone basis (the consumed-key map changes).
- Release driver: dated copy with ALL constants repointed (variant response ROOTs, cone
  data vector, prior source) + prior-provenance assertion in the release metadata
  (S2′ R10 guard, unchanged in form) + the NEW data-vector guard of Sec 2.2. IBU k=4;
  `L_data = 351000.0 nb^-1`; shape (491,246) per charge; data vector 980.

### 2.4 (d) Counters and diagnostics to record (mandatory, real counts — never sentinels)

Per charge, and per fine reco bin (NB_R=490) where applicable:

1. `n_alpha_pre / n_alpha_after / n_alpha_rejected` (prototype lines 717-719 pattern),
   on `pass_pre_alpha` (the S2′ reco selection, pre-cone).
2. Cone-rejection split by class: **matched-electron / det.int. / other**.
   - **Matched-electron sub-counter MUST be computed on the PRE-VETO mask
     (`evt & trk & ~alpha_ok & is_matched_electron`)**: on `pass_pre_alpha` it is
     vacuously 0 (because `not_e` already removed matched electrons) — a vacuous zero
     would mimic the alpha0p2 sentinel-counter trap (plan trap T11) and destroy
     comparability with the measured `.mc_alpha_counts` (RG 215,731 / DJ 273,538).
     Label it explicitly as a pre-veto diagnostic. On DATA this counter is trivially 0
     (no truth flags in data — ledger §4); record it as such, never as a measurement.
   - Det.int. sub-counter on `pass_pre_alpha & ~alpha_ok & is_detint` (well-defined:
     the classifier carries the e/V0 vetoes).
   - "Other" = the remainder (true signal + unflagged fakes).
3. In-cone weight broken down by fill class of the no-cone response: body-matched /
   pre-existing fake unflagged (fc0) / rerouted non-keeper det.int. (fc3) /
   pre-existing fake non-keeper (fc4) / pre-existing fake keeper (fc5) — the exact
   per-cell audit input for gates C2/C3.
4. Det.int. selection counters PRE-cone (existing, parentage-campaign-comparable:
   `n_detint_sel_prez/postz/infill`) and POST-cone twins (new).
5. `--dump-cone-removed-legs`: per-leg TTree (class, reco bin, gen bin, weight, run,
   event) mirroring the S2′ `--dump-rerouted-legs` precedent — the exact per-cell audit
   behind C2/C3.

---

## 3. Exactly-once ledger under cone baseline (the critical section)

Fractions vs the central data vector (no-cone, Σ 3,884,097) unless stated; template
magnitudes are the packaged L351000 cone-applied totals (handoff §3; skill).

| Category | No-cone S2′ chain today | Under cone baseline | External vector |
|---|---|---|---|
| MC-unflagged reco fakes | removed by variant `f_i` (response-internal RG 11.2403%/10.8737%; data-weighted 11.3577%/10.9768%) — exactly once | unchanged mechanism; in-cone fakes are removed by the CONE (data side) and the remainder by the cone-basis `f_i` — exactly once; `f_i` recomputed on the cone response (band Sec 5.4) | none (in-response) |
| Residual (matched) electrons | NOT in the response at all — `not_e` excludes them from body AND fake column (builder line 834/867; ledger §3: "the fake column contains no MC-flagged electrons"); data-side contamination unfolded as signal (under-subtracted) | in-cone electrons removed ONCE by the cone (data-side selection); out-of-cone residual electrons remain response-EXCLUDED (never fake-column); data-side residual removed ONCE by the external template when Option-A is applied | **YES — the packaged CONE-APPLIED template, consumed AS-IS** (see (v)) |
| V0 daughters | excluded entirely (`not_v0_match_top`; F3 drop); data-side unfolded as signal | unchanged exclusion; in-cone V0 removed by the cone, out-of-cone residual by the external cone-applied template — exactly once | **YES — cone-applied template, as-is** (132,065.2768 = 3.4002% of the no-cone central vector) |
| det.int. Class 2 (GEANT/STR charged-hadron secondaries) | removed RESPONSE-INTERNALLY via the S2′ reroute + enlarged `f_i` (removable fraction 0.909920; keeper kept as signal) | UNCHANGED mechanism, on cone-surviving tracks; in-cone det.int. (≈0.16% of the class; survival ≈0.9984) drops out of numerator and denominator together — response-internal, exactly once | **FORBIDDEN — stays forbidden** (double-removal + keeper violation, stronger under S2′) |
| low-Q2 leakage | nothing (source-distinct); external vector specified, unapplied | unchanged; template carries an α0p1 radius mismatch (bounded ≪0.01% by the α0p1→α0p2 chain ratio, mean 0.99972) — caveat or re-derive (Sec 8.4) | YES (only option under every convention) |
| DIFFVM ρ | nothing (source-distinct); systematic-only (ruling 2026-10-07) | unchanged; the cone removes 0/2,102 DIFFVM tracks (measured, QA-verified physical) — the class is untouched | NO — never a subtraction vector |

**The rules, itemized:**

- **(i) Cone-rejected matched electrons vs the fake column — RULE (with a premise
  correction).** In the S2′/central lineage, matched electrons NEVER enter the fake
  column: `not_e = (IsMatchedElectron == 0)` is part of `pass_reco_full` (builder
  lines 834, 867), so flagged electrons are excluded from the response ENTIRELY — body
  AND fake column (ledger §3, §5; ESTABLISHED). The "matched electrons in the fake
  column" semantics (fc=1 routing) belongs to the ALPHA0P2/ALLFAKE lineage, where the
  prototype disables the truth-electron veto (prototype lines 685-693, 389) — that
  disablement stays NOT ported (DO-NOT-PORT entry 1, still in force, Sec 0.4.2).
  **Rule under cone baseline: NO electron content may be routed to the fake column, in
  any cone region.** Cone-rejected matched electrons are removed by the cone (they were
  already response-excluded anyway); out-of-cone residual matched electrons remain
  response-excluded, NOT fake-column. There is no cone-vs-fake-column double-count risk
  in this lineage precisely because the allfake electron routing is not ported; the
  real exactly-once interaction of the cone is with the EXTERNAL electron template
  (item (v)). (If a future convention wanted fake-column electron treatment, that would
  be a separate observable-definition decision — out of scope here.)
- **(ii) Out-of-cone residual electrons stay external-vector material.** Measured:
  residual true e± after the cone, relative to true π/K/p after the veto, is
  `2729276/63340834 = 0.0430887` (RAPGAP) and `3319290/75590840 = 0.0439113`
  (DJANGOH) — i.e., the cone is a partial cleaner (92.4–92.7% of the electron class
  survives), and the out-of-cone residual remains ≈4.3% of the true-hadron level.
  It continues to be handled by the Option-A external per-bin additive vector,
  pre-unfold, once Option-A is applied.
- **(iii) V0 daughters stay excluded** — `not_v0_match_top` in force; the allfake V0
  fake-column routing stays NOT ported; data-side V0 via the external cone-applied
  template. The V0 no-cone unbounded-transfer caveat (ledger §9/§10: no cone-survival
  counter exists) CLOSES BY CONSTRUCTION under cone baseline — the cone-applied
  template becomes same-selection.
- **(iv) det.int. Class 2 removal stays response-internal** via the S2′ reroute on
  cone-surviving tracks. External det.int. vectors remain FORBIDDEN — more strongly
  than before: an external vector would double-remove the removable class AND violate
  the keeper (S2′ spec Sec 3; STATE.md promotion override note 10). The cone changes
  nothing about this rule.
- **(v) Option-A externals — electron-template semantics under cone baseline
  (VERIFIED).** The packaged L351000 electron component **148,976.4352 is the
  POST-CONE RESIDUAL** (cone-applied), NOT a no-cone total: it was measured under the
  alpha0p2 cone selection (the producer
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py`
  applies the α≥0.2 veto; provenance closure row 1), and the ledger §6 labels the class
  magnitude "cone-applied → understated" at **3.8355% of the no-cone central vector**
  (148,976.4352/3,884,097) vs **≈4.1441% no-cone (PROVISIONAL** counter-based rescale
  ÷0.9241/÷0.9270**)**. (The handoff's table denominator 3,865,073.0 is the
  CONE-APPLIED denominator — 148,976.4352/3,865,073 = 3.8543%.) **Exactly-once rule:**
  under cone baseline, the cone removes the in-cone electron content ONCE (selection)
  and the external template removes the out-of-cone residual ONCE (pre-unfold
  subtraction, `Var(d−B) = Var(d)+Var(B)`, clamp ≥0, when Option-A is applied). The
  packaged cone-applied e/V0 templates are consumed **AS-IS** — they become
  selection-consistent by construction (the no-cone rescale and the ≈4.1441% PROVISIONAL
  figure are RETIRED for the cone basis). **NEVER apply a no-cone-rescaled template on
  the cone basis** — it would subtract in-cone content the cone already removed
  (over-subtraction / double-removal of the in-cone class). Note: the Option-A external
  vectors remain UNAPPLIED to the central chain today (raw/no-bgsub); these rules govern
  their future application and the template semantics, and are binding on any consuming
  chain (e.g. the in-flight bgsub Set-R re-base — flagged as a dependency, not executed
  here).
- **(vi) DIFFVM: systematic-only, unchanged** (ruling 2026-10-07: per-bin subtraction
  forbidden; exclusive rho all-decay 8445-8448 only). The cone removes 0/2,102 DIFFVM
  tracks — measured, QA-verified physical — so the ruling and the envelope are
  unaffected.
- **e∩V0 overlap (112.2093 scaled):** IGNORED per user decision 2026-10-08 (max 0.0364%
  double-subtraction, accepted) — unchanged under cone baseline.
- **No double-removal introduced by the cone:** the cone acts on the DATA vector and the
  response reco space IDENTICALLY (C1); in-cone content vanishes from both sides of the
  unfold; the fake column and `f_i` are recomputed on the cone basis; the external
  templates are cone-applied. The chain's failure modes remain the ledger's
  under-subtraction items (out-of-cone e/V0/low-Q2 until Option-A is applied; DIFFVM
  envelope), not double-subtraction — with ONE new forbidden combination: cone basis +
  no-cone-rescaled templates (over-subtraction), and the standing one: cone data vector
  + no-cone response (double rejection, C1 hard gate).

---

## 4. Predicted impact (first-order; ESTIMATE unless marked MEASURED)

### 4.1 Data-side yield (MEASURED, hard bound)

- In-PS: 28,582 of 3,902,480 removed (**0.7325%**); in-grid: 3,884,097 → 3,865,073
  (**−19,024 = −0.4897%**). Direct measurement on the same 658 trees; this is a hard
  upper bound on anything the cone removes.
- MC-based decomposition of the 19,024 in-grid removed tracks (cone impact study §3.2,
  flagged): residual-electron class ≈11,700–12,200 (≈0.29–0.31 pp of the old vector);
  TRUE hadrons ≈2,300–2,700 (≈0.06–0.07 pp); V0 + det.int. + unflagged fakes
  ≈4,000–4,800 (≈0.10–0.12 pp; NO per-class counters — aggregate-bounded).
- MC aggregate in-cone loss (MEASURED, `.mc_alpha_counts`): **0.4259% RG**
  (287,277/67,440,033) / **0.4524% DJ** (364,520/80,569,547) of in-PS reco tracks;
  matched electrons are **75.1%** of the cone-rejected MC population in both
  generators.

### 4.2 Signal loss and matched-electron removal (MEASURED)

- **True π/K/p loss from the cone: 0.10649% (RAPGAP) / 0.11354% (DJANGOH)** of true
  hadrons (validated near-electron study; `docs/status/alpha0p2-background.md` line 20;
  cone impact study §2 "MEASURED"). Cheap signal loss — and acceptance-RECOVERED
  (Sec 2.3): the miss row gains the gen partners, the efficiency drops, the correction
  restores the signal statistically; first-order BIAS ≈ 0, only a statistical cost.
  [PINNED at spec time — register O1: the denominators are stated in the primary
  artifact `integrated_removed_counts_fractions_by_category.json` (variant `alpha_only`,
  category `true_piKp`): RG 67,523/63,408,357, DJ 85,924/75,676,764 — in-grid
  z005-global-bin true π/K/p reco tracks, unweighted count basis. Full record in the
  Sec 8 register.]
- **Matched e± removal by the cone — two measured conventions, do not mix:**
  - PINNED (denominators explicit, `.mc_alpha_counts`): **7.3027% RG**
    (215,731/2,954,136) / **7.587% DJ** (273,538/3,605,110) of pre-cone matched
    electrons (rejected + surviving). This is the citable pair for gate windows.
  - Near-electron-study variant counting: **5.07632% RG / 5.26673% DJ** — a DIFFERENT
    truth-classifier convention, also measured (cone impact study §2 note).
    [PINNED at spec time — register O2: the category is `all_true_e` (ALL true
    electrons: conversion + dalitz + matched non-conversion — the citing docs'
    "matched e±" label is the category name, not only scattered-electron matches):
    RG 145,956/2,875,232, DJ 184,537/3,503,827, same in-grid count basis. Full record
    in the Sec 8 register.]
- **Residual true e± after the cone relative to true π/K/p after veto: 0.0430887 RG /
  0.0439113 DJ** (skill; `docs/status/alpha0p2-background.md` line 21) — the
  out-of-cone residual level carried into the external-template accounting (Sec 3(ii)).

### 4.3 Net cross-section shift (ESTIMATE — bands pinned by the campaign, not by this section)

- **Direction: DOWN in integrated yield.** Raw data-vector yield: **−0.49% in-grid**
  (measured, Sec 4.1). Final unfolded cross-section shift vs the no-cone S2′ release:
  expected integrated order **−0.1% to −0.5% (ESTIMATE)**, hard-bounded above by the
  0.49% in-grid removal. Decomposition of the expectation:
  - in-cone previously-unsubtracted electron contamination: ≈ **−0.30 pp** (measured
    floor: 4.1441% no-cone PROV − 3.8355% cone-applied = 0.31 pp of the old vector);
  - in-cone V0 + unattributed content: ≈ **−0.1 pp** (no per-class counters; bounded);
  - plus the MC-unmodeled data/MC-excess population (×2.3–2.6 integrated) that the
    current chain unfolds as signal — removed by the cone, shifting DOWN by its
    (unmodeled, hence unquantifiable) share;
  - true-signal part: acceptance-recovered, ≈ 0 first-order bias;
  - second order: `f_i` recomposition (≲0.2 pp either way, Sec 4.5), prior-shape
    coupling over k=4, migration smearing.
  - **Per-bin shifts are much larger than integrated in the hot bins** (data removal
    ≥36.6% in the worst measured bin under α<0.1 — global bin 24 = h+ S0x0,
    z∈[0.2,0.3), pT∈[1.0,1.5); a direct per-bin check of the central vs alpha0p2
    vectors puts the worst α≥0.2 removal at 58.6% in h+ reco bin 49; concentration
    at the MEASURED hot-bin locations, NOT at z<0.05/low-pT — 2026-10-11 critic
    correction) — expect O(few%) shifts in the worst measured hot bins.
- The pre-registered band procedure (plan §7 C5: measured Δd + measured response-side
    changes + explicit first-order algebraic propagation ± registered width) pins the
    acceptance band; this section's range is the planning input only. The C5
    z-concentration/worst-bin diagnostic is NOT pre-registered around a low-z/low-pT
    expectation (2026-10-11 critic correction): it must test the per-bin
    concentration WHEREVER it is, with the measured hot bins named as the
    expectation (worst measured data-removal bin global bin 24 = h+ S0x0,
    z∈[0.2,0.3), pT∈[1.0,1.5), ≈36.6% under α<0.1; direct central-vs-alpha0p2
    per-bin check: worst removal 58.6% in h+ reco bin 49). The det.int.
    S2′ G10 band [−0.36%, −0.27%] does NOT apply to the cone shift (different physics
    content); attribute cone-vs-det.int contributions via the C3/C5 decomposition
    diagnostics, never by differencing releases.

### 4.4 The S2′ removable fraction under the cone — re-measurement question

The removable fraction **0.909920 ± 0.000269** (DJANGOH14 0.912611 / RAPGAP31 0.906412)
was measured by the parentage campaign on the **NO-CONE central selection**. Under the
cone:

- **DEFAULT: no dedicated re-measurement campaign.** The fraction is mother-lineage
  based (interaction-born + non-π/K/p-mother decay-born removed; ±321-mother keeper
  kept) and therefore cone-independent BY DEFINITION at the mother level; the class-level
  cone survival **≈0.9984** (in-cone ≈0.16% of the class; measured by the parentage
  campaign — S2′ spec G10 note, R2 closed at class level) bounds the transfer.
- **REQUIRED: G13 re-validation on the cone build** (Sec 5.5, C5) — the build's own
  realized removable fraction must land at ≈0.9099 within a window widened by the
  survival bound, and per-bin rerouted weights must match the reference map (with the
  matched-share correction) — this simultaneously catches a skipped keeper, an
  over-broad keeper, PDG semantic drift, AND cone-induced composition drift.
- **Composition caveat (why the check is not a formality):** the det.int. composition
  NEAR THE ELECTRON may differ from the class average (interaction-born secondaries
  from shower/conversion material could be enriched in-cone); only the class-level
  survival exists today — the in-cone mother-lineage composition is NOT decomposed in
  any artifact. The possible drift of the realized cone-basis removable fraction is
  bounded by the ≈0.16% class-level in-cone content. If the C5 integrated check lands
  outside its pre-registered window, the cone parentage re-slice (below) triggers and
  answers it directly.

### 4.5 `f_i` and contamination fractions (ESTIMATE)

- Response-internal `f_i` shift vs the S2′ values (11.2403%/10.8737% RG): **≲0.1–0.2 pp,
  sign undetermined** (no in-cone counter exists for the fake column's own content;
  ≈75% of the cone-rejected MC population is matched electrons, which are OUTSIDE the
  response already). Data-weighted convention: second-order ≲0.1–0.3 pp, plausibly
  down. The det.int. term is cone-insensitive (survival 0.9984). Band pinned at Phase 2
  from a merged-shard sample, pre-registered (plan §6 C4).
- Contamination fractions of the data vector (e + V0 + low-Q2): no-cone like-for-like
  ≈7.55% (with the quantified electron rescale); cone-adopted
  (148,976.44 + 132,065.28 + 208.38)/3,865,073 = **7.2766%** — net ≈ **−0.27 pp**
  (−3.6% relative). The bgsub subtraction set STAYS e + V0 + low-Q2 (it does not
  shrink); the electron vector remains material at 3.8543% of the new vector.

### 4.6 Per-bin parentage reference map — cone re-slice

The G13 per-bin reference
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/perbin_contamination_motherspecies_2026-10-09.csv`
(980 rows charge-major; `frac_avg_removable_keepK`; per-generator
`yield_b_rg/dj_removable_keepK`) is an ALL-ROWS **NO-CONE** central-selection map. A
**cone re-slice of the parentage diagnostic is LIKELY NEEDED** as the C5 per-bin
reference: one light pass over the existing 865 v6 tree units (~20-40 min single Condor
unit class; precedent clusters 5433299/5435475; the campaign machinery already recorded
per-class cone survival, so the cone-conditioning extension is small). RECOMMENDED
before or alongside the campaign; if deferred, C5 runs against the no-cone map with the
matched-share correction and the 0.9984 survival bound (plan G13 default), and the
re-slice becomes the contingency on any out-of-window C5 result.

---

## 5. Validation gates — ALL mandatory before any promotion

Structure: the S2′ G1–G13 suite carries where applicable (with the redefinitions
below), PLUS six cone-specific spec-level gates C1–C6. Any failure blocks promotion.

### 5.1 C1 — Data-vector/response selection identity (HARD GATE)

- The cone is applied IDENTICALLY in the data vector and the response reco legs: same
  formula (Sec 1.1, VERBATIM port), same constant (0.2), same placement (after e/V0
  truth vetoes, before the z-guard), same NaN semantics. **A cone response with a
  no-cone data vector is INVALID (double-rejection geometry, Sec 2.2); a no-cone
  response with a cone data vector is equally INVALID.**
- Data-vector reproduction: in-grid 3,865,073 (h+ 1,965,700 / h− 1,899,373); in-PS
  3,873,898; `alpha_pre` 3,902,480 (exact parity with the central in-PS count); in-PS
  rejected 28,582; in-grid removal 19,024 (−0.4897%); per-bin identity with the
  existing alpha0p2 vector 980/980 EXACT; monotonicity vs the central no-cone vector
  (per-bin delta ≥ 0, sum = 19,024); 0/980 negative inputs.
- Response-side S0 input verification: the cone builder's reco selection is bit-identical
  to the S2′ builder's except the cone term (assert from the builder headers/metadata;
  the S2′ `s0_input_verification.json` pattern).
- Driver guards demonstrably active: R10 prior-provenance assertion + the NEW
  data-vector guard (label / `electron_alpha_veto_rad` / in-grid total; no fallback
  path to any no-cone product).

### 5.2 C2 — Miss-row growth consistency

- Per gen bin, algebraically: `miss^cone − miss^nocone-S2′` == the gen partners of
  cone-rejected matched legs NOT consumed elsewhere (weight `w1·w2`), verified via
  `--dump-cone-removed-legs` on a smoke shard + the merged consumed-key audit.
  **The "NOT consumed elsewhere" clause is HARDENED (2026-10-11 critic condition)
  and may not silently absorb a key collision.** Required: (a) EXPLICIT ENUMERATION
  of the consumed-elsewhere set — per gen bin, from the merged consumed-key audit —
  with its expected composition stated: **shared-partner multi-leg cases ONLY** (a
  gen partner consumed by another cone-surviving matched leg in the same event); any
  other member is a gate FAILURE, not an absorbed residual. (b) A **zero-collision
  assertion of rerouted-leg keys against the CTG miss-candidate key set**: a
  rerouted leg's recorded STR-candidate key colliding at the 1e-8 rounding with a
  cone-rejected matched leg's GTR partner key would suppress that partner's miss
  growth *inside* the consumed-elsewhere clause, and the existing
  `n_rerouted_key_collisions` watchdog only sees recorded-vs-recorded collisions —
  it cannot catch this class. The assertion must compare the builder's rerouted-leg
  consumed-key recordings (lines ~1229-1253) directly against the CTG
  miss-candidate keys iterated by the miss pass (lines ~1280-1303) of
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
  and require ZERO overlaps.
- Truth conservation holds 245/245 per charge, stated as the precise two-level
  identity (2026-10-11 critic correction; the former "exact ... `body + miss` == the
  unchanged CTG pool marginal" wording was imprecise at weight level): **(a) COUNTS
  are exactly conserved vs the unchanged gen-side CTG pool per gen bin** — the count
  identity is the exact one against the unchanged pool, and the miss growth is the
  mechanism that keeps it exact; **(b) WEIGHTS: `body + miss` == the cone build's
  OWN `prior_gen_NWeff`** (body legs carry `w1·w2·NucliaWeight`, miss legs `w1·w2`),
  and the no-cone→cone weight difference per gen bin = Σ over in-cone matched legs
  of `w1·w2·(NucliaWeight−1)` (no-cone minus cone; auditable, ≈0 expected).
  **WARNING:** an implementer comparing `body + miss` WEIGHTS against the PLAIN
  unchanged pool marginal will see the ~0.45% NucliaWeight effect and must NOT "fix"
  it by suppressing miss growth — that is the single most dangerous
  misimplementation; the count identity is the exact one against the unchanged pool.
- The S2′ G1(a) "miss bit-identical to central" assertion is FALSE on the cone basis BY
  DESIGN — do not copy it (plan trap T7). `body^nocone = body^cone + in-cone body
  weight` and `fake^nocone = fake^cone + in-cone fake weight`, exact per cell / per
  reco bin including the fc decomposition (the C3 identities).

### 5.3 C3 — Exactly-once audit

- Cone-rejected (in-cone) electrons ABSENT from the fake column (trivially true in this
  lineage — all matched electrons are response-excluded — and audited: no fc category
  may contain in-cone content; the in-cone class breakdown of Sec 2.4(3) must sum to
  the total in-cone weight with zero residual).
- Out-of-cone residual electrons present EXACTLY ONCE in the accounting: zero times in
  the response (excluded by `not_e`), once in the external cone-applied template when
  Option-A is applied, never in `f_i`. NO fc=1/fc=2 (electron/V0) categories may exist
  in the build (allfake routing not ported — assert).
- External det.int. vectors ABSENT; det.int. in-cone content absent from BOTH body and
  fake (dropped with its own reroute — audited via the fc3 in-cone breakdown).
- Counter audit: real counts everywhere (no sentinel zeros); the pre-veto matched-e
  cone counter non-trivially populated on MC (comparable to `.mc_alpha_counts`
  215,731/273,538 within per-file windows); the DATA matched-e counter recorded as
  trivially zero (no truth in data), never interpreted as a measurement.
- Template-side audit: the consuming chain's electron/V0 templates are the CONE-APPLIED
  packaged ones, unrescaled (Sec 3(v)); no no-cone-rescaled template anywhere on the
  cone basis.

### 5.4 C4 — True-hadron loss and `f_i` bands

- Integrated true π/K/p in-cone loss within **~2× of the measured 0.10649% (RG) /
  0.11354% (DJ)** (per generator; count and weighted reported).
- Aggregate MC in-cone loss within the pre-registered window of **0.4259% (RG) /
  0.4524% (DJ)** with matched-e share ≈75.1% (both generators).
- Response-internal integrated `f_i` shift vs the S2′ values within the pre-registered
  band (predicted ≲0.2 pp, sign undetermined; band pinned from a merged-shard sample
  BEFORE the full merge is unsealed, and recorded in the acceptance register). Per-bin
  `f_i` bounded by the local in-cone content; 0 negative/exceeds bins (G8 form).

### 5.5 C5 — G13-analog per-bin reroute check (against the cone re-sliced parentage map)

- Reference: the cone re-sliced per-bin parentage map (Sec 4.6) if delivered; otherwise
  the no-cone map `perbin_contamination_motherspecies_2026-10-09.csv` with the
  matched-share correction (~0.862 central pooled; the build's own category exports
  refine it per bin) and the 0.9984 survival bound, per the S2′ G13 basis caveat.
- DECISIVE integrated check: the realized integrated removable fraction (rerouted
  matched non-keeper + pre-existing fake non-keeper, over all det.int. content) must
  land at ≈ **0.9099** (per-generator reference RG 0.906412 / DJ 0.912611) within the
  pre-registered window widened by the survival bound — simultaneously catching a
  skipped keeper (→1.000), an over-broad keeper (→low), GenMotherPDG semantic drift,
  and cone-induced composition drift (Sec 4.4). Out-of-window ⇒ the cone parentage
  re-slice triggers and adjudicates.
- Exclude data-zero and 1–4-data-track sparse bins from fractional gates (report-only),
  per the S2′ G13 precedent.

### 5.6 C6 — Closure gates re-run on the cone+S2′ response

- **G5:** 1000-toy Poisson closure per charge through the cone `f_i`/response/prior;
  covariance finite/symmetric/positive; nominal reproduces the main-gate IBU
  bit-consistently. WATCH: the S2′ negative-charge leg sat at the ≤4 window edge
  (|z|max 3.70, frac|z|<1 0.637 vs [0.593, 0.772]) — re-verify and record; edge contact
  = report, not auto-fail (S2′ remaining-work item 5).
- **G6:** DJANGOH14 cross-generator closure on the cone basis; generator dependence of
  the removed det.int. block quantified from the per-generator category vectors —
  never naive |DJ−RG| on stored driver legs (S2′ critic condition; the DJ stored-prior
  leg stays report-only).
- **G7:** refold chi2/ndf of the cone-unfolded result through the cone response + fake
  column vs the externally subtracted data; tolerance re-derived for the cone response,
  pre-registered.
- Carried unchanged in form: **G2** (supported bins 245/245), **G3** (independent IBU
  agreement ≤1e-12), **G4** (BBB-IBU consistency; central BBB reproduction untouched),
  **G8** (negative-bin/acceptance audit: 0/980, 0/490, eff ∈ (0,1], 0
  background-exceeds-data), **G11** (category vectors partition exactly; matched share
  consistent with the 86/14 parentage partition within the survival bound), **G12**
  (weight audit 0 bins >5× mean; fake-column MC-stat sumw2 recorded for the budget
  re-base).
- Redefined (do NOT copy the S2′ forms): **G1 → C2** (miss growth, not miss
  identity). The G1′(a) truth-conservation clause carries the precise two-level
  identity, not the former plain "`body+miss` == unchanged CTG pool marginal"
  wording: **(a) COUNTS are exactly conserved vs the unchanged gen-side CTG pool
  per gen bin** (the exact identity against the unchanged pool); **(b) WEIGHTS:
  `body + miss` == the cone build's OWN `prior_gen_NWeff`** (body legs
  `w1·w2·NucliaWeight`, miss legs `w1·w2`), and the no-cone→cone weight difference
  per gen bin = Σ over in-cone matched legs of `w1·w2·(NucliaWeight−1)` (auditable,
  ≈0 expected). WARNING: the ~0.45% NucliaWeight effect seen when `body + miss`
  weights are compared against the PLAIN unchanged pool must NOT be "fixed" by
  suppressing miss growth — the single most dangerous misimplementation (full
  statement and gate mechanics: Sec 5.2 C2).
  **G9 → C4 + the C3 decomposition identities** (the S2′ invariant-denominator identity
  has no cone-basis central to run against; a contingency cone-central pre-S2′ response
  exists only as a triggered option — Sec 8.5); **G10 → the C5 pre-registered cone-shift
  band** (the S2′ detint band [−0.36%, −0.27%] does not apply).

### 5.7 Mapping to the execution plan's operational gates

This spec's C1–C6 are the convention-level gates; the plan document §8 operationalizes
them (its own ids, execution-phase scoped): plan C1 ≡ spec C1 (data-vector parity);
plan C2 ≡ Phase 0 formula smokes (feeds C1/C4); plan C3 ≡ spec C2(b)/C3 decomposition
identities; plan C4 ≡ spec C4 (`f_i` band); plan C5 ≡ spec 4.3 shift band + C6
diagnostics; plan C7 ≡ shard/duplicate integrity (a production-hygiene precondition of
every gate); plan G1′/G9′/G10′ ≡ the spec 5.6 redefinitions. Pre-registration
discipline (S2′ G10 lesson): every band is fixed in the acceptance register BEFORE the
corresponding production; in-session modifications only as documented addenda.

---

## 6. Production plan (spec level; execution mechanics in the plan document)

RUNLIST rules honored: extend canonical scripts as dated variants, never fork blindly,
never edit in place; `L_data = 351000.0 nb^-1`; response shape (491,246) per charge;
data vector 980; IBU k=4; single-file login-node smokes (≤5 min, one file) first, then
Condor for anything multi-file; never overwrite central products.

1. **Cone+S2′ response builder:** new dated script
   `build_migration_th2_per_charge_s2prime_alphaveto.py` (clone of the S2′ builder +
   the Sec 2.1 cone port + the Sec 2.4 diagnostics), campaign
   `alphaveto_cone_s2prime_<launch-date>`; the header must document the DO-NOT-PORT
   reversal (entry 2 reversed by user decision 2026-10-11; entries 1 and 3 remain).
2. **Data-vector rebuild:** dated clone `build_data_vector_v4_alphaveto.py`
   (Sec 2.2); 658 files via Condor (~66 shards of ~10 files); merged cone vector JSON
   with the `raw_data_no_bgsub_alphaveto_cone` label and contract fields.
3. **Campaign scale (unchanged from S2′):** RAPGAP31 364 files → **~73 shards**;
   DJANGOH14 501 files → **~101 shards** (174 total; 5 files/shard; 24–32 GB/shard).
   Precedents: S2′ response build clusters 5469609/5469610, merge 5473512/5473607;
   coherent-prior rebuild 5342813 (73/73); all-fake 3987932. Duplicate guardrails:
   unique-shard-set + sha256 manifest at merge; shard sums == merged totals exactly; no
   tree re-production; Python-only (no H1OO compilation anywhere in this campaign).
4. **Cone parentage re-slice:** one light pass over the 865 v6 tree units (Sec 4.6),
   delivering the cone-conditioned per-bin mother-lineage map as the C5 reference —
   RECOMMENDED in the campaign scope.
5. **Release regeneration (candidate first):** dated candidate RAW release (IBU k=4 +
   BBB; coherent prior FROM the cone variant rapgap31 response; R10 + data-vector
   guards) under
   `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alphaveto_cone_s2prime_<date>/release_candidate/`,
   then checksum-verified copy into `data_release_candidates/` after gates pass;
   1000-toy stat sidecar (S2′ precedent cluster 5481723). **Post-promotion only
   (separate user decision):** install into `output_v6/central/data_release/`
   (checksum-verified additive copy; install record) → Born release with the UNCHANGED
   C_rad tables (per-bin Born shift == per-bin raw shift, ≤1e-12; S2′ precedent cluster
   5482794) → **full budget regeneration on the cone nominal, INCLUDING the
   shifted-response detector legs ×5 (elec_energy, elec_theta, hfs_jes, hfs_rces,
   hfs_theta: shifted data vectors + shifted CONE+S2′ responses) and the tracking
   ensemble 16 seeds WITH the cone** — closing S2′ remaining-work item 1 once, on the
   final basis (ordering rationale Sec 8.3) — plus `detint_removal_rel` /
   `fakecol_mcstat_rel` re-derivation and all anchors.
6. **R10 driver repointing guard:** dated driver copy
   `run_bbb_ibu_release_s2prime_alphaveto.py` with ALL constants repointed (variant
   response ROOTs, cone data vector, prior source) + prior-provenance assertion in the
   release metadata + the NEW data-vector guard (Sec 2.2); no code path may read a
   central/no-cone response, prior, or vector.
7. **Smoke order:** Phase 0 single-file smokes (one data tree + one MC tree; alpha_pre
   parity; rejected-count reproduction; finite-alpha fraction; per-file rate windows;
   det.int./keeper/reroute counters consistent with the S2′ smoke precedent;
   body+fake+miss conservation) → one data + one MC full smoke → Condor campaigns →
   merges → gate suite → 1000-toy sidecar → critic dispatch. **Install only after all
   gates pass and the critic verdict is recorded; promotion is a separate USER
   decision.**

---

## 7. Quoting rule (EXTENDED again — cone status becomes mandatory)

- The observable-basis flag gains the **cone basis**. **THREE released bases now
  exist:**
  1. **no-cone pre-S2′** — the coherent-prior release
     `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`
     — historical/audit only, PRE-S2′-BASIS;
  2. **no-cone S2′** — the current canonical RAW release (Sec 0.5) + its Born release +
     15-component budget — current until the cone promotion;
  3. **cone+S2′** — this baseline, once built and promoted; PROVISIONAL CANDIDATE
     until then.
- **Every released-value quote must state: (a) prior basis (coherent `body+miss`),
  (b) Born-vs-raw level, (c) S2′ detint observable basis, AND (d) cone status
  (α≥0.2 cone vs no-cone).** Quoting any cone-basis value against any no-cone value
  (or the reverse) without the cone-status flag is FORBIDDEN, pre- and post-promotion.
  Interim candidate numbers are PROVISIONAL CANDIDATE, never "current". Treat the cone
  shift as a SELECTION-CONVENTION (observable-basis) change, never as a like-for-like
  method update.
- **Side studies keyed to the no-cone selection** need selection-mismatch flags once
  the cone basis is promoted: the BSA fork `v6bsa` (raw, no cone — Sec 8.1), the qT/Q
  hybrid chain, the alpha0p2 diagnostics (their own scope labels), and the in-flight
  bgsub Set-R chain (needs its own cone re-base if it is to live on the promoted basis
  — a flagged dependency, not executed by this spec).

---

## 8. Open questions (SURFACED, NOT DECIDED — this spec does not close them)

1. **BSA side fork (FLAG ONLY).** The BSA fork `v6bsa` (source
   `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6bsa.C`;
   DATA only, 658/658 files; acceptance-free by construction; raw `A_LU^{sin phi_h} =
   -0.00337 ± 0.00228` consistent with zero) has NO cone and NO MC. Post-promotion it
   diverges from the central chain in observable basis. Flag: any future BSA quote must
   state cone status; BSA re-derivation under the cone (if physically warranted —
   the cone's measured per-bin removal concentrates at the measured hot bins (worst
   measured data-removal bin global bin 24 = h+ S0x0, z∈[0.2,0.3), pT∈[1.0,1.5); a
   direct per-bin check of the central vs alpha0p2 vectors puts the worst removal at
   58.6% in h+ reco bin 49), NOT at low-z/low-pT — 2026-10-11 critic correction; the
   false-asymmetry systematic's location must be re-argued from the measured cone
   structure, not from a low-z assumption) is a SEPARATE future decision with its
   own spec. No BSA edits here.
2. **alpha0p1 status — CONFIRMED comparison/provenance only** (skill: "alpha0p1
   (alpha<0.1 rad) is comparison/provenance only"; `docs/status/alpha0p2-background.md`).
   The cone-baseline adoption does not change it. Whether a 0.1-vs-0.2-rad radius
   sensitivity enters the successor budget as a NEW component (the α0p1 diagnostic
   products exist as a basis) or stays a documented selection-convention statement is a
   separate post-promotion user decision (plan Q4).
3. **Shifted-variant detector-systematics follow-up — COMPOUNDED; recommended ordering:
   cone FIRST, then shifted productions on the cone+S2′ basis.** S2′ remaining-work
   item 1: the 15-component budget's detector/tracking components are DATA-SIDE-ONLY
   (no shifted variant responses exist; ~+2.5 pp conservative convention gap vs
   central, elec_energy dominant). The cone adoption compounds this: the shifted
   productions must now carry BOTH the S2′ reroute AND the cone. RECOMMENDED ORDERING:
   complete the cone campaign and (on promotion) build the shifted detector legs ×5 and
   the tracking ensemble 16 seeds ON the cone+S2′ basis — so the long-pole shifted
   campaigns are run ONCE, on the final basis, and the conservative
   detector-component gap closes once. Building shifted responses on the no-cone S2′
   basis first would be invalidated by the cone promotion and would have to be redone.
4. **Low-Q2 template radius mismatch (DECISION AT PROMOTION; default: caveat).** The
   low-Q2 DjBkgd vector (208.3795 scaled tracks) was built under an α0p1 cone — a
   selection mismatch vs the α≥0.2 baseline, bounded ≪0.01% by the α0p1→α0p2 chain
   ratio (mean 0.99972). Carry as an explicit tiny caveat for the candidate phase
   (Option-A externals remain unapplied to the central chain anyway); re-derive the
   single low-Q2 vector with α≥0.2 (cheap, one sample, Condor) BEFORE any consuming
   chain uses it on the cone basis (plan Q1).
5. **Contingency cone-central (pre-S2′) response build (DECISION, default: NOT built).**
   The S2′ G9 invariant-denominator identity cannot be re-run exactly without a
   cone-basis pre-S2′ central response (~174 additional shards). Default accepts the
   replacement checks (Sec 5.4/5.6 redefinitions). Build only on trigger: C3/C4/C5
   failure or an out-of-band `f_i` shift (plan Q2).
6. **In-flight bgsub Set-R chain dependency (FLAG).** The separate bgsub re-base chain
   currently in flight (own critic gate; NOT consumed by this spec) is built on the
   no-cone basis; if it is to live on the promoted cone basis it needs its own cone
   re-base — flagged as a dependency, not decided here.

**OPEN-VERIFY register (denominator/convention provenance for quoted numbers; each
item records its resolution state — resolve or verify before use in any gate window):**

- **O1 — True-hadron loss denominators — RESOLVED at spec time (2026-10-11).** The
  0.10649% (RG) / 0.11354% (DJ) true π/K/p cone-loss quotes
  (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/status/alpha0p2-background.md`
  line 20; archived RUN_STATUS
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/archive/pre-compaction-startup-pipeline-2026-07-03/RUN_STATUS_20260703T000000.md`
  line 40) do not state their denominators; the primary artifact does:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_electron_opening_angle_veto_2026-06-20/alpha0p2_true_hadron_electron_efficiency_2026-06-23/integrated_removed_counts_fractions_by_category.json`
  — variant `alpha_only` (α<0.2 cone), category `true_piKp`:
  **RG 67,523/63,408,357 = 0.0010648912 ± 0.0000040959**;
  **DJ 85,924/75,676,764 = 0.001135408 ± 0.0000038712** — digit-for-digit matches of
  the quotes. Denominator population (from the study's counting script
  `count_alpha0p2_true_hadron_electron_efficiency.py`, same directory, lines 71-72,
  84-86): z005-global-bin VALID (in-grid) reco tracks under the study's standard
  event selection (Q2/y/Empz/|PVz|) and track selection (Pt/|Eta|/NHitDedx/RadLength/
  DcaCosh/IsFromPrimary/track class/`IsMatchedV0Daughter==0`/xF<0), truth-classified
  `IsMatchedHadron==1 & IsMatchedElectron==0 & |GenPDG| ∈ {211,321,2212}`;
  UNWEIGHTED COUNT basis. Usable in the C4 window as-is; "of true hadrons" in the
  citing docs means "of in-grid true π/K/p reco tracks".
- **O2 — "Matched e± removal" denominators — RESOLVED at spec time (2026-10-11), with
  a naming correction.** The 5.07632% (RG) / 5.26673% (DJ) pair (same citing docs) is
  category **`all_true_e`** in the same primary artifact — ALL true electrons
  (conversion_e + dalitz_e + matched_e_nonconversion; the study's own consistency
  check asserts the subcategories sum to `all_true_e`):
  **RG 145,956/2,875,232 = 0.050763208**;
  **DJ 184,537/3,503,827 = 0.0526672692** — digit-for-digit matches. The citing docs'
  "matched e±" label is the category NAME, not a statement that only
  scattered-electron matches are counted (the matched non-conversion subcategory alone
  removes 18.3% RG / 20.0% DJ). Same in-grid count basis as O1. This
  near-electron-study convention stays DISTINCT from the pinned `.mc_alpha_counts`
  matched-electron pair 7.3027% RG (215,731/2,954,136) / 7.587% DJ (273,538/3,605,110)
  — never mix the two conventions in one gate window (Sec 4.2).
- **O3 — L_MC values for any cone-campaign MC scaling — OPEN (verify at execution).**
  The campaign consumes the standing v6 MC luminosities RAPGAP31
  `L_MC = 9,132,891.0 nb^-1` (scale 0.03843251824641288) and DJANGOH14
  `L_MC = 10,487,495.0 nb^-1` (scale 0.03346843073584302). Verify at execution
  against
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/docs/RUN_STATUS.md`
  (lines 79-80) and the oomclumi-derived aggregate
  `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`
  (DJANGOH14 four-catalog sum; standing trap: never import L_MC from a sister
  workspace's submit script). Any luminosity question is normalization-owned
  (h1-normalization), not cone-spec-owned.
  2026-10-16 correction note: the RG31/DJ14 scale decimals above were corrected
  2026-10-16 against the correction authority
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/backgrounds/normalization_verification_recokin_mc_2026-10-08/verification_record.json`
  (fields `scale_factors`, `lowq2_scale_typo_correction`); any already-executed
  O3 verification must be re-verified against the corrected scales, not
  silently re-passed.

---

## Provenance

- **Decision record:** USER DECISION 2026-10-11, verbatim: "The cone cut must be
  baseline. We know we have many fake tracks from the scattered electron." (Sec 0.1).
  This spec encodes that decision; it does not independently recommend it. The
  superseded recommendation it replaces is the cone impact study §6 (Sec 0.3).
- **Documents (read-only inputs, absolute paths; none modified):**
  - S2′ spec Rev 2 (structural template; receives the append-only pointer):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md`
  - STATE.md (S2′ promotion override; current canonical basis):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/STATE.md`
  - Cone impact study 2026-10-11 (measured magnitudes §2/§3; rebuild chain §5):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md`
  - Companion execution plan (phases; operational gate ids; traps):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/plans/plan_alpha_cone_central_imposition_2026-10-11.md`
  - Consistency ledger 2026-10-07 (cone counters; template semantics; §9 items):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`
  - Alpha0p2 all-fake handoff 2026-06-28 (L351000 template totals; miss accounting):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`
  - Alpha0p2 status (line 20: the O1/O2 quotes; line 21: residual-e ratios):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/status/alpha0p2-background.md`
  - Archived RUN_STATUS 2026-07-03 (line 40: same quotes; near-electron study roots):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/archive/pre-compaction-startup-pipeline-2026-07-03/RUN_STATUS_20260703T000000.md`
  - Background provenance closure 2026-10-05 (lab-frame alpha; §4.6 double-rejection):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/critiques/v6_background_provenance_critic_loop_2026-10-05.md`
  - 2026-10-06 application audit (refuted upstream-application claim; file UNTOUCHED,
    recorded as refuted in Sec 1.3):
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/alpha0p2_application_audit_2026-10-06.md`
  - Backgrounds skill:
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/.opencode/skills/sidis-hq2-backgrounds/SKILL.md`
- **Measured artifacts (read-only; absolute paths):**
  - Cone counter provenance:
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/provenance/validation_alpha0p2_final_summary.json`
  - Cone-applied data vector (the C1 reproduction target):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/data_vector_alpha0p2_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json`
  - Near-electron study root (O1/O2 primary artifacts):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_electron_opening_angle_veto_2026-06-20/alpha0p2_true_hadron_electron_efficiency_2026-06-23/`
    (`integrated_removed_counts_fractions_by_category.json`;
    `count_alpha0p2_true_hadron_electron_efficiency.py`)
  - Det.int. per-bin parentage map (C5 reference; NO-CONE):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/perbin_contamination_motherspecies_2026-10-09.csv`
  - C_rad releasebin tables (unchanged; reco-only ruling):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/`;
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/`
  - Releases (Sec 0.5 / Sec 7): pre-S2′ coherent-prior
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`;
    current canonical S2′
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
- **Code (read-only provenance; never edited by this spec):**
  - S2′ response builder (the port target's parent):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
  - All-fake prototype (cone precedent):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_allfake.py`
  - Alpha0p2 v4 migration builder (formula precedent):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py`
  - Background vector producer (cone-applied template provenance):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py`
  - Central data-vector builder (clone parent for the cone vector):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_data_vector_v4.py`
  - Stage-1 tree builder (HFS-only `RejectTracksNearScatElec`; untouched):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`
  - BSA fork source (Sec 8.1 flag):
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6bsa.C`
- **Superseded DO-NOT-PORT entry (record, not an edit):** S2′ spec Sec 2.1 entry
  "the alpha cone (prototype `alpha_ok` / `ELECTRON_ALPHA_VETO_RAD`) — central is
  no-cone". Superseded by the user decision above; the reversal is specified here
  (Secs 1-2); the entry text in the S2′ spec is retained verbatim as provenance.
- **Append-only pointer note (the one permitted edit outside this file):** a dated
  2026-10-11 note inserted immediately after the S2′ spec's
  "DO NOT PORT (each would silently change the selection or the observable):"
  heading line, marking the alpha-cone entry superseded, pointing to this spec Sec 0,
  and stating the other two entries remain in force. Insert-only: no existing line of
  the S2′ spec was modified or removed.
- **Read-only statement:** NOTHING was rebuilt, launched, or centrally touched in the
  making of this specification — no central or diagnostic product, release, script,
  tree, or campaign was modified or started. The complete write footprint of this
  decision at spec time is: (1) this file
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/alpha_cone_baseline_spec_2026-10-11.md`;
  (2) the append-only pointer note in the S2′ spec's DO-NOT-PORT list. The cone basis
  becomes a product only via the Sec 6 campaign, the Sec 5 gates, and a separate user
  promotion (Sec 0.5).
# Detector-interaction parentage study — implementation-ready design (MODS level, full sample)

**Date:** 2026-10-08 · **Author:** h1-mc-expert · **Status:** DESIGN ONLY — no code, no
Condor, no central product touched. This document specifies the full-sample MODS-level
parentage study that splits the matched detector-interaction (det.int.) block of the
v6/z005 charged-hadron SIDIS analysis into interaction-born vs decay-born (vs other)
components, per the user decision following the audit
`detint_primary_vs_material_separation_2026-10-08.md` (sections D–F, U1–U4).

**Quarantine contract (inherited from the clone basis):** the study is a
NOT_TREEBUILDER diagnostic. It writes only under its own `outputs/`. It never writes
central trees, event lists, responses, steering, releases, or production binaries.

---

## A. Objective and headline deliverable

**Objective.** For every Tier-0 truth-matched det.int. track in the central v6
selection, record its complete MODS-level parentage chain (mother + up to six
ancestor levels, veto-walk result, sibling topology) plus every ingredient of the
response-builder selection, so that the matched det.int. block — the ~86% of det.int.
unfolded as signal (in-observable by design; ledger 2026-10-07) — can be decomposed
into operational parentage categories.

**Headline quantity.** The interaction-born fraction of the matched det.int. block,

    f_int = N(hadronic_interaction) / N(det.int. matched block),

per generator × period × charge, unweighted with binomial MC-statistical uncertainty
plus a weighted variant (w1·w2·NucliaWeight). Secondary: the same fraction per
(Q2,x) cell and per coarse gen (z,pT) bin; the re-derivation of the matched/fake
partition of det.int. under the **central (no-cone) selection** (the published
86.18/13.82 partition was measured under the alpha0p2 diagnostic variant —
`detint_response_partition_summary.json`, 20260626); and per-category scaled yields
at L = 351000.0 nb⁻¹.

**Why it matters (from the audit).** The det.int. label is a truth-source class, not
a physics-process class: single-file proxy composition was ~62% hyperon-decay
mothers, ~30% interaction candidates, ~8% K-parented (n=122, DJANGOH14 7091
`ep0607_0200`, PROVISIONAL). A blanket det.int. removal would also delete decay
feed-down; the observable convention decision needs this decomposition at full
sample. The ledger FORBIDS external det.int. subtraction; this study informs the
in-observable treatment only.

---

## B. Target population at MODS level (design item 1)

### B.1 The exact v6 label being reproduced

A reco track row of the v6 `ChargedTot` tree is a **matched det.int.** track iff:

1. **Tier-0 truth link exists:** `H1PartCand::GetPartMC() != nullptr`. The link is
   created by H1OO (`H1CreateSelTracks.C:115-145`, DTRY best STR track → GTR/STR
   link), the same object the Stage-2 builder reads as Tier 0
   (`ChargedSidisTreeBuilderHighQ2_v6.C:1688-1696`).
2. **Matched partner flags:** partner `IsStr()==1` (i.e. `fStrIdx != -1`), partner
   `IsGtr()==0` (`H1PartMC.C:187-196`), `IsMatchedHadron==1` (partner |PDG| ∈
   {211, 321, 2212}), `IsMatchedElectron==0`, `IsMatchedV0Daughter==0`
   (classifier string in `detint_response_partition_summary.json`; audit C.1).
3. **Response finalcandidate selection** (reco side, applied by
   `build_migration_th2_per_charge_v4.py`):
   - event fiducial: 150 < Q2es < 20000, 0.1 < Yes < 0.7, Empz > 45 (lines 238-241,
     606-615);
   - event cleanup: |PVz| < 35 cm (lines 266-267, 617-620);
   - track: Pt > 0.15 GeV, |Eta| < 1.6, NHitDedx ≥ 5, RadLength ≥ 5.0 (cm),
     |DcaPrime/cosh(Eta)| < 2.0, IsFromPrimary == 1, track class
     (IsCentralTrk==1 or TrackType ∈ {1,2}), xF_SIDIS_es < 0 (lines 269-273,
     623-645);
   - masks: IsMatchedElectron==0, IsMatchedV0Daughter==0 (lines 647-652, 659);
     z_outlier (z_SIDIS_es ≤ 0 or ≥ 1) removed at line 677.
   - Combined tracks enter via track_class_ok; the builder's Pass-2 dedup
     (same-charge, ΔR < 0.05 coincidence, `ChargedSidisTreeBuilderHighQ2_v6.C:1562-1571`)
     determines which combined tracks become separate `ChargedTot` rows.
4. **Matched-block membership** (the 86.18% sub-block; distinguishes unfolded-as-signal
   from fake-column): gen event cuts 150 < Q2_es_gen < 20000, 0.1 < Y_es_gen < 0.7,
   GenEmpz > 45, xFGen < 0, validGenElec (response gen loop, lines 247-250, 473-487,
   508); matched species in pool; GenPt > 0.15 (reco-symmetrization, line 113);
   in-grid (245-bin coarse gen grid, 7 (Q2,x) cells × 7 z × 10 pT, `binning_v3`).

### B.2 Piece-by-piece MODS recomputability

| Ingredient | MODS-level source in the electron-study framework | Status |
|---|---|---|
| Event admission (passRecoEnv envelope) | **Identical by construction**: the study parses the same per-chunk sidistree steer, reads only its `fEventListFiles`, and runs the native selected `H1Tree::Next()` loop (README_NOT_TREEBUILDER.md §v3 contract) | EXACT |
| Tier-0 link | `H1PartCand::GetPartMC()` — same MODS object as builder Tier 0 (v5 source line 98 pattern) | EXACT |
| Partner IsStr/IsGtr/StrIdx/PDG | matched `H1PartMC` getters (v5 Node fill) | EXACT |
| IsMatchedHadron / IsMatchedElectron | PDG pool test on partner | EXACT |
| IsMatchedV0Daughter / veto walk | port `findVetoAncestorPDG` 6-hop walk over `H1PartMCArrayPtr` (`ChargedSidisUtils_highQ2_v6sys.h:181-197`; builder 1762-1779). **The v5 3-level ancestor chain is insufficient** — the walk is 6 hops | EXACT after port |
| Q2es / Yes / Xes | `H1CalcKine::GetQ2es()/GetYes()/GetXes()` — the identical calls the builder makes after `H1Calculator::Instance()` (builder 379, 1057-1069; v5 already calls GetQ2es/GetXes) | EXACT |
| Empz | **Not a calculator getter.** Builder derivation from HAT `Ys` + electron terms: `Empz = sigmaH + elecContrib` with `sigmaH = Ys·elecContrib/(1−Ys)` (builder 1171-1177). Port verbatim | EXACT after port |
| PVz | HAT `"VtxZ"` pointer (builder 437) | EXACT |
| xF_SIDIS_es / z_SIDIS_es | port the eSigma-frame block (builder 1179+): `EprimeES = Q2e/(2·eBeamE·(1+cos eTheta))` if valid else HAT eE (1185-1187); γ* from beam 4-vectors minus eSigma scattered electron (HAT `ElecTheta/ElecPhi/ElecE`, builder 1122-1134); boosts; z/xF in γ*p frame | EXACT after port |
| weight1 / weight2 | HAT `"Weight1"/"Weight2"` pointers (builder 1039-1049) | EXACT after port |
| NucliaWeight | `H1NuclIACor::GetWeight` species-refined on partner PDG (builder 1639-1650, 1830-1840) | EXACT after port |
| ScatE 4-vector, beams | HAT pointers (builder 1122-1134) | EXACT |
| Gen event kinematics (Q2_es_gen, Y_es_gen, x_es_gen, GenEmpz, GenEnPhoton, validGenElec, gen beams) | HAT gen pointers `GenThElec/GenPhElec/GenEnElec/GenEnHad/GenPzHad` (builder 455-465) + builder gen block; GenEmpz fallback formula documented at builder 462-465 (`GenEmpz = GenEnHad − GenPzHad + GenElecE·(1−cos GenThElec)`), rad-safe ISR photon per FIX V5G-2 (builder 453) | EXACT after port |
| Matched-partner kinematics (GenP/Pt/Eta/Phi/Charge, zGen, xFGen) | builder matched fill (1731-1841) — v5 records only Node identity, **not** partner momentum | EXACT after port |
| Combined-track dedup | port builder PARITY 2 rule (1562-1571). v5 has no dedup | EXACT after port |
| Track fields (Pt, Eta, NHitDedx, RadLength, DcaPrime, IsFromPrimary, IsCentralTrk, TrackType, charge, P/Px/Py/Pz, Phi) | `H1PartSelTrack` getters (v5 records most; add TrackType/IsCentralTrk/P-components) | EXACT |
| IsScatElec skip | `H1PartCand::IsScatElec()` (v5 line 102) | EXACT |
| ParticleSource (continuity only) | port `classifySourceGTR` (utils 84-109) — audit: never returns kSecondaryInteraction for real input; recorded, not used for classification | EXACT after port |
| Alpha cone angle (auxiliary) | lab angle between track momentum and HAT scatE direction — recorded per row so the alpha0p2-conditioned subset is derivable offline; primary reporting is central (no cone) | EXACT |
| Gen-grid membership | offline in aggregation (binning_v3 port, pure Python) from recorded gen kinematics | EXACT (offline) |
| **Tier-2 greedy fallback** (builder 2.0° StrIdx scan, `PhysicsConstants_chargedsidis_highQ2.h:63`) | **NOT recomputable** — builder-side matching, no MODS object. Selection-passing tracks with null Tier-0 link are counted as a separate counter class `n_no_tier0_link_selected` | PROXY (bounded) |

### B.3 Fidelity verdict

**The det.int. flag is EXACTLY recomputable at MODS level** for the Tier-0 population:
event admission is identical by construction (same steer, same event lists, same
native loop), and every label flag and selection ingredient is read from the same
H1OO objects via the same calls the builder uses, provided the port list in §C.2 is
implemented. The single non-recomputable piece is the builder's Tier-2 fallback:
a det.int. row that the v6 tree matched only via Tier-2 appears in the study as a
selection-passing track with no Tier-0 link (counted, not parented). Expected
residual: **Tier-2 contributes ~0% of det.int.** (audit empirics: 100% of det.int.
matches were Tier-0 in the single file; Tier-2 rescued ~0.09% of signal overall), so
the study's Tier-0 det.int. population equals the tree's det.int. population to
~10⁻⁴ relative, and the per-unit parity gate (§J) measures the residual directly.

**Mandatory caveat:** the v5 electron study's counterfactual z used
`calc->Elec()->GetFirstElectron()` — a **different object** from the builder's
HAT-pointer scattered electron (`ElecTheta/ElecPhi/ElecE`, calibrated upstream by
stage_evls). Reusing the v5 kinematic path would break selection parity. The design
mandates the HAT-pointer path for every selection ingredient and **drops** the v5
calculator-electron fields from the clone (§C.2, P-5). This is the single most
important parity trap found in the clone-basis source; it is closable by porting, not
a blocker.

### B.4 What must be carried from the v6 selection rather than recomputed

Nothing from the v6 *trees* is read by the study. Everything is recomputed from the
input HAT+MODS stream. The v6 *steering* (per-chunk `fEventListFiles`) is carried
verbatim as the event-admission contract. The v6 *outputs* are used only in the
offline acceptance gates (§J), joined per unit (chunk ↔ output file), never by
run/event ID inside the event loop.

---

## C. Framework and clone basis

### C.1 Reused verbatim from the electron parentage study

Implementation root (new, sibling of the clone basis):
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/diagnostics/detint_parentage_study_NOT_TREEBUILDER/`

Clone basis: `.../diagnostics/electron_parentage_study_NOT_TREEBUILDER/`
(README, v5 source, launcher, schema `electron_parentage_NOT_TREEBUILDER_v3`, tree
`ElectronParentageDiag`, 87 branches, binary
`targeted_electron_parentage_diagnostic_NOT_TREEBUILDER`).

Naming: schema `detint_parentage_NOT_TREEBUILDER_v1`, tree `DetintParentageDiag`,
binary `targeted_detint_parentage_diagnostic_NOT_TREEBUILDER`, launcher
`run_targeted_detint_parentage_diagnostic_NOT_TREEBUILDER.sh`.

Reused unchanged:
- **Positional event-list contract** (README §v3): launcher accepts the supplied v6
  per-chunk sidistree steer, reads only its exact `fEventListFiles`; no EVLS
  override; one `EvlTree` entry per selected `H1Tree::Next()`; exact equality of
  consumed EvlTree entries and scanned selected iterations required; abort on EvlTree
  exhaustion; **no run/event-ID check or alignment join**.
- **HAT+MODS / no-ODS** input mode; no `H1StrBank` — SVX/STR production-vertex fields
  remain −999 sentinels (electron handoff 2026-07-31 confirms unavailability at MODS
  level).
- Counter/TNamed metadata pattern: schema name+version TNamed, source metadata,
  positional-contract TNamed, per-cut rejection counters.
- Campaign wrapper structure: `run_one_clean_v2.sh` pattern (9 positional args,
  `desygroupset h1`, H1OO 4.2.1 `thish1.sh`, exec launcher), manifest.tsv column
  contract (`steer, output_path, sample, generator, period, chunk, campaign_unit,
  fingerprint, max_events`), `.sub` + `campaign.args` structure, aggregation
  directory pattern (`aggregate.py`, retry logs, `summary.json`, `DONE.txt` —
  precedent `aggregation/parentage_operational_aggregation_20260731/`).
- Retain-before-cleanup philosophy: rows are recorded with per-cut pass flags; the
  population is defined offline in aggregation. **The v5 "nominal" pass is tree
  admission (Stage-2 nominal semantics), NOT the response selection — it must not be
  used as the population definition.**

### C.2 Required extensions (the port list — implementation-ready)

Every item is a verbatim port from `ChargedSidisTreeBuilderHighQ2_v6.C` (v6) or
`ChargedSidisUtils_highQ2_v6sys.h` (utils), with anchors:

| # | Port | Source anchor | Purpose |
|---|---|---|---|
| P-1 | weight1, weight2 (HAT `Weight1`/`Weight2`; `weight2 = 1.0` for data, but campaign is MC-only) | v6 1039-1049 | scaled yields; response weight convention |
| P-2 | `kine->GetYes()` | v6 1065 | event cut Yes ∈ (0.1,0.7) |
| P-3 | Empz derivation from HAT `Ys` + electron terms | v6 1171-1177 | event cut Empz > 45 |
| P-4 | PVz (HAT `VtxZ`) | v6 437 | event cleanup |PVz| < 35 |
| P-5 | eSigma SIDIS block: scatE 4-vector from HAT `ElecTheta/ElecPhi/ElecE` + beams; `EprimeES = Q2e/(2·eBeamE·(1+cos eTheta))` else eE; γ*, boosts; xF_SIDIS_es, z_SIDIS_es | v6 1122-1134, 1179-1290 | xF < 0 cut; z_outlier; z/pT breakdowns. **Use HAT path, drop v5 calculator-electron z** |
| P-6 | Gen event block: HAT `GenThElec/GenPhElec/GenEnElec/GenEnHad/GenPzHad`, Q2_es_gen, Y_es_gen, x_es_gen, GenEmpz (fallback formula), GenEnPhoton (FIX V5G-2), validGenElec, gen beams, boosts | v6 453-465, gen block ~1280-1375 | matched-block membership (offline) |
| P-7 | Matched-partner kinematics: GenP, GenPt, GenEta, GenPhi, GenCharge, zGen, xFGen, inFiducialChargedMatch | v6 1731-1841 | matched-block membership; breakdowns |
| P-8 | Veto walk: `findVetoAncestorPDG` (6 hops) + IsMatchedV0Daughter / IsMatchedConvOrDalitz / IsMatchedK0LDaughter / IsMatchedDIFMuon flags + walk depth | utils 181-197; v6 1762-1779 | det.int. label; em sanity category |
| P-9 | Ancestor chain extended to 6 levels (levels 1-3 as full Node clones per v5; levels 4-6 PDG+status only) | v5 Node + P-8 depth | offline re-derivation of any category convention |
| P-10 | NucliaWeight, species-refined on partner PDG | v6 1639-1650, 1830-1840 | weighted fractions |
| P-11 | Combined-track dedup (same charge, ΔR < 0.05) | v6 1562-1571 | population parity for Pass-2 rows |
| P-12 | `classifySourceGTR` | utils 84-109 | continuity field only |
| P-13 | Alpha cone angle (track vs HAT scatE, lab) | new, trivial | alpha0p2-conditioned comparison offline |
| P-14 | **Forbidden:** `H1PartCand::GetTrack()` / TrackLength on MC (FPACK abort, audit U4) | v6 1622-1633 sentinel | risk avoidance |

**Dropped from the v5 clone (electron-specific):** electron MLP; electron
conversion-pair sibling test as primary topology (replaced by generalized sibling
counts, §D); the calculator-electron counterfactual z and canonical pT/z fields
(P-5 supersedes; avoids the divergence trap).

---

## D. Schema v1 branch list (design item 2)

Tree `DetintParentageDiag`, one row per retained track (retain-before-cleanup), with
per-row denormalized event fields exactly as the v6 `ChargedTot` convention. Groups:

**G1 Identity/metadata (per file, TNamed + counters):** schema
`detint_parentage_NOT_TREEBUILDER_v1`; source steer path, campaign_unit,
fingerprint, generator, period, chunk, max_events; positional-contract TNamed;
counters: n_events_scanned == n_evl_consumed (abort on mismatch/exhaustion),
n_tracks_scanned, n_matched_tier0, n_matched_tier0_hadron (|PDG| ∈ pool),
n_detint_core (label core: partner IsStr && !IsGtr && hadron && !e && !V0),
n_detint_selected (core + full response selection, offline-defined but counted for
QA), n_no_tier0_link_selected, n_v0_ancestor_in_detint (sanity, expect 0),
per-cut rejection counters (v5 pattern).

**G2 Reco track (clone from v5 + additions):** P, Px, Py, Pz, Pt, Eta, Phi, Theta,
Charge, NHitDedx, RadLength (cm), DcaPrime, DcaPrime/cosh(Eta), IsFromPrimary,
IsCentralTrk, TrackType (add), IsCombined (add), IsScatElec, is_track pass flags
per cut (Pt, Eta, NHitDedx, RadLength, DcaCosh, class, finite) — recorded, applied
offline.

**G3 Matched partner Node (clone v5, + momentum):** partner idx, PDG, status,
is_gtr, is_str, stridx, SVX/STR sentinel fields (−999, kept for schema continuity
and the future ODS pilot); **add:** GenP, GenPt, GenEta, GenPhi, GenCharge, zGen,
xFGen, inFiducialChargedMatch.

**G4 Parentage chain:** GenMotherPDG, GenMotherStatus, GenMotherIsGtr, GenMotherIsStr
(clone); ancestor levels 1-3 as Node clones (idx/PDG/status/is_gtr/is_str/stridx +
sentinels) (clone); **add:** ancestor levels 4-6 (PDG, status only);
veto_ancestor_pdg, veto_walk_depth, IsMatchedV0Daughter, IsMatchedConvOrDalitz,
IsMatchedK0LDaughter, IsMatchedDIFMuon; ParticleSource (P-12).

**G5 Sibling topology (generalized):** n_same_mother_charged_siblings,
n_opposite_sign_same_mother_siblings, has_same_mother_hadron_sibling,
has_opposite_sign_electron_sibling (keeps the conversion-pair cross-check for the
em category).

**G6 Event reco (add, denormalized per row):** Q2es, Yes, Xes, Empz (P-3), PVz,
Q2e, eBeamE, pBeamP, scatE 4-vector (Px,Py,Pz,E), xF_SIDIS_es, z_SIDIS_es,
z_outlier flag, alpha cone angle (P-13), Q2s/Ys (HAT calibrated, for the Empz port
cross-check).

**G7 Event gen (add, denormalized per row):** Q2_es_gen, Y_es_gen, x_es_gen,
GenEmpz, GenEnPhoton, validGenElec, GenElec (E, Theta, Phi), GenBeamElecE,
GenBeamProtP, xFGen event flag ingredients.

**G8 Weights (add):** weight1, weight2, NucliaWeight.

**G9 Selection composition (add, recorded flags only — never applied in-loop):**
pass_evt_fid, pass_evt_cleanup, pass_trk, pass_track_class, pass_xF, pass_z,
detint_core, detint_label (== detint_core && Tier-0), selected_full
(== det.int. label + full response selection), matched_block_ingredients
(gen event cuts + species + GenPt; grid applied offline).

Row count estimate: the retained population is every Tier-0 matched charged track
(hadron pool + electrons + muons, pre-cleanup) plus counters for the unmatched —
sized like the electron study (which retained all matched |PDG|==11); per-unit
output files of order a few MB, no size risk.

---

## E. Parentage categories (design item 3)

### E.1 The ambiguity to be handled explicitly

GenMotherPDG at STR level is **probabilistic in interpretation** (audit D/E): a
K-mother π is K-DIF *or* K inelastic interaction — indistinguishable without vertex
information at MODS level; a starred-hyperon mother (Σ*, Ξ*) is decay-delivered but
often interaction-initiated; the H1ModsFiller error fallback sets Mo1 = 1, making a
genuine electron-mother secondary degenerate with the fallback signature
(`H1ModsFiller.C:224-287`). **Handling: no forcing.** Priority-ordered assignment to
exactly one primary category, explicit mixed/unresolved classes, all six ancestor
PDGs recorded so any alternative convention is re-derivable offline, plus dual
decay/interaction tags.

### E.2 Operational cut table (priority order; from recorded fields only)

Assignment rule: walk mother → ancestor1 → … → ancestor6; the **first level whose
PDG falls in any decision set determines the category** (within that level, the
highest-priority set wins); no informative level → C6. PDG codes are signed where
charged; sets use |PDG| unless noted.

| Priority | Category | Operational definition (recorded fields only) | Physics class | Single-file proxy (PROVISIONAL, n=122) |
|---|---|---|---|---|
| C0 | `v0_ancestor` | veto_ancestor_pdg ∈ {±310, ±130, ±311, ±3122} | (sanity gate) | **expected 0** — nonzero = veto-walk replication bug |
| C1 | `dalitz_ancestor` | nearest informative ancestor |PDG| ∈ {111, 221, 331, 443} (π⁰, η, η′, J/ψ 3-body decays) | decay-born | ~5% (6/122 had VetoAncestorPDG = 221, η) |
| C2 | `hyperon_decay` | nearest informative ancestor |PDG| ∈ {3122, 3212, 3222, 3112, 3312, 3322, 3334} ∪ {3214, 3224, 3114, 3314, 3324} (Λ, Σ⁰/±, Ξ⁰/±, Ω⁻; Σ*/Ξ* starred) | decay-born (starred: decay-delivered, possibly interaction-initiated — dual-tagged) | ~62% (hyperon mothers dominated) |
| C3 | `k_parented` | nearest informative ancestor |PDG| == 321 (K±) | **mixed: K-DIF and K inelastic interaction** — never split at MODS level | ~8% |
| C4 | `em_initiated` | nearest informative ancestor PDG == 22 (photonuclear) | interaction-born (em-initiated) | small |
| C5 | `hadronic_interaction` | nearest informative ancestor |PDG| ∈ {211, 2212, 2112} (π±, p, n; includes knock-out of material nucleons and Δ/N* intermediates — if the walk terminates inside the 1xxx/2xxx baryon-resonance families without reaching a decay-set member, assign here) | interaction-born | ~30% (meson-baryon mothers minus K) |
| C6 | `unresolved` | no valid chain: mother idx < 0, out of array bounds, PDG == 0, or mother == 11 (Mo1=1 fallback degeneracy) | unresolved — never forced | small (0 in proxy top-mothers) |

Notes:
- C0 is a **sanity category**: the det.int. label requires IsMatchedV0Daughter==0,
  so V0-ancestor membership must be zero; it exists to gate the veto-walk port.
- η-ancestor π± (η → π⁺π⁻π⁰) pass the V0 veto (η sets ConvOrDalitz, not
  V0Daughter) and land in C1 — decay-born, as observed in the proxy.
- C3 is reported as its own category in every table; headline variants that
  include/exclude it are specified in §F.
- Dual tags recorded per row: `is_decay_born` (C1|C2), `is_interaction_born`
  (C4|C5), `is_k_parented` (C3), `is_unresolved` (C6).

### E.3 What this classification cannot do (stated limits)

Without vertex information (MODS level), decay-vs-interaction is inferred from
ancestor species only. U2 (GEANT3 same-track/new-track semantics) underpins the
reading "a new STR record with hadron mother = interaction product". The ODS SVX
pilot (§H) is the path to a vertex-level split; until then, C2-starred, C3, and the
C5 interpretation carry the stated ambiguities.

---

## F. Denominators and reporting (design item 4)

**Strata:** generator (rapgap31, djangoh14) × period (ep0304, em0405, em06,
ep0607) × charge (h⁻ = charge < 0, h⁺ = charge > 0).

**Populations (offline, from recorded flags):**
- N_detint_selected: Tier-0 det.int. rows passing the full central response
  selection (§B.1 items 1-3).
- N_detint_matchedblock / N_detint_fakecolumn: sub-split by gen-event cuts +
  species + GenPt + in-grid (§B.1 item 4). This re-derives the 86.18/13.82
  partition under the **central (no-cone) selection** (secondary deliverable; the
  published partition was alpha0p2 — the recorded alpha angle allows the
  alpha0p2-conditioned comparison as a cross-check).

**Headline:** per stratum,
- f_int(strict) = N(C5) / N_detint_matchedblock — primary;
- variants: f(C5+C4), f(C5+C4+C3), f(non-decay) = 1 − f(C1+C2);
- unweighted with binomial σ = sqrt(f(1−f)/N);
- weighted variant: f_w = Σw(C5)/Σw(all), w = weight1·weight2·NucliaWeight,
  σ_w = sqrt(f_w(1−f_w)/N_eff), N_eff = (Σw)²/Σ(w²).

**Scaled yields (reporting continuity):** per category and stratum,
Y = Σ(w1·w2·NW) · (L / L_MC) with L = 351000.0 nb⁻¹ and L_MC = 9132891.0 nb⁻¹
(RAPGAP31) / 10487495.0 nb⁻¹ (DJANGOH14) (STATE.md). Report per-generator yields
and the two-generator average consistent with the background-template convention
(the existing det.int. template total 27405.8031 is a generator average — same
convention, restated in the output metadata).

**Breakdowns — FEASIBLE at MODS level** (all ingredients recorded):
- per (Q2,x) coarse cell (7 cells; from recorded Q2es/Xes, response cell decode);
- per coarse gen (z,pT) bin (245-bin grid, offline binning_v3 port from recorded
  zGen/GenPt/GenEta);
- thresholds: report per-cell fractions only where N_detint_matchedblock ≥ 50
  (aggregated over units first); sparser cells are tabulated with counts only.

**Aggregation precedent:** replicate `aggregation/parentage_operational_aggregation_20260731/`
(aggregate.py + retry condor + summary.json + DONE.txt).

---

## G. Coverage plan (design item 5)

- **Reuse the 865-unit campaign structure verbatim**: same manifest unit IDs and
  steer paths as `electron_parentage_hATmods_full4p_cleanrun_20260731/manifest.tsv`
  (columns: steer, output_path, sample, generator, period, chunk, campaign_unit,
  fingerprint, max_events=0), new output root under the new study's `outputs/`.
- **Expected PNFS risk:** 191/865 units (22.0%) failed in July 2026 with missing
  PNFS MODS inputs (cluster 4655647: 674/865 valid). /pnfs reliability is degraded
  (workspace note 2026-05-22) but the DIFFVM campaign (2026-10-07) read /pnfs
  HAT+MODS successfully — availability is intermittent, not absent.
- **Plan:**
  1. Full 865-unit submission (same `.sub`/`campaign.args` pattern).
  2. Up to **two retry passes** for missing/failed units (fresh submission; the
     aggregation precedent already has a retry-condor pattern).
  3. Coverage table per generator × period reported alongside every headline number.
  4. Coverage-bias checks: f_int stability vs cumulative unit coverage; per-period
     jackknife; early-vs-late unit comparison.
  5. **Coverage-corrected denominator:** for units whose PNFS input is unavailable,
     the v6 sidistree output exists (production completed; verified for
     djangoh14/7091: 100+ per-chunk files present under
     `output_v6/central/sidistree/mc/djangoh14/7091/`). The v6-tree-side det.int.
     count per missing unit (computable with the identical offline selection — §J)
     contributes to the full-sample N_detint; the parentage *fraction* transfer
     from covered to missing units is then the only assumption. If coverage < 80%,
     the transferred full-sample headline is flagged PROVISIONAL with the transfer
     assumption stated.

---

## H. ODS SVX pilot (design item 6) — separate, not part of the campaign

**Scope.** Single-file feasibility check on the login node (≤ 5 min, one input —
per workspace policy), answering: can SVX production-vertex X/Y/Z/Ivtype be accessed
per STR track at ODS/DST level, joined to STR via Itrhis, for one central-MC input?

**Steps:**
1. Inventory (listing only, no read): locate ODS/DST-level files for one central-MC
   runid (djangoh14 7091) under the /pnfs oo-4.0 MC trees.
2. Open one file under H1OO 4.2.1 on the login node; instantiate `H1StrBankPtr` and
   `H1SvxBankPtr` (`H1OOBanks/ODS/H1Str.h`, `H1Svx.h`); join STR↔SVX via Itrhis; read
   per-STR-track X/Y/Z/Ivtype.
3. If the login-node pass succeeds and scale is ever wanted: ONE single-node Condor
   bank-access test before anything further (batch-node StrBank/FPACK risk, below).

**Pass criteria (all required):**
- file opens; STR and SVX banks present and non-empty;
- Itrhis join resolves ≥ 99% of STR rows;
- production-vertex radii physical: GTR-linked STR tracks cluster at the primary
  vertex (r ≲ 5 cm); STR-only tracks show a displaced distribution;
- runtime ≤ 5 min for the one file; no FPACK/libdcap abort.

**Fail criteria / risk:** the known batch-node STR-bank/FPACK risk class (audit R6 /
U4 family): ODS/FPACK access requires libdcap.so, missing on Alma9 nodes;
`H1PartCand::GetTrack()` aborts on MC (audit U4) — the pilot must not call it. If
the pilot fails (file unavailable, banks absent, join broken, abort), the
vertex-level decay/interaction split is documented as BLOCKED; the main campaign
proceeds with the mother/ancestor categories of §E and their stated ambiguities;
the schema's SVX sentinel fields remain −999.

**What a pass would unlock (not required for the campaign):** vertex-level category
validation (displaced production vertex ⇒ decay-born; kink along parent trajectory
or material-map intersection ⇒ interaction-born), partial closure of U2 and U3, and
a potential C3 (K-DIF vs interaction) splitter.

---

## I. Risk register (design item 7)

Carried from the audit (U1-U4) where they touch this study, plus new items:

| ID | Risk | Impact on this study | Mitigation |
|---|---|---|---|
| U1 | DTRY "best STR track" rule details (majority-hits?) UNVERIFIED | Affects the det.int. class *boundary* (kink hybrids), not the composition within the label | Carried unchanged; the study measures composition within the existing label; smoke parity gate bounds any boundary effect |
| U2 | GEANT3 elastic-same-track / inelastic-new-track mapping UNVERIFIED | Underpins the decay-vs-interaction *interpretation* of categories C4/C5 (a new STR record with hadron mother is read as interaction-born) | Carried; stated limit §E.3; SVX pilot would partially close |
| U3 | STR 4-vector = production (not decay-end) kinematics UNVERIFIED | Affects recorded matched-STR kinematics; no flight-distance reasoning is used at MODS level | Carried; SVX pilot would partially close |
| U4 | `H1PartCand::GetTrack()` FPACK abort on MC | Would crash the study binary | Forbidden (P-14); no TrackLength use |
| R1 | **Selection-parity port risk** (P-1..P-11): a ported formula diverging from the builder silently biases the population | Highest implementation risk | Smoke-file row-level parity vs the v6 tree (§J gate G2): exact match required on Q2es/Yes/Empz/PVz/xF_SIDIS_es/z_SIDIS_es/weight1/weight2/NucliaWeight and on the det.int. count (122) |
| R2 | Calculator-electron vs HAT-electron divergence (v5 pattern) | Would break xF/z parity if the v5 path were reused | Design mandates HAT path; v5 fields dropped (P-5) |
| R3 | Tier-2 invisibility | Study counts a det.int. row as no-link where the tree matched it via Tier-2 | Expected ~0% (audit); bounded per unit by `n_no_tier0_link_selected`; parity gate G3 measures the residual |
| R4 | PNFS MODS availability | Up to ~22% unit loss (precedent) | Retry passes; coverage accounting; coverage-corrected denominator (§G.5); PROVISIONAL flag if < 80% |
| R5 | Batch-node bank risk | Campaign is HAT+MODS only — no ODS/FPACK in the campaign; risk applies only to the SVX pilot | Pilot is login-node first; single-node Condor test before any scale (§H) |
| R6 | K-parent (C3) and starred-hyperon (C2) ambiguity | Misassignment if forced | Explicit categories, never forced; dual tags; six ancestor levels recorded for offline re-derivation |
| R7 | H1ModsFiller Mo1=1 fallback degeneracy (mother==11) | e-mother secondaries indistinguishable from filler error | C6 `unresolved`, never forced into interaction |
| R8 | Generator dependence of composition | RAPGAP31 vs DJANGOH14 hyperon/interaction rates differ; a large f_int disagreement would be a model-sensitivity caveat for the in-observable treatment | Per-generator reporting is part of the deliverable; cross-generator comparison tabulated; disagreement flagged, not averaged away |
| R9 | Single-file proxy magnitudes | All proxy fractions (62/30/8/5%) are n=122, one DJANGOH14 file, unweighted, central mirror | Full-sample campaign is the purpose of this study; proxy numbers labeled PROVISIONAL everywhere |

---

## J. Acceptance gates (design item 8)

**G1 — Per-unit validity (electron-study pattern):**
- schema TNamed exactly `detint_parentage_NOT_TREEBUILDER_v1`;
- source-metadata and positional-contract TNameds present;
- counters: n_events_scanned == n_evl_consumed (abort/exit-nonzero on mismatch or
  EvlTree exhaustion; max_events=0 for campaign units);
- output uniqueness: one output file per manifest unit, name keyed by campaign_unit;
  no duplicate/missing outputs among terminated units;
- fingerprint recorded and equal to the manifest fingerprint.

**G2 — Pre-campaign smoke gate (login node, one file):**
the audit file `output_v6/central/sidistree/mc/djangoh14/7091/sidistree_djangoh14_7091_ep0607_0200.root`'s
input chunk must yield **exactly 122 Tier-0 det.int. rows** under the central
mirror (audit C.3; Tier-2 = 0 in that chunk), plus row-level parity of selection
ingredients against that tree file's branches (Q2es, Yes, Empz, PVz, xF_SIDIS_es,
z_SIDIS_es, weight1, weight2, NucliaWeight). Any mismatch blocks the campaign.

**G3 — Campaign self-consistency, per unit (offline, positional-by-unit):**
the study's per-unit Tier-0 det.int. count vs the **same-unit v6 sidistree file's**
det.int. count, computed with **one shared offline selection implementation applied
to both outputs**. The unit↔file mapping is derived from the manifest steer path
(`steering_v6/central/sidistree/mc/<gen>/<runid>/sidistree_<gen>_<runid>_<period>_<chunk>.steer`
↔ `output_v6/central/sidistree/mc/<gen>/<runid>/sidistree_<gen>_<runid>_<period>_<chunk>.root`).
This is a **unit-level join, not a run/event-ID join** — the no-join rule governs
only the in-loop EvlTree contract (README §v3), and this comparison happens in
aggregation, outside the event loop. Expected: exact equality where Tier-2 det.int.
rows are zero (residual = tree-side Tier-2 det.int. rows = study-side
`n_no_tier0_link_selected` det.int. subset). Gate: ≥ 95% of units exactly equal;
no unit > 0.2% relative mismatch without quarantine and investigation.

**G4 — Aggregation gates:**
- C0 `v0_ancestor` count == 0 in every stratum (veto-walk port sanity; nonzero =
  replication bug, blocks release);
- category fractions sum to 1 per stratum; dual tags consistent with categories;
- coverage table reported with every headline; jackknife stability recorded;
- weighted and unweighted fractions reported side by side; N_eff recorded;
- scaled yields cross-checked against the existing det.int. template total
  (27405.8031, generator-average convention) within MC-statistics — a large
  discrepancy indicates a population-definition drift, and blocks release.

**G5 — Contract gates:** no central product written (quarantine audit of output
paths); no GetTrack() calls (code review); HAT+MODS only (no ODS banks opened in
the campaign binary).

---

## K. ESTABLISHED / PROVISIONAL / UNVERIFIED register

**ESTABLISHED (source-backed):**
- Tier-0 link mechanism and IsStr/IsGtr semantics (H1CreateSelTracks.C:115-145;
  H1PartMC.C:187-196; builder 1688-1696);
- response selection cut values and structure (build_migration_th2_per_charge_v4.py
  238-273, 606-659, 677, 700);
- builder kinematic paths to port (H1CalcKine getters 1057-1069; HAT pointers
  1039-1049, 1122-1134; Empz derivation 1171-1177; eSigma block 1179+; gen pointers
  453-465);
- v5 framework contract, schema structure, campaign/aggregation precedents
  (README, v5 source, manifest.tsv, run_one_clean_v2.sh, aggregation dir);
- v6 MC sidistree per-chunk outputs exist (verified for djangoh14/7091);
- SVX production vertex absent at MODS level (electron handoff 2026-07-31);
- H1ModsFiller Mo1 fallback (H1ModsFiller.C:224-287);
- L_MC values, four periods, central response = RAPGAP31 + DJANGOH14 cross-check
  (STATE.md).

**PROVISIONAL (single-file proxy, n=122, DJANGOH14 7091 ep0607_0200, unweighted,
central mirror):** det.int. share 0.833% of selected tracks; unmatched 0.020%;
mother composition ~62% hyperon / ~30% interaction-candidates / ~8% K-parented;
6/122 η-ancestor; Tier-2 = 0 in that file; all category expectations in §E.2.
All to be superseded by the campaign.

**UNVERIFIED (carried, with the closing check):**
- U1 DTRY best-track rule — close by reading H1OO track-selection internals or an
  instrumented single-file test (not required for this study);
- U2 GEANT3 track semantics — partially closable by the ODS SVX pilot (§H);
- U3 STR 4-vector meaning — closable by the ODS SVX pilot;
- U4 GetTrack abort — avoidance is verified by code review (G5);
- Tier-2 det.int. rate at full sample — measured by gate G3;
- PNFS availability at campaign time — measured by the coverage table.

---

## L. Implementation sequence (recipe)

1. Create the study root; clone the v5 source, launcher, Makefile; rename schema/
   tree/binary; strip electron-specific fields; implement P-1..P-13 ports.
2. Login-node smoke on the djangoh14 7091 ep0607_0200 chunk; pass gate G2 (122;
   ingredient parity vs the v6 tree file).
3. Single-unit Condor smoke (one unit per generator), gate G1 + G3 on those units.
4. ODS SVX pilot (§H) — independent track, login node, may proceed in parallel.
5. Build the 865-unit manifest (clone paths, new output root), submit campaign,
   run retry passes, aggregate (coverage table, categories, headline, breakdowns),
   run gates G3/G4.
6. Write the results handoff next to this design; update the backgrounds ledger
   with the central-selection det.int. partition and the f_int measurement.

---

## Provenance

- Audit: `sidis_highQ2/docs/backgrounds/detint_primary_vs_material_separation_2026-10-08.md`
  (sections A-F; U1-U4 in F; empirical proxy in C.3).
- Ledger (authoritative): `sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`.
- Clone basis: `H1ChargedSidis_HighQ2/diagnostics/electron_parentage_study_NOT_TREEBUILDER/`
  (README_NOT_TREEBUILDER.md; targeted_electron_parentage_diagnostic_NOT_TREEBUILDER_v5.C;
  run_targeted_electron_parentage_diagnostic_NOT_TREEBUILDER_v5.sh; Makefile.v4.change-note).
- Campaign precedent: `H1ChargedSidis_HighQ2/condor_v6/central/qa/electron_parentage_hATmods_full4p_cleanrun_20260731/`
  (manifest.tsv 865 units; run_one_clean_v2.sh; .sub files).
- Outputs precedent: `.../electron_parentage_study_NOT_TREEBUILDER/outputs/full_v6_4period_cleanrun_20260731/`
  (cluster 4655647; 674/865 valid, 191 missing PNFS MODS);
  `.../aggregation/parentage_operational_aggregation_20260731/`.
- v6 Stage-2 builder: `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`
  (anchors cited inline).
- Response builder: `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`.
- Utils: `H1ChargedSidis_HighQ2/src/ChargedSidisUtils_highQ2_v6sys.h`.
- Partition artifact: `output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/detint_response_partition_20260626/detint_response_partition_summary.json`.
- Electron-study handoff: `sidis_highQ2/docs/diagnostics/electron_parentage_hATmods_cleanrun_2026-07-31.md`.
- H1OO 4.2.1 source: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`.
- STATE.md MC facts: central response RAPGAP31 + DJANGOH14 cross-check; periods
  ep0304/em0405/em06/ep0607; L_MC 9132891.0 / 10487495.0 nb⁻¹; L = 351000.0 nb⁻¹
  (user directive, STATE-consistent).

# Detint-fake response-variant specification — v6/z005 high-Q2 inclusive charged-hadron SIDIS

- **Spec date:** 2026-10-08. **Rev 2 amended 2026-10-09: FINAL CONVENTION ENCODED
  (USER-DECIDED 2026-10-09)** — the refined-definition detint-fake variant (S2
  construction with the k_parented carve-out) is ADOPTED per Sec. 0; S1/S2/S3-as-written
  are superseded/historical. All critic-signed Rev 1 elements (miss-fill suppression,
  driver repointing R10, G6/G9 fixes, R1 second-order notes) are retained unchanged.
  This revision is a SPEC AMENDMENT only — nothing was rebuilt or launched by it.
- **Nothing was rebuilt, launched, or modified by this spec.** No central or diagnostic
  product was touched. Read-only study; the single deliverable is this file.
- **Scope:** define the observable-meaning options for the detector-interaction (STR-born)
  charged-hadron secondary class, specify the response-variant construction that would
  remove it ("detint-fake"), the updated exactly-once ledger under such a variant, the
  mandatory validation gates, the production plan, the decay-feed-down fork, and the risk
  register. Luminosity `L_data = 351000.0 nb^-1` throughout (never 356613.0 or 297657.8).
- **Inputs consumed (authoritative):**
  - Consistency ledger 2026-10-07 (+ JSON sidecar in `data/backgrounds/`).
  - Class-separation audit 2026-10-08:
    `docs/backgrounds/detint_primary_vs_material_separation_2026-10-08.md` (the
    authoritative input for Class-1/Class-2 semantics and separability).
  - `STATE.md` (2026-10-08 state; backgrounds + blockers sections).
- **User decisions already fixed (this spec does not reopen them):**
  - Option A adopted: central response unchanged in all other respects; external per-bin
    additive track-level vectors for residual electrons, V0 daughters, low-Q2 applied
    pre-unfold; DIFFVM systematic-only; e∩V0 overlap 112.2093 IGNORED (user decision
    2026-10-08; max 0.0364% double-subtraction, accepted).
  - User physics definition: **Class 1** (primary-vertex tracks that interact in material
    and continue) = SIGNAL, stays; **Class 2** (new particles from inelastic scattering of
    a track on detector material) = FAKES, to remove.
  - **FINAL RULING (USER-DECIDED 2026-10-09):** REMOVE the refined-definition removable
    det.int. content via the response variant, with the **k_parented carve-out**
    (charged-K-mothered rows kept as signal). Encoded as the adopted convention in
    Sec. 0; supersedes the options framing of Sec. 1.
- **Audit facts this spec builds on (ESTABLISHED unless flagged):** Class 1 stays signal
  end-to-end today (GTR link; energy loss absorbed as reco-gen migration; NucliaWeight
  corrects complementary primary loss; no change needed). Class 2 = the existing det.int.
  label (`IsStr==1 && IsGtr==0 && IsMatchedHadron==1` + e/V0 vetoes), matched at Tier 0 to
  its OWN STR record (100% of checked det.int. rows). The central response matched fill has
  NO `IsGtr` requirement, so 86.18% of Class 2 unfolds as signal at the secondary's own gen
  bin (in-observable, integrated 0.7056% of the data vector, worst bin 2.20% h+ b420); the
  13.82% fake-column part is ~92% gen-event Q2/y failures, not unmatched tracks. At the
  label level ~100% of the matched det.int. block is Class 2. Data-side per-track separation
  is impossible (flags MC-only; 90.2% of det.int. survives |DcaPrime|<1 cm vs 99.5% of
  signal), so Class-2 removal must be STATISTICAL via the response fake column (enlarged
  `f_i`), not per-track data cuts.

---

## 0. Final Convention (USER-DECIDED 2026-10-09)

The user decision on the det.int. observable exclusion was made on 2026-10-09, after the
full-sample parentage campaign (cluster 5433299, 865 units, all gates PASS) and the
mother-species re-slice (cluster 5435475) delivered the class decomposition. The adopted
convention is the **refined-definition variant**: the S2 construction WITH a
**k_parented carve-out** (below, ruling 2). The rulings are the adopted convention; each
is USER-DECIDED 2026-10-09 and this spec does not reopen them. Sections 1-9 are amended
accordingly; pre-decision text is retained as superseded/historical context with status
markers.

1. **REMOVE (refined removable):** interaction-born det.int. rows (parentage category
   `hadronic_interaction` = C4+C5; **34.47%** of the block; pooled 391,825 of 1,136,839
   selected rows) AND decay-born rows (C1+C2, 56.53%) whose direct mother is NOT a
   charged π/K/p — measured **100.000% hyperon mothers** (Σ± 86.8%, Ξ 12.6%, Ω 0.57% of
   the decay-born part; ZERO charged-π/K/p-mother rows exist inside the DECAY-BORN C1+C2
   class, and ZERO neutral-K-mother rows exist inside det.int.; the charged-K-mother
   content of det.int. is EXACTLY the k_parented class (102,406 rows), which the keeper
   retains — so "zero π/K/p-mother rows inside det.int." must be read with this scope;
   no Λ mothers — the `IsMatchedV0Daughter` exclusion removes the
   Λ→pπ and K0S→ππ channels by construction).
2. **KEEP AS SIGNAL — k_parented carve-out:** the k_parented class (C3 = **102,406**
   pooled rows; **9.01%** of the block; mother species **100% charged kaons** — K−
   51,525 / K+ 50,881; mother record 99.72% GTR = primary-K decay-in-flight, 0.28% STR =
   interaction-produced K decays; the daughter's decay-vs-interaction mechanism is NOT
   split, per the campaign's never-split design). "Pions or kaons which decay are still
   signal" — charged K only. **Construction:** the reroute condition (Sec. 2.1) becomes
   `is_detint & matched & NOT keeper`, with the keeper = direct-mother PDG ∈
   **{+321, −321} ONLY** (K+/K−). The exact PDG list is mother-level ±321 only: the
   k_parented composition CSV shows K*/D0/φ solely in anc1 (the parent K's own
   fragmentation ancestry), never at mother level, so a mother-level condition
   reproduces the 102,406-row class exactly and a deep-ancestor match would NOT. Access
   path (verified this amendment): the v6 trees already carry per-reco-leg `GenMotherPDG`
   (tree builder lines 1746-1750, filled in the same matched-candidate block as the
   det.int. flags and the `findVetoAncestorPDG` walk — the machinery family the user
   directed to use; the DIF-muon flag at line 1778 is the standing mother-PDG-on-
   reco-leg precedent). NO tree re-production. Per-generator attribution note: the
   parentage doc §2 quotes C3 DJ 56,229 / RG 46,177 while the kparented CSV gives
   DJ 56,228 / RG 46,178 — pooled identical at 102,406; the ±1 per generator is
   file-set bookkeeping, not a classification ambiguity. Honest caveat on the kept
   class: within the 99.72% GTR-K majority the daughter's decay-vs-interaction
   mechanism is never-split, so the kept 9.01% contains K-nuclear-interaction
   secondaries (≤0.064% of the data vector in total) — quantifiable later via the
   recorded `mother_svx_*`/itrhis handles if ever needed (Sec. 6).
3. **NEUTRAL KAONS EXCLUDED from the signal-mother rule:** K0S/K0L/K0-mother feed-down
   is NOT "kaons which decay are still signal"; it stays in the V0 background class,
   flag species 310/130/311/3122 unchanged; the V0 external-vector treatment is
   unchanged (Option A). Measured: 0 neutral-K-mother rows inside det.int.
   campaign-wide — numerically inert for the det.int. block.
4. **Observable meaning after the variant:** charged tracks from the primary-interaction
   lineage (charged π/K/p), with charged-K decay-in-flight kept as signal; GEANT
   material-interaction secondaries and non-charged-π/K/p-mother hyperon feed-down
   removed statistically via the enlarged `f_i`; V0/e/low-Q2 handled by the unchanged
   Option-A external vectors; DIFFVM systematic-only.
5. **Removable fraction of the det.int. block: 0.909920 ± 0.000269** (k_parented kept;
   DJANGOH14 0.912611, RAPGAP31 0.906412; w_b 0.909155 ± 0.000321, w_a 0.909463 ±
   0.000321). Raw removable ≈ 0.9099 × 0.7056% ≈ **0.642%** of the data vector
   (definition level; central-selection in-grid cross-check 24,903.39 expected events =
   0.641%). ALL predicted shifts and `f_i` growth numbers are recomputed for 0.9099 of
   the block in Sec. 2.5 and Sec. 4 (G9/G10); the prior whole-block S2 numbers are
   superseded there and labeled as the S2-as-written envelope.
6. **GEN-SIDE OPEN ITEM (do NOT implement in this variant):** truth-level
   charged-hyperon feed-down exclusion from the GEN pool (response truth marginal) is a
   SEPARATE FUTURE DECISION, out of scope here. Feasibility is proven (one light
   `ChargedTotGen`-only pass over the existing 865 v6 tree units, ~20-40 min single
   Condor unit; branches already in the trees); the truth-side extension must scope
   "feed-down" to DISPLACED WEAK DECAYS (hyperons, K0S) — the GTR-side weak-decay
   hyperon content is ~0.5% (prompt Σ*/Ξ*→π strong decays; Λ-mother rows exactly 0,
   generator-stable) and a literal resonance-mother reading would remove ~94% of the
   truth pool (ρ/ω/K*/D0 → π/K fragmentation decays). Recorded as an open item only
   (Sec. 6).
7. **Per-bin reference vector:** the per-bin removable map
   (`perbin_contamination_motherspecies_2026-10-09.csv`, 980 rows charge-major;
   reference column `frac_avg_removable_keepK`, per-generator
   `yield_b_rg/dj_removable_keepK`) is the reference against which the variant's
   realized per-bin removal is gated — new gate **G13** (Sec. 4).

**Status changes propagated:** the option table (Sec. 1) marks the refined-definition
variant ADOPTED and S1/S2/S3-as-written superseded/historical; the S2-vs-S3 fork (Sec. 6)
is RESOLVED (the split is measured); the recommendation (Sec. 9) is superseded by the
decision. The critic-signed construction elements are UNCHANGED by the carve-out: the
miss-row-fill suppression (R1) and its invariants hold identically (keeper legs keep
their central inlined-gen-key consumption; rerouted legs never consumed CTG keys; no
miss fill is added for either); the driver repointing guard (R10) stands; the G6/G9
fixes and the R1 second-order notes stand.

---

## 1. Observable-definition options and consequences

The det.int. class today sits INSIDE the observable by design (the response unfolds the
86.18% matched block as signal at the secondary's own production kinematics; the
acceptance/efficiency normalization includes it in the body). Three definitions of the
published quantity were framed pre-decision; the USER-DECIDED 2026-10-09 convention
(Sec. 0) adopts the refined-definition variant, and the options below are retained as
superseded/historical context with their status marked:

### S1 — Keep current convention (all STR-born secondaries in the observable)

**STATUS: SUPERSEDED 2026-10-09 (historical nominal).** The adopted convention is the
refined-definition variant (Sec. 0); the installed S1 releases remain the standing
nominal until the variant is built, gated (G1-G13), and promoted (R9 hygiene).

- **Exact observable meaning:** "charged hadrons (π/K/p) reconstructed in the central
  selection and acceptance-corrected to truth, where truth includes GEANT STR-born charged
  secondaries as signal." A GEANT secondary is mapped at its own production kinematics
  (never onto an ancestor); an inelastically-interacting primary is accounted as missed
  (CTG-unconsumed, efficiency-corrected, with NucliaWeight compensating absorption), while
  its reconstructable secondary counts as a reconstructed hadron. The det.int. content is
  not an uncorrected background: it is modeled by the response in both the body (86.18%)
  and the fake column (13.82%, removed by `f_i`).
- **Comparability:** fully consistent with the Option-A external e/V0/low-Q2 vectors (they
  subtract reco-space contamination disjoint from the det.int. class by the e/V0 vetoes
  inside the classifier and by source distinctness). Consistent with the prior H1 inclusive
  charged-hadron convention as documented in the workspace
  (`docs/research/research-h1-1302-1321-charged-multiplicity.md`): vertex-fitted tracks;
  K0s/Λ-daughter and γ-conversion residuals treated as subtracted backgrounds/systematics
  (0.2% K0s); hadronic-interaction secondaries handled as a TRACKING-EFFICIENCY systematic
  (1% per central track, derived from inter-CJC secondary-vertex studies, with the MC
  π/K nuclear-interaction cross-section correction) — i.e., secondaries remain INSIDE the
  acceptance-corrected observable. Also consistent in spirit with the workspace gen-loop
  policy (`docs/handoffs/memo-mc-truth-policy-charged-sidis.md`: inclusive charged-hadron
  signal includes all primary fragmentation products reaching the detector as π/K/p,
  hyperon feed-down INCLUDED, PS=4 secondary-interaction excluded-but-vacuous for IsGtr).
- **Numbers today:** complete — this is the installed canonical RAW release
  (`output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/`) and the Born
  release (`born_crad_dj14xfcut_..._2026-10-07/`), integrated raw IBU h+ `11.483372 nb` /
  h− `11.190383 nb`.
- **Required new production:** none.

### S2 — Exclude the ENTIRE det.int. class (material secondaries + decay feed-down together)

**STATUS: SUPERSEDED by the adopted refined-definition variant (USER-DECIDED
2026-10-09).** S2-as-written OVER-REMOVES: the parentage campaign measured 9.01% of the
block as k_parented (100% charged-K mothers, primary-K decay-in-flight), which ruling 2
keeps as signal. S2 remains the measurable whole-label envelope; its construction
(Sec. 2.2 miss-fill suppression, gates, campaign pattern) carries into the adopted
variant unchanged except for the keeper.

- **Exact observable meaning:** "GTR-origin charged hadrons only." Reco-track legs whose
  truth link resolves to an STR-only record (the full Class-2 label: material-interaction
  secondaries AND GEANT-decayed feed-down — hyperon-decay daughters, K decay-in-flight —
  together) are fakes removed by an enlarged `f_i`; the truth marginal (body+miss prior and
  efficiency denominator) is GTR-only. The published quantity changes meaning from "reco
  hadrons including GEANT secondaries, acceptance-corrected" to "hadrons originating from
  the generator record".
- **Label-level fidelity to the user definition:** at the truth-link level ~100% of the
  matched det.int. block is Class 2 (a continuing primary cannot produce an STR-only link
  by construction), so the reroute removes exactly the user's Class 2 as labeled. BUT the
  label is NOT pure material interaction: single-file PROVISIONAL composition (n=122,
  DJANGOH14 7091) — ~62.3% hyperon-decay mothers, ~37.7% meson/baryon-mother (interaction
  candidates + ~8% K-DIF). S2 therefore OVER-REMOVES relative to the strict user physics
  definition (it also removes decay feed-down), and it embeds a generator/GEANT boundary
  artifact into the observable: generator-decayed hyperon daughters ARE GTR π/K/p (kept)
  while GEANT-decayed ones are STR-only (removed), so the split is generator-dependent.
  [Composition note superseded 2026-10-09: the full-sample parentage campaign measured
  the class — 56.53% hyperon-decay / 34.47% interaction-born / 9.01% k_parented
  (Sec. 6) — replacing the n=122 proxy.]
- **Comparability:** the external e/V0/low-Q2 vectors remain UNCHANGED and consistent
  (disjoint classes; see Sec. 3). DIFFVM stays systematic-only. The e∩V0 overlap-ignored
  decision is unaffected. S2 DEPARTS from the prior-H1 convention (secondaries in-observable
  with an efficiency systematic) and from the workspace gen-loop policy's "feed-down is
  signal" stance for the GEANT-decayed subset. Any charge-ratio derived observable shifts
  slightly (h+ and h− removals differ, ≈0.64pp vs ≈0.57pp of their vectors — see Sec. 4
  G10).
- **Numbers today:** the reroute mechanism is prototyped (all-fake variant) but NO
  central-selection (no-cone) detint-fake response exists — the all-fake build
  (cluster 3987932) is alpha0p2-cone AND reroutes e/V0 simultaneously, so it is not the
  S2 product. The 0.7056%/2.20% magnitudes are alpha0p2-cone-conditional (no cone-survival
  counter exists for det.int.; central no-cone magnitude unbounded from above by existing
  artifacts — the variant build itself closes this as a side product).
- **Required new production:** one detint-fake response campaign (RAPGAP31, 73-shard
  scale) + DJANGOH14 cross-generator variant (≈100-shard scale) + merge/validation + IBU/BBB
  release regeneration on the variant response + toys/closure revalidation; `C_rad` tables
  are gen-level GTR-only and UNCHANGED (Born shift = raw shift per bin); if promoted, the
  13-component systematics budget regenerates on the variant nominal. NO tree re-production
  (v6 trees already carry `IsStr/IsGtr/IsMatchedHadron/IsMatchedElectron/IsMatchedV0Daughter`).

### S3 — Exclude interaction-born only (the strict user Class 2)

**STATUS: HISTORICAL (superseded framing, USER-DECIDED 2026-10-09).** The parentage
campaign has since MEASURED the split (34.47% interaction-born of the block); the
adopted variant is defined by mother lineage (rulings 1-2: interaction-born AND
non-π/K/p-mother decay-born removed, k_parented kept), not by S3's interaction-born-only
cut. S3's pre-campaign statements below are retained as written.

- **Exact observable meaning:** "GTR-origin hadrons + GEANT decay feed-down, excluding only
  material-interaction secondaries." Requires splitting Class 2 into interaction-born vs
  decay-born at the track level.
- **Not determinable today:** the interaction-born fraction of the 86.18% matched block is
  NOT-DETERMINABLE from existing artifacts. `GenMotherPDG` alone is only probabilistic
  (π mothers are both interaction products and K→ππ⁰ DIF daughters; Σ/Ξ mothers are decay;
  n/p mothers are interaction candidates); `ParticleSource` cannot flag material
  secondaries in v6; the production vertex is the discriminator and is absent from the
  trees. Determining it needs (a) a full-sample MODS-level det.int. parentage diagnostic
  (mother/ancestor decomposition, analogous to
  `docs/diagnostics/electron_parentage_hATmods_cleanrun_2026-07-31.md`) plus (b) the STR
  production vertex (SVX X/Y/Z/Ivtype exists at ODS bank level; absent from H1PartMC and
  from the MODS-level production inputs, which record SVX/STR fields as −999) — see Sec. 6
  for the refined feasibility picture.
- **Does S3 change any number available today? NO — explicitly.** No existing artifact
  separates interaction-born from decay-born within Class 2 on the full sample; the
  single-file proxy (n=122) is PROVISIONAL and cannot be scaled to per-bin correction
  vectors. Every S3 number requires new production.
- **Required new production:** the parentage diagnostic campaign (+ SVX access, see
  Sec. 6), then either a builder extension staging the split flag into the trees (tree
  re-production) or a diagnostic-side per-track classification joined to the response
  build, then the same variant construction as S2 with the narrower classifier.

### Option table

| Option | Status (2026-10-09) | Observable meaning | Ready today? | Required new production |
|---|---|---|---|---|
| **S2′ refined-definition variant** = S2 construction + **k_parented carve-out** (Sec. 0) | **ADOPTED — USER-DECIDED 2026-10-09** | Charged tracks from the primary-interaction lineage (charged π/K/p), charged-K decay-in-flight KEPT as signal; interaction-born (34.47% of block) + non-π/K/p-mother hyperon feed-down (56.53%) removed via enlarged `f_i` (removable fraction **0.909920** of the block, k_parented kept); V0/e/low-Q2 via unchanged Option-A external vectors; DIFFVM systematic-only | **Response variant: not yet built** (mechanism prototyped; keeper branch verified in the trees; miss-fill suppression specified Sec. 2.2) | Detint-fake response campaign at UNCHANGED scale (RAPGAP31 ~73 + DJANGOH14 ~100 shards; NO tree re-production — `GenMotherPDG` already per-reco-leg), revalidation, dated-candidate release regeneration (R10 driver guard), systematics re-basing if promoted |
| **S1** (keep) | **SUPERSEDED** (historical nominal; installed releases stand until promotion) | Reco hadrons acceptance-corrected to truth, GEANT STR-born secondaries included as signal (in-observable by design) | **YES** — installed RAW + Born releases stand | None |
| **S2** (exclude whole det.int. label) | **SUPERSEDED by S2′** (over-removes: k_parented is signal per ruling 2) | GTR-origin hadrons only; material secondaries AND GEANT decay feed-down removed via enlarged `f_i`; GTR-only truth marginal | **Response variant: no** (mechanism prototyped; central-selection variant not built; ONE spec-level construction fix required, Sec. 2.2) | Superseded by S2′ (same campaign, plus the keeper) |
| **S3** (interaction-born only) | **HISTORICAL** (split since measured; superseded framing) | GTR-origin hadrons + GEANT decay feed-down, material-interaction secondaries only removed | **NO** (was: interaction-born fraction not determinable — MEASURED 2026-10-09 at 34.47%) | Superseded: the parentage diagnostic was delivered; the adopted variant needs no SVX/vertex production |

---

## 2. Response-variant construction (S2; S3 conditionally)

### 2.1 Base builder and the reroute to port

- **Base:** the central NO-CONE v4 finalcandidate response builder
  `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`
  — the builder that produced the canonical release inputs
  (`migration_inputs_v4_finalcandidate_...root` consumed by the release driver; the
  coherent-prior rebuild reproduced it exactly, body max rel dev 6.86e-15/9.42e-15).
  NAMING NOTE: RUNLIST Phase 2 also lists `unfolding/build_migration_th2_per_charge_v6.py`
  — that is the BROAD-GEN-ENVELOPE builder for the native v6 TUnfold diagnostic path
  (Phase 6), NOT the release chain; the detint-fake variant extends the v4 finalcandidate
  builder.
- **Prototype reference (verified this spec, canonical snapshot copy):**
  `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_allfake.py`
  (md5 `f4d9ea1b7121b3dc723fec39d988f2e0`, byte-identical to the study copy at
  `output_v6/studies/alpha0p2_allfake_response_z005_20260627/scripts/`). Line anchors
  verified: `is_detint` classifier **694-697**; `explicit_allfake` **698**;
  `gen_candidate_ok` **843-853**; `matched_gen_ok = gen_candidate_ok & not_explicit_allfake`
  **854-859**; `fake_cat` priority (electron=1 → V0=2 → detint=3 → other=0) **868-874**;
  fake-column category fill with per-reco-bin vectors (`fake_detint[ir]`, `w2` sums)
  **1021-1042**; miss-row fill for reclassified det.int. rows **1043-1066**.
- **PORT into the central v4 builder (the entire adopted S2′ change = S2 reroute +
  k_parented keeper; USER-DECIDED 2026-10-09, Sec. 0 ruling 2):**
  1. `is_detint = (IsStr==1) & (IsGtr==0) & (IsMatchedHadron==1) & (IsMatchedElectron==0)
     & (IsMatchedV0Daughter==0)` (prototype lines 694-697, identical classifier).
  2. **k_parented keeper:** `is_kpar = is_detint & (GenMotherPDG == ±321)` — det.int.-
     tagged matched legs reroute to the fake column UNLESS the direct mother is a
     charged kaon (K+ 321 / K− −321; the exact PDG list is **±321 ONLY** — the mother
     level is 100% ±321 campaign-wide per the kparented composition CSV, and K*/D0/φ
     appear only in anc1, never as mother, so a mother-level condition reproduces the
     102,406-row k_parented class exactly and a deep-ancestor match would NOT). The
     keeper uses the same per-reco-leg ancestor/mother access the builder already has:
     the v6 trees carry per-reco-leg `GenMotherPDG` (tree builder
     `ChargedSidisTreeBuilderHighQ2_v6.C` lines ~1746-1750, filled from
     `matched->GetMother1()` against the full `H1PartMCArrayPtr` MC array — the same
     matched-candidate block as the det.int. flags; the DIF-muon flag at line ~1778 is
     the standing mother-PDG-on-reco-leg precedent). NO tree re-production. Do NOT use
     `VetoAncestorPDG` for the keeper (it returns the first V0/γ/π0-class ancestor,
     not the mother).
  3. Matched-fill gate becomes `matched_gen_ok = gen_candidate_ok &
     ~(is_detint & ~is_kpar)` (prototype 854-859 with `not_explicit_allfake` reduced to
     the reroute condition): non-keeper det.int. legs leave the body; k_parented
     matched legs KEEP the central matched fill (signal, ruling 2). A builder
     following the pre-amendment `~is_detint` form would build S2-as-written and
     over-remove the 9.01% k_parented signal.
  4. Fake-column fill assigns `fake_cat = 3` to the rerouted non-keeper det.int. legs
     and accumulates per-reco-bin `fake_detint[ir]` / `fake_detint_w2[ir]` category
     vectors (prototype 1030-1042); the pre-existing gen-fail det.int. fake part
     (Sec. 2.4) keeps its own tag so the removable/keeper decompositions are exported
     exactly (G11/G13).
- **DO NOT PORT (each would silently change the selection or the observable):**
  - **[APPEND-ONLY SUPERSESSION NOTE, added 2026-10-11 — original entries below are
    retained verbatim and unmodified:]** the SECOND entry in this list ("the alpha cone
    (prototype `alpha_ok` / `ELECTRON_ALPHA_VETO_RAD`) — central is no-cone") is
    SUPERSEDED by USER DECISION 2026-10-11 ("The cone cut must be baseline. We know we
    have many fake tracks from the scattered electron."): the α ≥ 0.2 rad lab-frame
    scattered-electron cone is adopted as the BASELINE reco selection on top of the
    S2′ basis. Authoritative specification:
    `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/backgrounds/alpha_cone_baseline_spec_2026-10-11.md`
    (Sec 0 decision record; Secs 1-2 construction; Sec 5 gates; Sec 6 campaign;
    Sec 7 quoting rule). The OTHER TWO entries in this list (the e/V0 truth-veto
    disablement; the miss-row fill for reclassified det.int. rows) REMAIN IN FORCE
    unchanged.
  - the e/V0 truth-veto disablement (prototype 685-693) — central e/V0 exclusion stays;
  - the alpha cone (prototype `alpha_ok` / `ELECTRON_ALPHA_VETO_RAD`) — central is no-cone;
  - **the miss-row fill for reclassified det.int. rows (prototype 1043-1066)** — see 2.2;
    this is a spec-level correction to the prototype, not a port.

### 2.2 LOAD-BEARING DESIGN FINDING — the miss-row fill must be SUPPRESSED for S2

The all-fake prototype fills the miss row for every reclassified det.int. row whose own gen
candidate is valid (`fc==3 and i_gen_candidate>=0`, weight `w1·w2`, no NucliaWeight;
prototype 1043-1066), commented as keeping the truth marginal self-consistent. Under the
S2 observable that fill **cancels the removal**. Derivation, anchored on the verified
release-driver formulas (`run_bbb_ibu_release_coherentprior.py` lines 87-93:
`prior = coherent_prior()` = body+miss; `A = B/prior`; `eff = A.sum(axis=0)` =
`body_col/(body+miss)`; IBU unfolded with `P=prior`; BBB with `A_j=eff`):

- Final result per gen bin ≈ `u[j]/eff[j] = u[j]·(body+miss)[j]/body_col[j]`, where `u` is
  the data-scale estimate of reconstructed-as-signal truth content.
- With the fill: the det.int. matched content D moves body→miss, so `(body+miss)` is
  ~unchanged while `body_col` loses D; the enlarged `f_i` also removes D from the data, so
  `u` loses D. The two losses are proportional (`u ∝ body_col`) and cancel:
  `(u−D)·(body+miss)/(body−D) ≈ u·(body+miss)/body`. **The variant would be approximately
  INVARIANT vs central** — it would repartition the response while leaving the published
  numbers ~unchanged, and the det.int. truth content would survive through the efficiency
  denominator. (For e/V0 the prototype has NO miss fill, so those categories ARE removed —
  the prototype is internally inconsistent between categories on exactly this point. The
  ledger's "Option B removes det.int." statement and the audit's E.3 "the body+miss prior
  loses the det.int. content" hold only if the fill is suppressed; with the fill ported,
  the marginal does NOT lose the content — only the body does.)
- **S2 requirement:** the det.int. gen candidates must vanish from the truth marginal
  entirely — body loses them (reroute), miss does NOT gain them (suppress the fill). Then
  `prior_v = body_v + miss_v` is GTR-only and the removal survives the efficiency
  correction: final shift ≈ `−D_col/prior = −(raw body fraction) × eff` ≈ **−0.30% to
  −0.40%** integrated (Sec. 4 G10) — the efficiency recomputation SUPPRESSES the raw
  shift (×eff < 1), it does not amplify it.
- **Invariants that follow (all mandatory checks):**
  - the variant **miss row must be bit-identical to the central miss row** (expected diff
    0 rows). Mechanism: det.int. matched rows consume the SECONDARY's own inlined-gen key,
    which is never a ChargedTotGen key (CTG is GTR-only), so central and variant CTG
    consumption sets are identical and the CTG miss fill is unchanged. (Guard: the
    prototype's `reclassified_miss_keys` coincidence check stays as a defensive audit.)
  - `body_central − body_variant =` the reclassified det.int. matched legs, exactly, per
    (reco,gen) cell and per weight — auditable against the `fake_detint` category vectors.
  - `prior_gen_NWeff` (the NW-effective geometric-acceptance diagnostic fill) is unchanged
    (same consumed-CTG-keys argument).
  - per-reco-bin conservation: `B_ij + F_i` is invariant (`F` grows exactly as `B`
    shrinks), hence `f_i_v = f_i_c + D_i/(B+F)` with an UNCHANGED denominator (2.5).
- **Second-order residuals beyond the first-order cancellation (named for completeness;
  critic condition 2026-10-08):** (a) k=4 finite-iteration **prior-shape coupling** — IBU
  is initialized at P, so the variant changes the prior shape that regularizes sparse
  bins over the 4 iterations; (b) NW-weight mismatch between the body loss
  (`w1·w2·NucliaWeight`) and the ported-fill gain (`w1·w2`, prototype line 1061) — moot
  once the fill is suppressed, which is the S2 prescription; (c) migration smearing of
  the per-bin shift through the off-diagonal response. All three are empirically covered
  by G1/G3/G4/G5/G10 — no additional analytic correction is required.
- Status of this finding: **DERIVED IN THIS SPEC** from the verified driver formulas —
  flagged for critic review before production (risk R1, Sec. 7); critic review 2026-10-08:
  **R1 SIGNED OFF** (cancellation algebra verified exact at first order; the prescribed
  miss-fill suppression correct and sufficient; the `f_i` identity exact per reco bin by
  construction), with the six text conditions of that review applied in this revision.

### 2.3 Coherent prior, efficiency normalization, BBB acceptance

- `prior_v := body_v + miss_v` computed FROM THE VARIANT RESPONSE object (body legs
  `w_reco = w1·w2·NucliaWeight`; miss legs `w_gen = w1·w2`). Never rebuild priors the
  plain `w_gen`-only way (superseded 2026-10-07); never reuse the central marginal (it
  still contains D in the body).
- Efficiency: `eff_v[j] = body_v[j]/(body_v[j]+miss_v[j])`; BBB acceptance is the same
  `body/(body+miss)` per gen bin. Both LOSE the det.int. content from the numerator and
  (with the fill suppressed) from the denominator — the variant acceptance is smaller than
  central in det.int.-populated bins (D was reconstructed content, i.e. high-acceptance).
- The release driver `run_bbb_ibu_release_coherentprior.py` is unchanged in LOGIC but
  NOT safely reusable as-is: its `coherent_prior()` reads a HARDCODED central rebuild
  path (driver line 16; the response ROOT and data-vector constants are also hardcoded,
  lines 30-32), so an unmodified driver run against the variant response would consume
  the CENTRAL body+miss prior — silently reproducing the R1 cancellation at release
  level (risk R10). The variant must run as a dated driver copy with repointed
  constants and a prior-provenance assertion in the release metadata (Sec. 5 item 3);
  the variant response file must carry the same block structures (`B`, miss row,
  `prior_gen_NWeff`, binmap). IBU stays `k=4`; `L_data = 351000.0 nb^-1`; shape
  (491,246) per charge; data vector 980.
- NucliaWeight: application unchanged (`w1·w2·NW` on matched body legs; carried into the
  fake column by the same `w_reco` on rerouted legs). Under the variant, inelastically-
  interacted primaries are uniformly "not reconstructed" (primary in miss; secondary a
  fake), which matches the NucliaWeight absorption model MORE closely than central (where
  a reconstructable secondary counted as signal while NW still compensated absorption on
  other legs). No re-derivation is proposed; residual model dependence is covered by the
  DJANGOH14 closure gate (G6) and rides the existing `model` budget component.

### 2.4 The 13.82% gen-Q2/y-fail part — NO separate handling

The 13.82% fake-column part of the det.int. class (matched rows whose secondary's gen
EVENT fails the gen fiducial — ~92% of fake reasons — or whose own kinematics fall outside
the gen grid/pool) is ALREADY in the central fake column: those rows fail
`gen_candidate_ok` and never entered `matched_gen_ok` in the first place. Under the
variant they remain fakes via the same fill, now tagged `fc=3`. They are disjoint row sets
from the reclassified 86.18% by construction, and they are removed exactly once by `f_i`
under BOTH conventions. **No separate handling is required or permitted.** The `fc=3`
category vector from the variant build provides the exact central-selection 86/14
decomposition per reco bin as a side product (closing the ledger's alpha0p2-conditional
partition transfer caveat at the class level).

### 2.5 f_i algebra and predicted magnitude

Per reco bin: `f_i_v = (F_c + D_i)/(B_c + F_c) = f_i_c + D_i/(B_c + F_c)` — the denominator
is invariant (body loss = fake gain); under the adopted S2′ convention `D_i` is the
MATCHED NON-KEEPER det.int. content only (k_parented keeper legs stay in `B`).
Recomputed 2026-10-09 for the refined removable fraction **0.909920** of the block
(critic-verified, first-order ESTIMATE, alpha0p2-cone-conditional base; per-charge split
inferred from the per-generator template splits: RAPGAP31 det.int. template h+ `14201.49`
/ h− `12177.86`; DJANGOH14 h+ `15263.62` / h− `13168.63`; combined template `27405.80` ⇒
h+ share ≈ 53.7%):
- **What moves (the shift-chain input):** matched non-keeper ≈ **0.553%** of the data
  vector (0.9099 × the whole-block matched 0.61%); the already-removed fake part ≈
  **0.089%** does not shift; the 9.01% keeper (≈0.064% of the data vector) stays
  signal. The ruling-5 "0.642%" (0.9099 × 0.7056%) is WHOLE-CLASS DEFINITION-LEVEL and
  must NOT enter the shift chain — the chain uses the matched **0.553%**.
- matched-template / data-vector scale: `f_i` growth ≈ h+ **+0.58pp** / h−
  **+0.52pp** → predicted integrated `f_i` **≈11.30% (h+) / ≈10.92% (h−)** vs today's
  10.7141%/10.4019%. [S2-as-written whole-block envelope, superseded-for-S2′:
  +0.642pp/+0.572pp → 11.36%/10.97%.]
- response-internal scale (critic condition 2026-10-08 — the actual response-side
  growth is `D/(B+F)` in RAPGAP31-response normalization; conversion factors:
  data/MC yield ratio ~1.13, RG-only-vs-combined template ~0.965): ≈ h+ **+0.63pp** /
  h− **+0.56pp** → predicted integrated `f_i` ≈ **11.34% (h+) / 10.97% (h−)**
  response-internal. [S2-as-written envelope, superseded-for-S2′: +0.69pp/+0.62pp →
  11.40%/11.02%.] Both scales are kept, labeled; G9's acceptance band is defined in
  response-internal terms.
- Transfer assumptions: the pooled 0.909920 is applied uniformly over the matched/fake
  partition and over charge (per-partition/per-charge removable fractions not
  separately delivered; the keeper is charge-symmetric — K− 51,525 / K+ 50,881
  mothers); the variant's category-vector exports (G11) and gate G13 close both
  assumptions at build time.
Precedent uncertainty band: the all-fake chain's non-binding G9 diagnostic measured
fake-column det.int. category sums at 0.984-0.987 of the templates per generator/charge
(selection-object differences; shape correlations 0.99999). NucliaWeight on rerouted π/K
legs modulates at the few-percent level. Exact numbers are build outputs.

### 2.6 S3-conditional construction

If S3 is later adopted, the SAME construction applies with `is_detint` replaced by the
interaction-born sub-label (decay feed-down rows return to the matched body). That label
does not exist today (Sec. 6); the response reroute is then identical in form, and the
S2 variant machinery (category vectors, gates, campaign pattern) carries over unchanged.
S2 built first is therefore the natural envelope-bound product: the S3 answer lies between
S1 and S2 per bin.

---

## 3. Updated exactly-once ledger under the S2 variant

Fractions vs the central data vector (Σ `3884097`; h+ `1975301` / h− `1908796`); template
magnitudes alpha0p2-cone-conditional where flagged in the ledger.

| Category | Central chain today | Under S2 detint-fake variant | External vector |
|---|---|---|---|
| MC-unflagged reco fakes | removed by `f_i` (10.7141% h+ / 10.4019% h−) — exactly once | unchanged; same `f_i` term, disjoint row set (`fake_cat==0`) | none (in-response) |
| Residual electrons | nothing removes them (mask no-op in data; excluded from response) — under-subtracted | unchanged; **external per-bin additive vector pre-unfold (Option A)** | **YES, unchanged** (≈3.8355% cone-applied / ≈4.1441% no-cone PROV) |
| V0 daughters | nothing removes them — under-subtracted | unchanged; external per-bin additive vector pre-unfold (Option A) | **YES, unchanged** (3.4002% cone-applied) |
| det.int. GEANT charged-hadron secondaries | 86.18% in-observable as signal + 13.82% via `f_i` | **≈ 90.99% of the class inside `f_i`** under S2′ — the removable part: 78.42% matched non-keeper rerouted + 12.57% pre-existing fake non-keeper (disjoint row sets; denominator `B+F` invariant); plus the pre-existing fake KEEPER part ≈1.24% also inside `f_i`, exactly like the gen-fail part of ordinary signal (total inside `f_i` ≈92.2%); the **keeper ≈9.01% of the class stays as SIGNAL** (≈7.76% matched in the body, ≈1.24% its gen-fail part in `f_i`). Denominator convention: % of the det.int. CLASS (27,405.80 tracks alpha0p2 / 1,136,839 campaign rows); removable fraction 0.909920 applied uniformly over matched/fake (planning assumption, closed by the build's category exports / G13) | **FORBIDDEN — stays forbidden, more strongly**: any external det.int. vector would now double-remove the removable class AND cannot respect the keeper (would subtract kept signal) |
| low-Q2 leakage | nothing (source-distinct) | unchanged; external per-bin additive vector pre-unfold (0.0054%; 208.3795 tracks; alpha0p1-cone selection caveat) | **YES, unchanged** (only option under every convention) |
| DIFFVM ρ | nothing (source-distinct) | unchanged; **systematic-only** (0.0074% integrated; per-bin subtraction forbidden, ruling 2026-10-07) | NO — never a subtraction vector |

- **e∩V0 overlap (112.2093 scaled):** IGNORED per user decision 2026-10-08 (max 0.0364%
  double-subtraction, accepted) — unchanged under the variant; the det.int. reroute does
  not touch the e/V0 classes (the classifier carries the e/V0 vetoes, so det.int.-tagged
  rows are e/V0-vetoed by construction and the classes are disjoint).
- **No double-removal introduced:** the reroute moves rows between response blocks
  (body→fake column) with per-reco-bin `B+F` conserved; `f_i` is computed once from the
  variant response; the external e/V0/low-Q2 vectors subtract data-side contamination in
  classes the response never contains; DIFFVM is never subtracted. The S2 chain's
  failure mode remains the ledger's under-subtraction items (e, V0, low-Q2 until the
  Option-A vectors are applied; DIFFVM systematic envelope), NOT double-subtraction.
- **Full future chain (Option A + S2):** `d'_i = d_i − B_i(e) − B_i(V0) − B_i(lowQ2)`
  pre-unfold with `Var(d') = Var(d) + Var(B)` (templates carry `var_980`), clamp ≥0, then
  unfold through the detint-fake response with variant `f_i` — every track-level class
  then handled exactly once: e/V0/low-Q2 external, unflagged fakes + det.int. inside
  `f_i`, DIFFVM systematic-only.

---

## 4. Validation gates — ALL mandatory before any promotion

Precedents in brackets. The variant chain must pass every gate; any failure blocks
promotion (and per workspace policy, unfolding is already the required method — the
variant changes the response, not the method).

- **G1 Truth conservation, 245/245 per charge:** the variant's own truth marginal
  `body_v+miss_v` must satisfy the coherent-prior conservation gate on the variant
  response [coherent-prior release: exact 245/245 both charges], PLUS the two
  variant-specific marginal audits: (a) miss row bit-identical to central (expected 0
  differing bins; Sec. 2.2 mechanism), (b) `body_c − body_v` equals the reclassified
  det.int. legs exactly (cross-checked against the `fake_detint` category vectors).
- **G2 Supported bins preserved:** `(prior>0)&(eff>0)` on 245/245 per charge; any bin
  losing support (a gen bin whose body was purely det.int.) must be enumerated and
  justified — none is expected (det.int.-populated bins are strongly GTR-populated).
- **G3 IBU cross-check:** independent-implementation agreement at the coherent-prior
  release precedent level (max rel dev 3.31e-15 / 3.66e-15).
- **G4 IBU-vs-BBB consistency:** both methods on the variant response; differences
  consistent with the central precedent (BBB h+ 11.496351 / h− 11.202754 nb scale).
  NOTE (critic condition 2026-10-08): on the DJANGOH (model) leg this comparison is
  affected by that leg's internal prior inconsistency — IBU uses the STORED plain prior
  `ld["prior_gen"]` while BBB uses the response-internal `A_j` (driver lines 88-90) — so
  the G4 precedent comparison is clean on the RAPGAP leg only (see G6).
- **G5 Poisson-fluctuated closure:** 1000-toy stat propagation per charge through the
  variant `f_i` and response [stat_toys_coherentprior_2026-10-07, 1000/charge; per-toy
  machinery precedent poisson_toys_allfake_alpha0p2_20260629 — "fake column handled
  exactly once" run contract]; toy closure within ToysSE.
- **G6 DJANGOH14 cross-generator closure:** variant response built on DJANGOH14 (502
  sidistree files) and the closure re-derived [all-fake cluster 3987932 built both
  generators; P1 replica floors precedent cluster 5380196, 100/100]. **Model-leg
  attribution caveat (critic condition 2026-10-08):** the driver's `djangoh` leg uses
  the STORED PLAIN PRIOR (CTG-based, GTR-only; driver line 88), which the reroute does
  not touch — so in the diagonal limit the DJANGOH leg is EXACTLY INVARIANT under the
  variant, and the release-level |DJ−RG| difference would be MISATTRIBUTED as generator
  dependence of the removed block and absorbed into the `model` budget. Therefore:
  quantify the generator dependence of the removed block from the exported
  per-generator `fake_detint` vectors (G11 side-products) instead, or make the
  model-leg prior coherent before using this closure for that purpose. Also note the
  djangoh leg is internally inconsistent between IBU (stored P) and BBB
  (response-internal A_j), affecting the G4 precedent comparison.
- **G7 Refold chi2:** fold the variant-unfolded result through the variant response + fake
  column and compare to the externally subtracted data; chi2/ndf consistent with the
  central-chain precedent.
- **G8 Negative-bin / acceptance audit:** 0/980 negative reco inputs after external
  subtraction + `f_i`; 0/490 negative cross sections; acceptance `eff_v ∈ (0,1]` in all
  populated bins; 0 background-exceeds-data bins [f_i audit precedent gates].
- **G9 f_i magnitude audit:** verify the identity `f_i_v = f_i_c + D_i/(B_c+F_c)` per bin
  (invariant denominator; under S2′ `D_i` = matched NON-KEEPER content only); the
  acceptance band is defined in RESPONSE-INTERNAL terms (critic condition 2026-10-08;
  conversion in Sec. 2.5), RECOMPUTED 2026-10-09 for the adopted S2′ (removable
  fraction 0.909920): integrated growth ≈ **+0.63pp (h+) / +0.56pp (h−)**
  response-internal — predicted integrated `f_i` ≈ **11.34% (h+) / 10.97% (h−)**
  response-internal vs today's 10.7141%/10.4019% — within the G9-precedent uncertainty
  band (0.984-0.987 fake/template); the matched-template-scale figures (+0.58pp/+0.52pp;
  11.30%/10.92%, Sec. 2.5) are retained for the data-vector-side comparison only;
  [S2-as-written whole-block envelope, superseded-for-S2′: response-internal
  +0.69pp/+0.62pp → 11.40%/11.02%; template +0.642pp/+0.572pp → 11.36%/10.97%]; per-bin
  max grows from the central 32.24% by at most the local matched non-keeper det.int.
  fraction; export per-bin `fake_detint` vectors.
- **G10 Predicted shifts (must be confirmed, not assumed):**
  - **Direction: DOWN in every populated bin** (removal).
  - **Integrated (recomputed 2026-10-09 for the adopted S2′):** the class total is
    `0.7056%` of the data vector (`27405.80` tracks, alpha0p2 template); the refined
    removable is 0.909920 of it (whole-class definition level ≈ 0.642% — NOT the
    shift-chain input), of which the MATCHED non-keeper part that actually moves is ≈
    **0.553%** of the data vector (≈0.58pp h+ / 0.52pp h−, matched-template scale),
    the already-removed fake part ≈ **0.089%** does not shift, and the 9.01% keeper
    (≈0.064%) stays signal. Raw response-internal removal ≈ **−0.59% (h+) / −0.56%
    (h−)** (0.9099 × the whole-block −0.65%/−0.62%). At the final-cross-section level
    the shift is `−D_col/prior = −(raw body fraction) × eff` — the efficiency
    recomputation SUPPRESSES the raw shift (×eff < 1, eff ≈ 0.5), it does not amplify
    it: integrated final-level S2′ shift ≈ **−0.27% to −0.36%** (= 0.9099 × the
    whole-block band [−0.30, −0.40%]; ≈ −0.553% × eff), concentrated in the z<0.05
    rows. History: the earlier **−0.6% to −0.8%** band in the pre-review draft of this
    spec was d_sig-level (raw), not final-level, and is superseded in this text; the
    critic-corrected whole-block band −0.30% to −0.40% is superseded-for-S2′ (retained
    as the S2-as-written envelope). All magnitudes remain cone-conditional planning
    inputs (cone conditioning now BOUNDED: parentage cone survival ≈0.9984 — R2
    closed at class level).
  - **Worst bin:** h+ b420 (S3x0, z∈[0,0.05), pT∈[0,0.25)): alpha0p2 class fraction
    **2.20%** (template 339.30 / data 15443); matched non-keeper removable ≈ **1.73%**
    (from 1.90% whole-block); expected final-level shift ≈ **O(−1%)** — conclusion
    unchanged. Next: b350 (S2x1, z<0.05, pT<0.25) at 2.07% class fraction. Reference
    per-bin map (central selection, delivered 2026-10-09): worst removable-keepK bin is
    481 (h+, S3_x0, z∈[0.70,1.00], pT∈[0.25,0.50)) at 5.2685% — SPARSE (1 data track;
    shape statement, not a rate; per-generator DJ 6.694% / RG 3.843%); per-cell
    removable averages (kpar kept): S3_x0 1.027% highest, S0_x0 0.611% lowest.
  - **Born level:** `C_rad` is gen-level GTR-only and unchanged, so the Born-level
    relative shift equals the raw-release-level (unfolded) shift per bin
    (`sigma_Born = C_rad × Unfold`).
  - Charge-ratio consequence: h+ vs h− removals differ (≈0.58pp vs ≈0.52pp,
    matched-template scale; response-internal ≈ +0.63pp vs +0.56pp, Sec. 2.5), so any
    charge-asymmetry-style derived observable shifts by O(0.1%) relative.
- **G11 Composition side-products:** the variant build exports the central-selection
  det.int. content per charge (closing the ledger PROVISIONAL "no-cone det.int. magnitude
  unbounded" item at the class level) and the exact central-selection 86/14 partition
  (closing the alpha0p2 transfer caveat for the split).
- **G12 Response weight audit + fake-column MC-stat note (critic condition 2026-10-08):**
  (a) repeat the central response weight audit (central precedent: 0 bins above 5× the
  mean bin weight) on the variant response — the reclassification must not create
  weight outliers; (b) record that the fake-column MC-statistical uncertainty
  (pre-existing unpropagated `sumw2` gap — STATE.md covariance gaps) GROWS with D and
  must enter the systematics re-base if the variant is promoted.
- **G13 Per-bin removable reference check (NEW 2026-10-09; the variant's realized removal
  vs the parentage per-bin map):** reference =
  `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/perbin_contamination_motherspecies_2026-10-09.csv`
  (980 rows charge-major; reference column `frac_avg_removable_keepK`; per-generator
  `yield_b_rg/dj_removable_keepK`; `data_count`). BASIS CAVEAT (decisive for a
  meaningful comparison): the map is ALL-ROWS central-selection (matched+fake, w_b,
  data-vector scale), while the variant's realized removal is MATCHED-ONLY
  response-internal — a naive unity comparison fails by ~14% everywhere (the matched
  share). Compare the realized per-bin rerouted weight to the map × matched-share
  (~0.862 central pooled; the variant's own category exports refine it per bin), as
  per-bin ratios or differences with a tolerance of a few × the same-MC
  file-set/weight-convention uncertainty (generator mix RG-vs-average ±0.4% relative
  block-level, larger per-bin shape spread; w_a-vs-w_b ≲0.05% on fractions;
  per-generator template shape p05 0.933 / p95 1.006). EXCLUDE the 14 data-zero bins
  (reference fraction None) and the 1-4-data-track sparse bins from fractional gates
  (report-only). **DECISIVE integrated check:** the realized integrated removable
  fraction (rerouted matched non-keeper + pre-existing fake non-keeper, over all
  det.int. content) must land at ≈ **0.9099** (per-generator reference: RAPGAP31
  0.906412 / DJANGOH14 0.912611), NOT at 1.000 — this simultaneously catches a skipped
  keeper (→ 1.000), an over-broad keeper (→ low), and
  GenMotherPDG-vs-campaign-mother semantic drift (→ off-reference). Failure blocks
  promotion.

---

## 5. Production plan sketch (S2)

**RUNLIST rules honored:** extend canonical scripts, never fork blindly; `L_data =
351000.0 nb^-1`; response shape (491,246) per charge; data vector 980; IBU `k=4`; 490 rows
finite, no negatives; never overwrite central products; single-file smoke first (login
node, ≤5 min, one input file), then one data + one MC smoke, then Condor for anything
multi-file.

**Scripts to extend (canonical paths):**

1. **Response shard builder:** extend the v4 finalcandidate builder with the Sec. 2.1
   reroute as a dated study variant, following the coherent-prior rebuild campaign
   pattern — `output_v6/studies/coherent_prior_response_rebuild_z005_20261006/scripts/
   build_coherent_prior_response_shard.py` (a fork of the v4 finalcandidate builder, per
   its own header) + `merge_coherent_prior_response.py` + `validate_rebuild.py` + the
   `condor/rapgap31_rebuild.sub` / `run_shard.sh` / shard-list pattern. The all-fake
   script (canonical snapshot, md5 `f4d9ea1b7121b3dc723fec39d988f2e0`) is the reroute
   reference ONLY (port list Sec. 2.1; DO-NOT-PORT list Sec. 2.1).
2. **Data vector builder: UNCHANGED** (central data vector; the Option-A external e/V0/
   low-Q2 subtraction is applied to it pre-unfold by the release-side step, `lowq2sub`
   precedent from the 2026-10-05 all-fake chain).
3. **Release driver: logic unchanged, executed as a DATED VARIANT COPY (required —
   critic condition 2026-10-08):** `run_bbb_ibu_release_coherentprior.py` consumes
   body+miss via `coherent_prior()`, which reads a HARDCODED central rebuild path
   (driver line 16; ROOT/data-vector constants hardcoded, lines 30-32). The variant
   campaign must produce a dated driver copy with ALL constants repointed (variant
   response/prior source, response ROOT, data vector) plus a prior-provenance assertion
   in the release metadata (record and assert that the consumed prior source IS the
   variant response's body+miss, e.g. an extended `prior_basis`/source-path field).
   Repointing the response without repointing the prior source silently reproduces the
   R1 cancellation (risk R10). Output to a NEW dated candidate directory (never
   `central/`).
4. **Validation/toys:** `crosscheck_unfold_methods_v4.py` (IBU cross-check), the 1000-toy
   stat machinery, the f_i audit pattern (`data/audits/unfolding_bgsub_audit_2026-09-25/`),
   the all-fake chain's `s0_input_verification.json` pattern (assert the reco selection is
   bit-identical to central except the det.int. reroute — guards risk R8).

**Condor scale estimate (from prior response builds):**

- RAPGAP31 variant response: **≈73 shard jobs** at ~5 input files/shard over the 364
  central RAPGAP31 sidistree files — exactly the coherent-prior rebuild scale (cluster
  **5342813, 73/73** shards, same inputs).
- DJANGOH14 cross-generator variant: ≈100-101 shards over 502 sidistree files (the
  all-fake build cluster 3987932 covered both generators; same shard size).
- Plus: 2 merges, validation pass, release regeneration, 1000×2 stat toys, ~100-replica
  model floors if promoted. Total campaign O(300) CPU-jobs; **no tree re-production**
  (all classifier branches exist in v6 trees — ESTABLISHED).
- Single-file smoke (login node, ≤5 min, one MC sidistree file, e.g. one DJANGOH14 7091
  file): check (a) reroute active (`fake_detint > 0`; body differs from central),
  (b) **miss row identical to central**, (c) shape (491,246), (d) no e/V0/cone behavior
  change, (e) `prior_gen_NWeff` unchanged. Then one data + one MC smoke, then Condor.
- Output placement: dated study root (e.g.
  `output_v6/studies/detint_fake_response_z005_<date>/`); if promoted, a dated
  `data_release_candidates/`-style candidate installed by checksum-verified copy; if it
  reaches plots, the `cRad_bgfracsub` derived-variant packaging precedent (separate
  derived product, never merged into the nominal).

---

## 6. Decay-feed-down fork (S2 vs S3) — RESOLVED 2026-10-09; GEN-SIDE OPEN ITEM

**RESOLVED (USER-DECIDED 2026-10-09, Sec. 0):** the fork is closed. The full-sample
det.int. parentage campaign (Condor cluster 5433299, 865 units, all five validation
gates PASS) plus the mother-species re-slice (cluster 5435475) determined the
decomposition from n=1,136,839 selected rows: interaction-born (C4+C5) **34.47%**,
decay-born non-π/K/p-mother (C1+C2; 100% hyperon mothers) **56.53%**, k_parented (C3;
100% charged-K mothers) **9.01%**; removable fraction **0.909920 ± 0.000269**
(DJANGOH14 0.912611 / RAPGAP31 0.906412). The adopted S2′ variant removes the first two
and keeps the third; it is defined by measured mother lineage, so the S3-style
vertex/SVX production is NOT required for it. The pre-decision text below is retained
as the historical record, with its now-answered questions marked.

**GEN-SIDE OPEN ITEM (Sec. 0 ruling 6 — do NOT implement in this variant; separate
future decision):** truth-level charged-hyperon feed-down exclusion from the GEN pool
(the response truth marginal), scoped to DISPLACED WEAK DECAYS (hyperons, K0S).
Gen-side feasibility is PROVEN (2026-10-09 study verdict): one light
`ChargedTotGen`-only pass over the existing 865 v6 tree units (~20-40 min single
Condor unit), reading branches already in the trees
(`GenMotherPDG`/`ParticleSource`/`VetoAncestorPDG`). Measured context: the GTR truth
pool's hyperon-mother content is ~0.5% (DJANGOH14 0.48% / RAPGAP31 0.46%),
exclusively prompt Σ*/Ξ*→π strong decays; Λ(3122)- and Ξ(3312/3322)-mother rows are
exactly 0 in both generators (generator-stable — they decay only in GEANT, and their
reco-side daughters ARE the det.int. block S2′ removes). Definitional caveat: at
truth level a literal non-π/K/p-mother reading would remove ~94% of the pool
(ρ/ω/K*/D0 → π/K fragmentation decays) — the truth-side extension must scope
"feed-down" to displaced weak decays. This open item does not gate S2′.

**What S2 removes beyond the strict user Class 2 (single-file PROVISIONAL, n=122,
DJANGOH14 7091):** ~62.3% hyperon-decay mothers (Σ± 41, Σ∓ 21, Ξ 14 of 122), ~37.7%
meson/baryon-mother (π 24, K 10, n 6, p 6) of which ~8% are K-DIF candidates — i.e. the
removable block is roughly two-thirds physics feed-down, one-third interaction-born.
O(±9%) per-bin statistical uncertainty on the composition; generator- and period-
dependence unexplored. The strict material-interaction-only fraction is NOT-DETERMINABLE
today. [Superseded 2026-10-09: DETERMINED by the full campaign — 34.47% interaction-
born, 56.53% hyperon-mother decay-born, 9.01% k_parented, removable fraction 0.909920
(RESOLVED block above); generator dependence measured (f_int DJANGOH14 0.3536 /
RAPGAP31 0.3331).]

**The MODS-level det.int. parentage diagnostic (the S3-enabling first step):**

- Requirements: full-sample, weighted, per generator/period; for every det.int.-tagged
  selected track, record the matched STR record's mother/ancestor PDG chain (the
  `Mo1` nearest-ancestor walk already used for `GenMotherPDG`/`VetoAncestorPDG` labels),
  StrIdx, own kinematics, and — for the clean split — the STR production vertex;
  decompose the 86.18% matched block per gen bin. Schema precedent:
  `docs/diagnostics/electron_parentage_hATmods_cleanrun_2026-07-31.md` (mother/ancestor/
  StrIdx fields; pattern directly reusable for π/K/p).
- NAF feasibility: YES for the mother/ancestor decomposition — the electron-parentage
  cleanrun is the precedent (Condor campaign over the MC HAT/MODS production inputs; no
  central product touched; login-node single-file smoke first per workspace policy).
- The clean interaction-vs-decay split needs the SVX production vertex
  (`H1SvxRow::GetX/GetY/GetZ/GetIvtype/GetTof`, ODS bank level; absent from `H1PartMC`
  and from the MODS-level production inputs — the electron-parentage run recorded
  SVX/STR fields as −999). REFINED FEASIBILITY (supersedes the "ODS/DST pilot only"
  picture): the 2026-09-09 material-vertex study
  (`docs/research/research-material-vertex-dstfree-2026-09-09.md`) source-demonstrated the
  `H1PartMC → H1StrBankPtr[StrIdx] → GetSvx() → GetX/Y/Z` chain on the v6 Stage-1 MC
  HAT/MODS event-loop path (Stage-1 lines 147-156, 1358-1363, 1773-1783) — i.e. the
  vertex is reachable from the MODS-tier event loop WITHOUT a separate ODS/DST
  production, with one hard caveat: the Stage-1 standalone gen-loop `StrBank` access
  FAILED on batch nodes (lazy ODS/FPACK loading, lines 3006-3011), so any SVX-reading
  diagnostic or builder extension must pass a CONDOR batch-node smoke before scale-up
  (login-node success is insufficient evidence). A small MC-only Stage-2 extension
  (`StrProdR`, `StrProdZ`, `StrProdValid` branches, filled only for `matched->IsStr()`
  with valid `GetStrIdx()/GetSvx()`) is sketched in that study — status PROPOSED, NOT
  VALIDATED; it would require MC tree re-production if adopted into the trees.
- Mother-PDG-only split (no vertex) is PROBABILISTIC and is NOT acceptable as a physics
  observable definition; it can bound the composition but not define the class.

**Does S3 change any number available today? NO — explicitly.** No existing full-sample
artifact separates interaction-born from decay-born within the det.int. class; the
single-file proxy cannot be scaled to per-bin vectors; every S3 number (fraction, per-bin
content, shifted cross section) requires the new production above. Until that production
exists, S3 is a definition without a measurement, and the only measurable envelopes are
S1 (nothing removed) and S2 (whole label removed). [Superseded 2026-10-09: the
production above WAS delivered (clusters 5433299 + 5435475, with per-bin maps — the
G13 reference), so the interaction/decay split is now measured; and the adopted S2′
supersedes the S1/S2/S3 envelope framing per Sec. 0.]

---

## 7. Risk register

**Carried UNVERIFIED items U1-U4 (from the 2026-10-08 audit, with the check that closes
each):**

- **U1 — DTRY best-track rule.** `dtry[idtra]->GetInt(0)` = "best" STR track; the precise
  rule (majority of hits?) is a DST-simulation convention not verifiable from H1OO source.
  If "best" is not majority-hits, the kink-hybrid caveat changes SIZE, not direction.
  **Close with:** H1SIM/DST documentation, or an empirical DTRY hit-fraction study on ODS
  files.
- **U2 — GEANT3 process-to-track mapping** (elastic/ionization keep the same track;
  inelastic creates new tracks; a "continuing leading particle" after inelastic is a NEW
  track). Standard GEANT3/GHEISHA behavior, but the Class-1/Class-2 boundary rests on it.
  **Close with:** GEANT3 manual, or an STR parent-chain walk study on ODS files.
- **U3 — STR 4-vector = production-point kinematics** (vs first-hit/end-of-track).
  Supported by the empirical `(GenP−P)/P` median −0.009 and the structured opening
  angles; the matched-gen bin assignment of the 86.18% block depends on it.
  **Close with:** an STR/SVX comparison study at ODS level (or via the event-loop SVX
  dereference, Sec. 6).
- **U4 — `H1PartCand::GetTrack()` FPACK abort on MC** (TrackLength −999 sentinel), taken
  from the builder's own comment. **Close with:** a live MC file test.

**New risks identified in this spec:**

- **R1 — Miss-row-fill cancellation (DESIGN, HIGH if missed, resolved in Sec. 2.2):**
  porting the prototype's miss-row fill (lines 1043-1066) makes the variant approximately
  invariant — a "detint-fake" product that silently keeps the det.int. content in the
  published numbers via the efficiency denominator. Mitigation: suppress the fill;
  mandatory gates G1(a)/(b) (miss bit-identical; body diff = reclassified legs). This
  finding is DERIVED here — critic review 2026-10-08 **SIGNED IT OFF** (algebra verified
  exact at first order; suppression prescription correct and sufficient), with its text
  conditions applied in this revision.
- **R2 — Cone-transfer / planning-number provenance (MEDIUM):** the 0.7056%/2.20%/86-14
  numbers are alpha0p2-cone-conditional; no cone-survival counter exists for det.int., so
  central no-cone magnitudes are unbounded from above by existing artifacts and predicted
  shifts carry that caveat until the variant build measures them (G11 closes it).
- **R3 — Composition and generator dependence of the removed block (MEDIUM, physics):**
  S2 removes ~two-thirds decay feed-down (single-file PROVISIONAL) and embeds the
  GEANT-vs-generator decay-boundary artifact into the observable; the DJANGOH-vs-RAPGAP
  variant difference may grow vs central (gate G6; rides `model`).
- **R4 — Supported-bin loss (LOW):** a gen bin whose body was purely det.int. would lose
  support. Gate G2 (expect none; enumerate any).
- **R5 — f_i growth in sparse corners (LOW-MEDIUM):** `f_i` grows by the local det.int.
  fraction with an invariant denominator; worst bins are z<0.05/low-pT where central
  per-bin `f_i` already reaches 32.24%. Gates G8/G9 (0 negative/exceeds bins; identity
  audit).
- **R6 — Batch-node STR/SVX lazy-loading failure (S3-only, MEDIUM):** the Stage-1
  standalone gen-loop `StrBank` access failed on batch nodes; any SVX-reading diagnostic
  or builder extension must pass a Condor batch-node smoke (Sec. 6) — login-node success
  is insufficient.
- **R7 — NucliaWeight bookkeeping under the variant (LOW, monitor):** the variant makes
  the interacted-primary treatment MORE consistent with the NucliaWeight absorption model
  (Sec. 2.3), but no re-derivation is done; residual model dependence rides the existing
  `model` component and gate G6.
- **R8 — Accidental selection drift when porting (LOW, operational):** silently porting
  the prototype's e/V0 veto disablement or alpha cone would change the selection object.
  Mitigation: the S0 input-verification pattern (assert reco selection bit-identical to
  central except the reroute) + single-file smoke checks (a)-(e) in Sec. 5.
- **R9 — Promotion hygiene (operational):** the variant must never overwrite or silently
  replace central products; it is a dated candidate/derived product until the user
  promotes it (checksum-verified-copy install precedent; `cRad_bgfracsub` separation
  precedent). Quoting variant values without stating prior basis AND Born-vs-raw level
  remains FORBIDDEN.
- **R10 — Driver hardcoded prior-source trap (operational, HIGH if missed; critic
  condition 2026-10-08):** the release driver's `coherent_prior()` reads a hardcoded
  CENTRAL rebuild path (driver line 16; ROOT/data-vector constants hardcoded, lines
  30-32). Running the unmodified driver against the variant response would consume the
  central body+miss prior — which still contains D — and silently reproduce the R1
  cancellation at release level. Mitigation: dated variant driver copy with repointed
  constants plus a prior-provenance assertion in the release metadata (Sec. 5 item 3);
  the G1 marginal audits catch it only if run against the response the release actually
  consumed, so the metadata assertion is mandatory, not optional.
- **R11 — Keeper-condition fidelity (NEW 2026-10-09; HIGH if missed, mitigations
  specified):** a wrong keeper would keep or remove the wrong class while every
  S2-inherited gate still passes. Failure modes: the construction-vs-convention
  divergence (Sec. 2.1 previously said `~is_detint` with no keeper — RESOLVED by the
  2026-10-09 Sec. 2.1 amendment; a builder following the pre-amendment form builds
  S2-as-written and over-removes the 9.01% k_parented signal); a deep-ancestor or
  resonance-PDG match instead of direct mother ±321 (over-keeps; the kparented
  composition shows K*/D0/φ only in anc1, never as mother); `VetoAncestorPDG` misuse
  (returns the first V0/γ/π0-class ancestor, not the mother); unpopulated
  `GenMotherPDG` on det.int. legs. Mitigations: mother-level ±321-only condition;
  single-file smoke verification of keeper population and the removable/keeper split;
  and gate G13 — whose decisive integrated check (realized removable fraction ≈
  0.9099, not 1.000) catches a skipped keeper, an over-broad keeper, and
  GenMotherPDG-vs-campaign-mother semantic drift simultaneously.

---

## 8. ESTABLISHED / PROVISIONAL / UNVERIFIED register (this spec)

**ESTABLISHED (artifact- or source-backed, consumed here):** all ledger ESTABLISHED items
(MC-only flags ≡0 in data; central response excludes flagged e/V0; `f_i` removes
10.7141%/10.4019% = the unflagged fake class only; det.int. in-response 86.18/13.82 split
structural, magnitudes cone-conditional; external det.int. subtraction FORBIDDEN under
the central response; DIFFVM systematic-only; L_data=351000.0); all audit ESTABLISHED
items (Class 1 stays GTR-linked signal end-to-end; the det.int. classifier IS the Class-2
label, Tier-0 own-record matched at 100%; central matched fill has no IsGtr requirement;
`ChargedTotGen` is GTR-only; `ParticleSource` unusable; data-side separation impossible);
the release-driver normalization formulas (`prior=body+miss`, `A=B/prior`,
`eff=body/(body+miss)` for both IBU efficiency and BBB acceptance — verified at driver
lines 87-93); the all-fake reroute line anchors and canonical-snapshot md5; the
coherent-prior rebuild campaign precedent (cluster 5342813, 73/73, v4-builder shard
script); the per-generator per-charge det.int. template sums and fake-column ratios
(0.984-0.987, G9 non-binding); the prior-H1 convention documentation (secondaries
in-observable with a 1% tracking systematic; vertex-fitted; K0s 0.2%) and the workspace
gen-loop policy (feed-down included as signal).

**PROVISIONAL (planning inputs, not release-grade; recomputed 2026-10-09 for the
adopted S2′):** the per-charge det.int. split of the combined template (inferred from
per-generator splits); the matched/fake and per-charge UNIFORMITY assumptions on the
removable fraction (closed at build time by the category exports / G13); the predicted
`f_i` growth (response-internal ≈ +0.63pp/+0.56pp → 11.34%/10.97%; matched-template
scale +0.58pp/+0.52pp → 11.30%/10.92%; ±G9-precedent band) and the predicted
final-level shift band (−0.27% to −0.36% integrated; the whole-block S2 figures
+0.69pp/+0.62pp and −0.30% to −0.40%, and the pre-review −0.6/−0.8% d_sig-level band,
are superseded-for-S2′); all alpha0p2-derived magnitudes (P-ALPHA open); sparse-bin
worst-bin values (1-4 data tracks) as shape statements. [SUPERSEDED 2026-10-09 by the
full-sample parentage campaign (n=1,136,839 rows; clusters 5433299 + 5435475): the
single-file det.int. composition item (62.3% hyperon / 37.7% meson-baryon-mother incl.
~8% K-DIF; n=122) is closed — measured 34.47% interaction-born / 56.53%
hyperon-mother decay-born / 9.01% k_parented; and the "no-cone det.int. magnitude
unbounded" item is closed — cone survival ≈0.9984, no-cone excess +0.50% all-in
(the G11/G13 basis).]

**UNVERIFIED:** U1-U4 (Sec. 7). **DERIVED (this spec; critic-signed-off 2026-10-08,
SOUND-WITH-CONDITIONS, conditions applied):** the miss-row-fill cancellation argument and
its two invariant gates (Sec. 2.2, risk R1).

**Not determinable from existing artifacts (UPDATED 2026-10-09):** the interaction-born
fraction of the matched det.int. block on the full sample (S3's defining number) —
requires the production in Sec. 6. [SUPERSEDED 2026-10-09: that production WAS
delivered (parentage campaign, n=1,136,839 rows) and the fraction is DETERMINED —
34.47% interaction-born full-sample, with the per-bin maps as the G13 reference.
Remaining not-determinable-today: only the daughter decay-vs-interaction mechanism
split inside the kept k_parented class (never-split by campaign design; recorded
SVX/itrhis handles exist for a future quantification) and the gen-side truth-level
hyperon content on the full sample (GEN-SIDE OPEN ITEM, Sec. 6 — one light pass away,
separate decision).]

---

## 9. Recommendation — DECIDED 2026-10-09 (S2′ ADOPTED; pre-decision text retained)

**DECISION (USER-DECIDED 2026-10-09):** the refined-definition S2′ variant (S2
construction + k_parented carve-out) is ADOPTED as the target convention (Sec. 0). The
recommendation below is the pre-decision record: its step (2) (parentage diagnostic)
was delivered 2026-10-09, and its step (4) decision frame is resolved by the ruling.
Next steps under the adopted convention: build S2′ per this amended spec (single-file
smoke incl. the keeper checks → Condor campaign at unchanged scale → gates G1-G13 →
dated-candidate release regeneration with the R10 driver-copy guard → user promotion
decision per R9 hygiene); the GEN-SIDE OPEN ITEM stays parked (Sec. 6). Operational
execution hands off to h1-script-runner / h1-naf-operator per workspace scope rules.

Do not change the nominal observable now. Concretely: (1) **S1 stands as nominal** — it is
the prior-H1-consistent convention (secondaries in-observable, acceptance-modeled, with
the tracking systematic carrying the modeling uncertainty), it costs nothing, and the
det.int. content is not an uncorrected background. (2) **Commission the full-sample
MODS-level det.int. parentage diagnostic** (Sec. 6) — it is the S3-enabling first step,
is well-precedented on NAF, touches no central product, and simultaneously closes the
composition PROVISIONAL item; include the batch-node SVX smoke in its plan. (3) **Build
the S2 detint-fake response variant as a dated DIAGNOSTIC/derived product** (with the
Sec. 2.2 miss-fill suppression and all Sec. 4 gates) to bound the full removable
magnitude and to have the machinery validated — the S3 answer lies between S1 and S2 per
bin, so S2 is the measurable envelope while S3 is unmeasurable. (4) **Decide the nominal
convention (S1 vs S3) only after the parentage study quantifies the interaction-born
fraction**: if interaction-born turns out to dominate the class, S3 ≈ S2 and adopting the
removal becomes a one-step decision with the machinery already validated; if feed-down
dominates (as the single-file proxy suggests), S2 over-removes genuine physics feed-down
and the honest choices are S1 (status quo) or S3 (after the split exists). If the user's
intent is instead strictly "no GEANT-born track should count as signal whatever its
origin", then S2 is the honest implementation of that intent and is ready subject to the
Sec. 2.2 fix and the full gate suite — but it should still enter as a separate dated
variant first, not as a silent nominal replacement.

---

## Provenance

- Ledger + sidecar: `docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`,
  `data/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.json`.
- Class-separation audit: `docs/backgrounds/detint_primary_vs_material_separation_2026-10-08.md`.
- State: `sidis_highQ2/STATE.md` (backgrounds + blockers, 2026-10-08).
- Central response builder (v4 finalcandidate, no `IsGtr`/`IsStr` in matched fill; miss =
  CTG-unconsumed; `pass_reco_full` line 659):
  `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`.
- All-fake prototype (reroute reference; canonical snapshot, md5 `f4d9ea1b7121b3dc723fec39d988f2e0`):
  `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_allfake.py`
  (line anchors 694-698, 843-859, 868-874, 1021-1042, 1043-1066 verified 2026-10-08).
- Release driver (normalization formulas, lines 87-93):
  `output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/scripts/run_bbb_ibu_release_coherentprior.py`.
- Campaign precedent (73/73 shards; v4-builder shard script; merge/validate):
  `output_v6/studies/coherent_prior_response_rebuild_z005_20261006/` (cluster 5342813).
- Fake-column category vs template precedent:
  `output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/validation/template_vs_fakecolumn_report.json`.
- Prior-H1 convention: `docs/research/research-h1-1302-1321-charged-multiplicity.md`;
  gen-loop policy: `docs/handoffs/memo-mc-truth-policy-charged-sidis.md`;
  SVX feasibility: `docs/research/research-material-vertex-dstfree-2026-09-09.md`;
  parentage precedent: `docs/diagnostics/electron_parentage_hATmods_cleanrun_2026-07-31.md`.
- Parentage campaign (delivered 2026-10-09; authoritative for the Rev 2 amendment):
  results `docs/backgrounds/detint_parentage_results_2026-10-09.md` (Condor cluster
  5433299, 865 units) + JSON twin; mother-species re-slice + per-bin maps
  `docs/backgrounds/detint_motherspecies_perbin_results_2026-10-09.md` (cluster
  5435475) + JSON twin; CSVs under
  `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/`:
  `perbin_contamination_motherspecies_2026-10-09.csv` (G13 reference),
  `kparented_composition_2026-10-09.csv` (keeper PDG list),
  `motherspecies_decayborn_2026-10-09.csv`, `composition_per_stratum.csv`.
- Keeper branch verification (Rev 2): per-reco-leg `GenMotherPDG` fill in
  `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/builders/ChargedSidisTreeBuilderHighQ2_v6.C`
  (lines ~1746-1750; full `H1PartMCArrayPtr` indexing resolves STR mothers; DIF-muon
  mother-PDG precedent ~1778) — read-only source inspection 2026-10-09.
- Releases referenced (read-only): canonical RAW
  `output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`;
  Born `output_v6/central/data_release/born_crad_dj14xfcut_..._2026-10-07/`.
- Rev 2 (2026-10-09) amended this file only. No command was launched against any
  workspace product; all checks (both revisions) were read-only
  (script/text/JSON/CSV inspection) within login-node policy.

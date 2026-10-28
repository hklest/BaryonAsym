# Cone α<0.2 cut on the central track selection — impact quantification from existing artifacts

- **Study date:** 2026-10-11.
- **Type:** READ-ONLY impact-quantification study. No build, no re-production, no spec or release
  edits. Nothing under `docs/systematics/BGSUB_S2PRIME_REBASE_ADDENDUM_2026-10-11.md`, its JSON
  sidecar, or `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_build_2026-10-10/` was read or
  touched (another session is editing those; this study deliberately does not consume them).
- **Question adjudicated:** the user proposes imposing a cone α<0.2 cut (angle between charged-particle
  track and scattered electron) on the central track selection, expecting it to "dramatically reduce"
  fake tracks from the scattered electron and e⁺e⁻ pairs from final-state radiation. Today the central
  chain has NO such cut on tracks (the stage-1 `RejectTracksNearScatElec(0.2)` call at
  `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:394` affects hadronic calibration only, not tree
  admission); the alpha0p2-convention diagnostic chain HAS it.
- **Verdict in one line:** the cut is NOT justified by measured background reduction — it removes a
  directly measured **0.49% of the in-grid data vector (19,024/3,884,097; 0.73% in-PS)**, of which the
  measured electron-class share is only ≈**0.30 pp**, versus ≥**7.24%** externally-removable
  contamination; its real, quantifiable value is template-consistency (M-2/P-ALPHA closure), and it
  does NOT shrink the bgsub set or cure F-1.

---

## 1. Frame: what the cone is and is not in this workspace

- **Central chain (current canonical basis, S2′):** no cone on tracks. The response reco space is
  `pass_reco_full = evt & trk & not_e & not_v0_match_top` (builder lines 647–659); the H1OO-tagged
  scattered-electron track is already excluded by the v6 builder, so the "residual electron" template
  class is conversions/Dalitz/second-candidate electrons plus scattered-electron remnants — **not**
  the tagged electron itself (ledger §6). Imposing the cone therefore cannot remove "the scattered
  electron"; that track is already gone.
- **Alpha0p2 diagnostic chain:** same PS selection + cone α<0.2, no e/V0 truth veto in its response
  convention. Its data vector was built from the **same 658 central v6 data trees**
  (`data_vector_alpha0p2_...json` input list = `output_v6/central/sidistree/data/...`), with exact
  pre-cone parity: `alpha_pre = 3,902,480 = central n_tracks_in_ps` (ledger §4 counter cross-check).
  This parity is decisive twice: (i) it proves the cone is applicable at the vector/response-builder
  level from existing trees — **no tree re-production is required**; (ii) it makes the cone-removal
  data counters exact measurements on the central selection.
- **Cone semantics:** reco-only. Standing ruling (`docs/status/alpha0p2-background.md`): "The
  `alpha < 0.2` scattered-electron cone cut is reco-only and must not be imposed in gen-only `C_rad`."
- **Precedent:** Grebenyuk's H1 charged-particle measurement used track–scattered-electron angle
  >0.2 rad; the 2026-10-05 provenance critic loop closed "lab-frame alpha cone is intended physics
  (Grebenyuk precedent)" with no frame-convention defect (STATE.md backgrounds section). So the cone
  is a *defensible selection convention* — that is a different question from background reduction.
- **Prior adjudication of "dramatic":** the near-electron study already investigated an apparent
  **80% removal and found it "a misread of normalized/control/composition plots"**
  (`docs/status/alpha0p2-background.md`; archived RUN_STATUS 2026-07-03). The user's expectation
  restates a claim already tested and rejected at the integrated level.

---

## 2. Per-class in-cone evidence (Task 1)

Class magnitudes are the packaged L351000 cone-applied template totals (STATE.md track-level totals;
ledger §6). "In-cone share" = fraction of the class removed by α<0.2.

| Class (template magnitude, cone-applied) | In-cone share under α<0.2 | Evidence status | Note |
|---|---|---|---|
| Residual non-V0 electrons (148,976.44 = 3.8355% of central vector; ≈4.1441% no-cone) | **7.3027% (RG) / 7.587% (DJ)** of the class | **MEASURED** (`validation_alpha0p2_final_summary.json` `.mc_alpha_counts`: 215,731/2,954,136 RG; 273,538/3,605,110 DJ). Variant counting in the validated near-electron study: 5.07632%/5.26673% (different truth-classifier convention; also measured) | **92.4–92.7% of the class survives the cone.** Cone removes ≈0.29–0.31% of the data vector from this class. Composition (scattered-electron remnant vs conversion/Dalitz vs FSR pairs) NOT decomposed in any artifact (ledger §9 open item); the yield is dominated by a low-z/low-pT wide-angle component |
| V0 daughters (132,065.28 = 3.4002%) | unknown | **NO COUNTER** (ledger §9: "no cone-survival counter exists for V0 daughters"; no-cone magnitude unbounded) | Soft aggregate bound only: the non-electron in-cone MC population is ≈0.11% of non-electron tracks and ≈94% of it is true π/K/p (see true-hadron row), so the V0 in-cone share is small but unquantified |
| low-Q2 leakage (208.38 = 0.0054%) | unknown; template built with **α<0.1** cone (radius mismatch) | **NO COUNTER** + selection mismatch (ledger §6 caveat) | Bounded tiny: the α0p1→α0p2 full-chain IBU ratio has mean 0.99972 (validation summary), i.e. the 0.1→0.2 rad widening is worth ≈0.03% chain-level → low-Q2 effect ≪0.01% |
| det.int. Class 2 (27,405.80 = 0.7056% of vector) | **≈0.16% of the class** (parentage cone survival ≈0.9984) | **MEASURED at class level** (S2′ spec Sec 4 G10 note: "cone conditioning now BOUNDED: parentage cone survival ≈0.9984 — R2 closed at class level") | The S2′ removable fraction 0.909920 ± 0.000269 is mother-lineage-based and cone-independent; the class is essentially untouched by the cone |
| MC-unflagged reco fakes (the `f_i` class; 10.7141%/10.4019% removed pre-S2′) | unknown per class | **NO PER-CLASS COUNTER** | Bounded by the same aggregate: fakes are part of the ≤6% of the non-electron in-cone population that is not true π/K/p; expected `f_i` shift ≲0.1–0.2 pp (ESTIMATE, §3.4) |
| TRUE hadrons π/K/p (the signal) | **0.10649% (RG) / 0.11354% (DJ)** of true hadrons | **MEASURED** (validated near-electron study) | ≈94% of the non-electron in-cone MC population (67,448 of 71,546 RG, count level). This is SIGNAL LOSS, recovered statistically by the acceptance correction — not background reduction |
| DIFFVM ρ (285.78 = 0.0074%) | **0/2,102 = 0.000%** | **MEASURED DIRECTLY** (STATE.md DIFFVM block; QA-verified physical, not an unapplied cut) | Negligible either way |
| **Aggregate MC** (all reco tracks in PS) | **0.4259% (RG: 287,277/67,440,033) / 0.4524% (DJ: 364,520/80,569,547)** | **MEASURED** (`.mc_alpha_counts`) | Matched electrons are **75.1% of the cone-rejected MC population in both generators**; the remaining ≈25% is ≈94% true hadrons + small V0/det.int./fake parts |

**Adjudication of "dramatic reduction":** NOT SUPPORTED for any class. The only class with a measured,
sizeable in-cone share is the residual-electron class, and the measured share is **7.3–7.6%** — the
cut removes ≈0.30 pp of a ≈4.1% problem and leaves 92.4–92.7% of the class in the data. The scattered
electron itself is already excluded by the builder. FSR e⁺e⁻ pairs and conversions are a
non-decomposed sub-class of the electron template; the measured class survival says the class yield is
dominated by wide-angle (out-of-cone) tracks. V0, low-Q2, and unflagged fakes have no counters, and
the two classes with direct measurements (det.int., DIFFVM) are essentially untouched (0.16%, 0%).
The claim IS partially true per-bin: data track-level removal reaches ≈36.6% in global bin 24
(h+ S0x0, z∈[0.2,0.3), pT∈[1.0,1.5) under the v3 binmap decode; next bins ≈33.7/33.7/30.7%; measured
for α<0.1, so α<0.2 per-bin removal is ≥ these), and the worst-bin MC removed population is
**96–99% true e±** — the cone is a targeted per-bin electron cleanup, not an integrated one.

---

## 3. Integrated impact estimate (Task 2)

### 3.1 Directly measured total removal (no estimate needed)

The alpha0p2 campaign ran the identical selection on the same 658 data trees with the cone; the
pre-cone count equals the central in-PS count exactly. Imposing the cone on the central chain
therefore removes, by direct measurement:

- **In-PS: 28,582 of 3,902,480 data tracks = 0.7325%.**
- **In-grid (the data-vector quantity the unfolding consumes): 3,884,097 → 3,865,073 = −19,024
  tracks = −0.4897%.**

This is a hard upper bound on anything the cut can remove — background included.

### 3.2 Decomposition of the removal (MC-based; flagged)

| Component of the 19,024 in-grid removed tracks | Tracks (est.) | % of old data vector | Status |
|---|---|---|---|
| Residual-electron class in-cone | ≈11,700–12,200 | ≈0.29–0.31 pp | MEASURED survival × template (**the ~0.3 pp measured floor**) |
| TRUE hadrons in-cone | ≈2,300–2,700 | ≈0.06–0.07 pp | MEASURED rate × signal content (signal loss, acceptance-recovered) |
| V0 + det.int. + unflagged fakes in-cone | ≈4,000–4,800 | ≈0.10–0.12 pp | NO PER-CLASS COUNTERS (aggregate-bounded) |

Electrons are ≈63% of the in-grid removal; the unattributed non-hadron remainder is ≈0.1 pp.

### 3.3 The one genuine data-side argument FOR the cone — and its bound

MC predicts, lumi-scaled count-level, only ≈11.0k (RG) / ≈12.2k (DJ) cone-rejected tracks at
L_data = 351,000 nb⁻¹; data has 28,582 — an integrated excess of **×2.3–2.6**, consistent with the
validated per-bin statement "data removal fractions exceed MC by factors about 1.8–2.6 in hot bins"
(`alpha0p2-background.md`). The worst-bin MC removed population being 96–99% true e± implies the
excess is plausibly electron-induced data tracks (unmatched electron fragments etc.) that the MC
templates/response do not describe at the measured rate. The cone excises this population wholesale —
a real robustness gain in exactly the region where the `f_i` closure assumption (ledger §9.1) is
weakest. **But its size is bounded by the same measurement: ≤0.49% of the data vector.** Even
attributing 100% of the removal to background, the cone addresses ≤0.49 pp of the ≥7.24 pp
externally-removable contamination (≤6.8% of the problem).

### 3.4 Contamination fractions and f_i after imposition (ESTIMATES, flagged)

- **Background fractions of the data vector:** no-cone (with the quantified electron rescale):
  e ≈4.1441% + V0 3.4002% + lowQ2 0.0054% ≈ **7.55%** of 3,884,097 (≈293,233 tracks). Cone-adopted:
  (148,976.44 + 132,065.28 + 208.38)/3,865,073 = **7.2766%**. Net improvement ≈ **−0.27 pp** of the
  vector (−3.6% relative). The contamination problem is essentially unchanged: the packaged
  cone-applied electron template alone is 3.8543% of the new vector.
- **Fake fraction `f_i`:** the central response reco space excludes flagged e/V0, and ≈75% of the
  cone-rejected MC population is matched electrons — i.e. outside the response already. The response
  rebuild loses only the in-cone non-e/non-V0 content (≈0.11% of response content, ≈94% of it signal).
  Response-internal `f_i` (pre-S2′ 10.6153%/10.3124%; S2′ 11.2403%/10.8737%) is therefore expected to
  shift by **≲0.1–0.2 pp, sign undetermined** (no in-cone counter for the fake column's own content).
  The data-weighted convention (pre-S2′ 10.7141%/10.4019%; S2′ 11.3577%/10.9768%) additionally
  re-weights hot bins (up to −30/−37% local data content); second-order shift **≲0.1–0.3 pp,
  plausibly down** if the removed data tracks are fake-like. All flagged ESTIMATE — no artifact
  measures the in-cone fake content. (Under S2′, the det.int. term is cone-insensitive — parentage
  survival 0.9984 — so the variant `f_i` shifts by the same ≲0.2 pp.)

---

## 4. Consequences for open findings if the cut is imposed (Task 3)

**(a) F-1 (electron subtraction magnitude): NOT CURED — the electron vector remains material.**
The residual-electron magnitude moves only from ≈4.1441% (no-cone, PROVISIONAL) to 3.8355%
(cone-applied, now selection-consistent) — a −0.31 pp change of a ≈4% class (−7.4% relative). The
worst populated bins (h+ b349 26.95%, d=9; h− b350 15.67% at d≥100) are ALREADY cone-applied template
numbers and stand as-is; the charge asymmetry (h+ 3.749% / h− 3.963%, alpha-D; ≈0.21 pp) persists.
First-order bias on Born totals without the electron vector remains ≈+3.7 to +4.1% relative. The
critic's F-1 cure (extend the subtraction set to e+V0+lowQ2) is unchanged and still mandatory.

**(b) M-2 (cone-survival transfer gap): CLOSES for e/V0 — the ÷0.9241/÷0.9270 rescale becomes
unnecessary.** With the cone in the central selection, the packaged cone-applied e/V0 templates are
selection-consistent by construction: the electron no-cone rescale (the source of the ≈4.1441%
PROVISIONAL figure) is no longer consumed, and the V0 no-cone unboundedness (ledger §9/§10 PROVISIONAL
item 2) closes without needing a counter. Residual: the low-Q2 template was built with the α<0.1 cone
(radius mismatch), bounded ≪0.01% by the α0p1→α0p2 chain ratio (mean 0.99972) — carry as an explicit
tiny caveat or rebuild that one vector with α<0.2 (cheap, single low-Q2 sample). The e∩V0 overlap
(112.2093; 0.0364%, ignored per user decision 2026-10-08) carries over unchanged.

**(c) P-ALPHA (template heritage / alpha-veto consistency): both critic conditions discharge, scope
narrows.** Condition (i) (electron rescale or central-selection templates) discharges — the templates
become same-selection products. Condition (ii) (V0/low-Q2 cone-survival gap closed or carried) closes
for V0 by construction; low-Q2 remains the small radius-mismatch carry of (b). The "alpha0p2-derived
magnitudes are diagnostic-grade" caveat falls for the e/V0 templates; the residual alpha0p2-vs-central
convention difference (the e/V0 truth veto in the response) is the already-ESTABLISHED central
convention question (ledger §5), not a new one. P-ALPHA as a standing blocker would need a formal
closure/supersession entry in STATE.md.

**(d) bgsub subtraction set: STAYS e + V0 + lowQ2.** It does NOT shrink to V0+lowQ2+tiny-e.
Post-cone magnitudes: e 3.8543% + V0 ≈3.4168% + lowQ2 0.0054% ≈ **7.28%** of the new vector (vs the
ledger-prescribed 7.2411% of the old vector — the ledger figure is cone-applied numerators over the
no-cone denominator; like-for-like no-cone is ≈7.55%). The S2′ exactly-once rules are untouched:
det.int. stays response-internal (external det.int. subtraction remains FORBIDDEN, more strongly under
S2′), DIFFVM stays systematic-only.

---

## 5. Cost and scope of imposing the cut (Task 4)

### 5.1 Rebuild chain (evidence-annotated)

1. **Builder change — script level, NOT tree level.** The alpha0p2 campaign consumed the same 658
   central data trees and reproduced the central pre-cone count exactly, so the cone is computable
   from existing branches at the data-vector/response-builder level. Touch (as dated study variants,
   never in place): `build_data_vector_v4.py` and `build_migration_th2_per_charge_v4.py` (the
   `alpha_ok`/`ELECTRON_ALPHA_VETO_RAD` logic listed as DO-NOT-PORT in the S2′ spec becomes TO-PORT),
   with the S2′ detint reroute + k_parented keeper on top. **Tree re-production: NOT required**
   (contrary to the natural presumption; the parity measurement is the evidence). Stage-1
   `RejectTracksNearScatElec(0.2)` (calibration-only) is unaffected.
2. **Smoke:** login-node single-file smokes (≤5 min, one file) of both modified builders + one MC
   file; alpha_pre parity and rejected-count reproduction (28,582) as smoke assertions.
3. **Condor re-production:** data vector (658 files; precedent cluster 5435157); cone response
   rebuilds RAPGAP31 (73-shard precedent 5342813; S2′ campaign used 174 shards across both
   generators, clusters 5469609/5469610) + DJANGOH14 cross leg (502 files); merges + validations.
   No tree jobs.
4. **Response/prior/`f_i`:** coherent `body+miss` prior computed FROM the new (cone+S2′) response
   (R10 dated-driver guard, repointed constants); per-bin `f_i` re-derivation on both scales; gates
   G1–G13 re-run with the G9 band re-derived (predicted shift ≲0.2 pp response-internal).
5. **S2′ variant re-derivation:** the reroute/keeper/miss-fill-suppression construction ports
   unchanged onto the cone response; G13 per-bin removable map reusable (parentage cone survival
   0.9984 bounds the det.int. transfer).
6. **bgsub chain:** e+V0+lowQ2 external vectors = the packaged cone-applied templates REUSED AS-IS
   (now selection-consistent; no rescale); low-Q2 radius caveat carried or rebuilt; pre-unfold
   additive subtraction, re-unfold, addback; 24-item gate + the M-1 set-composition item + NEW
   cone-parity gate items (alpha_pre parity; rejected 19,024/28,582 reproduction; response content
   loss vs the MC in-cone predictions; `f_i` shift inside the predicted band).
7. **C_rad: NOT re-derived.** The cone is reco-only and must not enter gen-only `C_rad` (standing
   ruling, `alpha0p2-background.md`); the gen-level fiducial is unchanged. S2′ precedent: C_rad
   tables UNCHANGED, per-bin Born shift == per-bin raw shift (gated ≤1e-12). DJANGOH14 releasebin
   primary / RAPGAP31 emfix alternative are reused unchanged.
8. **Budgets:** full re-derivation on the new nominal — 15-component S2′ pattern + re-derived bgsub
   components (→19 per M-3), including the shifted-response detector components (S2′ remaining-work
   item 1), 1000-toy stat sidecars, all anchors (RU/CP/SA dilution anchors), `fakecol_mcstat_rel`.
9. **Gates + documentation cascade:** critic gate re-run; STATE/RUN_STATUS/supersession and promotion
   records; every fraction quoted against the new 3,865,073-based denominators; side studies keyed to
   the no-cone selection (BSA fork v6bsa, qT/Q hybrid, alpha0p2 diagnostics) need selection-mismatch
   flags or their own re-derivations.

### 5.2 Reusable unchanged vs must re-derive

**Reusable unchanged:** binning/binmap v3 + releasebin cells (980/490); `L_data = 351000.0 nb⁻¹`;
C_rad tables (gen-level; reco-only cone excluded by standing ruling); the v6 trees themselves (658
data + 364 RAPGAP31 + 502 DJANGOH14 + DIFFVM/low-Q2 samples); the packaged cone-applied e/V0 templates
+ DIFFVM alpha0p2-variant template (become the selection-consistent subtraction vectors); gate
machinery (G1–G13 suite, 24-item bgsub gate, `f_i`/weight audits); bgsub scripts (vectors builder,
re-unfold driver, budget 13→17/19 machinery, `lowq2sub` precedent); the S2′ spec construction
(reroute + keeper + miss-fill suppression, R10 guard) and the G13 parentage per-bin reference map;
normalization records (L_MC catalogs, template scales).

**Must re-derive:** cone data vector; cone response (both generators) + merges; coherent prior; `f_i`
(per-bin, both scales); S2′ variant response/`f_i`/gates on the cone basis; IBU/BBB raw release; Born
release (same C_rad, new raw); 1000-toy sidecars; all 15+ budget components and all anchors on the new
nominal; bgsub chain outputs; acceptance maps/QA; the track-level background systematic blockers are
NOT cured (still missing `overlap_980`/`n_raw_980` in the alpha0p2 aggregates); the full documentation
cascade. Net effort: comparable to the S2′ campaign (174-shard response builds) plus the entire
downstream release/budget/bgsub chain — i.e. a full-fiducial re-derivation, for a bounded −0.49% raw
yield effect.

---

## 6. Verdict and recommendation (Task 5)

**Justified by measured background reduction alone? NO.** The integrated removal is bounded by direct
data measurement at **0.49% of the in-grid data vector** (0.73% in-PS), the measured electron-class
share is **≈0.30 pp** (7.3027%/7.587% of a class that is itself only ≈4.1%), and every other class is
either untouched (det.int. 0.16% in-cone; DIFFVM 0/2,102), unmeasured-but-aggregate-bounded (V0,
unflagged fakes), or signal (true hadrons 0.106–0.114%, acceptance-recovered). Against the
ledger-prescribed **7.24%** externally-removable contamination (e+V0+lowQ2; ≈7.55% no-cone
like-for-like), the cone removes **≤6.8% of the problem**. "Dramatic reduction" is contradicted; the
apparent-80% claim was already adjudicated a misread; the tagged scattered electron is already
excluded by the builder. The measured ×2.3–2.6 data/MC in-cone excess is the only data-side argument
in favor, and it is bounded by the same 0.49%.

**Justified by template-consistency/simplification? PARTIALLY — this is where the real value is:**
closing M-2 (no ÷0.9241/÷0.9270 rescale; V0 no-cone unboundedness closed by construction), discharging
both P-ALPHA template-heritage conditions, making the packaged templates selection-consistent (partly
closing ledger §11.4 "no central-selection template production exists" for e/V0), excising the region
where the `f_i` closure assumption is measured weakest, and aligning the selection with the published
H1 charged-particle precedent (Grebenyuk, angle >0.2 rad). These are worth ≈0.3 pp of PROVISIONAL
uncertainty on the electron vector plus the removal of an unbounded V0 caveat — consistency gains, not
background statistics.

**Recommendation:** Do NOT impose the cone as a background-reduction measure. The already-mandatory
F-1/M-2 cure (S2′ re-base + e+V0+lowQ2 external vectors, electron cone-survival rescale applied, V0
gap carried explicitly) delivers the same physics correctness without a fiducial change. If the
consistency/precedent gains are wanted, the cheaper standalone option is a central-selection (no-cone)
template production (ledger §11.4) rather than a full-chain fiducial change. If the cone IS adopted
(e.g. for Grebenyuk-precedent alignment), it must be sold as a selection-convention change with:
bounded raw-yield effect **−0.49%** (in-grid; −0.73% in-PS), background improvement **≈−0.27 pp** of
contamination fraction, `f_i` shift **≲0.2 pp**, electron vector still **3.85%** (set stays
e+V0+lowQ2 ≈7.28%), and the full §5 re-derivation chain executed on the S2′ basis.

---

## 7. Provenance

- `sidis_highQ2/STATE.md` (S2′ promotion override; backgrounds section; DIFFVM block).
- `sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md` §4–§10
  (cone counters, per-class ledger, PROVISIONAL register, §9 no-counter list).
- `sidis_highQ2/docs/critiques/critic-gate-bgsub-promotion-2026-10-11.md` (F-1, M-2, P-ALPHA
  conditions, cure conditions 1–5).
- `sidis_highQ2/docs/status/normalization-verification-bgfracsub-2026-10-10.md` §4–§7
  (f_i basis, caveats).
- `sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md` (S2′ class table,
  removable fraction 0.909920 ± 0.000269, parentage cone survival ≈0.9984, DO-NOT-PORT list).
- `sidis_highQ2/docs/status/alpha0p2-background.md` (80%-misread adjudication; 5.07632%/5.26673%
  and 0.10649%/0.11354% measurements; 36.6% hot bin; 1.8–2.6× data/MC; reco-only-C_rad ruling;
  Grebenyuk precedent; L351000 template totals).
- `H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/provenance/
  validation_alpha0p2_final_summary.json` (data `alpha_pre/alpha_rejected/alpha_after`;
  `.mc_alpha_counts`; α0p1→α0p2 ratio) and its `data_vector_alpha0p2_...json` (658 central-tree
  input list).
- `H1ChargedSidis_HighQ2/output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/
  validation/template_vs_fakecolumn_report.json` (G9/S1b template-vs-fakecolumn content equality,
  allfake-convention caveat).
- Archived `docs/archive/pre-compaction-startup-pipeline-2026-07-03/RUN_STATUS_20260703T000000.md`
  (near-electron study provenance of the validated compact numbers).

## 8. Study limits and blockers

- The in-cone decomposition of the data-side removal is NOT measured (no truth in data); the per-class
  split in §3.2 is MC-template-based and inherits the ledger §9 closure assumptions.
- No cone-survival counters exist for V0 daughters, low-Q2, or the unflagged-fake class; the low-Q2
  template carries an α0p1 radius mismatch; the aggregate bounds used here are count-level and cannot
  replace dedicated counters.
- The integrated ×2.3–2.6 data/MC in-cone excess assumes the `.mc_alpha_counts` are full-sample raw
  counts at the recorded L_MC; the per-bin 1.8–2.6× excess is independently validated, so the
  conclusion does not rest on the integrated scaling alone.
- The electron-template composition (scattered-electron remnants vs conversions vs FSR pairs) is not
  decomposed in any artifact (ledger §9 open item); per-class FSR statements cannot be made.
- This study does not consume the in-flight S2′-rebase addendum or Set-R build products (see scope
  note); all findings rest on installed/released artifacts and the 2026-10-11 critic gate record.

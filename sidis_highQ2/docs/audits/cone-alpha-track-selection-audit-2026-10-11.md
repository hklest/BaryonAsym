# Cone-α track-selection audit — high-Q² charged SIDIS central chain (v6/z005)

- **Audit date:** 2026-10-11. **Mode:** READ-ONLY code audit. No file modified, no
  compile, no build, no job launched.
- **Premise under test (user statement):** the central chain does NOT impose the cone
  α<0.2 cut (angle between charged-particle track and scattered electron) on the track
  selection; the cone concept is used only inside the Empz calculation.
- **Workspace:** `/data/dust/group/h1/klesthen/BaryonAsym/`; package
  `H1ChargedSidis_HighQ2/` (v6 central production, steering `condor_v6/central/`).
- **Authoritative state consulted:** `sidis_highQ2/STATE.md` (2026-10-11),
  `sidis_highQ2/docs/RUN_STATUS.md` (S2′ promotion sections),
  `sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md` (§2–§4),
  `sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md`
  (Rev 2, §2.1 DO-NOT-PORT, Provenance).
- **H1OO framework source consulted (read-only):**
  `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Calculator_and_Weights/`
  (`H1CalcHad.{h,C}`, `H1CalcFs.C`, `H1CalcKine.C`).

---

## 1. Verdict

**Point 1 — no cone α<0.2 cut on the fiducial track sample: CONFIRMED at builder-code
level.** No track-vs-scattered-electron cone cut exists anywhere in the central chain:
not in the stage-2 C++ tree builder (zero occurrences of `alpha`/`cone`/
`RejectTracksNearScatElec` in `src/ChargedSidisTreeBuilderHighQ2_v6.C`), not in the
stage-1 event-list builder (the single `RejectTracksNearScatElec(0.2)` call there is
H1OO hadronic-calibration configuration, not track admission), and not in any central
python selection predicate (`pass_reco_full` in the response builders, `pass_reco` in
the data-vector builder — no alpha term in any of them). The current canonical S2′
variant response builder carries the same no-cone predicate and an explicit
DO-NOT-PORT note: "the alpha cone … — central is no-cone".

**Point 2 — the cone is used only inside the Empz calculation: CONFIRMED, with one
precision nuance.** The cone exists today exclusively inside the H1OO hadronic-final-
state (HFS) reconstruction configured at stage 1
(`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:394`). Its rejection is applied in
exactly two places in `H1CalcHad.C`, both inside HFS four-vector sums over HAT
particle candidates — it never touches the `H1PartSelTrackArrayPtr` track array from
which `ChargedTot` is filled. Nuance: the HFS object feeds not only the full-event
E-pz (`H1CalcFs::GetEmpz()`, stage-1 envelope cut) but also, through the same
four-vector, the sigma/eSigma kinematic estimators (`H1CalcKine::GetYes()` uses the
sigma-method `GetXs()`). So the precise scope is "HFS reconstruction (Empz and
HFS-derived kinematics)", not track admission. A second precision point: the H1OO
cone metric is an **η–φ distance** `sqrt(Δφ² + Δη²) < 0.2`, NOT the 3D opening angle
in radians used by the alpha0p2 chain — the two "0.2" cones are different geometric
objects.

**Resolved doc-level contradiction (recorded because it is the likely source of the
premise question):** `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md`
claims the α<0.2 veto "is applied upstream at v6 tree production" and "applies to the
`ChargedTot` inputs" (its table rows "APPLIED (upstream tree level)"). That mechanism
claim is **refuted at code level** by this audit and was already superseded by the
2026-10-07 consistency ledger ("affects hadronic calibration only, not tree
admission"). Decisive counter-evidence, independent of code reading: the central data
vector `n_tracks_in_ps = 3,902,480` equals the alpha0p2 chain's mask-off
`alpha_pre_count = 3,902,480`; the downstream python cone then removes a further
28,582 data tracks (0.733%) — impossible if the trees were already cone-vetoed
(ledger §4; counter JSONs in the alpha0p2 study). The later authoritative documents
(2026-10-07 ledger; 2026-10-08/09 detint spec §2.1 "DO NOT PORT … central is
no-cone") are consistent with the code; the 2026-10-06 audit's APPLIED rows should be
treated as superseded on the mechanism (its other finding — that data and response
draw from the same v6 trees with no data-vs-response alpha mismatch *within* each
chain — stands).

---

## 2. Central v6 track selection — exact predicates (quoted)

### 2.1 C++ stage-2 tree builder (tree admission) — `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`

The builder is deliberately **open-cuts** ("ChargedTot stores all primary central
tracks with only minimal sanity filters"; the physics fiducial is applied downstream
in python). Track admission, Pass 1 (central tracks):

- `:1581` — `if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;`
  (skip the H1OO-tagged scattered-electron track; a finder **bit**, not a cone)
- `:1585` — `if (!trk->IsFromPrimary()) continue;`
- `:1588` — `if (!trk->IsCentralTrk()) continue;`
- `:1598-1600` — finite-kinematics sanity (`Pt>0`, finite `Eta`, finite `P`)

Pass 2 (combined tracks):

- `:1922` — `if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;`
- `:1924-1926` — `IsFromPrimary`, skip central (handled in Pass 1), `IsCombinedTrk`
- `:1931-1933` — `if (Pt < 0.05f) continue;`, finite Eta, `if (P > 100.0f) continue;`
- `:1939-1950` — ΔR<0.05 same-charge dedup against kept centrals

**No cone cut in either pass.** `grep -ci` hits in this file: `alpha` 0, `cone` 0,
`RejectTracksNearScatElec` 0. The only electron-related per-track veto is the
`IsScatElec` finder-bit skip above. The MC truth flags (`IsMatchedElectron`,
`IsMatchedV0Daughter`, …) are only *assigned* inside the MC-matched block
(`:1731-1771`, inside `if (matched)`), i.e. MC-only labels, identically 0 in data
(ledger §4, empirically verified on two periods).

### 2.2 Central python reco predicate (data vector + response reco space)

Central response builder
`H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`
(this is the file the 2026-10-07 ledger cites at lines 647–659):

- `:648` — `not_e = (batch["IsMatchedElectron"] == 0)`
- `:652` — `not_v0_match_top = (batch["IsMatchedV0Daughter"] == 0)`
- `:659` — `pass_reco_full = evt & trk & not_e & not_v0_match_top`
- `:677` — `pass_reco_full = pass_reco_full & (~z_outlier)` (computed-z ∈ (0,1) guard)
- `trk` at `:636-647`: `Pt > Pt_min`, `|Eta| < 1.6`, `NHitDedx >= 5`,
  `RadLength >= 5.0`, `|DcaPrime/cosh(Eta)| < 2`, `IsFromPrimary == 1`,
  central-or-combined class, `xF_SIDIS_es < 0` (current hemisphere).
- `evt_fid` at `:608-618`: `150 < Q2es < 20000`, `0.1 < Yes < 0.7`, `Empz > 45`;
  `evt_cleanup`: `|PVz| < 35 cm`.

**No alpha term.** `grep -ci alpha` on this file: 0.

Current canonical S2′ detint-fake variant response builder
`H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
(the response of the promoted release
`output_v6/central/data_release/bbb_ibu_detints2prime_..._2026-10-09/`):

- `:867` — `pass_reco_full = evt & trk & not_e & not_v0_match_top` (identical
  predicate; the variant only reroutes non-keeper det.int. matched legs into the fake
  column, `fc=3/4/5` categories, conserving `B_ij + F_i`)
- Header `:71-74` (DO-NOT-PORT list, quoting verbatim): "the e/V0 truth-veto
  disablement (prototype 685-693) -- the central e/V0 exclusion stays (pass_reco_full
  keeps not_e & not_v0_match_top); * the alpha cone (prototype alpha_ok /
  ELECTRON_ALPHA_VETO_RAD) -- central is no-cone;"

Central data-vector builder (same directory, `build_data_vector_v4.py`):

- `:397` — `pass_reco = evt_fid & evt_cleanup & trk`
- `:405` — `pass_reco = pass_reco & z_ok` (`n_tracks_in_ps` counted at `:398`)

Other central-chain replicas of the same predicate (all no-cone):
coherent-prior response shard
`output_v6/studies/coherent_prior_response_rebuild_z005_20261006/scripts/build_coherent_prior_response_shard.py:799`
(`pass_reco_full = evt & trk & not_e & not_v0_match_top`); systematics shifted
responses
`scripts/unfolding/tunfold_official_v4_finalcandidate_.../build_shifted_response_data_vector_v4.py:521`
(`pass_reco_full = evt_fid & evt_cleanup & trk & z_ok`, with not_e/not_v0 folded into
`trk` at `:512-521`).

Provenance caveat: the on-disk central scripts carry later comment edits
("diagnostic variant … No Pt_max", `Pt_min = 0.15`; file mtimes 2026-09-09) while the
2026-06-10 release is older; the S2′ builder (2026-10-09, current canonical) carries
the same constants (`TRACK_CUTS_RECO` at its `:357-362`), so the quoted constants are
the ones in use by the current canonical chain. The release-name token `pT5` refers
to the 5-bin gen-pT reco-support binning per the S2′ release metadata `cuts` string,
not to a `Pt_min=0.5` cut.

### 2.3 Where the cone IS used today (Empz/HFS scope)

Stage-1 builder `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`,
hadronic-calibration initialization (reco mode only; skipped in `genOnly`):

- `:389-396` —
  `H1HadronicCalibration::Instance()->SetCalibrationMethod(H1HadronicCalibration::eHighPtJet);`
  `gH1Calc->CalibrateHadrooII();` `gH1Calc->CalibrateLatestHadrooII();`
  `gH1Calc->Had()->DoNotUseIron();` **`gH1Calc->Had()->RejectTracksNearScatElec(0.2);`**
  `gH1Calc->SetFinalStateToNC();`

H1OO semantics (release 4.2.1, `h1oo/H1Calculator_and_Weights/`):

- `H1CalcHad.h:218-222` — `void RejectTracksNearScatElec(Double_t R=0.2){ // reject
  tracks in a cone with radius R around the scattered electron
  bRejectTracksNearScatElec=true; fElecConeRadious = R; }`
- The flag is used in exactly **two** places in `H1CalcHad.C`: `:391`
  (`GetShiftedHFS`) and `:1087` (`GetModsHFS`) — both inside
  `partcands[i]->IsHFS()` four-vector sum loops over HAT particle candidates, with
  `if ( (ScatElec.E()>0) && (partcands[i]->IsSelTrack()) ){ ... Float_t dist =
  Dist(ScatElec, PartVec); if (dist<fElecConeRadious) continue; }`.
  `ScatElec = gH1Calc->Elec()->GetFirstElectron()` (`H1CalcHad.C:379`).
- `H1CalcHad::Dist` (`H1CalcHad.C:1104-1125`) returns the **η–φ distance**
  `sqrt(Δφ² + Δη²)` — not a 3D opening angle.
- Reach: `CalibrateLatestHadrooII()` → `GetHadComb()` overwrites `fHadComb =
  GetModsHFS()` (`H1CalcHad.C:263-270`) → `H1CalcFs::GetAllFinalStateParticles()`
  (`H1CalcFs.C:815-846`, = `Had->GetHad() + electrons + elecCones + muons`) →
  `H1CalcFs::GetEmpz()` (`H1CalcFs.C:484-496`, `fs.E()-fs.Pz()`).
- Consumer of that Empz in this analysis: stage-1 event envelope CUT 5
  (`src_stage1/..._v6.C:713-720`): `empz = gH1Calc->Fs()->GetEmpz(); if (empz <= 30.0
  || empz >= 75.0) { … passRecoEnv = false; }`. The same HFS also enters the
  sigma/eSigma kinematics (`H1CalcKine::GetYes()` = `GetQ2e()/(GetXs()*fSS)`,
  `H1CalcKine.C:377-410`; `GetXs` is the sigma method), i.e. the calibrated
  `Q2s/Ys/Q2es/Yes` stored per row — the "Empz-only" phrasing is correct about track
  admission but the HFS cone technically also lives under the eSigma/sigma kinematic
  estimators. The downstream `Empz > 45` fiducial uses the stage-2 stored per-row
  `Empz` branch (Sigma-method derivation from `Ys`, `src/ChargedSidisTreeBuilderHighQ2_v6.C:1171-1177`),
  which reaches the HFS cone only indirectly through eSigma-calibrated `Ys`.
- Stage 1 is an **event-list (EVLS) builder**; it performs no track selection, and
  stage 2 never queries the cone. `grep -ci` on the stage-1 file: `alpha` 0, `cone` 0,
  `RejectTracksNearScatElec` 1 (line 394).

---

## 3. Alpha0p2 reference implementation of the cone cut

The alpha0p2 chain applies the cone **downstream in python**, on the same central v6
trees, recomputing the angle from stored branches. Reference file:
`H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py`
(1338 lines; the doc `alpha0p2_application_audit_2026-10-06.md` cites `:390-397` and
`:681-698`, verified).

Constants (`:386-388`):

```python
ELECTRON_ALPHA_VETO_RAD = 0.2
ALPHA_RECO_ONLY = True
DISABLE_TRUTH_ELECTRON_VETO_FOR_ALPHA_CENTRAL = True
```

Angle definition (`:390-397`) — **3D opening angle in radians** between the reco
track momentum and the **reconstructed** scattered-electron direction:

```python
def _compute_reco_electron_alpha(Px, Py, Pz, sEX, sEY, sEZ):
    """3D opening angle between reco track and scattered-electron direction."""
    trk_norm = np.sqrt(Px*Px + Py*Py + Pz*Pz)
    ele_norm = np.sqrt(sEX*sEX + sEY*sEY + sEZ*sEZ)
    dot = Px*sEX + Py*sEY + Pz*sEZ
    cos_alpha = np.where((trk_norm > 0) & (ele_norm > 0), dot / (trk_norm * ele_norm), np.nan)
    cos_alpha = np.clip(cos_alpha, -1.0, 1.0)
    return np.arccos(cos_alpha)
```

Application (`:681-692`) — keep α ≥ 0.2 rad (veto α < 0.2); NaN α is rejected:

```python
pass_pre_alpha = evt & trk & not_v0_match_top
alpha = _compute_reco_electron_alpha(px, py, pz, sEX, sEY, sEZ)
alpha_ok = np.isfinite(alpha) & (alpha >= ELECTRON_ALPHA_VETO_RAD)
pass_reco_full = pass_pre_alpha & alpha_ok
```

- **Variables:** track `Px, Py, Pz` (ChargedTot row branches); electron `scatEX,
  scatEY, scatEZ` (row branches, filled at `src/ChargedSidisTreeBuilderHighQ2_v6.C:1128-1134`
  from HAT pointers `ElecE/ElecTheta/ElecPhi` — the LAr scattered electron, identical
  source for data and MC).
- **Which electron direction:** reconstructed scattered electron at reco level;
  **no gen-level alpha** (`ALPHA_RECO_ONLY = True`; metadata `"alpha_reco_only":
  "True"` at `:1192`). Gen-side rows (matched partners, miss/prior space) are filled
  without any cone — the response convention absorbs reco-side cuts via the miss row.
- **Separate convention difference (do not conflate with the cone):** the alpha0p2
  chain also *disables* the `IsMatchedElectron==0` truth veto
  (`DISABLE_TRUTH_ELECTRON_VETO_FOR_ALPHA_CENTRAL = True`; `pass_pre_alpha` at
  `:681` omits `not_e`) so electron-like reco rows are handled by the fake column.
  The central chain keeps `not_e`. Quoting the alpha0p2 docstring (`:52-53`):
  "alpha(track,e) >= 0.2 rad, IsMatchedV0Daughter == 0. / NO IsMatchedElectron==0
  truth-label veto is applied; electron-like reco rows surviving alpha are
  fakes/background."
- **Counters** (`:694-698`, metadata `:1191-1203`): `n_alpha_pre`, `n_alpha_after`,
  `n_alpha_rejected`, `n_alpha_rejected_matched_electron`,
  `n_alpha_survive_matched_electron` per charge — the counters behind the ledger's
  `alpha_pre_count = 3,902,480` / 28,582 data removals (0.733%) and the MC
  matched-electron cone-removal fractions (7.30% RAPGAP31 / 7.59% DJANGOH14).
- **Data-vector twin** (`copied_scripts/build_data_vector_v4.py:411-419`): same
  `_compute_reco_electron_alpha`, `pass_pre_alpha = evt_fid & evt_cleanup & trk`,
  `pass_reco = pass_pre_alpha & alpha_ok`, with data-side counters
  `n_tracks_pre_alpha / n_tracks_after_alpha / n_tracks_rejected_alpha` and
  matched-electron survival counters.
- **Precedent for a builder-side angle:** the det.int. parentage campaign design
  (`docs/backgrounds/detint_parentage_study_design_2026-10-08.md`, field P-13) already
  records per-row "Alpha cone angle (track vs HAT scatE, lab)" as an auxiliary field,
  defined as the lab angle between track momentum and HAT scatE direction — same
  3D-angle concept, recorded (not applied) so alpha0p2-conditioned subsets are
  derivable offline. An older veto-scan precedent also exists
  (`scripts/study_scat_elec_veto_optimization.py`, degree-based threshold scan of
  DIS-e rejection vs hadron loss, on an older cut set).

---

## 4. Change-scope assessment: adding the α<0.2 track cone to the central chain

**Key feasibility fact: no C++ change is strictly required.** The central v6 trees
already store, per ChargedTot row, the full reco electron 4-vector (`scatEX/Y/Z/E`,
branches `:741-744`), the track momentum (`Px/Py/Pz`, `Theta` in radians), and, on
MC, the gen electron direction (`GenElecPhi/GenElecTheta/GenElecE`, branches
`:808-810`; `GenScatE*_cg` + `GenBeamElecE_cg` on ChargedTotGen, `:599-600`).
Therefore the cone can be inserted into the python predicates exactly as the
alpha0p2 chain does — a one-line `alpha_ok` term in each selection script — followed
by Condor re-production of the response(s), data vector, and systematics.

**Option A — python-only (minimal code change, alpha0p2 precedent):**
- Insert `_compute_reco_electron_alpha` + `alpha_ok` into: the central response
  builder predicate (`pass_reco_full`), the S2′ variant builder (if the variant
  convention is retained), the data-vector builder, the coherent-prior shard
  builder, the shifted-response builders, and every active study replica of the
  predicate (see §5).
- Keep or drop `not_e` is an independent convention decision (the alpha0p2 chain
  dropped it; the detint spec §2.1 DO-NOT-PORT keeps it) — must be decided
  explicitly, not inherited silently.
- Production: full response + data-vector + systematics re-derivation on Condor
  (multi-file; `h1-naf-operator` territory once the code change is validated).

**Option B — builder-level (C++) change, if pre-vetoed trees or per-track alpha
branches are wanted:**
- **In-scope data at the insertion point:** the reco scattered-electron four-vector
  `scatElec4` is built at event level before the track loops
  (`src/ChargedSidisTreeBuilderHighQ2_v6.C:1127-1150`); per-track `Px/Py/Pz` are set
  at `:1609-1611` (Pass 1) and `:1957-1959` (Pass 2). The 3D angle is a one-liner
  against `scatElec4.Vect()` (or `scatEX/Y/Z`).
- **Insertion points:** Pass 1 after the sanity filters and kinematics fill, before
  the V5F truth-link matcher block (i.e., between `:1612` and `:1652`) — placing it
  *before* the matcher also saves the Tier-2 greedy 2°-scan cost for rejected
  tracks; Pass 2 after `:1960`. Both passes must carry it (TrackType 1 and 2).
- **Gen-level counterpart: feasible but not required.** In MC the gen scattered
  electron `genScatElec4_evt` (`:1218`, filled `:1273-1310` under `validGenElec`)
  is in scope where the gen loop fills ChargedTotGen (`:1531`), and gen hadron
  momenta come from `gp->GetMomentum()`; downstream, `GenElec*`/`GenScatE*` branches
  suffice for a python-side gen cone. However, the response structure does not need
  one: reco-side cuts act on reco legs; the matched gen partner follows its reco
  leg, and gen tracks whose reco partner was cone-rejected land in the miss row, so
  `body+miss` stays the truth marginal (the coherent prior then must be recomputed
  from the new response, exactly the S2′ R10 pattern). Adding a gen-side cone would
  additionally sculpt the truth space — a physics-policy decision
  (`h1-physics-analyst`/supervisor), not a builder necessity. The alpha0p2 reference
  is reco-only.
- **Keep the `IsScatElec` bit skip** (`:1581`, `:1922`): the tagged electron track
  sits at α≈0 so the cone subsumes it geometrically, but the bit veto is the cleaner
  primary, is data/MC-symmetric, and covers finder edge cases.
- **Ordering/efficiency:** O(1) per-track arithmetic; negligible CPU in C++ and
  vectorized numpy downstream. No ordering hazard beyond placing the cut before the
  matcher for cost; per-event state (`scatElec4`) is already computed before both
  loops. Nano-edge: define NaN/degenerate-electron behavior explicitly (alpha0p2
  rejects non-finite α; the builder has `eE>0` guaranteed by stage-1 CUT 3).
- **Expected magnitude (from the alpha0p2 counters):** ~0.733% of central data
  tracks (28,582/3,902,480) and 7.30%/7.59% of MC matched electrons
  (RAPGAP31/DJANGOH14) are inside the cone — a small but non-zero observable change;
  every released number would shift.
- **Content/schema impact (Option B):** the trees would no longer contain
  cone-rejected tracks — schema-identical but content-different, breaking
  comparability with the existing 658-file central v6 production and all products
  derived from it. A builder change requires a named fork (v6bsa precedent), a
  658-file Condor re-production, and the standard branch-schema regression checks
  (expected tree/branch counts; new alpha branch populated if added).
- **Physics-policy flag:** adopting the cone changes the central observable
  (selection definition). It also *creates* the data-vs-response consistency
  question that P-ALPHA tracks: under a cone-applied central chain the alpha0p2
  background magnitudes (e 3.8355%, V0 3.4002%, det.int. split) become
  convention-matched rather than cone-conditional. The decision and the P-ALPHA
  validation plan belong to the supervisor/physics side; this audit only scopes the
  implementation.

---

## 5. Affected consumers of the track selection (what a cone addition would touch)

Direct selection consumers (numeric outputs change; must be re-produced):

1. **Central raw data vector (980 reco bins)** —
   `scripts/unfolding/tunfold_official_v4_finalcandidate_.../build_data_vector_v4.py`
   (`pass_reco`, `n_tracks_in_ps`; current Σ 3,884,097 = h+ 1,975,301 / h− 1,908,796).
2. **Central response body + fake columns (pre-S2′ reference)** —
   `.../build_migration_th2_per_charge_v4.py` (`pass_reco_full` :659; matched and
   fake rows both draw from it).
3. **S2′ detint-fake variant response (CURRENT canonical)** —
   `scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
   (`pass_reco_full` :867; fc=0/3/4/5 fake categories; `B_ij+F_i` per-reco-bin
   conservation).
4. **Coherent-prior response rebuild shards** —
   `output_v6/studies/coherent_prior_response_rebuild_z005_20261006/scripts/build_coherent_prior_response_shard.py`
   (`pass_reco_full` :799, `~z_outlier` :817).
5. **Fake fraction f_i and the pre-unfold subtraction** —
   `scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`
   (`f_i = F_i/(Σ_j B_ij + F_i)` `:540-544`, `:1446`; `d_sig_i = d_i*(1-f_i)` `:66`);
   the S2′ response-internal RG integrated `f_i` (h+ 11.2403% / h− 10.8737%) and the
   enlarged-`f_i` variant convention would be re-derived on the new selection.
6. **IBU prior + efficiency (coherent `body+miss`)** and the release drivers
   (`run_bbb_ibu_release*.py`, S2′ stub) — prior recomputed from the new response
   (R10-pattern repoint); raw and Born releases re-issued (C_rad tables unchanged as
   objects; per-bin Born values shift with the raw parent).
7. **Systematics budget (15-component S2′ and its 13 central-mirroring components)**
   — every shifted-response/data-vector re-derivation:
   `.../build_shifted_response_data_vector_v4.py:521`,
   `scripts/unfolding/paired_legs_detints2prime_2026-10-11/build_shifted_response_data_vector_s2prime_v4.py`,
   tracking ensemble, B1 replica floors, phi_h reweight, stat toys; the ~+2.5 pp
   data-side-only convention-gap caveat would need re-adjudication.
8. **Study replicas of the predicate** — binning variants
   (`tunfold_official_v4_z2tmd_*`, `zlt0p7_4x`, `lowpt_2x`),
   `scripts/studies/flatphi_genreweight_z005_20260925/`,
   `scripts/studies/phi_reweight_z005_20260707/`,
   `scripts/studies/ptmax_scan_z005_2026-10-08/`, `scripts/omnifold/extract_omnifold_arrays.py`,
   and the parent `tunfold_official_v4_percharge/build_migration_th2_per_charge_v4.py`
   (+ LabAcceptance / RestrictedKinematics / ESigmaPhotonRecoFrame variants).
9. **Empz / HFS cone (stage 1): NOT redundant — must stay separate.** It operates on
   a different object (HAT `PartCand` HFS sums, not ChargedTot rows), at a different
   stage (stage-1 event envelope 30<Empz<75 + hadronic/eSigma calibration), with a
   different metric (η–φ distance vs 3D angle). A track-level α cone at stage 2 or
   in python does not substitute for it; removing it would change event-level Empz
   and the calibrated kinematics. The two cones would coexist, as they already do in
   the alpha0p2 chain (which runs on stage-1-processed trees).
10. **Control histograms / counters / QA:** stage-1 cut counters (`nEmpzCut`, …),
    stage-2 per-event/track counters (`nTracks`, `nChargedReco`, reco-pass/gen-only
    counts), the data-vector counters (`n_tracks_in_ps`), the alpha0p2 chain's alpha
    counters, per-file QA JSONs, QA/control-plot packages reading ChargedTot under
    the central fiducial, and the data-vs-MC comparison packages + canonical
    plotters (consume release tables → regenerate with the new release).
11. **Background/diagnostic templates whose conventions are selection-coupled:**
    alpha0p2 e/V0/det.int. reco-background vectors and the AziAsym alpha0p2 chains
    (`AziAsym/scripts/ibu_trento_phi_*_alpha0p2_v6syst`) would become
    convention-matched to the new central selection; the low-Q2 template
    (alpha0p1-cone) and the DIFFVM central-variant vector would still be
    selection-mismatched and need re-derivation; the ledger's PROVISIONAL no-cone
    rescale items (e ≈ 4.1441% etc.) would be moot; the det.int. parentage/S2′
    removable fraction (0.909920, measured under the central no-cone selection per
    the parentage design doc) is selection-conditional and would need
    re-measurement. The P-ALPHA validation object itself changes.
12. **BSA fork (v6bsa)** — raw beam-charge-asymmetry products were computed on the
    no-cone selection (658 files, 7,538,865 entries, entry-count parity with
    central); a central cone adoption requires re-forking/re-validating the BSA
    chain (its method is acceptance-free, but the observable sample changes).
13. **C_rad chain (gen-only selection on HAT: GenEmpz>45, xFGen<0, gen-only mode
    that skips the hadronic calculators entirely)** — NOT a track-selection
    consumer; the tables are structurally unaffected (per-gen-bin application on
    the response gen space, which a reco-side cone does not reshape), though Born
    values inherit the raw-release shift through the parent.
14. **All existing v6 central trees and derived releases** — comparability break
    under Option B (content change); under Option A the trees stay valid and only
    downstream products are re-derived (the recommended path).

---

## 6. Method note and limitations

- All quoted line numbers were read directly from the on-disk sources on
  2026-10-11; the two response-builder files most recently modified
  (2026-09-09/2026-10-09) postdate the 2026-06-10 release build, so for
  release-exact provenance the release's own script snapshots (e.g. the S2′ release
  `_run/` + `metadata.json`, and `scripts/canonical_highQ2_sidis_2026-06-30/`)
  should be diffed before any re-production. The S2′ release `metadata.json` was
  inspected and confirms its data vector is the central
  `data_vector_tunfold_official_v4_finalcandidate_...json` (no-cone) and its
  responses are the variant merges.
- No empirical ROOT-file check was run in this audit (read-only, and the flag/counter
  facts were already empirically established in the 2026-10-07 ledger §4 and its
  sidecar JSONs, which this audit cross-verified against code).
- The alpha0p2 application audit (2026-10-06) rows claiming "APPLIED (upstream tree
  level)" are superseded per §1; any consumer of that document should be pointed at
  this audit and the 2026-10-07 ledger.

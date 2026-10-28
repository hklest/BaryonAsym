# Plan — Alpha-cone (α ≥ 0.2 rad) imposition on the central v6/z005 S2′ chain (candidate build, gates, and post-promotion cascade)

**Plan date:** 2026-10-11. **Status:** executable specification; NOT executed. No production is launched by this plan.
**Scope:** high-Q2 charged-hadron SIDIS, v6/z005, 490 published bins (245 h+ + 245 h−), no species PID.
**Deliverable (first pass):** a dated CANDIDATE release — cone-coherent data vector + S2′-variant cone response + IBU k=4 / BBB — validated by an adapted G-gate suite + 1000-toy stat sidecar + critic review. **Central swap, Born release, and full budget regeneration happen ONLY after a separate user promotion decision (Phase 5).**

## 0. Decision record (user, 2026-10-11)

Impose the track-level `alpha >= 0.2 rad` scattered-electron cone cut on the CENTRAL baseline chain. The cone is the laboratory-frame opening angle between a reconstructed track and the scattered electron, computed downstream from stored branches `Px, Py, Pz` and `scatEX, scatEY, scatEZ` (arccos of the dot product over the product of magnitudes). This REVERSES, by explicit user decision, the alpha-cone entry of the S2' builder's DO-NOT-PORT list (`build_migration_th2_per_charge_s2prime.py` lines 68–76: "the alpha cone (prototype `alpha_ok` / `ELECTRON_ALPHA_VETO_RAD`) — central is no-cone"). The other two DO-NOT-PORT entries (e/V0 truth-veto disablement; miss-row fill for reclassified det.int. rows) REMAIN in force.

Scope decisions already taken (do not re-open):

- **Base lineage:** the CURRENT canonical RAW release, the S2′ detint-fake variant `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`. The cone is an additional observable-basis change on top of S2′. The S2′ response-internal det.int. removal (enlarged per-reco-bin `f_i`, removable fraction `0.909920 ± 0.000269`, charged-K DIF `k_parented` keeper) STAYS in the build.
- **First pass = candidate only:** dated candidate under `output_v6/central/data_release_candidates/`, never overwrite installed releases; G-gate-style suite + critic verdict before any central swap; full systematics regeneration only post-promotion.
- **NO tree reprocessing:** the cone is computed at aggregation from branches already in the v6 trees. Stage-1 (`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`) and Stage-2 (`src/ChargedSidisTreeBuilderHighQ2_v6.C`) builders are UNTOUCHED. The 658 central data trees and the central MC trees (RAPGAP31 364 files, DJANGOH14 501 files) are reused read-only.
- **Cone applies to RECO legs only** (data vector + MC reco legs of the response). GEN side unchanged. Gen-level releasebin C_rad tables are invariant (reco-only cut): applied primary `data/radiative/djangoh14_crad_releasebin_2026-10-09/`, formal alternative `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/`.
- **The e/V0 truth vetoes stay** (`not_e`, `not_v0_match_top`): the cone response is the S2′ variant response conditioned by the cone — it is NOT the allfake response; do not port allfake category routing.

## 1. Requirements Summary

### Confirmed requirements (user decision + workspace state)

1. Impose `alpha >= 0.2 rad` (lab frame, reco-only) on the central chain's track selection: data vector AND response reco legs, cone-coherent by construction.
2. Cone formula and reference implementation (authoritative):
   - `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py` line 113 (`ELECTRON_ALPHA_VETO_RAD = 0.2`), lines 363–371 (`compute_reco_electron_alpha`), lines 673–681 (branch-based computation and veto).
   - `H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py` line 386 (veto constant; formula lines 390–396), lines 690–692 (selection integration), lines 1191–1201 (metadata counters).
3. Build a dated CANDIDATE release (cone data vector + cone S2′ response + IBU k=4/BBB, coherent `body+miss` prior from the variant response itself, `L_data=351000.0 nb^-1`), run a G-gate-style validation suite + 1000-toy stat sidecar + critic review. Promotion/swap/Born/budget = Phase 5, post-promotion only.
4. z005 contract: 490 published bins = 245 h+ + 245 h−; response shape (491,246) per charge; data vector length 980 (charge-major: 0..489 h+, 490..979 h−); IBU k=4; binmap `binmap_reco_to_gen == repeat(arange(245),2)`.
5. Quoting rule EXTENDED again: every released-value quote must state (a) prior basis (coherent `body+miss`), (b) Born-vs-raw level, (c) S2′ observable basis, AND (d) **cone status** (α≥0.2 cone vs no-cone). Cross-basis quotes without the cone flag are FORBIDDEN, pre- and post-promotion.

### Inferred assumptions (verified against artifacts; re-assert at Phase 0)

- The cone is aggregation-computable from stored branches on BOTH data and MC trees; the parity measurement `alpha_pre = 3,902,480 = central n_tracks_in_ps` proves no tree re-production is needed (`docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md` §1).
- The cone-applied central data vector already exists in validated form (the alpha0p2 campaign vector, same 658 trees, truth vetoes inert on data): in-grid `3,865,073` (h+ `1,965,700` / h− `1,899,373`). The rebuild must reproduce it EXACTLY (gate C1).
- The S2′ construction (classifier, keeper, reroute, miss-fill suppression, category vectors) ports unchanged onto the cone-conditioned reco space; the removable fraction is mother-lineage-based and cone-independent (parentage cone survival ≈0.9984).
- No H1OO compilation is needed anywhere in this plan (Python aggregation scripts only). If any tree-level change appears to be required, STOP: that is a scope change requiring the user + `h1-builder-engineer`.

### Acceptance criteria (headline; full gate definitions in §3–§7)

- Data vector: exact reproduction of the cone-applied totals (in-grid 3,865,073 / h+ 1,965,700 / h− 1,899,373; in-PS 3,873,898; `alpha_pre` 3,902,480; in-PS rejected 28,582; in-grid removal 19,024 = 0.4897%), per-bin identity with the existing alpha0p2 vector (980/980 exact), per-bin monotonicity vs the central no-cone vector (delta ≥ 0 everywhere, sum = 19,024), 0/980 negative inputs.
- Response: truth conservation exact 245/245 both charges; per-cell body / per-reco-bin fake decomposition vs the no-cone S2′ variant exact via in-cone diagnostics; aggregate MC in-cone loss consistent with the measured predictions (0.4259% RG / 0.4524% DJ of in-PS reco tracks, ≈75% matched-e share); `f_i` integrated shift inside a pre-registered band (predicted ≲0.2 pp).
- Candidate release: 490 rows, 0 negative / 0 non-finite cross sections (IBU and BBB), k=4 recorded, coherent prior bit-asserted from the cone variant response, R10-style guards (no central-response/no-cone-vector fallback), integrated unfolded shift inside a pre-registered band with direction/z-concentration diagnostics.
- Suite verdict: all carried/adapted gates + cone gates PASS; critic verdict recorded; promotion remains a separate user decision.

## 2. Authoritative inputs (read before execution; cite in all outputs)

| Document / artifact | Path | Role |
|---|---|---|
| State + 10-item remaining-work ledger | `sidis_highQ2/STATE.md` (esp. "2026-10-10 S2′ detint-fake Promotion Override" section) | current canonical basis, campaign cluster precedents, carried blockers |
| S2′ promotion record | `sidis_highQ2/docs/promotions/detints2prime_promotion_2026-10-10.md` | promotion mechanics, install discipline, quoting rule, remaining-work ledger |
| Cone impact study (methodological backbone) | `sidis_highQ2/docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md` | measured magnitudes (§2), rebuild chain (§5.1), reuse/re-derive split (§5.2); its §6 recommendation is SUPERSEDED by the user decision — the §5 chain is executed as a selection-convention change |
| Chain plan (cone coherence by design) | `sidis_highQ2/docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` | §2 selection/binning enumeration, §3 branch names, Appendix A verified numbers; BLK-1 was exactly this defect class |
| S2′ variant builder | `H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py` | clone basis; DO-NOT-PORT lines 68–76; `pass_reco_full` line 867; `pass_pre_z` line 916; classifier/keeper lines 845–860 |
| S2′ release driver (stub) | `H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/run_bbb_ibu_release_s2prime_stub.py` | driver clone basis; R10 guard block lines 79–87, 183–222; prior construction lines 238–272 |
| S2′ gate suite report | `H1ChargedSidis_HighQ2/output_v6/studies/detint_variant_s2prime_2026-10-09/validation/gate_suite_G1_G13_report_2026-10-10.md` | gate definitions adapted in §6; G10 criteria-corrections lesson (pre-registration discipline) |
| Alpha0p2 reference script | `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py` | cone formula lines 363–371; constant line 113; veto lines 673–681 |
| Alpha0p2 v4 migration builder (cone precedent) | `H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py` | cone selection integration lines 690–692; counters lines 1191–1201 |
| Central data vector (no-cone reference) | `H1ChargedSidis_HighQ2/output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/data_vector_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json` | in-grid 3,884,097 (h+ 1,975,301 / h− 1,908,796); in-PS 3,902,480 |
| Alpha0p2 data vector (cone reference) | `H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/data_vector_alpha0p2_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json` | in-grid 3,865,073; schema `data_vector_v4_native_fine_reco_raw_no_bgsub` v1.1; 658 files / 0 failed |
| Central data-vector builder | `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_data_vector_v4.py` | clone basis for Phase 1 (already loads `Px,Py,Pz,scatEX,scatEY,scatEZ,scatEE`) |
| S2′ campaign structure | `H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/{condor,shard_lists}/`, `output_v6/studies/detint_variant_s2prime_2026-10-09/` | shard/Condor/merge/smoke/validation layout to mirror |
| Selection text (becomes TRUE post-promotion) | `sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/selection.tex:122,129-133` | per-track α ≥ 0.2 requirement; update only post-promotion (Phase 5) |

Key measured magnitudes (validated; do not re-derive — re-assert at the gates):

- Data: in-grid removal 19,024 (3,884,097 → 3,865,073, −0.4897%); in-PS 28,582 (3,902,480 → 3,873,898, −0.7325%); worst per-bin removal ≈36.6% (measured under α<0.1 in global bin 24; α<0.2 is ≥ this).
- MC in-cone (count level): aggregate 0.4259% RG (287,277/67,440,033) / 0.4524% DJ (364,520/80,569,547); matched-e share of cone-rejected MC 75.1% both generators; true π/K/p loss 0.10649% (RG) / 0.11354% (DJ); matched e± removal 5.07632% (RG) / 5.26673% (DJ); det.int. class cone survival ≈0.9984.
- `f_i` predicted shift ≲0.1–0.2 pp response-internal (ESTIMATE, no in-cone fake counter exists; band pinned at Phase 2/4).
- S2′ no-cone release values (shift reference, cone-status-flagged): integrated IBU h+ `11.446174 nb` / h− `11.158554 nb`; response-internal RG `f_i` `11.2403%`/`10.8737%` (data-weighted `11.3577%`/`10.9768%`).
- `L_MC`: RAPGAP31 `9,132,891.0 nb^-1`; DJANGOH14 `10,487,495.0 nb^-1` (re-verify at execution).

## 3. Campaign naming, directory layout, and product discipline

Dated campaign tag: `alphaveto_cone_s2prime_<LAUNCH-DATE>` (nominal `2026-10-11`; use the actual launch date in every dated name).

```
H1ChargedSidis_HighQ2/scripts/unfolding/alphaveto_cone_s2prime_<date>/
  build_data_vector_v4_alphaveto.py                    # Phase 1 builder (dated clone + cone port)
  build_migration_th2_per_charge_s2prime_alphaveto.py  # Phase 2 builder (dated clone + cone port)
  merge_migration_per_charge_s2prime_alphaveto.py      # dated clone of the S2' merge (logic unchanged)
  check_merged_s2prime_alphaveto.py                    # dated clone of check_merged_s2prime.py
  validate_smoke_s2prime_alphaveto.py                  # dated clone of validate_smoke_s2prime.py
  make_campaign_files.py                               # dated clone; regenerates shard lists + condor files
  run_bbb_ibu_release_s2prime_alphaveto.py             # Phase 3 driver (dated clone, repointed + guarded)
  acceptance_register.md (+ acceptance_register.json)  # Phase 0 pre-registered gate criteria
  condor/    (*.sub, *.args, run_*.sh, campaign_manifest.json)
  shard_lists/{rapgap31,djangoh14}/

H1ChargedSidis_HighQ2/output_v6/studies/alphaveto_cone_s2prime_<date>/
  smoke/          # one data file + one MC file smoke JSONs (Phase 0)
  data_vector/    # per-shard JSONs + merged cone data vector JSON (Phase 1)
  shards/         # 174 shard ROOTs (Phase 2)
  merged/         # response_rapgap31_..._alphaveto.root, response_djangoh14_..._alphaveto.root
  validation/     # gate-suite report + JSON twin + results/ + toys/ + shard_audit/ (Phase 4)
  stat_sidecar_campaign_<date>/   # 1000-toy sidecar campaign (Phase 4)
  release_candidate/bbb_ibu_detints2prime_alphaveto_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<date>/  (Phase 3)
```

After all Phase 4 gates PASS: checksum-verified copy (`rsync -a`, no `--delete`) of the release candidate (incl. the installed 1000-toy sidecar) into `H1ChargedSidis_HighQ2/output_v6/central/data_release_candidates/bbb_ibu_detints2prime_alphaveto_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<date>/`, with an install-side manifest of sha256s. The study-tree source remains intact as provenance. **No install into `output_v6/central/data_release/` happens in this plan** — that is the separate promotion step.

Variant identifiers (recorded in every product's metadata and asserted by guards):

- Response variant id: `detint_fake_response_variant_S2prime_alphaveto_<date>`.
- Data-vector label: `raw_data_no_bgsub_alphaveto_cone` with contract fields `electron_alpha_veto_rad = 0.2`, `alpha_reco_only = true`, and the central truth-veto contract string retained (inert on data; see trap T9).

## 4. Phase 0 — Evidence lock-in (login node only; single-file checks; ≤5 min each)

**Owner:** h1-script-runner (execution), h1-sidis-supervisor (coordination). No Condor yet.

1. **Branch/formula verification, one data tree.** Pick one data tree (suggest `output_v6/central/sidistree/data/ep0607/` first file). Read `ChargedTot` branches `Px, Py, Pz, scatEX, scatEY, scatEZ` (already in the central data-vector builder's required branch set, `build_data_vector_v4.py` lines 271–272). Compute `alpha` with the reference formula; assert: fraction of non-finite alpha ≈ 0 (guard behavior on zero-magnitude momenta), alpha distribution smooth in [0, π], and the arccos argument clipping active only at machine precision.
2. **Branch/formula verification, one MC tree.** Same check on one RAPGAP31 central MC tree (first entry of `shard_lists/rapgap31/rapgap31_0000.txt` from the S2′ campaign, reused read-only as the path source). Additionally assert the truth-class composition of the in-cone population on this file is qualitatively consistent with the measured aggregate (≈75% matched-e of cone-rejected MC; exact fractions vary per file — the assertion is a sanity window, not the campaign gate).
3. **Per-file cone-rate smoke (data).** Run the dated cone data-vector builder (Phase 1 script, written before this step) on the SAME single data file, and in parallel the alpha0p2 copied builder `electron_cone_alpha0p2_z005_20260623/copied_scripts/build_data_vector_v4.py` on the same file. Assert: (a) `alpha_pre` per-file parity with the no-cone selection (exact), (b) per-file in-PS rejection fraction consistent with the campaign mean 0.7325% (pre-registered per-file window in the acceptance register; suggested [0.3%, 1.5%] for a mid-size file — the register fixes the exact window), (c) per-bin counts IDENTICAL to the alpha0p2 builder output for this file (both implement the same effective data selection; deterministic counting → exact match; any mismatch = builder drift, STOP).
4. **Per-file smoke (MC).** Run the dated cone S2′ builder on one MC file. Assert: variant markers present; cone counters non-zero and plausible; det.int./keeper/reroute counters consistent with the S2′ smoke precedent (`output_v6/studies/detint_variant_s2prime_2026-10-09/smoke/`); body+fake+miss conservation on the file; in-cone diagnostic histograms populated; rerouted-key collision watchdog behaves as in S2′ (collisions expected >0, gate no fill).
5. **Acceptance register (the deliverable of Phase 0).** Write `acceptance_register.{md,json}` in the campaign scripts dir: every gate of §6 with its exact pass criterion, the per-file smoke windows, the f_i band-pinning procedure (C4), and the integrated-shift band-setting procedure (C5) — all PRE-REGISTERED before any Condor submission. Lesson applied: the S2′ G10 criteria were modified in-session and had to be documented in an addendum (`gate_suite_G1_G13_report_2026-10-10.md` "G10 criteria-corrections addendum"); any in-session modification in this campaign MUST be appended the same way (initial form / realized form / reason), never silently.
6. **Input inventories.** Freeze the 658-file data list (must equal the alpha0p2 campaign's input list; the vector JSON records it) and the MC lists (RG 364 / DJ 501; reuse the S2′ shard lists as the path source, regenerated by the dated `make_campaign_files.py` into the new campaign dir). Record sha256 of every consumed reference artifact (central vector, alpha0p2 vector, S2′ merged responses, S2′ release metadata) into the register.

**Phase 0 exit:** register complete; both smokes pass; supervisor sign-off recorded. Only then may Phase 1/2 Condor campaigns be submitted.

## 5. Phase 1 — Cone-coherent central data vector (980 bins)

**Owner:** h1-script-runner (Condor campaign), h1-sidis-supervisor.

**Script basis:** dated clone `build_data_vector_v4_alphaveto.py` from `tunfold_official_v4_finalcandidate_.../build_data_vector_v4.py` (the CENTRAL builder — not the alpha0p2 copied builder, to keep central provenance), plus the cone port from the alpha0p2 reference. The port is EXACTLY:

```python
ELECTRON_ALPHA_VETO_RAD = 0.2   # produce_reco_background_vector_alpha0p2_v2.py:113
ALPHA_RECO_ONLY = True

def _compute_reco_electron_alpha(Px, Py, Pz, sEX, sEY, sEZ):
    """3D opening angle between reco track and scattered-electron direction.
    VERBATIM from electron_cone_alpha0p2_z005_20260623/copied_scripts/
    build_migration_th2_per_charge_v4.py:390-397 (= alpha0p2_v2.py:363-371)."""
    trk_norm = np.sqrt(Px*Px + Py*Py + Pz*Pz)
    ele_norm = np.sqrt(sEX*sEX + sEY*sEY + sEZ*sEZ)
    dot = Px*sEX + Py*sEY + Pz*sEZ
    cos_alpha = np.where((trk_norm > 0) & (ele_norm > 0), dot / (trk_norm * ele_norm), np.nan)
    cos_alpha = np.clip(cos_alpha, -1.0, 1.0)
    return np.arccos(cos_alpha)

# in the per-file loop, after the existing reco-quality selection `reco_quality`
# and before the pass assembly (alpha0p2_v2.py:673-681 pattern):
alpha = _compute_reco_electron_alpha(
    batch["Px"].astype(np.float64), batch["Py"].astype(np.float64),
    batch["Pz"].astype(np.float64),
    batch["scatEX"].astype(np.float64), batch["scatEY"].astype(np.float64),
    batch["scatEZ"].astype(np.float64))
alpha_ok = np.isfinite(alpha) & (alpha >= ELECTRON_ALPHA_VETO_RAD)
pass_prebin = evt_fid & evt_cleanup & reco_quality & truth_veto & alpha_ok
```

with per-charge counters `n_alpha_pre / n_alpha_after / n_alpha_rejected` and metadata fields mirroring `build_migration_th2_per_charge_v4.py:1191-1201` (`electron_alpha_veto_rad`, `alpha_reco_only`, `alpha_pre/after/rejected_{pos,neg}`). Keep the CENTRAL truth-veto contract string (`IsMatchedElectron==0`, `IsMatchedV0Daughter==0` — inert on data, sentinel zeros); do NOT inherit the alpha0p2 vector's `disable_truth_electron_veto_for_alpha_central` MC-convention documentation (trap T9). Do not port the matched-electron sentinel-counter convention blindly: the new counters must be real counts (trap T11).

**Execution:** Condor mandatory (658 files). Structure: ~10 files/shard → 66 shards, per-shard JSON, single merge to `data_vector/data_vector_tunfold_official_v4_finalcandidate_..._alphaveto.json` (keep the schema `data_vector_v4_native_fine_reco_raw_no_bgsub` v1.1, label `raw_data_no_bgsub_alphaveto_cone`, `bg_subtracted=false`, `n_subtracted ≡ n_raw`). Precedents: the June alpha0p2 campaign (`wrap_build_data_vector_v4.sh` + `submit_build_data_vector_v4.condor` in `electron_cone_alpha0p2_z005_20260623/copied_scripts/`) and the 2026-10-09 qT/Q data-vector campaign (clusters 5435157 build / 5435246 merge).

**Validation (gate C1 + G-inputs):**

- C1(a) totals: in-grid `3,865,073` (h+ `1,965,700` / h− `1,899,373`); in-PS `3,873,898`; `alpha_pre` `3,902,480` (== central in-PS, exact parity); in-PS rejected `28,582`; in-grid removal `19,024` = −0.4897%.
- C1(b) per-bin identity: `n_raw` equal to the alpha0p2 vector's `per_bin` in 980/980 bins (exact; deterministic counting on the same trees and effective selection). Any mismatch → STOP and diagnose convention drift (enumerate: z/pTh pion-mass convention, binning version, file list).
- C1(c) monotonicity vs the central no-cone vector: per-bin delta ≥ 0 in all 980 bins; `sum(delta) = 19,024`; per-bin relative removal consistent with the measured hot-bin structure (worst measured data-removal bin = global bin 24 = h+ S0x0, z∈[0.2,0.3), pT∈[1.0,1.5), ≈36.6% under α<0.1; direct central-vs-alpha0p2 per-bin check: worst 58.6% in h+ reco bin 49; NOT a low-z/low-pT concentration; report the top-10 bins).
- 0/980 negative inputs; schema/label/contract fields present; sha256 of the merged vector recorded.

## 6. Phase 2 — Cone-coherent S2′-variant response (Condor, 174 shards)

**Owner:** h1-script-runner (campaign), h1-sidis-supervisor; h1-builder-engineer NOT involved (no tree builder change; Python only).

**Script basis:** dated clone `build_migration_th2_per_charge_s2prime_alphaveto.py` from `build_migration_th2_per_charge_s2prime.py`, with EXACTLY ONE additional physics delta on top of the S2′ delta list — the cone port — plus the new cone diagnostics. The S2′ delta (classifier, keeper, reroute, miss-fill suppression, category vectors, per-reco-bin rerouted-weight diagnostics, `--dump-rerouted-legs`, shard-list I/O) is preserved VERBATIM. The header must document the DO-NOT-PORT reversal: entry 2 (alpha cone) is REVERSED by user decision 2026-10-11; entries 1 (e/V0 truth-veto disablement) and 3 (miss-row fill for reclassified det.int. rows) REMAIN in force.

**Cone insertion point** (minimal-delta, mirroring the alpha0p2 v4 builder's `pass_pre_alpha → pass_reco_full` pattern at its lines 690–692). In the S2′ builder, replace the selection assembly at line 867 (`pass_reco_full = evt & trk & not_e & not_v0_match_top`) and line 916 (`pass_pre_z = ...`) with:

```python
# ---- CONE PORT (user decision 2026-10-11; REVERSES the S2' DO-NOT-PORT
# entry "the alpha cone ... central is no-cone". Formula VERBATIM from the
# alpha0p2 v4 builder lines 390-397 (= alpha0p2_v2.py:363-371); constant
# line 386 / alpha0p2_v2.py:113. Reco legs ONLY; gen side unchanged. ----
ELECTRON_ALPHA_VETO_RAD = 0.2
alpha = _compute_reco_electron_alpha(
    batch["Px"].astype(float), batch["Py"].astype(float),
    batch["Pz"].astype(float),
    batch["scatEX"].astype(float), batch["scatEY"].astype(float),
    batch["scatEZ"].astype(float))
alpha_ok = np.isfinite(alpha) & (alpha >= ELECTRON_ALPHA_VETO_RAD)

pass_pre_alpha = evt & trk & not_e & not_v0_match_top      # == S2' pass_reco_full, pre-cone
pass_reco_full = pass_pre_alpha & alpha_ok                 # cone conditions ALL reco fills
# pass_pre_z (det.int. counters) stays PRE-cone: pass_pre_z = pass_pre_alpha
# (parentage-campaign-comparable); NEW cone-conditioned det.int. counters added below.
```

The z-guard then applies as before (`pass_reco_full = pass_reco_full & (~z_outlier)`). Everything downstream (matched fill + `matched_gen_ok` gate, fake fill, reroute of non-keeper det.int. legs into `R[ir, GEN_OVERFLOW]` with `w_reco = w1*w2*NucliaWeight`, consumed-key recording, miss pass) operates unchanged on the cone-conditioned `pass_reco_full`. Mechanical consequences (must be understood, they are correct reco-cut semantics):

- In-cone legs fill NOTHING (no body, no fake, no reroute); their gen partners (for previously matched legs) become unconsumed → the MISS ROW GAINS content vs the no-cone S2′ response. The S2′ G1 assertion "miss bit-identical to central" is FALSE on the cone basis and is REDEFINED (see §9, G1′).
- The `k_parented` keeper and enlarged `f_i` construction are preserved: the removable class is rerouted within the surviving (out-of-cone) reco space; in-cone non-keeper det.int. legs (≈0.16% of the class, survival ≈0.9984) drop out entirely — numerator and denominator both lose them.
- Truth conservation still holds exactly BY CONSTRUCTION: `body + miss` == the (unchanged, gen-side) CTG pool marginal, 245/245 per charge.

**New diagnostics (required, not optional):** per charge, per fine reco bin (NB_R=490) and per cell where applicable: `n_alpha_pre/after/rejected` (+ matched-e sub-counter, real counts), in-cone weight broken down by class (body-matched / pre-existing fake / rerouted non-keeper det.int. fc3 / pre-existing fake non-keeper fc4 / keeper fake fc5 / unflagged fc0), in-cone leg counts, det.int. selection counters pre-cone (existing) and post-cone (new). Add `--dump-cone-removed-legs` (per-leg TTree: class, reco bin, gen bin, weight, run, event) mirroring the S2′ `--dump-rerouted-legs` precedent, for the exact per-cell audit behind G1′/C3.

**Execution:** mirror the S2′ campaign exactly — 5 files/shard; RAPGAP31 364 files → 73 shards; DJANGOH14 501 files → 101 shards (last shard 1 file); 174 shards total; per-shard ROOTs under `studies/alphaveto_cone_s2prime_<date>/shards/`; then the dated merge to `merged/response_{rapgap31,djangoh14}_detint_s2prime_alphaveto.root`. Request **24–32 GB per shard** (trap T1). Condor files regenerated by the dated `make_campaign_files.py` (`.sub`, `.args`, `run_shard_*.sh`, `merge` and `checks` submissions), with `campaign_manifest.json` recording builder sha256, shard lists, and a `submitted` flag (trap T2). S2′ cluster precedents for scale: response build 5469609/5469610; merge 5473512/5473607.

**Duplicate guardrails (trap T2):** unique-shard-set verification at merge (174/174 distinct outputs, sha256 manifest of shard ROOTs); no re-submission of an already-completed shard list (manifest flag); watch for concurrent duplicate clusters (DIFFVM precedent 5385054/5385055 — if a duplicate appears, keep ONE validated product and record the adjudication); shard sums must reproduce merged totals exactly (S2′ precedent: 73/73 RG + 101/101 DJ shard audit PASS).

**Validation inputs produced here (gates C3, C4, C7):**

- C3 per-cell decomposition: for each charge and reco bin (and cell for body): `body^nocone-S2' = body^cone + in-cone body weight` and `fake^nocone-S2' = fake^cone + in-cone fake weight`, exact, using the S2′ merged responses (read-only) and the new in-cone diagnostics. Rerouted fc3 decomposition included.
- C3 aggregate: total in-cone MC content vs the measured predictions (0.4259% RG / 0.4524% DJ of in-PS reco tracks; ≈75.1% matched-e share) — consistency window pre-registered in the acceptance register (per-generator, count level, from `.mc_alpha_counts` provenance in the cone impact study §2).
- C4 f_i band: response-internal integrated `f_i` shift vs the S2′ values (`11.2403%`/`10.8737%` RG) within the pre-registered band (predicted ≲0.2 pp, sign undetermined; the band is PINNED from a merged-shard sample at Phase 2 and recorded in the register before the full merge is unsealed for release use).
- C7 shard integrity: the duplicate-guardrail checks above.
- G13 input: per-bin removable fractions vs the parentage map reference (reusable; mother-lineage-based, cone-independent; integrated within spec refs `0.906412` RG / `0.912611` DJ, pooled `0.909920 ± 0.000269`).

## 7. Phase 3 — IBU k=4 + BBB candidate release (cone basis)

**Owner:** h1-script-runner.

**Driver:** dated clone `run_bbb_ibu_release_s2prime_alphaveto.py` from `run_bbb_ibu_release_s2prime_stub.py` with EXACTLY these repoints/guards (nothing else):

1. `VARIANT_ID_EXPECTED = "detint_fake_response_variant_S2prime_alphaveto_<date>"`; `VARIANT_RESPONSES` → the two merged cone response ROOTs; R10 preflight unchanged (variant marker, mc label, positive shard count 73/101, binning version) — no code path may read a central or no-cone response or prior (fallback IMPOSSIBLE, as in S2′).
2. **NEW data-vector guard (R10-style, this is the top mechanical risk — trap T8):** the S2′ stub consumed the CENTRAL no-cone data vector (`response_swap.data_vector: "central, UNCHANGED"`). The cone driver MUST consume the Phase 1 cone vector and MUST refuse to run unless the vector's label == `raw_data_no_bgsub_alphaveto_cone`, `electron_alpha_veto_rad == 0.2`, and the in-grid total == `3,865,073`. A silent stale-vector unfold is the failure mode this guard exists to make impossible.
3. Coherent prior := `body+miss` computed FROM the cone variant rapgap31 response (bit-asserted against the consumed matrix, as in S2′ `variant_M_ext_and_prior`); DJ leg keeps its stored `h_prior_gen` convention (report-only caveat as in S2′ G4/G6).
4. Constants: `LUMI = 351000.0`, `KSEL = 4`, iterations 1–10, binmap central UNCHANGED, shapes (491,246) per charge, data vector length 980.
5. Caveat/CAVEAT strings: extended quoting rule (prior basis + Born-vs-raw + S2′ observable basis + **cone status**); PROVISIONAL CANDIDATE labeling; pointer to this plan and the acceptance register.

**Candidate assembly:** output to `studies/alphaveto_cone_s2prime_<date>/release_candidate/bbb_ibu_detints2prime_alphaveto_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<date>/` following the S2′ release layout (`ibu/`, `bbb/`, `bin_map.{json,csv}`, `covariance/`, `manifest.json`, `metadata.json`, `summary_totals.json`, `plots/`, `_run/`). RAW ONLY — no C_rad attach, no Born products in the candidate.

**Acceptance (gates C5, C6 + G-carries):**

- C6: truth conservation exact 245/245 both charges; coherent prior bit-asserted; independent IBU implementation agreement ≤1e-12 (G3 form; S2′ precedent 4.15e-16/4.76e-16).
- G8 form: 0/980 negative reco inputs (the cone vector), 0/490 negative and 0 non-finite cross sections (IBU and BBB), eff in (0,1] in populated bins, 0 background-exceeds-data.
- G4 form: BBB-IBU integrated consistency within the S2′ precedent range (S2′: +0.078% h+ / +0.102% h− vs central +0.113%/+0.111%); the central BBB reproduction (11.496351/11.202754 nb) remains the untouched machinery check.
- **C5 integrated-shift gate — band defined by procedure, NOT invented here.** Before the candidate IBU run is accepted, pre-register in the acceptance register: (i) the measured per-bin data-side removal Δd (Phase 1, sum 19,024); (ii) the measured response-side changes (per-bin Δf_i, per-cell in-cone body loss, efficiency change) from the Phase 2 merged response; (iii) a first-order algebraic propagation of (i)+(ii) through the response to a predicted per-bin and integrated shift, documented explicitly in the register; (iv) the band = the algebraic center ± a width fixed in the register (rationale required, mirroring the S2′ G10 backstop logic: ~2× margin over the largest legitimate physics scale in the decomposition, catastrophic modes far outside). The realized shift (candidate vs S2′ no-cone release `11.446174`/`11.158554 nb`, cone-status-flagged) must land inside the band, with mandatory diagnostics: direction (majority-down expected — removed content is net contamination + signal, only the modeled signal part is acceptance-recovered), z-concentration measured wherever it falls (no pre-registered low-z/low-pT expectation — the measured hot bins are global bin 24 = h+ S0x0, z∈[0.2,0.3), pT∈[1.0,1.5) and h+ reco bin 49 at 58.6%), worst-bin enumeration vs that measured hot-bin list. Established bounds for sanity: the integrated shift magnitude is bounded above by the 0.49% in-grid removal; order ~0.1–0.5% expected — the exact band comes from (iii)/(iv).

## 8. Phase 4 — G-gate validation suite, 1000-toy sidecar, critic dispatch

**Owners:** h1-script-runner (suite + toys), h1-qa-specialist (independent QA), h1-analysis-critic (verdict, per workspace precedent), h1-sidis-supervisor (dispatch + adjudication).

**Gate suite:** adapt the S2′ suite (scripts `validation/scripts/assemble_gate_report_s2prime.py` pattern; report + JSON twin + `results/gate_results_raw.json` + `toys/` + `shard_audit/`). Condor for the toy and shard-audit legs (S2′ precedent: main gates 5474773, G5 toys 5474271, shard audit 5474272).

| Gate | Status vs S2′ | Cone-basis definition |
|---|---|---|
| C1 | NEW | Data-vector cone parity (§5): exact totals + 980/980 per-bin identity vs alpha0p2 vector + monotonicity vs central vector + 0/980 negatives |
| C2 | NEW | Phase 0 cone formula/branch smoke (one data + one MC file; finite-alpha fraction; per-file rate windows) |
| C3 | NEW | Response cone-content decomposition: per-cell body / per-reco-bin fake identity vs the no-cone S2′ merged response via in-cone diagnostics (exact); aggregate in-cone loss vs measured MC predictions (0.4259%/0.4524%, ≈75% matched-e) within the pre-registered window |
| C4 | NEW | `f_i` integrated shift within the pre-registered band (≲0.2 pp predicted); per-bin `f_i` bounded by local in-cone content |
| C5 | NEW | Integrated unfolded shift within the pre-registered band; direction/z-concentration/worst-bin diagnostics (§7) |
| C7 | NEW | Shard/duplicate integrity: 174/174 unique shards, checksum manifest, no duplicate-cluster products consumed, shard sums == merged totals |
| G1′ | REDEFINED | The S2′ form (miss bit-identical to central; body diff == fc3) is FALSE on the cone basis (cone moves matched-leg gen partners to miss). New: (a) truth conservation, two-level: COUNTS exactly conserved vs the unchanged CTG pool per gen bin (245/245); WEIGHTS body+miss == the cone build's OWN `prior_gen_NWeff` (body legs w1·w2·NucliaWeight, miss legs w1·w2), with the no-cone→cone weight difference per gen bin = Σ in-cone matched w1·w2·(NucliaWeight−1) (auditable, ≈0) — WARNING: comparing weights against the PLAIN unchanged pool shows the ~0.45% NW effect and must NEVER be 'fixed' by suppressing miss growth (the single most dangerous misimplementation); (b) miss DELTA identity — `miss^cone − miss^nocone-S2'` == gen partners of cone-removed matched legs not consumed elsewhere, where the consumed-elsewhere set must be EXPLICITLY ENUMERATED per gen bin with expected composition = shared-partner multi-leg cases only (anything else = gate FAILURE), plus a ZERO-COLLISION assertion of rerouted-leg recorded keys against the CTG miss-candidate key set; verified via `--dump-cone-removed-legs` on a smoke shard + the merged consumed-key audit; (c) body/fake decomposition == C3; (d) shard audit 73/73 + 101/101 PASS |
| G2 | carries | (prior>0)&(eff>0) on 245/245 per charge/generator |
| G3 | carries | independent IBU agreement ≤1e-12 |
| G4 | carries | BBB-IBU consistency (S2′ precedent deltas); central BBB reproduction untouched |
| G5 | carries | 1000-toy Poisson closure per charge through the cone `f_i`/response/prior; covariance finite/symmetric/positive; nominal reproduces the main-gate IBU bit-consistently. WATCH: the S2′ neg leg sat exactly at the ≤4 window edge (\|z\|max 3.70, frac\|z\|<1 0.637 vs [0.593,0.772]) — re-verify and record; edge contact again = report, not auto-fail (per S2′ remaining-work item 5) |
| G6 | carries | DJANGOH14 variant structural audit clean; removed-block generator dependence quantified from per-generator category vectors (never naive \|DJ−RG\| on stored driver legs) |
| G7 | carries | refold chi2/ndf consistent with the production code precedent (S2′: 4.724/4.300); tolerance re-derived for the cone response, pre-registered |
| G8 | carries | 0/980 negative reco inputs (cone vector), 0/490 negative IBU/BBB cross sections, eff in (0,1], 0 background-exceeds-data; enumerate any `f_i=1.0` empty-data edge cases |
| G9′ | REDEFINED | S2′ identity `f_i_v = f_i_c + D_i/(B_c+F_c)` needs a cone-basis CENTRAL (pre-S2′) response that this campaign does NOT build (cost decision, §11-Q2). Replacement: (a) internal consistency `f_i = F_i/(B_i+F_i)` per reco bin (trivial-but-asserted); (b) the C3 decomposition identity vs the no-cone S2′ variant (exact); (c) C4 band; (d) G13 removable-map cross-check. If any of these fail, the contingency cone-central build triggers (§11-Q2) |
| G10′ | REDEFINED | The S2′ band [-0.36%,-0.27%] measured the DETINT removal and does NOT apply. Replacement = C5 (cone-effect shift, band pre-registered per §7). Pre-registration discipline mandatory; in-session criteria modifications documented as addenda with initial/realized forms |
| G11 | carries | det.int. category vectors (fc3/fc4/fc5/fc0) partition exactly on the cone basis; matched share consistent with the parentage 86/14 partition within the survival-0.9984 bound |
| G12 | carries | (a) precedent-form weight audit, 0 flagged bins expected; (b) fake-column MC-stat sumw2 (fc3) recorded for the successor budget re-base |
| G13 | carries | per-bin removable fractions and rerouted shape consistent with the parentage map (matched-share-corrected basis); integrated within spec refs (RG 0.906412 / DJ 0.912611, pooled 0.909920 ± 0.000269); cone-independence argued via the 0.9984 survival bound |

**1000-toy stat sidecar:** mirror the S2′ sidecar campaign (`stat_sidecar_campaign_<date>/`, precedent cluster 5481723): 1000 toys per charge through the cone response/`f_i`/prior; installed into the candidate as `stat_sidecar_1000toy/` (cross sections bit-identical to the candidate; sidecar replaces the driver's embedded 100-toy covariance, S2′/coherent-prior precedent).

**Critic dispatch criteria (all required before dispatch):** (a) every gate C1–C7 + G1′–G13′ recorded PASS with the JSON twin complete; (b) the acceptance register shows zero undocumented in-session criteria modifications (or documented addenda); (c) the candidate release + sidecar assembled with manifest/sha256 complete and the data-vector + R10 guards demonstrably active; (d) the known-caveat list embedded (low-Q2 α0p1 radius mismatch — §11-Q1; BSA fork divergence — §12-T6; sentinel-counter history; DJ stored-prior leg report-only status); (e) the extended quoting rule applied in every recorded number. The critic verdict (target: SOUND-WITH-CONDITIONS or better, conditions enumerated FIXABLE-NOW vs POST-PROMOTION) is the exit artifact of Phase 4. **Promotion itself remains a separate USER decision** — this plan ends the first pass at the critic verdict + the checksum-verified candidate in `data_release_candidates/`.

## 9. Phase 5 — Post-promotion only (gated on the user promotion decision)

**Owners:** h1-script-runner (productions), h1-plotting-naf (plots), h1-sidis-supervisor (cascade), note writer (analysis note, only on user request).

Execute in the S2′-promotion order (install → Born → budget → plots → docs):

1. **Install:** checksum-verified additive copy of the promoted candidate into `output_v6/central/data_release/` (S2′ mechanics: `rsync -a`, no `--delete`; sha256 manifest; install record JSON; precedent dirs bit-identical pre/post).
2. **Born release:** `sigma_Born(bin) = C_rad(bin) × Unfold(data; R_cone)` with the UNCHANGED gen-level tables (applied primary DJANGOH14 releasebin h+ `1.1415172920054786` / h− `1.1410367377289135`; formal alternative RAPGAP31 releasebin emfix h+ `1.1342370401351234` / h− `1.1336689310022947`; `crad_table_inheritance` sha-asserted). Gate: per-bin Born shift == per-bin raw shift (≤1e-12; the cone is reco-only, so the C_rad attach is exactly the S2′ Born mechanics on the new raw parent). S2′ precedent: cluster 5482794, 17/17 checks.
3. **Full budget regeneration (successor to the 15-component S2′ budget):** all components re-derived on the cone nominal, including — resolving S2′ remaining-work item 1 — the **shifted detector legs ×5 WITH the cone** (elec_energy, elec_theta, hfs_jes, hfs_rces, hfs_theta: shifted data vectors + shifted cone responses through the detint-fake+cone builder, restoring the full-chain convention), **tracking ensemble 16 seeds** with cone, **phi_h re-derivation** (variant-nominal convention), **B1 floors** (Option A `max(base, coherent B1 floor)`), `detint_removal_rel` and `fakecol_mcstat_rel` re-derived on the cone basis (G12(b) sumw2 inputs from Phase 4), `c_rad_gen_rel`/`c_rad_stat_rel` carried UNCHANGED (dev 0 asserted). Data-side-only convention gap vs central should CLOSE with the shifted-response restoration — verify and record.
4. **Plots (h1-plotting-naf):** raw-parent 7-panel/canonical products on the cone basis; Born-plot regeneration / CRAD_CANDIDATES registration is a SEPARATE recorded plotter-table decision (S2′ ledger item 8 discipline); 4-generator Born comparison regeneration now requires the cone Born release as data side + NORAD-builder releasebin repoint (S2′ ledger item 6, now triply superseded until regenerated).
5. **P-ALPHA residue re-scoping:** both critic template-heritage conditions discharge by construction (cone-applied e/V0 templates become same-selection products; V0 no-cone unbounded-transfer problem disappears); residual classes shrink to cone-applied magnitudes (e 3.8355% cone-applied; V0 3.4002%; low-Q2 0.0054% with the α0p1 caveat unless re-derived — §11-Q1). Write the formal P-ALPHA closure/supersession entry in STATE.md.
6. **Analysis-note updates (note writer only, on user request):** `sections/selection.tex:122,129-133` (per-track α ≥ 0.2 requirement) becomes TRUE — schedule post-promotion, never before; ride along the pending `results.tex`/`systematics.tex` bgsub and no-radiative-correction text fixes (S2′ ledger item 7).
7. **Side-study selection-mismatch flags:** BSA fork `v6bsa` is UNTOUCHED (raw, no cone) — flag as observable-basis divergence; any future BSA quote must state cone status; BSA re-derivation under the cone is a separate future decision. The qT/Q hybrid chain and the alpha0p2 diagnostics carry their own scope labels; the in-flight bgsub Set-R chain (separate chain, own critic gate) needs a further cone re-base if it is to live on the promoted basis — flagged as a dependency, NOT executed here.

## 10. Run-status / blocker ledger update checklist

During the campaign (per phase, by h1-sidis-supervisor): append run-status entries to `sidis_highQ2/docs/RUN_STATUS.md` (campaign clusters, gate status, candidate path); keep `sidis_highQ2/context-handoff.md` startup card current (campaign in flight, candidate-only, no central change). Never edit installed releases or their READMEs.

Post-promotion (Phase 5, documentation dispatch mirroring the S2′ 10-step checklist):

- [ ] `sidis_highQ2/STATE.md`: header + Release row + backgrounds/unfolding pipeline text + Superseded list + full "α-cone Promotion Override" section (campaign clusters, install record, values, extended quoting rule).
- [ ] `sidis_highQ2/docs/RUN_STATUS.md` + `docs/status/current-release.md` + `docs/status/alpha0p2-background.md` (P-ALPHA closure) + `docs/status/unfolding-tunfold.md` (cone variant builder as canonical response basis).
- [ ] `sidis_highQ2/context-handoff.md` startup card.
- [ ] Promotion record `sidis_highQ2/docs/promotions/alphaveto_cone_promotion_<date>.md` (citable standalone record — write it AT promotion time, not restored later).
- [ ] Script registries: both `RUNLIST.md` copies + both `SEVEN_CATEGORY_SCRIPT_INDEX.md` copies, byte-identical, with the cone campaign entries.
- [ ] Skills: `sidis-hq2-state`, `sidis-hq2-backgrounds` (cone adopted; P-ALPHA closed; alpha0p2 guidance re-scoped), `sidis-hq2-unfolding` (cone variant builder canonical; DO-NOT-PORT reversal recorded), `sidis-hq2-systematics`, `sidis-hq2-results` (four-part quoting rule), `sidis-hq2-baseline-update`, `sidis-hq2-lessons`, `sidis-hq2-docmap`, `sidis-hq2-plotting`.
- [ ] Ownership registry: `docs/skills_build/ownership.tsv` + `COVERAGE_MANIFEST.md` for the new records.

**Exact WRONG → RIGHT supersession lines (post-promotion; adapt `<date>`/values at promotion):**

- WRONG: `output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/` as physics basis, or quoting its integrated IBU `11.446174`/`11.158554 nb` or its `f_i` scalars (`11.2403%`/`10.8737%` response-internal) as current. RIGHT: the cone release `output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<date>/` (values from its `summary_totals.json`), with the four-part quoting rule (prior basis + Born-vs-raw + S2′ basis + cone status).
- WRONG: `output_v6/central/data_release/born_crad_dj14relbin_detints2prime_..._2026-10-10/` as the current Born-level release. RIGHT: the cone Born release (Phase 5 item 2).
- WRONG: `output_v6/central/data_release/systematics_detints2prime_15comp_..._2026-10-10/` as the canonical budget. RIGHT: the cone successor budget (Phase 5 item 3).
- WRONG: quoting any cone-basis value against any no-cone value (or the reverse) without the cone-status flag; treating the cone shift as a like-for-like method update. RIGHT: treat it as an observable-basis (selection-convention) change; quote with cone status.
- WRONG (live-guidance negation): "the alpha cone is not part of the central selection" / the S2′ builder DO-NOT-PORT entry "central is no-cone" as live guidance / "det.int should not be blindly pre-subtracted" (already negated by S2′). RIGHT: cone in the central selection per user decision 2026-10-11; external det.int. subtraction remains FORBIDDEN (double removal + keeper violation, stronger under S2′); the dated cone builder documents the DO-NOT-PORT reversal (the original S2′ builder file stays untouched as provenance).
- WRONG: P-ALPHA carried as an open template-heritage blocker. RIGHT: P-ALPHA formally closed/rescoped per Phase 5 item 5 (residual: low-Q2 α0p1 radius caveat unless re-derived).

## 11. Open questions / decision points (flagged; defaults given)

- **Q1 — Low-Q2 template radius mismatch (DECISION, default: caveat):** the low-Q2 DjBkgd vector (208.3795 scaled tracks) was built under an α0p1 cone. Options: (a) carry as an explicit tiny caveat (bounded ≪0.01% by the α0p1→α0p2 chain ratio, mean 0.99972) — DEFAULT for the candidate phase, since the Option-A external vectors remain UNAPPLIED to the central chain; (b) re-derive the one vector with α≥0.2 (cheap, single low-Q2 sample, Condor) — required before any consuming chain (bgsub Set-R re-base or budget background components) uses it on the cone basis. Decide at promotion time.
- **Q2 — Contingency cone-central (pre-S2′) response build (DECISION, default: NOT built):** the S2′ G9 identity cannot be re-run exactly without a cone-basis central response. Default accepts the replacement checks (§8 G9′). Trigger for building it (174 additional shards): C3/C4/G9′ failure or an out-of-band f_i shift. Decide only on trigger.
- **Q3 — Candidate dated name:** `bbb_ibu_detints2prime_alphaveto_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<launch-date>/` — confirm the launch-date suffix at submission.
- **Q4 — Cone-radius sensitivity as a budget component (DECISION, post-promotion):** whether a 0.1-vs-0.2-rad radius variation enters the successor budget as a NEW component (the α0p1 diagnostic products exist as a basis) or stays a documented selection-convention statement. The 15-component successor structure in Phase 5 assumes NO new component; adding one is a user decision with its own spec.
- **Q5 — Phase 5 internal ordering (supervisor proposes, user approves at promotion):** Born vs shifted-response budget legs vs plots sequencing; the shifted detector/tracking legs are the long pole (multiple 174-shard-class campaigns).

## 12. Risks and traps (T-list; every execution prompt must carry the relevant entries)

- **T1 Shard memory:** request 24–32 GB per shard; 5 files/shard precedent; the cone adds only one array op but do not shrink the request.
- **T2 Duplicate guardrails:** unique-shard-set + sha256 manifest at merge; `campaign_manifest.json` `submitted` flag; concurrent-duplicate-cluster watch (DIFFVM precedent 5385054/5385055); shard sums == merged totals exactly.
- **T3 DO-NOT-PORT reversal scope:** ONLY the alpha-cone entry reverses. The e/V0 truth-veto disablement and the miss-row fill for reclassified det.int. rows REMAIN forbidden. The new builder header must record the reversal and its user-decision provenance; the original S2′ builder is never edited.
- **T4 Cone vs Stage-1 HFS cone:** `RejectTracksNearScatElec(0.2)` at `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:394` is a hadronic-calibration exclusion affecting the HFS object, NOT tree admission — a DIFFERENT object. Do NOT touch it. No tree-builder edit of any kind is in scope; if one appears needed, STOP and escalate (scope change, h1-builder-engineer + user).
- **T5 Low-Q2 α0p1 mismatch:** see Q1; never quote the low-Q2 vector as cone-coherent without the caveat or the re-derivation.
- **T6 BSA fork divergence:** `v6bsa` stays raw/no-cone; post-promotion it diverges in observable basis from the central chain. Flag; do not conflate; no BSA edits here.
- **T7 G1 trap:** do NOT copy the S2′ G1 "miss bit-identical to central" assertion — the cone moves matched-leg gen partners into the miss row by design. Use G1′ (§8).
- **T8 Stale data-vector trap:** the S2′ driver consumed the central no-cone vector; the cone driver must repoint AND guard (label + `electron_alpha_veto_rad` + in-grid 3,865,073) — no fallback path may read a no-cone vector.
- **T9 Truth-veto documentation trap:** keep the CENTRAL truth-veto contract string in the new data vector (inert on data); do not inherit the alpha0p2 vector's `disable_truth_electron_veto_for_alpha_central` MC-convention documentation. On MC the e/V0 truth vetoes STAY in the response reco space (the cone response is not allfake).
- **T10 Pre-registration discipline:** every band (C4, C5, G7 tolerance, C3 window) is fixed in the acceptance register BEFORE the corresponding production/unseal; in-session modifications only as documented addenda (S2′ G10 lesson).
- **T11 Sentinel-counter trap:** the alpha0p2 data vector's matched-electron alpha counters were sentinel zeros (bookkeeping placeholders, not physics zeros). The new builders must emit REAL counters (or explicitly sentinel-mark them); never interpret sentinel zeros as measurements.
- **T12 Login-node policy:** Phase 0 single-file checks only, ≤5 min each; EVERYTHING multi-file (658-file vector, 174-shard response, toys, shard audit) goes through Condor.
- **T13 No compilation:** no H1OO build steps; `make bin` is out of scope. Python-only campaign.
- **T14 Quoting discipline pre-promotion:** any interim candidate number quoted against S2′ values already carries the cone-status flag; interim numbers are PROVISIONAL CANDIDATE, never "current".
- **T15 Set-R non-conflation:** the in-flight bgsub Set-R chain (`docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md` + its S2′ re-base addendum) is a SEPARATE chain with its own critic gate; this plan neither consumes nor edits it. Its cone re-base is a flagged dependency only (Phase 5 item 7).
- **T16 Per-bin identity risk:** C1(b) exact per-bin identity vs the June alpha0p2 vector assumes no convention drift between the June copied builder and today's central builder (same v4 lineage, M_PION both legs, binning v3). If it fails, enumerate drift sources before any relaxation; do NOT widen the gate silently.

## 13. Owners summary

| Phase | Execution | QA / gates | Critic | Notes |
|---|---|---|---|---|
| 0 evidence lock-in | h1-script-runner | h1-sidis-supervisor sign-off | — | login node only; acceptance register |
| 1 data vector | h1-script-runner (Condor 66-shard class) | supervisor | — | C1 |
| 2 response | h1-script-runner (Condor 174 shards) | supervisor | — | C3/C4/C7; h1-builder-engineer ONLY if a tree-builder change emerges (forbidden by scope) |
| 3 candidate release | h1-script-runner | supervisor | — | C5/C6 + G-carries |
| 4 suite + sidecar + critic | h1-script-runner (Condor toys/audit) | h1-qa-specialist | h1-analysis-critic | verdict = first-pass exit; promotion is a user decision |
| 5 post-promotion | h1-script-runner; h1-plotting-naf (plots); note writer (note, on request) | supervisor | critic on regenerated budget/plots per workspace precedent | install → Born → budget → plots → docs cascade |

## 14. Todo

- [ ] **P0.1** Read the §2 authoritative documents; re-assert current canonical basis (S2′ release paths, values, cluster precedents) against STATE.md.
- [ ] **P0.2** Create campaign dirs `scripts/unfolding/alphaveto_cone_s2prime_<date>/` and `output_v6/studies/alphaveto_cone_s2prime_<date>/` (scripts + smoke only at this stage).
- [ ] **P0.3** Write `build_data_vector_v4_alphaveto.py` (dated clone + cone port per §5, counters, central truth-veto contract string).
- [ ] **P0.4** Write `build_migration_th2_per_charge_s2prime_alphaveto.py` (dated clone + cone port per §6, DO-NOT-PORT reversal header, in-cone diagnostics, `--dump-cone-removed-legs`).
- [ ] **P0.5** Login-node single-file checks: cone formula/branch verification on one data tree + one MC tree (C2); record finite-alpha fraction, alpha distribution sanity.
- [ ] **P0.6** Data-file smoke: new builder vs alpha0p2 copied builder on the same file — exact per-bin match, `alpha_pre` parity, per-file rejection rate in the pre-registered window.
- [ ] **P0.7** MC-file smoke: cone S2′ builder on one RAPGAP31 file — variant markers, cone/det.int./keeper/reroute counters, conservation identities, in-cone diagnostics populated.
- [ ] **P0.8** Write `acceptance_register.{md,json}`: every gate C1–C7, G1′–G13′ with exact criteria; C4/C5 band-pinning procedures; smoke windows; input inventories + sha256s of all reference artifacts.
- [ ] **P0.9** Supervisor sign-off on the register; only then proceed.
- [ ] **P1.1** `make_campaign_files.py` (dated clone): data-vector shard lists (658 files, ~10/shard) + condor files + manifest.
- [ ] **P1.2** Condor data-vector campaign (build + merge); monitor to completion; no held jobs left.
- [ ] **P1.3** Gate C1: totals (3,865,073 / 1,965,700 / 1,899,373; in-PS 3,873,898; alpha_pre 3,902,480; rejected 28,582; removal 19,024), 980/980 per-bin identity vs alpha0p2 vector, monotonicity vs central vector (deltas ≥ 0, sum 19,024, top-10 hot bins reported), 0/980 negatives, schema/label fields.
- [ ] **P1.4** Record sha256 of the merged cone vector; RUN_STATUS campaign entry.
- [ ] **P2.1** `make_campaign_files.py`: response shard lists (RG 364→73, DJ 501→101, 5 files/shard) + condor `.sub/.args` + `campaign_manifest.json` (builder sha256, `submitted` flag).
- [ ] **P2.2** Condor response campaign (174 shards, 24–32 GB); merge to `merged/response_{rapgap31,djangoh14}_detint_s2prime_alphaveto.root`; duplicate guardrails (C7) enforced at merge.
- [ ] **P2.3** `check_merged_s2prime_alphaveto.py` + gate C3: per-cell body / per-reco-bin fake decomposition vs no-cone S2′ merged responses (exact); aggregate in-cone loss vs measured predictions (0.4259%/0.4524%, ≈75% matched-e) in the pre-registered window.
- [ ] **P2.4** Pin the C4 band from a merged-shard sample; record in the register BEFORE release use of the merged responses.
- [ ] **P2.5** G13 input: per-bin removable fractions vs parentage map (refs RG 0.906412 / DJ 0.912611; survival 0.9984 bound); G12(b) fc3 sumw2 recorded.
- [ ] **P3.1** Write `run_bbb_ibu_release_s2prime_alphaveto.py` (dated clone; repoints per §7 item 1–5; NEW data-vector guard; extended CAVEAT strings).
- [ ] **P3.2** Pre-register the C5 band in the register (measured Δd + response-side changes + documented algebraic propagation + width rationale).
- [ ] **P3.3** Run the driver: IBU k=4 + BBB; assemble the candidate release in the study tree (RAW only, S2′ layout).
- [ ] **P3.4** Gates: C6 (truth conservation 245/245; prior bit-asserted; independent IBU ≤1e-12), G8 form (0/980, 0/490, eff, 0 background-exceeds-data), G4 form (BBB-IBU consistency), C5 (integrated shift in band + direction/z-concentration/worst-bin diagnostics).
- [ ] **P4.1** Assemble the gate suite (dated clone of the S2′ validation scripts): all C + G gates; JSON twin; raw results.
- [ ] **P4.2** Condor: 1000-toy closure (G5; watch the neg-leg window edge) + shard audit (G1′d).
- [ ] **P4.3** 1000-toy stat sidecar campaign; install into the candidate as `stat_sidecar_1000toy/` (cross sections bit-identical).
- [ ] **P4.4** Independent QA pass (h1-qa-specialist) over candidate + gates.
- [ ] **P4.5** Critic dispatch (criteria §8); record verdict + conditions; FIXABLE-NOW conditions executed immediately.
- [ ] **P4.6** Checksum-verified copy of the candidate (+ sidecar) into `output_v6/central/data_release_candidates/bbb_ibu_detints2prime_alphaveto_..._<date>/` with install-side sha256 manifest; study-tree source intact.
- [ ] **P4.7** RUN_STATUS + context-handoff entries: candidate ready, promotion pending user decision. FIRST PASS ENDS HERE.
- [ ] **P5.* (post-promotion, each gated on the user promotion decision):** P5.1 central install (checksum-verified additive copy + install record); P5.2 Born release (unchanged C_rad tables; per-bin Born shift == raw shift ≤1e-12); P5.3 shifted detector legs ×5 WITH cone + tracking 16 seeds + phi_h re-derivation + B1 floors + 15-component successor budget (verify the convention gap closes); P5.4 plots (h1-plotting-naf; Born-plot/CRAD_CANDIDATES registration as a separate recorded decision); P5.5 P-ALPHA closure entry + Q1 low-Q2 decision executed; P5.6 analysis-note updates (selection.tex becomes true; results/systematics text fixes; note writer only); P5.7 BSA divergence flag + side-study labels; P5.8 full documentation cascade per §10 (STATE.md, RUN_STATUS.md, status docs, context-handoff, promotion record, registries, skills, ownership) with the exact WRONG→RIGHT lines.

---

**Provenance of this plan:** built from `sidis_highQ2/STATE.md` (2026-10-10 S2′ promotion override + 10-item ledger), `docs/promotions/detints2prime_promotion_2026-10-10.md`, `docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md`, `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md`, the S2′ builder/driver/campaign/gate-suite artifacts, and the alpha0p2 reference scripts, all read directly on 2026-10-11. The user decision of 2026-10-11 (cone imposition on the central chain) supersedes the cone impact study's §6 recommendation; the study's §5 rebuild chain is the methodological backbone executed here as a selection-convention change.

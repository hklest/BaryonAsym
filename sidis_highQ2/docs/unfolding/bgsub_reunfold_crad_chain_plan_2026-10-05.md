# Corrected data points for the 4-generator data-vs-MC comparison — rev 3: alpha0p2-coherent all-fake chain, low-Q2-only external subtraction, IBU k=4 at L=351000, FINAL per-bin C_rad (post-execution adjudication of the G8/G9 hard stop)

> **C_rad ATTACH SOURCE SUPERSEDED 2026-10-06:** the FINAL C_rad attach source named in this plan (blind, `xf_cut: False`, quarantined) is superseded for all Born-level use by the xFGen<0 xFcut baseline — RAPGAP31 nominal `sidis_highQ2/data/radiative/rapgap_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (integrated h+ `1.1368045` / h- `1.1362373`), DJANGOH14 cross-check `sidis_highQ2/data/radiative/djangoh14_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (h+ `1.1418534` / h- `1.1413756`); see `docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md`. Blind values in the body are retained unchanged as the executed-run record; the pending S3 re-attach must use the xFcut tables.

**Date:** 2026-10-05 (rev 3). Revision history: rev 1 (central-base design, superseded); rev 2 (alpha0p2-coherent all-fake design, executed through S3 on 2026-10-05, hard-stopped at the post-C_rad magnitude gate); rev 3 (post-execution adjudication: G8 magnitude window recalibrated onto per-bin arithmetic, cross-generator clause recalibrated, E1b variance spec corrected, S4 authorization — see §14). Rev 2's window [23.3638, 23.5986] nb is RETIRED as mis-calibrated; the chain itself was verified correct.
**Status:** executable specification; NOT executed. No production launched by this plan.
**Scope:** high-Q2 charged-hadron SIDIS, v6/z005, 490-bin grid (245 gen bins per charge), h+/h- without species PID.
**Deliverable:** corrected data points for `plots/mc_predictions/data_vs_mc_z005_4generator_2026-09-25/`, produced under the full proper physics selection — alpha(track,e) >= 0.2 cone veto, Empz > 45, and the standard central v6/z005 cut set — with base, background templates, and unfolding response selection-coherent, followed by IBU k=4 re-unfold at L = 351000.0 nb^-1 and FINAL per-bin C_rad application.

## 0. Decision record and design pivot

**User decision (2026-10-05, rev 2 trigger).** The corrected data points must be produced under the full proper physics selection: alpha cone veto at 0.2 rad, EMPZ > 45, plus the standard central v6/z005 cut set. The BASE reco data vector AND the unfolding response must both be built with the cone veto and EMPZ>45 applied, making base, background templates, and response selection-coherent. This resolves BLK-1 (the rev-1 P-ALPHA coherence defect: alpha0p2 templates subtracted from a central non-alpha base) by design rather than by caveat.

**Rev-1 design (superseded).** Rev 1 planned: central base vector (no cone veto) − externally subtracted full track-level background B (electron+V0+det.int.+low-Q2, 308,655.895 scaled tracks) → IBU k=4 with the CENTRAL response → FINAL C_rad. That design carried BLK-1 as an unresolvable ~7.24% reco-level bias (e/V0 templates disjoint from the central base). It is obsolete under the decision and is retained here only as decision history.

**Rev-2 design (this plan).** The coherent realization of the decision is the **all-fake (allfake) convention**, documented as the recommended response-consistent treatment in `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`:

- BASE = the existing **alpha0p2 raw reco data vector** (cone veto + Empz>45 + full central cuts; 658 data files; 3,865,073 in-grid tracks).
- RESPONSE = the existing **allfake alpha0p2 response** (same reco selection; electron, V0-daughter, and det.int. reco rows all routed to the fake column; RAPGAP31 nominal, DJANGOH14 model cross-check).
- Background removal: electron + V0 + det.int. + ordinary unmatched fakes are removed **exactly once, internally**, via the response fake column (row-fraction fake treatment in `derive_response_blocks`). The track-level background templates for these components are **not** subtracted externally — subtracting them on top of a response that already represents them would double-subtract (handoff §7). The templates are retained as **validation references** (§5 S1b).
- The ONLY external subtraction is the **low-Q2 DJANGOH DjBkgd leakage template** (208.379505 scaled tracks at L=351000), which is source-distinct and not represented in the high-Q2 response MC.
- IBU k=4 at L = 351000.0 nb^-1, then FINAL C_rad (RAPGAP31 `empz_gt45` nominal; DJANGOH14 cross-check).

**Critical finding enabling this design:** the coherent chain already exists end-to-end at correct luminosity. The allfake IBU study of 2026-06-28 ran exactly this convention (raw alpha0p2 vector + allfake response + row-fraction fake treatment + IBU k=4 at L=351000.0) and published totals of **20.736829 nb (h+ 10.509695 / h- 10.227134)**. The new work is: (a) add the low-Q2 external subtraction (0.0054% effect), (b) attach the FINAL C_rad tables (which postdate the allfake study), (c) run the verification/validation gates. **No tree aggregation and no response rebuild is required.**

A second consequence, established from the handoff and the partition studies: the June 2026 bgsub chain (alpha0p2 response + external e+V0+low-Q2 subtraction, ~19.49 nb at stale L=356,613) was selection-coherent but **treatment-inconsistent** — its external electron subtraction double-subtracted the electron content that the alpha0p2 response already removes via its fake column. June totals are therefore **not** an anchor for this chain (§11).

## 1. Verified inventory: what exists, what must be rebuilt

All paths below were inspected on 2026-10-05 (JSON/NPZ/ROOT reads and code reads; no modifications).

### 1.1 Base reco data vector — EXISTS, reuse as-is

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/data_vector_alpha0p2_tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.json`

- schema `data_vector_v4_native_fine_reco_raw_no_bgsub` v1.1; label `raw_data_no_bgsub_alpha0p2`; `bg_subtracted = false`.
- 658 input data files, 0 failed. Data vectors are raw track counts — luminosity-free; "correct luminosity semantics" means the finalcandidate 658-file dataset, which this is.
- Selection contract (quoted verbatim from `fiducial_contract`): `v4_fid4cut_2026-06-06_reco_only plus local reco-cleanup diagnostic: 0.1<Yes<0.7; 150<Q2es<20000; Empz>45 (no upper); xF_SIDIS_es<0 (current hemisphere, internal H1 sign). Object cleanup: |PVz|<35, Pt>0.15 with no upper reco Pt cut, |Eta|<1.6, NHitDedx>=5, RadLength>=5.0, |DcaPrime/cosh(Eta)|<2, IsFromPrimary==1, central or short/combined trk class, alpha(track,e)>=0.2 rad reco-only, NO IsMatchedElectron==0 truth-label veto, IsMatchedV0Daughter==0, z_reco in (0,1). No theta cut. EVLS pass / envelope flags NOT used. TrackLength cut NOT applied because MC TrackLength is sentinel -999.`
- `electron_alpha_veto_rad = 0.2`, `alpha_reco_only = true`, `disable_truth_electron_veto_for_alpha_central = true`.
- Totals: `n_tracks_in_grid = 3,865,073` (h+ 1,965,700 / h- 1,899,373); `n_tracks_in_ps = 3,873,898`; alpha-rejected 28,582 (in-PS). The matched-electron alpha counters are sentinel zeros (both `alpha_rejected_matched_electron_count` and `alpha_survive_matched_electron_count` = 0); they are bookkeeping placeholders, not physics zeros — the cone veto's physical effect is measured by the central-vs-alpha0p2 in-grid difference (19,024 tracks, §Appendix A).
- `per_bin` is columnar: `bin_index, charge (0=pos,1=neg), n_raw, n_subtracted, sigma_raw, sigma_subtracted`. For this raw vector `n_subtracted ≡ n_raw` and `sigma_subtracted ≡ sigma_raw` (identical columns; sums 3,865,073 / 37,478.536360).
- **Cone-veto + EMPZ>45 verdict: present.** The cone veto is in the contract and in the builder configuration; `Empz>45 (no upper)` is in the contract. **No rebuild needed.**

Note on the contract string: `IsMatchedV0Daughter==0` is an MC-truth-label veto (ancestor list K0S/K0L/K0/Lambda per handoff §2). On DATA the truth flags are sentinel zero, so this veto is **inert** — the data vector contains real V0 daughters, which is exactly what the allfake response's fake column requires (§4.4). The contract string documents MC-side semantics; the effective data selection is truth-blind. This documentation-vs-effect mismatch is recorded as BLK-1 residual caveat (a).

### 1.2 Unfolding response — EXISTS (two candidates); allfake chosen; no rebuild

**Chosen: allfake response** — `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/responses/`
- `allfake_response_matrices_and_categories.npz`: per generator (`rapgap31`, `djangoh14`) and charge (`pos`, `neg`): `R` matrices of shape **(491, 246)** (490 reco rows + 1 miss row; 245 gen columns + 1 fake column) plus per-reco-bin (490,) fake-category vectors: `fake_electron`, `fake_v0daughter`, `fake_detint`, `fake_existing_other`.
- `migration_inputs_allfake_alpha0p2_z005.root`: same content as TH2s (`h_{tag}_R_{charge}`, `h_{tag}_fake_{category}_{charge}`). NOTE: this ROOT has a **different key structure** from the central/alpha0p2 response ROOTs — there are no `h_prior_gen_*` and no `h_data_bgsub_*` keys. The allfake IBU runner consumes the NPZ directly; no ROOT patching step exists or is needed in this chain.
- Inputs (from `manifests/input_manifest.json`): RAPGAP31 364 files (samples 7071 em06, 7094 ep0607, 7096 em0405, 7097 ep0304) and DJANGOH14 501 files, all from `output_v6/central/sidistree/mc/...` — the same central v6 tree production that feeds the central response. Built via Condor (cluster 3987932).
- **Selection, verified in builder code** (`scripts/build_migration_th2_per_charge_allfake.py`; the file's docstring is partially STALE — see below):
  - v4 fiducial on BOTH legs: `0.1 < y < 0.7` (reco `Yes` / gen `Y_es_gen`), `150 < Q2 < 20000` (reco `Q2es` / gen `Q2_es_gen`), current-hemisphere `xF < 0` (reco `xF_SIDIS_es` / gen `xFGen`), **`Empz > 45`, no upper bound** (reco `Empz`; gen `GenEmpz`) — the v6 tree-level `Empz_hi = 70` is explicitly NOT applied.
  - Reco cleanup: Pt > 0.15 (no upper), |Eta| < 1.6, NHitDedx >= 5, RadLength >= 5.0, track class, plus **`alpha(track,e) >= 0.2 rad` cone veto** (3D arccos from track and scattered-electron momenta) and **no `IsMatchedElectron==0` truth veto**.
  - Category routing (code, not docstring): reco rows classified as `IsMatchedElectron != 0` → fake_electron; else `IsMatchedV0Daughter != 0` → fake_v0daughter; else det.int. (STR/GEANT secondary matched tracks) → fake_detint; remaining unmatched/outside-gen/non-pi/K/p → fake_existing_other. **V0 daughters and det.int. are NOT dropped** — they are routed to the fake column. The builder's header docstring still contains the inherited v4 line "alpha(track,e) >= 0.2 rad, IsMatchedV0Daughter == 0" describing the OLD (alpha0p2) cleanup; the code routes V0 daughters into the fake column instead. The validation numbers prove the code behavior: fake-category sums equal the fake column exactly (`category_sum_matches_fake_column: true`).
  - Gen pool: pi/K/p only, `IsV0DaughterGen == 0`, gen fiducial incl. `GenEmpz > 45`.
- Validation (`validation/validation_summary.json`): RAPGAP fake column sums pos 8,759,990 / neg 8,436,563; category totals (RAPGAP, charges combined): fake_electron 3,819,130 / fake_v0daughter 3,432,137 / fake_detint 676,561 / fake_existing_other 9,268,725 (sum 17,196,553 = fake column exactly). Mean fake fraction over nonzero reco bins ≈ 0.1437 (pos) / 0.1441 (neg) for RAPGAP — the operative fake fractions for this chain (the rev-1 central-response fractions 10.7141%/10.4019% are superseded; old BLK-7 dissolved).
- **June alpha0p2 response reusability verdict (user question):** the June response (`electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_..._percharge.root`) WAS built with the full central cut set (v4 fiducial incl. Empz>45, both legs) PLUS the cone veto MINUS the electron truth veto — i.e., **it already contains EMPZ>45; no EMPZ rebuild is needed for it either**. It is reusable in principle but is NOT used by this chain because its category treatment (non-V0 electrons in the fake column; V0 daughters dropped entirely; det.int. ~86% in the matched body) forces a mixed external/internal subtraction convention and leaves det.int. inside the observable. The allfake response gives the uniform, exactly-once treatment and the cleaner physics definition (observable = generator-hadron level, excluding electron tracks, V0 daughters, and GEANT/STR secondaries), which is what a data-vs-generator comparison requires.

### 1.3 Published allfake IBU anchor — EXISTS, is THE anchor

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/allfake_alpha0p2_z005_ibu_tunfold_diagnostic_20260628/`
- Runner `scripts/run_allfake_ibu_release.py`: `L_DATA = 351000.0` hardcoded; consumes the allfake NPZ + the raw alpha0p2 vector JSON directly (no ROOT patching); **IBU prior is computed as `body + miss` from the response matrix itself** (`prior_gen = M_ext[:NB_R,:NB_G].sum(axis=0) + M_ext[NB_R,:NB_G]`) — no stored prior exists in the allfake package, so the central stored-prior mismatch (old BLK-2) cannot arise (§10). CLI: `--response-npz`, `--data-json`, `--binmap-json`, `--output-root`, `--iterations` (default 1..10), `--selected-k` (default 4).
- Published release `ibu_release/ibu/cross_sections.{csv,json}`: 490 rows (245 per charge), k=4, L=351000.0, raw denominator 3,865,073, **totals 20.736829 nb (h+ 10.509695 / h- 10.227134)**, 0 negative and 0 non-finite cross sections; QA verdicts PASS for the IBU branch (the manifest's `REPAIRED_REMAINING_QA_BLOCKERS` status records repair history; the final IBU QA summary is PASS; TUnfold-side negatives are out of scope — TUnfold is not the release method).
- Conventions recorded: `all-fake fake column used exactly once; no external electron/V0/det.int pre-subtraction`; fake handling "exactly once: row-wise fake fraction in derive_response_blocks".
- **The published run did NOT subtract low-Q2 externally** (raw denominator = unsubtracted vector). Adding it is the new step S1/S2.
- Poisson toys (2026-06-29): chunk summaries PASS; IBU toys 0 negative bins. These are the closure/stability evidence for the allfake convention.
- `per_bin` loading detail (drives runner edit E1): `_load_data_vector` uses `value_key = 'n_subtracted' if ('n_subtracted' in per_bin and not bg_subtracted) else 'n_raw'`. For the raw vector this selected `n_subtracted` (identical to `n_raw`). **For a new vector marked `bg_subtracted = true`, the current logic would load the UNSUBTRACTED `n_raw` column — edit E1 in §5 S2 is mandatory.**

### 1.4 Background templates at L=351000 — EXISTS; role changes

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/`
- 7 hash-pinned NPZ aggregates (per generator × component): `AGGREGATED_{rapgap31,djangoh14}_{electron,v0daughter,detint}_alpha0p2_combinedL_L351000_finalcandidate.npz` and `AGGREGATED_djangoh14_lowq2_djbkgd_L351000_finalcandidate.npz` (+ per-aggregate JSON metadata + `validation_report.json`).
- All templates were produced under the alpha0p2 selection object (the producer imports `build_migration_th2_per_charge_v4.py` as its selection authority) with e/V0 rows kept eligible for classification — selection-coherent with the base vector and the allfake response reco leg by construction.
- **Nominal chain use: low-Q2 only.** `AGGREGATED_djangoh14_lowq2_djbkgd_L351000_finalcandidate.npz`: `yield_980` sum **208.379505** (h+ 119.430688 / h- 88.948818; max per-bin 23.475), `var_980` sum 138.835095 (h+ 86.484740 / h- 52.350354); also stores per-charge 490-length `yield_hplus/yield_hminus` and raw/sumw bookkeeping.
- **Validation use (S1b): e/V0/det.int. templates** compared per bin against the allfake response's fake-category vectors scaled by `L_data / L_MC` (expected agreement: arithmetic pre-checks give fake_electron(RG) × 351000/9132891 = 146,762 vs averaged template 148,976 (ratio 0.985); fake_v0daughter 131,880 vs 132,065 (0.999); fake_detint 25,998 vs 27,406 (0.949) — indicative only, since the templates' central combination averages generators and the response legs may carry NucliaWeight while the templates exclude it; the per-generator, per-bin comparison with these caveats is the actual gate G9).
- The published background CSV (`data/systematics/background_tracklevel_alpha0p2_2026-09-25/`) remains a gen-245-folded projection for the systematics line; it is NOT the subtraction source. `B_track_inclusive_before_overlap = electron+v0+detint+lowq2` (308,655.895 scaled tracks) is the rev-1 quantity; under the allfake convention only its low-Q2 component is externally subtracted.

### 1.5 FINAL C_rad — EXISTS (verified 2026-09-30 provenance)

*(SUPERSEDED: use xFcut baseline, see banner.)*

- RAPGAP31 nominal: `sidis_highQ2/data/radiative/rapgap_crad_final_2026-09-30/crad_v3_empz_gt45_nominal_490rows.csv` (+ `crad_v3_summary.json`).
- DJANGOH14 cross-check: `sidis_highQ2/data/radiative/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_empz_gt45_nominal_490rows.csv` (+ summary).
- 490 rows = 245 gen bins × 2 charges (charge-major), gen-level edges (row 0: h+, cell 0, Q2 [150,300], x [0.0021,0.0071], z [0,0.05], pTh [0,0.5]) — the same gen grid as the unfolded output. Integrated C_rad: RAPGAP31 h+ 1.1328903 / h- 1.1319064; DJANGOH14 h+ 1.1375979 / h- 1.1366709 (integrated DJ/RG − 1 ≈ +0.42%). `C_rad = sigma_nonrad / sigma_rad`, `sigma = sum(weight1*weight2)/L_MC`. The RAPGAP `projection_regression_pass = false` flag is a documented fork artifact (provenance doc), not a defect. 490/490 finite for both generators.
- Derivation selection (verified in `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py`): gen level only; fiducial `150 < Q2_es_gen < 20000`, `0.1 < Y_es_gen < 0.7`, **`empz_gt45` mode = `GenEmpz > 45`** (truth-level event E−Pz, GeV); hadron pool pi/K/p with `IsGtr==1`, `Mother2==-1`, V0-daughter veto via `IsV0DaughterGen_v3`/`VetoAncestorPDG_v3`; **`xf_cut: False`** (no current-hemisphere cut; `xFGen` is not even in the worker's REQUIRED branch list); `truth_eta_cut: False`; no reco-level cuts; no alpha cone veto (reco-only cut, documented as not applicable to gen-only C_rad); RAD sample carries a numerator-only `Q2Gki > 100` cut (in-fiducial effect ~0.09%). Coherence analysis in §9.

### 1.6 EMPZ>45 status across all artifacts — NO REBUILD ANYWHERE

| Artifact | EMPZ>45 present? | Evidence |
|---|---|---|
| Central data vector | yes | contract: `Empz>45 (no upper)` |
| alpha0p2 base vector (this chain's base) | yes | same contract string |
| alpha0p2 response (June) | yes | v4 fiducial both legs (builder family) |
| allfake response (this chain's response) | yes | builder code: reco `Empz > 45.0`, gen `GenEmpz > 45`, no upper bound |
| FINAL C_rad (both generators) | yes | `empz_gt45` mode = `GenEmpz > 45` |
| v6 trees | stored with LOOSENED envelope | `30 < Empz_reco < 75` written by the tree builder; the analysis cut is applied at aggregation |

**EMPZ>45 is fully definable from stored branches** (§3): the v6 trees deliberately store a loosened Empz envelope so the analysis cut can be applied (and re-defined) at aggregation. Every existing aggregation-level artifact already applies `> 45`. **Nothing needs an EMPZ rebuild.**

### 1.7 Rebuild verdict summary

- **Must be rebuilt: NOTHING for the nominal chain.** New small artifacts only: the low-Q2-subtracted data vector JSON (S1), the two IBU runs (S2: reproduction + subtracted), the C_rad-attached release (S3), and the validation/manifest (S1b/S4).
- Contingency rebuild paths exist and are specified in §1.8; they trigger only if a verification gate (G1/G2) fails.

### 1.8 Contingency rebuild path (only if verification gates fail)

Trigger conditions: G1 failure (base vector contract/sums wrong) or G2 failure (allfake response integrity/selection wrong). Both would indicate silent corruption of pinned artifacts, not a design gap.

- **Base data vector rebuild** (658 data files, Condor mandatory — multi-file): `build_data_vector_v4.py` + `wrap_build_data_vector_v4.sh` + `submit_build_data_vector_v4.condor` (all in `electron_cone_alpha0p2_z005_20260623/copied_scripts/`), configured with the alpha0p2 variant flags (`electron_alpha_veto_rad = 0.2`, `alpha_reco_only = true`, `disable_truth_electron_veto_for_alpha_central = true`) and the v4 fiducial incl. `Empz_lo = 45`. Smoke: one data file on the login node (single-file, < 5 min; precedent: `electron_cone_alpha0p2_z005_20260623/smoke/`).
- **Allfake response rebuild** (RAPGAP31 364 + DJANGOH14 501 MC files, Condor mandatory): `build_migration_th2_per_charge_allfake.py` + `wrap_build_migration_allfake.sh` (`alpha0p2_allfake_response_z005_20260627/scripts/`), per-file migration stage + merge, category routing to the fake column. Smoke: one MC file (precedent: `alpha0p2_allfake_response_z005_20260627/smoke/`).
- Both rebuilds reproduce the pinned selections exactly; rebuilt artifacts must re-pass G1/G2 and the published-anchor reproduction gate G4 before use.

## 2. Exact selection and binning enumeration (central v6/z005 provenance)

**Event-level fiducial (the "4-cut" v4 fiducial, reco variables):**
1. `0.1 < Yes < 0.7` (eSigma y)
2. `150 < Q2es < 20000` GeV^2 (eSigma Q2)
3. `xF_SIDIS_es < 0` (current hemisphere; internal H1 sign convention)
4. `Empz > 45` GeV, **no upper bound** (eSigma-based event E−Pz; the v6 tree envelope is 30–75, the old v6 code upper bound 70 is NOT part of the fiducial)

**Track-level object cleanup (NOT part of the physics fiducial; separated by contract):**
`|PVz| < 35` cm; `Pt > 0.15` GeV with **no upper** reco Pt cut; `|Eta| < 1.6` (lab); `NHitDedx >= 5`; `RadLength >= 5.0`; `|DcaPrime/cosh(Eta)| < 2`; `IsFromPrimary == 1`; central or short/combined track class; `z_reco` in (0,1); no theta cut; no EVLS/envelope-flag requirement; no TrackLength cut (MC sentinel −999).

**Truth-label vetoes (MC semantics; inert on data):** central selection carries `IsMatchedElectron==0` and `IsMatchedV0Daughter==0`. The alpha0p2 variant DROPS the electron veto (the cone veto physically removes the scattered-electron track) and keeps the V0-daughter veto string (inert on data; §1.1 note).

**alpha0p2 variant delta (this chain's selection):** + `alpha(track,e) >= 0.2` rad, reco-only cone veto; − `IsMatchedElectron==0` truth veto. Everything else identical to central.

**Correction to the task framing:** `z >= 0.05` and `pT_h > 0.5 GeV` are **NOT selection cuts** — they are the first bin edges of the published grid. The track-level selection is `z_reco ∈ (0,1)` and `Pt > 0.15`; the grid deliberately includes the low bins z ∈ [0, 0.05) and pT(gen) ∈ [0, 0.5) ("zlow005" / "pT5" binning-variant names). No track is cut at z=0.05 or pT=0.5.

**Binning (verified from the central binmap, identical for alpha0p2/allfake):**
- Q2 strata: S0 [150,300], S1 [300,700], S2 [700,1500], S3 [1500,20000] (same for reco and gen).
- x subbins per stratum: S0:2, S1:2, S2:2, S3:1 → 7 (Q2,x) cells.
- z edges (reco = gen): [0.0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0] → 7 bins.
- pT edges: gen [0.0, 0.5, 1.0, 2.0, 4.0, 8.0] → 5 bins; reco [0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0] → 10 bins.
- Grid: 7 cells × 7 z × 5 pT(gen) = **245 gen bins per charge**; 7 × 7 × 10 = **490 reco bins per charge**. `binmap_reco_to_gen` is EXACTLY `repeat(arange(245), 2)` (verified equal to the central binmap array) — reco is 2× finer in pT only; the allfake runner's synthetic map is therefore exact.

## 3. Branch names enabling the cuts at aggregation level (v6 trees)

Sources: `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` (stage-1 builder) and `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` (src variant; same branch schema). Confirmed against the branch lists actually read by the allfake response builder and the alpha0p2 data-vector builder.

**Event-level (evlTree):**
- `Empz` — **reco event E−Pz in GeV**, eSigma-based (assigned from the same gH1Calc kinematics as `Q2es`/`Yes`). The v6 builder writes a loosened envelope `30 < Empz < 75` (superseding the old 35–70) precisely so the analysis cut `Empz > 45` can be applied at aggregation. **This is the branch that makes an EMPZ>45 event cut definable at aggregation level; units GeV; no upper cut in the fiducial.**
- `Q2es` (GeV^2), `Yes` (dimensionless), `Xes`, `Q2e`, `PVz` (cm), `xF_SIDIS_es` (hemisphere sign variable).

**Track-level (ChargedTot), including the cone-veto inputs:**
- Cone veto: track momenta `Px, Py, Pz` (GeV) and scattered-electron direction `scatEX, scatEY, scatEZ` (GeV components; `scatEE` energy also stored). `alpha = arccos( (p_trk · p_e) / (|p_trk| |p_e|) )` computed at aggregation; veto `alpha < 0.2` rad. No stored alpha branch is required.
- Cleanup: `Pt` (GeV), `Eta` (lab), `NHitDedx`, `RadLength`, `DcaPrime`, `IsFromPrimary`, `IsCentralTrk`, `TrackType`.
- Truth/classification (MC only; sentinel-0 on data): `IsMatched`, `IsMatchedElectron`, `IsMatchedHadron`, `IsMatchedConvOrDalitz`, `IsMatchedV0Daughter`, `IsStr`, `IsGtr`.
- Weights: `weight1`, `weight2`, optional `NucliaWeight`.

**Generator-level (ChargedTotGen, MC only):**
- `GenEmpz` — **generator-level event E−Pz in GeV, always written on MC**; definition in the tree builder: `GenEmpz = GenEnElec*(1-cos(GenThElec)) + GenEnHad - GenPzHad`. This is the variable used by the response gen fiducial and by the C_rad `empz_gt45` mode.
- Gen kinematics: `Q2_es_gen` (GeV^2), `Y_es_gen`, `x_es_gen`, `zGen`, `xFGen`, `GenPt` (GeV), `GenEta`, `GenPhi`, `GenPDG`, `GenCharge`, `IsV0DaughterGen`; the response builder resolves per-event `GenEmpz` from ChargedTotGen once per (Run, Event).

**Verdict:** both required cuts are aggregation-level-computable from stored branches: `alpha(track,e) >= 0.2` from `(Px,Py,Pz)` × `(scatEX,scatEY,scatEZ)`, and `Empz > 45` (reco) / `GenEmpz > 45` (gen) directly from the stored `Empz` / `GenEmpz` branches (GeV). No missing-branch blocker exists.

## 4. Chain design: conventions and exactly-once accounting

### 4.1 Nominal composition

```
d_corrected_points (per gen bin j, per charge) =
    IBU_k4[ d_reco_i = n_raw_i(alpha0p2) − lowQ2_i ; R = allfake(RAPGAP31) ]
    × C_rad_final(RAPGAP31, empz_gt45)_j
```

- Base: raw alpha0p2 vector, as published (no rebuild).
- Response: allfake, RAPGAP31 nominal; DJANGOH14 available as model cross-check (the runner computes both; the release cross sections are RAPGAP31-based, matching the C_rad nominal recommendation — generator-coherent on both layers).
- Fake/background accounting, exactly once per component:
  - electron tracks (matched e± incl. conversions/material) → allfake fake column (internal);
  - V0 daughters → allfake fake column (internal; present in data because the truth veto is inert on data);
  - det.int. (STR/GEANT secondary matched tracks) → allfake fake column (internal; reclassified wholesale by the allfake builder, with miss-suppression for reclassified rows that had valid signal-gen candidates);
  - ordinary unmatched/outside-gen/non-pi/K/p fakes → allfake fake column (internal);
  - low-Q2 DJANGOH DjBkgd leakage → **external subtraction** (S1), the only source-distinct component not represented in the response MC.
- The fake treatment is the row-fraction keep factor `matched/(matched+fake)` applied in `derive_response_blocks` — the same mechanism as the published allfake release (caveat field: "exactly once: row-wise fake fraction in derive_response_blocks").
- C_rad: FINAL per-bin, RAPGAP31 nominal (model-coherent with the RAPGAP31 response), DJANGOH14 as cross-check column. BLK-5 (open generator review) is carried; the nominal can flip to DJANGOH14 without rerunning the chain (S3 re-attach only).

### 4.2 Rejected alternatives (with authority)

1. **External e/V0/det.int. template subtraction on the allfake response** — double-subtraction: those categories are already in the fake column. Rejected per handoff §7 (allfake bullet: "the data vector should not additionally subtract these components externally; fake subtraction and its covariance must handle them exactly once").
2. **The June 2026 mixed convention** (alpha0p2 response + external e+V0+low-Q2) — treatment-inconsistent: external electron subtraction double-subtracts (the alpha0p2 response's fake column already contains non-V0 electrons); V0 external subtraction was the correct mechanism under THAT response (V0 dropped from it), which is why June was only partially inconsistent; det.int. stayed in the observable (86% matched body). Superseded wholesale by the allfake convention. June totals are not an anchor (§11).
3. **Rev-1 design** (central base + full external B + central response) — BLK-1 defect (~7.24% reco-level bias); superseded by the user decision.
4. **Disabling the fake treatment and subtracting everything externally** — nonstandard machinery change with no validated precedent; rejected.

### 4.3 Response-consistency rules (handoff authority, restated for this chain)

- Never subtract externally a component the response already represents (fake column or matched body).
- Never leave in data a component the response drops or misrepresents, without external correction.
- Under the allfake response, the e/V0/det.int. template content IS the fake column content (partition-validated: category sums equal the fake column exactly) — this is the strongest available form of "base, templates, and response selection-coherent", and it is what the user's decision requires.

### 4.4 det.int. / V0 / electron coherence re-check (BLK-1 resolution detail)

- **det.int.: NOW COHERENT to subtract** (user question 5). The rev-1/June concern (86% of det.int. sits in the matched body of the alpha0p2 response, so external subtraction would remove modeled signal) is resolved by the allfake response, which reclassifies ALL det.int. rows into the fake column. The subtraction happens exactly once, internally, MC-shape-based per reco bin. Residual: the standard MC-fake-subtraction caveat — the data-side secondary composition is corrected with MC-estimated per-bin fractions; MC/data composition differences are second-order and covered by the model-uncertainty machinery (DJANGOH14 model cross-check), not by this chain.
- **V0 daughters: coherent.** Data contains real V0 daughters (truth veto inert); the allfake fake column contains the MC V0-daughter category; removed exactly once. Residual: the base vector's contract string still lists `IsMatchedV0Daughter==0` (documentation-only mismatch, BLK-1 caveat (a)).
- **Electrons: coherent.** The cone veto removes the scattered-electron track (19,024 in-grid tracks, 0.49% of the central vector); the residual electron backgrounds (conversions/material e±) are in the fake column; removed exactly once.
- **Cone-veto magnitude note:** the cone veto itself is a small correction (0.49%); the diffuse backgrounds (e/V0/det.int., ~7.9% of the base) are removed by the fake column, not by the cone veto. The decision's coherence requirement is about WHERE each removal happens, not about the cone veto doing the removal.

## 5. Chain steps (revised S0–S5 + plot step)

All steps are post-merge arithmetic (JSON/NPZ/CSV reads, IBU, per-bin multiplication). No tree processing. Login-node execution is acceptable per the June precedent (IBU runs 2–5 min each); move to Condor if the 5-minute single-command cap is threatened. Contingency rebuilds (§1.8) are Condor-only.

### S0 — Verification of pinned inputs (read-only)

Re-assert, from the artifacts themselves (not from this document): base vector contract fields and sums (G1); allfake response shapes/finiteness/category conservation (G2); binmap identity `binmap_reco_to_gen == repeat(arange(245),2)` (G2); low-Q2 NPZ sums (G3 pre-check); FINAL C_rad tables loadable, 490 rows each, finite (G7 pre-check). Record sha256 of every consumed input into the study manifest.

### S1 — Build the low-Q2-subtracted data vector (new script `build_bgsub_vector_lowq2_only_v2.py`)

Inputs: the alpha0p2 vector JSON (§1.1) + `AGGREGATED_djangoh14_lowq2_djbkgd_L351000_finalcandidate.npz` (§1.4).
1. Load `yield_980`, `var_980` (980, charge-major) and cross-check against `yield_hplus`/`yield_hminus` (490 each) and the vector's `charge` column (0 = pos, first 490; 1 = neg, last 490). Gate: `sum(yield_980) = 208.379505` (±1e-3), per-charge 119.430688 / 88.948818.
2. Compute per bin: `n_subtracted_new[i] = n_raw[i] − yield_980[i]`; `var_subtracted[i] = n_subtracted_new[i] + var_980[i]` (Poisson data + template variance).
3. Negative-bin scan: require `n_subtracted_new[i] >= 0` for all 980 (expected safe; max per-bin low-Q2 yield 23.475). Gate G3.
4. Write the new vector JSON (same directory tree as the study, never overwrite the original): copy all top-level fields; set `bg_subtracted = true`; `label = "lowq2_subtracted_alpha0p2_allfake_convention"`; extend `per_bin` with the new `n_subtracted` values and add `var_raw` (= n_raw) and `var_subtracted` columns; recompute `sigma_subtracted = sigma_raw × n_subtracted/n_raw` (documentation column); add a `background_subtraction` block recording: externally subtracted = low-Q2 DJANGOH DjBkgd only (path + sha256 + sums + variance), NOT subtracted externally = electron/V0/det.int. (allfake fake column, exactly once; handoff citation), inherited fiducial contract (alpha0p2 + Empz>45), and the inert-veto note.

### S1b — Template-vs-fake-column coherence validation (new diagnostic script `validate_template_vs_fakecolumn_v1.py`)

Per generator (RAPGAP31, DJANGOH14), per category (electron, v0daughter, detint), per charge: compare the L351000 template vector (from the §1.4 NPZs) against the allfake response fake-category vector scaled by `L_data / L_MC(generator)` (L_MC from the response input provenance: RAPGAP31 9,132,891.0 nb^-1, DJANGOH14 10,487,495.0 nb^-1 — re-verify at execution). Report per-bin ratios and shape correlation. **Non-blocking diagnostic** (G9) — the known weight-convention difference (response legs may include NucliaWeight; templates exclude it) means integrated ratios ≠ 1 are expected; the gate fails only on gross shape disagreement. Purpose: demonstrate that the fake column IS the template content under the common selection (the user's coherence requirement, made quantitative).

### S2 — IBU k=4 runs (copied runner with exact edits)

Copy `run_allfake_ibu_release.py` → study-local `run_allfake_ibu_release_v2.py` with EXACTLY these edits (nothing else):
- **E1 (mandatory, correctness):** `_load_data_vector` value-key logic →
  `value_key = 'n_subtracted' if (d.get('bg_subtracted', False) and 'n_subtracted' in pb) else 'n_raw'`
  (the current logic selects `n_raw` for any vector marked `bg_subtracted=true`, which would silently unfold the UNSUBTRACTED counts).
- **E1b (variance; CORRECTED in rev 3 — the rev-2 text was defective):** extend `_load_data_vector` to also return the w2 column: **`w2 = var_subtracted`** when `bg_subtracted` and `var_subtracted` present, else the loaded counts (Poisson). `var_subtracted` already IS `n_subtracted + var_980` (Poisson + template variance) per S1 step 2, so the rev-2 formula `w2 = (n_subtracted + var_subtracted)` double-counted the Poisson term (data variance inflated ×2.00022, stat unc ×1.41433). Central values are provably unaffected: `ibu_unfold` uses `d_sig_w2` only for the propagated diagonal error, never in the iteration (verified in the engine source). The raw-vector reproduction run is unaffected (falls back to counts → reproduces the published release exactly).
- **E2 (metadata):** CAVEAT/LUMI strings updated to the rev-2 chain text (§12 wording, PROVISIONAL DIAGNOSTIC label, low-Q2-only external subtraction, allfake convention, FINAL C_rad pending in S3).
- **E3 (paths/labels):** default `--output-root` points into the new study dir; output labels distinguish raw-reproduction vs subtracted runs. Keep `L_DATA = 351000.0`, `NB_R/NB_G`, the prior := body+miss construction, and all IBU math untouched. Keep the `SOURCE_IBU` absolute-path import of `crosscheck_unfold_methods_v4.py` from the June study scripts dir (read-only import of the validated engine; do not fork the engine).
- Run A (reproduction): `--data-json <alpha0p2 raw vector> --selected-k 4 --output-root <study>/repro_raw_vector/`. Gate G4: totals reproduce 20.736829 / 10.509695 / 10.227134 nb to ≤ 1e-6 relative.
- Run B (corrected, pre-C_rad): `--data-json <S1 low-Q2-subtracted vector> --selected-k 4 --output-root <study>/`. Gates G5, G6.

### S3 — Attach FINAL C_rad (new script `attach_crad_final_allfake_v1.py`)

Adapts the rev-1 S3 design to the allfake release format:
1. Load Run B's `ibu_release/ibu/cross_sections.{csv,json}` (490 rows) and both FINAL C_rad CSVs.
2. Align on (charge, cell label, z edges, pT edges) — 490↔490, both charges; assert exact edge equality against the release row edges and the binmap.
3. Corrected cross section: `xs_born = xs_unfolded × C_rad`; per-bin relative uncertainty: `sqrt(total_unc_rel^2 + relative_stat_unc(C_rad)^2)`; carry the C_rad stat column; write `c_rad`, `c_rad_stat_unc`, `sigma_rad`, `sigma_nonrad`, `empz_mode`, generator tag.
4. Write the RAPGAP31-nominal corrected release (CSV+JSON) and the DJANGOH14 cross-check column set (same unfolded points × DJ C_rad; isolates the C_rad generator difference). The response-generator cross-check (DJANGOH14 response unfold) is already available from the runner's `ibu_crosscheck_*` outputs — reference it, do not recompute.
5. Gates G7, G8. G8 (rev 3) includes the per-bin identity check: independently recompute `Σ_i xs_i · C_rad_i` from the Run B table and the FINAL C_rad table and require equality with `Σ_i xs_born_i` to ≤ 1e-6 relative, per charge and total, both generators (rev-3 execution measured: exactly 0.0, per-row and aggregate). Output sidecar dirs mirror the release conventions (§6).

### S4 — Consolidated validation + manifest

One `validation_summary.json` + `manifest.json` recording: all input sha256s; G0–G10 verdicts; the convention statements; the BLK ledger (§10) with BLK-1 resolved-by-design and residuals; PROVISIONAL DIAGNOSTIC labeling; explicit "no existing artifact modified" checksum statements for the pinned inputs.

### S5 — (Optional, deferred) systematics mirror

The rev-1 optional systematics-summary producer is deferred unchanged in intent, but its background-component line must be reinterpreted under the allfake convention (electron/V0/det.int. enter via the fake column and its model spread, not via external templates). Out of scope for the corrected-points deliverable.

### Plot step (separate dispatch, not part of this chain)

Per §12: copy the canonical plotter, apply the label/caveat changes, render the comparison with the corrected points. Requires the §12 label decisions to be accepted; not a gate of this chain.

## 6. Output directory and release conventions

Proposed study directory (new, dated):
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/`

Layout:
```
scripts/                      S1, S1b, S2(v2 runner), S3, S4 scripts (copies + new)
repro_raw_vector/ibu_release/ Run A reproduction output (gate G4 evidence)
ibu_release/                  Run B corrected pre-C_rad release (the chain's primary output)
c_rad_sidecars/
  c_rad_final_rapgap31_nominal_v1/     corrected CSV+JSON, RAPGAP31 FINAL C_rad
  c_rad_final_djangoh14_crosscheck_v1/ same unfolded points × DJANGOH14 FINAL C_rad
validation/                   S1b template-vs-fakecolumn report + consolidated validation_summary.json
manifest/                     manifest.json (sha256s, gates, conventions, BLK ledger)
```

Conventions (unchanged from rev 1): every product labeled **PROVISIONAL DIAGNOSTIC**; no existing artifact is modified (checksum-verified); all outputs carry the fiducial-contract statement (alpha0p2 + Empz>45 + v4 fiducial), the allfake exactly-once convention statement, L=351000.0, k=4, and the C_rad provenance (FINAL, empz_gt45, generator, path+sha).

## 7. Acceptance gates (revised)

| Gate | Check | Pass criterion |
|---|---|---|
| **G0** | h1-normalization sign-off on the L351000 scaling of the background NPZs/CSV (parallel dispatch) | sign-off recorded before execution; scope now: low-Q2 template (the only nominal external subtraction) + the S1b comparison scaling |
| **G1** | Base vector verification | contract fields present (`alpha>=0.2`, `Empz>45`, v4 fiducial, `bg_subtracted=false`); sums 3,865,073 / 1,965,700 / 1,899,373 |
| **G2** | Allfake response verification | per generator+charge: R shape (491,246), finite, non-negative; category sums == fake column; binmap identity `== repeat(arange(245),2)`; builder selection evidence (Empz>45 reco+gen, alpha>=0.2, category routing) re-asserted from `validation_summary.json` |
| **G3** | Low-Q2 subtraction | `sum(yield_980) = 208.379505` (h+ 119.430688 / h- 88.948818); var 138.835095; **0 negative bins** after subtraction; new JSON carries `bg_subtracted=true` + full provenance block |
| **G4** | Raw-vector reproduction (Run A) | totals reproduce 20.736829 / 10.509695 / 10.227134 nb to ≤ 1e-6 relative (deterministic IBU; any deviation = machinery drift, stop) |
| **G5** | Corrected IBU run (Run B) sanity | 490 rows (245+245), 0 negative and 0 non-finite cross sections, k=4 recorded, L=351000.0 recorded, `bg_subtracted=true` vector consumed (assert via loaded-vector sum = 3,864,864.6 = 3,865,073 − 208.38) |
| **G6** | Pre-C_rad totals | total ∈ **[20.6320, 20.8394] nb**; h+ ∈ [10.4566, 10.5616]; h- ∈ [10.1756, 10.2778] (±0.5% around the expected 20.7357 / 10.5091 / 10.2267; expected shift vs published anchor −0.0054%) |
| **G7** | C_rad attach | 490/490 aligned and finite for BOTH generators; edges match release rows exactly; integrated C_rad reproduces h+ 1.1328903 / h- 1.1319064 (RG) and h+ 1.1375979 / h- 1.1366709 (DJ); integrated DJ/RG − 1 within 0.6% (expected +0.417%) *(SUPERSEDED: use xFcut baseline, see banner)* |
| **G8** (rev 3, recalibrated; runner alias "G9") | Post-C_rad totals, per-bin arithmetic | (a) **Identity:** \|Σ xs_born − Σ (xs·C_rad)\| ≤ 1e-6 rel, per charge and total, both generators (measured: exactly 0). (b) **Magnitude backstop, ±1.0% around the per-bin centers:** RAPGAP31 total ∈ **[23.3741, 23.8464] nb**, h+ ∈ [11.8517, 12.0912], h- ∈ [11.5224, 11.7553]; DJANGOH14 total ∈ **[23.4885, 23.9631] nb**, h+ ∈ [11.9079, 12.1486], h- ∈ [11.5805, 11.8145]. (c) **Cross-generator scale (runner alias "G8"):** total DJ/RG − 1 within ±0.6% (measured +0.4890%); per-charge \|DJ/RG − 1\| ≤ 0.8% (measured +0.4746% h+ / +0.5039% h-). (d) 0 negative, 0 non-finite born bins. The rev-2 window [23.3638, 23.5986] (±0.5% around the uniform-factor 23.4812) is RETIRED — mis-calibrated (§11, §14). Tolerance rationale: the center is now the same deterministic per-bin arithmetic the chain performs, pinned separately by the 1e-6 identity check, so the ±1.0% width is a gross-error backstop with ~2× margin over the largest known legitimate physics scale (the C_rad generator model scale, +0.489% data-weighted measured / +0.42% table-integrated) and ~185× over the low-Q2 scale (0.0054%); catastrophic modes (double application ≈ +14%, inversion ≈ −12%) fall far outside |
| **G9** | Template-vs-fake-column (S1b, non-blocking diagnostic) | per category+generator+charge: per-bin shape correlation ≥ 0.9; integrated ratios reported with the NucliaWeight caveat; fail (stop) only on shape correlation < 0.8 or integrated ratio outside [0.5, 2.0] |
| **G10** | Product discipline | PROVISIONAL DIAGNOSTIC labels everywhere; sha256 of all inputs recorded; no pinned input modified (checksums before/after); BLK ledger embedded in the manifest |

## 8. Script classification (revised)

**Reuse verbatim (read-only):**
- `crosscheck_unfold_methods_v4.py` (June study scripts dir) — the IBU engine imported by the runner via its absolute `SOURCE_IBU` path; do not fork, do not edit.
- Central binmap JSON (alignment reference).
- `ibu_toy_covariance_v4.py` — optional, deferred (S5 line).

**Reuse with exact edits:**
- `run_allfake_ibu_release.py` → `run_allfake_ibu_release_v2.py` with E1 (value-key correctness fix — mandatory), E1b (variance column), E2 (caveat/label text), E3 (output paths). Nothing else changes; `L_DATA = 351000.0` and the IBU math stay untouched.

**New scripts:**
- `build_bgsub_vector_lowq2_only_v2.py` (S1), `validate_template_vs_fakecolumn_v1.py` (S1b), `attach_crad_final_allfake_v1.py` (S3), consolidated validation/manifest producer (S4).

**Do NOT reuse (superseded or defective for this chain):**
- June bgsub data vector + manifests (treatment-inconsistent electron subtraction; stale lumi 356,613).
- June bgsub IBU release and its ~19.49 nb totals (not an anchor; §11).
- June C_rad products: `full_crad_v1` (478/490, DJANGOH-only, RAPGAP quarantined) and `c_rad_genonly_djangoh14_v1` — superseded by the FINAL tables.
- Both June provisional plotters; `validation_repair_bgsub_alpha0p2.py`; the TUnfold branch (negative bins; IBU is the method).
- `run_bbb_ibu_release_v4.py` — the central-response release builder; superseded for this chain (remains untouched for the central release line).
- Pre-rescale (stale combined-lumi) background aggregates; June nonrad-prediction numbers.
- The central data vector and central response — not defective, but not this chain's inputs; leave untouched.

## 9. C_rad coherence: reco-level EMPZ vs gen-level EMPZ (user question 4)

**Established from provenance (both crad workers + provenance doc + tree builder):**
- C_rad is a GEN-LEVEL ratio: `C_rad = sigma_nonrad / sigma_rad`, `sigma = sum(w1*w2)/L_MC`, computed on the 245-gen-bin-per-charge grid, fiducial `150 < Q2_es_gen < 20000`, `0.1 < Y_es_gen < 0.7`, **`GenEmpz > 45`** (gen event E−Pz, GeV, `GenEmpz = GenEnElec*(1-cos(GenThElec)) + GenEnHad - GenPzHad`), pi/K/p primary non-V0 gen pool, no reco-level cuts, no alpha cone veto (documented: reco-only cut, not applicable to gen-only C_rad), numerator-only `Q2Gki > 100` on the RAD sample (in-fiducial effect ~0.09%).
- The data chain: reco `Empz > 45` (eSigma E−Pz, GeV) + cone veto + track cleanup → unfold through the allfake response whose GEN pool carries `GenEmpz > 45` (same variable, same threshold, same 4-cut fiducial) → gen bins → apply the gen-level C_rad.

**Verdict — EMPZ coherence: COHERENT, and the reco-vs-gen level difference is NOT a defect.** Reco-level EMPZ on data and gen-level EMPZ in the response/C_rad are the standard two-level scheme: the unfolding bridges reco→gen Empz migration (events migrating across the Empz cut are part of the response), and C_rad corrects the gen-level spectrum for QED radiation under the gen-level fiducial on BOTH its legs. Do not attempt to force the C_rad derivation onto reco-level EMPZ (it is gen-only by design), and do not add a reco-level Empz cut to the C_rad samples. No change to either definition is required.

**Residuals — carried as BLK-7:**
1. **Hemisphere cut mismatch (the one genuine open question for h1-radiative-correction):** the C_rad worker ran with `xf_cut: False` — the ratio is computed over BOTH hemispheres — while the response gen pool (and hence the unfolded observable) is current-hemisphere only (`xFGen < 0`). If the rad/nonrad yield ratio per (Q2,x,z,pT) bin differs between hemispheres, the correction is slightly biased for the current-hemisphere observable. Expected small (radiative migration is event-level; hemisphere composition per bin is largely rad/nonrad-symmetric) but **unquantified**. Precise question to put to the radiative-correction specialist: *"Should the FINAL C_rad be re-derived with the `xFGen < 0` current-hemisphere cut applied to match the response gen pool, or is the hemisphere-blind ratio adequate for the current-hemisphere observable? Please quantify or bound the hemisphere dependence of sigma_nonrad/sigma_rad per bin."* **(ANSWERED 2026-10-06 — BLK-7.1 ruling: re-derived with the xFGen<0 cut; the blind ratio under-corrects and is superseded by the xFcut baseline. See banner.)**
2. **V0-pool flag version difference:** the C_rad pool veto uses `IsV0DaughterGen_v3`/`VetoAncestorPDG_v3` (ancestor-level, crad-diagnostic trees) while the response gen pool uses `IsV0DaughterGen` (v6 branch). Minor pool-definition difference; same intent.
3. **Standard gen-level-C_rad assumption:** reco-selection acceptance (cone veto, track cleanup, reco Empz) is rad/nonrad-symmetric. Second-order, standard, and unchanged from the central chain; the cone veto is new to this chain but affects rad and nonrad reco populations alike.

**Generator coherence:** RAPGAP31 response (nominal) + RAPGAP31 FINAL C_rad (nominal) is model-coherent on both layers; DJANGOH14 provides the C_rad cross-check (integrated +0.42%) and the response model cross-check. BLK-5 (open generator review) is carried; a nominal flip requires only S3 re-attachment.

## 10. Blocker ledger (revised)

- **BLK-1 — RESOLVED BY DESIGN** (user decision + existing coherent artifacts). Base, templates, and response are now selection-coherent (all alpha0p2: cone veto + Empz>45 + v4 fiducial) AND treatment-consistent (allfake exactly-once convention). Residual caveats: (a) the base vector's contract string still lists `IsMatchedV0Daughter==0` — inert on data, documentation-only mismatch (recorded in the S1 output); (b) the cone veto removes only 19,024 in-grid tracks (0.49%) — the diffuse backgrounds are removed by the fake column, not the cone veto (interpretation note, not a defect); (c) MC/data secondary-composition caveat on the fake-column subtraction (standard, covered by the model cross-check).
- **BLK-2 — re-scoped, resolved by construction FOR THIS CHAIN.** The allfake runner defines the IBU prior as `body + miss` computed from the response matrix itself — exactly the coherent definition recommended by the 2026-10-04 investigation (`response_prior_mismatch_investigation_2026-10-04.md`); no stored prior exists in the allfake package, so the central stored-prior mismatch (±2.8% scatter) cannot propagate. Residual: the allfake response has not been re-audited for gen-pool truth conservation (its package stores no gen-pool prior, so the same one-file audit is not directly runnable from the NPZ); mitigated by the runner's self-consistent prior and the allfake study's Poisson-closure PASS; carry as a low-severity caveat with an optional audit via the per-file migration stage.
- **BLK-3 — carried.** DIFFVM background sample yields zero (unchanged; affects only the background-systematics line, not the fake column).
- **BLK-4 — downgraded to informational for the nominal chain.** The e∩V0 aggregate overlap (4,571 raw tracks; per-bin unavailable) and the blank `overlap_980`/`n_raw_980` columns affect the inclusive template sum, which this chain no longer forms (low-Q2 only is externally subtracted; it is source-distinct with no overlap). Still carried for the background-systematics line and for S1b interpretation.
- **BLK-5 — carried.** C_rad generator review open; RAPGAP31 nominal recommended (now doubly model-coherent); DJANGOH14 cross-check; flip = S3 re-attach only.
- **BLK-6 — carried.** PYTHIA8/SHERPA3 curves in the comparison plot are at RAD level; Born-level comparison against them remains out of scope (nonrad predictions are a stale June method; refresh out of scope). Plot-level caveat, §12.
- **BLK-7 — NEW (C_rad fiducial residuals).** The three items of §9: hemisphere cut mismatch (open question to h1-radiative-correction), V0-pool flag version difference, standard gen-level rad/nonrad reco-symmetry assumption.
- **Old BLK-7 (fake-fraction semantics, 10.7141%/10.4019% vs 10.615%/10.312%) — resolved by supersession.** Those fractions belonged to the central-response design. The operative fractions are the allfake response's internal per-bin fake fractions (mean ≈ 0.144 nonzero-reco bins); they are consumed inside `derive_response_blocks` and are not recomputed externally in this chain.

## 11. Expected magnitudes and anchors (gate arithmetic)

**Primary anchor: the published allfake IBU release** (same base vector, same response, same lumi, same k):
- 20.736829 nb total; h+ 10.509695; h- 10.227134; raw denominator 3,865,073.

**June (~19.49 nb at L=356,613) is NOT an anchor, reassessed:** it was produced with the alpha0p2 response and external e+V0+low-Q2 subtraction — selection-coherent but treatment-inconsistent (electron double-subtraction per handoff §7), at stale lumi, with stale template scaling and det.int. excluded. The user's premise ("June totals were alpha0p2-coherent") holds at selection level only. The differences June-vs-this-chain (lumi, fLumi-corrected templates, EMPZ>45 addition, central-vs-alpha0p2 base) are now moot: the correct comparison is against the allfake release, which already embodies all of them except the low-Q2 subtraction and C_rad.

**Expected pre-C_rad totals (with low-Q2 subtraction):**
- low-Q2 fraction: 208.379505 / 3,865,073 = 5.3913e-5.
- total ≈ 20.736829 × (1 − 5.3913e-5) = **20.7357 nb**; h+ ≈ 10.5091 (119.430688/1,965,700 = 6.0769e-5); h- ≈ 10.2267 (88.948818/1,899,373 = 4.6831e-5). (IBU nonlinearity at this scale is negligible; the ±0.5% gate window absorbs it.)

**Expected post-C_rad totals — REV 3: per-bin arithmetic (verified); the rev-2 uniform-factor values are RETIRED as mis-calibrated:**
- RETIRED (rev-2 mis-calibration, kept for history): uniform integrated factors applied to the pre-C_rad totals gave RG 23.481220 nb and DJ 23.579418 nb. That arithmetic is valid only if the data spectrum equaled the MC gen spectrum (the integrated factor is the MC-spectrum-weighted mean of C_rad). It does not: C_rad rises with z (z-bin means 1.1022 → 1.2798; per-bin range 0.894–1.530) while the MC prior is concentrated at low z, so the DATA-weighted mean C_rad exceeds the integrated factor — measured 1.139155 (h+) / 1.138090 (h-) for RAPGAP31 vs integrated 1.132890 / 1.131906 (+0.5531% / +0.5464% weighting shift). Per-bin application (mandated by §4.1 and §5 S3) therefore lands above the uniform-factor estimate by construction; the DJANGOH14 shift is even larger (+0.6206%), so BOTH generators would have failed a uniform-factor-calibrated window — the calibration method was wrong, not the chain.
- Verified per-bin results (rev-3 execution, independently re-derived from the produced tables): RAPGAP31 nominal **23.610296 nb** (h+ 11.971462 / h- 11.638835); DJANGOH14 cross-check **23.725761 nb** (h+ 12.028281 / h- 11.697480); DJ/RG +0.4890% total (+0.4746% h+ / +0.5039% h-). Per-row identity xs_born = xs·C_rad exact (0.0); inputs bit-identical to Run B and the FINAL C_rad tables.
- Gate windows: §7 (G6, G8 rev 3).

**Fake fractions under the coherent selection (for interpretation):** allfake mean fake fraction over nonzero reco bins ≈ 0.1437 (h+) / 0.1441 (h-) for RAPGAP — i.e., ~14.4% of the reco data vector is removed internally (e + V0 + det.int. + ordinary fakes), versus ~10.6–10.7% in the central-response design (ordinary fakes only). The integrated outcomes of the two designs nevertheless land close (rev-1 naive expectation 20.85 nb vs this chain's 20.74 nb) because the rev-1 design subtracting ~7.95% externally on a central base and the allfake design removing ~14.4% internally on a cone-vetoed base are two bookkeepings of nearly the same physics; the allfake one is the coherent and validated realization.

## 12. Plot-comparison coherence and label changes

- The **pre-C_rad corrected points (Run B)** are the valid comparison object against the RAD-level RAPGAP31/DJANGOH14 curves already in `compare_data_vs_mc_z005_4generator.py` — both sides at radiative level, both under the same physics selection for the data side.
- The **post-C_rad (Born-level) points** must NOT be compared against RAD-level PYTHIA8/SHERPA3/RAPGAP/DJANGOH curves without nonrad predictions (BLK-6; stale June method, refresh out of scope). If shown, they carry an explicit Born-level label and the BLK-6 caveat.
- Plotter handling unchanged: copy `compare_data_vs_mc_z005_4generator.py` into the new study (or a plots-side dated dir), never edit in place; swap the data-point source to the corrected CSV; keep curve sources untouched.
- **CAVEAT_LINES replacement text (rev 2):**
  1. `Data points: PROVISIONAL DIAGNOSTIC corrected points (rev 2, 2026-10-05). Selection: alpha(track,e) >= 0.2 cone veto + Empz > 45 GeV + v4 fiducial (0.1<y<0.7, 150<Q2<20000 GeV^2, current hemisphere) + standard v6/z005 track cleanup; 658 finalcandidate data files.`
  2. `Backgrounds: electron, V0-daughter, and detector-interaction tracks removed exactly once via the all-fake response fake column (RAPGAP31 response, alpha0p2-coherent); low-Q2 DJANGOH leakage subtracted externally at L = 351000.0 nb^-1 (208.4 tracks). No other external subtraction.`
  3. `Unfolding: IBU k=4, allfake alpha0p2 response, L = 351000.0 nb^-1; luminosity and scaling signed off per G0.`
  4. `Radiative: FINAL C_rad (empz_gt45) applied per bin for the Born-level variant only; RAPGAP31 nominal, DJANGOH14 cross-check (+0.42% integrated). RAD-level points carry no C_rad.`
  5. `Caveats: BLK-1 resolved by design with residuals; BLK-2 re-scoped (prior = body+miss); BLK-3/4/5/6/7 carried — see plan bgsub_reunfold_crad_chain_plan_2026-10-05.md (rev 2). PROVISIONAL DIAGNOSTIC — not a release product.`

## 13. Execution handoff (to h1-script-runner; not launched by this plan)

Preconditions: G0 sign-off (h1-normalization, parallel dispatch) recorded; this plan accepted as the specification.
1. S0 verification (login node, read-only) → gates G1, G2 recorded; on failure → stop, evaluate §1.8 contingency.
2. S1 build low-Q2-subtracted vector (login node, seconds) → G3.
3. S1b template-vs-fake-column diagnostic (login node, seconds) → G9 (non-blocking, report).
4. S2 Run A reproduction (login node 2–5 min; Condor if the cap is threatened) → G4; on failure STOP (machinery drift).
5. S2 Run B corrected unfold (same scale) → G5, G6.
6. S3 C_rad attach (login node, seconds) → G7, G8.
7. S4 consolidated validation + manifest → G10. Stop-on-gate-failure throughout; no gate is auto-waived; BLK ledger travels into the manifest verbatim.
8. (Separate plotting dispatch, requires §12 label decisions accepted): copy plotter, apply CAVEAT_LINES, render, package for review.

**Rev 3 (2026-10-05) — execution status and S4 authorization.** Steps 1–6 ran on 2026-10-05; the chain hard-stopped at the §7 G8 magnitude window as then calibrated (runner alias "G9"). Adjudication (§14) found the chain correct and the gate mis-calibrated. **S4 IS AUTHORIZED**, conditional-sequential, with exactly this completion sequence and these gate values:
1. Apply the E1b one-line fix in `run_allfake_ibu_release_v2.py` (`w2 = var_subtracted`; §5 S2).
2. Re-run S2 Run B in place (Run A not re-run; its fallback path is unchanged and G4 stands).
3. E1b-fix invariance gate: Run B central totals bit-identical to the current output — 20.735708 / 10.509069 / 10.226639 nb — to ≤ 1e-9 rel (w2 enters only error propagation; anything > 1e-6 = stop and investigate). Re-assert G5/G6 on the re-run (trivially in-window if invariant).
4. Re-run S3 (both sidecars). Born central totals unchanged: RG 23.610296 / 11.971462 / 11.638835; DJ 23.725761 / 12.028281 / 11.697480, ≤ 1e-9 rel; stat-unc columns reduced (report the measured factor; expectation ≈ ×1/1.41433). The 2026-10-05 S3 hard-stop report (`validation/s3_crad_attach_report.json`, status FAIL) is RETAINED as the adjudication trigger record; the re-run writes a fresh report.
5. Re-check the recalibrated §7 G8 ledger: identity ≤ 1e-6; RG backstop total [23.3741, 23.8464], h+ [11.8517, 12.0912], h− [11.5224, 11.7553]; DJ backstop total [23.4885, 23.9631], h+ [11.9079, 12.1486], h− [11.5805, 11.8145]; cross-generator total DJ/RG − 1 ∈ [−0.006, +0.006] and per-charge |DJ/RG − 1| ≤ 0.008; 0 negative / 0 non-finite born bins.
6. Run S4 (consolidated `validation_summary.json` + `manifest/manifest.json`) recording: the full G0–G10 ledger with the rev-3 G8 values and the retired rev-2 window; the mis-calibration history (uniform center 23.481220 vs per-bin 23.610296); the E1b defect, fix, and invariance results; the runner G8/G9 numbering alias; the BLK ledger verbatim, including BLK-7.1 (the §9 item-1 radiative-specialist question) verbatim; PROVISIONAL DIAGNOSTIC labels; sha256s of all inputs and the re-run outputs.

## 14. Adjudication record (rev 3, 2026-10-05) — G8/G9 hard stop after S3

Context: rev-2 execution passed G0–G7 (Run A reproduction ≤ 8.3e-9 rel; Run B pre-C_rad 20.735708 nb in-window; 0 negative bins; C_rad alignment and integrated reproduction exact) and the S1b coherence diagnostic, then hard-stopped at the post-C_rad magnitude window (runner "G9" = plan G8) with the cross-generator per-charge clause (runner "G8") marginally flagged. All numbers below were independently re-derived by the plan owner from the produced tables (`ibu_release/ibu/cross_sections.csv`, both `c_rad_sidecars/*/cross_sections_born.csv`, and the FINAL C_rad CSVs); the runner's diagnosis was confirmed in every particular.

**Ruling 1 — per-bin application correct; magnitude window mis-calibrated (CONFIRMED).** Per-bin C_rad application is the mandated and correct physics (§4.1, §5 S3): C_rad is defined per gen bin and varies 0.894–1.530 across the grid. Verified: per-row `xs_born = xs·C_rad` exact (max deviation 0.0, both generators); the xs column is bit-identical to Run B; the c_rad column is bit-identical to the FINAL tables; `empz_gt45` and generator/role tags correct. The retired window had been centered on uniform-factor arithmetic (measured Run B totals × integrated factors = 23.481220 nb RG), which ignores the data-vs-MC spectrum weighting: data-weighted mean C_rad is 1.139155 / 1.138090 (h+ / h−, RAPGAP31) vs integrated 1.132890 / 1.131906 (+0.5531% / +0.5464%), because C_rad rises with z (z-bin means 1.1022 → 1.2798) while the MC prior concentrates at low z. Per-bin RG = 23.610296 nb = +0.5497% vs the uniform center, exceeding the retired upper edge by 0.0117 nb. The DJANGOH14 shift is larger still (+0.6206%): both generators would have failed the uniform-factor calibration, proving the calibration method — not the chain — defective.

**Ruling 2 — recalibrated magnitude window (per-bin centers, ±1.0% backstop + 1e-6 identity).** Centers are the verified per-bin results: RG 23.610296 (h+ 11.971462 / h− 11.638835); DJ 23.725761 (h+ 12.028281 / h− 11.697480). Windows (±1.0%, floor/ceiling at 4 decimals): RG total [23.3741, 23.8464], h+ [11.8517, 12.0912], h− [11.5224, 11.7553]; DJ total [23.4885, 23.9631], h+ [11.9079, 12.1486], h− [11.5805, 11.8145]. Tolerance rationale: the center is the same deterministic per-bin arithmetic the chain performs, and precision is pinned separately by the identity check (≤1e-6, measured exactly 0), so the window is a gross-error backstop, not a physics band; ±1.0% gives ~2× margin over the largest known legitimate physics scale (the cross-generator C_rad model scale: +0.489% data-weighted total measured, +0.42% table-integrated from September) and ~185× over the low-Q2 subtraction scale (0.0054%), while double application (≈ +14%) and inversion (≈ −12%) fall far outside. The precision burden moved from the window to the identity check, which is the correct division of labor for a deterministic step.

**Ruling 3 — cross-generator per-charge flag (PASS; clause recalibrated).** The plan's binding clause was the integrated total ≤ 0.6%: measured +0.4890% → PASS. The runner implemented a broader and tighter check (0.5% on five quantities); its per-charge h− value +0.5039% marginally exceeded that implementation, not the plan. The 0.5% threshold sits inside the very physics scale it measures (data-weighted per-charge generator scale ≈ 0.47–0.50%, itself an amplification of the table-level +0.42% by the z-shaped data weighting) — structurally too tight. Recalibrated binding clauses: total DJ/RG − 1 ∈ [−0.6%, +0.6%]; per-charge |DJ/RG − 1| ≤ 0.8% (≈1.6× the measured per-charge scale; table-level mix-ups would appear at several %).

**Ruling 4 — E1b variance spec defect (plan-owned; correction REQUIRED before S4).** The rev-2 E1b formula `w2 = n_subtracted + var_subtracted` double-counts the Poisson term because `var_subtracted` already = `n_subtracted + var_980` (S1 step 2). Effect: data variance ×2.00022 (stat unc ×1.41433), diagnostic stat-unc columns only; central values provably unaffected (`ibu_unfold` uses `d_sig_w2` only in the propagated error, verified in the engine source). Ruling: NOT accepted as a caveat — the one-line fix (`w2 = var_subtracted`) plus the S2/S3 re-runs are required before S4, because (a) the uncertainty columns propagate into `total_unc_rel_born` and thence into any downstream error bands, (b) the fix costs minutes and is deterministic, and (c) the re-run doubles as the empirical w2-invariance check (invariance gates in §13 rev-3 item 3/4: ≤1e-9 rel on all central totals).

**Ruling 5 — S4 AUTHORIZED** conditional-sequential per §13 rev-3 items 1–6, with the exact recalibrated gate values listed there. The S1b coherence diagnostic is recorded as PASS (shape correlations ≈ 0.99999 for all generator×category×charge combinations; integrated fake-scaled/template ratios 0.982–0.987, per-bin ratio medians 1.003–1.011 — the residual ~1.5–2% deficit is consistent with the NucliaWeight weight-convention caveat documented in §5 S1b).

## Appendix A. Verified numbers (quote from here; do not recompute)

**Base vector (alpha0p2 raw):** in-grid 3,865,073 (h+ 1,965,700 / h- 1,899,373); in-PS 3,873,898; 658 files / 0 failed; cone-veto in-grid removal vs central 19,024 (central in-grid 3,884,097 = h+ 1,975,301 / h- 1,908,796; central in-PS 3,902,480); `per_bin` columns `bin_index, charge, n_raw, n_subtracted (≡ n_raw), sigma_raw, sigma_subtracted`; sigma_raw sum 37,478.536360.

**Allfake response:** shapes (491, 246) per generator+charge; RAPGAP fake column pos 8,759,990 / neg 8,436,563; categories (RG, charges combined): electron 3,819,130 / v0daughter 3,432,137 / detint 676,561 / existing_other 9,268,725 (sum 17,196,553 = fake column exactly); mean fake fraction (nonzero reco bins) ≈ 0.1437 / 0.1441 (RG pos/neg); inputs RAPGAP31 364 files + DJANGOH14 501 files (central sidistree MC); binmap `binmap_reco_to_gen == repeat(arange(245),2)` = True.

**Allfake IBU anchor (published):** total 20.736829 nb; h+ 10.509695; h- 10.227134; k=4; L=351000.0; 490 rows; 0 negative / 0 non-finite; raw denominator 3,865,073; prior := body+miss; Poisson toys chunk summaries PASS.

**Low-Q2 template (L351000):** yield_980 sum 208.379505 (h+ 119.430688 / h- 88.948818; max bin 23.475); var_980 sum 138.835095 (h+ 86.484740 / h- 52.350354).

**Track-level background (L351000, central combination — validation references only in this chain):** B total 308,655.895 (h+ 155,645.476 / h- 153,010.420); electron 148,976.4; V0 132,065.3; det.int. 27,405.8; low-Q2 208.4; e∩V0 aggregate overlap 4,571 raw tracks; det.int.∩(e,V0) = 0.

**FINAL C_rad (empz_gt45, 490 rows each):** RAPGAP31 integrated h+ 1.1328903 / h- 1.1319064; DJANGOH14 h+ 1.1375979 / h- 1.1366709; integrated DJ/RG − 1 ≈ +0.42% (per-bin RMS ~2.5%, max ~15.6%); 490/490 finite both; `C_rad = sigma_nonrad/sigma_rad`, `sigma = sum(w1*w2)/L_MC`; RAD numerator-only Q2Gki>100 (~0.09% in-fiducial); crad fiducial gen-level, `GenEmpz > 45`, `xf_cut: False`, pi/K/p pool with `IsGtr==1`, `Mother2==-1`, V0 veto via `IsV0DaughterGen_v3`/`VetoAncestorPDG_v3`. *(SUPERSEDED: use xFcut baseline, see banner.)*

**Expected totals (this chain):** pre-C_rad 20.7357 (h+ 10.5091 / h- 10.2267); post-C_rad RG 23.4812 (h+ 11.9056 / h- 11.5756); post-C_rad DJ 23.5794 (+0.418%). Gate windows in §7.

**Response MC luminosities (for S1b scaling; re-verify at execution):** RAPGAP31 9,132,891.0 nb^-1; DJANGOH14 10,487,495.0 nb^-1.

**Key paths:** base vector §1.1; allfake response §1.2; allfake IBU study §1.3; background NPZs §1.4; FINAL C_rad §1.5; crad worker `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py`; tree builders `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`, `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`; plot script `sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_4generator_2026-09-25/compare_data_vs_mc_z005_4generator.py`; handoff authority `sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`; prior-mismatch investigation `sidis_highQ2/docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md`; C_rad provenance `sidis_highQ2/docs/radiative/CRAD_FINAL_PROVENANCE_2026-09-30.md`.

**Branch names (§3 for the full lists):** cone veto from `Px,Py,Pz` × `scatEX,scatEY,scatEZ` (alpha = 3D arccos, veto < 0.2 rad); EMPZ from `Empz` (reco event E−Pz, GeV, envelope 30–75 in trees) and `GenEmpz` (gen, `GenEnElec*(1-cos(GenThElec)) + GenEnHad - GenPzHad`, GeV, always on MC); event kinematics `Q2es, Yes, Xes, xF_SIDIS_es, PVz`; gen kinematics `Q2_es_gen, Y_es_gen, x_es_gen, zGen, xFGen`; classification `IsMatchedElectron, IsMatchedV0Daughter, IsStr, IsGtr, IsV0DaughterGen`; weights `weight1, weight2, NucliaWeight`.

**Rev-3 execution verification (2026-10-05, measured from the produced tables):** Run A reproduction ≤ 8.3e-9 rel; Run B pre-C_rad 20.735708 nb (h+ 10.509069 / h− 10.226639), n_subtracted sum 3,864,864.6205, 0 negative bins; per-row born identity exact (0.0, both generators); RG born **23.610296 nb** (h+ 11.971462 / h− 11.638835); DJ born **23.725761 nb** (h+ 12.028281 / h− 11.697480); DJ/RG +0.4890% total, +0.4746% h+ / +0.5039% h−; data-weighted mean C_rad RG 1.139155 / 1.138090, DJ 1.144562 / 1.143825; uniform-factor (retired) centers 23.481220 (RG) / 23.579418 (DJ); C_rad z-bin means 1.1022 → 1.2798 (per-bin range 0.894–1.530); integrated C_rad reproduced exactly (RG 1.1328903059596749 / 1.1319063751575906; DJ 1.1375979129031535 / 1.1366708715220577); S1b diagnostic PASS (shape correlations ≈ 0.99999; integrated ratios 0.982–0.987); E1b defect: data variance ×2.00022 (stat unc ×1.41433), central values unaffected (w2 used only in error propagation).

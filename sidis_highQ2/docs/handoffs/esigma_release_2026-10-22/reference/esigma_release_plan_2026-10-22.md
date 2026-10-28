# Plan: new release with the eΣ reco photon (written 2026-10-22, to execute after compaction)

**Goal.** Build a quotable, installed release with three properties:
- reco hadron z/pT built with the eΣ photon (user decision 2026-10-22);
- every uncertainty native in that frame, with the open critic items closed;
- a covariance package, canonical records and plots, and the analysis note, all consistent.

**Read first:**
- `docs/esigma_frame_production_plan_2026-10-22.md`: what was built, the gates, results, sections 7–10;
- `docs/esigma_frame_critic_review_2026-10-22.md`: open items 1–12;
- `docs/elecE_scale_study_2026-10-22.md`: why eΣ;
- `docs/final_production_plan_2026-10-21.md` §10–11: the electron-frame final re-release, user decisions D1–D4.

**House rules (CLAUDE.md):**
- Read the remote STATE.md head and grep its override sections before acting.
- Login node: one-file checks only, ≤5 min. Everything multi-file goes through Condor.
- Canonical edits must be scripted, additive and sha-guarded, made after re-reading the file, with pre-edit backups and a
  `--dry` run first.
- Peer writers (h1-sidis-e5, OpenCode) are active, so re-check sha256s right before any edit.
- Never re-raise user-dispositioned items: the NW 0.45% bias, the NW h+/h− zero, the kept double counts.

## 0. Where things are (as of 2026-10-22 ~01:00)
- **Code:** NAF `H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/`. Local copies are in
  `scripts/final_rerelease/esigma_frame/`.
  - `esigma_photon.py`: the photon helper. `SIDIS_RECO_PHOTON=electron` is the null mode.
  - `make_esph_forks.py` → 11 forks `<orig>_esph.py` next to their originals, listed in `fork_manifest.json`
    (md5 328051af…). Forks:
    - OPT-A response builder; track-class v4cat; NucliaWeight variant;
    - nominal DV; s02a event tables;
    - f1_nominal_consistent, the shifted response builder, the shifted DV builder;
    - straddle counter; e/V0 template producer; lowQ2 builder.
  - `run_prod.sh`: the production wrapper, md5-pinned. `ESPH_GATE_PHOTON=electron` is for null gates only.
  - `make_campaign_esph.py` → `prod/{big,small}.args`; `prod_{big,small}.sub` at 6 GB / 4 GB.
  - `merge_and_check_esph.py`: merges plus gates.
    - The generator side must be bitwise identical to the electron-frame nominal.
    - The zero leg must equal the nominal in every array.
  - `esph_downstream.py <step>`: steps templates, dvgate, vector, release, born, stradagg, detlegs, groupA, groupB,
    groupCcache, groupC, assemble, plots.
  - `run_post.sh <from-step>` + `post.sub` (edit `from =`): runs the chain to the end.
    - Groups A/C use LCG_107a; the merge uses the H1OO env; everything else uses `/usr/bin/python3`.
  - `plot_methods_7col.py`: photon-method comparison pages. `build_templates_esph.py`, `compare_outputs.py`,
    `smoke*.{sh,sub,args}`.
- **Outputs:** NAF `output_v6/studies/esigma_frame_2026-10-22/`.
  - `prod/` contains:
    - `opta/merged/` nominal RG/DJ;
    - `legs/merged/{gen}/{leg}` for 6 HFS + 4 electron + zero;
    - `v4cat/shards`, `nwk1/shards`, `straddle/`, `templates/partials`, `lowq2/`, `dv/`.
  - `downstream/` contains `templates/`, `vectors/`, `release/esph660`, `born/esph660`, `detlegs/`, `budget_group{A,B,C}/`,
    `budget/esph_budget.csv`, and `plots/{esph_quoted_total_23comp_7col,methods_7col}`.
  - Local pages: `plots/esigma_frame_budget_2026-10-22/index.html`, `plots/photon_methods_7col_2026-10-22/index.html`.
- **Current eΣ budget (study, not installed):** median total 8.20% (h+) / 8.80% (h−).
  - Carried verbatim from the electron-frame final budget: detint_removal(+model/stat), tracking_eff_m0p5, phi_h_gauss,
    and the per-bin ibu_model B1 floor.
  - bgsub_fakefrac is native.

## 1. Decisions needed from the user (ask once, at the start; batch them)
- **U1. ISR / QED leg** (physics B1). Method: response reweight w × (1 ± κ) for events with GenEnPhoton/E_e > 0.01;
  MC-only form; rank-1 with ρ = 1.
  - κ options: (a) from data/MC agreement of y_e/y_eΣ and of the E−pz tail (45–52 GeV) at low y; (b) from the DJ vs RG
    GenEnPhoton spectra; (c) a fiat ±X%.
  - It is a new quoted term, so it needs sign-off.
- **U2. Σ_h resolution / noise leg** (physics M1, chain 6). Method: MC-only smear of Yh by the data/MC resolution
  difference, then recompute Yes/Xes/Empz the H1Calc way. Optionally also a LAr-noise proxy (±10% of the unassociated-cluster
  energy, if that branch exists; check). New term, needs sign-off.
- **U3. JES term form.** Today the budget uses the single |jes_down| leg and the covariance uses (up − down)/2.
  Recommend the symmetrized (up − down)/2 for both, generator-averaged if RG and DJ agree within MC stat, plus a
  significance test.
- **U4. Low-Q² parity.** Switch the low-Q² analysis to the eΣ photon too, or document the difference. Separate effort.
- **U5. Install scope.** One promotion bundles every un-installed final-re-release change together with eΣ:
  - 660 files, L_eff, the consistent C_rad (DJ);
  - MC-only legs, ±1% E′;
  - unfolded env100;
  - eΣ.

  Confirm one bundled install with a swap-one attribution table, and the release name (proposal below).
- **U6.** Keep the ±0.5% E′ linearity legs out of scope? (348 shards; only the S6 linearity check uses them.)

## 2. Phase A: close the cheap open items (no new quoted terms)
- **A1. eΣ provenance.**
  - Give the merged responses and the release eΣ identifiers in sidecars, manifests and the release run-record. The
    188-key shard whitelist forbids new TNamed keys, so keep the provenance out of the shards.
  - Fix the mislabelled groupC gate `y0_vs_final660_release_PASS`: it actually compares against the eΣ release.
- **A2. ibu_model B1 floor, native** (chain 3). It binds in about 60% of bins.
  - Rerun the 100-replica bootstrap on the eΣ RG/DJ responses: `output_v6/studies/p1_replica_floors_coherentprior_2026-10-07/`,
    `scripts/b1_replica_driver.py`, `condor/submit_b1_full.condor`, `replicas_b1.txt`.
  - Inspect the driver: it was built on the no-cone v4 response/DV, so it must be re-pointed to the OPT-A eΣ chain
    (thin driver) with a null gate on the old inputs.
  - About 100 light jobs. Output: a new `b1_ibu_model_floors.json`, consumed by groupA via `inputs["b1_floor_json"]`.
- **A3. Φ_h native** (both critics).
  - The old term comes from flat-φ gen-reweighted responses in the no-cone v4 lineage:
    - `$H1/scripts/studies/flatphi_genreweight_z005_20260925/build_flatphi_response_shard_v1.py` (reco z/pT via
      `_compute_pTh_cm_z_phi`, reco call l.813);
    - the coherent-prior nominal;
    - per_generator_deltas.csv.
  - Native option: fork the OPT-A eΣ builder with the flat-φ gen weight. Port the weight function from
    `build_flatphi_response_shard_v1.py`.
  - Compute RG 73 + DJ 101 shards, then phi_h = |IBU(D_sub; M_flat) − IBU(D_sub; M_nom)|/nom, following the
    `comp_phi` convention in `components_groupB.py`. Check that convention first: which generator, what averaging.
  - Gate: with the weight set to 1, the fork equals the OPT-A eΣ shard bitwise.
- **A4. tracking_eff** (chain 11).
  - Either fork the randdrop DV builder (`output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/scripts/build_data_vector_v4.py`,
    16 seeds, about 16 jobs over 660 files, with cone + eΣ), or document the carry.
  - It is numerically inert (0.51%). Recommend the 16-job native rerun for cleanliness.
- **A5. detint_removal.** Keep it carried (median 0.03%) as a documented limitation, or run a reroute-disabled eΣ OPT-A
  build (174 shards). Recommend carry + document.
- **A6. Validation package** (chain 5, physics M2/M3); mostly 1-job studies:
  - data/MC control plots in y bins (especially y < 0.25) of Σ_h, E−pz, y_eΣ/y_e, z_eΣ/z_e, pT_h/pT_e, and the hadron share
    of Σ_h for high-z tracks in the high-x cells. Reuse the photon_options per-track inputs, plus `fit_*` for Σ_h/pT_h;
  - MC closure in eΣ: DJ pseudo-data through RG, and the half-sample split;
  - a k-scan (k = 1…10; `ibu_reg` rose 0.51 → 0.64, so check whether k = 4 is still adequate);
  - rerun the S7 prior-reweight check in the eΣ frame;
  - significance of the per-bin frame shifts: paired bootstrap using the s02a fork with both frames booked per event
    (the s02a fork exists), plus one toy job;
  - a per-track cross-check of the fork's eΣ z/pT against `photon_options_study.py` option B on one file. The two use
    different beam conventions (BeamElecE vs 27.6), so expect ~1e-3 agreement;
  - the number of xF(eΣ) vs xF(e) sign flips at z < 0.1; the lowQ2 template z-shape between frames.
- **A7. Records** (chain 4, physics M4):
  - re-specify the frame-keyed empty-D rules: EMPTY_D_BINS pin, the M1 criterion, the pos/b179 exemption, the
    straddle/bgsub guards. In eΣ there are no empty-D bins;
  - rewrite the m2_zshift "justified zero" rationale: the factorization holds, and the ISR residual belongs to the U1 leg;
  - update the C_rad contract text: the target is a radiative-level hybrid, nominal-beam q for Q²/z/pT and true x/y;
  - document that the xF cut keeps the electron-like photon.

## 3. Phase B: new legs (only after U1/U2 sign-off)
- **B1. ISR leg.**
  - Fork the OPT-A eΣ builder with `ISR_KAPPA` (MC weight × (1 + κ) where GenEnPhoton/E_e > 0.01).
  - Run RG 73 (+ DJ 101 if generator-averaged). MC-only form: IBU(D; M_κ) − IBU(D; M_nom).
  - Check whether GenEnPhoton includes FSR; the physics critic flagged the HAT bare-electron question. If it does,
    split ISR from FSR via the photon direction, if available.
  - Gate: κ = 0 is bitwise the nominal.
- **B2. Σ_h smear leg.** Size the smear from data/MC first (A6 control plots).
  - Implement it in the F1-style shifted machinery: smear Yh, recompute Yes/Xes/Empz with `esigma_kinematics`, and let
    the eΣ photon follow. It needs both an F1-fork variant and a DV variant if the data-only cross-check is wanted.
  - RG + DJ 174 shards. Gate: zero smear == nominal.
- **B3. JES form per U3:** recompute the column from the existing legs (0 jobs).
- Add every new term to the budget composition: an `esph_downstream assemble` TERMS update, with a version tag. The
  plotter's quadrature check catches a mismatch.

## 4. Phase C: budget v2 and its review
- **C1.** Rerun `run_post.sh` from `groupA`, with the B1 floor (A2), Φ_h (A3), tracking (A4) and the new legs wired in:
  - `step_groupA`: `inputs["b1_floor_json"]`;
  - `step_groupB`: replace the carried Φ_h/tracking with the native columns;
  - `step_assemble`: new TERMS.
- **C2.** A swap-one attribution table at each level: installed → final re-release (electron) → eΣ.
- **C3. Critic round 3:** implementation plus physics, on budget v2 plus validation. Proceed only on DISPATCH/SOUND.

## 5. Phase D: covariance package in the eΣ frame
- **D1. s02a**: per-file joint event tables over 660 files with the eΣ fork.
  - Driver: copy `scripts/final_rerelease/run_s02a_final.py`, switch the builder to `s02a_build_event_joint_tables_esph.py`.
  - The fork's banked-DV bitwise validation compares against electron-frame per-file DVs. Either point it at eΣ per-file
    DVs (they don't exist yet; the DV builder produced only the merged vector) or disable it, keeping the merged ==
    `dv_nominal_esph_660` gate.
  - Recommendation: gate the merged dv980 == `prod/dv/dv_nominal_esph_660.json` bitwise, plus electron-mode
    reproduction of the frozen tables on 3 files.
- **D2. s02b**: joint stat toys. Driver `run_s02b_final.py` re-pointed: merged table, eΣ RG response, eΣ vector/release,
  CLIP recorded.
- **D3. s03**: MC-stat toys. Driver `run_s03_final.py` re-pointed: re-pin RG_ROOT_SHA/RG_NPZ_SHA in common_covariance_optA
  to the eΣ merged files, plus the guard sums.
- **D4. s08/s09 package**: assemble C_total from the eΣ budget deltas.
  - Inputs: the detlegs `cov_deltas`, env100_hyp, the nonprimary/NW rank-1 vectors from groupC, and the new legs as
    rank-1 or their own form.
  - Then the v5/NW editions, or fold them in directly. The edition gates g0–g7 + ga10 are incremental: build a BASE
    package with its own gates (s03 closure, PSD, diagonal == budget², sha sweep) and do not compare to the electron base.
- **D5. h+/h− and A_h columns** from the covariance: cell-level ratio uncertainties, the groupC ratio terms, plus the
  detector/IBU/stat cross-charge terms.

## 6. Phase E: release candidate
- **E1. Names.** Raw release `bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphoton_..._2026-10-XX`; Born release
  with C_rad (DJ, consistent tables); budget `systematics_..._esigmaphoton_2026-10-XX`; covariance edition
  `z005_covariance_optA_esigmaphoton_2026-10-XX`. Keep the existing naming scheme and verify it against current-release.
- **E2. Package** into `output_v6/studies/...` first:
  - release, Born, budget CSV (all columns + total), covariance, binmap;
  - INSTALL_RECORD draft with every sha.
- **E3. Critic round 4:** a hostile review of the full candidate (release + budget + covariance + records). Then user sign-off.

## 7. Phase F: install (user-gated)
- **F1.** Use the s11/s14 pattern: `s1X_install` (Condor) copies the products into `output_v6/central/data_release/` and
  `data/covariance/`. Then `s1X_promote` applies the sha-guarded canonical edits:
  - STATE override section, RUN_STATUS, current-release, context-handoff, RUNLIST, script index;
  - skills: builder, unfolding, systematics, backgrounds, radiative, plotting, aziasym, lessons, state;
  - ownership/coverage.
  - Pre-edit copies go to `docs/promotions/<name>_preedit/`. `--dry` first. Re-read every file right before editing.
- **F2.** Promotion record `docs/promotions/esigmaphoton_promotion_2026-10-XX.md`:
  - the swap-one attribution;
  - user decisions D1–D4 (2026-10-22) plus U1–U6;
  - critic verdicts;
  - limitations: carried detint; xF electron-like; the low-Q² difference.
- **F3.** Post-install audit critic. Fix any text conditions with an additive `s1X_fix` script.

## 8. Phase G: plots and note
- **G1. Canonical systematic plots.** A new additive edition under `sidis_highQ2/plots/systematics/` produced by the
  label-fix producer through a thin driver (as in `esph_downstream.step_plots`). Also re-point P1/P6 (the Born plotter
  must read the installed release) and re-render the overlays.
  - Do not create a replacement canonical plotter without user approval.
- **G2. Analysis note:**
  - photon definition (q from eΣ; motivation from the elecE study);
  - the unfolding section: purity, migration, closure, k choice;
  - the systematics section and figures; the ISR/Σ_h legs; the hybrid target definition;
  - control plots: re-render `reco_kinematics_dist_2026-10-08`, `reco_multiplicity_note_2026-10-07`, and the plotting
    residual figures with the eΣ photon.
  - State Born vs raw for every number.

## 9. Pitfalls learned in this session
- Re-point modules only AFTER anything that must stay in the old frame has run, or carry such columns explicitly. This
  session's phi_h/tracking hybrid bug came from re-pointing first.
- `components_*` import path constants by name. Re-point every namespace: the common module, the runner, and
  `RESPONSE_PATHS`.
- The release preflight needs `merged_response_sha256.txt` and `merge_manifest_{gen}.json` in the merged dir.
- The OPT-A benign-mismatch certification is count-exact: 3 rows in RG 0008/0015/0069. It still holds in eΣ.
- An IBU null gate reproduces to ~1e-16 only in the same numpy build. Group A/C use LCG_107a; the release ran with
  `/usr/bin/python3` and matched final660 to ≤6e-16 (model_unc ≤9e-11).
- Condor: `queue args from` splits on commas. Use `executable = /usr/bin/python3` instead of bash -c with quotes.
  `on_exit_hold` is set.
- Python's `all()` treats a gate value of 0 as a failure. Compare counts explicitly.
- Login-node zsh: use `ssh … 'bash -s' <<'EOF'` for anything with variables. Never `pkill -f` a pattern that matches
  your own command.

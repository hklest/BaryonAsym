# Quoting-blocker audit: M1, N2, P-CRAD-LUMI (eSigma release, 2026-10-24)

Scope: this audit checks whether the three items that `sidis_highQ2/docs/handoffs/esigma_release_2026-10-24/START_HERE.md:97` lists as blocking quoting of the eSigma-photon release (installed 2026-10-23) are actually still open. It is read-only: nothing was edited, run or submitted. All paths below are relative to `/data/dust/group/h1/klesthen/BaryonAsym/` unless they are absolute.

## Summary (revised 2026-10-24 after the follow-up; supersedes the first-pass verdicts)

| Item | Verdict for the eSigma quoted budget | Confidence | Formal record |
|---|---|---|---|
| M1 env100 builder | **Closed in practice (superseded).** The eSigma env100 terms are recomputed by a recorded builder that does not read the unlocatable 10-19 CSV. `esph_downstream.py` is pinned (INSTALL_RECORD L141), but the module that computes env100, `components_groupC.py` (mtime 10-22 16:45), is not pinned. | Medium | Still listed OPEN. No re-scoping note. The literal M1 fix was never done. |
| N2 HFS E−pz | **Mechanically wired; closure pending.** The HFS columns trace to the shimmed `N2_MODE=corr` legs, and the shim is valid for the cut actually applied. D2 (the E′ > 11 GeV cut on shifted legs) has no disposition, and the effect size was never measured. | High (mechanics); medium (closure) | Still listed OPEN. No critic or user closure. |
| P-CRAD-LUMI | **Partially resolved; genuinely open.** The file sets are reconciled, but the generator-log check is blocked externally, and the DJ closure discrepancy is recorded but not dispositioned. | High | OPEN, correctly. |

Answer to "are you positive we didn't do this?":
- **M1 and N2.** The work was done inside the eSigma production on 2026-10-22/23, but nobody wrote the closure down. Every status file (`sidis_highQ2/context-handoff.md:571`, `sidis_highQ2/STATE.md:402,447`, `.supervisor/memory/facts/2026-10-23-esigma-release-installed.md:14`, `START_HERE.md:97`) copies the 10-21 blocker list forward unchanged.
- **P-CRAD-LUMI.** This one is not done. Closing it needs a user decision, not computation.

The first-pass "M1 OPEN" verdict was correct for the formal record but wrong for the eSigma numbers. The literal M1 acceptance test still applies to the historical OPT-A, v5 and nw budgets.

## M1: env100 builder (first pass; see the follow-up below)

**Verified facts.**
- **Definition.** The builder that produced the 2026-10-19 env100 dated-budget CSV cannot be located. The manifest pins a script that did not produce that CSV, and no run record or registry entry exists. Sources: `sidis_highQ2/docs/critiques/scriptaudit_2026-10-19/loop3_systematics.md:20,40,83`.
- **Acceptance test.** The regenerated total must equal `total_with_bgsub19_rel` exactly on bins pos/109, pos/179 and neg/240. Sources: `sidis_highQ2/context-handoff.md:425` and `sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/reference/HANDOFF_2026-10-21.md:61-70`.
- **2026-10-20.** The C1 detint fix was installed (`H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100_optA_c1fix_2026-10-20/`). The README of that install, at line 5, says: "OPEN (not closed here): M1 ... QUOTING REMAINS BLOCKED". `context-handoff.md:443` says the same.
- **2026-10-21.** The v5 and NucliaWeight installs close no blocker (`context-handoff.md:480`, `:525`). `HANDOFF_2026-10-21.md` lists "Step 2: M1" as still to be done.
- **2026-10-23 and 2026-10-24.** M1 is still listed as open: `context-handoff.md:556,571`, `.supervisor/memory/facts/2026-10-23-esigma-release-installed.md`, and `START_HERE.md:97`.
- **Git.** Neither repository has a commit that rebuilds the builder. The note-repo commits `adf7bac`, `52e29e8`, `dcd2abc`, `2b959f7` and `c45c3ef` (2026-10-19/20) are figure and text updates for the env100 freeze.

**Inference.** The user's belief probably comes from the 2026-10-20 C1 fix. That fix is called a "reconstruction path" (`sidis_highQ2/docs/critiques/scriptaudit_2026-10-19/detint_uncertainty_review_2026-10-20.md:70`), but it was explicitly not the M1 closure.

**What remains.**
1. Rebuild the builder as a thin appender over the 19-component CSV and `bgsub_env100_term_490_2026-10-18.csv`, with detint entered once.
2. Pin its sha, add a run record, and add registry entries in RUNLIST and SEVEN_CATEGORY_SCRIPT_INDEX (both copies).
3. Run the three-bin exact-match test.
4. Re-check the v5 `env100_hyp` column and the eSigma unfolded env100 term (D4, `context-handoff.md:564`).

**Cost.** The handoff calls this "mostly provenance" and expects no change to the numbers. No job count is stated anywhere. It is CSV arithmetic on existing outputs, so a login-node session of minutes to an hour should suffice (inference); no Condor jobs should be needed.

## N2: HFS E−pz channel (first pass; see the follow-up below)

**Verified facts.**
- **Definition (raised 2026-10-19).** The v6sys HFS shift acts only in gH1Calc, so Empz stays identical on the jes, rces and theta legs. Migrations across the Empz>45 cut are therefore missing, and the HFS errors under-cover. Sources: `sidis_highQ2/docs/critiques/scriptaudit_2026-10-19/loop2_treebuilder.md:40` and `loop3_treebuilder.md:55`.
- **Fix (2026-10-21).** The fix is the shim `empz_hfs_shim.py`, which applies Empz → Empz + 2E_e(Yh_leg − Yh_nom). The same work found a second defect: the HFS legs used 660 data files against 658 nominal. Source: `sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/reference/n2_hfs_empz_2026-10-21.md`. Its status line then read "campaign READY, production not yet submitted".
- **Use in eSigma (checked directly for this audit).** `H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/run_prod.sh`:
  - lines 15-16 pin the shim, and the hfs case at lines 32-34 runs the HFS legs through `run_with_empz_shim.py` with `N2_MODE=corr`;
  - it reads the recovered nominal set from `hfs_empz_n2_2026-10-21/`.
- **Shim output.** There are 1044 `*.n2shim.json` sidecars under `H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/prod/legs/shards/`. An example is `djangoh14/hfs_jes_up/djangoh14_0000_hfs_jes_up.root.n2shim.json` (`"mode": "corr"`, with nonzero `delta_empz` and `n_events_flip_in`).
- **Budget.** The eSigma budget `.../downstream/budget/esph_budget.csv` (Oct 23 00:48) carries `hfs_jes_rel`, `hfs_rces_rel` and `hfs_theta_rel`.
- **No closure record.** No document, memory entry or commit marks N2 closed. The "N2 HFS-Empz" text in `context-handoff.md:556,571`, the memory fact and `START_HERE.md:97` is carried forward from the 2026-10-21 OPT-A list (`context-handoff.md:480,525`).

**Inferences, not verified.**
- Context: the eSigma detector legs are MC-only (decision D1, `sidis_highQ2/context-handoff.md:564`), so the 658-vs-660 data-file part of N2 may be moot for eSigma.
- I did not trace column by column that the budget's HFS columns are built from these shim legs.
- The shim formula was derived for the electron-frame Empz. Nobody has checked that it is valid when the eSigma builder computes Empz.
- Whether the electron-only (v5) release ever got an N2 budget edition is not established.

**What remains.** Everything below runs on the login node, in minutes:
1. Confirm the budget lineage and the validity of the shim in the eSigma frame.
2. Record a critic or user closure.
3. Correct the "blocked" text in START_HERE, STATE, RUN_STATUS and the handoff.

Condor would be needed only if the lineage check fails. The 2026-10-21 estimate for a rerun is 1044 shard jobs of about 20 s each (about 6 CPU-h), plus 12 data-vector jobs and one merge.

## P-CRAD-LUMI: C_rad and MC exposure luminosity (first pass; see the follow-up below)

**Verified facts.**
- **Definition.** The "lumi" in this item is the per-leg MC exposure of the DJ14 and RG31 rad/nonrad samples that enters C_rad = σ_nonrad/σ_rad. It is not the data luminosity. Sources: `sidis_highQ2/docs/normalization/lumi_2p7_derivation_memo_2026-10-10.md` §1 and `sidis_highQ2/docs/RUN_STATUS.md:274`.
- **First raised.** The item dates from about 2026-06-09 (`sidis_highQ2/docs/research/research-crad-lumi-pilot-bunch-zvertex-2026-06-09.md`).
- **Electron-only Born release (2026-10-09).** The item was relabelled FILE-SET-RECONCILED (generator-log-open). All six legs are closed at file-set level, and DJ14 RAD matches exactly for 1373/1373 files. Sources: `context-handoff.md:236,336` and `sidis_highQ2/docs/radiative/CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md`.
- **What stayed open on 2026-10-09:**
  - The generator-log cross-check is "permanently blocked externally" because the MCDB CGI is disabled.
  - The RG31 two-valued σ̂ residual is bounded at −0.44%/+0.26% on per-beam C_rad.
  - These are carried open at `context-handoff.md:351,376`. `sidis_highQ2/STATE.md:86` (2026-10-16) says "still OPEN ... C_rad stays PROVISIONAL / not publication-grade".
- **2026-10-21.** A consistent C_rad was rebuilt. The physics critic then found that the DJ gen-only samples do not close against the detector-MC truth: +1.65% integrated, and −2.6/−4.0/−6.8% at z = 0.2/0.3/0.5. It recorded this as MAJOR (documentation). Source: `critic_phys_final_production_2026-10-21.md`, row 8. No later disposition of that finding was found.
- **eSigma (2026-10-23).** No C_rad was recomputed; the same releasebin tables are applied. `context-handoff.md:556` says P-CRAD-LUMI "stays open; this promotion closes none of them". The eSigma Born tables (`analysis_note/.../tables/highQ2_z005_esigmaphoton_born_public_2026-10-23/*.tex`, line 16) still say "C_rad is NOT publication-grade". Decision U1 (no separate ISR/QED term, covered by the applied C_rad) makes the release depend on this item.
- **Data luminosity (a different item, resolved).** L_eff = 350,890.85 nb⁻¹, "RESOLVED 2026-10-22" (`sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/reference/final_production_plan_2026-10-21.md:30,146`). Its 2.7% uncertainty is a documented value taken from arXiv:2403.10134 §6.3, not a measurement for this dataset. The V0 memory decision that reuses 2.7% "as high-Q2" relies on this documented value.

**Inference.** The user's belief probably mixes together the 2026-10-09 file-set reconciliation, the 2026-10-22 data-lumi resolution and the 2.7% decision. None of them lifts P-CRAD-LUMI. For eSigma, the physics critic ruled that C_rad needs no rerun because it is applied on the gen target, which the eSigma release does not change (`esigma_frame_critic_review_2026-10-22.md:9`). So eSigma kinematics add no new C_rad blocker; the blocker is the pre-existing provenance ceiling.

**What remains.**
1. **User decision (documentation only).** Accept the "file-set-reconciled, generator-log-open" ceiling as final, since the MCDB check cannot be done locally.
2. **Login node.** Disposition the DJ gen-vs-detector-truth closure discrepancy, either reconciled or caveated.
3. **Not required.** About 4,730 Condor jobs would be needed only if the gen target were switched to a true-photon target (`esigma_frame_critic_review_2026-10-22.md:14`); that is not planned.

## Follow-up (2026-10-24): filesystem provenance and the eSigma budget

The quoted budget is `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphoton_2026-10-23/esph_budget_v2.csv` (Oct 23 05:07). Its total is `total_quoted_esph_rel` = sqrt(sum of the terms below). It is built by `step_assemble` in `H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/esph_downstream.py` (TERMS at L301-307, total at L316 and L339, CSV written at L365). Two pins are recorded:
- the `budget_v2_sha256` in `INSTALL_RECORD_esigmaphoton_2026-10-23.json:12-13`;
- an sha assert in `s15_install_esigmaphoton_2026-10-23.py:96-100`.

**M1.** No reconstructed 10-19 builder, appender, run record or registry override exists. A find over `sidis_highQ2/` and `H1ChargedSidis_HighQ2/scripts/` for files newer than 2026-10-19 returned nothing; the full-tree find timed out at 240 s. However, the eSigma budget does not depend on the 10-19 builder:
- **Source of the env100 columns.** The `bgsub_env100_hyp_rel` and `bgsub_env100_rel` columns come from `step_groupC` (`esph_downstream.py:281-298`) through `H1ChargedSidis_HighQ2/scripts/unfolding/final_rerelease_2026-10-22/budget_groupC/components_groupC.py`. That script recomputes env100 as a fully unfolded joint re-unfold (L19-25; decision D4 of 2026-10-22).
- **The unlocatable CSV is not an input.** A grep of `components_groupC.py` and `build_mc_cache_groupC.py` for `term_490` or `bgsub19` finds nothing; I checked this directly.
- **Gate-only reads of the old lineage.** The installed 10-21 budgets (`BUDGET_NW` and `BUDGET_V5`, L83-84) and the optional `env100_reco_ratio` feed only the gate and `*_mixed` comparison columns.
- **Inferred, not traced.** The MC cache contents were not traced file by file.

What remains for M1:
- A documentation-only re-scope note plus registry lines (RUNLIST and SEVEN_CATEGORY_SCRIPT_INDEX, both copies) pointing at the esph shas and at the groupC sha, which still needs to be pinned. Login node, minutes.
- The literal M1 fix matters only for the historical OPT-A, v5 and nw budgets.
- Optional bitwise re-run of groupC: one Condor job.

**N2.** The chain is wired as follows (the jes link was not read):
1. **Shards.** `run_prod.sh:32-34` runs the shards in mode `corr`. There are 1044 sidecars, 174 per leg across 6 legs. They show nonzero flips, e.g. `hfs_jes_up` flip_in 24559.
2. **Merged legs.** The shards merge into `prod/legs/merged/*/hfs_*/merged_response_*.npz` (10-22 23:45).
3. **rces and theta columns.** `step_detlegs` (`esph_downstream.py:129-178`, MC-only per D1) uses the merged legs and writes `downstream/detlegs/detlegs_mconly_esph.json`. That file fills `hfs_rces_rel` and `hfs_theta_rel` (L163-164).
4. **jes column.** `hfs_jes_rel` comes from `phaseB/jes_sym/jes_sym_esph.json`, built from the same merged legs. The final linking step (in `extract_shard_R.py`) was not read.

The shim validity in the eSigma frame was also checked:
- The eSigma migration builder cuts `Empz > 45` on the electron-plus-HFS `Empz` branch, which the shim modifies.
- `esigma_photon.py` does not recompute Empz.
- The shim adds ΔΣ_h = 2E_e ΔYh (`empz_hfs_shim.py:63-90`), so the shim is frame-independent.

What remains for N2:
- A one-paragraph critic or user closure that the shimmed MC-only legs resolve N2, D1 and D2. D2 (the E′ > 11 GeV cut on shifted legs) has no recorded disposition. Login node, about 30 minutes.
- Measuring the size of the N2 effect, if wanted: null-mode against corr-mode legs, Condor, hours.

**P-CRAD-LUMI.** The C_rad terms come from `final_rerelease_2026-10-22/budget_groupB/components_groupB.py`:
- `c_rad_gen_rel` = 0.5 × smooth5(|RG−DJ|/DJ) and `c_rad_stat_rel` = the DJ statistical uncertainty (L240-262, L481-483);
- inputs: `output_v6/studies/final_crad_consistent_2026-10-21/tables/{djangoh14,rapgap31}/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`;
- the DJ table is applied to the eSigma Born values, with `"c_rad": "not rerun"` in `esph_provenance.json`.

No record after 2026-10-09 closes the generator-log ceiling. The DJ closure discrepancy (+1.65% integrated, −6.8% at z = 0.5; `critic_phys_final_production_2026-10-21.md:27`) is only restated in `esigmaphoton_promotion_2026-10-23.md:45` (D3), not dispositioned.

What remains for P-CRAD-LUMI:
- A user decision to accept the ceiling, and to accept that the RG−DJ spread covers the DJ closure gap or else caveat it. Documentation only.
- A DJ steering-comparison check, if wanted: Condor.

**Lumi.** `lumi_rel` is a hard-coded 0.027 (`components_groupB.py:8,67,501`), exactly constant over all 490 bins (checked). The 2.7% comes from `sidis_highQ2/docs/normalization/lumi_2p7_derivation_memo_2026-10-10.md` §2 (arXiv:2403.10134 §6.3). It is frame-independent by construction.

### Which frame each quoted uncertainty is evaluated in

The terms below are all the terms in `total_quoted_esph_rel`, per `esph_budget_v2_summary.json` (`term_sources`) and `esph_downstream.py` L301-307. Medians are of |value| over all 490 bins. STUDY = `H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/`.

| Term | Frame | Median % | Evidence |
|---|---|---|---|
| ibu_stat, ibu_model, ibu_reg, fakecol_mcstat | eΣ | 3.91, 1.51, 0.61, 0.33 | `esph_downstream.py:210-232`; STUDY `downstream/budget_groupA_v2/groupA_esph.json`; the ibu_model floor comes from the eΣ A2 replicas |
| hfs_jes | eΣ (MC-only, (up−down)/2) | 0.62 | STUDY `phaseB/jes_sym/jes_sym_esph.json`; L303 |
| hfs_rces, hfs_theta | eΣ (MC-only, N2-shimmed) | 0.15, 0.11 | STUDY `downstream/detlegs/detlegs_mconly_esph.json` |
| elec_energy (±1%), elec_theta | eΣ | 2.75, 0.28 | same file; the ±0.5% linearity legs are excluded (U6) |
| sigmah_smear | eΣ (new, U2) | 0.44 | STUDY `phaseB/sigmah_smear/sigmah_smear_mconly_esph.json` |
| tracking_eff | eΣ native (was carried until a critic fix) | 0.51 | L262; STUDY `phaseA/A4_trackeff/tracking_eff_native_esph.csv` |
| phi_h_gauss | **eΣ native**; the legacy 1.21/1.32% no longer enters | 1.27 (range 0.10-22.2) | L261; STUDY `phaseA/A3_phih/phi_h_native_esph.csv` |
| bgsub_model, bgsub_stage, bgsub_fakefrac | eΣ (eΣ vector `esph660`, eΣ f_i) | 0.23, 0.19, 0.18 | L239-251 |
| straddle_residual | eΣ | 0.13 | STUDY `prod/straddle/aggregate/ab_decomposition_join.csv` |
| bgsub_env100_hyp | eΣ (unfolded joint re-unfold, D4) | 3.67 | L281-298; `groupC_new_columns.npz` |
| nonprimary_norm_v5, nucliaweight_primaryloss | eΣ | 0.06, 0.76 | same |
| lumi | frame-independent (flat) | 2.70 | `components_groupB.py:8` |
| **c_rad_gen, c_rad_stat** | **electron-chain, inherited** (gen-level tables, not rerun) | 0.57, 0.43 | `components_groupB.py:240-262`; promotion record L9, L22 |
| **detint_removal** | **electron-chain, carried**, plus an eΣ-assembled ±100% normalisation term | 0.18 | L222-228 (`carried` from `final_budget.csv`, L195); promotion record §6 |
| m2_zshift | identically 0 in every bin (checked) | 0 | promotion record §6; U1 |

Binning is identical: 245 cells per charge, and `step_assemble` asserts `cell_label` and `z_idx` equality, so the inherited terms are joined bin by bin.

**Bottom line (high confidence on the classification):**
- No, the quoted uncertainties are not still for the electron-method photon. Every large term is evaluated in the eΣ frame, including Φ_h, whose legacy 1.21/1.32% was replaced by a native eΣ value.
- Only C_rad (gen and stat, about 0.4-0.6%) and the detint removal (about 0.2%) are inherited from the electron chain.
- The critic ruled that C_rad needs no rerun because it is a gen-level correction whose gen target is unchanged (`esigma_frame_critic_review_2026-10-22.md:9`). No critic has ruled that the C_rad uncertainties are frame-invariant (medium confidence that they are adequate).
- Outside the budget, the x_F cut still uses the electron-like photon. That is a pending user decision.

## Coverage gaps
- The M1 builder-script mtimes and the post-2026-10-20 RUNLIST bodies were not read.
- `CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md` was read only through the quotes of it in RUN_STATUS, STATE and the handoff.
- The N2 jes link (`extract_shard_R.py`) and `repoint_templates()` (behind the eΣ classification of the env100 term) were not read.

# Context Handoff: H1 High-Q² Charged-Particle SIDIS

---

## Current State — z005 Systematics, C_rad, and Alpha0p2 Response Diagnostics (2026-06-28)

This workspace covers only the **high-Q² inclusive charged-hadron SIDIS** analysis. It is not the proton/PID/V0 workspace, and results must not be mixed with `proton_HiQ/`, `lambda_HiQ/`, or low-Q² SIDIS.

Read `docs/RUN_STATUS.md` first for the single-source current run status. The current central high-Q² charged-SIDIS production is v6, with central work under `condor_v6/central/` and `output_v6/central/`; v5g and rev6-era products are historical/audit unless explicitly requested.

Current z005 release state:
- Central z005 working release: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`.
- z005 IBU/BBB detector, unfolding, and luminosity systematics are essentially complete.
- DJANGOH14 gen-only `C_rad = sigma_nonrad/sigma_rad` v1 remains the stable provisional radiative-correction baseline.
- RAPGAP31 old hardcoded aggregate-lumi `C_rad` result is superseded/quarantined for diagnostics only; the anomaly came from wrong aggregate-lumi normalization, not from selected yields/weights.
- Correct/promising RAPGAP diagnostic convention is the per-chain/group `fLumi` map (`fLumi-v3`): RAD `8947-8954 -> 15810.87`, NONRAD `EPLUSP` A-H `-> 13987.01`, NONRAD `EMINUSP` A-H `-> 13836.73`, with `sigma_rad(bin)=Σ_i sumw_i/L_i`, `sigma_nonrad(bin)=Σ_j sumw_j/L_j`, and `C_rad=sigma_nonrad/sigma_rad`. Do not use old global RAPGAP constants.
- Final RAPGAP physics inclusion remains blocked by provenance closure, schema/sparse-bin QA, and final systematic treatment; final release remains blocked by sparse-bin policy, publication-grade DJANGOH MC-luminosity provenance, RAPGAP decision/exclusion, nominal-method choice, final plots, and near-electron alpha-veto/background response-consistency validation.
- The reco-only `alpha<0.2` scattered-electron cone cut must **not** be applied to gen-only `C_rad`.
- Alpha0p2 reco-background/all-fake response handoff: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`; all-fake response products are diagnostic and central release/response products were not overwritten.
- For current Sherpa/Sherpa3 MC-operational context (valid S2 canonical-gen comparison, Sherpa2 card finding, Sherpa3 expert-card MEPS smoke status), read `MC/CONTEXT.md`; keep detailed MC execution notes there, not in this main handoff.
- Canonical script snapshot and reproduction run list for the current high-Q² z005 release are saved at local `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/` and remote `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/`; read `RUNLIST.md` there before attempting a full release reproduction.

## C_rad Reproducibility Snapshot — Gen-Only v1 (2026-06-24)

DJANGOH14 gen-only `C_rad` v1 is the stable provisional high-Q² z005 baseline, not final publication-grade until sparse-bin treatment, publication-grade DJANGOH MC-lumi provenance, and RAPGAP disposition are closed. Definition: `C_rad(bin) = σ_nonrad(bin) / σ_rad(bin)`. Normalization: `σ ∝ Σ(weight1*weight2)/L_MC`; data luminosity does not enter gen-only `C_rad`.

| Item | Value |
|---|---|
| Study root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/` |
| Builder/source/runner | `src/RC_treebuilder_genonly_v1.C`; `bin/x86_64-el9-gcc14-opt/RC_treebuilder_genonly_v1`; `scripts/genonly_condor/run_genonly_entry_direct.sh` |
| Outputs | `output/tables/full_crad_v1/`; `output/tables/full_crad_v1/full_crad_v1_summary.json`; `output/tables/validation/validation_rc_genonly_v1.json` |
| DJANGOH MC lumi | rad `4,108,957 nb^-1`, nonrad `3,560,233 nb^-1`; trace to MCDB/generator logs before publication |
| DJANGOH summary | h+ integrated `1.055329789`, h− integrated `1.055413655`, finite `239/245` per charge and `478/490` total, all-charge median `1.078836633` |

Stat caveat: median relative stat uncertainty is below `0.5%`, but p95 is about `30–33%` and max about `104–110%`; sparse bins need an explicit marking/merging/smoothing policy. Prior RAPGAP diagnostics to preserve: raw weighted no-lumi ratios were `≈0.9018` RAPGAP and `≈0.9145` DJANGOH; the old hardcoded RAPGAP aggregate ratio `L_rad/L_nonrad = 5293265/3397124 = 1.55816` drove `C_rad ≈ 1.405`; unweighted RAPGAP still gave `≈1.412`; Empz removal moved in the right direction only partially and tightening worsened, so Empz is diagnostic-only and not a fiducial fix. The `alpha < 0.2` scattered-electron cone cut is reco-only and must not be imposed in gen-only `C_rad`; the alpha0p2 gen-electron `C_rad` path was canceled/not adopted.

### RAPGAP fLumi-v3 Diagnostic Save Point (2026-06-25)

RAPGAP31 is no longer treated as an unexplained anomaly. The likely normalization fix has been identified and a promising diagnostic candidate produced, but it is **not** final physics and the old hardcoded aggregate-lumi result remains quarantined.

| Item | Value |
|---|---|
| fLumi map | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/diagnostics/rapgap_fLumi_diagnostic_2026-06-24/fLumi_map_v2.json` |
| Full output root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/diagnostics/rapgap_fLumi_diagnostic_2026-06-24/full_nonrad_map_v3_with_unc/rerun_mapping_fixed/` |
| Key files | `diagnostic_summary_v3.json`, `diagnostic_bins_v3.csv`, `tail_bins_v3.json`, `mapping_rule_v3.json` |
| fLumi convention | RAD sample IDs `8947-8954`: `15810.87`; NONRAD `EPLUSP` A-H: `13987.01`; NONRAD `EMINUSP` A-H: `13836.73` |
| Formula | `sigma_rad(bin)=Σ_i sumw_i/L_i`, `sigma_nonrad(bin)=Σ_j sumw_j/L_j`, `C_rad=sigma_nonrad/sigma_rad` |
| Integrated `C_rad` | h+ `1.0248113`, h− `1.0249101` |
| Finite rows | `476/490` |
| All-charge median | `1.0780425` |
| Out-of-range `[0.5,1.2]` rows | `27` |
| Median relative stat uncertainty | `0.4527%` |
| Zero-denominator rows | `14` |
| Validation status | `PROVISIONAL_ONLY`; integrity `INTEGRITY_PASS_WITH_CONCERNS`; QA `QA_CONCERNS` pending schema cleanup, tail-bin audit, and provenance closure |

Use note: the same unit convention is used across the `fLumi` map, so ratios are unit-invariant when applied consistently.

### C_rad 7-Column Regeneration Pointer (2026-06-25)

For the current DJANGOH-vs-RAPGAP diagnostic comparison after cut changes:
- Local script: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/radiative_corrections/plot_crad_djangoh_vs_rapgap_fLumi_7col.py`
- Remote script: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/plotting/plot_crad_djangoh_vs_rapgap_fLumi_7col.py`
- Remote plots: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/plots/crad_djangoh_vs_rapgap_fLumi_v3_7col_2026-06-25/`
- Local plots: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/radiative_correction_genonly_v1/crad_djangoh_vs_rapgap_fLumi_v3_7col_2026-06-25/`
- Plot files: `crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hplus.pdf/png`, `crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hminus.pdf/png`, `manifest.json`
- Style freeze: 7-column layout; legend labels only `Djangoh` and `Rapgap`; Djangoh blue, Rapgap orange; single legend in the top-left-most panel at lower-right; panel text only bin kinematics plus legend; y-range `[0.25,1.75]` except highest-z row `[0.0,2.0]`.

Future regeneration after cut changes:
1. Rerun gen-only partials for the new cut.
2. Recombine RAPGAP with the `fLumi` map per partial, not with old hardcoded aggregate lumis.
3. Recompute `sumw2`, counts, and statistical uncertainties.
4. Rerun the 7-column plot script against the DJANGOH table and the RAPGAP `diagnostic_bins_v3.csv` table.
5. Keep the old hardcoded RAPGAP aggregate result quarantined.

## Near-Electron / Alpha-Veto Study Save Point (2026-06-24)

This save records the high-Q² z005 near-electron track contamination study in inclusive charged-SIDIS. It studied `alpha(track,e)` opening-angle vetoes and true `e±` versus true `π/K/p` composition. Treat all outputs as **diagnostic / QA / analysis-note candidates** only. No central cut, subtraction, or correction has been finalized.

Key conclusions:
- The apparent `80%` removal was a misread of normalized/control/composition plots. Validated data track-level `alpha<0.1` removal max is about `36.6%` in global bin 24; next-largest bins are about `33.7%`, `33.7%`, and `30.7%`.
- Worst-bin MC removed population for `alpha<0.1` is overwhelmingly true `e±` (`96–99%`) with only about `0.5–3%` true `π/K/p`, but data removal fractions exceed MC by factors of about `1.8–2.6` in hot bins.
- Grebenyuk's H1 charged-particle precedent used track–scattered-electron angle `>0.2 rad` to suppress electron showering/conversion tracks; HFS excludes isolated electrons/muons. Momentum matching is not the H1-standard-looking solution.
- Momentum windows in `p_track/p_e` are not promising: `±50%` or `±25%` with alpha cuts removes almost no matched `e±`; alpha-only thresholds are the main handle.
- For `alpha<0.2`, integrated MC true `π/K/p` loss is `0.10649%` RAPGAP and `0.11354%` DJANGOH; matched `e±` removal is `5.07632%` RAPGAP and `5.26673%` DJANGOH. Bracketing between `alpha<0.1` and `alpha<0.3` passes.
- Residual true `e±` after `alpha<0.2`, relative to true `π/K/p` after veto, is RAPGAP `2729276/63340834 = 0.0430887` and DJANGOH `3319290/75590840 = 0.0439113`.
- Existing "Migration IN / fakes: gen OUTSIDE PS, reco INSIDE PS" plots do **not** include `IsMatchedElectron==1`; electron residuals need dedicated diagnostics.
- Do not use raw MC subtraction as central yet; angular vetoes are H1-precedented and likely target electron-like contamination, but data/MC mismatch requires control-normalized sideband validation and systematic treatment.

## Alpha-Veto Plot and Script Runbook

Use these local plot/script entry points first. For response-consistent alpha0p2 background/fake treatment, read `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md` before changing data vectors, responses, or fake subtraction.

| Purpose | Local path / script |
|---|---|
| Apples-to-apples `alpha<0.1` data/MC track fraction with MC `IsMatchedElectron` included | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-22/per_z005_global_bin_fixedPhoton_includeMatchedElectronMC/` |
| Absolute-yield `pT_h` fraction of all tracks with `alpha<0.1` | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/alpha_fraction_absolute_yields_pt_h_by_Q2_x_z/`; run `plot_alpha_fraction_absolute_yields_pt_h_by_Q2_x_z.py` |
| MC true `π/K/p` loss for alpha/momentum variants | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-22/true_hadron_loss_alpha_momentum_variants/` |
| Analysis-note true-hadron `alpha<0.1` one-page plot | figure `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/analysis_note/figures/true_hadron_loss_fraction_alpha_only_pt_h_by_Q2_x_z_analysis_note_onepage_x0to8_nobox.pdf`; script `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/true_hadron_loss_fraction_pt_h_by_Q2_x_z_analysis_note/plot_true_hadron_loss_fraction_pt_h_by_Q2_x_z_analysis_note_onepage_x0to8_nobox.py` |
| Analysis-note matched-electron `alpha<0.1` one-page plot | figure `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/analysis_note/figures/matched_electron_loss_fraction_alpha_only_pt_h_by_Q2_x_z_analysis_note_onepage_x0to8_nobox.pdf`; script `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/matched_electron_loss_fraction_pt_h_by_Q2_x_z_analysis_note/plot_matched_electron_loss_fraction_pt_h_by_Q2_x_z_analysis_note_onepage_x0to8_nobox.py` |
| Worst-bin `alpha<0.1` audit | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/worst_bin_alpha_veto_audit/`; key files `tables/worst_bins_by_data_alpha_removed_fraction_top10.csv`, `tables/mc_truth_composition_removed_top10.csv`, `plots/removed_vs_kept_control_distributions_top10.pdf`, `plots/worst_bin_fraction_comparison.pdf` |
| True electron versus hadron removal comparison | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-22/true_electron_vs_hadron_removal_alpha_momentum_variants/` |
| Matched-electron momentum diagnostic | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/matched_electron_momentum_diagnostic/`; key table `tables/integrated_efficiency_rejection_table.csv` |
| `alpha<0.2` true-hadron/electron efficiency | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/alpha0p2_true_hadron_electron_efficiency/`; note figures `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/analysis_note/figures/fig_true_hadron_loss_alpha0p2_onepage.pdf` and `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/analysis_note/figures/fig_matched_electron_removal_alpha0p2_onepage.pdf` |
| `alpha<0.2` residual electron background | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/alpha0p2_residual_electron_background/`; note figure `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/analysis_note/figures/fig_residual_all_true_e_over_true_piKp_after_alpha0p2_onepage.pdf` |
| Canonical `alpha<0.2` reco-background fraction 7-column display | local root `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_2026-06-25/background_fractions_vs_data_7col/`; remote script `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/scripts/regen_minimal_7col_plots.py`; remote outputs `plots/background_fractions_vs_data_onepage_7col.{pdf,png}`, `plots/background_fractions_vs_data_table_7col.{csv,json}`, `producer/manifest_regen_minimal_7col.json`; denominator raw alpha0p2 reco data vector under `output_v6/studies/electron_cone_alpha0p2_z005_20260623/`; z edges `[0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]` with first bin `0.0-0.05`; `zlow005` in paths is a tag/name only, not a `z >= 0.005` cut; display pT_h bins `[[0.0,0.5],[0.5,1.0],[1.0,2.0],[2.0,4.0],[4.0,8.0]]` with centers `[0.25,0.75,1.5,3.0,6.0]`; original electron+V0+lowQ2 validation denominator `3865073.0`, background `285747.68917109974`, fraction `0.07393073537578715`; uniform L351000 diagnostic totals including det.int are background `308655.8946`, fraction `0.0798577141`; prior first-z-bin `0.005-0.05`, x=0, or stale-center renders/metadata are superseded. |
| Analysis-note matched-V0-daughter over true-hadron yield | root `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_electron_opening_angle_veto_2026-06-23/matched_v0_daughter_over_true_hadron_yield_analysis_note/`; canonical script `plot_perbin_7col.py`; accepted final script `plot_matched_v0_daughter_over_true_hadron_yield_onepage_7col_y0to0p1_mathh_xSciCompact_pTh.py`; accepted final figure `matched_v0_daughter_over_true_hadron_yield_onepage_7col_y0to0p1_mathh_xSciCompact_pTh.pdf`; analysis-note copy `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/analysis_note/figures/fig_matched_v0_daughter_over_true_hadron_yield_onepage_7col_y0to0p1_mathh_xSciCompact_pTh.pdf`; superseded iterative variants are quarantined under `_quarantine_superseded_2026-06-24/` and must not be used. |

Remote study roots are all under `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_electron_opening_angle_veto_2026-06-20/`:
- `per_z005_global_bin_alpha_veto_fixedPhoton_includeMatchedElectronMC_2026-06-22/`
- `true_hadron_loss_alpha_momentum_variants_2026-06-22/`
- `true_electron_vs_hadron_removal_alpha_momentum_variants_2026-06-22/`
- `worst_bin_alpha_veto_audit_2026-06-23/`
- `matched_electron_momentum_diagnostic_2026-06-23/`
- `alpha0p2_true_hadron_electron_efficiency_2026-06-23/`

Use each directory's `run_log*` and manifest files for exact provenance. Compact Condor provenance: fixed `alpha<0.1` includeMatchedElectronMC cluster `3859184`; true-hadron variants `3879548`; electron-vs-hadron `3897867` with recoveries `3898386/3898395`; worst-bin audit `3900226/3900227/3900230`; matched-electron momentum diagnostic smokes `3906818/3906819` and production `3906831–3907704`; alpha0p2 smoke `3908407` and production `3908416` with held duplicate excluded/plots generated locally from merged outputs.

## Alpha0p2 Reco-Background / All-Fake Response Handoff (2026-06-28)

Primary handoff: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`. This is a diagnostic/provisional save point for future unfolding agents; central response/release products were not overwritten.

| Item | Path / value |
|---|---|
| Reco-background fraction study root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/` |
| Canonical 7-column plot script | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/scripts/regen_minimal_7col_plots.py` |
| Uniform L351000 package | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/` |
| Overlap diagnostics | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/alpha0p2_overlap_exclusivity_20260626/` |
| det.int response partition | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/detint_response_partition_20260626/` |
| electron/V0 response partition | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/electron_v0_response_partition_20260627/` |
| All-fake response root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/` |
| All-fake response builder | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/scripts/build_migration_th2_per_charge_allfake.py` |
| Local compact all-fake summaries | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/response_diagnostics/alpha0p2_allfake_response_z005_2026-06-27/` |

Validated compact numbers: use `L_data=351000.0 nb^-1`; uniform L351000 totals are electron avg `148976.4352`, V0 avg `132065.2768`, lowQ2 `208.3795`, det.int avg `27405.8031`, total `308655.8946`, denominator `3865073.0`, total fraction `0.0798577141`. Electron∩V0 scaled average overlap is `112.2093` (`0.0753%` of electron, `0.0850%` of V0, `0.0364%` of full total); det.int overlaps electron/V0 are zero, and lowQ2 is source-distinct. The det.int response partition cluster `3971447` found about `86%` matched physical block and `14%` fake, so blind pre-subtraction with unchanged response is not response-consistent. The electron/V0 partition cluster `3973257` found non-V0 electrons `100%` fake column in alpha0p2, V0 daughters `100%` excluded/dropped, and central v6 excludes both from physical matched signal response.

All-fake response artifacts: `responses/migration_inputs_allfake_alpha0p2_z005.root`, `responses/allfake_response_matrices_and_categories.npz`, `validation/validation_summary.json`, `diagnostics/category_fake_vectors_rapgap31.{json,csv}`, and `diagnostics/category_fake_vectors_djangoh14.{json,csv}`. Cluster `3987932` completed `8/8`; response shape is `(491,246)` per charge; RAPGAP/DJANGOH input files are `364/501`; duplicate guardrail PASS; no merged mixing. Weighted fake-column totals are RAPGAP `17196553.31` and DJANGOH `19609919.68`; category totals are RAPGAP electron `3819129.90`, V0 `3432137.16`, det.int `676560.99`, other `9268725.25`; DJANGOH electron `4354659.74`, V0 `3838524.72`, det.int `837573.52`, other `10579161.70`.

Response-consistency rules: with the current unchanged alpha0p2 response, V0 can be pre-subtracted/vetoed because it is excluded, non-V0 electrons are fake-column handled and must not also be pre-subtracted if fake subtraction is active, and det.int should not be blindly pre-subtracted because it is mostly matched response. With the all-fake variant, electron/V0/det.int are all fake-column components; the data vector must not additionally subtract them externally, and fake subtraction/covariance must handle them exactly once. Describe det.int as STR-only GEANT secondary charged `π/K/p` excluding electrons/V0, not pure material interactions.

## Plot Style and Script Traps

For the canonical `alpha<0.2` reco-background fraction 7-column display, use raw alpha0p2 reco data as the denominator, z edges `[0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]` with first bin `0.0-0.05`, and pair-sum pT_h native bins `(0+1),(2+3),(4+5),(6+7),(8+9)`. Treat `zlow005` in paths as a tag/name only, not a `z >= 0.005` cut, and reject prior first-z-bin `0.005-0.05`, x=0, or stale-low-pT-edge-center renders/metadata as superseded.

Canonical analysis-note alpha-veto plot format is the 7-column one-page layout, with `plot_perbin_7col.py` as the canonical per-bin alpha-veto analysis-note plotting template/script for this style:
- Use one PDF page with the 7-column layout for the z005 `pT_h`-by-(`Q²`, `x`, `z`, charge) panel grid when possible.
- Use `xlim 0–8`.
- Use the plot-specific y-axis needed by the figure; for the matched-V0-daughter over true-hadron figure keep the fixed range `[0, 0.1]`.
- Use steplines plus center markers plus binomial/statistical errors.
- Do not use filled areas, bars, or area/shape-normalized histograms for cut fractions.
- Use absolute count ratios only; do not shape-normalize.
- Put plain top-right panel text for numerical `$Q^{2}$`/`x`/`z` ranges and charge, with no frame or fill.
- Use numerical `$Q^{2}$`, `x`, and `z` ranges, not bin numbers.
- Use compact Bjorken-`x` notation like `2.1E-3` with no leading-zero exponent.
- Use a single hyphen in ranges, for example `150-300`, not `150--300`.
- Render `$Q^{2}$` and GeV$^{2}$ with superscripts in plot labels and text, or use unicode superscripts if the backend supports them.
- Format upper endpoints like `20000` as `2E4` in panel text.
- Use matplotlib mathtext with `usetex=False` for the x-axis label `$p_{T,h}$`.
- Use mathtext `$h^{+}$` and `$h^{-}$` for charge.
- Put compact per-panel RAPGAP/DJANGOH labels or legends in the upper-left area, shifted enough up/left to avoid the `$Q^{2}$`/`x`/`z` text, and draw no legend frame.
- Do not add plot titles or bottom explanatory text.

Script trap: in pandas plotting scripts, use `df['sample']`, not `df.sample`, to avoid the method-name collision encountered in the alpha0p2 plotting pass.

## Operational Guardrails

Before any NAF command, follow the repository SSH rules in `AGENTS.md` and `CONTEXT.md`: check the control socket once with `ssh -O check naf-hone.desy.de`, do not retry failed SSH commands, and use Condor for multi-file processing. Keep side-study outputs under `output_v6/studies/` and local artifacts under `sidis_highQ2/plots/`, `sidis_highQ2/data/`, or `sidis_highQ2/docs/` subdirectories. Do not modify central release products for diagnostic alpha-veto work.

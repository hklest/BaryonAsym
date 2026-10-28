# Cleanup Audit 2026-05-22 -- sidis_highQ2 docs and plots

**Author:** researcher (read-only audit; no file moves performed)
**Sources of truth used:** `sidis_highQ2/context-handoff.md`,
`docs/plans/plan-finalization-r1.md` (r2 revision),
`docs/handoffs/decision-binning-amendment-HQv5fr2.md`,
`docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md`,
`docs/INDEX.md`,
`docs/data/deprecation_audit_2026-05-21.md` (including the 2026-05-22 pending-review disposition pass),
plus targeted grep of `scripts/`.

> **Important interaction with the existing 2026-05-21 audit.** The earlier audit
> already banner-marked 24 docs/data JSONs and 21 docs/results MDs as DEPRECATED
> and 29 docs/results MDs as CURRENT, plus a 2026-05-22 pending-review
> disposition pass that hit 21 more files. This 2026-05-22 audit re-validates
> those dispositions against the supervisor-provided current-state context,
> extends the audit to `docs/plans/`, `docs/handoffs/`, `docs/references/`,
> `docs/research/`, `docs/reviews/`, `docs/specs/`, `docs/research-crad-v5f-attempt.md`,
> the `plots/` tree (directory-level), and `docs/data/_audit_logs/`, and
> flags every discrepancy or unresolved case as UNCERTAIN.

---

## Summary

- Total items inspected: **221**
  (docs/data: 121 entries incl. _audit_logs dir; docs/results: 59 files;
  docs/plans: 4; docs/handoffs: 4; docs/references: 14;
  docs/research: 18; docs/reviews: 10; docs/specs: 3;
  docs/research-crad-v5f-attempt.md: 1; plots/ subdirectories: 39;
  plus the _audit_logs/mc_v5f_launch/ subdirectory's 7 log files counted as one HISTORICAL_LOG group)

- **CURRENT:** 116
- **SUPERSEDED:** 51
- **HISTORICAL_LOG:** 35
- **PROTOTYPE:** 8
- **UNCERTAIN:** 11

- **Estimated bytes to move to `old/`:** ~37 MB of the docs/ + plots/ ~113 MB tree.
  Dominant chunks: `phih_harmonics_v5f_no_ep0304.json` (4.6 MB v1), `phi_h_moments_v5f_data.json`
  (450 KB), `acceptance_highQ2_charged_mc_prototype.json` (325 KB),
  `multiplicity_highQ2_charged_mc_prototype.json` (347 KB), `binning_study_highQ2.json`
  (660 KB), `plots/highQ2/` (5.8 MB pre-v5f QA), `plots/multiplicity_cross_section_v5f/`
  (1.5 MB), `plots/multiplicity_cross_section_v5f_no_ep0304/` (1.1 MB),
  `plots/asymmetry_v5f/` (612 KB), `plots/phi_h_moments_v5f/` (460 KB),
  `plots/phi_h_moments_v5f_no_ep0304/` (1.3 MB), `plots/phi_h_moments_v5f_no_ep0304_v2/`
  (1.6 MB once superseded by v3_coarse for headline use).

---

## Proposed `old/` layout

Mirroring source structure under a single `sidis_highQ2/old/` tree so files
can be restored to their original location by `git mv old/<rel> <rel>` if
ever needed:

```
sidis_highQ2/old/
├── docs/
│   ├── data/
│   │   ├── _audit_logs/                                    (HISTORICAL_LOG)
│   │   │   └── mc_v5f_launch/{01..07}_*.log
│   │   ├── A_h_v5f_no_ep0304.json                          (SUPERSEDED by _v2)
│   │   ├── abc_dis_v5f_FINAL.json                          (SUPERSEDED by _no_ep0304)
│   │   ├── abc_dis_v5f_FINAL_postrecovery_delta.json       (HISTORICAL_LOG)
│   │   ├── abc_dis_vs_epsilon_v5f.json                     (SUPERSEDED by _no_ep0304)
│   │   ├── acceptance_highQ2_charged_mc_prototype.json     (PROTOTYPE)
│   │   ├── asymmetry_v5f_data_HQv5fr1.json                 (SUPERSEDED by A_h_v5f_no_ep0304_v2)
│   │   ├── binning_study_highQ2.json                       (SUPERSEDED by binning_v5f_frozen)
│   │   ├── cross_section_v5f_data_HQv5fr1.json             (SUPERSEDED by multiplicities_..._v2)
│   │   ├── data_hadron_yields_v5f_HQv5fr1.json             (SUPERSEDED by yields_v5f_3period)
│   │   ├── data_yields_v5f_HQv5fr1.json                    (SUPERSEDED by yields_v5f_3period)
│   │   ├── deprecation_audit_2026-05-19.md                 (HISTORICAL_LOG)
│   │   ├── deprecation_audit_2026-05-21.md                 (HISTORICAL_LOG)
│   │   ├── em06_*.json,*.md                                (HISTORICAL_LOG — em06 recovery campaign)
│   │   ├── eps_dis_data_application_v5f_highQ2.json        (PROTOTYPE / SUPERSEDED by n_dis_eps_dis_v5f_HQv5fr1)
│   │   ├── eps_dis_highQ2_mc_prototype.json                (PROTOTYPE)
│   │   ├── g1c_*_launch*.{md,json}                         (HISTORICAL_LOG — production launches)
│   │   ├── highQ2_norad_hat_smoke.md                       (HISTORICAL_LOG)
│   │   ├── highQ2_oomclumi_condor_log.md                   (HISTORICAL_LOG)
│   │   ├── highQ2_v5d_v5e_phase_log.md                     (HISTORICAL_LOG)
│   │   ├── highQ2_v5f_schema_delta.md                      (HISTORICAL_LOG)
│   │   ├── multiplicities_cross_sections_v5f_no_ep0304.json (SUPERSEDED by _v2)
│   │   ├── multiplicity_highQ2_charged_mc_prototype.json   (PROTOTYPE)
│   │   ├── multiplicity_v5f_data_HQv5fr1.json              (SUPERSEDED by multiplicities_..._v2)
│   │   ├── n_dis_data_v5f_highQ2.json                      (SUPERSEDED by n_dis_eps_dis_v5f_HQv5fr1)
│   │   ├── n_dis_prototype_highQ2_mc.json                  (PROTOTYPE)
│   │   ├── norad_condor_dryrun.log                         (HISTORICAL_LOG)
│   │   ├── phase{3..7}_*_2026-05-19.md                     (HISTORICAL_LOG — port/validation log)
│   │   ├── phi_h_moments_v5f_data.json                     (SUPERSEDED by phih_harmonics_..._v3_coarse)
│   │   ├── phi_yields_condor_*                             (HISTORICAL_LOG — Condor dry-run/submission)
│   │   ├── phih_harmonics_v5f_no_ep0304.json               (SUPERSEDED by _v2)
│   │   ├── staleness-sweep-highQ2-sidis-scripts.json       (HISTORICAL_LOG)
│   │   ├── systematics_HQv5fr2_truncated.json              (SUPERSEDED — alt-binning study; see UNCERTAIN section)
│   │   ├── systematics_v5f.json                            (UNCERTAIN — still read by aggregate_systematics_v5f_v2.py)
│   │   ├── systematics_v5f_aggregated.json                 (SUPERSEDED by _v2)
│   │   ├── systematics_v5f_model_dependence.json           (SUPERSEDED by _v2)
│   │   ├── systematics_v5f_nuclia_proton_omission.json     (SUPERSEDED by _promoted_v2)
│   │   ├── systematics_v5f_nuclia_proton_omission_promoted.json (SUPERSEDED by _v2)
│   │   ├── systematics_v5f_unfolding_spread.json           (SUPERSEDED by _v2)
│   │   ├── unfolding_compare_v5f_HQv5fr2.json              (UNCERTAIN — see note)
│   │   ├── unfolding_compare_v5f_HQv5fr2_analyzed.json     (UNCERTAIN — see note)
│   │   ├── v5f_port_*_2026-05-19.md                        (HISTORICAL_LOG)
│   │   ├── validation_highQ2_data_availability.json        (HISTORICAL_LOG)
│   │   ├── validation_highQ2_mc_manifest.{json,CORRECTIONS.md} (HISTORICAL_LOG)
│   │   ├── validation_highQ2_mc_schema_full.json           (HISTORICAL_LOG)
│   │   ├── validation_highQ2_mc_species_matching.json      (HISTORICAL_LOG)
│   │   ├── yields_v5f_3period_HQv5fr2.json                 (UNCERTAIN — see note)
│   │   └── yields_v5f_3period_extraction.log               (HISTORICAL_LOG)
│   ├── results/
│   │   ├── abc_dis_v5f_FINAL.md                            (SUPERSEDED)
│   │   ├── abc_dis_v5f_FINAL_postrecovery.md               (SUPERSEDED / HISTORICAL diagnosis)
│   │   ├── abc_dis_vs_epsilon_v5f.md                       (SUPERSEDED)
│   │   ├── acceptance_highQ2_charged_mc_prototype.md       (PROTOTYPE)
│   │   ├── asymmetry_v5f_data.md                           (SUPERSEDED)
│   │   ├── binning_study_highQ2.md                         (SUPERSEDED)
│   │   ├── eps_dis_data_application_v5f_highQ2.md          (SUPERSEDED)
│   │   ├── eps_dis_highQ2_mc_prototype.md                  (PROTOTYPE)
│   │   ├── multiplicity_cross_section_v5f_data.md          (SUPERSEDED)
│   │   ├── multiplicity_highQ2_charged_mc_prototype.md     (PROTOTYPE)
│   │   ├── multiplicities_cross_sections_A_h_v5f_no_ep0304.md (SUPERSEDED)
│   │   ├── n_dis_data_v5f_highQ2.md                        (SUPERSEDED)
│   │   ├── n_dis_prototype_highQ2_mc.md                    (PROTOTYPE)
│   │   ├── phi_h_moments_v5f.md                            (SUPERSEDED)
│   │   ├── phih_harmonics_v5f_no_ep0304.md                 (SUPERSEDED by _v2)
│   │   ├── systematics_v5f.md                              (SUPERSEDED by rigor_status_per_source)
│   │   ├── systematics_v5f_aggregated.md                   (SUPERSEDED)
│   │   ├── systematics_v5f_model_dependence.md             (SUPERSEDED)
│   │   ├── systematics_v5f_nuclia_proton_omission.md       (SUPERSEDED)
│   │   ├── systematics_v5f_nuclia_proton_omission_promoted.md (SUPERSEDED)
│   │   ├── systematics_v5f_unfolding_spread.md             (SUPERSEDED)
│   │   ├── unfolding_compare_v5f.md                        (HISTORICAL_LOG)
│   │   ├── unfolding_compare_v5f_HQv5fr2.md                (UNCERTAIN)
│   │   ├── W4_synthesis_v5f_analysis.md                    (HISTORICAL_LOG)
│   │   ├── W4_deep_synthesis_v5f.md                        (HISTORICAL_LOG)
│   │   └── W5_synthesis_v5f.md                             (HISTORICAL_LOG)
│   ├── plans/
│   │   ├── plan-highQ2-sidis-next-steps.md                 (UNCERTAIN — partially superseded only)
│   │   ├── plan-phih-systematics.md                        (SUPERSEDED by plan-finalization-r1 Phase 7)
│   │   └── plan-workspace-migration-sidis-highQ2.md        (HISTORICAL_LOG — migration complete)
│   ├── reviews/
│   │   ├── acceptance_report_v5e_100k.md                   (HISTORICAL_LOG — pre-v5f diagnostic)
│   │   └── v5d_vs_v5e_100k.md                              (HISTORICAL_LOG — pre-v5f diagnostic)
│   └── specs/
│       ├── spec-v5d-builder-v0-and-vmd-flags.md            (HISTORICAL_LOG — documentation-only spec)
│       ├── spec-v5d-builder-v0-handling.md                 (HISTORICAL_LOG)
│       └── spec-v5e-addendum-gen-denominator-fix.md        (HISTORICAL_LOG)
└── plots/
    ├── highQ2/                                              (HISTORICAL_LOG — pre-v5f QA prototypes)
    ├── ep0607_smoke_qa/                                     (HISTORICAL_LOG — production smoke)
    ├── v5f_smoke_compare/                                   (HISTORICAL_LOG — v5b→v5f port QA)
    ├── crad_v5f/                                            (PROTOTYPE — empty except README; superseded by crad_v5f_strategy2)
    ├── asymmetry_v5f/                                       (SUPERSEDED by asymmetry_v5f_no_ep0304_v2/)
    ├── asymmetry_v5f_no_ep0304/                             (SUPERSEDED by _v2/)
    ├── multiplicity_cross_section_v5f/                      (SUPERSEDED by _no_ep0304_v2/)
    ├── multiplicity_cross_section_v5f_no_ep0304/            (SUPERSEDED by _v2/)
    ├── phi_h_moments_v5f/                                   (SUPERSEDED by _no_ep0304_v3_coarse/)
    ├── phi_h_moments_v5f_no_ep0304/                         (SUPERSEDED by _v2/ and _v3_coarse/)
    ├── phi_h_moments_v5f_no_ep0304_v2/                      (SUPERSEDED for headline use by _v3_coarse/; retained for fine-grid record — see note)
    ├── abc_dis_v5f_FINAL/                                   (SUPERSEDED by _no_ep0304/)
    ├── abc_dis_v5f_FINAL_postrecovery/                      (SUPERSEDED/HISTORICAL — README states functionally identical to FINAL)
    ├── abc_dis_vs_epsilon/                                  (SUPERSEDED by _no_ep0304/)
    └── abc_dis_low_Q2_investigation/                        (PROTOTYPE — empty dir)
```

> Note: `_v2` plot dirs that are superseded by `_v3_coarse` for headline use
> (specifically `plots/phi_h_moments_v5f_no_ep0304_v2/`) MAY be kept current
> as the "fine-grid" companion if the supervisor wants to keep both granularities
> live; see UNCERTAIN. The fine-grid `_v2` JSON+MD pair is still listed as CURRENT
> in `docs/INDEX.md`.

---

## Per-file disposition

### docs/data/

This table extends and re-validates the 2026-05-21 audit's dispositions. Files
not in this table inherit their disposition from that audit verbatim and remain
CURRENT or HISTORICAL_LOG as listed there.

| File | Disposition | Superseded by / note |
|------|-------------|----------------------|
| `_audit_logs/` (dir, 7 logs in `mc_v5f_launch/`) | HISTORICAL_LOG | Per supervisor rule 4: audit_logs is a historical store; preserve as-is inside `old/`. |
| `A_h_v5f_no_ep0304.json` | SUPERSEDED | `A_h_v5f_no_ep0304_v2.json` (v1 inconsistent empty-bin handling) |
| `A_h_v5f_no_ep0304_v2.json` | CURRENT | Supervisor-named authoritative. |
| `abc_dis_dedicated_unfolding_v5f.json` | CURRENT | Confirmed CURRENT by 2026-05-22 dispo (#1); per-period N source for `abc_dis_v5f_FINAL_no_ep0304.json`. |
| `abc_dis_low_Q2_crad_bias.json` | CURRENT | Confirmed CURRENT by 2026-05-22 dispo (#2); feeds `systematics_v5f_crad_residual_ABC.json`. |
| `abc_dis_low_Q2_em06_attribution.json` | CURRENT | Confirmed CURRENT by 2026-05-22 dispo (#3); period-drop evidence. |
| `abc_dis_stability_v5f.json` | CURRENT | Confirmed CURRENT by 2026-05-22 dispo (#4); W4-deep-B per-period A_BC^DIS. |
| `abc_dis_v5f_FINAL.json` | SUPERSEDED | `abc_dis_v5f_FINAL_no_ep0304.json` (4-period; ep0304 dropped). |
| `abc_dis_v5f_FINAL_no_ep0304.json` | CURRENT | Supervisor-named final A_BC^DIS. |
| `abc_dis_v5f_FINAL_postrecovery_delta.json` | HISTORICAL_LOG | NULL-recovery delta audit; preserved as evidence. |
| `abc_dis_vs_epsilon_v5f.json` | SUPERSEDED | `abc_dis_vs_epsilon_v5f_no_ep0304.json`. |
| `abc_dis_vs_epsilon_v5f_no_ep0304.json` | CURRENT | Supervisor-named final. |
| `acceptance_highQ2_charged_mc_prototype.json` | PROTOTYPE | Superseded by `acceptance_v5f_charged.json`. |
| `acceptance_v5f_charged.json` | CURRENT | HQ-v5f-r1 acceptance. Per supervisor rule on r2: the base file name has no `HQv5fr1` token, so it is not subject to the r1→r2 displacement rule by name. Still CURRENT for r1. |
| `acceptance_v5f_charged_HQv5fr2.json` | CURRENT | HQ-v5f-r2 alt-binning acceptance; supervisor named HQ-v5f-r2 as the new frozen binning. INDEX previously called this "alt-binning study"; after the 2026-05-21 r2 freeze it is part of the r2 rebuild generation. |
| `asymmetry_v5f_data_HQv5fr1.json` | SUPERSEDED | `A_h_v5f_no_ep0304_v2.json` (per 2026-05-21 audit and INDEX). No r2 counterpart exists yet, but supersession is by physics product, not by binning. |
| `binning_study_highQ2.json` | SUPERSEDED | `binning_v5f_frozen.json` (exploratory). |
| `binning_v5f_frozen.json` | CURRENT | Holds both `HQ-v5f-r2` (current) and `prior_scheme_HQ_v5f_r1` (archived). Confirmed by grep. |
| `crad_strategy2_norad_condor_plan.md` | CURRENT | Stream C Condor plan; still pending. |
| `crad_v5f_per_bin.json` | CURRENT | Placeholder=1 per-bin; consumed by `systematics_v5f_crad_placeholder_v2.json`. |
| `crad_v5f_strategy2.json` | CURRENT | BLOCKED stub status. |
| `cross_section_v5f_data_HQv5fr1.json` | SUPERSEDED | `multiplicities_cross_sections_v5f_no_ep0304_v2.json`. |
| `data_hadron_yields_v5f_HQv5fr1.json` | SUPERSEDED | `yields_v5f_3period.json`. |
| `data_yields_v5f_HQv5fr1.json` | SUPERSEDED | `yields_v5f_3period.json`. |
| `deprecation_audit_2026-05-19.md` | HISTORICAL_LOG | Earlier audit. |
| `deprecation_audit_2026-05-21.md` | HISTORICAL_LOG | Includes 2026-05-22 disposition appendix; preserved as the authoritative provenance for the banner state of every JSON/MD file. |
| `em06_g15_recovery_coverage.json` | HISTORICAL_LOG | em06 G15 recovery campaign (NULL outcome). |
| `em06_g15_recovery_jobs.json` | HISTORICAL_LOG | Condor manifest. |
| `em06_g15_recovery_launch.md` | HISTORICAL_LOG | Recovery launch log. |
| `em06_postrecovery_audit.json` | HISTORICAL_LOG | NULL audit. |
| `em06_postrecovery_extraction.json` | HISTORICAL_LOG | Postrecovery extraction result. |
| `ep0304_rate_audit_v5f.json` | CURRENT | ep0304 drop justification (+3.85σ). |
| `ep0304_rate_audit_v5f.md` | CURRENT | Memo for above. |
| `eps_dis_data_application_v5f_highQ2.json` | SUPERSEDED | `n_dis_eps_dis_v5f_HQv5fr1.json`. |
| `eps_dis_highQ2_mc_prototype.json` | PROTOTYPE | MC prototype; unified into HQv5fr1. |
| `g1c_27p6_data_production_jobs.json` | HISTORICAL_LOG | G1C ep0607 production manifest. |
| `g1c_27p6_data_production_plan.json` | HISTORICAL_LOG | G1C production plan. |
| `g1c_ep0607_data_production_launch.md` | HISTORICAL_LOG | Launch log. |
| `g1c_mc_v5f_production_launch_2026-05-19.md` | HISTORICAL_LOG | MC launch log. |
| `g1c_v5f_full_launch_2026-05-19.md` | HISTORICAL_LOG | Full launch log. |
| `g1c_v5f_full_launch_CORRECTION_2026-05-19.md` | HISTORICAL_LOG | Correction log. |
| `g1c_v5f_production_launch_2026-05-19.md` | HISTORICAL_LOG | Production launch log. |
| `grl_manifest_highQ2.json` | CURRENT | GRL run manifest. |
| `highQ2_mc_luminosity.json` | CURRENT | MC luminosity. |
| `highQ2_mc_per_file_schema.csv` | HISTORICAL_LOG | Schema dump. |
| `highQ2_nonrad_mc_inventory.json` | CURRENT | NORAD MC inventory (Stream C input). |
| `highQ2_norad_hat_smoke.md` | HISTORICAL_LOG | NORAD smoke test. |
| `highQ2_oomclumi_condor_log.md` | HISTORICAL_LOG | Condor MC lumi log. |
| `highQ2_per_run_beam_energy.csv` | CURRENT | Per-run beam energy table. |
| `highQ2_per_run_beam_energy.json` | CURRENT | Per-run beam energy table. |
| `highQ2_v5d_v5e_phase_log.md` | HISTORICAL_LOG | v5d/v5e phase log. |
| `highQ2_v5f_schema_delta.md` | HISTORICAL_LOG | Schema delta log. |
| `lumi_effective_per_period_v5f.json` | CURRENT | Effective lumi per period. |
| `migration_matrices_v5f.json` | CURRENT | HQ-v5f-r1 matrices, still consumed. |
| `migration_matrices_v5f_r2_default.json` | CURRENT | HQ-v5f-r2 alt matrices; per supervisor, r2 is the new frozen binning. |
| `multiplicities_cross_sections_v5f_no_ep0304.json` | SUPERSEDED | `_v2.json`. |
| `multiplicities_cross_sections_v5f_no_ep0304_v2.json` | CURRENT | Supervisor-named authoritative. |
| `multiplicity_highQ2_charged_mc_prototype.json` | PROTOTYPE | Pre-v5f prototype. |
| `multiplicity_v5f_data_HQv5fr1.json` | SUPERSEDED | `multiplicities_cross_sections_v5f_no_ep0304_v2.json`. |
| `n_dis_data_v5f_highQ2.json` | SUPERSEDED | `n_dis_eps_dis_v5f_HQv5fr1.json`. |
| `n_dis_eps_dis_v5f_HQv5fr1.json` | CURRENT | Supervisor-named input to plan-finalization-r1; current authoritative on r1 grid pending r2 rebuild (see plan §1.1 G4 "Closed on r1; r2 assembly-open"). |
| `n_dis_prototype_highQ2_mc.json` | PROTOTYPE | MC prototype. |
| `norad_condor_dryrun.log` | HISTORICAL_LOG | Condor dry-run log. |
| `norad_condor_production_plan.json` | HISTORICAL_LOG | NORAD Condor production plan. |
| `phase3_naf_cleanup_2026-05-19.md` | HISTORICAL_LOG | Phase 3 NAF cleanup. |
| `phase4_naf_new_root_2026-05-19.md` | HISTORICAL_LOG | Phase 4 ROOT install. |
| `phase5_phase6_validation_2026-05-19.md` | HISTORICAL_LOG | Phase 5–6 validation. |
| `phase7_bridge_condor_smoke_2026-05-19.md` | HISTORICAL_LOG | Phase 7 Condor smoke. |
| `phi_gap_attribution_v5f.json` | CURRENT | φ-bin gap diagnosis. |
| `phi_h_moments_v5f_data.json` | SUPERSEDED | `phih_harmonics_v5f_no_ep0304_v3_coarse.json` (4-period pre-ep0304 drop). |
| `phi_yields_condor_dryrun.log` | HISTORICAL_LOG | Condor dry-run log. |
| `phi_yields_condor_dryrun_v2.log` | HISTORICAL_LOG | Condor dry-run log v2. |
| `phi_yields_condor_submission.json` | HISTORICAL_LOG | Condor submission manifest. |
| `phi_yields_condor_submission_v2.json` | HISTORICAL_LOG | Condor submission manifest v2. |
| `phi_yields_v5f_3period.json` | CURRENT | Phase-1 fine phi yields, 3-period. |
| `phi_yields_v5f_3period_coarse.json` | CURRENT | Phase-1 coarse phi yields, 3-period. |
| `phih_harmonics_v5f_no_ep0304.json` | SUPERSEDED | `_v2.json` (v1 internally inconsistent). |
| `phih_harmonics_v5f_no_ep0304_v2.json` | CURRENT | Fine-grid 336 cells; supersedes v1; retained alongside v3_coarse as fine-grid record per INDEX.md. |
| `phih_harmonics_v5f_no_ep0304_v3_coarse.json` | CURRENT | Coarse 72-cell publishable extraction. |
| `phih_harmonics_v5f_no_ep0304_v4_unbinnedML.json` | UNCERTAIN | **NOT in supervisor-provided current-state context.** File mtime 2026-05-22 09:51 (today). Companion script `scripts/extract_phih_harmonics_v5f_v4_unbinned_ML.py` exists. No md companion in docs/results/; no INDEX entry; no deprecation banner. Either (a) work-in-progress unbinned-ML extraction that should be CURRENT and added to INDEX, or (b) experimental product that should be flagged or moved. **Supervisor must classify.** |
| `r_bc_v5f.json` | CURRENT | R_BC. |
| `staleness-sweep-highQ2-sidis-scripts.json` | HISTORICAL_LOG | Companion of `docs/reviews/staleness-sweep-highQ2-sidis-scripts.md`. |
| `systematics_HQv5fr2_truncated.json` | UNCERTAIN | Per 2026-05-22 dispo this was marked DEPRECATED because r1 was production at the time. With the 2026-05-21 r2 freeze it is now an HQ-v5f-r2 study that may need to be retained as the bridge artifact between r1 budget and the pending r2 rebuild (plan-finalization-r1 Phase 5). It IS read by the active `plot_systematics_HQv5fr2_stack.py`. **Supervisor must classify CURRENT-r2-bridge or genuinely SUPERSEDED.** |
| `systematics_v5f.json` | UNCERTAIN | Marked SUPERSEDED by `_aggregated_v2.json` in the prior audit and listed as "v1 must not be used downstream" in supervisor context. **However**, `scripts/aggregate_systematics_v5f_v2.py` (the live v2 aggregator) **still reads** `systematics_v5f.json` to extract the `track_eff` flat-catalog source (see `IN_CATALOG = DOCS_DATA / 'systematics_v5f.json'`, source #8 in its enumeration). Also read by `compute_crad_residual_ABC_W4_deep_D.py` and `aggregate_perbin_LAr_HFS_beam.py` and `build_systematics_HQv5fr2_truncated.py` and most per-source v2 scripts (for comparison metadata). Either (a) the live aggregator must be refactored to source track_eff elsewhere before this file can be moved, or (b) the supervisor must declare this file retained as a catalog-only sidecar. **DO NOT MOVE without that refactor or declaration.** |
| `systematics_v5f_aggregated.json` | SUPERSEDED | `_v2.json`. |
| `systematics_v5f_aggregated_v2.json` | CURRENT | v2 budget, supervisor-named. |
| `systematics_v5f_beam_energy_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_charge_flip_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_crad_placeholder_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_crad_residual_ABC.json` | CURRENT | A_BC C_rad residual. |
| `systematics_v5f_hfs_scale_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_lar_angle_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_lar_escale_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_luminosity_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_model_dependence.json` | SUPERSEDED | `_v2.json`. |
| `systematics_v5f_model_dependence_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_nuclia_proton_omission.json` | SUPERSEDED | `_promoted_v2.json`. |
| `systematics_v5f_nuclia_proton_omission_promoted.json` | SUPERSEDED | `_promoted_v2.json`. |
| `systematics_v5f_nuclia_proton_omission_promoted_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_option_h_mask_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_perbin_LAr_HFS_beam.json` | CURRENT | per-bin audit. |
| `systematics_v5f_track_eff_chargedep.json` | CURRENT | charge-dep audit. |
| `systematics_v5f_trigger_eff_v2.json` | CURRENT | per-source v2. |
| `systematics_v5f_unfolding_spread.json` | SUPERSEDED | `_v2.json`. |
| `systematics_v5f_unfolding_spread_v2.json` | CURRENT | per-source v2. |
| `trigweightac_audit_v5f.json` | CURRENT | TrigWeightAC=1 audit. |
| `trigweightac_audit_v5f.md` | CURRENT | Memo. |
| `unfolding_compare_v5f.json` | HISTORICAL_LOG | W3 unfolding method comparison (HQ-v5f-r1). Companion md is in audit-trail. |
| `unfolding_compare_v5f_HQv5fr2.json` | UNCERTAIN | Marked DEPRECATED by 2026-05-22 dispo (because r1 was production). With r2 frozen 2026-05-21 it may be the active r2 unfolding comparison rather than an alt-binning study; companion md still in docs/results/. **Supervisor must classify.** |
| `unfolding_compare_v5f_HQv5fr2_analyzed.json` | UNCERTAIN | Same situation as above. |
| `v5f_port_into_clean_workspace_2026-05-19.md` | HISTORICAL_LOG | v5d→v5f port log. |
| `v5f_port_smoke_compare_2026-05-19.md` | HISTORICAL_LOG | Port smoke comparison. |
| `validation_highQ2_data_availability.json` | HISTORICAL_LOG | Pre-G1C state check. |
| `validation_highQ2_mc_manifest.CORRECTIONS.md` | HISTORICAL_LOG | MC manifest corrections log. |
| `validation_highQ2_mc_manifest.json` | HISTORICAL_LOG | Pre-v5f MC inventory. |
| `validation_highQ2_mc_schema_full.json` | HISTORICAL_LOG | Pre-v5f schema dump. |
| `validation_highQ2_mc_species_matching.json` | HISTORICAL_LOG | Early species/matching audit. |
| `validation_highQ2_q2es_vs_q2e.json` | CURRENT | Q²_es vs Q²_e check; F4 evidence. |
| `yields_v5f_3period.json` | CURRENT | Hadron yields 3-period (HQ-v5f-r1). |
| `yields_v5f_3period_HQv5fr2.json` | UNCERTAIN | Marked DEPRECATED by 2026-05-22 dispo. With r2 frozen this is the active r2 yields generation; named in `docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md` (lines 38–48) as the canonical output of the rebuild. **Probably CURRENT post r2-freeze; supervisor must confirm.** |
| `yields_v5f_3period_extraction.log` | HISTORICAL_LOG | Extraction log. |

### docs/results/

| File | Disposition | Superseded by / note |
|------|-------------|----------------------|
| `abc_dis_dedicated_unfolding_v5f.md` | CURRENT | Per 2026-05-22 dispo, narrative for the W5-C dedicated unfolding (CURRENT). |
| `abc_dis_low_Q2_crad_bias.md` | CURRENT | Per 2026-05-22 dispo. |
| `abc_dis_low_Q2_em06_attribution.md` | CURRENT | Per 2026-05-22 dispo. |
| `abc_dis_stability_v5f.md` | CURRENT | Per 2026-05-22 dispo. |
| `abc_dis_v5f_FINAL.md` | SUPERSEDED | `abc_dis_v5f_FINAL_no_ep0304.md`. |
| `abc_dis_v5f_FINAL_no_ep0304.md` | CURRENT | Supervisor-named. |
| `abc_dis_v5f_FINAL_postrecovery.md` | SUPERSEDED | `abc_dis_v5f_FINAL_no_ep0304.md` (postrecovery NULL audit, absorbed). |
| `abc_dis_vs_epsilon_v5f.md` | SUPERSEDED | `abc_dis_vs_epsilon_v5f_no_ep0304.md`. |
| `abc_dis_vs_epsilon_v5f_no_ep0304.md` | CURRENT | Supervisor-named. |
| `acceptance_highQ2_charged_mc_prototype.md` | PROTOTYPE | Pre-v5f prototype. |
| `acceptance_v5f_charged.md` | CURRENT | HQ-v5f-r1 acceptance memo. |
| `asymmetry_v5f_data.md` | SUPERSEDED | `multiplicities_cross_sections_A_h_v5f_no_ep0304_v2.md`. |
| `binning_study_highQ2.md` | SUPERSEDED | `binning_v5f_frozen.md`. |
| `binning_v5f_frozen.md` | CURRENT | Frozen binning memo. |
| `crad_v5f_per_bin.md` | CURRENT | Placeholder=1 per-bin memo. |
| `crad_v5f_strategy2.md` | CURRENT | Strategy 2 BLOCKED status. |
| `eps_dis_data_application_v5f_highQ2.md` | SUPERSEDED | `n_dis_eps_dis_v5f_HQv5fr1.md`. |
| `eps_dis_highQ2_mc_prototype.md` | PROTOTYPE | MC prototype. |
| `lumi_effective_per_period_v5f.md` | CURRENT | Lumi memo. |
| `migration_matrices_v5f.md` | CURRENT | Migration matrices memo. |
| `multiplicities_cross_sections_A_h_v5f_no_ep0304.md` | SUPERSEDED | `_v2.md`. |
| `multiplicities_cross_sections_A_h_v5f_no_ep0304_v2.md` | CURRENT | Supervisor-named authoritative. |
| `multiplicity_cross_section_v5f_data.md` | SUPERSEDED | `multiplicities_cross_sections_A_h_v5f_no_ep0304_v2.md`. |
| `multiplicity_highQ2_charged_mc_prototype.md` | PROTOTYPE | Pre-v5f prototype. |
| `n_dis_data_v5f_highQ2.md` | SUPERSEDED | `n_dis_eps_dis_v5f_HQv5fr1.md`. |
| `n_dis_eps_dis_v5f_HQv5fr1.md` | CURRENT | Supervisor-named. |
| `n_dis_prototype_highQ2_mc.md` | PROTOTYPE | Pre-v5f prototype. |
| `phi_h_moments_v5f.md` | SUPERSEDED | `phih_harmonics_v5f_no_ep0304_v3_coarse.md`. |
| `phih_harmonics_v5f_no_ep0304.md` | SUPERSEDED | `_v2.md`. v1 inconsistency confirmed by reading first 30 lines of `_v2.md` and the deprecation banner on the v1 md. |
| `phih_harmonics_v5f_no_ep0304_v2.md` | CURRENT | Fine-grid (336 cells). Confirmed by reading first 40 lines: STATUS: CURRENT banner; Supersedes phih_harmonics_v5f_no_ep0304.md. Headline message: fix improves positivity/method-agreement but does not reduce |A_1| saturation. |
| `phih_harmonics_v5f_no_ep0304_v3_coarse.md` | CURRENT (publishable headline) | Coarse 72-cell. Confirmed by reading first 40 lines: STATUS: CURRENT banner; Supersedes phi_h_moments_v5f.md. Headline message: coarsening preserves inclusive A_n^data and pushes more cells to full-8 phi survival but does NOT eliminate per-cell A_1 saturation. v3 is "the cleanest publishable binned product available with the present tooling". |
| `r_bc_v5f.md` | CURRENT | R_BC memo. |
| `systematics_v5f.md` | SUPERSEDED | `systematics_v5f_rigor_status_per_source.md` + `systematics_v5f_aggregated_v2.json`. |
| `systematics_v5f_aggregated.md` | SUPERSEDED | Same. |
| `systematics_v5f_beam_energy_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_charge_flip_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_crad_placeholder_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_crad_residual_ABC.md` | CURRENT | A_BC C_rad residual memo. |
| `systematics_v5f_hfs_scale_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_lar_angle_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_lar_escale_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_luminosity_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_model_dependence.md` | SUPERSEDED | `_v2.md`. |
| `systematics_v5f_model_dependence_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_nuclia_proton_omission.md` | SUPERSEDED | `_promoted_v2.md`. |
| `systematics_v5f_nuclia_proton_omission_promoted.md` | SUPERSEDED | `_promoted_v2.md`. |
| `systematics_v5f_nuclia_proton_omission_promoted_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_option_h_mask_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_perbin_LAr_HFS_beam.md` | CURRENT | per-bin audit memo. |
| `systematics_v5f_rigor_status_per_source.md` | CURRENT | rigor-status memo; cited as primary input by plan-finalization-r1 §0. |
| `systematics_v5f_track_eff_chargedep.md` | CURRENT | charge-dep audit memo. |
| `systematics_v5f_trigger_eff_v2.md` | CURRENT | per-source v2 memo. |
| `systematics_v5f_unfolding_spread.md` | SUPERSEDED | `_v2.md`. |
| `systematics_v5f_unfolding_spread_v2.md` | CURRENT | per-source v2 memo. |
| `unfolding_compare_v5f.md` | HISTORICAL_LOG | W3 unfolding compare. INDEX explicitly lists in audit-trail section. |
| `unfolding_compare_v5f_HQv5fr2.md` | UNCERTAIN | INDEX lists in audit-trail, but with the 2026-05-21 r2 freeze this may have promoted to CURRENT for r2 unfolding. **Supervisor must classify.** |
| `W4_deep_synthesis_v5f.md` | HISTORICAL_LOG | W4 deep synthesis (audit-trail per INDEX). |
| `W4_synthesis_v5f_analysis.md` | HISTORICAL_LOG | W4 synthesis (audit-trail per INDEX). |
| `W5_synthesis_v5f.md` | HISTORICAL_LOG | W5 synthesis (audit-trail per INDEX). |

### docs/plans/

| File | Disposition | Superseded by / note |
|------|-------------|----------------------|
| `plan-finalization-r1.md` | CURRENT | This is the r2 revision (filename retained from r1) — confirmed by reading first 100 lines. r2 supersedes r1 internally; supervisor-named current finalization plan. Self-declares supersession of `plan-highQ2-sidis-next-steps.md` ON TWO FRONTS ONLY (systematics rigorization push and F2-r2 binning amendment); other content remains in effect. |
| `plan-highQ2-sidis-next-steps.md` | UNCERTAIN | r5 of the older plan. plan-finalization-r1 §0 explicitly states: "This plan supersedes: plan-highQ2-sidis-next-steps.md r5 specifically on the following fronts: (1) the systematics rigorization push, including the 14-source v2 aggregated budget and the six v1 defect fixes, and (2) the F2-r2 binning amendment from HQ-v5f-r1 to HQ-v5f-r2." So the rest of r5 (gates G1A–G17, naming conventions, NuclIA workstream details, A_BC blinding policy, first-week sequencing, etc.) remains in effect. Cannot fully classify as SUPERSEDED. **Recommendation: keep CURRENT in docs/plans/ with a top-of-file pointer to plan-finalization-r1 for the two superseded fronts; do not move.** Marked UNCERTAIN until supervisor decides whether to fragment the doc or leave intact. |
| `plan-phih-systematics.md` | SUPERSEDED | Self-declares "SUPERSEDED (2026-05-19): Scope now covered by plan-lowQ2-convergence-v2.md (WP1-WP5)" — that low-Q2 plan doc does NOT exist in this workspace (it lives in `sidis_lowQ2/`). The high-Q2-side superseding doc is `plan-finalization-r1.md` **Phase 7** (`φ_h moments full extraction on HQ-v5f-r2 (EXTENDED path only)`), which covers the same scope (phi_h r2 extraction + real per-cell systematics replacing bounded prescriptions). Confirmed by reading the doc's first 130 lines and plan-finalization-r1 §1219–1223 (Phase 7 action items + critic gates CG-7.0..7.3). Safe to move to old/. |
| `plan-workspace-migration-sidis-highQ2.md` | HISTORICAL_LOG | Workspace migration plan dated 2026-05-20; migration completed per context-handoff §1 ("Workspace genealogy: This workspace was carved out of H1/sidis_lowQ2/ on 2026-05-20 per docs/plans/plan-workspace-migration-sidis-highQ2.md"). Plan is now executed history. |

### docs/handoffs/

| File | Disposition | Superseded by / note |
|------|-------------|----------------------|
| `decision-binning-amendment-HQv5fr2.md` | CURRENT | Supervisor decision record for the F2 amendment (cited by plan-finalization-r1 §0). |
| `handoff-HQv5fr2-rebuild-on-naf.md` | CURRENT | The active NAF-side rebuild handoff (cited by plan-finalization-r1 §0). |
| `lessons-learned-for-highQ2.md` | CURRENT | Reference document; no supersession evidence. |
| `memo-mc-truth-policy-charged-sidis.md` | CURRENT | Frozen MC-truth policy memo. |

### docs/references/

All 14 files reviewed. None has a superseding sibling; all serve as the
authoritative reference for a specific frozen convention or input table.
Specific note: `reference-highQ2-mc-luminosity.md` explicitly marks ITS OWN
Section 7 (Task P provisional values) as SUPERSEDED — that is internal
provenance, not file-level supersession.

| File | Disposition | Note |
|------|-------------|------|
| `reference-ABC-scope-and-blinding-highQ2-sidis.md` | CURRENT | F14 anchor reference. |
| `reference-exclusive-vmd-background.md` | CURRENT | Background physics ref. |
| `reference-gen-reco-matching-charged-sidis.md` | CURRENT | F9 matching ref. |
| `reference-highQ2-charged-sidis-analysis-cuts.md` | CURRENT | Authoritative cut list (cited by plan-finalization-r1 §0). |
| `reference-highQ2-crad-availability.md` | CURRENT | NORAD availability ref. |
| `reference-highQ2-grl-manifest.md` | CURRENT | GRL ref. |
| `reference-highQ2-mc-luminosity.md` | CURRENT | LMC ref (Task P §7 internally superseded). |
| `reference-highQ2-per-run-beam-energy.md` | CURRENT | Per-run beam energy ref. |
| `reference-LAr-electron-scale-highQ2-sidis.md` | CURRENT | LAr scale procedure ref. |
| `reference-naming-aliases-highQ2-sidis.md` | CURRENT | Naming aliases ref. |
| `reference-nuclia-correction-highQ2-sidis.md` | CURRENT | NuclIA ref. |
| `reference-sidis-frame-gamma-star-p.md` | CURRENT | F1/F15 frame ref. |
| `reference-sidis-systematics.md` | CURRENT | Systematics taxonomy ref. |
| `reference-trigger-semantics-highQ2-sidis.md` | CURRENT | Trigger ref. |

### docs/research/

All 18 files reviewed. Research artifacts are not subject to typical
supersession; status-snapshots are dated but remain the reference of record
for that date. The single 2026-05-19 status snapshot is dated and lives next
to a newer one (context-handoff.md) but is not invalidated.

| File | Disposition | Note |
|------|-------------|------|
| `research-4d-unfolding.md` | CURRENT | Reference research. |
| `research-background-sources-migration.md` | CURRENT | Reference research. |
| `research-beam-polarization-charge.md` | CURRENT | Reference research. |
| `research-binning-grading-framework.md` | CURRENT | Reference research. |
| `research-crad-for-charged-sidis.md` | CURRENT | Reference research. |
| `research-dis-kinematic-methods.md` | CURRENT | Reference research. |
| `research-ff-primary-hadron-convention.md` | CURRENT | Reference research. |
| `research-g1c-v5f-evls-period-mapping-bug.md` | CURRENT | Reference research (bug record). |
| `research-genreco-matching-critique.md` | CURRENT | Reference research. |
| `research-h1-1302-1321-charged-multiplicity.md` | CURRENT | Reference research (literature). |
| `research-h1-charged-particle-analysis.md` | CURRENT | Reference research (literature). |
| `research-h1-trigger-system.md` | CURRENT | Reference research. |
| `research-highQ2-sidis-extension.md` | CURRENT | Reference research. |
| `research-highQ2-status-snapshot-2026-05-19.md` | CURRENT | Dated snapshot; carries TODO-migration banner; superseded as live state by context-handoff.md but retained as the 2026-05-19 reference. Keep CURRENT (snapshot semantics). |
| `research-LT-separation-feasibility.md` | CURRENT | Reference research. |
| `research-optimal-4d-binning.md` | CURRENT | Reference research. |
| `research-optimal-photon-reconstruction.md` | CURRENT | Reference research. |
| `research-tracking-systematics-deep.md` | CURRENT | Reference research. |

### docs/reviews/

| File | Disposition | Note |
|------|-------------|------|
| `acceptance_report_v5e_100k.md` | HISTORICAL_LOG | Pre-v5f diagnostic on /tmp/v5e_verify100k.root at low-Q² bin edges (Q²=[5,10,20,50,100]); cited by context-handoff §189. v5b builder is now production target; this is documentation evidence only. Confirmed by reading first 15 lines. |
| `audit-proton-naming-highQ2-sidis.md` | CURRENT | Reference review (naming audit referenced by migration plan). |
| `critic-sidis-systematics-distilled.md` | CURRENT | Reference review. |
| `critique-finish-analysis-plan-physics.md` | CURRENT | Reference critique. |
| `critique-phih-convention-freeze-highQ2-sidis.md` | CURRENT | Reference critique. |
| `critique-plan-highQ2-sidis-next-steps-r1.md` | CURRENT | Reference critique on r1 of the next-steps plan; superseded by newer revisions but retained as the critic record that drove them. Keep CURRENT (review semantics). |
| `investigation-analyze-proton-highQ2-script.md` | CURRENT | Reference investigation. |
| `review-mc-physics-matching-pool.md` | CURRENT | Reference review. |
| `staleness-sweep-highQ2-sidis-scripts.md` | CURRENT | Reference review companion of the JSON sweep. |
| `v5d_vs_v5e_100k.md` | HISTORICAL_LOG | Pre-v5f side-by-side at low-Q² bin edges; confirmed by reading first 15 lines (uses Q²=[5,10,20,50,100]). v5b production target; documentation evidence only. |

### docs/specs/

| File | Disposition | Note |
|------|-------------|------|
| `spec-v5d-builder-v0-and-vmd-flags.md` | HISTORICAL_LOG | Builder spec from 2026-05-20. **Still cited in context-handoff.md lines 643–646** ("v5d and v5e specs are carried as documentation only; they are operationally equivalent to v5b for charged-particle SIDIS"). Per context: "the high-Q2 v5d / v5e port (Phase A + B + C per low-Q2 spec) is OUT OF SCOPE" for this analysis. Therefore: documentation-only, not actively executed. Recommend keep accessible (don't delete) but classification is HISTORICAL_LOG. |
| `spec-v5d-builder-v0-handling.md` | HISTORICAL_LOG | Same situation. Header self-declares "Supersedes spec-v5d-builder-v0-and-vmd-flags.md for V0 scope". Documentation-only. |
| `spec-v5e-addendum-gen-denominator-fix.md` | HISTORICAL_LOG | Addendum to v0-handling. Documentation-only. |

### docs/research-crad-v5f-attempt.md (top-level)

| File | Disposition | Note |
|------|-------------|------|
| `research-crad-v5f-attempt.md` | CURRENT | 2026-05-21 research artifact documenting the BLOCKED Strategy 2 C_rad attempt and listing 5 unverified assumptions A1–A5. Stream C / NORAD Condor build is the named ongoing blocker (still BLOCKED per `crad_v5f_strategy2.md` and context-handoff). This is the live blocker record. Keep CURRENT. |

### plots/ (directories only)

| Directory | Disposition | Superseded by / note |
|-----------|-------------|----------------------|
| `abc_dis_dedicated_unfolding/` | CURRENT | Plots for the CURRENT `abc_dis_dedicated_unfolding_v5f.json`. |
| `abc_dis_low_Q2_crad_bias/` | CURRENT | Plots for CURRENT crad-bias json. |
| `abc_dis_low_Q2_em06/` | CURRENT | Plots for CURRENT em06-attribution json. |
| `abc_dis_low_Q2_investigation/` | PROTOTYPE | Empty directory (0 B). |
| `abc_dis_stability_v5f/` | CURRENT | Plots for CURRENT stability json. |
| `abc_dis_v5f_FINAL/` | SUPERSEDED | `abc_dis_v5f_FINAL_no_ep0304/` (4-period FINAL). |
| `abc_dis_v5f_FINAL_no_ep0304/` | CURRENT | Headline A_BC^DIS plots. |
| `abc_dis_v5f_FINAL_postrecovery/` | SUPERSEDED | README explicitly states "Functionally identical to the pre-recovery plot set in ../abc_dis_v5f_FINAL/" (which is itself superseded). Move to old/. |
| `abc_dis_vs_epsilon/` | SUPERSEDED | `abc_dis_vs_epsilon_no_ep0304/`. |
| `abc_dis_vs_epsilon_no_ep0304/` | CURRENT | Headline vs-ε plots. |
| `acceptance_v5f/` | CURRENT | Acceptance plots, HQ-v5f-r1. |
| `asymmetry_v5f/` | SUPERSEDED | `asymmetry_v5f_no_ep0304_v2/`. |
| `asymmetry_v5f_no_ep0304/` | SUPERSEDED | `asymmetry_v5f_no_ep0304_v2/`. |
| `asymmetry_v5f_no_ep0304_v2/` | CURRENT | Headline A_h plots. |
| `binning_v5f/` | CURRENT | Binning plots tied to `binning_v5f_frozen`. |
| `crad_v5f/` | PROTOTYPE | Empty except README (no data to plot). README says: "empty (no plots produced; no data to plot)". |
| `crad_v5f_strategy2/` | CURRENT (empty) | Empty directory tied to the still-BLOCKED Strategy 2 production. Keep current as placeholder. |
| `ep0607_smoke_qa/` | HISTORICAL_LOG | Production smoke directories from ep0607 d06/d07. |
| `highQ2/` | HISTORICAL_LOG | Pre-v5f QA prototype plots (16 PNGs + PDF). Context-handoff §403 calls them "High-Q2 prototype QA plots". Listed in context-handoff §886 as a carried artifact. Largest single move target (~5.8 MB). |
| `migration_v5f/` | CURRENT | Plots for migration matrices CURRENT json. |
| `multiplicity_cross_section_v5f/` | SUPERSEDED | `multiplicity_cross_section_v5f_no_ep0304_v2/`. |
| `multiplicity_cross_section_v5f_no_ep0304/` | SUPERSEDED | `..._v2/` (per INDEX). |
| `multiplicity_cross_section_v5f_no_ep0304_v2/` | CURRENT | Headline multiplicity/cross-section plots. |
| `n_dis_eps_dis_v5f/` | CURRENT | Plots for CURRENT n_dis_eps_dis_v5f_HQv5fr1. |
| `phi_gap_diag/` | CURRENT | Plots for CURRENT phi_gap_attribution. |
| `phi_h_moments_v5f/` | SUPERSEDED | `phi_h_moments_v5f_no_ep0304_v3_coarse/` (4-period → 3-period coarse). |
| `phi_h_moments_v5f_no_ep0304/` | SUPERSEDED | `..._v2/` (fine, per INDEX) and `..._v3_coarse/` (coarse). |
| `phi_h_moments_v5f_no_ep0304_v2/` | CURRENT (fine-grid record) | Per INDEX, fine-grid `_v2` is retained alongside `_v3_coarse` (publishable headline). Keep CURRENT. |
| `phi_h_moments_v5f_no_ep0304_v3_coarse/` | CURRENT | Headline phi_h plots. |
| `r_bc_v5f/` | CURRENT | R_BC plots. |
| `systematics_HQv5fr2/` | UNCERTAIN | Contains `stack/` subdir of HQv5fr2 stack plots. Tied to `systematics_HQv5fr2_truncated.json` (UNCERTAIN). Same supervisor decision required. |
| `systematics_v5f/` | CURRENT | Aggregated systematics plots. Contains both `stack/` (v1) and `stack_v2/` (v2) subdirs. The `stack/` subdir is internally superseded by `stack_v2/`; consider moving `stack/` subdir to old/ but keep the parent dir CURRENT. |
| `systematics_v5f_crad_residual/` | CURRENT | Crad-residual plots tied to CURRENT json. |
| `systematics_v5f_nuclia/` | CURRENT | NuclIA plots tied to CURRENT promoted_v2 source. |
| `systematics_v5f_perbin/` | CURRENT | Per-bin LAr/HFS/beam plots tied to CURRENT audit json. |
| `unfolding_v5f/` | CURRENT | Unfolding closure/spread plots (W3). Companion of `unfolding_compare_v5f.md` (HISTORICAL_LOG). Plots themselves are referenced by the `unfolding_spread_v2` systematic still-CURRENT. Keep CURRENT. |
| `v5f_smoke_compare/` | HISTORICAL_LOG | v5b vs v5f port smoke comparison; companion of `docs/data/v5f_port_smoke_compare_2026-05-19.md`. |

---

## UNCERTAIN items requiring human review

Each item below states the specific question that supervisor must resolve.

1. **`docs/data/systematics_v5f.json`** — Marked "must not be used downstream" in
   supervisor context, yet the live v2 aggregator `scripts/aggregate_systematics_v5f_v2.py`
   reads it to source the `track_eff` flat-catalog entry (its `IN_CATALOG` constant).
   Multiple v2 per-source scripts also read it (for comparison metadata).
   **Question:** Should we (a) refactor `aggregate_systematics_v5f_v2.py` to host
   the `track_eff` flat number directly (or in a new `systematics_v5f_track_eff_v2.json`)
   and then move v5f.json to old/, or (b) keep v5f.json in place as a flat-catalog
   sidecar with a banner explaining its remaining role?

2. **`docs/data/systematics_HQv5fr2_truncated.json`** — Marked DEPRECATED 2026-05-22
   because r1 was production then. After the 2026-05-21 r2 binning freeze it is
   the only systematics product on the new frozen grid. Read by the live
   `scripts/plot_systematics_HQv5fr2_stack.py`.
   **Question:** Promote to CURRENT (HQ-v5f-r2 bridge budget pending full r2 rebuild)
   or genuinely SUPERSEDED?

3. **`docs/data/unfolding_compare_v5f_HQv5fr2.json`** and
   **`docs/data/unfolding_compare_v5f_HQv5fr2_analyzed.json`** —
   Same situation: marked DEPRECATED before the r2 freeze. After the freeze
   they are the active r2 unfolding comparison. **Question:** Promote to CURRENT
   or confirm SUPERSEDED?

4. **`docs/results/unfolding_compare_v5f_HQv5fr2.md`** — Same situation as #3.

5. **`docs/data/yields_v5f_3period_HQv5fr2.json`** — Marked DEPRECATED 2026-05-22
   but explicitly named in `docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md` line
   38 as the canonical output of `scripts/extract_yields_v5f_3period.py --tag HQv5fr2`
   (which is the active r2 rebuild handoff). **Question:** Confirm CURRENT (active
   r2 rebuild generation, not yet final) or genuinely SUPERSEDED?

6. **`docs/data/phih_harmonics_v5f_no_ep0304_v4_unbinnedML.json`** —
   **Not mentioned anywhere in the supervisor-provided current-state context.**
   File modification time is today (2026-05-22 09:51). Companion extractor
   `scripts/extract_phih_harmonics_v5f_v4_unbinned_ML.py` exists. No md companion
   in docs/results/; no INDEX.md entry; no deprecation banner. This is either a
   work-in-progress unbinned-ML extraction that *will* become the next current
   phih harmonics generation (the v3_coarse md narrative explicitly named this as
   the "definitive next step"), or it is an experiment that should be flagged.
   **Question:** Classify as CURRENT (work-in-progress, requires INDEX entry +
   md narrative), STAGING (keep but flag), or SUPERSEDED?

7. **`docs/plans/plan-highQ2-sidis-next-steps.md`** — plan-finalization-r1 explicitly
   states it supersedes this doc only on the systematics-rigorization push and
   the F2-r2 binning amendment fronts; the rest of r5 (gates G1A–G17, naming
   conventions, NuclIA workstream details, A_BC blinding policy, first-week
   sequencing) remains in effect. **Question:** Keep CURRENT in docs/plans/ with
   a clear top-of-file pointer to plan-finalization-r1 for the superseded
   sections, or fragment the doc and move the superseded sections to old/?

8. **`plots/systematics_HQv5fr2/`** — Same as item #2 (HQv5fr2 systematics dir).
   Promote to CURRENT or SUPERSEDED depending on supervisor decision on item #2.

9. **`plots/systematics_v5f/stack/`** (subdir within the still-CURRENT
   `plots/systematics_v5f/`) — Internally superseded by `plots/systematics_v5f/stack_v2/`
   (which is the v2 stack rendering). **Question:** Move this *subdirectory* to
   `old/plots/systematics_v5f/stack/` while keeping the parent CURRENT, or leave
   in place because it's used by the deprecated `plot_systematics_v5f_stack.py`
   (the non-v2 plotter)? Conservative recommendation: leave in place; the audit
   moves only top-level directories.

10. **`docs/data/acceptance_v5f_charged_HQv5fr2.json`** — INDEX called this
    "CURRENT (alt-binning study)". After the r2 freeze it's no longer "alt"; it
    is the new frozen-binning acceptance. The base file (`acceptance_v5f_charged.json`,
    no `_HQv5fr2` suffix) holds the r1 result and remains CURRENT per supervisor
    rule (the rule only displaces standalone files named `_HQv5fr1`, which this
    base file is not). **Question:** Is this representation accurate? Should
    INDEX.md be updated to call the r2 file simply "CURRENT (HQ-v5f-r2)" without
    the "alt-binning study" qualifier?

11. **All HQv5fr1-tagged data files for which an r2 counterpart does NOT yet
    exist** — per the supervisor rule: "if not, mark it 'current-but-stale-pending-r2-regen'".
    The only HQv5fr1-tagged file in scope that survives all other supersession
    rules is `n_dis_eps_dis_v5f_HQv5fr1.{json,md}`. plan-finalization-r1 §1.1
    gate G4 marks this as "Closed on r1; r2 assembly-open" and the r2 rebuild
    handoff is pending. Audit classification above: CURRENT. **Question:** Confirm
    that "CURRENT" is the right label rather than the supervisor's proposed
    "current-but-stale-pending-r2-regen" label — and if the latter, add the
    label as an explicit banner update separate from this audit.

---

## Cross-reference findings

Each bullet records: searched file → basename queried → hit context → conclusion.

- `scripts/aggregate_systematics_v5f_v2.py` → `systematics_v5f.json` → live `IN_CATALOG`
  constant; `track_eff` source #8 explicitly sourced from this file ("flat-v1 source {sid}
  not found in {IN_CATALOG.name}"). **Conclusion:** v1 JSON is NOT fully orphaned;
  triggered UNCERTAIN #1.

- `scripts/aggregate_systematics_v5f.py` (non-v2) → `systematics_v5f.json` →
  reads input from `IN_CATALOG  = DOCS_DATA / 'systematics_v5f.json'` and writes
  `systematics_v5f_aggregated.json`. **Conclusion:** This script is itself the
  v1 aggregator; the supervisor's "v1 must not be used downstream" rule applies
  to its output. The script itself is a SUPERSEDED one-shot; its existence does
  not block superseding the v1 JSON. (Different from v2 aggregator's continued
  read.)

- `scripts/build_systematics_HQv5fr2_truncated.py` → `systematics_v5f.json` →
  reads it as the source of HQ-v5f-r1 catalog medians to broadcast onto the r2
  truncated grid. **Conclusion:** Another live consumer of v1; reinforces
  UNCERTAIN #1. Also implies the HQv5fr2_truncated json is a live downstream
  derivative; reinforces UNCERTAIN #2.

- `scripts/plot_systematics_HQv5fr2_stack.py` → `systematics_HQv5fr2_truncated.json`
  → live plotter that reads the truncated systematics on HQv5fr2 grid.
  **Conclusion:** Reinforces UNCERTAIN #2 — file IS actively consumed.

- `scripts/compute_crad_residual_ABC_W4_deep_D.py` → `systematics_v5f.json` →
  reads `syst_path = DATA / "systematics_v5f.json"`. **Conclusion:** Another
  live consumer of v1; reinforces UNCERTAIN #1.

- `scripts/aggregate_perbin_LAr_HFS_beam.py` → `systematics_v5f.json` →
  uses it as comparison reference, not as a write-into target. **Conclusion:**
  Less critical consumer; still triggers UNCERTAIN #1.

- `scripts/compute_systematics_v5f_*_v2.py` (12 per-source scripts) → `systematics_v5f.json`
  → all read v1 as `OLD_PATH` for `_supersedes` provenance and for the "compare
  v1 vs v2" message in the markdown output. **Conclusion:** These are metadata
  reads, not numerics reads — they would still complete if v1 were renamed or
  moved (provided the supersedes string is replaced). Reinforces UNCERTAIN #1
  with a workable refactor path.

- `scripts/extract_phih_harmonics_v5f_v4_unbinned_ML.py` exists; output
  `docs/data/phih_harmonics_v5f_no_ep0304_v4_unbinnedML.json` exists with today's
  mtime; no md companion. **Conclusion:** Triggers UNCERTAIN #6.

- `docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md` line 38 lists
  `python3 scripts/extract_yields_v5f_3period.py --tag HQv5fr2` as the rebuild
  command producing `yields_v5f_3period_HQv5fr2.json`. **Conclusion:** Reinforces
  UNCERTAIN #5 — the HQv5fr2 yields file is the active r2 rebuild output, not
  an alt-binning study.

- `docs/handoffs/decision-binning-amendment-HQv5fr2.md` line 51 explicitly states
  the existing HQv5fr1-named JSONs "remain on the HQ-v5f-r1 grid. They are renamed
  nowhere; they continue to be authoritative for their existing 4 z bins."
  **Conclusion:** Confirms that HQv5fr1-tagged files are CURRENT on r1 and a future
  r2 rerun will produce a new generation. Reinforces UNCERTAIN #11 and supports
  the recommendation to label `n_dis_eps_dis_v5f_HQv5fr1` as CURRENT
  (or "current-but-stale-pending-r2-regen" per supervisor's preferred phrasing).

- `context-handoff.md` lines 643–646 → `spec-v5d-builder-v0-and-vmd-flags.md`,
  `spec-v5d-builder-v0-handling.md`, `spec-v5e-addendum-gen-denominator-fix.md`
  → "carried as documentation only". **Conclusion:** Confirms HISTORICAL_LOG
  disposition for all three specs — they are not actively executed in this
  workspace and the high-Q2 v5d/v5e port is OUT OF SCOPE per context-handoff
  lines 640.

- `context-handoff.md` lines 189–191 → `acceptance_report_v5e_100k.md` and
  `v5d_vs_v5e_100k.md` and `plots/highQ2/` and `plots/v5e_qa/` → carried
  artifacts from v5b/v5d/v5e era. `plots/v5e_qa/` does not exist (was not
  carried into this workspace; the dir listing showed no such directory).
  **Conclusion:** Confirms HISTORICAL_LOG for the two review files and for
  `plots/highQ2/`. (The non-existence of `plots/v5e_qa/` is a documentation
  drift in context-handoff that is out of scope for this audit.)

- `docs/data/deprecation_audit_2026-05-21.md` §6 + §10 → 21 files listed as
  "pending supervisor review" → all dispositioned 2026-05-22; **9 CURRENT, 4
  DEPRECATED, 8 AUDIT-TRAIL.** This audit's UNCERTAIN #2, #3, #4, #5 cover the
  4 DEPRECATED entries that need re-evaluation after the r2 freeze.

- `docs/results/phih_harmonics_v5f_no_ep0304_v2.md` (first 40 lines) →
  STATUS: CURRENT banner; Supersedes phih_harmonics_v5f_no_ep0304.md.
  Confirms v1 SUPERSEDED.

- `docs/results/phih_harmonics_v5f_no_ep0304_v3_coarse.md` (first 40 lines) →
  STATUS: CURRENT banner; Supersedes phi_h_moments_v5f.md. Self-narrative
  identifies itself as "the cleanest publishable binned product available
  with the present tooling" — confirms v3_coarse is the headline.

- `docs/plans/plan-phih-systematics.md` (first 130 lines) → carries its own
  "SUPERSEDED (2026-05-19)" banner pointing to `plan-lowQ2-convergence-v2.md`
  (a low-Q2 doc absent from this workspace). The effective high-Q2-side
  superseding plan is `plan-finalization-r1.md` Phase 7 (CG-7.0..CG-7.3 + action
  items 7.1–7.4 in §1216–1223). Safe to move to old/.

- `docs/plans/plan-finalization-r1.md` §0 + first 100 lines → r2 revision dated
  2026-05-22; supersedes plan-highQ2-sidis-next-steps r5 ONLY on systematics
  rigorization + F2-r2 binning fronts. Other content remains live. Triggers
  UNCERTAIN #7.

- `plots/abc_dis_v5f_FINAL_postrecovery/README.md` → "Functionally identical to
  the pre-recovery plot set in ../abc_dis_v5f_FINAL/". **Conclusion:** Both
  postrecovery and FINAL plot dirs are SUPERSEDED by the `_no_ep0304/` dir.

---

## End of audit

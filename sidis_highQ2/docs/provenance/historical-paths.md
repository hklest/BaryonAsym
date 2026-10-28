# Historical and Deprecated Path Provenance — High-Q² Charged-Particle SIDIS

**Purpose:** preserve audit visibility for paths that are deprecated, superseded, quarantined, or historical so startup docs can stay compact without losing information.

## Current Allow Paths

Use these for current central high-Q² charged-SIDIS v6/z005 work unless a task is explicitly historical/audit/provenance.

| Item | Path / value |
|---|---|
| Workspace | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/` |
| Stage 1 source | `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` |
| Stage 2 source | `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` |
| Condor root | `H1ChargedSidis_HighQ2/condor_v6/central/` |
| Output root | `H1ChargedSidis_HighQ2/output_v6/central/` |
| EVLS pattern | `evls_v6_*.root` |
| Current release | `output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/` (INSTALLED 2026-10-07 by checksum-verified copy, 71/71 files, manifest 38/38 hashes matched; coherent `body+miss` prior; source candidate retained intact in `data_release_candidates/` as promotion provenance) |
| Canonical local script snapshot | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/` |
| Canonical remote script snapshot | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/` |

## Deprecated / Deny Paths

Do not use these for central current work.

| Path / pattern | Status |
|---|---|
| `output/` v5g products | Historical/audit provenance only |
| `condor/` v5g products | Historical/audit provenance only |
| `condor_v6_central/` | Deprecated path; not the same as `condor_v6/central/` |
| `evls_highQ2_*.root` | Deprecated EVLS pattern |
| `migration_inputs_v3_percharge_NWfix_v2.root` | Pre-v6 response/migration product; historical/audit only |
| Any pre-v6 response/migration product | Historical/audit only unless explicitly requested |
| All `data_release_rev6_*` directories | Historical/audit; must not be promoted as current |
| `output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/` | Superseded as physics basis (2026-10-07); retained untouched as plain-prior provenance, historical/audit only, no new physics claims |
| `output_v6/central/data_release_candidates/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/` quoted as the current central release path | Promotion-provenance source only; the installed `data_release/` copy is canonical |
| `output_v6/systematics/shifted_electron_q_from_Q2Yphi/final_plots_2026-06-09/` | 240 rows/charge and not z005-compatible; superseded for z005 total summary |
| `output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/` | Invalid historical v4 TUnfold diagnostic; ordinary fake/miss-bin encoding |
| `output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/` | Historical/audit v5 TUnfold diagnostic; visible scale plus external `x/eps` |
| RooUnfoldSvd/TSVDUnfold products/scripts | Quarantined; not topology-safe for current multidimensional flattened bin array |
| Old hardcoded aggregate-lumi RAPGAP C_rad result | Superseded/quarantined; diagnostics only |
| `reco_backgrounds_alpha0p2_z005_20260624/`, `reco_backgrounds_alpha0p2_z005_final_20260625/`, earlier v2/v3/v4 period-scaled/raw-sum alpha0p2 variants | Superseded for nominal alpha0p2 use; audit provenance only |
| `background_vector_only_356pb` / `L356p613pb` low-Q² DJANGOH background package labels | Historical provenance labels using superseded 356.613 pb^-1 normalization; current reruns target 351 pb^-1 |

## Historical Milestones Preserved

- 2026-06-03: final-binning candidate built from rev6-era TUnfold AREA workflow; pT-only common merge with 157 bins per charge; now audit pending current z005 nominal-method decision.
- 2026-06-02: TUnfold rev6 AREA 210-bin filtered internal release and tau-opt closure scan; now historical/audit.
- 2026-05-31: rev6 NWfix_v2 canonical response and 4-critic chain audit; structural lessons remain relevant, products are not current z005 central outputs.
- 2026-05-29: earlier background-subtracted base and xsec/A_h derivations; superseded by current z005 release state.

## Archive Locations

- Pre-compaction startup-pipeline preservation: `docs/archive/pre-compaction-startup-pipeline-2026-07-03/`.
- Migration map for this compaction: `docs/archive/pre-compaction-startup-pipeline-2026-07-03/MIGRATION_MAP.md`.
- Manifest for preserved originals: `docs/archive/pre-compaction-startup-pipeline-2026-07-03/MANIFEST.md`.

## Cross-Analysis Boundary

The high-Q² charged-SIDIS workspace is independent of `proton_HiQ/`, `lambda_HiQ/`, and the shared V0/proton high-Q² workspace `H1BaryonAsym_HighQ2/`. Historical path aliases containing `H1SIDIS_HighQ2` or references to shared high-Q² locations must be checked against the current active workspace before use.

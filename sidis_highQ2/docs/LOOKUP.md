# LOOKUP — Where To Look for High-Q² Charged-Particle SIDIS

Use this map after reading `context-handoff.md` and `docs/RUN_STATUS.md`. It routes startup questions to compact current status files or preserved provenance without requiring the old long startup docs.

## Flowchart-Style Map

```text
Need to start a session?
  -> Read context-handoff.md
  -> Read docs/RUN_STATUS.md
  -> Read this LOOKUP.md

Need exact scripts / rerun sequence?
  -> Read RUNLIST.md in the canonical script snapshot first
  -> Local:  /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/
  -> Remote: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/
  -> Prefer reuse or slight modification of canonical scripts over writing new scripts

Need current release path, v6 roots, blockers, or completed systematics?
  -> docs/status/current-release.md

Need allowed vs denied paths or historical provenance?
  -> docs/provenance/historical-paths.md
  -> docs/archive/pre-compaction-startup-pipeline-2026-07-03/MANIFEST.md
  -> docs/archive/pre-compaction-startup-pipeline-2026-07-03/MIGRATION_MAP.md

Need luminosity, MC scaling, or the 7091 vs 7110 trap?
  -> docs/status/normalization.md

Need C_rad status?
  -> docs/status/radiative-corrections.md
  -> docs/radiative/djangoh14_norad_supplement_pipeline_2026-07-07.md (validated short-path DJANGOH14 non-rad supplement smoke and production-script critical path)
  -> xFGen<0 xFcut baseline (2026-10-05 tables) is the current PROVISIONAL DIAGNOSTIC baseline; blind 2026-09-30 FINAL quarantined
  -> RAPGAP31 fLumi-v3 and old aggregate-lumi results stay superseded/quarantined, not nominal/systematic
  -> Do not apply alpha0p2 reco-only cuts to gen-only C_rad

Need alpha0p2 near-electron/background/fake response state?
  -> docs/status/alpha0p2-background.md
  -> docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md

Need TUnfold / SVD status?
  -> docs/status/unfolding-tunfold.md
  -> Native ROOT/TUnfold v6 miss-underflow is the standard diagnostic/candidate-comparison entry point
  -> SVD is quarantined

Need data-vs-MC comparison plots?
  -> Use canonical snapshot script MC/compare_data_vs_mc_z005_4generator_born.py (PROMOTED 2026-10-10; supersedes MC/compare_data_vs_mc_z005.py, retained historical/audit)
  -> Working source: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py
  -> Use --differentiate-4d for full 4D differential plots, pass --prediction-csv per generator + --lumi-pb 351.0, state Born-vs-raw level, and do not use stale SHERPA z005_20260627_normfix input

Need plotting/script roots or alpha0p2 plot conventions?
  -> docs/status/alpha0p2-background.md
  -> canonical script snapshot RUNLIST.md

Need current release and production roots?
  -> docs/status/current-release.md
  -> docs/provenance/historical-paths.md

Need deprecated/deny paths?
  -> docs/provenance/historical-paths.md
```

## Quick Branches

| Looking for | Go to |
|---|---|
| Canonical scripts and run order | `scripts/canonical_highQ2_sidis_2026-06-30/RUNLIST.md` locally or remotely |
| Current z005 release path | `docs/status/current-release.md` |
| Current v6 roots | `docs/status/current-release.md`; `docs/provenance/historical-paths.md` |
| Current blockers | `docs/status/current-release.md` |
| Lumi and normalization traps | `docs/status/normalization.md` |
| 7091 vs 7110 | `docs/status/normalization.md` |
| DJANGOH/RAPGAP C_rad | `docs/status/radiative-corrections.md` |
| alpha0p2 diagnostics | `docs/status/alpha0p2-background.md` |
| all-fake response handoff | `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md` |
| Native v6 TUnfold | `docs/status/unfolding-tunfold.md` |
| SVD quarantine | `docs/status/unfolding-tunfold.md` |
| Deprecated/deny paths | `docs/provenance/historical-paths.md` |
| BSA side study (result, method, traps) | `docs/bsa/RESULTS_bsa_araw_2026-09-16.md`; `context-handoff.md` §BSA |
| BSA systematics plan | `docs/bsa/systematics_plan_bsa_2026-09-16.md` |
| BSA scripts / data / plots | `scripts/bsa/`; `data/bsa/`; `plots/bsa/` |
| Old startup docs before compaction | `docs/archive/pre-compaction-startup-pipeline-2026-07-03/` |
| Migration map from old sections to new destinations | `docs/archive/pre-compaction-startup-pipeline-2026-07-03/MIGRATION_MAP.md` |

## Safe Discoverability Rules

- If a fact affects startup routing, keep it in `context-handoff.md` or `docs/RUN_STATUS.md` and link to detail here.
- If a fact is current but detailed, keep it in `docs/status/*.md`.
- If a fact is non-current, superseded, or audit-only, keep it in `docs/provenance/` or `docs/archive/`.
- If a task asks for a full reproduction, begin with canonical script snapshot `RUNLIST.md`, not with old roadmap prose.

# BaryonAsym

Repository for H1 Lambda/K0 baryon asymmetry analysis.

## Unified orchestration

- `pipeline/run_master.sh`: submit/monitor end-to-end stages across data + MC.
- `pipeline/config.sh`: stage submit-file configuration.

## Unified directory migration target

- `analysis_unified/` provides a single mode-based interface (`data`/`mc`) and a ROOT packaging macro that merges outputs into one tree file layout:
  - `data/rec`
  - `mc/rec`
  - `mc/gen`

Start here:

- `analysis_unified/run_unified_pipeline.sh`
- `analysis_unified/README.md`

Note: the master pipeline can auto-rewrite missing Condor executable paths using fallback binary directories configured in `pipeline/config.sh` (helpful when EL9/CentOS binary paths drift).


If submissions start but process 0 events, check for missing steering-file warnings in logs; the pipeline now prechecks steering presence and output directories before submit.

Use `pipeline/config.sh` toggles (`PRECHECK_STEERING_AND_OUTPUTS`, `AUTO_GENERATE_MINIMAL_STEERING`, `WAIT_BETWEEN_STAGES`) to avoid zero-event jobs from missing steering files and to enforce stage barriers.

No gtustin paths are used by default in pipeline executable fallbacks, path rewrites, or steering generation.

Executables are rebuilt by default before run via `scripts/run_h1_executable.sh` (`AUTO_REBUILD_EXECUTABLES=1`) using local klesthen sources.

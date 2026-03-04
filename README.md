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

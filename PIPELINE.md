# Unified pipeline (data + MC)

This repository includes a single master driver for running both data and MC chains:

- `pipeline/run_master.sh`
- `pipeline/config.sh`

## New merged-analysis migration path

To move away from split `H1LambdaC` + `H1LambdaC_MC` trees/scripts, use:

- `analysis_unified/run_stage.sh` (mode-based stage runner)
- `analysis_unified/run_unified_pipeline.sh` (submit + package wrapper)
- `analysis_unified/build_unified_treefile.C` (single ROOT file packaging)

`build_unified_treefile.C` writes TTrees into:

- `data/rec`
- `mc/rec`
- `mc/gen`

This keeps MC-specific information explicit while avoiding duplicate end-user workflows.

## What `run_master.sh` does

- Submits data and MC Condor jobs stage-by-stage:
  - event lists (`create_evls`, `create_evlsMC`)
  - reco trees (`lambdaTree`, `kaonTree`, `lambdaTreeMC`, `kaonTreeMC`)
  - MC gen tree (`genMCTree`)
- Optional job monitoring via `condor_q`.
- Optional post-processing hooks (`hadd`, analysis) via config.
- Minimal-mode submissions (`Queue 1`) for smoke tests.

## Quick start (smoke test)

```bash
./pipeline/run_master.sh --minimal 1 --do-analysis 0 --dry-run 1
./analysis_unified/run_unified_pipeline.sh --dry-run 1
```

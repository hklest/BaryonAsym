# Unified pipeline (data + MC)

This repository now includes a single master driver for running both data and MC chains in one place:

- `pipeline/run_master.sh`
- `pipeline/config.sh`

The master script is intentionally conservative and keeps all existing H1 binaries/submit files intact.
It orchestrates the existing interfaces rather than replacing them.

## What it does

- Submits data and MC Condor jobs stage-by-stage:
  - event lists (`create_evls`, `create_evlsMC`)
  - reco trees (`lambdaTree`, `kaonTree`, `lambdaTreeMC`, `kaonTreeMC`)
  - MC gen tree (`genMCTree`)
- Optional job monitoring via `condor_q`.
- Optional post-processing hooks (`hadd` commands and analysis commands) via config.
- Minimal-mode submissions (`Queue 1`) for smoke tests.

## Quick start (smoke test)

```bash
./pipeline/run_master.sh --minimal 1 --do-analysis 0
```

## Useful flags

- `--do-create-evls 0|1`
- `--do-lambda-tree 0|1`
- `--do-kaon-tree 0|1`
- `--do-gen-tree 0|1`
- `--do-hadd 0|1`
- `--do-analysis 0|1`
- `--do-wait 0|1`
- `--dry-run 0|1`
- `--poll-seconds <N>`
- `--timeout-seconds <N>`

## Configuration

Edit `pipeline/config.sh` to choose which submit files and post-processing commands are run.
The default configuration is intentionally minimal and safe for iterative testing.

## Submit-script cleanup

The duplicated environment/bootstrap logic in submit wrappers has been consolidated:

- shared env setup: `scripts/common_h1_env.sh`
- shared executable wrapper: `scripts/run_h1_executable.sh`

Existing submit wrapper paths are preserved; they now delegate to shared scripts.

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


## Executable path auto-fix (for EL9/CentOS path drift)

`run_master.sh` now preflights each Condor submit file executable.
If the submit file references a missing binary, it attempts to rewrite the executable path
by matching the binary basename (e.g. `create_evls`) in fallback directories from `pipeline/config.sh`
(`PIPELINE_EXEC_SEARCH_DIRS`).

- Keep `AUTO_FIX_EXECUTABLE=1` (default) to enable this behavior.
- Add your local binary directories to `PIPELINE_EXEC_SEARCH_DIRS` if needed.
- In `--dry-run 1`, missing executables only warn and continue.

This addresses failures like:
`ERROR: Executable file ... does not exist`.


## Troubleshooting: jobs run but process 0 events

If logs show messages like:

- `Could not open steering file ...`
- `Could not correctly open H1Tree: no files were given`

it usually means steering files were not generated (or submit files point to the wrong path).

`run_master.sh` now prechecks submit arguments and will:

- fail fast in non-dry-run when steering files are missing,
- create missing output directories for `-o` paths,
- apply configurable path rewrites (`PIPELINE_PATH_REWRITES`) for migrated working areas.

Before submitting, generate the steering files via the existing scripts under:

- `H1LambdaC/Steering/*/create_*_steerings*.sh`
- `H1LambdaC_MC/Steering/**/create_*steerings*.sh`

Then rerun a dry-run and confirm no missing-steering warnings appear.


Additional knobs in `pipeline/config.sh`:

- `PRECHECK_STEERING_AND_OUTPUTS=1`: validate `-f` steering and ensure `-o` output directories.
- `AUTO_GENERATE_MINIMAL_STEERING=1`: for missing common 2006 process-0 steering files, auto-generate minimal steering snippets in non-dry-run.
- `WAIT_BETWEEN_STAGES=1`: wait for each stage block to finish before submitting dependent stages.


## Steering generation from event lists

After `create_evls` stages complete, the master pipeline now calls:

- `analysis_unified/generate_tree_steering_from_eventlists.sh`

This creates `Tree06_*.steer` and `TreeMC_RAPGAP31_06_RAD_*.steer` from existing event-list ROOT files in the **klesthen** directory tree.

No `gtustin` paths are used by default anymore.


## Executable rebuild policy

`scripts/run_h1_executable.sh` now attempts to rebuild the requested executable target by default (`AUTO_REBUILD_EXECUTABLES=1`) from the local klesthen repository (`H1LambdaC` or `H1LambdaC_MC`) before execution.

This avoids depending on stale binaries from other user areas.

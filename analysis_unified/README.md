# Unified analysis layer (data + MC)

This directory is the migration target for collapsing `H1LambdaC` and `H1LambdaC_MC` workflows into one mode-driven interface.

## Components

- `run_stage.sh`:
  - One stage runner with `--mode data|mc` and `--stage evls|lambda|kaon|gen`.
  - Preserves existing H1 executables and argument conventions.
- `run_unified_pipeline.sh`:
  - Orchestrates submit stages through the existing master pipeline.
  - Optionally packages data(rec), mc(rec), mc(gen) TTrees into one file.
- `build_unified_treefile.C`:
  - ROOT macro that writes TTrees into a single structure:
    - `data/rec/*`
    - `mc/rec/*`
    - `mc/gen/*`

## Why this is safe

- Existing binary interfaces are untouched.
- Existing `H1LambdaC` and `H1LambdaC_MC` directories remain available during migration.
- This provides a single canonical "merged" entrypoint now, with full source consolidation as a follow-up step.

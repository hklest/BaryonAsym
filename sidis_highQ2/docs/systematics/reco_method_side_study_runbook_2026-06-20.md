# Reco-Method Side-Study Runbook — z005 High-Q² SIDIS

**Created:** 2026-06-20

## Purpose

This runbook defines the reliable recipe for z005 reconstructed-kinematic-method side studies in high-Q² charged SIDIS. Use it when comparing the central eΣ reconstruction against alternate reconstructed DIS methods such as electron and IΣ while keeping the generator-side definition unchanged.

Do not run remote production from this document alone. Treat it as the checklist that must be satisfied before, during, and after Condor production.

## Read First

1. `H1/CONTEXT.md`
2. `H1/sidis_highQ2/context-handoff.md`
3. `H1/sidis_highQ2/docs/RUN_STATUS.md`
4. `H1/sidis_highQ2/docs/systematics/z005_shifted_systematics_pipeline_2026-06-13.md`
5. This runbook

If a newer run-status or handoff entry conflicts with this file, follow the newer explicit z005 guidance and update this runbook.

## Scope

Use this workflow for reco-method side studies that must mirror the eta, lab-pT, and DCA cut-variation studies:

1. vary reconstructed DIS kinematics;
2. recompute reco-side SIDIS observables;
3. build study-local migration matrices;
4. build study-local data vectors;
5. merge response plus data vector;
6. unfold with the validated IBU/BBB machinery;
7. compare to the central z005 release;
8. plot, package, and fetch to the local study plot directory.

The gen side stays unchanged. Do not change generator fiducial branches or truth definitions for this reco-method study.

## Required Branch Semantics

### Central eΣ Baseline

The central z005 release uses the established eΣ baseline. A reco-method side study must show exactly where it departs from that baseline in both the migration builder and the data-vector builder.

### Electron Method

Use electron-method reconstructed kinematics on the reco side:

- `Q2 = Q2e`
- `y = Ye`
- `x = Q2e / (4 * BeamElecE * BeamProtP * Ye)`
- construct `q` from the reconstructed scattered-electron four-vector;
- recompute reco-side `z`, `pT_h`, and `xF` from that `q`;
- leave gen-side observables unchanged.

Do not use `Q2es` or `Yes` for the electron variant except as audit comparators. Prior branch audit found `Q2e == Q2es`, but `Ye` differs from `Yes`; therefore labels alone can hide a real method-routing failure.

### IΣ Method

Use IΣ reconstructed kinematics on the reco side:

- `Q2 = Q2s`
- `y = Ys`
- `x = Q2s / (4 * BeamElecE * BeamProtP * Ys)`
- construct `q` from `(Q2s, Ys, phi_e)` using the established helper logic;
- recompute reco-side `z`, `pT_h`, and `xF` from that `q`;
- leave gen-side observables unchanged.

`Xs` is absent in the current tree schema. Compute `x` on the fly; do not look for or require an `Xs` branch. Prior branch audit found `Q2s` and `Ys` differ from eΣ and are the correct IΣ branch pair.

## Branch Audit Gate

Before any production-sized side study, run a one-file branch audit and save the result in the study `_run/` area. The audit must confirm:

- required reco branches exist: `Q2e`, `Ye`, `Q2s`, `Ys`, `BeamElecE`, `BeamProtP`, and the electron four-vector or electron angle/azimuth inputs used by the q helper;
- `Xs` is absent or ignored, with x computed from `Q2/(4 E_e P_p y)`;
- `Q2e == Q2es` behavior is understood and documented;
- `Ye` differs from `Yes` on selected events;
- `Q2s/Ys` differ from eΣ on selected events;
- gen-side branch use is unchanged from the central z005 pipeline.

Failure handling: if the audit cannot prove method-sensitive branch routing, stop. Do not submit Condor production.

## Method-Switching Requirements

A reco-method study is valid only if both reco-side products are switched:

1. **Migration builder:** per-file migration ROOTs must use the selected reco method for reco binning and reco SIDIS observables.
2. **Data-vector builder:** data vectors must use the same selected reco method for reco binning and reco SIDIS observables.

Do not accept a script that merely parses `--reco-method`. Verify that the argument is passed into `process_file` or the equivalent worker function and is not reset to default eΣ internally. A previous failure parsed `--reco-method` but never propagated it, making electron and IΣ data vectors silently identical.

## Smoke-Test Gate

Before full Condor production, run a bounded one-file smoke for each candidate method and compare to central/eΣ. The smoke must show a nonzero method effect in at least one method-sensitive diagnostic:

- event/bin assignment changes;
- data-vector entry changes;
- migration reco-bin population changes;
- recomputed `z`, `pT_h`, or `xF` changes.

If electron and IΣ outputs are byte-identical or numerically identical after a method switch, treat the run as failed until branch routing proves otherwise. Do not trust output labels as evidence of method switching.

## Data-Vector Schema Contract

The z005 data-vector JSON uses a top-level method tag and a dict-of-arrays payload:

- `reco_method` is a top-level field, not nested under `metadata`;
- `per_bin` is a dictionary of six arrays;
- `len(per_bin) == 6` is expected and is not a failure;
- each array inside `per_bin` must have length `980`;
- total data-vector length for z005 is `980`.

Validation must check the lengths of the six arrays, not compare `len(per_bin)` to `980`.

## Per-File Migration And Merged Response Contract

For each reco-method variant, require:

- per-file migrations complete for both MC generators and both charges: `4 + 4` expected category outputs per variant;
- generator labels are explicit: use `--mc-label rapgap31` or `--mc-label djangoh14`;
- corrected output namespaces use the actual labels `rapgap31/` and `djangoh14/`, or documented compatibility symlinks if a legacy merge script expects `rapgap/` and `djangoh/`;
- merged ROOT contains the expected TH2D objects such as `h_R_pos_*` and `h_R_neg_*`;
- response shape is `(491,246)` per charge;
- binmap contract is `NB_R=490`, `NB_G=245`;
- data vector length is `980`;
- IBU/BBB downstream products produce `490` published rows.

A previous corrected v2 side-study namespace reportedly reached these pre-release validation targets for electron and IΣ data vectors, migrations, and merged response QA. Treat that as validation evidence for the side-study inputs only; do not call the physics release complete without the final production/release gate.

## Condor Submit Checklist

Use Condor for every multi-file migration, merge, unfold, or release step. The login node is only for one-file checks under the H1 workspace policy.

Before `condor_submit`, verify:

- output directories, work directories, and log directories already exist;
- wrapper executable path is absolute and executable;
- `InitialDir` is a DUST directory for the study, not a local path;
- submit paths are exact; avoid submit-time globs;
- `Executable` points to the wrapper, not `/bin/bash` with empty `Arguments=-lc`;
- if using `/bin/bash -lc`, the command string is present and quoted;
- `--mc-root-files` glob is quoted as one argv token;
- `--mc-label <rapgap31|djangoh14>` is provided explicitly;
- do not use `+Owner=klesthen`;
- do not use broad submits or broad `condor_rm` patterns;
- never use `condor_rm <user> <id>`; use cluster-id-only or a constrained, Cmd-filtered removal.

Failure handling: if a submit file expands globs into many argv tokens, omits `--mc-label`, or has an empty shell command, fix the wrapper/submit and revalidate with a one-job smoke before broad submission.

## Merge And Release Path Conventions

All reco-method side-study outputs must be study-local under `output_v6/studies/`, not under the central z005 release or central migration roots.

Recommended namespace pattern:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_method_<method>_z005_<date>/
```

Within each study root, keep:

- `_run/` for wrappers, submits, logs, audits, validation JSON, and provenance;
- `per_file_migration/rapgap31/` and `per_file_migration/djangoh14/` for migration fragments;
- `data_vector/` for data-vector JSON and validation;
- `merged_response/` for merged ROOT and binmap QA;
- `data_release/` for IBU/BBB release outputs;
- `plots/` or a packaged plot bundle for remote plots.

The merge step must receive the actual data-vector JSON as `--data-json`. Do not pass a validation JSON in its place.

## Reuse Existing Cut-Variation Machinery Carefully

Reuse the eta, lab-pT, and DCA side-study machinery wherever possible because it already encodes the z005 release, IBU/BBB, QA, plotting, and fetch pattern. Before reusing any copied script or submit file, verify:

- paths are study-local, not central;
- `--reco-method` is wired into both migration and data-vector workers;
- method-specific q construction is active;
- MC labels and generator subdirectories match the current `rapgap31`/`djangoh14` naming;
- merge arguments point to the real data-vector JSON;
- validation uses the z005 980/490/245 contract.

Do not copy a cut-variation wrapper and assume it is safe by label alone.

## Plotting And Fetch Convention

Remote plots should stay inside the study root. Local fetched plots should use an explicit local study directory such as:

```text
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_method_z005_<date>/
```

Always fetch bundles to an absolute local path and extract them locally. Do not leave only a tarball if the task requested reviewable plots.

## Do Not Do

- Do not write study migrations, data vectors, merged responses, or releases into central output paths.
- Do not trust filenames, labels, or `reco_method` metadata without branch-routing and smoke-test evidence.
- Do not compare electron vs IΣ if both were produced by a script that defaulted internally to eΣ.
- Do not pass validation JSON as `--data-json`; pass the actual data-vector JSON.
- Do not use the 448-row central compare file for z005 side-study comparisons; z005 release comparisons require the current 490-row contract.
- Do not compare `len(per_bin)` to `980`; check each `per_bin` array length instead.
- Do not run multi-file merges or production-like releases on the login node.
- Do not use `condor_rm <user> <id>` or broad user-wide removals.
- Do not use unquoted `--mc-root-files` globs.
- Do not provide bare positional MC labels; use `--mc-label` explicitly.
- Do not use legacy `rapgap/` and `djangoh/` subdir assumptions unless compatibility symlinks or fixed merge arguments are documented.

## Final Acceptance Checklist

A reco-method side study is ready for interpretation only if all are true:

- branch audit saved and proves method-sensitive branch availability;
- migration builder and data-vector builder both use the selected method;
- one-file smoke shows a nonzero method effect before production;
- data-vector JSON has top-level `reco_method` and six `per_bin` arrays of length `980`;
- per-file migrations are complete for both charges and both MC generators;
- merged response ROOT has expected TH2D objects and response shape `(491,246)`;
- binmap reports `NB_R=490`, `NB_G=245`;
- IBU/BBB outputs have `490` published rows;
- comparison uses the current central z005 release, not older 448-row or 240-bin products;
- all remote outputs are study-local under `output_v6/studies/`;
- plot bundle is fetched and extracted under an absolute local `sidis_highQ2/plots/...` path;
- provenance records the exact scripts, wrapper paths, submit files, cluster IDs, and validation JSONs.

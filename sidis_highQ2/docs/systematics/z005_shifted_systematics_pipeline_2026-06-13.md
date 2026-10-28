# z005 Shifted Systematics Pipeline Runbook (2026-06-13)

## Purpose

Fixed runbook for the high-Q² charged-SIDIS z005 shifted-systematics pipeline. This document is written to be followed step-by-step by a non-reasoning agent. It covers the accepted z005 workflow state as of 2026-06-13 for:

- shifted detector systematics already accepted into the z005 total summary;
- total uncertainty assembly for z005;
- required validation checks;
- failure handling;
- exact known script paths and output paths.

Use verified facts from this workspace state only. Do not improvise alternate binning, alternate releases, or older diagnostic products.

## Read First

1. `H1/CONTEXT.md`
2. `H1/sidis_highQ2/context-handoff.md`
3. `H1/sidis_highQ2/docs/RUN_STATUS.md`
4. This runbook

If any of those files disagree, prefer the newest 2026-06-13 z005 statements in the handoff and run-status files.

## SSH Safety Precheck And NAF Rules

Run this once before any remote action:

```bash
ssh -O check naf-hone.desy.de 2>&1
```

Rules:

- If the control-socket check fails, stop immediately and ask the user to re-establish SSH manually.
- Do not retry failed SSH commands.
- Check connectivity once per dispatch.
- The login node is only for interactive single-file checks, file listings, `du`, `scp`, and environment checks.
- Anything multi-file, production-like, merge-like, or longer than 5 minutes must go through Condor.
- H1OO setup on NAF, when needed:

```bash
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
```

- Compile with:

```bash
make bin
```

## Canonical z005 Contract

Use this nominal release only:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

Required contract:

- `NB_G_per_charge=245`
- `NB_R_per_charge=490`
- response shape per charge: `(491,246)`
- data vector length: `980`
- nominal IBU selected iteration: `k=4`
- published rows: `490` total
- IBU rows: `490`

Do not substitute any older 240-bin-per-charge product.

## Canonical Sources And Tags

### Current central code / production tags

- Stage 1 source: `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`
- Stage 2 source: `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`
- Condor root: `condor_v6/central/`
- Output root: `output_v6/central/`

### Shifted tags accepted into final z005 total

- `hfs_jes_down`
- `hfs_rces_up`
- `hfs_theta_up`
- `elecE_p1pct`
- `elecTheta_p1mrad`

### Explicit non-final note

`hfs_theta_down` exists historically, but it was **not incorporated into the accepted final total state** as of 2026-06-13. Do not include it unless it is independently validated and an envelope convention is explicitly defined.

## Inputs

### Nominal input

- Nominal release root:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

- Nominal IBU cross sections JSON used by total summary:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/ibu/cross_sections.json
```

### HFS shifted-release inputs

- JES down release:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_jes_down/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

- JES comparison JSON:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_jes_down/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/comparison_to_nominal_ibu_hfs_jes_down.json
```

- RCES up release:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_rces_up/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

- RCES comparison JSON:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_rces_up/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/comparison_to_nominal_ibu_hfs_rces_up.json
```

- Theta up release:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_theta_up/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

- Theta comparison JSON used by the total builder:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_theta_up/_run/comparison_to_nominal_ibu_hfs_theta_up.json
```

### Electron shifted-release inputs

Use the re-evaluated z005 shifted releases only:

- Electron energy release:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/elecE_p1pct/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

- Electron theta release:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/elecTheta_p1mrad/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

Known workflow script for electron shifted release generation:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/_run/shifted_release_workflow/run_electron_shifted_release_v4.sh
```

### Total-summary generator input script

Use this exact script for the accepted total with electron and luminosity:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/run_electron_compare_validate_total_lumi.py
```

### Luminosity component

Use:

- `lumi_rel=0.027`

## Outputs

### Shifted releases

- HFS JES-down release:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_jes_down/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`
- HFS RCES-up release:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_rces_up/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`
- HFS theta-up release:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_theta_up/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`
- Electron energy release:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/elecE_p1pct/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`
- Electron theta release:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/elecTheta_p1mrad/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`

### Total summary output

Remote total-summary root:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/
```

Expected products in that directory:

- `total_uncertainty_summary.json`
- `total_uncertainty_summary.csv`
- `provenance.json`
- `validation_checks.json`
- `total_uncertainty_summary.pdf`
- `png/total_uncertainty_summary_neg.png`
- `png/total_uncertainty_summary_pos.png`
- `png/total_uncertainty_summary_percentiles.png`
- `total_uncertainty_summary_bundle.tar.gz`

## Canonical Outputs As Of 2026-06-13

- Remote total-summary path:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/
```

- Local extracted path:

```text
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/hfs_systematics/total_uncertainty_z005_with_electron_lumi_2026-06-13/
```

- Local zoom path:

```text
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/hfs_systematics/total_uncertainty_z005_with_electron_lumi_2026-06-13/zoom_0to15pct/
```

If a bundle is pulled locally, extract it locally as well. Do not stop at only downloading the tarball.

## Workflow Overview

Execute in this order:

1. SSH safety precheck.
2. Confirm the nominal z005 contract.
3. Validate each shifted release against the z005 contract.
4. Generate or confirm comparison products for shifted releases.
5. Build the total summary including luminosity.
6. Pull the resulting bundle locally if required.
7. Extract the local bundle.
8. Verify final outputs and row counts.

## Step 1 — Validate Nominal z005 Release Contract

Confirm all of the following before using any shifted source:

- nominal release path matches exactly;
- nominal IBU JSON has `490` rows;
- contract is `245` gen bins per charge and `490` reco bins per charge;
- response shape is `(491,246)` per charge;
- data vector length is `980`;
- selected IBU iteration is `k=4`;
- IBU rows are `490`.

Acceptance checks:

- response shape `(491,246)` per charge;
- data vector length `980`;
- IBU rows `490`;
- selected `k=4`.

Failure handling:

- If any nominal contract item does not match, stop. Do not compare shifted outputs to that nominal release.

## Step 2 — Validate Shifted sidistree Counts Where Known

Use the accepted counts where applicable:

- data sidistree count: `660`
- RAPGAP sidistree count: `364`
- DJANGOH sidistree count: `501`

Apply these checks to the HFS and electron shifted workflows where those counts are part of the accepted completed workflow state.

Acceptance checks:

- data = `660`
- RAPGAP = `364`
- DJANGOH = `501`

Failure handling:

- If a shifted workflow produces different known sidistree counts in a place where these counts are expected, stop and treat the shifted release as non-matching until the mismatch is explained.

## Step 3 — Run Or Confirm Shifted Releases

### 3A. HFS shifted releases

Verified wrapper location pattern for RCES and theta:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/<tag>/_run/shifted_release_workflow/
```

Use the accepted tags only:

- `hfs_rces_up`
- `hfs_theta_up`

For JES-down, use the accepted JES-down workflow under the HFS systematics area; keep the final release and comparison paths exactly as listed in this runbook.

Known cluster provenance from completed runs:

- HFS JES merge/release clusters: `3683317`, `3683318`
- HFS RCES clusters: `3690894`, `3690895`, `3690923`, `3690924`
- HFS theta-up clusters: `3690975`, `3690976`, `3691064`, `3691067`

### 3B. Electron shifted releases

Use this exact workflow script:

```bash
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/_run/shifted_release_workflow/run_electron_shifted_release_v4.sh
```

Accepted electron tags:

- `elecE_p1pct`
- `elecTheta_p1mrad`

Known successful release cluster:

- `3691374`

### 3C. Required shifted-release acceptance checks

For each shifted release, verify:

- sidistree counts where known: data `660`, RAPGAP `364`, DJANGOH `501`;
- migration response shape `(491,246)` per charge;
- data vector length `980`;
- IBU rows `490`;
- selected `k=4`.

Failure handling:

- If any of the above checks fail, stop before computing the comparison or total summary.
- Do not substitute an older release.
- Do not use a 240-row-per-charge electron product.

## Step 4 — Build Or Confirm Comparison Products

Comparison products must be bin-aligned to the nominal z005 release.

### Required comparison files

- JES:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_jes_down/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/comparison_to_nominal_ibu_hfs_jes_down.json`
- RCES:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_rces_up/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/comparison_to_nominal_ibu_hfs_rces_up.json`
- Theta:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/hfs_theta_up/_run/comparison_to_nominal_ibu_hfs_theta_up.json`

Acceptance checks:

- comparison rows = `490`;
- all values finite;
- bins aligned to nominal.

### Special handling for HFS theta

The HFS theta comparison has a JSON-encoded `bin_key`.

Required alignment rule:

- parse `bin_key`;
- align by `(parsed.charge, parsed.bin_index)`;
- do not align by row order;
- do not align by the raw `bin_key` string alone.

Failure handling:

- If parsed `(charge, bin_index)` keys do not cover all `490` rows exactly once, stop.

## Step 5 — Build The Total Summary

Use this exact script:

```bash
python /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/run_electron_compare_validate_total_lumi.py
```

Use the accepted inputs only:

- nominal z005 `ibu/cross_sections.json`
- HFS JES comparison JSON
- HFS RCES comparison JSON
- HFS theta comparison JSON
- z005 electron shifted releases / accepted electron comparison products used in the completed workflow
- luminosity component `lumi_rel=0.027`

What this accepted total state includes:

- IBU statistical
- IBU model
- IBU regularization
- HFS JES (`hfs_jes_down`) as one-sided absolute shift
- HFS RCES (`hfs_rces_up`) as one-sided absolute shift
- HFS theta (`hfs_theta_up`) as one-sided absolute shift
- electron energy (`elecE_p1pct`)
- electron theta (`elecTheta_p1mrad`)
- luminosity (`0.027`)

What this accepted total state does not promote as symmetric detector envelopes:

- `hfs_theta_down`
- any unvalidated opposite-side HFS variant

## Step 6 — Validate Total Summary Outputs

Required output directory:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/
```

Acceptance checks:

- total summary rows = `490`;
- all components finite;
- total quadrature finite;
- bundle exists and is non-empty;
- JSON, CSV, provenance, validation, PDF, and PNG outputs exist.

The accepted output set is:

- `total_uncertainty_summary.json`
- `total_uncertainty_summary.csv`
- `provenance.json`
- `validation_checks.json`
- `total_uncertainty_summary.pdf`
- `png/total_uncertainty_summary_neg.png`
- `png/total_uncertainty_summary_pos.png`
- `png/total_uncertainty_summary_percentiles.png`
- `total_uncertainty_summary_bundle.tar.gz`

Failure handling:

- If `490` rows are not present, stop.
- If any component or total quadrature entry is non-finite, stop.
- If required products are missing or empty, stop.

## Step 7 — Pull And Extract Local Bundle

If local retrieval is required, use the canonical local destination:

```text
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/hfs_systematics/total_uncertainty_z005_with_electron_lumi_2026-06-13/
```

Rule:

- pulled bundles must be extracted locally.

Acceptance checks:

- extracted local directory exists;
- expected PDF/JSON/CSV/PNG products are present after extraction.

Failure handling:

- If the tarball is present but not extracted, do not mark the retrieval complete.

## Special Traps

1. Old electron diagnostics at:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/shifted_electron_q_from_Q2Yphi/final_plots_2026-06-09/
```

are `240` rows per charge and must **not** be used for z005.

2. HFS theta comparison has JSON-encoded `bin_key`; parse and align by `(parsed.charge, parsed.bin_index)`.

3. Current HFS JES/RCES/theta are one-sided/provisional.

4. Pulled bundles must be extracted locally.

5. Do not use any pre-v6 or deprecated central path.

## Exact Commands / Workflow Snippets

These are the exact known commands or command forms verified in this accepted workflow state.

### SSH precheck

```bash
ssh -O check naf-hone.desy.de 2>&1
```

### H1OO setup when needed

```bash
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
```

### Compile when needed

```bash
make bin
```

### Electron shifted-release workflow

```bash
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/electron/_run/shifted_release_workflow/run_electron_shifted_release_v4.sh
```

### HFS shifted workflow wrapper pattern

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/hfs/<tag>/_run/shifted_release_workflow/
```

### Total summary generator

```bash
python /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/run_electron_compare_validate_total_lumi.py
```

## Final Acceptance Checklist

Mark the z005 shifted-systematics pipeline complete only if all are true:

- SSH precheck passed before remote work.
- Nominal release path matches the canonical z005 release.
- `NB_G_per_charge=245`.
- `NB_R_per_charge=490`.
- Response shape is `(491,246)` per charge.
- Data vector length is `980`.
- IBU selected `k=4`.
- IBU rows = `490`.
- Shifted sidistree counts where known: data `660`, RAPGAP `364`, DJANGOH `501`.
- Each shifted comparison has `490` finite, bin-aligned rows.
- Theta comparison is aligned by parsed `(charge, bin_index)`.
- Total summary has `490` rows.
- All components are finite.
- Total quadrature is finite.
- Canonical remote total-summary directory exists with the expected products.
- Any pulled bundle was extracted locally.

## Next Remaining Blocker

Primary remaining systematics blocker after this accepted z005 shifted-systematics state:

- **C_rad uncertainty / multiplicity mapping**

Do not describe the systematics package as fully final for physics release until C_rad uncertainty / multiplicity mapping is re-derived on the z005/v6 chain.

# Runbook: High-Q² Charged SIDIS v4 / Native-Fine Unfolding and Diagnostic Products

**Status:** Iteration 1 draft
**Audience:** Future agents doing documentation-guided diagnostic/development work on the high-Q² charged SIDIS v4/native-fine unfolding chain.
**Scope:** This runbook documents the durable command-level workflow for the **v4/native-fine diagnostic and systematics machinery**. It does **not** supersede the current central-production authority in `CONTEXT.md`, `sidis_highQ2/context-handoff.md`, or `docs/RUN_STATUS.md`.

---

## 1. Purpose and Scope

Use this runbook when the task is to generate, merge, unfold, cross-check, diagnose, or package the **v4/native-fine** high-Q² charged SIDIS migration products and their related systematic/diagnostic outputs.

This runbook covers:

1. Deploying or verifying the script set under a chosen `scripts/unfolding/tunfold_official_v4*_percharge/` variant directory.
2. Building per-file migration matrices from **v6 sidistrees**.
3. Building the required **native fine-reco** data vector.
4. Merging response + data vector into the merged ROOT input.
5. Running migration QA plus rank/support diagnostics.
6. Running TUnfold nominal probes, tau scans, summaries, and closure tests.
7. Running IBU and BBB cross-checks with `crosscheck_unfold_methods_v4.py`.
8. Building model uncertainty from cross-generator closure where available.
9. Running shifted-electron and scalar-q diagnostic systematics workflows.
10. Packaging plots and retrieval-ready diagnostic/development artifacts.

This runbook does **not** authorize:

- treating v4/native-fine products as the current central release by default;
- making final cross-section or physics claims from diagnostic products;
- overwriting current central v6 authority files;
- mixing shifted/systematic outputs into `output_v6/central/`.

---

## 2. Read First and Runtime Authority

Before using this runbook, read these files in this order:

1. `H1/CONTEXT.md`
2. `H1/sidis_highQ2/context-handoff.md`
3. `H1/sidis_highQ2/docs/RUN_STATUS.md`
4. `H1/sidis_highQ2/docs/SCRIPT_ROADMAP.md`
5. `H1/sidis_highQ2/docs/ANALYSIS_WORKFLOW_ROADMAP.md`
6. `H1/sidis_highQ2/docs/migration_matrix_manifest_2026-06-10/README.md`

Why this matters:

- `CONTEXT.md`, `context-handoff.md`, and `RUN_STATUS.md` are the authority for **what is current now**.
- This runbook is intentionally limited to the **durable workflow structure** for the v4/native-fine development chain.
- If a path, source, or central-policy statement here conflicts with a newer authority file, the authority file wins.

Reference only, do not restate in full here:

- SSH ControlMaster safety rules come from `CONTEXT.md`.
- Login-node vs Condor policy comes from `CONTEXT.md`.
- The restricted-filesystem `find` prohibition comes from `CONTEXT.md`.

---

## 3. Non-Negotiable Conventions

### 3.1 Native fine-reco data vector is required

The v4 workflow requires a **native fine-reco** data vector on the reco axis of the chosen variant.

Default rule:

- **Native fine-reco data vector is mandatory.**
- **Legacy coarse-to-fine uniform spread is forbidden by default.**

This is explicitly enforced in the v4 merge scripts. For example, variant merge scripts contain messages such as:

- `Default policy forbids silent uniform-spread ...`
- `expected native fine reco ...`

If the available data JSON is only a coarse-parent vector and the script would need `--allow-coarse-spread` or similar fallback logic, stop and rebuild the native fine-reco vector instead.

### 3.2 Encoding-B response layout is canonical

Merged v4 response ROOT files use **Encoding B** with an axis-explicit convention:

- **X axis** = reco fine bins plus the reco-overflow/miss index.
- **Y axis** = gen coarse bins plus the gen-overflow/fake index.
- **body** = `X in [physical reco fine bins]` and `Y in [physical gen coarse bins]`.
- **fake slot** = `Y = gen-overflow/fake index`, with `X` still on physical reco fine bins.
- **miss slot** = `X = reco-overflow/miss index`, with `Y` still on physical gen coarse bins.
- **corner** = `X = reco-overflow/miss index` and `Y = gen-overflow/fake index`; treat as structurally unused unless a script explicitly documents otherwise.

Do **not** describe Encoding B with ambiguous row/column shorthand. Always describe it by explicit X and Y indices.

Orientation convention in metadata is the inverted/fine-reco layout:

- `X = reco`
- `Y = gen`
- `kHistMapOutputVert`

Future agents must preserve this wording in docs, QA, and handoffs.

### 3.3 Shifted/systematic outputs must live outside `central`

Nominal v4 development products may live under variant directories inside:

- `output_v6/central/<variant>/...`

But `output_v6/central/<variant>` is a **filesystem location only**. It does **not** by itself confer physics-authority, release, nominal, or central-status approval.

Shifted/systematic products must **not** be written into `output_v6/central`. Use:

- `output_v6/systematics/shifted_electron/...`
- `output_v6/systematics/shifted_electron_q_from_Q2Yphi/...`
- other `output_v6/systematics/<source>/...` roots as applicable.

### 3.4 BBB / IBU / TUnfold caveat

The v4 cross-check products are diagnostic unless full validation passes.

State the caveat explicitly:

- **diagnostic only**;
- often **raw / no-bgsub / no-C_rad / not Born-level**;
- not automatically suitable for final data release or physics claims.

When documenting merged inputs or method-comparison outputs, require the metadata booleans:

- `bg_subtracted`
- `C_rad_applied`
- `Born_level`
- `diagnostic_only`

Historical naming trap:

- histogram names such as `h_data_bgsub_<charge>` are **historical names only**;
- they do **not** prove background subtraction was applied;
- if metadata says `bg_subtracted=false`, document the input as raw/no-bgsub even if the histogram name still contains `bgsub`.

Do **not** claim there is a common fake-subtracted reco vector across methods unless a method explicitly derives and documents one. TUnfold, BBB, and IBU may consume the same merged ROOT while handling fake/miss content differently.

### 3.5 Smoke first, then batch

Before any batch launch:

- smoke-test **one data file** for the data-vector path,
- smoke-test **one MC file** for the migration path.

If either smoke fails, do not launch Condor fan-out.

### 3.6 Condor policy

- One-file, under-5-minute checks may run interactively on the login node.
- Multi-file processing, sweeps, closures, scans, or production-style fan-out must go through **Condor**.

---

## 4. Directory and Path Resolution

Use placeholders first, then resolve them from current authority docs.

### 4.1 Core placeholders

```text
WORK=<NAF high-Q² SIDIS workspace root>
CENTRAL_ROOT=$WORK/output_v6/central
SYSTEMATICS_ROOT=$WORK/output_v6/systematics
CONDOR_ROOT=$WORK/condor_v6/central
SCRIPT_VARIANT_DIR=$WORK/scripts/unfolding/<variant>
VARIANT_OUT=$CENTRAL_ROOT/<variant>
```

As of the cited manifest and current handoff, `WORK` is typically:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2
```

But do **not** hardcode that as eternal truth. Reconfirm from current docs.

### 4.2 Variant naming pattern

A variant usually has matching script and output roots:

```text
scripts/unfolding/<variant>/
output_v6/central/<variant>/
```

Examples recorded in the manifest:

- `tunfold_official_v4_percharge`  — deprecated baseline reference
- `tunfold_official_v4_z2tmd_2xpt_percharge`
- `tunfold_official_v4_z2tmd_4x_percharge`
- `tunfold_official_v4_z2tmd_lowpt_2xpt_percharge`
- `tunfold_official_v4_zlt0p7_4x_percharge`
- `tunfold_official_v4_z2tmd_lowpt_2xpt_eta1p6_radlen5_nhit5_percharge`
- `tunfold_official_v4_s3merged_z2tmd_lowpt_2xpt_percharge`  — partial in the manifest
- `tunfold_official_v4_z2tmd_lowpt_2xpt_eta1p6_radlen5_nhit5_S3xmerged_z0p05_percharge`  — partial in the manifest

### 4.3 Resolve current completeness from the manifest

Before choosing a variant, inspect:

- `docs/migration_matrix_manifest_2026-06-10/README.md`

Use it to confirm:

- whether the variant is marked `complete`, `partial`, or `deprecated`;
- whether the merged ROOT exists;
- whether the data vector status is `NATIVE_FINE_RECO`;
- the inferred `NB_R`, `NB_G`, z support, pT split, and S3 x-merge status.

If a variant is marked `partial` and lacks either the merged ROOT or native-fine vector, expect to complete the missing stages rather than assuming the chain is ready.

---

## 5. End-to-End Pipeline

The sequence below is the canonical order.

---

## 6. Step 0 — Choose the Variant and Verify the Script Surface

### Goal

Pick one `tunfold_official_v4*_percharge` variant and confirm the required scripts exist before running anything.

### Required files in the variant script directory

At minimum, confirm these exist in `SCRIPT_VARIANT_DIR` or, for the shared helper case, in the baseline v4 directory when the variant reuses them:

- `build_migration_th2_per_charge_v4.py`
- `build_data_vector_v4.py`
- `merge_migration_per_charge_v4.py`
- `unfold_tunfold_single_tau_v4.C`
- `wrap_tau_scan_v4.sh`
- `submit_tau_scan_v4.condor`
- `summarize_tau_scan_v4.py`
- closure helpers if present in that variant
- if using baseline systematic helpers:
  - `crosscheck_unfold_methods_v4.py`
  - `build_shifted_response_data_vector_v4.py`
  - `shifted_electron_kinematics_v4.py`
  - `finalize_scalar_q_systematics_v4.py`

### Validation gate

Do not proceed until you can answer all of these:

1. Which variant is being used?
2. Is it complete or partial in the manifest?
3. What are `NB_R` and `NB_G` for this variant?
4. Does it require native fine-reco input, and is that status recorded as such?
5. Is S3 x-merged or not?

### Common failure modes

- Using the deprecated `tunfold_official_v4_percharge` when a more specific variant is intended.
- Mixing helper scripts from one variant with binning assumptions from another.
- Assuming baseline dimensions `960 x 240` apply to all variants.

---

## 7. Step 1 — Smoke-Test One MC File and One Data File

### Goal

Catch schema, branch, matching, and binning failures before Condor fan-out.

### MC smoke target

Use one v6 MC sidistree from a supported generator, usually:

- `rapgap31` nominal-like path, and optionally
- `djangoh14` for cross-generator confirmation.

### Data smoke target

Use one v6 data sidistree from `output_v6/central/sidistree/data/<period>/...`.

### What to smoke

1. `build_migration_th2_per_charge_v4.py` on one MC file.
2. `build_data_vector_v4.py` on one data file, optionally with `--max-files 1` or equivalent smoke cap.
3. `merge_migration_per_charge_v4.py` on the smoke outputs if practical.

### Validation gate

Proceed to batch only if:

- migration builder writes the expected per-file ROOT without dimension mismatch;
- data vector writes JSON with native-fine metadata;
- no branch-missing or histogram-name mismatch appears;
- the smoke output dimensions match the variant manifest.

### Common failure modes

- zsh glob expansion issues when passing unquoted globs;
- wrong variant binning module imported;
- builder assumes branches absent from the current sidistree schema;
- output path accidentally points to an older variant directory.

---

## 8. Step 2 — Build Per-File Migration Matrices from v6 Sidistrees

### Goal

Generate per-file per-charge migration ROOTs from v6 MC sidistrees.

### Script

```text
build_migration_th2_per_charge_v4.py
```

### Input class

- v6 MC sidistrees from `output_v6/central/sidistree/mc/...`
- nominally separate streams for `rapgap31` and `djangoh14`

### Command template

Use the variant-local wrapper or direct script invocation as documented in that directory. The durable template is:

```bash
python3 "$SCRIPT_VARIANT_DIR/build_migration_th2_per_charge_v4.py" \
  --input-root-files "<mc_sidistree_or_glob>" \
  --out-root "<per_file_output.root>" \
  [variant-specific flags]
```

For multi-file production, use the variant’s Condor wrappers instead of hand-written loops.

### Expected outputs

Per-file ROOTs under something like:

```text
$VARIANT_OUT/per_file_migration/<mc_tag>/...
```

or the variant’s documented equivalent.

### Validation gate

Check for:

- both charges present;
- expected TH2 dimensions `(NB_R+1, NB_G+1)`;
- nominal / rapgap / djangoh histogram naming convention as applicable;
- metadata or printed diagnostics consistent with the v4 fiducial contract;
- no obvious NaN/Inf or empty-body pathology.

### Failure modes

- **ACLiC race** if many ROOT macro jobs compile simultaneously and collide on shared artifacts; stagger or precompile as needed rather than launching identical compile races.
- **Condor macro override** where wrapper-provided arguments silently override your intended input/output paths; inspect the wrapper and submit file pair together.
- wrong MC path family or stale non-v6 inputs.

---

## 9. Step 3 — Build the Native Fine-Reco Data Vector

### Goal

Construct the reco-space data vector **directly on the fine reco axis** for the chosen variant.

### Script

```text
build_data_vector_v4.py
```

### Why this step is special

The v4 chain no longer accepts the old workaround of taking a coarse-parent data vector and uniformly spreading it into fine reco children by default.

This runbook’s policy is therefore:

- build native fine reco;
- do not rely on coarse-parent rebinners except for explicit audit archaeology.

### Command template

```bash
python3 "$SCRIPT_VARIANT_DIR/build_data_vector_v4.py" \
  --data-root-files "<data_sidistree_glob_or_paths>" \
  --out-json "$VARIANT_OUT/data_vector_native_fine.json" \
  --label "<variant_label>" \
  [--max-files N for smoke only]
```

### Expected output

A JSON whose metadata should indicate native-fine reco semantics, e.g.

- schema or warning text making clear it is raw/no-bg-subtraction if applicable;
- `NB_PER_CHARGE` matching variant reco bins;
- charge-major layout;
- native-fine level / rebin strategy metadata;
- explicit booleans `bg_subtracted`, `C_rad_applied`, `Born_level`, and `diagnostic_only`.

### Validation gate

The data-vector JSON must show all of the following before merge:

- total length = `2 * NB_R` for the variant;
- charge-major layout;
- `meta_data_vector_level = NATIVE_FINE_RECO` or equivalent schema-level declaration;
- no fallback text indicating uniform spread was used.

### Failure modes

- **native-vs-coarse confusion:** output length equals `2 * NB_G` instead of `2 * NB_R`.
- using an older JSON from a previous variant because filenames look similar.
- forgetting that many current vectors are intentionally **raw/no-bgsub** stopgaps.

---

## 10. Step 4 — Merge Response and Data Vector into a Single ROOT

### Goal

Produce the merged TUnfold-ready ROOT that contains the response, priors, bin maps, and fine-reco data vector.

### Script

```text
merge_migration_per_charge_v4.py
```

### Command template

```bash
python3 "$SCRIPT_VARIANT_DIR/merge_migration_per_charge_v4.py" \
  --in-files "<per_file_migration_glob>" \
  --data-json "$VARIANT_OUT/data_vector_native_fine.json" \
  --out-root "$VARIANT_OUT/migration_inputs_<variant>.root" \
  [--out-binmap-json "..."]
```

Use the actual CLI names from the variant’s script help; keep this template conceptual if option names differ slightly by fork.

### Expected contents

The merged ROOT should contain, per charge and tag:

- `h_R_<charge>_<tag>`
- `h_R_<charge>_<tag>_w2`
- `h_prior_gen_<charge>_<tag>`
- `h_data_bgsub_<charge>` and `_w2` histograms
- `h_binmap_gen_to_reco`
- metadata keys describing orientation, fiducial contract, data-vector policy, and the booleans `bg_subtracted`, `C_rad_applied`, `Born_level`, `diagnostic_only`

Interpret `h_data_bgsub_<charge>` carefully: the name is historical. If metadata reports `bg_subtracted=false`, treat it as the raw reco-space data vector and describe it that way.

### Validation gate

Confirm all of these:

1. Histogram dimensions match the chosen variant.
2. `h_data_bgsub_<charge>` length is `NB_R + 1` including overflow slot.
3. Metadata explicitly says native-fine reco.
4. Metadata orientation says reco on X, gen on Y, Encoding B.
5. No fallback warning indicates silent coarse spreading.
6. Metadata booleans `bg_subtracted`, `C_rad_applied`, `Born_level`, and `diagnostic_only` are present.
7. The merged response passes the Encoding-B marginal gates:
   - per-gen marginal `body + miss` agrees with the prior within the documented numerical tolerance;
   - per-reco marginal `body + fake` agrees with the reco-MC marginal within the documented numerical tolerance;
   - acceptance derived from the response stays in `[0,1]` for all physical gen bins.

### Failure modes

- Merge script rejects the JSON because it sees `2*NB_G` instead of `2*NB_R`.
- Mixed variant inputs produce dimension mismatch.
- A stale merged ROOT from another run is mistaken for the new output.

---

## 11. Step 5 — Run Migration QA and Rank/Support Diagnostics

### Goal

Establish that the merged response is structurally usable before unfolding.

### Typical products

Depending on the variant, QA outputs usually include:

- migration matrix body plots;
- fake-vs-reco plots;
- miss-vs-gen plots;
- purity and stability plots;
- acceptance vs gen plots;
- summary CSV/JSON/PDF manifests.

Manifested examples exist under plot directories such as:

```text
plots/unfolding/<variant>/...
```

### What to inspect

1. **Support:** bins outside measurement support should be explained by the variant’s z/pT design, not by accidental cut mismatches.
2. **Purity/stability:** look for dead regions, especially in edge pT or high-z cells.
3. **Encoding-B fakes and misses:** inspect them with explicit axis language:
   - fake content lives at `Y = gen-overflow/fake index` with physical reco `X` bins;
   - miss content lives at `X = reco-overflow/miss index` with physical gen `Y` bins;
   - the corner bin is not a substitute for either and should remain structurally controlled.
4. **Marginals:** verify
   - per-gen marginal `body + miss` versus the prior,
   - per-reco marginal `body + fake` versus the reco-MC marginal.
5. **Acceptance bounds:** derived acceptance must stay in `[0,1]` for every physical gen bin.
6. **Dimensions/integrity:** check expected dimensions, finite entries only, and no unexpected negative bin contents in response ingredients.
7. **Effective rank / singular structure:** use the TUnfold single-tau diagnostics to inspect effective rank and eigen behavior.

### Validation gate

Do not proceed to method comparison or systematic deltas until:

- response histograms are non-empty in the intended support;
- no charge is catastrophically sparse due to a path or selection bug;
- per-gen marginal `body + miss` matches the prior within tolerance;
- per-reco marginal `body + fake` matches the reco-MC marginal within tolerance;
- acceptance is bounded in `[0,1]` for all physical gen bins;
- dimensions are exactly as expected for the variant;
- no NaN/Inf entries exist;
- no unexpected negative entries remain in response ingredients or merged histograms;
- rank/support diagnostics do not show an obviously unusable matrix.

### Failure modes

- support mismatch caused by variant z support (`zlt0p7`, `z2tmd`, `z0p05`) not matching what the downstream script expects;
- path mix-up between S3-merged and non-merged variants;
- interpreting current-hemisphere cut sign incorrectly.

---

## 12. Step 6 — Run TUnfold Nominal Probes, Tau Scans, Summaries, and Closure

### Goal

Generate TUnfold diagnostics on the merged v4 response.

### Core macro

```text
unfold_tunfold_single_tau_v4.C
```

### Common wrappers

- `wrap_single_tau_v4.sh`
- `wrap_tau_scan_v4.sh`
- `submit_tau_scan_v4.condor`
- `summarize_tau_scan_v4.py`
- variant-specific closure wrappers and summary scripts if present

### Single-tau probe command template

```bash
root -l -q -b "unfold_tunfold_single_tau_v4.C+(\"$IN_ROOT\",\"$CHARGE\",$TAU,\"$OUT_ROOT\",\"$OUT_JSON\",$WRITE_COV,\"$MODE\")"
```

Use the wrapper if the variant provides one.

### Typical modes

- `data`
- `dataDJ`
- closure-like modes such as `selfRG`, `selfDJ`, `crossDJ`, `crossRG` where supported by the wrapper/macro pair

### Expected outputs

Usually under:

```text
$VARIANT_OUT/tau_scan/
$VARIANT_OUT/closure/
```

with products like:

- `unfolded_<charge>_tau<...>.root`
- `unfolded_<charge>_tau<...>.json`
- `unfolded_<charge>_tau<...>.pdf`
- closure summary JSON/PDF outputs

### Validation gate

Require all of the following before treating a tau choice as usable:

- L-curve or equivalent tau summary exists;
- effective-rank/eigen diagnostics exist;
- refold/closure behavior is inspected, not inferred from integrated totals alone;
- problematic bins are documented rather than silently ignored.

Strong closure rule:

- deterministic or noiseless `R · truth` forward projection is only an arithmetic smoke test;
- it is **not** a physics closure test and must not be used to justify model or closure uncertainties;
- closure/model-uncertainty studies must use Poisson-fluctuated reco pseudo-experiments, or an explicitly documented equivalent treatment with the same statistical content.

### Failure modes

- **closure/tau pathology:** good integrated totals but bad per-bin closure, pathological rho/correlation, or rank collapse.
- running only one tau and calling it “selected” with no scan context.
- forgetting that integrated totals are not sufficient evidence of a healthy unfolding.

---

## 13. Step 7 — Run IBU and BBB Cross-Checks

### Goal

Compare TUnfold, IBU, and BBB on the **same merged response inputs**, while documenting any method-specific handling of fake/miss content explicitly.

### Script

```text
crosscheck_unfold_methods_v4.py
```

### Important caveat from the script itself

The script labels its outputs as:

- **DIAGNOSTIC ONLY**
- **raw/no-bgsub**
- **no C_rad**
- **not Born-level**

Preserve that caveat in any downstream documentation.

### Command template

```bash
python3 "$WORK/scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py" \
  --response-root "$VARIANT_OUT/migration_inputs_<variant>.root" \
  --tunfold-json "<single_tau_json>" \
  --charge pos \
  --response-tag nominal \
  --variant-name "<variant>" \
  --out-json "$VARIANT_OUT/crosscheck_methods_<charge>.json" \
  --out-csv  "$VARIANT_OUT/crosscheck_methods_<charge>.csv" \
  --out-pdf  "$VARIANT_OUT/crosscheck_methods_<charge>.pdf"
```

Repeat for `neg` and, if desired, for alternate response tags.

### Output schema expectations

The script writes:

- JSON report with schema like `sidis_highq2_unfold_crosscheck_v2`
- per-bin CSV
- multi-page PDF overlay

The outputs should contain:

- TUnfold unfolded vector;
- BBB unfolded vector;
- IBU iterations and selected iteration;
- ratio panels `IBU/TUnfold` and `BBB/TUnfold`;
- caveat text in outputs.

### Method semantics that must be documented

For each comparison product, document explicitly:

- how **fake** content is handled;
- how **miss** content is handled;
- which **prior** is used;
- for IBU, which **iteration grid** was scanned and which iteration was selected;
- for BBB, which acceptance definition is used and whether any epsilon floor or veto is applied;
- for any method using epsilon protection, the exact **epsilon handling** rule.

Minimum semantics to preserve in docs:

- **BBB** is a bin-by-bin diagnostic correction, not a migration-resolving unfold; it uses an acceptance/efficiency-style correction derived from MC and is sensitive to prior/model mismatch.
- **IBU** is iterative Bayesian unfolding whose result depends on the starting prior, the response, and the chosen iteration count.
- **TUnfold** uses the full matrix with regularization and explicit treatment of the Encoding-B fake/miss structure.

BBB and IBU remain **diagnostic only** in this runbook unless a separate validation note explicitly promotes them for a defined use.

### Validation gate

Use these products to answer:

1. Are BBB and IBU broadly consistent with TUnfold in supported bins?
2. Are disagreements concentrated in known low-support or high-regularization-stress regions?
3. Is the selected IBU iteration documented and reproducible?
4. Is fake/miss/prior handling documented clearly enough that another agent could reproduce the same comparison?

### Failure modes

- treating BBB or IBU as a promoted central result without validation sign-off;
- comparing results from different merged roots or different data vectors;
- leaving fake/miss/prior/iteration semantics implicit;
- forgetting that some variants have dynamic `NB_R/NB_G`, not baseline `960/240`.

---

## 14. Step 8 — Build Model Uncertainty from Cross-Generator Closure

### Goal

Estimate model dependence where the variant has both RAPGAP and DJANGOH response paths.

### Inputs

Use one or both of:

- cross-generator closure summaries from the TUnfold tau/closure chain;
- `crosscheck_unfold_methods_v4.py` products run on alternate response tags or generator choices.

### Durable rule

Model uncertainty is only meaningful if the comparison uses:

- the same publication binning / variant definition,
- the same data-vector semantics,
- clearly identified response generator choices.

Prior-mismatch gate:

- record the **prior generator**;
- record the **response generator**;
- record the **pseudo-truth generator** for closure or pseudo-data tests;
- do not mix prior, response, and truth generators unintentionally.

If any of those three differ, document the mismatch as intentional and explain what uncertainty it is meant to probe.

### Expected outputs

Examples include:

- model systematic JSON summaries;
- closure comparison PDFs;
- ratio tables used later by release-style diagnostics.

### Validation gate

Document all of the following:

- which generator is nominal;
- which generator defines the alternate model response;
- which generator defines the prior;
- which generator defines the pseudo truth in any closure test;
- whether the estimate comes from closure, direct response swap, or both;
- whether the result is still diagnostic-only.

### Failure modes

- mixing “cross-generator closure” with “method difference” as if they were the same source;
- computing a model envelope from different tau choices or different bin supports.

---

## 15. Step 9 — Run Shifted-Electron and Scalar-q Systematics

### Goal

Produce reco-side shifted response/data-vector products and diagnostic scalar-q summaries without contaminating the central root.

### 15.1 Shifted electron vector/response builder

#### Scripts

- `shifted_electron_kinematics_v4.py`
- `build_shifted_response_data_vector_v4.py`

#### Purpose

These scripts rebuild **only the reco side** under scattered-electron perturbations using existing v6 sidistrees.

Implemented shift families documented in the builder include examples like:

- `elecE_p1pct`
- `elecE_m1pct`
- `elecTheta_p1mrad`
- `elecTheta_m1mrad`

#### Output-root policy

The documented default locations are:

```text
$WORK/output_v6/systematics/shifted_electron/
$WORK/output_v6/systematics/shifted_electron_q_from_Q2Yphi/
```

Do **not** stage new shifted outputs under `output_v6/central`.

#### Command template

```bash
python3 "$WORK/scripts/unfolding/tunfold_official_v4_percharge/build_shifted_response_data_vector_v4.py" \
  --mode both \
  --input-root-files "<sidistree_glob>" \
  --shift elecE_p1pct \
  --out-json "$SYSTEMATICS_ROOT/shifted_electron/data_vector_shifted_elecE_p1pct.json" \
  --out-root "$SYSTEMATICS_ROOT/shifted_electron/mig_shifted_elecE_p1pct.root"
```

Use wrapper/Condor submit files when running many files.

#### Validation gate

Confirm:

- shifted data vector still has native fine-reco semantics;
- shifted migration keeps Encoding B structure;
- metadata marks `bg_subtracted=false` if that is still true;
- outputs are under `output_v6/systematics/...`;
- the zero-shift configuration reproduces the baseline within the documented tolerance before interpreting any nonzero shift.

### 15.2 Scalar-q diagnostic finalizer

#### Script

```text
finalize_scalar_q_systematics_v4.py
```

#### Purpose

This script finalizes scalar-q (`PHOTON_MODE = q_from_Q2Yphi`) diagnostic outputs by:

- locating scalar-q shifted data vectors and per-period migration roots;
- summing per-period response arrays in memory;
- producing migration-QA plots;
- computing BBB and IBU central values;
- comparing scalar-q results to the baseline vector-mode chain where available;
- writing `SUMMARY.md`, `manifest.json`, and compact CSV/JSON/PDF products.

#### Important caveat

The script labels outputs as:

- diagnostic only,
- raw / no-bgsub,
- no C_rad,
- plus-only shifts unless minus shifts were also produced.

#### Command template

```bash
python3 "$WORK/scripts/unfolding/tunfold_official_v4_percharge/finalize_scalar_q_systematics_v4.py" \
  --work "$WORK" \
  --scalar-root "$SYSTEMATICS_ROOT/shifted_electron_q_from_Q2Yphi" \
  --baseline-root "$CENTRAL_ROOT/tunfold_official_v4_percharge" \
  --v4-script-dir "$WORK/scripts/unfolding/tunfold_official_v4_percharge" \
  --out-dir "$SYSTEMATICS_ROOT/shifted_electron_q_from_Q2Yphi/final_plots_<date>"
```

#### Validation gate

Check that:

- baseline and scalar roots are not swapped;
- vector-mode baseline comparison files exist if scalar-vs-vector deltas are expected;
- missing-minus-shift status is carried into the summary instead of being hidden;
- a zero-shift scalar-q reproduction gate is passed before any shift-derived diagnostic is interpreted.

Blocking caveat:

- scalar-q outputs are **diagnostic only** in this runbook;
- they are **blocked for nominal systematic use** unless a later physics note explicitly promotes the scalar-q path.

### 15.3 Failure modes

- writing shifted outputs into `output_v6/central` by habit;
- using scalar-q as if it were the nominal electron systematic path;
- comparing scalar and vector chains that were built from mismatched baselines.

---

## 16. Step 10 — Package Plots and Retrieval-Ready Artifacts

### Goal

Collect the diagnostic outputs into stable, retrieval-ready bundles for later review.

### Typical artifact families

- migration QA PDFs and PNGs;
- tau scan summaries;
- closure summaries;
- TUnfold single-tau PDFs;
- method-comparison PDFs/CSVs/JSONs;
- scalar-q or shifted-electron manifests and summaries;
- comparison plots intended for diagnostic/development review.

### Durable packaging rule

Each packaging directory should contain enough metadata to answer:

- which variant;
- which charge(s);
- which input merged ROOT;
- which tau or iteration choice;
- whether the product is nominal, shifted-electron, or scalar-q;
- the caveat state: raw / no-bgsub / no-C_rad / not Born-level.

### Validation gate

Before declaring packaging complete, verify:

- every PDF referenced in a manifest actually exists;
- manifest paths point to the correct variant;
- no stale absolute path from another variant was copied into the bundle.

---

## 17. Variant Workflows

All supported variants follow the same pattern:

1. build migration;
2. build native fine-reco data vector;
3. merge;
4. QA;
5. TUnfold nominal/tau/closure;
6. cross-checks;
7. systematics/finalization as needed.

What changes by variant is the binning and support, not the workflow logic.

### 17.1 `zlt0p7`

Example:

- `tunfold_official_v4_zlt0p7_4x_percharge`

Meaning:

- reduced z support (`z < 0.7`) relative to the full-z baseline family.

Workflow note:

- confirm that all downstream summaries and comparisons use the same reduced z support.
- do not compare directly to full-z outputs without an explicit common-support reduction.

### 17.2 `z2tmd`

Examples:

- `tunfold_official_v4_z2tmd_2xpt_percharge`
- `tunfold_official_v4_z2tmd_4x_percharge`

Meaning:

- TMD-style z support with reco refinement variants.

Workflow note:

- `NB_R` changes with reco refinement; derive dimensions from the merged ROOT or manifest, not memory.

### 17.3 `lowpt`

Examples:

- `tunfold_official_v4_z2tmd_lowpt_2xpt_percharge`
- `tunfold_official_v4_z2tmd_lowpt_2xpt_eta1p6_radlen5_nhit5_percharge`

Meaning:

- low-pT gen-axis extension / diagnostic emphasis.

Workflow note:

- low-pT support tends to increase sensitivity to sparsity, fake/miss structure, and closure tails.
- scrutinize support and pathology diagnostics more aggressively here.

### 17.4 `S3-merged`

Examples in the manifest:

- `tunfold_official_v4_s3merged_z2tmd_lowpt_2xpt_percharge`
- `tunfold_official_v4_z2tmd_lowpt_2xpt_eta1p6_radlen5_nhit5_S3xmerged_z0p05_percharge`

Meaning:

- the S3 x bins are merged in the variant’s gen layout.

Workflow note:

- treat S3-merged and non-merged outputs as different binning universes.
- do not re-use non-merged data vectors or summaries for S3-merged interpretation.
- the manifest currently marks these examples as partial; expect missing merge/vector stages unless newer docs say otherwise.

---

## 18. IBU / BBB Cross-Check Caveats and Output Schema

When documenting or consuming `crosscheck_unfold_methods_v4.py` outputs, preserve these points:

1. The script is a **common-input method comparison**, not a central-release builder.
2. The JSON schema is method-comparison specific, not a final measurement schema.
3. IBU regularization uncertainty in these outputs is a **diagnostic** `±1 iteration` style quantity, not a full final systematic budget.
4. BBB remains migration-blind compared with matrix unfolding, so agreement/disagreement must be interpreted in that context.
5. BBB and IBU in this runbook stay diagnostic unless a separate validation note promotes them for a specific task.
6. All outputs must retain the caveat:
   - raw / no-bgsub / no-C_rad / not Born-level,
   - unless a later validated chain explicitly removes those caveats.

Recommended doc wording for future agents:

> BBB and IBU products from the v4/native-fine chain are method cross-check diagnostics on a common merged response input, with method-specific fake/miss handling documented explicitly. They are not final physics results unless a later validation note explicitly promotes them.

---

## 19. Troubleshooting

### 19.1 Condor macro override

Symptom:

- submit file appears correct, but jobs still write to an old path or old variant directory.

Cause:

- wrapper hardcodes arguments, overriding Condor macros or environment variables.

Fix:

- inspect both the submit file and wrapper;
- verify the effective executable arguments before launch;
- prefer one source of truth for paths.

### 19.2 ACLiC race

Symptom:

- ROOT macro compile failures appear intermittently in batch, often only on the first wave.

Cause:

- many jobs compile the same `+` macro simultaneously.

Fix:

- precompile once if appropriate or stagger compilation-sensitive launches;
- avoid identical first-touch compile storms.

### 19.3 Shell quoting / zsh globs

Symptom:

- builder sees literal wildcard text or, conversely, the shell expands to the wrong file set.

Fix:

- quote globs deliberately;
- prefer wrapper patterns already used by the variant;
- never assume bash and zsh treat globs the same way in ad hoc commands.

### 19.4 Native-vs-coarse data vector mismatch

Symptom:

- merge script says it expected native fine reco, or the JSON length is `2*NB_G` instead of `2*NB_R`.

Fix:

- rebuild with `build_data_vector_v4.py` for the chosen variant;
- do not enable coarse-spread fallback except for explicit historical audit work.

### 19.5 Missing DIS denominator / missing C_rad

Symptom:

- downstream user tries to read a diagnostic unfolded product as a final multiplicity or cross section.

Fix:

- state explicitly whether the product is raw/no-bgsub/no-C_rad/not-Born;
- do not backfill final semantics from separate files unless the chain was designed for that promotion.

### 19.6 Closure or tau pathology

Symptom:

- integrated totals look acceptable, but per-bin closure, rho, or eigen diagnostics look bad.

Fix:

- trust the per-bin/linear-algebra diagnostics over the integrated total;
- revisit tau choice, bin support, or variant suitability;
- document the pathology instead of smoothing it over.

---

## 20. Handoff Checklist for Future Agents

Before ending a v4/native-fine session, record all of the following in the handoff or run note:

1. **Variant used**:
   - exact variant directory name.
2. **Input lineage**:
   - which v6 sidistree surface was used;
   - which MC generators were used.
3. **Data-vector status**:
   - native fine-reco confirmed yes/no;
   - raw/no-bgsub yes/no.
4. **Merged ROOT**:
   - full path;
   - `NB_R`, `NB_G`;
   - Encoding-B orientation confirmed yes/no;
   - marginal gates passed yes/no (`body+miss` vs prior, `body+fake` vs reco marginal, acceptance in `[0,1]`).
5. **QA status**:
   - migration QA complete yes/no;
   - major support/rank findings.
6. **TUnfold status**:
   - single-tau probe done yes/no;
   - tau scan done yes/no;
   - closure done yes/no;
   - selected tau if any.
7. **IBU/BBB cross-check status**:
   - output JSON/CSV/PDF paths;
   - caveat preserved yes/no.
8. **Model uncertainty status**:
   - cross-generator comparison available yes/no;
   - prior generator;
   - response generator;
   - pseudo-truth generator;
   - source path.
9. **Systematics status**:
   - shifted-electron vector/response built yes/no;
   - shifted-electron zero-shift reproduction passed yes/no;
   - scalar-q finalization built yes/no;
   - scalar-q zero-shift reproduction passed yes/no;
   - outputs confirmed under `output_v6/systematics/...` yes/no.
10. **Packaging**:
    - final plot/manifest directory paths.
11. **Outstanding blockers**:
    - missing minus shifts,
    - partial S3-merged chain,
    - closure pathology,
    - missing native vector,
    - missing model comparison,
    - anything else blocking promotion beyond diagnostic status.

Recommended final sentence in the handoff:

> These products remain diagnostic/development outputs unless and until a newer run-status or release note explicitly promotes them.

---

## 21. Minimal Execution Summary

For quick orientation, the durable order is:

1. choose variant;
2. verify scripts and manifest completeness;
3. smoke one MC + one data file;
4. build per-file migration ROOTs from v6 sidistrees;
5. build native fine-reco data vector;
6. merge into `migration_inputs_<variant>.root`;
7. run migration QA and rank/support diagnostics;
8. run TUnfold single-tau, tau scans, summaries, and closure;
9. run `crosscheck_unfold_methods_v4.py` for BBB/IBU/TUnfold overlays;
10. derive model uncertainty from cross-generator closure where available;
11. run shifted-electron and scalar-q diagnostic systematics under `output_v6/systematics/...`;
12. package plots and write a handoff with caveats.

That is the canonical v4/native-fine workflow.

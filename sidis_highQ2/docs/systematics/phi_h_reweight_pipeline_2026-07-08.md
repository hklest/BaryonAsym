# phi_h Reweighting Side-Study Runbook — High-Q2 Charged SIDIS

**Created:** 2026-07-08  
**Scope:** high-Q2 inclusive charged-hadron SIDIS only (`sidis_highQ2/`)  
**Status:** completed side-study with QA PASS for systematic-treatment discussion; **not final-systematics-ready** until injected-phi closure validates recovery.

## Overview

This note records the completed `phi_h` reweighting side study for the current high-Q2 z005 charged-SIDIS release. The purpose is to test whether a reconstructed-level data/MC `phi_h` shape mismatch, combined with `phi_h`-dependent acceptance and response, can bias the **phi-integrated** high-Q2 charged-SIDIS cross sections.

What this study does:

- derives reco-level `phi_h` shape weights from data/MC ratios;
- applies those weights to the DJANGOH14 response construction only;
- rebuilds the response and reruns the z005 IBU side-study chain at `k=4`;
- compares the reweighted result to the nominal central z005 release.

What this study does **not** yet establish:

- it is **not** yet approved as a final systematic source;
- it has **no** RAPGAP envelope yet;
- it does **not** supersede any central products;
- it must **not** be added to the final systematic budget before injected-`phi` closure is done.

## Read First

1. `/Users/hklest/Desktop/opencode/H1/CONTEXT.md`
2. `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/context-handoff.md`
3. `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/RUN_STATUS.md`
4. `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/systematics/z005_shifted_systematics_pipeline_2026-06-13.md`
5. This runbook

If a newer high-Q2 handoff entry disagrees with this file, follow the newer explicit statement and update this document.

## Purpose and Study Definition

Target question:

- Does a reco-level data/MC `phi_h` mismatch, together with `phi_h`-dependent acceptance/response, create a bias in the **phi-integrated** high-Q2 charged-SIDIS cross sections?

Study definition used here:

- nominal data vector retained from the current z005 release context;
- response reweighted using **DJANGOH14 MC only**;
- weights built from reco data/MC `phi_h` shape ratios;
- no central output directories overwritten;
- recorded data luminosity: `L_data = 351000.0 nb^-1`.

## Final Status Snapshot

### Final study roots

- Final side-study root: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/`
- Pass1 full root: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/`

### Final status judgment

- **QA verdict:** PASS for side-study discussion.
- **Systematics verdict:** candidate `phi`-shape systematic only.
- **Blocking requirement before final-budget use:** injected-`phi` closure proving recovery.

## Directory and Artifact Map

| Item | Path |
|---|---|
| Pass1 shard builder | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/phi_reweight_z005_20260707/phi_reweight_pass1_shard.py` |
| Pass1 weight aggregator | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/phi_reweight_z005_20260707/aggregate_phi_pass1_weights.py` |
| Pass2 response shard builder | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/phi_reweight_z005_20260707/build_phi_reweighted_response_shard_v1.py` |
| Final side-study driver / packaging script | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/scripts/final_phi_reweight_side_study.py` |
| Pass1 validation summary | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/validation/validation_summary.json` |
| Pass1 pre-Pass2 QA | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/validation/qa_summary_pass1_prepass2.json` |
| Weight definition | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/weights/phi_reweight_definition.json` |
| Merged reweighted response ROOT | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/merged/response_phi_reweighted.root` |
| Merged reweighted response NPZ | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/merged/response_phi_reweighted.npz` |
| Final validation | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/validation/final_phi_reweight_validation.json` |
| Shift table CSV | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/shift/phi_systematic_shift_table.csv` |
| Shift table JSON | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/shift/phi_systematic_shift_table.json` |
| Final manifest | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/manifest_final.json` |

## Inputs and Contracts

### Inputs

- Current z005/v6 release and current data vector were used as the nominal reference.
- The reweighted response uses **DJANGOH14 only**.
- Recorded data luminosity for the side study: `351000.0 nb^-1`.
- No central output products were overwritten.

### Response and binning contract

Use the current z005 contract throughout:

- published rows: `490`
- `245` h+ rows + `245` h- rows
- response shape: `(491,246)` per charge
- IBU side-study iteration: `k=4`

## Weighting Implementation

### Grouping

Reco data/MC `phi_h` shapes were built for `16` `charge::group` keys:

- charges: `hplus`, `hminus`
- groups:
  - `all`
  - `S0_x0`
  - `S0_x1`
  - `S1_x0`
  - `S1_x1`
  - `S2_x0`
  - `S2_x1`
  - `S3_x0`

This gives `2 x 8 = 16` keys total.

### phi_h binning and ratio construction

- `phi_h` bins: `16` over `[-pi, +pi]`
- ratio type: area-normalized shape-only data/MC ratio
- regularization floor: `alpha = 1e-3`
- clipping: `[0.25, 4.0]`
- circular smoothing kernel: `[0.25, 0.5, 0.25]`
- final renormalization: weights scaled to MC-area mean `1` per group

### Final weight range

- minimum: `0.945979`
- maximum: `1.060744`
- median: `0.999766`
- empty or sparse groups: none

### Response-leg application rule

Weights were applied per track with leg-dependent `phi_h` use:

- gen / prior / miss: use **gen `phi_h`**
- reco matched / fake: use **reco `phi_h`**

Final response shape remained:

- `[2,491,246]`

## Step-by-Step Runbook

This section documents the completed pipeline order so it can be reproduced or audited later. Do not treat this note as authorization to rerun production blindly; re-check current handoff status first.

### Pass0 — smoke and setup QA

Purpose:

- verify wrappers, imports, environment assumptions, and file-routing before broad submission.

Checklist:

- confirm study-local roots under `output_v6/studies/phi_reweight_z005_20260707/`
- confirm no central path is used as an output target
- confirm the shard wrapper really processes the intended shard list, not a single test file only
- confirm Python imports resolve, including any `binning_v3` dependency
- confirm H1 environment assumptions are valid if a wrapper sources H1 setup
- confirm placeholder ROOT outputs are not mistaken for successful payloads

Acceptance target:

- smoke proves the shard wrapper and artifact routing are correct before Pass1 broad submission.

### Pass1 — shard production of phi_h histograms / ingredients

Purpose:

- build the reco-level data and MC `phi_h` ingredients needed for the weight definition.

Primary script:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/phi_reweight_z005_20260707/phi_reweight_pass1_shard.py`

Expected outputs under Pass1 root:

- shard-level outputs
- validation summaries
- aggregated hist inputs for weights

Key requirements:

- use the current z005 release context only
- keep data and MC accounting separate until aggregation
- preserve `hplus` and `hminus` separation
- preserve the `8` group labels per charge

### Pass1 validation

Primary artifacts:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/validation/validation_summary.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/validation/qa_summary_pass1_prepass2.json`

Validate before continuing:

- all expected charge/group keys exist
- no empty/sparse groups
- `phi_h` binning is the intended `16` bins over `[-pi, +pi]`
- data and MC histograms are finite
- no accidental single-file smoke output is being reused as if it were full production

### Weight aggregation

Purpose:

- convert Pass1 reco-level data/MC `phi_h` shapes into the final weight definition.

Primary script:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/phi_reweight_z005_20260707/aggregate_phi_pass1_weights.py`

Primary output:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/full_20260708_pass1_v2/weights/phi_reweight_definition.json`

Required checks:

- shape-only area normalization applied
- `alpha = 1e-3` floor applied
- ratio clipped to `[0.25, 4.0]`
- circular smoothing `[0.25, 0.5, 0.25]` applied
- per-group renormalization restores MC-area mean `1`
- final weight range stays in the validated band `0.945979–1.060744`

### Pre-Pass2 QA

Before rebuilding the response, confirm:

- no charge/group keys are missing
- no bins are NaN or inf
- the aggregated weight file is the **full Pass1 v2** artifact, not an obsolete earlier pass
- the intended input is a shard-list TXT or equivalent full-production manifest, not an obsolete direct ROOT path assumption

### Pass2 — reweighted response shards and recovery

Purpose:

- build the reweighted DJANGOH14 response using the validated Pass1 weights.

Primary script:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/phi_reweight_z005_20260707/build_phi_reweighted_response_shard_v1.py`

Key implementation rule:

- gen/prior/miss entries use gen `phi_h`
- reco matched/fake entries use reco `phi_h`

Requirements:

- use DJANGOH14 only for the reweighted response
- keep outputs in the Pass2 study root only
- do not overwrite central response or central data-release products
- guard against zero-byte ROOT placeholders from interrupted or temporary writes

### Response merge

Purpose:

- merge Pass2 shard outputs into the final reweighted response.

Merged outputs:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/merged/response_phi_reweighted.root`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/merged/response_phi_reweighted.npz`

Required checks:

- merged response finite
- merged response nonnegative
- response shape consistent with the z005 contract
- no accidental partial merge from placeholder files

### IBU k=4 side-study

Purpose:

- run the standard side-study comparison using the reweighted response and the current nominal data vector.

Primary study-packaging script:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/scripts/final_phi_reweight_side_study.py`

Expected outputs:

- final validation JSON
- shift-table CSV/JSON
- final manifest

### Final QA

Primary artifacts:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/validation/final_phi_reweight_validation.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/shift/phi_systematic_shift_table.csv`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/shift/phi_systematic_shift_table.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/phi_reweight_z005_20260707/pass2_reweighted_response_20260708_recovery/manifest_final.json`

QA pass conditions recorded for the completed study:

- merged response finite/nonnegative
- acceptance h+ range `0.02905–0.76617`
- acceptance h- range `0.02788–0.76589`
- no acceptance above `1`
- prior zero bins: `0`
- IBU rows: `490`
- shift rows: `490`
- finite shift rows: `490`
- negative cross sections: `0`
- alignment: PASS
- central overwrite safety: PASS

## Validated Results to Cite

### Global summary

- Final QA: **PASS for side-study discussion**.
- Median absolute relative shift: `1.6917%`
- Maximum relative shift: `37.3484%` in a tiny tail bin
- Integrated shifts:
  - h+: `+0.5193%`
  - h-: `+0.5746%`
  - total: `+0.5466%`

### Interpretation guidance

Use these results as evidence that the `phi_h` mismatch can induce a nonzero phi-integrated shift at the sub-percent integrated level with larger local tails in sparse bins. For any note text or systematic-treatment discussion, cite the **shift table** and the validation JSON, not rejected plotting attempts.

## Common Failure Modes Discovered During This Study

### 1. Smoke wrapper accidentally processed one file only

Problem:

- an early wrapper path or smoke setup effectively exercised only one file, which is not representative of the full pass.

Guardrail:

- verify the wrapper consumes the intended shard list or manifest before broad submission.

### 2. `thish1.sh` / `GROUPROOT` environment failure

Problem:

- wrappers depending on H1 setup can fail through the `thish1.sh` / `GROUPROOT` environment path.

Guardrail:

- validate setup assumptions explicitly in smoke before launching a pass that depends on the environment.

### 3. Empty paste / empty stdin input

Problem:

- manual wrapper construction or job-input staging can silently receive empty pasted content.

Guardrail:

- verify materialized wrapper contents and argument files before submit.

### 4. `binning_v3` import issue

Problem:

- at least one pass encountered a Python import/path issue around `binning_v3`.

Guardrail:

- resolve imports in smoke and do not assume worker-node Python path matches the interactive node.

### 5. Obsolete Pass1 ROOT path vs shard-list TXT mismatch

Problem:

- an obsolete direct Pass1 ROOT path assumption conflicted with the actual shard-list TXT or full-production input contract.

Guardrail:

- point downstream steps to the current full Pass1 v2 artifact set, not legacy direct ROOT references.

### 6. Zero-byte ROOT placeholders / temporary write hazard

Problem:

- interrupted jobs or temporary write patterns can leave zero-byte placeholder ROOT files that look like completed outputs by filename only.

Guardrail:

- reject zero-byte files during merge/recovery and confirm outputs are real payloads before counting them as complete.

### 7. Plotter pitfalls

Problem cases discovered:

- data=MC plotting bug
- misuse of global weights in place of intended grouped weights
- invalid full-stat plot built from `1/201` data and `1/1` MC

Guardrail:

- do not use these products as evidence.

## Plotting Caveats

Do **not** cite problematic before/after binned overlay sets from earlier rejected attempts.

Current plotting guidance:

- a valid ratio plot exists at `plots/global_bin_ratio_nominal_over_phi`
- several visible data/raw/reweighted overlay attempts were QA-rejected
- do not cite those rejected overlays unless they are regenerated and QA-passed

For note or systematic discussion, prefer:

- the shift table CSV/JSON
- final validation JSON
- manifest/provenance

## Separate but Related Caveat: RC gen-level phi_h

A separate radiative-correction diagnostic issue exists:

- a prior RC gen-level `phi_h` diagnostic used `ChargedGenRC.phi_h`, not `GenPhi`

This is **not** the same as the response-reweight side study documented here. Keep the two issues separate in any future note text.

## What Is Safe To Say Now

Safe current statement:

- the completed DJANGOH14-only `phi_h` reweight side study passes QA and shows a median absolute bin shift of `1.6917%` with integrated shifts around `+0.55%`, but it remains a **candidate** `phi`-shape systematic pending injected-`phi` closure.

Unsafe current statement:

- do **not** state that this systematic is final, budget-approved, generator-enveloped, or note-ready based on plots alone.

## Exact Next Action

The next agent must do this before any analysis-note/systematics inclusion:

1. run the injected-`phi` closure study;
2. confirm recovery before promoting this to final systematic status.

If note text is drafted before that closure is complete:

- cite the shift table and explicit caveats only;
- do **not** cite invalid/rejected before-after overlay plots;
- do **not** present this as a finalized systematic budget component.

## Minimal Acceptance Checklist For Future Reuse

Before reusing this side study in any downstream document, confirm all of the following:

- doc path matches this runbook
- final study root is the recovery root dated `20260708`
- Pass1 source is `full_20260708_pass1_v2`
- weight definition is the validated JSON under `weights/`
- merged response exists as both `.root` and `.npz`
- final validation JSON reports PASS-level consistency
- shift table rows = `490`
- central overwrite safety remains PASS
- caveat about injected-`phi` closure is preserved
- caveat about DJANGOH14-only scope is preserved
- caveat about local large tail shifts is preserved

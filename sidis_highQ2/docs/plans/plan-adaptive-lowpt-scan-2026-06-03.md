# RUN_PLAN: Adaptive Low-pT_h Refinement Scan -- 2026-06-03

Status: PREPARATION ONLY. No Condor submission yet. NAF execution
delegated to `h1-script-runner` / `h1-naf-operator` in a follow-up.

## Goal

Run two side-experiment per-charge TUnfold unfoldings for the H1 high-Q^2
charged-SIDIS analysis using ADAPTIVE low-pT_h gen binnings selected by
the dry-run occupancy floor (data/adaptive_lowpt_scan_dryrun_2026-06-03):

| Variant | Occupancy floor | Trusted physics bins | Uniform gen NB_G | Reco NB_R |
|---------|-----------------|----------------------|------------------|-----------|
| 5k      | 5000 / bin      | 235                  | 714              | 1680      |
| 1k      | 1000 / bin      | 339                  | 798              | 1848      |

Both variants preserve Scheme B′ cell labels (cells 0-5 = S0_x0 .. S2_x1;
cell 6 = S3merged) and Reading-A z mask (keep z_bin 5 only in S0_x0;
drop z_bin 4 in S2_x1 and S3merged).

## Architecture choice (preserves existing pipeline)

The adaptive per-(cell, z) variable pT structure is NOT natively
expressible by the existing builder/merger/unfold-macro family, which
assumes a UNIFORM (cell- and z-independent) per-cell pT grid via the
`flat_gen(cell, zp) = cell * NB_ZPT_GEN + zp` index convention.

To stay inside the existing architecture we use a **uniform-superset +
downstream aggregation** scheme:

1. Gen `PTH_EDGES_GEN` = union of all per-panel selected pT edges for
   the threshold (17 edges for 5k, 19 edges for 1k).
2. Reco `PTH_EDGES_RECO` = the gen edges plus a midpoint split of every
   gen pT interval >= 0.5 GeV wide.
3. The 235/339 "adaptive physics bins" are reconstructed in
   post-processing by an aggregation matrix `T_ADAPTIVE` (shape
   NB_G × N_ADAPTIVE) that sums neighboring uniform fine bins into
   each selected per-panel bin.
4. Reading-A masking is encoded both as a binary `TRUSTED_MASK_LOWPT_
   ADAPTIVE` over the 714/798 uniform gen vector and (more sharply)
   inside `T_ADAPTIVE` itself (untrusted uniform bins contribute to
   no adaptive column).

This re-uses the v2 builder, the refined-v1 merge structure, and the
refined-v1 TUnfold worker macro with **only constants and the binning
import re-targeted**. No edits to canonical files; no H1OO changes.

## Files created (all under
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/`)

### Binning modules (canonical edge tables verified against dry-run JSON)

- `binning_lowpt_adaptive_5k_v1.py`  (714 gen, 1680 reco, 235 adaptive)
- `binning_lowpt_adaptive_1k_v1.py`  (798 gen, 1848 reco, 339 adaptive)

### Build (monkey-patches `build_migration_th2_per_charge_NWfix_v2`)

- `build_migration_th2_per_charge_lowpt_adaptive_5k_v1.py`
- `build_migration_th2_per_charge_lowpt_adaptive_1k_v1.py`
- `wrap_build_migration_per_charge_lowpt_adaptive_5k_v1.sh`
- `wrap_build_migration_per_charge_lowpt_adaptive_1k_v1.sh`
- `submit_build_migration_per_charge_lowpt_adaptive_5k_v1.condor`
- `submit_build_migration_per_charge_lowpt_adaptive_1k_v1.condor`

### Merge (copies of refined-v1 with retargeted binning import and
canonical-pT-parent child maps for the new fine reco axis)

- `merge_migration_per_charge_lowpt_adaptive_5k_v1.py`
- `merge_migration_per_charge_lowpt_adaptive_1k_v1.py`
- `wrap_merge_lowpt_adaptive_5k_v1.sh`
- `wrap_merge_lowpt_adaptive_1k_v1.sh`

### TUnfold worker (.C) and tau scan

- `unfold_tunfold_single_tau_lowpt_adaptive_5k_v1.C`
- `unfold_tunfold_single_tau_lowpt_adaptive_1k_v1.C`
- `wrap_single_tau_lowpt_adaptive_5k_v1.sh`
- `wrap_single_tau_lowpt_adaptive_1k_v1.sh`
- `tau_scan_lowpt_adaptive_5k_v1_args.txt`  (80-job grid)
- `tau_scan_lowpt_adaptive_1k_v1_args.txt`  (80-job grid)
- `submit_tau_scan_lowpt_adaptive_5k_v1.condor`
- `submit_tau_scan_lowpt_adaptive_1k_v1.condor`

### Summarize

- `summarize_tau_scan_lowpt_adaptive_5k_v1.py`  (NB_G=714)
- `summarize_tau_scan_lowpt_adaptive_1k_v1.py`  (NB_G=798)

## Local validation performed (no NAF/SSH)

```
python3 -m py_compile  <all 8 new .py>           -> PASS
bash -n                <all 6 new .sh>           -> PASS
python3 binning_lowpt_adaptive_5k_v1.py          -> ALL TESTS PASS
python3 binning_lowpt_adaptive_1k_v1.py          -> ALL TESTS PASS
import build_migration_th2_per_charge_lowpt_adaptive_5k_v1
   -> patched v2: NB_R=1680 NB_G=714                 -> PASS
import build_migration_th2_per_charge_lowpt_adaptive_1k_v1
   -> patched v2: NB_R=1848 NB_G=798                 -> PASS
```

The binning self-tests verify, for each variant, that the hard-coded
per-(cell, z) pT edge tables AGREE bit-for-bit with the dry-run
`per_threshold_edge_table[<threshold>]` after Reading-A filtering, that
the adaptive physics bin count matches the dry-run kept-panel sum
(235 / 339), and that `T_ADAPTIVE` is yield-conservative.

## Exact command sequence for later NAF production

Assumes the standard ControlMaster session is alive
`h1-naf-operator`. All paths are NAF absolute paths under
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`.

### Step 0 - Sync new workspace scripts to NAF

```
rsync -av \
  /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/{binning,build,merge,unfold,wrap,submit,tau_scan,summarize}_*lowpt_adaptive_{5k,1k}_v1* \
  naf-hone.desy.de:/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v3_percharge/
```

Also sync the dry-run JSON so `LOWPT_ADAPTIVE_DRYRUN_JSON` is reachable
on workers (or rely on the hard-coded tables -- the dry-run check is
optional at runtime, the module loads without it).

### Step 1 - Per-(MC, period) build (Condor, 8 jobs per variant)

```
cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v3_percharge
condor_submit submit_build_migration_per_charge_lowpt_adaptive_5k_v1.condor
condor_submit submit_build_migration_per_charge_lowpt_adaptive_1k_v1.condor
```

Outputs:
- `output/tunfold_tree_v3_percharge/per_file_migration_lowpt_adaptive_5k_v1/{rapgap31,djangoh14}/mig_lowpt_adaptive_5k_v1_<mc>_<period>.root` (8 files per variant)
- `output/tunfold_tree_v3_percharge/per_file_migration_lowpt_adaptive_1k_v1/{rapgap31,djangoh14}/mig_lowpt_adaptive_1k_v1_<mc>_<period>.root` (8 files per variant)

Each ROOT contains TH2 `h_R_pos`, `h_R_neg` with shape `(NB_R+1, NB_G+1)`
= (1681, 715) for 5k and (1849, 799) for 1k.

### Step 2 - Merge (1 job per variant)

Run locally on the login node OR submit to Condor. The merger reads ~8
small ROOTs + the canonical 480-bin bg-subtracted data JSON. Output:

```
bash wrap_merge_lowpt_adaptive_5k_v1.sh
bash wrap_merge_lowpt_adaptive_1k_v1.sh
```

Outputs:
- `output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/migration_inputs_lowpt_adaptive_5k_v1.root`
- `output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/migration_inputs_lowpt_adaptive_1k_v1.root`
- Sidecar binmap JSONs alongside.

### Step 3 - Tau scan (Condor, 80 jobs per variant)

```
condor_submit submit_tau_scan_lowpt_adaptive_5k_v1.condor
condor_submit submit_tau_scan_lowpt_adaptive_1k_v1.condor
```

Each job produces `unfolded_<chg>_tau<tag>.{root,json}` in
`output/tunfold_tree_v3_percharge/lowpt_adaptive_<variant>_v1/tau_scan_<Area|None>/`.

> **Memory warning:** The condor submit currently requests 2000 MB. The
> 1k variant unfolds a 1848×799 response with 1246 curvature triplets;
> if jobs OOM, bump `request_memory` to 4000-6000 MB and resubmit only
> the affected `(chg, tau, cons)` tuples.

> **2026-06-03 update — tau wrapper ACLiC race fix:** The first attempt
> at Step 3 (clusters 3602582 / 3602583) failed because 80 concurrent
> Condor jobs invoked `root … unfold_…_v1.C+(...)` while `cd`'d into the
> shared `scripts/unfolding/tunfold_official_v3_percharge/` directory,
> racing on ACLiC build artifacts. Fixed locally by giving each job its
> own scratch workdir
> (`${_CONDOR_SCRATCH_DIR:-${TMPDIR:-/tmp}}/aclic_lowpt_adaptive_<5k|1k>_${CLUSTER}_${PROCESS}_$$`),
> copying the macro there, and invoking ROOT from scratch with the same
> absolute IN/OUT/JSON arguments. Workdir is cleaned via `trap … EXIT`.
> A `WRAP_DRYRUN=1` env var skips the ROOT call for local validation.
> Also bumped `submit_tau_scan_lowpt_adaptive_5k_v1.condor`
> `request_memory` from 2000 → 6000 MB (observed up to 4.6 GB on the
> previous attempt). The 1k submit was already at 5000 MB and is
> unchanged. Both submits already had `notification = Never`. Canonical
> (`wrap_single_tau.sh`, `submit_tau_scan.condor`) and refined-v1
> wrappers/submits were intentionally left untouched. Sync + resubmit
> are a separate task (see "Resync after ACLiC fix" below).

### Resync after ACLiC fix (2026-06-03)

After the local edits above, before the next `condor_submit`, sync only
the four touched files to NAF:

```
rsync -av \
  /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/wrap_single_tau_lowpt_adaptive_5k_v1.sh \
  /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/wrap_single_tau_lowpt_adaptive_1k_v1.sh \
  /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/submit_tau_scan_lowpt_adaptive_5k_v1.condor \
  /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/submit_tau_scan_lowpt_adaptive_1k_v1.condor \
  naf-hone.desy.de:/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v3_percharge/
```

Build+merge outputs from clusters that ran before the failed tau scan
are reusable as-is:
- `output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/migration_inputs_lowpt_adaptive_5k_v1.root`
- `output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/migration_inputs_lowpt_adaptive_1k_v1.root`

so Step 3 can be re-submitted directly after the rsync. No need to
re-run Step 1 or Step 2.

### Step 4 - Summarize

```
python3 summarize_tau_scan_lowpt_adaptive_5k_v1.py \
    --scan-area-dir output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/tau_scan_Area \
    --scan-none-dir output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/tau_scan_None \
    --out-json      output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/tau_scan_summary.json
python3 summarize_tau_scan_lowpt_adaptive_1k_v1.py \
    --scan-area-dir output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/tau_scan_Area \
    --scan-none-dir output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/tau_scan_None \
    --out-json      output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/tau_scan_summary.json
```

### Step 5 - Aggregate uniform unfolded vector into 235/339 adaptive
physics bins

Not yet scripted. Each summary JSON references the per-tau JSON file
with `unfolded` (length 714 / 798) and `cov_diag`. To produce the
adaptive readout call (locally or on NAF):

```python
import json, numpy as np
import binning_lowpt_adaptive_5k_v1 as bm
j = json.load(open('unfolded_pos_tau<tag>.json'))
x_unif = np.asarray(j['unfolded'])
var_unif = np.asarray(j['cov_diag'])      # diagonal cov only
x_a, var_a = bm.reaggregate_uniform_to_adaptive(x_unif, var_unif)
# x_a shape (235,), var_a shape (235,)
```

For full covariance propagation in adaptive space use the dense
`T_ADAPTIVE`:

```python
cov_a = bm.T_ADAPTIVE.T @ cov_full @ bm.T_ADAPTIVE   # (235, 235)
```

A dedicated postprocess script `postprocess_adaptive_readout.py` should
be authored when the unfolded ROOTs land; this is **not part of this
preparation task**.

## Blockers / open questions

### BLOCKER 1: Reco axis was chosen heuristically (must be reviewed)

The reco pT axis is constructed by 1-step midpoint refinement of the
gen pT axis where parents are >= 0.5 GeV wide. This gives a
reco:gen aggregate of ~2.35 (5k) and ~2.32 (1k), comparable to
refined-v1's 2.86. The very narrow low-pT gen bins (e.g.
[0, 0.0625) wide = 62.5 MeV) become 1:1 reco:gen, which removes
intra-bin resolution. **This may starve TUnfold of migration
information at very low pT and force large negative-bin counts.**

The user did not pre-specify the reco axis. The refined-v1 spec used a
slightly different strategy (legacy v3 reco edges, accept variable
reco:gen ratio). A more conservative alternative is to keep the
refined-v1 reco axis exactly and accept that gen bins narrower than
0.25 GeV (the legacy reco bin width) become rank-deficient in the
response matrix. **Final reco axis choice should be confirmed by
`h1-unfolder` before Step 1 is submitted.**

### BLOCKER 2: Data rebinning uniform-spread is approximate

The bg-subtracted data is currently available only in the canonical
240-bin (8 cells × 6 z × 5 pT) reco layout. The new fine reco axes
(1680 / 1848) are produced by uniform-spread splitting of canonical
parents into 8/4/4/2/2 children per canonical pT parent and 2 children
per canonical z parent (32 / 16 children per parent for 5k / 1k bins
in the [0, 0.5) GeV pT slice). This is a much larger split factor than
refined-v1's 4-way split and may inflate per-bin shot noise in the
adaptive readout. A "true" data rebin needs re-extracting the bg-sub
data at the adaptive reco layout; the workflow for this is the same
as the rev6 bg-subtraction chain and is out of scope of this prep.

### BLOCKER 3: `per_bin_sigma_rev6_v2.py` is hard-coded to NB_R=960,
NB_G=240

The user asked whether the current scripts can deliver σ_stat /
σ_reg / σ_model decomposition after the adaptive TUnfold outputs
exist. The answer is **NO without code changes**:

- `scripts/analysis/per_bin_sigma_rev6_v2.py` hard-codes `NB_R = 960`
  and `NB_G = 240`, uses canonical-layout filenames
  (`unfolded_<chg>_tau1_4384498883e-06.json` etc.), and depends on
  IBU bootstrap (`ibu_core_rev6`) and BBB inputs (`--bbb-json`) that
  do **not** exist for the adaptive layouts.
- The script also has hard-coded "240" strings in PDF/console labels.

Retrofitting is non-trivial:
  (a) parametrize NB_R / NB_G via CLI;
  (b) skip IBU / BBB blocks when those inputs are missing
      (currently they are unconditional `required` args);
  (c) replace literal "240" strings;
  (d) accept the adaptive aggregation matrix and emit BOTH 714/798
      uniform and 235/339 adaptive sigma breakdowns.

**Recommendation:** treat the σ-decomposition as a NEW deliverable to
be implemented after Step 3 unfolded outputs are in hand. A new
`per_bin_sigma_adaptive_lowpt_v1.py` (TUnfold-only path) is the
smallest viable artifact; do not try to retrofit `per_bin_sigma_rev6_v2.py`
in place.

### BLOCKER 4: Cell-label inconsistency between dry-run JSON and
binning module

Dry-run JSON uses `S3_xmerged`; refined-v1 / adaptive modules use
`S3merged`. Adaptive modules normalize the dry-run label internally at
the validate-against-dryrun step and pass. No downstream consumer
breaks. Flag for future tooling alignment.

## Audit update -- corrected uncertainty policy (2026-06-03)

The first cut of `scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py`
was audited and returned status `CONCERN` for three reasons:

1. The "closure-excess" component was implemented as an unsigned
   `max(0, delta_cross - delta_self)`, which silently zeroes any
   adaptive bin where the cross-response bias has the OPPOSITE sign
   of the self-response bias.
2. The data response swap term and the closure term were collapsed
   into a single `sigma_model`, hiding the dominance relationship
   and forcing a premature policy choice.
3. No hard policy flag was emitted on whether the covariance
   propagation was full (combinable) or diagonal-only (per-bin QA
   only); and the half-spread convention for the data response swap
   was not documented in the output.

The audit-corrected uncertainty pipeline (now implemented) is:

- **Stat** (`sigma_stat`): unchanged. Full uniform-grid covariance is
  projected when present (mode = `full_covariance_projected` and
  `covariance_combination_allowed = true`); otherwise diagonal
  variances are summed inside each adaptive bin (mode =
  `diagonal_uniform_approx`, `covariance_combination_allowed = false`,
  `diagonal_only_warning = true`) and downstream consumers MUST treat
  the result as per-bin QA only.
- **Reg** (`sigma_reg`): unchanged. `0.5 * |U(tau_hi) - U(tau_lo)|`.
- **Data response swap** (`sigma_data_model`): retained, with the
  half-spread convention `0.5 * |U_RG - U_DJ|` documented in the
  output field `data_model_half_spread_convention`.
- **Closure** (`sigma_closure_signed`): NEW. Computed in adaptive
  space from six closure inputs per charge -- `U(RG reco, RG resp)`,
  `U(RG reco, DJ resp)`, `U(DJ reco, DJ resp)`, `U(DJ reco, RG resp)`,
  `T_RG`, `T_DJ`. Uniform-grid arrays are projected through
  `T_ADAPTIVE`; already-adaptive arrays are passed through.
  Per-generator signed residuals and Delta_G are emitted, and
  `sigma_closure_signed = max(|Delta_RG|, |Delta_DJ|)`. The unsigned
  legacy form is gone. The self-test exercises a synthetic sign-flip
  case and verifies that the new formula catches adaptive bins the
  old form would have zeroed.
- **Model aggregates**: both `sigma_model_max =
  max(sigma_data_model, sigma_closure_signed)` and `sigma_model_quad =
  sqrt(sigma_data_model^2 + sigma_closure_signed^2)` are reported per
  bin. `sigma_model` is provisionally set to `sigma_model_max` and
  metadata records `sigma_model_policy =
  provisional_max_pending_correlation_review`. Per-bin
  `model_dominant_source_for_max` plus payload-level
  `model_diagnostics` (counts of `data_model`/`closure`/`tied`/
  `unavailable`, Pearson correlation of the magnitudes,
  median(quad/max) ratio) support the eventual aggregation-policy
  decision; the choice is NOT frozen here.

### C_rad deferral

Radiative-correction systematics are deliberately NOT folded into the
adaptive low-pT_h uncertainty budget at this stage. The C_rad
pipeline is hard-coded as deferred and the metadata field
`c_rad_status = deferred_ignored_for_adaptive_uncertainty` is written
on every payload. Downstream consumers must add C_rad in quadrature
externally when the per-bin C_rad table for the adaptive layout
becomes available.

### Closure file schema

`--closure-dir <path>` is auto-scanned for filenames matching the
template `closure_<role>_<charge>.json` (case-insensitive on role; a
small alias set is supported: e.g. `rg_rg` for `selfRG`, `rg_dj` for
`crossRG`, etc.). Explicit-path overrides are available per role:
`--closure-self-rg`, `--closure-self-dj`, `--closure-cross-rg`,
`--closure-cross-dj`, `--closure-truth-rg`, `--closure-truth-dj`.
Each JSON must carry a length-NB_G, length-(NB_G+1) (overflow
trimmed), or length-N_ADAPTIVE_BINS central array under
`unfolded_pub` / `unfolded` / `unfolded_uniform` (or `truth` /
`truth_uniform` / `gen` for truth roles).

### Command template (after closure production lands)

```
python3 scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py \
    --variant 5k \
    --scan-dir   /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/tau_scan_lowpt_adaptive_5k_v1/Area \
    --charge     both \
    --tau-pos    2.069e-06  --tau-neg 2.069e-06 \
    --model-mode paired_dj \
    --closure-dir   /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/adaptive_lowpt_closure_v1/5k \
    --closure-mode  signed_excess \
    --out-dir       /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/adaptive_lowpt_sigma_v1
```

(Same template for `--variant 1k` with the matching scan and closure
directories.) `--closure-mode auto` (the default) enables
`signed_excess` automatically when a closure dir or any explicit
closure path is given, and is a no-op (`none`) otherwise.

### Local validation performed (no NAF/SSH)

```
python3 -m py_compile scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py   -> OK
python3 scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py --help          -> OK
python3 scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py --self-test     -> ALL TESTS PASS
  - 5k: sign-flip caught in 126 of 235 adaptive bins
        (signed>0, old-unsigned=0)
  - 1k: sign-flip caught in 170 of 339 adaptive bins
        (signed>0, old-unsigned=0)
```

## Out of scope of this preparation task

- Submitting any Condor jobs.
- Touching H1OO tree builders.
- Modifying `binning_v3.py`, `binning_lowpt_refined_v1.py`, or
  `build_migration_th2_per_charge_NWfix_v2.py` (canonical files;
  changes encapsulated in new wrappers/binning).
- (Resolved by audit-update 2026-06-03; see section above.)
  Implementing σ_stat / σ_reg / σ_model decomposition is now in
  `scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py`.
- Writing `postprocess_adaptive_readout.py` (referenced in Step 5).

## Audit update 2026-06-03 (data-DJ + closure production support)

The audit requires two additional production streams alongside the
nominal RG tau scan:

1. **Data response-swap (`dataDJ`)**: unfold the SAME real data with
   the DJANGOH-trained response on the same 20-tau grid. The sigma
   helper then forms `sigma_data_model = 0.5 * |U_RG - U_DJ|` at the
   canonical tau (per the existing half-spread convention).
2. **Closure suite**: produce four closure unfoldings per (charge, tau)
   -- `selfRG`, `selfDJ`, `crossRG`, `crossDJ` -- plus dump truth
   priors `truthRG`, `truthDJ`. The helper consumes these and computes
   `sigma_closure_signed` from signed variant-wise residuals.

### Files added (this audit update)

Under
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/`:

- `unfold_tunfold_single_tau_lowpt_adaptive_5k_v1_modes.C`
- `unfold_tunfold_single_tau_lowpt_adaptive_1k_v1_modes.C`
  Mode-aware adaptive worker macros. Mirror the nominal-only adaptive
  macros (`*_lowpt_adaptive_{5k,1k}_v1.C`) line-for-line for the
  layout, custom L matrix (1106 triplets 5k, 1246 triplets 1k),
  pathology counters, and the ROOT/JSON wiring. Add a `mode` parameter
  resolving response/pseudo-data/truth pairs:

    data    -> resp h_R_<chg>_nominal,   pdata h_data_bgsub_<chg>
    dataDJ  -> resp h_R_<chg>_djangoh,   pdata h_data_bgsub_<chg>
    selfRG  -> resp rapgap,  pdata h_matched_reco_marginal_<chg>_rapgap,
               truth h_prior_gen_<chg>_rapgap
    selfDJ  -> resp djangoh, pdata h_matched_reco_marginal_<chg>_djangoh,
               truth h_prior_gen_<chg>_djangoh
    crossRG -> resp djangoh, pdata h_matched_reco_marginal_<chg>_rapgap,
               truth h_prior_gen_<chg>_rapgap
    crossDJ -> resp rapgap,  pdata h_matched_reco_marginal_<chg>_djangoh,
               truth h_prior_gen_<chg>_djangoh

  For closure modes the pseudo-data overflow bin is set to
  `prior_total_pdata - matched_total_pdata` (clamped to 0), matching
  the canonical macro's REV3 §F-C closure fix so kEConstraintArea
  is satisfied. The JSON output adds `unfolded_pub` (alias of
  `unfolded`, NB_G+1 entries) and `truth` (closure only, NB_G entries)
  on top of the existing nominal schema keys; everything else is
  preserved so the helper accepts both nominal and modes outputs.
  Canonical nominal macros are NOT modified.

- `wrap_single_tau_lowpt_adaptive_5k_v1_modes.sh`
- `wrap_single_tau_lowpt_adaptive_1k_v1_modes.sh`
  Mode-aware Condor wrappers. Reuse the same ACLiC scratch-isolation
  pattern fixed in `wrap_single_tau_lowpt_adaptive_{5k,1k}_v1.sh`
  (private workdir under `_CONDOR_SCRATCH_DIR`, macro copy, trap-based
  cleanup). Route outputs by mode:

    data    -> lowpt_adaptive_<var>_v1/tau_scan_<cons>/unfolded_<chg>_tau<tau>.{root,json}
    dataDJ  -> lowpt_adaptive_<var>_v1/tau_scan_<cons>_dataDJ/unfolded_dataDJ_<chg>_tau<tau>.{root,json}
    self*/cross* ->
               lowpt_adaptive_<var>_v1/closure_<cons>/closure_<mode>_<chg>_tau<tau>.{root,json}

  Filenames match the per_bin_sigma_adaptive_lowpt_v1.py discovery
  template:
    * `unfolded_dataDJ_<chg>_tau*.json` is recognized by `_FILE_RX` as
      the `dataDJ` kind for `find_paired_dj`.
    * `closure_<role>_<chg>.json`-style basenames are recognized by
      `_discover_closure_file` (case-insensitive role tokens with the
      alias set: `selfrg`/`selfdj`/`crossrg`/`crossdj`/`truthrg`/
      `truthdj`).

- `tau_scan_lowpt_adaptive_5k_v1_dataDJ_args.txt` (80 lines)
- `tau_scan_lowpt_adaptive_1k_v1_dataDJ_args.txt` (80 lines)
  Same 20-tau grid as the nominal RG scan (Area + None, 2 charges).

- `closure_lowpt_adaptive_5k_v1_args.txt` (160 lines, Area only)
- `closure_lowpt_adaptive_1k_v1_args.txt` (160 lines, Area only)
  Full closure tau grid: 4 modes x 2 charges x 20 tau.

- `closure_lowpt_adaptive_5k_v1_selected_tau_args.txt` (empty template)
- `closure_lowpt_adaptive_1k_v1_selected_tau_args.txt` (empty template)
  Reserved for the operator-populated selected-tau closure subset. The
  files ship empty so submitting them as-is queues zero jobs. The
  operator must look up the canonical tau values (per charge) from the
  sigma-helper output AFTER the nominal RG scan completes, then write
  the matching (mode, charge, tau, constraint) tuples here. This
  preparation task does NOT guess a canonical tau.

- `submit_tau_scan_lowpt_adaptive_5k_v1_dataDJ.condor`
  request_memory=6000, runtime cap 3600s, notification=Never, getenv=
  False, log dir
  `condor/tunfold_tree_v3_percharge/tau_scan_lowpt_adaptive_5k_v1_dataDJ/logs`.
- `submit_tau_scan_lowpt_adaptive_1k_v1_dataDJ.condor`
  Identical settings except request_memory=5000 and log dir
  `condor/tunfold_tree_v3_percharge/tau_scan_lowpt_adaptive_1k_v1_dataDJ/logs`.

- `submit_closure_lowpt_adaptive_5k_v1.condor`
  request_memory=6000, runtime cap 3600s, log dir
  `condor/tunfold_tree_v3_percharge/closure_lowpt_adaptive_5k_v1/logs`.
- `submit_closure_lowpt_adaptive_1k_v1.condor`
  request_memory=5000, log dir
  `condor/tunfold_tree_v3_percharge/closure_lowpt_adaptive_1k_v1/logs`.
- `submit_closure_lowpt_adaptive_5k_v1_selected_tau.condor`
- `submit_closure_lowpt_adaptive_1k_v1_selected_tau.condor`
  Same per-variant settings as the full-grid closure submits;
  separate log dirs under `closure_lowpt_adaptive_<var>_v1_selected_tau/logs`.

- `dump_closure_truth_lowpt_adaptive_v1.py`
  Tiny pyROOT one-shot dumper that reads the gen-level priors
  `h_prior_gen_<chg>_{rapgap,djangoh}` from the variant's
  migration_inputs ROOT and writes
  `closure_truth{RG,DJ}_{pos,neg}.json` into a target directory with
  schema `{schema, role, charge, NB_G, truth, truth_err,
  source_root, source_hist}`. Each `truth` array has length NB_G
  (uniform gen). The script runs in seconds against ONE local ROOT
  file and stays under the NAF login-node single-file 5-min cap; it
  does NOT use TUnfold or require Condor.

### Production commands (data-DJ + closure)

Under `WORK=/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`.
All paths are absolute. SSH/NAF execution is delegated to
`h1-script-runner` / `h1-naf-operator`; this plan only documents
the exact invocation sequence.

#### 5k variant

```
# 1. Data-DJ tau scan (mirrors the nominal RG scan, swapped response).
condor_submit $WORK/scripts/unfolding/tunfold_official_v3_percharge/submit_tau_scan_lowpt_adaptive_5k_v1_dataDJ.condor

# 2. Closure tau scan (full 20-tau grid, Area constraint, 4 modes x 2 charges = 160 jobs).
condor_submit $WORK/scripts/unfolding/tunfold_official_v3_percharge/submit_closure_lowpt_adaptive_5k_v1.condor

# 2-alt. Selected-tau closure (after operator populates the args file with chosen tau per charge):
#   - Edit
#     $WORK/scripts/unfolding/tunfold_official_v3_percharge/closure_lowpt_adaptive_5k_v1_selected_tau_args.txt
#     with one line per (mode, charge, tau, constraint) tuple.
#   - Then submit:
condor_submit $WORK/scripts/unfolding/tunfold_official_v3_percharge/submit_closure_lowpt_adaptive_5k_v1_selected_tau.condor

# 3. Truth dump (one-shot, login-node OK; produces the truthRG/truthDJ JSONs the helper needs).
python3 $WORK/scripts/unfolding/tunfold_official_v3_percharge/dump_closure_truth_lowpt_adaptive_v1.py \
    --variant 5k \
    --in-root $WORK/output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/migration_inputs_lowpt_adaptive_5k_v1.root \
    --out-dir $WORK/output/tunfold_tree_v3_percharge/lowpt_adaptive_5k_v1/closure_Area
```

#### 1k variant

```
condor_submit $WORK/scripts/unfolding/tunfold_official_v3_percharge/submit_tau_scan_lowpt_adaptive_1k_v1_dataDJ.condor
condor_submit $WORK/scripts/unfolding/tunfold_official_v3_percharge/submit_closure_lowpt_adaptive_1k_v1.condor
# selected-tau variant (after operator populates the args file):
condor_submit $WORK/scripts/unfolding/tunfold_official_v3_percharge/submit_closure_lowpt_adaptive_1k_v1_selected_tau.condor

python3 $WORK/scripts/unfolding/tunfold_official_v3_percharge/dump_closure_truth_lowpt_adaptive_v1.py \
    --variant 1k \
    --in-root $WORK/output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/migration_inputs_lowpt_adaptive_1k_v1.root \
    --out-dir $WORK/output/tunfold_tree_v3_percharge/lowpt_adaptive_1k_v1/closure_Area
```

### Expected output directories and per-file schema

Under
`$WORK/output/tunfold_tree_v3_percharge/lowpt_adaptive_<var>_v1/`:

```
tau_scan_Area/                # nominal RG, EXISTING (this audit does not touch it)
tau_scan_None/                # nominal RG, EXISTING
tau_scan_Area_dataDJ/         # NEW: 40 unfolded_dataDJ_{pos,neg}_tau<...>.{root,json}
tau_scan_None_dataDJ/         # NEW: 40 unfolded_dataDJ_{pos,neg}_tau<...>.{root,json}
closure_Area/                 # NEW: 160 closure_{selfRG,selfDJ,crossRG,crossDJ}_{pos,neg}_tau<...>.{root,json}
                              #      + 4 closure_truth{RG,DJ}_{pos,neg}.json from the dumper
```

Per-file JSON schema (modes worker output, both data-DJ and closure):

  * `schema`            = `tunfold_single_tau_lowpt_adaptive_<var>_v1_modes`
  * `charge`            in {pos, neg}
  * `mode`              in {data, dataDJ, selfRG, selfDJ, crossRG, crossDJ}
  * `response_mc`       in {nominal, rapgap, djangoh}
  * `pseudo_data_mc`    in {"" (data*), rapgap, djangoh}
  * `truth_mc`          in {"" (data*), rapgap, djangoh}
  * `is_closure`        bool
  * `tau`               float
  * `NB_R`, `NB_G`      `lowpt_adaptive_5k`: 1680/714; `_1k`: 1848/798
  * `L_triplets_pT`, `L_triplets_z`, `L_triplets_total`
                        5k: 630/476/1106 ; 1k: 714/532/1246
  * `chi2A`, `chi2L`, `dof`, `n_unf_bins`, `tot_unf`
  * `unfolded`          NB_G+1 entries (last = overflow)
  * `unfolded_pub`      alias of `unfolded`, NB_G+1 entries (helper
                        trims overflow when sizing equals NB_G+1)
  * `unfolded_err`      NB_G+1 entries
  * `refold`            NB_R+1 entries
  * `cov_diag`          NB_G+1 entries (diagonal of total cov)
  * `truth`             (closure only) NB_G entries on the uniform gen

Per-file JSON schema (truth dumper output):

  * `schema`     = `closure_truth_lowpt_adaptive_<var>_v1`
  * `role`       in {truthRG, truthDJ}
  * `charge`     in {pos, neg}
  * `NB_G`       714 (5k) or 798 (1k)
  * `truth`      NB_G entries
  * `truth_err`  NB_G entries
  * `source_root`, `source_hist`

### Sigma-helper consumption (no change to helper)

Once data-DJ tau scan and closure production land on the NAF, the
existing helper at
`scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py` consumes them
without modification. Example pull:

```
python3 scripts/analysis/per_bin_sigma_adaptive_lowpt_v1.py \
    --variant 5k \
    --scan-dir <local-mirror>/lowpt_adaptive_5k_v1/tau_scan_Area  \
    --charge both \
    --tau-pos <chosen> --tau-neg <chosen> \
    --model-mode paired_dj \
    --closure-dir <local-mirror>/lowpt_adaptive_5k_v1/closure_Area \
    --closure-mode signed_excess \
    --out-dir <local-out>/adaptive_lowpt_sigma_v1
```

The scan-dir is expected to ALSO contain (or be unioned with) the
dataDJ JSONs so `find_paired_dj` can locate them at the canonical
tau. Two operating options exist; pick one at consumption time:

  (a) Symlink/copy the dataDJ JSONs from
      `tau_scan_Area_dataDJ/` into `tau_scan_Area/` before running
      the helper.
  (b) Pass `--scan-dir <tau_scan_Area_dataDJ_+_tau_scan_Area>`
      (a merged staging directory).

Closure-dir convention: the helper's
`_discover_closure_file(closure_dir, role, charge)` picks a SINGLE
file per (role, charge) pair (the first match it finds for the
role-token + charge-token combination). A full-grid 20-tau closure
run therefore deposits 20 candidate files per (mode, charge); the
operator must either:

  (i) Run the closure production only at the chosen tau via
      `submit_closure_lowpt_adaptive_<var>_v1_selected_tau.condor`
      (after populating the selected-tau args file), so each
      (role, charge) pair has exactly one file; or
  (ii) Stage one closure JSON per role into a separate
      `closure_<var>_selected/` directory (e.g. by symlinking only
      the chosen-tau filename per (role, charge)) before pointing
      `--closure-dir` at it.

This consumption step is OUT of scope of the present preparation
task; it is recorded here so the operator can plan the retrieval and
staging layout.

### Local validation performed (no NAF/SSH)

```
python3 -m py_compile scripts/unfolding/tunfold_official_v3_percharge/dump_closure_truth_lowpt_adaptive_v1.py   -> OK
bash -n   scripts/unfolding/tunfold_official_v3_percharge/wrap_single_tau_lowpt_adaptive_5k_v1_modes.sh         -> OK
bash -n   scripts/unfolding/tunfold_official_v3_percharge/wrap_single_tau_lowpt_adaptive_1k_v1_modes.sh         -> OK
brace/paren balance check for the two _modes.C macros                                                            -> matched
g++ syntax-only check (with ROOT-API stubs) for the two _modes.C macros                                          -> only stub-related errors (Get/SetDirectory),
                                                                                                                    identical class to the existing adaptive macros
```

ROOT compilation (ACLiC) and TUnfold execution are intentionally NOT
run locally; both require the NAF LCG view and the live
migration_inputs ROOT files.

### Ready-state for the NAF operator

After this preparation task, the work that remains for
`h1-script-runner` / `h1-naf-operator` is:

1. `rsync` the new files in
   `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/`
   to `$WORK/scripts/unfolding/tunfold_official_v3_percharge/` on
   NAF. (`scripts` directory already exists; just sync new files.)
2. Ensure the log dirs exist (one-time mkdir):

   ```
   mkdir -p $WORK/condor/tunfold_tree_v3_percharge/tau_scan_lowpt_adaptive_5k_v1_dataDJ/logs
   mkdir -p $WORK/condor/tunfold_tree_v3_percharge/tau_scan_lowpt_adaptive_1k_v1_dataDJ/logs
   mkdir -p $WORK/condor/tunfold_tree_v3_percharge/closure_lowpt_adaptive_5k_v1/logs
   mkdir -p $WORK/condor/tunfold_tree_v3_percharge/closure_lowpt_adaptive_1k_v1/logs
   mkdir -p $WORK/condor/tunfold_tree_v3_percharge/closure_lowpt_adaptive_5k_v1_selected_tau/logs
   mkdir -p $WORK/condor/tunfold_tree_v3_percharge/closure_lowpt_adaptive_1k_v1_selected_tau/logs
   ```
3. `condor_submit` the four submits listed above (data-DJ x 2,
   full-grid closure x 2). Total: 80 + 80 + 160 + 160 = 480 jobs.
4. After completion, run `dump_closure_truth_lowpt_adaptive_v1.py`
   once per variant to deposit the four truth JSONs into the closure
   output dir.
5. (Optional, only if a selected-tau closure pass is desired) edit
   the `*_selected_tau_args.txt` files with the chosen
   (mode, charge, tau, constraint) tuples and `condor_submit` the
   matching `*_selected_tau.condor` submit file.
6. `rsync` the per-tau JSONs back to the local workspace under the
   directories named in the sigma-helper command template above.

No new H1OO build is required; no canonical script is modified.

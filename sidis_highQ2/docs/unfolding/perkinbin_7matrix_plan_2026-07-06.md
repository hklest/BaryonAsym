# Methodology plan: high-Q² charged-SIDIS TUnfold with 7 independent `(Q²,x)` matrices

**Date:** 2026-07-06  
**Scope:** read-only design for a side-study implementation; no analysis run and no ROOT code written here.  
**Workspace:** high-Q² inclusive charged-hadron SIDIS v6/z005 only. Do not mix with `proton_HiQ`, `lambda_HiQ`, low-Q² SIDIS, or historical v5g/pre-v6 products.

---

## 1. Goal and frozen binning contract

Implement a diagnostic/candidate-comparison TUnfold variant in which the high-Q² charged-SIDIS response is unfolded **independently in each of the seven generated `(Q²,x)` kinematic cells**, separately for h+ and h−, instead of using one global per-charge matrix. This matches the 2020 H1 charged-particle analysis style: each `(Q²,x)` bin is treated as a self-contained unfolding problem in `(z,pT_h)`, while migrations between `(Q²,x)` bins are not inverted but are absorbed as reco-side fakes and gen-side misses.

Frozen z005 binning per charge:

| cell | label | Q² range [GeV²] | x range |
|---:|---|---|---|
| 0 | `S0_x0` | `[150,300)` | `[0.0021,0.0071)` |
| 1 | `S0_x1` | `[150,300)` | `[0.0071,0.030)` |
| 2 | `S1_x0` | `[300,700)` | `[0.0042,0.016)` |
| 3 | `S1_x1` | `[300,700)` | `[0.016,0.069)` |
| 4 | `S2_x0` | `[700,1500)` | `[0.0099,0.034)` |
| 5 | `S2_x1` | `[700,1500)` | `[0.034,0.15)` |
| 6 | `S3_x0` | `[1500,20000)` | `[0.021,1.5)` |

Within each cell:

- gen `z` edges: `[0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]`, `NZ_GEN=7`;
- gen `pT_h` edges: `[0,0.5,1,2,4,8]`, `NPT_GEN=5`;
- **35 gen bins per cell per charge**;
- reco uses the same `z` binning and a 2× split of `pT_h`, hence **70 reco bins per cell per charge**.

Global per-charge indexing retained for alignment:

```text
gen_global  = cell*35 + iz*5  + ipt_gen       # 0..244
reco_global = cell*70 + iz*10 + ipt_reco_fine # 0..489
```

The seven-matrix study must preserve the final global output ordering used by the z005 release: 245 gen bins per charge, and 490 rows when h+ and h− are concatenated.

---

## 2. Core matched/fake/miss rule for one cell `c`

For each charge and MC response, define:

```text
G_c = {c*35, ..., c*35+34}  # global gen bins in cell c
R_c = {c*70, ..., c*70+69}  # global reco bins in cell c
```

Each cell-level TUnfold matrix has **70 measured reco bins × 35 truth bins**, plus ROOT underflow on the reco/input axis for misses if the native v6 convention is used.

### 2.1 MATCHED block

Fill the ordinary 70×35 physical response block for cell `c` only with tracks satisfying:

```text
reco cell == c AND gen cell == c
```

Equivalently, copy/slice only the global response entries:

```text
R_cell[r_local, g_local] = R_global[r_global, g_global]
where r_global in R_c and g_global in G_c
      r_local  = r_global - c*70
      g_local  = g_global - c*35
```

All migrations in `z` and `pT_h` **within** the same `(Q²,x)` cell remain in the 70×35 matrix. The two fine reco `pT_h` bins associated with each coarse gen `pT_h` bin are ordinary measured bins, as in the global v6 TUnfold geometry.

### 2.2 FAKE for cell `c` = reco-side inflow/background

A reco track in cell `c` is fake/background for the cell-level unfolding if it is measured in that cell but does not originate from a truth particle in the same generated cell. Therefore the cell-`c` fake vector is:

```text
fake_total_c[r in R_c] = fake_original_global[r]
                      + sum_{g not in G_c} R_global[r,g]
```

This includes two distinct components that must be saved separately for QA:

1. **cross-kinematic-bin inflow fake**  
   matched tracks with `reco cell == c` but `gen cell != c`;
2. **ordinary/original fake**  
   reco in phase space with no matched π/K/p truth partner in the analysis truth phase space, wrong species, unmatched, or otherwise routed to the global fake component.

Thus, in the user-specified language:

```text
FAKE for cell c = (reco cell == c AND gen cell != c)
               OR gen not in phase space / not matched / wrong species
```

### 2.3 MISS for cell `c` = gen-side outflow/inefficiency

A generated particle in cell `c` is missed by the cell-level unfolding if it does not reconstruct into the same `(Q²,x)` cell. Therefore the cell-`c` miss vector is:

```text
miss_total_c[g in G_c] = miss_original_global[g]
                      + sum_{r not in R_c} R_global[r,g]
```

This includes two distinct components that must be saved separately for QA:

1. **cross-kinematic-bin outflow miss**  
   matched tracks with `gen cell == c` but `reco cell != c`;
2. **ordinary/original miss**  
   gen in phase space with no reconstructed partner in reco phase space, or reconstructed but failing reco phase-space/selection requirements.

Thus, in the user-specified language:

```text
MISS for cell c = (gen cell == c AND reco cell != c)
                OR no reco / reco not in phase space
```

### 2.4 Relation to the single global matrix

Yes: the union of the seven per-cell physical matched blocks equals the **block diagonal** of the existing single global per-charge matrix:

```text
sum_c R_global[R_c, G_c] = block-diagonal part of R_global
```

All global off-diagonal matched entries with `reco cell != gen cell` are reclassified as:

- a **fake** in the destination/reco cell; and
- a **miss** in the source/gen cell.

This is intentional and is the 2020-H1-consistent self-contained construction. It does **not** mean the same track is counted twice inside one unfolding problem. It means the global cross-cell response correction is replaced by a local fake/miss treatment in the two affected cells.

Physics consequence: the seven-matrix method **cannot invert or correct cross-`(Q²,x)` migrations**. It assumes those migrations are small enough that treating them as inefficiency/background is adequate. This must be justified by explicit QA of cross-cell migration fractions, and the difference to the single global TUnfold must be treated as a key method-comparison diagnostic.

---

## 3. TUnfold dimension requirement and Schmitt trap

For each cell per charge:

```text
n_reco = 70
n_gen  = 35
n_reco >= n_gen  # PASS
```

This satisfies the TUnfold algorithm requirement. Cite the shared trap explicitly in implementation notes and validation reports:

- Schmitt 2012, arXiv:1205.6201v4, §1 immediately after Eq. (2), and §2.1: the measured data vector must be binned more finely than the unfolded distribution; in the paper notation `n ≥ m`, where `n` is the number of measured/reco bins and `m` is the number of truth/gen bins.
- Do **not** invert this ratio. A gen-finer-than-reco TUnfold setup is rank-deficient and regularizer-driven.

The global v6 setup also satisfies the requirement per charge (`490 ≥ 245`). The per-cell setup preserves the same 2:1 measured-to-truth granularity (`70 ≥ 35`).

---

## 4. Recommended response encoding for the side-study

### 4.1 Recommendation: use native v6 miss-underflow for the TUnfold input

For an apples-to-apples comparison with the current single-matrix v6 TUnfold diagnostic, the seven-matrix implementation should mirror the **native v6 miss-underflow convention**, not the historical v4 ordinary fake/miss-bin convention.

Current native v6 convention:

- pass a physical response of **70×35** per cell to `TUnfoldDensity`;
- use `TUnfold::kHistMapOutputVert`;
- put `miss_total_c[g]` into the ROOT reco/input-axis **underflow bin** `x=0` for each truth bin `g`;
- pass only the 70 physical measured reco bins to `SetInput`;
- treat the `GetOutput()` histogram as the full truth-fiducial result directly;
- do not apply a post-hoc `1/epsilon` correction.

The fake component is not represented as an ordinary output column in the TH2D passed to TUnfold. For strict consistency with the existing native v6 entry point, use the same external fake-subtraction convention as `build_native_miss_inputs_v6.py`:

```text
matched_same_reco_c[r] = sum_{g in G_c} R_global[r,g]
fake_total_c[r]        = fake_original_global[r] + sum_{g not in G_c} R_global[r,g]
keep_fraction_c[r]     = matched_same_reco_c[r] / (matched_same_reco_c[r] + fake_total_c[r])
data_fakesub_c[r]      = data_raw_c[r] * keep_fraction_c[r]
err_fakesub_c[r]       = err_raw_c[r]  * keep_fraction_c[r]
```

Save `fake_original_global` and `fake_cross_in` separately, even though they are summed for the nominal fake subtraction.

### 4.2 Keep an Encoding-B audit product, but do not feed it to the official comparison

It is useful to write an auxiliary audit TH2 per cell with ordinary Britzger Encoding B dimensions `(71 reco rows × 36 gen columns)`:

- row 70 = miss row;
- column 35 = fake column.

This audit object is ideal for conservation checks and for making fake/miss QA plots. However, it should not be the default object used for the official seven-matrix TUnfold comparison, because the current single-matrix source is native v6 miss-underflow.

### 4.3 Why not use the v4 ordinary fake/miss encoding as nominal here?

The v4 ordinary fake/miss-bin TUnfold path is historical/audit only in the high-Q² v6/z005 context. Using it for the new seven-matrix comparison would introduce two simultaneous changes:

1. global matrix → seven independent matrices; and
2. native miss-underflow/external fake handling → ordinary fake/miss bins.

That would not be a clean method comparison. The clean comparison changes only the block structure and cross-cell migration treatment while preserving the native v6 TUnfold convention.

---

## 5. Tau, regularization, and TUnfold configuration

Use the same TUnfold configuration as the current native single-matrix v6 diagnostic, with only the matrix dimensions and cell selection changed.

Nominal comparison setting:

```text
tau = 1e-6
```

Apply `tau=1e-6` to **each** of the seven cell-level TUnfolds for each charge and MC response. Use `tau=1e-6` for the single-matrix TUnfold numerator in the final ratio comparison as well.

Current native v6 macro settings to carry over unless the single-matrix reference is explicitly changed:

```text
class        = TUnfoldDensity
hist map     = TUnfold::kHistMapOutputVert
constraint   = TUnfold::kEConstraintNone
regularizer  = TUnfold::kRegModeNone
 density mode = TUnfoldDensity::kDensityModeNone
```

Do not introduce curvature regularization or bin-width density mode only in the seven-matrix variant. If a later single-matrix reference is rerun with `kRegModeCurvature` and/or `kDensityModeBinWidth`, the seven-matrix study must be rerun with the identical settings in every cell. For this planned apples-to-apples comparison, inherit the current native v6 setup.

Note: Schmitt 2012 recommends bin-width-aware density handling for non-uniform bins, but that is a separate optimization study. It should not be mixed into the first seven-matrix-vs-global comparison.

---

## 6. Reuse-first implementation design

Do not rewrite the response-building and TUnfold machinery from scratch. Fork the current canonical scripts into an isolated side-study script directory and apply minimal changes.

### 6.1 Primary strategy: slice/reclassify an existing global response

The per-cell response can be constructed from the existing global v6 response because the global matrix already contains cross-cell matched entries. Reprocessing sidistrees is not required for the first implementation if the needed global objects are available.

Inputs required from the global native/Encoding-B product:

- physical matched response `R_global[490,245]` per charge and MC;
- original global fake vector per reco bin;
- original global miss vector per gen bin;
- raw reco data vector per charge;
- optional prior/truth vectors for closure diagnostics.

Then perform deterministic slicing/reclassification using the rules in §2.

### 6.2 Scripts to fork

Fork these into the new isolated script root:

1. **Response/input construction**
   - Canonical source:
     - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py`
     - local mirror: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py`
   - Also fork/reuse:
     - `tunfold_official_v6_native_miss_z005/build_native_miss_inputs_v6.py`
     - `tunfold_official_v6_native_miss_z005/validate_native_miss_inputs_v6.py`
   - Minimal change:
     - create a new slicer/reclassifier, e.g. `build_perkinbin_native_miss_inputs_v6.py`, based primarily on `build_native_miss_inputs_v6.py`;
     - read the global response/fake/miss/data objects;
     - loop over `cell=0..6` and write 7 cell-level native-miss responses per charge/MC;
     - write component vectors `fake_cross_in`, `fake_original`, `miss_cross_out`, `miss_original`;
     - write conservation and migration-fraction JSON summaries.
   - Only fork the full tree-level `build_migration_th2_per_charge_v6.py` if a fresh global response must be regenerated inside the isolated study area. If used, keep the tree loop and selection logic unchanged; add only side-study output names and any extra cross-cell accounting summaries.

2. **TUnfold execution**
   - Canonical source directory:
     - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v6_native_miss_z005/`
     - local canonical mirror: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/tunfold_official_v6_native_miss_z005/`
   - Fork these files:
     - `unfold_native_miss_single_tau_v6.C` → `unfold_perkinbin_native_miss_single_tau_v6.C`
     - `produce_phase2_native_diagnostics_v6.py` → per-cell phase-2 driver
     - `finalize_native_phase2.py` → recombine seven cell outputs to global 245-bin per-charge order
     - `compare_native_tau0_vs_ibu_v6.py` → compare `tau=1e-6` seven-matrix and one-matrix TUnfold to IBU
     - `regenerate_tau0_native_roots.py` if the existing finalizer expects `tau0` naming
   - Minimal macro changes:
     - replace `NB_R=490, NB_G=245` with `NB_R=70, NB_G=35`;
     - add arguments `cell`, `charge`, `mc`, `tau`, `inputRoot`, `outDir`;
     - load object names including the cell label/index;
     - keep native miss-underflow validation: positive x-underflow miss sum, zero x-overflow miss sum, and `SetInput` length 70;
     - preserve the same TUnfold configuration and JSON diagnostics as the single-matrix macro.

3. **Plotting/QA**
   - Reuse plotting patterns from the native TUnfold diagnostic scripts where possible.
   - If new plot scripts are needed, keep them under the isolated side-study script root and write outputs only to the side-study plot directory.

---

## 7. Required output directory isolation

Everything for this study must live under a new, unambiguous side-study name. Recommended name:

```text
tunfold_perkinbin_7matrix_z005_20260706
```

Remote paths:

```text
Study output root:
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_perkinbin_7matrix_z005_20260706/

Study script root:
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_perkinbin_7matrix_z005_20260706/
```

Local paths:

```text
Local data/cache root:
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/unfolding/tunfold_perkinbin_7matrix_z005_20260706/

Local plot root:
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/unfolding/tunfold_perkinbin_7matrix_z005_20260706/

Local documentation path:
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/unfolding/perkinbin_7matrix_plan_2026-07-06.md
```

No central release, central response, or existing native v6 TUnfold directory should be overwritten.

---

## 8. Validation and conservation checks before unfolding

For each charge, MC, and cell, the input builder must write a validation JSON and fail on any violated gate below.

### 8.1 Geometry checks

- `NB_R_cell = 70` and `NB_G_cell = 35`.
- `SetInput` vector length is exactly 70.
- TUnfold response object has physical bins `70×35` plus miss in x-underflow only.
- No ordinary fake column is present in the native object used by TUnfold.
- Output bins map back to global gen bins by `global = cell*35 + local`.

### 8.2 Conservation checks against global matrix

For each charge/MC:

```text
sum_c sum_{r in R_c, g in G_c} R_global[r,g]
  == global block-diagonal matched sum
```

Cross-cell matched conservation:

```text
offdiag_global = sum_{r,g with cell(r) != cell(g)} R_global[r,g]

sum_c sum_r fake_cross_in_c[r]  == offdiag_global
sum_c sum_g miss_cross_out_c[g] == offdiag_global
```

Original fake/miss conservation:

```text
sum_c fake_original_c == global fake vector sum
sum_c miss_original_c == global miss vector sum
```

Data conservation:

```text
sum_c data_raw_c == global raw reco data vector for the charge
```

### 8.3 Acceptance/fake/miss sanity

For each cell and bin:

```text
0 <= keep_fraction_c[r] <= 1
0 <= fake_fraction_total_c[r] <= 1
0 <= miss_fraction_total_c[g] <= 1
```

where:

```text
keep_fraction_c[r]       = matched_same_reco_c[r] / (matched_same_reco_c[r] + fake_total_c[r])
fake_fraction_total_c[r] = fake_total_c[r] / (matched_same_reco_c[r] + fake_total_c[r])
miss_fraction_total_c[g] = miss_total_c[g] / (matched_same_truth_c[g] + miss_total_c[g])
```

Also save component fractions:

```text
fake_cross_fraction_c[r] = fake_cross_in_c[r] / (matched_same_reco_c[r] + fake_total_c[r])
miss_cross_fraction_c[g] = miss_cross_out_c[g] / (matched_same_truth_c[g] + miss_total_c[g])
```

### 8.4 Cross-cell migration QA required to justify seven independent unfoldings

The seven-matrix method is acceptable only if cross-cell migration is demonstrated to be small or if the observed difference to the global matrix is carried as a method/systematic caveat.

For each charge/MC/cell, report at least:

```text
f_in_c  = sum_{r in R_c, g not in G_c} R_global[r,g]
        / [sum_{r in R_c, all g} R_global[r,g] + fake_original_global in R_c]

f_out_c = sum_{r not in R_c, g in G_c} R_global[r,g]
        / [sum_{all r, g in G_c} R_global[r,g] + miss_original_global in G_c]

f_offdiag_global = sum_{cell(r) != cell(g)} R_global[r,g]
                 / sum_{all physical matched r,g} R_global[r,g]
```

Also write a 7×7 migration matrix:

```text
M_cell[reco_cell, gen_cell] = sum_{r in reco_cell, g in gen_cell} R_global[r,g]
```

No universal hard cut is imposed in this design note, but the report must flag any cell where cross-cell inflow or outflow is non-negligible compared with the cell's statistical/systematic precision. As a practical rule for interpretation:

- `f_in` and `f_out` at the few-percent level are plausibly compatible with the 2020-style independent-cell treatment;
- values approaching or exceeding 5–10% in a cell require explicit discussion, comparison to the single-matrix result, and likely a method systematic or a decision to prefer the global matrix for that region.

---

## 9. Standard QA plot set to hand to the plotter

Produce the following plot set **for each charge, MC, and kinematic cell** unless explicitly marked as global. Use the cell label in every title and file name.

### 9.1 Per-cell response and matrix-quality plots

1. **Native physical response heatmap**  
   - 70 reco bins vs 35 gen bins, physical matched block only.  
   - Show both linear and log-z versions if dynamic range requires it.  
   - Axes must annotate local `(iz, ipT)` structure and the parent cell label.

2. **Response with fake/miss sidebars**  
   - Physical 70×35 block plus side panels/bars for:
     - `fake_original`;
     - `fake_cross_in`;
     - `miss_original`;
     - `miss_cross_out`.
   - This is the preferred visualization of the reclassification rule.

3. **Purity and stability vs gen-compatible bin**  
   - Plot 35 gen bins on the x-axis.  
   - Define the two fine reco bins associated with gen bin `g` as `F(g)`.  
   - Stability:
     ```text
     stability_g = sum_{r in F(g)} R_cell[r,g]
                 / (sum_{r physical in cell} R_cell[r,g] + miss_total_c[g])
     ```
   - Purity:
     ```text
     purity_g = sum_{r in F(g)} R_cell[r,g]
              / sum_{r in F(g)} [sum_{g' in cell} R_cell[r,g'] + fake_total_c[r]]
     ```
   - Include MC-stat uncertainty or at least low-stat bin markers where available.

4. **Fake fraction vs local reco bin**  
   - 70 local reco bins.  
   - Show stacked or overlaid components:
     - total fake fraction;
     - cross-cell inflow fake fraction;
     - original fake fraction.

5. **Miss fraction / inefficiency vs local gen bin**  
   - 35 local gen bins.  
   - Show stacked or overlaid components:
     - total miss fraction;
     - cross-cell outflow miss fraction;
     - original no-reco/out-of-PS miss fraction.

### 9.2 Per-cell unfolding-result QA plots

6. **Reco data vs folded-MC/refold closure**  
   - local reco bin x-axis, 70 bins;  
   - plot fake-subtracted data vector used in `SetInput`;  
   - overlay `TUnfold::GetFoldedOutput()`;  
   - include ratio panel `refold/data` or `data/refold`;  
   - annotate `χ²/ndf`, `tau`, charge, MC, and cell.

7. **Unfolded data vs MC truth/prior shape**  
   - 35 local gen bins;  
   - plot unfolded result with covariance-derived errors;  
   - overlay the corresponding MC truth/prior normalized consistently for shape comparison;  
   - include ratio panel.

8. **MC closure ratio, Poisson-fluctuated**  
   - 35 local gen bins.  
   - Use Poisson-fluctuated reco pseudo-data, not a noiseless `R·truth` vector.  
   - Recommended minimum: 100 pseudo-experiments per cell/charge/MC.  
   - Plot median unfolded/truth ratio and 68% interval; optionally overlay mean bias.  
   - This plot is a required closure diagnostic before interpreting per-cell TUnfold as physics-quality.

9. **Uncertainty/correlation diagnostics**  
   - covariance diagonal or relative uncertainty vs local gen bin;  
   - max absolute global correlation per bin from `GetRhoIJtotal()` or equivalent;  
   - flag high-ρ bins.

10. **Condition/tau diagnostic**  
    - condition number or singular-value spectrum of the normalized 70×35 response; and
    - either L-curve/rho scan over the same tau grid used by native v6, or a compact plot showing the chosen `tau=1e-6` relative to the scan.

### 9.3 Global summary plots across cells

11. **7×7 cross-cell migration heatmap** per charge/MC  
    - `reco cell` vs `gen cell`;  
    - diagonal and off-diagonal values;  
    - normalized both by gen source cell and by reco destination cell if possible.

12. **Cross-cell migration fraction summary**  
    - bar chart of `f_in_c` and `f_out_c` for cells 0–6;  
    - separate h+ and h− or overlay with clear styling;  
    - include MC comparison if both RAPGAP and DJANGOH are used.

13. **Per-cell unfolded/IBU ratio preview**  
    - 35 local gen bins per cell;  
    - ratio `TUnfold(7-matrix, tau=1e-6) / IBU(k=4)`;  
    - used to identify which cells drive the global comparison.

14. **Negative/nonfinite-bin and status summary**  
    - table/plot by cell and charge containing:
      - `SetInput` status;
      - `DoUnfold` status;
      - number of negative unfolded bins;
      - number of nonfinite bins;
      - refold `χ²/ndf`;
      - max `ρ` or equivalent correlation metric.

---

## 10. Final comparison plot specification

### 10.1 Denominator: nominal IBU k=4 finalcandidate release

Use the canonical z005 IBU denominator:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

Local mirror if needed for plotting:

```text
/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/
```

Use the nominal IBU `k=4` products. Align by `(charge, global gen bin)` rather than by row order alone.

### 10.2 TUnfold single-matrix numerator

Use the current native v6 single-matrix TUnfold source:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v6_native_miss_z005_20260621/
```

Select or regenerate the single-matrix result at:

```text
tau = 1e-6
```

using the same native miss-underflow convention and no post-hoc `1/epsilon` correction.

### 10.3 TUnfold seven-matrix numerator

Use the recombined output from:

```text
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_perkinbin_7matrix_z005_20260706/
```

For each charge, concatenate the seven local 35-bin unfolded outputs in global cell order:

```text
cell 0 bins 0..34,
cell 1 bins 35..69,
...
cell 6 bins 210..244.
```

Then concatenate charges in the same convention as the IBU release to form a 490-row combined table if plotting both charges together.

### 10.4 Plot content

Produce the final method-comparison plot with both ratios on the same axes:

```text
ratio_7matrix = TUnfold(7 independent matrices, tau=1e-6) / IBU(k=4)
ratio_1matrix = TUnfold(single global matrix, tau=1e-6) / IBU(k=4)
```

Recommended plotting forms:

1. **Combined 490-bin global plot**  
   - x-axis: global release row/bin index, with h+ bins followed by h− bins if that is the release order;  
   - vertical separators at charge boundary and `(Q²,x)` cell boundaries;  
   - y-axis: ratio to IBU;  
   - two numerator curves/markers: seven-matrix TUnfold and one-matrix TUnfold;  
   - denominator line at 1.

2. **Per-charge 245-bin plots**  
   - x-axis: per-charge global gen bin 0–244;  
   - separators at cell boundaries every 35 bins;  
   - same ratio definitions.

3. **Per-cell small multiples**  
   - seven panels per charge, 35 bins per panel;  
   - useful for diagnosing which `(Q²,x)` cells drive deviations.

The comparison must annotate:

- `tau=1e-6` for both TUnfold variants;
- IBU denominator `k=4`;
- response convention: native v6 miss-underflow with external fake subtraction;
- MC response used for each TUnfold numerator;
- release/source paths in a sidecar JSON/manifest.

---

## 11. Physics caveats and decision logic

1. **Cross-cell migrations are not corrected by the seven-matrix method.**  
   They are converted to fakes/misses. This is 2020-H1-consistent, but it is only safe if the cross-cell migration fractions are small or covered by a method uncertainty.

2. **The single global matrix is the only method among the two TUnfold variants that can explicitly invert cross-`(Q²,x)` migrations.**  
   A difference between one-matrix and seven-matrix TUnfold localized in cells with large `f_in`/`f_out` should be interpreted as migration-model sensitivity, not as a numerical bug by default.

3. **Closure must be Poisson-fluctuated.**  
   Do not claim closure based on deterministic `R·truth` pseudo-data. Use Poisson reco pseudo-experiments and report bias/variance/coverage per cell.

4. **Fake/background covariance remains a promotion blocker.**  
   The seven-matrix study can use the same external fake subtraction as native v6 for an apples-to-apples diagnostic. It should not be promoted to nominal until fake/background covariance or `SubtractBackground()` treatment is addressed consistently.

5. **Tau comparison is diagnostic, not a nominal-method decision by itself.**  
   `tau=1e-6` is the requested common comparison value. The method decision still needs tau stability, high-ρ/negative-bin review, closure, response/MC systematics, and sparse/tail-bin scrutiny.

6. **Absolute cross-section policy remains outside this design.**  
   The ratio comparison to IBU is acceptable as a method diagnostic. Any normalization-sensitive physics product must use the current high-Q² normalization context and remains subject to the existing release blockers.

---

## 12. Implementation checklist

1. Create isolated remote/local directories listed in §7.
2. Fork canonical native v6 TUnfold scripts and response-input builder scripts into the isolated script root.
3. Build or read the global native/Encoding-B v6 response in the isolated study context.
4. Slice/reclassify the global response into seven native miss-underflow cell inputs per charge/MC using §2.
5. Write conservation and cross-cell migration QA JSONs; fail on geometry/conservation errors.
6. Run seven independent TUnfolds per charge/MC at `tau=1e-6` using 70 measured bins and 35 truth bins per cell.
7. Recombine cell outputs to the 245-bin per-charge global order.
8. Produce the QA plot set in §9.
9. Produce the final ratio comparison in §10 against IBU `k=4` and the single-matrix native v6 `tau=1e-6` result.
10. Interpret deviations using the cross-cell migration summaries before drawing any physics conclusion.

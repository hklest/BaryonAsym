# RAPGAP31 gen-only C_rad normalization deep-dive (2026-06-24)

## Scope
High-Q² inclusive charged-hadron SIDIS gen-only radiative-correction v1 forensic audit under:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/`

Read-only investigation. DJANGOH14 treated as the stable provisional baseline. RAPGAP31 remains quarantined.

## Executive verdict
**Primary verdict: GLOBAL_NORMALIZATION_LIKELY**.

The RAPGAP31 anomaly is overwhelmingly explained by the lumi term used in
`C_rad = (sum_nonrad/L_nonrad) / (sum_rad/L_rad)` rather than by the raw weighted event sums or by a strong per-bin shape distortion.

- RAPGAP31 integrated raw weighted-sum ratio is only about **0.902**.
- DJANGOH14 integrated raw weighted-sum ratio is only about **0.914**.
- These are close: RAPGAP is only **~1.4% lower** at raw-sum level.
- The large final offset comes from the lumi multiplier:
  - RAPGAP31 `L_rad/L_nonrad = 5293265 / 3397124 = 1.5581606677`
  - DJANGOH14 `L_rad/L_nonrad = 4108957 / 3560233 = 1.1541258676`
- The ratio of those lumi multipliers is **1.3500786278**, which nearly fully reproduces the RAPGAP/DJANGOH integrated C_rad gap.

So the RAPGAP31 `C_rad ≈ 1.405` is not driven by a large rad/nonrad weighted-yield imbalance; it is driven mainly by the adopted RAPGAP rad/nonrad relative luminosity normalization.

## Exact formula and selection implemented
From `scripts/histogramming/full_crad_worker.py` and `full_crad_finalize.py`:

### Weight formula
Per-track weight used in the worker:
- `w = weight1 * weight2`

### Event truth selection
- `150 <= Q2sGen < 20000`
- `0.1 < YsGen < 0.7`
- if present: `35 < GenEmpz < 70`

### Hadron truth selection
- `abs(GenPDG) in {211,321,2212}`
- `IsGtr != 0`
- `Mother2 == -1`
- if present: `ParticleSource != 4`
- `xFGen < 0`
- `abs(GenEta_lab) < 4.0`
- finite `zGen`, `pT_h`, `Q2sGen`, `YsGen`, `XsGen`
- no gen `pT_h` threshold in denominator
- **alpha < 0.2 is not used in this gen-only v1 path**

### Published formula
From `full_crad_finalize.py`:
- `sigma_proxy = sum(weight1*weight2) / L_MC`
- `C_rad = sigma_nonrad / sigma_rad`
- data luminosity does not enter

## Lumi constants hard-coded in finalizer
From `scripts/histogramming/full_crad_finalize.py`:

- RAPGAP31:
  - `L_rad = 5,293,265 nb^-1`
  - `L_nonrad = 3,397,124 nb^-1`
  - `L_rad/L_nonrad = 1.5581606677`
- DJANGOH14:
  - `L_rad = 4,108,957 nb^-1`
  - `L_nonrad = 3,560,233 nb^-1`
  - `L_rad/L_nonrad = 1.1541258676`

## Sample labels and manifests found
Study artifacts inspected:
- `scripts/histogramming/full_crad_finalize.py`
- `scripts/histogramming/full_crad_worker.py`
- `output/tables/full_crad_v1/full_crad_v1_summary.json`
- `data/manifests/genonly_same_generator_manifest_all.json`
- `data/manifests/manifest_rapgap31_rad.json`
- `data/manifests/manifest_rapgap31_nonrad.json`
- `data/manifests/manifest_djangoh14_nonrad.json`
- `docs/input_manifest_genonly_v1.json`
- `scripts/genonly_condor/run_genonly_entry_direct.sh`
- `README.md`

### RAPGAP31 manifest content
- nonrad entries: **520**
- rad entries: **1273**
- nonrad source labels: `NORAD.Q2GT100`
- rad source labels: `RAD.Q2GT60`
- nonrad year-period counts: `eminus06e7=256`, `eplus06e7=264`
- rad year-period counts: `eminus06e7=582`, `eplus06e7=691`
- rad sample IDs: `8947, 8948, 8949, 8950, 8951, 8952, 8953, 8954`
- nonrad sample IDs: `C0000000 ... C0000032`

### DJANGOH14 manifest content actually used in summary
The combined summary is internally consistent with:
- nonrad entries: **520**
- rad entries in final tables: **1373**
- hard-coded lumi used in summary: `L_rad=4108957`, `L_nonrad=3560233`

A side inconsistency was found:
- `data/manifests/manifest_djangoh14_rad.json` is currently empty (`entries=0`), while the final summary clearly contains populated DJANGOH rad outputs.
- This does not explain the RAPGAP anomaly, but it is a provenance inconsistency that should be cleaned up.

## Integrated recomputation / audit
Using `output/tables/full_crad_v1/full_crad_v1_summary.json` and the produced CSV tables:

### RAPGAP31
#### h+
- `sum_nonrad = 304,449,499.9818201`
- `sum_rad = 337,610,662.08916557`
- raw ratio `sum_nonrad/sum_rad = 0.9017769110`
- `sigma_nonrad = 89.61977837 nb`
- `sigma_rad = 63.78117515 nb`
- integrated `C_rad = 1.4051133137`

#### h-
- `sum_nonrad = 300,264,104.7674351`
- `sum_rad = 332,937,284.1586981`
- raw ratio `sum_nonrad/sum_rad = 0.9018638616`
- `sigma_nonrad = 88.38773762 nb`
- `sigma_rad = 62.89828379 nb`
- integrated `C_rad = 1.4052487968`

### DJANGOH14
#### h+
- `sum_nonrad = 276,408,750.0189315`
- `sum_rad = 302,285,116.6362153`
- raw ratio `sum_nonrad/sum_rad = 0.9143974837`
- `sigma_nonrad = 77.63782596 nb`
- `sigma_rad = 73.56735946 nb`
- integrated `C_rad = 1.0553297892`

#### h-
- `sum_nonrad = 273,126,746.7368084`
- `sum_rad = 298,672,129.26702595`
- raw ratio `sum_nonrad/sum_rad = 0.9144701496`
- `sigma_nonrad = 76.71597526 nb`
- `sigma_rad = 72.68806397 nb`
- integrated `C_rad = 1.0554136548`

## Stage-by-stage comparison: RAPGAP31 vs DJANGOH14
### 1) Raw weighted-sum ratio
Very similar between generators:
- RAPGAP31 h+: `0.9017769110`
- DJANGOH14 h+: `0.9143974837`
- RAPGAP/DJANGOH raw-ratio fraction: **0.9861979359**

So before lumi scaling, the generators differ by only **~1.4%**.

### 2) Lumi ratio factor
Strongly different:
- RAPGAP31 `L_rad/L_nonrad = 1.5581606677`
- DJANGOH14 `L_rad/L_nonrad = 1.1541258676`
- RAPGAP/DJANGOH lumi-factor fraction: **1.3500786278**

This is the dominant source of the final discrepancy.

### 3) Final integrated C_rad
- RAPGAP31 h+: `1.4051133137`
- DJANGOH14 h+: `1.0553297892`
- RAPGAP/DJANGOH final fraction: **1.3314447560**

This matches the decomposition:
- `1.3314447560 ≈ 0.9861979359 × 1.3500786278`

Thus the final gap is almost entirely lumi-driven, with only a tiny opposing raw-sum effect.

### 4) Finite-bin counts
- RAPGAP31: `238/245` finite per charge, `476/490` all-charge
- DJANGOH14: `239/245` finite per charge, `478/490` all-charge

Finite-bin coverage is similar and does not point to a catastrophic acceptance-definition mismatch.

### 5) Per-bin shape comparison
Per-bin medians:
- RAPGAP31 h+ median `C_rad = 1.4783988908`
- DJANGOH14 h+ median `C_rad = 1.0767678389`
- RAPGAP31 h− median `C_rad = 1.4772299457`
- DJANGOH14 h− median `C_rad = 1.0804518527`

But raw per-bin medians are much closer:
- RAPGAP31 h+ median raw ratio `nonrad_sumw/rad_sumw = 0.9488103002`
- DJANGOH14 h+ median raw ratio `0.9329726238`
- RAPGAP31 h− median raw ratio `0.9480600918`
- DJANGOH14 h− median raw ratio `0.9361646620`

Aligned-bin comparison:
- h+: median `(C_rad_RG / C_rad_DJ) = 1.33518`, but median raw-ratio fraction is only `0.98896`
- h−: median `(C_rad_RG / C_rad_DJ) = 1.33635`, but median raw-ratio fraction is only `0.98983`

Interpretation:
- There may still be secondary sample-definition effects in some tails.
- However, the broad per-bin uplift is also consistent with a near-global multiplicative normalization offset from lumi.
- The anomaly is **not** dominated by a dramatic raw-shape distortion.

## Required RAPGAP relative-lumi correction to match DJANGOH
To bring RAPGAP31 integrated h+ `C_rad` from `1.4051133137` down to the DJANGOH-like `1.0553297892`, the RAPGAP rad/nonrad relative normalization would need a factor:

- required multiplier on `(L_rad/L_nonrad)` = **0.7510638316**

Equivalent statements:
- RAPGAP current `L_rad/L_nonrad = 1.5581606677`
- required `L_rad/L_nonrad ≈ 1.1703032455`
- or current RAPGAP relative-lumi term is too large by **1.3314447560**
- equivalently the inverse correction on `(L_nonrad/L_rad)` is **1.3314447560**

This required correction is numerically very close to the observed final RAPGAP/DJANGOH `C_rad` ratio and does **not** correspond to any large raw weighted-sum mismatch.

## Does the needed factor match an obvious artifact ratio?
Yes, qualitatively and quantitatively:

- The needed correction is **not** matched by raw weighted-sum ratios.
- It is matched almost exactly by the discrepancy in the adopted lumi multipliers.

Specifically:
- RAPGAP vs DJANGOH raw-ratio fraction: `0.9862` (small)
- RAPGAP vs DJANGOH lumi-factor fraction: `1.3501` (large)
- Product: `1.3314`, equal to the final integrated `C_rad` inflation factor.

So the most obvious candidate root cause is that the RAPGAP31 rad/nonrad effective luminosities used in the finalizer are not on the same footing as the DJANGOH pair, or the RAPGAP rad/nonrad sample definitions imply a different effective normalization that was not accounted for consistently.

## Additional notable forensic findings
1. **RAPGAP rad/nonrad source labels differ in generator threshold naming**
   - rad labels: `RAD.Q2GT60`
   - nonrad labels: `NORAD.Q2GT100`
   The worker applies a common analysis truth selection `Q2 > 150`, so this alone does not force the anomaly, but it is a provenance red flag and should be checked against MCDB/generator logs.

2. **ParticleSource handling mismatch between documentation and worker reality**
   - README says `ParticleSource` is sentinel `-1` / TODO.
   - worker uses `ParticleSource != 4` whenever the branch exists.
   - summary diagnostics report `used_particle_source_veto_all_nonempty = true` for both generators and both rad/nonrad modes.
   This is not the main RAPGAP problem, but docs should be aligned with implementation.

3. **DJANGOH rad-manifest inconsistency**
   - standalone `manifest_djangoh14_rad.json` is empty, while final summary clearly used populated DJANGOH rad partials/tables.
   Fix for provenance bookkeeping is needed.

## Bottom line
The forensic evidence strongly favors a **global normalization / effective-lumi provenance problem** for RAPGAP31 rad vs nonrad, not a weight-formula problem and not a dominant shape-only/sample-definition effect.

## Recommended next action
1. **Do not use RAPGAP31 for nominal C_rad or generator-envelope systematics yet.** Keep quarantined.
2. **Route the exact RAPGAP31 rad/nonrad effective luminosities and sample IDs to MCDB / normalization review**:
   - RAPGAP rad sample IDs `8947–8954`
   - RAPGAP nonrad chunks `C0000000–C0000032`
   - verify whether `5,293,265 nb^-1` and `3,397,124 nb^-1` are the correct effective lumis for the actually processed subsets and for the same e±/period composition.
3. **Audit whether RAPGAP rad/nonrad should be recombined per year/beam-charge before total-lumi collapsing**, instead of using one global pair of hard-coded lumis.
4. **After lumi provenance is fixed, regenerate only the study-local final summary/tables** and re-check whether RAPGAP integrated `C_rad` moves near `~1.05–1.10`.

## Artifact paths
### Remote study artifacts inspected
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming/full_crad_finalize.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming/full_crad_worker.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/full_crad_v1_summary.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/rapgap31_full_crad_v1_pos_245bins.csv`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/rapgap31_full_crad_v1_neg_245bins.csv`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/djangoh14_full_crad_v1_pos_245bins.csv`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/djangoh14_full_crad_v1_neg_245bins.csv`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/genonly_same_generator_manifest_all.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/manifest_rapgap31_rad.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/manifest_rapgap31_nonrad.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/manifest_djangoh14_nonrad.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/docs/input_manifest_genonly_v1.json`

### Durable local report
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/radiative_corrections/rapgap_crad_normalization_deepdive_2026-06-24.md`

# Research: TUnfold v5 tau=0 vs central IBU coherent scale shift

**Date:** 2026-06-21  
**Researcher:** Researcher B  
**Scope:** Inclusive charged-hadron SIDIS high-Q² z005 only; no PID/species work. Read-only investigation of the coherent TUnfold v5 physical/fake-subtracted tau=0 vs central IBU yield shift.  
**Status:** Evidence-grounded diagnostic research. Central z005 remains IBU/BBB finalcandidate; TUnfold v5 remains diagnostic-only / not promoted.

## 1. Authoritative context and release status

Mandatory context read before investigation:

- `/Users/hklest/Desktop/opencode/H1/CONTEXT.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/context-handoff.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/RUN_STATUS.md`

Key authoritative points from those files:

- Current high-Q² charged-SIDIS central production is v6, with central z005 release:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`
- The current z005 release has 490 rows total = 245 h+ + 245 h−; `NB_G_per_charge=245`, `NB_R_per_charge=490`, and IBU selected iteration `k=4`.
- TUnfold z005 diagnostics are explicitly not promotable; central z005 remains IBU/BBB finalcandidate.
- Luminosity convention for the current central release is `the superseded pre-rescale luminosity nb^-1`. RUN_STATUS also records a prior local-summary normalization audit where a `351000.0 / the superseded pre-rescale luminosity = 1.179206...` mismatch affected only a nominal cross-section column, not yields or relative uncertainty plots.

Remote access was used read-only after exactly one required control-socket check:

```bash
ssh -O check naf-hone.desy.de 2>&1
# Master running (...)
```

No production or modifying commands were run.

## 2. Files and artifacts inspected

### Local comparison artifacts

- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21/tau0_vs_ibu/tunfold_tau0_vs_ibu_ratios.csv`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21/tau0_vs_ibu/tunfold_tau0_vs_ibu_ratios.json`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21/tau0_vs_ibu/manifest_tau0_vs_ibu.md`
- Local mirror check only: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/ibu/cross_sections.json`

### Remote TUnfold v5 diagnostic artifacts

Root:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/`

Inputs / sidecars:

- `inputs/tunfold_v5_physical_inputs.root`
- `inputs/raw_data.json`
- `inputs/fake_vectors.json`
- `inputs/fake_subtracted_data.json`
- `inputs/efficiency_vectors.json`
- `inputs/miss_vectors.json`
- `inputs/response_summaries.json`
- `inputs/geometry_validation.json`

Tau scan / comparison:

- `phase2_tau_scan/json/tunfold_{pos,neg}_{rapgap,djangoh}_tau_0.json`
- `phase2_tau_scan/json/tau_scan_summary_v5_physical_fakesub.json`
- `phase2_comparisons/tau0_vs_ibu/json/tunfold_tau0_vs_ibu_ratios.json`

Manifests / validation:

- `manifest/v5_phase1_manifest.json`
- `manifest/v5_phase1_summary.md`
- `phase2_manifest/manifest_phase2_tunfold_v5_physical_fakesub_z005.json`
- `phase2_validation/phase2_input_validation_v5_physical_fakesub.json`

### Remote central IBU/BBB release artifacts

- `metadata.json`
- `summary_totals.json`
- `ibu/cross_sections.json`
- `bbb/cross_sections.json`
- `_run/bbb_ibu_crosscheck_{rapgap,djangoh}_{pos,neg}.json`
- `_run/run_bbb_ibu_release.py`

### Remote scripts inspected

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/run_bbb_ibu_release_v4.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/crosscheck_unfold_methods_v4.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/build_physical_response_fakesub_inputs_v5.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/unfold_tunfold_single_tau_v5_physical_fakesub.C`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_cross_generator_closure_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/make_cross_closure_tau_browse_v5.py`

## 3. Top-level verdict

The coherent `TUnfold tau=0 / central IBU` yield-ratio median near `0.59` is **not primarily a luminosity or hidden absolute-normalization mismatch**. It is, to high confidence, the central IBU acceptance/miss correction showing up in the comparison because TUnfold v5 tau=0 was run on a **physical 490×245 response block with fake-subtracted reco data but without encoding the miss row as an output bin / efficiency correction**, while IBU explicitly divides by the gen-bin detection efficiency `eps_j = sum_i B_ij / P_j`.

The most direct evidence is per-bin: for early high-stat h+ RAPGAP bins,

```text
TUnfold_tau0 / IBU_yield  ~=  acceptance_rapgap
```

Examples from the comparison and central IBU rows:

| global bin | TUnfold/IBU h+ RG | IBU `acceptance_rapgap` | ratio / acceptance |
|---:|---:|---:|---:|
| 0 | 0.3503303338 | 0.3502896144 | 1.000116 |
| 1 | 0.4563427472 | 0.4550457952 | 1.002850 |
| 2 | 0.4686453535 | 0.4673882317 | 1.002690 |
| 3 | 0.4167913277 | 0.4159490139 | 1.002025 |
| 4 | 0.2251382188 | 0.2247862760 | 1.001566 |
| 5 | 0.5637811629 | 0.5597452768 | 1.007210 |
| 10 | 0.5895917175 | 0.5876464061 | 1.003310 |

Across all 245 bins per charge, the median of `(TUnfold/IBU)/acceptance` is very close to unity:

| charge | response | median `(TUnfold/IBU)/acceptance` | mean `(TUnfold/IBU)/acceptance` | p10 | p90 |
|---|---|---:|---:|---:|---:|
| h+ | RAPGAP | 0.99175 | 0.88408 | 0.68566 | 1.18606 |
| h+ | DJANGOH | 0.99321 | 0.90946 | 0.66925 | 1.22047 |
| h− | RAPGAP | 0.99129 | 0.94238 | 0.75506 | 1.23188 |
| h− | DJANGOH | 0.99186 | 0.93476 | 0.68742 | 1.25381 |

The mean is pulled by pathological/negative low-stat tau=0 bins, but the median-level relation is very strong.

## 4. Total and scale-factor audit

### 4.1 Central IBU/BBB totals from remote current release

Remote current central release (`summary_totals.json`, `ibu/cross_sections.json`, `bbb/cross_sections.json`) uses `lumi_nb = the superseded pre-rescale luminosity`:

| charge | IBU yield sum | IBU cross-section sum [nb] | yield/xsec | BBB yield sum | BBB cross-section sum [nb] | BBB yield/xsec |
|---|---:|---:|---:|---:|---:|---:|
| h+ | 4,025,509.067 | 13.52394954 | the superseded pre-rescale luminosity | 4,035,219.256 | 13.55657153 | the superseded pre-rescale luminosity |
| h− | 3,923,119.073 | 13.17996395 | the superseded pre-rescale luminosity | 3,932,166.602 | 13.21035969 | the superseded pre-rescale luminosity |

`metadata.json` also records the central response and release convention:

```json
{
  "lumi_nb": the superseded pre-rescale luminosity,
  "methods": ["IBU k=4", "BBB"],
  "central_response": "RAPGAP31",
  "model_response": "DJANGOH14 available",
  "caveats": [
    "PROVISIONAL diagnostic only",
    "raw/no-bgsub",
    "no C_rad",
    "not Born-level",
    "TUnfold central and TUnfold tau scans intentionally not used"
  ]
}
```

### 4.2 TUnfold v5 tau=0 sidecar totals vs IBU

From `inputs/*.json`, `phase2_tau_scan/json/tunfold_*_tau_0.json`, and central IBU:

| case | raw data sum | fake-sub data sum | TUnfold tau=0 unfolded sum | IBU yield sum for charge | TUnfold/IBU sum | fake-sub/IBU sum | raw/IBU sum | median TUnfold/IBU | median eff/acceptance |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| h+ RAPGAP | 1,975,301.0 | 1,763,665.127 | 1,762,033.058 | 4,025,509.067 | 0.437717 | 0.438122 | 0.490696 | 0.589592 | 0.605159 |
| h+ DJANGOH | 1,975,301.0 | 1,762,239.864 | 1,758,090.799 | 4,025,509.067 | 0.436738 | 0.437768 | 0.490696 | 0.596026 | 0.604775 |
| h− RAPGAP | 1,908,796.0 | 1,710,244.719 | 1,708,748.354 | 3,923,119.073 | 0.435559 | 0.435940 | 0.486551 | 0.590713 | 0.603027 |
| h− DJANGOH | 1,908,796.0 | 1,708,921.794 | 1,704,938.351 | 3,923,119.073 | 0.434587 | 0.435603 | 0.486551 | 0.591229 | 0.603113 |

Interpretation:

- TUnfold tau=0 unfolded sums are nearly the fake-subtracted reco data sums. This is consistent with TUnfold solving only the physical detected-response problem without extrapolating through the miss row.
- Central IBU totals are approximately fake-subtracted data divided by the yield-weighted acceptance. For h+ RAPGAP, `1,763,665 / 4,025,509 = 0.438`, i.e. a yield-weighted global detection fraction. This is lower than the per-bin median acceptance (`~0.605`) because high-yield low-z/low-pT bins have smaller acceptance (`~0.35–0.56`).
- The user-observed coherent per-bin median ratio near `0.59` is the **per-bin median acceptance**, not a luminosity scale.

### 4.3 Sidecar fake and miss totals

From TUnfold v5 input sidecars:

| case | fake sum | fake/raw | miss sum | response physical `B_sum` | efficiency mean | efficiency median | efficiency min/max |
|---|---:|---:|---:|---:|---:|---:|---:|
| h+ RAPGAP | 4,821,070.958 | 2.44068 | 51,284,102.950 | 40,595,178.531 | 0.572592 | 0.605159 | 0.025408 / 0.764728 |
| h+ DJANGOH | 5,505,852.099 | 2.78735 | 57,838,456.982 | 45,651,453.857 | 0.573151 | 0.604775 | 0.029159 / 0.766387 |
| h− RAPGAP | 4,551,789.983 | 2.38464 | 50,684,499.110 | 39,587,039.626 | 0.572675 | 0.603027 | 0.026007 / 0.764456 |
| h− DJANGOH | 5,208,638.896 | 2.72876 | 57,290,082.312 | 44,612,750.710 | 0.574707 | 0.603113 | 0.028187 / 0.766843 |

The fake sums are MC-weighted response-side totals, not data counts, so their absolute values being larger than raw data is not by itself a normalization bug. What matters for fake subtraction is the per-reco-row fraction `F_i / (sum_j B_ij + F_i)`.

The miss sums are large and are exactly the ingredient that TUnfold v5 records as validation sidecars but does not encode into the TUnfold solve. Omitting these misses from the solved response is the scale-shift mechanism.

## 5. Algorithm comparison: IBU includes efficiency/miss correction; TUnfold v5 does not

### 5.1 Central IBU/BBB release path

`run_bbb_ibu_release_v4.py` calls `derive_response_blocks` and then passes the physical response block plus fake-subtracted data into IBU:

```python
blocks = derive_response_blocks(ld["M_ext"], ld["data_fine"])
dvar = ld["data_fine_w2"] * (1.0 - blocks["f_i"]) ** 2
ibu = ibu_unfold(
    d_sig=blocks["d_sig"], d_sig_w2=dvar, B=blocks["B"],
    P=ld["prior_gen"], iters=ibu_iters)
bbb = bbb_unfold(d_sig=blocks["d_sig"], d_sig_w2=dvar,
                 A_j=blocks["A_j"], binmap=ld["binmap"])
```

`crosscheck_unfold_methods_v4.py` defines the fake fraction and acceptance:

```python
B = M_ext[:nb_r, :nb_g].copy()            # (NB_R, NB_G) physical
M_miss = M_ext[nb_r,  :nb_g].copy()       # (NB_G,) miss per gen
F_fake = M_ext[:nb_r, nb_g ].copy()       # (NB_R,) fake per reco

# Fake fraction per reco row: f_i = F_i / (sum_j B_ij + F_i).
reco_match_row = B.sum(axis=1)
denom_f = reco_match_row + F_fake
f_i = np.where(denom_f > 0, F_fake / np.maximum(denom_f, TINY), 0.0)
d_sig = data_fine * (1.0 - f_i)

# Acceptance per gen column: A_j = sum_i B_ij / (sum_i B_ij + M_j).
gen_match_col = B.sum(axis=0)
denom_A = gen_match_col + M_miss
A_j = np.where(denom_A > 0, gen_match_col / np.maximum(denom_A, TINY), 0.0)
```

IBU then builds `Aij = B / P`, derives `eps = Aij.sum(axis=0)`, and divides the update by `eps`:

```python
Aij = np.where(P[None, :] > 0, B / np.maximum(P[None, :], TINY), 0.0)
eps = Aij.sum(axis=0)
...
n_new = M_ji @ d_sig
n_new = np.where(valid, n_new / np.maximum(eps, TINY), 0.0)
```

This is the explicit efficiency/miss correction that maps detected fake-subtracted reco data to truth-fiducial yields.

### 5.2 TUnfold v5 physical/fake-subtracted path

`build_physical_response_fakesub_inputs_v5.py` states the v5 diagnostic convention directly:

```python
"""Build diagnostic v5 TUnfold physical-response/fake-subtracted inputs.
...
B_ij = R[i,j] for i=1..490, j=1..245
F_i  = R[i,246]
data_fakesub_i = raw_i * sum_j B_ij / (sum_j B_ij + F_i)
M_j  = R[491,j] and eps_j = sum_i B_ij / (sum_i B_ij + M_j) for validation only
No miss data bin and no fake output bin are written as TUnfold inputs.
"""
```

The implementation writes `h_B`, `h_fake`, `h_miss`, `h_eff`, and `h_data_fakesub`, but the miss and efficiency histograms are only sidecars:

```python
hB=ROOT.TH2D(..., NB_R, ..., NB_G, ...)
...
miss.append(m)
eff.append(s/den if den>0 else float("nan"))
...
hfs=hist1(f"h_data_fakesub_{ch}_{mc}", ..., fakesub, fakesuberr)
```

The TUnfold macro consumes only `h_B_*` and `h_data_fakesub_*`:

```cpp
TH2D* hB=(TH2D*)fin->Get(Form("h_B_%s_%s",ch.c_str(),m.c_str()));
TH1D* hd=(TH1D*)fin->Get(Form("h_data_fakesub_%s_%s",ch.c_str(),m.c_str()));
...
TUnfoldDensity unfold(hB,
                      TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeNone,
                      TUnfold::kEConstraintNone,
                      TUnfoldDensity::kDensityModeNone);
...
unfold.SetInput(hd);
unfold.DoUnfold(tau);
TH1* hunf=unfold.GetOutput("h_unfolded");
```

This macro never passes the miss vector to TUnfold and never applies a post-unfold division by `eff`. Therefore its tau=0 solution naturally lives on the detected/matched-truth scale, not the truth-fiducial yield scale used by central IBU.

The v5 phase-1 summary records the same convention:

> `Convention: ordinary physical response B=R[1..490,1..245]; fake-subtracted physical data uses raw_i * sum_j B_ij/(sum_j B_ij + F_i); miss row and efficiencies are validation sidecars only; no ordinary miss data bin or fake output bin is used by TUnfold.`

## 6. Quantity-mismatch audit

### 6.1 Yield vs cross-section / luminosity

The local comparison manifest says the ratio uses yields only:

```text
Compared quantity: unfolded yields. TUnfold tau=0 values come from ROOT h_unfolded_245; central IBU denominator is yield_unfolded from ibu/cross_sections.json.
```

The comparison JSON repeats:

```json
"compared_quantity": "unfolded yields (TUnfold tau=0 ROOT h_unfolded_245 divided by central IBU yield_unfolded); central cross_section_nb is retained as reference but not used in the plotted ratio"
```

Thus luminosity should not enter the plotted ratio. The remote central release also has `yield/xsec = the superseded pre-rescale luminosity` exactly for both charges. The known historical `351000/the superseded pre-rescale luminosity` factor is:

| factor | value |
|---|---:|
| `351000 / the superseded pre-rescale luminosity` | 1.179206 |
| `the superseded pre-rescale luminosity / 351000` | 0.848028 |

Neither matches the observed median ratio `~0.59` or inverse `~1.69`. Luminosity mismatch is therefore rejected as the main cause of the coherent shift.

**Caveat / release-convention surprise:** a local mirror read at `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/release/.../ibu/cross_sections.json` showed stale-looking `lumi_nb: 351000.0` and cross-section entries scaled as `yield/351000`. The remote current release inspected on NAF has `lumi_nb: the superseded pre-rescale luminosity` and `yield/cross_section = the superseded pre-rescale luminosity`. The tau0-vs-IBU comparison manifest points to the remote central file and uses yields, so this local mirror inconsistency is not the cause of the tau0/IBU yield ratio. It is still a release-convention trap if future local-only work reads cross-section columns from the mirror.

### 6.2 Charge/global-index mismatch

The comparison rows are 490 rows with `charge`, `bin_index`, and `global_bin_index`. The first rows line up h+ `global_bin_index=0..` with central IBU h+ rows. The per-bin equality with `acceptance_rapgap` would be extremely unlikely under a charge or global-index mismatch. Charge/global-index mismatch is rejected as the primary cause.

### 6.3 Response-specific vs central average

Central IBU release writes RAPGAP as the central response and DJANGOH as model diagnostic:

```python
r = results[charge]["rapgap"]
model = results[charge].get("djangoh")
vec = r["ibu_sel"] if method == "ibu" else r["bbb"]["N_corr"]
...
"central_response": "rapgap31",
"model_response_available": model is not None,
```

The tau0 comparison denominator uses central IBU `yield_unfolded` (RAPGAP-central). TUnfold numerators are shown for both RAPGAP and DJANGOH. Both produce medians around `0.59` because both response efficiency vectors have nearly the same median (`~0.603–0.605`). This is not a central-average mismatch.

### 6.4 Data periods / lumi differences

The TUnfold v5 inputs are built from the same central v4 finalcandidate merged ROOT noted in the central release metadata:

- Central metadata `merged_root`: `.../output_v6/central/tunfold_official_v4_finalcandidate.../migration_inputs_v4_finalcandidate...root`
- TUnfold v5 manifest `input_root`: the same `.../migration_inputs_v4_finalcandidate...root`

The raw data sums from TUnfold sidecars are h+ `1,975,301` and h− `1,908,796`, totaling `3,884,097`, matching the central data-vector sidecar `per_bin.n_raw` sum `3,884,097`. Data-period mismatch is rejected.

### 6.5 C_rad / bin-width / cross-section conversion

The central release caveat is explicitly raw/no-bgsub, no C_rad, not Born-level. The comparison uses yields. No C_rad or bin-width conversion is part of the compared values.

## 7. Density-mode / bin-width assessment

TUnfold v5 macro uses:

```cpp
TUnfoldDensity unfold(hB,
                      TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeNone,
                      TUnfold::kEConstraintNone,
                      TUnfoldDensity::kDensityModeNone);
```

and constructs `hB` with unit index axes:

```python
ROOT.TH2D(..., NB_R, 0.0, float(NB_R), NB_G, 0.0, float(NB_G))
```

With unit-width index axes, switching to a density mode that uses the TH1/TH2 bin widths would not introduce a coherent physical z/pT bin-width factor; all encoded histogram bins have width 1 in the flattened index coordinate. If physical 4D bin widths were encoded, the factors would vary strongly across z and pT bins (`z` widths and `pT_h` widths are not uniform), not produce a charge- and response-coherent constant around `0.59`.

Density mode is still a methodological issue for publication-quality TUnfold on nonuniform physical bins, but it is not the source of the observed coherent tau0/IBU yield-ratio median. The observed ratio follows acceptance, not bin width.

## 8. Ranked root-cause hypotheses

### Rank 1 — Confirmed: TUnfold v5 physical/fake-sub response omits miss/efficiency correction while central IBU includes it

**Evidence:**

- v5 build script explicitly says miss row and efficiencies are validation-only sidecars and no miss data bin/fake output bin are written as TUnfold inputs.
- TUnfold macro consumes only `h_B` and `h_data_fakesub`, with `kEConstraintNone`, no explicit efficiency correction, and no miss row.
- Central IBU explicitly computes `eps = sum_i B_ij / P_j` and divides by `eps` at every iteration.
- Per-bin `TUnfold/IBU` equals central `acceptance_rapgap` to about the percent level in high-stat bins; median `(TUnfold/IBU)/acceptance` is `0.991–0.993` across charges/responses.
- TUnfold unfolded sums are approximately fake-subtracted reco sums; IBU sums are approximately fake-subtracted sums divided by a yield-weighted acceptance.

**Conclusion:** This is the dominant explanation for the coherent `~0.59` per-bin median ratio and the `~0.436–0.438` integrated-sum ratio.

### Rank 2 — Confirmed semantic mismatch: tau=0 TUnfold output is a matched/detected-truth-scale solution, not a truth-fiducial yield

This is closely related to Rank 1 but worth separating semantically. TUnfold tau=0 is not simply “IBU with k → infinity” here because the response semantics differ. TUnfold v5 solves `B x ≈ d_fakesub` with `B` excluding missed truth. IBU solves an efficiency-corrected truth-yield problem by using `Aij = B/P` and `1/eps_j` in the update. The comparison labels both as `yield_unfolded`, but they are not the same yield definition.

### Rank 3 — Supported minor/trap: local mirror / stale luminosity columns can confuse cross-section comparisons but do not affect this yield ratio

The local mirror checked in the workspace showed stale `lumi_nb: 351000.0` and cross sections scaled accordingly, while the remote current release is corrected to `the superseded pre-rescale luminosity`. The tau0-vs-IBU comparison uses remote yields and explicitly does not use cross sections. This is not the `0.59` cause, but it is a real convention trap for future local-only tables.

### Rank 4 — Rejected: luminosity hidden scale factor

The observed median ratios (`0.5896`, `0.5960`, `0.5907`, `0.5912`) and inverse (`~1.69`) do not match the known luminosity mismatch factors (`0.8480` or `1.1792`). The ratio uses yields. Remote central yields/cross-sections self-consistently use `the superseded pre-rescale luminosity`. Luminosity mismatch is rejected as the coherent-shift cause.

### Rank 5 — Rejected as primary: charge/global index mismatch or response averaging mismatch

The per-bin ratio tracks acceptance in correctly aligned rows, and both response-specific TUnfold numerators show the expected acceptance-scale behavior. This is not a charge/global index misalignment.

### Rank 6 — Rejected as primary: `kDensityModeNone` / bin-width factor

The v5 TUnfold histograms use unit flattened-index axes; physical bin widths are not encoded. A real physical bin-width effect would vary by z/pT/Q²/x bin, not produce the observed acceptance-like coherent factor.

## 9. Recommended next concrete diagnostics

The single most decisive next diagnostic is a **four-panel table/plot by global bin**:

1. `TUnfold_tau0_yield / IBU_yield`
2. central `acceptance_rapgap` (or response-specific `eff` for DJANGOH numerator)
3. `(TUnfold_tau0_yield / IBU_yield) / acceptance`
4. `TUnfold_tau0_yield` vs `IBU_yield * acceptance`

Use log-y or per-cell panels to handle low-stat pathological bins, and add summary statistics excluding bins with negative TUnfold yields or `IBU_yield < O(10)`.

A compact proof table should include:

| charge | response | median TUnfold/IBU | median acceptance | median `(TUnfold/IBU)/acceptance` | sum TUnfold | sum IBU | sum fake-sub | sum fake-sub / sum IBU |
|---|---|---:|---:|---:|---:|---:|---:|---:|

If someone wants to test a corrected TUnfold semantics, the diagnostic should not be another luminosity check. It should compare one of:

- TUnfold output after explicit post-hoc division by `eff_j` vs IBU; or
- a TUnfold input construction that encodes the miss row according to the intended TUnfold convention and checks whether `GetOutput` is truth-fiducial; or
- TUnfold refold of `IBU_yield * acceptance` against fake-subtracted data.

These should remain diagnostic-only unless the full TUnfold z005 health blockers are separately resolved.

## 10. Open questions and coverage gaps

- I did not run a ROOT-level extraction of `h_unfolded_245` values directly; I used the existing tau0 comparison JSON/CSV and single-tau sidecars, which already record the sums and per-bin values. This is sufficient for the scale-factor question but not a ROOT object integrity audit.
- I did not inspect Researcher A’s findings; this note is independent by construction.
- I did not rerun any unfolding or produce new plots. All numerical checks were computed read-only from existing JSON/CSV sidecars and release artifacts.
- The local mirror inconsistency (`351000.0` in the local cross-section JSON) should be separately reconciled if future local workflows consume cross-section columns, but it is out of scope for the tau0/IBU yield-ratio root cause.

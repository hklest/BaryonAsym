# Research: TUnfold v5 tau=0 vs central IBU coherent normalization shift

**Date:** 2026-06-21  
**Researcher:** Researcher A  
**Scope:** Inclusive charged-hadron SIDIS high-Q² z005 only; no PID/species work. Read-only normalization-algebra and implementation investigation. Production execution was not performed. Central z005 remains IBU/BBB finalcandidate; TUnfold v5 physical fake-subtraction study remains diagnostic-only/not promoted. **Supersession note 2026-06-24:** the C_rad blocker wording below is historical; current status is DJANGOH14 gen-only v1 provisional C_rad baseline with sparse-bin, MC-lumi provenance, and RAPGAP quarantine blockers.

## Authoritative context read first

- `/Users/hklest/Desktop/opencode/H1/CONTEXT.md`
  - Historical 2026-06-21 context: z005 IBU/BBB finalcandidate remained current; TUnfold z005 diagnostic remained failed/not promotable; C_rad uncertainty was still listed as blocker. Superseded 2026-06-24 by DJANGOH14 gen-only v1 provisional C_rad baseline in `docs/RUN_STATUS.md`.
  - Shared luminosity: `the superseded pre-rescale luminosity nb^-1`.
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/context-handoff.md`
  - Current release path: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`.
  - Validated z005 contract: 490 bins total, `NB_G_per_charge=245`, `NB_R_per_charge=490`, response shape `(491,246)` per charge, IBU selected `k=4`.
  - RooUnfoldBayes independently reproduces in-house IBU k=4 to floating-point precision.
  - TUnfold remains diagnostic/not promotable.
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/RUN_STATUS.md`
  - Single source of truth confirms same central release and TUnfold diagnostic status.
  - Notes a prior unrelated luminosity pitfall: a local total-summary table once used `351000.0 / the superseded pre-rescale luminosity = 1.179206...`, affecting only a nominal cross-section column, not canonical IBU `cross_section_nb`.

## Inspected files and evidence paths

### Local tau0-vs-IBU artifacts

- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21/tau0_vs_ibu/manifest_tau0_vs_ibu.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21/tau0_vs_ibu/tunfold_tau0_vs_ibu_ratios.json`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21/tau0_vs_ibu/tunfold_tau0_vs_ibu_ratios.csv`

Key manifest lines:

```text
Compared quantity: unfolded yields. TUnfold tau=0 values come from ROOT `h_unfolded_245`; central IBU denominator is `yield_unfolded` from `ibu/cross_sections.json`.
```

Recorded ratios:

| charge | response | median TUnfold/IBU | negative TUnfold bins | outside [0.75,1.25] |
|---|---:|---:|---:|---:|
| h+ | RAPGAP | 0.5895917175 | 14 | 218/245 |
| h+ | DJANGOH | 0.5960258142 | 15 | 215/245 |
| h- | RAPGAP | 0.5907133418 | 11 | 222/245 |
| h- | DJANGOH | 0.5912292128 | 9 | 224/245 |

Tau0 sidecar sums in the ratio JSON show TUnfold output sums are essentially equal to fake-subtracted reco-data sums, not efficiency-corrected truth-yield sums:

| charge | response | TUnfold unfolded sum | fake-sub data sum | central IBU sum | TUnfold/IBU sum ratio |
|---|---:|---:|---:|---:|---:|
| h+ | RAPGAP | 1,762,033.06 | 1,763,665.13 | 4,025,509.07 | 0.4377 |
| h+ | DJANGOH | 1,758,090.80 | 1,762,239.86 | 4,025,509.07 | 0.4367 |
| h- | RAPGAP | 1,708,748.35 | 1,710,244.72 | 3,923,119.07 | 0.4356 |
| h- | DJANGOH | 1,704,938.35 | 1,708,921.79 | 3,923,119.07 | 0.4346 |

These integrated ratios are lower than the per-bin median because the largest low-z bins have especially low acceptance (`~0.35-0.45`).

### Remote TUnfold v5 scripts copied read-only for inspection

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/build_physical_response_fakesub_inputs_v5.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/unfold_tunfold_single_tau_v5_physical_fakesub.C`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/compare_tau0_tunfold_vs_ibu.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_cross_generator_closure_v5_physical_fakesub.py` (copied but not central to the normalization finding)

### Remote TUnfold v5 input summaries copied read-only for inspection

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/inputs/response_summaries.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/inputs/efficiency_vectors.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/inputs/fake_subtracted_data.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/inputs/fake_vectors.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/inputs/miss_vectors.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/v5_phase1_summary.md`

### Central IBU/BBB implementation and release files copied read-only for inspection

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/ibu/cross_sections.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/bbb/cross_sections.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/run_bbb_ibu_release_v4.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/crosscheck_unfold_methods_v4.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/ibu/ibu_core_rev6.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/ibu/run_ibu_rev6.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/ibu/run_ibu_rev6_v2.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/ibu/run_ibu_rev6_NWfix.py`

## TUnfold v5 algebra and implementation

### v5 input builder convention

`build_physical_response_fakesub_inputs_v5.py` declares the repaired v5 convention in its module docstring:

```python
B_ij = R[i,j] for i=1..490, j=1..245
F_i  = R[i,246]
data_fakesub_i = raw_i * sum_j B_ij / (sum_j B_ij + F_i)
M_j  = R[491,j] and eps_j = sum_i B_ij / (sum_i B_ij + M_j) for validation only
No miss data bin and no fake output bin are written as TUnfold inputs.
```

The implementation confirms this exactly:

```python
hB=ROOT.TH2D(..., NB_R, ..., NB_G, ...)
...
v=float(hR.GetBinContent(i,j)); ...; hB.SetBinContent(i,j,v); ...; s+=v
...
miss.append(m)
den=s+m
eff.append(s/den if den>0 else float("nan"))
...
fac=(rs/den) if den>0 else 1.0
v=r*fac
...
fout.WriteObject(hB,hB.GetName())
...
hfs=hist1(f"h_data_fakesub_{ch}_{mc}",...,fakesub,fakesuberr)
```

Important consequences:

1. The TUnfold response histogram `h_B_*` is the **raw absolute matched-count block** `B`, not a column-normalized probability matrix and not an efficiency-weighted probability matrix. It is a TH2 of MC-weight counts.
2. Fake subtraction is applied by row fake fraction only: `data_fakesub_i = raw_i * B_row_sum_i/(B_row_sum_i+F_i)`.
3. Misses and efficiencies are computed only as sidecars (`h_miss`, `h_eff`, JSON) and are **not supplied to TUnfold** as a miss/inefficiency correction.
4. Therefore, unless TUnfold internally interprets under/overflow/miss bins that are actually present in the response, the output cannot be expected to be truth counts including missed tracks. It solves against the physical matched block only.

The v5 phase-1 summary restates the convention:

```text
Convention: ordinary physical response `B=R[1..490,1..245]`; fake-subtracted physical data uses `raw_i * sum_j B_ij/(sum_j B_ij + F_i)`; miss row and efficiencies are validation sidecars only; no ordinary miss data bin or fake output bin is used by TUnfold.
```

### v5 tau0 TUnfold macro

`unfold_tunfold_single_tau_v5_physical_fakesub.C` constructs `TUnfoldDensity` as:

```cpp
TUnfoldDensity unfold(hB,
                      TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeNone,
                      TUnfold::kEConstraintNone,
                      TUnfoldDensity::kDensityModeNone);
...
Int_t set_status=unfold.SetInput(hd);
unfold.DoUnfold(tau);
TH1* hunf=unfold.GetOutput("h_unfolded");
TH1* href=unfold.GetFoldedOutput("h_refold");
```

The macro writes:

```cpp
fout->WriteObject(hB,"h_response_physical_B");
fout->WriteObject(hd,"h_data_fakesub_physical");
fout->WriteObject(hunf,"h_unfolded_245");
fout->WriteObject(href,"h_refold_490");
```

Key normalization implications:

- `kDensityModeNone`: no bin-width scaling is applied. Bin width/density cannot explain a coherent ~0.59 factor.
- `kEConstraintNone`: no global area/luminosity constraint is imposed. This avoids the older F1 area-constraint pathology but also does not add missed events.
- The response passed to TUnfold is only `(490 reco x 245 gen)` physical matched block. The miss row `M_j` and therefore `eps_j = matched/(matched+miss)` are absent from the TUnfold inverse problem.
- At `tau=0`, with no regularization penalty, the output is a least-squares/inversion-like solution for a vector `x` satisfying `B x ~ data_fakesub` under TUnfold's internal response semantics. It is expected to refold to the fake-subtracted reco scale, not necessarily to equal the truth fiducial population.

### TUnfold v5 output scale observed directly

From `tunfold_tau0_vs_ibu_ratios.json`, the TUnfold sidecars report:

```json
"pos_rapgap": {
  "unfolded_sum": 1762033.0578377962,
  "data_sum": 1763665.1273483722,
  "refold_sum": 1762033.0578377948
}
```

Analogous h+/DJANGOH, h-/RAPGAP, h-/DJANGOH sums are all within ~0.2% of their fake-subtracted data sums. This is strong evidence that v5 TUnfold tau0 outputs are essentially **matched-reco/fake-subtracted-yield scale**, not acceptance-corrected truth scale.

## Central IBU algebra and implementation

The central release builder uses `run_bbb_ibu_release_v4.py`, which calls `crosscheck_unfold_methods_v4.py` helpers. The release writer stores:

```python
"yield_unfolded": float(vec[i]),
"cross_section_nb": float(vec[i] / lumi),
...
"acceptance_rapgap": float(r["blocks"]["A_j"][i]),
"prior_gen_rapgap": float(r["loaded"]["prior_gen"][i]),
"central_response": "rapgap31",
```

with `LUMI_DEFAULT = the superseded pre-rescale luminosity` and selected IBU iteration `4`.

### Central response-block derivation

`crosscheck_unfold_methods_v4.py` documents and implements the extended response convention:

```text
M[i_reco, j_gen] ... physical reco bin i / physical gen bin j (matched MC events)
M[NB_R, j_gen] ... miss row M_j
M[i_reco, NB_G] ... fake column F_i
...
f_i  = F_i / (sum_j B_ij + F_i)
d_sig_i = d_i * (1 - f_i)
A_j  = sum_i B_ij / (sum_i B_ij + M_j)
```

Implementation:

```python
B = M_ext[:nb_r, :nb_g].copy()
M_miss = M_ext[nb_r,  :nb_g].copy()
F_fake = M_ext[:nb_r, nb_g ].copy()
...
f_i = np.where(denom_f > 0, F_fake / np.maximum(denom_f, TINY), 0.0)
d_sig = data_fine * (1.0 - f_i)
...
A_j = np.where(denom_A > 0,
               gen_match_col / np.maximum(denom_A, TINY), 0.0)
```

### Central IBU update includes explicit efficiency/acceptance correction

`crosscheck_unfold_methods_v4.py` IBU implementation:

```python
Aij = np.where(P[None, :] > 0, B / np.maximum(P[None, :], TINY), 0.0)
eps = Aij.sum(axis=0)                         # sum_i B_ij / P_j
valid = (P > 0) & (eps > 0)
...
num = Aij * n_k[None, :]
row_sum = num.sum(axis=1)
M_ji = np.where(row_sum[:, None] > 0,
                num / np.maximum(row_sum[:, None], TINY),
                0.0).T
n_new = M_ji @ d_sig
n_new = np.where(valid,
                 n_new / np.maximum(eps, TINY),
                 0.0)
```

The separate `ibu_core_rev6.py` expresses the same algebra even more explicitly for absolute-count responses:

```python
P[i,j]  = R[i,j] / R[:,j].sum()        column-normalized: P(reco|gen)
eps[j]  = R[:,j].sum() / prior[j]      per-gen acceptance (matched/prior)
...
p_next[j] = update[j] / eps[j]            <-- divide by per-gen acc
```

Thus central IBU's `yield_unfolded` is an **acceptance-corrected truth-fiducial yield** for the published bin. It is not merely a fake-subtracted reco yield. Cross sections are then `yield_unfolded / the superseded pre-rescale luminosity`.

## Algebra comparison table

| Aspect | TUnfold v5 physical fake-sub diagnostic | Central IBU/BBB finalcandidate |
|---|---|---|
| Input response object | `h_B_{charge}_{mc}`, shape `490x245`, raw absolute matched MC counts copied from physical block only | Extended response `(491,246)` decomposed into `B`, fake column `F_i`, miss row `M_j`, prior `P_j` |
| Fake handling | Row fake fraction: `data_fakesub_i = raw_i * sum_j B_ij/(sum_j B_ij+F_i)` | Same fake-fraction algebra: `d_sig_i = data_i * (1 - F_i/(sum_j B_ij+F_i))` |
| Miss/efficiency handling | Miss vector and `eff_j = sum_i B_ij/(sum_i B_ij+M_j)` computed for validation sidecars only; not supplied to TUnfold | Explicit `eps_j = sum_i B_ij / prior_j`; update divides by `eps_j` to correct acceptance/misses |
| Response normalization | Raw count TH2 passed to `TUnfoldDensity`; no explicit column normalization by script | Converts raw counts to `Aij = B/P_j`; equivalent to probability/efficiency response; central release records `acceptance_rapgap` |
| TUnfold/IBU output interpretation | Observed output sum tracks fake-subtracted reco data sum; effectively matched/reco-scale at tau=0 for this setup | Truth-fiducial, acceptance-corrected yield per bin, selected IBU k=4 |
| Bin-width/density | `kDensityModeNone`; no bin-width scaling | No bin-width scaling in yield; cross section is yield/lumi, not density in z/pT widths |
| Area/lumi constraint | `kEConstraintNone`; no global normalization constraint | No area constraint; absolute normalization from data counts, acceptance correction, then `/lumi` for cross section |
| Luminosity | Not used in tau0-vs-IBU yield comparison | `lumi_nb = the superseded pre-rescale luminosity` only converts yield to nb; ratio artifact compared yields, so lumi cancels / is irrelevant |

## Does ~0.59 match a known factor?

### It matches the per-bin acceptance/efficiency scale, not luminosity

The TUnfold/IBU ratio medians closely match the median efficiency sidecar values from v5 inputs:

| charge | response | median TUnfold/IBU | median `eff_j = matched/(matched+miss)` | median `(TUnfold/IBU)/eff_j` |
|---|---:|---:|---:|---:|
| h+ | RAPGAP | 0.58959 | 0.60516 | 0.99175 |
| h+ | DJANGOH | 0.59603 | 0.60477 | 0.99273 |
| h- | RAPGAP | 0.59071 | 0.60303 | 0.99129 |
| h- | DJANGOH | 0.59123 | 0.60311 | 0.99062 |

When restricting to bins with central IBU yield > 1000 (to avoid low-stat/ill-conditioned extremes), the match tightens:

| charge | response | bins | median `(TUnfold/IBU)/eff_j` | mean absolute `(ratio-eff)` | max absolute `(ratio-eff)` |
|---|---:|---:|---:|---:|---:|
| h+ | RAPGAP | 144 | 0.99555 | 0.02497 | 0.31347 |
| h+ | DJANGOH | 144 | 1.00023 | 0.03092 | 0.30114 |
| h- | RAPGAP | 138 | 0.99735 | 0.02846 | 0.42838 |
| h- | DJANGOH | 138 | 0.99894 | 0.03106 | 0.34424 |

Concrete examples from the artifact:

- h+ bin 0 RAPGAP: central IBU `664096.955`, TUnfold `232653.308`, ratio `0.350330`; central release `acceptance_rapgap=0.350289614` and v5 efficiency sidecar starts with `0.3503` for the corresponding h+ RAPGAP bin. This is an almost exact equality.
- h+ bin 1 RAPGAP: ratio `0.456343`, central BBB acceptance `0.455046`, same scale.
- h+ bin 4 RAPGAP: ratio `0.225138`, central BBB acceptance `0.224786`, same scale.

This pattern is the smoking gun: v5 TUnfold tau0 output is approximately `IBU truth yield * acceptance_j`, i.e. the matched/reco-visible component, because the miss/efficiency correction was omitted from the TUnfold response convention.

### It does not match luminosity

The tau0-vs-IBU artifact compares **yield to yield** (`h_unfolded_245` / `yield_unfolded`), not cross section to cross section. The central IBU luminosity `the superseded pre-rescale luminosity nb^-1` appears only in `cross_section_nb = yield_unfolded / lumi` and is not used in the plotted ratio. The known prior local-summary luminosity error factor `351000/the superseded pre-rescale luminosity = 1.1792` would shift by ~18%, not by ~40%, and in the opposite conceptual place (cross-section column only). Therefore the luminosity-mismatch hypothesis is not supported by the inspected evidence.

### It is not a charge-combination factor

The ratio is per charge with 245 bins per charge and is coherent for h+ and h-. A charge-combination or double-counting factor would likely be near 0.5 or 2 and would not track the per-bin acceptance values (0.03-0.77). The observed median ~0.59 and bin-by-bin pattern match acceptance, not charge combination.

### It is not bin-width/density scaling

`unfold_tunfold_single_tau_v5_physical_fakesub.C` explicitly uses `TUnfoldDensity::kDensityModeNone`. No bin-width factor is applied by the v5 macro. The release `yield_unfolded` fields are also yields, not density-scaled quantities.

### It is not fake subtraction alone

Fake subtraction changes total data sums only modestly compared with raw data:

| charge | response | raw data sum | fake-sub data sum | fake-sub/raw |
|---|---:|---:|---:|---:|
| h+ | RAPGAP | 1,975,301 | 1,763,665 | 0.8929 |
| h+ | DJANGOH | 1,975,301 | 1,762,240 | 0.8922 |
| h- | RAPGAP | 1,908,796 | 1,710,245 | 0.8960 |
| h- | DJANGOH | 1,908,796 | 1,708,922 | 0.8953 |

The observed median ratio ~0.59 is much closer to acceptance than to fake-sub/raw (~0.89). Fake subtraction is implemented consistently in both methods and cannot be the dominant coherent deficit.

### It is not an MC weight/luminosity scaling in the response

Both TUnfold v5 and central IBU start from the same absolute-weight response semantics. The central IBU code explicitly handles raw absolute counts by dividing `B` by the prior or column sums to form response probabilities and efficiencies. TUnfold v5 passes raw `B` directly and omits miss/prior information. The ratio follows `eff_j`, not a constant MC-luminosity scale. MC weight scale cancels in acceptance ratios and in TUnfold's response-shape solution unless the prior/miss normalization is used, which v5 does not do.

## Ranked root-cause hypotheses

### 1. Most likely / essentially established: TUnfold v5 omits miss/efficiency correction, so tau0 output is reco-visible/matched scale

**Evidence:**

- v5 builder explicitly excludes miss row from TUnfold inputs: `miss row and efficiencies are validation sidecars only`.
- v5 macro passes only `h_B` and fake-subtracted data to TUnfold.
- TUnfold sidecar sums match fake-subtracted data sums, not central IBU truth sums.
- Per-bin TUnfold/IBU ratios match acceptance/efficiency `matched/(matched+miss)`, with median ratio/efficiency near 0.99-1.00 and near-exact bin examples.
- Central IBU explicitly divides by `eps_j` in the D'Agostini update.

**Interpretation:** The repaired v5 fixed the fake/miss sign convention relative to v4 pathology but repaired it by removing misses from the TUnfold problem rather than representing them in a way TUnfold uses for acceptance correction. It therefore unfolded detector smearing among reconstructed/matched bins but did not extrapolate to the full published truth fiducial.

### 2. Secondary: TUnfold response scale semantics are not equivalent to central IBU probability/efficiency semantics

**Evidence:**

- v5 passes raw absolute count `B` to `TUnfoldDensity`; central IBU constructs `Aij=B/P_j` or equivalent `P(reco|gen)` plus `eps`.
- TUnfold can internally normalize response columns for migration probabilities, but because the miss row is absent, the column normalization can only describe reconstructed conditional distributions among visible reco bins. It cannot know the total truth denominator `matched+miss`.

**Interpretation:** The problem is not merely "raw vs normalized" in isolation; it is raw `B` without the truth prior/miss denominator. The missing denominator is exactly the acceptance factor.

### 3. Plausible contributor to low-stat outliers: tau0 direct inversion and ill-conditioned bins

**Evidence:**

- Tau0 TUnfold has 9-15 negative bins per charge/MC and poor refold chi2/ndf (`~3.85-10.24`), per sidecars.
- Some low-yield bins have extreme ratios (negative, >2, or near zero), so the per-bin ratio is not purely equal to acceptance everywhere.

**Interpretation:** Low-stat or ill-conditioned bins add scatter and negative outliers. They do not explain the coherent median shift, which is acceptance-driven.

### 4. Unlikely: luminosity/absolute-normalization mismatch

**Evidence against:**

- Compared quantity is yield/yield; luminosity is not used in numerator or denominator for the plotted ratio.
- Central `lumi_nb=the superseded pre-rescale luminosity` is correct in `ibu/cross_sections.json` and `run_bbb_ibu_release_v4.py` default.
- Known previous luminosity-column error was 1.1792, not ~0.59, and affected only a local nominal cross-section summary column.

### 5. Unlikely: bin-width/density, charge combination, fake-subtraction-only, MC-weight constant

**Evidence against:**

- `kDensityModeNone` and yield comparison rule out bin-width/density scaling.
- Per-charge comparison and ratio pattern rule out charge-combination.
- Fake-sub/raw is ~0.893-0.896, not ~0.59.
- Ratio is bin-dependent and tracks acceptance, not a constant MC-weight/lumi scale.

## Fastest diagnostic checks to confirm

These are read-only or minimal diagnostic suggestions for future execution; no production execution was done in this research pass.

1. **Overlay per-bin `TUnfold_tau0 / IBU` against v5 `h_eff` or central `acceptance_rapgap`.**  
   This is effectively already confirmed numerically here. A scatter plot of ratio vs efficiency should lie near the diagonal for high-yield bins.

2. **Divide TUnfold tau0 output by `eff_j` and compare to central IBU.**  
   Use `h_eff_{charge}_{mc}` or `efficiency_vectors.json`: compute `(h_unfolded_245 / eff_j) / IBU_yield_unfolded`. Expect median near 1 in populated bins if the missing acceptance correction is the full cause. This is the most direct scalar diagnostic.

3. **Check first few bins by hand from central release and v5 artifacts.**  
   Already strongly indicative: h+ bin 0 RAPGAP ratio `0.350330` equals acceptance `0.350290`; bin 4 ratio `0.225138` equals acceptance `0.224786`.

4. **Construct an analytic closure toy with v5 convention.**  
   Feed pseudo-data `d_i = sum_j B_ij` to v5 tau0; compare output to `gen_match_col = sum_i B_ij` and to `prior = gen_match_col + miss_j`. Expect v5 TUnfold to recover matched column scale, not prior truth, unless the miss row is encoded into the TUnfold response.

5. **If TUnfold is ever revisited, test a native TUnfold miss/inefficiency encoding.**  
   The current v5 convention intentionally writes no miss data bin or fake output bin. A future diagnostic should either use TUnfold's expected underflow/overflow inefficiency convention correctly or explicitly post-correct by `1/eff_j` with full uncertainty propagation. This would be a new diagnostic design, not a promotion of current v5.

## Risks, assumptions, and gaps

- **Risk:** The v5 TUnfold closure plots can look visually good because they are closing the visible/matched response problem, not the full truth-fiducial yield problem. This can mask the normalization defect.
- **Risk:** Post-hoc division by `eff_j` is not automatically a valid final TUnfold method; it would need covariance propagation, MC-stat uncertainty on efficiencies, and validation of correlations/regularization under the corrected truth-scale target.
- **Assumption:** The copied central `ibu/cross_sections.json` is the current release file cited by RUN_STATUS. This is supported by matching first-row `yield_unfolded` values used in the local tau0-vs-IBU artifact.
- **Gap:** I did not inspect the binary ROOT histograms directly beyond copied JSON summaries and script code; however the JSON sidecars are generated from those ROOT histograms and contain the relevant sums/efficiencies.
- **Gap:** No new ROOT/TUnfold execution was performed, per read-only/no-production instruction.

## Verdict

The coherent ~0.59 TUnfold tau0 / central IBU yield ratio is best explained by a **missing acceptance/miss correction in the v5 TUnfold physical-response/fake-sub convention**, not by luminosity. The v5 TUnfold input builder passes only the matched physical block `B` and fake-subtracted data to TUnfold; misses and efficiencies are sidecars only. Central IBU, by contrast, explicitly divides by `eps_j`/acceptance in the D'Agostini update. The observed ratios match `eff_j = matched/(matched+miss)` bin-by-bin, with medians near the efficiency medians and near-exact agreement in prominent bins. Therefore the TUnfold v5 tau0 output is on reco-visible/matched scale, while central IBU `yield_unfolded` is on acceptance-corrected truth-fiducial scale.

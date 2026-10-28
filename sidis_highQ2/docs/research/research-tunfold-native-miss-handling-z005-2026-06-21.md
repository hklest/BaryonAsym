# Research: Native ROOT TUnfold handling of misses/inefficiency for high-Q² charged-SIDIS z005

**Date:** 2026-06-21  
**Scope:** Inclusive charged-hadron SIDIS high-Q² z005 only. No PID/species work. Read-only research; no production, no Condor, no file edits outside this note.  
**Current release status at writing:** Central z005 remains IBU/BBB finalcandidate. All TUnfold v5 physical/fake-subtracted artifacts are diagnostic only and not promoted. **Supersession note 2026-06-24:** the line below saying C_rad uncertainty remained the release blocker is historical; current C_rad status is DJANGOH14 gen-only v1 provisional baseline with final release still blocked by sparse-bin policy, publication-grade DJ MC-lumi provenance, and RAPGAP quarantine decision.

## 1. Authoritative project context read first

Mandatory context files read before this investigation:

- `/Users/hklest/Desktop/opencode/H1/CONTEXT.md`
  - Historical 2026-06-21 status at time of investigation: z005 IBU/BBB finalcandidate remained current; RooUnfoldSvd/TSVDUnfold quarantined; TUnfold z005 diagnostic remained failed/not promotable; C_rad uncertainty was still described as the release blocker. Superseded 2026-06-24 by DJANGOH14 gen-only v1 provisional C_rad baseline in `docs/RUN_STATUS.md`.
  - Shared TUnfold trap at line 187: Schmitt 2012 requires `n_reco >= n_gen`; fakes should be handled by background subtraction, while the miss row is native.
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/context-handoff.md`
  - Current release: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`.
  - Validated z005 contract: 490 published rows total, `NB_G_per_charge=245`, `NB_R_per_charge=490`, response shape `(491,246)` per charge, nominal IBU `k=4`.
  - TUnfold v4 diagnostic remains not promotable; v5 artifacts discussed here are diagnostic follow-ups, not central release candidates.
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/RUN_STATUS.md`
  - Single source of truth confirms central z005 IBU/BBB finalcandidate, TUnfold diagnostic-only status, and SVD quarantine.

Remote inspection was needed and was started with the required control-socket check:

```bash
ssh -O check naf-hone.desy.de 2>&1
# Master running (...)
```

No remote write, production, Condor submission, or ROOT execution was performed. Remote commands were read-only file inspections.

## 2. Files and sources inspected

### Project/local artifacts

- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/tunfold_v5_physical_fakesub_z005_2026-06-21/tunfold_v5_reuse_manifest_2026-06-21.json`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/tunfold_v5_physical_fakesub_z005_2026-06-21/tunfold_v5_reuse_manifest_2026-06-21.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/research-tunfold-v5-normalization-shift-2026-06-21.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/research-tunfold-v5-tau0-ibu-scale-shift-2026-06-21.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`
- `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/docs/research/tunfold_paper_optimization_2026-05-31.md`

### Remote v5 diagnostic scripts inspected read-only

Script root:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/`

Inspected files:

- `build_physical_response_fakesub_inputs_v5.py`
- `unfold_tunfold_single_tau_v5_physical_fakesub.C`
- `unfold_tunfold_tau_scan_v5_physical_fakesub.C`
- `run_tau_scan_v5_physical_fakesub.sh`
- `run_cross_generator_closure_v5_physical_fakesub.py`
- `run_truth_fiducial_closure_v5_physical_fakesub.py`
- `compare_tau0_tunfold_effcorr_vs_ibu.py`
- `make_cross_closure_rhoi_tau_browse_v5_from_existing.py`

### ROOT/TUnfold sources and references consulted

- ROOT `TUnfold` class reference and source:
  - `https://root.cern/doc/master/classTUnfold.html`
  - `https://root.cern/doc/master/TUnfold_8cxx_source.html`
- ROOT `TUnfoldDensity` class reference:
  - `https://root.cern/doc/master/classTUnfoldDensity.html`
- ROOT `TUnfoldSys` class reference:
  - `https://root.cern/doc/master/classTUnfoldSys.html`
- ROOT `TUnfoldBinning` class reference:
  - `https://root.cern/doc/master/classTUnfoldBinning.html`
- Schmitt 2012 TUnfold paper:
  - S. Schmitt, *TUnfold, an algorithm for correcting migration effects in high energy physics*, JINST 7 (2012) T10003, arXiv:1205.6201v4.
  - Consulted via `https://ar5iv.labs.arxiv.org/html/1205.6201`.

`web-search` was unavailable in this environment due missing provider credentials; direct URL fetches and existing local research artifacts were used instead.

## 3. Executive verdicts

### 3.1 Native inefficiency/miss handling verdict

**FATAL for current v5 if the goal is native truth-fiducial TUnfold:** v5 does not encode misses in the ROOT/TUnfold-native way. It removes the miss row from the TUnfold response and stores misses/efficiencies only as sidecars. Consequently the raw TUnfold output solves a visible/matched problem. The later `x_visible / eps_response` comparison is a post-hoc hybrid correction, not native TUnfold miss handling.

**Correct native ROOT/TUnfold convention:** fill missed truth events in the detector-level underflow or overflow bins of the response histogram, i.e. the reconstructed axis outside the measured reco bins. For `kHistMapOutputVert`, the v5 macros use reco on the TH2 X axis and truth on the TH2 Y axis, so the native miss content belongs in `hM->SetBinContent(0, j, miss_j)` or `hM->SetBinContent(NB_R+1, j, miss_j)` for physical truth bin `j`, not in an ordinary measured row with a zero data input.

ROOT source explicitly supports this. In `TUnfold.cxx` constructor documentation:

```text
Treatment of overflow bins in the matrix hist_A
- Events reconstructed in underflow or overflow bins are counted as inefficiency. They have to be filled properly.
- Events where the truth level is in underflow or overflow bins are treated as a part of the generator level distribution. The full truth level distribution (including underflow and overflow) is unfolded.
```

The implementation then computes the generator normalization `fSumOverY` from the physical detector-bin sum plus detector underflow/overflow:

```cpp
// calculate sum over all detector bins excluding the overflow bins
...
// add overflow bins -> important to keep track of the non-reconstructed events
fSumOverY[nx] = sum;
fSumOverY[nx] += hist_A->GetBinContent(0, ix) + hist_A->GetBinContent(ny + 1, ix);
```

For `kHistMapOutputVert`, this means reco-underflow/reco-overflow on the TH2 X axis are included in the truth-column normalization but not in the fitted measured vector `y`. This is exactly Schmitt 2012 §3 eqs. 19-20: `A_ij = M_ij / s_j` for reconstructed bins `i > 0`, with `s_j = sum_{i=0..n} M_ij` including the miss row `i=0`.

### 3.2 Fake/background verdict

**FATAL for any method that treats fakes as a signal truth/output column:** ROOT documentation and Schmitt §5 say reco events with no truth origin in the unfolded fiducial are background to the measured distribution, not signal truth bins. They should be subtracted with `TUnfoldSys::SubtractBackground()` / `TUnfoldDensity::SubtractBackground()` or subtracted externally with an equivalent covariance supplied to `SetInput()`.

ROOT constructor advice is explicit:

```text
If unsure, do the following:
- store events where the truth is in underflow or overflow (sometimes called "fakes") in a separate TH1. Ensure that the truth-level underflow and overflow bins of hist_A are all zero.
- the fakes are background to the measurement. Use the classes TUnfoldSys and TUnfoldDensity instead of the plain TUnfold for subtracting background
```

Schmitt 2012 §5 states the same in statistical form. Background-subtracted data and covariance are:

```text
y = y0 - f^b b                                           (eq. 26)
V_yy = V^0_yy + delta_ij (f^b delta b_i)^2 + (delta f^b)^2 b_i b_j  (eq. 27)
```

ROOT `TUnfoldSys::SubtractBackground(bgr, name, scale, scale_error)` implements this: it subtracts `scale*bgr`, adds uncorrelated background histogram errors to the diagonal, and adds the correlated normalization uncertainty contribution `dscale*bgr_i dscale*bgr_j` to the input covariance.

### 3.3 Real ROOT/TUnfold verdict

**PASS:** the v5 diagnostic package uses real ROOT/TUnfold for actual unfolding. It is not a fake Python clone or ad hoc Python pseudo-inverse unfolding.

Evidence:

- `unfold_tunfold_single_tau_v5_physical_fakesub.C` includes real ROOT headers:

```cpp
#include "TUnfold.h"
#include "TUnfoldDensity.h"
```

- It constructs and executes real `TUnfoldDensity`:

```cpp
TUnfoldDensity unfold(hB,
                      TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeNone,
                      TUnfold::kEConstraintNone,
                      TUnfoldDensity::kDensityModeNone);
Int_t set_status=unfold.SetInput(hd);
unfold.DoUnfold(tau);
TH1* hunf=unfold.GetOutput("h_unfolded");
TH1* href=unfold.GetFoldedOutput("h_refold");
TH2* hcov=unfold.GetEmatrixTotal("h_cov_total");
TH2* hrho=unfold.GetRhoIJtotal("h_rho_total");
```

- `run_tau_scan_v5_physical_fakesub.sh` validates the real ROOT installation before execution:

```bash
PREFIX=$(root-config --prefix)
test -f "$(root-config --incdir)/TUnfold.h" || { echo missing TUnfold.h >&2; exit 21; }
test -f "$(root-config --incdir)/TUnfoldDensity.h" || { echo missing TUnfoldDensity.h >&2; exit 22; }
test -f "$(root-config --libdir)/libUnfold.so" || { echo missing libUnfold.so >&2; exit 23; }
root -l -b -q "$SCRIPT_DIR/unfold_tunfold_single_tau_v5_physical_fakesub.C(...)"
```

- Python closure scripts such as `run_cross_generator_closure_v5_physical_fakesub.py` use PyROOT and call real `ROOT.TUnfoldDensity`, `SetInput`, `DoUnfold`, `GetOutput`, `GetEmatrixTotal`, and `GetRhoIJtotal`.
- The one Python pseudo-inverse/pinverse use found is not unfolding: `make_cross_closure_rhoi_tau_browse_v5_from_existing.py` computes a global-correlation coefficient from already stored covariance matrices when `GetRhoI` histograms were not stored. It does not compute `x` or replace `DoUnfold()`.

## 4. Native ROOT/TUnfold handling of misses/inefficiencies

### 4.1 What ROOT expects in the response histogram

Schmitt §3 defines the Monte Carlo count matrix `M` with one extra reconstructed row `i=0` for generated events in truth bin `j` that are not reconstructed in any measured bin. TUnfold forms:

```text
A_ij = M_ij / s_j, where i > 0                 (eq. 19)
s_j  = sum_{i=0..n} M_ij                       (eq. 20)
x0_j = s_j                                     (eq. 21)
```

ROOT implements this through response-histogram underflow/overflow on the **reconstructed/input axis**, not by asking the user to pass a data bin for misses. For `TUnfold::kHistMapOutputVert`, the constructor maps:

- TH2 X axis = measured/reco/input bins (`ny = hist_A->GetNbinsX()`), physical bins only in the data vector;
- TH2 Y axis = unfolded/truth/output bins, with `GetNbinsY()+2` considered internally to allow truth under/overflow bins.

Therefore, for the high-Q² z005 per-charge geometry:

| Quantity | Native response placement for `kHistMapOutputVert` |
|---|---|
| Physical matched events `B_ij` | `hM->SetBinContent(i_reco=1..490, j_truth=1..245, B_ij)` |
| Missed truth events `M_j` | `hM->SetBinContent(i_reco=0, j_truth, M_j)` or `i_reco=491` overflow; use one convention and validate |
| Reco-only fakes `F_i` | **not** a truth column; separate background histogram `hFake(i_reco)` |
| Truth outside published fiducial | normally **not** a signal truth bin; treat as background if it can reconstruct inside measured bins, unless intentionally unfolding an outside truth distribution |

`SetInput()` should receive the measured reco vector only: 490 physical reco bins per charge, fake-subtracted or with backgrounds registered by `SubtractBackground()`. It should not include a miss bin, and it should not include a fake row/column as measured data.

### 4.2 Does TUnfold then output full truth-fiducial counts without external `1/eff`?

Yes, if the response histogram is filled in the native way. The missed-event underflow/overflow contributes to the column normalization `s_j`, so the probabilities `A_ij = M_ij/s_j` have column sums below one when there is inefficiency. In the least-squares equation `y ≈ A x`, `x_j` is the full truth count needed to produce the measured reco yield after efficiency loss. No external `x/eps` is needed for central value.

This is true with either `kEConstraintNone` or `kEConstraintArea`:

- With `kEConstraintNone`, the scale still comes from solving `A x ≈ y`; because `A` includes acceptance through column sums `<1`, `x` is on full truth scale.
- With `kEConstraintArea`, ROOT computes `epsilon_j = sum_i A_ij` over measured bins and enforces `sum_j epsilon_j x_j = sum_i y_i`, exactly matching Schmitt §2.2 eqs. 6-8. This constrains the detector-visible integral, not the full truth integral. It is consistent with missing events being represented in the response denominator.

### 4.3 Should one use `TUnfoldBinning` and `fAoutside`?

Use `TUnfoldBinning` for the multi-dimensional high-Q² SIDIS diagnostic, but do not try to manipulate `fAoutside` directly.

ROOT `TUnfoldDensity` documentation says complex multidimensional arrangements, underflow/overflow bins, and background bins are handled with `TUnfoldBinning`. `TUnfoldBinning` maps analysis bins onto one histogram axis, supports underflow/overflow per axis, and exposes `GetGlobalBinNumber()`, `CreateHistogramOfMigrations()`, and properly binned output/covariance histograms. It also enables `kDensityModeBinWidth` / `kDensityModeBinWidthAndUser` and axis-aware regularization steering.

`fAoutside` is a protected internal `TUnfoldSys` member described as:

```text
TUnfoldSys::fAoutside — Input: underflow/overflow bins.
```

It is the internal storage of outside-bin information, not a public API. The user-facing API is to fill the response histogram underflow/overflow bins and pass appropriate `TUnfoldBinning` objects to `TUnfoldDensity`.

For the z005 v6/native diagnostic, `TUnfoldBinning` should be used to define the 4D truth and reco structures and to avoid manual flat-index curvature. The key native miss test is still response-level: a generated fiducial track with no measured reco bin should fill detector-global bin 0 / reco underflow of the migration histogram for its truth bin.

## 5. Correct native treatment of reco-only fakes/backgrounds

### 5.1 Native preferred method

Use `TUnfoldDensity::SubtractBackground()` / `TUnfoldSys::SubtractBackground()` for reco-only fakes/backgrounds:

```cpp
TUnfoldDensity unfold(hM, TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeCurvature,
                      TUnfold::kEConstraintArea,   // or None for diagnostic AB
                      TUnfoldDensity::kDensityModeBinWidth,
                      truthBins, recoBins,
                      "signal", "*[UOB]");

unfold.SetInput(hDataRawOrPreBgSub, 0.0, 0.0, hVyy);  // if using explicit covariance
unfold.SubtractBackground(hFakeReco, "reco_only_fakes", 1.0, fakeScaleUnc);
unfold.DoUnfold(tau);
```

`SubtractBackground()` is especially valuable because it keeps both central value and covariance semantics inside ROOT/TUnfold. ROOT docs state it subtracts `scale*bgr` and adds:

- diagonal covariance from uncorrelated background-bin errors;
- fully correlated covariance from background normalization uncertainty.

These can later be inspected with `GetEmatrixSysBackgroundUncorr()` and `GetEmatrixSysBackgroundScale()`.

### 5.2 Is external fake subtraction acceptable?

External fake subtraction is acceptable only if it is statistically equivalent to Schmitt §5 eqs. 26-27 and ROOT `SubtractBackground()`:

1. The vector passed to `SetInput()` must be `y0 - b` in the 490 physical reco bins.
2. The covariance passed to `SetInput(hist_y, ..., hist_vyy)` must include:
   - original data statistical covariance;
   - uncorrelated fake/background shape uncertainties on the diagonal;
   - correlated fake/background normalization covariance across reco bins;
   - any covariance introduced by using MC-derived fake fractions to scale data.
3. The central-value and covariance provenance must be recorded.

The v5 fake-subtraction step uses a row fraction,

```python
data_fakesub_i = raw_i * sum_j B_ij / (sum_j B_ij + F_i)
```

but does not pass the full covariance impact of that MC-derived fake fraction into TUnfold. For diagnostic plots this is acceptable if labelled; for native TUnfold covariance semantics it is incomplete.

### 5.3 Should fakes ever be a truth/output column?

Not for the signal truth-fiducial result. ROOT explicitly warns that truth underflow/overflow bins are treated as part of the generator-level distribution and unfolded. Therefore filling a fake column on the truth axis makes TUnfold solve for a fake normalization as if it were a truth output parameter. This was the v4 failure pattern: fake/miss stored as ordinary bins, TUnfold fitted a positive miss row against zero data and treated fake column as unfolded truth, leading to O(100) negative bins.

Schmitt §5 allows extra `x` bins for background normalization only in special cases where auxiliary measurements constrain them. That is not the same as making reco-only fakes part of the published signal truth vector. For high-Q² charged-SIDIS z005, fakes should be background-subtracted/registered, not output.

## 6. Current v5 method vs recommended native method

### 6.1 What v5 does

`build_physical_response_fakesub_inputs_v5.py` declares the v5 convention in its docstring:

```python
B_ij = R[i,j] for i=1..490, j=1..245
F_i  = R[i,246]
data_fakesub_i = raw_i * sum_j B_ij / (sum_j B_ij + F_i)
M_j  = R[491,j] and eps_j = sum_i B_ij / (sum_i B_ij + M_j) for validation only
No miss data bin and no fake output bin are written as TUnfold inputs.
```

Implementation confirms:

```python
hB=ROOT.TH2D(..., NB_R, ..., NB_G, ...)
...
hB.SetBinContent(i,j,v)
...
miss.append(m)
eff.append(s/den if den>0 else float("nan"))
...
hfs=hist1(f"h_data_fakesub_{ch}_{mc}", ..., fakesub, fakesuberr)
```

`unfold_tunfold_single_tau_v5_physical_fakesub.C` then consumes only `hB` and `h_data_fakesub`:

```cpp
TH2D* hB=(TH2D*)fin->Get(Form("h_B_%s_%s",ch.c_str(),m.c_str()));
TH1D* hd=(TH1D*)fin->Get(Form("h_data_fakesub_%s_%s",ch.c_str(),m.c_str()));
...
TUnfoldDensity unfold(hB, TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeNone,
                      TUnfold::kEConstraintNone,
                      TUnfoldDensity::kDensityModeNone);
Int_t set_status=unfold.SetInput(hd);
unfold.DoUnfold(tau);
```

Therefore v5 excludes misses from the TUnfold solve. The miss vector and efficiency sidecars are used only later by Python comparison scripts.

### 6.2 Why `TUnfold_visible / eps` is not native TUnfold

The v5 corrected comparison computes:

```python
corr = u/eps if eps>0 and finite(eps) else float("nan")
correrr = ue/eps if eps>0 and finite(eps) else float("nan")
```

and labels the method:

```json
"compared_quantity": "(TUnfold tau=0 h_unfolded_245 visible yield / response epsilon) divided by central IBU yield_unfolded",
"efficiency_convention": "response-side eps_j = matched_j/(matched_j+miss_j) ...; no epsilon covariance propagation"
```

This is a hybrid two-stage procedure:

1. ROOT/TUnfold solves a visible/matched response problem using `B` only.
2. Python multiplies by `1/eps_j` after the fact.

For diagonal/no-migration toy cases, `x_visible/eps` can reproduce the same central value as native miss encoding. With migration and regularization, it is not algebraically identical in general:

- Native TUnfold solves with `A_ij = B_ij / (matched_j + miss_j)`, so the efficiency is inside the response matrix, the normal equations, the refolding residual, regularization interplay, and `D^{xy}` covariance propagation.
- v5 solves with `A'_ij = B_ij / matched_j` (effectively conditional on being reconstructed) and then scales the output afterward. Regularization acts on the visible-scale solution, not the truth-scale solution.
- Native `GetEmatrix*()` returns covariance for full-truth `x`. v5 `GetEmatrixTotal()` returns covariance for visible `x_visible`; Python then scales by `1/eps` but does not include uncertainty in `eps`, correlations in the efficiency estimates, or changes in regularization induced by putting efficiency inside `A`.

Thus post-hoc `1/eps` is a useful diagnostic cross-check, but it is not the intended native ROOT/TUnfold correction for misses.

### 6.3 Comparison table

| Aspect | Current v5 physical/fake-sub diagnostic | Recommended native ROOT/TUnfold diagnostic |
|---|---|---|
| Response shape | `490 x 245` physical matched block `B` only | TH2 with 490 physical reco bins x 245 physical truth bins **plus reco-axis under/overflow filled with misses** |
| Misses | `h_miss`, `h_eff` sidecars only | Fill reco underflow/overflow for each truth bin; ROOT includes in `s_j` normalization automatically |
| SetInput data | 490 fake-subtracted reco bins | 490 measured reco bins; no miss bin. Either raw data plus `SubtractBackground(fake)` or externally fake-subtracted data with full covariance |
| Fakes | External row-fraction subtraction; incomplete covariance | Prefer `SubtractBackground(hFake, name, scale, scale_error)`; or external subtraction with equivalent full covariance |
| Output interpretation | Visible/matched-scale TUnfold output; post-hoc `x/eps` for truth-scale comparison | `GetOutput()` is full truth-fiducial signal bins directly |
| Covariance | TUnfold covariance for visible output; post-hoc scaled diagonal/covariance; no epsilon covariance | TUnfold input-stat covariance on truth-scale output; background covariances included if using `SubtractBackground`; response MC stat/systematics via `GetEmatrixSys*` if configured |
| Regularization | `kRegModeNone` constructor + manual curvature calls; `kDensityModeNone`; flat index axes | `TUnfoldBinning` with physical 4D axes, `kRegModeCurvature` or controlled mixed mode, `kDensityModeBinWidth`/`BinWidthAndUser`, `regularisationAxisSteering="*[UOB]"` |
| Tau | Manual diagnostic scans and ad hoc selection; tau=0 comparison emphasized | Native `ScanLcurve()` and `ScanTau(kEScanTauRhoAvg/RhoMax)` as diagnostics; require interior minima and Poisson closure |
| Status | Diagnostic only; not promotable | New diagnostic path only; still not central unless it passes validation gates |

## 7. Concrete v6/native TUnfold diagnostic design requirements

This is a design specification for a **diagnostic** v6/native TUnfold path, not a promotion plan. Central z005 remains IBU/BBB finalcandidate.

### 7.1 Response histogram and bin placement

For each charge and MC response variant:

1. Build `TUnfoldBinning` truth tree for signal published truth bins:
   - node `signal` with physical axes corresponding to the z005 4D publication structure `(Q²/x cell, z, pT_h)` or a faithful equivalent that preserves the 245-bin order;
   - physical bins only for published truth; truth underflow/overflow should be zero unless intentionally unfolding a separate outside truth distribution.
2. Build `TUnfoldBinning` reco tree for measured reco bins:
   - 490 physical reco bins per charge or the existing reco-fine scheme mapped to publication output as appropriate;
   - include a detector-global outside/no-reco slot via histogram underflow/overflow for missed generated tracks.
3. Create the migration histogram with `TUnfoldBinning::CreateHistogramOfMigrations()` or an equivalent TH2 whose mapping is verified. With `kHistMapOutputVert`:
   - physical matched entries: `(x = reco_bin 1..490, y = truth_bin 1..245)`;
   - misses: `(x = 0 underflow or x = NB_R+1 overflow, y = truth_bin)`;
   - reco-only fakes: not filled on truth axis; accumulate a separate 490-bin `hFakeReco` histogram.
4. Response errors should be the MC statistical uncertainties on `M` entries. If using weighted events, `Sumw2`-equivalent errors must be filled consistently.

### 7.2 ROOT class/method choices

Recommended class: `TUnfoldDensity`, because it inherits `TUnfoldSys` and adds multidimensional binning and density-aware regularization.

Core constructor pattern:

```cpp
TUnfoldDensity unfold(hM,
                      TUnfold::kHistMapOutputVert,
                      TUnfold::kRegModeCurvature,
                      TUnfold::kEConstraintArea,      // also AB-test None
                      TUnfoldDensity::kDensityModeBinWidth,
                      truthBins,
                      recoBins,
                      "signal",
                      "*[UOB]");
```

Rationale:

- `TUnfoldDensity` is recommended by ROOT for most applications because it adds background subtraction, systematic uncertainty propagation, and multidimensional binning.
- `kDensityModeBinWidth` or `kDensityModeBinWidthAndUser` is needed because z005 physical bins are nonuniform in z and pT_h.
- `regularisationAxisSteering="*[UOB]"` excludes underflow, overflow, and boundary bins from regularization; this avoids smoothing across artificial boundaries.
- `kEConstraintArea` is paper-recommended for Poisson counting data, but `kEConstraintNone` should be AB-tested because prior high-Q² diagnostics had area-constraint pathologies. The miss handling itself does not require `Area`; it requires under/overflow in the response denominator.

### 7.3 Input histogram and covariance

`SetInput()` should get only measured reco bins:

```cpp
Int_t status = unfold.SetInput(hDataReco, 0.0, 0.0, hVyy);
if (status != 0) fail diagnostic gate;
```

Input requirements:

- `hDataReco` length: 490 physical reco bins per charge (or the reco-binning length if a finer reco scheme is used). No miss bin and no fake truth column.
- Bin contents: measured data counts in physical reco bins.
- Bin errors / `hVyy`: data statistical covariance. If using external fake subtraction, include background/fake covariance terms. If using `SubtractBackground`, leave data raw and register background separately.
- Check `SetInput()` return code. Several prior diagnostics tolerated non-OK `SetInput`; that should be a failure gate.

### 7.4 Background/fake handling

Preferred:

```cpp
unfold.SubtractBackground(hFakeReco, "reco_only_fake", 1.0, fakeScaleUnc);
```

Where:

- `hFakeReco` is 490 bins, same reco binning as data.
- `hFakeReco` bin errors represent MC-stat or shape uncertainty on the fake estimate.
- `fakeScaleUnc` encodes correlated normalization uncertainty. If no credible value exists, use `0` for a diagnostic but label covariance incomplete.

If external subtraction is used instead, provide `hVyy` with the equivalent covariance of Schmitt §5 eq. 27. A central-value row-fraction subtraction without covariance is not native-covariance-complete.

### 7.5 Output retrieval

After `DoUnfold(tau)` or native tau scan:

```cpp
TH1* hTruth = unfold.GetOutput("h_unfolded_truth_signal", "signal", "*[UO]");
TH1* hRefold = unfold.GetFoldedOutput("h_refold_reco", nullptr, nullptr, true, false);
TH2* hEmatInput = unfold.GetEmatrixInput("h_cov_input", nullptr, "signal");
TH2* hEmatTotal = unfold.GetEmatrixTotal("h_cov_total", nullptr, "signal");
TH1* hRhoI = unfold.GetRhoI("h_rhoi", nullptr, "signal");
```

Exact overload signatures vary by ROOT version; the diagnostic should use the `TUnfoldDensity` getter overloads where possible so `TUnfoldBinning` projection names and axis steering are respected. At minimum retrieve `GetOutput`, `GetFoldedOutput`, `GetEmatrixInput`/`GetEmatrixTotal`, and either `GetRhoI`/`GetRhoItotal` or `GetRhoIJ`/`GetRhoIJtotal`.

### 7.6 Validation gates

A v6/native diagnostic should fail closed unless all of these pass:

1. **Geometry:** response has 490 physical reco bins, 245 physical truth bins per charge; reco under/overflow miss totals equal the prior miss vector; truth under/overflow zero unless deliberately unfolded.
2. **No fake truth column:** truth/output dimension is 245 signal bins only.
3. **SetInput:** `SetInput()` return code is `0`.
4. **Native miss AB:** in a diagonal toy with known `eff_j`, native output equals full truth without post-hoc division; removing the miss underflow makes output drop by `eff_j`.
5. **Data refold:** `GetFoldedOutput()` agrees with data/background-subtracted reco within a documented χ²/ndf tolerance; gross refold failures are blockers.
6. **Non-negativity/pathology:** negative truth bins counted and localized; persistent O(100) negative bins remain fatal.
7. **Tau selection:** `ScanLcurve()` and `ScanTau()` must have interior, well-defined choices; boundary minima are unsafe.
8. **Poisson closure:** run Poisson-fluctuated pseudo-experiments, not deterministic `R·truth` only. Deterministic closure remains arithmetic smoke, not validation.
9. **Covariance sanity:** covariance positive semidefinite within numerical tolerance; diagonal nonnegative; `rho_i` not saturated near 1 across broad regions.
10. **Model closure:** RAPGAP response on DJANGOH pseudo-data and vice versa; compare central value and coverage.
11. **Comparison to IBU/BBB:** native TUnfold truth output should compare directly to IBU `yield_unfolded`, without `1/eps` post-scaling.

### 7.7 Minimal AB tests to prove native miss handling

1. **Single-bin analytic toy:** one truth bin, one reco bin, `B=60`, `miss=40`, data `y=60`. Native response with miss in reco underflow should return `x≈100` at tau=0/none. Response without miss should return `x≈60`. This tests ROOT semantics directly.
2. **Diagonal 245-bin toy:** for each bin `j`, set `B_j = eps_j * truth_j`, miss `(1-eps_j)*truth_j`, no migration/fakes. Native TUnfold should recover `truth_j` without post-hoc division.
3. **Migration toy:** two truth bins and two reco bins with off-diagonal migrations plus misses. Compare native TUnfold to closed-form least-squares using `A_ij = B_ij/(sum_i B_ij + miss_j)`. Compare v5 visible-then-divide to show the covariance/regularization difference.
4. **Fake toy:** add a reco-only fake vector. With `SubtractBackground`, output should match no-fake truth and `GetEmatrixSysBackground*` should be nonzero when background errors/scale uncertainties are nonzero.
5. **z005 response AB:** build two read-only diagnostic inputs from the same v5 source response: (a) v5 visible-only `B`, (b) native miss-underflow `B+miss`. The native output should move from the current visible scale to the IBU truth scale without Python `1/eps`.

## 8. Risk/blocker ranking

### FATAL

1. **Current v5 is not native truth-fiducial TUnfold.** It solves a visible/matched problem and then optionally applies `1/eps` in Python.
2. **Misses must not be fitted as ordinary measured zero rows.** That was the v4 failure mode and contradicts ROOT/Schmitt semantics. Misses belong in reco underflow/overflow of the response histogram.
3. **Fakes must not be published signal truth/output columns.** They are backgrounds unless deliberately constrained as nuisance/background normalization bins with auxiliary measurements.
4. **Post-hoc `1/eps` lacks native covariance semantics.** It omits epsilon MC-stat/model covariance and changes the regularization/refolding algebra relative to native miss encoding.

### MAJOR

1. **External fake subtraction covariance is incomplete in v5.** Central values may be adequate for diagnostics, but native covariance needs `SubtractBackground()` or an equivalent `V_yy`.
2. **Manual flat-index regularization and `kDensityModeNone` are poor fits to nonuniform 4D bins.** Use `TUnfoldBinning` and density mode for any revived TUnfold diagnostic.
3. **Tau selection remains unsafe unless native scans have interior minima and Poisson closure passes.** Deterministic closure alone is insufficient.
4. **Response MC systematics remain separate.** Native TUnfold can provide input-stat/background/response-stat covariance, but generator/model and detector shifts still require `AddSysError()` or ensemble/systematic propagation.

### MINOR

1. **Python correlation post-processing is not a fake unfold.** It uses `np.linalg` only to compute `rho_i` from stored covariance when `GetRhoI` was not saved; still, future macros should store ROOT `GetRhoI`/`GetRhoItotal` directly.
2. **`TUnfoldBinning` exact no-reco fill convention needs a small AB test.** ROOT simple TH2 semantics are clear; the `TUnfoldBinning::CreateHistogramOfMigrations()` global-bin-0/no-bin mapping should be smoke-tested before broad use.

## 9. Open questions and gaps

- I did not run any ROOT macro or inspect binary ROOT object contents beyond script code and manifest/sidecar evidence, per read-only/no-production instruction.
- I did not validate the exact `TUnfoldDensity` getter overload signatures available in the deployed LCG_109a ROOT 6.38.04 beyond v5 script usage; implementers should compile a small smoke macro before committing to method names.
- The exact choice of `kEConstraintArea` vs `kEConstraintNone` for high-Q² z005 remains a diagnostic AB question. Native miss handling does not depend on this choice, but final normalization/covariance behavior does.
- A full v6/native diagnostic still needs schema validation and response-build provenance on current v6 trees; this note only specifies the TUnfold response/input semantics.

## 10. Bottom line

ROOT/TUnfold already has native detector-inefficiency handling: put missed generated events into the reconstructed-axis underflow/overflow bins of the response histogram. Do **not** add a measured miss row to `SetInput()`, and do **not** fit it against zero data. With the native response, `GetOutput()` is the full truth-fiducial signal vector; external `1/efficiency` is unnecessary for the central value and changes covariance/regularization semantics.

For high-Q² charged-SIDIS z005, current v5 fixed the v4 fake/miss ordinary-bin catastrophe by removing fake/miss from the TUnfold problem, but that made the raw TUnfold output visible-only. The later `x_visible/eps_response` comparison explains why TUnfold/IBU moved from median `~0.59` to `~0.99`, but it is a hybrid diagnostic, not native ROOT/TUnfold truth-fiducial unfolding.

The package does use real ROOT/TUnfold (`TUnfoldDensity`, `SetInput`, `DoUnfold`, `GetOutput`, `GetEmatrixTotal`, `GetRhoIJtotal`) for actual unfolding. Python scripts in v5 are input builders, wrappers, comparison/plotting scripts, or covariance/rho post-processors; no inspected Python code replaces the unfolding with a fake clone or pseudo-inverse solver.

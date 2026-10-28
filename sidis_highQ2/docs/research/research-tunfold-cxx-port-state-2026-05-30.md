# Research: state of the official ROOT TUnfold C++ port for high-Q² SIDIS

**Date:** 2026-05-30
**Author:** researcher (read-only)
**Scope:** Read the 5 official-TUnfold scripts in
`H1/sidis_highQ2/scripts/unfolding/`, the existing Python-TUnfold path,
the 4D response builder and shared utilities, and Britzger's working
`g1j_ES` TUnfold cxx implementations on NAF. Synthesize the current
state of the C++ port, identify what "Option 2" likely means given the
absence of an in-repo enumeration, document Britzger's overflow-bin
convention, and propose a concrete finalization plan.

**Status:** The user said "use option 2". No repo file enumerates
"Option 1/2/3" for the TUnfold C++ port. Britzger's reference code
exposes exactly one natural set of three alternatives for handling
acceptance / fakes / migrations-out — Option 2 most likely refers to
the TUnfold-native overflow-bin construction
(`Migration_Matrix_moreRec + Migration_Matrix_moreRec_EFF`,
`kHistMapOutputVert`). See §2 for the inference and §7 for the open
question to the supervisor.

---

## 1. Current state of the C++ port

The official-TUnfold pipeline draft consists of five files in
`/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/unfolding/`.
File-by-file status (all are LOCAL drafts; nothing in this set has run
end-to-end on NAF as of 2026-05-30 — the canonical TUnfold result still
comes from the Python pipeline `unfold_TUnfold_4d.py`).

### 1a. `unfold_tunfold_official_v3final.C` — STATUS: **near-complete, plain-TH2 path only (no overflow / no acceptance bin)**

383 lines. Compiles and runs (per the prep step) as an ACLiC macro.
What it does:

- Lines 86–99: signature
  `unfold_tunfold_official_v3final(in_root, out_root, out_json, tauScanMin=1e-7, tauScanMax=1.0, nScan=100, tauOverride=-1.0, tauChoiceName="Lcurve")`.
- Lines 101–114: reads `h_R` (TH2D), `h_data_bgsub` (TH1D),
  `h_data_bgsub_w2` (TH1D), `h_prior_gen` (TH1D),
  `h_reco_marginal_MC` (TH1D, optional) from the prep ROOT file.
  Detaches them from the input file.
- Lines 124–128: constructs
  ```
  TUnfoldDensity unfold(h_R,
                        TUnfold::kHistMapOutputHoriz,
                        TUnfold::kRegModeCurvature,
                        TUnfold::kEConstraintArea,
                        TUnfoldDensity::kDensityModeBinWidth);
  ```
  — note `kHistMapOutputHoriz` (gen on x-axis), which is the *opposite*
  of Britzger's `kHistMapOutputVert`.
- Line 130: `unfold.SetInput(h_data)`.
- Lines 138–145: `ScanLcurve(nScan, tauScanMin, tauScanMax, ...)` →
  `tau_L`.
- Lines 161–168: `ScanTau(nScan, tauScanMin, tauScanMax,
  &splineScanTau, TUnfoldDensity::kEScanTauRhoAvg, nullptr, nullptr,
  &gLcurveAux, ...)` → `tau_chi2`.
- Lines 196–208: canonical τ choice. Default = `ScanTau` rho-avg
  minimum; `tauOverride` switches to a user-specified τ with a label.
- Lines 211–249: `DoUnfold(tau_opt)`; extract `GetOutput`,
  `GetEmatrixTotal`, `GetFoldedOutput`; clone into named TH1D/TH2D.
- Lines 251–265: refold χ²/ndf vs `h_data_bgsub` (using `h_data_w2`
  as variance with a Poisson-floor).
- Lines 274–315: τ-scan systematic — re-DoUnfold at τ×2 and τ/2, store
  per-bin `sigma_reg = max(|n_τ*2 − n_τ|, |n_τ/2 − n_τ|)`.
- Lines 318–328: `TNamed` scalars for tau values, χ², ndf, TUnfold
  version.
- Lines 333–372: writes a JSON sidecar with arrays and scalars.

**What is missing / risky:**

- **No TUnfoldBinning object.** The macro uses the simple TH2/TH1
  constructor with index-binning `[0, 480]`. It does not use
  `TUnfoldBinning::AddAxis` or `AddBinning`; there is no
  density-mode awareness beyond the constructor flag.
- **No overflow / acceptance-bin treatment.** `h_R` is a plain 480×480
  matrix of matched-events only (see §4). There is no gen-axis
  overflow row for "out-of-acceptance reco" (miss) and no reco-axis
  overflow column for "out-of-fiducial gen" (fake). With
  `kEConstraintArea`, TUnfold normalises the unfolded total to the
  data total, so an unfolded yield ≠ true cross-section by an
  acceptance factor that must be applied outside.
- **No `AddSysError`/`GetDeltaSys`.** Detector systematics must be
  added via shifted-response matrices for TUnfold's covariance to
  include them. The current macro does τ-variation only.
- **Orientation mismatch with Britzger.** Britzger uses
  `kHistMapOutputVert` (reco on x, gen on y). The macro uses
  `kHistMapOutputHoriz` (gen on x, reco on y). The prep step
  (`prep_tunfold_inputs.py` line 13–14, 76, 82–83) is internally
  consistent with `kHistMapOutputHoriz`, so this is not a bug today,
  just a convention difference.
- The plotting block at `plot_tunfold_official_diag.py` lines 51–60
  has a placeholder `ax.axhline(np.nan)` — harmless, just a stylistic
  leftover.

### 1b. `prep_tunfold_inputs.py` — STATUS: **complete for the current plain-TH2 path; does NOT emit overflow bins**

137 lines. Converts the 4D-builder NPZ + bg-subtracted data JSON into a
single ROOT file with five histograms.

- Lines 32–37: args `--response-npz`, `--data-json`, `--out-root`.
- Lines 43–52: loads NPZ keys `response`, `response_w2`, `prior_gen`,
  `prior_gen_w2`, `matched_reco_marginal`, `matched_reco_marginal_w2`.
  Asserts `R.shape == (480, 480)`.
- Lines 75–83: builds `h_R` as a 480×480 TH2D with index-axes
  `[0, 480]` on each. The orientation comment at lines 13–14 is
  explicit:
  > `h_R.SetBinContent(j+1, i+1, R[i,j])      # x=gen bin (j), y=reco bin (i)`
- Lines 85–99: builds `h_data_bgsub` (reco, 480-bin) and
  `h_data_bgsub_w2`. Floors zero-variance non-zero-content bins with
  `sqrt(|n|+1)` (Poisson floor).
- Lines 101–111: builds `h_reco_marginal_MC` (== R.sum(axis=1)) and
  `h_prior_gen` (gen pool, full prior — NOT R.sum(axis=0)).
- Lines 114–127: emits metadata `TNamed`s including sums and
  orientation note.

**What is missing:**

- No overflow bin on either axis. The `h_R` produced here does not
  include unmatched-reco events (miss) on the gen axis or unmatched-gen
  events (fake) on the reco axis.
- Does not load or use `reco_marginal` (== matched-hadron reco
  including events with no valid gen match). `build_response_4d.py`
  fills `reco_marginal` (line 464–465) but `matched_reco_marginal`
  here happens to coincide with `reco_marginal` because the
  `both_valid` filter is applied at fill time (see §4).
- No data overflow bin column to feed `unfold.SetInput` for the
  optional "out-of-fiducial" data slot.

### 1c. `postprocess_tunfold_official.py` — STATUS: **complete for the plain-TH2 outputs**

420 lines. Reads the ROOT output, builds per-cell yields, compares
against the Python-TUnfold canonical, optionally compares vs IBU, and
emits canonical JSON + covariance NPZ + sigma_reg JSON.

- Lines 78–98: reads `h_unfolded_TUnf`, `h_emat_total_TUnf`,
  `h_refold_TUnf`, `h_unfolded_tau2x_TUnf`, `h_unfolded_tauhalf_TUnf`,
  `h_data_input`.
- Lines 94–102: pulls the 480×480 covariance; sanity-checks
  `sqrt(diag(cov))` vs `GetBinError`.
- Lines 142–164: trust mask loader (schema `v5g_4d_trust_mask_v3p2`).
- Lines 167–173: loads canonical Python-TUnfold for cross-check.
- Lines 187–212: per-cell + per-stratum totals (CELL_LABELS S0a..S3b).
- Lines 215–248: negative-bin count, IBU cross-check.
- Lines 290–374: builds main output JSON; includes a
  `tau_internal_convention_equivalent` block (lines 304–319) that
  records the Python-side `tau_native * sqrt(N_eff)` conversion.
  Note: the comparison block at lines 351–367 already documents that
  the area-constraint TUnfold totals will *not* match the Python-TUnfold
  total because the Python version solves on `P=R/prior` (gen-pool
  scale) while the new C++ version is area-normalised to the
  bg-subtracted data total.

**What is missing:**

- No handling of an overflow / acceptance bin if one were added: the
  postprocess currently asserts `NB = 480` throughout and indexes from
  0..479. If the gen axis is extended to 481 bins (one "out-of-fiducial"
  overflow) the unfolded vector must be sliced to discard the overflow
  before per-cell yield aggregation, and the covariance restricted.
- No `GetDeltaSys` parsing.

### 1d. `run_tunfold_official_v3final.sh` — STATUS: **complete driver for the current path**

126 lines. End-to-end driver:

- Lines 20–22: sources LCG_109a setup (ROOT 6.38.04, TUnfold V17.9).
- Lines 28–62: input/output path layout. Inputs:
  `output/response_4d_v3final_FINAL_bgsubtracted.npz`,
  `output/data_subtracted_v3final_REAL_LUMI_filtered.json`,
  `data/v5g_4d_trust_mask_v3p2.json`,
  `output/unfolded_v3final_FINAL_bgsubtracted_TUnfold.json` (Python
  canonical for comparison),
  `output/unfolded_4d_v3final_IBU.json`.
- Lines 78–82: step 1 = prep.
- Lines 84–86: step 2a = canonical run (ScanTau policy).
- Lines 88–92: step 2b = audit L-curve run with explicit
  `tauOverride = $TAU_L`.
- Lines 94–105: step 3a = postprocess canonical.
- Lines 107–118: step 3b = postprocess L-curve audit.
- Lines 120–123: step 4 = diagnostic plots.

**What is missing:** nothing for the current plain-TH2 path. If
overflow bins are added the prep step needs new flags and the macro
needs the new histogram names threaded through.

### 1e. `plot_tunfold_official_diag.py` — STATUS: **complete**

153 lines. Produces `Lcurve.pdf`, `tau_scan.pdf`, `residuals.pdf`,
`tau_variants.pdf` from the C++ macro ROOT output. The Lcurve plot has
a harmless placeholder `ax.axhline(np.nan)` at line 53.

### 1f. End-to-end status

**What runs end-to-end today (in principle):** the five scripts form a
closed loop that solves the area-constrained TUnfold problem on the
480×480 matched-events response, with the bg-subtracted data on the
reco side. The result will be τ-optimised by `ScanTau` and normalised
to the bg-subtracted data total (≈ 2.9 M events; *not* to the
gen-pool-scale ≈ 5.2 M of the Python canonical).

**What does not run:** there is no acceptance correction at the
TUnfold level. Events that have no reco partner (miss) and events
with no gen partner (fake) are simply absent from `h_R`. The Python
canonical handles this by solving with `P = R / prior_gen` (probability
operator built from `build_probability_operator` in
`unfold_TUnfold_4d.py` lines 53–58) so the unfolded total lives on the
gen-pool scale — i.e., implicit acceptance correction. The C++ macro
does not have that correction; the result must be post-multiplied by a
per-bin acceptance factor, OR the response and data must be extended to
include an overflow / acceptance bin (the Britzger pattern, see §3).

There is no evidence in the repo that this pipeline has ever produced a
nominal NAF output. Search of NAF shows only the Python canonical at
`output/unfolded_v3final_FINAL_bgsubtracted_TUnfold.json` (verified
to exist, 26 346 bytes, 2026-05-29).

---

## 2. "Option 2" identification

### 2a. Repository search — NO enumeration found

I grepped `H1/sidis_highQ2/` for `Option 1`, `Option 2`, `Option 3`,
`Option A/B/C`, and matching lowercase variants in `*.md`, `*.py`,
`*.C`, `*.sh` files (and in `context-handoff.md`). Results in scope:

- `docs/methodology/gen_fiducial_acceptance_2026-05-24.md:368` —
  "hybrid option 1" (about gen fiducial, unrelated).
- `docs/research/research-electron-systematics-2026-05-25.md` — Option
  A vs Option B (analytic vs framework-level electron systematic,
  unrelated).
- `docs/research/research-xF-cut-critic-verdict-2026-05-27.md` — Option
  A/B/C for x_F > 0 cut (unrelated).
- `docs/research/unfolding_systematic_decomposition_2026-05-24.md:233`
  — "Option C (single method central, others as cross-checks)" — refers
  to unfolding-method spread (BBB/IBU/TUnfold), not to TUnfold C++ port.
- `context-handoff.md` — "Option C fiducial" is the x_F > 0 gen
  fiducial; not the TUnfold C++ port.
- `quarantine/docs/methodology/TUnfold_uniform_low_diagnosis_2026-05-24.md`
  — Option A/B/C for the L-curve corner-finder bug. Already resolved
  by raising τ to 0.10 and using `ScanTau` rho-avg. Not the C++ port.

There is **no enumeration** in code or docs that names "Option 1/2/3"
for the TUnfold C++ port. The five-script drafts in
`scripts/unfolding/` make no reference to options.

### 2b. Inference — what "Option 2" most likely means

Given the user's hint ("use option 2" with overflow-bin acceptance
comment), the natural three-way menu — derivable from Britzger's
reference code and from the standard TUnfold V17 idioms — is:

- **Option 1 — Plain-TH2 + external acceptance (current draft).**
  Build a 480×480 matched-events matrix. Use `kEConstraintArea` to
  normalise to data total. Apply an external per-bin acceptance
  correction `ε = matched_gen / prior_gen` AFTER unfolding to recover
  the gen-pool scale. This is what `unfold_tunfold_official_v3final.C`
  currently does. Pros: tiny matrix, simple. Cons: factorises
  acceptance and migration, loses cross-bin acceptance covariance.
- **Option 2 — TUnfold-native overflow-bin acceptance (Britzger
  pattern).** Extend the gen axis with ONE extra overflow bin
  representing "out-of-acceptance reco" / efficiency loss, and fill
  it from MC truth (events with valid reco but no valid gen, or, in
  Britzger's `_EFF`, gen-pool events with no reco partner). Optionally
  extend the reco axis with one overflow column for fakes. Construct
  TUnfold on the extended (481×481 or 480×481) matrix; data has zero
  in the overflow reco bin. After unfolding, discard the overflow gen
  bin and report only the 480 physical bins. Pros: full migration ↔
  acceptance covariance, matches Britzger and standard TUnfold
  practice. Cons: requires extending `build_response_4d.py` OR
  `prep_tunfold_inputs.py` to fill the overflow column/row from MC.
  **THIS IS THE MOST LIKELY INTENT** given the user's "overflow-bin
  acceptance" framing.
- **Option 3 — Solve on the probability operator (Python current).**
  Pre-divide `R` by `prior_gen` to form `P` and use TUnfold without
  area constraint (`kEConstraintNone`). Output is directly in the
  gen-pool scale. Pros: keeps the Python convention. Cons: needs
  `prior_gen` to be strictly positive for every populated bin; bins
  with `prior_gen = 0` (zero-acceptance) must be masked; loses the
  ability to use TUnfold's built-in acceptance handling.

**Recommended interpretation:** Option 2 = extend the response with
overflow bins, fill them from MC truth events that have no reco
partner (gen-only / "miss") on the gen-axis overflow row, and
optionally fakes on the reco-axis overflow column; then run
`TUnfoldDensity` with `kHistMapOutputVert` (or keep `Horiz` — the prep
script is internally consistent either way), `kRegModeCurvature`,
`kEConstraintArea`. After unfolding, drop the overflow bins before
postprocessing.

The user must confirm in §7.

---

## 3. Britzger reference pattern (concrete code excerpts)

Source: `naf-hone:/data/dust/group/h1/klesthen/g1j_ES/Analysis/`. Two
relevant tree levels:

- **Upstream H1OO filler:** `Analysis/AnalysisEventShapes.C` builds
  the `Migration_Matrix_*` TH2s from per-event matched/unmatched
  evaluations.
- **Downstream TUnfold solver:** `Analysis/RooUnfold/{NEWdec23_,my_,
  my_unfold2,SysUncertainty}.cxx` reads the pre-built TH2s and runs
  TUnfold.

There is NO `TUnfoldBinning` / `AddAxis` / `AddBinning` /
`SetUnderflow` / `kBinningUnderflow` use anywhere in the Britzger
RooUnfold code I inspected. The migration matrices are plain TH2s,
just with one extra "overflow" bin appended to the reco axis. So
"overflow" in Britzger is a literal extra physical bin in the TH2
binning, NOT the ROOT-level under/overflow bins (index 0 and N+1).

### 3a. Migration matrix construction (H1OO upstream)

`Analysis/AnalysisEventShapes.C` defines binning vectors
`q2bins`, `q2bins_more`, etc., where `..._more` has the original bins
+ one extra "overflow" bin appended. The four matrices per observable
are filled as follows (Q² example, lines 1217–1236):

```cpp
// "EFF": gen-only (no reco) — fills the LAST RECO BIN of moreRec
hmQ.Get<TH2D>("Migration_Matrix_moreRec_EFF", ...,  q2bins_more,q2bins)
   ->Fill(q2bins[9]+1, TMath::Log10(fGen.Q2), fGen.wgt);

// nominal: matched gen+reco — fills (RecQ2, GenQ2) cell
hmQ.Get<TH2D>("Migration_Matrix_moreRec", ...,  q2bins_more,q2bins)
   ->Fill(TMath::Log10(fRec.Q2), TMath::Log10(fGen.Q2), fRec.wgt);

// "BK": reco-only (no gen) — fills the LAST GEN BIN of moreRec
hmQ.Get<TH2D>("Migration_Matrix_moreRec_BK", ...,  q2bins_more,q2bins)
   ->Fill(TMath::Log10(fRec.Q2), 1000000, fRec.wgt);

// "OF": gen+reco both in PS but reco weight ≠ gen weight (rad. corr.)
hmQ.Get<TH2D>("Migration_Matrix_moreRec_OF", ...,  q2bins_more,q2bins)
   ->Fill(q2bins[9]+1, TMath::Log10(fGen.Q2), fGen.wgt-fRec.wgt);
```

Reading: the axis order in `Get<TH2D>` is (xbins, ybins) =
(`q2bins_more`, `q2bins`), so the TH2 has **reco on x with one extra
overflow bin** and **gen on y at the physical binning**. The "EFF"
matrix has its weight placed at `xbin = q2bins[9]+1` (the appended
overflow bin) — i.e., gen-only events with no reco are sent to the reco
overflow column. The "BK" matrix sends reco-only events to a synthetic
gen-overflow at `1000000` (which gets `TH2::Fill` overflow handling,
because the y-axis lacks an extra bin). The "OF" matrix corrects
within-PS weight mismatches.

### 3b. Combining matrices before TUnfold

`RooUnfold/NEWdec23my_sys_unfold.cxx` line 948–980 (nominal path):

```cpp
matrixRaQ2->Add(OFmatrixRaQ2);
matrixRaGIM05->Add(OFmatrixRaGIM05);
// ... etc for every observable ...
matrixDjQ2->Add(OFmatrixDjQ2);
matrixDjGIM05->Add(OFmatrixDjGIM05);
```

So **nominal matrix = `moreRec + OF`** (no EFF, no BK).

For systematics (`RooUnfold/NEWdec23my_sys_unfold.cxx` lines 604–622,
which is the sys-variation TUnfold path):

```cpp
TH2D* MigmatrixSys    = GetHistogram<TH2D>("merged.root",
                          Form("%s/Acceptance%s_-9999/Migration_Matrix_moreRec", ...));
TH2D* EFFMigmatrixSys = GetHistogram<TH2D>("merged.root",
                          Form("%s/Acceptance%s_-9999/Migration_Matrix_moreRec_EFF", ...));
MigmatrixSys->Add(EFFMigmatrixSys);    // <-- ADD EFF for sys path
...
MyTUnfoldDensity SysUnfold(MigmatrixSys, TUnfold::kHistMapOutputVert);
```

And `my_unfold.cxx` lines 147–153 confirms a *combined* recipe:

```cpp
TH2* MigMa_Sys     = GetHistogram<TH2D>("merged.root", ".../Migration_Matrix_moreRec");
TH2* MigMa_Sys_OF  = GetHistogram<TH2D>("merged.root", ".../Migration_Matrix_moreRec_OF");
TH2* MigMa_Sys_BK  = GetHistogram<TH2D>("merged.root", ".../Migration_Matrix_moreRec_BK");
MigMa_Sys->Add(MigMa_Sys_OF);
// (BK is loaded but NOT added in the AddSysError path)
unfoldsys.AddSysError(MigMa_Sys, Form("%d",isys[i]),
                      TUnfoldSys::kHistMapOutputVert,
                      TUnfoldSys::kSysErrModeMatrix);
```

So Britzger's canonical recipe is: **matched + OF** for nominal,
**matched + OF + EFF** for the sys-replica matrix passed to
`AddSysError`. Fakes (`_BK`) are NOT added into the matrix; they are
handled outside (by background subtraction from data, or absent from
this analysis).

The extended reco-axis structure means TUnfold sees a "more reco than
gen" rectangular matrix. The last reco bin is the "out-of-PS at reco"
bin and is filled with two contributions:

1. `moreRec` non-EFF: zero (no matched events have reco outside PS by
   construction).
2. `EFF`: gen events in PS with no reco partner (efficiency loss).

This is the canonical TUnfold acceptance-bin recipe: the gen-side
"missing reco" weight lives at the reco overflow column, which TUnfold
treats as a regular column whose corresponding *data* entry is zero —
so the unfolding has to attribute that gen weight to acceptance loss
rather than to a real data bin.

### 3c. TUnfold class + flags

`Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` line 144:

```cpp
MyTUnfoldDensity unfold(matrix, TUnfold::kHistMapOutputVert, regmode, constraint);
```

`MyTUnfoldDensity` (line 31) is a trivial `using TUnfoldDensity::TUnfoldDensity;`
subclass — same API. The construction args:

- Histogram map: `TUnfold::kHistMapOutputVert` — reco on x (input
  binning), gen on y (output binning). **Opposite of our current
  macro's `kHistMapOutputHoriz`.**
- Reg mode: `regmode`, set at line 1503 to
  `TUnfold::kRegModeCurvature`. Earlier diagnostic calls (lines
  2607–2619) use `kRegModeSize`; the production sweep (lines
  2715–2728) uses `kRegModeCurvature`. **Matches our macro.**
- Constraint: `TUnfold::kEConstraintArea` for the production sweep
  (lines 2715+); `kEConstraintNone` for the diagnostic sweep.
  **Matches our macro.**
- Density mode: NOT passed to the constructor (defaults to
  `kDensityModeNone` in TUnfoldDensity, which means no width
  rescaling). **Differs from our macro** which uses
  `kDensityModeBinWidth`. For uniform-width 1-bin-per-index axes (as in
  both Britzger and ours) the choice is mathematically equivalent.
- For the systematics-only path, `TUnfoldSys` is used instead of
  `TUnfoldDensity` (`SysUncertainty.cxx` lines 238–239):
  ```cpp
  TUnfoldSys unfoldRaDj(DjangoMigMa_moreRec,
                        TUnfoldSys::kHistMapOutputVert,
                        TUnfoldSys::kRegModeSize,
                        TUnfoldSys::kEConstraintArea);
  ```

### 3d. τ selection

`Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` line 179:

```cpp
int iBest = unfold.ScanTau(nScan, 0., 0., &scanResult, 0,
                           nullptr, nullptr,
                           &lCurve, &logTauX, &logTauY);
```

With `nScan = 30` (line 162), `tauMin = tauMax = 0` (let TUnfold pick
its own range), `mode = 0` which is the default `kEScanTauRhoAvg`.
There is a commented-out alternative (`kEScanTauRhoMax`) at line 168.

When `tau != 0` is passed in (production path, lines 2715+), the
explicit τ is used:
```cpp
unfold.DoUnfold(tau, data, scaleBias);
```

So Britzger's canonical recipe is **ScanTau + accept the result**, with
production runs pinning τ to a hand-chosen value from a pre-scan.
**Matches our macro.**

### 3e. Output extraction (central value, covariance, systematics)

`Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` lines 291, 537:

```cpp
TH1* output = unfold.GetOutput("Unfolding Output");
// ...
TH1* rho_ij = unfold.GetRhoIJtotal("rho_ij");
```

`SysUncertainty.cxx` lines 269–271:

```cpp
unfoldRaDj.GetOutput(histUnfoldRaDj, taubinmap);  // overwrite histogram
unfoldDjRa.GetOutput(histUnfoldDjRa, taubinmap);
```

The `binmap` argument is a remap from the full TUnfold output
(including overflow) to the published physical bins, so the overflow
bin is discarded by passing `-1` in slot 0 (taubinmap[0] = -1) and slot
N-1 (taubinmap[13] = -1). **This is the canonical way to drop the
overflow output bins.**

Systematic shifts (`SysUncertainty.cxx` lines 245–246, 256–263):

```cpp
unfoldRaDj.AddSysError(DjMigMa_moreRec_sys, Form("%d", isys[i]),
                       TUnfoldSys::kHistMapOutputVert,
                       TUnfoldSys::kSysErrModeMatrix);
// ...
unfoldRaDj.DoUnfold();
// later: GetDeltaSys(name, ...) — extract per-sys shift histogram
```

`GetEmatrixTotal` and `GetEmatrix` are NOT used explicitly in the
files I read (they read via `GetRhoIJtotal` only). The total
covariance comes through the `GetOutput` → bin errors and through
`GetRhoIJtotal` for correlations. **Our macro's use of
`GetEmatrixTotal` is correct and slightly more useful for downstream
covariance propagation.**

---

## 4. Required overflow-bin convention for our 4D response

### 4a. What `build_response_4d.py` produces today

`H1/sidis_highQ2/scripts/unfolding/build_response_4d.py` lines 320–353
defines `accumulate_response_and_marginal`:

```python
for (i_reco, w_reco, i_r_m, i_g_m, w_m) in stream_mc_reco_csv(...):
    # reco_marginal: all matched-hadron reco in range
    np.add.at(reco,  i_reco, w_reco)
    np.add.at(reco2, i_reco, w_reco * w_reco)
    # response R + matched_reco_marginal: restrict to valid gen too
    np.add.at(R,  (i_r_m, i_g_m), w_m)
    np.add.at(R2, (i_r_m, i_g_m), w_m * w_m)
    np.add.at(mreco,  i_r_m, w_m)
    np.add.at(mreco2, i_r_m, w_m * w_m)
```

And the upstream `stream_mc_reco_csv` (lines 228–279), particularly
lines 261–272:

```python
i_flat_r = flat480(i_chg_r, i_cell_r, i_zp_r)   # 480 reco bins
i_flat_g = flat480(i_chg_g, i_cell_g, i_zp_g)   # 480 gen bins
both_valid = (i_flat_r >= 0) & (i_flat_g >= 0)
reco_ok    = (i_flat_r >= 0)
# yields (reco_ok subset, both_valid subset)
yield (i_flat_r[reco_ok], w[reco_ok],
       i_flat_r[both_valid], i_flat_g[both_valid], w[both_valid])
```

Consequences:

- `R` is filled ONLY for `both_valid` (matched reco AND matched gen
  both in the 480-bin space). Gen-only events (no reco partner —
  acceptance loss) are NEVER seen by this loop because
  `stream_mc_reco_csv` reads only the MC reco file with
  `IsMatched == 1 & IsMatchedHadron == 1` (line 247–248). The MC gen
  file is read separately and goes into `prior_gen` only (no joint
  fill).
- `matched_reco_marginal` is `R.sum(axis=1)` by construction —
  consistent with the prep step's identification at line 50.
- `reco_marginal` (all matched-hadron reco in range, regardless of
  gen validity) is filled separately. For our cuts this equals
  `matched_reco_marginal` exactly (because in `stream_mc_reco_csv`
  the only way `reco_ok` is True but `both_valid` is False is if
  `i_flat_r >= 0` and `i_flat_g < 0` — i.e., reco in PS but matched-gen
  out of PS; these *do* exist and represent the "fake-like" bin
  contribution).
- There is **no separate accumulator for gen-only / miss events**.
  Acceptance is recoverable post hoc as
  `ε(gen_bin) = matched_gen(gen_bin) / prior_gen(gen_bin)` where
  `matched_gen = R.sum(axis=0)`.

So **`build_response_4d.py` does NOT produce overflow bins on either
axis today.** To implement Option 2 we need either:

- (a) extend `build_response_4d.py` to produce an extra 481-bin gen
  axis with the last bin = "gen events with no valid reco" (miss),
  and optionally a 481-bin reco axis with the last bin = "reco events
  with no valid gen" (fake), filled while streaming the MC reco file
  (and also from the MC gen file for the miss bin), OR
- (b) extend `prep_tunfold_inputs.py` to compute the overflow row from
  the existing NPZ contents: the miss row is
  `miss[g] = prior_gen[g] - matched_gen[g] = prior_gen[g] - R.sum(axis=0)[g]`,
  and the fake column is
  `fake[r] = reco_marginal[r] - matched_reco_marginal[r]` (which is
  zero today by construction; would need a separate accumulator in
  the builder to be non-zero).

**Path (b) is the lower-cost route** because it does not require
re-running the full Condor response build. It uses already-stored
quantities (`prior_gen`, `R`, `matched_reco_marginal`) and a single
new prep-time computation. Path (a) is cleaner but more expensive.

### 4b. Cite: line numbers for the relevant pieces

| Quantity | File | Lines |
|---|---|---|
| `R[reco, gen]` fill (both_valid only) | `build_response_4d.py` | 347–348 |
| `matched_reco_marginal` fill (both_valid only) | `build_response_4d.py` | 349–350 |
| `reco_marginal` fill (all matched-hadron reco) | `build_response_4d.py` | 344–345 |
| `prior_gen` fill (full gen pool, separate file) | `build_response_4d.py` | 356–368 |
| NPZ output keys | `build_response_4d.py` | 458–482 |
| Prep `h_R` fill (no overflow) | `prep_tunfold_inputs.py` | 75–83 |
| Prep `h_prior_gen` fill | `prep_tunfold_inputs.py` | 104–111 |
| Macro construct TUnfoldDensity | `unfold_tunfold_official_v3final.C` | 124–128 |

---

## 5. Concrete implementation plan

In dependency order. All NAF paths are absolute.

### Step 1 — Decide overflow convention (BLOCKING)

Confirm with the user that Option 2 = TUnfold-native overflow-bin
acceptance (Britzger pattern), per §2b. Subdecisions:

- 5.1a Use one gen-axis overflow row only (miss / acceptance loss), or
  also a reco-axis overflow column (fakes)? Fakes are not present in
  our matched-only response; only relevant if non-zero `reco_marginal
  - matched_reco_marginal` is expected to be subtracted at the
  unfolding step rather than upstream. **Default recommendation:
  gen-axis overflow only.** Reco-axis is already 480 bins of fully-
  matched reco; bg subtraction is done upstream
  (`data_subtracted_v3final_REAL_LUMI_filtered.json`).
- 5.1b Keep `kHistMapOutputHoriz` or switch to `kHistMapOutputVert` to
  match Britzger? **Default recommendation: keep `Horiz`** — the prep
  step is internally consistent and Horiz makes the response TH2
  "gen on x, reco on y" which is the same shape as the underlying
  NPZ. Document the difference vs Britzger.

### Step 2 — Extend `prep_tunfold_inputs.py` to add the overflow gen bin

Add a `--with-acceptance-overflow` flag (default True for Option 2).
When set, compute:

```python
matched_gen = R.sum(axis=0)                       # (480,)
miss        = np.maximum(prior_gen - matched_gen, 0.0)
miss_w2     = np.maximum(prior_gen_w2 - R_w2.sum(axis=0), 0.0)
NB_GEN_EXT  = NB + 1                              # 481
```

Build the TH2D with `NB_GEN_EXT` bins on the gen axis and `NB` on the
reco axis:

```python
h_R = ROOT.TH2D("h_R", "Response (with miss overflow);gen bin (output);reco bin (input)",
                NB_GEN_EXT, 0.0, float(NB_GEN_EXT),
                NB,         0.0, float(NB))
# physical 480x480: matched migrations
for j in range(NB):
    for i in range(NB):
        v = R[i, j]
        if v == 0 and R_w2[i, j] == 0: continue
        h_R.SetBinContent(j+1, i+1, float(v))
        h_R.SetBinError  (j+1, i+1, float(np.sqrt(max(R_w2[i,j], 0.0))))
# overflow gen bin (j = NB = 480): does NOT participate in the
# response per bin -- this is the "no reco" column from TUnfold's
# perspective. We DO NOT fill any reco bin for the overflow gen.
# Instead, TUnfold's area constraint with the implicit acceptance loss
# is encoded by leaving the (overflow_gen, *_reco) row empty AND
# loading the miss into a separate h_acceptance histogram on the gen
# axis, used downstream to renormalise the unfolded vector.
```

Wait — this is the subtle point. There are two equivalent encodings:

- **Encoding A (overflow gen):** Add one extra GEN bin that has no
  corresponding reco column. TUnfold will assign zero data to the
  ratio "this gen contributes only to the missing reco". This works
  with `kHistMapOutputVert` and the data input having only N_reco
  entries. Not directly compatible with `kHistMapOutputHoriz` in the
  same simple form.
- **Encoding B (overflow reco, Britzger):** Add one extra RECO bin
  that absorbs the gen-only weight (the EFF matrix). TUnfold sees a
  rectangular matrix with `N_reco_ext = N_reco + 1` and
  `N_gen = N_gen`. The data input has N_reco_ext entries, with the
  last entry = 0. The unfolding solves for gen bins, and the area
  constraint normalises gen → data total; the extra reco column
  ensures that the *gen* weight not seen in data is attributed to
  acceptance loss, not redistributed across physical bins.

**Britzger uses Encoding B.** This requires switching to
`kHistMapOutputVert` (or transposing `h_R` if keeping Horiz). The data
TH1 must also be extended by one bin (zero content, large error or
typical Poisson floor).

**Recommended:** switch to Encoding B + `kHistMapOutputVert`. Prep
emits:
- `h_R` of shape `(N_reco + 1, N_gen) = (481, 480)`, with the last
  reco bin filled by the per-gen miss weights `miss[g]`.
- `h_data_bgsub` of shape `(N_reco + 1) = (481)`, with the last bin
  set to 0 and its error to a small floor (e.g., `sqrt(sum(miss)/N_reco)`).
- `h_data_bgsub_w2` similarly extended.
- `h_prior_gen` unchanged (480 bins).

Metadata `meta_acceptance_overflow = "encoding=B, last_reco_bin = sum_g miss[g] * δ(g)"`.

### Step 3 — Extend `unfold_tunfold_official_v3final.C` to switch hist-map and consume the extended matrix

One-paragraph plan: change line 125 to
`TUnfold::kHistMapOutputVert` and keep `kRegModeCurvature +
kEConstraintArea`. Verify that `SetInput(h_data)` accepts the
`(N_reco + 1)`-bin TH1. Add a CLI flag `--n-reco-ext / argument
nRecoExt` so the macro knows which bins to drop in the output. After
`GetOutput`, the output histogram already has 480 gen bins (no
overflow to drop because the gen axis was kept physical). The
acceptance is absorbed in-solver. Add `binmap`-style slicing only if
we choose Encoding A.

### Step 4 — Update `postprocess_tunfold_official.py` to consume the extended outputs

The unfolded output is still 480 bins (gen axis was not extended in
Encoding B). The refold output is now 481 bins; slice
`refold[:NB]` before per-bin χ² and ignore the last bin (which folds
the miss weight back). Update the closure metric to either:

- Compute χ²/ndf on the first 480 reco bins only (excluding the
  acceptance bin), OR
- Compute χ² on the full 481 and report both numbers.

Cell-yield aggregation (`cell_yields`, `cell_yields_err`, lines 25–55)
is unchanged because it works on the 480-bin gen-side output.

Total comparison vs Python canonical (lines 351–367) should change:
under Encoding B the unfolded total is now in the **gen-pool scale**
(because acceptance is folded in), so the −13.35 % shift vs
6 005 256 should largely close. The new expected unfolded total is
≈ 6.0 M ± 0.1 M (matching Python canonical). Update the cross-check
note at lines 360–367 accordingly.

### Step 5 — Update `run_tunfold_official_v3final.sh`

Add `--with-acceptance-overflow` to the prep step (or make it
default-on). Bump the output filename suffix to
`_areaConstraint_overflow` so it does not collide with any prior
plain-TH2 audit run.

### Step 6 — (Optional, follow-up) `AddSysError` integration

Once Step 5 is producing a sensible nominal, add per-systematic
shifted-response loading. This requires `build_response_4d.py` runs on
the shifted MC samples already in
`H1/sidis_highQ2/data/responses_4d_v3final/sys_*` (NAF path TBD; see
`research-4d-unfolding.md` for sys-response inventory). Switch the
macro from `TUnfoldDensity` to `TUnfoldSys` (or subclass to add
`AddSysError`/`GetDeltaSys`) so the analytic systematic covariance can
be extracted alongside the statistical one.

### Step 7 — New files

If we cleanly separate Encoding A and Encoding B for audit, optionally
introduce:

- `prep_tunfold_inputs_overflow.py` (separate from
  `prep_tunfold_inputs.py`) so the old plain-TH2 path is preserved for
  comparison.
- `unfold_tunfold_official_v3final_overflow.C` (alongside the current
  macro) for the same reason.

**Recommendation:** flag-based extension of the existing files (no
new files) — the duplication tax outweighs the audit benefit, and
the old behaviour is recoverable by passing `--no-overflow`.

### Step 8 — NAF input paths for inputs (verified to exist 2026-05-30)

| Purpose | NAF path |
|---|---|
| Response NPZ (canonical bg-subtracted) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/response_4d_v3final_FINAL_bgsubtracted.npz` |
| Bg-subtracted data JSON | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data_subtracted_v3final_REAL_LUMI_filtered.json` |
| Python-TUnfold canonical | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/unfolded_v3final_FINAL_bgsubtracted_TUnfold.json` |
| Trust mask | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/v5g_4d_trust_mask_v3p2.json` |
| IBU primary (cross-check) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/unfolded_4d_v3final_IBU.json` (per driver `run_*.sh` line 39) |
| MC truth (gen prior pool) | already inside the response NPZ as `prior_gen` |
| MC matched-gen marginal | derive on the fly as `R.sum(axis=0)`; line 437 of build script confirms this is consistent |

The bg-subtracted response NPZ contains: `response`, `response_w2`,
`prior_gen`, `prior_gen_w2`, `matched_reco_marginal`,
`matched_reco_marginal_w2`, `reco_marginal`, `reco_marginal_w2`,
`data_marginal`, `data_marginal_w2`, plus metadata. The "miss" row for
Encoding B can be computed as
`prior_gen - R.sum(axis=0)` per gen bin, with corresponding variance
`prior_gen_w2 - R_w2.sum(axis=0)`, clipped at 0 for numerical safety.

---

## 6. Validation plan

### 6a. Smoke test (login node, ≤ 5 min, single input file)

Per AGENTS.md SSH safety rules, the login node is limited to one input
file and 5 minutes wall.

1. Locally edit prep/macro per Step 2–3.
2. `scp` the four edited scripts to NAF.
3. Run the prep step interactively on the login node:
   ```
   python3 prep_tunfold_inputs.py \
     --response-npz output/response_4d_v3final_FINAL_bgsubtracted.npz \
     --data-json    output/data_subtracted_v3final_REAL_LUMI_filtered.json \
     --out-root     /tmp/tunfold_inputs_smoke.root
   ```
   Expected wall: < 30 s. Verifies the prep produces a 481×480 (or
   481×481) TH2 and an extended data TH1 without errors.
4. Run the C++ macro:
   ```
   root -l -q -b 'unfold_tunfold_official_v3final.C+("/tmp/tunfold_inputs_smoke.root","/tmp/out_smoke.root","/tmp/out_smoke.json")'
   ```
   Expected wall: 2–4 min (TUnfoldDensity on a 481×480 matrix with
   ScanLcurve + ScanTau at nScan=100). If this exceeds 5 min on the
   login node, drop `nScan` to 30 for the smoke test only.

Pass criteria:
- macro returns 0.
- `out_smoke.json` lists `unfolded_total` within ±10 % of the Python
  canonical 5 203 379 (Encoding A) or within ±5 % of the gen-pool
  scale 6 005 256 (Encoding B).
- `refold_chi2_per_ndf` is in the range 0.10–2.0 (the Python canonical
  is 0.142; expect a similar order).
- No NaN / inf in the unfolded vector.

### 6b. Full Condor run

After smoke passes, submit a Condor job (single executor, single input)
to run the entire `run_tunfold_official_v3final.sh` driver. Expected
wall: 10–20 min. Outputs land in
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/`
under the `_areaConstraint_overflow.{root,json}` suffix and the
covariance NPZ.

### 6c. Comparison to Python-TUnfold canonical

Use `postprocess_tunfold_official.py`'s built-in canonical comparison
block (lines 167–212) to report:

- Total event count: must agree to within ±5 % of 5 203 379 (Encoding
  A) or 6 005 256 (Encoding B). Encoding B is the intended target
  because acceptance is now in-solver.
- Per-stratum totals (S0..S3): must agree to within ±3 %. The Python
  canonical's per-stratum shifts (S0a −36.69 %, S1a −24.03 %, etc.,
  per handoff line 615) reflect bg subtraction; the new C++ result
  starting from the bg-subtracted data should reproduce the bg-applied
  numbers, not the pre-bg numbers.
- Per-cell h+/h− symmetry: must agree at the 0.10 pp level (Python
  canonical: 0.05 pp).
- Refold χ²/ndf: must be in the range 0.10–0.30 (Python canonical:
  0.142).
- Negative-bin count: Python canonical has 23 negatives. Expect 20–35.

**τ-equivalence:** the Python canonical uses τ = 0.10 with L
pre-rescaled by `1/sqrt(N_eff)` with `N_eff = 3.40 × 10⁶`, equivalent
to standard TUnfold τ ≈ 5.4 × 10⁻⁵. The C++ macro's `ScanTau` will
land somewhere in 10⁻⁶ – 10⁻³ depending on the matrix structure; the
canonical τ should be cited in *both* conventions in the final JSON
(`postprocess_tunfold_official.py` already does this at lines 304–319).
Expect the macro's `tau_chi2` to fall within an order of magnitude of
5.4 × 10⁻⁵; if it differs by more than 2 orders of magnitude, that is
a red flag for the rescaling logic.

### 6d. Comparison to BBB and IBU

The postprocess script's `--ibu-json` argument (line 67–68) already
loads the IBU-4D primary if it exists. Pass
`output/unfolded_4d_v3final_IBU.json`. Per-stratum agreement target:
within ±5 % (Python TUnfold vs IBU was 1.41 % median per handoff line
432). For BBB the comparison is not currently wired in; add an
optional `--bbb-json` flag and load
`output/unfolded_4d_v3final_BBB.json` if present.

---

## 7. Open questions for the supervisor / user

1. **Confirm Option 2 = overflow-bin (Britzger) encoding.** No repo file
   enumerates "Option 1/2/3" for the C++ port. The most defensible
   reading is §2b. If the user means something else (e.g., "Option 2
   in a private design memo"), point to the memo so I can re-anchor
   the plan.

2. **Encoding A vs Encoding B.** Both achieve in-solver acceptance.
   Britzger uses Encoding B (reco-axis overflow + `kHistMapOutputVert`).
   Our current draft uses `kHistMapOutputHoriz`. The migration is
   smaller for Encoding B (just transpose + extend the reco axis +
   switch the hist-map flag); Encoding A requires gen-axis extension
   and `binmap` slicing on the output. **Default recommendation:
   Encoding B.** Please confirm.

3. **Fakes (`_BK` in Britzger).** Britzger fills a reco-only "BK"
   matrix for events with no gen partner and explicitly does NOT add
   it into the nominal TUnfold matrix; instead, fakes are handled by
   background subtraction. Our pipeline does upstream bg subtraction
   (`data_subtracted_v3final_REAL_LUMI_filtered.json`). So fakes are
   already absent from the data input. No additional handling needed.
   Confirm this is the intended convention.

4. **Should we promote to `TUnfoldSys` now or in a follow-up?** The
   current draft uses `TUnfoldDensity`. To incorporate detector
   systematics analytically (the Britzger `AddSysError` /
   `GetDeltaSys` pattern), we need `TUnfoldSys`. The handoff §13
   indicates the 4-term systematics budget is already complete
   externally via shifted-response Python re-runs, so the C++ port
   does NOT strictly need `TUnfoldSys`. Confirm scope: nominal only,
   or nominal + sys?

5. **τ choice policy.** The current macro defaults to `ScanTau`
   rho-avg minimum. The handoff (line 267) shows the production
   Python pipeline uses `τ = 0.10` (in our scale) chosen from an
   extended scan, not from `ScanTau`. For the C++ port: (i) pin τ to
   the equivalent ≈ 5.4 × 10⁻⁵ for an apples-to-apples comparison,
   then (ii) report `ScanTau`'s optimum as audit. Confirm preferred
   policy for the canonical run.

6. **MC-truth gen overflow for Encoding A (only if A is chosen).**
   For Encoding A we need `prior_gen - matched_gen` per gen bin; that
   is computable from the existing NPZ. But for full Britzger fidelity
   the gen overflow should ALSO include events with `gen ∈ PS` but
   `reco ∉ PS` (true acceptance loss as seen in MC). This is exactly
   `prior_gen - matched_gen` and the math is identical. No new MC
   pass needed. Confirm we are not expected to re-stream MC for this.

7. **TUnfoldBinning vs flat TH2.** Neither our draft nor Britzger uses
   `TUnfoldBinning::AddAxis`. The flat-TH2 path is sufficient for our
   index-binned 480-bin response. TUnfoldBinning would be required
   only if we wanted physical-unit axes (Q², x, z, pT_h) with
   density-mode bin-width normalisation. Confirm flat-TH2 is the
   intended path.

---

## Appendix: file/line cross-reference

| Topic | Where |
|---|---|
| Draft macro construction | `unfold_tunfold_official_v3final.C` 124–128 |
| Draft macro ScanTau | `unfold_tunfold_official_v3final.C` 161–168 |
| Draft prep TH2 fill | `prep_tunfold_inputs.py` 75–83 |
| Draft prep data fill | `prep_tunfold_inputs.py` 85–99 |
| Draft postprocess covariance | `postprocess_tunfold_official.py` 94–102 |
| Python canonical TAU = 0.10 | `unfold_TUnfold_4d.py` 49 |
| Python L-rescaling | `unfold_TUnfold_4d.py` 121–123 |
| Probability operator (Python) | `unfold_TUnfold_4d.py` 53–58 |
| Response builder R fill | `build_response_4d.py` 347–348 |
| Response builder NPZ keys | `build_response_4d.py` 458–482 |
| Britzger H1OO matched fill | `g1j_ES/Analysis/AnalysisEventShapes.C` 1223–1224 |
| Britzger H1OO miss (EFF) fill | `g1j_ES/Analysis/AnalysisEventShapes.C` 1217–1218 |
| Britzger H1OO fake (BK) fill | `g1j_ES/Analysis/AnalysisEventShapes.C` 1229–1230 |
| Britzger H1OO rad-corr (OF) fill | `g1j_ES/Analysis/AnalysisEventShapes.C` 1235–1236 |
| Britzger nominal matrix = moreRec + OF | `g1j_ES/Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` 948–980 |
| Britzger sys matrix = moreRec + EFF | `g1j_ES/Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` 605–622 |
| Britzger sys matrix = moreRec + OF (alt) | `g1j_ES/Analysis/RooUnfold/my_unfold.cxx` 147–153 |
| Britzger TUnfoldDensity construct | `g1j_ES/Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` 144 |
| Britzger ScanTau call | `g1j_ES/Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx` 179 |
| Britzger TUnfoldSys + AddSysError | `g1j_ES/Analysis/SysUncertainty.cxx` 238–246 |
| Britzger GetOutput + binmap slicing | `g1j_ES/Analysis/SysUncertainty.cxx` 269–271 |
| Handoff Python TUnfold canonical | `H1/sidis_highQ2/context-handoff.md` 619, 1143–1145 |
| Handoff τ choice 0.10 + N_eff = 3.40e6 | `H1/sidis_highQ2/context-handoff.md` 4 (As of 2026-05-27), 432, 448 |

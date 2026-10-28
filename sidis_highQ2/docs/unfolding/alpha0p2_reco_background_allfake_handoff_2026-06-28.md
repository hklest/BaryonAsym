# High-Q2 Charged-SIDIS alpha0p2 Reco-Background and All-Fake Response Handoff

Date: 2026-06-28
Audience: future unfolding agent for high-Q2 charged-SIDIS v6 z005.
Scope: local handoff only. Do not modify NAF files or rerun analysis from this document alone.

## 1. Context and purpose

This handoff summarizes the validated high-Q2 charged-SIDIS v6 z005 `alpha<0.2` reco-background / fake-treatment studies and the diagnostic all-fake response variant. The immediate unfolding question is how to handle reco-side non-signal components consistently with the response matrix, especially:

- matched electrons,
- matched V0 daughters,
- GEANT/STR charged-hadron secondaries currently labelled `det.int.`,
- separate low-Q2 DJANGOH leakage/background.

The aim is response-consistent treatment: do not subtract a component externally if it is already represented in the fake column and fake subtraction/covariance is active, and do not blindly subtract components that are mostly represented as matched physical response entries.

## 2. Background categories and definitions

- `electron`: `IsMatchedElectron != 0`. This includes truth `e±`, including STR/material/conversion/showering `e±` if matched and selected.
- `V0 daughter`: `IsMatchedV0Daughter != 0`. Ancestor list: `K0S(310)`, `K0L(130)`, `K0/K0bar(|311|)`, `Lambda/anti-Lambda(|3122|)`.
- `det.int.`: STR-only GEANT secondary charged `pi/K/p` matched tracks, with classifier
  `IsStr==1 && IsGtr==0 && IsMatchedHadron==1 && IsMatchedElectron==0 && IsMatchedV0Daughter==0`.
  Do not overclaim this as pure material interactions; safer label is GEANT/STR charged-hadron secondaries.
- `lowQ2`: separate pre-scaled DJANGOH low-Q2 leakage/background source, sample-distinct from high-Q2 RAPGAP/DJANGOH signal MC.

## 3. Current normalization convention and totals

Use `L_data = 351000.0 nb^-1`. The older `356613` convention is superseded for current high-Q2 charged-SIDIS background-yield comparisons.

Uniform L351000 diagnostic package:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/`

Validated L351000 integrated totals:

| Component | Scaled total |
|---|---:|
| electron avg | 148976.4352 |
| V0 avg | 132065.2768 |
| lowQ2 | 208.3795 |
| det.int avg | 27405.8031 |
| total | 308655.8946 |
| denominator | 3865073.0 |
| total fraction | 0.0798577141 |

## 4. Overlap and exclusivity

Electron/V0 overlap is tiny. At L351000 the scaled average `electron ∩ V0` overlap is `112.2093`, which is:

- `0.0753%` of electron,
- `0.0850%` of V0,
- `0.0364%` of the full total.

Therefore a simple sum is acceptable for most diagnostics, or subtract the overlap once if a strictly exclusive accounting is required. The `det.int.` classifier/audit gives exactly zero overlap with electron/V0. The `lowQ2` component is source-distinct by metadata.

Overlap diagnostics:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/alpha0p2_overlap_exclusivity_20260626/alpha0p2_overlap_exclusivity_summary.json`
- corresponding CSV in the same directory.

## 5. Response partition findings

### det.int. partition

Path:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/detint_response_partition_20260626/`

Condor cluster: `3971447`.

Finding: `det.int.` is approximately `86%` in the matched physical response block and approximately `14%` in the fake block. Therefore blind pre-subtraction of `det.int.` while keeping the unchanged response is not response-consistent.

### electron/V0 partition

Path:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/electron_v0_response_partition_20260627/`

Final Condor cluster: `3973257`.

Findings for current central v6 response convention:

- Non-V0 electrons are `100%` in the fake column of the alpha0p2 response.
- V0 daughters are `100%` excluded/dropped.
- Central v6 excludes both categories from the physical matched signal response.

## 6. All-fake response variant

Diagnostic output root:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/`

Key artifacts:

- `scripts/build_migration_th2_per_charge_allfake.py`
- `responses/migration_inputs_allfake_alpha0p2_z005.root`
- `responses/allfake_response_matrices_and_categories.npz`
- `validation/validation_summary.json`
- category fake-vector CSV/JSON files under the response/validation outputs
- compact local copies:
  `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/response_diagnostics/alpha0p2_allfake_response_z005_2026-06-27/`

Condor final cluster: `3987932`, `8/8` completed.

Response integrity:

- per-charge response shape `(491,246)`;
- `NB_R = 490`, `NB_G = 245`;
- finite and nonnegative;
- input guardrails: RAPGAP `364`, DJANGOH `501`;
- no duplicate files and no merged-file mixing.

Category weighted sums:

| Generator | fake column | electron | V0 | det.int. | other |
|---|---:|---:|---:|---:|---:|
| RAPGAP | 17196553.31 | 3819129.90 | 3432137.16 | 676560.99 | 9268725.25 |
| DJANGOH | 19609919.68 | 4354659.74 | 3838524.72 | 837573.52 | 10579161.70 |

Miss accounting convention: accepted nonfake reco matches only. `det.int.` reclassified rows with valid signal-gen candidates induce a miss once, with duplicate CTG miss suppression.

## 7. Response-consistent treatment recommendation

For the current unchanged alpha0p2 response:

- V0 can be pre-subtracted/vetoed because it is excluded/dropped from the response.
- Non-V0 electrons are fake-column handled, so do not also pre-subtract them if fake subtraction is active.
- `det.int.` should not be blindly pre-subtracted because it is mostly matched physical response, not mostly fake.

For the all-fake response variant:

- electron, V0, and `det.int.` are all fake-column components;
- the data vector should not additionally subtract these components externally;
- fake subtraction and its covariance must handle them exactly once.

Choose one convention and propagate it consistently through data vector construction, response, fake/background covariance, closure, and refolding.

## 8. Remaining next actions for unfolding agent

1. Build the fake-subtracted data vector under the all-fake response convention. Decide consistently between row-fraction fake treatment and explicit background subtraction; do not apply both.
2. Run at least two unfolding methods/diagnostics using the all-fake response, e.g. IBU, BBB/bin-by-bin, and/or TUnfold diagnostic. SVD remains quarantined in current high-Q2 SIDIS context unless explicitly revalidated.
3. Perform Poisson-fluctuated closure tests, not noiseless `R · truth` tests.
4. Run cross-generator closure and generator-dependence checks.
5. Refold unfolded results and report refold chi2.
6. Audit negative bins, high-correlation/high-regularization bins, and per-bin fake fractions.
7. Propagate fake/background covariance and systematic choices explicitly.
8. Compare all-fake response results to current-response results.
9. Check impacts on charge-separated cross sections, charge asymmetry, and SIDIS moments.

## 9. Known caveats and blockers

- The all-fake response is diagnostic; central products were not overwritten.
- `det.int.` should be described as GEANT/STR charged-hadron secondaries, not as pure material interactions.
- The `351000.0 nb^-1` luminosity convention must be used consistently in any background-yield comparison.
- Current high-Q2 final release is still blocked by broader issues documented in `sidis_highQ2/context-handoff.md` and `docs/RUN_STATUS.md` (including nominal method choice, C_rad sparse-bin/provenance policy, and final plot/release decisions).

## 10. Quick checklist for future use

- Use L351000 diagnostics for yield comparisons.
- Do not double-subtract electron/V0/det.int when using the all-fake response.
- Treat lowQ2 as a separate source-distinct background.
- Validate all response matrices as finite/nonnegative with expected `(491,246)` per charge.
- Require Poisson closure, cross-generator closure, and refold chi2 before physics conclusions.

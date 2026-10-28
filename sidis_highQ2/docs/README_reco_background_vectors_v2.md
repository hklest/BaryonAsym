# README — alpha0p2 reco background vectors v2

Implementation root:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/`

## Purpose

This package produces reviewed standalone reco-space background vectors for the high-Q² charged-SIDIS alpha0p2 study, without modifying central outputs or the existing no-background alpha0p2 release.

It explicitly replaces the invalid 2026-06-24 `h1-script-runner` implementation under:
- `release_electron_fake_vector_corrected_20260624/`
- `release_v0daughter_vector_corrected_20260624/`

Those old roots are invalid and must not be used for physics subtraction.

## Implemented scripts

- `scripts/produce_reco_background_vector_alpha0p2_v2.py`
- `scripts/aggregate_reco_background_vectors_alpha0p2_v2.py`

## Sumw / yield convention

The producer follows the required normalization convention exactly:

- `sumw` = raw weighted MC sum before luminosity scaling
- `sumw2` = raw weighted second moment before luminosity scaling
- `yield = scale * sumw`
- `var = scale^2 * sumw2`

Subtraction must use `yield`, not `sumw`.
Background statistical variance must use `var`, not `sumw2`.

When period is known, the producer fills per-period raw sums and builds totals as:

- `yield_total(bin) = Σ_p scale_p * sumw_p(bin)`
- `var_total(bin) = Σ_p scale_p² * sumw2_p(bin)`

## Modes

Supported modes are exactly:

- `electron`: classify reco track as background if `IsMatchedElectron != 0`
- `v0daughter`: classify reco track as background if `IsMatchedV0Daughter != 0`

Missing classifier branches are fatal errors. There is no silent fallback and no dummy fill path.

For V0 diagnostics, if `VetoAncestorPDG` exists, a sidecar ancestor summary is emitted in JSON and optional ancestor arrays are written to NPZ where scaling is unambiguous.

## Selection and binning provenance

The producer mirrors the alpha0p2 response-builder reco selection and binning from:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py`

It verifies that the copied builder contains the expected provenance snippets for:
- Q² range
- y range
- Empz threshold
- current-hemisphere `xF_SIDIS_es < 0`
- reco track cleanup cuts
- alpha0p2 veto
- `flat_reco(...)` reco bin construction

The producer also loads and validates the alpha0p2 binmap:
- `NB_R == 490`
- `NB_G == 245`
- reco-to-gen map length `490`
- reco z and pT edge structure used to reconstruct the same flat reco index

Output convention is charge-major 980 bins:
- `0..489` = h+
- `490..979` = h−

Out-of-range reco indices are counted in diagnostics and never filled.

## Output schema

NPZ keys:
- `sumw_980`, `sumw2_980`, `yield_980`, `var_980`
- `sumw_hplus`, `sumw_hminus`, `sumw2_hplus`, `sumw2_hminus`
- `yield_hplus`, `yield_hminus`, `var_hplus`, `var_hminus`
- per-period arrays when period-specific filling is available
- optional ancestor arrays for V0 diagnostics

JSON metadata includes:
- mode, mc, period argument, resolved periods
- input files and per-file scale metadata
- tree name and required branches
- classifier branch
- response-script/binmap provenance
- scales used and `L_data`
- raw/scaled totals
- selected/classified track counts
- out-of-range bin count
- finite/nonnegative checks
- schema version and command line

## Smoke policy

Smoke outputs must:
- live under `smoke/`
- use filenames containing `SMOKE_`

The producer enforces both when `--smoke` is passed.

## Recommended smoke gates

Run one-file login-node smokes only.
Do not launch multifile production from this package manually.
Use `--max-entries` and `--smoke` for safe review artifacts.

See `docs/smoke_commands_v2.md` for exact commands.

## Aggregation policy

Use `aggregate_reco_background_vectors_alpha0p2_v2.py` only for reviewed partial outputs.
It refuses to aggregate smoke files unless `--allow-smoke` is given explicitly.

## Known invalid old roots

Do not use artifacts from:
- `release_electron_fake_vector_corrected_20260624/`
- `release_v0daughter_vector_corrected_20260624/`

Reason: the old implementation had placeholder-fill defects, wrong mode submission, incomplete period coverage, ambiguous smoke naming, and missing independent alpha0p2/binmap verification.

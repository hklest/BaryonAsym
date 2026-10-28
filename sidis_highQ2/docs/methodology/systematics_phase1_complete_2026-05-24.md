# Systematics Phase 1 Complete — High-Q² Charged SIDIS (v3 / HQ-v5g-Bprime)

**Date:** 2026-05-24
**Owner:** `h1-systematics`
**Plan reference:** `H1/sidis_highQ2/docs/plans/plan-systematics-v5g-v3-2026-05-24.md`
**Status:** Phase 1 (machinery, contract, and reference baseline) **COMPLETE**.

## What this memo records

Phase 1 of the systematics plan is the no-physics-yet phase: lock down
the data contract, build the combination machinery, and ship a working
baseline so that every per-source dispatch in Phase 2+ has a fixed
target schema and a tested combiner. No real systematic was evaluated
in this phase; everything below is infrastructure.

## Deliverables

| Deliverable | Path |
|---|---|
| Canonical data contract memo (FROZEN) | `H1/sidis_highQ2/docs/methodology/systematics_data_contract_v3.md` |
| Nominal-reference emitter script | `H1/sidis_highQ2/scripts/systematics/emit_nominal_uncertainty_reference.py` |
| Canonical combination utility | `H1/sidis_highQ2/scripts/systematics/combine_systematics.py` |
| Smoke-test script | `H1/sidis_highQ2/scripts/systematics/test_combine_smoke.py` |
| Source-00 nominal reference JSON | `H1/sidis_highQ2/data/systematics_sources/source_00_nominal.json` |
| Combined master JSON (baseline only so far) | `H1/sidis_highQ2/data/systematics_combined_v3_HQv5gBprime.json` |

The data contract memo defines the canonical per-source JSON schema
(top-level keys, per-cell sub-schema, units convention, propagation
types, correlation classes, validation rules). Every per-source
dispatch in Phases 2–5 must emit JSON conforming to it.

The combination utility:
- reads every `source_*.json` in `data/systematics_sources/`,
- validates each against the contract (rejects malformed sources),
- combines per the plan's rules (quadrature for uncorrelated,
  signed linear for `correlated_across_bins`, sign-flipped for
  `anti_correlated`, forced-zero on `A_h` when `cancels_in_A_h == true`),
- emits a master JSON with per-bin `delta_total_4D`, per-source breakdown,
  dominant-source-per-bin labels, and per-cell summary statistics,
- prints warnings (does not fail) for expected sources missing from disk.

## Smoke-test result

Running `python3 scripts/systematics/test_combine_smoke.py` exercises
two scenarios:

1. **One uncorrelated 1% source** on top of source_00 nominal. Expected
   combined `delta_total / |X_nom| = 1.000%` on M, sigma, A_h.
   **PASS: 1148/1148 valid bins all show 1.000% combined delta.**

2. **Add a correlated 2% source with `cancels_in_A_h = true`.**
   Expected: `delta_total[M,sigma] = sqrt(1%² + 2%²) = 2.2361%`;
   `delta_total[A_h] = 1%` (the correlated source cancels in A_h).
   **PASS: 1148/1148 valid bins all match the expected combined values
   for M, sigma, and A_h.**

Both smoke source files are deleted on test completion, leaving only
`source_00_nominal.json` in `data/systematics_sources/`.

## Combiner handles missing sources gracefully

A bare run of `combine_systematics.py` with only `source_00_nominal.json`
present produces a warning listing the 19 expected sources that are not
yet on disk:

```
WARNING: 19 expected source(s) not present, skipped gracefully:
  - missing: tracking_eff_common
  - missing: tracking_eff_chargedep
  - missing: hfs_scale
  ... (all 18 plan sources, plus nominal-baseline tag if absent)
```

The combiner exits 0 in this case and writes a master JSON containing
just the nominal baseline. This unblocks Phase 2 dispatches to start
producing sources one by one without breaking the rest of the chain.

## Source roster and current status

All 18 sources from the plan's master systematics table (Section
"Master systematics table", lines 198–217 of the plan) are mapped to
expected `source_id` values in the combiner. None of them are evaluated
yet.

| # | `source_id`                  | Plan section | Status         | Phase | Propagation type expected   |
|---|------------------------------|--------------|----------------|-------|-----------------------------|
| – | `nominal`                    | n/a          | DONE           | 1     | analytic_constant (zeros)   |
| 1a | `tracking_eff_common`       | § 1          | not_started    | 2     | analytic_constant           |
| 1b | `tracking_eff_chargedep`    | § 1          | not_started    | 2     | analytic_constant / local_reweight |
| 2 | `hfs_scale`                  | § 2          | not_started    | 3     | shifted_extractor (Condor)  |
| 3 | `electron_energy_scale`      | § 3          | not_started    | 3     | shifted_extractor (Condor)  |
| 4 | `electron_theta_scale`       | § 4          | not_started    | 3     | shifted_extractor (Condor)  |
| 5 | `trigger_efficiency`         | § 5          | not_started    | 2     | local_reweight              |
| 6 | `luminosity`                 | § 6          | not_started    | 2     | analytic_constant           |
| 7 | `mc_model`                   | § 7          | not_started    | 4     | alt_mc (RAPGAP31)           |
| 8 | `pdf_set`                    | § 8          | not_started (likely blocked) | 2 | local_reweight or blocked |
| 9 | `crad_model`                 | § 9          | not_started    | 4     | alt_mc (alt C_rad)          |
| 10 | `background_subtraction`    | § 10         | not_started    | 4     | alt_mc                      |
| 11 | `beam_energy`                | § 11         | not_started    | 2     | local_reweight              |
| 12 | `vertex_z`                   | § 12         | not_started    | 3     | shifted_extractor (Condor)  |
| 13 | `y_window`                   | § 13         | not_started    | 3     | shifted_extractor (Condor)  |
| 14 | `empz_window`                | § 14         | not_started    | 3     | shifted_extractor (Condor)  |
| 15 | `track_quality_dca`          | § 15         | not_started    | 2 or 3 | local_reweight or shifted_extractor |
| 16 | `nuclia_clip`                | § 16         | not_started    | 2     | local_reweight              |
| 17 | `ep0304_inclusion`           | § 17         | not_started    | 2     | local_reweight (dataset split) |
| 18 | `frame_definition`           | § 18         | not_started    | 2     | local_reweight (alt frame)  |

## What Phase 2 unblocks

With the contract and combiner in place, the following can proceed in
parallel as independent specialist dispatches, since each emits one
self-contained `source_<NN>_<id>.json` that the combiner picks up
automatically:

- **`luminosity`** — pure analytic ±2.3% on `sigma`. Smallest possible
  Phase 2 dispatch. Expected: ~1 hour.
- **`tracking_eff_common`** — analytic 2%/track on M and sigma; cancels
  in A_h. Expected: 1–2 hours.
- **`tracking_eff_chargedep`** — charge-dependent residual; requires
  the v5f reference logic to be ported to Scheme B′ and v3 inputs.
  Expected: 3–4 hours.
- **`nuclia_clip`** — local reweight on the v3 per-track CSV
  (`H1/sidis_highQ2/data/pertrack_v5g_fixedFrame_v3_merged.csv.gz`),
  clipping NuclIA weights at 5 / 10 / 20. Expected: 2–3 hours.
- **`beam_energy`** — local propagation using per-event `BeamElecE` /
  `BeamProtP` from the same CSV. Expected: 2–3 hours.
- **`ep0304_inclusion`** — local reweight: drop ep0304 events from
  numerator AND DIS denominator, recompute, take half-difference.
  Expected: 2–3 hours.
- **`frame_definition`** — local recompute of `pT_h` / `z` / `phi_h`
  in an alternative (Breit) frame from the v3 per-track basis.
  Expected: 4–6 hours.
- **`trigger_efficiency`** — definition + small data-side variation
  test. Expected: 1–2 hours.
- **`pdf_set`** — audit + likely blocked placeholder. Expected: 1 hour.

Phase 3 (Condor-backed shifted-extractor sources) and Phase 4 (alt-MC
sources) remain blocked on production sweeps as described in the plan's
"Condor-required sources" and "Local / fast sources" lists.

## Cross-references to parallel workstreams

This Phase 1 dispatch was launched in coordination with the **unfolding
workstream**, which has three parallel dispatches in flight (IBU,
TUnfold, OmniFold) to address the **59 low-purity bins** that are
flagged `bbb_valid_4D == false` in the canonical xsec JSON.

Implications for Phase 2+ of systematics:

- The combiner's per-source AND-mask of `bbb_valid_4D` already excludes
  low-purity bins from totals automatically. So the bin-by-bin
  systematics pass will produce totals **only on the purity ≥ 0.5
  bins** until the unfolding workstream delivers corrected central
  values for the deferred bins.
- Each per-source JSON must still include those 59 bins in its
  payload (with `bbb_valid_4D = false` so the combiner skips them).
- Once unfolding produces an updated central observable JSON and a
  new bin-validity mask, `source_00_nominal.json` must be re-emitted
  to pick up the new mask, and per-source JSONs that were derived on
  the BBB grid must be re-validated against the unfolded grid
  (or kept separate and flagged `deferred_to_unfolding` in the master
  JSON for those bins).
- Unfolding may itself become one or more additional source ids
  (e.g. `unfolding_method_spread`, `unfolding_prior`) — these are
  not yet in the master systematics table and will need to be added
  by the unfolding workstream when it produces its own per-source
  JSON files in the same contract.

## What was NOT done (per dispatch constraints)

- No per-source systematic was evaluated (Phase 2+).
- No existing v5f script (`compute_systematics_v5f_*`) was modified;
  they remain frozen for audit.
- No production / Condor work; everything in Phase 1 ran locally on the
  workstation.
- The canonical extractor and central xsec JSON were not touched.

## Gaps / resolved ambiguities

These small under-specifications in the plan's "Output convention"
section were resolved here and codified in the data contract:

1. **Units of `delta_*_4D`**: plan implies but does not state. Resolved
   to **absolute units** matching the central observable, with the
   combiner converting to relative for reporting. This avoids
   divide-by-near-zero in low-stat corners.
2. **Symmetric vs asymmetric storage**: plan mentions both. Resolved by
   requiring `delta_central_4D` always (may be `null`) and allowing
   optional `delta_up_4D` / `delta_down_4D` for asymmetric sources.
   Combiner uses `delta_central_4D` if non-null, else `max(|up|,|down|)`.
3. **`phi_harmonics` payload shape**: plan lists it but does not pin
   the layout. Resolved by allowing per-harmonic scalar shifts per
   `(cell, charge)`; Phase 2 dispatches that produce harmonics will
   refine this if 1D-over-φ_h shifts are needed.
4. **bbb_valid mask consumption**: plan does not say which observable's
   mask wins for the A_h channel. Resolved: the A_h mask is the
   logical AND of the h+ and h- masks (a bin is valid for A_h only if
   both charges have valid BBB).
5. **Status semantics**: plan lists `ok`, `blocked`,
   `deferred_to_unfolding`, `placeholder_not_for_total`, but doesn't
   spell out combiner behavior for each. Codified in Section 7 of the
   data contract.
6. **Expected source roster**: the plan's master table has 18 sources
   but allows splitting (e.g. tracking common vs charge-dep). The
   combiner's `EXPECTED_SOURCE_IDS` list (in
   `combine_systematics.py`) is the operational source roster and
   includes 20 entries (1a/1b split + `nominal`). Update there when
   the unfolding workstream adds its own sources.

## Acceptance

Phase 1 is judged complete when all five of the deliverables above
exist and the smoke test passes. Both conditions are met as of
2026-05-24. Phase 2 (analytic + reweighting sources) is unblocked.

# Handoff: Build reco-space fake/background vector before IBU for high-Q² z005 alpha0p2

## Goal

Create a new high-Q² charged-SIDIS z005 alpha0p2 release where all known reco-only backgrounds are subtracted from the reco data vector **before** IBU unfolding.

The total reco-space subtraction vector should be:

`b_total_reco = b_lowQ2_DJANGOH_reco + b_scatteredElectronFake_reco`

Then unfold:

`d_reco_subtracted = d_reco_alpha0p2 − b_total_reco`

using the usual signal response:

`reco π/K/p → truth π/K/p`

This is required because surviving scattered-electron fake tracks that pass the α < 0.2 veto are otherwise likely unfolded as charged-hadron signal.

---

## Current context

Analysis:
- H1 high-Q² charged-particle SIDIS
- z005 finalcandidate binning
- inclusive charged hadrons, no PID
- alpha0p2 side-study: remove reco tracks with `α(track,e) < 0.2 rad`

Alpha0p2 study root:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/`

Alpha0p2 no-background release:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/release/`

Alpha0p2 response ROOT:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.root`

Alpha0p2 binmap:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.binmap_v3.json`

Alpha0p2 copied scripts:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/`

Important release behavior:
- `run_bbb_ibu_release_v4.py` consumes the ROOT histograms:
  - `h_data_bgsub_pos`
  - `h_data_bgsub_neg`
  - `h_data_bgsub_pos_w2`
  - `h_data_bgsub_neg_w2`
- It does **not** use `data_json` as the unfolding input.

---

## Existing alpha0p2 validation

Alpha0p2 release was produced and validated:
- data vector length: `980`
- response:
  - `NB_G = 245`
  - `NB_R = 490`
  - response shape `(491,246)` per charge/generator
- IBU k=4:
  - rows `490`
  - finite rows `490`
  - negative cross sections `0`
- BBB:
  - rows `490`
  - finite rows `490`
  - negative cross sections `0`

Alpha0p2 data effect:
- pre: `3,902,480`
- after α < 0.2 veto: `3,873,898`
- rejected: `28,582`

Alpha0p2 MC effect:

| MC | pre | after | rejected | rejected matched-electron | surviving matched-electron |
|---|---:|---:|---:|---:|---:|
| RAPGAP | 67,440,033 | 67,152,756 | 287,277 | 215,731 | 2,738,405 |
| DJANGOH | 80,569,547 | 80,205,027 | 364,520 | 273,538 | 3,331,572 |

Those surviving matched-electron tracks are the concern.

---

## Why this is needed

Current alpha0p2 treatment:
- removes tracks geometrically if `α(track,e) < 0.2`
- does **not** apply `IsMatchedElectron == 0`, which is correct because data has no truth label
- but also does not yet have an explicit scattered-electron fake subtraction sidecar

ROOT audit found:
- response matrices present and valid
- no explicit `IsMatchedElectron` histogram
- no explicit electron-fake histogram
- no explicit `fake` or `miss` sidecar by name

So the conservative conclusion is:
- surviving electron fakes in data are not explicitly subtracted
- if non-negligible, IBU unfolds them as signal charged hadrons
- cross sections could be biased high in affected bins

---

## Correct strategy

Do **not** apply `IsMatchedElectron == 0` as a reco selection.

Instead:
1. Use MC truth labels to estimate a reco-space electron-fake vector.
2. Add it to the low-Q² DJANGOH reco background vector.
3. Subtract the total reco background vector from data.
4. Unfold the remaining reco π/K/p signal-like data to truth π/K/p.

Formula:

`d_reco_sub = d_reco_alpha0p2 − b_lowQ2_DJANGOH_reco − b_electronFake_reco`

Then run IBU/BBB with the usual signal response.

---

## Low-Q² DJANGOH alpha0p2 background vector

Package:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/lowq2_djangoh_bg_alpha0p2_2026-06-23/background_vector_only_356pb/`

NPZ:
`expected_background_events_lowq2_djangoh7091_alpha0p2_L356p613pb_reco980_charge_major.npz`

Use expected-event yield arrays:
- `yield_hplus`
- `yield_hminus`
- or `bg_events_980` if present

Do **not** use raw `sumw`.

Validated totals:
- h+ = `121.34055790985866`
- h− = `90.37123843074282`
- total = `211.7117963406015`

Normalization:
- target lumi = `356613.0 nb⁻¹`
- MC sample = `djangoh14/7091`
- MC lumi = `4,379,935.0 nb⁻¹`
- scale = `0.08141970143392539`

This low-Q² vector is already normalized consistently.

---

## Scattered-electron fake vector to build

### Definition

For each MC reco track:
- pass same alpha0p2 reco/event selection
- pass same z005 reco binning selection
- require `IsMatchedElectron != 0` only to classify the reco track as electron fake
- fill the 980 reco-bin vector by charge and reco bin

Charge-major ordering:
- bins `0..489`: h+
- bins `490..979`: h−

The fake vector should contain expected reco fake counts at target data luminosity.

### What not to do

Do not:
- veto `IsMatchedElectron == 0` in reco MC selection
- put electron fakes into truth charged-hadron columns
- use raw track counts as a subtraction vector
- use raw `sumw` without luminosity scaling
- infer luminosity from survivor counts
- cap background to data if any bin goes negative

If any reco bin becomes negative after subtraction, stop and diagnose.

---

## MC generator convention

Likely central convention:
- RAPGAP31 is central
- DJANGOH14 is model-variation sidecar

Reason:
- alpha0p2 release uses RAPGAP as nominal response
- `h_R_*_nominal` aliases RAPGAP

Still verify this from release metadata before final approval.

---

## Signal-MC normalization for electron-fake vector

Use target luminosity:

`L_data = 356613.0 nb⁻¹`

Use MC weights:

`w = weight1 * weight2`

Then scale by generated luminosity:

`scale = L_data / L_MC_signal`

Recommended period-wise form:

`b_fake_reco = Σ_period (L_data / L_MC_period) × Σ_rows_in_period w`

Variance:

`σ²_fake = Σ_period (L_data / L_MC_period)² × Σ_rows_in_period w²`

Known aggregate MC lumi file inspected earlier:
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`

Values found:
- RAPGAP31 four-period total: `9,132,891 nb⁻¹`
- DJANGOH14 four-period total: `10,487,495 nb⁻¹`

Caveat:
- that aggregate file has stale `L_data_HERA-II_S67_nb = the superseded pre-rescale luminosity`
- do **not** use that data lumi
- use `356613.0 nb⁻¹`

---

## Implementation recommendation

Start from the alpha0p2 copied migration builder:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py`

That script already:
- applies alpha0p2 reco-side cut
- computes native 490-bin reco indices per charge
- avoids truth-label electron veto
- has response-building machinery

Create a study-local variant that additionally fills:
- `electron_fake_events_980_rapgap31`
- `electron_fake_w2_980_rapgap31`
- optionally `electron_fake_events_980_djangoh14`
- optionally `electron_fake_w2_980_djangoh14`

Use RAPGAP as the central vector if release metadata confirms RAPGAP central.

Use DJANGOH as model/systematic sidecar.

---

## Proposed output root

Use a new explicit study-local release root:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/release_fakevec_lowq2_electron_subtract_20260623/`

Suggested subdirectories:
- `inputs/`
- `fake_vectors/`
- `qa_plots/`
- `copied_scripts/`
- `_run/`
- `audits/`

Suggested sidecars:
- `fake_vectors/electron_fake_vector_rapgap31_reco980.npz`
- `fake_vectors/electron_fake_vector_djangoh14_reco980.npz`
- `fake_vectors/electron_fake_vector_summary.json`
- `inputs/lowq2_bg_vector_audit.json`
- `inputs/electron_fake_vector_audit.json`
- `inputs/total_fake_vector_audit.json`
- `inputs/subtraction_audit.json`
- `inputs/migration_inputs_alpha0p2_fakevec_subtracted.root`
- `qa_plots/fakevec_subtracted_vs_alpha0p2_nobkg.{csv,json,pdf,png}`

---

## Procedure for future agent

### Phase 0 — Safety/context

1. Read:
   - `H1/CONTEXT.md`
   - `sidis_highQ2/context-handoff.md`
   - `sidis_highQ2/docs/RUN_STATUS.md`
2. SSH check:
   `ssh -O check naf-hone.desy.de 2>&1`
3. Check quota:
   `/data/dust/group/h1/.quota-usage.csv`
4. Work only under alpha0p2 study directories. Do not modify central outputs.

### Phase 1 — Build binned electron-fake vector

1. Copy/adapt `build_migration_th2_per_charge_v4.py`.
2. Keep all alpha0p2 reco cuts identical.
3. Add classification:
   - if reco row has `IsMatchedElectron != 0`, fill electron-fake reco vector
4. Fill charge-major 980 vector:
   - h+ first 490 bins
   - h− next 490 bins
5. Fill weighted sums and sumw2:
   - `events_980`
   - `w2_980`
6. Produce RAPGAP central.
7. Produce DJANGOH sidecar if feasible.
8. Validate:
   - length 980
   - finite
   - no negative bins
   - charge sums
   - period/generator sums
   - MC lumi and scale recorded

### Phase 2 — Combine fake vectors

Combine:

`total_fake_980 = lowQ2_DJANGOH_980 + electron_fake_980_RAPGAP`

Variance:

`total_w2_980 = lowQ2_w2_980 + electron_fake_w2_980_RAPGAP`

If any variance component is unavailable, record explicitly:
- central values valid
- variance incomplete
- do not claim full statistical uncertainty

### Phase 3 — Patch response ROOT

1. Copy alpha0p2 response ROOT.
2. Replace data histograms:
   - `h_data_bgsub_pos = h_data_bgsub_pos_original − total_fake_980[0:490]`
   - `h_data_bgsub_neg = h_data_bgsub_neg_original − total_fake_980[490:980]`
3. Replace or update `_w2`:
   - original data `_w2 + total_fake_w2` if variance is valid
   - otherwise leave original `_w2` and document incomplete BG-stat variance
4. Do not touch response matrices.
5. Check:
   - no negative reco bins
   - sums match expectations
   - response matrices unchanged
   - hist bins 1..490 only; do not fill underflow/overflow

### Phase 4 — Run release

Use copied `run_bbb_ibu_release_v4.py`.

Inputs:
- patched response ROOT
- alpha0p2 binmap
- IBU k=4

Validate:
- IBU rows = 490
- BBB rows = 490
- finite = 490
- negative cross sections = 0
- no SVD

### Phase 5 — QA

Compare to alpha0p2 no-bkg release:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/release/`

Produce:
- ratio table by method/charge
- integrated shifts
- per-bin outlier table
- PDF/PNG plots
- tarball for retrieval

Required ratio stats:
- valid/missing
- min
- p10
- median
- mean
- p90
- max
- counts outside:
  - `[0.999, 1.001]`
  - `[0.995, 1.005]`
  - `[0.99, 1.01]`

### Phase 6 — Promotion status

This should initially be labelled:
- diagnostic / central-candidate
- not final central

Remaining final blockers:
- C_rad uncertainty on z005/v6
- electron-fake model systematic
- fake-vector normalization/provenance signoff
- no negative reco bins after subtraction
- closure/response stability if subtraction is sizable

---

## Important caveats

### The old low-Q² subtraction issue

Earlier wrong-normalized low-Q² DJANGOH subtraction used an over-scaled background and produced pathological shifts. Do not reuse:
- old central-base subtracted vector
- old wrong-normalized lowQ² vector
- raw `sumw` as expected counts

Use only the clean BG-only expected-event vector for low-Q².

### The alpha0p2 BG-only diagnostic

A diagnostic subtraction with only low-Q² BG showed tiny integrated shifts:
- IBU h+ integrated shift about `−6.08e−05`
- IBU h− integrated shift about `−4.92e−05`

So low-Q² BG alone is negligible. The important missing component is the surviving scattered-electron fake vector.

### Do not confuse fakes with truth signal

Electron-fake reco tracks should be subtracted from reco data. They should not:
- be truth charged-hadron signal
- enter gen denominator
- define response columns
- be removed via MC-only reco truth veto

---

## Suggested final deliverable summary format

When done, final agent should report:

- PASS/FAIL/RUNNING
- output root
- scripts used
- cluster IDs
- lowQ² BG vector totals
- electron fake vector totals by MC/charge
- total fake vector totals
- subtraction hist sums/min bins
- variance policy
- IBU/BBB validation
- ratio stats vs no-bkg alpha0p2
- QA plot paths
- local copy paths
- remaining caveats/blockers

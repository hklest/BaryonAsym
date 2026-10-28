# Plan: Generator-level MC predictions for SIDIS high-Q² (Sherpa, Pythia6, Pythia8)

## Coverage

This plan covers a new **prediction / theory-comparison** subproject inside the H1 charged-particle SIDIS high-Q² analysis workspace:

- generator-level DIS event generation with **Sherpa**, **Pythia6**, and **Pythia8**
- a shared analysis layer that bins all three generators in the frozen **HQ-v5g-Bprime** canonical binning
- a unified JSON prediction schema for comparison to H1 unfolded / acceptance-corrected / radiative-corrected results
- Condor production, per-job ROOT-ntuple reduction, downstream prediction building, and data-vs-MC plotting

This plan explicitly **does not** re-run H1 detector simulation, H1OO reconstruction, or acceptance/radiative-correction production. The target is **truth-level hadron predictions** directly comparable to the already corrected H1 results in this workspace.

Predictions are produced in two nominal selections: **full phase space** (no track cuts) and **wide gen fiducial** (`|η_gen| < 3.5`, `pT_gen > 0.05`). The fid selection matches the H1 high-Q² unfolding target; the fullps version quantifies the extrapolation implicit in the published numbers.

**Plan status:** implementation-ready except for the pilot-blocking and full-stat-blocking open items listed under `## Open items`.

**Annotation cycle iteration:** 2

**Research / source inputs used:**
- `H1/CONTEXT.md`
- `H1/sidis_highQ2/context-handoff.md`
- `H1/docs/shared-species-definition-option-h-2026-05-22.md`
- `H1/sidis_highQ2/docs/references/reference-sidis-frame-gamma-star-p.md`
- `H1/sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md`
- `H1/sidis_highQ2/docs/data/binning_v5f_frozen.json` (used only as a path anchor; implementation must load the frozen canonical B′ content from this file path and validate its tag/edges)
- `H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json`
- `H1/sidis_highQ2/data/unfolded_4d_v3_IBU.json`
- `H1/sidis_highQ2/data/v5g_4d_trust_mask.json`
- `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md`
- `H1/sidis_highQ2/MC/smoke_q2/smoke_test_2026-05-26.md`

## Requirements Summary

### Confirmed requirements

- Create a new local plan file at:
  - `H1/sidis_highQ2/docs/plans/plan-MC-predictions-sherpa-pythia6-pythia8.md`
- The plan must include, but not execute yet, creation of the local directory:
  - `H1/sidis_highQ2/MC/`
- The plan must also include the NAF-side directory:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/`
- Required subdirectories under both local and NAF MC roots:
  - `sherpa/`
  - `pythia6/`
  - `pythia8/`
  - `common/`
  - `scripts/`
  - `predictions_raw/`
  - `predictions/`
  - `plots/`
- This is a **generator-level** theory-prediction project only.
- Beam setup is HERA-II, head-on, with:
  - `E_e = 27.6 GeV`
  - `E_p = 920 GeV`
- The implementation must support both lepton charges where needed:
  - `e+ p` and `e- p`
- For charge-summed observables (`σ`, `M`) the prediction must use the same per-period luminosity weighting policy as the analysis.
- Canonical binning is frozen as **Scheme B′ / HQ-v5g-Bprime** with 8 `(Q²,x)` cells, 6 `z` bins, 5 `pT_h` bins, 16 `φ_h` bins, and charge-separated hadrons.
- All generators must read the same authoritative binning file through a shared Python loader:
  - `H1/sidis_highQ2/docs/data/binning_v5f_frozen.json`
  - shared implementation anchor: `MC/common/binning.py`
- Event-level generator cuts to apply:
  - `Q² > 150 GeV²`
  - `0.1 < y < 0.7`
  - `35 < (E - p_z) < 70 GeV`
  - skip `|VtxZ| < 35 cm` at pure truth level, but document this assumption
  - truth-level scattered electron angular cut: `4° < θ_e < 154°`
- Track-level truth handling:
  - default accepted hadron sample is **full phase space** after species selection only
  - a second nominal derived selection is the wide gen fiducial `|η_gen| < 3.5` and `pT_gen > 0.05 GeV`
  - species pool `|PDG| in {211,321,2212}` only, with the gen-level long-lived-wall charged-hadron definition
  - both `fullps` and `fid` predictions are built from the same per-job ROOT hadron ntuples; no second generation pass is needed
- Hadron observables must mirror the canonical extractor conventions exactly:
  - construct `q = k - k'` in the lab frame
  - boost to the `γ*+p` CM using per-event proton 4-momentum convention
  - define `pT_h` in CM relative to boosted `q`
  - define `z = (P·p_h)/(P·q)` and note its Lorentz invariance
  - define `φ_h` in Trento convention
- Required outputs per generator:
  - `d^4σ^{h±}/(dQ² dx dz dpT_h)`
  - multiplicities `M^{h±}`
  - hadron charge asymmetry `A_h`
  - azimuthal moments `<cos φ_h>`, `<cos 2φ_h>`, `<sin φ_h>`, `<sin 2φ_h>`
  - optional raw `φ_h` histograms
  - `A_BC` deferred to stretch goal, but hooks required in v1
- Default QED-radiation policy for theory predictions:
  - **QED ISR/FSR on the leptonic line OFF** (`rad_qed = false`)
  - rationale: data being compared are already radiative-corrected
  - rad-on variant retained as deferred cross-check
- Unified JSON schema and axis order are required.
- Unified parser / plotter required at:
  - `MC/scripts/compare_data_vs_mc.py`
- Production policy must obey NAF constraints:
  - login node only for tiny single-file / ≤5 minute sanity checks
  - all real generation on Condor standard jobs
  - `RequestCpus = 1`, `RequestMemory = 2000`, `+RequestRuntime = 10800`
- Statistical planning must estimate event counts such that MC statistical uncertainty in a typical populated cell is ≤ 5% of the corresponding data statistical uncertainty.
- Planned phases are fixed as bootstrap, phased pilots, full statistics, comparison memo, and deferred systematics.
- Per phase, the plan must specify dispatches and specialist agents.

### Inferred assumptions

- The authoritative **physics** binning is the B′ structure embedded in the referenced JSON path, even though the filename still says `v5f_frozen`; `MC/common/binning.py` must therefore validate the loaded content against the expected B′ edges and tag rather than trusting the filename.
- The existing high-Q² data JSONs are sufficient as the comparison baseline for v1; no new data-side extraction is in scope here.
- Since this is theory-only truth generation, the local `MC/` tree is primarily for scripts, schemas, cards, and lightweight outputs, while bulk generation artifacts live on NAF DUST.
- The final JSON should be produced separately for `e+ p` and `e- p` runs and then combined with explicit luminosity weighting for charge-summed observables, rather than trying to emulate mixed running in one generator stream.
- Per-job persistent output should be a **slim ROOT ntuple**, not JSON or HepMC, so later rebinning and new-observable studies remain possible without regenerating events.
- Both nominal selections, `fullps` and `fid`, must be derived from the same hadron ntuples by post-hoc selection on stored branches.
- Because `xsec_v3_HQv5gBprime_2026-05-24.json` still carries an absolute-normalization caveat, comparison plots should visibly separate normalized-shape checks from absolute-rate checks until normalization status is re-confirmed.

### Acceptance criteria

The subproject is ready for implementation when the following are unambiguous:

1. A concrete directory and file layout exists for local and NAF MC work.
2. Each generator has an explicit installation / discovery path, run-card strategy, Condor dispatch model, and converter path to the unified schema.
3. A single shared binning/observable implementation is defined for all generators.
4. A single shared species-selection implementation is defined for all generators, using a simple generator-level long-lived-wall rule plus a common ancestor-veto selector.
5. A single per-job ROOT ntuple format is defined for all generators and preserves enough information to rebuild observables, rebin later, and derive both `fullps` and `fid` predictions from the same events.
6. The unified schema includes enough additive information to merge job outputs exactly and regenerate the final observable JSON.
7. The phase gates define what must pass before moving to the next phase, including species and frame validation gates.
8. Open decisions that require factual verification are isolated as research dispatches rather than hidden in the execution work.

## Species Definition — gen-level primary π/K/p with long-lived wall

**Background reference:** `H1/docs/shared-species-definition-option-h-2026-05-22.md`

For these **generator-level** predictions, the operational species selection is:

> final-state `π±/K±/p/p̄` whose decay-chain ancestry contains **no long-lived parent** with `cτ ≳ 10 mm`.

The long-lived parent list to veto is:
- `K0_S` (`310`)
- `K0_L` (`130`)
- `Λ` / `Λ̄` (`±3122`)
- `Σ+` / `Σ−` (`±3222`, `±3112`)
- `Ξ0` / `Ξ̄0` (`±3322`)
- `Ξ−` / `Ξ̄+` (`±3312`)
- `Ω−` / `Ω̄+` (`±3334`)

All shorter-lived states must remain at normal generator defaults and therefore **must decay** so that their `π/K/p` daughters appear in the final state and are counted. This includes:
- strong short-lived hadrons such as `ρ`, `K*`, `Δ`, `Σ*`, `Ξ*`, `η`, `ω`, `φ`
- charm hadrons such as `D0`, `D±`, `Ds`, `Λc`
- bottom hadrons such as `B±`, `B0`, `Bs`

This is the **gen-level analog** of the H1OO Option-H charged-hadron definition used by DJANGOH and RAPGAP. We do **not** try to reproduce detector-level GTR/STR bookkeeping, `ParticleSource`, or `Mother2 == -1` logic here; those are H1OO infrastructure details, not generator-level truth rules. We enforce the gen-level rule by two switches:
1. generator decay-table settings that make the long-lived particles stable
2. a Python ancestor-walk in `MC/common/option_h_selector.py` as a redundant check

If any MC prediction in this study uses a different species definition, the data-vs-MC comparison is invalid.

### Long-lived wall to enforce

| Parent | PDG | Rule |
|---|---:|---|
| `K0_S` | `310` | must remain undecayed |
| `K0_L` | `130` | must remain undecayed |
| `Λ` / `Λ̄` | `±3122` | must remain undecayed |
| `Σ+` / `Σ−` and antiparticles | `±3222`, `±3112` | must remain undecayed |
| `Ξ0` / `Ξ−` and antiparticles | `±3322`, `±3312` | must remain undecayed |
| `Ω−` and antiparticle | `±3334` | must remain undecayed |

### Short-lived states that must still decay

| Category | Rule |
|---|---|
| strong resonances (`ρ`, `K*`, `Δ`, `Σ*`, `Ξ*`, `η`, `ω`, `φ`, …) | must decay |
| charm hadrons (`D0`, `D±`, `Ds`, `Λc`, …) | must decay |
| bottom hadrons (`B±`, `B0`, `Bs`, …) | must decay |

Caution:
- **do not globally disable hadron decays**
- doing so would strand short-lived parents in the event record and undercount final-state `π/K/p`

## Beam-axis convention: reducer-side R_y(180°)

H1 / HERA convention is:
- incoming electron along `−z`
- incoming proton along `+z`

All three generators were verified empirically to default to the **flipped** orientation, i.e. electron at `+z` and proton at `−z`. Evidence comes from:
- `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md` §1.1
- `H1/sidis_highQ2/MC/smoke_q2/smoke_test_2026-05-26.md` §1

Verified defaults:
- **Pythia8 8.317** with `Beams:idA = 11`, `Beams:idB = 2212`: electron at `+z`, proton at `−z`
- **Sherpa 3.0.3** with `BEAMS: [11, 2212]`: electron at `+z`, proton at `−z`
- **Pythia6 6.4.29.2** in its default CMS-mode setup: electron at `+z`, proton at `−z`

Sherpa 3.0.3 hard-rejects the generator-side beam swap (`BEAMS: [2212, 11]`), so a per-generator native-orientation strategy is not even available. The reducer-side fix is therefore forced.

### Decision

Apply a **180° rotation about ŷ** to every particle 4-vector inside `MC/scripts/reduce_hepmc_to_root.py` before filling the ROOT trees.

Generators run in their natural FLIPPED convention; ROOT trees on disk are always **H1 convention** by construction.

Exact transformation:

```python
(E, px, py, pz) -> (E, -px, py, -pz)
```

Apply this to:
- all final-state particles
- the scattered lepton
- the beam particles

for every event in the file whenever the file-level orientation check indicates a flipped beam setup.

### Algebra summary

The review memo shows that `Q²`, `y`, `x`, `W²`, and invariant `z` are unchanged because they are Lorentz invariants. `η_lab` flips sign and `φ_lab -> π - φ_lab`. CM-frame `pT_h` is invariant. Trento `φ_h` is preserved exactly because `R_y(180°)` is a **proper rotation** (`det = +1`) and Trento `φ_h` is built from triple products of CM-frame 3-vectors that all transform uniformly under a proper rotation. See `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md` §1.2(e) for the explicit derivation.

### Detection rule

The reducer reads event 0's beam particles to decide whether to apply the rotation:
- if `pz(proton) < 0`, apply `R_y(180°)` to the whole file
- otherwise pass through unchanged

Required metadata propagation:
- per-job ROOT file metadata field: `beam_orientation_correction_applied: bool`

### Closure test requirement

`MC/common/observables.py:__main__` must include a closure/unit test that proves `R_y(180°)` preserves:
- `Q²`
- `y`
- `x`
- `z`
- CM-frame `pT_h`
- Trento `φ_h`

between native-HERA and rotated-FLIPPED inputs within `1e-9`.

Status from the review findings: this analytic closure test is **already implemented and passing (6/6 PASS)**.

## Approach

Build the MC-prediction subproject as a **shared truth-analysis framework with generator-specific front ends**:

1. **One shared physics layer** in `MC/common/` loads the frozen binning, computes DIS and hadron observables with the exact CM-frame / Trento conventions of the canonical SIDIS extractor, applies the gen-level long-lived-wall species policy and truth-level cuts, and validates/serializes the unified schema.
2. **Three generator adapters** in `MC/sherpa/`, `MC/pythia6/`, and `MC/pythia8/` are responsible only for:
   - environment / build setup
   - steering card generation
   - running the generator on Condor
   - exporting per-job event records to HepMC (preferred) or a converter-friendly native format
3. **One per-job reduction step** converts each event file into a slim ROOT ntuple with per-event and per-hadron TTrees using the shared common layer.
4. **One prediction-building step** reads per-job ROOT ntuples directly, accumulates additive histograms in memory, and writes the final `predictions/<generator>_<tag>_<selection>.json` outputs.
5. **One skim-building step** writes a merged per-generator hadron skim ROOT for future rebinning and new-observable studies.
6. **One comparison script** overlays data and MC with the same trust mask and bin ordering used by the canonical analysis outputs.

This structure minimizes generator-specific duplication and is the safest way to guarantee that differences between Sherpa, Pythia6, and Pythia8 reflect generator physics rather than divergent binning or angle conventions.

## Rationale and key decisions

### 1. Born-level QED default

The plan freezes **QED radiation off on the leptonic line** for the nominal prediction set.

Rationale:
- the H1 data products for comparison are already corrected by `C_rad`
- a radiative generator prediction would mix a model of leptonic QED radiation back into a radiative-corrected comparison target
- the cleanest nominal theory comparison is therefore Born-level lepton kinematics

Deferred cross-check:
- keep a `rad_qed=true` steering variant for Phase 4 systematic studies

### 2. Shared CM-frame observable implementation

The plan requires a single shared implementation of:
- `q = k - k'`
- hadronic-CM boost using `W = q + P`
- CM-frame `pT_h`
- Trento `φ_h`
- invariant `z`

Rationale:
- the 2026-05-24 frame-fix memo established that lab-frame shortcuts are wrong at HERA-II asymmetric beams
- if each generator reimplements the frame logic independently, silent convention drift is likely
- a shared `MC/common/observables.py` plus a tiny cross-check against the canonical extractor is therefore mandatory

### 3. ROOT-tree per-job output; histograms and prediction JSON are downstream derived products

Each job will produce:
- a transient raw event file (`.hepmc3` preferred) only long enough to reduce it
- one slim ROOT file with `events` and `hadrons` TTrees as the persistent merge unit

Merging does **not** happen at the HepMC level. Prediction histograms and final JSONs are built downstream by reading the per-job ROOT files directly.

Rationale:
- HepMC3 ASCII is too bulky for current DUST headroom
- slim ROOT TTrees preserve per-hadron information needed for rebinning and future observable studies
- ROOT trees are far more storage-efficient than HepMC ASCII or per-event JSON
- HepMC can be deleted immediately after successful ROOT reduction and validation

### 4. Two-stage output model: per-job ROOT then derived products

Stage A:
- generator writes HepMC/native event record
- reducer converts to one per-job ROOT ntuple
- HepMC is deleted on success unless `--keep-hepmc` is requested for audit

Stage B:
- analysis scripts read the per-job ROOTs
- accumulate additive histograms in memory
- write final prediction JSONs for both `fullps` and `fid`
- optionally produce one merged skim ROOT per generator as the canonical rebinning artifact

Rationale:
- derived quantities like asymmetries and moments must be recomputed after reading the merged event content, not averaged job-by-job
- the ROOT ntuple keeps enough information to change binning, redo selections, and build additional observables later without regenerating events

## File and directory plan

### Local workspace paths to create later

- `H1/sidis_highQ2/MC/`
- `H1/sidis_highQ2/MC/common/`
- `H1/sidis_highQ2/MC/scripts/`
- `H1/sidis_highQ2/MC/sherpa/`
- `H1/sidis_highQ2/MC/pythia6/`
- `H1/sidis_highQ2/MC/pythia8/`
- `H1/sidis_highQ2/MC/predictions_raw/`
- `H1/sidis_highQ2/MC/predictions/`
- `H1/sidis_highQ2/MC/plots/`

### NAF DUST paths to create later

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/common/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/scripts/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/sherpa/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/pythia6/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/pythia8/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/predictions_raw/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/predictions/`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/plots/`

### Planned file roles

- `MC/common/binning.py`
  - loads `docs/data/binning_v5f_frozen.json`
  - validates that the loaded content matches canonical `HQ-v5g-Bprime` edges
  - exposes axis-edge arrays and `(Q²,x)` cell lookup
- `MC/common/observables.py`
  - computes DIS kinematics, hadron `z`, `pT_h`, `φ_h`, beam-axis rotation closure checks, full-phase-space vs wide-gen-fiducial flags, and charge/species classification
- `MC/common/option_h_selector.py`
  - single source of truth for gen-level primary `π/K/p` selection on HepMC/native event records
  - selects final-state `π/K/p` and vetoes any accepted candidate with excluded long-lived ancestor PDGs
  - acts as the second line of defense after generator-side decay-table lockdown
- `MC/common/schema.py`
  - schema constants, validators, final serializer, and `build_final_prediction(selection=...)`
- `MC/common/sanity_test.py`
  - tiny 1000-event end-to-end smoke for all three generators using one-file / ≤5 minute login-node allowance only
  - includes species-selector validation outputs persisted as `MC/common/sanity_test_results_<generator>_<date>.json`
- `MC/scripts/run_generator_job.py`
  - unified per-job driver wrapping generator execution + ROOT reduction
- `MC/scripts/reduce_hepmc_to_root.py`
  - detects beam orientation from event 0, conditionally applies reducer-side `R_y(180°)`, exposes `--expected-orientation {auto,hera,flipped}` for testing, converts HepMC/native output to one slim ROOT ntuple with `events` and `hadrons` TTrees using the shared common layer, and deletes HepMC on success unless `--keep-hepmc` is passed
- `MC/scripts/build_prediction_from_root.py`
  - reads per-job ROOT ntuples with uproot, accumulates histograms, and writes final generator JSONs for both `fullps` and `fid`
- `MC/scripts/skim_predictions_root.py`
  - concatenates per-job ROOT outputs into one merged per-generator hadron skim ROOT for future rebinning and new-observable studies
- `MC/scripts/compare_data_vs_mc.py`
  - loads data JSONs plus both `fullps` and `fid` prediction JSONs, applies trust mask, and makes overlays
- `MC/scripts/make_condor_submit.py`
  - materializes generator-specific Condor submit and wrapper files
- `MC/scripts/compare_fid_vs_fullps.py`
  - produces per-generator fid-vs-fullps comparison plots and a per-cell CSV summary
- `MC/sherpa/Sherpa.yaml`
  - Sherpa 3 YAML run card template
- `MC/pythia6/driver/pythia6_dis_driver.f90` (or `.f`)
  - Pythia6 DIS driver
- `MC/pythia6/driver/pythia6_to_hepmc3.cxx`
  - Pythia6-to-HepMC3 conversion wrapper
- `MC/pythia8/cmnd/pythia8_dis_nominal.cmnd`
  - nominal Pythia8 command file
- `MC/predictions_raw/<generator>/`
  - transient per-job ROOT ntuples
- `MC/predictions/<generator>_<tag>_fullps.json`
- `MC/predictions/<generator>_<tag>_fid.json`
- `MC/predictions/<generator>_<tag>_hadrons.root`
- `MC/plots/data_vs_mc_v1.pdf`

## Physics definitions to freeze in implementation

### Beam and period weighting

Use HERA-II beam energies:

```python
E_ELEC_GEV = 27.6
E_PROT_GEV = 920.0
M_PROT_GEV = 0.9382720813
```

For nominal charge-summed observables, combine beam-charge-specific predictions using the same period-weighted lumi mix as the analysis handoff:

```python
PERIOD_LUMI_NB = {
    "ep0304": 53706.0,
    "em0405": 105028.0,
    "em06": 57573.0,
    "ep0607": 140306.0,
}
LUMI_EP = PERIOD_LUMI_NB["ep0304"] + PERIOD_LUMI_NB["ep0607"]
LUMI_EM = PERIOD_LUMI_NB["em0405"] + PERIOD_LUMI_NB["em06"]
LUMI_TOTAL = LUMI_EP + LUMI_EM
WEIGHT_EP = LUMI_EP / LUMI_TOTAL
WEIGHT_EM = LUMI_EM / LUMI_TOTAL
```

Numerically this implies:
- `LUMI_EP = 194012 nb^-1`
- `LUMI_EM = 162601 nb^-1`
- `LUMI_TOTAL = 356613 nb^-1`
- `WEIGHT_EP ≈ 0.5440`
- `WEIGHT_EM ≈ 0.4560`

The plan keeps the user-provided overall `the superseded pre-rescale luminosity nb^-1` as the broader analysis-context number from `CONTEXT.md`, but for this subproject the implementation should read and apply the **high-Q² per-period S67 values from the high-Q² handoff**, because those are the values actually attached to the current analysis products.

### Event-level truth cuts

Apply at generator-analysis time:

```python
def pass_event_cuts(ev):
    return (
        ev.Q2 > 150.0 and
        0.1 < ev.y < 0.7 and
        35.0 < ev.empz < 70.0 and
        4.0 < ev.theta_e_deg < 154.0
    )
```

Documented truth-level omission:
- `|VtxZ| < 35 cm` is skipped because pure generator records have no detector vertex reconstruction analogue

`empz` at truth level must be computed only after any required reducer-side `R_y(180°)` correction, so that the event selection is evaluated in H1 convention.

### Track-level definition: full phase space (default) and wide-gen fiducial (audit)

```python
ALLOWED_ABS_PDG = {211, 321, 2212}
OPTION_H_EXCLUDED_PARENTS = {3122, 310, 130, 3222, 3112, 3312, 3322, 3334}


def classify_hadron(pdg):
    if abs(pdg) not in ALLOWED_ABS_PDG:
        return None
    return 0 if pdg > 0 else 1   # 0=h+, 1=h-


def is_fullps_hadron(track_lab):
    return True


def is_fid_gen_hadron(track_lab):
    return abs(track_lab.eta) < 3.5 and track_lab.pt > 0.05
```

The species cut is mandatory and is enforced separately by the generator decay-table lockdown plus `MC/common/option_h_selector.py`.

The `η/pT` choice is **post-hoc**, not a generation-time cut:
- `fullps`: all accepted hadrons after species selection
- `fid`: accepted hadrons with `|η_gen| < 3.5` and `pT_gen > 0.05 GeV`

Both selections are derived from the **same per-job ROOT hadron tree**; no additional event generation is needed.

Species and track/frame are separate enforcement axes:
- **species axis:** gen-level primary `π/K/p` with no long-lived ancestor
- **track axis:** full phase space vs wide gen fiducial, applied post-hoc from the same ROOTs
- **frame axis:** `γ*+p` CM + Trento convention matching to the canonical extractor

Both species and frame axes must pass before any plotting is allowed. Both track selections are reported.

### Common selector requirement

All converters must import a single shared selector module:

```python
def select_option_h_hadrons(event: HepMCEvent) -> list[Particle]:
    """Return final-state π±/K±/p/p̄ that pass the gen-level long-lived-wall rule.

    Implementation:
      1. Walk all particles with HepMC status == 1 (final state).
      2. Require |PDG| in {211, 321, 2212}.
      3. Walk ancestors; if any ancestor has |PDG| in OPTION_H_EXCLUDED_PARENTS,
         reject the particle.
      4. Otherwise accept.
    """
```

Implementation notes to freeze in the plan:
- excluded ancestor PDGs: `{3122, 310, 130, 3222, 3112, 3312, 3322, 3334}` and charge conjugates
- ancestor-PDG veto is the required minimum species check
- short-lived strong, charm, and bottom ancestors are **allowed** and expected
- composition fractions for charm/resonance/bottom may be recorded for transparency but are not gating quantities

### CM-frame hadron observables

The implementation must mirror the canonical frame-fix logic, not the old lab-frame shortcut.

Planned shared implementation anchor:

```python
import numpy as np


def compute_q(k_in, k_out):
    return k_in - k_out


def compute_z(P, p_h, q):
    return dot4(P, p_h) / dot4(P, q)


def boost_to_gamma_p_cm(P, q, *vecs):
    W = P + q
    beta = W.spatial / W.E
    return [lorentz_boost(v, -beta) for v in vecs]


def compute_pth_phi_trento_cm(k_in, k_out, P, p_h):
    q = compute_q(k_in, k_out)
    k_in_cm, k_out_cm, q_cm, p_h_cm = boost_to_gamma_p_cm(P, q, k_in, k_out, q, p_h)

    qhat = unit(q_cm.spatial)
    p3 = p_h_cm.spatial
    pth = np.linalg.norm(p3 - np.dot(p3, qhat) * qhat)

    l = np.cross(k_in_cm.spatial, k_out_cm.spatial)
    h = np.cross(q_cm.spatial, p_h_cm.spatial)
    cosphi = np.dot(l, h) / (np.linalg.norm(l) * np.linalg.norm(h))
    sinphi = np.dot(np.cross(l, h), qhat) / (np.linalg.norm(l) * np.linalg.norm(h))
    phi = np.arctan2(sinphi, cosphi)
    return pth, phi
```

Implementation note:
- `z = (P·p_h)/(P·q)` is Lorentz-invariant and may be evaluated before or after the boost
- the proton four-vector must follow the analysis convention using per-event proton momentum input, even if numerically fixed to 920 GeV for these generators

## Per-job ROOT output format

Each Condor job writes one slim ROOT file:

- `<generator>_<beam_mode>_seed<seed>.root`

with two TTrees and top-level metadata objects.

### TTree `events`

One entry per DIS event that passes event-level cuts.

Branches:
- `run`, `event` — `ULong64_t`
- `seed` — `Int_t`
- `beam_mode` — `Char_t` (`'p'` for ep, `'m'` for em)
- `weight` — `Float_t`
- `Q2_true`, `x_true`, `y_true`, `W2_true`, `nu_true` — `Float_t`
- `empz_true` — `Float_t`
- `theta_e_deg` — `Float_t`
- `beam_electron_pz_after_rotation` — `Float_t`
- `beam_proton_pz_after_rotation` — `Float_t`
- `n_hadrons_total` — `Int_t`
- `n_hadrons_fiducial` — `Int_t`
- `passed_event_cuts` — `Bool_t` (always true for written entries)

### TTree `hadrons`

One entry per accepted hadron before any post-hoc track selection.

Branches:
- `ievent` — `ULong64_t`
- `seed` — `Int_t`
- `beam_mode` — `Char_t`
- `weight` — `Float_t`
- `pdg` — `Int_t`
- `charge` — `Int_t`
- `eta_lab` — `Float_t`
- `pt_lab` — `Float_t`
- `phi_lab` — `Float_t`
- `p_lab` — `Float_t`
- `z` — `Float_t`
- `pth_cm` — `Float_t`
- `phi_h_trento` — `Float_t`
- `Q2_true` — `Float_t`
- `x_true` — `Float_t`
- `q2x_cell` — `Int_t`
- `z_bin` — `Int_t`
- `pth_bin` — `Int_t`
- `phi_bin` — `Int_t`
- `excluded_ancestor_pdg` — `Int_t` (`0` in nominal output; nonzero means the decay-table lockdown failed)
- `fiducial_eta_pt` — `Bool_t` (`|η_lab| < 3.5` and `pt_lab > 0.05`)

### File-level metadata

Store as `TParameter` / `TNamed` objects:
- `generator`
- `generator_family`
- `beam_mode`
- `seed`
- `n_generated`
- `n_pass_event_cuts`
- `sigma_gen_pb`
- `sigma_gen_err_pb`
- `rad_qed`
- `pdf_set`
- `tune`
- `hadronization`
- `binning_tag`
- `binning_sha`
- `species_convention`
- `mayDecay_off_pdgs`
- `selector_module`
- `selector_sha`
- `generator_env`
- `condor_cluster`
- `condor_proc`
- `host`
- `utc`
- `beam_orientation_correction_applied`

Compression / write policy:
- ROOT default `LZ4` is acceptable; `ZSTD` level 5 is also acceptable if available
- use modest `autobasket` / `autoflush` defaults suitable for slim TTrees

### Read example

Example using uproot to count positive hadrons in cell 0, `z_bin=2`, `pth_bin=3`, under the fiducial selection:

```python
import uproot
import awkward as ak

with uproot.open("H1/sidis_highQ2/MC/predictions_raw/pythia8/pythia8_ep_seed10042.root") as f:
    h = f["hadrons"].arrays([
        "charge", "q2x_cell", "z_bin", "pth_bin", "fiducial_eta_pt", "weight"
    ])
    mask = (
        (h["charge"] == 1) &
        (h["q2x_cell"] == 0) &
        (h["z_bin"] == 2) &
        (h["pth_bin"] == 3) &
        (h["fiducial_eta_pt"] == True)
    )
    n_hplus = ak.sum(h["weight"][mask])
```

### Observable numerators / denominators

`MC/scripts/reduce_hepmc_to_root.py` must, per file:
- inspect event 0 beam particles and set `beam_orientation_correction_applied`
- honor CLI flag `--expected-orientation {auto,hera,flipped}` with default `auto`
- if the incoming proton has `p_z < 0`, apply `R_y(180°)` to every particle 4-vector before computing any kinematics
- after correction, assert for every event that `p_z(electron) < 0` and `p_z(proton) > 0`; abort the job if not
- compute `Empz`, event cuts, and hadron observables only after any needed rotation
- fill the event-audit branches `beam_electron_pz_after_rotation` and `beam_proton_pz_after_rotation`

Current implementation status remains as inherited from the earlier plan: the reducer body is still `NotImplementedError` in the codebase and lands in Phase 1a.

`MC/scripts/build_prediction_from_root.py` must read per-job ROOTs with uproot and accumulate additive arrays in memory for:

```python
summary = {
    "n_dis": np.zeros((8, 2), dtype=float),
    "n_h": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "sum_w": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "sum_w2": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "sum_cos1": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "sum_cos2": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "sum_sin1": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "sum_sin2": np.zeros((8, 6, 5, 2, 2), dtype=float),
    "phi_hist": np.zeros((8, 6, 5, 2, 2, 16), dtype=float),
}
```

Axis meaning above:
- `(cell8, z6, pTh5, had_charge2, beam_charge2)`
- beam-charge index example:
  - `0 = e+ p`
  - `1 = e- p`

Final public JSON collapses or combines the beam-charge axis as appropriate.

## Unified output schema

### Final prediction JSON

Each merged prediction file will live at:

- `MC/predictions/<generator>_<tag>.json`
- NAF mirror: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/predictions/<generator>_<tag>.json`

Planned schema:

```json
{
  "schema_version": "sidis_mc_prediction_v1",
  "generator": "sherpa-3.0.3",
  "generator_family": "sherpa",
  "pdf": "NNPDF31_lo_as_0118",
  "tune": "default",
  "rad_qed": false,
  "hadronization": "default",
  "beam_orientation_correction_applied": true,
  "beams": {
    "e_energy_GeV": 27.6,
    "p_energy_GeV": 920.0,
    "beam_modes": ["ep", "em"]
  },
  "period_lumi_nb": {
    "ep0304": 53706.0,
    "em0405": 105028.0,
    "em06": 57573.0,
    "ep0607": 140306.0
  },
  "effective_lumi_weights": {
    "ep": 0.5440371498820849,
    "em": 0.4559628501179151
  },
  "n_events": {
    "ep": 5000000,
    "em": 5000000,
    "total": 10000000
  },
  "lumi_equiv_nb": {
    "ep": 123456.7,
    "em": 123210.4,
    "combined": 246667.1
  },
  "selection": "fullps",
  "binning_tag": "HQ-v5g-Bprime",
  "binning_source": "H1/sidis_highQ2/docs/data/binning_v5f_frozen.json",
  "binning_sha": "<sha256>",
  "axis_order": ["q2x", "z", "pth", "charge"],
  "charge_index": {"0": "h+", "1": "h-"},
  "species_definition": {
    "convention": "gen-level primary π/K/p (long-lived wall at cτ ≳ 10 mm)",
    "reference": "H1/docs/shared-species-definition-option-h-2026-05-22.md",
    "included_pdgs": [211, -211, 321, -321, 2212, -2212],
    "excluded_ancestors": [3122, -3122, 310, 130, 3222, -3222, 3112, -3112, 3312, -3312, 3322, -3322, 3334, -3334],
    "mayDecay_off_pdgs": [3122, 310, 130, 3222, 3112, 3312, 3322, 3334],
    "selector_module": "MC/common/option_h_selector.py",
    "selector_sha": "<git sha>"
  },
  "composition_validation": {
    "frac_lambda_ancestor": 0.0,
    "frac_k0s_ancestor": 0.0,
    "frac_klong_ancestor": 0.0,
    "frac_sigma_pm_ancestor": 0.0,
    "frac_xi_ancestor": 0.0,
    "frac_omega_ancestor": 0.0,
    "frac_resonance_decay": 0.0,
    "frac_charm_decay": 0.0,
    "frac_bottom_decay": 0.0
  },
  "observables": {
    "xsec_4d": {
      "shape": [8, 6, 5, 2],
      "units": "nb/GeV^2/GeV",
      "value_nb_per_GeV2_per_GeV": [[[[0.0]]]],
      "stat_err": [[[[0.0]]]]
    },
    "multiplicity_4d": {
      "shape": [8, 6, 5, 2],
      "value": [[[[0.0]]]],
      "stat_err": [[[[0.0]]]]
    },
    "A_h_4d": {
      "shape": [8, 6, 5],
      "value": [[[0.0]]],
      "stat_err": [[[0.0]]]
    },
    "cos_phi_h": {
      "shape": [8, 6, 5, 2],
      "value": [[[[0.0]]]],
      "stat_err": [[[[0.0]]]]
    },
    "cos_2phi_h": {
      "shape": [8, 6, 5, 2],
      "value": [[[[0.0]]]],
      "stat_err": [[[[0.0]]]]
    },
    "sin_phi_h": {
      "shape": [8, 6, 5, 2],
      "value": [[[[0.0]]]],
      "stat_err": [[[[0.0]]]]
    },
    "sin_2phi_h": {
      "shape": [8, 6, 5, 2],
      "value": [[[[0.0]]]],
      "stat_err": [[[[0.0]]]]
    },
    "phi_h_hist": {
      "optional": true,
      "shape": [8, 6, 5, 2, 16],
      "value": [[[[[0.0]]]]],
      "stat_err": [[[[[0.0]]]]]
    }
  },
  "derived_from": {
    "summary_merge": true,
    "beam_charge_combination": "lumi_weighted_for_sigma_and_M; separate_ep_em_available_in_internal_summary"
  },
  "provenance": {
    "git_sha": "<repo sha>",
    "run_date": "2026-05-26T00:00:00Z",
    "run_card_sha": "<sha256>",
    "host": "naf-hone.desy.de",
    "condor_clusters": [1234567, 1234568],
    "generator_env": "LCG_109a/x86_64-el9-gcc14-opt"
  }
}
```

### Earlier additive-summary JSON draft — superseded

An earlier draft of this plan used per-job additive JSON summaries as the merge unit. That design is superseded. The persistent merge unit is now the per-job ROOT ntuple. The final serializer in `MC/common/schema.py` derives `xsec`, `M`, `A_h`, and moments after reading the per-job ROOTs and accumulating histograms in memory.

Schema enforcement rules to freeze in implementation:
- `species_definition.convention` must equal exactly `"gen-level primary π/K/p (long-lived wall at cτ ≳ 10 mm)"`
- `composition_validation.frac_lambda_ancestor` must be exactly `0.0`
- `composition_validation.frac_k0s_ancestor` must be exactly `0.0`
- `composition_validation.frac_klong_ancestor` must be exactly `0.0`
- `composition_validation.frac_sigma_pm_ancestor` must be exactly `0.0`
- `composition_validation.frac_xi_ancestor` must be exactly `0.0`
- `composition_validation.frac_omega_ancestor` must be exactly `0.0`
- `MC/scripts/compare_data_vs_mc.py` must refuse to plot any prediction JSON that fails either species-convention or zero-long-lived-ancestor checks
- `frac_resonance_decay`, `frac_charm_decay`, and `frac_bottom_decay` are informational only and must not be used as plot-blocking gates

## Generator-specific implementation plan

## Sherpa

Citation anchor for this section:
- review memo: `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md`
- smoke test: `H1/sidis_highQ2/MC/smoke_q2/smoke_test_2026-05-26.md`
- smoke verdict: `σ_gen ≈ 4445.72 ± 38.49 pb` at `Q² > 150 GeV²`; Q² shape agrees with Pythia8 within a factor of about 2; run succeeds cleanly, but the hadron pilot remains blocked because the smoke card intentionally omitted the stable-particle decay lockdown.

### Target

- Preferred version: **Sherpa 3.0.3** on NAF LCG/CVMFS
- DIS steering via **Sherpa 3 YAML** only

### Environment discovery

First implementation dispatch must verify on NAF, via non-production environment checks:

```bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_*/x86_64-el9-gcc*/setup.sh
module avail sherpa
which Sherpa
Sherpa -v
```

If packaged Sherpa is absent, build under:
- local metadata path: `H1/sidis_highQ2/MC/sherpa/build-notes.md` (planned only; no implementation here)
- NAF build path: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/sherpa/build/`

### One-time build fallback recipe

```bash
./configure --prefix="$PWD/install" --enable-hepmc3 --enable-lhapdf
make -j4
make install
```

If build fallback is needed, it must be done via a **one-time Condor standard job**, not interactively.

### Steering content

Use the Sherpa 3.0.3 YAML format from the successful smoke test and the mc-expert review. The obsolete Sherpa 2.x `(run){}` syntax is removed from this plan.

Planned Sherpa 3.0.3 YAML fragment:

```yaml
EVENTS: 100000
EVENT_OUTPUT: HepMC3_GenEvent[sidis_hq2]

BEAMS: [11, 2212]
BEAM_ENERGIES: [27.6, 920]
PDF_LIBRARY: LHAPDFSherpa
PDF_SET: NNPDF31_lo_as_0118
ME_GENERATORS: [Comix]

PROCESSES:
  - 11 93 -> 11 93:
      Order: {QCD: 0, EW: 2}

SELECTORS:
  - [Q2, 11, 11, 150., 1e12]

YFS_MODE: 0

# OPEN ITEM #1: confirm exact stable-particle YAML key in 3.0.3
PARTICLE_DATA:
  310:  { Stable: 1 }
  130:  { Stable: 1 }
  3122: { Stable: 1 }
  3222: { Stable: 1 }
  3112: { Stable: 1 }
  3312: { Stable: 1 }
  3322: { Stable: 1 }
  3334: { Stable: 1 }
```

Required frozen content:
- process line must be `11 93 -> 11 93` with `Order: {QCD: 0, EW: 2}`
- `BEAMS: [11, 2212]` remains generator-native FLIPPED orientation because Sherpa rejects the reverse ordering
- `EVENT_OUTPUT: HepMC3_GenEvent[...]`
- Q² cut via `SELECTORS:` block using the syntax already proven in `H1/sidis_highQ2/MC/smoke_q2/sherpa/Sherpa.yaml`
- `YFS_MODE: 0` is the current candidate for nominal rad-off mode, but remains an open item for full inventory/sign-off
- decay-table lockdown for `{310, 130, 3122, 3222, 3112, 3312, 3322, 3334}` remains blocked on the exact YAML key name

For `e+ p` mode, switch to:
- `BEAMS: [-11, 2212]`
- `PROCESSES: -11 93 -> -11 93`

Submit-time wrapper requirement (per prior memo and smoke test): every job must source the LCG view and apply the Sherpa `LD_LIBRARY_PATH` fix:

```bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh
export LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/releases/MCGenerators/sherpa/3.0.3.atlas1-4ee77/x86_64-el9-gcc14-opt/lib64/SHERPA-MC:$LD_LIBRARY_PATH
```

### Particle-selection enforcement

Sherpa must enforce the gen-level long-lived wall by keeping the following particles stable in the decay table / particle configuration:
- `3122`, `310`, `130`, `3222`, `3112`, `3312`, `3322`, `3334` and antiparticles

Preferred enforcement mode:
- per-PDG stable override in Sherpa 3 YAML `PARTICLE_DATA`

Required plan rules:
- verify Sherpa default behavior on NAF and lock it explicitly in the steering/config so it cannot drift silently
- **all short-lived states must remain at default and therefore decay**
- **do not globally disable hadron decays**

Required research dispatch:
- confirm exact Sherpa YAML stable-particle key syntax (`Stable` vs `IsStable`) and cite the manual section in implementation notes
- inventory any additional Sherpa QED-off knobs beyond `YFS_MODE: 0`

Required validation:
- the reducer must still call `MC/common/option_h_selector.py` as the second line of defense
- Phase 0 sanity test must verify zero accepted hadrons with any long-lived-wall ancestor

Pilot status:
- **Sherpa smoke test succeeded for Q²-only validation**
- **Sherpa is blocked for Phase 1a hadron pilots** on open item P1 #1 (`Stable` vs `IsStable` / exact stable-particle YAML key)
- this blocking state is reinforced by the 2026-05-26 smoke test, which deliberately omitted the decay-table lockdown because Q² is leptonic-only

### Job output

- one transient HepMC3 ASCII file per job
- one persistent per-job ROOT ntuple per job
- no persistent per-job JSON summary

### Converter path

Preferred path:
- Sherpa HepMC3 → `MC/scripts/reduce_hepmc_to_root.py`

## Pythia6

Citation anchor for this section:
- review memo: `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md`
- smoke test: `H1/sidis_highQ2/MC/smoke_q2/smoke_test_2026-05-26.md`
- smoke verdict: `σ_gen = 853,718 ± 4.3 pb` for uncut inclusive DIS with the LCG `libpythia6_pdfdummy.so` stub; Q² shape is catastrophically distorted at high Q², with a factor `40–350×` excess above Sherpa for `Q² > 3000 GeV²`; this empirically confirms the LHAPDF6 bridge as a pilot-blocking item.

### Target

- Version: **Pythia 6.4.29.2**
- DIS steering via a local driver under our control
- PDF through **LHAPDF6**, once the bridge recipe is resolved

### Environment / build strategy

Pythia6 is not treated as a run-card-only generator here. The deliverable is a local driver plus a small conversion wrapper:
- `MC/pythia6/driver/pythia6_dis_driver.f90` (or `.f`)
- `MC/pythia6/driver/pythia6_to_hepmc3.cxx`

Planned NAF build path:
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/pythia6/build/`

Planned build recipe skeleton:

```bash
export PYTHIA6_ROOT="$PWD/install"
make pythia6
make pythia6_to_hepmc3
```

More explicit build content to freeze after research:
- compiler toolchain from LCG view
- static vs shared build choice
- LHAPDF6 bridge library or wrapper recipe
- HepMC3 conversion-wrapper implementation choice

This one-time build must also run through Condor if compilation is non-trivial.

### Steering content

Planned steering anchor in the driver:

```fortran
MSEL = 2
MSTP(51) = 315000
MSTP(52) = 2
MSTP(61) = 0
MSTP(71) = 1
MSTP(11) = 0
MSTP(12) = 0
CKIN(65) = 150.D0
```

Beam initialization is frozen to the explicit `3MOM` setup in FLIPPED orientation, matching the other generators so the reducer auto-detection path is exercised uniformly:

```fortran
P(1,1) = 0.D0
P(1,2) = 0.D0
P(1,3) = +27.6D0
P(2,1) = 0.D0
P(2,2) = 0.D0
P(2,3) = -920.D0
CALL PYINIT('3MOM', 'e-', 'p+', 0.D0)
```

For `e+ p` mode, change `'e-'` to `'e+'` in the `PYINIT` call.

Frozen planning decisions:
- use `PYCOMP(KF)`, not `LUCOMP(KF)`, for decay-table lockdown
- use `PYINIT('3MOM', ...)` with explicit beam 3-momenta
- keep `CKIN(65) = 150.D0` as the current Q²-min placeholder, but note that the 2026-05-26 smoke test found that this index had no effect in the tested DIS setup; manual verification remains required before production use

The exact DIS-process and radiation switches must be validated before implementation. The plan freezes the **physics goal**, not guessed library paths:
- HERA-II ep DIS at 27.6 × 920 GeV
- e+ and e- beam modes both supported
- LHAPDF6-backed PDF set documented
- default hadronization/tune documented
- nominal rad-off prediction and optional rad-on cross-check variant

### Particle-selection enforcement

Pythia6 must enforce the gen-level long-lived wall at generation time by keeping the long-lived strange parents stable.

Required pre-generation `MDCY(KC,1)=0` stable-particle switches for:
- `Λ` (`KF=3122`)
- `K0_S` (`KF=310`)
- `K0_L` (`KF=130`)
- `Σ+` (`KF=3222`)
- `Σ−` (`KF=3112`)
- `Ξ−` (`KF=3312`)
- `Ξ0` (`KF=3322`)
- `Ω−` (`KF=3334`)
- and antiparticles

The driver must use `PYCOMP(KF)`, not `LUCOMP(KF)`, e.g.:

```fortran
MDCY(PYCOMP( 310), 1) = 0
MDCY(PYCOMP( 130), 1) = 0
MDCY(PYCOMP(3122), 1) = 0
MDCY(PYCOMP(3222), 1) = 0
MDCY(PYCOMP(3112), 1) = 0
MDCY(PYCOMP(3312), 1) = 0
MDCY(PYCOMP(3322), 1) = 0
MDCY(PYCOMP(3334), 1) = 0
```

Required plan rules:
- this stable-wall behavior is expected to be close to Pythia6 defaults, but the actual settings must still be checked and locked
- `CKIN(65) = 150.D0` remains an open item for authoritative verification
- **all short-lived states must remain at defaults and therefore decay**
- **do not globally disable hadron decays**
- Pythia6 uses LHAPDF6 via a bridge library or shim; the plan must not guess a library path before the bridge recipe is resolved

Required validation:
- inspect HEPEVT or HepMC output and confirm these PDGs appear as stable final-state particles rather than decaying to charged daughters
- then apply `MC/common/option_h_selector.py` as a second-line ancestor veto before filling hadron histograms

Pilot status:
- **Pythia6 is blocked for Phase 1a pilots** on open item P1 #2 (LHAPDF6 bridge recipe)
- this blocker is now **empirically confirmed** by the 2026-05-26 smoke test: the LCG `libpythia6_pdfdummy.so` stub yields a wildly corrupted high-Q² tail and is unusable for physics predictions

### Output

Preferred:
- native HEPEVT → HepMC3 conversion wrapper → common reducer

Fallback:
- native event dump parsed directly by a dedicated adapter if HepMC3 bridge is unavailable

### Converter path

- `pythia6` native/HEPEVT → `MC/pythia6/driver/pythia6_to_hepmc3.cxx` → `MC/scripts/reduce_hepmc_to_root.py`
- if necessary, a dedicated `MC/pythia6/convert_pythia6_to_root.py`

## Pythia8

Citation anchor for this section:
- review memo: `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md`
- smoke test: `H1/sidis_highQ2/MC/smoke_q2/smoke_test_2026-05-26.md`
- smoke verdict: `σ_gen ≈ 1950 ± 201 pb` at `Q² > 150 GeV²` for 10k events; runtime ~4.6 s; Q² shape agrees with Sherpa within a factor of about 2; **Pythia8 is unblocked** for Phase 1a pilots.

### Target

- Version: **8.317**
- PDF via **LHAPDF6**
- HepMC3 output through standard interface

### Environment discovery

Verify via LCG view:

```bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_*/x86_64-el9-gcc*/setup.sh
python - <<'PY'
import sys
PY
pythia8-config --version
```

### Steering content

The plan adopts the corrected cmnd content from the review memo and the on-disk file `H1/sidis_highQ2/MC/pythia8/cmnd/pythia8_dis_nominal.cmnd`.

Planned command file fragment:

```text
Beams:idA = 11
Beams:idB = 2212
Beams:eA = 27.6
Beams:eB = 920
Beams:frameType = 2

SoftQCD:all = off
HardQCD:all = off
WeakBosonExchange:ff2ff(t:gmZ) = on

PhaseSpace:Q2Min = 150.

PDF:useHard = on
PDF:pHardSet = LHAPDF6:NNPDF31_lo_as_0118
PDF:pSet = LHAPDF6:NNPDF31_lo_as_0118

SpaceShower:QEDshowerByL = off
TimeShower:QEDshowerByL = off
SpaceShower:QEDshowerByQ = on
TimeShower:QEDshowerByQ = on

310:mayDecay = off
130:mayDecay = off
3122:mayDecay = off
3222:mayDecay = off
3112:mayDecay = off
3312:mayDecay = off
3322:mayDecay = off
3334:mayDecay = off
```

Frozen interpretation of this card:
- beam setup remains generator-native FLIPPED; reducer handles orientation normalization
- process content is minimal LO NC DIS
- `PhaseSpace:Q2Min = 150.` is **critical**; without it, nearly all CPU is wasted at low `Q²`
- lepton-line QED shower is off; quark-line QED shower remains on
- the long-lived wall is explicitly locked in the command file
- tune is the Pythia8 Monash 2013 default; alternative tunes are deferred to Phase 4
- beam-mode selection is one cmnd file plus driver CLI override (`Beams:idA = -11` for em mode), which is already implemented in the driver design

For e+ mode, switch `Beams:idA = -11`. No additional positron-specific fixes are needed. The different electroweak coupling sign between e− and e+ modes is correct physics and is not overridden.

Required plan rules:
- Pythia8 defaults will otherwise decay `K0_S` and `Λ`, silently corrupting the accepted charged-hadron sample
- `mayDecay = off` for the full long-lived-wall list is therefore mandatory
- `PhaseSpace:Q2Min = 150.` is mandatory; without it, the generator wastes ~99% of CPU at low `Q²`
- **all short-lived states must remain at defaults and therefore decay**
- **do not globally disable hadron decays**
- the implementation must add a CI-style pre-submit validator that greps the actual run card for all required `mayDecay = off` lines and blocks Condor submission if any are missing
- the reducer must still call `MC/common/option_h_selector.py` as the second line of defense
- document the nominal tune as `Pythia8 Monash 2013 (default 8.317 tune)`

Open factual check:
- confirm whether the per-lepton QED shower flags are sufficient alone or whether the master-switch scope should also be documented as defense in depth

Pilot status:
- **Pythia8 is unblocked and verified by the 2026-05-26 smoke test**
- **Phase 1a should start with Pythia8**

### Output

- transient HepMC3 ASCII per job
- persistent per-job ROOT ntuple per job

### Converter path

- Pythia8 HepMC3 → `MC/scripts/reduce_hepmc_to_root.py`

## Shared binning implementation

`MC/common/binning.py` must be the single source of truth for all three generators.

Planned implementation features:

```python
from pathlib import Path
import hashlib
import json

BINNING_PATH = Path("H1/sidis_highQ2/docs/data/binning_v5f_frozen.json")
EXPECTED_Q2 = [(150.0, 300.0), (300.0, 700.0), (700.0, 1500.0), (1500.0, 20000.0)]
EXPECTED_X = [
    [0.0021, 0.0071, 0.030],
    [0.0042, 0.0160, 0.069],
    [0.0099, 0.0340, 0.150],
    [0.0210, 0.0870, 1.500],
]
EXPECTED_Z = [0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]
EXPECTED_PTH = [0.0, 0.5, 1.0, 2.0, 4.0, 8.0]
EXPECTED_PHI = [-3.141592653589793 + i * (2.0 * 3.141592653589793 / 16.0) for i in range(17)]
```

Validation rule:
- fail fast if the file content does not match the frozen B′ edges and expected tag
- compute `binning_sha` from the exact JSON bytes

This protects against stale filename / content drift.

## Cross-section, multiplicity, and moment formulas

### Differential cross section

The merged prediction should use the generator-reported DIS cross section together with accepted hadron yields after cuts.

Planned formula:

```python
xsec[cell,z,pth,ch] = (
    sigma_dis_nb * N_h[cell,z,pth,ch] / N_gen_dis_total
    / dQ2dx_cell[cell] / dz[z] / dpth[pth]
)
```

Implementation note:
- the exact `dQ² dx` normalization for stratum-specific x bins must be handled explicitly in `schema.py`
- `dQ²dx_cell[cell] = (Q2_hi - Q2_lo) * (x_hi - x_lo)`
- units must match `nb / GeV² / GeV`

### Multiplicity

```python
M[cell,z,pth,ch] = N_h[cell,z,pth,ch] / N_DIS[cell] / dz[z] / dpth[pth]
```

### Charge asymmetry

```python
A_h[cell,z,pth] = (N_plus - N_minus) / (N_plus + N_minus)
```

### Azimuthal moments

```python
cos1 = sum_w_cos1 / sum_w_had
cos2 = sum_w_cos2 / sum_w_had
sin1 = sum_w_sin1 / sum_w_had
sin2 = sum_w_sin2 / sum_w_had
```

### Statistical errors

Planned v1 treatment:
- counting observables (`N_DIS`, `N_h`, `M`, `A_h`) use standard weighted Poisson / propagated variance from `sum_w` and `sum_w2`
- harmonic moments use variance of weighted sample means from additive sums of weights and trigonometric values
- if generator events are strictly unit-weight in a given mode, the builder may simplify to unweighted formulas but should still store weighted sums for generality
- `MC/common/schema.py` must expose `build_final_prediction(selection="fullps" | "fid")`

## Comparison script requirements

`MC/scripts/compare_data_vs_mc.py` must:

1. load any `MC/predictions/*_fullps.json` and `MC/predictions/*_fid.json`
2. refuse to plot if `species_definition.convention != "gen-level primary π/K/p (long-lived wall at cτ ≳ 10 mm)"`
3. refuse to plot if any of `composition_validation.frac_lambda_ancestor`, `frac_k0s_ancestor`, `frac_klong_ancestor`, `frac_sigma_pm_ancestor`, `frac_xi_ancestor`, or `frac_omega_ancestor` is nonzero
4. load:
   - `H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json`
   - `H1/sidis_highQ2/data/unfolded_4d_v3_IBU.json`
   - other comparison JSONs as needed for moments
5. load trust mask:
   - `H1/sidis_highQ2/data/v5g_4d_trust_mask.json`
6. apply the 480-cell mask before plotting comparisons
7. produce per `(Q²,x)` cell overlays for:
   - data points + MC-fid solid line + MC-fullps dashed line in the same panel
   - `M^{h±}` vs `z` for each `pT_h` slice
   - `M^{h±}` vs `pT_h` for each `z` slice
   - `<cos φ_h>` and `<cos 2φ_h>` vs `z`
   - `<cos φ_h>` and `<cos 2φ_h>` vs `pT_h`
   - optionally `sin` moments and raw `φ_h` histograms
8. use H1 plotting conventions, including Okabe-Ito palette and no chartjunk

Planned plotting-style anchor:

```python
OKABE_ITO = [
    "#000000", "#E69F00", "#56B4E9", "#009E73",
    "#F0E442", "#0072B2", "#D55E00", "#CC79A7",
]
```

Plot outputs:
- individual PNG/PDF panels under `MC/plots/`
- bundled report PDF: `MC/plots/data_vs_mc_v1.pdf`

## Condor and storage plan

### Job model

For each generator and beam mode:
- split production into about `50–200` jobs by random seed
- one core, 2 GB, 3 h standard jobs
- one transient raw event file per job
- one persistent ROOT ntuple per job under `predictions_raw/<generator>/...`
- one per-job wrapper runs generator → reduces to ROOT → deletes HepMC → returns only the ROOT file

Planned submit fragment:

```text
universe              = vanilla
request_cpus          = 1
request_memory        = 2000
+RequestRuntime       = 10800
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
queue
```

### Login-node hygiene

Allowed login-node use in this subproject:
- environment discovery (`which`, version checks)
- `MC/common/sanity_test.py` on 1000 events total / one tiny file / ≤5 minutes

Forbidden on login node:
- any multi-job generation
- any batch reduction
- any sweep over multiple outputs

### DUST hygiene

Before Phase 2 launch, explicitly check:
- `/data/dust/group/h1/.quota-usage.csv`

Planned storage policy:
- HepMC ASCII files are intermediate-only
- per-job wrapper must delete HepMC after successful ROOT reduction and validation unless `--keep-hepmc` is passed
- persistent artifacts are:
  - steering cards
  - Condor logs
  - per-job ROOT ntuples under `predictions_raw/<generator>/...`
  - final prediction JSONs (`*_fullps.json`, `*_fid.json`)
  - merged skim ROOTs under `MC/predictions/`
  - a tiny audit sample of HepMC files per generator only if explicitly retained

Revised footprint estimate:
- about `~500 MB` per million generated events per generator per beam mode
- about `~60 GB` total for `10M events × 3 generators × 2 beam modes`
- this fits inside current headroom provided HepMC is deleted promptly and per-job ROOTs are skimmed/cleaned after final products are built if needed

Environment requirement:
- uproot must be available in the analysis environment for ROOT-side reading and prediction building

## Statistical target and production sizing

### Target rule

The requested target is:
- MC statistical uncertainty in a **typical populated 4D cell** should be `≤ 5%` of the corresponding **data statistical uncertainty** in that cell

Interpretation for planning:
- if data relative stat error in a representative trusted cell is about `1/sqrt(N_data)` for count-dominated bins, require
  - `1/sqrt(N_mc) ≤ 0.05 / sqrt(N_data)`
  - therefore `N_mc ≥ 400 * N_data`

This is very demanding, so the plan adopts a practical estimator based on **representative trusted cells**, not the very highest-occupancy bins.

### Initial estimate from trust-mask counts

The trust mask shows a broad span, e.g. in `S0a_h+`:
- high-population bins: `N_data ~ 2.9e5`
- moderate bins: `N_data ~ 1e4`
- edge populated bins: `N_data ~ 1e3`

If one blindly used `N_data = 1e4`, the criterion implies:
- `N_mc ≥ 4e6` hadrons in that cell

That in turn suggests total generated DIS events in the **10^8-scale** range per generator may be needed for a fully smooth absolute prediction across all trusted bins, depending on acceptance into the 4D hadron grid.

### Practical production strategy

Phase 1 pilots will determine:
- accepted DIS fraction after cuts
- hadron occupancy per trusted 4D cell
- per-job wall time
- raw-event-file size and per-job ROOT size

Then Phase 2 will solve for required events using:

```python
target_events = max(
    min_events_floor,
    pilot_events * max_over_reference_cells((target_rel_err / pilot_rel_err[cell]) ** -2)
)
```

Planning seed estimate for Phase 2:
- start from **5M events per beam mode per generator** as the first scaling anchor
- expect eventual production to land in the rough range:
  - **10M–50M total events per generator** if weighted generation is efficient and moderate trusted-cell precision is sufficient
  - potentially more if unweighted low-efficiency DIS generation is used

### Wallclock / CPU placeholders to be measured in Phase 1

Initial planning assumptions only:
- Sherpa: `100k` events/job may be near the 2–3 h boundary depending on DIS setup
- Pythia8: `100k–500k` events/job likely feasible inside 3 h
- Pythia6: `100k–300k` events/job likely feasible inside 3 h once the PDF bridge is fixed

Pilot must record:
- events/sec
- ROOT reduction time
- HepMC size per 100k events
- ROOT size per 100k events
- total CPU estimate for full-stat campaign

## Phase plan and dispatch breakdown

## Phase 0 — bootstrap

### Goal

Create the directory design, freeze the common observable and gen-level long-lived-wall species conventions, and establish the smallest end-to-end smoke framework without launching real production.

### Gating conditions into Phase 0

- none

### Exit gate from Phase 0

Phase 0 cannot complete until the available / unblocked generator set passes the species sanity test and the remaining generators have their pilot-blocking items explicitly documented:
- `frac_lambda_ancestor == 0.0`
- `frac_k0s_ancestor == 0.0`
- `frac_klong_ancestor == 0.0`
- `frac_sigma_pm_ancestor == 0.0`
- `frac_xi_ancestor == 0.0`
- `frac_omega_ancestor == 0.0`
- sanity results persisted to `MC/common/sanity_test_results_<generator>_<date>.json`

If any of these is nonzero, the corresponding long-lived particle was decayed by the generator and the run card / decay table must be fixed before the relevant pilot sub-phase.

### Deliverables

- directory bootstrap commands documented
- shared `binning.py`, `observables.py`, `option_h_selector.py`, `schema.py`, `sanity_test.py` planned
- per-generator environment discovery tasks defined
- per-job ROOT schema frozen
- pipeline skeleton proves both `fullps` and `fid` predictions can be built from the same ntuple design
- species-sanity outputs persisted for the generators whose pilot blockers are resolved

### Dispatches

1. **Research dispatch: generator availability on NAF**
   - specialist: `researcher`
   - questions:
     - which Sherpa versions are visible via LCG/CVMFS on NAF?
     - is Pythia8 with LHAPDF6 and HepMC3 available in the same view?
     - is Pythia6 packaged anywhere or must it be locally built?
2. **Run-card / z-axis review incorporation dispatch**
   - specialist: `h1-mc-expert`
   - scope:
     - incorporate `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md` as authoritative for beam-axis normalization and run-card specifics
3. **Physics/run-card review dispatch**
   - specialist: `h1-mc-expert`
   - scope:
     - review DIS steering assumptions for all three generators
     - confirm rad-off nominal setup is physically consistent for corrected-data comparison
4. **Convention alignment dispatch**
   - specialist: `h1-software`
   - scope:
     - review `MC/common/observables.py` plan against canonical high-Q² extractor conventions
5. **Implementation dispatch**
   - specialist: `code-editor`
   - scope:
     - implement local `MC/common/*` and `MC/scripts/*` skeletons
     - add `MC/common/option_h_selector.py`
     - add run-card validators enforcing long-lived-wall decay-lockdown lines where applicable
6. **Critic dispatch**
   - specialist: `h1-analysis-critic`
   - scope:
     - review Phase 0 design before any pilot generation

### Planned bootstrap commands to include later in implementation

```bash
mkdir -p "H1/sidis_highQ2/MC"/{common,scripts,sherpa,pythia6,pythia8,predictions,plots}
mkdir -p "/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC"/{common,scripts,sherpa,pythia6,pythia8,predictions,plots}
```

## Phase 1 — sequential pilot program

### Goal

Run the pilot program in the generator-unblock order established by the review memo and smoke test, proving full pipeline closure from generation through ROOT reduction, prediction building, and comparison plotting.

### Entry gate

All of the following must be true:
- Phase 0 critic review passes
- schema validator passes on synthetic or tiny smoke output
- Pythia8 run-card and reducer orientation logic are implementation-ready
- open pilot blockers are explicitly tracked for Sherpa and Pythia6

### Deliverables

- pilot ROOT ntuples per launched generator in `predictions_raw/`
- `fullps` and `fid` pilot JSONs per launched generator in `predictions/`
- schema-validation report
- first `compare_data_vs_mc.py` overlay output using statistics-limited pilot results
- first `compare_fid_vs_fullps.py` comparison output for at least one generator
- measured runtime / file-size / occupancy report

### Phase 1a — Pythia8 pilot (UNBLOCKED, NOW)

Scope:
- land the body of `MC/scripts/reduce_hepmc_to_root.py`, including file-level orientation detection, conditional `R_y(180°)`, metadata propagation, event-audit branches, and per-event assertions
- run a `100k`-event Condor pilot as `1 ep job + 1 em job`
- validate ROOT output with `build_prediction_from_root.py --selection both`
- run `compare_fid_vs_fullps.py` on the pilot
- produce first sanity plots, including:
  - `η_lab` in HERA convention (`η > 0` proton/forward side; `η < 0` electron/backward side)
  - fid-vs-fullps overlay

Exit gate:
- pilot JSONs validate
- fid-vs-fullps plot exists
- `η_lab` orientation is verified in HERA convention after reduction
- per-job runtime / CPU are measured and recorded

### Phase 1b — Sherpa pilot (BLOCKED on open item P1 #1)

Begins only when the stable-particle YAML key resolves.

Scope once unblocked:
- implement `MC/sherpa/Sherpa.yaml` and Condor wrapper with the `LD_LIBRARY_PATH` workaround
- run `100k`-event pilot
- same downstream ROOT → JSON → plotting pipeline as Pythia8

Exit gate:
- same as Pythia8 Phase 1a

### Phase 1c — Pythia6 pilot (BLOCKED on open item P1 #2)

Begins only when the LHAPDF6 bridge resolves.

Scope once unblocked:
- implement `MC/pythia6/driver/pythia6_dis_driver.f90`
- implement `MC/pythia6/driver/pythia6_to_hepmc3.cxx`
- run `100k`-event pilot
- same downstream ROOT → JSON → plotting pipeline as Pythia8

Exit gate:
- same as Pythia8 Phase 1a

### Dispatches

1. **NAF environment and pilot submission**
   - specialist: `h1-production`
   - scope:
     - prepare standard Condor pilot jobs in unblock order
2. **Run-card physics sign-off**
   - specialist: `h1-mc-expert`
   - scope:
     - generator-specific steering audit before submit
3. **Converter and reducer implementation**
   - specialist: `code-editor`
   - scope:
     - HepMC/native reducers, reducer-side rotation logic, ROOT writer, and final serializer
4. **Lumi / weighting audit**
   - specialist: `h1-normalization`
   - scope:
     - verify period-lumi weighting implementation and metadata fields
5. **Plotting pilot**
   - specialist: `h1-plotting`
   - scope:
     - first data-vs-MC and fid-vs-fullps plots from pilot JSONs
6. **Phase-end critic review**
   - specialist: `h1-analysis-critic`
   - scope:
     - detect convention mismatches or misleading early comparisons

### Exit criteria

- Pythia8 emits valid `fullps` and `fid` pilot JSONs from the same per-job ROOT ntuples
- Sherpa and Pythia6 pilots begin only after their respective P1 blockers resolve and then meet the same criteria
- axis ordering matches `(q2x, z, pTh, charge)` exactly
- trust-mask application works
- pilot plots run end-to-end
- fid-vs-fullps comparison plot exists for at least one generator
- `η_lab` distributions and beam audit branches confirm HERA convention after reduction
- per-generator timing and storage metrics logged for Phase 2 scaling
- long-lived-wall ancestor fractions remain exactly zero in the pilot outputs for all launched generators

### Composition transparency note

Per-generator composition fractions such as:
- `frac_resonance_decay`
- `frac_charm_decay`
- `frac_bottom_decay`

should still be recorded in the prediction JSON for transparency, but they are **not gating quantities**. They may legitimately differ between Sherpa, Pythia6, and Pythia8 because of tune, hadronization model, and decay-table differences.

## Phase 2 — full statistics

### Goal

Scale each generator to the required statistics target using the Phase 1 occupancy and timing measurements.

### Entry gate

- Phase 1a, 1b, and 1c exit criteria complete
- event-count scaling model approved
- DUST headroom checked and recorded

Phase 2 begins only when **all three Phase 1 sub-phases close**.

### Deliverables

- full-stat `fullps` and `fid` prediction JSONs per generator
- merged skim ROOT per generator
- final merged metadata including seeds, clusters, run-card hashes
- retained audit subset of raw event files if needed

### Dispatches

1. **Quota/headroom dispatch**
   - specialist: `h1-production`
   - scope:
     - check `/data/dust/group/h1/.quota-usage.csv`
     - estimate raw and reduced footprint
2. **Full production submission**
   - specialist: `h1-production`
   - scope:
     - launch `50–200` jobs/generator/beam-mode as needed
3. **Merge and validation implementation**
   - specialist: `code-editor`
   - scope:
     - summary merger, provenance collation, restart-safe merge bookkeeping
4. **Normalization audit**
   - specialist: `h1-normalization`
   - scope:
     - sign off on lumi-equivalent metadata and charge-mix combination
5. **Phase-end critic review**
   - specialist: `h1-analysis-critic`
   - scope:
     - inspect statistical sufficiency and comparison readiness

### Exit criteria

- target MC precision met on approved representative cells
- merged JSONs validate
- no unresolved storage or provenance issues remain

## Phase 3 — data-vs-MC comparison memo

### Goal

Produce the first internal comparison package across Sherpa, Pythia6, and Pythia8 versus H1 data.

### Entry gate

- full-stat generator JSONs available
- plotting script stable
- critic approves that comparisons are not misleadingly over-interpreted

### Deliverables

- `MC/plots/data_vs_mc_v1.pdf`
- per-panel plot files under `MC/plots/`
- per-generator fid-vs-fullps comparison grid and CSV summary under `MC/plots/`
- one result memo under `H1/sidis_highQ2/docs/results/` (implementation by `doc-writer`, not by this planner)

### Dispatches

1. **Comparison plotting**
   - specialist: `h1-plotting`
   - scope:
     - final overlay plots and panel assembly
2. **Comparison script hardening**
   - specialist: `code-editor`
   - scope:
     - finish `compare_data_vs_mc.py`, legends, masks, labels, ratio panels if approved
3. **Physics interpretation review**
   - specialist: `h1-mc-expert`
   - scope:
     - generator-setting caveats and interpretation notes
4. **Final critic loop**
   - specialist: `h1-analysis-critic`
   - scope:
     - review the comparison package before memo drafting
5. **Memo drafting handoff**
   - specialist: `doc-writer`
   - scope:
     - write the results memo from approved plots and metadata

### Exit criteria

- PDF package produced
- memo inputs frozen
- generator caveats documented clearly

## Phase 4 — deferred systematics and variants

### Goal

List the first systematic / model variants without making them v1 blockers.

### Deferred items

- PDF variation
- scale variation (`μR`, `μF` × `0.5`, `2.0`)
- tune variation
- rad-QED-on cross-check
- `A_BC` beam-charge-asymmetry prediction extension
- external harmonic refits from raw `φ_h` histograms

### Dispatch hooks

- `h1-mc-expert` for theory/run-card variation design
- `h1-production` for Condor production
- `code-editor` for schema extension if `A_BC` becomes first-class
- `h1-analysis-critic` after each variation family

## Risks, trade-offs, and mitigations

### Risk 1: long-lived particles must remain undecayed; everything else must decay

- top-priority risk, especially for **Pythia8** defaults where `K0_S` and `Λ` decay unless explicitly disabled
- failure mode: long-lived weak parents decay and their charged daughters leak into the accepted sample, overcounting pions/protons at the `~5–10%` level at moderate `z`
- opposite failure mode: hadron decays are disabled too broadly, stranding short-lived parents and undercounting final-state `π/K/p`
- mitigation:
  - generator-side decay-table lockdown for the full long-lived-wall list only
  - shared `MC/common/option_h_selector.py` ancestor veto as a second line of defense
  - CI-style run-card validator for required `mayDecay = off` lines / stable-particle switches
  - Phase 0 zero-long-lived-ancestor sanity gate

### Risk 2: beam-orientation auto-detection fails on a generator that randomizes orientation per event

- failure mode: a generator or wrapper emits inconsistent event-to-event beam orientation and the reducer misclassifies the file-level orientation
- mitigation:
  - detect on beam particles from event 0 / file metadata
  - assert on every event after correction that `p_z(electron) < 0` and `p_z(proton) > 0`
  - block the job immediately if the convention check fails
  - startup sanity checks make mixed-orientation files visible immediately

### Risk 3: Trento `φ_h` sign convention silently flips if reducer applies `R_y(180°)` inconsistently

- failure mode: event particles and beam particles are not rotated coherently, flipping sign-sensitive triple products
- mitigation:
  - analytic closure unit test in `MC/common/observables.py` is already passing
  - add on-real-event regression in the reducer validation path
  - per-event beam-direction assertion in the reducer
  - audit branches `beam_electron_pz_after_rotation` and `beam_proton_pz_after_rotation`

### Risk 4: generator DIS steering is non-trivial or not available in packaged form

- highest for **Pythia6**, moderate for **Sherpa**, lower for **Pythia8**
- mitigation:
  - research availability first
  - allow local build path under generator subdirs
  - validate with 1000-event smoke before pilot

### Risk 5: frame/angle convention drift from canonical analysis

- this is analysis-killing if wrong
- mitigation:
  - one shared `observables.py`
  - explicit tiny common-sample comparison against canonical extractor conventions
  - critic review at end of each phase

### Risk 6: absolute cross-section normalization ambiguity

- current data xsec JSON still contains normalization caveats
- mitigation:
  - v1 plots should separate shape and absolute-rate comparisons
  - preserve generator-reported cross section metadata in summaries
  - require `h1-normalization` review before claiming absolute agreement/disagreement

### Risk 7: per-job ROOT files can multiply small-file count

- per-job ROOT files can multiply small-file count even if byte footprint is controlled
- DUST file count is already high, so directory fan-out and end-of-production skimming are mandatory
- mitigation:
  - write per-job ROOTs under batched hierarchies such as `predictions_raw/<generator>/seed_NNNN/`
  - build one merged skim ROOT per generator and delete per-job ROOTs afterward if pressure demands it
  - keep HepMC intermediate-only and delete after ROOT reduction

### Risk 8: Pythia6 interface fragility on modern toolchains

- mitigation:
  - isolate Pythia6 adapter
  - allow native-to-ROOT converter path if HepMC bridge is painful
  - do not block Sherpa/Pythia8 progress on Pythia6 packaging details once v1 core is validated

### Risk 9: Pythia8 without `PhaseSpace:Q2Min = 150` wastes ~99% of CPU at low `Q²`

- failure mode: the generator samples the `Q² -> 0` singular region, then the analysis throws away nearly all events
- mitigation:
  - require `PhaseSpace:Q2Min = 150.` in the cmnd file
  - run-card validator must treat its absence as a blocking defect

### Risk 10: Sherpa 3 YAML key `Stable` vs `IsStable` is unverified

- failure mode: the intended long-lived wall does not actually become stable and the pilot silently decays forbidden parents
- mitigation:
  - Sherpa hadron pilot blocked until the YAML key is resolved
  - keep selector-side ancestor veto and species sanity test as independent checks

### Risk 11: Pythia6 with the PDF stub produces a catastrophically wrong high-Q² tail

- failure mode: the default `libpythia6_pdfdummy.so` flat-PDF stub removes the real large-x suppression and produces a `~40–350×` excess for `Q² > 3000 GeV²`
- mitigation:
  - treat the LHAPDF6 bridge as mandatory for any physics use
  - keep Pythia6 pilot blocked until the bridge is resolved
  - re-run the Q² smoke after wiring the real PDF bridge before any hadron pilot

### Trade-off: one unified schema vs generator-specific richness

- unified schema simplifies comparisons and plotting
- generator-specific details may be richer than the common schema
- mitigation:
  - keep a concise public unified JSON
  - retain per-job generator metadata and run-card hashes in provenance

## Open items

Source: `H1/sidis_highQ2/MC/common/review-mc-expert-runcards-and-zaxis-2026-05-26.md` §4.

### P1 — pilot-blocking

1. **Sherpa 3.0.3 stable-particle YAML key** (`Stable` vs `IsStable` vs other)
2. **Pythia6 + LHAPDF6 link recipe** (bridge library; empirically confirmed pilot-blocker by the 2026-05-26 smoke test)

### P2 — must resolve before full statistics

3. **Sherpa QED-off knob inventory** (`YFS_MODE: 0` is the current candidate)
5. **Pythia6 Q²-min CKIN index** (verify `CKIN(65)`)
6. **Pythia8 QED-shower master-vs-per-lepton flag scope**
8. **Pythia6→HepMC3 bridge implementation choice** (`HEPEVT_to_GenEvent_nonstatic` vs custom)
9. **Reducer `R_y(180°)` closure test against generator events** (analytic test passed; need on-real-event regression)

### P3 — post-Phase-1 cleanup

4. **Pythia6 MSTP(11/12/61/71) authoritative settings**
7. **Sherpa per-event weight extraction via pyHepMC3**

### Current pilot status

- **Pythia8 is unblocked and verified by the 2026-05-26 smoke test.**
- **Sherpa is blocked on P1 #1.**
- **Pythia6 is blocked on P1 #2, empirically confirmed by the smoke test: the high-Q² tail is wildly distorted by the missing PDF bridge.**

Phase 1a proceeds with **Pythia8 first**. Sherpa and Pythia6 pilots launch only after their P1 items resolve.

## Open Questions

The following are not user-choice ambiguities; they are factual checks that require research before implementation should proceed fully.

1. **What exact content currently lives in `H1/sidis_highQ2/docs/data/binning_v5f_frozen.json`?**
   - Need a factual validation that this path now contains or aliases the frozen B′ binning, despite the stale filename.
   - If not, implementation must stop and either update the source path or add a documented indirection layer.

2. **What is the exact Sherpa syntax for per-PDG stable-particle overrides for `Λ`, `K0_S`, `K0_L`, `Σ±`, `Ξ`, and `Ω`, and where is it documented in the Sherpa manual?**
   - Required before freezing Sherpa long-lived-wall enforcement.

3. **What is the exact Pythia6→LHAPDF6 bridge recipe that works on the current NAF / LCG platform?**
   - Required before Phase 1c Pythia6 pilot.

4. **What are the authoritative Pythia6 DIS Q²-cut controls for the current setup?**
   - The review proposes `CKIN(65)`, but the 2026-05-26 smoke test found no effect in the tested DIS mode.

5. **Are the Pythia8 per-lepton QED shower flags sufficient on their own, or should a broader master-switch be added as defense in depth?**
   - Required before freezing the final nominal card commentary.

6. **What are the correct per-event truth definitions of `y` and `(E-p_z)` to apply consistently across Sherpa, Pythia6, and Pythia8 event records?**
   - Need a frozen common definition in the reducer, especially when QED radiation is off.

7. **Can all three generators provide a trustworthy generator-reported DIS cross section after the fiducial event cuts, or must the prediction framework compute fiducial normalization exclusively from generated event counts and nominal generator σ?**
   - Needed for robust `xsec_4d` normalization.

## Explicit research dispatches to run before implementation

1. **Dispatch to `researcher`: NAF generator software inventory**
   - Verify Sherpa/Pythia8 availability on CVMFS/LCG
   - Verify Pythia6 packaged/unpackaged status
   - Verify LHAPDF6 / HepMC2 / HepMC3 combinations

2. **Dispatch to `researcher`: canonical binning file content audit**
   - Read `H1/sidis_highQ2/docs/data/binning_v5f_frozen.json`
   - Confirm whether it encodes `HQ-v5g-Bprime`
   - Report exact authoritative keys to load

3. **Dispatch to `researcher`: Sherpa stable-particle override syntax audit**
   - Confirm exact Sherpa syntax for per-PDG stable overrides of `3122, 310, 130, 3222, 3112, 3312, 3322, 3334`
   - Cite the manual section to anchor implementation

4. **Dispatch to `h1-mc-expert`: generator physics/run-card review**
   - DIS process configuration per generator
   - QED-radiation-off nominal recommendation
   - decay-table lockdown review: long-lived strange weak parents stable, short-lived states left at default decayed behavior
   - beam-axis normalization review: reducer-side `R_y(180°)` for H1 convention on disk
   - PDF/tune defaults suitable for v1

5. **Dispatch to `h1-normalization`: luminosity/weight policy audit**
   - Confirm high-Q² period-lumi values and charge-mix combination policy for MC predictions
   - Confirm whether absolute xsec comparison should reference `356,613 nb^-1` high-Q² S67 recoverable total or another narrower normalization layer

6. **Dispatch to `h1-software`: canonical CM-frame convention audit**
   - Confirm the exact observable implementation contract to mirror from the canonical extractor and frame-fix memo

## Implementation notes for `code-editor`

The following coding pattern is approved for implementation once research closes the factual items above.

### ROOT-driven prediction builder pattern

```python
def build_from_root(paths, selection="fullps"):
    summary = init_summary_arrays()
    for path in paths:
        events, hadrons, meta = load_root_ntuple(path)
        fill_dis_counts(summary, events)
        fill_hadron_sums(summary, hadrons, selection=selection)
    return build_final_prediction(summary, selection=selection)
```

### Trust-mask application pattern

```python
def apply_trust_mask(arr4d, trust_bins):
    masked = arr4d.copy()
    for charge_idx in range(2):
        for cell_idx in range(8):
            for z_idx in range(6):
                for pth_idx in range(5):
                    idx = charge_idx * 240 + cell_idx * 30 + z_idx * 5 + pth_idx
                    if not trust_bins[idx]["trust"]:
                        masked[cell_idx, z_idx, pth_idx, charge_idx] = np.nan
    return masked
```

### Comparison-script data loading pattern

```python
data_xsec = json.load(open("H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json"))
mc_fid = json.load(open("H1/sidis_highQ2/MC/predictions/pythia8_nominal_fid.json"))
mc_fullps = json.load(open("H1/sidis_highQ2/MC/predictions/pythia8_nominal_fullps.json"))
trust = json.load(open("H1/sidis_highQ2/data/v5g_4d_trust_mask.json"))
```

## Todo

- [ ] Phase 0 / research: dispatch `researcher` to audit `H1/sidis_highQ2/docs/data/binning_v5f_frozen.json` content and confirm the exact B′ keys and tag to load.
- [ ] Phase 0 / research: dispatch `researcher` to inventory Sherpa, Pythia8, LHAPDF6, HepMC2, and HepMC3 availability on NAF CVMFS/LCG.
- [ ] Phase 0 / research: dispatch `researcher` to determine the exact Pythia6 LHAPDF6 bridge recipe usable on LCG_109a and whether any packaged bridge already exists.
- [ ] Phase 0 / research: dispatch `researcher` to confirm exact Sherpa per-PDG stable-particle override syntax and manual references for the long-lived-wall excluded-parent list.
- [ ] Phase 0 / physics: dispatch `h1-mc-expert` to review nominal DIS steering, decay-table lockdown, reducer-side `R_y(180°)` normalization, and rad-off physics assumptions for Sherpa, Pythia6, and Pythia8.
- [ ] Phase 0 / conventions: dispatch `h1-software` to review the shared `γ*+p` CM-frame / Trento implementation contract against the canonical high-Q² extractor.
- [ ] Phase 0 / normalization: dispatch `h1-normalization` to sign off the high-Q² per-period lumi weighting to be embedded in the prediction metadata and charge-mix logic.
- [ ] Phase 0 / implementation: create `H1/sidis_highQ2/MC/` and the local subdirectories `common/`, `scripts/`, `sherpa/`, `pythia6/`, `pythia8/`, `predictions_raw/`, `predictions/`, `plots/`.
- [ ] Phase 0 / implementation: create `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/` and the matching NAF subdirectories.
- [ ] Phase 0 / implementation: add `MC/common/binning.py` that loads `docs/data/binning_v5f_frozen.json`, validates frozen B′ edges, computes `binning_sha`, and exposes axis lookup helpers.
- [ ] Phase 0 / implementation: add `MC/common/observables.py` with lab-frame beam/electron parsing, `q=k-k'`, `W=q+P` CM boost, invariant `z`, CM-frame `pT_h`, Trento `φ_h`, reducer-side `R_y(180°)` closure test in `__main__`, gen-level long-lived-wall species classification, `is_fullps_hadron()`, and `is_fid_gen_hadron()`.
- [ ] Phase 0 / implementation: add `MC/common/option_h_selector.py` as the single source of truth for gen-level primary `π/K/p` selection, excluded-ancestor veto, and ancestry bookkeeping.
- [ ] Phase 0 / implementation: add `MC/common/schema.py` defining the final public schema, validators, `selection` field support, merge helpers, and required species/composition metadata blocks.
- [ ] Phase 0 / implementation: add `MC/common/sanity_test.py` to run a 1000-event smoke test for unblocked generators under the single-file / ≤5 minute login-node rule.
- [ ] Phase 0 / implementation: add `MC/scripts/run_generator_job.py` as the unified wrapper to launch one job, record provenance, run generation, reduce to ROOT, and delete HepMC on success.
- [ ] Phase 0 / implementation: add `MC/scripts/reduce_hepmc_to_root.py` to read HepMC/native events, detect beam orientation from event 0, honor `--expected-orientation {auto,hera,flipped}`, apply `R_y(180°)` when needed, assert `p_z(electron) < 0` and `p_z(proton) > 0` after correction on every event, fill audit branches `beam_electron_pz_after_rotation` / `beam_proton_pz_after_rotation`, apply event cuts and the Option-H selector, write one ROOT file, and delete HepMC unless `--keep-hepmc` is passed.
- [ ] Phase 0 / implementation: add `MC/scripts/build_prediction_from_root.py` to read per-job ROOTs with uproot, accumulate histograms, and serialize both `predictions/<generator>_<tag>_fullps.json` and `predictions/<generator>_<tag>_fid.json`.
- [ ] Phase 0 / implementation: add `MC/scripts/skim_predictions_root.py` to concatenate per-job hadron trees into `predictions/<generator>_<tag>_hadrons.root`.
- [ ] Phase 0 / implementation: add `MC/scripts/make_condor_submit.py` for standard-job submit generation with `RequestCpus=1`, `RequestMemory=2000`, `+RequestRuntime=10800`.
- [ ] Phase 0 / implementation: add generator steering templates under `MC/sherpa/`, `MC/pythia6/driver/`, and `MC/pythia8/cmnd/`, with nominal `rad_qed=false` defaults and explicit long-lived-wall decay lockdown for `310, 130, 3122, 3222, 3112, 3322, 3312, 3334` where the generator supports it.
- [ ] Phase 0 / implementation: add a run-card validator that blocks submission if required long-lived-wall decay-lockdown lines / stable-particle overrides are missing.
- [ ] Phase 0 / implementation: freeze the ROOT ntuple branch list, metadata objects, compression choice, and output naming `<generator>_<beam_mode>_seed<seed>.root`.
- [ ] Phase 0 / validation: run the species sanity test on a tiny sample for each unblocked generator and persist `MC/common/sanity_test_results_<generator>_<date>.json`.
- [ ] Phase 0 / validation: verify `frac_lambda_ancestor == 0`, `frac_k0s_ancestor == 0`, `frac_klong_ancestor == 0`, `frac_sigma_pm_ancestor == 0`, `frac_xi_ancestor == 0`, and `frac_omega_ancestor == 0` for each launched generator.
- [ ] Phase 0 / validation: prove the skeleton pipeline can build both `fullps` and `fid` prediction JSONs from the same pilot ROOT ntuple sample.
- [ ] Phase 0 / review: run `h1-analysis-critic` on the shared design before submitting any pilot jobs.
- [ ] Phase 1a / implementation: finish the body of `MC/scripts/reduce_hepmc_to_root.py`; Phase 1a cannot start until orientation detection, rotation, metadata propagation, and event-audit branches are live.
- [ ] Phase 1a / pilot prep: prepare one standard Condor submit file for a `100k`-event Pythia8 `ep` pilot and one for a `100k`-event Pythia8 `em` pilot.
- [ ] Phase 1a / pilot prep: validate that `H1/sidis_highQ2/MC/pythia8/cmnd/pythia8_dis_nominal.cmnd` contains `PhaseSpace:Q2Min = 150.`, the NNPDF3.1 LO LHAPDF6 lines, lepton-line QED-off lines, quark-line QED-on lines, and all 8 `mayDecay = off` entries.
- [ ] Phase 1a / pilot run: submit one `100k`-event Pythia8 `ep` Condor pilot job and reduce it to per-job ROOT plus `fullps` and `fid` prediction JSONs.
- [ ] Phase 1a / pilot run: submit one `100k`-event Pythia8 `em` Condor pilot job and reduce it to per-job ROOT plus `fullps` and `fid` prediction JSONs.
- [ ] Phase 1a / validation: run schema validation on the Pythia8 pilot prediction JSON pair.
- [ ] Phase 1a / validation: verify `η_lab` is in HERA convention after reduction (`η > 0` proton-side / forward, `η < 0` electron-side / backward).
- [ ] Phase 1a / validation: confirm the event metadata field `beam_orientation_correction_applied` and the branches `beam_electron_pz_after_rotation` / `beam_proton_pz_after_rotation` are filled and consistent.
- [ ] Phase 1a / validation: run `build_prediction_from_root.py --selection both` and confirm both `fullps` and `fid` JSONs are produced from the same ROOT input.
- [ ] Phase 1a / plotting: add `MC/scripts/compare_fid_vs_fullps.py` and produce the first Pythia8 fid-vs-fullps comparison plot plus per-cell CSV.
- [ ] Phase 1a / plotting: add `MC/scripts/compare_data_vs_mc.py` and prove it loads the pilot `fullps` and `fid` JSONs, `xsec_v3_HQv5gBprime_2026-05-24.json`, `unfolded_4d_v3_IBU.json`, and `v5g_4d_trust_mask.json` end-to-end.
- [ ] Phase 1a / validation: record Pythia8 runtime, events/sec, HepMC size, ROOT size, accepted-DIS fraction, and representative 4D-cell occupancies.
- [ ] Phase 1a / review: run `h1-analysis-critic` on the Pythia8 pilot outputs and block Phase 1b/2 if any convention or normalization problem is found.
- [ ] Phase 1b / blocker resolution: resolve Sherpa open item P1 #1 for the stable-particle YAML key before any hadron pilot is submitted.
- [ ] Phase 1b / pilot prep: prepare `MC/sherpa/Sherpa.yaml` with the proven `SELECTORS` syntax, `YFS_MODE: 0` candidate, and the resolved stable-particle block; include the required `LD_LIBRARY_PATH` workaround in the wrapper.
- [ ] Phase 1b / pilot run: submit one `100k`-event Sherpa pilot job only after P1 #1 resolves, then reduce to per-job ROOT plus `fullps` and `fid` prediction JSONs.
- [ ] Phase 1b / validation: verify Sherpa long-lived-wall ancestor fractions remain exactly zero and that the downstream ROOT → JSON → plot pipeline matches the Phase 1a contract.
- [ ] Phase 1c / blocker resolution: resolve Pythia6 open item P1 #2 for the LHAPDF6 bridge before any physics pilot is submitted.
- [ ] Phase 1c / implementation: implement `MC/pythia6/driver/pythia6_dis_driver.f90` (or `.f`) and `MC/pythia6/driver/pythia6_to_hepmc3.cxx` once the PDF bridge path is known.
- [ ] Phase 1c / pilot run: submit one `100k`-event Pythia6 pilot job only after P1 #2 resolves, then reduce to per-job ROOT plus `fullps` and `fid` prediction JSONs.
- [ ] Phase 1c / validation: re-run the Q² smoke-style comparison with the real LHAPDF6 bridge and confirm the high-Q² tail collapse relative to the dummy-PDF behavior before accepting any hadron pilot output.
- [ ] Phase 2 / scaling: use pilot occupancies to compute the required total events per generator so representative trusted cells meet the `≤ 5% of data-stat uncertainty` target.
- [ ] Phase 2 / scaling: check `/data/dust/group/h1/.quota-usage.csv` and compare expected raw + reduced footprint to available headroom before launch.
- [ ] Phase 2 / production: choose final job counts per generator/beam-mode, targeting roughly `50–200` jobs per generator overall while keeping within standard-job runtime.
- [ ] Phase 2 / production: launch full-stat Pythia8 production on Condor, reduce per job to ROOT ntuples, build a skim ROOT, and write final Pythia8 `fullps` and `fid` prediction JSONs.
- [ ] Phase 2 / production: launch full-stat Sherpa production on Condor only after Phase 1b closes, reduce per job to ROOT ntuples, build a skim ROOT, and write final Sherpa `fullps` and `fid` prediction JSONs.
- [ ] Phase 2 / production: launch full-stat Pythia6 production on Condor only after Phase 1c closes, reduce per job to ROOT ntuples, build a skim ROOT, and write final Pythia6 `fullps` and `fid` prediction JSONs.
- [ ] Phase 2 / validation: verify merged provenance fields include git SHA, run-card SHA, Condor cluster IDs, environment tag, seeds, and event totals.
- [ ] Phase 2 / validation: confirm final JSON axis order is `(Q²x_cell_idx, z_idx, pTh_idx, charge_idx)` with `charge_idx 0=h+`, `1=h-`.
- [ ] Phase 2 / validation: confirm final JSON `selection` field is present and valid for both `fullps` and `fid` outputs.
- [ ] Phase 2 / validation: confirm the `species_definition` and `composition_validation` blocks are present, correct, and still satisfy zero long-lived-wall ancestor requirements after full merge.
- [ ] Phase 2 / validation: confirm the optional `phi_h_hist` block is either present and valid or cleanly omitted with schema compliance.
- [ ] Phase 2 / validation: add the on-real-event regression that checks the reducer `R_y(180°)` closure against generator events, not just the analytic unit test.
- [ ] Phase 2 / review: run `h1-normalization` on the merged outputs to sign off lumi-equivalent metadata and charge-mix combination.
- [ ] Phase 2 / review: run `h1-analysis-critic` on the full-stat predictions before producing final comparison plots.
- [ ] Phase 3 / comparison: finalize `MC/scripts/compare_data_vs_mc.py` plotting outputs for `M^{h±}` vs `z` per `pT_h` slice in each `(Q²,x)` cell with data points, MC-fid solid, and MC-fullps dashed.
- [ ] Phase 3 / comparison: finalize `MC/scripts/compare_data_vs_mc.py` plotting outputs for `M^{h±}` vs `pT_h` per `z` slice in each `(Q²,x)` cell with data points, MC-fid solid, and MC-fullps dashed.
- [ ] Phase 3 / comparison: finalize comparison plots for `<cos φ_h>` and `<cos 2φ_h>` vs `z` and vs `pT_h` in each `(Q²,x)` cell.
- [ ] Phase 3 / comparison: optionally add `sin`-moment and raw `φ_h` distribution overlays if pilot/full-stat outputs support them cleanly.
- [ ] Phase 3 / comparison: run `MC/scripts/compare_fid_vs_fullps.py` for each generator and produce per-cell ratios `M_fullps / M_fid`, `σ_fullps - σ_fid`, integrated outside-fid fractions, and plot grids.
- [ ] Phase 3 / comparison: assemble `MC/plots/data_vs_mc_v1.pdf` from the approved panel set.
- [ ] Phase 3 / interpretation: dispatch `h1-mc-expert` to review generator-physics caveats for the first comparison package.
- [ ] Phase 3 / review: dispatch `h1-analysis-critic` on the assembled comparison package before memo drafting.
- [ ] Phase 3 / memo handoff: hand approved plots, metadata, and caveats to `doc-writer` for a results memo under `H1/sidis_highQ2/docs/results/`.
- [ ] Phase 4 / deferred: prepare PDF-variation steering hooks for all three generators.
- [ ] Phase 4 / deferred: prepare renormalization/factorization scale-variation hooks for all three generators.
- [ ] Phase 4 / deferred: prepare tune-variation hooks where generator infrastructure allows.
- [ ] Phase 4 / deferred: prepare `rad_qed=true` steering variants as a radiative cross-check against the nominal rad-off prediction.
- [ ] Phase 4 / deferred: add schema hooks for `A_BC` prediction outputs once beam-charge-asymmetry scope is promoted beyond stretch-goal status.

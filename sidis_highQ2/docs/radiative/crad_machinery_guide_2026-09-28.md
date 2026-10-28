# C_rad machinery guide: tools, data products, and how to use them (high-Q2 charged-hadron SIDIS)

**STATUS: DIAGNOSTIC. `c_rad_applied=false`. Central v6 untouched.**

This guide is for a future subagent picking up the gen-only QED radiative-correction machinery cold. It is operational reference material, not a release-application procedure.

## 1. Purpose

This machinery derives the gen-only QED radiative correction

`C_rad = sigma_nonrad/sigma_rad`

for high-Q2 charged-hadron SIDIS (`Q2>150 GeV^2`) using DJANGOH14 RAD versus NONRAD samples.

**Read first:** [`crad_session_pitfalls_2026-09-28.md`](crad_session_pitfalls_2026-09-28.md). It lists seven defects and the wrong `C_rad` value each produced.

## 2. Tree builder

| Item | Value |
|---|---|
| Source | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v3.C` |
| Binary | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/RC_treebuilder_genonly_v3` |
| H1OO setup | `source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh` |
| Build | `make bin BIN=RC_treebuilder_genonly_v3` |

Never run `make all`.

v1 is the **FROZEN audit baseline**. Do not modify it. v3 is additive over v1.

### Output trees

| Tree | Row definition | Event-key rule |
|---|---|---|
| `GenDIS` | One row per generated DIS event; event-complete because `dis.Fill()` is unconditional. | `(RunNumber,EventNumber)` is unique **within a file**. |
| `ChargedGenRC` | One row per pi/K/p candidate. Event-level branches are carried through the shared `EVENT(T)` macro. | Do not assume keys are unique across files. |

### Key v3 branches

| Category | Branches and use |
|---|---|
| Hard-vertex kinematics | `Q2Gki`, `XGki`, `YGki`, and `gki_valid`. These are true hard-vertex kinematics; DJANGOH's `Q2MIN` cut acts on them. |
| Framework e-Sigma | `Q2esGen`, `YesGen`, `XesGen`. This is what the CENTRAL response bins in. |
| Corrected custom electron method | `Q2_es_gen`, `Y_es_gen`, `x_es_gen`. |
| Historical bug branches | `Q2_es_gen_v1bug/...` contain swapped-sin/cos values. **Never use for physics.** |
| Radiation and beam state | `GenRad` (`0` none, `1` resolved ISR, `2` resolved FSR); `GenEnPhoton`; `GenBeamElecE_eff`. |
| Four-vectors | Full hadron/electron/beam vectors: `GenPx..GenE`, `GenElecPx..`, `GenBeamElecPx..`, `GenBeamProtPx..`. |
| Provenance | `Mother1`, `Mother2`, `IsGtr`, `MotherPDG_v3`, `ParticleSource_v3`, `VetoAncestorPDG_v3`, `IsV0DaughterGen_v3`. |

**Warning:** `GenRad` and `GenBeamElecE_eff` are registered on `ChargedGenRC` only, not `GenDIS`. Event-level work needing either branch must deduplicate `ChargedGenRC` per file (see script 07) or extend the builder.

**Warning:** Do **not** use `pass_truth_event_fiducial` or `pass_truth_hadron_fiducial` as the fiducial. The analyst applies true fiducial cuts at migration-matrix build time; this tree is deliberately loose.

## 3. Input data products

Base output directory:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/`

| Product | Beam directories | File count | Description |
|---|---|---:|---|
| RAD | `rad/{ep,em}/` | 731 / 642 | DJANGOH14 `RAD.Q2GT60`, official OO-4.0 HAT+MODS, detector-simulated. |
| OLD NONRAD | `nonrad_old/{ep,em}/` | 260 / 260 | Legacy Britzger `NORAD.Q2GT100`. |
| NEW NONRAD | `nonrad_new/{ep,em}/` | 530 / 530 | User campaigns: Stage1 DJANGOH -> Stage2 dst2all -> Stage3 truth tree; **NO GEANT**. |
| Exposure manifest | `input_output_exposure_manifest.csv` | -- | Retained-input and output exposure inventory. |

There are **2953 partials** in total.

Excluded and quarantined: July e- campaign `hk_0123_500x10k_prod_20260708` (500 files; duplicate-RNIN defect).

## 4. Exposures (weighted convention)

Use

`L_MC = sum_generated(weight1*weight2)/SIGMT`.

DJANGOH MCWE/GKI weights are non-unit: mean `weight1 ~3.46` for RAD and `~2.53` for NONRAD. `N/SIGMT` is wrong; that error alone gave `C_rad 2.04` instead of `1.11`.

| Sample | ep | em |
|---|---:|---:|
| RAD | 16,206,393.0 | 13,534,017.0 |
| OLD NONRAD | 12,920,115.855 | 12,841,572.442 |
| NEW NONRAD | 1,717,376.821 | 1,699,702.771 |
| Combined NONRAD | 14,637,492.676 | 14,541,275.213 |

Beam mixture: ep `0.5140930272`; em `0.4859069728`.

`L_data` never enters the ratio.

Pool OLD+NEW by summing raw weights **first**, then dividing by summed exposure. All values are `DIAGNOSTIC_MANIFEST_LEVEL`: exact for retained manifests, not raw-generator certified. The blocker is `P-CRAD-LUMI`.

## 5. Release-contract fiducial

### Event and hadron selection

- `150 <= Q2esGen < 20000 GeV^2`
- `0.1 < YesGen < 0.7`
- Seven `(Q2,x)` cells:
  1. `(150,300,.0021,.0071)`
  2. `(150,300,.0071,.030)`
  3. `(300,700,.0042,.016)`
  4. `(300,700,.016,.069)`
  5. `(700,1500,.0099,.034)`
  6. `(700,1500,.034,.150)`
  7. `(1500,20000,.021,1.5)`
- `GenEmpz > 45` nominal. See the open EMPZ question in the pitfall log.
- Species `{211,321,2212}`.
- Charge by `GenCharge` sign.
- `IsGtr==1`.
- `Mother2==-1`.
- V0-daughter veto through `IsV0DaughterGen_v3`.
- No truth eta cut.
- No `xF` cut.

### Published-bin layout

| Dimension | Edges / rule |
|---|---|
| `z` | `[0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]` |
| `pT_h` | `[0,0.5,1,2,4,8]` |
| Per-charge bin index | `cell*35 + z_idx*5 + pT_idx` |
| h+ rows | `0-244` |
| h- rows | `245-489` |
| Total rows | `490` |

### Required `pT_h` definition

`pT_h` must be the gamma*p CM quantity. Boost the hadron and `q` to the `W=q+P` rest frame with `beta = -W_vec/W_E`, then project transverse to the CM photon direction.

A lab-frame projection is wrong and is not release-compatible. The implementation matches central helper `build_migration_th2_per_charge_v6.py` to max absolute difference `0.0 GeV`. Preserve this as a hard gate.

### RAD domain match

Apply `Q2Gki>100` to the **NUMERATOR ONLY**. Do not rescale `L_RAD`. A proposed rescale by generated-domain `f_w=0.511` was refuted; it drives `C_rad` to `~0.59`.

## 6. Histogramming worker and finalizer

| Component | Path |
|---|---|
| Worker | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py` |
| Finalizer | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_finalize.py` |

The worker emits `6` EMPZ modes x `3` q variants = `18` partials per invocation. It writes y-resolved `(6,490)` arrays over `0.1<y<0.7` for the closure test. Projecting over y reproduces the 490-bin result bit-for-bit.

### EMPZ modes

`empz_gt45` (nominal), `empz_gt50`, `empz_gt55`, `empz_45_70`, `empz_35_70`, `empz_none`.

### q variants

| Variant | Definition |
|---|---|
| `nominal` | `q = k_beam - k'_electron` |
| `sigma_photon` | Rebuilt from `Q2sGen`, `YsGen`, `phi_e`. |
| `esigma_photon` | Rebuilt from `Q2esGen`, `YesGen`, `phi_e`. |

Photon reconstruction:

`q = (sqrt(Q2(1-y))cos(phi_q), sqrt(Q2(1-y))sin(phi_q), -E_e*y - Q2/(4E_e), E_e*y - Q2/(4E_e))`

with `phi_q = phi_e + pi`.

Closure: `|q.q+Q2|/Q2 < 5.46e-16`; y to `4.44e-16`.

### Completed campaigns

| Campaign | Contents | Status |
|---|---:|---|
| `.../output_v3/histogramming_v3_empz45_20260927/` | 51,912 partials, 4 modes | Integrity PASS. |
| `.../output_v3/histogramming_v3_empz6_20260927/` | 77,868 partials, 6 modes | Integrity PASS. |

## 7. Standalone diagnostic scripts

Directory:

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/crad_diagnostics_2026-09-28/`

All scripts are plain `python3 + uproot + numpy`, run on the NAF login node, read partials directly, and require no Condor. Several take minutes: run with `nohup` and poll.

| Script | What it does | Key output |
|---|---|---|
| `01_closure_split_old_vs_new.py` | Computes `C_rad` in the quiet `y in [0.1,0.2)` control three ways: OLD-only, NEW-only, and pooled OLD+NEW; per beam and q variant. | OLD/NEW agree to 0.4%: OLD/NEW = `0.996` ep, `1.003` em, exonerating pooling for the ratio. |
| `02_crad_y_dependence.py` | Computes `C_rad` in 6 y bins over `0.1-0.7`, per beam, for selected EMPZ modes. | Shape is flat to slightly falling: `1.158 -> 1.108`, not ISR-rising. |
| `03_empz_mode_scan.py` | Computes integrated `C_rad`, y1/y6, and slope for every EMPZ mode x q variant from empz6 campaign partials. | The 45/50/55 scan table. |
| `04_fw_infiducial.py` | Computes weighted matched/unmatched ratio from campaign partials: in-fiducial impact of `Q2Gki>100`. | `0.9993`, confirming the domain cut is negligible in-fiducial. |
| `05_isr_multiplicity_v2.py` | Computes correct per-event hadron multiplicity: `<n_had>` per fiducial DIS event for RAD/NONRAD, versus `GenEnPhoton`, and versus ISR veto. Uses file-scoped joins. | The `~1.05` multiplicity ratio and ISR suppression curve. |
| `06_make_isr_plots.py` | Renders figures 1-4 from the v2 JSON. | PDF and PNGs. |
| `07_radiation_fractions.py` | Computes weighted fraction of fiducial RAD events with `GenRad` 0/1/2. | none `0.6266`, ISR `0.1729`, FSR `0.2005`, any `0.3734`. |
| `QUARANTINE_isr_multiplicity_v1_BUGGY.py` | **Do not use.** Uses global `np.isin` joins across files. NONRAD `(Run,Event)` keys repeat across files: 27.6% OLD and 61.9% NEW, so hadrons were over-counted. | Spurious ratio `1.65`. Retained only as a worked bug example. |
| `QUARANTINE_decomposition_INVALID.py` | **Do not use without fixing.** Sums weights over only 40 files per sample while dividing by full-manifest luminosity. | Nonsensical event-rate factor `2.82/2.37`. Rerun over the full manifest. |

## 8. Critical gotchas for a future subagent

1. `(Run,Event)` keys are **not** unique across NONRAD files. Join hadrons to events **within a single file**. This is the easiest way to obtain a badly wrong result.
2. `GenEnPhoton` has an unphysical tail: `0.052%` of RAD events exceed the `27.6 GeV` beam energy, with maximum `237.0 GeV`. Before using it, apply `GenEnPhoton < GenBeamElecE`.
3. Use `isr_multiplicity_check_v2.json`, never `isr_multiplicity_check.json`; the latter is superseded and quarantined.
4. Never use `*_v1bug` branches for physics.
5. Keep the two `C_rad` definitions distinct. The binned cross-section ratio carries both a DIS event-rate factor and a per-event multiplicity factor. The multiplicity ratio carries only the second. Which is correct for a multiplicity measurement is an **OPEN** question.

## 9. Operational notes (Condor / NAF)

- `thish1.sh` returns non-zero and references an unset `GROUPROOT`. Wrappers must not use `set -e` or `set -u` around it. Source tolerantly and validate the environment positively. Violating this gives empty logs and 100% failure.
- HTCondor `retry_until` is STOP-on-true. For AFS-flaky nodes use `retry_until = (ExitCode != 127)`. The inverted `==` form never retries.
- `MAX_JOBS_PER_OWNER = 5000` on this schedd. Unrelated held jobs consume quota. Chunk large campaigns; the proven split was `1373/1373/1580`.
- Request `4000 MB`; a few `202 MB` nonrad_old inputs peak at `2442 MB`.
- Validate wrappers with a real multi-job Condor test, never login-node only.
- Regression-compare against a freshly rebuilt baseline binary, never a stale historical partial.
- Proven infrastructure to copy: `.../condor/histogramming_v3_empz6_20260927/`.

## 10. Current best numbers

### Binned cross-section ratio, variant A

| EMPZ mode | `C_rad` |
|---|---:|
| `empz_none` | 1.0999 |
| `35-70` | 1.1127 |
| `>45` | 1.1372 |
| `>50` | 1.1699 |
| `>55` | 1.3632 |

The y slope is `-2.5%` to `-4.4%`; for `>55` it is `-10.3%`.

### Per-event multiplicity ratios at `>45`

| Selection | e+ | e- |
|---|---:|---:|
| Nominal | 1.0555 | 1.0677 |
| ISR veto | 1.0467 | 1.0593 |

### `<n_had>` per fiducial DIS event

| Sample | e+ | e- |
|---|---:|---:|
| RAD all | 13.9669 | 13.9402 |
| RAD no-ISR | 14.0834 | 14.0509 |
| NONRAD pooled | 14.7417 | 14.8841 |
| OLD | 14.7873 | 14.9437 |
| NEW | 14.0358 | 13.9610 |

### Fiducial RAD radiation fractions, e+

| Category | Fraction |
|---|---:|
| None | 0.6266 |
| Resolved ISR | 0.1729 |
| Resolved FSR | 0.2005 |
| Any radiation | 0.3734 |

`<E_gamma|ISR> = 1.40 GeV`; median `0.86 GeV`.

# v6sys H1-precedent systematics menu — implementation status (high-Q² charged SIDIS)

**Date:** 2026-06-03
**Workspace:** `H1/sidis_highQ2/`
**Owner:** code-editor (under h1-sidis-supervisor)
**Frozen policy artifact:**
`H1/sidis_highQ2/docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md`
**Scope:** Document what of the frozen H1-published-precedent systematics
menu has been implemented now in the current v6sys / high-Q² release
artifacts, what remains as machine-readable BLOCKED stubs, and the
exact next step required to close the full numerical systematic
covariance.

> **No H1OO builder files or v6sys treebuilder files were modified.**
> **No `H1CalcSystematic` shifts were invoked for tracking.**
> **No numerical covariance was emitted for any migration-affecting
> electron/HFS source.** **No `--sys-allow-default` fallback was used.**
> **No systematics were invented beyond the frozen menu.**

---

## 1. Releases addressed

Both current rev6 release directories were processed:

- `data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/`
  (FINALBIN schema `h1_sidis_highQ2_finalbin_release_v1`, 157 bins/charge)
- `data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/`
  (210BIN schema `unfolded_yields_210_filtered_v1`, 210 bins/charge)

Per-release artifact directories:

```
H1/sidis_highQ2/data/systematics_sources/v6sys_h1_precedent_manifest/
  data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/
    manifest.json
    README.md
    stub_electron_energy_BLOCKED.json
    stub_electron_theta_BLOCKED.json
    stub_hfs_jes_BLOCKED.json
    stub_hfs_rces_BLOCKED.json
    stub_hfs_theta_BLOCKED.json
    stub_hfs_phi_BLOCKED.json
  data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/
    (same layout)
```

The manifest builder script is
`H1/sidis_highQ2/scripts/systematics/build_v6sys_h1_precedent_manifest.py`.
It is schema-aware (handles both FINALBIN and 210-bin layouts) and
re-runnable per release.

---

## 2. Category-by-category status

### 2.1 Category A — already present / reformat-only

The following pre-existing pipeline artifacts are indexed by the
manifest. They are NOT reformatted by this work (per task: "do not
recompute if not available"); a separate code-editor pass owns the
JSON-source reformat into `source_*.json` shape consumed by
`combine_systematics.py`.

| source_id | Existing artifact(s) | Manifest status |
|---|---|---|
| `model_DJvsRG` | `data/unfolded_4d_v3final_data_DJvsRG.json` (+ v3p2, v3) | `PRESENT_REFORMAT_REQUIRED_BY_CODE_EDITOR` |
| `unfolding_reg` | `data/sigma_reg_TUnfold_4d_v3final.json`, `data/tunfold_official_v3final/sigma_reg_TUnfoldOfficial_v3final.json` | `PRESENT_REFORMAT_REQUIRED_BY_CODE_EDITOR` |
| `unfolding_stat` | `data/finalbin_toys_djangoh/*_{pos,neg}.json` and `.npz` | `PRESENT_REFORMAT_REQUIRED_BY_CODE_EDITOR` |
| `MC-stat,response` | `data/sigma_MCstat_TUnfold_4d_v3final.json`, `data/mcstat_replicas_tau0p1/` | `PRESENT_REFORMAT_REQUIRED_BY_CODE_EDITOR` |
| `C_rad` | `data/crad_v3_HQv5gBprime_2026-05-24.json` | `PRESENT_REFORMAT_REQUIRED_BY_CODE_EDITOR` |

If a future release branch retires any of these, the manifest records
`missing_existing_artifact` instead of inventing values.

### 2.2 Category B — post-unfolding diagonal, implemented now

#### `tracking_central_h1_1302_1321` (DONE)

For both releases, the per-release artifact emitted by
`emit_source_tracking_central_h1_1302_1321.py` is present and validates:

- `sigma_rel` exactly = `0.01019803902718557` = √(0.010² + 0.002²)
  (1.0% central tracking ⊕ 0.2% K0S contamination; arXiv:1302.1321 §5).
- Per-bin diagonal-only covariance: off-diagonal entries are strictly
  zero (`cov - diag(diag(cov)) == 0` everywhere).
- Per-bin relative sigma constant to ≤ 1.7×10⁻¹⁸ (machine precision)
  across all populated bins, for both charges.
- `applies_per_charge_independently = true`, cross-charge cov block = 0.
- Explicitly excludes: 10% combined/forward tracks, `eElecTr*`,
  `H1CalcTrackDowngrade`, track momentum scale/resolution, any
  H1Calc shift, MC shifted-response sources, v6sys treebuilder
  variations. None of those were introduced.

Per-release bin counts (sanity):

| release | n_pos | n_neg |
|---|---|---|
| FINALBIN | 157 | 157 |
| 210bin   | 210 | 210 |

Manifest status for both releases: `PRESENT_AND_VALIDATED`.

#### `tracking_chargedep` (sanity-check, no regeneration)

Existing `data/systematics_sources/source_tracking_chargedep.json`
indexes a per-track-flat residual of 0.3% (`shift_magnitude = 0.003`),
anti-correlated h⁺/h⁻, rank-1 structure (per `research-tracking-systematics-deep.md`).
The frozen menu §1.2 row #11 calls for sanity-check against the Aaron
2009 PLB 681 §6 envelope of 2.5% on `A_h`.

The two values reflect different conventions (per-track flat bound vs
on-`A_h` envelope) and the ratio 0.003 / 0.025 = 0.12 is outside the
±30 % window. Per the frozen policy ("no invention") the manifest
flags this as **`PRESENT_SANITYCHECK_FAILED_REVIEW_REQUIRED`** with
review owner `h1-systematics`. No regeneration is performed; no new
shift magnitude is written.

`h1-systematics` decides whether to keep the existing 0.3%/track
convention or regenerate to the 2.5%-on-`A_h` envelope (which would
require re-emitting the source NPZ with the larger magnitude).

### 2.3 Category C — migration-affecting H1Calc sources, BLOCKED stubs only

Six BLOCKED metadata stubs are emitted per release. Each stub carries
the H1Calc enum, the integer value, the pinned ± magnitude, the
explicit setter recipe, magnitude provenance, the Stage1+Stage2 rerun
plan, the six common blockers, the seven validation gates, the
critic verdicts from previous rank-1 attempts (where applicable), and
the explicit assertion `cov_matrix_emitted: false`,
`do_not_fill_with_rank1_cov: true`.

| source_id | H1Calc enum | enum value | magnitude (sym ±) | application |
|---|---|---|---|---|
| `electron_energy` | `eElecEn` | 7 | ±0.005 | per LAr wheel × 5 |
| `electron_theta` | `eElecTh` + co-shifted `eElecTrTh` | 10 + 13 | ±0.001 (rad) | per θ region × 3 + matched track θ |
| `hfs_jes` | `eHadEn` | 0 | ±0.01 | per HFS object inside kT R=1.0 pT>5 GeV jet (η_lab∈[−1.5, 2.75]) |
| `hfs_rces` | `eHadEnRCES` (verify in relvol10/4.2.1) | 1 | ±0.01 | per HFS object outside any such jet |
| `hfs_theta` | `eHadTh` | 4 | ±0.001 (rad) | per HFS object, uniform 1 mrad |
| `hfs_phi` | `eHadPh` | 5 | ±0.020 (rad) | per HFS object, uniform 20 mrad |

Symmetry is required: both up and down sides MUST be produced (SIDIS
A_h needs symmetric ± for physical charge cancellation; the g1j_ES
one-sided convention is NOT acceptable for our analysis).

The `hfs_jes` stub additionally encodes the jet-classifier verification
requirement (kT R=1.0 pT>5 GeV, Andreev 2023 §1) and the fallback
(collapse JES+RCES into a single ±1% uniform HFS shift) in case the
v6sys jet collection cannot be matched.

The `hfs_rces` stub flags the enum-integer verification requirement
(`relvol10/4.2.1` may use `eHadEnCor=1` rather than `eHadEnRCES=1`;
`h1-software` to confirm).

The `electron_energy` and `electron_theta` stubs explicitly **supersede**
the existing critic-FATAL rank-1 sources
(`source_electron_energy.json`, `source_electron_theta.json`) and
encode the Stage-1 admission-boundary bias reason (a Stage2-only run
on a fixed Stage 1 event sample cannot recover events that cross
admission boundaries under the shift).

### 2.4 Category D — handled outside v6sys

| source_id | Existing artifact | Manifest status |
|---|---|---|
| `lumi` | `data/systematics_sources/source_lumi.json` (2.7%) | `PRESENT_INDEXED_PENDING_NORMALIZATION_REVIEW` |
| `trigger` | `data/systematics_sources/source_trigger.json` (0.5% placeholder ceiling) | `PRESENT_INDEXED_PLACEHOLDER_CEILING` |
| `NucliaWeight` | `data/systematics_sources/source_NucliaWeight.json` | `PRESENT_INDEXED` |
| `gamma_p_background_30pct` | — | `EXTERNAL_NOT_SUBTRACTED_NEGLIGIBLE_AT_HIGH_Q2` |

The `lumi` entry carries the high-Q² caveat that `h1-normalization`
must sign off whether the 2.7% HERA-II combined value applies to the
v6sys high-Q² release as-is. No revised magnitude is invented here.

The `gamma_p_background_30pct` entry is indexed but not emitted as a
source because no γp template is currently being subtracted from data
in the rev6 pipeline (Andreev 2023 §4 explicitly characterizes
background as "negligible" at high-Q² SIDIS under y<0.7 + Empz∈[35,70]).

### 2.5 Category E / dropped / not applicable

Recorded as machine-readable exclusions so later agents do not retry:

- `combined_forward_tracking_10pct` — excluded by |η_h|<1.3 fiducial.
- `spacal_electron_energy`, `spacal_electron_theta` — wrong regime
  (LAr at high-Q², not SpaCal).
- `electron_phi` — DROPPED from baseline; no g1j_ES recipe and no
  published HERA-II `eElecPh` magnitude. Existing critic-FATAL
  `source_electron_phi.json` superseded (do not carry forward).
- `noise_energy` — DROPPED; no pinned HERA-II SIDIS magnitude for
  `eNoiseEn`.
- `tracking_resolution` — DROPPED; no H1-paper precedent for an
  explicit tracking-precision systematic. Existing critic-FATAL
  `source_tracking_resolution.json` superseded.
- `legacy_tracking_efficiency` — DEPRECATED; superseded by
  `tracking_central_h1_1302_1321` (post-unfolding 1.0198% diagonal).
- `eElecTr_star_combined` — `eElecTrEn`, `eElecTrPh` are orthogonal;
  `eElecTrTh` is co-shifted inside `electron_theta` only.
- `H1CalcTrackDowngrade` — orthogonal calculator-level shift, not in
  the H1-published menu for this analysis.
- `method_spread_BBB_IBU_TUnfold` — coverage diagnostic, NOT a
  systematic (per `unfolding_systematic_decomposition_2026-05-24.md`).

---

## 3. Validation results

Run on 2026-06-03 from
`H1/sidis_highQ2/scripts/systematics/build_v6sys_h1_precedent_manifest.py`:

1. **py_compile**: PASSES.
2. **`tracking_central_h1_1302_1321`** (both releases):
   - `sigma_rel` in JSON = `0.01019803902718557` (exact match to
     expected constant).
   - Per-bin rel-sigma max deviation from constant: ≤ 1.7×10⁻¹⁸ (both
     charges, both releases).
   - Diagonal-only cov NPZ: `cov - diag(diag(cov)) == 0` exactly.
   - `cov_diag == sigma**2` element-wise.
   - `applies_per_charge_independently == true`, cross-charge cov = 0.
   - Bin counts: 157/157 (FINALBIN), 210/210 (210BIN).
3. **`tracking_chargedep`**: flagged
   `PRESENT_SANITYCHECK_FAILED_REVIEW_REQUIRED`; no regeneration
   performed.
4. **Category C stubs**: all six per release have
   - `status == "BLOCKED_PENDING_SHIFTED_RESPONSE"`,
   - `cov_matrix_emitted == false`,
   - `do_not_fill_with_rank1_cov == true`,
   - no `cov`, `covariance`, `cov_matrix`, `cov_diag`, `cov_full`,
     `sigma_per_bin`, `delta_signed_per_bin`,
     `fractional_sigma_per_bin`, `unfolded_up`, or `unfolded_down`
     fields leaked.
   - No reference to `--sys-allow-default` anywhere in the stub text.
5. **No undefined H1Calc magnitude listed as production-ready**: every
   stub explicitly declares
   `cov_matrix_emitted_reason == "BLOCKED_PENDING_SHIFTED_RESPONSE"`.
6. **No H1OO / v6sys builder files modified**: `git status` shows no
   changes to `h1oo-master/` or any v6sys treebuilder file.

---

## 4. Remaining blockers before a complete numerical systematic covariance

Per the per-release manifest `blocking_summary`, the full numerical
post-unfolding systematic covariance cannot be assembled until the
following blockers close:

- Reformat the five Category-A pipeline artifacts (`model_DJvsRG`,
  `unfolding_reg`, `unfolding_stat`, `MC-stat,response`, `C_rad`)
  into the `source_*.json` schema consumed by
  `combine_systematics.py`. Owner: code-editor.
- `tracking_chargedep` convention decision (keep 0.3%/track or
  regenerate to 2.5% on `A_h`). Owner: h1-systematics.
- `lumi` high-Q² applicability review of the 2.7% magnitude.
  Owner: h1-normalization.
- **All six Category-C migration-affecting sources** require paired
  Stage1+Stage2 v6sys shifted-MC response production + response-matrix
  rebuild + per-iSys unfolding + noise-floor characterization +
  critic sign-off. Owners: h1-builder-engineer + h1-naf-operator +
  h1-unfolder + h1-systematics + h1-analysis-critic.

Until those gates close, the per-source covariance contributions for
electron_energy, electron_theta, hfs_jes, hfs_rces, hfs_theta, and
hfs_phi remain at "not yet produced" in the combined budget. Per
policy, they MUST NOT be filled with rank-1 reweight covariances on
the 480-bin grid (that pattern was flagged critic-FATAL three times in
the rev6_v2 pipeline).

---

## 5. Exact next step for Stage1+Stage2 shifted MC response production

The frozen menu §4.D and §4.B prescribe the gate sequence. Concretely:

1. **h1-builder-engineer** confirms that the existing v6sys SIDIS
   treebuilder honours `ResetAllShifts(); Reset()` before evaluating
   `gH1Calc`-dependent Stage 1 preselection, and applies the per-wheel
   `SetShiftElecEnWheel(w, ±0.005)` array (not a single global
   `SetShift(eElecEn)`). Confirms `int(H1CalcSystematic::eHadEnRCES)`
   against `relvol10/4.2.1`. No builder code changes; verification
   only.
2. **h1-builder-engineer + h1-naf-operator** run a Stage 1
   event-processing smoke for each of the six syskeys (up side first)
   on at least one period of DJANGOH14 MC + data, on Condor. The
   smoke checks: per-period admitted-event count delta vs central,
   Stage 1 → Stage 2 chargedtot row count delta, per-bin mean
   ΔQ²es / ΔYes on the Scheme B′ grid.
3. **h1-naf-operator** then submits the full paired ± production for
   all six syskeys, per period (em0405, ep0405, ep0607a, ep0607b),
   per generator (DJANGOH14 nominal + RAPGAP cross-check). Expected
   total ~16 h Condor wall for electron_energy alone; allocate
   correspondingly for the other five.
4. **h1-unfolder** rebuilds the migration matrix per iSys (shifted
   reco + nominal gen) and reruns the TUnfold-4D unfolding of nominal
   data with each shifted response, per side.
5. **h1-systematics** computes per-bin σ_i = ½ |R_up(i) − R_down(i)|
   and characterizes the per-bin shift δ against the per-bin
   noise floor on the 480-bin grid; bins where σ_unfold ≳ |δ| are
   MASKED or noise-floor-corrected before any cov is admitted.
6. **h1-analysis-critic** signs off that the per-source covariance
   respects the noise floor and that no rank-1 outer-product cov is
   being shipped on noise-dominated bins.
7. Only then does code-editor replace each Category-C stub with a
   real per-source JSON + cov NPZ for the release.

Until step 7 closes for each source, that source's stub remains the
canonical artifact for the v6sys release.

---

## 6. Files created in this implementation

- `H1/sidis_highQ2/scripts/systematics/build_v6sys_h1_precedent_manifest.py`
- `H1/sidis_highQ2/data/systematics_sources/v6sys_h1_precedent_manifest/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/{manifest.json, README.md, stub_*_BLOCKED.json × 6}`
- `H1/sidis_highQ2/data/systematics_sources/v6sys_h1_precedent_manifest/data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/{manifest.json, README.md, stub_*_BLOCKED.json × 6}`
- `H1/sidis_highQ2/docs/methodology/v6sys_h1_precedent_systematics_manifest_2026-06-03.md` (this file)

No edits to H1OO sources, v6sys treebuilder, agent definitions, or
shared top-level `H1/` documentation.

# Canonical Systematics Data Contract — v3 (`HQ-v5g-Bprime`)

**Date:** 2026-05-24
**Owner:** `h1-systematics`
**Status:** FROZEN (Phase 1 of `plan-systematics-v5g-v3-2026-05-24.md`)
**Applies to:** all per-source uncertainty JSONs produced for the high-Q² charged-particle SIDIS analysis on the canonical v5g 4-period production with extractor v3 and binning `HQ-v5g-Bprime` (Scheme B′).

This memo is the single source of truth for the per-source uncertainty JSON
format. Every per-source dispatch in Phases 2–5 of the systematics plan
MUST emit JSON conforming to the schema below, and the combination
utility (`combine_systematics.py`) MUST refuse any JSON that does not.

Cited from:
- `H1/sidis_highQ2/docs/plans/plan-systematics-v5g-v3-2026-05-24.md`,
  Section "Output convention (single canonical JSON schema)" (line 123)
  and Section "Combination and reporting" (line 942).
- `H1/sidis_highQ2/docs/methodology/binning_adoption_HQ-v5g-Bprime_2026-05-24.md`
- `H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json` (nominal central reference).

This contract REPLACES the loose v5f convention used by the
`compute_systematics_v5f_*` scripts; those v5f scripts remain frozen for
audit but their JSON layout is not compatible and must not be combined
with v3 sources without translation.

---

## 1. Scope of one per-source JSON

A single per-source JSON describes the propagation of ONE systematic
source (e.g. luminosity, HFS scale, NuclIA clipping) to the nominal
v3 observables on all 8 Scheme B′ `(Q², x)` cells. It carries either
symmetric shifts (`delta_central_4D`) or asymmetric shifts
(`delta_up_4D` / `delta_down_4D`), plus the metadata needed to
combine it with other sources according to the rules in the plan.

---

## 2. Canonical observables and shapes

All per-source files must follow the canonical 4D layout from
`xsec_v3_HQv5gBprime_2026-05-24.json`:

| Object | Shape | Cell-level location |
|---|---|---|
| 4D maps | `[6 z bins, 5 pT_h bins]` per `(cell, charge)` | `cells[<cell_id>].by_charge[<charge>].<obs>_4D` |
| Integrated scalars | scalar per `(cell, charge)` | `cells[<cell_id>].by_charge[<charge>].<obs>_integrated` |
| φ_h harmonics | `[16 φ_h bins]` per `(cell, charge, harmonic_name)` (when defined) | `cells[<cell_id>].by_charge[<charge>].phih_harmonics.<name>` |

Cells (8): `S0a, S0b, S1a, S1b, S2a, S2b, S3a, S3b`.
Charges: `h+`, `h-`.

Observable families that every per-source JSON must address (or
explicitly skip with `null` and a reason):

| Observable family | Per-charge | Per-(Q²,x) cell layout |
|---|---|---|
| `M` (multiplicity `M^{h±}`) | yes | 4D `[6,5]` |
| `sigma` (cross section `d⁴σ/dQ²dxdzdpT_h`) | yes | 4D `[6,5]` |
| `A_h` (charge asymmetry) | NO (single value per (z,pT_h) bin) | 4D `[6,5]` |
| `phi_harmonics` (`A_UU^cosφ`, `A_UU^cos2φ`, etc.) | yes | per-harmonic, scalar or 1D over `φ_h` as defined |

`A_h` MUST be propagated by the source author by first shifting `h+`
and `h-` yields (or `M^{h+}` and `M^{h-}`) consistently with the
source's nature, then forming the varied `A_h`, then subtracting nominal.
Never combine independent h+ and h- shifts by hand prior to forming the
asymmetry. See `propagation_type` rules below.

---

## 3. Top-level JSON schema (REQUIRED)

```json
{
  "source_id": "<short_machine_id>",
  "source_name": "<human-readable name>",
  "production": "v5g_4period",
  "extractor": "extract_pertrack_v5g_fixedFrame_v3.py",
  "binning": "HQ-v5g-Bprime",
  "schema_version": "systematics_v3.0",
  "created": "<ISO-8601 datetime>",
  "nominal_inputs": {
    "xsec":   "H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json",
    "crad":   "H1/sidis_highQ2/data/crad_v3_HQv5gBprime_2026-05-24.json",
    "purity": "H1/sidis_highQ2/plots/v5g_purity_4D_v3.pdf"
  },
  "variation_description": "<one paragraph describing the variation>",
  "propagation_type": "<one of: local_reweight | shifted_extractor | alt_mc | analytic_constant>",
  "correlation_class": "<one of: correlated_across_bins | uncorrelated | anti_correlated>",
  "cancels_in_A_h": false,
  "observable_scope": {
    "M": true,
    "sigma": true,
    "A_h": true,
    "phi_harmonics": false
  },
  "axes": {
    "cells": ["S0a","S0b","S1a","S1b","S2a","S2b","S3a","S3b"],
    "charges": ["h+","h-"],
    "z_edges":   [0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0],
    "pTh_edges": [0.0, 0.5, 1.0, 2.0, 4.0, 8.0],
    "phih_n_bins": 16,
    "phih_range":  [-3.141592653589793, 3.141592653589793]
  },
  "status": "ok",
  "notes": "<optional free-form>",
  "cells": {
    "S0a": { ... per-cell block, see Section 4 ... },
    "S0b": { ... },
    "S1a": { ... },
    "S1b": { ... },
    "S2a": { ... },
    "S2b": { ... },
    "S3a": { ... },
    "S3b": { ... }
  }
}
```

### Top-level field semantics

| Field | Type | Meaning |
|---|---|---|
| `source_id` | string | Stable machine id, used as the dict key when combined. Must be unique across all sources. Convention: `source_<NN>_<short_name>` for ordering. |
| `source_name` | string | Human-readable label for plots and tables. |
| `production` | string | Fixed to `v5g_4period` for this analysis era. |
| `extractor` | string | Fixed to `extract_pertrack_v5g_fixedFrame_v3.py`. |
| `binning` | string | Fixed to `HQ-v5g-Bprime`. |
| `schema_version` | string | Fixed to `systematics_v3.0`. Bump on any incompatible change. |
| `nominal_inputs` | object | Absolute or repo-relative paths to the nominal artifacts the source was derived from. |
| `variation_description` | string | One paragraph describing what was varied and how. |
| `propagation_type` | enum | See Section 5. |
| `correlation_class` | enum | See Section 6. |
| `cancels_in_A_h` | bool | If true, the source's shifts on `A_h` are forced to zero by the combiner and the source contributes only to `M` and `sigma` totals. |
| `observable_scope` | object | Per-family on/off switch. If false, that family's arrays may be omitted or be all-`null`. |
| `status` | enum | `ok` / `blocked` / `deferred_to_unfolding` / `placeholder_not_for_total`. Only `ok` enters totals; others are preserved for bookkeeping. |
| `cells` | object | Per-cell uncertainty payload. MUST contain all 8 cell ids. |

---

## 4. Per-cell sub-schema

Each entry in `cells` carries the per-(cell) payload for both charges
and asymmetry. The exact key set depends on `propagation_type`:

```json
{
  "Q2_range": [150.0, 300.0],
  "x_range":  [0.0021, 0.0071],
  "by_charge": {
    "h+": {
      "M": {
        "delta_central_4D":  [[<6×5 floats or null>]],
        "delta_up_4D":       [[<6×5 floats or null>]],   // optional, asym
        "delta_down_4D":     [[<6×5 floats or null>]],   // optional, asym
        "delta_integrated":  <float or null>,
        "bbb_valid_4D":      [[<6×5 bools>]]
      },
      "sigma": { ... same shape ... }
    },
    "h-": { ... same as h+ ... }
  },
  "asymmetry": {
    "A_h": {
      "delta_central_4D": [[<6×5 floats or null>]],
      "delta_up_4D":      [[<6×5 floats or null>]],
      "delta_down_4D":    [[<6×5 floats or null>]],
      "delta_integrated": <float or null>,
      "bbb_valid_4D":     [[<6×5 bools>]]
    }
  },
  "phi_harmonics": {
    "h+": {
      "A_UU_cosphi":  { "delta_central": <float>, "delta_up": <float>, "delta_down": <float> },
      "A_UU_cos2phi": { ... }
    },
    "h-": { ... },
    "asymmetric": {
      "A_LU_sinphi": { "delta_central": <float>, ... }
    }
  }
}
```

### Required vs optional keys in the per-charge block

| Key | Required | Notes |
|---|---|---|
| `delta_central_4D` | YES | Symmetric shift. May be all-`null` if source is asymmetric-only, but the key must exist. Units: SAME as the central observable (absolute, not relative). |
| `delta_up_4D` | optional | Required only if source is asymmetric (`+` direction). |
| `delta_down_4D` | optional | Required only if source is asymmetric (`-` direction). |
| `delta_integrated` | YES if `M` family populated | Symmetric shift on the integrated multiplicity / cross section. |
| `bbb_valid_4D` | YES | Must mirror `bbb_valid_4D` from the nominal xsec JSON. Cells where `bbb_valid_4D[z][pTh] == false` are treated by the combiner as `deferred_to_unfolding` for that bin. |
| `phi_harmonics` block | optional | Required only if `observable_scope.phi_harmonics == true`. |

### Units convention (CRITICAL)

`delta_*_4D` values are **absolute shifts** in the same units as the
corresponding central observable. The combiner converts to relative
internally for percentage reporting. This avoids ambiguity when the
nominal is zero or near-zero in a low-stat bin.

Convention: `delta_central_4D[z][pTh] = X_var - X_nom` (signed).
For symmetric reporting the combiner uses `|delta_central_4D|`.
For asymmetric, the combiner uses `max(|delta_up|, |delta_down|)`.

---

## 5. `propagation_type` definitions

| Value | Meaning | How `delta_central_4D` is computed |
|---|---|---|
| `analytic_constant` | A pure relative constant on the observable (luminosity, common tracking-eff floor). | `delta = const_rel * |X_nom|` per bin. |
| `local_reweight` | Re-weight the nominal v3 per-track CSV without re-extracting (NuclIA clipping, beam-energy propagation, PDF reweight). | Recompute observables with shifted weights; `delta = X_var - X_nom`. |
| `shifted_extractor` | Re-run extractor v3 with a kinematic shift (HFS scale, electron energy, electron angle, Empz / VtxZ / y-window / DCA cut variations). | Recompute observables from the shifted extractor output. May produce asymmetric `delta_up_4D` / `delta_down_4D`. |
| `alt_mc` | Replace nominal MC sample with an alternative (DJANGOH vs RAPGAP31; nominal C_rad vs alternative C_rad). | `delta = 0.5 * (X_alt - X_nom)` per the plan's half-difference rule. |

Phase 1 only commits to the contract; no source actually evaluated yet
except `source_00_nominal` which uses `propagation_type = "analytic_constant"`
with `delta = 0` everywhere by construction.

---

## 6. `correlation_class` definitions and combination rules

The combiner uses `correlation_class` to decide how to add the source
to the running total per observable. From the plan, Section
"Combination and reporting":

| Value | Combination rule for `M`, `sigma` | Effect on `A_h` |
|---|---|---|
| `uncorrelated` | quadrature-add the per-bin absolute deltas with other `uncorrelated` sources | combine in quadrature per-bin in `A_h` |
| `correlated_across_bins` | linear-add (signed) the per-bin deltas with other sources of the same `correlation_class` (a separate tally), then quadrature-combine that tally with the `uncorrelated` tally | If `cancels_in_A_h == true`, contributes 0 to `A_h`; else, linear-add into `A_h` correlated tally |
| `anti_correlated` | linear-subtract (sign-flipped) the per-bin deltas with the `correlated_across_bins` tally for the same observable | Same; with `cancels_in_A_h == true` the cancellation goes the other way and the source contributes 0 to `A_h` |

Concrete cancellation rules:

- **Luminosity** → `correlation_class = "correlated_across_bins"`,
  `cancels_in_A_h = true`, `observable_scope.M = false`,
  `observable_scope.sigma = true`. Affects only `sigma`.
- **Common tracking efficiency floor** → `correlated_across_bins`,
  `cancels_in_A_h = true`. Cancels in `A_h` because both charges
  are shifted by the same fractional amount.
- **Charge-dependent tracking residual** → `correlated_across_bins`,
  `cancels_in_A_h = false`. Does NOT cancel in `A_h` because the
  two charges are shifted differently.
- **HFS scale** → `correlated_across_bins`, `cancels_in_A_h = true`
  (kinematic shift acts equally on both charges).
- **MC model dependence (RAPGAP vs DJANGOH)** → `correlated_across_bins`,
  `cancels_in_A_h = false` (separate per-charge acceptance differences
  do not cancel exactly).
- **NuclIA clip variation** → `uncorrelated` across bins,
  `cancels_in_A_h = false` (per-track weight, charge-symmetric in
  expectation but not exact bin-by-bin).

The combiner produces SEPARATE tallies for each `correlation_class`,
then folds them at the end:

```
delta_total^2 = delta_uncorr^2 + delta_corr^2
delta_corr    = | sum_i (sign_i * delta_corr_i) |
delta_uncorr  = sqrt( sum_j delta_uncorr_j^2 )
```

This preserves correlated-source covariance structure without forcing
quadrature combination across the correlated block.

---

## 7. `status` semantics

| `status` | Combiner behavior |
|---|---|
| `ok` | Source enters totals normally. |
| `blocked` | Source is preserved in master JSON but its deltas are NOT added to totals. The master JSON's `blocked_sources` list is populated. |
| `deferred_to_unfolding` | Source is preserved; per-bin entries that fall in low-purity bins (per `bbb_valid_4D == false`) are masked out of totals for those bins, with bin-level `status_per_bin` = `deferred_to_unfolding`. |
| `placeholder_not_for_total` | Source is preserved with zeros; explicitly excluded from totals; flagged in the master JSON's `placeholders` list. |

---

## 8. Validation rules (combiner enforces ALL)

Before a per-source JSON enters combination, the combiner validates:

1. All top-level required keys present (Section 3).
2. `binning == "HQ-v5g-Bprime"`, `production == "v5g_4period"`,
   `extractor == "extract_pertrack_v5g_fixedFrame_v3.py"`,
   `schema_version == "systematics_v3.0"`.
3. `cells` contains exactly the 8 cell ids.
4. Every per-cell, per-charge block has `bbb_valid_4D` of shape `[6,5]`.
5. Every populated `delta_*_4D` has shape `[6,5]`; off-shape arrays
   are a hard fail.
6. Every `delta_*` value is a finite float or `null` — `NaN`/`Inf` are
   a hard fail (silent NaNs are explicitly forbidden by the plan's
   acceptance criterion).
7. `propagation_type` and `correlation_class` are in the allowed sets.
8. `cancels_in_A_h` is a boolean.
9. If `observable_scope.phi_harmonics == true`, the `phi_harmonics`
   block exists per cell.

A failing source is reported by name and excluded from combination
(with status auto-set to `blocked` in the master JSON), and the
combiner exits non-zero unless `--allow-invalid-sources` is given.

---

## 9. Concrete worked examples

### 9.1 `analytic_constant` — Luminosity ±2.3%

```json
{
  "source_id": "luminosity",
  "source_name": "Luminosity normalization (±2.3%)",
  "production": "v5g_4period",
  "extractor": "extract_pertrack_v5g_fixedFrame_v3.py",
  "binning": "HQ-v5g-Bprime",
  "schema_version": "systematics_v3.0",
  "created": "2026-05-24T00:00:00Z",
  "nominal_inputs": { "xsec": "H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json" },
  "variation_description": "Flat ±2.3% normalization on absolute cross section; cancels exactly in multiplicity (ratio) and in charge asymmetry.",
  "propagation_type": "analytic_constant",
  "correlation_class": "correlated_across_bins",
  "cancels_in_A_h": true,
  "observable_scope": { "M": false, "sigma": true, "A_h": false, "phi_harmonics": false },
  "axes": { "...": "..." },
  "status": "ok",
  "cells": {
    "S0a": {
      "Q2_range": [150.0, 300.0],
      "x_range":  [0.0021, 0.0071],
      "by_charge": {
        "h+": {
          "sigma": {
            "delta_central_4D":  [[1.278, 1.024, 0.456, 0.198, 0.043], "... 5 more z rows ..."],
            "delta_integrated":  0.523,
            "bbb_valid_4D":      [[true, true, true, true, false], "..."]
          },
          "M": { "delta_central_4D": null, "delta_integrated": null, "bbb_valid_4D": "..." }
        },
        "h-": { "...": "..." }
      },
      "asymmetry": {
        "A_h": { "delta_central_4D": null, "delta_integrated": null, "bbb_valid_4D": "..." }
      }
    },
    "...": "S0b ... S3b"
  }
}
```
Each `delta_central_4D` here = `0.023 * |sigma_nom[z][pTh]|`.

### 9.2 `local_reweight` — NuclIA clip threshold

```json
{
  "source_id": "nuclia_clip",
  "source_name": "NuclIA weight clipping threshold variation (10 → 5, 20)",
  "production": "v5g_4period",
  "extractor": "extract_pertrack_v5g_fixedFrame_v3.py",
  "binning": "HQ-v5g-Bprime",
  "schema_version": "systematics_v3.0",
  "created": "2026-05-24T00:00:00Z",
  "nominal_inputs": {
    "xsec": "H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json",
    "per_track_data": "H1/sidis_highQ2/data/pertrack_v5g_fixedFrame_v3_merged.csv.gz"
  },
  "variation_description": "Recompute per-charge yields after clipping NuclIA weights to 5 and 20 instead of nominal 10. Asymmetric envelope from |max(up,down)|.",
  "propagation_type": "local_reweight",
  "correlation_class": "uncorrelated",
  "cancels_in_A_h": false,
  "observable_scope": { "M": true, "sigma": true, "A_h": true, "phi_harmonics": false },
  "axes": { "...": "..." },
  "status": "ok",
  "cells": {
    "S0a": {
      "by_charge": {
        "h+": {
          "M": {
            "delta_central_4D":  [[0.32, 0.41, 0.18, 0.05, 0.01], "..."],
            "delta_up_4D":       [[0.45, 0.50, 0.21, 0.06, 0.01], "..."],
            "delta_down_4D":     [[0.32, 0.41, 0.18, 0.05, 0.01], "..."],
            "delta_integrated":  0.18,
            "bbb_valid_4D":      "..."
          },
          "sigma": { "...": "..." }
        },
        "h-": { "...": "..." }
      },
      "asymmetry": {
        "A_h": {
          "delta_central_4D": [[0.0012, 0.0009, 0.0004, 0.0001, 0.0001], "..."],
          "delta_integrated": 0.0007,
          "bbb_valid_4D": "..."
        }
      }
    },
    "...": "..."
  }
}
```

### 9.3 `shifted_extractor` — HFS scale ±2%

```json
{
  "source_id": "hfs_scale",
  "source_name": "HFS energy scale ±2% (shifted extractor)",
  "production": "v5g_4period",
  "extractor": "extract_pertrack_v5g_fixedFrame_v3.py",
  "binning": "HQ-v5g-Bprime",
  "schema_version": "systematics_v3.0",
  "created": "2026-05-26T00:00:00Z",
  "nominal_inputs": {
    "xsec": "H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json",
    "shifted_up_csv":   "DUST:.../pertrack_v5g_fixedFrame_v3_hfs_up_merged.csv.gz",
    "shifted_down_csv": "DUST:.../pertrack_v5g_fixedFrame_v3_hfs_down_merged.csv.gz"
  },
  "variation_description": "Re-run extractor v3 with HFS four-momenta scaled by ±2% before recomputing DIS kinematics and per-bin observables. Migration in Q², x, z, pT_h is fully captured.",
  "propagation_type": "shifted_extractor",
  "correlation_class": "correlated_across_bins",
  "cancels_in_A_h": true,
  "observable_scope": { "M": true, "sigma": true, "A_h": true, "phi_harmonics": true },
  "axes": { "...": "..." },
  "status": "ok",
  "cells": {
    "S0a": {
      "by_charge": {
        "h+": {
          "M": {
            "delta_central_4D":  "max(|up|,|down|) per bin",
            "delta_up_4D":       "(M_up - M_nom) per bin",
            "delta_down_4D":     "(M_down - M_nom) per bin",
            "delta_integrated":  "...",
            "bbb_valid_4D":      "..."
          },
          "sigma": { "...": "..." }
        },
        "h-": { "...": "..." }
      },
      "asymmetry": {
        "A_h": {
          "delta_central_4D": "max(|A_h_up - A_h_nom|, |A_h_down - A_h_nom|)",
          "delta_up_4D": "...",
          "delta_down_4D": "...",
          "bbb_valid_4D": "..."
        }
      },
      "phi_harmonics": {
        "h+": { "A_UU_cosphi":  { "delta_central": 0.004, "delta_up": 0.004, "delta_down": -0.003 } },
        "h-": { "A_UU_cosphi":  { "delta_central": 0.004, "delta_up": 0.004, "delta_down": -0.003 } }
      }
    },
    "...": "..."
  }
}
```

### 9.4 `alt_mc` — DJANGOH14 vs RAPGAP31

```json
{
  "source_id": "mc_model",
  "source_name": "MC model dependence (DJANGOH14 vs RAPGAP31, half-difference)",
  "production": "v5g_4period",
  "extractor": "extract_pertrack_v5g_fixedFrame_v3.py",
  "binning": "HQ-v5g-Bprime",
  "schema_version": "systematics_v3.0",
  "created": "2026-05-28T00:00:00Z",
  "nominal_inputs": {
    "xsec": "H1/sidis_highQ2/data/xsec_v3_HQv5gBprime_2026-05-24.json",
    "alt_mc_reco":     "H1/sidis_highQ2/data/mc_v3/pertrack_mc_v3_reco_rapgap31_merged.csv.gz",
    "alt_mc_gen":      "H1/sidis_highQ2/data/mc_v3/pertrack_mc_v3_gen_rapgap31_merged.csv.gz",
    "alt_mc_disevent": "H1/sidis_highQ2/data/mc_v3/pertrack_mc_v3_disevent_rapgap31_merged.csv.gz"
  },
  "variation_description": "Recompute acceptance / cross section / multiplicity with RAPGAP31 in place of DJANGOH14 as the unfolding-step MC. Quoted shift = 0.5 * (X_RAPGAP - X_DJANGOH) per the plan rule.",
  "propagation_type": "alt_mc",
  "correlation_class": "correlated_across_bins",
  "cancels_in_A_h": false,
  "observable_scope": { "M": true, "sigma": true, "A_h": true, "phi_harmonics": true },
  "axes": { "...": "..." },
  "status": "ok",
  "cells": { "...": "..." }
}
```

---

## 10. Cross-reference to per-source tasks in the plan

Every per-source task in
`plan-systematics-v5g-v3-2026-05-24.md` (Sections "Per-source detailed
task list" entries 1–18) must, as its final acceptance criterion,
emit a JSON file at
`H1/sidis_highQ2/data/systematics_sources/source_<NN>_<short_id>.json`
that:

- conforms to this contract,
- passes the validation rules in Section 8,
- is consumable without modification by
  `H1/sidis_highQ2/scripts/systematics/combine_systematics.py`.

This requirement is inserted into the plan by reference; no per-source
JSON layout decision lives in any per-source dispatch document.

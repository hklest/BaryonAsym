# Beam-Energy Policy — H1 High-Q² Charged-Particle SIDIS

**Status:** Frozen 2026-05-22 by `h1-sidis-supervisor`.
**Authoritative:** This document governs all kinematic calculations in `sidis_highQ2/scripts/`.
**Companion utility:** `scripts/_beam_energy_utils.py` enforces the rules at runtime.

## Motivation

HERA-II beam energies varied run-to-run by O(0.1 GeV). In the current 3-period sample (em0405 + em06 + ep0607) the per-event √s spans **[318.1, 319.2] GeV** with mean **318.78 GeV**, std **0.14 GeV**, and discrete per-run spikes — not a single 319.0 GeV value. A hardcoded √s = 319 GeV systematically over-states s by ~0.07 % on average and washes out the per-run structure visible in the data.

For x_B reconstruction by the Sigma method (used as the analysis x in v5b/v5f trees) the dependence on s enters only indirectly through y_Σ; but for any closure check, gen-side x = Q² / (s·y) fallback, or cross-section normalisation the per-event s matters at the third-decimal level. For multiplicity ratios (which cancel s) the effect is below 1e-4. For φ_h moments (which depend on the lepton plane azimuth) the effect is also negligible. The cost of using per-event values is zero; the benefit is correctness, immunity to future MER inclusion, and traceability under beam-energy systematic shifts.

## Per-event distribution (3-period high-Q² data sample)

| Statistic | Value |
|-----------|-------|
| min √s | 318.130 GeV |
| max √s | 319.194 GeV |
| mean √s | 318.775 GeV |
| std √s | 0.141 GeV |
| n events | 183,634 |

Source: Lambda-analysis diagnostic histogram (`H1/lambda/...`); structurally identical to the high-Q² SIDIS sample. The discrete spikes correspond to distinct (E_e, E_p) HAT settings; the 319.10 spike is a small subset of runs with E_p slightly above 920 GeV.

## Policy

### Rule 1 — Always read per-event beam energies from the tree

| Code path | Branch (reco) | Branch (gen) |
|-----------|---------------|--------------|
| `ChargedTot` (matched MC + data) | `BeamElecE`, `BeamProtP` | n/a |
| `ChargedTotGen` (gen-only MC) | n/a | `GenBeamElecE`, `GenBeamProtP` |
| Older builders (pre-v5b) lacking gen branches | n/a | fallback allowed (see Rule 3) |

The v5b/v5f builder writes `GenBeamElecE = BeamElecE` and `GenBeamProtP = BeamProtP` per event (see `source_cache/ProtonTreeBuilderHighQ2.C` lines 931-932). Reco and gen sides are therefore numerically identical per event; using the gen branch on the gen side is the appropriate choice for response-matrix consistency.

### Rule 2 — Nominal values are fallbacks only

The nominal constants
```
E_BEAM_E_NOMINAL = 27.6   # GeV
P_BEAM_P_NOMINAL = 920.0  # GeV
```
defined in `_beam_energy_utils.py` may be used **only** as a per-element fallback for entries where the tree value is ≤ 0 or non-finite. They must **never** be used as a global scalar in a kinematic formula. Forbidden patterns:

```python
# FORBIDDEN
E_e = np.full(n, 27.6)
s_cms = 4.0 * 27.6 * 920.0
sqrt_s = 319.0
```

Allowed patterns:

```python
# ALLOWED
from _beam_energy_utils import resolve_reco_beams, resolve_gen_beams, s_cms_per_event
E_e, P_p = resolve_reco_beams(batch)     # data / matched MC
E_e, P_p = resolve_gen_beams(batch)      # gen-only MC
s        = s_cms_per_event(E_e, P_p)     # per-event Mandelstam s
```

### Rule 3 — Branch availability

- **Reco side (`BeamElecE`, `BeamProtP`):** required. All v5b+ trees carry them. `resolve_reco_beams(batch, require_tree=True)` (default) raises `RuntimeError` if absent.
- **Gen side (`GenBeamElecE`, `GenBeamProtP`):** required for v5b/v5f trees. `resolve_gen_beams(batch, require_tree=False)` (default) falls back to nominals if the branch is absent (older MC). Callers processing v5b/v5f-only data should pass `require_tree=True`.

### Rule 4 — Sanity envelope

Per-event beam energies are asserted to lie within the HERA-II operating envelope:

```
E_e ∈ [27.3, 27.8] GeV     # covers 27.5 (ep0304-low) and 27.6 (rest)
P_p ∈ [918.0, 922.0] GeV   # covers HAT-measured spread around 920
```

Any per-event value outside these bounds raises `RuntimeError`. This catches tree corruption, wrong-unit ingestion, and accidental zero-fills that slipped past the per-element fallback.

### Rule 5 — Inclusion of ep0304 (future)

When ep0304 e- recovery completes (currently blocked; see plan-finalization-r1 Phase 4), the sample will include some runs at E_e = 27.5 GeV. The per-event policy above handles this transparently. Any script that still uses `np.full(n, 27.6)` on the gen side will give wrong kinematics for those events — the verification scanner below catches this.

### Rule 6 — Beam-energy systematic variations (Pass-B iSys)

Beam-energy ±10 MeV systematic shifts (`systematics_v5f_beam_energy_v2.json`) are reproduced by re-running the MC builder with shifted `BeamElecE`/`BeamProtP` writes; the gen-side shift uses the same per-event shift. Scripts using the per-event branches automatically pick up the shift. Scripts using hardcoded 27.6/920 do NOT track the shift and will produce inconsistent ±shift propagation — another reason to enforce Rule 1.

## Status of scripts (audit 2026-05-22)

### CURRENT pipeline — patched 2026-05-22

| Script | Reco side | Gen side |
|--------|-----------|----------|
| `extract_phi_moments_data.py` | helper | helper |
| `unfold_phih.py` | (response uses gen beams for consistency) | helper |
| `compute_crad_charged.py` | n/a (gen only) | helper |
| `extract_multiplicities.py` | helper | helper |
| `extract_yields_v5f_3period.py` | per-event with fallback | n/a (data only) |
| `extract_phi_yields_v5f_3period.py` | per-event with fallback | per-event with fallback |
| `acceptance_v5f_charged.py` | per-event with fallback | per-event with fallback |
| `perbin_LAr_HFS_beam_shifts.py` | per-event with fallback | per-event with fallback |
| `systematics_v5f_nuclia_proton_omission.py` | n/a | per-event with fallback |
| `task_W_multiplicity_highQ2.py` | per-event with fallback | per-event with fallback |
| `migration_matrices_v5f.py` | per-event with fallback | per-event with fallback |
| `binning_v5f_compute.py` | per-event with fallback | per-event with fallback |

### Deprecated / not actively run

`tmd_analysis.py`, `tmd_resolution_acceptance.py`, `plot_in_vs_out_phase_space.py`, `plot_phi_acceptance.py`, `phi_gap_diag_extract.py`, `_unfold_compare_v5f_extract.py`, `study_migration_background.py`, `submit_norad_charged_sidis_highQ2.py`, `compute_highQ2_crad_charged.{C,py}` (.C is upstream NAF code). These carry the older `np.full(n, 27.6)` pattern but are not consumed by the active pipeline; they will be addressed when the `scripts/` staleness sweep is done.

## Verification

Run `python3 scripts/verify_beam_energy_usage.py` to scan all `scripts/*.py` for offending patterns. The scanner reports any:

1. Use of `np.full(..., E_BEAM_E_DEFAULT)` or `np.full(..., 27.6)` (forbidden global scalar)
2. Use of `4 * E_BEAM_E_DEFAULT * P_BEAM_P_DEFAULT` or `4 * 27.6 * 920` (forbidden global s)
3. Direct assignment `sqrt_s = 319.0` or `s_cms = 101568` (forbidden hardcoded s)
4. Missing `GenBeamElecE`/`GenBeamProtP` from a script that uses gen-side kinematics

CI / pre-commit hook: TBD.

## Numerical impact summary

For the current 3-period sample (no ep0304), all events have E_e=27.6, E_p=920 to within HAT precision (10 MeV). The per-event vs hardcoded difference is therefore at the 1e-4 fractional level for s — negligible for multiplicity ratios and φ_h moments, marginal for absolute cross-sections (below the lumi 2.3 % systematic), but **important** for:

- Beam-energy systematic propagation (Rule 6).
- Future ep0304 inclusion (Rule 5) where some events have E_e=27.5: a 0.4 % shift in s.
- Closure tests where gen-level x_calc = Q² / (s·y) is used as a fallback (`compute_crad_charged.py`).
- Any per-bin study where Q²/x boundary edges interact with the per-run s spikes.

The policy makes scripts immune to these failure modes at zero runtime cost.

## References

- `scripts/_beam_energy_utils.py` — runtime enforcement
- `scripts/verify_beam_energy_usage.py` — static-pattern scanner
- `source_cache/ProtonTreeBuilderHighQ2.C` lines 339, 572-573, 931-932 — builder writes gen-side beam-energy branches
- `docs/data/highQ2_per_run_beam_energy.{json,csv}` — per-run beam-energy table from HAT
- `docs/references/reference-highQ2-per-run-beam-energy.md` — reference doc on per-run table provenance
- `docs/data/systematics_v5f_beam_energy_v2.json` — beam-energy ±10 MeV systematic

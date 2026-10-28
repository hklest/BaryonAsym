# Implementation Plan: Bin-by-bin Systematic Uncertainty Evaluation for High-Q² Charged SIDIS on Canonical v5g v3

**Date:** 2026-05-24  
**Scope:** `H1/sidis_highQ2/` canonical 4-period, Scheme B′ (`HQ-v5g-Bprime`), extractor `extract_pertrack_v5g_fixedFrame_v3.py`  
**Plan status:** Draft for approval before any implementation dispatches  
**Primary execution owner:** `h1-systematics`  
**Supporting specialists:** `h1-acceptance`, `h1-radiative-correction`, `h1-unfolder`, `h1-mc-expert`, `h1-data-validator`, `h1-production`, `code-editor`  
**Research / reference inputs used:**
- `H1/CONTEXT.md`
- `H1/sidis_highQ2/context-handoff.md`
- `H1/sidis_highQ2/docs/results/systematics_v5f_hfs_scale_v2.md`
- `H1/sidis_highQ2/scripts/compute_systematics_v5f_track_eff_chargedep.py`
- `H1/sidis_highQ2/scripts/systematics_v5f_nuclia_proton_omission.py`
- `H1/sidis_highQ2/docs/research/research-tracking-systematics-deep.md`
- `H1/sidis_highQ2/docs/research/research-crad-for-charged-sidis.md`
- `H1/sidis_highQ2/docs/research/research-background-sources-migration.md`
- `H1/sidis_highQ2/docs/research/research-4d-unfolding.md`
- `H1/sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md`
- `H1/sidis_highQ2/docs/methodology/binning_adoption_HQ-v5g-Bprime_2026-05-24.md`
- `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md`
- `H1/sidis_highQ2/docs/references/reference-highQ2-crad-availability.md`
- `H1/sidis_highQ2/docs/references/reference-trigger-semantics-highQ2-sidis.md`
- `H1/sidis_highQ2/docs/references/reference-nuclia-correction-highQ2-sidis.md`
- `H1/sidis_highQ2/docs/references/reference-highQ2-charged-sidis-analysis-cuts.md`
- `H1/sidis_highQ2/docs/references/reference-LAr-electron-scale-highQ2-sidis.md`

## Executive summary

This plan defines the full implementation sequence for **bin-by-bin systematic uncertainty propagation across the canonical 4D high-Q² charged-particle SIDIS grid** on the current **v5g 4-period production** with the **v3 CM-frame extractor** and **Scheme B′ binning**. The target outputs are per-source bin-by-bin uncertainty maps for **multiplicities `M^{h±}`**, **charge asymmetry `A_h`**, **absolute cross sections `σ`**, and **φ_h harmonics**, followed by a single combined machine-readable uncertainty product and final tables carrying both statistical and systematic uncertainties.

The work is expected to span roughly **10–14 calendar days** once execution starts, with about **12 Condor sweeps** for sources that require re-extraction or shifted-MC propagation, plus a larger number of local fast reweight / aggregation passes that should complete in minutes per variation. The plan is intentionally granular: every task below is sized for a single specialist dispatch of at most one working day, and all 18 requested systematic sources are covered explicitly.

## Scope and exclusions

### Covered by this plan

- Bin-by-bin systematic propagation on the **full 4D analysis grid** for the canonical `HQ-v5g-Bprime` production.
- Per-source treatment for all 18 systematic sources named in the request.
- Propagation to all four observable families:
  - `M^{h+}` and `M^{h-}`
  - `A_h`
  - `σ^{h+}` and `σ^{h-}`
  - `φ_h` harmonic observables derived from the 16-bin φ_h axis
- Consistent output schema for downstream combination.
- Final combined statistical + systematic uncertainties.
- Explicit coordination boundary with unfolding and C_rad workstreams.

### Explicitly not owned by this plan

- **Unfolding implementation for the 59 bins with purity < 0.5.** This remains a separate planning / execution workstream owned by `h1-unfolder` and supporting specialists. This plan will consume unfolding outputs once available, but does not decide the unfolding method or execute that work.
- Absolute MC normalization policy beyond what is already frozen in the handoff and references.
- Re-definition of nominal analysis cuts, nominal binning, or nominal frame conventions.
- Broader publication packaging / HEPData release formatting.

### Operational exclusion rule for low-purity bins

For the first bin-by-bin systematics pass, this plan proceeds on the **purity ≥ 0.5 bins** as the nominal bin-by-bin-corrected domain and records the **59 low-purity bins** as **"requires unfolding handoff"**. Systematic products must preserve those bins in the output schema with an explicit status flag rather than silently dropping them.

## Requirements Summary

### Confirmed requirements

- Use the **canonical v5g v3 production state** as of 2026-05-24.
- Use **Scheme B′ / `HQ-v5g-Bprime`**: 8 `(Q²,x)` cells × 6 `z` × 5 `pT_h` × 16 `φ_h` × 2 charges.
- Use **extractor v3** as the canonical per-track CSV basis for any reweighting-based systematic.
- Use **Condor** for any extractor or builder re-run; do not propose multi-file login-node work.
- Produce a **single markdown implementation plan file** only; do not implement.
- Include a **granular numbered todo list** with priority, dependencies, and acceptance criteria per task.
- Cover **all 18 listed systematic sources**.
- Propose a **single output JSON convention** that supports downstream combination.

### Inferred assumptions used in this plan

- The current nominal central observable products exist and are usable as fixed reference inputs:
  - `xsec_v3_HQv5gBprime_2026-05-24.json`
  - `crad_v3_HQv5gBprime_2026-05-24.json`
  - purity products and the currently rendered PDF summaries
- RAPGAP31 is the only currently approved alternative MC model for the acceptance / model-dependence systematic.
- For high-Q² SIDIS, the electron-side systematic must be implemented as a **new local SIDIS procedure**, because the upstream `proton_HiQ` / `lambda_HiQ` workspaces do **not** provide a finished reusable LAr electron-scale recipe.
- C_rad nominal numbers already exist for v3, but **C_rad model-uncertainty propagation** may still require additional dedicated derivations or placeholders if an alternative high-Q² C_rad chain is not yet ready.
- The final uncertainty product must support bins that are nominal, bins deferred to unfolding, and bins blocked by missing upstream variations without schema breakage.

### Acceptance criteria for planning completeness

- All 18 systematic sources are mapped to a propagation method.
- Every source lists inputs, outputs, dependencies, and acceptance checks.
- Condor-required versus local-only sources are clearly separated.
- The plan defines one canonical combined output JSON schema.
- The plan keeps unfolding as a separate dependency and does not absorb that decision.

## Open questions

These do not block writing the plan, but they should be resolved by supervisor or specialist confirmation before execution of the affected tasks:

1. **LAr electron systematics amplitudes:** the local upstream analyses do not provide an authoritative reusable LAr energy / angle shift procedure. This plan assumes the requested working amplitudes from the task brief: **±0.5% (LAr)** and **±2 mrad**, and treats any SpaCal-specific number as out of scope unless a supervisor explicitly asks for a mixed-detector branch.
2. **PDF uncertainty machinery:** no verified current high-Q² SIDIS PDF-eigenvector pipeline was found locally. This plan assumes PDF uncertainty will be implemented via MC reweight vectors if available; otherwise it becomes a controlled placeholder / blocked source with explicit status.
3. **C_rad model variation source:** this plan assumes DJANGOH14 RAD/NORAD nominal plus an alternative model path (HECTOR if available, otherwise generator-spread proxy) can be defined by `h1-radiative-correction`. If not, the source remains schema-complete but blocked pending specialist input.
4. **Background subtraction source implementation:** this plan assumes a diffractive-excluded or diffractive-varied MC branch is either available or can be created as a focused MC variation. If neither exists, the source will be recorded as blocked with a prescribed fallback study.
5. **Beam-energy variation execution path:** this plan assumes the per-event `BeamElecE` / `BeamProtP` branches in v3 are sufficient for a lightweight propagated variation and do not require a builder re-run.
6. **ep0304 systematic prescription:** this plan adopts the requested rule: **half the difference between 4-period and 3-period** as the systematic. If supervisor wants it reclassified as cross-check-only rather than quoted source, that decision must be made before combination.

## Inputs and dependencies

| Input / dependency | Role in systematics program | Status on 2026-05-24 | Notes |
|---|---|---|---|
| `extract_pertrack_v5g_fixedFrame_v3.py` and merged v3 CSV | Canonical per-track basis for local reweight systematics | Exists | Mandatory for all local weight / cut-variation branches that do not require full reprocessing |
| `HQ-v5g-Bprime` binning definition | Shared 4D/φ_h bin contract | Exists | Frozen in binning adoption memo |
| Nominal 4D cross sections JSON/PDF | Central `σ` reference | Exists | Used as nominal baseline for per-source deltas |
| Nominal 4D multiplicity products | Central `M^{h±}` reference | Exists/inferred | Mentioned in background; assumed available alongside current plots |
| Nominal `A_h` products | Central asymmetry reference | Exists/inferred | Needed before source-level propagation |
| Nominal φ_h harmonic products | Central harmonic reference | Exists | `v5g_phih_harmonics.pdf` already produced |
| 4D purity map / low-purity bin mask | Gate bin-by-bin vs unfolding domain | Exists | 59/480 bins below purity 0.5 |
| `crad_v3_HQv5gBprime_2026-05-24.json` | Nominal C_rad input | Exists | Central values ready |
| DJANGOH14 RAD MC | Nominal acceptance / response MC | Exists | Current nominal MC |
| DJANGOH14 NORAD MC | C_rad denominator / radiative studies | Exists conceptually | Availability documented; exact production path owned by `h1-radiative-correction` |
| RAPGAP31 MC | Alternative model systematic | Exists | Use as acceptance/C_rad alternative where valid |
| Trigger semantics reference | Trigger treatment policy | Exists | Confirms data-only S67 cut, no MC trigger cut |
| NuclIA reference | NuclIA clipping and proton-handling behavior | Exists | Includes known clipping anomaly and stored-only semantics |
| Existing v5f HFS systematic memo | Prior method and validation baseline | Exists | Must be superseded numerically on v3 |
| Unfolding plan / outputs | Needed only for low-purity bins and final full-grid release | In progress / separate | This plan references but does not own it |
| Script infrastructure for JSON aggregation | Needed for single combined output schema | Needs to be generated/modified during execution | Owned by `code-editor` + `h1-systematics` dispatches |

## Output convention (single canonical JSON schema)

All source-level and final combination products should be compatible with one master schema:

**Primary combined output target:**
- `H1/sidis_highQ2/data/systematics_v3_HQv5gBprime_2026-05-24.json`

**Per-source auxiliary outputs:**
- `H1/sidis_highQ2/data/systematics_sources/<source_id>_v3_HQv5gBprime.json`
- `H1/sidis_highQ2/plots/systematics_v3_HQv5gBprime/<source_id>_*.pdf`

### Required top-level JSON structure

```json
{
  "analysis": "sidis_highQ2",
  "production_tag": "v5g",
  "extractor_tag": "fixedFrame_v3",
  "binning_tag": "HQ-v5g-Bprime",
  "sample_policy": "4-period_ep0304_included",
  "nominal_inputs": {
    "multiplicity": "data/multiplicity_v3_HQv5gBprime_2026-05-24.json",
    "asymmetry": "data/asymmetry_v3_HQv5gBprime_2026-05-24.json",
    "cross_section": "data/xsec_v3_HQv5gBprime_2026-05-24.json",
    "crad": "data/crad_v3_HQv5gBprime_2026-05-24.json",
    "purity": "data/purity_v3_HQv5gBprime_2026-05-24.json"
  },
  "axes": {
    "Q2x": 8,
    "z": 6,
    "pTh": 5,
    "phi_h": 16,
    "charge": 2
  },
  "bin_status": {
    "bbb_domain": "purity>=0.5",
    "unfolding_required_bins": ["S2b_z5_p4", "..."]
  },
  "sources": {
    "tracking_eff_common": {
      "type": "correlated_scale",
      "observable_shifts": {
        "M_hplus": [[[[0.0]]]],
        "M_hminus": [[[[0.0]]]],
        "A_h": [[[[0.0]]]],
        "sigma_hplus": [[[[0.0]]]],
        "sigma_hminus": [[[[0.0]]]],
        "phi_harmonics": []
      },
      "status": "ok"
    }
  },
  "totals": {
    "stat": {},
    "syst": {},
    "stat_plus_syst": {}
  }
}
```

### Required metadata per source

Every source block must carry:
- `source_id`
- `label`
- `variation_type` (`condor_rerun`, `shifted_mc`, `per_event_weight`, `per_track_weight`, `analytic`, `dataset_split`)
- `observable_scope`
- `correlation_class` (`global`, `charge-partial`, `bin-migration`, `local`, `normalization-only`)
- `nominal_input_paths`
- `variation_input_paths`
- `status` (`ok`, `blocked`, `deferred_to_unfolding`, `placeholder_not_for_total`)
- `combination_rule` (`quadrature`, `linear`, `covariance`, `normalization_only`)

## Master systematics table

| Source | Propagation type | Expected scale | Priority | Responsible specialist | Plan section |
|---|---|---|---|---|---|
| 1. Tracking efficiency (common + charge-dependent) | Per-track weight / analytic closure | Medium | P0 | `h1-systematics` + `h1-data-validator` | § Per-source 1 |
| 2. HFS energy scale ±2% | Shifted-MC / Condor re-run | Medium-large | P0 | `h1-systematics` + `h1-production` | § Per-source 2 |
| 3. Electron energy scale | Shifted-MC / Condor re-run | Medium | P0 | `h1-systematics` + `h1-data-validator` | § Per-source 3 |
| 4. Electron angle ±2 mrad | Shifted-MC / Condor re-run | Medium | P0 | `h1-systematics` + `h1-data-validator` | § Per-source 4 |
| 5. Trigger efficiency | Per-event weight / data-side comparison | Small | P1 | `h1-systematics` + `h1-data-validator` | § Per-source 5 |
| 6. Luminosity ±2.3% | Analytic normalization | Medium | P1 | `h1-systematics` | § Per-source 6 |
| 7. MC model dependence (DJANGOH vs RAPGAP) | Alternate MC full propagation | Large | P0 | `h1-systematics` + `h1-acceptance` | § Per-source 7 |
| 8. PDF set uncertainty | Reweight if available; else blocked placeholder | Small-medium | P2 | `h1-systematics` + `h1-mc-expert` | § Per-source 8 |
| 9. Radiative correction model uncertainty | Alternate C_rad derivation | Medium | P0 | `h1-radiative-correction` + `h1-systematics` | § Per-source 9 |
| 10. Background subtraction (diffractive / non-DIS) | Alternate MC branch / subtraction study | Medium-large in corners | P1 | `h1-mc-expert` + `h1-systematics` | § Per-source 10 |
| 11. Beam-energy systematic | Per-event propagated variation | Small | P2 | `h1-systematics` | § Per-source 11 |
| 12. Vertex-z cut variation | Re-extraction / Condor sweep | Small-medium | P1 | `h1-systematics` + `h1-production` | § Per-source 12 |
| 13. y-window variation | Re-extraction / Condor sweep | Medium | P1 | `h1-systematics` + `h1-production` | § Per-source 13 |
| 14. Empz cut variation | Re-extraction / Condor sweep | Medium | P1 | `h1-systematics` + `h1-production` | § Per-source 14 |
| 15. Track DCA / quality cut variation | Local shifted CSV or re-extraction depending implementation | Medium | P0 | `h1-systematics` + `code-editor` | § Per-source 15 |
| 16. NucliaWeight clipping | Per-track weight variation | Small-medium | P1 | `h1-systematics` | § Per-source 16 |
| 17. ep0304 inclusion variation | Dataset split | Medium | P1 | `h1-systematics` + `h1-data-validator` | § Per-source 17 |
| 18. Frame definition systematic | Alternate extractor branch / local recompute | Medium-large for φ_h | P0 | `h1-systematics` + `code-editor` | § Per-source 18 |

## Phased implementation

### Phase 1 — Normalize the machinery and freeze the uncertainty data contract

Objective: ensure the nominal inputs, bin masks, and output schema are fixed before any source-specific propagation starts.

### Phase 2 — Fast local / analytic / weight-based systematics

Objective: complete all sources that can be evaluated from existing v3 per-track CSV / nominal JSON inputs without full reprocessing.

### Phase 3 — Migration-driven and detector-shift systematics requiring Condor

Objective: execute or prepare all variations that require shifted extractor / shifted event kinematics / cut-reprocessing.

### Phase 4 — Model, C_rad, and dataset-composition systematics

Objective: propagate alternative MC, radiative, and period-composition effects with the same JSON contract.

### Phase 5 — Combination, validation, and final reporting products

Objective: combine per-source shifts into final statistical + systematic outputs and mark the analysis ready for the next gate.

## Per-source detailed task list

### 1. Tracking efficiency (charge-dependent and inclusive)

**Approach and rationale:** Reuse the v5f tracking-systematics logic as structural precedent, but update it to the v3 nominal products, Scheme B′ geometry, and current cancellation policy. Split the source into a **common tracking efficiency component** that is correlated and cancels strongly in `A_h`, and a **charge-dependent residual** that does not cancel. The plan follows the research note conclusion that the central-track common piece is small and mostly global, while the charge-dependent residual requires an explicit per-bin treatment.

**Likely files touched during implementation:**
- `H1/sidis_highQ2/scripts/compute_systematics_v3_track_eff_chargedep.py` (new)
- `H1/sidis_highQ2/scripts/compute_systematics_v3_track_eff_common.py` (new)
- `H1/sidis_highQ2/data/systematics_sources/tracking_eff_*.json`

**Representative code pattern to implement later:**
```python
track_eff_common_rel = 0.02
shift_sigma_hp = track_eff_common_rel * np.abs(sig_hp)
shift_sigma_hm = track_eff_common_rel * np.abs(sig_hm)
shift_M_hp = track_eff_common_rel * np.abs(M_hp)
shift_M_hm = track_eff_common_rel * np.abs(M_hm)
A_h_var = (M_hp * (1.0 + d_hp) - M_hm * (1.0 + d_hm)) / (M_hp * (1.0 + d_hp) + M_hm * (1.0 + d_hm))
shift_A_h = np.abs(A_h_var - A_h_nom)
```

**Trade-offs:** A pure analytic 2%/track treatment is fast and traceable, but the charge-dependent residual needs a data- or MC-informed constraint to avoid double-counting with other detector asymmetry sources.

#### 1.1 [P0] Freeze nominal observable inputs for tracking systematics
**Description:** Build the exact list of nominal v3 products that the tracking-efficiency scripts will read, including multiplicities, asymmetries, cross sections, and φ_h harmonics. The execution agent should verify naming consistency and ensure the observable arrays align with Scheme B′ indexing before any propagation logic is written.

**Input artifacts:** nominal v3 JSONs, Scheme B′ memo, purity mask  
**Output artifacts:** input manifest block in combined schema; checked nominal-path list  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** none  
**Acceptance criteria:** all nominal input paths resolve; array dimensions agree with 8×6×5×16×2 contract.

#### 1.2 [P0] Port v5f charge-dependent tracking script structure to v3 / Scheme B′ planning spec
**Description:** Use `compute_systematics_v5f_track_eff_chargedep.py` as a template reference and define the v3 script contract, new input names, and output schema. This includes mapping from v5f observable names to v3 names and recording which logic remains valid versus which must be recalibrated.

**Input artifacts:** v5f tracking script, v3 nominal JSONs, tracking research note  
**Output artifacts:** implementation spec section for the new v3 tracking script; source JSON field list  
**Specialist:** `h1-systematics` + `code-editor`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 1.1  
**Acceptance criteria:** a complete variable map exists; no v5f-era path or bin-shape assumptions remain in the spec.

#### 1.3 [P0] Define common tracking-efficiency propagation rule for M, σ, A_h, and φ_h observables
**Description:** Convert the requested “typically 2% per track, plus charge-dependent piece” into a formal source definition. The common piece must be treated as correlated across bins and charges where appropriate, affect `M` and `σ` directly, and be propagated to `A_h` only through the non-cancelling residual.

**Input artifacts:** task brief, tracking research note, systematics reference matrix  
**Output artifacts:** source-definition block for `tracking_eff_common` in plan and future JSON schema  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** 1.2  
**Acceptance criteria:** source definition explicitly states cancellation class and observable-specific propagation.

#### 1.4 [P0] Define charge-dependent residual estimator and validation tests
**Description:** Specify the central estimator for the charge-dependent residual in the v3 era. The default in this plan is to carry forward the v5f logic pattern—flat floor plus MC-informed estimate—but force revalidation on Scheme B′ and current observable arrays. Validation must check for negative / pathological bins and quantify stat-limited bins.

**Input artifacts:** v5f tracking script, nominal `A_h`, acceptance products, tracking research note  
**Output artifacts:** accepted estimator definition and validation checklist  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 1.2, 1.3  
**Acceptance criteria:** estimator path for every populated bin is defined; stat-limited fallback rule is documented.

#### 1.5 [P1] Produce tracking common and charge-dependent per-source JSON outputs
**Description:** Run the future v3 scripts to emit one JSON for the common piece and one for the charge-dependent piece, each with shifts for `M`, `σ`, `A_h`, and φ_h observables. The source split is retained so later combination can keep a clean cancellation structure.

**Input artifacts:** outputs of 1.1–1.4; nominal v3 products  
**Output artifacts:** `tracking_eff_common_v3_HQv5gBprime.json`, `tracking_eff_chargedep_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 1.4  
**Acceptance criteria:** both JSONs validate against the master schema; all populated bins have finite shifts or explicit nulls with reasons.

### 2. HFS energy scale ±2%

**Approach and rationale:** The existing v5f memo is a useful methodological starting point but is explicitly marked as an approximation pending a proper shifted-MC reproduction. For v3, the plan elevates HFS to a **real shifted-kinematics source** because it migrates `Q²`, `x`, `z`, and `pT_h` simultaneously.

**Likely files touched during implementation:**
- `H1/sidis_highQ2/scripts/compute_systematics_v3_hfs_scale.py` (new or forked)
- Condor steering for shifted extraction
- `H1/sidis_highQ2/data/systematics_sources/hfs_scale_v3_HQv5gBprime.json`

**Representative code pattern to implement later:**
```python
for direction, frac in {"down": -0.02, "up": +0.02}.items():
    df_var["hfs_E"] = df_nom["hfs_E"] * (1.0 + frac)
    df_var = recompute_dis_kinematics(df_var)
    df_var = refill_4d_histograms(df_var, bins=BINS)
```

**Trade-offs:** A local analytic shift is cheaper, but the brief explicitly prefers migration-aware shifted-MC treatment. Therefore the plan uses Condor-backed shifted extraction as headline, with local closure checks only as diagnostics.

#### 2.1 [P0] Audit v5f HFS memo and identify reusable logic versus obsolete assumptions
**Description:** Read `systematics_v5f_hfs_scale_v2.md` as an audit baseline and document which parts can be reused structurally and which cannot because of v3 frame fixes, Scheme B′ binning, and the need for real migration propagation.

**Input artifacts:** v5f HFS memo, current v3 handoff  
**Output artifacts:** HFS reuse audit note inside implementation spec  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** none  
**Acceptance criteria:** v5f-only assumptions are explicitly listed and excluded from the v3 implementation path.

#### 2.2 [P0] Define shifted-HFS extraction path and Condor input/output contract
**Description:** Specify how the ±2% HFS shift is introduced, where in the event reconstruction or extraction chain it lives, what outputs are required, and which artifacts must be generated on Condor. This is the execution contract for the reprocessing campaign.

**Input artifacts:** current extractor chain, HFS memo, high-Q² cuts reference  
**Output artifacts:** shifted-HFS run contract with file paths and naming convention  
**Specialist:** `h1-systematics` + `h1-production` + `code-editor`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 2.1  
**Acceptance criteria:** exact shifted sample labels and expected outputs are frozen for both directions.

#### 2.3 [P0] Run HFS-up and HFS-down Condor sweeps on canonical v3 nominal sample
**Description:** Submit and monitor the two HFS systematic sweeps. These should regenerate the minimal required shifted products for downstream 4D observable extraction, not a redundant full analysis chain.

**Input artifacts:** 2.2 run contract, canonical nominal sample  
**Output artifacts:** shifted HFS-up / HFS-down products on DUST  
**Specialist:** `h1-production`  
**Estimated effort:** L (4–8 h active, multi-day wall clock)  
**Dependencies:** 2.2  
**Acceptance criteria:** both sweeps complete with expected file counts and zero structural file failures.

#### 2.4 [P0] Derive per-bin HFS shifts for all observables
**Description:** Compare nominal versus HFS-up/down outputs and compute per-bin symmetrized or max-envelope shifts for `M`, `σ`, `A_h`, and φ_h harmonics. The implementation must preserve directional diagnostics even if the final quoted uncertainty is symmetrized.

**Input artifacts:** nominal observables, HFS-up/down outputs  
**Output artifacts:** `hfs_scale_v3_HQv5gBprime.json`, diagnostic summary plots  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 2.3  
**Acceptance criteria:** output contains both directional deltas and final quoted shift; corner-bin behavior is physically plausible.

### 3. Electron energy scale

**Approach and rationale:** The local references show the upstream high-Q² analyses do not provide a completed reusable LAr procedure. This plan therefore treats the task brief amplitudes as working inputs and explicitly requires a SIDIS-local shifted-kinematics propagation via Condor.

**Likely files touched during implementation:**
- new shifted-electron-energy systematic script / steering
- `electron_scale_v3_HQv5gBprime.json`

**Representative code pattern to implement later:**
```python
scatEE_var = scatEE_nom * (1.0 + shift)
q_var = beam_electron - scattered_electron_var
Q2_var = -(q_var.mass2())
x_var = Q2_var / (2.0 * proton.dot(q_var))
```

**Trade-offs:** The systematic is conceptually electron-side and correlated, but because the binning is fine in `(Q²,x)`, real migration must be measured rather than assumed to cancel.

#### 3.1 [P0] Freeze working amplitude and detector applicability for electron-energy variation
**Description:** Record the source definition using the task brief amplitude and the current high-Q² detector policy. Because the upstream LAr reference is not reuse-ready, execution should proceed only after this working assumption is explicitly frozen.

**Input artifacts:** task brief, LAr reference note, high-Q² handoff  
**Output artifacts:** frozen amplitude / applicability note in source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** source definition states exact shift amplitude, detector scope, and that this is a SIDIS-local procedure.

#### 3.2 [P0] Define electron-energy shifted extraction contract
**Description:** Specify where the electron-energy shift is applied in the event reconstruction chain and which branches or derived quantities must be regenerated. This must include whether only downstream extraction changes or whether an upstream evls-level rerun is mandatory.

**Input artifacts:** LAr reference, current extractor methodology, cut reference  
**Output artifacts:** electron-energy variation run contract  
**Specialist:** `h1-systematics` + `h1-data-validator` + `code-editor`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 3.1  
**Acceptance criteria:** the contract is explicit enough to dispatch a Condor job without further planning.

#### 3.3 [P0] Execute electron-energy shifted Condor sweep(s)
**Description:** Run the required shifted production for the chosen direction set. The default plan assumes symmetric ± shifts so the response can be checked for linearity.

**Input artifacts:** 3.2 contract, nominal production  
**Output artifacts:** electron-energy-shifted analysis products  
**Specialist:** `h1-production`  
**Estimated effort:** L (4–8 h active, multi-day wall clock)  
**Dependencies:** 3.2  
**Acceptance criteria:** shifted outputs pass integrity checks and contain the expected event statistics within plausible migration loss.

#### 3.4 [P0] Extract per-bin electron-energy systematic shifts
**Description:** Compute per-bin observable differences relative to nominal and package them into the unified JSON schema. Because this source is highly correlated, the output must retain a direction-aware payload for later covariance construction.

**Input artifacts:** nominal and shifted observables  
**Output artifacts:** `electron_energy_scale_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 3.3  
**Acceptance criteria:** all observable families receive a shift payload; no silent omission of φ_h or `A_h` branches.

### 4. Electron angle / θ_e ±2 mrad

**Approach and rationale:** Treat analogously to the electron-energy source, but as a separate kinematic distortion because `Q²`, `y`, and φ_h-related geometry respond differently to angle shifts.

#### 4.1 [P0] Freeze working θ_e variation amplitude and application point
**Description:** Use the requested ±2 mrad as the working high-Q² angle variation and record the exact stage where this shift must be applied.

**Input artifacts:** task brief, LAr reference  
**Output artifacts:** angle-shift source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** exact angle-shift prescription is written and approved for execution.

#### 4.2 [P0] Define and dispatch θ_e shifted extraction sweep(s)
**Description:** Create the run contract and execute the required shifted extraction on Condor.

**Input artifacts:** current extraction chain, source definition  
**Output artifacts:** θ_e-up / θ_e-down shifted products  
**Specialist:** `h1-production` + `code-editor`  
**Estimated effort:** L (4–8 h active, multi-day wall clock)  
**Dependencies:** 4.1  
**Acceptance criteria:** both directions complete and are structurally valid.

#### 4.3 [P0] Derive observable-level θ_e uncertainty JSON
**Description:** Compare shifted products to nominal and package the resulting per-bin shifts.

**Input artifacts:** nominal and θ_e-shifted observables  
**Output artifacts:** `electron_theta_scale_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 4.2  
**Acceptance criteria:** output exists for all observables; directional asymmetry is preserved diagnostically.

### 5. Trigger efficiency (S67 / T67)

**Approach and rationale:** The trigger reference freezes the crucial rule: **data require S67, MC does not cut on S67 and is not weighted by `TrigWeightAC`**. Therefore the systematic is primarily a **data-side residual** from trigger weighting / alternate trigger-weight semantics, expected to be small.

#### 5.1 [P1] Freeze trigger systematic prescription for high-Q² SIDIS v5g
**Description:** Convert the trigger reference into a source definition for this systematics campaign. Record that luminosity-style normalization on MC is not part of this source and that the relevant comparison is on data-side weighting or alternative trigger-efficiency treatment.

**Input artifacts:** trigger reference, handoff  
**Output artifacts:** trigger source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** the source definition does not contradict the frozen trigger policy.

#### 5.2 [P1] Define practical trigger-variation handle for current v5g analysis products
**Description:** Specify the concrete comparison to use now—e.g. nominal trigger weighting versus alternative trigger-weight branch or residual flat efficiency scan—given the currently available high-Q² data products.

**Input artifacts:** trigger reference, nominal data extraction chain  
**Output artifacts:** executable trigger-variation contract  
**Specialist:** `h1-data-validator` + `h1-systematics`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** 5.1  
**Acceptance criteria:** one explicit variation path is chosen; if blocked, the blocking reason is schema-visible.

#### 5.3 [P1] Produce per-bin trigger-efficiency uncertainty payload
**Description:** Evaluate the chosen variation and emit a source JSON. The expectation is sub-percent, but the implementation must still inspect bin structure rather than assume a global constant.

**Input artifacts:** 5.2 contract, nominal products  
**Output artifacts:** `trigger_efficiency_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 5.2  
**Acceptance criteria:** source JSON exists with bin-by-bin values or explicit blocked status.

### 6. Luminosity ±2.3%

**Approach and rationale:** Treat as a pure normalization uncertainty on `σ` only, fully correlated across all bins and excluded from multiplicity and `A_h` totals.

#### 6.1 [P1] Encode luminosity as a normalization-only source in the master schema
**Description:** Add the luminosity source definition to the output convention with explicit observable scope and combination rule.

**Input artifacts:** task brief, systematics reference matrix  
**Output artifacts:** normalization-only source spec  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** schema explicitly prevents luminosity from entering `M` or `A_h` totals.

#### 6.2 [P1] Emit luminosity source JSON and σ-only covariance metadata
**Description:** Produce a simple source JSON containing the flat ±2.3% on `σ` bins and the metadata needed to add it linearly as a correlated normalization term.

**Input artifacts:** nominal σ products, source definition  
**Output artifacts:** `luminosity_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** 6.1  
**Acceptance criteria:** `σ` payload is complete; `M`, `A_h`, and φ_h branches are omitted or flagged `n/a` by design.

### 7. MC model dependence (DJANGOH14 vs RAPGAP31)

**Approach and rationale:** This is a headline source and likely one of the dominant contributions in high-z / high-pT_h bins. The requested rule is **half the difference between nominal and alternative** using RAPGAP31 as the alternative for acceptance and C_rad where available.

#### 7.1 [P0] Freeze nominal-versus-alternative model comparison contract
**Description:** Record DJANGOH14 RAD as nominal and RAPGAP31 as the alternative model branch for acceptance and response-related systematics, with half-difference as the quoted source.

**Input artifacts:** handoff, systematics reference, task brief  
**Output artifacts:** model-dependence source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** source definition states nominal, alternative, and 0.5×difference rule unambiguously.

#### 7.2 [P0] Recompute acceptance / response / central observables with RAPGAP31 alternative MC
**Description:** Execute the alternative-MC branch through the same acceptance and downstream extraction chain used for nominal observables.

**Input artifacts:** RAPGAP31 MC, nominal extraction scripts  
**Output artifacts:** RAPGAP31-based observable JSONs  
**Specialist:** `h1-acceptance` + `h1-systematics`  
**Estimated effort:** M (3–6 h)  
**Dependencies:** 7.1  
**Acceptance criteria:** full alternative observable set exists and matches nominal schema exactly.

#### 7.3 [P0] Derive model-dependence per-source JSON
**Description:** Compare DJANGOH-based and RAPGAP-based results and emit the half-difference source for all observables, with special attention to high-z and low-stat bins.

**Input artifacts:** nominal and RAPGAP-based observables  
**Output artifacts:** `mc_model_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 7.2  
**Acceptance criteria:** half-difference rule is implemented consistently; large-corner bins are not masked unless explicitly invalid.

### 8. PDF set uncertainty

**Approach and rationale:** Treat as a reweight-based source if a PDF eigenvector infrastructure exists; otherwise preserve it as a blocked but schema-complete source so the final combination can distinguish “not evaluated” from “zero”.

#### 8.1 [P2] Audit availability of PDF reweight vectors or eigenvector infrastructure
**Description:** Determine whether the current high-Q² MC chain already stores enough information to support PDF reweighting without regeneration.

**Input artifacts:** MC metadata, specialist references, nominal MC products  
**Output artifacts:** PDF-availability decision note  
**Specialist:** `h1-mc-expert` + `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** none  
**Acceptance criteria:** source is classified as executable or blocked with evidence.

#### 8.2 [P2] Define PDF uncertainty propagation path or blocked placeholder policy
**Description:** If executable, define eigenvector propagation and combination. If blocked, define the exact placeholder semantics so the source appears in output products without contaminating totals.

**Input artifacts:** 8.1 decision note, master schema  
**Output artifacts:** PDF source execution or placeholder contract  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** 8.1  
**Acceptance criteria:** no ambiguity remains about whether PDF uncertainty contributes to totals.

#### 8.3 [P2] Emit PDF source JSON
**Description:** Run the executable path or emit the blocked placeholder JSON.

**Input artifacts:** 8.2 contract  
**Output artifacts:** `pdf_set_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 8.2  
**Acceptance criteria:** source JSON validates against schema and records its status correctly.

### 9. Radiative correction model uncertainty (C_rad)

**Approach and rationale:** Nominal v3 C_rad exists, but the systematic requires either an alternative radiative model or a justified generator-spread proxy. The source matters particularly at high y / low Q² in principle, though the high-Q² range should moderate it.

#### 9.1 [P0] Freeze the nominal C_rad source and alternative-model hierarchy
**Description:** Define the source hierarchy: preferred alternative model if available, fallback to generator-spread proxy if not, and explicit blocked status if neither can be made defensible.

**Input artifacts:** nominal C_rad JSON, C_rad research note, high-Q² C_rad availability reference  
**Output artifacts:** C_rad model-uncertainty source definition  
**Specialist:** `h1-radiative-correction` + `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** none  
**Acceptance criteria:** one nominal and one alternative path are defined or blocking is documented.

#### 9.2 [P0] Produce alternative C_rad table(s) for the v3 binning
**Description:** Run the selected alternative C_rad derivation path and package the results on the same binning as the nominal products.

**Input artifacts:** nominal C_rad machinery, alternative model path  
**Output artifacts:** alternative `crad_*` JSON for v3  
**Specialist:** `h1-radiative-correction`  
**Estimated effort:** M–L (3–8 h)  
**Dependencies:** 9.1  
**Acceptance criteria:** alternative C_rad table covers the nominal binning and is numerically reasonable.

#### 9.3 [P0] Propagate C_rad model uncertainty into M, σ, and A_h
**Description:** Recompute observable-level shifts from the difference between nominal and alternative C_rad tables. This includes explicit per-charge propagation into `A_h`.

**Input artifacts:** nominal and alternative C_rad tables; nominal observables  
**Output artifacts:** `crad_model_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 9.2  
**Acceptance criteria:** source JSON contains per-charge propagated `A_h` shifts and does not misclassify the source as fully cancelling.

### 10. Background subtraction (diffractive, non-DIS)

**Approach and rationale:** Treat as a dedicated subtraction / alternate-MC source, especially important in high-z migration-sensitive regions. The plan uses a diffractive-excluded or diffractive-varied MC branch if available, otherwise a blocked-but-visible source.

#### 10.1 [P1] Inventory executable background-variation handles for high-Q² SIDIS
**Description:** Determine whether diffractive-excluded, diffractive-scaled, or non-DIS background control samples are currently available for the v5g high-Q² workspace.

**Input artifacts:** background research note, MC inventory, systematics reference  
**Output artifacts:** background-handle availability decision  
**Specialist:** `h1-mc-expert` + `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** none  
**Acceptance criteria:** executable variation path or blocked status is evidenced.

#### 10.2 [P1] Define subtraction / variation prescription for quoted background source
**Description:** Freeze the exact rule for the source, including whether the variation is a full subtraction difference, a factor-of-two normalization envelope, or a diffractive-excluded branch difference.

**Input artifacts:** 10.1 decision, prior references  
**Output artifacts:** background source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 10.1  
**Acceptance criteria:** one reproducible propagation rule is selected.

#### 10.3 [P1] Produce background-source uncertainty JSON
**Description:** Execute the chosen path and emit the source JSON or blocked placeholder.

**Input artifacts:** 10.2 definition  
**Output artifacts:** `background_subtraction_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics` + `h1-mc-expert`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 10.2  
**Acceptance criteria:** source status is explicit and machine-readable.

### 11. Beam-energy systematic

**Approach and rationale:** The v3 extractor already propagates per-event beam energies, so the source can be treated as a lightweight propagated variation rather than a full builder rerun unless execution disproves that assumption.

#### 11.1 [P2] Define beam-energy variation magnitude and propagation scope
**Description:** Freeze the small beam-energy source as a per-event input variation affecting kinematic reconstruction and, through it, 4D bin assignment.

**Input artifacts:** task brief, beam-energy references, v3 extractor memo  
**Output artifacts:** beam-energy source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** variation magnitude and affected observables are stated explicitly.

#### 11.2 [P2] Implement lightweight propagated beam-energy variation path
**Description:** Define the later implementation path that perturbs `BeamElecE` / `BeamProtP` inputs from the canonical v3 per-track basis and recomputes derived kinematics.

**Input artifacts:** v3 per-track CSV schema, beam-energy source definition  
**Output artifacts:** implementation contract for local propagated variation  
**Specialist:** `h1-systematics` + `code-editor`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** 11.1  
**Acceptance criteria:** no Condor rerun is required unless a concrete blocker is found.

#### 11.3 [P2] Emit beam-energy source JSON
**Description:** Run the variation and package the resulting shifts.

**Input artifacts:** 11.2 contract, nominal products  
**Output artifacts:** `beam_energy_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 11.2  
**Acceptance criteria:** observed effect size is plausibly small and finite in all populated bins.

### 12. Vertex z cut (|VtxZ| < 35 cm)

**Approach and rationale:** This is a classic cut-variation source and should be handled through a dedicated re-extraction branch because it changes event-level acceptance and denominator counts.

#### 12.1 [P1] Freeze VtxZ variation prescription (±5 cm)
**Description:** Record the exact alternative cut values and observable scope.

**Input artifacts:** task brief, current cut reference  
**Output artifacts:** vertex-z source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** source definition contains both alternative windows.

#### 12.2 [P1] Define and dispatch VtxZ variation extraction sweeps
**Description:** Set up the required re-extraction passes for the tighter and looser cuts and run them on Condor.

**Input artifacts:** source definition, extraction chain  
**Output artifacts:** shifted VtxZ products  
**Specialist:** `h1-production` + `code-editor`  
**Estimated effort:** L (4–8 h active, multi-day wall clock)  
**Dependencies:** 12.1  
**Acceptance criteria:** both VtxZ variants finish with valid outputs.

#### 12.3 [P1] Emit VtxZ source JSON
**Description:** Compare shifted products against nominal and emit the source JSON.

**Input artifacts:** 12.2 outputs, nominal products  
**Output artifacts:** `vertex_z_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** 12.2  
**Acceptance criteria:** source shifts are finite and the larger envelope is recorded.

### 13. y window (0.1 < y < 0.7)

**Approach and rationale:** This is event-level and directly affects both DIS denominator and hadron numerator composition. Use re-extraction sweeps.

#### 13.1 [P1] Freeze y-window variation edges and labeling
**Description:** Document the exact alternate low/high edges to be tested around the canonical `0.1 < y < 0.7` selection.

**Input artifacts:** task brief, current cuts reference  
**Output artifacts:** y-window source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** both edge variations are defined and named consistently.

#### 13.2 [P1] Execute y-window variation sweeps
**Description:** Run the required variation passes on Condor and extract the shifted observables.

**Input artifacts:** 13.1 definition, nominal chain  
**Output artifacts:** y-varied observable products  
**Specialist:** `h1-production`  
**Estimated effort:** L (4–8 h active, multi-day wall clock)  
**Dependencies:** 13.1  
**Acceptance criteria:** all expected variant outputs are present and structurally valid.

#### 13.3 [P1] Emit y-window source JSON
**Description:** Build the source payload from the variant differences.

**Input artifacts:** 13.2 outputs, nominal products  
**Output artifacts:** `y_window_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** 13.2  
**Acceptance criteria:** source payload covers M, σ, A_h, and any denominator-driven metadata needed for traceability.

### 14. Empz cut (35 < Empz < 70)

**Approach and rationale:** Similar to the y-window source; especially relevant because the canonical widened Empz cut is new in v5g and must be stress-tested.

#### 14.1 [P1] Freeze Empz variation edges and extraction policy
**Description:** Specify the alternate Empz windows around `[35, 70]`, including whether the variation is symmetric or motivated by previous `[45,65]` history.

**Input artifacts:** task brief, Empz memo, cut reference  
**Output artifacts:** Empz source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** none  
**Acceptance criteria:** exact window values are recorded.

#### 14.2 [P1] Execute Empz variation sweeps
**Description:** Run the shifted extraction(s) on Condor.

**Input artifacts:** 14.1 definition  
**Output artifacts:** Empz-variant products  
**Specialist:** `h1-production`  
**Estimated effort:** L (4–8 h active, multi-day wall clock)  
**Dependencies:** 14.1  
**Acceptance criteria:** variant products complete with valid file counts.

#### 14.3 [P1] Emit Empz source JSON
**Description:** Compare variants to nominal and package the source.

**Input artifacts:** 14.2 outputs, nominal products  
**Output artifacts:** `empz_window_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** 14.2  
**Acceptance criteria:** widened-cut robustness is quantified bin-by-bin.

### 15. Track DCA / quality cut variation

**Approach and rationale:** This source can likely be handled locally on the v3 per-track basis if the CSV carries the needed quality columns; otherwise a focused re-extraction is required. The main target variation requested is `|DcaPrime/cosh(Eta)| < 2.0 -> 1.5 / 2.5`.

#### 15.1 [P0] Audit whether canonical v3 per-track CSV contains all needed quality columns
**Description:** Determine whether DCA and any other requested quality-cut fields are present in the canonical v3 local artifacts so the source can be evaluated without a full rerun.

**Input artifacts:** v3 per-track CSV schema, extraction notes  
**Output artifacts:** local-versus-rerun decision note  
**Specialist:** `h1-systematics` + `code-editor`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** none  
**Acceptance criteria:** source is classified as local or Condor-backed with evidence.

#### 15.2 [P0] Freeze DCA / quality variation prescription
**Description:** Define the nominal, tighter, and looser cuts and any additional quality handles to be bundled or split.

**Input artifacts:** task brief, tracking research note, 15.1 audit  
**Output artifacts:** DCA / quality source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1 h)  
**Dependencies:** 15.1  
**Acceptance criteria:** exact thresholds are frozen and source scope is clear.

#### 15.3 [P0] Execute local or Condor-backed DCA / quality variations
**Description:** Run the variation path selected in 15.1 and generate variant observables.

**Input artifacts:** 15.2 definition, nominal products or extraction chain  
**Output artifacts:** DCA/quality variant products  
**Specialist:** `h1-systematics` or `h1-production` depending path  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 15.2  
**Acceptance criteria:** variant observables are produced for all required observable classes.

#### 15.4 [P0] Emit DCA / quality source JSON
**Description:** Compare variants to nominal and emit the uncertainty source.

**Input artifacts:** 15.3 outputs  
**Output artifacts:** `track_quality_dca_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** 15.3  
**Acceptance criteria:** source shows expected larger effects in low-pT / migration-sensitive bins.

### 16. NucliaWeight clipping

**Approach and rationale:** The NuclIA reference already identifies an incomplete sentinel guard and recommends clipping to `(0,1]`. This source is naturally a per-track weight variation and can be done locally if the per-track basis includes NuclIA weights.

#### 16.1 [P1] Freeze nominal and alternative NuclIA clipping thresholds
**Description:** Define the nominal clip policy (`clip at 10` per task brief wording, while preserving the actual stored-weight semantics) and the alternative thresholds to test.

**Input artifacts:** task brief, NuclIA reference  
**Output artifacts:** NuclIA clip source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** none  
**Acceptance criteria:** source definition reconciles the task brief with the reference semantics clearly.

#### 16.2 [P1] Implement local NuclIA clipping variation contract
**Description:** Specify the exact later implementation path for applying alternative clip guards on the nominal per-track basis and recomputing observables.

**Input artifacts:** per-track schema, NuclIA reference  
**Output artifacts:** local reweight implementation contract  
**Specialist:** `h1-systematics` + `code-editor`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 16.1  
**Acceptance criteria:** all edge-case weights have deterministic treatment.

#### 16.3 [P1] Emit NuclIA clipping source JSON
**Description:** Run the local reweight variations and package the resulting shifts.

**Input artifacts:** 16.2 contract, nominal products  
**Output artifacts:** `nuclia_clip_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 16.2  
**Acceptance criteria:** source JSON exists; negative/unphysical weights are handled explicitly and counted.

### 17. ep0304 inclusion variation

**Approach and rationale:** The handoff is clear that ep0304 is analysis-included by default but should remain a dedicated cross-check in systematics. The requested prescription is half the 4-period versus 3-period difference.

#### 17.1 [P1] Freeze 4-period versus 3-period comparison contract
**Description:** Define exactly which three-period sample is used as the alternative and how luminosity and period bookkeeping are normalized for the comparison.

**Input artifacts:** handoff period policy, GRL / luminosity reference  
**Output artifacts:** ep0304 source definition  
**Specialist:** `h1-systematics` + `h1-data-validator`  
**Estimated effort:** S (1–2 h)  
**Dependencies:** none  
**Acceptance criteria:** alternative sample and half-difference rule are explicit.

#### 17.2 [P1] Produce 3-period comparison observables
**Description:** Recompute observables with ep0304 excluded using the canonical v3 methodology.

**Input artifacts:** nominal chain, period selection policy  
**Output artifacts:** 3-period comparison JSON set  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 17.1  
**Acceptance criteria:** 3-period outputs match nominal schema and period accounting is correct.

#### 17.3 [P1] Emit ep0304 source JSON
**Description:** Compare 4-period and 3-period observables and store half the difference as the source shift.

**Input artifacts:** nominal and 3-period observables  
**Output artifacts:** `ep0304_inclusion_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** S–M (1–3 h)  
**Dependencies:** 17.2  
**Acceptance criteria:** source JSON records both raw difference and quoted half-difference.

### 18. Frame definition systematic

**Approach and rationale:** This is unique to the v3 era and must be handled explicitly. The canonical v3 CM-frame definition remains nominal; the source is the difference to an alternate Breit-frame `pT_h` convention or equivalent wrong-but-standardized comparison branch as requested.

#### 18.1 [P0] Freeze the alternative frame convention and its scope
**Description:** Specify exactly what “Breit-frame pT_h as a frame-choice cross-check” means operationally and which observables it can affect. This must avoid ambiguity between a physics convention alternative and the already-known broken lab-frame bug.

**Input artifacts:** frame-bug memo, task brief, SIDIS frame reference  
**Output artifacts:** frame-systematic source definition  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–3 h)  
**Dependencies:** none  
**Acceptance criteria:** alternative frame is defined unambiguously and is not conflated with the invalid lab-frame bug.

#### 18.2 [P0] Define local alternative-frame recomputation contract from v3 basis
**Description:** Specify the later implementation path to recompute `pT_h`, `z`, and any frame-sensitive φ_h observables in the alternative frame using the canonical v3 per-track basis.

**Input artifacts:** v3 extractor outputs, frame memo  
**Output artifacts:** implementation contract for frame-choice variation  
**Specialist:** `code-editor` + `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 18.1  
**Acceptance criteria:** contract is detailed enough for a standalone specialist implementation dispatch.

#### 18.3 [P0] Produce alternative-frame observables and source JSON
**Description:** Run the alternative-frame computation and package the resulting observable differences as the frame-convention source.

**Input artifacts:** 18.2 contract, nominal observables  
**Output artifacts:** `frame_definition_v3_HQv5gBprime.json`  
**Specialist:** `h1-systematics`  
**Estimated effort:** M (2–4 h)  
**Dependencies:** 18.2  
**Acceptance criteria:** source JSON quantifies the effect on `M`, `A_h`, `σ`, and especially φ_h harmonics.

## Combination and reporting

### Combination rules

1. **Uncorrelated or effectively local sources:** combine in quadrature per bin.
2. **Global normalization-only source (luminosity):** keep separate from `M` and `A_h`; add to `σ` as a correlated normalization band.
3. **Migration-driven directional sources (HFS, electron-scale, angle, cut-window sweeps):** keep up/down vectors and derive the quoted symmetric envelope as the max absolute shift. Preserve direction-aware payload for future covariance building.
4. **Correlated source covariance construction:** for any source treated as fully correlated across bins, store the shift vector so downstream can build
   
   ```python
   cov_source = np.outer(delta_vec, delta_vec)
   ```
5. **A_h propagation:** never combine h+ and h− source terms by hand before forming the asymmetry. Always compute shifted h+/h− yields or multiplicities first, then form the varied `A_h`.
6. **Unfolding-dependent bins:** bins with purity < 0.5 remain in the master JSON with `status="deferred_to_unfolding"`; they do not receive a final bin-by-bin systematic total until the unfolding workstream supplies corrected central values and, if needed, unfolding-systematic payloads.

### Final output products

- **Master JSON:** `H1/sidis_highQ2/data/systematics_v3_HQv5gBprime_2026-05-24.json`
- **Per-source JSONs:** one file per source under `data/systematics_sources/`
- **Breakdown PDF:** dominant-source summary and per-source heatmaps under `plots/systematics_v3_HQv5gBprime/`
- **Final observable table with uncertainties:** updated combined tables for `M^{h±}`, `A_h`, `σ`, and φ_h harmonics, each carrying `stat`, `syst`, and `stat⊕syst` columns or structured JSON equivalents.

### Combination implementation checks

- The sum of enabled source blocks must reproduce the total systematic arrays exactly.
- `σ` totals must be reproducible with and without the separate luminosity band.
- `A_h` totals must demonstrably exclude luminosity and any source marked `cancels` exactly.
- Master JSON must preserve a `blocked_sources` and `deferred_bins` section.

## Risk register

| Risk | Why it matters | Mitigation in this plan |
|---|---|---|
| HFS / electron migration cannot be approximated reliably by weights alone | Misestimates 4D bin migration, especially in `(Q²,x)` and high-z corners | Treat these as Condor-backed shifted extractions, not purely analytic sources |
| Upstream LAr electron-scale procedure is not actually solved | Could stall source 3/4 if assumed reusable | Freeze SIDIS-local working amplitudes and explicit execution contract |
| PDF uncertainty infrastructure may be absent | Source 8 could silently disappear | Keep it as explicit executable-or-blocked source with schema placeholder |
| C_rad alternative path may not be execution-ready | Source 9 can become ambiguous in totals | Require alternative hierarchy and status flag before combination |
| Low-purity bins need unfolding | Final 4D table can otherwise mix incompatible correction methods | Mark bins explicitly as `deferred_to_unfolding`; do not force BBB totals there |
| NuclIA branch contains rare unphysical values | Can inject sign-flipped track weights if not guarded | Explicit clipping / guard source and count of affected entries |
| ep0304 source may be over-interpreted | Could double-count with period-composition effects | Keep source definition explicit as half-difference and separate from generic period systematics |
| φ_h / frame-source ambiguity | Could accidentally compare canonical result to known-bad lab-frame bug | Freeze alternative frame definition before implementation |
| Large number of Condor variants | Can stretch wall-clock schedule and DUST usage | Sequence sweeps by priority and reuse minimal extracted products |

### Condor-required sources

These **require Condor sweeps** in the current plan:
- 2. HFS scale
- 3. Electron energy scale
- 4. Electron angle
- 12. Vertex-z cut
- 13. y-window
- 14. Empz window
- 15. DCA/quality cut **if** not available from local v3 columns
- 10. Background subtraction **if** alternate MC branch must be regenerated
- Potentially 9. C_rad model uncertainty, depending on available alternative path

### Local / fast sources

These are expected to be **local reweight / analytic / split passes**:
- 1. Tracking efficiency
- 5. Trigger efficiency
- 6. Luminosity
- 7. MC model difference aggregation once alternative MC observables exist
- 8. PDF set uncertainty if reweight vectors exist
- 11. Beam energy
- 16. NuclIA clipping
- 17. ep0304 inclusion
- 18. Frame definition systematic

## Resource estimate

### Condor sweeps proposed

Nominal proposed sweeps / campaigns:
1. HFS up
2. HFS down
3. Electron-energy up
4. Electron-energy down
5. Electron-angle up
6. Electron-angle down
7. Vertex-z tight
8. Vertex-z loose
9. y-window low-edge / tight variant
10. y-window high-edge / loose variant
11. Empz tight / historical-comparison variant
12. Empz loose variant

**Additional conditional sweeps:**
- +2 if DCA / quality variation cannot be done locally
- +1 to +2 if background-subtraction variation requires dedicated alternate MC production
- +1 if C_rad alternative requires dedicated production-like extraction

**Planning headline count:** **12 core Condor sweeps**, **up to 16 including conditional branches**.

### CPU / wall-clock estimate

- Local per-source JSON derivations: minutes to a few hours each.
- Each Condor systematic sweep: expected on the order of one production-style pass over current files, typically **1–2 wall-clock days queued+run+merge**, but multiple sweeps can overlap if storage and queue headroom allow.
- End-to-end calendar estimate with prioritization and overlap: **10–14 days**.

### Disk estimate

- Source JSONs and plots: negligible (< few GB total).
- Shifted extracted products: moderate; assume **O(10–100 GB)** total depending on whether outputs are compact CSV/JSON derivatives or heavier intermediate ROOT artifacts.
- Plan requires checking DUST headroom before each large systematic campaign per `H1/CONTEXT.md` policy.

## Acceptance criteria for the analysis as a whole

Declare **"systematics done, ready for next analysis gate / unblinding preparation"** only when all of the following hold:

1. Every one of the **18 requested systematic sources** appears in the master JSON with status `ok`, `blocked`, or `deferred_to_unfolding`.
2. All **purity ≥ 0.5** bins have final `stat`, `syst`, and `stat⊕syst` values for `M^{h±}`, `A_h`, and `σ`; φ_h harmonics have corresponding source coverage where defined.
3. The **59 low-purity bins** are explicitly flagged and separated from final BBB totals pending the unfolding workstream.
4. The total systematic arrays reproduce the quadrature / covariance combination of the per-source blocks exactly.
5. Luminosity appears only where allowed (`σ`, not `M` or `A_h`).
6. Trigger treatment remains consistent with the frozen data-only S67 policy.
7. No per-source JSON contains silent NaNs in populated bins; all missing values are explicit nulls with status metadata.
8. The model-dependence source and the frame-definition source both pass sanity review in the kinematic corners where large shifts are expected.
9. A compact validation memo / plot set exists showing dominant sources, largest bins, and any blocked or unfolding-deferred regions.
10. Supervisor signs off that any remaining blocked sources are acceptable for the intended next milestone, or execution continues until they are cleared.

## Cross-references

### Current canonical state
- `H1/sidis_highQ2/context-handoff.md`
- `H1/sidis_highQ2/docs/methodology/binning_adoption_HQ-v5g-Bprime_2026-05-24.md`
- `H1/sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md`

### Prior systematics and methodology references
- `H1/sidis_highQ2/docs/results/systematics_v5f_hfs_scale_v2.md`
- `H1/sidis_highQ2/scripts/compute_systematics_v5f_track_eff_chargedep.py`
- `H1/sidis_highQ2/scripts/systematics_v5f_nuclia_proton_omission.py`
- `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md`

### Supporting research notes
- `H1/sidis_highQ2/docs/research/research-tracking-systematics-deep.md`
- `H1/sidis_highQ2/docs/research/research-crad-for-charged-sidis.md`
- `H1/sidis_highQ2/docs/research/research-background-sources-migration.md`
- `H1/sidis_highQ2/docs/research/research-4d-unfolding.md`

### Detector / cut / trigger / NuclIA references
- `H1/sidis_highQ2/docs/references/reference-highQ2-charged-sidis-analysis-cuts.md`
- `H1/sidis_highQ2/docs/references/reference-trigger-semantics-highQ2-sidis.md`
- `H1/sidis_highQ2/docs/references/reference-nuclia-correction-highQ2-sidis.md`
- `H1/sidis_highQ2/docs/references/reference-LAr-electron-scale-highQ2-sidis.md`
- `H1/sidis_highQ2/docs/references/reference-highQ2-crad-availability.md`

## Todo

1. **Phase 1 — Freeze nominal contracts and schemas**
   1.1 [P0] Freeze nominal observable input manifest for the systematics program.  
   **Depends on:** none.  
   **Acceptance:** all canonical nominal paths and array shapes are validated against Scheme B′.

   1.2 [P0] Freeze master JSON schema and per-source metadata contract.  
   **Depends on:** 1.1.  
   **Acceptance:** one master output convention exists and covers nominal, blocked, and unfolding-deferred bins.

   1.3 [P0] Freeze low-purity-bin handling rule and unfolding boundary.  
   **Depends on:** 1.1.  
   **Acceptance:** the 59 low-purity bins are represented explicitly in the output contract.

2. **Phase 2 — Fast local / analytic / reweight sources**
   2.1 [P0] Freeze tracking-efficiency common and charge-dependent source definitions.  
   **Depends on:** 1.1, 1.2.  
   **Acceptance:** cancellation classes and estimator logic are documented.

   2.2 [P0] Produce v3 tracking common / charge-dependent source JSONs.  
   **Depends on:** 2.1.  
   **Acceptance:** both tracking source JSONs validate and cover populated bins.

   2.3 [P1] Freeze trigger-efficiency variation prescription.  
   **Depends on:** 1.1, 1.2.  
   **Acceptance:** one data-side trigger source path is defined or marked blocked.

   2.4 [P1] Produce trigger-efficiency source JSON.  
   **Depends on:** 2.3.  
   **Acceptance:** bin-by-bin trigger shifts or blocked status are emitted.

   2.5 [P1] Produce luminosity normalization-only source JSON.  
   **Depends on:** 1.2.  
   **Acceptance:** only `σ` is affected; `M` and `A_h` are excluded by design.

   2.6 [P2] Audit PDF reweight availability and freeze source status.  
   **Depends on:** 1.2.  
   **Acceptance:** source becomes executable or explicitly blocked.

   2.7 [P2] Emit PDF source JSON or blocked placeholder.  
   **Depends on:** 2.6.  
   **Acceptance:** source appears in schema without ambiguity.

   2.8 [P2] Freeze and implement beam-energy propagated variation path.  
   **Depends on:** 1.1.  
   **Acceptance:** local propagated variation path is defined.

   2.9 [P2] Emit beam-energy source JSON.  
   **Depends on:** 2.8.  
   **Acceptance:** source payload is finite and small as expected.

   2.10 [P1] Freeze NuclIA clipping source definition.  
   **Depends on:** 1.1.  
   **Acceptance:** clipping semantics reconcile the task brief and NuclIA reference.

   2.11 [P1] Emit NuclIA clipping source JSON.  
   **Depends on:** 2.10.  
   **Acceptance:** clipping-affected entries are counted and handled explicitly.

   2.12 [P1] Freeze ep0304 inclusion source definition.  
   **Depends on:** 1.1.  
   **Acceptance:** half-difference rule and sample comparison are explicit.

   2.13 [P1] Produce 3-period comparison observables and ep0304 source JSON.  
   **Depends on:** 2.12.  
   **Acceptance:** source JSON records raw and quoted half-differences.

   2.14 [P0] Freeze frame-definition alternative convention.  
   **Depends on:** 1.1.  
   **Acceptance:** alternative frame is unambiguous and not the known-bad lab-frame bug.

   2.15 [P0] Emit frame-definition source JSON.  
   **Depends on:** 2.14.  
   **Acceptance:** effect on φ_h harmonics is quantified explicitly.

3. **Phase 3 — Condor-backed migration and cut-window sources**
   3.1 [P0] Audit v5f HFS systematic memo and freeze v3 HFS shifted-run contract.  
   **Depends on:** 1.1.  
   **Acceptance:** HFS shifted extraction naming and outputs are frozen.

   3.2 [P0] Run HFS-up/down Condor sweeps.  
   **Depends on:** 3.1.  
   **Acceptance:** both sweeps complete with valid outputs.

   3.3 [P0] Emit HFS source JSON.  
   **Depends on:** 3.2.  
   **Acceptance:** source contains directional and quoted envelope shifts.

   3.4 [P0] Freeze electron-energy variation contract.  
   **Depends on:** 1.1, 1.2.  
   **Acceptance:** SIDIS-local LAr energy-shift procedure is dispatch-ready.

   3.5 [P0] Run electron-energy shifted Condor sweep(s).  
   **Depends on:** 3.4.  
   **Acceptance:** shifted outputs are valid.

   3.6 [P0] Emit electron-energy source JSON.  
   **Depends on:** 3.5.  
   **Acceptance:** all observable families receive payloads.

   3.7 [P0] Freeze electron-angle variation contract.  
   **Depends on:** 1.1, 1.2.  
   **Acceptance:** SIDIS-local θ_e procedure is dispatch-ready.

   3.8 [P0] Run electron-angle shifted Condor sweep(s).  
   **Depends on:** 3.7.  
   **Acceptance:** shifted outputs are valid.

   3.9 [P0] Emit electron-angle source JSON.  
   **Depends on:** 3.8.  
   **Acceptance:** source JSON is complete and direction-aware.

   3.10 [P1] Freeze VtxZ variation contract and run sweeps.  
   **Depends on:** 1.1.  
   **Acceptance:** tight and loose variants complete.

   3.11 [P1] Emit VtxZ source JSON.  
   **Depends on:** 3.10.  
   **Acceptance:** larger-envelope per-bin shifts are recorded.

   3.12 [P1] Freeze y-window variation contract and run sweeps.  
   **Depends on:** 1.1.  
   **Acceptance:** all requested variants complete.

   3.13 [P1] Emit y-window source JSON.  
   **Depends on:** 3.12.  
   **Acceptance:** source covers numerator and denominator-driven effects.

   3.14 [P1] Freeze Empz variation contract and run sweeps.  
   **Depends on:** 1.1.  
   **Acceptance:** variant products complete successfully.

   3.15 [P1] Emit Empz source JSON.  
   **Depends on:** 3.14.  
   **Acceptance:** widened-cut robustness is quantified.

   3.16 [P0] Audit whether DCA / quality variation can be done locally.  
   **Depends on:** 1.1.  
   **Acceptance:** local vs Condor path is evidenced.

   3.17 [P0] Execute DCA / quality variations on the chosen path.  
   **Depends on:** 3.16.  
   **Acceptance:** variant observables are produced.

   3.18 [P0] Emit DCA / quality source JSON.  
   **Depends on:** 3.17.  
   **Acceptance:** low-pT / migration-sensitive structure is plausible.

4. **Phase 4 — Model, radiative, and background sources**
   4.1 [P0] Freeze DJANGOH-vs-RAPGAP model-dependence contract.  
   **Depends on:** 1.1.  
   **Acceptance:** half-difference rule is explicit.

   4.2 [P0] Recompute alternative RAPGAP31 observable chain.  
   **Depends on:** 4.1.  
   **Acceptance:** alternative observable JSONs exist with nominal-compatible schema.

   4.3 [P0] Emit MC model-dependence source JSON.  
   **Depends on:** 4.2.  
   **Acceptance:** high-z / high-pT bins are populated and not silently dropped.

   4.4 [P0] Freeze C_rad alternative hierarchy and source status.  
   **Depends on:** 1.1.  
   **Acceptance:** nominal and alternative paths are defined or blocking is explicit.

   4.5 [P0] Produce alternative C_rad table(s).  
   **Depends on:** 4.4.  
   **Acceptance:** alternative C_rad is available on nominal binning.

   4.6 [P0] Emit C_rad model-uncertainty source JSON.  
   **Depends on:** 4.5.  
   **Acceptance:** per-charge `A_h` propagation is included.

   4.7 [P1] Inventory executable background-subtraction variation handles.  
   **Depends on:** 1.1.  
   **Acceptance:** source is executable or blocked with evidence.

   4.8 [P1] Freeze background-subtraction prescription.  
   **Depends on:** 4.7.  
   **Acceptance:** one reproducible rule is chosen.

   4.9 [P1] Emit background-subtraction source JSON or blocked placeholder.  
   **Depends on:** 4.8.  
   **Acceptance:** source status is machine-readable and ready for combination.

5. **Phase 5 — Combination, validation, and final products**
   5.1 [P0] Validate every per-source JSON against the master schema.  
   **Depends on:** all source-production tasks.  
   **Acceptance:** no malformed source payload remains.

   5.2 [P0] Build per-observable total systematic arrays for BBB-domain bins.  
   **Depends on:** 5.1.  
   **Acceptance:** totals reproduce the per-source combination rules exactly.

   5.3 [P0] Add statistical + systematic combined uncertainties to final observable tables.  
   **Depends on:** 5.2.  
   **Acceptance:** `stat`, `syst`, and `stat⊕syst` are present for all purity≥0.5 bins.

   5.4 [P0] Mark low-purity bins as deferred to unfolding in the master JSON.  
   **Depends on:** 5.2.  
   **Acceptance:** all 59 bins are preserved with explicit deferred status.

   5.5 [P0] Produce dominant-source and per-source breakdown plots/PDFs.  
   **Depends on:** 5.2.  
   **Acceptance:** plots exist for internal review and largest bins are inspectable.

   5.6 [P0] Perform final validation gate for “systematics done”.  
   **Depends on:** 5.3, 5.4, 5.5.  
   **Acceptance:** all whole-analysis acceptance criteria in this plan are satisfied or blocked items are explicitly signed off by supervisor.

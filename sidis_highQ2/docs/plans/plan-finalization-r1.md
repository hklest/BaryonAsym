# Plan: High-Q2 Charged-Particle SIDIS Finalization (r2)

> **SUPERSEDED 2026-10-27:** replaced by the program plan `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/plans/2026-10-27-sidis-highq2-finalization.md` (its "What finalized means" table maps §7 to the eΣ campaign); frozen edition `esigmaphotonxf_2026-10-27b`. History only.

> **Revision note (r2, 2026-05-22):** revises r1 in response to 10 planning-critic defects. Main changes: explicit Phase 2 MC/data rerun matrix for electron-side iSys, corrected NAF-vs-local code anchors, CORE vs EXTENDED critical-path split, low-z `phi_h` exclusion default with new CG-7.0 gate, expanded HQ-v5f-r2 script touch list, synchronized timeline numbers, HB5 cross-regime coordination note + CG-6.0, source-disposition table, corrected CG-4.1 specialist, and an explicit >3× Pass-B disagreement failure branch.

## 0. Plan metadata

- **Date:** 2026-05-22
- **Revision history:**
  - **r2 (2026-05-22):** planning-critic revision pass; resolved the 10 defects listed in the r2 update request while preserving the r1 phase structure.
  - **r1 (2026-05-22):** initial finalization plan.
- **Supervisor of record:** `h1-sidis-supervisor`
- **Scope:** finalization plan for the H1 charged-particle SIDIS high-Q² analysis in `H1/sidis_highQ2/`, covering the path from the current HQ-v5f-r1 / partially rebuilt HQ-v5f-r2 state to an **internal-package-ready analysis** (the old r5 `G32` milestone) and then the remaining gates to a **publishable result**.
- **Primary analysis regime:** `Q2 > 150 GeV^2`
- **Nominal frozen frame / convention anchors:** F1 (`γ*+p` HCM frame), F14 (`A_BC` blinding), F15 (γ*+p frame + Trento convention)
- **Nominal frozen binning anchor:** F2-r2 = `HQ-v5f-r2` with `z = [0.0,0.1,0.2,0.3,0.5,0.7,1.0]`, 480 bins total, as approved in `docs/handoffs/decision-binning-amendment-HQv5fr2.md`
- **Primary authoritative state inputs used by this plan:**
  - `sidis_highQ2/context-handoff.md`
  - `sidis_highQ2/docs/references/reference-highQ2-charged-sidis-analysis-cuts.md`
  - `sidis_highQ2/docs/handoffs/decision-binning-amendment-HQv5fr2.md`
  - `sidis_highQ2/docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md`
  - `sidis_highQ2/docs/results/systematics_v5f_rigor_status_per_source.md`
  - `sidis_highQ2/docs/data/systematics_v5f_aggregated_v2.json`
  - `sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md`
- **This plan supersedes:** `sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md` **r5** specifically on the following fronts:
  1. the **systematics rigorization push**, including the 14-source v2 aggregated budget and the six v1 defect fixes, and
  2. the **F2-r2 binning amendment** from HQ-v5f-r1 to HQ-v5f-r2.
- **Internal-package target:** a locally consistent, critic-reviewed results package with regenerated headline plots, per-observable JSONs, per-source systematic breakdowns, explicit caveats, and an internal memo that can be circulated inside H1.
- **Publishable target:** a result set in which all blockers that presently prevent public release are either resolved or explicitly cause a scoped depublication of the affected observable family.

### Non-goals

- This plan does **not** implement code.
- This plan does **not** run NAF jobs.
- This plan does **not** draft the publication paper text or HEPData payloads.
- This plan does **not** reopen frozen decisions F1, F2-r2, F3, F5, F6, F7, F8, F9, F11, F13, F14, or F15 without explicit supervisor escalation.

### Scope exclusions

- Low-Q² SIDIS execution details are out of scope except where explicitly relevant to the shared `C_rad` / NonRad blocker.
- `proton_HiQ/` and `lambda_HiQ/` are not execution owners for this analysis; they may only serve as factual comparison points for detector-systematics methodology or normalization closure patterns.
- Phase 9 publication preparation is acknowledged only as a trigger-dependent follow-on; it is intentionally **out of scope** for detailed execution in this plan revision.

---

## 1. Current state assessment

The analysis has crossed the threshold from “prototype / launch planning” into a **finalization program**: the nominal v5f observable stack exists on HQ-v5f-r1, the systematic budget has been materially rigorized, and the supervisor-approved HQ-v5f-r2 grid has partially landed. The remaining work is no longer primarily about whether the analysis can be done at all; it is about whether the remaining blockers can be closed in a way that is methodologically defensible for internal-package and publication claims.

The most important change relative to the old r5 plan is that there are now **two partially overlapping state layers**:

1. **HQ-v5f-r1 fully assembled layer**: all headline per-bin observables exist, the v2 14-source budget exists, and the known defects in the old v1 systematic bookkeeping have been corrected.
2. **HQ-v5f-r2 partially rebuilt layer**: the new 6-z-bin grid is frozen and already populated for data yields, acceptance, and unfolding comparison, but multiplicity / cross-section / asymmetry assembly is blocked until `n_dis_eps_dis` is re-extracted on the new grid.

This means the analysis is already scientifically legible, but the **finalization baseline must now migrate from r1 to r2** without losing the rigor improvements achieved on r1.

### 1.1 Gate status table (mapped from the existing r5 gate vocabulary)

| Gate / blocker | Meaning in r5 vocabulary | Current status | Notes for finalization plan |
|---|---|---|---|
| G1A / inventory accounting | Production/accounting freeze | **Closed** | v5f tree inventory is known: 973 data files across `em0405 + em06 + ep0607`; 1209 MC files (`723 RAPGAP31 + 486 DJANGOH14`). |
| G1B / merge-vs-iterate operational choice | MC execution mode | **Closed enough for planning** | No longer a decision-driving gate for finalization; downstream products already exist. |
| G1C / data production coverage | Full data-side period coverage | **Partially closed, publication-open** | Three-period sum exists (`em0405 + em06 + ep0607`), but **ep0304 remains excluded by extractor design**. This is the practical content of **OQ7**. |
| G2 / schema & branch validation | Tree-level schema freeze | **Closed** | No new schema blocker surfaced in the rigorization push. |
| G3 / truth matching + species policy | Gen pool and matching semantics | **Closed** | Frozen by F9 and matching references. Species definition: **Option H** (primary fragmentation π/K/p, includes Δ/Σ*/Ξ* and charm/bottom decay daughters, excludes Λ/K⁰ₛ/Σ±/Ξ/Ω daughters) — frozen 2026-05-22. See `H1/docs/shared-species-definition-option-h-2026-05-22.md`. |
| G4 / DIS counting semantics | `DisTot` / zero-hadron denominator logic | **Closed on r1; r2 assembly-open** | Logic is settled; the new blocker is **rerunning `n_dis_eps_dis` on HQ-v5f-r2**. |
| G5 / binning study | Frozen nominal binning | **Superseded** | HQ-v5f-r1 frozen study is no longer the finalization target. **F2-r2** is now binding. |
| G6 / acceptance map and event-cut symmetry | Acceptance correctness | **Closed on r1; r2 refresh partly done** | Acceptance on HQ-v5f-r2 already extracted, but full result stack still needs reassembly. |
| G7 / multiplicity construction | Main observable assembly | **Closed on r1; open on r2** | HQ-v5f-r1 multiplicity exists; HQ-v5f-r2 multiplicity blocked by missing `n_dis_eps_dis`. |
| G8 / bin-classification / stability tagging | Stability / migration categorization | **Open for r2** | New z-edge bins need explicit stability tagging per the F2-r2 decision. |
| G9 / headline multiplicity result | Publishable multiplicities path | **Internal-package-ready on r1, not final on r2** | Needs r2 rebuild plus placeholder retirement steps to become publishable. |
| G10 / radiative corrections | `C_rad` treatment | **Open / hard blocker** | Real NonRad numerator absent; current `crad_placeholder` remains in headline budgets. |
| G11 / systematic program | Detector / correction / closure systematics | **Partly closed, partly open** | Rigorization completed most sources, but **Pass-B iSys reproduction** and **Option H scan** remain open. |
| G12 / unfolding choice and regularization | Response correction method | **Closed enough for r1, review-open for Pass-B/r2** | Default is now keep-all method spread; one critic review remains required before Pass-B detector reruns are adopted. |
| G13 / `phi_h` moments | Azimuthal moments | **Closed on r1 baseline, open on r2 and open on full-systematics realism** | Current moments exist per cell; r2 extraction and replacement of bounded prescriptions remain open. |
| G14 / absolute normalization | H1CalcWeight / effective MC luminosity | **Open / HB5 blocker** | Blocks publication of absolute hadron cross-sections. |
| G15 / e- period recovery | Full e± coverage for asymmetries | **Open / OQ7 blocker** | ep0304 files exist but are excluded; publication-grade `A_h` and `A_BC` remain blocked. |
| G15.5 / asymmetry physics validation | A_BC-specific credibility | **Open** | Depends on OQ7, `C_rad`, and final cancellation checks. |
| G16 / internal package | Old r5 internal-package assembly gate | **Not yet passed** | This plan’s Phase 8 is the modernized path to old `G32`/`G16` package readiness. |
| G17 / publication package | Publishable result | **Open** | Depends on HB5, OQ7, real `C_rad`, and detector-systematics completion. |

### 1.2 Publication blockers and their current interpretation

| Blocker tag | Description | Current effect | Can internal package proceed before it closes? |
|---|---|---|---|
| **HB5** | H1CalcWeight / effective MC luminosity not validated end to end | Blocks absolute `d^4σ^{h±}/dQ^2 d x d z d pT_h` from publication | **Yes**, if absolute cross-sections are clearly marked non-headline or excluded from package sign-off |
| **OQ7** | e- period recovery incomplete; practical missing period is `ep0304` | Blocks publication-grade `A_h` and `A_BC` | **Partly**; blinded/internal asymmetry products can continue under F14 |
| **NonRad blocker** | Charged-SIDIS NonRad MC unavailable | Leaves `crad_placeholder` in headline budget | **Yes** for internal package with caveats; **No** for fully finalized headline publication budget |
| **Pass-B detector iSys** | Proper iSys-loop detector-scale reruns not yet reproduced | Four detector-scale sources remain analytic approximations | **Yes**, but only as an interim internal-package state, not as full finalization |
| **Option H scan** | Flat 4% placeholder retained pending per-component scan | One source remains flat-tagged with provenance, not per-bin evaluated | **Yes**, but not for “analysis finalized” status |
| **HQ-v5f-r2 assembly gap** | Multiplicity / σ / asymmetry stack incomplete on the new 6-z grid | Prevents migration of headline package to the supervisor-approved final grid | **No**; internal-package baseline should be HQ-v5f-r2 |

### 1.3 Systematic-source status table

The v2 aggregated budget contains **14 sources**. Twelve already carry real numbers in the current headline budget; two still carry explicit placeholders. The table below is the planning baseline for what must be preserved, upgraded, or retired.

| # | Source name | Current evaluation class | Rigor class | Pass-B requirement | Supervisor decision state |
|---:|---|---|---|---|---|
| 1 | `model_rapgap_djangoh` | `fully_evaluated_mc_side_per_bin_v2` | **per-bin** | No mandatory Pass-B rerun | Frozen as nominal model-spread source |
| 2 | `unfolding_method_spread` | `fully_evaluated_mc_side_per_bin_v2_gated_keep_all` | **per-bin** | Review only; not Pass-B dependent | Frozen nominal default = keep-all envelope |
| 3 | `nuclia_proton_omission` | `fully_evaluated_mc_closure_per_bin_v2` | **per-bin** | No immediate Pass-B dependency | Frozen nominal closure source |
| 4 | `lar_electron_escale` | `analytic_per_bin_approximation_v2` | **per-bin, analytic** | **Yes** — replace with iSys rerun | Old r5 LAr option question remains effectively unresolved until Pass-B lands |
| 5 | `lar_electron_angle` | `analytic_per_bin_approximation_v2` | **per-bin, analytic** | **Yes** — replace with iSys rerun | Same as above |
| 6 | `hfs_hadronic_scale` | `analytic_per_bin_approximation_v2` | **per-bin, analytic** | **Yes** — replace with iSys rerun | Same as above |
| 7 | `trigger_eff` | `per_q2_constant_v2` | **flat-tagged / structured** | Optional Pass-B-style remeasurement | Open citation vs remeasurement decision |
| 8 | `track_eff` | `flat_v1_catalog_common_piece` | **flat-tagged** | No immediate Pass-B rerun required | Retained pending stronger data-driven cross-check |
| 9 | `beam_energy_per_run` | `analytic_per_bin_approximation_v2` | **per-bin, analytic** | Desirable but not the core Pass-B campaign in this plan | F13 frozen for nominal use |
| 10 | `luminosity` | `flat_correlated_per_bin_propagated_v2` | **flat-tagged / correlated** | Optional deeper closure under HB5 | 2.5% H1-standard retained |
| 11 | `charge_flip` | `analytic_per_bin_exact_v2` | **per-bin** | No Pass-B dependency for nominal adoption | v2 fix is already the nominal |
| 12 | `crad_placeholder` | `placeholder_pending_norad_mc_production_v2` | **placeholder** | Replaced by real `C_rad` program, not Pass-B | Open strategy decision |
| 13 | `option_h_mask` | `flat_pending_pass_B_per_component_scan_v2` | **placeholder-ish flat with provenance** | **Yes** — dedicated scan on NAF | Frozen F11 mask, but source magnitude still provisional |
| 14 | `track_eff_chargedep` | `mixed_mc_driven_and_h1_flat_bound_v2` | **per-bin hybrid** | No immediate Pass-B rerun required | Kept as explicit separate source |

### 1.4 Source disposition table for finalization

This table closes the planning gap for sources that are not on an active upgrade path in every phase. “Finalized” below means “good enough for the internal package under the stated conditions,” not automatically “publication-perfect in every observable.”

| Source | Disposition | Justification |
|---|---|---|
| `model_rapgap_djangoh` | Finalized for internal package | Per-bin v2 rigorized; supervisor recommendation A adopted |
| `unfolding_method_spread` | Finalized for internal package | Per-bin v2 with stability tagging |
| `nuclia_proton_omission` | Finalized for internal package | Per-bin v2 with closure-derived `A_BC` and physics-grounded `phi` |
| `track_eff_chargedep` | Finalized for internal package | Stat-aware combination; per-bin where MC resolves, else flat 0.3% |
| `lar_electron_escale` | Finalized for internal **if** Pass-B agrees within factor 2 | Analytic v2 acceptable for interim internal use; Pass-B replaces for publication |
| `lar_electron_angle` | Finalized for internal **if** Pass-B agrees within factor 2 | Same |
| `hfs_hadronic_scale` | Finalized for internal **if** Pass-B agrees within factor 2 | Same |
| `beam_energy_per_run` | Finalized for internal **and** publication | Analytic v2 is rigorous; effect is small (~0.11%) |
| `trigger_eff` | Finalized for internal **if** per-`Q²` numbers can be cited; recheck for publication | Per-`Q²` constants from H1 internal note |
| `track_eff` | Finalized for internal **if** per-`Q²` numbers can be cited; H1-standard for publication | H1 0.5%/track |
| `luminosity` | Finalized | H1-standard 2.5% per period |
| `charge_flip` | Finalized | Analytic per-bin from `f=0.0238` |
| `crad_placeholder` | Pending NonRad MC; `PLACEHOLDER` tagged in headline | Cross-regime blocker |
| `option_h_mask` | Pending Pass-B per-component scan; `FLAT` tagged in headline | NAF rerun required |

### 1.5 Rigorization push: six v1 defects caught and their resolution status

| # | Defect caught during v1 → v2 push | Resolution status | Residual follow-up needed? |
|---:|---|---|---|
| 1 | `charge_flip` gave `A_h = 0` in v1 | **Resolved in v2** via analytic exact propagation `dA_h = 2 f |A_h|` | Optional data-driven cross-check only |
| 2 | `C_rad` contribution to `A_BC` was ~150× too small | **Resolved as placeholder correction in v2** (`~5%` level on `A_BC_*`) | **Yes** — still must replace placeholder with real per-bin `C_rad` |
| 3 | luminosity contribution to `A_BC` was silently zeroed | **Resolved in v2** through correlated e+/e− propagation | HB5 closure still needed for absolute-normalization confidence |
| 4 | model-dependence clamp recommendation was wrong | **Resolved in v2** by using central per-bin default, clamp only as alternate cross-check | No blocking follow-up |
| 5 | unfolding default was “drop unstable,” contrary to H1 practice | **Resolved in v2** by making keep-all the default | One critic review still required before Pass-B-rerun adoption |
| 6 | NuclIA `phi` prescription used proton **excess** vs **fraction** incorrectly | **Resolved in v2** | `phi_h` still needs full per-cell real systematics on r2 |

### 1.6 High-level state conclusion

The analysis is **not blocked by ignorance**. It is blocked by a finite set of execution-heavy items whose logic is already mostly understood:

1. migrate the final baseline from HQ-v5f-r1 to **HQ-v5f-r2**,
2. replace analytic detector-scale approximations with **Pass-B iSys reruns**,
3. replace the `C_rad` placeholder with a **real NonRad-derived numerator**,
4. complete **ep0304 recovery** so `A_h` and `A_BC` are not three-period provisional,
5. replace the flat Option H placeholder with a **per-component scan**, and
6. close **HB5** if absolute cross-sections are to remain headline.

That set of tasks defines the finalization program.

---

## 2. Top-level dependency graph

### 2.1 Text-format DAG

```text
F2-r2 frozen binning
    -> [1.1] Re-extract n_dis_eps_dis on HQ-v5f-r2
    -> [1.2] Reassemble M / sigma / A_h / A_BC on HQ-v5f-r2
    -> [1.3] Re-aggregate / replot r2 baseline
    -> [8.x] Internal-package assembly baseline

Pass-B detector iSys reproduction
    -> [2.1] Builder SetSysShift hook + catalog
    -> [2.2] Smoke tests per iSys
    -> [2.3] Full Condor iSys campaign
    -> [2.4] Per-iSys observable re-derivation
    -> [2.5] Replace analytic detector-scale sources in budget
    -> [8.x] Internal-package final systematic tables
    -> [publishable headline detector budget]

NonRad / C_rad program
    -> [3.1] Obtain DJANGOH14 NonRad production path
    -> [3.2] Build Strategy-2 RAD/NORAD ratio tables
    -> [3.3] RAPGAP31 model-spread cross-check
    -> [3.4] Beam-sign-separated C_rad tables for A_BC
    -> [8.x] Internal-package non-placeholder budget
    -> [publishable M / sigma / A_BC]

OQ7 e- recovery
    -> [4.1] Beam-energy policy decision for ep0304
    -> [4.2] Re-extract yields including ep0304
    -> [4.3] Re-derive A_h / A_BC with 4-period coverage
    -> [4.4] Per-period stability cross-check
    -> [8.x] Internal-package asymmetry package
    -> [publishable A_h / A_BC]

Option H scan
    -> [5.1] Per-component mask toggles
    -> [5.2] Per-bin shifts on M
    -> [5.3] Quadrature combine components
    -> [5.4] Replace flat 4% placeholder
    -> [8.x] Final systematic tables

HB5 normalization closure
    -> [6.0] Cross-regime coordination + closure-standard agreement
    -> [6.1] H1CalcWeight validation
    -> [6.2] Effective MC lumi calibration
    -> [6.3] Re-derive absolute sigma
    -> [8.x] Internal package can include validated sigma as headline
    -> [publishable absolute sigma]

phi_h on r2 (EXTENDED path only)
    -> [7.0] low-z target-fragmentation suitability gate
    -> [7.1] Re-extract phi yields on HQ-v5f-r2
    -> [7.2] Per-cell fit + fit-quality review
    -> [7.3] Replace bounded NuclIA / detector prescriptions with real per-cell numbers
    -> [8.x] Extended internal package
```

### 2.2 Parallelism and blocking structure

**Can run in parallel after the r2 baseline is re-established:**
- Phase 2 Pass-B detector iSys reproduction
- Phase 3 `C_rad` / NonRad program
- Phase 4 ep0304 recovery / OQ7 closeout
- Phase 5 Option H per-component scan
- Phase 6 HB5 normalization closure
- Phase 7 `phi_h` r2 extraction **only if** the supervisor chooses the EXTENDED path

**Must happen first because it defines the baseline grid:**
- **Phase 1** HQ-v5f-r2 multiplicity assembly + budget extension

**Best-coupled sequence rather than hard block:**
- Phase 7 (`phi_h` on r2) can begin once the new z-grid extraction is operational, but its final systematic replacement should wait for Phase 2 detector outputs and Phase 3 `C_rad` policy closure where relevant.
- Phase 8 internal package assembly should not begin serious memo drafting until CORE-path phases are complete or explicitly scope-cut. If the supervisor selects the EXTENDED path, Phase 7 must also be complete or explicitly demoted to control-only reporting.

### 2.3 Critical path

There are explicitly **two internal-package paths** plus the publication extension.

#### Internal-package CORE path (mandatory)

```text
Phase 1 (HQ-v5f-r2 baseline complete)
  -> Phase 2 (detector iSys replacement)
  -> Phase 3 (real C_rad)
  -> Phase 4 (ep0304 recovery)
  -> Phase 5 (Option H scan)
  -> Phase 8 (package assembly and critic sign-off)
```

HB5 is **not** on the CORE path if the internal package clearly separates:
- multiplicities and asymmetries as headline internal physics products, and
- absolute cross-sections as either provisional, demoted, or excluded.

#### Internal-package EXTENDED path (only if supervisor decides `phi_h` is headline)

```text
CORE path
  + Phase 7 (phi_h r2 + real systematics under the low-z exclusion rule)
  -> extended internal package
```

#### Critical path to publishable result

```text
CORE or EXTENDED internal-package-ready baseline
  + Phase 6 (HB5 normalization closure)
  + publication-level supervisor decisions from Section 5
  -> publishable result set
```

### 2.4 Calendar estimate (one analyst at half-FTE, occasional NAF jobs)

These are rough planning estimates, not promises.

- **Best case:** **12-14 weeks** to internal-package readiness on the **CORE** path.
- **Realistic case:** **16-20 weeks** to internal-package readiness on the **CORE** path.
- **Worst case:** **24-32 weeks** if Pass-B reveals a model defect requiring re-extraction or if the shared `C_rad` / NonRad path remains blocked; the latter can become an indefinite blocker.
- **EXTENDED-path add-on:** **+1-2 weeks** if the supervisor requires `phi_h` in the headline internal package.
- **Longest single critical-path item by wallclock risk:** **Phase 2 Pass-B detector iSys reproduction**, because it combines builder modifications, smoke testing, Condor queue latency, data-side reruns for electron shifts, and downstream reintegration.

---

## 3. Workstreams broken into phases with explicit action items

## Phase 1: HQ-v5f-r2 multiplicity assembly + budget extension to new bins

**Goal:** move the finalization baseline from HQ-v5f-r1 to HQ-v5f-r2 while preserving the rigorized v2 systematic framework.

### Phase 1 action items

**1.1** Re-extract `n_dis_eps_dis` on the HQ-v5f-r2 binning so the supervisor-approved 6-z-bin grid has a complete denominator/correction basis.  
- **Owner:** `h1-sidis-supervisor` coordinating `h1-production` + `h1-acceptance` style execution handoff  
- **Inputs required:**
  - `sidis_highQ2/docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md`
  - `sidis_highQ2/docs/data/binning_v5f_frozen.json`
  - `sidis_highQ2/docs/references/reference-highQ2-charged-sidis-analysis-cuts.md`
- **Outputs expected:**
  - `sidis_highQ2/docs/data/*_HQv5fr2.json` generation including the missing `n_dis_eps_dis`-dependent products
  - explicit note in the local results ledger that r2 denominator extraction is complete
- **Estimated wallclock:** 3-5 analyst hours + 4-10 NAF hours (rough)
- **Dependencies:** none beyond current frozen state

**1.2** Re-run multiplicity, absolute-σ assembly, `A_h`, and all `A_BC` variants on HQ-v5f-r2, keeping F14 blinding unchanged for the beam-charge observables.  
- **Owner:** `h1-sidis-supervisor` / downstream extraction owner  
- **Inputs required:**
  - r2 `n_dis_eps_dis` outputs from 1.1
  - current r2 data-yield and acceptance outputs already noted in state
  - frozen prescriptions F1, F3, F5, F6, F7, F8, F11, F13, F14, F15
- **Outputs expected:**
  - `sidis_highQ2/docs/data/multiplicity_v5f_data_HQv5fr2.json`
  - `sidis_highQ2/docs/data/cross_section_v5f_data_HQv5fr2.json`
  - `sidis_highQ2/docs/data/asymmetry_v5f_data_HQv5fr2.json`
- **Estimated wallclock:** 2-4 analyst hours + 2-6 NAF hours
- **Dependencies:** 1.1

**1.3** Recompute the local systematic aggregations and all main stack/summary plots on HQ-v5f-r2, preserving v2 source semantics and carrying explicit caveats for the new low-z and high-z bins.  
- **Owner:** `h1-systematics` style local execution handoff under supervisor control  
- **Inputs required:**
  - `sidis_highQ2/docs/data/systematics_v5f_aggregated_v2.json` as semantic template
  - r2 observable JSONs from 1.2
  - existing per-source v2 JSONs, repointed or regenerated to `_HQv5fr2` where needed
- **Outputs expected:**
  - `sidis_highQ2/docs/data/systematics_v5f_aggregated_v2_HQv5fr2.json` or equivalent r2-named successor
  - `sidis_highQ2/plots/systematics_v5f/stack_v2_r2/` or equivalent updated plot set
- **Estimated wallclock:** 4-6 analyst hours
- **Dependencies:** 1.2

**1.3a** Update the full HQ-v5f-r2 script touch list rather than treating Phase 1 as a two-script patch.  
- **NAF rerun scripts with `Z_EDGES` change required:**
  - `scripts/extract_yields_v5f_3period.py`
  - `scripts/compute_data_hadron_yields_v5f.py`
  - `scripts/compute_asymmetries_v5f.py`
  - `scripts/extract_phi_yields_v5f_3period.py`
  - `scripts/compare_kinematic_methods.py`
  - the `n_dis_eps_dis` extractor(s): confirm the exact filename on NAF before execution and add it to the tracked rerun set in the execution brief
- **Local rerun scripts after NAF outputs land, with `Z_EDGES` change required:**
  - `scripts/plot_systematics_v5f_model_dependence.py`
  - `scripts/plot_systematics_v5f_nuclia_proton_omission.py`
  - `scripts/plot_systematics_v5f_stack.py`
  - `scripts/aggregate_systematics_v5f.py`
- **Planning note:** all four local scripts above already carry `NOTE` comments directly above their `Z_EDGES` line in this workspace, warning that the current values are still HQ-v5f-r1 and must be updated when the HQ-v5f-r2 outputs are used.
- **Dependencies:** 1.1 and the exact `n_dis_eps_dis` executable filename confirmation during execution handoff

**1.4** Introduce and populate per-bin stability tagging for all HQ-v5f-r2 bins, with special attention to the new `z in [0.0,0.1]` and `z in [0.7,1.0]` slices required by the F2-r2 amendment conditions.  
- **Owner:** `h1-systematics` with supervisor sign-off  
- **Inputs required:**
  - `sidis_highQ2/docs/handoffs/decision-binning-amendment-HQv5fr2.md`
  - r2 unfolding comparison outputs
  - r2 aggregated systematic outputs from 1.3
- **Outputs expected:**
  - per-bin `bin_stability` tags in the relevant JSONs or sidecar status table
  - updated plotting caveats and table footnotes for unstable bins
- **Estimated wallclock:** 2-3 analyst hours
- **Dependencies:** 1.3

**1.5** Produce a short r2 migration checkpoint memo confirming that HQ-v5f-r2, not HQ-v5f-r1, is now the live finalization baseline for every downstream phase.  
- **Owner:** supervisor  
- **Inputs required:** outputs from 1.1-1.4  
- **Outputs expected:** plan-consistency note embedded in later package memo inputs  
- **Estimated wallclock:** 1 analyst hour  
- **Dependencies:** 1.4

### Critical critic interjection points for Phase 1

- **CRITIC GATE 1.1:** before accepting the new `z in [0.0,0.1]` and `z in [0.7,1.0]` bins into the headline r2 baseline, dispatch `h1-analysis-critic` to verify that the physics caveats attached in `docs/handoffs/decision-binning-amendment-HQv5fr2.md` are still the right ones after full-stat extraction.
- **CRITIC GATE 1.2:** before freezing r2 multiplicities, dispatch `h1-analysis-critic` to verify that the low-z bin does not show a pathology inconsistent with the expected soft/target-fragmentation caveat.
- **CRITIC GATE 1.3:** before using the `z in [0.7,1.0]` bin in any headline multi-panel summary, dispatch `h1-analysis-critic` to check whether exclusive VMD / diffractive contamination appears larger than the current placeholder caveat can honestly support.
- **CRITIC GATE 1.4:** before locking the r2 budget, dispatch a critic review of the new per-bin stability tags to confirm that the “unstable” classification is being applied consistently and not only to the two new z bins.

### Phase 1 code-change anchor snippets

These snippets are included as implementation anchors only; this plan does not execute them.

```python
# scripts/extract_yields_v5f_3period.py
Z_EDGES = [0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]
TAG = "HQv5fr2"
```

```python
# scripts/assemble_multiplicity_cross_section_v5f.py
with open("docs/data/multiplicity_v5f_data_HQv5fr2.json", "w") as f:
    json.dump(payload, f, indent=2, sort_keys=True)
```

```json
{
  "scheme": "HQ-v5f-r2",
  "z_edges": [0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0],
  "bin_stability": "unstable"
}
```

### Phase 1 considerations and trade-offs

- The r2 migration is conceptually simple but operationally non-optional: continuing to treat r1 as the live baseline would undermine the supervisor-approved F2-r2 decision.
- The new edge bins are scientifically useful but will almost certainly need stronger caveat language than the middle bins.
- It is acceptable for Phase 1 to finish while HB5, OQ7, and real `C_rad` remain open; it is **not** acceptable for Phase 8 internal-package assembly to use r1 as the nominal final grid.

---

## Phase 2: Pass-B iSys reproduction for detector-scale systematics

**Goal:** replace the four current analytic detector-scale approximations with proper iSys-loop-derived per-bin numbers.

### Phase 2 action items

**2.1** Add a `SetSysShift(int iSys)` hook to the authoritative NAF `ChargedSidisTreeBuilderHighQ2` source with the exact detector-systematics catalog required for the Pass-B campaign.  
- **Owner:** `code-editor` under supervisor delegation; methodology checked by `h1-software` / `h1-analysis-critic` as needed  
- **Inputs required:**
  - **local source cache (read-only fallback reference):** `sidis_highQ2/source_cache/ProtonTreeBuilderHighQ2.C`
  - **NAF authoritative source:** `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2.C`
  - `sidis_highQ2/docs/handoffs/handoff-HQv5fr2-rebuild-on-naf.md`
  - this plan’s frozen iSys catalog
  - reference implementation pattern from g1j_ES: `/data/dust/group/h1/klesthen/g1j_ES/AnalysisRuns/23-03-19-21-20-FullSysUncFullCuts/NewAnaBase2-7-23.C` (around line 88 onward)
- **Outputs expected:** builder support for:
  - `±7` = electron Escale ±0.5% per wheel
  - `+10` = electron theta +1 mrad
  - `+2/-2` = HFS Escale ±1%
  - `+3` = HFS theta +1 mrad
- **Planning boundary:** the `SetSysShift(int iSys)` hook is added to the **NAF source**, not to the local `source_cache/` snapshot. The local cache remains unchanged during this phase unless a later documentation-only refresh is explicitly requested.
- **Estimated wallclock:** 4-8 analyst hours + compilation/smoke time
- **Dependencies:** Phase 1 baseline should be known, but code work can start in parallel after 1.1

**2.2** Execute one-file smoke tests for each iSys variation and verify that the shifted trees populate as expected without schema drift or null payloads.  
- **Owner:** `h1-production` / supervisor execution  
- **Inputs required:** modified NAF builder from 2.1, one representative file per relevant data/MC stream  
- **Outputs expected:** smoke-test logs and minimal validation tables per iSys  
- **Estimated wallclock:** 3-5 analyst hours + 4-8 NAF hours  
- **Dependencies:** 2.1

**2.3** Launch the full Condor iSys production campaign for MC and the required data-side reruns for the electron-side shifts.  
- **Owner:** `h1-production`  
- **Inputs required:** validated 2.2 smoke tests, period manifests, frozen steering  
- **Outputs expected:** per-iSys ROOT outputs for all planned shifts  
- **Dependencies:** 2.2

#### 2.3a Explicit iSys rerun matrix

| iSys code | Shift | MC rerun? | Data rerun? | Reason |
|---|---|---:|---:|---|
| `-9999` | nominal | reference | reference | already in production |
| `+7 / -7` | LAr Escale ±0.5% per wheel | YES | **YES** | electron-side scale affects data event reconstruction |
| `+10` | LAr theta +1 mrad | YES | **YES** | electron-side angle affects data event reconstruction |
| `+2 / -2` | HFS Escale ±1% | YES | **NO** | HFS scale calibrated on data → MC-only rerun (standard H1 practice) |
| `+3` | HFS theta +1 mrad | YES | **NO** | same |

**Explicit requirement:** the data-side rerun for `±7` and `+10` is **required**, not optional, because those electron-side shifts change reconstructed event kinematics and selection on data as well as MC. The HFS-side shifts remain MC-only under standard H1 treatment because the hadronic scale is calibrated on data.

#### 2.3b Condor cost split

- **MC-side campaign cost:** approximately **12 days** wallclock for **6 iSys shifts × 2 generators × 4 periods**.
- **Data-side campaign cost:** approximately **3-4 days** wallclock for **3 iSys states (`+7`, `-7`, `+10`) × 4 periods** over the **973 data files** already inventoried.
- **Planning implication:** the earlier single blended estimate understated the need to budget an explicit data-side rerun stream for the electron shifts.

**2.4** Re-derive the relevant observables per iSys variation, including acceptance / response-sensitive quantities, using the same HQ-v5f-r2 binning and the current nominal prescriptions.  
- **Owner:** supervisor-coordinated downstream extractor chain  
- **Inputs required:** iSys outputs from 2.3, r2 nominal extraction scripts, frozen cuts reference  
- **Outputs expected:** per-iSys observable JSONs, nominal-minus-shift delta tables  
- **Estimated wallclock:** 5-8 analyst hours + NAF/local processing time  
- **Dependencies:** 2.3

**2.5** Replace the current analytic estimates for `lar_electron_escale`, `lar_electron_angle`, `hfs_hadronic_scale`, and the detector-scale component of the Pass-B program with the new iSys-derived per-bin numbers in the aggregated budget.  
- **Owner:** `h1-systematics` under supervisor sign-off  
- **Inputs required:** 2.4 delta tables, current v2 source JSONs, current r2 aggregated budget  
- **Outputs expected:**
  - new per-source JSONs, preferably v3-suffixed or Pass-B-tagged
  - updated aggregated budget with analytic detector sources retired
- **Estimated wallclock:** 4-6 analyst hours
- **Dependencies:** 2.4

**2.6** Perform a before/after comparison of the analytic-v2 and iSys-derived detector systematics, documenting bins where the ratio exceeds the expected factor-2 agreement band.  
- **Owner:** supervisor + `h1-analysis-critic`  
- **Inputs required:** old v2 detector-source JSONs and new iSys-derived replacements  
- **Outputs expected:** a comparison memo or package appendix table  
- **Estimated wallclock:** 2-4 analyst hours  
- **Dependencies:** 2.5

### Critical critic interjection points for Phase 2

- **CRITIC GATE 2.1:** before launching the full Condor campaign, dispatch `implementation-critic` or equivalent code reviewer to verify the `SetSysShift(int iSys)` catalog exactly matches the planned H1 systematic semantics.
- **CRITIC GATE 2.2:** after smoke tests and before full submission, dispatch `h1-analysis-critic` to confirm the observed `Q²` / `y` / `E-pz` shifts are of the expected order for each iSys.
- **CRITIC GATE 2.3:** before adopting the iSys-derived numbers as nominal, dispatch `h1-analysis-critic` to compare them against the v2 analytic approximations and flag any source/bin combination that differs by more than roughly a factor of two without an understood physics reason.
- **CRITIC GATE 2.3b:** if any populated `(source × bin)` shows Pass-B vs analytic-v2 disagreement **>3×** without an understood physics reason, freeze adoption for that source and require an explicit discrepancy-resolution memo before it can re-enter the headline budget.
- **CRITIC GATE 2.4:** before finalizing the unfolding-sensitive detector-source replacements, dispatch a review of the upstream G18-style SVD-k regularization assumptions to ensure the detector reruns are not being combined with a stale regularization choice.

#### 2.3c Failure branch for >3× disagreement

If any populated `(source × bin)` shows Pass-B vs analytic-v2 disagreement **>3×** without an understood physics reason:

1. **Phase 2 fails closed for that source.**
2. The implementer reruns smoke tests on a **different MC file** to rule out a statistical fluke.
3. If the disagreement persists, the supervisor + `h1-analysis-critic` produce a **discrepancy memo** documenting the affected source, bins, checks performed, and proposed resolution.
4. **Phase 8 may proceed with the other detector-scale sources**, but the affected source is excluded from the headline budget until resolved.
5. **CG-2.3b** is the mandatory documentation gate for that resolution path.

### Phase 2 code-change anchor snippets

These are **conceptual patterns to be implemented on NAF**, not file-verified local edits. The local cache is only a read-only fallback reference.

```cpp
// CONCEPTUAL pattern for the NAF file:
// /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2.C
class ChargedSidisTreeBuilderHighQ2 {
public:
  void SetSysShift(int iSys);
private:
  int fSysShift = 0;
};
```

```cpp
// CONCEPTUAL pattern based on the g1j_ES SetSysShift style
void ChargedSidisTreeBuilderHighQ2::SetSysShift(int iSys) {
  fSysShift = iSys;
  m_h1CalcSystematic->SetSysShift(iSys);
}
```

```cpp
// planned iSys dispatch anchors
// +7 / -7 : H1CalcSystematic::eElecEn
// +10     : H1CalcSystematic::eElecTh
// +2 / -2 : H1BoostedJets::SysShiftHFSEnergy
// +3      : H1BoostedJets::SysShiftHFSTheta
```

### Phase 2 considerations and trade-offs

- This is the heaviest operational phase and the most likely source of queue-driven delay.
- The campaign is expensive enough that smoke-test quality matters more than usual; a bad iSys launch would waste weeks.
- The data-side rerun for `±7` and `+10` is a real schedule component and must be treated as mandatory, not implied.
- If the iSys-derived numbers agree reasonably with the analytic v2 approximations, the rigorization push will have been validated rather than overturned; if they do not, the plan already expects one rework cycle and now defines an explicit >3× failure branch.

---

## Phase 3: C_rad real numerator

**Goal:** eliminate `crad_placeholder` by deriving real per-bin radiative-correction numerators, including beam-sign dependence for `A_BC`.

### Phase 3 action items

**3.1** Secure a workable DJANGOH14 NonRad MC path for charged SIDIS at high Q², coordinating with the low-Q² supervisor if a shared production is possible.  
- **Owner:** supervisor coordinating with `h1-production` and, if needed, another supervisor  
- **Inputs required:** current blocker note from `context-handoff.md`, r5 `G24` strategy reference, any shared NonRad inventory notes  
- **Outputs expected:** explicit production decision: in-house build vs reuse-with-reweighting pathway  
- **Estimated wallclock:** 3-6 analyst hours + external coordination latency  
- **Dependencies:** none

**3.2** Produce Strategy-2 RAD/NORAD ratio tables per bin on the HQ-v5f-r2 grid as the nominal `C_rad` implementation path unless the supervisor explicitly overturns the strategy.  
- **Owner:** `h1-radiative-correction` style execution under supervisor delegation  
- **Inputs required:** RAD and NonRad generator-level trees, binning JSON, cuts reference  
- **Outputs expected:**
  - `sidis_highQ2/docs/data/crad_v*_HQv5fr2.json`
  - explicit per-bin `C_rad` values for multiplicity / sigma use
- **Estimated wallclock:** 4-8 analyst hours + 1-3 days NAF wallclock depending on production readiness
- **Dependencies:** 3.1

**3.3** Run a RAPGAP31-based cross-check to estimate the model spread of the `C_rad` correction itself.  
- **Owner:** `h1-radiative-correction` + `h1-analysis-critic`  
- **Inputs required:** 3.2 nominal tables and RAPGAP31 comparison products  
- **Outputs expected:** per-bin model-spread comparison and acceptance recommendation  
- **Estimated wallclock:** 2-4 analyst hours + NAF processing time  
- **Dependencies:** 3.2

**3.4** Derive beam-sign-separated `C_rad` tables for `A_BC^+`, `A_BC^-`, `A_BC^{sum}`, and `A_BC^{DIS}` rather than assuming cancellation.  
- **Owner:** `h1-radiative-correction` / asymmetry chain owner  
- **Inputs required:** 3.2 nominal `C_rad`, period-by-beam bookkeeping, asymmetry definitions  
- **Outputs expected:** beam-sign-aware `C_rad` lookup tables or sidecar JSONs for asymmetry assembly  
- **Estimated wallclock:** 3-5 analyst hours  
- **Dependencies:** 3.2

**3.5** Replace the `crad_placeholder` source in the aggregated budget and regenerate all affected observable tables and plots.  
- **Owner:** `h1-systematics` under supervisor sign-off  
- **Inputs required:** 3.2-3.4 outputs, current aggregated budget  
- **Outputs expected:** updated per-source JSON and new aggregated totals with no `placeholder_pending_*` for `C_rad`  
- **Estimated wallclock:** 2-4 analyst hours  
- **Dependencies:** 3.4

### Critical critic interjection points for Phase 3

- **CRITIC GATE 3.1:** before launching large NonRad production or adopting reused samples, dispatch `h1-analysis-critic` to verify the pre-launch smoke test confirms the NonRad configuration is physically the intended one.
- **CRITIC GATE 3.2:** before replacing the placeholder, dispatch `h1-analysis-critic` to verify that nearly all `C_rad` values are `O(1)` and that any outliers beyond ~20% are explained bin-by-bin.
- **CRITIC GATE 3.3:** before freezing `A_BC` budgets, dispatch `h1-analysis-critic` to inspect the beam-sign-separated `C_rad` tables and confirm that the observed e+/e− differences are physically plausible rather than an indexing or period-mapping bug.
- **CRITIC GATE 3.4:** before budget sign-off, dispatch critic review of RAPGAP-vs-DJANGOH `C_rad` model spread to decide whether the spread becomes its own explicit line item or remains a validation cross-check.

### Phase 3 code-change anchor snippets

```python
# conceptual r5-G24-style ratio assembly
c_rad = n_rad_gen / n_norad_gen
payload = {
    "scheme": "HQ-v5f-r2",
    "source": "DJANGOH14_RAD_over_NonRad",
    "values": c_rad.tolist(),
}
```

```json
{
  "source_id": "crad_real",
  "supersedes": "crad_placeholder",
  "beam_sign_split": true,
  "scheme": "HQ-v5f-r2"
}
```

### Phase 3 considerations and trade-offs

- This is the cleanest way to retire the single most awkward placeholder in the budget.
- If NonRad production proves unavailable, the analysis may still reach an internal package, but not the “fully finalized” state defined in Section 7.
- Beam-sign-separated handling is mandatory because the rigorization push already established that the old cancellation assumption was wrong.

---

## Phase 4: e- period recovery (OQ7 unblocking)

**Goal:** recover the missing `ep0304` period and move `A_h` / `A_BC` from a three-period provisional basis to a four-period publication-grade basis.

### Phase 4 action items

**4.1** Resolve the beam-energy policy for `ep0304`, explicitly deciding how to treat its `27.5 GeV` beam energy versus the `27.6 GeV` value in the later periods under frozen F13 per-run lookup semantics.  
- **Owner:** supervisor, with factual support from existing beam-energy reference docs  
- **Inputs required:**
  - `sidis_highQ2/docs/references/reference-highQ2-per-run-beam-energy.md` if present in workspace
  - F13 policy references in `context-handoff.md`
  - current period manifests
- **Outputs expected:** explicit sign-off memo or plan note defining the `ep0304` policy
- **Estimated wallclock:** 2-4 analyst hours
- **Dependencies:** none

**4.2** Modify or replace the extractor logic that currently excludes `ep0304`, then re-extract data yields with full four-period coverage using the per-run beam-energy lookup.  
- **Owner:** `code-editor` for extractor change, `h1-production` / supervisor for execution  
- **Inputs required:** existing extractor scripts, period manifests, F13 table, cuts reference  
- **Outputs expected:** four-period yield JSONs including `ep0304`  
- **Estimated wallclock:** 4-8 analyst hours + 1-2 days NAF wallclock  
- **Dependencies:** 4.1

**4.3** Re-derive `A_h`, `A_BC^+`, `A_BC^-`, `A_BC^{sum}`, and `A_BC^{DIS}` on the full four-period basis, maintaining F14 blinding for beam-charge observables until package sign-off.  
- **Owner:** asymmetry-chain owner under supervisor control  
- **Inputs required:** four-period data yields from 4.2, current acceptance/correction basis, current asymmetry assembly scripts  
- **Outputs expected:** updated asymmetry JSONs and comparison tables versus the current three-period baseline  
- **Estimated wallclock:** 2-4 analyst hours + compute time  
- **Dependencies:** 4.2

**4.4** Perform a per-period stability and yield-per-lumi consistency study to test the cancellation argument and surface any hidden period-specific pathology.  
- **Owner:** `h1-analysis-critic` + supervisor  
- **Inputs required:** 4.2-4.3 outputs, period luminosity tables  
- **Outputs expected:** period-stability note for inclusion in the internal-package memo  
- **Estimated wallclock:** 3-5 analyst hours  
- **Dependencies:** 4.3

### Critical critic interjection points for Phase 4

- **CRITIC GATE 4.1:** before any large re-extraction, dispatch `h1-analysis-critic` to verify that the adopted `ep0304` beam-energy policy is consistent with F13 and does not silently introduce a mixed-energy normalization bug.
- **CRITIC GATE 4.2:** before accepting four-period asymmetries as nominal, dispatch `h1-analysis-critic` to review per-period yield-per-lumi stability and confirm there is no unexplained period asymmetry bigger than the intended cancellation logic can tolerate.
- **CRITIC GATE 4.3:** before lifting the “publication blocked” label on `A_h`, dispatch `h1-analysis-critic` to compare the three-period and four-period `A_h` surfaces and ensure the change is qualitatively consistent with the expected ep0304 contribution rather than a bookkeeping defect.

### Phase 4 code-change anchor snippets

```python
# scripts/extract_yields_v5f_3period.py -> future 4-period generalization
PERIODS = ["ep0304", "em0405", "em06", "ep0607"]
beam_energy = beam_energy_by_run[str(run_number)]
```

```python
if period == "ep0304":
    # use per-run lookup, not a hard-coded later-period default
    e_beam = beam_energy_lookup[run_number]
```

### Phase 4 considerations and trade-offs

- This phase is conceptually narrower than Phase 2 or 3, but it directly controls whether the asymmetry program can be treated as publication-grade.
- Because `A_BC` is blinded under F14, the process discipline here matters as much as the final numbers.
- A negative outcome is possible: period recovery may reveal a genuine period-dependent systematic that weakens the cancellation story. The plan therefore includes explicit critic review rather than assuming recovery will be benign.

---

## Phase 5: Option H mask per-component scan (NAF)

**Goal:** replace the current flat 4% placeholder-style Option H mask treatment with a per-component, NAF-backed scan.

### Phase 5 action items

**5.1** Define the per-track mask toggle campaign on v5f MC by independently switching off each Option H component: `V0Daughter`, `ConvOrDalitz`, `Electron`, `Muon`, and `ParticleSource==4`.  
- **Owner:** supervisor + `h1-production`  
- **Inputs required:** F11 mask definition in `reference-highQ2-charged-sidis-analysis-cuts.md`, current Option H v2 provenance note  
- **Outputs expected:** formal scan matrix and steering list  
- **Estimated wallclock:** 2-3 analyst hours  
- **Dependencies:** Phase 1 baseline should exist; can otherwise run in parallel

**5.2** Execute the MC scan and derive per-bin multiplicity shifts for each mask component independently.  
- **Owner:** `h1-production` / extraction chain owner  
- **Inputs required:** 5.1 steering, nominal MC products, HQ-v5f-r2 binning  
- **Outputs expected:** component-by-component shift tables on HQ-v5f-r2  
- **Estimated wallclock:** 3-5 analyst hours + 1-3 days NAF wallclock  
- **Dependencies:** 5.1

**5.3** Quadrature-combine the per-component mask shifts into an updated Option H systematic source and compare it against the current flat 4% placeholder.  
- **Owner:** `h1-systematics`  
- **Inputs required:** 5.2 output tables, current `option_h_mask` v2 source  
- **Outputs expected:** `option_h_mask_v3`-style per-bin source JSON and comparison summary  
- **Estimated wallclock:** 2-4 analyst hours  
- **Dependencies:** 5.2

**5.4** Replace the current Option H source in the aggregated budget and propagate the update through the package tables and plots.  
- **Owner:** `h1-systematics` / supervisor  
- **Inputs required:** 5.3 outputs, current aggregated budget  
- **Outputs expected:** updated aggregate with no Option H placeholder semantics  
- **Estimated wallclock:** 1-2 analyst hours  
- **Dependencies:** 5.3

### Critical critic interjection points for Phase 5

- **CRITIC GATE 5.1:** before adopting the per-component scan as nominal, dispatch `h1-analysis-critic` to verify each individual component shift is physically reasonable and below the rough <5% expectation.
- **CRITIC GATE 5.2:** before replacing the current 4% placeholder, dispatch `h1-analysis-critic` to compare the quadrature-combined result against the current flat value and explain any large discrepancy.

### Phase 5 code-change anchor snippets

```python
OPTION_H_COMPONENTS = [
    "IsMatchedV0Daughter",
    "IsMatchedConvOrDalitz",
    "IsMatchedElectron",
    "IsMatchedMuon",
    "ParticleSourceEq4",
]
```

```python
shift_total = np.sqrt(np.sum(np.square(component_shifts), axis=0))
```

### Phase 5 considerations and trade-offs

- This source is not the dominant budget term, but leaving it placeholder-like would keep the analysis from satisfying the Section 7 finalization criteria.
- The per-component scan is relatively cheap compared with Phase 2 and 3, so it should not be postponed once NAF cycles are available.

---

## Phase 6: Absolute cross-section unblocking (HB5)

**Goal:** either validate absolute normalization end to end and keep `σ` headline, or explicitly demote absolute cross-sections from the headline package.

### Shared-infrastructure note before Phase 6 launch

HB5 validation is **shared infrastructure work**, not purely local SIDIS bookkeeping. `H1CalcWeight` / effective MC luminosity validation may also block `proton_HiQ`, `lambda_HiQ`, and potentially other high-`Q²` consumers. Before Phase 6 begins locally in this analysis, the consuming analyses should align on:

- what exact closure tests count as “validated,”
- whether period-by-period and generator-by-generator agreement thresholds are common across analyses,
- how any residual normalization differences are documented, and
- whether one shared memo or parallel analysis-specific memos will serve as the closure record.

A **cross-regime coordination meeting is recommended before Phase 6 launch** so the analyses do not drift into incompatible normalization conventions.

### Phase 6 action items

**6.1** Perform the H1CalcWeight validation needed to close HB5, including period-by-period closure checks against the effective MC luminosity model.  
- **Owner:** supervisor coordinating with `h1-normalization` / relevant software specialist  
- **Inputs required:**
  - current H1CalcWeight handling in the high-Q² chain
  - period luminosity tables
  - MC luminosity bookkeeping references
- **Outputs expected:** closure tables and a yes/no HB5 recommendation
- **Estimated wallclock:** 5-8 analyst hours + possible multi-day validation latency
- **Dependencies:** none formally, though r2 nominal products make comparisons cleaner

**6.2** Calibrate effective MC luminosity consistently across generators and periods, documenting any residual generator dependence.  
- **Owner:** `h1-normalization` style execution under supervisor control  
- **Inputs required:** current MC lumi book, H1CalcWeight outputs, generator manifests  
- **Outputs expected:** updated effective-lumi calibration table / JSON  
- **Estimated wallclock:** 3-5 analyst hours  
- **Dependencies:** 6.1

**6.3** Re-derive absolute `σ^{h±}` on HQ-v5f-r2 using the validated normalization and retire the old `data_side_option_H_caveat` if H1CalcWeight closure justifies it.  
- **Owner:** cross-section chain owner  
- **Inputs required:** 6.2 calibration, r2 multiplicity/cross-section chain, current caveat registry  
- **Outputs expected:** validated `cross_section_v5f_data_HQv5fr2.json` and updated plots/tables  
- **Estimated wallclock:** 2-4 analyst hours  
- **Dependencies:** 6.2

**6.4** If HB5 does not close cleanly, produce an explicit scope decision record stating that absolute cross-sections are excluded from the headline finalized package while multiplicities and asymmetries proceed.  
- **Owner:** supervisor  
- **Inputs required:** 6.1-6.3 closure evidence  
- **Outputs expected:** explicit exclusion record if needed  
- **Estimated wallclock:** 1 analyst hour  
- **Dependencies:** 6.3

### Critical critic interjection points for Phase 6

- **CRITIC GATE 6.0:** before Phase 6 launches locally, dispatch `h1-software` to confirm that the cross-regime HB5 coordination has happened and that a common closure standard has been agreed among the high-`Q²` consumers.
- **CRITIC GATE 6.1:** before claiming HB5 closure, dispatch `h1-analysis-critic` to review per-period H1CalcWeight closure and confirm that no hidden beam-sign or period-dependent bias remains.
- **CRITIC GATE 6.2:** before labeling absolute `σ` publishable, dispatch `h1-analysis-critic` to compare the integrated high-Q² normalization against published H1 inclusive DIS cross-sections or equivalent normalization anchors.

### Phase 6 code-change anchor snippets

```python
sigma = n_h_corr / (luminosity_data * acceptance * bin_width_q2 * bin_width_x * bin_width_z * bin_width_pth)
```

```json
{
  "HB5_status": "validated",
  "effective_mc_luminosity_source": "docs/data/highQ2_mc_luminosity.json"
}
```

### Phase 6 considerations and trade-offs

- This is the one phase that can still end with an explicit scope cut rather than a technical success.
- If HB5 remains open, the rest of the analysis can still be scientifically strong; the package just cannot honestly sell absolute cross-sections as headline results.
- Because HB5 is shared infrastructure, an analysis-local “closure” that uses a private standard is not acceptable.

---

## Phase 7: φ_h moments full extraction on HQ-v5f-r2 (EXTENDED path only)

**Goal:** migrate the `phi_h` program onto HQ-v5f-r2 and replace the remaining bounded / approximate systematic prescriptions with real per-cell numbers.

### Phase 7 precondition and decision rule

- **Default rule:** exclude the new low-z r2 bin `z ∈ [0.0, 0.1]` from **headline** `phi_h` fits. This bin is expected to be target-fragmentation-dominated and does not fit the standard SIDIS `φ_h` decomposition cleanly. It is still computed, but it must be flagged `phi_h_unsuited_target_fragmentation_dominant` and reported only as a control / cross-check point, never as a TMD-relevant headline point.
- **Alternative path (non-default):** if the supervisor explicitly approves inclusion of `z ∈ [0.0,0.1]`, it may enter the `phi_h` package only with an additional target-fragmentation systematic treated as a **14th-source-style bound** whose value is set by a dedicated study. This is documented as a possible extension, but is **not** the default plan.
- **Practical consequence:** headline `phi_h` reporting covers the **four central z bins** in `z ∈ [0.1, 0.7]`; the r2 extension bins are control-only unless explicitly re-approved.

### Phase 7 action items

**7.1** Re-extract `phi_h` yields on HQ-v5f-r2 so the new low-z and high-z bins are present in the azimuthal-moment analysis, even though the low-z bin is default-excluded from headline fits.  
- **Owner:** `h1-production` / `phi_h` extraction owner  
- **Inputs required:** updated z-grid extraction scripts, r2 event/yield basis, frozen F15 convention  
- **Outputs expected:** r2 `phi_h` yield JSONs per cell, including explicit suitability flags per z slice  
- **Estimated wallclock:** 3-5 analyst hours + NAF processing time  
- **Dependencies:** Phase 1 extraction machinery stable

**7.2** Refit each populated headline-eligible `phi_h` cell for `A1` and `A2`, storing statistical uncertainty and `χ²/dof` metrics explicitly. Keep the low-z control bin computed but out of the headline fit set unless the alternative path is invoked.  
- **Owner:** `h1-peak-fitter`-like fitting executor or local analysis chain owner  
- **Inputs required:** 7.1 yields, current fit model, F15 convention references  
- **Outputs expected:**
  - `phi_h_moments_v5f_data_HQv5fr2.json`
  - fit-quality summary table by cell
- **Estimated wallclock:** 4-6 analyst hours
- **Dependencies:** 7.1

**7.3** Replace the current bounded prescriptions for NuclIA and detector-systematic contributions to `phi_h` with real per-cell numbers propagated from the updated detector and closure programs.  
- **Owner:** `h1-systematics` + `phi_h` chain owner  
- **Inputs required:** Phase 2 detector outputs, current `phi_h` systematic basis, NuclIA source updates  
- **Outputs expected:** updated `phi_h` systematic JSONs on r2  
- **Estimated wallclock:** 3-5 analyst hours  
- **Dependencies:** 2.5 and 7.2

**7.4** Validate angular acceptance correction and `phi_h` resolution assumptions for the r2 package and regenerate the headline `phi_h` plots.  
- **Owner:** `h1-analysis-critic` + plotting chain owner  
- **Inputs required:** 7.2-7.3 outputs, prior resolution references  
- **Outputs expected:** plot set and acceptance-validation note  
- **Estimated wallclock:** 2-4 analyst hours  
- **Dependencies:** 7.3

### Critical critic interjection points for Phase 7

- **CRITIC GATE 7.0:** **before** `phi_h` extraction begins, dispatch `h1-analysis-critic` to confirm that the default exclusion of `z ∈ [0.0,0.1]` from headline `phi_h` fits is correct and to document the target-fragmentation rationale in the package notes.
- **CRITIC GATE 7.1:** before freezing `phi_h` results on r2, dispatch `h1-analysis-critic` to check per-cell fit quality, targeting `χ²/dof < 5` in >80% of cells with `N_phi >= 6`.
- **CRITIC GATE 7.2:** before adopting the updated `phi_h` detector uncertainties, dispatch `h1-analysis-critic` to review whether the `phi_h` resolution assumptions from the earlier G18-style work remain valid on the new z-edge bins.
- **CRITIC GATE 7.3:** before package sign-off, dispatch `h1-analysis-critic` to verify that the angular acceptance correction treatment is adequate and that no hidden Trento-convention regression appeared during the r2 migration.

### Phase 7 code-change anchor snippets

```python
fit_result = model.fit(y, x=phi_centers)
payload[cell_key] = {
    "A1": float(fit_result.params["A1"].value),
    "A2": float(fit_result.params["A2"].value),
    "chi2_ndf": float(chi2 / ndf),
    "suitability_flag": "phi_h_unsuited_target_fragmentation_dominant" if z_low == 0.0 and z_high == 0.1 else "headline_eligible",
}
```

```json
{
  "observable": "phi_A1",
  "scheme": "HQ-v5f-r2",
  "trento_convention": true,
  "headline_z_range": [0.1, 0.7]
}
```

### Phase 7 considerations and trade-offs

- `phi_h` is scientifically valuable, but it is no longer assumed to be mandatory for the CORE internal package.
- The low-z r2 extension bin is useful as a control point, but by default it is **not** treated as a standard current-fragmentation TMD result.
- The new low-z and high-z bins are exactly where angular acceptance and resolution assumptions are most likely to weaken, so this phase needs genuine critic attention.

---

## Phase 8: Internal results package assembly (old r5 G32 target)

**Goal:** assemble the fully reviewed internal package that becomes the analysis’ package-ready source of truth.

### Phase 8 action items

**8.1** Consolidate all per-observable JSONs and plots onto a single HQ-v5f-r2 package baseline, with version-locked references to the adopted systematic-source files.  
- **Owner:** supervisor  
- **Inputs required:** outputs from Phases 1-5 and, if applicable, 6-7  
- **Outputs expected:** curated package inventory list  
- **Estimated wallclock:** 3-4 analyst hours  
- **Dependencies:** CORE-path phases complete; EXTENDED-path products included only if selected

**8.2** Build per-observable per-bin result tables containing central values, statistical uncertainties, total systematic uncertainties, and per-source breakdowns.  
- **Owner:** `h1-systematics` / supervisor  
- **Inputs required:** final observable JSONs and final source JSONs  
- **Outputs expected:** machine-readable and memo-ready tables for `M^{h±}`, `A_h`, `A_BC^{+/-/sum/DIS}`, `phi_A1`, `phi_A2`, and `σ^{h±}` if HB5 is closed  
- **Estimated wallclock:** 4-6 analyst hours  
- **Dependencies:** 8.1

**8.3** Build the cancellation-matrix display showing which sources cancel, partially cancel, or do not cancel in each asymmetry observable.  
- **Owner:** supervisor + `h1-systematics`  
- **Inputs required:** final systematic source definitions, asymmetry formulas, rigorization notes  
- **Outputs expected:** package figure or appendix table  
- **Estimated wallclock:** 2-3 analyst hours  
- **Dependencies:** 8.2

**8.4** Write the internal-package memo summarizing methods, frozen decisions, observable definitions, caveats, blocker closures, and remaining publication-only items.  
- **Owner:** supervisor for the technical memo skeleton; later handoff to documentation specialist if needed  
- **Inputs required:** all package outputs, Section 7 acceptance criteria, critic-gate outcomes  
- **Outputs expected:** internal-package memo draft  
- **Estimated wallclock:** 6-10 analyst hours  
- **Dependencies:** 8.2, 8.3

**8.5** Perform the final dual-critic review: implementation-oriented review against this plan and physics-oriented review of every headline number.  
- **Owner:** supervisor dispatching `implementation-critic` and `h1-analysis-critic`  
- **Inputs required:** complete package inventory, memo draft, final plots/tables  
- **Outputs expected:** sign-off notes or blocking action list  
- **Estimated wallclock:** 3-5 analyst hours plus review latency  
- **Dependencies:** 8.4

**8.6** Perform a numerical-consistency spot-check by hand-rederiving at least two representative headline numbers directly from raw JSON inputs.  
- **Owner:** supervisor  
- **Inputs required:** final JSON package  
- **Outputs expected:** explicit hand-check appendix in memo or review notes  
- **Estimated wallclock:** 1-2 analyst hours  
- **Dependencies:** 8.5

### Critical critic interjection points for Phase 8

- **CRITIC GATE 8.1:** before memo finalization, dispatch `implementation-critic` to review the package against this plan and identify any scope drift or missing closure.
- **CRITIC GATE 8.2:** before headline tables are frozen, dispatch `h1-analysis-critic` to review every headline number and verify that caveats, blinding rules, and blocker states are being represented honestly.
- **CRITIC GATE 8.3:** before the internal package is declared ready, dispatch a numerical-consistency review that re-derives at least two results from the raw JSONs and checks agreement within rounding tolerance.

### Phase 8 code-change anchor snippets

```python
result_row = {
    "bin_id": bin_id,
    "value": central,
    "stat_unc": stat,
    "syst_total": syst_total,
    "by_source": source_breakdown,
}
```

```markdown
### Frozen decisions used
- F1: gamma*+p HCM frame
- F2-r2: HQ-v5f-r2 binning
- F14: A_BC blinding
- F15: Trento convention
```

### Phase 8 considerations and trade-offs

- The memo and package table work should wait until the hard scientific pieces are stable; otherwise the team will waste effort rewriting presentation around moving numbers.
- If HB5 is still open at this stage, the package must make the scope split explicit rather than burying it.
- If `phi_h` is on the EXTENDED path rather than the CORE path, the package must label that choice explicitly and keep the control-only low-z point out of the headline `phi_h` discussion.

---

## Phase 9: Publication preparation (deferred per r5 G33)

This phase is **out of scope** for detailed execution in `plan-finalization-r1.md`.

### Trigger condition

Phase 9 begins only after the supervisor signs off that **Phase 8 internal-package readiness is complete** and that the Section 7 acceptance criteria are either satisfied or explicitly satisfied with an agreed observable-level exclusion.

### What would happen next

- HEPData packaging
- paper draft outline
- collaboration-review preparation
- final publication-scoped blinding / unblinding governance as applicable

No detailed task breakdown is included here by design.

---

## 4. Risk register

This updates the older r5 risk framing using the post-rigorization state.

| Risk ID | Risk | Likelihood | Impact | Why it matters now | Mitigation / trigger |
|---|---|---|---|---|---|
| R1 | Pass-B detector iSys reproduction takes longer than expected | High | High | Phase 2 is the longest operational campaign and touches builder + Condor + downstream reintegration | Strict smoke tests, limited first launch, explicit factor-2 analytic-vs-iSys comparison before reruns expand |
| R2 | DJANGOH14 NonRad production fails or is unavailable | Medium-High | High | Keeps `crad_placeholder` alive and blocks full finalization of `M`, `σ`, and `A_BC` | Early strategy choice (Section 5), coordinate with low-Q² efforts, keep fallback exclusion logic explicit |
| R3 | `C_rad` model spread (RAPGAP vs DJANGOH NonRad) is large and dominates `A_BC` budget | Medium | High | Could change the qualitative interpretation of `A_BC` systematics after the placeholder is replaced | Require explicit critic review of model spread before freezing source treatment |
| R4 | e- recovery surfaces a period-dependent systematic not anticipated by cancellation arguments | Medium | High | Would weaken publication readiness of `A_h` / `A_BC` even after ep0304 is included | Mandatory per-period yield-per-lumi stability review before OQ7 is declared closed |
| R5 | HQ-v5f-r2 `z in [0.7,1.0]` is dominated by exclusive VMD contamination beyond current placeholder caveat | Medium | Medium-High | Could force demotion of the high-z bin from headline comparisons | Require explicit critic gate before using that bin in headline multipanel figures |
| R6 | One of the rigorization fixes itself has a methodological flaw | Low-Medium | High | A v2 fix could have corrected one defect while introducing another | Preserve critic gates on `C_rad`, unfolding-default choice, luminosity-on-`A_BC`, and detector reruns |
| R7 | HB5 does not close cleanly | Medium | Medium-High | May force absolute `σ` out of the final headline package | Keep multiplicities and asymmetries package-able independently; require explicit exclusion record if needed |
| R8 | r2 migration introduces silent divergence between old r1 and new r2 observables | Medium | Medium | Could hide bin-mapping or denominator bugs behind the legitimate new z bins | Require side-by-side r1/r2 overlap checks in the common z range |
| R9 | `phi_h` edge bins fail fit-quality or angular-acceptance tests | Medium | Medium | Could force a `phi_h` scope reduction even if the main non-phi program succeeds | Separate supervisor decision on whether `phi_h` is headline or follow-up if quality remains marginal |

### Risk posture summary

The dominant risks are now **execution-heavy**, not conceptual. The analysis logic is mature enough that the likely failure modes are delayed reruns, weak closure in cross-checks, or a late discovery that one edge bin / one observable family must be demoted from headline scope.

---

## 5. Open supervisor decisions

The plan is implementation-ready only after the **pre-execution** decisions below are made. Additional choices can be deferred to mid-execution without blocking the start of work.

### 5.1 Supervisor decisions REQUIRED before execution begins

1. **`phi_h` scope:** headline EXTENDED-path item or follow-up-paper / control-only item?
2. **`C_rad` strategy:** adopt **Strategy 2** as default, and define the explicit trigger for falling back to Strategy 1.
3. **LAr option A/B/C for publication-grade treatment:** confirm the intended final framing even though this plan defaults to dedicated Pass-B reruns.
4. **`ep0304` beam-energy policy framing for F13 / OQ7:** confirm the interpretation to be used when Phase 4 starts.
5. **HB5 coordination stance:** analysis-local closure attempt only, or shared cross-regime closure with the other high-`Q²` analyses?

### 5.2 Supervisor decisions DEFERRED to mid-execution

1. **Blinding posture after e− recovery:** whether `A_h` / `A_BC` remain blinded or partially unblinded after the recovery and validation stages.
2. **Trigger-efficiency treatment:** whether the per-`Q²` trigger-efficiency numbers are cited from the internal note or remeasured on the current v5f sample.

### 5.3 Why the split matters

- The **required-before-execution** decisions determine which phases are on the active path, how Phase 3 and Phase 4 launch, and whether Phase 6 must begin in cross-regime mode.
- The **deferred** decisions affect package finishing details but do not change the structure of Phases 1-6.

---

## 6. Calendar timeline

These estimates assume **one analyst at ~0.5 FTE** and occasional NAF jobs rather than continuous dedicated production support.

### 6.1 CORE-path best-case timeline

Assumes: no failed major campaign, queue delays are modest, and each major critic gate passes on the first try.

- Phase 1: 1-2 weeks
- Phase 2: 3-4 weeks
- Phase 3: 2-3 weeks
- Phase 4: 1-2 weeks
- Phase 5: 1 week
- Phase 8: 1-2 weeks
- Phase 6: can overlap and therefore is not mandatory for CORE readiness

**Best-case total to internal-package readiness (CORE):** **12-14 weeks**

### 6.2 CORE-path realistic timeline

Assumes: typical NAF queue delays, one rework cycle for the major technical gates (Pass-B, `C_rad`, or ep0304), and some memo/package iteration.

- Phase 1: 2 weeks
- Phase 2: 4-6 weeks
- Phase 3: 3-4 weeks
- Phase 4: 2-3 weeks
- Phase 5: 1-2 weeks
- Phase 8: 2 weeks
- Phase 6: can overlap, but may still affect publication readiness later

With overlap, the total is less than the raw sum.

**Realistic total to internal-package readiness (CORE):** **16-20 weeks**

### 6.3 CORE-path worst-case timeline

Assumes: Phase 2 reveals a methodology issue requiring re-extraction, NonRad access is delayed, or the shared cross-regime `C_rad` / HB5 dependencies stall.

**Worst-case total to internal-package readiness (CORE):** **24-32 weeks**, with a possible indefinite blocker if the shared NonRad path remains unavailable.

### 6.4 EXTENDED-path add-on

If the supervisor requires `phi_h` in the headline internal package, add:

- **Phase 7:** +1-2 weeks

So the EXTENDED path is the CORE timeline **plus 1-2 weeks**.

### 6.5 Longest critical-path item

The **single longest critical-path item** is **Phase 2 Pass-B detector iSys reproduction**, because its queue-limited execution is intrinsically slower than the local bookkeeping phases and because several later package-quality claims depend on it.

### 6.6 Reconciliation with phase wallclocks

- The per-phase analyst-hour estimates in Section 3 sum to substantially less than the calendar totals because the dominant schedule cost is **NAF / Condor wallclock and review latency**, not hands-on editing time.
- Phase 2 and Phase 3 dominate the calendar even though they do not dominate raw analyst hours.
- Phase 8 depends on the upstream scientific streams stabilizing; its calendar footprint is mostly delayed by dependencies, not by writing effort alone.

---

## 7. Acceptance criteria for “analysis is finalized”

The analysis is considered **finalized** only when all applicable criteria below are verifiably satisfied.

1. **Every populated HQ-v5f-r2 bin in every headline observable has a central value, statistical uncertainty, total systematic uncertainty, and per-source breakdown.**
2. **The headline baseline is HQ-v5f-r2, not HQ-v5f-r1, and all headline plots/tables use the 6-z-bin grid.**
3. **All 14 systematic sources are present by name in the final budget and each is classified as either `fully_evaluated_per_bin_*` or `flat_documented_with_blocker_provenance`; no headline source remains `placeholder_pending_*`, except where an explicit scope cut documented in this plan says the observable is not headline.**
4. **The `crad_placeholder` source has been removed from the headline budget and replaced by a real per-bin `C_rad` evaluation.**
5. **Pass-B iSys reproduction is completed and adopted for the four detector-scale sources: `lar_electron_escale`, `lar_electron_angle`, `hfs_hadronic_scale`, and the planned detector-shift catalog covered in Phase 2, unless a source has been explicitly failed closed under CG-2.3b and excluded from the headline budget pending resolution.**
6. **The Option H mask per-component scan is completed and the flat placeholder-style treatment is replaced by a per-bin or per-component-derived source.**
7. **ep0304 recovery is completed, and publication-grade `A_h` / `A_BC` use the four-period sum rather than the current three-period sum.**
8. **F14 blinding for `A_BC` is respected throughout intermediate steps and any unblinding/sign-off state is explicitly documented in the internal-package memo.**
9. **HQ-v5f-r2 multiplicity, asymmetry, and—if HB5 closes—absolute cross-section assembly are complete and internally consistent with the r2 acceptance / response products.**
10. **All headline plots are regenerated on HQ-v5f-r2 with stat+syst overlays and explicit caveats for the `z in [0.0,0.1]` and `z in [0.7,1.0]` bins.**
11. **If the supervisor approves `phi_h` as a headline EXTENDED-path observable, the headline `phi_h` fits cover the four central z bins in `z ∈ [0.1,0.7]`; the r2 extension bins are computed as control-only unless a dedicated target-fragmentation study re-approves low-z inclusion.**
12. **HB5 H1CalcWeight validation is completed, or else absolute cross-sections are explicitly excluded from the headline finalized package with a documented scope decision.**
13. **Implementation-critic and analysis-critic sign-off are both documented in the internal-package memo.**
14. **The internal-package memo is written, reviewed, and numerically spot-checked against raw JSON inputs.**
15. **All frozen decisions used in the final package (F1, F2-r2, F3, F5, F6, F7, F8, F9, F11, F13, F14, F15) are explicitly cited in the memo and reflected consistently in the result tables.**
16. **`phi_h`-specific criteria are applicable only if the supervisor approves `phi_h` on the headline internal-package path; otherwise `phi_h` may remain a follow-up/control-only stream without blocking CORE readiness.**

---

## 8. Critic interjection points (consolidated)

| Gate ID | Phase / action item | Critic specialist | Pass criterion | What blocks if it fails |
|---|---|---|---|---|
| **CG-1.1** | Phase 1 / 1.1-1.2 | `h1-analysis-critic` | New low-z and high-z bins remain physically interpretable with the caveats mandated by the F2-r2 decision | r2 baseline freeze |
| **CG-1.2** | Phase 1 / 1.2 | `h1-analysis-critic` | `z in [0.0,0.1]` bin looks like a caveated soft-fragmentation bin, not a broken extraction | headline use of low-z bin |
| **CG-1.3** | Phase 1 / 1.3 | `h1-analysis-critic` | `z in [0.7,1.0]` contamination level is compatible with current caveat language or triggers stronger exclusion language | headline use of high-z bin |
| **CG-1.4** | Phase 1 / 1.4 | `h1-analysis-critic` | Per-bin stability tagging is applied consistently across all bins | r2 budget freeze |
| **CG-2.1** | Phase 2 / 2.1 | `implementation-critic` | `SetSysShift(int iSys)` implementation matches planned H1 systematic semantics exactly | full Pass-B launch |
| **CG-2.2** | Phase 2 / 2.2 | `h1-analysis-critic` | Smoke-test kinematic shifts have plausible magnitudes and signs | full Pass-B Condor submission |
| **CG-2.3** | Phase 2 / 2.5 | `h1-analysis-critic` | iSys-derived detector shifts agree with analytic-v2 expectations to within understood factor-2-level deviations | adoption of new detector-source numbers |
| **CG-2.3b** | Phase 2 / >3× discrepancy branch | `h1-analysis-critic` + supervisor discrepancy memo | Any >3× disagreement is either resolved or the affected source is explicitly failed closed and excluded from the headline budget pending resolution | adoption of the affected detector source |
| **CG-2.4** | Phase 2 / 2.5-2.6 | `h1-analysis-critic` | Unfolding / regularization assumptions remain valid when detector reruns are inserted | final detector-systematics freeze |
| **CG-3.1** | Phase 3 / 3.1 | `h1-analysis-critic` | NonRad configuration smoke test demonstrates the intended physical setup | large NonRad production / reuse adoption |
| **CG-3.2** | Phase 3 / 3.2 | `h1-analysis-critic` | `C_rad` values are mostly `O(1)` with any >20% outliers understood | replacement of `crad_placeholder` |
| **CG-3.3** | Phase 3 / 3.4 | `h1-analysis-critic` | Beam-sign-separated `C_rad` tables for `A_BC` are physically plausible and consistently indexed | asymmetry budget freeze |
| **CG-3.4** | Phase 3 / 3.5 | `h1-analysis-critic` | RAPGAP-vs-DJANGOH `C_rad` spread is either acceptably small or properly elevated into the budget logic | final radiative-correction sign-off |
| **CG-4.1** | Phase 4 / 4.1 | `h1-analysis-critic` | ep0304 beam-energy policy is consistent with F13 and period bookkeeping | ep0304 re-extraction |
| **CG-4.2** | Phase 4 / 4.4 | `h1-analysis-critic` | Per-period yield-per-lumi stability shows no unexplained period pathology | OQ7 closure |
| **CG-4.3** | Phase 4 / 4.3-4.4 | `h1-analysis-critic` | Three-period vs four-period `A_h` change is qualitatively expected and not symptomatic of a bug | publication-grade `A_h` claim |
| **CG-5.1** | Phase 5 / 5.2 | `h1-analysis-critic` | Each Option H component shift is physically reasonable and below the rough <5% expectation | adoption of per-component Option H results |
| **CG-5.2** | Phase 5 / 5.3 | `h1-analysis-critic` | Quadrature-combined Option H source is consistent with or explainably different from the current flat 4% placeholder | final Option H source replacement |
| **CG-6.0** | Phase 6 / pre-launch coordination | `h1-software` | Cross-regime HB5 coordination has happened and closure standards are aligned across consuming high-`Q²` analyses | local Phase 6 launch |
| **CG-6.1** | Phase 6 / 6.1 | `h1-analysis-critic` | Per-period H1CalcWeight closure is adequate | HB5 closure claim |
| **CG-6.2** | Phase 6 / 6.3 | `h1-analysis-critic` | Absolute σ normalization is compatible with known H1 high-Q² inclusive anchors | publication use of absolute cross-sections |
| **CG-7.0** | Phase 7 / precondition | `h1-analysis-critic` | Default exclusion of `z ∈ [0.0,0.1]` from headline `phi_h` fits is confirmed and documented as target-fragmentation-driven | start of headline `phi_h` extraction |
| **CG-7.1** | Phase 7 / 7.2 | `h1-analysis-critic` | `phi_h` fit quality meets the package threshold (`χ²/dof < 5` in >80% of cells with `N_phi >= 6`) | final `phi_h` package inclusion |
| **CG-7.2** | Phase 7 / 7.3 | `h1-analysis-critic` | `phi_h` resolution assumptions remain valid on r2, including edge bins | adoption of `phi_h` systematic updates |
| **CG-7.3** | Phase 7 / 7.4 | `h1-analysis-critic` | Angular acceptance correction and Trento convention are verified on the final r2 extraction | `phi_h` sign-off |
| **CG-8.1** | Phase 8 / 8.4-8.5 | `implementation-critic` | Full package matches this approved plan with no unacknowledged scope drift | internal-package readiness claim |
| **CG-8.2** | Phase 8 / 8.5 | `h1-analysis-critic` | Every headline number, caveat, and blinding state is physically and procedurally sound | internal-package sign-off |
| **CG-8.3** | Phase 8 / 8.6 | `implementation-critic` or supervisor-led numerical check | Hand-rederived numbers match JSON-based results within expected rounding tolerance | package release to internal circulation |

Total consolidated critic gates: **27**.

---

## Todo

- [ ] **Phase 1 / 1.1** Re-extract `n_dis_eps_dis` on HQ-v5f-r2 using the existing NAF rebuild handoff.
- [ ] **Phase 1 / 1.2** Re-run `M`, `σ`, `A_h`, and `A_BC` assembly on HQ-v5f-r2.
- [ ] **Phase 1 / 1.3** Re-aggregate the systematic budget and rerender main plots on HQ-v5f-r2.
- [ ] **Phase 1 / 1.3a** Update the full HQ-v5f-r2 script touch set, including the five named NAF rerun scripts, the four local plot/aggregation scripts, and the exact `n_dis_eps_dis` extractor filename confirmation.
- [ ] **Phase 1 / 1.4** Add per-bin stability tagging on HQ-v5f-r2, including both new z-edge bins.
- [ ] **Phase 1 / 1.5** Record the r2 migration checkpoint making HQ-v5f-r2 the live finalization baseline.
- [ ] **Phase 1 / CG-1.1** Critic review of the new z-edge bins and caveats.
- [ ] **Phase 1 / CG-1.2** Critic review of the low-z bin behavior.
- [ ] **Phase 1 / CG-1.3** Critic review of the high-z diffractive / VMD caveat.
- [ ] **Phase 1 / CG-1.4** Critic review of per-bin stability tagging.
- [ ] **Phase 2 / 2.1** Add `SetSysShift(int iSys)` to the authoritative NAF `ChargedSidisTreeBuilderHighQ2.C`, using the local `source_cache/ProtonTreeBuilderHighQ2.C` only as a read-only reference.
- [ ] **Phase 2 / 2.2** Run one-file smoke tests for each iSys variation.
- [ ] **Phase 2 / 2.3** Launch the full MC-side iSys campaign plus the required data-side reruns for `+7`, `-7`, and `+10`.
- [ ] **Phase 2 / 2.4** Re-derive observables for each iSys variation on HQ-v5f-r2.
- [ ] **Phase 2 / 2.5** Replace analytic detector-scale systematics with iSys-derived per-bin sources.
- [ ] **Phase 2 / 2.6** Compare analytic-v2 vs iSys-derived detector-source sizes and document outliers.
- [ ] **Phase 2 / CG-2.1** Critic/code review of `SetSysShift(int iSys)` implementation.
- [ ] **Phase 2 / CG-2.2** Critic review of smoke-test shift magnitudes.
- [ ] **Phase 2 / CG-2.3** Critic review of analytic-v2 vs iSys-derived agreement.
- [ ] **Phase 2 / CG-2.3b** If any populated source/bin differs by >3×, document the discrepancy memo and fail the affected source closed until resolved.
- [ ] **Phase 2 / CG-2.4** Critic review of unfolding/regularization assumptions after detector reruns.
- [ ] **Phase 3 / 3.1** Decide and secure the DJANGOH14 NonRad path (in-house vs reuse/reweight).
- [ ] **Phase 3 / 3.2** Produce nominal Strategy-2 RAD/NORAD `C_rad` tables on HQ-v5f-r2.
- [ ] **Phase 3 / 3.3** Run RAPGAP31 `C_rad` cross-check for model spread.
- [ ] **Phase 3 / 3.4** Produce beam-sign-separated `C_rad` tables for all `A_BC` variants.
- [ ] **Phase 3 / 3.5** Replace `crad_placeholder` in the aggregated budget and regenerate affected plots/tables.
- [ ] **Phase 3 / CG-3.1** Critic review of NonRad configuration smoke test.
- [ ] **Phase 3 / CG-3.2** Critic review of `C_rad` sanity and outliers.
- [ ] **Phase 3 / CG-3.3** Critic review of beam-sign-separated `C_rad` tables.
- [ ] **Phase 3 / CG-3.4** Critic review of RAPGAP-vs-DJANGOH `C_rad` model spread.
- [ ] **Phase 4 / 4.1** Decide the `ep0304` beam-energy policy under F13.
- [ ] **Phase 4 / 4.2** Re-enable `ep0304` in the extractor and re-extract four-period yields.
- [ ] **Phase 4 / 4.3** Recompute `A_h`, `A_BC^+`, `A_BC^-`, `A_BC^{sum}`, and `A_BC^{DIS}` on four-period coverage.
- [ ] **Phase 4 / 4.4** Perform per-period yield-per-lumi stability checks.
- [ ] **Phase 4 / CG-4.1** Critic review of `ep0304` beam-energy policy by `h1-analysis-critic`.
- [ ] **Phase 4 / CG-4.2** Critic review of per-period stability.
- [ ] **Phase 4 / CG-4.3** Critic review of three-period vs four-period `A_h` behavior.
- [ ] **Phase 5 / 5.1** Define the Option H per-component toggle campaign.
- [ ] **Phase 5 / 5.2** Run the Option H component scan on v5f MC.
- [ ] **Phase 5 / 5.3** Quadrature-combine component shifts into an updated Option H source.
- [ ] **Phase 5 / 5.4** Replace the current Option H source in the aggregate budget.
- [ ] **Phase 5 / CG-5.1** Critic review of individual Option H component shift sizes.
- [ ] **Phase 5 / CG-5.2** Critic review of combined Option H shift vs current 4% placeholder.
- [ ] **Phase 6 / CG-6.0** Complete cross-regime HB5 coordination and agree a common closure standard before local SIDIS HB5 work begins.
- [ ] **Phase 6 / 6.1** Validate H1CalcWeight closure period-by-period for HB5.
- [ ] **Phase 6 / 6.2** Calibrate effective MC luminosity across periods/generators.
- [ ] **Phase 6 / 6.3** Re-derive validated absolute `σ^{h±}` on HQ-v5f-r2 if HB5 closes.
- [ ] **Phase 6 / 6.4** Record an explicit scope exclusion if HB5 does not close.
- [ ] **Phase 6 / CG-6.1** Critic review of H1CalcWeight closure.
- [ ] **Phase 6 / CG-6.2** Critic review of absolute-`σ` normalization against H1 anchors.
- [ ] **Phase 7 / CG-7.0** Confirm and document the default exclusion of `z ∈ [0.0,0.1]` from headline `phi_h` fits.
- [ ] **Phase 7 / 7.1** Re-extract `phi_h` yields on HQ-v5f-r2 with suitability flags for control-only bins.
- [ ] **Phase 7 / 7.2** Refit headline-eligible `phi_h` cells and store stat unc + `χ²/dof`.
- [ ] **Phase 7 / 7.3** Replace bounded `phi_h` systematics with real per-cell numbers.
- [ ] **Phase 7 / 7.4** Validate angular acceptance correction and regenerate `phi_h` plots.
- [ ] **Phase 7 / CG-7.1** Critic review of `phi_h` fit quality.
- [ ] **Phase 7 / CG-7.2** Critic review of `phi_h` resolution assumptions.
- [ ] **Phase 7 / CG-7.3** Critic review of final angular-acceptance / Trento handling.
- [ ] **Phase 8 / 8.1** Consolidate all final JSONs and plots onto one HQ-v5f-r2 package baseline.
- [ ] **Phase 8 / 8.2** Build per-observable per-bin result tables with stat+syst+source breakdown.
- [ ] **Phase 8 / 8.3** Build the cancellation-matrix display.
- [ ] **Phase 8 / 8.4** Draft the internal-package memo.
- [ ] **Phase 8 / 8.5** Run implementation-critic and analysis-critic package reviews.
- [ ] **Phase 8 / 8.6** Hand-check at least two headline numbers from raw JSON inputs.
- [ ] **Phase 8 / CG-8.1** Implementation-critic sign-off against this plan.
- [ ] **Phase 8 / CG-8.2** Analysis-critic sign-off of headline numbers and caveats.
- [ ] **Phase 8 / CG-8.3** Numerical-consistency sign-off.
- [ ] **Supervisor decision / required-before-execution** Decide whether `phi_h` is on the EXTENDED headline path or a follow-up/control-only stream.
- [ ] **Supervisor decision / required-before-execution** Confirm `C_rad` Strategy 2 default and the Strategy-1 fallback trigger.
- [ ] **Supervisor decision / required-before-execution** Confirm the LAr option A/B/C publication-grade framing.
- [ ] **Supervisor decision / required-before-execution** Confirm the `ep0304` beam-energy policy framing under F13.
- [ ] **Supervisor decision / required-before-execution** Confirm the HB5 coordination stance: local-only or shared cross-regime.
- [ ] **Supervisor decision / deferred** Decide blinding/unblinding posture for `A_h` / `A_BC` after e− recovery.
- [ ] **Supervisor decision / deferred** Decide trigger-efficiency citation vs remeasurement policy.

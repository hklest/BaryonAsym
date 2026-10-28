<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Plan: High-Q2 Charged-Hadron SIDIS Next Steps (r5)

## Revision history

- **r5 (2026-05-19)**
  - Marked major MC-side early gates as advanced from r4 based on second-wave task outputs: **G1A PASS**, **G2 MC PASS**, **G3 MC PASS**, **G4 MC PASS**, **G5 PASS**, **G6 MC PASS**; **G7/G8/G9/G12/G13-prep** are now plannable or launchable on the MC side.
  - Froze the authoritative v5b high-Q2 schema details completely: **`ChargedTot=86`, `ChargedTotGen=50`, `DisTot=49`**, with **`IsV0DaughterGen` as the 50th `ChargedTotGen` branch** and **`isMC` as the 49th `DisTot` branch**.
  - Added an explicit **Naming conventions** section: legacy `proton_*` filenames and `ProtonTreeBuilderHighQ2` names are historical fork artifacts in a **SIDIS** analysis; package code root is explicitly **`H1SIDIS_HighQ2/`**.
  - Froze eight new analysis prescriptions from second-wave task outputs: symmetric event-cut prescription, `GenMatchIdx` value semantics, MC weight prescription, data trigger/weight prescription, NuclIA storage/application rule, corrected trigger MC anchor, frozen HQ-A / HQ-B binning, and the canonical gen-pool / matching definitions.
  - Corrected the trigger anchor: **MC is neither cut on S67 nor weighted by `TrigWeightAC`; data is cut on S67 and weighted by `TrigWeightAC`**.
  - Replaced the r4 provisional NuclIA “blending” framing with the task-C verified builder behavior: **stored-only `NucliaWeight`, per-track gen-PDG dispatch, proton path = 1.0, unmatched = pion default, no pre-application into `weight1/weight2`**.
  - Hardened the phi_h workstream: G13-prep script patch is now **DONE**, missing `GenElecPhi` must raise, and the high-Q2 pipeline uses stored `GenElecPhi` directly (no low-Q2 reco-phi proxy hack).
  - Updated risks: removed resolved schema-drift and NuclIA double-count risks; added explicit risks for MC `TrigWeightAC` misuse, asymmetric event-cut leakage, `GenMatchIdx` positional misread, NuclIA sentinel anomalies, proton naming legacy, and stale `ProtonTot` references in active scripts.
  - Updated the work-status section: Tasks **A, B, C, D, E, F, H, I, J** complete; **G/N** still in retry / flight; next-batch tasks **K, L, M, N, O, P, Q** are now tracked explicitly.
- **r4 (2026-05-19)**
  - Corrected the authoritative v5b high-Q2 **per-run MC schema** to **`ChargedTot=86`, `ChargedTotGen=50`, `DisTot=49`** based on first-week validation artifacts; removed all remaining `86/49/48` references.
  - Expanded G1 into three sub-gates: **G1A production accounting**, **G1B v5b MC re-merge / multi-file decision**, and **G1C ep0607 v5b HighQ2 SIDIS DATA production**.
  - Encoded the finding that the pre-existing merged MC files in `/pnfs/.../sidis/highQ2_merged/` are **wrong-schema v4 carryover** (`82/41`, no `DisTot`) and must be quarantined from all downstream gates.
  - Encoded the finding that **no v5b HighQ2 SIDIS data exists for any period**, so G1C is now an immediate high-priority production blocker for every data-side downstream gate.
  - Froze the GRL state from task outputs: ep0607 GRL frozen, all other period GRLs identified, and the recovered **e+/e- luminosity ratio = 1.193**, which now feeds an explicit `A_BC` weighting-policy open question.
  - Reformulated the G11 LAr electron sub-gate from **REUSE** to **NEW**, because upstream `proton_HiQ` / `lambda_HiQ` do not contain a reuse-ready procedure; added three explicit supervisor decision paths (Option A/B/C).
  - Expanded the NuclIA workstream with new concrete risks and tasks: `nuclia_no_proton_path`, builder pre-application / double-count audit, blending-formula verification, and MC-only closure semantics.
  - Hardened G13-prep to patch all phi_h scripts so missing `GenElecPhi` raises instead of silently falling back to `phi_e_gen = 0`; added numerical resolution-verification tasks and LAr azimuthal-resolution citation verification.
  - Updated the first-week subset to reflect actual status: tasks 1, 2, 3, 9, 10, 11, 12 complete; tasks 4, 5, 6 in progress on per-run MC; tasks 7 and 8 can launch MC-only but remain data-blocked; new immediate priorities are G1C launch, G11 LAr option decision, and the builder-side NuclIA application audit.
  - Updated the risk register to reflect the newly surfaced blockers and traps: no v5b data, wrong-schema merged MC, LAr procedure absence, NuclIA proton-path bias, possible NuclIA double-count, documentation drift on schema, and `A_BC` weighting ambiguity from the 1.193 e+/e- lumi ratio.
- **r3 (2026-05-19)**
  - Encoded the user decision that **`phi_h` is in scope for the first internal high-Q2 package**. G13 is now a headline workstream, no longer optional, and now blocks G16 directly.
  - Encoded the user decision that **charge separation is headline scope for both asymmetry classes**: `A_h`, `A_BC(h+)`, and `A_BC(h-)` are headline observables; charge-summed `A_BC` is retained as the inclusive control and gamma-Z baseline cross-check.
  - Expanded the asymmetry systematics bookkeeping to require four explicit cancellation columns: `A_h`, `A_BC(charge-summed)`, `A_BC(h+)`, and `A_BC(h-)`.
  - Tightened G10 so `C_rad` for `A_BC` must be produced in **charge-separated** form for `A_BC(h+)` and `A_BC(h-)`, in addition to the charge-summed control channel.
  - Tightened G11 NuclIA closure so cancellation tests must run for **all four asymmetry observables**: `A_h`, `A_BC(charge-summed)`, `A_BC(h+)`, and `A_BC(h-)`.
  - Replaced the asymmetry-grid fallback as the default with a **contingency-only rule**: invoke a coarser `A_BC` grid only if, after full e- recovery, more than 20% of populated 4D bins have e- yield below 25 events.
  - Resolved the former LAr OQ3/HB7 blocker by freezing the decision to **REUSE** the already-solved LAr procedure from `proton_HiQ` / `lambda_HiQ`; updated G11 from NEW to REUSE and added first-week extraction/documentation tasks.
  - Removed the resolved LAr blocker question from the open-questions list and downgraded the old risk from an active blocker to a documentation-discipline warning: lift from `proton_HiQ` / `lambda_HiQ`, do not copy the low-Q2 SpaCal procedure.
- **r2 (2026-05-19)**
  - Reframed the high-Q2 charged SIDIS result as a **standalone paper**, not a low-Q2 companion; removed HEPData packaging from scope and deliverables for this plan revision.
  - Added **NuclIA correction as required scope**, including `NucliaWeight` enablement, semantics validation, A_h and A_BC cancellation closure tests, and NuclIA model uncertainty as a systematic source.
  - Added **beam-charge asymmetry `A_BC` as a first-class observable**, integrated through scope, gates, systematics, blinding, radiative-correction treatment, and internal-package rules.
  - Made **G15 e- period recovery a hard prerequisite for headline A_BC physics** and kept `A_h` explicitly blocked behind G15 plus blinding.
  - Patched all r1 critique fatal issues and major concerns: explicit multiplicity formula and bin-width gate, numerical zero-hadron DIS checks, per-charge acceptance enforcement, unfolding closure and regularization scan, `C_rad` dependency for internal packaging, explicit LAr electron-scale procedure gate, trigger-weight verification, ep0607-only `A_h` bias language, covariance treatment for correlated sources, numerical G14 and G15 exit criteria, gen-binned acceptance requirements, phi_h/Trento and `Q2es ≡ Q2e` freezes, corrected agent routing, and first-week sequencing.
  - Added previously omitted high-Q2 items: Empz freeze, vertex-z freeze and scan, QED Compton / elastic-radiative subtraction, electron-ID and FidVolCut validation, beam-gas and cosmics rejection, good-run-list freeze, per-period luminosity uncertainty handling, MC-stat propagation, asymmetry blinding policy, combined-track vs central-only closure, `x_diffr_flag` calibration, gamma-Z interference treatment, MC oo-release consistency check, `ParticleSource_cg` verification, broad-vs-analysis gen fiducial verification, and reconciliation of the two missing `DisTot` branches.
- **r1**
  - Initial gated execution plan for high-Q2 multiplicities with `A_h` blocked behind e- recovery and absolute cross-sections blocked behind normalization closure.

## Scope and non-goals

This plan covers the **high-Q2 only** H1 charged-particle inclusive SIDIS program in the shared `sidis_lowQ2/` workspace, with the regime defined as **`Q2 > 150 GeV2`**. It is the execution contract for a **standalone high-Q2 paper**, not a low-Q2 companion package.

The headline in-scope observables are:
- charge-separated multiplicities `M^{h+/-}(Q2, x, z, pT_h)`
- hadron charge asymmetry `A_h`
- beam-charge asymmetry `A_BC(h+)`
- beam-charge asymmetry `A_BC(h-)`
- charge-summed control asymmetry `A_BC(charge-summed)`
- azimuthal moments `A1 cos(phi_h)` and `A2 cos(2 phi_h)`

The multiplicity definition is frozen here and must be implemented verbatim:

`M^{h+/-} = N_h_corr / (N_DIS_corr * Dz * Dp_{T,h})`

where `N_h_corr` is the corrected hadron yield in the analysis bin and `N_DIS_corr` is the corrected DIS denominator in the matched `(Q2, x)` bin. The same density structure applies to multidifferential extensions, with the appropriate bin-width factors divided out explicitly for every published or internal numerical result.

The asymmetry observable family is frozen as follows:
- `A_h` probes hadron-charge asymmetry and is built from charge-separated corrected yields.
- `A_BC(h+)` and `A_BC(h-)` are **headline observables**, built in matched `(Q2, x, z, pT_h)` bins separately for positive and negative hadrons.
- `A_BC(charge-summed)` is retained as the inclusive control observable and as the main comparison channel for the expected inclusive DIS gamma-Z baseline.
- The high-Q2 `A_BC` physics motivation is explicit sensitivity to **two-photon exchange (TPE)** on top of a known small but non-zero **inclusive DIS gamma-Z interference** expectation at high `Q2`. The hadronic-side `A_BC` result must therefore be validated against, and clearly separated from, inclusive-DIS `A_BC`.

The plan is intentionally gated and must be read as a dependency-ordered path, not as a wish list.

## Naming conventions

The high-Q2 charged-particle analysis is **fully SIDIS**, not the proton/antiproton asymmetry analysis. The persistence of `proton_*` names in files, builder basenames, Condor labels, and some documentation is historical fork ancestry only.

- **Canonical package code root on NAF:** `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/`
- **Do not describe the active package as `H1ProtonAsymHighQ2/`.** That is documentation drift.
- **Legacy proton-prefixed filenames** like `proton_highQ2_mc_*.root` mean SIDIS v5b outputs in this context.
- **Any SIDIS-context reference to `ProtonTot` / `ProtonTotGen` as active trees is a defect.** Those trees were removed in v5b and stale references are being patched separately.

Authoritative alias / remediation reference:
- `sidis_lowQ2/docs/references/reference-naming-aliases-highQ2-sidis.md` (**to be emitted under next-batch task L**)

### In scope
- High-Q2 v5b production accounting, MC re-merge or explicit per-run iteration choice, and ep0607 v5b HighQ2 SIDIS data production.
- High-Q2 schema validation against the frozen **86 / 50 / 49** v5b branch counts for per-run MC and **86 / - / 49** for ep0607 v5b data.
- High-Q2 species, truth-matching, source-class, broad-gen-fiducial, and branch-delta audit for inclusive `{pi, K, p}` charged hadrons.
- High-Q2 DIS event counting from `DisTot`, including **numerical validation of zero-hadron DIS events**.
- High-Q2 freeze of event-level cuts and controls: `Empz`, `|VtxZ|`, good-run lists by period, beam-gas/cosmics rejection, electron-ID / LAr FidVolCut validation, QED Compton / elastic-radiative subtraction, and high-Q2 background sanity checks.
- High-Q2 binning, occupancy, purity, stability, migration, correction-method choice, and unfolding validation.
- High-Q2 acceptance for hadron yields and DIS denominators, derived **per charge** and with explicit MC-stat propagation.
- High-Q2 multiplicity extraction as the primary paper observable.
- High-Q2 `phi_h` azimuthal moments as a **headline internal-package deliverable**, with script hardening against missing `GenElecPhi`.
- High-Q2 `A_h`, `A_BC(h+)`, `A_BC(h-)`, and `A_BC(charge-summed)` with charge-separated propagation throughout.
- High-Q2 NuclIA correction enablement and validation, including proton-path bias treatment and builder-application audit.
- High-Q2 radiative-correction treatment, including separate `C_rad` paths for `A_BC(h+)`, `A_BC(h-)`, and `A_BC(charge-summed)` with e+p vs e-p structure handled explicitly.
- High-Q2 systematic program, including **NEW** LAr-specific detector treatment, per-period luminosity uncertainty handling for `A_BC`, and separate cancellation matrices for all asymmetry observables.
- Explicit gates for absolute normalization, e- period recovery, `A_BC` physics validation, and internal-package convergence.

### Explicit non-goals
- **Do not plan publication of absolute hadron cross-sections before G14 passes.** Those outputs remain blocked behind end-to-end `H1CalcWeight` / effective MC luminosity validation.
- **Do not plan publication or release of `A_h` before G15 passes and the `A_h` blinding policy is lifted.** High-Q2 `A_h` remains blocked until `ep0304`, `em0405`, and `em06` are recovered and validated.
- **Do not plan publication or release of any `A_BC` variant before G15 + G15.5 pass and the `A_BC` blinding policy is lifted.**
- **Do not include HEPData packaging in this plan revision.** HEPData is explicitly out of scope here.
- Do not mix low-Q2 execution tasks into this plan except where they are explicitly reused as shared tooling.
- Treat `proton_HiQ` and `lambda_HiQ` as upstream sources only for factual cross-checks already documented; they do not define SIDIS scope or selection decisions.
- Do not reopen frozen shared analysis decisions: **no P cut**, species = `{pi, K, p}`, **eSigma baseline**, **S67 high-Q2 data only**, **combined tracks included**.

## Inputs and assumed state

This plan is based on the following authoritative inputs:
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/plans/plan-highQ2-sidis-next-steps.md` (r4)
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/validation_highQ2_mc_manifest.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/validation_highQ2_mc_schema_full.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/validation_highQ2_q2es_vs_q2e.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/validation_highQ2_mc_species_matching.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/n_dis_prototype_highQ2_mc.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/results/n_dis_prototype_highQ2_mc.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/references/reference-trigger-semantics-highQ2-sidis.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/references/reference-nuclia-correction-highQ2-sidis.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/binning_study_highQ2.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/results/binning_study_highQ2.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/data/acceptance_highQ2_charged_mc_prototype.json`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/results/acceptance_highQ2_charged_mc_prototype.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/references/reference-gen-reco-matching-charged-sidis.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/reviews/audit-proton-naming-highQ2-sidis.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/context-handoff.md`
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_lowQ2/docs/references/reference-sidis-systematics.md`

### Frozen assumptions and authoritative session findings
- The authoritative v5b **per-run MC schema** is **`ChargedTot=86`, `ChargedTotGen=50`, `DisTot=49`**.
- The 50th `ChargedTotGen` branch is **`IsV0DaughterGen`**.
- The 49th `DisTot` branch is **`isMC`**.
- The pre-existing merged files in `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/` are **wrong-schema v4 carryover** (`ChargedTot=82`, `ChargedTotGen=41`, no `DisTot`, `ProtonTot` present) and are excluded from every v5b downstream gate.
- RAPGAP31 per-run MC files exist in two naming variants; the dedup rule is frozen: **ingest only** `proton_highQ2_mc_rapgap31_*.root`.
- 100 fully empty DJANGOH files are now explicitly excluded from the nominal MC set; effective recommended v5b MC input set is **364 RAPGAP + 386 DJANGOH = 750 PASS files**.
- **No v5b HighQ2 SIDIS DATA exists for any period.** The existing `H1SIDIS_HighQ2/output/data/` files are proton-pipeline schema, not SIDIS v5b data; the low-Q2 pipeline 577 files are a wrong-builder surrogate only and do not satisfy the high-Q2 schema.
- ep0607 GRL is frozen as the immediate v5b data-production target: **8091 runs, 140,306.219 nb^-1**.
- The full recoverable high-Q2 S67 total remains **356,613.59 nb^-1**; after recovery the **e+/e- luminosity ratio = 1.193**, which forces an explicit `A_BC` weighting-policy decision.
- `Q2es == Q2e` bit-identically on high-Q2 MC, and `Q2esGen == Q2eGen` bit-identically on the gen side.
- `Yes != Ye` as expected, so `Yes` remains the meaningful y variable for the eSigma baseline.
- The high-Q2 truth species set is verified as **exclusive `{pi, K, p}`**, one-to-one matching is verified, and `ParticleSource_cg` is present on `ChargedTotGen`.
- `N_DIS` zero-hadron accounting is verified on MC: `R_zero > 1` and `nChargedReco == 0` agrees exactly with join-derived zero-hadron accounting.
- `TrigWeightAC` on MC is **not** a physical MC weight. It carries pathological prescale-tail artifacts up to 524288 and must **not** be applied on MC.
- `NucliaWeight` is **stored only**, not pre-applied into `weight1` / `weight2`.
- The v5b builder does **not** do pion/kaon blending; it uses **per-track gen-PDG dispatch**:
  - default pion hypothesis on MC,
  - matched kaons -> kaon hypothesis,
  - matched protons -> `1.0`,
  - unmatched -> default pion path.
- One observed NuclIA anomaly produced `NucliaWeight = -2.58` at frequency `~4e-6`; downstream clip guard is therefore mandatory.
- The frozen high-Q2 recommended binning is **Scheme HQ-A** with corrected edges from Task E.
- The high-Q2 matching definition is frozen by reference: reco-driven, greedy, angle-only, charge-consistent, one-to-one, **2.0 degree** threshold, `GenMatchIdx` as **value**, not position.
- The phi_h formula and beam-charge treatment are correct, and G13-prep script hardening to raise on missing `GenElecPhi` is treated as completed session output.

## Frozen analysis prescriptions

These prescriptions are binding for all downstream implementation and code review. Any deviation is a defect unless explicitly superseded by a later approved plan revision.

### P1. Symmetric event-cut prescription
The acceptance and multiplicity numerator **must apply both**:
- the reco event cuts on the reco event, and
- the matched gen partner’s gen event cuts on the matched gen event.

**Forbidden form:** reco-event cuts on the numerator-event combined with gen-event cuts only on the denominator-event. That asymmetric form produces `eps > 1` through event-cut migration across the cut boundary.

This prescription is mandatory in **G6 / G7 / G9 / G13** and any downstream acceptance-like or asymmetry-like chain.

### P2. `GenMatchIdx` reading prescription
`ChargedTot.GenMatchIdx` is the **value of `ChargedTotGen.GenIdx`**, not a positional offset into the `ChargedTotGen` array. Any reco-to-gen join must therefore be done on:
- `(RunNumber, EventNumber, GenMatchIdx)` on reco side,
- `(RunNumber, EventNumber, GenIdx)` on gen side.

Treating `GenMatchIdx` as a flat or per-event positional index is a hard defect and collapses the match efficiency to zero.

### P3. MC weight prescription
MC track weight is frozen as:

`weight1 * weight2 * NucliaWeight_clipped`

with
- **no `TrigWeightAC` on MC**, and
- `NucliaWeight_clipped = clip(NucliaWeight, 0, 1)` with any out-of-range value defaulted to `1.0` for robustness.

Reason: `TrigWeightAC` on MC carries non-physical prescale-table artifacts with single-event weights up to 524288.

### P4. Data trigger / weight prescription
Once G1C lands, the data event prescription is frozen as:
- trigger requirement: `(L1L2L3ac[2] & (1 << 3)) != 0` (S67 actual-fired),
- data event weight: `TrigWeightAC`,
- no generator weight on data.

This data-side prescription must be re-confirmed on the produced ep0607 data files but is the frozen design anchor already.

### P5. NuclIA application prescription
`NucliaWeight` is **stored on `ChargedTot` and applied once downstream**. It is not pre-applied into `weight1` or `weight2`.

The builder behavior is frozen as:
- default pion hypothesis for MC tracks,
- matched K -> kaon hypothesis,
- matched p -> `1.0`,
- unmatched -> default pion path,
- no pion/kaon linear blending.

Therefore the old “blending closure” language is retired. G11 NuclIA validation must instead cover:
- on/off MC closure on multiplicities,
- on/off MC closure on asymmetries,
- charge-cancellation closure in `A_h` vs `A_BC` variants,
- sentinel-clip robustness.

### P6. Corrected trigger MC behavior anchor
The corrected supervisor anchor is:

> **MC events do not cut on trigger and are not weighted by `TrigWeightAC`. Data events cut on S67 and are weighted by `TrigWeightAC`.**

Any older wording to the contrary is superseded.

### P7. Frozen binning: Scheme HQ-A and fallback HQ-B
Nominal high-Q2 4D binning is frozen as:

```text
Q2 (GeV^2): [150, 300, 700, 2000, 20000]
x:          [0.003, 0.01, 0.03, 0.1, 0.5]
z:          [0.1, 0.2, 0.3, 0.5, 0.7]
pT_h (GeV): [0, 0.5, 1.0, 2.0, 4.0, 8.0]
Total: 4 x 4 x 4 x 5 = 320 bins per charge
```

Frozen performance anchors:
- 4D purity median: **0.66**,
- binding dimension: **z**,
- bin-by-bin viable on the well-populated subset of `(Q2, x)` cells,
- unfolding required for rim bins.

Fallback scheme for `A_BC` contingency only:
- **HQ-B:** `3 x 3 x 4 x 5 = 180 bins`.

### P8. Frozen gen-pool and matching definitions
The authoritative gen-pool definition and matching definition are frozen by:
- `sidis_lowQ2/docs/references/reference-gen-reco-matching-charged-sidis.md`

The binding gen pool is:
- `|PDG| in {211,321,2212}`,
- `inFiducialCharged == 1` (broad fiducial, verified bit-equal to `|GenEta|<3.5 && GenPt>0.05`),
- analysis fiducial `|GenEta| < 1.3 && GenPt > 0.15`,
- `IsV0DaughterGen == 0`,
- gen DIS cuts: `Q2eGen > 150`, `0.1 < YeGen < 0.6`, `35 < GenEmpz < 70`.

The binding matching definition is:
- reco-driven greedy nearest-angle assignment,
- strict charge consistency,
- one-to-one via per-event used-gen set,
- `MatchAngle < 2.0 deg`, **KEEP**.

## Gate status summary

| Gate | Status | Scope note |
|---|---|---|
| G1A | **PASS** | Corrected manifest frozen; MC inventory, exclusions, dedup, GRL state all documented. |
| G1B | **PENDING** | Low-priority operational decision: fresh merge vs explicit per-run iteration. |
| G1C | **BLOCKED / IN FLIGHT** | ep0607 v5b SIDIS data still absent; Task N retry is in flight. |
| G2 (MC) | **PASS** | 850/850 files at 86/50/49; `IsV0DaughterGen` and `isMC` identified. |
| G2 (data) | **BLOCKED on G1C** | No ep0607 v5b SIDIS data yet. |
| G3 (MC) | **PASS** | Species, one-to-one matching, `ParticleSource_cg` presence all confirmed. |
| G3 (data-side relevance) | **N/A until G1C** | Data carries no gen tree. |
| G4 (MC) | **PASS** | `R_zero > 1`; `nChargedReco==0` agrees exactly; zero-hadron accounting verified. |
| G4 (data) | **BLOCKED on G1C** | Final data-side `N_DIS` validation still unavailable. |
| G5 | **PASS** | HQ-A frozen; purity / occupancy / accessibility study complete. |
| G6 (MC) | **PASS** | Acceptance maps in `(0,1]`; symmetric event-cut defect fixed; charge ratio ~1. |
| G6 (data-side relevance) | **N/A** | Acceptance itself is MC-derived. |
| G7 (MC) | **PENDING / CAN LAUNCH** | Directly supported by `DisTot` machinery; symmetric event-cut prescription now frozen. |
| G7 (data) | **BLOCKED on G1C** | Needs produced ep0607 v5b data. |
| G8 | **PENDING / PLANNABLE NOW** | Purity threshold classification now straightforward from Task E. |
| G9 (MC-internal) | **PENDING / UNBLOCKED ON MC** | Can compute internal preliminary `M^{h+/-}` on MC now; final data result still blocked by G1C. |
| G10 | **PENDING** | Awaiting RAD/NORAD availability / Task Q feeder. |
| G11-NuclIA | **PENDING / REFORMULATED** | Builder behavior clarified; on/off + cancellation + sentinel robustness remain to run. |
| G11-LAr | **BLOCKED on supervisor decision** | Option A/B/C still unresolved. |
| G11 overall | **BLOCKED** | LAr option unresolved, though NuclIA and other systematics planning advanced. |
| G12 | **PENDING / PLANNABLE NOW** | ~26% of populated bins below 4D purity 0.5; unfolding path is now justified. |
| G13-prep | **DONE** | Script hardening and convention-freeze prep treated as completed session output. |
| G13 | **PENDING / PARTIALLY UNBLOCKED** | Uses direct `GenElecPhi`; no reco-proxy hack needed at high-Q2. |
| G14 | **BLOCKED on MC luminosity + G1C** | `oomclumi` / HAT+MODS luminosity extraction still needed; Task P in flight. |
| G15 | **BLOCKED on G1C** | ep0607 base data path must exist before full period recovery. |
| G15.5 | **BLOCKED on G11 + G15** | Physics validation unchanged. |
| G16 | **BLOCKED** | Waits on G9 + G11 + G13 + G14 + G15 as applicable by observable claim. |
| G17 | **BLOCKED** | Final publication package remains downstream of all blockers. |

## Gating tree

### G1A. High-Q2 v5b production accounting freeze
**Status:** PASS.

- **Exit criterion:**
  1. per-run v5b MC manifest frozen;
  2. RAPGAP31 dedup rule frozen;
  3. 100 fully empty DJANGOH files frozen into the exclusion ledger;
  4. wrong-schema merged v4 carryover files quarantined conceptually;
  5. data availability status frozen as no v5b HighQ2 SIDIS data exists;
  6. period GRL manifest frozen with ep0607 as immediate target.
- **Resolved corrections now recorded:**
  - 100 empty DJANGOH files,
  - corrected period mapping `7097/7093 -> ep0304`,
  - duplicate-cycle scope widened beyond RAPGAP,
  - schema spec corrected to `86/50/49`.
- **Explicit deliverable:**
  - `sidis_lowQ2/docs/data/validation_highQ2_mc_manifest.json`
  - `sidis_lowQ2/docs/data/validation_highQ2_data_availability.json`
  - `sidis_lowQ2/docs/data/grl_manifest_highQ2.json`
  - `sidis_lowQ2/docs/references/reference-highQ2-grl-manifest.md`

### G1B. v5b MC re-merge or multi-file execution decision
**Status:** PENDING.

- **Recommendation:** prefer explicit **per-run uproot iteration** over forcing a fresh merge, because the aggregate v5b MC footprint (~44 GB) is already comfortably below the 100 GB ceiling and the per-run manifest is now frozen and clean.
- **Exit criterion:** Either fresh `merged_highQ2_mc_<gen>_v5b.root` files exist with schema `86/50/49`, or downstream execution is frozen to the per-run manifest with no merged-v5b product.
- **Owner:** `h1-production`

### G1C. ep0607 v5b HighQ2 SIDIS DATA production
**Status:** BLOCKED / retry in flight (Task N).

- **Exit criterion:**
  1. ep0607 Condor submission complete under `H1SIDIS_HighQ2/` with frozen GRL,
  2. per-run v5b SIDIS data files exist,
  3. schema `86/-/49` validated,
  4. entry counts reconciled to the ep0607 GRL set,
  5. per-run data files validated on DUST (dCache scratch not reliably accessible as of 2026-05-22; /pnfs routing is retired).
- **Gating consequence:** every data-side downstream gate remains blocked until G1C succeeds.

### G2. High-Q2 v5b schema and branch-population validation
**Status:** MC PASS / data BLOCKED on G1C.

#### Status block
- **MC side:** PASS.
- **Data side:** BLOCKED on G1C.

#### Frozen criteria and findings
- MC per-run files match **`ChargedTot=86`, `ChargedTotGen=50`, `DisTot=49`** exactly across **850/850 files**.
- Wrong-schema merged v4 carryover files (`82/41`, no `DisTot`) are explicitly excluded from G2 PASS.
- The extra `ChargedTotGen` branch is **`IsV0DaughterGen`**.
- The extra `DisTot` branch is **`isMC`**.
- `Q2es == Q2e` and `Q2esGen == Q2eGen` are confirmed **bit-identically** on MC.
- Data-side schema validation remains pending the first ep0607 v5b SIDIS data files.

#### Explicit deliverable
- `sidis_lowQ2/docs/data/validation_highQ2_mc_schema_full.json`
- `sidis_lowQ2/docs/data/validation_highQ2_q2es_vs_q2e.json`

### G3. Species/matching/gen-truth audit at high-Q2
**Status:** MC PASS.

#### Status block
- **MC side:** PASS.
- **Data side:** not applicable until G1C for any data-bearing cross-checks.

#### Frozen criteria and findings
- `ChargedTotGen` contains exclusively `{pi, K, p}`.
- One-to-one matching is confirmed; 0 events with duplicated `GenMatchIdx` in the sampled audit.
- `ParticleSource_cg` is present on `ChargedTotGen`, resolving the earlier uncertainty.
- Broad fiducial semantics are verified; beam-proton contamination is bounded and removable by fiducial gating.
- The canonical matching and gen-pool definitions are now frozen by `reference-gen-reco-matching-charged-sidis.md`.

#### Explicit deliverable
- `sidis_lowQ2/docs/data/validation_highQ2_mc_species_matching.json`
- `sidis_lowQ2/docs/references/reference-gen-reco-matching-charged-sidis.md`

### G4. DIS event accounting, event-selection freeze, and trigger treatment
**Status:** MC PASS / data BLOCKED on G1C.

#### Status block
- **MC side:** PASS.
- **Data side:** BLOCKED on G1C.

#### Frozen criteria and findings
- `R_zero > 1` in every sampled file; `DisTot` includes zero-hadron DIS events.
- `nChargedReco == 0` agrees exactly with join-derived zero-hadron accounting.
- **Corrected trigger anchor now frozen:**
  - MC: no trigger cut, no `TrigWeightAC`, weight = `weight1 * weight2 * NucliaWeight_clipped`.
  - Data: S67 cut required, weight = `TrigWeightAC`.
- `TrigWeightAC` on MC is non-trivial and can carry values up to 524288; applying it on MC is forbidden.
- MC-side G4 PASS is sufficient to unlock MC-only G7 prototype work.

#### Explicit deliverable
- `sidis_lowQ2/docs/data/n_dis_prototype_highQ2_mc.json`
- `sidis_lowQ2/docs/results/n_dis_prototype_highQ2_mc.md`
- `sidis_lowQ2/docs/references/reference-trigger-semantics-highQ2-sidis.md`

### G5. Occupancy + purity + binning study at high-Q2
**Status:** PASS.

#### Status block
- PASS.

#### Frozen criteria and findings
- **Scheme HQ-A** is the frozen nominal 4D grid:
  - `Q2: [150, 300, 700, 2000, 20000]`
  - `x:  [0.003, 0.01, 0.03, 0.1, 0.5]`
  - `z:  [0.1, 0.2, 0.3, 0.5, 0.7]`
  - `pT_h: [0, 0.5, 1.0, 2.0, 4.0, 8.0]`
- 4D purity median is **0.66**.
- The **binding dimension is z**.
- Bin-by-bin is viable on the well-populated subset; full 4D unfolding is required for rim bins.
- **Explicit per-bin classification rule frozen for downstream:**
  - `purity >= 0.5` -> bin-by-bin allowed,
  - `purity < 0.5` -> unfolding required.
- Fallback **HQ-B** (`3 x 3 x 4 x 5 = 180`) remains contingency-only for `A_BC` if G15 statistics trigger fires.

#### Explicit deliverable
- `sidis_lowQ2/docs/data/binning_study_highQ2.json`
- `sidis_lowQ2/docs/results/binning_study_highQ2.md`

### G6. Hadron acceptance `eps_h(Q2,x,z,pT_h)` for h+ and h-
**Status:** MC PASS.

#### Status block
- **MC side:** PASS.
- **Data side relevance:** N/A (acceptance is MC-derived).

#### Mandatory gate criteria
1. **Symmetric event-cut prescription is mandatory.** Numerator must apply both reco and matched-gen event cuts. Asymmetric event-cut forms are forbidden.
2. **`GenMatchIdx` join-on-value prescription is mandatory.**
3. Acceptance must remain gen-binned and one-to-one matched.
4. No clipping of `eps > 1`; any such overflow is a bug.

#### Frozen findings
- All populated retained bins satisfy `eps in (0, 1]` after the symmetric-event-cut fix.
- `eps(h+)/eps(h-)` median is ~1 with reported statistical uncertainty.
- RAPGAP vs DJANGOH median acceptance ratio is ~1; ~20% of bins show >20% disagreement, concentrated in rim bins.
- Sub-acceptance breakdown (geometric / matching / track-quality) is a follow-up, not a blocker to the current PASS.

#### Explicit deliverable
- `sidis_lowQ2/docs/data/acceptance_highQ2_charged_mc_prototype.json`
- `sidis_lowQ2/docs/results/acceptance_highQ2_charged_mc_prototype.md`

### G7. DIS-event acceptance `eps_DIS(Q2,x)`
**Status:** MC CAN LAUNCH / data BLOCKED on G1C.

#### Status block
- **MC side:** PENDING but unblocked.
- **Data side:** BLOCKED on G1C.

#### Frozen criteria
- Compute `eps_DIS(Q2,x)` on MC from `DisTot` directly, using reco DIS variables for the numerator and gen DIS variables for the denominator.
- Apply the **symmetric event-cut prescription**.
- Data-side `eps_DIS` waits on actual ep0607 v5b data.

### G8. Correction-method choice per bin
**Status:** PLANNABLE NOW.

#### Status block
- PENDING / plannable now from frozen G5 and G6 outputs.

#### Frozen criteria
- Use Task E purity values and the fixed threshold:
  - `purity >= 0.5` -> `bin_by_bin`
  - `purity < 0.5` -> `unfold`
- z is the binding dimension and drives most rim-bin unfolding need.

### G9. Multiplicity extraction `M^{h+/-}`
**Status:** MC-internal UNBLOCKED / final data result BLOCKED on G1C.

#### Status block
- **MC-internal path:** can launch.
- **Final data-bearing path:** blocked on G1C.

#### Mandatory gate criteria
- Use **Scheme HQ-A**.
- Use **symmetric event-cut prescription**.
- Use frozen **gen-pool definition** and **matching definition**.
- Use frozen **MC weight prescription**:
  - `weight1 * weight2 * NucliaWeight_clipped`
  - never `TrigWeightAC` on MC.
- Any multiplicity extracted before G14 is **internal preliminary only**.

### G10. High-Q2 radiative-correction status (`C_rad`)
**Status:** PENDING.

- No structural change beyond r4.
- Feeder work from Task Q is running in parallel.
- All asymmetry-specific charge-separated `C_rad` requirements from r3/r4 remain in force.

### G11. High-Q2 systematics evaluation
**Status:** BLOCKED overall; NuclIA sub-gate materially advanced.

#### G11-NuclIA update
The old blending-closure concept is retired. Replace it with:
1. MC on/off closure on multiplicities,
2. MC on/off closure on `A_h`, `A_BC(charge-summed)`, `A_BC(h+)`, `A_BC(h-)`,
3. charge-cancellation closure between `A_h` and `A_BC` variants,
4. sentinel-clip robustness.

#### Additional frozen NuclIA requirements
- `nuclia_no_proton_path` remains a named systematic source.
- Builder application audit is now resolved in principle: stored-only, not pre-applied.
- Downstream must clip any `NucliaWeight` outside `(0,1]` to `1.0` and record clip counts.
- The observed out-of-range anomaly frequency is `~4e-6`; add to the builder v5c fix queue.

#### G11-LAr
- Unchanged from r4: still **NEW**, still critical-path blocked on supervisor Option A/B/C.
- Low-Q2 SpaCal procedure remains forbidden.

### G12. Unfolding for 4D multiplicities
**Status:** PLANNABLE NOW.

- Task E justifies the unfolding path explicitly: ~26% of populated bins have 4D purity < 0.5 and require unfolding.
- z is the binding dimension driving most of the off-diagonal migration challenge.

### G13-prep. phi_h hardening and verification
**Status:** DONE (session output treated as complete).

- Missing `GenElecPhi` now must raise; silent `phi_e_gen = 0` fallback is forbidden.
- High-Q2 phi_h uses stored `GenElecPhi` directly.
- Low-Q2 reco-phi proxy hack is not needed in the high-Q2 path.
- Convention freeze remains anchored by `critique-phih-convention-freeze-highQ2-sidis.md` plus patched script behavior.

### G13. High-Q2 `phi_h` azimuthal moments
**Status:** PARTIALLY UNBLOCKED.

- Formula and beam-charge handling are correct.
- High-Q2 path uses direct `GenElecPhi` from v5b.
- Resolution verification and packaged extraction remain pending, but the conceptual pipeline is no longer blocked by convention ambiguity.

### G14. Normalization validation gate for absolute cross-sections
**Status:** BLOCKED.

- High-Q2 v5b MC luminosity is still unknown.
- Add explicit task: rerun `oomclumi` (or validated equivalent) on the HAT+MODS input chain to determine effective MC luminosity for RAPGAP31 and DJANGOH14.
- Task P is running in parallel as feeder work.
- The numerical exit criteria from r4 remain unchanged.

### G15. e- period recovery gate for `A_h` and `A_BC`
**Status:** BLOCKED on G1C.

- No change in physics scope.
- Base ep0607 v5b SIDIS data path must exist before period recovery can meaningfully extend it.

### G15.5. `A_BC` physics validation gate
**Status:** BLOCKED on G11 + G15.

- No change to the physics definition or blinding policy in this revision.
- The current default formula remains the **lumi-weighted** one until the supervisor chooses otherwise.

### G16. First internal high-Q2 convergence package
**Status:** BLOCKED.

- Dependency list unchanged in structure, but now explicitly encode that the package waits on:
  - G9 (MC-only multiplicity path at minimum),
  - G11 (NuclIA closure at minimum, plus LAr path if claims need it),
  - G13 (`phi_h` moments),
  - G14 (for any absolute claim),
  - G15 (for any `A_BC` release).

### G17. Standalone-paper publication-grade packaging
**Status:** BLOCKED.

- No structural change.
- HEPData remains out of scope.

## Per-gate dependency graph

| Gate | Depends on | Blocks | Critical-path note |
|---|---|---|---|
| G1A | none | G1B, G1C, G2(MC), G15 | **PASS**; freezes authoritative manifests, quarantines wrong-schema merges, and freezes GRLs. |
| G1B | G1A | G2(MC), G3, G4(MC), G5, G6, G7(MC) | Operational decision still pending; MC-side work can already proceed from the per-run manifest. |
| G1C | G1A | G2(data), G4(data), G7(data), G9, G11(data), G14, G15, G15.5, G16 | **Hard data-side blocker** because no v5b SIDIS data exists yet. |
| G2(MC) | G1A, G1B or per-run manifest decision | G3, G4(MC), G13 | **PASS**; schema truth on per-run MC is established. |
| G2(data) | G1C | G4(data) | Data-side schema validation cannot begin before ep0607 data production lands. |
| G3 | G2(MC) | G5, G6 | **PASS**; species/matching truth is established. |
| G4(MC) | G2(MC) | G5, G7(MC) | **PASS**; MC-only `N_DIS` zero-hadron logic verified. |
| G4(data) | G1C, G2(data) | G7(data), G9, G14 | **Critical data path** because final `N_DIS` must be validated on actual ep0607 v5b data. |
| G5 | G3, G4(MC) | G6, G7, G8, G10, G13 | **PASS**; HQ-A and purity structure frozen. |
| G6 | G3, G5 | G8, G9, G13, G11 | **PASS (MC)**; hadron acceptance prescription frozen. |
| G7 | G4(MC/data), G5 | G8, G9 | MC path can launch; final data-bearing path requires G1C + G4(data). |
| G8 | G5, G6, G7(MC) | G9, G11, G12 | Plannable now from frozen purity numbers. |
| G9 | G1C for final data path; G5, G6, G7, G8 for MC-internal path | G10, G11, G12, G14, G16 | MC-only internal path can launch; final package still waits on data. |
| G10 | G5, G9 | G11, G15.5, G16, G17 | Pending RAD/NORAD availability. |
| G11 | G8, G9, G10 + supervisor decision on G11-LAr | G12, G15.5, G16, G17 | **Critical path**; blocked until LAr option chosen. |
| G12 | G8, G9 | G16, G17 | Plannable now; justified by Task E purity structure. |
| G13 | G2(MC), G5, G6 | G16, G17 | Partially unblocked; direct `GenElecPhi` path and hardened scripts are in place. |
| G14 | G1C, G4(data), G9 + MC lumi audit | G17, any publication multiplicities, all absolute xsecs | **Hard publication blocker**; MC lumi still unknown. |
| G15 | G1C | `A_h`, all `A_BC` variants, G17 if any asymmetry is claimed | **Hard blocker** for recovered beam-complete asymmetry physics. |
| G15.5 | G10, G11, G15 | all `A_BC` variants, G16 if `A_BC` included, G17 if `A_BC` claimed | **Hard physics-correctness blocker** for `A_BC`. |
| G16 | G9, G10, G11, G13, G14/G15 as claim-dependent | G17 | Main internal convergence milestone. |
| G17 | G16 + publication gates | none | Final high-Q2 standalone-paper endpoint. |

## Reuse vs new work

| Gate / workstream | Tag | Current r5 disposition |
|---|---|---|
| G1A | ADAPT | PASS; manifest corrections complete. |
| G1B | NEW | Still pending low-priority operational decision. |
| G1C | NEW | Still blocking data-side execution. |
| G2 | ADAPT | MC PASS; data side pending G1C. |
| G3 | ADAPT | MC PASS. |
| G4 | ADAPT | MC PASS with corrected trigger anchor. |
| G5 | ADAPT | PASS; HQ-A frozen. |
| G6 | ADAPT | MC PASS; symmetric event-cut and join-on-value prescriptions frozen. |
| G7 | ADAPT | MC side now directly plannable from DisTot. |
| G8 | ADAPT | Plannable now from purity thresholds. |
| G9 | ADAPT | MC-internal extraction now unblocked. |
| G10 | ADAPT | Awaiting RAD/NORAD input status. |
| G11-LAr | NEW | Still blocked on supervisor option A/B/C. |
| G11-NuclIA | NEW | Reformulated from “blending” to verified stored-only + PDG-dispatch + clip-guard logic. |
| G12 | ADAPT | Plannable now. |
| G13 | ADAPT | Hardened; direct high-Q2 gen-electron path retained. |
| G14 | NEW | Still blocked on MC luminosity audit. |
| G15 | NEW | Unchanged; blocked by lack of ep0607 data base. |
| G15.5 | NEW | Unchanged physics gate. |
| G16 | ADAPT | Dependency language clarified; still blocked. |
| G17 | ADAPT | Unchanged. |

## Systematics framing for `A_h` and `A_BC`

The systematics package must carry **four separate cancellation columns** for asymmetries:
- **`A_h cancellation`**
- **`A_BC(charge-summed) cancellation`**
- **`A_BC(h+) cancellation`**
- **`A_BC(h-) cancellation`**

Key enforced distinctions:
- In `A_h`, charge-symmetric detector effects may cancel to first order.
- In `A_BC(charge-summed)`, beam-charge-symmetric effects may cancel to first order, including the common part of tracking efficiency after explicit closure.
- In `A_BC(h+)` and `A_BC(h-)`, **tracking effects by hadron charge do not cancel** simply because the comparison is e+p vs e-p inside a fixed hadron-charge channel.
- `NucliaWeight` cancellation must be **checked**, not assumed, in all four asymmetry observables.
- `C_rad` for `A_BC` is **not** the same object as multiplicity `C_rad`; e+p and e-p interference terms differ in sign and magnitude.
- Per-period luminosity uncertainty is negligible for multiplicity ratios but **critical for all `A_BC` variants**, where beam-charge-separated yields are luminosity-weighted.

### Required cancellation matrix summary

| Source | A_h | A_BC(charge-summed) | A_BC(h+) | A_BC(h-) | Notes |
|---|---|---|---|---|---|
| Luminosity | cancels | partial | partial | partial | Cancels in `A_h`; enters beam-charge-separated yield normalization for `A_BC`. |
| Trigger efficiency / `TrigWeightAC` | partial | cancels | cancels | cancels | Beam-charge-symmetric trigger effects cancel in `A_BC`; charge asymmetries can leave residuals in `A_h`. |
| Electron energy / angle (LAr) | partial | cancels | cancels | cancels | Event-level beam-charge-symmetric source in `A_BC`; may leave charge-asymmetric migration in `A_h`. |
| Hadronic energy scale | partial | cancels | cancels | cancels | Same logic as electron source unless beam-sign asymmetry is observed. |
| MC model swap | partial | partial | partial | partial | Must be propagated explicitly in all observables. |
| Common tracking efficiency | partial | cancels | does_not_cancel | does_not_cancel | User decision frozen here. |
| Charge-dependent tracking efficiency | does_not_cancel | partial | does_not_cancel | does_not_cancel | Charge-summed `A_BC` can cancel the average part only after explicit closure. |
| Combined-track vs central-only closure | partial | partial | does_not_cancel | does_not_cancel | Treat as tracking-like source with explicit charge-channel propagation. |
| Material interactions | does_not_cancel | partial | does_not_cancel | does_not_cancel | Especially relevant for `h-` due to charge-asymmetric hadron-material interactions. |
| V0 / conversion contamination | partial | partial | partial | partial | Depends on charge composition and beam-charge composition; do not hard-code cancellation. |
| Diffractive / `x_diffr_flag` | partial | partial | partial | partial | Requires explicit propagation. |
| Photoproduction background | partial | partial | partial | partial | Requires explicit propagation. |
| QED Compton / elastic-radiative subtraction | partial | partial | partial | partial | Explicit subtraction source. |
| Beam-gas / cosmics | partial | partial | partial | partial | Explicit rejection/verification source. |
| Vertex-z (`vtxz_scan`) | partial | cancels | cancels | cancels | Beam-charge-symmetric event-selection source. |
| Empz cut | partial | cancels | cancels | cancels | Beam-charge-symmetric event-selection source. |
| Electron-ID / FidVolCut | partial | cancels | cancels | cancels | Event-level beam-charge-symmetric source. |
| QED radiative correction `C_rad` | partial | partial | partial | partial | Must be propagated separately for each asymmetry observable. |
| `nuclia_no_proton_path` | does_not_cancel | partial | partial | partial | Protons explicitly get `NucliaWeight = 1.0`; bias is missing correction on proton-like tracks. |
| NuclIA correction overall | unknown_until_tested | unknown_until_tested | unknown_until_tested | unknown_until_tested | Closure required at G11 for all four observables; MC-only. |
| Period split | does_not_cancel | does_not_cancel | does_not_cancel | does_not_cancel | Different beam periods are intrinsic to the asymmetry definitions. |
| MC statistical uncertainty | partial | partial | partial | partial | Propagate through acceptance and correction chain; no assumed cancellation. |
| Unfolding / regularization | partial | partial | partial | partial | Use covariance-aware propagation only. |

### NuclIA-specific frozen expectations
- `A_h`: NuclIA effect `O(10^-3)` absolute and **does not cancel**.
- `A_BC(charge-summed)`, `A_BC(h+)`, `A_BC(h-)`: cancellation expected to residual `O(10^-4)` if beam-symmetric; verify by MC closure, never assume.
- Closure tests are **MC-only** because `NucliaWeight = 1.0` on data by framework design.

## Blinding policy

- **`A_h`** remains blinded until G15 has passed and the systematic model is frozen.
- **All three `A_BC` variants** — `A_BC(charge-summed)`, `A_BC(h+)`, and `A_BC(h-)` — remain blinded until G15, G15.5, and the systematic program are complete.
- Internal development may use hidden-sign or offset-blinded asymmetry channels, but no unblinded central-value interpretation is allowed before the corresponding gate closes.
- Multiplicities and `phi_h` are not blinded, but any asymmetry-derived side products embedded in shared workflows must respect the asymmetry blinding policy.

## Representative code-change snippets to anchor implementation

These snippets are included to make the plan implementation-ready. They are examples of the exact logic expected in the eventual code changes; they are **not** implementation work performed here.

### Multiplicity formula with explicit bin-width division
```python
mh = n_h_corr / (n_dis_corr * dz * dpt_h)
```

### Numerical zero-hadron DIS accounting artifact
```python
zero_hadron = n_dis_from_distot - n_unique_events_with_hadron
ratio_zero = zero_hadron / max(expected_zero_hadron, 1)
assert abs(ratio_zero - 1.0) < 1e-3
```

### Charge-separated acceptance with no symmetrization
```python
acc_hplus = reco_matched_hplus_genbinned / gen_hplus
acc_hminus = reco_matched_hminus_genbinned / gen_hminus
acc_ratio = acc_hplus / acc_hminus
```

### A_h must be formed from corrected per-charge yields
```python
a_h = (nplus_corr - nminus_corr) / (nplus_corr + nminus_corr)
```

### Charge-separated A_BC channels (current default = lumi-weighted convention until supervisor changes it)
```python
abc_hplus = (n_ep_hplus_corr / lumi_ep - n_em_hplus_corr / lumi_em) / (n_ep_hplus_corr / lumi_ep + n_em_hplus_corr / lumi_em)
abc_hminus = (n_ep_hminus_corr / lumi_ep - n_em_hminus_corr / lumi_em) / (n_ep_hminus_corr / lumi_ep + n_em_hminus_corr / lumi_em)
abc_sum = ((n_ep_hplus_corr + n_ep_hminus_corr) / lumi_ep - (n_em_hplus_corr + n_em_hminus_corr) / lumi_em) / ((n_ep_hplus_corr + n_ep_hminus_corr) / lumi_ep + (n_em_hplus_corr + n_em_hminus_corr) / lumi_em)
```

### Frozen MC weight prescription
```python
nuclia_safe = np.where((nuclia_weight > 0.0) & (nuclia_weight <= 1.0), nuclia_weight, 1.0)
mc_track_weight = weight1 * weight2 * nuclia_safe
```

### Mandatory hard failure on missing GenElecPhi
```python
if not np.all(np.isfinite(gen_elec_phi)):
    raise RuntimeError("GenElecPhi missing or non-finite; silent fallback to phi_e_gen=0 is forbidden")
```

### Mandatory join-on-value for matched gen lookup
```python
# reco side: GenMatchIdx is a value, not a position
matched = gen_table.merge(reco_table, left_on=["RunNumber", "EventNumber", "GenIdx"],
                          right_on=["RunNumber", "EventNumber", "GenMatchIdx"])
```

### Correlated-source covariance accumulation
```python
cov_total = cov_model + cov_tracking + cov_unfold + cov_nuclia
sigma_total = np.sqrt(np.diag(cov_total))
```

## Tasks per gate

### G1A. High-Q2 v5b production accounting freeze
**Status:** PASS.
1. **Freeze the authoritative per-run MC manifest, corrected exclusion ledger, and GRL manifest.**
   - Agent: `h1-data-validator`
   - Evidence: `validation_highQ2_mc_manifest.json`, `grl_manifest_highQ2.json`
2. **Carry forward the corrected MC nominal set definition (364 RAPGAP + 386 DJANGOH PASS files).**
   - Agent: `h1-data-validator`
3. **Preserve wrong-schema merged MC quarantine and no-data status.**
   - Agent: `h1-data-validator`

### G1B. v5b MC re-merge or multi-file decision
**Status:** PENDING.
1. **Freeze whether downstream execution uses fresh merged v5b files or explicit per-run uproot iteration.**
   - Agent: `h1-production`
2. **If merged files are produced, validate schema `86/50/49`; otherwise publish the per-run nominal-inputs manifest as canonical.**
   - Agent: `h1-data-validator`

### G1C. ep0607 v5b HighQ2 SIDIS DATA production
**Status:** BLOCKED / retry in flight.
1. **Submit the ep0607 v5b HighQ2 SIDIS data campaign using the frozen GRL `nc_goodruns_0607.root`.**
   - Agent: `h1-production`
2. **Write per-run files to DUST. Outputs stay on DUST** (dCache scratch not reliably accessible as of 2026-05-22).
   - Agent: `h1-production`
3. **Validate produced ep0607 data files against schema `86/-/49` and reconcile entry counts with the 8091-run GRL set.**
   - Agent: `h1-data-validator`

### G2. High-Q2 v5b schema and branch-population validation
**Status:** MC PASS / data BLOCKED.
1. **Freeze MC-side schema validation verdict `86/50/49` across the full per-run set.**
   - Agent: `h1-data-validator`
2. **Cite `IsV0DaughterGen` as the 50th `ChargedTotGen` branch and `isMC` as the 49th `DisTot` branch.**
   - Agent: `h1-root-analyst`
3. **Record that `Q2es == Q2e` and `Q2esGen == Q2eGen` bit-identically at high-Q2.**
   - Agent: `h1-root-analyst`
4. **Data-side schema validation remains pending G1C.**
   - Agent: `h1-data-validator`

### G3. Species/matching/gen-truth audit at high-Q2
**Status:** MC PASS.
1. **Freeze the species verdict `{pi, K, p}` exclusive on `ChargedTotGen`.**
   - Agent: `h1-root-analyst`
2. **Freeze one-to-one matching verdict and `ParticleSource_cg` presence.**
   - Agent: `h1-root-analyst`
3. **Freeze the canonical gen-pool / matching definitions by citing `reference-gen-reco-matching-charged-sidis.md`.**
   - Agent: `h1-root-analyst`

### G4. DIS event accounting, event-selection freeze, and trigger treatment
**Status:** MC PASS / data BLOCKED.
1. **Freeze the MC-side `N_DIS` zero-hadron accounting PASS verdict.**
   - Agent: `h1-cross-section`
2. **Freeze the corrected MC trigger prescription: no S67 cut, no `TrigWeightAC`, MC weight = `weight1 * weight2 * NucliaWeight_clipped`.**
   - Agent: `h1-normalization`
3. **Freeze the data trigger prescription: S67 cut required, weight = `TrigWeightAC`.**
   - Agent: `h1-normalization`
4. **Run data-side G4 only after G1C.**
   - Agent: `h1-cross-section`

### G5. Occupancy + purity + binning study at high-Q2
**Status:** PASS.
1. **Freeze Scheme HQ-A as the nominal high-Q2 4D grid.**
   - Agent: `h1-acceptance`
2. **Freeze HQ-B as contingency-only for `A_BC` if the G15 statistics trigger fires.**
   - Agent: `h1-asymmetry`
3. **Freeze z as the binding dimension and the purity-threshold rule for G8/G12.**
   - Agent: `h1-acceptance`

### G6. Hadron acceptance `eps_h(Q2,x,z,pT_h)`
**Status:** MC PASS.
1. **Freeze the symmetric event-cut prescription as a mandatory acceptance criterion.**
   - Agent: `h1-acceptance`
2. **Freeze the `GenMatchIdx` join-on-value prescription for all matched-gen lookups.**
   - Agent: `h1-acceptance`
3. **Freeze the MC-side PASS verdict: all eps in `(0,1]`, no clipping, charge ratio median ~1.**
   - Agent: `h1-data-validator`
4. **Sub-acceptance breakdown (geometric / matching / track-quality) remains follow-up work.**
   - Agent: `h1-acceptance`

### G7. DIS-event acceptance `eps_DIS(Q2,x)`
**Status:** MC CAN LAUNCH / data BLOCKED.
1. **Compute MC-side `eps_DIS(Q2,x)` from `DisTot`, using reco DIS variables for the numerator and gen DIS variables for the denominator.**
   - Agent: `h1-acceptance`
2. **Apply the symmetric event-cut prescription to the DIS-event acceptance chain.**
   - Agent: `h1-acceptance`
3. **Launch data-side `eps_DIS` only after G1C and G4(data).**
   - Agent: `h1-acceptance`

### G8. Correction-method choice per bin
**Status:** PLANNABLE NOW.
1. **Classify each HQ-A bin as `bin_by_bin`, `unfold`, or `mask` using the frozen purity threshold.**
   - Agent: `h1-unfolder`
2. **Record explicitly that z is the binding dimension for the unfolding-rim bins.**
   - Agent: `h1-unfolder`

### G9. Multiplicity extraction `M^{h+/-}`
**Status:** MC-internal UNBLOCKED / final data BLOCKED.
1. **Compute internal preliminary `M^{h+/-}(Q2,x,z,pT_h)` on MC using HQ-A, the frozen gen-pool, the frozen matching definition, the symmetric event-cut prescription, and the frozen MC weight prescription.**
   - Agent: `h1-cross-section`
2. **State explicitly in outputs that any multiplicity is internal preliminary until G14 passes.**
   - Agent: `h1-cross-section`
3. **Final data-bearing multiplicities remain blocked on G1C.**
   - Agent: `h1-cross-section`

### G10. High-Q2 `C_rad` estimation
**Status:** PENDING.
1. **Await RAD/NORAD availability check (Task Q feeder).**
   - Agent: `h1-radiative-correction`
2. **Keep the charge-summed and charge-separated `A_BC` `C_rad` requirements unchanged.**
   - Agent: `h1-radiative-correction`

### G11. High-Q2 systematics evaluation
**Status:** BLOCKED overall; NuclIA sub-gate advanced.
1. **Keep G11-LAr blocked until the supervisor selects Option A/B/C.**
   - Agent: `supervisor decision required`
2. **Replace the retired blending-closure language with on/off MC closure on multiplicities and asymmetries, charge-cancellation closure, and sentinel-clip robustness.**
   - Agent: `h1-systematics`
3. **Freeze the builder-side NuclIA application prescription: stored-only, applied once downstream.**
   - Agent: `h1-software`
4. **Freeze builder-side dispatch: pion default, matched K -> kaon, matched p -> 1.0, unmatched -> pion default.**
   - Agent: `h1-software`
5. **Implement a downstream clip guard for NucliaWeight outside `(0,1]` and record clip counts.**
   - Agent: `h1-systematics`
6. **Evaluate `nuclia_no_proton_path` explicitly as a systematic source.**
   - Agent: `h1-systematics`

### G12. Unfolding for 4D multiplicities
**Status:** PLANNABLE NOW.
1. **Launch 4D unfolding development on the ~26% of populated bins with 4D purity < 0.5.**
   - Agent: `h1-unfolder`
2. **Use z as the primary binding dimension in regularization / response diagnostics.**
   - Agent: `h1-unfolder`

### G13-prep. phi_h hardening and verification
**Status:** DONE.
1. **Treat the missing-`GenElecPhi` raise patch as complete.**
   - Agent: `code-editor`
2. **Treat convention-metadata stamping as part of the completed hardening pass.**
   - Agent: `code-editor`
3. **Carry forward the high-Q2 direct-`GenElecPhi` path as frozen.**
   - Agent: `h1-analysis-critic`

### G13. High-Q2 `phi_h` azimuthal moments
**Status:** PARTIALLY UNBLOCKED.
1. **Use stored `GenElecPhi` directly; do not revive the low-Q2 reco-phi proxy hack.**
   - Agent: `h1-acceptance`
2. **Cite the convention-freeze critique and patched-script state as the frozen high-Q2 phi_h basis.**
   - Agent: `h1-analysis-critic`
3. **Proceed with high-Q2 `phi_h` extraction once the remaining resolution/package tasks are scheduled.**
   - Agent: `h1-plotting`

### G14. Normalization validation gate
**Status:** BLOCKED.
1. **Run `oomclumi` or validated equivalent on the HAT+MODS input chain to determine effective v5b MC luminosity.**
   - Agent: `h1-normalization`
2. **Carry forward the numerical closure criteria from r4 unchanged.**
   - Agent: `h1-analysis-critic`

### G15. e- period recovery gate
**Status:** BLOCKED on G1C.
1. **Do not start full e- recovery logic until the ep0607 base data path exists.**
   - Agent: `h1-production`
2. **Keep the statistics contingency trigger and the e+/e- lumi-ratio policy question unchanged.**
   - Agent: `h1-asymmetry`

### G15.5. `A_BC` physics validation gate
**Status:** BLOCKED on G11 + G15.
1. **Retain the lumi-weighted asymmetry formula as the current default until the supervisor chooses otherwise.**
   - Agent: `planner`
2. **Keep TPE / gamma-Z references and unblinding sign-off as explicit supervisor questions.**
   - Agent: `researcher`

### G16. First internal high-Q2 convergence package
**Status:** BLOCKED.
1. **Package waits on G9 (MC-only path at minimum), G11 (NuclIA closure at minimum), G13, G14 for any absolute claim, and G15 for any A_BC claim.**
   - Agent: `planner`

### G17. Standalone-paper publication-grade packaging
**Status:** BLOCKED.
1. **No HEPData planning enters this revision.**
   - Agent: `planner`

## Risk register

| Risk | Severity | Trigger | Mitigation | Owning gate |
|---|---|---|---|---|
| Absolute cross-section released before normalization validated | high | Any package or plot uses absolute-normalization language before `highQ2_normalization_validation.json` is PASS | Hard-block all absolute cross-section work behind G14 | G14 |
| `A_h` released from ep0607 only | high | High-Q2 `A_h` shown as a physical asymmetry without recovered e- periods | Keep `A_h` blocked behind G15 and state explicitly that ep0607-only `A_h` is biased, not just imprecise | G15 |
| `A_BC` released without gamma-Z / TPE separation | high | A measured `A_BC` is interpreted as TPE-sensitive without inclusive-DIS baseline validation | Require G15.5 and explicit hadronic-vs-inclusive separation before any release | G15.5 |
| **G11 LAr procedure does not exist upstream; G11-LAr is NEW, not REUSE** | high | Attempt to close G11 by assuming proton_HiQ / lambda_HiQ already solved the procedure | Supervisor must choose Option A/B/C and document it; no G11 PASS before that choice | G11-LAr |
| **No v5b HighQ2 SIDIS DATA on disk for any period** | high | Any downstream data-side gate assumes ep0607 data exists | Immediate G1C launch; block all data-side gates until ep0607 v5b data is produced and validated | G1C |
| Placeholder `C_rad` ships in the internal package without bounds | high | G16 contains observables whose `C_rad` remains undefined | Make G10 a dependency of G16 and explicitly exclude unsupported observables | G10 / G16 |
| Acceptance > 1 trap | high | Any retained bin has `eps_h > 1` | Surface and diagnose as a bug; never clip to 1 | G6 |
| `eps_DIS` denominator missing zero-hadron events | high | `N_DIS` derived from track trees or zero-hadron events absent from denominator | Require numerical zero-hadron validation at G4 and G7 | G4 / G7 |
| Bin-by-bin correction used where purity < 0.5 | high | Unfolding-required bins are silently corrected bin-by-bin | Freeze method map in G8 with per-bin enforcement | G8 |
| HEPData scope creep re-enters the plan | medium | Publication packaging expands into auxiliary-data tasks not approved in r5 | Keep HEPData explicitly out of scope in G17 and revision history | G17 |
| Trigger weight mis-propagated into MC acceptance | high | `TrigWeightAC` applied to MC or absent from data | Verify data-only trigger weighting at G4 and track trigger source in G11 | G4 / G11 |
| Recovered e- sample passes production checks but fails physics sanity checks | high | Period files exist but yield-per-luminosity or inclusive control `A_BC` is inconsistent | Enforce numerical G15 checks before asymmetry release | G15 |
| `A_BC` 4D statistics insufficient on the nominal grid | medium | After full e- recovery, more than 20% of populated 4D bins have e- yield < 25 events | Invoke contingency coarser grid for `A_BC` only; keep multiplicities and `A_h` on the nominal grid | G15 |
| Charge-separated propagation error in `A_BC(h+)` / `A_BC(h-)` | high | Implementation reuses charge-summed correction logic or wrongly assumes tracking cancellation in fixed-charge channels | Require explicit per-charge propagation, separate cancellation columns, and asymmetry-channel closure checks | G10 / G11 / G15.5 |
| **Existing merged MC files are wrong-schema v4 carryover** | medium | Any downstream script reads `/pnfs/.../highQ2_merged/merged_highQ2_mc_*.root` by mistake | Quarantine wrong-schema merged files and re-merge or freeze multifile iteration in G1B | G1A / G1B |
| **NuclIA no-proton-path bias affects `A_h`** | medium | Protons explicitly get `NucliaWeight = 1.0` and this is ignored in the A_h budget | Add `nuclia_no_proton_path` as explicit systematic; verify cancellation only in `A_BC` closure | G11-NuclIA |
| **e+/e- lumi ratio = 1.193; `A_BC` definition choice matters** | low | Reweighting and lumi-weighted asymmetry definitions are mixed inconsistently | Keep current default as lumi-weighted until supervisor decision; record the chosen convention in G15.5 outputs | G15.5 |
| **`TrigWeightAC` misapplied on MC** | high | Any downstream script multiplies `TrigWeightAC` on MC, importing weights up to 524288 | Freeze MC weight prescription and review all weighting paths | G4 / G9 / G11 |
| **Asymmetric event-cut bug** | high | Any script applies reco-event cuts to numerator but only gen-event cuts to denominator | Freeze symmetric event-cut prescription and review all acceptance / multiplicity / asymmetry paths | G6 / G7 / G9 / G13 |
| **`GenMatchIdx` misread as positional index** | high | Any downstream script treats `GenMatchIdx` as a positional offset instead of a `GenIdx` value | Freeze join-on-value prescription and review all matched-gen lookup code | G6 / G9 / G11 |
| **NucliaWeight sentinel anomaly** | low | Rare finite Fortran return >1 produces `NucliaWeight < 0` | Clip to `[0,1]` with default `1.0`, record counts, and queue a v5c builder fix | G11-NuclIA |
| **Proton naming legacy confuses contributors** | low | New contributor reads proton-prefixed files/binary names and assumes this is the proton asymmetry analysis | Emit naming-alias reference and perform Tier 0a + 1 + 2 naming cleanup | G1A / next-batch L |
| **Active code still references removed `ProtonTot` / `ProtonTotGen` trees** | low | Shifted-MC / merge scripts silently expect removed trees | Patch `produce_shifted_mc.py`, `produce_shifted_mc.C`, `merge_and_validate.py` | next-batch K |

## Open questions and decisions required from the user / supervisor

1. **G11-LAr option decision:** which path should the supervisor approve?
   - **Option A:** develop the procedure inside SIDIS using `H1CalcSystematic` directly;
   - **Option B:** drive `lambda_HiQ` Phase 4 WP6 to closure first and inherit later;
   - **Option C:** coordinate with the broader H1 high-Q2 community / prior published analyses to extract an established procedure.
2. **`A_BC` weighting policy:** should the asymmetry remain in the current default **lumi-weighted rate form**, or should one beam-sign sample be rescaled so the numerator is luminosity-symmetric by construction? The recovered e+/e- luminosity ratio is **1.193**, not 1.000.
3. **TPE / gamma-Z citations:** which specific arXiv references should be frozen for the TPE expectation and gamma-Z interference baseline at high `Q2`?
4. **Unblinding sign-off:** who is the sign-off person or committee for `A_BC` unblinding?
5. **Trigger anchor wording:** please confirm the corrected prescription text: **MC events do not cut on trigger and are not weighted by `TrigWeightAC`; data events cut on S67 and are weighted by `TrigWeightAC`.**
6. **Naming remediation tier set:** recommended execution set is **Tier 0a + Tier 1 + Tier 2 now; defer Tier 3; reject Tier 4**. Approve or modify?
7. **`analyze_proton_highQ2.py` on NAF:** should Task O treat it as stale until proven active, or is it known to be part of a live auxiliary workflow?
8. **v5c builder fix queue:** should we open and maintain a deferred queue for builder-level follow-ups such as the `NucliaWeight` sentinel guard and the `Q2sGen=0` cleanup, even if no immediate builder revision is authorized?
9. **Publication scope of `A_h`:** once G15 closes, should `A_h` remain in the eventual publication-facing high-Q2 paper, or should publication scope focus on multiplicities, `phi_h`, and the `A_BC` family only?
10. **If the `A_BC` contingency trigger fires, should the coarser fallback keep full 4D dimensionality with fewer bin edges, or collapse only the weakest dimension?**

## Work status: completed first-wave / second-wave / next-batch

### Completed through second-wave tasks
- **Task A COMPLETE** — MC `N_DIS` prototype and zero-hadron closure:
  - `sidis_lowQ2/docs/data/n_dis_prototype_highQ2_mc.json`
  - `sidis_lowQ2/docs/results/n_dis_prototype_highQ2_mc.md`
- **Task B COMPLETE** — trigger semantics reference:
  - `sidis_lowQ2/docs/references/reference-trigger-semantics-highQ2-sidis.md`
- **Task C COMPLETE** — NuclIA builder/application audit:
  - `sidis_lowQ2/docs/references/reference-nuclia-correction-highQ2-sidis.md`
- **Task D COMPLETE** — phi_h hardening / convention-prep wave
  - `sidis_lowQ2/docs/reviews/critique-phih-convention-freeze-highQ2-sidis.md`
  - patched-script state treated as complete session output for G13-prep
- **Task E COMPLETE** — high-Q2 binning study:
  - `sidis_lowQ2/docs/data/binning_study_highQ2.json`
  - `sidis_lowQ2/docs/results/binning_study_highQ2.md`
- **Task F COMPLETE** — MC acceptance prototype:
  - `sidis_lowQ2/docs/data/acceptance_highQ2_charged_mc_prototype.json`
  - `sidis_lowQ2/docs/results/acceptance_highQ2_charged_mc_prototype.md`
- **Task H COMPLETE** — corrected MC manifest:
  - `sidis_lowQ2/docs/data/validation_highQ2_mc_manifest.json`
- **Task I COMPLETE** — canonical matching / gen-pool reference:
  - `sidis_lowQ2/docs/references/reference-gen-reco-matching-charged-sidis.md`
- **Naming audit J COMPLETE** — legacy proton naming audit:
  - `sidis_lowQ2/docs/reviews/audit-proton-naming-highQ2-sidis.md`
- **Task 4 COMPLETE** — full MC schema sweep:
  - `sidis_lowQ2/docs/data/validation_highQ2_mc_schema_full.json`
- **Task 5 COMPLETE** — `Q2es == Q2e` verification:
  - `sidis_lowQ2/docs/data/validation_highQ2_q2es_vs_q2e.json`
- **Task 6 COMPLETE** — species/matching verification:
  - `sidis_lowQ2/docs/data/validation_highQ2_mc_species_matching.json`

### In-flight / retry status
- **Task G / Task N retry in flight** — ep0607 v5b SIDIS data production (G1C)

### Next-batch tasks now tracked
| Task | Purpose | Status |
|---|---|---|
| K | Patch active code paths that still reference removed `ProtonTot` / `ProtonTotGen` trees (`produce_shifted_mc.py`, `produce_shifted_mc.C`, `merge_and_validate.py`) | launched / in flight |
| L | Emit naming-alias reference and perform naming-remediation Tier 1/2 prose cleanup | launched / in flight |
| M | This r5 plan revision and status sync | COMPLETE by this revision |
| N | Retry / continue G1C ep0607 v5b SIDIS data production launch | in flight |
| O | Investigate whether `analyze_proton_highQ2.py` on NAF is stale or active | launched / in flight |
| P | Determine effective v5b MC luminosity / `oomclumi` path for G14 | launched / in flight |
| Q | RAD/NORAD availability / `C_rad` feeder work for G10 | launched / in flight |

## Definition of done for this plan

This plan is considered **done and ready to retire** when one of the following states is reached:

1. **Publication-grade high-Q2 multiplicity + `phi_h` path closed:**
   - G1A through G17 are complete for multiplicities and `phi_h`,
   - G13 has passed,
   - G14 has passed for multiplicities,
   - a standalone-paper publication package manifest exists,
   - and the next planning artifact should shift to publication-prep / review-response work.

2. **Publication-grade multiplicities + `phi_h` plus one or both asymmetry tracks closed:**
   - all multiplicity + `phi_h` conditions above are met,
   - `A_h` enters only after G15 + blinding release,
   - `A_BC(h+)`, `A_BC(h-)`, and `A_BC(charge-summed)` enter only after G15 + G15.5 + blinding release,
   - and the standalone-paper scope is frozen.

3. **Internal-only standalone-paper convergence package finalized:**
   - G16 is complete,
   - G14 and/or G15 and/or G15.5 remain unresolved and the user accepts an internal-only high-Q2 package pending blocker closure,
   - and a follow-on plan is needed specifically for normalization closure, period recovery, LAr-systematics closure, asymmetry promotion, or publication packaging.

At that point this document should be superseded by a **high-Q2 publication-prep plan** or a **high-Q2 blocker-closure plan**, depending on the reached state.

## Annotation cycle status
- Iteration: 5
- Covers: High-Q2 charged-hadron SIDIS only (`Q2 > 150 GeV2`) for a standalone paper, with multiplicities, `phi_h`, `A_BC(h+)`, `A_BC(h-)`, and `A_BC(charge-summed)` in headline internal-package scope, and `A_h` blocked behind G15 + blinding.
- Execution-state update: r5 converts second-wave task findings into frozen prescriptions and flips the major MC-side early gates to PASS / plannable while keeping the critical blockers explicit: no v5b data path yet, no chosen LAr path yet, no MC luminosity yet.
- Open decisions remain: G11-LAr option A/B/C, `A_BC` weighting convention, TPE/gamma-Z citation set, unblinding sign-off authority, trigger-anchor wording confirmation, naming-remediation tier approval, status of `analyze_proton_highQ2.py`, whether to open a v5c builder-fix queue, publication inclusion of `A_h`, and the exact contingency-shape policy if the `A_BC` statistics trigger fires.

## Todo
- [x] G1A / Freeze the authoritative per-run high-Q2 v5b MC manifest and quarantine the wrong-schema merged MC files.
- [x] G1A / Freeze the RAPGAP dedup rule (`proton_highQ2_mc_rapgap31_*` only), the 100-empty-file DJANGOH exclusion list, and the corrected period mapping.
- [x] G2(MC) / Validate representative high-Q2 per-run MC files against the frozen v5b schema `ChargedTot=86`, `ChargedTotGen=50`, `DisTot=49` across the full sweep.
- [x] G2(MC) / Identify the 50th `ChargedTotGen` branch as `IsV0DaughterGen` and the 49th `DisTot` branch as `isMC`.
- [x] G2(MC) / Verify `Q2es ≡ Q2e` and `Q2esGen ≡ Q2eGen` bit-identically at high-Q2.
- [x] G3(MC) / Confirm the high-Q2 species set is exclusively `abs(PDG) in {211,321,2212}`.
- [x] G3(MC) / Verify one-to-one reco-gen matching and `ParticleSource_cg` presence.
- [x] G4(MC) / Build the MC-only `N_DIS(Q2,x)` prototype from `DisTot`, explicitly including zero-hadron DIS events.
- [x] G4(MC) / Verify `nChargedReco==0` agrees exactly with the join-derived zero-hadron count.
- [x] G5 / Freeze Scheme HQ-A and the purity-driven bin-by-bin vs unfolding rule.
- [x] G6(MC) / Compute charge-separated high-Q2 hadron acceptance maps with the symmetric event-cut prescription and the `GenMatchIdx` join-on-value prescription.
- [x] G6(MC) / Verify all populated bins satisfy `eps_h in (0,1]` with no clipping.
- [x] G11-NuclIA / Verify builder-side NuclIA policy is stored-only, not pre-applied, and that matched protons get `NucliaWeight=1.0`.
- [x] G13-prep / Treat missing-`GenElecPhi` raise hardening and convention-freeze prep as complete session outputs.
- [ ] G1B / Freeze whether downstream execution uses fresh merged v5b files or explicit per-run uproot iteration.
- [ ] G1C / Submit ep0607 v5b HighQ2 SIDIS DATA production immediately using `nc_goodruns_0607.root`.
- [ ] G1C / Route ep0607 per-run data to DUST and keep on DUST (dCache scratch not reliably accessible as of 2026-05-22).
- [ ] G1C / Validate ep0607 produced data against schema `86/-/49` and reconcile entry counts with the 8091-run GRL.
- [ ] G2(data) / Run data-side schema validation once G1C lands.
- [ ] G4(data) / Run the data-side zero-hadron accounting only after G1C and G2(data) pass.
- [ ] G7(MC) / Compute `eps_DIS(Q2,x)` from `DisTot` using reco-vs-gen DIS variables with the symmetric event-cut prescription.
- [ ] G7(data) / Launch the data-side `eps_DIS` only after G1C and G4(data) pass.
- [ ] G8 / Freeze a per-bin method map (`bin_by_bin`, `unfold`, `mask`) from the HQ-A purity structure.
- [ ] G9(MC-internal) / Compute internal preliminary `M^{h+/-}(Q2,x,z,pT_h)` on MC using HQ-A, the frozen gen-pool, matching, symmetric event-cut prescription, and the MC weight prescription.
- [ ] G9 / State explicitly in outputs that any multiplicity result before G14 is internal preliminary.
- [ ] G10 / Check whether usable high-Q2 RAD and NORAD MC inputs exist for multiplicity `C_rad`.
- [ ] G10 / Produce nominal or bounded multiplicity `C_rad`, or explicitly exclude unsupported observables from G16.
- [ ] G10 / Build separate `C_rad` treatments for `A_BC(charge-summed)`, `A_BC(h+)`, and `A_BC(h-)`.
- [ ] G11-LAr / Supervisor selects Option A, B, or C for the high-Q2 LAr electron-energy / angle procedure.
- [ ] G11 / Implement the approved LAr option; do not copy the low-Q2 SpaCal procedure.
- [ ] G11 / Run high-Q2 cut and model systematics, including `vtxz_scan`, `Empz` scan, trigger verification, photoproduction, QEDC / elastic-radiative subtraction, beam-gas/cosmics, and diffractive handling.
- [ ] G11-NuclIA / Implement downstream `NucliaWeight` clipping to `[0,1]` with default `1.0`, and record clip counts.
- [ ] G11-NuclIA / Run MC-only NuclIA on/off closure checks in `M^{h+/-}`, `A_h`, `A_BC(charge-summed)`, `A_BC(h+)`, and `A_BC(h-)`.
- [ ] G11-NuclIA / Evaluate `nuclia_no_proton_path` explicitly as a systematic source.
- [ ] G11 / Produce the four asymmetry-cancellation columns in the systematics matrix.
- [ ] G11 / Produce covariance handling for shared MC samples and forbid pure quadrature on correlated statistical deltas.
- [ ] G12 / Run high-Q2 unfolding on bins marked `unfold` with z as the binding dimension.
- [ ] G12 / Perform regularization scans and retain the nominal setting with closure evidence.
- [ ] G12 / Require refolding / closure at the 5% level per unfolded bin and export the unfolding covariance.
- [ ] G13 / Freeze the high-Q2 `phi_h` Trento convention and verify consistency with low-Q2 convention.
- [ ] G13 / Extract high-Q2 `phi_h` azimuthal moments for the first internal package.
- [ ] G13 / Produce high-Q2 `phi_h` plots and summaries for G16.
- [ ] G14 / Run `oomclumi` or validated equivalent on HAT+MODS to determine effective v5b MC luminosity.
- [ ] G14 / Validate `H1CalcWeight` / effective MC luminosity semantics on the actual nominal file set.
- [ ] G14 / Compare the inclusive DIS control quantity against the named high-Q2 NC reference with the 3% per-bin / 1.5% integrated tolerance.
- [ ] G15 / Recover `ep0304`, `em0405`, and `em06` in the high-Q2 SIDIS data chain after the ep0607 base path is validated.
- [ ] G15 / Validate recovered period-tagged outputs and updated luminosity accounting.
- [ ] G15 / Run per-period inclusive DIS and charged-track yield-per-luminosity sanity checks with the 5% or statistical tolerance.
- [ ] G15 / Confirm the recovered sample yields an inclusive control `A_BC(charge-summed)` consistent with gamma-Z interference at the per-mille level within uncertainty.
- [ ] G15 / Audit 4D `A_BC` statistics and trigger the contingency coarser `A_BC` grid only if more than 20% of populated bins have e- yield < 25 events.
- [ ] G15 / State explicitly in the recovery artifact that ep0607-only `A_h` is biased, not just low precision.
- [ ] G15.5 / Freeze the `A_BC` observable definitions for `A_BC(h+)`, `A_BC(h-)`, and `A_BC(charge-summed)`.
- [ ] G15.5 / Freeze the `A_BC` weighting convention once the supervisor decides between lumi-weighted and reweighted formulations.
- [ ] G15.5 / Freeze the `A_BC` blinding policy and unblinding conditions for all three `A_BC` variants.
- [ ] G15.5 / Freeze the TPE and gamma-Z reference citation set.
- [ ] G15.5 / Freeze the unblinding sign-off person or committee.
- [ ] G16 / Assemble the first internal high-Q2 standalone-paper package with multiplicities, `phi_h`, and complete caveats.
- [ ] G16 / Include `A_BC(h+)`, `A_BC(h-)`, and `A_BC(charge-summed)` only if G15 + G15.5 + blinding release are complete.
- [ ] G16 / Keep `A_h` deferred unless G15 + blinding release are complete.
- [ ] Next-batch K / Patch active code paths still referencing removed `ProtonTot` / `ProtonTotGen` trees.
- [ ] Next-batch L / Emit `reference-naming-aliases-highQ2-sidis.md` and perform Tier 1/2 naming cleanup.
- [ ] Next-batch O / Determine whether `analyze_proton_highQ2.py` on NAF is stale or active.
- [ ] G17 / Build the standalone-paper publication package only for observables whose gates have passed.
- [ ] G17 / Exclude HEPData tasks from this plan revision’s publication packaging.

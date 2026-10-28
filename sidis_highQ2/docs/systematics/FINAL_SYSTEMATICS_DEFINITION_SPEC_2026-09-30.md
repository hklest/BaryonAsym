# Final systematic-uncertainty definition specification — v6/z005

**Date:** 2026-09-30  
**Scope:** H1 high-Q2 inclusive charged-hadron SIDIS only; v6/z005, IBU/BBB finalcandidate, 490 published bins (`245 h+ + 245 h-`).  
**Status:** **NOT FINAL / NOT PUBLICATION-GRADE.** This is the controlling definition for closing the budget. It does not alter the central release or regenerate any number.

## 0. Authority, release contract, and terminology

The numerical central-result authority is
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`.
The published ordering is fixed by `(charge, bin_index)`: all 245 `h+` bins followed by all 245 `h-` bins. The current historical total table is
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-07-03_with_tracking/total_uncertainty_summary_with_tracking.csv`.
It has **490 data rows** and its 10 numerical component columns are exactly:

`ibu_stat_rel`, `ibu_model_rel`, `ibu_reg_rel`, `hfs_jes_rel`, `hfs_rces_rel`, `hfs_theta_rel`, `elec_energy_rel`, `elec_theta_rel`, `lumi_rel`, and `tracking_eff_m0p5_rel`.

“Implemented” below means an auditable numerical artifact exists on the 490-bin basis. It does **not** mean accepted for the final physics release. “Provisional” means a value exists but fails this specification's final methodological conditions. “Blocked” means no value may enter a final total; it is not zero. “Dropped” means excluded from the baseline because it is inapplicable or has no pinned prescription. The counts in §6 apply only to the numbered register rows, not to cross-checks or exclusions mentioned elsewhere.

The frozen v6sys menu is the authority for physical magnitudes and propagation design. A historical table or analysis-note statement cannot override it. `STATE.md` remains authority for current blockers, including unapplied/provisional `C_rad`, background response consistency, response-truth conservation, and the IBU/BBB-versus-native-v6-TUnfold decision.

## 1. Source register

| # / total-CSV column or required column | Physical definition and frozen magnitude / H1 precedent | Propagation; sides; final symmetrisation | Charge / bin correlation model | Status and exact condition |
|---|---|---|---|---|
| 1 `ibu_stat_rel` | Data-statistical IBU uncertainty; current covariance uses 1000 data-Poisson toys (historical table used 100). This is statistical, not a detector systematic. | Re-unfolded toy covariance; no ± nuisance. | Charge cross-block is **ZERO BY ASSUMPTION**; within-charge covariance measured by toys. | **IMPLEMENTED.** Final treatment remains conditional on measuring/justifying the charge cross-block. |
| 2 `ibu_model_rel` | Response-model / cross-closure: `0.5 abs[IBU(R_DJANGOH)-IBU(R_RAPGAP)]`; Andreev 2024 §6.4 precedent. | Alternate response/unfolding; two-model half-difference. | Charge residuals correlated only as measured from the two charge-resolved variations; current diagonal fallback is not final. | **PROVISIONAL.** RAPGAP response/provenance disposition and covariance representation remain open. |
| 3 `ibu_reg_rel` | IBU regularisation variation around nominal `k=4`; frozen definition is half-difference of the prescribed up/down regularisation choices (the note's “±1 iteration” is an implementation statement, not a substitute definition). | Re-unfold each regularisation side; use `abs[(X_up-X_down)/2]` after stability validation. | Measured signed response if coherent; otherwise diagonal fallback. | **IMPLEMENTED** numerically in the current table; final covariance needs signed sidecar/validation on the v6 basis. |
| 4 `hfs_jes_rel` | `eHadEn`: ±1% only for HFS objects inside kT `R=1.0`, `pT>5 GeV`, `-1.5<eta_lab<2.75` jets. Andreev 2023 PLB 844 §4; Andreev 2024 EPJC 84 §6.2. | Migration-affecting. Required paired Stage1+Stage2 shifted reco data and MC, shifted response, and unfold for ± sides. | Expected charge-correlated; correlation across bins is measured after noise-floor test, otherwise diagonal. | **PROVISIONAL.** Current table uses one-sided `hfs_jes_down`; it is not evidence of the frozen paired ± implementation or classifier audit. |
| 5 `hfs_rces_rel` | `eHadEnRCES`: ±1% for HFS objects outside those jets; independent of JES. Same Andreev 2023/2024 precedent. | Migration-affecting paired Stage1+Stage2 shifted-response ± reruns. | Expected charge-correlated; measured or temporary diagonal bin model. | **PROVISIONAL.** Current table uses one-sided `hfs_rces_up`; verify the relvol10 enum/classifier and run both sides. |
| 6 `hfs_theta_rel` | `eHadTh`: ±1 mrad HFS polar-angle alignment; Andreev 2024 §6.1. | Migration-affecting paired ± shifted-response reruns. Final rule is half-difference only after response/noise test. | Expected charge-correlated; measured or diagonal, never unvalidated rank-1. | **PROVISIONAL.** Current table uses `hfs_theta_up`; the audited two-sided response is even/quadratic and quarantined (see §4). |
| 7 required `hfs_phi_rel` | `eHadPh`: ±20 mrad HFS azimuth; Andreev 2023 §4 and Andreev 2024 §6.1. | Migration-affecting paired Stage1+Stage2 shifted-response ± reruns. | Expected charge-correlated; measured or diagonal. | **BLOCKED.** No paired z005 source artifact or current-table column exists. |
| 8 `elec_energy_rel` | `eElecEn`: ±0.5% **per each of five LAr wheels** (BBE, CB1+2, CB3, FB1, FB2+IF); g1j_ES `AnalysisBase.C:108–129`, anchored to arXiv:0904.3513. | Migration-affecting paired Stage1+Stage2 shifted-response ± reruns. Apply all five explicit wheel setters per side; final magnitude is coherent half-difference only if noise-qualified. | Expected charge-correlated; measured or diagonal. | **PROVISIONAL.** Table contains a historical +1% post-hoc/electron vector, about twice the frozen amplitude; covariance package explicitly calls electron vectors provisional post-hoc. Frozen rerun is required. |
| 9 `elec_theta_rel` | `eElecTh` ±1 mrad for each of three regions **co-shifted with matched-track `eElecTrTh` ±1 mrad**; g1j_ES `AnalysisBase.C:130–141`, conservative envelope from arXiv:0904.3513. | Migration-affecting paired Stage1+Stage2 shifted-response ± reruns. | Expected charge-correlated; measured or diagonal. | **PROVISIONAL.** Current +1 mrad source exists, but must prove the co-shift and paired response reruns; current electron vector is post-hoc in covariance v4. |
| 10 `tracking_eff_m0p5_rel` | Current delivered random -0.5% tracking-efficiency ensemble, 16 valid seeds (`314159–314174`); use only `delta_track_rel` / `tracking_eff_m0p5_rel`. | Post-unfolding/repeated-release ensemble; one-sided production. The RMS source magnitude is the delivered column; `seed_rms` and `seed_mean_unc` are QA-only. | Charge-independent diagonal in current package; cross-charge block zero. | **IMPLEMENTED.** It is the actual z005 table source. It is not automatically equivalent to the frozen analytic 1.0%⊕0.2% central-tracking prescription. |
| 11 required `tracking_chargedep_rel` | Charge-dependent tracking residual, fully anti-correlated h+ / h-; Aaron et al. 2009 PLB 681 §6 precedent is **2.5% on `A_h`**. | Analytic/post-unfolding only after the observable convention is fixed; derive varied charge yields then `A_h`. | Fully anti-correlated between charges; bin model must be specified from the chosen nuisance. | **BLOCKED.** Existing 0.3%/track source fails the 2.5%-on-`A_h` sanity check (`0.003/0.025=0.12`, outside the allowed 30% agreement). It must not support an `A_h` release. |
| 12 required `response_mcstat_rel` | Response-MC statistical uncertainty by response bootstrap; Andreev 2017 §6 precedent. | Rebuild/re-unfold response replicas; no analytic percentage substitution. | Measured full 490-bin covariance. | **BLOCKED.** No response-MC-statistical covariance is emitted in covariance v4; response `sumw2`, fake-column uncertainty, and response replicas are not propagated. |
| 13 required `crad_rel` | Uncertainty of the per-bin Born correction `C_rad=sigma_nonrad/sigma_rad`, including validated alternate-model treatment if adopted. | Post-unfolding multiplicative correction only after `C_rad` is approved and applied; compare approved alternatives or propagate approved statistical/model uncertainty. | Correlation must follow the actual radiative replicas/model variation; do not assume diagonal. | **BLOCKED.** `C_rad` is unapplied; DJANGOH exposure/convention and coherent RAPGAP-vs-DJANGOH choice remain open. No magnitude is invented. |
| 14 `lumi_rel` | Flat 2.7% absolute-normalisation uncertainty; Andreev 2024 §6.3 and current v6 bookkeeping. | Analytic, one magnitude. | Fully charge- and bin-correlated rank-1: `C_ij=(0.027 sigma_i)(0.027 sigma_j)`. | **IMPLEMENTED** for absolute cross sections only. Exclude from multiplicities and charge asymmetries. |
| 15 required `trigger_rel` | S67 trigger efficiency; frozen placeholder ceiling ≤0.5%, pending data/MC or monitor-trigger measurement. | Analytic efficiency only if shown bin-independent; otherwise measured efficiency variation. | Fully correlated only if flatness is demonstrated; otherwise measured. | **BLOCKED.** Current bookkeeping uses 0.5% placeholder, while older research quotes 1.2%; no final value is pinned. |
| 16 required `NucliaWeight_rel` | Envelope of the existing per-event nuclear-interaction weight variation. | Post-unfolding/reweight envelope; side definition must be retained. | Measured from its event weights; cancels in `A_h` only after charge-resolved check. | **BLOCKED.** Adoption in the multiplicity budget remains a supervisor decision; no current z005 total-table column. |
| 17 required `background_rel` | Background scale/shape uncertainty for an actually subtracted background, including track-level electron/V0/det.int/low-Q2 accounting where applicable. | Requires response-consistent treatment, not a generic post-hoc fraction. | Measured scale/shape covariance, including overlap handling. | **BLOCKED.** The former retirement is not final: its huge-bin rationale was voided by the 2026-09-25 `fLumi` pb^-1 correction. Track-level construction lacks `overlap_980` and `n_raw_980`; P-ALPHA remains open. |
| 18 required `diffvm_rel` | Diffractive vector-meson residual (rho/phi/omega) background. | Evaluate surviving template/selection and propagate only if nonzero. | Measured after evaluation. | **BLOCKED.** DIFFVM is currently an assumed zero, not an assessed zero. |
| 19 required `gammap_bkg_30pct_rel` | ±30% of a subtracted gamma-p template; arXiv:1302.1321 §5. | Analytic template-scale variation only if such a template is subtracted. | Template-derived. | **BLOCKED.** Central is raw/no-background-subtraction and no gamma-p template is consumed; document no subtraction rather than emit zero. |
| 20 `electron_phi_rel` | `eElecPh` LAr electron azimuth. | Would be migration-affecting. | Would require measured covariance. | **DROPPED.** No pinned HERA-II/SIDIS magnitude or g1j_ES recipe; no value may be invented. |
| 21 `noise_energy_rel` | `eNoiseEn` residual LAr cluster-noise energy. | Would be migration-affecting. | Would require measured covariance. | **DROPPED.** No pinned SIDIS/H1 precedent; do not use a default shift. |
| 22 `tracking_resolution_rel` | Explicit track momentum-resolution smearing. | Would be migration-affecting paired response study. | Would require measured covariance. | **DROPPED.** Proposed ±50% smearing has no H1-precedent prescription; legacy rank-1 source is critic-fatal. |
| 23 `combined_forward_tracking_10pct_rel` | 10% combined/forward track efficiency from arXiv:1302.1321 §5. | Not applicable. | Not applicable. | **DROPPED.** The published hadron fiducial is within central tracking (`abs(eta_h)<1.3`); do not add it. |
| 24 `method_spread_BBB_IBU_TUnfold_rel` | Difference among BBB, IBU, and TUnfold. | Diagnostic comparison only. | Not a covariance source. | **DROPPED.** Method spread is coverage/validation, not a systematic. The nominal-method decision itself remains a release blocker. |

### Register rules

1. A frozen migration-affecting row cannot become IMPLEMENTED merely because a one-sided release or a post-hoc vector exists. It needs the paired Stage1+Stage2 reruns, shifted reco data plus shifted reco MC with nominal truth definition, response rebuild, unfold, closure/refold checks, and a noise-floor decision.
2. For a genuine paired source, retain `X_i^+`, `X_i^-`, and signed `Delta_i=(X_i^+-X_i^-)/2`. The nominal final magnitude is `abs(Delta_i)` only after the coherent-response/noise test. For a documented nonlinear but stable response, use the explicitly approved envelope `max(abs(X_i^+-X_i^0),abs(X_i^--X_i^0))`; do not silently substitute it.
3. One-sided historical inputs may remain in the audit table but are not final detector-source definitions. Their historical magnitude is `abs(X_i^var-X_i^0)` and must be labelled one-sided/provisional.
4. The frozen central-tracking precedent (1.0% central ⊕ 0.2% K0S = 1.0198039027%, diagonal and charge-independent) is a **separate prescription** from the delivered -0.5% random-drop ensemble. A single final baseline must choose one and document non-duplication; this specification does not silently combine them.

## 2. Magnitude-discrepancy audit

### 2.1 What is actually in the current z005 total

The inspected CSV has 490 rows and the 10 component names listed in §0. It contains no `hfs_phi`, `response_mcstat`, `crad`, `trigger`, `NucliaWeight`, background, DIFFVM, or charge-dependent-tracking column. Its detector entries are fractional numerical vectors only; neither this CSV nor its column names records the physical shift magnitude or whether a side was paired. The historical shifted-pipeline runbook supplies that provenance: `hfs_jes_down`, `hfs_rces_up`, `hfs_theta_up`, `elecE_p1pct`, and `elecTheta_p1mrad` were the accepted tags. Thus JES/RCES/HFS-theta were one-sided in that historical total. Electron energy was +1%; electron theta was +1 mrad. Covariance v4 further classifies electron energy and theta as **provisional post-hoc vectors**, not paired shifted-response covariances.

### 2.2 Findings and required actions

1. **Scattered-electron energy: 1% versus ±0.5% per LAr wheel.** The analysis note says “1% shift of `E_e'`”; the current implementation provenance is `elecE_p1pct` (+1%); the frozen v6sys authority is ±0.5% for each of five wheels. The correct final definition is the frozen ±0.5%-per-wheel prescription. The historical +1% vector is about twice the frozen variation and remains provisional. **Action:** replace the source with paired Stage1+Stage2, five-wheel explicit, ±0.5% shifted-response releases; do not rescale the existing vector.
2. **HFS JES: 2% versus ±1% in jet.** The note says 2%; the frozen high-Q2 menu says ±1% only inside the specified kT jets, independent of RCES. The current CSV’s `hfs_jes_rel` has no encoded amplitude and the historical accepted input is one-sided `hfs_jes_down`. The correct final definition is ±1% in-jet. **Action:** audit/rebuild the jet classifier and produce paired ± shifted-response JES; revise the note.
3. **HFS RCES: 2% versus ±1% outside jet.** The note says 2%; the frozen menu says an independent ±1% outside the specified jets. The CSV only carries `hfs_rces_rel`; historical accepted provenance is one-sided `hfs_rces_up`. The correct final definition is ±1% out-of-jet. **Action:** verify `eHadEnRCES` in relvol10, produce paired ± shifted-response RCES, and revise the note.
4. **Electron theta scope: 1 mrad alone versus `eElecTh` plus matched-track angle.** The note's 1 mrad is incomplete: the frozen source is ±1 mrad per each of three LAr theta regions **and** ±1 mrad co-shift of `eElecTrTh`. The current CSV’s `elec_theta_rel` and tag `elecTheta_p1mrad` do not, by themselves, prove this matched-track co-shift. **Action:** source-level setter audit, then paired full rerun. The numerical 1 mrad magnitude is retained, but the final physical implementation must include `eElecTrTh`.
5. **Detector sides and propagation: paired shifted response versus one-sided/post-hoc.** The frozen menu requires paired ± Stage1+Stage2 shifted reco data/MC response reruns for electron and HFS sources. The current z005 table was assembled from one-sided HFS tags (JES down, RCES up, theta up), electron +1%/+1 mrad historical releases, and covariance v4 labels the electron inputs provisional post-hoc vectors. Therefore the current total is not a paired-envelope detector budget. **Action:** do not promote the table; rerun all frozen migration sources on paired sides and rebuild the source covariances after noise tests.
6. **Tracking definition: frozen 1.0198% analytic versus current -0.5% ensemble.** The frozen menu’s central tracking source is 1.0% ⊕ 0.2% = 1.0198% diagonal. The actual CSV implements `tracking_eff_m0p5_rel`, a 16-seed random -0.5% efficiency ensemble with bin-dependent values (not a flat 1.0198% column). Neither may be assumed to replace or supplement the other without a decision. **Action:** choose and validate one baseline tracking-efficiency prescription; prevent double counting.
7. **Analysis-note completeness claim versus final-state blockers.** The note calls its list “components currently included,” but its prose says only `C_rad` is missing. The CSV excludes response MC statistics, trigger, NucliaWeight, background/DIFFVM, HFS phi, and charge-dependent tracking; several included detector terms are provisional. **Action:** label the table historical/provisional and replace “remaining missing component” wording with the register and gates in this document.

## 3. Decision list required before “final”

1. **Detector symmetrisation.** Options: coherent half-difference; approved max-side envelope for a demonstrably nonlinear response; retain one-sided only as provisional. Evidence: paired releases, signed shifts, same-side counts, closure/refold and noise controls. **Recommended default:** half-difference after coherence validation; otherwise block, not one-sided promotion.
2. **Tier-B detector bin correlation.** Options: rank-1 `Delta Delta^T`; diagonal `delta_ij sigma_i^2`; a measured covariance from replicas. Evidence: zero-shift/bootstrap noise floor, repeated shifted unfolding, and same-side audit showing `abs(Delta_i)` exceeds noise where rank-1 is used. **Recommended default:** diagonal provisional fallback; use rank-1 only after the noise-floor test passes.
3. **Quarantined two-sided HFS theta.** Options: rerun/diagnose the even response; adopt an explicit envelope after approval; retain one-sided provisional source. Evidence: explain audited signed mean +0.25681140% (up) and +0.25880271% (down), demonstrate deterministic detector effect versus quadratic/noise response. **Recommended default:** quarantine it and rerun; do not place either rank-1 or diagonal two-sided construction in final totals.
4. **Response-MC statistical covariance.** Options: response bootstrap with full covariance; defer the release. Evidence: response/fake replicas including `sumw2`, validation and PSD/alignment checks. **Recommended default:** block final publication until a 490-bin response-MC covariance exists.
5. **Trigger magnitude.** Options: measured efficiency uncertainty; frozen ≤0.5% ceiling; older 1.2%. Evidence: S67 data/MC or monitor-trigger study versus kinematics and charge. **Recommended default:** retain ≤0.5% only as a labelled placeholder and exclude it from a final claim until measured.
6. **`background_rel` retirement/reinstatement.** Options: retire after validated zero/subtraction logic; build track-level source; retain corrected event-level diagnostic pending policy. Evidence: regenerated alpha0p2 aggregates with `overlap_980` and `n_raw_980`, template exposure provenance, and P-ALPHA response consistency. **Recommended default:** BLOCKED, neither silently retired nor included.
7. **Charge-dependent tracking.** Options: regenerate to the 2.5%-on-`A_h` Aaron envelope; justify a different convention; omit `A_h` release. Evidence: a charge-resolved sanity calculation and covariance satisfying the precedent. **Recommended default:** use the 2.5%-on-`A_h` envelope only after re-emission/validation; otherwise block `A_h`.
8. **Tracking-efficiency baseline.** Options: use frozen 1.0198% analytic diagonal source; use delivered -0.5% ensemble; demonstrate a replacement relation. Evidence: common definition, applicable fiducial, closure, and no double counting. **Recommended default:** retain the audited delivered ensemble for the present historical table but require supervisor approval before calling it the final replacement for the frozen central-tracking source.
9. **`C_rad` as a source.** Options: no `C_rad` because result stays radiative level; apply approved DJANGOH correction with its uncertainty; use approved alternate-generator envelope. Evidence: GenEmpz convention, non-radiative exposure provenance, residual trend explanation, and coherent RAPGAP/DJANGOH decision. **Recommended default:** no Born-level final result and no invented `crad_rel` until the correction itself is approved; once applied, its uncertainty is mandatory.
10. **DIFFVM assumed zero.** Options: validated zero; template-derived source. Evidence: survival/selection assessment of rho, phi, omega samples and schema/normalisation validation. **Recommended default:** block final background closure until assessed.
11. **h+/h- statistical covariance.** Options: measure shared-event/statistical cross-block; retain zero assumption with an approved proof. Evidence: joint toy/bootstrap construction from the same data events. **Recommended default:** measure it; zero-by-assumption is not final for a charge asymmetry.
12. **Nominal unfolding method and response conservation.** Options: retain IBU/BBB; promote native v6 TUnfold only after its gates. Evidence: resolve `body+miss` versus stored-prior mismatch in all 245 bins per charge, then method validation including fake/background and response-MC covariance. **Recommended default:** retain IBU/BBB as central diagnostic candidate, but block final release until conservation and P-METHOD close.

## 4. Final combination prescription

Let `x_i` be the released charge-resolved observable in bin `i`, and let `s` label only approved sources. Store source-resolved signed shifts and matrices on the exact 490-bin map. A finite number in the historical CSV is not a substitute for this payload.

For paired, coherent migration source `s`,

`Delta_i^s = (x_i^{s,+}-x_i^{s,-})/2`,  and  `u_i^s = abs(Delta_i^s)`.

For an explicitly approved nonlinear envelope source,

`u_i^s = max(abs(x_i^{s,+}-x_i^0), abs(x_i^{s,-}-x_i^0))`.

For a one-sided historical source only,

`u_i^s = abs(x_i^{s,var}-x_i^0)`,

and it is excluded from the final detector tier until replaced by the paired definition. For a fractional CSV entry `r_i^s`, use absolute units `u_i^s = abs(x_i^0) r_i^s`.

### 4.1 Per-bin reporting total

For an observable whose approved sources have per-bin magnitudes `u_i^s`, the displayed diagonal uncertainty is

`u_i,total = sqrt( sum_s (u_i^s)^2 ) = sqrt( C_total,ii )`.

This quadrature identity is a reporting identity, not a licence to discard covariance. Statistical uncertainty is reported separately from systematic uncertainty unless the output explicitly requests total experimental uncertainty.

### 4.2 Covariance assembly by tier

On the common 490-bin basis,

`C_total = C_stat,data + sum_(s in A) C_s + sum_(s in B) C_s + sum_(s in C) C_s + C_norm`,

where only source matrices with status IMPLEMENTED and all applicable final gates passed enter. Blocked, dropped, placeholder, and diagnostic sources contribute **no numeric zero**; they appear in the release completeness ledger and prevent “final” status.

* **Tier A (measured/statistical and established analytic):** use the measured data-toy `C_stat,data`; use `C_lumi = l l^T`, `l_i=0.027 x_i`, only for absolute cross sections; use the approved tracking matrix exactly as validated (current ensemble: diagonal; frozen analytic alternative: diagonal). The h+/h- statistical off-diagonal block must be measured for final asymmetry work.
* **Tier B (electron/HFS detector shifts):** after paired shifted-response and noise validation, use `C_s=Delta^s (Delta^s)^T` only for a coherent one-nuisance response. If signed coherence is not defensible but the magnitude is accepted, use the explicitly labelled temporary `C_s,ij=delta_ij (u_i^s)^2`. A measured replica covariance supersedes either. Do not use rank-1 on noise-dominated vectors.
* **Tier C (model, regularisation, response MC, backgrounds, radiative/model weights):** use a measured alternate-response/replica covariance where available. A diagonal fallback is temporary and must be tagged. Model and regularisation are distinct; response MC statistics are distinct; BBB/IBU/TUnfold spread is never added.

Before summation every `C_s` must be finite, symmetric, PSD within numerical tolerance, aligned by `(charge, bin_index)`, and carry its source status/correlation metadata.

### 4.3 Integrated quantities

For any integrated quantity `I = a^T x` (where `a_i` contains bin widths, integration weights, or an explicit selection), use

`Var_s(I) = a^T C_s a`,  `Var(I)=a^T C_total a`,  `u(I)=sqrt(Var(I))`.

For a nonlinear derived observable `g(x)` use either varied charge-resolved outputs or the Jacobian `J`,

`C_g = J C_x J^T`.

Do **not** integrate by quadrature-summing per-bin percentages.

For charge asymmetry in matched bins,

`A_i=(x_i^+ - x_i^-)/(x_i^+ + x_i^-)`,

form `A_i^{s,+}` and `A_i^{s,-}` from varied **charge-separated** yields before extracting `Delta_A^s`; never infer cancellation from separate h+ and h- totals. Luminosity is excluded exactly. A common trigger, common NucliaWeight, common tracking efficiency, and common radiative factor may cancel only if the actual paired charge-resolved variation demonstrates it. Charge-dependent tracking is anti-correlated and does not cancel. Detector and model residuals must be evaluated, not assumed zero.

### 4.4 Observable inclusion rules

* **Absolute cross sections:** include approved detector/model/unfolding/background/radiative sources and luminosity.
* **Multiplicities:** exclude luminosity. Include `C_rad` only if the multiplicity definition applies it; otherwise state radiative level explicitly. Do not adopt NucliaWeight or trigger until their decisions close.
* **Charge asymmetry:** exclude luminosity and do not add a common source by fiat; evaluate from varied yields. Include charge-dependent tracking once validated. The current zero h+/h- statistical cross-block forbids final asymmetry covariance.

## 5. Acceptance gates for a publication-grade final budget

1. Confirm the canonical v6/z005 central release and exact 490-bin `(charge, bin_index)` map.
2. Publish a source ledger containing every register row, status, applicability, reference, magnitude, and exact artifact path; no missing result may be called zero.
3. Replace the historical electron +1% vector with paired ±0.5%-per-five-wheel `eElecEn` Stage1+Stage2 shifted-response results.
4. Demonstrate electron theta has three-region `eElecTh` **and** co-shifted `eElecTrTh`, then produce paired ± results.
5. Produce paired ± Stage1+Stage2 shifted-response releases for JES, RCES, HFS theta, and HFS phi; audit the prescribed in-jet/out-of-jet classifier and the `eHadEnRCES` enum.
6. For every Tier-B detector source, complete branch/setter audit, shifted data/MC consistency, response rebuild, 490-row finite output, closure/refold/stability, and noise-floor/same-side tests.
7. Choose and document per-source symmetrisation and covariance model; rank-1 is prohibited without a passed coherence/noise test.
8. Resolve or rerun the quarantined HFS-theta even/quadratic two-sided response; do not include it through either unvalidated covariance form.
9. Deliver response-MC replicas and a validated full 490-bin covariance including the stated fake/response treatment.
10. Resolve the RAPGAP model disposition, the IBU/BBB-versus-native-v6-TUnfold nominal-method decision, and the response truth-conservation mismatch.
11. Close `C_rad` convention, exposure provenance, generator choice, and its uncertainty before claiming Born-level results; if it is applied, insert `crad_rel` with covariance.
12. Measure trigger uncertainty or explicitly retain a non-final placeholder; decide NucliaWeight applicability.
13. Re-examine `background_rel` after adding `overlap_980` and `n_raw_980`, closing template provenance and P-ALPHA; assess DIFFVM rather than assume zero.
14. Choose one central tracking baseline without double counting; validate charge-dependent tracking against the 2.5%-on-`A_h` precedent.
15. Measure the h+/h- statistical cross-block (or supply an approved proof for zero) before an `A_h` covariance is final.
16. Validate all source and total matrices for alignment, finite values, symmetry, PSD tolerance, diagonal closure, and covariance-aware integrated results.
17. Update the analysis note: correct 1% electron energy, 2% JES/RCES, matched-track theta scope, historical one-sided/provisional status, and all missing-source claims; remove stale 552% background interpretation.
18. Regenerate only after the above decisions using the resolved canonical plotter; require source breakdown, statuses, statistical/total labels, and 490-bin validation.

## 6. Status census and release conclusion

For the 24 numbered rows in §1: **IMPLEMENTED 4; PROVISIONAL 6; BLOCKED 9; DROPPED 5.** The provisional and blocked rows mean the current `total_with_tracking_rel` column is a useful historical/audit uncertainty summary, not the final systematic uncertainty definition and not a publication-grade covariance.

### Primary evidence consulted

* `STATE.md` and `docs/RUN_STATUS.md` (both current through 2026-09-25).
* Frozen menu: `docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md`.
* Current CSV and historical shifted-systematics runbook: `docs/systematics/z005_shifted_systematics_pipeline_2026-06-13.md`.
* Covariance status: `docs/systematics/covariance_package_z005_2026-09-23.md`.
* Electron migration methodology: `docs/systematics/electron_systematics_methodology.md`.
* Background handoff: `docs/handoffs/SESSION_HANDOFF_backgrounds_systematics_2026-09-25.md`.
* Analysis note Table `tab:syst:z005-components`: `analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/systematics.tex`.

---

## Decision log addendum — 2026-10-10 (ADDITIVE; the frozen register and §2–§6 above are unchanged)

This addendum records user decisions taken on 2026-10-10 after the
PROVISIONAL→final gap audit (`docs/systematics/provisional_to_final_gap_audit_2026-10-10.md`).
It does not rewrite, re-status, or reinterpret any frozen row above; where a decision
closes a §3 decision item or register condition, that closure is recorded here and in the
completeness ledger (`docs/systematics/completeness_ledger_2026-10-10.md`).

**D-2026-10-10-1 — `phi_h_gauss_rel`: ADOPTED (option A of the 2026-10-02 adoption record).**
The Gaussian rolling-window floored phi_h flat-reweight envelope is adopted as a final
budget component with no re-derivation. Medians h+ `1.0482%` / h− `1.0550%` (coherent
rederivation 2026-10-06, floored; current basis of the canonical 13-component budget).
Mandatory carried caveat (adoption record `docs/systematics/flatphi_phih_systematic_adoption_2026-10-02.md`):
the envelope is **conservative** — it includes fork/prior and DJ-vs-RG generator-model
content (~1.6% median) already covered by `ibu_model_rel`; the pure flat-vs-unity phi_h
sensitivity is ~0.6% median, and the smoothing has no noise guard. Any use must carry this
caveat. Closes gap-audit §6.1 (option A) and P1 item 5.

**D-2026-10-10-2 — `tracking_eff_m0p5_rel` baseline: CONFIRMED.**
The delivered 16-seed −0.5% random-drop ensemble (coherent regeneration 2026-10-07, 16/16
valid seeds 314159–314174; systematic = |ensemble mean − coherent nominal|/|nominal|;
medians h+ `0.1806%` / h− `0.1828%`) is the single final tracking-efficiency baseline.
The frozen analytic `1.0% ⊕ 0.2% = 1.0198039027%` central-tracking prescription (register
rule 4) is **NOT adopted**; because the alternative is not used, no double-count arises.
Closes §3 decision 8 and gap-audit P1 item 4.

**D-2026-10-10-3 — P2 detector-tier campaign sequence: APPROVED for launch.**
The full 12-campaign paired detector-leg program is approved, in the order:
`eElecEn` ±0.5% per each of five LAr wheels → `hfs_jes` ±1% in-jet + `hfs_rces` ±1%
out-of-jet → `hfs_theta` ±1 mrad → `elec_theta` ±1 mrad with `eElecTrTh` co-shift →
`hfs_phi` ±20 mrad. `hfs_phi_rel` (register row 7) becomes the **14th** budget component
on completion. Every campaign is gated on: (i) single-file smoke test; (ii) normalization
check (`L=351000.0 nb^-1`, relative-only outputs, regime-B invariance); (iii) the
**canonical central 658-file input list** — the legacy HFS 660-file list (ep0607: 325 vs
central 323, gap audit §3.4) is cured by pinning 658 in all reruns. Paired
Stage1+Stage2 shifted reco of data and MC, shifted response rebuild, coherent `body+miss`
prior, k=4, closure/refold and noise-floor/same-side tests per §1 rows 4–9 and §5.3–5.8.
One-sided historical vectors remain excluded from the final detector tier (register rules
1–3). Campaign order and scope per gap audit §4/§7-P2.

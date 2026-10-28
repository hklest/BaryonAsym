# Master completion roadmap — H1 high-Q² inclusive charged-hadron SIDIS

**Date:** 2026-09-30  
**Scope:** v6/z005 high-Q² inclusive charged-hadron SIDIS only (`Q² > 150 GeV²`), its 490-row (`245 h+ + 245 h−`) release, and its analysis note. This roadmap does not authorize a central overwrite, a Born-level claim, or a final/publication-grade uncertainty claim.

**Central audit reference:** `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`.

> UPDATE 2026-10-06: the "single highest-priority blocker" (response truth-conservation defect) is root-caused and its coherent-prior fix is fork-validated; read `docs/unfolding/coherent_prior_fix_results_2026-10-05.md` for the resolved state and the adoption-pending requirements (COHERENT-PRIOR lineage, not yet adopted in central).

## SECTION A — EXECUTIVE SUMMARY

The analysis has a usable but non-final v6/z005 central candidate: IBU `k=4` with BBB as a diagonal cross-check, four-period luminosity `351000.0 nb⁻¹ = 351 pb⁻¹`, and a raw/no-background-subtraction, no-`C_rad` release. It is not Born level (`c_rad_applied=false`). The historical total-uncertainty table has 10 numerical components, including the delivered tracking ensemble, but it is not the final systematic definition: the controlling register contains **4 IMPLEMENTED, 6 PROVISIONAL, 9 BLOCKED, and 5 DROPPED** sources.

The shortest defensible path is deliberately asymmetric:

1. Repair and validate the existing IBU response/prior chain, then quantify the changed `k=4` result and complete finalcandidate-geometry closure.
2. Decide the radiative-correction contract for the absolute `cross_section_nb` observable, close its selection and exposure provenance, and only then decide whether to construct a separate immutable Born-corrected release.
3. Finalize the systematic source definitions and covariances around the repaired release, including paired detector reruns and response-MC statistics.
4. Retain IBU/BBB unless native v6 TUnfold independently meets its much larger promotion gate set.
5. Close response-consistent alpha-veto/background treatment and the outstanding background inventory/provenance decisions.
6. Regenerate final tables and figures only after the physics and covariance decisions, then complete the note with claims matched to the final artifacts.

**Single highest-priority blocker:** the RAPGAP response truth-conservation failure. In every truth bin of both charges, response `body + miss` differs from the stored IBU prior. This may bias the existing central IBU result and must be repaired before any release promotion, method defense, final covariance, or final analysis-note unfolding claim.

## SECTION B — CRITICAL PATH BLOCKERS, RANKED

### B1. Repair the response truth-conservation failure — highest priority

**Problem.** For the central `(491,246)` response of each charge, the correct truth marginal is `sum(physical reco body down the reco axis) + miss row` for each of 245 physical truth columns. It differs from the stored RAPGAP prior in **245/245 bins for h+ and 245/245 bins for h−**. The ratios `(body+miss)/prior` range from `0.964168` to `1.028029` for h+ and `0.965375` to `1.028519` for h−: approximately ±2.8% binwise scatter despite only +0.103565%/+0.095776% integrated offsets. With the stored prior used for both IBU initialization and column normalization, the exact iteration-1 local normalization envelope is **−2.77% to +3.72%**. It is not a rigorous `k=4` bound, but it demonstrates potential binwise bias.

**Decision or work required.** Freeze audit hashes; instrument the actual finalcandidate builder at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`; save the legacy prior, NW-effective prior, body column sum, miss, and selection-category differences in the same event loop. Rebuild an isolated response candidate with one coherent weighting convention, audit both truth and reco marginals, and run a corrected-prior IBU `k=1–10` comparison plus split-MC self/cross, reweighted-truth, pull, coverage, and covariance-aware refold tests.

**What it gates.** Any promotion of IBU `k=4`; k-selection; the method decision; valid response-model, regularization, and response-MC systematics; a final analysis-note unfolding/acceptance statement; and final release sign-off.

**Recommended default.** Preserve the current release as an audit reference, but stop promotion. Use the coherent NW-effective truth marginal as the stored IBU prior (or otherwise make matched body, miss, and truth marginal use one documented convention). Do not apply a global rescale. BBB acceptance may be retained only after confirming it is derived from body+miss rather than `h_prior_gen` and after the repaired response passes all audits.

### B2. Decide and close the `C_rad` normalization convention

**Problem.** The release table is an absolute `cross_section_nb` product, not a per-DIS-event multiplicity: it has no DIS denominator and metadata identifies it as raw/no-`C_rad`. A conditional hadron-multiplicity ratio is about `1.05`, whereas the diagnostic binned cross-section ratio is about `1.1372` at `GenEmpz>45`; these are different objects because the latter includes the selected DIS event-rate ratio. The current V0/`IsGtr` candidate is 490/490 finite but uses `GenEmpz>45`; it is not automatically compatible with the apparently open response truth space. Exposure provenance, OLD/NEW NONRAD compatibility, the reco/truth EMPZ contract, residual shape trend, and RAPGAP/DJANGOH coherence remain open.

**Decision or work required.** Audit the CSV-to-observable formula and full-manifest factorization separately in e+ and e−. Establish one executable central reco selection and one truth fiducial, including whether `GenEmpz` is absent. Certify OLD/NEW and RAD exposures with file-level generator/MCDB/log provenance and overlap audit; make a 490-bin OLD/NEW sidecar; rebuild the approved candidate; assess response-swap stability and, if possible, a provenance-valid matched RAPGAP RAD/NONRAD comparison. Keep all 490 bins only if the approved candidate meets finite, statistical, continuity, and range-review gates.

**What it gates.** Any Born-level result; `crad_rel` and radiative covariance; final absolute-cross-section tables and plots; radiative-corrections note text; and a final release claim if Born level is the intended observable.

**Recommended default.** The current observable is an absolute cross section, so use the full **binwise cross-section ratio** `C_rad = σ_nonrad/σ_rad`, not the ∼1.05 conditional multiplicity ratio, if and only if all closure gates pass. Use open truth (no truth `GenEmpz` cut) as the provisional target because the response truth denominator is documented as open; treat reco EMPZ agreement as release-affecting. Until approval, retain the radiative-level release with `c_rad_applied=false`; do not invent `crad_rel`.

### B3. Finalize the systematic-uncertainty definition and covariance

**Problem.** The existing total table is historical/audit information, not a publication-grade covariance. It contains numerical vectors for ten components, but detector terms include one-sided or post-hoc inputs and the register still has blocked response-MC statistics, `C_rad`, trigger, background, DIFFVM, charge-dependent tracking, and HFS phi. It also has unresolved tracking-baseline, correlation, statistical cross-block, and background decisions.

**Decision or work required.** Apply the source register in Section C: replace frozen migration sources by paired Stage1+Stage2 shifted-response releases; choose approved symmetrisation and correlation models; deliver full response-MC covariance; choose exactly one tracking baseline; settle the source applicability ledger; assess DIFFVM; measure or justify charge statistical cross-blocks; and validate each source and total covariance for alignment, finiteness, symmetry, PSD tolerance, and diagonal/integrated closure.

**What it gates.** A final uncertainty table or covariance; cross-section, multiplicity, and charge-asymmetry claims; final systematic plots; and the systematics, acceptance, and results portions of the note.

**Recommended default.** Treat blocked and provisional entries as nonzero unresolved gates, not zeros. Retain the delivered tracking ensemble only as the historical-table implementation pending supervisor approval of its replacement relation. Use paired half-differences only after coherence/noise validation; otherwise keep the source blocked rather than promote a one-sided vector.

### B4. Make the nominal-method decision after the response repair

**Problem.** Native v6 TUnfold has correct miss-underflow geometry but fails or lacks key physics-promotion gates: data refold χ²/ndf is poor, negative bins remain, high-ρ/tail failures persist, and Poisson closure, fake/background covariance, and response-MC propagation are incomplete. Pre-repair IBU `k=4` is also not yet defended by a quantitative split-MC bias–variance/coverage stopping rule.

**Decision or work required.** After B1, run finalcandidate-geometry IBU `k=1–10` split-MC Poisson closure in both generator directions, including pulls, 68% coverage, refolds, negatives, response-MC fluctuations, and reweighted-truth injection. Decide k from a documented plateau. TUnfold may be designed in parallel but needs the repaired response, one frozen fake/background convention and covariance, and successful tau/PSD/refold/negative/ρ/closure tests before promotion.

**What it gates.** The nominal-method declaration, unfolding section of the note, final regularization and model treatment, and publication plots.

**Recommended default.** Keep **IBU `k=4` nominal candidate with BBB as diagonal cross-check** after repair and validation. Keep native v6 TUnfold diagnostic/qualified cross-check unless it independently passes every promotion criterion. Do not add BBB/IBU/TUnfold spread as a systematic.

### B5. Close P-ALPHA and background treatment

**Problem.** The central candidate is raw/no-background-subtraction and uses response fake-fraction subtraction only; electrons and V0 are selection-vetoed, not subtracted. Alpha0p2 products are diagnostic/provisional. With unchanged response, non-V0 electrons are fake-column handled and det.int is mostly matched response, so generic subtraction can double count. Track-level background-systematic construction lacks `overlap_980` and `n_raw_980`; DjBkgd template exposure is framework-unit-correct but not provenance-closed; DIFFVM is assumed rather than assessed.

**Decision or work required.** Choose and demonstrate one response-consistent background treatment through closure and covariance. Regenerate alpha0p2 aggregates with overlap and raw-count vectors, complete exact template exposure provenance, reassess `background_rel` rather than retaining or reversing its retired status silently, and assess rho/phi/omega DIFFVM survival and normalisation. Confirm event-level closed backgrounds remain excluded by selection rather than assigned a template systematic.

**What it gates.** Background covariance; response/fake treatment used in IBU/TUnfold closure; final systematic completeness; and background/acceptance/note claims.

**Recommended default.** With unchanged response, only pre-subtract/veto V0 where validated; leave non-V0 electrons in fake-column handling and do not blindly pre-subtract det.int. With all-fake response, handle electron/V0/det.int exactly once as fake-column components. Keep `background_rel` blocked until the required artifacts and P-ALPHA closure exist.

## SECTION C — SYSTEMATIC UNCERTAINTY FINALIZATION

### C1. Action register

| Status | Sources | Analyst action and exact final rule |
|---|---|---|
| IMPLEMENTED (4) | `ibu_stat_rel`; `ibu_reg_rel`; `tracking_eff_m0p5_rel`; `lumi_rel` | Keep auditable artifacts, but complete remaining final conditions. Measure/justify h+/h− statistical cross-block for `ibu_stat_rel`. Retain signed k-side evidence for `ibu_reg_rel`. Use only `delta_track_rel`/`tracking_eff_m0p5_rel`, never `seed_rms` or `seed_mean_unc`, pending the tracking-baseline decision. Use 2.7% rank-1 luminosity covariance only for absolute cross sections; exclude it from multiplicities and charge asymmetries. |
| PROVISIONAL (6) | `ibu_model_rel`; `hfs_jes_rel`; `hfs_rces_rel`; `hfs_theta_rel`; `elec_energy_rel`; `elec_theta_rel` | Do not call final. `ibu_model_rel` needs RAPGAP disposition and a measured charge-aware covariance. All five detector sources require paired Stage1+Stage2 shifted reco data and MC, shifted response rebuilding, unfolding, closure/refold, 490-row checks, and noise/coherence decisions. Current one-sided/post-hoc vectors remain audit-only. |
| BLOCKED (9) | `hfs_phi_rel`; `tracking_chargedep_rel`; `response_mcstat_rel`; `crad_rel`; `trigger_rel`; `NucliaWeight_rel`; `background_rel`; `diffvm_rel`; `gammap_bkg_30pct_rel` | No blocked item enters a final total as zero. Produce paired HFS-phi reruns; rebuild charge-dependent tracking to a validated 2.5%-on-`A_h` convention before an asymmetry release; deliver response-bootstrap covariance including `sumw2`/fake treatment; apply `crad_rel` only after approved `C_rad`; measure trigger; decide NucliaWeight applicability; close P-ALPHA/track-level backgrounds; assess DIFFVM; and document that gamma-p has no consumed subtracted template rather than emitting a numerical source. |
| DROPPED (5) | `electron_phi_rel`; `noise_energy_rel`; `tracking_resolution_rel`; `combined_forward_tracking_10pct_rel`; `method_spread_BBB_IBU_TUnfold_rel` | Exclude from the baseline: there is no pinned recipe for electron phi/noise/tracking resolution; forward tracking is inapplicable to `|η_h|<1.3`; and method spread is validation coverage, not a covariance source. Do not revive a dropped source without an approved prescription. |

### C2. Precise detector prescriptions requiring paired shifted-response reruns

The following all require paired Stage1+Stage2 data-and-MC reruns, response rebuilding with unchanged nominal truth definition, unfolding, and the common validation bundle. A historical one-sided tag or a post-hoc vector is not a final implementation.

| Source | Frozen definition | Required correction to historical/note treatment |
|---|---|---|
| HFS JES | `eHadEn` ±1% only inside kT `R=1.0`, `pT>5 GeV`, `−1.5<η_lab<2.75` jets | Audit the jet classifier. Replace historical one-sided `hfs_jes_down`; do not use 2%. |
| HFS RCES | `eHadEnRCES` ±1% for HFS objects outside those jets, independent of JES | Verify relvol10 enum/classifier. Replace historical one-sided `hfs_rces_up`; do not use 2%. |
| HFS theta | `eHadTh` ±1 mrad | Rerun and diagnose the quarantined even/quadratic two-sided result; do not promote either side or a rank-1 construction until explained. |
| HFS phi | `eHadPh` ±20 mrad | Produce the missing paired z005 source artifact and table column. |
| Electron energy | `eElecEn` ±0.5% separately for each of five LAr wheels: BBE, CB1+2, CB3, FB1, FB2+IF | Apply all explicit wheel setters on both sides. Replace the historical +1% vector; do not rescale it. |
| Electron theta | `eElecTh` ±1 mrad separately for three regions, **co-shifted with matched-track `eElecTrTh` ±1 mrad** | Audit setters and prove the co-shift, then replace the historical +1 mrad/post-hoc treatment. |

For each paired source retain `X⁺`, `X⁻`, and `Δ = (X⁺−X⁻)/2` on the exact `(charge, bin_index)` map. Use `|Δ|` only after a coherent-response/noise test. For an approved stable nonlinear response, use the explicitly approved maximum-side envelope. Otherwise the source remains blocked; it is not converted into a final one-sided magnitude.

### C3. Open methodological decisions and defaults

| Decision | Required evidence | Recommended default |
|---|---|---|
| Detector symmetrisation | Paired releases, signed shifts, closure/refold, same-side counts, noise control | Coherent half-difference after validation; otherwise block. |
| Tier-B bin/charge correlation | Bootstrap/zero-shift noise floor and shifted-output coherence | Diagonal provisional fallback only when a magnitude is accepted; rank-1 `ΔΔᵀ` only after passed coherence/noise evidence. |
| Quarantined HFS theta | Explain the even response or reproduce it deterministically | Quarantine and rerun; do not include a two-sided construction yet. |
| Response-MC statistics | Response/fake replicas, `sumw2`, full covariance, PSD/alignment checks | Block final publication until a validated 490-bin covariance exists. |
| Trigger | S67 data/MC or monitor-trigger study versus kinematics and charge | ≤0.5% is only a labelled placeholder; exclude from final claim until measured. |
| `background_rel` | `overlap_980`, `n_raw_980`, template provenance, P-ALPHA closure | Blocked; neither silently retired nor included. |
| Charge-dependent tracking | Charge-resolved variations and 2.5%-on-`A_h` sanity result | Use the 2.5%-on-`A_h` envelope only after regeneration/validation; otherwise block `A_h`. |
| Tracking baseline | Common definition, fiducial applicability, closure, no duplication | Keep delivered −0.5% ensemble in the historical table; require supervisor approval before declaring it the replacement for frozen 1.0198% analytic central tracking. Choose one only. |
| `C_rad` source | Approved correction, convention, exposure, generator/model decision | No Born-level final result and no invented `crad_rel` until approval; once applied, include its statistical/model covariance. |
| DIFFVM | rho/phi/omega survival/selection and normalisation assessment | Block final background closure until assessed. |
| h+/h− statistical cross-block | Joint toys/bootstrap from shared events or approved zero proof | Measure it; zero-by-assumption is not final for charge asymmetry. |
| Nominal method and response conservation | B1 repair plus IBU/TUnfold validation | Retain IBU/BBB candidate, but do not finalize before conservation and P-METHOD close. |

### C4. Combination and reporting rules

Assemble source-resolved covariance matrices on the 490-bin charge-major map. For approved sources, report `u_total,i = √C_total,ii`, but never replace covariance-aware propagation with quadrature of percentages. Require every source matrix to be finite, symmetric, PSD within numerical tolerance, and aligned by `(charge, bin_index)`.

Use measured data-toy covariance for statistics; rank-1 luminosity only for absolute cross sections; approved tracking as its validated matrix; and measured alternate-response/replica covariance for model, regularization, response-MC, background, and radiative sources where available. For any integrated result `I = aᵀx`, propagate `Var(I) = aᵀ C_total a`. For charge asymmetry, vary charge-separated yields and form the asymmetry from each variation; luminosity cancels exactly, but other apparent common sources may cancel only if demonstrated.

## SECTION D — ANALYSIS NOTE COMPLETION PLAN

**Note repository audited by supervisor:** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/`, branch `development`, commit `05e9e1e`. It builds to 69 pages only after `export PATH=/cvmfs/sft.cern.ch/lcg/external/texlive/2025/bin/x86_64-linux:$PATH`.

### D1. Note work BLOCKED on an upstream physics decision (9 items)

1. **Unfolding and acceptance rewrite:** wait for the B1 repaired response, controlled corrected-prior IBU comparison, and closure results. `unfolding.tex` and `acceptance.tex` currently describe the `(491,246)` response and BBB/IBU `k=4` without the truth-conservation failure or its bias implications.
2. **Nominal unfolding-method declaration:** wait for P-METHOD after repaired IBU closure and the IBU-versus-native-v6-TUnfold decision.
3. **k=4 justification and regularization narrative:** wait for the repaired-response `k=1–10` bias–variance, coverage, and refold plateau.
4. **Radiative-corrections rewrite and Born-level labels:** wait for the absolute-cross-section `C_rad` convention, truth/reco EMPZ contract, exposure provenance, OLD/NEW disposition, and generator coherence decision. Until then state raw radiative level, `c_rad_applied=false`, and not Born level.
5. **Final systematics table, covariance, and completeness claim:** wait for the register gates, paired detector sources, response-MC covariance, tracking choice, trigger/NucliaWeight decisions, and final source ledger.
6. **Background systematic and alpha-veto narrative:** wait for P-ALPHA, regenerated track-level overlaps/raw counts, template provenance, and DIFFVM assessment.
7. **Final results tables, total uncertainties, and publication plots:** wait for repaired central candidate, nominal method, `C_rad` decision/application status, and final covariance/plot regeneration.
8. **Four-generator and expanded-release result-figure wiring:** wait for the decision to retain the relevant diagnostic figures in the note and manual re-application onto current `development`; do not cherry-pick conflicted commit `dadf971`.
9. **Any final charge-asymmetry statement:** wait for measured h+/h− statistical cross-block and validated charge-dependent tracking treatment.

### D2. Note work ACTIONABLE NOW regardless of physics closure (13 items)

1. Fix the only undefined citation key: add/correct `2005Hadroo2` in `references.bib` after verifying the intended reference.
2. Correct `sections/systematics.tex` physical definitions: electron energy is ±0.5% per each of five LAr wheels, not “1%”; HFS JES is ±1% in-jet, not 2%; HFS RCES is ±1% out-of-jet, not 2%; electron theta is ±1 mrad in three regions **plus** matched-track `eElecTrTh` co-shift.
3. Replace the statement that only `C_rad` remains missing with the current register status: 4 IMPLEMENTED / 6 PROVISIONAL / 9 BLOCKED / 5 DROPPED, and label the historical total table accordingly.
4. Do **not** reintroduce the stale “552%” or “genuine sparse-yield” background interpretation. It is absent from the current working tree and appears only in unmerged commit `3311a12`; do not merge that commit as-is because it would restore retracted pre-2026-09-25-`fLumi`-correction numbers.
5. Add an explicit current-status caveat wherever results are summarized: central release is raw/no-background-subtraction, no `C_rad`, `c_rad_applied=false`, and not Born level.
6. Add a concise response-integrity caveat to current unfolding/acceptance prose now; reserve quantitative repair results for D1. It must state that response truth conservation is under repair and pre-repair IBU values are not final.
7. Expand the nine small section stubs, using only verified current information and clearly marked pending material: `observables.tex` (1,088 B), `species_definition.tex` (1,203 B), `experimental_setup.tex` (1,668 B), `acceptance.tex` (1,984 B), `summary.tex` (2,113 B), `monte_carlo.tex` (2,145 B), `radiative_corrections.tex` (2,356 B), `introduction.tex` (2,602 B), and `binning.tex` (2,648 B). The note has 15 section files totaling 71,815 B; expansion must not turn provisional physics into final claims.
8. Preserve the correct luminosity statement throughout: all audited occurrences are `351 pb⁻¹`; there are zero occurrences of `297657.8`, `356613.0`, `356000`, or `26.7039 nb`. Do not change this correct content.
9. Preserve figure integrity: all 53 distinct `\includegraphics` targets resolve; `results.tex` has four includes (lines 61, 71, 107, 124), and all already carry `_z005`. Do not apply the stale five-name fix.
10. Manually re-apply only desired figure references from `dadf971` onto current `development`, after content review. That commit wires three four-generator figures and two expanded-release figures into `results.tex` (+153/−25) and trims six lines from `introduction.tex`; it conflicts in nine hunks because both it and Overleaf merge `05e9e1e` edit overlapping `results.tex` regions. Use `backup-premerge-dadf971` as recovery provenance; do not cherry-pick.
11. Decide the disposition of six untracked, currently unreferenced PDFs under `figures/`: `fig_syst_uncertainty_sources_expanded_{hplus,hminus}_{autoscale,fullscale,zoom15pct}.pdf`. Either wire reviewed figures into the note with provisional labels or delete them; do not leave ambiguous untracked publication artifacts.
12. Add explicit provenance/status captions for any diagnostic 4-generator, flat-phi, background, or systematic plots retained in the note; they must not be portrayed as final publication physics before upstream closure.
13. Retain the audited TeX build environment in contributor/build documentation and verify the 69-page build after each note-only change.

## SECTION E — DEPENDENCY-ORDERED TASK LIST

“Parallel” identifies work that may proceed without waiting for preceding numbered tasks; it does not remove listed dependencies. All execution work belongs outside the central release until formal promotion.

| # | Task | Owner specialist | Depends on | Blocks | Release gate | Parallel? |
|---:|---|---|---|---|---|---|
| 1 | Freeze hashes of central response/release and add column-wise body+miss/prior regression audit. | h1-qa-specialist | None | Response root-cause and all release interpretation | Yes | Yes |
| 2 | Instrument isolated finalcandidate response build; emit legacy, NW-effective, body, miss, and selection-difference marginals. | h1-unfolder | 1 | Coherent response convention | Yes | No |
| 3 | Select coherent weight convention, rebuild study response, and validate truth/reco marginals, axes, acceptance, fake fractions, and `sumw2`. | h1-unfolder | 2 | Corrected IBU, closure, method choice | Yes | No |
| 4 | Run corrected-prior IBU `k=1–10` comparison and quantify 490-bin/integrated changes. | h1-unfolder | 3 | k selection and note claims | Yes | No |
| 5 | Build finalcandidate-geometry split-MC IBU closure framework: self/cross directions, Poisson pulls/coverage, refold, response-MC toys, and injected reweighted truth. | h1-unfolder | 3, 4 | IBU defense, response-MC source, model coverage | Yes | No |
| 6 | Choose/defend k using predeclared plateau metrics; update candidate-release metadata without overwriting central. | h1-unfolder | 5 | P-METHOD and unfolding note | Yes | No |
| 7 | Decide P-METHOD: retain repaired IBU/BBB unless TUnfold passes independent gates. | h1-unfolder | 6; TUnfold evidence if promotion proposed | Nominal method declaration | Yes | No |
| 8 | Design/run native-v6-TUnfold cross-check: frozen fake/background convention, split-MC Poisson closure, tau/refold/PSD/negative/ρ diagnostics. | h1-unfolder | 3 for final inputs | Optional TUnfold promotion only | No if IBU retained | Yes, after 3 |
| 9 | Audit release observable and full-manifest factorization; decide cross-section versus multiplicity `C_rad` convention and executable EMPZ/reco/truth contract. | h1-radiative-correction | 1 for response-contract audit | All approved `C_rad` work | Yes for Born release | Yes |
| 10 | Certify RAD/OLD/NEW exposures, e− attrition, component manifests, MCDB/log provenance, and overlap removal. | h1-normalization | 9 contract | Valid radiative normalization/pooling | Yes for Born release | Yes, after 9 |
| 11 | Produce 490-bin OLD/NEW compatibility sidecar with approved pools and exposures. | h1-radiative-correction | 9, 10 | NONRAD numerator choice | Yes for Born release | No |
| 12 | Rebuild `C_rad`, perform finite/stat/range/continuity QA, response swap, trend classification, and valid generator comparison/deferment. | h1-radiative-correction | 9–11; repaired response for response-swap result | `crad_rel`, Born application, radiative note | Yes for Born release | Partly; final comparison after 3 |
| 13 | If approved, create immutable Born-corrected sibling release, propagate covariance, and validate 490/490 join. | h1-radiative-correction | 12 and review approval | Born results/plots/note | Yes for Born-level claim | No |
| 14 | Produce all paired Stage1+Stage2 detector shifted-response reruns: JES, RCES, HFS theta/phi, electron energy, electron theta plus `eElecTrTh`; audit setters/classifier. | h1-systematics | Repaired response convention from 3 | Final detector systematic tier | Yes | Yes, after 3 |
| 15 | Apply paired-source symmetrisation/noise/coherence tests and build source covariance metadata. | h1-systematics | 14 | Final detector covariance/total | Yes | No |
| 16 | Deliver full response-MC bootstrap covariance, including response `sumw2` and declared fake/background convention. | h1-systematics | 3, 5 | Final covariance and TUnfold/IBU closure | Yes | Partly after 3 |
| 17 | Decide one tracking baseline; measure h+/h− statistical cross-block; validate charge-dependent tracking if an `A_h` release is planned. | h1-systematics | 5 for final statistical framework | Final asymmetry and total covariance | Yes for `A_h` | Yes |
| 18 | Measure trigger or retain explicit non-final placeholder; decide NucliaWeight applicability. | h1-systematics | None | Final systematic completeness | Yes | Yes |
| 19 | Regenerate alpha0p2 track-level aggregates with `overlap_980`/`n_raw_980`; certify template exposure; demonstrate response-consistent P-ALPHA closure/covariance; assess DIFFVM. | h1-normalization | Repaired response/fake convention from 3 | Background source, final closure | Yes | Partly; provenance can start now |
| 20 | Decide `background_rel` disposition and complete source ledger/covariance integration, including `C_rad` if applied. | h1-systematics | 12/13 if Born level; 15–19 | Final systematic total | Yes | No |
| 21 | Locate approved canonical plotter and regenerate final tables/figures with source statuses and 490-bin validation. | h1-plotting-naf | 7, 20, and 13 if Born level | Note results and release package | Yes | No |
| 22 | Package immutable validation artifacts, final source ledger, release metadata, and independent QA sign-off. | h1-qa-specialist | 7, 13 if applicable, 20, 21 | Final circulation/release | Yes | No |
| 23 | Correct citations, stale prose, status caveats, stub sections, manual figure wiring, untracked-PDF disposition, and build documentation. | h1-analysis-note-writer | None for factual corrections; D1 decisions for final claims | Note quality; final note completion | Yes for final note | Yes |
| 24 | Update blocked note sections, tables, plots, and conclusions strictly from approved final artifacts; rebuild note in audited TeX environment. | h1-analysis-note-writer | 7, 20–22, and 13 if Born level | Analysis-note approval | Yes | No |
| 25 | Stage approved isolated runs, retain manifests/logs, and ensure no central overwrite. | h1-naf-operator | Exact approved task specifications from owners | Execution of tasks 2–5, 8, 10–16, 19, 21 | Indirect | Parallel when tasks are ready |
| 26 | Mechanically execute exact pre-approved small script reruns or artifact validations only when specified by a specialist. | h1-script-runner | Fully specified task, command, acceptance check | Individual task outputs | Indirect | Parallel when specified |

## SECTION F — STATE.md CORRECTIONS REQUIRED

Update `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/STATE.md` to remove or correct these supervisor-audited stale claims:

1. **Line 40 is stale/wrong:** the working-tree `sections/systematics.tex` does **not** contain stale “552%”/“genuine sparse-yield” text. That text exists only in unmerged commit `3311a12`; do not merge it as-is because it would restore retracted numbers invalidated by the 2026-09-25 `fLumi` pb⁻¹ correction.
2. **Line 41 is stale/wrong:** `sections/results.tex` does **not** have five figure names missing `_z005`. It has four `\includegraphics` entries at lines 61, 71, 107, and 124, and all four already include `_z005`.
3. Replace both stale note-defect claims with the actual note audit facts: 53 distinct figure targets all resolve; `2005Hadroo2` is the sole undefined key among ten used; systematics definitions and completeness claims require the corrections listed in D2; and the note builds to 69 pages only with the audited TeX PATH.
4. Reframe any implication that the current systematic total is final: the controlling status is 4 IMPLEMENTED / 6 PROVISIONAL / 9 BLOCKED / 5 DROPPED, and the historical 10-component table is not a publication-grade final definition.
5. Ensure response truth-conservation remains explicitly identified as the highest-priority release blocker, including its 245/245-bin, both-charge failure and binwise ±2.8% scatter; do not reduce it to the small integrated offset.
6. Preserve the correct current-luminosity statement (`351000.0 nb⁻¹ = 351 pb⁻¹`) and current raw/no-`C_rad`, not-Born status; do not restore superseded luminosities or obsolete radiative values.

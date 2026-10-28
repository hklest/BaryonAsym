# Systematic-uncertainty plan: exploratory high-Q² charged-hadron BSA

**Date:** 2026-09-16  
**Status:** written evaluation plan only — exploratory; no central-v6 product is an input or output  
**Scope:** raw charged-hadron beam-spin asymmetry (BSA) from the isolated **data-only** v6bsa trees:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6bsa/central/sidistree/data/`.

This study must remain isolated from `output_v6/`, `condor_v6/`, and every central v6/z005 release product. The trees were made by the additive `ChargedSidisTreeBuilderHighQ2_v6bsa` fork. Apart from the added beam metadata, their branches agree with central v6. The available MC samples (DJANGOH14 and RAPGAP31) are **unpolarized**: they cannot support a BSA closure test, BSA unfolding, or an MC-derived BSA acceptance correction. In particular, H1OO MC polarization is synthetic/random-histogram machinery and the v6bsa smoke test observed `beamPol=0` for every MC entry.

## 1. Observable, fixed estimator, and accounting convention

For each reconstructed hadron charge and each canonical 4D bin, retain the seven equal `phi_h` intervals over `[-pi,pi]`. `phi_h` is the Trento angle in the gamma*+p CM after the W=q+P boost, with

`l_perp = q x k_in`, `h_perp = q x p_h`, and
`sin(phi_h) = (l_perp x h_perp) . qhat / (|l_perp||h_perp|)`.

For every phi bin j form

`A_j = (N+_j - N-_j)/(N+_j + N-_j)`

with Agresti--Coull regularized binomial errors, then use weighted linear least squares for

`A_j = a0 + <|P|> A_sin sin(phi_j) + <|P|> A_cos cos(phi_j)`.

`A_sin` is the raw central estimator `A_LU^{sin phi_h}`; `A_cos` is parity-forbidden and therefore a null diagnostic; `a0` absorbs phi-independent relative helicity-luminosity imbalance. The fixed binning is finalcandidate S3xmerged/zlow005/pT5/eta1.6/radlen5/nhit5: 245 generator-layout kinematic bins per charge, 490 reconstruction bins per charge, and z edges `[0, .05, .1, .2, .3, .5, .7, 1]`. The BSA reporting basis is the 245-bin-per-charge layout (including empty/non-fit statuses explicitly), not an unfolded 490-bin cross-section basis.

The 60-of-658-file smoke is a reason to prioritize control tests, not a measurement: 250 bins fitted, weighted mean `A_sin=+0.0081 ± 0.0074`, `A_cos=-0.0186 ± 0.0073`, pull RMS 1.07, and median chi2/ndf 0.93. The nonzero-looking weighted `A_cos` is only 2.5 standard deviations on this non-independent exploratory aggregation and makes false-asymmetry studies priority 1 alongside polarization.

### Per-source convention and acceptance gate

For any re-analysis variation v, retain the nominal and variation fit output in every (hadron-charge, 4D) bin and define

`delta_s = max_v |A_sin(v) - A_sin(nominal)|`.

For explicitly signed two-sided variations, use `max(|A_up-A_nom|, |A_down-A_nom|)` after verifying they are statistically resolved. Use the same rule for `A_cos` and `a0` as diagnostics, but do **not** replace an `A_sin` uncertainty with a null-test number. Statistical overlap is substantial for cut variations, so estimate the variation-minus-nominal statistical covariance with run/block bootstrap or repeat the fit on paired resamples before interpreting a small displacement. A source is *unevaluated*, not zero, until its recipe and stability checks are complete. Combine demonstrably independent evaluated components in quadrature per bin, preserving source-level correlations for later combined/integrated results. Keep global polarization-scale uncertainty separate from point-to-point systematics.

The primary selection should use `beamPolQuality != 0`, finite nonzero `beamPol`, and the pre-declared BSA selection. The event-level tree variables permit direct re-selections for `Q2es`, `Yes`, `Empz`, `PVz`, `Pt`, `Eta`, `NHitDedx`, `RadLength`, `DcaPrime`, `RunNumber`, `beamPol`, `beamPolQuality`, `beamHelicity`, `beamLeptonCharge`, and `phi_h`/the 4D coordinates. They do not contain a polarized truth sample.

## 2. Prioritized sources and concrete recipes

### Rank 1 — Beam-polarization scale

* **Mechanism.** The fitted sinusoid measures the product `<|P|> A_sin`; hence `A_sin` is proportional to `1/<|P|>`. A scale bias in longitudinal polarization transfers directly to the physics amplitude: `delta A_sin/A_sin = delta P/P` (with opposite sign for the signed shift, but identical magnitude).
* **Cancellation.** It does not cancel in `A_j`: the helicity difference produces the polarization-weighted physics term, and division by `<|P|>` explicitly exposes its calibration. It is correlated across kinematics for a common polarimeter scale and may have component-specific correlations by polarimeter/period.
* **Evaluation recipe.** Make the nominal result separately for `beamPolQuality=1` (TPOL), `=2` (LPOL), and `=3` (combined/other-both), retaining run and beam-charge labels. Establish the effective `<|P|>` using the exact selected tracks/events and the nominal estimator prescription. Apply coherent ±polarization scale variations to the corresponding quality category before refitting, including a conservative correlation model for quality 3; compare TPOL-only, LPOL-only, and quality-3 results after their independent luminosity/helicity compositions are accounted for. The eventual numerical covariance should use official per-run/polarimeter correlated and uncorrelated terms, not a single invented error. H1OO's `H1RunLumi` interface has the relevant TPOL, LPOL, and HERA-orbit correlated/uncorrelated accessors, but these terms are not carried in the v6bsa trees; obtain them from the official H1 polarization/luminosity database before a result is unblinded.
* **Magnitude.** **Placeholder only:** no authoritative H1 HERA-II LPOL/TPOL scale number was located quickly in this workspace or the inspected H1OO builder source. Published H1 HERA-II analyses commonly quote a relative longitudinal-polarization scale uncertainty of order **3–4%** (often period/polarimeter dependent); this is a planning range, not an adopted number. It gives `~(0.03–0.04)|A_sin|`, likely below the statistical error in most current bins because the raw amplitude is small, but potentially material for an integrated nonzero amplitude.
* **Priority:** **1 (leading calibration systematic; globally correlated).** Do not assume TPOL-only and LPOL-only have equal scale errors: retain separate nuisance components until official calibration documentation establishes their magnitudes and correlations.

### Rank 2 — False/instrumental helicity asymmetry

* **Mechanism.** A detector, trigger, reconstruction, beam-condition, or helicity-luminosity-bookkeeping change correlated with helicity creates a numerator term that need not be sinusoidal physics. Phi-dependent occupancy or efficiency makes it especially dangerous. The fitted `a0` only absorbs a phi-independent imbalance.
* **Cancellation.** Helicity-independent acceptance cancels exactly in the fixed-phi ratio. Helicity-correlated acceptance, live time, trigger efficiency, backgrounds, or run mixtures does not.
* **Evaluation recipe.** (i) Treat `A_cos` as the first null test, quote its binwise pulls and a covariance-aware aggregate; require compatibility with zero before interpreting `A_sin`. (ii) Compare charge-combined with charge-separated amplitudes using their proper shared-data covariance. (iii) Repeat separately for `beamLeptonCharge=+1` (e+p) and `-1` (e-p), and for the four data periods/run blocks. The physical sin(phi) BSA should not reverse with lepton beam charge, whereas charge-period detector or bookkeeping effects can; test both a common-amplitude fit and an unphysical charge-sign-flip contrast. (iv) Perform randomized-helicity pseudo-experiments: within each `RunNumber`, randomly permute `beamHelicity` labels while preserving the observed +/− counts and all tracks/kinematics; refit the full seven-bin estimator many times. The pseudo-experiment distribution must be centered on zero with the observed fitted uncertainty/pull width. Also use deterministic run-half, time-order, and detector-condition splits where available. The systematic envelope is the larger statistically significant split/null displacement after trials accounting, or a stated upper bound if all pass.
* **Magnitude.** Target control sensitivity is `<=0.3 sigma_stat` in well-populated bins and `<=0.5 sigma_stat` conservatively before a physics interpretation. The smoke aggregate `A_cos=-0.0186 ± 0.0073` means this source could be O(1 sigma_stat) until full-data charge/period and scramble tests establish otherwise.
* **Priority:** **1 (second leading; gatekeeper).**

### Rank 3 — Polarization sign/helicity assignment

* **Mechanism.** If `beamHelicity` (run-constant H1 convention) and `sign(beamPol)` disagree, events can enter the wrong helicity count. Complete swapping reverses the physical amplitude; a partial mismatch dilutes it and can make run composition mimic an instrumental asymmetry.
* **Cancellation.** No cancellation: the helicity difference itself is corrupted. The fitted `a0` cannot repair a wrong sinusoidal sign.
* **Evaluation recipe.** For all valid-polarization selected tracks, tabulate `(beamHelicity, sign(beamPol), beamPolQuality, RunNumber, beamLeptonCharge)`. Require `beamHelicity in {-1,+1}` and `beamHelicity == sign(beamPol)` as the nominal consistency gate after confirming the H1 convention on a run-level control table. Flag, never silently relabel, mismatches. Refit after removing all mismatch runs/events; take the statistically deconvolved difference, or an upper bound based on mismatch fraction times the observed amplitude. Audit whether mismatches cluster in a quality category or period. A complementary diagnostic is to assign helicity from `sign(beamPol)` and compare only after the convention has been independently verified.
* **Magnitude.** Expected negligible (`<0.1 sigma_stat`) if the tree metadata are consistent, but potentially catastrophic (sign reversal/O(1) relative bias) if not. The smoke QA must not be treated as a sufficient full-sample audit.
* **Priority:** **2 (hard precondition; evaluate before numerical combination).**

### Rank 4 — Missing polarization measurement (`beamPolQuality == 0`)

* **Mechanism.** Such events have no usable event polarization (`beamPol` is zero in the intended data semantics), so they cannot enter a polarization-normalized BSA. Their exclusion changes period, luminosity, and beam-condition composition. It can bias an extrapolation to the full data sample if the missingness is helicity, period, or detector-condition correlated.
* **Cancellation.** They cannot simply cancel in the ratio because `<|P|>` is undefined. An overall luminosity change is irrelevant to an asymmetry, but quality-dependent sample composition and helicity exposure are not.
* **Evaluation recipe.** Nominally exclude quality 0. Report fractions versus `RunNumber`/period, beam charge, reconstructed phi, and 4D bin, and compare the selected quality-0 kinematic/track distributions with quality-positive data without assigning a BSA. Bound selection bias by (a) rerunning quality-positive fits in run/period strata that mimic the quality-0 period composition through reweighting or stratified combination, and (b) varying the retained valid-polarization run set by dropping quality categories/periods that bracket the quality-0 composition. Do **not** impute a period-average polarization into quality-0 events in the baseline. If an official run-level polarization and valid-luminosity record later supports recovery, make it a separately documented estimator and compare it to the exclusion baseline.
* **Magnitude.** Expected `0.1–0.5 sigma_stat` if missingness is a period-composition issue; unbounded until the stated stratification checks are done, because the smoke indicated significant fractions in some periods.
* **Priority:** **2.**

### Rank 5 — Phi_h resolution and seven-bin migration

* **Mechanism.** Finite track/kinematic resolution migrates tracks across the seven phi boundaries. A symmetric angular smearing convolves and therefore dilutes a sin(phi) amplitude by approximately `D=<cos(delta phi)>`; bin averaging produces an additional known sinc-like factor which is already part of the chosen binned fit convention. Nonuniform or phi-dependent migration can also generate harmonic leakage.
* **Cancellation.** Fixed-phi helicity-independent efficiency cancels, but migration changes the phi at which the numerator/denominator are formed and therefore does not algebraically cancel. A correction/bound is needed only for this angular smearing, not a 4D BSA unfolding.
* **Evaluation recipe.** Use unpolarized DJANGOH14 and RAPGAP31 **only** to construct matched-track `phi_h^reco` versus `phi_h^gen` seven-by-seven migration matrices in each 4D bin/charge (or in justified merged control bins). This use does not require a polarized MC. Inject a known truth yield modulation `1 + h P A_in sin(phi_gen)` with a grid of `A_in` values or analytically weight the matched events, pass it through the reco migration matrix, and run the identical seven-point fit. Define `D=A_fit/A_in`; correct only if `D` differs significantly from one and the model spread is controlled, otherwise assign `|1-D| |A_sin|` plus generator spread as a systematic/bound. Quantify off-diagonal fractions, `D`, harmonic leakage into cos and sin(2phi), and the stability after doubled/halved input amplitude. This is a resolution study, not MC BSA closure, acceptance correction, or unfolding.
* **Magnitude.** Expected `<<0.1 sigma_stat` if the phi resolution is much narrower than the bin width `2pi/7≈0.90 rad`; could reach `0.1–0.3 sigma_stat` in low-pT/boundary bins. Do not assign zero before measuring the matrices.
* **Priority:** **3.**

### Rank 6 — Background contamination and background helicity asymmetry

* **Mechanism.** Relevant reconstructed backgrounds are scattered-electron tracks, V0 daughters, non-primary/secondary tracks including detector-interaction/nuclear material products (the diagnostic det.int category is STR-only GEANT secondary pi/K/p, not a pure-material sample), plus negligible low-Q2 leakage. For helicity-independent background with zero BSA, `A_raw=f_sig A_true` and hence `A_true=A_raw/f_sig`; uncertainty in `f_sig` dilutes the result. A background with a helicity-correlated asymmetry can bias rather than merely dilute.
* **Cancellation.** Signal and background acceptance may cancel separately if helicity independent, but the mixed ratio does not remove dilution because numerator and denominator have different physics composition. Overall luminosity cancellation does not cure a background fraction or helicity-dependent background.
* **Evaluation recipe.** Use the existing alpha0p2 diagnostic work to define electron-enriched near-scattered-electron control selections and V0/non-primary/det.int categories where tree flags permit, then repeat the BSA fit for nominal and background-suppressed/enriched selections. Obtain per-(charge,4D,phi) fractions from the alpha0p2 reco-background diagnostic MC and propagate both dilution (`A_raw/f_sig`) and a conservative allowed background asymmetry from data control regions. Perform the alpha cone (`alpha<0.2`) veto as a **study variation**, not an automatic correction; it removes true pi/K/p by only 0.10649% (RAPGAP) or 0.11354% (DJANGOH), but its response treatment is diagnostic/provisional. The available uniform-L351000 diagnostic total background fraction is 0.0798577 (electron 148976.4, V0 132065.3, lowQ2 208.4, det.int 27405.8; denominator 3865073.0). This global number is not a BSA correction: determine the differential fractions and avoid double counting response categories. In the current alpha0p2 response bookkeeping, non-V0 electrons are fake-column handled, V0 daughters are excluded/dropped, and det.int is about 86% matched physical block/14% fake; it must not be blindly pre-subtracted.
* **Magnitude.** A fully unpolarized 8% global contamination corresponds to an approximately 8.7% *relative* correction to a nonzero amplitude, i.e. `~0.087|A_sin|`; likely `<0.2 sigma_stat` in most current bins but may be O(0.5–1 sigma_stat) in electron/V0-rich differential bins or if background carries an asymmetry.
* **Priority:** **3.**

### Rank 7 — Selection/cut variations

* **Mechanism.** Selections can change detector composition, radiation tails, track quality, background fractions, and helicity-correlated exposure. The ratio cancels common stable efficiency but not a selection-dependent helicity or phi effect.
* **Cancellation.** Partial cancellation is expected for helicity-independent changes at fixed phi; it must be demonstrated, not assumed. No acceptance/unfolding correction is available from polarized MC.
* **Evaluation recipe.** On the isolated data trees, repeat the complete estimator with one change at a time around the finalcandidate nominal selection: `Empz>45 GeV`, `|PVz|<35 cm`, `Pt>0.15 GeV`, `|Eta|<1.6`, `NHitDedx>=5`, `RadLength>=5`, and `|DcaPrime/cosh(Eta)|<2`. Predeclare physically adjacent working points after inspecting selected-edge populations (e.g. Empz threshold and/or upper-window alternatives; PVz ±30/±40; Pt 0.10/0.20; eta 1.4/1.8 only if detector coverage is justified; NHitDedx 4/6; RadLength 4/6; DCA 1/3). Keep bin definitions fixed, refit every variation, and use the covariance-aware maximum spread. Require the same null/split QA for a variation; a large shift correlated with `A_cos`, `a0`, or a particular period is a diagnostic, not an automatically valid systematic.
* **Magnitude.** Expected `0.1–0.5 sigma_stat` in bulk bins and potentially `>=1 sigma_stat` near low-pT, eta, or background-rich boundaries.
* **Priority:** **4.**

### Rank 8 — Fit-model and sparse-count prescription

* **Mechanism.** The seven-bin linear model neglects higher harmonics, and finite/sparse counts make the weights and minimum-per-phi requirement influential. A real or instrumental sin(2phi) component can leak into sin(phi) under nonuniform phi population or omissions.
* **Cancellation.** This is estimator bias, not an acceptance normalization, and does not cancel in the ratio.
* **Evaluation recipe.** Refit the same `A_j` points with `a0 + P[A_sin sin(phi)+A_cos cos(phi)+A_sin2 sin(2phi)]`; optionally include cos(2phi) as a diagnostic where statistics permit. Compare the `A_sin` displacement and goodness of fit, retaining the predeclared primary model. Repeat with the unregularized binomial variance where defined and with planned minimum valid-count thresholds (for example baseline and bracketing thresholds chosen before full-data inspection). For each option record fitted-bin loss, pull RMS, chi2/ndf distribution, and `A_cos`. The systematic is the deconvolved envelope only when alternatives are statistically well posed; otherwise label it a sparse-bin limitation.
* **Magnitude.** Expected `<0.2 sigma_stat` in populated bins; `0.5–1 sigma_stat` or bin exclusion is plausible in sparse bins. The smoke pull RMS/median chi2 are reassuring but are not a full-data model-validation substitute.
* **Priority:** **4.**

### Rank 9 — QED radiative effects

* **Mechanism.** Initial/final-state QED radiation changes reconstructed DIS kinematics, W boost, and therefore kinematic-bin/phi assignment. Unlike absolute cross sections, most multiplicative radiative normalization cancels between helicities in `A_j`. A residual matters only if the radiative kernel or migration is helicity/polarization dependent, or if the physics asymmetry varies sharply across radiatively mixed kinematics.
* **Cancellation.** Common helicity-independent radiation and its normalization largely cancel in the ratio. There is no basis for applying the central cross-section `C_rad` factor to a raw BSA amplitude: it is an unpolarized cross-section correction and would not make this BSA Born-level.
* **Evaluation recipe.** First, perform data-only stability variations of radiation-sensitive reconstructed selections (notably Empz, already in Rank 7) and compare by beam charge/period. Second, use the unpolarized RAD/NORAD MC only to map kinematic/phi migration and estimate a model envelope after injecting a range of known sin(phi) modulations at generator level; this is analogous to the phi-resolution test and is **not** a polarized-MC correction or closure. Any result must be reported as model-dependent residual migration sensitivity, not `C_rad` correction. If a dedicated polarized-radiative calculation becomes available, it supersedes this bound.
* **Magnitude.** Expected below the selection/migration envelope, typically `<0.2 sigma_stat`, but unevaluated until the injected-modulation study is completed; it can be larger in radiative tails.
* **Priority:** **5.**

## 3. Explicit non-applicability of the central cross-section components

The central cross-section 10-component budget is not transferable as a list of numerical BSA uncertainties. This BSA is a data-only helicity ratio, with no BSA unfolding and no acceptance correction. The following components therefore **do not apply as central-style terms**:

| Central component | Why it does not apply to this raw BSA | Residual BSA treatment, if any |
|---|---|---|
| IBU data statistical term | No IBU/unfolding is performed. BSA data statistical uncertainty comes directly from the helicity counts and fit covariance. | Use the estimator covariance/bootstraps, not IBU statistical toys. |
| IBU response-MC statistical term | There is no BSA response correction. | MC statistics enter only the optional unpolarized phi/radiative migration bounds. |
| IBU model term (DJANGOH vs RAPGAP) | No generator-derived BSA acceptance/unfolding model is used; MC is unpolarized. | Generator comparison may bound phi/radiative migration only, never a BSA acceptance correction. |
| IBU regularization term | No iterative unfolding / regularization exists. | Not applicable. |
| HFS JES, RCES, and theta | The central terms propagate detector shifts through reconstructed-to-truth migration/unfolding of cross sections. They are not automatically BSA systematics. | Their possible helicity-correlated effect is covered empirically by false-asymmetry, run splits, and selection stability. Dedicated detector shifts are future optional data/MC studies, not borrowed central numbers. |
| Electron energy and theta | Same: no cross-section unfolding or absolute response correction is used. | Residual radiative/kinematic-bin sensitivity is tested through data selection and unpolarized injected-modulation migration studies. |
| Luminosity 2.7% | An overall luminosity scale multiplies `N+` and `N-` equally and cancels in `(N+-N-)/(N++N-)`; the supplied four-period value 351000.0 nb^-1 is not used to normalize this asymmetry. | Do not include 2.7%. Relative helicity live-luminosity imbalance is instead absorbed by `a0` and tested under false-asymmetry controls. |
| Tracking-efficiency -0.5% normalization | A common tracking-efficiency normalization is helicity independent and cancels at fixed phi in the ratio. The central random-drop cross-section systematic is not a BSA correction. | Only a helicity- or phi-correlated tracking change can survive; test through run/charge/time splits and cut variations. Do not import the central -0.5% number. |

The same cancellation statement applies to any overall trigger-normalization scale. A helicity-correlated trigger efficiency is not a normalization error; it belongs to Rank-2 false asymmetry and must be constrained with run/trigger controls where available.

## 4. Correlation and reporting plan

Report each source per hadron charge and 245-bin kinematic layout with `delta A_sin`, relative shift where `A_sin` is safely nonzero, fit status, and the associated changes in `A_cos` and `a0`. Do not quote unstable relative percentages for amplitudes compatible with zero; use absolute asymmetry units and fractions of statistical error. Preserve these correlation classes:

* **Polarization scale:** global/period/polarimeter correlated nuisance components; TPOL and LPOL must remain distinct until official calibration covariance is available.
* **Sign/quality and false-asymmetry tests:** correlated within their affected run periods and often across kinematic bins; do not quadrature them as independent bin noise.
* **Selection, fit model, background, and migration:** generally correlated across nearby bins; retain signed shifts and variation metadata, then construct covariance when a combined result is requested.
* **Luminosity and common tracking normalization:** excluded from BSA total because they cancel, not entered with a zero-error value.

Before combining anything, require: valid helicity/polarization sign audit; randomized-helicity centering; `A_cos` and charge/period split review; no unexplained variation exceeding 50% of a statistically meaningful amplitude; and explicit status `evaluated`, `bounded`, or `unevaluated` for each source. No finite total is a claim of completeness while the polarization calibration or background/migration studies remain open.

## 5. Blinding and interpretation policy

This is a **raw, no-background-subtraction, no-Born-level, exploratory** result. It has no polarized-MC closure, no MC BSA unfolding, and no MC-derived BSA acceptance correction. Keep sign-sensitive aggregate values and full-data physics comparisons blinded to a predeclared review checklist until the Rank-1/2 tests and polarization calibration are reviewed. The existing smoke values are method-validation diagnostics only. No claim about nonzero hadron BSA, charge dependence, factorization, or comparison to phenomenology may be made until background/dilution, instrumental controls, polarization scale, and migration bounds are documented and approved. The 351000.0 nb^-1 absolute luminosity is contextual only and must never be used to imply an asymmetry normalization uncertainty.

## 6. Ranked expected leading uncertainties

Expected sizes below are planning estimates in units of the binwise statistical error; they are not measurements and must be replaced by the recipes above.

| Rank | Source | Expected magnitude relative to stat. error | Treatment / reason |
|---:|---|---|---|
| 1 | False/instrumental helicity asymmetry (including relative live luminosity and helicity-correlated trigger/detector changes) | O(0.3–1) now; require `<=0.5` after controls | Smoke `A_cos` makes this the immediate control gate. |
| 2 | Beam-polarization scale | `(3–4%)|A_sin|`; usually `<0.2`, but globally correlated | Placeholder range only; direct relative propagation and mandatory official calibration retrieval. |
| 3 | Missing/invalid polarization and sign/helicity consistency | `<0.1` if clean; potentially O(1) or sign reversal if not | Precondition audit; quality-0 composition may be 0.1–0.5. |
| 4 | Background dilution/asymmetry | `<0.2` typical; 0.5–1 in contaminated bins | Global diagnostic fraction is 7.98577%, but differential control is required. |
| 5 | Selection/cut variations | 0.1–0.5 bulk; `>=1` at boundaries | Data-only envelope with overlap-aware statistics. |
| 6 | Phi resolution/bin migration | `<<0.1` expected; 0.1–0.3 low-pT/boundary | Unpolarized matched MC injection is permitted for a dilution bound only. |
| 7 | Fit model/count regularization | `<0.2` populated; 0.5–1 sparse | Higher harmonic and weight/threshold envelope. |
| 8 | Residual QED-radiative migration | `<0.2` expected, unevaluated | No cross-section `C_rad` application; use data stability and unpolarized injection bound. |

## 7. Open blockers before an unblinded quantitative total

1. Obtain authoritative H1 HERA-II LPOL/TPOL polarization scale and correlation terms by period/quality; no authoritative numerical scale value was located in the quickly inspected workspace/H1OO material.
2. Complete full-658-file sign/quality audit, run-period/beam-charge false-asymmetry suite, and within-run randomized-helicity pseudo-experiments.
3. Produce matched unpolarized-MC phi migration matrices and injected-modulation dilution bounds, explicitly without claiming polarized-MC closure or acceptance correction.
4. Establish differential background fractions/control-region asymmetries and a response-consistent treatment; alpha0p2 diagnostics remain provisional.
5. Predeclare variation thresholds and covariance-aware comparison procedure before examining full-data shifts.

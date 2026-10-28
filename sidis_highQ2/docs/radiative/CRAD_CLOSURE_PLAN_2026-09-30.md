# Decision-ready closure plan: gen-only QED `C_rad` for v6/z005

**Date:** 2026-09-30  
**Scope:** Writing and audit only. This plan does not modify the central release, run a campaign, or approve a Born-level result.  
**Release contract:** 490 published rows (`245 h+ + 245 h-`), `C_rad = sigma_nonrad/sigma_rad`.  
**Current release status:** raw/no-background-subtraction, `no C_rad`, not Born level. `c_rad_applied=false` remains mandatory until every applicable gate below passes.

## Audit basis and decision rule

The central IBU CSV is

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/ibu/cross_sections.csv`.

Its columns include `cross_section_nb`, `stat_unc_nb`, `total_unc_nb`, and `total_unc_rel`; metadata fixes `lumi_nb=351000`, `490` rows/method, and identifies the product as raw/no-`C_rad`. It is therefore an **absolute differential hadron cross-section release in nb**, not a per-DIS-event multiplicity table. The release has no DIS denominator column and metadata explicitly says multiplicities are blocked because no DIS denominator is present.

Consequently, a Born correction for this release must be a bin-wise **cross-section ratio**, including the generator-predicted radiative change in selected DIS rate as well as the conditional hadron rate. A multiplicity-only ratio is the correct object only for a separately constructed `(1/N_DIS) dN_h/dPhi` measurement in which the data-side DIS normalization is independently Born-corrected such that the DIS-rate factor cancels. That is not the present 490-row observable.

The preferred future truth phase space is the open truth space of the response. The central response must be re-audited before application because the documented code history has conflicting EMPZ statements; do not infer the truth cut from a comment or a historical script.

## A. Normalization question — highest priority

**Precise question.** Does the DIS event-rate factor belong inside `C_rad`, or cancel against data normalization, and should the approved correction be about `1.05` or about `1.1372` at `GenEmpz>45`?

**Current evidence with numbers.** The release CSV is in `cross_section_nb`; its central values include, for example, h+ row 0 `1.8920141175 nb`. It is not a multiplicity. The diagnostic binned cross-section ratio (variant A, `GenEmpz>45`) is h+ `1.1375979`, h- `1.1366709` (headline `1.1372`). The conditional per-fiducial-DIS-event multiplicity ratios are only e+ `1.0555`, e- `1.0677`. The distinction is expected: the former equals a selected-DIS event-rate ratio times the conditional hadron-multiplicity ratio; the latter omits the event-rate factor. The previous event-rate decomposition using only 40 files with full-manifest luminosities is invalid and must not be used.

**Candidate resolutions.**
1. **Absolute-cross-section resolution (recommended):** apply the binned `sigma_nonrad/sigma_rad` cross-section ratio to `cross_section_nb`.
2. Multiplicity resolution: use only the conditional multiplicity ratio, but only if the release is replaced by an explicitly per-DIS-event multiplicity and its DIS denominator receives the matched Born correction.
3. No correction: retain raw release until the complete cross-section-ratio candidate closes all gates.

**Exact discriminating test.** Rebuild the event-rate/multiplicity factorization over the *full retained manifests*, separately in e+ and e-, with file-scoped event joins: verify numerically in each selected control and in each 490 bin that

`C_xsec = (sigma_DIS_nonrad/sigma_DIS_rad) * (<n_had>_nonrad/<n_had>_rad)`

within propagated MC statistics. Then trace the downstream formula from the CSV to the released observable and verify it has no `N_DIS` division. The latter audit is already strongly indicated by the CSV schema and metadata; the former establishes that the generator ratio was formed coherently.

**Existing script path.** Multiplicity side: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/crad_diagnostics_2026-09-28/05_isr_multiplicity_v2.py`. Rebuild/490-row candidate side: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py` and `crad_v3_finalize.py`.

**Acceptance criterion.** CSV/metadata audit proves `cross_section_nb` is the application quantity; full-manifest factorization closes to statistical precision, with no cross-file `(RunNumber,EventNumber)` join; a reviewer signs the formula mapping. Any mismatch above `max(0.5%, 3 sigma_stat)` in an integrated charge or unexplained bin-local mismatch blocks application.

**Recommended default.** Treat the 490-bin result as an absolute cross section and use the full binned cross-section ratio, never the `~1.05` multiplicity ratio, conditional on the gates below.

## B. OLD versus NEW NONRAD disagreement

**Precise question.** How can OLD and NEW have different integrated conditional multiplicities but agree in the quiet-y `C_rad` control, and which pool is nominal?

**Current evidence with numbers.** `<n_had>` per fiducial DIS event is OLD `14.7873` (e+) / `14.9437` (e-) and NEW `14.0358` / `13.9610`, a roughly 5% difference. NEW is close to RAD with ISR veto (`14.0834` / `14.0509`). Yet script 01 reports quiet-control OLD/NEW `C_rad` ratios of `0.996` (e+) and `1.003` (e-), i.e. agreement within 0.4%. These statements can coexist because an integrated conditional multiplicity averages a different phase-space mixture, whereas a local `C_rad` is a ratio of selected weighted cross sections in a restricted y/bin control; compensating event-rate and shape differences can preserve the latter. A single integrated agreement cannot justify pooling in all 490 cells.

**Candidate resolutions.**
1. Pool OLD+NEW at raw-weight level only after bin-local compatibility passes.
2. Use NEW alone as nominal because it is the current unique-content 530/530 production; retain OLD as a compatibility/control sample.
3. Use OLD alone; reject NEW if exposure or source audit fails.

**Exact discriminating test.** With the exact release-contract selection, source-pool gates (`|PDG| in {211,321,2212}`, `IsGtr!=0`, `Mother2==-1`, V0-daughter veto), exposures, and beam mixture, make a 490-row sidecar comparing OLD and NEW nonrad cross-section proxies separately by beam and in the mixed result. Record ratios, correlated/independent pulls, zero counts, effective statistics, manifests, and charge/bin keys. This test must be bin-local, not an integrated multiplicity or a single low-y control.

**Existing script path.** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/nonrad_old_new_v2_ratio_20260922/nonrad_old_new_v2_ratio.py`; the quiet-y preliminary control is `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/crad_diagnostics_2026-09-28/01_closure_split_old_vs_new.py`.

**Acceptance criterion.** All supported bins have finite OLD and NEW estimates; no coherent charge-, z-, or pT-dependent discrepancy; weighted median `|OLD/NEW-1| <= 2%`, 95th percentile `<=5%`, and no bin exceeds `max(10%,3 sigma_combined)` without an approved sparse treatment. Exposure reconciliation must identify the exact component manifests and one approved luminosity per component.

**Recommended default.** NEW is the nominal production population, but do not discard OLD or pool either sample until the 490-bin sidecar passes. If it passes, pool raw weights and divide once by the approved summed exposure; if it fails, NEW-only is the default only after a documented source/provenance review.

## C. EMPZ convention and reco discrepancy

**Precise question.** Does a truth `GenEmpz` threshold belong in a gen-only correction when unfolding targets open Born truth space, and is the reco EMPZ inconsistency release-affecting?

**Current evidence with numbers.** Variant-A integrated binned ratios are `1.0999` (none), `1.1127` (`35--70`), `1.1372` (`>45`), `1.1699` (`>50`), and `1.3632` (`>55`). Central-v6 truth-response documentation says GenEmpz is read for diagnostics and not cut, so a truth cut would change the Born phase space. The response code cited in the issue executes reco `35<Empz<70` at `build_migration_th2_per_charge_v6.py:172-178,511-521`; Stage 1 has a `(30,75)` envelope; a Stage-2 comment says `[45,65]`; an analyst statement says data `>45`. The current release metadata is not sufficient to resolve these conflicting declarations.

**Candidate resolutions.**
1. **Open-truth resolution (recommended):** use no `GenEmpz` threshold in `C_rad`, matching an open truth denominator; apply the reco selection only on the radiative and nonradiative reconstructed event definition if the correction is defined to the selected data observable.
2. If the approved response truth fiducial is proven to be `GenEmpz>45` (or `35--70`), use exactly that same truth cut in both RAD and NONRAD `C_rad`.
3. Rebuild the response/release with a new explicitly documented reco and truth EMPZ contract before applying any correction.

**Exact discriminating test.** Read the exact central response producer, its saved response metadata, and a representative central input/output selection audit. Count reco events after each cut stage and truth body/miss entries for: none, `>45`, `35--70`; verify the truth-bin total, body+miss conservation, and published 490-bin identity. Confirm the data-vector selection from the production script rather than comments. Compare the selection recorded in `metadata.json`, run manifests, and `ibu/cross_sections.csv` provenance.

**Existing script path.** `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`; EMPZ scan: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/crad_diagnostics_2026-09-28/03_empz_mode_scan.py`; candidate histogrammer/finalizer paths in A.

**Acceptance criterion.** One executable reco selection and one truth fiducial are documented and reproduced from central artifacts. The `C_rad` truth event and hadron fiducials are identical to the unfolding truth denominator, including whether EMPZ is absent. Any disagreement between actual data-vector and response reco EMPZ selection is release-affecting because it invalidates response/data matching; a stale comment alone is not release-affecting once executable code and artifacts agree.

**Recommended default.** No truth `GenEmpz` cut, because the stated central truth denominator is open; therefore use the no-cut table only as a **provisional target**, not an approved correction. Treat the reco EMPZ audit as release-affecting until the actual central data-vector and response execution are shown identical.

## D. P-CRAD-LUMI

**Precise question.** What turns diagnostic exposures into publication-grade exposures, and what residual luminosity error matters in the ratio?

**Current evidence with numbers.** RAD is exact oomclumi-verified over `1373/1373` retained chunks: e+ `16,206,393.0` and e- `13,534,017.0 nb^-1`. OLD/NEW NONRAD are only `DIAGNOSTIC_GENDIS_EVENT_LEVEL`: OLD e+/e- `12,920,115.855/12,841,572.442`, NEW `1,717,376.821/1,699,702.771`, combined `14,637,492.676/14,541,275.213 nb^-1`. A conflicting NEW exposure path differs by factors about `1.4670` (e+) and `1.4531` (e-), and e- GenDIS has a reported `4.67%` count shortfall. `L_data` cancels and is not part of this gate.

**Candidate resolutions.**
1. Obtain generator-log/MCDB provenance and oomclumi-equivalent weighted exposure for every retained OLD/NEW input.
2. Exclude a component that cannot be traced and recompute a component-complete candidate.
3. Retain diagnostic exposure only for study plots; it cannot support publication.

**Exact discriminating test.** For every OLD and NEW file in the frozen manifest, reconcile ReqID/genID, generator log, generated weighted sum, SIGMT units, retained HAT/DST/tree identity, duplicates/overlaps, and file attrition. Independently reproduce `L=sum(w1*w2)/SIGMT` and require component and beam totals to agree with the signed manifest. Audit the missing e- fraction explicitly.

**Existing script path.** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/nonrad_old_new_v2_ratio_20260922/nonrad_old_new_v2_ratio.py` (run only in the appropriate batch workflow); final candidate reconstruction is `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v0_isgtr_20260925/produce_crad_v0_isgtr.py` after constants are approved.

**Acceptance criterion.** 100% of retained OLD/NEW files map one-to-one to certified generator provenance; no unresolved duplicate/overlap; weighted exposure and SIGMT units reproduce each component within numerical precision; e- shortfall explained or removed with its manifest regenerated. A signed machine-readable exposure map, manifest hashes, and independent reproduction are required.

**Recommended default.** Keep `P-CRAD-LUMI` open. In `C=(S_N/L_N)/(S_R/L_R)`, common scale errors cancel, but relative RAD-vs-NONRAD exposure error does not: `delta C/C = delta L_R/L_R - delta L_N/L_N` (with beam-mixture extensions). Thus a 1% relative exposure mismatch shifts `C_rad` by 1%; a common 1% scale applied to both cancels.

## E. P-CRAD-SPARSE

**Precise question.** Is the sparse-bin blocker closed by the new V0/IsGtr candidate, and what must happen to tails?

**Current evidence with numbers.** V0/IsGtr reports `490/490` finite rows, median relative statistical uncertainty `0.42%`, maximum `5.74%`. The obsolete candidate had `479/490` finite rows and maximum `109.54%`. This closes the former *numerical finite-row/MC-statistics* failure, provided the candidate’s selection and exposure gates close; it does not by itself approve a physics correction.

**Candidate resolutions.**
1. No masking/merging for the existing 490-bin contract, retain all rows with their propagated MC statistical uncertainties.
2. Mask unsupported bins in a future release (not allowed for an unqualified 490-bin release).
3. Merge bins only through a separately approved revised physics binning and rerun unfolding/release, never post-hoc average a correction.

**Exact discriminating test.** Recompute the candidate from approved inputs and tabulate all 490 `C_rad`, uncertainty, `N_eff`, RAD/NONRAD sums, finiteness, range, and neighborhood continuity. Repeat after OLD/NEW and exposure decisions; compare to the obsolete candidate only as an audit.

**Existing script path.** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v0_isgtr_20260925/produce_crad_v0_isgtr.py`; release-contract finalizer alternative: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_finalize.py`.

**Acceptance criterion.** `490/490` finite, no zero denominator, relative MC-stat uncertainty `<10%` in every application row, and no unreviewed discontinuity beyond `max(20%,3 sigma_combined)` relative to adjacent z/pT bins. Values outside `[0.5,1.2]` require an explicit physics/provenance review, not automatic rejection.

**Recommended default.** No masking and no merging for the 490-row contract if the rebuilt approved candidate retains the present `max=5.74%`; otherwise do not apply `C_rad` to the affected rows and do not silently impute them.

## F. RAPGAP versus DJANGOH coherence and model systematic

**Precise question.** Is RAPGAP response with DJANGOH `C_rad` incoherent, what cross-check resolves it, and what uncertainty belongs to `C_rad`?

**Current evidence with numbers.** Central metadata identifies RAPGAP31 as the response and DJANGOH14 as an available model response. The radiative baseline is DJANGOH. RAPGAP’s old aggregate-luminosity result near `1.405` and fLumi-v3 diagnostic near `1.0248` are both quarantined, so neither is a valid generator envelope. The no-cut versus `>45` DJANGOH EMPZ movement is `1.0999 -> 1.1372` (about 3.4%), but it is a fiducial-definition variation, not a generator-model comparison.

**Candidate resolutions.**
1. Keep RAPGAP response and DJANGOH correction, conditional on a matched DJANGOH-response closure that shows the product is stable.
2. Replace the response and correction consistently with DJANGOH only after response-method review.
3. Build a provenance-closed matched RAPGAP RAD/NONRAD pair and use the DJANGOH--RAPGAP difference as a model envelope.

**Exact discriminating test.** Apply the approved DJANGOH `C_rad` candidate to otherwise identical 490-bin outputs unfolded with the central RAPGAP response and the available DJANGOH response; compare per bin and integrated values with shared-data covariance handled. Then, only after RAPGAP RAD/NONRAD pairing, domain, exposure, and sparse QA pass, compute a matched RAPGAP `C_rad` and compare it with DJANGOH bin-by-bin.

**Existing script path.** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/rad_crad_vs_detector_mc_20260921/compare_rad_crad_vs_detector_mc.py`; candidate inputs from the V0/IsGtr producer in E.

**Acceptance criterion.** First cross-check: response-swap shift is below the already-approved response-model systematic or is added as a separate correlated source. Generator cross-check: a provenance-closed RAPGAP table has `490/490` supported rows and its absolute per-bin difference from DJANGOH is documented; no hardcoded/global-lumi RAPGAP route is accepted. No 5% claim can be made until that table exists.

**Recommended default.** Mixed generator use is a coherence risk, not automatically an error. Carry `C_rad` MC statistics separately; do **not** call EMPZ variation a generator systematic. Until a valid RAPGAP RAD/NONRAD table exists, do not assign a generator-envelope number; retain a named pending model-systematic gate. Once valid, use the signed bin-wise half-envelope `|C_DJ-C_RG|/2` (or full difference if the review adopts conservative one-sided coverage), correlated by bin/charge as approved. Keep open-vs-`>45` EMPZ as a fiducial-definition closure test, not an uncertainty to absorb an unresolved contract.

## G. Application mechanics

**Precise question.** How is an approved `C_rad` applied to the 490-row release without overwriting central files?

**Current evidence with numbers.** The input is the central `ibu/cross_sections.csv`, with 490 rows and stable key `(charge, bin_index)`; the correction tables carry `(charge, bin_index_per_charge, global_bin_index)` and bin edges. Current release values are absolute `cross_section_nb` and uncertainties in nb. No correction has been applied.

**Candidate resolutions.**
1. Copy-and-derive a new immutable Born-corrected release directory (recommended).
2. Mutate central CSVs (forbidden).
3. Publish a sidecar only, without an explicit corrected release (allowed only as a pre-approval diagnostic).

**Exact procedure after approval.**
1. Freeze the approved 490-row `C_rad` CSV, summary, exposure-map hash, selection contract, and model-systematic decision.
2. Copy the entire central release into a new sibling directory named `bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10_borncrad_dj14_openempz_<approval-date>`; never write under the central directory.
3. Join only on `(charge, bin_index)` after requiring equality of `global_bin_index`, cell, `(Q2,x,z,pT_h)` edges, and exactly one match per row. Reject 489/491 rows, duplicated keys, or an edge mismatch.
4. Set `cross_section_nb_born = cross_section_nb_rad * C_rad`. Preserve original raw columns unchanged. Set statistical variance, under independent MC statistics, to `Var_stat_born = C_rad^2 Var_stat_release + cross_section_nb_rad^2 Var_stat_Crad`; retain any C_rad bin covariance if supplied. For total uncertainty, do not blindly scale an already-total diagonal: construct a documented covariance `V_born = D_C V_release D_C + D_sigma V_Crad D_sigma` plus approved cross terms; add the generator-model C_rad covariance as a separately labelled systematic. Luminosity common-mode treatment remains unchanged by a dimensionless correction.
5. Produce corrected IBU and BBB CSVs, copied bin map, full covariance products, `crad_application_manifest.json`, a `C_rad` sidecar, delta-QED (`1-C_rad`) plots, ratio-to-raw plots, and validation JSON.

**Existing script path.** Candidate generation: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v0_isgtr_20260925/produce_crad_v0_isgtr.py`; the release-compatible histogram path is the v3 worker/finalizer in A. A dedicated application script does not yet exist and must be reviewed before execution.

**Acceptance criterion.** 490/490 exact joins; no central-file write; all corrected values/uncertainties finite; output preserves raw values; total and per-charge sums reproduce direct multiplication; covariance is symmetric/positive-semidefinite to numerical tolerance; metadata says `Born-level via DJANGOH14 C_rad`, names all unresolved/non-applied sources, and records hashes. Require independent recomputation and review sign-off.

**Recommended default.** Prepare only this immutable-copy procedure. Do not execute it while A--F or central response/data EMPZ consistency remains open.

## H. Residual trend

**Precise question.** Is the variant-A trend physical or a defect: `C_rad(z)` rises `1.132 -> 1.299` (+14.7%) while `C_rad(pT_h)` falls by 4.8%?

**Current evidence with numbers.** A/B/C variants agree in integrated normalization to below 0.007%, but their shapes differ: z rise is +14.7% (A), +7.2% (B), +10.7% (C); pT fall is -4.8% (A), -1.2% (B), -1.9% (C). Thus it is not established as a detector effect, but is sensitive to the photon-axis/kinematic construction. ISR does physically reduce effective energy and can correlate with z and pT spectra, so a trend is plausible; its amplitude cannot yet be called physical while normalization, truth fiducial, OLD/NEW compatibility, and response coherence are unresolved.

**Candidate resolutions.**
1. Retain it as a physical generator prediction if it survives a fully independent reconstruction and generator cross-check.
2. Treat it as a kinematic-definition/model systematic if A/B/C and matched-generator comparisons quantify it.
3. Reject it as a defect if it disappears under release-matched truth variables, direct four-vector closure, or source-pool tests.

**Exact discriminating test.** Recompute all 490 rows from the same approved partials with A/B/C definitions; verify gamma*p-CM `pT_h` against the central migration helper at maximum absolute difference `0.0 GeV`; perform per-bin OLD/NEW and DJANGOH/RAPGAP comparisons; then compare the trends after the response-swap test. Plot signed difference maps and delta-QED, not only integrated projections.

**Existing script path.** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/crad_diagnostics_2026-09-28/03_empz_mode_scan.py` and `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py`; generator/response comparison path in F.

**Acceptance criterion.** Four-vector and central-helper pT closure is exact to floating-point precision; trend is stable within combined MC statistics under the approved truth convention; no OLD/NEW or generator comparison shows a larger unexplained shape difference. If A/B/C spread exceeds the approved model envelope in any supported bin, the trend remains a model/fiducial systematic rather than a physics claim.

**Recommended default.** Do not smooth or flatten the trend. Classify it as an unresolved, physically plausible but model-sensitive feature until the discriminating test passes.

## Dependency-ordered closure tasks

1. **Establish A and C contracts first:** audit the release CSV/metadata and executable central data-vector/response selections; decide absolute-cross-section correction and the open versus cut truth EMPZ fiducial. This blocks every candidate choice and application.
2. **Close D:** certify OLD/NEW exposures and e- attrition against frozen manifests. This blocks valid normalization and pooling.
3. **Close B:** run the source-pool-matched 490-bin OLD/NEW compatibility sidecar using approved exposures. This blocks a pooled nonrad numerator.
4. **Rebuild and validate the candidate:** enforce the approved A--D contract, then run E finite/range/statistical QA and produce delta-QED. This blocks all physics comparison and application.
5. **Close F and H together:** perform DJANGOH-response/RAPGAP-response comparison; obtain a provenance-valid RAPGAP RAD/NONRAD comparison or explicitly defer the generator envelope; settle trend classification. This blocks the C_rad model systematic.
6. **Execute G only after review approval:** create a new immutable Born-corrected release, with covariance propagation and validation gates. This is blocked by steps 1--5 and must never alter central v6.

## Provisional nominal table for planning only

If the recommended **open-truth, absolute-cross-section** contract is confirmed, the planning target is the 490-row variant-A no-`GenEmpz` diagnostic table:

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_empz_none_nominal_490rows.csv`

with integrated values h+ `1.0999516884`, h- `1.0999042249`. It is not an approved release input: its exposure label is `DIAGNOSTIC_MANIFEST_LEVEL`, and its prior mandatory closure gate was unavailable from older non-y-resolved partials. The V0/IsGtr `490/490` candidate at `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_v0_isgtr_20260925/results/crad_truth_v2_490rows.csv` is useful for sparse-statistics QA (integrated h+ `1.14063313`, h- `1.14017023` in `STATE.md`) but uses `GenEmpz>45` and cannot be the open-truth nominal table.

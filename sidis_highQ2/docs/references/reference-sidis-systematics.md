<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current systematics reference. Read together with `plan-convergence-to-publication.md` and the v3 builder-update plan.

# Purpose and Scope

This reference is the permanent SIDIS systematics playbook for future H1 charged-particle supervisor sessions. It covers inclusive charged-track SIDIS only, in the low-Q2 (5 < Q2 < 100 GeV2) and high-Q2 (Q2 > 150 GeV2) regimes, and defines how the SIDIS supervisor should drive systematic evaluation by calling the existing specialists explicitly rather than silently re-scoping `h1-systematics`; out of scope are V0, identified-proton, Xi, and exclusive phi. Sources of truth are, in order, the distilled critic brief `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/critic-sidis-systematics-distilled.md`, the literature inventory `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md`, and the framework audit `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md`.

# Hard Preconditions (Validity Gates)

> [!STOP]
> STOP: Do not quote any low-Q2 charged-SIDIS multiplicity or A_h systematic number until the low-Q2 charged backport fixes are applied and the low-Q2 MC is fully reprocessed and re-merged. This is the single most important blocker and invalidates the acceptance correction chain if unmet. Source: critic brief `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/critic-sidis-systematics-distilled.md` lines 217-219; fixes doc `/Users/hklest/Desktop/opencode/H1/proton_HiQ/docs/charged-particle-fixes-for-lowQ2.md` Section 1; framework `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md` lines 406, 570, 703-708.

1. Low-Q2 charged backport fixes not applied plus MC not reprocessed.
   - Status today: Blocked. The five backport fixes identified in `/Users/hklest/Desktop/opencode/H1/proton_HiQ/docs/charged-particle-fixes-for-lowQ2.md` are required, and the distilled brief classifies this as a hard blocker that blocks the low-Q2 pipeline entirely (`critic-sidis-systematics-distilled.md` lines 103-106, 217-219).
   - What unblocks it: Backport Fixes 1-5, preferably plus `GenIdx`, into the low-Q2 builder; rerun RAPGAP31 and DJANGOH14 low-Q2 MC production; re-merge; validate that every per-run output contains both `ChargedTot` and `ChargedTotGen` with non-zero entries.
   - Owning specialist: `h1-mc-expert` owns the reprocessing; `h1-data-validator` owns post-fix verification with file-level checks delegated to `h1-root-analyst`.

2. Low-Q2 acceptance bugs remain unresolved.
   - Status today: Blocked. The low-Q2 merged MC shows the incomplete `ChargedTotGen` problem, a fiducial mismatch, and a numerator definition defect; Bug #1 is the root cause of epsilon = 1.000 clipping and Bugs #2 and #3 still bias acceptance after Bug #1 is fixed (`/Users/hklest/Desktop/opencode/H1/proton/docs/research-acceptance-debug.md` lines 14-25, 28-115, 118-235).
   - What unblocks it: Reprocess the low-Q2 MC with complete charged trees, tighten or re-derive the fiducial definition to match analysis-level charged acceptance, and require fiducial matched-gen selection in the numerator or equivalent matched-gen kinematic cuts.
   - Owning specialist: `h1-data-validator` for the validity gate and acceptance sanity checks, with tree-content inspection by `h1-root-analyst`; implementation-side reruns go through `h1-production` after code changes by the responsible analysis code owner.

3. High-Q2 e- recovery is missing.
   - Status today: Blocked for period or beam-charge systematic evaluation and for any complete high-Q2 A_h reference. The distilled brief marks the period or beam-charge split as a target source that is currently blocked (`critic-sidis-systematics-distilled.md` lines 37-40, 108-109; framework lines 284-288, 574-579, 698-702).
   - What unblocks it: Recover and propagate the em04, em05, and em06 high-Q2 data periods through the charged-particle stage so both beam-charge compositions can be compared.
   - Owning specialist: `h1-data-validator` owns the coverage check and split definition; `h1-production` owns the production reruns once the missing data path is restored.

4. JSON-emitting systematics driver is absent.
   - Status today: Blocked for the automated pipeline as specified, though not for manual physics reruns. The distilled brief and framework both note that the existing charged scripts emit plots and text summaries, not structured JSON outputs (`critic-sidis-systematics-distilled.md` lines 107-107; framework lines 722-729).
   - What unblocks it: Add or adopt a systematics driver that runs the requested variations, re-emits the required JSON products, and records variation provenance for each source.
   - Owning specialist: `h1-systematics` owns the aggregation contract; `h1-production` owns orchestration of reruns; the SIDIS supervisor should call both explicitly until a dedicated driver exists.

# Pipeline Overview

1. Validate merged inputs and ROOT integrity.
   - Owner: `h1-data-validator` with `h1-root-analyst`
   - JSON-in: ROOT files
   - JSON-out: `validation_inputs_<regime>.json`
   - Wallclock: about 30 min
   - Mode: interactive
   - Description: Check branch presence, entry counts, run-range coverage, and both charged trees.

2. Validate regime-specific hard preconditions.
   - Owner: `h1-data-validator`
   - JSON-in: `validation_inputs_<regime>.json`
   - JSON-out: `validation_prereqs_<regime>.json`
   - Wallclock: about 15-30 min
   - Mode: interactive
   - Description: Enforce low-Q2 backport gates and record high-Q2 blockers or placeholders.

3. Provide luminosity and weight semantics only.
   - Owner: `h1-normalization`
   - JSON-in: `validation_prereqs_<regime>.json`
   - JSON-out: `normalization_<regime>.json`
   - Wallclock: about 10-20 min
   - Mode: interactive
   - Description: Publish luminosity and weight semantics without adding luminosity to multiplicity or A_h totals.

4. Compute nominal per-charge acceptance and response support.
   - Owner: `h1-acceptance` with optional `h1-unfolder`
   - JSON-in: `validation_prereqs_<regime>.json`, `normalization_<regime>.json`
   - JSON-out: `acceptance_<regime>_nominal.json`
   - Wallclock: about 0.5-2 h
   - Mode: interactive or batch
   - Description: Build nominal charge-separated acceptance and any justified response objects.

5. Compute nominal per-charge C_rad or placeholder status.
   - Owner: `h1-radiative-correction`
   - JSON-in: `validation_prereqs_<regime>.json`, `normalization_<regime>.json`
   - JSON-out: `crad_<regime>_nominal.json`
   - Wallclock: about 0.5-2 h
   - Mode: interactive
   - Description: Compute charge-separated radiative corrections or emit explicit placeholder status.

6. Compute nominal multiplicities or per-event yields per charge.
   - Owner: `h1-cross-section`
   - JSON-in: `acceptance_<regime>_nominal.json`, `crad_<regime>_nominal.json`, `normalization_<regime>.json`
   - JSON-out: `multiplicity_<regime>_nominal.json`
   - Wallclock: about 0.5-2 h
   - Mode: interactive
   - Description: Derive nominal charge-separated multiplicities or yields.

7. Compute nominal A_h.
   - Owner: `h1-asymmetry`
   - JSON-in: `multiplicity_<regime>_nominal.json`
   - JSON-out: `asymmetry_<regime>_nominal.json`
   - Wallclock: about 10-30 min
   - Mode: interactive
   - Description: Form the hadron charge asymmetry from the nominal charge-separated inputs.

8. Run one source variation at a time.
   - Owner: per-source owner via `h1-production`
   - JSON-in: nominal JSONs plus variation spec
   - JSON-out: `acceptance|crad|multiplicity|asymmetry_<regime>_<source>.json`
   - Wallclock: 0.5-3 h or 24-72 h
   - Mode: mixed
   - Description: Re-execute only the dependency cone touched by the variation.

9. Aggregate per-source deltas, cancellation classes, and totals.
   - Owner: `h1-systematics`
   - JSON-in: nominal JSONs plus variation JSONs
   - JSON-out: `systematics_<regime>.json`
   - Wallclock: about 10-30 min
   - Mode: interactive
   - Description: Compute per-bin deltas, cancellation tags, and quadrature totals.

10. Plot and publish the machine-readable manifest.
   - Owner: `h1-plotting`
   - JSON-in: `systematics_<regime>.json` and nominal JSONs
   - JSON-out: plot manifest plus figures
   - Wallclock: about 0.5-2 h
   - Mode: interactive
   - Description: Render stat-plus-syst plots, dominant-source views, and provenance.
| Step | Owner | Variations triggering re-execution of this step |
|---|---|---|
| 1 | `h1-data-validator` / `h1-root-analyst` | Any new merged file, any reproduction, any alternate generator file, any restored period sample |
| 2 | `h1-data-validator` | Any change in regime readiness, backport status, nonRad availability, e- recovery, or JSON-driver availability |
| 3 | `h1-normalization` | Period split, beam-charge split, or any change in weight semantics; not required for pure cut-only reruns if semantics unchanged |
| 4 | `h1-acceptance` / `h1-unfolder` | RAPGAP <-> DJANGOH swap, cut scans, per-period split, binning change, unfolding method or regularization change, match-angle, exclusivity, fiducial change, reco pTh mass variation, electron or hadronic scale rerun |
| 5 | `h1-radiative-correction` | C_rad model swap, availability of nonRad samples, electron-scale rerun if C_rad tables depend on the shifted kinematics |
| 6 | `h1-cross-section` | Any variation that changes acceptance, C_rad, period subset, or cut definition |
| 7 | `h1-asymmetry` | Any variation that changes h+ or h- multiplicities, including all nontrivial source variations |
| 8 | `h1-production` | Every non-nominal systematic source |
| 9 | `h1-systematics` | Whenever a new or revised variation JSON appears |
| 10 | `h1-plotting` | Whenever section 9 outputs are updated |

# Systematic Source Catalog

## Low-Q2 Ordered Catalog

| # | Source name | Variation method | Typical magnitude | Bin-to-bin correlation class | Observable affected | Owning specialist | Executable today | Notes |
|---|---|---|---|---|---|---|---|---|
| 1 | MC model dependence / acceptance-response model swap | Swap RAPGAP nominal acceptance-response input for DJANGOH alternative | 1.5-4.3% on moments; up to about 4% on multiplicity distributions from H1 Sys.2 (`/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md` lines 83-91, 552-555; paper pointer in critic lines 19-20) | coherent | both | `h1-acceptance` | Placeholder | Target source is authoritative, but low-Q2 trust depends on the hard validity gate |
| 2 | Track efficiency, split central vs combined, with charge-dependent residual for A_h | Propagate 0.5% central-track and 10% combined-track efficiency uncertainties; report charge-dependent residual separately for A_h | 1-5% on P(N), 1% on moments from H1 Sys.5 (`research-sidis-systematics-literature.md` lines 124-138, 559-562; critic lines 20-21) | central/combined split; detector term treated effectively uncorrelated per bin for charged spectra practice | both | `h1-data-validator` with `h1-root-analyst` support | Placeholder | For A_h, separate common tracking from the non-cancelling charge-sign residual per critic line 92 and matrix lines 131-133 |
| 3 | V0 / conversion contamination | Vary K0s daughter contribution by 50% and conversions or Dalitz by 0.5%; keep Lambda / anti-Lambda non-cancelling subset explicit for A_h | 1-7% on P(N), up to 2% on moments from H1 Sys.6 (`research-sidis-systematics-literature.md` lines 142-154, 563-565; critic lines 21-22) | coherent for inclusive contamination; partial cancellation structure for A_h | both | `h1-data-validator` | Placeholder | K0s is mostly cancelling in A_h, Lambda / anti-Lambda is not; treat those subcomponents separately in reporting |
| 4 | Diffractive contribution | Scale diffractive MC normalization by a factor of 2 in H1 style | 0.5-5.8% depending on observable from H1 Sys.7 (`research-sidis-systematics-literature.md` lines 158-168, 556-559; critic lines 22-23) | coherent overall normalization | both | `h1-mc-expert` | No | Authoritative target method is H1 factor-2 scaling; current execution blocked by missing diffractive MC per critic lines 79 and 114 |
| 5 | SpaCal energy scale plus angular variation | Shift SpaCal energy by plus or minus 1% and angle by plus or minus 1 mrad, then combine | 1-5% on multiplicity distributions and 2% on moments from H1 Sys.3 (`research-sidis-systematics-literature.md` lines 95-106, 566-568; critic lines 23-24) | coherent | multiplicity | `h1-data-validator` and `h1-production` | No | Cancels in A_h to first order per critic matrix lines 127-128 |
| 6 | Hadronic energy scale | Shift hadronic final-state energies by plus or minus 2% | 1-3% on multiplicity distributions and 2% on moments from H1 Sys.4 (`research-sidis-systematics-literature.md` lines 110-120, 569-570; critic lines 24-25) | coherent | multiplicity | `h1-data-validator` and `h1-production` | No | Treat as first-order cancelling in A_h but still verify numerically |
| 7 | QED radiative correction model dependence | Compare C_rad derived from DJANGOH and RAPGAP radiative or non-radiative branches | 1-2% on multiplicity distributions and up to 1% on moments from H1 Sys.1 (`research-sidis-systematics-literature.md` lines 59-75, 571-573; critic lines 25-26) | coherent | both | `h1-radiative-correction` | No | For A_h, derive and propagate per-charge C_rad; do not classify as fully cancelling |
| 8 | Period-to-period variation | Split low-Q2 data by 2005, 2006, 2007 and take the largest deviation vs the combined result | No H1 low-Q2 charged-track number; HERMES precedent is never larger than 2% (`research-sidis-systematics-literature.md` lines 345-350, 577-579; critic line 26) | partially correlated | both | `h1-data-validator` and `h1-systematics` | Yes | Low-Q2 has all three years available through run cuts; use the HERMES-style largest-deviation rule as policy |
| 9 | Photoproduction background verification | Reproduce gamma-p estimate and verify residual remains negligible under charged-SIDIS cuts | Paper quotes negligible with total contribution below 0.5% and no assigned systematic number (`research-sidis-systematics-literature.md` lines 186-194, 580-582; critic lines 27-28) | TBD - to be evaluated locally | both | `h1-mc-expert` | Placeholder | Do not inherit zero; if still negligible, record explicit verification provenance |
| 10 | Trigger inefficiency verification | Re-verify trigger inefficiency on independently triggered data for the charged-SIDIS selection | Paper states negligible with no numerical systematic entry (`research-sidis-systematics-literature.md` lines 198-203, 583-585; critic lines 28-29) | correlated if verified common; otherwise TBD - to be evaluated locally | both | `h1-data-validator` | Placeholder | Conceptually cancelling in A_h, but verification is still required |
| 11 | Wide-bin extrapolation for integrated moments only | Compare wide-bin interpolation or extrapolation with generator-level reference; assign the larger difference | 1.0% on mean, variance, and entropy only from H1 Sys.8 (`research-sidis-systematics-literature.md` lines 172-182, 574-576; critic lines 29-29) | coherent | multiplicity | `h1-cross-section` | Placeholder | Exclude from differential SIDIS bins by policy; include only if integrated moment-style observables are reported |

## High-Q2 Ordered Catalog

| # | Source name | Variation method | Typical magnitude | Bin-to-bin correlation class | Observable affected | Owning specialist | Executable today | Notes |
|---|---|---|---|---|---|---|---|---|
| 1 | MC model dependence / acceptance-response model swap | Swap nominal high-Q2 acceptance-response input to the alternate generator family | ZEUS high-Q2 precedent about -4%; expected 2-5% for charged-SIDIS planning (`research-sidis-systematics-literature.md` lines 296-298, 596-599; critic lines 32-33) | coherent | both | `h1-acceptance` | Yes | High-Q2 merged DJANGOH exists but has not yet been analyzed; do not copy low-Q2 H1 numbers directly as high-Q2 quoted results |
| 2 | Charge-dependent tracking / acceptance-border effects | Scan charged-track borders such as pT threshold and |eta| limit and quantify charge-sign residuals | ZEUS high-Q2 precedent is -3% to +6% for border effects plus plus or minus 3% overall tracking (`research-sidis-systematics-literature.md` lines 289-293, 599-603; critic lines 33-34) | partially correlated | both | `h1-data-validator` | Yes | This is the main non-cancelling detector source for high-Q2 A_h |
| 3 | Electron reconstruction / energy-angle scale in the LAr regime | Apply the authoritative LAr electron reconstruction variation procedure once defined | TBD - to be evaluated locally; framework says a few percent expected but no authoritative H1 number is established (`research-sidis-systematics-literature.md` lines 606-611; critic lines 34-35, 168-169) | coherent | multiplicity | `h1-data-validator` and `h1-production` | Placeholder | Low-Q2 SpaCal percentages must not be transferred numerically into this source |
| 4 | Hadronic energy scale / boost dependence | Shift hadronic energy scale and re-evaluate boost-sensitive observables | TBD - to be evaluated locally; framework expectation is a few percent (`research-sidis-systematics-literature.md` lines 606-611; critic lines 35-36) | coherent | both | `h1-data-validator` and `h1-production` | Placeholder | For A_h this cancels to first order only; still propagate via per-charge calculation |
| 5 | Radiative correction model dependence | Use DJANGOH14 gen-only v1 as provisional baseline; keep RAPGAP31 quarantined until provenance/lumi/sample-definition is resolved | DJANGOH14 integrated C_rad is about 1.055 per charge; sparse high-z/high-Q²/edge-pT bins have large MC-stat uncertainties; RAPGAP31 is anomalous and blocked | coherent plus sparse-bin policy TBD | both | `h1-radiative-correction` | Partially | Supersedes old high-Q² `C_rad=1` placeholder/nonRad-missing wording. Final publication requires sparse-bin policy, publication-grade DJ MC-lumi provenance, and RAPGAP decision/exclusion. |
| 6 | Diffractive contribution | Vary diffractive normalization, preferably with H1-style factor-2 method if diffractive MC exists | TBD - to be evaluated locally; literature expects a few percent in relevant corners, but no charged high-Q2 H1 number exists (`research-sidis-systematics-literature.md` lines 617-618; critic lines 37-38) | partially correlated | both | `h1-mc-expert` | No | Do not import ZEUS eta_max percentages as H1 final numbers |
| 7 | Period / beam-charge split | Split by period and beam-charge composition once e- recovery is complete | TBD - to be evaluated locally; currently blocked (`critic-sidis-systematics-distilled.md` lines 38-40, 84-85; framework lines 284-288, 619-622) | partially correlated; non-cancelling for A_h | both | `h1-data-validator` and `h1-production` | No | This source is defined in the catalog even though execution is blocked today |
| 8 | Photoproduction background verification | Verify residual gamma-p contamination remains negligible in high-Q2 charged SIDIS | TBD - to be evaluated locally; expected smaller than low-Q2 but no assigned number (`research-sidis-systematics-literature.md` lines 627-628; critic line 39) | TBD - to be evaluated locally | both | `h1-mc-expert` | Placeholder | Record explicit verification or non-negligible finding; do not assume zero |
| 9 | Trigger inefficiency verification | Verify trigger efficiency for the S67 and high-Q2 charged selection | TBD - to be evaluated locally; no number quoted (`research-sidis-systematics-literature.md` lines 623-624; critic line 40) | correlated if common; otherwise TBD - to be evaluated locally | both | `h1-data-validator` | Placeholder | Verify even though trigger is expected to cancel in A_h in principle |

## Added Beyond Literature

The distilled critic brief adds six policy items beyond the two upstream research documents.

1. Charge-dependent tracking-efficiency residual.
   - Policy: Keep this as an explicit A_h source separate from common tracking.
   - Source: `critic-sidis-systematics-distilled.md` line 92.
   - Owner: `h1-data-validator` and `h1-systematics`.

2. Vertex efficiency / vertex-Z cut dependence.
   - Policy: Treat this as a rerun-dependent soft systematic, not generic event selection noise.
   - Source: `critic-sidis-systematics-distilled.md` line 93.
   - Owner: `h1-data-validator` and `h1-production`.

3. Match-angle threshold variation and matching exclusivity variation.
   - Policy: Track both as real charged-track method handles.
   - Source: `critic-sidis-systematics-distilled.md` line 94.
   - Owner: `h1-acceptance` and `h1-production`.

4. Low-Q2 backport-fix dependence.
   - Policy: Treat this as a hard validity gate, not a quoted systematic source.
   - Source: `critic-sidis-systematics-distilled.md` line 95.
   - Owner: `h1-data-validator` and `h1-mc-expert`.

5. High-Q2 electron-finder / electron-reconstruction source.
   - Policy: Use an LAr-specific source and do not import SpaCal percentages numerically.
   - Source: `critic-sidis-systematics-distilled.md` line 96.
   - Owner: `h1-data-validator`.

6. Material-interaction asymmetry source for A_h.
   - Policy: Keep material interactions distinct from generic tracking efficiency in A_h bookkeeping.
   - Source: `critic-sidis-systematics-distilled.md` line 97.
   - Owner: `h1-asymmetry` and `h1-systematics`.
# Charge-Asymmetry Cancellation Matrix

When propagating systematics to A_h, evaluate per-charge first, then form the asymmetry; do not subtract systematics in quadrature from h+ and h- assuming full cancellation.

| Source | Cancels in A_h? | Reason |
|---|---|---|
| Luminosity | Yes | Common normalization cancels in ratio; keep out of A_h error total. |
| Trigger efficiency | Yes in principle / verify | Trigger not charge-selective at trigger layer; period-dependent composition must be checked. |
| Electron energy / angle scale | Yes | Same event-level kinematic migration for h+ and h-. |
| Hadronic energy scale | Yes to first order | Same event-level boost / bin assignment acts on both charges. |
| Radiative correction | Partial, not full | Must derive per-charge C_rad; do not classify as cancelling. |
| MC model dependence | Partial | Shared acceptance effects cancel only partly; charge-dependent response remains. |
| Common tracking efficiency | Partial large cancellation | Common part cancels, charge-sign residual does not. Final reference must split these. |
| Charge-dependent tracking efficiency | No | Explicitly non-cancelling. |
| Material interactions | No | Charge-sign-dependent hadron interaction with detector material. |
| V0 contamination: K0s -> pi+pi- | Mostly yes | Charge-symmetric contamination. |
| V0 contamination: Lambda / anti-Lambda | No | Positive/negative daughter asymmetry survives. |
| Diffractive contribution | Partial | Some charge-symmetric rho0-like content cancels; proton-remnant asymmetry does not. |
| Period / beam-charge composition | No | Different period composition can bias h+ and h- differently. |
| Wide-bin extrapolation | n/a | Not an A_h source for differential SIDIS bins. |

# Regime-Specific Overrides

## Low-Q2-only

- Use the SpaCal electron-scale source as the authoritative electron-side detector source for low-Q2. Source: `critic-sidis-systematics-distilled.md` lines 146-149.
- Enforce the low-Q2 backport-fix gate before any systematic evaluation. Source: `critic-sidis-systematics-distilled.md` lines 148-149.
- Use the 2005, 2006, and 2007 period split for low-Q2 period systematic studies. Source: `critic-sidis-systematics-distilled.md` lines 149-149.
- Treat low-Q2 nominal execution as bin-by-bin only until the backported charged response machinery is verified; unfolding remains a future cross-check, not the executable nominal. Source: `critic-sidis-systematics-distilled.md` lines 77-77.

## High-Q2-only

- Replace the SpaCal-specific electron source with an LAr electron-reconstruction source in high-Q2. Source: `critic-sidis-systematics-distilled.md` lines 151-154.
- Treat beam-charge or period systematics in high-Q2 as blocked until e- recovery is complete. Source: `critic-sidis-systematics-distilled.md` lines 153-154.
- Record H1CalcWeight status as relevant for absolute high-Q2 cross sections, but do not inject luminosity into multiplicity or A_h totals. Source: `critic-sidis-systematics-distilled.md` lines 154-154 and discrepancy resolution lines 82-83.

## Prohibited cross-transfer

- Do not transfer the multiplicity-paper SpaCal 1-5% number directly to high-Q2 LAr. Source: `critic-sidis-systematics-distilled.md` lines 156-157.
- Do not transfer high-Q2 ZEUS acceptance-border percentages into low-Q2 as H1 numbers. Source: `critic-sidis-systematics-distilled.md` lines 157-158.
- Do not import low-Q2 wide-bin extrapolation into the high-Q2 differential SIDIS pipeline unless integrated moments are reported. Source: `critic-sidis-systematics-distilled.md` lines 158-159.
- Do not import luminosity uncertainty into multiplicity or A_h totals in either regime. Source: `critic-sidis-systematics-distilled.md` lines 159-160.

# Minimal Viable Systematics Pass (MVP)

The MVP is the minimum preliminary charged-SIDIS systematics pass that may be shown internally once the hard validity gates for the relevant regime are satisfied. Apply the luminosity-band correction as follows: luminosity is not part of the MVP for multiplicities or A_h, so the 2.5% band must be excluded from the MVP error budget even though earlier framework drafts had included it. Source: critic brief lines 55-69 and discrepancy resolution lines 82-83. Label high-Q² z005 as `C_rad provisional`, not `missing C_rad`: DJANGOH14 gen-only v1 exists, while sparse-bin treatment, publication-grade MC-lumi provenance, and RAPGAP disposition remain open. The `missing C_rad` label below is now low-Q²-only or applies only to historical/pre-z005 high-Q² artifacts.

1. Run the validation gate.
   - Action: Confirm `ChargedTot` and `ChargedTotGen` exist, have expected branches, and cover the same run range; for low-Q2, stop if the backport or reprocessed MC gate fails.
   - Estimated wallclock: about 30 min.
   - Disk cost: zero new bulk output.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

2. Run the nominal acceptance, multiplicity, and A_h chain.
   - Action: Use nominal cuts and nominal merged files to emit the nominal acceptance, multiplicity, and A_h JSON products.
   - Estimated wallclock: about 1-2 h.
   - Disk cost: JSON outputs only; effectively negligible.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

3. Run the RAPGAP <-> DJANGOH model swap.
   - Action: Recompute the affected acceptance and downstream multiplicity and A_h products with the alternate generator sample.
   - Estimated wallclock: about 1-2 h.
   - Disk cost: zero if reused merged files; JSON outputs only.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

4. Run the DCA cut scan.
   - Action: Evaluate at least the 1.5, 2.0, and 2.5 cm DCA settings and keep the larger absolute deviation from nominal per bin.
   - Estimated wallclock: about 1-3 h.
   - Disk cost: zero if script rerun only.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

5. Run the pT threshold scan.
   - Action: Evaluate 0.10, 0.15, and 0.20 GeV charged-track thresholds.
   - Estimated wallclock: about 1-3 h.
   - Disk cost: zero if script rerun only.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

6. Run the |eta| scan.
   - Action: Evaluate |eta| < 1.2, 1.3, and 1.4 where operationally meaningful for the regime.
   - Estimated wallclock: about 1-3 h.
   - Disk cost: zero if script rerun only.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

7. Run the NHitDedx scan.
   - Action: Evaluate NHitDedx >= 8, 10, and 12.
   - Estimated wallclock: about 1-3 h.
   - Disk cost: zero if script rerun only.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

8. Run the period split where data exist.
   - Action: For low-Q2, compare 2005, 2006, and 2007 against the combined result using the HERMES-style largest-deviation rule; for high-Q2, do not execute this source until e- recovery is complete.
   - Estimated wallclock: about 1-2 h per split subset.
   - Disk cost: zero if existing merged inputs and run cuts are used.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

9. Emit explicit placeholder entries for the missing or provisional non-MVP sources.
   - Action: For low-Q², record placeholders for C_rad, diffractive, trigger, photoproduction, and electron-scale if not executed. For high-Q² z005, record `C_rad provisional` with DJANGOH14 gen-only v1 and open sparse-bin/MC-lumi/RAPGAP blockers rather than `missing C_rad`; the required plot label remains regime-specific and must mention C_rad, diffractive, trigger, electron-scale, and unfolding explicitly.
   - Estimated wallclock: about 10-20 min.
   - Disk cost: negligible.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

10. Exclude the luminosity band from multiplicity and A_h MVP totals.
   - Action: Do not add a flat 2.5% luminosity band to any MVP multiplicity or A_h error bar; document it only if an absolute cross-section branch is ever reported separately.
   - Estimated wallclock: negligible.
   - Disk cost: none.
   - Preliminary plot label requirement: low-Q² `MVP, missing C_rad / diffractive / trigger / electron-scale / unfolding`; high-Q² z005 `MVP, C_rad provisional / diffractive / trigger / electron-scale / unfolding`.

Operational estimate for the full MVP after validation: about one working day of interactive reruns with essentially zero new disk beyond JSON and plot outputs. Source: framework `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md` lines 659-663, corrected by critic discrepancy resolution at lines 82-83.

# Variation Execution Cookbook

This section is the operational core of the reference. For every variation type below, the SIDIS supervisor should call the named specialist, request the concrete handle, then re-run only the affected downstream JSON stages.

| Variation type | Handle name | File path or function | What changes | Downstream JSONs to re-emit | Cost class |
|---|---|---|---|---|---|
| RAPGAP <-> DJANGOH model swap | `mc_model_swap` | Low-Q2 merged files noted in framework lines 253-257; high-Q2 merged files at lines 267-272; charged scripts consume the merged ROOT file | Replace nominal MC acceptance-response input with alternate generator sample | `acceptance_<regime>_<source>.json`, `multiplicity_<regime>_<source>.json`, `asymmetry_<regime>_<source>.json`, then aggregate into `systematics_charged_<regime>.json` | script-rerun (cheap-medium) |
| DCA scan | `dca_scan` | `/Users/hklest/Desktop/opencode/H1/scripts/analyze_charged_xsec_corrected.py` BASE_CUT at framework lines 188-195; proton-thread CLI precedent in `analyze_cross_sections.py` lines 172-180 of framework excerpt | Tighten or loosen `abs(DcaPrime/cosh(Eta))` around nominal 2.0 cm | acceptance, multiplicity, asymmetry, systematics | script-rerun |
| pT scan | `pt_cut_scan` | `analyze_charged_xsec_corrected.py` BASE_CUT and `/Users/hklest/Desktop/opencode/H1/proton/analyze_sidis_charged.py` BASE_CUTS at framework lines 205-208 | Tighten or loosen charged-track pT threshold around nominal 0.15 GeV | acceptance, multiplicity, asymmetry, systematics | script-rerun |
| |eta| scan | `eta_cut_scan` | Same charged analysis script cut constants as above | Tighten or loosen charged-track |eta| cut around nominal 1.3 | acceptance, multiplicity, asymmetry, systematics | script-rerun |
| NHitDedx scan | `nhitdedx_scan` | `analyze_charged_xsec_corrected.py` BASE_CUT at framework lines 188-195 | Tighten or loosen NHitDedx threshold around nominal 10 | acceptance, multiplicity, asymmetry, systematics | script-rerun |
| RadLength scan | `radlength_scan` | `analyze_charged_xsec_corrected.py` BASE_CUT at framework lines 188-195 | Tighten or loosen RadLength threshold around nominal 10 cm | acceptance, multiplicity, asymmetry, systematics | script-rerun |
| Period split | `period_split` | Low-Q2 run-cut handles in `analyze_charged_xsec_corrected.py` lines 279-283 of framework | Restrict data and corresponding comparisons to year or beam-period subsets | acceptance if subset-specific, multiplicity, asymmetry, systematics | script-rerun |
| Match-angle scan | `match_angle_scan` | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/source_cache/PhysicsConstants_lowQ2.h` with `PROTON_MATCH_ANGLE = 5.0` at framework lines 160-165 and summary table line 382 | Change the angular reco-to-gen matching threshold, requiring rebuilt trees | validation, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| Matching exclusivity | `matching_exclusivity_toggle` | Builder-side change described in low-Q2 fixes doc Section 3 and framework lines 337-346, 381 | Toggle exclusive one-to-one reco-to-gen matching on or off | validation, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| Fiducial charged boundary | `fiducial_charged_scan` | `inFiducialCharged` in low-Q2 and high-Q2 builders at framework lines 215-227 | Tighten or loosen gen-level charged fiducial boundary used in denominator and matched fiducial tag | validation, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| Vertex-Z variation | `vtxz_scan` | evls-stage steering described in framework lines 236-247 | Tighten or loosen event vertex-Z window | validation, normalization if subset semantics change, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| y-window variation | `y_window_scan` | evls-stage steering in framework lines 239-247 | Tighten or loosen DIS y selection | validation, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| Q2-window variation | `q2_window_scan` | evls-stage steering in framework lines 239-247 | Tighten or loosen DIS Q2 selection at event-building stage | validation, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| Electron-scale variation | `electron_scale_shift` | evls-stage rerun requirement in framework lines 449-449 and low/high detector split in lines 576-577 | Shift SpaCal low-Q2 or LAr high-Q2 electron energy-angle reconstruction | validation, acceptance, possibly C_rad if coupled, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| Hadronic-scale variation | `hadronic_scale_shift` | evls-stage or HFS-scale rerun logic described in framework lines 603-606 and literature Sys.4 for low-Q2 | Shift hadronic final-state energy scale and the associated boost or kinematic reconstruction | validation, acceptance, multiplicity, asymmetry, systematics | full-reproduction (Condor-expensive) |
| C_rad model swap | `crad_model_swap` | `h1-radiative-correction` nominal and alternate radiative factors; framework lines 448-449 and critic lines 25-26, 36-37 | Replace nominal radiative-correction model with the alternate model or branch | `crad_<regime>_<source>.json`, then multiplicity, asymmetry, systematics | script-rerun when samples exist; currently blocked where nonRad absent |
| weight1*weight2 on or off | `weight_expr_toggle` | Charged scripts use `weight_expr='weight1*weight2'` at framework lines 305-312 | Replace nominal MC weight expression with unit weight for stress testing or normalization studies | acceptance, multiplicity, systematics | reweight-only |
| Reco pTh mass hypothesis | `reco_pth_mass_variation` | Planned offline Python reweight requiring Px, Py, Pz, GenPDG per framework line 450 | Recompute reco pTh under pion-default versus species-dependent mass assumptions | acceptance, multiplicity, asymmetry, systematics | reweight-only |
| Binning scheme variation | `bin_scheme_variation` | Charged bin edges in `analyze_charged_xsec_corrected.py` lines 349-353 of framework | Tighten or coarsen the histogram binning | acceptance, multiplicity, asymmetry, systematics | script-rerun |
| Bin-by-bin vs unfold | `method_swap_unfolding` | `h1-unfolder` methods and gating described in framework lines 491-519 | Replace bin-by-bin correction with unfolding, or vice versa, where purity or stability studies justify it | acceptance or response, multiplicity, asymmetry, systematics | script-rerun |
| Unfolding regularization scan | `unfolding_reg_scan` | D'Agostini iteration or SVD tau scan in framework lines 513-519 | Shift unfolding regularization strength around the nominal choice | acceptance or unfolded outputs, multiplicity, asymmetry, systematics | script-rerun |
| Diffractive factor-2 normalization | `diffractive_x2` | Diffractive MC normalization source defined by H1 multiplicity precedent and framework gap lines 713-716 | Scale diffractive MC contribution by a factor of 2 | acceptance if embedded there, multiplicity, asymmetry, systematics | script-rerun when diffractive MC exists; currently blocked |
| Trigger verification | `trigger_verification` | External trigger study path; framework lines 451, 607-612, 709-712 | Verify that trigger efficiency remains negligible or quantify a source-specific correction | multiplicity, asymmetry, systematics | placeholder or external study |
| Photoproduction verification | `photoproduction_verification` | External or dedicated MC estimate path from literature lines 186-194 and framework gap lines 713-716 | Verify residual photoproduction background in charged-SIDIS cuts | multiplicity, asymmetry, systematics | placeholder or external study |

Execution rules:

1. Request the smallest viable re-execution cone for each variation.
2. Re-emit every downstream JSON touched by the variation.
3. Mark blocked sources as `Placeholder` or `No` until inputs exist.
4. For low-Q2, do not run builder or acceptance variations until section 2 passes.
# Combination and Reporting Rules

1. Compute per-source deltas per bin as the absolute difference between the varied and nominal result.
   - Rule: `delta_source(bin) = |sigma_var(bin) - sigma_nom(bin)|`.
   - Source: existing `h1-systematics` method statement and framework JSON schema (`/Users/hklest/Desktop/opencode/H1/agents/h1-systematics.md` lines 43-45; framework lines 465-472).

2. Symmetrize asymmetric variations by keeping the larger absolute deviation.
   - Rule: If up and down shifts are both evaluated, store the larger of the two `|delta|` values as the source contribution in that bin.
   - Policy: This follows the current systematics-agent rule and should be applied uniformly to charged SIDIS.

3. Sum independent per-source contributions in quadrature.
   - Rule: `sigma_syst_total(bin) = sqrt(sum_sources delta_source(bin)^2)`.
   - Source: H1 and ZEUS systematic-combination practice summarized in the literature and critic brief (`research-sidis-systematics-literature.md` lines 73-74, 91, 106, 120, 138, 154, 168, 182, 308; critic line 73).

4. Report correlated sources separately when they conceptually cancel or are not part of the observable.
   - Rule: Keep luminosity out of multiplicity and A_h totals. If an absolute cross-section branch is ever reported, list luminosity as a separate correlated normalization uncertainty rather than mixing it into the charged-SIDIS multiplicity total.
   - Rule: Keep trigger verification separate until it is explicitly quantified, even if it is expected to cancel in A_h.

5. Propagate A_h sources by charge first, then asymmetry.
   - Rule: For every source, compute shifted h+ and shifted h- multiplicities separately, then form the varied A_h from those shifted charge-separated quantities.
   - Prohibition: Do not subtract h+ and h- systematic bands in quadrature assuming full cancellation.
   - Source: critic brief matrix and explicit guidance at section E, plus framework charge-path notes (`critic-sidis-systematics-distilled.md` lines 123-140; framework lines 521-559).

6. Record cancellation class per source.
   - Rule: Assign each source one of four values in the output JSON: `cancels`, `partial`, `does_not_cancel`, or `n/a`.
   - Policy mapping: Use the charge-asymmetry matrix in section 5 for the authoritative classification.

7. Output one machine-readable JSON per regime.
   - File name: `systematics_charged_<regime>.json`
   - Regime values: `lowQ2` or `highQ2`

```json
{
  "step": "systematics",
  "channel": "charged_lowQ2",
  "regime": "lowQ2",
  "status": "PASS",
  "input_files": [
    "validation_prereqs_lowQ2.json",
    "normalization_lowQ2.json",
    "acceptance_lowQ2_nominal.json",
    "crad_lowQ2_nominal.json",
    "multiplicity_lowQ2_nominal.json",
    "asymmetry_lowQ2_nominal.json"
  ],
  "output_file": "systematics_charged_lowQ2.json",
  "observable_manifest": [
    "mult_hplus_dz",
    "mult_hminus_dz",
    "A_h_z"
  ],
  "bins": {
    "A_h_z": [
      {"low": 0.2, "high": 0.3, "center": 0.25}
    ]
  },
  "nominal": {
    "hplus": {
      "mult_dz": [0.0]
    },
    "hminus": {
      "mult_dz": [0.0]
    },
    "asymmetry": {
      "A_h_z": [0.0]
    }
  },
  "sources": {
    "mc_model_swap": {
      "owner": "h1-acceptance",
      "observable_scope": "both",
      "correlation_class": "coherent",
      "cancellation_class": "partial",
      "variation_inputs": {
        "acceptance": "acceptance_lowQ2_mc_model_swap.json",
        "multiplicity": "multiplicity_lowQ2_mc_model_swap.json",
        "asymmetry": "asymmetry_lowQ2_mc_model_swap.json"
      },
      "per_charge": {
        "hplus": {
          "mult_dz": {
            "nominal": [0.0],
            "varied": [0.0],
            "delta": [0.0]
          }
        },
        "hminus": {
          "mult_dz": {
            "nominal": [0.0],
            "varied": [0.0],
            "delta": [0.0]
          }
        }
      },
      "derived_asymmetry": {
        "A_h_z": {
          "nominal": [0.0],
          "varied": [0.0],
          "delta": [0.0]
        }
      },
      "dominant_bins": []
    }
  },
  "errors": {
    "hplus": {
      "stat": {
        "mult_dz": [0.0]
      },
      "syst_total_quadrature": {
        "mult_dz": [0.0]
      }
    },
    "hminus": {
      "stat": {
        "mult_dz": [0.0]
      },
      "syst_total_quadrature": {
        "mult_dz": [0.0]
      }
    },
    "asymmetry": {
      "stat": {
        "A_h_z": [0.0]
      },
      "syst_total_quadrature": {
        "A_h_z": [0.0]
      },
      "syst_cancel": {
        "A_h_z": [0.0]
      },
      "syst_charge_dep": {
        "A_h_z": [0.0]
      }
    }
  },
  "dominant_source": {
    "hplus": {
      "mult_dz": ["mc_model_swap"]
    },
    "hminus": {
      "mult_dz": ["mc_model_swap"]
    },
    "asymmetry": {
      "A_h_z": ["mc_model_swap"]
    }
  },
  "validation": {
    "completeness": "PASS",
    "backport_fixes_applied": true,
    "luminosity_excluded_from_multiplicity": true,
    "luminosity_excluded_from_asymmetry": true,
    "h1calcweight_integrated": false,
    "notes": "Use placeholder entries rather than zeros for blocked sources."
  }
}
```

Allowed values for `cancellation_class` are exactly: `cancels`, `partial`, `does_not_cancel`, `n/a`.

# Open Questions (track in supervisor sessions)

1. Are usable charged-tree nonRad MC samples available, or must they be newly produced?
   - Owner: `h1-radiative-correction` with input from `h1-mc-expert`
   - Source: `critic-sidis-systematics-distilled.md` lines 166-166.

2. Does the merged low-Q2 DJANGOH14 file suffer the same incomplete `ChargedTotGen` problem as RAPGAP31?
   - Owner: `h1-data-validator` delegating tree inspection to `h1-root-analyst`
   - Source: `critic-sidis-systematics-distilled.md` lines 167-167 and `/Users/hklest/Desktop/opencode/H1/proton/docs/research-acceptance-debug.md` lines 351-353.

3. What is the authoritative high-Q2 electron-reconstruction variation procedure and numerical scale in the LAr regime?
   - Owner: human analysis lead plus `h1-data-validator`
   - Source: `critic-sidis-systematics-distilled.md` lines 168-168.

4. How will diffractive MC be produced or approximated for charged tracks in practice?
   - Owner: `h1-mc-expert`
   - Source: `critic-sidis-systematics-distilled.md` lines 169-169.

5. Should match-angle and exclusivity be promoted into the publication-grade systematic set or kept as acceptance-development cross-checks only?
   - Owner: human analysis lead with input from `h1-acceptance`
   - Source: `critic-sidis-systematics-distilled.md` lines 170-170.

6. What exact JSON schema and driver implementation will own orchestration?
   - Owner: `h1-systematics` and `h1-production`
   - Source: `critic-sidis-systematics-distilled.md` lines 171-171.

7. For high-Q2 A_h, is any provisional statement allowed before e- recovery is completed?
   - Owner: human analysis lead with input from `h1-asymmetry` and `h1-data-validator`
   - Source: `critic-sidis-systematics-distilled.md` lines 172-172.

# Pointers and Provenance

## Authoritative Pointers

| Topic | Primary pointer |
|---|---|
| H1 published low-Q2 systematic inventory | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md` Section `## 1. H1 Charged-Particle Multiplicity Paper (DESY 20-176): Systematic Inventory` |
| Exact H1 paper numbers | `/Users/hklest/Desktop/opencode/H1/proton/ChargedParticlePaper_extracted.md` lines 395-448, as cited by `critic-sidis-systematics-distilled.md` lines 186-187 |
| A_h cancellation logic | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md` Section `## 3.5`; `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md` Section `### 3.5`; distilled matrix in `critic-sidis-systematics-distilled.md` lines 121-140 |
| Ordered low-Q2 and high-Q2 source lists | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md` Section `## 4. Synthesis: Recommended Ordered Systematic Sources` |
| Current code and agent reality | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md` Sections `## 1` and `## 2` |
| Proposed pipeline | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md` Section `## 3` and `critic-sidis-systematics-distilled.md` lines 42-53 |
| MVP recipe | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md` Section `## 5`, corrected by `critic-sidis-systematics-distilled.md` lines 55-69 and 82-83 |
| Low-Q2 hard prerequisite fixes | `/Users/hklest/Desktop/opencode/H1/proton_HiQ/docs/charged-particle-fixes-for-lowQ2.md` |
| Known low-Q2 acceptance failure | `/Users/hklest/Desktop/opencode/H1/proton/docs/research-acceptance-debug.md` |
| This reference playbook | `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/reference-sidis-systematics.md` |

## Provenance

This reference synthesizes three upstream documents in the precedence order mandated by the request.

1. `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/critic-sidis-systematics-distilled.md`
   - Produced on 2026-04-28 by a `research-critic` review pass.
   - Role in this reference: source of truth when it conflicts with the upstream research documents.

2. `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-literature.md`
   - Produced on 2026-04-28 by a physics literature researcher.
   - Role in this reference: source for published-method precedence, quantitative magnitudes, and paper-level citations.

3. `/Users/hklest/Desktop/opencode/H1/sidis_lowQ2/research-sidis-systematics-framework.md`
   - Produced on 2026-04-28 as the methods and framework audit.
   - Role in this reference: source for executable handles, current repository reality, pipeline wiring, and operational costs.

Supervisor note: future SIDIS supervisor prompts should load this file at session start, but this does not modify `/Users/hklest/Desktop/opencode/H1/agents/h1-systematics.md`.

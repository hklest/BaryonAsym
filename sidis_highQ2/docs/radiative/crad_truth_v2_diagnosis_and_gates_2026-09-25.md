# Truth-v2 gen-only C_rad diagnosis and remaining gates — 2026-09-25

## Scope and decision

Read-only diagnosis of the candidate-only truth-v2 run3 table.  No production was submitted, central product modified, or correction applied.  The convention used throughout is

`C_rad = sigma_nonrad / sigma_rad`.

**Verdict: truth-v2 run3 central values are defective as a release candidate and must not be interpreted as a physical QED correction.**  The table is statistically well populated but fails the central-value sanity check: h+ integrated C_rad is 0.6770210 and h- is 0.8509564; 176/490 bins lie outside [0.5,1.2], with range 0.08054--1.55048 and median 1.13100.  This is incompatible with a single common luminosity rescale: restoring 1.05 would require a 1.5509 h+ numerator-sigma increase but only 1.2339 for h-, or equivalently different common-denominator reductions (0.6448 versus 0.8104).  A common RAD or NONRAD exposure error therefore cannot be the complete explanation.

The most probable root cause is an unvalidated normalization/input pairing of the pooled OLD+NEW NONRAD sample, compounded by a materially changed truth definition.  The charge-dependent integrated shift and extreme shape are evidence against an ordinary inclusive exposure error and against a credible pure-QED effect.  The immediate technical defect to resolve is the event-level NONRAD exposure/provenance and OLD/NEW compatibility, not MC statistics.

## What run3 actually applies

The run3 producer applies one common `consume()` selection to RAD, OLD NONRAD, and NEW NONRAD.  It requires finite inputs; `150<Q2_es_gen<20000`; `0.1<Y_es_gen<0.7`; `GenEmpz>45` with no upper bound; `xFGen<0`; pi/K/p; `IsGtr!=0`; `Mother2==-1`; `IsV0DaughterGen==0`; and the frozen 245 bins using gamma*p-CM photon-axis `pT_h`.  Thus there is no sample-asymmetric cut in the run3 Python selection loop.

This is not the v1 selection: v1 used `35<GenEmpz<70`, `Mother2==-1`, and `ParticleSource!=4` where available, with its historical kinematic conventions.  Truth-v2 replaces the upper-bounded EMPZ selection and source veto with the source-pool gates plus explicit V0-daughter veto, and changes the pT_h convention.  These changes can alter bin shapes and acceptance into the frozen bins, particularly near EMPZ and at z/pT_h edges, but the present artifacts contain no controlled same-input, one-cut-at-a-time comparison.  They therefore cannot quantify a physical selection contribution or demonstrate that it explains h+ 0.677, h- 0.851, or C_rad=0.0805.

The observed trend is too large to accept as QED radiation without that control: all-charge medians rise from 0.763 in z=0--0.05 to 1.271 in z=0.7--1.0, while falling from 1.243 for pT_h=0--0.5 to 1.046 for pT_h=4--8.  h+ low-z/high-pT_h bins produce the 0.08--0.15 failures.  Some z and transverse-momentum dependence is possible for radiative migration, but a factor about 1.6 in median z trend, values below 0.1, and a large charge split are a defect signal, not a validated radiative prediction.

## Normalization sensitivity

The companion JSON records reproducible sensitivity arithmetic.  To move h+ from 0.677021 to 1.05 requires 1.5509 times the NONRAD cross-section proxy (or 0.6448 times its exposure); h- needs 1.2339 (or 0.8104 exposure).  For 1.12 the corresponding numerator factors are 1.6543 and 1.3162.  No single scale gives both.

The run3 combined NONRAD exposures are e+ 14,637,492.6760 and e- 14,541,275.2122 nb^-1.  The 2026-09-21 topology-audited handoff values are 15,439,571.1325 and 15,311,385.4287 nb^-1, only 5.48% and 5.30% larger for the combined samples.  The apparent 1.47/1.45 discrepancy is confined to the NEW component: 2,519,454.5686/1,717,376.8213=1.4670 (e+) and 2,469,812.9872/1,699,702.7707=1.4531 (e-).  Substituting that component alone changes the combined exposure by only about 5%, and increases exposure, hence lowers C_rad; it neither restores the candidate nor has the required direction.  This excludes the advertised 1.47 factor as a solution, though it remains a serious provenance contradiction.

## RAD exposure provenance and conflicting audits

RAD run3 constants are exactly the later oomclumi audit totals: e+ 16,206,393.0 and e- 13,534,017.0 nb^-1.  The authoritative audit is `rad_oomclumi_exact_v2_20260924.json`, timestamp 2026-09-24 16:14:37 +0200.  It is PASS, covers 1373/1373 retained chunks with no missing/extra/failures, records component-level SIGMT consistency and sumW identities, and reproduces both beam totals (e+ residual -1.86e-09 nb^-1; e- zero).

The contradictory TSV is older (2026-09-24 04:18:19 +0200) and explicitly reports absence of component-associated oomclumi/SIGMT evidence.  It predates the exact component audit and is superseded for RAD exposure.  RAD may consequently be reclassified as **traced/exact oomclumi-verified for the frozen retained manifest**, rather than provisional.  That closes RAD exposure tracing only; it does not trace NONRAD or validate C_rad.

## Gate register

### Closed

1. **RAD retained-manifest exposure traceability — CLOSED.**  Exact oomclumi PASS: 1373/1373 chunks, eight components, beam sums matching run3 constants.
2. **P-CRAD-SPARSE statistical-support blocker — CLOSED for truth-v2 occupancy.**  The fixed joined audit has zero zero-sumw bins in every one of eight beam-by-sample groups, no bin above the 0.20 relative-stat limited threshold, and 490/490 finite C_rad rows.  C_rad median relative statistical uncertainty is 0.4187%, maximum 5.74%; group medians are 0.4--1.2% and p95 is at most 7%.  A separate policy is still needed for any later central-value rejection, but no mask/merge/smoothing is justified by truth-v2 MC sparsity.
3. **Frozen file inventory and shared run3 selection implementation — CLOSED.**  All expected inputs are present: RAD 731 e+/642 e-, OLD 260/260, NEW 530/530; the producer routes all three through the same selection function.

### Open, ordered gates and executable next action

1. **G-CRAD-NONRAD-EXPOSURE: resolve the contradictory NEW NONRAD event exposure and trace OLD+NEW provenance.**  The run3 map labels all four NONRAD components `DIAGNOSTIC_GENDIS_EVENT_LEVEL`; NEW differs from the documented valid-campaign exposure by factors 1.4670/1.4531.  Next action: submit the full event-level comparison using `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/nonrad_old_new_v2_ratio_20260922/nonrad_old_new_v2_ratio.py` (not on login node); require its `summary.json` and `nonrad_old_new_v2_ratio_rows.csv` to be accompanied by a signed exposure reconciliation that identifies the exact GenDIS manifests, SIGMT source, generated-weight convention, and one approved exposure per component.

2. **G-CRAD-OLDNEW-COMPAT: validate bin-local OLD/NEW compatibility under the exact source-pool selection used by run3.**  Existing broad-shape QA is inconclusive and its comparison script omits `IsGtr`/`Mother2`, whereas run3 adds both.  Next action: run a revised, source-pool-matched version of `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/nonrad_old_new_v2_ratio_20260922/nonrad_old_new_v2_ratio.py` in Condor after gate 1 fixes constants; produce a 490-row mixed-beam compatibility sidecar and summary with per-beam ratios, pulls, zeros, manifests, and an explicit accept/reject rule.

3. **G-CRAD-SELECTION-CONTROL: quantify truth-v2 versus v1 selection effects and prove no interface asymmetry.**  The shared run3 loop proves code-level symmetry but not equivalence of underlying RAD/NONRAD ntuple population or the magnitude of changed EMPZ/source/pT definitions.  Next action: extend and submit `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v2_20260923/produce_crad_truth_v2.py` in a controlled Condor diagnostic that writes paired cutflow and 490-row C_rad tables for: nominal truth-v2, v1 EMPZ 35--70/source selection where branches permit, EMPZ-only replacement, source/V0-only replacement, and pT convention comparison.  Required artifact: selection-closure JSON/CSV with RAD and NONRAD efficiencies per beam and ratio-of-ratios; no promotion without it.

4. **G-CRAD-NORMALIZATION-REBUILD: independently rebuild the candidate once gates 1--3 define an approved exposure map and matching selection.**  Next action: use `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v2_20260923/produce_crad_truth_v2.py` with the approved pinned constants and create a new immutable candidate directory containing `crad_truth_v2_490rows.csv`, `summary.json`, exposure-map SHA256, delta_QED plot, range-count report, and independent reproduction check.  Acceptance requires both charges near the controlled reference behavior and no unexplained extreme tails; it is not an application action.

5. **G-CRAD-PHYSICS/RESPONSE: approve a coherent DJANGOH-C_rad versus RAPGAP-response treatment and generator systematic/disposition.**  Next action: execute the existing comparison workflow `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/rad_crad_vs_detector_mc_20260921/compare_rad_crad_vs_detector_mc.py` only after the repaired candidate exists; produce a review package documenting sample/interface limitations, generator comparison, uncertainty treatment, and explicit include/exclude decision for RAPGAP.

6. **G-CRAD-RELEASE: physics and release approval.**  Next action: after gates 1--5 pass, prepare a candidate-only application proposal through the final validated table producer in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v2_20260923/produce_crad_truth_v2.py`; required artifact is an approval-reviewed release sidecar stating `c_rad_applied` transition, 490-bin alignment, covariance/systematic treatment, and rollback provenance.  Central products remain untouched until explicit approval.

## Blocking statement

The central release remains raw and `c_rad_applied=false`.  Blocking issues are the unresolved NONRAD exposure conflict, lack of source-pool-matched OLD/NEW bin-local compatibility acceptance, absent controlled selection-variation closure, defective current central values, and unresolved DJANGOH-C_rad/RAPGAP-response physics decision.  RAD luminosity is no longer a blocker for the frozen retained RAD manifest.

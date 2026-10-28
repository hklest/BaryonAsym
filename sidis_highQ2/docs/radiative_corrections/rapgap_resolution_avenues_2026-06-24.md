# RAPGAP31 gen-only C_rad quarantine: resolution avenues

Date: 2026-06-24
Scope: high-Q² inclusive charged-hadron SIDIS, gen-only `C_rad = sigma_nonrad / sigma_rad`, RAPGAP31 quarantine decision support.

## Starting point fixed by current evidence

The current anomaly is not adequately explained by the nominal lower-Q² sample labels alone once the analysis enforces `Q2 > 150 GeV^2` at gen level. The working issue is therefore a provenance / luminosity / sample-definition / period-matching problem unless and until contrary evidence is produced.

Current observed facts from the gen-only v1 study and run-status:

- Study root: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/`
- Current RAPGAP pair under quarantine:
  - RAD official PNFS OO samples `8947–8954`, labels `RAD.Q2GT60`
  - NONRAD Britzger DUST `norad_prod21/dst2all_rapgap`, labels `NORAD.Q2GT100`
- Current integrated RAPGAP result remains anomalous after gen-only isolation:
  - `C_rad ≈ 1.405` per charge
  - median `≈ 1.478`
  - `455/490` finite bins outside `[0.5, 1.2]`
- Weighted and unweighted RAPGAP ratios are both anomalous, so the effect is not a tiny per-event-weight pathology:
  - weighted `≈ 1.405`
  - unweighted `≈ 1.412`
- Raw weighted sum ratio is much closer to DJANGOH than the luminosity-normalized result, pointing directly at luminosity equivalence / sample pairing / period bookkeeping as the dominant unresolved axis.
- Current study metadata already encodes suspicious placeholders:
  - RAD manifest entries carry the same `lumi_mapping.total_nb_inv = 5293265` for all RAPGAP RAD chunks
  - NONRAD manifest entries carry the same `lumi_mapping.total_nb_inv = 3397124` for all RAPGAP NORAD chunks
  - both have empty `lumi_mapping.periods = {}`
- Current manifest summary is only two coarse buckets, not the four-period high-Q² target split:
  - RAPGAP RAD: `eplus06e7 = 691`, `eminus06e7 = 582`
  - RAPGAP NONRAD: `eplus06e7 = 264`, `eminus06e7 = 256`
- The current nonrad sample IDs are synthetic chunk IDs (`C00000xx`) rather than official MC catalog IDs, which is itself a provenance warning.

This note organizes the realistic resolution avenues.

---

## Decision map summary

### Recommended ordering

1. **Avenue 1 — Prove the current pair is valid with missing provenance/lumi/period evidence**
2. **Avenue 2 — Recompute the current pair with per-sample / per-period normalization if enough retained metadata exists**
3. **Avenue 3 — Search for a true official matched RAPGAP RAD/NONRAD pair and rerun**
4. **Avenue 7 — Build alternative bounds/cross-checks on model uncertainty if RAPGAP remains blocked**
5. **Avenue 5 — Use RAPGAP only as a shape diagnostic after external normalization, if explicitly justified**
6. **Avenue 6 — Exclude RAPGAP from final C_rad and document DJANGOH-only with quarantine**
7. **Avenue 4 — Generate or convert a missing counterpart sample**

Reason for this order: the first three avenues are the only plausible paths to a physics-use RAPGAP correction. If they fail, the analysis should pivot quickly to bounded systematic alternatives instead of spending long effort on speculative production.

---

## Avenue 1 — Prove the current pair is valid

### Goal

Demonstrate that the present RAPGAP RAD PNFS set and RAPGAP NONRAD Britzger DUST set are actually the same physics campaign in compatible normalization units, with correct period mapping and equivalent effective luminosity semantics.

### Required evidence

1. **MCDB / generator-log provenance for both sides**
   - official request IDs, production campaign tags, generator version, PDF set, beam charge / period split, radiative mode, generated phase space, filtering, and any reweighting
2. **Period mapping proof**
   - map RAD sample IDs `8947–8954` to concrete periods / beam charges / years
   - map Britzger NONRAD chunks to the same period structure, not just aggregate `eplus06e7` / `eminus06e7`
3. **Luminosity equivalence proof**
   - show that the RAD `5293265 nb^-1` and NONRAD `3397124 nb^-1` numbers are derived in the same convention and correspond to the same effective sum-of-weights semantics
4. **Physics-definition equivalence proof**
   - same final-state definition, same hadron content, same truth filtering, same event weights, same generator steering except radiative mode
5. **No hidden mixture proof**
   - verify the DUST NONRAD sample is not a stitched mixture of multiple campaigns with unequal period content or non-official postprocessing

### Exact artifacts/paths to inspect

- Study-local provenance/manifests:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/docs/input_manifest_genonly_v1.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/manifest_rapgap31_rad.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/manifest_rapgap31_nonrad.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/data/manifests/genonly_same_generator_manifest_all.json`
- Current results showing the anomaly:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/full_crad_v1_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/validation/validation_rc_genonly_v1.json`
- Official RAD source roots already enumerated in the manifest:
  - `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/rapgap31/8947/` through `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/rapgap31/8954/`
- Current NONRAD DUST source root:
  - `/data/dust/group/h1/britzger/norad_prod21/dst2all_rapgap/`
- Existing high-Q² lumi aggregate for official central high-Q² catalogs, useful as a cross-check baseline only:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`
- Evidence to request from MCDB / provenance work:
  - MC request pages, generator logs, steering cards, official luminosity summaries, and any saved-state metadata for 8947–8954 and the Britzger NONRAD source campaign

### Acceptance criteria

All of the following must be satisfied:

- One-to-one or defensibly many-to-one period mapping exists between RAD and NONRAD
- Same weight convention proven on both sides
- Same effective luminosity definition proven on both sides
- Same physics campaign / same intended kinematic regime except radiative mode proven
- No hidden period omission, duplication, or beam-sign imbalance remains
- Reconstructed integrated `C_rad` expectation from period-aware bookkeeping is no longer manifestly anomalous

### Risks

- The Britzger NONRAD sample may be a private derivative with incomplete surviving metadata
- Official MCDB records may not exist in a directly recoverable way for the DUST sample
- The present aggregate luminosities may be placeholders or mixed conventions rather than true publication-grade values

### Can this lead to physics-use RAPGAP?

**Yes**, but only if all provenance and lumi equivalence questions close cleanly.

### Priority

**Highest.** This is the cheapest path to salvaging RAPGAP without rerunning production.

---

## Avenue 2 — Recompute the current pair with per-sample / per-period normalized sums

### Goal

Retain the current RAD and NONRAD sample families, but recompute `C_rad` using matched per-sample or per-period pieces instead of one aggregate luminosity per side.

### Why this avenue is attractive

The present manifests already show that the anomaly is consistent with mismatched aggregate normalization. If retained partials can be grouped by true period / charge / sample family, the correction may move toward the expected range even without changing the underlying files.

### Required evidence

1. Retained per-file or per-sample metadata sufficient to assign each file to a period and beam sign
2. Effective luminosity per period / sample, not only one global total for all RAD and one for all NONRAD
3. Confirmation that partials are final enough to support a stable regrouping
4. If not retained, evidence that re-finalization from existing ntuples can reproduce those partial sums without rebuilding the ntuples themselves

### Exact artifacts/paths to inspect or produce

Inspect:
- Current ntuple outputs by generator/radmode:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/ntuples/RAPGAP31/rad/`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/ntuples/RAPGAP31/nonrad/`
- Finalization code and current lumi injection point:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming/full_crad_finalize.py`
- Study manifests carrying chunk labels and source paths:
  - `data/manifests/manifest_rapgap31_rad.json`
  - `data/manifests/manifest_rapgap31_nonrad.json`

Produce, if feasible:
- a per-period RAPGAP bookkeeping table such as
  - `output/tables/quarantine/rapgap31_period_lumi_audit.json`
- a recomputed per-period then combined RAPGAP `C_rad` table such as
  - `output/tables/quarantine/rapgap31_crad_periodmatched_recompute.json`
- a compact comparison note or CSV showing
  - global-aggregate result vs per-period recompute

### Acceptance criteria

- Recompute uses explicit period-resolved luminosities and year mapping
- Any unmatched period is excluded and reported, not silently folded in
- The recomputed integrated `C_rad` moves into a plausible corridor and binwise behavior becomes mostly smooth
- The result is reproducible from retained study artifacts without ambiguous manual intervention

### Risks

- Current manifests only expose coarse `eplus06e7` / `eminus06e7` labels and empty `periods` maps, which may be insufficient
- NONRAD chunk naming (`C00000xx`) may not encode true period identity
- If per-period effective luminosities cannot be reconstructed, this avenue collapses back into Avenue 1 or 3

### Can this lead to physics-use RAPGAP?

**Yes**, if period-matched bookkeeping becomes unambiguous and yields a stable, plausible correction.

### Priority

**Very high**, immediately after Avenue 1. It is the most direct technical repair path if provenance can be recovered enough to normalize properly.

---

## Avenue 3 — Find a true official matched RAPGAP RAD/NONRAD pair and rerun

### Goal

Abandon the present mixed official/private pair and instead identify an official RAPGAP RAD/NONRAD combination with common provenance and known luminosity bookkeeping.

### Required evidence

1. Existence of official RAPGAP NONRAD OO or DST/MODS/HAT samples matched to the RAD line
2. Verified period coverage across the four-period high-Q² target or a clearly documented subset
3. Official luminosity records or generator logs for both radiative states
4. Same charged-hadron truth semantics as the current gen-only study

### Exact artifacts/paths to inspect or produce

Inspect / request:
- MCDB or official H1 MC provenance records for RAPGAP31 NORAD high-Q² samples corresponding to the RAD 8947–8954 family
- Any official PNFS RAPGAP NORAD directories discovered via targeted MCDB evidence
- Existing central high-Q² official RAPGAP processed sample lineage, if any, to infer catalog ancestry

Potential new production/output locations if rerun becomes possible:
- under current study root, a clearly isolated branch such as
  - `output/ntuples/RAPGAP31_officialmatched/rad/`
  - `output/ntuples/RAPGAP31_officialmatched/nonrad/`
  - `output/tables/quarantine/rapgap31_officialmatched_crad/`

### Acceptance criteria

- Both RAD and NONRAD sides are official and provenance-complete
- Shared period coverage is explicit
- Luminosity derivation is official and symmetric in convention
- Rerun `C_rad` enters a physically plausible band and no longer looks like a normalization artifact

### Risks

- A true official matched RAPGAP NORAD counterpart may simply not exist in accessible storage
- Existing official NORAD may cover only a subset of periods or beam signs
- Recovery may require substantial MCDB digging before any physics result appears

### Can this lead to physics-use RAPGAP?

**Yes.** This is the cleanest salvage path if such a pair exists.

### Priority

**High.** It should be pursued in parallel conceptually with Avenue 1/2, but only after targeted provenance checks avoid blind searching.

---

## Avenue 4 — Generate or convert a missing counterpart sample

### Goal

If no suitable official counterpart exists, create a matched sample by new production or by a rigorously documented conversion of available generator-level material.

### Required evidence

1. Firm proof that no acceptable existing match is recoverable
2. Technical feasibility statement from MC experts / production support
3. Definition of exact target campaign:
   - periods
   - beam charges
   - generator version
   - PDF set
   - radiative mode toggle
   - weights and output format
4. Resource estimate and timeline

### Exact artifacts/paths to inspect or produce

Inspect:
- production runbooks and existing generator conversion machinery relevant to RAPGAP31
- current study builder compatibility with new source formats

Produce if approved:
- a dedicated production specification note under `sidis_highQ2/docs/radiative_corrections/`
- new source manifests under the study root
- new gen-only ntuples and final tables in a separate subcampaign namespace

### Acceptance criteria

- New sample is explicitly matched to the retained counterpart by campaign design
- MC lumi and period mapping are embedded from the start
- The resulting `C_rad` is stable, reproducible, and provenance-complete

### Risks

- Highest cost and slowest turnaround
- New production may be impractical for current analysis schedule
- Converted samples can inherit undocumented biases if not truly official or not generator-equivalent

### Can this lead to physics-use RAPGAP?

**Yes**, in principle, but only after a full production-quality validation cycle.

### Priority

**Low for immediate release planning, high only if RAPGAP is judged essential and all existing-sample routes fail.**

---

## Avenue 5 — Use RAPGAP only as a shape diagnostic after external normalization

### Goal

Treat RAPGAP as a qualitative or semi-quantitative shape cross-check rather than a standalone absolute `C_rad` provider.

### When this becomes relevant

If the sample pair cannot support absolute normalization, but the bin-to-bin variation after an external scale adjustment still carries useful model-shape information.

### Required evidence

1. Demonstration that RAPGAP bin-shape residuals are stable under reasonable normalization prescriptions
2. Explicit statement that the absolute normalization is not trusted
3. A controlled scaling prescription, e.g.
   - normalized to DJANGOH integrated `C_rad`
   - normalized in a central high-stat region
   - normalized per charge but not per bin
4. Evaluation that such usage does not masquerade as an independent absolute generator systematic

### Exact artifacts/paths to inspect or produce

Inspect:
- current RAPGAP bin tables in the gen-only v1 outputs
- DJANGOH baseline tables in the same study

Produce:
- shape-only ratio tables and plots such as
  - `RAPGAP_scaled_to_DJANGOH / DJANGOH`
  - central-region anchored residuals vs bin
- a compact justification note specifying that RAPGAP is excluded from absolute normalization use

### Acceptance criteria

- The scaling convention is explicit and reproducible
- Post-scaling shape differences are smooth enough to be interpretable
- The systematic model clearly labels RAPGAP as shape-only, not an independent absolute correction

### Risks

- Easy to over-interpret a renormalized problematic generator as stronger evidence than it deserves
- If the underlying sample mismatch is period-dependent, even shape differences may be contaminated

### Can this lead to physics-use RAPGAP?

**Not for nominal absolute C_rad.**
It can only support a **restricted shape-diagnostic role**.

### Is it acceptable for a generator-envelope systematic?

**Only in a limited, explicitly asymmetric, shape-only sense.**
It is **not acceptable** as a symmetric two-generator absolute `C_rad` envelope while the normalization/provenance problem is unresolved.

### Priority

**Medium after salvage attempts fail.** Useful as a fallback diagnostic, not as a primary rescue strategy.

---

## Avenue 6 — Exclude RAPGAP from final C_rad and document DJANGOH-only with RAPGAP quarantine

### Goal

Make an explicit analysis decision that DJANGOH14 gen-only v1 is the only usable `C_rad` basis for this release, while RAPGAP is documented as quarantined and not used in nominal or generator-envelope systematics.

### Required evidence

1. Clear record that RAPGAP remains provenance/lumi unresolved after targeted investigation
2. Justification that DJANGOH14 is internally stable enough for provisional/final use subject to sparse-bin policy and publication-grade lumi provenance
3. Documentation of what uncertainty is and is not covered without RAPGAP

### Exact artifacts/paths to inspect or produce

Inspect:
- current run-status and handoff entries already reflecting RAPGAP quarantine
- DJANGOH baseline tables and validation JSONs

Produce:
- a durable decision memo under `sidis_highQ2/docs/radiative_corrections/`
- if desired, a quarantine JSON or markdown summary with explicit no-go criteria and missing evidence list

### Acceptance criteria

- The exclusion is explicit, not implicit
- The final release text says RAPGAP is quarantined due to unresolved provenance/lumi/sample-definition mismatch
- No downstream code averages RAPGAP with DJANGOH or quotes a symmetric generator envelope from the current pair

### Risks

- Generator-model uncertainty on `C_rad` is then underconstrained unless replaced by another bound
- Reviewers may ask whether more could have been done to recover RAPGAP

### Can this lead to physics-use RAPGAP?

**No.** This is the formal no-go branch.

### Priority

**Medium-high as the fallback decision branch.** It should be prepared in parallel so release planning is not blocked indefinitely.

---

## Avenue 7 — Alternative cross-checks or internal consistency bounds without RAPGAP

### Goal

Bound the model uncertainty on `C_rad` even if RAPGAP never becomes usable as an absolute second generator.

### Possible alternatives

1. **DJANGOH internal consistency variations**
   - per-period stability of `C_rad`
   - beam-charge split stability
   - with/without optional truth-side filters that are physically motivated and already documented
   - sensitivity to sparse-bin treatment choices (mask / merge / smoothing)
2. **Normalization-robust RAPGAP diagnostics**
   - compare normalized shapes only after integrated anchoring, clearly labelled non-absolute
3. **Rad/nonrad internal checks within each generator**
   - compare event-level and hadron-level accepted fractions by period and charge
   - compare `selected_in_bins / event_pass` and `selected_in_bins / tree_entries` between rad and nonrad for anomalies
4. **Cross-check against central official high-Q² processed RAPGAP catalogs**
   - not as direct `C_rad`, but as a consistency check of year composition and luminosity scales versus known official central high-Q² RAPGAP samples (`7071`, `7094`, `7096`, `7097` in the central lumi JSON)
5. **Simple bounded nuisance treatment**
   - if no second generator survives, assign an asymmetric nuisance informed by DJANGOH period dependence and any shape-only RAPGAP residuals, rather than pretending to have a validated two-generator envelope

### Required evidence

- Quantified internal stability metrics from DJANGOH
- Explicit statement of what is being bounded: absolute normalization, bin-to-bin shape, or sparse-bin behavior
- Demonstration that the bound is conservative relative to observed stable variations

### Exact artifacts/paths to inspect or produce

Inspect:
- DJANGOH summary and validation JSONs already in the gen-only v1 study
- per-generator diagnostics in `full_crad_v1_summary.json` and `validation_rc_genonly_v1.json`

Produce:
- a compact model-uncertainty note or JSON tables for
  - per-period DJANGOH variation
  - sparse-bin policy spread
  - any shape-only anchored RAPGAP residuals if used

### Acceptance criteria

- The resulting uncertainty is honestly framed as a bound, not a validated generator envelope
- It is reproducible and based on observable internal stability metrics
- It is clearly separated from the quarantined RAPGAP absolute result

### Risks

- May be viewed as weaker than a clean two-generator comparison
- Requires careful language to avoid overclaiming model coverage

### Can this lead to physics-use RAPGAP?

**No**, except possibly as a shape-only diagnostic ingredient.

### Priority

**High fallback priority.** If RAPGAP salvage stalls, this avenue prevents total loss of model-uncertainty coverage.

---

## Go / no-go criteria for physics-use RAPGAP

### Go for physics-use RAPGAP only if

- RAD and NONRAD provenance are matched or defensibly equivalent
- period mapping is explicit and year-complete or deliberately subset-consistent
- luminosity derivation is symmetric and publication-grade
- recomputed or rerun RAPGAP `C_rad` lands in a plausible corridor and is smooth enough binwise
- remaining residual disagreement with DJANGOH looks like genuine generator physics, not bookkeeping

### No-go for physics-use RAPGAP if any of the following remain true

- aggregate luminosities cannot be traced to compatible definitions
- NONRAD sample provenance remains private/opaque relative to RAD official samples
- period mapping remains coarse or ambiguous
- corrected RAPGAP still sits near `1.4–1.5` broadly with most bins outside sanity range
- any needed rescue requires speculative undocumented manual normalization

---

## Recommended next concrete action

1. **Run a focused provenance/luminosity audit on the current pair before any new production.**
   - Ask MCDB / provenance support for official mappings of RAPGAP RAD `8947–8954` and any matched NORAD counterpart records.
   - Audit whether the Britzger NONRAD DUST files can be mapped to the same periods and luminosity semantics.
2. **In parallel, prepare a period-aware recomputation plan from retained manifests/ntuples.**
   - If per-period recovery is feasible, recompute before abandoning the pair.
3. **Set a decision gate:** if provenance and period-aware normalization are still unresolved after that targeted audit, do not keep RAPGAP on the critical path for the final `C_rad` release.

That makes Avenue 1 plus Avenue 2 the immediate action block, Avenue 3 the salvage fallback, and Avenue 6 plus Avenue 7 the release-protection fallback.

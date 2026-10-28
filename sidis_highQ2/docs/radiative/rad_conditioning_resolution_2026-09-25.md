# DJANGOH14 RAD conditioning resolution — 2026-09-25

## Verdict

**UNCONDITIONED with respect to the study reconstruction chain; upstream absolute generator-retention status remains unproven.**

The retained RAD builder uses only HAT/MODS navigation, contains no trigger/L4/L5/reconstructed-object/event-list predicate, and fills `GenDIS` unconditionally once for every event returned by `H1Tree::Next()`.  The inspected component-8956 file has HAT = MODS = H1Tree = `GenDIS` = 90,000.  Detector simulation is present (STR exists), but `IsGtr!=0` removes STR material products; detector simulation is not reconstruction conditioning.

The prior full-selection value 0.9331 for `|GenEta_lab|>3` is **numerically correct**, not a relaxed/full, beam-mixing, bin-edge, or normalization artifact.  It is a RAD-versus-NONRAD physics/domain comparison and is not evidence for reconstruction conditioning.

## Cache recomputation

The cache uses weighted `weight1*weight2`, full truth selection, and unit-area histograms.  The recomputation summed h+ and h− independently within each beam, then summed e+ and e−, and formed `[RAD/area(RAD)]/[OLD NONRAD/area(OLD NONRAD)]`.  `|eta|>3` means both signed tails, using `3<=|eta|<5` (all occupied cache support).

| |eta| interval | RAD / OLD NONRAD |
|---|---:|
| 0–1 | 1.0141799 |
| 1–2 | 0.9984635 |
| 2–3 | 0.9630892 |
| 3–10 | 0.9330083 |

The separately cached relaxed variant is radically different: 1.1472194, 1.1725391, 0.9319735, 0.7455462.  It is therefore not the source of the quoted 0.9331.  Replacing OLD by NEW NONRAD gives 1.0140556, 0.9986364, 0.9629783, 0.9340646: the conclusion does not depend on which NONRAD family is used.

The six-file direct fractions supplied in the request are a distinct bounded sample and a different aggregation.  They show a small RAD/OLD-NONRAD difference (RAD 0.498524/0.345375/0.141318/0.014778 versus OLD 0.492053/0.345823/0.146449/0.015670).  They do not invalidate the cache calculation; both results are compatible with a generator-kinematics effect rather than a detector acceptance cut.  A normalized RAD/NONRAD ratio must not be required to be unity because the samples are not matched in generated domain or radiative state.

## Attribution

| Observable | Established explanation | Reconstruction residual |
|---|---|---|
| `GenEta_lab` | Q2GT60 RAD versus Q2GT100 NONRAD plus QED ISR/FSR can change the hadronic boost, longitudinal spectrum, and selected event mixture. The 0.9330083 tail ratio is real for this unmatched pair. | None demonstrated. |
| `Q2_es_gen` | Generation cut is true Q2 while RAD `Q2_es_gen` uses ISR-effective beam energy. ISR migration through the 150 GeV2 analysis boundary and the Q2GT60/Q2GT100 domain mismatch necessarily produce a non-flat ratio. The earlier 1.0059, 0.9986, 0.9849, 0.9561, 0.9032 sequence is therefore not a reconstruction test. | None demonstrated. |
| `GenPt` | Radiation/domain kinematics change lab momentum spectra. Existing low bins 1.0011 and 1.0014 show no soft-track acceptance signature. | None demonstrated. |
| `GenP` | Radiation/domain kinematics change lab momentum spectra. Existing first two bins 1.0111 and 0.9962 show no low-momentum rejection signature. | None demonstrated. |

No numerical decomposition of the RAD/NONRAD difference into a pure Q2-domain term and a pure radiative term is identifiable from the present samples: it requires a matched true-Q2 radiative/non-radiative control.  The correct quantitative statement is that the full observed mismatch is explainable by those confounds and zero of it is uniquely attributable to reconstruction.

## Discriminating RAD-only test

One-file bounded RAD e+ `rad_ep_0642.root`, full truth selection, weighted, was stratified by `GenEnPhoton`.  The `|eta|>3` fraction is 0.0152926 for `E_gamma=0`, 0.0122684 for `0.5<=E_gamma<5`, and 0.0083806 for `E_gamma>=5` GeV.  Thus the forward population changes strongly with radiativeness (45.2% lower in the >=5 GeV stratum than at zero).  This is generator-kinematic correlation, not the radiativeness-independent signature expected from a reconstructed-event acceptance requirement.  No detector-edge discontinuity was established by this bounded test.

## Event-count accounting

| Beam | files | 100,000/file implied | retained GenDIS | difference | fraction |
|---|---:|---:|---:|---:|---:|
| e+ | 731 | 73,100,000 | 72,799,996 | 300,004 | 0.4104% |
| e− | 642 | 64,200,000 | 61,199,996 | 3,000,004 | 4.6729% |

The e+ difference is consistent with roughly three partial 100k chunks.  The e− difference is exactly about thirty 100k chunks and cannot be described as one partial final chunk.  It is consistent with many e− partial chunks, but source inventory counts alone do not distinguish that from upstream attrition.  Raw GEN/BOS event counts and event-ID equality are still required for an absolute retention claim.

## C_rad disposition

The existing RAD gen-level sample is usable as a truth-level numerator/denominator population for a reprocessed provisional `C_rad = sigma_nonrad_truth/sigma_rad_truth`; no rebuild is required **for reconstruction conditioning**.  It must be accumulated with `IsGtr!=0`, so STR material secondaries cannot enter.  The run3 RAD inflation was the already identified omitted-`IsGtr` accumulation defect, not reconstruction conditioning.

The actual blockers to an approved C_rad remain: matched RAD/NONRAD generated-domain methodology (Q2GT60 versus Q2GT100), MC luminosity/exposure provenance, sparse-bin policy, independent normalization validation, and the RAD ISR-effective-beam observable construction/phase-space validation.  `C_rad` remains unapplied.

## Artifacts

- `data/radiative/rad_conditioning_resolution_20260925/summary_tables.csv` — 18 lines.
- `docs/radiative/rad_conditioning_resolution_2026-09-25.md` — 62 lines.

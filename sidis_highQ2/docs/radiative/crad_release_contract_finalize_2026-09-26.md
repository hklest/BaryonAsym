# Release-contract `C_rad` finalization — 2026-09-26

**Status:** diagnostic calculation finalized; **`c_rad_applied=false`**. No central v6 input or release table was modified.

## Method and validation

The result uses the release-contract truth selection and the generator-level relation

`C_rad = [sum(w1*w2)/L_NONRAD] / [sum(w1*w2)/L_RAD]`.

OLD and NEW NONRAD raw sums are pooled before one beam-wide exposure division; e+/e- proxies are then mixed with `(0.5140930272, 0.4859069728)`. `L_data` is absent. The three photon definitions use the generated-electron four-vector (A), sigma photon (B), and e-Sigma photon (C) for `z` and gamma*p-CM photon-axis `pT_h`; the DIS/bin variables remain release e-Sigma quantities.

Input partial integrity is PASS: 12,978/12,978 nonzero/loadable partials with expected keys/shapes. The supplied finalizer's source manifest has blank per-file exposure cells, so it cannot parse that CSV. The preserved four-row `finalizer_exposure_manifest.csv` carries the supplied component totals: RAD `(16206393.0, 13534017.0)`, NEW NONRAD `(2519454.5686,2469812.9872)`, OLD NONRAD `(5467297.071,5461541.738)` nb^-1. This is a technical manifest repair only; it does not close luminosity provenance.

## Numerical result

The fully domain-matched result is approximately 2.04 in both charges, not a few-percent correction. This violates the requested expected-magnitude check and is diagnostic-only.

| variant | charge | integrated | mean | median | p05 | p25 | p75 | p95 | max | finite | outside [0.5,1.2] | rel-stat median/p95/max [%] |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| A nominal | h+ | 2.04009 | 2.10296 | 2.07289 | 1.72792 | 1.92158 | 2.26420 | 2.59824 | 2.88212 | 245 | 245 | 0.408/2.171/4.472 |
| A nominal | h- | 2.03902 | 2.11156 | 2.08158 | 1.75774 | 1.93045 | 2.28135 | 2.59243 | 2.77024 | 245 | 245 | 0.443/2.907/6.122 |
| B sigma photon | h+ | 2.03995 | 2.06835 | 2.06625 | 1.83261 | 1.94384 | 2.15352 | 2.37022 | 2.55784 | 245 | 245 | 0.402/2.133/4.415 |
| B sigma photon | h- | 2.03893 | 2.07434 | 2.07923 | 1.84213 | 1.96123 | 2.16178 | 2.34464 | 2.57135 | 245 | 245 | 0.438/2.850/6.066 |
| C e-Sigma photon | h+ | 2.03998 | 2.09133 | 2.09187 | 1.79254 | 1.96632 | 2.20338 | 2.44762 | 2.65413 | 245 | 245 | 0.401/2.153/4.496 |
| C e-Sigma photon | h- | 2.03895 | 2.09901 | 2.09570 | 1.82005 | 1.97858 | 2.20850 | 2.42977 | 2.66180 | 245 | 245 | 0.440/2.874/6.086 |

A→B/C leaves the integrated result unchanged (<0.007%); it changes only differential migration/shape. A’s z rise is 2.0273→2.3805, while B reduces it to 2.0349→2.2279 and C to 2.0314→2.3017. A’s pT fall is 2.0507→1.8843, B nearly removes it (2.0436→2.0072), and C partially removes it (2.0461→1.9701). Thus electron-four-vector hadron-variable definition is responsible for substantial *shape*, not the magnitude.

| aggregate bin | A nominal | B sigma photon | C e-Sigma photon |
|---|---:|---:|---:|
| z 0–0.05 | 2.02732 | 2.03486 | 2.03135 |
| z 0.05–0.1 | 2.08390 | 2.05508 | 2.06762 |
| z 0.1–0.2 | 2.11634 | 2.06542 | 2.08811 |
| z 0.2–0.3 | 2.15890 | 2.08278 | 2.11823 |
| z 0.3–0.5 | 2.20676 | 2.10633 | 2.15477 |
| z 0.5–0.7 | 2.27253 | 2.14410 | 2.20683 |
| z 0.7–1 | 2.38051 | 2.22791 | 2.30171 |
| pT 0–0.5 | 2.05070 | 2.04357 | 2.04606 |
| pT 0.5–1 | 2.03667 | 2.03710 | 2.03677 |
| pT 1–2 | 1.99619 | 2.02289 | 2.01305 |
| pT 2–4 | 1.94083 | 2.01103 | 1.98710 |
| pT 4–8 | 1.88428 | 2.00717 | 1.97007 |
| cell 0 | 2.01294 | 2.01294 | 2.01294 |
| cell 1 | 2.15551 | 2.15549 | 2.15549 |
| cell 2 | 1.88099 | 1.88097 | 1.88098 |
| cell 3 | 2.13106 | 2.13091 | 2.13092 |
| cell 4 | 1.87740 | 1.87729 | 1.87735 |
| cell 5 | 2.15191 | 2.15116 | 2.15122 |
| cell 6 | 2.02344 | 2.02186 | 2.02238 |

## Domain-match control

For A, `Q2Gki>100` changes the integrated result only from 2.03874/2.03764 (unmatched) to 2.04009/2.03902 (matched), h+/h− respectively: +0.066%/+0.068%. The full RAD finite-weight removal fraction is 33.0327%. Per-cell matched/unmatched values are: 0: 2.01294/2.01012; 1: 2.15551/2.15550; 2: 1.88099/1.87892; 3: 2.13106/2.13106; 4: 1.87740/1.87687; 5: 2.15191/2.15191; 6: 2.02344/2.02341. The historical Q2GT60/Q2GT100 generated-domain mismatch is invalid methodology, but it does not explain this factor-two result in this particular calculation.

## Historical attribution and verdict

The 2026-09-21 candidate (1.1156; median 1.1347; 114/490 outside) and v1 snapshot (1.0553/1.0554; median 1.0788) cannot be decomposed additively into the requested five named fixes from the available aggregated partials: they differ simultaneously in exposure construction, source population, selection and observable/bin definitions. Therefore no defensible individual numeric attribution is claimed for the swapped angle formula, lab-vs-CM pT, NONRAD mixture, or y fiducial. The available controls establish: y-fiducial change was +0.00314/+0.00316 (~0.28%) in the prior scan; generated-domain matching is +0.066%/+0.068% here; and A→B/C photon definition is <0.007% integrated. These are not dominant. The factor 2.04 versus 1.1156 (+82.9%) is therefore dominated by the changed normalization/exposure construction or an unresolved source-population compatibility issue, not photon definition, y, or domain matching. A reproducible one-change-at-a-time historical replay is required before assigning the remaining fixes.

**Physics verdict:** No—the corrected calculation is not consistent with the expected few-percent high-Q2 DIS correction for the 35<E-pz<70 fiducial. The original ~15% concern is not resolved into a small physical correction; the present 104% correction is more inconsistent. Its uniformly high, statistically well-populated values are not sparse-statistics artifacts. Per-bin maxima are only 2.56–2.88 with <=6.12% MC relative statistics, so sparse tails are not the cause of the magnitude. Do not apply this product.

## Blockers

- **P-CRAD-SPARSE:** sparse-bin mask/merge/smoothing policy remains undecided, despite all bins finite here.
- **P-CRAD-LUMI:** OLD/NEW NONRAD luminosity/provenance is not publication-grade; the input manifest contains blank per-file exposure fields and the supplied totals require independent closure.
- OLD/NEW NONRAD bin-local compatibility and pooled-numerator legitimacy remain unvalidated.
- The factor-two normalization/source-population discrepancy requires closure before any physical interpretation or release use.
- Coherent DJANGOH `C_rad` versus RAPGAP-response review remains open.

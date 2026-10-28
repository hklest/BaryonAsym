# Raw beam-spin asymmetry (A_LU^{sin phi_h}) — high-Q2 charged-hadron SIDIS

Date: 2026-09-16. **EXPLORATORY SIDE STUDY — raw, no background subtraction,
no radiative correction, not Born level, NOT physics-approved.**
The central v6/z005 cross-section analysis was not touched.

## 1. Result

Global, inverse-variance weighted over 330 fitted gen bins:

| quantity | value |
|---|---|
| A_sin (stat only, 349 bins) | **-0.00337 +- 0.00228** |
| A_sin (stat + sys, 330 bins) | **-0.00261 +- 0.00399** |
| A_cos null test | -0.00208 +- 0.00224 (-0.93 sigma) |
| bins above 3 sigma (total unc.) | **0 / 330** |

**The raw beam-spin asymmetry is consistent with zero** in every populated
4D bin and in the global average.

## 2. Production

Isolated additive fork of the central v6 Stage-2 builder. Central v6 source,
binary (md5 `9a585a3d0f4d3e94a699882069d7ef3c`, unchanged), steering, outputs
and release were only READ.

| item | path |
|---|---|
| source | `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6bsa.C` |
| binary | `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v6bsa` |
| condor | `H1ChargedSidis_HighQ2/condor_v6bsa/central/sidistree/` |
| outputs | `H1ChargedSidis_HighQ2/output_v6bsa/central/sidistree/data/` |

658/658 DATA files, rc=0, **7,538,865** `ChargedTot` entries; entry counts
identical file-by-file to central v6 (658/658 parity). Added branches:
`beamHelicity/I`, `beamPol/F`, `beamPolAbs/F`, `beamPolQuality/I`,
`beamLeptonCharge/I`. Nothing removed.

**MC was NOT reprocessed: `H1CalcPolarisation` returns identically zero
polarization for our MC**, so there is no polarized MC, hence no MC closure,
no MC-based BSA unfolding, and no MC acceptance correction. This is why the
method below is built to be acceptance-free.

## 3. Method

Per (charge, 4D bin) tracks are split into 7 `phi_h` bins on `[-pi,pi]`.
In each phi bin the **helicity-difference ratio** is formed:

    A_j = (N+_j - N-_j) / (N+_j + N-_j)

Helicity-independent detector acceptance **cancels exactly** in this ratio,
which is why no unfolding is applied. The 7 points are fitted with

    A_j = a0 + <|P|> * A_sin * sin(phi_j) + <|P|> * A_cos * cos(phi_j)

- `A_sin` = raw A_LU^{sin phi_h} (**central**)
- `A_cos` = parity-forbidden (**null test**)
- `a0`    = absorbs the helicity relative-luminosity imbalance (nuisance)

`phi_h` is the Trento azimuth in the gamma*+p CM frame after the `W=q+P`
boost, matching the AziAsym convention. Selection is byte-identical to the
canonical finalcandidate reco selection (S3xmerged/zlow005/pT5/eta1.6/
radlen5/nhit5) plus `beamPolQuality != 0`, `0 < |beamPol| < 1`, and
`sign(beamPol) == beamHelicity`.

Fit quality: pull RMS 0.99, chi2/ndf 0.98, inter-stratum consistency 0.95.

### Three bugs found and fixed during development
1. **Polarization-weighted raw moment is wrong.** `sum(P sin phi)/sum(P^2 sin^2 phi)`
   multiplies the *unpolarized* azimuthal acceptance by the mean polarization
   and produced a fake 9-sigma cos(phi) signal. Replaced by the phi-binned
   helicity difference.
2. **Degenerate phi bins.** Where one helicity state is empty the ratio
   saturates at +-1 and the binomial sigma collapses, giving a bin infinite
   fit weight and `|a0| -> 1`. Fixed with Agresti-Coull regularization plus a
   `>= 5` counts-per-helicity-per-phi-bin requirement and a condition-number cut.
3. **Stratification by output directory is invalid.** `ep0607/` holds both
   d06 (e-p) and d07 (e+p) files. Directory-stratified fits mix lepton
   charges, so the single shared `a0` cannot absorb two different
   relative-luminosity offsets and the residual leaks into `A_sin`. This alone
   produced a spurious **+0.021 +- 0.002** signal. Stratification is now by
   **beam configuration (lepton charge)**, and the signal vanishes.

## 4. Control / null tests

| control | A_sin +- stat | A_cos +- stat |
|---|---|---|
| nominal | -0.00337 +- 0.00228 | -0.00208 +- 0.00224 |
| helicity scrambled (seed 12345) | -0.00022 +- 0.00234 | -0.00374 +- 0.00230 |
| helicity scrambled (seed 777) | +0.00043 +- 0.00234 | -0.00245 +- 0.00230 |
| e+p only | -0.00515 +- 0.00295 | **-0.01160 +- 0.00290** |
| e-p only | -0.00074 +- 0.00358 | **+0.01194 +- 0.00352** |
| polQual=1 (TPOL) | -0.00518 +- 0.00424 | -0.00175 +- 0.00418 |
| polQual=2 (LPOL) | -0.01408 +- 0.00754 | **+0.02336 +- 0.00751** |
| polQual=3 (both) | -0.00052 +- 0.00280 | -0.00555 +- 0.00276 |

Scrambling the helicity sign per run removes the signal, confirming that what
remains is genuinely helicity-correlated rather than a fit artifact.

## 5. Systematics

Median per bin, over 330 bins:

| source | median | / stat |
|---|---|---|
| statistical | 0.0891 | 1.00 |
| **false asymmetry** | **0.1095** | **1.23** |
| selection (cut family, max shift) | 0.0259 | 0.29 |
| fit model (phi bins, sin2phi, regularization) | 0.0204 | 0.23 |
| polarization scale (3.5%, PLACEHOLDER) | 0.0019 | 0.02 |
| **total systematic** | **0.1220** | **1.42** |

**The leading systematic is the false/instrumental asymmetry**, taken per bin
as the largest of |A_cos|, half the |e+p - e-p| spread, and the scramble
residual. Its evidence is that **A_cos flips sign between beam charges**
(-4.0 sigma in e+p, +3.4 sigma in e-p) while a genuine beam-spin asymmetry
cannot depend on the lepton beam charge. It largely cancels in the combined
sample but not bin by bin. `polQual=2` (LPOL) also shows a +3.1 sigma A_cos,
so the polarimeter split is a second handle on the same effect.

Cut/fit variations are all below 0.5 sigma globally. The polarization scale
is negligible *only because A_sin is consistent with zero*; it is purely
multiplicative and would become leading for any nonzero signal.

Not applicable to an asymmetry (they cancel in the helicity ratio): the 2.7%
luminosity normalization, the -0.5% tracking efficiency, IBU stat/model/reg,
and the bulk of the radiative correction.

## 6. Blockers before any physics claim

1. Official H1 HERA-II LPOL/TPOL polarization scale uncertainty — the 3.5%
   used here is an explicitly flagged placeholder.
2. Root-cause the charge-dependent A_cos false asymmetry.
3. Background dilution: unpolarized background dilutes A by the signal
   fraction f (A_true = A_raw/f); not yet applied.
4. phi_h migration dilution bound using unpolarized MC (permitted use).

## 7. Artifacts

| kind | path |
|---|---|
| extractor | `sidis_highQ2/scripts/bsa/build_bsa_araw.py` |
| budget builder | `sidis_highQ2/scripts/bsa/build_bsa_systematics.py` |
| plotters | `sidis_highQ2/scripts/bsa/plot_bsa_results.py`, `plot_spin_qa_smoke_2026-09-16.py` |
| validator | `sidis_highQ2/scripts/bsa/validate_v6bsa_trees.py` |
| nominal result | `sidis_highQ2/data/bsa/araw_full_2026-09-16.json` |
| controls | `sidis_highQ2/data/bsa/araw_{scr1,scr2,eplus,eminus,polq1,polq2,polq3}_2026-09-16.json` |
| cut/fit variations | `sidis_highQ2/data/bsa/variations/` |
| systematic budget | `sidis_highQ2/data/bsa/systematics_budget_2026-09-16.json` |
| result plots | `sidis_highQ2/plots/bsa/results_2026-09-16/bsa_araw_results_2026-09-16.pdf` |
| spin QA plots | `sidis_highQ2/plots/bsa/spin_qa_smoke_2026-09-16/` |
| systematics plan | `sidis_highQ2/docs/bsa/systematics_plan_bsa_2026-09-16.md` |

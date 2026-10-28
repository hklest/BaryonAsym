# Raw beam-spin asymmetry (A_LU^{sin phi_h}) — two-pT-bin high-Q2 charged-hadron SIDIS

Date: 2026-09-16. **EXPLORATORY - raw, no background subtraction, no radiative correction, not Born level, NOT physics-approved.** The central v6/z005 cross-section analysis was not touched.

## 1. Product and binning

This JSON-only derived product collapses the original generator `pT_h` axis
`[0, 0.5, 1, 2, 4, 8]` GeV into `[0, 1, 8]` GeV:

- LOW `pT_h`: `[0,1)` GeV, exact raw-count union of old indices 0 and 1.
- HIGH `pT_h`: `[1,8)` GeV, exact raw-count union of old indices 2, 3 and 4.

The seven Q2/x cells (including merged `S3_x0`) and seven z bins are unchanged,
so there are **98 generator bins per charge** (196 total). No ROOT trees were
read and no events were reprocessed. The helicity yields were summed before
fitting; beam configurations `lep+1` and `lep-1` were fitted separately and
combined only afterwards by inverse variance.

## 2. Mandatory identity closure

Reconstructing the original five-pT layout from its stored raw helicity yields
and refitting gives **PASS** at relative tolerance `1e-6`: 490 bins and 1,745
finite fitted quantities were compared, with maximum relative deviation
`1.55216e-13`. The stored-fitter estimator is therefore reproduced.

## 3. Polarization-average QA

The merged-bin `<|P|>` is formed from old per-stratum values using `N_tot`
weights. This is a bookkeeping approximation because stored yields do not retain
per-track polarization sums. The old-pT relative spread exceeds 1% in **125**
(cell, z, charge, stratum) groups, reaching **6.2986%**. This is an explicit
blocker: results must not be promoted beyond this exploratory derived product
without a count-and-polarization-sum-preserving re-extraction.

## 4. Global inverse-variance results

`A_sin` values are stat-only and stat+systematic respectively. `A_cos` is the
statistical null test; its final column is its signed statistical significance.

| region | fitted / total bins | A_sin (stat) | A_sin (stat+sys) | A_cos (stat) | A_cos sigma |
|---|---:|---:|---:|---:|---:|
| LOW pT_h | 76 / 98 | -0.00424 +- 0.00251 | -0.00403 +- 0.00449 | +0.00045 +- 0.00252 | +0.18 |
| HIGH pT_h | 78 / 98 | +0.00030 +- 0.00545 | -0.00366 +- 0.00954 | -0.01098 +- 0.00485 | -2.26 |
| S3_x0 LOW pT_h | 9 / 14 | +0.00201 +- 0.01349 | -0.00369 +- 0.02194 | +0.02935 +- 0.01315 | +2.23 |
| S3_x0 HIGH pT_h | 10 / 14 | -0.02640 +- 0.02604 | -0.02807 +- 0.04722 | +0.00466 +- 0.02239 | +0.21 |

There are 154 fitted and 42 dropped charge-separated bins. **0 / 154** budgeted
bins exceed 3 sigma using total uncertainty.

## 5. Method and systematic budget

At each phi point, `A_j=(N+_j-N-_j)/(N+_j+N-_j)` is fit with
`A_j=a0+<|P|>*A_sin*sin(phi_j)+<|P|>*A_cos*cos(phi_j)`, using the original
binomial weighting, Agresti-Coull regularization, at least five counts in each
helicity state per used phi bin, and the original condition-number gate.
`phi_h` is Trento in the gamma*+p CM frame after the W=q+P boost.

The budget uses the original semantics: maximum shift within cut and fit
families, a visible 3.5% **PLACEHOLDER** polarization scale, and false
asymmetry `max(|A_cos|, |A_sin(e+p)-A_sin(e-p)|/2, scramble residual)` per bin.

## 6. Artifacts

- Binning: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/bsa/binning_bsa_pt2.py`
- Driver: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/bsa/rebin_bsa_pt2.py`
- Nominal: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/bsa/pt2/araw_full_pt2_2026-09-16.json`
- Budget: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/bsa/pt2/systematics_budget_pt2_2026-09-16.json`
- Closure: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/bsa/pt2/closure_identity_check.json`

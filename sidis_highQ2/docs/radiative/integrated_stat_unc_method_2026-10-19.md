# Integrated statistical uncertainties of the releasebin-basis integrated C_rad values — method record (2026-10-19)

**Scope.** Integrated statistical uncertainties (absolute, dimensionless, same units as
`C_rad`) for the integrated `C_rad` values of the five current releasebin-basis tables,
per hadron charge (h+, h−), in the ratio-of-integrals convention
(`integrated C_rad = sum(sigma_nonrad)/sum(sigma_rad)` over the 245 bins of each charge
on the 490-bin grid — exactly how the published integrated values were computed).
Deliverables: `data/radiative/eminus_withz_crad_releasebin_2026-10-19/integrated_stat_unc_for_note_2026-10-19.json`
(full precision, gates, provenance) produced by
`.../scripts/compute_integrated_stat_unc_for_note_2026-10-19.py`.
**Status: PROVISIONAL DIAGNOSTIC bookkeeping figure.** No central artifact was modified;
`c_rad_applied` status and the applied mixed-beam baseline are unchanged. This record
*complements, and does not supersede*, the 2026-10-19 derivation record
(`EMINUS_WITHZ_CRAD_DERIVATION_2026-10-19.md` §2 / caveat 5), which correctly states
that a rigorous integrated stat unc requires event-level covariance across bins that the
per-bin primitives do not carry.

## Method (leg-level effective statistics)

Campaign convention per leg per bin: `sigma_i = sum_y sumw_y / L`, `var_i = sum_y sumw2_y / L^2`
(weighted-event statistics). Integrated per charge per leg: `S = sum_i sigma_i`,
`V = sum_i var_i = (total sumw2)/L^2 = S^2/N_eff`. The four legs
(`nA = sigma_nonrad(em)`, `nD = sigma_nonrad(ep)`, `rB = sigma_rad(em)`, `rC = sigma_rad(ep)`)
are separate MC samples and statistically independent (the established 4-leg independence
assumption); DJANGOH14 nonrad pools old+new over the combined audited exposure. For a
ratio of integrals `C = S_n/S_r` with independent numerator/denominator legs:
`var(C) = V_n/S_r^2 + C^2 * V_r/S_r^2`; f-weight mixtures propagate as `var = sum_b f_b^2 V_b`.

- **Tables 1–2 (mixed DJANGOH14 / RAPGAP31-emfix):** `S_n = f_em*N_A + f_ep*N_D`,
  `S_r = f_em*R_B + f_ep*R_C` (f = table-construction weights 0.4859069728/0.5140930272);
  both legs of numerator and denominator enter the propagation.
- **Tables 3–4 (e− p WithZ, T = C_mixed·F_exact):** integrated
  `T_int = N_A/(g_em*R_B + g_ep*R_C)`, identically `C_mixed_int * F_exact_int` with
  `R_born_int = N_D/N_A`, `R_rad_int = R_C/R_B` (g = current data split
  0.4559592527026241/0.5440407472973759). Primary propagation: the direct 3-leg analytic
  formula (the `nD` leg cancels identically, `dT/dN_D = 0`). Cross-check: the 4-leg
  F_exact-route propagation lifted to integrated level (analytic gradient, shared legs
  handled exactly), mirroring the per-bin two-route machinery of the 2026-10-19 derivation
  — the two routes agree to ≤ 7.2e-16 relative.
- **Table 5 (NoZ photon-Born C_gamma_born, DJANGOH-only):**
  `C_int = N_G/(f_em*R_B + f_ep*R_C)` with the NoZ e− numerator (L = 14,119,125.013920162
  nb⁻¹) and the f-mixed DJANGOH14 RAD denominator. **The merged numerator `sumw2`
  primitive is genuinely absent** (the numerator exists only as 260 per-chunk partials;
  the campaign finalizer merged them in-memory), and no Condor re-merge was run for this
  deliverable. The numerator per-bin variance is therefore **recovered from the published
  per-bin `C_gamma_born_stat_unc` column**: `nv_i = n_i^2*((u_i/C_i)^2 - rv_i/r_i^2)`, with
  `rv_i` the mixed-RAD denominator variance computed exactly from the merged RAD npz legs.
  The recovery is algebraically exact given the campaign ratio formula
  `u = C*sqrt(nv/n^2 + rv/r^2)` and is validated by: `nv_i > 0` in 490/490 bins;
  Cauchy–Schwarz `N_eff,i = n_i^2/nv_i <= gamma_born_n,i` in 490/490 bins (0 violations);
  and y-resolved recovery consistency (`sum_y nv_yi = nv_i` to ≤ 8.6e-16).

**Caveat (required wording).** This integrated stat unc treats per-leg weighted statistics
as **independent across bins** (`V_leg = sum_i var_i`). It is a **lower-bound-style
bookkeeping number** in the same spirit as the per-bin convention, **not an event-level
covariance propagation**: entries sharing an event weight `w1` (multiple hadrons per
event, possibly across bins) are not propagated, and no event-level covariance information
exists in the primitives. The campaign convention of quoting integrated values without a
rigorous stat unc remains the governing statement; the numbers below are the labeled
bookkeeping figures requested for the analysis-note table.

## Results (integrated C_rad ± integrated stat unc, absolute)

| # | table | h+ | h− |
|---|---|---|---|
| 1 | mixed DJANGOH14 releasebin | 1.1415172920 ± 0.000124 (0.0108%) | 1.1410367377 ± 0.000125 (0.0109%) |
| 2 | mixed RAPGAP31 releasebin emfix | 1.1342370401 ± 0.000125 (0.0111%) | 1.1336689310 ± 0.000126 (0.0112%) |
| 3 | e− p WithZ DJANGOH14 (T) | 1.1478488555 ± 0.000151 (0.0132%) | 1.1466412341 ± 0.000152 (0.0133%) |
| 4 | e− p WithZ RAPGAP31 (T) | 1.1371916091 ± 0.000155 (0.0136%) | 1.1365215812 ± 0.000156 (0.0138%) |
| 5 | NoZ C_gamma_born DJANGOH14 | 1.0409605405 ± 0.000143 (0.0137%) | 1.0401468081 ± 0.000144 (0.0138%) |

Full-precision values and uncertainties (and per-leg `N_eff`, variance contributions,
leg sums/variances) are in the JSON. Integrated `N_eff` per leg per charge is
0.73–1.6 × 10⁸; the uncertainty is dominated roughly half by numerator and half by
denominator legs (tables 1–2), by the nonrad-em leg (~65–68%) for the e− WithZ T, and
~67%/33% numerator/denominator for the NoZ table. Absolute uncs are ~1.2–1.6 × 10⁻⁴
(0.011–0.014% relative) — about 40× smaller than the per-bin median (0.5–0.6%), as
expected for a ratio of integrated sums dominated by the highest-statistics bins.

## Gates (all PASS; every integrated value reproduced before its unc is quoted)

| gate | tolerance | measured (max) |
|---|---|---|
| integrated C_rad reproduced vs published (all 5 tables, both charges) | ≤ 1e-9 rel | 9.7e-16 |
| per-bin bookkeeping vs published stat-unc columns (mixed C/unc, per-beam C/unc, T/unc) | ≤ 1e-9 rel | 7.9e-16 (T columns bit-exact, 0.0) |
| e− WithZ integrated identity `T ≡ C_mixed·F_exact` | ≤ 1e-12 rel | 3.9e-16 |
| e− WithZ 4-leg vs 3-leg propagation routes | ≤ 1e-9 rel | 7.2e-16 |
| NoZ numerator-var recovery: positivity / Cauchy–Schwarz / y-resolved consistency | 490/490, 0 viol., ≤ 1e-9 | 8.6e-16 |
| NoZ RAD legs vs independent 2026-10-19 re-merge copies | bit-identical | pass |
| exposures vs npz diag + audited values; file counts vs campaign manifests; `sumw`/`n` projections exact | exact / ≤1e-14 | pass |

Checked non-issue: the plain `sumw2` arrays of the 2026-10-19 per-beam re-merge differ
from `sumw2_y.sum(axis=0)` only by float64 accumulation noise (≤ 3.8e-15 rel); the
convention quantity `sumw2_y` is bit-identical between the two independent merges
(2026-10-09 nozborn rebuild and 2026-10-19 re-merge) of the same partials.

**Provenance.** Legs from the merged per-beam npz primitives
(`eminus_withz_crad_releasebin_2026-10-19/perbeam_remerge/{djangoh14,rapgap31}/` and the
nozborn campaign's own RAD primitives); published references read from
`crad_eminus_withz_releasebin_validation.json` (tables 1–4, full precision) and
`cgammaborn_v1_summary.json` (table 5); 24 inputs sha256-hashed in the JSON. Task-quoted
published values agree with the artifact references exactly (tables 1/2/5) and to
≤ 4.2e-11 rel (tables 3/4, 10-dp quotes). **No Condor was needed or used.**

# Design Proposal: Multidimensional Smoothing of Per-Bin Systematic Components (v6/z005)

**Date written:** 2026-10-03 (filename keeps the requested 2026-10-02 label)
**Scope:** high-Q2 inclusive charged-hadron SIDIS, v6/z005, 490 published IBU-k=4 bins (245 h+ + 245 h-)
**Status:** DESIGN ONLY. Nothing implemented, no frozen-menu magnitude changed, no central product or total table regenerated or overwritten. All numbers below come from read-only inspection of existing CSV/JSON artifacts on the login node.

---

## 1. Recommendation in one paragraph

Adopt **source-wise, sign-aware 2D second-moment smoothing over (z, pT) within each (charge, (Q2,x) cell)**: recover the *signed* per-bin shift `d_i` for each source, estimate a per-bin noise floor `sigma_i` for the shift itself, smooth `d_i` with a noise-weighted 2D local-linear (Gaussian-kernel) smoother whose bandwidths are fixed a priori (`h_z = 1 z-bin`, `h_pT = 1 pT-bin`, matching the adopted phi_h precedent scale), and report the per-bin systematic as the **second moment** `u_i = sqrt(m_i^2 + v_i)`, where `m_i` is the smoothed signed estimate and `v_i` its estimation variance. This lifts zero-crossing dips automatically (at a sign crossing `u_i -> sqrt(v_i)`, never zero), never shrinks below the statistical precision of the shift (`v_i >= sigma_i^2/N_eff`), is the budget-consistent object under quadrature combination, and is the principled version of "smoothed plus added smoothing-systematic term". Full 4D (Q2,x,z,pT) fitting is **not justified** by the actual bin population and noise content; 2D within cell is the maximum defensible dimensionality. The |delta|-smoothing family (2D extension of the adopted `phi_h_gauss_rel` prescription) is the acceptable conservative **fallback** if the noise-floor prerequisites (Section 8) cannot be met.

---

## 2. Evidence: what the per-bin components actually look like

### 2.1 Bin geometry (verified)

`bin_index = (cell_idx * 7 + z_idx) * 5 + pT_idx` verified on all 490 rows. Per charge: **7 (Q2,x) cells** (`S0_x0, S0_x1, S1_x0, S1_x1, S2_x0, S2_x1, S3_x0`; S0/S1/S2 have two x-subbins, S3 has one) x **7 z bins** x **5 pT bins** = 245. Each (charge, cell) surface is a 7x5 = 35-point grid; each (charge, cell, z) pT row has only 5 points.

### 2.2 Jitter diagnostics of the published |delta| components

From `total_uncertainty_summary_with_tracking.csv` (490 rows, 10 components), profiles along pT within each (charge, cell, z) row:

| Component | lag-1 autocorr. (signed basis where available) | roughness (mean adjacent &#124;Δf&#124; / mean f) | V-dip bins (zero-crossing signature) | median | max |
|---|---:|---:|---:|---:|---:|
| `ibu_stat_rel` | **+0.137** | 0.41 | 0.0% | 3.73% | 60.7% |
| `ibu_model_rel` | -0.118 | 0.81 | 9.8% | 1.59% | 37.0% |
| `ibu_reg_rel` | -0.174 | 0.82 | 10.6% | 0.57% | 29.8% |
| `hfs_jes_rel` | -0.102 | 0.77 | 9.4% | 0.78% | 25.2% |
| `hfs_rces_rel` | -0.050 | 0.61 | 7.1% | 0.24% | 6.6% |
| `hfs_theta_rel` | -0.062 | 0.62 | 4.9% | 0.26% | 5.1% |
| `elec_energy_rel` | -0.115 | 0.87 | 10.6% | 1.45% | 140.9% |
| `elec_theta_rel` | -0.120 | 0.78 | 9.2% | 0.46% | 66.0% |
| `tracking_eff_m0p5_rel` | -0.046 | 0.67 | 6.3% | 0.18% | 2.8% |
| `lumi_rel` | constant 2.7% | — | — | — | — |

V-dip = center bin below half of *both* neighbors and below half the row median — the fingerprint of a zero crossing of the underlying signed shift. Every shifted/model component has **5-11% of bins in unphysical dips** and roughness of 0.6-0.9 (adjacent-bin differences are 60-90% of the mean value). Only `ibu_stat_rel` has positive spatial autocorrelation (genuine structure from yield sizes).

### 2.3 Signed-shift diagnostics (per-source comparison files)

| Source (signed basis) | % negative | pT-row sign flips (of 392 adjacent pairs) | lag-1 AC, signed | lag-1 AC, &#124;signed&#124; | median &#124;s&#124; / IBU-stat | % bins &#124;s&#124; < IBU-stat |
|---|---:|---:|---:|---:|---:|---:|
| `hfs_theta_up` | 21.4% | 65 (17%) | -0.029 | -0.062 | 0.09 | 99.4% |
| `hfs_jes_down` | 24.7% | 107 (27%) | -0.039 | -0.102 | 0.26 | 95.7% |
| `hfs_rces_up` | 22.7% | 65 (17%) | -0.021 | -0.050 | 0.09 | 99.4% |
| `elec_energy` | 46.1% | 183 (47%) | -0.066 | -0.115 | 0.45 | 84.5% |
| `elec_theta` | 36.3% | 144 (37%) | -0.123 | -0.120 | 0.15 | 99.4% |
| `tracking` (16-seed mean) | 65.7% | 97 (25%) | -0.027 | -0.046 | 0.05 | 100% |
| `ibu_model` (signed half-diff) | 48.8% | 185 (47%) | -0.080 | — | 0.45 | 78.4% |

Two decisive facts:

1. **The signed shifts are themselves noise-dominated.** Lag-1 autocorrelation of the *signed* shift is approximately zero to negative for every source. There is essentially no smooth per-bin signal to "recover" by signed smoothing; the coherent content lives at the pT-row level and above, and even there it is marginal (next table).
2. **The per-bin IBU statistical uncertainty is NOT the noise scale of the shifts.** Median |shift| is only 5-45% of `ibu_stat_rel` — nominal and shifted unfoldings share the same data events, so most data-Poisson noise cancels in the difference. A source-specific noise floor is mandatory; using `ibu_stat_rel` as the floor would be wrong in both directions (hugely conservative for hfs_theta/tracking, *underestimating* the true difference-noise for `elec_energy`, whose row-residual scatter is ~8.8%).

### 2.4 Noise vs coherent-structure decomposition

Residual scatter of the signed shift around its 5-bin pT-row mean (`sigma_bin`), vs the row-to-row scatter of row means (`sigma_row`, coherent row-level signal, deconvolved), and the z-direction residual scatter (`sigma_z`):

| Source | sigma_bin | sigma_row (coherent) | bin-noise fraction of variance | sigma_z |
|---|---:|---:|---:|---:|
| `hfs_theta_up` | 0.33% | 0.34% | 48% | 0.46% |
| `hfs_jes_down` | 2.70% | 0.00% | **100%** | 2.68% |
| `hfs_rces_up` | 0.37% | 0.20% | 77% | 0.42% |
| `elec_energy` | 8.80% | 2.22% | 94% | 8.97% |
| `elec_theta` | 3.11% | 0.66% | 96% | 3.20% |
| `tracking` (mean) | 0.34% | 0.20% | 74% | 0.35% |
| `ibu_model` | 4.84% | 2.23% | 83% | 5.09% |

**Classification of sources:**

- **Genuine, measurable structure (smoothing has something real to track):** `hfs_theta_up` (52% of variance coherent; 79% same-sign rows), `tracking` (coherent negative bias — 66% of bins negative, physically the expected yield loss; 16-seed ensemble gives the noise floor directly: median seed RMS 0.242%, median seed-mean uncertainty 0.060%, median |mean shift| 0.182%, i.e. ~3 sigma typical per-bin significance), `ibu_model` (coherent 2.2%; pos/neg correlation +0.51).
- **Zero-crossing jitter dominated (per-bin values unphysical; the coherent part is at/below measurement noise):** `hfs_jes_down` (0% coherent row-level variance in pT — pure bin noise), `elec_energy` (94% noise, 47% sign-flip rate = sign-random, 141% heavy tail), `elec_theta` (96% noise), `ibu_reg` (most anticorrelated component, lag-1 = -0.174), `hfs_rces_up` (marginal, 77% noise).
- **Also affected:** `crad_gen_envelope` (adopted 2026-10-02 side of the budget): roughness 0.75, 36.1% of signed `rel_diff_vs_mean` negative, median 0.92%. Special case: the underlying `c_rad` values are physically smooth curves and per-bin statistical uncertainties are already stored (`c_rad_djangoh_stat_unc`, `c_rad_rapgap_stat_unc`) — a weighted signed smoothing is well-posed here *once the C_rad convention blocker closes*.
- **NOT affected:** `ibu_stat_rel` (positive autocorrelation; it is itself the noise reference), `lumi_rel` (constant), the frozen central-tracking prior 1.0198039027% (constant, diagonal, charge-independent), `phi_h_gauss_rel` (already smoothed by its own adopted prescription).

### 2.5 Charge correlation (matters for A_h)

Matched-bin pos/neg correlation of the *signed* shifts: `tracking` +0.63, `ibu_model` +0.51, `hfs_jes` +0.27, `hfs_rces` +0.06 (Spearman +0.25), `hfs_theta` -0.07, `elec_theta` +0.01, `elec_energy` -0.43 (Pearson; Spearman +0.20 — heavy-tail driven). For the detector sources the per-bin charge *difference* — the part that does not cancel in A_h — is noise-dominated. Any smoothing that pools charges into one common mode would silently assert near-full charge cancellation in A_h, which the data do not support. Charges must be smoothed separately with shared hyperparameters, and the charge-correlation diagnostics reported alongside.

### 2.6 What the existing 1D precedent already established

The phi_h method-selection record (`flatphi_phih_systematic_adoption_2026-10-02.md`) is direct evidence for this design: noise-weighted local-linear (h=2.01) smoothing of the **signed** delta with a 2-sigma excursion guard *did not lift the zero-crossing dips* (18 h+ / 16 h- bins stuck at median 0.475% / 0.396%), while the Gaussian (sigma=1 pT bin) smoothing of **|delta|** lifted them (chosen, adopted as `phi_h_gauss_rel`), at the cost of a documented noise-guard defect (suppresses 10/37 and 13/29 of the >2-sigma halfA-significant excursions). The second-moment estimator below repairs both defects: it lifts dips *by construction* (the `v_i` term) and it *keeps* significant localized excursions (they enter `m_i` with their measured weight, and the guard flags rather than suppresses).

---

## 3. Statistical model (why each naive fix is biased)

Model per source and bin: measured signed shift `d_i = s_i + eps_i`, with `s_i` the true (unknown, physically smooth) systematic response and `eps_i` the noise of the shifted-vs-nominal unfolding difference, `eps_i ~ N(0, sigma_i^2)` (heavy-tailed in sparse bins). Then:

- **Raw |d_i|** (current table): upward-noise-biased where noise dominates (`E|s+eps| >= |s|`; at `s=0`, `E|eps| = 0.8 sigma`), and dips to zero at sign crossings. Jagged in both directions — exactly what is observed.
- **Signed smoothing then |smoothed|** (candidate a): regression to the mean. With near-zero signed autocorrelation (Section 2.3) the smoothed estimate `m_i` is shrunk toward the local/row mean, `|m_i|` undershoots the amplitude, and dips persist at the smooth component's zero crossings. Empirically confirmed (Section 2.6). Any post-hoc "local |delta| floor" pasted onto the dips double-counts the noise already present in `|d|` and is arbitrary.
- **|delta| kernel smoothing** (candidate b, the adopted precedent): no dips, but `E|s+eps| >= |s|` means it is biased *high* in noise-dominated bins (cosmetically fine, statistically an inflation), it flattens genuine peaks, and it suppresses genuinely significant localized excursions (documented for phi_h). Honest but uncalibrated; conservative.
- **Envelope max(|raw|, |smoothed|)** (candidate d): monotone, but keeps every raw noise spike (a 141% `elec_energy` tail bin survives untouched — the jitter complaint is not solved), and max-selection over noisy quantities is upward-biased (the same defect already documented for the two-generator phi_h envelope). Reject for the budget; keep as a sidecar audit column.
- **Second moment `u_i = sqrt(m_i^2 + v_i)`** (recommended): under the model, `E[s_i^2 | data] = m_i^2 + v_i` for a linear-Gaussian smoother, so `u_i` estimates the RMS magnitude of the true response. It (i) cannot dip to zero (`u_i >= sqrt(v_i)` at crossings), (ii) reduces to |d_i| when the signal is far above noise, (iii) reduces to the noise floor `~ sigma_i/sqrt(N_eff)` when the per-bin response is unmeasurable — which is the correct, non-cheating statement of "we cannot resolve this source in this bin; we assign our resolution", and (iv) is the object that is *consistent with quadrature combination* (variances add). The residual Jensen (sqrt) downward bias is quantified and corrected by the toy closure gate (G3).

**The recommended estimator is thus the principled synthesis of the candidate families:** the 2D geometry of (b), the signed information of (a), and the "smoothed + added smoothing-systematic term" variant of (d), with the noise model making it calibrated rather than cosmetic.

---

## 4. The recommended procedure (per source, per charge)

**S0. Scope.** Applied only to the jitter-affected, signed-recoverable sources: `hfs_jes_rel`, `hfs_rces_rel`, `hfs_theta_rel`, `elec_energy_rel`, `elec_theta_rel`, `tracking_eff_m0p5_rel`, `ibu_model_rel`, `ibu_reg_rel` (pending signed recovery), and later `crad_gen_envelope` (pending its convention blocker). Never to `ibu_stat_rel`, `lumi_rel`, the frozen 1.01980392% diagonal prior, `phi_h_gauss_rel`, or any total.

**S1. Signed input.** `d_i` = (shifted - nominal)/nominal per bin, per charge, from the existing per-source comparison artifacts (provenance in Section 9). For `tracking`: the 16-seed mean shift (signed, recoverable as `shift_mean - nominal`). For `ibu_model`: the signed half-difference of the two cross-closure biases (verified: the published `ibu_model_rel` equals `current_model_unc_rel` of the data-scaled crossclosure file in 490/490 bins). For `ibu_reg`: signed tau x2 / tau / 2 half-difference must be recovered from upstream artifacts (the release stores only absolute values) — prerequisite P2.

**S2. Per-bin noise floor `sigma_i` for the shift.**
- `tracking`: `sigma_i = seed_mean_unc_i` (16-replica SEM); also report the robust (MAD-based) SEM because the per-seed distribution is heavy-tailed.
- `crad_gen_envelope`: `sigma_i` from the stored per-bin `c_rad_*_stat_unc` of the two generators.
- One-sided detector shifts (JES, RCES, HFS theta, elecE, elecTheta) and `ibu_model`/`ibu_reg`: **no replica noise exists yet** — this is prerequisite P1 (Section 8). Production plan: N ~ 100 data-bootstrap (or disjoint event-split) replicas, nominal AND shifted re-unfolded per replica, `sigma_i` = RMS of the per-replica signed difference (paired, so data-Poisson largely cancels — this is the whole point). Interim fallback, explicitly labeled PROVISIONAL: robust row-residual scale `sigma_hat = 1.4826 x MAD` of signed residuals within each (charge, cell) 35-point grid (pooling z and pT residuals), with the inflation factor fixed a priori (Section 2.4 shows `sigma_bin` is the right order; the fallback cannot capture bin-dependent noise such as the pT-rising and high-z tails, which is why P1 is required before any physics-facing use).

**S3. 2D noise-weighted local-linear smoothing on the (z, pT) grid within each (charge, cell).** For target bin i, fit a local plane `a0 + a1(z_j - z_i) + a2(pT_j - pT_i)` by weighted least squares over the 35 bins j of the same (charge, cell), weights `w_ij = K_ij / max(sigma_j^2, 1e-12)`, Gaussian kernel `K_ij = exp(-0.5 ((z_j-z_i)/h_z)^2 - 0.5 ((pT_j-p_i)/h_p)^2)` truncated at the cell boundary (never borrow across cell, z-block, or charge boundaries). **Hyperparameters fixed a priori: `h_z = 1` z-bin, `h_pT = 1` pT-bin** (same scale as the adopted phi_h Gaussian; every cell point has nonzero support, no external borrowing). Local-linear removes edge bias without ad hoc edge renormalization. Robust fitting: bins with `|d_j| > 50%` are excluded from the fit and flagged for the instability investigation required by the standing trap (e.g. the 141% `elec_energy` tail bins must not drag their neighbors). Outputs: `m_i` (fitted signed value), `v_i` (weighted-LS estimation variance), `N_eff,i`.

**S4. Per-bin systematic.** `u_i = sqrt(m_i^2 + v_i)`. Sidecar per bin retains: `d_i` (raw signed), `|d_i|`, `m_i`, `v_i`, `sigma_i`, `N_eff,i`, fit-influence/exclusion flags, and `max(|d_i|, u_i)` as an audit-only envelope column.

**S5. Excursion guard (flag, do not suppress).** If `|d_i - m_i| > 2 sigma_i`, flag `excursion=True`. Flagged bins keep the S4 value in the budget but are listed for physics review (a genuinely localized response effect must be confirmed by an independent check, not silently smoothed away nor silently kept). This inverts the phi_h precedent's suppress-or-preserve behavior into flag-and-review, which is what "not cheating" requires: the smoother must never be the arbiter of what is physical.

**S6. Combination.** New column `total_s3m_rel = sqrt(sum_s u_{s,i}^2)` computed strictly after per-source smoothing. Smoothing is never applied to the total, never to a quadrature, and never iterated after the total is seen (gate G2). Existing products (`total_with_tracking_rel`, `total_with_phih_rel`, `..._with_crad`) are left untouched; results go to new dated directories.

**S7. Asymmetry bookkeeping.** `A_h` is derived afterward from the varied h+ and h- (per the frozen `Delta_i = (N_i^+ - N_i^-)/2` convention and A-from-varied-charges rule), using `m^±` and `sqrt(v^±)`. Luminosity cancels exactly. The charge-common mode of a source (extractable as a diagnostic, e.g. tracking rho = +0.63) largely cancels in A_h; the charge-uncorrelated noise-floor parts do not cancel and are bookkept separately as the A_h-relevant residual. No rank-1 `u u^T` covariance is constructed from the smoothed curves (standing trap); any future covariance must come from the P1 replicas.

---

## 5. Candidate families: verdict summary

| Family | Verdict | One-line reason |
|---|---|---|
| (a) Signed smoothing -> &#124;smoothed&#124; + zero-crossing patches | **Reject as primary** | Signed shifts have ~zero autocorrelation (nothing smooth to recover); regression-to-mean undershoot; dips empirically not lifted (phi_h record); ad hoc floors double-count noise. Its `m_i` survives as one term of S4. |
| (b) &#124;delta&#124; with 2D (z,pT) kernel | **Accept as fallback / conservative variant** | Minimal change from the adopted precedent, no dips; but biased high where noise dominates (uncalibrated), flattens peaks, and suppresses significant excursions; acceptable only if P1 noise floors cannot be produced, with the bias documented per source. |
| (c) Full 4D parametric fit (tensor B-spline / poly in log pT, z, log x, log Q2) | **Reject for the per-bin budget** | 245 bins/charge with 74-100% bin-noise variance and heavy tails cannot constrain it; CV would collapse it to near-constant cross-cell pooling that is physically wrong (migration responses differ per (Q2,x) cell) — systematic shrinkage dressed as rigor. Reserve for per-source global scale cross-checks and for the smooth `c_rad` curves (which have stored stat unc). |
| (d) Envelopes: max(&#124;raw&#124;,&#124;smoothed&#124;) or smoothed + smoothing-systematic | **Reject max-envelope; adopt the quadrature variant** | max keeps raw spikes (does not fix jitter) and doubles selection bias; "smoothed + smoothing-systematic in quadrature" is exactly S4 and is the defensible member of this family. |

---

## 6. Anti-cheating gates (all must pass before any budget use)

- **G1 — A-priori hyperparameters.** `h_z = 1` z-bin, `h_pT = 1` pT-bin, Gaussian kernel, local-linear degree 1, guard threshold 2 sigma, robust-exclusion threshold 50%, MAD inflation 1.4826: all fixed and recorded in the implementation plan *before* any smoothed total is computed. Any later change requires a dated amendment stating the reason. A bandwidth sensitivity scan (x0.5, x2) must be *reported* with the result, not *selected* from it.
- **G2 — Selection isolation.** Any data-driven bandwidth choice uses leave-one-bin-out cross-validation on the *signed shifts of that source alone*, computed before and independently of any combination. The published total is computed exactly once, after freeze. Tuning anything after seeing `total_s3m` (or its plots) is prohibited; if the total looks wrong, the fix is an amendment with a new dated artifact, not a retune.
- **G3 — Toy closure at the measured noise floor.** Inject known smooth signed profiles (constant; pT slope; sign-crossing ramp; cell-dependent pattern; plus one deliberately localized spike) plus noise drawn at the per-source measured `sigma_i` (Gaussian AND a heavy-tailed alternative scaled to the observed tails). Require, per source: (i) median bias of `u` vs true `|s|` within a stated tolerance (e.g. +-20%); (ii) no systematic undershoot: fraction of toy bins with `u < 0.8 |s|` below a stated rate; (iii) explicit zero-crossing test: `u` at injected crossings never falls below `sqrt(v)`; (iv) the localized-spike toy must be *flagged* by the guard more often than the smooth toys' false-flag rate (guard sensitivity check). Training toys and reporting toys are disjoint.
- **G4 — No-shrinkage criterion.** Per bin: `u_i >= sigma_i / sqrt(N_eff,i)` (never below the statistical precision of the shift estimate). Per source: report `median(u_i / |d_i|)` and the change in the quadrature-level total; a reduction of the source's median contribution by more than a stated tolerance (e.g. below 0.8) must be justified as noise removal with the G3 closure, not silently accepted. The deliverable is *calibrated per-bin values*, not a smaller total.
- **G5 — Provenance and no-overwrite.** Raw signed `d_i`, raw `|d_i|`, `m_i`, `v_i`, `sigma_i`, flags, and the audit envelope all persist per bin in sidecar files; smoothing is per source, before combination, never on the total; all outputs go to new dated directories; frozen menu definitions and magnitudes untouched (smoothing re-distributes the per-bin *estimate* of an unchanged source definition; the standing ">50% change -> investigate" trap is enforced by the S3 exclusion + S5 flag).
- **G6 — Convention consistency.** All smoothing operates on the frozen signed per-bin conventions: `Delta_i = (N_i^+ - N_i^-)/2` for paired shifts, `shifted - nominal` per charge for one-sided shifts; `A_h` is derived from varied h+ and h-, never from combined errors; charges smoothed separately with shared hyperparameters; per-source pos/neg correlation reported with every result so A_h cancellation assumptions stay auditable.
- **G7 — No covariance from the smoother.** Smoothed `u_i` are diagonal bookkeeping only. Rank-1 `u u^T` or any smoother-derived correlation is prohibited (standing noise trap); correlations, when needed, come from the P1 replicas or the existing 1000-replica IBU covariance.
- **G8 — Noise-floor characterization first.** For every source, the `sigma_i` model must be characterized *before* smoothing is applied to it (P1/P2 below). A source without a noise model is reported as UNEVALUATED-for-smoothing and keeps its raw values, per the failure-handling rule (missing variation results are reported, never assumed zero or silently smoothed).

---

## 7. Components that must NOT be smoothed, and why

| Component | Reason |
|---|---|
| `lumi_rel` (2.7% flat) | Constant, fully correlated, normalization-only; smoothing is meaningless; excluded from M^h and A_h anyway. |
| `ibu_stat_rel` | It *is* the statistical uncertainty (positive spatial autocorrelation, Section 2.2); smoothing it would conflate statistical and systematic terms and destroy the noise reference scale. |
| Statistical unfolding / response-MC bootstrap terms | Statistical components, not detector responses; the 1000-rep IBU covariance exists for their propagation. |
| Frozen central-tracking prior 1.0198039027% | Constant by construction (diagonal, charge-independent, arXiv:1302.1321 basis); it is a menu magnitude, not a per-bin measurement. (The *measured* `tracking_eff_m0p5_rel` ensemble component IS smoothed per S0-S4; the two must not be conflated.) |
| `phi_h_gauss_rel` | Already smoothed by its own adopted prescription; re-smoothing would double-smooth. If the user later wants it under the S3M scheme, that is a separate re-derivation decision with its own adoption record, not a re-smoothing of the adopted column. |
| Method spread (BBB/IBU/TUnfold) | Coverage diagnostic, not a systematic (frozen menu). |
| Any total column | Provenance rule G5: totals are combined after per-source smoothing, never smoothed themselves. |

---

## 8. Prerequisites and blockers (design-implementation gap list)

- **P1 (required, new Condor production):** replica-based noise floors `sigma_i` for the five one-sided detector shifts and for `ibu_model`/`ibu_reg` differences (bootstrap or event-split, paired nominal+shifted re-unfolds, N ~ 100). Until P1 exists, only the PROVISIONAL MAD fallback may be used, and any smoothed budget carrying it must be labeled PROVISIONAL INTERNAL DIAGNOSTIC.
- **P2 (required, locate or regenerate):** signed `ibu_reg` per-bin values (tau x2 vs tau / 2). The release stores only absolute `reg_unc_nb`.
- **P3 (blocked externally):** `crad_gen_envelope` smoothing is deferred until the C_rad convention decision closes (GenEmpz threshold blocker; `c_rad_applied=false`); the per-bin stat uncertainties it needs are already stored.
- **P4 (affects A_h bookkeeping only):** the charge-dependent tracking convention (0.3%/track vs the 2.5%-on-A_h precedent) is unresolved; S7 keeps the cancellation bookkeeping explicit so this does not contaminate the cross-section budget.
- **Standing context:** the response truth-conservation mismatch (body+miss vs stored RAPGAP prior, +-2.8% in 245/245 bins) affects the *interpretation* of `ibu_model` values; smoothing does not repair it and the caveat must travel with any smoothed `ibu_model` result. (2026-10-06: root-caused; coherent-prior fix validated, central adoption PENDING — see `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`.)

---

## 9. Inspected artifacts (read-only, login node)

- Total table: `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-07-03_with_tracking/total_uncertainty_summary_with_tracking.csv` (490 rows, 10 components).
- Nominal release: `.../output_v6/central/data_release/bbb_ibu_finalcandidate_..._2026-06-10/ibu/cross_sections.json` (490 rows; `model_unc_nb`/`reg_unc_nb` stored absolute; `stat_unc_nb` present).
- Signed per-source comparisons: `.../systematics/hfs/hfs_theta_up/_run/comparison_to_nominal_ibu_hfs_theta_up.csv`; `.../hfs/hfs_jes_down/data_release/.../_run/comparison_to_nominal_ibu_hfs_jes_down.csv`; `.../hfs/hfs_rces_up/data_release/.../_run/comparison_to_nominal_ibu_hfs_rces_up.csv`; `.../electron/elecE_p1pct/data_release/.../_run/comparison_to_nominal_ibu_elecE_p1pct.csv`; `.../electron/elecTheta_p1mrad/data_release/.../_run/comparison_to_nominal_ibu_elecTheta_p1mrad.csv`.
- Tracking ensemble: `.../output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/ensemble_ibu_data4p_v2_20260703/tracking_eff_m0p5_ensemble_systematic.csv` (16 seeds, `seed_rms_nb`, `seed_mean_unc_nb`).
- Model cross-closure: `.../systematics/model_crossclosure/ibu_crossclosure_z005_2026-06-14/ibu_crossclosure_model_uncertainty_z005_2026-06-14_data_scaled.csv` (published `ibu_model_rel` = `current_model_unc_rel`, 490/490).
- C_rad envelope: `.../systematics/sources/crad_gen_envelope_2026-10-02/crad_gen_envelope_490rows.csv`.
- Precedent docs: `docs/systematics/flatphi_unc_smoothing_proposal_2026-10-02.md`; `docs/systematics/flatphi_phih_systematic_adoption_2026-10-02.md`; `docs/systematics/z005_shifted_systematics_pipeline_2026-06-13.md`.
- Analysis scripts used (temporary, not workspace artifacts): `/tmp/opencode/smooth_design_analysis*.py`.

**Bottom line:** the per-bin jitter is not a cosmetic problem with a cosmetic fix — it is the signature of shift measurements whose noise exceeds their signal in 48-100% of variance. The only non-cheating resolution is a noise-calibrated estimator that reports magnitude *with* its resolution (second moment), keeps raw values and flags alongside, fixes every hyperparameter a priori, and proves calibration on toys before touching the budget.

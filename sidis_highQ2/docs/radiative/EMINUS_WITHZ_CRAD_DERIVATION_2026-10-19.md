# e− p Born-target C_rad (WithZ) on the releasebin basis — per-beam re-merge, F-factor algebra correction, and deliverable tables (2026-10-19)

**STATUS: PROVISIONAL DIAGNOSTIC. `c_rad_applied=false`.** Diagnostic only — no central
v6 release, no Born release, no canonical budget, no plotter registration, and no
`STATE.md` blocker row was touched. The APPLIED baseline remains the mixed-beam
releasebin pair (DJANGOH14 applied primary / RAPGAP31 emfix formal alternative,
`data/radiative/crad_release_baseline_2026-10-10/` reference), whose **mixed-beam-only
validity is unchanged**; this record derives what a PURE e− p Born target would require.

## 1. Objective

Close the per-beam e− Born-target gap in the C_rad chain: derive releasebin-basis
per-beam (ep AND em) WithZ primitives by repointing the per-beam re-merge machinery at
the releasebin campaign partials, then compute the per-bin beam-charge factor F(i) and
the deliverable

```
C_rad_e-_WithZ(i) = C_rad_mixed_releasebin(i) * F(i)
```

that takes the current central mixed-beam Born release to the PURE e− p Born cross
section with FULL electroweak content (WithZ samples — NOT the NoZ/C_gamma_born
photon-only variant of `data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/`).

All products live under
`data/radiative/eminus_withz_crad_releasebin_2026-10-19/` (per-beam re-merge under
`perbeam_remerge/{djangoh14,rapgap31}/`, deliverables at the top level).

## 2. F-factor algebra — CORRECTION of the recorded §2 form

Notation per bin i (all sigma are the gen-level cross-section proxies
`sum(weight1*weight2)/L_MC`, releasebin `(Q2esGen, x_es_gen, z, pT)` grid, recomputed
xF<0 both legs, EMPZ>45):

| symbol | definition |
|---|---|
| `nA, nD` | `sigma_nonrad(em)`, `sigma_nonrad(ep)` |
| `rB, rC` | `sigma_rad(em)`, `sigma_rad(ep)` |
| `C_em, C_ep` | `nA/rB`, `nD/rC` (per-beam WithZ C_rad) |
| `R_born` | `nD/nA` |
| `R_rad` | `rC/rB = R_born * C_em/C_ep` (identity) |
| `f` | table-construction beam weights `{ep 0.5140930272, em 0.4859069728}` (superseded 297,657.8 nb⁻¹ accounting) |
| `g` | CURRENT authoritative data split `{ep 0.5440407472973759, em 0.4559592527026241}` (GRL inventory beam fractions, total-invariant; `docs/normalization/beam_lumi_split_current_S67_2026-10-19.md`; L(e+p)=190,958.3023, L(e−p)=160,041.6977 of 351,000.0) |
| `U` | the mixed-beam unfolded data vector actually corrected by the central release: `U = g_em*D_em + g_ep*D_ep` |

The published mixed table is the linear mixture (`mix_by_y` collapses exactly to this;
verified against the published tables to ~4e-16):

```
C_mixed = sigma_n_mix/sigma_r_mix
        = [f_em*nA + f_ep*nD]/[f_em*rB + f_ep*rC]
        = C_em * [f_em + f_ep*R_born]/[f_em + f_ep*R_rad]
```

Target: `sigma_Born,e-(i) = C_em(i) * D_em(i)`. With the MC-modeled data beam ratio at
RAD level (`D_ep/D_em = R_rad`, the §2 machinery's model assumption),

```
U = D_em*(g_em + g_ep*R_rad)  =>  D_em = U/(g_em + g_ep*R_rad)
sigma_Born,e- = C_em * U/(g_em + g_ep*R_rad) = T * U,
T = C_em/(g_em + g_ep*R_rad)
  = C_mixed * F_exact,
F_exact = [f_em + f_ep*R_rad] / ([f_em + f_ep*R_born] * [g_em + g_ep*R_rad])
```

**Correction to the recorded §2 form** (`EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` §2):
the recorded `F1 = 1/[f_em + f_ep*R_born*C_em/C_ep] = 1/[f_em + f_ep*R_rad]` is the exact
factor ONLY when paired with `C_em` AND under the implicit assumption `g = f` (the §2
derivation used the table weights for the data split). Paired with `C_rad_mixed` — the
pairing the deliverable requires — the exact factor is `F_exact` above, which keeps the
f-weights of the table construction AND the true data split g separate. Checks:

* `F_exact` reduces EXACTLY to the recorded `F2 = 1/[f_em + f_ep*R_born]` when `g = f`
  (and `F1` is then the identical total factor via the C_em chain:
  `C_em*F1 = C_mixed*F2 = T`). Verified 2026-10-19 at machine precision (all identities
  ≤ 6.7e-16 on the xFcut per-beam primitives, both generators;
  `/tmp/opencode/eminus_withz_20261019/verify_algebra_xfcut.py`).
* Why g must be the CURRENT split: F converts the ACTUAL mixed data composition — the
  central release corrects `U` with the g-split (54.40% e+/45.60% e−), not the
  superseded f-split (51.41/48.59). Using f for the data side would mis-convert the
  e+ share by ~3 pp of luminosity. The f-weights remain correct on the TABLE side
  (they describe how the published mixed table was constructed).
* Recorded sensitivity variants (all in the deliverable CSVs):
  `F_old = 1/[f_em + f_ep*R_born]` (g=f, pairs with C_mixed),
  `F1_old = 1/[f_em + f_ep*R_rad]` (recorded §2 form; pairs with C_em; g=f implicit),
  `F_Cem_current = 1/[g_em + g_ep*R_rad]` (C_em-chain form: `T = C_em*F_Cem_current`).
* Per-bin identity (gated ≤ 1e-12, measured ≤ 6.4e-16 both generators):
  `T ≡ C_mixed*F_exact ≡ C_em*F_Cem_current`.
* Integrated identity (algebraic, holds exactly for ratio-of-integrals):
  `F_int = T_int/C_mixed_int ≡ [f_em + f_ep*R_rad_int]/([f_em + f_ep*R_born_int]*(g_em + g_ep*R_rad_int))`
  with `R_*_int` formed from integrated sums — the ratio-of-integrals and §2-style
  integrated factors are the SAME number; the §2 recorded integrals differ only because
  they are `F1_old`-style (C_em pairing), not because of the integration route.

### Statistical uncertainties

Four independent legs (nA, nD from the nonrad ep/em samples; rB, rC from the rad ep/em
samples). Campaign convention per leg: per-bin `sigma = sum_y sumw_y/L`,
`var = sum_y sumw2_y/L^2`. Analytic propagation:

* `T = nA/(g_em*rB + g_ep*rC)`:
  `var(T) = var(nA)/s1² + (nA*g_em)²·var(rB)/s1⁴ + (nA*g_ep)²·var(rC)/s1⁴`, `s1 = g_em*rB + g_ep*rC`.
* `F_exact`: 4-leg route through `F = T/C_mixed` (shared legs handled exactly),
  cross-checked against the 2-leg route via `var(R_born)`, `var(R_rad)` — the two
  routes agree to ≤ 8.7e-16 relative (gated ≤ 1e-9).
* Bookkeeping cross-checks vs the published columns (gated ≤ 1e-9, measured ≤ 8e-16):
  my `C_mixed`/`C_rad_stat_unc` reproduce the published mixed CSV, my `C_em`/unc
  reproduce the per-beam em CSV.
* Integrated statistical uncertainties are NOT computed: they require event-level
  covariance across bins, which the per-bin primitives do not carry (campaign
  convention quotes integrated values without stat unc).

## 3. Inputs and provenance

| item | value / path |
|---|---|
| DJ14 partials | `data/radiative/djangoh14_crad_releasebin_2026-10-09/partials/{rad,nonrad_old,nonrad_new}/{ep,em}/` (counts 731/642, 260/260, 530/530) |
| RG31 partials | `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/partials/{rad,nonrad}/{ep,em}/` (counts 691/582, 256/248; byte-identical copies of the original releasebin partials, md5-spot-verified 2026-10-19) |
| Exposures (audited, `CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md`) | DJ14 RAD ep 16,206,393.0 / em 13,534,017.0; DJ14 NONRAD old 12,920,115.855/12,841,572.442 + new 1,717,376.821/1,699,702.771 = combined 14,637,492.676/14,541,275.213. RG31 RAD ep 17,536,474.0 / em 14,461,067.0; RG31 NONRAD ep 13,954,768.0 / em **13,470,764.7 (EMFIX-REPAIRED; never the pre-repair 13,416,991.0)** |
| Published mixed tables | DJ14 `djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (h+ 1.1415172920054786 / h− 1.1410367377289135); RG31 emfix `rapgap_crad_releasebin_emfix_2026-10-09/...` (h+ 1.1342370401351234 / h− 1.1336689310022947) |
| Superseded original (RG31 pre-repair gate) | `rapgap_crad_releasebin_2026-10-09/` (h+ 1.1364508941898332 / h− 1.1358814928541603) |

**Provenance finding (emfix finalize.json is a LAYERED RECORD):** the emfix
`finalize.json` carries the PRE-REPAIR em exposure in `luminosity_nb_inv` and the
pre-repair integrated values in its `values` block; the corrected exposure lives in
`provenance_fix` (`L_nonrad_em_old 13,416,991.0 + missing 53,773.7 = corrected
13,470,764.7`, gated) and the corrected integrated values in
`validation.integrated.corrected`; its `output_file` points at the ORIGINAL (superseded)
table. The emfix `campaign_manifest.json` carries the corrected `luminosity_nb_inv`
verbatim. The first RG31 re-merge attempt (cluster **5785256**) failed its exposure
cross-check by reading `luminosity_nb_inv` directly; the fork was fixed to the
layered-record convention (old+missing==corrected gated; `values` block tied to the
original releasebin JSON) and resubmitted.

## 4. Execution

* **Fork scripts** (label/glob/reference repointing of the xFcut per-beam finalizers,
  clusters 5385358/5385359 precedent, plus the emfix-layered-record gates for RG31):
  `scripts/crad_v3_finalize_releasebin_perbeam_{djangoh,rapgap}.py`. Hard in-job gates:
  audited exposures cross-checked against campaign summary/manifest/finalize JSONs;
  hard file-count gates vs the campaign manifests (exact-exposure bookkeeping);
  DJ14 low-y closure vs the releasebin summary `prebeam_mixing_low_y_closure`
  (rel tol 1e-9); mixed reconstruction vs the PUBLISHED mixed table (rel tol 1e-9);
  RG31 second gate — pre-repair em exposure vs the SUPERSEDED original table
  (validates the repair delta end-to-end); 490/490 finite per beam.
* **Single-file smokes** (login-node legal): PASS both, degenerate constants exact —
  DJ14 em `RAD/NONRAD = 13,534,017.0/14,541,275.213 = 0.930731094883652` (analytic max
  rel dev 3.6e-16); RG31 em `14,461,067.0/13,470,764.7 = 1.0735149282208158` (4.1e-16).
  Records: `smoke/{djangoh14,rapgap31}/smoke_singlefile_*.json`.
* **Condor re-merge** (vanilla, 1 core, 4 GB, +RequestRuntime 3600,
  retry_until=(ExitCode!=127)): DJ14 cluster **5785255** PASS (61 s);
  RG31 cluster **5785378** PASS (attempt 2; first attempt 5785256 FAILED on the
  layered-record exposure read, fixed, see §3). In-job verdicts: mixed reconstruction
  bitwise (DJ14 max rel dev 0.0; RG31 vs emfix ≤ 7.6e-16 and vs the superseded original
  ≤ 7.7e-16), integrated vs baseline ≤ 2e-16, 490/490 finite per beam both.
* **Derivation** (login node, reads merged npz + per-beam CSVs + published mixed CSVs;
  does NOT trust the job verdicts — everything recomputed):
  `scripts/compute_eminus_withz_crad_releasebin.py`. Verdict **PASS** both generators.

## 5. Validation summary (derivation-level, independent of the job verdicts)

| check | DJANGOH14 | RAPGAP31 |
|---|---|---|
| finite 490/490 (C_em, C_ep, C_mixed, T, F_exact) | PASS | PASS |
| mixed reconstruction vs published (max rel) | 9.7e-16 | 2.0e-16 |
| identities `T≡C_mixed·F_exact`, `T≡C_em·F_Cem_current`, `R_rad·C_ep≡R_born·C_em` | ≤ 6.3e-16 | ≤ 6.4e-16 |
| F-unc two propagation routes | 8.5e-16 | 8.6e-16 |
| stat-unc bookkeeping vs published columns | ≤ 8e-16 | ≤ 8e-16 |
| T per-bin band [0.4, 1.8] | 490/490 | 490/490 |
| F_exact per-bin soft band [0.5, 1.5] (WARN-only) | 490/490, no warn | 490/490, no warn |
| exposures vs job summary (vs campaign files in-job) | PASS | PASS |

## 6. Results

### Integrated (ratio-of-integrals; h+ = bins 0–244, h− = 245–489)

| quantity | DJ14 h+ | DJ14 h− | RG31 h+ | RG31 h− |
|---|---|---|---|---|
| C_em_int (e− WithZ per-beam) | 1.1417294103 | 1.1412810054 | 1.1299783214 | 1.1294249000 |
| C_ep_int (e+ WithZ per-beam) | 1.1413148193 | 1.1408038614 | 1.1383097516 | 1.1377268502 |
| C_mixed_int (published) | 1.1415172920 | 1.1410367377 | 1.1342370401 | 1.1336689310 |
| **T_int = C_rad_e−_WithZ (deliverable)** | **1.1478488555** | **1.1466412341** | **1.1371916091** | **1.1365215812** |
| **F_int − 1 (primary integrated F−1)** | **+0.5547%** | **+0.4912%** | **+0.2605%** | **+0.2516%** |
| F1_old_int − 1 (§2 comparator) | +0.5063% | +0.4437% | +0.6030% | +0.5936% |
| recorded §2 (xFcut, F1_old) | +0.507% | +0.444% | +0.603% | +0.593% |
| F_old_int − 1 (g=f sensitivity) | +0.5250% | +0.4652% | +0.2253% | +0.2170% |
| F_Cem_current_int − 1 | +0.5360% | +0.4697% | +0.6384% | +0.6283% |
| R_born_int / R_rad_int | 0.989841 / 0.990201 | 0.990993 / 0.991407 | 0.995628 / 0.988341 | 0.995789 / 0.988523 |
| split shift F_exact_int − F_old_int (old→current data split) | +0.0297 pp | +0.0260 pp | +0.0352 pp | +0.0347 pp |

The releasebin `F1_old_int` reproduces the recorded §2 xFcut values to <0.001 pp —
expected, since the integrated ratio-of-sums is binning-independent (the releasebin
rederivation moved events between bins, never changed the fiducial total).

### Per-bin ranges

| quantity | DJ14 h+ | DJ14 h− | RG31 h+ | RG31 h− |
|---|---|---|---|---|
| T min / max | 0.8465 / 1.6386 | 0.8970 / 1.6189 | 0.8570 / 1.6284 | 0.8922 / 1.5321 |
| F_exact min / max | 0.9713 / 1.1001 | 0.9777 / 1.0958 | 0.9680 / 1.1117 | 0.9449 / 1.1464 |
| F_exact − 1 median | +0.761% | +0.398% | +0.131% | +0.293% |
| F_exact − F_old median / max | 0.052 / 0.643 pp | 0.034 / 0.508 pp | 0.036 / 0.620 pp | 0.043 / 0.773 pp |
| T rel stat unc median | 0.512% | 0.542% | 0.544% | 0.581% |

### Generator comparison (DJANGOH14 vs RAPGAP31, on T)

| metric | h+ | h− |
|---|---|---|
| integrated T(DJ) − T(RG) | +0.937% | +0.890% |
| per-bin rel diff median / p95 / max | 1.19% / 6.45% / 16.45% | 1.23% / 5.41% / 16.70% |
| C_em rel diff median / max | 1.32% / 23.11% | 1.40% / 13.98% |

Direction note: for DJ14 `C_em ≈ C_ep ≈ C_mixed` (R_born ≈ R_rad ≈ 0.990) and the F
shift is dominated by the unmixeding; for RG31 `C_ep > C_em` (R_rad_int 0.9883 <
R_born_int 0.9956), the mixed table sits ABOVE the e− per-beam correction, and the
total e− factor T_int (1.1372) still exceeds C_mixed_int (1.1342) because the data
unmixing (g-side) outweighs the table side.

## 7. Caveats

1. **Model assumption:** the decomposition uses the MC-modeled data beam ratio at RAD
   level, `D_ep/D_em = R_rad` (per bin). Any data/MC disagreement in the e+/e− ratio at
   RAD level would enter F directly. This is inherited from the §2 machinery, now with
   the correct data split.
2. **RG31 two-valued σ̂ residual** (audit Step 4, NOT a file-set mismatch): per-chunk
   sigma-hat {7287.915, 7323.939 pb} bookkeeping, bounded **−0.44%…+0.26%** per-beam
   C_rad — carried verbatim to the RG31 products here; locally unresolvable.
3. **Exposures are FILE-SET-RECONCILED (generator-log-open)**; the generator-log ceiling
   is permanently blocked externally. C_rad (mixed, per-beam, and this e− target) is NOT
   publication-grade.
4. **PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`**: nothing here is applied to any
   central product; the applied baseline and its mixed-beam-only validity are
   unchanged. Applying an e− Born target would be a new user-gated decision with its
   own promotion record.
5. **No integrated statistical uncertainties** (see §2); per-bin statistical
   uncertainties only. Generator/model uncertainties are NOT assigned here — the
   DJ14/RG31 comparison numbers above are the raw material for any future budget
   discussion (owned by `sidis-hq2-systematics`; not modified by this record).
6. **EMPZ>45 convention** (CLOSED 2026-10-09) and the recomputed-xF<0 hemisphere
   convention are inherited unchanged on both legs.
7. The RG31 deliverable uses the emfix-repaired exposure; the pre-repair value is used
   ONLY inside the in-job end-to-end repair validation against the superseded original
   table.

## 8. File inventory (all under `data/radiative/eminus_withz_crad_releasebin_2026-10-19/` unless noted)

| file | content |
|---|---|
| `crad_eminus_withz_djangoh14_releasebin_490rows.csv` | DJ14 deliverable, 490 rows × 33 cols (T, F_exact + variants, per-beam C/σ/R, stat uncs, finite flags) |
| `crad_eminus_withz_rapgap31_releasebin_490rows.csv` | RG31 emfix deliverable, same schema |
| `crad_eminus_withz_releasebin_validation.json` | full validation record (algebra, gates, integrated blocks, generator comparison, §2 comparators) |
| `crad_eminus_withz_releasebin.json` | output-contract JSON (step/channel/status/bins/values/errors/validation) |
| `manifest.json` | sha256 manifests of 18 inputs + 4 outputs, cluster IDs, smoke records, labels |
| `perbeam_remerge/djangoh14/`, `perbeam_remerge/rapgap31/` | per-beam merged npz primitives, per-beam 490/2940-row CSVs, job summaries, exposure-provenance copies |
| `scripts/` | both forks, both Condor wrappers + .sub, the derivation script |
| `smoke/{djangoh14,rapgap31}/` | single-file smoke records |
| `logs/` | Condor logs (5785255, 5785256 failed attempt, 5785378) and env logs |
| `sidis_highQ2/docs/radiative/EMINUS_WITHZ_CRAD_DERIVATION_2026-10-19.md` | this record |

**Cluster IDs:** DJ14 re-merge 5785255 (PASS); RG31 re-merge 5785378 (PASS, attempt 2);
RG31 first attempt 5785256 (FAILED, exposure cross-check read the layered-record
finalize.json `luminosity_nb_inv` instead of `provenance_fix`; fork fixed, no output
written by the failed attempt).

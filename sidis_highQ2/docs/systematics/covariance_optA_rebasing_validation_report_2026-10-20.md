# Covariance re-basing on the OPT-A nominal — wave validation report (quoting condition (d))

**Date:** 2026-10-20 (wave), report finalized 2026-10-21
**Channel:** `sidis_highQ2_chargedhadron_highQ2_z005_covariance_optA`
**Deliverable:** `H1ChargedSidis_HighQ2/data/covariance/z005_covariance_optA_2026-10-20/` (76 files)
**Builder:** `H1ChargedSidis_HighQ2/scripts/systematics/covariance_optA_2026-10-20/s09_build_covariance_package_optA_2026-10-20.py` (dated clone; frozen 2026-10-17 chain and frozen 2026-10-19 package untouched, sha-swept as protected)
**Status:** S9 **PASS** — 9/9 gates + ga10 guard, cluster **5803902**, wall 48.5 s, blockers none.
**Review target:** two-critic review (research/implementation) of this report and the package.

---

## 1. What this wave is

The covariance re-basing of the frozen 2026-10-19 clean-release package
(`z005_covariance_cleanrelease_2026-10-19/`) onto the **OPT-A nominal**
(quoting condition (d)): diagonals on the NEW c1fix/quoted budget totals,
the defective stored env100 covariance purged from the chain, the straddle
consumed as a special block from the c1fix RECORD, m2 entered as a
justified zero, and C_stat/C_MCstat re-derived on the OPT-A chain.

The public per-bin systematic quote remains the c1fix budget
`total_quoted_rel` (medians h+ 7.3121% / h- 7.6243% = hypot(c1fix total,
straddle); m2 a documented zero; `bgsub_stat` RETIRED audit-only) —
**untouched**. This package is the separate deliverable for covariance
consumers (global fitters). Central values inheriting into the package
remain PROVISIONAL (loop-3); quoting remains BLOCKED (§9).

## 2. Input basis and stage chronology (all clusters)

| Stage | Product / location | Cluster(s) | Status |
|---|---|---|---|
| c1fix budget (env100 C1-fix override) | `output_v6/central/data_release/systematics_bgsub_env100_optA_c1fix_2026-10-20/` | 5803354 | PASS (prior wave) |
| S2b joint event-cluster toys (RE-RUN, OPT-A basis) | `output_v6/studies/covariance_optA_2026-10-20/joint_stat_toys/` | 5803764 smoke / 5803774 main | COMPLETE — cross-block MEASURED on the OPT-A basis (seeds 32345 main / 42345 stability) |
| S3 response MC-stat toys (RE-RUN, OPT-A chain) | `.../response_mcstat_toys/` (cov_mcstat.json = AUTHORITATIVE) | 5803765 smoke / 5803775 K=200 / 5803776 K=100 repeat | PASS (seed 52345, fakecol slice 62345) |
| S8 symmetrize + re-base | `.../s08_symmetrize/` | 5803832 | PASS 11/11 (after post-mortems §7.1–7.2) |
| S9 covariance package (this deliverable) | `data/covariance/z005_covariance_optA_2026-10-20/` | 5803902 | **PASS** 9/9 + ga10 |
| S1 signed deltas, S4 shard bootstrap, S6, S7 truth reweight | frozen 2026-10-17 trees | — | FROZEN, consumed read-only (S7 seed-ledger gap carried per A8) |

All 40 s09 inputs are sha-pinned in the builder PIN dict and verified at
run start; 24 PIN entries + the frozen S1 CSV (record_input) are carried in
`s09_record.json`.

## 3. S9 run summary (cluster 5803902)

- **Destination guard:** clean DEST enforced (two failed attempts each left
  a partial DEST; both were wiped before resubmission — failed-run outputs
  only, listings verified before removal).
- **Protected-tree sha sweep (G5):** 20 protected trees + 9 pinned files;
  pre 16695 → post 16767 files hashed; **drifted 0, missing 0, extras
  outside DEST 0**, 72 new files inside DEST.
- **Arithmetic environment:** LCG_107a python3 (numpy 1.26.4) with
  OPENBLAS/OMP/MKL/BLIS_NUM_THREADS=1 via `condor/run_s09.sh` (required for
  the borderline native-units PSD gates; §7.1).
- **Inventory:** 76 files — 56 matrix npy (components + rel twins),
  2 bootstrap tables, bin_map.csv, delta_k_columns.csv, lineage/ (11),
  unpropagated_sources.json, README.md, validation.json, manifest.json,
  s09_record.json; 40 pinned inputs.

## 4. Gates (validation.json, schema `covariance_opta_s09_validation_v1`)

`all_pass: true`, `all_pass_including_guards: true`.

| Gate | Key numbers |
|---|---|
| g1 psd_rank_all_matrices | 25/25 matrices PSD in native units; most-negative min_eig −2.730e-06 |
| g2 diagonal_consistency_vs_budget | **unattributed 0/0**; resid_max 4.44e-16; per-source max dev 3.74e-16 |
| g3 denominator_alignment | labels 0 mismatch; axis dev 0.0; nominal 1.6e-15; anchors 1.78e-15; yield 1.7e-16; LUMI 351000 |
| g4 correlation_twin | max abs rho 1.000000; diag dev 2.2e-16 |
| g5 sha_sweep_protected_unchanged | pre 16695 / post 16767; drifted 0; missing 0; extras_outside 0; new_in_dest 72 |
| g6 manifest_enforcement_a6 | rank1 11; diag consume 8/10 (2 consume=false); excluded 12; bitwise coincidences 0; reconstruction exact; no-lumi identity 1.7e-18 ≤ 4.1e-14 |
| g7 total_vs_stat_guidance | ratio total/stat 1.959 (h+) / 1.834 (h−) |
| g8 near_singularity_report | C_total min_eig 1.468e-11 nb², cond 1.344e+10, numerical rank 490, top10 frac 0.9985, rank-1 partial rank 10 |
| g9 eigenfloor_twin | eps 2.236e-05/1.973e-05 counts²; max rel diag dev vs primary 4.35e-05/6.76e-05 (bound 1e-02) |

**ga10 cross-term duplication guard (PASS, reported alongside, not counted
in the 9):** fp-signature sets EXACTLY {`mag_ibu_stat_rel`} vs C_stat and
{} vs C_MCstat (criterion: max rel dev ≤ 1e-12); `mag_fakecol_mcstat`
consume=FALSE containment (A10); duplicated-variance fractions med
13.2659% (h+) / 12.9635% (h−) computed and recorded; S3 slice-closure
ratios 1.0011/1.0001 (|1−r| ≤ 0.2); pairwise bitwise coincidences among
consumed columns 0; fake-cell census 472/487 pos, 460/484 neg (matches the
S3 product verbatim).

## 5. Condition-(d) composition and identities

- **C_total with lumi = 21 terms** (no-lumi 20): C_stat (OPT-A sidecar
  marginals, seeds 12345/22345 + A1 re-based cross-block from the S2b
  re-run) + C_MCstat (S3 verbatim, K=200, rank 199/245 per charge,
  toy-limited by construction; eigen-floor twin per the pre-registered
  policy; fixed low-Neff cells 12,207/11,834 body per A5) + 11 rank-1
  (10 non-lumi + lumi; **env100 NEW**) + **cov_straddle (NEW special
  block)** + 7 diagonal-only terms.
- **G2 identity (machine-checked, 0 unattributed bins):** diag(C_total)
  equals the QUOTED budget quadrature with the fakecol column removed and
  the five named S8 convention causes (hfs_jes D1-sym; hfs_rces env;
  hfs_theta env; elec_energy sym-vs-env; elec_theta smoothed) + the
  C_MCstat addition + the detint split term.
- **cov_straddle:** consumed from the c1fix straddle RECORD
  (`records/straddle_residual_rel.json`, sha-pinned) with its documented
  correlation model — fully correlated h+/h− within a gen bin, uncorrelated
  bin-to-bin and vs other sources, documented partial overlap with
  ibu_model kept conservative; zeroed in the 3 empty-D bins. Record vector
  == S8 CSV custody twin == budget column, bitwise over 490. Medians
  h+ 0.1285% / h− 0.1245%; max 4.33% (h− bin 209).
- **m2 z-shift:** JUSTIFIED ZERO per the c1fix TREATMENT DECISIONS §3
  (declared nominal-beam bin-coordinate convention); no matrix, no term;
  documented in the unpropagated register.
- **bgsub_stat:** RETIRED from the c1fix quoted totals (C1 detint
  double-count, CROSS_AREA SYS-C1); register entry RETIRED_FROM_C1FIX_
  QUOTED_TOTALS.
- **Empty-D exemption re-flag:** the 3 empty-D bins (pos/109, pos/179,
  neg/240) are exempt from the 100% cap in every lineage total; the ONLY
  quoted bin above 100% is h+ bin 179 at **110.5935%**
  (`budget_total_quoted_rel_reference[179] = 1.105935002740245` in
  s09_record); no populated bin exceeds 100% (S8 gate
  `g_quoted_over100_emptyd_only` re-flagged).
- **Shrinkage:** the re-based C_stat cross-block needed the
  pre-registered fallback (i): scalar cross-block shrinkage,
  **alpha = 0.2222835111081017**, until PSD.
- **Headline:** package totals (median sqrt(diag)/sigma) 7.2480% (h+) /
  7.4952% (h−) vs the public quote 7.3121%/7.6243% (difference = the named
  convention causes + C_MCstat − dropped fakecol; gate G2). Dominant
  non-lumi systematic: env100.

## 6. Seed ledger (manifest.json `seeds`)

| Stage | Seed(s) | Status |
|---|---|---|
| Sidecar C_stat marginals | 12345 (h+), 22345 (h−) | re-derived in the OPT-A promotion wave (D4/TIER_A) |
| S2b joint event-cluster toys | 32345 main, 42345 200-toy stability | RE-RUN 2026-10-20 (OPT-A basis) |
| S3 response replicas | 52345 (K=200 main; nested K=20/K=100), 62345 fakecol slice | RE-RUN 2026-10-20 (OPT-A chain) |
| S4 shard bootstrap | 72345 (rank-1 legs), 82345 (extended, ibu_model/ibu_reg) | FROZEN, not re-run |
| S7 truth reweight | NOT RECORDED | seed-ledger gap carried as MISSING EVIDENCE per A8; S7 frozen |

## 7. Post-mortems (full engineering record)

### 7.1 BLAS/threading (s08 clusters 5803803, 5803821)
**Failure:** G-gate flips on the borderline native-units C_stat PSD check
(min_eig/scale ≈ −1.0e-12, one effectively-null direction) across Condor
slots. **Root cause:** the PSD verdict is stack- and thread-sensitive;
the default slot environment did not reproduce the gate environment of the
c1fix budget builder. **Fix:** run under the LCG_107a python3 view with
OPENBLAS/OMP/MKL/BLIS_NUM_THREADS=1 (wrapper `run_s08.sh`; the same
pattern was pre-emptively applied to `run_s09.sh`). **Verification:** s08
clean PASS 11/11 (5803832); s09 PSD gates PASS in the same environment.

### 7.2 s08 record-md `%c` TypeError (cluster 5803830)
Deferred `%`-formatting crash in the s08 record markdown writer (a literal
percent in a `%`-format string). Fixed in the s08 clone; the s09 clone was
audited for the same class before submission (%-format audit clean; no
deferred formatting).

### 7.3 s09 units bug in the clone-added authoritative-diagonal check (cluster 5803888)
**Failure:** `FATAL: S3 C_MCstat diagonal vs authoritative diag_rel_list
drift (pos): 1.109e+05` at ~46 s (after the pre-sweep). **Root cause:** a
clone-added check (absent from the frozen original — provenance confirmed
by grep against `covariance_cleanrelease_2026-10-17/s09_build_covariance_
package.py`) computed `_mc_diag_rel = sqrt(diag(C_mcstat_counts)) / sigma`,
but `C_mcstat_counts` is in counts² (counts = sigma × LUMI), so the
comparison against the RELATIVE `diag_rel_list` was inflated by LUMI =
351000: 0.316033 (pos diag_rel max) × 351000 = 1.109e+05, the observed
drift exactly. The G2 `mc_dev` check on the nb² matrix is correct and was
left unchanged. **Fix:** divide by `(sigma * LUMI)`. **Verification:**
login-node pre-flight against the authoritative S3 record before
resubmission — max dev 5.551e-17 (h+) / 1.388e-17 (h−), median dev 0.0;
in-run G2 per-source mcstat dev ≤ 1e-12 (gate PASS).

### 7.4 s09 missing straddle custody twin in `s8_cols` (cluster 5803894)
**Failure:** `KeyError: 'delta_straddle_residual_rel'` at the cov_straddle
custody check (the run had already written C_stat, C_mcstat, the
eigen-floor and all rank-1/diagonal matrices — the §7.3 fix confirmed
working in-run). **Root cause:** `s8_cols` was built from the S8 manifest
lists (rank-1 + diagonal + excluded) only; the straddle twin is a
SPECIAL-BLOCK column (matrix consumed from the RECORD, twin is custody
only), so it was never loaded — the same KeyError would have recurred in
the G2 quadrature term and in `chk_source`. **Fix:** `SPECIAL_BLOCK_TWINS`
appended to the `s8_cols` construction (the other two special-block twins,
`mag_ibu_stat_rel`/`mag_fakecol_mcstat_rel`, are already covered by the
diagonal manifest lists). **Verification (static, exhaustive):** three AST
audits of the whole builder — (1) every constant column reference vs the
owning CSV header; (2) every constant subscript of `s8_cols`/`s8col`/
`budcol` vs the actual runtime key set (34 / 23 keys); (3) dynamic
`_rel`→`_nb` twins for all 19 consumed columns, bin_map/release columns,
and 65 constant-key JSON paths vs the loaded records. All PASS.

### 7.5 G5 self-log drift hazard (fixed before it could fire)
The s09 protected trees include the scripts tree `HERE` (as in the frozen
original). With condor logs written to `HERE/condor/logs/`, the job's own
`.out`/`.log` files grow during the run and are hashed by both the pre-
and post-sweeps — a guaranteed G5 drift even with a clean DEST. The frozen
wave never faced this because the s09-family runs kept logs outside the
cleanrelease tree (no s09 logs exist under its condor dir; the frozen
validation's DEST-internal drift is the s09b in-place-amendment record,
whose PA1 policy pre-declares the package-internal change set).
**Fix:** redirect the submission logs to a non-protected studies dir
(`output_v6/studies/covariance_optA_2026-10-20/condor_logs_s09/`); the G5
gate code stays byte-faithful to the frozen original. **Verification:** G5
PASS with drifted 0 / extras_outside 0 (cluster 5803902).

**Process note (both s09 failures):** the destination guard means a
partial DEST from a failed attempt MUST be wiped before resubmission;
both wipes were listed and verified as failed-run outputs only. Both
failures were clone-side defects in never-executed code paths, not data
or prior-stage drift; all prior-stage identities they guarded were
re-verified after the fixes.

## 8. Unpropagated-source register (package `unpropagated_sources.json`)

- `prior_reweight_truthreweight` (S7): REPORT_ONLY_UNPROPAGATED_FROZEN_
  OLD_BASIS (A4; not a budget component; ibu_model unchanged).
- Response body/miss MC-stat (audit row 20, formerly the single GAP):
  **CLOSED_BY_THIS_PACKAGE** — propagated as C_MCstat (S3 re-run; supersedes
  the dossier bounded estimate medians 1.4769%/1.5752% with the measured
  re-based medians 0.8548% h+ / 0.9252% h−).
- `m2_zshift`: DOCUMENTED_JUSTIFIED_ZERO (c1fix TREATMENT DECISIONS §3).
- `bgsub_stat`: RETIRED_FROM_C1FIX_QUOTED_TOTALS (C1 detint double-count,
  proven 3×, ≤ ~171 ppm, strictly conservative).
- Carried open (unchanged statuses): trigger efficiency
  (UNEVALUATED_PLACEHOLDER), DIFFVM exclusive rho/photoproduction
  (UNEVALUATED_BLOCKED), e_cap_V0 overlap (IGNORED_BY_USER_DECISION
  2026-10-08), NucliaWeight r_min proxy / lowQ2 alpha≥0.2 rebuild /
  DjBkgd exact-exposure provenance (BOUNDED_CAVEAT_OPEN), b1 aggregation
  (FAIL_CARRIED_REPORT_ONLY; the machine check b1_floor_mapping vs
  canonical budget dev=0 re-asserted PASS), charge-dependent tracking
  convention (OPEN_CAVEAT per D7).

## 9. Status after this package

**Quoting remains BLOCKED.** Still open: M1 env100 builder reconstruction,
N2 HFS-Empz channel, plotter band re-pointing to the new package, and the
analysis-note update. This package closes ONLY the "covariance re-basing
on the OPT-A nominal" item. Consumers: read the package README first
(mixed-form superset; never collapse forms; never quote package diagonals
as the public per-bin systematic; do not re-add cov_fakecol_mcstat;
C_MCstat cross-charge block zero by D5, C_stat cross-block measured with
shrinkage alpha = 0.2222835111081017, cov_straddle cross-charge fully
correlated).

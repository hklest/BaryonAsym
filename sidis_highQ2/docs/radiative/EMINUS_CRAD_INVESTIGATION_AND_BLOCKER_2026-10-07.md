# e- p Born-level C_rad target — investigation, per-beam re-merge, quantification, and NoZ photon-Born C_gamma_born design (2026-10-07)

**STATUS: PROVISIONAL DIAGNOSTIC record — RESOLVED 2026-10-07 (third session). `c_rad_applied=false`.**
Diagnostic only. No central v6 product, no baseline table, and no canonical plot package was
touched. **Task B (per-beam e- C_rad tables) and Task C (e- plot package) are now COMPLETE and
independently verified (Sections 3b/4):** per-beam 490/490-finite tables under
`data/radiative/{rapgap,djangoh14}_eminus_crad_final_xfcut_2026-10-07/`, exact quantification in
Section 2, and the e- plot package
`plots/radiative/crad_v4_xfcut_eminus_djangoh_vs_rapgap_7col_2026-10-07/`.
The current C_rad RELEASE baseline remains the mixed-beam xFGen<0 tables of 2026-10-05/06.

## 1. Machinery: do per-beam (e-) merged primitives exist?

**No.** For the baseline xFcut campaign (recomputed xF<0 on both legs, EMPZ>45, nominal,
`empz_gt45_nominal`), per-beam per-bin `sumw/sumw2/n` primitives exist **only as per-file npz
partials**; no merged (file-summed) per-beam artifact was ever written:

- Finalizers `crad_v3_finalize_xfcut_{rapgap,djangoh}.py` sum per-beam pools in memory
  (`add()` over per-file npz), beam-mix (`mix_by_y`), and write **mixed-beam CSVs only**.
- Campaign validation (`crad_rapgap31_xfcut.json` G5) documents partial integrity per file
  (RAPGAP 1777/1777, DJANGOH 2953/2953 bit-identity clean) — nothing per-beam merged on disk.
- Only per-beam numbers in the baseline record: the `prebeam_mixing_low_y_closure` control
  (low-y slice, charge-integrated) in each `crad_v3_summary.json`.

Per-file npz partials (keys `sumw,sumw2,n` (490,) + `sumw_y,sumw2_y,n_y` (6,490) + `diag`;
schema verified on a single file on the login node):

| Generator | Pool (beam em) | Files | Path |
|---|---|---|---|
| RAPGAP31 | rad em | 582 | `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/rapgap_hist_xfcut_20261005/partials/rad/em/rapgap_rad_em_????_empz_gt45_nominal.npz` |
| RAPGAP31 | nonrad em (OLD pool only) | 248 | `.../rapgap_hist_xfcut_20261005/partials/nonrad/em/rapgap_nonrad_em_????_empz_gt45_nominal.npz` |
| DJANGOH14 | rad em | 642 | `.../djangoh_hist_xfcut_20261005/partials/rad/em/rad_em_0731..1372_empz_gt45_nominal.npz` |
| DJANGOH14 | nonrad_old em | 260 | `.../djangoh_hist_xfcut_20261005/partials/nonrad_old/em/old_nonrad_em_????_empz_gt45_nominal.npz` |
| DJANGOH14 | nonrad_new em | 530 | `.../djangoh_hist_xfcut_20261005/partials/nonrad_new/em/nonrad_new_em_????_empz_gt45_nominal.npz` |

e- merge scale: 830 files (RAPGAP31) + 1432 files (DJANGOH14). ep partials sit in the same trees
(RAPGAP 691+256; DJANGOH 731+260+530).

Related legacy e- efforts (neither provides a baseline-definition table):

- `data/radiative/britzger_eminus_crad_xf_y_eta_q2_2026-09-26/` — **top-level
  `britzger_eminus_crad_490rows.csv` is INVALID** (734-row recovery subset; see its
  `README_INVALID_734ROW_RECOVERY.md`). The repaired **`complete_2953/`** table is DJANGOH-only,
  e- target, 471/490 finite, but **different definition** (stored `xFGen<0` with the
  nominal-beam-q ISR bug, `abs(GenEta_lab)<4`, no GenEmpz cut) and diagnostic exposures.
  It does carry merged per-beam per-bin sumw columns (`rad_ep_sumw`, `rad_em_sumw`,
  `nonrad_old/new_{ep,em}_sumw`) — used below for indicative quantification only.
- `data/radiative/britzger_eminus_crad_v6exact_empz45_2026-09-27/` — baseline-style selection
  (GenEmpz>45, recomputed v6 z/pT_h/xF, xF<0), DJANGOH-only, 2953 per-file JSON partials on disk,
  but the **finalize was never run** (no finalize logs; monitor `MONITOR_TIMEOUT_120_POLLS`);
  no table, no plot. Incomplete; superseded in spirit by the 2026-10-05 xFcut campaign partials.

## 2. Physics: what correcting the MIXED data to the e- p Born level requires

Per bin i, beam b: `sigma_rad_b = sumw_rad_b/L_rad_b`, `sigma_nonrad_b = sumw_nonrad_b/L_nonrad_b`,
`C_b = sigma_nonrad_b/sigma_rad_b`; mixture `f_ep=0.5140930272`, `f_em=0.4859069728`;
`C_mixed = (f_ep sigma_nonrad_ep + f_em sigma_nonrad_em)/(f_ep sigma_rad_ep + f_em sigma_rad_em)`.
Mixture algebra verified exactly against the campaign y-resolved CSVs (low-y slice reproduced to
1e-9 from the per-beam closure sigmas).

Applying `C_em` to the mixed data gives
`C_em * D_mixed = f_ep*sigma_born_ep*(C_em/C_ep) + f_em*sigma_born_em`
= `sigma_born_em * [f_em + f_ep * R_born * (C_em/C_ep)]`, with
`R_born = sigma_born_ep/sigma_born_em`. This equals the e- p Born target **only if**
`R_born*(C_em/C_ep) = f_em/f_ep`, which is not satisfied: at Q2>150 GeV^2 gamma-Z interference
flips sign between e+ p and e- p, so `sigma_born_ep != sigma_born_em` (sub-percent here, growing
with Q2/M_Z^2). An **exact e- target therefore needs an additional per-bin beam-charge factor**
`F(i) = 1/[f_em + f_ep*R_born(i)*C_em(i)/C_ep(i)]` (equivalently: correct mixed->mixed-Born with
`C_mixed`, then multiply by `1/[f_em + f_ep*R_born(i)]`), **or** a per-beam data split (apply
`C_rad(ep)` to the e+ data slice and `C_rad(em)` to the e- slice; requires a new per-beam data
production, not available from the current mixed release). Existing samples CAN provide
`R_born(i)`: the same campaign's per-beam nonrad partials (ep and em, both generators) give
`sigma_born_ep/sigma_born_em` per bin after the blocked merge — with the caveat that it is a
generator-model statement with diagnostic-grade nonrad exposures.

### Quantification from existing merged artifacts

Exact baseline selection, low-y [0.1,0.2) control slice (charge-integrated, from the campaign
summary JSONs; the ONLY per-beam measurement under the baseline definition):

| Quantity | RAPGAP31 | DJANGOH14 |
|---|---|---|
| C_rad(ep), low-y | 1.1645205958 | 1.1666510324 |
| C_rad(em), low-y | 1.1598444505 | 1.1673636855 |
| (C_em−C_ep)/C_ep | **−0.4016%** | **+0.0611%** |
| C_mixed, low-y (CSV-verified) | 1.1622386902 | 1.1669981960 |
| (C_em−C_mixed)/C_mixed | **−0.2060%** | **+0.0313%** |
| sigma_born em/ep, low-y | +0.4318% | +0.5572% |
| sigma_rad em/ep, low-y | +0.8367% | +0.4958% |
| w_ep (ep share of mixed sigma_rad) | 0.5120 | 0.5129 |
| undershoot of `C_em*D_mixed` vs sigma_born_em | **−0.4266%** | **−0.2536%** |

So at the only exact-selection control available, correcting the mixed data with `C_rad(e-)`
**undershoots the true e- p Born cross section by ~0.25–0.43%**, and `|C_rad(e-)−C_rad(mixed)|`
is 0.03–0.21%. Note the **generator disagreement in the ep−em C_rad split** (−0.40% RAPGAP vs
+0.06% DJANGOH at low-y): the beam-charge radiative structure is generator-dependent at
sub-percent level and would itself enter a systematic budget.

**Exact per-bin quantification under the baseline definition — COMPUTED 2026-10-07** (per-beam
re-merge clusters 5385358/5385359; independently re-verified from the merged npz primitives;
full records in `eminus_quantification_2026-10-07.json` in each per-beam dated directory):

| Quantity (exact selection, 490 bins) | RG31 h+ | RG31 h- | DJ14 h+ | DJ14 h- |
|---|---|---|---|---|
| integrated C_rad(em) | 1.1348604 | 1.1343076 | 1.1420617 | 1.1416167 |
| integrated C_rad(mixed) | 1.1368045 | 1.1362373 | 1.1418534 | 1.1413756 |
| integrated (C_em−C_mix)/C_mix | **−0.1710%** | **−0.1698%** | **+0.0182%** | **+0.0211%** |
| per-bin \|C_em−C_mix\|/C_mix: median | 0.360% | 0.369% | 0.289% | 0.316% |
| per-bin \|C_em−C_mix\|/C_mix: p95 | 2.79% | 3.01% | 2.18% | 2.35% |
| per-bin R_born=σn_ep/σn_em: median | 0.9939 | 0.9912 | 0.9864 | 0.9926 |
| R_born: integrated | 0.9917 | 0.9918 | 0.9898 | 0.9910 |
| F(i)=1/[f_em+f_ep·R_born·C_em/C_ep]: median \|F−1\| | 0.659% | 0.627% | 0.897% | 0.597% |
| F: p95 \|F−1\| | 7.10% | 7.18% | 7.61% | 6.23% |
| **integrated F − 1** | **+0.603%** | **+0.593%** | **+0.507%** | **+0.444%** |

(Sparse-bin tails: max per-bin |C_em−C_mix|/C_mix 11.3%/9.6% RAPGAP31, 4.4%/10.0% DJANGOH14;
max |F−1| ~10–11%; see the JSON records. Cross-generator spread of F integrated, +0.60/+0.59%
RAPGAP31 vs +0.51/+0.44% DJANGOH14, is itself a sub-percent systematic to budget.)

**Conclusion (exact):** correcting the MIXED data with `C_rad(e-)` alone leaves an additional
beam-charge factor of **+0.44% to +0.60%** (integrated, generator/charge dependent) to reach the
exact e- p Born target, consistent with the low-y control estimates (−0.43%/−0.25% undershoot
there vs +0.60%/+0.51% exact F here); the per-bin |C_em−C_mix|/C_mix spread is
median 0.29–0.37%, p95 2.2–3.0%. All caveats in the next subsection still apply at full weight.

Historical indicative numbers (pre-merge, DJANGOH only, OLD selection, from `britzger
.../complete_2953` per-beam columns with canonical v3 exposures; n=466 finite; superseded by
the exact table above):

- per-bin |C_ep−C_em|/C_em: median 0.64% (h+ 0.646%, h- 0.638%), p95 30.6% (sparse-bin MC-stat tails);
- per-bin |C_mixed−C_em|/C_em = w_ep·|C_ep/C_em−1|: median 0.33% per charge, p95 14.7%;
  per-bin w_ep median 0.5123 (p5 0.4691, p95 0.5307);
- integrated: C_ep = 1.1057806, C_em = 1.1058801 → (C_em−C_ep)/C_ep = **+0.0090%** (DJANGOH);
- per-bin R_born = sigma_born_ep/sigma_born_em: median 0.9930, median |1−R| 1.50%, p95 19.5%;
  integrated sigma_born em/ep = **+0.88%**.

### Caveats that fold into an e--only correction

1. **e- GenDIS 4.67% count shortfall** (documented for the DJANGOH RAD e- pool: 642 files × 100k
   implied 64,200,000 vs 61,199,996 retained = 4.6729% short; e+ only 0.41%; partial chunks vs
   upstream attrition unresolved — `docs/radiative/rad_conditioning_resolution_2026-09-25.md`).
   If it is real attrition, `sigma_rad(em)=sumw/L_rad_em` is biased low and `C_rad(em)` biased
   high by up to ~4.7%. In the mixed table this is diluted by the clean e+ leg (~51% rad share);
   **an e--only correction carries it at full weight.** No equivalent published count audit
   exists for the RAPGAP RAD e- pool.
2. **e- NONRAD pool composition**: for RAPGAP31 the nonrad supplement is absent for BOTH beams
   (`nonrad_new = {ep:0.0, em:0.0}`; e- exposure 13,416,991.0 nb^-1 is OLD-pool only, 503/520
   chunks, 17 degenerate C0000032 excluded); DJANGOH14 e- nonrad = old (12,841,572.442) +
   new (1,699,702.771) = 14,541,275.213 nb^-1. Pool composition differs by generator.
3. **NONRAD exposure label `DIAGNOSTIC_MANIFEST_LEVEL`** (summary JSONs; STATE.md blocker wording
   `DIAGNOSTIC_GENDIS_EVENT_LEVEL`): nonrad exposures are manifest-level diagnostics, not
   oomclumi-traced (RAD exposures ARE traced). An e--only table inherits this at full weight on
   the nonrad leg; the conflicting NEW-exposure path (factors ~1.4670/1.4531, closure plan) is
   part of the same open blocker.
4. All values above remain PROVISIONAL DIAGNOSTIC; the Empz-threshold convention decision and the
   residual z-rising/pT_h-falling trend blockers (STATE.md) apply unchanged to any e- variant.

## 3. Original blocker — exact per-beam re-merge needed (RESOLVED, see Section 3b)

Per the login-node policy (multi-file processing -> Condor, always) and the delegation instruction
(do not launch Condor from this task), the e- tables could not be derived. **Exact merge needed:**

1. Fork `crad_v3_finalize_xfcut_{rapgap,djangoh}.py` (label/glob changes only, mirroring the
   xFcut fork discipline) to write **per-beam** tables — recommended: both beams in one pass
   (`C_rad(ep)` is needed for `R_born`, `C_em/C_ep`, and the exact mixed-vs-em decomposition).
2. Inputs: the per-file npz partials in Section 1 (em: 830 RAPGAP + 1432 DJANGOH files; ep adds
   947 + 1521). Sum `sumw/sumw2/n` per pool per beam (490-vec; optionally also the (6,490)
   y-resolved arrays for y-resolved variants); `sigma = merged_sumw / L_b`;
   `C_rad_b(i) = sigma_nonrad_b/sigma_rad_b`;
   `unc = C*sqrt(sumw2_nonrad/sumw_nonrad^2 + sumw2_rad/sumw_rad^2)`; same `meta()` 490-row schema.
3. Exposures (verbatim from the baseline summary JSONs): RAPGAP31 rad em 14,461,067.0 /
   nonrad em 13,416,991.0; rad ep 17,536,474.0 / nonrad ep 13,954,768.0. DJANGOH14 rad em
   13,534,017.0 / nonrad em 14,541,275.213 (old 12,841,572.442 + new 1,699,702.771); rad ep
   16,206,393.0 / nonrad ep 14,637,492.676 (old 12,920,115.855 + new 1,717,376.821). Keep the
   `DIAGNOSTIC_MANIFEST_LEVEL` label and the benign `projection_regression([])=False` convention
   for the absent RAPGAP nonrad_new pool (G5).
4. Vehicle: one Condor job per generator mirroring `run_finalize_xfcut.sh` (thish1.sh-sourcing
   wrapper; precedent clusters 5341298/5341299), single core, minutes of wall.
5. Outputs: `data/radiative/rapgap_eminus_crad_final_xfcut_2026-10-07/` and
   `data/radiative/djangoh14_eminus_crad_final_xfcut_2026-10-07/`, each with a 490-row CSV
   (`crad_v3_eminus_xfgen_lt0_empz_gt45_nominal_490rows.csv`), optional y-resolved variant, and a
   summary JSON recording formula `C_rad(e-)=sigma_nonrad(em)/sigma_rad(em)`, per-beam exposures,
   finite counts, `c_rad_applied=false`, status PROVISIONAL DIAGNOSTIC, hemisphere convention,
   and the e- target statement.
6. Validation: 490/490 finite expected (low-y closures show healthy em pools) but must be
   verified; range check [0.5,1.2]; smoothness; cross-generator comparison; then the exact
   per-bin/integrated |C_rad(e-)-C_rad(mixed)| numbers for Section 2 become computable.

### 3b. PREPARED per-beam re-merge (2026-10-07, second session — staged, NOT SUBMITTED)

The Section 3 merge is now fully staged (forks, wrappers, .sub files, smoke checks).
Nothing has been submitted; the baseline directories, the canonical plot package, and
all central v6 products are untouched.

**Fork scripts** (per-beam variants of the Section 3 originals, same campaign script dir):

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/crad_v3_finalize_xfcut_perbeam_rapgap.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/crad_v3_finalize_xfcut_perbeam_djangoh.py`

Design (identical for both generators; `mix_by_y` retained VERBATIM from the originals
and used only for the mixed-reconstruction validation):

- Sums the per-file npz partials per pool per beam (`sumw,sumw2,n` (490,) AND
  `sumw_y,sumw2_y,n_y` (6,490)).  RAPGAP pools `rad`/`nonrad` (the absent `_absent`
  new-pool glob reproduces the benign `projection_regression([])=False` G5
  convention); DJANGOH pools `rad`/`nonrad_old`/`nonrad_new` (a pool with nonzero
  exposure and zero files is fatal).
- Divides by the per-beam exposures hardcoded verbatim from the baseline
  `crad_v3_summary.json` files AND cross-checked against them at runtime (fatal on
  mismatch); the baseline summary is copied into the output dir as
  `exposure_provenance_baseline_summary.json`.
- **Per-beam layout (documented choice): separate per-beam files, beam in the
  filename, NO extra CSV column** — the 490-row schema stays byte-identical to the
  published mixed tables (`bin_index,charge,cell,Q2_min,Q2_max,x_min,x_max,z_min,z_max,
  pTh_min,pTh_max,empz_mode,sigma_rad,sigma_nonrad,C_rad,C_rad_stat_unc,
  relative_stat_unc,nonrad_n,rad_n,finite_C_rad`), so the canonical plot script
  consumes them after the trivial extract (`Q2_min/q2_max/pTh_*` renames, append
  `z_idx,pT_idx`, slice 245 rows/charge).
- Both beams written in one pass: `em` is the primary e- target; `ep` is the required
  by-product for `R_born` and the mixed-vs-em decomposition.
- `c_rad_applied=false` everywhere; status PROVISIONAL DIAGNOSTIC.

**Submission machinery** (one job per generator, mirroring the finalize precedent —
clusters 5341298/5341299 pattern; thish1.sh-sourcing wrapper, `retry_until =
(ExitCode != 127)`, `max_retries = 5`, exit 127 on AFS-unavailable, exit 96 on invalid
H1OO env; 1 core / 4 GB / 3600 s):

- RAPGAP wrapper: `.../output_v3/rapgap_perbeam_finalize_20261007/run_perbeam_finalize_rapgap.sh`;
  submission file: `.../output_v3/rapgap_perbeam_finalize_20261007/finalize_perbeam_rapgap.sub`
- DJANGOH wrapper: `.../output_v3/djangoh_perbeam_finalize_20261007/run_perbeam_finalize_djangoh.sh`;
  submission file: `.../output_v3/djangoh_perbeam_finalize_20261007/finalize_perbeam_djangoh.sub`
  (all under
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/`)

Submission commands (NOT executed): `condor_submit <sub path>` per generator.

**Expected outputs:**

- `data/radiative/rapgap_eminus_crad_final_xfcut_2026-10-07/`:
  `crad_v3_xfgen_lt0_empz_gt45_nominal_perbeam_{ep,em}_490rows.csv`,
  `crad_v3_xfgen_lt0_empz_gt45_nominal_perbeam_{ep,em}_yresolved_2940rows.csv`,
  `merged_primitives_{rad,nonrad_old}_{ep,em}.npz` (worker-partial array schema plus
  provenance keys `files,n_files,diag`), `crad_v3_perbeam_summary.json`,
  `exposure_provenance_baseline_summary.json`, `smoke/` (already populated).
- `data/radiative/djangoh14_eminus_crad_final_xfcut_2026-10-07/`: same, plus
  `merged_primitives_nonrad_new_{ep,em}.npz`.
- Filenames use `perbeam_{ep,em}` rather than the Section 3 sketch (`eminus_...`)
  because BOTH beams are written; the directory names keep the task-specified
  `eminus` branding.

**Validation criteria (hard gates in the forks; verdict FAIL + exit 1 on any):**

1. Exposure cross-check vs the baseline `crad_v3_summary.json` (exact equality; fatal).
2. Per-beam low-y [0.1,0.2) closure recomputed with the original formula, compared to
   the baseline `prebeam_mixing_low_y_closure` per beam: rel tol 1e-9 (expected
   ~1e-15; bit-exactness is NOT guaranteed across compute nodes because numpy SIMD
   summation order can differ — hence the explicit tolerance).
3. ep+em mixed reconstruction (`mix_by_y` verbatim + verbatim row loop) vs the
   published mixed 490-row table: per-bin rel tol 1e-9 on `sigma_rad`,
   `sigma_nonrad`, `C_rad`; exact integer equality of `nonrad_n`/`rad_n`; integrated
   mixed C_rad per charge vs the baseline summary rel tol 1e-9.
4. Per-beam 490/490 finite C_rad target (low-y closures indicate healthy em pools).
5. Post-merge manual checks (recorded here, not coded in the forks): range [0.5,1.2],
   smoothness, per-bin/integrated |C_rad(e-)-C_rad(mixed)| and per-bin R_born — the
   Section 2 quantification then becomes exact.

**Single-file smoke results (login-node legal, ONE partial per generator, both PASS):**

| Check | RAPGAP31 | DJANGOH14 |
|---|---|---|
| smoke input | `partials/rad/em/rapgap_rad_em_0000_empz_gt45_nominal.npz` | `partials/rad/em/rad_em_0731_empz_gt45_nominal.npz` |
| keys / shapes / dtypes | PASS | PASS |
| projection identity (bit-exact) | PASS | PASS |
| 490 rows / 2940 yrows | PASS | PASS |
| degenerate per-bin C_rad = RAD[em]/NONRAD[em] (analytic; tol 1e-12) | PASS, max rel dev 4.12e-16 (expected const 1.0778174480) | PASS, max rel dev 3.58e-16 (expected const 0.9307310949) |
| finite rows in the single file (degenerate) | 477/490 | 480/490 |
| CSV schema == published mixed 490 header | PASS | PASS |
| trivial extract == canonical plot input schema | PASS | PASS |
| bin mapping vs published table (rows 0/244/245/489) | PASS | PASS |
| merged-npz round trip | PASS | PASS |
| smoke record | `data/radiative/rapgap_eminus_crad_final_xfcut_2026-10-07/smoke/smoke_singlefile_rapgap31_2026-10-07.json` | `data/radiative/djangoh14_eminus_crad_final_xfcut_2026-10-07/smoke/smoke_singlefile_djangoh14_2026-10-07.json` |

(The 477/480 single-file degenerate finite counts reflect that ONE partial covers a
subset of bins; the 490/490 finite target applies to the full per-beam merge.)

**COMPLETED 2026-10-07 (third session):** the two .sub files were submitted and ran on Condor
(clusters **5385358** RAPGAP31 / **5385359** DJANGOH14; both rc=0, `variant_pass=true`, all
in-job gates pass).  Products landed exactly as specified above in
`data/radiative/{rapgap,djangoh14}_eminus_crad_final_xfcut_2026-10-07/` (per-beam 490-row +
y-resolved CSVs, merged npz primitives incl. DJANGOH nonrad_new, `crad_v3_perbeam_summary.json`,
exposure provenance copies; in-job mixed reconstruction and closure rel diffs 0.0).

**Independent re-verification (not trusting the job verdict)** — recomputed from the merged npz
primitives and per-beam CSVs by
`sidis_highQ2/scripts/radiative/eminus_crad_perbeam_quantification_2026-10-07/verify_and_quantify_eminus_perbeam.py`:

- finite rows: **490/490 per beam per generator** (flag and value);
- per-beam low-y closure from npz vs baseline `prebeam_mixing_low_y_closure`: **bit-exact**
  (rel diff 0.0, both beams, both generators);
- ep+em mixed reconstruction from npz vs published mixed 490-row tables (all 490 bins):
  max rel diff **6.1e-16** (RAPGAP31) / **6.3e-16** (DJANGOH14), **0 count mismatches**,
  0 finite mismatches;
- per-beam CSVs vs npz-recomputed sigmas: max rel dev **0.0**; C_rad recomputed from table
  sigmas consistent to <1e-12;
- verdict **PASS** both generators; records `eminus_quantification_2026-10-07.json` in each
  dated directory (verification block + the Section 2 exact quantification numbers).

## 4. Plot procedure (Task C — COMPLETED 2026-10-07)

Delivered package: `plots/radiative/crad_v4_xfcut_eminus_djangoh_vs_rapgap_7col_2026-10-07/`
(the name from the current task instruction; the earlier Section-4 sketch below wrote
`..._eminus_django_h_...`, a typo superseded by the instruction's `..._eminus_djangoh_...`).
Built exactly per the original procedure:

Target package: `plots/radiative/crad_v4_xfcut_eminus_django_h_vs_rapgap_7col_2026-10-07/`
(task-specified name; note the canonical script's actual filename is
`plot_crad_djangoh_vs_rapgap_fLumi_7col.py`). Copy it IDENTICALLY; change ONLY:

- `parse_args()` input paths (lines ~111-115) to four package-local 245-bin extracts generated
  from the new e- tables (canonical extract schema: `q2_min/q2_max/pT_min/pT_max` renames plus
  `z_idx/pT_idx`; **take ALL columns from the new e- tables** — do not replicate the canonical
  package's stale legacy sigma/count columns, see Section 6);
- ylabel (line ~585) `r"$C_{\rm rad}$"` -> `r"$C_{\rm rad}(e^-)$"`;
- sample labels / manifest strings -> "e- p Born-level target",
  `C_rad(e-)=sigma_nonrad(em)/sigma_rad(em)`, PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`.

Run locally (Agg; needs `pdfunite`, `pdftoppm`): produces per charge x {zoom,wide} = 4 PDF + 4 PNG,
`crad_v4_review.pdf` (4 pages), `page_images/` (4 PNG), `manifest.json` + `manifest_zoom.json` +
`manifest_wide.json`; then amend `manifest.json` with `variant`/`status`/`provenance_sidecars`
and write `validation.json` mirroring the canonical structure (490/490 rows/finite per generator,
integrated e- values, pdf_pages, `provisional_diagnostic: true`, `c_rad_applied: false`).

**Delivered (2026-10-07):** `plots/radiative/crad_v4_xfcut_eminus_djangoh_vs_rapgap_7col_2026-10-07/`

- script `plot_crad_djangoh_vs_rapgap_fLumi_7col.py` — copied IDENTICALLY from the canonical
  package, then only: 4 input paths -> package-local extracts; docstring/labels
  (`Djangoh/Rapgap $e^{-}p$` legend, ylabel `C_rad(e^-p)`, sample keys `... e- perbeam (em)`);
  output stem `..._eminus_7col_...`; manifest gains `target`/`status`/`c_rad_applied` keys
  (structure otherwise unchanged; diff vs canonical verified to contain ONLY these changes);
- extracts `{djangoh,rapgap}_{pos,neg}_245bins.csv` generated by package-local
  `make_eminus_extracts.py` — ALL columns from the perbeam_em 490-row tables (single source;
  no stale legacy columns), canonical extract schema (verified in the single-file smokes);
- outputs: `crad_djangoh14_vs_rapgap31_eminus_7col_{hplus,hminus}_{zoom,wide}.pdf/.png`
  (4+4, 49 panels/charge, all points finite and within the applied ylims),
  `crad_v4_review.pdf` (4 pages), `page_images/crad_v4_review-{1..4}.png`,
  `manifest.json`/`manifest_zoom.json`/`manifest_wide.json` (with e- target statements,
  PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`);
- `validation.json` mirroring the canonical structure: PASS, 490/490 rows/finite per generator,
  integrated e- values matching the per-beam summary JSONs to fp, pdf_pages 1/1/1/1/4,
  `mixed_beam_baseline_superseded: false`, `c_rad_applied: false`.
- The canonical package `crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/` and the baseline
  tables remain untouched (mtimes unchanged).

## 5. Supersession status of the pre-existing e- study

`scripts/radiative/britzger_eminus_crad_xf_y_eta_q2_2026-09-26/` (and its data/plot products):
**NOT superseded** — no baseline-definition e- derivation exists yet (blocked). Its
`complete_2953` table remains the only completed per-bin e- C_rad table in the workspace, with
its documented incompatibilities: DJANGOH-only, 471/490 finite, different definition
(stored `xFGen<0` nominal-beam-q bug bounded +0.039%, `abs(GenEta_lab)<4`, no GenEmpz cut),
diagnostic nonrad exposures, `DIAGNOSTIC_ONLY_NOT_RELEASE_COMPATIBLE`, `c_rad_applied=false`.
Its top-level 490-row table is INVALID (734-row recovery subset). **Update 2026-10-07: the
Section 3b per-beam re-merge has now landed and is independently verified, so the new
`..._perbeam_{ep,em}_490rows.csv` tables (490/490 finite, exact baseline definition) SUPERSEDE
the britzger `complete_2953` table for all baseline-definition use; the britzger study keeps
only historical/typology value.** The v6exact study (2026-09-27) never
finalized and has no table to supersede.

## 6. Hygiene note (canonical package, not altered)

The canonical package `plots/radiative/crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/` 245-bin
extract CSVs carry `C_rad`/`C_rad_stat_unc` from the xFcut baseline tables (plot-consumed
columns, verified) but their `sigma_rad/sigma_nonrad/rad_n/nonrad_n` columns are stale legacy
leftovers (RAPGAP ones verified byte-equal to the quarantined blind FINAL 2026-09-30 table).
The plot script does not read them, so the canonical plots are unaffected; future extract
generators should populate all columns from one source.

## 7. NoZ C_gamma_born — photon-only pure-Born target (2026-10-07 design; PIVOT 2026-10-08: existing NOZ Born sample = PRIMARY; CHAIN COMPLETE 2026-10-08 — Stage-4 exact-exposure finalize rerun EXECUTED (cluster 5413894), see 7g)

Motivation: the full-EW e- p Born target of Sections 2-3b requires the per-bin residual
factor F(i) = 1/[f_em + f_ep·R_born(i)·C_em(i)/C_ep(i)] because the full-EW Born level is
charge-dependent. Moving the target to the **photon-exchange-only pure Born level** removes
that obstruction exactly: photon Born is charge-symmetric (R_born^gamma == 1 identically), so
the mixed-beam data can be corrected to a single, beam-charge-blind target with one factor.

### 7a. Validated per-bin definition

```
C_gamma_born(i) = sigma_gamma_born,e-(i) / sigma_rad_mixed(i)
```

- Numerator: NoZ-Born e- sample histogrammed with the exact xFcut baseline selection
  (150<Q2_es_gen<20000; 0.1<Y_es_gen<0.7; GenEmpz>45; recomputed xF<0 q-nominal; charged
  hadrons; 490-bin schema; weight1*weight2) / NoZ-Born exposure. **Recommended and adopted.**
- Denominator (recommended): the per-beam merged RAD primitives
  (`djangoh14_eminus_crad_final_xfcut_2026-10-07/merged_primitives_rad_{ep,em}.npz`),
  beam-mixed with the byte-identical baseline `mix_by_y` construction — i.e. the full
  data-to-target factor, not a "strict c_rad-like" two-factor product. Rationale: (i) it is
  the actual multiplicative correction for the mixed data; (ii) the decomposition is exactly
  recoverable via the identity below, so nothing is lost; (iii) a single table avoids
  compounded rounding and keeps the numerator statistics explicit. DJANGOH-only on both
  sides (a RAPGAP denominator would confound EW/loop effects with generator differences —
  and no RAPGAP NoZ exists at all; recorded as a coverage gap).
- Charge symmetry: exact per bin at tree level — |M(e- q)|^2 = |M(e+ q)|^2, no weak terms,
  same PDFs; Britzger verified the integrated identity (218.84216 pb, both charges). The
  beam mixture enters only through the mixed RAD denominator, so **no residual per-beam
  factor is required** (the F(i) pathology of Section 2 vanishes). An e+ control
  would provide the empirical null test (G8), but NO photon-only e+ sample exists
  (audit-verified absence 2026-10-07) — G8 stays conditional/impossible with
  existing samples; the exact tree-level symmetry argument carries the target.
- Identity gate (hard, G4): C_gamma_born(i) = C_rad_mixed(i) × [sigma_gamma_born(i)/
  sigma_nonrad_mixed(i)], with C_rad_mixed from the published baseline table and
  sigma_nonrad_mixed reconstructed from the per-beam merged nonrad primitives; enforced per
  bin at rel 1e-9. Smoke-verified at 2.2e-16 with the real primitives and published table.

### 7b. Loop verdict (CORRECTED 2026-10-08) — the EXISTING NOZ sample IS the pure-Born photon target; no new production

**The 2026-10-07 verdict "new loops-off production required" is SUPERSEDED.** It was
based on the archived test card
`/data/dust/group/h1/britzger/norad_prod21/EW_gZ_correction/stdjh14_nc_eminusp_noZ_q2gt100_cteq6l_a_2Feb23`,
whose LPIN(2) line is COMMENTED — i.e. the NOZ1/corrected variant. The actual
production samples carry their own card echo in the GEN FPACK headers, and the
completed mc-db audit (`NOZ_PROVENANCE_INVENTORY_2026-10-07.md`) plus an independent
read-only re-verification (2026-10-08, evidence copies + md5 manifest under
`H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn_hist_xfcut_20261007/verification/`,
record `PIVOT_VERIFICATION_2026-10-08.md`) establish:

- **NOZ** (`dst2all_djangoh14_NoZ/`, e- only, A-D x 65 chunks, GEN DATE 230202):
  FPACK title = Levonian's standard NORAD title PLUS "Special executable with
  Z-exchange switched off (D.Britzger)"; card echo **ACTIVE `'LPIN' 2 0 0`** (pure
  Born photon-only, no soft&virtual 1-loop) — verified independently in the series-A
  first chunk C0000000 (byte-identical to the audit's dump, md5 9ce67bdf...) AND the
  series-D first chunk C0000000. Also echoed: LPIN(4)=0 ACTIVE, LPIN(7)=2 ACTIVE but
  **inert at Born** (the running-alpha terms it controls belong to the 1-loop
  corrections that LPIN(2)=0 switches off — confirmed by the Born validation log's
  VIRT&SOFT block), LPIN(8/9/10/11) commented (=0), INT3/ISM3 commented (off),
  INT2(1)/ISM2(1) ACTIVE, Q2MI=100 ACTIVE.
- **NOZ1** (`dst2all_djangoh14_NoZ_new/`): identical except **ACTIVE `'LPIN' 2 0 1`**
  (corrected, Born+SV) — the two productions differ exactly in LPIN(2).
- **Exe identity:** both from the fixed 2023 "neu" NoZ build
  `/data/dust/group/h1/britzger/djangoh14_NoZ/djangoh14test` (sha256
  22e105df7331a85bf7e1fb3b1a7f4c6bb9a2e5cc8c0f631fbba7fef40ad1852f, mtime 2023-02-01
  13:16 = banner "compiled at date 10223 and time 1316", present in both validation
  logs). Born validation `log.Dj.NoZ.2Feb23.txt`: "GDJCHA:PARAMETER LPIN( 2) CHANGED
  FROM DEFAULT 1 TO 0", VIRT&SOFT "1-LOOP CORRECTIONS: LPAR( 2) = 0",
  SIGMA-GAMMA-Z/-Z/-W = 0, SIGTOT 6.7086E+00 nb (SIGMT 0.670856E+04 pb = 6708.559 pb).
  NOZ1 validation `log.Dj.NoZ.NoBORN.txt`: LPIN(2) at default 1, LPAR( 2) = 1,
  SIGTOT 7.2859E+00 nb (7285.933 pb) — the sigma difference proves the neu build's
  running-alpha/LPIN(2) response (the broken first-2023 build did not respond).
- **Channel-label caveat:** the HERACLES .OUT channel label "NEUTRAL CURRENT /
  ELASTIC + SOFT&VIRTUAL" is FIXED BOILERPLATE — it appears IDENTICALLY in the Born
  (SIGTOT 6.7086) and corrected (SIGTOT 7.2859) validation logs. The originally
  spec'd G7 sub-check "channel label free of SOFT&VIRTUAL" is therefore NOT
  ENFORCEABLE and was replaced by the LPAR(2) echo + LPIN(2)-CHANGED line +
  SIGTOT-vs-6.7086nb(+-5%) checks (Born and corrected sigmas are 8.6% apart, so the
  band separates them). Recorded in every G7 record.
- **No production-scale LOGOUTPUT/STDOUT exists** for the 2023 NOZ/NOZ1 productions
  (DUST + dCache searched) — the sigma values are same-config validation-run values
  tied to the productions by identical card echo + identical exe banner; G7 is
  evidence-based by design (see 7d).

Loop-effect facts (unchanged, for interpretation): corrected/Born = 7.2859/6.7086 =
**+8.61% integrated at Q2>100**; +11.27% at Q2>1000; per bin it grows with Q2
(vacuum polarization ~ [alpha(Q2)/alpha(0)]^2 plus vertex/box/soft residuals) and
does NOT factorize as a per-bin alpha(Q2) factor. Our full-EW NONRAD Born legs
(stdjan family) run LPIN(2)=1, LPIN(7)=2, LPIN(8/9/11)=1 (SIGTOT 7.3239 nb em).

**Consequence (user decision 2026-10-08): primary track = existing NOZ Born sample
through the existing chain; NOZ1 = optional corrected-level crosscheck only.** The
prepared loops-off artifacts (card
`/data/dust/group/h1/klesthen/BaryonAsym/Djangoh14/stdjan_nc_eminusp_nozborn_q2gt100_cteq6l_a`
+ .diff, the three `hk_nozborn_probe*` campaigns, and
`Djangoh14/scripts/prepare_nozborn_campaign_20261007.py`) are RETAINED AS CONTINGENCY
ONLY and documented as superseded. The `EW_gZ_correction` TEST card must never again
be quoted as the production configuration.

### 7c. Ingestion chain — no code changes anywhere

Existing-chain facts established this session: the v3 treebuilder campaign consumes HAT+MODS
via 7-field queues (label, steer, out, sample, beam, hat, mods) from three interchangeable
sources (official pnfs OO pool, our stage-2 conversions, Britzger dst2all — proven read-only
by the old-nonrad recovery); the xFcut worker consumes the resulting ChargedGenRC ntuples via
3-field queues (input, prefix, sample=rad|nonrad); NOZ1 dst2all files are branch-identical to
the full-EW ones (HAT tree 219/219 identical).

**Empirical single-file smoke (login-node, bounded, 2026-10-07 on NOZ1; repeated
2026-10-08 on the NOZ PRIMARY):** the sha-pinned `RC_treebuilder_genonly_v3`
(ba21ee5f..., sha re-verified before each run) with `--max-events 3000`:
- 2026-10-07, `dst2all_djangoh14_NoZ_new/` (NOZ1) A.C0000000: rc=0, 66,452 hadron
  rows, all 84 branches, zero missing worker-required branches; UNMODIFIED xFcut
  worker (`--sample nonrad`): verdict PASS, exact baseline partial schema, MCWE
  weights live (1..97), event-level GenEmpz ~ 55.2 GeV (no-radiation expectation).
- 2026-10-08, `dst2all_djangoh14_NoZ/` (NOZ PRIMARY) A.C0000000: rc=0 in ~15 s,
  66,533 hadron rows, 84 branches, GenEmpz mean 55.2 GeV; worker verdict PASS,
  cutflow 66,533 -> 30,462 (DIS fiducial) -> 14,567 (xF<0 binned),
  `c_rad_applied=false`.
**Builder + worker consume the NOZ files with zero code changes — verified on the
primary sample, not just asserted.**

Chain per track (concrete commands in the campaign README): PRIMARY (NOZ Born,
files exist now) = READY 260-pair queue `nozborn_em_tree_queue.tsv` ->
`nozborn_treebuilder.sub` -> after ALL treebuilder jobs succeed:
`hist_nozborn.sub` (pre-built 260-entry `hist_queue_nozborn_em.tsv`) -> finalize
`run_finalize_nozborn.sh NOZBORN <L_MC|PENDING>`. OPTIONAL crosscheck (NOZ1,
corrected level, NOT the primary) = `noz1x_treebuilder.sub` -> `noz1x_hist.sub` ->
`run_finalize_nozborn.sh NOZ1_CROSSCHECK <L_noz1|PENDING>` (expected integrated
~1.136). The former Track B (own loops-off production) is superseded contingency.

### 7d. Prepared skeleton + gates (updated 2026-10-08)

Campaign dir: `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn_hist_xfcut_20261007/`
(builder `build_nozborn_campaign.py` — treebuilder 7-field + histogram 3-field modes,
the latter with pre-submission `--from-tree-queue`; wrappers; subs; BOTH queues READY
at 260 entries each for the primary `nozborn` and the optional `noz1x` crosscheck;
README with the full plan). Evidence dir `verification/` (GEN-header dumps of NOZ
series A + D first chunks, both neu-exe validation logs, md5 manifest,
`PIVOT_VERIFICATION_2026-10-08.md`, `g7_evidence.list`). Exact exposure sweep
prepared 2026-10-08: `exposure_sweep_20261008/` (260-row args TSV — pair set identical
to the treebuilder queue, all 520 files verified; per-chunk runner
`run_nozborn_oomclumi_chunk.sh` mirroring the RAD v2 precedent; sub
`submit_nozborn_oomclumi_sweep_20261008.sub`, 260 jobs, retry_until ExitCode!=127
max_retries=5; aggregator `aggregate_nozborn_oomclumi_sweep_20261008.py` with
coverage/sigma/identity/estimate-comparison gates, canonical JSON written on PASS
only; smoke verdicts in `exposure_sweep_20261008/SMOKE_VERDICT_2026-10-08.md`).
Finalize fork:
`scripts/histogramming_v3_xfcut_20261005/crad_v3_finalize_xfcut_nozborn.py` via
`run_finalize_nozborn.sh` (exercised end-to-end in a sandbox). Output dir (skeleton):
`data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/`. Ntuples
`output_v3/nozborn/em/`, partials `partials/nozborn/em/`.

Exposure (RESOLVED 2026-10-08, h1-normalization `NOZBORN_EXPOSURE_DERIVATION_2026-10-08.md`):
**L_MC = 14,032,218 nb^-1 SAMPLED_ESTIMATE** (4-series anchor mean weight 2.3533989 x
N 40,000,004 / sigma_Born 6708.559 pb x 1000; 1-sigma 36,500 nb^-1 = 0.26%) is the
`finalize_queue.tsv` row (was `PENDING`). Count-based exposure is INVALID and rejected
by the fork (MCWE mean weight 2.353399; count-based L = 5,962,533 nb^-1 would inflate
sigma_proxy ~2.35x and put integrated C ~2.46 outside the G6 band) — the provenance
schema is **weighted `{sumw, sigma_pb, authority}`** (closure L*sigma_pb/1000 == sumw),
and the fork hard-rejects `{n_events,...}`. Wiring in `run_finalize_nozborn.sh`
(sandbox-tested A-E): the **exact 260-chunk oomclumi weighted sweep**
(`exposure_sweep_20261008/`, forked from the `rad_oomclumi_exact_v2_20260924`
precedent) writes `nozborn_exposure_exact_2026-10-08.json` under the output dir at
status PASS only; when present+PASS the wrapper supersedes the queue value with
`lumi_from_sumw_nb_inv` and attaches the weighted provenance
(`exposure_source=EXACT_OOMCLUMI_SWEEP`); otherwise the h1-normalization estimate
record supplies the provenance (`exposure_source=SAMPLED_ESTIMATE`); an explicit
provenance argument always wins. `--nozborn-exposure-nb PENDING` still runs
PENDING_NORMALIZATION mode (L=1.0 placeholder, verdict never PASS, G6/G8
PENDING_EXPOSURE, no provenance). Wrapper bug fixed 2026-10-08: a `provargs`/`provars`
typo had silently dropped `--exposure-provenance` from the fork invocation.

Gates: G1 projection regression; G2 490/490 finite; G3 baseline reconstruction
(1e-9; smoke bit-exact 0.0); G4 identity (1e-9; exposure-INDEPENDENT — evaluated
also in PENDING mode; smoke 2.2e-16); G5 low-y closure (1e-9); **G6 range (requires
exposure): integrated per charge [0.95,1.12]** — expected ~1.046 = C_rad_mixed_integrated
1.14185/1.14138 x F_EWgamma 6.7086/7.32394 = 0.91598; the band EXCLUDES a
NOZ1-as-NOZ mixup (~1.136); per-bin absolute sanity [0.4,1.8] (published C_rad spans
[0.840,1.572]); soft report-only per-bin F_EWgamma band [0.75,1.02]; NOZ1_CROSSCHECK
label: integrated [1.06,1.21]. **G7 loop-config echo, EVIDENCE-BASED (hard for
NOZBORN):** per GEN-header dump (title Z-off sentence, ACTIVE 'LPIN' 2 0 0, Q2MI=100,
INT3/ISM3 commented) + per validation log (SIGTOT 6.7086 nb +-5%, LPAR(2)=0,
SIGMA-GAMMA-Z/Z/W=0, LPIN(2) CHANGED 1->0, banner 10223 1316); the "channel label
free of SOFT&VIRTUAL" sub-check is NOT ENFORCEABLE (fixed boilerplate, see 7b) and is
replaced by these checks; smoke: G7 PASS on the Born evidence, FAIL on the NOZ1 log
(negative test). G8 charge symmetry: conditional; NOT POSSIBLE with existing samples
(no photon-only e+ exists, audit-verified absence). Contract JSON
`crad_djangoh14_nozborn_xfcut.json` written with step/channel/status/bins/values/
errors/validation; `c_rad_applied=false`; all products PROVISIONAL DIAGNOSTIC;
DJANGOH-only caveat (no RAPGAP NoZ exists) recorded in the contract.

Finalize dry-run smoke (single 3000-event partial, PENDING): mechanics PASS —
G1 True, G3 0.0, G4 2.216e-16, G5 True, G7 PASS, G6/G8 PENDING_EXPOSURE/None,
status PENDING_NORMALIZATION_PROVISIONAL_DIAGNOSTIC; overall verdict FAIL only from
G2 (371/490 finite bins at single-partial scale) — expected gate firing, resolves
with the full 260-chunk numerator.

Exposure-sweep smokes (2026-10-08, login-node legal, single chunk): runner PASS on
A.C0000000 (rc=0, 20.9 s, 154,870 events; oomclumi prints SigmaBorn = Sigma =
SigmaTot = 6708.559 pb — exactly the normalization reference — and //Total
Lumi = 54.271 pb^-1, matching the h1-normalization spot check 54.2712; zvtx Lumi
0.000 as expected for fGenOnly=1); aggregator dry-run on the single result PASS
(mechanics): FAIL-as-designed 1/260, canonical JSON correctly NOT written (PASS-only),
parse/sigma/identity/comparison paths exercised. Finalize exposure-wiring tests
(sandbox) all PASS: (A) estimate fallback -> SAMPLED_ESTIMATE + weighted provenance
(rel_diff_vs_arg 1.7e-8); (B) exact JSON PASS -> override + EXACT_OOMCLUMI_SWEEP;
(C) exact JSON FAIL -> not consumed; (D) count-based provenance -> fork rejects,
rc=1; (E) PENDING mode unchanged.

### 7e. Audit status (LANDED 2026-10-07) and remaining inputs

The mc-db provenance audit is COMPLETE (`NOZ_PROVENANCE_INVENTORY_2026-10-07.md`);
its five inputs are answered and independently re-verified 2026-10-08 (7b):
1. Exe verdict: `djangoh14_NoZ/djangoh14test` = 2023 "neu" NoZ build (sha256
   22e105df..., banner 10223 1316) — VERIFIED.
2. NOZ/NOZ1 GEN inventory: 260 chunks each (A-D x 65), ~154.9k events/chunk,
   ~40.3M written events per sample — VERIFIED (comparable to the old em nonrad
   pool, so numerator statistics match the existing legs).
3. Sigma/SIGMT: NOZ Born 6708.559 pb (SIGTOT 6.7086 nb); NOZ1 7285.933 pb —
   VERIFIED; author fLumi observations PROVISIONAL (fLumi x sigma ~ N_generated),
   interpretation delegated to h1-normalization.
4. Embedded LPIN configuration: NOZ = Born (LPIN(2)=0 ACTIVE), NOZ1 = corrected
   (LPIN(2)=1 ACTIVE) — VERIFIED (7b).
5. dst2all conversion provenance: H1OO 4.1.1 per-chunk steers over the dCache GEN
   files — VERIFIED.

Remaining input — **RESOLVED 2026-10-08**: numerator exposure L_MC delivered by
h1-normalization as **14,032,218 nb^-1 SAMPLED_ESTIMATE**
(`NOZBORN_EXPOSURE_DERIVATION_2026-10-08.md` +
`data/radiative/nozborn_exposure_2026-10-08/nozborn_exposure_2026-10-08.json`;
sigma_Born 6708.559 pb from oomclumi SigmaBorn in the NOZ hat run headers; count-based
formation REJECTED). It is now the `finalize_queue.tsv` default. The EXACT
260-chunk oomclumi weighted sweep (`exposure_sweep_20261008/`, prepared + smoked,
launch runbook Stage 3 in the campaign README) supersedes the estimate at finalize
time automatically (consumed at status PASS only; |rel diff| vs estimate gated at
1%, expected ~0.3%). **No blockers remain** — the chain is launch-ready as
prepared; nothing has been submitted.

### 7f. Session hygiene (updated 2026-10-08)

Nothing written under `/data/dust/group/h1/britzger/` (read-only throughout: dCache
GEN header dumps, validation-log reads + copies into OUR campaign `verification/`
dir, hat/mods/ods reads, one exe sha256/stat). Nothing submitted (`condor_submit`
never invoked; all queues/subs PREPARED only). The C_rad baseline release
(2026-10-05/06), the canonical plot packages, and the per-beam merged primitives
are untouched (primitives consumed read-only by the finalize smoke). The output dir
`djangoh14_nozborn_crad_final_xfcut_2026-10-07/` is a pristine skeleton (README
only) — a dry-run artifact written there by mistake during wrapper testing was
removed immediately and the dir re-verified. The `sidis-hq2-radiative` skill fails
to load in this runtime (attempted again 2026-10-08); STATE.md and the authoritative
docs were read directly instead. All new products PROVISIONAL DIAGNOSTIC,
`c_rad_applied=false`.

Exposure-prep session (2026-10-08, later): same hygiene holds. The oomclumi sweep
smoke + wiring tests only READ britzger paths (hat/mods chunks, the lcg-releases
oomclumi binary — mtime still 2025-04-12); find over both NOZ sample dirs shows no
file newer than the session start. All finalize wiring tests ran in a
`/tmp/opencode/noz_smoke/wiring_test_20261008/` sandbox — the real output dir remains
a pristine skeleton (README only; the FAIL-status dry-run aggregate stays LOCAL to
the sweep dir and the canonical exact JSON is written on PASS only). Nothing
submitted: `condor_q` shows no cluster from this session (one PRE-EXISTING held job
5167027 from 2026-09-21, a memory-held python3 job unrelated to this campaign, was
observed and left untouched).

Exact-closure session (2026-10-08, latest): same hygiene. The aggregator integration
+ finalize pre-flight only READ britzger paths (the four maxevents artifacts were
produced by h1-data-validator under the same read-only discipline; copies live in
OUR campaign results/ and the output dir). The finalize pre-flight ran in a
`/tmp/opencode/noz_smoke/exact_preflight_20261008/` sandbox — the REAL output-dir
product (estimate-scale, 10:04) was NOT touched and awaits the operator's Stage-4
rerun. Nothing submitted this session; the 7 held jobs observed in condor_q are all
pre-existing and unrelated (3x memory-held python3 from 2026-09-21, 4x the
2026-09-26 britzger_eminus smoke campaign).

### 7g. Campaign execution + exact-exposure closure (2026-10-08)

**Chain executed (2026-10-08, operator-launched between sessions):**
treebuilder 260/260 (rc=0, sha256_ok on all), xFcut worker 260/260 (verdicts PASS),
finalize at the SAMPLED_ESTIMATE exposure 14,032,218 nb^-1: **verdict PASS**,
contract PROVISIONAL_DIAGNOSTIC_PASS, integrated C_gamma_born h+ 1.047716 /
h- 1.046900, all gates G1-G7 True (G8 None by design), `c_rad_applied=false`,
exposure_source=SAMPLED_ESTIMATE. Interim product in
`data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/` (estimate-scale,
2026-10-08 10:04) — superseded the same day by the Stage-4 exact-exposure rerun
below; nothing estimate-scale remains on disk.

**Exact sweep — trailer blocker and resolution:** the oomclumi sweep passed
256/260; the four series-final chunks (A/B/C/D.C0000064) failed DETERMINISTICALLY:
each carries a benign end-of-production **trailer record** as its last H1Tree entry
(EventNumber = Unix timestamp in the Feb-2023 production window, empty GKI bank,
Q2Gki=0 outside the sample phase space) and oomclumi exits(1) on an empty GKI bank
(no skip option). NOT file corruption — legitimate partial final chunks (~87k events
each). Diagnosis, differential proof (`oomclumi -n N_phys-1` vs `-n N_phys`), and
recovery by h1-data-validator:
`data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/nozborn_lastchunk_gki_trailer_diagnosis_2026-10-08.md`
(+ machine record `validation_nozborn_lastchunk_gki_2026-10-08.json` and the four
genuine `oomclumi -n` artifacts, same dir; method validated vs oomclumi on 8 healthy
chunks and the 4 failing chunks).

**Integrated 260/260 exact aggregate (aggregator adapted 2026-10-08):** the four
records integrate with DISTINCT provenance (`trailer_skipped_oomclumi_maxevents`,
maxevents args 87079/86359/86307/87605, physics events, trailer index/timestamp,
validator references; exact uproot sumw as canonical per-chunk values, oomclumi
print agreement 3.5-5.4e-6 within the 3-decimal rounding envelope, tolerance 2e-5);
coverage fields distinguish 256 full-run + 4 trailer-skipped; all original gates kept.
**Result: status PASS — total sumw 94,718,983.184259, L_MC = 14,119,125.013920
nb^-1** (sigma 6708.559 pb consistent + matches ref; identity 5.7e-12; +0.6193% vs
the estimate = +2.38 sigma — the estimate's anchor extrapolation under-allocated the
four partial final chunks; inside the 1% hard gate, flags documented). Canonical
exact JSON at
`data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/nozborn_exposure_exact_2026-10-08.json`
(status PASS; provenance label EXACT_OOMCLUMI_SWEEP with the 4 trailer-skipped
chunks recorded). Print-formation crosscheck 14,119,125.000 (immaterial 0.014 nb^-1).

**Finalize rerun — EXECUTED (Stage 4, 2026-10-08 11:23, Condor cluster 5413894,
rc=0):** the wrapper consumed the PASS exact JSON automatically and overrode the
14032218 queue value (queue file unchanged by design — documented fallback; log
line "exact oomclumi sweep PASS -> exposure 14119125.013920162 supersedes passed
value '14032218'" per `logs/f.5413894.0.out`, normal termination return value 0
per `logs/f.5413894.log`). The rerun OVERWROTE the estimate-scale product files
in the output dir with the exact-exposure product — the on-disk product in
`data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/` IS the
EXACT_OOMCLUMI_SWEEP result: **verdict PASS, contract
PROVISIONAL_DIAGNOSTIC_PASS, integrated C_gamma_born h+ 1.041267 / h- 1.040456,
L_MC = 14,119,125.013920 nb^-1**, all gates G1-G7 True (G8 None by design),
`c_rad_applied=false`, `exposure_source=EXACT_OOMCLUMI_SWEEP` — matching the
sandbox pre-flight prediction (machine-precision 1/L rescale of the
estimate-scale product, which had observed 1.047716/1.046900 at L=14,032,218)
and inside the G6 band [0.95, 1.12], which still excludes a NOZ1-as-NOZ mixup
(~1.136). Nothing estimate-scale remains on disk; the campaign README runbook
(Stage 4 DONE) and the output-dir README record the same completion state.

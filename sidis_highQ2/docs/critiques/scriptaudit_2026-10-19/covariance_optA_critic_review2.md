# CRITIC REVIEW 2 of 2 — Independent Verification, Covariance Re-Basing to OPT-A Nominal (quoting condition (d)), 2026-10-20/21

Auditor: physics-correctness critic, read-only; recomputation in independent JS rather than trust of any summary. Skill sidis-hq2-systematics not loadable from critic context (recorded precedent); review on first principles + family criteria embedded in the evidence.

**VERDICT: PASS-WITH-CONDITIONS — Review 1's PASS-WITH-CONDITIONS CONFIRMED.** No claim of Review 1 refuted; every load-bearing number independently recomputed and reproduced. Condition (d) can be closed, with the conditions in Section 6 entering the promotion record. Package physics SOUND at the achieved verification depth, subject to the carried conditions and the coverage limitation in Section 5.

## Per-claim verification of Review 1 (all 8 items)

**Item 1 — 19-column c1fix quadrature; defective env100 total absent: CONFIRMED.** All 490 bins recomputed: 19-col quadrature vs total_with_env100_c1fix_rel max |dev| 2.2204e-16 (worst neg/174); hypot(c1fix, straddle_residual) vs total_quoted_rel deviates exactly 0 (490/490); audit-path hypot dev 0; m2 all-zero (0/490); lumi flat 0.027 (490/490); acceptance in (0,1] all 490 (0.0179-0.765). Defective-total absence: frozen 21comp stored total differs from quoted-based prediction by up to 3.53e-5 (neg/240; pos/109 2.88e-5), while package diagonals match the quoted-based G2 prediction to 1.5e-15 — ten orders of magnitude discrimination; defective tree unreachable (40 manifest inputs, 0 paths touch systematics_bgsub_env100_21comp_2026-10-19; string "21comp" absent from all input paths). PIN audit resolved: 24 PIN-dict entries (builder lines 188-216) vs 40 recorded inputs — Review 1's "24" is the PIN dict, not the manifest count; no discrepancy.

**Item 2 — cov_straddle exact 2x2 block structure: CONFIRMED.** Builder lines 876-881 (per-gen-bin 2x2 cross-charge fill into zeros-initialized 490x490); custody record == S8 CSV twin == budget column bitwise 490/490 in all three pairings (pins pos[0]=0.0001254872133138931, neg[0]=0.00014780805205009094). Binary access impossible with this toolset; substituted discriminating eigen-fingerprint: reconstructed expected block form max_eig 7.528904862563573e-7 vs G1-reported 7.528904862563585e-7 (1.5e-15 rel), rank 245, diag_min 0; wrong forms excluded numerically (full outer product -> max_eig 2.477e-6 rank 1; diag-only -> 4.127e-7). Empty-D sides exactly zero.

**Item 3 — C_stat diagonal, alpha lineage, borderline PSD: CONFIRMED.** sqrt(diag)/sigma vs mag_ibu_stat_rel max rel dev 3.74469e-16, 359/490 bitwise. Per-charge blocks bitwise-identical to sidecars (60025/60025 elements per charge; seeds 12345/22345, 1000 toys); cross-block verified at element level (implied rho bin00 0.7036132427110172; max |rho| 0.7776272550494123). Alpha lineage: frozen 0.22220732740149143 -> OPT-A 0.2222835111081017 (delta 7.618e-5, +0.0343% rel), same pre-registered fallback (i); raw D4 graft (min_eig -3.052e6) never published. Borderline PSD recomputed: min_eig/scale -9.999996598552697e-13, floor -1e-12 x scale = -2.7300837783801426e-6, margin above floor 9.28623610621423e-13 counts^2, min_eig in nb^2 -2.216e-17 — one effectively-null direction, inside the pre-registered tolerance, numerically zero.

**Item 4 — G2 attribution identity + lumi peel: CONFIRMED.** Independent re-derivation of the full identity (diag^2(pkg with-lumi) = quoted^2 - fakecol^2 + mcstat^2 + (detint_model^2 + detint_stat^2 - detint_removal^2) + sum_5(s8col^2 - budcol^2)): max rel resid 1.50e-15 with-lumi (worst pos/64), 1.74e-15 no-lumi (worst pos/133), 0/490 unattributed both; lumi peel max dev from 0.027^2 = 4.24e-16; 21-term quadrature closure 4.4e-16. Implemented formula (builder lines 1035-1048, gate dict 1066-1082) matches the re-derivation line-for-line.

**Item 5 — units-fix preflight arithmetic: CONFIRMED.** Builder line 759 /(sigma * LUMI) with the 5803888 post-mortem comment; fixed-form max dev 5.551115123125783e-17 (pos), matching claim exactly; buggy sigma-only drift diag_rel x (L-1) = 0.316033 x 351000 = 1.109e5 at pos/179 — the observed failure magnitude exactly. G3 stale-lumi guard rejects "297657.8", requires LUMI == 351000.0.

**Item 6 — pos/b179 empty-D exemption: CONFIRMED.** pos/179 quoted 110.5935% (budget_total_quoted_rel_reference[179] = 1.105935002740245, bitwise at all 3 empty-D bins); over-100 census on all 9 lineage totals: exactly 1 bin each (pos/179), 0 populated violations.

**Item 7 — registry + protected trees: CONFIRMED.** Both canonical RUNLIST.md copies whole-file byte-identical (342 lines); OPT-A covariance entry at line 329 in both (7264 chars); budget @309, baseline @289 identical. All recomputed registry numbers match: public quote medians 7.3121%/7.6243% (0.0731214500309655/0.07624298168459993), package totals 7.2480%/7.4952% (0.07248028673214645/0.07495191629800207), C_MCstat medians 0.8548%/0.9252%, straddle medians 0.1285%/0.1245%, fake-cell census 472/487 pos and 460/484 neg (verbatim in lineage/cov_mcstat.json; s09b docstring 461/484 is the frozen-wave S3 value, not a discrepancy). G5: 20 protected trees + 9 pinned files, pre 16695 -> post 16767 (72 new DEST files), drifted 0 / missing 0 / extras_outside 0; DEST inventory 76 = 63 top-level + 13 lineage, unaccounted 0, phantom 0.

**Item 8 — coverage beyond Review 1: CONFIRMED, with findings.** Full builder diff (82 hunks; 952 optA-only / 397 frozen-only, net +555) fully accounted: only gate-region changes are the documented condition-(d) re-basing, the two clone-side fixes (both in NEW optA code: units fix line 759; SPECIAL_BLOCK_TWINS lines 635/638), and the ga10 guard as a pure 142-line addition. The G5 gate block is byte-faithful (optA lines 1493-1528 == frozen 1056-1091). 7.1 env pin verified in run_s09.sh (LCG_107a view + OPENBLAS/OMP/MKL/BLIS_NUM_THREADS=1); 7.5 log redirection verified in s09.sub. ga10 fp-signatures {mag_ibu_stat_rel} vs C_stat and {} vs C_MCstat confirmed in validation.json, equality independently verified (3.74e-16 << 1e-12); duplicated-variance fractions 13.2659%/12.9635%. Unpropagated register 12 entries; row 20 (response body/miss MC-stat, formerly the GAP) CLOSED_BY_THIS_PACKAGE (superseded 1.4769%/1.5752% -> measured 0.8548%/0.9252%, both recomputed exactly); S7 REPORT_ONLY_UNPROPAGATED_FROZEN_OLD_BASIS (gate FAIL, exceed 8.8246%/9.5431% > quoted 7.3121%/7.6243%); m2 DOCUMENTED_JUSTIFIED_ZERO; bgsub_stat RETIRED; b1 FAIL_CARRIED_REPORT_ONLY. README consumption rules 1-9 verified in the builder incl. rule 5 (straddle fixed 2x2 block, not a shift vector), rule 6 (borderline-PSD guidance), rule 9 (never quote package diagonals as the public systematic).

**Refuted claims: none.** Largest deviations anywhere at fp-identity level (<= 4.4e-16) except the intended differences fully attributed by G2.

## Minor findings (documentation wording, non-blocking)

1. Validation report §2 "All 40 s09 inputs are sha-pinned in the builder PIN dict" imprecise: 24 inputs PIN-verified with hard-stop (all load-bearing physics inputs); 16 sha256-recorded at run time without pre-registration (docs/provenance references, aux study records, Born CSV — the latter cross-validated by G3 at 1.6e-15 and G5-swept). No physics exposure; fix the sentence.
2. Validation report §3 "lineage/ (11)": 13 files = 11 non-npy + 2 verbatim npy (counted within the 56 matrix npy); totals close at 76. Wording only.

## Missing Evidence (coverage limitations)

- Byte-level .npy verification impossible with this toolset (binary reads rejected). Mitigations: table twins verified bitwise; eigen-fingerprint discrimination for cov_straddle; G6 exact reconstruction; G2/G4 cross-checks. Residual risk: a corrupted binary artifact not covered by a text twin could evade this review — low, nonzero; the one gap a third pass could close (see Recommended Next Check).
- Preflight md5 not independently recomputed (procedural); preflight ARITHMETIC claims independently reproduced.
- ga10 block equality with s09b at criterion/outcome level, not byte level (integration restructuring).

## Conditions entering the promotion record (closing condition (d))

1. C_stat borderline PSD carried: min_eig/scale -9.9999966e-13 vs pre-registered -1e-12 floor, margin 9.29e-13 counts^2, |min_eig| 2.2e-17 nb^2 (numerically zero). Consumers follow README rule 6 / g8 (eigen-decompose or regularize; never invert blindly); reproduction requires the pinned environment (LCG_107a numpy 1.26.4, single-threaded BLAS).
2. Package-vs-quote separation (README rule 9): package diagonals NEVER the public per-bin systematic; public quote stays the c1fix budget total_quoted_rel (7.3121%/7.6243%, untouched).
3. Empty-D marker to plotter + note: 3 exempt bins (pos/109, pos/179, neg/240); the ONLY quoted bin above 100% is h+ b179 at 110.5935% (empty-D exempt); 0 populated bins above 100%.
4. S7 truth-reweight FAIL carried report-only (frozen old basis; exceed 8.8246%/9.5431%) and b1 FAIL carried report-only remain in the note's carried-open list.
5. Plotter band re-pointing contract: consume cov_total_with_lumi (21 terms) by default or the no-lumi twin + own lumi; never re-add cov_fakecol_mcstat; straddle fixed cross-charge block; C_MCstat cross-block zero (D5), C_stat cross-block measured alpha = 0.2222835111081017; join on (charge, bin_index), never bin midpoints.
6. Quoting remains BLOCKED (M1, N2, plotter band re-pointing, note); this package closes ONLY condition (d).
7. Record wording fixes: the two documentation imprecisions above.

## Recommended Next Check

Single numpy-level byte audit of every .npy in the package re-asserting the published identities in one pass — diag(cov_total_with_lumi) == sum of the 21 term diagonals to fp; corr twins == C/(sigma sigma); rel twins == C_nb/(sigma sigma); counts^2 twins == nb^2 x L^2; cov_straddle == the record-reconstructed 2x2 block. 5-minute login-node check or one-job Condor pass; route to h1-data-validator. No physics issue expected: every text-layer trace already closes at <= 4.4e-16.

--- audit metadata ---
- Report persisted by the delegating supervisor session (critic toolset read-only), 2026-10-21.

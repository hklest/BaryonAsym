# Numerical audit: 12 signed rank-1 source columns — S1 clean-release covariance build
Date: 2026-10-18 | Auditor: physics-correctness critic (numerical audit delegation; session could not create files — content delivered inline and transcribed verbatim by the supervisor)
Package: covariance_cleanrelease_2026-10-17 | Overall verdict: CONCERN

Target: H1ChargedSidis_HighQ2/output_v6/studies/covariance_cleanrelease_2026-10-17/signed_deltas/delta_k_columns.csv
(490 rows, 59 columns, charge-major pos-first). Method: exhaustive read-only arithmetic over all
490 rows via direct file access; no analysis product recomputed. Cross-checked against
signed_deltas_record.json, bgsub_setrprime_budget19_cleandenom.csv,
bbb_ibu_.../ibu/cross_sections.csv, shard_bootstrap/z_perbin.csv, significance_report.json,
launch_status_s04.json. All numbers below are from the delivered files, recomputed independently.

## Roles (per column_registry — the delegation's "12 rank-1 columns" split)
Rank-1 S9 inputs: hfs_jes, hfs_rces, hfs_theta, elec_energy_sym, elec_theta_sym, ibu_model,
ibu_reg, detint_model, c_rad_gen, lumi (peel). NOT rank-1: delta_tracking (D7: diagonal,
kind signed_delta_informational_diagonal), delta_c_rad_gen_rawhalf (D8: raw retained custody
twin, kind signed_delta_raw_retained). Diagonal-only: all mag_* columns. Reconciliation-only:
elec_*_env twins.

## Per-column verdict table

| column | verdict | median |delta|rel h+/h- | budget-repro max dev | worst z-inconsistency |
|---|---|---|---|---|
| delta_hfs_jes | ANOMALY(MINOR) | 0.308% / 0.351% | 0.0 (exact) | pos b179: 3.26% (10.6x med) at z=1.26 |
| delta_hfs_rces | SOUND | 0.260% / 0.264% | 0.0 (exact) | neg b207: 1.60% (6.1x med) at z=1.92 |
| delta_hfs_theta | SOUND | 0.252% / 0.293% | 0.0 (exact) | neg b242: 1.04% (3.6x med) at z=0.93 (pos: none) |
| delta_elec_energy_sym | ANOMALY(MAJOR) | 1.237% / 1.355% | 0.636 vs budget (env twin 0.0) | pos b179: 68.6% (55x med) at z=1.81 |
| delta_elec_theta_sym | **BLOCKER** | 0.243% / 0.248% | 0.301 vs budget (env twin 0.0) | neg b244: 7.22% (29x med) at z=1.39 |
| delta_ibu_model | ANOMALY(MAJOR) | 1.395% / 1.425% | 1.25e-16 | n/a (no bootstrap z; sign dom 51%) |
| delta_ibu_reg | ANOMALY(MINOR) | 0.540% / 0.560% | 1.11e-16 | n/a (no bootstrap z; max 30.0% @ pos b179) |
| delta_detint_model | SOUND | 0.030% / 0.029% | 1.30e-18 | n/a (no bootstrap z) |
| delta_tracking | SOUND (D7 diagonal, informational) | 0.513% / 0.521% | 0.0 (exact) | n/a (diagonal; coherent-prior sign basis) |
| delta_c_rad_gen | SOUND | 0.584% / 0.580% | 0.0 (exact) | n/a (no bootstrap z) |
| delta_c_rad_gen_rawhalf | ANOMALY(MINOR) | 0.462% / 0.526% | 0.031 vs budget crad (raw vs smoothed, by design) | n/a (retained custody twin) |
| delta_lumi | SOUND | 2.700% / 2.700% (flat) | 0.0 (exact) | n/a (fully correlated peel) |

Anomaly counts: BLOCKER 1; ANOMALY(MAJOR) 2 (elec_energy_sym, ibu_model); ANOMALY(MINOR) 3
(hfs_jes, ibu_reg, rawhalf); SOUND 6. Plus 1 package-level MAJOR (S9 input-list ambiguity,
cross-column).

## Non-Blocking Checks Confirmed (exhaustive, all 490 rows)
- C1 structure PASS: 490 rows; 59 fields on every row; 0 empty cells; 0 non-finite values in all
  57 numeric columns; pos(0-244)/neg(245-489) blocks pure and sequential; (charge,bin) join vs
  budget complete with 0 global_bin_index and 0 cell_label mismatches.
- C5 PASS: nb = rel x sigma exactly for all 24 rel/nb pairs (max rel dev 0.0, 0 sign mismatches);
  sigma bit-identical to release cross_sections.csv cross_section_nb (max dev 0.0 over 490),
  vs budget sigma max dev 6.66e-16; all sigma > 0 (record gate: min 1.436037e-05 nb).
- C4 PASS: 21/21 budget mappings reproduce at <= 1e-12; 15 bit-identical (0.0), ibu_model
  1.249e-16, ibu_reg 1.110e-16, detint_model 1.30e-18, detint_stat 1.69e-21, detint_comp
  1.30e-18; no negative budget magnitudes. Matches the record's gate claim exactly.
- C3 PASS (rank-1 PSD by construction, numerically demonstrated per column): M = delta delta^T
  satisfies M[i][j] = delta_i delta_j exactly; v'Mv = (v.delta)^2 for random v; min diagonal =
  min delta_i^2 > 0 (1.5e-20 ... 1.5e-13); ||delta||^2 > 0. Over the combined 490-vector each
  source is fully correlated across h+/h- (intended for common detector/model sources; exact
  for lumi).
- C8 PASS: flag_ibu_model_floor_binds 152(pos)/167(neg) == record; flag_sdl_hfs_{jes,rces,theta}
  = 1 on all 490 rows; flag_tracking_sign_basis_coherentprior = 1 on all rows;
  flag_c_rad_gen_smooth_floor_bind 130+142 = 272 == record.
- C9 PASS: h+/h- median |delta|rel ratios 0.86-1.04 (jes 0.877, theta 0.859, rawhalf 0.878,
  lumi 1.000) — no unexplained charge asymmetry.
- Record consistency: all 12 context_stats combined signed medians and all 12 max_abs values
  match the exhaustive recomputation (e.g. tracking -0.0048876/0.0159625; elecE_sym
  0.00042705/0.6861753; ibu_model -0.0026354/0.1818058); S4 budget_column_median matches the env
  medians exactly (1.648%/1.706%; 0.461%/0.519%); delivered sym columns are bit-identical to the
  S4 sym z-sources (490/490); grid is 7 cells x (7 z x 5 pT) = 245 bins per charge.

## Fatal Issues
None. No demonstrated wrong published number; the arithmetic layer (magnitude, nb rule,
budget reproduction, PSD) is exact.

## Major Concerns
1. delta_elec_theta_sym — BLOCKER for S9 rank-1 consumption as delivered.
   Median |delta| 0.243%/0.248% vs env twin 0.461%/0.519% (median per-bin ratio 0.506/0.502).
   S4: noise_dominated both charges (frac |z|>=2 = 0.445/0.404 < D6 threshold 0.5; frac |z|<1 =
   0.298/0.367), smooth_recommended both charges, D6 decision DEFERRED, raw retained.
   C7: 29 (pos) / 34 (neg) bins with |delta|>2x med but |z|<2 (pos b242 6.35% @ z=1.62, b240
   6.25% @ z=1.46; neg b244 7.22% @ z=1.39, b240 6.89% @ z=0.79); max 31.3% at pos b179.
   Sign pattern coin-flip (dom 51%, 15-16 changes/35-bin cell). Mechanism, quantified: the
   +1/-1 mrad legs respond SAME-SIGNED in 66%/68% of bins (161/245, 167/245; ~5 sigma above
   chance) — a real common-mode response that the D1 half-spread (s_plus - s_minus)/2 cancels.
   Consuming delta delta^T raw under-covers elec_theta ~2x vs the published budget and injects
   bootstrap-noise sign structure into correlations. The raw column must not enter S9 until D6
   is taken (smooth per S4 recommendation, or substitute the budget-exact env twin / menu-1sigma
   after S6/S8). Sources: delta_k_columns.csv (col delta_elec_theta_sym_rel),
   significance_report.json (sources/delta_elec_theta_sym_rel, smoothing_recommendation),
   z_perbin.csv.
2. delta_elec_energy_sym — ANOMALY(MAJOR): budget-vs-covariance convention gap, documented and
   gated but unresolved. Median |delta| 1.237%/1.355% vs env 1.648%/1.706% (per-bin median ratio
   0.758/0.728); 164/490 bins |sym|>budget, 75 bins >2x budget, 72 bins <1/4 budget; max dev
   0.636 abs at pos b179 (sym 68.6% vs env 5.0% = 13.7x). S4: intermediate both charges
   (frac |z|>=2 = 0.555/0.580), raw_ok. C7: 15/19 A-bins (pos b179 68.6% @ z=1.81; neg b209
   44.5% @ z=1.47). +-leg sign agreement 40%/44% (below chance: mostly linear/antisymmetric
   response, minority common-mode). The released budget quotes elec_energy at the single-leg
   envelope convention (supervisor 2026-10-16); the S9 rank-1 input carries the half-spread:
   the covariance elec_energy term is ~25% smaller (median) than the published budget and
   differs per-bin up to 13.7x. D2 menu-1sigma NOT applied (gated on S6 linearity); S8
   reconciliation pending. Not budget-consistent until S6/S8 close.
3. delta_ibu_model — ANOMALY(MAJOR): noise-suspect sign basis on the largest source.
   Median 1.395%/1.425% (largest single component), max 17.7%/18.2% (b244/b240), budget-exact.
   D3: at floor-bound bins (152/167 = 319/490 = 65%) magnitude from the B1 floor, sign from the
   signed RG/DJ half-diff — i.e. the sign is taken from a quantity whose magnitude is BELOW the
   floor (that is why the floor binds; gate: budget == max(half,floor)). Exhaustive sign
   pattern: dominant sign 51.0%/51.4%, 15-16 changes per 35-bin cell — indistinguishable from a
   coin flip. Diagonal (variance) is budget-sanctioned and exact; the rank-1 correlation
   structure of the largest systematic is sign-random at ~65% of bins, which can materially
   distort shape/difference uncertainties. No bootstrap z exists for ibu_model, so noise cannot
   be confirmed or excluded from existing evidence. Decisive follow-up: extend the shard
   bootstrap to the RG/DJ half-diff.
4. Package-level — S9 input-list ambiguity (MAJOR). Record notes: "delta_*_nb columns are the
   S9 rank-1 inputs (delta_k delta_k^T); mag_* columns are diagonal-only; delta_lumi is the
   rank-1 peel." The column_registry overrides this for delta_tracking (D7 diagonal) and
   delta_c_rad_gen_rawhalf (D8 retained). An S9 consumer keyed on the delta_* prefix would
   (a) double-count c_rad_gen (rawhalf norm^2 2.11e-4 vs smoothed 2.20e-4, identical signs at
   all 490 bins — adding both rank-1 terms ~doubles that source's variance), and (b) violate D7
   by rank-1-izing tracking. S9 must consume an explicit enumerated list: rank-1 = {jes, rces,
   theta, elecE_sym (pending S6/S8), elecTheta_sym (BLOCKED pending D6), ibu_model, ibu_reg,
   detint_model, c_rad_gen}; diagonal = {tracking, mag_*}; excluded = {rawhalf, env twins}.

## Minor Concerns
5. delta_hfs_jes — mixed sign basis at 5 bins. Magnitudes bit-exact vs budget; signs from
   UNSUBTRACTED stage-3 rapgap31 legs (documented deviation: subtracted-chain signed vectors
   are not retained anywhere). Exhaustive comparison vs S4 bootstrap leg hfs_jes_down: 485/490
   sign agreement; 5 exact-negation bins with bit-identical magnitudes: pos {26 (+0.0063%),
   69 (-0.319% vs leg +0.319% — at median scale), 109 (-0.029%), 133 (+0.010%)}, neg {113
   (-0.028%)}. Launch gate records this (pos 241/245, "sign flips are recorded, not gated").
   delta_hfs_theta has 1 flip at pos b197 (|delta| = 2.5e-6, negligible); rces 0 flips
   (490/490, max diff 1.66e-15). Impact confined to correlation signs at small-|delta| bins;
   record the flip-bin list in S8.
6. delta_ibu_reg — same noise-sign class as ibu_model (raw k5-k3 half-diff sign, no floor;
   dom 53.9%/53.5%, 13-14 changes/cell), 2.6x smaller median; violent sparse-bin excursions
   (30.0% pos b179, 13.9% neg b235) inherited bit-exact from the released budget (budget-level
   feature, not an S1 artifact). Include in the extended bootstrap.
7. delta_c_rad_gen_rawhalf — column is faithful to its documented retention role (0.5 x sidecar
   rel_signed; |.| differs from budget c_rad_gen_rel at all 490 bins, max 0.0314, by design
   raw vs smoothed; signs identical to delta_c_rad_gen at 490/490). Risk is consumption (see 4).
8. Wording/registry: mag_detint_comp entry says the covariance package uses "detint_model
   (rank-1, S4-validated)" — no detint leg exists in the S4 shard bootstrap (legs: hfs_jes_down,
   hfs_rces_up, hfs_theta_up, elecE_p1pct/m1pct, elecTheta_p1mrad/m1mrad); "S4-validated" is
   inaccurate. Also note: all four elec legs themselves have weak sign coherence (dom 51-66%,
   11-16 changes/cell) and elecE leg fixed shifts (medians 0.13-0.32%) sit at or below their own
   bootstrap sigma (~0.55%) — the electron-scale response is statistics-limited per bin, which
   is the common root of findings 1-2. Sparse high-z/pT corner bins (b174/179/206-210/235-244)
   host every column's max |delta| and are mostly noise-consistent (z<2); a D6-consistent
   smoothing policy for tails would reduce covariance noise.

## Missing Evidence
- No bootstrap z for ibu_model, ibu_reg, detint_model, c_rad_gen, tracking, rawhalf (S4 covers
  only 3 HFS + 4 elec legs) — sign-basis noise classification for those sources is untested.
- S9 consumption manifest not supplied — cannot confirm whether rawhalf/tracking/sym columns
  were actually consumed per the notes' blanket delta_* rule (finding 4 is a risk, not a
  demonstrated error).
- S6 linearity check and S8 reconciliation outputs (pending) — needed to close the elec
  convention gap (findings 1-2).
- B1 floor (floor_rel) and V3 smooth semantics taken from the record; not independently
  re-derived (read-only scope).

## Recommended Next Check
Inspect the S9 input manifest and take the D6 decision on delta_elec_theta_sym: do not let the
raw noise-dominated column enter delta_k delta_k^T — apply the S4-recommended smoothing or
substitute the budget-exact elec_theta_env twin pending S8; simultaneously confirm rawhalf is
excluded and tracking is diagonalized (finding 4).

## Prioritized fix list
1. BLOCKER: bar delta_elec_theta_sym_nb from raw S9 consumption; take D6 (smooth recommended
   both charges) or use env twin / menu-1sigma after S6/S8.
2. MAJOR: close the elec_energy budget-vs-covariance gap (S8 + D2/S6); sym median 0.74x env,
   per-bin to 13.7x.
3. MAJOR: bootstrap the ibu_model (and ibu_reg) half-diffs to classify sign/magnitude noise;
   65% of ibu_model bins carry signs from sub-floor raw differences, pattern is a coin flip.
4. MAJOR: make the S9 input list explicit (exclude rawhalf; diagonalize tracking) — the notes'
   blanket delta_* rule contradicts the D7/D8 registry kinds and would double-count c_rad_gen.
5. MINOR: record the 6 unsubtracted-vs-subtracted sign-flip bins (jes pos 26/69/109/133, neg
   113; theta pos 197) in S8.
6. MINOR: correct the "S4-validated" detint_model provenance wording in the registry.
7. NOTE: adopt a D6-consistent tail-smoothing policy for sparse-bin excursions (b174/179/206-
   244), which dominate every column's max and are mostly noise-consistent (z<2).

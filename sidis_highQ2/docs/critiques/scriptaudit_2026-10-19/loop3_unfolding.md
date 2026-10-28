# Loop 3 — Final Adjudication: Unfolding Procedure (critique loop 3 of 3)

**Script audit 2026-10-19 · sidis_highQ2 · v6/z005 · cone+S2′+Set-R′ clean release (2026-10-17 FINAL)**
**Verdict: `SOUND` — overall PASS for the unfolding procedure. 0 CRITICAL, 0 MAJOR; 8 confirmed
findings, all MINOR-level (1 UNFOLD-NUMBER, 2 PROVENANCE, 1 MAINTENANCE, 4 DIAGNOSTIC-ONLY).
None corrupts a released central value; none corrupts the quoted 1000-toy statistical sidecar;
none blocks the current release.**

Loops 1 (SOUND, 3 MINOR) and 2 (SOUND upheld, 5/5 confirmed, +2 MINOR, +6 INFO) are incorporated;
confirmed items are not re-litigated. This loop closed the two open decisive questions (the S5f
consumer direction convention; the TUnfold second-method chain integrity), swept the residual
unaudited consumers, and bounded the treebuilder-M1 exposure into the unfolding layer.

## Material Reviewed (loop 3 only)

- **S5f consumers:** `compare_signsym_minus_vs_plus_covlegs_2026-10-17.py` (S5f record builder,
  full read); `s08_symmetrize_and_reconcile.py` (S5f budget consumer, delta-construction sites);
  `s01_build_signed_deltas.py` (adjacent elec two-sided delta); `validate_smoke_covlegs.py`
  (direction tags); `run_bbb_ibu_covlegs_stage3_2026-10-17.py` (docstring/grouping sites);
  `run_cone_systematics_envelope_successor_env.py` (magnitude-only consumer, grep-verified).
- **TUnfold native v6 chain:** `unfold_native_miss_single_tau_v6.C` (full); `build_native_miss_inputs_v6.py`,
  `validate_native_miss_inputs_v6.py` (full); `produce_phase2_native_diagnostics_v6.py`,
  `finalize_native_phase2.py`, `compare_native_tau0_vs_ibu_v6.py`, `regenerate_tau0_native_roots.py`
  (full); the v1–v5 second-method chain: `run_tunfold_s2prime_study.py` (+ README, + study summary
  JSON), `run_tunfold_s2prime_v2_nonneg_study.py` (clip), `run_tunfold_s2prime_v4_syssolve_study.py`
  (TUnfoldSys), `compute_v5_consistency.py`; STATE.md P-METHOD record (L67).
- **Residual sweep:** `postcheck_release_cone_s2prime.py` (full) + its run artifacts
  (`release_driver_stub/run/postcheck.json`, `isolation_compare.json`);
  `isolation_snapshot_cone_release.py`; `run_bgsub_setrprime_smoke_one_cone.sh` + run record;
  stage-2 tree residuals: `run_bgsub_gate_cone.py`, `run_bgsub_budget19_cleandenom_cone.py`,
  `measure_bgsub_stage_rel_cone.py` (headers + key sites), spec/pins and superseded
  `extend_budget_15to19` (pin-level); kinematic-branch grep of both cone builders (M1 bound).

## Part A — Residual Coverage

### A1. S5f sign-symmetry consumer — DECISIVE QUESTION RESOLVED: pairs by PHYSICAL DIRECTION; no sign inversion anywhere

1. **The S5f record builder re-pairs by shift direction, not custody**
   (`compare_signsym_minus_vs_plus_covlegs_2026-10-17.py`): `LEG_UP/LEG_DOWN` maps (L92-95) pair
   `hfs_jes_up`/`hfs_jes_down` etc.; the signed symmetrized delta is `delta = (u_up - u_dn)/2`
   (L295) with up/down = SOURCE SHIFT DIRECTIONS. The naming trap is spelled out in the docstring
   (L19-23) and in the record itself (`d1_leg_name_mapping` + `budget_plus_legs_note`, L470-482:
   "for hfs_jes the budget plus leg is the DOWN-direction leg"). Direction tags in
   `validate_smoke_covlegs.py` L84-88 (`hfs_jes_up: +1`, `hfs_rces_down: -1`, `hfs_theta_down: -1`,
   mirroring the plus-leg precedents jes_down/−1, rces_up/+1, theta_up/+1) are consistent with
   this mapping.
2. **The budget consumer consumes the pre-computed direction-paired columns**
   (`s08_symmetrize_and_reconcile.py`): the published `delta_hfs_jes_sym_rel` is taken directly
   from the S5f record's `delta_k_rel` (L223-226); the rces/theta SAME_DIRECTION envelopes are
   built from the direction-labeled per-leg `s_up_rel`/`s_down_rel` with dominant-leg sign
   (L236-247). The A8 cross-check (L194-217) pairs by the installed budget legs
   (jes_down/rces_up/theta_up) but uses them for magnitude bit-exactness only
   (sign-insensitive); its sign comparison enumerates the six known A8 flip bins verbatim.
3. **Adjacent consumers are also direction-safe:** `s01_build_signed_deltas.py` elec sym delta
   pairs the physically-named legs (`elecE_p1pct`/`elecE_m1pct`, L127-128) with per-leg empirical
   signs (L328-338); the shiftedlegs envelope successor uses magnitudes only.
4. **No consumer of the covlegs PLUS/MINUS grouping forms a signed delta** — grep across
   `scripts/` for `U_plus|U_minus|PLUS_LEGS|MINUS_LEGS` returns only the stage-3 producer
   (custodial grouping, unsigned comps only — loop 2), the S5f builder (direction-paired), and
   magnitude-only envelope code.

**Adjudication of loop-2 MINOR-2:** the JES sign-inversion risk does NOT materialize in any
consumer. The defect reduces to the stage-3 driver docstring (L13), which invites the wrong
reading. Class: MAINTENANCE (documentation). Fix: one-line docstring correction to
"(U_up − U_down)/2 with up/down = source shift directions; see the S5f record D1 mapping".

### A2. TUnfold native v6 diagnostic chain — DIAGNOSTIC-SOUND; no cross-check-invalidating defect

**June base machinery:**
- **Input geometry:** builder produces TH2 (490 reco × 245 truth) with the miss row rerouted to
  x-underflow bin 0 (kHistMapOutputVert), fake column excluded; data = raw × keep-frac with
  keep = matched/(matched+fake) = 1−f_i (same formula as IBU); errors scaled by the same frac.
  Geometry validator enforces 490/245, miss exclusively in x-underflow, zero truth under/overflow,
  all 245 matched+miss denominators finite-positive. The .C macro hard-FATALs on bad dimensions,
  miss outside x-underflow, and `SetInput` status != 0 (L48-54); phase-2 raises on the same
  (L97-103).
- **tau handling:** the .C takes tau as an argument (phase-1 comparisons ran tau=0); the phase-2
  TAUS grid includes 1e-5 through 3e-2 with active curvature regularization; the finalize manifest
  is `PASS_WITH_CAVEATS` with Poisson closure explicitly PENDING and closure labeled
  `deterministic_arithmetic_closure_only_not_poisson_validation` throughout — honest.
- **delivered = max(x,0):** implemented in the v2 chain (not the June study): `x_nominal =
  max(x(tau_sel), 0)` with the unclipped vector retained, a fold-operator hard gate ≤1e-10
  (manual column-normalized response INCLUDING the miss x-underflow vs `GetFoldedOutput`),
  one-sided per-bin clip bias |x_neg| propagated, integrated clip bias stated
  (+0.028%/+0.011%), and the STATE.md caveat that the delivered vector is NOT a well-defined
  estimator (post-hoc clip, unclipped covariance) — promotion to nominal ruled UNSOUND without a
  constrained re-solve. Honest and fully accounted.
- **v4 covariance-in-solve:** `run_tunfold_s2prime_v4_syssolve_study.py` mirrors the 15-comp
  budget into the solve via G = GetDXDY with a HARD GATE that the delivered central vector stays
  bit-identical to v2/v3; the TUnfoldSys twin is asserted identical (twin_dev == 0.0); routes
  that would pollute the solve weighting fVyy (SetInput(hist_vyy), SubtractBackground) are
  documented and rejected with ROOT 6.38.04 source citations; the v4 FINDING that v3's
  "stat" (GetEmatrixTotal on TUnfoldDensity, which IS a TUnfoldSys) also contained
  response-body/miss MC-stat is decomposed and reported (this also explains, and supersedes, the
  v1 stat-ratio ~2.1× vs the IBU toy sidecar).
- **tau=1e-5:** the v1 TAU_RULE (largest plateau tau: chi2/ndf ≤ 1.01×tau0, neg_bins ≤ tau0,
  |integral shift| ≤ 0.5%) selected tau_sel = 1e-5 (rapgap, both charges; djangoh own-tau 3e-5,
  headline same-tau = 1e-5); v4/v5 pin TAU = 1e-5 with bit-identity gates; v5 pins the expected
  refold chi2/ndf values matching the v1 record exactly.
- **Cross-check validity:** the v1 study reproduces the release IBU k=4 from its own inputs at
  ≤1e-15 (input identity gate); integrated TUnfold/IBU = −0.111%/−0.087%; the fold gate plus this
  agreement confirm the full-truth (miss-included) convention on both sides — no epsilon-mismatch
  artifact. Basis: pinned to the SUPERSEDED no-cone S2′ basis; STATE.md mandates the
  observable-basis flag on any quote and records cone-basis regeneration as optional. The
  P-METHOD decision (IBU nominal) does not depend on TUnfold being a well-defined estimator.

**Loop-3 findings in this chain (all DIAGNOSTIC-ONLY, all in the superseded June study; the
October v1–v5 chain is clean):**
- The June phase-2 uncertainty conversion uses `LUMI_UNC_NB = 356000.0` ("user-corrected 356
  pb⁻¹ … Central sidecar differs") — inconsistent with the campaign 351000.0 used by the v1–v5
  chain; affects only diagnostic ratio uncertainty bands, explicitly caveated in the output JSON.
- The June builder's fake-keep convention is keep=0 on empty response denominators (release
  convention: keep=1) — documented as corrected in the October S2′ builder (README delta 3).
- Cosmetics: `regenerate_tau0_native_roots.py` records RegularizeCurvature triplets at tau=0
  where they are inert; the phase-1 comparison script prints an unconditional "PASS".

### A3. Residual sweep — postcheck real and run; isolation genuine; wrappers clean

- **`postcheck_release_cone_s2prime.py` — checks are REAL and it RAN in the official chain.**
  All 7 checkpoint groups implemented as documented (490 rows/method with 245+245, 490/490
  finite, negative cs == 0, basis fields per row; integrated IBU inside pinned C5 bands AND equal
  to the gate-suite realization ≤1e-12; per-bin IBU reproduces the G5 nominal ≤1e-12; BBB vs G4
  precedent ≤1e-6; r10 metadata; 27-file manifest with sha/size verification; 13-file sidecar
  with seeds 12345/22345, n_toys 1000, closure pins; tarball sha unchanged). Exit 0 iff all pass.
  Run artifacts exist: `release_driver_stub/run/postcheck.json` verdict **PASS** (all checks
  PASS; IBU h+ 11.412232616 / h− 11.123626367 nb — matching the RU-1 pins), plus a preserved
  failed attempt (5559407) for provenance. **Scope note:** its pinned paths cover the 2026-10-13
  cone RAW release tree only; the 2026-10-17 clean release is accepted by the stage-2 driver's
  own recorded gate suite (sha-pinned responses, bitwise wiring identity, negative_bins 0
  input-side, RU-1/RU-2, fenced no-cone Mig_ROOT role) plus the 26-item `run_bgsub_gate_cone.py`
  (PM-4 no-promotion rule; stale C_rad sha fence cf810be4 vs 205e8218).
- **`isolation_snapshot_cone_release.py`:** genuine pre/post isolation evidence (hashes/mtimes of
  central trees, key inputs, both study trees excl. the stub); compare mode byte-compares and
  exits 0 iff isolated. Ran: `isolation_compare.json` → `isolated: true`, all five artifacts
  identical. No physics-computation role; no risk.
- **`run_bgsub_setrprime_smoke_one_cone.sh`:** Condor smoke wrapper (cluster 5658879; pos-only,
  25 toys, 3.9 s; writes only `official_smoke_one`; touches no validated product; run record
  written; RUN_SHEET documents the RU-2 window pass). Clean.
- **Stage-2 residuals (headers + key sites):** `run_bgsub_gate_cone.py` — 26-item gate runner
  with production-independent IV-7 cone-parity, CP-3 expectation-centered physics gate
  (+0.1232 pp ± 0.02), CP-5 quadrature identity ≤1e-12, PM-4 promotion hold;
  `run_bgsub_budget19_cleandenom_cone.py` — single clean denominator, ibu_stat (Poisson-only
  toy) explicitly distinct from bgsub_stat (template variance — no double count), lumi flat 0.027
  never-rescaled hard-checked, L = 351000.0; `measure_bgsub_stage_rel_cone.py` — cone residual
  zero-by-construction ASSERTED against the spec anchor (STOP otherwise), a-priori sparse
  declaration, honest over-cap reporting. Spec generator/pins modules validated at pin level via
  the drivers' preflight spec sha (f0376909…). `extend_budget_15to19` superseded, retained
  unmodified. **No forbidden luminosity constant (297,657.8 / 126,880 / 15,813.4) anywhere in
  the stage-2 tree.**

## Part B — Final Adjudication (loops 1 + 2 + 3 consolidated)

| # | Finding (origin) | Class | Corrupts central values? | Corrupts quoted stat? | Exact fix | Blocks release? |
|---|---|---|---|---|---|---|
| 1 | No in-code output negativity/finiteness guard in both `write_method`s (L1-M1; L2 confirmed; L3 scope note: postcheck covers the RAW tree only; clean-release producer guard still open) | UNFOLD-NUMBER | No (IBU structurally non-negative; inputs hard-gated; 0/490 recorded; postcheck PASS on RAW tree) | No | `np.isfinite(x).all() and (x >= 0).all()` assert in both write_methods + RU-1 repro rerun; optionally date-clone postcheck to the clean-release tree | No |
| 2 | Stale self-deprecating prose in response builder ("PHYSICALLY WRONG" NWeff label; round-5 vs round-8) (L1-M2; L2 confirmed) | PROVENANCE | No | No | Correct the two comment sites; no code change | No |
| 3 | Analytic IBU reg-unc uses frozen final-iteration response (L1-M3; L2 confirmed; diagnostic path only, never quoted) | DIAGNOSTIC-ONLY | No | No (quoted stat = 1000-toy sidecar, single lineage) | Doc note; optionally freeze at k=4 if ever quoted | No |
| 4 | 35/490 sub-unity DJ14 C_rad high-pT tail bins undocumented; v1 range flag dropped in v3 fork (L2-M1) | PROVENANCE | No (per-bin c_rad_gen_rel/c_rad_stat_rel bound it) | No | Physics note in radiative docs + explicit c_rad_gen coverage statement | No |
| 5 | Covlegs stage-3 docstring "delta_k = (U_plus − U_minus)/2" vs custodial grouping — JES sign-inversion hazard (L2-M2); L3: consumers verified direction-paired, hazard NOT realized | MAINTENANCE | No | No | One-line docstring fix; keep the S5f D1 mapping as authoritative convention record | No |
| 6 | June TUnfold phase-2 diagnostic scripts use 356000.0 nb⁻¹ for uncertainty-band conversion (vs campaign 351000.0), explicitly caveated (L3) | DIAGNOSTIC-ONLY | No | No | Mark June study superseded-historical or repin the constant with a note | No |
| 7 | June builder fake-keep keep=0 on empty denominators (release: keep=1); corrected in the October S2′ builder (L3) | DIAGNOSTIC-ONLY | No (June study superseded) | No | None (already fixed in the active chain) | No |
| 8 | June cosmetics: inert tau=0 regularization record; unconditional "PASS" print in the phase-1 comparison (L3) | DIAGNOSTIC-ONLY | No | No | Optional one-line notes | No |

### Treebuilder-M1 exposure bound into the unfolding layer

The unfolding layer's kinematic operands are, explicitly:
- **Reco side (both the response builder and the data-vector builder):** `Q2es`, `Yes`, `Xes`
  (eSigma method; event cuts 150 < Q2es < 20000, 0.1 < Yes < 0.7, Empz > 45; binning
  `cell_index_reco(Q2es, Xes)`), the scattered-electron four-vector branches (sEX/sEY/sEZ/sEE)
  for the γ*p-CM boost that computes z and pT_h, and the track four-vectors.
- **Gen side (response builder):** `Q2_es_gen`, `x_es_gen`, `Y_es_gen` (eSigma gen DIS kinematics,
  with legacy fallbacks Q2esGen/Q2eGen) and the gen hadron branches (GenPt, GenEta, zGen, …).

A word-bounded search for bare `Q2s`/`Ys` across the cone builder tree returns **zero matches**.
The treebuilder M1 finding (HAT-frozen Q2s/Ys) therefore does not touch any binning, selection,
or boost operand of the unfolding layer at the branch level — the unfolding-layer verdict is
independent of M1. The residual exposure is only the treebuilder-internal question of whether the
eSigma branches (Q2es/Yes/Xes/sE*) are themselves derived from HAT-frozen state; that closure
belongs to the treebuilder critique loop.

### Totals and verdict

- **Confirmed defect count by class:** UNFOLD-NUMBER 1 · PROVENANCE 2 · MAINTENANCE 1 ·
  DIAGNOSTIC-ONLY 4 → **8 total** (0 CRITICAL, 0 MAJOR).
- **Protected quantities:** released central values (clean-release IBU/Born) — untouched by every
  confirmed finding; quoted statistical errors (1000-toy Poisson-only sidecar, SA-3 single
  lineage, RU-1 toy_stat bitwise) — untouched.
- **Overall: PASS** for the unfolding procedure (loop-1 SOUND, loop-2 SOUND upheld, loop-3 SOUND).

### Minimal hardening list

1. Add the one-line output guard (`np.isfinite(x).all() and (x >= 0).all()`) to both
   `write_method`s and rerun RU-1 repro.
2. Fix the covlegs stage-3 docstring to "(U_up − U_down)/2, up/down = shift directions (S5f D1)".
3. Correct the two stale prose sites in the cone response builder.
4. Add the sub-unity C_rad tail-bin physics note + c_rad_gen_rel coverage statement to the
   radiative docs.
5. Optionally date-clone the release postcheck to the clean-release tree (acceptance parity).
6. Mark the June TUnfold study superseded-historical (356000 nb⁻¹ conversion constant;
   keep=0 legacy convention).

## Recommended Next Check

Close the M1 residual in the treebuilder loop: confirm that the eSigma branches
(Q2es/Yes/Xes/sE*) consumed by the unfolding layer are computed independently of the HAT-frozen
Q2s/Ys state in `ChargedSidisTreeBuilderHighQ2_v6.C` — a single read of the eSigma computation
block is decisive; if independent, the unfolding layer's M1 exposure is bounded to zero and this
PASS stands unconditionally.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 3 of 3 (unfolding procedure, final
  adjudication), read-only; no analysis script was modified.
- Report persisted by the delegating supervisor session (critic toolset read-only).

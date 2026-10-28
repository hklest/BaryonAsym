# Loop 1 — Unfolding Procedure: Physics-Correctness Critique

**Script audit 2026-10-19 · sidis_highQ2 · v6/z005 · cone+S2'+Set-R′ clean-release basis (2026-10-17 FINAL)**

## Verdict

**SOUND** — for the unfolding procedure as implemented. No CRITICAL and no MAJOR
physics defect was found in the reviewed chain. Three MINOR hardening/maintenance
findings and six INFO notes are recorded; none changes a released central value
or its quoted statistical error.

Scope of this verdict: the formula → code → number chain of the unfolding layer
(response construction, cone data vector, Set-R′ subtraction, IBU re-unfold,
release construction, and the Born = C_rad × unfolded packaging site). NOT
certified by this loop: the C_rad physics values themselves (loop 2,
radiative corrections), data-luminosity provenance (normalization loop), the
systematics budget (loop 3), and plotter-side density normalization (the known
FATAL `P-XVOL` width defect lives in the plotting chain, outside this file set).

All three evidence layers agree: (1) the intended formulas are explicit in code
headers, the machine-readable spec (`bgsub_setrprime_cone_spec_2026-10-15.json`,
sha-pinned), and STATE.md; (2) the implementations realize those formulas —
verified line-by-line across all reviewed files; (3) the resulting numbers are
traceably consistent (RU-1 graded/byte parity vs the canonical RAW release,
SA/IV gate pins, campaign totals, and STATE.md anchors all cohere).

## Material Reviewed

Observable: bin-integrated per-charge ep cross-sections (nb), 490 published bins
= 245 h+ + 245 h−, via IBU (D'Agostini, k=4) on the cone+S2′+Set-R′
background-subtracted data vector; Born = C_rad(bin) × unfolded. Family:
sidis_highQ2 (v6 central, cone basis). Evidence type: full line-by-line source
review plus targeted reads of spec pins, data-vector JSON headers, and STATE.md
(2026-10-19 freeze state).

Mandated file groups (all present at the given paths; **no substitution was
needed**):

1. `scripts/unfolding/cone_s2prime_2026-10-11/binning_v3.py` — binning/geometry.
2. `scripts/unfolding/cone_s2prime_2026-10-11/build_migration_th2_per_charge_cone_s2prime.py`
   (2563 lines) — cone+S2′ response builder.
3. `scripts/unfolding/cone_s2prime_2026-10-11/build_data_vector_v4_alphaveto.py`
   — cone data-vector builder.
4. `scripts/unfolding/cone_s2prime_2026-10-11/merge_migration_per_charge_cone_s2prime.py`
   + `check_merged_cone_s2prime.py` — merge and post-merge validation harness.
5. `scripts/unfolding/cone_s2prime_2026-10-11/run_bbb_ibu_release_cone_s2prime.py`
   (1128 lines) — RAW-basis release driver.
6. `output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/scripts/stage2_cone_2026-10-15/run_bgsub_setrprime_reunfold_cone.py`
   (1313 lines) + `common_bgsub_setr_cone.py` — stage-2 re-unfold driver and
   shared constants.

Auxiliary files reviewed to close contract elements (imports and consumers of
the mandated files):

- `scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`
  — the IBU/BBB engine imported verbatim by both drivers (never forked).
- `stage2_cone_2026-10-15/build_bgsub_vectors_setrprime_cone.py` — Set-R′
  subtraction-vector builder (the exactly-once construction site).
- `stage2_cone_2026-10-15/package_bgsub_setrprime_born_release_cone.py` — Born
  packaging; targeted read of the C_rad application site (lines 138–342, 400–466,
  582–603, 907–940).

Provenance note: an earlier attempt at this critique was cancelled by a server
restart before any output was produced; this is a fresh, complete review.

## Findings

### Critical — none

### Major — none

### Minor

**MINOR-1 — No output-level negativity/finiteness guard on released
cross-sections.**
Files: `run_bbb_ibu_release_cone_s2prime.py` and
`run_bgsub_setrprime_reunfold_cone.py` (`write_method` in both); engine
`crosscheck_unfold_methods_v4.py`.
Claim criticized (implicit): "released CSVs cannot contain negative or
non-finite cross-sections."
Evidence: both `write_method` implementations compute
`cross_section_nb = yield / LUMI` and write the release CSVs without any assert
on sign or finiteness of the output arrays; the engine's `validate()` is not
invoked by either driver. The only negativity checks are input-side (builder
SA-1 asserts 0 negative bins at 980 and 490; stage-2 sub mode asserts
`negative_bins == 0` on the data vector, lines 1098–1100). Aggravating detail:
the exact-text CSV writer (`common_bgsub_setr_cone.py`, `fmt_float`,
lines 197–205) returns `""` for non-finite floats — a NaN would silently become
an empty cell rather than a loud failure.
Why it matters physically: a degenerate response bin (zero column sum → 0/0) or
any future operand drift could emit NaN/Inf into a release CSV uncaught by the
producing driver.
Impact: follow-up hardening only — not analysis-killing for the current release.
IBU is structurally non-negative for non-negative operands (all update terms
non-negative), inputs are hard-gated to be non-negative, and recorded audits
show 0/980 negative reco inputs and 0/490 negative cross-sections (STATE.md).

**MINOR-2 — Stale self-deprecating comments invite future wrong edits.**
File: `build_migration_th2_per_charge_cone_s2prime.py`.
Evidence: the builder labels the `prior_gen_NWeff` histogram
"PHYSICALLY WRONG. DO NOT USE" while the adopted coherent prior (body+miss from
the cone variant response) is the actual release operand and is asserted against
the response blocks to ≤1e-12; separately, the builder metadata
`matching_strategy` prose says `round(GenPt,5)`/`round(GenEta,5)` while the code
rounds to 8 decimals (matching keys remain consistent between the CT and CTG
maps).
Why it matters physically: nothing today — both are prose-level. The hazard is
forward-looking: a maintainer who "fixes" either site breaks the bit-identity
chain (RU-1, R10, sha pins). The guards would catch it, but the comments
actively invite the wrong edit.
Impact: maintenance/provenance hygiene only; zero effect on current numbers.

**MINOR-3 — Analytic IBU error propagation uses the frozen final-iteration
response.**
File: `crosscheck_unfold_methods_v4.py` (`ibu_unfold`,
`ibu_reg_unc_plus_minus_one`).
Evidence: the analytic (regularized inverse) error propagation freezes the
smearing matrix at the final computed iteration (k = K_max = 10) and applies it
to the covariance of all saved iterations, including the selected k = 4.
Why it matters physically: the analytic per-iteration errors for k = 4 carry the
k = 10 sensitivity structure — an approximation if those errors were ever
quoted. They are not: the released statistical uncertainty is the 1000-toy
Poisson-only sidecar (SA-3 single lineage), which supersedes the analytic
errors.
Impact: none on the release; documented approximation; follow-up only.

### Info

**INFO-1 — Toy covariance excludes response MC-statistics and f_i
uncertainty.** The 1000-toy sidecar fluctuates the data vector only (Poisson,
SA-3); response-side statistical noise enters via budget components instead.
This is a documented covariance gap (STATE.md), a deliberate variance
partition, not an unfolding-procedure defect.

**INFO-2 — DJANGOH branch uses the plain stored prior for the model
diagnostic.** The djangoh model-diagnostic leg runs with the stored cone
`h_prior_gen` rather than the coherent rebuild (recorded ±2.8% response-truth
mismatch), which inflates the model uncertainty conservatively. Diagnostic-only;
the single published denominator is the RAPGAP-cone coherent-prior IBU k=4.

**INFO-3 — Stopgap schema of the RAW cone data-vector JSON.** The production
cone DV (`data_vector_cone_alpha_central_2026-10-11.json`, label
`raw_data_no_bgsub`, `bg_subtracted: false`) stores raw counts under
`per_bin.n_subtracted` with a loud in-file warning that this is a stopgap "MUST
be replaced … before any physics result is quoted". That condition is now
satisfied: the 2026-10-17 clean release consumes the true bg-subtracted vector
(label `bgsub_setRprime_conebase_e_v0_lowq2_subtracted`, `bg_subtracted: true`).
The same physical cone vector exists under three files/labels (production JSON,
June alpha0p2 reference, release `raw_data_no_bgsub_alphaveto_cone`); content
identity is gate-proven, not assumed: IV-3 per-bin max deviation 0.0 vs the June
alpha0p2 vector, bitwise wiring identity vs MIG_ROOT, RU-1 graded/byte parity,
and CP-2's double bitwise equality (add-back == `n_subtracted` AND == `n_raw`)
transitively proves `n_raw == n_subtracted` in the cone JSON. Label divergence
is provenance hygiene only.

**INFO-4 — Fine-level empty-bin guard is a conservative under-subtraction.**
`build_bgsub_vectors_setrprime_cone.py` lines 180–183: where `D_cone == 0` the
template content is not subtracted (guard to zero) rather than producing a
negative input bin. The guarded-away content is recorded and pinned (SA-1); the
direction (under-subtraction) is covered by the ±100% e+V0 envelope
(`bgsub_env100_rel`) in the quoted budget. A defensible, documented convention.

**INFO-5 — e∩V0 overlap ignored by design.** Per user decision 2026-10-08
(112.2093 cone-applied tracks = 0.0029% of D_cone), recorded in IV-6 and in the
data-vector JSON taxonomy. Systematic-only; covered by the env100 envelope.

**INFO-6 — Luminosity landscape verified, trap fenced.** L = 351000.0 nb⁻¹
(four-period S67 luminosity including ep0304) is the single luminosity constant
in both drivers (`LUMI`/`L_DATA_NB`) and the common module. Grep across both
reviewed script trees: the only occurrence of 297,657.8 is an anti-regression
comment in `run_bbb_ibu_release_cone_s2prime.py` (line 194) documenting that the
stale 2005–2007-only value silently reverted the 2026-06-11 rescale on
2026-06-14 — i.e., the trap is known and fenced. No forbidden constant
(126,880; 15,813.4; 22.1963651) appears anywhere in the reviewed chain. MC
luminosity never enters: gen weights w1·w2 are applied at builder level and
luminosity cancels in the acceptance; the only normalization applied to yields
is `yield / 351000.0`. Note the workspace-wide 297,657.8 nb⁻¹ anchor belongs to
other analysis families and does not apply to this combinedL campaign.

## Non-Blocking Checks Confirmed

1. **Binning integrity** (`binning_v3.py`): gen 7 cells × 7 z × 5 pT = 245,
   reco 7 × 7 × 10 = 490 per charge; TH2 shape (491, 246) with miss row index
   490 and fake column index 245; `digitize − 1` with −1 sentinels; self-test
   asserts. 490 published bins = 245 h+ + 245 h−; data vector length 980,
   charge-major (0..489 = h+, 490..979 = h−).
2. **Response orientation, no transpose errors**: A_ij = B_ij / P_j with
   eps_j = column sums; miss row excluded from the body, fake column excluded
   from the body; the IBU fixed point is exact under the coherent prior
   (asserted vs blocks ≤ 1e-12) and toy closure passes — any transpose or
   orientation error would break both.
3. **IBU algorithm**: D'Agostini update n_j = (1/eps_j) Σ_i M_ji·d_sig_i with
   prior initialization n⁰ = P (coherent body+miss from the cone+S2′ variant
   response); efficiency denominator = body/(body+miss) per gen bin;
   iterations 1–10 computed, k = 4 selected per the adopted P-METHOD pin
   (KSEL = 4 in both drivers).
4. **f_i applied exactly once**: f_i = F_i/(rowsum B + F_i) recomputed from the
   consumed cone response's own B/F blocks (no-cone tables FORBIDDEN and guarded
   by the pinned f_i crosscheck signature, tolerance 1e-9);
   d_sig = data·(1−f_i); dvar = w2·(1−f_i)²; RU-2 records `f_i_recomputed: False`
   with the source guard; the f_i sidecar is byte-identical between repro and
   sub modes (same response, same f_i). S2′ semantics: det.int. Class 2 removed
   response-internally exactly once; the charged-K DIF k-parented keeper stays
   signal (`is_kpar = is_detint & (|GenMotherPDG| == 321)`).
5. **Cone veto placement**: truth vetoes (`not_e`, `not_v0_match_top`) →
   alpha ≥ 0.2 rad lab-frame cone (NaN alpha rejected) → z-guard; gen side
   unchanged. Cone-rejected matched legs leave reco entirely and grow the miss
   row via the CTG pass with consumed-key recording; the count identity
   consumed + miss == mc_count holds exactly. No double counting between the
   miss row and matched legs (fc3 reroute records consumed keys; miss-fill
   suppressed for consumed keys). The data-vector builder applies the identical
   selection ordering.
6. **Set-R′ subtraction exactly once** (`build_bgsub_vectors_setrprime_cone.py`
   + driver): B_R′ = e_cone_applied (RG/DJ generator average, AS PACKAGED, no
   rescale) + V0_genavg + lowQ2; the det.int. template is loaded but verified
   NOT subtracted (IV-6 asserts the taxonomy: external det.int. subtraction
   FORBIDDEN, response-internal f_i owns it); pre-unfold, from the cone data
   vector; SA-1 asserts 0 negative bins at both 980 and 490; SA-2 pins the
   per-charge data_sub′ sums (stale-vector guard); CP-2 proves vector-level
   exactly-once bitwise: data_sub′ + B_R′_applied == D_cone == n_raw.
7. **Sub-mode wiring**: label and `bg_subtracted: true` guards; SA-3 asserts
   w2 == data_sub′ elementwise (Poisson-only; the E1b variance
   `var_sub = n_sub + varB_R′` is recorded but explicitly NOT used for
   unfold/toys — the retired rev-2 "Poisson added twice" defect stays retired);
   negative input bins asserted 0.
8. **Reproducibility chain**: RU-1 — BBB byte-identical and IBU graded
   (≤ 1e-10 relative, string columns byte-identical, toy_stat bitwise) vs the
   canonical cone RAW release; spec pins 11.41223261552874 (h+) /
   11.123626367451847 (h−) match the STATE.md RAW totals 11.412233 / 11.123626
   nb, with the campaign-record 11.123626367451845 documenting 1e-15-level
   cross-run noise. RU-2 — integrated f_i removal window from spec, pass
   recorded. All primary operands sha-pinned (responses, DV JSONs, spec, C_rad
   table, carried vectors); exact-text CSV universe; run records written before
   hard-stop asserts.
9. **Born contract verified at the application site**
   (`package_bgsub_setrprime_born_release_cone.py`): declared
   `sigma_Born = C_rad × IBU_k4(data_cone − B_R′; cone S2′ variant …)`; the
   canonical frozen merge block is asserted to retain
   `ibu[column] = ibu[column] * ibu['C_rad']` semantics; an independent
   re-application cross-checks the 8 absolute-nb columns as parent × C_rad
   bitwise; `finite_C_rad` asserted true per row; applied table = DJANGOH14
   releasebin (EMPZ>45), sha-pinned, integrated-C_rad 1e-12 provenance gate;
   "No other C_rad variant enters" the release (RAPGAP31 emfix remains the
   formal alternative, not in this chain); uncertainty columns scaled
   consistently by the same per-bin C_rad.
10. **Number coherence** (arithmetic on published totals only, no
    recomputation): D_cone in-grid 3,865,073 = 1,965,700 (h+) + 1,899,373 (h−);
    B_R′ 281,250.09 = 7.2767% of D_cone (e 3.8543% + V0 3.4166% + lowQ2
    0.0054%); clean-release IBU totals 10.508811 / 10.217503 nb are −7.92% /
    −8.15% vs the RAW totals 11.412233 / 11.123626 nb — consistent with the
    7.2767% integrated data-level removal given the per-bin removal structure
    under IBU reweighting (removed events concentrated in higher-unfolded-weight
    bins); same sign, bounded magnitude, no anomaly. Born totals 11.996558 /
    11.663956 nb. Cone in-grid removal vs the no-cone vector 19,024 = −0.4897%,
    inside the (0.1%, 5%) guard band; `data ≤ central` elementwise guard
    recorded TRUE. All cross-sections positive per recorded audits (0/490
    negative).
11. **Charge/variable integrity**: charge-major layout asserted at every
    consumer; per-charge sums pinned separately (h+ ≠ h− totals and pins);
    980→490 pT-pair summation one-to-one (IV-4 asserts every one of the 980
    reco bins used exactly once; PT_PAIRS consistent with the binning_v3
    gen/reco pT nesting); release row metadata built from the pinned binmap
    (Q2 strata, x sub-bins, z, pT(gen)) — no charge swap, no Q2/x swap, no
    hadron-tagged-DIS denominator issue in this chain (the observable is a
    track-level cross-section; the DIS selection lives upstream of the reviewed
    files).

## Missing Evidence and Scope Boundaries

- **C_rad values (loop 2)**: the application site, table identity, and
  finiteness guards are verified here; the physics of the DJANGOH14 releasebin
  C_rad values (and the RAPGAP31 emfix formal alternative) belongs to the
  radiative-correction critique loop. The releasebin baseline is recorded in
  STATE.md as PROVISIONAL, not publication clearance.
- **Data-luminosity provenance**: the constant 351000.0 is verified as used
  consistently and correctly placed; its own validation is the normalization
  loop's scope.
- **Systematics budget (loop 3)**: `bgsub_stat_rel`, `bgsub_model_rel`,
  `bgsub_stage_rel`, `bgsub_fakefrac_rel` and the env100 envelope were skimmed
  for construction sanity only.
- **Plotter-side normalization**: the `P-XVOL` FATAL (bookkeeping vs capped
  x-width in the 7-panel plotters) is a plotting-chain defect already tracked
  elsewhere; the released CSVs reviewed here are bin-integrated and unaffected.
- **MC generator semantics**: no finding in this report depends on unresolved
  BGcheck/ParticleSource/truth-bank semantics; the truth-side classifications
  used (electron/V0 truth vetoes, `GenMotherPDG == 321` keeper, det.int. Class 2
  fake routing) are explicit in the reviewed code and the spec taxonomy.

## Recommended Next Check

Proceed to loop 2 (radiative corrections): critique the pinned DJANGOH14
releasebin C_rad table (`crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`,
sha `cf810be4…`) and the RAPGAP31 emfix formal alternative at the value level —
per-bin magnitude/spike behavior, the residual z-rising/pT_h-falling trend
already flagged in STATE.md, and the emfix comparison. Fold MINOR-1's one-line
output guard (`np.isfinite(x).all() and (x >= 0).all()` on the released
cross-section arrays, in both `write_method` implementations, followed by an
RU-1 repro re-run to confirm no drift) into the next hardening pass.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 1 of 3 (unfolding procedure), read-only; no analysis script was modified.
- Report persisted by the delegating supervisor session (critic toolset read-only).

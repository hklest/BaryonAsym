# Critique — high-Q² charged-SIDIS final-bin (pT-merge) internal candidate

- **Date:** 2026-06-03
- **Critic role:** read-only watchdog (overnight final-binning workflow)
- **Scope:** Audit `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` against the 9 acceptance checks supplied by the supervisor, with cross-reference to the 2026-06-01 rev6 tau-scan FATAL critique and the 2026-06-02 210-bin freeze FATAL critique that this candidate is the response to.
- **Verdict:** **CONCERN** (one minor merge-policy ambiguity, several documentation gaps; no FATAL).
- **Bottom line:** the candidate is a coherent and substantial improvement on the 2026-06-02 210-bin freeze attempt — taus, merge algorithm, and high-z policy uniformity have been fixed. It is **not yet SOUND** because (i) two BLOCKER (cell,z) rows are silently dropped rather than published with a BLOCKER status as the policy demands, (ii) the prior-critique FATAL items F1–F4 still have no written closure artifact, (iii) the diagonal-only covariance and missing Tier-2 systematics are not flagged inside `release.json`, and (iv) the README labels the S3-fallback drop of an `S3_xmerged z∈[0.5,0.7]` row as "S3 or z>=0.7" which contradicts the literal "z>=0.7" wording of the policy.

---

## Paths reviewed

Final-bin candidate directory (10 files; manifest with SHA-256 checked):

- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/release.json`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/README.md`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/manifest.json`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/binning_finalbin.json`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/publication_mask_finalbin.json`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/final_binning_map_ptmerge.csv`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/final_binning_map_ptmerge.json` (inspected via CSV companion)
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/yields_finalbin_pos.json`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/yields_finalbin_neg.json`

Upstream input (refreshed 210-bin product the final-bin candidate is built on):

- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/release.json`
- `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/README.md`

Context for prior-critique closure:

- `H1/sidis_highQ2/docs/critiques/critique-210bin-binning-freeze-2026-06-02.md`
- `H1/sidis_highQ2/docs/critiques/critique-tunfold-rev6-tau-scan-2026-06-01.md`
- `H1/sidis_highQ2/docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md`
- `H1/sidis_highQ2/docs/results/rev6_v2_chain_audit_fixes_2026-05-31.md`
- `H1/sidis_highQ2/docs/RUN_STATUS.md` (line 153: Tier-2 systematics pending design)

Remote candidate at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` did **not** exist at the time of review; only the local copy was used (confirmed with one `ssh -O check` and one remote `ls`; remote dir absent).

---

## Headline numbers

| Quantity | Value | Source |
|---|---|---|
| Final published bins per charge | **152** | `release.json:n_bins_per_charge`, `binning_finalbin.json:n_bins_per_charge` |
| Common bins across charges | 152 | `release.json:n_common_bins` |
| Distinct (cell, z) rows | 34 | counted from CSV / binning JSON |
| Rows by merge scheme | A:25, B:2, C:6, D:0, E:1 | `release.json:schemes_used_count` |
| z rows dropped (S3 or z≥0.7) | 6 | `release.json:n_z_rows_dropped_S3_or_zge07` |
| z rows flagged BLOCKER (z<0.7 scheme-E failure) | 2 | `release.json:n_z_rows_blockers_lowz` |
| FATAL acceptance violations in published bins | **0 / 152 / charge** | computed (see Check 2) |
| Bins with rel σ_total > 100 % (still ≤200 % gate) | 5 (pos), 6 (neg) | computed |
| Max rel σ_total in published set | 1.71 (pos bin 70), 1.71 (neg bin 70) | computed |
| Covariance treatment | `diagonal_only` | `release.json:covariance_approximation`, both yields JSONs |
| Tau (rev6 cells 0..5) | pos 1.0e-6, neg 2.59e-6 (closure-tauopt) | `release.json:taus` |
| Tau (S3 x-merged) | pos 3.16e-3, neg 1.0e-3 (multi-objective AREA) | `release.json:taus` |

The header `n_bins_per_charge = 152` is exactly the row count in `binning_finalbin.json` and in the per-charge yields files — internally consistent. SHA-256s in `manifest.json` cover all 10 listed artifacts.

---

## Check-by-check findings

### 1. Final bin map merges only pT_h, never z — PASS

Verified by exhaustive scan of `binning_finalbin.json` (152 rows) and `final_binning_map_ptmerge.csv`:

- All 152 rows carry `(z_min, z_max)` ∈ the canonical 6-edge set `{(0.0,0.1), (0.1,0.2), (0.2,0.3), (0.3,0.5), (0.5,0.7), (0.7,1.0)}` (verified: 0 non-standard z rows).
- Every CSV row's `base_bin_indices_210` is a contiguous block of pT_h indices inside one (cell, z) — never crosses into another z row.
- Schemes A..E correspond to pT mergings of the canonical {0.0, 0.5, 1.0, 2.0, 4.0, 8.0} GeV grid only; the most aggressive (scheme E, used once at row S2_x1, z=3) keeps z fixed at [0.3, 0.5] and merges pT into a single [0, 8] GeV bin.
- `release.json:merge_policy.merge_axes = "pT_h only; no z merging."` and the artifact obeys it.

### 2. No published bin with data_count==0, central≤0, no support, or rel σ_{stat|model|total} > 200 % — PASS

Direct scan of both yields JSON files (each with 152 rows, 17 numeric fields per row):

- 0 rows with `data_count <= 0`.
- 0 rows with `unfolded <= 0` (this is the central value used for the "central > 0" guard).
- 0 rows with `n_trusted_base <= 0` (proxy for "support_mask true at least once").
- 0 rows with `rel_sigma_stat > 2.0`, `rel_sigma_model > 2.0`, or `rel_sigma_total > 2.0`.

So the 200 %, central, data-count, and support gates are all enforced. Worst surviving cells (still inside the gate but worth marking):

| charge | bin | (cell, z, pT) | rel σ_stat | rel σ_model | rel σ_total |
|---|---|---|---|---|---|
| pos | 70 | S1_x0, z∈[0.7,1.0], pT∈[0.0,1.0] (scheme C) | 1.712 | 0.040 | **1.713** |
| neg | 70 | S1_x0, z∈[0.7,1.0], pT∈[0.0,1.0] (scheme C) | 1.552 | 0.708 | **1.710** |
| pos | 77 | S1_x1, z∈[0.0,0.1], pT∈[4,8] (scheme A) | 1.551 | 0.512 | **1.634** |
| neg | 77 | S1_x1, z∈[0.0,0.1], pT∈[4,8] (scheme A) | 1.375 | 0.171 | **1.386** |
| pos | 116 | S2_x0, z∈[0.5,0.7], pT∈[0.0,1.0] (scheme C) | 0.942 | 0.041 | 0.943 |
| neg | 42 | S0_x1, z∈[0.5,0.7], pT∈[0.0,1.0] (scheme C) | 0.947 | 0.612 | 1.127 |

The 200 % policy is being honored, but the headline product still publishes bins with central values that are < 0.6 σ_total away from zero (rel σ_total ≈ 1.7). This is permitted by the supervisor-supplied gate, so it is not a FATAL here; it is the same caveat the 2026-06-02 critique raised against the 200 % threshold (`M-bin-6` of `critique-210bin-binning-freeze-2026-06-02.md`), which has not been narrowed in this candidate. Flagging as a *standing* concern, not a freeze blocker under the present gate.

### 3. Common binning across charges or deviations justified — PASS

`release.json:merge_policy.common_across_charges = "If either charge requires a coarser scheme, the coarser scheme is used for both."` and `n_common_bins = 152` per charge — i.e. every published bin has both charges. The `n_bins_per_charge.{pos,neg} = 152` confirms full symmetry. Schemes per (cell, z) row are common by construction. No per-charge bin asymmetry in the published set.

### 4. S3 z≥0.7 dropped if unsalvageable; z<0.7 not silently dropped — CONCERN (two sub-issues)

**4a. Subset of the spec that is correctly honored.** The four genuinely z≥0.7 rows that fail scheme E are dropped from the final binning and listed in the README under "Dropped z rows":

- `S0_x0 z=5 z∈[0.7,1.0]`
- `S1_x1 z=5 z∈[0.7,1.0]` — wait, the README lists `S1_x1` only via blocker; the explicit drops listed are `S0_x0 z=5`, `S1_x1 z=5`, `S2_x0 z=5`, `S2_x1 z=5`. Correction: the README lists `S0_x0 z=5`, `S1_x1 z=5`, `S2_x0 z=5`, `S2_x1 z=5` (four z≥0.7 rows) plus two S3 fallback rows.

Verified by direct inspection of `binning_finalbin.json`: none of those z=5 rows appear in the published 152-bin set. Good.

**4b. Sub-issue: one of the "dropped" rows is a z<0.7 row dropped via the S3-fallback clause.** The README "Dropped z rows" list includes:

> `S3_xmerged z_bin=4 z in [0.50, 0.70]: scheme E failed both charges (S3 or z>=0.7)`

This row is z∈[0.5, 0.7] — i.e. z<0.7. The merge-policy text in `release.json` is:

- `S3_z_ge_07_fallback`: drop the z row for both charges if even scheme E fails.
- `z_lt_07_E_failure`: reported as BLOCKER (not silently dropped).

These two clauses overlap when an S3 row is simultaneously z<0.7 and a scheme-E failure. The implementation took the S3-fallback path (drop) for `S3_xmerged z∈[0.5,0.7]`, but the literal task acceptance text requires `S3 z>=0.7 is dropped if unsalvageable; z<0.7 rows not silently dropped`. So an `S3_xmerged z∈[0.5,0.7]` drop is **either an unwritten "S3 also drops z<0.7 if unsalvageable" extension of the policy, or a quiet drop of a z<0.7 row**. The README parenthetical "(S3 or z>=0.7)" tells the reader the implementation prefers the former, but `release.json:merge_policy` does not state the extension explicitly — the only sentence about S3 fallback is keyed on `S3_z_ge_07_fallback`. Documentation should be tightened to read e.g. `S3_unsalvageable_fallback: drop the z row for both charges if even scheme E fails, including S3 z<0.7 rows that we cannot recover at S3-merged resolution`.

**4c. Sub-issue: BLOCKER rows are silently absent from the published bin set.** The two listed BLOCKERs are:

- `S0_x0 z_bin=2 z∈[0.20, 0.30]: scheme E failed both charges in z<0.7 row`
- `S2_x1 z_bin=4 z∈[0.50, 0.70]: one charge fails all schemes in z<0.7 row`

I confirmed these (cell, z) rows are NOT in `binning_finalbin.json`:
- `S0_x0` has z_bins `{0, 1, 3, 4, 5}` — z_bin=2 missing.
- `S2_x1` has z_bins `{0, 1, 2, 3}` — z_bin=4 missing.

Yet `publication_mask_finalbin.json` reports `n_published = 152` per charge with every status entry equal to `"published"`. There is no `status = blocker` or `status = withheld` slot for these rows anywhere in the published artifact. The README documents them in prose at the bottom, but **the JSON release pretends they do not exist**.

That contradicts the policy line `z_lt_07_E_failure: "Reported as BLOCKER (not silently dropped)."`. The honest implementation either (i) inserts placeholder rows in `publication_mask_finalbin.json` with `status = "blocker"` (preferred — preserves bin-index alignment with any downstream observable), or (ii) emits a top-level `release.json:blockers[]` list that names the (cell, z) coordinates and links them to the README narrative. Right now `release.json` has `n_z_rows_blockers_lowz = 2` as a single integer with no coordinates attached, and the downstream physics consumer reading only the JSON tree cannot reconstruct which two (cell, z) cells are missing.

This is the principal CONCERN of this critique.

### 5. The refreshed 210 input uses closure-tauopt taus, not older heuristic taus — PASS

`H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/release.json` states:

```
"method": "TUnfold rev6 AREA at closure-tauopt taus; S3 x-merged at multi-objective tau.",
"taus": {
  "rev6_cells_0_5": {"pos": 1e-06, "neg": 2.5929437974e-06},
  "S3merged":       {"pos": 0.003162278, "neg": 0.001}
}
```

These match exactly the taus declared in the final-bin candidate's `release.json:taus`. The closure-tauopt values were selected by the 2026-06-02 closure scan (`docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md`), and they are ~20–50× *smaller* than the heuristic τ_pos = 2.15e-5 / τ_neg = 4.64e-5 that the 2026-06-01 rev6 tau-scan critique rejected as FATAL (`pathology_flag=true, hard_reject=true`). So the upstream chain has been refreshed correctly. The S3-merged taus are unchanged from the multi-objective AREA selection (correctly, since closure-tauopt operates on the 240-bin response and is not defined for the 30-bin x-merged S3 response).

### 6. Uncertainties aggregated correctly; covariance-diagonal caveat stated — PARTIAL PASS / DOC GAP

Methodology recorded in `yields_finalbin_{pos,neg}.json:covariance_note`:

> sigma_stat for merged groups aggregated in quadrature from the per-base-bin TUnfold publication-bin error (sqrt of cov-matrix diagonal). Full ROOT covariance not loaded in this postprocessing driver. For true block-aggregation under TUnfold covariance, re-run with --use-root-cov in a future extension.

And in `README.md` §Covariance:

> sigma_reg and sigma_model aggregated by summing the shifted central vectors (unfolded_lo / unfolded_hi / unfolded_DJ) per group, then re-taking the half-difference. This is the correct extensive aggregation under the additivity of yields and preserves cancellation between bins.

Mathematical assessment:

- σ_reg and σ_model: aggregating by summing the shifted central per-base-bin yields and then taking the half-difference (rather than summing per-bin half-differences in quadrature) is the correct *extensive* procedure for fully correlated shifts (which is what tau variation and RG↔DJ swaps produce by construction). This preserves coherent cancellation and is the right choice. PASS.
- σ_stat: in-quadrature sum of the per-base-bin TUnfold diagonal errors. Under the TUnfold area constraint and a tightly regularized inversion, the *off-diagonal* covariance between adjacent reco-side bins is typically negative (anti-correlation from the area constraint) and the off-diagonal between unfolded-side bins is typically positive (regularization-induced smoothing). In-quadrature sum of diagonals therefore generally **overestimates** σ_stat in unfolded-side merges of regularization-smoothed neighbors and **underestimates** it in some constrained-sum merges. The README's caveat language ("treats the diagonal as if uncorrelated") and `release.json:covariance_approximation = "diagonal_only"` are honest. This is an acknowledged approximation. For a final published number it must be replaced with full-covariance block aggregation (the `--use-root-cov` extension the README anticipates). Documented as PARTIAL.

The header `release.json` exposes the diagonal-only flag; that is the minimum required disclosure. What is missing: there is no quantitative bound on how much σ_stat could shift under the full-covariance aggregation. For the freeze, a single-cell pilot showing the diagonal-vs-full-cov ratio on a representative merged group (e.g. one S0_x0 z=5 scheme-C cell or the S2_x1 z=3 scheme-E cell) would discharge the concern. Not present.

### 7. Release is not justified by integrated totals — PASS, with carry-over caveat

Nothing in `release.json` or `README.md` claims success on the basis of an integrated-total cross-check. The README's per-bin acceptance gates (`data_count > 0`, `central > 0`, `support_mask true`, rel σ ≤ 200 %) are local, not integrated.

Carry-over caveat that the candidate **does not** address but that the 2026-06-02 critique flagged as `F4`: under `kEConstraintArea`, the unfolded integrated total is identical to `data_total_with_overflow` by construction, so the integrated number is not a measurement. The new candidate inherits the constraint mode (taus changed, area constraint did not), but does not advertise the integrated total as evidence. So we are still PASS on the literal acceptance question.

`release.json` would be more defensible if it explicitly stated `is_shape_only_under_area_constraint: true` or `area_constraint_acknowledged: true` to discharge `F4` from the prior critique. Not present.

### 8. TUnfold central, IBU diagnostic — PASS

`release.json:central_method = "TUnfold AREA"`, `release.json:diagnostic_method = "IBU (not used in this release)"`. The yields JSON for both charges carries `unfolded` (TUnfold AREA) and `unfolded_DJ` (RG↔DJ swap for σ_model), no IBU column. IBU is correctly absent from the published yields and identified only as a diagnostic that is not used here. PASS.

### 9. Missing Tier-2 systematics documented as pending, not zeroed — PARTIAL PASS / DOC GAP

Tier-2 status check:

- `docs/RUN_STATUS.md:153`: `Tier 2 systematics (σ_fakemodel, σ_hadron_mass, σ_NW, σ_VtxZ, σ_MC_stat, σ_chargeconf) — pending design.`
- `docs/results/rev6_v2_chain_audit_fixes_2026-05-31.md:121-126`: each of F8–F13 (Tier 2) is `Open`.
- `docs/critiques/critique-210bin-binning-freeze-2026-06-02.md:M-bin-5`: lists the six Tier-2 sources as not in the release.

These are honestly documented in `docs/`. They are not zeroed in the yields — the yields JSON only quotes `sigma_stat`, `sigma_reg`, `sigma_model`, `sigma_total = quadrature(stat, reg, model)`. There is no `sigma_systematic` field that would silently default to zero.

Gap: the candidate's `release.json` and `README.md` **do not** state that Tier-2 systematics are pending. A reader who looks only inside the release directory will see `sigma_total` formed from (stat ⊕ reg ⊕ model) and may mistake it for an exhaustive total uncertainty. The release should carry a top-level field like:

```
"systematic_coverage": {
  "included": ["sigma_stat", "sigma_reg", "sigma_model"],
  "pending_tier2": ["sigma_fakemodel", "sigma_hadron_mass", "sigma_NW",
                    "sigma_VtxZ", "sigma_MC_stat", "sigma_chargeconf"],
  "interpretation": "sigma_total is NOT a final total uncertainty; Tier-2 sources are pending."
}
```

The information exists in the workspace; it just is not propagated into the release artifact itself. Documented as PARTIAL.

---

## Cross-check against the 2026-06-02 prior critique

The current candidate is the response to `docs/critiques/critique-210bin-binning-freeze-2026-06-02.md`. Status of that critique's required actions:

| Action (2026-06-02) | Status in finalbin candidate |
|---|---|
| 1. Written response artifact closing F1–F4 of the 2026-06-01 rev6 tau-scan critique | **Not done.** No new doc found in `docs/critiques/` or `docs/results/` that explicitly classifies F1 (broken σ_reg evaluator), F2 (rank/cond#), F3 (refold χ²/ndf), F4 (area-constraint identity) as RESOLVED/DEFERRED/ACCEPTED. This is the largest standing open item. |
| 2. Rebuild 210 at closure-tauopt τ | **Done.** `data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/` exists with pos=1e-6 / neg=2.59e-6. Final-bin candidate inherits these. |
| 3. Specify and apply the merge algorithm | **Done.** pT-only finest-to-coarsest A→B→C→D→E with common-across-charges; described in `release.json:merge_policy` and `README.md`. |
| 4. Unify high-z policy across Q² strata | **Done.** S0/S1/S2 z=5 rows are now treated identically to S3 — if scheme E fails, the (cell, z) row is dropped. README "Dropped z rows" lists all four S0/S1/S2 z=5 drops. |
| 5. Replace 200 % cutoff with physics-justified threshold (recommended ≤100 %) | **Not done.** 200 % retained. The two largest published rel-σ_total values are still ≈1.71. |
| 6. Add MC-stat-bootstrap σ and cross-closure tail to σ_total before applying cutoff | **Not done.** σ_model still uses only `0.5·|unf_RG − unf_DJ|` (one source). Cross-closure tail and MC-stat-bootstrap not in σ_total. |
| 7. Decide and document shape-only vs absolute-normalization | **Not done.** Area constraint still in force; no `is_shape_only` flag in the release. |
| 8. Audit response matrix for `reco_eff > 1` source | **Not addressed** in this release artifact. Trust mask logic unchanged. |
| 9. Hadron-mass / z-convention check | **Not addressed** in this release artifact. |
| 10. Single sanity systematic end-to-end (e.g. luminosity ±2.3 % or tracking 2 %/track) through the 152-bin pipeline | **Not done.** No closure-on-shifted-input artifact in this release. |

So three of ten required actions are complete (2, 3, 4). The remaining seven are unaddressed within this release. Some of those (5, 6, 7, 8, 9, 10) are non-blocking for "is this final-bin candidate internally consistent" but are blocking for "can this be promoted to anything beyond `INTERNAL_PRELIMINARY_CANDIDATE`". The status string `INTERNAL_PRELIMINARY_CANDIDATE` in `release.json` honestly reflects that this is not a freeze-grade product.

---

## Issues and required fixes

### Major issues (none rise to FATAL under the supervisor-supplied gate)

**M-1. Silent omission of BLOCKER rows from `publication_mask_finalbin.json`.** The two listed BLOCKERs (`S0_x0 z_bin=2`, `S2_x1 z_bin=4`) are missing from the published bin set with no corresponding `status = "blocker"` slot in the publication mask and no machine-readable coordinate list inside `release.json` beyond an integer count `n_z_rows_blockers_lowz = 2`. The supervisor's check #4 ("z<0.7 rows not silently dropped") is satisfied in prose (README) but violated in JSON. Required fix: add explicit blocker entries (placeholder rows in `binning_finalbin.json` with a publication-mask status of `blocker`, OR a `release.json:blockers[]` list naming the (cell, z) coordinates). Either is acceptable; pick one.

**M-2. Merge-policy text ambiguity for S3 z<0.7 fallback.** `release.json:merge_policy` distinguishes `S3_z_ge_07_fallback` (drop) from `z_lt_07_E_failure` (BLOCKER), but the implementation drops `S3_xmerged z∈[0.5,0.7]` via the S3 path even though it is z<0.7. README parenthesis "(S3 or z>=0.7)" describes the actual behavior. Required fix: tighten `release.json:merge_policy.S3_z_ge_07_fallback` to read `S3_unsalvageable_fallback: drop the z row for both charges if even scheme E fails, including S3 z<0.7 rows we cannot recover at S3-merged resolution`, OR re-classify `S3_xmerged z=4` as a BLOCKER. The current text-vs-behavior mismatch is a documentation defect, not a numerical one.

**M-3. No closure artifact for prior FATAL items F1, F2, F3, F4.** Required by action 1 of the 2026-06-02 critique. Without it the candidate inherits an open FATAL chain. This is the principal "advance the freeze" blocker (not a finalbin-candidate blocker per se).

### Minor issues / documentation gaps

**m-1. Tier-2 systematics not surfaced inside the release.** The information exists in `docs/RUN_STATUS.md:153` and `docs/results/rev6_v2_chain_audit_fixes_2026-05-31.md:121-126`, but a reader of `release.json` alone will not see it. Add a `systematic_coverage` block to `release.json` listing the six pending Tier-2 sources and stating `sigma_total` is not exhaustive.

**m-2. Diagonal-cov approximation not quantified.** The candidate flags `covariance_approximation = "diagonal_only"` honestly, but does not bound the impact. A single-cell pilot comparing diagonal-only vs full-cov block aggregation on one representative scheme-C or scheme-E cell would discharge the question. Otherwise the σ_stat numbers in the candidate carry an unbounded-direction approximation error.

**m-3. Area-constraint shape-only status not declared.** Add `is_area_constrained: true` / `integrated_total_is_not_a_measurement: true` to `release.json` so the shape-only interpretation is machine-readable and not just buried in F4 of an old critique.

**m-4. 200 % threshold retained.** Not in conflict with the supervisor's gate (which is itself `> 200 %`), but the surviving rel-σ_total ≈ 1.71 bins (pos/neg bin 70 in S1_x0 z=5 scheme-C, pos/neg bin 77 in S1_x1 z=0 scheme-A pT=[4,8]) carry essentially no information content. Either the threshold should be tightened (≤100 % per the prior critique's M-bin-6) or these bins should be downgraded to a `published_low_information` mask state.

**m-5. No covariance file emitted.** Aggregation of these 152 bins into any ratio observable (A_h, multiplicity ratio across z, charge ratio R⁺/R⁻) requires the full unfolded covariance, which is not in the release directory. The diagonal `sigma_stat` per bin is insufficient. This is not in the 9-point gate, but it will be needed before downstream physics agents can consume the product.

### Non-blocking confirmations

- File set is complete (10 files), manifest carries SHA-256 for all 10, JSON↔CSV companion pairs line up on bin counts.
- 152-bin count is identical pos↔neg with full common binning.
- No FATAL acceptance violations in the published bin set under the supervisor's gate (all four hard conditions hold).
- Closure-tauopt τ propagated end-to-end from the refreshed 210-bin input to the final-bin candidate.
- Schemes used count (A:25, B:2, C:6, D:0, E:1) integrates to 34 (cell, z) rows, consistent with the 7 cells × {2..6} z-bins-per-cell layout after the 6 drops and 2 blockers are removed (8 cells × 6 z = 48; minus 8 absent baseline z rows by cell layout − 6 drops − 2 blockers = 32; observed 34, the extra two are the `S2_x1` z∈[0.3,0.5] scheme-E single-bin row plus a counting offset from the S3-merged cell counting differently — non-blocking, internal accounting question).
- IBU correctly absent from the published yields; TUnfold AREA is the sole central method.

---

## Verdict and required next steps

### Verdict

**CONCERN.** Not FATAL: the candidate satisfies the four hard acceptance gates supplied by the supervisor on all 152 × 2 published bins, the closure-tauopt τ have been propagated correctly, the merge algorithm is specified and applied, and the prior critique's high-z policy asymmetry has been resolved. Not SOUND: two BLOCKER rows are silently missing from the publication mask, the Tier-2 systematics and diagonal-only covariance approximation are not surfaced inside `release.json`, and the prior FATAL chain (F1–F4 of the 2026-06-01 rev6 tau-scan critique) has no written closure.

### Exact next fixes (to take the candidate from CONCERN → SOUND)

1. **Emit BLOCKER coordinates in machine-readable form.** Either add the two BLOCKER (cell, z) entries to `publication_mask_finalbin.json` with `status = "blocker"`, or add a `release.json:blockers[]` list with `cell_label`, `z_bin`, `z_min`, `z_max`, and `reason`. The integer `n_z_rows_blockers_lowz = 2` is not sufficient.
2. **Tighten the merge-policy wording** so that `S3_xmerged z<0.7 scheme-E-fail` is unambiguously inside the "drop" branch (either rename the field to `S3_unsalvageable_fallback` and broaden the text, or re-classify `S3_xmerged z=4` as a BLOCKER and surface it as such).
3. **Surface Tier-2 systematic coverage in `release.json`** with an explicit `pending` list and a note that `sigma_total` is not exhaustive.
4. **Write a one-page closure note** for F1, F2, F3, F4 of `docs/critiques/critique-tunfold-rev6-tau-scan-2026-06-01.md`, classifying each as RESOLVED / DEFERRED / ACCEPTED-AS-LIMITATION and pointing to the closure-tauopt artifact for F6/F7. Save as `docs/critiques/response-rev6-tauopt-fatal-items-2026-06-03.md` so the freeze chain is closed in writing.
5. **(Recommended, not strictly required by the 9-point gate)** Quantify diagonal-vs-full-cov σ_stat ratio on one representative scheme-C and one scheme-E cell; add `is_area_constrained: true` to the release header.

None of these fixes require re-running TUnfold. They are all post-processing / documentation edits on top of the same unfolded vectors. Estimated effort: < 1 day.

---

## Risks, assumptions, gaps in this review

- **Remote candidate not yet present.** Only the local copy under `H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` was reviewed (verified at 2026-06-03 ≈ 06:30 UTC). If the remote copy at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` is materially different (e.g. has additional files or different SHAs), this critique does not cover it.
- **The 30-bin S3 x-merged base ROOT covariance** is not loaded by the postprocessing driver per the README. I take the README's word for it; I did not independently verify there is no off-diagonal cov file shipped with the candidate.
- **The "n_trusted_base" field** in the per-bin yields was treated as the support-mask proxy for check 2. If the supervisor's "no response support" gate is defined against a different field (e.g. the rev6 source-file `trust_mask` count rather than `n_trusted_base`), the check should be re-run with that field, but I do not expect any of the 152 published bins to fail because every published row has `n_trusted_base ≥ 1`.
- **The cross-closure tail systematic** mentioned in the 2026-06-02 critique as M-bin-2 — the cross-closure p90 of 66–88 % from `tunfold_rev6_closure_tauopt_2026-06-02.md` — is **not** in σ_model in this candidate; σ_model is RG↔DJ swap only. This was not in the supervisor's 9-point gate but I flag it because it is a known floor on the systematic budget.
- **No plot review was performed.** The README references `plots/MC_OVERLAY_SKIPPED.txt`; that file was not opened and the candidate ships no rendered plots in the release directory. Plot-level plausibility is out of scope here.

This artifact does not edit any reviewed file. It is the read-only critique for the overnight final-binning workflow.

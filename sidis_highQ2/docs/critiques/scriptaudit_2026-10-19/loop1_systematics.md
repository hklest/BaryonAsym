# Loop-1 Systematics Script Audit — high-Q2 charged-hadron SIDIS (v6/z005)

**Date:** 2026-10-19 | **Critic:** physics-correctness critic, loop 1 of 3 | **Mode:** read-only (no analysis script edited)

**VERDICT: CONCERN.** The 2026-10-19 Env100 quoted-budget chain fails its own declared basis formula — a demonstrated "component added twice" quadrature defect (numerically <=123 ppm relative, conservative direction) — and its builder script is unlocatable. The 2026-10-17 19-component clean-denominator budget, its driver, and the 26-item gate pass with no physics defects.

## 1. Material reviewed

- 19-component driver `run_bgsub_budget19_cleandenom_cone.py` (cluster 5721117; 2673 lines, full read) and shared module `common_bgsub_setr_cone.py` (full read).
- 26-item gate `run_bgsub_gate_cone.py` (1610 lines, full read).
- env100 chain: rev1 proposal `analysis_bgsub_final_scheme_2026-10-18.py` (full read), rev3 S2 builder `analysis_bgsub_final_scheme_rev3_s2edition_build_2026-10-18.py` (full read), installed tree `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100_21comp_2026-10-19/` (CSV header + 10 data rows; summary.json, STATUS.md, provenance_manifest.json, edition_md5_manifest.json, covariance/README.md, covariance/covariance_gates.json; string grep of the whole installed tree).
- 19-comp installed tree `systematics_bgsub_setRprime_19comp_cleandenom_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/`: validation_checks.json (full, 29/29 PASS), carried_components_record.json (full), comparison_vs_budget15to19.json (full), CSV header + row.
- `extend_budget_15to19_conebase_v2.py` (bgsub-4 component construction, formula-level read).
- Independent quadrature recomputation on 9 CSV bins spanning both charges, all strata S0-S3, zero- and nonzero-env100 bins, and the per-charge maximum bins.
- Skill `sidis-hq2-systematics` failed to load twice (tool error); the audit proceeded on file evidence only.

## 2. Findings

### CRITICAL

**C1. `detint_removal` enters the authoritative quoted total twice: the stored `total_with_env100_rel` does not implement the declared basis.**

- Where: `systematics_bgsub_env100_21comp_2026-10-19/bgsub_env100_21comp_budget.csv` (column `total_with_env100_rel`); enshrined in `summary.json` (basis string vs quoted_components list), `STATUS.md`, `covariance/covariance_gates.json` (active_terms), `covariance/README.md`; root cause sits in the untracked builder (see M1).
- Claimed: `summary.json` `"basis": "canonical clean denominator 19-component basis minus bgsub_stat_rel plus bgsub_env100_rel"` — i.e. total^2 = S19 - bgsub_stat^2 + env100^2 with detint entering once (19 active terms).
- Implemented (proven): total^2 = S19 - bgsub_stat^2 + detint_removal^2 + env100^2 — detint enters twice (20 active terms). Evidence, exact arithmetic on 9 bins (neg/0, neg/109, neg/174, neg/179, neg/240, pos/0, pos/109, pos/179, pos/240):
  1. The stored `total_with_bgsub19_rel` reproduces from the 19 component squares to <=1.1e-16 on every bin (19-comp arithmetic itself is sound).
  2. The stored `total_with_env100_rel` reproduces the DOUBLE-COUNT formula to <=1.1e-16 on 9/9 bins; the declared basis formula is rejected on 9/9 bins, with a miss equal to detint_removal^2/(2T) (magnitudes 2.2e-7 to 3.5e-5).
  3. Model-free demonstration on the three empty-D bins (env100 = 0 AND bgsub_stat = 0, so the declared basis reduces to total_with_env100_rel == total_with_bgsub19_rel exactly): stored totals instead EXCEED the 19-comp totals — pos/109: 0.24985123983280053 vs 0.24982041174607003; pos/179: 0.7365119705575378 vs 0.7365114588316988; neg/240: 0.4580627093914754 vs 0.45802799466701727 — each surplus equal to detint_removal^2/(2T) to first order. With both modification terms identically zero, the only possible extra contribution is the double-listed detint term.
  4. Root cause: `detint_removal_rel == hypot(detint_removal_model_rel, detint_removal_stat_rel)` EXACTLY (dev 0.0 on all 9 bins) — one physical component carried in three CSV columns; the quadrature term list includes all three.
- Self-contradictory metadata: summary.json basis string (19 terms) vs STATUS.md "active 20-term quadrature is exact to floating precision" (20 terms, the double-counted list) vs the 21-item quoted_components / active_terms lists (all three detint columns). STATE.md line 296 repeats "21 inventory columns, and 20 active quadrature terms". The three documents cannot all be right; the arithmetic follows the double-counting one.
- Why it matters: the authoritative quoted budget does not compute the formula its own metadata declares. A consumer re-deriving the total from the published basis will not reproduce the published numbers. The defect propagates into the shipped covariance diagonals (covariance_gates explicitly equates the gate diagonal to the double-counted total^2 and "passes" at 5.4e-15), the public Born tables (checkpoint 4 claims table-vs-budget deviation 0.0), and STATE.md's 16-digit quoted medians.
- Impact: numerically negligible — the relative inflation of the total is 0.7 to 123 ppm across the verified bins (largest on empty-D bins; typical bins ~2-8 ppm), the quoted medians shift by of order 1e-5 to 1e-4 percent-points (6th significant digit), and the direction is conservative (inflating). No physics conclusion changes. Not analysis-killing, but the record is defective and the internal gates cannot catch it because they validate the defective formula. Must be corrected and re-frozen before further quoting.
- Classification: follow-up (regenerate with detint entered once; medians change below any quoting precision; make the term-count statement consistent across summary/STATUS/README/STATE).

### MAJOR

**M1. The builder of the authoritative budget is unlocatable; the provenance-pinned script provably did not produce the installed CSV.**

- Where: `systematics_bgsub_env100_21comp_2026-10-19/provenance_manifest.json` lines 3-7 pin `analysis_bgsub_final_scheme_2026-10-18.py` (sha f82a7df2...) as the edition's script file; lines 21-24 pin the same sha as source.
- Evidence: a string grep of the ENTIRE installed tree — including the sha-identical copy of the pinned script — for the product-defining strings (`total_with_env100_rel`, `retired_bgsub_stat_rel_audit_only`, `bgsub_env100_21comp_budget`, `env100_dated_budget_2026-10-19_v1`) matches only the CSV header and the metadata JSONs, never the script. The pinned script (fully read in this audit) computes the rev1 proposal variants (t20_add/t19p, `bgsub_env100_merged_rel = max(env100, empz25)`) — a different product with a different column set. Searches for the real driver found nothing in: `output_v6/studies/bgsub_final_scheme_2026-10-18/`, `output_v6/studies/bgsub_fitnorm_2026-10-19/` (a different, fitnorm campaign), `H1ChargedSidis_HighQ2/scripts/{systematics,studies,bgsub_fitnorm_2026-10-19}`, `sidis_highQ2/scripts/{systematics,background_subtraction}`. Scope caveat: greps over the full `output_v6/studies` and `sidis_highQ2` trees timed out; the search covered the plausible locations, not every directory.
- Why it matters: the QUOTED systematic cannot be regenerated from its pinned provenance; the C1 defect site cannot be quoted or fixed in place; no cluster ID or run record is registered for the producing job (contrast: the 19-comp tree records its driver and cluster 5721117). This breaks the freeze/reproducibility discipline every other product in this chain follows.
- Impact: provenance/reproducibility defect on the authoritative product; follow-up (locate or reconstruct the driver, pin it, regenerate under it).

### MINOR

**m1. Superseded S2 2026-10-18 tree coexists in data_release under a near-identical name — quoting hazard.**

- Where: `central/data_release/systematics_bgsub_env100_21comp_S2_2026-10-18/` vs `central/data_release/systematics_bgsub_env100_21comp_2026-10-19/`.
- Evidence: both trees sit in the public data_release area with "21comp" names but different schemas and totals (S2: `total_with_bgsub21_rel`, medians h+ 7.7989% / h- 8.1606%, composition includes `bgsub_env100_merged_rel = max(env100, empz25)`, `compound_track_quality_rel`, `labpt010_rel`; 2026-10-19: `total_with_env100_rel`, medians 7.3203% / 7.6371%). STATE.md's Env100 section names only the 2026-10-19 tree; no explicit supersession entry was found for the S2 tree, yet the 2026-10-19 provenance manifest lists the S2 CSV as an input.
- Why it matters: anyone globbing "21comp" in data_release can quote the wrong (higher, differently composed) totals.
- Impact: hygiene/quoting hazard; follow-up (explicit supersession note or relocation per house rules).

**m2. 19-comp h+ median sits below the loop-2 expectation bracket.**

- Evidence: the loop-2 bracket expected the h+ total median in [6.2, 6.3]%; the clean-denominator budget delivered 5.9764% (h- 6.2968%). Recorded in the 19-comp tree as informational, not a gate. The mechanism is visible in comparison_vs_budget15to19.json: re-denomination on ~1.086x smaller subtracted denominators; clean-vs-carried median deltas <= 0.17 pp; five "smaller than carried" flags recorded with mechanisms.
- Impact: no defect demonstrated; superseded as quoted by the env100 freeze anyway; follow-up only if the bracket was a physics expectation rather than a target.

### INFO

**i1. Gate circularity is what let C1 reach freeze.** The 26-item gate is internally sound (exit 0 iff no FAIL; PASS_WITH_RECORDED_EXCEPTIONS counts as pass; CP-5 re-parses the CSV from disk and re-verifies carried cells), but CP-5(a) is an internal-consistency check (CSV == its own record), and items IV-1..IV-6, SA-2..SA-5, RU-* trust upstream self-reported flags. The env100 tree's five checkpoints all validate against the same double-counted formula — "quadrature exact to floating precision" is true of the wrong formula. A gate that re-derived the total from the declared basis string would have caught C1.

**i2. env100 is a declared fiat envelope, not a measured uncertainty.** `bgsub_env100_rel = (e_avg + v0_avg)/data_sub'` is a flat ±100% normalization envelope on the Set-R' electron+V0 content, fully correlated across bins and charges (STATE.md line 298; covariance/README.md line 5). It dominates the quoted budget and must be quoted as an envelope. The bgsub_stat retirement it replaces is implemented correctly (removed exactly once — verified arithmetically; the audit-only column duplicates the retired value; SA-3 distinctness vs ibu_stat is explicit in carried_components_record.json).

**i3. Covariance-basis misalignment is open and user-gated.** The env100 tree's `covariance/` ships systematic + ibu_stat matrices only (non-lumi/env100 components bin-diagonal, declared conservative); the response-MC-stat term (C_MCstat) and the full per-source package exist only on the superseded bgsub19 basis (`data/covariance/z005_covariance_cleanrelease_2026-10-19/`), and STATE.md line 310 records env100-basis alignment as OPEN. Consumers must not mix the two bases. The known S7 prior-reweight unpropagated register entry (medians 8.82%/9.54%, exceeding both budgets' totals) also remains open.

## 3. Non-blocking checks confirmed (what was ruled out, and why)

- **19-comp budget arithmetic is sound**: telescoping quadrature == direct-19; independent 9-bin recomputation reproduces `total_with_bgsub19_rel` to <=1.1e-16; on-disk re-parse closure (validation 1.11e-16) consistent. Ruled out: sqrt-twice, sign errors, missing or duplicated components anywhere in the 19-comp chain.
- **Single denominator holds**: all 19 components on official IBU k=4 over data_sub' (nominal recompute vs official CSV <=1e-9; validation_checks.json single_denominator_statement; L 351000.0 nb^-1). The carried bgsub-4 are data_sub'-denominated by construction (`bgsub_stat_rel = sqrt(varB_R_prime_applied)/data_sub_prime`, `bgsub_model_rel = hypot(v0_halfspread/data_sub_prime, ...)` in extend_budget_15to19_conebase_v2.py) — "already bgsub-denominated" substantiated; no mixed-denominator carry. Ruled out: single-denominator violations.
- **Retirement and insertion arithmetic**: bgsub_stat removed exactly once; env100 inserted exactly once; env100 = 0 exactly on the three empty-D bins {pos/109, pos/179, neg/240} per the rev1 definition. Ruled out: bgsub_stat double-count, env100 double-count.
- **Units**: all *_rel columns are fractions; the *_percent columns equal 100x the corresponding rel columns (checked on verified rows). Ruled out: percent/fraction and relative/absolute unit errors.
- **Luminosity**: L_data = 351000.0 nb^-1 everywhere checked (driver L_DATA_NB, provenance lumi_nb_inv, STATE, validation statement); lumi component flat 0.027, never rescaled. Ruled out: stale 297657.8 or 356613.0.
- **Claimed extremes reproduce**: pos max 73.65119705575378% = pos/179 stored total; neg max 88.43670499154085% = neg/174 stored total; 19-comp maxima match comparison_vs_budget15to19.json (73.65114588316987% / 86.98614454944678%).
- **Carried components**: 7840-cell string-verbatim bit-identity vs budget_15to19; c_rad V3 recompute dev 0; tracking 16-seed canonical form (data-side conservative form caveat OPEN, recorded); phi_h 0.85-floor re-expression = carried ABS deltas / new nominal (recipe dev 1.4e-17). Ruled out: carried-component staleness in the 19-comp tree.
- **env100 tree consumes the pinned 19-comp CSV correctly** (row neg/0 byte-identical between both installed CSVs).
- **Detector-shift legs** run the full chain on the cone+S2'+Set-R' subtracted basis with per-DV guards (driver read); no wrong-basis legs found.
- Ruled out for every component other than detint in the env100 total: double counting, omission, or misweighting — the exact 9-bin fits leave zero residual for any other term.

## 4. Missing evidence

- The actual builder script of `bgsub_env100_21comp_budget.csv` and its cluster/run record (unlocated; needed to quote the C1 defect site and regenerate the product).
- Row-level re-derivation of the public Born tables (checkpoint 4) — only the claimed deviation-0.0 statement was available.
- An exhaustive full-workspace builder search (greps over the largest trees timed out).

## 5. Recommended next check

Locate (or reconstruct and pin) the 2026-10-19 env100 dated-budget driver, remove the duplicate detint term so the total implements the declared 19-active-term basis, regenerate CSV + covariance diagonals + public tables, and re-freeze with one consistent term-count statement across summary.json, STATUS.md, covariance/README.md, and STATE.md. Shortest decisive acceptance test: on the three empty-D bins the regenerated `total_with_env100_rel` must equal `total_with_bgsub19_rel` EXACTLY (env100 = 0 and bgsub_stat = 0 there).

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 1 of 3 (systematics), read-only; no analysis script was modified.
- Prior attempt produced no output; this retry completed the full audit.
- Report persisted by the delegating supervisor session (critic toolset read-only).

# Plan: closure uncertainty revision for the final eΣ release (2026-10-26, rev 3)

Status: rev 3 (2026-10-27). **Approved and amended per the program plan**
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/plans/2026-10-27-sidis-highq2-finalization.md`
(package P1; its Decisions 1-5 and 12-14). Approval trail: treatment R and the new-tag install
(user 2026-10-26, "3 yes.", memory `decisions/2026-10-26-closure-treatment-r-new-tag.md`); DIFFVM not
subtracted (user 2026-10-26, `decisions/2026-10-26-diffvm-not-subtracted-negligible-rho0.md`); no
DIFFVM uncertainty term, per-bin DIFFVM table instead (user 2026-10-27, program plan Decision 12);
"Approve and start P1" (user 2026-10-27). Rev 2 (2026-10-26) assumed a DIFFVM subtraction; rev 3
removes everything that rested on it. Executed by P1 (S0-S8) and P3 (S9). Nothing is submitted by
this plan's author.

Parent campaign: `SQ/docs/plans/esigma_xf_bg_revision_2026-10-24.md` (installed tag
`esigmaphotonxf_2026-10-24`, not frozen; the freeze is program-plan P4).

Path prefixes (all absolute under `WS` = `/data/dust/group/h1/klesthen/BaryonAsym`):
- `SQ` = `WS/sidis_highQ2`, `HQ` = `WS/H1ChargedSidis_HighQ2`
- `E` = `HQ/scripts/unfolding/esigma_final_2026-10-24` (release scripts in `E/release/`, budget in
  `E/budget/`, s08/s09 in `E/phaseD/s08s09/`)
- `STN` = `HQ/output_v6/studies/esigma_final_2026-10-24` (the 10-24 edition root; read-only here)
- `ER` = `STN/closure_rev3_staging` (the new edition root; created in S0, renamed nowhere)
- `STUDY` = `HQ/output_v6/studies/stat_coverage_2026-10-26` (stored toys; read-only here)
- `EVS` = `HQ/scripts/studies/stat_coverage_2026-10-26` (`EV` = `EVS/eval_treatment_v2.py`)
- `DVO` = `HQ/output_v6/studies/diffvm_datatuned_2026-10-26`, `VCO` = `HQ/output_v6/studies/vm_veto_cut_2026-10-26`
- `DR` = `HQ/output_v6/central/data_release`; `REL24` = `DR/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-24`
- `BUD24` = `DR/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-24/esph_budget_v2.csv`
- `NOTE` = `SQ/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ`, `PAPER` = `SQ/analysis_note/H1-ChargedSIDIS-Paper-HighQ`,
  `LETTER` = `SQ/analysis_note/H1-HighQ2-SIDIS-Letter`
- `SCS` = `SQ/docs/unfolding/stat_coverage_study_2026-10-26.md`, `DVS` = `SQ/docs/backgrounds/diffvm_datatuned_subtraction_2026-10-26.md`,
  `OTC` = `SQ/docs/backgrounds/diffvm_onetrack_check_2026-10-27.md`, `VETO` = `SQ/docs/backgrounds/vm_veto_cut_2026-10-26.md`
- `NEWTAG` = `esigmaphotonxf_<date of the S3 s08 run>` (D5)
- `TBL` = `SQ/data/backgrounds/diffvm_perbin_<NEWTAG>` (D24)

## Answer to the user's question (rev 1, condensed)

"Shouldn't unfolding DJANGOH with RAPGAP and vice versa already cover closure?" Partly. ibu_model
(`E/budget/components_groupA.py:33-36,358-362`) compares two unfoldings of the same data, so it
measures model dependence, not bias. The closure term unfolds pseudo-data with known truth through
the quoted RG chain and measures its bias (SCS:172; EV:6,10,32); RG→DJ tests an estimator we do not
quote. The two overlap partly (median closure/ibu_model 0.82, Pearson r 0.65; SCS:179-183), and the
calibration with both terms lands near nominal coverage, so both are kept (D12a). The D18 per-bin
cross-closure (`SQ/plots/docfigs/esigmaphotonxf_2026-10-24/closure/closure_perbin_esph.json`)
is not usable as the term: it carries unquantified response-MC noise, has no data-like leg, and half
of it tests the wrong estimator. It stays a diagnostic.

## Goal

Install a new eΣ edition `NEWTAG` whose release (central values, statistical uncertainties, Born
corrections) is the installed `esigmaphotonxf_2026-10-24` release unchanged, and whose systematic
budget and covariance are rebuilt with (1) the per-bin closure term `unfold_closure_rel` (treatment
R, D1-D3), (2) the response-MC statistical uncertainty in the quoted totals (D4), and (3) **no DIFFVM
row**: the ±100% elastic-ρ⁰ term leaves the quoted budget and the covariance and is not replaced
(D27). As additive data, P1 also produces the per-bin DIFFVM table (D24) that P3 prints in the note.
S9 (documents) is specified here and executed by program-plan P3.

Non-goals: no DIFFVM subtraction (user 2026-10-26); no DIFFVM uncertainty term of any size (user
2026-10-27); no change to the unfolding method, C_rad, φ_h, s02a, s02b, s03, the cut-variation
shifts (D20) or any release file; no VM veto in the nominal (D12b); no T4 unfolding; no S9 freeze
(P4); no change to the parent's D18 closure diagnostics; no new physics study.

Expected change size: three new scripts (closure term ~120 lines, coverage gate ~80, DIFFVM table
~80), edits to `components_groupA.py` (~40 lines), `esph_downstream.py` (~10), `esph_base_common.py`
(~15), tag copies of s08, s09, assemble, s15, s16, s18 with constant edits (~60 lines of deltas in
all), one `theory_chi2.py` (~80, diagnostic). About 9 Condor jobs. LaTeX: about 40 lines across note,
paper and letter (P3).

## Approach

Reuse the whole 10-24 data chain and rebuild only what the three budget changes touch: group A
(two new columns), the budget assembly (`esph_downstream.py` step_assemble), the covariance (s08),
the gates (s09), the coverage gate, the package and the install. The closure term is built from the
**stored** coverage toys, which depend only on the vector, the responses, the B1 floors and the
release yields, none of which changes (D25). New outputs go under `ER`; nothing under `STN` or
`STUDY` is written. Then the campaign pattern: assemble, `s15 --dry`, hostile review, user
sign-off, `s15`, `s16`/`s18`.

Rejected:
- Reuse D18 `closure_perbin_esph.json` as the closure term: noisy, one-legged, tests the wrong estimator (above).
- R′: lower coverage, loses the separate model term (D12a).
- An explicit closure–ibu_model cross-correlation in C: uncalibrated; the study calibrated
  independent sources (h1-unfolder).
- Smoothing or symmetrizing b: would undo the held-out calibration in the high-z/low-pT corner.
- Rerunning the 120 toy shards: nothing they depend on changed (D25); the sha check in S0 proves it.
- Replacing the ±100% DIFFVM row by a ±42% row: the user chose a table over any term (program plan Decision 12).
- Deleting the DIFFVM computation from group C: keeping it as an INFO column costs no code and
  feeds the table (D27).

## Decisions

Kept from rev 2 (numbers unchanged so the program plan's references hold):

- D1 **Closure definition.** b_t(i) = mean_toys(N_k4 − T_t)(i) / |release_yield(i)|, a yield-unit
  bias over the release yield. Here t ∈ {djangoh (RG response), datalike}, mode=data, per bin and per
  charge. Quoted magnitude: `unfold_closure_rel` = sqrt(½Σ_t b_t²). Reason: this is exactly the
  quantity whose coverage SCS measured (EV:6,10,32-34). `release_yield` = `yield_unfolded` of
  `REL24/ibu/cross_sections.csv`.
- D2 **Closure covariance.** `cov_unfold_closure` = ½Σ_t b_t b_tᵀ over the full 490-vector (245 bins
  per charge), with the same truth shared by h+ and h−. It is rank 2, and its diagonal equals the
  quoted term squared. It goes in as an independent source. Reason: calibrated recipe (SCS:224-226).
  Consequence for s09 g7 (`:122-128`): the closure term is cross-charge correlated and joins that
  check's list (today only the int term, the diagonals and the `_rank1` names are special-cased).
- D3 **Closure placement.** Group A next to ibu_model/ibu_reg (`E/budget/components_groupA.py:358-369`,
  COLUMNS `:104-106`, SIGNED_COLUMNS `:108`) and in `TERMS["A"]` (`E/esph_downstream.py:342`). Signed
  columns `unfold_closure_{dj,dl}_signed_rel` go into `INFO["A"]` (`:349-350`). In the ratio/A_h code
  the term joins `grp["ibu_model_reg"]` (s09:196-197).
- D4 **Response-MC stat in the quoted totals** (memory `decisions/2026-10-26-response-mc-stat-in-quoted-totals.md`).
  Group A emits `response_mcstat_rel` = sqrt(diag C_MCstat)/|yield| from the existing
  `STN/phaseD/s03/response_mcstat_toys/cov_{pos,neg}.npy` (s08 already forms this as
  `mcstat_diag_rel_median`, s08:211-212); it replaces `fakecol_mcstat_rel` in `TERMS["A"]`
  (`esph_downstream.py:342`), and `fakecol_mcstat_rel` moves to `INFO["A"]` (it must stay in the
  group A json: the s03 closure gate reads it, `s03_response_mcstat…:406-418`). The g2 identity
  (s09:84-96, line 89) is rewritten for the new column and becomes an identity; `BUDGET_FORMS`
  (`esph_base_common.py:140`) is re-keyed from fakecol to `response_mcstat_rel`. Change from rev 2:
  **s03 does not rerun** (its inputs are the vector, the release and the responses only; D25).
- D5 **Tag.** `esigmaphotonxf_<date of the S3 s08 run>` (parent D1). `ER` is created in S0 with a
  date-free name and renamed nowhere; s15 refuses existing destinations (s15:89-92), so the old
  edition and its install record stay untouched.
- D6 **Coverage gate.** Coverage ≥ 0.66 in **each** N class with N≥4 (4–5, 6–9, …, ≥500), for
  `ALL__both`: pooled over six truths, family held out (EV:49-55): DJ-family truths with b_dl only,
  DL-family with b_dj only, rapgap with sqrt(½(b_dj²+b_dl²)). The gate reads the signed columns of
  the built budget (`--budget ER/downstream/budget_v2/esph_budget_v2.csv`), never the quoted rms.
  Separate check: built columns equal the in-memory EV closure (max |Δ| < 1e-9).
- D7 **Theory χ²** for MAPTMD24 with C_total of both editions (the overlay
  `SQ/plots/docfigs/esigmaphotonxf_2026-10-24/letter/scripts/make_maptmd24_overlay_esigma.py`
  computes none). Diagnostic only, no gate: the central values do not move, so χ² changes only
  through C_total.
- D8 **Old-release comparisons are diagnostics only** (memory
  `conventions/2026-10-25-no-old-release-agreement-blockers.md`). Hard gates are identity and
  internal-consistency checks: g0–g7, gP3, ga10, gD5, PSD, sha/count, D6.
- D9 **Known non-coverage is quoted, not fixed.** R under-covers stressA (0.48–0.72) and
  djangoh_djresp at N≥500 (0.62). Say so in the note; do not enlarge κ.
- D10 **Document refresh** (P3) reuses the parent S8 tests (`numbers_match`, `no_old_tag`,
  `no_draft_numbers`, `d9_once`, latexmk; parent plan :425-430). Pushing note and papers after review
  is authorized (memory `decisions/2026-10-24-esigma-papers-and-note-push-authorized.md`).
- D11 **No N<10 flags** (memory `decisions/2026-10-26-no-low-stat-flag-all-bins-presented.md`).
- D12 **User answers 2026-10-26** (do not reopen):
  - a. Treatment R, κ = 1, new-tag install only after the D6 gate passes and hostile review
    (memory `decisions/2026-10-26-closure-treatment-r-new-tag.md`).
  - b. Strict generator-level observable, no VM veto; T4 is a cross-check only (memory
    `decisions/2026-10-26-observable-strict-no-vm-veto.md`).
  - c. (superseded the same day) DIFFVM is **not** subtracted
    (`decisions/2026-10-26-diffvm-not-subtracted-negligible-rho0.md`, supersedes the subtract-and-publish
    entry now under `.supervisor/archive/`). The low-Q² ±30% term `bgsub_lowq2_norm_rel` stays as is.
- D14 **One release, reused.** No vector or release pointer changes. `esph_downstream.py` gets one
  constant `EDITION_ROOT` (env `ESPH_EDITION_ROOT`, default the 10-24 root so existing behaviour is
  unchanged) that redirects only the group A output dir and the budget_v2 output dir to `ER`; group B
  and C inputs, VEC, MERGED and RELEASE stay at `STN`. Basenames `groupA_esph.json`,
  `esph_budget_v2.csv` and the summary's `version: esph_budget_v2` stay, so the s15 gate at `:186`
  holds. The v2 outputs under `STN` are the old edition's sha references (s15:46-48,
  `EVS/cov_common.py:44`) and must never be overwritten.
- D19 **Term counts.** Quoted 26 − 1 (DIFFVM out, D27) + 1 (closure) + 0 (D4 swap) = **26**;
  covariance 28 − 1 (`cov_diffvm_rho_rank1`) + 1 (`cov_unfold_closure`) = **28**. `N_TERMS`
  (`esph_downstream.py:359`), `N_TERMS_QUOTED`/`N_TERMS_COV` (s15:52-53) and the s16 texts
  (s16:291,314) need no change; sha constants, `N_MANIFEST`, `N_INSTALL` (s15:46-50) are re-pinned.
- D20 **Cut variations keep their existing relative shifts**, trivially: their vectors are unchanged.
  The s15 copy keeps the `cutvar_{nominal,prenominal}_bitwise` gates (s15:163-169) against `STN`.
- D22 **T4 cross-check in the note** (P3 text, P2 figure). A short passage in `NOTE/sections/backgrounds.tex`
  inside the existing ρ⁰ subsection (`:119-165`): the T4 definition (VETO:10,65), signal efficiency
  ≥ 0.93 in every populated bin (VETO:84-99), DIFFVM rejection, and observed versus predicted removed
  tracks in gen bin 30 at s = 0.0795 (8 observed vs 3.4–4.1 predicted). One figure from
  `VCO/eps_per_genbin.csv`, `VCO/reco980_counts.csv` and `DVO/veto/veto_check.json`, produced by P2.
  No T4 unfolding.
- D23 **Order and gating.** Nothing is gated on external studies any more (slots (a) and (b) are
  closed, see Struck). Ungated: S0-S6. Gated on the D6 PASS, the hostile review and the user's
  sign-off: the real `s15` (S7) and everything after it.

New in rev 3:

- D24 **Per-bin DIFFVM table** (user 2026-10-27, program plan Decision 12). One script,
  `E/diffvm_table/build_diffvm_perbin_table.py`, reads the `NEWTAG` budget CSV
  (`ER/downstream/budget_v2/esph_budget_v2.csv`: `charge, bin_index, global_bin_index, cell_label,
  q2_stratum, z_idx, pT_idx, nominal_cross_section_nb, diffvm_rho_signed_rel`) and
  `DVO/fit/s_final_r2.json` (s = 0.07952, σ_s = 0.14571; UL95 0.42 as a constant with its DVS:11
  derivation in the header). Output: `TBL/diffvm_perbin_<NEWTAG>_{pos,neg}.csv`, one row per gen bin
  (245 per charge, all bins, so a theorist applies it uniformly), columns:
  `charge, bin_index, global_bin_index, cell_label, q2_min, q2_max, x_min, x_max, z_min, z_max,
  pT_min, pT_max, sigma_nb, frac_full_template, frac_datatuned, unc_1sigma, unc_ul95, any_contribution, sign` (sign column added, user 2026-10-27: s_i = sign(`diffvm_rho_signed_rel`), used in σ→σ·(1 − s_i·`frac_datatuned`) and ρ_ij = s_i·s_j)
  where `frac_full_template` = |`diffvm_rho_signed_rel`| (the relative change of the Born cross
  section if DIFFVM at its own generator cross section, lumi-scaled by 0.135913, were subtracted),
  `frac_datatuned` = 0.07952 × that (the central subtraction a theorist would apply),
  `unc_1sigma` = 0.14571 × that (**the recommended uncertainty on that subtraction**; fully
  correlated, ρ = +1, across all bins and both charges because one global s drives it),
  `unc_ul95` = 0.42 × that (the conservative 95% bound on the unsubtracted contribution; OTC §4 shows
  it also covers a doubled lost-pion topology), and `any_contribution` = `frac_full_template ≥ 1e-3`.
  Bin edges come from `REL24/ibu/cross_sections.csv` joined on `charge, bin_index`. A Markdown
  rendering `SQ/docs/backgrounds/diffvm_perbin_table_<NEWTAG>.md` lists the `any_contribution`
  rows (about 90 across both charges; `BUD24` has 90 rows above 1e-3, 33 above 1e-2, max 0.3457 at
  h⁻ bin 30) and states that every other bin is below 0.1% even for the full template. The table
  is not part of the release package; `TBL` and the Markdown are its only outputs. "Fraction of
  tracks" is quoted at the unfolded cross-section level (the quantity a theorist subtracts), not as
  a reco track count; the CSV header comment states each column's definition together with s, σ_s
  and UL95 and their DVS line pointers. Reasons: the budget INFO column is a
  release product, reproducible by release code and sha-pinned through the budget CSV; the linear
  scaling is what a theorist does; the 1σ is the uncertainty of a subtraction, the UL95 the bound
  for not subtracting, and both are quoted so nobody has to rederive them. Cross-check (not primary):
  `DVO/unfold/dt_global_r2/per_bin.csv` (`central_shift_rel` at s = 0.0795, `newterm_rel` at ±σ_s)
  and `DVO/unfold/unsub_k042/per_bin.csv` (`unsub_k_rel`), exact IBU runs; the linear values agree
  with them to within 10% relative in every bin with `frac_full_template` ≥ 1e-2 (h⁻ bin 30: 2.75%
  linear vs 2.6% exact). Nothing in this table changes a central value or the budget.
- D25 **Stored toys and s03 are reused.** The data-mode shards `STUDY/toys/data/<truth>/<ch>/shard_000..009.npz`
  (6 truths × 2 charges × 10 shards × 100 toys) are generated from `inputs/nominal_inputs.npz`
  alone (`EVS/run_coverage_toys.py:59-66`), built by `EVS/prepare_inputs.py:129-140` from the vector,
  the RG/DJ responses, the migration ROOT, the B1 floors and the release ibu/bbb csv; the budget CSV
  enters only as the pass-through `other_rel` and `budget_total_rel`, which no toy computation reads.
  Reuse is proven in S0 by sha256 of those inputs against `STUDY/inputs/prepare_record.json`
  (`inputs_sha256`; the budget CSV entry is allowed to differ). s03 (`E/phaseD/run_s03_esph.py`)
  reads the vector, the release and the responses with fixed seeds (K=200; `:8,40`), so its
  `cov_{pos,neg}.npy` stand. If the S0 check fails, stop and report; the fallback (rerun
  `EVS/coverage.args` data shards on Condor) is not planned here.
- D26 **g7 and rank expectations.** s09 copy: `exp_rank` (`s09:77`) drops `cov_diffvm_rho_rank1` and
  adds `cov_unfold_closure: 2`; g7 (`:122-128`) treats the closure term as cross-charge correlated
  (D2); the ident tuple at `s08:104` drops `delta_diffvm_rho_rel` (else `conv[...]` raises KeyError)
  and gains `delta_unfold_closure_rel`.
- D28 **detint norm leg removed, user 2026-10-27** ("Fix it"): `detint_removal_rel` = hypot(model, stat) again and `cov_detint_norm_rank1` leaves the covariance (its class is already varied by `bgsub_env100_hyp_rel` and `nonprimary_int_rel`); 26 quoted / 27 covariance terms; installed as a follow-up tag (P1b) on a fresh staging root `STN/closure_rev3b_staging`.
- D27 **DIFFVM leaves the budget, stays as INFO.** Move `diffvm_rho_rel` from `TERMS["C"]`
  (`esph_downstream.py:347`) to `INFO["C"]` (`:352`, where `diffvm_rho_signed_rel` already is);
  remove `cov_diffvm_rho_rank1` from RANK1 (`esph_base_common.py:114`) and BUDGET_FORMS (`:159`),
  from the s08 RAW entry (`s08:72`) and the label at `esph_downstream.py:438`. The CSV total (`:376`)
  sums TERMS only, so INFO columns never enter a quoted number. Group C is **not** rerun
  (`E/budget/components_groupC.py:181-186,226-227` keep writing both keys). Docstrings mentioning
  the DIFFVM term (`esph_downstream.py:11`, `s09:263,281`, `components_groupC.py:2-5,86-89`) are
  updated in the copies.

Struck in rev 3 (one line each; the program plan's Decision 2 and 3 name them):

- D13 (subtracted vector d′ = n − s·T): DIFFVM is not subtracted; no vector is built.
- D15 (σ_s term replacing the ±100% row): no DIFFVM term of any size (user 2026-10-27); D27 removes the row.
- D16 (slot (a), SDME decay-model term): no DIFFVM term exists to attach it to; `SQ/docs/backgrounds/diffvm_sdme_check_2026-10-26.md`
  is done and OTC §3 shows the reweights move the template by ≤ 5%.
- D17 (bin-30 two-track excess term): user 2026-10-26, `decisions/2026-10-26-diffvm-high-mass-excess-no-study.md` ("no extra term").
- D18 (slot (b), diffractive mix-in): resolves to no change, `decisions/2026-10-26-no-diffractive-term-or-mixed-response.md`.
- D21 (published subtraction columns and sidecar): nothing is subtracted; D24 is the additive table instead.
- Rev-2 S1 (vector producer, pointers, staging chain) and its tests `only_diffvm_moves`, `reproduces_study`
  (staging), `bin30_shift`, `sigma_s_term`: no vector change, no staging chain.
- Rev-2 S2 (close slots (a)/(b)): both closed by the decisions above; nothing to record at execution time.
- Rev-2 OQ1 (does (b) gate the install): moot, slot (b) is closed.
- Table-generator change (two D21 columns) and the `sub_columns`, `no_unsubtracted_claim` tests: the
  cross-section tables keep their format; the DIFFVM table is separate (D24).

## Outline

- **New** `E/closure_rev/build_closure_term.py` (+ `.sub`: 1 CPU, 4 GB, `executable = /usr/bin/python3`,
  pattern `EVS/eval_treatment_v2.sub`): reads the 40 shards of truths `djangoh` and `datalike`
  (`STUDY/toys/data/{djangoh,datalike}/{pos,neg}/shard_*.npz`, fields `N` (100,10,245), `truth`
  (245); `bias` = mean(N_k4 − truth), EV:11,32-34) and `REL24/ibu/cross_sections.csv`; writes
  `ER/closure_rev/closure_term.npz` (`b_dj`, `b_dl`, `unfold_closure_rel`, shape (490,),
  `bin_index`, `charge`) plus `.json` (medians, input shas). Gotcha: 0 is falsy, compare counts
  explicitly (START_HERE §7, `SQ/docs/handoffs/esigma_release_2026-10-24/START_HERE.md`).
- **New** `E/closure_rev/eval_coverage_gate.py` (+ `.sub` as EV's): imports EV's functions, reads all
  120 data shards, `--budget`, `--out ER/closure_rev/gate.json`; implements D6; exit 0 only on PASS.
- **New** `E/closure_rev/theory_chi2.py` (+ `.sub`): MAPTMD24 vectors as the overlay script reads
  them, C_total of both editions (`REL24/covariance/`, `ER` covariance tree); writes
  `ER/closure_rev/theory_chi2.json` (χ², ndf per charge and combined).
- **New** `E/diffvm_table/build_diffvm_perbin_table.py` (login node, three small files): D24.
- `E/budget/components_groupA.py`: COLUMNS `:104-106` + `unfold_closure_rel`, `response_mcstat_rel`;
  SIGNED_COLUMNS `:108` + the two closure signed columns; a new STEP block after `:369` reading
  `ER/closure_rev/closure_term.npz` and `STN/phaseD/s03/response_mcstat_toys/cov_{pos,neg}.npy`
  (yield from the release csv); docstring `:6-15`. Output dir from `EDITION_ROOT` (D14). Gotcha: a
  missing signed column calls `C.stop` in s08 with no fallback.
- `E/esph_downstream.py`: `EDITION_ROOT` (D14) for the group A output dir and the budget_v2 output
  dir (readers/writers at `:270,360,363,392-395,429` as rev 2 listed; verify before editing); TERMS
  `:342` (+closure, fakecol→response_mcstat), `:347` (−diffvm); INFO `:349-350` (+2 signed, +fakecol),
  `:352` (+diffvm_rho_rel); label `:438`; plot title/dir `:380,445` ("26 terms" stays true).
- `E/phaseD/s08s09/esph_base_common.py`: RANK1 `:101-118` − `cov_diffvm_rho_rank1` (`:114`); a rank-2
  entry for `cov_unfold_closure` beside `INT_TERM` (`:119`, the only rank-2 pattern); ORDER `:132-133`;
  BUDGET_FORMS `:135-161` (−`:159`, +closure, re-key `:140`); `protected_files()` `:58-69` + the
  closure npz. Paths under `ER` via the same constant.
- Tag copies `s08_esph_base_<NEWTAG>.py`, `s09_esph_base_<NEWTAG>.py` (from the 10-24 ones in
  `E/phaseD/s08s09/`): s08 RAW `:60-80`, terms `:84`, ident `:104`, term_plan `:215`, output under
  `ER/phaseD/package/`; s09 D26, g2 (`:84-97`), gP3 (`:109`), g6 (`:121`), ga10 (`:129-136`, adds b_t),
  texts `:263,281`. Both copies write only under `ER`.
- Tag copies `E/release/assemble_package_<NEWTAG>.py` (second root `ERN = ER` for budget `:79-80`,
  covariance `COV_SRC :34,81-83` and the s08 provenance entries; release/born/cgammaborn/cutvar
  stay at `STN`), `s15_install_<NEWTAG>.py` (TAG, PKG, REL,
  BUDN, COVN `:31-35`; PAIRS `:38-41`; shas `:46-48`; `N_MANIFEST`,
  `N_INSTALL` `:49-50`; superseded lists `:54-60`), `s16_promote_<NEWTAG>.py`,
  `s18_close_quoting_blockers_<NEWTAG>.py` (TAG/BUDN/REL strings; 21 and 6 targets unchanged).
  assemble gets a `.sub`.
- Pending note fixes carried from rev 2 (P2 for producers, P3 for captions): Figs 41/42 `fig.suptitle`
  (`SQ/plots/releases/_build/esigmaphotonxf_2026-10-24/U2_cutvar_ratio/producers/plot_ratio_esph.py:85`);
  Fig 30 caption `NOTE/sections/systematics.tex:497`; gen φ_h `labelleft=False`
  (`…/U6_phi_h/producer_gen/plot_phi_rad_nonrad_gen_v3_compact_esxf2.py:2081,2246`); Figs 3/4 captions
  `backgrounds.tex:92-105`; Figs 10–13 captions `systematics.tex:212-247` (producer uncertain, confirm
  first); Figs 26/31 captions `systematics.tex:461,506`; reco φ_h legend
  (`…/U6_phi_h/producer/plot_phi_reco_luminorm_v6.py:393`).

## Slices

0. **Preconditions and reuse proof (login node).**
   - Delivers `ER/` created; `ER/S0_reuse_check.json` with sha256 of the D25 inputs against
     `STUDY/inputs/prepare_record.json`; baseline shas of `STN/downstream/budget_v2/`,
     `budget_groupA_v2/`, `STUDY/`, `REL24/`, `BUD24`.
   - Tests: `toys_reusable`: every `inputs_sha256` entry except the budget CSV → equal;
     `s03_present`: `cov_pos.npy`, `cov_neg.npy` exist with `job_report.json`.
   - Checkpoint: the json says PASS; `h1-data-validator` confirms. On FAIL: stop, report (D25).
1. **Code (login node, no runs).**
   - Units (disjoint files): (1) `closure_rev/` three scripts; (2) `components_groupA.py`,
     `esph_downstream.py`, `esph_base_common.py`; (3) s08/s09 copies; (4) assemble/s15/s16/s18 copies;
     (5) `diffvm_table/`.
   - Tests: `py_compile` on every touched file; `terms_26_28`: a login-node import of the edited
     `esph_downstream` asserts `len(TERMS) == 26` and the s15 copy's constants read 26/28;
     `default_unchanged`: with `ESPH_EDITION_ROOT` unset, the module's output paths equal the 10-24 ones.
   - Checkpoint: all three tests pass in one JSON check; `h1-analysis-critic` reads the diff.
2. **Closure term (Condor, 1 job).** `build_closure_term.py` → `ER/closure_rev/closure_term.npz`.
   - Tests: `reproduces_ev`: EV in-memory closure on the same shards → max |Δ| < 1e-9; `shape`
     ((490,), finite, ≥ 0); `medians` (closure/σ_stat ≈ 0.32, N<10 ≈ 0.71, ± 0.02; SCS:186).
   - Checkpoint: job exits 0; medians printed on the login node. Specialist: `h1-unfolder`.
3. **Budget and covariance at NEWTAG (Condor, 4 jobs: group A, step_assemble, s08, s09).**
   - Delivers `ER/downstream/budget_groupA_v2/groupA_esph.json`, `ER/downstream/budget_v2/esph_budget_v2.csv`
     + summary, `ER/phaseD/package/z005_covariance_<NEWTAG>_base_<date>/`, s09 gate json. The s08
     run date fixes `NEWTAG` (D5).
   - Tests: `n_terms` (26/28, D19); `diag_identity` (diag C_unfold_closure = closure²·σ², rel. Δ <
     1e-12); `rank` (closure 2, every `_rank1` 1, int 2); `g_all` (g0–g7, gP3, ga10, gD5, PSD PASS);
     `g2_identity` (D4, 1 ± 1e-9); `no_diffvm_term`: `diffvm_rho_rel` absent from TERMS, present as a
     CSV column, and the quoted total excludes it; `v2_untouched` (S0 baseline shas unchanged);
     `bin30_total` (h⁻ ≈ 19.5% before closure, from 39.7% with the 34.6% row removed in quadrature;
     reported, not gated).
   - Checkpoint: s08/s09 exit 0 and the s09 gate json is all PASS. Specialist: `h1-systematics`.
4. **Ratio, A_h, χ² (Condor, 1 job for χ²; ratio columns come from s09).**
   - Tests: `ratio_growth` reported against ×1.006 median (not gated); `chi2_written` (both editions,
     per charge and combined). Checkpoint: the χ² job exits 0.
5. **Coverage gate (Condor, 1 job).** Tests: `budget_column_matches` (max |Δ| < 1e-9); `coverage`
   (each N≥4 class, held out, ≥ 0.66). Checkpoint: exit 0 and `ER/closure_rev/gate.json` PASS.
   Specialist: `h1-systematics`. On FAIL: stop before any install and report (program plan Risks).
6. **DIFFVM table (login node).** `build_diffvm_perbin_table.py` → `TBL/…_{pos,neg}.csv`,
   `SQ/docs/backgrounds/diffvm_perbin_table_<NEWTAG>.md`.
   - Tests: `rows_490` (245 per charge); `any_count` (rows with `any_contribution` = count of
     `frac_full_template ≥ 1e-3`, expected ≈ 90, reported); `matches_study` (D24 cross-check, rel.
     |Δ| < 10% for every bin with `frac_full_template` ≥ 1e-2, all three columns); `scale_header`
     (column definitions, s, σ_s, UL95 and their DVS line pointers in the CSV header comment).
   - Checkpoint: all four tests pass; `h1-analysis-critic` reads the Markdown for the stated derivation.
7. **Package and install (NEWTAG).** assemble (Condor, 1 job) → `s15 --dry` (login) → hostile review →
   user sign-off → `s15` (Condor, 1 job).
   - Tests: `dry_ok` (all hard gates PASS, counts match the re-pinned constants);
     `refuses_existing`; `old_untouched` (10-24 install record sha);
     `release_identical` (every `release/` file in the package sha-equal to `REL24`).
   - Checkpoint: `DR/INSTALL_RECORD_<NEWTAG>.json` written, 0 hard-gate failures. Specialists:
     `h1-analysis-critic` (hostile review), `h1-naf-operator` (jobs), `h1-data-validator` (installed files).
8. **Promotion (login node).** s16/s18 copies, `--dry` then real. Tests: `s16_all_edited` (21/21),
   `s18_targets` (6/6), `no_low_n_flag` (D11), `backups` in `SQ/docs/promotions/<NEWTAG>_preedit/`.
   Checkpoint: promotion record written; `s7`-style post-install audit PASS (program plan Slice 1).
9. **Documents (program plan P3; `writer` with `h1-analysis-note-writer`, `h1-systematics`, `h1-analysis-critic`).**
   - Note: `NOTE/sections/backgrounds.tex:139-142` → not subtracted, **no uncertainty assigned**, the
     per-bin table (D24 `any_contribution` rows, both charges; columns fraction at s = 0.080, 1σ and
     UL95 bounds) and the reading rule for a theorist who subtracts; drop `\diffvmSystPct`
     (`NOTE/main.tex:61`); third figure per program plan P2 (the ρ⁰ subsection keeps 3 figures; the
     table replaces none of them: none shows per-bin fractions); D22 T4 passage; omit the one-track
     ΔE < 0 excess (user 2026-10-27, program plan Decision 13). `systematics.tex:52-57` bullet (one
     normalization term, not two), `:117` row removed, `:181-184` caveat reworded, closure and
     response-MC-stat rows added, `:55-56` medians; `summary.tex:44-47` medians; tables
     `NOTE/tables/highQ2_z005_<NEWTAG>_born_public*/`.
   - Paper: `PAPER/paper_full.tex:228` item (not subtracted, table in the note, no term), `:264` row
     removed, medians `:234`, closure/response-MC rows. Letter: `LETTER/sections/observables_method.tex:32-35`
     table, `LETTER/sections/sm_crad_bins.tex:2-4` provenance.
   - Tests: D10 set; `no_diffvm_row` (no DIFFVM line in any budget table of the three documents);
     `diffvm_table_present` (once, values equal `TBL`); `t4_passage` (once); `no_onetrack_excess`;
     `fig41_no_title`; `genphi_ylabels`; latexmk ×3.
   - Checkpoint: latexmk exits 0 for all three; `h1-analysis-critic` PASS; commits per repo.

## Verification

Rerun `eval_coverage_gate.py` against the installed `NEWTAG` budget and require the D6 PASS. Confirm
`DR/INSTALL_RECORD_<NEWTAG>.json` lists 26/28 terms, no DIFFVM term, the closure and response-MC
columns, that every installed release file is sha-equal to `REL24`, and
that `numbers_match` holds across note, paper and letter (P3).

## Risks

- **Peer session or OpenCode server writes here.** Re-check sha256 before each edit (START_HERE §6).
- **IBU null gates** reproduce only within one numpy build (START_HERE §7): run s08/s09, the closure
  term and the gate on the same Condor image.
- **Python environment**: `env -i /usr/bin/python3 -I`. numpy exists on the login node (1.23.5 under
  `/usr/bin/python3 -I` on 2026-10-27) but differs from the Condor image, so the IBU gates, the
  closure term and the coverage gate run on the Condor image; small npz or CSV reads (S0, S6) may
  stay on the login node.
- **The coverage gate may fail at NEWTAG** (closure medians differ from SCS): stop before install.
- **`EDITION_ROOT` left unset** in one caller silently overwrites the 10-24 v2 outputs: `protected_files()`
  and the S0 baseline shas (`v2_untouched`) catch it.
- **Untested DJ-leg MC noise** (SCS:254); R may be slightly conservative there.
- **g7** may need the closure term listed explicitly (D2, D26); a g7 FAIL on the closure block is a
  list omission, not a physics finding.

## Scope Notes

- In scope: closure term, response-MC in totals, DIFFVM term removal (as INFO), covariance and
  gates, ratio/A_h, χ² diagnostic, DIFFVM table, package, install, promotion; S9 as P3's spec.
- Out of scope: see Non-goals; any figure production (P2); handoff/RUN_STATUS text (P5).
- Do not reopen: D12, program plan Decisions 12-14 (DIFFVM table, no term; omit the one-track excess;
  P6 commit scope); no extra Born-shift term (memory
  `decisions/2026-10-25-born-shift-no-extra-uncertainty.md`), tight-DCA κ
  (`decisions/2026-10-25-tight-dca-kappa-100-exceedances-quoted.md`), TUnfold nominal only
  (`decisions/2026-10-25-tunfold-detector-shifted-vectors-dropped.md`), everything rebuilt from eΣ
  inputs (`decisions/2026-10-24-esigma-rebuild-everything-no-inherited-terms.md`), the DIFFVM
  normalisation 0.135913 (`SQ/docs/backgrounds/diffvm_normalization_audit_2026-10-26.md`), and the
  user-dispositioned items in START_HERE §6.
- Login node: one file, ≤ 5 min; everything else Condor via `h1-naf-operator`.

## Assumptions

- The 10-24 release under `REL24` is byte-identical to `STN/downstream/release/esph660` (check: S0 shas).
- s08/s09 accept output roots under `ER` by constant edit as they did for the 10-24 tag (check: `py_compile` + first s08 run).

## Expected changes in the numbers

Estimates (SCS, DVS, h1-systematics 2026-10-26).
- Central values and statistical uncertainties: identical to `esigmaphotonxf_2026-10-24`.
- DIFFVM: the 34.6% (h⁻ bin 30) row disappears from the quoted total; that bin's total 39.7% → ≈ 19.5%
  before closure and response-MC. The table shows 2.75% (s = 0.080), 5.0% (1σ), 14.5% (UL95) there.
- Closure plus response-MC: per-bin total ×1.018 median, ×1.15 p90, ×1.83 max (h1-systematics);
  closure/σ_stat median 0.32 (SCS:186).
- Ratio and A_h totals: ×1.006 median, ×1.06 p90.
- Coverage at N≥4 (SCS:23,32): 0.61–0.66 → 0.67–0.70.

## Cost

Condor: S2 closure (1), S3 group A, step_assemble, s08, s09 (4), S4 χ² (1), S5 gate (1), S7
assemble and s15 (2): about 9 jobs, each minutes. Login node: S0 shas, S1 code, S6 table, `--dry`
runs, s16/s18. No toy rerun, no s02/s03 rerun, no release rebuild.

## Open Questions

None. OQ1–3 of the program plan were answered by the user on 2026-10-27 (program plan Decisions 12–14).

## Log

- 2026-10-27: S0-S9 executed at `esigmaphotonxf_2026-10-27` (P1, P3). D28 (detint ±100% normalisation leg removed as a double count) led to the follow-up tag `esigmaphotonxf_2026-10-27b` on `STN/closure_rev3b_staging` (26 quoted / 27 covariance terms), installed, promoted and frozen 2026-10-27 (`FREEZE_RECORD_esigmaphotonxf_2026-10-27b.json`, sha256 `77f14dda…`).

## Todo

Executor: each step names its script, Condor use, acceptance and the H1 specialist to brief.

- [x] S0 `ER` created; `ER/S0_reuse_check.json` PASS (`toys_reusable`, `s03_present`, baseline shas) — login — `h1-data-validator`
- [x] S1 `E/closure_rev/{build_closure_term,eval_coverage_gate,theory_chi2}.py` + `.sub`; `E/diffvm_table/build_diffvm_perbin_table.py`; groupA/esph_downstream/esph_base_common edits (D3, D4, D14, D27); s08/s09/assemble/s15/s16/s18 `<NEWTAG>` copies (D19, D26) — login — `py_compile`, `terms_26_28` (~~28 covariance terms~~ struck by D28: 27 at 27b), `default_unchanged`; `h1-analysis-critic` diff read
- [x] S2 `build_closure_term.py` → `ER/closure_rev/closure_term.npz` — Condor 1 — `reproduces_ev`, `shape`, `medians` — `h1-unfolder`
- [x] S3 group A → step_assemble → `s08_<NEWTAG>` → `s09_<NEWTAG>` under `ER` — Condor 4 — `n_terms`, `diag_identity`, `rank`, `g_all`, `g2_identity`, `no_diffvm_term`, `v2_untouched` — `h1-systematics`, `h1-naf-operator`
- [x] S4 ratio/A_h columns (s09) and `theory_chi2.py` → `ER/closure_rev/theory_chi2.json` — Condor 1 — `chi2_written` — `h1-systematics`
- [x] S5 `eval_coverage_gate.py --budget ER/downstream/budget_v2/esph_budget_v2.csv` → `ER/closure_rev/gate.json` — Condor 1 — `budget_column_matches`, `coverage` PASS — `h1-systematics`
- [x] S6 `build_diffvm_perbin_table.py` → `TBL/diffvm_perbin_<NEWTAG>_{pos,neg}.csv`, `SQ/docs/backgrounds/diffvm_perbin_table_<NEWTAG>.md` — login — `rows_490`, `any_count`, `matches_study`, `scale_header` — `h1-analysis-critic`
- [x] S7 `assemble_package_<NEWTAG>.py` (Condor 1) → `s15_install_<NEWTAG>.py --dry` (login) → hostile review (`h1-analysis-critic`) → user sign-off → `s15` real (Condor 1) → `DR/INSTALL_RECORD_<NEWTAG>.json` — `dry_ok`, `refuses_existing`, `old_untouched`, `release_identical` — `h1-naf-operator`, `h1-data-validator`
- [x] S8 `s16_promote_<NEWTAG>.py`, `s18_close_quoting_blockers_<NEWTAG>.py` `--dry` then real — login — `s16_all_edited` 21/21, `s18_targets` 6/6, `no_low_n_flag`, `backups`; post-install audit PASS — `h1-data-validator`
- [x] S9 (P3) documents per Slice 9 — `no_diffvm_row`, `diffvm_table_present`, `t4_passage`, `no_onetrack_excess`, D10 set, latexmk ×3 — `h1-analysis-note-writer`, `h1-systematics`, `h1-analysis-critic`
- [x] Verification on the installed tag (gate rerun, install record, `release_identical`); report to supervisor for P2–P6 — at 27b: install record 55/55 gates, post-install audit 8/8 PASS, freeze_verify PASS; the coverage gate ran on the staged budget (`budget_column_matches` PASS)

# PROMOTION RECORD — non-primary (STR-only) reco-track rate uncertainty v5 (2026-10-21)

- **Authorization:** user baseline decision 2026-10-21 ("Re-base covariance + install"), after three hostile-critic rounds:
  - physics / definition;
  - statistics / unfolding;
  - implementation / independent reproduction.
  Final round: all three SOUND-WITH-CONDITIONS, no blockers, and all conditions closed.
- **Category:** systematics + backgrounds (definition ruling).
- **Status:** INSTALLED, additive. QUOTING REMAINS BLOCKED on the pre-existing OPT-A conditions: M1 env100 builder
  reconstruction, N2 HFS-Empz channel, plotter band re-pointing, analysis-note update.

## 1. What is promoted

| artifact | path |
|---|---|
| quoted budget | `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_optA_2026-10-21/` (`bgsub_env100hyp_nonprimary_optA_budget.csv`, 490 rows; c1fix columns verbatim + 25 v5 columns) |
| quoted total | `total_quoted_with_nonprimary_v5_rel` = √(total_quoted² − bgsub_env100² + bgsub_env100_hyp² + nonprimary_norm_v5²); medians **h+ 7.4475% / h− 7.7992%** (were 7.3121% / 7.6243%); increment p95 +0.39 / +0.32 pp, max +0.80 / +0.78 pp |
| covariance edition | `H1ChargedSidis_HighQ2/data/covariance/z005_covariance_optA_nonprimary_v5_2026-10-21/`, an edition of `z005_covariance_optA_2026-10-20/` (source read-only, unchanged) |
| covariance changes | `cov_env100_rank1` → `cov_env100_hyp_rank1`; **new** `cov_nonprimary_int` (rank-1 per charge, zero cross-charge block) and `cov_nonprimary_strmu_rank1`; all other terms byte-identical copies |
| covariance totals | C_total with lumi medians **7.3655% / 7.5999%** (source 7.2480% / 7.4952%) |
| install record | `output_v6/central/data_release/INSTALL_RECORD_nonprimary_v5_2026-10-21.json` |
| installer | `H1ChargedSidis_HighQ2/scripts/unfolding/trackclass_variant_2026-10-20/s10_install_nonprimary_v5_2026-10-21.py` (Condor 5807767, exit 0) |
| plots | `sidis_highQ2/plots/systematics/nonprimary_norm_v5_overlay_2026-10-21/` (7×7 overlay per charge, panel labels from the binmap; per-bin and ratio/A_h scans) |

## 2. Definition (final)

- **Background provenance ruling (user, 2026-10-21).** A reco MC track is background only if its H1OO truth link is
  STR-only (`IsStr && !IsGtr`, GEANT-created). Every GTR-linked π/K/p is unfolded as migration, including:
  - any P/GenP, angle or charge;
  - split duplicates;
  - Tier-2 matches.
  Kinematic-consistency cuts and dedupe are not background definitions. The 2026-10-21 triple-check found real tracks
  in the v1 "absorbed / Tier-2 / P/GenP>1.5" classes.
- **Nominal response unchanged** (OPT-A). The K± keeper stays signal and its rate is not varied.
- **Sources** (fake-column content per provenance class, exported by
  `build_migration_trackclass_variant.py` md5 `c9af59df54d1a750a546fd0204299de0` with routing unchanged; shards
  `output_v6/studies/trackclass_variant_2026-10-20/shards/v4cat/`, response bit-identical to OPT-A on 174/174 shards):
  - **int:** STR-only π/K/p with a non-hyperon, non-K± direct mother, plus STR non-π/K/p non-μ. κ = ±100%
    **conservative fiat**. Independent; ρ = 0 across charges.
  - **hyp:** STR-only π/K/p with a hyperon direct mother. **The same nuisance as the V0 part of `bgsub_env100`**
    (user ruling). The joint re-unfold (data_sub′ − κ(e+V0) together with F + κC_hyp) gives an increment that is
    added to the installed env100, giving `bgsub_env100_hyp_rel`. This is a mixed-definition convention, since the
    installed env100 is not reproduced by a re-unfold (M1 open). Empty-bin-guard bins get no e+V0 data shift.
  - **strmu:** STR-only μ (π/K decay in flight). Re-sized from the DJ14–RG31 spread of the muon fraction per (cell, z):
    a noise-level placeholder (integrated −0.02%), partly contained in `ibu_model_rel` (conservative double count).
- **Charge ratio / A_h (mixed rule, user):** int ρ = 0; env100_hyp and strmu ρ = 1. Background-related terms on h+/h−
  integrate to ±0.58% (env100-only ±0.27%); A_h ±0.0029.
- **Born level:** the term is relative and level-invariant (C_rad multiplicative per bin, verified).

## 3. Validation

**Budget:**
- base cells verbatim;
- total closure 0.0;
- int/strmu quadrature 0.0;
- finite and non-negative;
- env100_hyp signed vector ≥ 0.

**Covariance gates (all PASS):**
- g0: source composition reproduced;
- g1: PSD and expected ranks (env100_hyp 1, int 2, strmu 1);
- g2: diag(C_new) − diag(C_old) = budget change per bin, ≤ 1e-12;
- g3: denominator alignment;
- g4: correlation twins;
- g5: sha sweep of protected inputs unchanged;
- g6: swap identity;
- g7: int cross-charge block exactly 0;
- ga10: duplication guard.

**Critic reproduction:**
- every deterministic v5 column to ≤ 2.6e-15;
- covariance rebuilt to 4e-16;
- the installed env100 cross-charge block verified as lumi⊗lumi + env100⊗env100, all positive.

## 4. Superseded (retained untouched)

- `total_quoted_rel` of `systematics_bgsub_env100_optA_c1fix_2026-10-20/` as the quoted total. It is the source
  edition of the new budget and stays the reference.
- `z005_covariance_optA_2026-10-20/` C_total for consumption. It is the source edition; never add its
  `cov_env100_rank1` to the v5 `cov_env100_hyp_rank1`.
- **Non-primary studies:**
  - v1 (2026-10-20, ±2.8% / ±2.2%): **WITHDRAWN**, real tracks in the background classes.
  - v2 (±2.07% / ±1.39%), v3 (±0.93% / ±0.82%) and v4 (±1.00% / ±0.98%): superseded study editions.

## 5. Carried caveats / open items

- κ_int = ±100% is a fiat. H1 1302.1321 constrains secondary interactions to ~1% after the nuclear-interaction
  correction. NucliaWeight (`H1NuclIACor`: 1 − ani_ineff, "nuclear i.a. undescribed by MC") implies data have ~1.5%
  more interactions per track.
  - Upgrade path: a DJ/RG or GHEISHA charge-composition estimate for int. That would also tighten h+/h− and A_h, which
    are driven by the int fiat at ρ = 0.
- **Not covered by any term:** the correlated primary-loss side of a material-interaction change. There is no
  NucliaWeight uncertainty column; this is a pre-existing gap.
- Per-bin maxima of the new terms and per-bin ρ=1 ratio values are not robust: quote integrated values and
  distributions.
- **Found during this work (not fixed here):**
  - The 2026-10-19 env100 fixed-y plot producer hard-codes wrong Q²/x panel labels. The plotted data are correct.
  - The detailed covariance package lineage is unaffected.

## 6. Records

Under `sidis_highQ2/docs/systematics/nonprimary_v5_2026-10-21/`:
- `nonprimary_v5_baseline_candidate_2026-10-21.md`: definition, results, round-3 closure.
- `nonprimary_norm_v3_critic_review_2026-10-21.md`: rounds 1–3, decisions.
- `trackclass_v3_provenance_2026-10-21.md`: provenance ruling.
- `trackclass_triplecheck_2026-10-21.md`: the class validation that withdrew v1.
- `trackclass_v2_selection_spec.md`: superseded.
- `trackclass_impact_2026-10-20.md`, `nonprimary_norm_term_2026-10-20.md`, `track_truth_classification_study_2026-10-20.md`:
  v1, withdrawn.
- `detint_uncertainty_audit_2026-10-20.md`: the original gap audit.

Critic scratch: `output_v6/studies/trackclass_variant_2026-10-20/critic2/`, `critic3/`.

**Condor clusters:**
- 5803732 / 5803906 / 5803813: variant shards;
- 5804013: v4cat export;
- 5804049 / 5805679: v5 term;
- 5807767: install.

**Concurrency note:** a peer session and an OpenCode server were active on this analysis. All canonical edits were
re-read and applied additively, with a sha check immediately before each write. Pre-edit copies are in
`docs/promotions/nonprimary_v5_2026-10-21_preedit/`.

## 7. Promotion summary (baseline-update checklist, executed 2026-10-21)

- **Step 1 — artifacts verified:**
  - budget: 5 files;
  - covariance edition: 96 files, validation all_pass;
  - plots: 17 files;
  - records: 9 docs plus this record;
  - install record over 100 files.
- **Step 2 — category:** systematics + backgrounds.
  - Siblings updated: systematics, backgrounds, state, baseline-update, lessons, docmap, plotting, results.
  - Verified unaffected: normalization, unfolding, radiative, builder, operations, mc, aziasym. The central values,
    response, C_rad, luminosity and trees are unchanged.
- **Steps 3–8 — edits** (`s11_promote_nonprimary_v5_2026-10-21.py`; additive, except that the "Last updated" header lines (STATE.md, RUN_STATUS) and the handoff startup-card line were replaced by prepended versions that keep the old text in their "prior:" chain, and one blank line in COVERAGE_MANIFEST was dropped; sha-guarded against concurrent
  writers, 0 skipped; 17 pre-edit copies in `docs/promotions/nonprimary_v5_2026-10-21_preedit/`):
  - `STATE.md`: "Last updated" entry, a superseded-list bullet, and the override section
    "2026-10-21 Non-primary (STR-only) Rate Uncertainty v5 Baseline Override".
  - `docs/RUN_STATUS.md`: "Last updated" entry, an ALLOW bullet, a Current Release Summary row, and the override
    section.
  - `docs/status/current-release.md`: the override section.
  - `context-handoff.md`: the startup-card update and the override section.
  - Both `RUNLIST.md` copies and `SEVEN_CATEGORY_SCRIPT_INDEX.md`: registry entries (script md5s).
  - Eight skills: an Authoritative Facts bullet, plus WRONG/RIGHT traps (systematics, backgrounds, lessons) and
    Canonical References rows (systematics, backgrounds, baseline-update, docmap, plotting, results).
  - `ownership.tsv` and `COVERAGE_MANIFEST.md`: 15 new entries.
- **Superseded (old → new):**
  - quoted total `total_quoted_rel` 7.3121% / 7.6243% → `total_quoted_with_nonprimary_v5_rel` 7.4475% / 7.7992%;
  - `bgsub_env100_rel` in totals → `bgsub_env100_hyp_rel`;
  - covariance `z005_covariance_optA_2026-10-20` C_total (7.2480% / 7.4952%) → edition `..._nonprimary_v5_2026-10-21`
    (7.3655% / 7.5999%);
  - non-primary v1 (withdrawn), v2–v4 (superseded).
- **Step 9 — verification grep:** the remaining "7.3121%" hits in STATE.md (3), RUN_STATUS (1) and context-handoff (4)
  are all inside the new entries ("were …"), the superseded bullet, or prior-dated ledger text. There are no live
  "current" claims; skills have 0 hits.
- **Conflicts found:** none in values. One concurrency observation: the OPT-A covariance re-basing (condition (d)) had
  been completed earlier the same day by another process. v5 was built as an edition of that package instead of
  duplicating it.

## 8. Post-install critic pass (2026-10-21)

- **Implementation:** PASS.
  - All 100 files match the install record, which lists exactly the installed set.
  - 61 copies are byte-identical; the source edition is untouched.
  - Every installed number is reproduced independently to ≤ 1e-14, and the new covariance terms exactly (0.0).
  - Non-blocking notes:
    - N1: the `ratio_unc_new_*` / `asym_unc_new_*` columns are signed net changes despite the 'unc' name.
    - N2: the study-script docstring's 'EXACT' wording is superseded by its own mixed-definition note.
    - N3: the post-round-3 changes are covered numerically by this audit.
- **Canonical text:** PASS-WITH-CONDITIONS. Closed by `s12_fix_promotion_text_2026-10-21.py`:
  - F1: supersession markers on RUN_STATUS, context-handoff and current-release rows that still named an older budget as current.
  - F2: quoting-blocked status added inside the four v5 override sections.
  - F3: wording above.
  - F4: the two ratio figures defined (±0.58% = all background-related terms on h+/h−, ±0.51% = signed net change vs env100-only).
  - N1 and N2 are documented in the canonical text. The sha-pinned installed packages and registered scripts were not modified.

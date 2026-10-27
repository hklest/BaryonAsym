# Plan: finalize the high-Q² charged-particle SIDIS analysis (eΣ campaign) — 2026-10-27

Status: approved by the user 2026-10-27 ("Approve and start P1"); amended 2026-10-27 with the
answers to Open Questions 1-3 (Decisions 12-14). Planning only; nothing here was executed.
Path prefixes (all under `/data/dust/group/h1/klesthen/BaryonAsym/`):
- `SQ` = `sidis_highQ2`, `HQ` = `H1ChargedSidis_HighQ2`, `MEM` = `.supervisor/memory`
- `EPLAN` = `SQ/docs/plans/esigma_xf_bg_revision_2026-10-24.md`
- `CPLAN` = `SQ/docs/plans/closure_uncertainty_revision_2026-10-26.md`
- `REL` = `HQ/scripts/unfolding/esigma_final_2026-10-24/release` (s15 install, s16/s18 promotion)
- `DR` = `HQ/output_v6/central/data_release`
- `NOTE` = `SQ/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ` (git: `development`)
- `PAPER` = `SQ/analysis_note/H1-ChargedSIDIS-Paper-HighQ` (git: `main`)
- `LETTER` = `SQ/analysis_note/H1-HighQ2-SIDIS-Letter` (git: `esigma-port`, tracks `origin/main`)
- `NEWTAG` = `esigmaphotonxf_<date of the P1 s08 run>` (CPLAN D5)

## Goal

Close out the eΣ high-Q² SIDIS campaign: land the already-decided closure-uncertainty revision as
a new release tag, freeze it, re-point the three documents and the plot set, bring the status docs
and project memory to the frozen state, and commit. Non-goals: any new physics study, any change to
the central production or unfolding method, any lambda/V0 work. Expected change size: one new tag
under `DR` plus budget/covariance files; ~6 release scripts amended or tag-copied and one small
`freeze_verify`; one plot-set edition; prose edits in three LaTeX repos (one new note subsection,
one systematics row, refreshed tables); status-doc edits in 4 files; ~10 memory entries.

### What "finalized" means here

Derived from `SQ/docs/plans/plan-finalization-r1.md:1104-1125` (§7), updated to the eΣ campaign.
"Headline" = the content of `LETTER`, `PAPER` and `NOTE` at `NEWTAG`.

| # | r1 §7 criterion, eΣ reading | State | Evidence |
|---|---|---|---|
| 1 | Every populated bin has central, stat, total syst, per-source breakdown | met on `esigmaphotonxf_2026-10-24`; re-met at `NEWTAG` (P1) | `DR/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-24/`, `DR/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-24/esph_budget_v2.csv` |
| 2 | One headline edition everywhere (was HQ-v5f-r2/6-z grid) | met for 10-24; re-point at `NEWTAG` (P2, P3) | `EPLAN:478` (S8 done), `SQ/plots/releases/esigmaphotonxf_2026-10-24/`, `NOTE/tables/highQ2_z005_esigmaphotonxf_born_public_2026-10-24/` |
| 3 | All sources named, none placeholder | met; closure and response-MC-stat terms added in P1 | `DR/systematics_..._2026-10-24/esph_budget_v2_summary.json`; `MEM/decisions/2026-10-24-esigma-rebuild-everything-no-inherited-terms.md` |
| 4 | Real per-bin C_rad, Born level | met | `EPLAN:488` (S5), `MEM/decisions/2026-10-25-crad-same-mc-as-nominal.md` |
| 5, 6, 11, 16 | v5-era items (Pass-B iSys, Option H mask, φ_h headline) | superseded: the eΣ budget was rebuilt from eΣ inputs; φ_h is appendix-only | `MEM/decisions/2026-10-24-esigma-rebuild-everything-no-inherited-terms.md`, `MEM/decisions/2026-10-24-gen-all-kinematics-true-gamma-isr-only-incl-phih.md` |
| 7 | ep0304 recovered, four-period sum | met: eΣ data are the four periods | `SQ/context-handoff.md:182` (ep0304 1,146,532 entries of 658/658 files), `:253` (four-period luminosity) |
| 8 | A_BC blinding memo | not an eΣ headline item; no action | headline set = `LETTER`, `PAPER` |
| 9 | Assembly consistent with response products | met; re-gated in P1 | `HQ/output_v6/studies/esigma_final_2026-10-24/s7_post_install_audit.json`, `EPLAN:476` |
| 10 | Headline plots regenerated with stat+syst, no electron-photon plots | met for 10-24; redo at `NEWTAG` (P2) | `MEM/decisions/2026-10-24-no-electron-photon-plots-regenerate-all.md` |
| 12 | Absolute cross sections: no-Z Born nominal | met; the 9% no-Z/with-Z gap flag stands as recorded, no study | `MEM/decisions/2026-10-24-noz-born-nominal-published-xsec.md` ("Open:" paragraph), `MEM/decisions/2026-10-24-noz-born-factor-no-own-uncertainty.md` |
| 13 | Critic sign-offs documented | partial: `h1-analysis-critic` PASS logged only in `EPLAN:478`; record in handoff (P5) | `EPLAN:478` |
| 14 | Package memo (= note + letter + paper) built and reviewed | met for 10-24 (pushed: note, paper 1f94cb6, letter 75274d5); redo at `NEWTAG` (P3) | `EPLAN:478`; three repos 0 ahead/0 behind (checked 2026-10-27) |
| 15 | Frozen decisions cited consistently (= S9 freeze record + memory) | not met | no `DR/FREEZE_RECORD_*`, no read-only dirs, no `freeze-*` tags (checked 2026-10-27) |
| new | Status docs match the frozen state | not met | `SQ/context-handoff.md:621` and `SQ/docs/RUN_STATUS.md` still say quoting BLOCKED pending S8+S9 |
| new | Memory and workspace commit at the frozen state | not met | `.supervisor/history.md` ends 2026-10-24; two stale facts (Outline) |

## Approach

Six vertical packages in one dependency chain, each a lead dispatch with its H1 specialists.
P1 lands the decided closure revision as `NEWTAG` (the campaign's own s15/s16/s18 mechanics), P2
regenerates the plot set, P3 updates the documents, P4 freezes, P5 catches the status docs up, P6
closes memory and commits. P3 precedes P4, against the brief's order, because the freeze record
hashes the document-repo commits and tags them (`EPLAN:433-435`). Rejected: freezing `esigmaphotonxf_2026-10-24` now and revising later
(two freezes, documents re-pointed twice); a flat plan (each package needs its own lead and
specialists); folding P2 into P3 (plots are script runs, prose is writing, different leads).

## Decisions

Resolved from the decision store and the repo; `executor`/`writer` must not reopen them.

1. The closure revision is approved and proceeds. `CPLAN:3-4` still says "DRAFT rev 2, awaiting
   user approval", but the user approved treatment R and the new-tag install
   (`MEM/decisions/2026-10-26-closure-treatment-r-new-tag.md`, verbatim "3 yes.") and the per-bin
   closure term (`MEM/decisions/2026-10-26-compute-per-bin-closure-uncertainty.md`). The plan
   file's header is stale, not the approval. P1's plan stage rewrites `CPLAN` as rev 3 in place.
2. DIFFVM is not subtracted (`MEM/decisions/2026-10-26-diffvm-not-subtracted-negligible-rho0.md`,
   supersedes `.supervisor/archive/memory/decisions/2026-10-26-diffvm-subtract-publish-amount.md`).
   `CPLAN` items that build or publish a subtracted vector are void: D13, D15, D21 and the Todo
   rows at `CPLAN:408` (vector producer) and the sub-table/D21 parts of `:409`, `:411`, `:419`; the
   tests `only_diffvm_moves`, `bin30_shift`, `reproduces_study`, `sigma_s_term` are dropped. The
   title and Goal items (1)-(2) are rewritten. The note gets a background subsection of three
   figures and one to two paragraphs instead.
3. Slot (b) resolves to no change (`MEM/decisions/2026-10-26-no-diffractive-term-or-mixed-response.md`):
   no diffractive-admixture term, no mixed response; `CPLAN` D18 and OQ1 are closed. The bin-30
   two-track excess term D17 is dropped (`MEM/decisions/2026-10-26-diffvm-high-mass-excess-no-study.md`).
   The T4 VM-veto plot stays a cross-check only (`MEM/decisions/2026-10-26-observable-strict-no-vm-veto.md`).
4. Response-MC statistics enter the quoted totals
   (`MEM/decisions/2026-10-26-response-mc-stat-in-quoted-totals.md`); no low-N flag in any table
   (`MEM/decisions/2026-10-26-no-low-stat-flag-all-bins-presented.md`,
   `MEM/decisions/2026-10-26-low-stat-cut-proposal-dropped.md`).
5. Install order: coverage gate (`CPLAN` D6, >= 0.66 per N>=4 class) passes, hostile review,
   user sign-off, then `s15` on `NEWTAG`; `s16`/`s18` promotion follows. No install before the gate.
6. The freeze (S9) applies to `NEWTAG` only. `INSTALL_RECORD_esigmaphotonxf_2026-10-24.json` stays
   as provenance; freezing a tag that the closure revision supersedes would mean two freezes.
7. `EPLAN` stays the campaign record: its two open Todo rows (`EPLAN:492-493`) are executed by P4
   and P5; `CPLAN` rev 3 is executed by P1 and P3. P5 alone ticks both plans, from the P1-P4 reports.
8. `SQ/docs/plans/plan-v6-finish-line.md` is superseded: zero checklist items, Steps 0-11 are the
   v6 production that the eΣ campaign rebuilt on, and nothing in `RUN_STATUS.md`, the handoff or
   `EPLAN` cites it (only `SQ/docs/archive/` and `SQ/docs/skills_build/` manifests do).
   `SQ/docs/plans/plan-finalization-r1.md` is superseded except §7, carried in Goal above. P5 adds
   one "Superseded by ..." line to each header.
9. Documents already point to `esigmaphotonxf_2026-10-24` (S8, `EPLAN:478`). Two memory facts are
   stale and P6 supersedes them: `MEM/facts/2026-10-24-note-repo-unpushed-commits.md` (all three
   repos 0 ahead/0 behind on 2026-10-27) and `MEM/facts/2026-10-24-esigma-release-plot-set.md`
   (the xf edition exists). `MEM/facts/2026-10-24-esigma-release-quoting-unblocked.md` is partly
   stale ("Phase G in progress").
10. Document pushes follow `MEM/decisions/2026-10-24-esigma-papers-and-note-push-authorized.md`:
    push after `h1-analysis-critic` review, fast-forward only, by `git-ops`. Freeze tags ride the
    same pushes (`EPLAN:433-435`).
11. The three document repos are nested git repos whose files the workspace repo also tracks as
    plain files (265 paths under `SQ/analysis_note/`). Workspace commits never `git add -A` and
    never stage `SQ/analysis_note/`, `HQ/`, `SQ/data*`, `SQ/MC`, or tarballs.

12. DIFFVM uncertainty (closes Open Questions 1; user 2026-10-27, verbatim): "I think that we will
    just mention these numbers in a table. Some theory frameworks may include diffractive rho,
    meaning we shouldn't just subtract it or even put an uncertainty on it by default. We'll just add
    a table mentioning all the bins that have any contribution from diffractive rho, the fraction of
    our tracks from it, and our recommended uncertainty to use on the subtraction if the theorists
    want to subtract it." No DIFFVM row in the quoted budget; the ±100% row is removed, not
    replaced. `CPLAN` rev 3 D24 specifies the table (P1 product, P3 prints it).
13. One-track ΔE < 0 excess in the note (closes Open Questions 2; user 2026-10-27, verbatim):
    "Omit it".
14. P6 workspace commit scope (closes Open Questions 3; user 2026-10-27, verbatim): "Add everything
    under sidis_highQ2/docs/" (text only; data, plots, MC and `HQ/` stay untracked).

## Outline

- `CPLAN`: rev 3 written 2026-10-27 (P1 plan stage); see its "Struck in rev 3" list (D13, D15-D18,
  D21, the rev-2 S1/S2 slices) and Slices S0-S9. Header approved; Open Questions emptied; no DIFFVM term
  of any size (Decision 12): the ±100% row is removed, `diffvm_rho_rel` stays an INFO column, and a
  per-bin DIFFVM table (CSV + Markdown, `SQ/data/backgrounds/diffvm_perbin_<NEWTAG>/`,
  `SQ/docs/backgrounds/diffvm_perbin_table_<NEWTAG>.md`) is the P1 product that P3 prints.
- `REL/`: tag-copies `s15_install_<NEWTAG>.py`, `s16_promote_<NEWTAG>.py`,
  `s18_close_quoting_blockers_<NEWTAG>.py` after the 2026-10-24 ones (pattern `REL/s15_install_esigmaphotonxf_2026-10-24.py`);
  `eval_coverage_gate.py` new, from `HQ/scripts/studies/stat_coverage_2026-10-26/eval_treatment_v2.py`.
  Invariant: `s15` refuses an existing destination; backups go to `SQ/docs/promotions/<NEWTAG>_preedit/`.
- `DR/`: new release, systematics and `INSTALL_RECORD_<NEWTAG>.json`; `FREEZE_RECORD_<NEWTAG>.json`
  (P4, shape per `EPLAN:432-436`); `chmod -R a-w` on the `NEWTAG` release, budget, covariance dirs.
- `HQ/scripts/plots/esigmaphotonxf_2026-10-24/copy_producers_2026-10-24.py` and `producers/`:
  tag-copy to `<NEWTAG>`; output `SQ/plots/releases/<NEWTAG>/` + tarball, build dir
  `SQ/plots/releases/_build/<NEWTAG>/` (pattern: the 2026-10-24 siblings).
- `NOTE/sections/backgrounds.tex`: new DIFFVM subsection (3 figures, 1-2 paragraphs; figures from
  `HQ/output_v6/studies/diffvm_datatuned_2026-10-26/` chosen in P2).
  `NOTE/sections/systematics.tex:117` ±100% DIFFVM row removed, not replaced (Decision 12); `:55-56` medians
  refreshed; closure-term and response-MC-stat rows per `CPLAN` S9; `NOTE/sections/summary.tex:44-47`
  medians refreshed; tables `NOTE/tables/highQ2_z005_<NEWTAG>_born_public*/`.
- `PAPER/paper_full.tex` tables `PAPER/tables/..._<NEWTAG>_noZ/blocks/`; `LETTER/sections/observables_method.tex:32-35`
  uncertainty table; `LETTER/sections/sm_crad_bins.tex:2-4` provenance comments.
- Status docs (P5): `SQ/context-handoff.md` `:5` date, `:7-13` startup updates, `:576-582` resume
  bundle, `:621-623` quoting line, FROZEN section appended after `:634-641` (QUOTING BLOCKERS CLOSED); `SQ/docs/RUN_STATUS.md`
  `:5` date and quoting line, new `NEWTAG` + freeze section after `:491-523`; `CONTEXT.md`,
  `SQ/docs/INDEX.md` per `SAVE-CONTEXT.md`.
- Memory (P6): supersede the facts in Decisions 9; new decision entries for the freeze and for
  Decisions 12-14; `history.md` lines for 2026-10-25..27 milestones.

## Slices

Slices are the packages in order; see `## Work Packages` for the blocks.

1. P1 closure revision -> `NEWTAG` installed and promoted. Tests: `coverage_gate: budget signed
   columns -> >= 0.66 in each N>=4 class`; `t4_counts`, `v2_untouched` (from `CPLAN`); `install:
   s15 --dry then real -> INSTALL_RECORD_<NEWTAG>.json, 0 hard-gate failures`; `promotion: s16, s18
   -> all targets EDITED, backups present`. Checkpoint: `s7`-style post-install audit PASS.
2. P2 plot set -> `SQ/plots/releases/<NEWTAG>/`. Tests: `no_old_tag: no 2026-10-24 or earlier
   release path in any producer`; `inventory: inventory.json lists every figure the three documents
   include`; `h1-plot-reviewer: no blocking anomaly`. Checkpoint: tarball built, `shamatch.json` consistent.
3. P3 documents. Tests: `numbers_match: every quoted median/total equals NEWTAG outputs`;
   `no_low_n_flag`; `diffvm_subsection: 3 figures, <= 2 paragraphs, states not subtracted, carries
   the per-bin DIFFVM table`; `no_diffvm_row: no DIFFVM line in any budget table`;
   `no_unsubtracted_claim inverted: no text claims a subtraction`. Checkpoint: `latexmk` builds all
   three; `h1-analysis-critic` PASS; commits (and pushes per Decision 10).
4. P4 freeze. Tests: `freeze_verify: recomputed sha256 == record`; `tags_exist: freeze-<NEWTAG> in
   the workspace repo and the three document repos at the recorded commits`; `readonly: no writable
   file under the frozen dirs`. Checkpoint: `freeze_verify` passes from a fresh shell.
5. P5 status docs. Tests: `no_blocked: grep -n 'QUOTING REMAINS BLOCKED' returns nothing in
   handoff and RUN_STATUS`; `dates: header dates = edit date`; `plans_ticked: EPLAN:492-493 and
   CPLAN Todo all ticked, superseded headers on the two old plans`. Checkpoint: SAVE-CONTEXT final
   check line listing the files updated.
6. P6 memory and commits. Tests: `store_clean: git status .supervisor empty after commit`;
   `facts_superseded: the two stale facts moved to archive with supersedes links`. Checkpoint:
   `git log -1` on `development` shows the close-out commit with the `git-safety` trailer.

## Verification

`freeze_verify` passes; `grep -rn 'esigmaphotonxf_2026-10-24' NOTE PAPER LETTER --include=*.tex`
returns only provenance comments or none; handoff, RUN_STATUS and `.supervisor/history.md` name
`NEWTAG` and the freeze; both legacy plans fully ticked.

## Risks

- The coverage gate may fail at `NEWTAG` staging; then P1 stops before install and reports (no
  install of an under-covering budget). Not covered by tests until the budget exists.
- Toys rerun (`CPLAN` S3, 120 shards) is Condor work; stored toys may be reusable since the data
  vector no longer changes (Decision 2), but reuse needs a sha check first.
- Nested repos tracked twice (Decision 11): P6 stages by explicit path.
- `chmod -R a-w` on DUST dirs is irreversible for other agents without a documented `chmod` undo;
  P4 records the exact dirs in the freeze record.

## Scope Notes

In: P1-P6 as blocked below. Out: the Goal non-goals and unfolding reruns beyond `CPLAN` S3 toys.
Each package's lead briefs the named H1 specialists; they are the authority for physics, H1OO and Condor. Login node: one file,
<= 5 min; everything else Condor via `h1-naf-operator`. Decisions above cite their memory entries
and are closed.

## Assumptions

- `REL` scripts accept a new tag by copy-and-edit, as they did for 2026-10-24 (check: `s15 --dry`).
- The DIFFVM study figures exist in `HQ/output_v6/studies/diffvm_datatuned_2026-10-26/` (check: `ls`).
- The three documents build with the tool S8 used (`latexmk` is not on PATH in the login shell on
  2026-10-27; check: the build command in each repo's README or `EPLAN:429`, else `pdflatex` x3 + `bibtex`).

## Work Packages

### P1: Closure revision to NEWTAG
- Goal: execute `CPLAN` as amended by Decisions 1-5; done when `INSTALL_RECORD_<NEWTAG>.json`
  exists, `s16`/`s18` report all targets EDITED and the post-install audit passes.
- Owns: `CPLAN`, `REL/`, `HQ/scripts/unfolding/esigma_final_2026-10-24/` closure and term scripts,
  `HQ/output_v6/studies/esigma_final_2026-10-24/`, `DR/` new tag, `SQ/docs/promotions/<NEWTAG>_preedit/`.
- Deps: none
- Stages: plan, execute
- Status: done
- Artifacts: `CPLAN` (rev 3 in place); `->`
- Scope Notes: `planner` writes rev 3 (annotation cycle, Decisions 2-4). `executor` briefs
  `h1-unfolder` (closure toys, treatment R), `h1-systematics` (budget assembly, coverage gate,
  signed columns), `h1-naf-operator` (Condor toys, s15 install), `h1-data-validator` (release
  files), `h1-analysis-critic` (hostile review before sign-off). User sign-off before `s15`. The per-bin
  DIFFVM table (Decision 12, `CPLAN` D24) is a P1 product at `SQ/data/backgrounds/diffvm_perbin_<NEWTAG>/`
  plus `SQ/docs/backgrounds/diffvm_perbin_table_<NEWTAG>.md`, additive only: no central value or
  budget number changes.

### P2: Plot set and document figures at NEWTAG
- Goal: regenerate the release plot set and the figures the three documents include; done when
  `SQ/plots/releases/<NEWTAG>/` and its tarball exist and `h1-plot-reviewer` reports no blocker.
- Owns: `HQ/scripts/plots/<NEWTAG>/`, `SQ/plots/releases/<NEWTAG>*`, `SQ/plots/releases/_build/<NEWTAG>/`,
  `NOTE/figures/`, `PAPER/figures/`, `LETTER/figures/` (files only; no `.tex`).
- Deps: P1 (the three DIFFVM subsection figures may start before P1 ends)
- Stages: execute
- Status: done
- Artifacts: `->`
- Scope Notes: `executor` briefs `h1-plot-reviewer` (every regenerated figure) and `h1-analysis-critic`
  (figure choice for the DIFFVM subsection). Producers run on release files, one process per
  figure set; anything over the login-node cap goes to Condor via `h1-naf-operator`.

### P3: Documents at NEWTAG
- Goal: note, paper, letter and supplemental carry the `NEWTAG` numbers, the closure and
  response-MC-stat terms, no DIFFVM term, and the DIFFVM background subsection with the per-bin
  table; done when
  all three build, `h1-analysis-critic` passes and the commits are in each repo.
- Owns: `NOTE/**/*.tex`, `NOTE/tables/`, `PAPER/**/*.tex`, `PAPER/tables/`, `LETTER/**/*.tex`.
- Deps: P1, P2
- Stages: write
- Status: write
- Artifacts: `->`
- Scope Notes: `writer` briefs `h1-analysis-note-writer` for all LaTeX edits, `h1-systematics` for
  the uncertainty rows and medians, `h1-analysis-critic` for the review. No low-N flag (Decision 4).
  Commits per repo by `git-ops`; pushes per Decision 10. Decision 12: no DIFFVM uncertainty term in
  the note's budget text (`systematics.tex:117` row removed, `paper_full.tex:264` row removed); the
  DIFFVM table (`CPLAN` D24 `any_contribution` rows) goes in the background subsection, which keeps
  its 3 figures (none shows per-bin fractions, so the table makes no figure redundant). Decision 13:
  omit the one-track ΔE < 0 excess.

### P4: S9 freeze
- Goal: freeze `NEWTAG` per `EPLAN:432-439`; done when `freeze_verify` passes from a fresh shell.
- Owns: `DR/FREEZE_RECORD_<NEWTAG>.json`, a `freeze_verify` script under `REL/`, permissions of
  the `NEWTAG` release/budget/covariance dirs, `freeze-<NEWTAG>` tags in four repos.
- Deps: P1, P2, P3
- Stages: execute
- Status: execute
- Artifacts: `->`
- Scope Notes: `executor` briefs `h1-naf-operator` (sha256 over DUST, `chmod -R a-w`) and `git-ops`
  (annotated tags; push per Decision 10). Writes no handoff text (P5 owns it); emits the memory
  decision entry's content for P6.

### P5: Status docs catch-up
- Goal: handoff, `RUN_STATUS.md`, `CONTEXT.md`, `SQ/docs/INDEX.md` describe the frozen `NEWTAG`
  state per `SAVE-CONTEXT.md`; done when the Slice 5 tests pass.
- Owns: `SQ/context-handoff.md`, `SQ/docs/RUN_STATUS.md`, `CONTEXT.md`, `SQ/docs/INDEX.md`,
  `SQ/STATE.md`, checklist state of `EPLAN` and `CPLAN`, header lines of the two superseded plans.
- Deps: P4
- Stages: write
- Status: write
- Artifacts: `->`
- Scope Notes: `writer` follows `SAVE-CONTEXT.md` steps 1-11; facts only from P1-P4 reports and
  records; briefs `h1-analysis-critic` for the consistency read (criterion 13 sign-off recorded).

### P6: Memory close-out and commits
- Goal: project memory and the workspace repo hold the frozen state; done when the Slice 6 tests pass.
- Owns: `.supervisor/`, workspace-repo commits of the files P5 owns and `SQ/docs/plans/`.
- Deps: P5
- Stages: execute
- Status: execute
- Artifacts: `->`
- Scope Notes: `supervisor` dispatches `curator` for the close-out (Decisions 9, new freeze and
  Decision 12-14 entries, history lines), then `git-ops` commits by explicit path (Decision 11)
  with the `git-safety` trailer. Commit scope (Decision 14): `.supervisor/`, the files P5 owns, and
  everything text under `sidis_highQ2/docs/`; never `SQ/analysis_note/`, `SQ/data*`, `SQ/plots`,
  `SQ/MC`, `HQ/` or tarballs. Nothing pushed from the workspace repo.

## Open Questions

None open. Open Questions 1-3 were answered by the user on 2026-10-27 and moved to Decisions 12-14.

## Log

- 2026-10-27 decision: plan approved by user ("Approve and start P1"); OQ1: DIFFVM — no default subtraction or uncertainty term; per-bin table of bins with any diffractive-rho contribution, fraction of tracks from it, and recommended uncertainty on a subtraction if theorists subtract; OQ2: omit the one-track ΔE<0 excess from the note; OQ3: P6 commit scope = everything (text) under sidis_highQ2/docs/
- 2026-10-27 P1 (plan): done; CPLAN rev 3 written in place (DIFFVM term removed not replaced, per-bin DIFFVM table D24, stored toys/s03 reused, 26/28 terms, ~9 Condor jobs); program plan Decisions 12-14 + P1/P3/P6 Scope Notes; critic PASS round 2; deferred: none
- 2026-10-27 decision: user sign-off for s15 install of esigmaphotonxf_2026-10-27 after fixing hostile-review NITs F7–F9 ("Install, after fixing F7–F9"); DIFFVM per-bin CSVs gain an explicit `sign` column ("Yes, add it"; D24 amended)
- 2026-10-27 P1 (execute): done; esigmaphotonxf_2026-10-27 installed (s15 Condor 5911769, 54/54 gates, DR/INSTALL_RECORD_esigmaphotonxf_2026-10-27.json), s16 21/21 and s18 6/6 EDITED, post-install audit PASS, old_untouched 266/266, DIFFVM table with sign column; deferred: figures (P2), note text F3–F5 and table printing (P3), freeze (P4), plan ticks (P5)
- 2026-10-27 P2 (execute): done; esigmaphotonxf_2026-10-27 plot set + tarball (82 PDFs, 107 includes of note/paper/letter, delta documented), docfigs installed (43 new 20261027 files, 9 untagged overwritten), Condor 5912799/5912849/5912800/5912801/5912802, h1-plot-reviewer PASS, h1-analysis-critic PASS after regroup fix; deferred: include re-pointing and backgrounds.tex DIFFVM text (P3), ratio_asym closure check (P3/h1-systematics)
- 2026-10-27 decision: note restructure per user feedback (no repetition, no superseded references, MC comparison after Results, apples-to-apples only: with-Z data vs EW MC and photon-only data vs MAPTMD24; event-level vs track-level background sections; paragraph breaks, fewer colons); conventions be7893b
- 2026-10-27 P3 (write): partial; paper + letter + SM edited and built; note restructured, build and round-2 review pending; cross-level fig:datamc replaced by placeholders; deferred: P2b with-Z data-vs-MC figures, note build/round 2/commits
- 2026-10-27 decision: letter carries a photon-only MAPTMD24 comparison figure ("Yes, add it"); per-bin DIFFVM table published as supplementary material cited by the paper ("Supplementary material"); P4 packages the CSVs with the freeze

## Todo

- [x] P1 plan: `CPLAN` rev 3 written (Decisions 2-4 folded in, OQ1 answer recorded)
- [x] P1 execute: closure toys, budget, coverage gate PASS, hostile review, user sign-off, `s15`, `s16`, `s18`, audit
- [x] P2: plot set and docfigs at `NEWTAG`, plot review PASS
- [ ] P3: three documents updated, built, reviewed, committed (pushed per Decision 10)
- [ ] P4: freeze record, tags, read-only dirs, `freeze_verify` PASS
- [ ] P5: handoff, RUN_STATUS, CONTEXT.md, INDEX.md; `EPLAN`/`CPLAN` ticked; two old plans marked superseded
- [ ] P6: curator close-out, facts superseded, history lines, workspace commit

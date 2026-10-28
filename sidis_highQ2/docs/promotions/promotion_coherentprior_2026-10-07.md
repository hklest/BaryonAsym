# Coherent-prior baseline promotion — 2026-10-07

**Declared by user; documentation/registry promotion only.** The candidate directory is promoted **in place**: no release directory was moved, copied, renamed, or modified. Historical artifacts remain provenance.

## Progress ledger

1. **PASS — VERIFY.** Verified 21 claimed paths: release directory, `README.md`, `validation/validation.json`, budget, three plot packages (17 PDFs total), covariance supplement, stat-toys, B1 study, phi_h, three regenerated systematics directories, four critic reports, plan, MC memo, and alpha audit.
2. **PASS — CATEGORIZE.** Release + unfolding/prior-basis + systematics + plotting/covariance. Updated skills: state, unfolding, systematics, results, plotting, lessons, baseline-update; additionally docmap references the old release and is updated.
3. **PASS — STATE.** Added a dated PROMOTED/CURRENT override and explicit WRONG→RIGHT release, method, budget, plot, covariance, stat-toy, and median-value negations to `STATE.md`.
4. **PASS — STATUS/HANDOFF.** Updated `RUN_STATUS.md`, `status/current-release.md`, `status/unfolding-tunfold.md`, and `context-handoff.md`; no other status file referenced the promoted release/budget.
5. **PASS — HANDOFF.** Startup/current-release, unfolding, and systematics facts now carry the promotion override.
6. **PASS — SCRIPT REGISTRY.** Updated both RUNLIST copies identically with the coherent release driver and superseded plain release driver; updated the seven-category map with the same release-category override.
7. **PASS — SKILLS.** Updated eight skills (the required seven plus docmap, discovered as an old-release reference; the MC memo retains its existing sidis-hq2-mc owner) with facts, procedures/traps, and canonical-reference addenda.
8. **PASS — OWNERSHIP REGISTRY.** Registered all requested new release/systematics/plot/covariance/study/document artifacts plus this promotion summary in `ownership.tsv` and `COVERAGE_MANIFEST.md`; owning skills contain the canonical-reference addendum.
9. **PASS — VERIFICATION.** Targeted supersession grep completed after final cleanup. Replaced mutable-current “candidate/current lineage/not yet swapped/PROVISIONAL” wording with PROMOTED CANONICAL IN PLACE wording in state, handoff, aggregate/per-topic status, skills, and both RUNLIST copies. Remaining hits are required explicit supersession/provenance markers, plus immutable historical plans/reports. The summary's own verification wording is the only mutable-current hit; no live current claim was retained. See “Verification leftovers” below.
10. **PASS — PROMOTION SUMMARY.** This durable ledger is the registered promotion summary.

## Files touched

- `STATE.md`; `context-handoff.md`; `docs/RUN_STATUS.md`; `docs/status/current-release.md`; `docs/status/unfolding-tunfold.md`.
- Both canonical `RUNLIST.md` copies and remote `SEVEN_CATEGORY_SCRIPT_INDEX.md`.
- Eight `SKILL.md` files: state, unfolding, systematics, results, plotting, lessons, baseline-update, docmap.
- `docs/skills_build/ownership.tsv`; `docs/skills_build/COVERAGE_MANIFEST.md`; this file.

## Verification leftovers

The old release, old budget, old plot names, old supplement, old stat covariance, plain-prior method, and `1.485/1.415` occur only as explicit `WRONG`, `SUPERSEDED`, historical, audit, provenance, or immutable-source-plan/report references. Mutable-current files were also checked for stale “candidate/current lineage/not yet swapped/PROVISIONAL” promotion wording and corrected to canonical-in-place language. Historical documents were not edited, moved, or deleted.

## Open blockers not cured

Canonical 7-column publication plotter; `C_rad` convention/NONRAD provenance; P-RAPGAP; P-METHOD; P-ALPHA; track-level background missing `overlap_980`/`n_raw_980`; trigger placeholder; charge-dependent tracking convention; covariance gaps; B1 follow-up attribution items.

## Addendum 2026-10-07: filesystem supersession markers

`_SUPERSEDED_DO_NOT_USE.md` markers were added (additive; no original file modified,
moved, or deleted) inside: the old central release dir `bbb_ibu_finalcandidate_..._2026-06-10/`,
its sibling tarball (`.SUPERSEDED.txt` note), the old budget `total_uncertainty_z005_with_phih_floor_2026-10-03/`,
the intermediate un-floored budget `total_uncertainty_z005_with_phih_coherentprior_2026-10-07/`,
the old covariance supplement `sidis_highQ2/data/covariance/z005_supplement_modelreg_2026-09-23/`,
and the three old plot packages (`canonical_*_2026-10-03/`). Purpose: block accidental
grabs at the filesystem level; the old release directory otherwise remains the newest
resident of `data_release/`. Note: the coherent supplement lives under
`H1ChargedSidis_HighQ2/data/covariance/`, the legacy one under `sidis_highQ2/data/covariance/`.

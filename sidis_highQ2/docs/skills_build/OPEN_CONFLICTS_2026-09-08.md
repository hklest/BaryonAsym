# Source conflicts surfaced during the skills port

Date: 2026-09-08. Found while digesting 421 documents into 14 skills.
These are NOT introduced by the port — they already existed in the source documents.
Each skill resolved its own case in favor of `STATE.md`, but the underlying
documents still disagree and should be reconciled.

Root: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/`

## Blocking — physics-facing, needs a decision

| # | Conflict | Source A | Source B | Skill resolution |
|---|----------|----------|----------|------------------|
| 1 | Paper draft asserts completed multiplicities, background subtraction, and radiative correction; release notes state no bgsub, no `C_rad`, no DIS denominator, no approval | `sidis_highQ2_paper/repo/paper_full.tex` | `data/release/bbb_ibu_finalcandidate_.../RELEASE_NOTES.md` + `STATE.md` | Followed release notes; paper draft is ahead of the actual release |
| 2 | Charge-dependent tracking systematic `0.3%/track` vs `2.5% on A_h`; sanity check reported FAILED | `docs/methodology/v6sys_h1_precedent_systematics_manifest_2026-06-03.md` | `docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` | Recorded both; flagged as requiring review |
| 3 | `phi_h` lab-frame vs CM-frame: lab-frame results invalidated by a frame bug | `docs/reviews/critique-phih-convention-freeze-highQ2-sidis.md` | `docs/methodology/pTh_phih_frame_bug_2026-05-24.md` | Required CM-frame implementation |

## Non-blocking — bookkeeping, resolve when convenient

| # | Conflict | Source A | Source B | Skill resolution |
|---|----------|----------|----------|------------------|
| 4 | Statistical replicas: 100 toy throws vs validated 1000-replica replacement | `data/release/.../SCHEMA.md` | `docs/provenance/ibu_1000rep_stat_update_2026-06-14_validation.md` | Recorded both; 1000-rep is later |
| 5 | Release manifest hashes stated stale, while the same file reports a 34-file manifest regeneration | `data/release/.../CHANGELOG.md` | `data/release/.../VALIDATION.md` | Recorded both |
| 6 | Direct-monitor lumi quoted as `356613.59` vs `356613.0 nb^-1` | `docs/references/reference-highQ2-mc-luminosity.md` | `docs/status/normalization.md` | Both superseded by `351000.0 nb^-1`; moot |
| 7 | Tracking common source `0.5%/track` vs frozen `1.0198039027%` | `docs/research/research-tracking-systematics-deep.md` | `docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` | Current z005 uses completed random-drop `-0.5%` per `context-handoff.md` |
| 8 | Trigger systematic `<=0.5%` placeholder vs `1.2%` S67 study value | `docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` | `docs/research/research-h1-trigger-system.md` | `<=0.5%` identified as current placeholder |
| 9 | Electron theta `±2 mrad` vs frozen `±1 mrad` | `docs/plans/plan-systematics-v5g-v3-2026-05-24.md` | frozen v6sys menu | Frozen value used |
| 10 | HFS `±2%` vs frozen JES `±1%` + RCES `±1%` | `docs/plans/plan-systematics-v5g-v3-2026-05-24.md` | frozen v6sys menu | Frozen split used |
| 11 | Herwig `MinQ2` interpretation differs between survey and later audit | `MC/common/survey-herwig-runcards-2026-05-27.md` | `MC/common/audit-herwig-2026-05-27.md` | Later audit wins (`MaxQ2` before `MinQ2`) |
| 12 | Canonical 7-column systematic plotter: snapshot lists `run_electron_compare_validate_total_lumi.py` as central; state docs say it is not | `scripts/canonical_highQ2_sidis_2026-06-30/docs/RUN_STATUS.md` | `STATE.md`, `context-handoff.md` | Treated as unresolved (matches the STATE.md blocker) |
| 13 | Historical guidance to impose gen detector cuts vs v6 policy of no gen `Pt`/`eta` cuts | various historical research docs | `docs/methodology/v6_production_changes_2026-06-04.md` | v6 methodology wins |

## Recurrent, already handled

Historical `the superseded pre-rescale luminosity` / `356613.0` / `356000 nb^-1` luminosities and historical
central-method or binning claims appear throughout `docs/research/`,
`docs/methodology/`, and older release directories. Every skill negates these
explicitly against the current `351000.0 nb^-1` and the v6/z005 IBU/BBB central
contract. No further action needed unless a historical document is promoted.

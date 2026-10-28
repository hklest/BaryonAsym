# Migration Map — High-Q² SIDIS Startup Documentation Compaction

**Date:** 2026-07-03  
**Purpose:** make every removed/compacted startup section auditable and map it to exactly one destination.

## Source-to-Destination Map

| Removed/compacted source section | Destination | Notes |
|---|---|---|
| `context-handoff.md` top current-state banner | `context-handoff.md` startup card and `docs/status/current-release.md` | Startup facts kept in handoff; detailed release/status moved to status file. |
| `context-handoff.md` C_rad reproducibility snapshot and RAPGAP fLumi diagnostic | `docs/status/radiative-corrections.md` | Current C_rad numbers, paths, RAPGAP provisional status, and alpha0p2 gen-only guard preserved. |
| `context-handoff.md` C_rad 7-column regeneration pointer | `docs/status/radiative-corrections.md` | Plot/regeneration scripts and style freeze preserved concisely. |
| `context-handoff.md` near-electron/alpha-veto study save point | `docs/status/alpha0p2-background.md` | Key conclusions and status preserved. |
| `context-handoff.md` alpha-veto plot/script runbook | `docs/status/alpha0p2-background.md` and canonical script snapshot `RUNLIST.md` | Detailed local/remote plot roots preserved where current; startup points to canonical scripts first. |
| `context-handoff.md` alpha0p2 reco-background/all-fake response handoff section | `docs/status/alpha0p2-background.md` plus `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md` | Compact numbers and response-consistency rules preserved; full handoff remains intact. |
| `context-handoff.md` plot style/script traps | `docs/status/alpha0p2-background.md` | Current alpha0p2 7-column and pandas traps preserved. |
| `context-handoff.md` operational guardrails | `context-handoff.md` startup guardrails | Kept compact in startup path. |
| `RUN_STATUS.md` allow/deny box | `RUN_STATUS.md` and `docs/provenance/historical-paths.md` | Startup allow/deny preserved; expanded deprecated list moved to provenance. |
| `RUN_STATUS.md` near-electron/alpha-veto diagnostic suites | `docs/status/alpha0p2-background.md` | Current conclusions, paths, and provenance preserved. |
| `RUN_STATUS.md` alpha0p2 reco-space background vectors and all-fake handoff | `docs/status/alpha0p2-background.md` and `docs/status/normalization.md` | Response rules, current roots, combined-L normalization, and validated totals preserved. |
| `RUN_STATUS.md` z005 release/systematics section | `docs/status/current-release.md` | Release contract, components, and blockers preserved. |
| `RUN_STATUS.md` operational normalization notes, low-Q² DJANGOH alpha<0.1 vector, and normalization audit note | `docs/status/normalization.md` | L_data, 7091 vs 7110, stale lumi values, and current scale preserved. |
| `RUN_STATUS.md` SVD quarantine and TUnfold entry point | `docs/status/unfolding-tunfold.md` | Native v6 TUnfold entry point, conventions, save points, blockers, and SVD quarantine preserved. |
| `RUN_STATUS.md` study-local detector/track-cut variations | Archived original `RUN_STATUS_20260703T000000.md` | Non-startup side-study history preserved in archive; not copied to compact current status because not current authority. |
| `RUN_STATUS.md` reco-method side-study runbook/status | Existing `docs/systematics/reco_method_side_study_runbook_2026-06-20.md` and archived original `RUN_STATUS_20260703T000000.md` | Current startup points to LOOKUP; detailed runbook already exists and original is archived. |
| `RUN_STATUS.md` production completeness/pre-z005 response-build snapshot/migration QA smoke | `docs/provenance/historical-paths.md` and archived original `RUN_STATUS_20260703T000000.md` | Historical/pre-z005 detail preserved as audit, not current startup authority. |
| `RUN_STATUS.md` C_rad status | `docs/status/radiative-corrections.md` | Current baseline/provisional/quarantine facts preserved. |
| `RUN_STATUS.md` blocked/open items | `RUN_STATUS.md` and `docs/status/current-release.md` | Current blockers kept compact; historical pre-z005 rows archived. |
| `RUN_STATUS.md` deprecated paths | `docs/provenance/historical-paths.md` | Expanded deprecated/deny map preserved. |
| `RUN_STATUS.md` active Condor jobs | `RUN_STATUS.md` and `docs/status/current-release.md` | Current no-active-jobs entry preserved as ephemeral/current. |
| `RUN_STATUS.md` quick-start and doc map | `docs/LOOKUP.md` | Flowchart-style map replaces duplicated doc map. |
| `RUN_STATUS.md` historical run status | `docs/provenance/historical-paths.md` and archived original `RUN_STATUS_20260703T000000.md` | Milestones preserved without startup bulk. |
| Long high-Q² SIDIS narrative in `CONTEXT.md` analysis table and shared traps | `sidis_highQ2/context-handoff.md`, `docs/RUN_STATUS.md`, and `docs/status/*.md` | Global file now keeps only boundary, active workspace, v6 roots, lumi trap, and pointers. |
| Current startup roadmap docs `ANALYSIS_WORKFLOW_ROADMAP.md`, `SCRIPT_ROADMAP.md`, `REPRODUCING_THE_ANALYSIS.md`, `plan-v6-finish-line.md`, `LESSONS_LEARNED.md`, `INDEX.md` | Archived timestamped copies in this directory; current startup uses `docs/LOOKUP.md` and canonical script `RUNLIST.md` first | Existing files remain on disk; archived copies preserve pre-compaction startup-facing authority. |

## Archive Copies

See `MANIFEST.md` in this directory for exact original and archived paths.

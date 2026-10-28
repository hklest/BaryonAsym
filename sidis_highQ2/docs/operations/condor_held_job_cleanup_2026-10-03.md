# Condor held-job cleanup audit — 2026-10-03

Operator: `h1-naf-operator` (subagent), authorized by user to remove stale HELD jobs on
`naf-hone.desy.de` (schedd `bird-htc-sched22.desy.de`). Two gated phases executed:
Phase 1 verification (no removals), Phase 2 removal by explicit cluster ID only.

## Pre-state (Phase 1, enumerated 2026-10-03 ~20:40–21:23 CEST)

- Total jobs in queue for `klesthen`: **884** — all HELD (`JobStatus=5`).
- Running: **0**; Idle: **0**; Completed/Removed/Suspended in queue: 0/0/0.
- 16 clusters, newest `QDate` 2026-09-27 08:08 — nothing submitted within 72 h of
  2026-10-03 ~20:45 CEST (cutoff 2026-09-30 ~20:45).
- Full per-job enumeration (cluster, proc, Cmd, Iwd, UserLog, QDate, hold code/subcode,
  reason) saved at `/tmp/opencode/held_cleanup_20261003/held_full.txt` (884 rows).

## Removed clusters (Phase 2, condor_rm executed 2026-10-03T21:25:12–13 CEST, all rc=0)

| Cluster | Jobs removed | Campaign | Hold reason (code_sub) | Output / need verification |
|---|---:|---|---|---|
| 4563688 | 1 | `data_mc_reco_audit_20260729` recovery_v4_full | Memory > 1.2x requested (26_0) | Output dir `output_v6/studies/data_mc_reco_audit_20260729/full_absolute_v6_4563688_0_recovery_v4/` empty; stage not required by STATE.md (superseded by completed 2026-09-25 bgsub audit and 4-generator data-vs-MC comparison); held since 2026-07-30 |
| 4568681 | 1 | `data_mc_reco_audit_20260729` recovery_v4_full | Runtime > reserved (26_0) | Same campaign; output dir empty; same superseded-stage justification; held since 2026-07-30 |
| 5035111 | 1 | `pncw_audit` (early attempt) | stdout `/tmp/opencode/pncw_audit/audit.out` missing (7_2) | Audit completed via later attempt: `H1BaryonAsym/output/xqz3d/pncw_audit_tmp/result.json` exists (2026-08-27 21:21); /tmp inputs gone, job could never run |
| 5035113 | 1 | `pncw_audit` (early attempt) | input transfer `/tmp/opencode/pncw_audit/audit_pncw.py` missing (13_2) | Same completed-audit output on DUST; durable copies `audit_pncw.py/.sub` saved in `pncw_audit_tmp/` |
| 5164803 | 1 (proc 1159) | `unbinned_v2` extraction remnant | Diskusage > requested (26_0) | 1523/1524 chunks done; `validation_unbinned_v2.json` status=PASS; all 1523 contract shards present (658/501/364); handoff `OMNIFOLD_UNBINNED_V2_HANDOFF_2026-09-21.md`: "stale hold ... not a current production blocker" |
| 5164807 | 2 | `unbinned_v2` merge | Memory > 1.2x (26_0) | Handoff: "historical merge attempt"; merged/monolithic routes "superseded and forbidden" — output explicitly not needed |
| 5164809 | 2 | `unbinned_v2` validate_merged_arrays | Diskusage > requested (26_0) | Validates the forbidden/superseded merge route — output explicitly not needed |
| 5167019 | 1 | `unbinned_v2` streaming production (historical) | Runtime > reserved (26_0) | Handoff: "historical full-production attempts ... do not recover state"; implementation deliberately non-resumable |
| 5167020 | 2 | `unbinned_v2` streaming production (historical) | Runtime > reserved (26_0) | Same as 5167019 |
| 5252321 | 501 | `flatphi_link_fixed_20260927` attempt_1 (djangoh14) | stdout open failure: `linked_trees/djangoh14/...` parent dirs absent (7_2) | `status/launch_status.json`: attempt_1 state `HELD_STOP`, "No payload was launched"; superseded by clusters 5252324 + 5252328 (both QUEUE_EXIT); outputs on DUST: `output_v6/studies/flatphi_genreweight_z005_fixed_20260927/linked_trees/djangoh14/` (497 root files) |
| 5252322 | 364 | `flatphi_link_fixed_20260927` attempt_1 (rapgap31) | stdout open failure: `linked_trees/rapgap31/...` parent dirs absent (7_2) | Same attempt_1 per `launch_status.json`; superseded by 5252325 + 5252329 (QUEUE_EXIT); outputs: `linked_trees/rapgap31/` (352 root files). Note: 5252322 was not on the prior stale list but is the rapgap31 counterpart of the same dead attempt_1 submission |

**Total removed: 877 jobs across 11 clusters.**

Removal method: `condor_rm <cluster-id>` per cluster, explicit IDs only. No
`condor_rm -all`, no constraint expressions. All 877 jobs re-verified `JobStatus=5`
(HELD) immediately before removal (pre-removal check 2026-10-03T21:23:38 CEST,
`/tmp/opencode/held_cleanup_20261003/pre_removal_verify.txt`: 877/877 held).
No log file, submit file, output file, or working directory was deleted, moved, or
truncated — queue removal only.

## Protected list (NOT removed — kept in queue)

| Cluster | Jobs | Campaign | Protection reason |
|---|---:|---|---|
| 5167027 | 1 | `rad_crad_vs_detector_mc_20260921` | Log path under `sidis_highQ2/data/radiative/rad_crad_vs_detector_mc_20260921/` — path-matches "crad" and "radiative". **Was mis-grouped as unbinned_v2 in the prior audit's "51670xx" list — it is not unbinned_v2.** |
| 5167030 | 1 | `rad_crad_vs_detector_mc_20260921` | Same as 5167027 |
| 5167033 | 1 | `rad_crad_vs_detector_mc_20260921` | Same as 5167027 |
| 5252190 | 2 | Britzger crad smoke (`britzger_eminus_crad_xf_y_eta_q2_2026-09-26`) | Iwd under `sidis_highQ2/scripts/radiative/...crad.../` — path-matches "crad" and "radiative". Listed stale by the prior audit, but the explicit path-protection rule overrides; C_rad/radiative work is active per STATE.md |
| 5252191 | 2 | Britzger crad smoke | Same as 5252190 |

Protected total: 7 jobs across 5 clusters. No held job had Cmd/Iwd/Log under
`condor_v6/central/` or `condor_v6bsa/central/` (the flatphi campaign lives under
`condor_v6/studies/`). Nothing was within the 72-hour window.

## Keep list (step-4 rule: no output AND stage required by STATE.md)

**Empty.** Every removed cluster either (a) already has its expected output on DUST
(pncw `result.json`; unbinned 1523 shards + PASS validation; flatphi linked trees from
replacement clusters) or (b) has an explicitly superseded/forbidden/not-needed stage
(data_mc_reco_audit recovery, unbinned merge/validate-merge routes, unbinned historical
streaming attempts). No unfulfilled STATE.md-required stage depended on a removed job.

## Post-state (verified 2026-10-03T21:26:51 CEST)

- Total for `klesthen`: **7 jobs; 0 completed, 0 removed, 0 idle, 0 running, 7 held, 0 suspended**.
- Remaining clusters: 5167027 (1), 5167030 (1), 5167033 (1), 5252190 (2), 5252191 (2) — exactly the protected list.
- Held count 884 → 7 (877 removed, matches plan exactly). Running 0 → 0 and idle 0 → 0, unchanged from Phase 1.

## Durable evidence

- Pre-removal enumeration: `/tmp/opencode/held_cleanup_20261003/held_full.txt` (884 rows)
- Pre-removal all-held re-verification: `/tmp/opencode/held_cleanup_20261003/pre_removal_verify.txt` (877/877 held)
- Removal log: `/tmp/opencode/held_cleanup_20261003/removal_log.txt` (per-cluster rc=0)
- Post-removal state: `/tmp/opencode/held_cleanup_20261003/post_removal_state.txt` (7 held)

Note: `/tmp/opencode` evidence is ephemeral; this audit file is the durable record.

## Follow-up (informational, no action taken)

- `flatphi_link` replacement campaign left 497/501 (djangoh14) and 352/364 (rapgap31)
  linked-tree files; if the missing chunks matter, remediate with a new verified
  submission for those chunks only — do not attempt to revive the removed attempt_1
  clusters (their submit-file stdout bug is documented in
  `condor_v6/studies/flatphi_link_fixed_20260927/status/launch_status.json`).
- The 5 protected held clusters (rad_crad_vs_detector_mc, Britzger crad smoke) remain
  held in the queue; their disposition is a separate owner decision.

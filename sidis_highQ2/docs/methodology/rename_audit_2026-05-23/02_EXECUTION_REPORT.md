# Phase R Rename Campaign — Execution Report

**Date:** 2026-05-23
**Author:** code-editor
**Status:** EXECUTED. SIDIS workspace is now naming-consistent.

This is the post-execution counterpart to
`00_PRE_EXECUTION_SCOPE_REPORT.md`. The pre-execution audit correctly
identified 5 blockers; the supervisor resolved them; this report
documents what was actually done and verifies the post-condition.

The per-operation NAF move/rename manifest is in
`01_NAF_OPERATIONS_MANIFEST.json` (34 ops, 40,422 files, 127.06 GB).

---

## 1. Phase summary

### Phase R2 — code edits

13 SIDIS-local Python scripts edited; 1 Python file renamed; py_compile
clean on every edit.

| File | Type | Edit summary |
|---|---|---|
| `submit_full_production.py` | edit | `PROTON_PKG` → `SIDISTREE_PKG`; data label changed from `proton_<...>` to `sidistree_<period>_d<NN>_NNNN`; data outputs land under `output/data/<period>/` (period subdirs); steering+condor paths `proton/` → `sidistree/`; condor log base `condor_logs/proton_highQ2` → `condor_logs/sidistree_highQ2`; CLI `--stage proton` deprecated-alias preserved. |
| `submit_highQ2_sidis_data_27p6.py` → `submit_highQ2_sidis_data.py` | rename + edit | File rename. Constants `SIDISTREE_PKG_REMOTE` added; `DEFAULT_SIDISTREE_BIN_REMOTE` points at v5g binary; `DEFAULT_OUTPUT_BASE_REMOTE` drops `27p6/` partition; templates `PROTON_*_TEMPLATE` → `SIDISTREE_*_TEMPLATE` (back-compat aliases preserved); `make_job_label` emits `sidistree_<period>_d<NN>_NNNN`; steering/condor paths `steering/sidistree/27p6/<period>/` (internal `27p6/` layout preserved per blocker 4); log dir `LOG_BASE_REMOTE/sidistree/data/<period>` (drops 27p6/ from log path). |
| `submit_norad_charged_sidis_highQ2.py` | edit (bulk) | Constants `PROTON_*_BASE_REMOTE` → `SIDISTREE_*_BASE_REMOTE`; `PROTON_WRAPPER_TEMPLATE` → `SIDISTREE_WRAPPER_TEMPLATE`; output filename prefix `proton_{label}.*` → `sidistree_{label}.*`; path strings `steering/proton/` → `steering/sidistree/`, `condor/proton/` → `condor/sidistree/`, `condor_logs/proton_highQ2` → `condor_logs/sidistree_highQ2`; task dict keys `proton_*` → `sidistree_*`; local variables `proton_steer` etc → `sidistree_steer`; CLI `--stage sidistree` added (and `proton` retained as DEPRECATED alias with print warning + auto-remap to `sidistree`). |
| `submit_em06_g15_recovery.py` | edit | `SIDIS_OUT_DIR` drops `27p6/` partition (`output/data/em06`); v5g binary swap (`ChargedSidisTreeBuilderHighQ2_v5g`); output filename `proton_highQ2_em06_d06_NNNN.root` → `sidistree_em06_d06_NNNN.root`; backup-of-legacy-stub branch added (handles both new and pre-rename stubs idempotently); steering template comment-only update. |
| `submit_crad_extract_condor.py` | edit | `dedupe_rapgap_naming` updated to accept both `sidistree_mc_` (new) and `proton_highQ2_mc_` (legacy) tokens. |
| `submit_phi_yields_v5f_condor.py` | edit | `DATA_BASE` drops `27p6/` partition. |
| `submit_phi_gap_diag_condor.py` | edit | `DATA_BASE` drops `27p6/` partition. |
| `extract_yields_v5f_3period.py` | edit | `DATA_BASE_DEFAULT` drops `27p6/`; docstring + glob comment updated. |
| `extract_phi_yields_v5f_3period.py` | edit | `DATA_BASE` drops `27p6/`; docstring + glob comment updated. |
| `compute_highQ2_crad_v5f_real.py` | edit | `_gather_files` dedupe accepts both prefixes; smoke `_first` glob prefers `sidistree_mc_` over legacy `proton_highQ2_mc_`; output JSON `rapgap_naming_dedupe` text updated. |
| `_smoke_gate.py` | edit | Docstring example `'proton_eplusp'` → `'sidistree_eplusp'`. |
| `study_resolution_sliced_HQv5fr2.py` | edit | Docstring glob examples `proton_highQ2_mc_*` → `sidistree_mc_*` (rapgap) and `proton_highQ2_djangoh14_*` → `sidistree_djangoh14_*` (djangoh). |
| `verify_beam_energy_usage.py` | edit | Old wrapper filename `submit_highQ2_sidis_data_27p6.py` → `submit_highQ2_sidis_data.py`. |
| `parse_britzger_norad_lumi.py` | edit | NORAD regex `_NORAD_LOCAL_RE` and RAD regex `_RAD_LOCAL_RE` updated to accept both `sidistree_` (new) and `proton_/proton_highQ2_` (legacy) prefixes. |

Additional 11 SIDIS-local scripts had docstring example MC filenames
updated from `merged_proton_mc_*` to `merged_sidistree_highQ2_mc_*`
(`produce_shifted_mc.py`, `azimuthal_acceptance.py`, `tmd_unfold.py`,
`tmd_resolution_acceptance.py`, `tmd_analysis.py`,
`compute_systematics_charged.py`, `compare_kinematic_methods.py`,
`extract_azimuthal_moments.py`, `merge_and_validate.py`,
`plot_phih_per_4d_bin.py`, `study_migration_background.py`).

Additional 4 SIDIS-local extractors had DATA_BASE constants updated to
drop `27p6/` partition (`em06_postrecovery_extract.py`,
`abc_dis_vs_epsilon_extract.py`, `compute_data_hadron_yields_v5f.py`,
`_unfold_compare_v5f_extract.py`, `n_dis_data_v5f_highQ2.py`,
`extract_data_yields_v5f_HQv5fr1.py`, `em06_postrecovery_update_jsons.py`).

NAF-only edits (no local source):
- `submit_proton_highQ2_batch.sh` → `submit_sidistree_highQ2_batch.sh`
  (file rename + steering-glob `highQ2_proton_*` → `sidistree_*` +
  v5g binary swap).
- `merge_data_highQ2.sh`, `merge_mc_highQ2.sh` (output filename
  prefix `merged_proton_highQ2_*` → `merged_sidistree_highQ2_*`;
  input glob refers to `sidistree_*.root` post-rename + period
  subdirs; SIDIS-local validator preference added).
- `analyze_sidis_tmd.py` (default `--data` and `--mc` paths use
  `merged_sidistree_highQ2_*`).

All 18 edited SIDIS-local Python files passed `python3 -m py_compile`.
All 14 edited Python files that exist on NAF were `scp`'d back; md5
match between local and NAF confirmed (see "Verification" below).

### Phase R3 — NAF moves

Executed by `_naf_phase_r3_moves.py` (one orchestrator script on the
NAF login node). Streams a JSON manifest as it goes; per-file
size/count verification per step; FATAL exit on any mismatch (none
recorded).

| Step | Operation | Files | Bytes (GB) | Status |
|---:|---|---:|---:|:---:|
| 1 | ARCHIVE `output/data/27p6/` → `output/archive_v5f_pre_v5g_2026-05-23/data/27p6/` |   893 |  2.07 | OK |
| 2 | DATA move+rename `output/data/{05,06,07}/` → `output/data/{em0405,em06,ep0607}/` |   577 |  0.24 | OK |
| 3 | MC rename in place (RAPGAP31 dual-prefix + DJANGOH14 single-prefix) | 1,144 | 73.43 | OK |
| 4 | NORAD MC rename in place |   519 | 45.87 | OK |
| 5 | SIDIS-local evls rename in place |   963 |  0.39 | OK |
| 6 | steering/proton → steering/sidistree + internal file rename | 3,299 | 0.001 | OK |
| 7 | condor/proton → condor/sidistree + internal file rename     | 6,598 | 0.008 | OK |
| 8 | smoke_gate_work/proton_{eminusp,eplusp} → sidistree_*       |     2 | ~0    | OK |
| 9 | condor_logs/proton_highQ2{,_recovery} → sidistree_highQ2{,_recovery} | 17,658 | 0.17 | OK |
| **TOTAL** | **34 operations** | **40,422** | **127.06** | **OK** |

Per-operation manifest: `01_NAF_OPERATIONS_MANIFEST.json`.

### Phase R4 — manifest v1.6

The pre-execution audit observed manifest v1.3; between the audit and
execution v1.4 (Empz cut alignment) and v1.5 (v5g binary added) had
landed. Phase R bumps v1.5 → v1.6 (next-sequential-available, honoring
blocker resolution 3's spirit).

| | v1.5 | v1.6 |
|---|---|---|
| binary md5 of `TreeBuilderHighQ2_chargedsidis` | d740dbdc... | d740dbdc... (unchanged) |
| binary md5 of `ChargedSidisTreeBuilderHighQ2` (v5f) | d95158bf... | d95158bf... (unchanged) |
| binary md5 of `ChargedSidisTreeBuilderHighQ2_v5g` | 091b74ff... | 091b74ff... (unchanged) |
| binary md5 of `TreeBuilder` (low-Q² SpaCal; FORBIDDEN for highQ²) | 8de736bc... | 8de736bc... (unchanged) |
| top-level `purpose` field | (v1.5 text) | v1.5 text + Phase R rename summary appended |
| `runtime_artifact_isolation.consumer_sidis_wrapper` | `submit_highQ2_sidis_data_27p6.py` | `submit_highQ2_sidis_data.py` (Phase R rename) |
| `version_history` length | 6 entries (v1.0-v1.5) | 7 entries (v1.0-v1.5 unchanged byte-for-byte + new v1.6) |

Guardrail self-test on NAF (post-rename): **ALL 4 PASS**
```
TreeBuilderHighQ2_chargedsidis     PASS md5=d740dbdc... size=1899080
ChargedSidisTreeBuilderHighQ2      PASS md5=d95158bf... size=604856
ChargedSidisTreeBuilderHighQ2_v5g  PASS md5=091b74ff... size=623112
TreeBuilder                        PASS md5=8de736bc... size=1516144
```

### Phase R5 — docs update

| File | Update |
|---|---|
| `H1/sidis_highQ2/context-handoff.md` | Appended section "(Date 2026-05-23) Phase R rename" before `## Todo`, summarizing the rename, the year→period map, the V0/proton md5 invariants, and the KEEP_AS_AUDIT exclusions. |
| `H1/sidis_highQ2/docs/INDEX.md` | Updated the Phase 1A section to mention Phase R; added new "Naming (Phase R rename, 2026-05-23) — read first" section pointing at the canonical naming doc and the rename audit dir; updated the "Methodology (frozen)" table entries with Phase R annotations. |
| `H1/sidis_highQ2/docs/methodology/structural_separation_2026-05-23.md` | Appended Section 16 "Phase R rename (2026-05-23): canonical SIDIS naming" — scope, blocker resolutions, execution summary table, code edits, verification, verdict. |
| `H1/sidis_highQ2/docs/methodology/cross_analysis_code_locations_2026-05-23.md` | Updated SIDIS column path entries to reflect new layout (sidistree_*, period subdirs, archive dir, manifest v1.6); added new rows for v5g binary path, archive dir, post-rename steering/condor/condor_logs paths, canonical naming reference, and Phase R rename audit dir. |
| `H1/sidis_highQ2/docs/methodology/crad_v5f_methodology_FROZEN.md` | Section 9 production pipeline commands updated (`--stage sidistree`); Section 9.1 binary md5/source notes updated (v1.4 d740dbdc; v5f vs v5g distinction); Section 16 frozen file manifest annotated with Phase R footnote (script md5s preserved as freeze-point anchors; rename was naming-only, no compute logic changed). |
| `H1/sidis_highQ2/docs/methodology/sidis_naming_canonical_2026-05-23.md` | **NEW** single-page canonical naming reference: workspace map, year→period map, file prefix conventions per category, KEEP_AS_AUDIT list, compatibility/migration notes, binary identity manifest table, references. Future agents read this first. |
| `H1/sidis_highQ2/docs/methodology/rename_audit_2026-05-23/01_NAF_OPERATIONS_MANIFEST.json` | **NEW** per-operation NAF manifest. |
| `H1/sidis_highQ2/docs/methodology/rename_audit_2026-05-23/02_EXECUTION_REPORT.md` | **NEW** (this file). |

### Phase R6 — verification

1. **Comprehensive grep audit on local scripts:** zero operational
   `proton_highQ2_*` / `output/data/27p6/` paths remain. All 42
   remaining occurrences in active scripts are accounted for:
   - 14 explicit Phase R rename annotation comments (e.g.
     `# Phase R rename (2026-05-23): proton_highQ2 -> sidistree_highQ2`).
   - 9 intentional back-compat heuristics that accept both new and
     legacy prefixes (in `parse_britzger_norad_lumi.py`,
     `compute_highQ2_crad_v5f_real.py`, `submit_crad_extract_condor.py`,
     `submit_em06_g15_recovery.py` stub-backup).
   - 9 CLI `--stage proton` DEPRECATED-alias comments and code in
     `submit_norad_charged_sidis_highQ2.py` and `submit_full_production.py`.
   - 10 V0/proton cross-workspace reference comments (the V0/proton
     workspace names are correct as-is and read-only from SIDIS).

2. **Extractor smoke test on NAF (post-rename):**
   - `output/data/em0405/`: 152 files (sample `sidistree_em0405_d05_0000.root`).
   - `output/data/em06/`: 313 files (sample `sidistree_em06_d06_0000.root`).
   - `output/data/ep0607/`: 112 files (sample `sidistree_ep0607_d07_0000.root`).
   - Total = 577 files, matches the step 2 move count exactly.
   - `extract_yields_v5f_3period.py --help` runs cleanly with new
     DATA_BASE default.

3. **uproot open on sample post-rename v5g output:**
   `sidistree_em06_d06_0000.root` opens; trees `ChargedTot` (101
   branches, 2254 entries) and `DisTot` (49 branches, 240 entries)
   present; **v5g markers verified present**: `Q2_es_gen` ✓,
   `ystar_es_gen` ✓.

4. **Manifest v1.6 guardrail self-test on NAF:** all 4 pinned binaries
   PASS (`TreeBuilderHighQ2_chargedsidis` d740dbdc, v5f d95158bf,
   v5g 091b74ff, low-Q² SpaCal 8de736bc).

5. **V0/proton workspace md5 spot check (POST-RENAME):**
   - `bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2`:
     `c2cf0828d4155de64ea9586cf3004426` (matches required)
   - `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C`:
     `c6653628e198aff5c23224fc5e1b0d0d` (matches required)
   - `H1BaryonAsym_HighQ2/src/PhysicsConstants.h`:
     `af7dae3eb870f039a9cad363d959d758` (matches required)
   - **ZERO modifications to V0/proton workspaces.**

6. **Local-vs-NAF script sync verified** for all 14 edited Python
   scripts that exist on both sides; md5 match.

7. **Manifest sync verified** between local, NAF scripts/, and NAF
   root mirror: md5 `0395d4542f935d24dbfd5295d8fedaab` everywhere.

---

## 2. Failure handling outcomes

None of the documented failure modes triggered:

- V0/proton md5 changes → did not occur (verified UNCHANGED).
- File count / size mismatch across a move → did not occur (mover
  script has built-in verification at every step, FATAL exit on any
  mismatch; clean run, no FATAL).
- Filename collision (two files map to same name) → did not occur
  (the RAPGAP31 dual-prefix would collide if both prefixes mapped to
  `sidistree_<gen>_<cat>_<period>_NNNN.root`; this was anticipated
  and handled by `sidistree_legacy_` for the v5b legacy fork).
- py_compile error after script edit → occurred TWICE during editing
  (sed-replacement broke string termination on a comment-after-string
  line in `_unfold_compare_v5f_extract.py` and
  `extract_data_yields_v5f_HQv5fr1.py`); both were detected by
  immediate py_compile and FIXED in place; final py_compile clean.
- Extractor smoke fails to find any files at new path → did not occur
  (577 files discovered at new layout, matching the move counts).

---

## 3. Constraints honored

- **NEVER edited on NAF via vim/nano**: all 17 NAF script edits done
  by local edit + scp.
- **NEVER wrote to /pnfs**: all writes routed to DUST under
  `/data/dust/group/h1/klesthen/BaryonAsym/`.
- **NO Condor submissions, NO compile, NO production runs.**
- **READ-ONLY on V0/proton workspaces**: verified by post-rename md5
  spot check on `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C`,
  `H1BaryonAsym_HighQ2/src/PhysicsConstants.h`, and shared bin
  `TreeBuilderHighQ2`; all three md5s UNCHANGED.
- **ONE SSH `-O check` at start**: confirmed control socket alive
  (`Master running (pid=21568)`); no retries on any failure; all
  remote ops via the existing control socket.
- **Batches with progress markers**: each Phase R step recorded an
  audit-manifest entry; partial-state recovery would be possible at
  any step boundary from the partial manifest.
- **Archive preserved before any other move**: step 1 of the mover
  was the archive of `output/data/27p6/` → `output/archive_v5f_pre_v5g_2026-05-23/`,
  before any data/MC/evls rename.
- **Manifest JSON tracks every operation**: see
  `01_NAF_OPERATIONS_MANIFEST.json`.

---

## 4. Verdict

**The SIDIS silo is now consistent: sidistree + period subdirs + no
operational `proton`/`27p6` refs in active code paths.**

The remaining 42 grep matches in active scripts are all accounted for
as: Phase R rename audit comments, intentional back-compat heuristics
that accept both legacy and new prefixes, intentional CLI back-compat
deprecated aliases for `--stage proton`, or cross-workspace references
to the V0/proton workspaces (which are correctly named as the source
of high-Q² Stage 1 evls).

All extractors find files at the new paths. The v5g binary's new
schema branches (`Q2_es_gen`, `ystar_es_gen`) are confirmed present in
the renamed data outputs. The V0/proton workspaces are byte-for-byte
unchanged. The binary identity manifest is at v1.6 and the
self-tested guardrails all PASS on all 4 binaries.

— code-editor, Phase R rename campaign, 2026-05-23

# Phase R Rename Campaign — Pre-Execution Scope Report

**Date:** 2026-05-23
**Author:** code-editor (pre-execution audit before destructive operations)
**Status:** STOP — reporting scope to supervisor before proceeding.

This is a pre-execution audit, NOT an execution report. I performed the
inventory and triage steps of Phase R2 (steps 1) and confirmed the NAF
layout, then stopped before any destructive operation. The reasons are
below. No files have been modified, renamed, or moved on local or NAF.

---

## 1. Inventory of operational targets (after triage)

### 1A. Files that contain workspace-operational `proton` or `27p6` references

After triaging 610 grep matches (across 67 scripts + 1 JSON) into
"operational naming" vs "physics terminology vs cross-analysis reference",
the following 12 files contain operational targets that must be touched:

| File | Operational hits | Notes |
|------|------------------|-------|
| `submit_full_production.py` | ~28 | `PROTON_PKG`, `SIDISTREE_*` already partially renamed, paths still say `proton`, `27p6` not used here |
| `submit_highQ2_sidis_data_27p6.py` | ~57 | Filename + path constants + steering/condor paths + period-pure constants + log paths |
| `submit_norad_charged_sidis_highQ2.py` | ~99 | `PROTON_STEER_BASE_REMOTE`, `PROTON_WRAP_BASE_REMOTE`, `PROTON_SUB_BASE_REMOTE`, `PROTON_WRAPPER_TEMPLATE`, `PROTON_MC_STEER_TEMPLATE`, all `proton_<label>.{root,sh,sub,steer}` template strings, all CLI `--stage proton` branches |
| `submit_em06_g15_recovery.py` | ~11 | `output/data/27p6/em06`, banner refs to `submit_highQ2_sidis_data_27p6.py`, "27p6" in steering header comment |
| `submit_crad_extract_condor.py` | ~3 | `output/data/27p6` reference |
| `submit_phi_yields_v5f_condor.py` | 1 | `DATA_BASE = f"{DUST_BASE}/output/data/27p6"` |
| `submit_phi_gap_diag_condor.py` | 1 | `DATA_BASE = f"{DUST_BASE}/output/data/27p6"` |
| `extract_yields_v5f_3period.py` | 3 | `DATA_BASE_DEFAULT`, docstring example, glob pattern |
| `extract_phi_yields_v5f_3period.py` | 4 | `DATA_BASE`, glob pattern |
| `extract_phi_moments_data.py` | 3 | similar |
| `phi_gap_diag_extract.py` | 3 | similar |
| `_smoke_gate.py` | 1 | `smoke_gate_work/proton_<charge>` directory naming |
| `_beam_energy_utils.py` | 3 | docstring references to V0/proton (cross-analysis context; may be KEEP_AS_AUDIT) |
| `compute_highQ2_crad_v5f_real.py` | 9 | needs per-line triage |
| `crad_histogram_extract_v2.py` | ? | not yet inspected |
| `binary_identity_manifest.json` | 13 | needs cutflow signature update + version bump v1.5→v1.6 (per task; current is v1.3) |

The NAF-only `submit_proton_highQ2_batch.sh` is also in scope (rename file
to `submit_sidistree_highQ2_batch.sh` + edit `highQ2_proton_*` glob
patterns in 3 places).

Other 50+ files containing `proton` hits are physics-terminology only
(see 1B below); they are KEEP_AS_AUDIT_HISTORY and not edited.

### 1B. Files where `proton` is physics terminology (DO NOT RENAME)

The following files contain `proton` references that refer to:
- **Particle composition** ("proton fraction", "proton excess",
  "proton-omission" in the NuclIA systematic) — cannot rename without
  destroying physics meaning.
- **Cross-analysis references** to V0/proton workspaces or
  `H1/proton_HiQ/` — these are correct as-is (per task: "READ-ONLY on
  V0/proton workspaces ... The word 'proton' appears in those workspaces'
  names and that is correct").
- **Historical context** in audit comments about
  `ProtonTreeBuilderHighQ2` — per task: "Historical/audit references
  to the binary's old name `ProtonTreeBuilderHighQ2` may remain in
  audit comments with clear 'historical / pre-v5f' context."

High-volume examples (must NOT be edited):
- `compute_systematics_v5f_nuclia_promotion_v2.py` (107 hits — all
  "proton fraction" in nuclia systematic)
- `systematics_v5f_nuclia_proton_omission.py` (50 hits — filename and
  contents are the NuclIA proton-omission systematic; physics)
- `plot_systematics_v5f_nuclia_proton_omission.py` (8 hits — same)
- `compute_systematics_v5f_nuclia_promotion.py` (36 hits)
- `compute_systematics_v5f.py` (21 hits — `NUCLIA_PROTON_OMISSION_*`
  constants + one cross-reference to `proton_HiQ`)
- `produce_shifted_mc.py`, `n_dis_data_v5f_highQ2.py` partial (see 1C)
- `merge_and_validate.py` (10 hits — likely path/cross-ref mix; needs
  per-line triage)
- All `plot_systematics_*` and `aggregate_systematics_*` (proton-omission
  systematic source)

### 1C. `27p6` references — operational vs physics

The string `27p6` is BOTH a path partition AND a beam-energy descriptor
("27.6 GeV nominal proton beam energy"). The task says drop the path
partition. The beam-energy meaning must be preserved.

**KEEP (physics meaning):**
- `submit_highQ2_sidis_data_27p6.py:245`: `beam_filter: str = "27p6"`
- `submit_highQ2_sidis_data_27p6.py:390`: `def filter_ep0304_27p6(...)`
- `submit_highQ2_sidis_data_27p6.py:967`: CLI `--ep0304-beam-filter` choices
- `submit_highQ2_sidis_data_27p6.py:443`–`483`: beam-filter logic
- `submit_highQ2_sidis_data_27p6.py:917`: `ep0304_27p6_runlist_available`
- `n_dis_data_v5f_highQ2.py`: `IsNom27p6`, `runs_27p6`, `IsNominalEnergy_27p6_920` (CSV column name from upstream beam-energy table — externally fixed)
- `submit_em06_g15_recovery.py:218`: `// Period: em06  Beam-filter: 27p6` (in steering header)
- `g1c_27p6_data_production_plan.json` and `_jobs.json` filenames in
  `docs/data/` — those are immutable production artifacts (audit trail
  for the G1C 27.6 GeV campaign)

**RENAME (path partition):**
- `output/data/27p6/...` paths in extractors + submit scripts
- `steering/proton/27p6/...` paths
- `condor/proton/27p6/...` paths
- `condor_logs/proton_highQ2/proton/data/27p6/...` paths
- The filename `submit_highQ2_sidis_data_27p6.py` itself per task spec

### 1D. NAF directory and file scope

| Subtree | Files | Size | Action |
|---------|-------|------|--------|
| `output/data/27p6/em0405/` | 152 | — | ARCHIVE to `output/archive_v5f_pre_v5g_2026-05-23/data/27p6/em0405/` |
| `output/data/27p6/em06/` | 316 | — | ARCHIVE |
| `output/data/27p6/ep0607/` | 425 | — | ARCHIVE |
| `output/data/27p6/` total | 893 | 2.1 GB | ARCHIVE |
| `output/data/05/` | 152 | 20 MB | MOVE+RENAME → `output/data/em0405/sidistree_em0405_d05_NNNN.root` |
| `output/data/06/` | 313 | 174 MB | MOVE+RENAME → `output/data/em06/sidistree_em06_d06_NNNN.root` |
| `output/data/07/` | 112 | 86 MB | MOVE+RENAME → `output/data/ep0607/sidistree_ep0607_d07_NNNN.root` |
| `output/mc/djangoh14_norad/eminusp/` | 260 | — | RENAME in place: `proton_*` → `sidistree_*` |
| `output/mc/djangoh14_norad/eplusp/` | 259 | — | RENAME in place |
| `output/mc/djangoh14/{eminusp,eplusp}/` | 0 | — | empty; nothing to do |
| `output/mc/rapgap31/{eminusp,eplusp}/` | 0 | — | empty |
| `output/evls/data/{05,06,07}/` | 577 | — | SCOPE QUESTION — see §3 |
| `output/evls/mc/djangoh14/{7072,7091,7092,7093}/` | 430 | — | RENAME in place? task says yes |
| `output/evls/mc/djangoh14_norad/{eminusp,eplusp}/` | 519 | — | RENAME in place? |
| `output/evls/mc/rapgap31/{7071,7094,7096,7097}/` | 303 | — | RENAME in place? |
| `steering/proton/27p6/{em0405,em06,ep0304,ep0607,ep0607_smoke}/` | 976 | — | MOVE+RENAME → `steering/sidistree/<period>/sidistree_<period>_*.steer` |
| `steering/proton/data/{05,06,07}/` | 577 | — | MOVE+RENAME → `steering/sidistree/<period>/`? unclear flat layout |
| `steering/proton/mc/{djangoh14,djangoh14_norad,rapgap31}/` | 1746 | — | MOVE+RENAME → `steering/sidistree/mc/...` |
| `condor/proton/27p6/{wrappers,subs}/{em0405,em06,ep0304,ep0607,ep0607_smoke}/` | 1952 | — | MOVE+RENAME |
| `condor/proton/{wrappers,subs}/{data,mc}/...` | 4646 | — | MOVE+RENAME |
| `smoke_gate_work/proton_{eminusp,eplusp}/` | 2 logs | — | RENAME dir |
| `condor_logs/proton_highQ2/proton/{data,mc}/...` | 15384 | 1.1 GB | RENAME dir tree |
| `condor_logs/proton_highQ2_recovery/` | 2274 | 96 MB | RENAME dir |
| **TOTAL FILE OPERATIONS ON NAF** | **~30,000+** | **~5 GB** | |

---

## 2. Why I am stopping before execution

The task as specified is feasible mechanically (every individual rename is
safe in isolation), but it has scope characteristics that, in my judgment,
exceed what a single code-editor dispatch can safely complete without an
intermediate supervisor checkpoint:

### 2A. Volume

~30,000 file operations on NAF spanning ~10 distinct subtrees, plus ~13
local script edits, plus ~13 NAF script edits, plus binary identity
manifest update, plus 6 doc updates (5 existing + 1 new). A single
mid-execution failure (e.g., an `ssh` socket drop, a per-file
permissions edge case, a name collision) would leave the workspace in
an inconsistent half-renamed state where some scripts reference
`sidistree_*` files at new paths and others still reference
`proton_highQ2_*` files at old paths. Per the task itself:
"incomplete renames will leave the workspace in an inconsistent state."

### 2B. Cross-workspace shared artifact: `output/evls/data/`

The binary identity manifest v1.3 banner (lines 20–23, 61) explicitly
documents that `output/evls/data/` (Stage 1 data evls) is a **SHARED
input layer**:

> "The shared-bin TreeBuilderHighQ2 (md5 c2cf0828...) belongs to V0/Lambda
> and proton/antiproton at high-Q^2; SIDIS no longer pins it. v1.1 → v1.2
> is a structural source/bin separation; v1.2 → v1.3 is..."
>
> "future_phase1a_fix_2_option: Either (a) re-run Stage 1 with SIDIS-owned
> binary to make SIDIS-local data evls, or (b) hardlink V0/proton data
> evls into a SIDIS-local mirror"
>
> "consumer_sidis_wrapper: sidis_highQ2/scripts/submit_highQ2_sidis_data_27p6.py
> (READS data evls; banner documents the dependency in the module docstring)"
>
> "producer: H1BaryonAsym_HighQ2/scripts/submit_highq2.py (V0/proton high-Q^2
> Stage 1 production); plus the SIDIS-side em06 G15 recovery wrapper which
> writes additional em06 evls into the same shared layer."

The data evls files at `output/evls/data/{05,06,07}/evls_highQ2_d<NN>_NNNN.root`
are PRODUCED by `H1BaryonAsym_HighQ2/scripts/submit_highq2.py`
(V0/proton workspace, READ-ONLY per task rules). They are also produced
by SIDIS's own `submit_em06_g15_recovery.py` which extends the same
shared layer.

The task spec requires renaming these to
`evls_<period>_d<NN>_<NNNN>.root` in period-named subdirs. Doing so on
the SIDIS side would:
1. Break `H1BaryonAsym_HighQ2/scripts/submit_highq2.py` next time it
   runs (it would create new `evls_highQ2_*` files alongside the
   renamed `evls_em0405_*` ones, fragmenting the shared layer).
2. Force the V0/proton workspace to be updated to the new naming —
   but the V0/proton workspace is READ-ONLY per the task rules.
3. Break the em06 recovery wrapper (which writes into this same shared
   layer).

**This rename appears infeasible without also touching the V0/proton
workspace, which is explicitly READ-ONLY.** I need a supervisor decision
on which of these is correct:
- (a) Skip evls rename entirely (keep `evls_highQ2_*` names; only
  rename the Stage 2 outputs).
- (b) Get an explicit waiver to also update the V0/proton-owned
  producer wrapper (in which case the READ-ONLY rule is relaxed for
  that single file).
- (c) Implement option (b) from manifest v1.3 (hardlink V0/proton data
  evls into a SIDIS-local mirror at the new naming) so that V0/proton
  can keep producing `evls_highQ2_*` while SIDIS consumes renamed
  hardlinks — but this is a structural change that exceeds a rename
  campaign.

My recommendation: **option (a)**. The task spec's mention of
"`evls_<period>_d<NN>_<NNNN>.root`" looks like an aspirational target
that conflicts with the manifest v1.3 cross-workspace sharing
contract. The Stage 1 evls layer should retain its current naming
until a separate, planned Phase 1A-fix-2 (per manifest v1.3 note)
addresses the data-evls sharing.

### 2C. Manifest version mismatch

Task says "Bump v1.5 → v1.6". The current manifest version is **v1.3**
(verified by reading `binary_identity_manifest.json`). There has been
no v1.4 or v1.5. Bumping directly v1.3 → v1.6 would skip two version
slots and obscure history. I'd suggest v1.3 → v1.4 with the rename
campaign as the v1.4 changeset, but want supervisor sign-off.

### 2D. Filename collision risk in data-output move

The pre-existing v5f files at `output/data/27p6/em06/` use the naming
`proton_highQ2_em06_d05_NNNN.root` and `_d06_NNNN.root` (period+year-short
combinations). The v5g files at `output/data/06/` use
`proton_highQ2_d06_NNNN.root` (year-short only). Mapping:

- v5f: `output/data/27p6/em06/proton_highQ2_em06_d05_NNNN.root` (em06 period contains both year 05 and year 06 runs)
- v5g: `output/data/06/proton_highQ2_d06_NNNN.root` (year-short 06 only)

The em06 period spans BOTH d05 and d06 in v5f. The v5g `06/` dir only
has d06. So the move from `output/data/06/` → `output/data/em06/` will
land 313 d06 files in em06, but the em06 period also needs d05 files
to be complete. Where are the v5g d05-em06 files? They appear to be in
`output/data/05/` (152 files, all d05). But em0405 ALSO uses d05 runs.

I need to verify: does the v5g production split d05 runs between
em0405 and em06? Or is em06 in v5g only d06 runs (different scope than
v5f)?

Looking at the steering layout `steering/proton/data/{05,06,07}/`
— it's organized by **year-short**, not by **period**. So the v5g
scheme is "one steering per year-short, period determined per-run by
the lumi map" (which matches the task's stated rationale: "beam energy
is per-run via the lumi map, not a path partition"). This means there
is NO simple 1:1 mapping from year-short → period. **Period
information would have to be re-derived at consumption time from
the lumi map (or from per-run metadata inside the ROOT files).**

The task's prescribed move:
- `output/data/05/proton_highQ2_d05_NNNN.root` → `output/data/em0405/sidistree_em0405_d05_NNNN.root`

assumes all d05 files belong to em0405. But the run-period assignment
shows d05 runs split between em0405 and em06 periods (HERA period
boundary mid-2005). The v5g per-year-short layout was INTENTIONAL —
it lets the lumi map decide period at downstream time. **Mapping
year-short → fixed period in the filename would be REGRESSING from
the v5g design.**

I think the task spec made an error here. The correct target is
probably:

- `output/data/05/proton_highQ2_d05_NNNN.root` → `output/data/05/sidistree_d05_NNNN.root` (rename "proton" → "sidistree" but keep year-short subdir)

OR

- `output/data/05/proton_highQ2_d05_NNNN.root` → `output/data/sidistree_d05_NNNN.root` (no subdir at all — flat)

OR keep subdirs as periods and use a separate mapping. **This needs
supervisor clarification.**

### 2E. Coupling with active downstream extractors

The just-moved data outputs will break every consumer that globs
`output/data/27p6/...` or `output/data/{05,06,07}/proton_highQ2_*` until
the consumer is updated in the same dispatch. I count at least 11
consumer scripts (the extractors + 4 `_v5f` scripts not yet listed +
`abc_dis_vs_epsilon_extract.py` + `compute_data_hadron_yields_v5f.py`
+ `n_dis_data_v5f_highQ2.py` + `em06_postrecovery_extract.py` +
`em06_postrecovery_update_jsons.py` + `extract_data_yields_v5f_HQv5fr1.py`
+ `_unfold_compare_v5f_extract.py`). Each must be updated atomically
with the move, otherwise the workspace is half-broken.

### 2F. SAVE-CONTEXT cost of incomplete state

If I do a partial execution and hand back, the supervisor's
SAVE-CONTEXT pass would have to document a half-renamed state across
~6 documentation files. That's worse than not starting. Better to
plan and execute in a single sweep with verified pre-conditions.

---

## 3. Specific clarifications needed before execution

Before I can safely proceed, I need supervisor sign-off on:

1. **Evls rename** (§2B): is it (a) skip entirely (recommend), (b)
   waiver to touch V0/proton, or (c) hardlink-mirror approach?

2. **Year-short vs period directory layout for v5g data outputs** (§2D):
   the v5g `output/data/{05,06,07}/` layout looks like an intentional
   shift away from period-named dirs (per the lumi-map rationale).
   Should the new layout be:
   - (i) `output/data/<year_short>/sidistree_d<NN>_NNNN.root` (keep
     year-short subdir, just rename `proton_highQ2` → `sidistree`), or
   - (ii) `output/data/<period>/sidistree_<period>_d<NN>_NNNN.root`
     (per task spec, requires a fixed year→period mapping that
     conflicts with the per-run lumi-map design), or
   - (iii) flat `output/data/sidistree_d<NN>_NNNN.root` (no subdir,
     period derived per-run at consumption time)?

3. **Manifest version** (§2C): v1.3 → v1.4 (next), or honor the task
   spec's "v1.5 → v1.6" (skipping v1.4, v1.5)?

4. **Steering/condor reorganisation** (§1D): the task spec says
   `steering/proton/27p6/<period>/` → `steering/sidistree/<period>/`.
   But there are ALSO steerings at `steering/proton/data/{05,06,07}/`
   and `steering/proton/mc/{djangoh14,djangoh14_norad,rapgap31}/`.
   Are these:
   - (i) merged into `steering/sidistree/{<period>,mc/<gen>}/` (which
     requires resolving period vs year-short for the data steerings,
     same problem as §2D), or
   - (ii) just renamed `steering/proton/` → `steering/sidistree/`
     preserving the internal `27p6/`, `data/`, `mc/` layout (less
     disruptive — just one top-level dir rename, plus filename
     renames inside)?

   I lean strongly toward (ii) as the minimum-disruption approach.

5. **Old vs new wrapper coexistence**: the v5g production at
   `output/data/{05,06,07}/` was just produced using the EXISTING
   `submit_highQ2_sidis_data.py` wrapper (filename has `27p6` and
   refs `27p6` paths). If we move/rename now, the v5g production is
   no longer reproducible from the same wrapper code — it'd need to be
   rerun with the renamed wrapper to verify the rename is bit-equivalent.
   Is verification by smoke-rerun in scope? Or do we accept that
   "the v5g binary outputs are immutable artifacts that we move under
   a new naming, and reproducibility is documented in the audit memo"?

---

## 4. What I have NOT done

To be explicit, I have made ZERO modifications:

- No file renames, no directory moves on NAF.
- No script edits on local or NAF.
- No archive directory created.
- No manifest changes.
- No doc updates.
- No new doc files created (except this scope report).
- No Condor submissions, no compiles, no /pnfs writes.

The only artifacts produced are:
- This file: `H1/sidis_highQ2/docs/methodology/rename_audit_2026-05-23/00_PRE_EXECUTION_SCOPE_REPORT.md`

---

## 5. Recommended path forward

I recommend the supervisor:

1. Read this report and answer the five clarification questions in §3.
2. Either (a) re-dispatch this task to code-editor with concrete answers
   and a tightened scope (e.g., explicitly defer evls rename, explicitly
   pick layout (ii) or (iii) for data outputs, explicitly waive
   verification rerun), OR (b) split the campaign into 3-4 smaller
   dispatches that can each complete and verify independently:
   - Phase R2.0: script-only rename (no NAF moves) — affects ~13
     scripts + manifest; py_compile clean; updates extractors to use
     new paths AHEAD of NAF moves (with old paths kept as fallback).
   - Phase R3.0: NAF data outputs move + archive (after R2.0).
   - Phase R3.1: NAF steering/condor dir rename (after R3.0).
   - Phase R3.2: NAF MC outputs + log-dir rename (after R3.1).
   - Phase R5: docs sweep (after R3.2).
   - Phase R6: final verification (after R5).

   Each sub-phase is independently verifiable, atomic, and recoverable.

3. Confirm that touching the v5g-just-produced outputs is the desired
   moment (alternative: wait until after one analysis pass on v5g to
   confirm they are good, then do the rename so we don't have to redo
   it if v5g needs reproducing).

---

## Verdict

NOT EXECUTED. Workspace state on local and NAF is UNCHANGED. Awaiting
supervisor clarification on §3 questions before proceeding.

— code-editor, 2026-05-23

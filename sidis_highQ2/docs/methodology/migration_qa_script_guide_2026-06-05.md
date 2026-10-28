# Migration QA Script Guide: `quantify_v6_migrations_smoke.py`

**Date:** 2026-06-05
**Analysis:** H1 Charged-Particle SIDIS High-Q² (`Q² > 150 GeV²`)
**Script path (local):**
`H1/sidis_highQ2/scripts/diagnostics/quantify_v6_migrations_smoke.py`

---

## 1. Purpose

The script verifies migration-category coverage in a single Stage 2 smoke
ROOT file produced by the v6 `ChargedSidisTreeBuilderHighQ2_v6sys` builder.
Its primary use case is confirming that the Stage 1 / Stage 2 event-list
logic and the relaxed gen-denominator envelope (Q²_gen > 60 GeV² OR
Stage 2 acceptance; see `v6_production_changes_2026-06-04.md`) populate
both the reco-side tree (`ChargedTot`) and the gen-side tree
(`ChargedTotGen`) with the expected event overlap.

Run it after any change to:

- the Stage 1 event-list steering or GRL injection;
- the gen-denominator cuts in the response builder (e.g. removal of the
  gen Pt cut or gen |η| cut introduced in v6 Change #1);
- the OR-envelope logic that gates which MC events reach `ChargedTotGen`.

The script operates **entirely locally** on one ROOT file and produces no
NAF jobs.

---

## 2. The Four Migration Categories

In a SIDIS detector-level analysis the response matrix connects generator
truth events to reconstructed events. Four quadrants exist:

| Category | Gen in fiducial? | Reco in fiducial? | In this script? |
|---|---|---|---|
| **gen out → reco in** | No | Yes | Yes — counted as `reco_only` events |
| **gen in → reco out** | Yes | No | Yes — counted as `gen_only` events |
| **gen in → reco in** | Yes | Yes | Yes — counted as `both` events |
| **gen out → reco out** | No | No | No — neither tree contains these events; they are unavailable background context and cannot be observed in the selected trees |

The script quantifies the first three categories by comparing the sets of
`(RunNumber, EventNumber)` pairs present in `ChargedTot` (reco-side) and
`ChargedTotGen` (gen-side), with and without fiducial selection applied.

The fourth category (gen out → reco out) is not populated in the Stage 2
output trees and is therefore absent from all counts. Do not attempt to
construct a "gen-out, reco-out" estimate from this script; the underlying
events were never written.

---

## 3. Event-Level Approximation (not per-track matched)

The fiducial classification is an **event-level, any-track approximation**.
An event is flagged as "in fiducial" on the reco side if ANY charged-hadron
track row in `ChargedTot` for that `(RunNumber, EventNumber)` satisfies all
per-track fiducial cuts. Similarly for the gen side in `ChargedTotGen`.

This is **not** a per-track matched response matrix. No track-to-track
association between the reco and gen sides is performed. A single event
can appear in `reco_only` simply because its one passing reco track has
no corresponding gen track that also passes the gen fiducial (even if the
event has gen tracks that fail individually). The `notes.event_classifier`
field in the JSON output records this explicitly.

This approximation is sufficient for a smoke-level sanity check of event
population and category balance. For physics analysis, the response matrix
is built by the per-charge Python builder (`build_migration_th2_per_charge_v6.py`)
using inlined gen-matching on consumed `(Run, Event)` keys.

---

## 4. Fiducial Definition

The signal fiducial applied to both sides:

| Variable | Condition | Reco branch (preferred / fallback) | Gen branch (preferred / fallback) |
|---|---|---|---|
| Q² | > 150 GeV² | `Q2s` / `Q2es` | `Q2sGen` / `Q2_es_gen` / `Q2esGen` |
| y | 0.1 < y < 0.7 | `Ys` / `Yes` | `YsGen` / `Y_es_gen` / `YesGen` |
| Empz | 35–70 GeV | `Empz` | `GenEmpz` |
| xF | < 0 | `xF_SIDIS_es` / `xF_SIDIS` / `xF_H1` | `xFGen` |
| z | [0, 1] (optional) | `z_SIDIS_es` / `z_SIDIS` | `zGen` |

Two fiducial tables are computed: one without the z cut (`signal_fiducial_no_z`)
and one with it (`signal_fiducial_with_z`). A baseline table with no
fiducial applied (`no_fiducial`) is also produced for raw event-overlap
reference.

---

## 5. Required Input Branches

The script resolves branch names at runtime via `pick_branch()`, which
returns the first candidate found in the tree's key set. If no candidate
is found the role is `None` and the corresponding cut is silently skipped
(recorded in `branches.missing_roles` in the JSON).

**Mandatory for event-level classification:**

- `RunNumber` and `EventNumber` in both `ChargedTot` and `ChargedTotGen`.
  Without these, event-key packing fails and the script aborts.

**Strongly recommended (absence degrades fiducial accuracy):**

- All reco and gen variable branches listed in Section 4 above.

**Branch-name adaptation points** (where fallback logic is applied):

| Role key | Priority order |
|---|---|
| `q2_reco` | `Q2s`, `Q2es` |
| `y_reco` | `Ys`, `Yes` |
| `xf_reco` | `xF_SIDIS_es`, `xF_SIDIS`, `xF_H1` |
| `z_reco` | `z_SIDIS_es`, `z_SIDIS` |
| `q2_gen` | `Q2sGen`, `Q2_es_gen`, `Q2esGen` |
| `y_gen` | `YsGen`, `Y_es_gen`, `YesGen` |
| `empz_gen` | `GenEmpz` (no fallback; add one if the branch is renamed) |
| `xf_gen` | `xFGen` (no fallback) |
| `z_gen` | `zGen` (no fallback) |

The `z_gen_range` block in the JSON reports how many gen tracks have `zGen`
outside [0, 1] or NaN; large counts here indicate an unphysical fill value
(sentinel −999 or similar) requiring investigation before using the z-cut table.

---

## 6. Running the Script Locally

The script runs locally. No SSH or NAF access is needed provided the ROOT
file is already present on disk.

**Dependencies:** `uproot`, `numpy`, `matplotlib` (standard analysis environment).

**Example command** (using the canonical v6 smoke file from 2026-06-05):

```bash
python3 /Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/diagnostics/quantify_v6_migrations_smoke.py \
  --input /Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/v6_stage1_or_envelope_q2gen60_smoke_2026-06-05/sidistree_v6_djangoh14_7072_em06_0007_smoke.root \
  --outdir /Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/v6_migration_smoke_2026-06-05
```

Both arguments have hard-coded defaults pointing at the above paths; omit
them to rerun on the same canonical smoke file.

**H1 SSH note:** If the ROOT file does not exist locally and must be fetched
from NAF, you must transfer it first using the project's ControlMaster SSH
rules (check `ssh -O check naf-hone.desy.de 2>&1` before any remote
command; see `H1/AGENTS.md` SSH Safety Rules). Do not run the script
remotely or pipe through SSH — the script writes local output files.

---

## 7. Outputs and How to Read Them

Three files are written to `<outdir>/`:

```
v6_migration_smoke_<stem>.json
v6_migration_smoke_<stem>.pdf
v6_migration_smoke_<stem>.png
```

where `<stem>` is the ROOT file stem (e.g.
`sidistree_v6_djangoh14_7072_em06_0007_smoke`).

### 7.1 JSON

The JSON is the machine-readable record of every count. Key fields:

| Field | Meaning |
|---|---|
| `input_exists`, `input_size_bytes` | Basic file-presence check; non-null confirms the file was found |
| `fiducial` | Copy of the cut constants used (useful for auditing future runs) |
| `notes.event_classifier` | Prose description of the any-track approximation |
| `branches.selected` | The branch name actually used for each role (or `null` if absent) |
| `branches.missing_roles` | Roles where no candidate branch was found |
| `trees.ChargedTot.n_track_entries` | Total track-level rows in the reco tree |
| `trees.ChargedTot.n_unique_events` | Unique `(RunNumber, EventNumber)` pairs in the reco tree |
| `trees.ChargedTotGen.n_unique_events` | Unique event pairs in the gen tree |
| `categories.no_fiducial` | Raw event-set overlap before any kinematic cuts |
| `categories.signal_fiducial_no_z` | Overlap with full signal fiducial except z |
| `categories.signal_fiducial_with_z` | Overlap with full signal fiducial including z |
| `categories.per_variable_single_cut` | Per-variable single-cut tables (Q², y, Empz, xF, z independently) |
| `z_gen_range` | Distribution of `zGen` values: in-range, out-of-range, NaN counts |
| `blockers` | Non-empty if the script aborted early (missing trees, missing file) |

Within each category table, look for:

- **`reco_only`** — events that migrate *into* the reco selection from
  outside the gen fiducial. Large values relative to `both` indicate broad
  acceptance migration; expected to grow in v6 where gen geometric cuts
  were removed.
- **`gen_only`** — events that migrate *out* of the gen fiducial at reco
  level. Large values indicate reco inefficiency or resolution effects near
  kinematic boundaries.
- **`both`** — events well-contained in both sides; the stable core of
  the measurement.

The `per_variable_single_cut` tables isolate which kinematic variable
is the dominant driver of each migration type. For example, a large
`Q2.gen_only` count points to Q² resolution smearing near the 150 GeV²
threshold; a large `Empz.reco_only` count would indicate Empz is
the main source of in-migration from the undetected region.

### 7.2 PNG

A single-page two-panel summary figure (symlog y-axis):

- **Top panel:** Grouped bar chart of `reco_only` / `gen_only` / `both`
  event counts for the three fiducial scenarios (no-fiducial, signal-no-z,
  signal-with-z). Colors follow the Okabe-Ito palette.
- **Bottom panel:** Per-variable single-cut bar chart across Q², y, Empz,
  xF, z showing the same three categories for each dimension independently.

Use the PNG for a quick visual pass. A healthy v6 smoke file shows:

- Non-zero `reco_only` counts (expected: v6 removed gen Pt and |η| cuts,
  so the gen net is tighter than the reco acceptance in some regions).
- Non-zero `gen_only` counts (expected: some events resolve below the
  reco Q² threshold or are lost to reco inefficiency).
- `both` is the largest category under the signal fiducial.

### 7.3 PDF

Page 1: same figure as the PNG.
Page 2: monospace text tables with the full branch-selection log, tree
entry counts, z_gen range, and all numeric category counts. Use the PDF
when you need to review the branch mapping alongside the figure.

If the script aborted due to blockers, both PNG and PDF show an
"DIAGNOSTIC ABORTED" message with the blocker list.

---

## 8. Porting to Another Analysis

To adapt the script to a different H1 analysis (e.g. `lambda/`, `proton/`,
`sidis_lowQ2/`), make the following targeted changes:

### 8.1 Tree Names

Replace `"ChargedTot"` and `"ChargedTotGen"` with the tree names in the
target file. Verify with:

```python
import uproot
f = uproot.open("your_file.root")
print(f.keys())
```

Update the two hard-coded tree name strings near line 245–253 and the
`result["trees"]` keys throughout.

### 8.2 Event-Key Branches

The script assumes `RunNumber` and `EventNumber` exist in both trees with
those exact names. If your analysis uses different branch names (e.g.
`run`, `evtno`), update the four entries in the `branches` dict:
`run_reco`, `evt_reco`, `run_gen`, `evt_gen`.

### 8.3 Reco / Gen Branch Mappings

Update the candidate lists in `branches = {...}` to match your analysis
branch naming. Use `pick_branch()` priority lists to support both old and
new naming conventions without breaking on either.

### 8.4 Fiducial Predicate

Update the module-level fiducial constants (`Q2_MIN`, `Y_MIN`, `Y_MAX`,
`EMPZ_MIN`, `EMPZ_MAX`, `XF_MAX`, `Z_MIN`, `Z_MAX`) and both
`per_track_fid_reco()` and `per_track_fid_gen()` functions to reflect
the target analysis cuts. For analyses without an xF variable (e.g.
proton/antiproton), remove or skip that cut rather than forcing a
sentinel — do not fake a branch value.

### 8.5 Output Labels

Update the `DEFAULT_OUTDIR`, the `fig.suptitle(...)` string, and the
output filename prefix (`"v6_migration_smoke_"`) to reflect the target
analysis tag so outputs do not collide with high-Q² SIDIS outputs.

### 8.6 Validation Checklist When Porting

1. **Confirm branch presence.** Run the script once with a real file and
   check `branches.missing_roles` in the JSON. Missing roles silently
   drop the corresponding cut; if a branch is expected but absent it is
   more likely a naming mismatch than a genuine absence.
2. **Check sentinel handling.** Inspect `z_gen_range` (or the equivalent
   field for your variable of interest) for anomalous out-of-range counts.
   A large count of z < 0 entries typically means a −999 sentinel is being
   counted as in-range; add a sentinel filter in the fiducial predicate if
   needed.
3. **Do not fake missing categories.** If a tree is absent (e.g. there is
   no gen tree in a data-only file), let the script exit via the `blockers`
   path rather than constructing dummy counts. The blocker output is
   the correct signal that this file type is not compatible with a
   two-sided migration diagnostic.
4. **Keep the any-track approximation note.** The `notes.event_classifier`
   JSON field should be updated to name the target analysis, but the
   methodological caveat must be retained: the script is a smoke diagnostic,
   not a per-track matched response matrix.

---

## 9. Known Limitations

- **Single-file smoke only.** The script is not designed for batch
  production over many files. Use the standard Condor pipeline for
  full-sample diagnostics.
- **Any-track fiducial.** An event with one passing track and many
  failing tracks is still counted as "in fiducial." In high-multiplicity
  events this can overcount events relative to a strict per-track matched
  matrix.
- **No track-to-track matching.** `reco_only` and `gen_only` counts
  cannot be decomposed into detector-efficiency losses vs pure kinematic
  migration without the per-track matched response matrix.
- **`gen_out → reco_out` not observable.** Events outside both
  fiducials are not present in the selected trees and cannot be counted;
  see Section 2.

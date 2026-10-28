# SIDIS High-Q² Canonical Naming Reference

**Date:** 2026-05-23
**Author:** code-editor (Phase R rename campaign)
**Status:** AUTHORITATIVE. Future agents read this first before doing
any work that creates, renames, or globs SIDIS workspace artifacts.

This page defines the canonical on-disk naming for every category of
SIDIS workspace artifact (outputs, steering, condor, condor_logs,
submit wrappers, smoke gate workdirs). The pre-existing "naming
aliases" document
(`docs/references/reference-naming-aliases-highQ2-sidis.md`) covered
the LEGACY-vs-physics distinctions and remains valid for understanding
why proton-prefixed names existed historically. This document covers
the POST-rename canonical state.

---

## 1. Workspace map (target final state, NAF)

```
H1ChargedSidis_HighQ2/
├── bin/x86_64-el9-gcc14-opt/
│   ├── TreeBuilderHighQ2_chargedsidis          (Stage 1 evls; v1.4 binary md5 d740dbdc...)
│   ├── ChargedSidisTreeBuilderHighQ2           (Stage 2 v5f; retained for audit; md5 d95158bf...)
│   └── ChargedSidisTreeBuilderHighQ2_v5g       (Stage 2 v5g PRODUCTION; md5 091b74ff...)
├── src/                                         (Stage 2 source; SIDIS-local)
├── src_stage1/                                  (Stage 1 source; SIDIS-local since Phase 1A)
├── steering/
│   ├── evls/
│   └── sidistree/                               (was steering/proton; Phase R 2026-05-23)
│       ├── 27p6/<period>/sidistree_<period>_d<NN>_<NNNN>.steer
│       ├── data/{05,06,07}/sidistree_d<NN>_<NNNN>.steer
│       └── mc/<gen>/<cat>/sidistree_<gen>_<cat>_<period>_<NNNN>.steer
├── condor/
│   ├── evls/
│   └── sidistree/                               (was condor/proton; Phase R 2026-05-23)
│       ├── 27p6/{wrappers,subs}/<period>/sidistree_<period>_d<NN>_<NNNN>.{sh,sub}
│       └── {wrappers,subs}/{data,mc}/...
├── condor_workdir/
├── smoke_gate_work/
│   ├── sidistree_eminusp/                       (was proton_eminusp; Phase R)
│   └── sidistree_eplusp/                        (was proton_eplusp; Phase R)
├── output/
│   ├── archive_v5f_pre_v5g_2026-05-23/          (NEW Phase R; immutable audit trail)
│   │   └── data/27p6/{em0405,em06,ep0607}/proton_highQ2_<period>_d<NN>_<NNNN>.root
│   ├── data/
│   │   ├── em0405/sidistree_em0405_d05_<NNNN>.root      (v5g; was data/05/proton_highQ2_d05_*)
│   │   ├── em06/sidistree_em06_d06_<NNNN>.root          (v5g; was data/06/proton_highQ2_d06_*)
│   │   └── ep0607/sidistree_ep0607_d07_<NNNN>.root      (v5g; was data/07/proton_highQ2_d07_*)
│   ├── mc/
│   │   ├── rapgap31/<cat>/
│   │   │   ├── sidistree_mc_rapgap31_<cat>_<period>_<NNNN>.root      (v5f primary; was proton_highQ2_mc_*)
│   │   │   └── sidistree_legacy_rapgap31_<cat>_<period>_<NNNN>.root  (v5b legacy duplicate; was proton_highQ2_rapgap31_*)
│   │   ├── djangoh14/<cat>/sidistree_djangoh14_<cat>_<period>_<NNNN>.root  (was proton_highQ2_djangoh14_*)
│   │   └── djangoh14_norad/{eminusp,eplusp}/sidistree_djangoh14_norad_<charge>_<letter>_<chunk>.root  (was proton_djangoh14_norad_*)
│   └── evls/
│       ├── data/{05,06,07}/evls_<period>_d<NN>_<NNNN>.root          (SIDIS-local Stage 1; was evls_highQ2_d<NN>_*)
│       └── mc/
│           ├── djangoh14/<cat>/evls_djangoh14_<cat>_<period>_<NNNN>.root   (was evls_highQ2_djangoh14_*)
│           ├── rapgap31/<cat>/evls_rapgap31_<cat>_<period>_<NNNN>.root     (was evls_highQ2_rapgap31_*)
│           └── djangoh14_norad/{eminusp,eplusp}/evls_djangoh14_norad_<charge>_<letter>_<chunk>.root
└── scripts/
    ├── submit_full_production.py                (canonical SIDIS-local launcher)
    ├── submit_highQ2_sidis_data.py              (was submit_highQ2_sidis_data_27p6.py; Phase R)
    ├── submit_norad_charged_sidis_highQ2.py
    ├── submit_em06_g15_recovery.py
    ├── submit_sidistree_highQ2_batch.sh         (was submit_proton_highQ2_batch.sh; Phase R)
    ├── extract_yields_v5f_3period.py            (DATA_BASE drops 27p6/)
    ├── extract_phi_yields_v5f_3period.py        (DATA_BASE drops 27p6/)
    ├── submit_phi_yields_v5f_condor.py          (DATA_BASE drops 27p6/)
    ├── submit_phi_gap_diag_condor.py            (DATA_BASE drops 27p6/)
    ├── compute_highQ2_crad_v5f_real.py          (dedupe heuristic handles both new and legacy prefixes)
    ├── submit_crad_extract_condor.py            (same)
    ├── merge_data_highQ2.sh                     (input glob output/data/*/sidistree_*; output merged_sidistree_*)
    ├── merge_mc_highQ2.sh                       (same)
    └── analyze_sidis_tmd.py                     (default --data/--mc paths use merged_sidistree_*)

/data/dust/group/h1/klesthen/BaryonAsym/condor_logs/
├── sidistree_highQ2/                            (was proton_highQ2/; Phase R)
└── sidistree_highQ2_recovery/                   (was proton_highQ2_recovery/; Phase R)
```

The shared workspaces at `H1BaryonAsym_HighQ2/`, `H1BaryonAsym/`, and
`bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2` are OWNED BY V0/proton and
are READ-ONLY from the SIDIS side. SIDIS no longer creates or renames
anything outside `H1ChargedSidis_HighQ2/` and `condor_logs/sidistree_highQ2*/`.

---

## 2. Year-short → period map (data outputs)

For SIDIS-included data periods (ep0304 is PERMANENTLY EXCLUDED):

| year_short (HAT subdir) | period           | example output filename                                   |
|-------------------------|------------------|-----------------------------------------------------------|
| 05                      | em0405           | `output/data/em0405/sidistree_em0405_d05_<NNNN>.root`     |
| 06                      | em06             | `output/data/em06/sidistree_em06_d06_<NNNN>.root`         |
| 07                      | ep0607           | `output/data/ep0607/sidistree_ep0607_d07_<NNNN>.root`     |
| 04                      | ep0304 (EXCLUDED)| not produced; would be `output/data/ep0304/...` if added  |

The year-short → period mapping is deterministic in the SIDIS sample
because the v5g production split year 05 entirely into em0405 (no em06
runs landed in year 05 in this campaign), and year 06 into em06 (em0405
runs are absent from year 06). Year 07 maps cleanly to ep0607. The v5f
legacy data outputs at `output/data/27p6/em06/` mixed `d05` and `d06`
file-tail tokens (em06 period spans both years in v5f) and are
preserved in `output/archive_v5f_pre_v5g_2026-05-23/data/27p6/em06/`
without modification for audit reproducibility.

---

## 3. File prefix conventions

### Data Stage 2 outputs

`sidistree_<period>_d<NN>_<NNNN>.root`

- `<period>`: em0405 / em06 / ep0607 (drives the parent dir too)
- `<NN>`: year-short (05/06/07) — kept for per-year traceability
- `<NNNN>`: zero-padded HAT chunk index

### MC Stage 2 outputs (RAPGAP31 dual-prefix)

For RAPGAP31 catalogs (7071, 7094, 7096, 7097), the v5b campaign produced
TWO files per HAT chunk under two different naming conventions. After
Phase R the on-disk forms are:

- v5f primary: `sidistree_mc_rapgap31_<cat>_<period>_<NNNN>.root`
                (was `proton_highQ2_mc_rapgap31_*`)
- v5b legacy:  `sidistree_legacy_rapgap31_<cat>_<period>_<NNNN>.root`
                (was `proton_highQ2_rapgap31_*`)

The two are NOT byte-identical (ROOT TFile UUIDs/timestamps differ) but
have equivalent entry counts at matching HAT indices. Extractors use a
dedupe heuristic that prefers the `sidistree_mc_` (v5f) naming. Both
heuristics in `compute_highQ2_crad_v5f_real.py` and
`submit_crad_extract_condor.py` accept either the new `sidistree_mc_`
token or the legacy `proton_highQ2_mc_` token (in case a not-yet-renamed
mirror is presented).

### MC Stage 2 outputs (DJANGOH14 single-prefix)

For DJANGOH14 catalogs (7072, 7091, 7092 — and 7093 for ep0304-EXCLUDED):

- `sidistree_djangoh14_<cat>_<period>_<NNNN>.root`
  (was `proton_highQ2_djangoh14_*`; for 7093 see below)

The DJANGOH catalog 7093 (ep0304-EXCLUDED) has files with the older
`proton_highQ2_mc_djangoh14_*` prefix; these are renamed to
`sidistree_mc_djangoh14_*` for consistency but are not consumed by any
3-period extractor.

### MC NORAD outputs

`sidistree_djangoh14_norad_<charge>_<letter>_<chunk>.root`

- `<charge>`: eminusp / eplusp
- `<letter>`: Britzger NORAD generator-letter token (A/B/C/D/...)
- `<chunk>`: C-prefixed chunk index (e.g. C0000037)

### Stage 1 evls outputs (SIDIS-local)

Per the data Stage 1 outputs:
`output/evls/data/<year_short>/evls_<period>_d<NN>_<NNNN>.root`
(was `evls_highQ2_d<NN>_<NNNN>.root`; the new prefix carries the
period tag inline).

Per the MC RAD evls:
`output/evls/mc/<gen>/<cat>/evls_<gen>_<cat>_<period>_<NNNN>.root`
(was `evls_highQ2_<gen>_<cat>_<period>_<NNNN>.root`).

Per the MC NORAD evls (SIDIS-local since Phase 1A-fix):
`output/evls/mc/djangoh14_norad/<charge>/evls_djangoh14_norad_<charge>_<letter>_<chunk>.root`

### Steering & condor wrapper filenames

`sidistree_<...>.{steer,sh,sub}` — exact same root-name as the produced
output ROOT, with the corresponding extension.

### Merged files (downstream)

`output/merged/merged_sidistree_highQ2_<class>_<sample>.root`

(was `merged_proton_highQ2_*`). Default args of `analyze_sidis_tmd.py`
have been updated.

---

## 4. What was NOT renamed (KEEP_AS_AUDIT)

The following references are physics terminology or audit history and
were INTENTIONALLY preserved:

### Physics terminology

- `M_PROTON`, `M_p`, "proton fraction", "proton excess", "NuclIA
  proton-omission" — all reference the SPECIES proton, not the
  workspace.
- `27p6` as a beam-energy descriptor (HERA 27.6 GeV electron beam)
  in:
  - `submit_highQ2_sidis_data.py`: `beam_filter: str = "27p6"`,
    `PERIODS_PURE_27P6`, `--ep0304-beam-filter` CLI choices,
    `filter_ep0304_27p6()` function name.
  - `n_dis_data_v5f_highQ2.py`: `IsNom27p6`, `IsNominalEnergy_27p6_920`
    (CSV column names from the externally-fixed beam-energy table).
  - Steering header comment `// Period: em06  Beam-filter: 27p6` in
    `submit_em06_g15_recovery.py`.
- `output/archive_v5f_pre_v5g_2026-05-23/data/27p6/` — the archive
  preserves the legacy on-disk layout intact (audit trail).

### Cross-workspace references

- `H1/proton_HiQ/` and `H1BaryonAsym_HighQ2/`: V0/proton workspace
  paths. These are READ-ONLY from SIDIS and their names are correct
  as-is.
- "ProtonTreeBuilderHighQ2" mentions in audit history comments where
  the context says "historical / pre-v5f".

### Audit-trail filenames in docs/data/

- `g1c_27p6_data_production_plan.json` — immutable G1C campaign audit
  artifact; the `27p6` token reflects the beam-energy descriptor.
- `g1c_27p6_data_production_jobs.json` — same.
- `submission_audit_*_*.json` — Guardrail 5 forensics; immutable.

### Internal `27p6/` subdir in steering and condor

The `steering/sidistree/27p6/<period>/` and
`condor/sidistree/27p6/{wrappers,subs}/<period>/` subdirs preserve the
internal layout from the legacy `steering/proton/27p6/<period>/` tree
per blocker resolution 4 (minimum-disruption rename). This subdir was
kept because:

1. The legacy steering files inside reference an exact path layout
   that downstream condor wrappers consume by absolute path; flattening
   it would break in-flight condor jobs (none were in flight during
   Phase R but the safety margin is preserved).
2. The brief explicitly authorized this minimum-disruption variant.

The OUTPUT-path partition `27p6/` IS dropped: data outputs sit at
`output/data/<period>/` not `output/data/27p6/<period>/`. Only the
steering and condor internal layout retains the literal `27p6/`
intermediate dir.

---

## 5. Compatibility / migration notes

### For extractors

`compute_highQ2_crad_v5f_real.py`, `submit_crad_extract_condor.py`,
`extract_yields_v5f_3period.py`, `extract_phi_yields_v5f_3period.py`,
`submit_phi_yields_v5f_condor.py`, `submit_phi_gap_diag_condor.py`:
all use `output/data/<period>/*.root` globs (no prefix lock-in) so they
work against both pre- and post-rename file pools. The dedupe heuristic
in the crad scripts handles both legacy and new prefixes.

### For new code

Use the canonical naming above. Reference `sidistree_*` prefixes; use
`output/data/<period>/` (no `27p6/` partition); use `condor_logs/sidistree_highQ2/`.

### For old code that still references the legacy names

If you find a script that references `proton_highQ2_*`, `output/data/27p6/`,
`steering/proton/`, `condor/proton/`, or `condor_logs/proton_highQ2/` in an
operational (not audit/physics) context, update it per the conventions
above and re-run `py_compile` to confirm. Use the audit memo at
`docs/methodology/rename_audit_2026-05-23/00_PRE_EXECUTION_SCOPE_REPORT.md`
for the categorization of legitimate vs operational references.

---

## 6. Binary identity manifest (post-rename, v1.6)

`scripts/binary_identity_manifest.json` is at version 1.6 as of Phase R.
Binary md5/size/source-md5 for all 4 pinned entries are UNCHANGED from
v1.5 (the Phase R rename is metadata-only). Guardrail self-test
verified PASS on all 4 binaries against v1.6 on 2026-05-23T22:30Z:

| binary_key                            | expected_md5                          | PASS |
|---------------------------------------|---------------------------------------|------|
| TreeBuilderHighQ2_chargedsidis        | d740dbdc44e11d4fc4001fe561adab74      | ✓    |
| ChargedSidisTreeBuilderHighQ2 (v5f)   | d95158bf537331c2713f85c5f9d8f84b      | ✓    |
| ChargedSidisTreeBuilderHighQ2_v5g     | 091b74ff22942f6e0362f4b17a99ac63      | ✓    |
| TreeBuilder (low-Q² SpaCal; FORBIDDEN)| 8de736bc2b8995b1a8428784a7cf3b95      | ✓    |

V0/proton workspace md5s verified UNCHANGED post-Phase-R:

| path                                                        | md5                                  |
|-------------------------------------------------------------|--------------------------------------|
| bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2 (shared)         | c2cf0828d4155de64ea9586cf3004426     |
| H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C                 | c6653628e198aff5c23224fc5e1b0d0d     |
| H1BaryonAsym_HighQ2/src/PhysicsConstants.h                  | af7dae3eb870f039a9cad363d959d758     |

---

## 7. References

- Pre-execution scope report:
  `docs/methodology/rename_audit_2026-05-23/00_PRE_EXECUTION_SCOPE_REPORT.md`
- Per-operation NAF manifest:
  `docs/methodology/rename_audit_2026-05-23/01_NAF_OPERATIONS_MANIFEST.json`
- Phase R section in structural-separation memo:
  `docs/methodology/structural_separation_2026-05-23.md` Section 16
- Naming-aliases (legacy historical context):
  `docs/references/reference-naming-aliases-highQ2-sidis.md`
- Cross-analysis code locations:
  `docs/methodology/cross_analysis_code_locations_2026-05-23.md`
- Binary identity manifest (v1.6):
  `scripts/binary_identity_manifest.json`

— end of canonical naming reference —

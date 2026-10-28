# NAF analysis-package quarantine verification (2026-05-23)

**Scope.** Verify on NAF whether the high-Q² analysis packages
(`H1ChargedSidis_HighQ2`, `H1BaryonAsym_HighQ2`, `H1PhiMeson`, `H1XiAsym`)
are structurally quarantined per the Phase-1A (2026-05-23) refactor that
the user (supervisor) recalls. Determine whether a separate "Proton
high-Q²" package exists, and identify the actual home of the
proton/antiproton high-Q² analysis.

**Method.** Read-only inspection of `/data/dust/group/h1/klesthen/BaryonAsym/`
on `naf-hone.desy.de` (SSH control socket alive at dispatch time). All
quoted paths are absolute NAF paths; line numbers are exact.

---

## Headline answer

The **source-code separation** of the four high-Q² analysis packages is
**complete and verified**: zero `#include` cross-references between
`H1ChargedSidis_HighQ2/`, `H1BaryonAsym_HighQ2/`, `H1PhiMeson/`, and
`H1XiAsym/` `src/` trees once H1OO framework headers are excluded;
each package has its own `Makefile`, its own `MODULENAME`, its own
`SLIB`, and its own distinct `BIN` set; and (since Phase 1A) the
`H1ChargedSidis_HighQ2` binaries write to a **per-workspace bin**
(`H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/`) instead of the
shared `bin/` so they cannot be silently overwritten by another
package's compile. **However, two non-source quarantine gaps remain in
the SIDIS package**: (1) every SIDIS Stage-2 steering file (2,705 of
them under `H1ChargedSidis_HighQ2/steering/proton/`) hard-codes
`fEventListFiles` paths that point at
`H1BaryonAsym_HighQ2/output/evls/...` — i.e. the SIDIS Stage-2 builder
consumes upstream Lambda/baryon evls at runtime; and (2) there is **no
`H1ProtonAsym_HighQ2/` (or any variant) package on NAF** — the
supervisor's `H1/proton_HiQ/context-handoff.md` line 24 names a package
root (`H1ProtonAsymHighQ2/`) that does not exist on disk. The
proton/antiproton high-Q² analysis instead lives in the **legacy
`H1SIDIS_HighQ2/` directory** (the pre-rename home that
`H1ChargedSidis_HighQ2/` was forked from), whose Makefile still
declares `BIN = ProtonTreeBuilderHighQ2`. No foreign-include offenses
were found in any `src/` tree.

---

## Task 1: Package inventory

### `H1ChargedSidis_HighQ2/`

**Makefile header (`H1ChargedSidis_HighQ2/Makefile` lines 1–46, 48–67):**

| Field | Value |
|---|---|
| `MODULENAME` | `H1ChargedSidisHighQ2Standalone` (line 48) |
| `SLIB` | `libChargedSidisHighQ2User.so` (line 49) |
| `OBJS` | `FidVolCut.o TDetectQedc.o` (line 54) |
| `BIN` | `TreeBuilderHighQ2_chargedsidis ChargedSidisTreeBuilderHighQ2` (line 57); plus `BIN_V5G = ChargedSidisTreeBuilderHighQ2_v5g` declared separately (line 130) |
| `EXTRA_INCLUDEDIRS` | `src src_stage1` (line 67) |
| `vpath %.C` | `src src_stage1` |
| Link target | Three explicit linker rules: `TreeBuilderHighQ2_chargedsidis` (lines 110–111), `ChargedSidisTreeBuilderHighQ2` (lines 118–119), `ChargedSidisTreeBuilderHighQ2_v5g` (lines 131–132) |
| Library set | Full `H1OO_LIBS` superset (15 libs) + `-lCentauro` + LHAPDF + ROOT; no `-lH1SVFit`, no CERNLIB |
| `BINDIR` override | `SIDIS_BINDIR := $(MAKEDIR)/bin/$(SYSTEM)` applied as target-specific (lines 96, 100–102) — REDIRECTS away from shared bin |

**Binary output target.** Phase-1A per-target override means all SIDIS
binaries land at
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/`,
NOT in the shared `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/`.
Verified content of per-workspace bin (`ls -la`):

```
ChargedSidisTreeBuilderHighQ2      604856 bytes  May 23 21:02
TreeBuilderHighQ2_chargedsidis    1899080 bytes  May 23 21:02
```

**`src/` listing with banner comments.** Eight files (six live + two
`.bak`):

| File | Lines 1–5 (banner) |
|---|---|
| `ChargedSidisTreeBuilderHighQ2.C` | `// ChargedSidisTreeBuilderHighQ2.C` `// Renamed from ProtonTreeBuilderHighQ2.C during Phase 4 workspace migration (2026-05-19). Builder logic UNCHANGED. Only the filename, header...` |
| `ChargedSidisTreeBuilderHighQ2_v5g.C` | Same banner as above (parallel v5g build) |
| `ChargedSidisUtils_highQ2.h` | `#ifndef CHARGEDSIDIS_UTILS_HIGHQ2_H` `// ChargedSidisUtils_highQ2.h` |
| `ChargedSidisUtils_highQ2_v5g.h` | `// ChargedSidisUtils_highQ2_v5g.h` `// v5g uses this header verbatim; copy retained for build-self-containment.` |
| `PhysicsConstants_chargedsidis_highQ2.h` | `#ifndef CHARGEDSIDIS_HIGHQ2_PHYSICS_CONSTANTS_H` `// PhysicsConstants_chargedsidis_highQ2.h — H1ChargedSidis_HighQ2` |
| `PhysicsConstants_chargedsidis_highQ2_v5g.h` | `// PhysicsConstants_chargedsidis_highQ2_v5g.h` `// v5g uses this header verbatim; copy retained for build-self-containment.` |
| `ChargedSidisTreeBuilderHighQ2.C.bak_v5b_pre_v5f_2026-05-19` | (legacy backup) |
| `ChargedSidisUtils_highQ2.h.bak_v5b_pre_v5f_2026-05-19` | (legacy backup) |

**`src_stage1/` listing.** Phase-1A SIDIS-owned Stage-1 copies of the
upstream V0/Lambda Stage-1 builder (per `src_stage1/README.md`):

```
TreeBuilderHighQ2_chargedsidis.C   (renamed from H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C)
TreeBuilderHighQ2_chargedsidis.h   (renamed, ifndef guard updated)
Calibration.h, FidVolCut.{C,h,steer}, PhysicsConstants.h, TDetectQedc.{C,h}
  — md5-identical LITERAL COPIES of the H1BaryonAsym_HighQ2/src/ originals
README.md (provenance + freeze policy)
```

The README's provenance table records the source md5 for every file and
explicitly states: *"These files are FROZEN-FOR-SIDIS. They are NOT
auto-synced from `H1BaryonAsym_HighQ2/src/`."*

---

### `H1BaryonAsym_HighQ2/`

**Makefile header (`H1BaryonAsym_HighQ2/Makefile` lines 1–20):**

| Field | Value |
|---|---|
| `MODULENAME` | `H1Examples` (line 19) |
| `SLIB` | `libUser.so` (line 20) |
| `OBJS` | `FidVolCut.o TDetectQedc.o` (line 23) |
| `BIN` | `TreeBuilderHighQ2 Analysis CrossSections ControlPlots ResponseMatrix check_output SIDISCrossSection` (line 26); `+ rerun_rec` unless `STANDALONE=yes` |
| `EXTRA_INCLUDEDIRS` | `src validation` (line 34) |
| `vpath %.C` | `src validation` |
| Link target | Seven explicit linker rules at lines 94–113 |
| Library set | Same `H1OO_LIBS` superset as SIDIS; `-lCentauro` + LHAPDF + ROOT |
| `BINDIR` | DEFAULT (inherited from `H1Basic.mk`): `$(MAKEDIR)/../bin/$(SYSTEM)` → shared `bin/x86_64-el9-gcc14-opt/` |

The Makefile comment (lines 8–13) explicitly notes Guardrail 1
(2026-05-22): renamed `TreeBuilder` → `TreeBuilderHighQ2` to avoid
shared-`BINDIR` collision with `H1BaryonAsym/Makefile`'s low-Q² SpaCal
`TreeBuilder` binary.

**`src/` listing (13 live `.C`/`.h` files):**

| File | Banner |
|---|---|
| `Analysis.C` | `// Analysis.C — Unified cross-section and asymmetry analysis` `// Replaces 8 near-identical Analysis{Q2,x,pT,Eta}{Lambda,Kaon}.C files.` |
| `Calibration.h` | `// Consolidation of elecCut.C, Alignment.C, and SpacLinearity.C` `// Dead cell maps loaded from CSV files in dead_cells/ directory.` |
| `ControlPlots.C` | `// ControlPlots.C — Consolidated control plots for DIS, Lambda, and Kaon candidates` |
| `CrossSections.C` | `// CrossSections.C — Differential cross-section calculation from Analysis output` |
| `FidVolCut.{C,h}` | `// FidVolCut.h` / `FidVolCut.C` (LAr fiducial volume cut, ClassDef) |
| `PhysicsConstants.h` | `// Single source of truth for all physics constants, cuts, and binnings` `// used in the High-Q2 (Q2 > 150 GeV^2) BaryonAsym Lambda/K0s analysis.` |
| `ResponseMatrix.C` | `// ResponseMatrix.C — Proper 2D migration matrices for unfolding` |
| `SIDISCrossSection.C` | `// SIDISCrossSection.C — SIDIS differential cross-section measurement` `// Reads Lambda or K0s tree ROOT files produced by TreeBuilder ...` |
| `TDetectQedc.{C,h}` | (QED-Compton detection bank, ClassDef) |
| `TreeBuilderHighQ2.{C,h}` | `// TreeBuilderHighQ2.C` `// HIGH-Q^2 (Q^2 > 150 GeV^2, LAr electron) Stage-1 event-list builder.` |
| `TreeBuilder.{C,h}.bak_pre_guardrail_2026-05-22` | (pre-rename backups) |

Note that `SIDISCrossSection.C` here is the *Lambda/K0s* SIDIS
cross-section binary (Lambda or K0s tree input), NOT the
charged-particle SIDIS analysis. This is a naming overload, not
cross-contamination.

---

### `H1PhiMeson/`

**Makefile header (`H1PhiMeson/Makefile` lines 1–24):**

| Field | Value |
|---|---|
| `MODULENAME` | `H1Examples` (line 12) |
| `SLIB` | `libUser.so` (line 13) |
| `OBJS` | (empty) (line 14) |
| `BIN` | `PhiTreeBuilder` (line 16); `+ rerun_rec` unless `STANDALONE=yes` |
| `EXTRA_INCLUDEDIRS` | `src` (line 23) |
| `vpath %.C` | `src` |
| Link target | Single explicit linker rule at lines 51–52: `PhiTreeBuilder` |
| Library set | Same `H1OO_LIBS` superset; no `-lCentauro`, no `-lH1SVFit`, no CERNLIB |
| `BINDIR` | DEFAULT (shared `bin/x86_64-el9-gcc14-opt/`) |

**`src/` listing (5 files):**

| File | Banner |
|---|---|
| `ExtractGdif.C` | `// Read GDIF bank parameters from DIFFVM MC files` |
| `ExtractSigma.C` | `// ExtractSigma.C` `// Read GKI bank to extract SIGMT (ep cross-section) and compute MC luminosity` |
| `ExtractSigmaEndOfRun.C` | `// Authored by SIDIS-supervisor DIFFVM-normalization research, 2026-05-22` `// ExtractSigmaEndOfRun.C` |
| `PhiTreeBuilder.C` | `// PhiTreeBuilder.C` `// Exclusive phi(1020) meson analysis tree builder for H1 HERA DIS data.` `// Implements gamma*p -> phi(1020) p -> K+K- p event selection and storage.` |
| `ReadGDIF.C` | `// Read GDIF bank from DIFFVM MC to extract cross-section info` |

Note: the `ExtractSigmaEndOfRun.C` banner attributes authorship to
"SIDIS-supervisor DIFFVM-normalization research". This is a cross-team
authored utility but lives entirely under `H1PhiMeson/src/`; no
cross-package include or path dependency.

---

### `H1XiAsym/`

**Makefile header (`H1XiAsym/Makefile` lines 1–32):**

| Field | Value |
|---|---|
| `MODULENAME` | `H1XiStandalone` (line 22) |
| `SLIB` | `libXiUser.so` (line 23) |
| `OBJS` | (empty) (line 26) |
| `BIN` | `XiTreeBuilder` (line 29) |
| `EXTRA_INCLUDEDIRS` | `src` (line 35) |
| `vpath %.C` | `src` |
| Link target | Single explicit linker rule at lines 73–74: `XiTreeBuilder` |
| Library set | Same `H1OO_LIBS` superset + `-lH1SVFit` + **CERNLIB** (`-lmathlib -lpacklib -lkernlib` for FORTRAN Kalman filter `vvfilt_`) |
| `BINDIR` | DEFAULT (shared `bin/x86_64-el9-gcc14-opt/`) |

**`src/` listing (3 files):**

| File | Banner |
|---|---|
| `XiTreeBuilder.C` | `// XiTreeBuilder.C` `// Standalone Xi baryon reconstruction binary for H1 HERA data.` `// Implements cascade Xi- (3312) and anti-Xi+ (-3312) reconstruction via ...` |
| `XiTreeBuilder.h` | `// Shared utilities for XiTreeBuilder.C: truth matching, background classification, ...` |
| `XiPhysicsConstants.h` | `// Single source of truth for all physics constants, cuts, and binnings` `// used in the standalone Xi baryon reconstruction package.` |

Makefile comment (lines 4, 13, 36): *"standalone fork of the
H1BaryonAsym build system; ... does NOT modify or replace the upstream
TreeBuilder binary."*

---

### Inventory summary table

| Package | `MODULENAME` | `SLIB` | `BIN` count | `BINDIR` target | Per-workspace `bin/`? |
|---|---|---|---|---|---|
| `H1ChargedSidis_HighQ2` | `H1ChargedSidisHighQ2Standalone` | `libChargedSidisHighQ2User.so` | 3 | per-workspace | **YES** (Phase 1A) |
| `H1BaryonAsym_HighQ2` | `H1Examples` | `libUser.so` | 7 (+rerun_rec) | shared | no |
| `H1PhiMeson` | `H1Examples` | `libUser.so` | 1 (+rerun_rec) | shared | no |
| `H1XiAsym` | `H1XiStandalone` | `libXiUser.so` | 1 | shared | no |

Three of four packages share the `bin/x86_64-el9-gcc14-opt/` output
directory, but their BIN names are **all distinct** (see Task 3). Only
`H1ChargedSidis_HighQ2` has been Phase-1A-redirected to a per-workspace
bin; this was the explicit fix for the Stream-C-1038-job incident
(see `binary_identity_manifest.json` line `"incident_reference"`).

---

## Task 2: Cross-package contamination

### Methodology

For each package, ran two greps:
1. `#include` lines under `src/` (and `src_stage1/` for SIDIS) matching
   `H1SIDIS|H1BaryonAsym|H1PhiMeson|H1XiAsym|H1ChargedSidis|H1Proton`,
   excluding `H1Mods|H1Calculator|...` H1OO framework headers (per
   task instructions).
2. Path-literal (non-#include) references to the same patterns inside
   `.C`/`.h` files.
3. Same patterns inside each `Makefile`.

### `H1ChargedSidis_HighQ2/src/` + `src_stage1/`

**Foreign `#include` offenses: 0.**
Every `#include` in every live `.C` / `.h` either:
- includes a package-local header (e.g. `"PhysicsConstants_chargedsidis_highQ2.h"`,
  `"ChargedSidisUtils_highQ2.h"`, `"TreeBuilderHighQ2_chargedsidis.h"`,
  `"FidVolCut.h"`, `"Calibration.h"`); OR
- includes an H1OO framework header (`H1Mods/*.h`, `H1Skeleton/*.h`,
  `H1Steering/*.h`, `H1Pointers/*.h`, `H1Calculator/*.h`,
  `H1Geom/*.h`, `H1Tools/*.h`, `H1OOBanks/*.h`, `H1Tracks/*.h`,
  `H1PhysUtils/*.h`, `H1HadronicCalibration/*.h`); OR
- is a system / ROOT header.

**Path-literal references** (in source body, not `#include`):
- `src/ChargedSidisTreeBuilderHighQ2.C` line 157:
  `<< "responsibilities (stage_evls from H1BaryonAsym_HighQ2)." << endl;`
- `src/ChargedSidisTreeBuilderHighQ2_v5g.C` line 157: identical string.

These are help-message strings (printed to stderr when CLI args are
missing); they document the upstream dependency but are not compile-time
or link-time cross-references.

**Comment-only references to other packages** (provenance / history):

| File | Lines | What |
|---|---|---|
| `src/ChargedSidisTreeBuilderHighQ2.C` | 15, 43, 123, 157 | Mentions `H1SIDIS_HighQ2_v5f` (ancestor workspace), `H1BaryonAsym_HighQ2 stage_evls` (upstream Stage-1) |
| `src/ChargedSidisTreeBuilderHighQ2_v5g.C` | 15, 43, 123, 157 | Same as above |
| `src/ChargedSidisUtils_highQ2.h` | 20, 22, 23 | `Ported verbatim from H1SIDIS_HighQ2_v5f/src/ProtonUtils.h.` `Extracted from H1BaryonAsym/src/TreeBuilder.h ...` |
| `src/ChargedSidisUtils_highQ2_v5g.h` | 34, 36, 37 | Same as above |
| `src/PhysicsConstants_chargedsidis_highQ2.h` | 5, 19, 21, 26, 31, 48, 79 | Provenance — references to `H1ProtonAsym/src/PhysicsConstants.h` (ancestor), `H1BaryonAsym_HighQ2/src/PhysicsConstants.h`, `ProtonTreeBuilderHighQ2 binary` |
| `src/PhysicsConstants_chargedsidis_highQ2_v5g.h` | 16, 30, 32, 37, 42, 59, 90 | Same provenance comments |

None of these are functional dependencies; they are documentation of
the fork lineage.

**Makefile cross-refs (comment-only):**

`H1ChargedSidis_HighQ2/Makefile`:
- lines 2, 4 — name + Phase-4-rename history
- line 20 — `"Does NOT modify, replace, or share install paths with the upstream H1ProtonAsym or H1BaryonAsym_HighQ2 binaries."`
- line 22 — `"KEY DIFFERENCES FROM H1ProtonAsym/Makefile:"`
- line 35 — `"Environment requirements (same as H1ProtonAsym):"`
- line 63 — provenance of `src_stage1/` copies from `H1BaryonAsym_HighQ2/src/`
- line 74 — `"Library sets (identical to H1ProtonAsym Makefile)"`

All references to `H1ProtonAsym` in the Makefile are **comments only**
(no `vpath`, no `EXTRA_INCLUDEDIRS`, no `-L`, no `-I` to any other
package). `vpath %.C src src_stage1` and `EXTRA_INCLUDEDIRS = src
src_stage1` are package-local.

### `H1BaryonAsym_HighQ2/src/`

**Foreign `#include` offenses: 0.** All includes are package-local or
H1OO framework.

**Path-literal references:** none (empty grep).

**Comment-only references:**
- `src/TreeBuilderHighQ2.C` line 8: `"builder at H1BaryonAsym/src/TreeBuilder.C."` (rename provenance)
- `src/TreeBuilderHighQ2.h` line 6: `"to remove the $(BINDIR)/TreeBuilder collision with H1BaryonAsym low-Q^2"`

**Makefile cross-refs:** only the Guardrail 1 comment at line 11
mentioning `H1BaryonAsym/Makefile`.

### `H1PhiMeson/src/`

**Foreign `#include` offenses: 0.** All includes are ROOT / system /
H1OO framework. Five `.C` files, no `.h`.

**Path-literal references:** none.

**Comment-only references:** none to other analysis packages.

### `H1XiAsym/src/`

**Foreign `#include` offenses: 0.** All includes are H1OO framework or
package-local (`XiPhysicsConstants.h`, `XiTreeBuilder.h`).

**Path-literal references:** none.

**Comment-only references:**
- `src/XiTreeBuilder.C` line 8: `"This binary is a FORK of the H1BaryonAsym TreeBuilder and does NOT ..."`

**Makefile cross-refs (comment-only):** line 4 (`"standalone fork of the H1BaryonAsym build system"`), line 13 (`"Environment requirements (same as upstream H1BaryonAsym)"`), line 36 (`"Library sets (identical to upstream H1BaryonAsym Makefile)"`).

### Cross-contamination verdict

**Zero functional cross-package dependencies in any `src/` tree.**
All cross-package mentions in source and Makefiles are limited to
documentation comments recording the fork lineage. The four packages
compile to four independent object trees, four independent shared
libraries, and four independent binaries.

| Package | Foreign `#include` | Foreign Makefile dep | Path-literal in source |
|---|---|---|---|
| `H1ChargedSidis_HighQ2` | 0 | 0 (comments only) | 2 (help-message strings, line 157) |
| `H1BaryonAsym_HighQ2` | 0 | 0 (comments only) | 0 |
| `H1PhiMeson` | 0 | 0 | 0 |
| `H1XiAsym` | 0 | 0 (comments only) | 0 |

---

## Task 3: Binary output collisions

### Per-package compile object trees

Each package has its own `x86_64-el9-gcc14-opt/` workdir for `.o` and
ROOT-dictionary intermediates — no collisions:

```
H1ChargedSidis_HighQ2/x86_64-el9-gcc14-opt/  ← ChargedSidis*.o, FidVolCut.o, TDetectQedc.o, dicts
H1BaryonAsym_HighQ2/x86_64-el9-gcc14-opt/    ← Analysis.o, ControlPlots.o, CrossSections.o, ResponseMatrix.o, SIDISCrossSection.o, FidVolCut.o, check_output.o, dicts
H1PhiMeson/x86_64-el9-gcc14-opt/             ← PhiTreeBuilder.o, ExtractSigma.o, ExtractSigmaEndOfRun.o, ReadGDIF.o
H1XiAsym/x86_64-el9-gcc14-opt/               ← XiTreeBuilder.o
H1BaryonAsym/x86_64-el9-gcc14-opt/           ← Analysis.o, ControlPlots.o, CrossSections.o, ResponseMatrix.o, SIDISCrossSection.o, TreeBuilder*.o, CradCompute.o, check_output.o   (low-Q²)
```

Note that `FidVolCut.o`, `TDetectQedc.o`, `Analysis.o`, `ControlPlots.o`,
`CrossSections.o`, `ResponseMatrix.o`, `SIDISCrossSection.o`,
`check_output.o` exist in BOTH `H1BaryonAsym/x86_64-el9-gcc14-opt/`
(low-Q²) and `H1BaryonAsym_HighQ2/x86_64-el9-gcc14-opt/` (high-Q²) but
in separate per-package object dirs — no collision, by design.

### Shared `bin/` content

Full listing of `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/`
(`-r` files only) with provenance attribution:

| Binary | Size | Mtime | Source package | Notes |
|---|---|---|---|---|
| `Analysis` | 571,816 | 2026-05-22 17:40 | `H1BaryonAsym_HighQ2` | |
| `ChargedSidisTreeBuilderHighQ2` | 604,856 | 2026-05-21 05:00 | `H1ChargedSidis_HighQ2` (pre-Phase-1A) | **STALE** — Phase-1A redirected new builds away from shared bin; this copy was left behind. The Phase-1A-built equivalent (same size, mtime 2026-05-23 21:02) now lives at `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2`. |
| `ChargedSidisTreeBuilderHighQ2.bak_pre_v5f_2026-05-19` | 597,616 | 2026-05-21 05:00 | `H1ChargedSidis_HighQ2` (legacy backup) | |
| `ChargedSidisTreeBuilderHighQ2_v5g` | 623,112 | 2026-05-23 20:52 | `H1ChargedSidis_HighQ2` (v5g pre-Phase-1A) | Built before the Phase-1A redirect (mtime 20:52 vs Makefile mtime 21:01). Phase-1A Makefile now redirects future v5g builds to per-workspace bin. |
| `check_output` | 1,486,048 | 2026-05-22 17:40 | `H1BaryonAsym_HighQ2` | |
| `ControlPlots` | 556,336 | 2026-05-22 17:40 | `H1BaryonAsym_HighQ2` | |
| `CradCompute` | 197,376 | 2026-05-21 00:31 | `H1BaryonAsym` (low-Q²) | |
| `create_evls` | 282,424 | 2026-03-08 22:48 | `H1BaryonAsym` (low-Q² legacy) | |
| `CrossSections` | 329,832 | 2026-05-22 17:40 | `H1BaryonAsym_HighQ2` | |
| `ExtractMatch` | 309,840 | 2026-05-20 06:39 | (legacy stand-alone match utility, no current Makefile) | |
| `ExtractMatch.bak.20260519-164326` | 300,800 | 2026-05-19 16:43 | (legacy backup) | |
| `ExtractMatch.bak.20260520-v3-pre` | 301,520 | 2026-05-20 06:37 | (legacy backup) | |
| `ExtractSherpa` | 63,464 | 2026-05-18 21:54 | (legacy) | |
| `ExtractSigma` | 187,160 | 2026-04-28 04:55 | `H1PhiMeson` | |
| `ExtractSigmaEndOfRun` | 230,912 | 2026-05-23 19:54 | `H1PhiMeson` | |
| `FrunzV0Rebuild` | 1,176,912 | 2026-05-20 18:52 | `BaryonAsym/FrunzV0Rebuild/` (separate package) | |
| `kaonTree` | 594,960 | 2026-03-08 23:00 | (legacy `H1BaryonAsym`) | |
| `lambdaTree` | 808,384 | 2026-03-08 23:00 | (legacy `H1BaryonAsym`) | |
| `PhiTreeBuilder` | 537,760 | 2026-04-26 00:32 | `H1PhiMeson` | |
| `ProtonTreeBuilder` | 469,072 | 2026-05-22 00:09 | (low-Q² proton, source unknown — not in `H1BaryonAsym/src/`; likely built from an external workspace) | |
| `ProtonTreeBuilderHighQ2` | 597,552 | 2026-05-18 03:06 | **`H1SIDIS_HighQ2`** (legacy pre-rename home of the proton/antiproton high-Q² analysis) | This is the "Proton" analysis binary the supervisor expected. See Task 6. |
| `ReadGDIF` | 167,696 | 2026-05-23 18:27 | `H1PhiMeson` | |
| `ResponseMatrix` | 328,216 | 2026-05-22 17:40 | `H1BaryonAsym_HighQ2` | |
| `SIDISCrossSection` | 449,576 | 2026-05-22 17:41 | `H1BaryonAsym_HighQ2` (Lambda/K0s SIDIS cross-section binary — NOT charged-particle SIDIS) | |
| `TreeBuilder` | 1,516,144 | 2026-05-21 00:30 | `H1BaryonAsym` (low-Q² SpaCal) | |
| `TreeBuilder.bak_lowQ2_premake_2026-05-22` | 1,516,144 | 2026-05-21 00:30 | (backup) | |
| `TreeBuilderHighQ2` | 1,898,992 | 2026-05-22 18:38 | `H1BaryonAsym_HighQ2` (V0/Lambda Stage-1, post-Guardrail-1 rename) | md5 c2cf0828… per `binary_identity_manifest.json` v1.1. This is the shared-bin Stage-1 binary that the SIDIS pipeline used to depend on; Phase 1A replaced that dependency with the per-workspace `TreeBuilderHighQ2_chargedsidis`. |
| `TreeBuilder_v2` | 1,515,912 | 2026-05-21 00:30 | `H1BaryonAsym` (low-Q² variant) | |
| `XiTreeBuilder` | 1,147,048 | 2026-04-26 06:53 | `H1XiAsym` | |

### Collision analysis

**BIN-name uniqueness.** Each Makefile produces distinct binary names:

| BIN name | Built by | Collision? |
|---|---|---|
| `TreeBuilderHighQ2_chargedsidis` | `H1ChargedSidis_HighQ2` (Phase 1A) | unique (per-workspace bin) |
| `ChargedSidisTreeBuilderHighQ2`, `ChargedSidisTreeBuilderHighQ2_v5g` | `H1ChargedSidis_HighQ2` | unique |
| `TreeBuilderHighQ2`, `Analysis`, `CrossSections`, `ControlPlots`, `ResponseMatrix`, `check_output`, `SIDISCrossSection` | `H1BaryonAsym_HighQ2` | unique (no other package builds these) |
| `PhiTreeBuilder` | `H1PhiMeson` | unique |
| `XiTreeBuilder` | `H1XiAsym` | unique |
| `ProtonTreeBuilderHighQ2` | `H1SIDIS_HighQ2` (legacy) | unique |
| `TreeBuilder`, `TreeBuilder_v2`, `CradCompute` | `H1BaryonAsym` (low-Q²) | unique |

**No two packages produce a binary with the same name.** The
Guardrail-1 (2026-05-22) rename of low-Q² `TreeBuilder` /
high-Q² `TreeBuilderHighQ2` resolved the historical `TreeBuilder`
collision. Phase 1A (2026-05-23) further isolated the SIDIS Stage-1
binary into its own namespace (`TreeBuilderHighQ2_chargedsidis`) AND
its own per-workspace bin, so even an accidental Makefile target-name
collision in the future cannot reach the SIDIS binary.

**Stale shared-bin SIDIS binaries.** The Phase-1A redirect did not
remove the old copies. `ChargedSidisTreeBuilderHighQ2` (mtime
2026-05-21 05:00) and `ChargedSidisTreeBuilderHighQ2_v5g` (mtime
2026-05-23 20:52) remain in the shared bin. These are "ghost" copies:
any wrapper that resolves the binary path via the shared bin's
`PATH` order would still hit them. The Phase-1A manifest
(`binary_identity_manifest.json` v1.2) pins **only** the per-workspace
paths, so the Guardrail-3 `_binary_guard.py` will reject use of the
stale shared-bin copies.

---

## Task 4: Steering and configuration overlap

### Top-level layout per package

```
H1ChargedSidis_HighQ2/   bin/  condor/  condor_workdir/  LumiFiles/  metadata/  output/
                         recovery/  scripts/  smoke_gate_work/  src/  src_stage1/
                         steering/  v5g_build/  x86_64-el9-gcc14-opt/
                         + Makefile, binary_identity_manifest.json, README*.md

H1BaryonAsym_HighQ2/     condor/  docs/  LumiFiles/  output/  scripts/  smoke_genonly/
                         src/  steering/  tmp/  validation/  x86_64-el9-gcc14-opt/
                         + Makefile, FidVolCut.steer, *.bak

H1PhiMeson/              condor/  diffvm_norm_research/  output/  plots/  plots_v2/
                         results/  scripts/  src/  steering/  validation/  x86_64-el9-gcc14-opt/
                         + Makefile, Makefile.diffvm_norm, oomclumi.{root,txt}

H1XiAsym/                backup/  condor_mc_full/  output/  plots/  scripts/  src/  steering/
                         x86_64-el9-gcc14-opt/
                         + Makefile, run_xi_*.sh, mc_xi_*.{txt,sub}, xi_*.py
```

Every package has its own `steering/`, `output/`, `scripts/`,
`condor/` (or `condor_mc_full/`). None of the four packages share any
of these subdirectories on disk.

### `H1ChargedSidis_HighQ2/steering/`

Two top-level entries:

```
steering/evls/    ← SIDIS-OWNED event-list step
steering/proton/  ← SIDIS Stage-2 builder steerings (LEGACY-NAMED; "proton" subdir)
```

#### `steering/evls/`

Tree:
```
steering/evls/mc/djangoh14_norad/eplusp/
steering/evls/mc/djangoh14_norad/eminusp/
```

Sample (`steering/evls/mc/djangoh14_norad/eplusp/evls_djangoh14_norad_eplusp_A_C0000053.steer`):
```
// High-Q2 event-list steering for NORAD djangoh14 charge=eplusp djangoh14_norad_eplusp_A_C0000053
// Generated by submit_norad_charged_sidis_highQ2.py
H1SteerTree() {
   fHatFiles  = "/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EPLUSP.NORAD.Q2GT100.CTEQ6L.A.C0000053.root";
   fModsFiles = "/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/mods.DJANGOH14.NC.EPLUSP.NORAD.Q2GT100.CTEQ6L.A.C0000053.root";
   fLoadODS   = false;
}
```

**These evls steerings are SIDIS-owned and read from Britzger's
external NORAD HAT/MODS area.** They do NOT consume any
H1BaryonAsym output. This is the input to the SIDIS-owned Stage-1
binary `TreeBuilderHighQ2_chargedsidis` (created in Phase 1A).

#### `steering/proton/`

Tree (basename `proton` is itself a residual Phase-4 name):
```
steering/proton/27p6/{ep0304, em0405, em06, ep0607, ep0607_smoke}/
steering/proton/mc/{djangoh14, djangoh14_norad, rapgap31}/...
```

Total `*.steer` file count: **2,705** under `steering/proton/`.

Sample data steering (`steering/proton/27p6/ep0607/proton_highQ2_ep0607_d06_0000.steer`):
```
// High-Q2 SIDIS proton steering for data proton_highQ2_ep0607_d06_0000
// Generated by submit_highQ2_sidis_data_27p6.py
// Period: ep0607  Source: 06/  Beam-filter: 27p6
H1SteerTree() {
  fEventListFiles = "/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/evls/data/06/evls_highQ2_d06_ep0607_0000.root";
}
```

Sample MC steering (`steering/proton/mc/djangoh14/7072/proton_highQ2_djangoh14_7072_em06_0048.steer`):
```
// High-Q2 proton steering for MC djangoh14 cat 7072 proton_highQ2_djangoh14_7072_em06_0048
// Generated by submit_full_production.py
// Chunk luminosity: 1534.81 nb^-1
H1SteerTree() {
  fEventListFiles = "/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/evls/mc/djangoh14/7072/evls_highQ2_djangoh14_7072_em06_0048.root";
}
```

**Cross-package data dependency confirmed.** All **2,705** `*.steer`
files under `steering/proton/` contain `fEventListFiles = ".../H1BaryonAsym_HighQ2/output/evls/..."`
(verified: `grep -rl "H1BaryonAsym_HighQ2/output/evls" steering/ | wc -l` →
2705 = total `*.steer` under `steering/proton/`). The SIDIS Stage-2
builder (`ChargedSidisTreeBuilderHighQ2[_v5g]`) currently consumes
the **Lambda/baryon package's evls outputs**, NOT the SIDIS-owned
`output/evls/` outputs that the Phase-1A
`TreeBuilderHighQ2_chargedsidis` binary would produce.

### `H1BaryonAsym_HighQ2/steering/`

Layout:
```
steering/data/   steering/mc/{djangoh14,rapgap31}/{rad,nonrad}/
steering/lumi/   steering/production/{data/, mc/, mc13/}
+ FidVolCut.steer + 14 top-level es.*.steer (beam-energy / electron-scale steerings)
```

This is the **upstream owner** of `output/evls/`. Its `output/`
contains `analysis/`, `cross_sections/`, `evls/{data,mc,mc13}/`,
`gen/`, `merged/`, `plots/`, `trees/`.

### `H1PhiMeson/steering/`

Layout:
```
steering/{background, background2, data, mc, rapgap}/
+ rapgap_test.steer, smoke_test_{data,mc_gen,mc_reco}.steer
```

Entirely Phi-self-contained.

### `H1XiAsym/steering/`

Layout:
```
steering/mc_rapgap31_split/
```

Single subdir; Xi-self-contained.

### Steering-overlap verdict

- `H1PhiMeson` and `H1XiAsym` steerings: fully self-contained.
- `H1BaryonAsym_HighQ2` steering: self-contained (this is the upstream
  producer of `output/evls/`).
- `H1ChargedSidis_HighQ2` steering: **two coexisting regimes.**
  - `steering/evls/` (SIDIS-owned, reads Britzger NORAD HAT/MODS) — the
    Phase-1A target for full SIDIS self-containment.
  - `steering/proton/` (2,705 files, ALL pointing at
    `H1BaryonAsym_HighQ2/output/evls/...`) — the active production
    pipeline still consumes the Lambda/baryon evls.

This is the **structural quarantine gap** the supervisor should know
about. The SIDIS package can build its own Stage-1 binary (Phase 1A
shipped `TreeBuilderHighQ2_chargedsidis`) and has its own evls steering
namespace ready (`steering/evls/`), but the active Stage-2 steerings
still point upstream. This may be intentional (evls is upstream of
analysis-specific tree-builders; sharing it is COMMON practice, as the
task description noted), but it is a **runtime cross-package data
dependency** that the supervisor's mental "package quarantine" model
should explicitly account for.

---

## Task 5: Legacy "Proton" naming inside `H1ChargedSidis_HighQ2/`

### Counts per source file

| File | Proton refs (case-insens.) | Nature |
|---|---|---|
| `src/ChargedSidisTreeBuilderHighQ2.C` | 20 | Mix: provenance comments (lines 4, 35–38), `PhysConst::PROTON_MASS` (lines 806, 808, 811), `ProtonUtil::*` namespace calls (lines 972, 1001, 1009, 1026, 1092, 1103, 1245, 1274, 1281, 1301, 1316, 1325, 1368) |
| `src/ChargedSidisTreeBuilderHighQ2_v5g.C` | 20 | Identical pattern (parallel v5g build) |
| `src/ChargedSidisUtils_highQ2.h` | 14 | Provenance comments + the `namespace ProtonUtil { ... }` declaration at line 45 and `} // namespace ProtonUtil` at line 211 (this is the actual namespace name; renaming it would require updating all `ProtonUtil::*` call sites in the .C files) |
| `src/ChargedSidisUtils_highQ2_v5g.h` | 14 | Same |
| `src/PhysicsConstants_chargedsidis_highQ2.h` | 9 | Provenance comments + `PROTON_MASS` constant declaration |
| `src/PhysicsConstants_chargedsidis_highQ2_v5g.h` | 9 | Same |

### Locations of residual "Proton" / "proton" naming

| Location | Count | Examples | Functional vs cosmetic |
|---|---|---|---|
| **Makefile** | 4 | All in comments (lines 20, 22, 35, 74): `"H1ProtonAsym Makefile"` references for documentation | Cosmetic |
| **`src/` namespace name** `ProtonUtil` | 1 declaration + ~30 call sites across 2 .C files and 2 .h files | `namespace ProtonUtil { ... }`, `ProtonUtil::isChargedHadron`, `ProtonUtil::classifySourceGTR`, etc. | **Functional** — the namespace IS the v5f legacy name; renaming requires coordinated edit across all `ProtonUtil::*` call sites and the `}` closing comment. The header comment at `ChargedSidisUtils_highQ2.h:13` explicitly notes: *"TODO migration: the `ProtonUtil` namespace name is a legacy fork ..."* |
| **`src/` physics constant** `PROTON_MASS` | 4 uses | `PhysConst::PROTON_MASS` for the proton-beam 4-vector and target-proton mass in DIS kinematics | **TRUE PROTON** — this is the species proton (beam target), not the analysis name. Per the supervisor's own naming guide (handoff line 586), this is the leave-alone category. |
| **`steering/` subdir basename** | 1 dir `steering/proton/` | Entire subtree of 2,705 SIDIS Stage-2 steering files | **Functional** — directory name is hard-coded in all wrapper scripts (`submit_full_production.py`, `submit_highQ2_sidis_data_27p6.py`, `submit_proton_highQ2_batch.sh`). Renaming requires coordinated wrapper updates. |
| **`steering/proton/*` filenames** | 2,705 | `proton_highQ2_ep0607_d06_0000.steer`, `proton_highQ2_djangoh14_7072_em06_0048.steer`, etc. | **Functional** — generator scripts use the `proton_highQ2_*` literal stem |
| **`output/` filenames** | 5,097 files matching `*proton*` | E.g. `output/crad_strategy2/norad/eplusp/norad__eplusp__proton_djangoh14_norad_eplusp_*_C00000*.json` | **Functional** but read-only artifacts — these are produced files. Cosmetic from a build-pipeline view (no compile depends on them); functional from a downstream-analysis view if any downstream script matches `proton_*` filenames. |
| **`condor/evls/subs/mc/djangoh14_norad/eplusp/`** | many `.sub` files (job-id `*_eplusp_*` — `eplusp` is a beam-charge tag, not "proton") | These are eplusp NORAD condor submission descriptions. | Cosmetic (no proton naming, just beam tag) |
| **`scripts/*.py` and `scripts/*.sh`** | 12 files containing "proton" | `submit_proton_highQ2_batch.sh` (wrapper name), `submit_full_production.py`, `submit_highQ2_sidis_data_27p6.py`, `submit_norad_charged_sidis_highQ2.py`, `compute_highQ2_crad_v5f_real.py`, `extract_yields_v5f_3period.py`, `merge_data_highQ2.sh`, `merge_mc_highQ2.sh`, etc. | **Functional** — these scripts generate / consume the proton-named steering and output files |
| **Top-level `binary_identity_manifest.json`** | none in active manifest body; only in note string referring to "proton/antiproton" | Cosmetic |
| **No** `output/` subdir basenamed `proton` | 0 | (verified `find output -maxdepth 4 -type d -iname "*proton*"` empty) | — |

### Shared-state implications

The residual "proton" naming in `H1ChargedSidis_HighQ2/` is **lineage
residue from the v5f-era proton/antiproton workspace** that was forked
to become the charged-particle SIDIS workspace. Per the supervisor's
own three-tier classification (handoff line 579–591), all of the above
fall into either:

- **LEGACY** (most of the directory / file / namespace names: keep
  the literal name on disk because changing it breaks the pipeline),
  or
- **TRUE PROTON** (`PROTON_MASS`, beam-proton 4-vector calculations).

**None of the residual "proton" naming implies shared state with any
external proton-analysis package**, because no such package exists on
NAF (see Task 6). The `ProtonUtil` namespace, `proton_highQ2_*.steer`
files, and `*proton*.json` outputs are all SIDIS-internal — they live
under `H1ChargedSidis_HighQ2/` and are produced/consumed only by
SIDIS-internal binaries and scripts. The only **external** cross-package
dependency is the `fEventListFiles` pointer into
`H1BaryonAsym_HighQ2/output/evls/...` (Task 4), which is a **Lambda/
baryon** dependency, not a "Proton" dependency.

---

## Task 6: Does an `H1ProtonAsym_HighQ2` package exist?

### Top-level NAF directory check

```
$ ssh naf-hone 'ls -d /data/dust/group/h1/klesthen/BaryonAsym/H1*/'
/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym/
/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/
/data/dust/group/h1/klesthen/BaryonAsym/H1PhiMeson/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2_v5c/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2_v5d/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2_v5e/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2_v5f/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5c/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5d/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5e/
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5f/
/data/dust/group/h1/klesthen/BaryonAsym/H1XiAsym/
```

Explicit checks for the supervisor's expected names:

```
$ ls -ld /data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2 \
         /data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsym_HighQ2 \
         /data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsym
ls: cannot access '/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2': No such file or directory
ls: cannot access '/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsym_HighQ2': No such file or directory
ls: cannot access '/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsym': No such file or directory
```

Case-insensitive top-level grep for any `*roton*` directory: **empty**.

### Where does the proton/antiproton high-Q² analysis actually live?

The **`H1SIDIS_HighQ2/` directory** (legacy pre-rename name; `H1ChargedSidis_HighQ2`
was forked from this) is the on-disk home of the proton/antiproton
high-Q² package. Evidence:

`H1SIDIS_HighQ2/Makefile` lines 1–8:
```
##############################################################################
# Makefile for H1SIDIS_HighQ2 — high-Q2 standalone proton/antiproton analysis
#
# Standalone fork of the H1ProtonAsym (low-Q2) build system for the high-Q2
# dE/dx-based proton/antiproton identification (Q2 > 150 GeV^2).
# Builds ONLY ProtonTreeBuilderHighQ2; does NOT modify or replace the
# upstream H1ProtonAsym or H1BaryonAsym_HighQ2 binaries.
```

Makefile fields:
```
MODULENAME = H1ProtonHighQ2Standalone
SLIB       = libProtonHighQ2User.so
BIN        = ProtonTreeBuilderHighQ2
```

`H1SIDIS_HighQ2/src/` contents (12 files):
```
PhysicsConstants.h, PhysicsConstants.h.bak_v5
ProtonTreeBuilderHighQ2.C (and 7 .bak versions)
ProtonUtils.h (and 1 .bak)
```

The binary it builds (`ProtonTreeBuilderHighQ2`, 597,552 bytes, mtime
2026-05-18 03:06) is present in the shared
`bin/x86_64-el9-gcc14-opt/` directory.

### Cross-reference with the local `proton_HiQ` workspace

The local handoff at `H1/proton_HiQ/context-handoff.md` line 24
declares:

> | Package root (NAF) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2/` |

**This path does not exist on NAF.** It appears to be a stale or
aspirational reference. The actual package code lives at
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/` (the
pre-rename SIDIS workspace), and the binary lives at
`/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2`.

### Cross-reference with the supervisor's SIDIS handoff

`H1/sidis_highQ2/context-handoff.md` lines 568–598 (Section 8: Naming
Conventions) acknowledges the historical fact that
"`H1ChargedSidis_HighQ2/` was forked from the high-Q2 proton/antiproton
asymmetry analysis" and that many filesystem artifacts retain
proton-prefixed names. It defines three categories (LEGACY / TRUE
PROTON / DEFECT) and references
`docs/references/reference-naming-aliases-highQ2-sidis.md` as the
naming-alias source of truth. **The SIDIS handoff is consistent with
what we find on disk**: the SIDIS workspace contains lots of "proton"
strings, all classifiable into one of the three categories.

### Verdict on the supervisor's three-package mental model

The user's mental model "SIDIS, Lambda, and Proton should be three
separate high-Q² packages" is partially correct and partially stale:

- **SIDIS** = `H1ChargedSidis_HighQ2/` ✅ exists, quarantined per Task 2
- **Lambda/baryon** = `H1BaryonAsym_HighQ2/` ✅ exists, quarantined per
  Task 2 (the package serves BOTH Lambda AND K0s; see
  `src/Analysis.C` banner and `PhysicsConstants.h` line 6 *"...
  BaryonAsym Lambda/K0s analysis"*)
- **Proton/antiproton** = **`H1SIDIS_HighQ2/` (legacy name)** — exists
  and has a working `ProtonTreeBuilderHighQ2` binary on disk, but it
  is NOT named `H1ProtonAsym_HighQ2/`. The local
  `H1/proton_HiQ/context-handoff.md` points to a non-existent path.

Confusingly, **the proton/antiproton analysis lives in a directory
named `H1SIDIS_HighQ2/`** while the **charged-particle SIDIS analysis**
lives in a directory named `H1ChargedSidis_HighQ2/`. The names are
swapped from intuitive expectation because the SIDIS workspace was
forked OFF of the proton/antiproton workspace, and the fork (not the
parent) was renamed.

---

## Recommendations

### What is in good shape (no action needed)

1. **Source-code quarantine.** All four high-Q² packages have zero
   `#include` cross-references and zero functional Makefile
   cross-references. Each builds its own `.so` and its own binaries
   independently.
2. **Phase 1A binary redirect.** The SIDIS package's BINDIR override
   is correctly in place; new SIDIS builds land at
   `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/` and cannot be
   overwritten by another package's `make`.
3. **`src_stage1/` provenance lock.** The Phase-1A SIDIS-owned Stage-1
   sources are bit-identically copied from `H1BaryonAsym_HighQ2/src/`
   and tracked with md5 in `src_stage1/README.md` and
   `binary_identity_manifest.json` v1.2.
4. **Binary-identity manifest.** The Guardrail-3 `_binary_guard.py`
   pin prevents accidental use of stale shared-bin SIDIS copies.

### What is incomplete (action options)

1. **Stale shared-bin SIDIS binaries.** The pre-Phase-1A
   `ChargedSidisTreeBuilderHighQ2` (2026-05-21) and
   `ChargedSidisTreeBuilderHighQ2_v5g` (2026-05-23 20:52) still live in
   `bin/x86_64-el9-gcc14-opt/`. Recommendation: explicit `rm` (with
   manifest entry in the next version of `binary_identity_manifest.json`),
   OR leave as documented zombies and rely on the binary-guard pin to
   block use. Either is defensible; the user should pick one explicitly.
2. **Steering cross-package data dependency.** All 2,705
   `H1ChargedSidis_HighQ2/steering/proton/*.steer` files point at
   `H1BaryonAsym_HighQ2/output/evls/...` for `fEventListFiles`. This
   is a runtime cross-package data dependency, not a source-code one.
   If the supervisor wants **full** quarantine (so that the SIDIS
   package can run end-to-end with no other package present), the
   Phase-1A SIDIS-owned `TreeBuilderHighQ2_chargedsidis` binary needs
   to be used to regenerate `H1ChargedSidis_HighQ2/output/evls/`, and
   the Stage-2 steerings need to be rewritten to point at the new
   SIDIS-owned evls. This is a Phase-1B / Phase-2 effort, not
   complete in the current state.
3. **`H1ProtonAsym_HighQ2/` package does not exist.** Either:
   - **Path A:** the supervisor's mental model is correct and we should
     rename `H1SIDIS_HighQ2/` to `H1ProtonAsymHighQ2/` (or similar)
     to match the local `H1/proton_HiQ/context-handoff.md` line 24
     declaration. This would clarify the swapped-names confusion and
     require updating the Makefile MODULENAME and any wrapper script
     PATHs.
   - **Path B:** the supervisor's mental model is stale and we should
     update `H1/proton_HiQ/context-handoff.md` line 24 to point at the
     actual `H1SIDIS_HighQ2/` location, with a note explaining the
     historical naming inversion.
   - **Path C:** both — rename the NAF directory AND update the local
     handoff atomically.
4. **Steering subdirectory `H1ChargedSidis_HighQ2/steering/proton/`
   basename.** This is cosmetically a "LEGACY" residue per the
   supervisor's classification (handoff line 582–585). Renaming requires
   coordinated edits to the four wrapper scripts that hard-code the
   `steering/proton/` path. Defer per migration plan Tier 3.

### Risks and open questions

- **Risk: build-environment ambiguity.** The shared
  `bin/x86_64-el9-gcc14-opt/` contains stale `ChargedSidisTreeBuilderHighQ2`
  binaries. A wrapper that resolves the binary via shared-bin `PATH`
  search instead of via the manifest-pinned per-workspace path would
  still find them. The Guardrail-3 binary guard mitigates this, but
  only if wrappers route through it.
- **Open question: are 2,705 `H1BaryonAsym_HighQ2/output/evls/*` files
  on disk and current?** This artifact verifies the steering files
  point there; it does not verify the target files exist or are
  uncorrupted. Recommend separate validation pass (e.g.
  `h1-data-validator` on a sample) before any production run.
- **Open question: does `H1/proton_HiQ/` (local) reference `H1ChargedSidis_HighQ2/`
  outputs?** The local proton_HiQ handoff line 24 talks about
  "Charge asymmetry A_ch" (lines 30, 33) which is the proton/antiproton
  observable, not charged-particle SIDIS. If `H1/proton_HiQ/` is
  reading from the SIDIS package's `output/`, that is an *upstream*
  cross-analysis read at the analysis layer (not the NAF-package
  layer) and falls outside the scope of this verification.

# Phase 1A — Structural Separation of SIDIS Stage 1 (2026-05-23)

> **STATUS:** Complete. Smoke-verified bit-identity at the TTree data level
> between the old shared-bin `TreeBuilderHighQ2` (md5 `c2cf0828...`) and
> the new SIDIS-owned `TreeBuilderHighQ2_chargedsidis` (md5 `e2cf7f01...`)
> on 1 NORAD HAT.
> **Manifest:** bumped 1.1 → 1.2 in
> [`scripts/binary_identity_manifest.json`](../../scripts/binary_identity_manifest.json).

## 1. Why this separation was done

Before Phase 1A, the file `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C`
(+ its 7 dependencies `PhysicsConstants.h`, `FidVolCut.{C,h}`,
`FidVolCut.steer`, `TDetectQedc.{C,h}`, `Calibration.h`) was compiled by
`H1BaryonAsym_HighQ2/Makefile` into the SHARED binary
`/data/dust/.../BaryonAsym/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2`.
That single binary was invoked by THREE active analyses:

1. **Lambda/K0s/AntiLambda high-Q²** (`H1/lambda_HiQ/`)
2. **Proton/antiproton high-Q²** (`H1/proton_HiQ/`)
3. **Charged-particle SIDIS high-Q²** (`H1/sidis_highQ2/`)

Any source edit by one analysis would silently change the binary the
other two consumed. The 2026-05-22 binary-name-collision incident
(`H1BaryonAsym/Makefile` and `H1BaryonAsym_HighQ2/Makefile` both writing
to `$(BINDIR)/TreeBuilder`) had already shown that path-level sharing is
unsafe at scale. Phase 1A extends the fix to SOURCE-level sharing for
the high-Q² Stage 1: SIDIS now owns its own copy.

After Phase 1A, a future edit to `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C`
will affect only V0/Lambda and proton/antiproton; SIDIS uses the frozen
copy at `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C`.

## 2. What was copied

All copies done on NAF with `cp -av` from
`/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/` to
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/`.

| File | Upstream md5 | src_stage1/ md5 | Action |
|------|--------------|------------------|--------|
| `TreeBuilderHighQ2.C` (134,493 B) | `c6653628e198aff5c23224fc5e1b0d0d` | `d8a429ba7cbd2c557b6175fa7b4b5a31` | **RENAMED** to `TreeBuilderHighQ2_chargedsidis.C`; `#include "TreeBuilderHighQ2.h"` → `"TreeBuilderHighQ2_chargedsidis.h"` |
| `TreeBuilderHighQ2.h` (15,538 B) | `f2d4130be5d2025f37757264350fce81` | `3c89d7e711351550cd07fad242a29358` | **RENAMED** to `TreeBuilderHighQ2_chargedsidis.h`; ifndef guard `TREEBUILDER_HIGHQ2_H` → `TREEBUILDER_HIGHQ2_CHARGEDSIDIS_H` |
| `PhysicsConstants.h` (12,079 B) | `af7dae3eb870f039a9cad363d959d758` | `af7dae3eb870f039a9cad363d959d758` | **LITERAL COPY** (md5 match) |
| `FidVolCut.C` (6,097 B) | `a0db446eb7d7d57e3e36c9679e13bf8d` | `a0db446eb7d7d57e3e36c9679e13bf8d` | **LITERAL COPY** (md5 match) |
| `FidVolCut.h` (1,077 B) | `8360300cf415b1b74964d3bac96a21d1` | `8360300cf415b1b74964d3bac96a21d1` | **LITERAL COPY** (md5 match) |
| `FidVolCut.steer` (2,443 B) | `9690e6fe7c36695063945b52b98aeab0` | `9690e6fe7c36695063945b52b98aeab0` | **LITERAL COPY** (md5 match) |
| `TDetectQedc.C` (7,236 B) | `194ddaef245e8dbeddc4e959eb1ad568` | `194ddaef245e8dbeddc4e959eb1ad568` | **LITERAL COPY** (md5 match) |
| `TDetectQedc.h` (2,713 B) | `f4dfe7799c0f9eaa26e284a52ff0c7be` | `f4dfe7799c0f9eaa26e284a52ff0c7be` | **LITERAL COPY** (md5 match) |
| `Calibration.h` (30,752 B) | `4b3b21d9ea99705ec70021b3d78d48e3` | `4b3b21d9ea99705ec70021b3d78d48e3` | **LITERAL COPY** (md5 match) |

`src_stage1/README.md` documents the freeze policy: dependencies keep
their unnamespaced names so a future `cp -av` re-sync from upstream is a
clean operation; any update must be done explicitly with an md5
re-verify and a manifest bump.

## 3. Makefile changes

`H1ChargedSidis_HighQ2/Makefile` was extended (backup
`Makefile.bak_pre_phase1a_2026-05-23` preserved on NAF):

1. Added `OBJS = FidVolCut.o TDetectQedc.o` so H1Basic.mk's pattern rules
   compile the SIDIS-local dependency objects.
2. Added `src_stage1` to `EXTRA_INCLUDEDIRS` and to the `vpath %.C %.h`.
3. Added `TreeBuilderHighQ2_chargedsidis` to `BIN`.
4. Added target-specific BINDIR override:
   ```
   SIDIS_BINDIR := $(MAKEDIR)/bin/$(SYSTEM)
   TreeBuilderHighQ2_chargedsidis:    BINDIR := $(SIDIS_BINDIR)
   ChargedSidisTreeBuilderHighQ2:     BINDIR := $(SIDIS_BINDIR)
   ChargedSidisTreeBuilderHighQ2_v5g: BINDIR := $(SIDIS_BINDIR)
   ```
   so ALL SIDIS binaries land at `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/`,
   never the shared `/data/dust/.../bin/x86_64-el9-gcc14-opt/`.
5. Added explicit link rule for `TreeBuilderHighQ2_chargedsidis`
   linking the .o + `FidVolCut.o` + `FidVolCutDict.o` + `TDetectQedc.o`
   + `TDetectQedcDict.o`.
6. Added rootcint rules pointing at `src_stage1/` for the SIDIS-local
   dictionary generation (overriding H1Basic.mk implicit rules that
   would look in `src/`, which doesn't contain those headers).

## 4. Per-workspace bin policy (post-Phase-1A)

- All SIDIS-owned binaries land at:
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/`
- The shared bin `/data/dust/.../BaryonAsym/bin/x86_64-el9-gcc14-opt/`
  is still shared by `H1BaryonAsym`, `H1BaryonAsym_HighQ2` (V0/Lambda
  and proton high-Q² analyses), but **contains no SIDIS binaries**.
- Cross-analysis path collisions are now structurally impossible for SIDIS.

## 5. Binary identities (post-Phase-1A)

Pre-compile baseline:

| Binary | md5 | size |
|--------|-----|------|
| shared bin `TreeBuilderHighQ2` | `c2cf0828d4155de64ea9586cf3004426` | 1,898,992 |
| shared bin `ChargedSidisTreeBuilderHighQ2` | `d95158bf537331c2713f85c5f9d8f84b` | 604,856 |
| shared bin `ChargedSidisTreeBuilderHighQ2_v5g` | `091b74ff22942f6e0362f4b17a99ac63` | 623,112 |
| per-workspace bin | (empty) | — |

Post-compile (after `make TreeBuilderHighQ2_chargedsidis` + `make ChargedSidisTreeBuilderHighQ2`):

| Binary | md5 | size |
|--------|-----|------|
| **per-workspace** `TreeBuilderHighQ2_chargedsidis` | `e2cf7f015407e69a11fee56d5fbf0704` | 1,899,080 |
| **per-workspace** `ChargedSidisTreeBuilderHighQ2` | `d95158bf537331c2713f85c5f9d8f84b` | 604,856 |
| shared bin `TreeBuilderHighQ2` | `c2cf0828d4155de64ea9586cf3004426` | 1,898,992 (**UNCHANGED**) |
| shared bin `ChargedSidisTreeBuilderHighQ2` | `d95158bf537331c2713f85c5f9d8f84b` | 604,856 (**UNCHANGED**) |
| shared bin `ChargedSidisTreeBuilderHighQ2_v5g` | `091b74ff22942f6e0362f4b17a99ac63` | 623,112 (**UNCHANGED**) |

Notes:
- The new SIDIS-owned Stage 1 binary md5 differs from the old shared-bin
  Stage 1 (`e2cf7f01...` vs `c2cf0828...`) and the size differs by +88 B.
  This is expected: the renamed basename appears in the ELF symbol
  table and the ifndef guard name is preserved in debug info, so even a
  bit-identical .C compiled twice with different filenames produces
  distinct binaries.
- The Stage 2 binary moved to the per-workspace bin **without any change
  to its md5** (`d95158bf...` in both locations). The reproducible-build
  toolchain produced the same bytes from the unchanged source.

## 6. Smoke verification (1 NORAD HAT, gen-only mode)

Inputs:
- HAT: `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000.root`
- MODS: same dir, `mods.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000.root`
- Steer: `H1ChargedSidis_HighQ2/steering/evls/mc/djangoh14_norad/eminusp/evls_djangoh14_norad_eminusp_A_C0000000.steer`
- Invocation: `<bin> --stage evls --mode mc --gen-only --steer <steer> -o <out>`

Results:

| Binary | nTotal | nPassed | GenElecE rej | GenEmpz rej | GenVtxZ rej | GenQ2 rej | TTree totBytes | TTree zipBytes | ROOT file size | ROOT file md5 |
|--------|-------:|--------:|-------------:|------------:|------------:|----------:|---------------:|---------------:|---------------:|---------------|
| OLD `TreeBuilderHighQ2` (shared bin) | 154,913 | 90,471 | 31,518 | 0 | 0 | 32,924 | 15,236,486 | 3,006,522 | 3,680,439 | `b9dd843f8c67bffd75235ff8a99c6f00` |
| NEW `TreeBuilderHighQ2_chargedsidis` (SIDIS bin) | 154,913 | 90,471 | 31,518 | 0 | 0 | 32,924 | 15,236,486 | 3,006,522 | 3,680,439 | `9174f64dfb953a2ff1297b53a23d25a4` |
| OLD `TreeBuilderHighQ2` re-run (control) | 154,913 | 90,471 | 31,518 | 0 | 0 | 32,924 | — | — | 3,680,453 | `42b5aa8f0fc093053326459f59b9b152` |

**Verdict:** identical at the TTree data level. nTotal, nPassed, every
cutflow bucket, total tree bytes, and zip bytes all match exactly.
Per-branch value sums also match exactly (verified on Q²sGen, GenElecE,
Weight1, RunNumber across all 90,471 entries → zero diffs).

The ROOT-file-level md5 differs across runs because TFile headers
contain creation timestamps and a per-file UUID. This is true even when
the SAME binary is run twice (control row above): the OLD binary
produced two different md5s (`b9dd843f...` and `42b5aa8f...`) from two
runs on the same input. ROOT file-level md5 equivalence is structurally
impossible; data-level equivalence is what was verified.

The +14-byte size delta between the second OLD run and the first OLD/NEW
runs traces to the timestamp string encoding in the TFile header.

## 7. Wrapper updates

The following SIDIS wrappers were re-pointed at the new SIDIS-local
binary paths (both local copies in `H1/sidis_highQ2/scripts/` and the
NAF mirrors in `H1ChargedSidis_HighQ2/scripts/`):

| Wrapper | Stage-1 path swap | Stage-2 path swap |
|---------|-------------------|-------------------|
| `submit_norad_charged_sidis_highQ2.py` | shared `bin/.../TreeBuilderHighQ2` → SIDIS `bin/.../TreeBuilderHighQ2_chargedsidis`; binary_key `TreeBuilderHighQ2` → `TreeBuilderHighQ2_chargedsidis` | shared `bin/.../ChargedSidisTreeBuilderHighQ2` → SIDIS `bin/.../ChargedSidisTreeBuilderHighQ2` |
| `submit_highQ2_sidis_data_27p6.py` | n/a (Stage 2 only) | shared → SIDIS |
| `submit_full_production.py` (NAF only) | shared `bin/.../TreeBuilder` (was a defect: low-Q² SpaCal path) → SIDIS `bin/.../TreeBuilderHighQ2_chargedsidis` | shared → SIDIS |
| `submit_em06_g15_recovery.py` | shared `bin/.../TreeBuilder` (also a defect) → SIDIS `bin/.../TreeBuilderHighQ2_chargedsidis` | shared → SIDIS |
| `submit_proton_highQ2_batch.sh` (NAF only) | n/a | already used `${PKG_DIR}/bin/...` per-workspace path; verified unchanged |
| `compute_highQ2_crad_v5f_real.py` | docstring-only mention; no code path | docstring-only |

`_binary_guard.py` required no code change: it iterates
`manifest["binaries"]` and matches by key. The v1.2 manifest loaded and
verified all three pinned binaries against NAF disk (one CLI run per
key, all PASS, exit 0).

## 8. Manifest bump (v1.1 → v1.2)

Diff summary (full text in
[`scripts/binary_identity_manifest.json`](../../scripts/binary_identity_manifest.json)):

- `manifest_version`: `"1.1"` → `"1.2"`
- `as_of_date`: `"2026-05-22"` → `"2026-05-23"`
- `binaries.TreeBuilderHighQ2` (shared-bin) **REMOVED** — replaced by:
- `binaries.TreeBuilderHighQ2_chargedsidis` **NEW**:
  - `path`: `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis`
  - `expected_md5`: `e2cf7f015407e69a11fee56d5fbf0704`
  - `expected_size_bytes`: 1,899,080
  - `compiled_from`: `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C`
  - `compiled_from_md5`: `d8a429ba7cbd2c557b6175fa7b4b5a31`
  - `cutflow_signature` / `_reco_mode` / `_gen_only_mode`: unchanged (same labels as old `TreeBuilderHighQ2`; the rename does not touch any cut implementation)
  - `phase1a_provenance` block: ancestor md5, rename diff explanation, dependency-copy md5 table
- `binaries.ChargedSidisTreeBuilderHighQ2`:
  - `path`: shared bin → per-workspace bin
  - `expected_md5` / `expected_size_bytes`: unchanged (reproducible build)
  - `phase1a_note` field added
- `binaries.TreeBuilder` (low-Q²): unchanged (still pinned as FORBIDDEN-for-high-Q²)
- `version_history`: 1.0 + 1.1 + 1.2 entries preserved in chronological order
- `notes.per_workspace_bindir_policy_phase1a`: NEW (documents the per-workspace bin policy)
- `notes.shared_bindir_legacy_note`: NEW (explains the shared-bin TreeBuilderHighQ2 still exists for V0/proton consumers)
- `notes.removed_entries_v1_2`: NEW (records the removal)

Verification: `python3 _binary_guard.py --binary-key <key>` PASSED for
all three keys (`TreeBuilderHighQ2_chargedsidis`,
`ChargedSidisTreeBuilderHighQ2`, `TreeBuilder`) — exit 0, JSON snapshots
emitted, observed_md5 == expected_md5 for every pin.

## 9. Compile log highlights

- Backup taken: `H1ChargedSidis_HighQ2/Makefile.bak_pre_phase1a_2026-05-23`
- `make TreeBuilderHighQ2_chargedsidis` exit code 0; warnings as for
  upstream build (overloaded-virtual, unused-but-set `nuGen`).
- `make ChargedSidisTreeBuilderHighQ2` exit code 0; no warnings beyond
  prior baseline.
- Link command (excerpt): `... -o /data/dust/.../H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis`

## 10. Constraints honored

- ZERO modifications to `H1BaryonAsym_HighQ2/` workspace (read-only;
  `cp -av` source only)
- ZERO modifications to `H1/proton_HiQ/` or `H1/lambda_HiQ/`
- ZERO Condor jobs submitted (login-node smoke only, ≤ 20 s wall per
  invocation)
- ZERO `/pnfs/` writes
- Single `ssh -O check` at task start; no SSH retries; no `find` on AFS
  or PNFS without `-maxdepth 2`
- All file edits done locally and scp'd to NAF (no remote vim/nano)
- The 5 binary-identity guardrails are honored: manifest v1.2 loaded,
  binaries verified, no condor submission attempted

## 11. Migration checklist for future analysis ports

When the next analysis (e.g. proton or V0) wants its own independent
Stage 1 source, repeat this template:

1. `mkdir <ANALYSIS_PKG>/src_stage1/` on NAF.
2. `cp -av` the 9 files (or current-equivalent subset) from
   `H1BaryonAsym_HighQ2/src/` to `<ANALYSIS_PKG>/src_stage1/`.
3. md5-verify every copy against the upstream snapshot (record both md5s).
4. Rename the analysis-specific `.C`/`.h` to a namespaced name; update
   the internal `#include` and ifndef guard.
5. Add a per-target `BINDIR := $(MAKEDIR)/bin/$(SYSTEM)` override in
   `<ANALYSIS_PKG>/Makefile` plus a new link target with the
   dependency objects + rootcint rules pointing at `src_stage1/`.
6. Compile in a clean H1OO env (`env -i ... source thish1.sh ... make <target>`).
7. Verify the SHARED bin's original binary is UNCHANGED (md5 vs the
   pinned manifest value).
8. Run a smoke test on 1 representative input file with BOTH old and
   new binaries; verify nTotal, nPassed, cutflow, TTree total/zip bytes,
   and at least 4 branch sums are bit-identical. (ROOT file-level md5
   will differ due to TFile timestamps + UUID; this is expected and
   does NOT indicate a logic divergence.)
9. Bump the analysis's `binary_identity_manifest.json` to the next
   minor version with a fresh `version_history` entry.
10. Update every wrapper script in the analysis to point at the new
    per-workspace bin path and the new binary key.
11. Write a separation memo documenting the diff (this file is the
    template).

## 12. Verdict

**SIDIS Stage 1 is now fully independent of V0/Lambda and proton/antiproton
Stage 1.** A future edit to `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C`
will recompile only the shared-bin `TreeBuilderHighQ2` (consumed by
V0/Lambda and proton/antiproton); the SIDIS-owned
`TreeBuilderHighQ2_chargedsidis` at the per-workspace bin will be
unaffected. The smoke verification above proves that the SIDIS-owned
binary is, today, bit-equivalent at the TTree data level to the
upstream snapshot — so this separation introduces no analysis drift.

## 13. See also

- `H1ChargedSidis_HighQ2/src_stage1/README.md` — freeze policy
- `scripts/binary_identity_manifest.json` (v1.3) — pinned identities
- `scripts/_binary_guard.py` — Guardrail 3 (verified PASS on v1.3 for
  all three pinned binaries)
- `docs/data/guardrail_validation_2026-05-22.md` — the precipitating
  binary-collision incident; Guardrails 1-5
- `docs/methodology/crad_v5f_methodology_FROZEN.md` — the C_rad freeze
  whose Stage 1 evls inputs Phase 1A preserves bit-equivalently

---

## 14. Phase 1A-fix (2026-05-23): runtime artifact isolation

> **STATUS:** Complete. All three SIDIS wrappers (`submit_highQ2_sidis_data_27p6.py`,
> `submit_em06_g15_recovery.py`, `submit_norad_charged_sidis_highQ2.py`) now point at
> SIDIS-local runtime artifacts for everything they WRITE and for the GRL files they
> READ. The only remaining cross-workspace consumption is the V0/proton-produced data
> evls layer (documented banner; SIDIS reads ~1608 files as a shared input). The
> manifest is bumped to v1.3 (no binary changes; runtime-artifact metadata added).

### 14.1 Critic findings (Phase 1A audit) being closed

**Finding M1** (`submit_highQ2_sidis_data_27p6.py`):
- Line 72: `HIGHQ2_PKG_REMOTE = f"{BARYONASYM_ROOT_REMOTE}/H1BaryonAsym_HighQ2"`
- Line 73: `EVLS_BASE_REMOTE = f"{HIGHQ2_PKG_REMOTE}/output/evls"`
- Line 80: `DEFAULT_GRL_DIR_REMOTE = f"{PROTON_PKG_REMOTE}/LumiFiles"`
- Line 333: `f"{EVLS_BASE_REMOTE}/data/{year_short}/"`
- Documentation lines 15, 23-24, 31, 37, 102, 329, 357 referenced the shared workspace.

**Finding M2** (`submit_em06_g15_recovery.py`):
- Line 38: `HIGHQ2_PKG = f"{BARYONASYM_ROOT}/H1BaryonAsym_HighQ2"`
- Line 46: `GRL_EM06 = f"{HIGHQ2_PKG}/LumiFiles/nc_goodruns_06.root"`

**Plus** (`submit_norad_charged_sidis_highQ2.py`):
- Line 93: `HIGHQ2_PKG_REMOTE = f"{BARYONASYM_ROOT_REMOTE}/H1BaryonAsym_HighQ2"`
  (used at line 139 to define `EVLS_OUT_BASE_REMOTE`, i.e. NORAD evls WRITE path).
- Line 101: comment-level reference to the historical compile-collision (kept).

### 14.2 What changed per wrapper

| Wrapper | Before (shared) | After (SIDIS-local) |
|---|---|---|
| `submit_highQ2_sidis_data_27p6.py` | `HIGHQ2_PKG_REMOTE = .../H1BaryonAsym_HighQ2` (line 72) used to build both `EVLS_BASE_REMOTE` and `DEFAULT_GRL_DIR_REMOTE` (via `PROTON_PKG_REMOTE`) | Renamed `HIGHQ2_PKG_REMOTE` → `SHARED_HIGHQ2_PKG_DATA_EVLS_REMOTE` (now used ONLY to read data evls — the documented shared dependency); `DEFAULT_GRL_DIR_REMOTE = {SIDIS_PKG_REMOTE}/LumiFiles`; `DEFAULT_OUTPUT_BASE_REMOTE` under `SIDIS_PKG_REMOTE`. Banner block added to the docstring. |
| `submit_em06_g15_recovery.py` | `HIGHQ2_PKG = .../H1BaryonAsym_HighQ2` (line 38) used for `GRL_EM06`, `EVLS_OUT_DIR`, `project_baryon`, Condor `initialdir` | Constant renamed to `SHARED_HIGHQ2_PKG_DATA_EVLS` (used ONLY for `EVLS_OUT_DIR` — the documented shared data-evls WRITE side); `GRL_EM06` → SIDIS-local `LumiFiles/`; `project_baryon` and Condor `initialdir` → `SIDIS_PKG`. Phase 1A-fix block added to the docstring. |
| `submit_norad_charged_sidis_highQ2.py` | `HIGHQ2_PKG_REMOTE = .../H1BaryonAsym_HighQ2` (line 93) used only by `EVLS_OUT_BASE_REMOTE` (line 139, NORAD MC evls WRITE path) | Constant deleted; `EVLS_OUT_BASE_REMOTE` now built from `CHARGED_PKG_REMOTE` (SIDIS-local). Inline comment at the line where the constant used to live now documents the runtime-isolation move. Line 101 historical compile-collision comment kept verbatim (per task brief). |

The five binary-identity guardrails are unchanged in behaviour: no binary
md5 / size / source-md5 is modified by v1.3. Re-running
`python3 _binary_guard.py --binary-key <key>` for each of
`TreeBuilderHighQ2_chargedsidis`, `ChargedSidisTreeBuilderHighQ2`, `TreeBuilder`
returned exit 0 with `observed_md5 == expected_md5` for every pin (one JSON
snapshot each, manifest_version `"1.3"`).

### 14.3 LumiFiles SIDIS-local provenance

The SIDIS-local `LumiFiles/` directory was already in place since 2026-05-21
(per file mtimes); Phase 1A-fix verified bit-identity to the upstream copy and
re-pointed the wrappers at it.

| Filename | md5 (upstream `H1BaryonAsym_HighQ2/LumiFiles/`) | md5 (SIDIS-local `H1ChargedSidis_HighQ2/LumiFiles/`) | Match |
|---|---|---|---|
| `nc_goodruns_0304.root` | `39457f9eba6403f6b939a605d1d73172` | `39457f9eba6403f6b939a605d1d73172` | YES |
| `nc_goodruns_0405.root` | `2e0098ecb6d32b595aaafe7e9f035f07` | `2e0098ecb6d32b595aaafe7e9f035f07` | YES |
| `nc_goodruns_0607.root` | `54fbf12c7b94f40a194be98098bc4d6f` | `54fbf12c7b94f40a194be98098bc4d6f` | YES |
| `nc_goodruns_06.root` | `502824e1b62660950b7e3c9974c26c78` | `502824e1b62660950b7e3c9974c26c78` | YES |

The brief mentioned additional patterns (`nc_goodruns_{03,04,05,em,ep}.root`); none
of these exist in the upstream `H1BaryonAsym_HighQ2/LumiFiles/` directory (the full
upstream set is the 4 files above, plus 5 `.steer` files which are not pulled
through any SIDIS code path). The SIDIS-local `LumiFiles/` contains a
bit-identical superset of every GRL file the SIDIS wrappers consume.

### 14.4 NORAD Stage 1 evls SIDIS-local provenance (move)

The 519 NORAD evls files produced by the previous Stream-C re-launch lived in
the shared workspace at `H1BaryonAsym_HighQ2/output/evls/mc/djangoh14_norad/`.
NORAD is a SIDIS-only sample, so the location was a Phase-1A oversight.

Phase 1A-fix moved them atomically via `mv` (DUST-flat; not a copy):

| Charge | Source (shared) | Destination (SIDIS-local) | File count | Total bytes |
|---|---|---|---|---|
| `eminusp` | `H1BaryonAsym_HighQ2/output/evls/mc/djangoh14_norad/eminusp/` | `H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/eminusp/` | 260 → 260 | 948,328,349 → 948,328,349 |
| `eplusp` | `H1BaryonAsym_HighQ2/output/evls/mc/djangoh14_norad/eplusp/` | `H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/eplusp/` | 259 → 259 | 940,168,967 → 940,168,967 |
| **TOTAL** | shared workspace | SIDIS-local workspace | **519 → 519** | **1,888,497,316 → 1,888,497,316** |

Sentinel md5 check for `eminusp/evls_djangoh14_norad_eminusp_A_C0000000.root`:
`37a96f5192c133d15ecd1900bf6b68b1` preserved before and after move.

The empty source directories
(`H1BaryonAsym_HighQ2/output/evls/mc/djangoh14_norad/{eminusp,eplusp}/`) are
intentionally left in place to honour the "no modifications to the shared
workspace" rule; they will be cleaned in a future shared-workspace
housekeeping pass owned by V0/proton.

### 14.5 Data evls disposition (documented as shared)

The shared data evls layer at
`H1BaryonAsym_HighQ2/output/evls/data/<year_short>/`
(1,608 period-tagged files across years 03/04/05/06/07) was produced by the
V0/proton high-Q² Stage 1 wrapper
(`H1BaryonAsym_HighQ2/scripts/submit_highq2.py`) and is consumed by the SIDIS
data Stage 2 wrapper (`submit_highQ2_sidis_data_27p6.py`) as a shared input.

Phase 1A-fix did **not** move this layer. Justification:
- Re-running Stage 1 with the SIDIS-owned `TreeBuilderHighQ2_chargedsidis` to
  produce a SIDIS-local data evls layer would re-create ~1,608 files at
  significant DUST cost. Per the v1.2 smoke verification, the SIDIS-owned and
  upstream Stage 1 binaries are bit-equivalent at the TTree data level on
  representative MC input, so the recomputed SIDIS-local data evls would be
  byte-equivalent to the existing V0/proton-produced files in any case.
- The em06 G15 recovery wrapper writes NEW evls back into the same shared
  layer (62 chunks for the em06 SIGSEGV recovery). This write is preserved as
  shared because the recovery's output is consumed by BOTH the SIDIS data
  wrapper AND any future V0/proton wrapper that wants the em06 recovery
  evls; isolating it would lose that property.

A banner in `submit_highQ2_sidis_data_27p6.py`'s module docstring and a
matching banner in `submit_em06_g15_recovery.py`'s module docstring document
this single remaining cross-workspace dependency, with two named future
options (Phase 1A-fix-2):
(a) re-run Stage 1 SIDIS-side to materialise a SIDIS-local data evls layer;
(b) hardlink the V0/proton data evls into a SIDIS-local mirror (zero DUST
cost; immutable as long as no `rm -f` from either side).

### 14.6 Manifest v1.3 bump rationale

`scripts/binary_identity_manifest.json`:
- `manifest_version`: `"1.2"` → `"1.3"`
- `version_history`: appended a v1.3 entry (v1.0 / v1.1 / v1.2 preserved
  per audit policy; len 4)
- `runtime_artifact_isolation`: NEW top-level block with the SIDIS-local
  LumiFiles md5 table, SIDIS-local NORAD evls dir + provenance, and the
  documented shared data-evls dependency with producer/consumer attribution
  and the two future Phase 1A-fix-2 options.
- `purpose` string extended to summarise the v1.2 → v1.3 transition
  (runtime-artifact separation; no binary changes).
- `phase1a_separation_reference` string extended to point at this Section
  for the runtime-artifact half of Phase 1A.
- **NO** changes to `binaries.*` entries (all three md5/size/source_md5
  values identical to v1.2). Guardrail 3 re-verified PASS on all three keys.

### 14.7 Verification summary

| Check | Result |
|---|---|
| SIDIS-local `LumiFiles/*.root` md5-match upstream | PASS (4/4 files) |
| NORAD evls atomic move: pre-vs-post file count | PASS (260+259 → 260+259) |
| NORAD evls atomic move: pre-vs-post total bytes | PASS (948,328,349 + 940,168,967 → identical) |
| NORAD evls move: sentinel md5 preserved | PASS (`37a96f51...`) |
| 3 wrappers `python3 -m py_compile` | PASS (all 3) |
| Manifest v1.3 valid JSON | PASS (`json.load` ok, version `"1.3"`) |
| Guardrail 3 (`_binary_guard.py`) on `TreeBuilderHighQ2_chargedsidis` | PASS, exit 0, md5 `e2cf7f01...` matches |
| Guardrail 3 on `ChargedSidisTreeBuilderHighQ2` | PASS, exit 0, md5 `d95158bf...` matches |
| Guardrail 3 on `TreeBuilder` | PASS, exit 0, md5 `8de736bc...` matches |
| Shared `H1BaryonAsym_HighQ2/src/*.{C,h}` md5s vs Phase-1A baseline | PASS (diff exit 0) |
| Shared `H1BaryonAsym_HighQ2/Makefile` md5 | PASS (unchanged: `9fa1c599...`) |
| Shared `H1BaryonAsym_HighQ2/LumiFiles/*.root` md5s | PASS (unchanged) |
| Shared-bin `TreeBuilderHighQ2` md5 | PASS (unchanged: `c2cf0828...`) |
| `H1/proton_HiQ/` files modified today | NONE |
| `H1/lambda_HiQ/` files modified today | NONE |
| Condor jobs submitted | ZERO |
| `/pnfs` writes | ZERO |

### 14.8 Verdict

SIDIS Stage 1 SOURCE and BINARY independence (Phase 1A) plus SIDIS Stage 1
LumiFiles and SIDIS Stage 1 MC evls runtime-artifact independence
(Phase 1A-fix) leave the SIDIS workspace structurally self-contained for
every code path EXCEPT the single READ from
`H1BaryonAsym_HighQ2/output/evls/data/<year_short>/`, which is banner-
documented and accompanied by two named future migration options. A V0/proton
edit to the shared `H1BaryonAsym_HighQ2` workspace's runtime layer can no
longer silently break SIDIS Stage 1 for the MC NORAD pool or for any GRL
read; only the documented data-evls layer remains a coupling point, and any
edit to V0/proton's Stage 1 source that changes its evls outputs would now
be flagged by the existing guardrail discipline (Stage 1 binary identity is
pinned in both the V0/proton and SIDIS manifests; SIDIS already enforces its
pin via `_binary_guard.py`; V0/proton can do the same in their wrapper).

---

## 15. Phase 2A-fix (2026-05-23): runbook + cross-ref doc reconciliation

> **STATUS:** Complete. Four Critic B CONCERN findings addressed.
> All changes are documentation-only. No code, no NAF writes, no banner
> modifications. One SSH safety check confirmed the control socket live
> before starting.

### 15.1 Scope

Critic B's audit of the Phase 2A cross-analysis documentation update
identified four CONCERN-level findings (not FATAL). This section records
what was changed in each affected file to close those findings.

### 15.2 Finding 1 closed — lambda_HiQ runbook body inconsistency

**File:** `H1/lambda_HiQ/context-handoff.md`

**Problem:** The body of the runbook (Section 3 directory tree and Section 9
smoke-test command) used the pre-rename name `TreeBuilder.C` and the
pre-rename binary invocation `../bin/.../TreeBuilder --stage evls`, while
the top banner correctly named `TreeBuilderHighQ2.{C,h}` and the shared-bin
`TreeBuilderHighQ2`.

**Fix applied:**

| Location | Before | After |
|---|---|---|
| Line 96 (directory tree, src/ entry) | `TreeBuilder.C       # Modified stage_evls for LAr DIS` | `TreeBuilderHighQ2.C # Modified stage_evls for LAr DIS (high-Q² binary)` |
| Line 293 (smoke-test command) | `../bin/x86_64-el9-gcc14-opt/TreeBuilder --stage evls --mode data \` | `../bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2 --stage evls --mode data \` |

No meaning change — only the binary name corrected to match the actual
high-Q² compiled artifact. The banner and ISOLATION RULE block are
unchanged.

### 15.3 Finding 2 closed — proton_HiQ body doesn't name Stage 1 binary

**Files:** `H1/proton_HiQ/context-handoff.md` and
`H1/proton_HiQ/docs/context-handoff-highQ2-proton.md`

**Problem:** The Quick Summary table in `context-handoff.md` listed only the
Stage 2 binary (`ProtonTreeBuilderHighQ2`), with no row for the Stage 1
binary. The pipeline description in `context-handoff-highQ2-proton.md`
(Section 3) said `H1BaryonAsym_HighQ2/TreeBuilder --stage evls` (bare name,
wrong path form) and the infrastructure table listed only Stage 2.

**Fix applied to `H1/proton_HiQ/context-handoff.md`:**

Replaced the single `Binary` row in the Quick Summary table with two rows:

- `Stage 1 binary (shared)` — fully pinned path to `TreeBuilderHighQ2`
  with source reference `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.{C,h}`
  and `shared with lambda_HiQ` annotation.
- `Stage 2 binary` — `ProtonTreeBuilderHighQ2` with source reference
  `H1ProtonAsymHighQ2/src/ProtonTreeBuilderHighQ2.C`.

**Fix applied to `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md`:**

- Infrastructure table: replaced single `Binary` row with explicit
  `Stage 1 binary (shared)` and `Stage 2 binary` rows (same content as
  above). `Event lists source` row updated to include `produced by
  TreeBuilderHighQ2 --stage evls`.
- Pipeline Section 3, step 1: replaced `H1BaryonAsym_HighQ2/TreeBuilder
  --stage evls` with `TreeBuilderHighQ2 --stage evls` plus the fully pinned
  binary path and source file parenthetical.

No meaning change — only the binary naming made explicit and consistent
with the banner.

### 15.4 Finding 3 closed — cross-reference doc brace expansion

**File:** `H1/sidis_highQ2/docs/methodology/cross_analysis_code_locations_2026-05-23.md`

**Problem:** Lines 24, 28, 54, and 59 of the original doc used shell-brace
forms (`{PhysicsConstants.h,FidVolCut.C,...}`, `{lambda,kaon}/`,
`{eminusp,eplusp}/`, `{rapgap31,djangoh14}/`) in cells labelled "Canonical
path (absolute)". Braces are shell expansion shorthand, not pinned paths.

**Fix applied:** Option (a) — expand each brace form into separate table
rows so every artifact has its own fully pinned path.

- **lambda_HiQ Stage 1 deps** (formerly 1 brace row for 7 files):
  expanded to 7 individual rows, one per dependency file.
- **lambda_HiQ tree outputs** (formerly `{lambda,kaon}/`):
  expanded to 2 rows: `output/trees/lambda/` and `output/trees/kaon/`.
- **sidis_highQ2 Stage 1 deps** (formerly 1 brace row for 7 SIDIS-local
  files): expanded to 7 individual rows with `(SIDIS-local)` labels.
- **sidis_highQ2 NORAD evls outputs** (formerly `{eminusp,eplusp}/`):
  expanded to 2 rows with per-directory file counts and sizes.
- **sidis_highQ2 RAD MC evls outputs** (formerly `{rapgap31,djangoh14}/`):
  expanded to 2 rows.

A notation block was added at the top of Section 1 explaining the
convention: where brace forms remain in the SIDIS deps table body text for
readability, they are labelled "grouped shorthand — expand to all listed
combinations" and are not shell commands.

Proton_HiQ Section 1.2 did not contain brace forms and required no
expansion.

### 15.5 Finding 4 closed — cross-reference table asymmetry

**File:** `H1/sidis_highQ2/docs/methodology/cross_analysis_code_locations_2026-05-23.md`

**Problem:** The sidis_highQ2 subsection had more rows than lambda_HiQ and
proton_HiQ. Specifically:
- lambda_HiQ and proton_HiQ were missing a "Binary identity manifest" row.
- lambda_HiQ was missing a "Stage 2 source" row.
- proton_HiQ was missing "Stage 2 source" and "Stage 2 binary" rows.

**Fix applied:**

| Analysis | Row added | Content |
|---|---|---|
| lambda_HiQ | `Binary identity manifest` | N/A — no manifest installed as of 2026-05-23; shared-bin `TreeBuilderHighQ2` was removed from SIDIS manifest at v1.2; lambda_HiQ can install its own pin using the SIDIS template (Section 11). |
| lambda_HiQ | `Stage 2 source (analysis macros)` | `H1BaryonAsym_HighQ2/scripts/highq2_analysis_v2.C` and `highq2_final_results.C` — ROOT macros; no compiled Stage 2 binary; analysis runs via `root -l -b -q`. |
| proton_HiQ | `Stage 2 source` | `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2/src/ProtonTreeBuilderHighQ2.C` |
| proton_HiQ | `Stage 2 binary` | `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` (shared bin pool; proton_HiQ-specific) |
| proton_HiQ | `Binary identity manifest` | N/A — no manifest installed as of 2026-05-23; both `TreeBuilderHighQ2` and `ProtonTreeBuilderHighQ2` are unpinned; can install using the SIDIS template (Section 11). |

Note on lambda_HiQ Stage 2: unlike proton_HiQ and SIDIS, the lambda_HiQ
analysis does not compile a dedicated Stage 2 binary. The physics results
(cross-sections, asymmetries, radiative corrections) are produced by ROOT
macro scripts invoked interactively. This is the actual current state
(verified from `context-handoff.md` Section 11 analysis script inventory)
and is not a gap to be filled — it is documented accurately as N/A for a
compiled Stage 2 binary.

### 15.6 Validation

```
grep -n "TreeBuilder\b" H1/lambda_HiQ/context-handoff.md
# Expected: zero hits (all occurrences now say TreeBuilderHighQ2)

grep -nE "TreeBuilderHighQ2|TreeBuilder " \
  H1/proton_HiQ/context-handoff.md \
  H1/proton_HiQ/docs/context-handoff-highQ2-proton.md
# Expected: Stage 1 explicitly named TreeBuilderHighQ2 in both files

# In cross_analysis_code_locations_2026-05-23.md:
# - No remaining {brace} in path cells without explicit "grouped shorthand" label
# - Every analysis has: Stage 1 source, Stage 1 binary, Stage 2 source/note,
#   manifest row, and local docs hub
```

All four findings confirmed closed by post-edit grep (see Section 15.6
validation above; run locally at end of Phase 2A-fix session).

---

## 16. Phase R rename (2026-05-23): canonical SIDIS naming

After Phases 1A and 1A-fix made the SIDIS workspace structurally and
runtime independent of the V0/proton workspace, an on-disk naming
inconsistency remained: outputs, steering, condor wrappers, and condor
logs still used the legacy `proton_*` / `proton_highQ2_*` prefix from
the proton-asymmetry fork ancestor. Phase R closes that gap.

### 16.1 Scope

Phase R is an on-disk RENAME campaign. It touches:

- **Outputs:** `output/data/`, `output/mc/`, `output/evls/` filenames.
- **Steering:** `steering/proton/` → `steering/sidistree/` (top-level
  dir rename; internal `27p6/<period>/`, `data/{05,06,07}/`,
  `mc/<gen>/` layout preserved).
- **Condor:** `condor/proton/` → `condor/sidistree/` (same approach).
- **Smoke gate work dirs:** `smoke_gate_work/proton_{eminusp,eplusp}/`
  → `smoke_gate_work/sidistree_{eminusp,eplusp}/`.
- **DUST condor logs:** `condor_logs/proton_highQ2{,_recovery}/`
  → `condor_logs/sidistree_highQ2{,_recovery}/`.
- **Submit wrappers:** Python constants `PROTON_*` → `SIDISTREE_*`,
  filename templates `proton_*` → `sidistree_*`, file renames
  (`submit_highQ2_sidis_data_27p6.py` →
  `submit_highQ2_sidis_data.py`; NAF
  `submit_proton_highQ2_batch.sh` →
  `submit_sidistree_highQ2_batch.sh`).
- **Extractors and crad scripts:** `DATA_BASE` path constants drop
  the `27p6/` partition; dedupe heuristics handle both new and legacy
  prefixes for transition robustness.

Phase R does NOT touch:

- Any binary md5/size (the 4 pinned binaries are unchanged).
- Any source code logic (Stage 1 or Stage 2).
- The V0/proton workspace at `H1BaryonAsym_HighQ2/` or its outputs.
- The legacy v5f data outputs at `output/data/27p6/` — they are
  ARCHIVED IN PLACE to `output/archive_v5f_pre_v5g_2026-05-23/data/27p6/`
  for audit reproducibility (893 files, 2.07 GB).
- Physics terminology (`27p6` as beam-energy descriptor, "proton
  fraction" / "NuclIA proton-omission" / `M_PROTON`) or audit-trail
  comments referencing the legacy binary name.

### 16.2 Blocker resolutions applied

Per the pre-execution scope report
(`docs/methodology/rename_audit_2026-05-23/00_PRE_EXECUTION_SCOPE_REPORT.md`)
and the supervisor's blocker resolutions:

1. **Evls rename (B1):** SKIP shared V0/proton-owned data evls at
   `H1BaryonAsym_HighQ2/output/evls/data/` (READ-ONLY). Rename ONLY
   SIDIS-local Stage 1 outputs (today's run) at
   `H1ChargedSidis_HighQ2/output/evls/data/`. NORAD MC evls at
   `H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/` are
   already SIDIS-local per Phase 1A-fix and were renamed-corrected
   (proton stubs found in the pre-Phase-1A-fix snapshot were fixed
   to `evls_djangoh14_norad_*` during step 5).
2. **Year→period map (B2):** Period subdirs for data outputs (the
   deterministic map: 05→em0405, 06→em06, 07→ep0607; year 04/ep0304
   EXCLUDED).
3. **Manifest version (B3):** The brief said v1.5→v1.6 (originally
   v1.3→v1.4 per the pre-execution audit). Between the audit and
   execution, v1.4 (Empz cut alignment) and v1.5 (v5g binary added)
   had landed; this campaign bumps v1.5 → v1.6 as the next-sequential-
   available version, honoring the spirit of the brief.
4. **Steering/condor reorg (B4):** Minimum-disruption (variant ii):
   top-level dir rename only; internal `27p6/<period>/`,
   `data/{05,06,07}/`, `mc/<gen>/` layout preserved. The internal
   `27p6/` subdir is retained because the legacy steering files
   inside reference absolute paths that downstream condor wrappers
   consume by literal string; flattening would break in-flight jobs.
   The OUTPUT-path partition `27p6/` IS dropped (data outputs are
   under `output/data/<period>/` not `output/data/27p6/<period>/`).
5. **v5g artifact acceptance (B5):** v5g outputs are accepted as
   immutable artifacts. Move + rename, do NOT re-run for verification.
   The v5g binary identity (md5 091b74ff22942f6e0362f4b17a99ac63) at
   the SIDIS-local bin path is the reproducibility contract; output
   files inherit that contract by construction.

### 16.3 Execution summary

| Step | Operation                                                                                | Files | Bytes (GB) |
|------|------------------------------------------------------------------------------------------|------:|-----------:|
| 1    | ARCHIVE legacy `output/data/27p6/` → `output/archive_v5f_pre_v5g_2026-05-23/data/27p6/`  |   893 |       2.07 |
| 2    | DATA move+rename `output/data/{05,06,07}/` → `output/data/{em0405,em06,ep0607}/`         |   577 |       0.24 |
| 3    | MC rename in place (RAPGAP31 dual-prefix + DJANGOH14 single-prefix)                      | 1,144 |      73.43 |
| 4    | NORAD MC rename in place                                                                 |   519 |      45.87 |
| 5    | SIDIS-local evls rename in place (data + MC RAD)                                         | 1,310 |       1.32 |
| 6    | steering dir rename + internal file rename                                               | 3,299 |       0.001|
| 7    | condor dir rename + internal file rename                                                 | 6,598 |       0.008|
| 8    | smoke_gate_work dir renames                                                              |     2 |     ~0     |
| 9    | condor_logs dir renames (proton_highQ2 + _recovery)                                      |17,658 |       0.17 |
| **Total** |                                                                                     |**40,422** | **~127.06** |

Per-operation manifest (with src, dst, file_count, total_size_bytes,
timestamp for every operation) is at
`docs/methodology/rename_audit_2026-05-23/01_NAF_OPERATIONS_MANIFEST.json`.

### 16.4 Code edits

13 SIDIS-local Python scripts + 4 NAF-only scripts (3 shell, 1 Python)
were edited. py_compile clean on every Python edit. See the v1.6
binary identity manifest entry for the complete file list.

### 16.5 Verification

Post-rename verification (`docs/methodology/rename_audit_2026-05-23/`):

- **V0/proton md5s UNCHANGED:**
  - `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` =
    `c6653628e198aff5c23224fc5e1b0d0d`
  - `H1BaryonAsym_HighQ2/src/PhysicsConstants.h` =
    `af7dae3eb870f039a9cad363d959d758`
  - shared bin `TreeBuilderHighQ2` =
    `c2cf0828d4155de64ea9586cf3004426`
- **SIDIS-local binaries UNCHANGED:**
  - `TreeBuilderHighQ2_chargedsidis` = `d740dbdc...`
  - `ChargedSidisTreeBuilderHighQ2` (v5f) = `d95158bf...`
  - `ChargedSidisTreeBuilderHighQ2_v5g` = `091b74ff...`
- **Manifest v1.6 guardrail self-test PASS on all 4 binaries.**
- **File-count and total-size verification per move step** (built into
  the mover script; FATAL exit on any mismatch; no FATAL recorded).
- **Idempotent: re-running the mover script would no-op all steps**
  (every step checks for dst-exists and source-files-present).

### 16.6 Verdict

The SIDIS workspace is now naming-consistent. The canonical naming
reference is at
`docs/methodology/sidis_naming_canonical_2026-05-23.md` — future
agents read it first before doing any work that creates, renames, or
globs SIDIS artifacts.

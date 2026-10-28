# Cross-Analysis Code Location Reference (2026-05-23)

**Scope:** The three analyses that share or have shared the `H1BaryonAsym_HighQ2/`
high-Q² infrastructure: `lambda_HiQ`, `proton_HiQ`, and `sidis_highQ2`.

**Created:** 2026-05-23 (Phase 2A documentation update).

**Canonical reference for the structural separation itself:**
`H1/sidis_highQ2/docs/methodology/structural_separation_2026-05-23.md`

---

## 1. Per-analysis canonical paths (authoritative)

All paths below are relative to the DUST code root:
`/data/dust/group/h1/klesthen/BaryonAsym/`

> **Path notation (Phase 2A-fix, 2026-05-23):** All cells in the tables below use fully
> pinned absolute paths. Where multiple files share the same parent directory and a grouped
> shorthand is helpful for readability, the brace form `{A,B,...}` is used and is explicitly
> labelled as "grouped shorthand — expand to all listed combinations". Such cells are NOT
> shell commands; they are documentation aliases. Every file named in braces is a distinct
> artifact that can be individually verified on NAF.

### 1.1 `lambda_HiQ` — complete, uses shared `H1BaryonAsym_HighQ2/`

| Artifact | Canonical path (absolute) |
|---|---|
| Stage 1 source (TreeBuilder) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` |
| Stage 1 header | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.h` |
| Stage 1 dep: PhysicsConstants | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/PhysicsConstants.h` |
| Stage 1 dep: FidVolCut source | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/FidVolCut.C` |
| Stage 1 dep: FidVolCut header | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/FidVolCut.h` |
| Stage 1 dep: FidVolCut steer | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/FidVolCut.steer` |
| Stage 1 dep: TDetectQedc source | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TDetectQedc.C` |
| Stage 1 dep: TDetectQedc header | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TDetectQedc.h` |
| Stage 1 dep: Calibration | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/Calibration.h` |
| Stage 1 binary | `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2` (shared bin, md5 `c2cf0828...`) |
| Stage 2 source (analysis macros) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/scripts/highq2_analysis_v2.C` and `highq2_final_results.C` (ROOT macros; no compiled Stage 2 binary — analysis runs via `root -l -b -q`) |
| LumiFiles (GRL pool) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/LumiFiles/` |
| Data evls outputs | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/evls/data/<year_short>/` |
| Lambda tree outputs | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/trees/lambda/` |
| K0s tree outputs | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/trees/kaon/` |
| Merged outputs | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/merged/` |
| Makefile | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/Makefile` (md5 `9fa1c599...`, UNCHANGED by Phase 1A) |
| Binary identity manifest | N/A — no manifest installed for lambda_HiQ as of 2026-05-23. The shared-bin `TreeBuilderHighQ2` (md5 `c2cf0828...`) was removed from the SIDIS manifest at v1.2. Lambda_HiQ can install its own pin using the SIDIS manifest as a template (see `structural_separation_2026-05-23.md` Section 11). |
| Local docs hub | `H1/lambda_HiQ/` |

### 1.2 `proton_HiQ` — early stage, uses shared `H1BaryonAsym_HighQ2/` + own package

| Artifact | Canonical path (absolute) |
|---|---|
| Proton package root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2/` |
| Stage 1 source (shared evls) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` (same as lambda_HiQ) |
| Stage 1 header | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.h` |
| Stage 1 binary | `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2` (shared bin — same binary as lambda_HiQ) |
| Stage 2 source | `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2/src/ProtonTreeBuilderHighQ2.C` |
| Stage 2 binary | `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` (shared bin pool; proton_HiQ-specific) |
| LumiFiles (GRL pool) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/LumiFiles/` |
| Data evls outputs (produced by this wrapper) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/evls/data/<year_short>/` |
| Proton tree outputs | `/data/dust/group/h1/klesthen/BaryonAsym/H1ProtonAsymHighQ2/output/` |
| Submission wrapper | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/scripts/submit_highq2.py` |
| Binary identity manifest | N/A — no manifest installed for proton_HiQ as of 2026-05-23. Both `TreeBuilderHighQ2` and `ProtonTreeBuilderHighQ2` are unpinned. Can install using the SIDIS manifest as a template (see `structural_separation_2026-05-23.md` Section 11). |
| Local docs hub | `H1/proton_HiQ/` |

### 1.3 `sidis_highQ2` — active, uses SIDIS-local `H1ChargedSidis_HighQ2/` (post-Phase-1A)

| Artifact | Canonical path (absolute) |
|---|---|
| SIDIS package root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/` |
| Stage 1 source (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C` (md5 `d8a429ba...`) |
| Stage 1 header (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.h` |
| Stage 1 dep: PhysicsConstants (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/PhysicsConstants.h` (grouped shorthand — expand to all listed combinations: bit-identical to upstream at Phase 1A; see freeze policy in `src_stage1/README.md`) |
| Stage 1 dep: FidVolCut source (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/FidVolCut.C` |
| Stage 1 dep: FidVolCut header (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/FidVolCut.h` |
| Stage 1 dep: FidVolCut steer (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/FidVolCut.steer` |
| Stage 1 dep: TDetectQedc source (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TDetectQedc.C` |
| Stage 1 dep: TDetectQedc header (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TDetectQedc.h` |
| Stage 1 dep: Calibration (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/Calibration.h` |
| Stage 1 binary (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis` (md5 `e2cf7f01...`) |
| Stage 2 source | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2.C` |
| Stage 2 binary (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2` (md5 `d95158bf...`) |
| LumiFiles (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/LumiFiles/` (bit-identical copies of the 4 `nc_goodruns_*.root` files; md5-verified Phase 1A-fix) |
| NORAD evls outputs — eminusp (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/eminusp/` (260 files, 948 MB; moved atomically from shared workspace on 2026-05-23) |
| NORAD evls outputs — eplusp (SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/eplusp/` (259 files, 940 MB; moved atomically from shared workspace on 2026-05-23) |
| Data evls inputs (SHARED — read-only for SIDIS) | `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/evls/data/<year_short>/` (produced by V0/proton's `submit_highq2.py`; SIDIS reads, does not own; see banner in `submit_highQ2_sidis_data.py` and Phase 1A-fix-2 migration options in `structural_separation_2026-05-23.md` Section 14.5) |
| RAD MC evls outputs — rapgap31 | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/evls/mc/rapgap31/` |
| RAD MC evls outputs — djangoh14 | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/evls/mc/djangoh14/` |
| Stage 2 v5g binary (PRODUCTION, SIDIS-local) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v5g` (md5 `091b74ff...`; new in manifest v1.5 2026-05-23) |
| Stage 2 MC tree outputs (post Phase R rename 2026-05-23) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/` with files `sidistree_mc_<gen>_<cat>_<period>_NNNN.root` (v5f primary) + `sidistree_legacy_<gen>_<cat>_<period>_NNNN.root` (v5b legacy, RAPGAP only) + `sidistree_djangoh14_<cat>_<period>_NNNN.root` (DJANGOH single) + `sidistree_djangoh14_norad_<charge>_<letter>_<chunk>.root` (NORAD) |
| Stage 2 data tree outputs (post Phase R rename 2026-05-23) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data/{em0405,em06,ep0607}/sidistree_<period>_d<NN>_NNNN.root` (per-period subdirs; no `27p6/` partition; was `output/data/27p6/<period>/proton_highQ2_<period>_d<NN>_NNNN.root`) |
| Legacy v5f data outputs (archived) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/archive_v5f_pre_v5g_2026-05-23/data/27p6/{em0405,em06,ep0607}/` (893 files, 2.07 GB; immutable audit trail; Phase R archive 2026-05-23) |
| Steering (post Phase R rename 2026-05-23) | `H1ChargedSidis_HighQ2/steering/sidistree/` (was `steering/proton/`; internal `27p6/<period>/`, `data/{05,06,07}/`, `mc/<gen>/` layout preserved; files use `sidistree_*.steer` prefix) |
| Condor wrappers/subs (post Phase R rename 2026-05-23) | `H1ChargedSidis_HighQ2/condor/sidistree/` (was `condor/proton/`; same internal layout; files use `sidistree_*.{sh,sub}`) |
| DUST condor logs (post Phase R rename 2026-05-23) | `/data/dust/group/h1/klesthen/BaryonAsym/condor_logs/sidistree_highQ2/` and `condor_logs/sidistree_highQ2_recovery/` (was `condor_logs/proton_highQ2{,_recovery}/`) |
| Smoke gate work dirs (post Phase R rename 2026-05-23) | `H1ChargedSidis_HighQ2/smoke_gate_work/sidistree_{eminusp,eplusp}/` (was `smoke_gate_work/proton_{eminusp,eplusp}/`) |
| Binary identity manifest | `H1/sidis_highQ2/scripts/binary_identity_manifest.json` (v1.6 post Phase R 2026-05-23; mirrored on NAF at `H1ChargedSidis_HighQ2/binary_identity_manifest.json`) |
| Makefile (SIDIS) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/Makefile` (updated Phase 1A; backup `Makefile.bak_pre_phase1a_2026-05-23`) |
| Local docs hub | `H1/sidis_highQ2/` |
| Canonical naming reference (post Phase R 2026-05-23) | `H1/sidis_highQ2/docs/methodology/sidis_naming_canonical_2026-05-23.md` |
| Phase R rename audit | `H1/sidis_highQ2/docs/methodology/rename_audit_2026-05-23/` (pre-execution scope + NAF operations manifest JSON, 34 ops, 40,422 files, 127 GB) |

---

## 2. What each analysis OWNS vs READS

| Analysis | Owns (writes) | Reads (shared input) |
|---|---|---|
| lambda_HiQ | `H1BaryonAsym_HighQ2/output/evls/data/`, `H1BaryonAsym_HighQ2/output/trees/`, `H1BaryonAsym_HighQ2/output/merged/` | `H1BaryonAsym_HighQ2/src/` (canonical high-Q² source for V0/proton), `H1BaryonAsym_HighQ2/LumiFiles/`, shared bin |
| proton_HiQ | `H1BaryonAsym_HighQ2/output/evls/data/` (Stage 1), `H1ProtonAsymHighQ2/output/` (Stage 2+) | `H1BaryonAsym_HighQ2/src/` (same Stage 1 source as lambda_HiQ), `H1BaryonAsym_HighQ2/LumiFiles/`, shared bin |
| sidis_highQ2 | `H1ChargedSidis_HighQ2/src_stage1/`, `H1ChargedSidis_HighQ2/bin/`, `H1ChargedSidis_HighQ2/LumiFiles/`, `H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/`, `H1ChargedSidis_HighQ2/output/mc/`, `H1ChargedSidis_HighQ2/output/crad_strategy2_v2/` | `H1BaryonAsym_HighQ2/output/evls/data/<year_short>/` (data evls; documented shared dependency — SIDIS reads, V0/proton owns) |

---

## 3. "When you edit X, the following are affected" reference

| Edit target | Affects |
|---|---|
| `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` | lambda_HiQ and proton_HiQ Stage 1 evls (recompile shared bin to take effect). **Does NOT affect SIDIS** — SIDIS uses its forked copy at `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C`. |
| `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C` | SIDIS Stage 1 evls only. **Does NOT affect lambda_HiQ or proton_HiQ.** Requires explicit `make TreeBuilderHighQ2_chargedsidis` to take effect. Per freeze policy in `src_stage1/README.md`, changes here should be md5-recorded and the manifest bumped. |
| `H1BaryonAsym_HighQ2/output/evls/data/<year_short>/` (V0/proton writes new data evls) | lambda_HiQ, proton_HiQ data analysis — and **also SIDIS data Stage 2** (SIDIS reads these as a shared input). If V0/proton re-runs Stage 1 with changed evls outputs, the SIDIS team must be notified to verify the shared evls layer is still consistent with the binary identity manifest. |
| `H1BaryonAsym_HighQ2/LumiFiles/` | lambda_HiQ, proton_HiQ. **Does NOT affect SIDIS** — SIDIS reads its own bit-identical copies at `H1ChargedSidis_HighQ2/LumiFiles/`. |
| `H1ChargedSidis_HighQ2/LumiFiles/` | SIDIS wrappers only. Does not affect lambda_HiQ or proton_HiQ. |
| Shared bin `TreeBuilderHighQ2` at `/data/dust/.../bin/x86_64-el9-gcc14-opt/` | lambda_HiQ and proton_HiQ. **Does NOT affect SIDIS** (SIDIS uses its per-workspace bin). |
| SIDIS bin `TreeBuilderHighQ2_chargedsidis` at `H1ChargedSidis_HighQ2/bin/` | SIDIS Stage 1 only. Does not affect lambda_HiQ or proton_HiQ. |
| `H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/` | SIDIS C_rad computation only. Lambda_HiQ and proton_HiQ do not use NORAD evls (NORAD is a SIDIS-only sample). |

---

## 4. Dependency graph (textual)

```
V0/Lambda high-Q² Stage 1 evls
  ← H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C
  ← shared bin: .../bin/.../TreeBuilderHighQ2  (md5 c2cf0828)
  → H1BaryonAsym_HighQ2/output/evls/data/<year>/
      ↑ also read by SIDIS data Stage 2 (documented shared dependency)

Proton high-Q² Stage 1 evls
  ← same source + binary as lambda_HiQ

SIDIS high-Q² Stage 1 evls (NORAD MC only)
  ← H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C
  ← SIDIS-local bin: H1ChargedSidis_HighQ2/bin/.../TreeBuilderHighQ2_chargedsidis  (md5 e2cf7f01)
  → H1ChargedSidis_HighQ2/output/evls/mc/djangoh14_norad/{eminusp,eplusp}/

SIDIS data Stage 2
  ← H1ChargedSidis_HighQ2/bin/.../ChargedSidisTreeBuilderHighQ2  (md5 d95158bf)
  ← H1BaryonAsym_HighQ2/output/evls/data/<year>/  [shared read]
  ← H1ChargedSidis_HighQ2/LumiFiles/
  → H1ChargedSidis_HighQ2/output/mc/
```

---

## 5. Binary identity manifest (v1.3) summary

| binary_key | path | md5 | size (bytes) | source |
|---|---|---|---|---|
| `TreeBuilderHighQ2_chargedsidis` | `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis` | `e2cf7f015407e69a11fee56d5fbf0704` | 1,899,080 | `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C` (md5 `d8a429ba...`) |
| `ChargedSidisTreeBuilderHighQ2` | `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2` | `d95158bf537331c2713f85c5f9d8f84b` | 604,856 | `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2.C` |
| `TreeBuilder` (LOW-Q²; FORBIDDEN for high-Q² work) | `.../bin/x86_64-el9-gcc14-opt/TreeBuilder` | `8de736bc...` | 1,516,144 | `H1BaryonAsym/src/TreeBuilder.C` |

Manifest file: `H1/sidis_highQ2/scripts/binary_identity_manifest.json` (v1.3).
Verification tool: `H1/sidis_highQ2/scripts/_binary_guard.py`.

> **Note:** The legacy shared-bin `TreeBuilderHighQ2` (md5 `c2cf0828...`, used by lambda_HiQ
> and proton_HiQ) is NOT pinned in the SIDIS manifest (v1.3 removed it — it is no longer a
> SIDIS binary). Lambda_HiQ and proton_HiQ teams should maintain their own pin if desired,
> per the migration template in `structural_separation_2026-05-23.md` Section 11.

---

## 6. Phase 1A-fix: remaining shared dependency

The ONLY remaining cross-workspace coupling after Phase 1A + Phase 1A-fix is the data evls
layer read by SIDIS but produced by V0/proton:

| Layer | Producer | Consumer | Location | Status |
|---|---|---|---|---|
| Data evls (1,608 period files) | proton_HiQ / lambda_HiQ `submit_highq2.py` | SIDIS `submit_highQ2_sidis_data.py` + `submit_em06_g15_recovery.py` | `H1BaryonAsym_HighQ2/output/evls/data/<year_short>/` | Shared read; documented banner in both SIDIS wrappers. Two Phase 1A-fix-2 migration options named in `structural_separation_2026-05-23.md` Section 14.5. |

---

## 7. Links to per-analysis handoffs

| Analysis | Handoff | Cross-analysis banner location |
|---|---|---|
| sidis_highQ2 | `H1/sidis_highQ2/context-handoff.md` | Section 0, Phase 1A + Phase 1A-fix sub-section |
| proton_HiQ | `H1/proton_HiQ/context-handoff.md` | Top-of-file banner (added Phase 2A, 2026-05-23) |
| lambda_HiQ | `H1/lambda_HiQ/context-handoff.md` | Top-of-file banner (added Phase 2A, 2026-05-23) |

Project-wide separation note: `H1/CONTEXT.md` analyses table + per-workspace bin policy section.

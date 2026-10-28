# SIDIS High-Q² v6 Production Builder Changes (2026-06-04)

**Status:** In progress. Change #1 implemented (Python response builder
fork). Changes #2 and #3 documented as patch plans; not yet applied to
the live `v6sys` C++ source on NAF. NO production launch in this
dispatch.

**Owner specialist:** h1-builder-engineer (this dispatch). Follow-on
specialists: h1-software (Change #3 API confirmation), h1-physics-analyst
and h1-analysis-critic (policy on JES+RCES merging vs split, and the
Python reco Pt cut value), h1-naf-operator (production after smoke
passes).

## Scope

The user-requested v6 production lineage is distinct from the v6sys
detector-systematic Stage 2 fork already documented in
`plan-v6sys-treebuilder.md`. v6 here means:

1. Remove all gen-level detector/geometry cuts from the migration-matrix
   gen denominator. Corrected-to phase space uses ONLY kinematic event
   cuts plus species + prompt/GTR truth-source definitions.
2. Loosen the reco track Pt cut and ensure the high-Q² SIDIS trees carry
   the Lee-West / Grebenyuk-thesis tracking diagnostics (start/end
   radius, NHit CJC, track length, DCA' diagnostics).
3. Integrate H1CalcSystematic support for v6 systematics production with
   the policy:
   * HFS energy ±1% applied to the **whole HFS** with JES+RCES MERGED
     (no separate split into eHadEn and eHadEnRCES).
   * HFS theta +1 mrad.
   * Electron theta +1 mrad.
   * Electron energy variation for LAr (high-Q² electron is in LAr,
     not SpaCal).

Reference for tracking criteria: `H1/docs/h1th-700.pdf` (Grebenyuk
thesis).

## Change #1 — Gen denominator cuts (DONE)

**Status:** DONE. Forked the canonical NWfix_v2 response builder to a
v6 variant to preserve rev6 release reproducibility, then removed the
two gen-cut sites.

**Files changed:**

* New file (local + copied to NAF):
  * Local: `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_v6.py`
  * NAF: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_v6.py`
* Source script (unchanged):
  * `build_migration_th2_per_charge_NWfix_v2.py` is the rev6 canonical
    and is preserved bit-identical.

**Gen denominator policy in v6 (`compute_gen_bin` and the inlined
matched-fid in `compute_reco_bin`):**

KEPT at gen:
* Event-kinematic: `Q2_es_gen > 150`, `0.1 < Y_es_gen < 0.7`
  (`EVENT_CUTS_GEN`).
* `GenEmpz` window 35–70 GeV (`GEN_FID_OPTION_C`).
* `xFGen < 0` (target/current-region SIDIS definition).
* Species pool: `|GenPDG| ∈ {211, 321, 2212}`.
* Prompt / GTR truth-source via the V0-daughter veto
  (`IsV0DaughterGen==0` on the gen path,
  `IsMatchedV0Daughter==0` on the matched path). The H1OO matcher
  + `ProtonUtil::classifySourceGTR` + `findVetoAncestorPDG`
  populate these.
* `zGen` finite in [0,1] (numerical sanity, not a fiducial cut).

REMOVED at gen (the cuts the user identified):
* `GenPt > TRACK_CUTS_RECO["Pt_min"]` (was a symmetrization with
  reco Pt > 0.15).
* `|GenEta| < GEN_FID_OPTION_C["GenEta_abs"]` (geometric proxy;
  effective acceptance is now recovered through the response matrix
  rather than baked into the denominator).

**Validation performed:**
* `python3 -m py_compile build_migration_th2_per_charge_v6.py` →
  PARSE_OK.
* File copied to NAF; size 47895 bytes (vs 45839 bytes for NWfix_v2 —
  +2 kB of header documentation only; logic delta is ~10 lines).

**Validation NOT performed (out of scope for this dispatch):**
* Smoke run on a representative MC migration build.
* Closure / acceptance impact sanity check vs NWfix_v2.

**Downstream-script impact:**
* The v6 script writes to the **same TH2 layout** as NWfix_v2 (961×241
  Britzger encoding B, same per-charge file split). Downstream
  merge/postprocess scripts can consume v6 outputs unchanged provided
  they're pointed at the v6 output directory.
* The migration condor submitter (`submit_build_migration_per_charge*.condor`)
  must be cloned with a v6 variant pointing at `_v6.py` and a v6
  output directory before any production launch.

## Change #2 — Reco track Pt cut + r_start branches (BUILDER DONE, POLICY OPEN)

**Status:** BUILDER DONE; POLICY OPEN.

**Builder side (DONE in the existing v6sys C++ source):**

The current v6sys Stage 2 source
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6sys.C`
(version `v6sys-trackqual-2026-06-04`) already:

* Loosens the reco-track filter to "minimal sanity only": requires
  `IsFromPrimary`, `IsCentralTrk` (or combined-track in Pass 2), and
  `Pt > 0` with finite `Pt`, `Eta`, `P`. The historical hard cut at
  `Pt > 0.15` is NOT applied at builder level — it lives downstream
  in the response-builder Python (`TRACK_CUTS_RECO["Pt_min"]`).
* Persists Lee-West / QHQT diagnostic branches on `ChargedTot`,
  populated from the underlying `H1Track` via `H1PartCand::GetTrack()`:
  * `StartRadius` (`H1Track::GetStartRadius()`)
  * `EndRadius`   (`H1Track::GetEndRadius()`)
  * `TrackLength` (`H1Track::GetLength()`)
  * `DcaPrimeSinTheta` (`DcaPrime * sin(Theta)`)
  * `NHitCJC`, `NHitCJC1`, `NHitDedx` (already retained)
  * `DcaPrime`, `RadLength` (already retained)
  * `IsCentralTrk`, `IsCombinedTrk`, `IsFromPrimary`, `TrackType`
  * Polar angle `Theta` for offline cuts using SinTheta.
* Writes a `TrackQualityPayloadVersion="v6sys-trackqual-2026-06-04"`
  TNamed at file root so downstream consumers can distinguish files
  with populated track-quality branches from earlier v6sys outputs
  that wrote sentinel -999.

**No further H1OO source edit is required for Change #2 itself.**
Lee-West-style cuts can already be evaluated offline from these
branches without rebuilding trees.

**Policy questions for h1-physics-analyst / h1-analysis-critic
(not for h1-builder-engineer):**

* What is the new value for `TRACK_CUTS_RECO["Pt_min"]` in the v6
  Python response builder (currently 0.15)? Lower bound is constrained
  by reconstruction efficiency falling off rapidly below ~0.10–0.12
  GeV; this is a physics + Monte Carlo closure decision, not a code
  decision.
* Which subset of {StartRadius ≤ 30 cm, EndRadius depending on θ,
  NHitCJC ≥ N, TrackLength ≥ L, DcaPrime/sin(theta) ≤ D} should
  become the v6 nominal track-quality criterion vs systematic
  variation only? This is a tracking-systematics-policy question
  best answered by `h1-physics-analyst` with the Grebenyuk thesis
  recipe and the H1 internal Lee-West reference (h1th-700).
* If only the Python Pt cut changes, no new C++ build is needed —
  Change #2 reduces to a single-line edit in
  `build_migration_th2_per_charge_v6.py` (`TRACK_CUTS_RECO["Pt_min"]`).
  Defer this edit until the policy value is supplied.

**No NAF rebuild or smoke needed for Change #2 in this dispatch.**

## Change #3 — H1CalcSystematic merged HFS energy (PATCH PLAN, BLOCKED)

**Status:** PATCH PLAN. BLOCKED on h1-software API confirmation and
h1-analysis-critic policy sign-off before applying the edit.

**Where the change lives:** the v6sys H1Calc helper
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/H1CalcSystematic_v6sys.h`,
used by both Stage 1 (`TreeBuilderHighQ2_chargedsidis_v6sys`) and
Stage 2 (`ChargedSidisTreeBuilderHighQ2_v6sys`) binaries.

**Current state of the helper:**
* `electron_energy_{up,down}` → eElecEn (LAr wheel array ±0.5% on
  five wheels via `SetShiftElecEnWheel(w, 0.005)` + `UseElecSysEnergyArray`).
  This is the LAr-correct path for high-Q² and matches the user request.
* `electron_theta_{up,down}` → eElecTh (±1 mrad on three theta-positions
  via `SetShiftElecTheta(p, 0.001)` + `UseElecSysThetaArray`) PAIRED
  with eElecTrTh ±1 mrad. Matches the user request.
* `hfs_theta_{up,down}` → eHadTh ±1 mrad. Matches the user request.
* `hfs_phi_{up,down}` → eHadPh ±20 mrad. Not requested but retained.
* `hfs_jes_{up,down}` → eHadEn ±1% (alone). **DOES NOT match the user
  request** for "JES+RCES merged".
* `hfs_rces_{up,down}` → eHadEnRCES ±1% (alone). **DOES NOT match
  the user request** — user wants this folded into a single combined
  family.

**Requested change (NOT yet applied):** add a new merged family
`hfs_energy_combined_{up,down}` that simultaneously shifts BOTH
eHadEn and eHadEnRCES by ±1% in one variation. Keep the existing
`hfs_jes` / `hfs_rces` keys for back-compat / split-systematic
studies, but the v6 production wrapper will only ever request the
combined family for the HFS-energy slot.

**Proposed patch (single-file edit) — `H1CalcSystematic_v6sys.h`:**

1. Extend the audited-enums table with a synthetic "combined" family
   carried as a separate sysKey but anchored on the canonical
   primary enum (eHadEn=0). The combined family is recognised by
   `LookupFamily` so `BuildConfig` works, but the magnitude policy
   below shifts BOTH eHadEn and eHadEnRCES.

   ```cpp
   // In AuditedEnums(), add (anchored on eHadEn but tagged as combined):
   {"hfs_energy_combined",   0,  true},  // SHIFTS BOTH eHadEn + eHadEnRCES
   ```

2. Extend `V6SysScalarMagnitude` to return 0.01 for the new family
   name (purely cosmetic; the combined path will apply this magnitude
   to both enums explicitly):

   ```cpp
   if (family == "hfs_energy_combined")   return 0.01f;
   ```

3. In `InitV6SysShift`, add a branch for the combined family that
   sets BOTH magnitudes via `SetShift`:

   ```cpp
   } else if (cfg.sysFamily == "hfs_energy_combined") {
       sys->SetShift(0 /*eHadEn*/,     0.01f);
       sys->SetShift(1 /*eHadEnRCES*/, 0.01f);
   }
   ```

4. In `ApplyV6SysShift`, add a paired ShiftUp/ShiftDo for the
   combined family so both enums move in the same direction every
   event:

   ```cpp
   if (cfg.sysDir == "up") {
       sys->ShiftUp(cfg.sysFlag);
       if (cfg.sysFamily == "electron_theta") sys->ShiftUp(13);
       if (cfg.sysFamily == "hfs_energy_combined") sys->ShiftUp(1 /*eHadEnRCES*/);
   } else if (cfg.sysDir == "down") {
       sys->ShiftDo(cfg.sysFlag);
       if (cfg.sysFamily == "electron_theta") sys->ShiftDo(13);
       if (cfg.sysFamily == "hfs_energy_combined") sys->ShiftDo(1 /*eHadEnRCES*/);
   }
   ```

   Note: the primary enum for the combined key is eHadEn (=0), so the
   first `ShiftUp(cfg.sysFlag)` already moves the JES portion; the
   added `ShiftUp(1)` then moves the RCES portion in the same step.

**Why this is BLOCKED, not applied:**

* h1-software (or runtime-read h1-software) must confirm that
  `SetShift(eHadEn, 0.01); SetShift(eHadEnRCES, 0.01); ShiftUp(eHadEn);
  ShiftUp(eHadEnRCES);` is the H1-precedent idiom for a coherent
  combined HFS-energy shift, rather than a hidden master switch
  (e.g. an enum `eHadEnCombined` we did not see in the audited
  header, or a `UseHadSysCombined`-style array enabler). The
  audited header lists eHadEnCor=2 and eHadEnUnc=3 but none of
  those describe a JES+RCES merger. The deployed h1-software agent
  in `.opencode/agents/h1-software.md` must be consulted.
* h1-analysis-critic must explicitly sign off that "JES+RCES merged
  ±1%" is the v6 production policy, given that the previously
  audited v6sys lineage exposed them as separate sources. Switching
  to a merged source removes a correlation freedom that may matter
  for the covariance.
* Without those two gates, applying this patch and producing v6
  systematic trees would conflict with the v6sys H1-precedent
  manifest in `docs/methodology/v6_h1_precedent_systematics_manifest_*`
  and could silently corrupt the existing v6sys campaign.

**No NAF compile or smoke for Change #3 in this dispatch.**

## Production handoff checklist (for h1-naf-operator AFTER smoke passes)

This list is the post-smoke-pass production-readiness gate. It is NOT
authorising production now.

1. Confirm Change #1 v6 script is on NAF at
   `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_v6.py`.
2. Confirm the v6 Python script's `TRACK_CUTS_RECO["Pt_min"]` reflects
   the physics-analyst-approved v6 Pt cut value (currently 0.15, same
   as rev6).
3. Confirm v6sys C++ helper has the `hfs_energy_combined` family
   approved by h1-software and h1-analysis-critic and that the
   binary has been rebuilt with `make bin` and a one-data + one-MC
   smoke pair has been run, schema audited (`StartRadius`,
   `EndRadius`, `TrackLength`, `DcaPrimeSinTheta`, `NHitCJC`,
   `NHitCJC1`, `NucliaWeight`, etc.), and shifted vs central diffs
   verified to move the expected reconstructed distributions in the
   expected direction for at least one HFS-energy shift and one
   electron-energy shift.
4. Confirm v6sys output autorouting writes to
   `output/v6/<sampleclass>/<syskey>/...` (or chosen v6 directory
   convention — distinct from v5g and v6sys central paths) with no
   collision against rev6 v5g production outputs.
5. Confirm Condor submit templates and wrapper scripts for the v6
   production are present and dry-run validated for at least
   `central`, `hfs_energy_combined_up`, `hfs_energy_combined_down`,
   `hfs_theta_up`, `hfs_theta_down`, `electron_theta_up`,
   `electron_theta_down`, `electron_energy_up`,
   `electron_energy_down`.
6. Confirm storage-footprint estimate has been refreshed against the
   audit-backed ~46.8% MC savings target (v6sys plan §"Storage
   Planning Anchored To Local Audit Numbers"), with the v6 builder
   schema additions (TrackLength, DcaPrimeSinTheta, etc.) accounted
   for.

## Specialist follow-ups

* `h1-software` (deployed in `.opencode/agents/`): confirm
  combined-HFS-energy idiom for Change #3.
* `h1-analysis-critic`: policy review on Change #3 (JES+RCES merging)
  and on Change #2's new Pt cut value.
* `h1-physics-analyst`: choose v6 `Pt_min` for the Python response
  builder and the Lee-West nominal cut subset.
* `h1-builder-engineer`: once Change #3 is approved, apply the
  `H1CalcSystematic_v6sys.h` patch, recompile with `make bin`, and
  smoke-test single data + single MC file with `--sys-key
  hfs_energy_combined_up`.
* `h1-qa-specialist`: audit v6 Python output trees once Change #1 is
  smoke-run, confirming gen denominator integral grows in the
  expected direction relative to NWfix_v2 (broader gen acceptance →
  larger denominator → lower per-bin efficiency, recovered through
  the response matrix).
* `h1-naf-operator`: production-side rollout AFTER all smoke and
  critic gates pass.

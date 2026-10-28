<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Research: High-Q2 Charged-Hadron SIDIS — Status Snapshot for Incoming Supervisor

**Date:** 2026-05-19
**Scope:** Present-state snapshot of the H1 inclusive charged-hadron SIDIS
analysis in the high-Q2 regime (`Q2 > 150 GeV2`) only. Read-only research.
This document does NOT propose a plan, fix, or sequence; it documents what
exists, what is blocked, and what is undocumented as of today.
**Audience:** New `h1-sidis-supervisor` or planner taking over the high-Q2
SIDIS track ("Paper 2" in the two-paper SIDIS program).
**Source files reviewed:** see Section 9.

> The SIDIS `context-handoff.md` and most planning documents under
> `H1/sidis_lowQ2/` are SHARED between low-Q2 and high-Q2. This snapshot
> extracts only the high-Q2 content. It does NOT re-summarize low-Q2 work.

---

## 1. Executive Summary

- **Production state:** v5b builder is FINAL and compiled
  (`/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` —
  the builder binary retains its legacy proton-prefixed name from the v5b
  SIDIS fork; the physics is inclusive charged-hadron SIDIS. See
  `docs/references/reference-naming-aliases-highQ2-sidis.md` for the alias table).
  v5b high-Q2 MC production is reported complete: **1086 MC jobs** per
  `context-handoff.md` line 5; live DUST listing shows
  724 RAPGAP31 + 486 DJANGOH14 = **1210 per-run MC files** under
  `H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/`.
- **Merged MC on /pnfs:** two single-file hadd merges exist at
  `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/`:
  `merged_highQ2_mc_rapgap31.root` (~47 GB) and
  `merged_highQ2_mc_djangoh14.root` (~24 GB), both safely under the
  ROOT 100 GB TTree limit (verified 2026-05-19).
- **Schema (v5b):** `ChargedTot=86` / `ChargedTotGen=49` / `DisTot=48`
  branches for high-Q2 (per handoff line 838-842).
- **Data:** ep0607-only at **140,306 nb^-1**. Missing periods `ep0304`
  (53,706 nb^-1), `em0405` (105,028 nb^-1), `em06` (57,573 nb^-1).
  Total **recoverable** S67 lumi = **356,613 nb^-1** (39% currently in).
- **Data per-run outputs and merged data file:** NOT verified on DUST/pnfs
  for SIDIS_HighQ2 in the current snapshot — DUST subdirs for data periods
  exist as empty shells (e.g. `output/data/ep0304/`, `ep0607_07/`); no
  merged high-Q2 data ROOT file is visible under `highQ2_merged/`.
  (See Section 2 and Section 10 for the explicit gap.)
- **Recommended primary observable (per handoff and r3 plan):**
  charge-separated multiplicities `M^{h+/-}(Q2, x, z, pT_h)`,
  scoped as an **internal-only companion** by default; promotion to
  publication-grade requires end-to-end H1CalcWeight / effective MC
  luminosity validation (handoff "Frozen Analysis Decisions" line 1011;
  `plan-finish-sidis-analysis-r3.md` lines 6-7, 98, 514-517).
- **NOT recommended yet:**
  (a) High-Q2 charge asymmetry `A_h` — BLOCKED by default by `OQ7`;
      cannot enter publication scope until missing e- periods are recovered
      AND user-approval changes the default-blocked policy
      (handoff line 634; r3 plan lines 99, 103, 517, 525-526, 541).
  (b) Absolute hadron cross-sections — BLOCKED until H1CalcWeight/effective
      MC luminosity validated end-to-end (r3 plan lines 39, 514-516, 527).
  (c) `phi_h` azimuthal moments at high-Q2 — no high-Q2-specific phi_h
      study artifact exists in `H1/sidis_lowQ2/docs/`. Gen-electron
      branches DO exist in v5b high-Q2 trees, so the low-Q2 reco-phi-as-gen
      proxy hack is NOT needed at high-Q2 (handoff line 423-424 + v5b
      "Branches added" line 845-847; `research-highQ2-sidis-extension.md`
      Section 9 item 1).
- **Trigger:** S67 (LAr NC) enforced for high-Q2 **data only**; MC has
  no trigger requirement at the event-list level. Nominal trigger
  weighting is `TrigWeightAC` (handoff lines 1019-1028).
- **Electron detector:** LAr (`4° < theta_e < 154°`), structurally different
  from low-Q2 SpaCal. The high-Q2 LAr electron-scale systematic procedure
  is documented as `OQ3`/`SB4` and remains **unresolved**
  (`research-highQ2-sidis-extension.md` Sections 4 and 9 item 4).

---

## 2. Production State

### 2.1 Builder

- **Binary:**
  `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2`
  (compiled, smoke-tested at v5b — handoff line 884).
- **Source family:** v5b builder, derived from the high-Q2 fork of
  `ProtonTreeBuilder` family. Per `research-highQ2-sidis-extension.md`
  Sections 3.1-3.3, the high-Q2 builder is a structural fork (not a
  descendant) of the low-Q2 builder, sharing the charged-particle
  remediation work but **owning DIS selection / hadronic calibration
  upstream** (i.e., from `H1BaryonAsym_HighQ2` evls files), not inline.
- **v5b schema (per handoff lines 836-842, verbatim):**

  | Tree | Low-Q2 | High-Q2 |
  |------|--------|---------|
  | ChargedTot | 86 branches | **86 branches** |
  | ChargedTotGen | 44 branches | **49 branches** |
  | DisTot | 50 branches | **48 branches** |
  | ProtonTot / ProtonTotGen | REMOVED | REMOVED |

  Confirmed numeric schema for high-Q2 = **86 / 49 / 48** (matches the
  user-stated values in the supervisor's instructions). The supervisor's
  prompt cited "86/49/48"; the handoff text is consistent.

- **v5b branches added vs v3.1 (handoff lines 844-866, applies to both
  regimes unless flagged):** `GenElecPhi`, `GenElecTheta`, `GenElecE`
  (per-event, broadcast per-track on `ChargedTot`); `GenEmpz`; `GenVtxZ`;
  `IsV0DaughterGen` (on `ChargedTotGen`); `NucliaWeight`; trigger bits
  `L1L2L3ac[4]` and `L1L2L3rw[4]` in gtustin packing; `TrigWeightAC`,
  `TrigWeightRW`; `DcaPrimeSinTheta`, `StartRadius`, `EndRadius`
  (sentinel-only because `H1PartSelTrack` does not expose track-radius
  APIs at the needed level).
- **v5b branches removed:** `ProtonTot`/`ProtonTotGen` trees; 10 dE/dx PID
  branches from `ChargedTot` (`DedxRaw`, `Dedx{Pion,Kaon,Proton}`,
  `Lk{Pion,Kaon,Proton}`, `Nlh{Pion,Kaon,Proton}`); hard quality cuts on
  `ChargedTot` (store broadly, cut downstream).

### 2.2 Job campaign

- **MC:** Handoff line 5 + line 893 — "**1086 MC jobs** submitted and
  completed with v5b builder. Outputs on DUST." Live listing on
  `naf-hone` (2026-05-19) under
  `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/`:
  - `rapgap31/`: subdirs `7071`, `7094`, `7096`, `7097`,
    total **724 per-run `.root` files** (found via `find -maxdepth 2 -name "*.root"`).
  - `djangoh14/`: subdirs `7072`, `7091`, `7092`, `7093`,
    total **486 per-run `.root` files**.
  - Sum = **1210** per-run files vs handoff's "1086 MC jobs" — small
    discrepancy, likely files-per-job multiplicity vs job count.
    (inference) Either the handoff job count is jobs while DUST holds
    per-chunk output files, or some files come from a partial earlier
    campaign. Not authoritative resolution available; flag for validation.
- **Data:** Handoff (line 893) does not state a data job count for v5b
  high-Q2. Per `research-highQ2-sidis-extension.md` Section 2.1, the
  previous proton-driven campaign had **635 data files** (ep0607 only).
  DUST listing today shows `H1SIDIS_HighQ2/output/data/` contains
  subdirs `ep0304/`, `04/`, `05/`, `06/`, `07/`, `em06/`, `ep0607_06/`,
  `ep0607_07/` plus `proton_highQ2_data_smoke.root`; `ep0304/` is empty
  and the per-period subdir populations were not enumerated in this
  snapshot. (inference) Data production for SIDIS_HighQ2 v5b appears
  incompletely landed on DUST; this is a gap in evidence (see Section 10).

### 2.3 Storage paths

| Path | Contents (verified 2026-05-19) |
|------|-------------------------------|
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/` | Code root for high-Q2 SIDIS package (`src/`, `scripts/`, `steering/`, `output/`, etc.) |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/` | DUST per-run MC outputs (724 + 486 ROOT files) |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/data/{ep0304,04,05,06,07,em06,ep0607_06,ep0607_07}/` | Period-tagged data dirs (most empty in current snapshot; only `smoke` file at top level) |
| `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/` | **2 merged MC files** (see below) |
| `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_trees/` | Subdirs `kaon/`, `lambda/` — not SIDIS charged-hadron content |
| `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_v4/` | `mc/{rapgap31,djangoh14}/` (mostly empty, only 1-2 chunk dirs); `data/` empty; `merged/` empty |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/` | Upstream package: event-list (evls) source for the high-Q2 charged tree builder |

### 2.4 Merge status

Merged outputs (verified 2026-05-19 listing of `highQ2_merged/`):

| File | Size | Notes |
|------|------|-------|
| `merged_highQ2_mc_rapgap31.root` | ~47 GB (49,995,656,580 B) | Below the ROOT 100 GB TTree limit |
| `merged_highQ2_mc_djangoh14.root` | ~24 GB (24,755,592,177 B) | Below the ROOT 100 GB TTree limit |

**No high-Q2 merged data file is visible** in `highQ2_merged/`.
Per handoff line 276 ("Merge complete") this is recorded as complete,
but the live evidence only confirms MC merges. (See Section 10.)

### 2.5 ROOT 100 GB hadd constraint

Shared infrastructure rule (handoff "Mistakes Made #12" / CONTEXT.md
line 80-84): per-run files MUST NOT be merged with `hadd` if the
resulting TTree exceeds ~100 GB; ROOT silently truncates or fails.
The high-Q2 MC merges sit at 47 GB and 24 GB — under the ceiling
and therefore acceptable for `hadd`. The low-Q2 v5b chain has had to
adopt uproot multi-file iteration because its merged trees exceed the
ceiling; for high-Q2, the merged single-file form is presently usable.

---

## 3. Luminosity and Beam-Charge State

### 3.1 Per-period S67 lumi inventory (from
`research-highQ2-sidis-extension.md` Section 2.2, table at lines 154-160)

| Period | Beam | S67 lumi (nb^-1) | Currently processed | Recoverable |
|--------|------|------------------|---------------------|-------------|
| ep0304 | e+ | 53,706 | NO (missing `/03/` + wrong evls in `/04/`) | YES |
| em0405 | e- | 105,028 | NO (wrong good-run file) | YES |
| em06 | e- | 57,573 | NO (split-`/06/` bug) | YES |
| ep0607 | e+ | **140,306** | **YES (already in)** | (already in) |
| **Total recoverable** |  | **356,613** | **140,306 (39%)** | **+216,307 (61%)** |

### 3.2 Currently included data periods

Only **ep0607** (e+p, 140,306 nb^-1) is in the high-Q2 data sample.
Both `em0405` and `em06` (e-p periods) are absent. `ep0304` (additional
e+p) is also absent.

### 3.3 Recovery root cause (per
`proton_HiQ/docs/research-data-coverage-gap.md` as cited in
`research-highQ2-sidis-extension.md` Section 2.2)

The recovery is a **configuration fix**, not a missing-data problem:
1. Add `/03/` to the `DATA_PERIODS` map in `submit_full_production.py`.
2. Split `/06/` into `em06` (chunks c060220-c060319) and `ep0607`
   (chunks c060000-c060212) sub-ranges.
3. Re-point proton/SIDIS steering files at the period-tagged evls
   names (the correct event lists already exist in
   `H1BaryonAsym_HighQ2/output/evls/data/`).
4. Re-run the proton/SIDIS stage on ~700 additional Condor files.

### 3.4 Implication for A_h vs multiplicities

Beam-charge asymmetry `A_h = (N+ - N-) / (N+ + N-)` requires BOTH e+p
and e-p data for a meaningful, lepton-charge-symmetric measurement (or
at minimum requires explicit treatment of the missing e- exposure).
With only ep0607 in hand, the e+/e- imbalance is **complete** (zero e-).

This is the explicit basis for OQ7's default-blocked policy on high-Q2
`A_h` for publication. Multiplicities `M^{h+/-}` are not subject to the
same e+/e- balance requirement and can in principle be computed on the
ep0607-only sample; however, period-stability systematics
(period-split source in low-Q2) cannot be evaluated until e-
recovery, since "period_split" requires multiple periods to compare
(handoff Section 9 line 580 + `reference-sidis-systematics.md` lines 23-24).

---

## 4. Observable Feasibility (per handoff + r3 plan + extension research)

### 4.1 Multiplicities `M^{h+/-}(Q2, x, z, pT_h)`

- **Relative status:** Feasible to compute on the existing v5b high-Q2
  merged MC + ep0607 data once data merge is confirmed. r3 plan F6.2
  names this as the high-Q2 nominal chain to be built first
  (lines 524, 558-559 of r3).
- **Publication promotion gate:** Requires **end-to-end H1CalcWeight /
  effective MC luminosity validation** to pass F6.5 (r3 plan
  lines 515-516, 527). If that gate does not pass, Paper 2 stays
  **internal-only companion** with an explicit promotion path.
- **Acceptance:** The acceptance framework `tmd_resolution_acceptance.py`
  exists for low-Q2 and is structurally reusable
  (`research-highQ2-sidis-extension.md` Section 6.5: "Reuse the low-Q2
  acceptance machinery essentially unchanged"). No high-Q2-specific 4D
  acceptance map artifact is documented in `H1/sidis_lowQ2/docs/`
  (see Section 10).
- **Binning:** Low-Q2 uses Scheme A `4×4×4×6 = 384 bins`. The high-Q2
  4D grid is expected to need **coarser binning** because per-bin
  statistics will be ~3-5× lower per cell
  (`research-highQ2-sidis-extension.md` Section 6.5). No high-Q2-specific
  occupancy or binning study exists in `H1/sidis_lowQ2/docs/` (gap,
  Section 10).

### 4.2 Charge asymmetry `A_h`

- **BLOCKED by default for publication** (r3 plan line 39, 99, 525-526;
  handoff Open Questions table line 634).
- **Blocker name:** **OQ7** ("May high-Q2 A_h be released provisionally
  before e- recovery?"). Default policy is "blocked for publication"
  (r3 plan F6.3). The default cannot change without **explicit user
  approval** (decision D5 in r3 plan line 633).
- **Even with OQ7 approval**, publication-grade A_h additionally requires
  e- period recovery (F6.4) AND normalization closure (F6.5)
  (r3 plan line 541).
- (inference) Internal-only A_h on ep0607-only is technically computable
  but is not a meaningful physical asymmetry result since it would
  fold beam-charge asymmetry into hadron-charge asymmetry. Handoff
  "Mistakes #" does not list this explicitly but `reference-sidis-systematics.md`
  line 24 ("the period or beam-charge split as a target source that is
  currently blocked") and line 224 are consistent with this reading.

### 4.3 Absolute hadron cross-sections

- **BLOCKED.** r3 plan lines 514-517 list "Publication-grade high-Q2
  multiplicity release without H1CalcWeight / effective MC luminosity
  validation" as an automatic F6 failure. By implication, **absolute
  hadron cross-sections** (which require the absolute normalization)
  are also blocked.
- The r3 plan keeps absolute hadron cross-sections explicitly out of
  high-Q2 publication scope unless later normalization closure promotes
  them (lines 39, 67, 171, 524).
- Underlying technical blocker is `HB5` in
  `research-highQ2-sidis-extension.md` Section 7.1: "H1CalcWeight not
  integrated → no absolute normalization." This was originally documented
  in `proton_HiQ/docs/research-mc-normalization-highQ2.md` (not re-read
  for this snapshot).

### 4.4 Azimuthal moments (`A1 cos phi_h`, `A2 cos 2 phi_h`)

- **Feasibility (per the schema):** v5b high-Q2 trees store
  `GenElecPhi`, `GenElecTheta`, `GenElecE` per event on `ChargedTot`
  (handoff lines 844-847). This eliminates the low-Q2 gen-phi
  reconstruction problem and the reco-phi-as-gen proxy hack
  (handoff Mistakes #7 + #8 and Section 6 "phi_h Gen-Level Computation
  Bug" lines 402-417).
- **Status (per the documents reviewed):** No high-Q2-specific phi_h
  study, acceptance, fit, or systematics artifact exists in
  `H1/sidis_lowQ2/docs/`. The phi_h work (`compute_phih_systematics.py`,
  `plot_phih_per_4d_bin.py`, etc.) and its remaining issues (handoff
  lines 941-961) are explicitly low-Q2.
- **Implication:** A high-Q2 phi_h measurement is **structurally
  unblocked at the tree level**, but no implementation, plot, fit, or
  systematic exists yet for high-Q2. Documentation gap, Section 10.

---

## 5. Schema and Tree Differences vs Low-Q2

### 5.1 v5b branch-count delta (verbatim from handoff lines 838-842)

| Tree | Low-Q2 | High-Q2 | Delta |
|------|--------|---------|-------|
| `ChargedTot` | 86 | 86 | 0 |
| `ChargedTotGen` | 44 | **49** | **+5 on high-Q2** |
| `DisTot` | 50 | **48** | **-2 on high-Q2** |

The handoff itself does not enumerate which 5 extra `ChargedTotGen`
branches exist at high-Q2 vs low-Q2, nor which 2 `DisTot` branches
are present at low-Q2 but absent at high-Q2. (inference) Likely
candidates for the high-Q2 extra `ChargedTotGen` branches are the
gen scattered-electron 4-vector pieces that the low-Q2 HB6 trees did
not previously expose (see handoff lines 418-424 noting the high-Q2
trees historically stored gen electron branches that low-Q2 lacked).
This inference is consistent with `research-highQ2-sidis-extension.md`
Section 9 item 1 ("Does the v3 high-Q2 tree already have all
gen-electron branches needed for 4D photon-axis pT_h acceptance?
Task prompt says yes — 28 ChargedTotGen branches").

### 5.2 Branches confirmed present in high-Q2 trees

- Gen scattered electron 4-vector on `ChargedTotGen` (`GenElecPx/Py/Pz/E`,
  `GenElecTheta`, `GenElecPhi`, `GenElecEn`) per the v5b additions
  documented in `plan-builder-update-combined-tracks.md` Section 9
  and the handoff line 845-847.
- `MatchAngle` (per `research-highQ2-sidis-extension.md` Section 2.3,
  referencing builder line 348).
- v5b shared additions (handoff lines 844-866): `GenElecPhi/Theta/E`,
  `GenEmpz`, `GenVtxZ`, `IsV0DaughterGen`, `NucliaWeight`, trigger bits
  `L1L2L3ac[4]`/`L1L2L3rw[4]`, `TrigWeightAC`/`TrigWeightRW`,
  `DcaPrimeSinTheta`, `StartRadius`/`EndRadius` (sentinel).

### 5.3 High-Q2-specific tree differences vs low-Q2

- **Gen electron content was historically richer at high-Q2.** Low-Q2
  HB6 trees did NOT store gen electron 4-vectors pre-v5b; high-Q2 v3
  trees did (handoff lines 418-424; `research-highQ2-sidis-extension.md`
  Section 9 item 1). v5b unifies this by adding `GenElec*` to both,
  but the structural origin and the historical gap still characterize
  the regimes.
- **DIS-method storage history differs.** Per
  `research-highQ2-sidis-extension.md` Sections 2.3 and 3.1:
  - At high-Q2, DIS selection and hadronic calibration happen
    **upstream of the builder** (in the `H1BaryonAsym_HighQ2` evls
    stage). The high-Q2 builder reads pre-selected events and
    explicitly **skips `CalibrateHadrooII`** (builder lines 175-178).
  - The high-Q2 builder historically read only `Q2s, Ys` from HAT
    (and computed `xBj` from them); pre-v5b it lacked the full
    `Q2e/Ye/Xe`, `Q2da/Yda/Xda`, `Q2es/Yes/Xes`, `Q2h/Yh/Xh` set that
    the low-Q2 builder stores. (v5b status: see Section 5.4 below.)
  - The `Q2s`/`Ys` labels on the high-Q2 trees historically already
    referred to **eSigma-calibrated** values (because eSigma is the
    upstream calibrated kinematic choice at high-Q2 — see
    `research-highQ2-sidis-extension.md` Section 3.2). This is
    structurally different from low-Q2 where `Q2s/Ys` are true Sigma
    values. The user-facing implication is that for high-Q2 the
    OQ8 method-comparison story is "essentially one frozen best
    method (eSigma)" rather than a real choice
    (`research-highQ2-sidis-extension.md` Sections 3.2-3.3, 5.4).
- **`inFiducialCharged` definition.** Per
  `research-highQ2-sidis-extension.md` Section 2.3, the high-Q2 builder
  (pre-v5b in that document) used the **analysis-level** cuts
  (`|GenEta| < 1.3 && GenPt > 0.15`, builder line 718), NOT the broad
  low-Q2 gen fiducial (`|GenEta| < 3.5, GenPt > 0.05`). Whether v5b
  brings high-Q2 in line with the low-Q2 broad fiducial is NOT
  re-stated in the handoff text. (inference) The handoff v5b summary
  mentions "Hard quality cuts on ChargedTot (store broadly, cut
  downstream)" applied generically, but does not explicitly confirm
  the broad gen fiducial was adopted at high-Q2. Flag for validation.
- **`ParticleSource_cg`** on `ChargedTotGen`: per
  `research-highQ2-sidis-extension.md` Section 2.3, this branch was
  NOT present in the high-Q2 `ChargedTotGen` pre-v5b. The handoff does
  not list it among v5b additions for high-Q2 either. (inference) Verify
  whether it was added in v5b parity work; gap if unconfirmed.
- **Trigger handling (data only).** S67 (LAr NC) is enforced for
  high-Q2 **data only** at the event-list level
  (handoff lines 1019-1023). Low-Q2 event lists have no trigger
  requirement. MC has no trigger cut in either regime; trigger
  weighting via `TrigWeightAC` is the nominal policy.
- **Electron detector / fiducial.** LAr-based reconstruction with
  `4° < theta_e < 154°` (`research-highQ2-sidis-extension.md` Section
  1.5; `research-highQ2-proton-asymmetry.md` referenced therein),
  vs SpaCal-based backward calorimetry at low-Q2.
- **Event-level cuts.** Per `research-highQ2-sidis-extension.md`
  Section 1.1 table:

  | Cut | High-Q2 | Low-Q2 |
  |-----|---------|--------|
  | Q2 range | 150 -- ~20,000 GeV2 | 5 -- 100 GeV2 |
  | y range | 0.1 -- 0.9 | 0.1 -- 0.6 |
  | E - p_z | [45, 65] GeV | [35, 70] GeV |
  | abs(VtxZ) | < 35 cm | < 30 cm |
  | Trigger | S67 (LAr NC) | S61 (SpaCal DIS) |
  | Electron | LAr | SpaCal |

### 5.4 What v5b did or did not bring at high-Q2

The handoff's v5b additions section (lines 844-866) is regime-agnostic
in tone but refers to "Combined tracks supported (TrackType=1 central,
2=combined)" and the new gen-electron / trigger / NuclIA additions as
present in the v5b builders for **both** low-Q2 and high-Q2 (since both
binaries were recompiled and produce the v5b schema). Live confirmation
of the exact set of high-Q2 `ChargedTot=86 / ChargedTotGen=49 / DisTot=48`
branch contents was not performed in this snapshot. The branch lists
in `plan-builder-update-combined-tracks.md` describe the **low-Q2**
target schema explicitly; the implied "high-Q2 parity" deltas are
described in Sections 14 ("Low-Q2 vs High-Q2 Schema Parity Table",
lines 733-763 of that plan) but are pre-v5b numerics and should not be
read as v5b-current. (Verification gap, Section 10.)

---

## 6. Inherited Tooling from Low-Q2

Pulled primarily from `research-highQ2-sidis-extension.md` Section 6 and
handoff Section 5 / Section 12 ("Analysis Scripts").

### 6.1 Directly reusable for high-Q2 (per the extension research)

- `tmd_analysis.py` — TMD observable computation and 4D occupancy.
  Reusable per `research-highQ2-sidis-extension.md` Section 6.5 ("Reuse
  the low-Q2 acceptance machinery essentially unchanged"). The GenElecPhi
  fix has been applied (handoff line 968).
- `tmd_resolution_acceptance.py` — Resolution, purity/stability, 4D
  acceptance correction. Same reuse note.
- `tmd_unfold.py` — 4D unfolding driver. (inference) reusable in form;
  the binning grid will need to be reset for high-Q2.
- `extract_multiplicities.py` — multiplicity and `A_h` extraction via
  uproot per-run iteration (handoff line 971); the GenElecPhi
  numerator/denominator inconsistency was fixed here. Reusable in
  function but file lists, lumi, and trigger handling are regime-specific.
- `extract_azimuthal_moments.py` — A1/A2 extraction. Reusable; gen
  electron source can now be the stored `GenElecPhi` branch directly
  at high-Q2 (no proxy needed).
- `compute_systematics_charged.py`, `compute_phih_systematics.py`,
  `compute_model_systematic.py` — systematics drivers. Reusable for
  shared sources; LAr electron-scale source must replace SpaCal sources
  in the high-Q2 chain (`research-highQ2-sidis-extension.md` Sections
  4 and 7.1 HB7).
- `produce_shifted_mc.py` + `.C` — shifted-MC production for detector
  systematics. **Caution:** the shift magnitudes were validated for SpaCal
  (`±1% energy, ±1 mrad angle`) and HFS (`±2%`); the LAr scale is
  position-dependent (1-5% across 5 z-impact regions) and is NOT
  a single-value post-hoc shift (`research-highQ2-sidis-extension.md`
  Section 4.2, item 2). Direct reuse for LAr is incorrect.
- `validate_v3_output.py` — QA validator (despite the name validates
  v5b outputs per handoff line 981). Reusable for high-Q2 schema check
  with updated expected counts (86/49/48).
- `study_migration_background.py`, `plot_in_vs_out_phase_space.py`,
  `study_binning_purity.py` — reusable.
- `merge_and_validate.py` — partially obsolete due to the 100 GB
  limit at low-Q2; for high-Q2 the merged single-file form is currently
  usable and `merge_and_validate.py` may still be relevant.

### 6.2 Need adaptation for high-Q2

- Anything wiring per-period luminosity, beam type, trigger choice
  (S67 vs S61), or electron-reconstruction systematics. The trigger
  policy is structurally different (S67 enforced data-side; S61 not
  cut in low-Q2). The electron-scale source is structurally different
  (LAr vs SpaCal — `reference-sidis-systematics.md` lines 183, 223-227;
  `research-highQ2-sidis-extension.md` Section 4).
- Binning. High-Q2 4D grid is expected to be coarser; no concrete
  high-Q2 binning exists in `H1/sidis_lowQ2/docs/`
  (`research-highQ2-sidis-extension.md` Section 9 item 3).
- Normalization. H1CalcWeight integration / effective MC luminosity is
  the high-Q2-specific publication gate (`research-highQ2-sidis-extension.md`
  Section 6.7 / HB5; r3 plan F6.5).
- Electron-scale systematic procedure: `OQ3`/`SB4`/`HB7` —
  authoritative procedure UNDECIDED. Possibly requires evls-stage
  re-run for both data and MC, ~1.4k jobs scaled
  (`research-highQ2-sidis-extension.md` Sections 4.2, 6.6).

### 6.3 Not directly applicable

- Low-Q2 phi_h reco-phi-as-gen proxy logic
  (handoff lines 402-417 + Mistakes #7-#8). At high-Q2, `GenElecPhi`
  is stored directly; no proxy is needed.

---

## 7. Known Traps and Embedded Supervisor Anchors Specific to High-Q2

1. **High-Q2 absolute hadron cross-sections are BLOCKED until
   normalization is validated end-to-end.** Hard gate at F6.5 of
   `plan-finish-sidis-analysis-r3.md` (lines 514-516, 527). Without
   `highQ2_normalization_validation.json` showing PASS, Paper 2 stays
   internal-only and absolute cross-sections cannot be released.
2. **Beam-charge asymmetry `A_h` is BLOCKED until e- period recovery
   completes.** Do NOT approximate `A_h` from ep0607-only. The default
   high-Q2 `A_h` publication policy is "blocked" (`OQ7`, r3 plan
   D5 line 633; handoff line 634). Even an internal-only result must
   carry an "e-p coverage = 0%" caveat.
3. **eSigma:** `Q2es ≡ Q2e` by H1CalcKine design (handoff lines
   656-661; "OQ8 Resolution (2026-05-16)" subsection). This is a
   shared low-Q2/high-Q2 design point. At high-Q2 there is the further
   structural fact that the upstream evls stage uses eSigma
   calibration, so even the `Q2s/Ys` branches store eSigma-calibrated
   values (`research-highQ2-sidis-extension.md` Sections 3.2, 5.4).
   The high-Q2 OQ8 story is therefore: one effectively-frozen "best"
   kinematic method; `_es` branch chasing is explicitly forbidden as
   a method discriminator (handoff "Frozen Analysis Decisions"
   line 1009-1010; r3 plan line 79).
4. **ROOT TTree 100 GB hadd limit (shared).** Currently the high-Q2 MC
   merges are under the ceiling (47 GB + 24 GB) and can be opened as
   single files. If the data merge ever grows beyond 100 GB the
   uproot-multifile policy must apply.
5. **Trigger:** S67 enforced for high-Q2 **data only**. MC has no
   trigger requirement. Nominal weighting is `TrigWeightAC`; **do NOT
   cut on S67 or any individual subtrigger** (handoff line 1023).
   Trigger bits are stored in gtustin format `L1L2L3ac[4]` /
   `L1L2L3rw[4]`.
6. **LAr electron-scale procedure is undocumented authoritatively
   (OQ3 / SB4 / HB7).** Do NOT transfer the SpaCal `±1%` energy scale
   number into the high-Q2 LAr regime. Per
   `reference-sidis-systematics.md` lines 183, 223-227, 229 and
   `research-highQ2-sidis-extension.md` Section 4.4, the high-Q2
   electron-scale systematic must be evaluated **independently** with
   the position-dependent LAr procedure (1-5% across 5 z-impact
   regions), not as a post-hoc 4-vector shift.
7. **NonRad MC is missing for both regimes**, so `C_rad = 1` is the
   placeholder at high-Q2 (per
   `research-highQ2-sidis-extension.md` Section 2.6 and Section 6.8).
   This is a cross-regime issue, not high-Q2-specific.
8. **DUST quota is shared** at ~92% (CONTEXT.md line 73). Any new
   high-Q2 production must respect storage routing rules
   (CONTEXT.md lines 86-97).

---

## 8. Pending Decisions and Open Questions for High-Q2

### 8.1 From the handoff Open Questions table (line 632-637)

| ID | Question | Blocks publication? |
|----|----------|---------------------|
| **OQ7** | May high-Q2 `A_h` be released provisionally before e- recovery? | YES, blocks high-Q2 `A_h` only. Default = blocked. (r3 plan D5) |
| OQ8 | Which DIS method should be nominal for each SIDIS variable? | Resolved 2026-05-16 (handoff lines 656-674). At high-Q2 the "method comparison" is structurally moot because the upstream eSigma calibration leaves effectively one method. |
| OQ9 | Should gen-electron branches be added to ChargedTotGen for low-Q2? | Resolved by v5b for both regimes (`GenElec*` branches added). |
| OQ10 | What phi_h convention should be frozen for publication? | Applies to both regimes equally if phi_h is ever released. |

### 8.2 From `research-highQ2-sidis-extension.md` Section 9 ("Areas
Needing Further Investigation")

1. Does the v5b high-Q2 tree contain all gen-electron branches needed
   for 4D photon-axis pT_h acceptance? Live verification still needed
   per item 1.
2. What is the actual data event count after e- recovery?
   ~195k additional events estimated but not reconciled with evls
   EvlTree counts (item 2).
3. What 4D bin grid is statistically supported at high Q2?
   No high-Q2 occupancy study exists (item 3).
4. What is the authoritative LAr electron-scale variation procedure?
   `OQ3` / `HB7`, unresolved (item 4; `reference-sidis-systematics.md`
   line 507).
5. Is the bounded F3/F6/F7 charge-mismatch leak (0.76% in pre-v5b
   high-Q2 inspection) acceptable for publication asymmetries, or must
   it be closed first? The low-Q2 remediation reduced the analogous
   leak below 0.1%; high-Q2 publication threshold is not documented
   (item 5). (inference) v5b may have brought high-Q2 to the same
   leak level as low-Q2, but no v5b-specific leak number is recorded.
6. Will diffractive-MC processing happen for high-Q2 alongside low-Q2?
   `x_diffr_flag` thresholds for high-Q2 not calibrated (item 6).
7. Should LAr electron-scale variation be applied via evls re-run, or
   is there a faster post-hoc analytic proxy? Affects whether HB7
   takes weeks or months (item 7).

### 8.3 From r3 plan decision register

- **D3:** Descope Paper 2 (high-Q2) to internal companion if normalization
  closure fails. Default = yes, descope (r3 plan line 631).
- **D5:** OQ7 — allow provisional high-Q2 `A_h` release? Default = no
  (r3 plan line 633).

### 8.4 From `reference-sidis-systematics.md`

- (line 507) "What is the authoritative high-Q2 electron-reconstruction
  variation procedure and numerical scale in the LAr regime?" — OQ3.
- (line 523) "For high-Q2 A_h, is any provisional statement allowed
  before e- recovery is completed?" — OQ7.

---

## 9. List of High-Q2-Relevant Local Documents Reviewed

| Path (under `H1/`) | High-Q2 relevance |
|--------------------|-------------------|
| `CONTEXT.md` | Shared infrastructure (NAF, ssh, storage, ROOT 100 GB limit, Condor); lists "Charged SIDIS (high-Q2)" status = "v5b production complete; MC reprocessing done, analysis pending" (line 22) |
| `sidis_lowQ2/context-handoff.md` | The SIDIS handoff is SHARED. Authoritative for: v5b schema 86/49/48 (line 5, 840-842), 1086 MC jobs (line 5, 893), `_es` resolution, NuclIA, trigger policy, phi_h gen bug + resolution, frozen decisions, Q2sGen=0 issue, OQ7 (line 634), trigger S67 (line 1019-1028). The high-Q2 callouts are scattered through Sections 5, 6, 9 (Open Questions), 11 (Mistakes), and "Session End State". |
| `sidis_lowQ2/docs/research/research-highQ2-sidis-extension.md` | **Primary high-Q2 scoping document.** 771 lines. Covers kinematic coverage, data periods + recovery, builder gap analysis vs low-Q2 (pre-v5b), LAr vs SpaCal systematics, TMD physics motivation, production roadmap (Sections 6.1-6.9), blocker ledger HB1-HB8 (Section 7.1), risk register R1-R9 (Section 7.2), recommendation (one program, two papers, Section 8), open items Section 9. Status note at top: "high-Q2 builder parity is now updated and the high-Q2 merge on `/pnfs` is complete." |
| `sidis_lowQ2/docs/plans/plan-finish-sidis-analysis-r3.md` | Latest planning artifact. Treats high-Q2 explicitly: F6 phase (lines 499-544) is the entire high-Q2 close-out; F6.1-F6.9 tasks; Paper 2 DoD; decisions D3 + D5; hard-fail conditions on H1CalcWeight + OQ7 + e- recovery (lines 514-517). Two-paper strategy: low-Q2 first, high-Q2 companion (lines 4-7). |
| `sidis_lowQ2/docs/plans/critique-plan-finish-sidis-analysis-r3.md` | Critique of r3, verdict APPROVE-WITH-MINOR-REVISIONS. Confirms high-Q2 protection mechanisms intact (line 68-72). |
| `sidis_lowQ2/docs/plans/plan-finish-sidis-analysis-r1.md`, `-r2.md`, `critique-r1.md`, `critique-r2.md` | Historical iterations of the same plan; superseded by r3 but show evolution of high-Q2 treatment. |
| `sidis_lowQ2/docs/plans/plan-convergence-to-publication.md` | Master convergence plan. Section "C11 high-Q2 electron-scale closure" (lines 144-146, 172, 615-630, 813). States "low-Q2 drives the first paper; high-Q2 is companion/preliminary until electron-scale and period-recovery issues close" (line 74). Strong recommendation "do not block low-Q2 on simultaneous high-Q2 publication" (lines 725, 817). |
| `sidis_lowQ2/docs/plans/plan-builder-update-combined-tracks.md` | Builder/schema source of truth. Primarily low-Q2 focused but contains the explicit "Low-Q2 vs High-Q2 Schema Parity Table" (Section 14, lines 733-763). Note these parity counts are **pre-v5b** numerics ("Current high-Q2 baseline" = 82 / 35) and should not be confused with the v5b 86/49/48 numbers. |
| `sidis_lowQ2/docs/references/reference-sidis-systematics.md` | Systematics playbook covering both regimes. High-Q2 source table at lines 145-156 (LAr electron, ZEUS-style border effects, S67 trigger verification, etc.). Explicit rules: do not transfer SpaCal numbers to LAr (line 229); high-Q2 period/beam-charge systematics blocked pending e- recovery (line 224, 281); H1CalcWeight relevant for absolute cross-sections (line 225). |

(Not re-read for this snapshot but referenced indirectly via
`research-highQ2-sidis-extension.md`:
`proton_HiQ/docs/research-data-coverage-gap.md`,
`proton_HiQ/docs/research-mc-normalization-highQ2.md`,
`proton_HiQ/docs/research-highQ2-proton-asymmetry.md`,
`proton_HiQ/docs/context-handoff-highQ2-proton.md`,
`lambda_HiQ/context-handoff.md`. These are cross-analysis references
for shared high-Q2 infrastructure: trigger S67, LAr electron, e-
period recovery, H1CalcWeight.)

---

## 10. Explicit Gaps in the Available Evidence

The following are NOT documented (or only partially documented) anywhere
in `H1/sidis_lowQ2/docs/` for the high-Q2 SIDIS analysis as of 2026-05-19:

1. **High-Q2 merged data ROOT file is not present.** Live listing of
   `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/` shows
   only the two MC merges. The handoff records "Merge complete" generically
   (line 276) but no high-Q2 data merge file is in evidence. DUST data
   subdirs (`H1SIDIS_HighQ2/output/data/{ep0304,ep0607_07,...}`) include
   `ep0304/` empty and others not enumerated here. **Gap:** is there a
   v5b high-Q2 data per-run population on DUST or /pnfs, and is there
   a merged data file anywhere?
2. **v5b high-Q2 data job count.** Handoff line 893 reports 1086 MC jobs
   completed but says nothing about a v5b high-Q2 data job campaign.
   `research-highQ2-sidis-extension.md` Section 2.1 reports the pre-v5b
   proton-driven campaign had 635 data files (ep0607 only). Is there a
   v5b data re-production yet?
3. **High-Q2 v5b schema verification at the branch level.** The numbers
   86/49/48 are taken from the handoff table. No live `validate_v3_output.py`
   report against an actual high-Q2 v5b ROOT file is documented. (The
   five extra `ChargedTotGen` branches and the two missing `DisTot`
   branches relative to low-Q2 are not enumerated in any read document.)
4. **High-Q2 acceptance map.** No 4D acceptance `epsilon(Q2, x, z, pT_h)`
   for h+ and h- has been produced for high-Q2. The low-Q2 4D acceptance
   exists; the high-Q2 acceptance is a placeholder ("reuse the low-Q2
   machinery essentially unchanged" per
   `research-highQ2-sidis-extension.md` Section 6.5, but not executed).
5. **High-Q2 purity / stability / occupancy study.** No high-Q2-specific
   occupancy or purity study exists. Low-Q2 has 4D occupancy
   (13/16 (Q2,x) cells, ~66k tracks/cell from 1M entries — handoff
   lines 458-462) and 30% per-dimension purity. High-Q2 binning grid
   is unset.
6. **High-Q2 phi_h study.** No high-Q2-specific phi_h acceptance, fit,
   moment extraction, or systematic exists. The low-Q2 phi_h work
   (handoff lines 941-961) is regime-specific.
7. **High-Q2 first-look A_h or M^{h+/-} number.** Handoff "First Physics
   Results" lines 902-940 are all on the **low-Q2** v5b chain
   ("Multiplicities... extracted from full dataset (34 RAPGAP MC +
   577 data)", "A_h = +0.001 to +0.018 across (Q2, x) cells"). No
   first-look numbers exist for high-Q2.
8. **Authoritative LAr electron-scale procedure.** `OQ3` / `HB7` /
   `SB4` open question. `reference-sidis-systematics.md` line 507
   names it explicitly. No procedure document exists.
9. **H1CalcWeight / effective MC luminosity validation for high-Q2.**
   `HB5` per `research-highQ2-sidis-extension.md` Section 6.7. Source
   document is `proton_HiQ/docs/research-mc-normalization-highQ2.md`
   (not re-read here). No SIDIS-side status memo is present in
   `H1/sidis_lowQ2/docs/`.
10. **e- period recovery completion status.** The recovery plan exists
    (`research-highQ2-sidis-extension.md` Section 6.2, derived from
    `proton_HiQ/docs/research-data-coverage-gap.md`). Whether it has
    been executed and resulted in a SIDIS-side data set is **not**
    documented in `H1/sidis_lowQ2/docs/`.
11. **High-Q2 charge-mismatch leak under v5b.** Pre-v5b inspection
    reported 0.76% leak (`research-highQ2-sidis-extension.md` Section
    2.5). The v5b high-Q2 leak number is not recorded.
12. **Whether the broad gen fiducial `|GenEta|<3.5, GenPt>0.05` was
    adopted at high-Q2 in v5b.** The handoff's v5b summary describes
    the generic "store broadly, cut downstream" policy but does not
    explicitly confirm the high-Q2 builder's `inFiducialCharged`
    definition matches low-Q2 (pre-v5b it used analysis-level
    cuts — `research-highQ2-sidis-extension.md` Section 2.3).
13. **Whether `ParticleSource_cg` is now stored on high-Q2
    `ChargedTotGen`.** Pre-v5b it was absent
    (`research-highQ2-sidis-extension.md` Section 2.3). v5b status
    not stated.

These gaps are exhaustive of what was NOT findable in the documents
reviewed. They are not by themselves blockers — they are simply
unanswered questions a planner should be aware of before assuming the
high-Q2 analysis is in a particular state.

---

## Provenance / Live Verification (2026-05-19)

- SSH control socket: `Master running (pid=23461)` — PASS.
- Remote commands executed (read-only `ls`/`find` only):
  - `ssh -O check naf-hone.desy.de`
  - `ls /pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/`
  - `ls /pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/` (subdirs)
  - `ls /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/`
  - `ls /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/`
  - `find /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/ -maxdepth 2 -name "*.root" | wc -l`
  - `ls /data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilder*`
- No find on `/cvmfs`, `/afs`, or unrestricted `/pnfs` was performed.

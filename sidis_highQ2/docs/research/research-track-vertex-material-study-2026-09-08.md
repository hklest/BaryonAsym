# Track-start position for v6/z005 detector-material study

**Scope and method.** High-Q2 inclusive charged-hadron SIDIS only; read-only source/documentation research. Read `context-handoff.md`, `STATE.md`, `docs/RUN_STATUS.md`, and the requested `sidis-hq2-builder`/`sidis-hq2-lessons` skills. `STATE.md:5-10` identifies v6/z005 as current. No compile, job, smoke, or ROOT-file inspection was performed.

## 1. Why `StartRadius` and `EndRadius` were removed

**Verified.** The recorded reason is explicitly: “StartRadius and EndRadius have been removed from the schema (**they were unused downstream**).” (`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C:654-659`). The v5g branch audit independently classified both as `diagnostic_keep_optional`, not required by the current pipeline (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/results/tree_branch_audit_v5g_2026-06-03.md:256-258`).

**Conclusion (verified):** this was not recorded as the FPACK remedy. The same source separately records the FPACK reason for retaining `TrackLength` only on data (`v6.C:656-660`), and the quarantine record repeats that separation (`src/_quarantine/2026-06-05_pre_v6_envelope/README.md:27-30`). The old v5g source only declared/reset the radius fields to sentinels; it did not fill them (`src/ChargedSidisTreeBuilderHighQ2_v5g.C:419-465,1309-1312`).

## 2. Is the FPACK assertion still accurate?

**Verified operationally.** Current v6 guards the exact chain `H1PartSelTrack::GetParticle() -> H1PartCand::GetTrack()` on *all MC* and says it triggers an FPACK abort in this H1OO release; central and combined fill loops both enforce MC `TrackLength=-999` (`v6.C:1622-1633,1970-1980`). Current QA/provenance continues to document the MC sentinel (e.g. `qa/v6_data_mc_qa_rapgap_2026-06-06/v6_qa_summary_central_v6_full.json:4118`).

**Verified qualification.** A paired Stage-2 MC smoke failed in `H1FpackIndexManager::LoadEvent: FPSORIG2` while H1OO resolved the DST via unavailable `/acs/mc2`; the report says the steering had reachable `/pnfs` HAT/MODS and `fLoadODS=false`, and attributes failure to the HAT-internal catalog/lazy DST resolution, not a v6 regression (`condor/v6sys_stage1stage2_smoke_pnfsfix_20260604_231916/SMOKE_REPORT.md:20-25,94-109`). The diagnostic’s own header says the candidate track chain drives navigation “back through MODS to DST” (`src/diagnostics/probe_startradius_mc.C:4-20`).

**Inferred conclusion.** The claim remains accurate for the deployed MC Stage-2 HAT/MODS-to-DST path, but evidence does **not** establish a universal 4.2.1 API defect. It is tier/catalog/ODS-lazy-loading dependent. No successful remapped-DST MC test or probe output was found; the review called the MC sentinel/absence evidence insufficient (`docs/critiques/critique-v6-or-envelope-migration-2026-06-05.md:378-389`).

## 3. MC-safe alternatives that avoid `H1PartCand::GetTrack()`

### A. Direct `H1CentralFittedTrackArrayPtr` / `H1CombinedFittedTrackArrayPtr`

**Verified API capability, unresolved MC safety/association.** The arrays expose `CentralFittedTracks` and `CombinedFittedTracks` (`/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Tracks/H1CentralFittedTrackArrayPtr.h:14-26`, `H1CombinedFittedTrackArrayPtr.h:14-26`). Fitted tracks inherit `GetStartHit()` (`H1VertexFittedTrack.h:21-47`); central `GetStartHit()` is position at first-hit radius (`H1CentralTrack.C:289-302`), while combined delegates to its central component (`H1CombinedFittedTrack.C:340-362`).

**Association blocker (verified).** `H1CreateSelTracks` creates selected tracks by making a candidate from each selected fitted-track object, then making `H1PartSelTrack` from that candidate (`H1CreateSelTracks.C:103-150`), but `H1PartSelTrack` stores no fitted-track index/ID (`H1PartSelTrack.h:24-49,81-198`). The reverse candidate link is precisely `H1PartCand::GetTrack()` (`H1PartCand.h:172-181`). Thus there is no documented stable key to join direct-array rows to the builder’s `H1PartSelTrack` loop. Momentum/charge/hitword matching is speculative and unsafe with duplicate tracks.

**MC status:** unresolved, not demonstrated safe. The arrays may still require the same ODS/DST payload; neither a successful MC access test nor a safe association exists in evidence.

### B. `H1SelVertexArrayPtr`

**Verified but unsuitable.** `H1SelVertexArrayPtr("SelVertices")` exposes event-level selected vertices (`H1SelVertexArrayPtr.h:14-26`) whose entries contain only position and type (`H1SelVertex.h:25-45`). It can provide the event primary vertex or MC `vtGen`; v6 already loops it to write event `GenVtxZ` (`v6.C:1236-1257`). It has no track membership or per-track association, so it cannot yield a reconstructed start point for each SIDIS track. **MC safe as an event-vertex access is already evidenced; not a solution.**

### C. Derive it from existing `RadLength`, `TrackLength`, `DcaPrime`

**Not feasible (verified semantics).** `RadLength` is assigned from `seltrack->GetLength()` in H1OO selection (`H1CreateSelTracks.C:258-260`), so it is a length, not start radius. `DcaPrime` is a DCA; `TrackLength` is a length and is `-999` on MC. Three scalar lengths/impact parameter do not determine the 3D first-hit `(x,y,z)` without the fitted trajectory/phase and first-hit radius. No invertible relation is documented. This route cannot recover `r,z` and cannot meet the MC requirement.

## 4. Recommended route and implementation sketch

**Recommendation: do not add a blind direct-array lookup. First perform a bounded MC accessibility/identity validation; if it passes, add an explicit source-object association key at the H1OO selection layer, then persist coordinates in Stage 2.** The correct executor is `h1-builder-engineer`; any multi-file validation is `h1-naf-operator`/`h1-production`.

**Concrete sketch for the implementer:** During the selected-track construction path, preserve the selected `H1VertexFittedTrack` identity (class plus `GetUniqueID()` if proven stable for the event) in a new `H1PartSelTrack`-accessible association, or modify the selection payload to write a one-to-one selected-track ordinal keyed to the same fitted-array ordinal. For central tracks use `H1CentralFittedTrack::GetStartHit()`; for combined use `H1CombinedFittedTrack::GetStartHit()` (central component). In Stage 2 retrieve through that explicit association—not candidate `GetTrack()`—and write `TrackStartX`, `TrackStartY`, `TrackStartZ`, `TrackStartR=sqrt(x*x+y*y)`, `TrackStartSource` (central/combined), and `TrackStartValid`. Preserve one row per existing `ChargedTot` row.

**Residual-risk ranking:** (1) **high, blocking**: direct fitted-array availability may still trigger lazy ODS/DST/FPACK on MC; validate on an accessible remapped DST before design commitment. (2) **high**: without an explicit identity field, association is non-unique. (3) **medium**: `GetStartHit` is first detector hit, not physical production vertex; label it accordingly. (4) **low**: coordinate/radius arithmetic.

## 5. Existing no-builder-change artifact?

**No, for the requested study (verified from available schema/provenance evidence).** Current v6 source branches omit `StartRadius`/`EndRadius` (`v6.C:701-705`); the current builder skill and canonical plan also specify their absence and MC `TrackLength` sentinel. Searches found no v6 per-track `TrackStartX/Y/Z`, start `r/z`, or STR production-vertex `r/z` output under either requested root.

**Historical non-solution:** the data-only pre-envelope v6sys smoke documented `StartRadius`, `EndRadius`, and `TrackLength` in `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/condor/v6sys_stage1stage2_smoke_pnfsfix_20260604_231916/output/stage2/data/central/d06_0220_stage2_central.root` (`SMOKE_REPORT.md:75-85`), but it is only a data smoke, radius-only (not x/y/z), historical/superseded, and has no MC counterpart. `ChargedTotGen.GenVtxZ` is an event primary-z diagnostic, not per-track STR production vertex (`v6.C:1236-1257`). Therefore no existing artifact enables the requested data-and-reco-MC material study without a builder/selection-payload change.

## Required follow-up

Hand off the implementation/one-data+one-MC smoke design to **h1-builder-engineer**. Before it changes source, route MC input-tier/catalog availability and MC semantics to **h1-mc-db** then **h1-mc-expert**. Route any actual batch validation to **h1-naf-operator**. No production release change is implied.

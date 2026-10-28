# v6/z005 detector-material vertex study without DST

**Scope.** Read-only design for high-Q² inclusive charged-hadron SIDIS v6/z005. Governing context read: `CONTEXT.md`, `context-handoff.md`, `STATE.md`, and `docs/RUN_STATUS.md`; requested `sidis-hq2-builder` skill loaded. `/acs`/DST/ODS track access is excluded. This assessment uses only the v6 HAT/MODS path and existing v6 trees. No code, job, or ROOT inspection was performed.

## Decision

**NOT achievable DST-free as the requested exact reco-track production-vertex `(r,z)` map.** It *is* achievable DST-free as a scientifically useful material-distortion study: normalized data/MC distributions in reconstructed displacement/track-quality proxies, anchored by an MC-only STR true-production `(r,z)` material reference. The proxy must never be labelled a reconstructed production vertex.

## 1. STR truth: `H1PartMC → STR → SVX`

**YES — source-demonstrated on the MC Stage-1 HAT/MODS production path, with no `H1Track`, `H1CentralFittedTrack`, or `GetStartHit()` call.** Stage 1 includes `H1PartMC`, `H1StrBankPtr`, and `H1Svx` (`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:147-156`), creates `H1PartMCArrayPtr gtrdata` and `H1StrBankPtr StrBank` in the MC event loop (`:1358-1363`), and at `:1773-1783` selects a non-GTR daughter then dereferences `StrBank[gp->GetStrIdx()]->GetSvx()->GetX/Y/Z()`. This is precisely the requested chain (the `H1StrRow` dereference is implicit in `StrBank[...]`).

**Qualification (verified).** Do this only in the MC reco/event-processing path that v6 central used. Do not put it in the Stage-1 standalone gen loop: that loop explicitly forbids `StrBank` access because lazy ODS/FPACK loading failed on batch nodes (`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:3006-3011`). This limitation does not invoke DST track APIs. STR particles are GEANT3 products, including material secondaries (`docs/handoffs/memo-mc-truth-policy-charged-sidis.md:25-46`).

## 2. Reconstructed side without DST

### (a) Existing `ChargedTot` fields

**Available, but FAIL for exact per-track start `(r,z)`.** `ChargedTot` already stores `NHitCJC`, `NHitCJC1`, `DcaPrime`, `RadLength`, `IsFromPrimary`, `IsCentralTrk`, `Theta`, `TrackLength`, `DcaPrimeSinTheta`, and `PVz` (`src/ChargedSidisTreeBuilderHighQ2_v6.C:677-719`); fill points are `:1615-1634` (central) and `:1963-1981` (combined).

`DcaPrime` is a transverse/r–phi impact parameter, not a start radius (documented as cm in `docs/research-primary-proton-dedx.md:127,314-327`). Define displacement proxies

`d0 = DcaPrime`, `d3-proxy = DcaPrimeSinTheta = DcaPrime sin(theta) = DcaPrime/cosh(eta)`.

For a *straight* trajectory born at transverse radius `r_prod`, with transverse direction making angle `alpha` to the radial vector, `|d0| ≈ r_prod |sin(alpha)|`; hence `r_prod ≥ |d0|`, but the unknown `alpha` makes this only a lower bound. Curvature, multiple scattering, vertex-fit conventions, and a nonzero primary-vertex radius further weaken it. `PVz` is the event primary z, not the track-origin z. `RadLength` and `TrackLength` are path-length/quality quantities, not a first-hit radius; moreover `TrackLength=-999` on MC by design (`v6.C:1622-1633,1970-1980`). Thus no invertible `(r,z)` relation exists from these scalars.

### (b) `IsFromPrimary()` / `IsFromSecondary()` / `SetVertexType`

**FAIL as a material-origin tag.** The selected charged-SIDIS builder explicitly retains only `IsFromPrimary()` tracks (`v6.C:1583-1588,1924-1926`) and persists only `IsFromPrimary`, not `IsFromSecondary` (`:692-705`). H1OO `SetVertexType(1)` sets the secondary bit only for `vtDecay=2`; `IsFromSecondary()` therefore means fitted to a decay vertex, not GEANT material production (`docs/research-isfromsecondary-deep.md:13-83`). Selection also prefers a primary-fitted hypothesis over a decay-fitted duplicate (`:140-166`). It cannot locate an interaction or identify material secondaries.

### (c) `H1SelVertexArrayPtr` non-primary entries

**Unresolved whether non-primary entries are populated at MODS; FAIL regardless for per-track association.** v6 demonstrates only event-array access and only consumes `vtGen` to fill `GenVtxZ` (`src/ChargedSidisTreeBuilderHighQ2_v6.C:1236-1257`); it has no enumeration/count evidence for `vtDecay`/`vtSecondary` entries. The API evidence describes entries as position plus type and no track membership (`docs/research/research-track-vertex-material-study-2026-09-08.md:29-32`). Even if non-primary entries exist, no stable selected-track-to-entry association is available, so they cannot provide each SIDIS track’s origin.

## 3. Recommended DST-free study

1. **Data and reco-MC proxy comparison (existing trees, zero builder change):** apply the same reco/SIDIS selection, then make unit-integral 1D/2D distributions in `abs(DcaPrimeSinTheta)` (primary displacement proxy), `DcaPrime` (signed diagnostic), `RadLength`, `NHitCJC`, `NHitCJC1`, `Theta`/`Eta`, and `PVz`. The closest requested “ratio map” is the binwise unit-area **data/MC map in `(abs(DcaPrimeSinTheta), RadLength)`**, with a parallel `(abs(DcaPrimeSinTheta), PVz)` map. State explicitly that neither axis is track-origin `(r,z)`.
2. **MC truth material reference:** independently histogram STR true `r_STR=sqrt(X^2+Y^2)` versus `z_STR=Z` through the demonstrated STR/SVX chain, restricted to the charged π/K/p definition and, when studying reco contamination, to reco-associated STR particles. Compare *structure/radii*, not bin-by-bin directly to the reco proxy map.
3. **Physics interpretation:** test whether data/MC disagreement localizes at large displacement proxy, short radial/path quality, or a known STR material radius; then quantify the induced change in the nominal cross section only via an analysis-owned validated variation. The current `det.int` label is STR-only GEANT secondary charged π/K/p excluding electrons/V0, and is not synonymous with pure material interactions (`docs/status/alpha0p2-background.md:52`).

This answers the material-distortion question without falsely claiming vertex reconstruction. Route production/ROOT validation to `h1-builder-engineer`/`h1-data-validator` and any correction or systematic to `h1-systematics`; this document does neither.

## 4. Minimal builder change

**For the recommended proxy study: NONE.** All proxy branches and `PVz` already exist in `ChargedTot` (`v6.C:677-719`).

**For the exact MC reco-versus-STR-true `(r,z)` half: a small MC-only Stage-2 extension is required, because current trees store `IsStr` but no STR SVX coordinates** (`v6.C:747-769`, truth-link fill `:1730-1737`). Proposed branches in `ChargedTot`: `StrProdR`, `StrProdZ`, `StrProdValid` (optionally retain `StrProdX`, `StrProdY` for audit). Declare adjacent to the existing track-quality variables before `v6.C:660`; branch adjacent to `DcaPrimeSinTheta` at `:702-705`; fill in the central-track matched block after `matched` is established at `:1730-1737`, only for `matched->IsStr()` with a valid `GetStrIdx()`/`GetSvx()`. Use `r=sqrt(x*x+y*y)`. Combined tracks have no matching in v6 (`:2038-2051`), so mark invalid rather than invent an association.

**Status of that change: proposed, not validated.** Its access pattern is supported by Stage-1 MC-reco code (Section 1), but `h1-builder-engineer` must perform a one-file MODS-only MC smoke before implementation is accepted. It must not introduce any `H1Track`, `H1CentralFittedTrack`, `GetStartHit`, or `/acs` dependency.

## Evidence status

- **Verified:** v6 lineage/current state; existing schema and selection; MC `TrackLength` sentinel; Stage-1 STR/SVX call chain; meaning/limitations of the vertex-type bit; lack of an existing per-track STR coordinate branch.
- **Inferred:** straight-line lower-bound relation and its usefulness only as a displacement proxy.
- **Unresolved:** population of non-primary `H1SelVertexArrayPtr` entries at MODS tier; it is non-decisive because no per-track association exists.

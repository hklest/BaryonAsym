# RAD sample reconstruction/filter provenance — 2026-09-25

## Determination

**PROVISIONAL — reconstructed-source provenance VERIFIED; a reconstruction/trigger-only event retention rule is not established.**

The retained DJANGOH14 RAD `C_rad` ntuples are built from official OO-4.0 **HAT+MODS ROOT pairs**, not from raw generator BOS/GEN files.  HAT+MODS were supplied to a study-local tree builder running in `gen_only` mode.  “Gen-only” therefore describes the output tree content and builder mode; it does not identify the upstream input format as raw GEN.

No checked record identifies a trigger, L4/L5, reconstruction-success, DST classification, or other event-retention criterion applied to the **stored HAT/MODS event population**.  The source builder loops over every event made available by H1Tree and fills `GenDIS` once per loop iteration; it contains no trigger or reconstructed-object predicate.  This does not prove that upstream DST/OO production retained every generated event.  The decisive raw GEN/BOS-versus-DST/OO count comparison is unavailable.

## RAD identifiers, source files, and labels

| Components/sample directory IDs | Generator/mode as source-labelled | genID | ReqID | Filename tokens | Exact input type | Evidence |
|---|---|---|---|---|---|---|
| 8955–8962 | `DJANGOH14`, `RAD.Q2GT60.CTEQ6L`; e+ 8955,8958–8960; e− 8956–8957,8961–8962 | **INSUFFICIENT EVIDENCE** | **INSUFFICIENT EVIDENCE** | `A[1-3]`, `S39600`, `R97905` retained as opaque | OO-4.0 `hat.*.root` + `mods.*.root`, filenames ending `.ftt.DST.<index>.root` | mapping TSV header/rows; inventory JSON lines 4–382; `rad_component_exposure...json` lines 28–32 |

Representative fully labelled chain (all 1,373 mappings use the same form):

```text
component/sample-directory ID 8956
  -> /pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/djangoh14/8956/
     hat.4.0.25.DJANGOH14.NC.EMINUSP.05.RAD.Q2GT60.CTEQ6L.A1.8956.S39600.R97905.ftt.DST.0000.root
     mods.4.0.25.DJANGOH14.NC.EMINUSP.05.RAD.Q2GT60.CTEQ6L.A1.8956.S39600.R97905.ftt.DST.0000.root
  -> retained gen-only ROOT ntuple
```

The exact HAT/MODS pair is stated in the mapping TSV line 2 and in the production EVLS steering lines 2–3.  The inventory verifies 1,373 HAT mappings, 1,373 MODS mappings, 1,373 retained ntuples, and all eight components (inventory lines 372–382).  The retained v2 manifest records 731 e+, 642 e− inputs and the `RC_treebuilder_genonly_v2` binary (manifest lines 1–12).

The `RC_treebuilder_genonly_v1` source explicitly parses `SetHatFiles` and `SetModsFiles`, adds both to `H1Tree`, then loops `while (H1Tree::Instance()->Next())`; it increments `nEvents` and fills `GenDIS` once per available event (source lines 129–159, 279–286, 364–370).  The v2 source has the same HAT/MODS input and all-available-event loop (lines 135–153, 192–224).  Its `gen_only` label is output metadata, not raw-GEN provenance (v1 lines 178–180).

## RAD selection/filter evidence

| Proposed requirement | Result | Source-backed evidence |
|---|---|---|
| Study builder trigger requirement | **NO (for the study builder)** | v1 source lines 279–365 and v2 lines 192–223 contain no trigger/L4/L5/reco-success/DST-classification predicate; every H1Tree event is entered in `GenDIS`. |
| Study builder reconstruction-success requirement | **NO (for the study builder)** | Same loop and unconditional `disTree->Fill()` (v1 line 364; v2 line 222). |
| Upstream official DST/OO trigger or reco retention | **UNKNOWN** | HAT/MODS prove a DST-derived OO input; no MCDB row, ReqID, generator log, official DST production log, or raw GEN/BOS event count was recovered.  The public MCDB CGI is disabled. |
| Stored file count reduction relative to raw generation | **UNKNOWN** | `GenDIS` count is known for retained ntuples, but no authoritative raw GEN/BOS `NEVENT`/generated-event total associated with components 8955–8962 was found. |

The only identified selections in the RAD study source are **truth-analysis flags/rows** (e.g. `pass_truth_event_fiducial`, `IsGtr`, `Mother2`, hadron kinematics).  They do not control `GenDIS` filling: `GenDIS` is filled once for each available source event.  They must not be conflated with upstream event retention.

## RAD count/exposure comparison

| Beam | Exact component set | SIGMT values [pb] | Oomclumi exposure [nb^-1] | Exposure × SIGMT = weighted sumW (not an unweighted generated-event count) | Actual retained `GenDIS` entries | Difference | Interpretation |
|---|---|---:|---:|---:|---:|---:|---|
| e+ | 8955, 8958, 8959, 8960 | 15040.275, 15042.467, 15046.548, 15048.433 | 16,206,393.0 | 243,784,131.8915 | 72,799,996 | 170,984,135.8915 (70.1375% of sumW) | **Not a deficit of generated events.** `sumW` is weighted; cannot be compared to unweighted `GenDIS` entries as a filter test. |
| e− | 8956, 8957, 8961, 8962 | 15077.351, 15078.504, 15084.840, 15088.075 | 13,534,017.0 | 204,057,124.7490 | 61,199,996 | 142,857,128.7490 (70.0084% of sumW) | **Not a deficit of generated events.** Same reason. |
| total | 8955–8962 | — | 29,740,410.0 | 447,841,256.6405 | 133,999,992 | 313,841,264.6405 | No raw generator count available; no selection-loss rate can be inferred. |

The beam-level `N_generated` / `GenDIS_entries_total` values are 72,799,996 / 61,199,996 / 133,999,992 (component-exposure JSON lines 6–25).  Oomclumi instead reports `sumW = Sigma × Lumi` per chunk (oomclumi JSON line 150); its component sums and SIGMT values are recorded in lines 32–144.  Therefore the requested exposure-times-SIGMT calculation is a weighted normalization identity, not a prediction of event count.  It is **not evidence** for 70% trigger/reconstruction loss.

## NONRAD provenance

| Family | Source format / chain | Detector simulation | Trigger/reco filter status | Evidence | Determination |
|---|---|---|---|---|---|
| NEW e+ `hk_0123_530x10k_eplusp_prod_20260911` | user-run DJANGOH14 BOS -> `dst2all` -> HAT/MODS -> `RC_treebuilder_genonly_v1` | No detector-simulation stage evidenced; `dst2all` conversion is present | No trigger/reco selection in stage-1 generation or stage-3 builder evidenced | stage-1 wrapper lines 22–25; stage-1 card lines 13–16, 58–60, 104–113; stage-3 wrapper line 33; validation JSON lines 7–34 | **VERIFIED user generation; PROVISIONAL pure-generator-event population.** Conversion to HAT/MODS occurred, so it is not raw GEN/BOS at final-tree input. |
| NEW e− `hk_0123_eminusp_regen_20260916` | user-run DJANGOH14 BOS -> `dst2all` -> HAT/MODS -> `RC_treebuilder_genonly_v1` | No detector-simulation stage evidenced; `dst2all` conversion is present | No trigger/reco selection in stage-1 generation or stage-3 builder evidenced | stage-1 wrapper lines 28–38; stage-2 wrapper line 33; stage-3 wrapper line 33; per-file validation `0001` line 1 | **VERIFIED user generation; PROVISIONAL pure-generator-event population.** Conversion to HAT/MODS occurred, so it is not raw GEN/BOS at final-tree input. |
| OLD NONRAD | existing `britzger_dust` HAT/MODS from `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh/`; then `RC_treebuilder_genonly_v2` | **UNKNOWN** | **UNKNOWN** upstream; study builder has no reco alpha cut | OLD manifest lines 2–29; representative steering lines 1–3 | **INSUFFICIENT EVIDENCE** whether this is official production or user generation, and whether its upstream HAT/MODS population has a trigger/reco filter. |

For NEW campaigns, `NEVENT 10000` is explicit in the e+ source card (line 60), and the stage-1 wrapper requires a nonempty BOS plus log (lines 22–25).  Each validated stage-3 output has `GenDIS=9,998`, e.g. e− job 0001 (per-file JSON line 1): this demonstrates a small 2-event per-job nonidentity after the generation/conversion chain, not a detector reconstruction criterion.  The aggregate NEW campaigns pass 530/530 unique-content validation with `GenDIS=5,298,940` each (session handoff lines 7–14).  The source record does not establish the cause of the two-event shortfall, so it is not attributed to a trigger or reconstruction selection.

## Common-footing verdict

**NO — not on demonstrated common footing.**

1. RAD has verified official OO-4.0 HAT/MODS inputs derived from `.DST`-labelled production files; NEW NONRAD is verified user generation followed by BOS-to-HAT/MODS conversion; OLD NONRAD is HAT/MODS of unresolved upstream provenance.
2. RAD upstream trigger/reconstruction retention is **UNKNOWN**; NEW NONRAD has no such requirement evidenced; OLD NONRAD upstream status is **UNKNOWN**.  Thus the claim that RAD is specifically reco-filtered while NONRAD is not is **not proven**, but equivalence is also not proven.
3. Independently and decisively, RAD is `Q2GT60` and OLD/NEW NONRAD is `Q2GT100`, so the generated domains are not matched.  This alone prevents a valid pure generator-level RAD/NONRAD ratio irrespective of the unresolved reconstruction question.  See `rad_sigmt_phase_space.csv` lines 2–13 and normalization-forensics document lines 9–23.

## Alternative pure generator-level RAD availability

**INSUFFICIENT EVIDENCE — no usable alternative located.**

Checked evidence identifies only the retained RAD OO/HAT/MODS-derived set (8955–8962).  The public MCDB endpoint `https://www-h1.desy.de/cgi-bin/imc` returned HTTP 200 with the disabled-scripts notice, so no database search by RAD, Q2 cut, beam, `genID`, or saved GEN state was possible.  The session handoff states that new RAD smokes aborted in AREMIT (`MSTA39`) and produced header-only BOS files (lines 20–22).  No source-backed disk path or MCDB row for a DJANGOH14 RAD raw GEN/BOS sample with e+ and e− and a compatible Q2 range was recovered.

## Parameter coverage and sources checked

**Parameter coverage:** searched components/sample-directory IDs 8955–8962; generator `DJANGOH14`; RAD/NORAD; beam; period; `Q2GT60`/`Q2GT100`; CTEQ6L; SIGMT; Oomclumi exposure; HAT/MODS/OO paths; `.DST` filename segment; source-builder input and loop logic; `GenDIS` totals; trigger/L4/L5/reco/filter text; old/new campaign stage scripts; `genID`; `ReqID`; saved GEN.  Not accessible: live MCDB query fields/rows, ReqID/genID mapping, official generator logs, saved-state fields, official raw GEN/DST count records.

| Source | Result |
|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rad_retained_hatmods_mapping_20260923/rad_retained_hatmods_mapping_20260923.tsv` | HAT/MODS source-pair mapping, component/sample-directory labels, 1,373 mappings. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rad_v1_provenance_inventory_20260923.json` | 8 components; HAT/MODS and retained counts; root metadata inventory. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rad_oomclumi_exact_v2_20260924/rad_oomclumi_exact_v2_20260924.json` | Exact component/beam SIGMT and Oomclumi normalization identity. |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rad_component_exposure_v2_20260924/rad_component_exposure_v2_20260924.json` | Retained `GenDIS` totals; explicitly unresolved genID/ReqID. |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/steering/production/mc13/djangoh14_8956/mc13_8956_0000_evls.steer` | Representative official HAT/MODS pair. |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v1.C` and `RC_treebuilder_genonly_v2.C` | HAT/MODS consumption and event-loop/fill semantics. |
| `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/` and `hk_0123_eminusp_regen_20260916/` | NEW user-generated BOS, dst2all, stage-3 outputs and validation. |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/condor/old_nonrad_v2/submit/input_manifest.json` | OLD HAT/MODS provenance only; no official/user production identity. |
| `https://www-h1.desy.de/cgi-bin/imc` | HTTP 200; H1 scripts disabled notice. |
| `docs/radiative/SESSION_HANDOFF_crad_resolution_2026-09-21.md` | retained RAD identity and failed new RAD smoke. |

## Next required lookup

Obtain an authoritative MCDB export or archived official generator/DST log for each RAD component that labels `genID`, `ReqID`, generated-event count, and any production event-selection/trigger condition; then compare that raw generated count to the exact `GenDIS` event identifiers in the corresponding HAT/MODS-derived ntuples.  For OLD NONRAD, obtain the corresponding `norad_prod21` production log/source-BOS provenance before treating it as a pure-generator counterpart.

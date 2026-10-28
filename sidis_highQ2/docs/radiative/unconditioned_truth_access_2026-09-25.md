# DJANGOH14/RAPGAP unconditioned-truth access investigation — 2026-09-25

## Verdicts

| Question | Verdict | Evidence |
|---|---|---|
| Are the official DJANGOH14 OO-4.0 HAT/MODS files proven to contain every generator-produced event? | **NO: UNDETERMINED.** They contain 90,000 stored OO events in the inspected `8956/0000` pair, but no raw generator `NEVENT`/GEN/BOS count or official production retention record was recovered. | Direct ROOT inspection: `HAT.GetEntries()=90,000`, `MODS.GetEntries()=90,000`; `rad_em_0000/runtime.log:29-37`; `rad_sample_reco_filter_provenance_2026-09-25.md:35-38,42-50`. |
| Are they proven to contain only reconstructed/trigger/L4/L5-selected events? | **NO.** | No such predicate is in the inspected study steering or builder; no official production selection record was recovered: `rad_sample_reco_filter_provenance_2026-09-25.md:31-40`. |
| Does the current RAD steering/eventlist mechanism filter events? | **NO.** | Official `mc13_8956_0000_evls.steer:1-5` has only `fHatFiles`, `fModsFiles`, `fLoadODS=false`; retained study `rad_em_0000.steer:1-3` has only `SetHatFiles`, `SetModsFiles`, `END$`; runtime states “No HAT selection” and “No Ascii List of Events”: `runtime.log:33-37`. `fEventListFiles` is absent. |
| Does `H1Tree::Next()` intrinsically skip events without reconstructed objects in this invocation? | **NO EVIDENCE; current direct test refutes an additional steering selection.** | H1Tree opens 90,000 events and v2 reports 90,000 processed: `runtime.log:29-37,131`; builder loop is unconditional `while (H1Tree::Instance()->Next())`, then unconditional `GenDIS.Fill()`: `RC_treebuilder_genonly_v2.C:192-224`. |
| Is the claimed RAD/NONRAD eta-flatness test a valid proof of unconditioned truth? | **NO.** | RAD includes real QED ISR/FSR and the compared samples have different generator domains, RAD `Q2GT60` versus NONRAD `Q2GT100`; a normalized kinematic-shape ratio need not be unity even for unconditioned samples. The unequal domains are documented at `rad_sample_reco_filter_provenance_2026-09-25.md:64-68`. |

## H1OO bank/file semantics

| Object/bank | Semantics | Physical availability in this workflow | Citation |
|---|---|---|---|
| `H1PartMCArrayPtr` | Per-event merged `H1PartMC` array. | Construct after `H1Tree::Next()`; iterate `0 <= ig < GetEntries()`. | `RC_treebuilder_genonly_v2.C:211-221`; `memo-mc-truth-policy-charged-sidis.md:25-33`. |
| `H1PartMC` | MC particle object; use `GetPDG()`, `GetMother1()`, `GetMother2()`, and `IsGtr()`. | Generator-truth candidate access object. | `RC_treebuilder_genonly_v2.C:212-218`. |
| GTR bank | Generator record; `IsGtr()==true`; charged-hadron truth is `|PDG| in {211,321,2212}`, `IsGtr()`, `Mother2==-1`. | **Not physically stored in MODS.** It is an underlying BOS/FPACK bank loaded through the raw-DST index when requested. | `memo-mc-truth-policy-charged-sidis.md:25-50`; `research-fpsorig2-acs-crash.md:81-89`. |
| STR bank | GEANT3 detector-simulation products; `IsGtr()==false`. | Not generator denominator. | `memo-mc-truth-policy-charged-sidis.md:30-46`. |
| HAT | OO event-level HAT tree. | Inspected tree has 90,000 stored entries. | Direct ROOT inspection; `runtime.log:29-37`. |
| MODS | OO reconstructed-object tree plus `IndexDST`; does not itself contain BOS banks including GTR/STR. | `IndexDST` resolves original raw FPACK DST on demand; missing originals cause bank-access failure. | `research-fpsorig2-acs-crash.md:81-105`. |
| Raw DST/FPACK BOS | Underlying event payload, including GTR. | Required for a source-backed claim that the GTR record is available; GEN/BOS is required only if the question is the generator’s pre-DST event total. | `research-fpsorig2-acs-crash.md:81-99`. |

## Current builder/access path

`RC_treebuilder_genonly_v2.C` already uses the correct truth iterator: `H1Tree::AddFile(HAT)`, `H1Tree::AddFile(MODS)`, `H1Tree::Open()`, `while (H1Tree::Next())`, `H1PartMCArrayPtr gtrdata`, then `H1PartMC::IsGtr()` (`RC_treebuilder_genonly_v2.C:151-153,192-224`). The code applies no trigger, L4/L5, reconstructed-track, reconstruction-success, DST-classification, or eventlist predicate before `GenDIS.Fill()` (`:193-223`). `classifySourceGTR()` is GTR-only and avoids `H1StrBankPtr`/ODS access (`TreeBuilder.h:200-262`).

`ParticleSource` is diagnostic only for this charged-hadron truth definition: GTR excludes GEANT3 material/STR secondaries. Do not require `ParticleSource!=4` after `IsGtr()`; it is redundant (`memo-mc-truth-policy-charged-sidis.md:48-62`).

## RAPGAP equivalent

RAPGAP31 uses the identical H1OO truth path: HAT+MODS for event navigation, raw-DST-backed GTR on demand, `H1PartMCArrayPtr`, `H1PartMC`, and `IsGtr()`; the source explicitly documents that only GTR is generator truth (`TreeBuilderHighQ2_chargedsidis_v6.C:2928-2938`). RAPGAP RAD/NONRAD can be a truth-availability cross-check only after exact paired-subset, beam/period, generator-domain, raw-count, and normalization provenance are established. Its old aggregate-luminosity `C_rad` route remains quarantined (`sidis-hq2-radiative` authoritative facts; `context-handoff.md:221-227`).

## Executable production/validation plan

1. Use the existing official pair manifest for DJANGOH14 RAD components `8955-8962`; e.g. `8956/0000` HAT/MODS paths in `rad_retained_hatmods_mapping_20260923.tsv:2`.
2. Use a two-line steering file only: `SetHatFiles(<exact hat>)`, `SetModsFiles(<exact mods>)`, `END$`. Do **not** add `fEventListFiles`, `H1SteerEventList`, an ASCII event list, or HAT selection. The existing `rad_em_0000.steer:1-3` is the required pattern.
3. **No physics modification is required** to `RC_treebuilder_genonly_v2.C` for the current HAT/MODS population. Run the existing `H1Tree::Next()`/`H1PartMCArrayPtr` loop. A builder change cannot restore an event absent from the source OO tree.
4. Before declaring the result unconditioned relative to generator production, obtain the raw FPACK-DST file list from each MODS `IndexDST`, count its events/bank records, and obtain the official DJANGOH generator `NEVENT`/log for each chunk. Require equality of generator, raw-DST, HAT, MODS, and `GenDIS` event identifiers/counts. If the raw-DST/GTR record cannot be opened, restore the `H1_DATA`/dcap-supported ODS environment; MODS alone is insufficient for GTR-bank proof.
5. Expected current output count is **90,000 `GenDIS` events per official RAD HAT/MODS file**, not a larger inferred number: HAT=MODS=H1Tree=builder=90,000 for inspected `8956/0000`. A different expected count is **undetermined** until the raw generator record is recovered.
6. Validate retention by event-ID/count equality against raw DST/GEN, not by requiring RAD/NONRAD normalized `GenEta_lab` unity. Then compare RAD/NONRAD only with matched generator cuts and beam composition; quantify the physical radiative shape difference rather than treating it as a retention failure.

## Raw generated-event count

**NOT RECOVERED.** The only recovered official RAD count is the stored OO/`GenDIS` population: e+ `72,799,996`, e− `61,199,996`, total `133,999,992`; these are not established raw generator counts (`rad_sample_reco_filter_provenance_2026-09-25.md:42-50`). `Sigma x Lumi` is weighted sumW, not an unweighted count (`:44-50`). The inspected per-file OO count is 90,000. The disabled MCDB CGI, absent official generator logs, absent raw GEN/BOS count, and unresolved `genID`/`ReqID` prevent an absolute retention fraction (`:72-78,93-95`).

## Remaining undetermined

1. Official DJANGOH14 `8955-8962` generator `NEVENT` and raw GEN/BOS/DST counts per chunk.
2. Whether official OO production applied any event-retention condition before HAT/MODS writing.
3. Equality of the source raw-DST event identifiers with HAT/MODS/`GenDIS` identifiers.
4. The absolute RAD retention fraction and any source of the observed eta/Q2 shape ratios.
5. A generator-domain-matched DJANGOH14 RAD/NONRAD pair; current RAD `Q2GT60` and NONRAD `Q2GT100` are not matched.

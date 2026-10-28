# RAPGAP-π (pion-exchange) MC sample availability for the high-Q2 FNC study

- **Date:** 2026-10-20
- **Author:** h1-script-runner subagent (bounded read-only MC inventory)
- **Scope:** inventory of the RAPGAP31 `PIPEXCH` (pion-exchange) MC sample directories on `/pnfs` (and the attempted `/acs` generator-file area); double-counting overlap between same-genID ReqIDs; one OO spot-check; workspace prior-consumption cross-reference; recommendation of a single ReqID for an ep0607-era (06_07e+) high-Q2 FNC study, smearing variant preferred.
- **Method:** login-node read-only checks only, no Condor: `ls`/`du -sk` per directory; plain system ROOT 6.40.04 (no H1OO libraries) for single-file opens, each well under 5 min wall. Multi-file operations (exact per-ReqID event totals) were **not** run and are reported as open items.
- **Established facts used (from the delegating session, not re-verified):** `/pnfs` online MC area readable; ReqID 9134 = genID 5623, RAPGAP31 PIPEXCH e+ 06_07e+ SW84, GRV-LO-π, Holtmann-π flux, LN θ-smearing `sigx=46.0e-6`, `sigy=168.0e-6`, generator files C00–C72, 10,000,002 events, MCDB Lumi 305.06 (as recorded); genID mapping 8946/9134/9137 → 5623 and 8945/9133/9136 → 5622; the analysis chain consumes HAT/MODS-type OO files, not DST.
- **Related docs:** `fnc_h1oo_availability.md` (FNC in H1OO 4.2.1 and in the data-side oo-4.0 hat/mods), `fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` (v6fnc MC FNC matching-efficiency diagnosis, which references a RAPGAP-π comparison sample without ReqID-level provenance).

---

## 1. Executive summary

| Question | Answer |
|---|---|
| Are the RAPGAP-π OO (HAT/MODS) samples on `/pnfs` readable? | **Yes** — all 10 ReqID directories under `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/` list cleanly; OO files of all six 894x/913x ReqIDs opened with plain ROOT 6.40.04 (no H1OO libs). |
| Do 8946 / 9134 / 9137 (genID 5623) overlap? | **Yes, 100%.** Identical DST ranges (C0000001–C0001000), identical OO numbering (101 HAT + 101 MODS, DST.0000–0100), identical sampled per-file event counts. They are independent re-reconstructions of the *same* generated events. Any **one** covers the full C00–C72 range; combining any two double counts every event. |
| Do 8945 / 9133 / 9136 (genID 5622) overlap? | **Yes, same structure.** 8945 and 9136 are complete identical-coverage re-reconstructions (896 DST, 90 HAT + 90 MODS). 9133 has the *same* complete DST set but an **incomplete OO set** (HAT missing 0012, 0045; MODS missing 0012, 0038, 0045). |
| OO spot-check verdict | 9134 `hat...DST.0000.root` opens with plain ROOT; TTree `HAT`, 90,000 entries, 219 branches incl. `PartFNC`; partner MODS file has TTree `MODS`, 90,000 entries, 25 branches incl. `FNCPart` (`H1PartFNC`). No plain-ROOT-level readability obstruction. |
| Recommended single ReqID (06_07e+, smearing) | **ReqID 9134** (genID 5623) — complete gap-free 101 HAT + 101 MODS set covering the full generator range; verified openable; correct era/steering/smearing variant. Never combine with 8946 or 9137. |
| `/acs` generator-file inventory | **BLOCKED** — `/acs` is not mounted on this node; the B00–B81 / C00–C72 sets could not be inventoried locally. |

---

## 2. Inventory

### 2.1 OO area — `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/<ReqID>/`

File-name pattern: `hat.<ooVer>.RAPGAP.PIPEXCH.EPLUS.RAD.GRVLO.Q2GT3[.smearing].<ReqID>.<Stag>.<Rtag>.ftt.DST.NNNN.root` and the matching `mods.*` file (identical numbering). Classes present per file: TTree `HAT` or `MODS` plus keys `MC` (TNamed), `H1TreeEventList` (H1FileInfo); MODS files additionally carry `IndexDST` (H1FpackIndexManager). "Readable" = directory listing OK; file-open column states whether a plain-ROOT open was performed. Sizes are `du -sk` (1 KiB blocks); GB ≈ KiB×1024/10⁹.

| ReqID | genID (established) | tag / S,R / H1OO ver | files (HAT+MODS) | numbering | complete? | total size | open-check |
|---|---|---|---|---|---|---|---|
| 8433 | — (not established) | no smearing tag; S39400 R97901; 4.0.6 | 90 + 90 = 180 | 0000–0089 | yes | 35,047,438 KiB ≈ 35.9 GB | listing only |
| 8434 | — (not established) | no smearing tag; S39400 R97901; 4.0.6 | 101 + 101 = 202 | 0000–0100 | yes | 39,905,940 KiB ≈ 40.9 GB | listing only |
| 8502 | — (not established) | smearing; S39500 R97903; 4.0.6 | 90 + 90 = 180 | 0000–0089 | yes | 36,091,230 KiB ≈ 37.0 GB | listing only |
| 8503 | — (not established) | smearing; S39500 R97903; 4.0.6 | 101 + 101 = 202 | 0000–0100 | yes | 40,316,509 KiB ≈ 41.3 GB | listing only |
| 8945 | 5622 | smearing; S39600 R97905; 4.0.25 | 90 + 90 = 180 | 0000–0089 | yes | 35,620,971 KiB ≈ 36.5 GB | OPEN_OK (hat.0000) |
| 8946 | 5623 | smearing; S39600 R97905; 4.0.25 | 101 + 101 = 202 | 0000–0100 | yes | 39,788,703 KiB ≈ 40.7 GB | OPEN_OK (hat.0000, hat.0100) |
| 9133 | 5622 | smearing; S39600 R97905; 4.0.25 | 87 + 86 = 173 | 0000–0088 | **no — HAT missing 0012, 0045; MODS missing 0012, 0038, 0045** | 34,151,374 KiB ≈ 35.0 GB | OPEN_OK (hat.0000) |
| 9134 | 5623 | smearing; S39600 R97905; 4.0.25 | 101 + 101 = 202 | 0000–0100 | yes | 39,697,180 KiB ≈ 40.6 GB | OPEN_OK (hat.0000/0050/0100, mods.0000) |
| 9136 | 5622 | smearing; S39600 R97905; 4.0.25 | 90 + 90 = 180 | 0000–0089 | yes | 35,539,272 KiB ≈ 36.4 GB | OPEN_OK (hat.0000) |
| 9137 | 5623 | smearing; S39600 R97905; 4.0.25 | 101 + 101 = 202 | 0000–0100 | yes | 39,699,083 KiB ≈ 40.6 GB | OPEN_OK (hat.0000) |

Typical per-file sizes (5623 family): HAT ≈ 55 MB, MODS ≈ 311 MB.

**Structural note:** the odd/even ReqID pairing (8433/8434, 8502/8503, 8945/8946, 9133/9134, 9136/9137) is mirrored across all families — odd members have 90 HAT/90 MODS (0000–0089), even members 101/101 (0000–0100). For the 894x/913x families the established genID mapping assigns odd→5622, even→5623. The genIDs of 8433/8434/8502/8503 were **not** established in this inventory; their identical count structure suggests the same two underlying generator samples, but this must be confirmed via MCDB (h1-mc-db) before any combined use.

### 2.2 DST area — `/pnfs/desy.de/dphep/online/h1/mc/rapgap31/07/<ReqID>/`

File-name pattern: `RAPGAP.PIPEXCH.EPLUS.RAD.GRVLO.Q2GT3.smearing.<ReqID>.S39600.R97905.ftt.DST.C00000NN` (no `.root`). Sampled files (9134 C0000001, C0001000) **fail to open with plain ROOT** — these are BOS-format DST, consistent with the chain consuming only the ROOT-format OO HAT/MODS files. All six directories list cleanly; all C-ranges are contiguous with no gaps.

| ReqID | genID | files | C-range | total size | note |
|---|---|---|---|---|---|
| 8945 | 5622 | 896 | C0000001–C0000896 | 394,572,331 KiB ≈ 404 GB | complete |
| 8946 | 5623 | 1000 | C0000001–C0001000 | 440,837,045 KiB ≈ 451 GB | complete |
| 9133 | 5622 | 896 | C0000001–C0000896 | 395,109,165 KiB ≈ 405 GB | complete (OO area is the incomplete one) |
| 9134 | 5623 | 1000 | C0000001–C0001000 | 441,386,911 KiB ≈ 452 GB | complete |
| 9136 | 5622 | 896 | C0000001–C0000896 | 395,066,957 KiB ≈ 404 GB | complete |
| 9137 | 5623 | 1000 | C0000001–C0001000 | 441,349,100 KiB ≈ 452 GB | complete |

Typical DST C-file size ≈ 448–453 MB (last 5622 file C0000896 ≈ 261 MB, i.e. a short tail).

**Per-ReqID DST vs OO content:** every ReqID exists in *both* areas — a BOS DST production (complete in all six cases) and the ROOT-format HAT+MODS re-reconstruction (what the chain consumes; incomplete only for 9133). The DST areas are not consumable by the HAT/MODS chain and are not needed for the study.

### 2.3 `/acs` generator-file area — NOT ACCESSIBLE (blocker)

`/acs/mc2/rapgap31/` **could not be inventoried**: `/acs` is not mounted on this node (absent from `/` and `/proc/mounts`; `/pnfs/desy.de/dphep/` contains only `online` and `scratch`; no `/acs` or `mc2` reference exists anywhere in `sidis_highQ2/docs/`). The B00–B81 (non-smearing) / C00–C72 (smearing) generator-file sets and their sizes therefore remain **unverified from this node**; the C00–C72 range for genID 5623 is recorded from the delegating session's established MCDB facts only. Resolving this needs either a node with `/acs` mounted or an `h1-mc-db` MCDB query.

---

## 3. Same-genID overlap verdict

### genID 5623 (e+ 06_07e+ SW84, GRV-LO-π, Holtmann-π flux, LN θ-smearing sigx=46.0e-6 / sigy=168.0e-6; generator files C00–C72, 10,000,002 events; MCDB Lumi 305.06 as recorded)

| property | 8946 | 9134 | 9137 |
|---|---|---|---|
| DST files / range | 1000 / C1–C1000 | 1000 / C1–C1000 | 1000 / C1–C1000 |
| OO HAT files | 101 (0000–0100) | 101 (0000–0100) | 101 (0000–0100) |
| OO MODS files | 101 (0000–0100) | 101 (0000–0100) | 101 (0000–0100) |
| OO gaps | none | none | none |
| hat.DST.0000 HAT entries | 90,000 | 90,000 | 90,000 |
| hat.DST.0100 HAT entries | 9,998 | 9,998 | not opened |
| file naming | hat/mods.4.0.25, S39600 R97905 | identical | identical |

Corresponding files are **not byte-identical** (per-file sizes differ by ≲0.4%, e.g. DST C0000001: 448,592,760 / 449,036,280 / 447,483,960 B for 8946/9134/9137) — these are independent re-reconstructions, not copies — but their file counts, numbering ranges, and sampled per-file event counts are identical, and all three carry the same genID (same generated events).

**Verdict: identical full-coverage re-reconstructions of the same generator production.** Any single one of {8946, 9134, 9137} covers the full C00–C72 generator range. Combining any two (or all three) would double (triple) count **every** event. Use exactly one.

### genID 5622 (companion sample; MCDB Lumi 273.45 as recorded)

| property | 8945 | 9133 | 9136 |
|---|---|---|---|
| DST files / range | 896 / C1–C896 | 896 / C1–C896 | 896 / C1–C896 |
| OO HAT files | 90 (0000–0089) | 87 (0000–0088, **missing 0012, 0045**) | 90 (0000–0089) |
| OO MODS files | 90 (0000–0089) | 86 (0000–0088, **missing 0012, 0038, 0045**) | 90 (0000–0089) |
| hat.DST.0000 HAT entries | 90,000 | 90,000 | 90,000 |

**Verdict:** 8945 and 9136 are identical full-coverage re-reconstructions of each other (same relation as the 5623 trio). 9133 re-reconstructs the same sample — its DST area is complete and identical in range, but its OO area is missing files (173 of 180 expected HAT/MODS files). For OO consumption use exactly one of {8945, 9136}; avoid 9133's OO set (or repair the gap) and never combine any of them.

### Older families (8433/8434, 8502/8503)

8433/8434 (no smearing tag, H1OO 4.0.6, S39400/R97901) and 8502/8503 (smearing tag, H1OO 4.0.6, S39500/R97903) mirror the 90/101 file-count structure. Their genIDs were not established here; **if** MCDB assigns them genID 5622/5623 they would be further double-counting re-reconstructions of the same samples. No combined use with the 894x/913x families should be attempted without an MCDB confirmation. For the 06_07e+ smearing study they are excluded regardless (843x is the non-smearing variant; 850x is an older H1OO 4.0.6 production).

---

## 4. OO spot-check (plain ROOT, no H1OO libraries)

Primary file: `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/9134/hat.4.0.25.RAPGAP.PIPEXCH.EPLUS.RAD.GRVLO.Q2GT3.smearing.9134.S39600.R97905.ftt.DST.0000.root` (55,101,188 B).

- **Opens OK** with system ROOT 6.40.04. File format version 53404 (ROOT 5.34/04-era writer, consistent with the H1OO 4.0.x production named in the file).
- Keys: `HAT` (TTree, two cycles), `MC` (TNamed), `H1TreeEventList` (H1FileInfo). Warnings only for missing H1OO dictionaries (`H1Pointer`, `H1PointerArrayIndex`) — expected without the libraries, and the tree header is fully readable regardless.
- TTree `HAT`: **90,000 entries**, 219 branches, including `PartFNC` and `PartFPS`, the full raw trigger content (`Il1te`, `Il1rw`, `Il1ac`, `ITrigac`, `L4dec`, `Iclass`, …) and generator-level branches — the same HAT structure the v6 Stage-1 reads on data.
- Partner `mods...DST.0000.root` (311,122,356 B): TTree `MODS`, **90,000 entries**, 25 branches: `Particles`, `SelectedTracks`, `EmParticles`, `Muons`, `MCParticles`, **`FNCPart`** (TClonesArray of `H1PartFNC` — class name confirmed via the streamer warning), `FPSPart`, …, `RunNumber`, `EventNumber`, `IndexDST`. The MC OO files therefore **structurally carry the same FNC branches** as the data-side oo-4.0 production (cf. `fnc_h1oo_availability.md` §3, §5). FNCPart *occupancy* (FNC simulation content) was not measured here — see open items.
- Cross-file entry counts (single-file opens): hat.0000 = 90,000 for all six 894x/913x ReqIDs; hat.0050 (9134) = 100,000; hat.0100 = 9,998 for both 8946 and 9134. Per-file event counts are variable (90k–100k), consistent with a ~10M-event total split across 101 files, but the exact per-ReqID OO total was not summed (multi-file operation → Condor; not run, per task instructions).
- **oo-4.0 vs H1OO 4.2.1 readability:** no obstruction observable at plain-ROOT level — the files open, and tree/branch/entry metadata read via streamer info. What plain ROOT cannot verify is full H1OO-class event streaming under H1OO 4.2.1 (the login node cannot load the H1OO libraries; GLIBCXX mismatch per `fnc_h1oo_availability.md` §2). The reader/production combination the study would use — the H1OO 4.2.1 chain reading oo-4.0 ROOT OO hat/mods — is the same combination already in production use on the data side; the first Condor job over these MC files doubles as the streaming validation.

---

## 5. Workspace cross-reference (prior consumption)

Word-boundary `rg` searches for `9134`, `9137`, `8946`, `8945`, `9133`, `9136`, `5623`, `5622` across `/data/dust/group/h1/klesthen/BaryonAsym` (including gitignored files via `--no-ignore`, excluding binaries): **zero matches**. An unbounded substring search only hits numeric coincidences inside unrelated floating-point numbers/timestamps (e.g. JSON floats). No code, steering, script, doc, or JSON in the workspace references any of these ReqIDs or genIDs.

Conclusion: **no prior consumption recorded** — the RAPGAP-π samples have never entered the v6 or v6fnc chains per available text provenance. (`fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` references a RAPGAP-π comparison sample qualitatively, but records no ReqID/genID provenance for it; this document provides that provenance.)

---

## 6. Recommendation (facts only)

**Best single ReqID for an ep0607-era (06_07e+) high-Q2 FNC study, smearing variant: ReqID 9134 (genID 5623).**

1. **Correct sample identity and variant:** genID 5623, RAPGAP31 PIPEXCH e+ 06_07e+ SW84, GRV-LO-π with Holtmann-π flux, LN θ-smearing (`sigx=46.0e-6`, `sigy=168.0e-6`) — the smearing-tagged production (the untagged 8433/8434 family is the non-smearing variant).
2. **Complete, gap-free OO set covering the full generator range:** 101 HAT + 101 MODS files (DST.0000–0100) over the C00–C72 generator range (10,000,002 generated events), ≈40.6 GB on `/pnfs`; the chain consumes these ROOT OO files, and the complete BOS DST area (1000 files, ≈452 GB) is additionally available but not needed.
3. **Verified readable** at plain-ROOT level (this inventory): HAT/MODS TTrees with expected entry counts; `FNCPart`/`PartFNC` FNC branches present, matching the branch set the v6fnc study reads on data.
4. **No double counting when used alone; guaranteed double counting otherwise:** 8946 and 9137 are equivalent full-coverage re-reconstructions of the same events — valid single-ReqID alternates if 9134 were ever unavailable, but they must never be combined with 9134 or with each other.
5. **Companion era sample (if ever needed):** genID 5622 via 8945 **or** 9136 (one only); 9133's OO set is incomplete (missing 0012/0038/0045) and should not be used as-is.
6. **Normalization (facts only, no weights derived here):** MCDB Lumi as recorded: 305.06 (genID 5623), 273.45 (genID 5622); units as recorded in MCDB, not independently verified. Final MC weights/scaling are a later h1-normalization task; note the workspace's 2026-09-25 unit correction (`H1SteerTree::fLumi` is pb⁻¹, `STATE.md`) as a caution for that task.

---

## 7. Open items / blockers

1. **`/acs` generator-file inventory BLOCKED** — `/acs` not mounted on this node; B00–B81 / C00–C72 sets and sizes unverified. Needs a `/acs`-capable node or an `h1-mc-db` MCDB query.
2. **Exact per-ReqID OO event totals** require summing entries over 101 files per ReqID (multi-file → Condor, not run per task instructions). Sampled: 90,000 (hat.0000), 100,000 (hat.0050), 9,998 (hat.0100).
3. **FNCPart occupancy / FNC-response content in the RAPGAP-π MC not measured here** — branch structure verified only; the known MC FNC-response cone limitation is documented in `fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` and is a separate question from sample availability.
4. **genIDs of 8433/8434/8502/8503 unconfirmed** — MCDB check required before any use alongside the 894x/913x families.
5. **9133 OO gap** (HAT 0012, 0045; MODS 0012, 0038, 0045) — recorded, no repair attempted (read-only scope).

---

## 8. Audit trail (checks performed, all single-file, < 5 min each)

- `ls` + `du -sk`: all 10 OO ReqID directories and all 6 DST ReqID directories (counts, numbering ranges, gap analysis, sizes).
- Plain-ROOT 6.40.04 opens (`oocheck.C`/`oobranches.C`, no H1OO libs): 9134 hat.0000 (full key/branch listing), mods.0000, hat.0050, hat.0100; 8946 hat.0000, hat.0100; 9137 hat.0000; 8945 hat.0000; 9136 hat.0000; 9133 hat.0000.
- Plain-ROOT open attempts on 9134 DST C0000001 / C0001000 (fail — BOS DST, expected).
- `rg` word-boundary searches (default and `--no-ignore`) for the eight ReqID/genID tokens across the workspace; `/acs`, `mc2`, `B00-`, `C00-` reference search in `sidis_highQ2/docs/`.
- Corresponding-file size comparison across same-genID ReqIDs (DST C0000001/C0001000 or C0000896; hat/mods DST.0000).

*End of artifact.*

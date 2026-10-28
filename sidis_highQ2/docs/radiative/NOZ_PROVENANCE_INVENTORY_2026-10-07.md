# Britzger norad_prod21 MC provenance inventory: NoZ / NOZ1 / BORN_NORAD / NORAD / RAD (2026-10-07)

Read-only provenance audit supporting the photon-exchange-only ("NoZ") C_rad derivation in the
high-Q2 charged-hadron SIDIS analysis. Companion to `BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md`
(same directory). Nothing under `/data/dust/group/h1/britzger/` was modified; the only artifacts
created by this audit are GEN FPACK header dumps under `/tmp/opencode/hdr_*.{bin,txt}`.

Audited areas:
- DUST: `/data/dust/group/h1/britzger/norad_prod21/` (dst2all conversion dirs, 2021 production
  areas, `EW_gZ_correction/` validation campaign) and `/data/dust/group/h1/britzger/djangoh14_NoZ/`
  (2023 noZ build), `/data/dust/group/h1/britzger/EventShapes/KinFitBIRM_git/Steering/` (fLumi).
- dCache: `/pnfs/desy.de/dphep/scratch/h1/generator_files/djangoh14/` (GEN sources; readable
  2026-10-07 via timeout-wrapped reads, despite the 2026-05-22 workspace note on /pnfs).

## 1. Headline verdicts

| Question | Verdict | Confidence |
|---|---|---|
| Exe for NOZ and NOZ1 | Britzger 2023 "neu" noZ build `/data/dust/group/h1/britzger/djangoh14_NoZ/djangoh14test` (5,899,408 B, mtime 2023-02-01 13:16 = banner "compiled at date 10223 and time 1316"); GEN title records carry "Special executable with Z-exchange switched off (D.Britzger)" | VERIFIED |
| NOZ vs NOZ1 | NOZ = photon-only BORN (GEN header ACTIVE 'LPIN' 2 0 0); NOZ1 = photon-only corrected/Born+SV (ACTIVE 'LPIN' 2 0 1) | VERIFIED |
| Safe photon-only sample | NOZ1 = `dst2all_djangoh14_NoZ_new/`; the only sample the author analyzed (steer chains read NoZ_new / NOZ1_NORAD files) | VERIFIED |
| BORN_NORAD exe | Stock full-EW 2023 production exe (title WITHOUT Z-off sentence; ARIADNE 4.12 _dev banner) | VERIFIED |
| 3169/3137 pb vs 7.3298/7.2939 nb | "Visible x-sect" = sigma_phys x (N_written/N_generated), MCWE bookkeeping, NOT a cross-section; physical sigma = SIGMT from .OUT | VERIFIED (arithmetic reproduced) |
| Photon-only e+ sample | Does not exist (dCache + DUST searched) | VERIFIED (absence) |
| Photon-only RAD sample | Does not exist; RAD is full-EW | VERIFIED (absence) |
| DUST / dCache readability | All reads succeeded 2026-10-07 | VERIFIED |

## 2. DUST inventory (`/data/dust/group/h1/britzger/norad_prod21/`)

### 2.1 dst2all conversion dirs (OO ROOT hat+ods+mods, converted from dCache GEN FPACK)

| Dir | Sample (GEN family) | ROOT files | Size | Charges x series x chunks | ods mtime range |
|---|---|---|---|---|---|
| `dst2all_djangoh14_NoZ/` | NOZ_NORAD (photon-only Born) | 260 hat + 260 ods + 260 mods = 780 | 189 GB | e- only; A-D x C0000000-0064 (65 each) | 2023-02-02 17:44 -> 2023-02-03 10:33 |
| `dst2all_djangoh14_NoZ_new/` | NOZ1_NORAD (photon-only corrected) | 260 x 3 = 780 | 188 GB | e- only; A-D x 65 | 2023-02-05 14:46 -> 2023-02-06 05:56 |
| `dst2all_djangoh14_BORN_NORAD/` | BORN_NORAD (full-EW Born, fixed alpha) | 519 x 3 = 1557 | 375 GB | e- 259 + e+ 260 chunks; A-D x 65 | 2023-02-02 13:43 -> 2023-02-05 23:24 |
| `dst2all_djangoh14_NORAD/` | NORAD (full-EW, non-rad) | 520 x 3 = 1560 | 376 GB | e- 260 + e+ 260; A-D x 65 | 2023-02-06 21:15 -> 2023-02-07 01:40 |
| `dst2all_djangoh14_RAD/` | NC.E(MINUS\|PLUS)P.RAD (full-EW, radiative) | 126 x 3 = 378 | 92 GB | e- 61 + e+ 65 chunks; single series per charge (C0000000-...) | 2023-02-08 20:00 -> 2023-02-09 12:53 |

File naming: `hat.|ods.|mods.<SAMPLE>.<series>.C<NNNNNNN>.root`; per chunk also
`fpack.<...>.steer` and `log.dst2all.<...>.txt` (NORAD/RAD dirs additionally keep `tmp2.<...>`
leftovers). Each dir has the `dst2all.fpacksteer` template (BULK 2000 DST 3 FILE, NEVENT 100000000;
leftover RAPGAP31 comment = template heritage) and `runall.EMINUS.sh` (+`runall.EPLUS.sh` for the
two-charge dirs). The NoZ and BORN dirs contain a symlink (`djangoh14_NoZ`, `djangoh14_dphep_scratch`)
pointing at the dCache GEN area.

Conversion provenance (VERIFIED): per-chunk steers open
`file="/pnfs/desy.de/dphep/scratch/h1/generator_files/djangoh14/<GEN file>"`, e.g.
`DJANGOH14.NC.EMINUSP.NOZ_NORAD.Q2GT100.CTEQ6L.A.C0000000`; conversion ran under H1OO 4.1.1
(logs cite `oo-releases/relvol10/releases/4.1.1/.../H1OOSnapshot.root`; FastJet 3.3.2 banner).
GEN-level inputs produce expected warnings (no SIPA/TCTE/TLV2/DMIS banks).

Runs and events: per-event MC run counters restart at 1 in each series and reach ~112 across the
65 chunks (NoZ logs: A.C0000000 -> run 1, A.C0000032 -> 44, B/D.C0000000 -> 1, D.C0000044 -> 76,
D.C0000064 -> 112). 154,895 events in chunk D.C0000044 ("H1Tree::Close: For tree ODS 154895 events
written") x 65 chunks = ~10.07M events per series, matching the 2021 LOGOUTPUT convention
(sum(wtx1) = 10,000,000; Output records 10000003). "Monte-Carlo Run: 1" is the initial counter of
the 2021 series-A LOGOUTPUT, not a global run number.

Known gap: `dst2all_djangoh14_BORN_NORAD/` is missing EMINUSP B C0000047 (0-byte
`fpack.DJANGOH14.NC.EMINUSP.BORN_NORAD.Q2GT100.CTEQ6L.B.C0000047.steer`; log line 8:
"Fortran runtime error: Sequential READ or WRITE not allowed after EOF marker"). The source GEN
exists on dCache (799,999,200 B, 2023-02-01), so the chunk is reconvertible.

### 2.2 2021 production / test areas (siblings)

- `djangoh14_ar4.10/`: 2021 stdjan (Grindhammer-card) full-EW NORAD production area; stock cards
  (`stdjan_nc_{eminusp,eplusp}_norad_q2gt100_cteq6l_a` with ACTIVE RNIN seeds), 8 x .LOG + 8 x .OUT
  (e+- x A-D), `SEEDS.txt` (e-: A 845990, B 845991, C 845992, D 846624; e+: A 846015, B 846016,
  C 845995, D 846623).
- `djangoh14_ar4.10_noZ/`: Apr-2021 re-runs (2 LOGs + 2 .OUTs, LPIN 8/9/11 = 0) + 2021 noZ attempt
  (exe symlinks, stdjan cards); `pnfs/` 130 GEN copies (97 GB), `oo-files/` 387 ROOT (94 GB).
- `djangoh14_ar4.10_noZ-2/`: second Apr-2021 attempt; EPLUSP LOG, 607 MB `fort.2` partial GEN,
  `pnfs/` 67 files (49 GB), `oo-files/` 198 ROOT (48 GB).
- `djangoh14_ar4.10_Q2gt60/`, `djangoh14/`, `djangoh14_lhapdf5.7.1/`, `guenterg/`, `lcg-rel4.1.1/`,
  2021-era `dst2all_djangoh_ariadne4.10`, `dst2all_djangoh_Ee27p5/p6`, `dst2all_rapgap*` dirs,
  `dst2all_test*`; symlinks `DJ`, `RG31_sl5` -> levonian job areas; `dst2all_djangoh` ->
  `dst2all_djangoh14_NORAD`.
- `EW_gZ_correction/`: Jan-Feb 2023 noZ test/validation campaign (~45 small logs, the production-style
  noZ card `stdjh14_nc_eminusp_noZ_q2gt100_cteq6l_a_2Feb23`, symlink `djangoh14_NoZ` ->
  `../../djangoh14_NoZ/djangoh14test`).
- `test_henry_2026/`: July-2026 test area (Q2GT60 test + `fix/` subdir); not audited in depth.

### 2.3 dCache GEN area (readable 2026-10-07)

`/pnfs/desy.de/dphep/scratch/h1/generator_files/djangoh14/`: EMINUSP NORAD / NOZ_NORAD / NOZ1_NORAD /
BORN_NORAD (A-D, 65 chunks each), EPLUSP NORAD / BORN_NORAD (A-D), `NC.EMINUSP.RAD` (61) +
`NC.EPLUSP.RAD` (65), empty `test/`. GEN files ~800 MB each (SPLITMB=800), no extension
(e.g. `DJANGOH14.NC.EMINUSP.BORN_NORAD.Q2GT100.CTEQ6L.A.C0000000`). No *.LOG/*.OUT on dCache.
Sibling dCache dirs: `djangoh14_ar4.10/` (520 files), `djangoh14_ar4.10_noZ/` (EMPTY).
No DUST mirror of the 2023 GEN files exists; DUST hat+mods(+ods) are sufficient for HAT-level
analysis without /pnfs.

## 3. Which executable

Evidence: FPACK title record ("18A USER: ... DATE: ... CPU: ... <title text>"), version banner,
steering-card echo in the GEN headers (dumps: `/tmp/opencode/hdr_EMINUSP_{NOZ_NORAD,NOZ1_NORAD,
BORN_NORAD,NORAD_2021,RAD}.{bin,txt}`), validation-log banners, and sigma/config matching.

- NOZ (GEN DATE 230202) and NOZ1 (GEN DATE 230205): title = Levonian's standard NORAD title
  ("NC DIS, QED non-radiative events with Q2e > 100 GeV2 ... ARIADNE 4.12 ME and CDM PS with
  MSTA(39) = 2, Albert's tuning, LHAPDF 5.7.1, 10041 = CTEQ 6L LO ...") PLUS the appended
  sentence "Special executable with Z-exchange switched off (D.Britzger)"; USER S.Levonian,
  CPU naf-hone12. Version banner "DJANGOH 1.40 ... ARIADNE 4.12 _dev" (note: this banner is
  common to ALL 2023 Levonian productions incl. BORN/NORAD/RAD and is NOT by itself a noZ marker).
  -> the 2023 "neu" noZ build `/data/dust/group/h1/britzger/djangoh14_NoZ/djangoh14test`
  (mtime 2023-02-01 13:16 = "compiled at date 10223 and time 1316", the banner in
  `log.Dj.NoZ.2Feb23.txt`, `log.Dj.NoZ.NoBORN.txt`, `log.Dj.NoZ.NoBORN.LPIN8911.txt`,
  `log.Dj.NoZ.NoBORN.LPIN8911eq0.txt`). NOT the 2021 build (banner "220221 1729", ARIADNE 4.10
  era, used for both the stock 2021 production and the Apr-2021 re-runs); NOT the older noZ build
  of the Jan-2023 tests (banner "300320 1231").
- NOZ = photon-only BORN: GEN header ACTIVE 'LPIN' 2 0 0; sigma = 6708.559 pb (SIGMT 0.670856E+04,
  SIGTOT 6.7086E+00 nb) = `log.Dj.NoZ.2Feb23.txt` (LPIN(2) CHANGED FROM DEFAULT 1 TO 0).
- NOZ1 = photon-only CORRECTED: GEN header ACTIVE 'LPIN' 2 0 1; sigma = 7285.933 pb (SIGMT
  0.728593E+04, SIGTOT 7.2859E+00 nb) = `log.Dj.NoZ.NoBORN.txt` (no LPIN(2) change). sigma differs
  from the Born value, i.e. running-alpha/SV terms active -> the FIXED build (the earlier build
  did not respond to LPIN(2)).
- SAFE sample for corrected-level photon-only physics: NOZ1 = `dst2all_djangoh14_NoZ_new/`.
  The author's `es.Norad.em.NoZ.GenLumi.steer` chains ("DjNoZ_emA_1"..., 28 chains, fPath =
  `dst2all_djangoh14_NoZ_new`, fFileNames = "DJANGOH14.NC.EMINUSP.NOZ1_NORAD.Q2GT100.CTEQ6L.
  <A-D>.C0000000-0000009" style, 10 or 5 files per chain) read ONLY the NOZ1 files. VERIFIED.
- BORN_NORAD (e- GEN title DATE 210212; e+ card vintage 230201): identical stock title WITHOUT
  the Z-off sentence; ARIADNE 4.12 _dev -> stock full-EW 2023 production exe. Config: LPIN(2)=0
  (Born) and LPIN(7)=0 (fixed alpha) ACTIVE, LPIN(4)=0, weak-SE 8/9/10/11 commented (=0).
- NORAD 2021 (dCache djangoh14/, title DATE 210212, GEN files dated 2021-02-12): stock title,
  no Z-off sentence; LPIN 8/9/11 = 1 ACTIVE (weak SE ON), LPIN(2)/LPIN(7) at defaults 1/2,
  INT3/ISM3 commented. Full-EW.
- RAD (dCache, title DATE 230208, "QED radiative events", LHAPDF 5.8.8 in title): no Z-off
  sentence -> full-EW; LPIN 8/9/11 = 1 ACTIVE; INT3(1)=INT3(2)=INT3(3)=20 and ISM3(1)=ISM3(2)=
  ISM3(3)=1 ACTIVE (ISR+FSR+Compton; CC variants 7 commented).

## 4. Loop configuration (LPIN / INT / ISM), exact card values

Sources: stock card `djangoh14_ar4.10/stdjan_nc_eminusp_norad_q2gt100_cteq6l_a` + its .OUT
"CHANGED FROM DEFAULT" lists; GEN-header card echoes (grep of '/tmp/opencode/hdr_*.txt');
validation .OUT CHANGED lists in `EW_gZ_correction/`.

| Parameter | Stock stdjan card/.OUT (2021) | dCache NORAD GEN (2021) | NOZ GEN hdr (2023) | NOZ1 GEN hdr (2023) | BORN GEN hdr (2023) | RAD GEN hdr (2023) |
|---|---|---|---|---|---|---|
| LPIN(1) | commented -> D=2 (G_mu) | commented -> 2 | commented -> 2 | commented -> 2 | commented -> 2 | commented -> 2 |
| LPIN(2) | commented -> D=1 (Born+SV) | commented -> 1 | ACTIVE 0 (Born only) | ACTIVE 1 (Born+SV) | ACTIVE 0 (Born) | commented -> 1 |
| LPIN(4) | ACTIVE 0 (.OUT: 1->0) | ACTIVE 0 | ACTIVE 0 | ACTIVE 0 | ACTIVE 0 | ACTIVE 0 |
| LPIN(7) | commented -> D=2 (running alpha) | commented -> 2 | ACTIVE 2 | ACTIVE 2 | ACTIVE 0 (fixed alpha) | commented -> 2 |
| LPIN(8) | ACTIVE 1 (.OUT: 0->1) | ACTIVE 1 | commented -> 0 | commented -> 0 | commented -> 0 | ACTIVE 1 |
| LPIN(9) | ACTIVE 1 | ACTIVE 1 | commented -> 0 | commented -> 0 | commented -> 0 | ACTIVE 1 |
| LPIN(10) | commented -> 0 | commented -> 0 | commented -> 0 | commented -> 0 | commented -> 0 | commented -> 0 |
| LPIN(11) | ACTIVE 1 | ACTIVE 1 | commented -> 0 | commented -> 0 | commented -> 0 | ACTIVE 1 |
| INT2(1)/ISM2(1) | ACTIVE 1 (.OUT: 0->1) | 1 | ACTIVE 1 (validation .OUT: 0->1) | ACTIVE 1 | ACTIVE 1 | ACTIVE 1 |
| INT3/ISM3 | all commented (off) | all commented (off) | commented (off) | commented (off) | commented (off) | INT3(1,2,3)=20, ISM3(1,2,3)=1 ACTIVE |

Common to all five (card echo + validation .OUT CHANGED lists): EELE 27.6 GeV, EPRO 920 GeV,
Q2MI 100 GeV^2, LLEP -1/+1, LST(15)=10041 (CTEQ 6L LO via LHAPDF), LST(16)=2, MCWE 1 150
(weighted writes), NPVE 50000 (.OUT: 1000->50000), P112 0.235, NFMA 5, WSOPHIA 5, LST(12)=5,
IARIAD=1, ARIADNE CTUN '4.08' with PARA(10)=1.2, PARA(25)=1.2, PARA(27)=0.9, MSTA(34)=0,
MSTA(39)=2. Seeds: 2021 stdjan card has ACTIVE RNIN (845990 for series A e-; full list in
`djangoh14_ar4.10/SEEDS.txt`); all 2023 cards have RNIN commented (*RNIN 314159) -> auto seeds;
2023 per-series seeds are NOT recoverable from cards/logs (would need ISEED from GEN headers or
the levonian job area via the `DJ` symlink).

Archived noZ card note: `EW_gZ_correction/stdjh14_nc_eminusp_noZ_q2gt100_cteq6l_a_2Feb23` has the
LPIN(2) line COMMENTED, i.e. it corresponds to the NOZ1/corrected variant; the NOZ/Born production
ran the same card with LPIN(2)=0 enabled (per its GEN header).

## 5. Cross-sections and the "Visible x-sect" mismatch

### 5.1 2021 LOGOUTPUT blocks (Q2 > 100, e+-)

| LOGOUTPUT | Tot x-sect (pb) | N_generated | N_written (sum wtx1) | "Visible x-sect" (pb) | Date/Time |
|---|---|---|---|---|---|
| `djangoh14_ar4.10_noZ/DJANGOH14.NC.EMINUSP.NORAD...A.LOG` (re-run, 8/9/11=0) | 7329.79150 +/- 0.73298 | 23,126,597 | 10,000,000 | 3169.42065 | 210420 18:47 |
| `djangoh14_ar4.10_noZ/DJANGOH14.NC.EPLUSP.NORAD...A.LOG` (re-run) | 7293.94678 +/- 0.72939 | 23,249,632 | 10,000,000 | 3137.23096 | 210420 18:49 |
| `djangoh14_ar4.10/DJANGOH14.NC.EMINUSP.NORAD...A.LOG` (original, 8/9/11=1) | 7323.93945 +/- 0.73239 | 23,139,834 | 10,000,000 | 3165.07861 | 210304 06:25 |
| `djangoh14_ar4.10/DJANGOH14.NC.EPLUSP.NORAD...A.LOG` (original) | 7287.91504 +/- 0.72879 | 23,240,288 | 10,000,000 | 3135.89697 | 210226 20:50 |

RESOLUTION (VERIFIED, arithmetic reproduced): "Visible x-sect" = sigma_phys x (N_written /
N_generated) - a MCWE (1 150) weighted-write bookkeeping quantity, NOT a cross-section:
- 7329.79150 x 10,000,000 / 23,126,597 = 3169.4207 pb (LOG: 3169.42065)
- 7293.94678 x 10,000,000 / 23,249,632 = 3137.231 pb (LOG: 3137.23096)
- originals: 3165.079 / 3135.897 pb (also reproduce)
The physical cross-sections are the SIGMT values of the same runs:
- Stock original .OUT (LPIN 8/9/11=1): SIGMT 0.732394E+04 pb (e-) / 0.728791E+04 pb (e+) =
  7.3239 / 7.2879 nb.
- Apr-2021 re-run .OUT (LPIN 8/9/11=0): SIGMT 0.732979E+04 pb (e-) / 0.729395E+04 pb (e+) =
  7.3298 / 7.2939 nb -> these are the "7.3298/7.2939 nb" of the question.
Weak fermionic SE (LPIN 8/9/11) is worth +5.85 pb (e-, full-EW).

### 5.2 NoZ photon-only cross-sections (e-, Q2 > 100, production settings)

From the Feb-2023 same-config validation logs of the neu exe (banner 10223 1316):
- NOZ Born: X-SECTION IN PB WITHIN CUTS (LINIT) = 6708.559 pb; SIGMT 0.670856E+04; SIGTOT
  6.7086E+00 nb (`EW_gZ_correction/log.Dj.NoZ.2Feb23.txt`).
- NOZ1 corrected: 7285.933 pb; SIGMT 0.728593E+04; SIGTOT 7.2859E+00 nb
  (`EW_gZ_correction/log.Dj.NoZ.NoBORN.txt`).
- NOZ1 + weak SE (LPIN 8/9/11=1): 7287.827 pb; SIGTOT 7.2878E+00 nb
  (`EW_gZ_correction/log.Dj.NoZ.NoBORN.LPIN8911.txt`); LPIN8911eq0 variant reproduces 7.2859.
- Z-exchange contribution (e-, Q2 > 100, full-EW minus photon-only): 7329.79 - 7285.93 =
  43.86 pb (~0.6%).
CAVEAT (PROVISIONAL link): production-scale (10M-event) LOGOUTPUT/STDOUT for the NOZ/NOZ1
productions do not exist on DUST or dCache (searched); the sigma values above are validation-run
values, tied to the productions by identical card echo (GEN headers) and identical exe banner.

### 5.3 Author exposure numbers (H1OO fLumi, pb^-1)

`/data/dust/group/h1/britzger/EventShapes/KinFitBIRM_git/Steering/`:
- NOZ1 chains: fLumi = 12964 (`es.Norad.em.NoZ.GenLumi.steer`; 28 chains over NoZ_new).
- NORAD e-: 12841.587; NORAD e+: 12970.73 (NORAD blocks in the NoZ/BORN steers and the
  disMC10/13 Norad2021 steers).
- BORN e-: 13960; BORN e+: 14108 (`es.Norad_BORN.{em,ep}.GenLumi.steer`, BORN_NORAD blocks,
  fFileNames "DJANGOH14.NC.E(MINUS|PLUS)P.BORN_NORAD.Q2GT100.CTEQ6L.<A-D>.C0000000-0000009").
Observation (PROVISIONAL, interpretation deferred to h1-normalization): fLumi x sigma ~ 94.0-94.5M
for both NOZ1 (12964 x 7285.93 pb) and NORAD e- (12841.587 x ~7324-7330 pb), suggesting
fLumi = N_generated/sigma; the steers do not document the derivation. N_GenDIS does not appear in
any generator or dst2all log (H1OO HAT-level quantity).

## 6. e+ and RAD photon-only coverage

- NO photon-only e+ sample exists: dCache has EPLUSP only for NORAD and BORN_NORAD; no EPLUSP
  NOZ/NOZ1 on dCache or DUST (NoZ/NoZ_new dirs: EPLUSP count 0, runall.EMINUS.sh only). VERIFIED.
- BORN_NORAD e+ exists but is full-EW fixed-alpha Born (no Z-off sentence; LPIN(7)=0), NOT
  photon-only; the different fLumi (14108 vs 13960 for e+) reflects the Z-induced charge asymmetry.
- NO photon-only RAD sample exists: the only RAD is full-EW (title 230208, LPIN 8/9/11=1,
  INT3/ISM3 active).
- dCache `djangoh14_ar4.10_noZ/` is EMPTY.
=> A photon-only e+ or photon-only radiative correction cannot be taken from existing samples;
options (reweighting vs new production) are a physics/analysis decision (h1-mc-expert /
h1-analysis-critic), not a provenance question.

## 7. ID semantics (no genID / ReqID)

These are private generator productions (Levonian/Britzger), not H1 MC-DB productions. No MC-DB
row, ReqID, or genID label exists: GEN headers contain no ReqID/GenID/request strings (grep of
header dumps empty); FPACK title records carry only USER/DATE/CPU. Filename fields
`DJANGOH14.NC.<EPLUSP|EMINUSP>.<FAMILY>.Q2GT100.CTEQ6L.<A-D>.C<NNNNNNN>` are: generator, process,
beam charge, sample family, Q2-cut tag, PDF tag, series letter (one ~10M-event production each),
chunk index (800 MB split). Series letter != ReqID; chunk number != genID. Any MC-DB mapping:
INSUFFICIENT EVIDENCE (and no MC-DB source labels these samples).

## 8. Parameter coverage and open items

Searched: directory listings and file counts (all five dst2all dirs + siblings), du sizes, ods
mtime ranges, per-chunk dst2all logs (run numbers, event counts, H1OO release), fpack steers,
dst2all.fpacksteer, runall scripts, GEN FPACK title records (USER/DATE/CPU, Z-off marker), version
banners, steering-card echoes (LPIN/INT/ISM/EELE/EPRO/Q2MI/LLEP/RNIN/MCWE), LOGOUTPUT
cross-section blocks, .OUT SIGMT/CHANGED lists, compile banners, EventShapes GenLumi steers
(fLumi, fPath, fFileNames), SEEDS.txt, dCache listings. Not accessible / not preserved:
production-scale LOGOUTPUT/STDOUT for the 2023 NOZ/NOZ1 runs; 2023 per-series seeds; MC-DB fields
(not applicable). No live MC-DB CGI query was needed (no MC-DB productions involved).

Open items / next lookups: none required for provenance. Delegation: LPIN physics meaning,
BGcheck/ParticleSource semantics, and the construction of a photon-only C_rad from
NOZ1/NORAD/RAD -> h1-mc-expert; fLumi/oomclumi normalization policy -> h1-normalization;
analysis-gating criticism -> h1-analysis-critic.

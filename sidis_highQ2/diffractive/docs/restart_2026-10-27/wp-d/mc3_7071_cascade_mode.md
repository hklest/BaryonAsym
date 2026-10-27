# MC-3: cascade mode of 7071 and the ND ME+PS / CDM sets (h1-mc-db, 2026-10-27)

**Verdict (VERIFIED, generator-log level).** 7071 is **RAPGAP 3.1038 ME + Pythia-type PS (IFPS=3), not ARIADNE CDM**. The DB comment "ARIADNE ME and CDM ps" is a transcription error that the DB itself corrects. 7091 and 8955-8962 are ARIADNE CDM. 7094/7096/7097 and 8947-8954 are RAPGAP ME+PS with steering identical to 7071. 8240-8271 are LEPTO-type ME+PS (LST(8)=12, ARIADNE off). The prior "7071 vs 7091" envelope was therefore ME+PS vs CDM, not CDM vs CDM; its defect is the period mismatch (em06 vs ep0607), not the cascade class.

Sources: `DB` = `/afs/desy.de/group/h1/iww2/imc/h1db.js` (IMC snapshot, 2019-03-15; `SetgenDB(ID,GENERATOR_NAME,FILE_NAME,LUMI,MAIN_CUTS,...,commentUSERNAME,commentTYPE,commentTEXT,REQUEST_ID)` line 2201; `SetrequestDB(ID,PARENT_ID,RUN_PERIOD,...,SW_VERSION,EVENTS,...)` line 1). `LOG` = `/afs/desy.de/group/h1/iww2/imc/logfiles/<gen>/gen_<genID>.txt` (AFS mirror of the 401-protected web logs). `OO` = `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/<gen>/<ReqID>/` listing (hat/mods counts by `ls | grep -c '^hat\.'`).

## 1. 7071 (genID 5031, RAPGAP31, 2006_e-, SW 72)
- DB `SetgenDB(5031,...)` line 2586, MAIN_CUTS: "NC DIS, e-p, 10041 = CTEQ 6L LO, QED radiative events using ARIADNE ME and CDM ps, Q2min = 60 GeV2, ...". Same row, commentUSERNAME `guenterg`, commentTYPE `INFO`, commentTEXT: **" 19 Feb : in the comments: 'ARIADNE ME and CDM ps' is wrong, it should be 'RAPGAP ME and PS'."**
- LOG `rapgap31/gen_5031.txt` header (lines 8-14): "Heracles 4.6.1, Rapgap 3.1038, Pythia 6.212, LHAPDF 5.7.1 ... 10041 = CTEQ 6L LO ... Special Pythia settings for higher resonances (ALEPH tuning)." No ARIADNE/CDM wording.
- LOG line 94 `'IPRO' 1 0 1200`; line 109 `'IDIR' 1 0 1`; line 119 `'NFQC' 1 0 3`; line 173 **`'IFPS' 1 0 3 ! (D=3) P.S.`** with the option list at 174-177 "=3 initial and final state PS / IFPS = 10 ARIADNE". Active `IFUL 1`, `IORD 1`, `ISOG 1` (shower cards, lines 90-240). No active `MSTA`/`AR40`/`ICCFM` card anywhere; the GARI block (lines 429-520) is entirely commented. Runtime section (lines 882-1031) prints no ARIADNE banner.
- Card-level identity with the acknowledged ME+PS set: the 203 active cards of `gen_5031.txt` and `gen_5062.txt` (7094 parent) differ in **one** card only, `'LEPI' 1 0 11` vs `-11` (beam lepton), diff run 2026-10-27.

## 2. The other ND sets (all parents checked individually)
| ReqID(s) | genID(s) | Generator | Cascade switch in LOG | Class | SW / period | OO hat+mods |
|---|---|---|---|---|---|---|
| 7071 | 5031 | RAPGAP 3.1038 | `IFPS 3` (l.173), no `AR40` | **ME+PS** | 72 / 2006_e- | 61+61 |
| 7094 / 7096 / 7097 | 5062 / 5063 / 5064 | RAPGAP 3.1038 | `IFPS 3` (l.174 each); DB comment "RAPGAP 3.1038 ME and PS, NFQC=3, scale q2 = Q2 + pt**2" (DB l.2613-2615) | **ME+PS** | 72 / 06_07e+, 04_05_e-, 2004_e+ | 121+121, 121+121, 61+61 |
| 7091 | 5058 | DJANGOH 1.4 + ARIADNE 4.10 | `'AR40' 0 0 1` (l.363, "switch ARIADNE on/off"), `'MSTA' 39 0 2` (l.553); `LST 8` card commented (l.255, doc: "9 = set by ARIADNE ... CDM") | **CDM** | 72 / 06_07e+ | 201+201 |
| 8138 | 5058 | same GEN file as 7091 | as 7091 | **CDM** | 77 / 06_07e+ (DB l.1137) | 200+200 |
| 8240-8271 | 5547-5578 (map DB l.1217-1248) | DJANGOH 1.4, "ME+PS (LEPTO) DEFAULT OPTIONS" (header l.10) | all 32 logs: `'AR40' 0 0 0` (l.286) and **`'LST ' 8 0 12 ! (D=12) QCD effects`** (l.201; doc l.202-207: "2=parton shower ... 9=CDM ... 12-14=ME+PS as above") | **ME+PS (LEPTO)** | 77 / four periods, Q2>60/100/1000/10000 slices | 20+20 per 2M slice (8240, 8260, 8261); 8271: 2+2 |
| 8947-8954 | 6034-6041 | RAPGAP 3.1038 | `IFPS 3`, no `AR40`/`MSTA`, header "RAPGAP 3.1038 ME and PS" (all 8 logs) | **ME+PS** | 84 / four periods | 188,201,201,101,201,166,201,14 |
| 8955-8962 | 6042-6049 | DJANGOH 1.4 + ARIADNE 4.12 | `'AR40' 0 0 1`, header "ARIADNE 4.12 ME and CDM PS" (all 8 logs) | **CDM** | 84 / four periods | 188,201,186,201,201,141,201,54 |

All of the above use PDF 10041 = CTEQ6L (DB MAIN_CUTS / filename token `CTEQ6L`); ND samples carry no DPDF. 7071/7091/7094-7097/8947-8962 share the E2start<=150 weighting (DB comments); 8240-8271 are "unweighted" or "weighted Q2<500" (LOG l.12 of 5563/5577) and lack the Grindhammer/Albert tunings.

## 3. Genuine CDM vs ME+PS pairs with HAT+MODS present
1. **SW 72, 06_07e+, CTEQ6L, same E2start weighting: 7091 (CDM) vs 7094 (ME+PS).** Both already v6dif-processed (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/output_v6dif/mc/{7091,7094}`). Recommended fallback pair. Residual non-cascade differences: hard-process generator (DJANGOH/LEPTO vs RAPGAP), Q2e vs Q2h cut, tunings.
2. **SW 84, all four periods: 8947-8954 (ME+PS) vs 8955-8962 (CDM).** HAT+MODS on OO for all 16; none yet v6dif-processed (MC13 option of gap MC-4).
3. **SW 77, 06_07e+, same generator family: 8138 (5058 CDM re-sim) vs 8260/8261 (DJ14 LEPTO ME+PS Q2>60 / Q2>100).** Cleanest cascade-only swap in principle, but the MEPS side has 20+20 files per slice and different weighting/tuning; cross-check only. 8138 is not in the gap-list inventory.
- 7071 vs 7091 is a valid ME+PS vs CDM pair in class, but mismatched in period (em06 vs ep0607) and the lepton charge; 7071 (em06) has no CDM partner at SW 72 (the 5058 GEN is e+ only).

## 4. Coverage, blockers, notes
- Parameter coverage: searched by ReqID, parent genID, generator token, SW_VERSION, RUN_PERIOD, MAIN_CUTS/comment text, correction comments, GEN/OO/DST paths, and generator-log steering cards (IPRO, IDIR, NFQC, IFPS, AR40, LST(8), MSTA). Not accessible: live IMC CGI (`https://www-h1.desy.de/cgi-bin/imc` -> 200, "H1 scripts are disabled" placeholder, 664 B); web logs `https://www-h1.desy.de/imc/logfiles/{rapgap31,djangoh14}/gen_{5031,5058,5062,5547}.txt` -> HTTP 401 `Basic realm="H1 internal"` with `-k`, TLS failure without; `/acs` DST not mounted.
- Trap: `/afs/desy.de/group/h1/iww2/imc/steering_files/mcrequest_<N>` is keyed by **ReqID** (`mcrequest_5563` is a SW 53 DVCS request, parent 3735), carries only H1SIMREC Fpack steering, and contains no generator cards.
- Supersedes gap list `mc_gap_list.md` §3 table row "CDM (ARIADNE) ... 7071 (RAPGAP31) [P]" and the "CDM vs CDM" prior-envelope paragraph (§3) and the MC-3 fallback "treat 7071 as CDM".
- Delegation: whether IFPS=3 RAPGAP showers vs LEPTO LST(8)=12 showers are physically distinct enough to count as two ME+PS edges, and the truth-bank check of dipole status codes, belong to `h1-mc-expert`.

# Djangoh / Rapgap with QED radiation in the hit-level chain: feasibility

Date: 2026-10-27. Scope: can QED-radiative (HERACLES) Djangoh and/or Rapgap events, ideally the
"standard" official samples, be pushed through `hit_level_sim/hits_chain_wrapper.sh` to make hit
samples equivalent to the Sherpa/Pythia8 ones (Q²>150 and 5<Q²<100, 2006-07 e⁺)? Routes:
A = re-simulate official generator-level files, B = run the generators ourselves, C = reuse official
DST/MODS. Read-only study; no jobs run. `$H` = `/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim`,
`$P` = `/pnfs/desy.de/dphep/online/h1/mc` (`.../h1/mc2` is a symlink to it).

## 1. How non-HepMC input enters the chain (verified)

- The wrapper has two entry points. Sherpa/Pythia8 → HepMC3 → converter → `h1hepevt` → BOS
  (`$H/hits_chain_wrapper.sh:111-112`, h1hepevt at `:166-173`); lAger is the `else` branch at
  `:133-137` (event window `SKIP=JOB_IDX*NEV`). Preprocessing (SIMRUN + ADDRAND) reads
  `${DST_BOS}` at `:177-184` through `__DSTIN__` in `$H/steering/official_preprocessing_HE.tmpl`.
- An H1 BOS generator file (official or Djangoh/Rapgap `BOSOUTPUT`) can go straight to
  preprocessing, bypassing the HepMC converter and `h1hepevt`: `$H/HANDOFF.md:148-150` ("this route
  is not wired into the wrapper").
- Precedent runs exist: `$H/rapgap_build_check.sh` (official file, 300 ev) and
  `$H/gen_build_check.sh` (private Djangoh file). Rapgap: `pre.log:180-182` in
  `$H/sherpa_pilot/rapgap_build_check/cut_0.0/` reports 299/300 events simulated (the known
  one-event loss, HANDOFF §9); `hits.root` holds 298 events, 91 branches (`$H/rapgap_hits.sh`).
- Caveat: both precedents ran bare `h1simrec9` from PATH (`rapgap_build_check.sh:12`,
  `gen_build_check.sh:10`), i.e. the installed ksfitt-bugged binary; outputs predate the patched
  `$H/h1simrec_fix/h1simrec9` (2026-10-21 18:48). Hits/truth are unaffected, HAT is not (HANDOFF §6).
- Event windows: the template selects `RECORD=1:`; the official 7094 steering uses the range form
  `RECORD=1:10000` (`$H/steering/official/h1simrec_06_07e+_7094_mainsteer.txt:5`). A BOS branch
  needs a `RECORD=<skip+1>:<skip+NEV>` placeholder (INFERRED: counts RUNEVENT records; test at an offset).

## 2. Route A: official generator-level files (verified, h1-mc-db)

Source: IMC DB `/afs/desy.de/group/h1/iww2/imc/h1db.js` (`SetrequestDB` line 1, `SetgenDB` line 2201),
gen logs under `/afs/desy.de/group/h1/iww2/imc/logfiles/`.

| Sample (role) | genID / ReqID | QED | Q² gen cut | GEN-level BOS on disk? |
|---|---|---|---|---|
| DJ14 NC e⁺ 0607 (highQ2 main) | 5058 / 7091 | RAD (log gen_5058 l.9) | >60 | No: tape only, `/acs/mc2/...` not mounted |
| DJ14 e⁻06, e⁻05, e⁺04 (highQ2) | 5035, 5060, 5061 / 7072, 7092, 7093 | RAD | >60 | No (tape) |
| RG31 NC e⁺ 0607 (highQ2) | 5062 / 7094 | RAD | >60 | No: DB "All files lost due to broken tape" |
| RG31 e⁻06/05, e⁺04 | 5031, 5063, 5064 / 7071, 7096, 7097 | RAD | >60 | No (5064 lost, 5063 partly) |
| DJ14 low-Q² (lambda, lowQ2 SIDIS) | e.g. 5753 / 8709; 93 ReqIDs 8709-8743, 8872-8929 | RAD (gen_5753 l.9) | >2 | No (tape) |
| RG31 low-Q² (lambda, lowQ2 SIDIS) | e.g. 5730 / 8686; 35 ReqIDs 8686-8708, 8930-8941 | RAD | >2 | No (tape) |
| DJ14 / RG31 low-Q² NONRAD (C_rad twins) | 5855-5877 / 5878-5894 | NONRAD | >2 | Yes, `$P/generator_files/{djangoh14,rapgap31}/<genID>/` |
| RG31 charm (not standard) | 5850 / 8790 | RAD | >4, ccbar only | Yes (64/200 files) |

Sample lists: `sidis_highQ2/docs/status/normalization-gate-paired-leg-elecE-up-s2prime-2026-10-11.md:36-37`,
`sidis_highQ2/context-handoff.md:259`, `lambda/context-handoff.md:1610-1612`.
DB inconsistency: `SetgenDB(5855)`/`(5866)` say RADIATIVE_MC="Y" but the filename, comment and log
(`djangoh14/gen_5855.txt:9`) say non-radiative; trust the log.

**Verdict A (strict): blocked.** No QED-on standard GEN file is on disk; the only RAD file on disk
(5850) is charm-only and must not stand in for inclusive DIS (it is the file the Rapgap precedent used).
Staging from `/acs` tape is possible only via DESY-IT / H1 data preservation (not checked).

### A′: generator record recovered from official DSTs (new, partially verified)

- Low-Q² official DSTs are on dCache: DJ14 RAD 8709 (`$P/djangoh14/07/8709/`, 300 files, e.g.
  `DJANGOH14.DIS.NC.JOINT.LOWQ2.POSITRON.2007.RAD.RW50.1.8709.S39600.R97905.ftt.DST.C0000001`,
  573 MB) and RG31 RAD 8686 (`$P/rapgap31/07/8686/`, 388 files). VERIFIED by `ls`.
- Bank census (`$H/bankscan.sh`, first 20 MB of that 8709 file): `GTR:404 STR:395 SVX:377 HEAD:366
  CJKR:386 CJKT:336 CSTS:332 DTRA:353`, `CRJT/CRJE/CRJX/CTJX/CRZT/CRZE:0`. So the official DST keeps
  the generator record (GTR) but not the CJC raw-hit banks.
- The highQ2 standard ReqIDs 7091/7094 have empty dstPATH (h1-mc-db): A′ covers **low Q² only**.
- Two ways to re-simulate from those DSTs (both UNTESTED):
  1. Feed the DST directly to preprocessing (h1simrec reads GTR/GKI; whether it ignores or chokes
     on the old STR/SVX/reco banks is unknown; may need a bank DROP/SELECT step).
  2. `dst2hepmc` (`/data/dust/group/h1/klesthen/h1hepmc/dst2hepmc.cc:55-70`, reads `GTR`) →
     HepMC3 → `H1HepMC_exportBOS` → BOS generator file. Only a centos7-gcc9 build exists
     (`h1hepmc/bin/x86_64-centos7-gcc9-opt`); needs an EL9 rebuild. The Sherpa converter is not used.
- INFERRED risks: GTR may be a pruned record (enough for re-simulation, since h1sim tracks only
  stable particles, but check status/daughter links); events already carry an assigned run/vertex
  that SIMRUN overwrites.
- VERIFIED: the standard samples are **weighted**: `h1db.js:2609` (5058: events weighted for
  low Q², `weight = max(1.0, E2start/...)`) and `h1db.js:3052` (5753: "Weight = 50"). Per-event
  weights must be carried into the hits output and L = Σw/σ; this also applies to route-B cards copied from 5058. Events are the *same* events as the official sample, which is
  the point of "standard sample included".

## 3. Route B: run the generators ourselves (verified, researcher-worker)

**Djangoh 1.4 with HERACLES: binary and tooling exist, but RAD generation has never completed.**
- EL9 binary in the release: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/x86_64-el9-gcc14-opt/bin/djangoh14test`
  (`ldd`: no missing libs; links `BOSOUTPUT`, `HEPEVT`). Writes H1 BOS, so output enters preprocessing with no converter.
- Condor campaign tooling: `/data/dust/group/h1/klesthen/Djangoh14/{campaigns,scripts}`, driver
  `scripts/prepare_rad_campaign.py`. All large private campaigns (500-530 × 10k) are NONRAD Q²>100.
- The RAD smokes `campaigns/hk_0123_rad_eplus_smoke_20260920` (`DJANGOH14.NC.EPLUSP.0607.RAD.Q2GT60.CTEQ6L`,
  a replica of the 7091 card) and `hk_0123_rad_eminus_smoke_20260920` **aborted at the first
  event**: `.../stage1_djangoh/jobs/job_0000/log/djangoh.stdout.txt:2406,2471` (e⁺; e⁻ 2393,2459) "Cannot use EFGH model (MSTA(39)>0) with
  initial state hadrons ... Error is fatal". Their BOS files are 138600 B (header only); the
  `stage1.ok` marker saying `state=ok` is a false positive. The cards keep the official
  ARIADNE `MSTA(39)=2` (also in the 5058 card, `h1db.js:2609`; see the known trap in
  `scripts/verify_campaign_seeds_20260916.py:23`). The smokes ran a different binary
  (`/data/dust/group/h1/britzger/norad_prod21/test_henry_2026/djangoh14test`), so whether the
  release binary accepts the card is untested.
- Steering facts (e⁻ smoke `steering/stdjan_0000.steer`): `OPEN BOSOUTPUT ... RECL=27720` (l.13-15),
  `NEVENT 10000` (l.59), `RNIN 930000` (l.120, seed), `Q2MI 60.` (l.200), `'LPIN' 4 0 1` QED on
  (l.239), `LST 15=10041` (CTEQ6L, l.262).
- No low-Q² RAD private steering exists; it would be derived from
  `/afs/desy.de/group/h1/iww2/imc/logfiles/djangoh14/gen_5753.txt` (Q²>2, 0<y<1).
- The only Djangoh hit-chain test used a NONRAD Q²>100 e⁻ file
  (`$H/sherpa_pilot/gen_build_check/djangoh_hiQ2_em/pre.steer:17`) with the bugged binary and never ran `dst2ods`.
- **Verdict B (Djangoh): feasible pending the MSTA(39) fix** (or a deviation MSTA(39)=0 from the standard card).

**Rapgap 3.3: possible but costly.**
- Sources and H1 interface on AFS: `/afs/desy.de/group/h1/h1gen/rapgap33/` (`rapgap-3.305`,
  `h1rapgap/*.F`, README with configure line), steerings `h1rapgap/rapgap-rad.steer`
  (`'LPIN' 4 0 1`, `OPEN BOSOUTPUT`) and `rapgap-nrad.steer`.
- No EL9 build: centos7 binaries miss `libLHAPDF.so.0`, `libreadline.so.6`, `librapgap33.so`,
  Ariadne `libar4`; no rapgap/ariadne in `/cvmfs/sft.cern.ch/lcg/releases/MCGenerators/`.
- Physics caveat: the standard samples are Rapgap **3.1** (IPRO 1200), not 3.3; a 3.3 sample is
  "Rapgap-like", not the standard sample. The RAD seed card `*RNIN` is commented out (debug only).

## 4. Route C: official DST/MODS as hit samples (verified: not usable)

- Official DSTs lack the CJC raw-hit banks (census above; `$H/HANDOFF.md:21`,
  `$H/README_SHERPA_HITS.md:7`) and were made with `SKECUT(CENT)=999`, so secondaries carry only ancestor labels (HANDOFF §5).
  The hits tree cannot be built from them. **Blocked.**

## 5. QED topology in the truth record (verified, with gaps)

- BOS route: the Sherpa converter (`/data/dust/group/h1/klesthen/BaryonAsym/sim_smoketest/sherpa/convert_hepmc3_to_hepevt.py:195-199`,
  highest-E status-1 beam-flavour lepton) is bypassed, so its radiative mis-identification does not
  apply. It would apply if Rapgap 3.3 were run with HepMC output instead of BOS.
- `$H/extract_hits.C:127-145,259-265` stores STR rows as px,py,pz,e,pdg,parent,gtr,vtx: no status
  code, no ISR/FSR flag, and the hits tree has **no Q²/x/y branches**. `PruneTruth`
  (`extract_hits.C:431-451`) keeps every `str_gtr>0` row, so radiated photons survive (Rapgap
  test: 1.40 generator e± per event on average, event 0 has 8 generator e/γ rows).
- Consequence: extraction does not break, but downstream users must identify the scattered lepton
  and radiated photon themselves. Recommended small addition: store per-event generator Q², x, y
  (Born and lepton-side) from the GKI/HEAD generator bank, as HAT `Q2sGen/YsGen` does for official
  BOS input (HANDOFF §5). INFERRED: GTR column semantics (status/mother) not checked.

## 6. Kinematic windows, cost

- Generator cuts are defined on generator-level (lepton-vertex) Q². With QED on, true and lepton Q²
  differ, so generate looser than the window: Q²>60-100 for the Q²>150 window (the 7091 card is
  Q²>60), Q²>2-4 for 5<Q²<100 (the low-Q² cards are Q²>2). Window cuts then go at analysis level.
  Efficiency: at Q²>60 only a fraction of events have Q²>150 (σ ratio not computed here).
- Condor cost from the precedents: about 2.6 s/event preprocessing+simrec (Rapgap 299 ev in 13 min;
  Djangoh 298 ev 13.6 min), consistent with HANDOFF §8 (0.95-2.7 s/event + ~35 s startup).
  250 events ≈ 11 CPU-min, fits the existing 1 h / 3 GB / 4 GB job. Disk: sim.dst about 1.0 MB/ev at
  SKECUT 0 (300 MB for 300 ev); use `KEEP=hits` (HANDOFF §8). Djangoh generation itself is
  seconds per 10k-event job (INFERRED from campaign sizes; not measured).
- Seeds: no RNDM/SEED cards in the preprocessing/simrec templates; per-job distinctness of
  GEANT/ADDRAND randoms is unverified (INFERRED: run/event-number derived). With a single input
  file split by `RECORD` windows, check two jobs overlay different random-trigger events.

## Recommended route

1. **Low Q² (5<Q²<100), Djangoh and Rapgap: route A′.** Official RAD DSTs 8709 (DJ14) and 8686
   (RG31) are on dCache with GTR: these are the standard events themselves. Effort ~1 day (DST →
   preprocessing test, `GEN=h1bos` wrapper branch, weight carry-through).
2. **High Q² (Q²>150), Djangoh: route B pending the MSTA(39) fix** (release EL9 binary, card from
   the 5058 log). Effort ~1 day beyond item 1 if the fix is a steering change; unknown otherwise.
3. **High Q² Rapgap:** no standard events exist (7094 lost, no DST); only an EL9 build of
   Rapgap 3.3 (1-2 days, not the 3.1 standard) or tape staging of other periods.

Outline:
1. Rerun one 100-event RAD Djangoh smoke on Condor with the release EL9 binary and unchanged card;
   if it aborts, fix MSTA(39)/Ariadne steering; confirm the BOS file is larger than the header.
2. Test A′ option 1: one 8686 DST file → preprocessing, 10 events; inspect log for bank errors.
3. If 2 fails: rebuild `h1hepmc` on EL9, run `dst2hepmc` → `H1HepMC_exportBOS` on one file, then preprocessing.
4. Add `GEN=h1bos` to `hits_chain_wrapper.sh`: `BOS_INPUT`, skip stages 0/1 and `h1hepevt`.
5. Add a `__RECORD__` placeholder to `official_preprocessing_HE.tmpl`; window = `[i·NEV+1, (i+1)·NEV]`.
6. Replace the HEPEVT crash-split (`:200ff`, `split_hepevt.py`) by RECORD sub-windows for BOS input.
7. Verify an offset window (job 3) selects the intended events and a different random-trigger overlay than job 0.
8. Write Djangoh RAD cards: e⁺ 0607 high-Q² (copy of the smoke card, Q2MI per decision) and low-Q² from gen_5753.
9. Generate Djangoh BOS files on Condor with fresh `RNIN` seeds (record offsets in HANDOFF §3 table).
10. Optional: add generator Q²/x/y (Born and lepton-side) branches to `extract_hits.C`.
11. Pilot: 20 × 250 events per sample; compare acceptance and y spectrum with Sherpa/Pythia8 pilots.
12. Production with `KEEP=hits`, N per decision; L = Σw/σ, per-event weight carried into hits.root.
13. Document in HANDOFF §3/§7 and README; note which binary and which input provenance each sample used.

## Open decisions

1. High-Q² Djangoh: accept private RAD regeneration (same card, new events) as "the standard
   Djangoh", and if the card's MSTA(39)=2 aborts also with the release binary, accept MSTA(39)=0
   (a deviation), or instead request tape staging of the official 5058 GEN files from DESY-IT?
2. Rapgap high Q²: build Rapgap 3.3 on EL9 (not the 3.1 standard), stage tape, or skip?
3. Generator-level Q² cut for the high-Q² window (60 like 7091, or ~100 for efficiency) and sample sizes.
4. QED-on samples break the "QED off" parity with Sherpa/Pythia8 (decision
   `.supervisor/memory/decisions/2026-10-27-pythia8-qed-showers-off-2.md`): keep both as separate sample classes?

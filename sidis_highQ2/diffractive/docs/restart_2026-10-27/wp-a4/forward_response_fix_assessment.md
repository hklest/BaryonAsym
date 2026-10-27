# WP-A4: forward-response mismatch: simulation, reconstruction or physics, and what a fix would take

Date 2026-10-27. Answers the user question "Could you look in to what would be needed to fix these issues and run new
simulation? Is it a reconstruction issue or simulation or both?" after the G1 and G1′ KILLs
(`D/docs/restart_2026-10-27/wp-a/g1_verdict.md`, `D/docs/restart_2026-10-27/wp-a2/g1prime_verdict.md`) and the WP-A3
"does not factorise" ruling (`D/docs/restart_2026-10-27/wp-a3/response_reweighting_scoping.md`).
Abbreviations: `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`; `HS` =
`/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim`; `IMC` = `/afs/desy.de/group/h1/iww2/imc`; `K` / `C` =
`D/condor/restart_2026-10-27/wp-a4/logs/{kin_5917311,comp_5917453}.out` (kinematic sweep / composition fit). Tags: [V]
verified from the cited file, log or paper; [I] inference or specialist judgement. S = simulation (geometry, material,
digitisation, noise and random-trigger overlay); R = reconstruction (clustering, noise suppression, calibration, HAT/ODS
filling); P = common physics modelling (generator forward flow, proton dissociation, cocktail composition).

**Short answer.**
- It is mostly simulation, with a physics-modelling part that cannot be fully separated; reconstruction is minor (§1).
- No component can be fixed by re-reconstruction from the preserved files. The DSTs keep calorimeter cells only after
  noise suppression and no FMD raw banks (§1.0). An analysis-level emulation of the forward-active FMD/FTS efficiency
  from stored per-layer arrays is possible but untested (§2.1).
- [I] Re-running the simulation with today's code is expected to reproduce the mismatch: there is no LAr-forward, Plug
  or FMD change between SW 72 and SW 84 (§1.5).
- A simulation fix therefore means changing the forward detector model, then re-simulating: a detector-tuning project.
- A new simulation can technically run on NAF today, and the diffractive samples are cheap. The nondiffractive (ND)
  samples 7071/7091 that set G1(c) cannot be re-simulated: their generator files and DSTs are not on disk (§2).

## 1. Attribution

### 1.0 Discriminating evidence gathered for this package

- **Kinematic sweep** [V]. Script `D/scripts/restart_2026-10-27/wp-a4/kin_scaling_wpa4.py` (md5 `45239c6a…`); skim
  fork `skim_wpa4.py` (md5 `0a8bfc1e…`; keys shared with the WP-A3 skim are identical). Sample: inclusive A_pre,
  35 < E−p_z < 70, no veto and no gap cut (the validation-chain sample). Six cells: Q² 150-300 / 300-700 / > 700 GeV²,
  each split at the median x; data ⟨W⟩ = 204/134/207/131/210/145 GeV (K0…K5, `kin_scaling.json` `data_cells`), so W
  only distinguishes low x from high x within a Q² bin. Flatness χ² (ndf 5) and trend z were pre-registered (docstring
  lines 25-28: p < 0.01 or |z| ≥ 3 ⇒ kinematic). At this statistics level almost every series fails that rule; where a
  reading below rests on the size of the spread instead, it says so [I].
- **Composition fit** [V]. `composition_wpa4.py` (md5 `b9e5ab7d…`), same sample and weights. It fits each series either
  with one additive data−MC offset or with one scale per MC class (elastic EL, proton-dissociative PD, Reggeon RG, ND),
  and gives the MC response P(reco edge | truth η_max). Condor total for WP-A4: **0.0070 Condor-days** (clusters
  5917268, 5917311, 5917453; `D/data/restart_2026-10-27/wp-a4/condor_tally.csv`).
- **Preserved levels** [V, `h1-mc-db`]. HAT+MODS are on dCache for every ReqID used. DSTs are on dCache only for 9012,
  9013, 9155, 9157, 9158 and MC13 8947-8962 (e.g. 240 files in `/pnfs/desy.de/dphep/online/h1/mc/rapgap31/07/9012/`).
  7071, 7091, 7094, 7096, 7097, 6671 and 6677 have an empty dstPATH (`IMC/h1db.js:59,65,193,210,213-215`); `/acs` is
  not mounted, and no generator file of these genIDs is on disk. A bank census of one 9012 DST found GTR/GKI, DCEL
  ("DST calorimeter cell bank (all calos)", `/afs/desy.de/group/h1/h1banks/h1banks.car:10466`), DCLU, DFTS (`:10746`)
  and rt00-rt02 (`IMC/steering_files/mcrequest_9012:39-55`). It found no raw LAr or Plug digis, no FMD banks and no
  CJC raw hits.
- **Overlay** [V, `h1-mc-expert`; corrects `wp-a/s1c_mc_expert_lookup.md:27-31`]. ADDRAND reads real random-trigger
  (RT) events through the run index `/afs/desy.de/group/h1/h1sim/rtdb` → `/acs/random_sim/{03..07}/H1RAND_OA.*`. A
  50-event official-steering smoke opened 71 distinct RT files
  (`HS/production/rapgap_lowQ2_smoke/logs/logs_part_1.tgz`, preprocessing.stdout:1362-1363). The forward overlay
  therefore follows the run; only the LAr NOISINP pools are snapshots of 2-4 runs per period.
- **Simulation mode** [V]. SHOWER_LIB=Y covers only SpaCal and FNC (`IMC/steering/blocks/fpack_SL_new:1-6`,
  `OTTO_SL:11`). The forward LAr and the Plug use OSGD modes CALO 3 / PLUG 1
  (`HS/steering/official/h1simrec_06_07e+_7091_mainsteer.txt:113-118`); their meaning is [I].
- **Literature** [V; arxiv.org full texts]. arXiv:1203.4495 (EPJC 72 (2012) 2074) shows no η_max, Plug, FMD or FTS
  control plot. It tunes forward noise with overlaid random triggers and fixes the FMD/FTS/Plug variations "by
  comparison … for samples in which forward detector activity is required" (§3.5). Its HERA-II LRG sample stops at
  Q² = 105 GeV² (`wp-a2/s1_definition_audit.md:20`). **No published HERA-II high-Q² forward control exists**, so
  discriminant (ii) of the brief cannot be applied.

### 1.1 LAr forward/IF region, cluster η_max 2.2-3.6

Edge pile-up at η 3.40-3.55: **S dominant (≈ S 70 / R 20 / P 10, judgement), confidence MEDIUM.**
- [V] The primary evidence is that the localised excess/deficit pair at the acceptance edge depends on the threshold
  (it moves by 0.91 and 4.23 between 0.8 and 0.4 GeV), is shared by RAPGAP ME+PS and CDM, appears in a Plug-tagged
  sample, and is not caused by geometry or vertex (Δη shift 0.0004) (`wp-a2/s2_diagnostic_record.md:55-64,74-79,85-90,141-146`).
- [V] Across kinematics the deficit depends on the normalisation. With each cell normalised to unit area (NALL, the
  shape test), DATA/MC is 0.65-0.72 (7071), 0.56-0.59 (7091) and 0.65-0.72 (nd4cat), a spread of ≤ 0.07 against an
  offset of 0.3-0.4 (`K:14,32,50`). For 7091 the pre-registered rule does not call it kinematic (p 0.034, |z| ≤ 0.7).
  For 7071 and nd4cat it does (z(⟨hatEtaMaxGen⟩) +4.2/+4.3), through a drift that mirrors their generator-dependent
  2.5-3.0 slope. Normalised to 1.0 ≤ η < 2.0 (N1), the ratio is 0.56-1.09 (7071), 0.49-0.66 (7091) and 0.57-1.09
  (nd4cat), with p 0.000 (`K:13,31,49`). Near-flatness is therefore weak support [I].
- [V] MC response (`C:27-38`): P(reco in [3.40, 3.55) | truth η_max ≥ 4) is 0.26 (9012), 0.43 (7071) and 0.50 (7091).
  For truth 3.0-3.3 it is 0.03, 0.32 and 0.39; reco ≥ 3.55 is ≤ 0.003. [I] Until `h1-mc-expert` confirms the truth branch these are not detector-response numbers (a
  class-dependent upward shift of 0.1-0.5 in η cannot come from one detector effect). The reco edge bin collects activity
  from beyond the LAr acceptance. Its filling depends on the event class and the generator, so the MC response alone
  does not separate S from P. One S reading is over-efficient edge clustering (leakage or back-scatter near the beam
  pipe). `etaMaxGen` and `hatEtaMaxGen` agree to 16 digits (`K:59-60`), so the truth definition is unconfirmed (§5).
- R share [I]: LAr noise is added at REC (h1rec/ascale, OSRN word 10) from 2-4 NOISINP runs per period
  (`D/docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md` §2.1). Edge clusters near threshold depend on it.

Rise from η ≈ 2.25 and the 2.5-3.3 excess: **mixed; P present (≈ P 60 / S 30 / R 10, judgement), confidence MEDIUM.**
- [V] The level depends on the generator: DATA/MC in 3.0-3.3 is 1.17-1.31 (7071) and 1.44-1.53 (7091) (`K:10,28`). In
  2.5-3.0, 7071 falls from 1.36 to 1.00 with Q² while 7091 stays at 1.48-1.61 (p 0.36) (`K:8,26`). At η 3.0 DATA/MC is
  1.7 (7071) vs 2.8 (7091) (`s2_diagnostic_record.md:144-146`). The drift starts near η ≈ 2.0-2.25
  (`response_reweighting_scoping.md:105-110`).
- [I] Generator dependence shows that P is present. It does not prove that P leads, because one detector mismodel
  applied to different truth spectra also gives different ratios. S share: dead material in front of the forward LAr
  and the fast-simulation hadronic response.

### 1.2 Plug calorimeter

**Low-energy occupancy: S/R, confidence MEDIUM. Deep-gap > 7 GeV fail rate: undetermined (P, S × composition, or for 7091 an additive term),
confidence LOW.**
- [V] DB treatment excluded (EnCor = 1, Thresh = 0.3 GeV in all runs, `s2_diagnostic_record.md:36-37`). Zero-energy share (ePlug < 0.1 GeV): MC has too many such events everywhere. DATA/MC is 0.39-0.52 (7071, nd4cat)
  and 0.57-0.64 (7091) in the forward-active sample η_max ≥ 3.3, with p 0.000 (7071, nd4cat) / 0.019 (7091) and a ⟨hatEtaMaxGen⟩ trend (z −4.9/−5.2) against the rule,
  which the S/R reading overrides
  (`K:15,33,51`). In the deep gap η_max < 2.6 it is 0.61-0.80 and 0.54-0.73 (`K:19,37,55`). [I] Plug noise enters at
  REC (h1rec/pscale, OSRN word 9, `wp-a/s1c_mc_expert_lookup.md:16-23`), and the data Plug fires alone ~4× more often
  than MC (`response_reweighting_scoping.md:80-81`). Too little low-energy occupancy in MC therefore points to the
  pscale noise model or the PDIGI cell response (S/R). Beam-induced activity correlated with the DIS trigger is an
  alternative that the stored variables cannot test.
- [V] Fail rate ePlug > 7 GeV. In the forward-active sample DATA/MC is 0.95-0.98 (7091) and 1.06-1.17 (7071)
  (`K:16,34`). In the deep gap it is 2.25-2.55 in the lowest-Q², low-x cell K0 and 1.13-1.55 in K4/K5. At fixed
  ⟨W⟩ ≈ 205 GeV it follows Q² (K0/K2/K4 = 2.25/1.56/1.13 for 7071, `K:20,38,56`).
  Composition fit (`C:7,15,23`): one additive offset fails for 7071/nd4cat (p ≤ 2e-7) but fits 7091
  (p 0.17); an unconstrained per-class rate scale fits (p 0.02-0.91) only with unphysical scales (EL −0.8 to −4.9, PD
  ×8.9-9.9). [I] No physical per-class scaling has been shown to describe the trend (a constrained fit was not run).
  This overrides the script's pre-registered rule (docstring line 19, p > 0.01 ⇒ "not evidence for flow") because
  the scales are unphysical. Undetermined set: P (M_Y/flow), S × composition, or (7091) a class-independent additive
  noise/beam term.
- Discriminant (i) of the brief (Plug layer vs calibration) **was not run**. HAT holds only `HfsClusPlugE`
  (`s1_definition_audit.md:31`). Plug cells are in DCEL (H1PlugCell "constructor from DCEL row",
  `H1Clusters/H1PlugCell.C:21`; 1808 cells, `H1Geom/H1CaloGeometry.C:55`). A layer test is possible from the DST for
  9012/915x only [I: needs a DCEL reader].

### 1.3 FMD

**Forward-active sample: S, confidence MEDIUM. Deep gap: undetermined (P or S × composition), confidence LOW.**
- [V] Forward-active sample: the DATA/MC fail fraction is 0.81-0.86 for all chains, spread ≤ 0.03 (p 0.002/0.000/0.002
  against the flat rule, |z| ≤ 1.2) (`K:17,35,53`). MC fires the veto layers ~15% too often wherever forward particles
  exist. [I, `h1-mc-expert`] With the unsimulated layer-4 inefficiency (`s2_diagnostic_record.md:114-120`), this reads
  as an efficiency mismodel at digitisation (h1sim/mdigi): S.
- [V] Deep gap: 1.41-1.48 in K0, falling to 0.83-1.05 in K3-K5 (0.83-0.87 for 7071/nd4cat, close to the
  forward-active value) (`K:21,39,57`). An
  additive offset fails (p ≤ 5e-15); per-class scales fit (p 0.21-0.53) with an unphysical negative RG scale (`C:9,17,25`).
- Stage [V]: RT noise and digitisation at simulation (OSRN word 8, h1sim/mdigi); no FMD banks in the DST.

### 1.4 FTS/PRT

**Forward-active sample: S, confidence MEDIUM. Deep gap: undetermined, confidence LOW.**
- [V] Forward-active sample: DATA/MC is 0.93-0.99 (7071), 0.87-0.92 (nd4cat) and 0.95-0.97 (7091, 1.09 in K5). It
  passes the flat rule for 7071 and nd4cat (p 0.13-0.14) but not for 7091 (p 0.000) (`K:18,36,54`).
- [V] Deep gap: 1.69-1.74 in K0, falling to 0.93-0.98 at high Q² (`K:22,40,58`). An additive offset fails; per-class
  scales fit (p 0.66-0.82) only with RG ×8.9-10.6 (`C:10,18,26`).
- [I, `h1-mc-expert`] Station structure (`s2_diagnostic_record.md:114-120`): the dead ep0607 channels at indices 4-7
  (DATA 0.005-0.03 vs MC 0.06-0.08) are a dead-channel map missing in MC (S/DB). The ~2× excess at indices 0-3 is
  proton dissociation plus beam-line secondaries (P + S).
- Stage [V]: RT noise is applied at REC h1rec/dstmak (OSRN word 7), and the DST keeps DFTS and rt00-rt02. A DST
  re-reconstruction is plausible only here [I: DFTS raw content unknown].

### 1.5 Software version

[V, `h1-mc-expert`] SW 72 (VT-72, 2010; 7071/7091) and SW 84 (VT-84, 2012; 9012/915x) differ forward only by VT-73
(forward-beamline option, FTS/FNC) and VT-74 (FNC). VT-82 ("bug fixes in H1SIM 39600") is unspecified
(`IMC/doc/h1simrec_VT.html` rows 72-84). Forward RT noise has been in place since VT-45..60 (`:237-282`). [I] On the
change log alone, today's code would not change the forward response. The release 4.2.1 `h1simrec9` is an untagged git build (`RELEASE_NOTES_rollingdocument:57-60`), most likely beyond VT-86.

## 2. What a fix would take

### 2.1 R-only (re-reconstruction, HAT refill, analysis-level emulation)

- **Re-reconstruction or refill is possible only where a DST exists**: 9012/9013/915x and MC13 8947-8962. It is
  **impossible for 7071/7091/7094-7097**: no DST on disk, and the `/acs` tape copies are unchecked (`h1-mc-db`).
- **Reach** [V/I]: DCEL holds cells after DST-level noise suppression. A refill can re-cluster, change η_max or Plug
  cell thresholds, and test the Plug layer pattern. It cannot change the noise model below the suppression, the
  digitisation, or the FMD. Threshold variants (0.4 vs 0.8 GeV) were already tested and did not close the gate
  (`g1prime_verdict.md:50`).
- **Analysis-level emulation** [I, untested]: drop MC FMD hits in veto layers 1-2 (`nFmu12`, `n1_wpa2.py:73`) with
  per-layer data/MC rates, and mask the ep0607 FTS dead indices in MC, using the per-event arrays `nFmuLayer` (6) and
  `nFtsLayer` (16) stored in the trees (`skim_wpa4.py`), with rates from a split disjoint from the gate test. This works for every chain, 7071/7091 included. It addresses
  only the forward-active FMD/FTS S part, not the LAr edge or the Plug.
- **Tools** [V]: `dst2all` with the MODS fix (`HS/modshat_wrapper.sh`, `HS/lib/libModsFillMCFix.so`); the v6dif
  producer reads the DST via `--diff-ods` (`D/condor/v6dif_mc9012_stage2/2026-10-18/v6dif_mc9012_stage2_job.sh`).
  dst2ods costs 0.09 s/event (`HS/production/pythia100k/hiQ2/metrics`). The HAT fillers (`H1OO/H1HatFiller/`) are in
  the public release; there is no active maintainer [I].

### 2.2 S (re-simulation with a changed detector model)

- **Runnable today** [V, `HS/HANDOFF.md` §3, §6-8]. The patched `h1simrec9` (release 4.2.1 EL9, ksfitt fix) runs on
  NAF with the official 06_07e+ steering: SIMRUN, ADDRAND RT overlay, shower library. It has produced about 53k Pythia8
  and 45k Sherpa events at high Q² (plus low-Q² samples, `HS/HANDOFF.md:311`), and 50-event DST-fed smokes
  (`GEN=h1bos`, `bosstrip`) for 8686/8709. The official FTT-emulation step `h1fttemu` is not run (`h1mcproduction/README`).
- **Other periods** [V]. Per-period steering exists for every ReqID
  (`IMC/steering_files/mcrequest_{9012,9013,9155,9157,9158,7071,7091,7096,7097}`), and the LARNOISE pools are on
  `/pnfs/desy.de/dphep/online/h1/noise/`. Only 06_07e+ is templated in `HS/steering/official/`; each other period
  needs ≈ 0.5-1 day plus a smoke [I].
- **Inputs to change** [I, `h1-mc-expert`]: LAr forward/IF material and fast-simulation mode (OSGD CALO 3) and more
  NOISINP runs per period; Plug pscale noise and PDIGI response; FMD layer efficiencies and the ep0607 FTS dead-channel
  map. Each needs H1SIM/H1REC code or DB work, not steering, and has no maintainer.
- **Generator input** [V]. The 9012/915x DSTs carry GTR, so `GEN=h1bos` reuses the *same* events. Only 4.82% of 9012
  passes stage 1 (`D/condor/v6dif_mc9012_evls/2026-10-18/README.md:80-81`), so this adds no generator statistics. New
  diffractive events need RAPGAP 3.305 (`/afs/desy.de/group/h1/h1gen/rapgap33/standalone/bin/rapgap_hepmc` runs on
  NAF, `wp-i/crad_strategy.md:61`), a stand-in for the 3.1034 standard. For ND only MC13 8947-8962 (RAPGAP ME+PS and
  DJANGOH CDM, SW 84, all periods, 0.68M-20M events per ReqID, DSTs on dCache, `h1db.js:1797-1812`) can be re-simulated.
- **Cost** [V measured: h1simrec9 3.24 s and whole chain 3.9 s per output event, Pythia8 high-Q² official steering,
  212 jobs (`HS/production/pythia100k/hiQ2/metrics`); 783 kB DST + 254 kB ODS + ≈ 106 kB HAT/MODS per event. Totals [I]]:

| sample | events | CPU-core-days at 3.5 s/event | DST+ODS kept | HAT/MODS + trees only |
|---|---|---|---|---|
| signal ≥ 5× data L at Q²>150, generator cut Q²>100 (fraction ≈ 2.2% [I]; ≈ 30-34 × 9012 from `D/docs/mc/composite_scale_table_v3_2026-10-20.csv` row 9012; without a cut ≈ 37M events, ≈ 38 TB) | ≈ 0.8M | ≈ 32 | ≈ 0.8 TB | ≈ 0.1 TB |
| ND, one ≈ 19-20M MC13 ReqID per period, Q²>60 | ≈ 4 × 19M | ≈ 3,100 | ≈ 79 TB | ≈ 8 TB |
| ND, Q²>150 subset of MC13 (filter at `bosstrip`) | unknown | unknown | — | — |
| PDISS 9155/9157/9158, same events | 20M | ≈ 810 | ≈ 20 TB | ≈ 2 TB |

  DUST has 16.1 TB free of 40 TB (`HS/HANDOFF.md:50-54`), so DST+ODS cannot be kept for the large rows. The Condor slot
  count is undocumented, so wall-clock time needs a pilot.
- **Validation chain** [I]: (1) a same-event comparison of private `h1simrec9` against official SW 84 HAT for 9012
  (none exists, `HS/HANDOFF.md` §7); (2) the §1.0 forward controls (edge, Plug f0/f7, FMD/FTS, forward-active and
  deep-gap, per kinematic cell and MC class) in the inclusive high-Q² sample before any gap selection; (3) G1-type gates
  on a sample split by event parity.

### 2.3 P (truth-space reweighting to a measurement)

- **No measurement to tune to** [V, literature]. There is no H1 hadron-level forward-flow measurement at Q² > 150 GeV²
  in lab η 2.5-3.5. hep-ex/9907027 (EPJC 12 (2000) 595) is HERA-I and in the hadronic CMS, and its Plug-region bins carry a 20%
  "modelling of the physics" uncertainty. arXiv:1302.1321 covers 5 < Q² < 100 GeV² only.
- **Parameters** [I, `h1-mc-expert`; not checked against the generator logs]: RAPGAP 3.1 ME+PS PARL(3)/PARP(91),
  PT2CUT, IFPS; ARIADNE PARA(10)/PARA(11), MSTA(30); the shared JETSET string PARJ(21), PARJ(41-42). A retune needs
  generator re-runs plus re-simulation; for 7071/7091 that means new ND events.
- **Data-driven truth reweighting** to the inclusive control is the WP-A3 map in another form. It inherits the ND
  generator (14-49% chain dependence, `response_reweighting_scoping.md:130-133`), and it is circular when the target is
  η_max (`:191`).
- **Gap-sample composition**: the M_Y spectrum and the PDISS/ND fractions are measured inputs in the H1 papers
  (arXiv:1203.4495 §3.5: M_Y correction 13% for 2004-07). They can be varied as systematics. The fitted PD scale of
  ≈ 9-10 (`C:7,23`) is far outside such a variation.

## 3. Who and how

- **Official route** [V]. Requests go to the MC production coordinator via the IMC web tool "Prepare a request", which
  writes `mcrequest_<ReqID>` (`IMC/doc/steering_doc.html`, "since March-2008"). The listed coordinator is Sergey
  Levonian (levonian@mail.desy.de); the page was updated on 29 Sep 2021 (`IMC/index.html:214,217`). The last ReqID in
  the DB is 9373, with generator 6356 registered on 12-MAR-18 (`h1db.js:2199,3586`). `mcrequest_9374-9376` (2021-06-23,
  SW 86) are not in the DB. [I] Production since then is occasional. No procedure for *changing* the detector model in
  preserved software is documented.
- **Tape and calendar**: `/acs` DSTs and GEN files (7071/7091, all GEN) need staging by DESY-IT/H1 data preservation;
  survival unknown [V]. New official samples with unchanged simulation take weeks to months; a detector-model fix has no
  owner and needs an H1 forward-detector or simulation expert, so count months, with unknown availability [I].
- **Private production for publication** [I]: a sample made with the official steering, noise pools and a documented
  patched binary is technically equivalent to official MC. Whether H1 review accepts it, and especially a privately
  *modified* detector model, is a collaboration decision; no document found states the rule.

## 4. Decision table

P(pass) = probability that a G1-type gate (same clauses and thresholds) passes after the option; all judgement [I].

| option | fixes | CPU-core-days | calendar | people | P(pass) and reasoning |
|---|---|---|---|---|---|
| R-fix only: HAT refill from DCEL, FTS from DFTS, analysis-level FMD/FTS efficiency emulation | Plug-layer test; forward-active FMD/FTS S part (all chains, untested) | < 5 | 2-4 weeks | user | **< 5%**. The LAr edge (G1(c)) and the Plug occupancy (G1(a)) are untouched |
| S re-simulation, current model (statistics, all periods, MC13 ND) | MC statistics, SW matching, period coverage | ≈ 32 signal + up to ≈ 3,100 ND + ≈ 810 PDISS | 1-3 months | user | **≈ 5%**. Same code reproduces the edge (§1.5); the forward-active offsets are flat and generator-shared |
| S re-simulation, modified detector model | LAr edge, FMD/FTS efficiency, Plug noise | as above plus tuning iterations (unknown) | 6-12+ months | user + H1 simulation expert (availability unknown) | **≈ 20-30%** if an expert is found. The 2.5-3.3 P part remains; the deep-gap S/P split is undetermined (could raise or lower this); publication acceptability unknown |
| P reweighting to a measurement | — | — | — | — | **not available**: no Q²>150, η_lab 2.5-3.5 measurement; the data-driven version is circular for η_max |
| combination: modified S + generator retune + M_Y/PDISS revision | most of the mismatch | > 1,000 plus iterations | ≥ 12 months | user + simulation expert + H1 review | **≈ 30-40%**, gated on the expert and on collaboration acceptance |
| stop (close-out) | — | 0 | days | user | n/a. Recommended by both kill gates (`g1prime_verdict.md:124`) |

Unknowns: `/acs` tape survival; Condor slot count; Q²>100/150 fractions of 9012 and MC13; expert availability; H1's
rule on private or modified MC; DFTS raw content.

## 5. Risks, open questions, coverage gaps

- The deep-gap per-class fits are unconstrained rate scales (ndf 2, nearly collinear); no physical mix is shown to fit. The truth definition of `etaMaxGen`/`hatEtaMaxGen` (one branch? leading proton
  included?) is unconfirmed (`K:59-60`). The S/R/P weights are judgement. The private chain lacks FTT emulation, and
  its equivalence to SW 84 is unverified.

## 6. Sign-off (`h1-analysis-critic`)

Re-review (h1-analysis-critic, 2026-10-27): SIGNED, conditional on rewording the §1.5 edge-response sentence (done).
The three earlier conditions are met. Attribution is judgement. Forward-active FMD/FTS (S) and Plug occupancy (S/R)
stand. The deep-gap entries stay undetermined: no physical per-class scale fits, and a constant offset fits only Plug
7091. Confirm `hatEtaMaxGen` before reading the response as a detector effect.

# Plan: real H1 hit-level data for FM4NPP pretraining

Date: 2026-10-27, annotation cycles 1 (2026-10-27) and 2 (2026-10-28). Stage: planning (program with
work packages).
Research artifacts:
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/research/2026-10-27-fm4npp-h1-real-data-blockers.md`
(cited as "research §n") and
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/research/2026-10-27-fm4npp-component-validation.md`
(cited as "validation A1-A5, B").

Abbreviations: `$W` = `/data/dust/group/h1/klesthen/BaryonAsym`, `$H` = `$W/hit_level_sim`,
`$R` = `/data/dust/group/h1/klesthen/rawhit_probe`, `$V` = `$R/validation`,
`$P` = `/pnfs/desy.de/dphep/online/h1/data`, `$D` = `$H/realdata` (new; see D6),
`$MC0` = `$H/production/cov_loQ2/loQ2/hits/hits_part_0.root` (reference MC hits file),
h1rec = `/data/dust/group/h1/sschmitt/h1soft/h1rec`,
H1OO env = `source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh`
(as in `$H/env.sh`).

## Goal

Produce real H1 HERA-II hit-level inputs in the `hits` tree format of `$H/extract_hits.C`, so the
FM4NPP converter `h1_to_fm4npp.py` (LCRC only) reads real data as it reads simulation. Real data
takes the NDB-driven path (`$R/calib_consts/py/h1hits.py` with per-run `consts_<run>.json`, h1rec
coordinates from `calibprobe`), never the MC geometry or calibration hard-coded in
`$H/extract_hits.C:16-57`, and only events passing the H1OO standard run and detector-status
selection (D17).

Non-goals: changing the simulation chain or `extract_hits.C`; changing the converter on LCRC;
forward tracker, LAr, SpaCal; 2003-2004 and HERA-I data; supervised truth labels for data (none
exist, research B10).

Expected change size: about 1,650 lines across the program: WP0 parity script about 150 lines of
Python; WP1 writer, run-list dump and checks about 700; WP2 job wrapper, submit file and layout about 150; WP3
about 100 (one flag in `calibprobe.C`, one submit, one diff script); WP4 and WP5 about 200 each;
WP6 documentation about 150. This plan is about 640 lines.

## Approach

WP0 first shows that the D5 representation matches MC by comparing the same residual on both
samples.

One data-side writer (`$D/writer/write_hits_data.py`) then reads the text dumps the existing
probes already produce (`events.txt` from `calibprobe`, `cjcprobe.txt` from `cjcprobe`, later
`dataprobe.txt` and `cst_full.txt`) plus `consts_<run>.json` and the HAT T0 (D11), computes the
D5 coordinates with `h1hits.py`, and writes a `hits` tree with the branch list of `extract_hits.C`
(truth branches present and empty, data-only branches additive). WP1 runs it on the 38 `$R/lam/`
dumps; WP2 wraps the unchanged probes plus the writer into one Condor job per 2007 e+p mODS
file; WP3 dumps constants for the other periods with a constants-only mode; WP4 and WP5 add CST
and CIP/COP after their own research; WP6 records provenance, errata and updates the handoff.

Rationale: every validated piece (`h1hits.py`, `calibprobe`, `cjcprobe`, `dataprobe`, `cstprobe`)
exists and runs on Condor at about 0.07 s/event (research §5, `$R/lam/logs/condor.log`); the plan
adds glue, format parity and validation, not new reconstruction.

Rejected alternatives (one line each):
- Reimplement `h1_to_fm4npp.py` on NAF: its schema is inferred only (research B9); parity with the
  `hits` tree is the cheaper contract.
- Write ROOT via uproot jagged arrays: uproot writes `x[n]/F` leaves, not `std::vector<float>`;
  PyROOT gives identical leaf types (D8).
- Extend `extract_hits.C` with a data branch: it reads ODS bank classes from simrec, while data
  hits come from raw FPACK via h1rec in-process; separate code keeps the MC path untouched.
- A new merged C++ probe: three unchanged probes in one wrapper reach the same output with no new
  Fortran linkage risk (D9).
- Full 2007 pass: 145k Q2e>5 events per file × 112 files is about 16M events, about 400 GB of
  intermediates; the first release caps per file (D3).
- Track-informed default coordinates (β terms from `CJXR`): unlike MC and circular with h1rec (D5).

## Decisions

Defaults D1-D4 were adopted by the supervisor after the user answered "continue" without choosing;
D5 is the supervisor's recommendation from annotation cycle 1 and D17 from annotation cycle 2; the
user may overturn each at plan approval. D6-D16 are lead decisions with evidence.

- **D1 Converter (default; confirm).** Match the `hits` tree schema of the current
  `$H/extract_hits.C` (reference file `$MC0`, 110 branches: 4 `Int_t`, 56 `vector<float>`, 50
  `vector<int>`, validation A5) exactly; data-only branches are additive and allowlisted (D7). The
  converter's exact needs stay a risk until the user copies it to NAF (Open Question Q1).
- **D2 Labels (default; confirm).** Pretraining is self-supervised; H1 reconstruction pseudo-labels
  (`cjc_reco_trk`, `cjc_reco_side`, `cjkr_*`, `cjkt_*`, `cjkv_*`) are carried as in MC but never
  presented as truth (`is_data = 1`, D7).
- **D3 First-release scope (default; confirm).** 2007 e+p, HAT selection `Q2e>5`. WP1 converts
  `$R/lam/` (57,038 events, 47 runs). WP2 extracts the first 5,000 `Q2e>5` events of each of the 112
  2007 mODS files, about 560k events before the D17 selection. Rationale: about 10× the existing sample and 5× the 100k MC
  pools (`$H/production/sherpa100k*`, `pythia100k`); about 6 min per file at 0.07 s/event; about
  85 GB of text dumps (about 150 KB per event: `events.txt` 160 MB per 1,501 events in
  `$R/lam/c070060/calib/`, `cjcprobe.txt` 26 MB, `dataprobe.txt` about 30 KB per event) and about
  15 GB of `hits` trees; the full 16M events stay one `-n` change away. Measured: `c070060` HAT has
  797,529 entries, 144,909 with `Q2e>5`.
- **D4 Detectors (default; confirm).** First release: CJC digis and coordinates, COZ digis and
  on-track coordinates. CST (WP4) and CIP/COP (WP5) follow; their branches stay empty until then.
- **D5 Coordinates (user decision; confirm at approval).** The default CJC input is the MC
  representation: `cjc_xp/yp`, `cjc_xm/ym`, `cjc_drift`, `cjc_zcd` from the same geometric formula
  as `extract_hits.C` (drift distance from the drift time, `:43,304`; fixed per-wire drift
  direction, `aloro = 0.635`, `:57,74-75`; both mirrors `wire ± |d|·dir`, `:313-314`; no
  track-angle term), computed on data by `CJC.simple()` (`$R/calib_consts/py/h1hits.py:375`,
  `cosb=1, sinb=0`, so the β, isochrone and `dalor·tnb` terms vanish) with the per-run NDB
  constants of `consts_<run>.json` and the event T0 of D11. The mm tail at small |cos β| and long
  arc length (validation A1, `$V/plots/cjc/fig4_tail_vs_beta_slen.png`) belongs to this
  representation and is expected in MC too; WP0 checks that the two agree (D16). h1rec
  track-informed `CJXR` coordinates (`XHIT YHIT ZHIT`) go to additive branches `cjc_trk_x/y/z` for
  hits with `cjc_reco_trk >= 0`, zero otherwise, as optional labels, never the default input.
  Rejected: β terms from `CJXR` in the default (track-informed, unlike MC).
- **D6 Code location.** New code lives in `$D = $H/realdata/` with `writer/`, `condor/`, `consts/`,
  `validation/`, `docs/`; outputs in `$H/production/realdata_2007ep/<set>/hits/hits_part_<file>.root`
  (same shape as the MC sets, for example `$H/production/cov_loQ2/loQ2/hits/`). Reasons: the schema
  contract (`extract_hits.C`), the reference MC files and the FM4NPP handoff (`$H/HANDOFF.md`) live
  in `$H`; `$H` is inside the workspace, so `$W/AGENTS.md` rules apply (per-topic subdirectory, no
  loose files). The h1rec-linked executables stay in `$R` (`calib_consts/bin/calibprobe`,
  `bin/x86_64-el9-gcc14-opt/cjcprobe`, `data_display/`, `cst_data/`, `lib/*.so`) because they carry
  the patched Fortran builds and the `mirror/` search path; the writer imports `h1hits.py` from
  `$R/calib_consts/py/` by absolute path and records its sha256 in every output file (D7).
  Rejected: code in `$R` (outside the workspace, not where the schema lives); a copy of
  `h1hits.py` in `$D` (two drifting copies while `$R` is still edited).
- **D7 Schema parity and additions.** Every branch of the reference MC file is present with the
  same leaf type; truth branches are empty vectors, `n_str_all = n_svx_all = 0`. Additive data-only
  branches, all in the allowlist of `$D/validation/check_schema.py`: scalars `is_data` (Int_t, 1),
  `cjc_t0` (Float_t, ticks), `cjc_t0_src` (Int_t, D11: 0 = HAT `T0_CJC`, 1 = run default,
  2 = `cjavt0`), `run_selected` (Int_t, D17), `run_quality` (Int_t, `H1RunQuality::ERunQual` 0-3),
  `det_on` (Int_t, D17 status verdict); vectors `cjc_trk_x/y/z` (D5), `coz_trk_x/y/z` (D12),
  `hv_off`, `caen_off`, `bbl3_off` (`vector<int>`, indices of status bits that are off, D17), later
  `cst_sp_*` (D13). Each file also holds a `meta` TNamed with JSON provenance (run list, event
  counts, D17 selection counters `seen`, `rejected` (the union, with its shares by run list and by
  status bits) and `empty` after selection, so seen == written + rejected + empty; T0 source counts; sha256 of `h1hits.py`, of each `consts_<run>.json` and
  of the run-list and run-quality tables; probe paths, NDB path, date). Raw CJC words follow the MC convention `cjc_drift_raw = DL*0.001`
  (`extract_hits.C:159`); in data `DL` is time in FADC ticks (1/50 clock,
  `$R/calib_consts/py/h1hits.py:7`), documented in WP6.
- **D8 Writer technology.** PyROOT in the LCG environment after `thish1.sh` (Python 3.11.9, ROOT
  6.34.04, uproot 5.3.11 and numpy present; verified on 2026-10-27) with `std::vector<float|int>`
  branches, so `TTree::Print()` leaf types match MC. Environment order matters: run
  `desygroupset h1` before `thish1.sh` (as `$R/lam/job.sh` does) or call the LCG python by full
  path; otherwise `python3` is `/usr/bin/python3`, which cannot import ROOT (validation,
  corrections 7). Never `python3 -I` for the writer: it drops the LCG `PYTHONPATH`
  (`$H/HANDOFF.md:234-235`); the probe's stdlib-only scripts keep their `-I`.
- **D9 WP2 reuses the probes unchanged.** The job runs `calibprobe`, `make_consts.py`, `cjcprobe`
  and `dataprobe` exactly as `$R/lam/job.sh` and `$R/scale/job.sh` do, then the WP1 writer, so WP2
  changes no C++ and the text formats stay those WP1 was validated on. `dataprobe.txt` (raw `SIFE`,
  `CRM2`, `CRME`, `CRZE`) is kept per file so WP4 and WP5 are post-processing plus one extra probe
  step, not a re-extraction. Text dumps are kept (about 85 GB, D3); if DUST space runs short,
  `events.txt` and `cjcprobe.txt` may be deleted after a file's validation passes, since
  re-extraction costs about 6 min per file.
- **D10 CJC-off events.** The D17 status cut is the primary handling. As a safety net the writer
  still skips any surviving event with zero CRJE hits and counts it in `meta` (expected about 0; a
  non-zero count means a CJC-off state the status bits miss and is reported, not hidden). Run
  492559 is CJC-off per event, not per run (1,316 of 1,501 events empty, 185 with hits), and the
  `SelectedRuns.root` list rejects it; it serves as the validation case of the Slice 1
  `hv_vs_cjc_off` test.
- **D11 Event T0.** Primary: the h1rec event T0 `DMIS TZERO_M`, read from the HAT branch `T0_CJC`
  (`/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/H1Filler/H1HatFiller.C:855`
  fills it from `GetTzero_m()`) joined by `(run, event)`; `cjc_t0_src = 0` for every joined HAT
  value, whatever it holds (the HAT carries no measured/default marker). Fallbacks, only on a
  failed join: `cjavt0(T0track, |DT0|)` over the event's CJKR rows (`events.txt` CJKR cols 27-28),
  `cjc_t0_src = 2`; else the run default `ARC0J8(1)` from the consts json, `cjc_t0_src = 1`.
  Never the `# CJCC … t0` line of `events.txt`: it is calibprobe's own mean `T0track` (−537 to
  2900), not CJCREC's (validation A1). The real T0 buys provenance, not resolution (track-free
  robust σ 604.8 µm with DMIS versus 604.2 with `cjavt0`, validation B).
- **D12 COZ.** `coz_wire/drift/qplus/qminus` from raw CRZE words for all hits. The probe dumps hold
  no h1rec COZ coordinates (`CZZR` is all zeros, validation A2), so `coz_trk_x/y/z` come from
  `h1hits.COZ.hit()` with the `CZR2`-matched track after a `t <= 810 ns` cut (beyond the isochrone
  table; removes the 7 garbage hits). Validated at the isochrone step only (D16). Track-free COZ φ
  (`COZ.phi_chargediv`) stays unvalidated and off (research B4; A2 p95 2.4 rad).
- **D13 CST (WP4).** Data CST enters as h1rec space points (`cstprobe` chain CSLIGEOM, CSCLUS2,
  CSFILL2 on raw SIFE) in new `cst_sp_*` branches; `csts_strip/csts_adc` stay empty because the
  MC decoder (`$H/decoders/h1_digi_decode.py:114+`, MC geometry, `CST_PN_RATIO` fitted on MC) is
  not valid on data (research §4). Acceptance per the D16 CST block. The converter then needs to
  read `cst_sp_*` (user action, Q5).
- **D14 CIP/COP (WP5).** Raw banks on data are `CRM2` (CIP2k) and `CRME` (COP), the same as MC;
  the research artifact's `CRPE/CRTE` are not banks (`ICRPE` is a variable, h1rec
  `include/cjcmac/ctcommon.inc:10`). If the data-stream `CGAM` equals the MC `CGAM`
  (`$H/decoders/dbdump/geometry_banks_mc0607.txt`), `cip_pad/cop_pad` carry the raw words and the
  converter's MC pad decoder is valid on data; else port the pad geometry from `H1CipCopGeo`
  (4.1.1 only: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.1.1/h1oo_include/H1CipCopGeo.h`,
  `FillFromNDB(Int_t runnr)` at `:35`). h1rec has no CIP2k pad decoder (only old CIZ,
  `ct_cizrec/cizupk.F`).
- **D15 Periods (WP3).** Constants for 2005 e− (runs 399101-436893), 2006 e− (444094-468529),
  2006 e+ (468530-492499) and 07low/07med (500918-511079; 460/575 GeV, detector unchanged, so
  included for pretraining by default, Q3). 2003-2004 excluded. Runs below 452777 take
  `IVRSQT = 2` (`$R/calib_consts/calrun.F:13-15`, h1rec `ct_cjcrec/cjrunc.F:465-467`); validation
  A4 exercised it (2005: 0.937, 2006 e−: 0.912 within 10 µm) and IVRSQT 2 and 3 take the same
  hit-position branch; a missing `C3Z8` is harmless in ellipse mode. 2005 needs the
  `$R/mirror/acs/data/05/dst7` symlink (WP3 unit).
- **D16 Validation thresholds.**
  - *Constants chain (regression only).* `python3 -I py/validate_cjc.py .` in `$R/calib_consts`
    reproduces 0.9442 within 10 µm (c070060, Q2e>150, 89 events, 54,062 hits). The same exact path
    (hit3d, ICOR=3) versus `CJXR` on lam/ Q2e>5 gives 0.926 pooled, 0.915-0.944 per run; the floor
    is 0.91 per run. Per new period (WP3): ≥ 0.90 within 10 µm on one full-dump file. This tests the
    NDB constants and `h1hits.py`, not the default representation.
  - *Default representation (D5), r-φ.* Thresholds are data-versus-MC agreement from WP0, never
    absolute quantiles (Risks: resolution figures). Residual: distance of the nearer geometric
    mirror to the reconstructed track circle from CJKR parameters, same code on both samples;
    verdict sample: all 38 lam dirs against `$MC0`. In every |cos β| bin (width 0.1) and
    arc-length bin (10 cm) with ≥ 500 hits in both samples, the data median lies within a factor
    2 of the MC median; the cumulative fractions below 0.5, 1, 2 and 5 mm agree within 5
    percentage points. WP1 and WP2 re-run the same script on written `hits` files and must stay
    within the same tolerances.
  - *z.* Data `cjc_zcd` against the CJKR track z at the hit radius, MC `cjc_zcd` against the
    reco-track z from `cjkr_*`, same helix code; the robust σ of the two residuals agree within a
    factor 2; the fractions of code values (±181, ±191, 201) are reported. `CJXR ZHIT` is never
    the reference for `cjc_zcd` (track-pulled, see Risks).
  - *COZ.* After the `t <= 810 ns` cut, ≥ 99.99% of hits have |Δt| < 2e-3 ns between `COZ.hit()`
    and `CZR2` TIME (100% measured on 1.36M hits, validation A2).
  - *CST (WP4).* Per layer, the signed r-φ residual to the nearest CJKR track has a core width
    below 300 µm (133-231 µm measured) and a median within ±20 µm (±6 µm measured); the rotated-φ
    control median exceeds 5 mm (about 10 mm measured); DCLH match ≥ 99% stays as a
    reproducibility check; |Δz| versus CSHY is reported, not thresholded (partly circular, A3).
  - *CIP/COP (WP5).* Set in its research stage.
- **D17 Run and detector-status selection (supervisor recommendation; confirm at approval).** The
  lam/ sample and the earlier plan selected on `Q2e>5` only (`$R/lam/job.sh:12`), so runs outside
  any good-run list and events with CJC HV off are in. WP2, and WP0/WP1 on the lam/ sample
  retroactively, apply the H1OO standard data selection: the run is accepted by an `H1RunList`
  AND, per event, `H1DetectorStatus::IsOn` holds for the exported subdetectors (CJC1, CJC2, COZ;
  CST and CIP added when WP4 and WP5 land). This is what `H1AnalysisGoodRunCut.C:80-92` does
  (`IsOn` AND `IsSelected`), and H1OO refuses a data chain without it (`H1AnalysisChain.C:718`).
  `IsOn` (`H1DetectorStatus.C:833-860`) reads the HAT branches `Mhv1[43]`, `Mhv2[31]` (CAEN) and
  `Mbbl3[16]`, filled per event from DMIS `Kslow1/Kslow2/Kbbl3` (`H1HatFiller.C:301-313`); a bit
  equal to 1 means off. Bits per subdetector (`H1DetectorStatus.C` setters): CJC1 = HV 0 + CAEN 0;
  CJC2 = HV 1 + CAEN 1, 2; COZ = HV 3 + CAEN 4-6; CJC BBL3 0; CST = HV 36; CIP = HV 7 + CAEN 11.
  No trigger-phase or trigger-element requirement: physics-analysis extras pretraining does not
  need. Measured on the first 20k HAT events of c070060: CJC1 off 0.21%, CJC2 off 0.17%.
  - *Run list.* `$W/H1BaryonAsym/SelectedRuns.root` (`H1RunList`, 16,967 accepted runs
    399629-500611, 297.6 pb^-1, `$W/CONTEXT.md:75`; the low-Q2 V0 list, read the same way in
    `$W/H1BaryonAsym/src/TreeBuilder_v7.C:288-302,857-862`). It is an `oolumi` product: good and
    medium runs, poor rejected (`oolumi.C:313,328-333`), built with CJC1, CJC2, CIP, LAr, ToF, Lumi
    and BDC HV required. It accepts 45 of the 47 lam/ runs (rejects 492559 in `c070000` and
    500490 in `c070108`). H1OO ships no default list: `goodruns9400.txt`
    (`H1AnalysisSteer.C:23`) is a default file name only, and lists are built from the NDB
    run-quality table with `oolumi`. Alternatives: `$W/H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0607.root`
    (8,091 runs 468532-500611, 140.3 pb^-1, high-Q2 NC list, same CJC bits), or a new `oolumi`
    list requiring only CJC and COZ HV, which is also the only route for 07low/07med: no local list
    covers 500918-511079 (Q3).
  - *Where.* In the writer, as a filter on `(run, event)` at write time, not in the probes. The
    status words sit on the HAT the writer already joins for `T0_CJC` (D11), so the dumps need no
    HV word and the probes stay unchanged (D9); the lam/ dumps are filtered without re-extraction;
    a rejected event costs probe time only (about 0.07 s). Rejected: `H1AnalysisGoodRunCut` inside
    `calibprobe` (edits the frozen C++ and changes the dumps' event sets). The run list and the
    run-quality class are dumped once to text tables under `$D/writer/runlist/` (sha256 in `meta`)
    from `SelectedRuns.root` and `H1RunQuality` (`oolumi.C:300-313`), so the PyROOT writer never
    loads the H1OO libraries.
  - *Stored.* `run_selected`, `run_quality`, `det_on`, `hv_off`, `caen_off`, `bbl3_off` per event
    (D7), so the selection can be tightened later from the trees; relaxing it means rerunning the
    writer with `--select none` on the kept dumps (D9). `meta` holds the D7 counters so each
    criterion's removal fraction is reported (Slices 1 and 2).

## Outline

Pointers to follow; signatures are the contract, bodies are executor's.

`$D/validation/parity/parity_cjc.py` (new, WP0, about 150 lines)
- `mc_residuals(hits_file) -> table`: from `$MC0`, hits with `cjc_reco_trk >= 0`; wire = midpoint
  of the two mirrors, drift direction = their difference; circle and helix from
  `cjkr_kappa/phi/dca/theta/z0` (`extract_hits.C:189-191`); residual = distance of the nearer
  mirror to the circle; |cos β| per `h1hits.py:378`; arc length from the DCA point; z residual
  `cjc_zcd − z_track` for hits without a code value. `cjc_reco_side` cross-checks the mirror choice.
- `data_residuals(lam_dirs, consts_dir) -> table`: the same quantities with `CJC.simple()` mirrors
  (D5; `cjavt0` T0 suffices here) against the CJKR row of `XR.itr`, reading `events.txt` and
  `cjcprobe.txt` (the per-hit CSVs `$V/cjc_lam/out/*.hits.csv.gz` from validation A1 may seed
  it); `XR` columns `COSB`, `SLEN`, `ZEXP` cross-check the data-side geometry.
- `compare(mc, data) -> parity.json`: binned medians, CDF points, robust σ of z, the D16 verdict.
- Outputs, absolute: `/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/validation/parity/parity.json`
  and, under `/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/validation/parity/plots/`,
  `rphi_median_vs_cosb.png`, `rphi_median_vs_slen.png`, `rphi_cdf.png`, `zcd_width.png` (data and
  MC overlaid, D16 tolerance bands drawn).
- Gotchas: the MC drift direction is fixed per wire (`extract_hits.C:57`), data uses the per-run
  Lorentz angle; the comparison is of the representation, not of the Lorentz angle. Login node:
  `$MC0` plus at most 5 lam dirs under 5 min; more goes to one Condor job (`parity.sub`).

`$D/writer/write_hits_data.py` (new, about 450 lines)
- `read_events(path) -> iter[EventDump]`: parse `events.txt` line types `EV`, `#`, `CJKR/CJKM/CJKT/CJKV i <int32 words>`
  (floats are bit patterns, reinterpret as `$R/calib_consts/py/events.py` `flt()` does), `H i wire dl q+ q- f1 f2`,
  `XR ichan itr kdr DRFM XHIT YHIT ZHIT WR WZ QHIT DRFE TLEN COSB SINB SLEN ZEXP DCOR ZCOR BZ`
  (`$R/calib_consts/README.md:17`; `kdr` is the 1-based `H` row), `CRZE`, `CZR2`. Reuse
  `events.read_events` where it fits; the writer must not edit `$R`. The `# CJCC` line is ignored (D11).
- `read_cjcprobe(path) -> iter[(run, evt, raw_hits, trk, crze)]`: `EV`, `TRK`, `RAW CRZE`, `HIT wire dl qplus qminus flag1 flag2`
  (`$R/cjcprobe/cjcprobe.C:30-70`); `HIT` lines belong to the preceding `EV`, banks concatenated.
- `read_hat(hat_file) -> dict[(run, event) -> (T0_CJC, Mhv1, Mhv2, Mbbl3)]`: uproot on the HAT
  branches `RunNumber`, `EventNumber`, `T0_CJC` (D11), `Mhv1`, `Mhv2`, `Mbbl3` (D17).
- `select(run, status, run_table) -> (run_selected, run_quality, det_on, hv_off, caen_off, bbl3_off)`:
  `det_on` replicates `H1DetectorStatus::IsOn` for the D17 bit set (`H1DetectorStatus.C:860-861`,
  index only the listed bits; the HAT arrays are shorter than the class loops); `run_selected` and
  `run_quality` from the text tables of `dump_runlist`.
- `join(events, cjc) -> iter[Event]`: key `(run, evt)`; both dumps are in mODS order with equal
  `EV` counts (1,501 per lam dir). Invariant: every `XR` row maps to exactly one raw hit through
  `kdr -> H row -> (wire, dl)` after `CJC.qht8(row)` (`h1hits.py:40`); unmapped rows are an error.
- `coords(ev, cjc: h1hits.CJC) -> dict of vectors`: `CJC.prepare_event(raw_hits)` (`:354`), T0 per D11
  (`cjavt0` at `:397`), `CJC.simple(hit, t0, side=±1)` (`:375`) for the two mirrors (`drd_final` is
  in 10 µm; cm = ×1e-3), `CJC.cell(ich)` (`:121`) for ring/layer/cell, wire x/y from `cp_x/cp_y`
  in the consts json. `cjc_reco_trk` = 0-based index of `XR.itr` into the event's CJKR list,
  `cjc_reco_side` from the sign of `DRFM` (executor confirms the sign convention against
  `extract_hits.C:171` and README:17). COZ per D12 with the `t <= 810 ns` cut.
- `fill_tree(tree, vectors)`: branch declarations mirror `extract_hits.C:121-247` one-to-one (names,
  `std::vector<float>` versus `std::vector<int>`, scalar Int_t for `run`, `event`, `n_str_all`,
  `n_svx_all`); plus the D7 additions and the `meta` TNamed.
- CLI: `write_hits_data.py --lam-dir <dir> --consts-dir <dir> --hat <file> --run-table <txt> --out <hits_part_X.root> [--max-events N] [--select standard|none]`;
  the consts json is chosen per event by run (1-3 runs per lam dir); `--select none` writes every
  event with the D17 branches filled (D17 relaxation path).
- Gotchas: `python3` after `desygroupset h1` and `thish1.sh`, never `python3 -I` (D8); `cjkr_*`
  column mapping must follow the CJKR bank layout used by `extract_hits.C:189-198` (28 raw words
  per row in `events.txt`; `cjkr_cov9` only if the words exist, else empty); COZ `CRZE` rows are
  six unsigned words (`events.txt` `CRZE i <6>`); a lam dir can hold `EV` lines with no `HIT`
  lines (D10).

`$D/writer/runlist/dump_runlist.py` (new, WP1, about 40 lines)
- Writes `runs_selectedruns.txt` (columns run, selected, quality, lumi) from the `H1RunList` of
  `SelectedRuns.root` (read as `TreeBuilder_v7.C:857-862`) and `H1RunQuality::Quality(run)`
  (`oolumi.C:300-313`, needs the NDB). Needs the H1OO libraries: in PyROOT load ROOT `libEG` first,
  then the libraries in `ldd oolumi` order (`TDatabasePDG::Instance` is otherwise undefined;
  checked 2026-10-28); if the NDB access does not work from PyROOT, build it as a small C++ program
  in the `oolumi` pattern instead. Run once per list; output and sha256 recorded in `meta`.

`$D/validation/check_schema.py` (new, about 80 lines)
- `compare(data_file, ref_file, allowlist) -> report`: branch name and leaf type sets via uproot
  (`library="np"` as `$H/decoders/validate_decoders.py:36`); PASS when the MC set is a subset with
  identical types and every extra branch is in the allowlist; prints per-branch non-empty fractions.

`$D/validation/validate_data_cjc.py` (new, about 120 lines)
- Reads the written `hits` file, not the dumps, so it also exercises the tree. Exact path: the
  constants-chain regression of D16 (fraction within 10 µm versus `cjc_trk_x/y`, pooled and per
  run). Default representation: imports `parity_cjc.mc_residuals` and runs it on the data file
  (the data tree has the same branches), then `compare` against the stored MC table from WP0.
  Prints the D16 verdicts; JSON output like `validate_decoders.py` (`validation.json`).

`$D/condor/job_realdata.sh`, `$D/condor/realdata_2007ep.sub` (new, WP2)
- Pattern: `$R/lam/job.sh` (env: `desygroupset h1`, `thish1.sh`, bash; writes `s.steer` with
  `fHatFiles`/`fModsFiles`/`fHatSelection="Q2e>5"`; `calibprobe -f s.steer -n N`; `make_consts.py`;
  `cjcprobe -f s.steer -n N`) plus the `dataprobe` step of `$R/scale/job.sh`, then the writer with
  `--hat`, then a `DONE` marker. Submit pattern: `$R/lam/job.submit` (`+RequestRuntime`,
  `request_memory = 3000`, `requirements = OpSysAndVer == "RedHat9"`, `queue f in (...)`) and the
  workspace style `universe = vanilla`, `RequestCpus = 1` (`$W/lambda/scripts/xqz3d/*.sub`). Output
  layout: `$H/production/realdata_2007ep/q2e5_n5000/{dumps/<file>/, hits/hits_part_<file>.root, logs/}`.
  Mirror: `$R/mirror/acs/data/07/dst7` already exists. `-n 5000` (D3).

`$R/calib_consts/calibprobe.C` (edit, WP3, about 20 lines)
- A steering or environment flag `CONSTS_ONLY` that keeps the per-run `CALRUN` dump and
  `banks_<run>.txt` and suppresses the per-event `EV/H/XR/...` output (per-event dump is at
  `calibprobe.C:96-232`, new-run detection and `CALRUN` at `:117-128`, the per-run `caldmp` and
  `banks_<run>.txt` dump at `:230-237`), built to a separate binary `bin/calibprobe_consts` so
  `bin/calibprobe`, which WP2 runs on Condor, is never replaced. Without the flag a full-file pass
  writes about 15 GB of text per file.

`$D/consts/<period>/` (WP3 outputs): `consts_<run>.json` per run (about 1 MB each), `bankdiff.txt`
per period from a diff script modelled on research §3 (which `raw_ndb_banks` vary across runs).
Mirror symlink needed: `$R/mirror/acs/data/05/dst7 -> $P/05/dst7` (06 and 07 exist).

WP4 and WP5 outlines are written in their child plans after their research stages (see Work
Packages); the facts they start from are in D13 and D14.

## Slices

Slices map one-to-one onto packages; executor runs them in order, WP3 may run in parallel with
WP0-WP2 (disjoint files; WP3 never replaces `$R/calib_consts/bin/calibprobe`).

0. **WP0: representation parity.** Delivers `parity_cjc.py`, `parity.json` and the four PNGs
   under `/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/validation/parity/plots/`.
   Units: one (script and plots).
   Tests: `mc_side_check: $MC0 -> nearer mirror equals cjc_reco_side for ≥ 90% of on-track hits` (data: 98%, validation A1);
   `median_vs_cosb: data/MC median ratio within [0.5, 2] in every |cos β| bin with ≥ 500 hits in both`;
   `median_vs_slen: the same per 10 cm arc-length bin`;
   `cdf: |F_data − F_MC| ≤ 0.05 at 0.5, 1, 2 and 5 mm`;
   `zcd_width: robust σ(data zcd − z_track) / robust σ(MC zcd − z_track) within [0.5, 2]`;
   `plots: four PNGs exist with data and MC overlaid and the tolerance bands drawn`.
   Checkpoint: smoke run on the login node with `$MC0` plus at most 5 lam dirs (under 5 min);
   the verdict comes from all 38 lam dirs via `parity.sub`, and `parity.json` holds
   `verdict: PASS`. A FAIL returns to the user (Risks), not to WP1.
1. **WP1: lam/ to `hits` tree.** Delivers 38 `hits_part_<file>.root` under
   `$H/production/realdata_2007ep/lam_q2e5/hits/` plus schema and validation reports.
   Units: (a) writer, (b) `check_schema.py`, (c) `validate_data_cjc.py` (disjoint files).
   Tests: `schema_parity: hits_part_c070060.root vs $MC0 -> PASS, extras ⊆ allowlist`;
   `event_count: c070060 -> 1501 minus (rejected + empty) events, both counts in meta`;
   `xr_mapping: every XR row of c070060 -> exactly one raw hit (0 unmapped)`;
   `h1hits_regression: python3 -I py/validate_cjc.py . in $R/calib_consts -> 0.9442 within 10 µm`;
   `exact_path_lam: 38 files -> pooled 0.926 ± 0.01, every run ≥ 0.91`;
   `representation_parity: validate_data_cjc.py on the 38 files -> within the WP0 tolerances (D16 r-φ and z)`;
   `t0_src: every event has cjc_t0_src in {0, 1, 2}; events with a HAT T0_CJC have 0; meta counts match`;
   `run_list: 38 files -> exactly runs 492559 (c070000) and 500490 (c070108) rejected by the run list, every other run accepted`;
   `hv_vs_cjc_off: c070000 with --select none -> all 1,316 zero-CRJE events have det_on == 0 (CJC1 or CJC2 bits off)`;
   `selection_fractions: meta of the 38 files -> D17 counters present, pooled fractions by run list and by status bits printed, seen == written + rejected + empty in every file`;
   `truth_empty: any file -> all (f)-group branches empty, n_str_all == 0, is_data == 1`.
   Checkpoint: one lam dir on the login node (one file, under 5 min), then the 38 dirs as one
   Condor job array (`$D/condor/lam_convert.sub`), then `check_schema.py` and
   `validate_data_cjc.py` over the 38 outputs.
2. **WP2: Condor extraction, 2007 e+p.** Delivers `$H/production/realdata_2007ep/q2e5_n5000/`
   with 112 `hits_part_<file>.root`, dumps and logs. Units: (a) `job_realdata.sh`, (b) `.sub`
   and layout, (c) in-job validation call (reuses WP1 scripts).
   Tests: `smoke: c070060, -n 300 on Condor -> DONE, hits file with 300 minus (rejected + empty) events, schema PASS`;
   `multi_run: a file whose first 5000 events span 2+ runs -> per-event consts selection, meta lists all runs`;
   `production: 112 files -> 112 DONE, sum of events in meta about 560k, 0 jobs over runtime`;
   `validation_sample: validate_data_cjc.py on every 10th file -> D16 thresholds`;
   `selection_fractions: 112 metas -> run-list and status-bit fractions printed per file and pooled; pooled status-bit fraction in accepted runs below 1% (D17)`.
   Checkpoint: `condor_submit $D/condor/realdata_2007ep.sub`, then `ls .../hits | wc -l` = 112 and the
   validation JSONs.
3. **WP3: constants for 2005, 2006, 07low/07med.** Delivers `$D/consts/<period>/consts_<run>.json`
   for every run met in the mODS files of each period, `bankdiff.txt` per period, and one exact-path
   validation per period. Units: (a) `calibprobe.C` flag and the `calibprobe_consts` build target,
   (b) `consts_<period>.sub` and the 05 mirror symlink, (c) diff script.
   Tests: `consts_only_mode: calibprobe_consts on c070060 -n 1500 -> consts_497718.json and consts_497719.json byte-identical to $R/lam/c070060/calib/`;
   `mirror_05: $R/mirror/acs/data/05/dst7 -> IndexDST resolves for c050000 (CALRUN ier 0)`;
   `ivrsqt2_path: one 2005 run -> CALRUN ier 0, consts json has the cjc and coz blocks`;
   `period_validation: one file per period, full dump -n 1500 -> ≥ 0.90 within 10 µm vs CJXR`;
   `bankdiff: per period -> list of varying banks; C0J8 Lorentz word within 0.60-0.64 rad`.
   Checkpoint: per period, `ls $D/consts/<period>/consts_*.json | wc -l` versus distinct runs in the
   period's mODS files, and the validation JSON.
4. **WP4: CST.** Delivers `cstprobe` in the WP2 job, `cst_sp_*` branches and the CST validation.
   Tests (from D16): `per_layer_width < 300 µm`, `per_layer_median within ±20 µm`,
   `rotated_control median > 5 mm`, `dclh_match ≥ 99% (reproducibility)` on ≥ 1,000 events;
   `schema: cst_sp_* in allowlist, csts_* empty`. Checkpoint: child plan's.
5. **WP5: CIP/COP.** Delivers the data pad geometry decision (CGAM equality or ported geometry),
   `cip_pad/cop_pad` filled, validation against CJKR extrapolation. Tests set in its research stage
   (candidate: fraction of tracks with pt > 0.3 GeV having a pad within one pitch in ≥ 3 of 5 CIP
   layers). Checkpoint: child plan's.
6. **WP6: provenance and handoff.** Delivers `$D/docs/README.md`, a data section in `$H/HANDOFF.md`
   and an erratum note for both research artifacts (list in Risks). Tests: `paths: every path in the docs exists`;
   `meta_consistency: run lists in meta == consts json present`; `errata: every item of the Risks
   erratum list appears in the note`. Checkpoint: `critic` read of the docs plus a path check script.

## Verification

After WP0-WP2 (first release): `parity.json` PASS; `check_schema.py` PASS on every output file;
`validate_data_cjc.py` on the WP2 validation sample within D16; `meta` totals equal the file count
and events written; a sample file reads with the `uproot` snippet in `$H/HANDOFF.md:216-220` with
the same code as an MC file. WP3-WP5 add their own checks; WP6 closes with the documentation
checks.

## Risks

- **Inferred converter schema.** `h1_to_fm4npp.py` is on LCRC only; which branches it needs, and
  whether it tolerates empty truth vectors or extra branches, is unknown (research B9). Mitigation:
  exact MC branch set, additive extras, Q1.
- **Representation parity may fail for physical reasons.** Data and MC geometric mirrors differ by
  the data drift velocity and Lorentz angle, the H1SIM smearing and the event mix; if WP0 exceeds
  the D16 tolerances the result goes to the user as a decision (accept, tighten, or change D5), not
  to executor.
- **CJXR z is track-pulled.** Exact-path z residuals against `CJXR ZHIT` are log-uniform from 1 µm
  to 1 mm with about 16% above 1 mm and an overflow at 1 m (`$V/plots/cjc/fig1_residual_hists.png`
  panel c, `$V/plots/consts_periods/dz_exact_path.png`), so `ZHIT` is not plain charge-division z
  (inference) and is never the reference for `cjc_zcd` (D16 z).
- **Fragile probe workarounds.** Patched `CJXREF` work bank (`MACC=200*NRCJKR+2000`,
  `$R/calib_consts/build_lib.sh`), skipped `CJCRJE`, private `GUFLD` (`gufldp.F`, 11.608 kG) per
  `$R/calib_consts/README.md:148-162`. A rebuild of `libcjcprobe.so` or an H1OO release change can
  break them silently; WP6 records the library hashes, WP1's `h1hits_regression` test runs first.
- **Resolution figures are not comparable across metrics.** 0.26 µm / 94% are the exact path on
  c070060 `Q2e>150` (89 events); 634 and 604 µm are robust σ against CJKR circles; the data
  track-free 95% quantile is 7.3 mm. The default representation is judged against MC (D16), not
  against a resolution target.
- **Selection coverage.** The run list is stricter than CJC/COZ alone and ends at 500611 (D17, Q3).
  The status words update about every 10 s (`H1AnalysisGoodRunCut.C:79`), so shorter trips pass,
  and a partially read-out CJC (some sectors off) is not a status bit; the D10 empty-event skip and
  the reported `meta` counters cover the gross case only.
- **Period coverage.** 2005 and 2006 e− constants are validated on about 30 events each (A4); the
  2006 e+ hit-level check is missing (needs a CJC-on file); `mirror/acs/data/05` does not exist
  yet; dst7/gmcut tape locality was not checked.
- **CRME absence.** 0 `RAW CRME` lines in 120 dumped 2007 events (`$R/data_display/dataprobe.txt`),
  so COP may be absent from the raw stream or not read; WP5 research settles it.
- **No SIFE in lam/.** The lam dumps hold only `calib/` and `cjc/`; CST for the lam sample needs a
  `cstprobe` pass (WP4), not a re-read of lam/.
- **Volume.** Dumps are about 150 KB per event, so the first release needs about 100 GB of DUST
  including the `hits` trees (D3, D9).
- **Python environment.** `python3` resolves to `/usr/bin/python3` unless `desygroupset h1` runs
  before `thish1.sh` (D8); `python3 -I` drops the LCG `PYTHONPATH` (`$H/HANDOFF.md:235`); the writer
  and the probe scripts need different invocations in one job.
- **Errata** to carry into WP6's note. Research artifact: `CRPE/CRTE` are not banks (D14);
  `H1CipCopGeo.h` is in release 4.1.1, not 4.2.1; `calrun.F:13-15` is the probe's copy (h1rec:
  `cjrunc.F:465-467`); the lam sample is the first 1,501 `Q2e>5` events per file, every third file,
  so "70 runs" are a sample of 2007; the event T0 is on the DST (DMIS) and HAT, only CJCC is not.
  Validation note: the CST "over all 300 tracks" fractions use the wrong denominators (227 for
  c070030 and 163 for c070100 per the logs); run and file counts on canvases differ by one from
  the text; `$V/plots/coz/coz_fig5_perfile.png` has unlabelled 15.5 mm and about 7e-3 ns lines;
  the D16 "95% < 2 mm" FAIL compared against a threshold the plan has withdrawn.

## Scope Notes

In scope: WP0-WP6 as above. Out of scope: changes to the simulation chain or `extract_hits.C`;
the converter (LCRC); forward tracker, calorimeters; 2003-2004, HERA-I; exact CJCC event T0
(CJCREC rerun; DMIS/HAT replaces it, D11); track-dependent CJC corrections in the default
coordinates (`CJHLXE/CJHLXH` terms, README:141-147; D5, executor must not reopen); moving `$R`
into the workspace; editing the two research artifacts (WP6 appends an erratum note instead).
No user decisions are recorded in memory for this topic; D1-D4 are supervisor defaults and D5 and
D17 the supervisor's recommendations until the user confirms them.

## Assumptions

- `calibprobe`, `cjcprobe`, `dataprobe` and `cstprobe` binaries in `$R` still run against the
  current H1OO 4.2.1 release; checked by WP1's regression test and WP2's smoke job.
- The 2007 dst7/gmcut files referenced by the 112 mODS `IndexDST` are online; checked by the
  smoke job's `CALRUN ier` and by Condor job exit codes.
- The AFS NDB (`/afs/desy.de/group/h1/ndb_read_only_dp`) stays readable; checked at every job start
  (job.log line "H1 Master Database").
- About 100 GB free on DUST under `$H/production/`; checked with `df` before WP2 submission.
- The HAT files named in each lam dir's `s.steer` are still readable for the `T0_CJC` and
  status-word join (D11, D17); checked by WP1's `t0_src` and `selection_fractions` tests.

## Work Packages

### WP0: Data/MC representation parity
- Goal: show that the D5 geometric mirrors on data behave like MC `cjc_xp/xm` from
  `extract_hits.C`; done when `parity_cjc.py` reports the D16 r-φ and z agreement between `$MC0`
  and the lam/ sample and the four PNGs exist.
- Owns: `$D/validation/parity/` (script, `parity.sub`, `parity.json`, `plots/`)
- Deps: none
- Stages: execute
- Status: blocked: execute: user decision on the D16 FAIL (accept, tighten or change D5) and on COZ in det_on
- Artifacts: ->

### WP1: lam/ to hits tree and schema parity
- Goal: convert the 38 `$R/lam/` dumps into `hits` trees; done when `check_schema.py` passes on all
  38 files against `$MC0` and the Slice 1 tests hold.
- Owns: `$D/writer/` (including `runlist/`), `$D/validation/` except `parity/`,
  `$D/condor/lam_convert.sub`, `$H/production/realdata_2007ep/lam_q2e5/`
- Deps: WP0
- Stages: execute
- Status: blocked: execute: WP0 parity verdict must be PASS
- Artifacts: ->

### WP2: Condor extraction pipeline, 2007 e+p
- Goal: one Condor job per 2007 mODS file running the unchanged probes plus the WP1 writer; done
  when 112 `DONE` markers, 112 `hits_part_*.root`, about 560k events in `meta`, and the validation
  sample meets D16.
- Owns: `$D/condor/job_realdata.sh`, `$D/condor/realdata_2007ep.sub`,
  `$H/production/realdata_2007ep/q2e5_n5000/`
- Deps: WP1
- Stages: execute
- Status: blocked: execute: needs the WP1 writer and validation scripts
- Artifacts: ->

### WP3: constants for 2005, 2006, 07low and 07med
- Goal: a `consts_<run>.json` for every run in the mODS files of the four periods, with a bank
  diff and one exact-path validation per period; done when the Slice 3 tests hold.
- Owns: `$R/calib_consts/calibprobe.C` (flag only), the new `calibprobe_consts` target in
  `$R/calib_consts/Makefile` and its binary `$R/calib_consts/bin/calibprobe_consts`,
  `$R/mirror/acs/data/05/`, `$D/consts/`, `$D/condor/consts_<period>.sub`
- Deps: none
- Stages: execute
- Status: done
- Artifacts: ->

### WP4: CST space points on data
- Goal: `cst_sp_*` branches from the h1rec CST chain in the WP2 job, validated per D16; done when
  the Slice 4 tests hold on ≥ 1,000 events and the schema check passes with the extended allowlist.
- Owns: `$D/cst/`, the `cstprobe` step in `$D/condor/job_realdata.sh`, `cst_sp_*` in the writer
  (coordinated edit after WP2 lands)
- Deps: WP2
- Stages: research, plan, execute
- Status: blocked: research: WP2 layout and raw SIFE dumps must exist first
- Artifacts: `.supervisor/working/research/research-fm4npp-cst-data.md`,
  `.supervisor/working/plans/plan-fm4npp-cst-data.md`

### WP5: CIP/COP data decoder
- Goal: data pad geometry settled (CGAM equality or `H1CipCopGeo` port), `cip_pad/cop_pad` filled
  from `CRM2/CRME`, validated against CJKR extrapolation; done when the child plan's tests hold.
- Owns: `$D/cipcop/`, `$H/decoders/dbdump/` data-stream variant, `cip_*`/`cop_*` in the writer
  (coordinated edit after WP2 lands)
- Deps: WP2
- Stages: research, plan, execute
- Status: research
- Artifacts: `.supervisor/working/research/research-fm4npp-cipcop-data.md`,
  `.supervisor/working/plans/plan-fm4npp-cipcop-data.md`

### WP6: provenance and handoff
- Goal: document the data path, formats, run lists, hashes and errata; done when `$D/docs/README.md`
  and the `$H/HANDOFF.md` data section exist, every cited path resolves, `meta` run lists match
  the consts present, and the erratum note covers the Risks list.
- Owns: `$D/docs/`, the data section of `$H/HANDOFF.md`, an erratum note for both research
  artifacts (appended, never edits in place)
- Deps: WP1, WP2
- Stages: write
- Status: blocked: write: WP1 and WP2 outputs are its subject
- Artifacts: ->

## Open Questions

Each with the recommended answer; D1-D5 stand until the user answers.

- **Q1** Can `h1_to_fm4npp.py` be copied to NAF (or its branch list shared), so `check_schema.py`
  can assert the real requirement instead of MC parity? Recommended: copy it; until then D1.
- **Q2** Is pretraining self-supervised, or does it consume labels? Recommended: self-supervised;
  pseudo-labels stay optional (D2).
- **Q3** Include 07low/07med (460/575 GeV) in WP3? Recommended: yes for pretraining; drop if the
  inputs must match the 319 GeV analysis. If yes, their extraction needs a run list built with
  `oolumi` (none in the workspace covers 500918-511079, D17); WP3 constants are unaffected.
- **Q4** First-release size: 5,000 events per file (about 560k) as in D3, a stride within each
  file (`fHatSelection = "Q2e>5 && EventNumber%29==0"`; HAT has `RunNumber`, `EventNumber`, `Q2e`),
  or more? Recommended: D3, then scale by `-n`.
- **Q5** For CST, will the converter be changed to read `cst_sp_*` point branches (D13)? If not,
  WP4 must instead produce data strips in CSTS packing and accept the MC decoder's known bias.

## Log

- 2026-10-27 planner: annotation cycle 1 from the component-validation note, the plot review and
  the scout read of `extract_hits.C`; WP0 added, D5, D7, D8, D11, D12, D13, D15, D16 revised,
  Risks and WP6 errata extended; deferred: WP4/WP5 `Owns` overlap with WP1/WP2 writer and job
  files (pre-existing, sequential via `Deps`; outside this cycle's scope)
- 2026-10-27 decision: D5 default CJC coordinates are the MC geometric representation
  (`CJC.simple()`, `cosb=1`, per-run NDB constants); `CJXR` coordinates are optional labels in
  `cjc_trk_x/y/z`, never the default input; source: user
- 2026-10-27 decision: WP0 representation parity precedes WP1; D16 default-representation
  thresholds are data-versus-MC agreement (factor 2 on binned medians, 5 pp on the CDF, factor 2
  on the zcd width), not absolute quantiles; source: lead
- 2026-10-27 decision: D16 exact path (0.9442 on c070060; 0.926 pooled, floor 0.91 per run on
  lam/) is a regression test of the constants chain only; source: lead
- 2026-10-27 decision: D11 T0 is HAT `T0_CJC` (DMIS `TZERO_M`), `cjc_t0_src` 0 = HAT, 1 = run
  default, 2 = `cjavt0`; the `events.txt` `# CJCC` line is never used; source: lead
- 2026-10-27 decision: D12 `coz_trk_x/y/z` from `COZ.hit()` with the `CZR2`-matched track and
  `t <= 810 ns`; source: lead
- 2026-10-27 decision: D13/D16 CST acceptance is the per-layer signed r-φ residual width with the
  rotated-φ control; the DCLH match is a reproducibility check; source: lead
- 2026-10-27 decision: D8 `desygroupset h1` before `thish1.sh`, or the LCG python by full path;
  source: lead
- 2026-10-27 decision: data z is validated as `cjc_zcd` against the CJKR track z, never against
  `CJXR ZHIT` (track-pulled); source: lead
- 2026-10-27 decision: D15 WP3 owns the `mirror/acs/data/05/dst7` symlink; IVRSQT 2 and a
  missing `C3Z8` need no special handling; source: lead
- 2026-10-28 planner: annotation cycle 2 (user question on good-run selection); D17 added, D7,
  D10, Outline, Slices 1-2, Verification, Risks, Q3 revised; deferred: none
- 2026-10-28 decision: D17 data events pass the H1OO standard selection, run accepted by the
  `H1RunList` of `H1BaryonAsym/SelectedRuns.root` AND `H1DetectorStatus::IsOn` for CJC1, CJC2,
  COZ (CST, CIP later) from the HAT status words, applied in the writer; no trigger requirement;
  `run_selected`, `run_quality`, `det_on` and the off-bit lists stored per event; alternative list
  `nc_goodruns_0607.root` or a new `oolumi` list; source: supervisor default, user to confirm
- 2026-10-28 decision: user approved the plan and wave 1 ("Sure, go ahead with wave 1."): defaults D1–D5, D16, D17 stand; WP0 and WP3 started
- 2026-10-28 WP0: parity_cjc.py, parity.json and four figures done on all 38 lam dirs vs $MC0; verdict FAIL (CDF −7 to −8.5 pp at 0.5-2 mm; arc-length bin 10-20 cm ratio 2.07; |cos β| medians and zcd width 1.55 pass); D17 removes 2.67% by run list and 8.30% by status bits (COZ HV dominates); commit 6099cb9; deferred: user decision on FAIL (accept, tighten or change D5; T0 confound untested, 23% of events on run-default T0), COZ in det_on
- 2026-10-28 WP3: done; consts for 19,888 runs (2005 6,605, 2006 10,264, 07low 2,192, 07med 827), 0 failed, AFS NDB, IVRSQT 2/3 as calrun.F; regression 0.920-0.927 on 6 files (all >= 0.90); commit 3ec0514; deferred: COZ re-dump of 110 runs with empty COZ constants, Lorentz band 0.60-0.64 holds for 07low only (2005/2006/07med partly outside; band choice for the user), 07low/07med run list (Q3/D17), cjxref segfault and piped-wrapper early exit to handle in WP2

## Todo

- [x] WP0 `parity_cjc.py`: `mc_residuals` on `$MC0`, `data_residuals` on the 38 lam dirs (login
      smoke on ≤ 5), `compare`
- [x] WP0 four PNGs and `parity.json`; verdict PASS or return to the user
- [ ] WP1 writer: `read_events`, `read_cjcprobe`, `read_hat`, `join`, `select`, `coords`, `fill_tree`, CLI, `meta`
- [ ] WP1 `dump_runlist.py` (run list and run quality tables, sha256); D17 branches; `run_list`,
      `hv_vs_cjc_off` and `selection_fractions` tests
- [ ] WP1 `check_schema.py` with the D7 allowlist; run against `$MC0`
- [ ] WP1 `validate_data_cjc.py`; run `h1hits_regression` first
- [ ] WP1 login-node run on one lam dir; Condor array over 38; the eleven Slice 1 tests
- [ ] WP2 `job_realdata.sh` (probes + dataprobe + writer `--hat`) and `.sub`
- [ ] WP2 smoke `c070060 -n 300`; `df` check; production 112 files; validation every 10th file
- [x] WP3 `CONSTS_ONLY` flag in `calibprobe.C`, built as `bin/calibprobe_consts`; byte-identity test on c070060
- [x] WP3 mirror symlink 05; per-period submit; bank diff; one full-dump validation per period
- [ ] WP4 research and child plan (SIFE, cstprobe in job, `cst_sp_*`, D16 CST tests)
- [ ] WP5 research (data CGAM versus MC; CRME presence) and child plan
- [ ] WP6 `$D/docs/README.md`, `$H/HANDOFF.md` data section, erratum note (Risks list), hash and
      path checks

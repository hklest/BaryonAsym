# Plan: real H1 hit-level data for FM4NPP pretraining

Date: 2026-10-27. Stage: planning (program with work packages). Research artifact:
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/research/2026-10-27-fm4npp-h1-real-data-blockers.md`
(cited below as "research §n").

Abbreviations: `$W` = `/data/dust/group/h1/klesthen/BaryonAsym`, `$H` = `$W/hit_level_sim`,
`$R` = `/data/dust/group/h1/klesthen/rawhit_probe`, `$P` = `/pnfs/desy.de/dphep/online/h1/data`,
`$D` = `$H/realdata` (new; see D6), h1rec = `/data/dust/group/h1/sschmitt/h1soft/h1rec`,
H1OO env = `source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh`
(as in `$H/env.sh`).

## Goal

Produce real H1 HERA-II hit-level inputs in the `hits` tree format of `$H/extract_hits.C`, so the
FM4NPP converter `h1_to_fm4npp.py` (LCRC only) reads real data as it reads simulation. Real data
takes the NDB-driven path (`$R/calib_consts/py/h1hits.py` with per-run `consts_<run>.json`, h1rec
coordinates from `calibprobe`), never the MC geometry or calibration hard-coded in
`$H/extract_hits.C:16-57`.

Non-goals: changing the simulation chain or `extract_hits.C`; changing the converter on LCRC;
forward tracker, LAr, SpaCal; 2003-2004 and HERA-I data; supervised truth labels for data (none
exist, research B10).

Expected change size: about 1,400 lines across the program: WP1 writer and checks about 600 lines
of Python; WP2 job wrapper, submit file and layout about 150; WP3 about 100 (one flag in
`calibprobe.C`, one submit, one diff script); WP4 and WP5 about 200 each; WP6 documentation about
150. This plan is about 420 lines.

## Approach

One data-side writer (`$D/writer/write_hits_data.py`) reads the text dumps the existing probes
already produce (`events.txt` from `calibprobe`, `cjcprobe.txt` from `cjcprobe`, later
`dataprobe.txt` and `cst_full.txt`) plus `consts_<run>.json`, computes track-free CJC coordinates
with `h1hits.py`, and writes a `hits` tree with the branch list of `extract_hits.C` (truth branches
present and empty, data-only branches additive). WP1 runs it on the 38 `$R/lam/` dumps; WP2 wraps
the unchanged probes plus the writer into one Condor job per 2007 e+p mODS file; WP3 dumps
constants for the other periods with a constants-only mode; WP4 and WP5 add CST and CIP/COP after
their own research; WP6 records provenance and updates the handoff.

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

## Decisions

Defaults D1-D5 were adopted by the supervisor after the user answered "continue" without choosing;
the user may overturn each at plan approval. D6-D16 are lead decisions with evidence.

- **D1 Converter (default; confirm).** Match the `hits` tree schema of the current
  `$H/extract_hits.C` (reference file `$H/production/cov_loQ2/loQ2/hits/hits_part_0.root`, 110
  branches) exactly; data-only branches are additive and allowlisted (D7). The converter's exact
  needs stay a risk until the user copies it to NAF (Open Question Q1).
- **D2 Labels (default; confirm).** Pretraining is self-supervised; H1 reconstruction pseudo-labels
  (`cjc_reco_trk`, `cjc_reco_side`, `cjkr_*`, `cjkt_*`, `cjkv_*`) are carried as in MC but never
  presented as truth (`is_data = 1`, D7).
- **D3 First-release scope (default; confirm).** 2007 e+p, HAT selection `Q2e>5`. WP1 converts
  `$R/lam/` (57,038 events, 47 runs). WP2 extracts the first 5,000 `Q2e>5` events of each of the 112
  2007 mODS files, about 560k events. Rationale: about 10× the existing sample and 5× the 100k MC
  pools (`$H/production/sherpa100k*`, `pythia100k`); about 6 min per file at 0.07 s/event; about
  85 GB of text dumps (about 150 KB per event: `events.txt` 160 MB per 1,501 events in
  `$R/lam/c070060/calib/`, `cjcprobe.txt` 26 MB, `dataprobe.txt` about 30 KB per event) and about
  15 GB of `hits` trees; the full 16M events stay one `-n` change away. Measured: `c070060` HAT has 797,529 entries, 144,909 with `Q2e>5`.
- **D4 Detectors (default; confirm).** First release: CJC digis and coordinates, COZ digis and
  on-track coordinates. CST (WP4) and CIP/COP (WP5) follow; their branches stay empty until then.
- **D5 Coordinates (default; confirm).** Track-free coordinates are the default input: `cjc_xp/yp`,
  `cjc_xm/ym`, `cjc_zcd`, `cjc_drift` from `CJC.simple()` with the event T0 of D11 (r-φ about
  634 µm, `$R/calib_consts/README.md:132-137`). Track-informed h1rec coordinates (262 µm) go to new
  branches `cjc_trk_x`, `cjc_trk_y`, `cjc_trk_z`, copied from `CJXR` (`XHIT YHIT ZHIT`) for hits with
  `cjc_reco_trk >= 0`, zero otherwise.
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
  `cjc_t0` (Float_t, ticks), `cjc_t0_src` (Int_t, 0 = `cjavt0`, 1 = run default); vectors
  `cjc_trk_x/y/z` (D5), `coz_trk_x/y/z` (D12), later `cst_sp_*` (D13). Each file also holds a
  `meta` TNamed with JSON provenance (run list, event counts, skipped CJC-off events, sha256 of
  `h1hits.py` and of each `consts_<run>.json`, probe paths, NDB path, date). Raw CJC words follow
  the MC convention `cjc_drift_raw = DL*0.001` (`extract_hits.C:159`); in data `DL` is time in
  FADC ticks (1/50 clock, `$R/calib_consts/py/h1hits.py:7`), documented in WP6.
- **D8 Writer technology.** PyROOT in the LCG environment after `thish1.sh` (Python 3.11.9, ROOT
  6.34.04, uproot 5.3.11 and numpy present; verified on 2026-10-27) with `std::vector<float|int>`
  branches, so `TTree::Print()` leaf types match MC. Never `python3 -I` for the writer: it drops the
  LCG `PYTHONPATH` (`$H/HANDOFF.md:234-235`); the probe's stdlib-only scripts keep their `-I`.
- **D9 WP2 reuses the probes unchanged.** The job runs `calibprobe`, `make_consts.py`, `cjcprobe`
  and `dataprobe` exactly as `$R/lam/job.sh` and `$R/scale/job.sh` do, then the WP1 writer, so WP2
  changes no C++ and the text formats stay those WP1 was validated on. `dataprobe.txt` (raw `SIFE`,
  `CRM2`, `CRME`, `CRZE`) is kept per file so WP4 and WP5 are post-processing plus one extra probe
  step, not a re-extraction. Text dumps are kept (about 85 GB, D3); if DUST space runs short,
  `events.txt` and `cjcprobe.txt` may be deleted after a file's validation passes, since
  re-extraction costs about 6 min per file.
- **D10 CJC-off events.** Skip events with zero CRJE hits and count them in `meta`. Run 492559 is
  CJC-off per event, not per run (1,316 of 1,501 events empty, 185 with hits), so no run veto.
- **D11 Event T0.** `cjavt0(T0track, |DT0|)` over the event's CJKR rows (cols 27-28 of
  `events.txt` CJKR lines); with no track, the run default `ARC0J8(1)` from the consts json and
  `cjc_t0_src = 1`. Because `cjavt0` averages the H1 track T0s, the default coordinates are
  track-free in position only (research §7, circularity); forcing the run T0 for every event would
  cost 668 versus 634 µm (README:132-137). Exact CJCC T0 would need a CJCREC rerun (research B2);
  not done.
- **D12 COZ.** `coz_wire/drift/qplus/qminus` from raw CRZE words for all hits; h1rec `CZR2`
  coordinates to `coz_trk_x/y/z` for on-track hits. Track-free COZ φ (`COZ.phi_chargediv`) stays
  unvalidated and off (research B4).
- **D13 CST (WP4).** Data CST enters as h1rec space points (`cstprobe` chain CSLIGEOM, CSCLUS2,
  CSFILL2 on raw SIFE) in new `cst_sp_*` branches; `csts_strip/csts_adc` stay empty because the
  MC decoder (`$H/decoders/h1_digi_decode.py:114+`, MC geometry, `CST_PN_RATIO` fitted on MC) is
  not valid on data (research §4). The converter then needs to read `cst_sp_*` (user action, Q5).
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
  `IVRSQT = 2` (`$R/calib_consts/calrun.F:13-15`, h1rec `ct_cjcrec/cjrunc.F:465-467`), which is
  the first code path not yet exercised.
- **D16 Validation thresholds.** CJC: the `h1hits.py` regression (`validate_cjc.py`) must
  reproduce 0.944 within 10 µm; the track-free nearest mirror versus `CJXR` on lam/ must give
  median |Δr-φ| < 0.5 mm and 95% < 2 mm (σ about 634 µm, README:132-137); z residual is reported
  with a provisional median < 1 cm. Per new period (WP3): one file with the full dump must give
  ≥ 0.90 within 10 µm on the exact path (same test as 2007). CST (WP4): DCLH cluster match ≥ 99%,
  |Δz| versus CSHY 90% < 0.5 mm, nearest point to CJKR tracks median < 0.4 mm (measured: 100%,
  91%, 253 µm on 121 events). CIP/COP (WP5): thresholds set in its research stage.

## Outline

Pointers to follow; signatures are the contract, bodies are executor's.

`$D/writer/write_hits_data.py` (new, about 450 lines)
- `read_events(path) -> iter[EventDump]`: parse `events.txt` line types `EV`, `#`, `CJKR/CJKM/CJKT/CJKV i <int32 words>`
  (floats are bit patterns, reinterpret as `$R/calib_consts/py/events.py` `flt()` does), `H i wire dl q+ q- f1 f2`,
  `XR ichan itr kdr DRFM XHIT YHIT ZHIT WR WZ QHIT DRFE TLEN COSB SINB SLEN ZEXP DCOR ZCOR BZ`
  (`$R/calib_consts/README.md:17`; `kdr` is the 1-based `H` row), `CRZE`, `CZR2`. Reuse
  `events.read_events` where it fits; the writer must not edit `$R`.
- `read_cjcprobe(path) -> iter[(run, evt, raw_hits, trk, crze)]`: `EV`, `TRK`, `RAW CRZE`, `HIT wire dl qplus qminus flag1 flag2`
  (`$R/cjcprobe/cjcprobe.C:30-70`); `HIT` lines belong to the preceding `EV`, banks concatenated.
- `join(events, cjc) -> iter[Event]`: key `(run, evt)`; both dumps are in mODS order with equal
  `EV` counts (1,501 per lam dir). Invariant: every `XR` row maps to exactly one raw hit through
  `kdr -> H row -> (wire, dl)` after `CJC.qht8(row)` (`h1hits.py:40`); unmapped rows are an error.
- `coords(ev, cjc: h1hits.CJC) -> dict of vectors`: `CJC.prepare_event(raw_hits)` (`:354`), T0 per D11
  (`cjavt0` at `:397`), `CJC.simple(hit, t0, side=±1)` (`:375`) for the two mirrors (`drd_final` is
  in 10 µm; cm = ×1e-3), `CJC.cell(ich)` (`:121`) for ring/layer/cell, wire x/y from `cp_x/cp_y`
  in the consts json. `cjc_reco_trk` = 0-based index of `XR.itr` into the event's CJKR list,
  `cjc_reco_side` from the sign of `DRFM` (executor confirms the sign convention against
  `extract_hits.C:171` and README:17).
- `fill_tree(tree, vectors)`: branch declarations mirror `extract_hits.C:121-247` one-to-one (names,
  `std::vector<float>` versus `std::vector<int>`, scalar Int_t for `run`, `event`, `n_str_all`,
  `n_svx_all`); plus the D7 additions and the `meta` TNamed.
- CLI: `write_hits_data.py --lam-dir <dir> --consts-dir <dir> --out <hits_part_X.root> [--max-events N]`;
  the consts json is chosen per event by run (1-3 runs per lam dir).
- Gotchas: `python3`, not `python3 -I` (D8); `cjkr_*` column mapping must follow the CJKR bank
  layout used by `extract_hits.C:189-198` (28 raw words per row in `events.txt`; `cjkr_cov9`
  only if the words exist, else empty); COZ `CRZE` rows are six unsigned words
  (`events.txt` `CRZE i <6>`); a lam dir can hold `EV` lines with no `HIT` lines (D10).

`$D/validation/check_schema.py` (new, about 80 lines)
- `compare(data_file, ref_file, allowlist) -> report`: branch name and leaf type sets via uproot
  (`library="np"` as `$H/decoders/validate_decoders.py:36`); PASS when the MC set is a subset with
  identical types and every extra branch is in the allowlist; prints per-branch non-empty fractions.

`$D/validation/validate_data_cjc.py` (new, about 120 lines)
- For hits with an `XR` row: nearest track-free mirror versus `(XHIT, YHIT)`; `cjc_trk_z` versus
  `cjc_zcd`; prints median, 68%, 95% quantiles and the D16 verdict; JSON output like
  `validate_decoders.py` (`validation.json`). Reads the written `hits` file, not the dumps, so it
  also exercises the tree.

`$D/condor/job_realdata.sh`, `$D/condor/realdata_2007ep.sub` (new, WP2)
- Pattern: `$R/lam/job.sh` (env: `desygroupset h1`, `thish1.sh`, bash; writes `s.steer` with
  `fHatFiles`/`fModsFiles`/`fHatSelection="Q2e>5"`; `calibprobe -f s.steer -n N`; `make_consts.py`;
  `cjcprobe -f s.steer -n N`) plus the `dataprobe` step of `$R/scale/job.sh`, then the writer, then a
  `DONE` marker. Submit pattern: `$R/lam/job.submit` (`+RequestRuntime`, `request_memory = 3000`,
  `requirements = OpSysAndVer == "RedHat9"`, `queue f in (...)`) and the workspace style
  `universe = vanilla`, `RequestCpus = 1` (`$W/lambda/scripts/xqz3d/*.sub`). Output layout:
  `$H/production/realdata_2007ep/q2e5_n5000/{dumps/<file>/, hits/hits_part_<file>.root, logs/}`.
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
WP2 (disjoint files; WP3 never replaces `$R/calib_consts/bin/calibprobe`).

1. **WP1: lam/ to `hits` tree.** Delivers 38 `hits_part_<file>.root` under
   `$H/production/realdata_2007ep/lam_q2e5/hits/` plus schema and h1rec validation reports.
   Units: (a) writer, (b) `check_schema.py`, (c) `validate_data_cjc.py` (disjoint files).
   Tests: `schema_parity: hits_part_c070060.root vs cov_loQ2/loQ2/hits/hits_part_0.root -> PASS, extras ⊆ allowlist`;
   `event_count: c070060 -> 1501 minus skipped empty events, skipped count in meta`;
   `xr_mapping: every XR row of c070060 -> exactly one raw hit (0 unmapped)`;
   `trackfree_vs_cjxr: lam/ all 38 -> median |Δr-φ| < 0.5 mm, 95% < 2 mm`;
   `h1hits_regression: python3 -I py/validate_cjc.py . in $R/calib_consts -> 0.944 within 10 µm`;
   `cjc_off: c070000 -> 1316 events skipped, 185 written`;
   `truth_empty: any file -> all (f)-group branches empty, n_str_all == 0, is_data == 1`.
   Checkpoint: one lam dir on the login node (one file, under 5 min), then the 38 dirs as one
   Condor job array (`$D/condor/lam_convert.sub`), then `check_schema.py` and
   `validate_data_cjc.py` over the 38 outputs.
2. **WP2: Condor extraction, 2007 e+p.** Delivers `$H/production/realdata_2007ep/q2e5_n5000/`
   with 112 `hits_part_<file>.root`, dumps and logs. Units: (a) `job_realdata.sh`, (b) `.sub`
   and layout, (c) in-job validation call (reuses WP1 scripts).
   Tests: `smoke: c070060, -n 300 on Condor -> DONE, hits file with 300 minus skipped events, schema PASS`;
   `multi_run: a file whose first 5000 events span 2+ runs -> per-event consts selection, meta lists all runs`;
   `production: 112 files -> 112 DONE, sum of events in meta about 560k, 0 jobs over runtime`;
   `validation_sample: validate_data_cjc.py on every 10th file -> D16 thresholds`.
   Checkpoint: `condor_submit $D/condor/realdata_2007ep.sub`, then `ls .../hits | wc -l` = 112 and the
   validation JSONs.
3. **WP3: constants for 2005, 2006, 07low/07med.** Delivers `$D/consts/<period>/consts_<run>.json`
   for every run met in the mODS files of each period, `bankdiff.txt` per period, and one exact-path
   validation per period. Units: (a) `calibprobe.C` flag and the `calibprobe_consts` build target,
   (b) `consts_<period>.sub`, (c) diff script.
   Tests: `consts_only_mode: calibprobe_consts on c070060 -n 1500 -> consts_497718.json and consts_497719.json byte-identical to $R/lam/c070060/calib/`;
   `mirror_05: $R/mirror/acs/data/05/dst7 -> IndexDST resolves for c050000 (CALRUN ier 0)`;
   `ivrsqt2_path: one 2005 run -> CALRUN ier 0, consts json has the cjc and coz blocks`;
   `period_validation: one file per period, full dump -n 1500 -> ≥ 0.90 within 10 µm vs CJXR`;
   `bankdiff: per period -> list of varying banks; C0J8 Lorentz word within 0.60-0.64 rad`.
   Checkpoint: per period, `ls $D/consts/<period>/consts_*.json | wc -l` versus distinct runs in the
   period's mODS files, and the validation JSON.
4. **WP4: CST.** Delivers `cstprobe` in the WP2 job, `cst_sp_*` branches and the CST validation.
   Tests (from D16): `dclh_match ≥ 99%`, `dz_vs_cshy 90% < 0.5 mm`, `rphi_nearest median < 0.4 mm` on
   ≥ 1,000 events; `schema: cst_sp_* in allowlist, csts_* empty`. Checkpoint: child plan's.
5. **WP5: CIP/COP.** Delivers the data pad geometry decision (CGAM equality or ported geometry),
   `cip_pad/cop_pad` filled, validation against CJKR extrapolation. Tests set in its research stage
   (candidate: fraction of tracks with pt > 0.3 GeV having a pad within one pitch in ≥ 3 of 5 CIP
   layers). Checkpoint: child plan's.
6. **WP6: provenance and handoff.** Delivers `$D/docs/README.md`, a data section in `$H/HANDOFF.md`
   and a research-artifact erratum. Tests: `paths: every path in the docs exists`;
   `meta_consistency: run lists in meta == consts json present`. Checkpoint: `critic` read of the
   docs plus a path check script.

## Verification

After WP1-WP2 (first release): `check_schema.py` PASS on every output file; `validate_data_cjc.py`
on the WP2 validation sample within D16; `meta` totals equal the file count and events written; a
sample file reads with the `uproot` snippet in `$H/HANDOFF.md:216-220` with the same code as an MC
file. WP3-WP5 add their own checks; WP6 closes with the documentation checks.

## Risks

- **Inferred converter schema.** `h1_to_fm4npp.py` is on LCRC only; which branches it needs, and
  whether it tolerates empty truth vectors or extra branches, is unknown (research B9). Mitigation:
  exact MC branch set, additive extras, Q1.
- **Fragile probe workarounds.** Patched `CJXREF` work bank (`MACC=200*NRCJKR+2000`,
  `$R/calib_consts/build_lib.sh`), skipped `CJCRJE`, private `GUFLD` (`gufldp.F`, 11.608 kG) per
  `$R/calib_consts/README.md:148-162`. A rebuild of `libcjcprobe.so` or an H1OO release change can
  break them silently; WP6 records the library hashes, WP1's `h1hits_regression` test runs first.
- **Validation sample mismatch.** The 0.26 µm / 94% figures come from `c070060`, `Q2e>150`, runs
  497718-497719, 89 events, exact path with CJXR track inputs, not from `lam/` or the track-free
  path. The track-free default input has about 634 µm r-φ, not 262 µm (D5, D16).
- **CJC-off and HV-off events.** Run 492559 has 1,316 of 1,501 events empty; no local good-run or
  HV list exists (only `CJAC`/`SLOWSTA1` in-process). D10 skips empty events; partial HV states are
  not detected.
- **Unvalidated periods.** 2005 and early 2006 take `IVRSQT = 2`; `C3Z8` can be missing per run
  (research §3); `mirror/acs/data/05` does not exist yet; dst7/gmcut tape locality was not checked.
- **CRME absence.** 0 `RAW CRME` lines in 120 dumped 2007 events (`$R/data_display/dataprobe.txt`),
  so COP may be absent from the raw stream or not read; WP5 research settles it.
- **No SIFE in lam/.** The lam dumps hold only `calib/` and `cjc/`; CST for the lam sample needs a
  `cstprobe` pass (WP4), not a re-read of lam/.
- **Volume.** Dumps are about 150 KB per event, so the first release needs about 100 GB of DUST
  including the `hits` trees (D3, D9).
- **Python environment.** `python3 -I` drops the LCG `PYTHONPATH` (`$H/HANDOFF.md:235`); the writer
  and the probe scripts need different invocations in one job.
- **Research-artifact errata** to carry into WP6: `CRPE/CRTE` are not banks (D14); `H1CipCopGeo.h`
  is in release 4.1.1, not 4.2.1; `calrun.F:13-15` is the probe's copy (h1rec: `cjrunc.F:465-467`);
  the lam sample is the first 1,501 `Q2e>5` events per file, every third file, so "70 runs" are a
  sample of 2007, not all its runs.

## Scope Notes

In scope: WP1-WP6 as above. Out of scope: changes to the simulation chain or `extract_hits.C`;
the converter (LCRC); forward tracker, calorimeters; 2003-2004, HERA-I; exact CJCC event T0
(CJCREC rerun); track-dependent CJC corrections (`CJHLXE/CJHLXH` terms, README:141-147); moving
`$R` into the workspace. No user decisions are recorded in memory for this topic; D1-D5 are
supervisor defaults until the user confirms them.

## Assumptions

- `calibprobe`, `cjcprobe`, `dataprobe` and `cstprobe` binaries in `$R` still run against the
  current H1OO 4.2.1 release; checked by WP1's regression test and WP2's smoke job.
- The 2007 dst7/gmcut files referenced by the 112 mODS `IndexDST` are online; checked by the
  smoke job's `CALRUN ier` and by Condor job exit codes.
- The AFS NDB (`/afs/desy.de/group/h1/ndb_read_only_dp`) stays readable; checked at every job start
  (job.log line "H1 Master Database").
- About 100 GB free on DUST under `$H/production/`; checked with `df` before WP2 submission.

## Work Packages

### WP1: lam/ to hits tree and schema parity
- Goal: convert the 38 `$R/lam/` dumps into `hits` trees; done when `check_schema.py` passes on all
  38 files against `$H/production/cov_loQ2/loQ2/hits/hits_part_0.root` and the Slice 1 tests hold.
- Owns: `$D/writer/`, `$D/validation/`, `$D/condor/lam_convert.sub`,
  `$H/production/realdata_2007ep/lam_q2e5/`
- Deps: none
- Stages: execute
- Status: execute
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
- Status: execute
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
  and the `$H/HANDOFF.md` data section exist, every cited path resolves, and `meta` run lists match
  the consts present.
- Owns: `$D/docs/`, the data section of `$H/HANDOFF.md`, an erratum note appended to the research
  artifact
- Deps: WP1, WP2
- Stages: write
- Status: blocked: write: WP1 and WP2 outputs are its subject
- Artifacts: ->

## Open Questions

Each with the recommended answer; D1-D5 defaults stand until the user answers.

- **Q1** Can `h1_to_fm4npp.py` be copied to NAF (or its branch list shared), so `check_schema.py`
  can assert the real requirement instead of MC parity? Recommended: copy it; until then D1.
- **Q2** Is pretraining self-supervised, or does it consume labels? Recommended: self-supervised;
  pseudo-labels stay optional (D2).
- **Q3** Include 07low/07med (460/575 GeV) in WP3? Recommended: yes for pretraining; drop if the
  inputs must match the 319 GeV analysis.
- **Q4** First-release size: 5,000 events per file (about 560k) as in D3, a stride within each
  file (`fHatSelection = "Q2e>5 && EventNumber%29==0"`; HAT has `RunNumber`, `EventNumber`, `Q2e`),
  or more? Recommended: D3, then scale by `-n`.
- **Q5** For CST, will the converter be changed to read `cst_sp_*` point branches (D13)? If not,
  WP4 must instead produce data strips in CSTS packing and accept the MC decoder's known bias.

## Log

(empty; `fixer` appends lead reports and `decision:` lines)

## Todo

- [ ] WP1 writer: `read_events`, `read_cjcprobe`, `join`, `coords`, `fill_tree`, CLI, `meta`
- [ ] WP1 `check_schema.py` with the D7 allowlist; run against `cov_loQ2/loQ2/hits/hits_part_0.root`
- [ ] WP1 `validate_data_cjc.py`; run `h1hits_regression` first
- [ ] WP1 login-node run on one lam dir; Condor array over 38; the seven Slice 1 tests
- [ ] WP2 `job_realdata.sh` (probes + dataprobe + writer) and `.sub`
- [ ] WP2 smoke `c070060 -n 300`; `df` check; production 112 files; validation every 10th file
- [ ] WP3 `CONSTS_ONLY` flag in `calibprobe.C`, built as `bin/calibprobe_consts`; byte-identity test on c070060
- [ ] WP3 mirror symlink 05; per-period submit; bank diff; one full-dump validation per period
- [ ] WP4 research and child plan (SIFE, cstprobe in job, `cst_sp_*`, D16 CST tests)
- [ ] WP5 research (data CGAM versus MC; CRME presence) and child plan
- [ ] WP6 `$D/docs/README.md`, `$H/HANDOFF.md` data section, research-artifact erratum, hash and
      path checks

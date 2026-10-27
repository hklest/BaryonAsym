# Research: real H1 HERA-II data for FM4NPP pretraining — blockers and verdicts

Date: 2026-10-27. Research only; no code, no Condor, no production.

**Scope.** What blocks feeding real H1 data into the FM4NPP pretraining inputs, which so far come only
from H1 GEANT3 simulation. For each blocker: can it be resolved from NAF, and how. Covers the sim
pipeline and its features, calibration, geometry, and data availability.

Labels: **[V]** verified here (file:line, command output, or a worker's recorded check); **[I]** inference.
Abbreviations: `$H` = `/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim`,
`$R` = `/data/dust/group/h1/klesthen/rawhit_probe`, `$P` = `/pnfs/desy.de/dphep/online/h1/data`,
h1rec source = `/data/dust/group/h1/sschmitt/h1soft/h1rec` (identical to the 4.2.1 build tree
`/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1rec/`, per `$R/calib_consts/README.md:7`).

## Headline

The recorded blocker ("calibration and geometry for data are in run-database banks that are not in
the event record") is **already resolved for the central jet chamber (CJC), and for COZ on-track hits, in 2007 e+p data**.
The work was done in a separate probe, `$R`, after the FM4NPP handoff was written. That probe reads
h1rec's own constants from the H1 NDB per run, re-implements the CJC hit formula in Python, and
matches h1rec to well under 10 µm for 94% of hits. CST space points come from h1rec routines run on
raw data. The probe has not yet been turned into a data-prep pipeline. The gaps that remain are
engineering work: other periods (2005, 2006, 07low/07med), the CIP/COP decoders for data, a
`hits`-tree-compatible data writer, and the FM4NPP converter, which is on LCRC only. Real data has
no truth, which is a fundamental limit; whether it matters depends on how pretraining is labelled.

## 1. Existing FM4NPP work (simulation)

- **Location [V].** `$H` (inside this workspace but not listed in `AGENTS.md`). Handoff:
  `$H/HANDOFF.md` ("H1 hit-level simulation for FM4NPP", maintainer H. Klest, as of 2026-10-21);
  sample catalogue: `$H/README_SHERPA_HITS.md`.
- **Chain [V]** (`$H/HANDOFF.md:57-74`): Sherpa 3.0.3 → `h1hepevt` (BOS) → official preprocessing
  (real 2006–07 e+ run numbers, vertices, random-trigger overlay) → patched `h1simrec9` (GEANT3,
  digitisation, full reco; the DST keeps every BOS bank) → `dst2ods -f hit_banks.steer` (adds
  CRJT, CRJE, CRJX, CTJX, CJKR, CJKT, CJKV, CRVT, CSTS, CRZT, CRZE) → `run_extract.C`/`extract_hits.C`
  → flat ROOT tree `hits`, one entry per event, `hits_part_N.root`.
- **Data level read:** BOS banks inside the simrec DST, exported via ODS. No HAT/mODS physics objects
  are used for hits.
- **Converter [V/I].** `h1_to_fm4npp.py` ("hits trees → FM4NPP training format, one point per CJC
  mirror candidate, plus the other detectors", `$H/README_SHERPA_HITS.md:159`) and the geometry banks
  it uses live only on LCRC at `/lcrc/project/eic/henry/H1Tracking/data/fm4npp/`
  (`README_SHERPA_HITS.md:41-47`). NAF cannot reach LCRC, and no copy exists on NAF (find, maxdepth 6,
  under `/data/dust/group/h1/klesthen`). The exact FM4NPP input schema is therefore **inferred**: the
  CJC mirror points `cjc_xp/yp`, `cjc_xm/ym` with z `cjc_zcd`, plus decoded CST/COZ/CIP/COP points.
- **Samples [V]:** catalogue in `README_SHERPA_HITS.md:25-39`.

### Features and how MC computes them

| Detector | Raw bank → stored branches | MC coordinate method | MC-only inputs |
|---|---|---|---|
| CJC | CRJE → `cjc_wire, ring, cell, layer, drift_raw, qplus, qminus`; derived `cjc_drift, cjc_zcd, cjc_wire_x/y, cjc_xp/yp, cjc_xm/ym` | linear fit to GEANT truth: `drift = (DL·0.001 − 0.4987)/1.0465`, `z = −2.38 + 196.04·(Q+−Q−)/(Q++Q−)` (`extract_hits.C:43-44,304`); wire table re-implements the **MC branch** of `cjgeom.F` (Lorentz 0.635, stagger ±150 µm, no ring rotation, no sag; `extract_hits.C:16-18,47-57`) | constants fitted on truth; MC geometry branch; no event T0 |
| COZ | CRZE → `coz_wire, drift, qplus, qminus` | CGAZ/CDCZ formulas (`decoders/h1_digi_decode.py:184-209`) | MC NDB dump |
| CST | CSTS → `csts_strip, csts_adc` | strips → clusters → p/n match, banks CGL6/CGSN/CGPH/CMVS (`h1_digi_decode.py:296-433`); `CST_PN_RATIO=1.55` self-calibrated on MC | MC geometry bank set; MC has no CST noise |
| CIP2k / COP | CRM2 / CRME → `cip_pad`, `cop_pad` | pad → point via CGAM (`h1_digi_decode.py:68-154`) | CGAM from the MC stream |
| Forward (FTD/FST) | — | not extracted in any sample (`README_SHERPA_HITS.md:210`) | — |

MC geometry is hard-coded to the MC stream [V]: `decoders/dbdump/geometry_banks_mc0607.txt` (7 banks,
run 469682, stream MC; `dbdump.C:15,22`), with `runs.C:6` listing 27 MC runs 468608–490365.

Truth branches with no data analogue [V]: `str_*`, `svx_*`, `crjt_*`, `cjc_truth_*`, `crvt_*`,
`crzt_*`, `crmt_*`, `coz_str`, `csts_crvt`. Branches that do exist for data: `cjc_reco_trk`,
`cjc_reco_side`, `cjkr_*`, `cjkt_*`, `cjkv_*` (H1's reconstruction).

## 2. Where the blockers are recorded

- `$H/HANDOFF.md:187-188` [V]: "Drift and z use linear MC calibrations fitted to GEANT truth, because
  h1rec takes them from run database banks that are not in the event record. Raw values are stored,
  so these can be redone."
- `$H/README.md:86-87` [V] names the banks: "per-run NDB banks (CJCC, C2J8) that are not in the event
  record".
- `$R/AGENTS.md:54` [V]: "Raw `CRJE` is drift *time* and charge, not coordinates. Converting it needs
  geometry, calibration, the event t0, and the drift sign (left/right ambiguity)."
- `$R/AGENTS.md:51` [V]: not on DST: `CJCC` (event t0), `CSKR`, `FTKR`, `CJXR`.
- `$R/AGENTS.md:73` [V]: hits are not stored in the standard MC tiers. This is why the sim chain
  exists.
- No memory entry in `.supervisor/memory/` covers FM4NPP or raw hits [V, grep].

**What simulation supplied that data lacked:** (a) GEANT truth to fit drift and z constants, and the
MC-branch CJC geometry; (b) an MC NDB stream for the CST/COZ/CIP/COP decoders; (c) truth labels and a
truth-nearest left/right choice; (d) no need for an event T0 (MC drift is a length, `$R/AGENTS.md:73`).

## 3. Calibration for real data

| Detector | Data needs | NDB banks (data) | Status |
|---|---|---|---|
| CJC | wire t0, global T0, drift velocity, Lorentz angle, isochrones, charge-division gain and effective length, survey, z corrections | C0J8, C1J8, C2J8, CJCV, CJLV, CJLL, CJLP, CJLC, CJBR, CJIM, CJPC, CZDD…CZSD (43 banks in `raw_ndb_banks`) | **Solved, validated** [V]: `$R/calib_consts/py/h1hits.py` `CJC.hit3d()` is an exact CJFH3D. Against h1rec `CJXR`, 54,062 hits: median Δxy 0.26 µm, 94% < 10 µm, z median 12 µm (`README.md:123-140`) |
| CJC event T0 | per-event T0 (CJCC, not on DST) | — | **Approximated** [V]: `cjavt0()` on DST `CJKR` T0track; r-φ to the CJKR circle 634 µm, versus 668 µm with one run T0 and 262 µm with h1rec's track-dependent terms (`README.md:91-98,135`) |
| COZ | T0, drift velocity, isochrone tables | C0Z8, C1Z8, C3Z6/C3Z7, C4Z8, COZG, CGAZ | **Solved for on-track hits** [V]: drift time and ellipse match h1rec `CZR2` to 7e-4 ns on all 3556 hits (`README.md:108-138`). On data, COZ φ comes from the CJC track (`README.md:120`). Track-free φ via `COZ.phi_chargediv` is coded but unvalidated |
| CST | strip masks (dead/noisy), cluster cuts | CSMK, CSSL, CSST (+ geometry, §4) | **Solved by calling h1rec** [V]: `$R/cst_data/cstprobe.C` runs CSLIGEOM, CSCLUS2, CSFILL2 on raw SIFE; checked against DST CVLH; no quantitative validation table |
| CIP2k / COP | pad maps, dead channels | — | **Not done** [V]: raw banks are dumped (`$R/data_display/dataprobe.C:62`), but there is no data decoder |
| FTD/FST | — | FPG1/FQG1/FPLC/FQLC/FGAP/FGAQ (h1rec `ft_*`) | **Not done**. Absent from the MC samples too |
| B field | solenoid field map | FIEL × FIELFUD | **Solved** [V]: `$R/calib_consts/gufldp.F`, 11.608 kG. The H1OO process otherwise finds a dummy GUFLD that returns 0 |
| LAr/SpaCal | — | — | **Not used** by the current FM4NPP inputs [V]: the sim chain extracts central-tracker banks only (`README_SHERPA_HITS.md:210`). Calorimeter calibration level at HAT/DST not examined |

Dead or bad CJC hits are removed by h1rec `CJRUNC` and CJKBCR inside the probe chain [V,
`$R/AGENTS.md:60`]. The CST masks are applied but not exported.

**Run dependence [V].** `$R/scale/` holds 12 2007 mODS files (c070005…c070110, all with `DONE`) and
27 distinct per-run constant dumps (`scale/*/calib/consts_<run>.json`, runs 492937–500595). Diffing
`raw_ndb_banks` across the 27 runs, only C0J8 (including the Lorentz-angle word, 0.605–0.637 rad),
C0Z8 (T0, −25.0 to −29.5), COZG (COZ drift velocity per wire), CJAC (HV monitoring), CSF8, and C3Z8
(absent for run 497959, so it can be missing per run) differ.
All other banks are bit-identical. This corrects `calib_consts/README.md:13`, whose "only dE/dx gains
differ" holds only for 497718 versus 497719. `$R/lam/` holds 47 runs (38 mODS files, Q2e>5); the union with `scale/` and `calib_consts/` is 70 runs.
`calrun.F:13-15` branches at NCCRUN 281276 and 452777. The second cut falls inside HERA-II, so 2005 and
early-2006 runs take IVRSQT=2 rather than 3 [I, from H1 run-number ranges].

**NDB access [V].** `/afs/desy.de/group/h1/ndb_read_only_dp` → `dphep/FilesForValidation/ndb_read_only`
(FPACK, 4.26 GB, 2015-10-08). Probe logs show it opened on 2026-10-22
(`$R/scale/c070055/calib/job.log`). A second copy,
`/data/dust/group/h1/ndb_read_only_gz-141014` (FPACK despite the name, 4.25 GB, 2014-10-14,
owner sschmitt), is older and differs in its header. It is unvalidated as a fallback. H1OO has no
dedicated NDB C++ accessor in the release tree that was searched. Access is through the lazily loaded
`libh1ndbOO.so` plus h1rec Fortran (`UGTRUN`, `calrun.F:25`), linked with `-Wl,-u,getpidf_`.

## 4. Geometry for real data

- **CJC [V].** `ct_cjcrec/cjgeom.F` branches on IRMC (`:70`). The data branch uses the stagger
  0.0095/0.012 (CJC1) and 0.0115/0.013 (CJC2) (`:102-105`), ring rotations `ARC0J8(18/19)` and the
  Lorentz angle `ARC0J8(6) ∓ ARC0J8(30)` (`:139-143`), plus CJPC survey, CJAW and CJCW. Data values
  for 2007 are in `consts_<run>.json`: Lorentz angle 0.627/0.622 rad versus MC 0.635, ring rotation
  −0.0123°/+0.0223°, sag and twist. **The `extract_hits.C` geometry is MC-only and must not be used
  on data.** `h1hits.py` with the data JSON is the data path.
- **COZ [V].** COZINT builds COZG from CGAZ + C4Z8. The CGAZ words for MC run 469682 and data run
  497719 are identical (0 differences in 51 words). [I] The MC decoder's COZ geometry is therefore
  usable on data, with data C0Z8/C1Z8 drift constants.
- **CST [V].** Data uses the run-keyed banks CVSD/CVSA/CVLA2/CVNG2, re-read on run change
  (`ct_cstrec/csligeom.F:147-156`, `cstprobe.C:75`). MC uses a different bank set, CGL6/CGSN via
  h1sim `csrdcgsa.F`. HERA-II layers are off-centre (layer 1 r 3.6–10 cm, layer 2 r 7.8–13.6 cm;
  `cst_data/README.md`). [I] The MC CST decoder is not known to be equivalent; use h1rec on data.
- **CIP2k / COP [V].** Data raw banks are CRPE/CRTE (plus CRM2/CRME, also dumped). No data geometry
  path has been written. Candidates: `H1Geom/H1CipCopGeo.h` in the 4.2.1 release and CGAM read on a
  data run.
- **H1OO geometry classes [V]:** `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Geom/`
  (`H1Align*`, `H1CipCopGeo`, `H1FstGeometry`, `H1Bst*Geometry`, `H1MeanVertex`). The brief's
  `BaryonAsym/h1oo-master/` does not exist; only `BaryonAsym/h1oo-build/` does [V, ls].
- **Period dependence:** untested. Only 2007 e+p runs have been dumped.

## 5. Data availability on NAF

Chain [V] (`$R/AGENTS.md:14-31`): mODS/HAT (ROOT) → `IndexDST` → DST7 (FPACK) → origin pointer →
H1GMCUT raw (FPACK). The mODS `IndexDST` stores `/acs/...`, which needs one symlink per year in
`$R/mirror/acs/data/YY/dst7`. Only 06 and 07 exist [V].

| Year | hat/mods n | mods GB | dst7 n / GB | gmcut n / TB | Periods (INDEX names) |
|---|---|---|---|---|---|
| 03 | 20 | 29 | 198 / 315 | 916 / 1.46 | P2 |
| 04 | 63 | 104 | 626 / 994 | 2754 / 4.41 | E4, P1–P3 |
| 05 | 152 | 241 | 1518 / 2424 | 7159 / 11.45 | E1–E4 |
| 06 | 313 | 452 | 3128 / 5001 | 12849 / 20.55 | E1–E2, P1–P3 |
| 07 | 112 | 155 | 3485 / 5570 | 13483 / 21.57 | dst7: HE, ME, L1, L2; gmcut: L1–L3, P1 |
| 07low / 07med | 170 / 69 | 188 / 75 | in `07/dst7` [I] | in `07/gmcut` [I] | — |

HAT/mODS sit under `$P/oo-4.0/YY/{hat,mods}.4.0.6.10dst7.c0YNNNN.root`, DST7 under
`$P/YY/dst7/CDST7.C0YNNNNN`, raw under `$P/YY/gmcut/NN/H1GMCUT.C0YNNNNN`. HERA-I oo-4.0 (96–00)
also exists. [V] dCache locality of `$P/oo-4.0/05/mods...c050000.root` is ONLINE (dst7 and gmcut
were not checked). One-file check: `$P/oo-4.0/05/hat.4.0.6.10dst7.c050000.root` has tree `HAT` with
1,060,604 entries; `$P/05/dst7/` and `$P/05/gmcut/00/` exist. [V] `07/dst7` index names are HE, ME, L1, L2 (`ls $P/07/dst7`). [I] These are the high, medium and low
proton-energy periods, so the 07low/07med DSTs sit in `07/dst7`; E/P mean e−/e+. dst7 counts include a few `*.INDEX` files.

**Existing real-data hit extractions [V]** (all 2007, text dumps, none in `hits`-tree format):

| Dir | Content | Scale |
|---|---|---|
| `$R/lam/` | per file: DST CJKR/CJKM/CJKT/CJKV, h1rec CJXR, per-run constants, raw CRJE/CRZE (`cjc/cjcprobe.txt`) | 38 files, Q2e>5, 57,038 events, 47 runs, 6.5 GB, all DONE |
| `$R/scale/` | constants + CJXR, CST clusters/points, raw CRZE/CRM2/SIFE/CRJE | 12 files × 400 events (Q2e>150), 837 MB, all DONE |
| `$R/data_display/`, `$R/cst_data/` | 120-event raw and CST dumps | validation |
| `$R/dstprobe/`, `$R/lamprobe/` | DST tracks/V0s for the Lambda study | not hit-level |

**Throughput [V/I].** About 0.07 s per event on Condor (`$R/lam/logs/condor.log`). Select events via
the HAT; full-year raw scans (11–22 TB per year) are not sensible.

## 6. Blocker verdicts

| # | Blocker | Verdict | Mechanism / steps | Effort [I] |
|---|---|---|---|---|
| B1 | CJC drift and z calibration (NDB banks not in event record) | **Resolved** for 2007 e+p (70 distinct runs dumped across `scale/`, `lam/`, `calib_consts/`; the bank diff covers the 27 `scale/` runs); **resolvable** for 05/06/07low/07med | `calibprobe` dumps the constants per run from the NDB; `h1hits.py` applies them. For other periods, add the `mirror` symlinks for 03–05, run one calib job per run, and check the NCCRUN (IVRSQT) code path and the C0J8 diff | Small: one Condor wave plus a diff |
| B2 | CJC event T0 (CJCC not on DST) | **Resolvable with an approximation** | `cjavt0()` on DST T0track (634 µm), or one run T0 (668 µm) to avoid inheriting H1 track fits | None for an approximation; exact needs a CJCREC rerun |
| B3 | CJC geometry | **Resolved** (2007) | data branch of `cjgeom.F` via the data JSON; drop the MC geometry in `extract_hits.C` for data | Small |
| B4 | COZ calibration + geometry | **Resolved for on-track hits** (2007); track-free φ **resolvable** | `h1hits.py` `COZ.hit()`; CGAZ same as MC. Validate `COZ.phi_chargediv` against CJC tracks | Small |
| B5 | CST geometry/alignment + masks | **Resolved by calling h1rec**, lightly validated | `cstprobe.C` (CSLIGEOM/CSCLUS2/CSFILL2). Steps: a residual check against CJKR tracks; compare with the MC decoder output; data noise is real (MC has none) | Medium-small |
| B6 | CIP2k / COP data decoder | **Resolvable** | data pad geometry from CGAM on a data run or `H1CipCopGeo`; validate pads against CJC track extrapolation | Medium |
| B7 | Forward tracker | Not a blocker for parity: the MC samples lack it too | only if FM4NPP wants it | Medium–large |
| B8 | Data-side writer in FM4NPP format | **Resolvable** | write a data extractor that emits the `hits`-tree branch set without truth (CRJE + h1hits coordinates + CJXR/CJKR pseudo-labels + CST/COZ points); `$R/lam/` can be converted at once as a 57k-event 2007 sample | Medium |
| B9 | FM4NPP converter `h1_to_fm4npp.py` (LCRC only) | **Open**: needs the user | copy it to NAF, or run the data `hits` trees through it on LCRC via `scripts/ship_to_lcrc.sh` | Small, with user action |
| B10 | No truth in data | **Hard limit** for supervised targets | data offers H1 pseudo-labels only (CJXR→CJKR; MC hit purity 98%); hits not on tracks are unlabeled. Fine for self-supervised pretraining [I] | — |
| B11 | NDB availability | **Resolved** | AFS NDB readable on 2026-10-22; the DUST 2014 copy is an unvalidated fallback | — |
| B12 | Data/MC domain differences | Not a blocker; a known shift | data has 1.5–2× more off-track CJC hits, CST noise, worse CJC z (`README_SHERPA_HITS.md:213`) | — |

## 7. Risks, open questions, coverage gaps

- **Untested periods.** 2006: raw access chain tested (c060039, `$R/AGENTS.md:24,33`), but no
  constants dumped. 2005 e−, 07low/07med and 03–04 have not been read at all. HV, dead-detector runs (for example run 492559 with the CJC off), and code-path changes are
  plausible.
- **Track-free resolution.** Hits off H1 tracks get about 500–630 µm r-φ, because the track-dependent
  terms (β, s, dB) are not ported. Hits on tracks get 262 µm. This is a choice for the FM4NPP input
  definition.
- **Circularity.** The event T0 and the CJXR coordinates inherit H1's reconstruction.
- **Software fragility.** The probe needs patched work-bank sizing in CJXREF, skips CJCRJE, and uses
  a private GUFLD (`calib_consts/README.md:148-162`).
- **Not verified here:** C1J8/C2J8 run-to-run stability (beyond the bank diff), CST point quality,
  the 2005 IndexDST resolution, dst7/gmcut tape locality, and the LAr/SpaCal status at HAT level.
- **Open questions for the user:** (1) What exact input schema does `h1_to_fm4npp.py` consume, and
  can it be copied to NAF? (2) Is pretraining self-supervised (truth not needed), or does it use
  labels (H1 pseudo-labels only)? (3) Which periods and Q² selections, and how many events?
  (4) Must CIP/COP and CST be in the first data release, or is CJC+COZ enough? (5) Are track-dependent
  coordinate corrections (H1-reco-informed) acceptable in the inputs, or must the inputs be track-free?

## 8. Recommended next stage

A planner stage for a data-side `hits` writer: the 2007 `$R/lam/` sample first, then 2005/2006
constants, then CST and CIP/COP. The open questions in §7 should be answered first.

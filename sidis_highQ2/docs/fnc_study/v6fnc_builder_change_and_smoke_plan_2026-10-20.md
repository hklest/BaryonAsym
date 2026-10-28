# v6fnc Stage-1 FNC additive fork — builder change record and smoke plan (2026-10-20)

- **Vehicle:** isolated Stage-1 fork `v6fnc` (the proven `v6bsa`/`v6dif`
  additive-fork pattern; see `docs/fnc_study/fnc_h1oo_availability.md` §7).
  The central v6 Stage-1 source/binary, the `evls_v6_*` production, and all
  central v6 products are untouched. Selection, weighting, event loop, and
  all pre-existing branches are inherited bit-for-bit (diff vs the v6
  source: 1 changed line = the startup banner string; +286 added lines =
  FNC header docs, includes, branches, fill logic).
- **Companion research:** `docs/fnc_study/fnc_h1oo_availability.md`
  (banks, accessors, data-side FNC occupancy: c070005 FNC-good 18.55%,
  c070000 FNC-excluded 0%).

## Files

| File | Change |
|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6fnc.C` | NEW — fork of `TreeBuilderHighQ2_chargedsidis_v6.C` + additive FNC block (see branch table) |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/Makefile` | NEW isolated target `TreeBuilderHighQ2_chargedsidis_v6fnc` (BINDIR override + explicit link rule; same object set as v6 Stage 1: FidVolCut.o, FidVolCutDict.o, TDetectQedc.o, TDetectQedcDict.o; NOT in default `BIN`) |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6fnc` | NEW binary (build verdict below) |

Nothing under `output_v6/`, `condor_v6/`, or `steering_v6/` was modified;
the fork reuses the central steering read-only.

## New EvlTree branches (stage_evls; all Fnc-prefixed, event level)

Reconstructed FNC (data + MC). Source object: MODS `FNCPart`
(`H1PartFNC`, production-time `H1FindFNC` output = FNCL cluster 0 only,
hard E > 10 GeV; empty on FNC-excluded runs).

| Branch | Type | Meaning (sentinel when `FncPresent==0`) |
|---|---|---|
| `FncPresent` | I | 1 = FNC particle stored in MODS FNCPart |
| `FncGoodRun` | I | 1 = `H1FNCQuality::IsRunGood(run)` (HERA-II BR10/FNC DAQ/readout list; dummy lookup on MC) |
| `FncE` | F | cluster energy [GeV] (`H1PartFNC::GetE()` = raw `fFncEne`; -1) |
| `FncX`/`FncY`/`FncZ` | F | cluster position [cm], z ~ 10600 (-999) |
| `FncTimeToF` | F | FNCL Timetof (`H1PartFNC::GetTimeToF()`) (-999) |
| `FncTime` | F | FNCL Timecell cluster time (`H1PartFNC::GetTime()`) (-999) |
| `FncClType` | I | 1 = photon-like, 2/3 = neutron/hadron-like (-1) |
| `FncQuality` | I | FNCL reconstruction quality word (-1) |
| `FncThetaN` | F | theta_n [rad] from cluster position and the event vertex (primary vertex as steered, vtOptimalNC): `atan(sqrt((x-xv)^2+(y-yv)^2)/(z-zv))`; vertex-corrected, differs from the vertex-free `H1PartFNC::GetTheta()` (-1) |
| `FncPhiN` | F | phi_n [rad] = `atan2(y-yv, x-xv)` (-999) |
| `FncHtype` | I | HAT `PartFNC` type/quality code via `H1Calculator::Fnc()->GetHtypeFNC()`: 0 none, 1 good had, 2 bad had, 3 good EM, 4 bad EM (a code, NOT an index) |

Generator-neutron truth (MC only; created under `--mode mc` like
`Q2sGen`/`YsGen`/`GenEmpz`). Source object: MODS `MCParticles`
(`H1PartMC`), GTR only (`IsGtr()`), PDG 2112, E > 10 GeV (mirrors the
finder floor), theta < 0.010 rad (generous builder-side envelope around
the FNC acceptance, > 2x margin — NOT an analysis fiducial); leading =
highest energy.

| Branch | Type | Meaning |
|---|---|---|
| `FncGenNFound` | I | 1 = leading forward GTR neutron found (validity flag) |
| `FncGenNE` | F | its energy [GeV] (0 when not found) |
| `FncGenNTheta` | F | its polar angle [rad] from the 3-momentum (0 when not found) |
| `FncGenNPhi` | F | its azimuth [rad] (0 when not found) |
| `FncGenNVx`/`FncGenNVy`/`FncGenNVz` | F | production-vertex columns [cm]: carry the EVENT-level simulated vertex (MODS SelVertices `vtGen`, via `H1CalcVertex::GetPosition(vtGen)`) as the documented best-available proxy; zero when no leading neutron. The per-particle GTR production vertex is NOT stored in MODS/HAT (`H1PartMC` has no vertex members) |
| `FncGenNVtxMode` | I | vertex semantics code: 1 = event-simulated-vertex proxy (current version); 2 = true per-particle production vertex (reserved; produced by no current input) |

Gen-only mode (`--gen-only`, NORAD HATs): all Fnc branches are filled
with the documented absent sentinels (`FncPresent=0`,
`FncGenNFound=0`, `FncGoodRun` still the honest run lookup).

## Gen-neutron-truth availability verdict — djangoh14/7091 (MEASURED, single-file inspection 2026-10-20)

Input: `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/djangoh14/7091/mods.4.0.6.DJANGOH14.NC.EPLUSP.0607.RAD.Q2GT60.CTEQ6L.A.7091.S39200.R97800.ftt.DST.0001.root`
(+ matching hat), 100,000 events, plain-ROOT streamer-info emulation
(login node; same technique as the availability research):

- **Generator neutron truth IS available.** MODS `MCParticles` carries
  GTR neutrons (PDG 2112): 85,003 total; 57,015 with E > 10 GeV;
  30,221 forward (theta < 0.010 rad, E > 10 GeV) in 25,298 events
  (25.3% of events); forward mean E ≈ 338 GeV, max ≈ 915 GeV. (Also
  60,546 anti-neutrons, not stored.) DJANGOH14/7091 DOES transport a
  forward neutron population — the "DJANGOH did not transport it"
  fallback does not apply.
- **FNC reco content IS present in the MC**: `FNCPart` non-empty in
  6,268/100,000 events (6.27%), mean cluster E ≈ 274 GeV, ClType
  mostly 2/3 (hadronic), a few 1 (EM); HAT `PartFNC`: 1=1521, 2=4720,
  3=17, 4=10 (nonzero fraction 6.268% = MODS fraction exactly).
- **Reco-vs-true is viable**: 6,131 of the 6,268 FNC-part events
  (97.8%) also contain a forward GTR neutron (E>10 GeV,
  theta<0.010 rad).
- **NOT available**: per-particle neutron production vertex (no vertex
  members in `H1PartMC`; HAT carries only event-level `VtxZ` recon) —
  hence the `FncGenNVtxMode==1` event-vertex-proxy design. The
  event-level simulated vertex IS available (MODS `SelVertices`
  fType==1 once per MC event, mean Z ≈ -0.52 cm).
- Notable: no GTR neutron with E>10 GeV has theta < 0.0008 rad, yet
  FNC clusters exist — the FNC-relevant GTR neutrons sit at
  0.0008 < theta < 0.010 rad (beamline offsets/acceptance), i.e. the
  0.010 rad envelope contains the full FNC-relevant population with
  margin.

## Compile

- Environment: `export GROUPROOT=/afs/desy.de/group/h1; export
  H1_ROOT=/afs/desy.de/group/h1; export H1CURRENT=current; source
  /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh`
- Command (established explicit-isolated-target procedure; NOT in
  default `BIN`, so central binaries are not rebuilt):
  `make bin BIN=TreeBuilderHighQ2_chargedsidis_v6fnc`
  in `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`
- **Verdict: PASS (2026-10-20, BUILD_EXIT=0).** Binary:
  `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6fnc`
  (2.15 MB; central v6 is 2.13 MB). Compile produced only warnings
  (pre-existing `-Woverloaded-virtual` family plus the inherited
  `nuGen` unused-variable warning at the old line number); zero
  errors. All 21 `Fnc*` branch-name and leaf-type strings verified in
  the binary. `readelf -d` comparison vs the central v6 binary:
  identical `DT_NEEDED` set and identical single ROOT-lib `RPATH` —
  same Condor runtime profile as central production.

### Build-environment notes (login node, 2026-10-20; needed to reproduce the link)

Two environment facts, not code defects, affected the build and are
recorded so the next fork build does not rediscover them:

1. **Link needs the LCG view lib dirs on `LD_LIBRARY_PATH`.** ROOT
   6.34.04's `libImt.so`/`libROOTVecOps.so` transitively need
   `libtbb.so.12`/`libvdt.so`, which live in the LCG_107a view
   (`/cvmfs/sft.cern.ch/lcg/views/LCG_107a/x86_64-el9-gcc14-opt/{lib64,lib}`),
   NOT in the ROOT lib dir. `thish1.sh` sources the view setup but its
   line 141 `sed -e "s|$H1_LIBS||g"` runs with `H1_LIBS` still unset
   (`H1_LIBS` is only exported later, line 219): the sed fails with
   "no previous regular expression" and emits nothing, WIPING
   `LD_LIBRARY_PATH` (view dirs included) on every fresh source. GNU
   ld does consult `LD_LIBRARY_PATH` for transitive `NEEDED`
   resolution, so with the wiped path the final link fails on
   unresolved TBB symbols. Fix used (environment-only, no file
   changes): after sourcing `thish1.sh`, prepend
   `/cvmfs/sft.cern.ch/lcg/views/LCG_107a/x86_64-el9-gcc14-opt/lib64:/cvmfs/sft.cern.ch/lcg/views/LCG_107a/x86_64-el9-gcc14-opt/lib`
   to `LD_LIBRARY_PATH` before `make`.
2. **`BRANCHFILE=` skips an ~8-13 min Makefile-parse crawl.**
   `H1Basic.mk` line 60 evaluates
   `BRANCHFILE = $(shell find $(MAKEDIR) -name makeptr.branches)` at
   parse time; over this DUST workspace the `find` takes 8-13 minutes
   (no `makeptr.branches` exists; `PTRS` is inert). Passing
   `BRANCHFILE=` on the make command line overrides it harmlessly and
   skips the crawl. Build time drops to ~1 min (compile ~30 s +
   link).

Exact reproducer (BUILD_EXIT=0):

```bash
export GROUPROOT=/afs/desy.de/group/h1; export H1_ROOT=/afs/desy.de/group/h1; export H1CURRENT=current
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
export LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/views/LCG_107a/x86_64-el9-gcc14-opt/lib64:/cvmfs/sft.cern.ch/lcg/views/LCG_107a/x86_64-el9-gcc14-opt/lib:$LD_LIBRARY_PATH
cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2
make bin BIN=TreeBuilderHighQ2_chargedsidis_v6fnc BRANCHFILE=
```

The LD_LIBRARY_PATH augmentation is a LINK-time need on the login
node only; at runtime on Condor the standard production wrapper shape
(`desygroupset h1` + `thish1.sh`, as in
`condor_v6/studies/flatphi_link_fixed_20260927/wrappers/`) suffices —
the central v6 binaries with the identical `NEEDED` set run under it
in production.

## Smoke plan (single data file + single MC file; login-node execution BLOCKED, see below)

Run from CWD `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`
(production `initialdir`; `FidVolCut.steer` resolution matches
production). Outputs go to a NEW smoke directory, NOT under
`output_v6/`:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/smoke_v6fnc_2026-10-20/`.

1. DATA (FNC-good ep0607 chunk c070005, runs from 492991):

   ```
   /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6fnc \
       --stage evls \
       --good-runs /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0607.root \
       --mode data \
       --steer /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/steering_v6/central/evls/data/07/evls_v6_d07_0005.steer \
       -o /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/smoke_v6fnc_2026-10-20/evls_v6fnc_d07_0005_c070005_smoke.root
   ```

2. MC (djangoh14/7091 signal MC, file 0001):

   ```
   /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6fnc \
       --stage evls \
       --good-runs /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0607.root \
       --mode mc \
       --steer /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/steering_v6/central/evls/mc/djangoh14/7091/evls_v6_djangoh14_7091_ep0607_0001.steer \
       -o /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/smoke_v6fnc_2026-10-20/evls_v6fnc_djangoh14_7091_ep0607_0001_smoke.root
   ```

### Smoke acceptance checks

- Tree `EvlTree` present; all 25 pre-existing branch names unchanged;
  13 new reco-FNC branches present on both; 8 new gen-neutron branches
  present on MC only (absent on data, like `Q2sGen`).
- Entry counts must match the central v6 Stage-1 output for the same
  chunks (`output_v6/central/evls/data/07/evls_v6_d07_0005.root`,
  `output_v6/central/evls/mc/djangoh14/7091/evls_v6_djangoh14_7091_ep0607_0001.root`)
  — the keep logic is untouched (reco_env || gen_env).
- DATA c070005 expectations (from the availability research, full
  chunk): `FncPresent==1` fraction ~18.5% of kept events
  (selection-modulated; order-of-magnitude check), `FncGoodRun==1` for
  all kept events (runs 492991+, outside the 492559-492832 exclusion),
  `FncE` mean O(200 GeV), `FncZ` ~ 10600 cm, `FncThetaN` ~ 1e-3 rad,
  `FncHtype` distribution 0-4.
- MC expectations (measured on this very file): `FncPresent==1` in
  ~6.3% of kept events, `FncGenNFound==1` in a large fraction
  (25.3% of ALL events have a fwd GTR neutron E>10; the kept MC
  sample is envelope-OR so the fraction can differ), overlap
  `FncPresent&&FncGenNFound` ~ 97.8% of FncPresent events,
  `FncGenNE` mean O(300 GeV) for found neutrons,
  `FncGenNVtxMode==1` everywhere, `FncGenNVz` mean ~ 0 cm.
- Optional control: rerun the same chunk with the central v6 binary
  and diff entry counts (must be identical).

### Environment blocker (why the smoke did not run on the login node)

This NAF login node currently cannot load the H1OO shared-library
stack: the binaries require `GLIBCXX_3.4.31/3.4.32`, which the system
`/lib64/libstdc++.so.6` (gcc 11 / EL9) does not provide (verified again
2026-10-20 via `ldd` on the central v6 Stage-1 binary: same failure as
recorded in `docs/fnc_study/fnc_h1oo_availability.md` §2). Per the task
instruction this is NOT worked around (no libstdc++ swapping); the two
commands above must be executed through the Condor path
(`sidis-hq2-operations` / `h1-naf-operator`), e.g. as one-job vanilla
submissions mirroring `condor_v6/central/evls/subs/*` with the v6fnc
binary and the smoke output paths above (RedHat9/X86_64 requirement,
`initialdir = H1ChargedSidis_HighQ2`, ~6 GB memory; each file is a
single-chunk Stage-1 pass, comparable to the ~1-2 min production jobs).

## Downstream impact

- Stage 2 (`ChargedSidisTreeBuilderHighQ2_v6`) consumes only Stage-1
  EVL files; it ignores unknown branches, so central Stage-2 runs are
  unaffected. An FNC-aware Stage-2 fork (copying the Fnc* event-level
  branches into the track trees or an event-level FNC tree) is a
  follow-up task if per-track correlation tuples are needed.
- Production-scale FNC EVLS generation over the 658 data chunks + MC
  chunks is a Condor campaign owned by operations; NOT launched here.
- Physics policy (FNC fiducial cuts, neutron selection for
  measurement, FNC-trigger treatment) remains with the physics
  analyst; the builder stores unfiltered finder output + generous
  gen-truth envelopes.

## Status

- [x] Fork source written; additive-only verified by diff (1 banner-line
      swap + 286 added lines; re-verified after the build).
- [x] Makefile target added.
- [x] Gen-neutron-truth availability measured (available; vertex only
      as event-level proxy).
- [x] Compile + link PASS (`make bin BIN=TreeBuilderHighQ2_chargedsidis_v6fnc`,
      BUILD_EXIT=0, 2026-10-20 00:54; warnings only; 21 Fnc* branch
      strings present in the binary; `DT_NEEDED`/`RPATH` identical to
      central v6). Link required the LCG-view `LD_LIBRARY_PATH`
      augmentation and the `BRANCHFILE=` crawl skip documented under
      "Compile" — environment quirks, not code changes.
- [ ] Smoke: BLOCKED on the login node by the GLIBCXX runtime issue
      (identical 81-line `ldd` not-found signature as the central v6
      binary — same toolchain, no workaround applied per task rules);
      commands prepared above for Condor routing.

## Dated erratum — 2026-10-20: reco--truth overlap statement corrected

The statement in the MC expectations that `FncPresent && FncGenNFound` is
approximately 97.8% of `FncPresent` is **incorrect for the produced v6fnc
Stage-1 tree**.  Direct count in the recorded smoke tree is
`2312 / 5981 = 38.66%` (and the independently quoted raw-file check is
`2443 / 6268 = 38.98%`).  The 97.8% figure must not be used as a validation
criterion or described as a reco--truth match efficiency.

`FncGenNFound` denotes the leading GTR neutron in the deliberately broad
builder envelope (`E>10 GeV`, `theta<10 mrad`), not an FNC-fiducial,
one-to-one truth match.  An analysis reco--truth comparison must instead
require a generator neutron in a documented FNC geometrical fiducial
(projected position/azimuthal acceptance at the FNC) and then apply an
explicit one-to-one angular/energy association.  The event-level v6fnc
branches contain no truth--reco association identifier and no charged-track
content, so that analysis-level match cannot be certified from these EVL
files alone.  This is an analysis-product limitation/definition issue, not
evidence for an error in the additive builder branch filling.

## Dated addendum — 2026-10-20 (later session): Part A implemented; FNC-aware Stage-2 fork added

**Part A (gen-neutron FNC projection branches, Stage 1).**  The fork source
`src_stage1/TreeBuilderHighQ2_chargedsidis_v6fnc.C` now adds four MC-only
EvlTree branches when a gen neutron was found (`FncGenNFound==1`), sentinel 0
otherwise:

- `FncGenNXfnc/F`, `FncGenNYfnc/F` — straight-line extrapolation of the gen
  neutron to the FNC z plane: anchor vertex
  `gH1Calc->Vertex()->GetPosition()` (steered `vtOptimalNC`, same call as the
  reco `FncThetaN/FncPhiN`), target z = the event's FNC cluster z
  (`fncPart->GetZ()`) when a reco cluster exists, else nominal 10550 cm
  (`H1PartFNC::GetTheta()` convention); guarded by `fncProjDz > 0 && pz > 0`.
- `FncGenNThetaProj/F`, `FncGenNPhiProj/F` — projected angles with the same
  vertex convention as the reco angles; **equal to the momentum-direction
  angles (`FncGenNTheta/FncGenNPhi`) by construction** and stored to fix the
  downstream convention.

Diff vs central v6 Stage-1 remains banner-swap-only removal (1 line) plus
additions.  Rebuilt with `make bin BIN=TreeBuilderHighQ2_chargedsidis_v6fnc`
(BUILD PASS; all 21 Fnc* branch strings verified in the binary).  The
recorded pre-Part-A smoke EVLS in `H1ChargedSidis_HighQ2/smoke_v6fnc_2026-10-20/`
predate this change (no projection branches) and are retained as the
baseline / graceful-path inputs; fresh Part-A smokes write to `_v6fncPA_`
files (see `SMOKE_COMMANDS.md` there).

**Part B (FNC-aware Stage-2 fork).**  New isolated source
`src/ChargedSidisTreeBuilderHighQ2_v6fnc.C` (new Makefile target, binary
`ChargedSidisTreeBuilderHighQ2_v6fnc`; central v6 Stage-2 source/binary
untouched, md5 re-verified `9a585a3d0f4d3e94a699882069d7ef3c`).  Diff vs
central v6 Stage-2: 3 banner-string replacements only, everything else
additive.  It denormalises event-level FNC values from the v6fnc Stage-1
EvlTree sidecar onto every `ChargedTot` row (v6bsa pattern; ChargedTotGen and
the detached DisTot unchanged): 7 reco branches data+MC
(`FncPresent/I, FncGoodRun/I, FncE/F, FncThetaN/F, FncPhiN/F, FncClType/I,
FncHtype/I`), 9 gen branches in MC mode only (`FncGenNFound/I, FncGenNE/F,
FncGenNTheta/F, FncGenNPhi/F, FncGenNXfnc/F, FncGenNYfnc/F,
FncGenNThetaProj/F, FncGenNPhiProj/F, FncGenNVtxMode/I`).  Output metadata
adds `TNamed v6fnc_stage2_version =
"ChargedSidisTreeBuilderHighQ2_v6fnc/2026-10-20"`.  Input contract: 7 reco
Fnc* REQUIRED (central `evls_v6_*` rejected, exit 5); 5 core FncGenN*
REQUIRED in `--mode mc` (exit 5); 4 Part-A projections OPTIONAL (one WARN
line, sentinel-0 columns).  Compile PASS; all 16 branch strings + leaflists
verified in the binary.

**Schema compatibility (plain-ROOT, login-node-safe; macro
`smoke_v6fnc_2026-10-20/check_stage2_input_schema.C`):** per-chunk data EVLS
(`evls_v6fnc_d07_0005.root`) and both recorded smoke EVLS are USABLE Stage-2
fork inputs (EvlList key present, required branch sets complete; the MC
smoke exercises the Part-A-absent graceful path).  The merged
`evls_v6fnc_d07_ep0607_merged.root` is NOT usable (hadd dropped the EvlList
key) — Stage-2 must consume per-chunk files.  Note: plain ROOT cannot
instantiate the `H1EventList` object; key presence must be checked at
`TKey` level (`FindKey`), not via `TFile::Get`.

**Smoke commands prepared (execution still Condor-only):** see
`H1ChargedSidis_HighQ2/smoke_v6fnc_2026-10-20/SMOKE_COMMANDS.md` — fresh
Part-A Stage-1 data/MC smokes (`_v6fncPA_` outputs), Stage-2 on each fresh
EVLS, optional graceful-path Stage-2 runs on the recorded old EVLS, plus a
negative input test (central evls_v6 must exit 5).  Wrappers under
`smoke_v6fnc_2026-10-20/wrappers/`, steers under `smoke_v6fnc_2026-10-20/steer/`.
The smoke-command block earlier in this document (writing fresh Stage-1
smokes onto the recorded filenames) is superseded by the `_v6fncPA_` naming
in `SMOKE_COMMANDS.md` so the recorded baseline files are preserved.

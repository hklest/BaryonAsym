# Research: Pythia8 port of the hit-level chain (`GEN=pythia8`)

Date: 2026-10-27. Scope: what an executor must build so that
`/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/hits_chain_wrapper.sh` accepts
`GEN=pythia8` and runs the same downstream chain as `GEN=sherpa` (HepMC3, then
`convert_hepmc3_to_hepevt.py`, `h1hepevt`, preprocessing, patched `h1simrec9`, `dst2ods`,
`run_extract.C`), with Pythia8 reproducing the hiQ2 and loQ2 Sherpa phase space for a
100k FM4NPP sample. No code here. Abbreviations: `$H` = `.../BaryonAsym/hit_level_sim`,
`$V` = `/cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt`.
Labels: **[V]** verified in this session (file:line or command output), **[I]** inference.

## 1. Sherpa path end to end

- [V] Sherpa branch: `$H/hits_chain_wrapper.sh:108-119`. Seed `SEED=$((SEED_OFFSET + JOB_IDX))`
  (l.109). Card copied to `Sherpa.yaml`, `Results.zip` and `Process/` copied from
  `SHERPA_RESULTS` (l.110-111). Run inside a clean shell:
  `env -i HOME="$HOME" PATH=/usr/bin:/bin bash -c "source $H/sherpa_env.sh; $SHERPA_BIN -f Sherpa.yaml -R $SEED -e $NEV"`
  (l.112-113), timed by `run_stage` (l.75-82; stdout to `sherpa.stdout`).
- [V] Output: card line `EVENT_OUTPUT: HepMC3_GenEvent[events]` (`$H/sherpa_pilot/cards/Sherpa_hiQ2.yaml:25`)
  writes an uncompressed HepMC3 ASCII v3 file named `events` in the cwd. The wrapper checks `[ -s events ]` (l.115),
  renames it to `part_<i>.hepmc` (l.116-117) and runs
  `python3 $SHERPA_CONVERTER part_<i>.hepmc part_<i>.hep part_<i>_truth.csv` (l.118). The lAger branch (l.120-125)
  is the only other generator. Everything after l.126 is generator-agnostic.
- [V] Sherpa env: `$H/sherpa_env.sh:3-6` sources the LCG_109a view, adds the Sherpa 3.0.3.atlas1 lib dir and sets `SHERPA_BIN`.
  The H1OO env (`$H/env.sh`, LCG_107a) is sourced only after conversion (l.128).
- [V] Real Sherpa file (`$H/sherpa_pilot/output/hiQ2/gen/part_0.hepmc`): header `HepMC::Version 3.03.01`,
  `HepMC::Asciiv3-START_EVENT_LISTING`, `U GEV MM`. The positron beam is `P 1 0 -11 ... pz=+27.6 ... status 4`, the proton
  `P 3 0 2212 ... pz=-919.9995`. **Sherpa lepton is +z, proton -z.** Decay vertices are written with `@` positions in mm (4406 positioned `V` lines in this file, z from -52.9 to +4.5 mm), and the converter passes them to VHEP.
  `W` holds a per-event weight; `README_SHERPA_HITS.md` (Gotchas) says weights are dropped and negligible for these LO cards.

Converter `/data/dust/group/h1/klesthen/BaryonAsym/sim_smoketest/sherpa/convert_hepmc3_to_hepevt.py` (274 lines):
- [V] Plain-text parser, not a HepMC3 library reader. It reads only `E`, `P` and `V` lines (l.234-250) and ignores `W`, `A`, `U`, `T`.
  It stops at the first `END_EVENT_LISTING` (l.251-255). Units are taken as GeV and mm without conversion (docstring l.14).
- [V] **Hard-coded rotation** `ROTATE_Y_PI = True` (l.56): every momentum and vertex goes (x,z) -> (-x,-z) (l.62-63, 93-95),
  because "SHERPA writes the lepton beam along +z" (l.46-55). There is no CLI flag; the args are positional `in out csv` (l.271-273).
- [V] Keeps **all** particles. ISTHEP=1 if HepMC status==1, else 2 (l.146). Mothers and daughters come from the `pvtx` encoding
  (l.116-169). Production-vertex position: explicit `V ... @ x y z t` when present, else 0 (l.172-175).
- [V] HEPEVT NEVHEP = 0-based running count, not the HepMC event number (l.231). Beams are identified as `pvtx==0` with
  PDG ±11 or 2212 (l.187-193). The truth CSV takes the highest-E final-state lepton with the beam PDG (l.196-202).
- [V] `h1hepevt` reader `/data/dust/group/h1/klesthen/h1hepevt-build/source/hpread.F:24-37` reads that text and
  recomputes E from p and m (l.34-35). `PYHEPC(2)` copies the record into PYJETS. `PYEXEC` runs only if `PYEX!=0`, and
  the default is 0 (`hpini.F:5`). The steering template `$H/steering/hits_h1hepevt.steer.tmpl` sets no `H1HP`
  parameters (l.35-37); only its comment banks mention lAger (l.21,28), so they are cosmetic. HEPEVT capacity is
  `NMXHEP=4000` (H1 4.2.1 `chepevt.inc:2`).
- [V] No other stage reads generator-specific content. `extract_hits.C` keeps generator particles via the GTR link
  (`str_gtr`, l.135, 432) and makes no Sherpa assumption.

## 2. Sherpa phase space to reproduce (both cards identical except Q²)

| Item | hiQ2 (`$H/sherpa_pilot/cards/Sherpa_hiQ2.yaml`) | loQ2 (`.../Sherpa_loQ2.yaml`) |
|---|---|---|
| Beams | `[-11, 2212]`, `[27.6, 920.]` (l.27-28): e⁺p | same (l.29-30) |
| Process | `-11 93 -> -11 93`, `Order {QCD:0, EW:2}` (l.38-40): LO NC DIS, γ/Z, Comix | same (l.37-39) |
| PDF | `NNPDF31_lo_as_0118` via `LHAPDFSherpa` (l.30-34) | same |
| Cuts | `SELECTORS: [Q2, -11, -11, 150., 1e12]` (l.42-43) | `[Q2, -11, -11, 5., 100.]` (l.42-43) |
| QED | `YFS: MODE: Off`, `ME_QED: ENABLED: false` (l.45-57) | same |
| Shower | Sherpa CSS, `SHOWER: KIN_SCHEME: 1` (l.63-64); Sherpa hadronisation (default) | same |
| Stable | 310, 130, 3122, 3222, 3112, 3312, 3322, 3334 `Stable: 1` (l.66-74) | same (l.59-67) |
| σ | 4.3005 nb (`HANDOFF.md` §3 table) | not recorded |

- [V] **There are no y, W or x cuts** in either card, so no event-level filter is needed. Both Q² limits map to card settings:
  `PhaseSpace:Q2Min` and `PhaseSpace:Q2Max` (Pythia 8.317 `xmldoc/PhaseSpaceCuts.xml:103-118`; Q2Max is applied when > Q2Min).
- [I] The shower and hadronisation models differ by construction (CSS/AHADIC vs Pythia simple shower/Lund). Only the beams, process,
  PDF, Q² window, rad-off lepton line and stable list are matched.

## 3. Pythia8 driver

**What exists.** [V] `sidis_highQ2/MC/pythia8/driver/` holds only `README.md`. The C++ source, Makefile and
`cmnd/pythia8_dis_nominal.cmnd` it describes are not on disk. [V] The card content is preserved in
`sidis_highQ2/docs/plans/plan-MC-predictions-sherpa-pythia6-pythia8.md:1134-1162`: `Beams:idA=11` (e⁺: `-11`), `idB=2212`,
`eA=27.6`, `eB=920`, `frameType=2`, `SoftQCD:all=off`, `HardQCD:all=off`, `WeakBosonExchange:ff2ff(t:gmZ)=on`,
`PhaseSpace:Q2Min=150.`, `PDF:useHard=on`, `PDF:pHardSet`/`PDF:pSet = LHAPDF6:NNPDF31_lo_as_0118`,
`Space/TimeShower:QEDshowerByL=off`, `QEDshowerByQ=on`, and `mayDecay=off` for the same 8 PDG codes as Sherpa.
[V] `sidis_highQ2/.opencode/skills/sidis-hq2-mc/SKILL.md:56` adds the required `SpaceShower:dipoleRecoil=on`,
`SpaceShower:pTmaxMatch=2` and `PDF:lepton=off` ("absent settings caused σ instability").
[V] A working Pythia8 to HepMC3 driver template exists: `/data/dust/group/h1/klesthen/h1hepmc/h1hepmc/pyEvt.cc` (S. Schmitt,
156 lines, `-n nevent -o out -s steering`, `HepMC3::Pythia8ToHepMC3` + `WriterAscii`). It has three flaws for this use:
it has no seed option, it builds the xmldoc path from `$PYTHIA8__HOME` (l.96-97), and it does `if (!pythia.next()) continue;`,
so it writes fewer than N events. Its card `h1hepmc/h1hepmc/pythia8.steer:1-41` has **proton as beam A (+z)**, QED on and
`HardQCD:all=on`. **Do not reuse that card** (see §4).

**Toolchain in LCG_109a** [V] (commands run in a clean `env -i` shell after sourcing `$V/setup.sh`):
- `pythia8-config --version` = 8.317; `--cxxflags --libs` =
  `-O2 -std=c++11 ... -fPIC -DGZIP -I/cvmfs/sft.cern.ch/lcg/releases/MCGenerators/pythia8/317-bd2a2/x86_64-el9-gcc14-opt/include -L.../lib -Wl,-rpath,.../lib -lpythia8 -lz -ldl`;
  `--with-hepmc3` = false (irrelevant: the header plugin is used), `--with-lhapdf6` = true; `$V/lib/libpythia8lhapdf6.so` exists.
  Override `-std=c++11` with `-std=c++17` (HepMC3 3.3 headers) [I].
- `$V/include/Pythia8Plugins/HepMC3.h` defines `HepMC3::Pythia8ToHepMC3` (l.33) and `Pythia8::Pythia8ToHepMC` (l.308).
  It sets `Units::GEV, Units::MM` (l.73), skips entry 0, and adds beams (mother 0) via `add_tree`, so in ASCII they get `pvtx 0`.
- HepMC3 3.3.1 (`HepMC3-config --version` 3.03.01, same as Sherpa's files): include
  `/cvmfs/sft.cern.ch/lcg/releases/hepmc3/3.3.1-1abbe/x86_64-el9-gcc14-opt/include`, lib `.../lib64` (`-lHepMC3`).
- `PYTHIA8DATA` is set by the view, so use the default `Pythia` constructor. LHAPDF 6.5.5, `NNPDF31_lo_as_0118`
  is installed, and `LHAPDF_DATA_PATH` is set by the view.

**Probe** [V] (Python bindings `pythia8.so`, 1500 events, settings as in the minimal card below, without mayDecay; scratch only):
- `idA=-11, idB=2212, frameType=2`: beam A `-11` has pz = +27.6 and beam B `2212` has pz = -919.9995, identical to Sherpa.
- hiQ2 `Q2Min=150`: σ_gen = 4.22 nb (Sherpa 4.30 nb). loQ2 `Q2Min=5`, no Q2Max: 3.3 % of events have Q² = -t̂ > 100.
  With `PhaseSpace:Q2Max=100.` the maximum Q² is 96.9 and σ_gen = 263.1 nb, about 1.7 % above the 258.7 nb implied by the no-Q2Max run; plausibly statistical at 1500 events. [I] `sigmaGen` then refers to the window; confirm with `sigmaErr` in the smoke run.
- **`PhaseSpace:mHatMin` (default 4 GeV) silently cuts loQ2.** Since ŝ = Q²/y, it removes y > Q²/16 GeV⁻²:
  σ 253.0 nb (mHatMin=4) vs 267.5 nb (mHatMin=0), and the fraction with y>0.5 drops from 7.1 % to 4.2 %.
  Sherpa has no such cut. Set `PhaseSpace:mHatMin = 0.` (harmless at hiQ2: ŝ ≥ Q² ≥ 150).
- About 0.1 % of `next()` calls fail (remnant/shower errors, normal), so the driver must loop until N events are accepted.
- Pythia event-record size: mean 50–80, max 312 entries, well below `NMXHEP=4000`.

**Minimal driver spec** (new, e.g. `$H/pythia8/src/pythia8_dis_hits.cc`; base it on `pyEvt.cc`, not on the lost sidis_highQ2 driver):
- CLI mirroring Sherpa's role: `--card FILE --seed INT --nev INT --out PATH` (+ optional `--info-out JSON`).
  `readFile(card)`, then `readString("Random:setSeed = on")`, `readString("Random:seed = <seed>")`.
  **Reject seed ≤ 0**: Pythia seed 0 means a clock-based seed, and the wrapper gives seed 0 when `SEED_OFFSET=0, JOB_IDX=0`.
  Valid range ≤ 900000000.
- Loop `while (nacc < nev)`: `if (!next()) continue;`, then convert with `Pythia8ToHepMC3::fill_next_event(pythia, &evt)`,
  then `WriterAscii::write_event`. Cap total tries at `Main:timesAllowErrors`-like bound. Print `sigmaGen` (σ in the Q² window), `sigmaErr`,
  tries and accepted counts.
- Do **not** rotate in the driver (see §4).

**Cards** (new, `$H/pythia8/cards/pythia8_{hiQ2,loQ2}.cmnd`), same content except Q²:
`Beams:frameType=2`, `Beams:idA=-11`, `Beams:idB=2212`, `Beams:eA=27.6`, `Beams:eB=920.`, `SoftQCD:all=off`, `HardQCD:all=off`,
`WeakBosonExchange:ff2ff(t:gmZ)=on`, `PhaseSpace:Q2Min=150.` (loQ2: `5.` and `PhaseSpace:Q2Max=100.`), `PhaseSpace:mHatMin=0.`,
`PDF:lepton=off`, `PDF:pSet=LHAPDF6:NNPDF31_lo_as_0118` (+ `PDF:useHard=on`, `PDF:pHardSet` same), `SpaceShower:dipoleRecoil=on`,
`SpaceShower:pTmaxMatch=2`, `SpaceShower:QEDshowerByL=off`, `TimeShower:QEDshowerByL=off`, and the 8 lines
`310/130/3122/3222/3112/3312/3322/3334:mayDecay=off`. Plus `Next:numberCount=0` (keeps logs short).

## 4. Beam orientation (most likely silent failure)

- [V] H1 frame: proton +z, lepton -z (converter comment l.46-50: HEAR requires PLEPIN<0, PPIN>0).
- [V] Sherpa writes lepton +z (§1 file excerpt), and the converter **always** applies R_y(π) (l.56).
- [V] Pythia8 puts beam A along +z. With `Beams:idA=-11, idB=2212` the probe gives lepton +z, the same as Sherpa.
  The `sidis_highQ2` plan l.187 says the same for `idA=11`.
- **Conclusion: lepton = beam A in the card, no rotation in the driver, converter unchanged.** The failure mode is a card
  with `idA=2212` (as in `h1hepmc/pythia8.steer:3`). The converter would then rotate a frame that is already H1-like into the
  wrong one. Nothing crashes, but the electron ends up in the forward LAr and the hadrons go backward.
- Guard (cheap): after conversion, for the first events check that the beam-lepton `PHEP(3)` < 0 in the HEPEVT
  and that the final-state Σ(E-pz) ≈ 55.2 GeV (`$H/hep_empz.py`). For loQ2, truth-CSV `scat_theta` should be ≳ 150°.

## 5. Operational

- **Env.** [V] Pythia, HepMC3 and LHAPDF are all in LCG_109a, which `sherpa_env.sh` already sources (l.3). Add
  `$H/pythia8/pythia8_env.sh` (one line: source `$V/setup.sh`) and run the driver exactly like Sherpa, in
  `env -i HOME="$HOME" PATH=/usr/bin:/bin bash -c "source $H/pythia8/pythia8_env.sh; $H/pythia8/bin/pythia8_dis_hits --card ... --seed $SEED --nev $NEV --out events"`.
  This keeps it out of the H1OO LCG_107a env, which is sourced only at l.128.
- **Build.** A `Makefile` in `$H/pythia8/`, using `pythia8-config --cxxflags --libs` + `HepMC3-config --cflags --ldflags` (+ `-ldl`,
  `-Wl,-rpath` for the HepMC3 lib64), built once on the login node in the LCG_109a env. Binary at `$H/pythia8/bin/`.
  No integration step is needed (unlike `sherpa_integrate.sh`).
- **Wrapper edit.** Add `elif [ "$GEN" = "pythia8" ]` next to l.108. Use the variables `PYTHIA_CARD` (and optionally `PYTHIA_BIN`), then
  stage name `pythia8`, `[ -s events ]`, `mv events ${TAG}.hepmc`, and the **same** `SHERPA_CONVERTER` call (l.118). Update the header
  comments (l.14, 29-31) and the `HANDOFF.md` §3 table. [V] Running jobs (411, `sherpa100k_set2`) use
  `production/sherpa100k_set2/hits_chain_wrapper_frozen.sh`. That copy is identical to the live wrapper now (`diff -q` empty), so editing
  the live wrapper is safe. Freeze a copy per production as before (`README_SHERPA_HITS.md`, "How to make more").
- **Production layout.** Mirror `$H/production/sherpa100k/make_submit.sh:1-24`: `GEN=pythia8 NEV=250 STEER_SET=official
  SKECUT_CENT=0.0 KEEP=hits`, 1 CPU, 3 GB, 4G disk, `RequestRuntime 7200`, `RedHat9`, `queue JOB_IDX from seq 0 199` per card.
  Output goes to `$H/production/pythia100k/{hiQ2,loQ2}`.
- **Seeds.** [V] Offsets in use: 100000–1000000 in 100000 steps plus 910000
  (`production/sherpa100k_set2/make_submit.sh`: 800000/1000000; `skecut_scan.sh`, `local_e2e.sh` etc.). Recommend
  **2000000 (hiQ2) and 2100000 (loQ2)**, which are free and inside Pythia's seed range.
- **Validation sequence** (login node ≤ 5 min, one file):
  (a) driver alone, 100 events per card, then converter, `hep_empz.py`, and the orientation and σ checks from §3/§4;
  (b) one Condor job with `NEV=10` per card (the HANDOFF quick test takes about 10 min, which is over the login limit). Check
  `Chain complete`, the `events_out` metric and `validate_hits.py` on the hits file;
  (c) Condor pilot of 5 × 250 per card. Compare against `sherpa100k` (hits per event, CPU per event, crash/drop rate, Q²/y truth);
  (d) 200 × 250 per card, run `log_guard.sh --hold` while polling, and check the quota first (25.1/40 TB used today).

## 6. Pitfalls that carry over (`HANDOFF.md` §9, `README_SHERPA_HITS.md` Gotchas)

- Count events from the outputs: preprocessing drops 1 event per job or chunk, and GEANT crashes drop single events.
  Use `events_out` and the hits-tree size, with L = N_out/σ. Use the Pythia σ in the Q² window (§3), not the Sherpa σ.
- `SKECUT_CENT=0` leads to about 1/600 ZEBRA overflows. The watcher and re-split handle them (`hits_chain_wrapper.sh:177-256`).
  `split_hepevt.py` relies on the `NF==2` event-header lines, which the same converter writes, so it is unaffected.
- Metrics CSV: the `gen` column takes `$GEN` and the stage name is new (`pythia8`). Any accounting script that filters
  on `gen==sherpa` or stage `sherpa` must learn the new values [I]. The size row `gen` covers the `.hepmc` file.
- Log caps and runaway `h1simrec9` logs are handled by the wrapper. Pythia stdout is small if `Next:numberCount=0`.
- [V] Both generators write decay vertices in mm, and the converter passes them unchanged (l.88-95, 172-175). The Pythia path
  therefore uses the same VHEP route as the production Sherpa samples. A cheap check of the D-decay vertex scale stays in step 10.
- [I] The Pythia record carries more status-2 partons and string entries (id 92) than Sherpa's. They enter GTR as non-tracked rows and
  slightly enlarge the pruned truth. The converter's truth-CSV lepton pick (highest-E final e⁺) can pick a Dalitz/conversion e⁺ at
  y→1 for either generator.

## Recommended implementation outline

1. Create `$H/pythia8/{src,cards,bin}` and `pythia8_env.sh` (source LCG_109a `setup.sh`).
2. Write `src/pythia8_dis_hits.cc` from `h1hepmc/h1hepmc/pyEvt.cc`: CLI `--card --seed --nev --out [--info-out]`, default `Pythia` ctor.
3. In the driver: seed > 0 check, `Random:setSeed/seed` after the card, loop until `nev` accepted.
4. Print and write σ_gen (already the Q²-window σ), σ error, tries and accepted counts.
5. Write `Makefile` (pythia8-config + HepMC3-config, `-std=c++17`, rpath). Build on the login node in a clean LCG_109a shell.
6. Write `cards/pythia8_hiQ2.cmnd` and `pythia8_loQ2.cmnd` exactly as listed in §3 (lepton = beam A, `mHatMin=0`, 8 `mayDecay=off`).
7. Login smoke: 100 events per card, then the converter. Check lepton pz<0 after conversion, Σ(E-pz)≈55.2, Q² ranges, absence
   of decayed 310/3122/..., and σ_hiQ2 ≈ 4.2 nb.
8. Add the `pythia8` branch to `hits_chain_wrapper.sh` (share the convert call with sherpa) and update the header comments.
9. Update `HANDOFF.md` §3 variables table and `README_SHERPA_HITS.md` "How to make more" with `GEN=pythia8`.
10. Condor: 1 job × `NEV=10` per card to `$H/pythia8/test/`. Check `Chain complete`, `validate_hits.py`, and D-decay vertex scale.
11. Condor pilot of 5 × 250 per card. Compare with `sherpa100k` for hits per event, CPU per event, drops and truth Q²/y.
12. Write `$H/production/pythia100k/make_submit.sh` (seeds 2000000/2100000), freeze the wrapper copy, check quota, submit 2 × 200 jobs.
13. Monitor with `log_guard.sh --hold`, resubmit failures as in `sherpa100k/*_retry.submit`, record N_out and σ per card.

## Open decisions (need the user)

1. Split of the "100k": mirror `sherpa100k` (50k hiQ2 + 50k loQ2, 200 × 250 per card), or 100k of one card?
2. Quark-line QED: keep `QEDshowerByQ=on` (sidis_highQ2 nominal Pythia card), or switch it off to match Sherpa's "no QED radiation" exactly?

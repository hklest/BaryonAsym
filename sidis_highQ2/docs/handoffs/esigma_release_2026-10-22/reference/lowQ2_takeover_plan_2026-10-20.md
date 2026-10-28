# Low-Q² charged SIDIS — takeover plan to high-Q² parity (2026-10-20)

Scope: 5 ≤ Q² < 100 GeV², SpaCal electron, HERA-II 2005–07 (L_data = 297,657.8 nb⁻¹ per `PIPELINE.md`).
Parity target: high-Q² OPT-A nominal chain (`H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_...2026-10-20/`) and its budget (`systematics_bgsub_env100_optA_c1fix_2026-10-20/`).

## Where low-Q² actually stands (verified on NAF today)

| Item | State |
|---|---|
| Docs tree | `sidis_lowQ2/`. `STATUS.md` is **stale** (2026-05-29, v5g headline 463.9 ± 47.9 nb). The latest truth is `docs/BINNING_STATUS_2026-09-30.md`. |
| Builder line | **v5g retired** (user, 2026-09-30). The line is `src/ChargedSidisTreeBuilderLowQ2_v6isr.C` in `H1SIDIS_LowQ2_v5g/`. |
| Trees | v6isr production complete and validated: `H1SIDIS_LowQ2_v5g/output/v6isr_highq2semantics_2026-09-25/`. Data 577 files (d05 152 / d06 313 / d07 112); DJANGOH14 rad 93; RAPGAP31 rad 35. |
| Schema vs high-Q² v6 | Common branches agree. **Missing on low-Q²:** `GenScatE{E,X,Y,Z}`, `ystar_es_gen` (gen), and `TrackLength` (reco). Low-Q² adds the SpaCal/`PassTu*` diagnostics. |
| Systematic shift machinery | **Absent in v6isr.** It existed in `_v6sys.C` (2026-06-05: HFS shift branches, LeeWest track). The high-Q² v6 tree carries the `v6sys_*` payload. |
| Background MC (PHP PYTHIA 9261, DJANGOH Q² 2–5 gap 7080–83, DIFFVM ρ/ω/φ) | Exists **only in the v5g schema** (`output/mc/`). |
| Non-rad MC for C_rad | Only `output/nonrad_v5g_2026-09-23` (v5g). |
| Binning | **BLOCKER.** The resolution accumulator has not been ported to v6isr (three failed dispatches). Candidate B (`unfolding_highq2parity_2026-09-23/config/frozen_candidate_B.json`) diverges from the template. |
| Response / unfold / bkg / C_rad / budget on v6 | None yet. |
| Condor | No low-Q² jobs in the queue. The held v5g clusters were removed 2026-09-30. |

## Update 2026-10-20 (after user decisions)
- **Decisions:**
  - D1: workspace `H1ChargedSidis_LowQ2/`, created.
  - D2: `GenEmpz>45`, as in high-Q².
  - D3: binning deferred to the unfolding stage, so P2 moves after P4.
  - D4: port the v3 rule.
  - D5: 2003–04 data and the 4 deferred systematics are in.
- **F1, critical:** the v6isr MC is reco-gated by gtustin event lists. All events have reco Q²es in [4.2, 121] and E−pz in [35, 70], so there are no gen-in/reco-out events. The MC must be reprocessed from raw through a **new low-Q² Stage 1 with envelope-OR**, as in high-Q². The draft `evls_genbroad/` has the envelope cuts.
- **F2:** port the high-Q² `H1CalcSystematic_v6sys.h --sys-key` machinery, not the low-Q² coherent ±2% scaffold.
- **F3:** a SpaCal electron E shift is a plain `SetShift(eElecEn)`. Provisionally use ±1% (the H1OO SpaCal-Fl preset), pending a dedicated study.
- Recorded in NAF `sidis_lowQ2/STATE.md`.

## Plan (critical path in **bold**)

### P0 — Takeover hygiene (small, first)
1. Create `sidis_lowQ2/STATE.md`, with a "Last updated" head plus append-only override sections, as in high-Q². Mark every v5g number SUPERSEDED. Add `AGENTS.md`.
2. Seed skills `sidis_lowQ2/.opencode/skills/sidis-lq2-{state,builder,binning,unfolding,backgrounds,radiative,systematics,normalization,lessons,operations}`. Copy the high-Q² structure and keep only verified low-Q² facts.
3. Register canonical dirs. The code and outputs workspace is decision D1.

### P1 — Builder parity: v6isr → v6isr-sys (gates everything downstream)
4. **Patch the v6isr builder.**
   - Add `GenScatE{E,X,Y,Z}`, `ystar_es_gen` and `TrackLength`.
   - Port the `_v6sys.C` shift machinery and the `v6sys_*` metadata keys.
   - Add the SpaCal electron legs: energy scale and theta, magnitudes from low-Q² H1 precedent (research task). **Do not transfer the LAr ±0.5%/±1 mrad values**; the high-Q² skill forbids it.
   - Keep the HFS legs: JES ±1% in-jet, RCES ±1% out-of-jet, θ ±1 mrad, φ ±20 mrad.
   - Smoke on one data and one MC file on the login node.
5. **Full Condor reproduction:**
   - Nominal: data, DJ14 and RG31 rad.
   - Non-rad DJ14/RG31, gen-only, for C_rad.
   - Background MC: PHP 9261, DJ 7080–83, DIFFVM.
   - Paired ± shifted legs: data and MC Stage1, each.
   - Create **all** output dirs before submitting. Use `+` instead of commas in `queue from`.
6. Validate integrity, as in the 2026-09-30 report: open failures, zero-entry files, sentinels, and the gen/reco y-cut alignment (MC `Ys<0.1` tail is ~15%).

### P2 — Binning (current blocker; can start now on the existing v6isr trees, in parallel with P1)
7. **Port the accumulator** following the six steps in `BINNING_STATUS_2026-09-30.md`:
   - Drop the DisTot join.
   - Read `Q2_es_gen/Y_es_gen/x_es_gen` and reject `-999`.
   - Add continuous residual moments and σ68 histograms.
   - Smoke, then Condor, then reduce.
8. Evaluate the candidates against the resolution and occupancy gates. Freeze the binning following the template rules: reco 2:1 in pT only, z 1:1, per-Q²-stratum x bands, `n_reco ≥ n_gen`, and z lower edge 0.05 (`zlow005`). The remaining choices are decision D3.

### P3 — Normalization
9. Re-verify L_data per period. Check the SpaCal trigger prescale weighting (`TrigWeightAC/RW`, recomputed in the builder). Re-derive the MC lumi table for the v6isr samples, since `L_MC_FINAL_RESOLVED_v2.json` is v5g.
10. Data/MC normalization gate, the analog of high-Q² `normalization-gate-*`.

### P4 — Response and truth contract (port the high-Q² OPT-A chain)
11. Port `scripts/unfolding/cone_s2prime_optA_2026-10-19/` (response) and `build_data_vector_v4_alphaveto.py` (data vector) to the frozen low-Q² binning:
    - Truth: `ChargedTotGen`, π/K/p, current hemisphere (reco `xF>0` ≡ gen `xFGen<0`), and the HCM frame for z and pT.
    - Body gen-Q² index = `Q2esGen`, mirroring the C_rad booking (the OPT-A fix).
    - Pool weight `w1*w2`; body weight `w1*w2*NucliaWeight`, applied exactly once.
    - Track class follows the **v3 provenance rule** (STR-only links are background; every GTR-linked track is unfolded). Det.int. is routed response-internally via S2′ `f_i`; external det.int. subtraction is forbidden.
    - Electron cone and alpha veto.
12. Check the count identity: body vs pool, as in the OPT-A merge check.

### P5 — Backgrounds, subtracted pre-unfold (Set R′ analog)
13. Low-Q² Set R′ = e_cone + V0_genavg + **Q²<5 migration (DJ 7080–83) + PHP (PYTHIA 9261)**. Assess DIFFVM ρ/ω/φ the way high-Q² did on 2026-10-07.
14. Uncertainties:
    - `bgsub_env100_rel` (±100% on e+V0) for parity.
    - A new PHP normalization term. Its magnitude needs low-Q² precedent research, and PHP is much larger at low Q² than at high Q².
    - Compton/QED-Compton, which is deferred in v5g.

### P6 — Unfolding
15. Nominal: coherent-prior IBU k=4 (B1-floored).
    - Cross-checks: BBB and native TUnfold (diagnostic only).
    - Validation: closure, DJ↔RG cross-closure, k/regularization scan, and stat toys (Poisson replicas).

### P7 — Radiative corrections
16. Gen-only C_rad from DJANGOH14 rad/non-rad on the release binning, applied post-unfold: σ_Born = C_rad × Unfold. Uncertainties: `c_rad_gen` (DJ vs RG) and `c_rad_stat`. The RAPGAP rad/non-rad lumi is already recovered (`docs/radiative_corrections/`).

### P8 — Systematics budget and covariance (column parity with the OPT-A c1fix budget)
17. Columns:
    - Unfolding: `ibu_stat`, `ibu_model`, `ibu_reg`.
    - Detector: HFS JES/RCES/θ (full-chain paired legs), SpaCal elec E/θ.
    - Normalization and tracks: `lumi`, `tracking_eff_m0p5` (16 seeds), `phi_h_gauss`.
    - C_rad: `c_rad_gen`, `c_rad_stat`.
    - Det.int. and fakes: `detint_removal_{model,stat}`, `fakecol_mcstat`.
    - Background subtraction: `bgsub_{model,stage,fakefrac}`, `bgsub_env100`.
    - `straddle_residual`, `m2_zshift`.
    - Low-Q² extras: PHP norm, trigger efficiency, SpaCal fiducial/dead cell, vertex z.
    - Build with the C1-fixed totals (det.int. enters once).
18. Covariance package per source, with the consumption contract. Gate each with a two-critic review, as in high-Q².

### P9 — Results
19. Release: raw and Born, per charge, `bin_map.csv`. Also h+/h−, data vs MC for 4 generators, and plots and note. BCA/AziAsym is optional and comes after the release.

## Decisions needed from you
- **D1 Workspace.**
  - (a) New `H1ChargedSidis_LowQ2/` mirroring the high-Q² layout (`output_v6/{central,studies,systematics}`, `condor_v6/`, `scripts/`), with the v6isr trees referenced in place, not copied. **Recommended.**
  - (b) Keep working inside `H1SIDIS_LowQ2_v5g/`.
- **D2 Gen fiducial Empz.** The high-Q² OPT-A rad-safe gen fiducial includes `GenEmpz>45`, but the low-Q² doctrine says no gen Empz cut. Parity argues for adopting it plus C_rad; the doctrine argues against.
- **D3 Binning divergences in Candidate B:**
  - z 2:1 vs the template's 1:1.
  - 11 gen pT bins vs 5.
  - 8 (Q², x) cells with no merging.
  - The gates (purity/stability 0.5, k·σ68) are unvalidated.
- **D4 Track-class and nonprimary norm.** Port the high-Q² v3 provenance rule now (recommended), or wait for the high-Q² C1–C12 fixes.
- **D5 Scope.** The 2003–04 data and the 4 deferred v5g systematics: in or out?

## Suggested first execution block
P0 (1–2), then P1-4 builder patch and smoke, and P2-7 accumulator port and smoke in parallel. Then a single Condor wave covering the P1-5 reproduction and the P2 accumulator.

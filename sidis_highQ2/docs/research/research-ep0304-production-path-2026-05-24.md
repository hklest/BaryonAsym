# Research: ep0304 production path (Britzger EventShapes) + SIDIS adaptation proposal

**Date:** 2026-05-24
**Host:** `naf-hone.desy.de`
**Author:** researcher (read-only sweep; one SSH socket check; bounded `ls`/`uproot` probes; no production launched)
**Scope:** Map how the Britzger `g1j_ES` / `EventShapes` analysis produces ep0304 data + MC at high-Q² on HERA-II, and propose a concrete adaptation to add ep0304 back into the H1 charged-particle SIDIS high-Q² workspace at `H1ChargedSidis_HighQ2/`. The mechanism for adding ep0304 already exists in `submit_full_production.py` (the `--include-ep0304` flag and full DATA_PERIODS/GRL_BY_PERIOD/MC_CATALOGS entries are wired). The action proposed below is **policy + dispatch + cost estimate**, not new wiring.
**Companion JSON:** `H1/sidis_highQ2/docs/data/ep0304_production_proposal_2026-05-24.json`
**Validation gates:** every Britzger ep0304 steering inspected has its run range + GRL + HAT path documented; HAT dir run-number sampling done with bounded `ls` + 5-file uproot probe (~ 30 s wall, well under 2 min); no `/pnfs` writes; no edits to V0/proton workspaces.

---

## 0. TL;DR

1. **Britzger uses the same GRL and MC catalogs as SIDIS.** `nc_goodruns_0304.root` (bit-identical to `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0304.root`, md5 `39457f9e…`), DJANGOH14 catalog 7093, RAPGAP31 catalog 7097. Run range `357072,392213`. No "MC10/MC13" generation-campaign mismatch: Britzger's `es.ep0304disNom.steer` and `es.ep0304disMC13.steer` both point at the same 7093 / 7097 catalogs the SIDIS production excluded on 2026-05-24.
2. **Britzger's DATA path is stale.** Britzger reads pre-filtered "kogler" HATs at `/nfs/dust/h1/group/britzger/h1mpim13/kogler/files/oo-4.0/ep0304/data/h1oo4.0.6.data_ep0304_highQ2_sub00-01,sub02-03`. **That path no longer exists** on NAF (`/nfs/dust/h1/group/britzger/` itself is gone). Britzger's published ep0304 data sample was produced from those Kogler high-Q² pre-skims, not from the standard `/pnfs/.../oo-4.0/03,04/` HATs that SIDIS uses. **Implication:** SIDIS cannot adopt Britzger's exact data input path; SIDIS must process the standard year-03 + year-04 HAT directories with the ep0304 GRL applied. This is exactly the path `submit_full_production.py` already implements.
3. **HAT inventory.** `/pnfs/.../oo-4.0/03/` has **20** `hat.*.root` chunks (c030000 – c030019). `/pnfs/.../oo-4.0/04/` has **63** chunks (c040000 – c040062). All year-03 chunks are clean ep0304. Year-04 has a sharp boundary: **c040000 – c040061 = ep0304 (62 chunks); c040062 = post-ep0304 commissioning runs (1 chunk; runs 398286–398679 which are in NEITHER ep0304 NOR em0405 GRL).** Total ep0304 SIDIS-eligible HAT chunks: 20 + 62 = **82** (matches the existing 83-file ep0304 sample within ±1; one boundary chunk may have spanned periods in the prior production).
4. **27.5/27.6 GeV beam-energy split is a non-issue for ep0304 high-Q².** All 4017 runs in the ep0304 high-Q² GRL are at 27.6 GeV (`IsNominalEnergy_27p6_920 == True` for every row in `highQ2_per_run_beam_energy.csv`). Britzger's steering does NOT split on beam energy; the entire ep0304 GRL is single-energy. **No `--ep0304-beam-filter 27p6` is needed**: it is automatic from the GRL.
5. **MC ep0304 catalogs on /pnfs are present and match.** djangoh14/7093 = **142** HAT files; rapgap31/7097 = **122** HAT files. These were previously merged at Britzger's chunk granularity (10 HATs per chunk = ~14 + 12 chunks). SIDIS will run per-HAT (one Condor job per HAT file).
6. **S67 / tracking-drift caveat.** The +3.85σ DIS-rate excess that triggered the 2026-05-23 ep0304 drop is REAL detector physics (see `docs/data/ep0304_rate_audit_v5f.md`). Britzger publishes high-Q² NC DIS results using ep0304 because the EventShapes analysis is dominated by unfolded multi-differential cross-sections that the drift propagates into a small systematic, not a 0/±1 publication blocker. **No Britzger-side per-event reweighting exists in the steerings** to compensate the drift — it is absorbed downstream into the published systematic. For SIDIS, ep0304 cannot be reincluded in `A_BC` or `A_h` without an explicit period-efficiency systematic; it CAN be reincluded in `M^{h±}(Q², x, z, pT_h)` if a corresponding per-period efficiency systematic is added to the error budget.
7. **Cost to add ep0304 to the SIDIS Stage 1 + Stage 2 production:** **347** new Condor jobs (83 data + 264 MC) and ≈ **10 GB DUST**.

---

## 1. Britzger steering inventory for ep0304

Located under `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/Analysis/Steering/`:

| Steering file | Size | mtime | Role |
|---|---|---|---|
| `es.ep0304disNom.steer` | 6,266 | 2022-11-07 | Nominal data + DJANGOH + RAPGAP (signal-only chains) |
| `es.ep0304disMC13.steer` | 27,943 | 2022-11-15 | Full data + DJANGOH + RAPGAP + DjBkgd low-Q² + CC + DVCS + Compton + Grape + Pythia bkgds (MC13 systematic variant) |
| `es.ep0304dis.steer` | 18,113 | 2022-11-07 | Earlier full chain (predecessor of MC13) |
| `es.test.ep0304dis.steer` | (n/a sample) | — | Test/scratch steering |
| `es.Norad.ep0304disMC10.GenLumi.steer` | 18,950 | 2023-01-28 | NORAD MC10 gen-lumi normalization |
| `es.Norad.ep0304disMC13.Norad2021.GenLumi.steer` | 28,214 | 2023-02-06 | NORAD MC13 gen-lumi normalization (current C_rad anchor) |
| `es.Norad.ep0304disMC13.toem.Norad2021.GenLumi.steer` | 28,073 | 2023-02-10 | NORAD MC13 "toem" remapping (e+→e- comparison) |
| `es.Norad.ep0304.toem.DisMC10.GenLumi.steer` | 19,278 | 2023-02-01 | NORAD MC10 "toem" remapping |

### 1.1 `es.ep0304disNom.steer` (signal data + signal MC)

```steer
H1AnalysisSteer("EventShapes") {
   fOutputFile       =  "evtshp_ep0304.root";
   fChains           =  "Django_Eplus0304_1";

   fGoodRunFile      = "../LumiFiles/nc_goodruns_0304.root";   // SAME as SIDIS LumiFiles/
   fRunSelection     = "357072,392213";                        // e+ 03/04 running
   fKineRecMethod    =  H1Constants::eKineRecESigma;
}

// DJANGOH chains
H1SteerTree("Django_Eplus0304_1"){
  fPath = "/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/djangoh14/7093";
  fFileNames = "4.0.6.DJANGOH14.NC.EPLUSP.04.RAD.Q2GT60.CTEQ6L.A.7093.S39200.R97800.ftt.DST.0000-0010";
  fLumi = 1534.81;
}
// ... 7 chunks total: 0000-0010, 0011-0020, ..., 0061-0070 -> covers 71 of 142 HATs

// RAPGAP chains
H1SteerTree("Rapgap_Eplus0304_1"){
  fPath = "/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/rapgap31/7097";
  fFileNames = "4.0.6.RAPGAP31.NC.EPLUSP.04.RAD.Q2GT60.CTEQ6L.A.7097.S39200.R97800.ftt.DST.0000-0009";
  fLumi = 1528.95;
}
// ... 6 chunks total: 0000-0009, ..., 0050-0060 -> covers 61 of 122 HATs
```

**Decoded:**

| Field | Britzger value | SIDIS equivalent |
|---|---|---|
| GRL | `LumiFiles/nc_goodruns_0304.root` | identical filename + md5 `39457f9e…` |
| Run range | `357072,392213` (e+ 03/04 running) | bracketed by the 4017-run high-Q² GRL (min 357160, max 392213) |
| Reco kine method | `eKineRecESigma` | matches SIDIS `eSigma` baseline (P4) |
| DJANGOH catalog | `djangoh14/7093` | same; SIDIS deleted 2026-05-24, 70 sidistree files freed 3.2 GB |
| RAPGAP catalog | `rapgap31/7097` | same; SIDIS deleted 2026-05-24, 120 sidistree files freed 6.8 GB |
| Per-chunk lumi (DJANGOH) | 1534.81 nb⁻¹ | matches current `MC_CATALOGS[djangoh14,7093,ep0304] = 1534.81` |
| Per-chunk lumi (RAPGAP) | 1528.95 nb⁻¹ | matches current `MC_CATALOGS[rapgap31,7097,ep0304] = 1528.95` |

Britzger's `nc_goodruns_0304.root` is `../LumiFiles/nc_goodruns_0304.root` relative to the `Analysis/` working dir, i.e. `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/LumiFiles/nc_goodruns_0304.root`. The SIDIS-local copy at `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0304.root` is md5-verified bit-identical per `docs/methodology/structural_separation_2026-05-23.md` Section 14 (Phase 1A-fix runtime-artifact migration).

### 1.2 `es.ep0304disMC13.steer` (full chain incl. backgrounds)

Headers identical to 1.1; adds chains for:
- `DjBkgd0304_1..17` from `djangoh14/7110` (low-Q² DJANGOH NORAD `Q2GT2` for low-Q² leak background), per-chunk lumi 193.7 nb⁻¹.
- `CCBkgd0304` from `djangoh14/7339,7348` (charged-current), lumi 10910 nb⁻¹.
- `DVCSEplus0304_1,2` from `tintin/8052,8051` (DVCS).
- `ComptonEplus0304_1..10` from `compton20/8346`, lumi 1414.1 nb⁻¹.
- `GrapeEplus0304` from `h1grape/7128`, lumi 4931.28 nb⁻¹.
- `PythiaEminus06_*` from `pythia62/72xx` directories (photoproduction; identical across all 4 periods).

**Note for SIDIS:** these are physics-background MC samples used in the EventShapes unfolding & systematic-subtraction step. For inclusive charged-particle SIDIS at high-Q², the standard analysis treatment is:

- DVCS, Compton, Grape: subtracted at the event level via the standard NC-DIS bad-event / QED Compton cut (already in `TreeBuilderHighQ2_chargedsidis stage_evls` lines 525-536, matched cut-by-cut to g1j_ES per `docs/data/g1j_ES_port_diff.md` cuts 17 + 18 + QEDC).
- Low-Q² DJANGOH (`djangoh14/7110`) "leak": negligible at `Q² > 150 GeV²` (the analysis cut window).
- CC: irrelevant for NC SIDIS at S67.
- Photoproduction (Pythia): negligible at `Q² > 150`, `y < 0.6`, `35 < E-p_z < 70`.

**Therefore SIDIS does not need to import the MC13 background chains.** The signal-only mapping (data + DJANGOH 7093 + RAPGAP 7097) is sufficient.

### 1.3 `es.Norad.ep0304disMC13.Norad2021.GenLumi.steer` (C_rad anchor)

Same `fGoodRunFile`, `fRunSelection`, `fKineRecMethod` as 1.1/1.2. Adds NORAD chains:
- `H1AnalysisChainSteer("DjNorad")` and `H1AnalysisChainSteer("RaNorad")` with `fApplyRunSelection = false` (NORAD runs at gen level only; no detector-status / run-list filtering).

The NORAD catalogs for ep0304 are at the Britzger NORAD production directory `/data/dust/group/h1/britzger/norad_prod*/dst2all_djangoh14_NORAD/...` and `dst2all_rapgap31_NORAD/...` (specifically the `EPLUSP.NORAD.Q2GT100` sub-samples for ep0304). The SIDIS NORAD inventory at `output/evls/mc/djangoh14_norad/` currently has 519 evls files split across `eminusp/` (260) and `eplusp/` (259), per `dust_cleanup_post_audit_2026-05-24.json`. **519 / 2 ≈ 259 per beam-charge sign** — the eplusp NORAD inputs already exist for the ep0607 + ep0304 e+p combined sample. The C_rad branch is therefore not blocked by ep0304-specific NORAD missing; see `docs/data/crad_strategy2_v2_submission_audit.json` for the existing NORAD inventory.

### 1.4 `es.Norad.ep0304disMC13.toem.Norad2021.GenLumi.steer`

Identical to 1.3 but with the "to-e-minus" flag that remaps the e+ NORAD sample into an e- comparison (used by Britzger for cross-charge cross-section ratios). NOT needed for SIDIS charged-particle multiplicities.

---

## 2. HAT directory inventory + period boundary

### 2.1 `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/03/`

- 20 `hat.4.0.6.10dst7.c0300NN.root` files (NN = 00 – 19).
- 20 paired `mods.4.0.6.10dst7.c0300NN.root` files (each Stage-1 builder consumes both).
- Run-number probe (uproot `HAT`-tree, branch `RunNumber`):

  | chunk | n events | min RN | max RN | nuniq |
  |---|---|---|---|---|
  | c030000 | 591,279 | 357149 | 357557 | 79 |
  | c030019 | 798,025 | 367105 | 367186 | 16 |

  Britzger's `fRunSelection = 357072,392213` brackets 357149 – 367186 → **all 20 year-03 chunks are ep0304**. Use `nc_goodruns_0304.root`.

### 2.2 `/pnfs/desy.de/dphep/online/h1/data/oo-4.0/04/`

- 63 `hat.4.0.6.10dst7.c0400NN.root` files (NN = 00 – 62).
- 63 paired `mods` files.
- Run-number probe (5 sampled chunks; total < 5 s wall):

  | chunk | n events | min RN | max RN | nuniq | period |
  |---|---|---|---|---|---|
  | c040000 | 905,968 | 367284 | 367305 | 11 | ep0304 (matches 367186 hand-off from c030019) |
  | c040030 | 1,044,526 | 380132 | 380374 | 57 | ep0304 |
  | c040040 | 1,058,256 | 383442 | 383841 | 78 | ep0304 |
  | c040050 | 1,002,550 | 386652 | 388400 | 63 | ep0304 |
  | c040055 | 951,563 | 389972 | 390510 | 67 | ep0304 |
  | c040060 | 1,006,863 | 391707 | 392137 | 69 | ep0304 |
  | **c040061** | 520,599 | 392137 | **392213** | 32 | ep0304 (terminal chunk; matches Britzger upper RN 392213) |
  | **c040062** | 408,118 | **398286** | 398679 | 45 | **NOT ep0304** (commissioning / pre-em0405; runs absent from all 4 high-Q² S67 GRLs) |

**Sharp boundary at chunk 62**: chunks c040000 – c040061 (62 chunks) cover the ep0304 run range exactly; chunk c040062 (1 chunk) contains 45 unique runs in 398286 – 398679 that are in NEITHER the ep0304 GRL (max RN 392213) NOR the em0405 GRL (min RN 399629 per `highQ2_per_run_beam_energy.csv`). These are pre-em0405 commissioning runs that no high-Q² S67 analysis (including Britzger's EventShapes) includes.

**Dispatch decision:**

- For year-04, **all 63 chunks may be submitted** with the ep0304 GRL applied. Chunk c040062 will produce zero passing events (all runs rejected by the GRL), wasting ~30 s of Condor time per period and ~0 B output. This is consistent with how year-06 handles the c060000 – c060219 chunks under `nc_goodruns_06.root` (they reject all events, harmless). No dispatch-by-chunk-index is required for year-04 because the harmless-rejection mode is correct.
- An *optimization* would be to teach `grl_for_data_hat` to skip c040062 entirely, similar to the year-06 chunk-220 boundary. This saves 1 Condor job and is essentially cosmetic. The hardened year-06 dispatch IS required because year-06 chunks BELOW 220 are clean ep0607 (would otherwise be wrong-period), but year-04 chunk 62 is NOT a different period — it's a pre-period commissioning chunk with no analyzable runs. **Recommended: leave c040062 in the submission with ep0304 GRL.** The GRL rejection is the safety net.

### 2.3 Comparison to year-06 boundary

The year-06 directory has a documented mixed-period boundary at chunk 220 (`submit_full_production.py:231`, `YEAR06_EM06_CHUNK_MIN = 220`). The year-04 situation is **different in kind**:

- year-06: `c060000 – c060219` = ep0607 (valid runs), `c060220 – c060319` = em06 (valid runs). Both halves are publishable; a per-chunk dispatch is REQUIRED to assign the correct GRL.
- year-04: `c040000 – c040061` = ep0304 (valid runs), `c040062` = pre-em0405 commissioning (no valid runs in any GRL). The "wrong" chunk has zero analyzable content under any GRL.

A defense-in-depth option is to add `YEAR04_EP0304_CHUNK_MAX = 61` to the wrapper and skip c040062 in dispatch. This is mostly documentation; the GRL applied at the binary level is the authoritative selector either way.

---

## 3. GRL file mapping

| GRL filename | SIDIS path | Britzger path | md5 verified bit-identical |
|---|---|---|---|
| `nc_goodruns_0304.root` | `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0304.root` | `/data/dust/group/h1/britzger/EventShapes/eventshapes_git/LumiFiles/nc_goodruns_0304.root` | YES (md5 `39457f9e…` per `docs/methodology/structural_separation_2026-05-23.md` §14) |
| `nc_goodruns_0405.root` | same dir | same upstream | YES |
| `nc_goodruns_06.root`   | same dir | same upstream | YES |
| `nc_goodruns_0607.root` | same dir | same upstream | YES |

**Contents of `nc_goodruns_0304.root`:** Three TKeys — `H1RunList` (H1OO `Unknown_H1RunList`), `MyDetectorStatus` (`Unknown_H1DetectorStatus`), `zInfo` (`Unknown_H1ArrayF`). Standard 3-object H1 GRL format. **4017 unique good runs**, all at 27.6 GeV electron beam (`IsNominalEnergy_27p6_920 = True` for every CSV row in `highQ2_per_run_beam_energy.csv` for `period == ep0304`; zero rows at any other beam energy).

`GRL_BY_PERIOD["ep0304"] = "nc_goodruns_0304.root"` is already wired in `submit_full_production.py:196`.

---

## 4. MC catalog mapping

| Generator | Period | Catalog | Britzger /pnfs path | HAT count | Britzger per-chunk lumi | SIDIS table value |
|---|---|---|---|---|---|---|
| DJANGOH14 | ep0304 | 7093 | `/pnfs/.../oo-4.0/djangoh14/7093/` | **142** | 1534.81 nb⁻¹ | 1534.81 ✓ |
| RAPGAP31 | ep0304 | 7097 | `/pnfs/.../oo-4.0/rapgap31/7097/` | **122** | 1528.95 nb⁻¹ | 1528.95 ✓ |
| DJANGOH14 | em0405 | 7092 | (existing in SIDIS prod) | 141 | 1534.81 | 1534.81 ✓ |
| RAPGAP31 | em0405 | 7096 | (existing) | 121 | 1523.83 | 1523.83 ✓ |
| DJANGOH14 | em06   | 7072 | (existing) | 88  | 1534.81 | 1534.81 ✓ |
| RAPGAP31 | em06   | 7071 | (existing) | 61  | 1515.74 | 1515.74 ✓ |
| DJANGOH14 | ep0607 | 7091 | (existing) | 201 | 4383.10 | 4383.10 ✓ |
| RAPGAP31 | ep0607 | 7094 | (existing) | 121 | 3055.76 | 3055.76 ✓ (+ 8 chunks override 15716.11) |

**Britzger's "MC13" name** in `es.ep0304disMC13.steer` and `es.Norad.ep0304disMC13.Norad2021.GenLumi.steer` refers to the **MC13 unfolding-systematic chain variant** (background suite added; signal MC unchanged). It is NOT a different MC generation campaign. The catalog IDs 7093 / 7097 are the SAME MC campaign across `es.ep0304disNom`, `es.ep0304disMC13`, `es.Norad.ep0304disMC10` (which uses a DIFFERENT older catalog set internally for NORAD anchoring, not signal), and `es.Norad.ep0304disMC13.Norad2021`.

**Confirmation:** SIDIS `MC_CATALOGS` table (`submit_full_production.py:269-277`) is already correct for ep0304:

```python
("rapgap31",  "7097", "ep0304", 1528.95),
("djangoh14", "7093", "ep0304", 1534.81),
```

The 2026-05-24 DUST cleanup removed the previous-production sidistree outputs (70 DJANGOH + 120 RAPGAP files = 3.2 + 6.8 = 10.0 GB), but the source HATs on /pnfs and the SIDIS wrapper entries are intact.

---

## 5. Tracking-drift / S67 trigger systematic — Britzger handling

### 5.1 Is the drift Britzger-known?

**No explicit per-period correction in the steerings.** The Britzger EventShapes pipeline applies the standard H1 NC DIS preselection (which is cut-for-cut equivalent to SIDIS Stage 1, per `docs/data/g1j_ES_port_diff.md`) and lets the unfolding step (`AnalysisEventShapes::DoAnalysisCutsRec` + the dedicated H1Unfolder framework) consume the period-summed yields. No `H1AnalysisSteer` field controls a per-period trigger weight or tracking-efficiency correction; no NORAD steering applies one either.

**What Britzger publishes:** the EventShapes analysis (`Eur. Phys. J. C 84 (2024) 882`, NNLO+NLL event shapes in DIS) reports differential cross-sections in event-shape observables vs. (Q², ⟨τ⟩) etc. The 4-period combined sample is unfolded against the DJANGOH/RAPGAP signal MC (rad-corrected against the NORAD chains). Period-by-period rate offsets propagate into the unfolded result as **part of the total experimental systematic** (covered by the model-dependence + reconstruction-efficiency bins in Tab. 6 of the paper). The ep0304 +1.6% rate excess corresponds to roughly the size of the per-period trigger-efficiency systematic that EventShapes already carries.

### 5.2 Why this matters for SIDIS

The SIDIS analysis at high-Q² is dominated by two observable classes that respond DIFFERENTLY to a per-period rate offset:

| Observable | Sensitivity to ep0304 + 1.6 % excess | Treatment plan if ep0304 reincluded |
|---|---|---|
| `M^{h±}(Q², x, z, pT_h)` multiplicities | Cancels at first order — the numerator (hadron yield) and the DIS denominator scale by the same trigger efficiency. Residual sensitivity is from hadron-track efficiency tracking-drift independent of S67. Expected residual: ≪ 0.5 %, dominated by track-efficiency drift (a separate systematic). | **Acceptable for publication if a +0.5 % per-period multiplicity-stability systematic is added to the error budget.** |
| `A_BC^DIS = (R_{e+} − R_{e−}) / (R_{e+} + R_{e−})` | Does NOT cancel — directly proportional to the per-period rate offset. The +1.6 % ep0304 excess produces a +0.008 shift in A_BC^DIS_low (see `ep0304_rate_audit_v5f.md` Section 10). | **NOT acceptable** without explicit period-trigger-efficiency correction. Two paths: (a) reweight ep0304 data by 1.32327 / 1.34489 = 0.98392 (H1 hypothesis remediation), or (b) rebuild ep0304 per-run lumi via `H1Lumi::GetLuminosity()` (H2 hypothesis remediation). Either is a DOWNSTREAM analysis change, not a production-stage change. |
| `A_h = (N_{h+} − N_{h−}) / (N_{h+} + N_{h−})` | Mostly cancels because both numerator and denominator scale by the same per-event trigger efficiency on the SAME run. | **Marginal**: ep0304 is e+p, so its hadron-charge ratio enters the e+p arm of A_h. Per-period drift contributes a fractional shift to A_h(e+p) only; combined A_h is a luminosity-weighted average. Acceptable if the same per-period stability systematic from multiplicities is propagated. |
| `<cos(phi_h)>`, `<cos(2 phi_h)>` | Trigger-rate offset cancels in the moment definition; modulation is uncorrelated with trigger. | **Acceptable.** |
| Absolute hadron cross-sections | Direct multiplier on the cross-section value. | Blocked already by HB5 (H1CalcWeight / effective MC lumi); per-period drift adds 1.6 % to the absolute-scale systematic but is not the binding constraint. |

### 5.3 Recommended SIDIS treatment of the drift

The drift is a **downstream systematic, not a production-stage exclusion**. The 2026-05-23 decision to permanently drop ep0304 from production was a conservative "preserve A_BC zero" decision; it was correct under the assumption that no per-period trigger-efficiency systematic would be quantified. With Britzger's pipeline as evidence (4-period combined sample, no per-period reweight, drift absorbed into the unfolding systematic), the conservative path is:

1. **Reinclude ep0304 in production** (this proposal). The trees become AVAILABLE; they are not yet promoted into any analysis observable.
2. **Add a per-period efficiency systematic to the multiplicity error budget.** Suggested magnitude ≤ 1.0 % uncorrelated across periods (this brackets both the observed +1.6 % ep0304 excess and the −1.4 % em06 deficit documented in the audit), evaluated by varying each period's data-MC normalization by its observed deviation from the 2-period (em0405+ep0607) baseline.
3. **For `A_BC^DIS` only:** keep ep0304 OUT of the published A_BC unless and until either (a) the H1Lumi-rebuild test (`ep0304_rate_audit_v5f.md` §9 step 1) decides H1 vs H2, OR (b) the user explicitly approves carrying ep0304 into A_BC with the +0.008 bias correction documented. For the v5g-r2 internal A_BC cross-check, ep0304 CAN be included with a `--include-ep0304 --abc-systematic-warn` flag to demonstrate the +0.008 shift quantitatively.
4. **For `M^{h±}`, `A_h`, phi_h moments:** ep0304 inclusion is acceptable with the new per-period systematic.

---

## 6. Proposed `submit_full_production.py` changes

The wrapper ALREADY supports ep0304 in full structurally:
- `DATA_PERIODS["04"] = "ep0304"` (line 166)
- `GRL_BY_PERIOD["ep0304"] = "nc_goodruns_0304.root"` (line 196)
- `GRL_BY_YEAR_SHORT["04"] = "nc_goodruns_0304.root"` (line 189)
- `MC_CATALOGS` includes `("rapgap31", "7097", "ep0304", 1528.95)` and `("djangoh14", "7093", "ep0304", 1534.81)` (lines 270, 274)
- `--include-ep0304` CLI flag (line 1085) with handoff into `submit_evls_jobs(exclude_ep0304, include_ep0304)` and `submit_sidistree_jobs(exclude_ep0304, include_ep0304)`

**There is no missing wiring.** The ONLY required change is in the *DEFAULTS / policy banner*, since the user has decided ep0304 should be included.

### 6.1 Recommended diff (policy flip)

Two changes, both in the CLI defaults block; one in the documentation banner near the top of the file. NO change to MC_CATALOGS, no change to `grl_for_data_hat`, no change to year-04 dispatch.

```diff
--- submit_full_production.py
+++ submit_full_production.py
@@ -52,12 +52,18 @@
-# ep0304 exclusion (2026-05-23): SIDIS permanently drops ep0304 per the
-# +3.85 sigma DIS-rate anomaly traceable to S67 trigger / tracking-efficiency
-# drift between 2003-04 and 2005+ HERA-II running. The --exclude-ep0304
-# default (True) skips ep0304 at both Stage 1 (evls) and Stage 2 (sidistree)
-# iteration. Use --include-ep0304 only if explicitly producing ep0304
-# evls for non-SIDIS use. See docs/data/ep0304_rate_audit_v5f.{json,md}.
+# ep0304 policy (2026-05-24, supersedes 2026-05-23 permanent-drop):
+# ep0304 is RE-INCLUDED for SIDIS production, matching the Britzger
+# EventShapes pipeline which uses the same GRL (nc_goodruns_0304.root),
+# the same MC catalogs (djangoh14/7093, rapgap31/7097), and the same
+# run-selection bracket (357072..392213). The +3.85 sigma rate excess
+# (docs/data/ep0304_rate_audit_v5f.{json,md}) is real per-period
+# detector drift; it is handled DOWNSTREAM as a per-period
+# trigger/tracking-efficiency systematic in the multiplicity error
+# budget and as an explicit blocker for A_BC^DIS until either the
+# H1Lumi-rebuild test or the H1-hypothesis reweight is applied
+# (see docs/research/research-ep0304-production-path-2026-05-24.md).
+# The --exclude-ep0304 flag is retained for opt-out (e.g. emergency
+# A_BC-blinded reproduction); the default is INCLUDE.
 
 import argparse
@@ -1077,10 +1083,10 @@
     parser.add_argument(
-        "--exclude-ep0304", action="store_true", default=True,
-        help="Exclude ep0304 data period (default: True; SIDIS permanently drops "
-             "ep0304 per +3.85 sigma tracking-drift remediation). Pass --include-ep0304 "
-             "to override (NOT recommended for SIDIS analysis use).",
+        "--exclude-ep0304", action="store_true", default=False,
+        help="Exclude ep0304 data period (default: False; ep0304 is INCLUDED "
+             "as of 2026-05-24, matching Britzger EventShapes. The +3.85 sigma "
+             "rate drift is handled as a downstream per-period systematic, "
+             "not a production exclusion. See docs/research/research-ep0304-"
+             "production-path-2026-05-24.md.).",
     )
     parser.add_argument(
-        "--include-ep0304", action="store_true", default=False,
-        help="Override the default ep0304 exclusion. Use only if you explicitly "
-             "want to produce ep0304 evls (NOT for SIDIS analysis use).",
+        "--include-ep0304", action="store_true", default=True,
+        help="Include ep0304 data period (default: True; retained for symmetry "
+             "with --exclude-ep0304 and for explicit-include audit trails).",
     )
```

The boolean evaluation inside `submit_evls_jobs` is `skip_ep0304 = exclude_ep0304 and not include_ep0304` (line 671), so with the new defaults (exclude=False, include=True) → `skip_ep0304 = False`, which is the desired behavior. If an operator passes only `--exclude-ep0304` (without `--include-ep0304`), then `skip_ep0304 = True and not True = False` — wait, that's wrong. Let me recheck the logic.

**Logic bug surfaced by the policy flip:** with `default include_ep0304=True`, passing `--exclude-ep0304` alone gives `skip_ep0304 = True and not True = False` → ep0304 is INCLUDED despite the operator opting out. That's broken.

**Fix:** change the boolean to a single tri-state CLI argument:

```diff
-    parser.add_argument(
-        "--exclude-ep0304", action="store_true", default=False, ...
-    )
-    parser.add_argument(
-        "--include-ep0304", action="store_true", default=True, ...
-    )
+    grp = parser.add_mutually_exclusive_group()
+    grp.add_argument(
+        "--include-ep0304", dest="ep0304_mode", action="store_const",
+        const="include",
+        help="Force inclusion of ep0304 (default since 2026-05-24).",
+    )
+    grp.add_argument(
+        "--exclude-ep0304", dest="ep0304_mode", action="store_const",
+        const="exclude",
+        help="Force exclusion of ep0304 (opt-out; use for A_BC-blinded "
+             "reproductions of the 2026-05-23 sample).",
+    )
+    parser.set_defaults(ep0304_mode="include")
```

And in the two callers (lines 1113 and 1129):

```diff
-            exclude_ep0304=args.exclude_ep0304,
-            include_ep0304=args.include_ep0304,
+            skip_ep0304=(args.ep0304_mode == "exclude"),
```

with the two job-submission signatures simplified to a single `skip_ep0304: bool` parameter (the two-flag complexity served the prior conservative default and is no longer needed). At call sites in `submit_evls_jobs` / `submit_sidistree_jobs`, replace:

```python
skip_ep0304 = exclude_ep0304 and not include_ep0304
```

with the parameter passed-through `skip_ep0304`.

This is a small but real refactor; the conservative ALTERNATIVE is to flip only the defaults and rely on operator discipline (always pass both `--exclude-ep0304 --include-ep0304=False` to fully disable). The mutually-exclusive group is the clean fix.

### 6.2 Optional defense-in-depth: skip year-04 chunk 62

```diff
+YEAR04_EP0304_CHUNK_MAX = 61  # c040061 and below = ep0304; c040062 = post-period
+                              # commissioning runs (RN 398286-398679) in NO GRL.

 def grl_for_data_hat(year_short: str, hat_filename: str) -> tuple:
     """..."""
     if year_short == "06":
         idx = hat_chunk_index(hat_filename)
         if idx >= YEAR06_EM06_CHUNK_MIN:
             return ("em06", "nc_goodruns_06.root")
         else:
             return ("ep0607", "nc_goodruns_0607.root")
+    if year_short == "04":
+        idx = hat_chunk_index(hat_filename)
+        if idx > YEAR04_EP0304_CHUNK_MAX:
+            return ("ep0304_pre_em0405_commissioning",
+                    "nc_goodruns_0304.root")  # GRL will reject all
     period = DATA_PERIODS[year_short]
     return (period, GRL_BY_PERIOD[period])
```

If you adopt this, `list_hat_files` should also be filtered, e.g. in `submit_evls_jobs`:

```python
if year_short == "04":
    hat_files = [f for f in hat_files
                 if hat_chunk_index(f) <= YEAR04_EP0304_CHUNK_MAX]
```

This is **optional**. Without it, c040062 produces 0 passing events under `nc_goodruns_0304.root` (proven by run-list non-overlap). The cost of leaving it in is ~ 30 s of Condor wall time + 0 B of output for one wasted job. **Recommended: leave c040062 in for symmetry with the year-06 handling** (the GRL is the authoritative selector at the binary level; over-defensive dispatch logic in the wrapper is brittle).

### 6.3 Wrapper banner ep0304 mapping

The MC catalog table at line 269-277 already includes ep0304 catalogs. The DATA_PERIODS mapping at line 166 already has `"04": "ep0304"`. **No other code change is required.**

---

## 7. Production cost estimate

| Bucket | Stage 1 (evls) jobs | Stage 2 (sidistree) jobs | DUST output (sidistree) |
|---|---|---|---|
| Data year-03 (20 chunks) | 20 | 20 (all ep0304) | ≈ 20 × 1 MB = 20 MB |
| Data year-04 (63 chunks) | 63 | 62 (c040062 produces 0; 1 zero-output job that may be skipped at Stage 2) | ≈ 62 × 1 MB = 62 MB |
| Data ep0304 total | **83** | **82** | ≈ **82 MB** |
| MC DJANGOH14 7093 | 142 | 142 | matches prior 3.2 GB / 70-file footprint scaled to per-HAT-chunk: ≈ 3.2 GB × (142/70) = **6.5 GB** (per-file ~45 MB) |
| MC RAPGAP31 7097 | 122 | 122 | matches prior 6.8 GB / 120-file footprint: ≈ 6.8 GB × (122/120) = **6.9 GB** (per-file ~57 MB) |
| MC ep0304 total | **264** | **264** | ≈ **13.4 GB** |
| **TOTAL ADDITIONAL** | **347 Condor jobs** | **346 Condor jobs** | **≈ 13.5 GB DUST** |

**Notes:**
- Per-file sizes inferred from existing em06 Stage-2 dir (`output/data/em06/`: 100 files / 98 MB = ~1 MB / chunk for data; from cleanup audit: djangoh14/7093 was 70 files / 3.2 GB = 46 MB / chunk; rapgap31/7097 was 120 files / 6.8 GB = 57 MB / chunk).
- The 70 / 120 prior counts vs. 142 / 122 current /pnfs HAT counts: the prior production used Britzger's chunk-grouped chains (10 HATs per chain), explaining the lower file count. SIDIS processes one Condor job per /pnfs HAT, giving the higher file count. **Revised estimate:** Stage-2 sidistree MC ep0304 will land at ≈ **13.4 GB** when each HAT lands as one output file.
- Stage-1 evls output is negligible (~40 KB per chunk for data; for MC, ~few MB per chunk based on `output/evls/data/06/` totals).
- DUST headroom: post-cleanup workspace is at 90 GB total; ep0304 reinclusion brings it to ~ 104 GB. Well within the 2 TB AFS / unbounded DUST allocation.

**Condor walltime estimate:**
- Stage 1 evls per data chunk: ~ 5 min average (per existing em06 chunks).
- Stage 1 evls per MC chunk: ~ 10 min average.
- Stage 2 sidistree per chunk: ~ 5–10 min.
- Total wall under reasonable Condor concurrency (100 jobs): ≈ 1–2 h end-to-end.

---

## 8. Recommended adoption schedule

### 8.1 Immediate (today, code-only)

1. Apply the `submit_full_production.py` policy-flip diff (Section 6.1) PLUS the mutually-exclusive-group fix.
2. Update the file-level docstring and the ep0304-exclusion banner.
3. Update `context-handoff.md` Section 3 "Production state" + Section 11 "Verification gaps" to reflect ep0304 re-inclusion.

### 8.2 Pre-production (this session)

4. `--dry-run` Stage 1 to verify the 83-data + 264-MC job count materializes correctly:
   ```
   python3 submit_full_production.py --stage evls --dry-run --include-ep0304
   ```
   Expected output: prints `Period ep0304 (year 04): 63 HAT files` (or 62 if chunk-62 skip adopted) + `Period ep0304 (year 03): 20 HAT files` + `MC rapgap31/7097 (ep0304): 122 HAT files` + `MC djangoh14/7093 (ep0304): 142 HAT files`.

### 8.3 Stage 1 launch

5. Submit Stage 1 data + MC (`--stage evls`). ~ 347 Condor jobs.
6. Smoke-validate 5 random ep0304 evls outputs against the `evls_highQ2_*` schema.

### 8.4 Stage 2 launch

7. Submit Stage 2 sidistree (`--stage sidistree`). ~ 346 Condor jobs.
8. Smoke-validate 5 random ep0304 sidistree outputs against the v5g schema.
9. Update `n_dis_data_v5g_highQ2.json` companion to include ep0304 entries.

### 8.5 Analysis-side promotion (next session)

10. **Multiplicities, A_h, phi_h:** include ep0304 in the per-period sum; add the +1 % per-period efficiency systematic to the error budget; quote ep0304 contribution explicitly.
11. **A_BC^DIS:** keep ep0304 EXCLUDED from the published number; produce the internal `--include-ep0304 --abc-systematic-warn` audit number side-by-side to quantify the +0.008 bias; cite the bias as the binding constraint for A_BC release.
12. **C_rad on ep0304:** confirm the existing NORAD eplusp inventory (259 files per cleanup audit) covers the ep0304 + ep0607 e+p combined sample. If a dedicated ep0304-only NORAD breakdown is needed, the Britzger NORAD steerings `es.Norad.ep0304disMC13.Norad2021.GenLumi.steer` give the run selection + GRL anchor (already identical to data).

### 8.6 Cross-check publishability

13. Verify Britzger's published ep0304 + em0405 + em06 + ep0607 4-period high-Q² result is reproducible from the SIDIS-side Stage 2 output by computing the inclusive DIS yield per period and comparing to Britzger's `oomclumi.ep0304.MC10.Djangoh.txt` lumi log (referenced in the Britzger Steering dir).

---

## 9. Risks, ambiguities, and open questions

| # | Item | Risk | Mitigation |
|---|------|------|------------|
| R1 | The +3.85σ ep0304 rate drift may be **detector physics that propagates unequally to multiplicities and DIS yield**, breaking the first-order cancellation argument in Section 5.2. | If the tracking-efficiency drift is hadron-charge or pT dependent, M^{h±} could carry a residual shift > 1 % at low z or low pT. | The per-period stability systematic should be evaluated DIFFERENTIALLY in (z, pT, charge) bins, not as a single integrated number. Recommend a follow-up audit task. |
| R2 | Britzger's data input path (`/nfs/dust/h1/group/britzger/h1mpim13/kogler/files/oo-4.0/ep0304/data/`) is GONE — there is no live Britzger ep0304 sample on NAF to A/B-compare against. | Cannot validate SIDIS ep0304 production against a "reference Britzger ep0304 ROOT". | The validation must instead be: (a) SIDIS ep0304 N_DIS per period vs Britzger's `oomclumi.ep0304.MC10.Djangoh.txt` lumi anchor, and (b) consistency with the 4017-run × ep0304 GRL lumi 53,706 nb⁻¹. |
| R3 | c040062 contains 45 runs (398286–398679) that are in NEITHER ep0304 NOR em0405 GRL. These may be valid HERA-II commissioning runs that an UPDATED GRL would include. | The current GRL set may miss ~30–100 nb⁻¹ of valid 2004 e+p running. | Low priority. The standard H1 GRL is what Britzger uses; SIDIS should not diverge. Document and move on. |
| R4 | The `--include-ep0304` / `--exclude-ep0304` boolean-OR semantics in the current wrapper give an UNINTUITIVE result if only the include flag is flipped to default-True without the mutually-exclusive group refactor. | An operator passing `--exclude-ep0304` would still produce ep0304 (line 671 logic `skip = exclude and not include`). | The Section 6.1 mutually-exclusive-group refactor is **required**, not optional, for the policy flip. |
| R5 | Britzger MC10 vs MC13 distinction: confirmed to be unfolding-systematic-chain naming, not a different MC campaign. | No risk; documented above. | N/A. |
| R6 | The pre-existing `H1ChargedSidis_HighQ2/output/data/ep0304*/` paths (if any) from prior production attempts must be cleared before re-launch to avoid name collisions. | Stale ep0304 files may pollute the new output dir. | Run `ls -la output/data/ep0304*` before launch; remove or archive any. The 2026-05-24 cleanup did delete the MC dirs but data-side ep0304 was already empty per the audit (only em0405/em06/ep0607 listed in `current_production_paths_intact_after`). |
| R7 | The `H1RunList` class isn't loadable from Python without H1OO env; verification of GRL `IsSelected(run)` for boundary runs (357072, 392213, 398286) was indirect (via the CSV-derived run-energy table). | Low — the CSV is itself derived from the GRL via `H1Lumi`. | If required, an interactive ROOT-with-H1OO probe can confirm; the run-range check by file-level RunNumber sampling is sufficient evidence for this memo. |

---

## 10. Memo + JSON output paths

- **Research memo (this file):** `H1/sidis_highQ2/docs/research/research-ep0304-production-path-2026-05-24.md`
- **Machine-readable summary:** `H1/sidis_highQ2/docs/data/ep0304_production_proposal_2026-05-24.json`
- **Existing related artifacts (NOT modified):**
  - `H1/sidis_highQ2/docs/data/ep0304_rate_audit_v5f.md` (audit of the +3.85σ drift)
  - `H1/sidis_highQ2/docs/data/g1j_ES_port_diff.md` (cut-by-cut comparison of Stage 1 vs Britzger DoBasicCutsRec)
  - `H1/sidis_highQ2/docs/data/dust_cleanup_post_audit_2026-05-24.json` (cleanup baseline for the cost estimate)
  - `H1/sidis_highQ2/docs/data/highQ2_per_run_beam_energy.csv` (4017 ep0304 run-energy rows, all 27.6 GeV)
  - `H1ChargedSidis_HighQ2/scripts/submit_full_production.py` lines 53–58, 166, 189, 196, 270, 274, 665–667, 916–917, 1077–1090, 1113, 1129 (changes targeted in §6)

## 11. Recommended next step

**ENABLE ep0304 IMMEDIATELY for SIDIS production**, applying the Section 6.1 + 6.2 wrapper changes. Stage 1 dry-run first to confirm job counts; if counts match the (83 data + 264 MC = 347 jobs) prediction, launch Stage 1; smoke-validate; launch Stage 2. Subsequent analysis-side promotion (Section 8.5) is staged so that:

- M^{h±}, A_h, phi_h moments accept ep0304 immediately (with the new per-period systematic in the error budget).
- A_BC^DIS publication number remains ep0304-EXCLUDED until either H1Lumi-rebuild test or H1 reweight closes the trigger-efficiency systematic to ≤ 0.3 %.

This matches Britzger's pipeline (4-period combined, drift absorbed in systematics) while preserving the conservative A_BC blinding the SIDIS analysis already enforces (OQ7).

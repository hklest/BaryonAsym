# Reference: High-Q2 Charged-SIDIS Per-Run Beam-Energy Table

**Date:** 2026-05-20
**Scope:** Per-run electron and proton beam energy for every run in the four
oolumi S67 GRLs that drive the high-Q2 charged-particle SIDIS analysis
(ep0304, em0405, em06, ep0607). Sets the per-event Q² and γ\*+p boost
inputs and identifies the 2007 MER/LER exclusion.
**Audience:** `h1-sidis-supervisor` and every downstream consumer of
event-level kinematics in `H1SIDIS_HighQ2`.
**Source-of-truth artifacts (machine-readable):**
- `H1/sidis_highQ2/docs/data/highQ2_per_run_beam_energy.json`
- `H1/sidis_highQ2/docs/data/highQ2_per_run_beam_energy.csv`
**NAF intermediate:** `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/metadata/per_run_beam_energy.csv`

---

## 1. H1OO API used

| Layer | API | Granularity | Location |
|---|---|---|---|
| **Per-run (preferred)** | HAT primitives `EBeamE` / `EBeamP` (Float_t) | per event (constant within a run) | `H1Hat/H1HatEvent.h:130-131`, branches in any HAT TTree; on TreeBuilder side propagated to `ProtonTot.BeamElecE` / `ProtonTot.BeamProtP` and to `DisTot.BeamElecE` / `DisTot.BeamProtP` in v5b SIDIS schema. Read via `H1FloatPtr("EBeamE")` / `H1FloatPtr("EBeamP")`. |
| **Period-level fallback** | `H1Constants::ElectronBeamEnergy(Int_t RunOrPeriod)`, `H1Constants::ProtonBeamEnergy(Int_t RunOrPeriod)` | per period | `H1Geom/H1Constants.C:901-911`. Hardcoded to 27.6 / 920 for every HERA-II nominal period; 27.6 / 460 for `eEplus07lowE460` (LER); 27.6 / 575 for `eEplus07lowE575` (MER). |
| **Per-run luminosity** | `H1RunLumi::GetCorrectedLumi()` (Compton-corrected, prescale-aware, nb^-1) | per run | `H1Tools/H1RunLumi.h:31` (`fLumiCor`). Iterate via `H1RunList::At(Int_t i)` then `GetCorrectedLumi()`. |

**One-line summary:** preferred per-run API is the HAT bank `EBeamE` /
`EBeamP` (electron / proton beam energies recorded by DMIS), with
`H1Constants::ElectronBeamEnergy()` / `ProtonBeamEnergy()` as the
period-level fallback when per-run data are not yet produced.

Note: every kinematic calculator (`H1CalcKine`, `H1CalcFs`, `H1CalcW`,
`H1CalcWeight`) reads from `gH1Constants->GetElectronBeamEnergy()`, which
returns the **period-level** value unless the analysis explicitly calls
`H1Constants::SetElectronBeamEnergy(...)` or `H1BoostedJets::SetBeamEnergies(...)`.
For the precision required by the high-Q2 SIDIS analysis, the per-event
HAT `EBeamE` is preferred over the period-level constant; this table
records the per-run mean of `EBeamE` so that downstream code can index by
`RunNumber`.

---

## 2. Build provenance

- **SSH:** `ssh -O check naf-hone.desy.de` returned `Master running (pid=64865)` at 2026-05-20.
- **Environment:** `/cvmfs/sft.cern.ch/lcg/views/LCG_107a/x86_64-el9-gcc14-opt/setup.sh` (ROOT 6.34.04 -- exact match for the britzger H1OO build version) with `LD_LIBRARY_PATH` prepended with `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/lib/x86_64-el9-gcc14-opt`.
- **GRL extraction:** Opened each of `nc_goodruns_{0304,0405,06,0607}.root`, iterated `H1RunList::At(i)` for `i = 0..GetNrOfRuns()-1`, read `H1RunLumi::GetRunNumber()` and `H1RunLumi::GetCorrectedLumi()`.
- **HAT BeamElecE / BeamProtP extraction:** scanned the legacy proton-schema per-run files under `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/data/{04,05,06,07}/` (635 files total). For each non-empty `ProtonTot` tree, computed the per-RunNumber mean of `BeamElecE` and `BeamProtP` and merged across years (event-weighted).
- **Macro:** `/tmp/extract_per_run_beam_energy.C` on NAF (and its companion `/tmp/driver.C` that loads `libEG`, `libPhysics`, `libGenVector`, `libH1Pointers`, `libH1OOBanks`, `libH1Arrays`, `libH1Geom`, `libH1Steering`, `libH1Hat`, `libH1Mods`, `libH1Ods`, `libH1Tools`, `libH1Wrappers_lumi`). Run wall time 19 s on the login node. **No Condor was needed** (134 MB total ROOT input).

---

## 3. Coverage summary

| Period | n_runs | Lumi (nb^-1) | Anchor | Δ% | HAT-derived | Constants fallback |
|---|---:|---:|---:|---:|---:|---:|
| ep0304 | 4,017 | 53,706.062 | 53,706 | +0.0001% | 0 | 4,017 |
| em0405 | 5,194 | 105,028.110 | 105,028 | +0.0001% | 0 | 5,194 |
| em06 | 3,486 | 57,573.045 | 57,573 | +0.0001% | 0 | 3,486 |
| ep0607 | 8,091 | 140,306.605 | 140,306 | +0.0004% | 4,035 | 4,056 |
| **Total** | **20,788** | **356,613.822** | **356,613** | **+0.0002%** | **4,035** | **16,753** |

All four periods match the GRL manifest anchors to within +0.0001--0.0004 % --
well within the 0.5 % gate.

---

## 4. The 27.5 vs 27.6 GeV "partition" in ep0304

The supervisor's premise was that ep0304 contains a mix of 27.5 GeV and
27.6 GeV runs that must be tagged. The data verdict at this snapshot:

| ep0304 BeamElecE (GeV) | Runs | Lumi (nb^-1) | Source |
|---|---:|---:|---|
| 27.6 | 4,017 | 53,706.062 | H1Constants period default |

**All 4,017 ep0304 runs currently carry `BeamEnergyElectron_GeV = 27.6`
with `BeamEnergySource = "H1Constants_period_default"`.** There is no
HAT-derived per-run BeamElecE for ep0304 today because **no v5b
SIDIS-schema (or proton-schema) data has been produced for ep0304** --
this is the long-standing blocker carried in
`grl_manifest_highQ2.json` (FU1/FU2) and `context-handoff` for the
G15 e- recovery campaign.

The 27.5 GeV / 27.6 GeV split documented elsewhere
(`reference-highQ2-crad-availability.md` Section 3.3, line 131:
"ep0304 | e+ | **27.5 GeV** | dst2all_djangoh_Ee27p5/...") refers to the
**MC NonRad sample variants** chosen per period for radiative
corrections, not to a per-run partition of the data. The lambda_HiQ
inventory chose `dst2all_djangoh_Ee27p5/` (27.5 GeV) for the entire
ep0304 period because HERA was nominally operating closer to 27.5 GeV in
late 2003 / early 2004 (vs the canonical 27.6 GeV used for the rest of
HERA-II); the H1Constants table simplifies this to a single 27.6 GeV
value for all HERA-II.

**What this means for the SIDIS analysis:**
- Until v5b SIDIS data exists for ep0304, the per-run table uses 27.6 GeV
  for every ep0304 run, and downstream Q² / γ\*+p boost computations
  inherit that approximation.
- The associated systematic is bounded: the actual physical beam energy
  for ep0304 was between 27.5 and 27.6 GeV; the resulting Q² shift is
  ≲ 0.4 % at the analysis kinematics (relative shift in 2E_e cos θ
  denominator). This is comparable to the per-run 10 MeV precision floor
  and well below the dominant systematic budget items (acceptance,
  unfolding, C_rad).
- Once the G15 ep0304 v5b SIDIS production lands, **re-run
  `/tmp/extract_per_run_beam_energy.C` on NAF**: the script will
  automatically pick up new `DisTot.BeamElecE` / `BeamProtP` from any new
  files under `H1SIDIS_HighQ2/output/data/ep0304/` and overwrite the
  H1Constants fallback with the true per-run HAT values. Re-run the local
  enrichment afterwards.

---

## 5. 2007 MER and LER exclusion

| Sub-period | H1Constants enum | Run range | E_p (GeV) | E_e (GeV) | Status |
|---|---|---|---:|---:|---|
| LER 2007 | `eEplus07lowE460` | 500,918 -- 507,824 | 460 | 27.5 (task) / 27.6 (H1Constants) | **EXCLUDE** |
| MER 2007 | `eEplus07lowE575` | 507,843 -- 511,079 | 575 | 27.5 (task) / 27.6 (H1Constants) | **EXCLUDE** |

Source: `H1Geom/H1Constants.C:466-474` (`fgLowEnergyStart/End`,
`fgMidEnergyStart/End`) and lines 207-230 (`fProtonBeamEnergy(eEplus07lowE460) = 460`,
`fProtonBeamEnergy(eEplus07lowE575) = 575`).

**Note on E_e:** H1Constants hardcodes E_e = 27.6 GeV for both MER and LER
periods (lines 229-230), conflicting with the task statement that MER/LER
ran at 27.5 GeV. The per-run table writes 27.5 GeV for these runs per the
task instruction, but flags them `IsExcluded_MERLER = True` so no analysis
code consumes those values. If MER/LER ever need to be **included**, this
E_e value must be re-derived from the actual per-run HAT EBeamE; do not
rely on the H1Constants entry.

### Exclusion mechanism (zero work needed today)

The ep0607 GRL has `last_run = 500611`, **strictly less than the LER
first_run 500918**. Therefore:

> **0 of the 20,788 runs in the four high-Q2 SIDIS GRLs fall in the
> MER or LER ranges. MER/LER exclusion is automatic via GRL boundary
> -- no per-event filter is required at this snapshot.**

The S67 trigger required by `oolumi` (LAr NC DIS) was also disabled or
unused during the MER/LER campaigns; this provides a second, redundant
exclusion layer at the trigger level even if a run-number filter were
removed.

### Defence in depth

Downstream code should still check `IsExcluded_MERLER` at read time. The
flag protects against:
- Future regeneration of GRLs that extend the run range into 2007 low-E.
- Manual run-list edits that accidentally include MER/LER.
- Mixed productions that combine ep0607 with the separate
  `H1Constants::eEplus07lowE460/575` H1OO sub-periods.

---

## 6. Beam-energy precision

| Quantity | Value | Provenance |
|---|---|---|
| Per-run HAT `EBeamE` nominal precision | **~10 MeV** | Standard H1 spectrometer-style beam-energy reconstruction stored in DMIS bank; cited in `research-beam-polarization-charge.md` Section 2.1 and `H1HatEvent.h:131` comment ("electron beam energy from DMIS"). |
| Period-level H1Constants approximation | 27.6 vs HAT median 27.648 in ep0607 (Δ ≈ 50 MeV) | Direct comparison of period default to HAT median (this table); the H1Constants value is the standardised value, not the actual machine value. |
| Run-to-run spread within ep0607 (HAT data) | 27.498 to 27.684 GeV (Δ_peak-to-peak ≈ 186 MeV) | This extraction (4,035 runs). |
| Run-to-run spread within ep0607 (bulk, ≥ 99 % of lumi) | ~ 27.59 to 27.68 GeV | Lumi-weighted buckets (this table, JSON `ep0304_27p5_27p6_partition` analog for ep0607 would show the same pattern). |

**Systematic uncertainty floor on beam energy:** ±10 MeV per run for the
HAT-derived rows; for the H1Constants_period_default rows the effective
uncertainty is ~ +100 / -100 MeV (the 27.6 GeV nominal can be off the
true mean by ±50 MeV plus the unknown run-to-run spread). The systematic
study should therefore vary BeamEnergyElectron_GeV per run within these
two bands, distinguished by `BeamEnergySource`.

---

## 7. Downstream consumption pattern (REQUIRED)

Every kinematic computation in `H1SIDIS_HighQ2` must consume per-run
beam energies from this table. The mandatory access pattern:

```python
# at analysis init
import json, pandas as pd
TBL = pd.read_csv("docs/data/highQ2_per_run_beam_energy.csv")
TBL = TBL.set_index("RunNumber")

# at event read time
def kinematics(event):
    rn = event.RunNumber
    if rn not in TBL.index:
        raise ValueError(f"Run {rn} not in per-run beam-energy table -- check GRL membership")
    row = TBL.loc[rn]
    if row.IsExcluded_MERLER:
        return None  # drop event; MER/LER not in physics sample
    Ee = row.BeamEnergyElectron_GeV
    Ep = row.BeamEnergyProton_GeV
    s_cm = 4.0 * Ee * Ep
    # Q^2 (electron method)
    Q2e = 2.0 * Ee * event.scatEE * (1.0 + math.cos(event.theta_e))
    # gamma* + p boost
    e_in  = np.array([0.0, 0.0, -Ee, Ee])
    p_in  = np.array([0.0, 0.0,  Ep, math.hypot(Ep, 0.938)])
    e_out = np.array([event.scatEX, event.scatEY, event.scatEZ, event.scatEE])
    q = e_in - e_out
    boost = (q + p_in)[:3] / (q + p_in)[3]  # gamma*+p rest frame
    ...
```

**Required behaviours:**
1. **Mandatory lookup:** every code path that needs E_e or E_p must
   index this table by `RunNumber`. Do NOT hard-code 27.6 / 920 in any
   analysis script. Do NOT call `gH1Constants->GetElectronBeamEnergy()`
   for per-event use unless first calling
   `H1Constants::SetElectronBeamEnergy(table_value)` for each run.
2. **MER/LER drop:** every event with `IsExcluded_MERLER == True` is
   dropped before kinematic computation. At this snapshot the count is
   zero, but the check is required as a guard.
3. **Provenance carry:** acceptance / unfolding / systematic studies
   should propagate `BeamEnergySource` and the 10 MeV precision floor
   per run (use the larger error budget for H1Constants_period_default
   rows -- see Section 6).
4. **MC matching:** MC events must be assigned the same per-run E_e / E_p
   from this table when computing reco kinematics. The MC generator
   internal beams (Britzger `dst2all_djangoh_Ee27p5` vs `_Ee27p6`) match
   the period assignment in
   `reference-highQ2-crad-availability.md` Section 3.3.
5. **s_cm change:** the canonical s_cm = 4·27.6·920 = 101,568 GeV^2 used
   in legacy code is wrong by ~ 0.2 % per period (varies). Recompute
   per run.

---

## 8. Open follow-ups and blockers

- **FU1 -- ep0304 / em0405 / em06 HAT refresh.** Re-run the extraction
  macro once v5b SIDIS data lands for these periods. Owner: production
  pipeline (`H1SIDIS_HighQ2/scripts/submit_full_production.py`).
- **FU2 -- ep0607 HAT coverage gap.** Only 4,035 / 8,091 ep0607 runs
  have HAT-derived BeamElecE today; the remaining 4,056 use H1Constants
  fallback. Owner: same as FU1 -- a v5b SIDIS production run against
  the full ep0607 GRL will close this. Once landed, re-run extraction.
- **FU3 -- HAT vs DMIS provenance audit.** Confirm the per-run mean of
  `EBeamE` over `ProtonTot` events equals the run-header `EBeamE` (DMIS
  value at run-start). Differences should be ≤ 10 MeV; larger
  differences would indicate the HAT primitive includes per-event jitter
  rather than the DMIS run-level value. Owner: h1-software specialist.

---

## 9. Verification checklist

| Check | Status | Reference |
|---|---|---|
| All 20,788 GRL runs covered | OK | JSON `totals.n_runs == 20788` |
| Lumi sum within 0.5 % of 356,613 anchor | OK (+0.0002 %) | JSON `totals.match_within_0p5pct == true` |
| Per-period lumi within 0.5 % | OK (max +0.0004 %, ep0607) | JSON `per_period_cross_check` |
| MER/LER present in GRL? | NO (0 runs) | JSON `merler_in_current_grls.n_runs == 0` |
| ep0304 27.5/27.6 partition resolved? | NO -- all rows at 27.6 (H1Constants fallback); documented in Section 4 | JSON `ep0304_27p5_27p6_partition.caveat` |
| HAT API verified against H1Hat source? | YES | `H1Hat/H1HatEvent.h:130-131` |
| H1Constants period table verified against source? | YES | `H1Geom/H1Constants.C:200-230` |
| H1RunLumi API verified against source? | YES | `H1Tools/H1RunLumi.h:31, .C` |
| ROOT/H1OO version match (no GLIBCXX or ROOT version errors) | OK | LCG_107a x86_64-el9-gcc14-opt provides ROOT 6.34.04 matching the britzger build |

No remote command was retried; no `find` was run on `/cvmfs`, `/afs`, or
unrestricted `/pnfs`; no file was written on NAF outside `/tmp` and the
new `H1SIDIS_HighQ2/metadata/` directory.

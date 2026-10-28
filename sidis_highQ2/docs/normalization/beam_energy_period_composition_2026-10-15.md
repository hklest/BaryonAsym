# Beam Energy and Period Composition of the High-Q2 Charged-SIDIS Data Sample — Inputs for the Hand-Convention Virtual Photon Flux

**Date:** 2026-10-15
**Prepared by:** `h1-normalization` (read-only fact extraction, delegated normalization/flux-input task)
**Scope:** Per-period luminosities, beam energies, effective sqrt(s), and per-(Q2,x)-cell y/epsilon
values for the current v6/z005 four-period high-Q2 charged-SIDIS release, as required to
construct a Hand-convention virtual photon flux per release cell.
**Products touched:** NONE. This session was read-only; the only file written is this report.
**Skill note:** the project skills `sidis-hq2-normalization`/`sidis-hq2-builder` failed to load in
this runtime (tool error, empty content); per the standing precedent
(`docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` §7f) the authoritative
documents were read directly instead.

---

## 0. Answers at a glance

| Question | Answer |
|---|---|
| Per-period split of `L_data = 351,000.0 nb^-1`? | **Not recorded.** The target is a single global round number (351 pb^-1), externally anchored to Andreev 2024. The authoritative per-period *delivered* luminosities are the oolumi S67 GRL values (§1); their sum is 356,613.59 nb^-1, superseded as a normalization default but valid as the GRL inventory. |
| Beam energies in the sample | **All four periods: ~27.6 GeV lepton x ~919-920 GeV proton.** No other configuration contributes. |
| 2007 575-GeV-proton (MER) / 460-GeV (LER) data | **Absent.** 0 of 20,788 GRL runs fall in MER/LER ranges (GRL boundary exclusion, §2.3). The 2007 data in the sample is 27.6 GeV e+ on 920 GeV protons. |
| Chain handling | **Merged**: all 658 files from the four period directories are globbed into one 980-bin unweighted data vector; `L_data` is applied once, post-unfold, as a single global constant. No per-period lumi weighting anywhere (§3). |
| Beam energy granularity in code | **Per event** from the HAT bank (`EBeamE`/`EBeamP`), stored as `BeamElecE`/`BeamProtP`; a hardcoded 27.6/920 fallback exists only in the Stage-1 gen-only path. The release binning kinematics (`Q2es`/`Yes`/`Xes`) use **fixed H1CalcKine constants 27.6/920** (§3, exact file:line). |
| 2007 statistical contribution | 59,233.04 nb^-1 = **16.61%** of the GRL total (42.2% of ep0607), all at 920 GeV — material for statistics, **zero** contribution to beam-energy spread (§2.4). |
| Effective sqrt(s) | Fixed H1OO constant: s = 101,568.0 GeV^2, sqrt(s) = 318.697 GeV (defines the release `Yes`/`Q2es`/`Xes`). Lumi-weighted effective: s_eff = 101,589.04 GeV^2, sqrt = 318.730 GeV (+0.021%). Run-level spread 318.119-319.194 GeV (-0.18%/+0.16%) (§4). |
| y / epsilon over release cells | Populated y in [0.100, 0.700] everywhere (reco `Yes` envelope); epsilon in **[0.5505, 0.9945]** across all populated release phase space; per-cell table in §5. |
| Task-premise corrections | (a) The release has **4 Q2 strata / 7 (Q2,x) cells**, not seven Q2 strata. (b) 2007 in-sample is at 920 GeV protons, not 46x575 (MER/LER excluded). (c) H1OO records **27.6 GeV for ALL HERA-II periods**; 27.5 GeV appears only as the ep0304 MC NonRad generation choice and the ep0304 physical-range caveat — not as a 2005-2006 data value. |

---

## 1. Per-period decomposition of L_data = 351,000.0 nb^-1

### 1.1 What IS recorded: the oolumi S67 GRL per-period luminosities

Source of truth: `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_*.root` (oolumi outputs;
bit-identical SIDIS-local copies of the upstream `H1BaryonAsym_HighQ2/LumiFiles/` set,
md5-verified 2026-05-23). Reference: `docs/references/reference-highQ2-grl-manifest.md`
(2026-05-19, frozen). All four GRLs use the identical S67 (LAr NC DIS) selection,
prescale limit 1, |z_vtx| < 35 cm, Compton correction on, min run lumi 0.2 nb^-1.

| Period | Beam | GRL file | Runs | First-Last run | L (nb^-1) | Share of GRL | Charge state |
|---|---|---|---:|---|---:|---:|---|
| ep0304 | e+ | `nc_goodruns_0304.root` | 4,017 | 357160-392213 | 53,706.105 | 15.06% | 100% e+ |
| em0405 | e- | `nc_goodruns_0405.root` | 5,194 | 399629-436893 | 105,028.172 | 29.45% | 100% e- |
| em06 | e- | `nc_goodruns_06.root` | 3,486 | 444312-466997 | 57,573.094 | 16.14% | 100% e- |
| ep0607 | e+ | `nc_goodruns_0607.root` | 8,091 | 468532-500611 | 140,306.219 | 39.34% | 100% e+ |
| **Total** | | | **20,788** | | **356,613.59** | 100% | e+ 194,012.324 / e- 162,601.266 |

Charge composition per period confirmed from `data/bsa/validation_v6bsa_2026-09-16.json`
(ep0304 100% e+; em0405 100% e-; em06 100% e-; ep0607 100% e+). Run sets are disjoint
(union = 20,788 = sum). e+/e- GRL ratio = 1.193; GRL e+ fraction = 0.5440.

### 1.2 What is NOT recorded: a per-period split of the 351,000.0 target

- **No per-period decomposition of `L_data = 351,000.0 nb^-1` exists anywhere in the
  workspace.** The target is a single global round number, 351 pb^-1, current for the
  four-period S67 sample including ep0304 (`STATE.md` line 12;
  `docs/status/normalization.md`; `docs/normalization/lumi_2p7_derivation_memo_2026-10-10.md`).
- Its external anchor is Andreev et al. 2024, EPJC 84 (2024) 718, arXiv:2403.10134
  (groomed event shapes), performed on the **same 351 pb^-1 HERA-II 2003-2007 dataset**;
  the flat **2.7%** luminosity uncertainty is anchored to its Sec. 6.3.
- The chain needs no per-period split: the data vector is merged and the target is
  applied once. The explicit policy is `docs/status/normalization.md`: MC vectors are
  scaled with one generator-wide constant, `yield_980 = (351000.0 / L_MC_total) * sumw_980`,
  and "**Do not scale each period to full data luminosity and then sum periods**".
- If a per-period decomposition of the *target* is ever required (e.g. per-period flux
  weighting), the only defensible in-workspace derivation is proportional scaling of the
  GRL shares by `351000.0 / 356613.59 = 0.984260` — a **derived convention, not a
  recorded fact**. Making it exact requires oolumi-level confirmation of the target's
  construction (§9, item 1).
- Relationship of the two numbers: 356,613.59 nb^-1 is the *delivered* GRL sum;
  351,000.0 nb^-1 is the *analysis target*. `356613.0`/`356000` are superseded as
  normalization defaults (`docs/status/normalization.md` "Do Not Use"); they remain
  valid only as the GRL inventory sum. Guard list: `297,657.8` (2005-2007 only,
  excludes ep0304), `356,613.0`/`356,000` (superseded direct-monitor quotes),
  `126,880` (forbidden, wrong by ~42x).
- Historical only: v5f-era *realised effective* per-period lumis
  {ep0304: 37,981.522; em0405: 69,678.028; em06: 22,798.097; ep0607: 58,470.084} nb^-1
  (sum 188,927.7; e+/e- = 1.04299) are DST-coverage-fraction products of the superseded
  v5f era; they survive only in quarantine/historical documents and must not be used.

### 1.3 Beam-charge mixture weights (adjacent record, for completeness)

The applied C_rad mixed-beam weight `f_ep = 0.5140930272` derives from the **superseded**
297,657.8 nb^-1 accounting (L(e+) = 106,823.1 + 46,200.7 = 153,023.8; L(e-) = 144,634.0).
The current four-period GRL e+ fraction is **0.5440**; the v5f-era realised estimate is
~0.5105. The induced C_rad bias is bracketed at <= 0.02% (negligible; provenance-wording
item only) — `docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` §ii and
gap item 8. For the virtual-photon flux this is irrelevant: y and epsilon are
kinematic quantities identical for e+ and e- at the same beam energies.

---

## 2. Beam energies per period actually in the sample

### 2.1 Per-period table (verified from workspace records, not memory)

| Period | E_e (GeV) | E_p (GeV) | sqrt(s) (GeV) | Evidence |
|---|---|---|---|---|
| ep0304 | 27.6 (H1Constants); physical range 27.5-27.6 | 920 | 318.697 (const) | H1Constants period default; no HAT extraction predates v6 (see caveat below) |
| em0405 | 27.6 | 920 | 318.697 (const) | H1Constants period default |
| em06 | 27.6 | 920 | 318.697 (const) | H1Constants period default |
| ep0607 | 27.6 (HAT: run means 27.498-27.684) | 919-921 (HAT: 919.086-920.817) | 318.1-319.2 per run | 4,035/8,091 runs with HAT in the 2026-05-20 per-run CSV; direct v6-tree read below |

- H1OO API layering (`docs/references/reference-highQ2-per-run-beam-energy.md`):
  preferred per-run source is the HAT bank `EBeamE`/`EBeamP` (DMIS-recorded, ~10 MeV
  precision); `H1Constants::ElectronBeamEnergy()/ProtonBeamEnergy()`
  (`H1Geom/H1Constants.C:901-911`) is the period-level fallback, hardcoded to
  **27.6 / 920 for every HERA-II nominal period** (460 for `eEplus07lowE460`, 575 for
  `eEplus07lowE575` — both excluded, §2.3).
- **Caveat on the per-run CSV** (`H1SIDIS_HighQ2/metadata/per_run_beam_energy.csv`,
  20,788 rows): it was built 2026-05-20 from the then-existing legacy per-run files,
  so only 4,035 ep0607 runs carry HAT values and all ep0304/em0405/em06 rows use the
  27.6/920 fallback. The **v6 central trees now carry per-event HAT beam energies for
  all 658 files** (§3), so this coverage gap reflects the CSV's build era, not the
  current sample. A CSV refresh from the v6 trees would close it (§9, item 2); the
  effect on s is bounded at the ~0.2% level (§4).
- **ep0304 27.5-vs-27.6 caveat**: the actual ep0304 machine energy was between 27.5 and
  27.6 GeV; H1Constants simplifies to 27.6 for all HERA-II; the ep0304 MC NonRad
  radiative-correction variant was generated at 27.5 GeV
  (`dst2all_djangoh_Ee27p5/`, per `reference-highQ2-crad-availability.md` §3.3).
  Resulting Q2-level shift <= 0.4% at analysis kinematics — documented, bounded, and
  well below dominant systematics. This is a **MC-variant choice**, not a data
  per-run partition (all 4,017 ep0304 runs carry the single 27.6 H1Constants default).

### 2.2 Direct verification from the v6 sample trees

Single-file ROOT inspection of
`output_v6/central/sidistree/data/ep0607/sidistree_ep0607_d07_0000.root`
(tree `ChargedTot`, 8,713 entries): run range **492560-492577**,
`BeamElecE = 27.611 GeV`, `BeamProtP = 919.125 GeV` — direct proof that the 2007 (d07)
files in the sample are 27.6 GeV e+ on ~919-920 GeV protons.

### 2.3 2007 MER/LER exclusion (no 575-GeV or 460-GeV protons in the sample)

| Sub-period | H1Constants enum | Run range | E_p (GeV) | Status |
|---|---|---|---:|---|
| LER 2007 | `eEplus07lowE460` | 500,918-507,824 | 460 | **EXCLUDED** |
| MER 2007 | `eEplus07lowE575` | 507,843-511,079 | 575 | **EXCLUDED** |

The ep0607 GRL ends at run **500,611**, strictly below the LER start (500,918) and the
MER start (507,843). **0 of the 20,788 GRL runs fall in MER/LER ranges**; the exclusion
is automatic via the GRL boundary (redundantly reinforced by the S67 trigger, which was
not in use during the MER/LER campaigns). Source: `H1Geom/H1Constants.C:466-474` and
`:200-230`; `reference-highQ2-per-run-beam-energy.md` §5.
**Consequence for the flux:** no low-energy sqrt(s) mode contaminates the sample; the
task premise "46.0 x 575 for 2007" does not apply to this release.

### 2.4 Materiality of the 2007 contribution

ep0607 runs >= 490,000 (the 2007 calendar portion; the 111 `d07` files):
**2,770 runs, L = 59,233.04 nb^-1 = 16.61% of the GRL total = 42.2% of ep0607**, all at
27.6 GeV x ~919-920 GeV (§2.2). The 212 `d06` files in `ep0607/` are ep0607-*period*
(e+) chunks from calendar-2006 (chunks c060000-c060212), likewise at 920 GeV.
**Conclusion: 2007 data contributes materially to statistics and nothing to
beam-energy spread.**

---

## 3. How the central chain handles beam configuration

### 3.1 Merged, single-L policy

- Trees are produced per period into
  `output_v6/central/sidistree/data/{ep0304,em0405,em06,ep0607}/`
  (83 / 152 / 100 / 323 files; 658 total; ep0607 = 212 d06 + 111 d07).
- The data-vector builder
  (`scripts/unfolding/cone_s2prime_2026-10-11/build_data_vector_v4_alphaveto.py`)
  globs **all four period directories** into **one merged 980-bin vector of unweighted
  (+1) counts** (usage block lines 160-164). Reco event cuts: `150 < Q2es < 20000`,
  `0.1 < Yes < 0.7`, `Empz > 45` (lines 208-212, applied at lines 433-438); stage-1
  loose envelope `0.05 < y < 0.9`, `30 < Empz < 75`, `Q2 > 100`.
- `L_data = 351,000.0 nb^-1` enters **only downstream, once, as a single global
  constant** (post-unfold normalization; MC side: one generator-wide scale,
  `docs/status/normalization.md` §"Alpha0p2 Background Vector Normalization").
  **No per-period lumi weighting, no per-period target split, no beam-charge weighting
  is applied to the data anywhere in the central chain.**

### 3.2 Beam energy per event, per file — exact code locations

| Where | File:line | What |
|---|---|---|
| Stage 2, HAT bind | `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C:434-435` | `H1FloatPtr hatEBeamE("EBeamE")` / `hatEBeamP("EBeamP")` — per-event HAT read |
| Stage 2, store | same file `:1121-1123` | `BeamElecE`/`BeamProtP` stored per event |
| Stage 2, kinematics | same file `:1139` | `xBj = Q2s/(4.0*pBeamP*eBeamE*Ys)` — uses **per-event HAT** energies |
| Stage 2, boosts | same file `:1144-1147` (beam 4-vectors), `:1150-1163` (gamma*, gamma*p, Breit) | gamma*p / Breit boosts from **per-event HAT** energies |
| Stage 2, es-method | same file `:1057-1066` | `Q2es/Yes/Xes = gH1Calc->Kine()->Get...()` — **H1CalcKine fixed constants** (below) |
| Stage 1, HAT bind | `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:1008-1009` | per-event HAT pointers |
| Stage 1, store | same file `:1339-1342` | per-event read/store |
| Stage 1, fallback | same file `:2874-2875` | **hardcoded 27.6/920**, only when the HAT value is < 1.0 (gen-only, HAT-zero path) |
| H1CalcKine constants | `H1Calculator/H1CalcKine.h:79-83` | `GetConstants()`: `PBEAM/EBEAM/fSS = TheConstants().GetProtonBeamEnergy()/GetElectronBeamEnergy()/GetCMSEnergySquared()` |
| H1Constants defaults | `H1Geom/H1Constants.C:51-52` | `fElBeamE = 27.6` / `fPrBeamE = 920` ("Always set the same") |
| CM energy | `H1Geom/H1Constants.h:139` | `GetCMSEnergySquared() = 4*fElBeamE*fPrBeamE = 101,568.0 GeV^2` |

There are **no callers** of `H1Constants::SetElectronBeamEnergy`/`SetProtonBeamEnergy`
anywhere in the H1OO tree or the analysis sources, so every H1CalcKine-based quantity
runs at the fixed 27.6 x 920 constants.

**Consequence:** the release binning/selection variables (`Q2es`, `Yes`, `Xes`) are
defined at fixed s = 101,568.0 GeV^2; per-event HAT energies enter `xBj`, `Ws`, and the
gamma*p/Breit frame quantities (xF, current hemisphere), not the release binning.

---

## 4. Effective sqrt(s)

| Quantity | Value | Provenance |
|---|---|---|
| H1OO fixed constant (release kinematics) | s = 101,568.0 GeV^2, sqrt(s) = **318.697 GeV** | `4*27.6*920`, H1Constants.h:139 |
| Lumi-weighted effective (HAT where available, 27.6/920 fallback elsewhere) | E_e = 27.6075 GeV, E_p = 919.9421 GeV, s_eff = 101,589.04 GeV^2, sqrt(s_eff) = **318.730 GeV** (+0.021% vs constant) | per-run CSV, lumi-weighted, all 20,788 GRL runs |
| Run-level spread | **318.119 GeV** (run 470608, -0.18%) to **319.194 GeV** (run 470840, +0.16%) | per-run CSV, HAT runs |
| 27.5-GeV e-beam variant | sqrt(s) = 318.119 GeV | coincides with the run-level minimum |
| HAT beam-energy ranges | E_e 27.498-27.684 GeV; E_p 919.086-920.817 GeV | 4,035 ep0607 HAT runs |

- The per-run beam-energy reference doc's remark that the canonical
  `s = 4*27.6*920` is "wrong by ~0.2% per period" is quantified here: the lumi-weighted
  effective value is **+0.021%** above the constant; the ~0.2% figure is the per-run
  spread bound, not the weighted mean shift.
- **For the Hand-convention flux this is negligible**: using the fixed
  s = 101,568.0 GeV^2 for per-cell y shifts y by +0.021% relative to s_eff — three
  orders of magnitude below the flat 2.7% luminosity uncertainty and far below the
  within-cell y spans (§5). The fixed value is also the *self-consistent* choice,
  since the release cells and the `Yes` selection are themselves defined at the fixed
  constant (§3).

---

## 5. Per-cell y and epsilon over the release cells (Hand convention)

### 5.1 Definitions and bin structure

- Convention (in-workspace, from `exclu_phi/docs/research-cross-section-method.md`,
  H1 exclusive-phi paper Eq. 15):
  `y = Q2/(x*s)`, `epsilon = (1-y)/(1-y+y^2/2)`,
  flux `Gamma = (alpha/pi)*(1-y+y^2/2)/(y*Q2) = (alpha/pi)*(1-y)/(epsilon*y*Q2)`.
- s = **101,568.0 GeV^2** (H1OO constant; consistent with the release `Yes`/`Q2es`/`Xes`
  definition; s_eff comparison in §4).
- Release bin structure (`bin_map.csv` of the current Born release
  `born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/`): **4 Q2 strata**
  (S0 150-300, S1 300-700, S2 700-1500, S3 1500-20000 GeV^2) crossed with x subbins =
  **7 (Q2,x) cells** ("S3xmerged" = S3 single merged x bin); each cell 70 bins
  (2 charges x 7 z x 5 pT); 490 rows total. The canonical "7col" plot grid is
  7 z-columns x 7 cells = 49 panels/charge.

### 5.2 Per-cell table

`y_ctr`/`eps_ctr` = arithmetic midpoint of the (Q2, x) cell; `y_geo`/`eps_geo` =
geometric center; "corners" = y at the four (Q2, x) cell corners; "populated" = the
y span inside the cell clipped to the reco `Yes` envelope (0.1, 0.7) — the phase space
that actually contributes to the release.

| Cell | Q2 range (GeV^2) | x range | y_ctr | y_geo | y corners | y populated | eps_ctr | eps_geo | eps populated |
|---|---|---|---:|---:|---|---|---:|---:|---|
| S0_x0 | 150-300 | 0.0021-0.0071 | 0.4816 | 0.5409 | 0.208-1.407 | 0.208-0.700 | 0.8172 | 0.7584 | 0.5505-0.9734 |
| S0_x1 | 150-300 | 0.0071-0.03 | 0.1194 | 0.1431 | 0.049-0.416 | 0.100-0.416 | 0.9920 | 0.9882 | 0.8709-0.9945 |
| S1_x0 | 300-700 | 0.0042-0.016 | 0.4874 | 0.5504 | 0.185-1.641 | 0.185-0.700 | 0.8119 | 0.7480 | 0.5505-0.9795 |
| S1_x1 | 300-700 | 0.016-0.069 | 0.1158 | 0.1358 | 0.043-0.431 | 0.100-0.431 | 0.9925 | 0.9894 | 0.8599-0.9945 |
| S2_x0 | 700-1500 | 0.0099-0.034 | 0.4934 | 0.5499 | 0.203-1.492 | 0.203-0.700 | 0.8063 | 0.7486 | 0.5505-0.9749 |
| S2_x1 | 700-1500 | 0.034-0.15 | 0.1177 | 0.1413 | 0.046-0.434 | 0.100-0.434 | 0.9922 | 0.9885 | 0.8571-0.9945 |
| S3_x0 | 1500-20000 | 0.021-1.5 | 0.1392 | 0.3038 | 0.010-9.377 | 0.100-0.700 | 0.9889 | 0.9378 | 0.5505-0.9945 |

Reference points: `eps(y=0.1) = 0.9945`, `eps(y=0.7) = 0.5505`.

### 5.3 Interpretation notes for the flux construction

- **Global populated envelope:** across all release cells, y in [0.100, 0.700] and
  epsilon in **[0.5505, 0.9945]**. The lowest epsilon (0.5505) occurs at the high-y
  corner of the low-x cells (S0_x0/S1_x0/S2_x0 at y -> 0.7).
- The **x0 cells at S0-S2** sit at y_ctr ~ 0.48-0.49 (eps_ctr ~ 0.81): the flux factor
  `(1-y+y^2/2)/y` varies strongly across these cells (populated y 0.185-0.700); a
  single-y flux per cell should use a **data-weighted mean y** (extractable from the
  trees), not the geometric center.
- The **x1 cells** are high-epsilon (populated eps >= 0.857, ~0.99 at center): the flux
  reduces to nearly `(alpha/pi)/(y*Q2)` there.
- The **merged S3 cell** spans an enormous y corner range (0.010-9.377); its populated
  range is the full reco envelope [0.100, 0.700]. Neither the arithmetic center
  (y=0.1392) nor the geometric center (y=0.3038) is representative; if a single-y
  flux factor for S3 is required, use a data-weighted per-cell mean y and state the
  convention.
- Per-cell single-y choices aside, the table's *populated ranges* are the correct
  envelopes for any per-cell epsilon-averaging or flux-integration check.

---

## 6. Normalization verification (unchanged facts, re-confirmed 2026-10-15)

- `L_data = 351,000.0 nb^-1 = 351 pb^-1` — **CURRENT** four-period S67 target including
  ep0304 (`STATE.md` line 12; `docs/status/normalization.md`; `docs/RUN_STATUS.md`).
  Superseded/forbidden: `297,657.8` (2005-2007 only), `356,613.0`/`356,000`
  (superseded direct-monitor), `126,880` (forbidden).
- **7091 trap unchanged:** processed `merged_7091.root` is `djangoh14/7091` RAD
  high-Q2 signal MC (`NC.EPLUSP.0607.RAD.Q2GT60`) with
  `L_MC = 4,379,935.0 nb^-1` (provenance
  `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`,
  `per_catalog/djangoh14_7091_ep0607`) and current scale
  `351000.0/4379935.0 = 0.08013817556653238`. The `15,813.4` steering sum belongs to
  the **DjBkgd five-ReqID family** (7087/7104/7105/7107/7110, all `NC RAD Q2GT2
  CTEQ6L`), never to 7091; per the 2026-09-25 fLumi unit correction it is
  framework-correctly read as `15,813.4 pb^-1 = 15,813,400 nb^-1`
  (correct DjBkgd scale `0.0221963651`). Using it against 7091 is the known stale-scale
  failure mode.
- Combined generator lumis (current): RAPGAP31 `9,132,891.0 nb^-1`
  (scale 0.03843251824641288); DJANGOH14 `10,487,495.0 nb^-1`
  (scale 0.03346843073584302).
- Luminosity uncertainty: flat **2.7%**, correlated across all bins (Andreev 2024 §6.3
  anchor), disjoint from `C_rad` components and from beam-energy effects.

## 7. Beam-energy vs luminosity bookkeeping — conflict check

**No conflicts found.** Specifically:

1. All luminosity records (per-run Compton-corrected GRL lumis, the 351,000.0 target,
   MC effective lumis) are period/run-level quantities, independent of beam-energy
   configuration; no lumi record is keyed by energy mode.
2. MER/LER contribute zero runs, so there is no energy-configuration mixture that
   could conflict with the single-target normalization.
3. The one beam-energy asymmetry on record — ep0304 MC NonRad generated at 27.5 GeV
   while data kinematics use the 27.6 GeV H1Constants value — is a documented,
   bounded (<= 0.4% in Q2) MC-variant choice, not a luminosity conflict.
4. The per-run doc's recommendation to recompute s per run is quantified in §4:
   +0.021% lumi-weighted overall; the release legitimately keeps fixed-constant
   es-kinematics by construction (§3), and the flux impact is negligible.

## 8. Flag: BSA output-directory artifact (non-central, discovered during this extraction)

- `output_v6bsa/central/sidistree/data/em06/` contains **312 files** =
  100 `sidistree_em06_d06` (e-) **+ 212 `sidistree_ep0607_d06` (e+)** — a year-tag
  grouping artifact (the ep0607-period, calendar-2006 chunks landed in the `em06/`
  directory). `output_v6bsa/.../ep0607/` holds only the 111 d07 files.
- **Central v6 is clean**: `ep0304/` 83, `em0405/` 152, `em06/` 100,
  `ep0607/` 323 (= 212 d06 + 111 d07); 658/658.
- Consequence: any **directory-stratified beam-charge separation on `output_v6bsa`
  is broken** (`em06/` mixes charges). This explains the handoff trap "directory-
  stratified fit mixes lepton charges", and corrects its directory attribution: the
  mixed directory is **`em06/`**, not `ep0607/`. The published integrated BSA result
  (raw, consistent with zero) is unaffected; the artifact matters only for
  charge-stratified BSA reprocessing.

## 9. Numbers NOT recorded / needing oolumi

1. **Per-period split of the 351,000.0 nb^-1 target** — the only true gap. The
   authoritative per-period *delivered* lumis are the oolumi GRL values (§1.1); the
   target itself has no recorded per-period composition. Exact resolution requires
   oolumi-level confirmation of the target's construction or a supervisor-approved
   proportional convention (`351,000.0 x GRL_share`, factor 0.984260). This is also
   CRAD mixed-beam gap item 8 (effect there already bracketed <= 0.02%).
2. **HAT per-run beam energies for ep0304/em0405/em06 and the 4,056 non-HAT ep0607
   rows of the 2026-05-20 CSV** — the CSV predates v6; the v6 trees now carry
   per-event HAT beam energies for all 658 files, so a refresh from the v6 trees
   would close this (bounded effect: fallback vs HAT differs by <= ~0.2% in s).
3. (Carried elsewhere, unchanged, not new:) DjBkgd oomclumi/generator-log accounting
   over the exact resolved HAT lists for ReqIDs 7087/7104/7105/7107/7110 remains
   outstanding (`docs/status/normalization.md`).

## 10. Verification provenance

- Read-only session; no analysis product modified; the only file written is this report.
- All §5 numbers were recomputed this session directly from the release
  `bin_map.csv` (all 7 cells reproduced independently); §4 effective-s values were
  recomputed from `H1SIDIS_HighQ2/metadata/per_run_beam_energy.csv` (lumi-weighted,
  all 20,788 runs; HAT/fallback split 4,035/16,753 reproduced); §2.2 by direct ROOT
  read of `sidistree_ep0607_d07_0000.root` (LCG_107a ROOT 6.34.04, single-file
  login-node check, seconds); §8 by directory listings of the central and BSA data
  trees; code citations in §3 were re-checked against the sources line-by-line.
- Primary sources: `docs/references/reference-highQ2-grl-manifest.md` (2026-05-19),
  `docs/references/reference-highQ2-per-run-beam-energy.md` (2026-05-20),
  `docs/status/normalization.md`, `docs/normalization/lumi_2p7_derivation_memo_2026-10-10.md`,
  `docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md`,
  `STATE.md` (2026-10-14), `data/bsa/validation_v6bsa_2026-09-16.json`,
  `exclu_phi/docs/research-cross-section-method.md` (epsilon/flux convention),
  H1OO sources under
  `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`.
- Note on the workspace-wide glob/find: it times out on the BaryonAsym root; all
  lookups were targeted per subdirectory.

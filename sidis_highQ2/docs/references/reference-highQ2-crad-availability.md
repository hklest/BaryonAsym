<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Reference: High-Q2 SIDIS C_rad Sample Availability and Strategy

**Date:** 2026-05-19
**Author:** h1-mc-expert (delegated for h1-radiative-correction)
**Scope:** Determine whether MC samples needed for QED radiative-correction
(`C_rad`) computation are available for the **v5b high-Q2 charged-particle
SIDIS analysis** (Q2 > 150 GeV2), and recommend a closure path for
`plan-highQ2-sidis-next-steps.md` gate **G10**.
**Status:** HISTORICAL / SUPERSEDED for current z005 `C_rad` production. Verified on NAF (`naf-hone.desy.de`) on 2026-05-19 as read-only inventory; no NAF files modified. Current high-Q² z005 `C_rad` status is DJANGOH14 gen-only v1 at `studies/radiative_correction_genonly_v1/`, with `C_rad = σ_nonrad/σ_rad`; see `docs/RUN_STATUS.md` and `context-handoff.md` before using any strategy or convention below.

> Companion JSON: `H1/sidis_highQ2/docs/data/highQ2_nonrad_mc_inventory.json`.

---

## 0. Bottom Line First

| Question | Answer |
|----------|--------|
| Does v5b NonRad builder output exist for high-Q2 SIDIS on NAF? | **NO** |
| Do RAD v5b builder outputs exist for high-Q2 SIDIS on NAF? | **YES** (DJANGOH14: 486 per-run files merged to `merged_highQ2_mc_djangoh14.root`; RAPGAP31: 724 per-run files merged to `merged_highQ2_mc_rapgap31.root`) |
| Do HAT/MODS/ODS NonRad H1OO source files exist on NAF? | **YES**, in `/data/dust/group/h1/britzger/norad_prod21/` for both DJANGOH14 and RAPGAP31, both beam energies (27.5 / 27.6 GeV), both beam charges (EPLUSP / EMINUSP) |
| Recommended G10 strategy | **Strategy 2** (gen-loop on Britzger NonRad HAT files, no builder rerun required) with **Strategy 1 as a higher-cost backup** |
| Can G10 close without a Condor v5b-NonRad production launch? | **YES**, via Strategy 2 |
| Per-charge `C_rad(h+/-)` practical at high-Q2? | **YES** -- the v5b builder writes `ChargedTotGen` with `GenCharge`, and the Britzger HAT files have full beam-charge separation (EPLUSP / EMINUSP), giving the four-way split needed for `A_BC` |
| Critical blocker for G10 requiring supervisor decision | A small one: **(i)** Britzger NonRad samples are pre-MC13 (likely oo-4.0.6); the v5b RAD output is built on MC13 (oo-4.0.25). For **gen-level** `C_rad` this is physically irrelevant, but supervisor should confirm gen-level-only is acceptable. **(ii)** RAPGAP NonRad exists but RAPGAP is not the standard C_rad generator; recommend DJANGOH14 NonRad as nominal, RAPGAP31 NonRad only as a generator-model systematic. |

---

## 1. G10 Requirement Restated (from plan-r4/r5)

Per `plan-highQ2-sidis-next-steps.md` Section G10 (lines 263-273), G10
closure requires either:

- (a) a nominal `C_rad` with bounded uncertainty for high-Q2 multiplicities
  AND charge-separated AND beam-sign-separated `C_rad` for `A_BC(h+)`,
  `A_BC(h-)`, and the charge-summed `A_BC` control; or
- (b) explicit exclusion of observables that require `C_rad` from the
  G16 internal package.

The plan's G10-as-dependency-of-G16 rule (Section "Risks", line 840)
explicitly forbids shipping a placeholder `C_rad` (Strategy 4) in the
internal package.

Historical convention recorded in this inventory was:

    C_rad(bin) = (N_rad(bin) / L_rad) / (N_nonrad(bin) / L_nonrad)
               = sigma_rad(bin) / sigma_nonrad(bin)         (denominator convention)

**Superseded for current high-Q² z005:** the current gen-only v1 convention is `C_rad(bin) = σ_nonrad(bin) / σ_rad(bin)` with `σ ∝ Σ(weight1*weight2)/L_MC`; data luminosity does not enter. Evaluate current status from `docs/RUN_STATUS.md`, not from this historical May-2026 recipe.

---

## 2. v5b High-Q2 NonRad Builder Output (Strategy 1 Status)

**Verdict: ABSENT.**

NAF inventory (2026-05-19, `ssh ... ls` on the SIDIS code root):

```
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/
    djangoh14/
    rapgap31/
```

No `*nonrad*`, `*NonRad*`, `*norad*`, `*NORAD*` subdirectory anywhere
under the SIDIS HighQ2 output tree. No NonRad steering directory under
`H1SIDIS_HighQ2/steering/` (the existing steering subdirs are
`data/`, `evls/`, `mc/`, `proton/`, `smoke/`).

The /pnfs scratch tree shows three high-Q2 areas
(`highQ2_merged/`, `highQ2_trees/`, `highQ2_v4/`); only
`highQ2_merged/` contains the two merged products
(`merged_highQ2_mc_djangoh14.root` ~24 GB, `merged_highQ2_mc_rapgap31.root`
~47 GB). Both are pure RAD merges from Task 1's confirmed v5b RAD
output.

Conclusion: Strategy 1 ("standard rad/nonrad ratio from gen-level v5b
builder output") is **not executable today**. It would require launching
a dedicated Condor v5b-NonRad production using the HighQ2 builder on
the Britzger NonRad HAT files (or a new steering set against the
analogous /pnfs catalogs if one ever appears, which per the lambda_HiQ
inventory it does not).

---

## 3. HAT/MODS-Level NonRad Source Files (Strategy 2 Status)

**Verdict: PRESENT and intact.**

The Britzger private production area
`/data/dust/group/h1/britzger/norad_prod21/` was inventoried on NAF.
Cross-referenced against
`H1/lambda_HiQ/docs/research-nonrad-inventory.md` (the canonical NonRad
sample inventory written for the lambda_HiQ analysis, 2026-04-26), the
files are unchanged.

### 3.1 DJANGOH14 NonRad (recommended nominal)

| Directory | HAT (count) | EPLUSP HAT | EMINUSP HAT | Beam energy | Notes |
|-----------|-------------|------------|-------------|-------------|-------|
| `dst2all_djangoh14_NORAD/` | 520 | 260 | 260 | 27.6 GeV | Standard NORAD (`LPIN(4)=0`, self-energy ON). Recommended nominal C_rad input. |
| `dst2all_djangoh_Ee27p5/` | 520 (per inventory) | -- | -- | 27.5 GeV | `ep0304` beam-energy variant |
| `dst2all_djangoh_Ee27p6/` | 520 (per inventory) | -- | -- | 27.6 GeV | Earlier Apr-2021 production at same energy as `dst2all_djangoh14_NORAD` |
| `dst2all_djangoh14_BORN_NORAD/` | 519 | 260 | 259 | 27.6 GeV | Born-only variant (no self-energy). NOT for nominal C_rad. Use only if pure Born subtraction is needed. |
| `dst2all_djangoh14_RAD/` | 126 | 65 | 61 | 27.6 GeV | Matched RAD from same Britzger production. Useful as a Britzger-RAD / Britzger-NORAD cross-check pair if the v5b-RAD vs Britzger-NORAD pairing raises concern. |

Each HAT file is co-located with the matching MODS and ODS files. One
sample file confirmed readable on NAF:
```
-rw-r--r--. britzger atlas 26066232 Feb 6 2023
    dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EPLUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000.root
```

### 3.2 RAPGAP31 NonRad (historical recommendation superseded; quarantined for current z005)

> **Current z005 supersession:** RAPGAP31 gen-only `C_rad` is anomalous/quarantined (integrated about `1.405` per charge, median about `1.478`, most finite bins outside `[0.5,1.2]`). Do not use RAPGAP31 as nominal or as a generator-envelope systematic until provenance/lumi/sample issues are resolved; see `docs/RUN_STATUS.md`.

| Directory | Total files | HAT | EPLUSP files | EMINUSP files | Beam energy |
|-----------|-------------|-----|--------------|---------------|-------------|
| `dst2all_rapgap/` | 1560 | 520 | 792 | 768 | symlinks to Ee27p6 |
| `dst2all_rapgap_Ee27p5/` | 2267 | 528 | 1128 | 1128 | 27.5 GeV |
| `dst2all_rapgap_Ee27p6/` | 2608 | 520 | 1320 | 1280 | 27.6 GeV |

### 3.3 Per-period mapping (from lambda_HiQ inventory Table 3)

| Period | Charge | Beam energy | DJANGOH NonRad path | RAPGAP NonRad path |
|--------|--------|-------------|---------------------|---------------------|
| ep0304 | e+ | 27.5 GeV | `dst2all_djangoh_Ee27p5/` EPLUSP | `dst2all_rapgap_Ee27p5/` EPLUSP |
| em0405 | e- | 27.6 GeV | `dst2all_djangoh14_NORAD/` EMINUSP | `dst2all_rapgap_Ee27p6/` EMINUSP |
| em06 | e- | 27.6 GeV | `dst2all_djangoh14_NORAD/` EMINUSP | `dst2all_rapgap_Ee27p6/` EMINUSP |
| ep0607 | e+ | 27.6 GeV | `dst2all_djangoh14_NORAD/` EPLUSP | `dst2all_rapgap_Ee27p6/` EPLUSP |

Note that EMINUSP NonRad is shared between em0405 and em06 -- standard
H1 practice (NonRad physics is beam-energy and beam-charge dependent
only).

### 3.4 Generation parameters (DJANGOH14 NORAD)

From the steering at
`/data/dust/group/h1/britzger/norad_prod21/djangoh14/stdjan_nc_eminusp_norad_q2gt100_cteq6l_a`
(quoted in `lambda_HiQ/docs/research-nonrad-inventory.md` Section 2):

- Generator: DJANGOH 1.4
- Process: NC DIS, **non-radiative** (`LPIN(4) = 0`)
- Self-energy: ON (`LPIN(8,9,11) = 1`)
- PDF: CTEQ6L (LHAPDF 5.7.1, set 10041)
- Q2 truth threshold: **Q2e > 100 GeV2** -- well below the high-Q2
  SIDIS analysis floor of 150 GeV2, so the NonRad sample fully covers
  the analysis kinematic range.
- Proton beam energy: 920 GeV
- Parton shower: ARIADNE 4.10 (CDM, MSTA(39)=2)
- Fragmentation: JETSET (ALEPH tuning)
- Event weighting: `MCWE 1 150.` (gen-weight required; per-event weight
  in GKI bank word 20)
- Statistics: ~10,000,001 events per sub-sample x 4 sub-samples per
  charge x 2 charges ~= 156 M events per generator for the 27.6-GeV set.

### 3.5 Known caveat: H1OO version mismatch (gen-level-safe)

Per the lambda_HiQ inventory Section 6: the Britzger NonRad files are
**pre-MC13** (likely oo-4.0.6, the LCG-rel4.1.1 production chain).
The v5b SIDIS HighQ2 RAD MC is **MC13** (oo-4.0.25). For
**generator-level** C_rad (`Strategy 2`), this mismatch is physically
irrelevant: gen-level kinematics (PDG, GenEta, GenPt, Q2sGen, YsGen,
GenElecEn, GenEmpz, VtxZgen) are produced by DJANGOH itself, before the
H1OO detector simulation. The H1OO version only affects reco-level
quantities, which do not enter a gen-level rad/nonrad ratio.

If a reco-level C_rad were ever required (it should not be for the
internal package), the Britzger `dst2all_djangoh14_RAD/` set is the
correct matched RAD; pairing it with Britzger NORAD eliminates the
version mismatch concern at the cost of using fewer RAD events than
the v5b MC13 RAD provides.

---

## 4. Strategy Comparison

The four strategies named in the task brief:

### Strategy 1 -- Standard rad/nonrad gen-level ratio from v5b builder output

- **Inputs:** v5b builder NonRad output (ChargedTotGen tree built by
  `ProtonTreeBuilderHighQ2` running on Britzger NonRad HAT files).
- **Currently executable:** **NO** -- v5b NonRad builder output does not
  exist (Section 2).
- **Cost to make executable:** Launch a dedicated NonRad Condor v5b
  production. Estimated effort: 1-2 analyst days (steering generation,
  submit harness adaptation, validation) + 2-5 days Condor wallclock
  (NonRad sample is ~520 + 520 + 528 = ~1568 HAT-input units; assume
  one job per HAT, similar throughput to the completed 1086 RAD MC
  jobs).
- **Risk:** Low. Same builder, same cuts, same schema as RAD; output
  drops directly into the existing analysis pipeline.
- **Verdict:** Acceptable but **slow**. Counts as a new Condor
  campaign; should not gate G10 if Strategy 2 is viable.

### Strategy 2 -- Gen-level rad/nonrad ratio from HAT directly (RECOMMENDED)

- **Inputs:** Britzger NonRad HAT files (Section 3) + v5b RAD MC merged
  ROOT (Section 2). For internal consistency the RAD side can be
  computed two ways:
  - **2a (preferred):** Use the v5b `ChargedTotGen` tree from
    `merged_highQ2_mc_djangoh14.root` for the RAD numerator. Build a
    matching gen-level extraction from the Britzger NORAD HAT files for
    the NonRad denominator using a lightweight ROOT/uproot script.
  - **2b (cross-check):** Use the Britzger RAD HAT files
    (`dst2all_djangoh14_RAD/`) for the RAD numerator, paired with
    Britzger NORAD HAT for the NonRad denominator. Same gen-loop
    script, fully consistent production chain.
- **Currently executable:** **YES**, immediately. All inputs exist on
  NAF; the only new artifact is a gen-loop script.
- **Cost to make executable:** ~1 analyst day to write
  `compute_highQ2_crad_charged.py` (a HAT-driven gen-loop using
  `uproot` or PyROOT; should reuse the cut set from the V0 reference
  in `research-crad-for-charged-sidis.md` Section 6, adapted to
  high-Q2 fiducial `Q2 > 150`, charged-hadron PDG, and the four
  beam-charge / beam-energy paths in Section 3.3 above). Runtime over
  the NORAD HAT set is small (HAT files are 26 MB; gen-level event-loop
  is much cheaper than reco processing).
- **Risk:** The gen-loop must read GKI word 20 for the per-event
  weight (`MCWE 1 150.` weighting scheme) -- this is a known idiom; see
  lambda_HiQ inventory Section 11 open question 3. Misreading the
  weight would bias the absolute luminosity normalization; the
  rad/nonrad RATIO is partly self-correcting only if the same weight
  scheme applies to both, which here it does NOT (RAD uses a different
  weighting). The lambda_HiQ note 3 explicitly flags that NonRad MC
  luminosity must be computed from sum-of-weights, not event count.
- **Verdict:** **RECOMMENDED.** Closes G10 without a Condor campaign.
  Carries a moderate per-bin uncertainty from MC statistics (NonRad has
  ~156 M events per generator, which is ample for coarse Q2 bins).
- **Note on per-charge:** Strategy 2 supports per-charge `C_rad(h+/-)`
  trivially because the HAT files store `MCParticles` (H1PartMC array)
  with PDG -- the gen-loop already requires PDG-based species and
  charge separation.

### Strategy 3 -- Borrow published high-Q2 C_rad values

- **Inputs:** A published H1 high-Q2 SIDIS or inclusive analysis with
  per-bin C_rad tables that overlap the (Q2, x, z, pT_h) phase space of
  this analysis.
- **Currently executable:** Unverified, but unlikely to be a clean
  drop-in. The H1 inclusive NC analyses (e.g., DESY 12-107, the
  h1mpim13 publication that motivated the Britzger production) publish
  C_rad in (Q2, x) only; SIDIS-binned C_rad in (z, pT_h) at Q2 > 150
  GeV2 is not, to my knowledge, in any public H1 paper.
- **Cost:** literature search + binning interpolation. Carries
  irreducible model-dependence risk because the borrowed C_rad uses a
  different species set, different fragmentation, different fiducial.
- **Verdict:** **Fallback only.** Acceptable for sanity-checking the
  Strategy 2 central value, not for nominal.

### Strategy 4 -- Placeholder C_rad with bounded uncertainty

- **Inputs:** Set `C_rad = 1` per bin; assign a flat ~5-10% uncertainty.
- **Currently executable:** YES (trivial).
- **Verdict:** **NOT ACCEPTABLE** for G16 internal package per plan-r4
  G10-as-dependency-of-G16 rule (Section "Risks", line 840). Allowed
  only for low-Q2 MVP; the high-Q2 internal package explicitly
  forbids it.

### Strategy ranking

| Rank | Strategy | Executable today | Cost | Closes G10 |
|------|----------|------------------|------|------------|
| 1 | **Strategy 2 (gen-loop on HAT)** | **YES** | ~1 analyst day | **YES** |
| 2 | Strategy 1 (full v5b NonRad rebuild) | NO | 1-2 days + 2-5 days Condor | YES |
| 3 | Strategy 3 (literature borrow) | maybe | 1-3 days literature | partial (cross-check only) |
| 4 | Strategy 4 (placeholder) | YES | 0 | NO (violates plan rule) |

---

## 5. C_rad Cancellation Behavior in SIDIS Observables

This section reconfirms the plan-r4 G10 statement on cancellation, with
explicit reasoning. Source: the V0 + low-Q2 SIDIS research note
`research-crad-for-charged-sidis.md` Sections 2.6, 2.7; the
H1 systematics matrix in `reference-sidis-systematics.md` lines 138 and
152; and the V0 application script `lambda/scripts/apply_crad.py` lines
204-233.

### 5.1 Per-observable cancellation table

| Observable | Definition | C_rad cancellation? | Reasoning |
|------------|------------|---------------------|-----------|
| `M^{h+/-}(z, pT_h, Q2, x)` (multiplicity) | `N_h / (N_DIS * dz * dpT_h)` | **DOES NOT CANCEL** -- enters as a multiplicative bin-by-bin correction. Specifically: `M_Born = M_meas * (C_rad_DIS / C_rad_h)`. The DIS-event correction in the denominator and the per-hadron correction in the numerator approximately cancel to first order but a per-bin residual O(1%) remains. | Numerator (`N_h`) and denominator (`N_DIS`) each receive separate rad corrections; the ratio of the two corrections is what survives. |
| `A_h = (N_h+ - N_h-) / (N_h+ + N_h-)` (charge asymmetry at fixed beam) | hadron-charge asymmetry | **CANCELS** to first order **iff** C_rad is hadron-charge-symmetric (the standard assumption: QED radiation acts on the leptonic side and is hadron-charge-blind). The per-charge difference `C_rad(h+) - C_rad(h-)` is expected to be sub-percent at high-Q2; the correction to A_h is at the per-mil level. Classify in the systematics matrix as **"partial"** per `reference-sidis-systematics.md` line 138; do not assume full cancellation without measuring. | The DIS-event factor cancels exactly in the per-charge numerator and denominator ratios; the per-charge `C_rad_h` differences come only from gen-fiducial migration. |
| `A_BC(charge-summed) = (N_{e+p} - N_{e-p}) / (N_{e+p} + N_{e-p})` (beam-charge asymmetry, charge-summed hadrons) | beam-charge asymmetry on total hadron yield | **DOES NOT CANCEL.** The QED corrections differ between e+p and e-p because the gamma-Z interference sign flips (the interference contribution to dsigma/dQ2 changes sign with lepton charge), and ISR/FSR + Compton-radiative subtraction pieces have different magnitudes for opposite beam charges. C_rad for e+p and C_rad for e-p must be computed **separately**, then enter A_BC as a non-trivial ratio: `A_BC_Born = (N_{e+p}/C_rad^{e+p} - N_{e-p}/C_rad^{e-p}) / (N_{e+p}/C_rad^{e+p} + N_{e-p}/C_rad^{e-p})`. | Different beam-sign physics dominates the rad correction. |
| `A_BC(h+)` and `A_BC(h-)` (beam-charge asymmetry per hadron charge) | per-hadron-charge BC asymmetry | **DOES NOT CANCEL** -- same as A_BC(charge-summed). Requires charge-separated AND beam-charge-separated C_rad: four independent C_rad tables: `C_rad(h+, e+p)`, `C_rad(h-, e+p)`, `C_rad(h+, e-p)`, `C_rad(h-, e-p)`. | Same e+p vs e-p logic as A_BC, but resolved by hadron charge. |

### 5.2 One-line summary

```
M^{h+/-}                : DOES NOT CANCEL (residual ~1% per bin)
A_h                     : CANCELS to first order (per-charge residual sub-%)
A_BC(charge-summed)     : DOES NOT CANCEL (e+p vs e-p physics differs)
A_BC(h+) and A_BC(h-)   : DOES NOT CANCEL (charge + beam-sign separated)
```

This reconfirms the G10 plan requirement.

### 5.3 Required number of independent C_rad tables for the full
high-Q2 SIDIS package

| Object | Need? | Notes |
|--------|-------|-------|
| `C_rad_DIS(Q2, x)` (event-level, beam-charge-summed) | YES, for `M^{h+/-}` denominator | One per (Q2, x) bin |
| `C_rad_DIS(Q2, x; e+p)` and `C_rad_DIS(Q2, x; e-p)` | YES, for A_BC denominators | Beam-sign separated |
| `C_rad(h+, bin)` and `C_rad(h-, bin)` | YES, for `M^{h+/-}` numerator and for A_h per-charge correction | Charge-separated, beam-charge-summed |
| `C_rad(h+, e+p, bin)`, `C_rad(h-, e+p, bin)`, `C_rad(h+, e-p, bin)`, `C_rad(h-, e-p, bin)` | YES, for A_BC(h+), A_BC(h-) | Fully four-way separated |

The Britzger NonRad samples have full EPLUSP / EMINUSP split (Section
3.1, 3.2), so this four-way separation is achievable from a single
gen-loop pass.

---

## 6. Per-Charge C_rad Practicality at High-Q2

**Verdict: practical, with current samples, no builder rerun required.**

Two pieces of evidence:

1. **v5b high-Q2 builder writes per-charge gen info.**
   `H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` confirmed on NAF to
   contain `ChargedTotGen` tree with `GenCharge` branch (filled at line
   898 from `ProtonUtil::chargeSignForPDG(GenPDG_cg)`). So the RAD-side
   gen-level charged hadron yield can be split by hadron charge from
   the existing `merged_highQ2_mc_djangoh14.root`.

2. **Britzger NonRad HAT files have full beam-charge separation.**
   EPLUSP and EMINUSP HAT files are independent (Section 3.1 table).
   The gen-loop reads `MCParticles` with PDG, so hadron charge splitting
   is available; the file-level EPLUSP/EMINUSP split gives the beam
   charge directly. The four-way table is therefore producible.

Caveat: for the v5b RAD side, the existing `merged_highQ2_mc_*.root`
files are merged across periods. If the period-tagged file manifests
required by G15 (e- recovery) are available -- they are pending per
the plan G15 -- then the RAD side can also be beam-charge separated
from the existing merged file (`RunNumber` is a stored branch). If
not, a per-period RAD merge would be required.

---

## 7. Historical Recommendation — SUPERSEDED for current z005

> **Current z005 supersession:** Do not use the recommendation below for current high-Q² z005 `C_rad`. The maintained baseline is DJANGOH14 gen-only v1 under `studies/radiative_correction_genonly_v1/` with `C_rad = σ_nonrad/σ_rad`. RAPGAP31 is anomalous/quarantined and must not be used as nominal or as a generator-envelope systematic until provenance/lumi/sample issues are resolved.

**Original May-2026 recommendation (audit only): adopt Strategy 2 for G10 closure.** Specifically:

1. **Nominal:** DJANGOH14 RAD (v5b ChargedTotGen) / DJANGOH14 NORAD
   (Britzger HAT gen-loop) ratio, per kinematic bin, in the four-way
   beam-charge x hadron-charge split.

2. **Generator-model systematic (SUPERSEDED — do not follow for current z005):** Repeat with RAPGAP31 RAD (v5b) /
   RAPGAP31 NORAD (Britzger). Assign half the DJANGOH-vs-RAPGAP
   difference as the model uncertainty (standard H1 convention).

3. **Production-version cross-check:** Pair Britzger RAD
   (`dst2all_djangoh14_RAD/`) with Britzger NORAD via the same gen-loop
   script. If the gen-level C_rad agrees with the v5b-RAD/Britzger-NORAD
   pair within 1-2%, the pre-MC13 vs MC13 H1OO version concern is
   bounded.

4. **Implementation entry point:** A new script
   `H1/sidis_highQ2/scripts/compute_highQ2_crad_charged.py` (analogous to
   the low-Q2 `compute_crad_charged.py` recipe in
   `research-crad-for-charged-sidis.md` Sections 6.3-6.4), but adapted
   to:
   - HAT-level NonRad inputs (single-file steering, no MODS/ODS access
     needed for gen-only)
   - high-Q2 fiducial: `Q2 > 150`, hadron `pT > ...`, `|eta| < ...`
     (read from the high-Q2 binning manifest once G5 is frozen)
   - GKI word 20 weight reading for NonRad sample (MCWE 1 150 weighting)
   - per-charge AND per-beam-sign output tables

5. **Strategy 1 is the fallback only if** the gen-loop reveals an
   unexpected reco-level dependence that gen-only cannot capture (not
   expected, but possible if e.g. the high-Q2 z definition depends on
   reco quantities not present in the HAT trees).

**Does NOT require a Condor v5b-NonRad production launch.** G10 can
close on Strategy 2 within ~1 analyst day of scripting + runtime,
provided the high-Q2 binning is frozen (G5).

---

## 8. Open Items for Supervisor

1. **Confirm gen-level-only C_rad is acceptable for G10.** The
   lambda_HiQ analysis (its DONE status row in
   `lambda_HiQ/context-handoff.md` line 113) explicitly used
   "C_rad from DJANGOH14 rad/nonrad, cross-checked with RAPGAP" at gen
   level. The same paradigm should apply to SIDIS at high-Q2.
   Supervisor sign-off recommended before script writing.

2. **Historical RAPGAP31 systematic question — superseded for current z005.** RAPGAP31 is now anomalous/quarantined and must not be used as a generator-model systematic or nominal `C_rad` until provenance/lumi/sample issues are resolved. Per Section 4.4 of the lambda_HiQ inventory, RAPGAP was already not the H1 standard for C_rad because it is not as consistently configured between RAD and NORAD modes as DJANGOH; current z005 evidence strengthens the exclusion/quarantine stance.

3. **Confirm the period mapping in Section 3.3.** The high-Q2 SIDIS
   ep0607-only baseline currently in the data path means the
   immediately-needed C_rad subset is the `dst2all_djangoh14_NORAD`
   EPLUSP set (one of four sub-samples A-D). The other three periods
   become relevant when G15 (e- period recovery) unlocks `ep0304`,
   `em0405`, `em06`.

4. **G15 dependency for A_BC C_rad.** A_BC requires e+p and e-p data
   in the same kinematic range. C_rad for A_BC therefore requires
   both EPLUSP and EMINUSP NonRad gen-level extraction, which is fine
   on the sample side, but the data side is not yet available
   pre-G15. Recommend treating G10's A_BC sub-deliverables
   (`highQ2_crad_abc.json`) as **deferred until G15 unlocks the e-
   data path**. G10's multiplicity deliverable
   (`highQ2_crad_nominal.json`) can close on EPLUSP-only.

---

## 9. What I Did NOT Verify

- The exact disk content of one Britzger NonRad HAT file by opening it
  on NAF with ROOT (the inventory above confirms file presence, size,
  and naming pattern; per the lambda_HiQ inventory the HAT contains the
  required GKI/MCParticles content, but a fresh smoke-open on EL9 with
  oo-4.2.1 is recommended before serious gen-loop work).
- The exact gen-level branch names in the HAT trees (the lambda_HiQ
  inventory Section 4.1 lists the H1OO classes; a one-file smoke test
  with `TFile::ls()` and `T->Print()` is recommended to confirm the
  flat branch names before scripting).
- Whether the v5b high-Q2 merged MC retains period information at the
  per-event level (`RunNumber` is in the v5b ChargedTotGen schema per
  the low-Q2 handoff Section 4.4; presumed identical for high-Q2 but
  not explicitly confirmed in this inventory).
- The per-period luminosity values for the Britzger NonRad samples
  (the lambda_HiQ note Section 11 question 3 explicitly leaves this
  open; sum-of-weights must be computed from GKI word 20).
- Whether RAPGAP31 NonRad has the same charge-blind generator behavior
  as DJANGOH14 NORAD (it should, per the steering-file documentation
  in the lambda_HiQ inventory, but not directly inspected here).

---

## 10. References

| Path | Used for |
|------|----------|
| `H1/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md` | G10 definition and exit criteria (Section 1 above) |
| `H1/sidis_highQ2/docs/research/research-crad-for-charged-sidis.md` | C_rad formula, V0 implementation, low-Q2 recipe template |
| `H1/sidis_lowQ2/context-handoff.md` | Phase 5 placeholder `crad_model_swap` reason ("NonRad charged-tree MC unavailable") |
| `H1/lambda_HiQ/docs/research-nonrad-inventory.md` | **Primary source** for Britzger NonRad sample inventory (Section 3) |
| `H1/lambda_HiQ/context-handoff.md` | Documents successful use of DJANGOH14 rad/nonrad C_rad at high-Q2 for the Lambda analysis |
| `H1/proton_HiQ/docs/research-highQ2-proton-asymmetry.md` Section 3.4 | Cross-reference: independent confirmation that NonRad samples live in `britzger/norad_prod21/` |
| `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md` line 250 | Same NonRad gap statement for proton_HiQ |
| `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md` lines 138, 152 | Cancellation classification reference |
| `H1/sidis_highQ2/docs/data/highQ2_nonrad_mc_inventory.json` | Machine-readable companion to this note |
| NAF: `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/` | Verified absence of NonRad v5b builder output (Section 2) |
| NAF: `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/` | Verified presence of HAT/MODS/ODS files (Section 3.1) |
| NAF: `/data/dust/group/h1/britzger/norad_prod21/dst2all_rapgap_Ee27p{5,6}/` | Verified presence of RAPGAP31 NonRad HAT files (Section 3.2) |
| NAF: `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` | Confirmed `ChargedTotGen` writes `GenCharge` (Section 6) |

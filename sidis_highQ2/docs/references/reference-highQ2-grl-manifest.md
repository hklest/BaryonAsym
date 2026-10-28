# Reference: High-Q2 Charged-Particle SIDIS GRL Manifest (FROZEN ep0607, IDENTIFIED rest)

**Date:** 2026-05-19
**Scope:** Authoritative good-run-list (GRL) inventory for the H1 high-Q2
charged-particle SIDIS analysis (Q2 > 150 GeV^2) across all four HERA-II
e+/e- periods. ep0607 drives the currently-available analysis path; the
other three drive the G15 e- period recovery.
**Audience:** `h1-sidis-supervisor`, normalization, A_BC weighting, and the
production submitter (`submit_full_production.py`).
**Source-of-truth files (upstream origin):** `H1BaryonAsym_HighQ2/LumiFiles/nc_goodruns_*.root`
on NAF (oolumi outputs). **SIDIS-local copies** (bit-identical, md5-verified) exist at
`H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_*.root` since Phase 1A-fix (2026-05-23).
The three SIDIS submission wrappers now READ from the SIDIS-local path.
**Companion artifact:** `H1/sidis_highQ2/docs/data/grl_manifest_highQ2.json`.

This is NOT the V0/Lambda/K0s GRL and NOT the low-Q2 (S61) GRL.

---

## 1. Summary Table

| Period | Beam | GRL file (NAF) | Run count | First-Last run | L (nb^-1) | Trigger | Status |
|---|---|---|---:|---|---:|---|---|
| **ep0304** | e+p | `nc_goodruns_0304.root` | 4,017 | 357160 - 392213 | 53,706.105 | S67 | IDENTIFIED |
| **em0405** | e-p | `nc_goodruns_0405.root` | 5,194 | 399629 - 436893 | 105,028.172 | S67 | IDENTIFIED |
| **em06** | e-p | `nc_goodruns_06.root` | 3,486 | 444312 - 466997 | 57,573.094 | S67 | IDENTIFIED |
| **ep0607** | e+p | `nc_goodruns_0607.root` | 8,091 | 468532 - 500611 | 140,306.219 | S67 | **FROZEN** |
| **Total** | | | **20,788** | | **356,613.59** | S67 | |

All GRL files originate from `/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/LumiFiles/` (upstream oolumi source). Bit-identical SIDIS-local copies (md5-verified, Phase 1A-fix 2026-05-23) live at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/LumiFiles/`. The SIDIS submission wrappers read from the SIDIS-local path.

### Numerical reconciliation against documented anchors

- Per-period lumi sum = **356,613.59 nb^-1** vs the supervisor anchor of **356,613 nb^-1** (snapshot Section 3.1, proton_HiQ/research-highQ2-proton-asymmetry.md Section 5.1). **Match within 0.59 nb^-1 (rounding only).**
- ep0607 alone = **140,306.219 nb^-1** vs the anchor of **140,306 nb^-1**. **Match within 0.22 nb^-1 (rounding only).**
- e- subtotal (em0405 + em06) = **162,601.266 nb^-1**.
- e+ subtotal (ep0304 + ep0607) = **194,012.324 nb^-1**.
- Currently-in-data lumi (ep0607 only) = 140,306.219 nb^-1 = **39.35%** of recoverable.
- Recoverable to add via G15 = ep0304 + em0405 + em06 = **216,307.371 nb^-1**.

### Disjointness check

- Sum of per-period run counts (20,788) equals the union of all run numbers (20,788) ⇒ **zero overlap between any pair of periods**.
- Run-number boundary em06.LastRun=466997 < ep0607.FirstRun=468532 (gap of 1535 runs not in either GRL), consistent with the documented `/06/` split between em06 (chunks c060220-c060319) and ep0607 (chunks c060000-c060212 + 2007), per `proton_HiQ/docs/research-data-coverage-gap.md`.

---

## 2. What "FROZEN" vs "IDENTIFIED" Means

- **FROZEN (ep0607):** The GRL is internally consistent, matches the supervisor
  anchor lumi within rounding, and is the active selection driving the
  currently-available SIDIS high-Q2 analysis path. No further validation is
  required for the GRL itself before downstream use. The "frozen" status
  applies to the **GRL object only**, not to the data sample - see
  Section 5 blocker.
- **IDENTIFIED (ep0304, em0405, em06):** The GRL is located, opens cleanly,
  matches the per-period anchor lumi, and is ready to be wired into a v5b
  SIDIS high-Q2 production. The G15 e- recovery requires submitting
  Condor jobs against these GRLs (no v5b SIDIS-schema data exists for
  these periods yet on DUST/pnfs).

---

## 3. Trigger and Selection Encoded in the GRL

All four GRLs were produced by `oolumi` with **identical** detector and
vertex selection. The only per-period differences are the run range
(`fFirstRun`/`fLastRun`) and the period-specific excluded-runs lists.

| Steering field | Value | Meaning |
|---|---|---|
| `fSubTrigs` | "67" | **S67 (LAr NC DIS)** required |
| `fPrescaleLimit` | 1 | Reject runs with effective prescale > 1 |
| `fCJC1`, `fCJC2` | 1, 1 | CJC1 and CJC2 on |
| `fLAR` | 1 | LAr on |
| `fTOF` | 1 | TOF on |
| `fLUMI` | 1 | Lumi system on |
| `fCIP` | 1 | CIP on |
| `fCaloTrig` | 1 | Calorimeter trigger on |
| `fZ0`, `fDZ` | 0.0, 35 cm | Vertex window |z_vtx| < 35 cm |
| `fComptonCorrection` | 1 | Apply Compton lumi correction |
| `fMinRunLumi` | 0.2 nb^-1 | Reject runs with delivered lumi < 0.2 nb^-1 |

Steering files (per period, source code committed alongside the GRLs):

- `nc_lumi_0304.steer` - excludes run ranges 360177-360281 (armon), 361882-362486 (CIP), 372336-372619 (CIP), 384836-385073 (CIP after powerglitch), and 9 single low-yield/DAQ-bad runs.
- `nc_lumi_0405.steer` - excludes 404826-404848 (CTC test), 408903-408927 (CIP), 409345-410185 (LAr hot cells), 409350-409371 (CIP), 414908-418000 (LAr HV), 424009-424015 (LAr noise test), plus 6 single bad runs.
- `nc_lumi_06.steer` - excludes single run 445735 (low yield, CJC+CIP off).
- `nc_lumi_0607.steer` - excludes 495235-495260 (LAr noise scanning) and 9 single low-NC-yield runs from Andrei.

**This is the same S67 selection used by `proton_HiQ` and `lambda_HiQ`.** The GRL content
is identical across all three analyses. `proton_HiQ` and `lambda_HiQ` still read directly
from `H1BaryonAsym_HighQ2/LumiFiles/`. SIDIS now reads from its own bit-identical copies
at `H1ChargedSidis_HighQ2/LumiFiles/` (Phase 1A-fix, 2026-05-23; md5-verified). Do NOT
regenerate the SIDIS-local copies independently — re-sync from `H1BaryonAsym_HighQ2/LumiFiles/`
with md5-verification if the upstream GRL is ever updated.

---

## 4. Why these GRLs (not the V0 / low-Q2 GRLs)

| Analysis family | GRL family | Trigger | Total L | NOT for SIDIS high-Q2? |
|---|---|---|---:|---|
| **Low-Q2 SIDIS / V0 / proton** | `SelectedRuns.root` (S61) | S61 (SpaCal DIS) | the superseded pre-rescale luminosity nb^-1 | **DO NOT USE.** Wrong trigger; covers only 2005-2007. |
| **High-Q2 SIDIS / proton / lambda** | `nc_goodruns_*.root` (S67) | S67 (LAr NC DIS) | 356,613 nb^-1 | This document's authoritative set. |

The high-Q2 GRLs are the source-of-truth because:
1. High-Q2 data enforces **S67** at the event-list level (handoff Section
   "Frozen Analysis Decisions" lines 1019-1023). The low-Q2 S61 GRLs do
   not encode the LAr-NC subtrigger requirement and would be incorrect.
2. The high-Q2 lumi includes the 2003-2004 e+ period that S61/low-Q2
   GRLs do not cover (low-Q2 = 2005+2006+2007 only).
3. `proton_HiQ` and `lambda_HiQ` already adopted these GRLs (the proton
   high-Q2 analysis ran on the ep0607 GRL to produce its 140,306 nb^-1
   result; lambda_HiQ uses the same family). Cross-analysis consistency.

---

## 5. Cross-Reference: GRL vs Produced Data Per-Run Files (ep0607)

Per task 2 (`H1/sidis_highQ2/docs/data/validation_highQ2_data_availability.json`,
dated 2026-05-19):

- The v5b period-tagged subdirs `H1SIDIS_HighQ2/output/data/ep0607_06/`
  and `.../ep0607_07/` both contain **0 ROOT files**.
- The legacy `H1SIDIS_HighQ2/output/data/06/` (313 files) and `.../07/`
  (109 files) contain proton-asymmetry-schema files
  (`ProtonTot=56 branches`, NO `ChargedTot`/`DisTot`). These cannot
  drive the SIDIS analysis.
- No merged high-Q2 data ROOT file exists at
  `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/`.

**Conclusion:** A direct comparison of the 8,091-run ep0607 GRL set
against the produced run set is **not possible today**, because no
SIDIS-schema v5b high-Q2 data exists for any period. The per-chunk file
naming (`proton_highQ2_d07_NNNN.root` — legacy proton-prefixed name from the
v5b SIDIS fork; see `docs/references/reference-naming-aliases-highQ2-sidis.md`)
does not carry the run number, so
the cross-check would require opening each ROOT file and reading the
RunNumber branch. **Deferred as FU1 in the JSON manifest, owned by
task 2 once SIDIS-schema v5b data lands.**

---

## 6. Blockers and Follow-Ups

### Blocker (carried forward from task 2)

**No SIDIS-schema v5b high-Q2 data exists for ANY period (including
ep0607).** The "FROZEN" status for the ep0607 GRL applies to the GRL
object on disk only - the downstream data sample is empty for SIDIS.
This must be resolved before the high-Q2 SIDIS analysis can produce any
multiplicity or asymmetry number, even on the ep0607-only path.

### FU1 - GRL vs production run-set cross-check (owner: task 2)

Once SIDIS-schema v5b ep0607 data lands, open each per-run file, read
the RunNumber, and diff against `grl_runs_ep0607.txt` (8,091 entries,
extracted to `/tmp/grl_runs_ep0607.txt` on NAF and to
`/var/folders/n1/zy1hghps5tlg9538vlknzmdm0000gs/T/opencode/grls/`
locally).

### FU2 - Wire all four GRLs into the production submitter (owner: production)

Follow the period-to-GRL mapping in
`proton_HiQ/docs/research-data-coverage-gap.md` lines 84-89:

```
("03", "ep0304"): "LumiFiles/nc_goodruns_0304.root",
("04", "ep0304"): "LumiFiles/nc_goodruns_0304.root",
("05", "em0405"): "LumiFiles/nc_goodruns_0405.root",
("06", "em06"):   "LumiFiles/nc_goodruns_06.root",
("06", "ep0607"): "LumiFiles/nc_goodruns_0607.root",
("07", "ep0607"): "LumiFiles/nc_goodruns_0607.root",
```

Pass `--good-runs <path>` explicitly in
`H1SIDIS_HighQ2/scripts/submit_full_production.py`. The current default
in `TreeBuilder.C` (line 250 per the proton_HiQ research) is
`nc_goodruns_0607.root`, which rejects all non-ep0607 runs. This is
the documented G15-recovery configuration fix.

### FU3 - A_BC weighting policy (owner: supervisor)

After G15 recovery the e+/e- breakdown becomes:

- e+ (ep0304 + ep0607) = 194,012.324 nb^-1
- e- (em0405 + em06)   = 162,601.266 nb^-1
- Ratio = 1.193 (not 1.000)

Decide whether the asymmetry is computed beam-charge-symmetric (requires
re-weighting one side) or beam-charge-weighted (uses the full delivered
lumi mix). This is a normalization-policy question, not a GRL question.

---

## 7. Verification Provenance

- **SSH:** `ssh -O check naf-hone.desy.de` returned `Master running (pid=23461)` at 2026-05-19.
- **H1RunList read:** loaded `libEG`, `libH1Tools`, `libH1Mods` from
  `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/lib/x86_64-el9-gcc14-opt/`,
  added include path
  `-I/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo`, then
  ran the macro `/tmp/dump_grl.C` (single-pass per-file
  `TFile::Open` -> `H1RunList::GetNrOfRuns/GetIntLumi/FirstRun/LastRun`).
- **Per-run dump:** ran `/tmp/run_extractor.C` to extract the full
  run-number lists via `H1RunLumi::GetRunNumber()` and write to
  `/tmp/grl_runs_{ep0304,em0405,em06,ep0607}.txt`. Copied locally to
  `/var/folders/n1/zy1hghps5tlg9538vlknzmdm0000gs/T/opencode/grls/`.
- **Steering files:** `cat`-ed all four
  `LumiFiles/nc_lumi_*.steer` files; confirmed identical trigger/detector/vertex selection across the four.

No remote command was retried; no `find` was run on `/cvmfs`, `/afs`, or
unrestricted `/pnfs`; no file was written on NAF outside `/tmp`.

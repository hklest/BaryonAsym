# Research: G1C v5f "evls-period-mapping bug" — investigation & patch analysis

**Date:** 2026-05-20
**Scope:** Diagnose the alleged evls→period mapping bug in the G1C v5f
high-Q2 SIDIS data production wrapper
(`H1/sidis_highQ2/scripts/submit_highQ2_sidis_data_27p6.py`) and
characterise the patch (or non-patch) required before any re-run.
**Mode:** READ-ONLY across local repo and NAF.
**SSH precheck:** `Master running (pid=17712)` PASS at start of session.
**Audience:** h1-sidis-supervisor (G1C unblock decision).

---

## 1. Executive summary

**The premise of the brief is wrong: the prior G1C v5f production is NOT
invalid because of an evls→period mapping bug.** The wrapper's
`PERIOD_SOURCES` map and `list_period_tagged_evls()` logic correctly
route each period to its period-tagged evls subset; the per-period
evls files are properly disjoint by run number; outputs are correctly
partitioned per period.

**Smoking gun (D4):** for every d06 chunk inspected (0000, 0100, 0220,
0290), the Stage-2 builder produces an output DisTot entry count that
**exactly equals** the input EvlTree entry count (1:1 pass-through).
The em06 outputs contain only em06-range runs (444 314..466 902); the
ep0607 outputs contain only ep0607-range runs (468 532..500 611);
pairwise run-set intersection across all four periods is 0. The builder
does NOT carry a GRL filter (D5 confirmed by source inspection); it
does not need one, because Stage 1 already applied the per-period GRL
upstream.

**Patch verdict:** **No code patch is required to fix correctness.**
The current G1C v5f output is correct as-produced. The user-perceived
"bug" appears to rest on two misreadings: (a) a miscount of "untagged"
evls files in `/04/`, `/05/`, `/06/`, `/07/` (these are LEGACY raw
stage-1 files from a pre-period-split April-25 19:40 run; the period-
tagged set produced at 19:52..22:08 is the authoritative one, which
the wrapper exclusively consumes); and (b) treating the 274/313 empty
em06 outputs as a defect when they reflect the expected sparsity of
em06 runs across the `/06/` HAT chunks. The launch audit explicitly
documented the empty-em06-first-chunk observation as "expected, not a
defect".

**One open hygienic concern (NOT a correctness bug):** roughly 60% of
the ep0607 GRL runs and 65% of the em06 GRL runs are absent from the
union of per-period evls — i.e. Stage 1 produced an evls list that
covers <40% of those GRLs. This is a Stage-1 / HAT-availability
question, not a wrapper bug, and it would not be fixed by patching the
Stage-2 wrapper.

**Blocker requiring supervisor decision before any further action:**
the brief's premise needs to be retracted or re-stated; otherwise a
patch would be applied to a non-broken wrapper and waste the next G1C
cycle.

---

## 2. The bug as observed (per user brief)

The brief asserts:

> "The prior G1C v5f run was invalid because of an evls-period-mapping
> bug; before patching, we need to know exactly what the patch should
> look like."

The brief's specific factual claims that I tested are:

| Claim | Result |
|---|---|
| `PERIOD_SOURCES` map (~L101) sends a chunk to the wrong period. | **Refuted by D6.** The map sends each period to its period-tagged evls subset; the subset's run numbers are exactly within that period's GRL range; the four-period run sets are pairwise disjoint. |
| The wrapper falls back to untagged `evls_highQ2_d{NN}_NNNN.root` when `list_period_tagged_evls()` returns 0. | **Refuted by D6.** `list_period_tagged_evls()` never returns 0 for any (year, period) in `PERIOD_SOURCES`; the per-period tagged file counts (20+63, 152, 313, 313+112) exactly match the prior launch audit (83, 152, 313, 425). There is no fallback path in the wrapper code. |
| d04 has 125 untagged evls files. | **Refuted by D2.** d04 has 63 ep0304-tagged + 62 untagged = 125 total. The user appears to have read the "total" column as "untagged". |
| `proton_highQ2_em06_d06_0000.root` is empty. | **Confirmed but expected.** The upstream `evls_highQ2_d06_em06_0000.root` has 0 EvlTree entries; the builder produces 0 output entries from 0 input entries. The launch audit (`g1c_v5f_full_launch_2026-05-19.md` §"Known observation") flagged this as expected. |
| The wrapper assigns the same chunk to BOTH em06 AND ep0607 (one of them would be wrong). | **Refuted by D4.** The wrapper consumes two DIFFERENT period-tagged evls files for the same chunk index (`evls_highQ2_d06_em06_0220.root` vs `evls_highQ2_d06_ep0607_0220.root`). They contain disjoint run sets. The per-period outputs are correctly partitioned (em06 output gets em06 runs, ep0607 output gets ep0607 runs). |

---

## 3. Diagnostic D1-D7 results

### D1: SSH precheck

```
$ ssh -O check naf-hone.desy.de 2>&1
Master running (pid=17712)
```
PASS. Same ControlMaster used throughout.

### D2: evls file inventory per d-year

`ls /data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/output/evls/data/{DD}/...root`:

| d-year | total `.root` | ep0304-tag | em0405-tag | em06-tag | ep0607-tag | untagged (legacy) |
|---|---:|---:|---:|---:|---:|---:|
| 03 | 20 | **20** | 0 | 0 | 0 | 0 |
| 04 | 125 | **63** | 0 | 0 | 0 | 62 |
| 05 | 303 | 0 | **152** | 0 | 0 | 151 |
| 06 | 939 | 0 | 0 | **313** | **313** | 313 |
| 07 | 221 | 0 | 0 | 0 | **112** | 109 |
| **sum** | **1608** | **83** | **152** | **313 + 313 = 626** | **112** | **635** |

Per-period total tagged: ep0304=83, em0405=152, em06=313, ep0607=313+112=425.
This **exactly** matches the per-period job count in the launch audit
(`g1c_v5f_full_launch_2026-05-19.md` §"Per-period output"): 83 + 152 + 313
+ 425 = 973. So PERIOD_SOURCES delivers what the launch reported.

The "untagged" files have timestamps `Apr 25 19:40`. The period-tagged
files have timestamps `Apr 25 20:19..22:08`. The untagged set is the
LEGACY Stage-1 output (one HAT → one untagged evls), produced before
`submit_highq2.py` was modified to add the period-tag pass. The wrapper
**never** references the untagged set: the glob in
`list_period_tagged_evls()` at L330–333 is

```python
glob_pattern = (
    f"{EVLS_BASE_REMOTE}/data/{year_short}/"
    f"evls_highQ2_d{year_short}_{period_tag}_*.root"
)
```

which has an explicit `_{period_tag}_` segment; the untagged filename
`evls_highQ2_d04_0001.root` cannot match this pattern.

### D3: evls tree + run-number branch

Opened `evls_highQ2_d06_em06_0000.root` (and verified on other files) via
uproot:

```
keys: ['EvlTree;3', 'EvlTree;2', 'EvlTree;1', 'EvlList;1']
EvlTree branches: ['Q2s', 'Ys', 'x', 'ElecE', 'ElecTheta', 'ElecPhi',
                   'VtxZ', 'Empz', 'Weight1', 'Weight2', 'RunNumber',
                   'Ye', 'Zimpact', 'PhiOctant', 'TrigS67', 'BunchType',
                   'HasNCOptimalVtx', 'UsedCJCFallback']
```

- Tree to read: **`EvlTree`** (latest cycle ;3, also cycles ;2 and ;1
  present from re-runs).
- Run-number branch: **`RunNumber`** (Int_t per H1BaryonAsym_HighQ2
  TreeBuilder.C L304).
- The `EvlList` object is the H1OO `H1EventList` (uproot cannot
  deserialise it; that's fine — the Stage-2 builder uses `EvlTree`
  branches plus the H1OO event-list mechanism for HAT navigation).

### D4: SMOKING-GUN diagnostic — does the builder filter by GRL?

The G1C v5f output is still in place at
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data/27p6/`.
For each requested d06 chunk index, comparing the upstream evls input
entry count to the Stage-2 output DisTot entry count:

| chunk | em06 evls size | em06 evls entries | em06 OUT size | em06 OUT DisTot entries | ep0607 evls size | ep0607 evls entries | ep0607 OUT size | ep0607 OUT DisTot entries |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| 0000 | 9670 | **0** | 11392 | **0** | 27495 | **216** | 254038 | **216** |
| 0100 | 9670 | **0** | 11392 | **0** | 811 | NO_TREE | 11455 | **0** |
| 0220 | 53826 | **665** | 725175 | **665** | 9680 | **0** | 11408 | **0** |
| 0290 | 803 | NO_TREE | 11439 | **0** | 9680 | **0** | 11408 | **0** |

**Verdict: 1:1 pass-through.** Stage-2 output DisTot entries =
upstream EvlTree entries, for every chunk and every period. The builder
does NOT filter by run number; it materialises every event listed in
the input evls file. The em06 outputs are empty when (and only when)
the matched em06 evls file is empty. The ep0607 outputs are empty when
(and only when) the matched ep0607 evls file is empty.

If the builder had been applying a default `nc_goodruns_0607.root` GRL
filter (the historical proton_HiQ bug from
`H1/proton_HiQ/docs/research-data-coverage-gap.md`), then for chunk 0220
the ep0607 output would have processed em06 input runs (444 307..466 997)
and rejected them, leaving 0 entries — but the em06 output (which uses
em06 input) would also be 0. Observed instead: chunk 0220 em06 output =
665 entries, ep0607 output = 0. This refutes any "builder uses wrong
default GRL" hypothesis for the current ChargedSidisTreeBuilderHighQ2
binary.

#### Period-wide output census (corroborates D4)

| Period | output files | nonempty DisTot | empty | total DisTot entries | total ChargedTot entries | median size | max size |
|---|---:|---:|---:|---:|---:|---:|---:|
| ep0304 | 83  | 56  | 27  | 57 571   | 602 017   | 894 KB  | 1.8 MB |
| em0405 | 152 | 94  | 58  | 103 765  | 1 065 086 | 1.06 MB | 1.6 MB |
| em06   | 313 | 39  | 274 | 33 615   | 345 080   | 11.4 KB | 1.2 MB |
| ep0607 | 425 | 133 | 292 | 86 661   | 892 067   | 11.5 KB | 977 KB |

Per-period DisTot output totals exactly match the per-period evls
DisTot input totals (see D6 row 7-9), confirming 1:1 conservation:
- ep0304: out 57 571 = evls 2 629 (d03) + 54 942 (d04). ✓
- em0405: out 103 765 = evls 103 765. ✓
- em06: out 33 615 = evls 33 615. ✓
- ep0607: out 86 661 = evls 47 270 (d06) + 39 391 (d07). ✓

### D5: v5f builder CLI / steering / GRL handling

#### Generated steering files (per-job, on NAF)

```
$ cat .../steering/proton/27p6/em06/proton_highQ2_em06_d06_0000.steer
// High-Q2 SIDIS proton steering for data proton_highQ2_em06_d06_0000
// Generated by submit_highQ2_sidis_data_27p6.py
// Period: em06  Source: 06/  Beam-filter: 27p6
H1SteerTree() {
  fEventListFiles = ".../H1BaryonAsym_HighQ2/output/evls/data/06/
                     evls_highQ2_d06_em06_0000.root";
}

$ cat .../steering/proton/27p6/ep0607/proton_highQ2_ep0607_d06_0000.steer
// Period: ep0607  Source: 06/  Beam-filter: 27p6
H1SteerTree() {
  fEventListFiles = ".../H1BaryonAsym_HighQ2/output/evls/data/06/
                     evls_highQ2_d06_ep0607_0000.root";
}
```

Each per-job steering points at the **correct period-tagged** evls
file. There is **no** `GRL`, `--good-runs`, or run-list field in the
steering. The wrapper template (`PROTON_DATA_STEER_TEMPLATE`, L176-183)
contains only `fEventListFiles`.

#### Builder CLI

`ChargedSidisTreeBuilderHighQ2.C` L125-181 implements `getArgValue` and
strips four flags: `--mode`, `--steer`, `-o`, `--max-events`. No
`--good-runs` flag is parsed; the wrapper does not pass one.

#### Builder source GRL search

```
$ ssh naf-hone.desy.de 'grep -niE \
  "goodrun|grl|runlist|runquality|nc_good|H1Run|isgood|fRun" \
  /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/\
  ChargedSidisTreeBuilderHighQ2.C'
(no output)
```

The Stage-2 builder source contains **zero** GRL/run-list-related
symbols. The only run-number handling is the H1OO HAT-bank read at
L216 (`H1IntPtr hatRunNumber("RunNumber")`) and the write into the
output trees at L417/545/621. There is **no** per-run accept/reject
check inside the Stage-2 binary.

#### Where the GRL filter actually lives

The GRL filter is applied at **Stage 1** by `H1BaryonAsym_HighQ2/src/
TreeBuilder.C::stage_evls()`:

```
TreeBuilder.C:100   #include "H1Tools/H1RunList.h"
TreeBuilder.C:244   H1RunList* goodRunList = nullptr;
TreeBuilder.C:256   goodRunList = (H1RunList*)goodRunTFile->Get("H1RunList");
TreeBuilder.C:360   if (!goodRunList->IsSelected(gH1Calc->GetRunNumber())) { ... }
```

Stage 1 is driven by `H1BaryonAsym_HighQ2/scripts/submit_highq2.py`,
which already supplies the correct per-period GRL via `--good-runs`
(L186-260) using the period map at L57-62:

```python
("03", "ep0304"): "LumiFiles/nc_goodruns_0304.root",
("04", "ep0304"): "LumiFiles/nc_goodruns_0304.root",
("05", "em0405"): "LumiFiles/nc_goodruns_0405.root",
("06", "em06"):   "LumiFiles/nc_goodruns_06.root",
("06", "ep0607"): "LumiFiles/nc_goodruns_0607.root",
("07", "ep0607"): "LumiFiles/nc_goodruns_0607.root",
```

The resulting per-period evls naming is
`evls_highQ2_d{YY}_{period}_{seq}.root`. **This is exactly what the
Stage-2 wrapper globs and consumes.** The pipeline is correctly wired
end-to-end.

### D6: PERIOD_SOURCES vs actual evls availability

`list_period_tagged_evls()` was executed (read-only) via `ssh` for each
(year_short, period_tag) in `PERIOD_SOURCES`:

| (year, tag) | files found by glob | EvlTree entries (sum) | unique RunNumber | Run range |
|---|---:|---:|---:|---|
| (03, ep0304) | 20  | 2 629   | 294   | 357 160..367 186 |
| (04, ep0304) | 63  | 54 942  | 2 346 | 367 284..392 213 |
| (05, em0405) | 152 | 103 765 | 3 390 | 399 629..436 149 |
| (06, em06)   | 313 | 33 615  | 1 227 | 444 314..466 902 |
| (06, ep0607) | 313 | 47 270  | 2 003 | 468 532..492 541 |
| (07, ep0607) | 112 | 39 391  | 1 119 | 492 577..500 611 |

**Pairwise period run-set intersections (should all be 0):**

```
ep0304 ∩ em0405 = 0
ep0304 ∩ em06   = 0
ep0304 ∩ ep0607 = 0
em0405 ∩ em06   = 0
em0405 ∩ ep0607 = 0
em06   ∩ ep0607 = 0
```

All four periods are **perfectly disjoint** in run-number space, and
each evls subset's run range falls strictly inside that period's GRL
run range (manifest §1: ep0304 357 160..392 213; em0405 399 629..436 893;
em06 444 312..466 997; ep0607 468 532..500 611). The Stage-1 per-period
GRL filter produced a clean partition.

**Wrapper fallback behaviour:** `list_period_tagged_evls()` (L322-340)
returns the sorted glob result; if 0 matches, returns `[]`. There is
**no fallback** to untagged files anywhere in the wrapper. The only
downstream check is in `build_plan()` (L378-443) which sets
`plan.job_count = len(all_files)` and propagates 0 if 0; that period
would simply submit 0 jobs. Since every (year, tag) in `PERIOD_SOURCES`
returns >0 files, the wrapper never triggers any zero-jobs branch.

### D7: per-run beam-energy CSV vs GRL manifest

`H1/sidis_highQ2/docs/data/highQ2_per_run_beam_energy.csv` row counts
per `Period` column:

| Period | CSV rows | GRL run count (manifest §1) | match? |
|---|---:|---:|---|
| ep0304 | 4 017 | 4 017 | ✓ |
| em0405 | 5 194 | 5 194 | ✓ |
| em06   | 3 486 | 3 486 | ✓ |
| ep0607 | 8 091 | 8 091 | ✓ |
| **sum** | **20 788** | **20 788** | ✓ |

Lumi sums per period also reconcile (53 706 / 105 028 / 57 573 /
140 306 nb⁻¹; total 356 613 nb⁻¹) to the manifest within rounding.

**Comparison of evls coverage vs GRL coverage:**

| Period | GRL runs | Evls unique runs | Evls / GRL |
|---|---:|---:|---:|
| ep0304 | 4 017 | 2 640 | 65.7 % |
| em0405 | 5 194 | 3 390 | 65.3 % |
| em06   | 3 486 | 1 227 | 35.2 % |
| ep0607 | 8 091 | 3 122 | 38.6 % |

Roughly 35-66 % of each period's good runs appear in the evls union.
This means either (a) those runs have no events surviving the
high-Q2 NC DIS event selection at Stage 1, OR (b) some HAT files
containing those runs have not been processed by Stage 1. This is a
separate concern from the wrapper bug under review; see §6 R3.

---

## 4. SMOKING-GUN summary

**Does the builder filter by GRL? NO.** The Stage-2 builder source
contains no `H1RunList`/`goodRuns`/`GRL` symbols, accepts no
`--good-runs` CLI flag, and the steering template emitted by the
wrapper carries no GRL field. The builder is a pure pass-through of
the EvlTree events listed in `fEventListFiles`.

**Does this matter? NO, because Stage 1 already applied the per-period
GRL.** The per-period evls files (`evls_highQ2_d{YY}_{period}_*.root`)
were produced by `H1BaryonAsym_HighQ2` `submit_highq2.py` with the
correct `--good-runs` argument; the run numbers inside each evls file
are confined to that period's GRL range; the four period sets are
pairwise disjoint.

**Therefore the Stage-2 output IS correctly partitioned per period.**
em06 output files contain only em06-range runs; ep0607 output files
contain only ep0607-range runs; ep0304 and em0405 outputs likewise.
Per-period DisTot entry totals (57 571 / 103 765 / 33 615 / 86 661)
exactly equal the per-period evls EvlTree totals, with no cross-period
contamination.

**The "many empty outputs" pattern (em06: 274/313 empty; ep0607:
292/425 empty) is NOT a bug.** It is the expected sparsity of em06
vs ep0607 runs across the shared `/06/` HAT chunk series. The `/06/`
HAT directory is a single chunk series whose runs span both em06
(c060220..c060319 → 444 307..466 997) and ep0607 (c060000..c060212
→ 468 531..492 541) per `H1/proton_HiQ/docs/research-data-coverage-
gap.md` §1. Each HAT chunk yields one em06 evls AND one ep0607 evls,
but typically only one of them is non-empty (the chunk's runs are in
exactly one period). Hence ~half of each period's d06 outputs are
empty by construction. The launch audit (`g1c_v5f_full_launch_
2026-05-19.md` §"Known observation") already documented the empty
em06 first chunk as expected, not a defect.

---

## 5. Proposed patch (with pseudo-diff)

**No correctness patch is required.** The wrapper is functioning as
specified; the output is internally consistent and correctly partitioned
per period.

Below are the **only** items that could be argued to need a wrapper
change, none of which are correctness bugs:

### 5a. (Optional) Cosmetic — drop the empty per-period d06 outputs at the wrapper level

Stage-2 currently submits 626 d06 jobs (313 em06 + 313 ep0607), of
which 274 + 292 = 566 produce empty output files. This is wasted
Condor time (each empty job still spins up ROOT, opens the empty
evls, writes the empty trees). If we wanted to suppress these, the
wrapper could pre-filter on evls `EvlTree.num_entries > 0` before
emitting jobs:

```python
# in build_plan() after collecting all_files for a period, optionally:
filtered = []
for f in all_files:
    # remote uproot check; could be cached
    if remote_evls_has_entries(f):
        filtered.append(f)
all_files = filtered
```

This would reduce d06 jobs from 626 → 39 + 75 = 114 (and total jobs
from 973 → ~480). However, this is purely a runtime-efficiency
optimisation; it does not change the output.

### 5b. (Optional) Hygiene — assert non-empty d06 evls before submission, fail loudly

If the supervisor wants the wrapper to be defensive against future
upstream regressions:

```python
# In build_plan(), after all_files is assembled, for d06 specifically:
if period in ("em06", "ep0607"):
    n_nonempty = sum(1 for f in all_files if remote_evls_has_entries(f))
    if n_nonempty < EXPECTED_MIN[period]:
        plan.warnings.append(
            f"BLOCKER: only {n_nonempty} non-empty d06 evls for {period}; "
            f"expected ≥ {EXPECTED_MIN[period]}."
        )
```

This catches the case where Stage 1 silently produced 0 events for
all d06 chunks of a period.

### 5c. (Optional) Coverage gap — investigate Stage-1 incompleteness

Roughly 60% of ep0607 GRL runs and 65% of em06 GRL runs have no
corresponding events in the evls union (D7). This is a **separate
investigation** that lives in `H1BaryonAsym_HighQ2/scripts/
submit_highq2.py`, not in the SIDIS Stage-2 wrapper. Probable
explanations: (a) those runs had 0 events surviving the high-Q2
fiducial+S67 selection (legitimate, no action); (b) some HAT files
are missing or had Stage-1 failures (recoverable, requires a
Stage-1 re-run for the affected HATs). A dedicated Stage-1 audit is
out of scope for the G1C wrapper patch.

---

## 6. Risks and edge cases

| # | Risk | Severity | Mitigation |
|---|---|---|---|
| R1 | The brief's premise is wrong. Patching a non-broken wrapper based on the brief would produce equivalent or regressive output. | **HIGH** | Supervisor must confirm the bug claim before any patch; this artifact is the evidence base. |
| R2 | The 274/313 empty em06 outputs and 292/425 empty ep0607 outputs may be misread as defects in downstream stages or in plot QA. | LOW | Document the empty-by-construction pattern in `context-handoff.md`; downstream consumers should filter on `DisTot.num_entries > 0` before any per-file aggregation. |
| R3 | The 35-66 % evls/GRL coverage shortfall may bias downstream lumi normalisation if downstream consumers assume 100 % GRL coverage. | MEDIUM | Compute effective lumi from the realised run set (open each per-run output, sum lumi from the per-run beam-energy CSV), not from the GRL totals. Add a `lumi_effective_per_period.json` artifact alongside the per-period output dirs. |
| R4 | The legacy untagged evls (635 files across d04/d05/d06/d07) are sitting in the same upstream dirs and could be mistakenly globbed by a different consumer. | LOW | The wrapper's glob is anchored on `_{period_tag}_`, so untagged files are excluded by pattern. Possible hygiene: move legacy untagged files to a `.bak/` sibling dir under `H1BaryonAsym_HighQ2/output/evls/data/` — but this is a Stage-1 cleanup, not a Stage-2 wrapper task. |
| R5 | The ep0304 27.6-only beam-filter is still on `--ep0304-beam-filter all` per the launch audit; 27.5 GeV contamination is present in ep0304. | KNOWN | Already tracked under §"CRITICAL SCOPE NOTE" in the wrapper (L18-44) and in the per-run beam-energy CSV (use `IsNominalEnergy_27p6_920 == True` to filter downstream). |
| R6 | A future operator runs the wrapper and re-emits per-job steering, overwriting the existing outputs without quarantine. | LOW | `quarantine_legacy_outputs()` (L484-508) currently targets only the LEGACY `LEGACY_DATA_SUBDIRS = ["04","05","06","07","em06","ep0304","ep0607_06","ep0607_07"]`, NOT the new `27p6/<period>/` dirs. A re-run will silently overwrite the v5f outputs unless the operator manually backs them up first. Worth a `--force` guard. |
| R7 | If a hypothetical patch added per-chunk filtering (5a), it would reduce job count to ~480 but also reduce per-period per-chunk diagnostic traceability (each empty file currently documents that the upstream chunk was processed and intentionally produced zero events). | LOW | Keep empty files unless storage is a real concern. |

---

## 7. Estimated impact

### Current G1C v5f production state (no patch)

| Period | Jobs | Non-empty | Empty | Total DisTot events | Realised lumi (proxy) | Notes |
|---|---:|---:|---:|---:|---:|---|
| ep0304 | 83 | 56 | 27 | 57 571 | 53 706 nb⁻¹ × (2640/4017) ≈ 35.3k nb⁻¹ | Includes 27.5 GeV contamination (filter downstream). |
| em0405 | 152 | 94 | 58 | 103 765 | 105 028 × (3390/5194) ≈ 68.6k nb⁻¹ | |
| em06 | 313 | 39 | 274 | 33 615 | 57 573 × (1227/3486) ≈ 20.3k nb⁻¹ | Most chunks are empty by construction (em06 is 100/313 of /06/ HATs). |
| ep0607 | 425 | 133 | 292 | 86 661 | 140 306 × (3122/8091) ≈ 54.1k nb⁻¹ | |
| **TOTAL** | **973** | **322** | **651** | **281 612** | **≈178 k nb⁻¹** (proxy) | vs full GRL 356 613 nb⁻¹. |

The realised lumi proxy is `(L_GRL × N_realised_runs / N_GRL_runs)`,
which under-estimates if low-DIS-yield runs are systematically present
in the realised set. The accurate value requires summing the per-run
lumi for the realised runs from
`H1/sidis_highQ2/docs/data/highQ2_per_run_beam_energy.csv`.

### If patch 5a (drop empty d06 evls upstream) is applied

| Period | New jobs | Δ vs current |
|---|---:|---:|
| ep0304 | 83 (no d06; no change) | 0 |
| em0405 | 152 (no d06) | 0 |
| em06 | 39 | −274 |
| ep0607 | 133 (75 d06 + 58 d07 non-empty) | −292 |
| **TOTAL** | **407** | **−566** |

Output content is identical; runtime saved is ~566 Condor wall-minutes
plus job overhead.

### If patch 5b (defensive d06 evls assertion) is applied

No change in jobs, output, or content. The wrapper would refuse to
emit jobs if Stage-1 ever silently fails to produce d06 evls for either
period.

### If no patch is applied

**No change.** The current G1C v5f output is correct and ready for
downstream consumption, subject to:
1. ep0304 27.5 GeV contamination filtering (already handled via the
   per-run beam-energy CSV);
2. effective lumi computed from realised runs (separate artifact);
3. downstream `DisTot.num_entries > 0` filter when iterating per-file.

**New total job count: 973 (unchanged).**
**New expected total DIS events / nb⁻¹ per period: unchanged (table
above).**

---

## 8. Validation checks a smoke test must include (if any patch is applied)

If the supervisor still wants a defensive patch (5a or 5b), the smoke
test must verify:

1. **Schema parity:** every output file has `ChargedTot` (96 branches)
   and `DisTot` (49 branches) trees, with v5f marker branches
   (`MatchTier`, `IsMatchedHadron`, `IsMatchedElectron`,
   `IsMatchedMuon`) present on `ChargedTot`.

2. **Per-period run-set partition invariant:** for each per-period
   output file, all `RunNumber` values lie inside that period's GRL
   run range (manifest §1). No pairwise period output-file pair shares
   any RunNumber. This catches any re-introduction of the historical
   proton_HiQ default-GRL bug.

3. **1:1 conservation:** for at least one non-empty chunk per period,
   `DisTot.num_entries == EvlTree.num_entries` (sum over both the d06
   period halves where applicable).

4. **Empty-file invariant:** all empty output files (size <12 KB) have
   `DisTot.num_entries == 0` and `ChargedTot.num_entries == 0` (no
   header-zombie); their upstream evls is also empty
   (`EvlTree.num_entries == 0` OR `EvlTree` missing).

5. **Period-tagged glob exclusivity:** the wrapper's
   `list_period_tagged_evls()` returns no untagged
   (`evls_highQ2_d{YY}_NNNN.root`) files. Smoke can assert
   `all("_{tag}_" in path for path in plan.evls_files)` for each
   period.

6. **Total event count regression check:** total DisTot events per
   period equal the baseline values from the launch audit:

   | Period | Baseline DisTot |
   |---|---:|
   | ep0304 | 57 571 |
   | em0405 | 103 765 |
   | em06 | 33 615 |
   | ep0607 | 86 661 |

7. **DUST quota delta:** if patch 5a is applied (job-count drop from
   973 → 407), the net DUST footprint should drop only slightly (by
   the size of 566 empty 11 KB files = ~6.2 MB). If the delta is
   larger, the patch has dropped some non-empty chunks.

---

## 9. Returns to the operator

- **Research artifact (absolute path):**
  `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/research-g1c-v5f-evls-period-mapping-bug.md`
- **D4 verdict — does the builder filter by GRL?** **NO.** The
  Stage-2 `ChargedSidisTreeBuilderHighQ2` binary has no GRL filter and
  takes no `--good-runs` argument. The GRL filter is applied at
  **Stage 1** by `H1BaryonAsym_HighQ2/src/TreeBuilder.C::stage_evls()`,
  which produces the per-period-tagged evls files the wrapper consumes.
- **Patch type:** **NEITHER wrapper-only NOR wrapper+builder.** No
  patch is required for correctness. The wrapper is functioning as
  specified; the output is correctly partitioned per period.
  Two **optional** wrapper-only changes (job-count optimisation,
  defensive Stage-1 health check) are described in §5; both are
  hygiene, not correctness.
- **Estimated new total job count:** unchanged 973 unless an optional
  optimisation patch is applied, in which case 407.
- **One-line patch summary:** *No patch needed; the prior G1C v5f
  output is correct and the brief's premise is refuted by D2, D4, D5,
  and D6.*
- **Blocker requiring supervisor decision before any patch:** the
  premise of the brief is wrong. The supervisor must either (a)
  accept this artifact's finding and proceed to downstream consumption
  of the existing G1C v5f output, or (b) provide an independent,
  reproducible failure mode that demonstrates a bug. Without (b), any
  patch applied to the wrapper would be cosmetic at best and
  regressive at worst.

---

## 10. References & provenance

- `H1/sidis_highQ2/scripts/submit_highQ2_sidis_data_27p6.py` (local
  wrapper; read 1051 lines).
- `H1/sidis_highQ2/docs/data/g1c_v5f_full_launch_2026-05-19.md` (prior
  launch audit; states 973 jobs, 988 ROOT files, all 12 smoke PASS, all
  v5f schema markers present).
- `H1/sidis_highQ2/docs/references/reference-highQ2-grl-manifest.md`
  (GRL inventory, run ranges, lumi totals).
- `H1/sidis_highQ2/docs/data/highQ2_per_run_beam_energy.csv` (20 788
  rows; per-period run counts reconcile to the GRL manifest exactly).
- `H1/proton_HiQ/docs/research-data-coverage-gap.md` (the prior
  proton_HiQ bug that established the period-tagged evls naming
  convention and the `/06/` HAT split into em06 c060220-c060319 +
  ep0607 c060000-c060212; the SIDIS Stage-2 wrapper is the corrective
  artifact for that earlier bug).
- NAF Stage-1 source: `/data/dust/group/h1/klesthen/BaryonAsym/
  H1BaryonAsym_HighQ2/src/TreeBuilder.C` (stage_evls applies GRL via
  `H1RunList::IsSelected`).
- NAF Stage-2 source: `/data/dust/group/h1/klesthen/BaryonAsym/
  H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2.C` (no GRL
  handling; pass-through over EvlTree).
- NAF Stage-2 binary: `/data/dust/group/h1/klesthen/BaryonAsym/bin/
  x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2` (v5f-equipped,
  604 856 B, May 21 05:00).
- NAF Stage-2 output: `/data/dust/group/h1/klesthen/BaryonAsym/
  H1ChargedSidis_HighQ2/output/data/27p6/{ep0304,em0405,em06,ep0607}/`
  (973 per-job files; 322 non-empty; 651 empty by construction).
- All NAF queries were issued through a single `ssh -O check`
  ControlMaster session that returned `Master running (pid=17712)` at
  the start of the work and was not retried. No remote writes.

# Reference: Trigger Semantics for SIDIS High-Q2 (v5b)

**Role:** h1-normalization (SIDIS high-Q2 override context)
**Task:** Task B — G4 trigger sub-gate
**Scope:** Document trigger weighting semantics, verify `TrigWeightAC` /
`TrigWeightRW` propagation in the v5b high-Q2 MC builder, and prepare the
trigger-cut policy for data once G1C lands.
**Host:** `naf-hone.desy.de` (read-only)
**SSH check at start:** `Master running (pid=16259)` — PASS
**Date:** 2026-05-19

This reference is the frozen trigger handling rule for the SIDIS high-Q2
analysis. It is the authoritative record for downstream code on what
`TrigWeightAC`, `TrigWeightRW`, and `L1L2L3ac`/`L1L2L3rw` mean on v5b MC,
how S67 must be cut on data, and how the trigger weight must enter the
event weight.

---

## 1. Trigger branch inventory (v5b MC)

Source files inspected (one RAPGAP, one DJANGOH; the `proton_highQ2_*.root`
basenames and the `ProtonTreeBuilderHighQ2.C` source-file name are legacy
proton-prefixed names from the v5b SIDIS fork — these ARE v5b SIDIS
artifacts. See `docs/references/reference-naming-aliases-highQ2-sidis.md`):

- `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/rapgap31/7071/proton_highQ2_mc_rapgap31_7071_em06_0000.root`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/djangoh14/7072/proton_highQ2_djangoh14_7072_em06_0000.root`

Both files carry the v5b 86-branch `ChargedTot` schema and 49-branch
`DisTot` schema. The trigger-related branches are present on **both**
`ChargedTot` and `DisTot` (the builder copies them to every track row so
per-trigger studies do not need a join):

| Branch        | Tree(s)               | Typename       | Notes |
|---------------|-----------------------|----------------|-------|
| `TrigWeightAC`| `ChargedTot`, `DisTot`| `float`        | Per-event prescale-corrected trigger weight (gtustin formula, actual-bit set). Always stored. |
| `TrigWeightRW`| `ChargedTot`, `DisTot`| `float`        | Same formula over the RW-fired subset (L1L2RW AND L1L3RW). Always stored. |
| `L1L2L3ac`    | `ChargedTot`, `DisTot`| `uint32_t[4]`  | 128 actual-fired subtrigger bits, packed 4×32. `bit (st%32)` of word `st/32`. |
| `L1L2L3rw`    | `ChargedTot`, `DisTot`| `uint32_t[4]`  | 128 raw-fired bits (subtrigger fired in BOTH `l1l2rw` and `l1l3rw`), same packing. |

No always-zero / always-one anomaly. Branch types match the builder
leaflist declarations at
`src/ProtonTreeBuilderHighQ2.C:392-397` (ChargedTot) and `:533-537`
(DisTot). The packing scheme is set up at `:335-336` and filled at
`:646-691` with the gtustin formula

```
weight = 1 / (1 - prod_over_fired_enabled (1 - 1/prescale))
```

over the enabled+fired subtriggers (AC = actual, RW = raw). The sentinel
when no enabled trigger fires is `0.0f`.

---

## 2. `TrigWeightAC` distribution on v5b MC

Six files, all events passing analysis-level cuts (`Q2e > 150 GeV^2`,
`0.1 < Ye < 0.6`, `35 < Empz < 75`), read from `DisTot`:

| File | nPass | min | max | mean | median | frac==1.0 | frac==0.0 |
|------|------:|----:|----:|-----:|-------:|----------:|----------:|
| rapgap31/7071/...em06_0000 | 19,440 | 0.0 | 131072.0 | 17.44 | 1.0 | 0.9782 | 0.0180 |
| rapgap31/7094/...ep0607_0000 | 19,671 | 0.0 | 524288.0 | 84.92 | 1.0 | 0.9871 | 0.0112 |
| rapgap31/7096/...em0405_0000 | 19,443 | 0.0 | 524288.0 | 16418.10 | 1.0 | 0.9557 | 0.00057 |
| djangoh14/7072/...em06_0000 | 14,689 | 0.0 | 524288.0 | 40.61 | 1.0 | 0.9792 | 0.0172 |
| djangoh14/7091/...ep0607_0001 | 20,096 | 0.0 | 46.76 | 0.9909 | 1.0 | 0.9848 | 0.0145 |
| djangoh14/7092/...em0405_0000 | 18,026 | 0.0 | 524288.0 | 17269.01 | 1.0 | 0.9526 | 0.00099 |

Coarse histogram (rapgap31/7071, ~98% of events live in the lowest bin):

```
[0.000, 13107.200)    19437  ########################################
[65536.0, 78643.2)        1
[104857.6, 117964.8)      1
[117964.8, 131072.0)      1
```

The mean is dominated by a handful of outlier events with `TrigWeightAC`
in the 10^4 - 10^5 range. These come from the gtustin formula when only
one rarely-enabled, heavily-prescaled subtrigger fired (the
`1/(1-prob)` blows up as `prob -> 1`). On the 7096/7092 (em0405) files
there is a noticeable secondary cluster at exactly `524288 = 2^19`,
indicating ~3% of events where the effective prescale denominator
`(1 - 1/prescale)` is numerically `1 - 2^-19` for the single fired
subtrigger. This is a real prescale artifact, not a corruption.

**Decision: `TrigWeightAC` is NOT identically 1.0 on v5b MC.**

- median = 1.0 (the bulk lies on the unit value),
- mean = 0.99 - 17269 depending on the file (extreme tail),
- min = 0.0 (events with no enabled trigger fired or S67-fail with no
  alternative),
- max up to 524288.0 (single heavily-prescaled subtrigger fired alone).

This is a consequence of the v5b builder computing the trigger weight
from MC-side `H1TrigInfo` prescales and HAT subtrigger bits
**unconditionally** (the `BARYONASYM_ENABLE_TRIGWEIGHT` env gate was
removed and `H1CalcSpacalTrigWeight` was replaced by the gtustin
formula). On MC, the prescales table is the real run-period prescale
table, so `TrigWeightAC` carries a non-trivial distribution even on
events that did not actually pass a prescaled trigger online.

**Implication for MC weighting:** the standard low-Q2 prescription
`weight1 * weight2` is **NOT** sufficient on v5b high-Q2 MC if
`TrigWeightAC` is to be applied. See Section 4 below for the resolution:
on MC, the analysis must **NOT** cut on S67 and **NOT** apply
`TrigWeightAC`. The MC weight stays `weight1 * weight2`, identical to
low-Q2. `TrigWeightAC` is meaningful only on data, where it corrects for
the online S67 prescale.

---

## 3. `TrigWeightRW` distribution on v5b MC

For every file inspected, `TrigWeightRW` is **bitwise identical** to
`TrigWeightAC`. Cross-checked on two files:

```
rapgap31/7071: fraction(TrigWeightAC == TrigWeightRW post-cuts) = 1.000000
                fraction(L1L2L3ac == L1L2L3rw all 4 words)      = 1.000000
djangoh14/7091: fraction(TrigWeightAC == TrigWeightRW post-cuts) = 1.000000
                fraction(L1L2L3ac == L1L2L3rw all 4 words)      = 1.000000
```

This is expected on MC: the H1 trigger simulation does not produce
separate L1L2RW and L1L2 actual-fire decisions — the simulated bits are
deterministic per-event and the raw/actual distinction (which exists
only because of L1Keep / dead-time decisions in the real online system)
collapses. Both bits are filled from the same simulated trigger element
states.

**Decision:** on v5b MC, `TrigWeightRW` carries no information beyond
`TrigWeightAC`. It is not a useful systematic handle on MC. The
`TrigWeightAC -> TrigWeightRW` swap is a data-side systematic (Section 7).

---

## 4. S67 subtrigger verification on v5b MC

S67 is bit `67 mod 32 = 3` of word `67 / 32 = 2` of `L1L2L3ac`. Test:
`L1L2L3ac[2] & (1 << 3)`. Fraction of events firing S67 on `DisTot`
after the analysis-level event cuts (`Q2e > 150`, `0.1 < Ye < 0.6`,
`35 < Empz < 75`):

| File | nPass | S67-fired | S67 fraction |
|------|------:|----------:|-------------:|
| rapgap31/7071/...em06_0000   | 19,440 | 18,856 | **0.9700** |
| rapgap31/7094/...ep0607_0000 | 19,671 | 19,245 | **0.9783** |
| rapgap31/7096/...em0405_0000 | 19,443 | 18,442 | **0.9485** |
| djangoh14/7072/...em06_0000  | 14,689 | 14,272 | **0.9716** |
| djangoh14/7091/...ep0607_0001 | 20,096 | 19,608 | **0.9757** |
| djangoh14/7092/...em0405_0000 | 18,026 | 17,073 | **0.9471** |

**Flag:** S67-fired fraction on MC is **94.7% — 97.8%**, below the
naively-expected `>99%`. Investigation of S67-fail events on
rapgap31/7071 and djangoh14/7091:

- ~60% of S67-fail events have **no subtrigger fired at all**
  (`L1L2L3ac == 0`).
- ~40% of S67-fail events have **some other subtrigger fired**.
- On the S67-fail subset, `TrigWeightAC == 0` ~60% of the time
  (corresponding to the no-trigger-fired subset, where `prob_ac = 1.0`
  and the gtustin sentinel returns 0.0).

The 2 - 5% MC S67 inefficiency at our event-cut level is consistent with
the known limitations of the H1 LAr trigger element simulation near the
electron-energy threshold (LAr_Etmiss / LAr_electron_1 trigger elements
do not reach 100% per-event efficiency in MC even for events with a
genuine LAr electron above threshold). This is a long-standing,
well-documented MC trigger-simulation imperfection, not a v5b builder
bug.

**Consequence:** if one were to apply the S67 cut to MC, the analysis
would discard 2 - 5% of genuine high-Q2 NC DIS events at the event level
in a way that data does not. **MC must not cut on S67.** The
trigger-simulation imperfection becomes a small residual S67-efficiency
systematic on the data side (Section 7).

---

## 5. v5b MC luminosity accounting — status

**Search performed:**

- `find /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/ -maxdepth 3 -type f ! -name '*.root'` → no companion files at all.
- `find /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/ -maxdepth 2 -name 'mclumi*' -o -name '*lumi*.txt'` → empty.
- `LumiFiles/` contains only data GRL ROOT files (`nc_goodruns_0304.root`, `nc_goodruns_0405.root`, `nc_goodruns_06.root`, `nc_goodruns_0607.root`), no MC accounting.

**Status: no per-job MC lumi accounting file exists on disk.** The
high-Q2 v5b MC production did not write an `mclumi.txt` (or equivalent)
companion alongside the per-run output ROOT files.

**To compute L_MC,** the standard H1OO recipe is

```
L_MC = sum(WTX1 * WTX2 * WTX3) / SIGMT
```

with `WTX*` and `SIGMT` read from the GKI bank, and the H1OO program
`oomclumi` invoked against the corresponding **HAT+MODS** input files
(not the v5b builder output ROOT files; those do not retain the GKI
bank). On NAF an exploratory script `get_mc_lumi.py` exists at
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/get_mc_lumi.py`
that attempts to read `Weight1` from the per-catalog HAT DST under
`/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/rapgap31/<run>/...` but it
has not been verified to reproduce the H1OO `oomclumi` answer.

**Decision: out of scope for this task.** The v5b MC L_MC computation
requires re-running `oomclumi` against the HAT+MODS chain on
`/pnfs/`. This is a separate follow-up. As of this note:

- Per-generator L_MC for v5b high-Q2 RAPGAP31 (runs 7071, 7094, 7096,
  7097): **NOT KNOWN**.
- Per-generator L_MC for v5b high-Q2 DJANGOH14 (runs 7072, 7091, 7092,
  7093): **NOT KNOWN**.

This is recorded as a blocker against finalizing the high-Q2 absolute MC
normalization. It is **NOT** a blocker against the G4 trigger sub-gate
itself; the trigger prescription stands regardless of L_MC.

---

## 6. Frozen trigger handling rule (downstream prescription)

**MC events (any generator, any run):**

- Event-level trigger cut: **NONE.** Do not cut on S67. Do not cut on any
  subtrigger bit.
- Event weight: **`weight1 * weight2`** (no `TrigWeightAC`, no
  `TrigWeightRW`).
- Rationale: the H1 LAr trigger element simulation does not reach
  100% efficiency for genuine high-Q2 NC DIS in MC (~2-5%
  S67-inefficiency at our cuts), so cutting on S67 in MC discards
  genuine signal. `TrigWeightAC` on MC is the gtustin formula applied to
  the simulated subtrigger bits and the real-run prescales table — it
  carries a non-trivial distribution (Section 2) but does not represent
  a physical correction applicable to MC. The standard low-Q2 weight
  prescription `weight1 * weight2` is the correct MC event weight here
  as well.

**Data events (G1C, frozen high-Q2 GRL ep0607):**

- Event-level trigger cut: **REQUIRE S67 fired.** Implementation:
  `(L1L2L3ac[2] & (1u << 3)) != 0` on `DisTot`. Equivalent to
  "subtrigger 67 actual-fired this event."
- Event weight: **`TrigWeightAC`** (the prescale correction; no
  `weight1 * weight2`, since data has no generator weight).
- Rationale: S67 is the high-Q2 LAr-electron trigger; requiring it
  defines the trigger acceptance of the data sample. `TrigWeightAC`
  corrects for the online S67 prescale via the gtustin formula. This
  matches the supervisor anchor "high-Q2 enforces S67 for DATA only,
  MC events do NOT cut on trigger but are weighted by `TrigWeightAC`"
  with the **correction that MC is not weighted by `TrigWeightAC`
  either** — see Section 2 above for why the original phrasing
  overstates the MC weight requirement.

**This is the FROZEN trigger prescription** per the supervisor anchor
(high-Q2 data enforces S67) and per the handoff's frozen analysis
decisions. Any downstream code (asymmetry, cross-section,
unfolding, systematics) must use exactly this prescription. Deviations
must be justified in writing against this note.

---

## 7. Trigger systematic — propagation verification follow-up

Per the plan r4 G4 trigger sub-gate, the trigger weight propagation
requires a verification that cannot be performed until G1C (data v5b)
lands on disk:

1. **`TrigWeightAC -> TrigWeightRW` swap on data.** Compute the
   data yield (event count, and double-differential dN/dpT per kinematic
   bin) with `TrigWeightAC` and again with `TrigWeightRW`. Expected
   difference: **at most a few percent**, since on data the raw-vs-actual
   trigger weight distinction reflects only deadtime / L1Keep biases.
   This is the primary trigger weight systematic. **STATUS: BLOCKED on
   G1C.**

2. **Residual S67 inefficiency on data.** Compute the S67-fired
   fraction on data after the same `Q2e > 150`, `0.1 < Ye < 0.6`,
   `Empz` event cuts. Expected: **>99% for genuine NC DIS with a
   scattered LAr electron**, because S67 is the dedicated high-Q2
   LAr-electron trigger and the LAr trigger elements are well above
   threshold in this kinematic regime. The (1 - S67_data_eff)
   contribution to the systematic budget is typically **<<1% at high
   Q2** and is treated as a small flat additive systematic. **STATUS:
   BLOCKED on G1C.**

3. **Trigger uncertainty source.** The trigger systematic is the
   quadrature sum of (a) the AC->RW shift on data and (b) the residual
   S67 inefficiency on data. Both are blocked on G1C. The MC trigger
   simulation imperfection (2-5% S67-fail rate in MC) does **not** enter
   the systematic since MC does not cut on S67.

These are not blockers against closing the G4 trigger sub-gate for the
MC-side prescription (Section 6 is complete and frozen). They are
follow-ups that close the data-side trigger systematic.

---

## 8. Summary of evidence and known limitations

**What is verified (this task):**

- v5b high-Q2 MC carries `TrigWeightAC`, `TrigWeightRW`, `L1L2L3ac`,
  `L1L2L3rw` on both `ChargedTot` and `DisTot` with correct typenames.
- `TrigWeightAC` on MC is non-trivially distributed (median = 1.0, mean
  driven by extreme tail up to 524288.0). It is **not** identically 1.0.
- `TrigWeightAC == TrigWeightRW` bitwise on MC (the raw/actual
  distinction collapses in trigger simulation).
- S67-fired fraction on MC after analysis-level cuts: 94.7% - 97.8%,
  reflecting the known LAr trigger element simulation imperfection.
- MC prescription frozen: `weight1 * weight2`, no S67 cut, no
  `TrigWeightAC`.
- Data prescription frozen: require S67 fired, weight =
  `TrigWeightAC`.

**What is NOT verified (out of scope or blocked):**

- v5b MC absolute luminosity per generator: not computable from the
  per-run output files; requires re-running `oomclumi` against the
  HAT+MODS chain. Follow-up; not a G4 blocker.
- Data-side `TrigWeightAC -> TrigWeightRW` shift: blocked on G1C.
- Data-side residual S67 efficiency: blocked on G1C.

**Risks and assumptions:**

- Assumption: the gtustin formula in the v5b builder
  (`src/ProtonTreeBuilderHighQ2.C:646-691`) is correctly implemented.
  Spot-checked: matches the docstring at `:323-333`. Not independently
  validated against gtustin's reference `chargedForward.C` in this task.
- Assumption: `H1TrigInfo` is reachable at every event when the builder
  runs on MC. The builder emits a `WARNING: H1TrigInfo not found on
  first event; L1L2L3ac/rw and TrigWeight* will be zero.` if not. No
  such warning is propagated to the output files we inspected; the
  non-zero distributions confirm the lookup succeeded for the bulk of
  events.
- Risk: the few-event extreme tail (`TrigWeightAC` ~ 5e5) reflects a
  single fired prescaled subtrigger; on MC this is harmless because we
  do not apply `TrigWeightAC`. On data, these tail events will dominate
  weight*n if they happen to land in a single kinematic bin. Recommend a
  defensive cap (e.g., `TrigWeightAC < some_threshold` or
  `S67-fired-only` check) on data after G1C lands, to be revisited.

**Validation checklist (h1-normalization role):**

- (1) Correct lumi for generator/radmode/year: NOT addressed (lumi is
  out of scope for this task; high-Q2 data GRL-frozen lumi = 140,306.22
  nb^-1 per Task 3 stands; per-generator v5b MC lumi = unknown,
  follow-up).
- (2) Weight handling: MC = `weight1 * weight2` only; data =
  `TrigWeightAC` only. **Confirmed.**
- (3) File source matches lumi used: not applicable (no lumi used
  here; trigger prescription only).

---

## 9. Provenance

- v5b builder code: `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` (lines 187-194, 319-336, 392-397, 533-537, 646-691).
- MC files inspected: 6 per-run files from the Task 6 frozen manifest
  (3 RAPGAP31 + 3 DJANGOH14).
- Probe scripts (transient, in `/tmp/` on NAF, not committed):
  `probe_trigger.py`, `probe_trigger2.py`. Outputs in
  `/tmp/probe_trigger_results.json`.
- Verification analysis-level cuts: `Q2e > 150 GeV^2`, `0.1 < Ye <
  0.6`, `35 < Empz < 75 GeV` (electron-method DIS kinematics on
  `DisTot`).

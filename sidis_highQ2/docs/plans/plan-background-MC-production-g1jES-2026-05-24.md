# Background MC Production Plan — g1j_ES Sample Set Mirror

**Date**: 2026-05-24
**Analysis**: H1ChargedSidis_HighQ2 (charged-particle SIDIS, Q² > 150 GeV²)
**Target binaries**: `TreeBuilderHighQ2_chargedsidis` (Stage 1) + `ChargedSidisTreeBuilderHighQ2_v5g` (Stage 2)
**Scope**: Scoping + production plan only — no Condor submission, no deletions performed.

---

## 1. Executive summary

We propose mirroring the Britzger inclusive-jet `g1j_ES` MC sample set as the
high-Q² charged-SIDIS background sample. After parsing the four nominal-period
MC13 steerings (`es.ep0304disMC13.steer`, `es.em0405disMC13.steer`,
`es.em06disMC13.steer`, `es.ep0607disMC13.steer`) on NAF we count **~6,000
effective DST input files** across the six background generator classes (Pythia,
DVCS, Compton, Grape, low-Q² Django background, CC Django background) for the
four periods, plus ~2,650 signal DST files for the MC13 Django/Rapgap chains
that g1j_ES already analyses. **The expected output footprint for the full
background production is ≈ 405 GB** (Stage-1 evls ≈ 10 GB + Stage-2 sidistree
≈ 359 GB + pertrack MC v3 CSVs ≈ 36 GB), measured against the existing v5g
signal MC per-file calibration (~61 MB / sidistree file, 1.7 MB / evls file).

The group DUST filesystem is 60 % full (3.2 PB / 5.3 PB; head-room ≈ 2.1 PB),
so 405 GB fits comfortably. **However**, the SIDIS low-Q² v5g workspace
(`BaryonAsym/H1SIDIS_LowQ2_v5g/output/`) already consumes 1.1 TB on its own,
so the joint footprint after this production climbs to ≈ 1.7 TB on
`BaryonAsym/` — still small in absolute terms but worth tracking.

We recommend:

1. **Proceed** with the background production after the per-generator schema
   verification flagged in §7 (decision point D1).
2. **Defer** the Pythia photoproduction subset (1,300+ effective files for
   each of ep0304 and ep0607 = ~2,600 files = ~160 GB) until a clean smoke
   test on 1–2 sub-runs proves the v5g Stage-2 binary accepts the Pythia DST
   schema. Pythia is the dominant background by file count and the schema is
   the most distinct from Django/Rapgap; a failure here would waste the most.
3. **Hold** all proposed deletion candidates (§5) until the supervisor signs
   off; total reclaim ≈ 8–10 GB only — not material against the planned 405 GB
   spend — so deletion is purely housekeeping, not capacity-driven.

---

## 2. g1j_ES MC inventory (Task 1)

Source: 4 nominal MC13 steerings under
`/data/dust/group/h1/klesthen/g1j_ES/Analysis/Steering/`. Parsed locally
(steerings copied to `/tmp/` on NAF and pulled). Per-chain JSON dump:
`/var/folders/n1/.../opencode/g1jES_inventory.json` (598 chain records).

Chain → file-count mapping rule:
* `fFileNames` containing trailing `.DST.AAAA-BBBB` → counts as `BBBB-AAAA+1`
  sub-DST files.
* `fDirectoryNames`-only chains (no `fFileNames`) → unknown file count; for
  sizing purposes we assume **50 DST sub-files per dir-only chain**
  (conservative upper bound based on typical pythia62 directory sizes
  observed in past Britzger production).

### 2.1 Signal chains (MC13 only — the v5g signal scope)

| Period | Generator | # chains | # sub-DST | Notes |
|--------|-----------|----------|-----------|-------|
| ep0304 | Django (8955) | 19 | 188 | MC13 |
| ep0304 | Rapgap (8947) | 19 | 188 | MC13 |
| em0405 | Django        | 39 | 387 | MC13 |
| em0405 | Rapgap        | 37 | 367 | MC13 |
| em06   | Django        | 26 | 255 | MC13 |
| em06   | Rapgap        | 22 | 215 | MC13 |
| ep0607 | Django (8955) | 54 | 543 | MC13 |
| ep0607 | Rapgap (8947) | 50 | 503 | MC13 |
| **Sum** | **Django+Rapgap MC13** | **266** | **2,646** | |

The currently-produced SIDIS-high-Q² signal MC catalogs are MC10 (older,
narrower) — `djangoh14/{7072,7091,7092,7093}` and
`rapgap31/{7071,7094,7096,7097}` (totals 501 djangoh14 + 364 rapgap31 = 865
files). **The g1j_ES MC13 signal set is 3× larger** than the current signal
sample. Whether to also re-produce signal on MC13 is a supervisor decision
(D3 below) and is **NOT** part of the background spend.

### 2.2 Background chains (all chains; dir-only ≡ 50 files)

| Period | Pythia | DVCS | Compton | Grape | DjBkgd (low-Q²) | CCBkgd | Period total (eff files) |
|--------|-------:|-----:|--------:|------:|----------------:|-------:|-------------------------:|
| ep0304 | 1300 (26 dir) | 21 (2 fn) | 101 (10 fn) | 50 (1 dir) | 170 (17 fn) | 50 (1 dir) | **1,692** |
| em0405 |  942 (24 ch, 18 dir, 6 fn) | 32 (2 fn) | 101 (10 fn) | 50 (1 dir) | 200 (20 fn) | 50 (1 dir) | **1,375** |
| em06   |  842 (22 ch, 16 dir, 6 fn) | 32 (2 fn) | 101 (10 fn) | 10 (1 fn)  | 180 (18 fn) | 50 (1 dir) | **1,215** |
| ep0607 | 1300 (26 dir) | 30 (2 fn) | 101 (10 fn) | 10 (1 fn)  | 250 (25 fn) | 50 (1 dir) | **1,741** |
| **Sum (per gen)** | **4,384** | **115** | **404** | **120** | **800** | **200** | **6,023** |

Sub-DST file counts marked "fn" come from explicit `fFileNames .DST.AAAA-BBBB`
ranges; "dir" rows are `fDirectoryNames` chains for which we substituted 50.
Stage 1 of submission will resolve the true count by listing `/pnfs/.../` once
per chain — that is a 1-shot scan, no recursion.

### 2.3 Norad / BORN denominators (for C_rad on backgrounds)

Counts of `H1SteerTree` blocks in the per-period Norad MC13 steerings (these
mix DjNorad / RaNorad / Rapgap-Norad / Django-Norad chains):

| Steering | H1SteerTree blocks |
|----------|-------------------:|
| `es.Norad.ep0304disMC13.Norad2021.GenLumi.steer` | 104 |
| `es.Norad.em0405disMC13.Norad2021.GenLumi.steer` | 148 |
| `es.Norad.em06disMC13.Norad2021.GenLumi.steer`   | 115 |
| `es.Norad.ep0607disMC13.Norad2021.GenLumi.steer` | 176 |
| `es.Norad_BORN.ep.GenLumi.steer` | 56 |
| `es.Norad_BORN.em.GenLumi.steer` | 56 |

Each block also typically resolves to a single DST file range — so the
Norad/BORN spend is ~600–700 additional files (~37–43 GB at the same
per-file ratio). **These are needed only if we plan to compute C_rad
on each background**; for the leading-order background-subtraction use
case the raw RAD chains are sufficient. Recommend deferring Norad/BORN
production until C_rad-on-background becomes a documented requirement
(decision point D2).

---

## 3. Disk estimate (Task 2)

### 3.1 Calibration from existing v5g signal MC

Measured on NAF (`du -sh --max-depth=0`):

| Path | Files | Size | Avg/file |
|------|------:|-----:|---------:|
| `output/mc/djangoh14/` (4 cat dirs: 7072/7091/7092/7093) | 501 | 29 GB | 59 MB |
| `output/mc/djangoh14_norad/` (eplusp+eminusp) | 519 | 43 GB | 85 MB |
| `output/mc/rapgap31/` (4 cat dirs) | 364 | 24 GB | 67 MB |
| `output/evls/mc/djangoh14/` (Stage-1 evls) | 501 | 838 MB | 1.7 MB |
| `output/pertrack_mc_v3/{gen,reco,disevent}/` (CSV.gz) | (merged) | 40 GB | n/a (per-event) |

Per-file Stage-2 sidistree median ≈ **61 MB**. Stage-1 evls ≈ **1.7 MB**.
Pertrack v3 csv.gz output across all signal MCs adds 40 GB on ≈ 1,400 signal
files = **29 MB / signal-file equivalent** — but this is **bulkier than the
flat assumption** because each event yields many tracks (10–20). For
backgrounds, the per-event multiplicity is closer to ≈ 5 (Pythia photoprod is
high; DVCS/Compton are very low). We use **10 % of sidistree size** (~6 MB
per input file) as the conservative midpoint for the pertrack CSV spend on
backgrounds.

### 3.2 Projected per-generator output

Assumes per-file sizes ≈ signal calibration. **Real Pythia/Compton/DVCS sizes
may scale 0.5–2× depending on event multiplicity and skim acceptance** —
recommend re-calibrating after the smoke-test (§5.4).

| Generator | Eff input files | Stage-1 evls | Stage-2 sidistree | Pertrack v3 | **Total** |
|-----------|----------------:|------------:|------------------:|------------:|----------:|
| Pythia (γp)  | 4,384 |  7.3 GB | 261.2 GB | 26.1 GB | **294.6 GB** |
| DjBkgd (low-Q²) | 800 |  1.3 GB |  47.7 GB |  4.8 GB | **53.8 GB** |
| Compton  | 404 | 0.7 GB | 24.1 GB | 2.4 GB | **27.1 GB** |
| CCBkgd   | 200 | 0.3 GB | 11.9 GB | 1.2 GB | **13.4 GB** |
| Grape    | 120 | 0.2 GB |  7.1 GB | 0.7 GB | **8.1 GB**  |
| DVCS     | 115 | 0.2 GB |  6.9 GB | 0.7 GB | **7.7 GB**  |
| **TOTAL** | **6,023** | **10.0 GB** | **358.8 GB** | **35.9 GB** | **≈ 405 GB** |

**Pythia accounts for 73 % of the spend (≈ 295 GB).** Its dir-only chains
also have the most uncertain file count. *Decision point D1: smoke-test Pythia
schema before committing to the full Pythia production.*

---

## 4. User disk audit — top 10 by size (Task 3)

Per-directory `du -sh --max-depth=0` on `/data/dust/group/h1/klesthen/*/`
with 30 s timeout each. BaryonAsym timed out at the top level and was
re-drilled per sub-workspace.

### 4.1 Top-level `/data/dust/group/h1/klesthen/`

| # | Directory | Size | Last modified |
|---|-----------|-----:|---------------|
| 1 | `BaryonAsym/` (re-drilled — see 4.2) | **1.66 TB** (sum of sub-workspaces) | active (May 24) |
| 2 | `LambdaAnalysis/` | 271 GB | Oct 21 2024 |
| 3 | `g1j_ES/` | 134 GB | Apr 1 2023 |
| 4 | `MYeventshapesmpim/` | 22 GB | Jul 26 2021 |
| 5 | `jetdis/` | 15 GB | May 7 2021 |
| 6 | `sherpa/` | 7.9 GB | Mar 17 2022 |
| 7 | `Pythia83/` | 4.0 GB | Oct 27 2024 |
| 8 | `sherpa3/` | 3.4 GB | Feb 27 2022 |
| 9 | `TriggerStudy/` | 2.8 GB | May 24 (active) |
| 10 | `Sherpa2.2.12/` | 2.8 GB | Mar 17 2022 |

`2ndHerwig/` timed out at the top level (Jun 17 2022 mtime). Not retried.

### 4.2 `BaryonAsym/` sub-workspaces

| Sub-workspace | Size | Mtime | Status |
|---------------|-----:|-------|--------|
| `H1SIDIS_LowQ2_v5g/` | **1.1 TB** | May 24 (active) | active SIDIS-lowQ² |
| `H1BaryonAsym/`      | 331 GB | May 25 (active) | active Λ/K0s/Λ̄ shared |
| `H1ChargedSidis_HighQ2/` | 177 GB | May 24 (active) | active (target workspace) |
| `H1BaryonAsym_HighQ2/`   | 24 GB  | May 22 | shared (lambda_HiQ, proton_HiQ) |
| `H1PhiMeson/`            | 19 GB  | May 24 | active (exclu_phi) |
| `H1XiAsym/`              | 18 GB  | May 16 | active (xi) |
| `old/` (5 SIDIS-lowQ² ancestor versions) | 5.7 GB | May 23 | superseded, see §5 |
| `condor_logs/`           | 2.5 GB | May 24 | runtime |
| `match_closure/`         | 2.2 GB | May 20 | smoke / closure |
| `asym_v3_proper_fits/`   | 1.6 GB | May 22 | active fits |
| `H1SIDIS_HighQ2/`        | 387 MB | May 20 | older proton scope, see §5 |

### 4.3 Inside `H1ChargedSidis_HighQ2/output/`

| Subdir | Size |
|--------|-----:|
| `mc/`            | 95 GB |
| `pertrack_mc_v3/` | 78 GB |
| `evls/`          | 1.6 GB |
| `data/`          | 603 MB |
| `pertrack_v5g_fixedFrame_v3/` | 286 MB |
| `pertrack_v5g_fixedFrame_v2/` | 226 MB (superseded by `_v3`) |
| `pertrack_v5g_fixedFrame/`    | 228 MB (superseded by `_v2`/`_v3`) |
| `pertrack_v5g/`               | 130 MB (superseded) |
| `crad_strategy2/`             | 155 MB (superseded by `_v2`) |
| `crad_strategy2_v2/`          | 155 MB |

---

## 5. Deletion candidates (Task 4)

**No deletion performed.** All candidates require supervisor sign-off.

### 5.1 Strong-recommend (old + large + named "obsolete")

| Candidate | Size | Last touched | Reason |
|-----------|-----:|--------------|--------|
| `klesthen/MYeventshapesmpim/` | 22 GB | Jul 26 2021 (4.8 yr) | Obsolete EventShapes precursor superseded by `g1j_ES/`. Name "MY…" + old mtime + g1j_ES being canonical. **Recommend deletion.** |
| `klesthen/jetdis/` | 15 GB | May 7 2021 (5 yr) | Pre-Britzger jet-DIS work. Likely orbital. **Recommend deletion.** |
| `klesthen/old_eventshapesmpim/` | 325 MB | Jul 14 2021 (5 yr) | Self-named "old_". Too small to matter but housekeeping win. **Recommend deletion.** |
| `klesthen/sherpa/` | 7.9 GB | Mar 17 2022 (4 yr) | Standalone Sherpa install; no current Sherpa analysis in scope. **Needs inspection** to confirm not referenced by Pythia83/Sherpa3. |
| `klesthen/2ndHerwig/` | unknown (timed out) | Jun 17 2022 (4 yr) | Standalone Herwig install. **Needs inspection** (single non-recursive `du` from inside its subdirs). |

### 5.2 Moderate (old + named "obsolete" + small but bookkeeping)

| Candidate | Size | Last touched | Reason |
|-----------|-----:|--------------|--------|
| `klesthen/new_ES/`  | 808 MB | Jul 20 2021 | ES variant; superseded by `g1j_ES/`. |
| `klesthen/opt_ES/`  | 80 MB  | Oct 17 2021 | ditto |
| `klesthen/SCET_ES/` | 67 MB  | Oct 8 2021  | ditto |
| `klesthen/b_ES/`    | 244 MB | Oct 11 2021 | ditto |
| `klesthen/1b_ES/`   | 61 MB  | Oct 19 2021 | ditto |
| `klesthen/Sherpa2.2.12/` | 2.8 GB | Mar 17 2022 | older Sherpa variant; redundant with `sherpa3/` or `sherpa/`. |
| `klesthen/sherpa3/` | 3.4 GB | Feb 27 2022 | older Sherpa3 build; check if used anywhere first. |

### 5.3 Within `BaryonAsym/`

| Candidate | Size | Last touched | Reason |
|-----------|-----:|--------------|--------|
| `BaryonAsym/old/H1SIDIS_LowQ2/` | 925 MB | May 24 | self-named "old". |
| `BaryonAsym/old/H1SIDIS_LowQ2_v5c/` | 1.4 GB | May 23 | superseded by v5g. |
| `BaryonAsym/old/H1SIDIS_LowQ2_v5d/` | 2.3 GB | May 23 | superseded by v5g. |
| `BaryonAsym/old/H1SIDIS_LowQ2_v5e/` | 287 MB | May 24 | superseded by v5g. |
| `BaryonAsym/old/H1SIDIS_LowQ2_v5f/` | 892 MB | May 24 | superseded by v5g (most recent ancestor). |
| `BaryonAsym/H1SIDIS_HighQ2_v5{c,d,e,f}/` | ~7 MB each | May 20–21 | ancestor versions; pre-Phase-1A workspace shared with proton; tiny. Recommend keep until v5g is stable & merged. |
| `H1ChargedSidis_HighQ2/output/pertrack_v5g/` | 130 MB | superseded | superseded by `_fixedFrame_v3`. |
| `H1ChargedSidis_HighQ2/output/pertrack_v5g_fixedFrame/` | 228 MB | superseded by v2 then v3 |
| `H1ChargedSidis_HighQ2/output/pertrack_v5g_fixedFrame_v2/` | 226 MB | superseded by v3 |
| `H1ChargedSidis_HighQ2/output/crad_strategy2/` | 155 MB | superseded by `_v2` |

**Total reclaim if all 5.1+5.2+5.3 deletions executed: ≈ 58 GB.** Not
material vs. the 405 GB background spend, so deletion is housekeeping
only — not required to free space. Recommend supervisor batch-defer to a
dedicated cleanup pass and proceed with background production immediately.

### 5.4 Do NOT delete

* `BaryonAsym/H1SIDIS_LowQ2_v5g/` (1.1 TB) — active sidis-lowQ² production.
* `BaryonAsym/H1BaryonAsym/` (331 GB) — active Λ/K0s workspace.
* `klesthen/LambdaAnalysis/` (271 GB) — supervisor-claimed analysis; mtime
  2024-10-21 not stale enough to delete without explicit confirmation.
* `klesthen/g1j_ES/` (134 GB) — **this is the reference workspace whose
  steering files we are mirroring.** Keep.

---

## 6. Production plan (Task 5)

### 6.1 Source: clone g1j_ES chain definitions into SIDIS steering tree

Canonical SIDIS layout discovered on NAF:
`H1ChargedSidis_HighQ2/steering/{evls,sidistree}/mc/<generator>/<catalog>/<steer>.steer`

Each individual `.steer` file references exactly one HAT + one MODS file
(per current `submit_full_production.py` output). Example
`evls_highQ2_djangoh14_7091_ep0607_0000.steer`:

```
H1SteerTree() {
   fHatFiles  = "/pnfs/.../djangoh14/7091/hat....DST.0000.root";
   fModsFiles = "/pnfs/.../djangoh14/7091/mods....DST.0000.root";
   fLoadODS   = false;
}
```

**Proposed background layout** (parallels existing signal):
```
H1ChargedSidis_HighQ2/steering/evls/mc_bg/<generator>/<catalog>/evls_highQ2_<gen>_<cat>_<period>_NNNN.steer
H1ChargedSidis_HighQ2/steering/sidistree/mc_bg/<generator>/<catalog>/sidistree_<gen>_<cat>_<period>_NNNN.steer
```

Where `<generator>` ∈ {`pythia62`, `tintin` (DVCS), `compton20`, `h1grape`,
`djangoh14_lowQ2bkgd`, `djangoh14_cc`} and `<catalog>` is the pnfs catalog
number (7236, 8346, 8051, 7128, 7110, 7339/7348, etc.).

The chain → catalog/file mapping is fully derivable from the parsed
inventory JSON; the generator/extender script will be:

```
scripts/submit_bg_production.py
  --stage {evls|sidistree|all}
  --gen {pythia|dvcs|compton|grape|djbkgd|ccbkgd|all}
  --period {ep0304|em0405|em06|ep0607|all}
  [--dry-run]
```

This script is a copy of `submit_full_production.py` with
`MC_CATALOGS` replaced by a `MC_BG_CATALOGS` table populated from
`g1jES_inventory.json` (the local JSON). Output paths land at
`output/mc_bg/<generator>/<catalog>/sidistree_*.root`.

### 6.2 Binaries

* Stage 1: `bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis`
  (SIDIS-owned, since Phase 1A 2026-05-23).
* Stage 2: `bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v5g`
  (current production v5g). The v5f binary is **not** to be used per
  `submit_full_production.py:88`.

The supervisor's brief asserts "background tree schemas have already been
verified bit-identical to signal." This is the load-bearing assumption.
Decision point D1 below proposes the smallest sufficient confirmation.

### 6.3 Two-stage pipeline

Identical to signal: evls (filter & event-list) → sidistree (build flat
SIDIS tree). Stage 2 reads the per-event Stage-1 event-list ROOT files
from `output/evls/mc_bg/<gen>/<cat>/`, then writes the flat tree to
`output/mc_bg/<gen>/<cat>/`. Wall time per Stage-2 job ≤ 5 s/file
extrapolated from signal MC; Stage 1 typically dominates at ~5–30 s/file
(reading from `/pnfs/` is the cost driver).

### 6.4 Per-event extraction

After Stage 2, run the existing extractors over the new output tree dirs:

* `scripts/extract_pertrack_mc_v3/extract_pertrack_mc_reco_v3.py`
* `scripts/extract_pertrack_mc_v3/extract_pertrack_mc_gen_v3.py`
* `scripts/extract_pertrack_mc_v3/extract_pertrack_mc_disevent_v3.py`

These will need a `--mc-bg-root output/mc_bg/<gen>/<cat>` argument
parallel to the existing `--mc-root output/mc/<gen>/<cat>` (small patch;
the extractors currently iterate over `output/mc/<gen>/<cat>/`). Output
CSVs land at `output/pertrack_mc_v3_bg/<gen>/{reco,gen,disevent}/`.

### 6.5 Output location summary

```
output/
  evls/mc_bg/<gen>/<cat>/evls_highQ2_<gen>_<cat>_<period>_NNNN.root          # Stage 1
  mc_bg/<gen>/<cat>/sidistree_<gen>_<cat>_<period>_NNNN.root                  # Stage 2
  pertrack_mc_v3_bg/<gen>/{reco,gen,disevent}/pertrack_v3_*.csv.gz            # post-extraction
```

### 6.6 Condor cluster plan

One submission per (generator × period) combination. Effective per-cluster
job counts (= # Stage-2 jobs = # input DST sub-files, one job per file):

| Period   | Pythia | DVCS | Compton | Grape | DjBkgd | CCBkgd | Period total |
|----------|------:|----:|-------:|-----:|------:|------:|------------:|
| ep0304   | 1300 |  21 | 101 |  50 | 170 |  50 | **1,692** |
| em0405   |  942 |  32 | 101 |  50 | 200 |  50 | **1,375** |
| em06     |  842 |  32 | 101 |  10 | 180 |  50 | **1,215** |
| ep0607   | 1300 |  30 | 101 |  10 | 250 |  50 | **1,741** |
| **All**  | **4,384** | **115** | **404** | **120** | **800** | **200** | **6,023** |

**24 Condor clusters total** (6 generators × 4 periods), one per
(gen, period). Sub-runs within a cluster run in parallel.

**Wall-time estimate per cluster.** Assuming Stage-2 ≤ 5 s/file, Stage-1
~10–30 s/file (PNFS read-bound), and 200-job concurrent slots on NAF:

| Cluster size | Stage-1 wall | Stage-2 wall | Total per cluster |
|-------------:|------------:|-------------:|------------------:|
| ≤ 100 files (DVCS, Compton, Grape, CCBkgd) | < 1 min | < 1 min | **≤ 2 min** |
| 100–300 (DjBkgd small periods, Grape em06) | 1–3 min  | < 1 min | **≤ 5 min** |
| 300–1000 (DjBkgd ep0607, all em06/em0405 Pythia) | 5–15 min | 1–3 min | **≤ 20 min** |
| 1000–1400 (Pythia ep0304 / ep0607) | 15–30 min | 3–5 min | **≤ 30 min** |

**All clusters fit comfortably within 30 min wall**, well under the
+MaxRuntime=7200 used by `submit_full_production.py`.

**Serial total** (if all 24 clusters ran one-after-another): ~4–5 h of
wall time. Realistically, **submit all 24 in one batch** = whole
production wraps in 30 min wall + ~30 min for Stage-2 to drain after
Stage-1 finishes per cluster = **≤ 1 h end-to-end**.

### 6.7 Disk delta after full production

| Workspace | Current | After bg | Delta |
|-----------|--------:|---------:|------:|
| `H1ChargedSidis_HighQ2/` | 177 GB | ≈ 582 GB | **+405 GB** |
| `BaryonAsym/` | 1.66 TB | ≈ 2.07 TB | +405 GB |
| `klesthen/` | 1.79 TB | ≈ 2.20 TB | +405 GB |
| DUST group | 3.2 PB used / 5.3 PB total | 3.2 PB + 0.4 TB | +0.013 % of group quota |

**No capacity concern.**

### 6.8 Smoke-test plan (1 sub-run per generator)

Recommended proof-of-concept job per generator (single Stage-1 + Stage-2 on
the smallest period that has chains for that generator) before launching
the bulk:

| Generator | Smoke chain | Catalog | Period | Reason |
|-----------|-------------|---------|--------|--------|
| Pythia    | `Pythia_Eminus06_1` | `pythia62/7236` (or similar) | em06 | smallest pythia chain count for em06; verify schema |
| DVCS      | `DVCS_Eplus0304_1`  | `tintin/8052` | ep0304 | smallest by # files (21 sub-DST) |
| Compton   | `Compton_Eplus0304_1` | `compton20/8346` | ep0304 | standard Compton schema |
| Grape     | `Grape_Eminus06`    | `h1grape/7128` (or em06 equiv) | em06 | only 10 explicit-file chain |
| DjBkgd    | `DjBkgd_0304_1`     | `djangoh14/7110` | ep0304 | small (10 sub-DST), Django schema = trivially OK |
| CCBkgd    | `CCBkgd_0304`       | `djangoh14/{7339,7348}` | ep0304 | exotic CC schema; verify |

Smoke-test verdict gate: each smoke must (a) emit a sidistree ROOT file
with > 0 entries, (b) the SIDIS branches (`Q2_es`, `pT_h`, `phi_h`, `R_BC`,
`charged_tot`) must be populated, (c) the v5g scattered-electron suppression
must apply — verifiable by spot-checking the SCAT_ELEC track is excluded
from `ChargedTot` in 1 event. If any of these fail for a given generator,
**defer that generator** and escalate.

---

## 7. Safety checklist (Task 6)

| # | Check | Status | Evidence |
|---|-------|--------|----------|
| 2 | `/pnfs/.../djangoh14/7093` accessible (existing signal) | **PASS** | `ls -d` returned path |
| 3 | `/pnfs/.../djangoh14/8955` accessible (MC13 signal) | **PASS** | `ls -d` returned path |
| 4 | `/pnfs/.../pythia62/7236` accessible (Pythia γp bg) | **PASS** | `ls -d` returned path |
| 5 | `/pnfs/.../compton20/8346` accessible (Compton) | **PASS** | `ls -d` returned path |
| 6 | `/pnfs/.../tintin/8052` accessible (DVCS Milou) | **PASS** | `ls -d` returned path |
| 7 | `/pnfs/.../h1grape/7128` accessible (Grape) | **PASS** | `ls -d` returned path |
| 8 | Stage-1 binary present | **PASS** | `bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis` |
| 9 | Stage-2 binary present (v5g) | **PASS** | `bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v5g` |
| 10 | `submit_full_production.py` exists & inspected | **PASS** | 1,216 lines; structure understood |
| 11 | Per-period GRL files exist | **DEFER** | `H1ChargedSidis_HighQ2/data/` does NOT exist; GRL is likely at `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_*.root` per submit script header; not directly verified this pass |
| 12 | Pertrack extractors exist | **PASS** | `scripts/extract_pertrack_mc_v3/{reco,gen,disevent}_v3.py` present |
| 13 | Background MC does NOT require TrigWeightAC | **CLAIMED PASS** | Per brief; not independently verified — recommend code-grep in extractor confirming the AC trigger weight is only applied to chains tagged `eData` |
| 14 | Background tree schema bit-identical to signal | **UNVERIFIED — DECISION POINT D1** | Brief asserts this; recommend Pythia + DVCS + Compton smoke-tests as the minimum cross-schema validation before bulk launch |
| 15 | DUST group filesystem has head-room for 405 GB | **PASS** | 5.3 PB total, 60 % used = 2.1 PB free |
| 16 | No conflict with active condor production | **CHECK at submit-time** | `H1SIDIS_LowQ2_v5g` is actively producing (1.1 TB output dir, mtime today); recommend coordinating cluster submission windows to avoid slot starvation |

---

## 8. Decision points for supervisor (before launch)

* **D1 (BLOCKING). Schema compatibility.** Confirm that the v5g Stage-2
  binary `ChargedSidisTreeBuilderHighQ2_v5g` correctly reads (a) Pythia 6.2
  γp DST events, (b) DVCS Milou DST events, (c) Compton20 DST events,
  (d) H1grape lepton-pair DST events. Approach: run §6.8 smoke-tests
  (one Stage-1 + Stage-2 job per generator class). Estimated wall time
  ≤ 2 min per smoke job × 6 = ≤ 15 min total. **No bulk production until
  all 6 smokes pass with non-zero output and populated SIDIS branches.**

* **D2 (OPTIONAL). Norad/BORN denominators.** The g1j_ES tree contains
  Norad / DjNorad / RaNorad / NoradBORN steerings (~600 additional
  sub-files; ~37 GB additional spend). These are needed **only if** we
  plan to compute C_rad **on each background**. For the baseline
  background-subtraction use case (background contribution to data minus
  background contribution to signal MC = corrected SIDIS yield), C_rad is
  applied at the signal level only and backgrounds are subtracted RAW.
  **Recommend: defer Norad/BORN production** until a documented C_rad-on-bg
  requirement appears. If approved later, add as Phase 2 of this plan.

* **D3 (OPTIONAL, OUT OF SCOPE). MC13 signal re-production.** The g1j_ES
  set's MC13 signal Django/Rapgap is **3× larger** than the existing v5g
  signal sample (2,646 MC13 files vs. 865 MC10 files). Re-producing signal
  on MC13 would yield much smaller statistical uncertainties on the
  acceptance correction. Estimated additional spend: ~ 175 GB. **Not part
  of the background production proposal**; flag for separate decision.

* **D4 (OPTIONAL). Cleanup pass.** Section 5 lists ~58 GB of deletion
  candidates. None block this production. Recommend: defer to a dedicated
  cleanup PR.

* **D5 (CONFIRMATORY). Trigger weighting in extractors.** The brief
  asserts "background MC does NOT need TrigWeightAC (already baked into
  existing extractors)." Recommend a 1-line `grep TrigWeightAC` audit of
  the three pertrack-mc-v3 extractors to confirm the AC trigger weight is
  guarded by an `if generator in (data-like)` predicate. If applied
  unconditionally, the background CSV would carry an inappropriate weight.

* **D6 (CONFIRMATORY). Per-period GRL wiring.** The submit script header
  says GRL is `LumiFiles/nc_goodruns_*.root`; the canonical SIDIS pipeline
  injects the per-period GRL on the `--good-runs` flag. Confirm this is
  inherited unchanged by the new `submit_bg_production.py` for each
  background period.

---

## 9. Recommended execution order

1. **Phase 0 (≤ 1 day).** Resolve D5, D6 with grep / inspection. Write
   `scripts/submit_bg_production.py` by `cp submit_full_production.py
   submit_bg_production.py` + populate `MC_BG_CATALOGS` from the parsed
   `g1jES_inventory.json` (committed to repo at `H1/sidis_highQ2/data/`).
2. **Phase 1 (≤ 30 min).** Submit 6 smoke-test jobs (§6.8). One per
   generator class, smallest catalog. Each is a single (Stage-1 + Stage-2)
   pair. Inspect outputs against the v5g schema checklist (entries > 0,
   SIDIS branches populated, no v5G-1 contamination). **D1 gate.**
3. **Phase 2 (≤ 1 h).** Submit all 24 Condor clusters as a single batch.
   Monitor via `condor_q`; expect ≤ 1 h end-to-end. Capture per-cluster
   Stage-1/Stage-2 file counts and disk deltas in
   `condor_logs/bg_highQ2_2026-05-XX/`.
4. **Phase 3 (≤ 30 min).** Run the patched
   `extract_pertrack_mc_{reco,gen,disevent}_v3.py` against the new
   `output/mc_bg/` tree. Inspect merged CSV sizes against §3.2 projections
   (re-calibrate Pythia in particular).
5. **Phase 4 (review).** Run the canonical post-extraction analysis
   pipeline against the combined (signal + background) per-track tree.
   Document the impact of background subtraction on the leading SIDIS
   asymmetries / multiplicities. **This is the validation deliverable.**

---

## Appendices

* **Inventory JSON**: `/var/folders/n1/.../opencode/g1jES_inventory.json`
  (598 chain records; structure documented inline). To be copied into
  the H1 repo at `H1/sidis_highQ2/data/g1jES_mc_inventory.json` as part
  of Phase 0.
* **Parser script**: `/var/folders/n1/.../opencode/parse_steerings.py`.
* **Source steerings (read-only mirror at /tmp on NAF)**:
  `/tmp/es.{ep0304,em0405,em06,ep0607}disMC13.steer`.

<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Staleness Sweep: `H1SIDIS_HighQ2/scripts/` (and cross-check `H1SIDIS_LowQ2/scripts/`)

**Date:** 2026-05-20
**Author:** researcher (delegated by supervisor)
**Scope:** Pre-migration (Task Z8) sweep of every script under
`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/` for
v5b-staleness markers: references to removed `ProtonTot`/`ProtonTotGen`
trees, removed dE/dx PID branches (`LkProton`, `Dedx{Pion,Kaon,Proton}`,
`Nlh{Pion,Kaon,Proton}`), or non-SIDIS observables
(proton/antiproton asymmetry, `A_p`, `dsigma_p`/`dsigma_pbar`,
`GenPDG==±2212`-only filters).

**Cross-check:** `H1SIDIS_LowQ2/scripts/` was also scanned for any
HighQ2-targeted scripts that live in the wrong directory.

**Mode:** Read-only on NAF. SSH control socket check at start:
`Master running (pid=64865)` -- PASS. No NAF writes. No execution of
any script.

**Reference v5b SIDIS schema (per `sidis_lowQ2/context-handoff.md`
Section 4 and `validate_v3_output.py:46-58`):**
- `ChargedTot = 75` (or 86 in raw count per the task brief; the audit
  uses the live builder count of 75/v3.1)
- `ChargedTotGen = 44` (low-Q2) / `49` (high-Q2)
- `DisTot = 46` (low-Q2) / `45` (high-Q2)
- **REMOVED:** `ProtonTot`, `ProtonTotGen` trees
- **REMOVED:** `LkProton`, `LkPion`, `LkKaon`, `DedxProton`, `DedxPion`,
  `DedxKaon`, `NlhProton`, `NlhPion`, `NlhKaon`, `DedxRaw`
- **RETAINED:** `NHitDedx` (track-quality diagnostic only)

> The task brief states `ChargedTot=86 / ChargedTotGen=50 / DisTot=49`.
> The live v3.1 builder per the handoff produces `75/44/46` (low-Q2) and
> `75/49/45` (high-Q2). The discrepancy is between the v5b raw branch count
> at builder emission and the v3.1 trimmed-schema count after the dE/dx PID
> drops -- it does not affect this audit's pass/fail classification. The
> tree NAMES (`ChargedTot`/`ChargedTotGen`/`DisTot`) are stable across
> v3.1 / v5b / v5c-OPT1 / v5d / v5e.

---

## 1. Executive Summary

### 1.1 Counts

| Metric | Value |
|---|---:|
| Total scripts in `H1SIDIS_HighQ2/scripts/` | **13** |
| `*.py` | 8 |
| `*.sh` | 5 |
| `*.C` | 0 |

### 1.2 Disposition breakdown

| Category | Count | Scripts |
|---|---:|---|
| **KEEP (ACTIVE-SIDIS)** | **3** | `analyze_sidis_charged.py`, `analyze_sidis_full.py`, `analyze_sidis_tmd.py` |
| **KEEP (INFRASTRUCTURE)** | **5** | `submit_full_production.py`, `submit_proton_highQ2_batch.sh`, `merge_data_highQ2.sh`, `merge_mc_highQ2.sh`, `pull_results.sh` |
| **DEPRECATE** | **5** | `analyze_proton_highQ2.py`, `analyze_sidis_acceptance.py`, `analyze_sidis_acceptance_v2.py`, `smoke_test_analysis.py`, `validate_outputs_highQ2.py` |
| **INVESTIGATE** | **0** | -- |
| **AMBIGUOUS** | **0** | -- |

### 1.3 Headline findings

1. **The entire May-15 22:58 cohort (7 scripts) was dropped into the
   directory in one bulk operation.** It mixes proton-asymmetry leftovers
   (`analyze_proton_highQ2.py`, `smoke_test_analysis.py`,
   `validate_outputs_highQ2.py`) with legitimate infrastructure
   (`submit_full_production.py`, the merge scripts, `pull_results.sh`).
   The infrastructure scripts correctly drive the v5b SIDIS builder
   (`ProtonTreeBuilderHighQ2` is the legacy-named SIDIS v5b builder per
   the Task J audit) -- they only look proton-named, they actually produce
   SIDIS schema output. KEEP them.
2. **The Apr 25-27 cohort (5 `analyze_sidis_*` scripts)** is mixed:
   - `analyze_sidis_charged.py`, `analyze_sidis_full.py`, and
     `analyze_sidis_tmd.py` are the **real SIDIS analyzers** that read
     `ChargedTot`/`ChargedTotGen` for MC. KEEP. **They share one defect:
     they read DATA from `ProtonTot` and assume the data-side schema is
     still the proton-asymmetry schema. This is a known data-readiness
     issue documented in `context-handoff.md` Section 6 ("Low-Q2 data
     compatibility decision") and the high-Q2 analog. It is a defect to
     fix, not a stale-fork-leftover.**
   - `analyze_sidis_acceptance.py` and `analyze_sidis_acceptance_v2.py`
     are misnamed: their bodies compute **proton-tracking** acceptance
     (gen/reco matched **protons**) using `ProtonTot`/`ProtonTotGen` and
     `GenPDG==±2212`. They are SIDIS-titled but proton-asymmetry-bodied.
     DEPRECATE.
3. **No CRITICAL cross-script breakage** if the 5 DEPRECATE candidates
   are removed:
   - `validate_outputs_highQ2.py` is invoked by `merge_data_highQ2.sh`
     and `merge_mc_highQ2.sh`. Its removal requires those two shell
     scripts to be re-pointed at the canonical
     `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2`,
     which already exists, supports both variants, and forbids the
     `ProtonTot` tree by design.
   - No other DEPRECATE candidate is imported, sourced, or shell-invoked
     by any of the 8 KEEP scripts.
4. **No HighQ2 scripts are misplaced in `H1SIDIS_LowQ2/scripts/`** beyond
   the already-flagged `analyze_proton_full.py` (Task U).
   `validate_v3_output.py` lives in LowQ2 and supports HighQ2 via
   `--variant highQ2`; it is correctly positioned as a shared utility and
   is the SIDIS-correct replacement for the deprecated
   `validate_outputs_highQ2.py`.
5. **Low-Q2 SIDIS is NOT at risk from any DEPRECATE action.** Every
   DEPRECATE candidate lives in `H1SIDIS_HighQ2/scripts/`. The low-Q2
   chain uses `H1SIDIS_LowQ2/scripts/` and the local
   `H1/sidis_lowQ2/scripts/`; none of them import or invoke any of the
   five HighQ2 deprecation candidates.
6. **Workspace migration (Task Z8) can proceed safely.** Recommendation:
   carry KEEP set forward; quarantine DEPRECATE set in a
   `legacy_proton_asym_leftover/` subdir; re-point the two merge shell
   scripts to use the canonical validator.

---

## 2. Per-script inventory

### 2.1 `H1SIDIS_HighQ2/scripts/`

All sizes in bytes. mtime in NAF local time.

| # | Filename | Size | mtime | Category | Disposition | One-line summary |
|---:|---|---:|---|---|---|---|
| 1 | `analyze_proton_highQ2.py` | 21,818 | 2026-05-15 22:58 | analyze_* | **DEPRECATE** | Proton/antiproton asymmetry analyzer (`A = (N_p/eff_p − N_pbar/eff_pbar)/sum`). Reads `ProtonTot`/`ProtonTotGen`, uses `LkProton`/`DedxRaw`, `GenPDG==±2212`, hard-codes proton-analysis luminosity `356,613 nb⁻¹`. Already classified STALE FORK-LEFTOVER in Task O (`investigation-analyze-proton-highQ2-script.md`). Not invoked by anything. |
| 2 | `analyze_sidis_acceptance.py` | 27,687 | 2026-04-26 01:52 | analyze_* | **DEPRECATE** | Despite the SIDIS-flavored filename, the body computes **proton** tracking acceptance from `ProtonTot`/`ProtonTotGen` with `RECO_MATCHED_POS` using `GenPDG==2212`. GEN_FID is the proton-momentum window `0.3 < GenP < 1.3`. Computes proton/antiproton "charge asymmetry" A_corr from acceptance-corrected proton yields. Calls `fdata.Get('ProtonTot')` and `fmc.Get('ProtonTotGen')`. |
| 3 | `analyze_sidis_acceptance_v2.py` | 8,345 | 2026-04-26 02:10 | analyze_* | **DEPRECATE** | Slimmer v2 of #2. Hard-coded `DATA_FILE = merged_proton_highQ2_data_all.root`, opens `ProtonTot`/`ProtonTotGen`, `RECO_MATCHED = ... abs(GenPDG)==2212`, GEN_FID requires `GenP > 0.3 && GenP < 1.3` (proton momentum window). Same staleness pattern as #2. |
| 4 | `analyze_sidis_charged.py` | 27,089 | 2026-04-25 22:58 | analyze_* | **KEEP** (ACTIVE-SIDIS, with caveat) | SIDIS charged-particle distributions at high Q². Docstring explicitly says "No proton PID cuts are applied -- this is a charged particle analysis." Cuts `|eta|<1.3, Pt>0.15, Q2s>150`. **BUT** opens `fdata.Get('ProtonTot')` and `fmc.Get('ProtonTot')` for DATA (line 602, 610). This matches the documented "data-side still has v4 schema" provenance: per `validation_highQ2_data_availability.json`, the high-Q2 DATA files on NAF still carry the v4 proton-asym schema (ProtonTot=56). The script needs a one-line data-tree-name fix once data are reprocessed under v5b, but the observable IS the SIDIS charged-particle distribution. Migration should KEEP and patch. |
| 5 | `analyze_sidis_full.py` | 30,196 | 2026-04-26 09:13 | analyze_* | **KEEP** (ACTIVE-SIDIS, with caveat) | Charge-separated SIDIS distributions, A_h, MC acceptance, acceptance-corrected A_h for 8 SIDIS variables. Docstring: "MC: ChargedTot (reco, IsMatched) + ChargedTotGen (gen, inFiducialCharged)." Computes inclusive `A_h = (N_{h+} − N_{h-})/sum`, NOT proton asymmetry. Same `ProtonTot`-for-data caveat as #4: line 692-696 streams data from `ProtonTot`. KEEP and patch when data reprocessing lands. |
| 6 | `analyze_sidis_tmd.py` | 50,741 | 2026-04-27 03:45 | analyze_* | **KEEP** (ACTIVE-SIDIS, with caveat) | SIDIS TMD analysis with gen-level p_T,h acceptance correction. Streams `ChargedTot`/`ChargedTotGen` for MC (lines 1138, 1189). Same `ProtonTot`-for-data caveat (line 1084) -- treats data as ProtonTot. The SIDIS observables (z, pTh, phi_h) and Q2/x/y reconstruction are SIDIS-correct. KEEP and patch when data reprocessing lands. |
| 7 | `merge_data_highQ2.sh` | 455 | 2026-05-15 22:58 | merge_* | **KEEP** (INFRASTRUCTURE) | `hadd -f merged_proton_highQ2_data_all.root <data/*.root>`. Output filename uses legacy proton-prefix per the Task J audit naming convention. Then invokes `validate_outputs_highQ2.py --mode data` -- this invocation must be **re-pointed to** `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2` when #9 is deprecated. |
| 8 | `merge_mc_highQ2.sh` | 774 | 2026-05-15 22:58 | merge_* | **KEEP** (INFRASTRUCTURE) | hadd MC by generator (`rapgap31`, `djangoh14`) into `merged_proton_highQ2_mc_<gen>.root`. Also invokes `validate_outputs_highQ2.py --mode mc` twice. Same re-pointing required. |
| 9 | `pull_results.sh` | 1,428 | 2026-05-15 22:58 | other (rsync) | **KEEP** (INFRASTRUCTURE) | rsync of plots/tables/JSON from `H1SIDIS_HighQ2/output/{validation,plots}/` to local. Observable-agnostic. |
| 10 | `smoke_test_analysis.py` | 10,475 | 2026-05-15 22:58 | smoke_* | **DEPRECATE** | Quick-look "smoke test" but computes **proton-asymmetry** observable: line 114 cut `LkProton > {PID_CUT}` + `P > 0.3 && P < 1.3`; line 187 prints `A_raw = (N_p − N_pbar) / (N_p + N_pbar)`. Opens `ProtonTot`. Output filename `asymmetry_highQ2_smoke.txt`. Computes nothing SIDIS-related. |
| 11 | `submit_full_production.py` | 28,152 | 2026-05-15 22:58 | submit_* | **KEEP** (INFRASTRUCTURE) | Two-stage Condor submitter: Stage 1 (`TreeBuilder --stage evls`), Stage 2 (`ProtonTreeBuilderHighQ2`). Despite the legacy `PROTON_*` variable names, this is the live v5b SIDIS production driver. The builder it invokes (`ProtonTreeBuilderHighQ2`) IS the SIDIS v5b builder per the Task J audit. The script does NOT itself open any tree or compute any observable -- it is a thin Condor harness. KEEP. |
| 12 | `submit_proton_highQ2_batch.sh` | 4,151 | 2026-05-15 22:58 | submit_* | **KEEP** (INFRASTRUCTURE) | Template-style bash batch submitter (`for steer in highQ2_proton_data_*.steer; do condor_submit ... done`). Functional equivalent / fallback for #11. Same legacy naming, same v5b SIDIS builder invocation. KEEP. |
| 13 | `validate_outputs_highQ2.py` | 7,034 | 2026-05-15 22:58 | validate_* | **DEPRECATE** | Validates `ProtonTot` tree exists with ~50 expected branches (`PROTON_TOT_BRANCHES` includes `DedxRaw`, `DedxProton`, `LkProton`, `LkPion`, `LkKaon`, `NlhProton`, `NlhPion`, `NlhKaon`, etc.) and `ProtonTotGen` tree with `inFiducial` branch. Reports FAIL if `ProtonTot` is absent. This is the v4-schema validator. The canonical replacement is `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2`, which expects `ChargedTot=75 / ChargedTotGen=49 / DisTot=45` and explicitly FORBIDS `ProtonTot`. |

### 2.2 Cross-script invocations (within `H1SIDIS_HighQ2/scripts/`)

| Caller | Callee | Notes |
|---|---|---|
| `merge_data_highQ2.sh:11` | `validate_outputs_highQ2.py` | **Hard dependency.** Must re-point when #13 is deprecated. |
| `merge_mc_highQ2.sh:11,17` | `validate_outputs_highQ2.py` | **Hard dependency.** Must re-point when #13 is deprecated. |
| (none) | `analyze_proton_highQ2.py` | Orphan (self-references only). |
| (none) | `analyze_sidis_acceptance.py` | Orphan. |
| (none) | `analyze_sidis_acceptance_v2.py` | Orphan. |
| (none) | `analyze_sidis_charged.py` | Orphan (run manually). |
| (none) | `analyze_sidis_full.py` | Orphan (run manually). |
| (none) | `analyze_sidis_tmd.py` | Orphan (run manually). |
| (none) | `smoke_test_analysis.py` | Orphan. |
| (none) | `submit_full_production.py` | Top-level user-invoked. |
| (none) | `submit_proton_highQ2_batch.sh` | Top-level user-invoked. |
| (none) | `pull_results.sh` | Top-level user-invoked. |

**Only one cross-script dependency exists** within the directory:
the two `merge_*.sh` scripts depend on `validate_outputs_highQ2.py`. All
five `analyze_*` scripts and `smoke_test_analysis.py` are orphans.

### 2.3 v5b-staleness pattern summary

| Script | `ProtonTot` (data) | `ProtonTotGen` | `LkProton`/`Dedx*`/`Nlh*` | `GenPDG==±2212` filter | proton/antiproton observable | DEPRECATE? |
|---|:-:|:-:|:-:|:-:|:-:|:-:|
| analyze_proton_highQ2.py     | YES (reco) | YES | YES (LkProton+DedxRaw) | YES | YES (A_p/pbar) | **YES** |
| analyze_sidis_acceptance.py  | YES | YES | NO | YES | YES (proton-acc, "A_corr") | **YES** |
| analyze_sidis_acceptance_v2.py | YES | YES | NO | YES | YES (proton-acc) | **YES** |
| analyze_sidis_charged.py     | YES (data only) | NO | NO | NO | NO (charged-h dists) | NO |
| analyze_sidis_full.py        | YES (data only) | NO (uses ChargedTotGen) | NO | NO | NO (A_h inclusive) | NO |
| analyze_sidis_tmd.py         | YES (data only) | NO (uses ChargedTotGen) | NO | NO | NO (TMD multiplicities) | NO |
| smoke_test_analysis.py       | YES | NO | YES (LkProton) | implicit (Charge>0 vs <0 under LkProton cut) | YES (A_raw) | **YES** |
| validate_outputs_highQ2.py   | YES (validates as required) | YES (validates as required) | YES (validates as required) | NO | NO (validator) | **YES** |
| submit_full_production.py    | NO | NO | NO | NO | NO | NO |
| submit_proton_highQ2_batch.sh | NO | NO | NO | NO | NO | NO |
| merge_data_highQ2.sh         | NO (just hadd) | NO | NO | NO | NO | NO |
| merge_mc_highQ2.sh           | NO (just hadd) | NO | NO | NO | NO | NO |
| pull_results.sh              | NO | NO | NO | NO | NO | NO |

The three KEEP analyzers (#4-#6) that have `ProtonTot` in their **data-read
path only** are not stale-fork-leftovers in the v5b-schema sense: the data
files on NAF still carry the v4 proton-asym schema (per
`validation_highQ2_data_availability.json:6,11-12`), so reading them from
`ProtonTot` is *correct against the current state of the data*. When the
HighQ2 data is reprocessed under v5b (Task Z8 follow-up), each of these
three scripts needs a one-line tree-name update
(`'ProtonTot'` → `'ChargedTot'`). This is a known migration item, not a
deprecation.

---

## 3. Cross-check against `H1SIDIS_LowQ2/scripts/`

NAF listing of `H1SIDIS_LowQ2/scripts/`: 24 files (Python, shell, ROOT
macro, backup). Already audited by Task K, Task O, Task U, and the
`audit-proton-naming-highQ2-sidis.md` review.

### 3.1 Scripts in LowQ2 that match HighQ2 patterns

Grep for `highQ2|HighQ2|Q2.*>.*150` in `H1SIDIS_LowQ2/scripts/`:

| Script | Hit type | Verdict |
|---|---|---|
| `validate_v3_output.py` | Supports `--variant highQ2` with expected schema `ChargedTot=75 / ChargedTotGen=49 / DisTot=45` | **CORRECTLY PLACED.** This is a shared two-variant utility. It is the **canonical SIDIS-correct replacement** for the HighQ2 `validate_outputs_highQ2.py`. It explicitly forbids `ProtonTot` (`FORBIDDEN_TREES = ("ProtonTot",)`). |

No other LowQ2 script matches HighQ2 keywords. **No misplaced HighQ2
scripts found in `H1SIDIS_LowQ2/scripts/` beyond what Task U already
flagged.**

### 3.2 Local-vs-NAF SIDIS mapping (for the migration plan)

This table lists the local `H1/sidis_lowQ2/scripts/` chain that is the
SIDIS-canonical implementation, with the NAF-side counterpart (if any).
**This is the most important table for Task Z8.**

| Function | Local canonical (`H1/sidis_lowQ2/scripts/`) | NAF HighQ2 equivalent | Disposition for migration |
|---|---|---|---|
| Multiplicity extraction (M^{h±}) | `extract_multiplicities.py` (57,195 B, May 20) | -- | Adapt local for HighQ2; no NAF counterpart |
| φ_h moments | `extract_phi_moments_data.py` (75,792 B, May 19) | -- | Adapt local for HighQ2 |
| φ_h unfolding | `unfold_phih.py` (67,499 B, May 19) | -- | Adapt local for HighQ2 |
| MVP systematics | `compute_systematics_charged.py` (76,639 B, May 20) | -- | Adapt local for HighQ2 |
| Shifted-MC production | `produce_shifted_mc.py` / `.C` (May 19) | -- | Reuse local |
| Schema/output validation | (local lookalike?) | `H1SIDIS_LowQ2/scripts/validate_v3_output.py` (already two-variant) | **Use NAF LowQ2 script for HighQ2 too** -- replace HighQ2's `validate_outputs_highQ2.py` |
| TMD analysis prototype | `tmd_analysis.py` (46,431 B, May 19) | `analyze_sidis_tmd.py` (HighQ2, KEEP-with-patch) | Decide: keep both or migrate HighQ2 to local-equivalent. The NAF `analyze_sidis_tmd.py` is older (Apr 27) and computes a similar observable; recommend reviewing whether the local `tmd_analysis.py` already supersedes it before keeping the NAF copy. |
| Acceptance correction (TMD) | `tmd_resolution_acceptance.py` (72,566 B, May 19) | `analyze_sidis_acceptance.py` (HighQ2, DEPRECATE -- proton-bodied) | Migrate local for HighQ2 |
| Charged-particle SIDIS analysis | (in `analyze_charged_xsec_corrected.py` per handoff) | `analyze_sidis_charged.py`, `analyze_sidis_full.py` | Compare and decide which to carry. Recommend: keep `analyze_sidis_full.py` (the most complete SIDIS analyzer of the three KEEP analyzers), retire `analyze_sidis_charged.py` if its observables are fully covered. |
| Condor production submission | -- | `submit_full_production.py` + `submit_proton_highQ2_batch.sh` | KEEP NAF (production scripts live on NAF by design). |
| Merge | (no local hadd driver) | `merge_data_highQ2.sh`, `merge_mc_highQ2.sh` | KEEP NAF |
| Result rsync | -- | `pull_results.sh` | KEEP NAF (rsync is NAF→local by definition) |
| HighQ2 C_rad | `compute_highQ2_crad_charged.py` / `.C` (May 20) | -- | Local-only; HighQ2-specific charged C_rad. |
| HighQ2 multiplicity (Task W) | `task_W_multiplicity_highQ2.py` (May 20) | -- | Local-only. |
| HighQ2 data submission (27.6 GeV) | `submit_highQ2_sidis_data_27p6.py` (May 20) | -- | Local-only. |
| v5e QA plots | `make_v5e_qa_plots.py` (May 20) | -- | Local-only. |

**Local-side HighQ2 chain already exists** for C_rad, Task-W multiplicity,
data submission (27.6), and QA. The migration target is to converge the
NAF HighQ2 analysis cohort onto local-canonical SIDIS modules,
deprecating proton-asym leftovers and re-using the LowQ2-provided
`validate_v3_output.py` schema validator.

---

## 4. Detailed disposition with reasoning

### 4.1 DEPRECATE (5 scripts)

#### D1. `analyze_proton_highQ2.py` (21,818 B, 2026-05-15 22:58)
**Reason:** All five staleness criteria triggered.
- Trees: `TREE_RECO = "ProtonTot"`, `TREE_GEN = "ProtonTotGen"` (lines 52-53)
- PID: `LkProton > {pid_cut}` default 0.1 (line 509), `DedxRaw` (lines 149, 183)
- Species: `pdg_cut = "GenPDG == 2212"` / `"GenPDG == -2212"` (line 211)
- Observable: `A = (N_p/eff_p − N_pbar/eff_pbar) / (sum)` (docstring lines 11-12)
- Luminosity: hard-coded `DATA_LUMI_ALL = 356613.0 nb⁻¹` (S67 proton-analysis luminosity, not the S61 SIDIS luminosity the superseded pre-rescale luminosity nb⁻¹).
Already classified STALE FORK-LEFTOVER in Task O. Not invoked by any
script in `H1SIDIS_HighQ2/scripts/`, `H1SIDIS_LowQ2/scripts/`, or
`H1BaryonAsym_HighQ2/scripts/`. ZERO breakage if removed.

#### D2. `analyze_sidis_acceptance.py` (27,687 B, 2026-04-26 01:52)
**Reason:** SIDIS-titled but proton-bodied.
- Trees: `data_tree = fdata.Get('ProtonTot')` (line 623), `mc_tree = fmc.Get('ProtonTot')` (line 631), `gen_tree = fmc.Get('ProtonTotGen')` (line 632). Hard-fails if missing.
- Species filter: `RECO_MATCHED_POS = ... && GenPDG == 2212` (line 118); `GEN_FID_POS = GEN_FID + ' && GenPDG > 0'` (line 131) -- protons; `GEN_FID_NEG = GEN_FID + ' && GenPDG < 0'` -- antiprotons.
- Observable: docstring lines 8-9: `ε(X) = N_reco_matched_proton(X) / N_gen_fiducial_proton(X)` -- this is **proton-species** acceptance, NOT inclusive charged-hadron acceptance.
- Fiducial: GEN_FID is the proton-momentum window `0.3 ≤ GenP ≤ 1.3` (line 131 implicit via `GEN_FID`).
Not invoked by any other script. ZERO breakage if removed.

#### D3. `analyze_sidis_acceptance_v2.py` (8,345 B, 2026-04-26 02:10)
**Reason:** Same pattern as D2, slimmer body.
- Hardcoded `DATA_FILE = "output/merged/merged_proton_highQ2_data_all.root"` (line 10), `MC_FILE = "output/merged/merged_proton_highQ2_mc_rapgap31.root"` (line 11).
- `td = fd.Get("ProtonTot")` (line 36), `tr = fm.Get("ProtonTot")` (line 38), `tg = fm.Get("ProtonTotGen")` (line 39).
- `RECO_MATCHED = ... && IsMatched == 1 && abs(GenPDG) == 2212` (line 31).
- `GEN_FID = "abs(GenEta) < 1.3 && GenPt > 0.15 && GenP > 0.3 && GenP < 1.3"` (line 33) -- proton momentum window.
Not invoked. ZERO breakage.

#### D4. `smoke_test_analysis.py` (10,475 B, 2026-05-15 22:58)
**Reason:** "Smoke" name + SIDIS-flavored docstring, proton-asymmetry body.
- Opens `ProtonTot` (line 101, 108).
- Cut `base_cut = f"P > {P_MIN} && P < {P_MAX} && abs(Eta) < {ETA_MAX} && LkProton > {PID_CUT}"` (line 114) with `P_MIN=0.3, P_MAX=1.3, PID_CUT=0.1` -- the dE/dx-PID-safe proton-momentum window with the proton-likelihood cut.
- Output: `asymmetry_highQ2_smoke.txt` writes `A_raw = (N_p − N_pbar) / (N_p + N_pbar)` (lines 187-188).
Computes nothing SIDIS-related. Not invoked by anything. ZERO breakage.

#### D5. `validate_outputs_highQ2.py` (7,034 B, 2026-05-15 22:58)
**Reason:** Validates the **wrong** (v4 proton-asym) schema.
- Checks for `ProtonTot` tree with branches including `DedxRaw, DedxProton, DedxPion, DedxKaon, LkProton, LkPion, LkKaon, NlhProton, NlhPion, NlhKaon` (lines 27-29) -- all REMOVED in v5b.
- Checks for `ProtonTotGen` tree with `inFiducial` branch (line 170) -- the proton-asym fiducial flag, not the SIDIS `inFiducialCharged`.
- Will report ALL FAIL on any v5b SIDIS file (no `ProtonTot` present).
**Has 2 callers:** `merge_data_highQ2.sh:11`, `merge_mc_highQ2.sh:11,17`. Both callers must be re-pointed to the canonical `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2` (already exists, schema-correct, forbids `ProtonTot`).

### 4.2 KEEP -- ACTIVE-SIDIS (3 scripts)

#### K1. `analyze_sidis_charged.py` (27,089 B, 2026-04-25 22:58)
SIDIS charged-particle distribution analyzer at Q² > 150 GeV². Observable:
single-differential / charge-separated / 2D distributions for all charged
hadrons (h+ and h-). Cuts: `|eta|<1.3, Pt>0.15, Q2s>150, xF_SIDIS!=-999`.
No proton PID cuts (explicit in docstring lines 7-8).
**Caveat:** opens `ProtonTot` for data (lines 602, 610). This is the
data-side schema-staleness issue documented in `context-handoff.md`. It
will fail on v5b-reprocessed data unless patched to read `ChargedTot`.
Migration: KEEP; patch tree name when HighQ2 data reprocessing under v5b
completes.

#### K2. `analyze_sidis_full.py` (30,196 B, 2026-04-26 09:13)
Comprehensive charged SIDIS analyzer: charge-separated distributions,
hadron charge asymmetry A_h, MC acceptance, acceptance-corrected A_h for
8 SIDIS variables. Reads `ChargedTot` (reco) and `ChargedTotGen` (gen)
for MC (lines 692-750). Streams data from `ProtonTot` (line 692) -- same
caveat as K1.
Observable: `A_h = (N_{h+} − N_{h-}) / sum` (inclusive charged), NOT
proton asymmetry. Uses `inFiducialCharged` (line 9) -- correct SIDIS
fiducial flag. Migration: KEEP; patch data tree name.

#### K3. `analyze_sidis_tmd.py` (50,741 B, 2026-04-27 03:45)
SIDIS TMD analyzer with gen-level p_T,h acceptance correction. Reads
`ChargedTot` / `ChargedTotGen` for MC (lines 1138, 1189). Reads data from
`ProtonTot` (line 1084) -- same caveat. Observable set: p_T,h
distributions, z ∈ [0.2, 0.8] cut, multiplicities in (z, p_T,h) bins.
Migration: KEEP; patch data tree name. May be partially superseded by
local `tmd_analysis.py` -- recommend a direct comparison before declaring
this the canonical HighQ2 TMD analyzer.

### 4.3 KEEP -- INFRASTRUCTURE (5 scripts)

#### I1. `submit_full_production.py` (28,152 B, 2026-05-15 22:58)
Two-stage Condor submitter. Stage 1: `TreeBuilder --stage evls` (from
`H1BaryonAsym_HighQ2`). Stage 2: `ProtonTreeBuilderHighQ2` (the v5b
SIDIS builder, legacy-named per Task J audit). Despite all
`PROTON_*` variable names and the "Stage 2 (proton)" docstring, the
output of this script is the v5b SIDIS schema. The script does NOT
itself read any tree or compute any observable -- it is a Condor harness.
Migration: KEEP. Note the legacy naming should be addressed under the
Tier 3 rename in the Task J audit, not in the staleness cleanup.

#### I2. `submit_proton_highQ2_batch.sh` (4,151 B, 2026-05-15 22:58)
Bash submitter template. Globs `highQ2_proton_data_*.steer` and calls
`condor_submit` per file, invoking
`bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` (the SIDIS v5b
builder). Functional fallback for I1. Migration: KEEP.

#### I3. `merge_data_highQ2.sh` (455 B, 2026-05-15 22:58)
`hadd -f merged_proton_highQ2_data_all.root <data/*.root>` then invokes
the deprecated validator. Migration: KEEP and patch the validator
invocation to `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2`.

#### I4. `merge_mc_highQ2.sh` (774 B, 2026-05-15 22:58)
`hadd` per-generator: `rapgap31`, `djangoh14`. Same validator-patch
requirement as I3. Migration: KEEP.

#### I5. `pull_results.sh` (1,428 B, 2026-05-15 22:58)
rsync of `output/{validation,plots}/` from NAF to local
`pngs/highQ2/`. Observable-agnostic. Migration: KEEP.

---

## 5. Risk assessment for Task Z8 (workspace migration)

### 5.1 Low-Q2 SIDIS exposure

**Zero exposure.** None of the 5 DEPRECATE scripts is imported, sourced,
or shell-invoked by any script in `H1SIDIS_LowQ2/scripts/` or
`H1/sidis_lowQ2/scripts/`. The local LowQ2 analysis chain
(`extract_multiplicities.py`, `extract_phi_moments_data.py`,
`unfold_phih.py`, `compute_systematics_charged.py`, etc.) is fully
self-contained.

The two LowQ2 scripts that share the proton-asym fork-leftover heritage
(`analyze_proton_full.py`, `merge_proton_output.sh`,
`submit_proton_batch.sh`) are out of scope for this audit -- they were
already addressed by Tasks K, O, U.

### 5.2 HighQ2 SIDIS exposure

Three KEEP analyzers (K1, K2, K3) have a **data-tree-name patch
prerequisite** before they can run on v5b-reprocessed data. This is a
known issue documented in `context-handoff.md` and is not a
staleness-sweep problem. The migration plan should track it as a
dependency: HighQ2 data must be reprocessed under v5b before these
analyzers can read it cleanly.

Two merge scripts (I3, I4) have a **validator-invocation patch
prerequisite** before D5 (`validate_outputs_highQ2.py`) is removed. The
patch is a one-line shell change to call
`H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2`.

### 5.3 Other concerns

- **Legacy naming everywhere (`PROTON_*` variables, `proton_highQ2_*.root`
  output basenames)** is documented in the Task J audit. NOT addressed
  in this sweep; not blocking for Z8 if Tier 1+2 alias-doc is in place.
- **The Task O wrinkle that `proton_HiQ/context-handoff.md` cites a
  non-existent NAF path (`H1ProtonAsymHighQ2/`)** is still open and
  unrelated to this sweep.

### 5.4 Migration safety verdict

**Task Z8 (workspace migration) can proceed safely** provided the
following five actions are taken **in this order**:

1. Patch `merge_data_highQ2.sh:11` and `merge_mc_highQ2.sh:11,17` to
   call `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2`
   instead of the local `validate_outputs_highQ2.py`.
2. Quarantine D1-D5 (rename to `.deprecated.py` with header, OR move
   to `legacy_proton_asym_leftover/` subdir). Recommend the `.deprecated.py`
   rename pattern already established by Task U.
3. Patch K1-K3's data-tree open call (`'ProtonTot'` → `'ChargedTot'`)
   WHEN HighQ2 data v5b reprocessing completes. NOT before, or they
   will fail on the existing v4-schema data files. (Optionally: add a
   runtime fallback that tries `ChargedTot` first and falls back to
   `ProtonTot` with a deprecation warning, to bridge the cutover.)
4. Keep I1-I5 unchanged through the migration. Their legacy names are
   harmless because the builder they drive IS the SIDIS v5b builder.
5. Update the migration manifest (Z8) to reflect the
   `validate_outputs_highQ2.py` → `validate_v3_output.py --variant highQ2`
   substitution.

---

## 6. Critical KEEP set (5-10 items)

For Task Z8, these are the must-carry-forward files:

1. **`submit_full_production.py`** -- the live two-stage v5b SIDIS Condor
   driver. Critical.
2. **`submit_proton_highQ2_batch.sh`** -- functional fallback / template
   submitter. Critical (used by data 27.6 GeV plan per local
   `submit_highQ2_sidis_data_27p6.py`).
3. **`merge_mc_highQ2.sh`** -- MC hadd driver. Critical for the merged
   `rapgap31` and `djangoh14` products feeding all downstream MC
   analysis.
4. **`merge_data_highQ2.sh`** -- DATA hadd driver. Critical when v5b
   HighQ2 data lands.
5. **`analyze_sidis_full.py`** -- the most complete HighQ2 SIDIS analyzer
   (8 SIDIS variables, A_h, MC acceptance, acceptance-corrected A_h).
6. **`analyze_sidis_tmd.py`** -- gen-level p_T,h acceptance-corrected
   TMD analyzer.
7. **`analyze_sidis_charged.py`** -- single-/double-differential
   charged-h dists (simpler than `analyze_sidis_full.py`, useful for
   sanity checks).
8. **`pull_results.sh`** -- rsync to local. Convenience but cheap to
   keep.

Plus from `H1SIDIS_LowQ2/scripts/`:
9. **`validate_v3_output.py`** -- the canonical SIDIS-correct schema
   validator. Should be invoked by the HighQ2 merge scripts after the
   migration patch.

---

## 7. AMBIGUOUS list

**None.** Every script was classifiable into KEEP or DEPRECATE on the
evidence available. The three KEEP analyzers (K1-K3) have a known
data-side schema caveat but it is clearly documented and clearly a
patch issue, not an ambiguity.

---

## 8. Risks, assumptions, gaps

### Risks
- If the validator patch in I3/I4 is forgotten, the merge step will
  emit FAIL on every merged file (or, if D5 is deleted first,
  emit a missing-file error). Mitigation: patch I3/I4 *before*
  removing D5.
- If K1/K2/K3 are patched to read `ChargedTot` for data BEFORE the
  HighQ2 v5b data reprocessing is complete, they will fail on the
  existing v4-schema data. Mitigation: schedule the patch in the same
  migration step as the v5b data reprocessing, or use a runtime
  fallback.

### Assumptions
- `ProtonTreeBuilderHighQ2` (the binary invoked by I1, I2) IS the v5b
  SIDIS builder. Source of assumption: Task J audit, Section 4 ("the
  builder identity is SIDIS-physics despite the legacy filename"), and
  the validated v5b MC outputs at
  `H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/<run>/proton_highQ2_mc_*.root`
  carry the `ChargedTot=86`/`ChargedTotGen=50`/`DisTot=49` schema (per
  `validation_highQ2_mc_manifest.json`).
- HighQ2 DATA is still on the v4 proton-asym schema (per
  `validation_highQ2_data_availability.json`). If this has changed since
  2026-05-19, the data-tree-name patch on K1-K3 can be done immediately
  instead of being scheduled with the reprocessing.

### Gaps
- The actual analyze-time behavior on a real v5b MC file was not tested
  for the KEEP analyzers; the schema-compatibility claim is inferred
  from the scripts' tree-name choices.
- No `git` history is available in `H1SIDIS_HighQ2/scripts/` (no
  `.git/`), so the May-15 22:58 cohort's authorship is unknown.
- The local `H1/sidis_lowQ2/scripts/` HighQ2 module set
  (`compute_highQ2_crad_charged.py`, `task_W_multiplicity_highQ2.py`,
  `submit_highQ2_sidis_data_27p6.py`) was not deeply compared against
  the NAF KEEP analyzers; some functional overlap may exist that would
  let the migration retire more NAF scripts than this audit recommends.
  Recommend a follow-up direct comparison.

---

## 9. One-paragraph summary

Of the 13 scripts in `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/`,
**5 are stale fork-leftovers to be deprecated**
(`analyze_proton_highQ2.py`, `analyze_sidis_acceptance.py`,
`analyze_sidis_acceptance_v2.py`, `smoke_test_analysis.py`,
`validate_outputs_highQ2.py`); **3 are live SIDIS analyzers to keep**
(`analyze_sidis_charged.py`, `analyze_sidis_full.py`,
`analyze_sidis_tmd.py`; each requires a one-line data-tree-name patch
once HighQ2 data is reprocessed under v5b); **5 are infrastructure
scripts to keep** (`submit_full_production.py`,
`submit_proton_highQ2_batch.sh`, `merge_data_highQ2.sh`,
`merge_mc_highQ2.sh`, `pull_results.sh`). The only cross-script
dependency on a DEPRECATE candidate is the two `merge_*.sh` scripts
calling `validate_outputs_highQ2.py`; the canonical SIDIS-correct
replacement, `H1SIDIS_LowQ2/scripts/validate_v3_output.py --variant highQ2`,
already exists and supports HighQ2. **No HighQ2 scripts are misplaced
in `H1SIDIS_LowQ2/scripts/` beyond `analyze_proton_full.py` already
flagged by Task U.** **Low-Q2 SIDIS is at zero risk** from any DEPRECATE
action. **Task Z8 (workspace migration) can proceed safely** provided
the two merge scripts are re-pointed at `validate_v3_output.py` before
`validate_outputs_highQ2.py` is removed.

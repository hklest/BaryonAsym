<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Investigation: `analyze_proton_highQ2.py` in `H1SIDIS_HighQ2/scripts/`

**Date:** 2026-05-19
**Scope:** disambiguate the AMBIGUOUS Category-3 entry from `audit-proton-naming-highQ2-sidis.md`
(Section 4.1, item 6d). Determine whether `analyze_proton_highQ2.py` is a stale
fork-leftover, an active but misplaced proton-analysis script, or an active SIDIS
script with legacy naming.
**Access mode:** read-only on NAF. SSH control socket checked once at start
(`Master running, pid=16259`). No NAF writes; one `scp` pull of the script
to local `/var/folders/.../opencode/proton_inv/` for inspection.
**Inputs read:**
- `H1/sidis_highQ2/docs/reviews/audit-proton-naming-highQ2-sidis.md` (full).
- `H1/sidis_lowQ2/context-handoff.md` (full).
- `H1/proton_HiQ/context-handoff.md` (full).
- `/var/folders/.../analyze_proton_highQ2.py` (full, 578 lines, copied from
  `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/analyze_proton_highQ2.py`).
- NAF directory listings of `H1SIDIS_HighQ2/scripts/`, `H1SIDIS_LowQ2/scripts/`,
  `H1BaryonAsym_HighQ2/scripts/`, and the BaryonAsym root.

---

## 1. Findings at a glance

| Question | Finding |
|---|---|
| File present on NAF? | YES, at the expected path `H1SIDIS_HighQ2/scripts/analyze_proton_highQ2.py` |
| Size / mtime | 21,818 bytes; **mtime `May 15 22:58`** (2026-05-15) |
| Executable bit set? | YES (`-rwxr-xr-x`) |
| Docstring observable | proton/antiproton asymmetry `A = (N_p/eff_p - N_pbar/eff_pbar) / (sum)` and `dsigma_p`, `dsigma_pbar` |
| Trees opened | `ProtonTot` (reco) and `ProtonTotGen` (gen) — the REMOVED-in-v5b schema |
| PID approach | dE/dx-based: `LkProton` cut (default `LkProton > 0.1`) + `DedxRaw` PID branch + `P < 1.3 GeV` cap |
| Gen-species ID | `GenPDG == 2212` / `GenPDG == -2212` (proton-specific, NOT inclusive charged-hadron) |
| Hard-coded legacy values | `DATA_LUMI_ALL = 356613.0 nb^-1` (HERA-II S67 — wrong for the SIDIS analysis); `LkProton`/`DedxRaw`/`inFiducial` branch names (all REMOVED in v5b) |
| Output | PNG plots + `summary_table_highQ2.txt`; header literally prints `H1SIDIS_HighQ2 — Analysis Summary` while computing proton/antiproton asymmetry |
| Invoked by any other script in `H1SIDIS_HighQ2/scripts/`? | **NO** — `grep -l 'analyze_proton_highQ2' .../H1SIDIS_HighQ2/scripts/*` returns only the file itself |
| Invoked anywhere else on NAF (BaryonAsym root)? | **NO** — `grep -l` in `H1BaryonAsym_HighQ2/scripts/` returns nothing |
| Referenced in any local SIDIS doc/plan/handoff? | **NO** — `grep` of `H1/sidis_lowQ2` returns no hits |
| Sibling `analyze_proton_lowQ2.py` exist? | NO file with that exact name. The *parent* `analyze_proton_full.py` lives at `H1SIDIS_LowQ2/scripts/analyze_proton_full.py` — itself a fork-leftover (the script header acknowledges "H1 primary-vertex proton/antiproton analysis", not SIDIS) |
| SIDIS analysis scripts in the same directory? | YES, four older files: `analyze_sidis_acceptance.py`, `analyze_sidis_acceptance_v2.py`, `analyze_sidis_charged.py`, `analyze_sidis_full.py`, `analyze_sidis_tmd.py` (all Apr 25-27, 2026; ~9 weeks older mtime) — these are the actual SIDIS analyzers |
| Where does the live high-Q2 proton/antiproton analysis actually live? | `/data/dust/.../BaryonAsym/H1BaryonAsym_HighQ2/` (NOT `H1ProtonAsymHighQ2/` as `proton_HiQ/context-handoff.md` line 24 claims; that path does not exist on disk) |

**Classification: STALE FORK-LEFTOVER (with two complicating wrinkles — see
Section 6).**

The script is a proton/antiproton asymmetry analyzer copied from the low-Q2
proton chain into the SIDIS high-Q2 scripts directory on 2026-05-15. It reads
the REMOVED v5b trees, uses the REMOVED v5b dE/dx PID branches, and computes
the proton/antiproton asymmetry observable that does NOT belong to the SIDIS
analysis. It is not invoked by anything. It has not been touched since the day
it was created.

---

## 2. What the script actually does

### 2.1 Docstring (verbatim, lines 1–21)

> ```
> analyze_proton_highQ2.py — Full analysis for H1SIDIS_HighQ2
>
> Adapts the low-Q2 proton analysis chain (analyze_proton_full.py) to the
> high-Q2 phase space (Q2 > 150 GeV^2) with coarser binning appropriate
> for the reduced statistics (~35K candidates vs ~500K at low Q2).
>
> Physics chain (iteration 1):
>     dsigma_p/dX    = N_p    / (eff_p    * L_data * C_rad * DeltaX)
>     dsigma_pbar/dX = N_pbar / (eff_pbar * L_data * C_rad * DeltaX)
>     A = (N_p/eff_p - N_pbar/eff_pbar) / (N_p/eff_p + N_pbar/eff_pbar)
> ```

The docstring is unambiguous: the observable is the **proton/antiproton charge
asymmetry**, which is the deliverable of the *separate* proton/antiproton
analysis (see `H1/proton_HiQ/context-handoff.md` line 30: "Charge asymmetry
A_ch rises from ~0.3% (z < 0.04) to ~37% (z > 0.5)"). The SIDIS observables
defined in `sidis_lowQ2/context-handoff.md` Section 1 are `A_h` (charged-hadron
asymmetry, `(N_{h+} - N_{h-})/(N_{h+} + N_{h-})`), `M^{h±}`, and the φ_h
moments. **The observable in this script is NOT one of those.**

The first line of the docstring (`"Full analysis for H1SIDIS_HighQ2"`) is the
*only* SIDIS-flavored phrase in the file. It is descriptive of the *directory
the script was dropped into*, not of the *physics it computes*.

### 2.2 Trees opened (lines 52–53, 531–544)

```python
TREE_RECO = "ProtonTot"
TREE_GEN  = "ProtonTotGen"
```

Used in `open_tree(args.data, TREE_RECO)` and `f_mc.Get(TREE_GEN)`.

These are the **REMOVED-in-v5b** trees. Per `sidis_lowQ2/context-handoff.md`
line 247: "v3.1 removes the ProtonTot and ProtonTotGen trees…". Per
`audit-proton-naming-highQ2-sidis.md` Section 6: every SIDIS doc that mentions
these two trees does so to label them as REMOVED. The v5b SIDIS schema uses
`ChargedTot`, `ChargedTotGen`, and `DisTot` (see `context-handoff.md`
Section 4). **This script targets the wrong schema.**

If pointed at a v5b SIDIS-built output file (`proton_highQ2_mc_*.root`), the
`open_tree` call at line 124 will trigger
`ERROR: TTree 'ProtonTot' not found in <path>` and `sys.exit`. The script
cannot function on the v5b SIDIS production files. It *can* still function on
the older v4 proton/antiproton outputs, which do contain `ProtonTot` /
`ProtonTotGen` (see `audit-proton-naming-highQ2-sidis.md` Section 6,
`validation_highQ2_data_availability.json:6,12`).

### 2.3 PID branches used (REMOVED in v5b)

| Branch | Used at line(s) | Status in v5b |
|---|---|---|
| `DedxRaw` | 149, 183 | **REMOVED** (context-handoff.md line 248) |
| `LkProton` | 509 (`pid_cut`) | **REMOVED** (context-handoff.md line 249) |

The `pid_cut` constructed on line 509 (`f"LkProton > {args.pid_cut} && P < {args.p_max} && P > 0.3"`) embeds both:
- `LkProton > 0.1` — the proton-likelihood cut from the proton/antiproton analysis.
- `P < 1.3` and `P > 0.3` — the proton-momentum window (0.3–3.0 GeV in
  low-Q2; capped at 1.3 here for dE/dx-PID-safe region). Per
  `sidis_lowQ2/context-handoff.md` line 60: "The P momentum window (0.3-3.0
  GeV) from the proton/antiproton analysis has been explicitly DROPPED for
  the charged-hadron SIDIS measurement."

The script also references `inFiducial` (line 219), which is the proton
analysis's gen-fiducial flag, NOT the SIDIS `inFiducialCharged`. This is
another schema-version mismatch.

### 2.4 Observable computed

Per the docstring and the function bodies:

- `compute_raw_asymmetry` (lines 290–314): computes `A = (N_p - N_pbar) / (N_p + N_pbar)` where
  `N_p` is `Charge > 0` and `N_pbar` is `Charge < 0`, **with the
  `LkProton` PID cut applied**. This is the proton/antiproton asymmetry
  (positive *protons*, negative *antiprotons*), NOT the inclusive
  charged-hadron asymmetry `A_h` (which would be all positive hadrons / all
  negative hadrons regardless of species).
- `compute_efficiency` (lines 201–231): MC efficiency in proton/antiproton
  bins using `GenPDG == 2212` (proton) and `GenPDG == -2212` (antiproton).
  This is **proton-species-specific** efficiency, not the inclusive
  charged-hadron efficiency the SIDIS analysis needs.
- `plot_corrected_asymmetry_vs_p`: efficiency-corrected proton/antiproton
  asymmetry.
- `make_summary_table`: tabulates `N_p`, `N_pbar`, `A_raw`, `sigma_A`.

**No multiplicity (`M^{h±}`) computation.** **No `A_h` (charged-hadron
asymmetry) computation.** **No `φ_h` moment extraction.** None of the SIDIS
TMD observables are produced. The script's entire output set is proton/antiproton.

### 2.5 Hard-coded legacy values

| Line | Value | Issue |
|---|---|---|
| 33 | `DATA_LUMI_ALL = 356613.0` nb⁻¹ | This is the **total HERA-II S67 luminosity** as used by the proton analysis. The SIDIS-low-Q2 analysis uses `the superseded pre-rescale luminosity nb⁻¹` (S61, see context-handoff.md Section 3). The SIDIS-high-Q2 analysis would use a different luminosity again. This is the proton-analysis luminosity, hard-coded. |
| 36 | `Q2_BINS = [150.0, 300.0, 700.0, 20000.0]` | High-Q2 ≥ 150 GeV² — consistent with the proton_HiQ scope. |
| 37 | `P_BINS = [0.3, 0.7, 1.0, 1.3]` | The dE/dx-PID-safe proton-momentum window. Not applicable to SIDIS h±. |
| 38 | `PT_BINS = [0.15, 0.4, 0.7, 1.0]` | Generic pT binning. |
| 42–49 | `P_SLICES` for dE/dx characterization | dE/dx PID infrastructure — proton-analysis only. |
| 499 | `--p-max default=1.3` | dE/dx-PID-safe cap. |
| 497 | `--pid-cut default=0.1` | `LkProton` threshold default. |

### 2.6 Output

- PNG plots written to `--output-dir`:
  - `dedx_vs_p_highQ2.png`
  - `dedx_slices_highQ2.png`
  - `efficiency_vs_p_highQ2.png`
  - `efficiency_vs_q2_highQ2.png`
  - `raw_asymmetry_vs_p_highQ2.png`
  - `raw_asymmetry_vs_q2_highQ2.png`
  - `corrected_asymmetry_vs_p_highQ2.png`
- Text file: `summary_table_highQ2.txt` with header
  `"H1SIDIS_HighQ2 — Analysis Summary (iteration 1)"` and a table of `N_p`,
  `N_pbar`, `A_raw`, `sigma_A` in P-bins.
- No JSON output. No machine-readable contract.

Note the cognitive dissonance: the printed banner says "H1SIDIS_HighQ2" but
the numbers printed underneath are proton/antiproton asymmetries.

---

## 3. Provenance

### 3.1 Date evidence

- File mtime: **2026-05-15 22:58** (exactly the same minute as
  `merge_data_highQ2.sh`, `merge_mc_highQ2.sh`, `pull_results.sh`,
  `smoke_test_analysis.py`, `submit_full_production.py`,
  `submit_proton_highQ2_batch.sh`, `validate_outputs_highQ2.py`). All
  seven files share the May-15 22:58 timestamp.
- The five `analyze_sidis_*.py` files in the same directory have mtimes from
  **Apr 25–27, 2026** — about three weeks earlier.

Interpretation: the May-15 cohort was likely created in a single session
(probably a bulk copy or a single commit dropping a "high-Q2 production +
analysis kit" into the SIDIS scripts directory). The pre-existing
`analyze_sidis_*.py` files were untouched in that operation.

### 3.2 Lineage

The script's docstring (lines 4–5) self-attributes:

> "Adapts the low-Q2 proton analysis chain (analyze_proton_full.py) to the
> high-Q2 phase space"

The parent file exists on NAF at:

```
/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2/scripts/analyze_proton_full.py
```

with header docstring:

> "analyze_proton_full.py — Comprehensive H1 primary-vertex proton/antiproton
> analysis."

So `analyze_proton_highQ2.py` is a high-Q2 adaptation of a low-Q2
proton/antiproton analyzer that *itself* lives in the SIDIS low-Q2 scripts
directory under the same legacy-naming pattern. **Both files share the same
provenance pathology:** proton-analysis code surviving in directories that
have since been re-purposed for SIDIS.

### 3.3 Where the actual high-Q2 proton/antiproton analysis lives

`proton_HiQ/context-handoff.md` line 24 declares the package root as
`/data/dust/.../BaryonAsym/H1ProtonAsymHighQ2/`. **That directory does not
exist on NAF.** It was apparently renamed at some point. The live
high-Q2 proton/antiproton package is:

```
/data/dust/group/h1/klesthen/BaryonAsym/H1BaryonAsym_HighQ2/
```

Its `scripts/` directory contains a completely different analysis toolchain
(many ROOT C macros: `highq2_full_analysis.C`, `highq2_final_results.C`,
`highq2_radiative_corrections.C`, `highq2_systematics.C`, plus Python:
`extract_yields.py`, `compute_cross_sections.py`, `asymmetry_breit.py`, etc.).
None of those scripts reference `analyze_proton_highQ2`.

**This means:** the real proton/antiproton high-Q2 analysis on NAF does NOT
use `analyze_proton_highQ2.py`. It uses an unrelated C-macro-based pipeline.
The Python script in `H1SIDIS_HighQ2/scripts/` is therefore not even the
active proton-analysis script — it appears to be either an experimental
adaptation that never landed, or a forgotten copy from a prior workflow.

### 3.4 No invocation anywhere

```
ssh naf "grep -l 'analyze_proton_highQ2' .../H1SIDIS_HighQ2/scripts/*"
  → only itself
ssh naf "grep -l 'analyze_proton_highQ2' .../H1BaryonAsym_HighQ2/scripts/*"
  → no matches
grep -rn 'analyze_proton_highQ2' H1/sidis_lowQ2/
  → no matches
grep -rn 'analyze_proton_highQ2' H1/
  → no matches
```

The script is:
- not imported anywhere,
- not invoked by any shell script in either the SIDIS or the proton package
  on NAF,
- not referenced by any local doc/plan/handoff in the SIDIS workspace,
- not part of any documented analysis chain.

It is an orphan.

### 3.5 Sibling-script analysis (what SIDIS scripts coexist with it)

| File in `H1SIDIS_HighQ2/scripts/` | mtime | What it does |
|---|---|---|
| `analyze_sidis_acceptance.py` | Apr 26 01:52 | SIDIS acceptance (the real thing) |
| `analyze_sidis_acceptance_v2.py` | Apr 26 02:10 | SIDIS acceptance v2 |
| `analyze_sidis_charged.py` | Apr 25 22:58 | SIDIS charged-hadron analyzer |
| `analyze_sidis_full.py` | Apr 26 09:13 | SIDIS full analysis |
| `analyze_sidis_tmd.py` | Apr 27 03:45 | SIDIS TMD analysis |
| `analyze_proton_highQ2.py` | **May 15 22:58** | Proton/antiproton asymmetry (this file) |
| `validate_outputs_highQ2.py` | May 15 22:58 | Output validator |
| `smoke_test_analysis.py` | May 15 22:58 | Smoke tests |
| `merge_mc_highQ2.sh`, `merge_data_highQ2.sh` | May 15 22:58 | Merge scripts |
| `submit_full_production.py`, `submit_proton_highQ2_batch.sh` | May 15 22:58 | Submitters |
| `pull_results.sh` | May 15 22:58 | Result puller |

The SIDIS-named scripts (Apr 25–27) are the real SIDIS analyzers. The
proton-named script (May 15) is a different toolchain dropped in alongside
them but never wired in.

---

## 4. Classification

**STALE FORK-LEFTOVER.**

Justification (against each criterion in the task brief):

| Criterion | Evidence |
|---|---|
| References REMOVED-in-v5b trees | `TREE_RECO = "ProtonTot"`, `TREE_GEN = "ProtonTotGen"` (lines 52–53). Cannot read v5b SIDIS files. |
| Uses REMOVED-in-v5b PID branches | `DedxRaw`, `LkProton` (lines 149, 183, 509). Cannot run on v5b outputs. |
| Computes a non-SIDIS observable | Proton/antiproton asymmetry per the docstring lines 11–12. SIDIS observables are A_h, M^{h±}, φ_h moments — none are produced. |
| Uses MC-truth species filter `GenPDG == ±2212` | Lines 211, 219. Proton-species-specific, not inclusive charged hadron. |
| Not invoked by any SIDIS code | Confirmed via grep across `H1SIDIS_HighQ2/scripts/` (no hits except self). |
| Not invoked by any proton analysis code | Confirmed via grep across `H1BaryonAsym_HighQ2/scripts/` (no hits). |
| Hard-coded legacy luminosity | `DATA_LUMI_ALL = 356613.0 nb⁻¹` (line 33). |
| No SIDIS doc treats it as authoritative | Zero references in the SIDIS workspace. |

The only counter-evidence is the docstring's first line
(`"Full analysis for H1SIDIS_HighQ2"`) and the printed banner
(`"H1SIDIS_HighQ2 — Analysis Summary"`). These appear to be artifacts of the
author dropping the script into the H1SIDIS_HighQ2 directory and naming the
banner after the directory, NOT a statement that the physics it computes is
SIDIS physics. The body of the script is unambiguously proton/antiproton.

This is not "ACTIVE BUT MISPLACED" because:
- The actual high-Q2 proton/antiproton analysis (`H1BaryonAsym_HighQ2/`)
  uses an entirely different toolchain and does not need or invoke this
  script.
- The script targets the v4-era schema (`ProtonTot` + `LkProton` + dE/dx PID)
  which is itself superseded for the proton analysis: per
  `proton_HiQ/context-handoff.md` the proton-asymmetry results were extracted
  by the live `H1BaryonAsym_HighQ2/scripts/` chain (`extract_yields.py`,
  `compute_cross_sections.py`, etc.), not by this Python script.

This is not "ACTIVE SIDIS WITH WRONG NAME" because:
- The observable is NOT a SIDIS observable.
- The trees, PID cuts, momentum window, and luminosity are all
  proton-analysis specific.
- A simple rename would NOT make it a SIDIS analyzer; the body would have to
  be rewritten end-to-end.

This is not "UNCLEAR" because:
- The evidence is consistent and unambiguous in all the dimensions checked.

---

## 5. Recommended disposition

**RECOMMENDATION: DEPRECATE / QUARANTINE on NAF; do NOT integrate, do NOT
rely on it for any SIDIS or proton analysis output.**

In ranked order of preference:

### Option A (recommended): mark stale, leave in place

- Add a one-line `DEPRECATED.md` (or top-of-file comment if a NAF edit is
  ever permitted) flagging the script as a fork-leftover that targets the
  REMOVED v5b schema and is not invoked by any production pipeline. This
  costs nothing operationally because nothing runs the script.
- The script is harmless as long as nothing executes it on v5b inputs (it
  would just `sys.exit("ERROR: TTree 'ProtonTot' not found")`).
- The naming-aliases doc recommended in `audit-proton-naming-highQ2-sidis.md`
  Section 9 should explicitly call out this file.

### Option B: move it (DEFER until a NAF write window is approved)

- Target: a quarantine subdirectory such as
  `H1SIDIS_HighQ2/scripts/legacy_proton_asym_leftover/`, OR re-home into
  `H1BaryonAsym_HighQ2/scripts/legacy_python_attempts/` if the proton
  team would find it useful as a historical reference.
- BUT: per `H1/CONTEXT.md` SSH rules and per `audit-proton-naming-highQ2-sidis.md`
  Section 4.2 Tier-4 cost analysis, any move on NAF is mid-pipeline risk and
  should not be done casually. Defer to the same window as the broader
  proton-naming cleanup.

### Option C: delete

- The script has no consumers and is harmless. Deletion is also acceptable.
- Risk: if anyone later wants to revive the proton-Python-analysis pathway
  they would have to re-derive it. Low risk given that `analyze_proton_full.py`
  (the parent) still exists in `H1SIDIS_LowQ2/scripts/`.
- Same NAF-write caveat as Option B.

### Why NOT rename and reuse for SIDIS

The script cannot be salvaged for the SIDIS analysis. Every meaningful
analysis line (tree names, PID cuts, momentum window, GenPDG cuts, output
plot semantics, summary-table semantics) would need to be rewritten. The
existing `analyze_sidis_*.py` files in the same directory already cover the
SIDIS workflow, and the local `H1/sidis_lowQ2/scripts/` chain
(`tmd_analysis.py`, `compute_systematics_charged.py`,
`extract_phi_moments_data.py`, `unfold_phih.py`) is the actual authoritative
SIDIS pipeline.

### Breakage if moved/deleted

**Zero SIDIS scripts would break.** Confirmed via grep. The script is an
orphan with no consumers.

The proton/antiproton high-Q2 analysis on NAF (`H1BaryonAsym_HighQ2/`) does
not depend on it either; that analysis uses a different toolchain. Confirmed
via grep.

---

## 6. Wrinkles worth surfacing to the supervisor

1. **`proton_HiQ/context-handoff.md` cites a nonexistent NAF path.** Line 24
   says `H1ProtonAsymHighQ2/`. That directory does not exist on NAF. The
   live package is `H1BaryonAsym_HighQ2/`. This is an independent defect in
   the proton_HiQ handoff that should be filed against that analysis's
   maintainers. It also matches the documentation-drift defect flagged in
   `audit-proton-naming-highQ2-sidis.md` lines 218 and 223 for
   `research-highQ2-sidis-extension.md`, which uses the same stale
   `H1ProtonAsymHighQ2/` name.

2. **Parent script `analyze_proton_full.py` is also misplaced.** It lives in
   `H1SIDIS_LowQ2/scripts/` but its own header says "H1 primary-vertex
   proton/antiproton analysis." This is the same class of fork-leftover, in
   the low-Q2 package, and should be evaluated in a follow-up investigation
   if cleanup of the SIDIS-LowQ2 scripts directory is in scope. (It is
   referenced by `H1SIDIS_LowQ2/scripts/merge_proton_output.sh` and
   `submit_proton_batch.sh`, also fork-leftovers.)

3. **The May-15 cohort in `H1SIDIS_HighQ2/scripts/` is consistent with a
   bulk drop.** Seven scripts share the May-15 22:58 timestamp. If this was
   a single supervisor or builder rollout, then the *intent* of dropping
   `analyze_proton_highQ2.py` into that directory is recoverable from the
   session record around 2026-05-15. This investigation did not attempt
   that recovery; mtime is the only provenance signal used.

4. **The script would silently do the wrong thing if pointed at a legacy v4
   `proton_highQ2_*.root` file** that still happens to have `ProtonTot` and
   `ProtonTotGen`. Such files exist per
   `audit-proton-naming-highQ2-sidis.md` Section 6 (`validation_highQ2_data_availability.json:11–12`
   references `proton_highQ2_data_smoke.root` with the proton-asymmetry schema,
   56 branches, no `ChargedTot`/`DisTot`). Running this script on such a
   file would produce *correct* proton/antiproton asymmetry plots labelled
   `"H1SIDIS_HighQ2 — Analysis Summary"`. That naming collision is the
   highest-impact downstream confusion risk and is the single strongest
   reason to deprecate the script explicitly rather than leave it silent.

5. **The script defaults `pid-cut 0.1` and `p-max 1.3 GeV`.** Both are
   proton-PID-specific. A naive user reading the docstring line 1
   ("Full analysis for H1SIDIS_HighQ2") and running it on a v4 merged file
   could mistake the resulting asymmetry-vs-p plot for a SIDIS `A_h(p)`
   measurement, which would be physically wrong by ~30–40 percentage points
   in the highest-z bin (compare `proton_HiQ/context-handoff.md` line 30:
   `A_ch ~ 0.3% to ~37%`).

---

## 7. Update needed in `audit-proton-naming-highQ2-sidis.md`

The audit (Section 4.1 row 6d) marks this file as **AMBIGUOUS** and says
"Requires read of the full file body to disambiguate." This investigation
provides that disambiguation:

> **Update row 6d to:** **STALE FORK-LEFTOVER** — the script's body is a
> proton/antiproton asymmetry analyzer using REMOVED-in-v5b trees
> (`ProtonTot`/`ProtonTotGen`) and REMOVED-in-v5b PID branches
> (`DedxRaw`/`LkProton`/`inFiducial`), with a hard-coded
> proton-analysis luminosity (`DATA_LUMI_ALL = 356613.0 nb⁻¹`) and
> proton-specific momentum window. The first line of the docstring claims
> "Full analysis for H1SIDIS_HighQ2" but the body computes
> `A = (N_p/eff_p - N_pbar/eff_pbar) / (sum)`, which is the
> proton/antiproton-analysis observable, not the SIDIS `A_h`. The script is
> not invoked by any production or analysis chain. mtime 2026-05-15.
> Recommendation: deprecate in place (Option A) or move to a quarantine
> subdirectory in the same NAF write window as the rest of the proton-naming
> cleanup. Zero SIDIS or proton breakage if moved or deleted.

---

## 8. Risks, assumptions, gaps

### Risks
- If a future session searches `H1SIDIS_HighQ2/scripts/` for an analyzer and
  picks the file with `_highQ2` in the name (this one), they will get the
  wrong observable. Deprecation marking is the single cheapest intervention
  against this.

### Assumptions
- That `H1BaryonAsym_HighQ2/` is the live proton/antiproton high-Q2 package
  (asserted on the basis of directory presence + script set; the
  authoritative confirmation would be in the proton_HiQ handoff after that
  handoff itself is corrected).
- That no out-of-tree script (outside the BaryonAsym root) invokes the file.
  Not verified globally because of the `find`-on-`/afs`/`/pnfs` SSH rule;
  only the BaryonAsym subtree was greppped.

### Gaps
- The actual analyze-time behavior on a real `proton_highQ2_data_smoke.root`
  file was not tested; the failure modes are inferred from the script body.
- No `git` history is available in the live NAF directories (no `.git/` at
  `H1SIDIS_HighQ2/`), so mtime is the only provenance signal.
- No author/session attribution recovered.

---

## 9. One-paragraph summary

`analyze_proton_highQ2.py` (NAF `/data/dust/.../H1SIDIS_HighQ2/scripts/`,
21,818 bytes, mtime 2026-05-15) is a **stale fork-leftover**: a Python adaptation
of the low-Q2 proton/antiproton analyzer (`analyze_proton_full.py`) dropped into
the SIDIS high-Q2 scripts directory but never wired into any pipeline. It opens
the REMOVED-in-v5b trees `ProtonTot` and `ProtonTotGen`, uses the
REMOVED-in-v5b PID branches `DedxRaw` and `LkProton`, applies the
proton-analysis momentum window `0.3 < P < 1.3 GeV`, hard-codes the
proton-analysis luminosity `356,613 nb⁻¹`, and computes the
proton/antiproton charge asymmetry `A = (N_p/eff_p − N_pbar/eff_pbar) / (sum)`
— **not** any of the SIDIS observables (`A_h`, `M^{h±}`, φ_h moments). It is not
imported, invoked, or referenced by any other script in `H1SIDIS_HighQ2/`,
by `H1BaryonAsym_HighQ2/` (the live high-Q2 proton analysis package), or by
any local SIDIS doc. **Disposition: deprecate in place; no SIDIS or proton
analysis breaks if it is later moved or deleted.**

<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Audit: legacy "proton" naming in the high-Q2 charged-particle SIDIS analysis

**Scope:** documentation-first inventory of every "proton"-named entity used in the
high-Q2 charged-particle SIDIS context (inclusive h+/h- multiplicities, A_h, A_BC,
phi_h moments at Q^2 > 150 GeV^2). The legacy naming is artifact from when the
SIDIS builder was forked off the proton/antiproton asymmetry analysis.

**This audit does NOT rename anything.** It classifies and recommends.

**Read-only audit.** SSH control socket check at task start: PASS
(`Master running, pid=16259`). No NAF writes. No local repo writes other than
this audit file.

**Workspaces audited:**
- `sidis_lowQ2/` -- canonical SIDIS workspace at workspace root (real workspace).
- `H1/sidis_lowQ2/` -- nested vestige directory at workspace root, containing
  only two JSON files (`acceptance_highQ2_charged_mc_prototype.json`,
  `validation_highQ2_mc_species_matching.json`); both appear to be stale,
  partial copies of files that also live in `sidis_lowQ2/docs/data/`. NOT the
  canonical workspace. The user task description used "H1/sidis_lowQ2/" as a
  path prefix following the AGENTS.md naming convention; in this repository
  layout that resolves to `sidis_lowQ2/`. **The audit body refers to the
  canonical `sidis_lowQ2/`.** See Section 6 for the nested vestige.
- NAF: `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/` (read-only ls
  and head; no writes).

---

## 1. Executive summary

### Counts per category

| Category | Total occurrences | LEGACY | TRUE PROTON | AMBIGUOUS | DEFECT (ProtonTot in SIDIS context) |
|----------|---:|---:|---:|---:|---:|
| **C1 SIDIS docs** (md/json/csv under `sidis_lowQ2/docs/` + handoff) | **1943** lines | **~1830** | **~90** | ~5 | **22 lines** (subset of LEGACY/DEFECT) |
| **C2 SIDIS filenames** containing "proton" | **8 files** | 8 (cached/old source) | 0 | 0 | 0 |
| **C3 NAF code root / binary** | 6 inventoried items | 6 | 0 | 0 | n/a |
| **C4 SIDIS analysis scripts** | 5 files, ~17 string-literal hits | 17 | 0 | 0 | 8 (ProtonTot references in scripts) |
| **C5 ProtonTot/ProtonTotGen refs in SIDIS context** | **22 lines** in 8 files | n/a | n/a | n/a | **22 lines (all DEFECT-or-historical)** |
| **C6 Session-produced artifacts** | 23 files inventoried | **20 files inherited proton naming** | n/a | n/a | 3 files retain ProtonTot DEFECT refs |

The 1943 line count for C1 is dominated by per-file path strings in three
JSON/CSV data files (`highQ2_mc_per_file_schema.csv:850`,
`validation_highQ2_mc_schema_full.json:208`,
`validation_highQ2_mc_manifest.json:119`). After collapsing repeated path
strings, the number of *distinct* legacy patterns is approximately
**6 distinct strings repeated thousands of times**:
1. `proton_highQ2_mc_<gen>_<run>_<period>_NNNN.root` (filename pattern)
2. `proton_highQ2_<gen>_<run>_<period>_NNNN.root` (RAPGAP-dual filename pattern)
3. `H1SIDIS_HighQ2/output/mc/<gen>/<run>/proton_highQ2_*.root` (full path)
4. `ProtonTreeBuilderHighQ2.C` and `ProtonTreeBuilderHighQ2` (source/binary names)
5. `ProtonTot`/`ProtonTotGen` (REMOVED v5b tree names appearing in SIDIS docs)
6. `proton-asymmetry pipeline` / `proton-pipeline` (descriptive phrasing)

### Top recommendations

1. **Do NOT physically rename anything on NAF in this session or the next.**
   The NAF artifact rename (Tier 3, Tier 4) breaks the live production pipeline
   and gains no physics. Defer to a fresh production cycle.
2. **Author a single naming-convention alias doc** (`reference-naming-aliases-highQ2-sidis.md`)
   that maps every legacy proton-prefixed artifact to its SIDIS meaning. This
   is the cheapest single intervention with the highest legibility gain.
3. **Tier 1 (safe doc rewrites)** is recommended for execution NOW, restricted
   to descriptive prose (where "proton-pipeline" becomes "SIDIS builder" or
   "v5b SIDIS builder forked from the proton-asymmetry package") but
   PRESERVING all literal NAF-path strings unchanged (those must match the
   on-disk filenames).
4. **The 22 ProtonTot/ProtonTotGen lines (Category 5) are defects regardless
   of the rename policy** -- v5b removed those trees and any SIDIS doc that
   cites them as part of the active schema is misleading. Several are
   *correctly* labelled as REMOVED (e.g., research-highQ2-status-snapshot,
   review-builder-final-readiness, plan-highQ2-sidis-next-steps), but a few
   require clarification.
5. **Session-produced artifacts (Category 6)** are easy targets for Tier 2
   re-emission with naming-convention disclaimers added to the JSON
   `_meta` blocks and Markdown headers; the literal NAF path strings inside
   them MUST be preserved unchanged.

### Top 5 highest-impact LEGACY renames (most-cited or most-confusing)

| Rank | Legacy string | Why high-impact | Recommended treatment |
|---:|---|---|---|
| 1 | `proton_highQ2_mc_*.root` (per-run MC file basename pattern) | Appears in ~2000 lines across 9+ files; is the single most-visible legacy name | DO NOT rename files; add naming-alias doc; preserve literal strings in audit/JSON; rephrase prose around them |
| 2 | `ProtonTreeBuilderHighQ2.C` / `ProtonTreeBuilderHighQ2` (source / binary) | Builder identity; appears in 14+ docs and is the canonical NAF source | DO NOT rename; alias-doc maps `ProtonTreeBuilderHighQ2 -> SIDIS builder (high-Q2 v5b)`; defer physical rename to next builder major revision |
| 3 | `H1SIDIS_HighQ2/output/mc/.../proton_highQ2_*.root` (full NAF path) | Hard-coded in submission scripts AND in every data manifest | DO NOT rename (would break pipeline); alias-doc clarifies the path namespace |
| 4 | `ProtonTot` / `ProtonTotGen` in SIDIS context | Refers to REMOVED v5b trees; their continued presence in docs is a defect | DEFECT FIX: clarify in 8 files that these names refer to the v4/pre-v5b schema only |
| 5 | "proton-pipeline" / "proton-asymmetry pipeline" / "proton/antiproton" describing the SIDIS production | Pure prose ambiguity; readers conflate "SIDIS production happens to share infra" with "this IS the proton asymmetry analysis" | Rephrase to "v5b SIDIS builder (forked from the proton-asymmetry package)" or "shared production infrastructure with proton/antiproton analysis" |

### Count of session-produced artifacts that inherited proton naming

**20 of 23** session-produced files inspected contain at least one legacy
proton-prefixed string (most often the NAF path string for the v5b MC files).
See Section 7 for the full table.

### Recommended tier set to execute now

- **Tier 1 (descriptive prose rewrites only): EXECUTE NOW.**
- **Tier 2 (re-emit session-produced JSON/MD with `_meta` disclaimer and
  prose rewrites): EXECUTE NOW.** Preserve every literal NAF path string.
- **Tier 3 (NAF builder source rename + recompile): DEFER** to next builder
  major revision (v6 or pre-data-production refresh).
- **Tier 4 (NAF output-file rename): REJECT** unless a fresh production cycle
  is being launched anyway.
- **Tier 5 (submission/downstream script patches): DEFER**; tied to Tier 3/4.

### Surprising TRUE PROTON occurrences that should still be cleaned up

1. **`PROTON_MASS = 0.93827` and `M_PROTON` Python constants** in
   `sidis_lowQ2/docs/research/research-optimal-photon-reconstruction.md` lines
   1001, 1044 -- TRUE PROTON (mass of the species), but in context they refer
   to the *beam proton* energy for the gamma*-proton frame boost. The naming
   is fine, but the snippet would be clearer if commented as
   `# beam proton (target) mass for gamma*-N frame`.
2. **`research-tracking-systematics-deep.md` line 1164**:
   `"analysis uses the same builder family (proton_HiQ/) ..."` -- TRUE PROTON
   (refers to the separate proton_HiQ analysis), but the surrounding sentence
   structure makes it easy to mis-read as "uses the proton builder". Light
   prose tweak recommended.
3. **`reference-LAr-electron-scale-highQ2-sidis.md`** lists `proton_HiQ` and
   `lambda_HiQ` *dozens* of times (lines 24-466) referencing the separate
   high-Q2 analyses. All TRUE PROTON -- but the doc could benefit from a
   single up-front clarification box: "proton_HiQ in this document means the
   separate proton/antiproton asymmetry analysis, NOT the SIDIS analysis."

---

## 2. Category 1 -- SIDIS docs referencing proton

Below is the categorised inventory. Per-file counts of strict legacy patterns
(`proton_highQ2|ProtonTreeBuilder*`) are summarised first; individual lines
are listed for the small files and for representative samples of the large
files.

### 2.1 Per-file counts of strict legacy patterns (`proton_highQ2*` OR `ProtonTreeBuilder*`)

| File | Strict-legacy hits | Notes |
|---|---:|---|
| `sidis_lowQ2/docs/data/highQ2_mc_per_file_schema.csv` | **850** | Per-file CSV; one line per `proton_highQ2_mc_*.root` NAF path. LEGACY in the *naming convention* sense; literal strings must be preserved. |
| `sidis_lowQ2/docs/data/validation_highQ2_mc_schema_full.json` | **208** | Per-file schema dump; one entry per NAF path. Same pattern as above. |
| `sidis_lowQ2/docs/data/validation_highQ2_mc_manifest.json` | **119** | Manifest of 1210 v5b MC files; documents the dual `proton_highQ2_*` / `proton_highQ2_mc_*` naming pattern. Also contains the only legacy mentions of `ProtonTot=56`, `ProtonTotGen=20` as *historical* schema (PROPERLY LABELED as "merged proton-asym v4 leftover"). |
| `sidis_lowQ2/docs/data/validation_highQ2_mc_species_matching.json` | **(50 total `proton` hits, of which 21 strict-legacy)** | Mixed: `proton_highQ2_*` paths (LEGACY) plus `beam_proton`, `beam_proton_n`, `beam_proton_frac_of_all_gen`, `beam_proton_contamination` (all TRUE PROTON -- beam-proton physics). |
| `sidis_lowQ2/docs/data/validation_highQ2_q2es_vs_q2e.json` | **10** | 10 per-file NAF path strings (LEGACY). |
| `sidis_lowQ2/docs/data/n_dis_prototype_highQ2_mc.json` | **8** | NAF path strings (LEGACY). |
| `sidis_lowQ2/docs/data/grl_manifest_highQ2.json` | **1** + 8 prose mentions | One legacy path mention (`proton_highQ2_d07_0000.root`, LEGACY); 8 references to the parallel `proton_HiQ` analysis (TRUE PROTON). |
| `sidis_lowQ2/docs/data/validation_highQ2_data_availability.json` | **4** | Mixed: `proton_highQ2_data_smoke.root` subdir name (LEGACY, but matches on-disk subdir); references to "proton-asymmetry schema" (TRUE/DESCRIPTIVE). |
| `sidis_lowQ2/docs/data/validation_highQ2_mc_manifest.CORRECTIONS.md` | **5** | Path strings (LEGACY). |
| `sidis_lowQ2/docs/research/research-highQ2-sidis-extension.md` | **7 strict, 41 total** | The single highest density of mixed TRUE/LEGACY mentions. See 2.2. |
| `sidis_lowQ2/docs/research/research-sidis-systematics-framework.md` | **11** | Mostly `ProtonTreeBuilderHighQ2.C` source citations (LEGACY). One `merged_proton_highQ2_mc_rapgap31.root` filename (LEGACY). |
| `sidis_lowQ2/docs/research/research-dis-kinematic-methods.md` | **14** | Source-file citations of `ProtonTreeBuilderHighQ2.C` (LEGACY). |
| `sidis_lowQ2/docs/research/research-crad-for-charged-sidis.md` | **10** | Source-file citations of `ProtonTreeBuilder_lowQ2.C` (LEGACY low-Q2 sibling). |
| `sidis_lowQ2/docs/research/research-highQ2-status-snapshot-2026-05-19.md` | **5** | Includes `ProtonTreeBuilderHighQ2` binary path (LEGACY) and `proton_highQ2_data_smoke.root` (LEGACY). Also correctly notes ProtonTot/ProtonTotGen as REMOVED in v5b. |
| `sidis_lowQ2/docs/research/research-genreco-matching-critique.md` | **4** | `ProtonTreeBuilderHighQ2.C` source citations (LEGACY). |
| `sidis_lowQ2/docs/research/research-v0-daughter-detection-api.md` | **7** | `ProtonTreeBuilder_lowQ2` (LEGACY, low-Q2 sibling) but also several TRUE PROTON refs (Lambda decay daughter proton, V0-tagger `GetProton()` API). |
| `sidis_lowQ2/docs/references/reference-gen-reco-matching-charged-sidis.md` | **9** | All `ProtonTreeBuilderHighQ2.C` source citations (LEGACY). |
| `sidis_lowQ2/docs/references/reference-nuclia-correction-highQ2-sidis.md` | **12** | `ProtonTreeBuilderHighQ2.C` source citations + one `proton_highQ2_mc_*.root` file path (LEGACY). |
| `sidis_lowQ2/docs/references/reference-trigger-semantics-highQ2-sidis.md` | **5** | NAF paths + `src/ProtonTreeBuilderHighQ2.C` citations (LEGACY). |
| `sidis_lowQ2/docs/references/reference-LAr-electron-scale-highQ2-sidis.md` | **3 strict, 64 total** | The strict-3 are `ProtonTreeBuilderHighQ2_v2.C` snapshot references and one `ProtonTreeBuilderHighQ2`. The other 60+ are TRUE PROTON references to the separate `proton_HiQ` analysis (which the SIDIS analysis cannot inherit LAr-systematics infrastructure from). |
| `sidis_lowQ2/docs/references/reference-highQ2-grl-manifest.md` | **1** | One `proton_highQ2_d07_NNNN.root` (LEGACY, refers to file basename pattern). |
| `sidis_lowQ2/docs/plans/plan-highQ2-sidis-next-steps.md` | **5 strict, 22 total** | Multiple `proton_highQ2_mc_*` dedup rule citations (LEGACY). |
| `sidis_lowQ2/docs/plans/plan-builder-update-combined-tracks.md` | **4** | `ProtonTreeBuilder_lowQ2.C` (LEGACY low-Q2 sibling). |
| `sidis_lowQ2/docs/plans/plan-tmd-sidis.md` | **1** | `ProtonTreeBuilder_lowQ2.C` (LEGACY). |
| `sidis_lowQ2/docs/reviews/review-builder-final-readiness.md` | **4 strict, 22 total** | LEGACY low-Q2 builder citations + ProtonTot legacy-tree mentions (correctly flagged as REMOVED). |
| `sidis_lowQ2/docs/reviews/review-mc-physics-matching-pool.md` | **4 strict, 36 total** | Mixed builder citations + TRUE PROTON species refs. |
| `sidis_lowQ2/docs/results/n_dis_prototype_highQ2_mc.md` | **2** | NAF path strings (LEGACY). |
| `sidis_lowQ2/docs/results/acceptance_highQ2_charged_mc_prototype.md` | **0 strict** | Clean of strict legacy patterns. |
| `sidis_lowQ2/docs/results/binning_study_highQ2.md` | **0 strict** | Clean. |
| `sidis_lowQ2/docs/handoffs/memo-mc-truth-policy-charged-sidis.md` | **6 strict, 23 total** | `ProtonTreeBuilder_lowQ2.C` source-line citations (LEGACY) + many `proton`/`pbar`/`Sigma0->p` TRUE PROTON physics refs (PS classification semantics for the species proton). |
| `sidis_lowQ2/context-handoff.md` | **2 strict, 9 total** | LEGACY: `ProtonTreeBuilder_lowQ2.C` cached source reference. TRUE PROTON: beam proton contamination, proton/antiproton momentum window history. |

### 2.2 Representative line-by-line classifications

Sampled lines from the highest-density files; classifications use the rubric
from the task brief.

**`sidis_lowQ2/context-handoff.md`** (all 9 hits, all classified):

| Line | Snippet | Classification |
|---:|---|---|
| 60 | `The P momentum window (0.3-3.0 GeV) from the proton/antiproton analysis has been` | **TRUE PROTON** (legitimate provenance reference) |
| 173 | `... migration studies. The beam proton ...` | **TRUE PROTON** (beam proton physics) |
| 247 | `v3.1 removes the ProtonTot and ProtonTotGen trees and 10 dE/dx PID` | **DEFECT-adjacent / HISTORICAL** -- correctly says "removes", refers to old schema |
| 248 | `branches from ChargedTot (DedxRaw, Dedx{Pion,Kaon,Proton},` | **TRUE PROTON** (dE/dx hypothesis name; species set) |
| 249 | `Lk{Pion,Kaon,Proton}, Nlh{Pion,Kaon,Proton}); NHitDedx is retained as` | **TRUE PROTON** |
| 261 | `.../H1SIDIS_LowQ2/src/ProtonTreeBuilder_lowQ2.C` | **LEGACY** (source-file name) |
| 308 | `source_cache/ProtonTreeBuilder_lowQ2.C` | **LEGACY** |
| 375 | `### Beam Proton Contamination` | **TRUE PROTON** |
| 377 | `The broad gen fiducial ... admits beam protons` | **TRUE PROTON** |

**`sidis_lowQ2/docs/data/validation_highQ2_data_availability.json`** (the
key data-availability summary):

| Line | Snippet | Classification |
|---:|---|---|
| 6 | `summary: ... contain only the proton-asymmetry schema (ProtonTot, 56 branches) ...` | **DESCRIPTIVE** (correctly distinguishes proton-asym schema from SIDIS schema) |
| 11 | `subdirs_present: [..., "proton_highQ2_data_smoke.root"]` | **LEGACY** (subdir literal name, matches on-disk) |
| 12 | `schema_observed: ProtonTot=56 branches (NO ChargedTot/DisTot)` | **DESCRIPTIVE** (correctly identifies non-SIDIS schema) |
| 16 | `files: [..., "highQ2_noproton_mc.root", ...]` | **LEGACY** (smoke file name) |
| 46 | `file: .../H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` | **LEGACY** |
| 52 | `files_sampled: [evls_d05_0000_proton.root, ...]` | **LEGACY** (evls per-file naming) |
| 76, 78 | `condor submit-file counts ... H1SIDIS_HighQ2/condor/proton/subs/data/...` | **LEGACY** (condor subdir naming) |
| 86 | `Submit v5b HighQ2 data campaign (H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C) ...` | **LEGACY** |

**`sidis_lowQ2/docs/research/research-highQ2-sidis-extension.md`** (lines
1-65, the head of the file):

| Line | Snippet | Classification |
|---:|---|---|
| 22 | `H1/sidis_lowQ2/source_cache/ProtonTreeBuilderHighQ2.C` | **LEGACY** (cached source filename) |
| 29-36 | refs to `H1/proton_HiQ/...` | **TRUE PROTON** (separate analysis) |
| 58-60 | `Sources: H1/proton_HiQ/docs/context-handoff-highQ2-proton.md ... ProtonTreeBuilderHighQ2.C header lines 16-23` | **TRUE PROTON** (proton_HiQ ref) + **LEGACY** (builder source-file name) |
| 99 | `Track-level cuts from ProtonTreeBuilderHighQ2.C (lines 38-43)` | **LEGACY** |
| 115 | `see research-highQ2-proton-asymmetry.md Section 5.x` | **TRUE PROTON** |
| 133-148 | "Package: H1ProtonAsymHighQ2", "Binary: bin/.../ProtonTreeBuilderHighQ2", etc. | **LEGACY** (the NAF package was renamed to H1SIDIS_HighQ2; this doc still calls it H1ProtonAsymHighQ2 -- itself a documentation drift bug; see Note below) |
| 134-135 | `The production was driven by the proton/antiproton analysis, which writes both ProtonTot/ProtonTotGen ...` | **DESCRIPTIVE** (historical; correctly noted in v5b status snapshot as REMOVED) |
| 164 | `they were produced by the lambda_HiQ pipeline. The proton/SIDIS pipeline` | **DESCRIPTIVE** but ambiguous: "proton/SIDIS pipeline" hides whether SIDIS or asymmetry pipeline meant. Recommend clarification. |
| 170-171, 193, 212 | "Re-pointing proton steering files...", "proton analysis (builder ...)", "charged gen SIDIS uses the same boost vectors as the proton gen" | **MIXED** -- references to the proton-asymmetry sibling analysis as the source of infrastructure decisions. Largely TRUE PROTON but worth a clarifying note. |
| 274 | `Add ParticleSource_cg to ChargedTotGen | trivial | low | Already classified in proton gen loop` | **TRUE PROTON** (the proton gen loop in the v4 builder) |
| 322-324 | `H1ProtonAsymHighQ2/scripts/submit_full_production.py`, `H1ProtonAsym/output/...`, `H1ProtonAsymHighQ2/output/...` | **DOCUMENTATION-DRIFT** -- this entire section refers to the package by its pre-fork name H1ProtonAsymHighQ2; the actual current NAF root is `H1SIDIS_HighQ2`. **Defect to fix.** |
| 483-488 | discussion of "(A) live in H1ProtonAsymHighQ2/" recommendation | **STALE** (recommendation was adopted: package was renamed to H1SIDIS_HighQ2) |

**Note on a NEW defect uncovered:** the doc
`research-highQ2-sidis-extension.md` consistently writes
`H1ProtonAsymHighQ2/` but the NAF directory is actually `H1SIDIS_HighQ2/`
(verified by `ssh ... ls /data/dust/.../BaryonAsym/H1SIDIS_HighQ2/src/`). This
is **a documentation defect independent of the proton -> SIDIS rename
question**: the package rename already happened, but this research doc was
not updated. Flag for Tier 1 prose fix.

### 2.3 Bare "proton" / "Proton" mentions to FLAG as TRUE PROTON

These appear in SIDIS docs but legitimately refer to:

| Pattern | Locations | Classification |
|---|---|---|
| "beam proton", "beam proton remnant", "Beam Proton Contamination" | handoff line 173/375/377; species_matching JSON `beam_proton_*` | **TRUE PROTON** (physical beam proton) |
| "proton" in species set {pi, K, p} or PDG 2212 | many docs (memo-mc-truth-policy, research-h1-charged-particle, etc.) | **TRUE PROTON** |
| "Lambda -> p pi-", "p (proton) from Lambda decay" | tracking-systematics-deep, memo-mc-truth-policy | **TRUE PROTON** (decay product) |
| `M_PROTON`, `PROTON_MASS`, `BeamProtP` | research-optimal-photon-reconstruction lines 1001/1044; plan-builder-update-combined-tracks line 728 | **TRUE PROTON** (mass constants / branch name) |
| `H1/proton/`, `H1/proton_HiQ/`, `H1/lambda_HiQ/` | dozens of refs in research-sidis-systematics-literature, reference-LAr-electron-scale, research-highQ2-sidis-extension | **TRUE PROTON** (separate analyses) |
| `proton/antiproton`, `Protons vs antiprotons` | research-tracking-systematics-deep lines 6-9, 200-204, 331-336 | **TRUE PROTON** (physics of pp vs pbar-p cross-sections) |
| "GetProton()" / "GetPion()" V0 daughter API | research-v0-daughter-detection-api line 195 | **TRUE PROTON** (H1OO API name) |
| `DedxProton`, `LkProton`, `NlhProton` (PID branch names) | validation_highQ2_mc_manifest.json lines 77/81/84 | **TRUE PROTON** (proton-hypothesis dE/dx PID branches; these are REMOVED in v5b per the handoff but their names are inherently proton-species) |

---

## 3. Category 2 -- SIDIS artifact filenames containing "proton"

`find sidis_lowQ2/ -iname "*proton*"` (excluding `__pycache__`):

| Path | Type | Size | Classification | Note |
|---|---|---:|---|---|
| `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` | cached source | 59 KB | **LEGACY** | The low-Q2 builder source (cached on May 14). NAF lives at `H1SIDIS_LowQ2/src/ProtonTreeBuilder.C`. |
| `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2_v6_draft.C` | cached source | 70 KB | **LEGACY** | v6 draft of the low-Q2 builder. |
| `sidis_lowQ2/source_cache/ProtonTreeBuilderHighQ2.C` | cached source | 63 KB | **LEGACY** | The high-Q2 builder source (cached on May 14; the live NAF copy is `H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C`). |
| `sidis_lowQ2/source_cache/ProtonUtils_lowQ2.h` | cached header | 5 KB | **LEGACY** | Utility namespace; defines `ProtonUtil::isChargedHadron`, `ProtonUtil::classifySourceGTR`, `ProtonUtil::getMassForPDG`, etc. *The namespace name is itself a legacy*. |
| `sidis_lowQ2/source_cache/ProtonUtils_lowQ2_v6_draft.h` | cached header | 10 KB | **LEGACY** | v6 draft. |
| `sidis_lowQ2/source_cache/ProtonUtils_highQ2.h` | cached header | 5 KB | **LEGACY** | High-Q2 variant of the utility namespace. |
| `sidis_lowQ2/old/source_cache_bak/ProtonTreeBuilder_lowQ2.C.bak` | backup | 55 KB | **LEGACY** | Pre-v5b backup. Under `old/` -- low priority. |
| `sidis_lowQ2/old/source_cache_bak/ProtonTreeBuilderHighQ2.C.bak` | backup | 47 KB | **LEGACY** | Pre-v5b backup. Under `old/`. |

8 LEGACY files total. None TRUE PROTON. All under `source_cache/` or
`old/source_cache_bak/`. None are referenced by live analysis scripts as
import targets (the live scripts read the NAF source via SSH).

**Recommendation:** rename the LIVE `source_cache/` files only as part of
Tier 3 (when the NAF source is also renamed); the cached basenames must
match the NAF source basenames or grep-back-to-source workflows break. The
`old/source_cache_bak/` copies can stay as-is (they are historical
backups by definition).

---

## 4. Category 3 -- NAF code root and binary naming

`ssh -O check naf-hone.desy.de` → PASS. All NAF interactions were
read-only (`ls`, `head`, `grep`).

### 4.1 Inventory and classification

| # | NAF item | Type | Classification | Rebuild required? | Resubmit required? | Downstream script breakage if renamed? |
|---:|---|---|---|---|---|---|
| 1 | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` | builder source (.C, 1279+ lines per docs) | **LEGACY** (file name says proton; physics is SIDIS per header lines 3-16: "High-Q2 standalone primary-vertex charged-hadron SIDIS analysis ... forked from the low-Q2 charged-hadron builder, which itself originated as a proton/antiproton analysis package") | YES if renamed (must `make bin` to relink) | NO (binary continues to work until next `make bin`) | YES (the submission script `submit_full_production.py` references the binary by basename `ProtonTreeBuilderHighQ2`) |
| 2 | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/PhysicsConstants.h` | constants header | **TRUE PROTON FILE NAME** (PhysicsConstants is generic) but **contains LEGACY proton-specific constants** per `review-builder-final-readiness.md` Section D1 (lines 162-178): `PROTON_NHIT_DEDX_MIN`, `PROTON_P_MAX_TREE`, `PROTON_ETA_MAX_TREE`, `PROTON_PT_MIN_TREE`, `PROTON_MATCH_ANGLE`, `PROTON_P_FIDUCIAL`, `PROTON_ETA_FIDUCIAL` -- none of which are referenced by the SIDIS builder. | NO (header name is fine) | NO | NO (header rename would break `#include`, but constants-cleanup is independent and already planned in builder review) |
| 3 | `/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` | compiled binary | **LEGACY** (same name as source) | n/a (binary name is determined by source basename via `make bin`) | YES (binary path is referenced by every submission script) | YES (all submission scripts hard-code the binary name) |
| 4 | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/<run>/proton_highQ2_mc_*.root` | per-run MC output ROOT files (1210 files, 122 in `7071/` alone, May-18 build) | **LEGACY** (naming convention inherited from proton-pipeline submitter; physics is SIDIS) | NO | YES if renamed (must `mv` 1210 files + rerun merge) | YES (every `validate_outputs_highQ2.py` glob, `merge_mc_highQ2.sh` hadd, `analyze_sidis_*.py` script, and the 9 local docs in `sidis_lowQ2/docs/data/` would need to be updated) |
| 5 | Dual-naming on NAF: `7071/` contains 122 files split between `proton_highQ2_mc_rapgap31_*` and `proton_highQ2_rapgap31_*` | dual-naming artifact | **LEGACY × 2** (both basename patterns are inherited proton names). The dedup rule `keep only _mc_` is documented in `plan-highQ2-sidis-next-steps.md` line 105 and `validation_highQ2_mc_manifest.json` line 30. | NO | n/a (just a glob-rule) | n/a (the dedup is already implemented) |
| 6 | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/submit_full_production.py` | submission script | **LEGACY** (hardcoded `PROTON_BIN`, `PROTON_PKG`, `PROTON_STEER_BASE`, `PROTON_WRAPPERS`, `PROTON_SUBS`, `create_proton_data_job`, `create_proton_mc_job`, `submit_proton_stage`, `condor_logs/proton_highQ2`, output stem `proton_*`) | NO | NO (existing submissions stand) | YES if renamed (would break in-flight Condor wrappers and the recovery harnesses) |
| 6b | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/submit_proton_highQ2_batch.sh` | bash batch submitter | **LEGACY** (filename + `BIN=.../ProtonTreeBuilderHighQ2`, glob `highQ2_proton_data_*.steer`) | NO | NO | YES if renamed |
| 6c | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/merge_mc_highQ2.sh` | bash merge | **LEGACY** (output name `merged_proton_highQ2_mc_*.root`) | NO | YES if renamed (downstream analysis scripts glob `merged_proton_highQ2_mc_*.root`) | YES |
| 6d | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/analyze_proton_highQ2.py` | analysis driver script | **LEGACY** (filename + docstring describes `dsigma_p`, `dsigma_pbar`, `A = ...`; the doctring is *currently* about the proton-asymmetry pipeline though the surrounding SIDIS_HighQ2 directory is the SIDIS one). **AMBIGUOUS** -- the script's *content* may still be the proton-asymmetry analyzer, or it may be a stale stub. Requires read of the full file body to disambiguate. | n/a | n/a | n/a |
| 7 | Internal hard-coded paths in `submit_full_production.py`: `LOG_BASE = "condor_logs/proton_highQ2"`, output stem `proton_*` (`stem.replace("evls_", "proton_")` lines 550, 600) | hard-coded literals | **LEGACY** | NO | YES if renamed | YES (output filenames are derived from this string concat) |

### 4.2 Cost analysis: physical-rename vs alias-doc

**Physical-rename cost (Tier 3 + Tier 4 + Tier 5):**
- 1210 per-run MC ROOT files renamed.
- 1 binary recompile (`make bin`).
- 1 source file rename.
- ~5 submission/merge/analysis scripts patched.
- All Condor wrappers regenerated for any future submission.
- ALL 9 local docs in `sidis_lowQ2/docs/data/` (CSV + JSONs) re-emitted with
  new paths.
- ALL 23 session-produced artifacts re-emitted.
- Total LoC change: ~3000 lines of doc/JSON, ~500 lines of script, 1
  recompile, 1210 file moves.
- **Risk:** breaks any in-flight Condor recovery harness; breaks any
  third-party reference (the lambda_HiQ analysis cross-references the
  SIDIS pipeline by these paths).

**Alias-doc cost (recommended alternative):**
- 1 new file: `sidis_lowQ2/docs/references/reference-naming-aliases-highQ2-sidis.md`
- Lists each legacy artifact -> SIDIS meaning.
- ~200 lines.
- Zero NAF impact, zero production impact, zero breakage.
- Solves the *legibility* problem (the underlying concern in the audit
  request) without solving the *legacy-name* problem.

### 4.3 Submission script proton-prefix occurrences (from NAF `grep`)

`submit_full_production.py`:

| Line | Snippet |
|---:|---|
| 8 | `Stage 2 (proton): Run H1SIDIS_HighQ2/ProtonTreeBuilderHighQ2 on the` |
| 38 | `# Stage 2 binary (ProtonTreeBuilderHighQ2)` |
| 39 | `PROTON_BIN = BARYONASYM_ROOT / "bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2"` |
| 45 | `PROTON_PKG = BARYONASYM_ROOT / "H1SIDIS_HighQ2"` |
| 51 | `LOG_BASE = BARYONASYM_ROOT / "condor_logs/proton_highQ2"` |
| 63 | `PROTON_OUT_BASE = PROTON_PKG / "output"` |
| 66 | `EVLS_STEER_BASE = PROTON_PKG / "steering/evls"` |
| 67 | `PROTON_STEER_BASE = PROTON_PKG / "steering/proton"` |
| 70-73 | `EVLS_WRAPPERS`, `EVLS_SUBS`, `PROTON_WRAPPERS`, `PROTON_SUBS` definitions |
| 194 | `PROTON_WRAPPER_TEMPLATE = """\` |
| 246 | `PROTON_DATA_STEER_TEMPLATE = """\` |
| 254 | `PROTON_MC_STEER_TEMPLATE = """\` |
| 413, 472 | `project=PROTON_PKG,` |
| 546-589 | `def create_proton_data_job(year_short, evls_file): ... label = stem.replace("evls_", "proton_") ...` |
| 596-644 | `def create_proton_mc_job(...): ... label = stem.replace("evls_", "proton_") ...` |
| 651, 813 | `def submit_proton_stage(...)`, `cluster_ids = submit_proton_stage(...)` |
| 668 | `sub_path, label = create_proton_data_job(year_short, evls_file)` |
| 703 | `sub_path, label = create_proton_mc_job(...)` |
| 833 | `print(f" Proton output: {PROTON_OUT_BASE}/")` |

---

## 5. Category 4 -- Local analysis scripts with proton-prefix string literals

Local `sidis_lowQ2/scripts/`:

| File:line | Snippet | Classification | Patch needed if rename? |
|---|---|---|---|
| `plot_smoke_qa.py:3` | `ROOT file (low-Q2 or high-Q2 ProtonTreeBuilder output).` | LEGACY (docstring) | NO if Tier 1; YES if Tier 3 |
| `produce_shifted_mc.py:10` | `plus copies of all other trees (ChargedTotGen, ProtonTot, ProtonTotGen,` | **DEFECT** (refers to v5b-removed trees) + LEGACY | YES |
| `produce_shifted_mc.py:40` | `Generator-level trees (ChargedTotGen, ProtonTotGen) are NEVER shifted:` | **DEFECT** | YES |
| `produce_shifted_mc.py:41` | `they represent truth.  The reco proton tree (ProtonTot) is also` | **DEFECT** | YES |
| `produce_shifted_mc.py:43` | `future analysis requires shifted ProtonTot the same shift functions` | **DEFECT** | YES |
| `produce_shifted_mc.py:109` | `TREES_PASSTHROUGH = ('ChargedTotGen', 'ProtonTot', 'ProtonTotGen', 'DisTot')` | **DEFECT + ACTIVE CODE PATH** (will silently no-op on v5b inputs because the trees do not exist) | **YES, urgent** |
| `produce_shifted_mc.py:124` | `# in ProtonTreeBuilder_lowQ2.C (ProtonUtil::getMassForPDG).` | LEGACY (comment) | NO if Tier 1 |
| `produce_shifted_mc.py:234` | `Mirrors the C++ logic in ProtonTreeBuilder_lowQ2.C lines 939-1175.` | LEGACY (comment) | NO |
| `produce_shifted_mc.py:309` | `electron angle.  Builder reference: ProtonTreeBuilder_lowQ2.C lines` | LEGACY (comment) | NO |
| `produce_shifted_mc.C:7` | `// (ChargedTotGen, ProtonTot, ProtonTotGen, DisTot) are cloned unchanged` | **DEFECT** | YES |
| `produce_shifted_mc.C:137-138` | `for (const char* name : {"ChargedTotGen", "ProtonTot", "ProtonTotGen", "DisTot"})` | **DEFECT + ACTIVE CODE PATH** | **YES, urgent** |
| `merge_and_validate.py:48` | `# Expected schema (post-reprocess, low-Q2 ProtonTreeBuilder)` | LEGACY | NO |
| `merge_and_validate.py:51` | `EXPECTED_RECO_BRANCHES = 82    # ChargedTot (counted from ProtonTreeBuilder_lowQ2.C)` | LEGACY + STALE (live count is 78 per builder review) | YES (defect; already on Tier-1 plan in `review-builder-final-readiness.md`) |
| `merge_and_validate.py:52` | `EXPECTED_GEN_BRANCHES  = 35    # ChargedTotGen (counted from ProtonTreeBuilder_lowQ2.C)` | LEGACY + STALE (live count is 44) | YES |
| `merge_and_validate.py:75` | `'ProtonTot', 'ProtonTotGen', 'DisTot']` (as part of `EXPECTED_TREES`) | **DEFECT + ACTIVE CODE PATH** | **YES, urgent** |
| `merge_and_validate.py:415` | `# Some trees (ProtonTot, etc.) may legitimately be absent` | **DEFECT** (assumption was true for v4, false for v5b) | YES |
| `merge_and_validate.py:637` | `for tname in ['ProtonTot', 'ProtonTotGen', 'DisTot']:` | **DEFECT + ACTIVE CODE PATH** | **YES, urgent** |
| `validate_v3_output.py:3` | `SIDIS tree outputs produced by the cleaned ProtonTreeBuilder.` | LEGACY (docstring) | NO |
| `validate_v3_output.py:5` | `The cleaned builders drop the ProtonTot tree and the per-track PID` | DESCRIPTIVE (correctly says "drop") | NO |
| `validate_v3_output.py:48` | `# ProtonTot must NOT exist in either variant after cleanup.` | **CORRECT** (this is a positive defect guard) | NO |
| `validate_v3_output.py:64` | `FORBIDDEN_TREES  = ("ProtonTot",)` | **CORRECT** (positive guard against legacy schema) | NO |

**Total:** 17 LEGACY hits (8 of which are DEFECTS, of which 4 are
"active code paths" -- they will mis-behave on v5b inputs).

---

## 6. Category 5 -- `ProtonTot`/`ProtonTotGen` references in SIDIS context

All 22 lines in 8 files. Already enumerated in `/tmp/protontot.txt`; the full
list:

| File:line | Snippet | Classification |
|---|---|---|
| `research-highQ2-sidis-extension.md:135` | "...analysis, which writes both ProtonTot/ProtonTotGen and..." | **HISTORICAL** (about v4) -- prose correctly says "writes". OK with clarifying tag "(v4; removed in v5b)" |
| `research-highQ2-status-snapshot-2026-05-19.md:90` | `| ProtonTot / ProtonTotGen | REMOVED | REMOVED |` | **CORRECT** (status table says REMOVED) |
| `research-highQ2-status-snapshot-2026-05-19.md:104` | `v5b branches removed: ProtonTot/ProtonTotGen trees ...` | **CORRECT** |
| `research-background-sources-migration.md:529` | "Per-run file is the v3 builder output before proton-tree removal (still contains ProtonTot)" | **CORRECT** (says "before" / "still") |
| `plan-highQ2-sidis-next-steps.md:104` | "...wrong-schema v4 carryover (ChargedTot=82, ChargedTotGen=41, no DisTot, ProtonTot present)..." | **CORRECT** (labels as wrong-schema carryover) |
| `reference-highQ2-grl-manifest.md:124` | "ProtonTot=56 branches, NO ChargedTot/DisTot. These cannot..." | **CORRECT** (in context of legacy `/data/06/`, `/data/07/` subdir contents that are non-SIDIS schema) |
| `grl_manifest_highQ2.json:75` | "legacy_pre_v5b_subdirs: ... proton-asymmetry-schema (ProtonTot=56 branches, NO ChargedTot/DisTot)..." | **CORRECT** |
| `validation_highQ2_data_availability.json:6, 12` | "... ProtonTot, 56 branches ... NO ChargedTot/DisTot" | **CORRECT** (data-availability summary distinguishing schemas) |
| `validation_highQ2_mc_manifest.json:39-40, 262-263, 271-272, 277-278, 286-287` | Lists `ProtonTot: 56`, `ProtonTotGen: 20` as branches in the *merged-file v4 leftover* schema; clearly labelled `"merged_proton-asym-pipeline (v4) product, not a SIDIS v5b product"` (line 101) | **CORRECT** (manifest is auditing v4 leftover files, not claiming v5b carries them) |
| `review-builder-final-readiness.md:123, 264` | "still hard-codes ProtonTot/ProtonTotGen in EXPECTED_TREES (line 74-75), which the builder no longer produces" | **CORRECT** (positively flags merge_and_validate.py defect) |
| `context-handoff.md:247` | "v3.1 removes the ProtonTot and ProtonTotGen trees..." | **CORRECT** |

**Verdict:** all 22 lines correctly contextualize ProtonTot/ProtonTotGen
as historical or as positive guards. **Zero are misleading SIDIS-context
claims that v5b carries those trees.** However, **8 lines in active analysis
scripts** (Section 5 / Category 4) DO treat the trees as still-present,
which IS a defect -- but that is a script bug, not a doc-text bug.

---

## 7. Category 6 -- Session-produced artifacts

The user's task references 23 specific files. **Inventory and verification:**

### 7.1 `sidis_lowQ2/docs/data/`

| File | Exists? | proton hits | Inherited naming | Notes |
|---|---|---:|---|---|
| `validation_highQ2_mc_manifest.json` | YES | 119 strict + 20 prose | YES (heavy) | Path strings + ProtonTot historical labels (correct) |
| `validation_highQ2_data_availability.json` | YES | 11 | YES | proton-asym schema descriptive labels (correct) |
| `validation_highQ2_mc_schema_full.json` | YES | 208 | YES (heavy) | Per-file path strings only |
| `highQ2_mc_per_file_schema.csv` | YES | 850 | YES (heaviest) | Per-file CSV; one line per NAF path |
| `grl_manifest_highQ2.json` | YES | 10 | MIXED | 1 legacy filename + 9 proton_HiQ TRUE PROTON refs |
| `validation_highQ2_mc_species_matching.json` | YES | 50 | MIXED | 21 LEGACY paths + 29 `beam_proton_*` TRUE PROTON physics |
| `validation_highQ2_q2es_vs_q2e.json` | YES | 10 | YES | Path strings |
| `n_dis_prototype_highQ2_mc.json` | YES | 8 | YES | Path strings |
| `binning_study_highQ2.json` | YES | 0 | NO | **Clean.** |
| `acceptance_highQ2_charged_mc_prototype.json` | YES | 0 | NO | **Clean.** |
| `validation_highQ2_mc_manifest.CORRECTIONS.md` | YES | 5 | YES | Path strings + builder-source ref |

**Note:** the two "clean" JSONs (`binning_study_highQ2.json`,
`acceptance_highQ2_charged_mc_prototype.json`) appear clean *as JSON content*
because they do not embed NAF paths. They are derived from the upstream MC
files, however, so they inherit the LEGACY-named provenance implicitly.

### 7.2 `sidis_lowQ2/docs/references/`

| File | Exists? | proton hits | Inherited naming |
|---|---|---:|---|
| `reference-highQ2-grl-manifest.md` | YES | 11 (1 strict) | YES |
| `reference-nuclia-correction-highQ2-sidis.md` | YES | 56 (12 strict) | YES (heavy) |
| `reference-LAr-electron-scale-highQ2-sidis.md` | YES | 64 (3 strict) | MIXED (60+ TRUE PROTON to proton_HiQ analysis; 3 LEGACY builder refs) |
| `reference-ABC-scope-and-blinding-highQ2-sidis.md` | YES | 0 | NO (**clean**) |
| `reference-trigger-semantics-highQ2-sidis.md` | YES | 5 | YES |
| `reference-gen-reco-matching-charged-sidis.md` | YES | 22 (9 strict) | YES |

### 7.3 `sidis_lowQ2/docs/results/`

| File | Exists? | proton hits | Inherited naming |
|---|---|---:|---|
| `n_dis_prototype_highQ2_mc.md` | YES | 2 strict | YES |
| `binning_study_highQ2.md` | YES | 0 | NO (**clean**) |
| `acceptance_highQ2_charged_mc_prototype.md` | YES | 0 strict | NO (clean of legacy patterns; may contain TRUE PROTON physics refs) |

### 7.4 `sidis_lowQ2/docs/reviews/`

(Session-produced files inventoried; this audit itself will be the 7th.)

| File | proton hits | Inherited naming |
|---|---:|---|
| `critique-phih-convention-freeze-highQ2-sidis.md` | 7 | YES (low) |
| `critic-sidis-systematics-distilled.md` | 7 | YES (low) |
| `critique-plan-highQ2-sidis-next-steps-r1.md` | 0 | NO |
| `critique-systematics-implementation.md` | 13 | YES |
| `review-builder-final-readiness.md` | 22 | YES (heavy; mostly DEFECT-flagging) |
| `review-mc-physics-matching-pool.md` | 36 | YES (mostly TRUE PROTON species refs) |

### 7.5 Summary

- **23 files inspected.**
- **20 contain at least one proton-prefixed string** (LEGACY or TRUE PROTON).
- **3 are clean** of strict legacy patterns: `binning_study_highQ2.json`,
  `binning_study_highQ2.md`, `acceptance_highQ2_charged_mc_prototype.json`,
  `acceptance_highQ2_charged_mc_prototype.md`,
  `reference-ABC-scope-and-blinding-highQ2-sidis.md`,
  `critique-plan-highQ2-sidis-next-steps-r1.md` (count = 6 if we count
  "strict-legacy-clean" rather than "all-proton-clean"; the 20/3 split is for
  ANY proton-prefixed string).
- **Inherited naming severity:**
  - heavy (>100 hits): 3 (highQ2_mc_per_file_schema.csv, validation_highQ2_mc_schema_full.json, validation_highQ2_mc_manifest.json)
  - medium (10-100): 5
  - light (1-10): 12
  - clean: 3-6 depending on counting rule

### 7.6 Nested vestige `H1/sidis_lowQ2/docs/data/`

Two files only:
- `H1/sidis_lowQ2/docs/data/acceptance_highQ2_charged_mc_prototype.json` (15402 lines; first 5 lines indicate this is the same artifact as `sidis_lowQ2/docs/data/acceptance_highQ2_charged_mc_prototype.json`)
- `H1/sidis_lowQ2/docs/data/validation_highQ2_mc_species_matching.json` (~1490 lines; same prefix structure as the canonical workspace copy)

These look like duplicate emits from a session that used the wrong base path
prefix (`H1/H1/...` vs `H1/...`). **Recommendation:** delete the nested
`H1/sidis_lowQ2/` after confirming the contents are not newer than the
canonical copies. This is a workspace-hygiene action independent of the
proton-rename question.

---

## 8. Recommended remediation tiers

### Tier 1 -- Safe documentation rewrites (EXECUTE NOW)

**Scope:** prose-only edits to clarify proton-vs-SIDIS distinction. **Do
NOT alter literal NAF path strings, filename literals, or branch-name
literals** (those must match on-disk reality). Editable items:

1. Add a single up-front "Naming convention" box to each high-traffic
   research doc:
   - `research-highQ2-sidis-extension.md`
   - `research-highQ2-status-snapshot-2026-05-19.md`
   - `reference-LAr-electron-scale-highQ2-sidis.md`
   - `reference-nuclia-correction-highQ2-sidis.md`
   The box should state: "The high-Q2 SIDIS builder is named
   `ProtonTreeBuilderHighQ2` and writes files named `proton_highQ2_*.root`
   for historical reasons (fork from the proton/antiproton package). The
   physics is inclusive charged-hadron SIDIS. References to `proton_HiQ` in
   this document refer to the SEPARATE proton/antiproton asymmetry analysis."
2. Rewrite the `research-highQ2-sidis-extension.md` section that uses
   `H1ProtonAsymHighQ2/` (stale package name); update to `H1SIDIS_HighQ2/`
   to reflect the realized rename.
3. Rewrite the prose phrase "proton-pipeline" / "proton/antiproton" /
   "proton/SIDIS pipeline" where it describes the SIDIS production
   infrastructure (`plan-highQ2-sidis-next-steps.md:107`,
   `research-highQ2-sidis-extension.md:164, 484, 488`) to read "v5b SIDIS
   builder (shares production infrastructure with the proton/antiproton
   analysis)" or similar.
4. In `context-handoff.md` Section near line 60, clarify that the
   P-momentum-window provenance is the proton/antiproton analysis but the
   SIDIS analysis no longer applies that window.

**Estimated cost:** 1 `code-editor` pass, ~15 file edits, ~50 lines changed.
**Production impact:** zero. **Risk:** zero.

### Tier 2 -- Session-produced artifact re-emission (EXECUTE NOW)

**Scope:** add a `_meta.naming_convention` block to each session-produced
JSON, and a "Naming convention" header section to each session-produced MD.
Re-emit the artifacts with prose rewrites of the kind in Tier 1.

**Important:** preserve every literal NAF path string. The point of Tier 2
is to add disclaimer text, not to rename the on-disk paths inside the JSON.

Affected files:
- 11 JSONs and 1 CSV under `sidis_lowQ2/docs/data/`
- 6 MDs under `sidis_lowQ2/docs/references/`
- 2-3 MDs under `sidis_lowQ2/docs/results/`
- 6 MDs under `sidis_lowQ2/docs/reviews/`

**Estimated cost:** 1-2 `code-editor` passes; could be batched as a single
"add `_meta` block + add header note" patch.

**Production impact:** zero.

**ALTERNATIVE (recommended): write one new file
`sidis_lowQ2/docs/references/reference-naming-aliases-highQ2-sidis.md` and
cross-reference it from each affected JSON/MD via a single line:
`See reference-naming-aliases-highQ2-sidis.md for legacy-vs-SIDIS naming.`**
This is far cheaper than per-file `_meta` edits and achieves the same
legibility outcome.

### Tier 3 -- NAF builder source rename + recompile (DEFER)

`ProtonTreeBuilderHighQ2.C -> SIDISTreeBuilderHighQ2.C` (or similar).
Requires:
- 1 NAF file rename.
- 1 `make bin` rerun (creates new binary `SIDISTreeBuilderHighQ2`).
- Edits to `submit_full_production.py` (3 string literals).
- Edits to `submit_proton_highQ2_batch.sh` (1 string + filename).
- Edits to `merge_mc_highQ2.sh` (1 string).
- Old binary `ProtonTreeBuilderHighQ2` continues to work until the next
  `make bin`, so there is a window of dual-binary operation.

**Production impact:** LOW if done before next data campaign; HIGH if
mid-campaign.

**Verdict:** DEFER to next builder major revision. The current SIDIS analysis
has no v5b SIDIS data on DUST yet (per
`validation_highQ2_data_availability.json`), so the v5b data campaign is the
natural moment to rename.

### Tier 4 -- NAF output-file rename (REJECT unless fresh production)

`proton_highQ2_mc_*.root -> sidis_highQ2_mc_*.root` for 1210 MC files (+
future data files when the data campaign runs).

**Production impact:** destroys every existing downstream script glob
pattern. Requires `mv` of every file, re-merge, re-validate, re-emit of
every doc that records a NAF path.

**Verdict:** REJECT unless the v5b data campaign launch is the trigger
event. If a fresh production launch is happening anyway, Tier 4 is cheap
(just change the output-stem template). If not, it is expensive and
risk-laden.

### Tier 5 -- Submission/downstream script patches (TIED TO Tier 3/4)

Auto-implied by Tier 3 or Tier 4. Independent execution makes no sense.

---

## 9. Recommendation

**Execute Tier 1 + Tier 2 now.** Tier 2 should be implemented in the
**alias-doc form** (write one new
`reference-naming-aliases-highQ2-sidis.md` plus a one-line cross-reference
from each affected file), not the per-file `_meta` block form, unless the
user specifically wants every JSON regenerated.

**Defer Tier 3** to the next builder major revision (currently v6 draft is
in `source_cache/ProtonTreeBuilder_lowQ2_v6_draft.C` for low-Q2; a high-Q2
v6 is not yet drafted). The natural trigger is the v5b -> v6 transition or
the moment when v5b data production launches.

**Reject Tier 4** unless a fresh production cycle is being launched anyway.
The 1210 files on disk are physically labelled per the legacy convention;
renaming them as a standalone act is high-risk and low-value.

**ADDITIONAL urgent recommendation independent of the proton-rename
question:** patch the **Category 4 active-code-path defects in
`produce_shifted_mc.py`, `produce_shifted_mc.C`, and `merge_and_validate.py`**
that reference `ProtonTot`/`ProtonTotGen` as still-present trees. These will
silently no-op or mis-validate on v5b inputs. The `review-builder-final-readiness.md`
already lists these as defects (Section D1 and the
"analysis-side" checklist item line 264). Tier 0a: patch the scripts to
drop the obsolete tree names from `EXPECTED_TREES` / `TREES_PASSTHROUGH` /
`FORBIDDEN_TREES` is already-correct. **Execute Tier 0a NOW** (before any
other tier).

---

## 10. Verbatim list of every LEGACY-naming string suitable for follow-up `code-editor` patch task

For Tier 1 + Tier 2 batched edits, the following distinct string patterns
should be the substitution targets (the LEFT column is the legacy literal
that appears in docs; the RIGHT column is the recommended prose substitution
**for descriptive text only -- DO NOT substitute inside path literals or
filename literals**):

| LEGACY string (in prose) | Recommended SIDIS substitution |
|---|---|
| "the proton builder" (where it refers to the SIDIS high-Q2 builder) | "the v5b SIDIS builder (`ProtonTreeBuilderHighQ2`, legacy name)" |
| "the proton high-Q2 builder" | "the high-Q2 SIDIS builder (legacy-named `ProtonTreeBuilderHighQ2`)" |
| "proton-pipeline" | "v5b SIDIS pipeline" |
| "proton pipeline" | "v5b SIDIS pipeline" |
| "the proton stage" (in submission script descriptions) | "the SIDIS tree-building stage (Stage 2)" |
| "proton/SIDIS pipeline" | "v5b SIDIS pipeline (shares production infrastructure with the proton/antiproton analysis)" |
| "the proton analysis" (when the surrounding sentence is about SIDIS infrastructure decisions) | "the SIDIS analysis" |
| "the proton/antiproton charged trees ARE the same trees" | "the v5b SIDIS charged trees are the same physical trees as those used by the proton/antiproton analysis" |
| "The production was driven by the proton/antiproton analysis, which writes both ProtonTot/ProtonTotGen and ..." | "Earlier (v4) production was driven by the proton/antiproton analysis and wrote ProtonTot/ProtonTotGen trees. v5b removed those trees; the production is now SIDIS-only." |
| "H1ProtonAsymHighQ2/" (in `research-highQ2-sidis-extension.md`) | "H1SIDIS_HighQ2/" (factual correction; package was renamed) |
| "H1ProtonAsymHighQ2" (as a package identifier) | "H1SIDIS_HighQ2" |
| "proton steering files" (in the SIDIS context, line 170/498 of `research-highQ2-sidis-extension.md`) | "SIDIS steering files (legacy directory `steering/proton/`)" |
| `EXPECTED_TREES = [..., 'ProtonTot', 'ProtonTotGen', 'DisTot']` (in scripts) | `EXPECTED_TREES = [..., 'DisTot']` (drop ProtonTot/ProtonTotGen; v5b removed them) |
| `TREES_PASSTHROUGH = ('ChargedTotGen', 'ProtonTot', 'ProtonTotGen', 'DisTot')` (scripts) | `TREES_PASSTHROUGH = ('ChargedTotGen', 'DisTot')` |
| `FORBIDDEN_TREES = ("ProtonTot",)` (`validate_v3_output.py:64`) | KEEP -- this is a positive guard against legacy schema |
| (header doc-string) "for the H1ProtonAsym analysis" | "for the H1SIDIS analysis (forked from H1ProtonAsym)" |
| `EXPECTED_RECO_BRANCHES = 82` (`merge_and_validate.py:51`) | `EXPECTED_RECO_BRANCHES = 78` (per live builder count) |
| `EXPECTED_GEN_BRANCHES = 35` (`merge_and_validate.py:52`) | `EXPECTED_GEN_BRANCHES = 44` (per live builder count) |

**Preserve unchanged (literal artifacts):**

| Preserve-unchanged string | Reason |
|---|---|
| `proton_highQ2_mc_*.root` (in NAF path literals) | Matches on-disk filenames |
| `proton_highQ2_*.root` (RAPGAP dual-naming pair) | Matches on-disk filenames |
| `proton_highQ2_data_smoke.root` | Matches on-disk filename |
| `/data/dust/.../H1SIDIS_HighQ2/output/.../proton_highQ2_*.root` | Real NAF paths |
| `/data/dust/.../bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` | Real NAF binary |
| `/data/dust/.../H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` | Real NAF source |
| `H1SIDIS_HighQ2/scripts/submit_full_production.py` | Real NAF script |
| `merged_proton_highQ2_mc_rapgap31.root`, `merged_proton_highQ2_mc_djangoh14.root` | Real NAF merge targets (not yet produced; defined in `merge_mc_highQ2.sh`) |
| `evls_dYY_NNNN_proton.root` | Real NAF evls per-file naming |
| `ProtonUtil::isChargedHadron`, `ProtonUtil::classifySourceGTR`, `ProtonUtil::getMassForPDG` | Real C++ namespace + symbol names |
| `DedxProton`, `LkProton`, `NlhProton` | Real (REMOVED) branch names; preserve where the doc explicitly says they are removed |
| `BeamProtP` | Real branch name |
| `M_PROTON`, `PROTON_MASS` | Real Python/C++ constants for the species proton mass |
| References to `H1/proton_HiQ/`, `H1/proton/`, `H1/lambda_HiQ/` | TRUE PROTON references to the separate analyses |
| `beam_proton`, `beam_proton_n`, `beam_proton_frac_of_all_gen`, `beam_proton_contamination` | TRUE PROTON physics fields (in species_matching JSON) |
| `Lambda -> p pi-`, "proton from Lambda decay" | TRUE PROTON decay products |

---

## 11. Risks, assumptions, open questions

### Risks identified during audit

- **R1.** `produce_shifted_mc.py`, `produce_shifted_mc.C`, and
  `merge_and_validate.py` contain ProtonTot/ProtonTotGen references in
  *active code paths*. If they are run against v5b inputs they will produce
  wrong results (either silent no-ops on a tree that does not exist, or a
  schema-mismatch validation failure). This is a defect independent of the
  proton-rename question; see Section 5.
- **R2.** `analyze_proton_highQ2.py` on NAF has a docstring describing a
  *proton/antiproton asymmetry analysis* (`A = (N_p/eff_p - N_pbar/eff_pbar) / (N_p/eff_p + N_pbar/eff_pbar)`)
  inside the `H1SIDIS_HighQ2/scripts/` tree. This is **AMBIGUOUS**: either
  the script is a stale stub from the fork, or it is an active analyzer for
  a different observable. **Needs investigation** before any rename can
  proceed.
- **R3.** `research-highQ2-sidis-extension.md` consistently writes
  `H1ProtonAsymHighQ2/` for the package path when the real NAF path is
  `H1SIDIS_HighQ2/`. This is a separate documentation defect; the package
  was renamed and the research doc was not updated.
- **R4.** The nested `H1/sidis_lowQ2/` workspace vestige contains two stale
  copies of session-produced JSON files. Workspace hygiene cleanup needed.

### Assumptions

- The user's path prefix `H1/sidis_lowQ2/` resolves to `sidis_lowQ2/` at
  workspace root, NOT to the nested vestige `H1/sidis_lowQ2/`. This is
  consistent with the AGENTS.md convention and the file existence at the
  workspace-root path.
- "Session-produced" in Category 6 means the files inventoried by the user,
  which all exist at the workspace-root canonical paths.
- The `naming convention` alias doc (recommended in Tier 2) is a sufficient
  legibility intervention; the user does not require physical renames.

### Open questions

- **Q1.** Is `analyze_proton_highQ2.py` on NAF a live SIDIS-pipeline
  script or a stale fork-leftover? Resolving Q1 determines whether Category 4
  needs to expand by one script.
- **Q2.** Are there any *downstream* tools outside `H1SIDIS_HighQ2/` that
  glob the legacy `proton_highQ2_*.root` filename pattern (e.g., lambda_HiQ
  cross-analysis joins)? An unconstrained `grep` outside the SIDIS workspace
  was not performed in this audit.
- **Q3.** Does the package-name drift `H1ProtonAsymHighQ2/` -> `H1SIDIS_HighQ2/`
  (Risk R3) indicate a previous incomplete rename attempt? If so, the lessons
  learned would inform the Tier 3 plan.

### Coverage gaps

- The audit did NOT inspect:
  - The `proton_HiQ/` workspace itself (out of scope; separate analysis).
  - The `lambda_HiQ/` workspace.
  - The `h1oo-master/` framework.
  - Top-level `scripts/` directory at workspace root (`/Users/hklest/Desktop/opencode/H1/scripts/`).
  - `sidis_lowQ2/old/local_layout_precleanup_snapshot/` (under `old/`).
  - `sidis_lowQ2/old/superseded_docs/` (under `old/`).
  - `analyze_proton_highQ2.py` body (only the head was read).
- A full grep of these would likely find additional LEGACY mentions but is
  not blocking the recommendation in Section 9.

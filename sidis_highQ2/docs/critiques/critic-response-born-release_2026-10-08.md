# Critic response — 2026-10-06 raw and 2026-10-07 Born release chain (2026-10-08)

## Scope

Two independent critic reviews were run on 2026-10-08 against the installed
central release chain:

- **Critic A — physics-consistency critic**: reviewed the physics consistency
  of the coherent-prior RAW central release
  `bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06`
  (installed 2026-10-07) and the Born-level release
  `born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07`
  (installed 2026-10-07, `c_rad_applied=TRUE`,
  `sigma_Born(bin) = C_rad(bin) x Unfold(data; R)`), including the C_rad
  xFcut derivation chain and the worker-level selection semantics.
- **Critic B — implementation-integrity critic**: reviewed the same two
  releases for documentation/status integrity: pre-promotion self-labels,
  stale snapshots, dead relative paths, and release-vs-candidate consistency.

This document records the dispositions and the executed **doc-only** fixes
(2026-10-08). No physics file, no data file, and no analysis-code logic was
changed. Every edit inside a release directory was applied identically to the
installed copy and the corresponding `data_release_candidates/` source
candidate copy, and every manifest-hashed edited file had its manifest entry
recomputed.

## Findings and dispositions

| ID | Finding | Disposition |
|---|---|---|
| A-F1 | C_rad x-cell binning variable mismatch | **QUANTIFIED 2026-10-08**: study `H1ChargedSidis_HighQ2/output_v6/studies/crad_xbinning_sensitivity_2026-10-08/`, report `docs/radiative/CRAD_XBINNING_SENSITIVITY_2026-10-08.md`. RAD-leg flip rate 1.7483% (NONRAD 0.00305%); per-bin \|ΔC_rad\|/C_rad median 3.5049% (h+ 3.5031% / h− 3.5138%), max 11.3712%; integrated cancels (h+ 0, h− 0). **VERDICT: NOT second-order** — new dedicated C_rad x-binning-definition systematic component needed, or rederive the applied table with release-safe (Q2esGen, x_es_gen) binning. **RESOLVED 2026-10-09: user approved rederivation; release-safe tables rederived, validated, gated, and promoted — see docs/provenance/releasebin-install-verification-2026-10-09.md** |
| A-F2 | reco xF species-mass vs pion-mass asymmetry | **ACCEPTED BY USER** as intended design (no species PID available); no change; remains a documented convention |
| A-F3 | gen-charge routing of matched rows | **QUANTIFIED 2026-10-08**: study `H1ChargedSidis_HighQ2/output_v6/studies/charge_misid_response_2026-10-08/`, report `docs/unfolding/charge_misid_response_study_2026-10-08.md`. Charge-misID rate 0.3368% unweighted / 0.3353% weighted; per-bin unfolding bias h+ median 0.0464% / h− 0.0507%; 0/490 bins exceed the crad13 per-bin budget. **VERDICT: negligible for per-bin cross sections** (documented subdominant source, no release correction); **MATERIAL for charge asymmetry** — mandatory systematic or identity-consistent correction before any A_h quote. **CLOSED with A_h caveat carried forward** |
| A-F4 | Wrong gen DIS branch names in the Born `SELECTION.md` C_rad section | **FIXED 2026-10-08** (doc-only; see below) |
| A-F5 | Misleading `crad_v3_worker_xfcut.py` docstring (implied the recomputed safeXF removes the ISR nominal-beam-q bias) | **FIXED 2026-10-08** (doc-only; see below) |
| B-F1 | Raw candidate `SELECTION.md` post-install drift (installed copy lacks the "Empz is layered" section that the candidate copy carries) | **OPEN**, not actioned per user instruction; the drift is retained and is the ONLY installed-vs-candidate difference in the raw release |
| B-F2 | 9-page vs 6-page `born_crad13` review package description | **OPEN**, not actioned per user instruction |
| B-F3 | Stale pre-promotion self-labels in release `README.md` / `metadata.json` | **FIXED 2026-10-08** (doc-only; see below) |
| B-F4 | Stale 2026-06-30 `provenance/RUNLIST.md` snapshots inside both releases | **FIXED 2026-10-08** (dated banner prepended; snapshot content unchanged; see below) |
| B-F5 | Dead relative `data/radiative/...` shorthand in `docs/RUN_STATUS.md` and `context-handoff.md` | **FIXED 2026-10-08** (absolute qualification + path note; see below) |
| B-F6 | 100-toy remnants in `summary_totals.json` (by declared refresh scope) | Already documented in-release (README "IBU statistical sidecar refresh" section and refresh provenance note); **no action** |

## Fixes executed

### A-F4 — wrong gen DIS branch names (Born `SELECTION.md`)

The C_rad gen-level selection bullet attributed the DIS fiducial to
`150 < Q2sGen < 20000`, `0.1 < YsGen < 0.7`. Verified against the worker
(`crad_v3_worker_xfcut.py:117-119`): the DIS fiducial is computed from
`Q2_es_gen` / `Y_es_gen` / `x_es_gen`
(`dis = ... (q2>=150)&(q2<20000)&(y>.1)&(y<.7)`); `Q2sGen`/`YsGen` feed only
the `sigma_photon` q-variant (`qs["sigma_photon"] = q_photon(a["Q2sGen"],
a["YsGen"], ...)`; the `esigma_photon` variant uses `Q2esGen`/`YesGen`).

Edit (identical in installed and candidate copies):

- Before: `- Gen-level selection of the C_rad tables: `150 < Q2sGen < 20000`,
  `0.1 < YsGen < 0.7`, `GenEmpz > 45` (nominal, no upper bound), ...`
- After: `- Gen-level selection of the C_rad tables: `150 < Q2_es_gen < 20000`,
  `0.1 < Y_es_gen < 0.7`, `GenEmpz > 45` (nominal, no upper bound), ...` plus
  the added clarifying sentence: `The `Q2sGen`/`YsGen` branches are NOT part of
  this DIS fiducial; they feed only the sigma_photon q-variant of the worker.`

Manifest note: the delegation brief assumed `SELECTION.md` is not
manifest-hashed. Verified: true for the RAW manifest (no `SELECTION.md` entry)
but **false for the Born manifest** (`SELECTION.md` IS hashed there). The Born
manifest entry was recomputed: bytes `8084 -> 8216`, sha256
`e9bafd2e62e2fc37a912862bcac785eedd1e580b205a8b3ed5a7bf60a8449098` ->
`9e6d2036ff56c82e6814e0a5bca19d58d704b518bcdf4624c29c6d534b2f1ca3`.

### A-F5 — misleading worker docstring (`crad_v3_worker_xfcut.py`)

The docstring said the stored RAD `xFGen` branch is not used due to a "known
nominal-beam-q bug under ISR, bounded +0.039% integrated", implying the
recomputed safeXF removes the ISR bias. In fact the recomputation uses the
SAME nominal-beam q, so the +0.039% ISR-q bias is shared identically by the
C_rad fiducial and the release gen fiducial (they remain consistent with each
other; the bias is NOT removed).

Edit (docstring only):

- Before: `... the stored RAD xFGen branch is NOT used (known nominal-beam-q
  bug under ISR, bounded +0.039% integrated).  Definition mirrors ...`
- After: `... the stored RAD xFGen branch is NOT used.  The recomputation uses
  the SAME nominal-beam q as the stored branch, so the known nominal-beam-q
  bias under ISR (bounded +0.039% integrated) is NOT removed: it is shared
  identically by this C_rad fiducial and the release gen fiducial, which
  remain consistent with each other.  Definition mirrors ...`

Location discrepancy vs the delegation brief: the brief stated the script
exists in two snapshot copies under
`sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/` and
`H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/`. Verified
exhaustively (full-tree finds of both workspaces plus complete file listings
of both canonical snapshot trees, 17 + 111 files): **no copy exists in either
canonical snapshot tree, and exactly one copy exists workspace-wide**, at
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/crad_v3_worker_xfcut.py`
(the path registered in `crad_djangoh14_xfcut.json` and
`xfcut_campaign_manifest.json`). That single copy was edited; the two-copy
md5-identity check is therefore vacuous. Post-edit md5:
`89eba761bc5852f1a7d78918178414e7`; Python syntax re-validated. No release
copy of the script exists (checked against both release manifests and a
full-tree find). The old wording remains INTENTIONALLY RETAINED in frozen
provenance (see the retained-strings section).

### B-F3 — stale pre-promotion self-labels

All edits applied identically to installed and candidate copies; manifest
entries recomputed for every hashed file.

RAW `README.md` (installed + candidate):

- Line 1 before: `# RELEASE CANDIDATE — coherent prior; pending promotion;
  canonical release 2026-06-10 still authoritative until promotion`
- Line 1 after: `# RELEASE — INSTALLED CANONICAL RAW CENTRAL RELEASE since
  2026-10-07; coherent body+miss prior; RAW PARENT of the Born release since
  the 2026-10-07 c_rad promotion; k=4; L_data=351000.0 nb^-1`
- Line 3 before: `PASS: candidate IBU+BBB release, ...`
- Line 3 after: `PASS: coherent-prior IBU+BBB release, ...` (the physics-level
  statements — raw/no-bgsub, no C_rad, not Born-level, not final physics
  approval — remain true for the raw release and are unchanged)
- Manifest: bytes `3342 -> 3417`, sha256
  `7f1f913c2431ea168c22141cd80ebf813a689c02d131529b11a3bf352190bfa4` ->
  `ded6ed706bb1dde71abeecfb76f25a3065b8a0f08f561a1eae2e52324dc4cdb7`

BORN `README.md` (installed + candidate):

- Lines 1-9 before: `# BORN-LEVEL CANDIDATE RELEASE — C_rad applied
  (DJANGOH14 xFcut); NOT canonical` ... `This is a NEW dated candidate under
  `data_release_candidates/` ... It is **not canonical and not promoted** ...`
- Lines 1-9 after: `# BORN-LEVEL RELEASE — INSTALLED CANONICAL since
  2026-10-07 — C_rad applied (DJANGOH14 xFcut)` plus a status block stating:
  installed canonical BORN-LEVEL release since 2026-10-07;
  `c_rad_applied=TRUE`; `sigma_Born(bin) = C_rad(bin) x Unfold(data; R)`
  (post-unfold, gen-level, per-bin); applied primary DJANGOH14 xFGen<0 xFcut
  EMPZ>45; RAPGAP31 xFcut FORMAL ALTERNATIVE scheme; NOT publication-grade
  (`P-CRAD-LUMI` exposure provenance `DIAGNOSTIC_MANIFEST_LEVEL`, PROVISIONAL
  xFcut baseline, open EMPZ>45 convention decision); parent raw release
  remains installed and canonical as the RAW product (RAW PARENT of this
  release).
- Two residual in-body self-labels also fixed: `this candidate is raw/no-bgsub
  at track level` -> `this release is ...` and `No plots are shipped in this
  candidate.` -> `No plots are shipped in this release.`
- Manifest: bytes `11595 -> 11925`, sha256
  `484dfc3b8d642e67c4b440550024af94ec4fb26748a36605ad9ed2bfc86e9baf` ->
  `076df962ac9fa7e63b8730e1fdb055d13740c8181b165911ea7b25a10c8b5b96`

RAW `metadata.json` (installed + candidate):

- `outputs.release_dir`: `.../data_release_candidates/bbb_ibu_coherentprior_..._2026-10-06`
  -> `.../data_release/bbb_ibu_coherentprior_..._2026-10-06`
- `outputs.plots_dir`: same candidates-path defect in the same `outputs` block;
  fixed in parallel (`.../data_release/.../plots`) so the block stays
  internally consistent
- Manifest: bytes `7423 -> 7401`, sha256
  `2e6d2180c80a2d46119c39ea1d9c6fb825ef10092e60a3c5f177afa25a2c7205` ->
  `471b8ecd0ede74579c641128d9640228d240cb6a866b4875ba05cdbf17b7f7ee`

BORN `metadata.json` (installed + candidate):

- `status`: `CANDIDATE -- BORN-LEVEL PROVISIONAL; not canonical; promotion is
  a separate step and no context document is updated by this build` ->
  `INSTALLED canonical BORN-LEVEL release since 2026-10-07 -- BORN-LEVEL
  PROVISIONAL; not publication-grade (P-CRAD-LUMI, PROVISIONAL xFcut, open
  EMPZ>45 convention)`
- `parent_release`: candidates path -> installed `data_release/` raw release
  path
- `parent_release_role`: `c_rad_applied=false provenance release, protected in
  place; the installed central release is its checksum-verified copy` ->
  `RAW PARENT of this Born release (c_rad_applied=false raw product, protected
  in place); installed canonical RAW central release since 2026-10-07`
  (consequence of the pointer fix)
- `alternative_scheme.role`: `... subproduct inside this candidate; ...` ->
  `... subproduct inside this release; ...`
- Manifest: bytes `10877 -> 10921`, sha256
  `eafab135453adb005cd1f511388d5fefeb1fc72764717d7569be574a052c50d9` ->
  `d1438adef5adc813b25033d92db52ded644a798fe866dd0b4ad66696c3dc6a8b`

Stat-sidecar provenance note (`provenance/stat_sidecar_1000toy_refresh_2026-10-07.md`,
both releases): **left unchanged by judgment**. It is a dated historical
record of the refresh action executed pre-promotion ("QA Condition 1 executed
on the coherent-prior candidate release before promotion"; "Refresh date:
2026-10-07"; acceptance checks dated 2026-10-07). Its "Candidate release" and
"Backup location" candidate-path references are historically accurate, and
the referenced backup exists at the candidates path in both the installed and
candidate copies. Rewriting them would falsify the action record.

### B-F4 — stale RUNLIST snapshots

`provenance/RUNLIST.md` in all four release copies (RAW installed/candidate,
BORN installed/candidate) is a stale 2026-06-30 snapshot whose "current"
language names the superseded 2026-06-10 plain-prior release, a retired
budget, the now-reversed C_rad label assignment (RAPGAP31 nominal /
DJANGOH14 cross-check), and a macOS mirror path. Per instruction the content
was NOT rewritten; the following identical banner was prepended at the very
top of all four copies:

> **HISTORICAL SNAPSHOT — 2026-06-30. Superseded.** Release, budget, and
> C_rad label assignments below are outdated. Current state:
> /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/RUN_STATUS.md

`provenance/RUNLIST.md` IS manifest-hashed in both releases; both manifest
entries were recomputed: bytes `12888 -> 13095`, sha256
`bdea08e568637a7bbc4eb48e96bbc1e0d216d5294c0d22f6f8362a9084bb57e5` ->
`8a1446ebab8e54ca1e2c2c9e679de03d17ea3f94c596df91d6fc224e8b8799fc`
(identical file, hence identical hash, in both manifests).

### B-F5 — dead relative path shorthand

The shorthand `data/radiative/djangoh14_crad_final_xfcut_2026-10-05/...` and
`data/radiative/rapgap_crad_final_xfcut_2026-10-05/...` does not resolve under
the declared active workspace `H1ChargedSidis_HighQ2/` (which has no
`data/radiative/` directory); the tables live at
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/...`.
No other content or numbers were altered.

- `docs/RUN_STATUS.md`: the two table paths were qualified with the absolute
  prefix in the Compact Allow block (line 33, first mention) and in the
  C_rad Summary table rows (lines 159-160). One "Path note" sentence was
  added after the C_rad Summary provenance line stating that every remaining
  `data/radiative/...` shorthand in the document (including `quarantine/` and
  `esigma_photon_variant_2026-09-26/`) resolves under
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/`, not
  under `H1ChargedSidis_HighQ2/`.
- `context-handoff.md`: the two C_rad Status table paths (lines 261-262) were
  qualified with the absolute prefix, and the same "Path note" sentence was
  added at the end of the C_rad Status section (covering the `quarantine/`
  and `esigma_photon_variant_2026-09-26/` shorthands that remain in that
  document).

## Edit log

| # | File (absolute path; candidate twin edited identically) | What changed | Why | Manifest updated |
|---|---|---|---|---|
| 1 | `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/SELECTION.md` (+ candidate twin) | C_rad gen DIS cut branch names `Q2sGen`/`YsGen` -> `Q2_es_gen`/`Y_es_gen`; added clarifying sentence (sigma_photon q-variant only) | A-F4: doc contradicted the worker cut | Yes (BORN manifest: SELECTION.md 8084->8216, sha -> `9e6d2036ff56...`) |
| 2 | `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/crad_v3_worker_xfcut.py` (single workspace copy; no candidate twin exists) | Docstring reworded: recomputation uses the SAME nominal-beam q; +0.039% ISR-q bias shared by C_rad fiducial and release gen fiducial, NOT removed | A-F5: docstring implied the bias is removed | N/A (not in any release manifest) |
| 3 | `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/README.md` (+ candidate twin) | Line 1 status -> installed canonical RAW central release since 2026-10-07 (+ RAW PARENT since c_rad promotion, coherent body+miss prior, k=4, L_data=351000.0 nb^-1); line 3 "candidate" -> "coherent-prior" | B-F3: stale pre-promotion self-label | Yes (RAW manifest: README.md 3342->3417, sha -> `ded6ed706bb1...`) |
| 4 | `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/README.md` (+ candidate twin) | Lines 1-9 status block -> installed canonical BORN-LEVEL release since 2026-10-07 (c_rad_applied=TRUE, sigma_Born definition, DJANGOH14 primary / RAPGAP31 formal alternative, NOT publication-grade); two in-body "this candidate" -> "this release" | B-F3: stale pre-promotion self-labels | Yes (BORN manifest: README.md 11595->11925, sha -> `076df962ac9f...`) |
| 5 | `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/metadata.json` (+ candidate twin) | `outputs.release_dir` (and `outputs.plots_dir`, same block) candidates path -> installed `data_release/` path | B-F3: stale self-pointer to the pre-promotion location | Yes (RAW manifest: metadata.json 7423->7401, sha -> `471b8ecd0ede...`) |
| 6 | `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/metadata.json` (+ candidate twin) | `status` self-label; `parent_release` -> installed raw release path; `parent_release_role` reworded; `alternative_scheme.role` "this candidate" -> "this release" | B-F3: stale pre-promotion self-labels and parent pointer | Yes (BORN manifest: metadata.json 10877->10921, sha -> `d1438adef5ad...`) |
| 7 | `.../bbb_ibu_coherentprior_..._2026-10-06/provenance/RUNLIST.md` (+ candidate twin) | Dated HISTORICAL SNAPSHOT banner prepended; content unchanged | B-F4: stale 2026-06-30 snapshot read as current | Yes (RAW manifest: RUNLIST.md 12888->13095, sha -> `8a1446ebab8e...`) |
| 8 | `.../born_crad_dj14xfcut_..._2026-10-07/provenance/RUNLIST.md` (+ candidate twin) | Identical banner prepended; content unchanged | B-F4 (same file shipped in the Born release) | Yes (BORN manifest: RUNLIST.md 12888->13095, sha -> `8a1446ebab8e...`) |
| 9 | `.../bbb_ibu_coherentprior_..._2026-10-06/manifest.json` (+ candidate twin) | Hash/bytes entries recomputed for README.md, metadata.json, provenance/RUNLIST.md; n_files 38 unchanged; no files added/removed | Manifest integrity after edits | N/A (manifest is self-excluded) |
| 10 | `.../born_crad_dj14xfcut_..._2026-10-07/manifest.json` (+ candidate twin) | Hash/bytes entries recomputed for README.md, SELECTION.md, metadata.json, provenance/RUNLIST.md; n_files 46 unchanged; no files added/removed | Manifest integrity after edits | N/A (manifest is self-excluded) |
| 11 | `sidis_highQ2/docs/RUN_STATUS.md` | Two C_rad table paths qualified with the absolute `sidis_highQ2/` prefix (lines 33, 159-160); one "Path note" sentence added | B-F5: shorthand unresolvable under the active workspace | N/A |
| 12 | `sidis_highQ2/context-handoff.md` | Two C_rad table paths qualified with the absolute prefix (lines 261-262); one "Path note" sentence added | B-F5 | N/A |
| 13 | `sidis_highQ2/docs/critiques/critic-response-born-release_2026-10-08.md` | New decision record (this file) | Edit log / decision record requirement | N/A |

## Intentionally retained stale strings (frozen provenance / historical records)

In-release, frozen verbatim records of executed code (not edited, per the
strict rules):

- RAW `scripts/run_bbb_ibu_release_coherentprior.py` — embeds the
  pre-promotion README template string ("RELEASE CANDIDATE — coherent prior;
  pending promotion; ..."). Verbatim record of the executed driver.
- RAW `scripts/validate_release.py` (and `run_validation.sh`,
  `submit_validation.sub`, `run_release.sh`, `submit_release.sub`) —
  candidate-era strings retained; verbatim script copies.
- RAW `_backup_100toy_stat_2026-10-07/README.md` (and the whole backup
  directory) — pre-refresh README copy; frozen backup provenance.
- RAW + BORN `provenance/stat_sidecar_1000toy_refresh_2026-10-07.md` — dated
  historical record of the pre-promotion refresh action; "Candidate release"
  and "Backup location" candidate-path references are historically accurate
  and resolve (see B-F3 section).
- RAW `metadata.json` field `ibu_stat_1000toy_refresh_provenance.backup_location`
  — candidates-path backup pointer inside the dated refresh provenance block;
  the backup exists at that path; retained as a historical record.
- RAW `README.md` line 6 ("Canonical driver copied from
  `<candidates>/provenance/canonical_driver_source.txt`; candidate driver:
  ...") and line 27 ("candidate nominal reproduction ~1e-16") — build
  provenance pointer and refresh-validation terminology; both accurate
  records of the build/refresh, not status self-labels.
- BORN `scripts/build_born_candidate.py` and `logs/*` (rev-3 and run-1 builder
  sources, superseded README/metadata/SELECTION/validation snapshots) —
  verbatim builder sources and superseded run records; still carry the old
  `Q2sGen`/`YsGen` selection text and pre-promotion status strings.
- BORN `validation/validation.json` and `validation/validation_prev_superseded.json`
  — frozen run records.
- BORN `ibu/` and `bbb/` `cross_sections.{csv,json}` per-row `caveat` columns —
  shipped DATA files whose caveat string reads "PROVISIONAL BORN-LEVEL
  CANDIDATE -- c_rad_applied=true ...". Data files are out of doc-fix scope;
  a future re-derivation would have to change the builder's caveat template.
- BORN `metadata.json` `"schema": "sidis_highq2_born_crad_candidate_metadata_v1"`
  — schema version identifier (validation-keyed), not a status self-label.
- BORN `metadata.json` `columns_intentionally_changed.caveat` and BORN
  `README.md` scaling-policy table — "the Born-level candidate caveat (QA C3)"
  is the historical QA-C3 gate name of the caveat string, matching the frozen
  `validation/validation.json` language.
- BORN `README.md` "Provenance pointers" parent-release line (candidates
  path) and "protected trees (parent candidate, ...)" validation description —
  accurate build-provenance pointers to the intact source candidate; the
  fixed header states the parent is installed and canonical as the RAW
  product.
- BORN `SELECTION.md` inheritance statement (line 8, candidates-path parent
  pointer) — build provenance ("inherited UNCHANGED from the parent
  coherent-prior release"); per instruction everything except the C_rad
  branch names was kept unchanged.
- RAW + BORN `provenance/RUNLIST.md` content below the banner — the stale
  2026-06-30 snapshot text is intentionally retained verbatim; the banner
  marks it superseded.
- RAW `README.md` "IBU statistical sidecar refresh" section and
  `summary_totals.json` 100-toy remnants — B-F6, already documented in-release
  by declared refresh scope; no action.

Outside the releases, frozen provenance carrying the OLD A-F5 docstring
wording ("stored RAD xFGen branch NOT used (known nominal-beam-q bug under
ISR, bounded +0.039% integrated)"), all intentionally retained:

- `sidis_highQ2/data/radiative/djangoh14_crad_final_xfcut_2026-10-05/crad_djangoh14_xfcut.json`
  and `.../xfcut_campaign_manifest.json`
- `sidis_highQ2/data/radiative/rapgap_crad_final_xfcut_2026-10-05/crad_rapgap31_xfcut.json`
  and `.../xfcut_campaign_manifest.json`
- `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/worker_xfcut.diff`
  (verbatim fork-diff record)
- The edited worker's own runtime config string (`xf_definition` in `main()`),
  which still says "stored RAD xFGen branch not used (nominal-beam-q bug under
  ISR)": it is a runtime diagnostic string recorded into output metadata, not
  the docstring; changing it belongs to a code-side change, not this doc-only
  pass.
- Dated historical docs mentioning the nominal-beam-q wording:
  `docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md`,
  `docs/radiative/SESSION_HANDOFF_crad_2026-09-25.md`,
  `docs/radiative/infiducial_boost_impact_2026-09-25.md`,
  `docs/radiative/EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md`,
  `docs/radiative/rad_hplus_proton_excess_2026-09-25.md`.

## Acceptance checks (2026-10-08, post-edit)

- `diff -rq` installed vs candidate, BORN release: **ZERO differences**.
- `diff -rq` installed vs candidate, RAW release: exactly one difference —
  the pre-existing `SELECTION.md` drift (B-F1, OPEN and intentionally not
  actioned per user instruction). No new differences were introduced; every
  file edited in this task is byte-identical across the two copies.
- Manifest hash verification: RAW installed 38/38 PASS, RAW candidate 38/38
  PASS, BORN installed 46/46 PASS, BORN candidate 46/46 PASS (recomputed
  sha256 + byte length for every listed file).
- File counts unchanged: RAW installed 71, RAW candidate 71, BORN installed
  68, BORN candidate 68. No file was added, removed, moved, or renamed inside
  any release directory.
- `crad_v3_worker_xfcut.py`: only one workspace copy exists (see A-F5
  section); post-edit md5 `89eba761bc5852f1a7d78918178414e7`; Python syntax
  valid.
- Fixed strings no longer appear in the fixed files; all remaining
  occurrences are in the frozen files enumerated above.

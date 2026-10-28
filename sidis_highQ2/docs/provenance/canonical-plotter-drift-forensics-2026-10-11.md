# Canonical-plotter drift forensics — 2026-10-11

Read-only forensic investigation of the LIVE canonical plotter drift recorded by
the bgsub set-R build validation (study `output_v6/studies/bgsub_setr_build_2026-10-10/`,
candidate validation JSON). No file was modified by this dispatch except this report.
All hashes below were computed on 2026-10-11 (NAF, DUST).

**Finding under investigation:** the LIVE canonical plotter
`make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` was
re-pointed at the installed Born release on 2026-10-10; its merge block no
longer multiplies `*_nb` values by C_rad. The correct merge-block pair is
preserved in the frozen provenance copy sha256-pinned to the rev-4 validation
record; the bgsub packager sources that frozen copy.

**Scope caveat:** the workspace git repo is stale (last commit 2026-03-08) and
both registry trees are untracked (`git status`: `??`; not gitignored; no
`git log --follow` history exists for any plotter path). The timeline below is
reconstructed from file mtimes, retained backups, frozen provenance copies, and
the registry documents (`RUNLIST.md`, `RUN_STATUS.md`, `STATE.md`). A concurrent
bgsub session was still writing to the study directory during this
investigation (official chain completed 2026-10-11 01:43); no evidence below
was taken from files that changed during the investigation.

Registry topology (per `RUNLIST.md` line 220/222): the "remote bundle" copy is
`H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/`, the "local
snapshot" copy is `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/`.
The original Mac-side registry paths in `MANIFEST.json`
(`/Users/hklest/Desktop/opencode/H1/...`) are not accessible from NAF; the
on-NAF census below is complete.

---

## 1. Per-copy census and verdicts

### 1.1 `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (nominal Born plotter)

| # | Copy (absolute path) | mtime | size (B) | md5 | sha256 | Drifted? |
|---|---|---|---|---|---|---|
| 1 | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (LIVE, local snapshot) | 2026-10-10 07:02:11 | 51345 | `271e3437cdfeade41bbe63b14b64dd2a` | `4ee97d7a51625b5d2bc4eca751dbb487e76c156f6e82ef8b794691b93c295e99` | **YES** |
| 2 | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (LIVE, remote bundle) | 2026-10-10 07:02:11 | 51345 | `271e3437cdfeade41bbe63b14b64dd2a` (identical) | `4ee97d7a...` (identical) | **YES** |
| 3 | `.../H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py_pre_repoint_2026-10-07` (pre-repoint backup, remote bundle only) | 2026-10-01 05:59 | 39444 | `278710ff93825b510147f43722540a31` | `834fd9f94a2f90cef55319a25aa960151c84136d41cc1a2264e1d0e2f10733f7` | NO — multiplication present (lines 223–225) |
| 4 | `.../H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/provenance/canonical_plotter_crad_applied_source.py` (frozen) | 2026-10-07 18:36 | 48962 | `01e20e93f46dc600acfbc5952016593f` | `89801128435ad049d598567f6b776b956a5c3a3720792529cf4f9251189d9bb6` | NO — correct |
| 5 | `.../H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/provenance/canonical_plotter_crad_applied_source.py` (frozen; **the** sha-pinned provenance copy of the installed Born release) | 2026-10-07 18:36 | 48962 | `01e20e93f46dc600acfbc5952016593f` | `89801128...` (identical to #4) | NO — correct |
| 6 | `.../H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_build_2026-10-10/born_release_candidate/DRYRUN_born_crad_bgsub_setR_centralbase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/provenance/canonical_plotter_crad_applied_source.py` (embedded as-found snapshot; mtime preserved from source) | 2026-10-10 07:02:11 | 51345 | `271e3437...` | `4ee97d7a...` | Content = drifted LIVE script. **Deliberate as-found audit snapshot**, not used as machinery |
| 7 | `.../H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_build_2026-10-10/born_release_candidate/born_crad_bgsub_setR_centralbase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/provenance/canonical_plotter_crad_applied_source.py` (same, official candidate) | 2026-10-10 07:02:11 | 51345 | `271e3437...` | `4ee97d7a...` | same as #6 |
| 8 | `.../H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_veto_release_v6_20261005/plots/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (study-local variant, reads the study's own release) | 2026-10-06 16:28 | 39448 | `0b720b696f1ef7b9cffb566216a79d45` | `7c391d6b03315b3c641c83aa3e0ea0d879c0232b469f17c000f39e17fb33d6d3` | NO — multiplication present (lines 223–224) |

Execution evidence: `__pycache__/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.cpython-311.pyc`
in **both** registry trees is dated 2026-10-10 07:03 — the drifted script was
imported/executed ~50 s after the 07:02:11 edit.

### 1.2 `make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py` (derived bgfracsub variant)

| # | Copy | mtime | size (B) | md5 | sha256 | Drifted? |
|---|---|---|---|---|---|---|
| 1 | `.../sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py` (LIVE) | 2026-10-09 19:39:45 | 56172 | `34c49e7a38a2229fd031f89d0d7b8012` | `f1c805c8780b9ab6d4dbac7baa4ee05ce70f8e77b3da63266b97cbf0ce0a40f1` | NO — applies `(1−f_b)·C_rad` (line 763); Oct 9 edit was registration-only |
| 2 | `.../H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py` (LIVE) | 2026-10-09 19:39:45 | 56172 | `34c49e7a...` (identical) | `f1c805c8...` | NO |
| 3 | `.../H1ChargedSidis_HighQ2/scripts/.../make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py_pre_repoint_2026-10-07` (backup) | 2026-10-01 10:00 | 46100 | `98737aa7dcfe2dd3535f2c35824a82e2` | `5ba3db0f70cc304970224458764f65f69d825c8f30e1436a0a154f39e2eef01c` | NO — factor intact (line 636) |
| 4 | `.../born_crad_dj14xfcut_..._2026-10-07/provenance/canonical_plotter_crad_bgfracsub_source.py` (frozen) | 2026-10-07 22:58 | 54841 | `ab87d5efccfe05ccb6094ec516900677` | `7459b3720f8739c0524499ae721889e96706f4c839890e925693f184e228cc24` | NO |
| 5 | `.../born_crad_dj14relbin_..._2026-10-09/provenance/canonical_plotter_crad_bgfracsub_source.py` (frozen) | 2026-10-07 22:58 | 54841 | `ab87d5ef...` (identical) | `7459b372...` | NO |

Related derivative (different filename, not a registered copy):
`.../sidis_highQ2/scripts/cross_section/finalcandidate_crad_envelope_2026-10-02/canonical_plotter_crad_bgfracsub_env.py`
(2026-10-02, md5 `cbd70ca2448b077fb6ae7989bfa3ab7e`) still applies
`(1−f_b)·C_rad` (line 636) — not affected.

### 1.3 Frozen merge-block extracts (`canonical_merge_block_extracted.py`, block pair sha256-pinned)

All correct (block1 sha256 `819eb2ba7c9154204a612b5c75fd28e02a62b1e9929bba363ae34eda5034909f`,
block2 sha256 `351bbac241829ef176b60f0964d1122f2a85cfc6d48005794be3a91ec640e3c7`;
file sha256 `8dedd657c9df53e5b4319dbdae070a5425067035bf6751489cdc500427ab0e53`, md5 `9fa15f2972f3bacf8dcafbae26ee4e74`, 6672 B):

- `.../born_crad_dj14xfcut_..._2026-10-07/provenance/canonical_merge_block_extracted.py` (mtime 2026-10-07 23:32)
- `.../born_crad_dj14relbin_..._2026-10-09/provenance/canonical_merge_block_extracted.py` (mtime 2026-10-09 01:22) — **the rev-4-pinned original**
- `.../born_crad_dj14relbin_detints2prime_..._2026-10-10/provenance/canonical_merge_block_extracted.py` (mtime 2026-10-09 01:22, `cp -p` from the above)
- bgsub candidates (both): `provenance/canonical_merge_block_frozen_source.py` — byte-identical to the relbin-1009 original (sha256 `8dedd657...`)
- bgsub candidates (both): `provenance/canonical_merge_block_extracted.py` — verbatim re-extraction with an added drift-note header (sha256 `9e974386e1c2a4cfc4f8dae284ca9ac197fd2053a880dbed0703e088118eb935`); same block1/block2 pins

### 1.4 House-rule check (registry copies md5-identical)

**INTACT for both plotters.** cRadApplied: md5 `271e3437...` in both registry
trees; bgfracsub: md5 `34c49e7a...` in both. The 2026-10-10 drift edit was
applied symmetrically to both cRadApplied copies (mtimes 07:02:11, 0.25 s
apart). The two `RUNLIST.md` copies are also md5-identical (`3eee0d09...`, both
mtime 2026-10-10 07:04).

**However**, `RUNLIST.md` line 220 still pins the nominal plotter at md5
`01e20e93f46dc600acfbc5952016593f` ("md5 ... in both") and describes the
registered contract "reads the parent raw `ibu/cross_sections.csv`, applies the
table selected by `CRAD_TABLE_KEY`" — that pin and contract are now **stale**
for the live copies (current md5 `271e3437...`; the live script reads the
already-Born release and applies no C_rad multiplication).

---

## 2. Exact drift characterization (live vs frozen, nominal plotter)

Diff basis: frozen copy #5 (sha256 `89801128...`, 2026-10-07 18:36) vs live
copy #1 (sha256 `4ee97d7a...`, 2026-10-10 07:02:11). All differences were
introduced 2026-10-09/10; the merge-block change itself is part of the
2026-10-10 edit.

**(a) The C_rad multiplication was REMOVED and replaced by a comment.**

Frozen (correct), lines 351–356:

```python
351: # Cross sections and all absolute nb uncertainties used by this plot scale as
352: # sigma_corrected = sigma_uncorrected * C_rad.  Relative uncertainties are
353: # dimensionless and therefore remain unchanged.
354: absolute_nb_columns = [column for column in ibu.columns if column.endswith('_nb')]
355: for column in absolute_nb_columns:
356:     ibu[column] = ibu[column] * ibu['C_rad']
```

Live (drifted), lines 372–374 (same location, after the kinematic-mismatch
asserts at live lines 369–370):

```python
372: # The selected input is already the installed Born release, so absolute nb
373: # values must not be multiplied by C_rad again.  The C_rad table remains a
374: # provenance/bin-alignment gate only.
```

**(b) The merge block now joins a budget column defined outside the block**
(live lines 313–316; `_budget_key` built at live lines ~184–190 from
`BUDGET_CSV` = `total_uncertainty_summary_with_phih_coherentprior_b1floor_crad13_releasebin_emfix.csv`,
column `total_with_crad13_rel`):

```python
313: ibu = ibu.merge(_budget_key, on=['charge', 'bin_index'], how='left', validate='one_to_one')
314: assert len(ibu) == 490 and ibu[BUDGET_COLUMN].notna().all(), \
315:     f"Budget join failed for {CRAD_TABLE_KEY}: require 490/490 matched bins"
316: assert np.isfinite(ibu[BUDGET_COLUMN]).all() and (ibu[BUDGET_COLUMN] >= 0).all()
```

**(c) Input repoint** (live ~lines 92–107): `RELEASE_DIR` changed from the
coherent-prior RAW candidate
(`.../data_release_candidates/bbb_ibu_coherentprior_..._2026-10-06`) to
`BORN_ROOT = .../output_v6/central/data_release/born_crad_dj14relbin_..._2026-10-09`
with comment "2026-10-10 user-directed band update: consume the installed Born
release tables directly"; `IBU_CSV` selects the
`alternative_scheme_rapgap31_relbin/` subdir when `CRAD_TABLE_KEY=RAPGAP31_relbin`.

**(d) Same-edit consequences:** `density_err` = `density * BUDGET_COLUMN`
(was `total_unc_nb / vol`, live ~line 397); `TOTAL_UNC_DISCLOSURE` rewritten to
the budget band; `CRAD_TABLE_KEY = os.environ.get("CRAD_TABLE_KEY",
"DJANGOH14_relbin")` (env-overridable; bgfracsub keeps a plain literal);
in-panel WIP styling (`WIP_FONTSIZE = 11`, bold, x=0.46 / map (0.03, 0.06), no
page titles); manifest header text updated.

**Registration block (block1) is INTACT and separate:** the live CRAD_CANDIDATES
map (live lines 117–146) carries the 2026-10-09 registration exactly as
recorded — `DJANGOH14_relbin` APPLIED PRIMARY
(`djangoh14_crad_releasebin_2026-10-09/...`, integrated h+ `1.1415172920054786`
/ h− `1.1410367377289135`), `RAPGAP31_relbin` FORMAL ALTERNATIVE
(EMFIX-REPOINTED 2026-10-09, `rapgap_crad_releasebin_emfix_2026-10-09/...`,
h+ `1.1342370401351234` / h− `1.1336689310022947`), xFcut entries marked
SUPERSEDED-F-APPLICATION 2026-10-09, quarantined blind entries unchanged. The
integrated-value runtime assert survived the drift (the C_rad merge/gate code
remains; only the multiplication loop is gone).

---

## 3. Timeline (best evidence; git unusable — paths untracked, repo stale since 2026-03-08)

| When (2026) | Event | Evidence |
|---|---|---|
| Oct 1 05:59 / 10:00 | Content later backed up as `_pre_repoint_2026-10-07` (multiplication present) | backup mtimes (preserved by `cp -p` on Oct 7) |
| Oct 7 | Repoint to coherent-prior release; "repaired canonical plotters" (labelled table key, integrated assert, per-key output dirs); backups created; frozen copies captured | frozen copies mtimes Oct 7 18:36 / 22:58; RUNLIST line 220-222 |
| Oct 7–8 | xFcut Born release built/installed; official Born review package `plots/systematics/born_crad13_2026-10-08/` (pre-drift producer) | RUNLIST/RUN_STATUS |
| Oct 9 01:22 | releasebin Born release (`born_crad_dj14relbin_..._2026-10-09`, cluster 5433949) built; full plotter sources + merge-block extract frozen into its provenance | provenance mtimes; RUN_STATUS |
| Oct 9 19:39:45 | **CRAD_CANDIDATES emfix registration update** applied identically to all four registry copies (bgfracsub md5 `34c49e7a...` in both — still current; nominal recorded at `d1c08b82...` at that stage); `py_compile` PASS; executed 19:40 / 20:16 (pyc) | bgfracsub mtimes; RUNLIST "2026-10-09 emfix repoint" entry (line ~232); RUN_STATUS 2026-10-10 item 3 (registration VERIFIED in all four copies) |
| Oct 9 evening | User-directed rendering-spec changes (in-panel WIP; nominal md5 `d6088571...` recorded) with re-renders of `born_relbin_canonical7panel_2026-10-09` | RUNLIST entries |
| Oct 10 ~06:36 | User-directed WIP-styling re-render (review PDF + 6 PNGs); producer state transient, md5 `ef6ea42d...` (recorded in the product manifest; **not preserved anywhere**) | product mtimes 06:36:19–22; manifest.json `producer_md5` |
| **Oct 10 07:02:11** | **THE DRIFT EDIT** — both live cRadApplied copies repointed to the Born release, budget band joined, C_rad multiplication removed (see §2). `RUNLIST.md` updated 07:02–07:04 in both trees (md5-identical): documents the WIP styling (line 241) and the band update (line 243) but NOT the multiplication removal; line 220 md5 pin left stale | live mtimes 07:02:11; RUNLIST mtime 07:04; diff §2 |
| Oct 10 07:02:16–07:02:50 | Drifted script EXECUTED (pyc 07:03): per-scheme re-render of `sidis_highQ2/plots/systematics/born_relbin_canonical7panel_2026-10-09/` (both scheme dirs) + manifest/README | product mtimes; pycache |
| Oct 10 later | S2' promotion: `born_crad_dj14relbin_detints2prime_..._2026-10-10` installed (cluster 5482794, in-release builder, no plots shipped); frozen merge-block extract copied into its provenance (`cp -p`); STATE.md/RUN_STATUS ledger item 8 records "the live canonical plotter was repointed 2026-10-10 to read the installed central Born release directly" | RUN_STATUS §S2'; provenance mtimes |
| Oct 10 23:06 → Oct 11 01:46 | bgsub set-R build session: packager (rev-1 of the chain, rev-4 Born-builder machinery) sources the merge blocks from the FROZEN provenance copy, sha256-pinned and runtime-asserted (block1/block2 pins; block2 must retain the C_rad multiplication); DRYRUN candidate validated 13/13 PASS (validation.json 2026-10-11 00:24); drift note + live sha256 `4ee97d7a...` recorded in candidate validation.json / README / manifest / inputs ledger / gate PM-2 protected-tree ledgers; official candidate validation 01:43 | study tree; `package_bgsub_setr_born_release_v1.py` lines 93–105, 233–284 |

**Attribution (best evidence):** the drift was introduced by the 2026-10-10
early-morning user-directed session (band update + WIP styling + data-vs-MC
plotter promotion; `MC/` registry dir touched 07:02, RUNLIST updated 07:04). It
was a deliberate, internally-commented repurpose of the live plotter for
direct Born-release rendering — documented at ledger level (STATE.md/RUN_STATUS
item 8) — but it silently broke the registered merge-block contract
(`sigma_Born = sigma_raw × C_rad`) still pinned in `RUNLIST.md` line 220, and
it was applied without updating that pin. **The 2026-10-09 registration session
is exonerated:** its edits are intact, md5-symmetric across all four copies,
and demonstrably separate from the drift — the bgfracsub plotter received only
the registration edit (diff vs its frozen copy is registration-only, 45 diff
lines) and still applies C_rad; the multiplication removal appears only in the
cRadApplied copies, in the later Oct 10 07:02 edit.

---

## 4. Impact scan (consumers of the live plotter after the drift)

**Product built with the drifted merge block: YES — exactly one plot product.**

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/born_relbin_canonical7panel_2026-10-09/`
— per-scheme outputs (`DJANGOH14_relbin/`, `RAPGAP31_relbin/`), `manifest.json`,
`README.md`, all written 2026-10-10 07:02:16–07:02:50 by the drifted live
script (run started 5 s after the 07:02:11 edit). The 06:36 review PDF
(`born_cross_section_review.pdf`) + `png/` pages were re-rendered the same
morning from a transient state (manifest `producer_md5` `ef6ea42d...`).

Nature of the contamination — **numeric values NOT mislabeled, provenance
compromised**:
- The run's input was the already-Born release, so the plotted values ARE
  Born-level: the plotter's own manifests record 490/490 matched bins,
  `max_relative_deviation 0.0` vs the release tables, and the integrated-C_rad
  runtime assert still passed. No raw-labeled-as-Born numbers were rendered.
- Provenance defects: (i) the top-level `manifest.json` records
  `producer_md5: ef6ea42d1d62502e9c7a450219b11098`, which matches **no
  preserved copy** of the script (not the live `271e3437...`, not any frozen or
  backup copy) — the render-time producer bytes are unrecoverable; (ii) the
  per-scheme `plot_manifest_nominal_no_bgsub.json` still carries the stale
  self-description `inputs.crad_formula = "sigma_corrected = sigma_uncorrected *
  C_rad; all absolute *_nb uncertainties scale by C_rad"`, which the drifted
  code no longer performs; (iii) the product's band basis
  (`emfix_2026-10-09` budget dir) is already a recorded pending-regeneration
  item (pre-S2' basis; RUN_STATUS pending user decision (a); S2'-basis
  Born-plot regeneration is ledger item 8).

**No other product consumed the drifted block:**
- bgsub set-R chain: the packager extracts the merge blocks from the FROZEN
  provenance copy (sha256 `8dedd657...`, block pins asserted at runtime,
  explicit assert that block2 retains the C_rad multiplication) and records the
  live script's sha256 only as as-found audit material. DRYRUN and official
  candidate validations PASS (13/13). The drift is recorded, not consumed. Gate
  PM-2 ledgers hash the live script before/after to prove it was not modified
  by the packager.
- The S2' Born release (2026-10-10) was built by its own in-release builder
  (cluster 5482794); no plots ship in it.
- No Condor submit files reference the plotter (no production submission of the
  live plotter post-drift); the Oct 10 renders were login-node interactive
  renders.
- Registry-document impact: `RUNLIST.md` line 220's md5 pin (`01e20e93...`) and
  raw-input contract are now false for the live copies (both trees) — the
  registry's own integrity check would flag the live files as diverged from
  their recorded pin.

---

## 5. Blockers and limitations

- Git cannot date or diff any of this: repo last commit 2026-03-08; both
  registry trees untracked. Timeline rests on mtimes, backups, frozen copies,
  and registry docs.
- Transient plotter states recorded only as md5 literals in `RUNLIST.md`
  (`d1c08b82...` Oct 9 emfix stage, `d6088571...` Oct 9 rendering stage,
  `ef6ea42d...` Oct 10 06:36 render stage) are not preserved as files; the
  06:36 render's exact producer bytes are unrecoverable.
- Whether the 06:36 transient state already contained the repoint/no-multiply
  cannot be determined from preserved bytes; the 07:02:16–07:02:50 renders are
  positively drifted-state.
- The Mac-origin registry (MANIFEST `source_path` values) is not reachable from
  NAF; no statement is made about copies off-NAF.
- A concurrent bgsub session was completing the official chain during this
  investigation (study dir still changing at 01:43–01:46 Oct 11); findings for
  that study reflect the artifacts as of this writing.

## 6. Chain of evidence (hash pins)

- Live drifted nominal plotter (both registry copies): sha256
  `4ee97d7a51625b5d2bc4eca751dbb487e76c156f6e82ef8b794691b93c295e99` — matches
  `canonical_script_original_sha256` in both bgsub candidate `validation.json`
  files and the gate PM-2 protected-tree ledgers.
- Frozen merge-block file (relbin-1009 provenance; identical copies in
  xfcut-1007, detints2prime-1010, both bgsub candidates'
  `canonical_merge_block_frozen_source.py`): sha256
  `8dedd657c9df53e5b4319dbdae070a5425067035bf6751489cdc500427ab0e53`; block1
  `819eb2ba...`, block2 `351bbac2...` — matches the pins in the rev-4
  validation record and the packager asserts.
- Frozen full plotter sources (both Born releases' provenance): sha256
  `89801128...` (nominal, md5 `01e20e93...` — the md5 still pinned, now
  stalely, by RUNLIST line 220) and `7459b372...` (bgfracsub, md5
  `ab87d5ef...`).

*Report generated 2026-10-11 by filesystem-forensics dispatch (read-only; no
analysis files modified).*

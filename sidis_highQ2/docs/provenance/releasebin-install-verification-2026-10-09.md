# Releasebin C_rad promotion — install verification record (2026-10-09)

- **Scope:** filesystem-only promotion install of the 2026-10-09 release-safe rederived
  (releasebin) C_rad chain — Born release install, protected-tree verification, budget
  status promotion, quarantine-baseline doc refresh. No physics recomputation; no candidate
  artifact rewritten.
- **Gates binding this step (all passed 2026-10-09):**
  - Critic: **PASS WITH CONDITIONS** — `sidis_highQ2/docs/critiques/critic-gate-releasebin-promotion-2026-10-09.md`
  - Normalization: **PASS WITH CAVEAT** — `sidis_highQ2/docs/status/normalization-gate-releasebin-2026-10-09.md`
  - QA: **PASS WITH CONDITIONS** — `sidis_highQ2/docs/qa/qa-gate-releasebin-2026-10-09.md`

## 1. Install method

- **Source (intact, retained as promotion provenance):**
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release_candidates/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
  (66 files; built and validated by Condor cluster 5433949, `validation/validation.json`
  14/14 checks pass, per the QA gate).
- **Destination (dated directory, fresh install):**
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
- **Method:** archive-mode recursive copy (`cp -a`) of the candidate into the dated
  destination, with checksum verification before and after (below). No files were added,
  removed, or edited inside the destination after the copy: the installed release remains
  `diff -rq`-identical to the candidate, which is why this record lives in
  `sidis_highQ2/docs/provenance/` and not inside the release.

## 2. Checksum results

- Pre-copy source verification: manifest `manifest.json` (schema `manifest_v1`, 46 product
  files) re-verified at the source — **46/46 sha256 + byte-size OK**.
- Post-copy destination re-verification: the same 46 manifest hashes re-verified at the
  destination — **46/46 OK, zero mismatches**.
- (Manifest intentionally does not list `logs/` and `status.json`, declared volatile run
  artifacts; every product file is covered.)

## 3. Diff and inventory result

- `diff -rq` source vs destination: **ZERO differences** (exit 0).
- File count: **source 66 = destination 66** (48 non-log product files + 18 `logs/`,
  matching the QA gate's inventory-parity check).

## 4. Protected-tree results (verified byte-identical before and after the install)

Full-tree sha256 sweeps of both protected trees were taken immediately before the copy and
repeated after the install; the before/after hash lists are identical.

- **Raw release (canonical RAW product, unchanged):**
  `.../data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`
  — 71 files, 71/71 hashes identical before/after; integrated IBU h+
  `11.483372048266185` nb / h− `11.190382714709287` nb (canonical `11.483372` /
  `11.190383`) confirmed unchanged on disk.
- **Old Born release (2026-10-07 xFcut basis; stays in place untouched as provenance — not
  moved, not renamed):**
  `.../data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/`
  — 68 files, 68/68 hashes identical before/after.

## 5. Supersession statement (this promotion)

- **Current Born basis:** the installed
  `.../data_release/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
  (`c_rad_applied=true`, applied primary DJANGOH14 releasebin
  `djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`,
  sha256 `cf810be44a59ed790ba78e0476eb8f59df96358357cc39e258e034ed31b22ef5`; RAPGAP31
  releasebin = formal alternative scheme).
- **Current budget basis:** `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/`
  — README status flipped from CANDIDATE to INSTALLED CANONICAL BUDGET BASIS (2026-10-09)
  and a dated promotion note (three gate verdicts + record paths) appended to its
  `provenance.json`. No CSV or covariance content altered.
- **Superseded-for-application, retained in place untouched as provenance:**
  - `.../data_release/born_crad_dj14xfcut_..._2026-10-07/` (old Born release)
  - `.../summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_2026-10-07/` (old 13-component budget)
  - `sidis_highQ2/data/radiative/djangoh14_crad_final_xfcut_2026-10-05/` and
    `sidis_highQ2/data/radiative/rapgap_crad_final_xfcut_2026-10-05/` (old applied xFcut tables)
- **Raw release unchanged** as the canonical RAW product (and RAW PARENT of both Born releases).
- Promotion is NOT publication clearance; carried-open caveats travel unchanged
  (`P-CRAD-LUMI` exposure provenance `DIAGNOSTIC_MANIFEST_LEVEL`; EMPZ>45 convention decision
  open; mixed-beam-only validity; raw/no-bgsub basis with response fake-fraction subtraction
  only; per-bin definition sensitivity confined to the z<0.05 row).

## 6. Filename-collision rule (critic gate condition 4)

The releasebin CSVs carry filenames identical to the old xFcut CSVs
(`crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` in both the 2026-10-05 xFcut and the
2026-10-09 releasebin campaign directories); only the parent directory distinguishes them.
**Every consumer must use directory-qualified paths.** Any future rederivation should
disambiguate by filename or sidecar marker. The budget README/quarantine baseline note
likewise use directory-qualified paths.

## 7. Annotations (campaign outputs NOT rewritten, per critic gate)

- **Critic condition 3 — stale provenance strings in the new DJ14 releasebin
  `crad_v3_summary.json`** (`sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/`):
  it carries the stale template-inherited `"contract": "crad_djangoh14_xfcut_fork_2026-10-05"`
  string and a DEAD pre-quarantine `blind_final_reference` path
  (`data/radiative/esigma_photon_variant_2026-09-26/empz45_nominal/`, now quarantined).
  Annotated here; the campaign output was NOT rewritten. The authoritative table provenance
  is `campaign_manifest.json` + `crad_djangoh14_releasebin.json` (both correct).
- **Critic condition 5 — stale docstring:** both copies of `finalize_releasebin.py` (DJ14 and
  RG31 releasebin campaign dirs) claim consumption of xFcut-worker partials; the actual
  worker is `crad_v3_worker_releasebin.py`. To be corrected on next touch; annotated here.

## 8. Documentation updates executed in this promotion step

- Budget README status line + supersession note flipped to installed canonical
  (`total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/README.md`);
  dated promotion note appended to its `provenance.json` (three gate verdicts + record paths).
- Stale baseline sentence refreshed in
  `sidis_highQ2/data/radiative/quarantine/README.md`: the releasebin basis (2026-10-09) is
  now the applied baseline; the xFcut tables are superseded-for-application, retained as
  provenance. All quarantine entries left intact.

## 9. Outstanding follow-ups bound by gate conditions (NOT executed in this filesystem step)

- **STATE.md update** (QA condition 1): last updated 2026-10-08; still records the xFcut
  basis as current — must record the releasebin Born release and budget as installed current.
- **Plotter table registration** (critic condition 2 / QA condition 2): the canonical
  plotters' `CRAD_CANDIDATES` map still registers only the xFcut tables; a recorded
  registration decision (add releasebin entries; mark xFcut superseded-for-application) is
  required BEFORE any Born-level plot regeneration on the new basis.
- **Downstream supersession** (critic condition 1): `data_vs_mc_z005_4generator_born_2026-10-08`
  package and analysis-note Born totals/figures still embed the xFcut tables and must be
  repointed/superseded.
- **Analysis-note text fixes** (QA condition 2 / critic condition 6): `results.tex` /
  `systematics.tex` "no radiative correction applied" claims remain open and are now doubly
  stale.
- **Per-file exposure manifest** (normalization caveat 1, non-blocking): the releasebin
  campaign dirs ship only stub per-file manifests; copy or reference the 2026-10-05 per-file
  `input_output_exposure_manifest.csv` from releasebin provenance if the releasebin tables
  become the long-term applied basis.

## 10. Verification artifacts

- Local (login-node) verification scratch: `/tmp/opencode/releasebin-promo-2026-10-09/`
  (source/destination manifest checks, `diff -rq` log, before/after protected-tree sha256
  sweeps). Ephemeral; this record is the durable evidence.

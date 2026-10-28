# C_rad Application Promotion Record — 2026-10-07 (`c_rad_applied=TRUE`, z005)

**Promotion events:** 2026-10-07 (user decision). **Record completed:** 2026-10-08 (10-step procedure, steps 0–5 in a prior dispatch, steps 6–10 in the registry/skills dispatch).
**Authority:** `STATE.md` > `docs/RUN_STATUS.md` > `docs/status/current-release.md` / `docs/status/radiative-corrections.md` > `context-handoff.md`. This file is the full promotion record; it does not override them.
**Owner (registry):** `sidis-hq2-radiative` (row in `docs/skills_build/ownership.tsv`).
**Provenance invariant:** historical documents, the 2026-06-10 release, the coherent-prior release and its candidate, quarantined blind tables, every `C_rad` table, the Born release's in-release files, and budget directories are untouched. Nothing was deleted; supersessions are named, never silent.

---

## 1. Decision and Definition

- `c_rad_applied=TRUE` (user decision 2026-10-07; supersedes `c_rad_applied=false`).
- Applied correction: `sigma_Born(bin) = C_rad(bin) x Unfold(data; R)` — **post-unfold, gen-level, per-bin**.
- **APPLIED PRIMARY:** DJANGOH14 xFGen<0 xFcut EMPZ>45, `data/radiative/djangoh14_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (integrated h+ `1.1418534312056339` / h− `1.1413755529189475`). Table sha256 `cab01f2631ed5069d508c298d94a9d4ede75ade2d46b811425fdbbad6cac53f1`.
- **FORMAL ALTERNATIVE-SCHEME variation:** RAPGAP31 xFcut, `data/radiative/rapgap_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (h+ `1.1368044552974204` / h− `1.1362373358660744`); cross-generator agreement `+0.444%` (h+) / `+0.452%` (h−), carried as `c_rad_gen_rel`.
- **Born release (CURRENT, installed 2026-10-07):** `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/` — 68 files, checksum-verified copy, `diff -rq` vs the intact source candidate = ZERO differences. Integrated (read from its `summary_totals.json`, never derived): IBU h+ `13.150141284110202 nb` / h− `12.8138695166339 nb`; BBB h+ `13.115633874580633 nb` / h− `12.783718063600624 nb`. Yields stay RAW (**CHOICE A**: `yield = cross_section_nb x lumi_nb` deliberately broken). Its `plots/` ships only a README by design.
- **RAW PARENT:** the coherent-prior raw release `bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/` remains byte-identical in place — canonical RAW product and provenance of the Born release.
- **Invariant under the flip:** data vector, response, migration, acceptance, IBU `k=4` raw output, and all RELATIVE systematic products; only absolute nb products rescale. `L_data=351000.0 nb^-1` unchanged; `lumi_rel` (flat 2.7%) and `c_rad_stat_rel` are DISJOINT.
- **Quote rule:** quoting a cross section without saying Born-or-raw is AMBIGUOUS AND FORBIDDEN; every quote states prior basis AND Born-vs-raw level.

## 2. Step-2 Category Scan

| Category | Object |
|---|---|
| release | Born release installed alongside the intact raw parent |
| radiative | C_rad applied (definition, primary/alternative tables, DEAD path, retired floor) |
| systematics | 13-component crad13 budget supersedes the 11-component b1floor basis |
| results | Born-or-raw two-level quoting rule; Born integrated values; CHOICE A |
| plotting | two registered Born plotters; Born plot regeneration pending at promotion time — COMPLETED 2026-10-08 (`plots/systematics/born_crad13_2026-10-08/`, documented-deviation study plotter; see §12 item 1) |
| operations/registry | RUNLIST / SEVEN_CATEGORY_SCRIPT_INDEX override + DENY section |

Affected skills: `sidis-hq2-radiative`, `sidis-hq2-systematics`, `sidis-hq2-state`, `sidis-hq2-results`, `sidis-hq2-plotting`, `sidis-hq2-baseline-update`, `sidis-hq2-lessons`.

## 3. Files Touched — Dispatch 1 (steps 0–5)

- Born release build + install: `output_v6/central/data_release/born_crad_dj14xfcut_..._2026-10-07/` (68 files; source candidate intact in `data_release_candidates/`).
- Canonical budget: `output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_2026-10-07/` (13 components; registered, never modify).
- Decision plots: `plots/systematics/crad_promotion_decision_2026-10-07/`.
- Studies: `docs/systematics/CRAD_STATFLOOR_STUDY_2026-10-07.md` (+ json), `docs/radiative/CRAD_UNC_GENERATOR_SPREAD_2026-10-07.md` (+ json).
- Born plotters repaired/repointed in both registry copies with `*_pre_repoint_2026-10-07` backups (remote bundle `release/scripts/` only).
- Five state docs flipped: `STATE.md`, `docs/RUN_STATUS.md`, `docs/status/radiative-corrections.md`, `docs/status/current-release.md`, `context-handoff.md`.

## 4. Files Touched — Dispatch 2 (steps 6–10)

- `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py` — remote stale copy synced from local (see §6).
- `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/RUNLIST.md` and the local snapshot copy `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/RUNLIST.md` — both edited/synced, md5-identical `7024d5b7d6bdf5d8ef793689b5bf829a` (226 lines). Edits: Born release in "Canonical release target"; raw release re-scoped as RAW PARENT; Phase 3 crad13 current + b1floor superseded; Phase 4 label reversal + application status; Phase 7 prior-basis AND Born-or-raw rule; DENY section rewritten (new denials: `c_rad_applied=false` claims, DEAD `empz45_nominal` path, retired floor / VOID figures, blind tables, `cRad_bgfracsub` merge, Born-vs-RAD-MC); new section "2026-10-07 c_rad promotion registry override — CURRENT" (plotters + md5s, backups, divergence-closure, reproduction order, gate evidence, carried-open blockers).
- `SEVEN_CATEGORY_SCRIPT_INDEX.md` (both copies) — merged (4 local-only diagnostic rows added, categories 5/6/7 fixed, c_rad override added), md5-identical `cec59d6b03513acf7133ff90acf3e80b`.
- Seven `SKILL.md` files (see §7).
- `docs/skills_build/ownership.tsv` — 9 rows appended (462 → 471 lines).
- `docs/skills_build/COVERAGE_MANIFEST.md` — counts updated by added lines only: radiative 11→15, systematics 21→24, results 63→65.
- This record (`docs/radiative/CRAD_APPLICATION_PROMOTION_2026-10-07.md`).

## 5. Superseded Values (old -> new)

1. `c_rad_applied=false` -> `c_rad_applied=TRUE`; `sigma_Born(bin) = C_rad(bin) x Unfold(data; R)` (post-unfold, gen-level, per-bin).
2. RAPGAP31 "nominal" / DJANGOH14 "cross-check" -> **DJANGOH14 xFcut APPLIED PRIMARY** (h+ `1.1418534312056339` / h− `1.1413755529189475`) / **RAPGAP31 xFcut FORMAL ALTERNATIVE** (h+ `1.1368044552974204` / h− `1.1362373358660744`).
3. 11-component b1floor budget (totals h+ `5.684%`/`160.613%`, h− `6.095%`/`84.748%`) -> 13-component crad13 budget (totals h+ `5.713%`/`160.627%`, h− `6.150%`/`84.772%`; deltas `+0.0291 pp`/`+0.0555 pp`), adding `c_rad_gen_rel` (medians h+ `0.5298%`/h− `0.5009%`, max `5.872%`/`5.834%`) and `c_rad_stat_rel` (medians h+ `0.4272%`/h− `0.4480%`, max `4.472%`/`6.123%`).
4. `max(half-difference, u_diff)` statistical floor -> RETIRED in regime B (double-counts the applied DJANGOH14 leg); replaced by `c_rad_stat_rel` from the applied table's `relative_stat_unc`.
5. `+0.077 pp`/`+0.021 pp` total impact and V4(1.0)-blind recommendation (`CRAD_STATFLOOR_STUDY_2026-10-07.md`) -> VOID (priced the retired floor on the superseded 2026-10-02 budget base and the blind pairing).
6. Pre-quarantine path `data/radiative/esigma_photon_variant_2026-09-26/empz45_nominal/...` -> DEAD; repoint to the xFcut tables (primary/alternative above).
7. Blind 2026-09-30 tables remain quarantined (under-correct `+0.35/+0.38%` RAPGAP31 / `+0.37/+0.41%` DJANGOH14 integrated; bias confined to z<0.05).
8. Old Born trap ("do not apply C_rad; release remains raw") -> REVERSED (marked SUPERSEDED in `sidis-hq2-radiative`).
9. Raw release scope: canonical central release -> canonical RAW product + RAW PARENT/provenance of the Born release (byte-identical, untouched).

## 6. Conflicts Found and Fixed

- **bgfracsub plotter registry divergence (found + fixed):** remote copy stale md5 `3e93843a96b931d1f583f870722449a0` vs local `ab87d5efccfe05ccb6094ec516900677` (= Born release archive `provenance/canonical_plotter_crad_bgfracsub_source.py`). Synced remote <- local; both copies now identical. Recorded in the RUNLIST override.
- **SEVEN_CATEGORY_SCRIPT_INDEX divergence:** remote had the flatphi section + coherent-prior override; local had 4 diagnostic rows remote lacked. Merged into remote, then synced local <- remote; both `cec59d6b03513acf7133ff90acf3e80b`.
- **Backup-location divergence (recorded, by design):** `*_pre_repoint_2026-10-07` backups exist only in the remote bundle `release/scripts/`; the local snapshot holds only the two repaired plotters. Closure recorded in the RUNLIST override.
- **Pre-existing registry count lag (noted, NOT fixed by this promotion):** `COVERAGE_MANIFEST.md` systematics count lagged `ownership.tsv` before these edits (21 vs 28). Manifest counts were updated only by the lines this promotion added; tsv counts now radiative 15, systematics 31, results 66.

## 7. Skills

**Updated (7), all with explicit WRONG/RIGHT supersessions, never silent deletion:**
- `sidis-hq2-radiative`: APPLIED baseline AF; label reversal with full-precision integrated values; superseded `c_rad_applied=false` claim; Procedures 1 & 10 superseded; old Born trap REVERSED; +3 traps; +3 Canonical Ref rows; ownership labels; c_rad addendum.
- `sidis-hq2-systematics`: 13-component AFs (medians/maxes, totals, deltas); C_rad menu row; 3 traps; crad13/b1floor/statfloor ref rows; old addendum budget marked superseded; c_rad addendum.
- `sidis-hq2-state`: RAW PARENT + Born release AFs; Version History +5 rows; `P-CRAD-LUMI` updated, new `P-CRAD-APPLIED` row with the eleven carried-open blockers, `P-RAPGAP` updated; Release/Radiative topic snapshots; 3 traps; ref rows; c_rad addendum.
- `sidis-hq2-results`: Born release AFs with exact values + CHOICE A; two-level status AF; Procedure 5 rewritten; 3 traps; +2 ref rows; c_rad addendum.
- `sidis-hq2-plotting`: raw-vs-Born plot AF; +2 inventory rows with md5s; new Procedure 5 (renumbered 5–9); +3 traps; c_rad addendum.
- `sidis-hq2-baseline-update`: baseline anchor -> Born + crad13; old addendum budget marked superseded; c_rad addendum.
- `sidis-hq2-lessons`: AF; regime-A-to-regime-B floor-retirement trap row; Methodology Decisions + C_rad application row; c_rad addendum.

**Verified unaffected (8):** `sidis-hq2-backgrounds` (alpha-veto guard still true), `sidis-hq2-unfolding` (zero hits — flip-invariant), `sidis-hq2-normalization` (`L_data` disjointness still true), `sidis-hq2-mc`, `sidis-hq2-aziasym` (BSA fork legitimately stays raw), `sidis-hq2-docmap`, `sidis-hq2-builder`, `sidis-hq2-operations` (zero hits). Additionally verified live-correct this session: `sidis-hq2-results` references (`release-lineage.md`, `schema.md`, `note-and-paper.md` — RAW-product-level statements and the recorded paper-draft conflict) and `sidis-hq2-mc` trap line (RAPGAP31-not-nominal, consistent with the reversal).

## 8. Ownership Registration (step 8)

Nine rows appended to `docs/skills_build/ownership.tsv` (462 → 471 lines): statfloor md+json and the crad13 budget dir -> `sidis-hq2-systematics`; generator-spread md+json, the decision-plots dir, and this promotion summary -> `sidis-hq2-radiative`; the Born release and its candidate -> `sidis-hq2-results`. Canonical References rows were added in the owning skills during step 7. `COVERAGE_MANIFEST.md` section counts updated by these lines only (radiative 11→15, systematics 21→24, results 63→65). Three other 2026-10-07 radiative docs (`BRITZGER_EW_gZ_NOZ_METHOD_REVIEW`, `EMINUS_CRAD_INVESTIGATION_AND_BLOCKER`, `NOZ_PROVENANCE_INVENTORY`) were deliberately NOT registered (outside the promotion's 9-item list).

## 9. Gate Evidence Chain (2026-10-07)

- Critic gate: **PASS WITH CONDITIONS** — seven questions answered, five must-fix items all executed.
- Normalization verdict: **PASS WITH CAVEAT**.
- Independent QA: **PASS WITH CONDITIONS** — all four conditions closed; validation **14/14 PASS**.
- Residual parse-convention note: `9.716e-13`/`9.103e-13` relative in the plotter parse universe (documented residue, not a blocker).
- Install verification: `diff -rq` vs the intact source candidate = ZERO differences (68 files).

**Born-product reproduction order (recorded in the RUNLIST override):** (1) raw parent in place -> (2) applied table validated 490/490 -> (3) in-release `scripts/build_born_candidate.py` (CHOICE A, rev-4 exact-text) -> (4) checksum-verified install, candidate intact -> (5) Born plots ONLY via the two registered plotters (per-key output dirs; NOT yet run at promotion time — the official Born cross-section review package `plots/systematics/born_crad13_2026-10-08/` completed this step 2026-10-08 via the documented-deviation study plotter `generate_official.py`; see §12 item 1) -> (6) quote totals from crad13 with a Born-or-raw statement.

## 10. Blockers Carried OPEN (eleven, verbatim; do not close)

1. `P-CRAD-LUMI` exposure provenance is `DIAGNOSTIC_MANIFEST_LEVEL` and C_rad is NOT publication-grade.
2. The xFcut baseline is PROVISIONAL and its review closed only as a promotion gate, not as publication clearance.
3. The EMPZ>45 convention decision is open and a future flip supersedes the applied table.
4. The DJANGOH e- RAD `4.67%` count shortfall is bounded but unresolved.
5. The release stays raw/no-bgsub with response fake-fraction subtraction only at `10.7141%` (h+) / `10.4019%` (h-).
6. Per-bin definition sensitivity up to `+/-6.8%` / `+/-3.9%` sits in the z<0.05 row.
7. The correction is mixed-beam only, not valid for any per-beam or e- Born target.
8. Born values must not be compared against RAD-level MC predictions without the same correction.
9. `cRad_bgfracsub` stays a separate derived variant and must not be merged into the nominal.
10. `P-PLOTS`, `P-ALPHA`, `P-METHOD` remain open.
11. The analysis note text at `sections/results.tex:103` and `systematics.tex` still claims no radiative correction is applied and is now FALSE, pending a note update.

(Source: `docs/status/current-release.md` §"c_rad Application — Blockers Carried OPEN", lines 69–79.)

## 11. Grep Pass (step 9) — Classes and Counts

Scope: `STATE.md`, `context-handoff.md`, `docs/**/*.md`, `.opencode/skills/**/*.md` (whole-workspace greps excluded — output trees; the document surfaces are the task scope). Classes: **A** supersession marker / live-correct statement in the five flipped state docs or seven edited skills; **B** legitimately-raw side-product/side-study statement (BSA/AziAsym fork, v4-native runbook, DIFFVM study, RAW-release schema/lineage references) — true by design, untouched by the central promotion; **C** historical dated record (immutable provenance: dated session records, plans, specs, critiques, handoffs, roadmaps, provenance docs, archived copies); **D** audit-only (pre-flip `note_discrepancy_review_2026-10-07` records, protected `CRAD_STATFLOOR_STUDY`, `CRAD_UNC_GENERATOR_SPREAD`, `EMINUS_CRAD_INVESTIGATION`, `OPEN_CONFLICTS_2026-09-08`, the SUPERSEDED-banner 2026-10-05 chain plan, the self-declared HISTORICAL `reference-highQ2-crad-availability.md`); **E** unrelated coincidental match; **F** live contradiction (fixed). Line-hit counts:

| Token sweep | Hits | A | B | C | D | E | F |
|---|---|---|---|---|---|---|---|
| `c_rad_applied` (incl. `c_rad,applied`) | 111 | 61 | 0 | 19 | 31 | 0 | 0 |
| RAPGAP31-"nominal" / DJANGOH14-"cross-check" labels | 54 | 13 | 3 | 25 | 13 | 0 | 0 |
| `esigma_photon_variant_2026-09-26` / `empz45_nominal` / `rapgap_crad_final_2026-09-30` blind+dead paths | 31 | 14 | 0 | 13 | 4 | 0 | 0 |
| retired floored form `max(half-difference, u_diff)` | 23 | 23 | 0 | 0 | 0 | 0 | 0 |
| VOID `+0.077`/`+0.021` pp figures | 27 | 18 | 0 | 0 | 4 | 5 | 0 |
| `not Born` | 58 | 14 | 7 | 25 | 12 | 0 | 0 |
| `no C_rad` (incl. `no-`C_rad`` / ``no `C_rad``) | 73 | 17 | 11 | 38 | 7 | 0 | 0 |
| **Total** | **377** | **160** | **21** | **120** | **71** | **5** | **0** |

**Zero live contradictions; zero fixes required.** Notable classifications: the generic `half-difference` convention for OTHER sources (`ibu_model_rel`, `ibu_reg_rel`, electron/HFS shifts) is NOT retired — only the C_rad floored form is; `docs/LOOKUP.md`, `docs/references/reference-highQ2-charged-sidis-analysis-cuts.md`, and `docs/methodology/systematics_data_contract_v3.md` label hits are unfolding-response/quarantine contexts unaffected by the C_rad label reversal; `docs/ANALYSIS_WORKFLOW_ROADMAP.md` is a dated (2026-06-24) snapshot that defers authority to `RUN_STATUS.md` (historical, consistent with its treatment in the earlier coherent-prior promotion); `results` skill references describe the RAW product level, which remains true.

## 12. Remaining Actions

1. **DONE 2026-10-08 — official Born plot regeneration** (supersedes the original open item: "Born 7panel/x-Q2 official plot regeneration NOT yet run — use ONLY the two registered plotters, per-key output dirs; do not copy raw-parent plots into the Born release"). COMPLETED as the official Born cross-section review package `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/systematics/born_crad13_2026-10-08/` — 9-page `born_cross_section_review.pdf` with 9 PNGs in `png/` and refreshed `manifest.json`, visually accepted 2026-10-08 after three review rounds. Composition: page 1 compact summary; pages 2–5 DJANGOH14_xFcut PRIMARY and pages 6–9 RAPGAP31_xFcut ALTERNATIVE, each scheme giving h+ and h− 7-panel, x-Q2 bin map, and C_rad band-width-difference views (schemes separated by directory; table-key overwrite guard verified). Bands: 13-component crad13 total INCLUDING `c_rad_gen_rel` and `c_rad_stat_rel` (primary band), 11-component total as gray dashed reference, C_rad-only contribution as red dotted element, plus dedicated per-bin absolute band-width-difference pages; the per-bin plotted total reproduces the budget CSV over all 490 bins at max deviation `0.0`. Producer: study plotter `generate_official.py` in that directory — a DOCUMENTED deviation from the two registered canonical cRad plotters (systematic-decomposition view the release plotters do not provide), justified in its `manifest.json` and README. Status: BORN-LEVEL PROVISIONAL, INTERNAL REVIEW ONLY, not physics-final; no theory/QCD/TMD comparison. No raw-parent plots were copied into the Born release (its `plots/` still ships only a README by design); the two registered plotters remain the canonical Born release 7panel/x-Q2 entry points. Rendering trap recorded for future plot work: every title beginning with the word "Born" lost its leading glyph through the font + `pdftoppm` rasterization path despite a correct PDF text stream; titles were reworded so "Born" is never the first word and `pdf.fonttype=42` was set.
2. **S3 xFcut sidecar re-attach** — only if the 2026-10-05 bgsub chain is ever reused (its sidecars still carry the blind C_rad; superseded for physics by the installed Born release).
3. **Note text** — `sections/results.tex:103` and `systematics.tex` still claim no radiative correction is applied (FALSE); update pending.
4. `P-CRAD-LUMI`: trace MC exposures to MCDB/generator logs before any publication claim.
5. EMPZ>45 convention decision remains open; a future flip supersedes the applied table.
6. Registry hygiene (pre-existing, optional): reconcile `COVERAGE_MANIFEST.md` systematics count (24) with `ownership.tsv` (31).

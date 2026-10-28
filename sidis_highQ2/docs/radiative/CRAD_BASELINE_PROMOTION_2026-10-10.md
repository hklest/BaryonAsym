# c_rad Baseline Promotion Record — 2026-10-10 (emfix budget + consolidated c_rad baseline release)

**Promotion event:** 2026-10-10 (user decision). **Record completed:** 2026-10-10 (ten-step checklist per `sidis-hq2-baseline-update`; steps 1–9 executed this session, step 10 = this record).
**Authority:** `STATE.md` > `docs/RUN_STATUS.md` > `docs/status/current-release.md` / `docs/status/radiative-corrections.md` > `context-handoff.md`. This file is the full promotion record; it does not override them.
**Owner (registry):** `sidis-hq2-radiative` (row in `docs/skills_build/ownership.tsv`).
**Provenance invariant:** historical documents, the raw central release and its candidate, the Born release and its candidate, all four budget directories, every `C_rad` table directory and sidecar, the quarantined blind tables, the consolidated release's source directories, `RUNLIST.md` (both copies), all registry plotter scripts, and all plot packages are untouched. Nothing was deleted; supersessions are named, never silent. All edits were direct file edits; **no git commits were made**.

---

## 1. Decision and Definition

- **User decision 2026-10-10 — two artifacts promoted together:**
  1. **CANONICAL SYSTEMATICS BUDGET** = the 13-component emfix budget `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/` — totals h+ median `5.7093%`/max `160.627%` (p95 `24.748%`), h− median `6.1523%`/max `84.775%` (p95 `33.856%`); `c_rad_gen_rel` emfix medians h+ `0.5844%`/max `5.762%`, h− `0.5804%`/max `5.663%`; `c_rad_stat_rel` UNCHANGED h+ `0.4272%`/max `4.408%`, h− `0.4429%`/max `6.036%`; the other 12 components carried BYTE-IDENTICAL (30 carried columns x 490 rows = `14,700/14,700` cells). Only `c_rad_gen_rel` and the per-bin totals containing it differ from the superseded 2026-10-09 releasebin budget.
  2. **CURRENT c_rad BASELINE REFERENCE** = the consolidated role-prefixed release `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_release_baseline_2026-10-10/` — 11 checksummed byte-identical copies (all SHA256/byte checks PASS) in four role subdirs: `applied_primary_djangoh14_releasebin/`, `formal_alternative_rapgap31_releasebin_emfix/`, `generator_spread_withz_emfix/`, `noz_cgamma_born_provisional_diagnostic/`; manifests `RELEASE_2026-10-10.md` + `release_manifest.json`. It is a **CONSOLIDATED REFERENCE, NOT A REPOINTING**: the consumed applied-primary canonical path STAYS `data/radiative/djangoh14_crad_releasebin_2026-10-09/`; Born-release manifests and plotter registrations are NOT repointed.
- **Applied primary (UNCHANGED):** DJANGOH14 releasebin `data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`, integrated h+ `1.1415172920054786` / h− `1.1410367377289135`; table sha256 `cf810be44a59ed790ba78e0476eb8f59df96358357cc39e258e034ed31b22ef5`.
- **Formal alternative scheme (current):** RAPGAP31 releasebin EMFIX `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/` — h+ `1.1342370401351234` / h− `1.1336689310022947`; emfix pair spread `+0.6419%` (h+) / `+0.6499%` (h−). Repair provenance: RG31 NONRAD e− exposure repair `+53,773.7 nb^-1` (missing chunk `D/em/C0000009`, tree `rapgap_nonrad_em_0104`) → `L_nonrad(em) = 13,470,764.7 nb^-1`; audit addendum `docs/radiative/CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md`.
- **NoZ C_gamma_born:** h+ `1.040960540463156` / h− `1.0401468080806677` — PROVISIONAL DIAGNOSTIC, unapplied (`c_rad_applied=false`), DJANGOH-only; proxy adopted κ=0.02 medians h+ `0.62225%`/max `5.77012%`, h− `0.61890%`/max `5.67399%` (κ=0.03 conservative variant `0.65807%`/`0.65283%` retained); mc-expert validation recorded in `cgammaborn_gen_unc_proxy_v3emfix.json`.
- **Born release (UNCHANGED CURRENT):** `output_v6/central/data_release/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/` — manifest 46/46 sha256, install VERIFIED; `c_rad_table_key: DJANGOH14_relbin`, subproduct `alternative_scheme_rapgap31_relbin/`; integrated IBU h+ `13.107258377622092 nb` / h− `12.77278197828459 nb`, BBB h+ `13.089458939808843 nb` / h− `12.759120412771287 nb`; yields stay RAW (**CHOICE A**).
- **Unchanged by construction:** `c_rad_stat_rel` (so `covariance_cradstat/` is carried forward byte-identical and NOT regenerated); the raw central release and its RAW PARENT role; `L_data=351000.0 nb^-1`; all relative non-crad products; the 12 carried budget components.
- **Quoting rule (carried):** every `C_rad` quote states table basis AND Born-vs-raw level; bare-filename quoting of any C_rad CSV is forbidden — directory-qualified paths mandatory.

## 2. Step-1 Artifact Verification and Step-2 Category Scan

**Step 1 (all PASS, verified on disk before promotion):** the emfix budget directory and its README values (totals, component medians/maxes, byte-identity counts, sha256); the consolidated release manifest (11/11 SHA256 + byte PASS across the four roles); the RG31 emfix directory + `emfix_validation.json`; the pre-emfix dirs carrying `README_SUPERSEDED_2026-10-09.md` (RG31 table dir) and `crad_gen_unc_v3_SUPERSEDED_2026-10-09.md` (DJ14 pre-emfix sidecar); the Born release `summary_totals.json` and manifest; `RUNLIST.md` line 232 in both copies; md5s of all four registry plotter copies. No missing artifact; promotion allowed.

**Step 2 categories:** radiative (consolidated baseline reference; emfix formal-alternative basis; supersession of pre-emfix tables and sidecars); systematics (canonical budget swap, single-component `c_rad_gen_rel` change); state/results (anchors, supersession lists, quoting); plotting and operations/registry (VERIFY-ONLY — no script promoted). Affected skills: `sidis-hq2-radiative`, `sidis-hq2-systematics`, `sidis-hq2-state`, `sidis-hq2-results`, `sidis-hq2-baseline-update`.

## 3. Files Touched (13 = 12 edited + this record; all direct edits, no git commits)

- **Five state docs:** `STATE.md` (Last-updated line; Release and §C_rad Status rows; Superseded list + 4 new rows; `2026-10-10 c_rad Baseline Promotion Override` section); `docs/RUN_STATUS.md` (Last-updated; Compact Allow / budget / summary / C_rad rows; `2026-10-10 c_rad Baseline Promotion Override` section); `docs/status/radiative-corrections.md` (live-claim rewrite to the emfix/consolidated basis + promotion section); `docs/status/current-release.md` (Last-updated; applied-correction-definition row; canonical-budget row + superseded-budget list; `P-CRAD-LUMI` row; STATUS UPDATE note on the dated 2026-10-07 blocker list); `context-handoff.md` (startup card; Release Facts rows; §C_rad Status rows; override pointer).
- **Five skills:** `sidis-hq2-radiative`, `sidis-hq2-systematics`, `sidis-hq2-state`, `sidis-hq2-results`, `sidis-hq2-baseline-update` (details in §7).
- **Two registry files:** `docs/skills_build/ownership.tsv` (+4 rows, 474 → 478 lines); `docs/skills_build/COVERAGE_MANIFEST.md` (header extension note; +4 lines; section counts radiative 15→18, systematics 24→25).
- **This record.**
- **NOT touched:** `RUNLIST.md` (both copies) and `SEVEN_CATEGORY_SCRIPT_INDEX.md` (verify-only, §5); all registry plotter scripts; the consolidated release and its source directories; all four budget directories; the Born release and its candidate; all plot packages; no other analysis's context or registry.

## 4. Superseded Values (old -> new)

1. **Canonical budget:** pre-emfix `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/` (totals h+ median `5.7125%`/max `160.626%`, h− `6.1448%`/`84.773%`; `c_rad_gen_rel` medians h+ `0.5505%`/max `5.866%`, h− `0.5166%`/max `5.703%`) -> emfix `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/` (totals `5.7093%`/`160.627%`, `6.1523%`/`84.775%`; `c_rad_gen_rel` `0.5844%`/`5.762%`, `0.5804%`/`5.663%`). Median deltas `-0.0032 pp` (h+) / `+0.0075 pp` (h−); per-bin delta vs the superseded budget: median `+0.0019 pp`, max |delta| `0.0437 pp` (h+); median `+0.0034 pp`, max `0.0266 pp` (h−); increment vs the 11-component base at the median `+0.0255 pp` (h+) / `+0.0573 pp` (h−). The pre-emfix dir is retained untouched with its `README_SUPERSEDED_2026-10-09.md`.
2. **Canonical budget ARTIFACT:** intermediate `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/` -> `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/` (numerically identical; CSV byte-identity verified cell-by-cell pre-write and by sha256 post-write; the 2026-10-09 package retained untouched as provenance).
3. **`c_rad_gen_rel` basis sidecars:** pre-emfix `crad_gen_unc_v3_releasebin_490rows.csv` (in both releasebin table dirs; medians `0.5505%`/`0.5166%`) -> emfix sidecar `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/crad_gen_unc_v3_releasebin_emfix_490rows.csv` (medians `0.5844%`/`0.5804%`); the DJ14 dir's pre-emfix sidecar carries `crad_gen_unc_v3_SUPERSEDED_2026-10-09.md`.
4. **Formal alternative scheme table:** pre-emfix `data/radiative/rapgap_crad_releasebin_2026-10-09/` (integrated h+ `1.1364508941898332` / h− `1.1358814928541603` — STALE pre-repair values) -> `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/` (h+ `1.1342370401351234` / h− `1.1336689310022947`); the old dir is retained untouched with `README_SUPERSEDED_2026-10-09.md`.
5. **Cross-generator spread statement (current):** pre-repair `+0.4458%`/`+0.4539%` -> emfix pair `+0.6419%`/`+0.6499%` (the repair increases the spread, as the audit predicted).
6. **c_rad baseline REFERENCE LOCATION (only):** scattered historical tables -> the consolidated release `crad_release_baseline_2026-10-10/`. Per `RELEASE_2026-10-10.md` it supersedes, **as a reference location only**, the xFcut tables `data/radiative/djangoh14_crad_final_xfcut_2026-10-05/` and `data/radiative/rapgap_crad_final_xfcut_2026-10-05/` (A-F1; already superseded-for-application 2026-10-09), the pre-emfix `data/radiative/rapgap_crad_releasebin_2026-10-09/`, and the pre-emfix sidecars documented by supersession notes. NOT a repointing of the consumed canonical paths, Born-release manifests, or plotter registrations.
7. **UNCHANGED (named to prevent false supersession):** applied primary DJ14 releasebin values (`1.1415172920054786`/`1.1410367377289135`); `c_rad_stat_rel` (`0.4272%`/`0.4429%` medians, `4.408%`/`6.036%` max; `covariance_cradstat/` carried byte-identical, NOT regenerated); the Born release and its integrated values; the raw central release; `L_data`.
8. **Budget chain restated:** 11-component b1floor (`5.684%`/`6.095%`) -> crad13 2026-10-07 (`5.713%`/`6.150%`; `c_rad_gen_rel` `0.5298%`/`0.5009%`, `c_rad_stat_rel` `0.4272%`/`0.4480%`) -> releasebin 2026-10-09 (pre-emfix) -> **emfix 2026-10-10 (CANONICAL)**.

## 5. Plotter / Registry Verification (checklist step 6 — VERIFY-ONLY, no edit)

- The `RAPGAP31_relbin` FORMAL ALTERNATIVE SCHEME entry already resolves to `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` in **all four registry plotter copies** (nominal + bgfracsub, both registry roots); md5s identical across copies: nominal `d1c08b82c56aa65b659b631c0ee36aea`, bgfracsub `34c49e7a38a2229fd031f89d0d7b8012`. The `DJANGOH14_relbin` applied primary and the default `CRAD_TABLE_KEY` are unchanged.
- `RUNLIST.md` line 232 in **both copies** records the 2026-10-09 emfix repoint (integrated-value assert update and md5s included).
- **No edit was made; no script was promoted; NO misdated 2026-10-10 repoint note was added** — the repoint predates this promotion (2026-10-09), and re-recording it under a 2026-10-10 date would falsify registry history. Checklist step 6 was therefore a verify-only no-op: no script became canonical or was superseded by this promotion.

## 6. Conflicts Found and Recorded

1. **EMPZ>45 wording (both values recorded):** `RELEASE_2026-10-10.md` and the emfix budget README carry the inherited caveat "open EMPZ>45 convention"; `STATE.md` addendum A (2026-10-09) records the user decision CLOSING the convention choice (retain `GenEmpz>45`; no applied table changed). The closure is authoritative; the release/budget README wording is inherited caveat text. Recorded in `STATE.md` ("Promotion does not cure") and here. NOT edited (historical artifacts).
2. **`sidis-hq2-plotting` staleness (pre-existing, NOT edited):** SKILL.md lines 12, 31, 78, 97 still anchor on the 2026-10-07 xFcut Born release / xFcut-basis products (superseded 2026-10-09 by `born_crad_dj14relbin_..._2026-10-09/`). The staleness predates this promotion; the skill's routing content is otherwise unaffected. Recorded as grep leftover (§9, item 1).
3. **RUNLIST dated layers (historical, NOT edited):** the Phase-4 "Current xFcut" layer, line 205 (DENY repoint target still names the xFcut tables), and line 234 (dated "Carried OPEN" list with pre-repair labels: PROVISIONAL xFcut baseline, open EMPZ>45, `DIAGNOSTIC_MANIFEST_LEVEL`) are historical layers superseded by later dated entries (line 232 and the 2026-10-09/10 dated sections) per the RUNLIST dated-section convention.
4. **RUNLIST line 243 band-basis pointer (pending user decision):** the 2026-10-10 user-directed band update points both canonical plotter registry copies at `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/` (numerically identical) rather than the canonical `..._emfix_2026-10-10/` artifact — follow-up (a), §10.
5. **`make_born_emfix.py` (package-local, NOT edited):** line 23 in `plots/systematics/born_relbin_crad13_emfix_2026-10-09/` still maps `RAPGAP31_relbin` to the pre-emfix `data/radiative/rapgap_crad_releasebin_2026-10-09/` table (publication-grade-review-2026-10-10 finding). Additionally, observed in this record's final verification, its hard-coded constants at line 25 carry the emfix medians (`5.7093`/`6.1523`) but the pre-emfix maxes (`160.626`/`84.773` vs the emfix `160.627`/`84.775`). Build script of a completed plot package; recorded, not edited.
6. **`sidis-hq2-baseline-update` line-10 dated anchor:** the "Current baseline anchor (as of `STATE.md`, last updated 2026-10-07)" snapshot is superseded by the skill's dated addenda (now including the 2026-10-10 c_rad addendum appended by this promotion). Kept as a dated snapshot per the skill's own addenda-supersede-anchor convention; recorded here.
7. **Registry-coverage observation:** the three budget DIRECTORIES `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/`, `..._crad13_releasebin_emfix_2026-10-09/`, and `..._crad13_releasebin_emfix_2026-10-10/` have no `ownership.tsv` rows (only the 2026-10-07 crad13 dir and the new emfix README are registered). This promotion's registration scope was the four documents in §8; directory-level registration is left as optional registry hygiene.
8. **Dated addendum E records (historical by design):** the 2026-10-09 addendum E entries in `STATE.md`, `docs/RUN_STATUS.md`, and `context-handoff.md` name `emfix_2026-10-09` as canonical — historical, NOT rewritten (provenance invariant); superseded by the new 2026-10-10 promotion sections.

## 7. Skills

**Updated (5), all with explicit supersession markers, never silent deletion:**
- `sidis-hq2-radiative`: Authoritative Facts rewritten to the consolidated release + emfix basis (four roles, integrated values, spreads, NoZ status, quoting rule, carried caveats); superseded pre-emfix budget/ARTIFACT, pre-emfix RG31 table and sidecars, and xFcut layers negated in Traps; +3 Canonical References rows (release README, release manifest, this record); addendum.
- `sidis-hq2-systematics`: C_rad menu row and budget-basis line -> emfix; superseded-budgets list (pre-emfix, intermediate artifact, 2026-10-07 chain); traps 70/72; 4 Canonical References rows including the canonical emfix budget README; line-9 Born-release fix (dj14relbin); addendum.
- `sidis-hq2-state`: anchor lines 13/20; Version History +4 supersession rows (canonical budget, budget artifact, RG31 table, sidecar basis) + `P-CRAD-LUMI` relabel `FILE-SET-RECONCILED` + `P-CRAD-APPLIED` rewrite + `P-RAPGAP` update; Release/Radiative topic snapshots; traps 104/105 + line-103 Born-release fix; reference rows + 3 new; addendum.
- `sidis-hq2-results`: lines 10/11 Born-release and budget anchors; trap 52; Canonical References rows relbin-current + xFcut-superseded; addendum.
- `sidis-hq2-baseline-update`: appended the 2026-10-10 c_rad baseline promotion addendum (anchor; WRONG/RIGHT traps including the verify-only plotter rule, the not-a-repointing rule, and the no-regeneration rule for `covariance_cradstat/` and band plots).

**Verified unaffected (10):** `sidis-hq2-docmap`, `sidis-hq2-normalization` (`L_data` disjointness unaffected), `sidis-hq2-unfolding` (flip-invariant), `sidis-hq2-backgrounds`, `sidis-hq2-aziasym` (BSA fork legitimately stays raw), `sidis-hq2-builder`, `sidis-hq2-operations`, `sidis-hq2-mc` (routing-level references only), `sidis-hq2-lessons` (dated mentions acceptable), and `sidis-hq2-plotting` (verified WITH recorded pre-existing staleness, §6 item 2 — not edited by this promotion).

## 8. Ownership Registration (step 8)

Four rows appended to `docs/skills_build/ownership.tsv` (474 → 478 lines), absolute NAF paths:

| New row | Owner |
|---|---|
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_release_baseline_2026-10-10/RELEASE_2026-10-10.md` | `sidis-hq2-radiative` |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_release_baseline_2026-10-10/release_manifest.json` | `sidis-hq2-radiative` |
| `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/CRAD_BASELINE_PROMOTION_2026-10-10.md` (this record) | `sidis-hq2-radiative` |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/README.md` | `sidis-hq2-systematics` |

`COVERAGE_MANIFEST.md`: header extension note + the same four lines; section counts updated by these lines only (radiative 15→18, systematics 24→25). Canonical References rows were added in the owning skills during step 7. See §6 item 7 for the budget-directory registration gap observation.

## 9. Grep Verification Pass (step 9)

Scope: `STATE.md`, `context-handoff.md`, `docs/**/*.md`, `.opencode/skills/**/*.md`; the `RUNLIST.md` copies counted separately. Initial-sweep hits per superseded token: `crad13_releasebin_2026-10-09` 53; `rapgap_crad_releasebin_2026-10-09` 39 (+2 RUNLIST); stale RG31 integrated `1.1364508941898332` / `1.1358814928541603` 32 each; stale medians `0.5505` / `0.5166` 38 each (+2 RUNLIST); xFcut table dirs 20 each (+6 RUNLIST).

**Seven stale live claims found and FIXED (each re-verified in place at final check):**
1. `STATE.md` line 59 — phi_h budget-basis history chain still ended at the pre-emfix releasebin budget; extended to name the emfix_2026-10-10 canonical.
2. `STATE.md` line 95 — DEAD-path repoint target still named the xFcut tables; repointed to the releasebin DJ14 applied primary / RG31 emfix formal alternative.
3. `docs/RUN_STATUS.md` line 122 — phi_h current-budget chain; now names emfix_2026-10-10 (since the 2026-10-10 baseline promotion).
4. `context-handoff.md` line 97 — legacy-budget current-basis pointer; now emfix_2026-10-10.
5. `context-handoff.md` line 152 — phi_h section budget chain; now emfix_2026-10-10.
6. `sidis-hq2-systematics` SKILL.md line 9 — Born release named the superseded dj14xfcut release; now dj14relbin (xFcut retained as provenance).
7. `sidis-hq2-state` SKILL.md line 103 — Born-release trap named dj14xfcut; now dj14relbin.

**Verdict: PASS.** After the seven fixes, every remaining hit is an explicit historical, supersession, or dated marker. **Ten leftover items recorded** (not fixed — pre-existing, dated, or pending-decision): (1) `sidis-hq2-plotting` lines 12/31/78/97 (§6.2); (2) RUNLIST Phase-4 xFcut layer (§6.3); (3) RUNLIST line 205 (§6.3); (4) RUNLIST line 234 (§6.3); (5) RUNLIST line 243 band-basis pointer (§6.4, follow-up (a)); (6) `make_born_emfix.py` line 23 + line-25 constants (§6.5); (7) EMPZ "open" wording in the release/budget READMEs (§6.1); (8) baseline-update line-10 dated anchor (§6.6); (9) dated addendum E canonical-budget records (§6.8); (10) budget-directory registration gap (§6.7). No live contradiction remains.

## 10. Blockers Carried OPEN and Pending Follow-ups

**(c) Carried blockers (unchanged by this promotion; do not close):**
- `P-CRAD-LUMI` generator-log ceiling — `FILE-SET-RECONCILED` per the 2026-10-09 audit; generator-log closure permanently blocked externally (MCDB CGI disabled); RG31 two-valued σ̂ {`7287.915`, `7323.939` pb} bookkeeping residual bounded `-0.44%…+0.26%` per-beam. C_rad remains NOT publication-grade.
- PROVISIONAL baseline — promotion gate only, not publication clearance.
- EMPZ>45 convention — CLOSED 2026-10-09 (retain `GenEmpz>45`) per `STATE.md` addendum A; the release/budget READMEs carry inherited "open" wording (recorded conflict, §6.1).
- Mixed-beam-only validity — no per-beam or e− Born target.
- NoZ DJANGOH-only PROVISIONAL DIAGNOSTIC — `c_rad_applied=false`.
- DJANGOH e− RAD `4.67%` count shortfall — ROOT-CAUSED (pool-sizing feature per audit Step 1; zero bias to C_rad); precision-only, carried in `c_rad_stat_rel`.
- Raw/no-bgsub release with response fake-fraction subtraction only (`10.7141%` h+ / `10.4019%` h−).
- z<0.05 xF-definition sensitivity — CAVEAT-ONLY.
- A-F3 charge-asymmetry systematic mandatory before any A_h quote.
- Analysis-note "no radiative correction applied" text pending (`sections/results.tex:103`, `sections/systematics.tex`).
- `P-PLOTS`, `P-ALPHA`, `P-METHOD` open.
- Conservative overlap caveat (carried, re-evaluated on the emfix pair): double-counted piece at the median bin `0.2008 pp` (h+) / `0.3026 pp` (h−); net total inflation if removed `0.0035 pp` / `0.0074 pp`; direction conservative. `b1_aggregation_status: FAIL` (4 supported bins) inherited report-only, not a blocker.

**(a) PENDING USER DECISION — band-basis regeneration:** `plots/systematics/born_relbin_crad13_2026-10-09/` has a verified pre-emfix band basis (superseded with pointer by `born_relbin_crad13_emfix_2026-10-09/`); the band-basis pointers of `born_relbin_crad13_emfix_2026-10-09/`, `born_relbin_canonical7panel_2026-10-09/`, and `RUNLIST.md` line 243 all reference the numerically identical `..._emfix_2026-10-09/` budget dir rather than the canonical `..._emfix_2026-10-10/` artifact. This promotion does not regenerate any plots.

**(b) RESOLVED BY VERIFICATION — NoZ plots metadata check:** `plots/radiative/cgammaborn_v4_releasebin_nozborn_7col_2026-10-09/` contains NO pre-emfix WithZ-spread reference; the only pre-emfix spread mention in the NoZ chain is the historical-context sentence in the source-dir proxy JSON `cgammaborn_gen_unc_proxy_v3emfix.json` (line 14). **No regeneration is required by this promotion.**

## 11. Remaining Actions

1. Follow-up (a): user decision on band-basis regeneration and/or pointer updates (§10).
2. Analysis-note text update (`sections/results.tex:103`, `sections/systematics.tex`) — carried from the 2026-10-07 promotion.
3. `P-CRAD-LUMI`: trace MC exposures to MCDB/generator logs before any publication claim (generator-log closure permanently blocked externally; `FILE-SET-RECONCILED` stands).
4. Optional registry hygiene: budget-directory ownership rows (§6.7); plotting-skill refresh of the pre-existing xFcut anchoring (§6.2).
5. NOT required by this promotion: `covariance_cradstat/` regeneration (`c_rad_stat_rel` unchanged); Born or NoZ plot regeneration; RUNLIST re-edits (verify-only by contract).

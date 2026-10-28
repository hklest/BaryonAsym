# Publication-Grade Baseline Review — High-Q² Charged-SIDIS Born Chain (releasebin C_rad + emfix)

**Date:** 2026-10-10
**Reviewer:** H1 BaryonAsym physics-correctness critic (read-only on all products; critique authority only)
**Review class:** Publication-grade baseline review of the canonical Born-level cross-section baseline + uncertainty budget. The 2026-10-09 releasebin review closed as a promotion gate only; this review makes the publication call.

## VERDICT: PUBLICATION-GRADE WITH CONDITIONS

The canonical Born chain — raw coherent-prior IBU release → releasebin DJANGOH14 C_rad table → installed Born release (`born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09`) → emfix 13-component budget — is bitwise-verified end to end (independent recomputations, byte-identity checks, exact quadrature closure) and is publication-grade at the chain level. Publication of Born-level cross-sections and the uncertainty budget is conditional.

- The single physics-mandatory condition (C-1) is that the budget gains a treatment of the measured raw/no-bgsub contamination (10.7141% h+ / 10.4019% h−), which currently sits outside the budget while exceeding its median totals (h+ 5.7093% / h− 6.1523%). Until C-1 is executed, the budget must be presented as incomplete for absolute charged-hadron cross-section claims.
- The remaining conditions (C-2…C-7) are product- and documentation-level, including three NEW downstream-product defects found by this review (Findings 1–3).

**Verdict scope:** Born-level cross-section baseline + uncertainty budget only. This is not a clearance of theory comparisons, multiplicities (blocked: no DIS denominator; raw/no-bgsub), or charge asymmetries (A-F3 gate mandatory first).

## 1. Material reviewed

- **State authority:** `sidis_highQ2/STATE.md` (incl. 2026-10-09 addendum A–G), `sidis_highQ2/docs/RUN_STATUS.md`, `sidis_highQ2/context-handoff.md` (2026-10-09 addendum).
- **Gate records (2026-10-09):** critic gate `docs/critiques/critic-gate-releasebin-promotion-2026-10-09.md` (PASS WITH CONDITIONS, 6 conditions); normalization gate `docs/status/normalization-gate-releasebin-2026-10-09.md` (PASS WITH CAVEAT, 2 caveats); QA gate `docs/qa/qa-gate-releasebin-2026-10-09.md` (PASS WITH CONDITIONS, 3 conditions).
- **New evidence:** `docs/radiative/CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md` (+ emfix addendum), `docs/radiative/CRAD_EMPZ_CONVENTION_SENSITIVITY_2026-10-09.md`, `docs/radiative/CRAD_ZLOW_DEFINITION_SENSITIVITY_RELEASEBIN_ADDENDUM_2026-10-09.md`, `docs/critiques/critic-response-born-release_2026-10-08.md` (A-F1…F5, B-F1…B6 dispositions), `docs/provenance/releasebin-install-verification-2026-10-09.md` (46/46 hashes, diff 0).
- **Chain products (read-only):** installed Born release `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/` (README, `validation/validation.json`, `alternative_scheme_rapgap31_relbin/`); canonical emfix budget `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/` (README, `validation_checks.json`, `independent_verification_2026-10-09.json`); emfix repair `sidis_highQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/emfix_validation.json`; DJ14/RG31 releasebin and emfix C_rad tables (sha256-verified identities).
- **Downstream products:** emfix review plot package `sidis_highQ2/plots/systematics/born_relbin_crad13_emfix_2026-10-09/` (`make_born_emfix.py`, both plot manifests, `captions.txt`, README); note tables `sidis_highQ2/data/tables/highQ2_z005_born_crad13_releasebin_2026-10-09/provenance.json`; NoZ variant `sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/` (observation only).
- **Evidence types:** formulas and basis statements embedded in product metadata/provenance notes; build scripts and manifests (implementation layer); per-bin and integrated numbers; sha256 table identities; gate-record condition texts verbatim.

## 2. Gate-condition closure (Mandate 1)

| Gate | Condition (gist) | Closure at this review | Evidence |
|---|---|---|---|
| Critic 1 | Downstream supersession: installed 2026-10-07 xFcut Born release, `data_vs_mc_z005_4generator_born_2026-10-08` package, and analysis-note Born totals/figures repointed to releasebin-embedded products with recorded diff-based verification (old-table residue: per-bin median ~3.50%, max ~11.37%) | **PARTIAL.** Born release install EXECUTED (checksum-verified 46/46, diff 0). `data_vs_mc` package and note totals/figures NOT repointed (carried; covered by C-5). NEW: two further stale embeds found inside accepted/canonical products (Findings 1–2) | Install record; Findings 1–2 |
| Critic 2 | Canonical plotter `CRAD_CANDIDATES` registration before any Born-level plot regeneration | **EXECUTED.** All four copies (remote + local snapshot, both plotters) updated: DJANGOH14_relbin primary, RAPGAP31_relbin repointed to the emfix table, xFcut entries marked SUPERSEDED-FOR-APPLICATION | `make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` CRAD_CANDIDATES; install record |
| Critic 3 | Stale provenance strings in DJ14 `crad_v3_summary.json` (template-inherited xFcut contract, dead pre-quarantine path) — correct or annotate, no silent rewrite | **EXECUTED.** Annotated in install record §7; campaign outputs not rewritten | Install record §7 |
| Critic 4 | Filename collision (releasebin CSVs share xFcut filenames; directory-qualified consumption mandatory; promotion record states the distinction) | **EXECUTED** (install record §6 + state docs). **NEW GAP:** the same collision pattern now distinguishes pre-emfix vs emfix RG31 tables (`crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` exists in both `rapgap_crad_releasebin_2026-10-09/` and `rapgap_crad_releasebin_emfix_2026-10-09/`); Finding 1 is a live instance. Rule must extend to the emfix pair (C-4) | Install record §6; Finding 1 |
| Critic 5 | Stale docstring in `finalize_releasebin.py` (claims xFcut-worker partials) | **DISPOSITIONED.** Annotated in install record §7; correct on next touch | Install record §7 |
| Critic 6 | Carried-open items quoted alongside any promoted Born-level number | **EXECUTED in state docs** (STATE.md carried-OPEN list); **STALE in the newest note tables** — status_lines quote superseded caveats (P-CRAD-LUMI DIAGNOSTIC_MANIFEST_LEVEL; EMPZ OPEN; ±6.8%/±3.9%) → Finding 3 | STATE.md; note-tables `provenance.json` |
| Norm caveat 1 | Per-file exposure manifest regression: copy or reference the 2026-10-05 per-file manifest IF releasebin becomes the long-term applied basis | **DISPOSITIONED, NOT EXECUTED.** Releasebin is now the applied basis → the conditional trigger has fired (C-6). Per-leg totals verified correct, recorded in four places per campaign | Normalization gate caveat 1; exposure audit |
| Norm caveat 2 | Carried-open exposure/normalization blockers travel with acceptance | **SUPERSEDED in part.** P-CRAD-LUMI upgraded to FILE-SET-RECONCILED (generator-log externally blocked); EMPZ decision CLOSED; DJ e− RAD 4.67% shortfall explained (variable chunk sizing, zero bias, precision only in `c_rad_stat_rel`); RG31 σ̂ two-valued residual bounded. Remaining: raw/no-bgsub and mixed-beam-only — adjudicated in §4 | Exposure audit; EMPZ doc; §4 |
| QA 1 | Promotion mechanics: checksum-verified install; recorded supersession of the 2026-10-07 xFcut Born release and the `crad13_2026-10-07` budget; STATE.md update | **EXECUTED.** 46/46 hashes, diff 0; supersession recorded (incl. README_SUPERSEDED marker on the superseded budget); STATE.md updated | Install record; STATE.md |
| QA 2 | Documentation follow-ups: quarantine README refresh; CRAD_CANDIDATES registration; analysis-note text fixes | **PARTIAL.** Quarantine README refreshed; registration done; **note text still PENDING** (STATE.md lines 44–45, 75: `results.tex:103` and `systematics.tex` still claim no radiative correction is applied — false under `c_rad_applied=TRUE`; stale 552% "genuine sparse-yield" text; 5 figure names missing `_z005`; undefined citation `2005Hadroo2`) | STATE.md; QA gate condition 2 |
| QA 3 | Carried-open physics caveats quoted; promotion is NOT publication clearance | **CARRIED — now adjudicated** by this review (§4), which is the publication call this gate deferred | QA gate condition 3; §4 |

## 3. New-evidence resolution of PROVISIONAL-baseline objections (Mandate 2)

1. **Exposure provenance (P-CRAD-LUMI) — resolved to publication-limiting-but-documentable.** The 2026-10-09 exposure audit reconciled all six legs at FILE-SET-RECONCILED level; per-leg totals are recorded identically in four places per campaign. The generator-log closure is PERMANENTLY BLOCKED externally (MCDB CGI disabled) — an infrastructure limit, not an analysis defect. The DJ e− RAD 4.67% count shortfall is explained as variable chunk sizing with zero bias (numerator and denominator use the same files; it enters only the precision of `c_rad_stat_rel`). The old DIAGNOSTIC_MANIFEST_LEVEL objection is retired; the note must document the new grading and the external blocker (C-7).
2. **EMPZ convention — fully resolved.** User decision CLOSED (retain GenEmpz>45); the 45–70 window is identical (0.000 difference); alternative windows are truth redefinitions; no applied table changed. The "future flip supersedes the applied table" objection is retired.
3. **emfix repair (RG31 nonrad(em) exposure defect) — resolved at chain level, with propagation gaps.** L_nonrad(em) 13,416,991.0 → 13,470,764.7 (+53,773.7); per-bin C_rad shift medians −0.1946% (h+) / −0.1949% (h−), integrated −0.1948% (mean per-bin −0.1965%); `emfix_validation.json` all checks pass; deviation from the audit prediction ~1.4e-5. The budget swap is bitwise-verified: 30 carried columns byte-identical (0/14,700 mismatches), quadrature closure exact; only `c_rad_gen_rel` (medians 0.5505/0.5166 → 0.5844/0.5804) and the 4 total columns differ; totals h+ 5.7093% (was 5.7125%), h− 6.1523% (was 6.1448%). HOWEVER, the repair did not propagate to two downstream products: the accepted emfix review package's RG31 pages and the installed Born release's alternative-scheme subproduct still consume the pre-emfix RG31 table (sha256 `3860a100…`, integrated 1.1364508941898332 / 1.1358814928541603) instead of the emfix table (sha256 `f669aed1…`, integrated 1.1342370401351234 / 1.1336689310022947) — Findings 1–2, condition C-4.
4. **z<0.05 definition sensitivity — resolved as documentable (caveat-only, user decision).** Releasebin quantification: DJ14 max 9.925% (h+) / 8.023% (h−); RG31 6.461% / 5.200%; all confined to the z<0.05 row; z≥0.05 rows 0.000%. The old ±6.8%/±3.9% figures are superseded. Because 9.925% exceeds the median total (5.7093%), the caveat must be surfaced per-row wherever the z<0.05 row is quoted (C-2).

## 4. Adjudication of carried limitations (Mandate 3)

| # | Carried limitation | Adjudication | Physics basis |
|---|---|---|---|
| L1 | raw/no-bgsub with response fake-fraction subtraction only (10.7141% h+ / 10.4019% h−); inherited unsubtracted track-level background is multiplied by the correction factor | **Publication-blocking for absolute charged-hadron cross-section claims as constituted; curable by C-1.** | The measured ~10% contamination is neither subtracted nor budgeted; it exceeds the median total uncertainty (5.7093%/6.1523%), so quoted error bars do not cover a dominant known bias and central values are known-biased high. Not verdict-fatal: the contamination is measured, stable, and charge-symmetric to ~0.3pp (so it does not fake inter-charge structure), and two executable cures exist (bounded budget component from the measured fraction; or the existing `cRad_bgfracsub` variant promoted through its own gate). Blocking for multiplicities and any hadron-level interpretation until cured. |
| L2 | mixed-beam-only C_rad validity (not valid for per-beam or e− Born targets; no Born-vs-RAD-level-MC comparison without the same correction) | **Publication-limiting-but-documentable.** | Correction and published observable are both mixed-beam — the primary scheme is self-consistent. The prohibited uses are scope exclusions to state in the note (C-7). |
| L3 | RG31 two-valued σ̂ residual {7287.915, 7323.939 pb}: −0.44%…+0.26% per-beam; mixed −0.09%/−0.10% | **Documentable; alternative-scheme only.** | Affects only the alternative scheme's per-beam normalization; effect on the total budget <0.005pp; conservative direction; mixed-beam residual ~0.1%. Quote in the alternative-scheme description (C-7); no action on the primary scheme. |
| L4 | generator-log closure permanently blocked (external) | **Publication-limiting-but-documentable.** | External infrastructure limit (MCDB CGI disabled); six legs FILE-SET-RECONCILED; per-leg totals cross-recorded ×4. Document grading + blocker (C-7); no internal cure exists or is required. |
| L5 | z<0.05 caveat-only sensitivity (DJ14 9.925%/8.023%; RG31 6.461%/5.200%; z≥0.05 = 0.000%) | **Publication-limiting-but-documentable (C-2).** | Quantified, row-confined definition sensitivity exceeding row-level median totals in the first z row. The user's no-budget-component decision is acceptable for the baseline only with mandatory per-row documentation. |
| L6 | z-rising / pT_h-falling total-uncertainty trend | **Expected behavior; non-blocking.** | Composition-consistent: high-z bins are sparse (stat-dominated; maxima 160.626%/84.773%); low-pT_h bins carry tracking-eff/model dominance. The trend reflects component structure, not an artifact; explain in the note's systematics section. |
| L7 | sparse-bin totals up to ~160% | **Documentable; presentation-level; non-blocking.** | Honest bookkeeping for low-occupancy bins. Mark or mask sparse bins in note figures/tables; do not merge bins without a recorded binning decision (R-3). |
| L8 | `cRad_bgfracsub` stays a separate derived variant (never merged into nominal) | **Correct as-is; becomes publication-relevant via C-1.** | Merging an unvalidated subtraction into the nominal would be worse than keeping it separate. It is the ready-made ingredient for C-1 path (b) and must not be quoted as nominal until it passes its own gate. |
| L9 | filename-collision rule | **Executed; extend to the emfix pair (C-4).** | The recorded xFcut-vs-releasebin rule must also govern pre-emfix-vs-emfix RG31 directories (identical CSV filename; only directory and sha256 distinguish). Finding 1 is the live instance. |
| L10 | A-F3 charge-misid scope | **Out of scope for this verdict; documentable for cross-sections.** | Study exists with merged outputs: median relative bias 0.046%/0.051%, integrated shift ±0.34% — negligible for cross-section central values. Charge-asymmetry publication requires the A-F3 gate first (scope exclusion). |
| L11 | analysis-note stale text: "no radiative correction applied" at `sections/results.tex:103` and `sections/systematics.tex`; stale 552% "genuine sparse-yield" text; 5 figure names missing `_z005`; undefined citation `2005Hadroo2` | **Publication-blocking for the NOTE; mechanical fix (C-5).** | The note currently makes false claims about the correction basis of every table it shows. Blocks note circulation, not the baseline chain. |
| L12 | P-CRAD-LUMI relabeling (DIAGNOSTIC_MANIFEST_LEVEL → FILE-SET-RECONCILED) | **Executed in state docs; stale in note tables (Finding 3, C-3).** | The relabeling is correct and evidence-backed; remaining work is propagation into regenerated note-table status lines. |

## 5. NEW findings (Mandate 4)

### Finding 1 — CONCERN (major; mandatory fix before any external use)

**Claim criticized:** the accepted emfix review package presents its RAPGAP31_relbin "formal alternative scheme" pages as being on the emfix basis.

**Evidence:** `make_born_emfix.py` line 23 points `RAPGAP31_relbin` at `data/radiative/rapgap_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (pre-emfix directory); `RAPGAP31_relbin/plot_manifest_nominal_no_bgsub.json` records `c_rad_sha256 = 3860a1005c4394e88ba9d152a1f6e513a9fb1ef03204e59916831e007f10d403` (pre-emfix hash; the emfix table's hash is `f669aed1…`); `captions.txt` pages 4–5 state "uncertainty basis is the emfix 13-component releasebin budget"; the README claims the package supersedes its predecessor "because its bands use the new emfix budget" and nowhere discloses the central-value basis.

**Why it matters physically:** the RG31 central values plotted are ~+0.19% high (the bias emfix removed: per-bin medians −0.1946%/−0.1949%, integrated −0.1948%), while the bands' `c_rad_gen_rel` component is derived from the emfix RG31 table — a hybrid pairing corresponding to no single consistent scheme assignment. The "490-bin reproduction maximum deviation 0.0" check passes only because it reproduces against the Born release's alternative subproduct, which is itself pre-emfix (Finding 2) — circular consistency, not correctness.

**Severity rationale:** small magnitude (~0.2%) and an internal-review-only product, so not fatal to the baseline; but this is the accepted review artifact for the emfix basis and it misdescribes its own inputs. Fix under C-4.

### Finding 2 — CONCERN (major; same root cause; mandatory annotation or rebuild)

**Claim criticized:** the installed canonical Born release's alternative scheme is current.

**Evidence:** `born_crad_dj14relbin_…_2026-10-09/README.md` lines 131–141 describe `alternative_scheme_rapgap31_relbin/` as "the FORMAL ALTERNATIVE SCHEME" with integrated C_rad 1.1364508941898332 / 1.1358814928541603 (pre-emfix values) and "documented cross-generator difference +0.446% (h+) / +0.454% (h−)" (pre-emfix spread; the emfix pair is +0.6419% / +0.6499%); the subproduct integrated totals (13.040167 / 12.703023 nb) are pre-emfix (≈0.19% high vs an emfix rebuild); no supersession annotation exists anywhere in the subproduct or its README entry.

**Why it matters physically:** any consumer quoting RG31-based Born cross-sections or the cross-generator spread from the canonical release gets values inconsistent with the canonical emfix budget's `c_rad_gen_rel` basis (the budget's scheme-spread component assumes the emfix RG31 table), and the documented spread understates the current cross-generator difference by ~0.2pp. The primary DJ14 scheme is unaffected (the emfix repair touched only the RG31 table). Fix under C-4.

### Finding 3 — CONCERN (documentation correctness; mandatory regeneration before note tables are used)

**Claim criticized:** the note-tables package is on the current budget with current caveats.

**Evidence:** `data/tables/highQ2_z005_born_crad13_releasebin_2026-10-09/provenance.json` (generated 2026-10-09T18:50:55Z, status "OK, all gates passed") consumes `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_2026-10-09/…releasebin.csv` (sha256 `831cc3f4…` — the superseded pre-emfix budget; the canonical budget is `…_crad13_releasebin_emfix_2026-10-09`), and its `provenance_notes.status_lines` carry stale caveats: "P-CRAD-LUMI … DIAGNOSTIC_MANIFEST_LEVEL; C_rad is NOT publication-grade" (superseded by FILE-SET-RECONCILED), "EMPZ>45 convention decision OPEN" (superseded by CLOSED), "per-bin definition sensitivity up to ±6.8%/±3.9%" (superseded by releasebin 9.925%/8.023% DJ14, 6.461%/5.200% RG31, z≥0.05 rows 0.000%).

**Why it matters physically:** the numerical impact is negligible (budget totals differ by ≤0.0075pp: 5.7125→5.7093, 6.1448→6.1523), but these are the tables destined for the analysis note: they would embed a superseded uncertainty basis and caveat lines contradicted by the 2026-10-09 closures. Root cause is timing (tables generated before the emfix supersession was recorded), not arithmetic. Fix under C-3.

### Minor observations (non-blocking)

- **O-1 (undocumented NoZ variant):** `data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/` (PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`; C_gamma_born integrated 1.040960540463156 / 1.0401468080806677) feeds the note tables' no-Z mode and is not described in the state docs read for this review. Out of canonical with-Z Born scope. If the note publishes no-Z tables, this variant needs its own gate and state-doc entry; if diagnostic-only, document it as such (R-1).
- **O-2 (statistic labeling):** the emfix addendum's "mixed −0.1965%" is the mean per-bin expected shift; the integrated mixed shift is −0.1948% (per-bin medians −0.1946%/−0.1949%). Label the statistic when quoting (R-2).
- **O-3 (stale spread figures):** the startup card's cross-generator spread +0.4458%/+0.4539% is stale for the emfix pair (+0.6419%/+0.6499%); fold into C-4.
- **O-4 (h+ median-total decrease, verified consistent):** the emfix budget's h+ median total decreased (5.7125→5.7093%) while the per-bin `c_rad_gen_rel` deltas have a positive median (+0.0019pp). This is statistically consistent, not an arithmetic error: the median of per-bin quadrature sums is not the quadrature of medians, and the totals are dominated bin-by-bin by the larger non-radiative components. Verified against the 30 byte-identical carried columns and the exact quadrature closure.

## 6. Conditions for publication

**Mandatory physics (blocks publication of absolute cross-sections until executed):**

- **C-1 (background treatment):** EITHER add a bounded systematic component to the budget covering the measured raw/no-bgsub contamination (10.7141% h+ / 10.4019% h−, with the alpha0p2 study's uncertainty as the component width), OR promote the `cRad_bgfracsub` variant to the published correction through its own validation gate, OR explicitly scope every published table as raw/no-bgsub with the contamination quantified. Until one path is executed, the budget must be presented as incomplete for absolute charged-hadron cross-section claims.

**Mandatory product/documentation (blocks external circulation until executed):**

- **C-2:** surface the z<0.05 row sensitivity (DJ14 9.925%/8.023%; RG31 6.461%/5.200%) per-row in note tables/figures; state z≥0.05 rows are 0.000%.
- **C-3:** regenerate the note tables on the emfix budget with updated status lines (FILE-SET-RECONCILED; EMPZ CLOSED; releasebin z-low figures).
- **C-4:** repair the emfix-basis presentation of the RG31 alternative scheme: regenerate review-package pages 4–6 on the emfix table (or annotate the hybrid and regenerate before circulation); annotate/supersede the Born release's `alternative_scheme_rapgap31_relbin/` and correct its README spread figures; extend the filename-collision rule to the emfix-vs-pre-emfix directory pair.
- **C-5:** fix the analysis-note text (`results.tex:103` and `systematics.tex` "no radiative correction applied" claims; stale 552% "genuine sparse-yield" text; 5 figure names missing `_z005`; undefined citation `2005Hadroo2`) and repoint note Born totals/figures to releasebin-embedded products (completes critic condition 1).
- **C-6:** copy or reference the 2026-10-05 per-file exposure manifest into the releasebin campaign provenance (norm caveat-1 trigger has fired: releasebin is the long-term applied basis).
- **C-7:** document in the note's radiative/systematics sections: FILE-SET-RECONCILED grading with the externally blocked generator-log closure; DJ e− 4.67% shortfall (precision-only); RG31 σ̂ two-valued residual (alternative-scheme, <0.005pp on total, conservative); mixed-beam-only scope; raw/no-bgsub basis; and the Born-or-raw + `body+miss` prior statement on every quote (QA condition-3 continuation).

**Recommended:**

- **R-1:** add the NoZ variant to state docs with explicit scope (diagnostic vs publishable) — O-1.
- **R-2:** label shift statistics (median vs mean vs integrated) in the emfix addendum — O-2.
- **R-3:** mark or mask sparse bins (>100% totals) in presentation figures — L7.

## 7. Non-blocking checks confirmed

- **Born release arithmetic:** `canonical_arithmetic_diff = 0` (independent recomputation bitwise, per table per scheme); integrated Born cross sections equal the sum of per-bin raw × C_rad products to machine precision; applied-table integrated C_rad reproduces 1.1415/1.1410 within the 1e-12 gate; unscaled columns byte-identical to the parent CSV text.
- **emfix budget swap:** 30 carried columns byte-identical (0/14,700 mismatches); quadrature closure exact; only `c_rad_gen_rel` + 4 total columns differ; independent verification 2026-10-09 PASS.
- **emfix repair:** validation all-pass; deviation from audit prediction ~1.4e-5; integrated shift −0.1948% consistent with per-bin medians −0.1946%/−0.1949%.
- **Review-package DJ14 pages:** 490-bin reproduction deviation 0.0 against the Born release; bands from the emfix budget column `total_with_crad13_rel` with medians h+ 5.7093% / h− 6.1523% and maxima 160.626% / 84.773% matched at published precision.
- **Normalization gate five checks PASS:** `L_data = 351000.0 nb^-1` consistency; byte-identical unscaled columns; no forbidden-lumi traps; unit sanity (pb/nb conventions).
- **No uniform-factor misreading possible:** the release README documents both the data-weighted Born/raw factors (1.1414120 / 1.1414071) and the table's sigma-weighted integrated C_rad (1.1415 / 1.1410) with the per-bin-application explanation.
- **A-F3 charge-misid bias small:** median relative bias 0.046%/0.051%, integrated shift ±0.34% — no cross-section impact at publication precision.
- **Scope discipline held:** no multiplicities, no theory comparison, and no charge-asymmetry claims are cleared by this review.

## 8. Missing evidence

- Direct .tex read of the analysis note (repo path not locally indexed in the state docs read; STATE.md lines 44–45 and 75 accepted as the authority for the pending note fixes).
- `data_vs_mc_z005_4generator_born_2026-10-08` package contents not re-inspected this session (carried critic-condition-1 residue; disposition under C-5).
- Per-bin RG31 pre-emfix vs emfix consumed-value table for the review package (table identity is covered by the builder path + sha256 + emfix validation; per-bin shifts quoted from `emfix_validation.json`).

## 9. Recommended next check

Execute the **C-1 background-treatment decision first** — it is the only condition that changes published numbers or their error bars, and every other condition is mechanical once specified. Hand the C-1 method choice (bounded budget component from the measured fake fraction vs `cRad_bgfracsub`-as-nominal with its own gate vs explicit raw-scope reframing) to the systematics/backgrounds specialists for a bounded-component recommendation; then run the mechanical regenerations C-3/C-4/C-5 as exactly-specified edits. The Born baseline and emfix budget themselves need no rederivation.

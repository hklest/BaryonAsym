# C_rad Follow-up Comparison — New Documentation (2026-10-07 → 2026-10-10) vs the 2026-10-07 Note-Discrepancy Proposals

**Date:** 2026-10-10
**Auditor role:** read-only audit. No source file was modified; the only file written is this report.
**Scope:** compare the c_rad documentation produced 2026-10-07 through 2026-10-10 against the prior audit proposals R1–R7 and UNVERIFIED items 10–11 of `docs/research/note_discrepancy_review_2026-10-07/unfolding_backgrounds_radiative.md` (§4–§5), and review the updated note section `sections/radiative_corrections.tex` against the current authoritative state.

**Prior-proposal source (read in full, re-verified this session):**
- `docs/research/note_discrepancy_review_2026-10-07/unfolding_backgrounds_radiative.md` — §4 findings R1–R7 (lines 233–296), §5 UNVERIFIED items 10–11 (lines 312–313).

**New documentation audited (read in full in the prior session pass; key statements re-verified by targeted re-read this session):**
1. `docs/radiative/CRAD_APPLICATION_PROMOTION_2026-10-07.md`
2. `docs/radiative/CRAD_UNC_GENERATOR_SPREAD_2026-10-07.md`
3. `docs/systematics/CRAD_STATFLOOR_STUDY_2026-10-07.md`
4. `docs/radiative/CRAD_XBINNING_SENSITIVITY_2026-10-08.md`
5. `docs/radiative/NOZBORN_EXPOSURE_DERIVATION_2026-10-08.md`
6. `docs/radiative/EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md`
7. `docs/radiative/BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md`
8. `docs/radiative/CRAD_EMPZ_CONVENTION_SENSITIVITY_2026-10-09.md`
9. `docs/radiative/CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md`
10. `docs/radiative/CRAD_ZLOW_DEFINITION_SENSITIVITY_RELEASEBIN_ADDENDUM_2026-10-09.md`

**Authoritative baseline (read in full / key lines re-verified this session):**
- `STATE.md` (last updated 2026-10-10; releasebin promotion in header line 1, Release row line 10, Pipeline paragraph line 14, mixed-scheme line 65, superseded list lines 89–98, 2026-10-07 override lines 109–113 marked superseded, 2026-10-09 addendum lines 166–176).
- `docs/status/radiative-corrections.md` (header 2026-10-08, lines 1–88; 2026-10-09 tail sections lines 90–126).
- `docs/status/current-release.md` (P-CRAD-SPARSE row line 59).
- `docs/RUN_STATUS.md` (radiative rows; consistent with STATE.md).
- Note sections: `sections/radiative_corrections.tex` (183 lines), `sections/results.tex`, `sections/systematics.tex`.
- Machine records: `data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/` (pairv1 CSVs/JSONs), `plots/radiative/crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/`, `plots/radiative/cgammaborn_v4_releasebin_nozborn_7col_2026-10-09/`.

---

## 0. Authority resolution — two supersession layers (read this first)

The task briefing described the **2026-10-07 xFcut promotion** as the current state. Between the briefing and this report, a second promotion occurred. Every disposition below is given against **both** layers:

- **Layer 1 — 2026-10-07 xFcut promotion** (the briefing's facts, still the facts the note's 2026-10-08 edit implemented):
  - `c_rad_applied=TRUE`; Born release `born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/` (68 files).
  - Applied primary DJANGOH14 xFcut, integrated h+ `1.1418534312056339` / h− `1.1413755529189475`; RAPGAP31 xFcut formal alternative, h+ `1.1368044552974204` / h− `1.1362373358660744`; cross-generator `+0.444%`/`+0.452%`; both 490/490 finite.
  - Label reversal 2026-10-07: DJANGOH14 "applied primary", RAPGAP31 "formal alternative scheme" (was nominal/cross-check).
  - Canonical budget `total_uncertainty_z005_coherentprior_b1floor_crad13_2026-10-07/` (13 components; `c_rad_gen_rel` medians `0.5298%`/`0.5009%`, `c_rad_stat_rel` medians `0.4272%`/`0.4480%`).
- **Layer 2 — 2026-10-09 releasebin promotion (CURRENT as of this report)**, superseding parts of Layer 1 for application:
  - Critic A-F1: the xFcut tables binned the x cell with framework `XesGen` while the release/response bin gen x with the ISR-safe `x_es_gen`; per-bin |ΔC_rad|/C_rad median `3.5049%`, max `11.3712%`, integrated cancels, verdict NOT second-order (`STATE.md:1`; study `CRAD_XBINNING_SENSITIVITY_2026-10-08.md`).
  - Current Born-level release: `born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/` (installed 2026-10-09, manifest 46/46 sha256, `diff -rq` ZERO differences; integrated IBU h+ `13.107258` / h− `12.772782` nb). The xFcut Born release, the crad13_2026-10-07 budget, and the xFcut tables are SUPERSEDED, retained untouched as provenance (`STATE.md:96–98`).
  - Applied primary: DJANGOH14 releasebin `data/radiative/djangoh14_crad_releasebin_2026-10-09/`, integrated h+ `1.1415172920054786` / h− `1.1410367377289135`. Formal alternative: RAPGAP31 releasebin **emfix** `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/`, integrated h+ `1.1342370401351234` / h− `1.1336689310022947` (the pre-emfix `rapgap_crad_releasebin_2026-10-09/`, h+ `1.1364508941898332` / h− `1.1358814928541603`, is superseded for use; emfix added `53,773.7 nb^-1` to the e− NONRAD denominator, `13,416,991.0 → 13,470,764.7 nb^-1`).
  - Canonical budget: 13-component `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/` — `c_rad_gen_rel` medians h+ `0.5844%` / h− `0.5804%`, `c_rad_stat_rel` medians h+ `0.4272%` / h− `0.4429%`, totals h+ median `5.7093%` / h− `6.1523%` (`STATE.md:174`; status doc lines 124–126).
  - 2026-10-09 blocker closures (STATE.md addendum A–G, lines 170–176): EMPZ>45 CLOSED (retain `GenEmpz>45`); z<0.05 xF-definition sensitivity CAVEAT-ONLY (DJ14 `9.925%`/`8.023%`, RG31 `6.461%`/`5.200%`; supersedes the xFcut-basis `±6.8%`/`±3.9%`); `P-CRAD-LUMI` relabeled FILE-SET-RECONCILED (generator-log-open; MCDB CGI permanently disabled externally); DJANGOH e− RAD `4.67%` shortfall ROOT-CAUSED (variable chunk sizing; pool == retained 1,373/1,373; ZERO bias to sigma_rad(em)/C_rad(em), precision only, already carried in `c_rad_stat_rel`).

**Consequence for this audit:** the note's 2026-10-08 edit was written correctly against Layer 1 and was overtaken by Layer 2 the next day. Dispositions below state both ("implemented, now one promotion stale").

---

## 1. Task 1 — Disposition of prior proposals R1–R7 and UNVERIFIED items 10–11

All line numbers refer to `analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/radiative_corrections.tex` (183 lines, commit `3469e10`, 2026-10-08; that commit changed only the `.tex`).

### 1.1 Disposition table

| Item | Prior severity | Disposition | Evidence |
|---|---|---|---|
| R1 — historical DJANGOH14 v1 numbers as current | HIGH | **IMPLEMENTED** (2026-10-08), **now one promotion stale** | tex:27–45 replace v1 with the xFcut campaign; quoted numbers superseded 2026-10-09 by releasebin (§1.2) |
| R2 — both figures use superseded fLumi-v3 route | HIGH | **PARTIALLY IMPLEMENTED — FIGURES STILL PENDING** | text pointer added tex:54–56; both `\includegraphics` (tex:60, tex:67) still the old fLumi-v3 PDFs (md5 ≠ xFcut package) |
| R3 — h− PDF under h+ label | MEDIUM | **IMPLEMENTED** | tex:60 now the hplus PDF; captions tex:61/tex:68 correct; residual: labels still read `flumi-v3` (tex:62, tex:69) |
| R4 — missing current-baseline status caveats | MEDIUM | **MOSTLY IMPLEMENTED; two clauses now stale; one guard still absent** | tex:46–51 (applied-not-publication-grade, blind quarantine, exposure caveat); EMPZ clause stale, P-CRAD-LUMI clause stale, α<0.2 guard absent |
| R5 — "corrected to the e−p cross section" | MEDIUM | **IMPLEMENTED** (claim removed; mixed-beam statement + photon-only subsection) | tex:20–25; tex:72–183 |
| R6 — DJANGOH-only framing inverts nominal/cross-check | MEDIUM | **SUPERSEDED BY THE 2026-10-07 LABEL REVERSAL**; note now matches the current assignment | tex:32–39 (DJ14 primary) matches status doc line 24 |
| R7 — stale red TODO on MC statistics | LOW | **IMPLEMENTED IN SUBSTANCE** | TODO gone; tex:39 `490/490`; tex:42–43 stat medians quoted; tex:43–45 low-MC-stat sentence; max `5.74%` and the (now closed) policy sentence not carried |
| UNVERIFIED 10 — e−p claim (= R5) | UNVERIFIED | **RESOLVED** (removed/rewritten) | tex:20–22 (mixed-beam), tex:85–97 + tex:153–158 (charge-symmetry reasoning) |
| UNVERIFIED 11 — v1 luminosities 4,108,957 / 3,560,233 | UNVERIFIED | **RESOLVED by removal** | historical luminosities gone; note now cites oomclumi-traced exposures (tex:47–49) and the exact NOZ closure (tex:119) |

### 1.2 Per-item detail

**R1 — IMPLEMENTED, one promotion stale.** The note (tex:27–45) now states the xFcut campaign definition (xFGen<0 recomputed q-nominal both legs, EMPZ>45, 150<Q²<20,000 GeV², 0.1<y<0.7, 490 bins), the applied primary DJANGOH14 xFcut with integrated `1.1418534`/`1.1413756`, RAPGAP31 alternative `1.1368045`/`1.1362373`, 490/490 finite, cross-generator `+0.444%`/`+0.452%`, stat medians `0.4272%`/`0.4480%`, per-bin spans `[0.840,1.572]`/`[0.898,1.530]`. This exactly implements R1's intent and matches the status doc lines 24–33 as of 2026-10-08. The historical v1 values (1.0553/1.0554, 478/490) are gone. **However**, every quoted number is now superseded-for-application by the 2026-10-09 releasebin rederivation: integrated DJ14 `1.1415173`/`1.1410367`, RG31 emfix `1.1342370`/`1.1336699`, cross-generator `+0.4458%`/`+0.4539%` on the pre-emfix releasebin pair, stat medians `0.4272%`/`0.4429%` (`STATE.md:65,97,98`). The per-bin spans were verified against the xFcut pairv1 machine records and are xFcut-basis; releasebin spans are not quoted in STATE.md and must be re-read from the applied releasebin table. Note also that R1's originally proposed replacement text ("RAPGAP31 (nominal) … not yet applied, `c_rad_applied=false`") was itself superseded twice before the edit (2026-10-07 application + label reversal); the note editors correctly tracked both — the section says "applied" (tex:46) with DJ14 primary.

**R2 — text done, figures PENDING (the last HIGH item).** The text now points to the xFcut 7-column package (tex:54–56 → `plots/radiative/crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/`, verified to exist with `crad_v4_review.pdf` + hplus/hminus × zoom/wide pages). But both `\includegraphics` (tex:60, tex:67) still read `figures/crad_djangoh14_vs_rapgap31_fLumi_v3_7col_{hplus,hminus}_zoom.pdf`, and those PDFs are the **superseded fLumi-v3 route** products (committed via Overleaf 2026-10-04; md5 differs from the same-named files inside the xFcut package; commit `3469e10` changed only the `.tex`, so the figures were never swapped). The plotted integrated correction is ≈1.025 (fLumi-v3: h+ `1.0248113`, h− `1.0249101`, status doc line 72) vs the text's ≈1.142 — the figure content contradicts the section text by ~11%. **Filename trap:** the xFcut package reuses the exact fLumi-v3 filenames (the plotter is the frozen fLumi-v3 snapshot script repointed at xFcut inputs), so a reader cannot distinguish old vs new content by name — only by md5 or content. The interim fix is a file swap (copy the package's same-named zoom pages over the note's figures, md5-verify), and the full fix is regeneration from the releasebin tables (see §4, N1/N2).

**R3 — IMPLEMENTED.** tex:60 includes the hplus PDF; captions read "for $\hplus$" (tex:61) and "for $\hminus$" (tex:68). Residual: both `\label`s still say `flumi-v3` (tex:62, tex:69) — cosmetic now, actively misleading after R2's swap unless the labels are renamed (recommended: `fig:crad:djangoh-rapgap-hplus` / `-hminus`).

**R4 — MOSTLY IMPLEMENTED; two stale clauses; one absent guard.** Implemented (tex:46–51): "applied but not publication-grade"; blind 2026-09-30 quarantine with under-correction `+0.35–0.41%` confined to z<0.05 (still correct); NORAD/RAD exposure statements as of 2026-10-08. Now stale: (a) "the EMPZ>45 convention decision remains open" (tex:46–47) — CLOSED 2026-10-09, retain `GenEmpz>45` (STATE.md:170-A); (b) "NORAD exposures are manifest-level diagnostics (the P-CRAD-LUMI publication-grade provenance is open)" (tex:47–49) — relabeled FILE-SET-RECONCILED (generator-log-open), all six legs closed (STATE.md:173-D). Still absent: the reco-only α<0.2 guard sentence (R4 recommendation; status doc §Alpha0p2 Guard, lines 78–80). Also missing relative to the 2026-10-09 state: the z<0.05 caveat-only sentence, the Born-vs-RAD-MC comparison guard, the mixed-beam-only validity statement for the full-EW correction, and the 13-component budget cross-reference (see §4, N5–N8).

**R5 — IMPLEMENTED.** The "corrected to the e−p cross section" sentence is gone. tex:20–25 states the mixed-beam → mixed-beam full-EW Born mapping with the per-bin post-unfold application formula; tex:72–183 adds the photon-only Born variant subsection with the full charge-convention reasoning (charge-symmetric photon-only target, tex:85–97; per-beam variant differences only through RAD legs, tex:159–164). This resolves UNVERIFIED 10 and goes beyond R5's ask by deriving and documenting the per-beam numbers.

**R6 — SUPERSEDED BY EVENTS; note now correct.** R6 criticized the DJANGOH-only framing because RAPGAP31 was then nominal. The 2026-10-07 user decision reversed the labels (DJANGOH14 applied primary, RAPGAP31 formal alternative; status doc line 24, STATE.md:92), making the note's DJ14-primary framing (tex:32–39) the correct current assignment. No R6 action remains beyond the releasebin number refresh (R1/N2).

**R7 — IMPLEMENTED IN SUBSTANCE.** The red TODO is gone. tex:39 records `490/490` finite; tex:41–43 carries the cross-generator component and the applied table's statistical component with per-charge medians (`0.4272%`/`0.4480%`); tex:43–45 keeps the low-MC-statistics sentence (high z, high Q², edge pT). Not carried: the maximum (`5.74%`, current-release.md:59) and any policy sentence — now moot in the originally proposed form because **P-CRAD-SPARSE is CLOSED (pre-application)** per `docs/status/current-release.md:59` ("`490/490` finite bins per generator per charge, rel-stat median `0.42%`, max `5.74%`; the applied table's per-bin statistics are now carried as the `c_rad_stat_rel` budget component"). Recommended wording update in §4. The h− median quoted is xFcut-basis (releasebin: `0.4429%`).

**UNVERIFIED 10 — RESOLVED.** See R5. The claim's ambiguity was replaced by an explicit two-convention treatment in which the only e−p target is the photon-only Born variant, and that target is shown to be charge-symmetric so the beam mixture enters only through the RAD denominator (tex:153–158).

**UNVERIFIED 11 — RESOLVED by removal.** The v1 hard-coded luminosities (4,108,957 / 3,560,233 nb⁻¹) no longer appear. The note cites oomclumi-traced RAD exposures and manifest-level NORAD exposures as of its writing (tex:47–49; now relabeled, see R4) and, for the photon variant, the exact weighted closure `14,119,125.013920 nb^-1` (tex:119), which matches the machine record in `EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md:684,703`.

---

## 2. Task 2 — Consistency review of the 10 new documents vs STATE.md and the status authority

**Headline: zero number disagreements.** Every numeric statement checked in the 10 documents traces to the machine records or matches STATE.md / the status authority. All findings below are staleness, missing supersession/closure markers, or internal header-vs-tail inconsistencies — not value contradictions. Twelve findings (D1–D12); four have in-session re-verified line evidence quoted.

### D1 — Promotion record presents the xFcut Born release as CURRENT with no releasebin pointer (HIGH)
`CRAD_APPLICATION_PROMOTION_2026-10-07.md:16`: "**Born release (CURRENT, installed 2026-10-07):** `born_crad_dj14xfcut_..._2026-10-07/`". Superseded 2026-10-09 by `born_crad_dj14relbin_..._2026-10-09/` (STATE.md:96 names the supersession explicitly, but the promotion record carries no in-doc pointer). A reader entering through the promotion record quotes a superseded release as current.

### D2 — Promotion record's "Blockers Carried OPEN (eleven, verbatim; do not close)" is now four clauses stale (MEDIUM)
`CRAD_APPLICATION_PROMOTION_2026-10-07.md:99` carries eleven open blockers verbatim. At least four have since closed/changed: EMPZ>45 convention (CLOSED 2026-10-09); P-CRAD-LUMI (relabeled FILE-SET-RECONCILED); DJANGOH e− RAD 4.67% shortfall (ROOT-CAUSED, zero bias); per-bin z<0.05 `±6.8%`/`±3.9%` sensitivity (superseded by releasebin caveat numbers `9.925%/8.023%` / `6.461%/5.200%`). STATE.md's addendum (lines 166–176) records all of this, but nothing in the promotion record points forward. As a dated promotion record it is arguably historical — but it self-describes as the promotion registry surface ("do not close"), so it needs a one-line forward pointer to the 2026-10-09 addendum.

### D3 — Statfloor study's "Adopt V4(1.0)" recommendation is VOID with no in-doc marker (MEDIUM)
`CRAD_STATFLOOR_STUDY_2026-10-07.md:171` recommends "Adopt V4(1.0)" (details lines 184–207, definition guidance lines 245–247). The status authority voids it: "The `+0.077 pp` / `+0.021 pp` total impact and the V4(1.0)-blind recommendation … -> VOID: they priced the retired floor on the superseded 2026-10-02 budget base and on the blind pairing" (status doc line 66; STATE.md:93). No VOID marker exists anywhere in the study doc (verified: no "VOID" match in the file). Mitigation: the promotion record classifies the doc as protected audit-only (Class D, promotion record line 117), and the note does not cite it. The study's regime-B bookkeeping conclusion (carry the applied table's `relative_stat_unc` as its own component; retire the max-form floor once C_rad is applied — lines 132–143) **was** implemented as `c_rad_stat_rel`, so only the regime-A recommendation is void.

### D4 — Generator-spread doc: contemporaneous nominal-label contradiction (resolved), superseded sizings, no adoption marker (MEDIUM)
`CRAD_UNC_GENERATOR_SPREAD_2026-10-07.md:1,3`: "DJANGOH14 nominal … **Decision being implemented (user, 2026-10-06):** nominal `C_rad` = DJANGOH14". The contemporaneous `CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md:21–22` still reads "RAPGAP31 (nominal)" / "DJANGOH14 (cross-check)". The contradiction was real on 2026-10-06/07 and was resolved by the 2026-10-07 reversal — but the provenance doc was never annotated, so a reader of the two dated records sees opposing "nominal" labels with no marker explaining the sequence. Additionally: (a) the spread doc's 12-component sizings use the superseded 2026-10-02 base budget (line 232) and its A-vs-B blind comparisons (lines 17, 30, 45–49) are historical; (b) the adopted component (Step 3, line 119: V3 = 0.5×GaussSmooth, symmetric, per-bin relative, half-difference — lines 137–139) **is** the implemented `c_rad_gen_rel` (status doc line 44: "un-floored `V3 = 0.5 x GaussSmooth_pTrow(|rel|)` at the DJ14-xFcut vs RG31-xFcut pair"), but the doc carries no adoption marker. 

### D5 — NOZBORN doc's pending-closure status superseded without in-doc marker; actual closure fell outside the doc's own expectation window (LOW)
`NOZBORN_EXPOSURE_DERIVATION_2026-10-08.md:13–16`: "Σw ≈ 94,135,964 (estimate; exact … pending on Condor) … **SAMPLED_ESTIMATE** — replace with the exact weighted closure before any PASS verdict. Queue value to report: **14032218**"; line 130: "Expected within ±0.5% of 14,032,218". The exact closure was subsequently computed and recorded in `EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md:684,703`: `L_MC = 14,119,125.013920 nb^-1`, all gates G1–G7 True — and is quoted in the note (tex:119). Two issues: (a) no in-doc closure marker or cross-reference in the NOZBORN doc; (b) the exact value differs from the queued estimate by **+0.62%**, slightly outside the doc's stated ±0.5% expectation — worth a one-line in-doc note since the doc framed the expectation as a validation bound.

### D6 — EMINUS doc: 4.67%-shortfall caveat and EMPZ-open clause superseded without in-doc markers (MEDIUM)
`EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md:139–140` documents the e− GenDIS 4.67% count shortfall as caveat 1 with per-beam-variant bias potential up to ~4.7% at full weight (line 124: "All caveats in the next subsection still apply at full weight"). The 2026-10-09 root cause (STATE.md:173-D) shows ZERO bias to sigma_rad(em)/C_rad(em) — variable chunk sizing, precision only, already carried in `c_rad_stat_rel`. Line 155 lists the Empz-threshold convention decision as part of the same open blocker — closed 2026-10-09. Neither closure is marked in the doc. Mitigation: the doc is a dated investigation record (promotion record Class D protects it as audit-only), and §7g (lines 684–703) is the design/validation record the note cites; but the caveats subsection is the doc's most quote-prone part and now overstates the shortfall's physics impact.

### D7 — BRITZGER doc §5.3-1 executable question resolved 2026-10-08, unmarked (LOW)
The BRITZGER method review leaves open whether the photon-only target requires a regenerated executable; the resolution (user decision 2026-10-08: primary track = existing NOZ Born sample) is recorded only in `EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md:468`. The BRITZGER doc has no resolution marker or cross-reference. Same dated-record mitigation as D6.

### D8 — EMPZ sensitivity doc's closing "remain independently open" closed/relabeled the same day, unmarked (MEDIUM)
`CRAD_EMPZ_CONVENTION_SENSITIVITY_2026-10-09.md:66`: "The user convention decision and `P-CRAD-LUMI` remain independently open". The user decision (retain `GenEmpz>45`) was taken the same day, 2026-10-09 (STATE.md:170-A); P-CRAD-LUMI was relabeled the same day. The doc's header (line 3) correctly scopes itself as decision evidence ("does not … decide the convention"), so only the tail sentence is stale — but it is the doc's last word and needs a one-line closure pointer. The doc's physics content (45–70 window identical to >45; >50/>55/no-cut/35–70 shift +2.7%/+19.4%/−3.0%/−2.0% and are truth redefinitions) matches STATE.md exactly.

### D9 — Exposure audit's Step-5 blocker text retains two same-day-stale clauses (MEDIUM)
`CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md:307–308` still lists: "PROVISIONAL xFcut baseline (promotion-gate review only), open EMPZ>45 convention decision (a future flip supersedes the applied table), mixed-beam …". Two stalenesses: (a) the applied baseline became releasebin on 2026-10-09 (the audit's file-set reconciliation itself transfers, because the releasebin rederivation reused the campaign exposures verbatim — but the doc does not say so); (b) the EMPZ clause was closed the same day. The doc's own repair addendum (lines 319, 351) records the emfix remedy and the upgraded overall verdict FILE-SET-RECONCILED (generator-log-open) — consistent with STATE.md:173-D — making the un-updated Step-5 text an internal inconsistency within one document.

### D10 — xBinning doc header's "new budget component needed" prescription mooted by the rederivation (LOW)
`CRAD_XBINNING_SENSITIVITY_2026-10-08.md:3` (Status line): "PASS — material per-bin correction-definition effect; **new budget component needed** before this Born correction can be publication-grade." The resolution actually adopted (2026-10-09) was the body's primary option (line 99: "resolved by rederiving/replacing the applied correction table with release-safe x binning") — the releasebin rederivation replaced the applied table entirely and **no new budget component was added** (the crad13_releasebin_emfix budget keeps 13 components). The in-doc 2026-10-09 addendum (lines 101–107) records the rederivation cross-check but the header Status line was not updated. A reader quoting the header would expect a 14-component budget that does not exist.

### D11 — Status authority is internally inconsistent: 2026-10-08 header/top vs 2026-10-09 tail (HIGH — most consequential doc finding)
`docs/status/radiative-corrections.md` line 5: "**Last updated:** 2026-10-08"; lines 7–9 and 20–44 present the xFcut baseline as the APPLIED CURRENT state (xFcut table paths, integrated `1.1418534…`/`1.1413756…`, `+0.444%`/`+0.452%`, crad13_2026-10-07 budget medians, "label REVERSED 2026-10-07" table); lines 47–59 carry the eleven blockers verbatim with "do not close" (including the open-EMPZ, open-P-CRAD-LUMI, ±6.8/±3.9, and 4.67%-unresolved clauses); lines 61–68 mark the statfloor V4(1.0) recommendation VOID. Then lines 90–126 record the 2026-10-09 releasebin/emfix state (EMPZ CLOSED, z<0.05 caveat numbers, RG31 emfix, `c_rad_gen_rel` 0.5844%/0.5804%). **The top of the file — the declared "Status authority" — carries no releasebin supersession marker.** A reader stopping at the top (or grepping the table) quotes superseded xFcut values as current. Note the top's xFcut numbers are still internally correct as a dated snapshot and STATE.md carries the full supersession (lines 89–98) — but this file is the compact authority the workspace points to first. This mirrors, at doc level, exactly the R1-pattern error the note audit was run to catch.

### D12 — STATE.md's note-text blocker is half-stale (LOW)
`STATE.md:75`: "`sections/results.tex:103` and `sections/systematics.tex` still claim no radiative correction is applied — note update PENDING." As of 2026-10-08 the results.tex claim is gone (the section is now Born-basis; results.tex:102–104 quotes the applied-campaign nonrad/rad ratios `1.136523`/`1.141616`), while `sections/systematics.tex:13` still reads "The remaining missing component is the [radiative uncertainty]" (pre-promotion, 10-component table, no `c_rad_gen_rel`/`c_rad_stat_rel`/phi_h rows). The blocker should now name only systematics.tex (plus the radiative section's Layer-2 staleness, §4).

### 2.1 Cross-document observations (not counted as findings)

- **No contradictions among the 10 docs' numbers and STATE.md.** Spot-verified pairs: promotion record integrated values = status doc lines 25–27 = STATE.md:14; generator-spread table B/C values = xFcut tables; statfloor floor sizings = its own machine outputs (historical basis, correctly stated in-doc as sizings, line 217); NOZBORN four-leg Σw/L consistency (lines 94–99) = EMINUS §7g closure; EMPZ shifts = STATE.md:170-A; audit leg closures = STATE.md:173-D; zlow addendum = STATE.md:171-B.
- The promotion record's Class A–F doc-sweep classification (line 117) is itself the workspace's intended mitigation for D3–D7: dated study records are deliberately left unedited as historical. The findings above stand because each doc is either a live authority surface (D1, D2, D11) or carries a quote-prone unmarked stale clause; the recommended remedy is a one-line forward pointer, not a rewrite.

---

## 3. Task 3 — Review of the updated `radiative_corrections.tex` (183 lines)

### 3.1 What is correct now (verified against machine records)

- **Mixed-beam definition and application formula (tex:8–25).** C_rad = σ_nonrad/σ_rad, per-bin post-unfold, mixed-beam → mixed-beam full-EW Born; matches the status authority definition (status doc lines 9–16) and the mixed-beam-only validity blocker.
- **Photon-only Born subsection (tex:72–183) — all numbers verified against the pairv1 machine records** (`data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/`): C_γBorn integrated `1.041267`/`1.040456` (tex:138–139), F_EWγ `0.9119`/`0.9116` (tex:139–140), identity tolerance `10^-9` with observed max `2.3×10^-16` (tex:131), NoZ L_MC `14,119,125.013920 nb^-1` (tex:119, = EMINUS:684,703), SIGTOT `6.7086 nb` + LPIN(2)=0 echo (tex:104–106), running-α `+8.61%` at Q²>100 (corrected 7.2859 vs Born 6.7086 nb), full-EW NORAD 7.3239 nb, `+0.5%` Z+γZ, `+11.27%` at Q²>1000, F_EWγ per-bin range `[0.785,0.991]` (tex:143–151), per-beam `(C_ep−C_em)/C_em = +0.99%`/`+0.87%` with medians `1.76%`/`1.17%` (tex:160–163), integrated range band `[0.95,1.12]` (tex:135–136). All match.
- **Blind-table quarantine sentence (tex:49–51)** — still correct (under-correction +0.35–0.41%, z<0.05, quarantined).
- **Provisional/diagnostic status of the photon variant (tex:166–171)** — correct (c_rad_applied=false in its contract; DJANGOH-only; no photon-only RAPGAP sample).
- **results.tex** (adjacent): Born-basis comparison section updated 2026-10-08 (commit `411d22e`); the old FALSE "no radiative correction" claim is gone.

### 3.2 Remaining issues in `radiative_corrections.tex` (N1–N10), with line numbers

**N1 (HIGH, carries R2) — Both comparison figures are still the superseded fLumi-v3 route.** tex:60 and tex:67 include `figures/crad_djangoah…fLumi_v3_7col_{hplus,hminus}_zoom.pdf` whose content shows integrated C_rad ≈ 1.025 vs the section's ≈ 1.14/1.14 (~11% magnitude error), and whose md5 differs from the same-named pages of the current xFcut package. The labels (tex:62, tex:69) still read `flumi-v3`.

**N2 (HIGH) — The baseline paragraph quotes the superseded xFcut numbers as "the current baseline" (tex:27–45).** Specifically: "The current baseline is the xFcut campaign of 2026-10-05/06" (tex:27) — false since 2026-10-09; DJ14 integrated `1.1418534`/`1.1413756` (tex:35) → releasebin `1.1415173`/`1.1410367`; RG31 `1.1368045`/`1.1362373` (tex:38–39) → emfix `1.1342370`/`1.1336699`; table paths tex:33–34/36–37 → releasebin directories; cross-generator `+0.444%`/`+0.452%` (tex:40) → `+0.4458%`/`+0.4539%` (pre-emfix releasebin pair; per-bin component medians `0.5844%`/`0.5804%` on the emfix pair); stat median h− `0.4480%` (tex:43) → `0.4429%` (h+ `0.4272%` unchanged); per-bin spans `[0.840,1.572]`/`[0.898,1.530]` (tex:43–44) are xFcut-basis — re-read from the applied releasebin table. Note: the integrated spread on the **emfix** pair is ≈ `+0.642%`/`+0.650%` by arithmetic from the STATE.md-quoted integrated values — this is derived here, not a recorded quote; if the note wants an integrated spread on the current pair, record it in the status authority first.

**N3 (MEDIUM) — "the EMPZ>45 convention decision remains open" (tex:46–47) is stale.** Closed 2026-10-09: retain `GenEmpz>45` (release-matched truth contract; no applied-table change).

**N4 (MEDIUM) — Exposure sentence (tex:47–49) is stale.** "NORAD exposures are manifest-level diagnostics (the P-CRAD-LUMI publication-grade provenance is open)" → all six legs file-set-reconciled; P-CRAD-LUMI relabeled FILE-SET-RECONCILED (generator-log-open; MCDB CGI permanently disabled externally). "RAD exposures are oomclumi-traced" remains true (both RAD legs CLOSED_OOMCLUMI_EXACT). "Not publication-grade" remains true (releasebin baseline PROVISIONAL, promotion-gate review only).

**N5 (MEDIUM) — Missing reco-only α<0.2 guard** (R4 residual): "The α<0.2 cone veto is a reconstruction-level cut and is never imposed on the gen-only C_rad selection" (status doc lines 78–80).

**N6 (MEDIUM) — Missing z<0.05 caveat-only sentence.** Results in the z<0.05 row carry an xF-definition sensitivity with releasebin maxima DJ14 `9.925%`/`8.023%`, RG31 `6.461%`/`5.200%`, documented as a caveat, not a budget component (STATE.md:171-B; zlow addendum).

**N7 (MEDIUM) — Missing Born-vs-RAD-MC guard and mixed-beam-only validity for the full-EW correction.** Status doc lines 55–56: "Born values must not be compared against RAD-level MC predictions without the same correction"; "mixed-beam only, not valid for any per-beam or e− Born target". (The photon subsection correctly states its own charge convention; the full-EW part of the section does not carry the mixed-beam-only clause.)

**N8 (LOW) — Missing 13-component budget cross-reference.** The section names `c_rad_gen_rel`/`c_rad_stat_rel` (tex:41–43) but never states they are components of the canonical 13-component budget (`total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/`), nor quotes the gen-spread per-bin medians on the current pair (`0.5844%`/`0.5804%`).

**N9 (MEDIUM) — Photon-only subsection pointers/values are one refresh stale (tex:138–139, 173–180).** The quoted C_γBorn integrated values (`1.041267`/`1.040456`) and both pointers (`djangoh14_nozborn_crad_final_xfcut_2026-10-07/`, `cgammaborn_v4_xfcut_nozborn_7col_2026-10-08/`) are the xFcut basis. A releasebin refresh exists: `plots/radiative/cgammaborn_v4_releasebin_nozborn_7col_2026-10-09/` (verified to exist: review + hplus/hminus × zoom/wide pages) with integrated C_γBorn `1.040961`/`1.040147`. Either re-quote on the releasebin basis with the new pointers, or state the xFcut basis explicitly and point to the refresh.

**N10 (LOW) — tex:156 lacks the fiducial qualifier.** "the integrated cross section is 218.84216~pb for both beam charges" — this is the selected fiducial cross section at Q²>100 GeV² (the sample's production fiducial), not the full-phase-space generator cross section (`6708.559 pb`, tex:104). Add "in the $\Qsq>100~\GeV^{2}$ fiducial".

**Adjacent sections (not counted in N1–N10):** results.tex:102–104 quotes the xFcut nonrad/rad ratios `1.136523`/`1.141616` (superseded basis; STATE.md:40 records them as xFcut-campaign values) and the comparison package is superseded-basis (blocker G, STATE.md:176); systematics.tex:13 still claims the radiative-uncertainty component is missing (FALSE since 2026-10-07; the note-update blocker is half-stale, D12).

### 3.3 LaTeX-ready replacement text

**(a) Replacement for the baseline paragraph, tex:27–56** (releasebin-current; keep tex:8–25 unchanged):

```latex
The current baseline is the release-safe rederivation (``releasebin'') of
2026-10-09, which supersedes the xFcut campaign of 2026-10-05/06 for
application: generator-level only, with the current-hemisphere cut
xFGen$<$0 (recomputed $x_F$, $\hat q$-nominal) imposed on both the RAD and
NORAD legs, the nominal EMPZ$>$45 selection (retained by decision
2026-10-09 as the release-matched truth contract), the generator-level
electron-scattering fiducial $150<\Qsq<20\,000~\GeV^{2}$ and $0.1<y<0.7$,
and the 490 published bins (245 per charge).  The $x$ cell of every bin is
assigned with the release-safe, ISR-safe $x_{\mathrm{es,gen}}$ variable,
matching the response and release binning; the superseded xFcut tables
assigned $x$ with the framework \texttt{XesGen} variable, a per-bin
definition mismatch with a median effect of $3.50\%$ and a maximum of
$11.37\%$ that cancels in the integral but is not second-order bin by
bin.  The applied primary is the DJANGOH14 releasebin table
\path{crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv} under
\path{sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/},
with integrated factors 1.1415173 for $\hplus$ and 1.1410367 for
$\hminus$; the RAPGAP31 table of the same campaign, corrected for the
repaired e$^-$ NORAD exposure and installed under
\path{sidis_highQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/},
is the formal alternative scheme, with integrated factors 1.1342370 and
1.1336689.  Both tables are 490/490 finite; the releasebin tables share
file names with the superseded xFcut tables, so directory-qualified paths
are mandatory.  The generator spread is carried as the
\texttt{c\_rad\_gen\_rel} systematic component (per-bin medians $0.5844\%$
for $\hplus$ and $0.5804\%$ for $\hminus$ on the emfix-corrected pair)
and the statistical uncertainty of the applied table as
\texttt{c\_rad\_stat\_rel} (per-bin medians $0.4272\%$ and $0.4429\%$);
these are the two radiative components of the 13-component uncertainty
budget.  Bins at high $z$, high $\Qsq$, and edge $\pTh$ have low MC
statistics; the sparse-bin policy closed pre-application, with all bins
finite and the per-bin statistics carried as \texttt{c\_rad\_stat\_rel}.
The baseline is applied but not publication-grade: the exposures of all
six RAD/NORAD legs are file-set-reconciled (the former P-CRAD-LUMI
provenance blocker is relabeled FILE-SET-RECONCILED, generator-log-open;
the generator-log cross-check is permanently blocked externally because
the MCDB CGI interface is disabled), and the releasebin baseline remains
provisional, its review closed only as a promotion gate.  Results in the
$z<0.05$ row carry an $x_F$-definition sensitivity (maxima $9.925\%$
$\hplus$ / $8.023\%$ $\hminus$ for DJANGOH14 and $6.461\%$ / $5.200\%$
for RAPGAP31), documented as a caveat and not included in the uncertainty
budget.  The earlier blind (xF-cut-free) FINAL tables of 2026-09-30
under-correct by $+0.35$--$0.41\%$ integrated, with the bias confined to
the $z<0.05$ row, and are quarantined; the xFcut tables of 2026-10-05/06
are superseded for application and retained untouched as provenance.  The
$\alpha<0.2$ scattered-electron cone veto is a reconstruction-level cut
and is never imposed on the gen-only $C_{\mathrm{rad}}$ selection; the
correction is valid for the mixed-beam data only; and Born-level values
must not be compared against RAD-level MC predictions without applying
the same correction.  The rederivation study and provenance are
documented in
\path{sidis_highQ2/docs/radiative/CRAD_XBINNING_SENSITIVITY_2026-10-08.md}
and the releasebin promotion record in \path{sidis_highQ2/STATE.md}.
```

Caveats for the editors: (i) the per-bin span sentence was dropped from this draft because releasebin spans are not yet recorded — re-read `[min,max]` per generator from the applied releasebin table and re-insert; (ii) if an integrated cross-generator figure is wanted on the emfix pair (≈ `+0.642%`/`+0.650%`, derived here from the STATE.md-quoted integrated values), record it in the status authority before quoting it; (iii) verify the releasebin CSV filename inside `djangoh14_crad_releasebin_2026-10-09/` matches the xFcut name (STATE.md:98 says the releasebin CSVs share filenames; the path above assumes it).

**(b) Interim figure fix for tex:58–70** (content swap; the xFcut package reuses the exact fLumi-v3 filenames, so the `.tex` lines can stay while the PDFs are replaced — md5-verify against the package):

```latex
\begin{figure}[p]
  \centering
  \includegraphics[width=1.05\textwidth,height=0.82\textheight,keepaspectratio]{figures/crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hplus_zoom.pdf}
  \caption{Comparison of $C_{\mathrm{rad}}=\sigma_{\mathrm{nonrad}}/\sigma_{\mathrm{rad}}$
    for DJANGOH14 and RAPGAP31 for $\hplus$, on the xFcut basis
    (integrated $1.1419$ vs $1.1368$); to be regenerated from the
    releasebin tables.}
  \label{fig:crad:djangoh-rapgap-hplus}
\end{figure}

\begin{figure}[p]
  \centering
  \includegraphics[width=1.05\textwidth,height=0.82\textheight,keepaspectratio]{figures/crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hminus_zoom.pdf}
  \caption{Comparison of $C_{\mathrm{rad}}=\sigma_{\mathrm{nonrad}}/\sigma_{\mathrm{rad}}$
    for DJANGOH14 and RAPGAP31 for $\hminus$, on the xFcut basis
    (integrated $1.1414$ vs $1.1362$); to be regenerated from the
    releasebin tables.}
  \label{fig:crad:djangoh-rapgap-hminus}
\end{figure}
```

The **full** fix (recommended before circulation): regenerate the 7-column comparison from the releasebin tables (DJ14 releasebin vs RG31 releasebin emfix) via the frozen snapshot plotter with explicit current-table inputs (status doc line 84), rename the outputs basis-explicitly (the `fLumi_v3` filename stem is a trap), and update the `\includegraphics` names and captions accordingly. The status doc's own plot pointer (line 85) still names the xFcut package, so this regeneration is also an open action on the status-authority side (D11).

**(c) Targeted edits in the photon subsection:**

- tex:138–139, add the basis (if staying on xFcut values):
  ```latex
  The integrated results are $C_{\gamma\mathrm{Born}} = 1.041267$ for $\hplus$
  and $1.040456$ for $\hminus$ (xFcut basis; the releasebin refresh gives
  $1.040961$ and $1.040147$), with integrated $F_{\mathrm{EW}\gamma}$ of
  $0.9119$ and $0.9116$.
  ```
- tex:156, add the fiducial qualifier:
  ```latex
  section is 218.84216~pb for both beam charges in the
  $\Qsq>100~\GeV^{2}$ fiducial),
  ```
  (i.e., replace "the integrated cross section is 218.84216~pb for both beam charges" with "the integrated selected cross section in the $\Qsq>100~\GeV^{2}$ fiducial is 218.84216~pb for both beam charges".)
- tex:179–180, add the refresh pointer after the xFcut plot path:
  ```latex
  ; the releasebin refresh of these review plots is in
  \path{sidis_highQ2/plots/radiative/cgammaborn_v4_releasebin_nozborn_7col_2026-10-09/}.
  ```

---

## 4. Task 4 — Study-to-proposal lineage: what each new study resolves / narrows / leaves open

| Study (date) | Prior item it answers | Effect | Status after the study |
|---|---|---|---|
| EMINUS investigation (2026-10-07) | R5 / U10 (beam-charge convention) | **Resolves.** Derives the per-beam factors (full-EW per-beam F-factor +0.44–0.60% integrated; photon-variant per-beam (C_ep−C_em)/C_em +0.99%/+0.87%, medians 1.76%/1.17%): no per-beam treatment is needed or valid for the mixed correction; the only e−p target is the photon-only Born variant, reached exactly by a charge-blind factor. Also documents the e− RAD 4.67% shortfall caveat (later root-caused, D6) and contains §7g, the design/validation record the note's photon subsection cites. | R5/U10 closed; e− shortfall later closed by the audit (2026-10-09) |
| BRITZGER EW gZ/NoZ method review (2026-10-07) | R5 support (Born-convention method) | **Resolves** the method basis for a photon-only target: Z+γZ contributes +0.60% (e−) / +0.11% (e+) at Q²>100, so a full-EW e−p target would be charge-dependent; the photon-only target is charge-symmetric. Its §5.3-1 executable question was resolved 2026-10-08 (existing NOZ sample; EMINUS:468) — unmarked in-doc (D7). | Method basis closed; exe question closed 2026-10-08 |
| NOZBORN exposure derivation (2026-10-08) | Supports the photon subsection (U11-adjacent provenance) | **Narrows.** Establishes weighted-normalization necessity (Σw/N = 2.353399 ≠ 1), four-leg consistency, and the closure path; the exact closure (14,119,125.013920 nb⁻¹) landed in EMINUS §7g, outside this doc (D5). | Exposure closed at manifest level; publication-grade generator-log provenance permanently blocked externally |
| Statfloor study (2026-10-07) | R7 (MC-statistics policy) | **Resolves the bookkeeping, voids its own headline recommendation.** Regime-B conclusion (carry `relative_stat_unc` of the applied table as `c_rad_stat_rel`; retire the max-form floor) was implemented in the crad13 budgets. The regime-A "Adopt V4(1.0)" recommendation is VOID (superseded 2026-10-02 base, blind pairing; status doc line 66) with no in-doc marker (D3). P-CRAD-SPARSE itself is now CLOSED (current-release.md:59). | R7 substance closed; policy closed; V4(1.0) void |
| Generator-spread study (2026-10-07) | R1's "carry the cross-generator spread" clause | **Resolves.** Its Step-3 component (V3 = 0.5×GaussSmooth_pTrow(|rel|), symmetric, per-bin relative) is the implemented `c_rad_gen_rel`. Its sizings (2026-10-02 base, tables A/B/C) are historical (D4). | Component adopted; sizings superseded by crad13 budgets |
| xBinning sensitivity (2026-10-08) | New issue, post-dates the proposals (A-F1) | **Creates the current staleness.** Discovered the `XesGen` vs `x_es_gen` x-cell mismatch (median 3.50%, max 11.37%, not second-order) and triggered the 2026-10-09 releasebin rederivation that supersedes exactly the numbers the note's 2026-10-08 edit introduced (N2). Its header's "new budget component needed" was mooted by the rederivation (D10). | Resolved by rederivation; note refresh pending |
| EMPZ convention sensitivity (2026-10-09) | R4's EMPZ clause | **Resolves.** 45–70 window identical to lower-only >45; alternatives are truth-fiducial redefinitions (+2.7%/+19.4%/−3.0%/−2.0%); user decision 2026-10-09 retains `GenEmpz>45`. The doc's closing "remain open" is same-day stale (D8). | EMPZ blocker CLOSED; no applied-table change |
| Exposure provenance audit (2026-10-09) | U11-adjacent (exposure provenance); R4's P-CRAD-LUMI clause | **Narrows.** All six legs file-set-reconciled (exact oomclumi / tree-sum / per-chunk closures); 4.67% shortfall root-caused with zero bias; P-CRAD-LUMI relabeled FILE-SET-RECONCILED. Permanent external ceiling: generator-log closure impossible (MCDB CGI disabled). RG31 two-valued per-chunk σ̂ residual bounded (−0.44%…+0.26%). Step-5 text retains stale clauses (D9). | Provenance narrowed to generator-log-open; publication-grade status unchanged |
| z<0.05 definition sensitivity, releasebin addendum (2026-10-09) | R4-adjacent (z<0.05 sensitivity blocker) | **Narrows to caveat-only.** Releasebin maxima DJ14 9.925%/8.023%, RG31 6.461%/5.200%; median 0.000% in every z≥0.05 row; supersedes the xFcut-basis ±6.8%/±3.9% figures. Per user decision: caveat, not a budget component. | Blocker converted to a documented caveat; note sentence missing (N6) |

**Net effect on the prior proposal set:** R1, R3, R5, R6, R7, U10, U11 are implemented or resolved; R2 remains the one partially-implemented HIGH item (figures); R4 is mostly implemented with two clauses closed by the 2026-10-09 studies and one guard still absent. No prior proposal was dropped silently; every supersession has a recorded authority (STATE.md superseded list + status doc), though several study docs lack in-doc forward markers (D3–D10).

---

## 5. Counts and top-3 findings

### 5.1 Counts

- **New-doc consistency findings (Task 2): 12** (D1–D12; 4 within-doc staleness/missing-marker findings on live surfaces D1/D2/D11 carry the most risk). **Number disagreements with STATE.md among the 10 new docs: 0.**
- **Remaining issues in `radiative_corrections.tex` (Task 3): 10** (N1–N10: 2 HIGH — N1 figures, N2 superseded baseline numbers; 5 MEDIUM — N3, N4, N7, N9 and the N5/N6 guard/caveat pair; 3 LOW — N8, N10, and the span refresh inside N2). **Adjacent note sections: 2** (results.tex superseded-basis quotes; systematics.tex stale "missing component" claim).
- **Prior-proposal ledger:** 7 of 9 items implemented/resolved (R1*, R3, R5, R6, R7, U10, U11; *R1 implemented but one promotion stale), 1 partially implemented (R2 — figures pending), 1 mostly implemented (R4 — two stale clauses + one absent guard).

### 5.2 Top-3 findings

1. **The note's 2026-10-08 edit was correct against the 2026-10-07 xFcut promotion and was overtaken by the 2026-10-09 releasebin promotion.** The A-F1 x-binning fix (per-bin |ΔC_rad|/C_rad median `3.5049%`, max `11.3712%`) superseded exactly the numbers the edit introduced: the integrated factors, the cross-generator spread, the h− stat median, the table paths, and the "current baseline is the xFcut campaign" sentence (tex:27–45) — while R2's figure replacement, the last HIGH item, is still pending with fLumi-v3 figures whose content (C_rad ≈ 1.025) contradicts the section text (≈ 1.14) by ~11%. The section currently needs one coordinated refresh (§3.3a–b), not a re-audit.
2. **The declared status authority is internally inconsistent (D11).** `docs/status/radiative-corrections.md` carries a 2026-10-08 header and presents the xFcut baseline as APPLIED CURRENT in its top sections and blockers table, while its own tail (lines 90–126) records the 2026-10-09 releasebin promotion, EMPZ closure, z<0.05 caveat, and emfix repair. A reader stopping at the top quotes superseded numbers as current — the doc-level mirror of the R1 error class this audit program exists to catch. The same head-vs-tail pattern affects the promotion record (D1/D2) and four study docs whose same-day closures are unmarked (D6–D9).
3. **The documentation set is numerically trustworthy: zero number disagreements with STATE.md were found across all 10 new documents.** Every value checked traces to machine records (pairv1 CSVs/JSONs, exposure audits, campaign JSONs), and the note's photon-only Born subsection verifies number-by-number against the pairv1 records (C_γBorn 1.041267/1.040456, F_EWγ 0.9119/0.9116, per-beam +0.99%/+0.87%, NoZ closure 14,119,125.013920 nb⁻¹, tolerance 2.3×10⁻¹⁶). All 12 doc findings are staleness or missing-marker issues, not value contradictions — the failure mode to police going forward is supersession bookkeeping, not arithmetic.

---

## 6. Risks, assumptions, open questions, and coverage gaps

- **Two-layer reading risk.** Any consumer of this report must not quote Layer-1 (xFcut) numbers as current. Conversely, the note's 2026-10-08 text is not "wrong" — it is correctly dated to a superseded basis; the fix is a refresh, and §3.3 provides the text.
- **Derived value flagged.** The ≈ +0.642%/+0.650% integrated spread on the emfix releasebin pair is arithmetic derived here from STATE.md-quoted integrated values; it is not a recorded quote. Record it in the status authority before using it in the note.
- **Releasebin per-bin spans not yet recorded** in STATE.md or the status doc; the replacement text deliberately omits them pending a re-read from the applied table.
- **Releasebin CSV filename assumption.** §3.3a keeps the xFcut filename inside the releasebin directory per STATE.md:98 ("releasebin CSVs share filenames with xFcut CSVs"); the editors should confirm the exact filename with a directory listing before committing.
- **Figure regeneration is an open action on the authority side too.** The status doc's plot pointer (line 85) still names the xFcut 7-column package; a releasebin comparison package does not yet exist for the full-EW correction (only the cgammaborn releasebin refresh exists). The full R2 fix therefore requires a plotting action, not just a note edit.
- **Untouched in this audit:** the note's unfolding/backgrounds findings (U1–U12, B1–B7) from the prior report are unaffected by the c_rad chain except where the Born release changes quoted values; results.tex/systematics.tex were reviewed only for the radiative-related statements (D12, §3.2 adjacent). The qT/Q hybrid chain, DIFFVM block, and MC-comparison promotion (2026-10-10) were consulted only as context.
- **No source files were modified.** All replacement text in §3.3 is proposed for the note editors; the status-authority fixes (D11, and the forward pointers of D1–D10) are recommendations for the radiative-correction owner.

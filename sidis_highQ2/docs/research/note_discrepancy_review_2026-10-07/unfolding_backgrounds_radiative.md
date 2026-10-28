# Analysis-Note Discrepancy Review — Unfolding, Backgrounds, Radiative Corrections

**Date:** 2026-10-07
**Auditor role:** read-only audit of note sections against the current baseline; the note itself was not modified.
**Note audited (assigned sections, read in full):**

- `analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/unfolding.tex` (75 lines)
- `analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/backgrounds.tex` (49 lines)
- `analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/radiative_corrections.tex` (38 lines)

**Authoritative baseline consulted (read in full):**

- `docs/RUN_STATUS.md` (last updated 2026-10-06)
- `docs/status/unfolding-tunfold.md` (updated 2026-10-07 during this audit — see §1.2)
- `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`
- `docs/status/radiative-corrections.md`
- `docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md`
- `docs/status/alpha0p2-background.md`
- `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`

**Supporting evidence additionally verified (needed to check note claims):**

- `STATE.md` (2026-10-07), `docs/status/current-release.md` (2026-10-07), `docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md`, `docs/LESSONS_LEARNED.md` (CC2, TP-D1), `docs/results/rev6_v2_chain_audit_fixes_2026-05-31.md` (F7), `docs/critiques/rev6_chain_audit_2026-05-31/area2-matching-critic-physics.md`
- Skills loaded per workspace rule: `sidis-hq2-unfolding`, `sidis-hq2-backgrounds`, `sidis-hq2-radiative`
- Figure provenance verified by md5: the note's purity/acceptance figures are **byte-identical** to plots computed from the *diagnostic all-fake alpha0p2 response* (`H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/plots/response_metrics_vs_pTh_7col_2026-06-30/`; input `responses/allfake_response_matrices_and_categories.npz`).
- Response fill logic verified in `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py` (F7 gen-charge matched fill; fakes by reco charge; misses by gen charge).
- The note's background total 285747 was traced numerically to the superseded 356613 nb^-1 convention (see finding B1).
- The note's C_rad numbers were traced to the historical `full_crad_v1` route (`docs/radiative_corrections/rapgap_crad_normalization_deepdive_2026-06-24.md`; archived 2026-07-03 status snapshots).

---

## 1. Baseline snapshot used for this audit

### 1.1 Baseline facts (verified in the docs listed above)

- **Central method:** IBU/BBB finalcandidate, IBU `k=4`. The central IBU is the *in-house* implementation; RooUnfoldBayes 2.0.1 / ROOT 6.38.04 is the cross-check, reproducing in-house IBU to floating precision (max relative difference `2.6e-15` on the 2026-06-10 release; `3.31e-15`/`3.66e-15` on the coherent-prior candidate revalidation) — `docs/status/current-release.md:20,22`, `docs/RUN_STATUS.md:60`.
- **Response/data contract:** `NB_G_per_charge=245`, `NB_R_per_charge=490`, response `(491,246)` per charge, combined data vector length `980` — `docs/RUN_STATUS.md:59`.
- **TUnfold:** native v6 `TUnfoldDensity` miss-underflow is the standard *diagnostic* entry point; v4 ordinary fake/miss encoding and v5 visible-TUnfold plus external `x/eps` are invalid/historical; RooUnfoldSvd/TSVDUnfold quarantined; TUnfold promotion blocked (Poisson closure, fake/background covariance, tau gates, refold chi2, negative bins, tails/high-rho) — `docs/status/unfolding-tunfold.md`, `docs/RUN_STATUS.md:170–182`.
- **P-PRIOR:** the stored plain (`w_gen`-only) IBU prior is a critic-verified defect (±2.8% vs response truth marginal `body+miss`, 245/245 bins, both charges); coherent `body+miss` fix fork-validated (k=4 shift ~1% median, max ~3.6%, integrated `+0.1240%`, efficiency-dominated; acceptance/BBB unaffected) — `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`.
- **C_rad current baseline (2026-10-06):** xFGen<0 xFcut re-derivation, PROVISIONAL DIAGNOSTIC, not applied (`c_rad_applied=false`). RAPGAP31 nominal integrated h+ `1.1368045`, h− `1.1362373`; DJANGOH14 cross-check h+ `1.1418534`, h− `1.1413756`; `490/490` finite both; cross-generator `+0.444%`/`+0.452%`. Supersedes the quarantined blind (`xf_cut: False`) 2026-09-30 FINAL (blind under-corrects `+0.35/+0.38%` RAPGAP31, `+0.37/+0.41%` DJANGOH14 integrated; bias confined to z<0.05). Old hardcoded aggregate-lumi RAPGAP C_rad and fLumi-v3 route quarantined/superseded. Exposure provenance open (`DIAGNOSTIC_MANIFEST_LEVEL`, P-CRAD-LUMI). 2026-10-05 born-chain sidecars still carry blind C_rad (S3 re-attach pending). Reco-only alpha<0.2 must never be applied to gen-only C_rad — `docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md`, `docs/status/radiative-corrections.md`, `docs/RUN_STATUS.md:133–148`.
- **Alpha0p2:** all reco-background/all-fake response products diagnostic/provisional; central products not overwritten; no alpha cut promoted as central (P-ALPHA open); true pi/K/p loss `0.10649%` RAPGAP / `0.11354%` DJANGOH; matched e± removal `5.07632%`/`5.26673%`; uniform L351000 four-category total background fraction diagnostic `308655.8946/3865073.0 = 0.0798577141`; V0 pre-subtract allowed with unchanged response, non-V0 electrons fake-column handled, det.int not blindly pre-subtracted; exactly-once handling with all-fake response — `docs/status/alpha0p2-background.md`, `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`.

### 1.2 Baseline motion during this audit (important)

The P-PRIOR status moved while this audit was running:

- `docs/RUN_STATUS.md` (2026-10-06) and the task briefing say **ADOPTION PENDING**.
- `docs/status/current-release.md` and `docs/status/unfolding-tunfold.md` (both read 2026-10-07, during this audit) now record **P-PRIOR RESOLVED/ADOPTED 2026-10-07**: the coherent prior (`body+miss`) is the **NOMINAL IBU prior method** for v6/z005, executed as the dated candidate re-release `output_v6/central/data_release_candidates/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/` (PROVISIONAL until refreshed QA/critic verdict and P-PLOTS close; **not yet swapped into `central/data_release/`**). The 2026-06-10 plain-prior central release is "superseded as physics basis … retained as provenance", and "quotes must state prior basis".

Either way, the constraint given for this audit holds unchanged: **the note must not present the plain prior as correct, and must not present the coherent lineage as a completed central swap.** Findings below use the 2026-10-07 doc state and flag the motion explicitly.

---

## 2. Findings — `sections/unfolding.tex`

### U1 — IBU implementation attribution reversed (MEDIUM)

1. **Location:** `sections/unfolding.tex:12`
2. **Note claim:** "The nominal unfolding method is the D'Agostini-style Iterative Bayesian Unfolding (IBU) implemented in RooUnfold, using $k=4$ iterations".
3. **Current baseline:** The central IBU is the in-house implementation; RooUnfoldBayes is the *cross-check* that reproduces the in-house IBU (max relative difference `2.6e-15`; candidate revalidation `3.31e-15`/`3.66e-15`). Source: `docs/status/current-release.md:20,22`; `docs/RUN_STATUS.md:60`; `docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:93,119`.
4. **Severity:** medium (method attribution/status inverted; the physics content of the correction is unaffected).
5. **Recommendation (proposed text):** "The nominal unfolding method is the D'Agostini-style Iterative Bayesian Unfolding (IBU) implemented in the in-house analysis code, using $k=4$ iterations; an independent RooUnfoldBayes implementation (RooUnfold 2.0.1, ROOT 6.38.04) reproduces the in-house IBU to floating-point precision (maximum relative difference $2.6\times10^{-15}$), validating implementation equivalence but not the choice of $k$."

### U2 — Nominal IBU presented with no prior-basis statement while the plain-prior chain is a validated defect (HIGH)

1. **Location:** `sections/unfolding.tex:12` (and by omission the whole section, which presents IBU k=4 outputs as the validated nominal result).
2. **Note claim:** IBU k=4 is "the nominal unfolding method", with cross-generator closure and iteration stability presented as established (lines 16) and no mention of the IBU prior or its basis anywhere in the section.
3. **Current baseline:** The central release's stored plain (`w_gen`-only) IBU prior is a critic-verified defect: it is not the response truth marginal `body+miss` in 245/245 gen bins for both charges (±2.8% scatter; h+ sum mismatch +0.1036%, h− +0.0958%); it feeds both IBU initialization and efficiency normalization, so "the k=4 values are not presently validated as unbiased" (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:40`). The coherent fix (prior := response truth marginal `body+miss`) is fork-validated with k=4 impact h+ median `1.031%`/max `3.583%`, h− median `1.099%`/max `3.463%`, integrated `+0.1240%` combined, efficiency-dominated; acceptance/BBB unaffected (`docs/unfolding/coherent_prior_fix_results_2026-10-05.md`). As of 2026-10-07 the coherent prior is the ADOPTED nominal IBU prior basis, executed as the PROVISIONAL candidate re-release `output_v6/central/data_release_candidates/bbb_ibu_coherentprior_…_2026-10-06/`, not yet swapped into `central/data_release/`; the 2026-06-10 plain-prior release is superseded as physics basis and "quotes must state prior basis" (`docs/status/current-release.md:14–15,60`; `docs/status/unfolding-tunfold.md:9`).
4. **Severity:** high (the section implicitly presents the plain-prior chain as the correct, validated nominal method; the baseline says those values are not validated as unbiased and the prior basis is superseded).
5. **Recommendation (proposed text, to be inserted after the nominal-method sentence at line 12):** "The IBU consumes a generation-level prior in two roles (iteration initialization and efficiency normalization). The prior originally stored in the 2026-06-10 release was filled with generator weights only and was subsequently shown to be inconsistent with the response's own truth marginal (matched body plus miss) at the ±2.8% level in all 245 truth bins for both charges. A coherent prior, defined as the response truth marginal (body+miss), has been validated and adopted (2026-10-07) as the nominal IBU prior basis; at $k=4$ the change shifts cross sections by about 1% per bin (median; maximum 3.6%), +0.124% integrated, and is dominated by the efficiency normalization; the acceptance and bin-by-bin (BBB) paths are unaffected because they are computed from the response alone. All values quoted in this note are taken from the coherent-prior candidate re-release of 2026-10-06 (provisional until the refreshed QA and critic verdicts close) / [alternative: from the 2026-06-10 release on the superseded plain-prior basis, to be updated after the coherent re-release is finalized]." Whichever basis is quoted, the text must state it explicitly and must not describe the plain prior as correct or the coherent lineage as a completed central swap. If the note editors want a decisive check before wording, the required test is the completion of the refreshed QA/critic verdict on the coherent-prior candidate re-release chain (unfolding domain), after which the note should quote the coherent lineage.

### U3 — "TUnfold … is not an alternative central result" preempts the open P-METHOD decision (MEDIUM)

1. **Location:** `sections/unfolding.tex:12`
2. **Note claim:** "TUnfold is presented as a diagnostic comparison candidate and is not an alternative central result."
3. **Current baseline:** The nominal-method choice between IBU/BBB finalcandidate and native v6 TUnfold is an explicit open blocker (`P-METHOD`): "Nominal IBU/BBB vs native v6 TUnfold decision" (`STATE.md:64`); "Nominal-method choice between IBU/BBB finalcandidate and native v6 TUnfold" (`docs/RUN_STATUS.md:76`, blocker 3); `docs/status/current-release.md:57`. The recommended default is to keep IBU/BBB nominal (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:85`), but the decision is not yet made.
4. **Severity:** medium (status claim presented as decided).
5. **Recommendation (proposed text):** "TUnfold is retained as a diagnostic comparison candidate; the final nominal-method decision between IBU/BBB and native v6 TUnfold (P-METHOD) is still pending, with IBU/BBB the central candidate in the meantime."

### U4 — Charge-confusion handling description does not match the current v6 response; TUnfold variant unspecified (MEDIUM)

1. **Location:** `sections/unfolding.tex:14`
2. **Note claim:** "Charge-confused tracks are treated as misses in one matrix and fakes in the other." (The paragraph also never specifies which TUnfold implementation/encoding is used.)
3. **Current baseline:** In the canonical v6 per-charge response builder, matched entries are filled by **generator** charge (F7 fix: "Matched-fill uses `gen_chg_inl` not `reco_chg` — Eliminate charge-confusion contamination in per-charge ε"; `docs/results/rev6_v2_chain_audit_fixes_2026-05-31.md:20`), so a charge-confused matched track enters the *generated-charge* response body (its gen is consumed, so it produces no miss); fakes are assigned by reconstructed charge and misses by generator charge (`H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py`, matched/fake/miss fill block, comments "F7: gen charge for matched / reco charge for fakes"). The current standard TUnfold diagnostic is the native v6 `TUnfoldDensity` miss-underflow convention; v4 ordinary fake/miss-bin encoding and v5 visible-TUnfold plus external `x/eps` are invalid/historical entry points (`docs/status/unfolding-tunfold.md:21–29,59–62`; `docs/RUN_STATUS.md:178`).
4. **Severity:** medium (response-construction description wrong vs the current builder; unspecified method variant risks referring to superseded v4/v5 encodings).
5. **Recommendation (proposed text):** "For the diagnostic TUnfold comparison the native ROOT TUnfoldDensity (v6 miss-underflow) convention is used: the matched response occupies ordinary $490\times245$ bins per charge, missed truths enter the reco-axis underflow, only the 490 physical reco bins are passed to SetInput, and no post-hoc $1/\epsilon$ correction is applied. In the per-charge response, matched entries are filled by generated charge, so charge-confused matched tracks enter the generated-charge response; fakes are assigned by reconstructed charge and misses by generated charge, and the residual charge-confusion asymmetry is tracked as a separate open systematic." (Unfolding domain; no new analysis needed — the fill logic is already implemented and documented.)

### U5 — "only 0.19% of tracks are reconstructed with the wrong charge" (MEDIUM, UNVERIFIED)

1. **Location:** `sections/unfolding.tex:14`
2. **Note claim:** "only 0.19\% of tracks are reconstructed with the wrong charge, so this is a reasonable approximation".
3. **Current baseline:** No current-baseline document states a 0.19% charge-confusion rate. The workspace's recorded evidence is "Integrated charge confusion is 0.35% — per-bin can be 1–2% in some bins" (`docs/critiques/rev6_chain_audit_2026-05-31/area2-matching-critic-physics.md:39`) and the open lesson "CC2 | 0.35% charge confusion as unaccounted systematic in per-charge architecture | Open" (`docs/LESSONS_LEARNED.md:204`). The F7 fix removed charge-confusion contamination from per-charge efficiencies, with the consequence that it "now needs to be quoted explicitly as a per-bin systematic" (`docs/results/rev6_v2_chain_audit_fixes_2026-05-31.md:134`).
4. **Severity:** medium (unverified number that conflicts with the workspace's recorded 0.35%, and the associated systematic is open).
5. **Recommendation:** Do not decide the text from existing docs. Required analysis test (unfolding/backgrounds domain): measure the charge-confusion rate (reconstructed charge ≠ generated charge among matched tracks) on the full central v6 production (not the single-file rev6-audit estimate), update the 0.19% figure with the measured value and its uncertainty, and state that the residual per-charge charge-confusion systematic (CC2) is open. Until then, mark the 0.19% claim UNVERIFIED in the note.

### U6 — TUnfold regularization strength "was chosen" (MEDIUM)

1. **Location:** `sections/unfolding.tex:14`
2. **Note claim:** "The value of the regularization strength, $\tau$, in TUnfold was chosen to minimize correlations between bins as well as the model, statistical, and regularization uncertainties."
3. **Current baseline:** No physics-valid tau exists; tau selection is an unresolved TUnfold promotion blocker. "physics-valid tau choice | FAIL. The scan's diagnostic selections are interior but have refold chi2/ndf 607–2267 and 7–9 negative bins … explicitly diagnostic-only—not a validated L-curve/global-rho/closure selection" (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:66`); "Hard tau-selection gates" and "TUnfold promotion is blocked" (`docs/status/unfolding-tunfold.md:44–53`); open lesson "TP-D1 | TUnfold τ chosen by ad-hoc gate, not L-curve scan | Open" (`docs/LESSONS_LEARNED.md:201`). `tau=1e-6` is only "the documented common native-v6/per-cell comparison setting" (sidis-hq2-unfolding skill).
4. **Severity:** medium (an unresolved, failed-gate selection is presented as a resolved principled choice).
5. **Recommendation (proposed text):** "No physics-valid regularization strength has been established for the TUnfold diagnostic: candidate points from the tau scan remain diagnostic-only (interior selections with high refold $\chi^2$ and residual negative bins), and tau selection is an explicit promotion blocker. The comparisons shown use documented diagnostic settings (e.g. $\tau=0$ and the common comparison setting $\tau=10^{-6}$) rather than a validated choice."

### U7 — Cross-generator closure presented as validation; "validated cross-closure input table" (MEDIUM)

1. **Location:** `sections/unfolding.tex:16` ("The IBU unfolding does a good job of reproducing the truth of Rapgap when the reconstruction-level Rapgap data is unfolded with the Djangoh migration matrix, and vice versa"; "IBU and TUnfold generally agree quite well") and `sections/unfolding.tex:20` ("These figures are note-ready diagnostics derived from the validated cross-closure input table").
2. **Note claim:** Qualitative closure success and method agreement presented as established results from a "validated" input.
3. **Current baseline:** The closure plan explicitly assesses these note claims: "The analysis note says IBU is stable in most bins below 10 iterations and claims cross-generator closure qualitatively, but it supplies neither a numerical iteration-to-iteration convergence table nor Poisson coverage/pull evidence" (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:93`); "IBU cross-generator deterministic closure | PARTIAL / NOT SUFFICIENT | The analysis note has RAPGAP↔DJANGOH cross-closure figures. They are deterministic pseudo-data diagnostics and do not establish toy coverage, independent response/pseudodata closure, or corrected-prior validity" (same doc, line 121); "the Section 1 mismatch invalidates a claim of full central self-closure" (line 130); Poisson-fluctuated closure is MISSING (line 64). For TUnfold, the current quantitative comparison is the native v6 tau0/IBU median ratios `0.99175/0.99273/0.99129/0.99062` (`docs/status/unfolding-tunfold.md:37`). The note figures' exact source was not re-located in this audit (their md5 does not match the 2026-06-14 `output_v6/systematics/model_crossclosure/ibu_crossclosure_z005_2026-06-14/` plot PDFs), so "validated cross-closure input table" is also UNVERIFIED as stated.
4. **Severity:** medium (validation claim that remains blocked; "validated" overstates a deterministic diagnostic).
5. **Recommendation (proposed text):** "The cross-generator comparison (DJANGOH reconstruction unfolded with the RAPGAP response and vice versa) is a deterministic pseudo-data diagnostic. Poisson-fluctuated closure with bias/variance/coverage, statistically independent response/pseudo-data closure, and covariance-aware refold $\chi^2$ are still pending (P-METHOD-related gates) and are required before any closure claim; the deterministic ratios shown here are arithmetic smoke tests only." Required analysis test before stronger wording is possible (unfolding domain): the closure-plan Section 4 suite — split-MC Poisson closure (≥100 toys per direction, both generator directions), pull mean/width and 68% coverage, data refold chi2 on the coherent-prior basis (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:128–147`).

### U8 — "IBU also provides stable results for most bins at any value of N_iter < 10" (MEDIUM)

1. **Location:** `sections/unfolding.tex:16`
2. **Note claim:** Broad iteration stability below 10 iterations, stated without quantitative support.
3. **Current baseline:** The closure plan flags exactly this sentence: "the analysis note currently asserts broad stability for $N_{iter}<10$ without documenting a quantitative, reproducible stopping rule" and "the pre-repair k=4 rationale cannot be treated as final" (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:91`); the release saves iterations 1–10 and its regularization component is the k=3/k=5 variation, which "is not a stopping criterion" (line 93); after the prior repair, "scan k when response/prior handling changes; do not assume k=4 transfers blindly" (`docs/unfolding/coherent_prior_fix_results_2026-10-05.md:177`).
4. **Severity:** medium (unsupported validation claim; the supporting scan is a required, not-yet-run release gate).
5. **Recommendation (proposed text):** "The release producer saves IBU iterations 1–10 and selects $k=4$; the regularization component of the uncertainty is the $k=3/k=5$ variation. A quantitative $k=1$–$10$ bias–variance and coverage scan on the repaired (coherent-prior) response is required before the $k=4$ choice can be defended; until then $k=4$ is retained as the current candidate, not a defended final choice." Required analysis test (unfolding domain): the k=1–10 split-MC Poisson scan of closure-plan Section 3 (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:97–109`).

### U9 — Purity/acceptance figures are products of the diagnostic all-fake alpha0p2 response, presented as the analysis's central purity/acceptance (HIGH)

1. **Location:** `sections/unfolding.tex:8–10` (definitions and claims), `43–59` (purity figure), `61–73` (acceptance figures).
2. **Note claim:** "The binning of the analysis … was selected to have an acceptable purity and acceptance in almost all bins. … The purities for $h^+$ and $h^-$ are shown in Figs. …" — presented as the analysis's (central) purity and acceptance, with no diagnostic caveat on these figures (in contrast to line 18, which correctly labels the all-fake response "diagnostic and provisional only").
3. **Current baseline:** md5-verified: `figures/fig_alpha0p2_response_purity_hp_7col.pdf` is byte-identical to `H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/plots/response_metrics_vs_pTh_7col_2026-06-30/purity_vs_pTh_onepage_7col_hp.pdf`, and `figures/fig_alpha0p2_response_acceptance_hp_7col.pdf` is byte-identical to the matching `acceptance_vs_pTh_onepage_7col_hp.pdf`; the plot manifest records the input as `responses/allfake_response_matrices_and_categories.npz`, i.e. the **diagnostic all-fake alpha0p2 response**. All alpha0p2/all-fake response products are diagnostic/provisional; "Central z005 release and central response products were not overwritten" (`docs/status/alpha0p2-background.md:8`; `docs/RUN_STATUS.md:154`). The note itself states (line 18) that this response is diagnostic and that central products were not overwritten — so the section is also internally inconsistent.
4. **Severity:** high (purity/acceptance values from a non-central, diagnostic response are presented as the analysis's binning-validation metrics; the note's own caveat on the same response is not carried to these figures).
5. **Recommendation:** Either (a) regenerate the purity/acceptance 7-column figures from the central (coherent-prior candidate) response and quote those values, or (b) if the all-fake-response metrics are intentionally shown, add the same diagnostic/provisional caveat to text and captions as is used for the migration-matrix figure. Required analysis test before the values can be finalized (unfolding domain): recompute purity/acceptance from the central migration inputs (adapt the existing `response_metrics_vs_pTh` plotter to the central response) and quantify the difference vs the all-fake values shown now.

### U10 — Worst-acceptance mechanism "tracks predominantly fall at η>1.6" (LOW, UNVERIFIED)

1. **Location:** `sections/unfolding.tex:10`
2. **Note claim:** "The worst acceptances are found in the low-$z$, high $\pTh$ region, where the tracks predominantly fall at $\eta>1.6$ in the lab frame."
3. **Current baseline:** The low-z/high-pT location is consistent with the all-fake-response acceptance table (lowest averaged acceptances at z bin 0, pT 4–8 GeV; verified from `acceptance_vs_pTh_table_7col.csv`), but no baseline doc verifies the η>1.6 dominance mechanism (it is plausible given the `eta1p6` selection tag). Low-acceptance diagnostics exist at `H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/diagnostics/low_acceptance_truth_columns_2026-07-02/`.
4. **Severity:** low.
5. **Recommendation:** Verify with the low-acceptance truth-column diagnostic (unfolding domain) before keeping the η>1.6 statement; otherwise soften to "where the tracks predominantly fail the lab-frame track acceptance".

### U11 — Purity figure caption/reference defects (LOW)

1. **Location:** `sections/unfolding.tex:10` (text reference), `43–59` (figure environment).
2. **Note claim:** The text cites "Figs.~\ref{fig:unfolding:alpha0p2-purity} and~\ref{fig:unfolding:alpha0p2-purity-hm}" as two figures, but the second label is a subfigure inside the first figure environment; the parent caption reads "Purity versus $\pTh$ for $\hplus$ in all bins." (duplicating subcaption (a) and omitting h−), and subcaption (b) is only "$\hminus$".
3. **Current baseline:** n/a (LaTeX/caption correctness).
4. **Severity:** low (cosmetic).
5. **Recommendation:** Parent caption: "Purity versus $\pTh$ for $\hplus$ (top) and $\hminus$ (bottom) in all bins."; subcaption (b): "Purity versus $\pTh$ for $\hminus$ in all bins."; cite only the parent figure in the text.

### U12 — "worst purities … around 5%" (LOW)

1. **Location:** `sections/unfolding.tex:10`
2. **Note claim:** "The worst purities are found in the higher $x$ strata at high $\pTh$, where they are around 5\%."
3. **Current baseline:** In the (all-fake) plotted data, the worst averaged purities are 4.4–6.1% and additionally sit at the highest z bin [0.7,1.0] (x subbin 1, pT 4–8 GeV); the per-generator minimum is 3.6% (verified from `purity_vs_pTh_table_7col.csv` and its manifest). Approximately consistent, but the location description omits the highest-z dependence and "around 5%" understates the per-generator minimum.
4. **Severity:** low.
5. **Recommendation:** After U9's regeneration from the central response, restate as: "The worst purities (down to about 4%) are found at the highest $z$, in the higher-$x$ strata at high $\pTh$."

### Verified-consistent claims in `unfolding.tex` (no discrepancy)

- Response contract at line 18: per-charge shape `(491,246)`, rows 0–489 physical reco, row 490 miss, columns 0–244 truth, column 245 fake — matches `docs/RUN_STATUS.md:59`.
- The all-fake migration-matrix figure and its "diagnostic and provisional only; central response and release products were not overwritten" caveat (lines 18, 22–27) — matches `docs/status/alpha0p2-background.md:8`.
- "twice the number of reconstruction-level bins in $\pTh$" (line 14) — consistent with `NB_R_per_charge=490` vs `NB_G_per_charge=245` (2× finer pT).
- "bin-by-bin correction is retained as a cross-check" (line 12) — consistent with BBB as diagonal cross-check (`docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:3`).
- Model-uncertainty definition (line 14, RAPGAP-vs-DJANGOH response difference) — consistent with `docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md:137`.
- Negative-bin challenge for TUnfold (line 14) — consistent with native v6 negative-bin counts 14/15/11/9 (`docs/status/unfolding-tunfold.md:38`).
- The note makes no SVD claim (nothing presenting RooUnfoldSvd/TSVDUnfold as current/valid).

---

## 3. Findings — `sections/backgrounds.tex`

### B1 — Integrated background total 285747 is the superseded-luminosity, pre-det.int number (HIGH)

1. **Location:** `sections/backgrounds.tex:37`
2. **Note claim:** "The integrated reco track count is $3865073$, of which the estimated background total is $285747$, shared roughly equally between $e^\pm$ and V0 daughters located mostly at low-$z$."
3. **Current baseline:** 285,747 is exactly the sum of the three plotted components at the **superseded 356613 nb^-1** convention (electron avg `151,358.78` + V0 avg `134,177.19` + lowQ2 `211.71` = `285,747.69`, per the study summary `H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/plots/summary_background_fractions_vs_data_7col.json`; equivalently the documented pre-det.int fraction step `7.3930735%` of 3,865,073, `STATE.md:26`). The current baseline at `L_data=351000.0 nb^-1` is: electron avg `148,976.4352` + V0 avg `132,065.2768` + lowQ2 `208.3795` = `281,250.09` (three-category), and the four-category total including det.int avg `27,405.8031` is **`308,655.8946`**, i.e. integrated fraction **`0.0798577141` (7.9857714%)** — `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:35–45`, `docs/status/alpha0p2-background.md:38`, `STATE.md:26,33`. The 356613 convention is explicitly superseded ("Do not use older `356613.0 nb^-1` … as current defaults", `docs/RUN_STATUS.md:121`; "The older `356613` convention is superseded for current high-Q² charged-SIDIS background-yield comparisons", handoff §3).
4. **Severity:** high (wrong background total/fraction presented as current: 285,747/3,865,073 = 7.39% vs the current 308,655.9/3,865,073 = 7.986%; the stale number also mixes the superseded MC-luminosity convention with the correct data denominator).
5. **Recommendation (proposed text):** "The integrated reco track count is $3{,}865{,}073$; the estimated four-category background total at $L=351{,}000~\mathrm{nb}^{-1}$ is $308{,}656$ (residual $e^\pm$ $148{,}976$, V0 daughters $132{,}065$, GEANT/STR secondary hadrons $27{,}406$, low-$Q^2$ $208$), i.e. an integrated background fraction of $7.99\%$." The fraction figure should be (re)generated from the uniform L351000 four-category package (`…/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/`); the note figure's md5 does not match the study's current onepage rendering, so its content must be checked/regenerated as part of the edit (backgrounds + normalization domains).

### B2 — det.int (GEANT/STR secondary hadrons) category omitted; "two non-negligible categories" is wrong (HIGH)

1. **Location:** `sections/backgrounds.tex:10–17` (enumeration) and `31–39` (plotted components/total).
2. **Note claim:** "two non-negligible reconstructed-track background categories are considered in the current high-$\Qsq$ analysis: (1) electron and positron tracks …; (2) charged tracks arising from decays of long-lived neutral hadrons." The plotted components listed are e±, V0 daughters, low-Q2 only.
3. **Current baseline:** The validated accounting has **four** track-level categories; the missing one, `det.int.`, is STR-only GEANT secondary charged π/K/p (`IsStr==1 && IsGtr==0 && IsMatchedHadron==1 && IsMatchedElectron==0 && IsMatchedV0Daughter==0`), with scaled average `27,405.8031` at L351000 — 0.71% of reco tracks, ~130× the low-Q2 track-level component the note does mention — and it must be described as GEANT/STR charged-hadron secondaries, "not pure material interactions" (`docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:22–24,36–45`; `docs/status/alpha0p2-background.md:38,52`).
4. **Severity:** high (a non-negligible background category is omitted and the category enumeration is wrong relative to the validated accounting).
5. **Recommendation (proposed text):** add a third category to the enumeration: "(3) charged tracks arising from GEANT/STR secondary charged $\pi/K/p$ produced in the detector-simulation stage (denoted detector-interaction, det.int.; these are simulation secondaries, not pure material interactions)"; add det.int to the plotted-component list and the total; fold its yield into B1's corrected total.

### B3 — α≥0.2 veto and alpha0p2 background products presented as the central analysis treatment without P-ALPHA provisional status (HIGH)

1. **Location:** `sections/backgrounds.tex:20–21` ("accepted tracks satisfy $\alpha(\mathrm{track},e')\ge0.2~\mathrm{rad}$" presented as the applied selection), `31–37` ("The current canonical reco-background fraction display … after the $\alpha<0.2$ selection"), `41–49` (figure caption "after the $\alpha<0.2$ selection").
2. **Note claim:** The α<0.2 cone veto is the analysis's track selection and the alpha0p2 background fractions are the analysis's current background treatment.
3. **Current baseline:** All alpha0p2 reco-background and all-fake response products are diagnostic/provisional; "No alpha cut, raw MC subtraction, fake subtraction, or all-fake response treatment is promoted as central until response-consistent closure, fake/background covariance, and nominal-method decisions are complete" (`docs/status/alpha0p2-background.md:8`); P-ALPHA is an open blocker (`docs/status/current-release.md:59`; `docs/RUN_STATUS.md:78`). The central v6 builder contains no alpha cut (verified: no `alpha` match in `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`); the central release "is explicitly raw/no-bgsub, consumes NO background template, applies only response fake-fraction subtraction ($d\_sig=data*(1-f_i)$ …); Electrons and V0 are vetoed in selection, not subtracted" (`STATE.md:34`).
4. **Severity:** high (a diagnostic, not-yet-promoted selection and background treatment are presented as the central analysis).
5. **Recommendation (proposed text):** Add a status paragraph: "The near-electron opening-angle veto ($\alpha\ge0.2$ rad) and the associated reconstructed-background fraction displays are diagnostic/provisional (P-ALPHA): the current central release applies no $\alpha$-cone cut and no external background-template subtraction; electrons and V0 daughters are removed by the selection/classification, and the only background treatment in the central unfolding is the response-derived fake-fraction subtraction. Promotion of the $\alpha$-veto selection and of any background subtraction requires response-consistent closure and a fake/background covariance treatment." The section must also state that the α cut is reco-only and must never be imposed on gen-only C_rad (`docs/status/alpha0p2-background.md:10`).

### B4 — DIFFVM "studied … and found to be negligible" (HIGH)

1. **Location:** `sections/backgrounds.tex:8`
2. **Note claim:** "the contribution from exclusive vector meson production was studied with DIFFVM and found to be negligible in the high $Q^2$ phase space, as could be expected from the steep $Q^2$ scaling of exclusive cross sections."
3. **Current baseline:** DIFFVM is an **assumed zero and remains unassessed** in high-Q2: "DIFFVM (rho 8445-8448 all-decay only, phi, omega; ~55 GB v5g low-Q2 trees) is an ASSUMED ZERO and remains unassessed" (`STATE.md:37`); "OPEN (MC-semantics review …): DIFFVM assumed zero (scoping's 0.5% at z>=0.7 in 150-300 GeV^2 / 0.05% elsewhere never applied)" (`STATE.md:27`); "Its high-Q2 disposition requires a dedicated validated assessment or documented systematic, not an assumed zero" (sidis-hq2-backgrounds skill); "assess DIFFVM rather than assume zero" (`docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md:166`). No DIFFVM high-Q2 study exists in the current baseline.
4. **Severity:** high (claims a validation study and a negligible finding that do not exist in the current baseline; the assumed-zero disposition is an open review item).
5. **Recommendation (proposed text):** "Exclusive vector-meson production (DIFFVM; $\rho$ all-decay samples 8445–8448, $\phi$, $\omega$) is currently treated as an assumed-zero background: the scoping estimate (about 0.5% at $z\ge0.7$ in $150$–$300~\mathrm{GeV}^2$ and about 0.05% elsewhere) has been neither applied nor validated, and a dedicated validated assessment or a documented systematic is still required." Required analysis test before any "negligible" claim can be made (backgrounds + MC domains): run the high-Q2 selection over the DIFFVM samples (all-decay rho set only, unweighted, with sample-level IMC-luminosity provenance) and bound the contamination per bin, or assign a documented systematic.

### B5 — Pythia photoproduction "5 events" claim (MEDIUM, UNVERIFIED)

1. **Location:** `sections/backgrounds.tex:8`
2. **Note claim:** "The background from $Q^2 < 4$ GeV$^2$ (including photoproduction) was evaluated with Pythia and found to be negligible, only 5 photoproduction events are expected in the whole event sample."
3. **Current baseline:** The current baseline closes photoproduction (together with Compton, Grape, DVCS, CC) by event-level cuts, not by a Pythia evaluation: "Compton (`compton20/8346`), Grape (`h1grape/7128`), DVCS, CC, photoproduction are all removed by event-level CUTS (stage1 CUT 13 BG finders, CUT 14 anti-QED-Compton) and are properly closed — no template, no systematic" (`STATE.md:37`). No Pythia photoproduction study exists in the current baseline docs; the claim appears inherited from the earlier (2020) analysis.
4. **Severity:** medium (unverifiable claim whose stated mechanism does not match the current baseline's closure mechanism).
5. **Recommendation (proposed text):** "Photoproduction and the other non-DIS event classes (Compton, Grape, DVCS, charged-current) are removed by the event-level background-finder and anti-Compton cuts (stage1 CUT 13/CUT 14); they are closed without template or systematic." If the Pythia/5-event statement is to be retained, the required test (backgrounds + MC domains) is to locate or produce a Pythia photoproduction sample processed through the current high-Q2 selection and re-derive the expected-event bound; otherwise remove the sentence.

### B6 — Missing response-consistency / exactly-once treatment rules and central bgsub status (MEDIUM)

1. **Location:** `sections/backgrounds.tex` (whole section; nothing states how the categories are treated relative to the response).
2. **Note claim:** (omission) The section models the categories and shows fractions but never states the response-consistent treatment.
3. **Current baseline:** The validated rules are: with the unchanged alpha0p2 response, V0 daughters may be pre-subtracted/vetoed (they are excluded from the response), non-V0 electrons are fake-column handled and must not also be pre-subtracted when fake subtraction is active, and det.int (≈86% matched physical block / 14% fake) must not be blindly pre-subtracted; with the all-fake response, electron/V0/det.int are all fake-column components and must be handled exactly once (`docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:123–137`; `docs/status/alpha0p2-background.md:46–52`). The central release is raw/no-bgsub with response fake-fraction subtraction only (`STATE.md:34`).
4. **Severity:** medium (required treatment rules absent; a reader cannot tell whether subtraction is response-consistent or double-counted).
5. **Recommendation (proposed text):** Add a paragraph: "Background handling must be response-consistent and exactly-once. With the current response, V0 daughters are excluded from the response and may be vetoed/pre-subtracted; non-V0 electrons are represented in the response fake column and must not also be pre-subtracted; GEANT/STR secondary hadrons are ≈86% matched response entries and are not blindly pre-subtracted. Under the all-fake response variant, electrons, V0 daughters, and secondaries are all fake-column components and are handled exactly once by the fake treatment."

### B7 — 60/150 GeV² boundary claims (LOW, UNVERIFIED)

1. **Location:** `sections/backgrounds.tex:8`
2. **Note claim:** "The phase space of $60 < Q^2< 150$ GeV$^2$ is handled by the high $Q^2$ DJANGOH and RAPGAP MC samples. The region $Q^2 < 60$ GeV$^2$ is handled by just Djangoh, and the contribution is sub percent in almost all bins."
3. **Current baseline:** The low-Q2 component is a separate pre-scaled DJANGOH low-Q2 leakage source (`djangoh14/7091`), consistent in kind; the integrated track-level low-Q2 total is `208.3795` (0.0054% of the denominator), consistent with "sub percent" integrated (`docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:25,38`). The specific 60/150 GeV² boundaries and the per-bin "almost all bins" statement are not verifiable from the baseline docs.
4. **Severity:** low.
5. **Recommendation:** Verify the Q² boundaries against the low-Q2 sample definition and the per-bin low-Q2 fractions from the L351000 package (backgrounds + MC domains) before publication; keep the current wording only if confirmed.

### Verified-consistent claims in `backgrounds.tex` (no discrepancy)

- V0 ancestor set $|\mathrm{PDG}_{\mathrm{ancestor}}| \in \{310, 130, 311, 3122\}$ (lines 23–29) — matches `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:21` exactly.
- Integrated reco track count denominator `3865073` (line 37) — matches the L351000 package denominator (`docs/status/alpha0p2-background.md:38`).
- "curves for the $e^\pm$ and V0 backgrounds are taken as the average of DJANGOH and RAPGAP" (line 39) — matches the `electron avg`/`V0 avg` convention.
- "shared roughly equally between $e^\pm$ and V0 daughters" — consistent (148,976 vs 132,066).
- Event-level background framing ("only event-level background … DIS events outside the measurement phase space") — consistent with the event-level inventory (only `djangoh14_lowQ2bkgd` exists; `STATE.md:37`).
- Figure z convention "first column corresponds to $0.0 \le z < 0.05$" (lines 46–47) — matches the canonical display convention (`docs/status/alpha0p2-background.md:56`).

---

## 4. Findings — `sections/radiative_corrections.tex`

### R1 — Historical DJANGOH14 v1 C_rad numbers presented as the current result (HIGH)

1. **Location:** `sections/radiative_corrections.tex:15–24`
2. **Note claim:** "For DJANGOH14, the MC luminosities used in the $C_{\mathrm{rad}}$ computation are $4\,108\,957~\nbinv$ for the radiative sample and $3\,560\,233~\nbinv$ for the non-radiative sample. These values are adequate for the current internal baseline but still need publication-grade MCDB or generator-log provenance closure before final release. The DJANGOH14 v1 integrated results are $C_{\mathrm{rad}}=1.055329789$ for $\hplus$ and $1.055413655$ for $\hminus$. Finite rows are $239/245$ per charge ($478/490$ total), with all-charge median $1.078836633$."
3. **Current baseline:** These are the historical `full_crad_v1` route values (hard-coded luminosities in `scripts/histogramming/full_crad_finalize.py`; recorded in `docs/radiative_corrections/rapgap_crad_normalization_deepdive_2026-06-24.md:62,93,126,155` and the archived 2026-07-03 status). The status authority says: "The DJANGOH14 gen-only v1 snapshot (h+ `1.055329789`, h− `1.055413655`, `478/490` finite, `full_crad_v1` tables whose absolute normalization is blocked) is historical; do not quote it as current" (`docs/status/radiative-corrections.md:42`). The current baseline (2026-10-06) is the xFGen<0 xFcut re-derivation, PROVISIONAL DIAGNOSTIC, not applied (`c_rad_applied=false`): RAPGAP31 (nominal) integrated h+ `1.1368045`, h− `1.1362373`; DJANGOH14 (cross-check) h+ `1.1418534`, h− `1.1413756`; `490/490` finite per generator per charge; cross-generator `+0.444%`/`+0.452%` (`docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md`; `docs/status/radiative-corrections.md:16–30`). The v1 values differ from the current baseline by ~7–8% in the correction itself.
4. **Severity:** high (superseded, normalization-blocked numbers presented as the current radiative correction; wrong physics numbers as current).
5. **Recommendation (proposed replacement for lines 15–24):** "The current $C_{\mathrm{rad}}$ baseline (2026-10-06) is the per-bin, gen-level ratio $C_{\mathrm{rad}} = \sigma_{\mathrm{nonrad}}/\sigma_{\mathrm{rad}}$ with $\sigma \propto \sum(\text{weight1}\cdot\text{weight2})/L_{\mathrm{MC}}$, computed with the current-hemisphere cut $xF_{\mathrm{gen}}<0$ (recomputed $xF$, q-nominal) applied on both the RAD and NONRAD legs, and $E_{\mathrm{mpz}}>45$ GeV. RAPGAP31 (nominal) gives integrated $C_{\mathrm{rad}} = 1.1368045$ ($h^+$) and $1.1362373$ ($h^-$); the DJANGOH14 cross-check gives $1.1418534$ and $1.1413756$; all 490 bins per charge are finite in both generators, and the cross-generator agreement is $+0.444\%$ ($h^+$) and $+0.452\%$ ($h^-$). This baseline is a provisional diagnostic until reviewed and is not yet applied to the central release ($c_{\mathrm{rad,applied}} = $ false; the central release is raw, not Born-level). It supersedes the blind (no $xF$ cut) tables of 2026-09-30, which under-correct by $+0.35/+0.38\%$ (RAPGAP31) and $+0.37/+0.41\%$ (DJANGOH14) integrated, with the bias confined to $z<0.05$. The RAD/NONRAD exposures are `oomclumi`-derived and remain at diagnostic-manifest provenance level (P-CRAD-LUMI) pending MCDB/generator-log reconciliation, and the 2026-10-05 born-chain sidecars still carry the blind $C_{\mathrm{rad}}$ pending an S3 re-attach."

### R2 — Both comparison figures use the superseded fLumi-v3 route (HIGH)

1. **Location:** `sections/radiative_corrections.tex:26–38` (both `\includegraphics` use `figures/crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hminus_zoom.pdf`)
2. **Note claim:** The section's DJANGOH14-vs-RAPGAP comparison figures are the fLumi-v3 7-column plots.
3. **Current baseline:** "The per-chain `fLumi-v3` diagnostic (integrated h+ `1.0248113`, h− `1.0249101`; `PROVISIONAL_ONLY`) is likewise superseded by the exposure-based FINAL/xFcut chains and must not be used as nominal or envelope" (`docs/status/radiative-corrections.md:38`); the fLumi-v3 route is on the deny list with the old aggregate-lumi RAPGAP C_rad (`docs/RUN_STATUS.md:44–45`; task briefing). The current 7-column comparison package built from the xFcut tables is `plots/radiative/crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/` (`docs/status/radiative-corrections.md:51`; verified to exist locally). Note the fLumi-v3 figures show integrated corrections near 1.025 while the current baseline is near 1.137/1.142 — the plotted correction magnitude is wrong by ~11%.
4. **Severity:** high (superseded provisional-only values presented as the section's current comparison; the task explicitly flags the fLumi-v3 route as quarantined/superseded).
5. **Recommendation:** Replace both figures with the h+ and h− pages of the current xFcut package `plots/radiative/crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/` (radiative/plotting domains), and update the captions to name the xFcut baseline inputs.

### R3 — The h+ figure includes the h− PDF (MEDIUM)

1. **Location:** `sections/radiative_corrections.tex:28` (figure labeled `\label{fig:crad:djangoh-rapgap-flumi-v3-hplus}` includes `crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hminus_zoom.pdf`, identical to line 35's h− figure)
2. **Note claim:** The first figure is captioned "for $\hminus$" but labeled as the hplus comparison, and both figures include the same hminus file; the hplus PDF (`crad_djangoh14_vs_rapgap31_fLumi_v3_7col_hplus_zoom.pdf`) exists in `figures/` but is unused (verified by directory listing).
3. **Current baseline:** n/a (LaTeX correctness), but combined with R2 the section currently shows no h+ comparison at all.
4. **Severity:** medium (wrong figure content presented under the h+ label; the rendered note shows the same h− figure twice).
5. **Recommendation:** Point the first figure at the h+ file (or, per R2, at the xFcut h+ page) and correct the caption to "$\hplus$".

### R4 — Missing current-baseline status caveats (MEDIUM)

1. **Location:** `sections/radiative_corrections.tex` (whole section)
2. **Note claim:** (omission) No PROVISIONAL DIAGNOSTIC status, no `c_rad_applied=false`/not-Born-level statement, no blind-FINAL quarantine/supersession note, no S3 re-attach pending item, no exposure-provenance caveat tied to the current tables, no reco-only-α guard.
3. **Current baseline:** All of these are current-baseline facts: PROVISIONAL DIAGNOSTIC until reviewed, not applied (`c_rad_applied=false`; central release raw, not Born-level — `docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md:3`, `STATE.md:14`); blind 2026-09-30 FINAL quarantined under `data/radiative/quarantine/` with under-correction numbers (same doc:35–43); pending S3 re-attach of the 2026-10-05 born-chain sidecars (`docs/status/radiative-corrections.md:32`); exposures `DIAGNOSTIC_MANIFEST_LEVEL`, P-CRAD-LUMI open (`docs/status/current-release.md:55`; `docs/status/radiative-corrections.md:29`); "The `alpha < 0.2` scattered-electron cone cut is reco-only and must not be applied to gen-only `C_rad`" (`docs/status/radiative-corrections.md:44–46`).
4. **Severity:** medium (missing provisional-status caveats).
5. **Recommendation:** Fold the status sentences of the R1 replacement text into the section, and add: "The $\alpha<0.2$ cone veto is a reconstruction-level cut and is never imposed on the gen-only $C_{\mathrm{rad}}$ selection."

### R5 — "The data are corrected to the $e^-p$ cross section" (MEDIUM, UNVERIFIED)

1. **Location:** `sections/radiative_corrections.tex:13`
2. **Note claim:** "The data are corrected to the $e^-p$ cross section."
3. **Current baseline:** The current xFcut C_rad tables are explicitly **beam-mixed**: "sigma_nonrad/sigma_rad (ratio of cross sections), y-projected 490 bins, beam-mixed, exposure-normalized" with ep and em input trees and per-beam-charge closure controls recorded (`data/radiative/rapgap_crad_final_xfcut_2026-10-05/crad_rapgap31_xfcut.json`, fields `c_rad_definition`, `input_files/trees`, `validation/closure_controls`). No e−p-specific correction exists in the current baseline docs. (The four-period S67 dataset contains both e+ and e− periods, per `STATE.md:21`.)
4. **Severity:** medium (claim unverifiable and apparently inconsistent with the beam-mixed current definition).
5. **Recommendation:** Remove or rewrite the sentence once the intended beam-charge convention is confirmed. Required clarification/test (radiative domain): document whether any beam-charge treatment is intended for the Born-level correction; if none, delete the sentence; if a specific convention is wanted, it must be derived and validated against the beam-mixed tables (the per-charge closure controls in `crad_rapgap31_xfcut.json` are the starting point).

### R6 — DJANGOH-only framing inverts the current nominal/cross-check assignment (MEDIUM)

1. **Location:** `sections/radiative_corrections.tex:15–24` (quantitative content is DJANGOH14-only; RAPGAP appears only via the superseded figures)
2. **Note claim:** The only quantitative C_rad results quoted are DJANGOH14.
3. **Current baseline:** RAPGAP31 is the **nominal** C_rad generator (matching the central RAPGAP response); DJANGOH14 is the cross-check (`docs/radiative/CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md:21–22`; `docs/status/radiative-corrections.md:20–23`). A physics review of the coherent RAPGAP-response vs DJANGOH-C_rad choice is an open blocker (`docs/RUN_STATUS.md:75`, blocker 2; `STATE.md:63`).
4. **Severity:** medium (method-status framing; the nominal source is absent from the text).
5. **Recommendation (proposed text):** "RAPGAP31 is the nominal $C_{\mathrm{rad}}$ source, matching the central RAPGAP response; DJANGOH14 provides the cross-check (integrated agreement $+0.444\%$/$+0.452\%$ for $h^+$/$h^-$). The coherent choice of response generator and $C_{\mathrm{rad}}$ generator remains under physics review."

### R7 — Red TODO on MC statistics is stale (LOW)

1. **Location:** `sections/radiative_corrections.tex:24`
2. **Note claim:** "\color{red} TODO: Figure out why we seem to have low MC stats when we have ~10x the data stats. \color{black}"
3. **Current baseline:** Sparse-bin finiteness is closed at the current fineness: `490/490` finite per generator per charge, rel-stat median `0.42%`, max `5.74%` (`STATE.md:62`); per-bin `C_rad_stat_unc` is carried in the tables (sidis-hq2-radiative skill). The remaining open item is the *policy* for MC-statistics-limited bins (P-CRAD-SPARSE: "Define marking, merging, or smoothing policy … before publication-grade Born cross sections", `docs/status/current-release.md:54`).
4. **Severity:** low (stale editorial leftover, but must be resolved before circulation).
5. **Recommendation (proposed text):** "All 490 bins per charge are finite in both generators; the median relative statistical uncertainty is 0.42% with a maximum of 5.74%. A marking/merging/smoothing policy for the remaining MC-statistics-limited bins is still to be defined (P-CRAD-SPARSE)." Delete the red TODO.

### Verified-consistent claims in `radiative_corrections.tex` (no discrepancy)

- $C_{\mathrm{rad}} = \sigma_{\mathrm{nonrad}}/\sigma_{\mathrm{rad}}$ definition (lines 8–12) — matches the baseline definition (`docs/status/radiative-corrections.md:9–12`).
- Both generators interfaced to HERACLES for RAD/NONRAD generation (line 13) — consistent with the baseline RAD/NONRAD chain; not contradicted.
- "median relative statistical uncertainty is below 0.5%" (line 23) — consistent with the current rel-stat median 0.42% (though quoted here in the superseded v1 context; see R1).

---

## 5. UNVERIFIED note claims (cannot be confirmed from the baseline docs)

Per the audit instruction, these are marked UNVERIFIED rather than assumed correct:

1. **`unfolding.tex:14`** — "only 0.19% of tracks are reconstructed with the wrong charge" (U5). Conflicts with the workspace's recorded 0.35% integrated charge confusion (open systematic CC2). Needs a full-production measurement.
2. **`unfolding.tex:14`** — "Unfolding with larger numbers (>2×) of reconstruction-level bins gave similar results at the cost of significantly larger compute time." Historical v4-era claim; no current-baseline doc confirms it for v6/z005.
3. **`unfolding.tex:10`** — "worst acceptances … where the tracks predominantly fall at $\eta>1.6$ in the lab frame" (U10). Mechanism unverified; low-acceptance diagnostics exist but were not consulted in this audit.
4. **`unfolding.tex:10`** — "worst purities … around 5%" (U12). Approximately consistent with the plotted all-fake-response table (worst averaged bins 4.4–6.1%; per-generator minimum 3.6%); the central-response purity is not available in the docs and must be recomputed (see U9).
5. **`unfolding.tex:20`** — "note-ready diagnostics derived from the validated cross-closure input table." A 2026-06-14 model-crossclosure table exists (`output_v6/systematics/model_crossclosure/ibu_crossclosure_z005_2026-06-14/`), but the note figures' md5 matches nothing in that study and no doc records these specific figure files; provenance unconfirmed. The word "validated" is itself contradicted by the closure plan (U7).
6. **`backgrounds.tex:8`** — Pythia photoproduction evaluation, "only 5 photoproduction events … expected" (B5). No such study in the current baseline; the baseline closes photoproduction by event-level cuts.
7. **`backgrounds.tex:8`** — "The phase space of $60 < Q^2 < 150$ GeV$^2$ …; the region $Q^2 < 60$ GeV$^2$ …" boundary values, and "sub percent in almost all bins" per-bin claim (B7). Integrated low-Q2 is sub-percent (0.0054%), but the boundaries and per-bin statement are unverified.
8. **`backgrounds.tex:37`** — e±/V0 backgrounds "located mostly at low-$z$". Plausible and not contradicted; no baseline doc states the z-distribution of the residual components.
9. **`backgrounds.tex:8`** — "For this analysis, diffractive DIS is not subtracted." No baseline doc addresses diffractive DIS; not contradicted, but unverifiable from the provided baseline.
10. **`radiative_corrections.tex:13`** — "The data are corrected to the $e^-p$ cross section" (R5). Current tables are beam-mixed; the claim's meaning and validity are unverified.
11. **`radiative_corrections.tex:15–17`** — DJANGOH14 MC luminosities 4,108,957 / 3,560,233 nb^-1. Traced to the historical `full_crad_v1` hard-coded route (so their *provenance* is verified as historical), but they are not the current baseline's exposures (current: `oomclumi`-derived, `DIAGNOSTIC_MANIFEST_LEVEL`, values in `crad_v3_summary.json` beside the xFcut tables).

---

## 6. Severity summary

| Section | High | Medium | Low | Total |
|---|---:|---:|---:|---:|
| `unfolding.tex` | 2 (U2, U9) | 7 (U1, U3, U4, U5, U6, U7, U8) | 3 (U10, U11, U12) | 12 |
| `backgrounds.tex` | 4 (B1, B2, B3, B4) | 2 (B5, B6) | 1 (B7) | 7 |
| `radiative_corrections.tex` | 2 (R1, R2) | 4 (R3, R4, R5, R6) | 1 (R7) | 7 |
| **Total** | **8** | **13** | **5** | **26** |

**Top 3 most consequential findings**

1. **R1** — The radiative section quotes the historical, normalization-blocked DJANGOH14 v1 C_rad (1.0553/1.0554, 478/490 finite) as the current correction; the current xFcut baseline is RAPGAP31 1.1368/1.1362 (nominal) and DJANGOH14 1.1419/1.1414, 490/490 finite, PROVISIONAL DIAGNOSTIC and not applied — a ~7–8% error in the stated correction.
2. **B1+B2** — The background total 285,747 is the superseded-356613-nb^-1, three-category number; the current L351000 baseline is 308,655.9 four-category (7.986% integrated fraction), and the note omits the det.int category (0.71% of reco tracks) entirely.
3. **U2** — The unfolding section presents plain-prior IBU k=4 as the validated nominal method with no prior-basis statement, while the stored plain prior is a critic-verified ±2.8% defect whose coherent fix is now the adopted nominal prior basis (2026-10-07, PROVISIONAL candidate re-release, not yet swapped into `central/data_release/`).

Close runners-up: **U9** (purity/acceptance figures are byte-identical to diagnostic all-fake-response plots, presented as central), **B4** (DIFFVM "studied and found negligible" vs assumed-zero-unassessed), **B3** (alpha0p2 diagnostic selection presented as central, P-ALPHA open), **R2** (superseded fLumi-v3 figures showing C_rad ≈ 1.025 vs current ≈ 1.14).

---

## 7. Risks, assumptions, and gaps in coverage

- **Baseline motion:** P-PRIOR moved from "ADOPTION PENDING" (RUN_STATUS 2026-10-06, matching the task briefing) to "RESOLVED/ADOPTED 2026-10-07" (docs/status/current-release.md, docs/status/unfolding-tunfold.md) during this audit. The discrepancy direction against the note is unchanged under either state; the note must state its prior basis and must not present the plain prior as correct or the coherent lineage as a completed central swap. Any note edit should re-check `docs/status/current-release.md` on the day.
- **Figure-internal content:** For `fig_reco_background_fractions_alpha0p2_onepage_7col.pdf` and the two `fig_ibu_crossclosure_*` figures, the exact generating run could not be re-located by md5 (the reco-background figure matches neither the 356613-convention study plot nor any located L351000 rendering). The text-level findings (B1, U7) stand on their own; the figure contents should be re-verified/regenerated as part of the note edit.
- **Out of scope:** Other note sections (e.g., `selection.tex`, which also presents the α≥0.2 cut as central selection at lines 128–130, and `systematics.tex`, which STATE.md already flags as carrying stale 552% text) were not audited beyond what was needed to verify claims in the three assigned sections.
- **No note files were modified**; all recommendations are proposed text for the note editors.

# Note-vs-Baseline Discrepancy Review — `species_definition.tex`, `binning.tex`, `observables.tex`, `acceptance.tex`

**Audit date:** 2026-10-07
**Auditor:** researcher (read-only audit; no note files were modified)
**Note under audit:** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/`
**Assigned sections:** `sections/species_definition.tex`, `sections/binning.tex`, `sections/observables.tex`, `sections/acceptance.tex`, plus a skim of `main.tex` for document-level claims.

## 1. Scope and method

Each assigned section was read in full and cross-checked against the authoritative baseline documents listed below, plus supporting baseline material discovered during the audit (all under `sidis_highQ2/`). Cross-reference targets invoked by the assigned sections (e.g. `sec:selection:final-tracks`) were also read. Claims that could not be verified from baseline documents are marked **UNVERIFIED**.

Baseline documents read in full:

- `docs/RUN_STATUS.md` (last updated 2026-10-06)
- `docs/status/current-release.md`
- `docs/status/unfolding-tunfold.md`
- `docs/status/alpha0p2-background.md`
- `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`
- `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`

Supporting baseline material read (all within `sidis_highQ2/`):

- `STATE.md` (last updated **2026-10-07** — newer than RUN_STATUS; see §2)
- `docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md`
- `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` (esp. §1–§2)
- `docs/unfolding/perkinbin_7matrix_plan_2026-07-06.md` (current 7-cell (Q²,x) grid)
- `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md`
- `docs/mc/prior_weight_semantics_2026-10-06.md`
- `docs/critiques/critique-v6-or-envelope-migration-2026-06-05.md`
- `docs/methodology/pTh_phih_frame_bug_2026-05-24.md` (φ_h/pT_h frame fix)
- `docs/methodology/binning_freeze_2026-05-24.md` and `docs/methodology/binning_adoption_HQ-v5g-Bprime_2026-05-24.md` (historical B-prime scheme)
- `docs/archive/pre-compaction-startup-pipeline-2026-07-03/RUN_STATUS_20260703T000000.md` (archived contract/trap statements)
- Note files read for cross-reference/context only: `sections/selection.tex`, `sections/unfolding.tex`, `sections/results.tex` (portions), `analysis_note/.../README.md`

## 2. Critical baseline status straddle discovered during the audit (P-PRIOR)

The task's listed baseline documents (RUN_STATUS.md, current-release.md, unfolding-tunfold.md, coherent_prior_fix_results_2026-10-05.md — all reflecting the 2026-10-06 state) state that the coherent-prior fix is **fork-validated but ADOPTION PENDING (P-PRIOR)**, with the central release still consuming the plain (`w_gen`-only) prior.

However, `STATE.md` (last updated **2026-10-07**, the audit date) records a newer status:

> "Release | CURRENT RESULT LINEAGE (coherent prior ADOPTED 2026-10-07, PROVISIONAL until pending gates close): `output_v6/central/data_release_candidates/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/` — not yet swapped into `central/data_release/`. On-disk central release ... 2026-06-10 ... is SUPERSEDED AS PHYSICS BASIS, retained untouched as provenance; every released-value quote must state its prior basis." (STATE.md line 10)

> "'Coherent-prior adoption pending' claims -> ADOPTED 2026-10-07 with explicit pending gates (B1 floor decision; model/reg covariance supplement rebuild; refreshed QA/systematics review; P-PLOTS)." (STATE.md line 81)

This means the task-listed baseline docs are themselves one day stale on P-PRIOR relative to STATE.md. **The note is discrepant under either status** (it discloses no prior basis at all — see Finding H1), and the recommendations below are drafted against the newest status (STATE.md, 2026-10-07). A docs-side action item (outside the note) is that `RUN_STATUS.md`, `docs/status/current-release.md`, and `docs/status/unfolding-tunfold.md` still describe P-PRIOR as adoption-pending and should be updated to the 2026-10-07 adoption-with-pending-gates state.

Common facts that hold under both strata (used throughout): the stored plain prior is a validated defect (±2.8% peak-to-peak vs the response truth marginal `body+miss`, 245/245 gen bins, both charges; `docs/unfolding/coherent_prior_fix_results_2026-10-05.md` §1); the k=4 coherent-vs-plain shift is ~1% per-bin median (max 3.583%/3.463%), +0.1240% integrated, efficiency-dominated, initialization negligible; the acceptance column `A_j = body_j/(body_j+miss_j)` and the BBB path are unaffected (same report §1, §5–6); binning, response shape, k=4, luminosity, and data vector are unchanged between the 2026-06-10 release and the coherent-prior candidate (`docs/backgrounds/alpha0p2_application_audit_2026-10-06.md`, "Candidate inheritance check").

## 3. Section-by-section findings

Severity definitions used: **high** = wrong physics/number presented as current; **medium** = superseded labeling or status; **low** = cosmetic/stale reference or completeness gap.

### 3.1 `sections/species_definition.tex`

Verified consistent with the baseline (no discrepancy):

- Truth pool restricted to charged pions, kaons, protons, `|PDG| ∈ {211, 321, 2212}` (lines 13–17): verified — the canonical v6 response-builder gen leg applies "species pool `|PDG| ∈ {211, 321, 2212}`, prompt/GTR via `IsV0DaughterGen==0` V0-daughter veto" (`docs/critiques/critique-v6-or-envelope-migration-2026-06-05.md` lines 63–67); gen pool "pi/K/p only" (`docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` §1.2 line 56); C_rad hadron pool pi/K/p with `IsGtr==1`, `Mother2==-1`, V0-daughter veto (same doc line 86).
- No species PID; all selected tracks counted as inclusive h± (lines 8–11, 21–22): verified — "h+/h- without species PID" (bgsub plan line 7); the α<0.2 cone veto referenced by "veto requirements" is applied upstream at v6 tree production to both the data vector and the central response inputs (`docs/backgrounds/alpha0p2_application_audit_2026-10-06.md` lines 11–17), so the phrase is accurate.
- Abstract-level "primary vertex π±, K±, p/p̄" (main.tex line 74) matches the prompt/GTR pool requirement.

Findings:

**S1 (LOW, UNVERIFIED) — cross-analysis convention claim.**
1. `sections/species_definition.tex` line 18.
2. Note claim: "This convention is shared by the low- and high-$\Qsq$ charged-SIDIS analyses."
3. Baseline: no high-Q² baseline document states the low-Q² analysis's truth-pool convention. The low-Q² workspace (`sidis_lowQ2/`) is outside this audit's allowed scope, and the workspace rules treat the analyses as self-contained. The note repo's own README describes the sister analysis as sharing framework/dataset but having *separate* steering and binning (`analysis_note/.../README.md` lines 12–17) and does not confirm a shared truth convention.
4. Severity: low (unverifiable cross-analysis claim; risk of stale cross-reference).
5. Recommendation: **concrete additional check needed before the text can be decided** — a cross-check of the low-Q² truth-pool definition (species list, prompt/GTR requirement, V0-daughter veto) against `sidis_lowQ2/` sources; domain: low-Q² SIDIS analysis owner / QA (out of this audit's scope). If verification is not possible now, replace the sentence with: "The truth-level convention is documented here for the high-$\Qsq$ analysis; the low-$\Qsq$ charged-SIDIS analysis documents its own convention in its workspace."

**S2 (LOW) — V0-daughter and prompt/source restrictions not stated in the species section.**
1. `sections/species_definition.tex` lines 13–20.
2. Note claim: the truth convention is "restricted to charged pions, charged kaons, and protons" and "excludes hyperons and other charged particles from the truth denominator".
3. Baseline: correct as far as it goes, but incomplete — the implemented pool also (a) excludes V0 daughters (K0S/K0L/K0/Λ decay products) even though they are π/K/p (`docs/critiques/critique-v6-or-envelope-migration-2026-06-05.md` line 65; `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md` §2), and (b) requires prompt/GTR hadrons ("prompt/GTR"; C_rad pool `IsGtr==1 && Mother2==-1`). `acceptance.tex` line 23 covers this only as the terse "species/source/V0-veto conventions".
4. Severity: low (completeness, not error).
5. Recommended text change — append to line 20: "…for acceptance and migration purposes. The pool is further restricted to prompt generator-truth hadrons and excludes V0 daughters ($K^0_S$, $K^0_L$, $K^0/\bar K^0$, $\Lambda/\bar\Lambda$ decay products) even though they are $\pi/K/p$."

**S3 (LOW) — data/MC asymmetry of the reco-level vetoes glossed.**
1. `sections/species_definition.tex` lines 10–11.
2. Note claim: "At reconstruction level all selected charged tracks enter either the $\hplus$ or $\hminus$ sample, subject to the standard charged-SIDIS track-quality and veto requirements."
3. Baseline: in the central selection the matched-electron and V0-daughter vetoes are MC truth-label vetoes (`IsMatchedElectron==0`, `IsMatchedV0Daughter==0`) that are **inert on data** (sentinel-zero truth flags), so real electron tracks and V0 daughters do enter the data samples and are handled as response fakes; only the α≥0.2 cone veto acts on both data and MC (upstream, tree level). (`docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` §1.1 note and §2 "Truth-label vetoes"; `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md` lines 11–24.)
4. Severity: low (vague phrasing, not a wrong claim).
5. Recommended text change — append after line 11: "The matched-electron and V0-daughter vetoes are MC truth-label requirements and are inert on data, where the corresponding tracks enter the samples and are treated as response fakes; the near-electron opening-angle veto acts on both data and MC."

### 3.2 `sections/binning.tex`

**Verified fully consistent with the current baseline — no discrepancies found.** The task asked for special attention to the exact current z edges and bin counts; itemized verification:

- z edges `[0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]`, 7 bins (line 32): matches `docs/RUN_STATUS.md` line 58, `docs/status/current-release.md` line 17, `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` §2 line 134. First bin 0.0–0.05 (also explicit in `sections/selection.tex` line 125); no `z ≥ 0.005` cut implied anywhere, and no `zlow005` misinterpretation appears (bgsub plan §2 line 129: "`z >= 0.05` and `pT_h > 0.5 GeV` are NOT selection cuts").
- Published pT edges `[0.0,0.5,1.0,2.0,4.0,8.0]` GeV, 5 bins (line 33): matches bgsub plan §2 line 135 and `docs/status/alpha0p2-background.md` line 56 (pair-summed pT bins).
- (Q²,x) intervals, 7 cells (line 34): matches the current 7-cell grid exactly — S0 `[150,300)×[0.0021,0.0071)`, `[0.0071,0.030)`; S1 `[300,700)×[0.0042,0.016)`, `[0.016,0.069)`; S2 `[700,1500)×[0.0099,0.034)`, `[0.034,0.15)`; S3 `[1500,20000)×[0.021,1.5)` — `docs/unfolding/perkinbin_7matrix_plan_2026-07-06.md` lines 17–23; bgsub plan §2 lines 132–133 ("S3:1 → 7 (Q2,x) cells"); corroborated by `docs/diagnostics/highx_highz_lowpt_onefile_2026-09-22.md` line 9. Importantly, the note does **not** present the historical v5g "Scheme B′" 8-cell scheme (S3 split at x=0.087 → 280 bins/charge) from `docs/methodology/binning_freeze_2026-05-24.md` / `binning_adoption_HQ-v5g-Bprime_2026-05-24.md`; the current 7-cell grid is presented. Correct.
- 245 published bins per charge, 490 total (lines 35, 45–47): matches `docs/RUN_STATUS.md` line 58 (490 = 245 h+ + 245 h−); arithmetic 7 cells × 7 z × 5 pT = 245 verified (bgsub plan line 136).
- Response `(491,246)` per charge, data-vector length 980 (line 36): matches `docs/RUN_STATUS.md` line 59, `docs/status/current-release.md` line 18, `docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md` line 20.
- Reco side "twice as fine in $\pTh$, using the ten-bin edge set given in Sec.~\ref{sec:selection:final-tracks}, with 490 reconstructed bins total per charge" (lines 49–51): matches bgsub plan §2 lines 135–136 ("reco is 2× finer in pT only"; reco edges `[0.0,0.25,0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0,8.0]` → 10 bins; 7×7×10 = 490). The cross-reference target `sec:selection:final-tracks` exists (`sections/selection.tex` line 104) and contains exactly that ten-bin edge set (lines 122–125).
- "An additional row and column in the stored migration matrix is used for handling fakes and misses" (lines 51–52): matches the miss row 490 / fake column 245 convention (`docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md` line 20; `docs/status/unfolding-tunfold.md` "Native convention").
- Figure `figures/HERA_xQ2_virtual_photon_eta.pdf` exists in the note's figures directory.
- The table caption "Validated binning and response contract for the current high-$\Qsq$ analysis release" remains valid under the 2026-10-07 coherent-prior adoption: the candidate lineage keeps the same response ROOT, binning, k=4, and luminosity (`docs/backgrounds/alpha0p2_application_audit_2026-10-06.md`, "Candidate inheritance check").

No text change required in `binning.tex`. (The superseded B′ labeling problem does exist elsewhere in the note repo — see Finding M4.)

### 3.3 `sections/observables.tex`

Verified consistent with the baseline:

- Luminosity `L = 351 pb⁻¹` (line 8): matches `L_data = 351000.0 nb^-1 = 351 pb^-1` (`docs/RUN_STATUS.md` lines 113–119; STATE.md line 12).
- 4D charge-separated cross sections binned in (Q², x, z, pT_h) as the primary observable (line 8): matches the release contract.
- Multiplicative Born correction `c_rad` (lines 8–12): sign/direction consistent with `C_rad = σ_nonrad/σ_rad` (STATE.md line 14; bgsub plan §1.5 line 85).
- φ_h and the moments ⟨cos φ_h⟩, ⟨cos 2φ_h⟩ defined in the γ*p CM frame, with "Earlier lab-frame diagnostic columns … not used for the current release" (lines 14–17): verified — the lab-frame pT_h/φ_h computation was a documented bug; the canonical implementation is CM-frame (γ*p), with the lab-frame `phi_h_old` retained only as a broken audit reference and all pre-2026-05-24 lab-frame harmonics invalid (`docs/methodology/pTh_phih_frame_bug_2026-05-24.md` lines 9–23, 33, 50–57, 75; `docs/radiative/crad_release_fiducial_contract_2026-09-26.md` line 110: "Do not use the obsolete lab-frame `safeTrentoPhi`"). CM-frame computation is a hard release-compatibility gate (`docs/radiative/crad_machinery_guide_2026-09-28.md` line 129).

Findings:

**O1 (MEDIUM) — 4D bin-volume definition omits the Δx cap for the highest-x cell.**
1. `sections/observables.tex` lines 8–12 (equation at lines 10–12).
2. Note claim: the differential cross section is `N_Unf × c_rad / (L × ΔxΔQ²ΔzΔpT_h)`, with "the 4D bin volume (ΔxΔQ²ΔzΔpT_h)" and no qualification of Δx.
3. Baseline: the highest-x cell (S3: 1500 < Q² < 20000, 0.021 < x < 1.5, as tabulated in `binning.tex` line 34) has a nominal upper edge exceeding the physical limit x < 1. The documented convention for 4D densities is to cap the highest-x bin at `Δx = 1.0 − x_min` when `x_max > 1.0` (`docs/RUN_STATUS.md` line 18), and the note's own results chapter applies exactly this cap for the diagnostic 4D plots (`sections/results.tex` lines 50–51). A reader applying the observables formula naively would use Δx = 1.479 instead of 0.979 and dilute the S3-cell density by a factor ≈ 1.51.
4. Severity: medium (incomplete definition that can produce a wrong number; the caveat exists elsewhere in the note but not at the definition).
5. Recommendation: concrete text change — add after the equation: "For the highest-$x$ interval ($1500<\Qsq<20000~\GeVsq$, $0.021<x<1.5$) the nominal upper edge exceeds the physical limit $x<1$; the bin volume used in the denominator is $\Delta x = 1.0-x_{\min}$." In addition, **a concrete check is needed before final wording**: confirm from the release schema/results tooling whether the *final published* tables apply the same cap as the diagnostic plots (`--differentiate-4d` convention, RUN_STATUS line 18) or quote per-bin cross sections without volume division; domain: results/QA. (See UNVERIFIED item U4.)

**O2 (LOW) — `c_rad` presented in the observable definition without current application status.**
1. `sections/observables.tex` lines 8–12.
2. Note claim: the defined observable includes the radiative correction factor `c_rad`.
3. Baseline: the current central v6 release is raw/no-`C_rad`, not Born level (`c_rad_applied=false`); the xFGen<0 xFcut `C_rad` baseline (RAPGAP31 nominal, DJANGOH14 cross-check, 490/490 finite) is PROVISIONAL DIAGNOSTIC until reviewed, and the 2026-10-05 chain born tables still carry the superseded blind C_rad pending an S3 re-attach (STATE.md line 14; `docs/RUN_STATUS.md` lines 133–148). The definition itself is the intended final observable and is not wrong; the note's results chapter already states the radiative correction is not applied in the diagnostic plots (`sections/results.tex` line 103).
4. Severity: low (definition vs status; no false claim in this section).
5. Recommended text change — append one status sentence after the equation: "The current central release is raw/no-$c_{\mathrm{rad}}$ (not Born level); the per-bin $c_{\mathrm{rad}}$ baseline is provisional pending review (Sec.~radiative corrections)." Cross-check that the radiative-corrections chapter carries the same status (outside this audit's sections).

**O3 (LOW) — provenance of `N_Unf` (raw/no-bgsub + response fake fraction) not stated.**
1. `sections/observables.tex` lines 8–12.
2. Note claim: the observable is defined from "the unfolded yields (N_Unf.)" with no statement of background handling.
3. Baseline: the central release is explicitly raw/no-bgsub and applies only response fake-fraction subtraction (`d_sig = data × (1−f_i)`, removing 10.7141% h+ / 10.4019% h−); electrons and V0 are vetoed in selection, not subtracted; no external alpha0p2 background vector is consumed (STATE.md line 34; `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md` line 24).
4. Severity: low (completeness; the backgrounds chapter may carry this — cross-check).
5. Recommendation: add one sentence ("The unfolded yields are obtained from the raw data vector with response-derived fake-fraction subtraction; no external background template enters the central release — see Sec. backgrounds") and verify the backgrounds chapter states the same; domain: backgrounds.

### 3.4 `sections/acceptance.tex`

Verified consistent with the baseline:

- Response built per charge; shape `(491,246)` = 490 reco + 245 gen bins + bookkeeping; data vector length 980 (lines 14–19): matches `docs/RUN_STATUS.md` line 59 and the investigation doc.
- Response-builder gen-leg convention (lines 21–26): verified in detail — "No active gen Q², gen y, gen Empz cut for the migration build; no `xF`, `GenPt`, or `GenEta`. Only finite-`zGen`, species pool `|PDG| ∈ {211, 321, 2212}`, prompt/GTR via `IsV0DaughterGen==0` … Final fiducial is imposed at TUnfold / truth-selection downstream" (`docs/critiques/critique-v6-or-envelope-migration-2026-06-05.md` lines 63–67); corroborated by the archived run-status row "Final fiducial | Imposed at TUnfold / truth-selection stage only" (`docs/archive/.../RUN_STATUS_20260703T000000.md` line 399) and by the fill-path audit (`docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md` line 31: gen pool requires event/numerical, pool, V0, charge, coarse-bin checks — no reco selection, no gen fiducial cuts).
- Release contract for nominal + shifted releases (245/490/490; lines 28–30): matches the tracking-ensemble per-seed validation (490 rows = 245+245, finite 490/490, 0 negatives; `docs/RUN_STATUS.md` lines 83–95) and the archived statement that HFS/electron shifted releases "were built with the same z005 IBU contract" (archived RUN_STATUS line 202). The coherent-prior candidate keeps the same contract (alpha0p2 application audit, "Candidate inheritance check").
- BBB retained as diagonal reference/cross-check (lines 10–11): consistent — the acceptance column and BBB are unaffected by the prior defect (`docs/unfolding/coherent_prior_fix_results_2026-10-05.md` §1, §6).
- The chapter makes **no purity claims** and uses no alpha0p2-response figures; on its own text it does not conflict with the alpha0p2 diagnostic status. (The purity/acceptance figures and claims live in `sections/unfolding.tex` — see §3.6 adjacent findings.)

Findings:

**H1 (HIGH) — central result presented without prior basis; plain-prior defect and coherent-prior adoption omitted.**
1. `sections/acceptance.tex` lines 8–12 and 28–30.
2. Note claim: "The BBB correction is retained as a transparent diagonal reference and as a release cross-check, while the current central z005 working result uses IBU with nominal iteration k=4." and "The nominal z005 release and all current shifted systematic releases use the same release contract…" — presented as the validated current result, with no mention of the IBU prior basis anywhere in the note (grep over the whole note finds no occurrence of "prior", "coherent", "body+miss", or "w_gen").
3. Baseline: the central release's stored plain (`w_gen`-only) IBU prior is a **validated defect** — it is not the response's own truth marginal (`body+miss`) in 245/245 gen bins for both charges, ±2.8% peak-to-peak (h+ sum mismatch +0.1036%, h− +0.0958%); it enters IBU through both the D'Agostini initialization and the efficiency normalization; the coherent `body+miss` fix shifts k=4 results by ~1% per-bin median (max 3.583%/3.463%), +0.1240% integrated, efficiency-dominated, acceptance/BBB unaffected (`docs/unfolding/coherent_prior_fix_results_2026-10-05.md` §1, §4–6; `docs/RUN_STATUS.md` blocker 6; `docs/status/unfolding-tunfold.md` lines 7–9). Per STATE.md (2026-10-07): the coherent prior is **ADOPTED as the NOMINAL IBU PRIOR METHOD**, the current result lineage is the coherent-prior candidate `bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/` (PROVISIONAL until the B1 floor decision, model/reg covariance supplement rebuild, refreshed QA/systematics review, and P-PLOTS close), and the 2026-06-10 release is "SUPERSEDED AS PHYSICS BASIS… every released-value quote must state its prior basis" (STATE.md lines 10, 68–69, 78–81). Under the task-listed 2026-10-06 docs the same text is already discrepant (validated defect + adoption pending undisclosed).
4. Severity: high (the physics basis of the central result is defective/superseded and the note presents the old basis as the current validated result with no prior-basis statement).
5. Recommended text change — replace acceptance.tex lines 9–12 with:
   "The z005 release stores both bin-by-bin (BBB) and iterative Bayesian unfolding (IBU) products. The BBB correction is retained as a transparent diagonal reference and as a release cross-check, while the central result uses IBU with nominal iteration $k=4$. The IBU prior basis is the response's own truth marginal (\emph{body+miss}): the plain $w_{\mathrm{gen}}$-only prior stored in the 2026-06-10 release was subsequently identified as a defect (a $\pm2.8\%$ per-bin deviation from the \emph{body+miss} marginal in 245/245 generator bins, both charges), and the coherent \emph{body+miss} prior was adopted as the nominal IBU prior method on 2026-10-07. The current result lineage is the coherent-prior candidate release (provisional until the pending gates close); relative to the plain-prior release it shifts the $k=4$ result by $\sim1\%$ per-bin median (maximum $3.6\%$) and $+0.124\%$ integrated, an efficiency-dominated effect that leaves the acceptance column and the BBB path unchanged. All released values are quoted with their prior basis."
   The same prior-basis statement must be mirrored wherever the note quotes cross-section values (results/systematics chapters — other auditors' sections), per STATE.md line 10.

**A2 (MEDIUM) — validation story omits the raw/no-bgsub fake-handling status and the open P-ALPHA validation.**
1. `sections/acceptance.tex` lines 28–33 (validation paragraph), in the context of lines 8–19 (response/fake bookkeeping discussion).
2. Note claim: "Validation checks require finite cross sections in all 490 IBU rows and no negative bins in the study-local shifted releases used for the detector/track-selection robustness comparisons." — implying the release validation story is complete.
3. Baseline: the central release is raw/no-bgsub with response fake-fraction subtraction only; no external background template is consumed; all alpha0p2 reco-background and all-fake response products are diagnostic/provisional and central products were not overwritten; response-consistent fake/background closure and covariance (P-ALPHA) remain open blockers (`docs/status/alpha0p2-background.md` lines 6–10, 46–52; `docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md` §7, §9; STATE.md lines 34, 67; `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md` lines 40–46: "This does not close P-ALPHA").
4. Severity: medium (status omission in a chapter that frames the release validation).
5. Recommended text change — append after line 33: "The central release is raw/no-background-subtraction: the only background handling is the response-derived fake fraction applied at unfolding, and no external background template (electron, V0-daughter, detector-interaction, or low-$\Qsq$ leakage) enters the central release. Response-consistent fake/background treatments under study (near-electron veto, all-fake response variants) remain diagnostic/provisional and are not part of the central result."
   No additional analysis test is required for this text; the underlying validation work is the already-open P-ALPHA item (domain: backgrounds/unfolding).

**A3 (LOW, partially UNVERIFIED) — "no negative bins in the study-local shifted releases" is directly evidenced only for part of the set.**
1. `sections/acceptance.tex` lines 31–33.
2. Note claim: "…no negative bins in the study-local shifted releases used for the detector/track-selection robustness comparisons."
3. Baseline: explicit zero-negative/finite-490 validation is documented for the tracking ensemble (`docs/RUN_STATUS.md` lines 87, 94), the central release (STATE.md line 34: "0/980 negative reco inputs, 0/490 negative cross sections"), and the coherent-prior candidate (STATE.md line 68: "490 rows finite / 0 negatives"). The HFS and electron shifted releases are described as "validated" with the same z005 IBU contract (archived RUN_STATUS lines 177–181, 202) but no explicit negative-bin statement for them was found in the documents read.
4. Severity: low (claim is plausible and consistent; direct evidence incomplete in the read baseline).
5. Recommendation: **concrete check** — read the per-release QA summaries for `hfs_jes_down`, `hfs_rces_up`, `hfs_theta_up`, `elecE_p1pct`, `elecTheta_p1mrad` (under `output_v6/systematics/...`, NAF side) and confirm each states finite-490/0-negative before the sentence is retained verbatim; domain: systematics/QA. (See UNVERIFIED item U3.)

**A4 (LOW, UNVERIFIED) — response-builder design presented as settled; the 2026-06-05 critic's requested decisions are not cited.**
1. `sections/acceptance.tex` lines 21–26.
2. Note claim: "The response-builder stage deliberately avoids imposing final generator-level physics cuts inside the migration matrix. … This avoids mixing detector-acceptance inefficiency with a generator-side detector-geometry cut." (Presented as a settled, validated design.)
3. Baseline: the described convention is accurate (see verified items above), but the independent critic review of exactly this design returned verdict **CONCERN (not FATAL, not SOUND)**: two residual gen-side selections (the broad gen-y window scope and the `zGen ∈ [0,1]` numerical filter scope) "require an explicit decision and a documented written justification before full production" (`docs/critiques/critique-v6-or-envelope-migration-2026-06-05.md` lines 22–37). No document recording that decision/justification was found in the searched tree (grep for decision/justification records citing this critique returned none).
4. Severity: low (the note's claim is accurate; the gap is the missing decision record/citation, i.e. a validation-provenance gap).
5. Recommendation: **concrete check** — locate the written decision/justification for (a) the broad gen-side y window and (b) the zGen filter scope (domain: unfolding). If it exists, cite it in the note; if not, produce it and add one sentence to the acceptance chapter, e.g.: "The residual gen-side selections (broad generator-level $y$ envelope and the finite-$z_{\rm gen}$ filter) were reviewed and retained with the published fiducial contained inside the generation envelope on every dimension." (See UNVERIFIED item U2.)

**A5 (LOW) — response generator provenance not stated.**
1. `sections/acceptance.tex` lines 8–19.
2. Note claim: describes "the response matrices" without stating which generator built them.
3. Baseline: the central response is RAPGAP (RAPGAP31) with a DJANGOH14 response cross-check; the coherent RAPGAP-response vs DJANGOH-C_rad choice remains an open physics review (`docs/RUN_STATUS.md` blocker 2; STATE.md line 64).
4. Severity: low (completeness).
5. Recommended text change — add to line 14: "The response is built separately for the two charges from the RAPGAP signal MC (nominal), with a DJANGOH response used as a model cross-check."

### 3.5 Document-level findings (`main.tex`, note-repo `README.md`)

**M1 (MEDIUM) — the Acceptance Corrections chapter is excluded from the built document.**
1. `main.tex` line 90: `%\input{sections/acceptance}` (commented out).
2. Note claim/state: the acceptance chapter exists as a source file but is not part of the compiled note; the response-contract, response-builder convention, and shifted-release validation statements it carries are absent from the built document (partially duplicated by `binning.tex`'s table and `unfolding.tex`'s figures).
3. Baseline: acceptance/migration treatment is a mandatory part of the current release contract and its validation story (`docs/RUN_STATUS.md` lines 56–64; STATE.md).
4. Severity: medium (structural/status: chapter content not presented).
5. Recommendation: after applying findings H1/A2 to `acceptance.tex`, re-enable `\input{sections/acceptance}` in `main.tex`; alternatively fold the content into the Unfolding chapter explicitly. Confirm no other `\ref{sec:acceptance}` targets dangle (none were found in the assigned sections).

**Verified (main.tex abstract, lines 71–76):** luminosity 351 pb⁻¹ ✓; truth-level convention "primary vertex π±, K±, p/p̄" ✓ (matches prompt/GTR pool); "490 published bins in total (245 h+ and 245 h−)" ✓. No discrepancy in the abstract for my sections.

**M4 (MEDIUM) — note-repo README presents superseded "Scheme B′" binning as current canonical.**
1. `analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/README.md` lines 16 and 43.
2. Note-repo claim: "…separate steering, binning (Scheme B′ canonical for high-Q²)…" and "4D binning: (Q², x, z, pT^h) with Scheme B′ canonical binning (frozen 2026-05-24)".
3. Baseline: the 2026-05-24 Scheme B′ freeze is the historical v5g-era 8-cell scheme (S3 split at x = 0.087 → 8 cells; `docs/methodology/binning_freeze_2026-05-24.md`, `docs/methodology/binning_adoption_HQ-v5g-Bprime_2026-05-24.md`), superseded by the current v6/z005 7-cell grid with 245 bins per charge (`docs/unfolding/perkinbin_7matrix_plan_2026-07-06.md`; bgsub plan §2; RUN_STATUS line 58). The task baseline explicitly states Scheme B′ labeling is historical/superseded and must never be presented as current canonical. (The LaTeX `binning.tex` chapter itself is correct — see §3.2.)
4. Severity: medium (superseded labeling presented as current canonical in the note repository's own documentation).
5. Recommended text change — README line 43: replace with "4D binning: (Q², x, z, pT^h) on the current v6/z005 grid — 7 (Q²,x) cells × 7 z bins × 5 published pT bins = 245 bins per charge, 490 total (the 2026-05-24 Scheme B′ freeze is historical/superseded)"; and line 16: delete "(Scheme B′ canonical for high-Q²)".

**M5 (MEDIUM) — note-repo README states superseded production sample and wrong Empz fiducial.**
1. `README.md` line 46.
2. Note-repo claim: "Production sample: SIDIS-local v5g 4-period sample, Empz=[35,70], ep0304 included".
3. Baseline: current central production is v6/z005 (`evls_v6_*.root`, `condor_v6/central/`, `output_v6/central/`); v5g products are on the DENY list for central work (`docs/RUN_STATUS.md` lines 24–46). The current event fiducial is `Empz > 45` GeV with **no upper bound** ("the old v6 code upper bound 70 is NOT part of the fiducial"; bgsub plan §2 line 120); the note's own `selection.tex` line 97 correctly says `E-p_z > 45 GeV`.
4. Severity: medium (superseded production status and a wrong fiducial window in note-repo metadata).
5. Recommended text change — replace line 46 with: "Production sample: central v6/z005 four-period S67 sample (evls_v6_*.root), ep0304 included; event fiducial Empz > 45 GeV (no upper bound)."

**L7 (LOW) — stale descriptors in the note-repo README.**
1. `README.md` lines 4, 44–47.
2. Claims: "charged-hadron SIDIS multiplicities and azimuthal modulations" (line 4); "Phase 2 systematics in progress (critic-iteration cycle)" (line 47); "Extractor: v3 (pT_h + φ_h frame fix applied 2026-05-24)" (line 45).
3. Baseline: the current release observable is cross sections, not multiplicities (multiplicities are blocked — no DIS denominator; `docs/unfolding/coherent_prior_fix_results_2026-10-05.md` header line 4); the systematics table is completed through tracking and the phi_h component (10→11 components; RUN_STATUS lines 61–64), with the phi_h budget re-based on the coherent-prior candidate 2026-10-07 (STATE.md line 58); the v3 extractor is the v5g-era downstream tool, while the current v6 chain computes CM-frame quantities in the tree builders/response helper (`docs/methodology/pTh_phih_frame_bug_2026-05-24.md` line 75; `docs/radiative/crad_machinery_guide_2026-09-28.md` line 129).
4. Severity: low (stale descriptors in repo metadata, not note text).
5. Recommended text change: line 4 → "…charged-hadron SIDIS cross sections and azimuthal modulations…"; line 47 → "Systematics: 11-component budget on the coherent-prior candidate basis (provisional)"; line 45 → retain only as history or delete ("Extractor: v3" is superseded by the v6 tree-builder chain).

### 3.6 Adjacent-scope observations (for the auditors of those sections; not counted above)

These were found while tracing cross-references from the assigned sections and are recorded here because they bear directly on the task's "acceptance/purity claims vs alpha0p2 diagnostic status" question:

- `sections/unfolding.tex` line 12: "The nominal unfolding method is the D'Agostini-style Iterative Bayesian Unfolding (IBU) **implemented in RooUnfold**" — inverts the baseline roles: the release IBU is the in-house implementation; RooUnfoldBayes is the independent cross-check that reproduced it to 2.6e-15 (`docs/RUN_STATUS.md` line 60; `docs/status/current-release.md` line 20). Also "TUnfold … is not an alternative central result" pre-judges the still-open P-METHOD nominal-method decision (RUN_STATUS blocker 3; STATE.md line 65).
- `sections/unfolding.tex` lines 8–10 and 43–73: the purity/acceptance claims and figures use the **alpha0p2 response** products (`fig_alpha0p2_response_purity_*`, `fig_alpha0p2_response_acceptance_*`), whose captions lack the diagnostic/provisional caveat that the all-fake matrix figure carries (line 25 "Diagnostic/provisional…"). Per `docs/status/alpha0p2-background.md` lines 6–10 all alpha0p2 response products are diagnostic/provisional. The "worst purities … around 5%" and "may be dropped from the final set of published bins" claims also need verification against the current response and bin-dropping policy. (Definitions themselves — purity N_stay/N_rec, acceptance N_rec/N_gen — are consistent with the baseline acceptance `A_j = body/(body+miss)`.) The 0.19% wrong-charge claim (line 14) is UNVERIFIED from the baseline docs read.
- `sections/results.tex` lines 50–51, 96–103: the Δx cap is correctly applied for the diagnostic plots and the missing C_rad/low-Q² subtraction is stated — consistent with baseline; results-section auditor should confirm the final-table convention (see U4).

## 4. Severity summary and counts

| Severity | Count | Findings |
|---|---|---|
| High | 1 | H1 |
| Medium | 5 | M1, M4, M5 (document/repo level); O1, A2 (assigned sections) |
| Low | 8 | S1, S2, S3 (species); O2, O3 (observables); A3, A4, A5 (acceptance); L7 (README) |

Of these, 4 involve claims that could not be fully verified from the baseline documents and are marked UNVERIFIED below. `binning.tex` itself contains **zero discrepancies** — every edge, count, and shape matches the current v6/z005 contract (itemized in §3.2).

## 5. UNVERIFIED items

- **U1 (= S1):** "This convention is shared by the low- and high-Q² charged-SIDIS analyses" (`species_definition.tex` line 18). Requires a low-Q²-side check (out of this audit's scope).
- **U2 (= A4):** Whether the explicit decision + documented justification requested by the 2026-06-05 critic for the broad gen-y window and the `zGen ∈ [0,1]` filter scope was ever recorded. No record found in the searched docs; the note's design description itself is verified accurate.
- **U3 (= A3):** Whether each of the five HFS/electron shifted releases has an explicit finite-490/0-negative QA statement (direct evidence found only for the tracking ensemble, the central release, and the coherent-prior candidate).
- **U4 (= O1):** Whether the final published cross-section tables apply the `Δx = 1.0 − x_min` cap for the S3 cell (documented only for the `--differentiate-4d` diagnostic plots, RUN_STATUS line 18) or quote per-bin cross sections without volume division.

## 6. Risks, assumptions, and gaps in coverage

- **Time-sensitivity risk (highest):** the P-PRIOR status changed on the audit date itself (STATE.md 2026-10-07: adopted, provisional, gates pending) versus the task-listed baseline docs (2026-10-06: adoption pending). Finding H1 and its recommended text are drafted against STATE.md; if the adoption is rolled back, the same finding stands with the "adoption pending" framing. Downstream budget tables were also re-based on the coherent-prior candidate on 2026-10-07 (STATE.md lines 58, 80) — this affects the systematics chapter (other auditors) and any note text quoting total uncertainties.
- The audit relied on baseline documents only; no ROOT files, release CSVs, or builder sources were opened (the canonical sources live under `H1ChargedSidis_HighQ2/`, outside this audit's allowed scope). Where baseline docs quote builder line numbers, they were taken as documented.
- The alpha cone veto's central status was resolved using the 2026-10-06 application audit (applied upstream at v6 tree level to both data vector and central response inputs); this makes the note's selection/species "veto requirements" phrasing accurate, and means the alpha0p2 *response/background products* being diagnostic is a separate question from the *cut* being central — the note should not conflate them (covered by A2's recommended wording).
- Coverage: the four assigned sections were audited exhaustively; `main.tex` was skimmed for document-level claims; README.md findings arose from the B-prime labeling check. Other sections (selection, backgrounds, unfolding, radiative, systematics, results) were touched only where cross-references from the assigned sections required it, and their issues are listed as adjacent-scope observations only.

2026-10-16: x_max=1.5 is an internal migration-catch container edge only; the published cell is x < 1.0 with Delta x = 1.0 - x_min = 0.979 (P-XVOL).

# Note-vs-Baseline Discrepancy Review — `introduction.tex`, `experimental_setup.tex`, `monte_carlo.tex`, `selection.tex`

**Audit date:** 2026-10-07
**Auditor:** researcher (read-only audit; no note files were modified)
**Note under audit:** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/`
**Assigned sections:** `sections/introduction.tex`, `sections/experimental_setup.tex`, `sections/monte_carlo.tex`, `sections/selection.tex`, plus a skim of `main.tex` and the note-repo `README.md` for document-level claims.

## 1. Scope and method

Each assigned section was read in full and cross-checked against the authoritative baseline documents listed below, plus supporting baseline material discovered during the audit (all under `sidis_highQ2/`; the production workspace `H1ChargedSidis_HighQ2/` was **not** read, per the audit's scope constraint). Claims that could not be verified from in-workspace baseline material are marked **UNVERIFIED**.

Baseline documents read in full:

- `docs/RUN_STATUS.md` (last updated 2026-10-07)
- `docs/status/current-release.md`
- `docs/status/normalization.md`
- `docs/status/alpha0p2-background.md`
- `docs/provenance/historical-paths.md`
- `STATE.md` (last updated 2026-10-07)

Supporting baseline material read (all within `sidis_highQ2/`):

- `docs/methodology/v6_production_changes_2026-06-04.md` (authoritative v6 response-builder gen-cut policy)
- `docs/unfolding/coherent_prior_fix_results_2026-10-05.md` (prior/response weight-incoherence evidence)
- `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md` (α-veto application point)
- `docs/data/g1j_ES_port_diff.md` (preselection cut-value port audit)
- `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` (central selection contract, Empz>45)
- `docs/diagnostics/highx_highz_lowpt_onefile_2026-09-22.md` (finalcandidate reco method)
- `docs/handoffs/SESSION_HANDOFF_backgrounds_systematics_2026-09-25.md` (CUT 13/14, `pass_reco_full`)
- `scripts/canonical_highQ2_sidis_2026-06-30/RUNLIST.md` (canonical response builder identity)
- `scripts/canonical_highQ2_sidis_2026-06-30/tree_production/audits/2026-06-06_wrapper_output_audit/REPORT.md` (stage-1/stage-2 file counts)
- `data/release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/README.md` and `RELEASE_NOTES.md` (central response identity, lumi rescale)
- `docs/research/research-diffvm-exclusive-rho-highq2-v6_2026-10-07.md` (in-workspace secondary source citing v6 Stage-1 source lines for the reco envelope)
- Note files read for document-level context: `main.tex`, `README.md`, `references.bib`
- Sibling audit reports in this directory (`binning_observables_acceptance.md`, `unfolding_backgrounds_radiative.md`, `systematics_results_summary.md`) — cross-referenced only; their sections are out of scope here.

## 2. Baseline status straddle (P-PRIOR) — same straddle as the sibling reports

The task's baseline facts reflect the 2026-10-06 state (coherent-prior adoption pending; canonical release = `bbb_ibu_finalcandidate_..._2026-06-10/`). `STATE.md` and `docs/RUN_STATUS.md` (both updated 2026-10-07, the audit date) record the newer status: the **coherent prior (IBU prior := response truth marginal `body+miss`, for initialization and efficiency) was ADOPTED 2026-10-07 as the NOMINAL IBU prior method for v6/z005**, executed as the dated candidate `output_v6/central/data_release_candidates/bbb_ibu_coherentprior_..._2026-10-06/` (not yet swapped into `central/data_release/`); the on-disk 2026-06-10 release is **SUPERSEDED AS PHYSICS BASIS**, retained as provenance, and "every released-value quote must state its prior basis" (STATE.md:10, 78–79; RUN_STATUS.md:30–32, 48–49, 85).

Findings and recommendations below are drafted against the newest (2026-10-07) status. The four audited sections are discrepant under either status (see H2): they disclose no prior basis at all and assert a weight consistency that was critic-verified as false for the 2026-06-10 release.

## 3. Findings summary

Severity definitions (matching the sibling reports): **high** = wrong physics/method description presented as current; **medium** = factual misstatement that would mislead a reader, or superseded labeling/status; **low** = cosmetic/stale reference or completeness gap.

| ID | Severity | Location | Issue |
|---|---|---|---|
| H1 | high | `monte_carlo.tex:29-34` | Response-builder gen-cut description is wrong: x_F/Q²/y/E−p_z gen fiducial cuts **are** imposed |
| H2 | high | `monte_carlo.tex:22-25` | "Weights applied consistently" contradicted by the critic-verified ±2.8% prior/response weight incoherence; no prior basis stated |
| M1 | medium | `monte_carlo.tex:17-19` | "all 1523 data files" misattributes the stage-2 total as a data count (1523 = 658 data + 865 MC) |
| M2 | medium | `monte_carlo.tex:10-14` | Central response (RAPGAP31) vs model diagnostic (DJANGOH14) roles not stated |
| M3 | medium | `monte_carlo.tex` (section gap) | No MC sample identity / luminosity / data-MC scale content |
| M4 | medium | `selection.tex:111-131` | Track-cut list omits reco-side V0-daughter veto, track-class requirement, and Stage-2 tagged-electron skip; α-veto application point imprecise |
| M5 | medium | note-repo `README.md:46` (out-of-section) | "Production sample: SIDIS-local v5g 4-period sample" — superseded; current production is v6/z005 |
| L1 | low | `experimental_setup.tex:12` | Citation `2005Hadroo2` undefined (bib key is `Peez:2005Hadroo2`) |
| L2 | low | `introduction.tex:36-40` | Release-contract paragraph commented out (its content is correct) |
| L3 | low | `monte_carlo.tex:26-27` | Response-shape decomposition omits the fake column (246 = 245 gen bins + 1 fake column) |
| L4 | low | `selection.tex:114` | "At least 5 hits in the CJC" imprecise — the cut is `NHitDedx>=5` (dE/dx hits) |
| L5 | low | `main.tex:90` (document-level) | `acceptance.tex` is commented out of the build |

Counts: **2 high, 5 medium, 5 low**, plus 6 UNVERIFIED items (§5).

## 4. Detailed findings

### H1 (HIGH) — Response-builder generator-level fiducial cuts wrongly described as absent

1. **Location:** `sections/monte_carlo.tex` lines 29–34.
2. **Note claim (quoted):** "The response builder does not impose final generator-level fiducial cuts such as GenPt, GenEta, $x_F$, final-$\Qsq$, $y$, or $E-p_z$ inside the migration-matrix construction. It applies the finite-$z_{\mathrm{gen}}$ filter and the charged hadron species/source/V0-veto conventions, while the final physics fiducial is imposed at the unfolding and truth-selection stage."
3. **Baseline fact:** the canonical v6 response builder (`unfolding/build_migration_th2_per_charge_v6.py`, confirmed as the current central-chain response source at `scripts/canonical_highQ2_sidis_2026-06-30/RUNLIST.md:57-58`) **keeps** the following generator-level cuts in the migration-matrix gen denominator (`docs/methodology/v6_production_changes_2026-06-04.md`, Change #1, "KEPT at gen", lines 56–67):
   - `Q2_es_gen > 150` and `0.1 < Y_es_gen < 0.7` (`EVENT_CUTS_GEN`);
   - `GenEmpz` window 35–70 GeV (`GEN_FID_OPTION_C`);
   - `xFGen < 0` (current-hemisphere target definition);
   - species pool `|GenPDG| ∈ {211, 321, 2212}`; prompt/GTR truth-source via the V0-daughter veto; `zGen` finite in [0,1].
   Only **two** gen cuts were removed in v6: the `GenPt > TRACK_CUTS_RECO["Pt_min"]` symmetrization and the `|GenEta|` geometric proxy (same doc, "REMOVED at gen", lines 69–74). The kept xF/Q²/y/E−p_z gen fiducial is also independently confirmed in the radiative-chain selection descriptions (e.g. `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md:53,86`: v4 fiducial on both legs includes `150 < Q2_es_gen < 20000`, `0.1 < Y_es_gen < 0.7`, `GenEmpz` lower bound, `xFGen < 0`).
4. **Severity:** high — the note inverts the central methodological fact of the v6 response design (which cuts live at gen vs which were deliberately removed), and would mislead any reader checking closure, acceptance, or the corrected-to phase space.
5. **Recommended text change** — replace lines 29–34 with wording to the effect: "The response builder imposes the generator-level event-kinematic fiducial $\Qsq_{\mathrm{gen}} > 150~\GeVsq$ and $0.1 < y_{\mathrm{gen}} < 0.7$, the generator-level $E-p_z$ window $35$–$70~\GeV$, and the current-hemisphere requirement $x_{F,\mathrm{gen}} < 0$, together with the charged-hadron species pool ($\pi^\pm$, $K^\pm$, $p/\bar p$), the prompt/GTR truth-source definition with the V0-daughter veto, and a finite-$z_{\mathrm{gen}}$ filter. The v6 production removed only the generator-level $p_T$ and $|\eta|$ detector-geometry proxies from the migration-matrix denominator, so that geometric acceptance is recovered through the response matrix rather than baked into the truth definition. The same migration matrix is reused for the nominal release and for systematic variants." (Exact gen-side Empz phrasing should follow the resolution of the open GenEmpz-threshold question — see U6.)

### H2 (HIGH) — "MC event weights are applied consistently" contradicted by the validated prior/response weight incoherence; no prior basis stated

1. **Location:** `sections/monte_carlo.tex` lines 22–25.
2. **Note claim (quoted):** "MC event weights are applied consistently in the response and data-vector construction. The standard factors are the generator weight, the L5 user weight, the trigger weight where applicable, and the NuclIA correction."
3. **Baseline fact:** in the 2026-06-10 central release that this note documents, weights were **not** applied consistently between the stored IBU prior and the response: the stored plain prior `h_prior_gen_{pos,neg}_rapgap` was filled with `w_gen = weight1*weight2` only, while the response body legs carry `w_reco = weight1*weight2*NucliaWeight` (plus full reco selection and matched-key resolution); the miss row uses `w_gen`. The resulting prior-vs-truth-marginal mismatch is ±2.8% peak-to-peak in 245/245 gen bins for both charges (h+ sum mismatch +0.1036%, h− +0.0958%), critic-verified as a FATAL implementation defect (`docs/unfolding/coherent_prior_fix_results_2026-10-05.md` §1, lines 16–42; `STATE.md:68`). The fix — the coherent prior, defined as the response's own truth marginal `body+miss`, used for both IBU initialization and efficiency — was **ADOPTED 2026-10-07 as the nominal IBU prior method** (STATE.md:68–69, 78; RUN_STATUS.md:85), with k=4 impact ~1% per-bin median (max 3.583%/3.463%), +0.1240% integrated, efficiency-dominated; acceptance and BBB are unaffected. The 2026-06-10 plain-prior release is superseded as physics basis and is on the DENY list for new physics claims (RUN_STATUS.md:48–49); "never rebuild priors the `w_gen`-only way; state the prior basis of every quote" (STATE.md:60, 78).
4. **Severity:** high — the note asserts as current method design the exact inconsistency that was critic-verified as a defect, and omits the prior-basis disclosure that the baseline now mandates for every released value.
5. **Recommended text change** — replace lines 22–25 with wording to the effect: "MC event weights enter the response and data vector per leg: matched response legs and the reconstructed data side carry the generator weight, the L5 user weight, and the NuclIA correction; the response miss row carries the generator weight only. The IBU prior is the coherent prior — the response's own truth marginal (matched body plus miss), used for both the iteration initialization and the efficiency normalization — adopted as the nominal method on 2026-10-07 after a critic-verified ±2.8% incoherence was found in the earlier plain-prior construction; the acceptance and bin-by-bin BBB paths are unaffected. All quoted cross sections state their prior basis." (Trigger-weight phrase: see U4.) This finding should be coordinated with the unfolding-chapter findings in the sibling report `unfolding_backgrounds_radiative.md`, which covers the same defect from the unfolding side.

### M1 (MEDIUM) — "all 1523 data files" misattributes the Stage-2 total as a data count

1. **Location:** `sections/monte_carlo.tex` lines 17–19.
2. **Note claim (quoted):** "Stage-1 event-list production contains 1525 files, and Stage-2 sidistree production is complete for the MC and for all 1523 data files."
3. **Baseline fact:** Stage-2 comprises **1523 files in total = 658 data + 865 MC**, not 1523 data files. The wrapper-output audit group tables give stage-2 data groups `data/03`: 20, `data/04`: 63, `data/05`: 152, `data/06`: 312, `data/07`: 111 → **658 data** (`scripts/canonical_highQ2_sidis_2026-06-30/tree_production/audits/2026-06-06_wrapper_output_audit/REPORT.md:85,90-94`); the same tables give stage-1 data groups 20+63+152+313+112 = **660 data** out of 1525 total (REPORT.md:48,53-57) — i.e. stage-1 = 660 data + 865 MC = 1525, stage-2 = 658 data + 865 MC = 1523. The 658 stage-2 data count is independently confirmed by the tracking-ensemble data-vector validation ("input count `658` with period counts `ep0304 83`, `em0405 152`, `em06 100`, `ep0607 323`", `docs/status/current-release.md:43`). "Complete" itself is correct (the one file still running at audit time, REPORT.md:105, later completed; RUNLIST.md Phase 0: "v6 sidistree production complete").
4. **Severity:** medium — a reader would conclude the data sample contains 1523 files (a factor ~2.3 overstated); the composition of data vs MC is a sample-identity fact.
5. **Recommended text change** — replace with: "Stage-1 event-list production contains 1525 files (660 data + 865 MC); Stage-2 sidistree production is complete with 1523 files (658 data + 865 MC)."

### M2 (MEDIUM) — Central response (RAPGAP31) vs model diagnostic (DJANGOH14) roles not stated

1. **Location:** `sections/monte_carlo.tex` lines 10–14.
2. **Note claim (quoted):** "The nominal signal-model comparison uses DJANGOH and RAPGAP response matrices. DJANGOH provides a DIS sample with QED radiative effects through HERACLES and QCD shower modelling through the standard H1 generator chain; RAPGAP provides an independent DIS generator model for the response and model uncertainty comparison."
3. **Baseline fact:** the **central response is RAPGAP31**; the DJANGOH14 response is included only as a model-difference diagnostic/cross-check. Release README: "Central response: RAPGAP31. DJANGOH14 response is included only as a model-difference diagnostic where available" (`data/release/bbb_ibu_finalcandidate_.../README.md:7`); "central response is RAPGAP and a DJANGOH-response cross-check exists" (RUN_STATUS.md:81; STATE.md:64). The generator descriptions themselves (DJANGOH/HERACLES, RAPGAP, GEANT) are correct.
4. **Severity:** medium — the note leaves the central-generator assignment ambiguous ("uses DJANGOH and RAPGAP response matrices"), which matters for interpreting the model systematic and the data-vs-MC comparisons (RAPGAP31 truth curves are not independent of the unfolded data, STATE.md:39).
5. **Recommended text change** — add one sentence: "The central response matrix is derived from RAPGAP (v3.1); a DJANGOH (v1.4) response is used as an independent model-difference cross-check and enters the model-uncertainty evaluation."

### M3 (MEDIUM) — No MC sample identity / luminosity / data-MC scale content

1. **Location:** `sections/monte_carlo.tex` (whole section; lines 8–20 in particular).
2. **Note claim:** the section names only the generator programs; it identifies no MC samples, request IDs, MC luminosities, or data/MC scale factors.
3. **Baseline fact:** the baseline requires sample identity and effective MC luminosity to be stated and verified before any normalization-sensitive use (`docs/status/normalization.md:64-66`, "Validation Rule"). Current values the note should carry (normalization.md:21-42, 55-62):
   - Signal MC for the radiative-correction derivation: DJANGOH 1.4 RAD sample, ReqID 7091, `NC.EPLUSP.0607.RAD.Q2GT60`, `L_MC = 4379935.0 nb^-1`, data/MC scale `351000.0/4379935.0 = 0.08013890218154498` (and the trap: `merged_7091.root` is not the DjBkgd template).
   - DjBkgd background samples: ReqIDs 7087/7104/7105/7107/7110, all `NC RAD Q2GT2 CTEQ6L`, steering sum `15813.4 pb^-1 = 15813400 nb^-1`, correct scale `351000.0/15813400 = 0.0221963651`, `L_MC/L_data = 44.34`; the prior scale `22.5513172` / ratio `0.044` was a pure pb⁻¹/nb⁻¹ unit error (normalization.md:36-42). Caveat that must accompany any absolute DjBkgd yield: exact-template exposure provenance is not closed (oomclumi/generator-log audit outstanding).
   - Response-generator luminosities for combined background vectors: RAPGAP31 `9132891.0 nb^-1` (scale 0.03843280892435371), DJANGOH14 `10487495.0 nb^-1` (scale 0.03346878747387811) (normalization.md:55-62).
4. **Severity:** medium — a reproducibility/traceability gap: the note cannot currently be used to identify which MC was produced, nor to reproduce the normalization; the fLumi unit-error history shows this is a live trap, not bookkeeping nicety.
5. **Recommendation:** add a short "MC samples and normalization" subsection to `monte_carlo.tex` listing generator versions, ReqIDs/genIDs, `L_MC` values, and the data/MC scale factors above, with the DjBkgd provenance caveat. Domain: normalization (h1 side: `docs/status/normalization.md` is the authority).

### M4 (MEDIUM) — Track-cut list omits reco-side vetoes and track-class requirement; α-veto application point imprecise

1. **Location:** `sections/selection.tex` lines 111–131 (final reconstructed-track selection).
2. **Note claim:** the track-cut list (lines 111–121) comprises pT>0.15 GeV, |η|<1.6, ≥5 CJC hits, radial length ≥5 cm, |DCA′|sinθ<2 cm, primary-vertex label, x_F<0, and finite in-binning z/pT_h. The α paragraph (lines 127–131) describes a reconstructed-side opening-angle requirement "calculated" per track.
3. **Baseline fact:** the central selection contract additionally includes (quoted verbatim from the `fiducial_contract` at `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md:38`): "central or short/combined trk class", "`IsMatchedV0Daughter==0`", and "alpha(track,e)>=0.2 rad reco-only, NO IsMatchedElectron==0 truth-label veto". Tagged electrons and V0 daughters "are vetoed in selection (`pass_reco_full`), not subtracted" (`docs/handoffs/SESSION_HANDOFF_backgrounds_systematics_2026-09-25.md:114`). The tagged scattered-electron track skip is applied at Stage-2 tree production (`src/ChargedSidisTreeBuilderHighQ2_v6.C:1573-1584`), and the α<0.2 cone veto is applied **upstream at v6 tree production** through the H1OO call `gH1Calc->Had()->RejectTracksNearScatElec(0.2)` configured in Stage-1 (`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:389-396`); Stage-2 has no alpha branch and neither the data-vector builder nor the response builder recomputes α (`docs/backgrounds/alpha0p2_application_audit_2026-10-06.md:11-24`). The note's per-track "calculated" phrasing implies an analysis-level calculation that does not exist in the release chain.
4. **Severity:** medium — the described selection is incomplete (a reader could not reproduce it) and mislocates the α veto; the physics content that is present (α≥0.2 rad, reco-only, loss numbers) is correct.
5. **Recommended text change:**
   - Append to the track-cut list: "the track is a central or short/combined track in the H1OO classification; the matched V0-daughter veto is applied on the reconstructed side; tracks identified by H1OO as the scattered-electron track are skipped at tree production."
   - In the α paragraph, replace "For each reconstructed track, the opening angle … is calculated in the lab frame" with wording to the effect: "The cone veto is applied at tree-production level through the standard H1OO near-electron rejection (`RejectTracksNearScatElec(0.2)`), so both the data vector and the response inputs inherit it; no additional α calculation is applied in the release chain." Keep the (correct) reco-only statement and the loss numbers.
   - Cross-reference the data/MC asymmetry nuance (truth-label vetoes inert on data; electron tracks handled as response fakes) already documented in the sibling report `binning_observables_acceptance.md`, Finding S3.

### M5 (MEDIUM, out-of-section) — Note-repo README presents the superseded v5g production sample as current

1. **Location:** note-repo `README.md` line 46 (document-level; the four assigned body sections contain **no** v5g claim — this was explicitly checked).
2. **Note claim (quoted):** "Production sample: SIDIS-local v5g 4-period sample, Empz=[35,70], ep0304 included."
3. **Baseline fact:** current central production is **v6/z005** (`STATE.md:5-8`: sources `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`, `src/ChargedSidisTreeBuilderHighQ2_v6.C`, condor `condor_v6/central/`, outputs `output_v6/central/`); all v5g products are DENY-listed as "historical/audit provenance only" (RUN_STATUS.md:40; `docs/provenance/historical-paths.md:27-28,33`). The `Empz=[35,70]` fragment conflates the response-builder gen window (35–70, GEN_FID_OPTION_C) with the production sample description. The README's "Scheme B′ canonical binning" claims (lines 16 and 43) are covered by the sibling report `binning_observables_acceptance.md`, Finding M4 (historical v5g-era 8-cell scheme, superseded by the current 7-cell 245-bin/charge grid) and are not repeated here.
4. **Severity:** medium — the README is the first thing a collaborator reads; it misdescribes the production basis of the entire note.
5. **Recommended text change** — replace README line 46 with: "Production sample: v6/z005 central production (four-period S67 sample incl. ep0304; stage-1 tree envelope E−p_z 30–75 GeV; response gen-level E−p_z window 35–70 GeV)." Apply together with the sibling report's M4 fixes for lines 16/43.

### L1 (LOW) — Undefined citation `2005Hadroo2`

1. **Location:** `sections/experimental_setup.tex` line 12.
2. **Note claim:** "…see Refs.~\cite{Kogler:2011zz,Britzger:2013,2005Hadroo2}".
3. **Baseline fact:** the bibliography key is `Peez:2005Hadroo2` (`references.bib:67`); `2005Hadroo2` is undefined and produces a "?" citation. Already flagged as a pre-existing note defect in `STATE.md:44`. All other keys cited by the audited sections exist (`Kogler:2011zz` bib:36, `Britzger:2013` bib:46, `Charchula:1994kf` bib:217, `Kwiatkowski:1990es` bib:227, `Jung:1993gf` bib:207, `Brun:1987ma` bib:260).
4. **Severity:** low (broken reference, no physics impact).
5. **Recommended text change:** cite `Peez:2005Hadroo2`.

### L2 (LOW) — Release-contract paragraph commented out in the introduction

1. **Location:** `sections/introduction.tex` lines 36–40 (commented block).
2. **Note claim (commented):** the working release covers Q²>150 GeV², 490 published bins (245 h⁺ + 245 h⁻), z edges [0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0], first bin 0.0 ≤ z < 0.05.
3. **Baseline fact:** the content is **correct and current** (`docs/status/current-release.md:18-19`: 490 bins = 245+245; z edges identical; `zlow005` is a path tag, not a physical cut; Q²>150 scope throughout). The surrounding figure caption (lines 45–47) states the Q²>150 region but no bin-count contract appears in the visible text.
4. **Severity:** low (the note's only release-contract statement is disabled).
5. **Recommended text change:** uncomment lines 36–40 (updating "current working release" to name the release lineage and prior basis per §2, e.g. the coherent-prior candidate adopted 2026-10-07).

### L3 (LOW) — Response-shape decomposition omits the fake column

1. **Location:** `sections/monte_carlo.tex` lines 25–27.
2. **Note claim (quoted):** "per charge the response matrix has shape $(491,246)$, corresponding to 490 reconstructed bins plus the additional response row and 245 generator bins."
3. **Baseline fact:** the shape is correct (`(491,246)` per charge, data vector length 980 — `docs/status/current-release.md:20`), but 491 = 490 reco bins + 1 **miss** row and 246 = 245 gen bins + 1 **fake column**; the note explains the +1 row but not the +1 column (`docs/status/alpha0p2-background.md:44`; the fake column is the mechanism behind the response-derived fake-fraction subtraction, `d_sig = data*(1-f_i)`).
4. **Severity:** low (correct number, incomplete decomposition).
5. **Recommended text change:** "…490 reconstructed bins plus the miss row, and 245 generator bins plus one fake column."

### L4 (LOW) — "At least 5 hits in the CJC" imprecise

1. **Location:** `sections/selection.tex` line 114.
2. **Note claim:** "At least 5 hits in the CJC".
3. **Baseline fact:** the implemented cut is `NHitDedx >= 5` (dE/dx hit count; release tag `nhit5`; contract quote "NHitDedx>=5" at `docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md:38`; `docs/diagnostics/highx_highz_lowpt_onefile_2026-09-22.md:9`). The total CJC hit count (`NHitCJC`) is a separate retained diagnostic branch, not the cut (`docs/methodology/v6_production_changes_2026-06-04.md:116`).
4. **Severity:** low (imprecision that could mislead a reproduction attempt).
5. **Recommended text change:** "At least 5 dE/dx hits in the CJC (`NHitDedx >= 5`)".

### L5 (LOW, document-level) — `acceptance.tex` commented out of the build

1. **Location:** `main.tex` line 90 (`%\input{sections/acceptance}`).
2. **Note claim:** n/a (build wiring).
3. **Baseline fact:** the acceptance chapter exists and was audited by the sibling report (`binning_observables_acceptance.md` §3.4) but is not included in the built note; the introduction's binning-contract paragraph is also disabled (L2), leaving the built note with no release-contract/acceptance statement in the setup chapters.
4. **Severity:** low (document completeness).
5. **Recommendation:** re-enable the acceptance section (and L2's paragraph) before circulation, or state explicitly why acceptance is deferred.

## 5. UNVERIFIED items

Items that could not be verified from in-workspace baseline material (the production workspace `H1ChargedSidis_HighQ2/` is outside this audit's read scope). None of them is a confirmed discrepancy; each needs the stated check before final note sign-off.

- **U1 (attaches to `selection.tex:70-82`):** non-ep veto mechanics — the exact R-threshold rules ("$R<0.1$ for finder 7 and $R<0.5$ for finders 5 or 6"; "two simultaneous finder flags … rejection for events with $R>0.1$") and the quoted fractions (0.29% rejected; 1.61% with R<0.5; 0.06% with R<0.1 retained). The finder set {0,1,5,6,7} and the existence of `HadPtElecPtRatio` gates ARE verified (`docs/data/g1j_ES_port_diff.md:41`; `docs/handoffs/SESSION_HANDOFF_backgrounds_systematics_2026-09-25.md:186`). The note's own provenance comment (`selection.tex:60-61`) points to `H1ChargedSidis_HighQ2/output_v6/studies/bgfinder_ptratio_cutflow_2026-10-06/merged/summary_bgfinder_cutflow.json` (production workspace). **Needed test:** read that JSON plus the Stage-1 CUT 13 source lines; domain: backgrounds (h1-mc-expert for finder semantics).
- **U2 (attaches to `experimental_setup.tex:14-16`):** the claim that 351 pb⁻¹ is "also the luminosity used … for the corresponding low-Q² charged SIDIS analysis". The high-Q² value is verified; the sister analysis's luminosity is not verifiable from this workspace, and the workspace-level AGENTS.md states most active analyses use the 2005–2007 dataset (297,657.8 nb⁻¹), which makes the shared-luminosity claim suspect. **Needed test:** check `sidis_lowQ2/context-handoff.md`; domain: low-Q² SIDIS analysis owner.
- **U3 (attaches to `experimental_setup.tex:9`):** √s = 319 GeV. Standard HERA-II value (Ee = 27.5 GeV, Ep = 920 GeV → √s ≈ 318.4 GeV, conventionally quoted as 319 or 318 GeV); no workspace document states it. Low risk; verify the convention against the H1 publications cited alongside it.
- **U4 (attaches to `monte_carlo.tex:23-24`):** "the trigger weight where applicable". No in-workspace baseline document evidences a trigger weight in the response/data-vector chain (S67 is data-only; the weight documentation covers `weight1*weight2` and `NucliaWeight` only — `docs/unfolding/coherent_prior_fix_results_2026-10-05.md:17-24`). **Needed test:** grep the canonical response/data-vector builders for trigger weighting; if absent, delete the phrase. Domain: builder.
- **U5 (attaches to `selection.tex:31-34`):** the Stage-1 TTree-generation envelope "100 < Q²_eΣ ≤ 20000, 0.05 < y_eΣ < 0.9, 30 < E−p_z < 75". Corroborated by an in-workspace secondary source citing the v6 Stage-1 source with line numbers — "stage-1 reco envelope (electron E/θ, Empz 30-75, Q2>100, y 0.05-0.9, BG-finder and QED-Compton gates) — `TreeBuilderHighQ2_chargedsidis_v6.C:699-832`" (`docs/research/research-diffvm-exclusive-rho-highq2-v6_2026-10-07.md:65`) — and consistent with `y_e < 0.94` (port diff cut 15). The Q² upper bound 20000 and the exact bound conventions (≤ vs <) are not explicitly corroborated. **Needed test (gold standard):** direct read of `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:699-832`. Note the related but distinct fact that the Stage-1 MC gen-envelope OR-rescue is `Q2sGen>60, 0.01<YsGen<0.99, 25<GenEmpz<75` (same doc, `:834-859`) — a possible confusion source the note correctly does not quote.
- **U6 (attaches to H1's replacement wording):** the gen-side Empz convention is genuinely open — the v6 response builder keeps `GenEmpz` 35–70 (`GEN_FID_OPTION_C`, `docs/methodology/v6_production_changes_2026-06-04.md:59`), while the corrected-points chain contract applies `GenEmpz > 45` no upper bound on both legs (`docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md:53,120`), and STATE.md records an open GenEmpz-threshold blocker ("C_rad swings `1.10597`->`1.17160` from no-cut to `>50 GeV`", STATE.md:63). The note's reco-side `E−p_z > 45` (selection.tex:97) IS verified (contract "Empz>45 (no upper)" for the central data vector, bgsub plan:38,92; C_rad `empz_gt45` nominal coherence, RUN_STATUS.md:147-148). The gen-side phrasing added under H1 should be checked against the builder and the open blocker before the note is finalized.

## 6. Verified-consistent claims (no discrepancy)

### `introduction.tex`
- No species PID; only h⁺/h⁻ categories; truth = summed primary-vertex π/K/p (lines 8–12): matches the gen pool `|GenPDG| ∈ {211, 321, 2212}` with prompt/GTR and V0-daughter veto (`docs/methodology/v6_production_changes_2026-06-04.md:61-66`).
- DIS/SIDIS variable definitions (lines 14–34): standard; pT_h and φ_h in the γ*p c.m. frame consistent with the frame conventions used throughout the baseline (e.g. `STATE.md:47`).
- Commented release-contract paragraph content (lines 36–40): matches baseline exactly (see L2).
- TODO list (lines 51–55): accurate — C_rad is provisional-diagnostic and not applied (`c_rad_applied=false`, STATE.md:14); central is raw/no-bgsub with response fake-fraction subtraction only (STATE.md:34).

### `experimental_setup.tex`
- L = 351 pb⁻¹ collected 2003–2007 (lines 14–15): verified — `L_data = 351000.0 nb^-1`, four-period S67 including ep0304 (`docs/status/normalization.md:9-13`; RUN_STATUS.md:126); 356,613.0 nb⁻¹ superseded (normalization.md:18). The note-repo README header (lines 5–8) states this correctly too.
- Flat 2.7% luminosity component, fully correlated, cancels in charge ratios (lines 17–19): verified (`docs/status/current-release.md:29`: "flat `2.7%`, `lumi_rel = 0.027`").
- S67 high-Q² trigger path for data (line 22): verified — S67 for data only is the documented H1 NC DIS policy (`docs/data/g1j_ES_port_diff.md:27,98`).
- LAr electron acceptance and standard non-ep rejection (line 23): verified (FidVolCut_HERA2 and BG finders, port diff cuts 12/17).
- Citations `Kogler:2011zz`, `Britzger:2013` defined (only `2005Hadroo2` broken — L1).

### `selection.tex`
- Preselection (lines 19–38): E_e ≥ 11.0 GeV (port diff cut 5); θ_e < 2.7 rad (cut 6); vertex "optimal NC or (θ_e<30° CJC fallback)", |z_vtx| ≤ 35 cm (cut 8 and run-list row); envelope and y_e < 0.94 (cut 15; U5 for the envelope); bunch-type colliding requirement with non-rejection when unavailable (cut 16: `GetBunchType()==3` data-only with try/catch static-disable on missing bank); good-run list and detector status (cut 1).
- LAr fiducial (lines 40–58): z_impact (−190, 15) ∪ (25, ∞) (cut 10); φ-octant 2°–43° or z_impact > 300 (cut 11); run-dependent FidVolCut_HERA2 veto (cut 12) — the note's description of the veto (run number, azimuthal position at the LAr edge, z_impact, electron energy; removes sub-100% trigger-efficiency regions) matches the FidVolCut_HERA2 mechanism.
- Non-ep finders (lines 70–74): flags {0,1,5,6,7} combining CJC+LAr timing with topology (cut 17; handoff CUT 13); single-flag-as-weak-evidence framing consistent with the ratio-gate design (thresholds U1).
- Anti-QED-Compton veto (lines 83–87): two EM particles, each E > 4 GeV, summed E > 18 GeV, −cos(|Δφ|) > 0.95 (cut 18; handoff CUT 14) — exact match.
- Final event selection (lines 92–101): 150 < Q² < 20000 GeV²; 0.1 < y < 0.7 (`docs/diagnostics/highx_highz_lowpt_onefile_2026-09-22.md:9`; contract line 38); E−p_z > 45 GeV (contract "Empz>45 (no upper)"; U6 caveat is gen-side only); |z_vtx| < 35 cm; same requirements on response reco side and data (coherent-prior report: body legs require full reco selection; `docs/backgrounds/alpha0p2_application_audit_2026-10-06.md:21-22`).
- Track cuts (lines 113–121): pT > 0.15 GeV ("Pt>0.15 with no upper reco Pt cut"); |η| < 1.6; NHitDedx ≥ 5 (see L4 for wording); RadLength ≥ 5 cm; |DCA′|sinθ < 2 cm (contract "|DcaPrime/cosh(Eta)|<2" — identical since sinθ = 1/coshη); IsFromPrimary; x_F < 0 current hemisphere; finite in-binning z/pT_h. Omissions covered by M4.
- Binning (lines 122–125): z edges [0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0] with first bin 0.0–0.05 (current-release.md:19); reco pT_h edges [0.0,0.25,0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0,8.0] — 10 reco bins that pair-sum exactly to the 5 published gen pT bins [0,0.5,1,2,4,8] (`docs/status/alpha0p2-background.md:56`), consistent with the 490-reco/245-gen contract.
- α veto (lines 127–131): α(track, e′) ≥ 0.2 rad, lab frame, reco-only — verified (`docs/status/alpha0p2-background.md:10,17`; `STATE.md:25` lab-frame Grebenyuk precedent); gen-level true π/K/p losses 0.106% (RAPGAP) / 0.114% (DJANGOH) match the validated 0.10649% / 0.11354% (alpha0p2-background.md:18; RUN_STATUS.md:168). Application-point imprecision covered by M4.

### `monte_carlo.tex`
- Same reconstruction chain for data and MC (lines 8–10): verified (v6 trees for both; alpha audit).
- GEANT-based detector simulation (line 15): standard, consistent.
- Production complete; stage-1 = 1525 files (lines 17–18): verified as a total (M1 for the stage-2 attribution).
- Response shape (491,246) per charge, reused for nominal and shifted releases (lines 25–27): verified (current-release.md:20; L3 for the decomposition).
- Finite-z_gen filter and species/source/V0-veto conventions in the builder (lines 31–32): verified (v6 production changes, KEPT list).

### Document level
- `main.tex` abstract (lines 72–75): 351 pb⁻¹ and 490 bins (245+245) verified; "full HERA-II dataset at √s=319 GeV" — the four-period 2003–2007 S67 sample is the full HERA-II high-Q² sample (ep0304 included), consistent; √s value itself U3.
- Note-repo README header (lines 5–8): lumi 351,000.0 nb⁻¹ incl. ep0304, 356,613.0 superseded — correct.
- Release-notes artifact (not the note): `RELEASE_NOTES.md:24` carries a stale "HERA-II 2005-2006-2007" dataset label alongside the correct 351 pb⁻¹ — the note's "2003–2007" is the correct statement; no note change needed, but the release-notes label is a known-stale artifact.

## 7. Coverage and gaps

- **Covered:** all four assigned sections in full; main.tex and note README document-level claims; every task-listed baseline fact was checked against the named baseline docs and, where possible, independently corroborated.
- **Not covered (out of scope):** the other note chapters (audited by the sibling reports in this directory); the production workspace `H1ChargedSidis_HighQ2/` sources, cutflow JSONs, and release `_run` drivers (scope constraint — hence U1, U4, U5 remain open and builder-level claims rely on in-workspace documentation).
- **Coordination points:** H2 must be fixed consistently with the unfolding-chapter findings (`unfolding_backgrounds_radiative.md`); M5 must be fixed together with the binning report's M4 (same README); M2/M3 content overlaps the backgrounds chapter's sample descriptions — the sample-identity subsection (M3) should be written once and referenced.
- **Docs-side action (outside the note, already noted by the sibling report):** the 2026-10-06-vintage baseline docs predate the 2026-10-07 coherent-prior adoption; RUN_STATUS.md and current-release.md consulted here already reflect the 2026-10-07 state, so the note should be drafted against STATE.md 2026-10-07 and must state the prior basis of every released value.

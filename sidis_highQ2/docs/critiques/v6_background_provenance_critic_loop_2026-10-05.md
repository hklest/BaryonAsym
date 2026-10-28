# v6/z005 track-level background provenance: three-reviewer verification and decision record

**Date:** 2026-10-05
**Scope:** inclusive charged-hadron SIDIS, high-Q2 regime (Q2 > 150 GeV^2), central production v6/z005, no species PID, L_data = 351000.0 nb^-1
**Status:** ESTABLISHED WITH STATED LIMITS
**Reviewers:** h1-analysis-critic, h1-qa-specialist, h1-mc-expert, in an independent three-reviewer loop. Package-level verdicts recorded in the corrected manifest: DOCUMENT WITH STATED LIMITS (analysis-critic), PASS WITH LIMITS (qa-specialist), VERIFIED WITH CORRECTIONS (mc-expert).
**Central release:** untouched. The central v6/z005 release and all `output_v6/central/` products are unmodified by this loop and by the actions recorded here; the affected artifacts are the diagnostic plot package, its driver, and this record.

## 1. Question posed

The user challenged whether the track-level background inputs were the superseded v5g-era products carrying the erroneous laboratory-frame variable definitions, and asked for the canonical 7-panel background-ratio-percent plot to be regenerated from v6 tree-built backgrounds. Seven points required settlement: what frame the near-electron cone angle alpha is defined in and where it is computed; whether that definition is intended physics or a v5g leftover; whether z and pT_h in the background scripts use the correct hadronic frame; whether the background aggregates are v6 tree-built; whether the v5g-to-v6 builder transition changed the background-relevant branch logic; whether the remembered laboratory-frame defect ever touched the background chain; and whether any v5g-to-v6 frame rebuild of the backgrounds is required.

## 2. Verdict summary

All three reviewers reached the same verdict on every claim, working independently. In this vocabulary, ESTABLISHED means verified against source and artifacts as stated; ESTABLISHED with limit means verified with a named residual limitation.

| # | Claim | Verdict | Deciding evidence (file, line) |
|---|---|---|---|
| 1 | alpha is the laboratory-frame cone angle between a reconstructed track and the scattered electron, the arccosine of the dot product of the two 3-vectors over the product of their magnitudes; tracks are accepted at alpha >= 0.2 rad; the angle is computed downstream of the builder, not in it, from the branches Px, Py, Pz and scatEX, scatEY, scatEZ | ESTABLISHED | `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py` lines 363-371 (formula), line 113 (`ELECTRON_ALPHA_VETO_RAD = 0.2`), lines 673-681 (branch-based computation and veto); identical code in `H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/scripts/produce_detint_full_alpha0p2.py` (lines 364, 114, 687-695) and in `H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py` line 386 (veto constant; formula at lines 390-396) |
| 2 | The laboratory-frame alpha definition is the intended physics, a detector-geometry near-electron veto following the H1 Grebenyuk precedent, not a v5g leftover | ESTABLISHED, with limit: no analysis document explicitly debates lab versus CM | `sidis_highQ2/docs/research/research-v5g-scat-elec-track-assoc.md` lines 255-260 (the H1OO LAr electron finder matches tracks to the electron cluster under DCA < 12 cm with an azimuthal opening-angle cut; near-electron angular geometry is a standard H1 construction); `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/docs/RUN_STATUS.md` line 34 (Grebenyuk's H1 charged-particle precedent used a minimum track-scattered-electron angle > 0.2 rad). Quantitative support: at alpha below 0.1 rad the removal is 281001 fake-electron against 44900 true-hadron tracks, weighted, in RAPGAP; after the alpha >= 0.2 veto, residual matched electrons remain 4.31 to 4.39 percent of true pi, K and p, so the cone is a partial cleaner, not an electron eliminator |
| 3 | z and pT_h in the same background scripts use the correct frame: a full W = q + P hadronic rest-frame boost, with the CM-frame xF_SIDIS_es < 0 hemisphere selection | ESTABLISHED | producer `produce_reco_background_vector_alpha0p2_v2.py` lines 374-425 (`_boost_to_W_rest` and `compute_pth_cm_and_z`, the W = q + P boost); `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 1876-1887 (xF_SIDIS_es and z_SIDIS_es evaluated in the eSigma-method W rest frame); producer line 668 (`(xF_all < 0.0)` applied to the builder's xF_SIDIS_es branch) |
| 4 | The background aggregates are v6 tree-built, from `H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc` | ESTABLISHED with limit | the input ROOT file carries `v6_envelope_stage2_version` = ChargedSidisTreeBuilderHighQ2_v6 dated 2026-06-05 and `v6_payload_version` = v6-envelope-OR-stage2-2026-06-05, written by the builder at `src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 418-421; the v6-only branch TrackLength is present (builder line 704) and the v5-only branches StartRadius and EndRadius are absent (builder lines 655-656, 702-703); the aggregate inputs consumed by the plot package are the `AGGREGATED_*_L351000_finalcandidate` files under `output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/`. Limit: provenance was verified from one representative input file, per the login-node single-file rules; no file-by-file audit of all inputs was performed |
| 5 | The v5g and v6 builder branch logic for the background-relevant families is unchanged | ESTABLISHED with limit | targeted family-by-family comparison plus the builder header claim, `src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 20-40: the v6sys line was forked verbatim from v5g, and the v6 changes are scoped to the event keep policy, the passRecoEnv gating of the reco track loops, the counters, and the output metadata, with the gen-track loop unchanged. Limit: the comparison was targeted, not an exhaustive full-source diff |
| 6 | The laboratory-frame defect the user remembered belongs to the pT_h and phi_h extraction chain; it was documented CRITICAL on 2026-05-24 and fixed the same day, and it never touched the background chain | ESTABLISHED | `sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md`: the defect lives in `compute_pth_phih_z()` in the Python extraction chain (`extract_phih_moments_v5f.py`, `extract_phi_moments_data.py`); the fix landed on 2026-05-24 in `extract_pertrack_v5g_fixedFrame_v3.py`. The background producer computes its own CM-frame observables (row 3) and does not call the broken helper |
| 7 | No v5g-to-v6 frame rebuild of the backgrounds is required | ESTABLISHED (all three reviewers) | follows from rows 1 through 6: the background chain never consumed the broken laboratory-frame pT_h/phi_h helper, its own frame handling is correct, and its inputs are v6 tree-built |

## 3. Statements the critic loop refuted or corrected

Two statements carried by the first version of the regenerated package and its manifest were refuted; one label was corrected; two framings were qualified. None of the refuted statements is partially correct; each is wrong as stated.

### 3.1 REFUTED: "the integrated background fraction moving from 7.3931 to 7.985771 percent is purely the luminosity rescale"

Refuted independently by h1-qa-specialist and h1-analysis-critic. A pure luminosity rescale cannot produce the observed move. The correct statement is a two-step decomposition. The historical 7.3930735376 percent was a three-category total, electron plus V0 daughter plus low-Q2, over the raw denominator 3865073.0. Adding the detector-interaction category raises the plotted fraction to 8.1134754985 percent at the old luminosity 356613.0 nb^-1, a plus 9.744 percent content step, plus 0.709063 percent absolute. The luminosity correction to 351000.0 nb^-1 then gives 7.9857714104 percent, a minus 1.57395 percent step, through the factor 0.9842602485, which equals 351000.0 over 356613.0. The combined observed factor is 1.080169346, which is why a pure-rescale story fails: the content step and the luminosity step act in opposite directions, and neither alone reproduces the move. Every per-component template is nevertheless a pure rescale, with a maximum deviation of 2.2e-16, so no template physics moved; what changed is the plotted background definition, from three categories to four. The corrected manifest of the regenerated package now carries this two-step explanation in place of the false one.

### 3.2 REFUTED: "the package holds 5 PDFs"

The package holds 6 PDFs: hplus and hminus each in linear and log scales (four files), the 98-panel combined one-pager, and the review PDF, with 8 PNGs. The corrected manifest records `pdf_count` 6 and `png_count` 8.

### 3.3 CORRECTED LABEL: "detector-interaction"

Detector-interaction is properly GEANT/STR charged-hadron secondaries. IsStr is bank provenance, filled from `matched->IsStr()` (builder `src/ChargedSidisTreeBuilderHighQ2_v6.C` line 1735), so the category includes calorimeter re-interactions and decays in flight, not only material interactions. The corrected package caveat states this.

### 3.4 QUALIFIED: the C3 background uncertainty does not "stand" as a settled result

The C3 background uncertainty stands as machinery validated, value provisional. The metadata label is PROVISIONAL_NOT_FOLDED_INTO_RELEASE (`sidis_highQ2/data/systematics/background_c3_2026-10-04/bg_source_c3_metadata_barlowfix.json`), with P-ALPHA, overlap_980, n_raw_980 and DjBkgd provenance open. Its B1 gate passes only by 1.04513e-5 against the one-third limit: the gate requires the pooled template-MC precision to be at most one third of the quoted component systematic in every active bin, and the tightest bin, detector-interaction h-, leaves a margin of 1.0451330859029273e-05 to that limit (`barlow_c3_report_barlowfix.json`, B1_chickenfeed).

### 3.5 QUALIFIED: stage-1 changed between v5g and v6

Stage-1 did change between v5g and v6: the envelope-OR event keep policy (`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` header). Kinematic migrants are neutralized, because the stage-1 reco envelope, Q2 > 100 with y in 0.05 to 0.9 and Empz in 30 to 75, is a superset of the producer fiducial, Q2 > 150 with y in 0.1 to 0.7 and Empz > 45, so no event can fall out of stage 1 for kinematic reasons and still pass the producer. The residual channel is a bounded unknown: generator-envelope-rescued events that fail a stage-1 quality gate can still carry valid in-fiducial Q2es, Yes and Empz and be counted with no data counterpart. The stage-2 builder documents the design-level closure, with reco variables zeroed and both reco track loops skipped for gen-only events so that ChargedTot receives zero rows (builder header lines 20-24), but the quantitative bound requires the stage-1 reco-only, gen-only and both-pass counters, which do not exist yet as extracted artifacts. The generator denominator is irrelevant here, because the background vectors are built solely from ChargedTot (producer line 18, `TREE_NAME = "ChargedTot"`) and never from ChargedTotGen.

## 4. Newly identified holes in the background budget

Every item in this section came out of the MC-semantics review (h1-mc-expert) and appears in no earlier record. Status: OPEN for each item.

### 4.1 The four categories are not exhaustive (OPEN)

Four sources sit outside the electron, V0 daughter, detector-interaction and low-Q2 template categories. First, unmatched tracks, with IsMatched = 0 and MatchTier = 255 (builder lines 532, 543, 751), form the largest fake-column bucket, called other, at 9.27M of 10.58M weighted; the all-fake diagnostics carry it as the `fake_existing_other` column (`output_v6/studies/alpha0p2_allfake_response_z005_20260627/diagnostics/category_fake_vectors_rapgap31.csv`). Second, matched muons, prompt and decay-in-flight, are flagged IsMatchedMuon and IsMatchedDIFMuon (builder lines 780, 773) and have no template category. Third, tier-2 steals: background tracks greedily matched to a generator pi, K or p within CHARGED_MATCH_ANGLE = 2.0 degrees (`src/PhysicsConstants_chargedsidis_highQ2_v6sys.h` line 74) are counted as matched signal and are invisible to all flags. Fourth, on the data side, every classifier is MC truth, so data tracks cannot be tagged and the templates are scaled MC shapes only.

### 4.2 Double-counting conditions against the unchanged central response (OPEN)

Against the unchanged central response, the electron template lies inside the response fake column, so external electron subtraction double counts. V0 daughters are excluded from the response by the reco-side IsMatchedV0Daughter == 0 veto (`build_migration_th2_per_charge_v4.py` line 713), so external V0 subtraction is the response-consistent alignment. Detector-interaction, at roughly 86 percent matched-physical and 14 percent fake, is response-inconsistent under external subtraction. Low-Q2 is source-distinct and always external exactly once. Under an all-fake response, electron, V0 and detector-interaction are all fake-column components, and any external subtraction of them double counts.

### 4.3 DIFFVM exclusive vector-meson production is assumed zero and unassessed (OPEN)

The analysis's own scoping recommended a 0.5 percent systematic in the z >= 0.7 stratum of the 150 to 300 GeV^2 region and 0.05 percent elsewhere, with 32 percent of rho decay daughters at z > 0.7 (`sidis_highQ2/docs/methodology/background_scoping_2026-05-24.md`, sections 3.3 and 4). That recommendation was never applied. For a high-z charged-hadron observable an assumed zero is not defensible.

### 4.4 Low-Q2 leakage provenance (OPEN)

The djangoh14 DjBkgd sample is NC RAD Q2GT2 CTEQ6L. RAD is the correct mode, because leakage into the Q2es > 150 electron-method fiducial is partly radiative, so the real mismatch would be NORAD. Residual risks are the uncovered Q2 < 2 floor, a single-generator model, and unclosed luminosity provenance. The magnitude is 208.4 tracks of 3865073, i.e. 0.0054 percent, so an order-unity mode error moves the central value by less than 0.01 percent and the effect lands in the uncertainty.

### 4.5 The 2013 re-productions are not poolable with the 2010 central samples (OPEN)

The 2013 re-productions, RAPGAP31 ReqIDs 8947 to 8954 and DJANGOH14 8955 to 8962, are not poolable with the 2010 central 70xx samples: different production release, 4.0.25 against 4.0.6, and different tag, A2 against A, R97905 against R97800. The release difference acts on reco-level V0 daughter rates through GEANT decay, reconstruction and matching, and the A to A2 content difference is undocumented. Legitimate uses are generator-only C_rad and a model-variation envelope. The current background templates are unaffected, being verified central 70xx only.

### 4.6 The laboratory-frame cone veto has no geometric protection from the eta fiducial (OPEN)

The |eta_h| < 1.6 fiducial does not protect the cone veto, because the scattered-electron direction always lies within or at the edge of the tracked eta range. The cone clips real current-hemisphere hadrons at high y, backward electron with eta_e >= -1.51 from ELEC_THETA_MAX = 2.7 rad (`src_stage1/PhysicsConstants.h` line 73), and at low y, forward electron at eta_e about +1.4 near Q2 = 150. This is exactly why P-ALPHA matters: double rejection occurs if the cone is applied to the data vector against a non-cone response.

## 5. Actions taken as a result

The canonical ratio-percent package was regenerated against the v6 L351000 finalcandidate aggregates and then text-corrected in place on 2026-10-05 for the two refuted statements, with a REVISION_2026-10-05.md in the package recording the corrections. Package: `sidis_highQ2/plots/backgrounds/canonical_7panel_bg_fraction_v6_L351000_2026-10-04/`, holding 6 PDFs and 8 PNGs, with the corrected manifest generated at 2026-10-04T23:00:09 UTC. Driver: `sidis_highQ2/scripts/backgrounds/canonical_7panel_fraction_v6_2026-10-04/regenerate.py`, corrected in place; its manifest block now carries the two-step explanation, the 6-PDF inventory, the GEANT/STR label, the response-consistency caveats, and the critic_review block with the three reviewers' verdicts. The central release and all output_v6 products are untouched.

## 6. Required follow-ups

Evidence needed to promote ESTABLISHED WITH STATED LIMITS to ESTABLISHED, and to close each open hole:

1. P-ALPHA data-side cone equivalence study: quantify the double rejection when the alpha >= 0.2 cone is applied to the data vector against the unchanged non-cone response, or rebuild a cone-consistent response. Closes 4.6 and is required before the C3 value can leave PROVISIONAL_NOT_FOLDED_INTO_RELEASE.
2. Stage-1 quality-gate counters: extract the reco-only, gen-only and both-pass counters from the v6 stage-1 production. The counters are defined in the stage-1 source cutflow and no extracted artifact exists yet; the extraction bounds the generator-envelope-rescued channel of 3.5 and lifts that limit.
3. Per-bin overlap_980 and n_raw_980 in the aggregate producer: emit the electron-V0 overlap per bin (it exists today only as the integrated scalar 112.2093) and the raw count per bin, so the weight-outlier audit can run and exactly-once accounting can be evaluated per bin. Closes the C3 blocker and enables item 6.
4. DjBkgd per-chunk oomclumi exposure audit: close the exact-template exposure provenance over the resolved HAT lists for the five ReqIDs, including duplicate and overlap accounting, extending the 2026-09-25 fLumi unit resolution. Closes the luminosity-provenance risk in 4.4.
5. DIFFVM scoping turned into an applied component: convert the 2026-05-24 recommendation, 0.5 percent at z >= 0.7 in the 150 to 300 GeV^2 stratum and 0.05 percent elsewhere, into an applied systematic or a validated template. Closes 4.3.
6. Per-component exactly-once ledger: for each of electron, V0 daughter, detector-interaction and low-Q2, record under each response convention (unchanged central, all-fake) whether the component is handled in the fake column, pre-subtracted, or excluded, so that no component is counted zero times or two times. Closes 4.1 and 4.2.

Promotion of the record status additionally requires lifting the two provenance limits: the claim-4 limit, one representative input file checked, for which a full-input header audit is needed, and the claim-5 limit, a targeted family comparison, for which an exhaustive v5g-versus-v6 diff is needed. The claim-2 limit, that no analysis document explicitly debates lab versus CM, is a documentation gap; a methods paragraph stating the rationale closes it.

## Appendix. Claim-to-reviewer mapping

Each verdict-table claim was verified independently by all three reviewers, and the verdicts were unanimous. Attribution beyond that, where the loop recorded it: the pure-rescale statement (3.1) was refuted independently by h1-qa-specialist and h1-analysis-critic; the inventory and label corrections (3.2, 3.3) are recorded in the corrected manifest's critic_review corrections block, without separate per-reviewer attribution; the newly identified holes (4.1 through 4.6) came out of the h1-mc-expert MC-semantics review.

| Claim | Verified by |
|---|---|
| 1 alpha definition, downstream computation | all three reviewers, unanimous |
| 2 intended physics, Grebenyuk precedent | all three reviewers, unanimous |
| 3 z and pT_h frame correctness | all three reviewers, unanimous |
| 4 v6 tree-built provenance | all three reviewers, unanimous; single-file check per login-node rules |
| 5 v5g/v6 branch logic unchanged | all three reviewers, unanimous; targeted family comparison |
| 6 lab-frame defect confined to the pT_h/phi_h chain | all three reviewers, unanimous |
| 7 no frame rebuild required | all three reviewers, unanimous |

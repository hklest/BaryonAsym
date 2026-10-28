# H1 high-Q2 charged SIDIS — local notes (remote analysis lives on NAF)

**START HERE:** `docs/HANDOFF_2026-10-21.md`. It covers the current baseline, the four remaining quoting blockers in order (N2 → M1 → plotters → note), and user-dispositioned items not to re-raise.

This folder holds local documentation and helper scripts only. The analysis itself is on NAF.

## Access
- `ssh klesthen@naf-hone.desy.de` (BatchMode works non-interactively from this machine).
- Docs/state tree: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2`
- Code: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`
- Outputs: `.../H1ChargedSidis_HighQ2/output_v6/` (`central/data_release/` = installed releases and budgets; `studies/` = study products)

## Remote house rules (from the remote `AGENTS.md`; respect them)
- Read remote `STATE.md` first. It is ~200 KB, so read the head ("Last updated" paragraph) and grep the `## ...` override sections; never cat the whole file. The latest override section at the end wins.
- Topical knowledge lives in skills: `sidis_highQ2/.opencode/skills/sidis-hq2-*/SKILL.md` (backgrounds, systematics, unfolding, state, lessons, ...). Load the matching one before answering.
- Login node: interactive one-file checks only, ≤5 min wall. Multi-file work goes through Condor.
- Do not write loose files at the remote top level. This audit made NO remote changes.
- zsh on NAF: quote `"====="` in echo, and avoid `--include=*.md` unquoted.
- Remote `context-handoff.md` (~150 KB) is the remote agent's own handoff log.

## Current state (as of 2026-10-20)
- Nominal central values: OPT-A Q2esGen release `bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_..._2026-10-20/` (promoted 2026-10-20).
- **Quoting is BLOCKED** until budgets and covariance are regenerated on OPT-A. Those include the env100 budget `systematics_bgsub_env100_21comp_2026-10-19/` (21 cols, total `total_with_env100_rel`, medians 7.32% / 7.64%).
- Background convention: Set R' = e_cone_applied + V0_genavg + lowQ2, subtracted pre-unfold. det.int. is removed response-internally via the S2' `f_i`; external det.int. subtraction is forbidden.
- The ±100% on e and V0 is `bgsub_env100_rel` (added 2026-10-19). It is e + V0 only.

## Audits done here
- `docs/detint_uncertainty_audit_2026-10-20.md`: **det.int. carries no rate/normalization uncertainty.** Only generator spread (~0.03% median) and MC stat are booked. The unpriced removed content is ~0.7% integrated, ~0.3% per-bin median, max ~2.6%. Includes a recommended `detint_norm_rel` construction and open items (C1 double count, NucliaWeight convention mismatch).
- `scripts/detint_fc3_share.py`: reproduces the per-bin det.int. share. Run with `ssh klesthen@naf-hone.desy.de 'python3 -' < scripts/detint_fc3_share.py`.
- `docs/track_truth_classification_study_2026-10-20.md`: **which MC reco tracks are signal.** It covers the H1 GTR/STR/DTRY/SVX conventions (verified in H1OO source), how the S2' builder routes every track, and a census.
  - The det.int. label misses GTR-linked "absorbed secondaries" (~1.0–1.1% of body), split-track duplicates (~0.3%), Tier-2 matches (0.1%) and keeper K interactions (0.05%). In total that is ~1.5% of body, 2× the S2' det.int. reroute, and 8.7% at pT 0.15–0.2 GeV.
  - Proposed signal predicate: Tier-0 & GTR & π/K/p & not e/V0 & charge agree & 0.7<P/GenP<1.5 & angle<10° & dedupe.
  - Scripts are in `scripts/track_class_study/` (run each with `ssh ... "FILE=<root> python3 -" < script`, one file per call).
  - MC trees: `H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/{djangoh14,rapgap31}/<ReqID>/*.root`, tree `ChargedTot`.

- **INSTALLED 2026-10-21 (user-authorized baselining): non-primary v5.**
  - Quoted budget: NAF `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_optA_2026-10-21/`; `total_quoted_with_nonprimary_v5_rel` medians 7.4475% / 7.7992%.
  - Covariance edition: `H1ChargedSidis_HighQ2/data/covariance/z005_covariance_optA_nonprimary_v5_2026-10-21/` (edition of the OPT-A package another process built 2026-10-21; C_total with lumi 7.3655% / 7.5999%; gates g0–g7 + ga10 PASS).
  - Install record: `INSTALL_RECORD_nonprimary_v5_2026-10-21.json`. Promotion record: NAF `sidis_highQ2/docs/promotions/nonprimary_v5_promotion_2026-10-21.md` (local copy `docs/`).
  - Canonical edits: STATE.md, RUN_STATUS, current-release, context-handoff, RUNLIST ×2, script index, 8 skills, ownership/coverage. Applied via `s11_promote_nonprimary_v5_2026-10-21.py`, sha-guarded; pre-edit copies in `docs/promotions/nonprimary_v5_2026-10-21_preedit/`.
  - Records on NAF: `sidis_highQ2/docs/systematics/nonprimary_v5_2026-10-21/`. Post-install critic pass done (see item 6).
  - CONCURRENCY: a peer session (h1-sidis-e5) and an OpenCode server also write to this analysis. Always re-read canonical files right before editing.
- `docs/nonprimary_v5_baseline_candidate_2026-10-21.md`: (v5 candidate record, now installed). It passed 3 critic rounds (round 3: all SOUND-WITH-CONDITIONS, no blockers; conditions closed).
  - Hyperon feed-down is tied to the env100 V0 nuisance: `bgsub_env100_hyp_rel` replaces env100.
  - STR interaction-born: κ=±100% fiat. STR μ: re-sized (DJ–RG f_μ, noise-level).
  - Totals: total_quoted median 7.31→7.45% (h+) / 7.62→7.80% (h−).
  - h+/h−: background terms ±0.58% vs env100-only ±0.27%; A_h ±0.0029.
  - Install ONLY together with the OPT-A covariance re-basing (env100 rank-1 block → env100_hyp).
  - Package: NAF `output_v6/studies/trackclass_variant_2026-10-20/nonprimary_norm_term_v5/`; local copy `data/nonprimary_norm_term_v5/`; plots `plots/nonprimary_norm_v5_overlay_2026-10-21/`.
- `docs/trackclass_v3_provenance_2026-10-21.md`: (v3 definition ruling still holds: provenance-only background; its term is superseded by v5).
  - Background = STR-only truth links (det.int. + K± keeper). Every GTR-linked track is unfolded: no kinematic cuts, no dedupe.
  - Impact: −0.033% / −0.028%.
  - `nonprimary_norm_rel` v3 (κ=1): coherent ±0.93% / ±0.82%; per-bin max 2.6% / 3.4%; never the largest component; total_quoted p95 increment +0.12 / +0.07 pp.
  - Overlay plots: `plots/nonprimary_norm_v3_overlay_2026-10-21/` (NAF `sidis_highQ2/plots/systematics/...`).
- `docs/trackclass_triplecheck_2026-10-21.md`: (v2, superseded by v3; its validation evidence still stands). Triple-check of the background classes using independent tests: matched DCA′ fit, curvature pull, and physical constraints. Real tracks were found in v1 (Tier-2, P/GenP>1.5, part of the large-angle class).
  - v2 removes only clear background: det.int., P/GenP<0.7 and the keeper. Duplicates are a separate column.
  - Impact: h+ −0.526% ± 0.005% (MC), h− +0.057%.
  - `nonprimary_norm_rel` v2 (κ=1): coherent ±2.07% / ±1.39%, per-bin median 0.44% / 0.38%; duplicates ±0.33% / ±0.40%. total_quoted median increment +0.01 pp.
  - Package: NAF `output_v6/studies/trackclass_variant_2026-10-20/nonprimary_norm_term_v2/`; local copy `data/nonprimary_norm_term_v2/`.
- **v1 results below are WITHDRAWN (2026-10-21 triple-check: real tracks were in the v1 background classes); see `docs/trackclass_triplecheck_2026-10-21.md` for v2.**
- `docs/trackclass_impact_2026-10-20.md`: **full-chain impact (DIAGNOSTIC, v1, withdrawn)** of the reclassification through a variant OPT-A response (Condor 5803732, 613 jobs) and the lean IBU k=4 re-unfold (validated to 1e-15 vs the official result; null test bit-identical).
  - Impact: integrated h+ −0.559% ± 0.005% (MC) ± 0.001% (data), h− +0.095%; per-bin median −0.25%. The window choice is irrelevant (±0.004%).
  - Rate-shift systematic, all non-primary at ±100%: h+ ∓2.6%, h− ∓2.1% integrated, ~1.1–1.2% per-bin median; +0.13–0.17 pp on the median total. It is linear in κ.
  - Code: `scripts/trackclass_variant/` (local copies) = NAF `H1ChargedSidis_HighQ2/scripts/unfolding/trackclass_variant_2026-10-20/`. Outputs: NAF `output_v6/studies/trackclass_variant_2026-10-20/{shards,impact}/`, with local copies in `data/trackclass_impact/`.
  - Condor gotcha: `queue ... from` splits on commas. Use `+` in args, and run login-node commands under `bash -c` (zsh doesn't word-split).

- `docs/nonprimary_norm_term_2026-10-20.md`: **the extra uncertainty `nonprimary_norm_rel`**, a DIAGNOSTIC extension of the OPT-A c1fix budget (κ = 1, fully correlated).
  - Coherent ±2.83% (h+) / ±2.19% (h−); per-bin median 1.2% / 1.1%, about 3% at z6 and up to 47% in sparse S3 z6 bins.
  - total_quoted median 7.31 → 7.49% / 7.62 → 7.75%.
  - Package: NAF `output_v6/studies/trackclass_variant_2026-10-20/nonprimary_norm_term/`; local copy `data/nonprimary_norm_term/`.

## Pending user decisions / next steps
1. Choose κ for a det.int. normalization term: ±100% like env100, or a class-split justification.
2. If approved, implement it as an fc3-scaled `f_i` variant through IBU k=4 on the OPT-A nominal. This would be part of the OPT-A budget regeneration and runs via Condor on NAF.
3. Verify the NucliaWeight convention of the installed response (S2' spec vs fitnorm spec disagree).
4. Track-class study decisions: adopt the signal predicate and its window; remove the keeper; treat Tier-2 as fake. Then run a full-sample Condor census, and build the response variant **with miss-row fill for GTR-linked reroutes** (see Sec. 6 of the study). The normalization uncertainty must then cover ~2.2% of body, not 0.65%.
5. (History) `nonprimary_norm_rel` v3 was **NOT baselinable as is**; see `docs/nonprimary_norm_v3_critic_review_2026-10-21.md` (3 critics). Arithmetic and statistics are verified.
   - Must fix: C1 keeper consistency (drop fc5/fc8 vs promote the keeper reroute); C2 source-split nuisances (interaction / hyperon / keeper) with charge correlation, since h+/h− goes 0.11% at ρ=1 vs 1.21% at ρ=0; C3 κ policy (derived vs fiat); C4 STR-only μ in fc0.
   - Plus: C5 efficiency correlation, C6 NucliaWeight on secondaries, C10 provenance rebuild, C11 Born check, C12 inherited plot-label bug (also in the official 2026-10-19 plots). Decide whether the reclassification is adopted as the nominal or only enters as a systematic. Open checks: isolate the charge-flip origin of the h+/h− asymmetry (−0.65%), Born-level propagation, covariance.

## Low-Q2 takeover (2026-10-20)
- Authority: NAF `sidis_lowQ2/STATE.md`. The old `STATUS.md` is historical (v5g). The new workspace is `H1ChargedSidis_LowQ2/`, which mirrors the high-Q2 layout.
- Plan: `docs/lowQ2_takeover_plan_2026-10-20.md` (copy at NAF `sidis_lowQ2/docs/plans/plan-lowQ2-parity-2026-10-20.md`).
- The v6isr MC trees are reco-gated, so a new Stage 1 with envelope-OR is needed before any response work.

6. v5 INSTALLED and post-install audited 2026-10-21: implementation PASS; canonical text PASS-WITH-CONDITIONS, conditions closed by `s12_fix_promotion_text_2026-10-21.py`. Quoting still blocked by the OPT-A conditions (M1, N2, plotter re-pointing, note). Neither the plot-label bug nor the NucliaWeight gap is a quoting blocker.
   - Plot labels FIXED 2026-10-21: additive edition NAF `plots/systematics/bgsub_env100_quoted_total_21comp_7col_fixedy_labelfix_2026-10-21/` (binmap labels; data pixel-identical); STATE/RUN_STATUS/plotting skill caveats updated.
   - NucliaWeight primary-loss term BUILT as a DIAGNOSTIC candidate, not installed. Record: `docs/nucliaweight_primaryloss_candidate_2026-10-21.md` (NAF `docs/systematics/nucliaweight_primaryloss_2026-10-21/`).
     - Method: κ=±1 on (1−NW), "loss-to-miss" form, Condor 5815153/5815199.
     - Integrated ±1.18%; per-bin median 0.76%/0.78%; total median 7.4475→7.4848% / 7.7992→7.8604%; cancels in h+/h− and A_h.
     - Package: NAF `output_v6/studies/nucliaweight_variant_2026-10-21/nucliaweight_term/` (local `data/nucliaweight_term/`); plots `plots/nucliaweight_primaryloss_overlay_2026-10-21/`.
     - **Central-value finding:** the nominal books no NW deficit in the miss row, so efficiency = nC/(nC+M). Fixing this moves the OPT-A nominal +0.453% (both charges); the 2026-09-09 memo ruled this form wrong. Not ours to fix; reported to the user.
     - Degeneracy check: the fake-leg piece partially overlaps bgsub_fakefrac; possible overlap with tracking_eff. The user kept both (conservative).
     - Critics: implementation PASS-WITH-CONDITIONS, physics SOUND-WITH-CONDITIONS (reviews in NAF `docs/systematics/nucliaweight_primaryloss_2026-10-21/`).
     - User decisions: κ=±100% ratified; the +0.453% bias is NOT logged in STATE (negligible); per-charge results are covered by the charge split, and h+/h−/A_h is zero by construction (caveat). Proton bound 0.03% integrated.
     - Covariance CANDIDATE edition: NAF `output_v6/studies/nucliaweight_variant_2026-10-21/covariance_candidate/z005_covariance_optA_nonprimary_v5_nw_2026-10-21/`; gates 9/9 PASS; C_total medians 7.4016% / 7.6525%.
     - **INSTALLED 2026-10-21 (user sign-off):** CURRENT QUOTED BUDGET = NAF `output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_2026-10-21/`.
       - `total_quoted_with_nonprimary_v5_nw_rel` medians 7.4848% / 7.8604%.
       - Covariance edition `data/covariance/z005_covariance_optA_nonprimary_v5_nw_2026-10-21/` (C_total with lumi 7.4016% / 7.6525%; 9/9 gates).
       - Install record `INSTALL_RECORD_nucliaweight_2026-10-21.json`; promotion record NAF `docs/promotions/nucliaweight_promotion_2026-10-21.md` (local copy `docs/`).
       - Applied with `s13_install` (Condor 5816023) and `s14_promote`: 15 sha-guarded canonical edits; pre-edit copies in `docs/promotions/nucliaweight_2026-10-21_preedit/`.
       - Quoting is still blocked (M1, N2, plotter re-pointing, note).

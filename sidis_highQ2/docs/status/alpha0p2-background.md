# Alpha0p2 Background and Response Diagnostics — High-Q² Charged-Particle SIDIS

**Status authority:** compact current detail for the alpha0p2 near-electron/reco-background/all-fake response diagnostic chain.  
**Primary handoff:** `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md`.

## Status

All alpha0p2 reco-background and all-fake response products are diagnostic/provisional. Central z005 release and central response products were not overwritten. No alpha cut, raw MC subtraction, fake subtraction, or all-fake response treatment is promoted as central until response-consistent closure, fake/background covariance, and nominal-method decisions are complete. [2026-10-14 annotation: the 2026-10-13 cone-baseline promotion now imposes the `alpha >= 0.2 rad` lab-frame reco selection as the CENTRAL baseline (see the cone paragraph below), so the "no alpha cut" clause no longer covers the reco selection convention; the DIAGNOSTIC alpha0p2 subtraction treatments named here — raw MC subtraction, external fake subtraction, all-fake response treatment — remain unpromoted.]

**S2' promotion update (user decision 2026-10-10):** the CENTRAL chain now removes the det.int. removable class RESPONSE-INTERNALLY (S2' detint-fake variant; canonical RAW release `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`). The alpha0p2 diagnostic products listed in this file remain diagnostic/provisional and untouched. External det.int. pre-subtraction on the central chain is now FORBIDDEN (double removal + keeper violation; see Response-Consistency Rules below and `docs/promotions/detints2prime_promotion_2026-10-10.md`).

**Cone-baseline promotion update (user declaration 2026-10-14; triad installed 2026-10-13):** the `alpha >= 0.2 rad` LABORATORY-FRAME scattered-electron cone is now BASELINE on the RECO side of the central chain (canonical RAW release `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/`; inserted AFTER the e/V0 truth vetoes, BEFORE the z-guard / matched-fake classification / det.int. reroute; gen side unchanged; cone-rejected matched legs flow to the miss row via the ordinary consumed-key machinery). The cone data vector `raw_data_no_bgsub_alphaveto_cone` (`electron_alpha_veto_rad 0.2`; in-grid `3,865,073`; removal `19,024` = `-0.4897%` vs the no-cone central vector) is per-bin IDENTICAL to the alpha0p2 vector, 980/980 bins EXACT — the central chain now embeds exactly the near-electron removal the alpha0p2 diagnostic chain measured. External cone/near-electron subtraction on the cone chain is FORBIDDEN (DOUBLE REMOVAL — the cone is applied inside the response/data-vector construction), and no-cone-rescaled electron/V0 templates must not be applied on the cone basis (the packaged CONE-APPLIED templates are consumed AS-IS, cone spec Sec 3(v)). The det.int. NEGATED rule below carries to the cone basis unchanged. The alpha0p2 diagnostic products listed in this file remain diagnostic/provisional and untouched. Promotion record: `docs/promotions/alphaveto_cone_promotion_2026-10-13.md`.

**Clean-release promotion update (user decision 2026-10-16; installed 2026-10-17):** the central chain now ADDS the pre-unfold EXTERNAL background subtraction Set R' = e_cone_applied + V0_genavg + lowQ2 on top of the cone-basis S2' chain (canonical RAW release `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/`; B_R' `281250.09` = `7.2767%` of D_cone; component shares e `3.8543%` + V0 `3.4166%` + lowQ2 `0.0054%`; single denominator = official_reunfold IBU `k=4` on data_sub'). The packaged CONE-APPLIED templates were consumed AS-IS (cone spec Sec 3(v)); no-cone paths were never input; EXACTLY-ONCE verified vs the response-internal S2' `f_i` det.int. removal (external e/V0/lowQ2 once; det.int. removable class response-internally once); the e∩V0 overlap stays IGNORED per user decision 2026-10-08 (`112.2093` cone-applied tracks = `0.0029%` of D_cone, systematic-only); register rows 18/19 (DIFFVM / photoproduction) stay BLOCKED (never subtracted; systematic-only). The external det.int. pre-subtraction FORBIDDEN rule is UNCHANGED — the det.int. removable class stays response-internal via `f_i`; Set R' subtracts e/V0/lowQ2 ONLY. The alpha0p2 diagnostic products listed in this file remain diagnostic/provisional and untouched. Promotion record: `docs/promotions/bgsub_cleanrelease_promotion_2026-10-17.md`. P-ALPHA (alpha-veto/background response-consistency validation) is CLOSED by this promotion — the gated chain is now the installed central basis, gate-complete.

The `alpha < 0.2` scattered-electron cone cut is reco-only and must not be imposed in gen-only `C_rad`. [2026-10-14 annotation: UNCHANGED by the cone-baseline promotion — the cone stays RECO-ONLY; the gen side of the chain and the gen-only `C_rad` tables are untouched, so the reco-only cone must still never be applied to gen-only `C_rad`.]

## Near-Electron Study Conclusions

- The apparent `80%` removal was a misread of normalized/control/composition plots.
- Validated data track-level `alpha<0.1` removal max is about `36.6%` in global bin 24; next-largest bins are about `33.7%`, `33.7%`, and `30.7%`.
- Worst-bin MC removed population for `alpha<0.1` is overwhelmingly true `e±` (`96–99%`) with about `0.5–3%` true `π/K/p`; data removal fractions exceed MC by factors about `1.8–2.6` in hot bins.
- Grebenyuk's H1 charged-particle precedent used track–scattered-electron angle `>0.2 rad`; momentum matching is not the H1-standard-looking solution.
- For `alpha<0.2`, integrated MC true `π/K/p` loss is `0.10649%` RAPGAP and `0.11354%` DJANGOH; matched `e±` removal is `5.07632%` RAPGAP and `5.26673%` DJANGOH.
- Residual true `e±` after `alpha<0.2`, relative to true `π/K/p` after veto, is RAPGAP `2729276/63340834 = 0.0430887` and DJANGOH `3319290/75590840 = 0.0439113`.

## Current Diagnostic Roots

| Item | Path / value |
|---|---|
| Reco-background fraction study root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/` |
| Canonical 7-column plot script | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/scripts/regen_minimal_7col_plots.py` |
| Local canonical plot copy | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_2026-06-25/background_fractions_vs_data_7col/` |
| Uniform L351000 package | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/` |
| Overlap diagnostics | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/alpha0p2_overlap_exclusivity_20260626/` |
| det.int response partition | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/detint_response_partition_20260626/` |
| electron/V0 response partition | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/electron_v0_response_partition_20260627/` |
| All-fake response root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/` |
| All-fake response builder | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/alpha0p2_allfake_response_z005_20260627/scripts/build_migration_th2_per_charge_allfake.py` |
| Local compact all-fake summaries | `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/data/response_diagnostics/alpha0p2_allfake_response_z005_2026-06-27/` |

## Validated Compact Numbers

Use `L_data=351000.0 nb^-1`. Uniform L351000 totals are electron avg `148976.4352`, V0 avg `132065.2768`, lowQ2 `208.3795`, det.int avg `27405.8031`, total `308655.8946`, denominator `3865073.0`, total fraction `0.0798577141`.

Electron∩V0 scaled average overlap is `112.2093` (`0.0753%` of electron, `0.0850%` of V0, `0.0364%` of full total); det.int overlaps electron/V0 are zero, and lowQ2 is source-distinct.

The det.int response partition cluster `3971447` found about `86%` matched physical block and `14%` fake, so blind pre-subtraction with unchanged response is not response-consistent. The electron/V0 partition cluster `3973257` found non-V0 electrons `100%` fake column in alpha0p2, V0 daughters `100%` excluded/dropped, and central v6 excludes both from physical matched signal response.

All-fake response artifacts include `responses/migration_inputs_allfake_alpha0p2_z005.root`, `responses/allfake_response_matrices_and_categories.npz`, `validation/validation_summary.json`, `diagnostics/category_fake_vectors_rapgap31.{json,csv}`, and `diagnostics/category_fake_vectors_djangoh14.{json,csv}`. Cluster `3987932` completed `8/8`; response shape is `(491,246)` per charge; RAPGAP/DJANGOH input files are `364/501`; duplicate guardrail PASS; no merged mixing.

## Response-Consistency Rules

- With the current unchanged alpha0p2 response, V0 can be pre-subtracted/vetoed because it is excluded.
- With the current unchanged alpha0p2 response, non-V0 electrons are fake-column handled and must not also be pre-subtracted if fake subtraction is active.
- NEGATED (2026-10-10 S2' promotion; ledger item 10): under the promoted CENTRAL basis the det.int. removable class is removed RESPONSE-INTERNALLY via the enlarged per-reco-bin `f_i` (removable fraction `0.909920 +- 0.000269`), so any external det.int. pre-subtraction would DOUBLE-REMOVE the class AND violate the k_parented keeper (charged-K DIF kept as signal): FORBIDDEN (spec `docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md` Sec 3: "stays forbidden, more strongly"). Historical pre-S2' wording (unchanged alpha0p2 diagnostic response): det.int should not be blindly pre-subtracted because it is mostly matched response. [2026-10-14 annotation: this prohibition CARRIES to the cone basis unchanged — the cone chain inherits the S2' response-internal removal via the enlarged per-reco-bin `f_i` (cone-basis pooled removable fraction `0.909618`, re-validated in `[0.907782, 0.912058]`); external det.int. pre-subtraction on the cone chain remains FORBIDDEN.]
- With the all-fake variant, electron/V0/det.int are all fake-column components; the data vector must not additionally subtract them externally, and fake subtraction/covariance must handle them exactly once.
- Describe det.int as STR-only GEANT secondary charged `π/K/p` excluding electrons/V0, not pure material interactions.

## Plot/Script Style Trap

For the canonical `alpha<0.2` reco-background fraction 7-column display, use raw alpha0p2 reco data as denominator, z edges `[0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]`, first bin `0.0-0.05`, and pair-summed pT_h bins `[[0.0,0.5],[0.5,1.0],[1.0,2.0],[2.0,4.0],[4.0,8.0]]`. Treat `zlow005` in paths as a tag/name only. In pandas plotting scripts, use `df['sample']`, not `df.sample`.

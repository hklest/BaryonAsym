# FINAL production plan: eΣ release with eΣ x_F, eΣ-native budget, freeze (2026-10-24)

Roots: `HQ` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`, `SH` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2`,
`U` = `HQ/scripts/unfolding`, `F` = `U/esigma_frame_2026-10-22` (current scripts), `ST` = `HQ/output_v6/studies/esigma_frame_2026-10-22`,
`FN` = `U/esigma_final_<date>` and `STN` = `HQ/output_v6/studies/esigma_final_<date>` (new campaign roots, D22),
`gA/gB/gC` = `U/final_rerelease_2026-10-22/budget_group{A,B,C}/components_group*.py`, `DOC` = `SH/plots/docfigs/esigmaphoton_2026-10-24`,
`NOTE`/`PAPER`/`LETTER` = `SH/analysis_note/{H1-ChargedSIDIS-AnalysisNote-HighQ, H1-ChargedSIDIS-Paper-HighQ, H1-HighQ2-SIDIS-Letter}`.
Current release (superseded, kept byte-unchanged): `HQ/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphoton_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-23`.

**Status: READY (2026-10-24).** User answers to U1-U4, verbatim: "U1: x_F should be in the gamma+p frame, not the lab frame! This is
actually very important. U2: Yes, use the true photon for all of them. Also, if we subtract every photon from the beam energy it is
indeed wrong. Apply the correction. I agree, b with correction. u3: a. U4: yes, a. Implement those. Final: You have my sign off to
implement and smoke test them."
**Authorization scope:** implement the code changes (S1) and run the smoke stage (S2) only. The production waves (S3 onward) need a
further user go-ahead after the smoke results; install (S7) needs its own sign-off. The plan was approved on condition that this is
the final production run, then everything is frozen.

## User rulings during production (2026-10-25)

- **Benign pT_h-edge mismatches, option A:** "We don't care about such small differences." The benign rule's file scope covers djangoh14 as well as rapgap31, with every per-row check unchanged (FN/release/opta_benign_mismatch_rule.py; w1 rows: DJ 0002 K+, RG 0008, RG 0069, all at the 0.5 GeV generator pT_h sub-bin edge; pin STN/s3/benign_mismatch/).
- **truth_parity retired as an install gate:** "we don't need truth parity" (it fails on the 2026-10-22 campaign too; the response MC and the C_rad MC are different productions). The checks are born_shift and the per-bin co-movement of the C_rad radiative leg with the response truth.
- **Same MC for C_rad:** "we should run over the same MC as we had previously for our nominal c_rad calculation." Proven: STN/s3/crad_sameMC/crad_same_mc_proof.json (input file/leg lists sha-identical to the 2026-10-21 queues, 2953 DJANGOH + 1777 RAPGAP; no input modified since; exposures and f(e+p) identical).

- **Old-vs-new gates are diagnostics** (user, verbatim): "Remove any blockers related to the two being the same, unless they should be byte identical a priori." Reclassification:

| Gate | Old status | New status |
|---|---|---|
| born_shift per-bin and z-row limits (S4) | blocker | diagnostic (`old_vs_new_diagnostic`); null test 9e-16 and finiteness stay hard |
| RU-2 window centred on 2026-10-20 | blocker | re-centred on this campaign (7.3155% / 6.9910%, same +/-0.12 pp construction; `STN/s3/ru2_rederivation/`), hard for reruns; old centres informational |
| f_i vs 2026-10-11 central-cone crosscheck signature | disabled diagnostic | diagnostic; new hard internal check: f_i recomputed from the merged response == consumed sidecar (max diff 0.0) |
| truth_parity (S4) | blocker | retired (user) |
| benign-mismatch rule scope rapgap31 only | blocker | both generators, per-generator count-matching, per-row checks unchanged (option A) |
| detint_integrated_sign vs 2026-10-23 (S4) | blocker | diagnostic; hard: finite and nonzero |
| detint_native_median band 0.1-0.4% (S4) | blocker | diagnostic; hard: 245 finite values per charge |
| ssp_gate f within 0.02 of 0.910 (S4) | blocker | diagnostic; SSP formula identity, f == n_num/n_den, groupA == SSP stay hard |
| gen phi_h G1-R R2, R6 | blocker | diagnostics (selection follows the true gamma*: no ES migration loss, no ISR one-sided y bias) |
| gen phi_h G1-N flag_consistency, G1-R R5 (stored old-definition flag) | blocker | hard check redefined against an independent true-gamma* recomputation; stored-flag mismatch is a diagnostic |
| gen phi_h G4 anchored to old CRAD_FINAL | blocker | hard, anchored to this campaign's C_rad; old anchor diagnostic |
| born_integral_delta, crad_nonrad_identity, term_delta_table | report only | unchanged |

- **Born shift accepted** (user): "I'm not surprised about the born shift. We should accept it, those bins will have large uncertainty anyway. It will be covered." Diagnostic STN/s4/born_shift_vs_ibu_model.json (3 bins with shift > total: h+ 193, 222, 234).
- **Tight-DCA (user "1A"):** kappa stays +/-100%; the two exceedance bins of the 1000-toy run (h+ 176 at 114%, h- 129 at 104% of the bgsub_env100_hyp envelope) are quoted in the paper; the 100-toy outlier (h- 234, 469%) is low-statistics and superseded (the release covariance uses 1000 toys; D24's "100-toy median" was wrong).
- **TUnfold (user "2A"):** v4/v5 dropped (they need data-side shifted detector vectors that the eSigma campaign does not build); the TUnfold cross-check covers the nominal result only, and the documents say so.

Kept hard: null tests, same-MC file lists, data-vector identity where no decided change applies, non-radiative bitwise checks outside U1/U4, fork md5 pins, benign per-row checks, closure, covariance positive-definiteness and gates g0-g7/ga10, file counts and exit codes. Pre-install cross-check: h1-mc-expert estimates the z-row Born migration and the RU-2 drop from generator-level MC; disagreement is a stop.

## Resolved questions (user, 2026-10-24)

- **U1 = (a):** x_F = 2 p*·P̂*/W along the proton direction in the γ*p frame (P̂* = −q̂*), cut x_F < 0 (current hemisphere), at reco
  and gen level in every builder, fork, producer and radiative worker; replaces the lab-z convention (D13).
- **U2 = (b) with the ISR-only correction:** E_eff subtracts only initial-state photons (D0's original formula
  E_eff = BeamElecE − max(GenEnPhoton, 0) is superseded); the true γ* defines the generator-level Q², y, x, the fiducial cuts, the Q²
  cell and x_F. Extended by the user (verbatim): "It should be for z and pt as well I believe." Generator z and p_T therefore also use
  the true γ* (ISR-only E_eff), i.e. option (c); this supersedes the earlier "gen z and pT keep their current photon".
- **U3 = (a):** DIFFVM elastic ρ⁰ ±100% as a rank-one term, and the low-Q² ±30% normalisation as its own rank-one ρ = +1 term; signed
  off by the user as new quoted terms (26 quoted sources).
- **U4 = (a):** the true hadron (generator PDG) mass in generator p_T.

## Goal

- One final production of the eΣ release: x_F on the eΣ photon at reco level and on the true γ* at generator level; every budget
  term, sign, scalar, table and correction factor built from inputs of this campaign, with each retained item shown to be
  frame-independent (Readiness audit); gates, install, promotion; all figures and document numbers regenerated; then a freeze.
- Non-goals: tree (C++) rebuild; low-Q² or V0 work; reopening binding decisions (Scope Notes); the swap-one table is removed.
- Expected change size (code): about 16 builder/fork scripts, one helper, 2 radiative workers, groupA/B/C, downstream and s08/s09,
  about 8 release-script copies, 3 new small steps (closure, package assembly, freeze), about 20 producers re-pointed, 3 document repos.

### Expected size of the physics changes

| Quantity | Expected change | Evidence |
|---|---|---|
| Data vector | U1(b): bitwise identical (0 of 3.88M tracks); U1(a): changes near x_F = 0, mostly z < 0.05, not estimated | two-way `summary.json` |
| MC reco side | DJANGOH 0 of 76.7M, RAPGAP 4 of 64.3M with the PDG mass; the D12 pion-mass switch adds an unestimated change (S1 `mass_effect`) | two-way `README.md:46` |
| MC gen truth, z ∈ [0,0.05) (35 of 245 bins per charge) | +0.6-0.7% from x_F (measured with a nearby photon); U2(b) moves radiative events between Q², x cells | `samples/<gen>/gen_all`; S2 |
| Raw unfolded σ and C_rad, same row | up and down by about the same; Born nearly unchanged | h1-mc-expert |
| C_γBorn | F_EWγ rebuilt (old x_F, η/J/ψ veto, 0.514 mix removed); below 0.5% | estimate; D14 |
| Integrated Born 12.02/11.69 nb, no-Z 10.96/10.65 nb | below 0.3% | estimate |
| detint_removal | median about 0.2%, model part about 0.03% | h1-systematics |
| Rank-one signs (ibu_model, ibu_reg, detint, c_rad_gen) | some bins flip; ratio and A_h errors may move noticeably | S6 reports |
| Total quoted median 8.34% / 8.89% | below 0.1 pp, plus the U3 terms | estimate |

## Approach

1. One tested helper (`F/esigma_xf.py`, copied into `FN`) computes reco x_F, the gen true γ* and gen x_F for every builder, fork
   and radiative worker; trees hold every input (no tree-level x_F preselection: tree builder `:1581-1599`; stage-1 "No xF cut",
   `HQ/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:42`). Rejected: tree rebuild; per-script formulas.
2. One campaign under a new root (`FN`, `STN`) reruns every consumer of data or MC; a lineage gate refuses any input outside it
   except a short allowlist. Rejected: partial reruns.
3. Every sign and scalar comes from this campaign or is in the audit with frame-independence evidence.
4. Release scripts are copied and edited under the new tag. Producers are re-pointed, not rewritten. A freeze stage pins everything.

## Decisions

- **D0 Generator photon** (user: "Indeed, use A", true γ*): q_gen = k_eff − k′_gen with E_eff = E0 − E_γ for ISR rows and E0
  otherwise (correction of the original formula, FYI above). ISR tag, one rule in every consumer (builders, C_rad and no-Z
  workers): on each GenEnPhoton > 0 row, ISR if Q2_es_gen is nearer to Q2sGen than the nominal-beam Q² is, else FSR, with no
  threshold (where both are equally near, Eg/E0 is below noise and the choice does not matter); on ChargedTot the nominal-beam Q²
  comes from GenElecE, GenElecTheta and BeamElecE. Recovered-energy rows (tree builder `:1260-1263`) are counted. GenRad
  (`HQ/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v3.C:85`) serves only to validate the tag in S1.
  Used for x_F, Q², y, x, the fiducial, the cell, and gen z and p_T (U2 extended by the user; supersedes
  `.supervisor/memory/decisions/2026-10-24-xf-gen-true-gamma-star-option-a.md` on z and p_T). Builders recompute gen z and p_T in
  python instead of reading stored `zGen`/`pTh_cm_gen`; the generator φ_h appendix pages follow too (D21).
- **D1 Tag** `esigmaphotonxf_<run date>` for package, release, budget and covariance edition; s15 refuses existing destinations
  (`U/esigma_release_2026-10-23/s15_install_esigmaphoton_2026-10-23.py:57-60`).
- **D2 NaN x_F fails the cut** (`isfinite(xF) & xF < 0`), reco and gen alike, also in the radiative workers (which return −999
  today); counted in the gate records. Existing convention (`F/esigma_photon.py:27-31`).
- **D3 Sentinel rows** (839 data, 8,605 RAPGAP, 10,422 DJANGOH) are kept by the eΣ cut today with 0 flips
  (`ST/phaseA/A6_validation/xf_esigma_twoway/README.md:18-19`); the helper decides from eΣ inputs; S1 regression count.
- **D4 Shifted electron legs** (`U/final_detlegs_optA_2026-10-21/f1_nominal_consistent_esph.py:131-134`) recompute x_F from the shifted
  kinematics; the y_e < 0.94 and 30 < E−pz < 75 stage-1 envelope vetoes (`:137-144`) stay (electron-based by design). The HFS shims
  get the same moved-out E−pz ≥ 75 veto with a counter (`F/phaseB/sigmah_smear/sigmah_smear_shim.py:85` and
  `U/final_detlegs_optA_2026-10-21/empz_hfs_shim.py`), after
  S1 confirms the stage-1 edge (`HQ/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:24`).
- **D5 C_rad** rerun (2026-10-21 consistent campaign, both generators) with D0 and U2 in the worker; `c_rad_gen`, `c_rad_stat`
  (`gB:240-262`), the `bgsub_stage` input (`gB:357-375`), the import-time constants `gB:98-100` and the `born` step
  (`F/esph_downstream.py:124`) read the new tables.
- **D6 Detector-interaction removal, native.** Extra leg: OPT-A builder with rerouting off (174 shards). Per generator,
  inc = IBU(nominal eΣ response, rerouted) − IBU(reroute-off eΣ response); model = ½|inc_DJ − inc_RG|/v, stat = |inc_RG|·SSP/v,
  norm (κ = ±100%) = |inc_RG|/v (`gA:347-364`, both responses eΣ); `detint_removal_rel` = hypot(model, stat, norm) exactly.
  `_ref_M` (`F/esph_downstream.py:216-218`) points to the reroute-off pair. Rejected: the partly-native path (`gA:368-417`).
- **D7 Background split** per bin by source (electron, V0, low-Q², det.-int., DIFFVM) from the rerun's templates and fake partition.
- **D8 Signs, all from this campaign** (user 1a, `.supervisor/memory/decisions/2026-10-24-esigma-rebuild-everything-no-inherited-terms.md`):
  - `ibu_model`: the installed A9 rule, sign of smooth5 over the (cell, z) row of the unfloored half-difference, all bins, stop on
    exact zeros (`HQ/scripts/systematics/covariance_optA_2026-10-20/s08_symmetrize_and_rebase_optA_2026-10-20.py:726-745`).
  - `ibu_reg`: raw sign of 0.5 (k5 − k3)/(L v) (`gA:341`; same file `:773-790`).
  - `c_rad_gen`: raw per-bin sign of RG − DJ times the smoothed magnitude (same file `:823-828`; `gB:241,482`).
  - `detint_model`: sign(inc_DJ − inc_RG); detint norm: per-bin sign(inc_RG) as a signed rank-one vector
    (`detint_removal_norm_signed_rel`, `s08:70`). Mixed signs are allowed and reported; S4 stops only if the integrated increment
    changes sign. The A9 smooth/raw classification stays a fixed convention.
  - s08 reads every sign from these columns and raises if one is missing: the fallbacks at `s08:57-60,69-80` and the `GROUPA_EL`
    gate `:103-114` are deleted, `GROUPA_EL` leaves `protected_files` (`esph_base_common.py:66`), the new columns join the identity
    list `s08:98-100` and the `INFO` columns of `assemble` (`F/esph_downstream.py:308-312,347`).
- **D8b Cut variations rerun** (user: all plots regenerated, `.supervisor/memory/decisions/2026-10-24-no-electron-photon-plots-regenerate-all.md`).
- **D9 Note sentence** (user-approved): all photon-frame variables, including the x_F < 0 cut, use the eΣ photon; in the note and the
  selection text of the long paper and letter; the wording follows U1.
- **D10 Waves** stay under 4,500 jobs. The 246 old held jobs are removed before S3 (user, `.supervisor/memory/decisions/2026-10-24-final-production-run-readiness-audit-freeze.md`).
- **D11 Supersede, never delete**: the 2026-10-23 editions are sha-swept before and after install (`s15:125-152`).
- **D12 Masses.** Reco x_F, z, p_T, φ_h: pion mass for data and MC (a reco quantity must not use truth; the stored MC x_F used the
  matched PDG mass, tree builder `:1845-1850`). Gen: z and x_F use the generator PDG mass; gen p_T per U4. One shared helper serves
  builders and workers.
- **D13 Hemisphere axis**: U1 = (a), along the proton direction in the γ*p frame.
- **D14 C_γBorn rebuilt** (user 1c). First reproduce the current factor bitwise: r = C_rad(10-21) / C_rad_mixed_published(10-09) as one
  ratio multiplies C_γB, σ_γB and stat_unc; σ_rad_mixed, F_EWγ and the relative stat are copied (recovered formula, 0 mismatches
  on 490 rows per h1-mc-expert). Then rebuild: 260 no-Z e⁻ jobs on a fork of the 10-21 worker (D0/U2 photon, no η/J/ψ veto),
  denominators from the new D5 DJANGOH partials, finalize with f(e+p) = `CRAD_F_EP` = 0.5439017406824048
  (`HQ/scripts/radiative/final_crad_consistent_2026-10-21/run_finalize_prod.sh:4`, on the L_processed basis; the user's 0.544);
  `compute_eminus_withz_crad_releasebin.py:61` uses the same constant. Output `SH/data/radiative/cgammaborn_<tag>/cgammaborn_<tag>.csv`.
- **D15 SSP native.** The OPT-A builder writes the 2026-10-13 reslice counters (removable vs K±-parented det.-int., the schema of
  `HQ/output_v6/studies/cone_s2prime_2026-10-11/parentage_reslice/results/detint_motherspecies_perbin_results_cone_2026-10-13.json`)
  in w1 as a per-shard sidecar JSON (not in the response ROOT/npz, whose merge rejects unknown keys,
  `merge_migration_per_charge_cone_s2prime_optA.py:36-41`); a new `ssp_agg` step before groupA writes the
  `headline_refined_removable.pooled.removable` json that `gA:230-235` reads; a missing `parentage_json` is a STOP (no fallback to
  the `gA:107-109,246` default). Gate: SSP == sqrt((1 − f)/(f N)) from the new counts, f within 0.02 of 0.910. Recorded fallback
  needing no rerun: weighted f and n_eff from the merged `h_rerouted_reco_marginal_*` / `h_keeper_*` arrays.
- **D16 Background normalisation**: `bgsub_env100_hyp` is one rank-one ρ = +1 term (`esph_base_common.py:110`, `gC:22-24,203-212`).
  Low-Q² split and DIFFVM per U3; if split, the 0.30·lq piece leaves the `bgsub_model` hypot (`gB:310-311`) and lq is passed to
  groupB, from the same unguarded s·lq and pair sum (`gB:225-229,300-311`), with the gate old_model² == new_model² + (0.30·lq/d)²
  at 1e-15 relative tolerance (absolute where d ≤ 0, `gB:308-310`); column `bgsub_lowq2_norm_rel` in `TERMS["B"]`, s08
  `RAW["delta_lowq2_norm_rel"]` from that column with a uniform + sign (lq ≥ 0), term `cov_lowq2_norm_rank1`, `BUDGET_FORMS`
  text for `bgsub_model_rel` (`esph_base_common.py:147`) updated. DIFFVM (U3a/b): a `diffvm_agg` step sums the per-file outputs to
  `STN/downstream/diffvm/diffvm_reco980_<tag>.npz` (asserting `L_eff_nb` and `scale` = L_eff/2,581,730); groupC unfolds
  d ∓ 1·T_DIFFVM with no further template scale (`gC:378-381` applies L_eff/351000 to the other templates), T zeroed in
  `empty_bin_guard` bins (`gC:384-386`), and writes `diffvm_rho_rel` and `diffvm_rho_signed_rel` (sign((s− − s+)/2) × magnitude,
  as env100 at `gC:211-212`, rule `gC:7,21`); both in `INFO["C"]`, s08 `RAW["delta_diffvm_rho_rel"]`, RANK1 `cov_diffvm_rho_rank1`, and the
  identity tuple `s08:98-101`.
- **D17 Ratio and A_h** use the full covariance with cross-charge blocks (`F/phaseD/s08s09/s09_esph_base_2026-10-23.py:172-220`;
  `plot_asym_esph.py:120-139`); `ratio_unc_*` columns (`gC:270-308`) are not quoted. Diagonal terms keep ρ = 0 across charges
  (existing convention, conservative for ratio and A_h).
- **D18 Closure** (user 1b, `.supervisor/memory/decisions/2026-10-24-long-paper-closure-numbers-esigma.md`): a new step on the new
  responses writes bin-by-bin closure (DJ factors on RG pseudo-data vs RG truth and the reverse, both charges, integrated and per
  bin) and the IBU cross-closure (`DOC/closure/compute_ibu_crossclosure_esph.py`). TUnfold (`PAPER:184`) is ported to the eΣ
  OPT-A responses: `U/tunfold_s2prime_z005_20261012/` (τ scan), `U/tunfold_s2prime_z005_v2_nonneg_20261012/`,
  `U/tunfold_s2prime_z005_v3_20261013/` (toys), `U/tunfold_s2prime_z005_v5_20261014/`; inputs today are
  `response_*_detint_s2prime.root` and `tunfold_s2prime_blocks.npz`. If the port fails its S2 smoke, the run stops there and the user
  decides between dropping the TUnfold sentences and fixing the port.
- **D19 DIFFVM fractions**: the eΣ data-vector builder fork (`U/cone_s2prime_2026-10-11/build_data_vector_v4_alphaveto_esph.py` with the
  helper) runs on the DIFFVM sidistrees (reqIDs 8445-8448), so the selection is the release's; scaled by L_eff/2,581,730.0 (sum first,
  scale once); computed in S4. The June extractor `SH/scripts/backgrounds/diffvm_rho_reco980_2026-10-07/extract_diffvm_rho_reco980.py`
  is retired; its hard check weight1·weight2 == 1 (`:558-561,685-688`) moves into the fork, which ignores weights.
- **D20 E−pz of the HFS legs**: flip counts from the 1,044 `*.n2shim.json` sidecars in one Condor job; `n_envelope_veto` logged in
  the f1 fork; the shim reads the batch Yes so x_F follows shifted legs; `PAPER:223` states that the E−pz effect is included through
  the shim and gives the flip counts. The one-sided E′ < 11 GeV truncation is documented (data and MC alike).
- **D21 Generator φ_h pages on the true γ*** (user, verbatim: "Yes, all should be the true gamma*, unless you see a reason for it not to
  be."; supersedes `.supervisor/memory/decisions/2026-10-24-gen-phih-appendix-keeps-truth-photon.md`). The 90-shard production
  `HQ/scripts/plotting/phi_reco_luminorm_2026-06-12/condor_esigma_gen_2026-10-24/shards.sub` plus finalize is rerun with the helper
  (true γ*, ISR-only E_eff, U1 axis, PDG mass) instead of the stored xFGen/zGen (`plot_phi_rad_nonrad_gen_v3_compact.py:763-782`);
  appendix sentence: "The generator-level distributions use the exchanged virtual photon, q = k_eff − k′, where k_eff is the incoming
  lepton after subtracting any initial-state photon, and φ_h is measured in the γ*p centre-of-mass frame."
- **D22 New campaign roots** `FN`/`STN`. Every file under `F` and `U/esigma_cutvar_2026-10-23/` that contains `esigma_frame_2026-10-22`
  or `esigma_cutvar_2026-10-23` (about 111 .py/.sh/.sub/.args files by grep, logs and pre-edit copies excluded) is copied to `FN`
  and re-pointed; args files are regenerated by the copied `make_campaign_*` (never edited in place). Examples beyond those below:
  `run_prod.sh:6,21`, `make_campaign_esph.py:9-10`, `phaseD/common_covariance_optA.py:60-62` (the response s02b/s03 read),
  `phaseD/run_s02a_esph.py:19`, `run_s03_esph.py:13,21`, `cutvar_downstream.py:19-25,79`, `run_cutvar.py:9`. Listed ones:
  `esph_downstream.py:22-38`, `esph_base_common.py:12-38`, `merge_and_check_esph.py:12-15`, `run_post.sh:4`, `phaseD/run_s02b_esph.py:13-22`,
  `phaseA/A2_b1floor/model_unfloored_diag.py:7` and `b1_esph_full.sub`, `phaseA/A3_phih/compute_phih_native.py:12,17`,
  `phaseA/A4_trackeff/compute_tracking_native.py:10,17`, `phaseB/sigmah_smear/merge_reduce.py:8-11`,
  `phaseB/jes_sym/jackknife_jes.py:8,14`, `finalize_jes_sym.py:6`.
- **D23 R_MIN_STANDING** = 0.98167 (`gB:68,342`) stays: a pooled track-provenance prior with no frame variable or binning, already
  the conservative edge of its envelope (|1 − r| = 0.0183, `SH/docs/systematics/BGSUB_S2PRIME_REBASE_ADDENDUM_2026-10-11.md:459,517`);
  its bin dependence (f_standing, `gB:318-354`) is recomputed in eΣ.
- **D24 Tight-DCA envelope check** (`PAPER:231`) reruns on the eΣ chain as a 7th cut variant. The shim can only change existing
  cut keys (`U/esigma_cutvar_2026-10-23/cutvar_shim.py:18-21,45-50`) and no builder has a raw |DCA′| cut (they cut |DCA′/cosh η| < 2),
  so a new key `DcaPrime_abs` (nominal ∞) is added at the fill point of the data-vector (`:218-222,429,454`), OPT-A (`:583-587,1261,1276`)
  and template (`:115-120,601-622`) builders, and to the low-Q² builder, which reads `DcaPrime` as stage 3 did
  (`HQ/scripts/unfolding/tightdca_stage3_2026-10-17/build_lowq2_djangoh_bg_hist_v6_alpha0p2_reco980_tightdca.py:78-102`). The shim gets
  the key in `NOMINAL` and `NOMINAL_LOWQ2` and a `tightdca` variant, and a missing key becomes a hard stop with no exception. The toy
  step is a rewrite on the OPT-A runner, not a spec swap of `run_tightdca_shared_toys.py` (built on electron-frame S2′ modules,
  `:69-71,120-134`): both legs unfolded with the OPT-A runner (nominal on the new release inputs, tight on the `tightdca` variant
  outputs), CC-8 set to the release's 100-toy median at seeds 12345/22345 (`run_bgsub_setrprime_reunfold_cone_optA.py:101,169`),
  outputs under `STN`; the stage-3 significance definition is kept; the eta1p4 mismatch-count rule (`cutvar_downstream.py:150-156`)
  applies to `tightdca` too. If a significant shift needs more than 100% of the envelope, the run stops before install and
  the user decides (κ is binding).

## Readiness audit: would anything force another production?

C = covered in this run; FI = frame-independent (evidence); U = user question. No item is open.

| Item | Status | Where / evidence |
|---|---|---|
| Reco x_F cut, all builders, forks and control fills (11 manifest forks, A3, A4, σ_h shim, cutvar, `controls/reco_kinematics_esph.py`, `controls/reco_multiplicity_esph.py`, φ_h reco) | C | Outline; S1 static gate `no_stored_xf_reco` |
| Gen x_F (OPT-A, nucliaweight, trackclass, shifted, straddle, A3 flatphi, C_rad, no-Z) | C | D0; S1 static gate `no_stored_xfgen` |
| Hemisphere axis | U1 | |
| Gen Q², y, x, fiducial, cell | U2 | D0 |
| Gen z | C | D0 (true γ*) |
| Gen p_T mass | U4 | D12 |
| Reco Q², y, x, bin assignment (Q2es, Yes, Xes) | FI | already eΣ (OPT-A builder `:1242-1247,1367`) |
| Gen EMPZ > 45, alpha-cone veto, y_e < 0.94 and E−pz envelope vetoes | FI | lab-frame truth or reco by design (D4; OPT-A `:1338`) |
| Q2eGen/YeGen fallbacks in forks | FI | dead fallback, used only without eΣ branches |
| Low-Q² builder stored-z preselection (`:289-292`) | C | finite eΣ z; S1 counts |
| Masses | C | D12 |
| Prior, ibu_reg k3/k5, fake column, miss, straddle, bgsub_model/stage/fakefrac, env100, nonprimary, NW, tracking, φ_h, JES, σ_h, detlegs | C | rerun on new inputs; run_post order (Outline) |
| A2 replicas (ibu_model floor) | C | 100 jobs + diag + aggregate after w1 |
| detint model, stat, norm; SSP | C | D6, D15 |
| Five rank-one signs | C | D8 |
| R_MIN_STANDING | FI | D23 |
| lumi 2.7%, L_eff, κ conventions, 0.30 low-Q² norm, MC lumi scalars, DIFFVM L 2,581,730 nb⁻¹ | FI | run set or fixed conventions (`gB:67`, `F/esph_downstream.py:3,33,36`) |
| Bin map, row order of the installed edition | FI | binning identical (`s08:44-46`, `s09:117`) |
| Electron reads for information or shape only: `stradagg` clean releases (`apply_crad_and_diff_vs_cleanrelease_optA.py:19-20`), groupA reference `MIG_ROOT` shape (`esph_downstream.py:216`), A3 `phi_h_gauss_rel_carried_electronframe` | FI | lineage allowlist (D22); not used in any quoted value |
| `final_budget.csv` columns, dashed electron total (`esph_downstream.py:333-360,397,405`), `_old_pairing` (`gB:512-524`), gC OLD/BUDGET_NW (`gC:71-84`) | C | removed |
| `m2_zshift_rel` "justified zero" (`gB:16,502`) | C | binding no-ISR-term decision (stays unquoted); with gen z on the true γ* the zero is no longer automatic, so S4 recomputes and reports it |
| C_rad tables, variants rows a-c (`NOTE/sections/radiative_corrections.tex:105-160`), medians `:92`, caption `:158`, text `:193-199` | C | D5, D14; e⁻p with-Z row via per-beam remerge (2 jobs) |
| No-Z factor | C | D14 |
| Background split; normalisation ρ = +1 | C / U3 | D7, D16 |
| Ratio and A_h covariance | C | D17 |
| Bin-by-bin closure, TUnfold, IBU cross-closure | C | D18 |
| Tight-DCA numbers `PAPER:231` | C | D24 |
| DIFFVM `PAPER:157` | C (fractions) / U3 (term) | D19 |
| Low-Q² fraction (0.005%) and photoproduction (about 5 PYTHIA events) in `PAPER:157` | C | low-Q² template rerun; S1 locates the PYTHIA count's producer and recounts with the eΣ selection, else the sentence is removed |
| E−pz coverage of HFS legs; `PAPER:208,223` | C | D4, D20 |
| Cut variations; robustness table `PAPER:277` | C | D8b |
| Paper \draft numbers `:210,223,277,323,349`; editorial items `:49,88,90,182,379,390` | C / editorial | S8 |
| Supplemental Material C_rad content (none exists, `LETTER/README.md:22`) | C | S8 writes it from the new tables |
| Note `unfolding.tex:109-111` stale sentence | C | S8 replaces the response figures, deletes the sentence |
| Reco φ_h pages (203 jobs); gen φ_h pages (90 shards + finalize) | C | S8; D21 |
| Background-fraction figure, release README/MANIFEST/tarball, electron swap-one and trackshift plot removal | C | S8 |
| C_γBorn mismatch 1.041/1.040 vs 1.040/1.039 across documents | C | one table, `numbers_match` |
| `build_beam_sign_data_vectors_v6.py` (electron reads `:78`) | C | S1 confirms it is not called, else switches it |

## Outline

- Helper `FN/esigma_xf.py`: `reco_xf(batch, mass=M_PION, axis=<U1>)`, `gen_photon(batch) -> q` (D0 ISR tag),
  `gen_xf(batch, axis=<U1>)` (PDG mass), `gen_pt(batch, mass=<U4>)`, and per U2 `gen_kinematics(batch) -> (Q2, y, x, z, p_T)`; with U1(a)
  x_F = 2 p*·P̂*/W, cut x_F < 0; the C_rad worker's `xf_observable` uses the same projection. Reads Q2es/Yes from the batch, so
  shimmed legs follow. md5-pinned in `FN/fork_manifest.json` (`make_esph_forks.py`), pins at `run_prod.sh:12,18`; A3 flatphi, A4
  randdrop and the σ_h shim join the pinned set; cutvar pins (`cutvar_shim.py:12-16`, `run_cutvar.py:10-21`) updated.
- Reco cut sites (keep `isfinite & < 0`): `U/cone_s2prime_2026-10-11/build_data_vector_v4_alphaveto_esph.py:416,443,459`;
  `F/phaseA/A4_trackeff/build_data_vector_v4_alphaveto_esph_randdrop.py:420,447,463`;
  `U/cone_s2prime_optA_2026-10-19/build_migration_th2_per_charge_cone_s2prime_optA_esph.py:1266-1267`;
  `F/phaseA/A3_phih/build_migration_th2_per_charge_cone_s2prime_optA_esph_flatphi.py:1379-1380`;
  `U/nucliaweight_variant_2026-10-21/build_migration_nucliaweight_variant_esph.py:1309-1310`;
  `U/trackclass_variant_2026-10-20/build_migration_trackclass_variant_esph.py:1303-1304`;
  `U/final_detlegs_optA_2026-10-21/build_migration_th2_per_charge_cone_s2prime_shifted_optA_esph.py:851,1480-1481`,
  `.../build_data_vector_v4_alphaveto_shifted_f1_esph.py:209,362`, `.../f1_nominal_consistent_esph.py:131-134`;
  templates `HQ/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_v4_final_weightfix_20260625/copied_scripts/produce_reco_background_vector_alpha0p2_v2_esph.py:616-626`;
  low-Q² `HQ/output_v6/studies/lowq2_djangoh_bg_alpha0p2_2026-06-23/subtraction_ready/build_lowq2_djangoh_bg_hist_v6_alpha0p2_reco980_esph.py:216-222,289-292,296`;
  covariance `HQ/scripts/systematics/covariance_cleanrelease_2026-10-17/s02a_build_event_joint_tables_esph.py:216-222,252,257,279`.
- Gen sites: OPT-A builder `:1081,1483` (and per U2 the cell/fiducial `:1103-1109,1456-1457,1528-1531`); nucliaweight `:1528`;
  trackclass `:1520`; shifted `:1712`; straddle `U/cone_s2prime_optA_2026-10-19/opta_straddle_percell_counter_esph.py:364`;
  A3 flatphi `:1188,1202,1240,1566-1567,1596` (stored xFGen and zGen; both recomputed with the true γ*). OPT-A builder gen z/p_T reads `:1130,1453-1454,1499-1530` likewise.
- Gen p_T sites (U4): `_compute_pTh_cm_and_z` (OPT-A builder `:710-719`) serves the pool (`:1127`), the body (`:1512`) and reco
  (`:1360`); it gets a `mass` argument (gen calls pass `gen_mass(GenPDG)` per U4, reco passes `M_PION`) instead of an edit at `:719`;
  the same in the trackclass, nucliaweight, shifted and flatphi forks, the straddle counter, the C_rad worker (`observables :61`) and
  the no-Z worker. Body and pool must agree (`n_opta_body_vs_pool_mismatch`, `:2002-2010`).
- Benign-mismatch certification (`opta_benign_mismatch_rule.certify`, pinned to 3 rows, `:37-60,278-301`): the w1 OPT-A builder writes
  each body-vs-pool mismatch row (run, event, key, both p_T, cell, z, ip) to the shard sidecar, and an S3 step re-runs the benign
  checks on them and writes a new pinned artifact. The `FN` copy of `opta_benign_mismatch_rule.py` sets path, sha256, campaign id and
  MAX (= the recorded row count) from that artifact (`:78-86,321-322`), with the per-row benign checks unchanged; its importers
  (`run_bbb_ibu_release_cone_s2prime_optA.py`, `run_bgsub_setrprime_reunfold_cone_optA.py`, `cutvar_downstream.py`,
  `smoke/check_smoke_optA.py`) import the `FN` copy. Any row failing a benign check stops the run before S4.
- Control fills `HQ/scripts/plots/esigmaphoton_2026-10-23/controls/reco_kinematics_esph.py:766,792,807` and
  `controls/reco_multiplicity_esph.py:384,410,425` cut on stored `xF_SIDIS_es`: switched to the helper (pion mass) and refilled with
  copies of `HQ/scripts/plots/esigmaphoton_2026-10-23/condor/reco_kinematics_*.sub` and `reco_multiplicity_*.sub` (19 jobs) with
  `--out-root` under `STN` (note `backgrounds.tex:71,84`); the six sub files are in the `no_old_root` scope.
- Fork naming: new forks get the suffix `_esxf.py` next to the originals (the 10-23 `_esph.py` forks stay untouched; `make_esph_forks.py:116-117`
  refuses to overwrite), every `sys.path.insert` of `F` (OPT-A `:517`, nucliaweight `:517`, data vector `:181`, f1 `:18`, trackclass,
  s02a, template, low-Q², `plot_phi_reco_luminorm_v6.py:37-38`, `controls/reco_*_esph.py:240,198`) points to `FN`, and the cutvar PINS
  (`cutvar_shim.py:12-16`) and all callers name the new files. groupA gets the new A2 floor file explicitly (`gA:105-106,245`).
- Radiative: `HQ/scripts/radiative/final_crad_consistent_2026-10-21/crad_v3_worker_final_consistent.py` (x_F; fiducial `:130`,
  cell `:129` per U2; GenEnPhoton added to `REQUIRED`), its finalize scripts; no-Z fork replacing
  `HQ/studies/radiative_correction_genonly_v1/output_v3/nozborn_hist_releasebin_20261009/scripts/crad_v3_worker_releasebin.py`
  and its finalize (`crad_v3_finalize_nozborn_releasebin.py:97` BEAM → CRAD_F_EP; G3/G5 on the new C_rad, G6 bands `:103-105`).
- Detint: `--no-detint-reroute` flag (OPT-A CLI `:2699-2732`), `optaoff)` case next to `opta)` (`run_prod.sh:30`), 174 lines in
  `prod/big.args`, `optaoff` merge in `merge_and_check_esph.py:54-98`; the SSP counters (D15) in the same builder.
- Merge gates (`merge_and_check_esph.py:14,62,86`; cut variations `cutvar_downstream.py:79`): gen side of each leg identical to the
  new nominal, and comparisons against the `STN` release, not the installed one or the electron OPT-A.
- Merge runs after w1 and before w3 (A2 reads the merged responses, `b1_esph_full.sub`).
- `run_post.sh`: run `groupCcache` whenever `groupC` runs (`:12`; `esph_downstream.py:287-289`); after `release` insert `a2diag`,
  `a2agg`, `a3`, `a4`, `smear_reduce`, `jes`, `ssp_agg`, `diffvm_agg`, `closure`. A3, A4 and `assemble` take bin keys from the new
  release `ibu/cross_sections.csv` instead of the v1 budget (`compute_phih_native.py:40`, `compute_tracking_native.py:45`,
  `esph_downstream.py:318,334,371`). groupB drops `phi_csv`, `comp_tracking`, `comp_phi` and `_old_pairing` (`gB:498-499,541-542,581-582`).
- Budget term lists, kept in step for U3: `TERMS["B"]` (low-Q²) and `TERMS["C"]` (DIFFVM), `assert len(ALL)==24` → 25 or 26
  (`esph_downstream.py:307,316`), `RANK1`, `BUDGET_FORMS` (`esph_base_common.py:100-155`), s08 RAW, the "24" strings in s09 (`:99,267`),
  the source count in the documents; plot styles (`esph_downstream.py:402-403`) extended; s15 gate count (13, `s15:90-91`) and
  manifest counts (138 and 106, `s15:73-74,83-84,148-149`; +2 files per new term) updated; groupC `ratio_unc_*` columns kept (s09 checks them, `s09:191-199`).
- Release scripts: copies of `esph_base_common.py` (`INSTALLED` only for the bin map), s08 (D8; date `:215`), s09 (`:242`; record
  strings `:236-238,247,263-265,284-286` and `BUDGET_FORMS` text `esph_base_common.py:134,147` updated), s15 (`PKG`, `REL`, `BUDN`, `COVN`, `INST`,
  `MANIFEST_SHA`, `SUPERSEDED`, counts, `BUD_CSV_SHA`), s16 (names, TARGETS pre-edit shas `:40-61` refreshed), s18 (closures carried,
  P-CRAD-LUMI re-checked), s17 only on audit. New package-assembly script (provenance/, SOURCES.tsv, PHASE_F_COMMANDS.md,
  MANIFEST.sha256, draft install record, as in the 10-23 package).
- Producers (re-point release, budget, covariance, C_γBorn, response and output tag): `HQ/scripts/plots/esigmaphoton_2026-10-23/`
  (`plot_born_7panel_esph.py:22-27`, `plot_asym_esph.py:26-27`, `plot_ratio_esph.py:12`, `plot_covariance_esph.py:14-16`,
  `controls/plot_reco_kinematics_esph.py`, `cut_variations/plot_cut_variations.py:13-16`, `budget_grouped/`, `make_release_readme.py:10-11`);
  `SH/plots/systematics/esigmaphoton_2026-10-23/budget_7col/plot_budget_7col_esigmaphoton_2026-10-23.py`; `DOC`
  (`letter/scripts/build_esigma_data_sides.py:16,19,20`, `fig2_letter_noz/producer_esigma_noz_letter_2026-10-24.py`,
  `letter/v4/compare_data_vs_mc_esigma_v4.py`, `letter/scripts/make_maptmd24_overlay_esigma.py`, `note/plot_born_7panel_note.py:23-30`
  (electron overlay dropped), `note/plot_reco_multiplicity_note.py:23`, `note/born_xsec_noz/plot_born_7panel_note_noz.py:28-34`,
  `note/born_xsec_vertical_noz/plot_born_xsec_vertical_noz.py:46`, `note/theory_comparison/scripts/make_theory_comparison_note.py`,
  `closure/plot_ibu_crossclosure_7col_zoom_z005.py:16`); response figures (`HQ/output_v6/studies/alpha0p2_xfgen_response_z005_20261007/scripts/plot_purity_vs_pTh_7col_alpha_xf.py`,
  `HQ/scripts/plots/esigmaphoton_2026-10-24/plot_migration_matrix_esph.py`); background fractions
  (`HQ/scripts/canonical_highQ2_sidis_2026-06-30/plotting/regen_minimal_7col_plots.py`, `run_bgfrac_esigma.sub`); reco φ_h
  (`HQ/scripts/plotting/phi_reco_luminorm_2026-06-12/condor_esigma_2026-10-24/`, importing the helper); table generator
  `SH/scripts/tables/latex_cross_section_tables_2026-10-09/` (`--crad-noz-csv` = D14 file).
- Documents: note (`sections/summary.tex:26-28,46`, `systematics.tex:16,32,44`, `selection.tex:117`, `unfolding.tex:100,109-111,115,124,130`,
  `radiative_corrections.tex:58-61,92,105-160,193-199`, appendix φ_h), paper (`paper_full.tex:74,88,124` selection; numbers
  `:157,165,171,182,184,188,195,208,210,212,223,231,233,237,252,255,277,281,303,323-339,349,376`), letter
  (`sections/observables_method.tex:26,30,32,35`, `results.tex:1`, `summary_outlook.tex:1`, Fig. 2, selection text, new SM file).

## Slices

| Stage | Jobs | Per job | Elapsed |
|---|---|---|---|
| S2 smoke + smoke2, TUnfold and tight-DCA smoke | about 34 | 1-2 min | 10 min |
| S3 w1: prod big (+174 `optaoff`) | 2,784 | 1.4 / 3.8 min | 25 min |
| S3 w2: prod small | 1,733 | < 0.1 / 1.8 min | 25 min |
| S3 w3: A2 100, A3 174, A4 33, σ_h 174, JES 7 | 488 | 1-2 min | 15 min |
| S3 w4a: C_rad DJANGOH | 2,953 | 8 s / 1.5 min | 10 min |
| S3 w4b: C_rad RAPGAP + 3 finalize | 1,780 | 11 s / 6 min | 10 min |
| S3 w5: no-Z 260 + merge + finalize, e⁻p remerge 2, DIFFVM data-vector fork (about 404, S1 confirms), sidecar pass 1 | about 669 | minutes | 20 min |
| S4 post chain (merge … assemble, with the inserted steps) | about 16 | 0.1-5 min | 1 h with waits |
| S5c closure 1, TUnfold about 12 | about 13 | minutes | 20 min |
| S5′ cut variations: 7 × (174 + 1,732) + posts + 2 toy jobs, 3 waves | about 13.4k | 1 / 22 min | 2 h |
| S6 s02a 660 + merge, s02b, s03, s08, s09 | 665 | < 0.3 min | 10 min |
| S7 s15 install | 1 | 0.3 min | 1 min |
| S8 φ_h reco DAG 203, φ_h gen 90 shards + finalize, control refills 19, bgfrac 1 | about 316 | about 2 min | 30 min |

About 25k jobs; Condor wall about 6-8 h in waves; with gates, reviews and fix cycles, about two working days.

### S1: helper, all switches, proven on one file per sample
- Units: (a) helper + tests; (b) builders and forks; (c) templates, low-Q², straddle, s02a, cutvar; (d) radiative workers;
  (e) groupA/B/C, downstream, s08/s09 copies; (f) closure, TUnfold port, DIFFVM fork, producers.
- Tests: `no_old_root: grep -rn 'esigma_frame_2026-10-22\|esigma_cutvar_2026-10-23\|<the 11 manifest fork basenames>' over FN, every file in
  FN/fork_manifest.json and every re-pointed producer -> only allowlisted lines`; `no_stored_xf_reco: no executed builder, fork or
  producer reads xF_SIDIS_es`; `pt_mass_calls: every gen-side call passes gen_mass(GenPDG) (U4a), every reco call M_PION`;
  `axis_sign: U1(a) and U1(b) agree in sign on > 99.9% of tracks with |x_F| > 0.1; U1(a) == −xF_Rivet on one file`;
  `dca_inf_identity: DcaPrime_abs = ∞ -> bitwise identical output`; `gen_pt_mass: one file -> per-bin gen p_T change logged`;
  `reco_matches_twoway: one RAPGAP file, U1(b) axis, PDG mass -> two-way per_file counts`; `data_unchanged_U1b: one data file
  -> kept mask identical`; `mass_effect: one DJANGOH file -> pion vs PDG change logged`; `isr_tag_vs_genrad: one RC v3 file, GenEnPhoton > 0 rows, weighted
  -> tag == (GenRad == 1) on > 99%`; `gen_kin_identity: true-γ* Q², y == Q2sGen, YsGen per row`;
  `same_events_check: are response truth and C_rad σ_rad the same events; is IsV0DaughterGen == IsV0DaughterGen_v3`; `fsr_combined: GenElecE − GenElecUncE == GenEnPhoton
  on FSR rows (hard stop)`; `gen_nonrad_identity: GenEnPhoton == 0 rows -> gen x_F (U1b) equal stored xFGen within float32 tolerance`;
  `no_stored_xfgen: no executed script reads xFGen` (static); `reroute_off_identity: flag absent -> bitwise equal`;
  `cgb_reproduce: recovered formula -> current C_γBorn CSV bitwise`; `ssp_counters: one file -> counters match the 10-13 schema`;
  `empz_edge: stage-1 75 GeV edge present -> shim veto counts logged`; `beamsign_dead`; `lowq2_zpre`; `pythia_count_located`;
  `nan_dump`; `sentinel_count`.
- Checkpoint: login node, one file each, under 5 min; tests exit 0; manifest md5 check passes.

### S2: Condor smoke through the merged response
- Tests: `dv_delta: data vector change per bin reported (U1a) or bitwise (U1b)`; `gen_delta: per-bin truth change table; under
  U1(b) z ≥ 0.05 below 0.1%, under U1(a) report only`; `tunfold_smoke: exit 0`; `tightdca_smoke: n_dca rejected > 0, tight ≤ nominal
  in every bin for raw counts and positive weights (data vector, response reco marginal, templates), overall reduction inside the stage-3
  window`; `tightdca_toys_smoke: toy step runs on the smoke merged outputs`; `ssp_smoke: counters -> ssp_agg -> load_stat_scale`;
  `lineage_smoke: every input under STN or allowlist`.
- Checkpoint: gate JSON all True; independent critic review before S3.

### S3: production waves 1-5
- Tests: `jobs_complete: 2784/1733/488/2953/1780/~669 exit 0`; `benign_mismatch_recertified: new pinned artifact written`; `crad_nonrad_identity: σ_nonrad == 2026-10-21 within float32
  tolerance under U1(b) (any U2); under U1(a) the change is reported`; `noz_gates: G3, G5, G6 pass`; `merge_gate: gen side identical across legs`.
- Checkpoint: `run_post.sh` preflight passes on the merged dirs.

### S4: budget, all from this campaign
- Tests: `truth_parity`, for RAPGAP and DJANGOH: response gen marginal (pool + miss)/L_MC vs C_rad σ_rad per bin, after aligning
  the known selection differences (IsV0DaughterGen vs `_v3`, the worker's Q2Gki > 100 domain `:123,127` and y-index containment
  `:147`); exact identity if S1 finds the same events, else global χ² p > 0.01 and no bin beyond 4σ;
  `born_shift`, on fixed data (the 2026-10-23 data vector unfolded with the new responses and C_rad): stop if any z ≥ 0.05 bin moves by
  more than max(3σ of the new Born stat, 0.25 × its total quoted uncertainty) or any z row's integral by more than 0.3%; the
  z ∈ [0,0.05) row is reported against the S2 prediction; under U1(a) each bin's threshold is widened by its |dv_delta| (the fixed
  data were cut on the old axis);
  `detint_native_median: 0.1-0.4%`; `detint_hypot_exact`; `sign_columns: 490 finite signed values per rank-one term,
  |signed| == magnitude`; `detint_integrated_sign: same sign as the pinned 2026-10-23 integrated increment, else stop`; `ssp_gate`; `lineage_new_campaign_only: every path in
  groupA/B/C inputs, term_sources, extra_inputs under STN or allowlist`; `lowq2_split_identity` (U3a); `term_delta_table`.
- Checkpoint: budget CSV with `total_quoted_esph_rel`; medians per charge.

### S5: Born and no-Z tables
- Tests: `born_integral_delta: below 0.3%, else report`; `noz_stat_matches`; `one_cgb_table`.
- Checkpoint: 490 rows per table; integrals printed.

### S5c: closure and TUnfold
- Tests: `bbb_closure_written: integrated and per bin, both directions`; `tunfold_consistency: χ²/ndf, negative-bin pulls, toy bias written`.
- Checkpoint: JSONs under `STN`.

### S5′: cut variations including tight DCA
- Tests: `variants_complete: 7/7 exit 0`; `tightdca_envelope: every significant shift within 100% of the eΣ envelope, else stop`.
- Checkpoint: `plot_cut_variations.py` reads all seven.

### S6: covariance, package and gates
- Delivers: s02a/s02b/s03/s08/s09 copies; package assembly; s16 TARGETS refreshed.
- Tests: g0-g7, ga10 PASS (`s09:78-140`); `g_s03_closure`; `no_inst_sign: no sign read from INSTALLED`; `lineage_new_campaign_only`
  over s08 `inputs_sha256`, `protected_files` and the input paths s02a/s02b/s03 record; `manifest_complete`; ratio and A_h columns written.
- Checkpoint: all gates True (s15 count updated); reviews by critic, h1-analysis-critic and h1-systematics before S7.

### S7: install and promotion (user sign-off)
- Tests: `manifest_bitwise`; `old_release_untouched`; `s16_dry`, `s18_dry` all DRY, then EDITED.
- Checkpoint: post-install audit PASS; backups in `SH/docs/promotions/<tag>_preedit/`.

### S8: figures and documents
- Delivers: every producer re-run; reco and generator φ_h pages; release README, MANIFEST, tarball; trackshift and electron swap-one plots removed;
  numbers, tables, D9 and D21 sentences, `unfolding.tex:109-111` replaced, SM C_rad table, \draft numbers.
- Tests: `numbers_match: every number listed in the Outline equals the new outputs`; `no_old_tag: no *_2026-10-23 or earlier release,
  budget, covariance, C_γBorn or response path in any producer or table generator`; `no_draft_numbers`; `d9_once`.
- Checkpoint: each repo builds (latexmk) without errors; commits on the current branches.

### S9: FREEZE
- Delivers: `HQ/output_v6/central/data_release/FREEZE_RECORD_<tag>.json` with sha256 of every release, package, budget, covariance,
  C_rad, C_γBorn, closure and figure file, every script and producer that ran, the Condor cluster ids, and each document repo's
  commit; annotated tags `freeze-<tag>` in the workspace repo and the three document repos (pushed with the documents per
  `2026-10-24-esigma-papers-and-note-push-authorized`); release, budget and covariance dirs made read-only (`chmod -R a-w`); a memory
  decision entry and a STATE/handoff "FROZEN" section.
- Tests: `freeze_verify: recomputed sha256 == record`; `tags_exist: 4 tags at the recorded commits`.
- Checkpoint: `freeze_verify` passes from a fresh shell.

## Verification

`freeze_verify` passes and every gate record from S1-S8 is True.

## Risks

- U2 with gen z and p_T reopens the 2026-10-19 "MC-2 double-count" question; S4 `truth_parity` and `born_shift` are the checks, and the
  h1-analysis-critic review at S2 must rule on it before S3.
- U1(a) and U2 change the measured definition; the size is only known after S2. The gates stop before install, not after.
- A rank-one sign flip (D8) can move ratio and A_h errors; S6 reports it before install.
- The tight-DCA check could need more than the ±100% envelope; the run stops before install by design.
- Copy-and-edit of hard-coded constants; covered by the lineage gates.
- The figure producers are new and largely unrun; S8 may need fixes.

## Scope Notes

- Out: tree rebuild, low-Q² parity, the swap-one table, ±0.5% E′ legs, editorial \draft items.
- Binding (do not reopen): no-Z Born is nominal; C_γBorn carries the C_rad gen/stat components, no own term; no ISR term; JES
  RG-only symmetrised; Σ_h smear only; Φ_h convention; det.-int. κ = ±100%, rank one; one bundled promotion; low-Q² parity
  documented only (`SH/docs/handoffs/esigma_release_2026-10-24/START_HERE.md` §2); user decisions of 2026-10-24 in
  `.supervisor/memory/decisions/` (`xf-esigma-photon-rerun-new-release`, `xf-gen-true-gamma-star-option-a`,
  `esigma-background-uncertainties-recompute`, `esigma-rebuild-everything-no-inherited-terms`, `long-paper-closure-numbers-esigma`,
  `cgammaborn-source-table-rebuild-f0p544`, `gen-phih-appendix-keeps-truth-photon`, `final-production-run-readiness-audit-freeze`,
  `no-electron-photon-plots-regenerate-all`, `noz-*`).
- Independent critic before S3 and before S7.

## Assumptions

- Builders read the highest ChargedTot cycle; S1 checks one file.
- The DIFFVM sidistrees carry the eΣ input branches; S1 checks one file.

## Log

- 2026-10-25 executor: S2 PASS (STN/smoke/s2_gates.json 13/13; clusters 5884527-5884542); data -21 tracks all U1(a) axis at |x_F|<0.013, z<0.05; gen truth +6.6% = FSR events restored by ISR-only E_eff (+6.1%) plus ISR z/pT on the true gamma* (high-z rise), C_rad rad leg moves alike; no-photon rows change only by U4 (non-pions) and U1(a); new S3 step FN/steps/benign_mismatch_recertify.py; reviews h1-analysis-critic PASS, h1-mc-expert OK, critic C1-C4 fixed; deferred: e-p per-beam C_rad row has no producer (S8 open item), truth_parity run after w1+w4 before later waves

- 2026-10-25 executor: S7 INSTALLED (s15 cluster 5888201, 49/49 hard gates, 106 files bitwise, 1705 protected unchanged; record sha 89d07b5d); s16 21/21 and s18 6/6 EDITED; post-install audit PASS (output_v6/studies/esigma_final_2026-10-24/s7_post_install_audit.json); deferred: S8

- 2026-10-25 executor: S8 DONE: release plots plots_esigmaphotonxf_2026-10-24 and docfigs regenerated (plot reviews fixed), tables and numbers inventory, e-p per-beam C_rad rows (exact to the nominal); note d82bd90 (development), paper 1f94cb6 (main), letter 75274d5 (esigma-port->main) pushed fast-forward; h1-analysis-critic round 2 PASS note/letter, paper fixes verified; deferred: S9 freeze (not requested), SAVE-CONTEXT handoff

## Todo

- [x] User answers U1-U4; plan marked READY (scope: S1 and S2 only)
- [x] Remove the 246 old held jobs (user-authorized)
- [x] S1 helper, switches, signed columns, SSP counters, closure, TUnfold port, DIFFVM fork, producers; tests
- [x] S2 smoke and gate; critic review
- [x] S3 waves 1-5 and gates
- [x] S4 budget
- [x] S5 Born and no-Z; S5c closure and TUnfold; S5′ cut variations with tight DCA
- [x] S6 covariance, package; three reviews
- [x] User sign-off for install; S7 install and promotion
- [x] S8 figures and documents
- [x] S9 freeze — done at `esigmaphotonxf_2026-10-27b` (2026-10-27; `FREEZE_RECORD_esigmaphotonxf_2026-10-27b.json`, tags `freeze-esigmaphotonxf_2026-10-27b`; program plan P4)
- [x] Handoff and memory per SAVE-CONTEXT.md — handoff section '2026-10-27 FINALIZATION' (program plan P5); memory close-out handed to program plan P6 (pending at the time of this tick)

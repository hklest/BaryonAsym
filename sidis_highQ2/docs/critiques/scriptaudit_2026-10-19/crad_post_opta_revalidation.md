# Post-Rebuild RC-Chain Re-Validation — H1 high-Q2 charged-hadron SIDIS (v6/z005), Option-A truth contract

**Auditor:** radiative-correction specialist, read-only. No file modified. Skill `sidis-hq2-radiative` load failed at runtime (recorded precedent, re-confirmed); adjudication on file + artifact evidence. Report persisted by the delegating supervisor session.

## VERDICT: **NO-RE-DERIVATION — the applied C_rad table remains fully valid**

The chain data(reco) -> IBU(response) -> radiative-level gen cells (Q2esGen basis) -> C_rad(k) -> Born(k) is now **level-exact end to end on artifacts**, not just code. The OPT-A rebuild completed exactly the harmonization the prior adjudication prescribed (C.4 actions 1-2): body gen-Q2 cell -> calculator Q2esGen (pool/miss convention retained), gen fiducial stayed rad-safe, z/pT NOT switched (MC-2 respected). Zero C_rad re-derivation follows from Option A. The DJANGOH14 releasebin table (sha cf810be4..., re-verified byte-identical this session) applied to the OPT-A IBU output (sha b2603acc..., pin verified) is the exact correction for the adopted truth contract, up to the budgeted c_rad_gen_rel modeling term.

## 1. Contract re-verification on artifacts — CONFIRMED

**(a) OPT-A response gen booking vs C_rad worker legs — matched on every axis** (per-axis table below). Evidence: builder code (build_migration_th2_per_charge_cone_s2prime_optA.py L1100-1131 pool, L1449-1527 body), run_record_sub.json operand description, merge manifest meta_const/meta_int_sums, audit counters: n_opta_q2esgen_lookup_miss = 0 (both generators, HARD-FAIL gate); body-vs-pool mismatches 3/58,768,556 (rapgap31) and 0/69,695,815 (djangoh14), all certified benign pT-edge bit flips (|dpT| <= 1.7e-7 GeV at the 0.5 GeV edge, cell and iz agreeing; forensics sha 9955dc8d... re-hashed, matches pin); cell-change counters 1,357,250/1,574,230; no-pool-partner 41,390/49,878 (k_parented/STR legs, informational). Q2esGen event-stable (0 variation rows, recorded).

**(b) Application keying unaffected by the harmonization — CONFIRMED.** (charge, bin_index % 245) keyed merge (apply script L77-124); OPT-A IBU bin_index is charge-local 0-244 for both charges and global_bin_index = neg*245 + bin_index holds for all 490 rows (verified numerically); 7-pair edge asserts passed on all 490 rows; both merges validate="one_to_one"; OPT-A variant-id gate passed. The harmonization changed cell **content**, never grid, keying, or geometry.

**(c) Per-bin application well-defined in all 490 bins — CONFIRMED.** 490/490 finite_C_rad=true; all C_rad finite and positive; all OPT-A cross-sections finite and positive (0 empty cells, 0 NaN — no empty denominators anywhere, including the clean references); Born = raw x C_rad reproduced to 2.2e-16; totals h+ 11.997445909 / h- 11.664705534 nb (match summary); rel_delta_raw == rel_delta_born to 4e-16 in every row. Sub-unity tail bins owned **exactly once** (single post-unfold application; response contains no QED correction; identical z/pT constructions both sides). Census precision note: "35 sub-unity" = C_rad < 0.999 (exactly 35); strictly < 1 gives **36** — one marginal sparse corner bin (h+ bin 244, cell 6, z[0.7,1.0), pT[4,8), C_rad = 0.99935, rad_n = 1648). All sub-unity bins are high-pT (pT_min >= 2 GeV) tails of cells 1/3/5/6, h+ 21 / h- 15 — the documented structure, min 0.83937 at h+ bin 194 (cell 5, z[0.2,0.3), pT[4,8)), max 1.5315.

## 2. Per-axis level table (OPT-A response truth vs C_rad legs)

| Axis | Response (body / pool-miss) | C_rad worker legs | Verdict |
|---|---|---|---|
| Q2 cell operand | Q2esGen (body L1524; pool L1100-1101) | Q2esGen (worker L121 bq) | **EXACT** (the OPT-A harmonization itself) |
| x operand | x_es_gen (body L1453; pool L1104-1105) | x_es_gen (worker L121 bx) | **EXACT** — the hybrid (radiative Q2 cell, rad-safe x) is mirrored identically; no residual level mismatch |
| z | stored zGen, nominal-beam safeZ (body L1449/L1526; pool L1127/L1131) | computed massive-proton z at stored GenE, q_nominal (worker L138) | **EXACT** (identical q/frame; only difference is the safeZ proton-mass treatment, ~1e-6 relative, documented negligible; NOT switched — MC-2) |
| pT | _compute_pTh_cm_and_z: CM of W=q+P, massive proton (0.93827), pion-mass hadron (0.13957), nominal-beam q (body L1508; pool L1124) | observables(): identical construction (worker L50-60, L138) | **EXACT** |
| Selection (DIS) | rad-safe Q2_es_gen/Y_es_gen, [150,20000), (0.1,0.7) (body gevt_ok L1470-1477; pool evt_ok L1061-1068) | rad-safe Q2_es_gen/Y_es_gen (worker L120, L122) | **EXACT** |
| Empz | GenEmpz>45 (L1478/L1077; Empz_lo=45.0) | GenEmpz>45 (worker L133) | **EXACT** (convention CLOSED 2026-10-09) |
| xF | stored xFGen<0 (L1479/L1078) | recomputed safeXF(q_nom, stored GenE)<0 (worker L129/L136) | **EXACT** at contract level (0 sign disagreements, max |d| 8.4e-5; shared +0.039% nominal-beam-q bias retained by design) |
| Species | POOL_PDG=(211,321,2212) (L596) | |GenPDG| in {211,321,2212} (worker L125) | **EXACT** |
| Provenance (IsGtr/Mother2) | CTG fill-time IsGtr()+Mother2==-1 (central v6 builder L1401/L1406; documented SOURCE_POOL equivalence in the v0/isgtr study) | explicit IsGtr==1 & Mother2==-1 (worker L125) | **EXACT** by construction |
| V0 veto | IsV0DaughterGen==0 (pool L1092; matched-side stand-in L1493) | IsV0DaughterGen_v3==0 **&** VetoAncestorPDG_v3==0 (worker L124-125) | **EXACT** on the V0-daughter component; recorded nuance: worker additionally vetoes any non-V0 flagged ancestor (superset; expected-empty intersection with the GTR+Mother2==-1 pool; pre-existing, unchanged by OPT-A, previously adjudicated consistent). Not a mismatch; size quantification = Missing Evidence (optional) |
| Grid | binning_v3 CELLS/Z/PT edges, 245/charge | worker CELLS/ZED/PTED, c*35+zi*5+pi, 245/charge (L32-35, L83-89) | **EXACT** (identical edges/layout) |
| Charge offset | per-charge response; IBU global = neg*245+bin_index (all 490 rows verified) | worker glob = idx + (neg ? 245 : 0) (L141); table h- charge-major 245..489 with %245 remap (finalizer L36) | **EXACT** (one-to-one merge re-verified numerically) |
| Weights | w1*w2 both response sides | w1*w2 (worker L118) | **EXACT** |

**No residual level mismatch on any axis.**

## 3. Generator term — CONFIRMED unchanged, not amplified

Response is RAPGAP31 (detint_fake_response_variant_S2prime_alphaveto_optA_q2esgen_2026-10-19; prior = coherent body+miss from the OPT-A rapgap response, Q2esGen body binning — the IBU is anchored on radiative-level gen cells); C_rad is DJANGOH14. c_rad_gen_rel compares **two C_rad tables** (DJ14 releasebin vs RG31 releasebin emfix, V3-recipe sidecar) — independent of the response; OPT-A touched neither table (applied sha re-verified; emfix table verified). Re-verified from artifacts: emfix pair integrated agreement **+0.6419% (h+) / +0.6499% (h-)** (computed directly from the two tables); sidecar c_rad_gen_rel medians **0.5844% / 0.5804%** (max 5.7622%/5.6627%); both carried unchanged in the frozen env100 21-comp budget. c_rad_stat_rel = the applied table's relative_stat_unc: medians re-verified directly **0.4272% / 0.4429%**, identical in the env100 budget — **UNCHANGED**. The mixed-scheme (RG31 response / DJ14 C_rad) difference remains the budgeted modeling term, present identically under Option A. **NO-ACTION.**

## 4. M2 z-shift — NO new C_rad derivation for validity; systematic home required

Under Option A the nominal-beam z contract is exact (zGen response-side == worker nominal-beam z); the per-bin C_rad z-gradient (~1.05 low-z -> 1.53 max, verified from the table) owns the ISR z-shift exactly once; OPT-A did not switch z/pT. The 3.25% z-bin-row shift (100% upward toward rad-safe z; dominant 0->1: 465, 1->2: 289; concentrated z<0.05/0.05-0.1; single-slice rapgap31_7071_em06_0060) is the truth-**definition** difference, material per-z-bin, common-mode, **not covered by existing budget terms** (ibu_model_rel/c_rad_gen_rel blind to common-mode). Assessment: **not adequately carried by the existing budget terms alone**; resolvable without touching the applied table via (i) the documented nominal-beam truth-contract statement, (ii) full-sample per-z-bin quantification on the 490 published bins (can ride the same Condor pass already recommended for straddle counters/f_matched), (iii) a user decision on form: **caveat-only** (precedent: the 2026-10-09 xF-definition sensitivity decision, caveat-only, z<0.05 max 9.925%/8.023% DJ14) or **budget component**. If a component is chosen, the only sound derivation is a self-consistent scheme-**pair** diagnostic — a z_rad response variant (gen z at photon-subtracted q) **plus** a co-derived Born-z C_rad' (fork crad_v3_worker_releasebin.py: z from the photon-subtracted q construction on **both legs**, same grid/selection/exposures/beam-mix, RG31 twin for the pair), differenced per bin against the nominal scheme — **never** a z_rad response with the current table (z-migration double-count at the 3.25%-shift strength, adjudication B.2/MC-2). Systematic-leg only; the applied table is not replaced.

## 5. Sweep of other RC-chain items touched by OPT-A

| Item | Finding | Label |
|---|---|---|
| Beam-mixing validity (mixed-beam-only trap) | Unchanged: C_rad mixed-beam by y (finalizer mix_by_y, ep 0.5141/em 0.4859); OPT-A response/IBU/data all mixed-beam; per-beam/e- Born targets remain out of scope; blocker carried in docs/status/radiative-corrections.md | **NO-ACTION** (status quo carried; OPT-A creates no per-beam demand) |
| 2026-10-09 EMPZ-convention closure | Verified both sides (worker empz_gt45 + config metadata; response Empz_lo=45.0, gempz_ok); CLOSED; OPT-A explicitly froze it (builder L29-30, run record) | **NO-ACTION** |
| xF shared-bias decision | Worker recomputes safeXF(q_nom, stored GenE), deliberately retaining the shared +0.039% bias (worker docstring L9-15); response stored xFGen<0; sign-identical; unchanged | **NO-ACTION** |
| Q2Gki>100 domain cut | RAD-leg-only (~0.09% in-fiducial; worker L115/L119; nonrad partial cutflow confirms after_rad_domain == input_rows — no cut on nonrad); domain-matching to the NONRAD Q2GT100 generation domain; unchanged | **NO-ACTION** |
| Stale summary-JSON labels | Verified present in the artifact (crad_v3_summary.json: c_rad_applied: False, PROVISIONAL_DIAGNOSTIC, xFcut-fork contract string, DIAGNOSTIC_MANIFEST_LEVEL), hardcoded at finalizer L59 and worker L143/L147/L154. Values in the same JSON are correct (integrated re-verified). Live applied status is authoritative only in promotion records/skill/STATE | **NEEDS-ACTION (hygiene):** supersession annotation (adjudication C.4 action 5) + directory-qualified quote enforcement. Provenance hygiene only |
| Skill wording discrepancy | SKILL.md L12: "150 < Q2sGen < 20000, 0.1 < YsGen < 0.7" — actual selection is rad-safe Q2_es_gen/Y_es_gen (worker L120/L122; worker metadata and partial diag correct); Q2sGen/YsGen used only in the sigma_photon q-variant (L128). L12 also understates the worker's ancestor veto (superset nuance, axis table) | **NEEDS-ACTION (hygiene):** wording fix via the skills maintenance path (runtime load currently fails — recorded precedent) |
| OPT-A adoption conditions (chain hygiene, supervisor-owned; re-confirmed) | RU-2 f_i removal-window re-derivation (disabled; measurement 0.11280/0.10907 recorded, in old window informationally); two-stage SHA re-pin (spec response shas TO-BE-ESTABLISHED); registry discipline — RUNLIST.md / SEVEN_CATEGORY_SCRIPT_INDEX.md override entries; **STATE.md has NO OPT-A entry yet (grep-verified)**; budget regeneration on the OPT-A nominal (56 component-exceeding cells + straddle-residual home; c_rad_gen_rel/c_rad_stat_rel carry over **unchanged**, values verified) | **NEEDS-ACTION (blocking for OPT-A promotion, not C_rad items)** |
| NoZ C_gamma_born | Unchanged, PROVISIONAL DIAGNOSTIC, unapplied | **NO-ACTION** (status quo) |

## Complete action list

**Blocking-for-quoting (none is a C_rad re-derivation):**
1. **Truth-contract statement** in the radiative docs (adjudication C.4 action 4): C_rad derived and applied at (Q2esGen, x_es_gen) cell binning, rad-safe selection (Q2_es_gen/Y_es_gen/x_es_gen + GenEmpz>45 + safeXF(q_nom)<0), nominal-beam z/pT; response truth identical (cite worker L120-121, L128, L138, L147); OPT-A body harmonization recorded as the completing step. — blocking-for-quoting, documentation.
2. **M2 z-shift systematic home:** full-sample per-z-bin quantification (Condor; rides the straddle/f_matched pass) + user decision caveat-only vs budget component (if component: the self-consistent z_rad-response + Born-z C_rad' pair, spec in section 4, diagnostic-only). — blocking-for-quoting of per-z-bin values.
3. **OPT-A adoption conditions** (supervisor-owned, from the opta adjudication): budget regeneration on the OPT-A nominal (C_rad components carried unchanged), RU-2 re-derivation, SHA re-pin, RUNLIST/SEVEN_CATEGORY/STATE override entries. — blocking for OPT-A promotion to release.

**Hygiene:**
4. Annotate the releasebin/xFcut summary-JSON stale labels as superseded by promotion records; enforce directory-qualified C_rad quotes.
5. Fix SKILL.md L12 selection wording (Q2sGen/YsGen -> Q2_es_gen/Y_es_gen; state the ancestor-veto superset precisely).
6. Sub-unity census wording: quote "35 bins with C_rad < 0.999" (36 strictly < 1, incl. one marginal sparse corner bin h+ 244, C_rad 0.99935) if numerically cited.
7. Optional: quantify the non-V0-ancestor row fraction (worker-veto superset vs response pool) once in the full-sample pass.

## Missing Evidence

1. **Full-sample per-cell straddle rates and f_matched** — single-slice only; decides definitive per-cell magnitudes (not needed for the C_rad validity verdict; already recorded by the opta adjudication).
2. **Full-sample per-z-bin z-shift rates on the published grid** — single-slice 3.25% only; required for action 2.
3. **Non-V0-ancestor (conv/Dalitz) row fraction** among GTR+Mother2==-1 pi/K/p — the worker-veto-superset nuance; expected ~0, previously adjudicated consistent; optional closure.
4. **Source-location note (provenance, not missing physics evidence):** the task-cited canonical path holds only RC_treebuilder_genonly_v1.C; the authoritative RC_treebuilder_genonly_v3.C lives in the study tree (studies/radiative_correction_genonly_v1/src/, sha deef89f6... verified against the provenance doc), and the applied table's own worker/finalizer are archived beside the tables in data/radiative/djangoh14_crad_releasebin_2026-10-09/scripts/ — these archived copies are the ones verified (consistent with the prior adjudication's basis). Response-vs-worker pT/z agreement is construction-exact, not bitwise; the 3 certified pT-edge flips (<=1.7e-7 GeV) quantify the scale with no demonstrated bin-content effect.

--- audit metadata ---
- Auditor: radiative-correction specialist, read-only; no analysis script modified.
- Persisted by the delegating supervisor session, 2026-10-20.
- Companion reports: crad_truth_contract_adjudication.md (pre-rebuild contract), opta_diff_consistency_adjudication.md (rebuild impact).

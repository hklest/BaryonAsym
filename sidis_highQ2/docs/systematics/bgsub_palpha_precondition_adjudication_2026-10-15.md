# P-ALPHA bgsub Set R′ cone-base chain — precondition adjudication for resume (2026-10-15)

**Task:** READ-ONLY adjudication of the §H production preconditions (P1–P5) for
resuming and finishing the P-ALPHA background-subtraction Set R′ chain
(cone-basis, v6/z005 high-Q² inclusive charged-hadron SIDIS, Q² > 150 GeV²),
in the post-2026-10-14 reality where a parallel shifted-legs campaign delivered
full-chain detector products and a LEG-ENVELOPE 15-comp budget that the user
INSTALLED as canonical.

**Status:** ADJUDICATION ONLY. No production was launched, no Condor jobs were
submitted, no campaign tree / release / central artifact was modified. All
computations reported below were performed read-only (JSON/CSV parsing,
per-bin comparisons, sha256 and identity checks on existing files). This
document is the only write.

**Governing documents (read first, per dispatch):**

- Spec: `sidis_highQ2/docs/systematics/BGSUB_S2PRIME_REBASE_ADDENDUM_2026-10-11.md`
  Rev 3 + §H (lines 567–972; §H.2 ≈ 643–693, §H.3 ≈ 695–743, §H.4 ≈ 745–782,
  §H.5/§H.5-bis ≈ 784–880, §H.6 ≈ 882–924, §H.7 ≈ 926–971; Rev-3 §C ≈ 299–372,
  §D ≈ 374–445, §E ≈ 447–487, §F ≈ 489–524, §G ≈ 526–565).
- JSON v4: `sidis_highQ2/docs/systematics/bgsub_s2prime_rebase_spec_2026-10-11.json`
  (schema `…_v4`; `production_preconditions_h` with `pinned_constants`).
- Parent scheme: `BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md` +
  `bgsub_scheme_spec_2026-10-10.json` (same directory).
- Campaign ledger: `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/RUN_SHEET.md`
  (P0 COMPLETE 2026-10-12; P1–P5 PENDING) + `cone_15comp/P0_RECORD.md`.
- Parallel campaign: `H1ChargedSidis_HighQ2/output_v6/studies/cone_s2prime_shiftedlegs_2026-10-14/`
  (plus/minus legs, `sign_symmetry_record_2026-10-14.md`, three budget trees).
- Canonical installs: RAW
  `output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_cone_…_2026-10-13/`,
  Born `born_crad_dj14relbin_detints2prime_alphaveto_cone_…_2026-10-13/`, budget
  `systematics_cone_s2prime_15comp_shiftedlegs_legenv_…_2026-10-14/`
  (validation PASS 29/29, 0 violations; install record
  `INSTALL_RECORD_shiftedlegs_legenv_budget_2026-10-14.json`); STATE.md /
  RUN_STATUS.md 2026-10-14 cone-baseline promotion entries.
- Critic gate: `sidis_highQ2/docs/critiques/critic-gate-bgsub-promotion-2026-10-11.md`
  (FATAL F-1/F-2 vs the pre-S2′ no-cone Set-R candidate; stands).

---

## 1. Headline verdict table

| Stage | §H content | Verdict | Remaining Condor jobs |
|---|---|---|---|
| P1 | 5 cone shifted DVs + zero-shift anchor (6 jobs, §H.2) | **SATISFIED-BY-EXISTING** (shiftedlegs plus-leg cone DVs) **with convention flags**; §H.2 numeric acceptances verified read-only ALL PASS; re-run under §H.2 conventions would produce divergent, unconsumed products — **MUST-NOT-RUN as specced** | 0 (optional zero-shift anchor: 1) |
| P2 | tracking-drop cone port + 16 seeds + P=0 anchor (17 jobs, §H.3) | **NOT REQUIRED for the chain** — no consumer once P4 is superseded; §H.3 was never executed anywhere; the canonical budget carries the legacy-ensemble differenced data-side tracking component (caveat OPEN, user-recorded). Re-derivation is a separate user-gated budget-successor campaign, NOT a P-ALPHA precondition | 0 (user-gated option: 17) |
| P3 | cone 1000-toy stat sidecar (2 jobs + assembler, §H.4) | **SATISFIED-BY-EXISTING** — the canonical 2026-10-13 cone RAW release carries an installed `stat_sidecar_1000toy/` (reused G5 gate-suite 1000-toy product, seeds 12345/22345, 1000 toys/charge, IBU k=4); §H.4 acceptance identities verified read-only EXACT (0.0 dev); §H.4-as-written deviations flagged (§5) | 0 (optional bitwise re-run: 1) |
| P4 | cone 15-comp budget build (1 job, 31 gates, §H.5) | **MUST-NOT-RUN as specced — SUPERSEDED** by the installed LEG-ENVELOPE canonical budget (user decision 2026-10-14). A §H.5 data-side build would produce a divergent second 15-comp package (elec_energy ~4.6% data-side vs 1.48/1.70% full-chain canonical) and break CP-5 byte-identity against the canonical. The legenv package substitutes as the CP-5 parent; the P0 extender edition CANNOT consume it as-is → **a dated extender re-edition is REQUIRED** (engineering, 0 Condor jobs) | 0 |
| P5 | Stage-2 Set R′ chain (Rev-3 §C/§G, 26-item gate) | **MUST-RUN** — unchanged prerequisite chain, all inputs exist and are sha-pinned; anchors UNCHANGED; spec repoints needed (extender parent, CP-5 reference, release operands → canonical 2026-10-13, PM-2 resolution) | **2** (1 smoke-one throwaway + 1 official chain; login-node certification steps ≤ 8 s each per pre-S2′ precedent) |

**Total remaining REQUIRED Condor jobs: 2** (both P5). Optional/validation-debt
jobs, none required for the chain: zero-shift anchor (1), sidecar one-charge
bitwise re-run (1), §H.3 cone tracking campaign (17, user-gated budget-successor
work).

---

## 2. What changed since §H was written (the adjudication basis)

§H (2026-10-11) prescribed the cone 15-comp budget production under the
**data-side convention** (5 cone shifted DVs unfolded through the single
nominal cone variant response; 16 cone seed DVs; fresh 1000-toy sidecar;
31-gate builder). At that time no cone shifted DVs, no cone tracking seeds,
and no cone sidecar existed, and the 2026-10-13 promotion-time cone budget had
to use the DIFFERENCED data-side form with the explicit OPEN follow-up
"shifted-response cone productions would restore the central full-chain
convention on the cone nominal (post-promotion)"
(`systematics_cone_s2prime_15comp_…_2026-10-13/README.md` §"Detector/tracking
DIFFERENCED data-side convention").

The parallel `cone_s2prime_shiftedlegs_2026-10-14` campaign then EXECUTED
exactly that follow-up for the five detector components: per-leg own shifted
cone response + own coherent body+miss prior + own f_i + own shifted cone DV
(shift→cone order), stage-3 IBU per leg, RAPGAP31 governing, FULL ABS
single-sided; plus the two minus legs and the sign-symmetry study; plus the
critic-pinned LEG-LEVEL ENVELOPE successor. The user INSTALLED the legenv
budget as canonical (STATE.md / RUN_STATUS.md 2026-10-14; validation 29/29;
`total_with_cone15_rel` medians h+ 5.8032% / h− 6.1919%; csv sha256
`568040fefb868680faef9aeebdfc0767b8dfc9083d7a47c24ce950b8ae5f076a`), with the
explicit caveat scoping: convention caveat CLOSED for the five detector
components ONLY; `tracking_eff_m0p5_rel` REMAINS data-side conservative
(0.49%), caveat OPEN.

Consequence: the §H detector-component convention (data-side, single nominal
response) and the §H.1 sanity windows calibrated on data-side medians are
SUPERSEDED for the five detector components by a recorded user decision. The
§H P1/P2/P3 productions fed exactly one consumer — the §H.5 P4 builder —
whose output would now CONFLICT with the installed canonical. The adjudication
below therefore closes P1/P3 as satisfied-by-existing, P2 as not-required,
P4 as superseded, and routes all remaining work to P5 with a small set of
engineering repoints.

---

## 3. Q1 — §H P1 (5 cone shifted DVs + zero-shift anchor): SATISFIED-BY-EXISTING

**Products found (shiftedlegs campaign, 5 plus legs):**
`output_v6/studies/cone_s2prime_shiftedlegs_2026-10-14/data_vector/data_subtracted_v6_REAL_LUMI_filtered_alphaveto_cone_{hfs_jes_down,hfs_rces_up,hfs_theta_up,elecE_p1pct,elecTheta_p1mrad}.json`
(+ perfile trees, 660 files per HFS leg / 658 per electron leg; per-leg
`dv_campaign_validation_*.json`, 23–25 checks each, 0 failed). Provenance:
plus-leg clusters (shards 5615905–5615914; DV perfile 5615916/5615918/5615921/
5615923/5615925; DV merged 5615917/5615920/5615922/5615924/5615926; merges
5616571; stage-3 IBU 5618814; budget assembly 5618995) — legenv README
"Provenance".

**§H.2 pinned-input cross-check (JSON v4 `shifted_dv_campaign` +
`pinned_constants`):**

| §H.2 pin | Shiftedlegs reality | Verdict |
|---|---|---|
| HFS legs: 660 legacy shifted sidistrees per tag, reused byte-identical, cone-ported `build_data_vector_v4.py` | 660 input files per tag opened/0 failed (`dv_campaign_validation_hfs_jes_down.json`: `merged_n_input_files` 660), cone applied at admission, alpha identity holds | MATCH (semantics identical; builder is the campaign's own copy, same cone port semantics) |
| Electron legs: same 658 central sidistree files, post-hoc shift, cone from **RAW (UNSHIFTED)** reco branches — "the cone rejection set is IDENTICAL across all shift legs" | 658 files, post-hoc shift, but **shift→cone on the SHIFTED direction** (decision (a); validation item `merged_cone_shift_order`; alpha-rejected counts differ per leg, e.g. elecE_p1pct 14,512/14,083 vs nominal 14,465/14,117) | **CONVENTION CONFLICT** for the 2 electron legs (see §8, flag C-1) |
| Governing generator | RAPGAP31 governs (data-side columns are cone-RAPGAP-chain); DJ14 pure-DJ diagnostic | Same governing generator; construction differs (full-chain per-leg responses vs single nominal response — flag C-2) |
| DV builder identity: §H dated copies in `cone_15comp/scripts/` | Campaign's own shifted-DV builder (schema `data_vector_v4_shifted_native_fine_reco_raw_no_bgsub`, stamps `photon_mode: shifted_electron`, variant `cone_s2prime_shiftedlegs_2026-10-14`) | Different script copy; HFS-leg semantics equivalent, electron-leg semantics differ via C-1 |
| Zero-shift anchor leg (REQUIRED, 1 job; per-bin BIT-IDENTICAL to `data_vector_cone_alpha_central_2026-10-11.json`) | **NOT PRODUCED** by the campaign | Gap (flag C-3); purpose served by other validation (below) |

**§H.2 numeric acceptances (a)–(d), verified READ-ONLY 2026-10-15** against
the legacy no-cone shifted DVs
(`output_v6/systematics/{hfs,electron}/<tag>/tunfold_official_v4_finalcandidate_…/data_vector_*.json`)
and the JSON v4 pins (legacy sums reproduced EXACTLY for all 5 legs; cone
nominal 1,965,700/1,899,373; no-cone nominal 1,975,301/1,908,796):

| Leg | (b) subset vs legacy (980 rows) | (c) cone-removed % pos/neg (window [0.35, 0.65]) | (d) cone Δ% pos/neg vs legacy Δ% (dev pp; tol ±0.05) |
|---|---|---|---|
| hfs_jes_down | 0 violations — PASS | 0.4867 / 0.4935 — PASS | +0.0391/+0.0518 vs +0.0397/+0.0516 (0.0006/0.0002) — PASS |
| hfs_rces_up | 0 violations — PASS | 0.4863 / 0.4935 — PASS | +0.3005/+0.3088 vs +0.3007/+0.3086 (0.0002/0.0002) — PASS |
| hfs_theta_up | 0 violations — PASS | 0.4865 / 0.4933 — PASS | +0.3032/+0.3109 vs +0.3036/+0.3106 (0.0004/0.0003) — PASS |
| elecE_p1pct | 0 violations — PASS | 0.4837 / 0.4899 — PASS | +1.0771/+1.1150 vs +1.0747/+1.1112 (0.0024/0.0038) — PASS |
| elecTheta_p1mrad | 0 violations — PASS | 0.4862 / 0.4941 — PASS | +0.0615/+0.0551 vs +0.0616/+0.0555 (0.0001/0.0004) — PASS |

All five legs pass all three numeric §H.2 acceptances despite the electron-leg
cone-semantics difference (the removed fractions sit on the §H.7 per-leg
window centers). Schema acceptance (a) — 980 rows, charge-major, cone stamps,
alpha-rejection counters > 0 — is covered by the campaign's own 23–25-check
validations (all PASS).

**Zero-shift-anchor gap (C-3) — mitigating evidence (read-only):**
- The shiftedlegs stage-3 `nominal_reproduction` gate
  (`ibu_shifted/stage3_ibu_manifest.json`) pins the CANONICAL cone release
  manifest/tarball shas (`a7ee02c3…`/`f093a74d…`) and reproduces the cone
  release CSV/npz at ≤ 1.5e-15 with integrated values EXACT
  (11.41223261552874 / 11.123626367451845 nb) — the campaign's machinery
  reproduces the nominal cone chain bitwise through its own code path.
- `data_vector_cone_alpha_central_2026-10-11.json` == the
  `cone_s2prime_2026-10-11` nominal DV per-bin EXACT (verified read-only,
  980/980) — the nominal objects the campaign consumed are the campaign cone DV.

**Verdict P1: SATISFIED-BY-EXISTING; MUST-NOT-RUN as specced.** Re-running the
electron legs under the §H.2 cone-from-RAW convention would produce shifted
DVs INCONSISTENT with the installed canonical budget's shift→cone full-chain
legs (the exact objects its five detector components were built from), with no
remaining consumer (the §H.5 data-side builder is superseded, §6). The
zero-shift anchor is validation debt, not a physics precondition; if the
supervisor wants the artifact, it is 1 Condor job through the P0-dated
`build_shifted_response_data_vector_s2prime_v4.py --shift zero` with
bit-identity acceptance to the campaign cone DV (OPTIONAL).

---

## 4. Q2 — §H P2 (tracking-drop cone port + 16 seeds + P=0 anchor): NOT REQUIRED for the chain

**Confirmed: no tracking-drop ensemble exists anywhere on the cone base.** The
shiftedlegs campaign produced no seed legs; the canonical budget lineage is:
2026-10-13 cone budget tracking = "16-seed ensemble mean vs no-cone baseline
leg (differenced data-side)" (medians 0.4937/0.4984%) — i.e. computed from the
LEGACY no-cone seed DVs (seeds 314159–314174); legenv budget carries that
column bit-identical ("13 components … string-verbatim from the installed
CSV"). RUN_STATUS.md 2026-10-14 records the caveat explicitly: "tracking
REMAINS data-side conservative 0.49%, caveat OPEN for tracking."

**Why P2 is NOT a chain precondition:** its only consumer is the §H.5 P4
builder (gate #3 SEED_PATHS from the cone `ensemble_manifest.json`; gate #21
seed-ensemble component) — superseded (§6). The P5 extender consumes the
canonical 15-comp parent, whose tracking column already exists. Running §H.3
now would produce a NEW cone tracking component that is NOT in the canonical
package; folding it would require a new budget successor + reinstall + a fresh
user promotion decision — budget-successor work, explicitly out of P-ALPHA
scope. The §H.1 sanity window for tracking is satisfied by the carried value
(0.4937/0.4984% vs window [0.368, 0.614]/[0.370, 0.616], verified read-only).

**§H.3 exact pinned inputs (recorded for the optional future campaign; JSON v4
`tracking_drop_port` + `pinned_constants`):**
- Port: merge parent A
  `output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/scripts/build_data_vector_v4.py`
  (randdrop patch: `H1_RANDDROP_P=0.005`, `H1_RANDDROP_SEED_TAG=seed<N>`,
  `make_keep_mask` on valid rows, `n_reco_dropped_randdrop` counters,
  `.randdrop_audit.json` sidecar) with parent B (campaign cone-ported
  `copied_scripts/build_data_vector_v4.py`: `ELECTRON_ALPHA_VETO_RAD=0.2`, NaN
  alpha rejected, cone at admission BEFORE the z-guard, cone QA counters);
  disjoint code sites; drop FIRST, cone at admission AFTER. **The merged port
  already exists and is self-tested** (`cone_15comp/scripts/build_data_vector_v4_cone_randdrop.py`;
  P0 RECORD §d: P=0 single-file smoke, per-bin bit-identity re-verified
  2026-10-12).
- Campaign: 16 jobs, seeds 314159–314174 (legacy set reused EXACTLY), each over
  the SAME 658-file list (`manifests/data_all.txt`), outputs
  `tracking_cone/data_vector_cone_seed<N>_all4p.json` (+ qa + randdrop_audit),
  label `tracking_eff_m0p5_randdrop_seed<N>_cone_data4p_prod_data` (~8 GB,
  workday flavour); PLUS 1 P=0 anchor job (`H1_RANDDROP_P=0.0`, per-bin
  BIT-IDENTICAL to the campaign cone DV); packaging writes
  `tracking_cone/ensemble_manifest.json` (schema mirror of
  `tracking_ensemble_coherentprior_2026-10-07/ensemble_manifest.json`); NO
  re-unfold campaign (data-side convention computes in-machinery).
- Acceptance per seed: (a) schema/stamps + randdrop_audit with
  `n_reco_dropped_randdrop` ≈ 0.5% of pre-drop (legacy DV-level drop fractions
  0.49276%/0.49349%); (b) per-bin subset vs the legacy seed DV (EXACT, 980
  rows); (c) removed fraction vs legacy within [0.35, 0.65]% (expected ≈
  0.4837% pos / 0.4912% neg); (d) campaign-level P=0 anchor bit-identity.
- Pinned references (JSON v4): per-seed legacy sums (16 seeds; e.g. 314159
  1,965,606/1,899,622 … 314174 1,966,236/1,899,897); ensemble means
  1,965,567.4375/1,899,376.3125; nominal cone removal 9,601/9,423 in-grid =
  0.4861%/0.4937% of the no-cone nominal.

**Verdict P2: NOT REQUIRED for finishing the P-ALPHA chain (0 jobs).**
MUST-RUN only inside the superseded §H.5 P4 route, or as a separate user-gated
budget-successor campaign (17 jobs) with its own reinstall decision. The OPEN
tracking caveat is carried, not hidden, in the canonical records.

---

## 5. Q3 — §H P3 (cone 1000-toy stat sidecar): SATISFIED-BY-EXISTING

**What exists:** the CANONICAL 2026-10-13 cone RAW release carries
`stat_sidecar_1000toy/` (covariance JSONs + plots +
`stat_component/ibu_stat_rel_{pos,neg}.csv` + `manifest_stat_sidecar.json` +
README). Mode: **REUSED gate-suite G5 1000-toy closure product** (cluster
5550195; source `studies/cone_s2prime_2026-10-11/validation/toys`), staged
additively 2026-10-13 with the original release manifest/tarball recorded
unchanged (`a7ee02c3…`/`f093a74d…`, 27-file manifest untouched). Protocol:
1000 toys/charge, seeds pos 12345 / neg 22345, IBU k=4, independent Poisson on
the rounded cone vector, fixed cone f_i/response/prior — identical protocol to
§H.4's "unchanged" list. The §H.4 sidecar's only consumer is the 15-comp
budget's `ibu_stat_rel` (§H.1 component #1) and the §H.5 gate #15 — the
canonical budget lineage (2026-10-13 → shiftedlegs → legenv) consumed exactly
this sidecar ("1000-toy sidecar on the cone chain (data-stat only)").

**§H.4 acceptance identities, verified READ-ONLY 2026-10-15** (both charges):
- `stat_component` CSVs: 245 rows each, columns
  `charge,bin_index,nominal_yield_coherent,toy_stat_unc_yield,ibu_stat_rel_new,ibu_stat_rel_old`.
- `nominal_yield_coherent` vs the cone release `ibu/cross_sections.json`
  `yield_unfolded`: max rel dev **0.00e+00** (§H.4 tol ≤ 1e-9) — PASS.
- `toy_stat_unc_yield / nominal_yield_coherent == ibu_stat_rel_new`: max rel
  dev **0.00e+00** (§H.4 tol ≤ 1e-12) — PASS.

**§H.4-as-written deviations (flagged, none forcing a re-run):**

| §H.4 requirement | Reality | Disposition |
|---|---|---|
| Fresh campaign: dated trio copies, 2 Condor jobs + assembler, repointed at the cone chain | Reuse of the G5 gate-suite 1000-toy product (cluster 5550195) | §H.4 delta (3) DROPPED the precedent's bit-level cross-check because "no cone gate-suite toy product exists" — that product NOW EXISTS and is the source; the reuse is protocol-identical and G5-validated (stronger provenance than a fresh un-cross-checked campaign) |
| Install ADDITIVE at the STUDY-TREE cone RAW release (manifest `6ca6b4a1…`, tarball `b16424f0…` unchanged pre/post) | Installed at the CANONICAL 2026-10-13 release (manifest `a7ee02c3…`, tarball `f093a74d…` recorded unchanged) | Install-location supersession; the study-tree release (which indeed has NO sidecar dir) is retained provenance |
| Closure re-check: "n\|z\|>3 ≤ 4 is the stop condition; a count > 4 STOPS the campaign before install" | Realized closure: pos n\|z\|>3 = 1; **neg n\|z\|>3 = 5** (n\|z\|>4 = 1; z_absmax 4.182 < 4.5) — adjudicated under the G5 pinned criteria (hard bound \|z\|max < 4.5; pinned frac windows pass; neg-leg edge watch REPORTED per the pinned `g5_watch` rule; `stop_condition_hit: false`; verdict "PASS with neg-leg edge watch REPORTED") | A §H.4-fresh campaign would have STOPPED at the neg-leg count; the installed sidecar instead passed under the gate-suite G5 rule set with the edge watch recorded. Spec-vs-reality divergence to be recorded, not re-run — the product's physics content (per-bin toy stat) is unaffected |
| Deterministic reproduction: one-charge re-run must reproduce the CSV BITWISE | Not executed (reuse mode) | Residual validation debt; 1 optional Condor job if the supervisor wants it |

**Does §H.4 require a campaign-specific sidecar?** For the P-ALPHA chain: NO.
The `ibu_stat_rel` column is already inside the canonical 15-comp parent the
P5 extender consumes, and its §H.4 identities hold EXACTLY. A fresh §H.4
campaign would change nothing in the P5 chain. Note the P5 candidate's OWN
statistical lineage (1000-toy Poisson on `data_sub′` through the cone variant
f_i/response/prior, Rev-3 §C.5/§G) is a P5-internal production inside the
official chain job (pre-S2′ precedent: full-scope 1000-toy re-unfold in the
single official job) — it is NOT the §H.4 sidecar and is unaffected by this
verdict.

**Verdict P3: SATISFIED-BY-EXISTING (0 required jobs).**

---

## 6. Q4 — §H P4 (cone 15-comp budget build, 31 gates): SUPERSEDED — MUST-NOT-RUN as specced; extender re-edition REQUIRED

**Can the installed LEG-ENVELOPE budget substitute? YES — it IS the cone
15-comp package now.** It is user-installed canonical (STATE.md/RUN_STATUS.md
2026-10-14; validation PASS 29/29, 0 violations; physical bounds 490/490;
22/22 files sha-verified at install), and CP-5's reference ("the 15 existing
components byte-identical to the CONE 15-comp package") must resolve to it.

**Why the §H.5 build must NOT run:** it would re-derive the five detector
components under the superseded data-side convention and produce a SECOND,
DIVERGENT 15-comp package. Quantified (read-only, 2026-10-15; legenv medians
vs the §H.1 ±25% windows around the no-cone S2′ data-side medians):

| Component (h+/h−) | legenv median % | §H.1 window % | Result |
|---|---|---|---|
| elec_energy_rel | 1.4820 / 1.6971 | [3.4192, 5.6988] / [3.6317, 6.0527] | **OUTSIDE** — the documented full-chain/data-side ratio for elecE is 2.61× (2026-10-13 budget README); a §H.5 build would give ≈ 4.6% (the superseded 2026-10-13 value 4.5905/4.8863%), i.e. two canonical-claiming packages differing by ~3× on the dominant detector component |
| detint_removal_rel | 0.0386 / 0.0340 | [0.0222, 0.0370] / [0.0211, 0.0352] | h+ marginally outside (+4.3% over the +25% bound) — the cone new-source re-derivation recorded in the 2026-10-13 budget; carried into legenv |
| All other 13 components (27 component-charge cells) | — | — | PASS (verified read-only; incl. tracking 0.4937/0.4984, ibu_stat 3.6314/4.0114, phi_h 1.0522/1.0550) |

Running P4 would therefore CREATE the exact conflict (divergent packages,
broken CP-5 byte-identity against the canonical) that the gate exists to
prevent. The 31 §H.5 gates' unique machinery validations are otherwise
covered: the §H.7 pinned operands (G9/G12 cone internals, variant release
reproduction ≤ 1.8e-15, leg anchors) were pre-validated read-only at spec time
and are enforced by the cone gate suite (G9′/C5/G12/G13, 18/18 PASS) and the
shiftedlegs nominal-reproduction gate; the canonical budget lineage carries
its own 29/29 construction validations at each successor step.

**Can the P0 extender edition consume the legenv budget? NOT as-is — a dated
re-edition is REQUIRED.** Verified read-only against
`cone_15comp/scripts/extend_budget_15to19_conebase_v1.py`:

- `PKG_CSV` is hard-pinned to
  `bgsub_setr_cone_build_2026-10-11/cone_15comp/systematics_package/total_uncertainty_summary_s2prime_variant_15comp.csv`
  (the §H.5 P4 output, which will now never exist).
- The parent `FULL_HEADER` is asserted name-by-name (43 names) and differs
  from the legenv CSV header in exactly **6 columns**:
  `tracking_eff_m0p5_source_abs_nb` → `tracking_eff_m0p5_source_abs_nb_central_campaign`,
  `tracking_eff_m0p5_source_nominal_nb` → `tracking_eff_m0p5_source_nominal_nb_central_campaign`,
  `total_with_s2prime15_rel` → `total_with_cone15_rel`,
  `total_with_s2prime15_percent` → `total_with_cone15_percent`,
  `total_s2prime15_increment_rel` → `total_cone15_increment_rel`,
  `total_s2prime15_increment_percent_points` → `total_cone15_increment_percent_points`.
- Everything else is structurally compatible (verified read-only): the legenv
  CSV is CRLF, 491 lines, 43 fields/row, 490 rows keyed (charge ∈ {pos, neg},
  bin_index 0–244) — the extender's 491-line/CRLF/43-field/key asserts would
  hold; `FIFTEEN_COMPONENTS` names all exist in the legenv header; the
  byte-carry would be 43×490 = 21,070 cells as designed; the pinned
  `CONE_NPZ` sha `736fae14…` verified against
  `cone_alpha_central_build_2026-10-11/production/migration_s2prime_cone_rapgap31_2026-10-11.npz`.

**Required re-edition (dated new copy; the P0 edition is never modified):**
(1) repoint `PKG_CSV` to the installed canonical
`output_v6/central/data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/total_uncertainty_summary_cone_variant_15comp.csv`
and sha256-pin it (`568040fefb868680faef9aeebdfc0767b8dfc9083d7a47c24ce950b8ae5f076a`);
(2) apply the 6 header renames in `FULL_HEADER` (the 19-comp outputs keep the
`total_with_bgsub19_*` naming; the parent-derived columns carry the `_cone15_`
names); (3) re-run the extender self-test harness (synthetic dry-run through
`main()` with the real pinned npz; byte-carry, quadrature ≤ 1e-12, §C.1/SA-2
sums, RU-2 windows, driver-record cross-checks); (4) record the supersession
of the §H.5-bis "parent = P4 PKG_CSV" clause and the void AST-check against
the superseded P4 builder. This is a script edit + self-test (login node),
0 Condor jobs.

**Verdict P4: MUST-NOT-RUN as specced (superseded); the installed legenv
budget substitutes as the CP-5 parent; a dated extender re-edition is the
required enabling action.**

---

## 7. Q5 — P5 (Stage-2 Set R′ chain): MUST-RUN, prerequisites ready, anchors UNCHANGED

**Prerequisite chain after this adjudication:** P1 satisfied (§3) → P2 waived
(§4) → P3 satisfied (§5) → P4 superseded with the legenv parent + re-edited
extender (§6) → **P5 is UNBLOCKED** once the extender re-edition exists.

**Inputs verified to exist (read-only):** packaged cone-applied electron
templates
(`reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/AGGREGATED_{rapgap31,djangoh14}_electron_alpha0p2_combinedL_L351000_finalcandidate.npz`);
V0/lowQ2 B_R vectors
(`bgsub_setr_build_2026-10-10/vectors/bgsub_setr_vectors_{980,490}.csv`);
cone data vector
(`cone_alpha_central_build_2026-10-11/production/data_vector_cone_alpha_central_2026-10-11.json`,
in-grid 3,865,073 = 1,965,700/1,899,373); cone S2′ variant responses
(`migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.{root,npz}`; RG sha
`fda5466f…`, DJ sha `c84a8c2d…`, npz `736fae14…` verified); cone coherent
manifests/priors (Phase-D, from the cone variant response); f_i crosschecks;
C_rad releasebin DJ14 table (sha `205e8218…` ledger); canonical cone RAW and
Born releases (2026-10-13); canonical legenv budget (parent); installed
sidecar; Stage-2 script set
(`sidis_highQ2/scripts/systematics/bgsub_setr_build_2026-10-10/`: vector
builder, re-unfold driver, packager, stage-rel measurement, gate runner — to
be run from dated cone-edition copies with the Rev-3 §C/§F repoints already
specified in §H.5's Stage-2 machinery note).

**Anchor checks — CONFIRMED UNCHANGED (Rev-3 §D; JSON v4 `expected_anchors`;
RUN_SHEET):**

| Anchor | Value | Gate |
|---|---|---|
| B_R′_cone totals | h+ 140,912.91681341422 / h− 140,337.17473977088 / **281,250.09155318514 = 7.2767% of D_cone** | SA-2 |
| data_sub′ sums | h+ 1,824,787.4209930277 / h− 1,759,035.953778433 | SA-2 |
| Integrated f_res′ on D_cone | 7.1686% / 7.3886% (asym −0.2200 pp ± 0.02) | SA-5 |
| Post-subtraction RAW IBU dilution anchor | **10.510658 / 10.219647 nb** (full precision 10.510657911476054 / 10.219646767689696) | RU-3 (±0.5% window; soft landing ≈ 10.4969/10.2060) |
| Post-subtraction Born dilution anchor (RU-3 center) | **11.998797 / 11.666545 nb** (11.998797392290179 / 11.666544718471744) | RU-3 (±0.5%; integrated-C_rad cross-check 11.998098/11.660992) |
| Cone variant f_i removal on data_sub′ | expected 11.3063% / 10.9326%; window [11.2144, 11.4544]% / [10.8402, 11.0802]% | RU-2 (recomputing f_i or the no-cone table = STOP) |
| Charge-asymmetry shift | +0.1232 pp ± 0.02 vs unsubtracted cone Born asymmetry +1.2808% | CP-3 (expectation-centered) |
| Add-back closure | `data_sub′ + B_R′_cone` (== D_cone bitwise) through the cone variant chain reproduces the cone RAW release bitwise (≤ 1e-9) | CP-2 |
| Cone parity | IV-2/IV-3 counters exact; shard alpha-QA 174/174; response-eligible rejections 162,528 (no e/V0 class); Phase-F guard values 697/700/777/845 | IV-7 |
| Negative bins | 0 TRUE negatives (D>0 & B>D); guarded fine-empty bins h+ 2 (218/358), h− 4 (341/410/470/480), guarded-away 0.4663 | SA-1 |
| Variance convention | E1b exactly-once Poisson; `varB_R′_cone` with packaged variances (NO rescale factors); double-Poisson = STOP | SA-3 |

Template validity constraint (standing, from the critic F-1 cure + Rev-3 §B):
Set R′ = e + V0 + lowQ2; on the cone basis the **packaged CONE-APPLIED
templates are the ONLY valid templates** (e 148,976.4352 cone-applied consumed
AS-IS; the Rev-2 rescale is RETIRED; no-cone-rescaled templates FORBIDDEN);
external det.int. subtraction FORBIDDEN; e∩V0 overlap (112.2093 = 0.00290% of
D_cone) IGNORED per user decision 2026-10-08. fc1/fc2 ≡ 0 re-verified on the
cone variant (Rev-3 §A).

**Spec updates needed given the legenv reality (record at P5 dispatch):**
1. §H.5-bis Stage-2 extender clause: parent = the installed legenv package
   (sha `568040fe…`), 6 header renames, supersession of the "parent = P4
   PKG_CSV" pin and the AST cross-check vs the superseded P4 builder.
2. CP-5 gate reference: byte-identity of the 15 components (and the carried
   descriptive/derived columns) vs the installed legenv package CSV.
3. §C.3/§F release operands (RU-1 repro target, CP-1/CP-2 operands, PM-2
   objects): resolve "the cone RAW/Born release" to the CANONICAL installed
   2026-10-13 releases (`bbb_ibu_detints2prime_alphaveto_cone_…` /
   `born_crad_dj14relbin_detints2prime_alphaveto_cone_…`; values identical to
   the study-tree 2026-10-11 builds — 11.412233/11.123626 and
   13.026987/12.697511 nb; canonical manifest `a7ee02c3…`/tarball `f093a74d…`).
   The §H/§H.7 pins `6ca6b4a1…`/`b16424f0…` remain valid as the study-tree
   objects' provenance shas.
4. PM-2: the "cone releases are STUDY-TREE PROVISIONAL (R9)" clause is
   RESOLVED by the 2026-10-13/14 cone-baseline promotion; the P5 candidate
   promotion must record its basis against the canonical triad.
5. §E/PM-4 quoting: the 19-comp total `total_with_bgsub19_rel` extends the
   legenv `total_with_cone15_rel` base (medians 5.8032%/6.1919%); bgsub
   component expectations UNCHANGED (stat ≈ 0.0806/0.0958%, model ≈
   0.2016/0.2279%, fakefrac ≈ 0.1846/0.1815%, stage re-measured on the cone
   chain — exact values are build outputs); every quote states prior basis,
   Born-vs-raw, observable basis (cone-basis S2′), bgsub status (Set R′
   e+V0+lowQ2), and the cone-selection flag.
6. §H.0-bis/§H.1/§H.2/§H.3 production clauses: record P1/P3
   satisfied-by-existing, P2 waived-with-open-caveat, P4 superseded (this
   document as the adjudication record); the §H.1 sanity windows remain valid
   only as the no-cone data-side reference and are superseded for the five
   detector components.
7. PM-1: the P-ALPHA formal closure/supersession entry in STATE.md remains
   REQUIRED (Rev-3 §0-bis.3d: both critic conditions discharge by
   construction — same-selection templates; V0 gap closed, lowQ2 α0p1 radius
   mismatch carried ≪0.01% inside the ±30% term) — the legenv install did NOT
   write it; RUN_STATUS still lists P-ALPHA among carried-open blockers.

The 2026-10-11 critic FATAL verdict stands against the PRE-S2′ NO-CONE Set-R
candidate only; its rebuild conditions map onto Rev-3: (1) e+V0+lowQ2 — §C.1;
(2) canonical variant basis — the cone-basis S2′ variant chain; (3)
cone-survival rescale / V0 gap — DISCHARGED by construction on the cone basis;
(4) re-derived anchors + 15→19 budget + gate — §D/§E/§F with the 26-item gate;
(5) hygiene. Nothing in the legenv reality changes this mapping.

---

## 8. Q6 — Convention-conflict register (per stage; what forces re-run vs what is superseded)

| ID | Stage | §H pin | Shiftedlegs/canonical reality | Force re-run? |
|---|---|---|---|---|
| C-1 | P1 (electron legs) | Cone computed from RAW unshifted reco branches; rejection set IDENTICAL across shift legs (§H.2; P0 builder banner "cone = ONLY selection change … identical across all shift legs") | shift→cone on the SHIFTED direction (decision (a)); per-leg rejection sets differ (elecE_p1pct 14,512/14,083 vs nominal 14,465/14,117) | NO — superseded by the user-installed canonical whose detector legs ARE shift→cone; re-running would orphan the products. Numeric §H.2 acceptances still PASS (§3) |
| C-2 | P1/P4 | Data-side detector components: shifted DVs through the single NOMINAL cone variant response (§H.5 gate #18; "no cone coherent detector campaign exists or is needed") | FULL-CHAIN: per-leg OWN shifted cone response + OWN coherent prior + OWN f_i + stage-3 IBU; RAPGAP31 governing; FULL ABS single-sided; leg-level envelope for elecE h+ | NO for the chain (user decision; canonical installed). This is THE convention fork that supersedes §H P4 |
| C-3 | P1 | Zero-shift anchor leg REQUIRED (1 job, bit-identity) | Not produced; purpose served by the stage-3 nominal-reproduction gate (canonical release reproduced ≤ 1.5e-15, integrated exact) + read-only cone-DV per-bin identity | NO (validation debt; optional 1-job artifact) |
| C-4 | P2 | 16 cone seed DVs + P=0 anchor + manifest (§H.3) | Nothing exists; canonical tracking = legacy-ensemble differenced data-side carry (caveat OPEN, recorded) | NO for the chain; user-gated budget-successor option (17 jobs) |
| C-5 | P3 | Fresh §H.4 campaign; install at the STUDY-TREE release; stop condition n\|z\|>3 ≤ 4; bitwise re-run acceptance | Reused G5 product installed at the CANONICAL release; neg n\|z\|>3 = 5 adjudicated under the G5 pinned rule (z_absmax 4.182 < 4.5, edge watch REPORTED); no bitwise re-run | NO (§H.4 identities hold EXACT; the counter divergence is a spec-vs-reality record item) |
| C-6 | P4 | §H.5 data-side build, 31 gates, `total_with_s2prime15_*` header, campaign package path | Installed legenv canonical: full-chain legs, `total_with_cone15_*` header, central data_release path, sha `568040fe…`; §H.1 windows FAIL on elec_energy (both charges) and detint_removal (h+, marginal) | MUST-NOT-RUN as specced — running it would CREATE the conflict; supersede instead |
| C-7 | P5 | Extender parent = §H.5 P4 package (path + 43-name FULL_HEADER); CP-5 reference = "the cone 15-comp package"; release operands = study-tree 2026-10-11 cone releases; PM-2 "study-tree PROVISIONAL" | Parent/reference → installed legenv package (6 header renames, sha pin); release operands → canonical 2026-10-13 releases (same values); PM-2 clause resolved by the promotion | Engineering repoints (dated extender re-edition + gate-runner cone-edition copies) — NO physics re-run |

---

## 9. Remaining execution sequence (per-stage acceptance checks and STOP conditions)

All stages follow the RUN_SHEET discipline: every stage acceptance is a STOP
condition; failed acceptance stops the chain (report, do not improvise);
smokes before every batch campaign; all batch work via Condor; nothing
canonical touched until the recorded user promotion; no promotion by the
executor.

**S0 — Extender re-edition + P5 scaffolding (login node; 0 Condor jobs).**
Dated cone-edition copies of the
`bgsub_setr_build_2026-10-10` script set with the Rev-3 §C/§F repoints
(already specified by §H.5's Stage-2 machinery note) + the §6 extender
re-edition (parent repoint, 6 header renames, legenv sha pin
`568040fe…`); self-tests re-run (vector-builder asserts, extender synthetic
dry-run through `main()` with the real pinned npz: byte-carry 21,070/0,
quadrature ≤ 1e-12, §C.1/SA-2 sums, RU-2 windows, driver-record cross-checks).
ACCEPTANCE: py_compile/pyflakes clean; all self-tests PASS; extender no longer
expected-fail on the parent (fails only on the not-yet-existing P5 study
tree). STOP: any self-test failure.

**S1 — B_R′_cone vector build (login node; ≤ 8 s precedent; 0 Condor jobs).**
`build_bgsub_vectors_setr_v1.py` cone edition from the packaged cone-applied
templates (e as packaged, NO rescale; V0_genavg; lowQ2; simple sum; e∩V0
ignored). ACCEPTANCE / STOP: IV-1 totals as packaged (e 148,976.4352; V0
132,065.2768; lowQ2 208.3795; det.int. 27,405.8031 NOT subtracted) +
cone-DV==alpha0p2 identity; IV-2/IV-3 cone vector identity (3,865,073;
alpha_pre 3,902,480; rejections 28,582 in-PS / 19,024 in-grid; matched-e
counters 0/0); SA-1 0 true negatives (guarded bins h+ 218/358, h−
341/410/470/480; guarded-away 0.4663); SA-2 B_R′ = 281,250.09155318514 =
7.2767% of D_cone; data_sub′ sums h+ 1,824,787.4209930277 / h−
1,759,035.953778433; f_res′ 7.1686%/7.3886%. STOP: any anchor mismatch, any
negative bin, any template read from a no-cone-rescaled source (FORBIDDEN).

**S2 — Re-unfold driver evidence (login node; 0 Condor jobs).**
`run_bgsub_setr_reunfold_v1.py` cone edition: `--mode repro` (must reproduce
the canonical cone RAW release 11.412233/11.123626 nb; config identity RU-1:
cone S2′ RG variant response sha `fda5466f…`, coherent body+miss prior FROM
the cone variant response, IBU k=4, L = 351,000; no code path may read a
no-cone response or prior); smoke `--mode sub --scope smoke` per charge (25
toys; RU-2 in-window); `--mode sub --scope dryrun`; `--mode addback` (CP-2
bitwise). STOP: repro failure, RU-2 window miss, add-back non-closure,
negative reco input.

**S3 — OFFICIAL chain (Condor; 2 jobs: 1 smoke-one throwaway + 1 chain).**
Pre-S2′ RUN_SHEET §E pattern, repointed: smoke-one submission-mechanics check
(throwaway outsub; RU-2 pass line), then the single official chain job (1
core, 8 GB, fail-fast; repro → sub full scope with the 1000-toy Poisson
lineage on data_sub′ (SA-3: Poisson-only `w2 = data_sub′`; double-Poisson =
STOP) → addback → candidate packaging
`born_crad_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_<date>/`).
ACCEPTANCE / STOP: "OFFICIAL CHAIN COMPLETE" log line; packager
validation.json pass=true; RU-3 integrated within ±0.5% of Born
11.998797/11.666545 (RAW 10.510658/10.219647; integrated-C_rad cross-check
11.998098/11.660992); RU-2 removal within [11.2144, 11.4544]/
[10.8402, 11.0802]%; CP-2 ≤ 1e-9 vs the canonical cone RAW release; RU-4
C_rad bitwise (releasebin DJ14 table, sha `205e8218…` ledger); RU-5
physicality/exact-text serialization; 0 negative cross sections.

**S4 — Certification (login node; 0 Condor jobs).**
`measure_bgsub_stage_rel_v1.py` (CP-1: per-bin
|pre-unfold − coneBorn·(1−f_res′)|/pre-unfold ≤ 1% (≤ 2% sparse, justified),
integrated ≤ 0.5%, signed vector saved; operands = the CANONICAL cone Born
release); re-edited extender → `budget_15to19/bgsub_setrprime_budget19.{csv,json}`
(CP-5 byte-identity vs the legenv package; quadrature ≤ 1e-12; r_min
0.9816719489793092 pinned; npz sha `736fae14…`; fc0 standing shares recorded
per CP-4); `run_bgsub_gate_v1.py` 26-item gate (IV-1..IV-7 + SA-1..5 +
RU-1..5 + CP-1..5 + PM-1..4). ACCEPTANCE: expected 25× PASS (CP-1 expected
PASS_WITH_RECORDED_EXCEPTIONS on sparse bins) and PM-4
READY_FOR_USER_DECISION. STOP: any FAIL blocks promotion (state rule).

**S5 — Records and user decisions (login node; 0 Condor jobs).**
`cone_15comp/P5_RECORD.md` + RUN_SHEET ledger update; PM-1 P-ALPHA formal
closure entry in STATE.md; PM-2/PM-4 promotion records (candidate install,
19-comp quoting-policy switch to `total_with_bgsub19_rel`, basis sentences
per §7 item 5) — ALL USER DECISIONS, not executor actions.

**Total REQUIRED Condor jobs: 2** (S3). Optional, not required: zero-shift
anchor artifact (1), sidecar one-charge bitwise re-run (1), §H.3 cone tracking
campaign (17, user-gated).

---

## 10. Blockers

1. **Engineering prerequisite (only hard blocker to starting S1):** the
   extender re-edition (§6). The P0 edition hard-fails on the legenv parent
   (path pin + 6 header names). It is a dated-copy edit + self-test, 0 Condor
   jobs, and must be recorded as superseding the §H.5-bis parent clause.
2. **User-decision gates (not executor blockers):** PM-4 promotion of the P5
   candidate and the 19-comp quoting switch; PM-1 P-ALPHA closure entry in
   STATE.md; optional §H.3 tracking re-derivation (budget-successor campaign).
3. **No physics blockers:** every P5 physics input exists and is sha-pinned
   (§7); all Rev-3 anchors are pre-computed and recorded; the packaged
   cone-applied templates are selection-consistent by construction. The
   carried-open items in the canonical records (tracking data-side
   conservative; HFS sign-symmetry unstudied; DJ–RG model×shift interaction
   recorded-but-not-enveloped; P-CRAD-LUMI; P-PLOTS) are REPORT-ONLY for this
   chain and do not gate P5.
4. **Record-keeping debt to clear at S5:** the C-1/C-5/C-6 supersession
   entries (§H.2 electron-leg cone semantics; §H.4 closure-rule divergence and
   reuse mode; §H.4→legenv budget supersession) and the CP-5/PM-3 sha-ledger
   extension with the legenv package and canonical-release sidecar artifacts.

---

## 11. Verification provenance (this adjudication, all read-only)

- §H.2 acceptance recomputation: per-bin subset / removed-fraction /
  total-change checks of the 5 shiftedlegs plus-leg cone DVs vs the legacy
  no-cone shifted DVs and the JSON v4 pins (§3 table) — pure-Python JSON/CSV
  parsing, 2026-10-15.
- §H.4 identity checks: sidecar `stat_component` CSVs vs the canonical cone
  release `ibu/cross_sections.json` (both charges, 0.0 deviation) (§5).
- Extender parent compatibility: AST extraction of `FULL_HEADER` from
  `extend_budget_15to19_conebase_v1.py` vs the legenv CSV header (43 names, 6
  renames); CRLF/491-line/490-row/43-field checks; sha256 of the legenv CSV
  (`568040fe…`) and the cone variant RG npz (`736fae14…`) (§6).
- §H.1 sanity-window comparison: legenv per-component lower medians (n=245)
  vs the ±25% windows around the no-cone S2′ 15-comp medians (27/30 PASS;
  elec_energy ×2 and detint_removal h+ outside — both explained) (§6).
- Identity checks: `data_vector_cone_alpha_central_2026-10-11.json` vs the
  `cone_s2prime_2026-10-11` nominal DV (per-bin EXACT); anchor full-precision
  values from JSON v4 `expected_anchors`; legenv validation_checks.json
  status PASS 29/29.
- No file was modified; no Condor job was submitted; the only write is this
  document.

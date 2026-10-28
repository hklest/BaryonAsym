# Normalization Gate — bgsub set-R Born chain candidate (2026-10-11)

- **Gate:** luminosity / weight / scaling / exactly-once bookkeeping semantics of the
  background-subtracted (set R) Born chain candidate, before promotion (read-only
  verification; the normalization leg of the critic + normalization + QA promotion
  pattern).
- **Gating authority:** `h1-normalization` (luminosity and weight authority;
  `STATE.md` read first per AGENTS.md; skill `sidis-hq2-normalization` unavailable at
  load time — gate executed from the primary artifacts and the governing context
  documents cited below).
- **Date:** 2026-10-11.
- **Scope (candidate chain, all read-only):**
  - Study root: `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_build_2026-10-10/`
  - Official candidate: `born_release_candidate/born_crad_bgsub_setR_centralbase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/` (Condor official build, cluster `condor_5486183_batch1565.desy.de`, built 2026-10-10T23:43Z; integrated IBU h+ `12.62353085471277` / h− `12.302194886769733` nb)
  - `official_reunfold/`, `official_repro/`, `official_addback/`, `vectors/`,
    `budget_13to17/`, `stage_rel/`, `gate/bgsub_gate_24items_official.json`
  - Build scripts: `sidis_highQ2/scripts/systematics/bgsub_setr_build_2026-10-10/`
  - Scheme spec: `sidis_highQ2/docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md`
  - Basis verification (this chain's fake-fraction provenance):
    `sidis_highQ2/docs/status/normalization-verification-bgfracsub-2026-10-10.md` (PASS WITH CAVEAT)
  - Gate pattern mirrored: `sidis_highQ2/docs/status/normalization-gate-releasebin-2026-10-09.md`

## VERDICT: **PASS WITH CAVEAT**

All six delegated normalization checks PASS, with independent numerical reproduction
of every headline figure (integrated totals, C_rad bitwise product, yield→nb
conversion, fake-fraction removals, B_R/data_sub sums, budget byte-identity and
quadrature closure). No luminosity defect, no forbidden luminosity value, no
double-counted or omitted background class, no MC-luminosity leak into the
subtraction. The caveats are (1) a **basis-supersession promotion condition** — the
candidate is a pre-S2'-basis product while the canonical chain moved to the S2'
detint-fake response on 2026-10-10 — and (2) carried-open items that travel with any
acceptance (CP-1 recorded exceptions, P-ALPHA waiver, NucliaWeight caveat, DjBkgd
exposure provenance, C_rad publication grade). This gate covers normalization
semantics only; it is not the baseline review and not publication clearance.

## Caveat list

1. **Basis supersession — PROMOTION CONDITION (new, decision-required).** The
   candidate chain is built on the pre-S2' central base: raw parent
   `bbb_ibu_coherentprior_..._2026-10-06/` (response fake fraction, data-weighted
   `10.7141%` h+ / `10.4019%` h−), Born anchor
   `born_crad_dj14relbin_..._2026-10-09/` (h+ `13.107258` / h− `12.772782` nb),
   coherent `body+miss` prior, C_rad = DJ14 releasebin. `STATE.md` (updated
   2026-10-11) records the S2' detint-fake promotion (user decision 2026-10-10):
   the canonical raw release is now `bbb_ibu_detints2prime_..._2026-10-09/` with
   enlarged per-reco-bin `f_i` (data-weighted `11.3577%`/`10.9768%`, SUPERSEDING the
   pre-S2' scalars for the canonical chain) and the canonical Born release is
   `born_crad_dj14relbin_detints2prime_..._2026-10-10/` (h+ `13.064758` /
   h− `12.736523` nb). Per STATE.md, pre-S2'-basis values are PRE-S2'-BASIS and
   quoting them against S2' values without the observable-basis flag is FORBIDDEN.
   The candidate is internally coherent and exactly-once **on the central base**
   (its adjudication: det.int. 86.18% matched body response-modeled / 13.82%
   fake-column in `f_i`), and it does NOT externally subtract det.int. (so it does
   not violate the strengthened S2' prohibition). But promotion must explicitly
   record (a) the observable-basis flag on every quoted figure of this chain, and
   (b) the user's decision whether the published bgsub chain stays central-base or
   is re-derived on the S2' response (a re-based set-R chain is a different scheme:
   the enlarged S2' `f_i` already removes det.int. Class 2, so the fake-fraction
   figures and the fiducial-residual adjudication would both change). This gate
   does not adjudicate that choice; it flags it as a condition of promotion.
2. **CP-1 recorded exceptions (gate item 16, PASS_WITH_RECORDED_EXCEPTIONS).** 5
   bins (2 h+, 3 h−) exceed the 2% sparse-bin cap; all are declared sparse
   (`toy_stat_rel ≥ 5%`, fixed a priori), all below their own statistical
   resolution, all carried at full size in `bgsub_stage_rel` (h+ bin 34 — the
   spec-§3.3-monitored 14% corner bin — at 6.11% vs 18.8% stat). Integrated
   stage difference `2.83e-4` (h+) / `2.71e-4` (h−) inside the `5e-3` window.
   PM-4 requires explicit user acknowledgement at promotion.
3. **P-ALPHA waived, not closed.** The B_R template bank is alpha0p2-selected
   (electron-cone veto) while the chain is central; the waiver carries the bounded
   cone-residual argument (Δ 19,024 tracks = 0.49% of D, electron-dominated;
   denominator effect −0.011 pp; cone-residual max 0.62%/0.55% carried in
   `bgsub_stage_rel`). Full closure requires a state-level declaration (user
   decision); alpha0p2-template products stay PROVISIONAL/diagnostic with the
   waiver.
4. **NucliaWeight caveat on the `bgsub_fakefrac_rel` basis (carried per spec).**
   `r_min = 0.9816719489793092` (djangoh14_electron_neg) from the G9/S1b
   template-vs-fakecolumn report; integrated ratios 0.9817–0.9872 are expected
   under the NucliaWeight mismatch (response legs may carry NucliaWeight, templates
   exclude it). Spec allows carry or re-derive; carried.
5. **DjBkgd lowQ2 template exposure provenance outstanding (inherited blocker).**
   The lowQ2 template (208.3795 tracks at L351,000; 0.0054% of D) derives from the
   five-ReqID DjBkgd sum (15,813,400 nb⁻¹, fLumi pb→nb unit-corrected); the
   oomclumi/generator-log accounting over the exact resolved HAT lists remains
   open. Its ±30% scale term is folded into `bgsub_model_rel` (~0.002%, negligible),
   and the template's absolute scale is validated by IV-1 against the L351,000 bank.
6. **`f_i` per-bin statistical uncertainty not propagated (inherited covariance
   gap).** Response `sumw2` histograms remain unconsumed; the fake-fraction
   statistical uncertainty is not quantified in the chain (the generator diagnostic
   spread ≈0.07 pp integrated is a model-dependence indication only).
7. **C_rad publication grade (inherited, unchanged).** `P-CRAD-LUMI` is
   FILE-SET-RECONCILED (generator-log-open); C_rad is not publication-grade; the
   releasebin baseline is PROVISIONAL. Absolute normalization of any Born quote
   inherits this until `P-CRAD-LUMI` closes. The applied table itself is the
   canonical DJ14 releasebin table (sha256
   `cf810be44a59ed790ba78e0476eb8f59df96358357cc39e258e034ed31b22ef5`, verified on
   disk) — no stale/blind sidecar enters (PM-3).
8. **New-component sparse-bin tails vs spec drafting anchors (informational,
   systematics-owner domain).** Measured maxima (stat `7.245%`/`1.702%`, model
   `4.390%`/`3.404%`, stage `6.111%`/`2.476%`) exceed the spec §4 drafting-stage
   anchor values; the budget README records that the normative per-release-bin
   definitions are used and the spec anchors are informative. Quadrature closure is
   exact (max dev `2.2e-16`); medians are tiny (stat 0.0524/0.0615%, model
   0.1292/0.1435%, stage 0.0909/0.0950%, fakefrac 0.1851/0.1811%).
9. **Budget absolute-nb diagnostic columns** (`nominal_cross_section_nb`,
   `tracking_eff_m0p5_*_abs_nb`) are raw-level carry-overs inherited byte-identical
   from the emfix 13-component budget (same convention as the accepted crad13
   budget, prior gate non-caveat observation); the budget is relative (all
   components `_rel`) and its totals do not depend on them.

## Check results

### 1. `L_data = 351000.0 nb^-1` consistency — **PASS**

- Every product header in the chain carries `lumi_nb: 351000.0` and no other data
  luminosity: candidate `summary_totals.json` + `ibu/bbb/cross_sections.json`;
  `official_reunfold/` (+ 4 `_run/bbb_ibu_crosscheck_*.json` + run record);
  `official_repro/`, `official_addback/`, smoke/dryrun subproducts. Single script
  constants: `L_DATA_NB = 351000.0` (`common_bgsub_setr.py:101`),
  `LUMI_NB = 351000.0` (packager line 122). The budget JSON carries **no** luminosity
  field at all (relative-only product); `lumi_rel = 0.027` (2.7% flat) is carried
  byte-identical and disjoint from the new components.
- **Independent arithmetic (this gate):** `cross_section_nb == yield_unfolded /
  351000.0 × C_rad(bin)` holds BITWISE (max rel dev 0.0) for all 490 rows, both
  ibu and bbb — the yield→nb conversion uses exactly 351000.0 and nothing else;
  the only additional factor is the per-bin C_rad.
- The toy-covariance sidecars pin the Born conversion recipe
  `Cov_Born[i][j] = C_i·D_ij·C_j` with `C_i = C_rad(bin_i)/L_data`,
  `L_data = 351000.0 nb^-1`.
- No superseded data luminosity (`297657.8`, `356613.0`, `356000`, `126880`) is
  quoted or consumed anywhere in the chain (see check 6).

### 2. Fake-fraction application exactly-once — **PASS**

- **The table is the unchanged central response fake fraction.** The per-bin `f_i`
  sidecars are byte-identical (sha256) across candidate = `official_reunfold` =
  `repro` (pos `d7b5137f6dc44bc2...`, neg `db9211056f99ed53...`); the repro run
  reproduces the installed central raw parent release
  (`bbb_ibu_coherentprior_..._2026-10-06/`: bbb CSV byte-identical; ibu graded
  PASS, max non-toy numeric rel dev `4.64e-12`; 1000-toy lineage bitwise;
  100-toy vs installed stat `9.54e-15`/`5.98e-15`). Run record:
  `f_i_recomputed: false`; recomputation is a STOP condition and did not occur.
  My independent recomputation of the sidecar statistics reproduces the verified
  central table exactly (h+ median 10.0987% / max 32.2439%; h− median 9.9844% /
  max 30.6705% — the figures of the 2026-10-10 fake-fraction verification).
- **Applied once, not zero:** `d_sig = data_sub × (1 − f_i)` inside
  `derive_response_blocks` (same engine, same response ROOT). Integrated removal
  on `data_sub`: h+ `0.10712841080755742` (10.7128%) / h− `0.10400528669952303`
  (10.4005%) — independently reproduced from sidecar × data vector, inside the
  spec RU-2 windows [10.60, 10.83]% / [10.29, 10.51]%.
- **Not twice:** the same `f_i` applied to the unsubtracted central vector gives
  10.7141% / 10.4019% — the verified nominal headline figures. The combined
  central→`d_sig` removal is h+ `13.7509%` / h− `13.4546%` and equals
  `1 − (1 − f_res)·(1 − f_i)` to all computed digits — the signature of an
  exactly-once composition (a second fake-fraction pass would give ≈22%).
- **Wiring vs the parent chains:** CP-2 addback closure — `data_sub + B_R_applied`
  is bitwise identical to the central vector in every bin, and unfolding it
  reproduces the nominal raw release (totals rel dev 0.0; per-bin cross-section
  max rel `6.6e-16`). The candidate vs the Born release `born_crad_dj14relbin_..._2026-10-09/`
  differs only by the input vector; the 8 absolute-nb columns equal
  official_reunfold × C_rad BITWISE (max dev 0.0, both methods), and every unscaled
  column is byte-identical to the parent CSV text except the intentional `caveat`
  column (490/490, documented).

### 3. Background bookkeeping exactly-once across classes — **PASS**

Per-class adjudication (central-base scheme spec §2.1), each verified in code and
numbers:

| Class | Share | Handling in the candidate | Once? |
|---|---|---|---|
| Unflagged reco fakes (response fake fraction) | 10.7141% h+ / 10.4019% h− (central D) | `f_i` on `data_sub` (10.7128%/10.4005%) | exactly once |
| V0 daughters (residual heritage) | 3.4002% of D | set-R subtraction: V0 gen-avg h+ `67,092.201` / h− `64,973.076` (total `132,065.277` = 3.4002% of D) | exactly once |
| low-Q2 source | 0.0054% of D (h+ 0.0060% / h− 0.0047%) | set-R subtraction: h+ `119.431` / h− `88.949` | exactly once |
| Non-V0 electrons | 3.8355% (cone-applied heritage; 3.749%/3.963% alpha0p2-D) | NOT in B_R (B_R = V0 gen-avg + lowQ2 only); covered by `f_i` (100% fake column) | exactly once — NOT subtracted again |
| det.int. | 13.82% fake-column / 86.18% matched | NOT in B_R; fake-column part via `f_i`; matched body stays response-modeled signal; external det.int. subtraction (FORBIDDEN) not performed | exactly once / zero |
| DIFFVM ρ | 0.0074% | not subtracted, not in `f_i` (never in the v6 response) — explicit documented uncovered residual; budget rows 18/19 remain BLOCKED | zero, by design (bounded omission, not silent) |

- `B_R = v0_avg + lowq2` only (builder line 132; `|B_R − (v0_avg+lowq2)|` ≤
  `1.8e-12` at 490 level); electron/detint templates are loaded for IV-1 totals
  verification only. B_R sums h+ `67,211.631` (rel dev `4.1e-9`) /
  h− `65,062.025` (`7.5e-10`) / total `132,273.656`; `data_sub` h+
  `1,908,089.524` / h− `1,843,734.068` (spec anchors ≤ `8.1e-8` rel, guarded
  convention); `data_sub == n_raw − B_R_applied` bitwise; 0 negative bins (980 and
  490); the 3 empty bins (h+ 109/179, h− 240) subtract exactly 0 (guard).
- Integrated f_res (central D): h+ `3.4026%` / h− `3.4085%`, asymmetry
  `−0.0059 pp` (charge-symmetric subtraction); per-charge B_R kept separate.
- No DIFFVM token appears anywhere in the chain (grep clean) — no double
  subtraction and no silent omission; the omission is the documented, bounded
  residual (0.0074%) with its budget row still BLOCKED.

### 4. Unit sanity (nb/pb conventions; template→subtraction-vector units) — **PASS**

- All luminosity fields are explicitly nb (`lumi_nb`; 351,000.0 nb⁻¹ = 351 pb⁻¹);
  no pb⁻¹-denominated field exists in the chain; no `fLumi`/`SetMCLumi`/
  `fFileLumis` usage (the 2026-09-25 fLumi pb-trap is not in play).
- The B_R templates come from the **L351000-rescaled bank**
  (`reco_backgrounds_L351000_finalcandidate_20260626`, data-equivalent track
  counts at L_data); IV-1 reproduces the validated totals exactly (electron
  `148,976.4352` / V0 `132,065.2768` / det.int. `27,405.8031` / lowQ2 `208.3795`;
  abs dev ≤ `3.2e-5`). Template counts, data vector, `w2`, and unfolded yields are
  all in the same track-count units; the only unit conversion in the chain is
  `/351000.0` (verified bitwise, check 1) plus the dimensionless per-bin C_rad.
- Toy covariance sidecars are explicitly `units: unfolded_yield_counts` with a
  written unit warning and the Born recipe pinning `L_data = 351000.0` and the
  releasebin C_rad table (charge-local `bin_index % 245` remap).
- Budget: 35 parent columns byte-identical to the frozen emfix 13-component budget
  (independent text comparison: **0 / 17,150 cell mismatches**); all components
  `_rel` (dimensionless); the four new components independently re-derived exactly
  from the vectors product (stat `sqrt(varB_R_applied)/data_sub` dev 0.0; model
  `hypot(hs/d, 0.3·lowq2/d)` with empty-bin guard dev 0.0; fakefrac
  `|1−r_min|·f_release(data_sub, f_i)` dev 0.0; stage from the official
  measurement); 17-component quadrature closure max dev `2.2e-16` (≤1e-12);
  totals h+ `5.7159%` / h− `6.1586%` vs 13-component `5.7093%`/`6.1523%`.
- YIELD POLICY CHOICE A warning present (`yield = sigma × lumi_nb` deliberately
  broken at Born level, with the recovery recipe) — the nb/pb and yield/nb
  conventions are documented at every product boundary.

### 5. MC luminosity non-entry — **PASS**

- **No MC luminosity is applied anywhere in the candidate chain.** Zero `L_MC`
  tokens in the bgsub scripts (no `9132891`, `10487495`, `15813.4`, `4379935`,
  no generator scale factors `0.0384…`/`0.0334…`/`0.0221…`); the mclumi
  aggregation file is referenced nowhere; the candidate's 15-entry provenance
  sha256 ledger contains no MC-luminosity input; no DjBkgd/7091/7110 trap.
- `f_i` is a same-sample, same-weight ratio of the central response
  (`F_i/(Σ_j B_ij + F_i)`, both columns filled with `w_reco = w1·w2·NucliaWeight`
  in the same pass) — any global MC normalization including L_MC cancels exactly
  (established in the 2026-10-10 fake-fraction verification; the candidate uses
  the identical engine function on the sha-ledgered response ROOT).
- The B_R templates are loaded as-is from the pre-rescaled L351000 bank — the only
  place MC luminosity ever entered is the upstream 2026-06 template-bank rescale
  to L_data scale, which is the REQUIRED data-equivalent normalization for a
  count-space subtraction, is validated by IV-1 against the frozen totals, and is
  not re-applied or modified by this chain. Its residual scale uncertainty is
  inherited (DjBkgd exposure provenance outstanding, caveat 5; lowQ2 ±30% term in
  `bgsub_model_rel`, ~0.002%).
- Toys are Poisson-only on `data_sub` (SA-3 exactly-once, asserted bitwise
  `w2 == data_sub`); the release driver uses `L_data` only; the budget is
  relative-only. No MC normalization can leak between the template bank and the
  published nb.

### 6. Stale-field scan (`L_data_HERA-II_S67_nb: 297657.8`) — **PASS**

- The stale header field of
  `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json` (flagged as
  caveat 4 of the 2026-10-10 fake-fraction verification) is **not consumed and not
  quoted**: zero occurrences of `L_data_HERA-II_S67_nb` across the entire study
  tree, the candidate, and the build scripts; the mclumi aggregation file itself
  is referenced nowhere in the chain (grep clean).
- No `297657.8` / `356613` / `356000` / `126880` value appears in any product,
  script, or steering file of the chain. The only grep hits are coincidental
  digit substrings inside covariance float payloads (e.g.
  `-0.016959514332297657`, `0.29765756062511173`) — the same documented
  float-noise pattern as the 2026-10-09 releasebin gate; these are matrix
  elements in yield units, not luminosities.

## Blockers (carried OPEN; this gate does not close them)

1. **S2' basis decision (new; promotion condition)** — see caveat 1: the user must
   record the observable-basis flag and decide central-base vs S2'-re-based
   publication of the bgsub chain (PM-4 input).
2. `P-ALPHA` — waived with documented bounded argument; full closure requires a
   state-level declaration (user decision).
3. `P-CRAD-LUMI` — C_rad exposure provenance FILE-SET-RECONCILED
   (generator-log-open); C_rad NOT publication-grade; releasebin baseline
   PROVISIONAL.
4. DjBkgd lowQ2 template exposure provenance (oomclumi/generator-log accounting)
   outstanding — inherited, bounded (±30% term folded in `bgsub_model_rel`).
5. CP-1 recorded exceptions require explicit user acknowledgement at promotion
   (PM-4); `bgsub_stage_rel` carries them at full size.
6. Fake-fraction per-bin statistical uncertainty not propagated (inherited
   covariance gap); `response sumw2` unconsumed.
7. `P-PLOTS`, `P-METHOD` open, unchanged. Multiplicities remain blocked (no DIS
   denominator); absolute cross-section quoting remains governed by the existing
   normalization policy (candidate README scope limits, respected).

## Evidence

- Candidate: `born_release_candidate/born_crad_bgsub_setR_centralbase_..._2026-10-10/`
  (`summary_totals.json`, `README.md`, `manifest.json`,
  `validation/validation.json` 13/13 pass, `provenance/inputs_sha256_ledger.json`
  15 entries, `f_i_sidecar/`, `covariance/` sidecars with units metadata,
  `ibu|bbb/cross_sections.csv`).
- Chain products: `official_reunfold/run_record_sub.json` (RU-2, wiring, ledger),
  `official_reunfold/summary_totals.json` (raw IBU h+ `11.058645` /
  h− `10.777259` nb = −3.70%/−3.69% vs nominal, consistent with the 3.66%/3.66%
  data-weighted f_res), `repro/run_record_repro.json` (RU-1),
  `official_addback/run_record_addback.json` (CP-2), `vectors/bgsub_setr_vectors.json`
  + `bgsub_setr_vectors_490.csv` + `data_vector_bgsub_setr_centralbase.json`,
  `budget_13to17/bgsub_setr_budget17.{csv,json}` + `README_13to17.md`,
  `stage_rel/bgsub_stage_rel_measurement_official.json`
  (+ `bgsub_stage_signed_difference_official.csv`),
  `gate/bgsub_gate_24items_official.json` (22 PASS + CP-1 exceptions + PM-4
  READY_FOR_USER_DECISION; 0 FAIL).
- Read-only references: installed raw parent
  `output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/`, Born
  release `born_crad_dj14relbin_..._2026-10-09/`, emfix budget
  `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/`,
  C_rad table `data/radiative/djangoh14_crad_releasebin_2026-10-09/`
  (sha256 `cf810be4...`), template bank
  `reco_backgrounds_L351000_finalcandidate_20260626/`, G9/S1b report
  `allfake_alpha0p2_L351000_.../validation/template_vs_fakecolumn_report.json`.
- Independent verification scripts (this gate, read-only):
  `/tmp/opencode/normgate_bgsub_check1.py`, `normgate_bgsub_check2.py`,
  `normgate_bgsub_check3.py` (+ inline follow-ups): integrated totals, bitwise
  yield→nb and C_rad products, full-column byte-identity, f_i sidecar sha256 and
  removal recomputation, B_R/data_sub sums and composition, budget byte-identity /
  component re-derivation / quadrature closure, stale-token and mclumi scans.

*Verification performed read-only on the NAF login node (single-file CSV/JSON
reads only; no release, table, or artifact was modified).*

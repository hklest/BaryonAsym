# Independent QA gate — background-subtracted (Set R) Born chain candidate (2026-10-11)

- **Gate:** promotion QA for the 2026-10-10 background-subtracted (Set R) Born chain candidate
  (pre-unfold V0 gen-avg + low-Q2 subtraction on the central data vector with the empty-bin
  guard; response fake fraction applied exactly once; C_rad applied from the releasebin
  DJANGOH14 table; central base).
- **QA role:** independent (`h1-qa-specialist`); the internal 24-item gate's verdicts were NOT
  trusted — every load-bearing number below was re-derived read-only from the artifacts. Method:
  hand arithmetic at 6–10 significant digits, line-level CSV structure checks, textual comparison
  of head/tail rows, and sha256 anchors cross-checked across independent artifacts (candidate
  manifest, chain run records, gate JSON, README), corroborated by the chain's own recorded
  bitwise recomputations (packager `canonical_arithmetic_diff` 0/6860 cells; gate RU-4 max dev
  0.0). No shell was used; sha256 values were not recomputed by this gate, only cross-anchored.
- **Date of gate:** 2026-10-11.
- **Grounding note:** the `sidis-hq2-backgrounds` skill was not loadable in this runtime; the
  review was grounded directly in `STATE.md` (updated 2026-10-11), `docs/RUN_STATUS.md`, the
  governing spec `docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md` (+ machine
  JSON `bgsub_scheme_spec_2026-10-10.json`, sha256
  `44b995c52c08246f408f47c588f241c6f0cd318ca5d46c02f85dc482a708e714`, anchored independently in
  the smoke run record and the candidate inputs ledger), and the on-disk artifacts.

## VERDICT: PASS WITH CONDITIONS

The candidate is internally consistent, reproduces its chain, and passes all six mandated checks
with zero defects found against its own 2026-10-10 spec. The conditions bind the **promotion
step**, not the candidate integrity: the chain is PRE-S2′-basis while the canonical central basis
moved to S2′ on 2026-10-10, so the promotion record must carry the basis decision explicitly.
This gate is a promotion gate, NOT publication clearance (carried-open caveats — `P-CRAD-LUMI`
generator-log ceiling, PROVISIONAL releasebin baseline, mixed-beam-only C_rad validity, A-F3
charge-asymmetry systematic before any A_h quote, covariance gaps, publication plots — remain
unchanged).

## Candidate and chain verified

| Artifact | Path (absolute, under `…/H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_build_2026-10-10/`) | Key facts re-verified |
|---|---|---|
| Born candidate release | `born_release_candidate/born_crad_bgsub_setR_centralbase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/` | 33 manifest files; validation all-pass; integrated IBU h+ `12.62353085471277` / h− `12.302194886769733` nb reproduced across 4 independent artifacts (candidate `summary_totals.json`, gate RU-3, chain log step 4, validation); RU-3 dev −0.0283%/−0.0271% vs anchors `12.62710539454316`/`12.305532542413683` (window ±0.5%) |
| Chain run records | `official_repro/`, `official_reunfold/`, `official_addback/`, `official_smoke_one/` (each `run_record_*.json`) | Repro reproduces the installed pre-S2′ coherent-prior release: BBB byte-identical (`b6fc5044…`), IBU max non-toy dev `4.64e-12` (documented cross-node BLAS; string columns identical), toy_stat bitwise; re-unfold totals `11.058645`/`10.777259` nb; addback CP-2 bitwise (max rel dev 0.0) returning exactly the repro totals `11.483372048266185`/`11.190382714709283` nb |
| Condor steering + logs | `…/H1ChargedSidis_HighQ2/condor_v6/central/bg_subtraction/bgsub_setr_build_2026-10-10/logs/` | Smoke cluster `5486182` ends `SMOKE-ONE COMPLETE 2026-10-10T23:39:55Z` (node batch1655); chain cluster `5486183` ends `OFFICIAL CHAIN COMPLETE 2026-10-10T23:43:37Z` (node batch1565 — matches candidate validation.json `condor_5486183_batch1565.desy.de`); all four steps report pass in the chain log; `.err` logs clean of error patterns |
| Stage-rel measurement | `stage_rel/bgsub_stage_rel_measurement_official.json` | CP-1 evidence: 11 bins >1% (5 pos + 6 neg), all declared sparse, 5 exceed the 2% sparse cap (recorded exceptions), all below own stat resolution, all carried full-size |
| Budget 13→17 | `budget_13to17/` (CSV + JSON + README) | 490 rows; 17 components; quadrature closure; frozen-13 inheritance; totals17 medians h+ `5.715871200490443%` / h− `6.158571463968941%` |
| Vectors | `vectors/bgsub_setr_vectors.json` + `vectors/bgsub_setr_vectors_490.csv` | SA/IV blocks all pass; per-bin f_res, B_R, data_sub, stat/model terms consistent with budget17 (spot-verified pos/34, pos/244) |
| Internal gate | `gate/bgsub_gate_24items_official.json` | 24 items: 22 PASS, CP-1 `PASS_WITH_RECORDED_EXCEPTIONS`, PM-4 `READY_FOR_USER_DECISION`, 0 FAIL, 0 held |

## Check results 1–6

| # | Check | Result | Evidence (independent re-derivation) |
|---|---|---|---|
| 1 | Schema / observable validation | **PASS** | Candidate IBU and BBB CSVs: 491 lines = header + 245 `pos` + 245 `neg` rows; charge-block transitions verified line-level (line 246 = `pos,244,244`; line 247 = `neg,0,245`; line 491 = `neg,244,489`); releasebin-consistent labels (S0–S3 strata, x/z/pT sub-indices, global 0–489); full release column set present incl. per-component uncertainties and the PROVISIONAL-candidate caveat column. Zero negative cross sections/subtractions (SA blocks: 0 negative bins, 980/490 checked). Magnitudes sane; integrated h+ `12.62353085471277` / h− `12.302194886769733` nb. Empty-DATA-bin convention identical to the nominal release: h+ bins 109/179 and h− bin 240 subtract exactly 0; their unfolded values are nonzero via migration inflow, matching parent behavior (RU-5 note) — not artifact bins. Multiplicities remain blocked (no DIS denominator) per caveat |
| 2 | Reproduction / identity | **PASS** | C_rad product identity: gate RU-4 max dev 0.0 (bitwise, 6860/6860 cells); hand check on pos bins 0/1/2 (parent raw × C_rad = candidate at 1e-8..1e-10); candidate C_rad provenance copy textually matches the canonical releasebin table (sha `cf810be4…`); integrated C_rad h+ `1.1415172920054786` / h− `1.1410367377289135` (1e-12 gate). Reproduction: `official_repro` reproduces the installed pre-S2′ coherent-prior release (BBB byte-identical; IBU ≤4.64e-12 documented cross-node BLAS with string columns identical; toy bitwise) — chain log step 1 confirms `11.483372`/`11.190383` nb. Re-unfold: RU-2 integrated removal `0.10712841080755742` / `0.10400528669952303` inside windows [0.106, 0.1083] / [0.1029, 0.1051]; f_i sidecars byte-identical across re-unfold, repro, and the candidate manifest (pos `d7b5137f6dc44bc2e5a0a1c3ad1b362d38c49885430437b68d0d0877351732df`, neg `db9211056f99ed53d1c4a0d39c81ce5f5b4d7da137c7f5da17eac35d2aab6b0f`; head/tail rows textually identical candidate↔repro, 491 lines, bins 0–489). Addback closure: CP-2 bitwise, totals identical to repro. Unscaled columns textually identical to raw parent (pos/0). Integrated anchors reproduced (RU-3 above) |
| 3 | Budget17 QA | **PASS** | 17 components; CSV 490 rows (neg block 245 + pos block 245; line 491 = `pos,244,244`); all components finite, non-negative. Quadrature closure hand-verified on `neg/0` this session: 17-component sum of squares → `0.029329338` vs recorded `0.02932933804302427` (8 digits); crad13 subtotal → `0.029141277` vs recorded `0.02914127699699309`; and on `neg/4` earlier (~7 digits). Frozen-13 inheritance: `neg/0` row textually identical to the emfix parent CSV (nominal `1.8593914506266185` + all 13 components; parent sha `a385b77b…`); gate budget item records 0/14700 mismatches. Totals17 medians h+ `5.7159%` / h− `6.1586%` (vs total13 `5.7093%`/`6.1523%`); new-component medians: stat `0.0524`/`0.0615%`, model `0.1292`/`0.1435%`, stage `0.0909`/`0.0950%`, fakefrac `0.1851`/`0.1811%`. No undeclared spikes: model smoothing QA 0 spikes (max ratio 1.837, raw default stands); fakefrac r_min `0.9816719489793092` (djangoh14_electron_neg, NucliaWeight caveat carried) |
| 4 | Internal-gate evidence audit (5 SA items + CP-1) | **PASS** | Gate JSON: 22 PASS + CP-1 `PASS_WITH_RECORDED_EXCEPTIONS` + PM-4 `READY_FOR_USER_DECISION`, 0 FAIL. SA items re-derived from vectors: B_R h+ `67211.63127235907` / h− `65062.02504847545`; data_sub h+ `1908089.5242996668` / h− `1843734.0675192538`; integrated f_res `3.4026%`/`3.4085%` (asym −0.0059 pp); 0 negative bins (980/490); empty bins subtract exactly 0. CP-1: 11 bins >1%, all declared sparse a priori (`toy_stat_rel ≥ 0.05`); the threshold `SPARSE_STAT_REL_MIN = 0.05` is hard-coded in `measure_bgsub_stage_rel_v1.py` and identical in the pre-official gate — fixed before the official comparison, i.e. pre-registered, not post-hoc. 5 bins exceed the 2% sparse cap; each is a recorded exception with written justification, below its own statistical resolution, carried full-size in `bgsub_stage_rel`. Bin 34 (h+) stage diff independently re-derived: (0.00030342139828147703 − 0.00028594582381763473)/0.00028594582381763473 = `0.0611150` = recorded `0.06111498405721622` (pre-unfold subtraction vs post-unfold-dilution variant, relative to the published value); toy_stat_rel 18.80% ≥ 0.05 → sparse; 6.111% < 18.80% own-stat. Integrated stage `2.83e-4`/`2.71e-4` PASS |
| 5 | Data-MC / physics sanity | **PASS** | f_res medians ~1.5–1.6%; single h+ outlier bin 34 at `14.02%` (`f_res_central = 0.14022301562883016`, confirmed in `vectors_490.csv`) is the spec §3.3 monitored bin, flagged and justified in the stage-rel JSON as designed. h− max f_res `5.225%` at global 245 re-verified (`0.05224889736077887`). Charge symmetry of the subtraction sane (integrated f_res asym −0.0059 pp; B_R and data_sub magnitudes consistent between charges). No artifact bins; sparse-bin behavior consistent with the raw parent |
| 6 | Provenance completeness | **PASS** | Candidate `provenance/inputs_sha256_ledger.json`: 14 entries covering C_rad table (`cf810be4…`), canonical plotter script (`4ee97d7a…`), frozen merge block (`8dedd657…`), spec JSON (`44b995c5…`), raw-parent IBU (`96e66d02…`), Born-base IBU (`1be81161…`), binmap, vectors (JSON `a2dc1c09…`, 490-CSV `9e5ed633…`, data vector `29575ba8…`), re-unfold run record + IBU/BBB CSVs, frozen-13 emfix budget (`a385b77b…`), fake-fraction template report (`7b0db88a…`); response (`53234e21…`), coherent rebuild (`4f5171f0…`), prior and data vector are recorded inside the re-unfold/repro run records, which the ledger anchors by hash — chain-complete. Frozen provenance plotter copy + the 2026-10-10 live-plotter drift note (re-point; merge block no longer multiplies by C_rad) recorded in candidate README + validation.json; frozen blocks `819eb2ba…`/`351bbac2…` pinned to the 2026-10-09 Born release's own validation. Run clusters verified from the Condor logs themselves (5486182/5486183, both COMPLETE, see table). Protected-trees ledger before==after in packager validation (raw release IBU `96e66d02…`, Born IBU `1be81161…`, C_rad `cf810be4…`, canonical script `4ee97d7a…`, frozen blocks `8dedd657…`); `candidate_installed_outside_study: []`; no promotion entries in state docs (`promotion_entries_in_state_docs: []`, `unauthorized_promotion_detected: false`) — PM-4 evidence confirmed |

## Key finding — S2′ basis conflict (drives the conditions)

The entire chain is **PRE-S2′-basis**: response/prior (2026-10-06 coherent rebuild), per-reco-bin
f_i (integrated removal 10.7128%/10.4005%; superseded data-weighted scalars `10.7141%`/`10.4019%`),
Born base (2026-10-09 releasebin Born release), and budget parent (pre-S2′ 13-component emfix
budget). `STATE.md` (updated 2026-10-11) records the 2026-10-10 S2′ detint-fake promotion as
canonical: S2′ RAW release (`bbb_ibu_detints2prime_…_2026-10-09/`, integrated IBU h+
`11.446174` / h− `11.158554` nb; data-weighted variant f_i `11.3577%`/`10.9768%`), S2′ Born
release (`born_crad_dj14relbin_detints2prime_…_2026-10-10/`, `13.064758`/`12.736523` nb), and
the 15-component S2′ budget (medians `8.1864%`/`8.4079%`). All pre-S2′ products are superseded
as physics basis (retained as provenance, historical/audit only), and quoting an S2′ value
against a pre-S2′ value (or the reverse) without the observable-basis flag is FORBIDDEN.

The candidate, its spec, and the RUN_SHEET do not mention S2′. Set R (V0 gen-avg + low-Q2)
remains the fiducial residual under S2′ as well — it is a different background class from
det.int. Class 2 (which S2′ removes response-internally via enlarged per-reco-bin f_i), and
V0/low-Q2 remain unremoved under S2′ (vetoed in selection, not subtracted; the Option-A external
vectors remain unapplied to the central chain per RUN_STATUS). The external-det.int.-vector
prohibition on the S2′ response (double-removal + k_parented keeper violation) does not restrict
the Set R class, but basis consistency does: a current-basis Set R product requires re-derivation
on the S2′ base (variant response, coherent prior recomputed from the variant response, variant
f_i), not a cross-basis splice.

## Conditions (bind the promotion step)

- **C1 — Basis decision (binding).** Before any promotion/install of the candidate into
  `output_v6/central/`, the promotion record must (i) label the candidate PRE-S2′ OBSERVABLE
  BASIS (prior basis: coherent `body+miss`; level: Born; observable basis: pre-S2′), and
  (ii) record the explicit user decision between: (a) promote as a pre-S2′-basis derived variant,
  with the observable-basis flag mandatory on every quote, or (b) hold promotion and re-derive
  the Set R chain on the S2′ base first. If (b), the re-derivation must use the S2′ variant
  response/prior/f_i and must not apply any external det.int. vector. Until the decision is
  recorded, the candidate remains a study-tree product; no quote of candidate values alongside
  S2′ canonical values without the observable-basis flag.
- **C2 — PM-4 user decisions (binding).** The promotion step must record the three designed
  decisions: (i) CP-1 sparse-bin exceptions acknowledgment — 11 bins >1%, 5 exceeding the 2%
  sparse cap, all below own statistical resolution, carried full-size in `bgsub_stage_rel`;
  (ii) P-ALPHA waiver acceptance — cone difference 19,024 tracks, electron-dominated,
  denominator effect −0.011 pp integrated, carried full-size in `bgsub_stage_rel`; full closure
  of the alpha0p2 PROVISIONAL status additionally requires the state-level exactly-once
  declaration in STATE.md/RUN_STATUS.md; (iii) the 17-component quoting switch —
  `total_with_bgsub17_rel` becomes the quoted published total for this product (medians h+
  `5.7159%` / h− `6.1586%`).
- **C3 — Budget nominal-column label (binding, consumer-facing).** The budget17
  `nominal_cross_section_nb` column is the inherited parent (pre-S2′ raw IBU) nominal, NOT the
  candidate Born cross section (e.g. pos,34: budget nominal `0.0002905461646252388` = parent raw
  IBU, vs candidate Born `0.00028594582381763473`). The promotion record / README must state
  this so no consumer reads the nominal column as the candidate value.
- **C4 — Spec-text gap (non-blocking remediation).** Record the numeric sparse-declaration
  threshold (`toy_stat_rel ≥ 0.05`, `SPARSE_STAT_REL_MIN` in `measure_bgsub_stage_rel_v1.py`) in
  the next revision of the spec text; the spec pre-authorizes declared-sparse bins with written
  justification via the CP-1 thresholds (1%/2%/0.5%) only. The threshold was fixed in the tool
  before the official run and is identical in the pre-official gate — pre-registered relative to
  the official comparison, not post-hoc; this is a documentation gap only.

## Minor findings (recorded, non-blocking)

- **M1.** f_i "byte-identical to nominal sidecar" (RU-2) is established via the repro run: the
  installed pre-S2′ release ships no `f_i_sidecar/` file itself, but `official_repro` reproduces
  the installed release (BBB byte-identical) using exactly this table (pos `d7b5137f…` / neg
  `db921105…`), and the re-unfold and candidate copies carry the same hashes. Evidence sufficient;
  recorded for provenance clarity.
- **M2.** The 3 empty DATA bins (h+ 109/179, h− 240) subtract exactly 0 yet unfold to nonzero
  cross sections via migration inflow — identical to the nominal release (RU-5 note); not a
  defect.
- **M3.** Chain certification steps (stage-rel, budget 13→17, gate) ran on the login node per
  RUN_SHEET after the Condor chain; all their inputs are the official Condor products verified
  above. Consistent with the login-node policy for single-file checks; noted for the record.

## Statement

- Overall verdict: **PASS WITH CONDITIONS** (C1–C3 binding on promotion; C4 remediation).
- Blockers: none.
- Next validation artifact needed: the promotion/install record containing the C1 basis decision
  and the C2 user decisions (or, if C1 option (b), the S2′-basis re-derivation chain and its own
  gate). No rerun of the present chain is required for promotion under option (a).
- Nothing was written outside this QA record; all review work was read-only.

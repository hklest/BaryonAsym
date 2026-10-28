# Coherent-prior IBU fix: quantified k=4 impact (side-study results)

**Study date:** 2026-10-05 (artifacts written 20:59); completion verification and this report: 2026-10-06.
**Status:** PROVISIONAL INTERNAL DIAGNOSTIC — coherent-prior (body+miss) IBU k=4 side study; no central product modified; raw/no-bgsub, no C_rad, not Born level; multiplicities blocked (no DIS denominator).
**Study directory:** `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/coherent_prior_ibu_z005_20261005/`
**Predecessor investigation:** `docs/unfolding/response_prior_mismatch_investigation_2026-10-04.md` (reproduction in `data/audits/response_prior_mismatch_2026-10-04/`)
**STATE entry:** blocker "Response truth conservation" (see the Blockers section of `STATE.md`)

This report documents the controlled quantification of the stored-prior/response incoherence defect
in the central v6/z005 RAPGAP response and its IBU k=4 consumer. The study was run read-only against
the central migration ROOT and data vector; all outputs live in the study directory. Nothing in the
central release was changed, and the central ROOT sha256 was verified identical before, after, and
again at completion time.

## 1. Defect recap

The central migration ROOT stores the plain IBU prior `h_prior_gen_{pos,neg}_rapgap`, filled by the
canonical builder from every accepted generator row with `w_gen = weight1*weight2` only. The response
itself is filled differently:

* **Body legs** (matched `ChargedTot` rows, physical block `h_R_{ch}_rapgap[0:490, 0:245]`) carry
  `w_reco = weight1*weight2*NucliaWeight`, and additionally require a valid reco bin, full reco
  selection, and matched-key resolution.
* **Miss row** (`h_R_{ch}_rapgap[490, 0:245]`, unconsumed accepted gen rows) uses `w_gen`.

Consequently the stored plain prior is **not** the response's own truth marginal (`body+miss`) in
245/245 gen bins for both charges, with a ±2.8% peak-to-peak scatter:

| charge | (body+miss)/plain min / median / max | plain-prior sum | body+miss sum | sum mismatch |
|---|---|---:|---:|---:|
| h+ | 0.964168 / 0.997185 / 1.028029 | 91,784,225.0791 | 91,879,281.4817 | +0.1036% |
| h- | 0.965375 / 0.994197 / 1.028519 | 90,185,163.1419 | 90,271,538.7361 | +0.0958% |

Central IBU k=4 consumes this plain prior in **both** of its roles — D'Agostini iteration
initialization and the efficiency normalization (`eps_j = body_j / prior_j`). The defect was
critic-verified 2026-10-05 by two independent critics (cold + hot stances): the "IBU implementation
mistake" claim is sound, and intentional-design and cancellation attacks both failed
(`docs/critiques/critic-prior-mismatch-{cold,hot}_2026-10-05.md`).

**Not affected:** the release acceptance `A_j = body_j/(body_j+miss_j)` is computed from the response
alone and is coherent by construction; therefore the acceptance column and the BBB (diagonal
acceptance) path are untouched by this defect. The defect acts only through IBU prior/efficiency.

## 2. Fix definition (as implemented in the side study)

The fix rebuilds the consumed IBU prior as the response's **own truth marginal**:

```
truth_j = sum_i B_ij + miss_j      (physical body columns 0..244 of the (491,246) response;
                                    fake column 245 excluded)
```

and uses it in both prior roles. With `p_eff = truth`, the IBU efficiency becomes
`eps_j = body_j/(body_j+miss_j)`, i.e. **identical to the release acceptance definition** — removing
the double standard between IBU's internal efficiency and the reported acceptance.

Four variants were unfolded with otherwise identical semantics (exact release equations, IBU k=4,
L = 351000 nb, response-derived fake fraction `frac = fake/(body_row_sum+fake)` applied to the data
vector `h_data_bgsub_{ch}`):

| variant | p_init | p_eff | role |
|---|---|---|---|
| `ibu_baseline` | plain | plain | central release semantics (control) |
| `ibu_coherent` | truth | truth | the fix |
| `ibu_initonly` | truth | plain | isolates the initialization role |
| `ibu_effonly` | plain | truth | isolates the efficiency role |

**NWeff note:** the canonical builder defines `h_prior_gen_{tag}_NWeff` (matched legs
`w_gen*NW_partner`, misses `w_gen`), but the central ROOT contains **no** NWeff object
(`comparison/prior_variants_comparison.json`: `NO_STORED_NWEFF`, empty candidate list, before/after
central-file identity). The coherent prior was therefore rebuilt as the response's exact column
marginal — precisely the investigation's recommended option, computable from the stored response
without any reprocessing.

## 3. Validation gates (validation.json: PASS; summary.json: PASS)

* **Data-vector integrity:** sha256 `dcde3231…82f5e26f` matches the expected value recorded in the driver.
* **Baseline gate:** the study's `ibu_baseline` reproduces the central release
  `data_release/bbb_ibu_finalcandidate_…_2026-06-10/ibu/cross_sections.csv` (490 rows) to a maximum
  relative deviation of **3.31e-15** (h+ 3.12e-15, h- 3.31e-15). The study's IBU semantics are exactly
  the release's, so the measured deltas are attributable to the prior change alone.
* **Coherent-prior consistency:** the extracted truth marginal equals `body+miss` exactly by
  construction (self-check: max_abs 0.0, ratio min/median/max = 1.0/1.0/1.0, both charges).
* **Prior-input mismatch reproduced:** coherent/plain = 0.964168/0.997185/1.028029 (h+) and
  0.965375/0.994197/1.028519 (h-), matching the 2026-10-04 audit bin-for-bin.
* **Output sanity:** all four variants have **490 rows** (245 h+ + 245 h-), all cross sections finite,
  **zero negative bins** anywhere.
* **Central-product integrity:** migration ROOT sha256
  `53234e21d1faf26478e13502ec2024d5ac30550d34e32d5ec007bed10cab6019` identical before and after the
  run, and re-verified unchanged at completion time (2026-10-06). No central product was written.

## 4. Coherent vs original (plain-prior) IBU k=4

| charge | median abs Δrel | max abs Δrel | bins >0.5% | bins >1% | bins >2% | integrated old (nb) | integrated new (nb) | integrated shift |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| h+ | **1.031%** | **3.583%** | 165/245 | 125/245 | 46/245 | **11.468687** | **11.483372** | **+0.1280%** |
| h- | **1.099%** | **3.463%** | 174/245 | 129/245 | 62/245 | **11.176977** | **11.190383** | **+0.1199%** |

Combined integrated total: 22.645664 nb → 22.673755 nb, i.e. **+0.1240%**. This confirms the
investigation's pre-rerun prediction ("sub-percent to few-percent per bin at k=4, not an integrated
0.1% normalization shift"): the integrated shift (+0.12–0.13%) is close to the prior-sum mismatch
(+0.10%), but the per-bin effect is ~1% median, up to 3.6%.

**z pattern** (median abs Δrel by gen-z index, h+ / h-):

| z bin | [0,0.05) | [0.05,0.1) | [0.1,0.2) | [0.2,0.3) | [0.3,0.5) | [0.5,0.7) | [0.7,1] |
|---|---:|---:|---:|---:|---:|---:|---:|
| median | 0.820% / 0.787% | 1.279% / 1.406% | 1.303% / 1.358% | 1.190% / 1.227% | 1.163% / 1.304% | 0.927% / 0.983% | 0.964% / 0.910% |

**pT pattern** (median abs Δrel by gen-pT index, h+ / h-):

| pT bin (GeV) | [0,0.5) | [0.5,1) | [1,2) | [2,4) | [4,8) |
|---|---:|---:|---:|---:|---:|
| median | 0.968% / 0.917% | 1.051% / 1.227% | 1.363% / 1.336% | 1.230% / 1.187% | 0.960% / 1.099% |

The shift is smallest in the first z bin and in the softest pT bin, and peaks at intermediate
z ∈ [0.05,0.5) and pT ∈ [1,2) GeV at ~1.2–1.4% median. There is no monotone edge blow-up and no bin
exceeds 3.6% — a smooth reweighting consistent with the ±2.8% prior mismatch propagated through four
iterations and the bin-coupled efficiency normalization.

**Independent cross-check:** the flat-phi fork_rg contamination medians (1.037% / 1.103%, recorded in
the flat-phi study lineage) agree with the coherent-vs-baseline medians (1.031% / 1.099%), as expected
for a truth-coherent-prior contamination of the same magnitude.

## 5. Attribution: efficiency-dominated, initialization negligible

| variant | h+ median / max | h- median / max | h+ >1% | h- >1% | integrated shift h+ / h- (recomputed) |
|---|---:|---:|---:|---:|---:|
| `initonly` | 0.0330% / 1.425% | 0.0318% / 0.983% | 2 | 0 | +0.0001% / +0.0006% |
| `effonly` | 1.052% / 3.455% | 1.156% / 3.251% | 125 | 131 | +0.1278% / +0.1193% |
| `coherent` | 1.031% / 3.583% | 1.099% / 3.463% | 125 | 129 | +0.1280% / +0.1199% |

* **Efficiency-prior replacement carries essentially the whole effect:** `effonly` reproduces the
  coherent per-bin medians, maxima, and >1% counts almost exactly. Mechanistically, replacing
  `p_eff` makes the IBU efficiency equal to the response acceptance `body/(body+miss)`; the plain
  prior's ±2.8% bin-by-bin deviation from the truth marginal enters directly as an efficiency bias.
* **Initialization-only is negligible:** median ~0.03%, zero bins above 2% for either charge, and a
  recomputed integrated shift below 0.001%. Four D'Agostini iterations suppress the prior
  initialization wherever migration constrains the bin.
* Note: `summary.json`'s attribution blocks repeat the coherent integrated shift in their
  `integrated_relative_shift` field for all variants (summarizer quirk: the field is hardwired to the
  coherent numerator). The per-variant integrated shifts in the table above were recomputed at
  completion time from the stored variant CSVs (`sum(yield_unfolded)/L` per charge), which is the
  correct attribution; the per-variant medians/maxima/threshold counts in `summary.json` are
  variant-specific and were independently reproduced from `coherent_vs_baseline.csv`.

## 6. Acceptance-unaffected confirmation

* The release acceptance `A_j = body_j/(body_j+miss_j)` is computed from the response alone and never
  touches the stored plain prior (investigation §"Which prior central IBU actually consumes";
  critic refinement recorded in the 2026-10-05 prior-mismatch critic reports). It is therefore **unaffected** by the defect, and
  the acceptance column inherited by all study variants is unchanged from the central release.
* BBB, being the diagonal acceptance correction, is likewise unaffected.
* The exact baseline gate (3.3e-15) additionally proves the study's control is the true central
  semantics, so the reported deltas measure the fix and not implementation drift.
* Under the fix, IBU's internal efficiency becomes *identical* to this acceptance, i.e. the fix
  removes an inconsistency rather than introducing a new convention.

## 7. Provisional status and formal-adoption requirements

This study is a **PROVISIONAL INTERNAL DIAGNOSTIC**. It uses raw data (no bgsub beyond the
response-derived fake fraction), no radiative correction (`C_rad`), and is not Born level; it is not
a release product and modifies nothing central. The adoption decision recorded in STATE.md
("Decision on adopting the coherent prior into central production still pending") remains open, and
the central release retains plain-prior semantics until it is made.

Formal adoption would require, at minimum:

1. **Builder-level coherent rebuild** of the response and stored prior — fill the consumed IBU prior
   as the exact response column marginal, or write and explicitly consume the truth-coherent NWeff
   object (matched legs `w_gen*NW_partner`, misses `w_gen`) — applied consistently to the RAPGAP and
   DJANGOH response/prior pairs. Do **not** patch central histogram bins in place.
2. **Merge-time column-conservation assertion** (`|body+miss − prior| ≤ 1e-6`) so incoherence cannot
   silently recur (fork-proven achievable: the flat-phi fork closes at exactly 1).
3. **Central release regeneration** from the rebuilt inputs: response + prior + release QA rerun, and
   an iteration/regularization diagnostic re-check (scan k when response/prior handling changes; do
   not assume k=4 transfers blindly).
4. **Downstream regeneration of everything derived from the central cross sections:** shifted-response
   comparisons, model/envelope systematics (including the flat-phi envelope and phi_h floor
   package), floored total-uncertainty budgets, smoothed values, and release plots — all currently
   built on plain-prior numbers.
5. **Record updates:** STATE.md blocker closure, release metadata, and refreshed critic review of the
   regenerated chain.

## 8. Artifacts

All under `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/coherent_prior_ibu_z005_20261005/`:

* `run_coherent_prior_study.py`, `run.log` — driver and stdout summary (status PASS).
* `validation/validation.json` — all gates above (status PASS).
* `comparison/summary.json` — baseline gate, coherent-vs-baseline statistics, attribution, prior
  input (status PASS).
* `comparison/coherent_vs_baseline.csv` — 490 rows, 49 columns: per-bin central-release row plus
  `baseline_cross_section_nb`, `coherent_cross_section_nb`, `central_release_cross_section_nb`,
  `delta_rel_coherent_vs_baseline`, `delta_rel_coherent_vs_central_release`,
  `delta_rel_initonly_vs_baseline`, `delta_rel_effonly_vs_baseline`. (Note: its `yield_unfolded` /
  `cross_section_nb` / uncertainty columns are the central release row copied through; the
  study-specific values are the `baseline_*`/`coherent_*`/`delta_*` columns.)
* `comparison/coherent_vs_baseline_plotting.csv` — **derived plotting-ready CSV** (written at
  completion; pure column subset, no recomputation, 490 rows, 24 columns): `charge, bin_index,
  global_bin_index, cell_label, q2_stratum, q2_min, q2_max, x_subbin, x_min, x_max, z_idx, z_min,
  z_max, pT_idx, pT_min, pT_max, baseline_cross_section_nb, coherent_cross_section_nb,
  central_release_cross_section_nb, delta_rel_coherent_vs_baseline, delta_rel_initonly_vs_baseline,
  delta_rel_effonly_vs_baseline, stat_unc_nb, total_unc_nb`.
* `ibu_{baseline,coherent,effonly,initonly}/cross_sections.csv` — full per-variant outputs (490 rows
  each), including `prior_initialization`, `prior_efficiency`, and `efficiency` columns.
* `comparison/prior_variants_comparison.json`, `audit_nweff_availability.py`,
  `nweff_availability.log`, `nweff_status.txt` — NWeff-availability evidence (no stored NWeff
  object; central file unchanged).
* `README.md`, `status.txt` — study status (`COMPLETED 2026-10-06 validation PASS comparison PASS`).

This report: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/unfolding/coherent_prior_fix_results_2026-10-05.md`

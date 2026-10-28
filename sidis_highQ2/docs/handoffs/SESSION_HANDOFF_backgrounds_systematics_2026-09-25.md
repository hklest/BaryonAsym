# Session Handoff — Backgrounds & Systematics, 2026-09-25

Scope: high-Q2 inclusive charged-hadron SIDIS (`Q2 > 150 GeV^2`), v6/z005.
Status of everything below: **PROVISIONAL INTERNAL DIAGNOSTIC**. Nothing here is approved physics.

---

## 1. TL;DR for the next session

Four things happened:

1. Built canonical 7-column expanded-systematics plots (cut shifts + background) and added them to the analysis note. **The note text is now known-wrong and must be fixed** — see §6.
2. The 2026-09-25 fLumi unit correction invalidates the apparent huge event-level template magnitude: `H1SteerTree::fLumi` is `pb^-1`, not `nb^-1`; see §2.
3. The `background_rel` retirement was partly justified by that invalid magnitude. Its justification is void; the decision **REQUIRES RE-EXAMINATION**, while track-level work remains BLOCKED on two missing arrays — §3.
4. Audited whether background subtraction in the central unfolding is correct. Verdict **INCOMPLETE**, plus a **new blocking finding** on response/prior conservation — §4.

Highest-value next action, in my judgement: **§4 response/prior conservation**. It touches the central result, not just a systematic.

---

## 2. 2026-09-25 fLumi correction — event-level template magnitude revised

`H1SteerTree::fLumi` is in `pb^-1`, not `nb^-1`. H1OO source evidence: `H1Analysis/H1Analysis.C:1012` converts `runList->GetIntLumi()` with `/1000.0` to work in pb^-1; `H1AnalysisChain.C:649` passes `SteerTree->GetLumi()` to `H1CalcWeight::SetMCLumi`; and `H1CalcWeight.C:324-339` applies `fDataLumi/fMCLumi`. `H1Skeleton/H1SteerTree.h:145` and `.C:63-66` establish the field role. `H1SteerTree.C:277-287` labels related `fFileLumis` as `Lumi(pb-1)` only as contextual support, not as a direct unit declaration for `fLumi`.

The DjBkgd steering sum is `15813.4 pb^-1 = 15813400 nb^-1`: 80 chains over five ReqIDs/four periods — 7087 em0405 `4557.60`, 7104 em06 `3690.90`, 7105 ep0607 `1708.80`, 7107 ep0607 `2563.20`, 7110 ep0304 `3292.90` pb^-1. All five samples, including 7110, are `NC RAD Q2GT2 CTEQ6L`. The five-ReqID sum is correct; the prior claim of an over-scale from comparing it with one ReqID was wrong. The error was purely the unit.

At `L_data=351000.0 nb^-1`, correct scale is `0.0221963651` and `L_MC/L_data=44.34`. Event-level lowQ2 leakage is `276.5991634` events. The all-five background total is `308932.4938634` events, and the integrated fraction is `7.953779%`. These absolute template-derived values are **not final**: `15813.4 pb^-1` is the framework-correct unit interpretation of the steering-chain sum, but is not independently provenance-closed as the exact template input exposure. oomclumi/generator-log accounting over exact resolved HAT lists for ReqIDs 7087, 7104, 7105, 7107, and 7110, including duplicate/overlap audit, remains outstanding.

`background_rel` used `delta_BG = 0.5 * B / Y` from the event-level lowQ2 leakage template. In h- `bin_index 34`, corrected `0.5*B/Y = 0.0055214` (`0.55214%`), not a 552% excursion. The characterization of this as a genuine sparse-yield bin is invalid.

Per-event weight `= sumw/n_raw` across populated bins:

| charge | populated bins | min | median | p99 | max |
|---|---:|---:|---:|---:|---:|
| h+ | 95 | 0.9728 | 0.9958 | 1.3410 | 1.3512 |
| h- | 81 | 0.9820 | 0.9959 | 9.9446 | **43.1292** |

Global mean weight `1.0380`. The h- bin-34 event is a **42x outlier**, the largest in all 490 bins; it remains untraced and worth investigating, but does not produce the prior huge excursion.

**Indexing was explicitly verified correct**: `i245 == bin_index` with **0/245** kinematic mismatches (`z_idx`, `pT_idx`, `z_min`, `pT_min`) against the release. The only sidecar-vs-release difference is the cosmetic label `S3_xmerged` vs `S3_x0` in 35 rows.

Event-level lowQ2 `276.5991634` and track-level lowQ2 `208.3795` are distinct objects but the same order of magnitude; the previous 1349x claim is invalid.

Template: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/bg_subtraction/lowq2_djangoh_v6_bprime_245/`

Do not present the corrected absolute yield as final pending the outstanding exposure-provenance closure.

---

## 3. Track-level background systematic — BLOCKED

User decision had been to assign the background uncertainty on **track-level** backgrounds only and retire the event-level term. Because the apparent huge event-level template magnitude was part of that justification and is invalid after the fLumi correction, the retirement decision **REQUIRES RE-EXAMINATION**; do not silently retain or reverse it.

Track-level components (alpha0p2), generator-averaged RAPGAP31/DJANGOH14, at `L_data = 351000.0 nb^-1`.
All independently reproduced exactly:

| component | integrated |
|---|---:|
| electron | 148976.4352 |
| V0 daughter | 132065.2768 |
| det.int. (GEANT/STR charged-hadron secondaries) | 27405.8031 |
| lowQ2 **track-level** | 208.3795 |
| **total** | **308655.8946** |
| fraction of denominator 3865073.0 | 0.0798577141 |

Source package:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/`

Work written to:
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/systematics/background_tracklevel_alpha0p2_2026-09-25/`

### Why it is blocked — two concretely missing arrays

1. **No `overlap_980`.** The electron-V0 overlap is only available as the integrated scalar `112.2093`
   (0.0753% of electron, 0.0850% of V0, 0.0364% of total). Without a per-bin vector, exactly-once accounting
   cannot be done per bin. det.int. overlaps with electron and V0 are zero; track-level lowQ2 is source-distinct.
2. **No `n_raw_980`.** The aggregates store `yield_980`, `var_980`, `sumw_980`, `sumw2_980` but no per-bin raw
   counts, so the `sumw/n_raw` weight-outlier audit **that caught the §2 bug cannot be run on this package**.

Consequence: per-bin distribution, bins >5%, and `B_track > Y` counts are all **UNEVALUATED**. No per-bin
source was fabricated, deliberately.

Also still open: **P-ALPHA** response-consistency. With the unchanged alpha0p2 response, V0 may be
pre-subtracted/vetoed, non-V0 electrons are fake-column-only and must not also be pre-subtracted, and det.int.
(~86% matched physical / ~14% fake) must not be blindly pre-subtracted.

**To unblock:** regenerate the alpha0p2 aggregates with `overlap_980` and `n_raw_980` included.

---

## 4. Is background subtracted correctly in the unfolding? — INCOMPLETE + new blocker

Central release under audit:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`

### What central actually does today

Producer: `.../_run/run_bbb_ibu_release.py`, importing `load_response_and_data` / `derive_response_blocks`
from `scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`.

Response fake-fraction subtraction **only**, applied once:
```
f_i   = F_fake / (B.sum(axis=1) + F_fake)
d_sig = data_fine * (1 - f_i)
dvar  = data_fine_w2 * (1 - f_i)^2
```
Removes 10.7141% (h+) and 10.4019% (h-).

Central `metadata.json` `caveats` literally contains `raw/no-bgsub`.

### Does central consume any track-level background? NO

No alpha0p2 vector, no all-fake response, **neither** low-Q2 template touches central.
Tagged electrons and V0 daughters are **vetoed in selection** (`pass_reco_full`), not subtracted.
No double counting found. But this differs from the fake-column reference convention and needs its own closure.

### Clean results

- negative raw reco inputs: **0 / 980**
- negative fake-subtracted reco inputs: **0 / 980**
- bins where fake removal exceeds data: **0 / 980**
- negative unfolded cross sections: **0 / 490** IBU, **0 / 490** BBB

### Weight-outlier audit on CENTRAL inputs — clean

Central response retains `h_mc_count_{pos,neg}`, so the audit was possible here:

| charge | global mean | min | median | p99 | max | bins >5x mean |
|---|---:|---:|---:|---:|---:|---:|
| h+ | 1.407944 | 1.000000 | 1.087888 | 3.682616 | 3.756705 | **0** |
| h- | 1.408244 | 1.000000 | 1.081047 | 3.617135 | 3.643910 | **0** |

The §2 pathology is confined to the retired template.

### NEW BLOCKING FINDING — response truth conservation fails

> RESOLVED (2026-10-06): root-caused; coherent-prior fix validated in fork, central adoption PENDING — resolved state and k=4 numbers in `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`. Finding body below stands as provenance.

`body + miss` does not reproduce the stored RAPGAP prior. **I recomputed this independently, column-wise**,
because the subagent had summed the miss row the wrong way (row-wise). Correct column-wise numbers:

| charge | body+miss | prior | total diff | per-gen-bin ratio min / median / max | bins off >1e-6 |
|---|---:|---:|---:|---|---:|
| h+ | 91879281.4817 | 91784225.0791 | +0.103565% | 0.964168 / 0.997185 / 1.028029 | **245/245** |
| h- | 90271538.7361 | 90185163.1419 | +0.095776% | 0.965375 / 0.994197 / 1.028519 | **245/245** |

Total offset is only ~0.1%, but **every single gen bin is off**, scattering **+-2.8%**. Scatter, not offset —
consistent with the prior histogram being filled with a different weight set or selection than the response.
This feeds IBU prior normalization and acceptance directly.

Shapes `(491,246)` per charge, 0 negative, 0 NaN/Inf. Efficiency ranges h+ `[0.0254083, 0.7647282]`,
h- `[0.0260073, 0.7644560]`; fake fractions h+ `[0, 0.3224391]`, h- `[0, 0.3067053]` — all within `[0,1]`.

Reco-side conservation is **UNEVALUATED**: only the matched reco marginal is stored (equals the body), no
stored total reco marginal including fakes. Recommend storing an explicit `body+fake` marginal.

### Covariance gaps

`h_data_bgsub_*_w2 ~= h_data_bgsub_*` to floating precision, i.e. effectively Poisson counting variance.
Response `sumw2` histograms exist but are **not** propagated. Published 1000-rep IBU covariance Poisson-fluctuates
data with response/fake fraction **fixed**. Absent: fake-column uncertainty, response-MC statistical covariance,
background scale/shape covariance.

Audit artifacts: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/`

---

## 5. Event-level background inventory — only one sample exists

Asked whether Compton / DIFFVM / Grape templates exist. Answer: **no**, and for documented reasons.

`output_v6/central/sidistree/mc_bg/` contains exactly one directory, `djangoh14_lowQ2bkgd`.
Steering has only `data/` and `mc/`.

Per `docs/research/research-ep0304-production-path-2026-05-24.md` §1.2:

| sample | upstream | lumi | disposition |
|---|---|---|---|
| Compton | `compton20/8346` | 1414.1 nb^-1 | removed by event-level cut |
| Grape | `h1grape/7128` | 4931.28 nb^-1 | removed by event-level cut |
| DVCS | `tintin/8052,8051` | - | removed by event-level cut |
| CC | `djangoh14/7339,7348` | 10910 nb^-1 | irrelevant for NC SIDIS at S67 |
| photoproduction | `pythia62/72xx` | - | negligible at Q2>150, y<0.6 |

Cuts verified present in `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`:
- **CUT 13** non-ep background finders: `GetIbg(0,1,5,6,7)` with `HadPtElecPtRatio` logic (cosmic/beam-gas/halo)
- **CUT 14** anti-QED-Compton: `NEmParts>=2`, both `E>4.0` GeV, `E1+E2>18.0` GeV, acoplanarity `-cos|dphi| > 0.95`

Both have live counters `nBgFinderCut`, `nQEDComptonCut` in the stage-1 cutflow.
So Compton/Grape are handled **by selection, not subtraction** — no template, no systematic. Properly closed.

### DIFFVM is the one real gap

Unlike Compton/Grape, DIFFVM was never dismissed. It is currently an **assumed zero**, which the backgrounds
skill explicitly says is not acceptable without a validated assessment.

12 samples exist as low-Q2 v5g trees (~55 GB) at `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5g/output/mc/`:
- phi -> K+K-: 8408, 8413, 8418, 8423
- omega: 8409, 8414, 8419, 8424
- rho0 all-decay: **8445-8448 only** (never also 8407/8412/8417/8422 — would double count)

Unweighted (`weight1 = weight2 = 1.0`). Schema reuse by the high-Q2 builder is **unverified**.
Suggested cheap first step: gen-level count of rho/phi surviving `Q2>150` + SIDIS fiducial using existing v5g trees.

---

## 6. Analysis note — CONTAINS KNOWN-WRONG TEXT, FIX BEFORE CIRCULATING

Note: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/`
Added subsection `sec:syst:expanded-candidate-budget` in `sections/systematics.tex` (+155 lines, 6 figures).
Builds to **69 pages**.

**Build requires CVMFS TeX Live 2025:**
```
export PATH=/cvmfs/sft.cern.ch/lcg/external/texlive/2025/bin/x86_64-linux:$PATH
```
System `/usr/bin/pdflatex` lacks `tcolorbox.sty` and `cleveref.sty`.

### Must fix
`sections/systematics.tex` describes the 552% bin as a *"genuine sparse-yield bin"* and says it is
*"not a rendering error"*, implying it is physical. **It is a one-event weight artifact (§2).**
Offending lines: **153, 162-166, 187, 201, 228, 242**.
The three autoscale figures exist specifically to display that bad bin and should be removed with the text.

Given §3 retires the event-level background entirely, the cleanest fix is to drop `background_rel` from the
expanded budget and the note subsection, keeping the four cut-shift sources, and reinstate a background term
only once §3 is unblocked.

### Pre-existing note defects, NOT from this work
- 5 figure names in `sections/results.tex` omit the `_z005` suffix -> missing files, draft boxes on pages 14/27
- citation `2005Hadroo2` undefined

---

## 7. Artifacts produced this session

| path | contents |
|---|---|
| `sidis_highQ2/plots/systematics/canonical_7panel_expanded_sources_2026-09-25/` | 6 PDFs (fixedaxis/zoom15pct/autoscale x h+/h-), 6-page review PDF, 6 review PNGs, plotter, manifest |
| `sidis_highQ2/plots/backgrounds/canonical_7panel_bg_yields_2026-09-25/` | background-yield 7col plots, linear+log per charge, 4-page review PDF, per-bin CSV, manifest |
| `sidis_highQ2/data/systematics/background_tracklevel_alpha0p2_2026-09-25/` | track-level derivation, BLOCKED, validation.json records why |
| `sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/` | central subtraction audit numbers |

### Canonical 7-column layout — settled, do not re-litigate
"7col" = **7 z-columns x 7 (Q2,x) rows = 49 panels per charge**, `panel_count_total = 98`.
Confirmed by byte-identical match to the four pre-existing note figures.
Producer: `output_v6/systematics/summary_plots/total_uncertainty_z005_2026-07-03_with_tracking/canonical_7col_with_tracking_2026-07-04/plot_fractional_uncertainty_sources_7col_pTh_bycharge_with_tracking.py`
A plot reviewer wrongly flagged the 49-panel grid as a defect this session. It is correct.

### Background-yield plot totals (all reproduce exactly)

| source | integrated @ L351000 | zero bins /490 |
|---|---:|---:|
| electron | 148976.4352 | 11 |
| V0 daughter | 132065.2768 | 1 |
| det.int. | 27405.8031 | 4 |
| lowQ2 (track-level) | 208.3795 | 340 |
| lowQ2 (event-level leakage) | 276.5991634 | 314 |
| **TOTAL** | **308932.4938634** | 0 |

TOTAL is the corrected sum of five components; it is provisional because the event-level template exposure is not provenance-closed.

**Critical distinction:** event-level lowQ2 leakage (`276.5991634`) and track-level lowQ2 (`208.3795`) are different objects but the same order of magnitude. Never sum them as one source.

Reco-980 -> published-490 collapse uses `h_binmap_gen_to_reco` from the central migration ROOT:
`out[ch*245 + binmap[i]] += yield_980[ch*490 + i]`. Acceptance check: collapsed electron total must equal
`148976.4352`. Verified.

### Expanded systematics source table (unchanged, still valid for the 4 cut shifts)
`sidis_highQ2/plots/systematics/uncertainty_7col_all_sources_binwise_bg_2026-09-24/source_table.csv`

| source | median | max | bins >5% |
|---|---:|---:|---:|
| `background_rel` | 0.0000 | 0.0055214 | 49 | <- retirement REQUIRES RE-EXAMINATION; corrected template exposure remains provisional, see §2/§3
| `compound_track_quality_rel` | 0.00857 | 0.17456 | 42 |
| `empz25_rel` | 0.00343 | 0.47608 | 26 |
| `labpt_rel` | 0.00043 | 0.11146 | 1 |
| `dca_scan_rel` | 0.00019 | 0.02099 | 0 |
| `total_with_tracking_rel` | 0.0609 | 1.6251 | - |
| `expanded_total_rel` | 0.0784 | 5.5261 | - |

Verified on all 490 bins: quadrature identity to 1e-12, expanded >= nominal, no component exceeds its total,
`lumi_rel` exactly `0.027`. The eta-cut variation is deliberately **excluded**.

---

## 8. Recommended order for next session

1. **§4 response/prior +-2.8% conservation mismatch.** Affects the central result. Highest value.
2. **§3 unblock**: add `overlap_980` and `n_raw_980` to the alpha0p2 aggregates, then build and weight-audit
   the track-level background systematic.
3. **§6 note fix**: correct the stale "genuine sparse-yield bin" / 552% text and autoscale-figure interpretation; the corrected excursion is `0.55214%` and the absolute template exposure remains provisional.
4. **§2 trace** the weight-43.1292 event to its source file/event.
5. **§5 DIFFVM** gen-level survival count at Q2>150.

# Background-subtraction systematic — final scheme proposal (user sign-off)

- **Date**: 2026-10-18. **Author**: h1-systematics (final synthesis session).
- **Status**: PROPOSAL. Nothing installed; the canonical clean release
  (`bbb_ibu_bgsub_setRprime_conebase_..._2026-10-17/`, Born
  `born_crad_bgsub_setRprime_conebase_..._2026-10-15/`, budget
  `systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17/`) and both
  campaign trees are untouched. `L_data = 351000.0 nb^-1` only.
- **Inputs** (all read-only; sha256 recorded in the summary JSON):
  canonical 19-comp budget CSV; Set-R' 490 release-bin vectors CSV
  (`.../stage2_setrprime_2026-10-15/vectors/bgsub_setrprime_vectors_490.csv`);
  Path B stability term CSV
  (`.../stage3_tightdca_2026-10-17/stageE_toys/stability_term_tightdca_cone_2026-10-17.csv`);
  Path A review + R2 appendix (`docs/backgrounds/fitnorm_pathA_review_2026-10-17.md`);
  tightened-chain anchors JSON; Born release `summary_totals.json`.
- **All numbers below were recomputed from these artifacts by the keepable
  script** `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_final_scheme_2026-10-18/analysis_bgsub_final_scheme_2026-10-18.py`
  (validation gates V1–V6 asserted in-script, §6). Path B's own headline
  statistics are reproduced exactly (V5): medians 0.004609/0.005429, max valid
  0.078002 (neg bin 39), 40 excluded (18/22), 87/450 bias-flagged, p90 0.025757,
  p99 0.061576.

---

## 1. Agreement check — Path A (fitnorm) vs Path B (tightened chain)

**What each path measures.** Path A (R1+R2) attempted to *measure* the e/V0
(Set-R') template normalizations in the |DcaPrime|>0.25 cm control tail and
failed: the model is unidentifiable (mu_V0 pinned at the 0 bound in 4/4 h+
groups while mu_e floats to 2.5–3.5 at chi2/ndf 4.98; the C2 signal-float
variant flips the decomposition; photoproduction consistent but covers only
8.5% of the fitted excess). What survives from Path A: the *central value is
safe* (gate-passing groups consistent with mu=1; the feared -6.5% shift is
dead) and a residual **~1.4% of D_cone of unexplained |DcaPrime|>0.25 cm tail
excess of unknown composition** (junk-photo floor 0.12%). Path B measures the
*effect on the corrected result* of removing ~23–30% of that tail content by
rebuilding the entire chain under `|DcaPrime| < 0.3 cm` (tightened data
vector, responses, templates, IBU k=4, Born; background fraction 7.17/7.39% →
5.57/5.69% integrated f_res, i.e. 22.3/23.0% of the subtracted content
removed; D-side lever rejection r_D 8.14/7.40%).

**The statistical question.** The shared-Barlow-toy construction gives
Var(Delta) = Var(nom) − Var(tight) exactly for the nested tightened sample
(CC-7 identity PASS 100% of eligible bins both charges, median residual 0.22%
of VarT), so sigma_Delta **is** the correct statistical expectation of the
difference under the null "the lever only re-thins the same statistical
sample". Under pure noise the two-sided 3-sigma exceedance probability is
0.0027, i.e. **450 x 0.0027 = 1.21 expected flags**. Observed: **87/450**
(pos 46, neg 41) — Poisson p ≈ 3.2e-126. The flags are if anything
*under*counted: the 123 guard-flagged bins use the conservative
Var(nom)+Var(tight) fallback (larger sigma_Delta), and the primary term is
central-value-based, not toy-based. **Conclusion: the lever shift is NOT a
statistical fluctuation; it is a real, resolved, few-per-mille-to-few-percent
systematic effect.**

**Is it the kind of effect the envelope covers?** Fit the model
Delta_i ≈ −kappa · f_i · nom_i, with f_i = (e_avg+v0_avg)/data_sub' from the
released stage-2 vectors: kappa_median = **0.156 (h+) / 0.161 (h−)** (i.e. the
lever moves the corrected result by ~16% of the local e+V0 content),
yield-weighted kappa = 0.279/0.160, Spearman(term, f) = 0.33/0.27 (moderate:
f-proportional in magnitude with bin-to-bin scatter). With kappa_median the
model predicts **133** 3-sigma flags (pos 65 / neg 68) — the right order as
the observed 87, vs 1.2 under noise. The shift therefore behaves like a
background-content-normalization effect of size ~16–28% of content, precisely
the class of effect a ±100% content envelope is built to cover.

**Do the two paths agree?** Yes, quantitatively. If the unexplained 1.4%-of-
D_cone tail excess were entirely spurious background, removing it should move
the corrected result by −1.4%/92.7% ≈ **−1.51%**. Path B observes integrated
**−2.38% (h+) / −1.41% (h−)** raw IBU (10.508811→10.258462 /
10.217503→10.073554 nb) and **−2.37% / −1.40%** Born (11.996558→11.712309 /
11.663956→11.500379 nb). h− matches the naive expectation; the h+ excess
(~−0.9 pp) is within the signal-Dca-tail mis-modeling interpretation Path A
admits (R2: displaced DIS track component off at the mu_s = 1.02–1.16 level).
Both paths say: real, few-percent, composition mixed (background + signal
tail mis-modeling), **no central-value change**.

**VERDICT: DEMOTE the completeness concern.** The R2 review's demotion
criterion — "stability within envelope" — is satisfied twice: integrated
(the lever moves the result by 31% (h+) / 18% (h−) of the envelope's coherent
size, §2) and per-bin (all 87 statistically significant shifts lie strictly
inside the ±100% envelope; §2). What *would* have escalated, and does not
occur: (i) any significant per-bin shift outside the envelope, (ii) an
integrated shift exceeding the envelope, (iii) a stability term incompatible
with an f_i-scaled content effect. The residual tail excess is recorded as
**covered, composition unknown, no central-value change**.

---

## 2. Final component proposal

### 2.1 New component: `bgsub_env100_rel`

```
bgsub_env100_rel,i = 1.00 x (e_avg,i + v0_avg,i) / data_sub'_i
```

the fiat ±100% correlated normalization envelope on the subtracted
electron+V0 content (R2 ruling item iv; lowQ2 NOT enveloped — fixed at
template, its ±30% leg lives in bgsub_model). f_i = 0 on the three empty-D
bins (pos 109, pos 179, neg 240; house release convention). The f_i
computation is validated against the pre-registered A2 anchor of
`apply_fitnorm_to_budget.py`: median(f)x0.25 = 0.7896%/0.8909% exactly.

**Medians: 3.1584% (h+) / 3.5635% (h−)**; max 28.39%/29.93% (all <50% → no
R3 flag, <100% → no R4 stop). The envelope becomes the **largest
non-statistical component** of the budget (vs lumi 2.7%, elec_energy
1.65/1.71%, phi_h 1.10/1.10%), second only to the statistical ibu_stat
(3.73/4.04%).

### 2.2 Bookkeeping: which bgsub legs are REPLACED vs KEPT

| component | median h+/h− (%) | disposition | reason |
|---|---|---|---|
| `bgsub_stat_rel` | 0.0799 / 0.0954 | **REPLACED (retired into the envelope)** | It is the template-MC *normalization* variance of the same subtracted content (sqrt(varB_R')/data_sub'); a ±100% normalization envelope strictly contains it. Its lowQ2 variance sliver is negligible (lowq2_var ~11 vs e_avg_var ~275). Cost of retiring: **−0.0005/−0.0009 pp** on the total medians — immaterial; retained option changes nothing (both computed, §2.4). |
| `bgsub_model_rel` | 0.2016 / 0.2277 | **KEPT** | R2 ruling: "quadrature with retained half-spreads". The generator half-spreads (e RG/DJ, V0, lowQ2 ±30%) are the only data-driven model-disagreement leg on these templates; formally subsumed at ±100% but retained at negligible quadrature cost (~2.5% of content vs the envelope's 100%). |
| `bgsub_stage_rel` | 0.2103 / 0.2453 | **KEPT** | Methodological (pre-vs-post-unfold placement of the subtraction), not a content normalization; the budget's own `double_count_watch` declares it DISTINCT from content legs. The envelope does not cover placement coherence. |
| `bgsub_fakefrac_rel` | 0.1828 / 0.1804 | **KEPT** | Response-internal fc0 common-model proxy (fake column, r_min = 0.9817, f_i,standing) — a different object from the external Set-R' e/V0 content; untouched by the envelope. |
| `bgsub_env100_rel` | 3.1584 / 3.5635 | **ADDED** | Fiat ±100% correlated envelope (§2.1). |
| Path B stability term | 0.461 / 0.543 | **ABSORBED (not a component)** | See §2.3. |

Net structure: **19 components** (18 carried + envelope), preserving the
19-comp count and the single-denominator discipline. The R2-literal
alternative (20-comp: add the envelope, keep bgsub_stat) is also computed and
differs by <0.001 pp (§2.4); it is the zero-machinery-change fallback.

### 2.3 Keep-vs-absorb the Path B stability term — ABSORB

The stability term (medians 0.461%/0.543%) is ~1/7 of the envelope at the
median, but the median-to-median ratio is the wrong test: the stability term
is heavy-tailed (p90 2.58%, p99 **6.16%**, max valid 7.80%), and the honest
question is per-bin coverage. Results (449 valid bins with f>0):

- **All 87 statistically significant lever shifts (|Delta|>3sigma_Delta) lie
  strictly inside the per-bin envelope**: ratio stability/env median
  0.267 (h+) / 0.164 (h−), max **0.974 (h+) / 0.672 (h−)**; zero
  significant bins outside.
- **19 bins (9 h+ / 10 h−) have ratio > 1** (max 5.39, pos bin 204) — but
  every one of them is statistically *insignificant* (|delta/sigma| ≤ 2.5,
  i.e. compatible with noise; nom_yield 52–842, just above the nom<50
  exclusion edge; 13/19 at the pT_idx=4 edge, 13/19 at z≥3; 5/19
  guard-flagged with conservative sigma). The envelope has no obligation to
  cover noise. Full list: `coverage_ratio_gt1_bins_2026-10-18.csv`.
- **The 40 stability-excluded bins (nom<50)** have no stability measurement at
  all — a kept `bgsub_tightstab_rel` would need an arbitrary convention
  there (a zero would claim *no* uncertainty in the sparsest bins —
  backwards). The envelope is defined in exactly those bins (medians
  1.67%/2.72%), so it is the more complete instrument.
- **Keeping it would double-count**: the significant part of the lever effect
  is an instance of the content-misspecification class the envelope already
  prices; adding it in quadrature costs **+0.11/+0.07 pp** on the valid-bin
  medians (7.08→7.19 / 7.31→7.39) with no new physics.

**Recommendation: absorb.** The Path B term enters the record as the
*empirical validation* of the envelope (the R2 ruling's own role assignment:
"Path B can settle whether published values move when the tail is excluded"),
not as a 20th/21st budget component. Its residual non-covered part is
statistically unresolved in 19 sparse edge bins and is carried by the
envelope's own per-bin size there.

### 2.4 New totals (recomputed per-bin from the canonical CSV)

| total | median h+ | median h− | bins >100% | max h+/h− (%) |
|---|---|---|---|---|
| `total_with_bgsub19_rel` (current, carried) | 5.9764% | 6.2968% | 0 / 0 | 73.7 / 87.0 |
| **`total_with_bgsub19env_rel` (RECOMMENDED: retire stat, add env)** | **7.3203%** | **7.6371%** | **0 / 0** | 73.7 / 88.4 |
| `total_with_bgsub20_rel` (R2-literal: add env, keep all 19) | 7.3208% | 7.6380% | 0 / 0 | 73.7 / 89.6 |

Increment of the recommended scheme: **+1.344 pp (h+) / +1.340 pp (h−)**.
Note: the R2 appendix projected "totals ~6.8%/~7.2%" from quadrature-of-medians
(sqrt(5.98²+3.2²)); medians do not commute with per-bin quadrature (t19 and
env co-vary in the sparse bins), and the correct per-bin computation gives
7.32%/7.64%. The per-bin values are authoritative.

**Coherent integrated effect** (the envelope is fully correlated, so its
integrated one-sided size is the content-weighted mean, not the median):
eps = sum(e_avg+v0_avg)/sum(data_sub') = **7.716% (h+) / 7.973% (h−)**, i.e.
**±0.926 nb on Born h+ 11.996558 nb and ±0.930 nb on Born h− 11.663956 nb**
(±0.811/±0.815 nb on the RAW IBU values). The observed Path B integrated
shift (−2.37%/−1.40% Born) is **31%/18% of the envelope** — the propagation
is empirically sub-linear-to-linear (22–23% content removal → 2.4/1.4%
result shift; no amplification through the unfolding), confirming that the
input-level envelope is not under-transferred.

### 2.5 Double-count audit vs existing selection/track-quality systematics

- The 19-comp column list contains **no DcaPrime-based component**: grep of
  the canonical builder
  (`.../cone_15comp/scripts/stage2_cone_2026-10-15/run_bgsub_budget19_cleandenom_cone.py`)
  and of all 12 rederivation records for "dca": **zero hits**.
- `tracking_eff_m0p5_rel` is a per-track **efficiency** variation (16 seed
  data-vectors 314159..314174 + no-cone baseline, differenced through the
  nominal chain; caveat OPEN) — a different axis from a Dca *shape/selection*
  cut; no overlap with the Path B lever.
- The budget's own `double_count_watch` separations remain valid:
  ibu_stat (data-Poisson toys, w2=data_sub') vs bgsub_stat (template
  variance) were DISTINCT — with bgsub_stat retired into the envelope this
  watch item simplifies; detector legs vs bgsub_stage remain DISTINCT.
- The envelope's only internal double-count is the retained bgsub_model
  (strictly subsumed, ~1/40 of the envelope in size) — retained by explicit
  R2 ruling, cost negligible.

---

## 3. Correlation structure of the fiat term

- **Fully correlated across all bins within each charge** (a single
  normalization doubt on the common e/V0 template content), and to be
  treated as **fully correlated between charges** for any future h+/h−
  ratio or asymmetry use (the template doubt is common; the per-bin f_i
  shape differs). This mirrors the existing `lumi_rel` precedent: a flat /
  structured per-bin column carried in the CSV with the correlation
  *declared* in the budget metadata — the CSV/quadrature machinery carries
  no inter-bin covariance object.
- **Per-Q2-stratum blocks are NOT recommended**: the machinery already
  supports them (`apply_fitnorm_to_budget.py` takes per-(charge x S0..S3)
  factors — 8 groups), but R2 established the per-group normalizations are
  *unidentifiable*, so there is no evidential basis for stratum-resolved
  factors, and blocking would reduce the coherent integrated effect
  (under-counting the correlated component). A single envelope with factor
  1.0 in all 8 groups is both simpler and conservative.
- **Implementation hook** (all validated, candidate-dir-only; installation to
  `data_release/` remains a user-gated promotion):
  - Script: `H1ChargedSidis_HighQ2/scripts/bgsub_fitnorm_2026-10-19/apply_fitnorm_to_budget.py`
    — reproduces the released STEP-12 quadrature chain bit-exactly; inject
    mode `--inject-mu-comb 0.0 --inject-sigma-comb 0.0` gives factor
    hypot(|0−1|, 0) = 1.00, i.e. exactly the envelope; anchors A1/A2/A3
    pre-registered and reproduced (A2 by this study's V3 gate).
  - Columns: adds `bgsub_fitnorm_rel` + `total_with_bgsub20_rel`
    (20-comp add variant, machinery-ready today). For the recommended
    19-comp variant, adopt with the column renamed **`bgsub_env100_rel`**
    and add a `--retire-bgsub-stat` flag (one-line quadrature change;
    total column `total_with_bgsub19env_rel`).
  - Canonical builder to extend for the adoption edition:
    `run_bgsub_budget19_cleandenom_cone.py` (STEP 12 totals chain).
  - R-gates: envelope max 29.9% → no R3 (f>0.5) flag, no R4 (>100%) stop;
    medians 3.16/3.56% pass R5 plausibility; R1 not applicable (fiat, z
    undefined).
- A dated adoption edition should also record the correlation declaration in
  its budget JSON (new-CP-5-style key), as `lumi_rel` does.

---

## 4. Quoting-rule update (release-note text; supersedes the R2 interim sentence)

> The electron and V0 (Set-R′) subtraction templates are MC-derived and
> remain unvalidated by data: dedicated control-region fits could not
> measure their normalizations — the two-class |DcaPrime|-tail model is
> unidentifiable (μ_V0 pins at its lower bound while μ_e drifts to 2.5–3.5 at
> χ²/ndf ≈ 5, and the decomposition flips under a floated signal), while the
> gate-passing groups are consistent with unity — so the template
> normalization uncertainty is carried as a fully correlated, ±100% fiat
> envelope on the subtracted electron+V0 content (per-bin
> (e_avg+v0_avg)/data_sub′; median 3.16% (h+)/3.56% (h−); the largest
> non-statistical component of the budget). A full tightened-selection
> stability rebuild (|DcaPrime| < 0.3 cm applied to data, responses,
> templates and unfolding) moves the corrected result by only −2.4%/−1.4%
> integrated (median per-bin 0.46%/0.54%), i.e. by at most about a third of
> the envelope's coherent size, and every statistically significant per-bin
> shift lies strictly inside the envelope; the residual
> |DcaPrime| > 0.25 cm tail excess (≲1.4% of the cone sample, composition
> unknown) is thereby covered and implies no central-value change. All quoted
> values state their level: RAW = official IBU k=4 on data_sub′
> (h+ 10.508811 / h− 10.217503 nb), Born = per-bin C_rad applied
> (h+ 11.996558 / h− 11.663956 nb); the quoted total is the adopted
> bgsub-envelope budget total (medians 7.32% (h+)/7.64% (h−)).

---

## 5. User decision list

1. **Envelope size: ±100% (recommended) or ±50%?**
   Recommendation: **±100%**. It is the smallest round number covering every
   admitted estimate (R1 blend μ 1.4–3.2; R2 ensembles μ_e 0.28–3.60,
   μ_V0 0–2.44), and the Path B measurement itself now demonstrates
   coverage *empirically*: a ±50% envelope would leave **10 of the 87
   statistically significant** lever shifts outside it (9 h+ / 1 h−) and
   would lower the totals to only 6.51%/6.76% — repeating, in weakened form,
   the demonstrated non-coverage that killed the flat-30% option.
2. **Bookkeeping: retire `bgsub_stat` into the envelope (recommended,
   19-comp `total_with_bgsub19env_rel`, medians 7.3203%/7.6371%) or the
   R2-literal 20-comp add (`total_with_bgsub20_rel`, 7.3208%/7.6380%)?**
   Recommendation: **retire** — the envelope strictly subsumes the
   template-stat normalization of the same content, the 19-comp count is
   preserved, and the difference is <0.001 pp. Choose the 20-comp add only
   if zero machinery change is preferred; both per-bin CSVs are delivered.
3. **Path B stability term: absorb (recommended) or carry as its own
   component `bgsub_tightstab_rel`?**
   Recommendation: **absorb** (§2.3): all 87 significant shifts are inside
   the envelope (max ratio 0.97/0.67), the 19 ratio>1 bins are all
   statistically insignificant sparse edge bins, the term is undefined in
   the 40 excluded bins where the envelope is not, and keeping it would add
   +0.11/+0.07 pp of pure double-count.
4. **Post-adoption regeneration sequencing.**
   Recommendation: upon sign-off, (i) build the dated adoption edition
   (candidate → gate → user-gated install) with the correlation declaration;
   (ii) regenerate the appendix cross-section tables
   (`tables/highQ2_z005_s2prime_born_public_*`) and the systematics 7-panel
   plots on the new total (P-PLOTS item); (iii) replace the note's interim
   bgsub sentence with §4 and update the budget medians quoted in
   `sections/systematics.tex`. Until then the interim sentence and the
   current `total_with_bgsub19_rel` (5.9764%/6.2968%) remain the quoted
   state; no released number changes as a result of this proposal alone.

---

## 6. Artifacts, validation, and paths

Study dir (all outputs): `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_final_scheme_2026-10-18/`

| file | content |
|---|---|
| `analysis_bgsub_final_scheme_2026-10-18.py` | keepable analysis script (read-only on all inputs; asserts V1–V6) |
| `bgsub_env100_term_490_2026-10-18.csv` | 490 rows: per-bin envelope term, joined stability term, sigma_delta, flags, `stab_over_env` coverage ratio |
| `bgsub_final_budget_terms_2026-10-18.csv` | per-bin `bgsub_env100_rel`, `total_with_bgsub20_add_rel`, `total_with_bgsub19prime_rel` (= recommended 19env), `..._plus_tightstab_rel` variant, carried `bgsub_stat_rel` |
| `coverage_ratio_gt1_bins_2026-10-18.csv` | the 19 insignificant ratio>1 bins, fully labelled |
| `bgsub_final_scheme_summary_2026-10-18.json` | all headline numbers + input sha256 + validation record |
| `systematics_bgsub_final_scheme_2026-10-18.json` | output-contract file (step/channel/status/bins/values/errors/validation) |

In-script validation gates (all asserted PASS):
V1 t19 medians == 5.976416521492156/6.296839261150574%;
V2 bgsub4 quadrature medians == 0.4445/0.4604% (A3);
V3 median(f)×0.25 == 0.7896/0.8909% (A2 anchor → machinery f_i convention);
V4 empty-D bins == {pos 109, pos 179, neg 240};
V5 Path B headline reproduction (medians/max/excluded/bias-flags/p90/p99);
V6 label join budget=vectors=stability 490/490.

Read-only inputs (canonical and campaign trees untouched):
- `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_setRprime_19comp_cleandenom_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/bgsub_setrprime_budget19_cleandenom.csv`
- `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/stage2_setrprime_2026-10-15/vectors/bgsub_setrprime_vectors_490.csv`
- `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/stage3_tightdca_2026-10-17/stageE_toys/stability_term_tightdca_cone_2026-10-17.csv`
- `.../stage3_tightdca_2026-10-17/stageD_unfold/tightened_chain_anchors_2026-10-17.json`
- `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-15/summary_totals.json`
- `sidis_highQ2/docs/backgrounds/fitnorm_pathA_review_2026-10-17.md` (R1+R2 rulings)

---

# Rev2 (2026-10-18): DCA-overlap audit and Path-B-only coverage re-derivation

This section responds to two user objections to the ±100% envelope raised on
2026-10-18. The rev1 text above is kept intact; where rev2 corrects it, the
correction is stated explicitly in §Rev2.5. Everything below was recomputed
from artifacts by the keepable rev2 script
(`.../bgsub_final_scheme_2026-10-18/analysis_bgsub_final_scheme_rev2_dcaaudit_2026-10-18.py`,
gates V1–V6 asserted in-script, §Rev2.6). Read-only on all inputs;
`L_data = 351000.0 nb^-1` for every current-basis quantity; the dca1/dca3
study ratios are L-cancelling (those studies ran at the historical
`L = 297657.8 nb^-1`).

**The objections.** O1: the R2 fit values (mu_e 2.5–3.6, V0-to-bound) were
extracted from the out-of-cone |Dca|>0.25 tail — a region with known
composition problems (junk-photo floor 0.12%, ~1.4% unmodeled tail excess,
signal-tail degeneracy) — so the admitted-mu-range argument for 100% is
contaminated; R2 is evidence the templates are UNVALIDATED, not evidence of
the magnitude of their error; magnitude must come from Path B (the published
selection). O2: "we already have the DCA shift as one of our uncertainties,"
so a full ±100% envelope is partly subsumed by that existing shift and would
overestimate.

## Rev2.1 DCA-overlap audit (resolves the factual conflict)

Full trace of every track-parameter-shift leg in the analysis inventory,
joined 490/490 on (charge, bin_index) with label checks (gate V1):

| Leg | Status | Variation that produces it | median h+/h− (%) | max h+/h− (%) | Physical overlap with the \|DcaPrime\|<0.3 lever |
|---|---|---|---|---|---|
| `tracking_eff_m0p5` | **IN the 19-comp budget** | per-track efficiency −0.5% random drop, 16 seeds (314159–314174), data-side, differenced through the nominal chain | 0.5132 / 0.5211 | 1.036 / 1.596 | **NONE** — flat efficiency vs DCA-shape selection; different detector effect |
| `dca_scan_rel` ("DCA scan") | DIAGNOSTIC ONLY — 7-col expanded-source table 2026-09-24/25 (status label: "Expanded total is NOT the final total") | `DcaCosh_abs` cut 2.0→1.0 (tighten) / →3.0 (loosen), full-chain cut-shift studies 2026-06-15 (finalcandidate basis); value = 0.5·\|1/r_dca1 − 1/r_dca3\| | 0.0157 / 0.0210 | 0.700 / 2.099 | **PARTIAL, far tail only**: probes \|DcaPrime\| ∈ [cosh η·1, cosh η·3] cm; the lever's effect is dominated by the near tail 0.3–cosh η·1 cm, which no existing leg probes |
| dca1 tightening, one-sided | DIAGNOSTIC ONLY (same studies, tightening direction, \|1/r₁−1\|) | same | 0.0314 / 0.0417 | 1.400 / 4.199 | PARTIAL, far tail only — used as the *aggressive* credit to bracket O2 |
| dca3 loosening, one-sided | DIAGNOSTIC ONLY | loosening direction | 0.0000 / 0.0000 | 0.0006 / 0.0036 | NONE (opposite direction of a tightening lever) |
| `compound_track_quality_rel` | DIAGNOSTIC ONLY (same table) | nhit 5→10 AND radlen 5→10, full-chain study 2026-06-15 | 0.8446 / 0.8732 | 12.77 / 17.46 | NONE — hit count + material budget, not impact parameter |
| `labpt_rel` | DIAGNOSTIC ONLY | lab-pT threshold 0.5→0.10 GeV | 0.0414 / 0.0451 | 11.15 / 3.75 | NONE (momentum threshold) |
| `empz25_rel` | DIAGNOSTIC ONLY | E−pz window variation | 0.3772 / 0.3336 | 20.54 / 47.61 | NONE (event kinematics) |
| `track_quality_dca` (v5g master-plan source 15) | PLANNED, NEVER RUN (phase-1 close 2026-05-24: "not_started"; v5g tree path dead; no artifact exists) | planned DCA/quality cut variations | — | — | N/A |

**Resolution of the conflict.** Both statements were partially right.

- The installed 19-comp budget contains **no DCA leg**: gate V4 confirms all
  19 canonical components by name and **zero** columns containing "dca"
  (the rev1 grep of the builder + 12 rederivation records stands). Its only
  track leg is `tracking_eff_m0p5`, an efficiency variation.
- The analysis inventory **does** contain a measured DCA shift: the
  2026-06-15 dca1/dca3 full-chain cut-shift studies, tabulated as
  `dca_scan_rel` and displayed as "DCA scan" in the expanded 7-col
  systematics plots. It has never been a component of any quoted budget
  (the expanded total is explicitly labeled "NOT the final total"). The
  user's O2 premise is therefore **affirmed on existence** and **corrected
  on bookkeeping**: within the quoted `total_with_bgsub19_rel` there is
  nothing for the envelope to double-count against.
- The leg the user most plausibly means is `dca_scan_rel` ("DCA scan");
  second candidate `tracking_eff_m0p5` (in-budget, but efficiency, not
  DCA); third the never-run v5g `track_quality_dca`.

## Rev2.2 Required-E distribution (O1+O2 sizing rule)

O1 is accepted: the R2 admitted-mu-range argument is **retired** as
magnitude evidence. The envelope is sized from Path B alone: per
significant bin, the minimal E satisfying sqrt((E·f_i)² + d_i²) ≥ |Δ_i|,
with f_i = (e+v0)/data_sub′, Δ_i = the Path B stability term, and d_i =
the overlapping leg's per-bin value. Over the **87 significant bins**
(46 pos / 41 neg), per charge (median / p90 / p99 / max in %; counts of
bins needing >30% / >50% / >100%):

| Scenario | h+ med/p90/p99/max | h+ >30/50/100 | h− med/p90/p99/max | h− >30/50/100 |
|---|---|---|---|---|
| (a) no credit — E = \|Δ\|/f | 26.67 / 63.60 / 96.09 / **97.45** | 20 / 9 / 0 | 16.37 / 27.84 / 58.24 / **67.24** | 3 / 1 / 0 |
| (b) quadrature credit, dca_scan | 26.56 / 63.46 / 95.96 / 97.41 | 20 / 9 / 0 | 16.37 / 27.73 / 58.12 / 67.23 | 3 / 1 / 0 |
| (b2) quadrature credit, one-sided dca1 (aggressive) | 26.20 / 63.05 / 95.59 / 97.31 | 20 / 9 / 0 | 16.36 / 27.37 / 57.75 / 67.20 | 3 / 1 / 0 |
| (c) quadrature credit, dca_scan + tracking | 24.47 / 62.84 / 94.75 / 95.65 | 20 / 9 / 0 | 13.06 / 26.09 / 57.04 / 65.92 | 3 / 1 / 0 |
| (d) fully-optimistic correlated subtraction, dca_scan | 24.34 / 59.47 / 91.68 / 94.90 | 20 / 8 / 0 | 16.00 / 25.55 / 55.49 / 66.06 | 3 / 1 / 0 |
| (d2) fully-optimistic correlated subtraction, dca1 | 22.18 / 55.33 / 87.28 / **92.36** | 17 / 7 / 0 | 15.21 / 25.20 / 52.95 / 64.88 | 3 / 1 / 0 |
| (e) 3σ-discounted floor, no credit: E = max(0,\|Δ\|−3σ_rel)/f | 11.62 / 37.84 / 47.04 / **47.52** | 7 / 0 / 0 | 7.17 / 15.84 / 20.02 / 20.30 | 0 / 0 / 0 |

Driving bins (the max-requirement bins; both are barely-resolved):
- h+ `S3_x0` z2 pT3: Δ = 2.668%, f = 2.738%, 3.67σ. E = 97.45% (no
  credit) → 97.41% (scan credit) → 92.36% (maximal dca1 linear credit) →
  17.69% (3σ-discounted). At E = 50% the point estimate exceeds the
  envelope by 1.30% = 1.79σ (one-sided p = 3.7%).
- h− `S0_x1` z2 pT4: Δ = 2.059%, f = 3.061%, 3.56σ. E = 67.24% → 64.88% →
  10.64% (3σ-discounted). At E = 50%: excess 0.53% = 0.91σ (p = 18%).

**What O2 actually buys** (a vs b/d2): conservative crediting of the
existing DCA scan moves the median requirement by −0.12/−0.00 pp and the
max by −0.03/−0.01 pp, and changes **no** >30/>50/>100 count. Even the
maximal credit (one-sided dca1, fully-optimistic linear subtraction)
leaves the max at 92.4% and reduces the >50% count only from 10 to 8.
The reason is measured, not asserted: over the 87 significant bins the
Path B shift exceeds the DCA scan by a median factor **26.7× (h+) /
38.3× (h−)** (minimum 5.8×/3.8×); **0 of 87** bins are covered by the
scan (or by dca1) alone; the scan's energy is 0.52%/0.70% of the Path B
shift energy. The two variations share only the far-tail band
|DcaPrime| ≳ cosh η·1 cm; the lever's effect lives in the
0.3–cosh η·1 cm near tail.

**Integrated (coherent) requirement.** With ε = B_R′/data_sub′ = 7.722%/
7.978% and Δ(Born) = 2.369%/1.402%: required integrated E = **30.68% (h+) /
17.58% (h−)** without credit; crediting the dca1 integrated shift
(0.432%/0.295%): 30.17%/17.19% (quadrature) or 25.09%/13.89% (linear).
A ±30% envelope therefore fails the h+ integrated requirement even before
any per-bin consideration (30.68% needed; 30.17% after quadrature credit;
it drops below 30% only under the fully-optimistic linear credit).

## Rev2.3 Verdicts on O1 and O2, and Path B's role

- **O1 — UPHELD** as a sizing-logic objection. The admitted-mu-range
  argument (rev1 §3, "factor 2 covers every admitted mu estimate") is
  contaminated as magnitude evidence and is withdrawn; R2 stands as
  evidence that the template normalizations are UNVALIDATED (which is why
  a fiat envelope exists at all), not as evidence of its size. Rev2 sizes
  the envelope from Path B alone — and the re-derived sizing independently
  still requires up to 97.45% for point-estimate coverage.
- **O2 — affirmed on existence, refuted on consequence.** The DCA shift
  exists (`dca_scan_rel`; rev1's "no Dca variation anywhere in the
  19-comp build" is corrected to "no Dca leg in the quoted budget; a
  measured DCA-scan diagnostic exists but is unquoted" — see §Rev2.5).
  But (i) it is not in the quoted 19-comp total, so the envelope cannot
  double-count anything currently quoted; (ii) the physical overlap is
  far-tail-only and 26.7×/38.3× too small at the median to matter;
  (iii) crediting it changes the required-E distribution by ≤0.03 pp
  (conservative) or ≤5.1 pp (maximal) and never brings ±50% into
  point-estimate coverage.
- **Path B's role — not reduced; promoted.** Because the overlap is
  negligible, there is no ground to demote the 87 significant shifts to
  closure evidence. Under O1, Path B is promoted from corroborating
  stability evidence to the **sole magnitude basis** of the envelope.

## Rev2.4 Reconciled recommendation

**Keep the fiat envelope at ±100%** (decision question 1 unchanged), now
justified on corrected grounds: it is the smallest round number that
covers the point estimates of all 87 significant Path B shifts under any
crediting scenario (max required E 97.45% no-credit; 92.36% maximal
credit), with the R2 mu-range argument retired.

The alternatives are now precisely priced:
- **±50% is defensible only under a 3σ-excess coverage philosophy**: the
  significance-discounted floor peaks at 47.52% (h+), so ±50% covers every
  bin's >3σ excess with 2.5 pp margin. Its cost: the point estimates of 10
  significant bins (9 h+ / 1 h−) remain uncovered, and the h+ driving bin
  (`S3_x0` z2 pT3) is covered only at 3.7% one-sided confidence. Totals
  would be 6.51%/6.76% (rev1 §3).
- **±30% is refuted**: it fails the h+ integrated requirement (30.68%
  needed; 30.17% after conservative crediting) and 23 significant bins
  per-bin (20 h+ / 3 h−), still 7 h+ bins under the 3σ-discounted floor.
- O2 can be honored in the bookkeeping at zero numerical cost: record
  `dca_scan_rel` in the scheme note as an existing, unquoted,
  far-tail-only DCA diagnostic that the envelope subsumes; no component
  is added, removed, or resized.

## Rev2.5 Corrections to the rev1 synthesis

- Rev1 double-count-audit sentence "no Dca variation exists anywhere in
  the 19-comp build (grep-verified)" is **correct as stated** (the quoted
  budget, its builder, and its 12 rederivation records contain no Dca
  variation — re-verified by gate V4) but was **over-extended in intent**:
  the analysis inventory does contain DCA-shift measurements (dca1/dca3
  studies → `dca_scan_rel`, plotted 2026-09-25) and a planned-but-never-run
  v5g source. The rev1 conclusion (no double-count within the quoted
  budget) is unchanged; the rev1 wording is superseded by this section.
- Rev1's §3 sizing rationale ("factor 2 covers every admitted mu
  estimate") is withdrawn per O1; the Path-B coverage argument (rev1 §2.4)
  is now the primary sizing basis, sharpened into the required-E
  distributions of §Rev2.2.

## Rev2.6 Rev2 artifacts, validation, and paths

Study dir (unchanged): `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_final_scheme_2026-10-18/`

| file | content |
|---|---|
| `analysis_bgsub_final_scheme_rev2_dcaaudit_2026-10-18.py` | rev2 keepable script (read-only on all inputs) |
| `dca_overlap_audit_rev2_2026-10-18.csv` | 490 rows: per-bin Path B term, f_i, sigma_rel, flags, all six DCA/track legs, and E under all seven scenarios |
| `rev2_required_E_summary_2026-10-18.json` | audit legs, required-E distributions, driving bins, overlap quantification, integrated requirements, gates, input sha256 |

Rev2 in-script gates (all asserted PASS): V1 490/490 joins with label
agreement across env × source_table × dca1 × dca3 × budget19; V2
significant count 87 (46/41) matching rev1; V3 scenario (a) reproduces
the rev1 coverage stats bitwise (medians 0.26669765042301774 /
0.16368241115772178, maxima 0.9744558605292302 / 0.6724433581379562);
V4 all 19 canonical budget components present and zero "dca"-named
columns; V5 `dca_scan_rel` reproduces 0.5·\|1/r₁−1/r₃\| from the raw
dca1/dca3 ratio tables to ≤1e-12; V6 crediting monotonicity
(E_a ≥ E_b ≥ E_c, E_a ≥ E_d, E_d2, E_e per significant bin).

Additional read-only inputs (canonical and campaign trees untouched):
- `sidis_highQ2/plots/systematics/uncertainty_7col_all_sources_binwise_bg_2026-09-24/source_table.csv` (+ manifest/validation)
- `H1ChargedSidis_HighQ2/output_v6/studies/dca1_zlow005_finalcandidate_S3xmerged_pT5_eta1p6_radlen5_nhit5_2026-06-15/ratio_plots/nominal_vs_dca1_ratio_by_global_bin.csv`
- `H1ChargedSidis_HighQ2/output_v6/studies/dca3_zlow005_finalcandidate_S3xmerged_pT5_eta1p6_radlen5_nhit5_2026-06-15/ratio_plots/nominal_vs_dca3_ratio_by_global_bin.csv`
- `.../stage3_tightdca_2026-10-17/stageD_unfold/tightened_chain_anchors_2026-10-17.json` (integrated ε and Born operands)
- `sidis_highQ2/docs/plans/plan-systematics-v5g-v3-2026-05-24.md` (source 15, never run)

---

# Rev3a (2026-10-18): bin-by-bin background uncertainty from the Path B lever shifts

This section executes the user's 2026-10-18 instruction to replace the flat
±100% envelope by a per-bin, statistics-guarded measurement of background
knowledge: "evaluate precisely what background uncertainty we should apply in
each bin using the shifts of the cuts… define a bin-by-bin uncertainty on how
well we know the background instead of assuming we know it equally well (to
100%) everywhere… make sure the bins you're drawing conclusions from are not
statistically limited for the background — that can easily be the source of the
large deviations up to 100%. If the bin has almost no background, 100% is
fine, but we shouldn't apply 100% to bins we know well that have a significant
amount of background." Rev1/rev2 above are kept intact; where rev3a supersedes
their sizing *interpretation*, this is stated explicitly. Everything below was
recomputed from artifacts by the keepable rev3a script
(`.../bgsub_final_scheme_2026-10-18/analysis_bgsub_final_scheme_rev3_binbybin_2026-10-18.py`,
gates W1–W9 asserted in-script, §Rev3a.8), read-only on all inputs; canonical
release and campaign trees untouched; `L_data = 351000.0 nb^-1` only (it enters
no ratio here); neither retired wrong-scale constant appears in the live code
(gate W9). All lever/vector quantities are RAW-level (pre-C_rad); per-bin
relative terms are level-agnostic; coherent nb effects are stated on the Born
anchors h+ `11.996558` / h− `11.663956` nb (raw IBU `10.508811`/`10.217503`).

## Rev3a.1 Per-bin lever model

For every release bin (490 rows = 245 bins × 2 charges):

- `B_nom,i = e_avg,i + v0_avg,i` (nominal Set-R′ electron+V0 content; lowQ2
  excluded, envelope convention — its ±30% leg lives in `bgsub_model`);
- tightened template content `B_tight,i` from the stage-D tightened vector
  (`data_vector_bgsub_setrprime_cone_tightdca_490.csv`, joined 490/490 with
  label checks, W1; tightened sums reproduce the stage-C/D anchors exactly,
  W3: D_tight 1,805,705/1,758,896; data_sub′_tight 1,705,166.90/1,658,812.66;
  e_tight 105,761.475130; v0_tight 94,628.63→**94,659.628473**; B_R′_tight
  100,538.4157/100,083.4444);
- moved background `rB_i = B_nom,i − B_tight,i`, moved fraction
  `r_i = rB_i/B_nom,i` (computed per bin, **never assumed 30%**: per-bin r
  ranges 0.04–0.40; integrated r = 0.2867/0.2870);
- `f_i = B_nom,i/data_sub′_i` (0 on the three empty-D bins, house convention);
- Path B unfolded relative shift `t_i = term_i` (stage-E CSV, signed) with
  `sigma_t,i = sigma_delta,i/nom_i`;
- **primary estimate** `eps_i = t_i/(r_i·f_i)`, `sigma_eps,i = sigma_t,i/(r_i·f_i)`
  — the fractional displaced-content mis-modeling that would be needed to
  produce the observed result-level shift through the lever.

**Dilution treatment (documented, no correction applied).** The formula is
calibrated at the *result* level: it uses the actually observed unfolded shift
of the tightened chain, which was re-unfolded with its own tightened response,
so the leading response-dilution difference between the chains partially
cancels inside `t_i`. The reco-level content ratio
`eps_reco,i = X_i/rB_i − 1` (X_i = D_cone,i − D_tight,i) is carried as a
cross-check column: the empirical dilution `eps_i/eps_reco,i` over the 58
rB ≥ 50 bins has median 0.31 (p10 0.15, p90 0.49) — i.e. the result-level
lever response to displaced content is ~3× softer than a transparent reco
transfer, and using the result-level formula *without* an extra dilution
correction is the conservative choice for a result-level budget term
(`eps·f = t/r` either way; the reco-level eps would be larger as a content
statement but transfers diluted).

**Attribution caveat (conservative).** Integrated, the lever removes
X = 300,472 data tracks but only rB = 80,628 of template background content
(X/rB = 3.96 h+ / 3.49 h−): most of what the lever removes is *displaced
signal* (Dca tail of good hadrons), not background. `eps_i` therefore
over-attributes the mismatch to background. For a background *uncertainty*
this is conservative; it is also the reason rev3a's eps does **not** come out
small (§Rev3a.5a).

## Rev3a.2 Background-statistics guard and classification

Pre-registered quotability: **quotable** ⟺ not stability-excluded AND
`rB_i ≥ 50` counts AND `|Δ_i|/σ_Δ,i > 2`. Classes (priority order):
`empty` (f=0, 3 bins) / `quotable` / `tinybg` (f<1%, non-quotable → 100% cap,
user-sanctioned; verified cost 0.000 pp on both total medians) / `bgstat`
(rB<50, non-quotable) / `insig` (rB≥50 but |Δ|/σ ≤ 2). 19 bins have
numerically zero moved background (15 exact zeros + 2 at −1 ULP float noise);
all fall in `bgstat`.

| class | n | assigned eps | raw point eps (documentation) |
|---|---|---|---|
| quotable | **55** (26 h+/29 h−) | min(1, \|eps\|): median 1.000, 27 bins sub-cap, 28 capped | median **1.005**, p90 2.714, max 4.416; significance median 8.4σ, min 2.1σ |
| bgstat | 417 | UL: min(1, max(2σ_eps, group)): median 1.000 (20 bins group-bound) | median **3.151**, p90 10.1, max 5.6e15 — noise-inflated, NOT used (the user's caution, demonstrated) |
| insig | 3 | UL (3 bins group-bound) | median 0.144 — insignificant, not quotable |
| tinybg | 12 | 1.00 (100% cap) | raw explodes (rB ~ 0); cost 0.000 pp |
| empty | 3 | term 0 | — |

The 55 quotable bins carry **84.3% (h+) / 87.8% (h−) of the total background
content** and 69.4%/75.1% of the data_sub′ statistics: the measurement covers
the bins that matter even though it is only 55/490 by count. Among the 87
statistically significant Path B shifts: **50 quotable, 37 background-stat-
limited, 0 insignificant** — the significant shifts split cleanly into
measurable (big-background) and unmeasurable-as-background (starved) bins.

**Group model** (pre-registered primary): per-(charge × Q2-stratum) median of
quotable |eps| (n≥3, else charge-global): h+ S0 1.654, S1 1.712, S2 0.967,
S3 1.586(global); h− S0 0.889, S1 1.019, S2 0.716, S3 0.891(global). All but
h− S2/S3 exceed the 100% cap when assigned. The alternative smooth-in-f model
(ln|eps| ~ a + b·ln f; slopes −1.03/−0.93) reproduces the same totals to
<0.001 pp — the group assignment is not the sensitive part.

## Rev3a.3 The user's hypothesis test — the rev2 driving bins

**CONFIRMED: both driving bins are background-stat-limited; the 97%/67%
required-E values do not survive as real background requirements.**

| | h+ S3_x0 z2 pT3 (bin 223) | h− S0_x1 z2 pT4 (bin 49) |
|---|---|---|
| rev2 required-E (naive r=1 transfer) | 97.45% | 67.24% |
| observed shift \|t\| (significance) | 2.668% (3.67σ) | 2.059% (3.56σ) |
| moved background rB | **3.19 counts** (15.7× below the 50-count threshold) | **2.62 counts** (19.1× below) |
| moved fraction r / content f | 0.0821 / 2.738% | 0.0726 / 3.061% |
| lever-model eps = \|t\|/(r·f) | **1187% ± 324%** | **926% ± 260%** |
| moved-bg Poisson σ alone (in eps units) | ±56% (σ_eps_reco 225%) | ±62% (σ_eps_reco 210%) |
| max background share of the shift (eps≤1 through the lever) | **≤ 8.4%** | **≤ 10.8%** |

The shifts are real (3.7σ/3.6σ) but cannot be background: attributing them to
the displaced background component requires eps ≈ 9–12, unphysical (>100% cap)
even at −2σ, in bins where the lever moves ~3 background counts. Under the
measured per-bin lever fraction, background mis-modeling can produce at most
r·f = 0.22%/0.22% of result shift — ≤ 8–11% of what is observed. **Verdict:
the ~100% required-E values of rev2 were an artifact of dividing a
signal-tail-driven shift by a small background fraction in bins where the
background is statistically negligible. They collapse: the bins take the
unconstrained 100% cap (eps=1.0; final term 2.859%/3.196% with the 30% floor),
which still covers the observed shifts (ratios 1.071/1.553).** The observed
shifts themselves are displaced-*signal*-tail mis-modeling — an effect the
background uncertainty has no obligation to cover beyond f, and which no
other budget component covers (rev2: no DCA leg in the quoted budget; the
dca_scan diagnostic is far-tail-only and 26–38× smaller).

## Rev3a.4 Residual fiat floor (Dca-invisible direction) and final terms

The lever constrains only the displaced (Dca-orthogonal) direction. A
Dca-invisible component (small-Dca electron-induced content; the unvalidated
normalization per the Path A/R2 verdict) is NOT constrained. Rev3a therefore
carries `eps_total,i = hypot(eps_lever,i, eps_floor)` with the floor scanned
over {10, 20, 30, 50}%. **Recommended floor: 30%** — it matches the only
template model-disagreement precedent (the lowQ2 ±30% leg in `bgsub_model`),
never dominates a quotable measurement (p10 of quotable |eps| = 0.51 > 0.30),
and carries the unvalidated-normalization doubt explicitly.

New totals (medians h+/h−; same bookkeeping as rev1's recommended scheme:
`bgsub_stat` retired into the content term, 19-comp structure preserved;
component `bgsub_epsbin_rel,i = eps_total,i·f_i` replaces `bgsub_env100_rel,i
= 1.00·f_i`; bit-reproduction gates W4/W5: env term max dev 0.0, t19env
medians 7.32028833/7.63706360):

| total | median h+ | median h− | bins>100% |
|---|---|---|---|
| `total_with_bgsub19_rel` (current, carried) | 5.9764% | 6.2968% | 0/0 |
| `total_with_bgsub19env_rel` (rev1 flat ±100%) | 7.3203% | 7.6371% | 0/0 |
| rev3a eps-term, floor 0 (pure lever measurement) | 7.3128% | 7.4933% | 0/0 |
| rev3a eps-term, floor 10% | 7.3265% | 7.5194% | 0/0 |
| rev3a eps-term, floor 20% | 7.3674% | 7.5474% | 0/0 |
| **rev3a eps-term, floor 30% (RECOMMENDED)** | **7.4350%** | **7.6858%** | 0/0 |
| rev3a eps-term, floor 50% | 7.5218% | 7.8560% | 0/0 |
| rev3a 20-comp add variant (floor 30%) | 7.4364% | 7.6866% | 0/0 |

Maxima (floor 30%): 73.65%/88.67% — no bin exceeds 100% in any variant. Note
452 bins carry `eps_total` > 1.0 (max 1.044): this is the quadrature floor on
top of the physical 100% lever-cap (a 2D doubt), not a claim of >100% content
mis-modeling; the lever-direction eps itself never exceeds the cap.

**Coherent integrated effect** (floor 30%): content-weighted
eps_int = 6.903%/5.429% → **±0.828 nb (h+) / ±0.633 nb (h−) on Born
11.996558/11.663956 nb**, vs the envelope's ±0.926/±0.930 nb — a 10.5%/31.9%
integrated reduction that lives in the big (quotable) bins; the medians move
little because 447/490 bins sit at the cap. The observed Path B integrated
shift (−2.382%/−1.409% raw; −2.369%/−1.402% Born) is 34.5%/26.0% (raw/raw) of
the recommended integrated term; the lever-calibrated integrated eps requirement
(|Δ|/(r_int·f_int)) is 1.077/0.616 — unphysical for h+ (signal-tail
dominated), physical but sub-cap for h−, consistent with the Path A reading.

## Rev3a.5 Sanity checks

**(a) eps vs f and vs B (user's expectation).** Among quotable bins,
Spearman(|eps|, f) = −0.58 (h+)/−0.54 (h−) and Spearman(|eps|, B) =
−0.74/−0.68: the expected *structure* is present — better-measured
(moved-background-rich) bins yield smaller eps. But the *level* is high: raw
|eps| median 1.005, B-weighted 0.677; the five best-known bins are all h−
(eps 0.335–0.457 at 2.1–23σ, including the largest h− bin, B = 28,203,
eps = 0.352 ± 0.018); the largest h+ bins measure eps 0.65–0.85; h+'s best is
~0.51. **The data do not support "well-known high-B bins have eps ≪ 100%"**:
through this lever, the displaced content is measured to be mis-modeled at
O(50–100%) even in the best bins, because the lever shift is dominated by
displaced-signal-tail mismatch (X/rB ≈ 3.5–4.0 integrated). The bin-by-bin
scheme's reduction is real but modest: lever-term medians 3.158%/3.520%
(floor 0) vs envelope 3.158%/3.563%; the per-bin term falls below the
envelope in 43 bins (27 measured sub-cap + 16 group/UL sub-cap), by up to
65% in the biggest bins (h− bin 0: eps 0.352 → term 4.16% vs env 11.84%).

**(b)** New totals vs status quo and flat-100%: see §Rev3a.4 table —
**+1.459/+1.389 pp vs current; +0.115/+0.049 pp vs the rev1 envelope** at the
recommended floor 30% (−0.008/−0.144 pp at floor 0). The bin-by-bin scheme
does **not** buy a materially smaller budget; its value is methodological.

**(c)** Coherent integrated: §Rev3a.4 (Born-level, both anchors stated).

**(d)** Bins >100%: raw |eps| > 1 in 28/55 quotable bins (max 4.42, list in
the summary JSON and CSV column `eps_lever_abs`); all capped at 1.0 — in
those bins the shift *exceeds* what any physical background mis-modeling can
produce through the lever, i.e. it is signal-tail-dominated; totals >100%:
**0/0 in every variant**.

**(e) Hot bin** (h+ local bin 34, S0_x0 z6 pT4; f_env = 28.387% = rev1's max
envelope bin; CC-5 `f_res_prime` 31.858% cross-checked, W7): B_nom = 14.51
counts, rB = 0.58, shift 2.857% at 0.12σ (insignificant, guard-flagged) →
class `bgstat` → **eps = 100% cap, eps_total(f30) = 1.044, term = 29.64%**
(envelope: 28.39%). We know nothing about its background (D_cone = 75): 100%
is correct there, exactly as the user allowed.

**(f) Coverage of the 87 guard-passing Path B shifts:** 87/87 covered by the
final floor-30% terms; min ratio term/|t| = 1.071 (the h+ driving bin);
median ratio 4.2 (mean 5.6); **0 residuals** (per-bin ratios in
`rev3_coverage_residuals_2026-10-18.csv`). Quotable bins cover by
construction with margin 1/r ≥ 2.5; unquotable significant bins cover because
2σ_eps > |t|/f whenever r < 2/(3·|Δ|/σ) and the physical cap 1.0 ≥ max
|t|/f = 0.9745. Against the *strict* lever-model requirement (|t|/r) the 28
capped quotable bins and the 37 starved bins are uncoverable *as background*
— that excess is the signal-tail effect (§Rev3a.3), recorded, not hidden.

## Rev3a.6 Correlation structure proposal

- **Measured + group eps**: the same displaced-track mis-modeling drives
  neighboring bins — propose **fully correlated within each (charge × Q2
  stratum) block** (8 blocks per charge), uncorrelated across strata, for any
  coherent/integrated use; per-bin values carried in the CSV with the
  correlation declared in budget metadata (lumi precedent; the CSV machinery
  carries no covariance object).
- **Floor (30%)**: fully correlated across all bins within a charge and
  between charges (common template doubt), like `lumi_rel`.
- The two parts are independent directions → quadrature in the per-bin term.

## Rev3a.7 Honest caveats and verdict

1. The lever probes only the Dca-orthogonal (displaced) direction; the
   Dca-invisible component is carried only by the 30% floor (unvalidated
   normalization per R2; Path A could not measure it).
2. eps over-attributes displaced-*signal*-tail mismatch to background
   (conservative); a signal-tail selection systematic exists in the data
   (87 resolved shifts, integrated −2.4%/−1.4%) that this component absorbs
   conservatively but that is conceptually not background.
3. The group model predicts from 55 quotable bins (84–88% of content) into
   435 bins; predictions are cap-dominated (≥0.72 everywhere), so the
   prediction uncertainty is bounded by the cap.
4. The tightened chain is a single lever; other directions of background
   mis-modeling (pT/eta shape, charge asymmetry of the templates) are only
   covered by `bgsub_model` (generator half-spreads) and the floor.

**VERDICT: BIN-BY-BIN SCHEME VIABLE-WITH-CAVEATS.** It is constructible,
fully gated, and it does what the user asked: it measures where the
background is measurable, caps at 100% where it is not, and it **proves the
97%/67% driving-bin requirements were background-stat-limited artifacts**
(rB ≈ 3 counts). But the measurement itself finds displaced-content
mis-modeling of O(100%) (median 1.005 raw; B-weighted 0.677; best bin 0.335)
— the lever does not license terms much below the flat envelope where the
background is significant. The recommended rev3a scheme (eps-term + 30% floor)
totals **7.4350%/7.6858%** — essentially at the rev1 envelope at the median
(+0.115/+0.049 pp) but smaller in the coherent
integrated effect (−10.5%/−31.9%) and, importantly, now *measured or capped
per bin with an explicit floor*, rather than flat fiat. If a single flat
number must be kept for simplicity, the rev1 ±100% envelope remains a
defensible summary of the same evidence; the bin-by-bin table is the honest
per-bin statement.

## Rev3a.8 Artifacts, validation, and paths

Study dir (unchanged): `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_final_scheme_2026-10-18/`

| file | content |
|---|---|
| `analysis_bgsub_final_scheme_rev3_binbybin_2026-10-18.py` | rev3a keepable script (read-only on all inputs; asserts W1–W9) |
| `bgsub_binbybin_eps_term_490_rev3_2026-10-18.csv` | 490 rows × 46 cols: per-bin B_nom/B_tight/rB/r/f/X, signed eps ± σ, reco cross-check + dilution, class + eps_source, group value, capped eps, eps_total and term for floors {0,10,20,30,50}%, totals for all variants, Path B flags |
| `rev3_binbybin_summary_2026-10-18.json` | all headline numbers, classification, group tables, driving-bin verdicts, hot bin, coverage, coherent integrated, floor scan, correlation proposal, input sha256 |
| `systematics_bgsub_final_scheme_rev3_binbybin_2026-10-18.json` | output-contract file (step/channel/status/bins/values/errors/validation) |
| `rev3_coverage_residuals_2026-10-18.csv` | the 87 significant shifts: final-term coverage ratios, class, lever-model requirement |

Rev3a in-script gates (all asserted PASS): W1 490/490 joins with label
agreement (budget=vectors=tightened=stability); W2 rev1 anchors (t19 medians
5.976416521492156/6.296839261150574%, bgsub4 quadrature 0.4445/0.4604%,
median(f)×0.25 = 0.7896/0.8909%, empty-D bins, Path B headline medians/max/
87-flag/p90/p99); W3 tightened sums == stage-C/D anchors; W4 recomputed
envelope term bitwise == rev1 (max dev 0.0); W5 recomputed t19env medians ==
7.32028833025428/7.637063595222382%; W6 rev2 driving bins reproduced
(term/f/dσ/E_a bitwise vs `rev2_required_E_summary_2026-10-18.json`); W7 hot
bin f == 0.28387185898143148 and CC-5 f_res_prime == 0.31857691434196245;
W8 class counts (55/12/417/3/3; 87 = 50 quotable + 37 bgstat + 0 insig; 19
zero-rB; 12 tiny-f); W9 no forbidden retired-scale constants in live code.

Read-only inputs additional to rev1/rev2:
- `.../stage3_tightdca_2026-10-17/stageD_unfold/data_vector_bgsub_setrprime_cone_tightdca_490.csv`
- `.../bgsub_final_scheme_2026-10-18/bgsub_env100_term_490_2026-10-18.csv` (rev1, gate only)
- `.../bgsub_final_scheme_2026-10-18/rev2_required_E_summary_2026-10-18.json` (rev2, gate only)

---

# Rev3b (2026-10-18): disposition of the diagnostic-only shift legs

*Provenance note (2026-10-18 relabel): Rev3b is this study dir's diagnostic-legs
disposition thread (artifacts 08:28–08:38 UTC+2, `*_rev3_diagnosticlegs_*`,
gates RV1–RV6 PASS; leg columns reconcile bitwise with
`dca_overlap_audit_rev2_2026-10-18.csv` on a 490/490 join). Section relabeled
Rev3b to disambiguate from Rev3a (bin-by-bin lever scheme); artifact filenames
retain their original `rev3` labels.*

Policy question (user): should the measured-but-unquoted diagnostic legs be
promoted into the quoted budget? Rev3b applies the following standard to
**every** measured variation in the package.

## Rev3b.0 Standard and pre-stated thresholds

A measured variation is admissible in exactly one of three states:

(i) **QUOTED** as a budget component; (ii) **COVERED** by an existing quoted
component (per-bin overlap argument); (iii) **RETIRED** with a documented
reason. Unquoted-but-measured diagnostics are **not an admissible final
state** for a publication budget.

Retirement thresholds (fixed before the audit):
- *Negligible*: median < 0.10% (both charges) AND per-bin value ≤ the bin's
  quoted `ibu_stat_rel` in every valid bin (nowhere statistically resolvable).
- *Superseded*: a quoted component/scheme now covers the same physics.
- *Definition*: the variation moves the observable's fiducial definition, not
  a detector-modeling doubt.

Scenario pricing: **S0** = rev1 final scheme as proposed (env100 + retire
`bgsub_stat`, keep model/stage/fakefrac) = `total_with_bgsub19prime_rel`;
**S1** = S0 + quadrature of DISJOINT legs; **S2** = S1 + PARTIAL legs merged
per-bin as max(leg, nearest) without double-adding. Basis: all promoted
values are as-measured one-sided full-chain ratios from the 2026-06-14/15 +
2026-07-05 finalcandidate-era studies (L-cancelling ratios; current-basis
quantities at L_data = 351000.0 nb^-1 only; MC scales in force per
`verification_record.json`: RG31 0.03843251824641288, DJ14 0.03346843073584302
— recorded, not consumed numerically).

## Rev3b.1 Complete inventory of measured-but-unquoted legs

| leg | definition | med h+/h− % | max h+/h− % | provenance |
|---|---|---|---|---|
| `labpt010` | lab-pT 0.15→0.10 GeV loosening, full-chain, one-sided | 0.0414/0.0451 | 11.15/3.75 | `studies/labpt010_..._2026-06-14/ratio_plots/labpt010_vs_nominal_ratio_table.csv` |
| `dca_scan` | DcaCosh 2→1/→3 two-point half-spread, full-chain | 0.0157/0.0210 | 0.70/2.10 | dca1+dca3 tables via 2026-09-24 `source_table.csv` |
| `dca1_tighten` | DcaCosh 2→1 tightening alone, one-sided | 0.0314/0.0417 | 1.40/4.20 | `studies/dca1_..._2026-06-15/ratio_plots/...` |
| `dca3_loosen` | DcaCosh 2→3 loosening alone, one-sided | 0.0000/0.0000 | 0.0006/0.0036 | `studies/dca3_..._2026-06-15/ratio_plots/...` |
| `compound_track_quality` | nhit 5→10 AND radlen 5→10 tightening, one-sided | 0.8446/0.8732 | 12.77/17.46 | `studies/nhit10_trklen10_..._2026-06-15` via `source_table.csv` |
| `empz25` | E−pz window 45→25 GeV, one-sided | 0.3772/0.3336 | 20.54/47.61 | `studies/empz25_..._2026-07-05` via `source_table.csv` |
| `eta1p4` | \|η\| 1.6→1.4 fiducial tightening, one-sided | 0.6388/0.7708 | 61.53/77.97 | `studies/eta1p4_..._2026-06-14/ratio_plots/...` |
| `background_proxy_50pct` | lowQ2-only 50% B/Y linearized proxy (NOT full-chain) | 0.0000/0.0000 | 30.33/552.14 | `data/systematics/background_binwise_50pct_lowq2_2026-09-24` via `source_table.csv` |

Non-candidates, documented (no measurement to promote): v5g source-15
track_quality_dca (planned, never run); `hfs_phi_rel` (register B14, P2
campaign 5 not launched); register rows C15–C18 blocked (tracking_chargedep,
response_mcstat, trigger placeholder, NucliaWeight — the nuclint JSON is
status FAIL with the value deliberately blank, not zeroed); C20 diffvm (rho
0.0074% negligible, rest unassessed); C21 gammap (no template subtracted);
D22–D25 dropped (electron_phi, noise_energy, tracking_resolution,
combined_forward_tracking); D26 method spread (diagnostic by principle);
reco-method electron/IΣ (parked, C_rad undefined); `elecE_linearity_2026-10-17`
(leg-integrity on quoted elec_energy: linearity FAILED, D2 keeps ±1%, deviation
recorded); charge_misid (A_h-scope term, dispositioned 2026-10-11); muon
component (template constituent inside the Set-R' bgsub family).

## Rev3b.2 Per-leg overlap audit vs the quoted budget

Nearest quoted component and per-bin crossing statistics (490 bins; env100
comparisons exclude the 3 empty-D bins where the envelope is zeroed):

| leg | nearest | leg>nearest (bins pos/neg) | leg>ibu_stat (bins) | ruling |
|---|---|---|---|---|
| `labpt010` | tracking | 29/30 (11.8%/12.2%) | 11/10 | DISJOINT — threshold turn-on ≠ flat efficiency |
| `dca_scan` | env100 | 4/2 (0.8%/0.4%), all sub-2σ | 3/1 | COVERED beyond noise |
| `dca1_tighten` | env100 | 4/6, **0 beyond 2σ** (ratio's own unc) | 5/4 | COVERED beyond noise |
| `dca3_loosen` | env100 | 4/2 (sub-2σ) | 0/0 | negligible |
| `compound` | tracking | 145/149 (59.2%/60.8%) | 35/30 | DISJOINT — see ruling below |
| `empz25` | env100 | 25/19 (10.2%/7.8%) | 27/20 | PARTIAL — same family, different failure mode |
| `background_proxy` | env100 | 28/25 | 32/33 | superseded (and numerically unstable: max 552% where Y→0) |

**Compound vs tracking ruling (the critical case).** The two legs are not the
same detector effect. `tracking_eff_m0p5` drops each track independently with
0.5% probability — a flat efficiency-scale doubt whose per-bin response is
~0.5% with weak structure. The compound tightening removes *marginal-quality*
tracks (few hits, high radiation length) — a population structured in η/pT
concentrated near acceptance and momentum edges. The measured responses
confirm the separation: the compound exceeds tracking in ~60% of bins, is
statistically resolvable in 35/30 bins (e.g. pos S3_x0 z3 pT4: 9.56% vs
tracking 1.00% vs stat 9.08%; neg S3_x0 z3 pT4: 12.28% vs 0.52% vs 9.19%),
and its maxima (12.8–17.5%) are 10–30× the tracking leg. A flat-efficiency
leg cannot cover a structured selection response. Classification: **DISJOINT**
— the quoted budget currently carries only the flat axis and understates
track-quality modeling wherever the compound dominates.

**DCA family.** `dca_scan` ≈ 0.5·`dca1` by construction (the dca3 leg is
null), so its crossings are a subset of dca1's; dca1's crossings over env100
are **zero beyond 2σ** of the ratio's own uncertainty. The COVERED ruling of
rev2 stands and is now noise-guarded: the envelope's mandate (DCA-tail
background doubt, sized by Path B which removes strictly more) covers the
family. This coverage is **conditional on envelope adoption** (§5 decision 1);
if the envelope is rejected, the dca family must be re-dispositioned.

## Rev3b.3 The lab-pt finding

The lab-pt leg's median (0.04%) is misleading: the effect is concentrated in
the highest-yield bins. In **all seven** z0/pT0 strata bins — which together
with the rest of the top-10 carry ~62% of the cross section — the signed
variant/nominal ratio is 1.027–1.034 for both charges: a **coherent,
one-directional +2.7–3.4%** response to loosening the threshold, at 4–14× the
bin's statistical uncertainty (stat 0.23–0.75% there). The cs-weighted
coherent effect is **1.59%/1.69%** — lumi-sized (lumi wmean 2.70%), and the
4th-largest coherent component after env100 (8.56/8.83%) and lumi. Nothing
quoted covers it (DISJOINT from tracking; not a generator-model or
subtraction effect). The 2026-09-22 provisional cut-shift study carried these
bins per-bin in its max-merge envelope but never computed an integrated
statement — its median-based presentation was structurally blind to exactly
this effect. Rev3b promotes `labpt010` to QUOTE.

## Rev3b.4 Scenario pricing

| scenario | median h+/h− % | max % | bins>100% | integrated coherent % (nb on Born) |
|---|---|---|---|---|
| S0 (rev1 scheme) | 7.3203/7.6371 | 73.65/88.44 | 0/0 | 9.1434/9.3975 (1.0969/1.0961) |
| S1 = S0 + compound + labpt | 7.7847/8.1291 | 74.78/88.56 | 0/0 | 9.3103/9.5674 (1.1169/1.1159) |
| S2 = S1 + empz max-merge | 7.7989/8.1606 | 77.55/98.11 | 0/0 | 9.3398/9.5983 (1.1205/1.1195) |

Single-leg marginals (vs S0): `compound` +0.4260/+0.4453 pp median but only
+0.0301/+0.0192 pp integrated (structured, correlated with the total,
r(S0,compound)≈0.35); `labpt` +0.0000/+0.0005 pp median but **+0.1373/+0.1510
pp integrated** (coherent in top-yield bins, invisible to medians); `empz25`
merge +0.0796/+0.0510 pp median, +0.0301/+0.0315 pp integrated, active in
25/19 (sparse, low-envelope) bins. The S2 neg maximum 98.11% is the empz
merge (47.61% over env100 2.50%) in bin S2_x1 z0 pT4 — cs 0.000425 nb, stat
54.7% — a sparse-bin artifact, not a physics statement.

Sensitivities: (a) track-family max-merge variant (per-bin max of
tracking/labpt/compound instead of quadrature — the 2026-09-22 construction):
medians 7.7485/8.0693% (−0.04/−0.06 pp vs S1), integrated 9.3302/9.5945%
(+0.02/+0.03 pp vs S1) — a wash; quadrature stands as primary since the three
axes (momentum threshold, hit/material quality, flat efficiency) remove
distinct populations. (b) empz25 as full quadrature add instead of merge:
S2_empz_add computed and stored; merge is the defensible-minimal choice.
Basis caveat: promoted values are as-measured on the unsubtracted
finalcandidate basis; for the compound the current subtracted denominator
would make the relative effect ~28% larger (as-quoted understates); for
labpt the current subtraction may absorb part of the soft influx (as-quoted
may overstate) — direction genuinely ambiguous; a current-basis re-derivation
(Path-B-style) is the refinement if either leg is adopted.

## Rev3b.5 Per-leg recommendation

| leg | recommendation |
|---|---|
| `compound_track_quality` | **QUOTE** — DISJOINT, non-negligible, resolvable in 35/30 bins; cherry-picking exposure if left unquoted while displayed |
| `labpt010` | **QUOTE** — DISJOINT, coherent 1.59/1.69% integrated in the dominant-yield bins; nothing quoted covers it |
| `empz25` | **MERGE** into the envelope per-bin (max with env100) — PARTIAL; document as part of the envelope's mandate |
| `dca_scan`, `dca1_tighten` | **COVERED-keep-diagnostic** (conditional on envelope adoption; noise-guarded) |
| `dca3_loosen` | **RETIRE-NEGLIGIBLE** (max 0.0006/0.0036%, unresolvable everywhere) |
| `eta1p4` | **RETIRE-DEFINITION** (fiducial boundary; the 62–78% maxima are the definition dependence itself; deliberately excluded from the diagnostic plots) |
| `background_proxy_50pct` | **RETIRE-SUPERSEDED** (Set-R' scheme + envelope; also Y→0 unstable) |

Register integration: `compound_track_quality` and `labpt010` are **not**
among the frozen 24-row register — adopting them is a spec amendment (two new
rows) bringing the quoted budget to 21 components (18 kept + merged envelope
+ compound + labpt). Correlation structure for the covariance package: treat
both as coherent (fully correlated across bins, per-bin amplitudes), same
convention as the envelope and the lumi precedent.

**Inflation cost of honesty.** Full adoption (S2) costs +0.48/+0.52 pp at the
median (+6.5/+6.8% relative), +0.196/+0.201 pp integrated (+2.1% relative),
+0.024 nb on the integrated Born uncertainty, and pushes one sparse bin to
98.1% (none over 100%). The alternative — leaving compound and labpt
unquoted — violates the stated standard: both are measured, both fail the
negligence thresholds, and neither is covered by any quoted component. The
defensible-minimal instinct cannot retire them; the only compliant
alternative to quoting is a current-basis re-derivation campaign that
demonstrates coverage, which does not exist today.

## Rev3b.6 Rev3b artifacts, validation, and paths

Study dir (unchanged): `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_final_scheme_2026-10-18/`

| file | content |
|---|---|
| `analysis_bgsub_final_scheme_rev3_diagnosticlegs_2026-10-18.py` | rev3b keepable script (read-only on all inputs) |
| `diagnostic_legs_inventory_rev3_2026-10-18.csv` | 490 rows: all 8 legs, nearest columns, S0/S1/S2 (+empz-add variant), merged envelope |
| `rev3_diagnostic_legs_summary_2026-10-18.json` | inventory, overlap stats, mechanical rules, verdicts, scenarios, marginals, hot bins, wmeans, gates, input sha256 |

Rev3b gates (all asserted PASS): RV1 490/490 joins with label agreement across
rev2audit × source_table × budget19 × rev1-terms × eta1p4 × dca1; RV2 S0
recomputed from components equals the rev1 delivered column to ≤1e-12 and
medians equal t19p_reco (7.32028833025428/7.637063595222382%) to ≤1e-9; RV3
rev2 audit-leg medians reproduced to ≤1e-9; RV4 monotonicity S0 ≤ S1 ≤ S2 per
bin; RV5 the 2026-09-22 provisional precedent reproduced — `cut_shift_rel` =
max(\|labpt\|,\|dca1\|,\|dca3\|) matches to 7.4e-11 (bitwise; the provisional
used the same study tables); RV6 S0 bins>100% = 0/0 (rev1 statement), S1/S2
counts reported (0/0, 0/0).

Additional read-only inputs (canonical and campaign trees untouched):
`studies/eta1p4_..._2026-06-14/ratio_plots/nominal_vs_eta1p4_ratio_by_global_bin.csv`,
`studies/dca1_.../ratio_plots/...` (ratio_total_unc for the 2σ noise guard),
`studies/provisional_cutshift_2026-09-22/provisional_cutshift_diagnostic.csv`,
`sidis_highQ2/data/backgrounds/normalization_verification_recokin_mc_2026-10-08/verification_record.json`
(MC scales in force), and the rev1/rev2 summaries in this study dir.

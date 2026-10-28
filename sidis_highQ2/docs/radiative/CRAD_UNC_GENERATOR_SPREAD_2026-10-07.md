# C_rad generator-spread uncertainty — DJANGOH14 nominal (2026-10-07)

**Decision being implemented (user, 2026-10-06):** nominal `C_rad` = DJANGOH14; the `C_rad`
UNCERTAINTY = the per-bin difference between RAPGAP31 and DJANGOH14. This note sizes that term.

**Read-only sizing note.** No plot, release, response, or systematic-budget file was modified.
The central v6 release remains raw/no-`C_rad` (`c_rad_applied=false`, not Born-level); both
xFcut tables carry `PROVISIONAL DIAGNOSTIC` status until reviewed.

**Inputs** (490 rows = 245 h+ + 245 h-; bin keys verified identical across all three files;
490/490 `finite_C_rad=True`; `empz_mode=empz_gt45` everywhere; 7 (Q2,x) cells x 7 z-bins
(0-0.05, 0.05-0.1, 0.1-0.2, 0.2-0.3, 0.3-0.5, 0.5-0.7, 0.7-1.0) x 5 pT bins (0-0.5, 0.5-1,
1-2, 2-4, 4-8 GeV) per charge):

| Label | File (under `sidis_highQ2/`) | Generator (content-verified) | xFcut (xFGen<0, both legs) | Integrated h+ | Integrated h- |
|---|---|---|---|---|---|
| A (applied today by central on-disk plots and alpha-veto plots, per task statement) | `data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_empz_gt45_nominal_490rows.csv` | DJANGOH14 | NO (blind) | `1.1375979129031535` | `1.1366708715220577` |
| B (user-decision nominal) | `data/radiative/djangoh14_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` | DJANGOH14 | YES | `1.1418534312056339` | `1.1413755529189475` |
| C (cross-check) | `data/radiative/rapgap_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` | RAPGAP31 | YES | `1.1368044552974204` | `1.1362373358660744` |

"Integrated" = `sum(sigma_nonrad)/sum(sigma_rad)` over the 245 bins of the charge; verified to
be numerically identical to the sigma_rad-weighted mean of `C_rad` (to 1e-15), so it is both
the ratio-of-integrated-proxies and the exposure-weighted mean.

## Step 1 — identity resolution (by content, not by directory name)

- **A** reproduces the documented blind DJANGOH14 FINAL of
  `CRAD_FINAL_PROVENANCE_2026-09-30.md` (h+ `1.1375979129031535`, h- `1.1366708715220577`) to
  machine precision, and that provenance record names this exact path as the DJANGOH14 chain
  result. Content = **DJANGOH14, blind (no xF cut), EMPZ>45, `nominal` variant**. No content
  mismatch vs the documented record.
- **B** matches the documented DJANGOH14 xFcut baseline of
  `CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md` (h+ `1.1418534`, h- `1.1413756`); the
  co-located `crad_djangoh14_xfcut.json` declares channel
  `charged_sidis_highQ2_crad_xfgen_lt0_djangoh14` with the recomputed current-hemisphere
  `xF<0` convention on both RAD and NONRAD legs. Content = **DJANGOH14 + xFcut**; directory
  name matches content.
- **C** matches the documented RAPGAP31 xFcut baseline (h+ `1.1368045`, h- `1.1362373`);
  `crad_rapgap31_xfcut.json` declares channel `charged_sidis_highQ2_crad_xfgen_lt0_rapgap31`,
  same xFcut convention. Content = **RAPGAP31 + xFcut**; directory name matches content.

### Naming defects

1. The table the plots apply today (A) lives in
   `data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/` — a
   **quarantined** directory named after an **e/sigma-photon variant study**, with no generator
   tag in the file name, and its siblings in that directory are genuine variant products
   (`_esigma_photon_`, `_sigma_photon_`, other EMPZ windows). The file itself is NOT a variant
   diagnostic: it is the documented blind DJANGOH14 FINAL (`nominal` variant). The content is
   faithful to the provenance record; the defect is that the LIVE applied C_rad table is
   simultaneously (a) quarantine-resident, (b) named like a variant-study side product, and
   (c) superseded for Born-level use by the xFcut baseline (BLK-7.1). This is a
   mislocation/mislabeling trap, not a wrong-content defect.
2. No other naming-vs-content mismatch found: B and C contents match their directory names and
   their validation JSONs.

### DJANGOH14 xFcut availability

**Yes — a DJANGOH14 table WITH the xFGen<0 convention exists** (B, 2026-10-05, 490/490 finite,
validation `crad_djangoh14_xfcut.json` PASS). Its difference from the blind DJANGOH14 table in
use (B vs A): integrated `+0.374%` (h+) / `+0.414%` (h-), reproducing the BLK-7.1 ruling
(`+0.37/+0.41%`, blind under-corrects; bias confined to z<0.05 bins). Per-bin structure of
this definition shift is quantified in Step 2.

## Step 2 — generator spread at fixed definition (RAPGAP31 vs DJANGOH14, both xFGen<0)

Per-bin signed relative difference `rel = (C_rad_RAPGAP31 − C_rad_DJANGOH14)/C_rad_DJANGOH14`
(nominal = DJANGOH14 = table B; cross-check = RAPGAP31 = table C), 245 bins per charge,
nearest-rank percentiles:

| Statistic | h+ | h- |
|---|---|---|
| median abs rel | `0.840%` | `0.908%` |
| mean abs rel | `1.630%` | `1.497%` |
| signed mean | `+0.322%` | `−0.173%` |
| p5 (signed) | `−2.687%` | `−3.220%` |
| p95 (signed) | `+5.246%` | `+3.383%` |
| max abs rel | `14.438%` | `18.317%` |
| integrated (sum-proxy) rel | `−0.442%` | `−0.450%` |

Worst bin, both charges: `bin_index 244` (h+) / `489` (h-) — cell 6 (Q2 1500-20000 GeV^2,
x 0.021-1.5), z 0.7-1.0, pT 4-8 GeV, with RAPGAP31 HIGHER than DJANGOH14 (+14.4% / +18.3%).

Structure: the spread rises with z (z-row medians, h+ / h-: 0.54/0.52% at z<0.05 up to
2.69/2.51% at z 0.7-1.0) and concentrates at high pT (all top-5 bins are pT 4-8 or 2-4 GeV) —
a structured fragmentation-model difference, not white noise. `93/245` (h+) and `92/245` (h-)
bins exceed 2x the combined MC-statistical uncertainty of the difference
`sqrt(u_DJANGOH^2+u_RAPGAP^2)`.

### Reproduction and split of the previous MIXED-definition comparison

The previously quoted mixed comparison (max `14.57%` h+ / `18.52%` h-, median `0.93%` / `0.98%`)
is reproduced EXACTLY by RAPGAP31-xfcut vs DJANGOH14-blind (C vs A): max `14.568%` / `18.517%`,
median `0.927%` / `0.982%`. That is the pairing the earlier numbers came from.

Definition-vs-generator split (quantified):

- **At the maxima, the mixed numbers were ~99% generator physics, not definition.**
  h+ bin 244: `14.568%` mixed = `0.114%` definition (blind->xFcut) + `14.438%` generator;
  h- bin 489: `18.517%` = `0.169%` + `18.317%`. The xF-cut definition change contributes only
  0.11-0.17 percentage points at the worst bins.
- **At the median level the definition change contributed ~10% relative:** h+ `0.93% -> 0.84%`
  (~0.09 pp), h- `0.98% -> 0.91%` (~0.07 pp).
- The definition-only shift (B vs A) is confined to z<0.05 exactly as ruled in BLK-7.1: median
  |rel| = `0.000%` in every z>=0.05 row; max `6.825%` (h+) / `3.870%` (h-) inside z<0.05,
  `1.551%`/`1.076%` in z 0.05-0.1, `<=0.36%` elsewhere.
- **Conclusion:** the large quoted maxima were NOT a definition artifact — they persist at
  matched definition (`14.44%` / `18.32%`). Mixing definitions inflated the medians by ~7-10%
  relative and the maxima by ~1% relative.

Statistical quality of the tail: the worst h+ bin (244) is a `3.0 sigma` difference vs the
combined MC stat uncertainty (u_DJ `3.50%`, u_RG `3.42%`); the worst h- bin (489) is `2.2 sigma`
(u `5.64%`/`6.02%`); some large-spread bins are stat-limited (h- bin 454, cell 5 z 0.7-1.0
pT 4-8: `11.71%` at only `1.3 sigma`). So the extreme tail is real but partly MC-statistics
limited.

## Step 3 — component recommendation

**Recommendation (decision-ready):** adopt a new systematic component

`c_rad_gen_rel(bin) = 0.5 x GaussSmooth_pTrow( |rel(bin)| )`,

with `rel = (C_rad_RAPGAP31 - C_rad_DJANGOH14)/C_rad_DJANGOH14` at the matched xFGen<0
definition (Step 2), and the Gaussian kernel `exp(-0.5*(Delta_i)^2/sigma^2)`, `sigma = 1.0`
pT-bin, edge-truncated and renormalized within each (charge, (Q2,x) cell, z-row) five-point
pT row — i.e. the exact construction of `phi_h_gauss_rel`, applied to the single
alternative-generator difference instead of a max over two generators. The kernel
implementation was verified end-to-end against the phi_h sidecar (`gauss_smooth_perbin.csv`)
to `1.4e-17`. This fills the slot the multidim smoothing proposal already reserved as
`crad_gen_envelope` ("pending its convention blocker", `docs/methodology/
systematics_smoothing_multidim_proposal_2026-10-02.md` S0) and resolves that blocker.

### The four definition choices

| Choice | Decision | Reason (numbers) |
|---|---|---|
| Symmetric vs one-sided | **symmetric** | the signed spread has no stable direction: signed mean `+0.322%`/`-0.173%`, p5 `-2.687%`/`-3.220%`, p95 `+5.246%`/`+3.383%`, integrated `-0.442%`/`-0.450%`; the budget stores symmetric positive `*_rel` components. |
| Relative vs absolute | **per-bin relative** | `C_rad` enters multiplicatively (`sigma_Born = C_rad x sigma_rad,true`), so a relative error on `C_rad` is the relative error on the corrected cross-section; matches every `*_rel` component. |
| Full vs half difference | **half-difference (`0.5x`)** | the budget convention for generator-spread brackets: `ibu_model_rel = 0.5*|IBU(R_DJ)-IBU(R_RG)|/nominal` and `phi_h_gauss_rel = 0.5*max_g smoothed|flat_g - nominal|`. The 2026-10-06 decision ("the DJANGOH-RAPGAP difference is the C_rad uncertainty") supplies the envelope INPUT; the `0.5` converts the two-model bracket into a per-model uncertainty, identically to its two siblings. The full-difference variant is exactly `x2` by linearity of the smoother (sensitivities in Step 4). |
| Tail: cap / floor / significance gate / Gaussian envelope | **Gaussian-smoothing envelope** | see below. |

### Why the Gaussian envelope, not a cap, floor, or significance gate

- **Significance gate rejected.** `152/245` (h+) and `153/245` (h-) bins sit below `2x` the
  combined MC-stat uncertainty of the difference; with `u_comb` median `0.62%`/`0.65%`, a
  true 0.5-1% model difference cannot reach `2 sigma` per-bin, so a gate would zero ~62% of
  bins and systematically underestimate. The bulk spread is real, not noise: e.g. bin 0 shows
  `-0.548%` (h+) / `-0.502%` (h-) at `15.7 sigma` / `14.3 sigma`.
- **Cap rejected.** The smoothing already tames the stat-limited tail in a validated,
  precedent-backed way: bin 244 `14.438% -> 5.872%`, bin 489 `18.317% -> 5.834%`, and the
  stat-limited bin 454 `11.709% -> 3.806%` (the `1.3 sigma` case). A cap would add an ad hoc
  knob on top of a mechanism that already has the right behavior.
- **Floor rejected.** The C_rad tables' own MC-statistics (`u_comb`: median `0.62%`/`0.65%`,
  p95 `3.26%`/`4.09%`, max `6.26%`/`9.12%`) are carried per-bin in the tables
  (`relative_stat_unc`) and belong to the future C_rad-application STATISTICAL budget, not to
  the model term; folding them in would double the statistics into the model envelope.
- **Envelope adopted.** Same kernel, same `sigma = 1.0`, same row definition as
  `phi_h_gauss_rel`; it suppresses single-bin MC-stat noise while preserving the real
  structure. The NOMINAL C_rad table itself stays untouched — the radiative-correction rule
  forbids smoothing/filling the correction — only the uncertainty envelope is smoothed,
  exactly as `phi_h_gauss_rel` smooths an uncertainty envelope and not a correction.
- Residual caveat carried with the term: even after smoothing, the extreme tail remains
  partly MC-stat-limited (worst bins `3.0 sigma`/`2.2 sigma`), and the term must stay
  PROVISIONAL DIAGNOSTIC until the xFcut baseline clears review and C_rad application is
  enabled.

### z- / pT-dependent treatment

No additional parametrization is justified or needed. The per-bin smoothed term already
carries the real z-structure (term z-row medians, h+/h-: `0.31%`/`0.30%` at z<0.05 rising to
`1.18%`/`1.38%` at z 0.7-1.0), because the smoothing never mixes z-rows. The deliberate
partial pT-averaging within rows is the point: the high-pT tail is where the MC-stat noise
lives (all top-5 raw bins are pT 2-8 GeV), and a fully pT-resolved term would import
statistical fluctuations (bin 454's `11.71%` at `1.3 sigma`). A flat one-number term is ruled
out by the same z-structure (it would understate the z>=0.5 rows by ~3x).

### Does `c_rad_gen_rel` double-count `ibu_model_rel`?

**No. The two terms measure the same generator pair through disjoint propagation channels,
and the numbers bound any residual shared-driver effect.** Both are built from a
DJANGOH-vs-RAPGAP comparison, but:

1. **Different correction stages.** `ibu_model_rel = 0.5*|IBU(R_DJ) - IBU(R_RG)|/nominal` is
   a detector-response/migration term: two unfoldings of the same data with different
   response matrices; `C_rad` never enters it (C_rad is not applied anywhere in the
   cross-closure). `c_rad_gen_rel` is a generator-level RAD->Born conversion term: a pure
   true-level ratio, no detector, no unfolding. Since `sigma_Born = C_rad x Unfold(data; R)`,
   a single generator-model deficiency propagates through BOTH channels and the two
   contributions ADD; measuring each channel once is required, not redundant. The
   radiative-conversion channel is currently empty in the budget (`c_rad_applied=false`; none
   of the 11 components is a radiative term), so the new term fills an empty slot rather than
   re-measuring a filled one.
2. **Empirical correlation: partial shared driver, not identity.** Per-bin Pearson `r`
   between `c_rad_gen_rel` and `ibu_model_rel`: `+0.506` (h+, Spearman `+0.513`), `+0.402`
   (h-, Spearman `+0.442`). The floor for genuinely unrelated terms is
   `r(c_rad_gen, tracking) = +0.19/+0.20`. If the same effect were measured twice, per-bin
   values would be near-proportional (`r -> 1`); they sit roughly halfway between the
   unrelated-pair floor and identity — a partly common driver (generator fragmentation)
   realized through different channels, consistent with the tail correlations (z>=0.5:
   `+0.58/+0.23`; pT>=4: `+0.32/+0.31`).
3. **The direction of the residual correlation makes double-counting impossible.** Both terms
   are positive envelopes of same-signed generator differences, so honoring the measured
   correlation would RAISE the combined model block, not lower it:
   `sqrt(a^2+b^2+2*rho*a*b)` vs quadrature `sqrt(a^2+b^2)` gives `+0.25 pp` (h+ median-term
   bin 28: `2.16% -> 2.40%`) and `+0.17 pp` (h- bin 436: `1.11% -> 1.28%`); at the two worst
   bins `+2.82 pp` (bin 244: `35.04% -> 37.86%`) and `+1.86 pp` (bin 489: `11.30% ->
   13.16%`) — against bin totals of `162.9%` and `89.2%` (<=2% relative). Quadrature is the
   conservative-in-the-right-direction approximation; there is no configuration in which
   adding the term inflates `ibu_model_rel`'s channel.
4. **The budget's real, documented generator-model overlap is elsewhere.** The
   `phi_h_gauss_rel` README states it "includes fork/prior and generator-model
   contamination", and indeed `r(c_rad_gen, phi_h_gauss) = +0.65/+0.57` — HIGHER than the
   `ibu_model` correlation. `phi_h_gauss_rel` is the term carrying the contamination flag
   (and it remains PROVISIONAL INTERNAL DIAGNOSTIC); `c_rad_gen_rel` introduces no overlap
   that `phi_h_gauss_rel` has not already introduced, and the radiative channel is absent
   from both existing terms.

**Action item:** record the measured correlations (`+0.506/+0.402` vs `ibu_model_rel`,
`+0.65/+0.57` vs `phi_h_gauss_rel`) in the budget provenance, so that if the budget is ever
combined source-resolved, the three generator-pair terms (`ibu_model_rel`, `phi_h_gauss_rel`,
`c_rad_gen_rel`) can be treated as one correlated generator-model block rather than as
independent. At the current quadrature level, ignoring these correlations distorts the
total by at most the bounds above (`+0.25/+0.17 pp` at the median bin). Supporting
classification: the new term's signed input has matched-bin h+/h- correlation `+0.56`
(Pearson), comparable to `ibu_model`'s documented `+0.51` — a genuine coherent model
difference, not white noise.

## Step 4 — budget impact

All numbers below are prospective: computed here and recorded in the companion JSON
(`crad_unc_generator_spread_2026-10-07.json`); **no budget, release, response, or plot file
was modified.** Base budget: `total_uncertainty_z005_with_phih_2026-10-02` (11 components,
`total_with_phih_rel = sqrt(total_with_tracking_rel^2 + phi_h_gauss_rel^2)`, PROVISIONAL
INTERNAL DIAGNOSTIC, `c_rad_applied=false`): h+ median `6.047%`, max `162.887%`; h- median
`6.356%`, max `89.168%` — reproduced exactly by this note's re-computation (budget rows
physics-matched 490/490 via the central `bin_map.csv`).

### New term per charge (recommended half-difference smoothed envelope)

| charge | median | p95 | max (bin) |
|---|---|---|---|
| h+ | `0.530%` | `2.170%` | `5.872%` (bin 244: cell 6, z 0.7-1.0, pT 4-8 GeV) |
| h- | `0.501%` | `2.081%` | `5.834%` (bin 489: same cell/z/pT) |

At the median the term ranks 8th of 12 components for both charges — between `ibu_reg_rel`
(`0.577%` h+ / `0.566%` h-) and `elec_theta_rel` (`0.452%` / `0.497%`) — and is subdominant to
`ibu_stat_rel` (`3.68%`/`3.98%`), `lumi_rel` (`2.70%`), `ibu_model_rel` (`1.73%`/`1.54%`),
`elec_energy_rel` (`1.34%`/`1.64%`), `phi_h_gauss_rel` (`1.38%`/`1.35%`) and `hfs_jes_rel`
(`0.76%`/`0.81%`) at the median.

### 12-component quadrature total (recommended term)

`total_with_phih_and_crad_rel = sqrt(total_with_phih_rel^2 + c_rad_gen_rel^2)`:

| charge | median total | increase at median | worst bin (unchanged) | worst-bin total |
|---|---|---|---|---|
| h+ | `6.081%` (was `6.047%`) | `+0.034 pp` (median per-bin increase `+0.019 pp`) | bin 179 (cell 5: Q2 700-1500 GeV^2, x 0.034-0.15, z<0.05, pT 4-8 GeV) | `162.892%` (was `162.887%`, `+0.005 pp`) |
| h- | `6.364%` (was `6.356%`) | `+0.009 pp` (median per-bin increase `+0.019 pp`) | bin 424 (same cell/z/pT as 179) | `89.173%` (was `89.168%`, `+0.005 pp`) |

The worst-total bins do not move: they are already dominated by existing components (the new
term is only `1.23%` (h+, bin 179) / `0.98%` (h-, bin 424) there), so the C_rad term never
sets the total anywhere.

### Bins where the new term exceeds `ibu_model_rel` or `tracking_eff_m0p5_rel`

- Exceeds `ibu_model_rel`: `42/245` (h+) and `42/245` (h-). Worst offenders: h+ bin 239
  (cell 6, z 0.5-0.7, pT 4-8 GeV: term `4.63%` vs `ibu_model 1.32%`), h- bin 454 (cell 5,
  z 0.7-1.0, pT 4-8 GeV: `3.81%` vs `3.18%`). Full per-bin lists in the companion JSON.
- Exceeds `tracking_eff_m0p5_rel`: `212/245` (h+) and `222/245` (h-) — the term exceeds
  tracking almost everywhere (tracking median `0.18%`).
- Exceeds either: `218/245` (h+) and `223/245` (h-).

### Sensitivity: full-difference reading (no `0.5`)

If the 2026-10-06 decision is read as the FULL per-bin difference, every value scales as
shown (the variant is exactly `x2` by linearity of the smoother): term median
`1.060%`/`1.002%` (p95 `4.341%`/`4.161%`); 12-component total median `6.215%` (h+,
`+0.168 pp`) and `6.391%` (h-, `+0.035 pp`); exceeds `ibu_model_rel` in `76`/`84` bins and
`tracking` in `241`/`238`. Even at `x2` the term remains subdominant at the median.

### Caveat

`C_rad` is not applied centrally (`c_rad_applied=false`; the central v6 release is raw, not
Born-level) and both xFcut tables carry PROVISIONAL DIAGNOSTIC status. The term therefore
sizes the model uncertainty of a correction that is not yet in the central chain: it becomes
meaningful only once C_rad application is enabled, at which point the tables' own per-bin
MC-statistical uncertainty (`u_comb`: median `0.62%`/`0.65%`, max `6.26%`/`9.12%`) must enter
as a separate statistical component. Until then the 12-component totals above are a
prospective sizing only.

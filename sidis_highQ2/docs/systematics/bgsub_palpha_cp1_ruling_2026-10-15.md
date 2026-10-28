# P-ALPHA bgsub Set R′ cone-basis chain — CP-1 gate-failure physics ruling (2026-10-15)

**Task:** READ-ONLY physics ruling on the CP-1 FAIL recorded by the 26-item
official gate of the P-ALPHA background-subtraction Set R′ Stage-2 cone-basis
chain (v6/z005 high-Q² inclusive charged-hadron SIDIS, Q² > 150 GeV², h+/h−
without PID, 490-bin release grid), including the per-bin statistical
adjudication of bin 176 and every failing bin, the charge-correlation /
common-cause analysis, the PM-3 ledger findings, and the PM-4 state.

**Status:** RULING ONLY. No production was launched, no Condor jobs were
submitted, no campaign tree, gate record, RUN_SHEET, spec, or central artifact
was modified. All numbers below were re-derived read-only from the official
JSON/CSV products (login node, seconds; JSON/CSV parsing only, per the
campaign's login-node adjudication precedent). This document is the only
write. Adopting any criterion change proposed here is a **USER DECISION**
(gate/spec adjustment is forbidden in-session per RUN_SHEET discipline).

**Governing documents:**

- Spec: `sidis_highQ2/docs/systematics/BGSUB_S2PRIME_REBASE_ADDENDUM_2026-10-11.md`
  (component #18 line 458; CP-1 line 514; PM-3 line 521; PM-4 line 522).
- Gate record: `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/stage2_setrprime_2026-10-15/gate/bgsub_gate_26items_official.json`
  (schema `bgsub_gate_26items_cone_v1`, mode official; summary n_items 26 /
  n_pass 23 / n_fail 2 (CP-1, PM-3) / n_held 1 (PM-4);
  `all_25_pass_for_promotion` false).
- Measurement: `.../stage2_setrprime_2026-10-15/stage_rel/bgsub_stage_rel_measurement_official.json`
  + sibling `bgsub_stage_signed_difference_official.csv` (per-bin evidence).
- Campaign ledger: `.../bgsub_setr_cone_build_2026-10-11/RUN_SHEET.md`
  (S4 entries; PM-3 root cause; CHAIN STOP AT S4 record).
- Execution adjudication: `sidis_highQ2/docs/systematics/bgsub_palpha_precondition_adjudication_2026-10-15.md`
  (§9 S4 acceptance expectation).
- Pre-S2′ precedent: `.../bgsub_setr_build_2026-10-10/stage_rel/bgsub_stage_rel_measurement_official.json`.

(Paths abbreviated with `...` =
`H1ChargedSidis_HighQ2/output_v6/studies`.)

---

## 1. CP-1 operand definition and recorded numbers (question 1)

**Operand (gate record, republished from spec line 514).** Per-bin

```
|pre-unfold − coneBorn · (1 − f_res′)| / pre-unfold
```

with **operands = the canonical cone Born release**
(`born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13`;
unsubtracted cone Born totals h+ 13.026987 / h− 12.697511 nb; RU-3 dilution
anchors h+ 11.998797 / h− 11.666545 nb). `pre-unfold` is the Set R′
candidate's IBU result on `data_sub′ = D_cone − B_R′_cone`
(B_R′ = cone-applied e + V0 gen-avg + lowQ2); `f_res′` is the per-bin
packaged cone subtraction fraction (SA-4 envelope: medians 3.0635%/3.4409%,
max 31.858%/23.034% at bin 34).

**Thresholds (unchanged from the pre-S2′ chain).** Per-bin ≤ 1%
(`per_bin_rel` 0.01); sparse bins ≤ 2% with written justification
(`per_bin_sparse_with_justification` 0.02); integrated ≤ 0.5%
(`integrated_rel` 0.005). Sparse criterion fixed a priori:
`toy_stat_rel ≥ 0.05` (the bin's own released statistical uncertainty at
least 5× the per-bin window).

**Recorded outcome (official measurement).**

| quantity | h+ | h− |
|---|---|---|
| integrated pre-unfold (nb) | 11.996558161307064 | 11.663955534683723 |
| integrated dilution operand (nb) | 11.998797392290179 | 11.666544718471744 |
| integrated rel dev | 1.8666e-4 (**PASS** ≤ 5e-3) | 2.2198e-4 (**PASS** ≤ 5e-3) |
| bins over 1% (all) | 19 | 24 |
| declared sparse (toy ≥ 5%) | 92 | 102 |
| non-sparse bins > 1% | **4** (106, 176, 181, 211) | **7** (59, 106, 175, 176, 181, 195, 211) |
| bins > 2% | 9 (8 sparse + **176**) | 11 (10 sparse + **176**) |
| sparse exceptions below own stat | 8 / 8 | 10 / 10 |
| `bgsub_stage_rel` median | 0.2103% | 0.2453% |
| `bgsub_stage_rel` max (sparse bins) | 16.873% | 12.569% |

- Integrated: PASS on both charges (identical to the RU-3 integrated
  deviations −0.0187%/−0.0222%; anchor cross-checks post-vs-coneD rel 0.0/0.0).
- Sparse path: every >1% sparse bin is below its own statistical resolution —
  the pre-registered sparse exception path covers them all, as expected.
- **Bin 176 exceeds the 2% sparse window on BOTH charges while being
  non-sparse and above its own toy stat**: h+ 2.084% and h− 2.290% vs
  candidate toy_stat 1.389%/1.420%. This, plus the 11 non-sparse >1% bins,
  is the FAIL.
- Verdict recorded by the measurement and the gate: **FAIL** (a genuine STOP
  condition; no interpretation was improvised at execution time — RUN_SHEET).

**The adjudicated expectation that failed** (execution adjudication §9 S4,
spec line 514): "CP-1 expected PASS_WITH_RECORDED_EXCEPTIONS on sparse bins"
— i.e. exceptions confined to sparse-below-stat bins, with
`bgsub_stage_rel` "expected SMALLER" than the pre-S2′ medians
(0.0909%/0.0950%, spec component #18 line 458).

---

## 2. Per-bin statistical resolution of bin 176 and every failing bin (question 2)

### 2.1 The correct null for a stage difference

The two CP-1 operands are **not statistically independent**: CP-2 proves
`data_sub′ + B_R′_cone == D_cone` bitwise, so the candidate's pre-unfold and
the cone Born release derive from the *same data vector*. The data-Poisson
fluctuation therefore cancels in the stage difference to first order (it
re-enters only through the non-linearity of the unfold, i.e. through the very
effect being measured, and at O(f) through the dilution operand). The
statistically meaningful uncertainties of the difference are:

- the **template-MC statistics** of the subtracted background, propagated
  through the unfold — carried per bin as `bgsub_stat_rel` (budget component
  #16; raw scale `sqrt(varB_R′)/B_R′` = 0.22–0.89% in the z0/z1 failing bins,
  5.4–6.8% in the z4 corner bins 59/195);
- the **toy sidecar** statistics of the two released operands (candidate toy
  `toy_stat_rel`; canonical-release 1000-toy sidecar `ibu_stat_rel_new`),
  which enter the difference only at O(f) because of the cancellation above.

The per-bin resolution of the stage difference is therefore adjudicated with
the conservative construct

```
σ_Δ = hypot( f_res′ · toy_nom_sidecar , bgsub_stat_rel , bgsub_stat_rel · f_res′/(1−f_res′) )
z_sub = signed_dev / σ_Δ          (deterministic-effect resolution)
z_own = |dev| / toy_stat_rel      (naive own-stat resolution, for reference)
```

### 2.2 Adjudication table — bin 176, all 11 failing bins, and their charge mirrors

Union of failing bins {59, 106, 175, 176, 181, 195, 211} × both charges
(14 rows; h+ 59/175/195 are sub-window mirrors of the h− failures). All 14
bins are **non-sparse** (`declared_sparse = False`). `rem−f` = actual
unfolded removal minus diagonal f_res′, in pp.

| chg | bin | cell (z,pT) | f_res′ % | signed dev % | toy_cand % | toy_nom % | bgsub_stat % | σ_Δ % | z_own | z_sub | κ | rem_act % | rem−f pp |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| h+ | 59 | S0_x1 (4,4) | 3.884 | −0.629 | 4.265 | 4.152 | 0.2234 | 0.2757 | 0.15 | −2.3 | 0.155 | 4.485 | +0.600 |
| h+ | 106 | S1_x1 (0,1) | 5.710 | +1.122 | 0.767 | 0.763 | 0.0239 | 0.0497 | 1.46 | **+22.6** | 0.187 | 4.640 | −1.070 |
| h+ | 175 | S2_x1 (0,0) | 13.657 | −0.969 | 0.750 | 0.668 | 0.0348 | 0.0978 | 1.29 | −9.9 | 0.061 | 14.486 | +0.828 |
| h+ | **176** | S2_x1 (0,1) | 5.985 | **+2.084** | 1.389 | 1.378 | 0.0372 | 0.0905 | 1.50 | **+23.0** | 0.334 | 3.984 | −2.001 |
| h+ | 181 | S2_x1 (1,1) | 4.943 | +1.206 | 1.842 | 1.836 | 0.0465 | 0.1020 | 0.65 | **+11.8** | 0.235 | 3.783 | −1.160 |
| h+ | 195 | S2_x1 (4,0) | 2.824 | +0.473 | 3.966 | 3.984 | 0.1913 | 0.2220 | 0.12 | +2.1 | 0.164 | 2.362 | −0.462 |
| h+ | 211 | S3_x0 (0,1) | 5.324 | +1.357 | 1.112 | 1.106 | 0.0301 | 0.0662 | 1.22 | **+20.5** | 0.245 | 4.022 | −1.303 |
| h− | 59 | S0_x1 (4,4) | 4.716 | −1.026 | 4.764 | 4.731 | 0.2663 | 0.3477 | 0.22 | **−3.0** | 0.205 | 5.684 | +0.968 |
| h− | 106 | S1_x1 (0,1) | 5.812 | +1.193 | 0.771 | 0.743 | 0.0246 | 0.0497 | 1.55 | **+24.0** | 0.196 | 4.675 | −1.137 |
| h− | 175 | S2_x1 (0,0) | 14.378 | −1.094 | 0.752 | 0.693 | 0.0368 | 0.1064 | 1.46 | **−10.3** | 0.064 | 15.305 | +0.927 |
| h− | **176** | S2_x1 (0,1) | 6.030 | **+2.290** | 1.420 | 1.375 | 0.0380 | 0.0912 | 1.61 | **+25.1** | 0.365 | 3.828 | −2.203 |
| h− | 181 | S2_x1 (1,1) | 5.273 | +1.220 | 1.834 | 1.942 | 0.0494 | 0.1137 | 0.67 | **+10.7** | 0.222 | 4.102 | −1.170 |
| h− | 195 | S2_x1 (4,0) | 4.292 | +1.046 | 4.754 | 4.731 | 0.3055 | 0.3671 | 0.22 | **+2.9** | 0.236 | 3.280 | −1.012 |
| h− | 211 | S3_x0 (0,1) | 5.466 | +1.418 | 1.199 | 1.135 | 0.0314 | 0.0696 | 1.18 | **+20.4** | 0.249 | 4.106 | −1.360 |

(Bold z_sub = the 11 CP-1-failing rows. `κ = |signed_diff|/(born·f_res′)` —
see §5. `toy_nom` = canonical cone-release 1000-toy sidecar
`ibu_stat_rel_new`; `bgsub_stat` = budget19 component #16 per bin.)

### 2.3 Adjudication

- **Against the bins' own released statistics, nothing is resolvable.**
  z_own spans 0.12–1.61 over the 14 rows (bin 176: 1.50 h+ / 1.61 h−).
  Over the full non-sparse field the maximum is z_own = 1.97 (h+, bin 35) /
  2.09 (h−, bin 105); two-sided p_max = 0.049 / 0.036; **Bonferroni over the
  non-sparse multiplicity (153 h+ / 143 h− tests) gives p = 1.00 on both
  charges.** The workspace's look-elsewhere convention is Bonferroni on an
  exact null (precedent: cone gate-suite per-file data cone-rejection 2/648,
  exact gamma-Poisson NB-mixture null, Bonferroni p = 1.0, adjudicated
  BENIGN — `sidis_highQ2/docs/promotions/alphaveto_cone_promotion_2026-10-13.md`
  line 57, mirrored in RUN_STATUS.md); no Benjamini–Hochberg procedure is in
  use anywhere in this campaign. Under that convention the fluctuation
  hypothesis **cannot be rejected** — but it also cannot explain the pattern
  (next bullet and §3).
- **Against the physically correct subtraction-uncertainty null, every
  failing bin is resolved.** z_sub = 2.9–25.1σ over the 11 failing rows
  (bin 176: 23.0σ h+ / 25.1σ h−; the three sub-window mirror rows resolve at
  2.1–9.9σ). The deviations are **deterministic**, not fluctuations: the
  data-Poisson term that dominates `toy_stat_rel` cancels bitwise between
  the two operands, and what remains — template-MC statistics at full size —
  is 10–100× too small to produce 1–2.3% deviations in these bins.
- **Bin 176 specifically:** h+ +2.084% / h− +2.290% (both above the 2%
  sparse window), z_own 1.50/1.61 (unremarkable as a fluctuation),
  z_sub 23.0/25.1 (unambiguous as a deterministic effect), κ 0.334/0.365,
  actual unfolded removal 3.984%/3.828% vs diagonal 5.985%/6.030%
  (deficit −2.001/−2.203 pp).
- Every failing bin is non-sparse by the a-priori criterion, so the
  pre-registered sparse exception path cannot cover any of them; the gate
  correctly recorded FAIL rather than reinterpreting the pinned criteria.

---

## 3. Charge correlation and common cause — why bin 176 fails both charges (question 3)

h+ and h− are statistically independent data samples. If the per-bin stage
deviations were fluctuations, the signed h+ and h− deviation fields would be
uncorrelated. They are not:

- Pearson r of the signed relative stage deviations
  (`signed_diff_nb / post_unfold_dilution_nb`) across all 245 matched bins:
  **r = 0.839** (sign agreement 87.3%); restricted to the 182 bins where the
  subtraction actually acts (f_res′ ≥ 2%): **r = 0.912** (sign agreement
  90.1%).
- **All 7 union bins deviate with the same sign in both charges**, including
  the three bins that fail on one charge only (59: −/−, 175: −/−, 195: +/+).
  The four doubly-failing matched bins (106, 176, 181, 211) have per-bin
  signed magnitudes that are nearly equal across charges — e.g. bin 211:
  +0.0011100 nb (h+) vs +0.0011116 nb (h−), a 0.14% relative difference
  between independent samples.
- **Common cause identified: response-mediated non-locality of the cone-basis
  subtraction, organized as a pT0/pT1 dipole at z0.** Signed deviations of
  the z0 pT0 / pT1 pairs, both charges:

| pair (z0) | h+ pT0 → pT1 | h− pT0 → pT1 | f_res′ pT0 (h+/h−) |
|---|---|---|---|
| S0_x0 (0/1) | −0.103% → +0.264% | −0.108% → +0.280% | 10.3/10.6% |
| S0_x1 (35/36) | −0.548% → +0.681% | −0.599% → +0.732% | 10.9/11.2% |
| S1_x0 (70/71) | +0.035% → +0.309% | +0.042% → +0.330% | 10.0/10.3% |
| S1_x1 (105/106) | −0.734% → **+1.122%** | −0.795% → **+1.193%** | 11.9/12.4% |
| S2_x0 (140/141) | +0.295% → +0.473% | +0.296% → +0.502% | 9.9/10.3% |
| S2_x1 (175/176) | −0.969% → **+2.084%** | −1.094% → **+2.290%** | 13.7/14.4% |
| S3_x0 (210/211) | −0.488% → **+1.357%** | −0.464% → **+1.418%** | 12.5/12.9% |

  All 14 z0-pT1 bins are under-removed (signed positive); 10 of 14 z0-pT0
  bins are over-removed (signed negative), with the over-removal leg present
  in both S0 cells and the high-x cells of S1–S3 (S0_x0, S0_x1, S1_x1,
  S2_x1, S3_x0) and absent only in the low-x S1_x0/S2_x0 cells
  (+0.04…+0.30%). The pT0 over-removed bins carry the largest f_res′ in the
  field (10–14.4%).
- The field is **continuous under the 1% window**: the h+ mirrors of the h−
  failures sit at −0.969% (175), −0.629% (59), +0.473% (195) — the same
  structures, marginally below threshold. The gate threshold landed inside a
  smooth deterministic field, not on isolated outliers.
- **Mechanism.** The unfolded background migrates across the pT0/pT1
  boundary: in the pT1 bins the unfolded removal is 19–37% *below* the
  diagonal B_R′ (deficit −1.0…−2.2 pp on f_res′ ≈ 4.9–6.0%), while in the
  pT0 partners it *exceeds* the diagonal by +0.6…+0.97 pp on f_res′ ≈
  10–14.4% — consistent with pT-boundary migration leakage of the
  cone-applied (low-pT-enriched) background templates through the S2′
  variant response, whose enlarged fake column carries integrated removal
  11.3063% (h+) / 10.9326% (h−) (RU-2). The net field closes in integral:
  −0.0187%/−0.0222% integrated.
- **Why the cone basis produced it.** The cone-applied per-event templates
  roughly doubled the per-bin f_res′ in exactly these cells relative to the
  pre-S2′ chain — bin 176: 2.98% → 5.99% (h+), 2.92% → 6.03% (h−); bin 175:
  3.57% → 13.66% (h+), 3.66% → 14.38% (h−) — so the response-mediated
  non-locality (which scales with the subtracted fraction and its migration
  pattern, not with the released statistical resolution) grew correspondingly.
- **Pre-S2′ contrast on the same 14 bins** (no-cone Set R chain,
  2026-10-10, same measure semantics): stage differences 0.043–0.298%
  (bin 176: 0.107%/0.156%); verdict PASS_WITH_RECORDED_EXCEPTIONS with
  **zero** non-sparse bins >1% and all exceptions sparse-below-stat;
  stage medians 0.0733%/0.0817%; budget `bgsub_stage_rel` medians
  0.0909%/0.0950%. The cone basis legitimately changed the subtraction
  field; the sparse-only expectation was an extrapolation from the
  pre-S2′ configuration.

**Conclusion for bin 176:** it fails both charges because it sits on the
under-removal leg of the S2_x1 z0 dipole, the strongest cell of a
deterministic, charge-correlated, spatially continuous field — not because
of any bin-specific defect or fluctuation.

---

## 4. Defect exclusion (input to question 4)

Every item that could indicate a machinery defect passed exactly, on the
official products: RU-1 bitwise/graded reproduction of the canonical cone
RAW release (11.412233/11.123626 nb; config diff vs nominal = input vector
only); CP-2 add-back closure bitwise (≤1e-9, measured 0.0); RU-2 cone
variant f_i byte-identical, integrated removal inside windows
(11.3063%/10.9326%); RU-4 C_rad bitwise (table sha `cf810be4…` == spec pin);
RU-5 physicality + exact-text serialization (max dev 0.0 on all 490×7
`*_nb` columns); SA-1..SA-5 exact vs pins (0 true negatives; B_R′ sums
exact; E1b exactly-once Poisson; f_res′ field matches the spec-pinned SA-4
envelope exactly; integrated asymmetry −0.2200 pp in window);
IV-1..IV-7 exact (template totals as packaged; data vector identity
980/980; f_i parity; cone-parity guards).

CP-1 is moreover **insensitive to a global template-normalization error by
construction**: both operands consume the same B_R′ field (the candidate via
the subtraction, the dilution operand via f_res′), so a global B error
cancels to first order in the difference. Attributing the entire integrated
deviation (0.019%/0.022% of Born) to a global B-normalization error would
require |δ| ≈ 0.019%/7.28% ≈ 0.26% < 0.3% of B — while the failing bins
exhibit *local* unfolded-removal mismatches of 19–37% of the local B. No
normalization-error channel can produce the observed dipole; the effect is
response-mediated, as concluded in §3.

---

## 5. RULING (question 4)

### RULING: **(a) EXPECTATION-REVISION** — the CP-1 adjudicated expectation was wrong; the chain is not defective.

**One-line justification:** the 1.0–2.3% non-sparse deviations are
deterministic (z_sub = 2.9–25.1σ against the subtraction uncertainty, with
the data-Poisson term cancelling bitwise between operands),
charge-correlated (r = 0.912; same sign in both charges in every failing
bin), spatially structured (z0 pT0/pT1 dipole), continuous with the
sub-window field, integral-closing (−0.019%/−0.022%), and produced by
bitwise-verified machinery — they are the genuine response-mediated
non-locality of the cone-basis subtraction, which the sparse-only
expectation (an extrapolation from the pre-S2′ chain, where the same bins
showed only 0.04–0.30%) failed to anticipate.

Not (b) REAL DEFECT: no arithmetic, identity, response, provenance, or
normalization channel can account for the pattern (§4), and the effect's
magnitude and structure follow from the cone-applied template field
(~2× f_res′ in the affected cells) plus the S2′ variant response. Not (c)
INCONCLUSIVE: the statistical and correlation evidence is unambiguous on
every failing bin.

### Consequences under (a)

- `bgsub_stage_rel` (component #18) already carries the **full measured**
  per-bin differences at full size by design, so the 19-component budget and
  `total_with_bgsub19_rel` require **no re-derivation** under this ruling.
  The spec's "expected SMALLER" prediction for the stage medians is
  superseded by the realized 0.2103%/0.2453% (vs pre-S2′ 0.0909%/0.0950%).
- The integrated ≤0.5% clause holds with large margin; the sparse exception
  path worked exactly as pre-registered; only the non-sparse per-bin clause
  needs a corrected expectation.

### Corrected CP-1 criterion (PROPOSED TEXT — adoption is a USER DECISION)

> **CP-1 (cone-basis, corrected).** Per-bin
> `|pre-unfold − coneBorn·(1−f_res′)|/pre-unfold` ≤ 1%; integrated ≤ 0.5%.
> A per-bin exceedance is a **recorded exception** (not a FAIL), carrying its
> full measured difference in `bgsub_stage_rel`, if either path applies:
>
> 1. **Sparse path (unchanged, a priori):** `toy_stat_rel ≥ 0.05` AND the
>    deviation is below the bin's own released statistical uncertainty;
>    written justification per bin.
> 2. **Non-sparse charge-correlated non-locality path (new):** the deviation
>    has the **same sign in both charges at the matched bin**, AND
>    `κ = |signed_diff| / (born · f_res′) ≤ 0.5` — i.e. the local
>    unfolded-removal mismatch is at most half of the subtracted fraction,
>    consistent with response-mediated migration of the subtracted
>    background rather than a subtraction error.
>
> Any bin exceeding 1% that satisfies neither path remains a FAIL.

Realized-field calibration of the new path: κ over the 487 bins with
f_res′ > 0 has p50 = 0.069, p75 = 0.162, p90 = 0.329, p95 = 0.501; all 11
non-sparse exceptions have κ ≤ 0.365 (bin 176: 0.334/0.365), i.e. they sit
inside the p90 of the field, and every one of them is same-sign across
charges. Under this criterion the realized measurement is
**PASS_WITH_RECORDED_EXCEPTIONS** (integrated PASS; 8+10 sparse exceptions
unchanged; 11 non-sparse exceptions recorded with κ and sign evidence).

---

## 6. PM-3 technical facts (question 5)

**Finding (verified read-only, both in the earlier session and re-verified
fresh for this ruling):**

- The gate's **sole** PM-3 failure is
  `ledger_extension_markers.AGGREGATED_djangoh14_lowq2 = false`. All 10
  ledger pin equalities MATCH (ledger == fresh == pin); no bad or
  blind/quarantine manifest files; the applied C_rad table
  `cf810be4…` equals the spec pin (the adjudication §7/§C.4 value
  `205e8218…` is STALE and is superseded in the gate evidence).
- The candidate `provenance/inputs_sha256_ledger.json` has **37 entries**
  and contains only three npz's: the cone S2′ migration npz
  (`736fae14…`) and the two **electron** template npz's
  (`a04dd1a6…` / `66141a54…`). The lowQ2 template npz
  `AGGREGATED_djangoh14_lowq2_djbkgd_L351000_finalcandidate.npz` is absent.
- **Root cause:** the packager's `ledger_paths` block
  (`package_bgsub_setrprime_born_release_cone.py`, lines 1178–1199) adds only
  `TEMPLATE_FILES["rg_electron"]` / `TEMPLATE_FILES["dj_electron"]` from the
  background-template directory; the gate
  (`run_bgsub_gate_cone.py`, lines 1055–1063) hard-codes
  `AGGREGATED_djangoh14_lowq2` among its 14 required ledger markers. The
  gate is **stricter than the spec text**, which enumerates only the cone
  campaign artifacts, the two carried electron npz's, and the C_rad table.
- **No provenance is actually lost:** the lowQ2 npz is a consumed,
  spec-pinned chain input (B_R′ = e + V0 + lowQ2; IV-1 totals verified:
  208.3795), and its sha256
  `c40227f007bb9743f7a26528e4879bb653a67963aeca1e488d33633f9edcb063` is
  recorded in the S1 builder's
  `vectors/bgsub_setrprime_vectors.json` `sha256_inputs` (14 entries) —
  **re-verified fresh against the file this session: identical**. The four
  V0/detint template npz's are likewise absent from the candidate ledger and
  are *not* required as gate markers; the candidate ledger is non-exhaustive
  over template npz's by construction.
- Verdict: **primary = packager ledger-extension coverage gap; secondary =
  spec-text vs gate-marker-list inconsistency.** This is a provenance-hygiene
  finding with no physics content; it is independent of CP-1.

**Resolutions (both USER DECISIONS; none executed here):**

- **(A) Record-only annotation.** The candidate stays byte-frozen. The
  records (RUN_SHEET already carries the root cause; this ruling adds the
  fresh sha re-verification) establish the lowQ2 input's identity via the
  spec pin + the S1 vectors `sha256_inputs` + the gate evidence. The
  candidate ledger remains non-exhaustive over template npz's, consistent
  with its treatment of the V0/detint npz's. Consequence: the gate marker
  `AGGREGATED_djangoh14_lowq2` would still evaluate false on any re-run, so
  this resolution requires a gate-marker amendment (or an explicit
  recorded-exception reading of PM-3) at promotion time — a gate adjustment,
  hence a user decision.
- **(B) Ledger re-edition via one-line packager extension + repackage.**
  Add the lowQ2 npz to `ledger_paths` and re-run the packager (login node,
  deterministic, 0 Condor jobs). The physics products are bit-identical in
  content, but the candidate directory identity changes (new
  `provenance/inputs_sha256_ledger.json`, new manifest), so the full 26-item
  gate must be re-run against the repackaged candidate and PM-2's
  candidate-name/ledger entries must be re-recorded. This resolves PM-3
  without any gate/spec change but churns the frozen candidate.

Either resolution is compatible with ruling (a); the choice does not affect
any physics number.

---

## 7. PM-4 independence from CP-1 (question 6)

**Yes — PM-4 is content-independent of the CP-1 ruling; it is HELD purely by
the state rule.**

- PM-4 (`HELD_PENDING_GATE_PASS`) requires 25× PASS (items 1–25 by id, with
  CP-1's expected terminal state PASS_WITH_RECORDED_EXCEPTIONS and PM-4
  READY_FOR_USER_DECISION). The realized count is 23 — the shortfall is
  exactly CP-1 + PM-3. No third blocker exists.
- The promotion content PM-4 guards is already adjudged on its own merits:
  CP-5 PASS (budget closure: 15 components byte-identical to the installed
  leg-envelope package — 21,070 byte-carry cells, 0 mismatches; 19-component
  quadrature identity max dev 0.0 ≤ 1e-12; budget GATE_GRADE), and
  `bgsub_stage_rel` already carries the full measured per-bin stage
  differences, so **no budget re-derivation is implied by ruling (a)**.
- PM-3 is a provenance-hygiene item (§6) with no physics content.
- Therefore: if the user adopts the corrected CP-1 criterion (§5) and a PM-3
  resolution (§6), PM-4 unblocks through the normal state rule without any
  further physics work. Until then the chain remains stopped at S4: S5 NOT
  executed, `P5_RECORD.md` NOT written, the STATE.md P-ALPHA closure entry
  NOT written, the candidate remains **CANDIDATE ONLY**, nothing canonical
  touched, and the 2-Condor-job budget is spent (clusters 5658879, 5658880).

---

## 8. Summary of required user decisions

1. **CP-1 criterion**: adopt (or reject) the corrected criterion text of §5
   (non-sparse charge-correlated non-locality exception path). Adoption
   converts the realized measurement to PASS_WITH_RECORDED_EXCEPTIONS; the
   budget needs no change either way.
2. **PM-3**: choose record-only annotation (A) — requiring a gate-marker
   amendment or recorded-exception reading at promotion — or the one-line
   packager extension + repackage + full gate re-run (B).
3. **Chain resumption**: both decided, PM-4 unblocks via the state rule; S5
   (records, STATE.md P-ALPHA closure entry, promotion and quoting switch)
   remains all-user-decision territory.

# Cone vs no-cone comparison — H1 high-Q² charged-SIDIS (2026-10-11)

- **Date:** 2026-10-11. **Type:** READ-ONLY comparison over the validated
  cone-base products of the `cone_alpha_central_build_2026-10-11` campaign.
  No builds, no promotion, no edits to any release or study tree.
- **Question:** how much does imposing the cone α≥0.2 (3D opening angle to the
  reconstructed scattered electron; `not_e` kept) change the published
  high-Q² charged-SIDIS results, and does the measured change match the
  predictions of the impact study
  (`docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md`)?
- **Bases compared (both validated, S2′ detint-fake convention, IBU k=4):**
  - **no-cone (canonical S2′):** RAW
    `output_v6/central/data_release/bbb_ibu_detints2prime_..._2026-10-09/`
    (integrated IBU h+ 11.446174 / h− 11.158554 nb); Born
    `born_crad_dj14relbin_detints2prime_..._2026-10-10/` (13.064758 / 12.736523 nb).
  - **cone (study-tree, R9 PROVISIONAL):** RAW
    `output_v6/studies/cone_alpha_central_build_2026-10-11/production/release_driver/bbb_ibu_cone_alpha_central_..._2026-10-11/`
    (11.412233 / 11.123626 nb); Born
    `born_crad_dj14relbin_cone_alpha_central_..._2026-10-11/`
    (13.026987 / 12.697511 nb; per-bin C_rad byte-identical to canonical,
    table sha256 `205e8218…`, 17/17 validation gates green).
- **Verdict in one line:** the cone shifts the integrated results by
  **−0.297% (h+) / −0.313% (h−) RAW IBU** and **−0.289% / −0.306% Born**
  (≈ −0.30% as expected), damped from the −0.490% in-grid data-vector removal
  by the response-side losses exactly as the response accounting predicts;
  the per-bin hot bins (worst −42.9% / −47.5% unfolded, data removals up to
  37.5%/34.9%) sit in the impact study's predicted 30–37% band at low z, and
  the f_i shift is DOWN by 0.004–0.024 pp — inside every predicted band. The
  measured change MATCHES the impact-study predictions in all load-bearing
  quantities; one non-load-bearing decode annotation (the hot-bin pT range)
  is superseded by the release-level measurement.

---

## 1. Integrated deltas (cone vs no-cone)

| Quantity | h+ no-cone | h+ cone | h+ delta | h− no-cone | h− cone | h− delta |
|---|---|---|---|---|---|---|
| RAW IBU (nb) | 11.446174 | 11.412233 | **−0.2965%** | 11.158554 | 11.123626 | **−0.3130%** |
| Born IBU (nb) | 13.064758 | 13.026987 | **−0.2891%** | 12.736523 | 12.697511 | **−0.3063%** |
| BBB (nb, reference) | 11.455122 | 11.420985 | −0.2980% | 11.169982 | 11.134861 | −0.3144% |

Full precision: RAW deltas −0.296527% / −0.313013%; Born deltas −0.289100% /
−0.306298%. Both methods and both levels agree: an integrated shift of
**≈ −0.30%**, slightly larger in magnitude for h−. The Born per-bin shift
equals the raw per-bin shift to max dev 2.2e-14 (C_rad is a per-bin factor,
byte-identical between the bases — Born log gate ≤1e-12 PASS).

## 2. Per-bin deltas (RAW IBU, 245 release bins per charge)

Distribution of the per-bin relative shifts (cone/no-cone − 1):

| Charge | median | min | max | \|Δ\|>1% | \|Δ\|>2% | n positive |
|---|---|---|---|---|---|---|
| h+ | +0.0008% | −42.93% | +4.28% | 52/245 (21.2%) | 36 | 123/245 |
| h− | −0.0004% | −47.54% | +3.33% | 57/245 (23.3%) | 34 | 122/245 |

**Worst three bins per charge** (decode from the release CSV's own
cell/z/pT columns):

| Charge | Release bin | Cell / z / pT | RAW IBU shift | data removal |
|---|---|---|---|---|
| h+ | 59 | S0_x1, z∈[0.3,0.5), pT∈[4.0,8.0) | **−42.93%** (5.5805e-3 → 3.1850e-3 nb) | 31.89% |
| h+ | 24 | S0_x0, z∈[0.3,0.5), pT∈[4.0,8.0) | **−42.20%** (5.9119e-3 → 3.4169e-3 nb) | 37.54% |
| h+ | 19 | S0_x0, z∈[0.2,0.3), pT∈[4.0,8.0) | **−26.74%** | 25.92% |
| h− | 59 | S0_x1, z∈[0.3,0.5), pT∈[4.0,8.0) | **−47.54%** (5.2360e-3 → 2.7470e-3 nb) | 34.67% |
| h− | 24 | S0_x0, z∈[0.3,0.5), pT∈[4.0,8.0) | **−38.19%** (6.0697e-3 → 3.7516e-3 nb) | 34.94% |
| h− | 19 | S0_x0, z∈[0.2,0.3), pT∈[4.0,8.0) | **−26.03%** | 25.30% |

Next-worst (both charges): bins 54 (S0_x1, z∈[0.2,0.3), pT∈[4.0,8.0);
removals 19.91%/19.66%, shifts −24.04%/−24.51%) and 89 (S1_x0, z∈[0.2,0.3),
pT∈[4.0,8.0); removals 15.10%/14.87%, shifts −16.85%/−16.14%). The hot region
is **low z (0.2–0.5) × high pT (4.0–8.0 GeV) in the low-Q² strata (S0/S1)** —
i.e. exactly the region where electron-adjacent tracks carry large photon-axis
pT. Positive tail (largest gains): h+ bin 69 (S0_x1, z∈[0.7,1.0), pT∈[4.0,8.0))
+4.28%, bin 49 (S0_x1, z∈[0.1,0.2), pT∈[4.0,8.0)) +3.52%; h− bin 49 +3.33%,
bin 99 (S1_x0, z∈[0.5,0.7), pT∈[4.0,8.0)) +2.57% — unfolding redistribution
from the depleted hot bins; roughly half the bins shift positive while the
integrated effect stays −0.30%.

## 3. Comparison with the impact-study predicted hot bins

The impact study (§2, from α<0.1 track-level QA counters) predicted: hot-bin
data removal ≈36.6% ("global bin 24, h+ S0x0, z∈[0.2,0.3), pT∈[1.0,1.5) under
the v3 binmap decode"), next bins ≈33.7/33.7/30.7%, with the explicit
statement that **α≥0.2 per-bin removal is ≥ these** (larger cone); worst-bin
MC removed population 96–99% true e±.

Measured (α≥0.2, actual cone data vector, release level):

- **Magnitudes MATCH:** worst release-bin removals 37.54% (h+) / 34.94% (h−),
  top-5 per charge spanning 14.9–37.5% — the measured maximum exceeds the
  α<0.1 figure (36.6%) exactly as the "≥" statement requires, and the
  30–37% hot-bin band is confirmed.
- **Low-z placement MATCHES:** all top bins sit at z 0.2–0.5 in the S0/S1
  strata.
- **pT annotation SUPERSEDED:** the impact study's decode placed its hot bin
  at pT∈[1.0,1.5); the release-level measurement (authoritative — read from
  the release CSV's own `pT_min/pT_max` columns) places every top bin at
  **pT∈[4.0,8.0)** (pT_idx 4). The study itself hedged this decode ("under
  the v3 binmap decode"), and its per-bin figures were fine-grid α<0.1
  track-level counts, not release-level α≥0.2 vector comparisons. This does
  not affect any conclusion of that study: its load-bearing content was the
  magnitude band and the electron-dominated composition, both confirmed.
- **Local amplification is real and understood:** the unfolded per-bin shifts
  exceed the local data removals (e.g. h+ bin 59: −42.9% unfolded vs 31.9%
  removed). The cone removes more DATA content in the hot bins than the MC
  response describes — the validated ×1.8–2.6 data/MC in-cone excess
  (`docs/status/alpha0p2-background.md`) — so the response-side efficiency
  recovery is proportionally smaller than the data loss locally.

## 4. f_i shift

**Official campaign crosscheck** (central convention, cone response vs June
no-cone merged; per combo, 490 fine reco rows):

| Combo | bit-identical | float-noise ≤1e-6 | real-change >1e-6 | median rel | max rel | max abs Δf_i | denom drop |
|---|---|---|---|---|---|---|---|
| RAPGAP h+ | 207 | 156 | 127 | 0.1748% | 2.2373% | 0.01368 | −0.0940% |
| RAPGAP h− | 212 | 152 | 126 | 0.1651% | 2.5578% | 0.01402 | −0.0923% |
| DJANGOH h+ | 208 | 152 | 130 | 0.1794% | 2.3050% | 0.01293 | −0.1011% |
| DJANGOH h− | 212 | 153 | 125 | 0.1762% | 2.2897% | 0.01245 | −0.1006% |

All cone f_i ∈ [0,1); the largest per-bin |Δf_i| moves DOWN in every combo
(the worst bins lose fake share).

**Integrated shift** (computed read-only from the crosscheck CSVs and the
cone S2′ variant response):

| Convention | h+ no-cone → cone | h+ shift | h− no-cone → cone | h− shift |
|---|---|---|---|---|
| Central, response-internal (RG / DJ) | 10.6153→10.6089 / 10.7626→10.7553 | −0.0064 / −0.0073 pp | 10.3124→10.3086 / 10.4546→10.4496 | −0.0039 / −0.0051 pp |
| Central, data-weighted on own D | 10.7141→10.6899 / 10.7863→10.7663 | −0.0242 / −0.0200 pp | 10.4019→10.3843 / 10.4712→10.4566 | −0.0176 / −0.0146 pp |
| **S2′ variant, on-D (the published chain's f_i)** | **11.3577 → 11.3344** | **−0.0233 pp** | **10.9768 → 10.9602** | **−0.0166 pp** |

**Direction: DOWN everywhere; magnitude 0.004–0.024 pp.** The impact study
predicted ≲0.1–0.2 pp response-internal and ≲0.1–0.3 pp data-weighted,
"plausibly down" — the measurement is inside every band by a factor ≳5 and
confirms the direction.

## 5. Yield accounting closure (raw removal → unfolded −0.30%)

- **Data side (exact, from the cone data vector):** in-PS 28,582 of 3,902,480
  = 0.7324% (h+ 14,465 / h− 14,117); **in-grid 19,024 of 3,884,097 = 0.4897%**
  (h+ 9,601 = 0.4861%, h− 9,423 = 0.4937%). The cone data vector is per-bin
  **bit-identical** to the June alpha0p2 vector (max dev 0, both charges) —
  two independent builds of the same selection on the same 658 trees.
- **Unfolded:** −0.2965% (h+) / −0.3130% (h−) — a damping of the in-grid data
  loss by **+0.1895 / +0.1806 pp**.
- **Response side (campaign records):** the Phase-F guard measured the cone
  response BODY content difference vs the no-cone S2′ response at
  **0.2045%–0.2158%** (RG h+ 0.2079%, RG h− 0.2101%, DJ h+ 0.2045%, DJ h−
  0.2158%); response-eligible MC rejections 162,528 of 141,450,334 = 0.1149%
  (h+ 83,782/71,867,097 = 0.1166%, h− 78,746/69,583,237 = 0.1132%), of which
  matched-signal 152,560 legs — i.e. the in-cone loss is concentrated in the
  matched-signal (true-hadron) class, consistent with the impact study's
  measured true-hadron in-cone rates 0.10649% (RG) / 0.11354% (DJ), a SIGNAL
  loss recovered statistically by the acceptance correction; unflagged-fake
  rejections 8,195 and det.int. 1,773 — the f_i denominator drop is only
  0.092–0.101%.
- **Closure:** first order, Δσ/σ ≈ (in-grid data loss) + (response signal-
  efficiency recovery). The body lost ≈0.21% of matched content, so the IBU
  acceptance correction rises ≈0.21%, recovering +0.19–0.22 pp of the −0.49%
  data loss: −0.49% + 0.21% ≈ −0.28%, against the measured −0.297%/−0.313% —
  **matched in sign and magnitude per charge** (h− lost slightly more data,
  0.4937% vs 0.4861%, and dropped slightly more, −0.3130% vs −0.2965%). The
  residual ~0.01–0.03 pp is the second-order prior/f_i/redistribution change.
  The local AMPLIFICATION in hot bins (§3) is the same coin: where the data/MC
  in-cone excess is largest, the response-side recovery is smallest relative
  to the data loss.

## 6. Verdict — does the measured change match the impact-study predictions?

**YES, on every load-bearing prediction:**

| Prediction (impact study 2026-10-11) | Measured (this comparison) | Match |
|---|---|---|
| In-grid raw-yield removal −0.4897% (−0.73% in-PS) | 19,024/3,884,097 = −0.4897% (28,582/3,902,480 = −0.7324%), exact | ✅ exact |
| Unfolded effect ≈ −0.30% (damped from −0.49%) | −0.2965%/−0.3130% RAW, −0.2891%/−0.3063% Born | ✅ |
| Damping consistent with response-side losses | +0.19 pp recovery vs 0.204–0.216% body loss, per charge | ✅ |
| Hot bins lose 30–37%; α≥0.2 removal ≥ α<0.1 figures | worst removals 37.54%/34.94% ≥ 36.6%; top-5 in 14.9–37.5% | ✅ |
| Hot bins at low z (electron-adjacent) | all top bins z∈[0.2,0.5), S0/S1 strata | ✅ |
| Hot-bin pT decode pT∈[1.0,1.5) | measured pT∈[4.0,8.0) by the release decode | ⚠️ annotation superseded (non-load-bearing; study self-hedged) |
| f_i shift ≲0.1–0.2 pp response-internal, ≲0.1–0.3 pp data-weighted, plausibly down | −0.004…−0.007 pp / −0.015…−0.024 pp, DOWN in all combos and the S2′ variant | ✅ |
| Post-cone contamination ≈7.2766% of the new vector | 281,250.09/3,865,073 = 7.2767% | ✅ (0.0001 pp) |

## 7. Provenance and limits

- **Cone-base products (validated, study-tree, R9 PROVISIONAL — not installed
  canonical):** RAW and Born releases under
  `H1ChargedSidis_HighQ2/output_v6/studies/cone_alpha_central_build_2026-10-11/production/release_driver/`;
  data vector `data_vector_cone_alpha_central_2026-10-11.json` (in-grid
  3,865,073 = h+ 1,965,700 / h− 1,899,373); S2′-convention cone responses
  `migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.{root,npz}`; f_i
  crosschecks `f_i_crosscheck_cone_*.json/csv` (4 combos); QA sums
  `{central,s2prime}_shards_alpha_qa_sum.json` (174/174 shards with cone
  counters); Phase-F rerun record `qa_tools/check_phase_f_rerun_outputs.json`
  (guard E values exact 697/700/777/845; body-diff values quoted in §5).
- **No-cone references (canonical, read-only):** the S2′ RAW/Born releases
  under `output_v6/central/data_release/` (§Bases).
- **Logs:** `studies/cone_alpha_central_build_2026-10-11/docs/PRODUCTION_LOG_2026-10-11.md`,
  `BORN_LOG_2026-10-11.md`; guard adjudication
  `sidis_highQ2/docs/qa/cone-missrow-guard-adjudication-2026-10-11.md`.
- **Computation:** read-only recomputation on the NAF login node (release
  CSVs, data-vector JSON, crosscheck CSVs, cone S2′ response ROOT via the
  pinned `crosscheck_unfold_methods_v4.derive_response_blocks` engine;
  scratch script `/tmp/opencode/cone_rev3_computation.py`, output retained).
- **Limits:** per-bin comparisons are release-level (245 bins/charge); the
  impact study's per-bin predictions were fine-grid α<0.1 track-level counts,
  so the hot-bin comparison is at band/region level, not bin-identity level.
  The damping decomposition in §5 is first-order (the cone also changes the
  coherent prior and f_i); its residual (~0.01–0.03 pp) is second-order
  redistribution. All cone-side values carry the study-tree PROVISIONAL flag
  until promotion.

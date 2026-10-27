# WP-A2 S3: correction derivation and H3 structure test (derivation split)

Date 2026-10-27. Inputs: S2 readings (`s2_diagnostic_record.md`), S1 record (sha256 `bb95183d…bd23`, R1-R8),
`plug_calib.csv`, even-split payloads. Machine-readable record:
`D/data/restart_2026-10-27/wp-a2/s3_corr/s3_correction_record.json` (its sha256 is frozen in §5 and quoted in the S4
submit logs). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. No odd-split number has been
read.

## 1. Per-hypothesis outcome and correction (D3 order)

| hyp. | outcome | criterion value (even split) | correction (kind: value) | evidence |
|---|---|---|---|---|
| H5 | not supported | EnCor = 1, Thresh = 0.3 GeV, all runs | none (`db_rescale` × 1 = identity) | `s1_db/plug_calib.csv` |
| H1 | supported (response) | excess/deficit move 0.91/4.23 (7071), 0.94/3.67 (7091), ≥ 0.5 required; geometry shift 0.0004 | `cut`: N−1 / Stage-B η_max < 3.3 with `etaMaxClus08` (published 800 MeV / 3.3); `threshold`: fit uses `etaMaxClus04` | `s3_corr/analysis/h1_threshold_choice.json` |
| H6 | not supported | geometry shift 0.0004 < 0.03 | none | `s2_diag/analysis/s2_analysis_even.json` |
| H4 | true-detector | edge worst \|MC/DATA−1\| 0.71-0.73 (7071), 0.89-1.03 (7091), pattern in every cell | `fit_range`: η_max < 3.4 plus one merged bin [3.4, 5) | same |
| H3 | not supported | no variant reaches ≥ 10× in all 8 fits for any alternative; H4 true-detector is supported (R3) | none: 2 templates, no E−p_z or candidate cut in the fit | `s3_corr/analysis/h3_table.{csv,json}` |
| H2 | supported by its criterion (layer/station-specific); correction **excluded** | uniformity chi² p 1e-240 to 1e-10; no run or lumi trend (min p 0.046) | none in the frozen set; remedy-1 q is a systematic leg only (§4) | `s2_diag/analysis/s2_analysis_even.json`; q: `D/data/restart_2026-10-27/wp-a/s2_remedy1/noise_table.json` |

Threshold-choice numbers (H1). The edge residual is the deviance over [3.15, 3.55) in the ALL fine two-template fit,
summed over 7071, 7091 and nd4cat: 6153 with `etaMaxClus08` and 4070 with `etaMaxClus04`, so 0.4 GeV is chosen.
This choice is made on the full range, before the H4 range is applied (D3 order).

## 2. H3 fit table (even split, 1000 toys per fit, p = (count+1)/1001, floor 0.0010)

| variant | alt | max p over the 8 fits | ALL fine D/ndf | ALL coarse D/ndf | ≥ 10× in all 8? |
|---|---|---:|---|---|---|
| (i) baseline | 7071 / 7091 / nd4cat | 0.067 / 0.074 / 0.038 | 3837/358, 4046/358, 3283/358 | 2563/97, 2977/97, 2438/97 | — |
| (ii) + 35 < E−p_z < 70 | 7071 / 7091 / nd4cat | 0.072 / 0.077 / 0.048 | 3889/358, 3914/358, 3215/358 | 2504/96, 2886/96, 2356/96 | no |
| (iii) (ii) + x_IP < 0.04 ∧ M_X > 3 | 7071 / 7091 / nd4cat | 0.438 / 0.474 / 0.431 | 2284/358, 2042/358, 1836/358 | 987/80, 980/80, 959/80 | no |
| (iv) (ii), 3 templates, t ∈ [0.5, 2] | 7071 / 7091 / nd4cat | 0.083 / 0.100 / 0.058 | 3670/357, 3760/357, 3013/357 | 2405/95, 2844/95, 2272/95 | no |

Of the 96 fits, 68 sit at the toy floor. The ALL fits stay at the floor except (iii) 7071 fine ALL (p = 0.0030). The maximum p values come from single
D-cell fits.

- `h3_variants_complete`: 12 (variant, alt) rows, each with 8 fits, PASS.
- `pdiss_bound_respected`: in (iv), t lies in [0.50, 2.00] in all 24 fits, PASS. t sits at the upper bound in 18 of
  them and at the lower bound in 3 (coarse D2); it is interior (0.72-0.77) only in coarse D1.

Gen-x_IP ≥ 0.03 share of the 9012 cut-removed template (purity fact for WP-B/WP-F, not a correction):
(i) 0.605, (ii) 0.603, (iii) 0.434, (iv) = (ii). The source is
`D/data/restart_2026-10-27/wp-a2/s3_corr/analysis/xip_share_9012.json`, the even split of 9012.root. It exceeds the
32.5% of `binning_record.md`, which is quoted inside the WP-C fiducial.

## 3. Frozen definition for G1′ (S4, odd split)

- **N−1 / Stage-B basis** (G1′ (a), (b)): stack C, `etaMaxClus08` < 3.3 (`n1_wpa2.py --eta-cut 3.3`). No noise
  removal.
- **§7.1 fit** (G1′ (c)): cut-removed basis on `etaMaxClus04`; fit range η < 3.4 in 0.05 bins plus one merged bin
  [3.4, 5); two templates; 7071, 7091 and nd4cat; 1000 toys; coarse and fine;
  D0, D1, D2 and ALL.
- Derivation-split context, not a choice: the frozen fit without toys gives D/ndf =
  3137/265 (7071), 2777/265 (7091) and 2724/265 (nd4cat) on the even split.

## 4. D5 admissibility

- `cut`, `threshold` and `fit_range` meet D5 (i)-(iii). H5 and H6 are identities.
- H2 `noise_removal` (WP-A remedy-1): **excluded** by `h1-analysis-critic` (ruling on the escalated stalemate with
  `h1-mc-expert`). It fails D5 (i): the criterion fired on mixed-sign layer and station structure, which a uniform
  event removal cannot represent, and the per-sub-period wording belongs to the time-trend branch, which is null here.
  It fails D5 (ii): q = (f_d − f_MC)/(1 − f_MC) is a data-minus-MC rate measured on the f_fail difference that
  G1′(a)/(b) test, and its kind is not in the D5 list. Remedy-1 may be carried only as a labelled systematic leg for
  G1′(b). It does not enter G1′(a) or (c) or the ruling. This package does not run it.
- `corrections_are_definitions`: PASS for the frozen set. No value comes from the odd split.
- No `user:` item was applied: no Plug-veto abandonment, no x_IP window beyond 0.03, no new MC request.
- Notes for later packages:
  - WP-B: the 0.4 GeV fit branch fixes template normalisations, which WP-B maps onto the 0.8 GeV η_max < 3.3 gap.
    The 0.4 GeV deficit sits in 3.40-3.55, inside the merged bin.
  - WP-F: the forward-edge feature may also sit in the diffractive templates (plan Risks). The 3.15-3.35 excess
    stays inside the fit range; the even-split frozen fit has D/ndf ≈ 10-12.
  - WP-F: in variant (iv), t sits at a bound in 21 of 24 fits.

## 5. Freeze

- Erratum after freeze (code review C1/C2, even split, no effect on the frozen set): the H2 criterion values in the
  frozen JSON (`uniformity chi² p 1e-240..1e-10; min p 0.046`) are first-pass numbers. The corrected values are
  1e-71 to 2.5e-7 (MC error included) and 0.063 (A_pre count as the lumi proxy). The H2 reading is unchanged, and H2
  has no correction in the frozen set. The JSON is not edited, so its sha256 still matches the S4 submit logs.

- `s3_correction_record.json` sha256 at freeze (2026-10-27, before any S4 submission): `4b2b5e269a1f8d07f22d34a24c9416742eba53ebb6da23b85014bcf3bf79d719`

## 6. Sign-off

`h1-analysis-critic` (2026-10-27). Round 1: SIGNED no. The one blocker was remedy-1, which fails D5 (i) and (ii)
and is now excluded. H1 is OK: the literal reading of the R5 window is acceptable, and the c04 choice is a definition
(04 < 08 for every alternative, also on the merged-3.4 fits). Applying H1 to both the N−1 basis and the fit follows
the D3 text. H4 is admissible: the ND-share test is not part of the D3 criterion, and a 12-15% admixture moves the
ratio by at most 0.18. H3 is not supported. Round 2 (the delta only): SIGNED yes.

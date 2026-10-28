# Generator-Level Flat-phi_h Sensitivity Side Study — High-Q2 Charged SIDIS

**Created:** 2026-09-25  
**Scope:** high-Q2 inclusive charged-hadron SIDIS only (`sidis_highQ2/`)  
**Status:** completed diagnostic side study; **not a systematic, not physics-approved, and not central**. No central product was overwritten.

## Overview

This note records a diagnostic of the sensitivity of acceptance and IBU unfolding to the **generator-level**, CM-frame Trento `phi_h` distribution. For each generator, per-generator weights were derived to make the generator-level `phi_h` distribution exactly flat. Responses built from the flattened samples were unfolded with IBU `k=4` against the unchanged nominal data vector and compared with matched unity-weight controls made through the same code path.

This is deliberately different from the July 2026 reco-level data/MC `phi_h` reweight study. That study weighted the matched reco leg at reco `phi_h`. Here the weighting policy is truth coherent:

- gen prior and miss legs use generator `phi_h`;
- a matched reco leg uses its matched generator partner's `phi_h`;
- fakes have weight exactly `1.0`, because a fake has no truth `phi_h`.

The policy keeps the prior, miss, and matched response coherent and preserves response truth conservation.

## Read First

1. `/data/dust/group/h1/klesthen/BaryonAsym/CONTEXT.md`
2. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/context-handoff.md`
3. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/STATE.md`
4. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/RUN_STATUS.md`
5. `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/systematics/phi_h_reweight_pipeline_2026-07-08.md` (related but methodologically distinct reco-level study)
6. This runbook

If a newer high-Q2 handoff disagrees with this note, follow the newer explicit statement and retain this file as the dated record of this completed diagnostic.

## Purpose and Study Definition

Target question:

- How sensitive are acceptance and IBU unfolding to the generator-level Trento `phi_h` model?

Study definition:

- derive exact, yield-preserving generator-level `phi_h` flattening weights independently for DJANGOH14 and RAPGAP31;
- rebuild four responses: `djangoh14_unity`, `djangoh14_flat`, `rapgap31_unity`, and `rapgap31_flat`;
- unfold each response with IBU `k=4` and the identical nominal data vector;
- compare flat versus unity within each generator; use unity DJANGOH14 versus unity RAPGAP31 only as generator-context information;
- keep this raw: no background subtraction, no `C_rad`, and no Born-level correction;
- write only under the isolated study root; do not overwrite central products.

Recorded data luminosity: `L_data = 351000.0 nb^-1`.

## Inputs and Contracts

The current z005 contract was retained throughout:

- `490` published rows: `245` h+ and `245` h-;
- response shape `(491,246)` per charge;
- data-vector length `980`;
- IBU iteration `k=4`.

All four unfolds used the same data vector, SHA-256 `dcde323108b07c04998c467c579f13de4a0cf56c79283fbedcc37dcd82f5e26f`.

## Directory and Artifact Map

Study root:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/`

| Item | Path |
|---|---|
| Builder fork | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/flatphi_genreweight_z005_20260925/build_flatphi_response_shard_v1.py` |
| Pass 1 | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/pass1/pass1_gen_phi_{djangoh14,rapgap31}.npz` and summaries |
| Weights | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/weights/flatphi_weights_{djangoh14,rapgap31}.json` |
| Merged responses | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/pass2/{djangoh14_unity,djangoh14_flat,rapgap31_unity,rapgap31_flat}/merged/response_<variant>.{root,npz}` |
| IBU releases | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/ibu/<variant>/cross_sections.{csv,json}` |
| IBU comparison | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/ibu/comparison/` |
| Validation | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/flatphi_weight_derivation_validation.json`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/response_validation_flatphi.json`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/RESPONSE_VALIDATION_SUMMARY.md`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/ibu_flatphi_validation.json` |
| Reviewed plots | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/plots/flatphi_genreweight_study/flatphi_genreweight_study.pdf` (7 pages; 7 PNG page images; reviewed and approved) |
| Iteration 2 weights | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/weights/flatphi_weights_perbin_{djangoh14,rapgap31}.json`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/weights/flatphi_weights_perbin_halfA_{djangoh14,rapgap31}.json` |
| Iteration 2 responses | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/pass2/{djangoh14_perbin,djangoh14_perbin_halfA,rapgap31_perbin,rapgap31_perbin_halfA}/merged/response_<variant>.{root,npz}` |
| Iteration 2 IBU and decomposition | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/ibu/<variant>/cross_sections.{csv,json}`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/ibu/comparison_perbin/perbin_decomposition.csv` |
| Iteration 2 validation | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/flatphi_perbin_weight_derivation_validation.json`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/ibu_flatphi_perbin_validation.json`; `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/validation/PERBIN_DECOMPOSITION_SUMMARY.md` |
| Iteration 2 reviewed plots | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/plots/cutshift_style_shifts_perbin/flatphi_perbin_cutshift_style_review.pdf` (8 pages; reviewed and approved); `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/plots/phih_truth_pre_post_perbin/phih_truth_pre_post_perbin.pdf` (7 pages; reviewed and approved) |
| Iteration 1 uncertainty-band bundle | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/plots/cutshift_style_shifts/flatphi_cutshift_style_review.pdf` (5 pages) |

## Pass 1 — Generator phi_h Shapes and Weights

Pass 1 ran in Condor cluster `5236798`: `174/174` shards completed, with `0` failures and `0` zero-byte outputs. The inputs were DJANGOH14 (`501` files in `101` shards) and RAPGAP31 (`364` files in `73` shards).

For the `all` group, the generator-level `phi_h` max/min ratios were:

| Generator | h+ | h- |
|---|---:|---:|
| DJANGOH14 | 1.15011543 | 1.14414123 |
| RAPGAP31 | 1.14085594 | 1.13354759 |

The two generator shapes are only slightly different: their maximum absolute normalized-bin difference is `6.40e-4` for h+ and `5.84e-4` for h-.

### Weight definition and closure

Weights used exact flattening with `smoothing: none`. This is deliberate: smoothing would leave residual structure and defeat the test's purpose. Normalization is yield preserving per key, so the diagnostic isolates shape sensitivity.

| Generator | True unclipped weight range | Sparse-fallback bins | Guard hits `[0.25,4.0]` | Worst post-reweight max/min |
|---|---:|---:|---:|---:|
| DJANGOH14 | `[0.82131404, 1.07146324]` | 0 | 0 | 1.0000000000000002 |
| RAPGAP31 | `[0.82640089, 1.07806344]` | 0 | 0 | 1.0000000000000004 |

Thus every generator-level `phi_h` key closes to flatness at floating-point precision, without clipping or sparse-bin fallback.

## Pass 2 — Response Construction and Validation

Four independent response productions completed and drained:

| Variant | Condor cluster | Completion |
|---|---:|---:|
| `djangoh14_unity` | 5239619 | `101/101`, 0 failures, 0 zero-byte |
| `djangoh14_flat` | 5239620 | `101/101`, 0 failures, 0 zero-byte |
| `rapgap31_unity` | 5239621 | `73/73`, 0 failures, 0 zero-byte |
| `rapgap31_flat` | 5239622 | `73/73`, 0 failures, 0 zero-byte |

All response variants have shape `(491,246)` per charge, contain no negative or non-finite response entries, and have zero bins with acceptance above one. The unity controls reproduce their central response counterparts with median acceptance difference `0`.

### Truth conservation and the central prior blocker

For every charge and all four variants, `body_colsum_j + M[490,j]` divided by the stored study prior has ratio min/median/max `1/1/1`, with `0` bins off by more than `1e-6`. The fork is therefore internally truth conserving.

This is important for interpretation: the known central problem in which central `body+miss` disagrees with the stored RAPGAP prior in `245/245` generator bins, with about `+-2.8%` scatter, is a **stored-prior provenance issue**. It does not affect this internally coherent study.

### Guarded pre-existing empty rows

Reco rows `79`, `219`, and `359` have exactly zero MC activity in the relevant charge/generator combinations and therefore a `0/0` fake fraction. The same row indices occur in the central response. They are a pre-existing empty-bin artifact, not a study defect; they were explicitly guarded as `0/0`, not treated as a failure. Any raw response-validator status driven solely by those undefined fake fractions must not be read as a failure of response finiteness, acceptance, or truth conservation.

## Acceptance Sensitivity

Flat/unity acceptance ratios per generator bin are:

| Generator | Min / median / max | Bins moving >1% / >2% / >5% |
|---|---:|---:|
| DJANGOH14 | `0.9956 / 1.0006 / 1.0501` | `12.2% / 4.3% / 0.2%` |
| RAPGAP31 | `0.9929 / 1.0005 / 1.0519` | `11.6% / 4.3% / 0.2%` |

Integrated flat/unity prior ratios confirm yield preservation by construction:

| Generator | h+ | h- |
|---|---:|---:|
| DJANGOH14 | 0.999995 | 0.999993 |
| RAPGAP31 | 0.999994 | 0.999992 |

## IBU k=4 Results

Every unfold produced `490` rows, all finite, with `0` negative cross sections.

| Comparison | Median absolute relative shift | Maximum shift | Integrated h+ | Integrated h- | Integrated total |
|---|---:|---|---:|---:|---:|
| DJANGOH14 flat vs unity | 0.5372% | 5.1196% at h+ bin 209 | +0.00492% | +0.00807% | +0.00647% |
| RAPGAP31 flat vs unity | 0.5364% | 5.8912% at h+ bin 184 | +0.05887% | +0.06068% | +0.05976% |

Context only: the unity-weight DJANGOH14 versus RAPGAP31 integrated difference is h+ `-0.58059%`, h- `-0.63985%`, and total `-0.60985%`. The corresponding unity generator-to-generator median per-bin spread is `1.61%`.

### IBU versus acceptance-only sensitivity

IBU amplifies the median acceptance-only (BBB) sensitivity:

| Generator | BBB median shift | IBU median shift | IBU / BBB |
|---|---:|---:|---:|
| DJANGOH14 | 0.1752% | 0.5372% | 3.07x |
| RAPGAP31 | 0.1572% | 0.5364% | 3.41x |

The methodologically relevant observation is that a median response-level acceptance perturbation of roughly `0.06%` becomes a roughly `0.54%` IBU shift after iteration.

## Interpretation

The generator-level `phi_h` model is a real but sub-dominant lever in this diagnostic: the median per-bin IBU effect is about `0.54%`, roughly one third of the existing `1.61%` unity generator-to-generator median spread. Integrated effects are very small (`0.006%` for DJANGOH14 and `0.060%` for RAPGAP31), well below the approximately `0.61%` generator-to-generator integrated difference.

The effect is strongly localized. Only about `0.2%` of bins move by more than `5%`; the largest excursions occur in high-z or otherwise sparse bins. The approximately threefold IBU amplification is relevant evidence for the open `P-METHOD` IBU-versus-TUnfold decision, but it does not select a nominal method on its own.

No theory-facing conclusion follows here: this is raw/no-background-subtraction/no-`C_rad`/not-Born-level work.

## What Is Safe To Say Now

- This completed, isolated diagnostic shows that exactly flattening the generator-level Trento `phi_h` prior changes the IBU `k=4` result by a median `0.54%` per bin for either generator, with very small integrated changes.
- The response fork is internally truth conserving, and its unity controls reproduce the central response acceptance at median difference zero.
- The local response sensitivity is amplified by IBU by about `3x`; this is relevant input to method-validation discussion under `P-METHOD`.
- No central product was overwritten, and this result is not a central correction or approved systematic.

## Unsafe To Say

- Do **not** call this an approved or budget-ready systematic.
- Do **not** treat it as a physics result, a Born-level statement, or a theory-facing generator comparison.
- Do **not** attribute the central `body+miss` versus stored-prior mismatch to this fork; this study demonstrates that the fork is internally truth conserving and identifies the central issue as stored-prior provenance.
- Do **not** compare this numerically as if it were the July 2026 reco-level data/MC `phi_h` reweight systematic; the weighted leg and the question tested are different.

## Iteration 2 — Published-Bin-Resolved Flattening

### Motivation and design

Iteration 1 used one weight per charge x `(Q2,x)` cell x `phi_h`, integrated over `z` and `pT`. Since the `phi_h` modulation varies with `z` and `pT`, that averaging removes most of the structure and provides a conservative floor. Iteration 2 therefore resolved the flattening at full published-bin granularity: charge x `245` published generator bins x `phi_h`, simultaneously in all four published variables.

The weight keys are `<hplus|hminus>::binNNN` for `NNN = 000..244`, with `<charge>::all` fallback. The recipe remains `w_j = mean(sumw)/sumw_j`, normalized to unit sumw-weighted mean in each `(charge, bin)` group to preserve yield; `smoothing: none`; and a `[0.25,4.0]` clipping guard. Raw counts below `50` use the charge-level `all` weight. Of the `3920` `(bin,phi)` cells per charge, sparse fallback affected DJANGOH14 h+ / h- in `97 / 226` cells and RAPGAP31 h+ / h- in `154 / 337`; there were no empty cells.

| Generator | True unclipped weight range | Clip engaged | Worst post-reweight max/min |
|---|---:|---:|---:|
| DJANGOH14 | `[0.339040, 1.875547]` | 0 | `1.0000000000000004` |
| RAPGAP31 | `[0.342192, 2.159457]` | 0 | `1.0000000000000004` |

These ranges are much wider than the iteration-1 ranges, DJANGOH14 `[0.821,1.071]` and RAPGAP31 `[0.826,1.078]`, directly showing the local structure that cell integration had averaged away.

### Structure revealed below the iteration-1 cell average

Per-published-bin pre-flattening generator `phi_h` max/min has the following min / median / max values:

| Generator | h+ | h- |
|---|---:|---:|
| DJANGOH14 | `1.0300 / 1.4049 / 7.9000` | `1.0279 / 1.4280 / 6.5000` |
| RAPGAP31 | `1.0207 / 1.4751 / 5.0252` | `1.0199 / 1.4851 / 5.4515` |

For comparison, iteration 1's cell-integrated values were only DJANGOH14 h+ / h- `1.1501 / 1.1441` and RAPGAP31 h+ / h- `1.1409 / 1.1335`. Non-uniformity grows with `pT`: the DJANGOH14 h+ median max/min rises from `1.154` at `pT` index 0 to `1.786` at index 4, and RAPGAP31 h+ rises from `1.194` to `1.955`.

### Builder, production, and response validation

The existing fork `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/flatphi_genreweight_z005_20260925/build_flatphi_response_shard_v1.py` gained auto-detected per-bin key lookup, vectorized to a dense `(2,246,16)` array. Regression checks found unity output element-wise identical to the prior validated smoke and the cell-keyed path element-wise identical to the original loop, with maximum `|delta| = 0`.

The truth-leg policy is **unchanged** and remains central to correctness: prior and miss use gen `phi_h`; matched reco uses the matched generator partner's `phi_h`, generator published bin, and generator charge; fakes have weight exactly `1.0`.

| Variant | Condor cluster | Completion |
|---|---:|---:|
| `djangoh14_perbin` | 5240921 | `101/101`, 0 failures, 0 zero-byte |
| `djangoh14_perbin_halfA` | 5240922 | `101/101`, 0 failures, 0 zero-byte |
| `rapgap31_perbin` | 5240923 | `73/73`, 0 failures, 0 zero-byte |
| `rapgap31_perbin_halfA` | 5240924 | `73/73`, 0 failures, 0 zero-byte |

All four iteration-2 responses are `(491,246)`, with no negative or non-finite entries and zero acceptance-above-one bins. Truth conservation is min / median / max `1/1/1`, with zero bins off by more than `1e-6`. Iteration-1 unity responses were reused as the baseline. Reco rows `79`, `219`, and `359` yield guarded `0/0` fake fractions; this is identical in the CENTRAL response and is a pre-existing empty-bin artifact, not a study defect.

### Acceptance and IBU results

Acceptance ratios relative to unity are:

| Variant | Min / median / max | Fraction >1% / >2% / >5% |
|---|---:|---:|
| DJANGOH14 perbin | `0.769498 / 1.002110 / 1.114000` | `30.20% / 17.35% / 7.55%` |
| DJANGOH14 halfA | `0.882404 / 1.001755 / 1.119943` | `28.78% / 17.76% / 6.94%` |
| RAPGAP31 perbin | `0.811581 / 1.000579 / 1.153754` | `23.27% / 14.08% / 6.12%` |
| RAPGAP31 halfA | `0.839731 / 1.000332 / 1.153836` | `24.08% / 13.88% / 6.53%` |

All IBU `k=4` unfolds used the identical nominal data vector SHA-256 stated above and `L = 351000.0 nb^-1`; each has `490` rows, all finite, with zero negative values. Integrated gen-prior ratios relative to unity are DJANGOH14 perbin `1.00000000` for both charges; DJANGOH14 halfA `1.00361981` (h+) and `1.00374375` (h-), exceeding `0.1%` because half-derived weights were applied to the full sample; RAPGAP31 perbin `1.00000000` for both charges; and RAPGAP31 halfA `1.00020540` (h+) and `1.00007860` (h-).

## Iteration 2 Headline — Per-Bin Flattening Is Noise-Dominated

The half-sample control `S_halfA`, which by construction contains no real model variation, nearly reproduces the entire per-bin shift. The median `|S_perbin| / |S_halfA| / |S_cell|` is DJANGOH14 `1.0183% / 1.0011% / 0.5372%` and RAPGAP31 `0.9589% / 0.9737% / 0.5364%`. **For RAPGAP31, the noise floor exceeds the signal.** Bins with `|S_perbin| < |S_halfA|` are DJANGOH14 `232/490 (47.35%)` and RAPGAP31 `244/490 (49.80%)`.

| Generator | Noise-subtracted median true sensitivity, raw | sqrt(2)-scaled |
|---|---:|---:|
| DJANGOH14 | `0.0202%` | `0.7021%` |
| RAPGAP31 | `0.0037%` | `0.6301%` |

The apparent rise from the iteration-1 cell level of about `0.54%` to about `1.0%` per bin is therefore largely statistical fluctuation from finely binned weights, **not** newly revealed generator-model dependence. Half-sample weights are noisier than full-sample weights by roughly `sqrt(2)`: unscaled subtraction is the conservative estimate, while sqrt(2)-scaled subtraction is the less-conservative bracket. The true sensitivity is consequently bracketed by `0.02%` to `0.70%` for DJANGOH14 and `0.004%` to `0.63%` for RAPGAP31.

Integrated h+ / h- / total shifts are DJANGOH14 perbin `-0.1614% / -0.1613% / -0.1613%`, halfA `-0.1608% / -0.1609% / -0.1609%`, and cell `+0.0049% / +0.0081% / +0.0065%`; RAPGAP31 perbin `-0.0766% / -0.0650% / -0.0709%`, halfA `-0.0751% / -0.0642% / -0.0697%`, and cell `+0.0589% / +0.0607% / +0.0598%`.

The significance comparison likewise makes signal and noise nearly indistinguishable: DJANGOH14 `S_perbin` is above statistical uncertainty in `50/490 (10.20%)` bins and above `total_with_tracking_rel` in `4/490 (0.82%)`, versus `48/490` and `4/490` for `S_halfA`; RAPGAP31 `S_perbin` is `58/490 (11.84%)` and `7/490 (1.43%)`, versus `57/490` and `7/490` for `S_halfA`. Median `|S_perbin|` by `pT` index 0 through 4 is DJANGOH14 `0.7566% / 0.5172% / 0.7170% / 1.3094% / 2.2515%` and RAPGAP31 `1.0568% / 0.7170% / 0.7661% / 1.1166% / 1.8413%`.

### Iteration 2 conclusion

Iteration 1's cell-level median result of about `0.54%`, about one third of the `1.61%` generator-to-generator spread, remains the more defensible sensitivity estimate. Iteration 2 finds no hidden large `z`/`pT`-dependent `phi_h` sensitivity: its additional apparent shift is consistent with weight noise. Together, the two iterations bracket the answer and support the statement that the measurement is not strongly sensitive to whatever produces the `phi_h` distribution in DJANGOH14 or RAPGAP31.

The existing caveats remain unchanged: this is raw, without background subtraction or `C_rad`, not Born level, diagnostic only, not a systematic, not physics-approved, and no central product was overwritten.

## Reproduction Runbook

This is a completed-run record, not authorization to rerun central production. Re-check the current handoff and blockers before any reproduction.

1. Confirm the study root is isolated under `output_v6/studies/flatphi_genreweight_z005_20260925/` and that no central path is an output target.
2. Run Pass 1 on Condor to produce generator-level CM-frame Trento `phi_h` histograms for DJANGOH14 and RAPGAP31; aggregate and validate `pass1_gen_phi_{djangoh14,rapgap31}.npz`.
3. Derive yield-preserving, exact-flat weights independently by generator and validate closure, sparse fallback, and guard clipping.
4. Build both unity and flat Pass 2 responses for each generator. Apply truth-leg weights to prior, miss, and matched reco using the matched truth partner; retain fake weight `1.0`.
5. Merge and validate each response: shape, finite/nonnegative entries, acceptance, central-control agreement, truth conservation, and guarded empty reco rows.
6. Run IBU `k=4` for all four responses against the exact same nominal data vector; validate rows, finiteness, non-negativity, alignment, and central overwrite safety.
7. Package the flat/unity and unity-generator comparison tables and reviewed plots under the study root only.

The Condor resource stanza used for the multi-file passes was:

```condor
request_cpus = 1
request_memory = 2000MB
+RequestRuntime = 10800
getenv = True
```

The login-node policy was respected: interactive work was limited to one-file checks within five minutes, and all multi-file processing was submitted to Condor.

## Minimal Acceptance Checklist for Future Audit

- Pass 1 completion is `174/174`, with no failures or zero-byte outputs.
- Each Pass 2 variant has its expected shard count and no failures or zero-byte outputs.
- Every response is `(491,246)` per charge, finite/nonnegative, with no acceptance above one.
- Truth conservation is unity within floating precision for every variant.
- Empty fake-fraction rows are recognized as the pre-existing guarded `0/0` rows, not a new response failure.
- All four unfolds use the stated identical data-vector hash and yield `490` finite, non-negative rows.
- Outputs remain exclusively under the study root; central overwrite safety is confirmed.
- This diagnostic-only and correction-status caveat remains attached to any downstream mention.

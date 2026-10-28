# RAD reconstructed-source bias test — 2026-09-25

## Scope and convention

Read-only test of whether the retained DJANGOH14 RAD generator record is conditioned by an upstream reconstruction/acceptance requirement.  The test uses the already-produced 2,953-file cached weighted histograms under `plots/radiative/crad_charge_asymmetry_2026-09-25/cache/`: RAD e+/e−, OLD NONRAD e+/e−, and NEW NONRAD e+/e−.  No ROOT reprocessing was performed.  All values below use the **full** truth selection used for the cache:

`Q2_es_gen>150`, `Q2_es_gen<20000`, `0.1<Y_es_gen<0.7`, `GenEmpz>45`, `xFGen<0`, pi/K/p, `IsGtr!=0`, `Mother2==-1`, and `IsV0DaughterGen==0`.

For each variable, the statistic is

`R(v) = [H_RAD(v)/sum H_RAD] / [H_JOINED_NONRAD(v)/sum H_JOINED_NONRAD]`,

where JOINED NONRAD is the raw OLD+NEW histogram sum over both beam charges.  It is therefore a shape-only diagnostic, not a cross-section ratio and not an approved C_rad construction.  `pT_h` is the photon-axis gamma*p-CM variable stored by the RC ntuples.  `Q2_es_gen` and `Y_es_gen` are the stored eSigma generator estimators.

## Verdict

**CONFIRMED: the RAD generator-level population has a detector-acceptance-like imprint.**  `GenEta_lab` has a structured one-sided suppression after the central region: 1.0142 at `|eta|<1`, 0.9985 at `1<|eta|<2`, 0.9631 at `2<|eta|<3`, and 0.9331 at `|eta|>3`.  In the positive (proton-going) direction the fall begins at `eta≈2.2`: 0.9877 (2.0–2.2), 0.9712 (2.2–2.4), 0.9447 (2.4–2.6), 0.9172 (2.6–2.8), and 0.9070 (2.8–3.0); it stays about 0.92–0.95 through eta=5.  The negative tail instead remains above unity (1.03–1.04 for eta<−2).  This is not a flat RAD/NONRAD truth-shape ratio and is consistent with acceptance conditioning on the proton-going side.

This establishes a retained-RAD generator-population bias but does **not** identify the upstream retention predicate.  Existing provenance proves OO HAT/MODS `.DST` input, while raw GEN/BOS counts and official trigger/reconstruction retention records are absent.  Thus the physical observation is a truth-shape imprint, not a proof of a particular H1 reconstruction cut.

## Shape results

The machine-readable numbers are in `data/radiative/rad_reco_bias_test_20260925/rad_reco_bias_summary.csv`.

| Variable | Region | R = RAD / joined-NONRAD, unit-area |
|---|---:|---:|
| GenEta_lab | `|eta|<1` | 1.0142 |
| GenEta_lab | `1<|eta|<2` | 0.9985 |
| GenEta_lab | `2<|eta|<3` | 0.9631 |
| GenEta_lab | `|eta|>3` | 0.9331 |
| GenPt | 0–0.5 GeV | 1.0011 |
| GenPt | 0.5–1 GeV | 1.0014 |
| GenP | 0–1.25 GeV | 1.0111 |
| GenP | 1.25–2.5 GeV | 0.9962 |
| Y_es_gen | 0.1–0.2 | 0.9784 |
| Y_es_gen | 0.2–0.4 | 0.9981 |
| Y_es_gen | 0.4–0.6 | 1.0167 |
| Y_es_gen | 0.6–0.7 | 1.0299 |
| Q2_es_gen | 150–300 GeV2 | 1.0059 |
| Q2_es_gen | 300–700 GeV2 | 0.9986 |
| Q2_es_gen | 700–1500 GeV2 | 0.9849 |
| Q2_es_gen | 1500–5000 GeV2 | 0.9561 |
| Q2_es_gen | 5000–20000 GeV2 | 0.9032 |

The predicted low-momentum reconstruction signature is **not** present: the lowest `GenPt` bins are 1.0011 and 1.0014, and the lowest `GenP` bin is 1.0111.  The observed `GenPt` and `GenP` depletion is in the high-momentum tail, not at low momentum.  Consequently, the evidence supports a non-uniform upstream conditioning but does not support low-track-momentum inefficiency as its mechanism.

`Y_es_gen` shows a smooth 0.9784 to 1.0299 rise across the selected interval.  `Q2_es_gen` is flat only in the low-Q2 interval relevant to the generation-cut boundary (150–300: 1.0059; 300–700: 0.9986), but then falls steadily: 0.9849 at 700–1500, 0.9561 at 1500–5000, and 0.9032 at 5000–20000 GeV2.  It is therefore **not flat above 150 GeV2 as a whole**.

## Q2GT60 versus Q2GT100 confound

The immediate Q2GT60/Q2GT100 threshold does not explain the signature immediately above the analysis cut: `R_Q2=1.0059` in 150–300 GeV2 and 0.9986 in 300–700 GeV2.  The large high-Q2 shape deficit shows that another kinematic/domain or retained-population mismatch is active.  The Q2GT60/Q2GT100 samples nevertheless remain an invalid unmatched generated-domain pair, so neither the high-Q2 suppression nor the acceptance-like eta effect can be uniquely attributed to reconstruction conditioning using this pair alone.

`Q2_es_gen` is ISR-dependent: its estimator uses the ISR-corrected effective lepton-beam energy, whereas true Q2 does not.  RAD ISR migration through the 150 GeV2 boundary can therefore differ from NONRAD even if their true-Q2 domains were matched.  The near-boundary flatness is reassuring against a large threshold step but is not a removal of this migration confound.

## Relation to the anomalous C_rad shape

The unit-area shape ratios have the opposite z/pT orientation from a low-z/low-pT tracking-loss explanation:

| Axis bin | RAD/joined-NONRAD shape ratio | reported run3 median C_rad |
|---|---:|---:|
| z 0–0.05 | 1.0100 | 0.763 |
| z 0.05–0.10 | 0.9904 | — |
| z 0.10–0.20 | 0.9789 | — |
| z 0.20–0.30 | 0.9645 | — |
| z 0.30–0.50 | 0.9468 | — |
| z 0.50–0.70 | 0.9212 | — |
| z 0.70–1.00 | 0.8791 | 1.271 |
| pT_h 0–0.5 GeV | 0.9916 | 1.243 |
| pT_h 0.5–1 GeV | 1.0023 | — |
| pT_h 1–2 GeV | 1.0221 | — |
| pT_h 2–4 GeV | 1.0396 | — |
| pT_h 4–8 GeV | 1.0526 | 1.046 |

The z shape is qualitatively aligned with the reported C_rad rise: RAD becomes relatively depleted at high z, so `sigma_nonrad/sigma_rad` would rise with z.  The pT shape is also aligned with the reported C_rad fall: RAD becomes relatively enhanced at high pT, so `sigma_nonrad/sigma_rad` would fall with pT.  However, the proposed *low-z soft-track loss* sign is wrong: at z=0–0.05 the RAD shape ratio is 1.0100, not below one; at pT_h=0–0.5 it is 0.9916, not a large RAD depletion.  The C_rad anomaly can therefore be reproduced only by the observed broad RAD population/domain distortion (high-z depletion plus high-pT enhancement), **not** by a simple low-momentum tracking-acceptance mechanism.

## Run3 RAD-only, charge-dependent sumw inflation

**Identified mechanism: the run3 production did not execute the present frozen producer source.**  The run3 wrapper invoked `produce_crad_truth_v2.py` at 2026-09-23 22:08–23:57 UTC with `--allow-provisional-exposure-map`, but the current source has no such argparse option.  Its file modification time is 2026-09-24 16:45 local, after run3.  Thus the source now at the invoked path is a post-run repair, not the code that generated the CSV, and cannot be used to claim a reset/duplicate/charge-branch mechanism for run3.

The available numerical evidence isolates the obsolete run3 code-path defect: run3 RAD h+/h− sumw ratios were 1.274166/1.2771, while a direct RAD `rad_em_0000.root` read under the full selection with `IsGtr!=0` gives 1.0153575 and the full fresh cache gives 1.0156854 (RAD) versus 1.0161–1.0163 (NONRAD).  Dropping `IsGtr!=0` in the same single file gives 1.2656606, which reproduces the direction and scale of the run3 RAD-only h+ excess.  `Mother2==-1` is a no-op in that file.  Therefore the run3 inflation is an **obsolete RAD accumulation/selection path that omitted the required IsGtr source-pool gate (or equivalently accumulated pre-IsGtr RAD candidates)**.  This admits a charge-asymmetric STR population (especially positive protons), yielding the observed RAD-only, charge-dependent inflation.

The present source prevents that defect: `consume()` applies `IsGtr!=0` before charge accumulation to every sample, `collect()` constructs a fresh `empty()` accumulator for each `(sample, beam)`, and both `GenCharge` and `chargeSign` are identical in the inspected RAD file.  The frozen 731/642 file counts also exclude a simple inventory duplication, and the e+/e− groups are accumulated independently before beam mixing.  The precise historical code revision is not retained, so the exact omitted conditional cannot be recovered beyond this demonstrated source-pool mismatch.

## Status and blockers

1. The retained RAD truth population is not suitable as an unbiased generator-level denominator for an approved C_rad while its upstream conditioning and the Q2GT60/Q2GT100 generated-domain mismatch remain unresolved.
2. The test does not establish the exact upstream predicate because authoritative raw generated-event counts and official DST production/trigger logs are absent.
3. The high-Q2 `Q2_es_gen` mismatch and ISR-dependent boundary migration require a matched true-Q2 RAD/NONRAD control before separating kinematic-domain effects from reconstruction conditioning.
4. C_rad remains candidate-only and unapplied.  No absolute normalization, cross section, or central release product was changed.

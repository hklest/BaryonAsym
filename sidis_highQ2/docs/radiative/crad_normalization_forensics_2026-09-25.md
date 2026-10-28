# Gen-only DJANGOH `C_rad` normalization forensics — 2026-09-25

## Scope and disposition

Read-only investigation.  No central product was modified and `C_rad_applied=false` remains mandatory.

## Decisive normalization finding

The retained RAD and OLD/NEW NONRAD samples do **not** have a common generated phase space:

| side | generator ancestry / campaign cut | SIGMT [pb] |
|---|---|---:|
| RAD 8955–8962 | `RAD.Q2GT60.CTEQ6L` | 15040.275–15088.075 |
| OLD NONRAD e+ | `NORAD.Q2GT100.CTEQ6L` | 7287.9104 |
| OLD NONRAD e− | `NORAD.Q2GT100.CTEQ6L` | 7323.9400 |
| NEW NONRAD e+ | `NORAD.Q2GT100.CTEQ6L` | 7287.9104 |
| NEW NONRAD e− | `NORAD.Q2GT100.CTEQ6L` | 7323.9400 |

Thus the `SIGMT` values are not cross sections for the same Q2-generated domain.  The e+ ratio is

`15040.275 / 7287.9104 = 2.0637294059`.

It is in the direction that enlarges the RAD cross-section proxy and depresses `C_rad`.  However, treating this factor alone as a correction is invalid: it produces h+ `1.3971882` and h− `1.7561438`, not `1.05–1.12`.  The phase-space mismatch is therefore a decisive normalization-invalidity, but not a complete numerical repair.

## Working candidate versus broken run3

| item | 2026-09-21 working candidate | truth-v2 run3 broken candidate |
|---|---|---|
| integrated `C_rad` | h+ `1.1155728783`, h− `1.1155253915` | h+ `0.6770210394`, h− `0.8509564022` |
| RAD exposure | e+ `16206393.0`, e− `13534017.0 nb^-1` | identical exact oomclumi values |
| NONRAD exposure | e+ `15439571.1325`, e− `15311385.4287 nb^-1` | e+ `14637492.6760`, e− `14541275.2122 nb^-1` |
| NONRAD exposure difference | — | run3 is lower by `5.48%` (e+) and `5.30%` (e−); this would lower, not restore, `C_rad` only by about 5% |
| weights | `weight1*weight2`; `weight2=1`; weighted sum and sumw2 | identical product and weighted sum/sumw2 |
| numerator pooling | OLD+NEW raw sums, divided once by charge-specific combined exposure | identical pooling pattern |
| truth selection | v1: `150<=Q2sGen<20000`, `0.1<YsGen<0.7`, `35<GenEmpz<70`, pi/K/p, `IsGtr!=0`, `Mother2==-1`, `ParticleSource!=4` where available, `xFGen<0`, `|eta_lab|<4` | v2: `150<Q2_es_gen<20000`, `0.1<Y_es_gen<0.7`, `GenEmpz>45` (no upper bound), pi/K/p, `IsGtr!=0`, `Mother2==-1`, `IsV0DaughterGen==0`, `xFGen<0`, gamma*p photon-axis `pT_h`; no `ParticleSource` veto or eta cut |

The candidate/run3 difference is therefore not an ordinary common exposure or weight-product change.  It is an invalid RAD/NONRAD generated-domain pairing plus a non-controlled v1-to-v2 truth-definition/interface change.

## RAD h+ excess

Run3 integrated proxies are h+ `16.3360690`, h− `12.7918590`, h+/h− `1.2771`; NONRAD is h+ `11.0598624`, h− `10.8853143`, h+/h− `1.0160`.  The asymmetry ratio is `1.2570`, matching the required h+/h− repair-factor ratio `1.2569`.

| grouping | RAD h+/h− proxy ratio | h+ mean `C_rad` | h− mean `C_rad` |
|---|---:|---:|---:|
| z 0–0.05 | 1.1826 | 0.7044 | 0.8332 |
| z 0.05–0.10 | 1.8008 | 0.7530 | 0.9916 |
| z 0.10–0.20 | 1.5471 | 0.9150 | 1.0801 |
| z 0.20–0.30 | 1.1736 | 1.0626 | 1.1418 |
| z 0.30–0.50 | 1.1684 | 1.1559 | 1.1784 |
| z 0.50–0.70 | 1.3013 | 1.2109 | 1.2238 |
| z 0.70–1.00 | 1.4998 | 1.2803 | 1.2758 |
| pT 0–0.5 | 1.1183 | 1.1917 | 1.2065 |
| pT 0.5–1 | 1.3891 | 1.0933 | 1.1616 |
| pT 1–2 | 1.7565 | 0.9493 | 1.0841 |
| pT 2–4 | 1.9106 | 0.9024 | 1.0351 |
| pT 4–8 | 1.7494 | 0.9220 | 1.0303 |

The excess is concentrated at low z and pT above 1 GeV, especially pT 2–4 GeV.  It tracks the reported `C_rad` trend: greater RAD h+ excess corresponds to lower h+ correction.  The separately established pT>8 tail is outside the frozen displayed bins, but validates a RAD-side hadron/interface problem; it cannot directly account for a 0–8-bin integrated result.

## Event-level exposure versus track-level numerator

Both pipelines use event-level exposure and hadron-row `sum(weight1*weight2)` numerator.  This is legitimate only when the selected per-event charged-hadron multiplicity is matched between RAD and NONRAD.  It is not matched.

| sample | h+ selected weighted tracks / weighted event | h− selected weighted tracks / weighted event |
|---|---:|---:|
| OLD NONRAD e+ | 1.510273 | 1.487261 |
| OLD NONRAD e− | 1.518563 | 1.493700 |
| NEW NONRAD e+ | 1.512084 | 1.488662 |
| NEW NONRAD e− | 1.518030 | 1.492934 |
| RAD e+ | 1.086459 | 0.849123 |
| RAD e− | 1.082506 | 0.849580 |

Data-mixture values are NONRAD h+ `1.514140`, h− `1.490246`; RAD h+ `1.084657`, h− `0.849332`.  The RAD/NONRAD multiplicity ratios are h+ `0.716352` and h− `0.569927`; their ratio is `1.256925`, again the charge-anomaly ratio.  Multiplicity-normalizing the current proxies gives h+ `0.4849852`, h− `0.4849833`: equality is restored but the result is not physical.  This proves that the mismatch is a selected-hadron population/domain problem, not a valid correction prescription.

## Integrated-treatment arithmetic

| treatment | h+ | h− | in `1.05–1.12`? |
|---|---:|---:|---|
| run3 as stored | 0.6770210 | 0.8509564 | no / no |
| RAD proxy × `7287.9104/15040.275 = 0.48455965` | 1.3971882 | 1.7561438 | no / no |
| shape-only, normalize each charge by its own total selected sumw | 1.0000000 | 1.0000000 | no / no |
| event/track multiplicity corrected: `C_run3 × m_RAD/m_NONRAD` | 0.4849852 | 0.4849833 | no / no |
| working 2026-09-21 v1-selection candidate | 1.1155729 | 1.1155254 | yes / yes |

No algebraic treatment of the run3 primitives is an approved repair.  The only value in range is the historical candidate, whose pairing/selection must be independently reconstructed before use.

## History result

The prior physical-range result is documented in `docs/radiative/SESSION_HANDOFF_crad_resolution_2026-09-21.md` (2026-09-21): h+ `1.1155728783`, h− `1.1155253915`.  The v1 baseline `1.055329789/1.055413655` is documented in `docs/status/radiative-corrections.md` and `docs/ANALYSIS_WORKFLOW_ROADMAP.md`, but `docs/radiative/norad_supplement_reconciliation_2026-09-11.md` itself later labels its old `3,560,233 nb^-1` NONRAD aggregate invalid.  No prior document was found that solves the present RAD Q2GT60 versus NONRAD Q2GT100 pairing and v2 selection/interface mismatch.

## Required blockers

1. Build a matched RAD/NONRAD generated-domain pair; do not use `SIGMT` across Q2GT60/Q2GT100 domains.
2. Run the controlled same-input v1/v2 selection closure, including RAD and NONRAD cutflows and gamma*p pT convention.
3. Resolve the RAD h+ population/interface pathology before any `C_rad` reconstruction.
4. Reconcile OLD/NEW NONRAD exposure provenance and bin-local compatibility.
5. Keep central release raw; no correction application.

## Machine-readable artifacts

- `data/radiative/crad_normalization_forensics_20260925/normalization_arithmetic.csv` (9 lines; preserved)
- `data/radiative/crad_normalization_forensics_20260925/recomputed_integrated_crad_candidates.csv` (9 lines; preserved)
- `data/radiative/crad_normalization_forensics_20260925/rad_sigmt_phase_space.csv` (13 lines)
- `data/radiative/crad_normalization_forensics_20260925/rad_side_recomputations.csv` (37 lines)

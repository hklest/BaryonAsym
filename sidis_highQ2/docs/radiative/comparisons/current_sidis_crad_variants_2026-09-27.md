# Current high-Q2 SIDIS `C_rad` variants — comparison framework (2026-09-27)

**Scope and status.** Read-only comparison of existing diagnostic tables for the high-Q2 charged-hadron SIDIS v6/z005 analysis.  All entries use the cross-section convention

```text
C_rad = sigma_nonrad / sigma_rad,
 sigma = sum(weight1*weight2) / L_MC.
```

`L_data` is not part of this ratio.  **No correction is applied** (`c_rad_applied=false` throughout), and no pending result is used below.  The central release remains raw/not Born level.

## Executive comparison

| Variant | Product / status | Beam treatment | `E-pz` cut | Published-bin compatibility | Finite bins | Integrated `C_rad` (h+, h-) | Median `C_rad` (h+, h-) | Relative-stat median / p95 / max (h+, h-) |
|---|---|---|---|---|---:|---:|---:|---|
| Previous complete-2953 stored-observable table | `britzger_eminus.../complete_2953`; diagnostic only | **e- only** (despite RAD and NONRAD e+ partial columns being retained) | none | no: stored `zGen`, stored nominal-beam `xFGen,pT_h`; old phase-space/bin contract | 471/490 | 1.10590, 1.10585 | 1.15548, 1.14928 | 0.629% / 35.73% / 141.42%; 0.620% / 34.72% / 122.47% |
| Historical V0 / `IsGtr` candidate | `crad_v0_isgtr_20260925`; current historical candidate, not release compatible | e+/e- mixed, `f_ep=0.5440407` | `GenEmpz>45` (no upper bound) | no: `xF<0`, no eta cut, frozen truth-v2 cells/observables, stored-ISR observable issue | 490/490 | 1.14063, 1.14017 | 1.15527, 1.15747 | 0.417% / 2.091% / 4.299%; 0.430% / 2.842% / 5.768% |
| **Pending v6-exact target** | must be rebuilt from v6-exact truth quantities; **no numerical result used** | required e+/e- mixture must be declared with the exact retained exposures | requested `GenEmpz>45`; release-contract baseline instead specifies strict `35<GenEmpz<70` | target: release cells, eSigma DIS variables, CM `pT_h`, invariant `z`, no truth eta/xF cut, response-equivalent V0 veto | unknown until production | not available | not available | not available |

**How the completed figures were calculated.** Previous-table values above were recomputed from its finite e- rows only: `sum(eminus_nonrad_sigma)/sum(eminus_rad_sigma)` by charge; quantiles are finite-row empirical quantiles. V0 values are recomputed from mixed proxy sums and agree with its reported summary. Thus the two completed rows are not a common observable, common phase-space, or common beam-mixture comparison.

## Definitions and cuts

| Feature | Previous complete-2953 stored-observable table | Historical V0 / `IsGtr` | Required v6-exact target |
|---|---|---|---|
| DIS variables | `150<=Q2_es_gen<20000`, `0.1<Y_es_gen<0.7` | `150<Q2_es_gen<20000`, `0.1<Y_es_gen<0.7` | `150<=Q2_es_gen<20000`, `0.1<Y_es_gen<0.7`; use eSigma quantities consistently for cut and cells |
| `GenEmpz` | **not applied** | `>45`, no upper bound | user-labelled `>45` target; must resolve/document conflict with release-fiducial contract `35<GenEmpz<70` before production |
| Truth hadrons | finite; pi/K/p; nonzero charge; `IsGtr==1`; `Mother2==-1`; V0-daughter veto | finite; pi/K/p; `IsGtr!=0`; `Mother2==-1`; V0-daughter veto | pi/K/p; signed charge; `IsGtr==1`; `Mother2==-1`; response-equivalent V0/decay veto |
| Additional truth cuts | `xFGen<0`, `|GenEta_lab|<4` | `xFGen<0`; no eta cut | **no** truth `xF` or eta cut; no reco alpha/theta/photon cut |
| `z,pT_h` and bins | stored observables, including known nominal-beam ISR issue; not release exact | frozen truth-v2 gamma*p-CM convention but retained stored-ISR issue; old/frozen cells | `z=P·h/P·q`; exact W-rest-frame, photon-axis `pT_h`; exact v6 7 `(Q2_es,x_es)` cells × 7 `z` × 5 `pT_h` |
| Exposure/provenance | supplied diagnostic aggregate; RAD labelled diagnostic and OLD/NEW NONRAD not publication-grade | RAD exact oomclumi verified; OLD/NEW GenDIS-event diagnostic | exact paired retained manifests/exposures must be recorded; publication-grade NONRAD provenance remains a gate |

## Already-quantified effects

### `GenEmpz` threshold dependence (historical V0 selection)

Under otherwise fixed V0/`IsGtr` selection, the completed scan gives:

| Threshold [GeV] | h+ integrated | h- integrated | change from no cut (h+, h-) | RAD retained | NONRAD retained |
|---:|---:|---:|---:|---:|---:|
| none | 1.10597 | 1.10581 | — | 1.000000 | 1.000000 |
| 35 | 1.11830 | 1.11795 | +1.12%, +1.10% | 0.989056 | 1.000000 |
| 40 | 1.12640 | 1.12599 | +1.85%, +1.83% | 0.981971 | 1.000000 |
| **45** | **1.14070** | **1.14024** | **+3.14%, +3.11%** | **0.969683** | **1.000000** |
| 50 | 1.17160 | 1.17105 | +5.94%, +5.90% | 0.944136 | 1.000000 |

The completed scan establishes a monotonic, non-negligible selection effect: through 50 GeV, the threshold removes RAD yield only. It does **not** establish the v6-exact `>45` result because the observable, binning, truth phase space, beam mixture, and exposure choices differ.

### What a v6-exact recomputation can change

The direction/magnitude is not numerically predictable from a single completed table, because several changes are coupled. Evidence already available supports the following bounded statements:

1. **Do not infer v6-exact values from V0.** V0 has `xF<0`, `GenEmpz>45` without upper bound, a different beam mixture, and historical stored observables. v6 exact removes truth `xF`/eta cuts, uses release bins and recomputed `z,pT_h`, and requires validated V0 ancestry equivalence.
2. **Observable construction is expected primarily to change shape, not necessarily normalization.** In the completed diagnostic re-finalization, alternative photon definitions changed integrated values by less than 0.007% while modifying the z/pT trend. That evidence is diagnostic only and is not a substitute for v6-exact calculation.
3. **Generated-domain matching is small in available controls:** +0.0674% (h+) and +0.0692% (h-) for the `E-pz>45` nominal diagnostic. It cannot explain a large inter-variant shift.
4. **The `E-pz` decision is material.** In the release-contract diagnostic, nominal integrated values were 1.09995/1.09990 with no `E-pz` cut, 1.11295/1.11237 for `35<E-pz<70`, and 1.13760/1.13667 for `E-pz>45`. These diagnostic values use their own release-contract re-finalization; they are included only to quantify cut sensitivity, not to fill the pending v6-exact row.
5. **Statistics should not be the limiting difference in the V0 candidate** (all 490 finite; rel-stat maxima 4.30%/5.77%), whereas the no-`E-pz` stored-observable table is tail-limited (19 non-finite rows and p95 relative statistics about 35%).

## Provisional conclusions

- The only sound common formula target is `sigma_nonrad/sigma_rad`, with identical RAD/NONRAD truth selection and cross-section proxy. It is not a multiplicity ratio and must not be inverted.
- The historical V0 result demonstrates that retaining `IsGtr` is mandatory: removing it is a known GEANT-STR contamination defect. Its 1.1406/1.1402 integrated values are nevertheless **historical candidate diagnostics**, not a release correction.
- `GenEmpz>45` is expected to raise `C_rad` relative to no cut by about 3.1% in the otherwise historical V0 scan, because it preferentially removes radiative events. That is the strongest presently computed cut effect, but it is not transferable as the exact v6 result.
- The previous complete-2953 table is useful only as a historical e--only, stored-observable/no-`E-pz` reference. Its finite-tail statistics and lack of release compatibility prevent direct binwise comparison or application.

## Strict blockers before using the pending target

1. **Definition conflict:** reconcile the user-labelled pending `GenEmpz>45` e- target with the release-fiducial contract's strict `35<GenEmpz<70` target. Do not silently substitute either.
2. Recompute from v6-exact truth quantities and exact release bin map; do not reuse stored `zGen`, `pT_h`, or `xFGen` values affected by the historical nominal-beam ISR construction.
3. Validate a response-equivalent V0/decay provenance veto and exact RAD/NONRAD input pairing.
4. Close/label NONRAD exposure provenance and verify OLD/NEW bin-local pooling compatibility.
5. Produce y-resolved or event-level closure controls, adopt the sparse-bin policy, and retain `c_rad_applied=false` pending physics/release review.

## Source paths (strict)

- Previous no-`GenEmpz`, stored-observable complete-2953 table/summary: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/britzger_eminus_crad_xf_y_eta_q2_2026-09-26/complete_2953/`
- Historical V0/`IsGtr`, `GenEmpz>45` table/summary: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_v0_isgtr_20260925/results/`
- Completed threshold scan and V0 status: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/SESSION_HANDOFF_crad_2026-09-25.md`
- Release-compatible target contract: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/crad_release_fiducial_contract_2026-09-26.md`
- Existing diagnostic `E-pz` variants (not pending target results): `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/crad_empz45_nominal_diagnostic_2026-09-27.md`

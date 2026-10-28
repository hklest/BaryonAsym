# DjBkgd `fLumi` unit resolution — read-only forensic finding

**Scope:** high-Q² inclusive charged-hadron SIDIS v6/z005, DJANGOH14 low-Q² leakage template. **No analysis product was modified.**

## Definitive finding

`H1SteerTree::fLumi` is in **pb^-1**, not nb^-1.

* Declaration/comment: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Skeleton/H1SteerTree.h:145` calls it the luminosity of the files.
* Explicit unit comment: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Skeleton/H1SteerTree.C:277-287` prints `fFileLumis` as `Lumi(pb-1)`; the same class documentation at lines 63-66 defines `fLumi` as the luminosity of the files.
* The framework data luminosity is explicitly converted to pb^-1 at `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Analysis/H1Analysis.C:1012`.
* Consumption: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Analysis/H1AnalysisChain.C:643-654` supplies `SteerTree->GetLumi()` to `H1CalcWeight::SetMCLumi`. When enabled, `/afs/.../H1Calculator/H1CalcWeight.C:324-339` multiplies each chain/event by `L_data/L_MC`. Thus ordinary `fLumi` is the exposure of the **entire H1SteerTree chain**, not each physical file. It is neither averaged nor automatically summed across chains. Only the separate `fUseEventMCLumi/fFileLumis` path sums eligible phase-space exposures event-by-event (`H1CalcWeight.C:365-421`).

## Britzger chain reconciliation

The EventShapes steering values must therefore be read in pb^-1. The requested calculations below compare only like units.

| catalog | documented full-catalog oomclumi exposure | EventShapes chains | per-chain `fLumi` | chain sum | comparison / conclusion |
|---|---:|---:|---:|---:|---|
| DJANGOH14/7091 | `4,379,935 nb^-1 = 4,379.935 pb^-1` | 20 | `16,283.27 pb^-1` | `325,665.4 pb^-1` | sum/full = `74.3539`; a chain contains a grouped/overlapping or otherwise differently normalized file subset, not one disjoint oomclumi chunk. |
| DJANGOH14/7093 | `1,535,773 nb^-1 = 1,535.773 pb^-1` | 7 | `5,666.29 pb^-1` | `39,664.03 pb^-1` | sum/full = `25.8268`; same contradiction with “one fLumi per physical chunk.” |
| RAPGAP31/7096 | `3,033,378 nb^-1 = 3,033.378 pb^-1` | 12 | `12,209.99 pb^-1` | `146,519.88 pb^-1` | sum/full = `48.3025`; same result. |

Therefore `fLumi` is **per analysis chain’s effective exposure for the exact files/normalization configured in that chain**. It is not a universal catalog exposure, not a physical cross section, and not a per-file value. The signal-chain steering cannot reproduce the later all-HAT oomclumi catalog accounting because the two samples/selections/groupings are not established identical. This is expected from the source semantics and is not a unit contradiction.

## Provenance and independent signal cross-check

`docs/references/reference-highQ2-mc-luminosity.md:12,54-58,177-180` identifies the documented signal values as all-HAT, per-chunk `oomclumi` results summed in native pb^-1 and multiplied by 1000 to nb^-1. `oomclumi` itself is documented there as

`L_MC = sum(WTX1*WTX2*WTX3)/SIGMT`, with `SIGMT` in pb and native `Lumi` in pb^-1 (lines 20-24).

The primary aggregate JSON independently stores both units. For 7093: `L_MC_pbinv=1535.773` and `L_MC_nbinv=1535773.0` at `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json:108-121`; for 7091 it stores `4379.935 pb^-1` and `4379935.0 nb^-1` (lines 78-91). Hence the reference labels are genuinely nb^-1.

### The 1534.81 versus 1,535,773 relation

`1534.81` in the historical EventShapes/SIDIS catalog table is an `fLumi`-style value and therefore **pb^-1**, despite the research memo’s erroneous “nb^-1” label (`research-ep0304-production-path-2026-05-24.md:77-78,183-199`). Converting the all-HAT oomclumi result gives `1,535.773 pb^-1`, so the residual is

`1,535.773 - 1,534.81 = 0.963 pb^-1 = 0.0627046%`;
`1,535,773 / 1,534.81 = 1000.62744`.

The factor approximately 1000 is the missed pb↔nb conversion. The residual 0.6 per mille is **not resolved by available evidence**. It is consistent with rounded/legacy steering normalizations or a non-identical file set, but no contemporaneous generator-log/oomclumi result for exactly the steering chain was inspected. It must not be silently rounded away.

## DjBkgd verdict and consequences

The five DjBkgd chain sums are `3292.9 + 4557.6 + 3690.9 + 1708.8 + 2563.2 = 15813.4 pb^-1`, as recorded in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/normalization/djbkgd_flumi_recovery_2026-09-25/djbkgd_flumi_summary.json:2-108`. Therefore:

* **Correct effective exposure:** `15,813.4 pb^-1 = 15,813,400 nb^-1`.
* **Correct scale at current target:** `351 pb^-1 / 15,813.4 pb^-1 = 0.0221963651` (equivalently `351000 / 15813400`).
* The historical `356613/15813.4 = 22.5513172` treated pb^-1 as nb^-1 and is exactly 1000 too large.
* **Correct event-level low-Q² leakage at L_data=351000 nb^-1:** `276.5991634` events, not `276,599.1634`.

The current `STATE.md` low-Q² event-level values are thus normalization-invalid pending regeneration/relabeling, although central v6 is raw/no-bgsub and consumes none of this template.

If the template is corrected by 1/1000, the all-five background display becomes

`589,678.2806 - 281,022.3859 + 276.5991634 = 308,932.4938634` events,

with integrated fraction versus the requested raw collapsed denominator

`308,932.4938634 / 3,884,097.0 = 0.07953779060` (**7.953779%**).

Invalidated conclusions: (a) the `background_rel` retirement cannot be justified by a 552% low-Q² template excursion; (b) the “KNOWN DEFECTIVE weight 43.1292” diagnosis as the cause of an unacceptable systematic is invalid—43.1292 remains an outlier requiring provenance review, but its scaled bin effect is 0.55214%, not 552%; (c) the 552% bin claim is invalid; (d) the all-five `589678.2806` and `15.182%` integrated fraction are invalid. The independently defined track-level low-Q² result (`208.3795`) is a different object and is unchanged.

## Event-count sanity check

The template has `12,197` selected grid tracks (`6,177 h+ + 6,020 h-`) from `8,407,750` tracks seen in 800 files. The old scale `22.5513` turns an approximately `12,197` raw unit-weight sum into approximately `275,000` selected expected tracks: it asserts that the dedicated 800-file DjBkgd MC exposure is only 1/22 of data. The source-determined pb^-1 exposure instead gives scale `0.0225513` for the historical 356.613-pb target (or `0.0221964` for current 351 pb), yielding approximately `275` events. The latter is physically compatible with a genuine leakage tail above Q²>150 and with the historical “negligible” description; the former is not. This is unit-free corroboration, not the primary unit proof.

## Signal-chain unit consistency

Using pb^-1 for every EventShapes `fLumi` is self-consistent with H1OO’s `L_data/L_MC` implementation and puts signal chain values at plausible MC exposures (thousands to hundreds of thousands of pb^-1). Interpreting them as nb^-1 would make their MC exposures 1000 times smaller than the framework data luminosity unit and cause 1000-fold excess weights. It also fails against the independently stored oomclumi signal values: 7091’s all-HAT exposure is `4379.935 pb^-1`, not `4,379,935 pb^-1`. The remaining disagreement between individual chain sums and all-HAT catalog exposure is a provenance/grouping issue, not evidence for nb^-1.

## Explicit UNKNOWNs / blockers

1. The exact source and intended file accounting of each historical EventShapes signal `fLumi` value (including the 7091/7093/7096 chain-sum excesses) is UNKNOWN without contemporaneous chain-level oomclumi/generator logs and exact resolved filename expansion.
2. The 0.963-pb^-1 (0.0627%) 1534.81-versus-1535.773 residual is UNKNOWN; no evidence establishes rounding versus a different input subset.
3. The DjBkgd 15813.4-pb^-1 value is source-proven as a sum of five steering-chain exposures, but a standalone all-HAT oomclumi/generator-log verification for those five low-Q² ReqIDs has not been performed. That is the remaining provenance validation, not a unit ambiguity.
4. The source event carrying weight 43.1292 remains UNKNOWN and should be traced before any reuse of the template, irrespective of the now-correct scale.

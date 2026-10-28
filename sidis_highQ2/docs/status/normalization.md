# Normalization and Luminosity Status — High-Q² Charged-Particle SIDIS

**Status authority:** current compact detail for data luminosity, MC scaling traps, and normalization-sensitive high-Q² SIDIS work.

## Current Data Luminosity

Use:

```text
L_data = 351000.0 nb^-1 = 351 pb^-1
```

This is the current four-period S67 target luminosity for high-Q² charged-SIDIS including ep0304.

## Do Not Use These As Current Defaults

- `the superseded pre-rescale luminosity nb^-1`: 2005+2006+2007 only; wrong for current 0304-included high-Q² SIDIS normalization.
- `356613.0 nb^-1 = 356.613 pb^-1`: older direct-monitor quote; superseded for current high-Q² charged-SIDIS defaults.
- `356000 nb^-1`: stale diagnostic shorthand; not authoritative.

## 7091 vs DjBkgd Trap

Processed `merged_7091.root` is **not** the DjBkgd template input.

| Item | Correct value |
|---|---|
| Processed file | `djangoh14/7091` RAD high-Q² signal MC, `NC.EPLUSP.0607.RAD.Q2GT60` |
| Input path | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/djangoh14/merged_7091.root` |
| Effective MC luminosity | `L_MC = 4379935.0 nb^-1 = 4379.935 pb^-1` |
| Provenance record | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`, `per_catalog/djangoh14_7091_ep0607` |
| Current scale | `351000.0 / 4379935.0 = 0.08013817556653238` |
| DjBkgd samples | ReqIDs 7087/7104/7105/7107/7110; all `NC RAD Q2GT2 CTEQ6L` (7110 is not NORAD) |
| DjBkgd steering sum | `15813.4 pb^-1 = 15813400 nb^-1`, across 80 chains/five ReqIDs/four beam periods: 7087 em0405 `4557.60`, 7104 em06 `3690.90`, 7105 ep0607 `1708.80`, 7107 ep0607 `2563.20`, 7110 ep0304 `3292.90` pb^-1 |
| Correct DjBkgd scale | `351000.0 / 15813400 = 0.0221963651`; `L_MC/L_data = 45.05` |

### 2026-09-25 fLumi Unit Correction and Provenance Caveat

`H1SteerTree::fLumi` is in `pb^-1`, not `nb^-1`. H1OO evidence: `H1Analysis/H1Analysis.C:1012` converts `runList->GetIntLumi()` with `/1000.0` to work in pb^-1; `H1AnalysisChain.C:649` passes `SteerTree->GetLumi()` to `H1CalcWeight::SetMCLumi`; `H1CalcWeight.C:324-339` applies `fDataLumi/fMCLumi`. `H1Skeleton/H1SteerTree.h:145` and `.C:63-66` establish the field role. `H1SteerTree.C:277-287` labels the related `fFileLumis` array `Lumi(pb-1)`; this is contextual support, not a direct declaration for `fLumi`.

Thus `15813.4` is the framework-correct **unit interpretation** of the steering-chain sum, not a statement that the exact template input exposure is provenance-closed. Independent oomclumi/generator-log accounting over the exact resolved HAT lists for ReqIDs 7087, 7104, 7105, 7107, and 7110, including a duplicate/overlap audit, remains outstanding. Do not present absolute DjBkgd template yields as final.

The prior `22.5513172` scale and `L_MC/L_data=0.044` resulted from inserting the pb^-1 steering number into nb^-1 arithmetic. The five-ReqID sum is correct; the error was purely the unit, not summing five ReqIDs against one ReqID's luminosity.

Treat artifacts scaled with stale DjBkgd factors or old 356.613 pb^-1 target as historical/provisional unless corrected.

## Alpha0p2 Background Vector Normalization

For nominal combined reco-background vectors, first sum raw `sumw_980` and `sumw2_980` over all accepted files for one `(generator, mode)` sample, then apply one generator-wide scale:

```text
yield_980 = (351000.0 / L_MC_total[generator]) * sumw_980
var_980   = (351000.0 / L_MC_total[generator])^2 * sumw2_980
```

Current combined generator lumis:

| Generator | `L_MC_total` | Scale |
|---|---:|---:|
| RAPGAP31 | `9132891.0 nb^-1` | `0.03843251824641288` |
| DJANGOH14 | `10487495.0 nb^-1` | `0.03346843073584302` |

Do not scale each period to full data luminosity and then sum periods for the nominal combined vector.

## Validation Rule

Before any normalization-sensitive result, verify both sample identity and effective MC luminosity. If the file path, reqID, period, generator, or MC-lumi record is ambiguous, stop and resolve provenance before scaling.

2026-10-16 correction application: the 7091, DjBkgd-ratio, and combined-generator scale decimals recorded in this file were corrected on 2026-10-16 per the 2026-10-08/09 verification record (`data/backgrounds/normalization_verification_recokin_mc_2026-10-08/verification_record.json`) and the 2026-10-10 gate item O1; products were unaffected.

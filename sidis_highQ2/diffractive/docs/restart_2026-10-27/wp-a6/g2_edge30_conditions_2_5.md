# WP-A6 edge30: critic conditions 2 and 5 (2026-10-28)

`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Mechanical, no refit, login node. Script `D/scripts/restart_2026-10-27/wp-a6/edge30/cond25_edge30_wpa6.py` (reuses `fnd_edge30_wpa6.counts` and `fnd_wpa6.resolve_all/fnd`). Outputs in `D/data/restart_2026-10-27/wp-a6/edge30/`: `stability_SB30_even.csv`, `rho_band_SB30.csv`, `delta_edge_SB30.csv`. Frozen files re-checked (sha256 unchanged).

## Condition 2: stability at SB30 (even split), f_ND of the largest chain per variant (bold = > 0.30)

| M_X bin | nominal | nobb | lo1p0 | shared |
|---|---|---|---|---|
| [4, 10) | 0.051 (7091) | 0.234 (7091) | 0.227 (7091) | 0.155 (7091) |
| [10, 20) | 0.127 (nd4cat) | 0.297 (7091) | 0.261 (7071) | 0.158 (7091) |
| [20, 40] | 0.269 (nd4cat) | **0.353** (7091) | 0.233 (nd4cat) | 0.166 (7091) |

Nominal reproduces 0.051 / 0.127 / 0.269. Flag for WP-F: `nobb`, [20, 40], 7091, f_ND = 0.353 (nominal for that chain 0.232); `nobb` [10, 20) is 0.297, 0.003 below the ceiling. Per-chain values are in the CSV.

## Condition 5: rho on the residual band (ec08 < 3.0, ec04 >= 3.0)

rho = max |data/prediction - 1| over all band bins (ec04 >= 3.0 up to the sel30 tail) with >= 20 data, nominal resolved scales, per (chain, M_X, cell); class-summed fallback where no bin has >= 20 data (the [4, 10) cells; LOG lines). The frozen-payload rho came from the 8 bins [3.0, 3.4) of the cluster sample. Max rho over cells is 1.0-2.3 in [4, 10), 2.2-4.0 in [10, 20), 1.3-3.0 in [20, 40] (band value, `delta_edge_SB30.csv`); `rho_band_SB30.csv` also carries the first-8-bin rho.

f_ND + delta_edge, largest chain, per-chain rho / chain-independent rho (max over chains per cell); previous delta_edge in brackets.

| split | M_X bin | chain | f_ND | delta (chain) | delta (max chain) | f + delta | crosses 0.30 |
|---|---|---|---|---|---|---|---|
| odd | [4, 10) | nd4cat | 0.040 | 0.009 [0.008] | 0.009 | 0.049 / 0.049 | no / no |
| odd | [10, 20) | 7091 | 0.243 | 0.122 [0.037] | 0.210 | 0.365 / 0.453 | yes / yes |
| odd | [20, 40] | nd4cat | 0.221 | 0.082 [0.043] | 0.122 | 0.303 / 0.343 | yes / yes |
| even | [4, 10) | 7091 | 0.051 | 0.001 [0.004] | 0.013 | 0.052 / 0.064 | no / no |
| even | [10, 20) | nd4cat | 0.127 | 0.127 [0.028] | 0.172 | 0.254 / 0.299 | no / no (0.001 below) |
| even | [20, 40] | nd4cat | 0.269 | 0.114 [0.030] | 0.315 | 0.383 / 0.584 | yes / yes |

Non-largest chains: odd [10, 20) nd4cat 0.270 / 0.293 (no); odd [20, 40] 7071 0.262 / 0.288 (no); even [20, 40] 7091 crosses under both (0.307 / 0.470).

## Reading

- With rho taken on the band itself, delta_edge is about 2-5 times the previous value in [10, 20) and [20, 40]; the largest-chain f_ND + delta_edge crosses 0.30 in odd [10, 20), odd and even [20, 40] under both rho choices, and stays below in [4, 10) and in even [10, 20) (0.254 / 0.299).
- Caveat for Open Q1: rho is a max over bins, so sparse tail bins (20-60 data) set it; rho > 1 means data exceed the prediction by more than a factor 2 in some tail bin. The result is sensitive to the bin range chosen for the max; the first-8-bin variant is in the CSV for comparison.
- This is information for Open Q1, not a re-ruling.

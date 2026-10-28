# In-fiducial ISR-boost impact — bounded one-file diagnostic

**Scope:** one RAD e+ file (`rad_ep_0680.root`) and one NONRAD e+ file (`nonrad_new_ep_0000.root`), read-only. Truth selection exactly reproduces `produce_crad_truth_v2.py`; weights are `weight1*weight2`. `z` is `P·h/P·q`; `pT_h` is transverse to the boosted photon axis in the gamma-star--proton CM. Variant A uses nominal incoming-electron energy; Variant B uses `eeff=GenBeamElecE-GenEnPhoton`, subject to the builder guard `eeff>0.5 && GenElecE>0.5`.

## Variant-A reproduction (selected rows)

| sample | selected rows | weighted A--stored z abs median | z rel median | pT abs median (GeV) | pT rel median | status |
|---|---:|---:|---:|---:|---:|---|
| RAD | 317115 | 2.48813e-7 | 1.49159e-5 | 1.92722e-6 | 4.46282e-6 | PASS |
| NONRAD | 51662 | 2.60555e-7 | 1.51642e-5 | 2.02730e-6 | 4.76119e-6 | PASS |

## RAD selected-row shifts

Affected fraction (`GenEnPhoton>=0.5 GeV` and builder guard valid): **0.19857546**.

| subset | pT_B-pT_A median (GeV) | p99 (GeV) | z_B-z_A median | p99 |
|---|---:|---:|---:|---:|
| all selected | 0 | 0.415564 | 0 | 0.0628420 |
| affected | 0.000610157 | 2.01882 | 0.00384127 | 0.179590 |

## RAD migrations and B/A yields

Fractions are of all selected rows of the charge. Yield ratios are independently binned B divided by A; overflow/underflow is not included in a displayed bin.

| charge | z-bin change | pT-bin change | either change |
|---|---:|---:|---:|
| h+ | 0.0337443 | 0.0360753 | 0.0537681 |
| h- | 0.0316754 | 0.0351053 | 0.0517206 |

| z interval | h+ B/A | h- B/A |
|---|---:|---:|
| [0,0.05) | 0.981832 | 0.982261 |
| [0.05,0.1) | 1.020366 | 1.024579 |
| [0.1,0.2) | 1.037026 | 1.037113 |
| [0.2,0.3) | 1.079566 | 1.081844 |
| [0.3,0.5) | 1.116359 | 1.101434 |
| [0.5,0.7) | 1.197206 | 1.242266 |
| [0.7,1.0] | 1.507340 | 1.684937 |

| pT_h interval (GeV) | h+ B/A | h- B/A |
|---|---:|---:|
| [0,0.5) | 0.998463 | 0.999951 |
| [0.5,1) | 0.998511 | 0.995541 |
| [1,2) | 1.001032 | 0.998601 |
| [2,4) | 1.007212 | 1.012974 |
| [4,8] | 1.062609 | 1.063989 |

The total fraction retained in the displayed `(z,pT_h)` ranges changes A to B from 0.99902047 to 0.99863198 (h+) and from 0.99913176 to 0.99874156 (h-): ratios 0.99961112 and 0.99960946.

## NONRAD null check

Affected fraction is 0; all weighted z, pT, and either migrations are 0 for h+ and h-; every displayed B/A ratio is 1.0; all shift medians and p99 values are 0.

## Verdict

**No.** This repair cannot move integrated `C_rad` from 0.6770210/0.8509564 toward 1.05--1.12: the selected, displayed-range RAD total changes by only -0.0389%, implying a first-order `C_rad` increase of only about +0.039%. Its z behavior is opposite the reported anomaly: correcting RAD raises high-z RAD yields strongly, hence would lower `C_rad` at high z rather than explain the observed monotonic rise. Its pT behavior is also opposite: the repair raises high-pT RAD yields (about 6.3%), hence would lower `C_rad` at high pT rather than explain the observed fall. The anomalous normalization and shape must therefore arise from another source, requiring the still-open matched RAD/NONRAD exposure/provenance and bin-local compatibility/normalization investigation; sparse-bin policy and the broader C_rad methodology remain blockers.

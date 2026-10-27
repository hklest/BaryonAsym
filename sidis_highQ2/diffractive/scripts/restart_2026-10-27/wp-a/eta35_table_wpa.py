#!/usr/bin/env python3
"""WP-A S1 diagnostic (no gate role): forward energy scale in eta_max > 3.5
(ND-dominated) from the S2 partials' eta35_eplug block (ePlug before remedy).
Per data period: DATA mean ePlug and fraction >= 7 GeV vs the chain cocktail
projected onto the period (as in noise_table_wpa.py); also the MC/DATA ratio of
the mean."""
import json, sys
import numpy as np
import wpa_common as W
ch = sys.argv[1] if len(sys.argv) > 1 else "7071"
R = W.OUT / "s2_remedy1/partials/test"; L = W.period_lumi()
comps = W.DIFF8 + W.CHAINS[ch]["nd"]
out = {}
for k in W.PERIODS:
    e = W.load_partial(R / "data", k)["wpa"]["eta35_eplug"][k]
    dm = e["sum_eplug_raw"] / e["n_raw"]; dfr = e["n_ge_T_raw"] / e["n_raw"]
    sw = swe = swT = 0.0
    for l in comps:
        if k not in W.represented(l, ch):
            continue
        m = W.load_partial(R / ch, l)["wpa"]["eta35_eplug"]["ALL"]; r = (L[k] / W.L_DATA) / W.table_fac(l)
        sw += r * m["sum_w"]; swe += r * m["sum_w_eplug"]; swT += r * m["w_ge_T"]
    out[k] = {"n_data": e["n_raw"], "data_mean_eplug": dm, "mc_mean_eplug": swe / sw, "mean_ratio_mc_over_data": swe / sw / dm,
              "data_frac_ge7": dfr, "mc_frac_ge7": swT / sw, "data_over_mc_yield": e["n_raw"] / sw}
o = W.OUT / ("s2_remedy1/analysis/eta35_eplug_%s.json" % ch); json.dump(out, open(o, "w"), indent=1)
for k, v in out.items():
    print(k, {a: round(b, 3) for a, b in v.items()})

#!/usr/bin/env python3
"""WP-D2: per-period weights of the 840x family. w_R = L_data(period) / L_eq(R); stream factor NOT applied (floated in WP-A56').
L_data: certified ACCEPT lumi per period (s1_period_split_record.md; ep0607 = fall-06 + 2007). L_eq = N_gen / sigma_Tot (Round 3 of mc_candidates_assessment)."""
import json, csv
from pathlib import Path
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
Ld = {"ep0304": 43833.59, "em0405": 98087.05, "em06_true": 51216.11, "ep0607_true": 79108.01 + 44968.04}   # nb^-1
sig = {"5643": 30275.2, "5644": 30209.4}                                                                      # pb
Nint = 10_000_000
R = {"8402": ("5644", "e+", "ep0304", "03/04"), "8403": ("5643", "e-", "em0405", "04/05"),
     "8404": ("5643", "e-", "em06_true", "2006"), "8405": ("5644", "e+", "ep0607_true", "06/07")}
tot = sum(Ld.values()); out = {"L_data_nb": Ld, "L_data_total_nb": tot, "sigma_pb": sig, "samples": {}}
chk = 0.
for r, (g, ch, per, lab) in R.items():
    Leq_nb = Nint / sig[g] * 1e3                      # nb^-1
    w = Ld[per] / Leq_nb
    out["samples"][r] = {"gen": g, "charge": ch, "period": per, "L_eq_nb": Leq_nb, "L_eq_pb": Leq_nb / 1e3, "w_period": w,
                         "L_eq_recorded_pb": 331.0 if g == "5644" else 330.3}
    chk += w * Leq_nb
out["sum_w_Leq_nb"] = chk; out["sum_check_rel"] = chk / tot - 1
out["stream_factor"] = {"applied": 1.0, "prior_offset_to_9012": "+6..8 % (A:45-47); floated per generator stream in WP-A56'"}
dd = D / "data/restart_2026-10-27/wp-d2"
json.dump(out, open(dd / "period_weights.json", "w"), indent=1)
with open(dd / "scale_table_840x.csv", "w", newline="") as f:     # skim schema: scale_to_data = w_period, exposure_factor = 1
    wr = csv.writer(f); wr.writerow(["component", "L_MC_nb", "scale_to_data", "exposure_factor", "stream_flag", "weight_recipe"])
    for r, s in out["samples"].items():
        wr.writerow([r, "%.1f" % s["L_eq_nb"], "%.8f" % s["w_period"], "1.0", "rapgap31043 gen %s %s %s, stream factor not applied" % (s["gen"], s["charge"], s["period"]),
                     "w_event=weight1*weight2 (=1); fill_w=w_event*w_period"])
print(json.dumps(out["samples"], indent=1)); print("sum", chk, out["sum_check_rel"])

#!/usr/bin/env python3
"""WP-A2 S3: H3 fit table from the even-split fits (tests h3_variants_complete,
pdiss_bound_respected) and the H3 reading per D3/R3: supported iff for one ND
alternative the toy p of (ii), (iii) or (iv) is >= 10x the (i) value (floor 0.0010
-> >= 0.010) in every D cell and ALL, coarse and fine, AND H4 true-detector is not
supported.  Writes s3_corr/analysis/h3_table.{csv,json}."""
import csv, json
from pathlib import Path
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a2")
F = D / "s3_corr/fits"; OUT = D / "s3_corr/analysis"
S2 = json.load(open(D / "s2_diag/analysis/s2_analysis_even.json"))
rows = []; P = {}
for v in ("h3i", "h3ii", "h3iii", "h3iv"):
    for alt in ("7071", "7091", "nd4cat"):
        for b, fb in (("fine", "fine"), ("coarse", "coarse_ge100_data")):
            for s in ("D0", "D1", "D2", "ALL"):
                r = [x for x in csv.DictReader(open(F / v / ("fit_summary_%s_%s_%s.csv" % (alt, b, s)))) if x["binning"] == fb and x["scope"] == s]
                assert len(r) == 1
                r = r[0]; P[(v, alt, b, s)] = float(r["toy_p"])
                row = {"variant": v, "alt": alt, "binning": b, "scope": s, "toy_p": float(r["toy_p"]), "n_toys": 1000,
                       "at_floor": abs(float(r["toy_p"]) - 1 / 1001) < 1e-12, "D": float(r["deviance"]), "ndf": int(r["ndf"])}
                if v == "h3iv":
                    j = json.load(open(F / v / ("section_7_1_twofree_fit_results_%s_%s_%s.json" % (alt, b, s))))
                    rec = j[fb][alt][s]; row["t_pdiss_over_elastic"] = rec["t_pdiss_over_elastic"]; row["t_at_bound"] = rec["t_at_bound"]
                    assert 0.5 - 1e-9 <= rec["t_pdiss_over_elastic"] <= 2.0 + 1e-9
                rows.append(row)
cols = ["variant", "alt", "binning", "scope", "toy_p", "n_toys", "at_floor", "D", "ndf", "t_pdiss_over_elastic", "t_at_bound"]
with open(OUT / "h3_table.csv", "w", newline="") as f:
    w = csv.DictWriter(f, cols); w.writeheader(); w.writerows(rows)
rise = {}
for alt in ("7071", "7091", "nd4cat"):
    for v in ("h3ii", "h3iii", "h3iv"):
        rise["%s|%s" % (v, alt)] = all(P[(v, alt, b, s)] >= 10 * P[("h3i", alt, b, s)] - 1e-12 for b in ("fine", "coarse") for s in ("D0", "D1", "D2", "ALL"))
h4det = S2["H4"]["reading"] == "true-detector"
res = {"n_rows": len(rows), "rise_ge_10x": rise, "h4_true_detector": h4det, "supported": any(rise.values()) and not h4det,
       "max_p": max(r["toy_p"] for r in rows), "n_at_floor": sum(r["at_floor"] for r in rows)}
json.dump(res, open(OUT / "h3_table.json", "w"), indent=1); print(json.dumps(res))
for v in ("h3i", "h3ii", "h3iii", "h3iv"):
    for alt in ("7071", "7091", "nd4cat"):
        rr = [r for r in rows if r["variant"] == v and r["alt"] == alt]
        a = [r for r in rr if r["scope"] == "ALL"]
        print(v, alt, "max p %.4f" % max(r["toy_p"] for r in rr), "ALL fine %.0f/%d coarse %.0f/%d" % (a[0]["D"], a[0]["ndf"], a[1]["D"], a[1]["ndf"]),
              ("t=" + ",".join("%.2f" % r["t_pdiss_over_elastic"] for r in rr)) if v == "h3iv" else "")

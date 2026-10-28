#!/usr/bin/env python3
"""WP-A6 S3a tests of fnd_wpa6.py outputs (login-node). Usage: tests_s3_wpa6.py --split {even,odd}"""
import argparse, csv, json, sys
from pathlib import Path
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive"); O = D / "data/restart_2026-10-27/wp-a6"
sys.path.insert(0, str(D / "scripts/restart_2026-10-27/wp-a")); import wpa_common as W
MX, CH = ["MX4_10", "MX10_20", "MX20_40"], ["7071", "7091", "nd4cat"]

def rd(name): return list(csv.DictReader(open(O / name)))
def rep(name, ok, msg): print(f"{'PASS' if ok else 'FAIL'} {name}: {msg}"); return ok

def t_g2_table(sp):
    rows = [r for r in rd("f_nd_per_mx.csv") if r["split"] == sp]; need = ["f_nd", "sig_fit", "spread", "delta_edge", "above_ceiling"]
    keys = {(r["mx_bin"], r["chain"]) for r in rows}; full = keys == {(m, c) for m in MX for c in CH} and len(rows) == 9
    return rep("g2_table", full and all(r[k] != "" for r in rows for k in need), f"{len(rows)} rows, 3x3 grid={full}")

def t_spread_as_syst(sp):
    rows = [r for r in rd("f_nd_per_mx.csv") if r["split"] == sp]; worst = 0.0
    for m in MX:
        f = {r["chain"]: float(r["f_nd"]) for r in rows if r["mx_bin"] == m}; exp = max(abs(f["7091"] - f["7071"]), abs(f["nd4cat"] - f["7071"]))
        for r in rows:
            if r["mx_bin"] == m: worst = max(worst, abs(float(r["spread"]) - exp))
    return rep("spread_as_syst", worst < 1e-12, f"max |spread - max|f(chain)-f(7071)|| = {worst:.1e}")

def t_parts_sum(sp):
    rows = [r for r in rd("f_nd_per_mx.csv") if r["split"] == sp]; w = max(abs(float(r["f_nd_infit"]) + float(r["f_nd_band"]) - float(r["f_nd"])) for r in rows)
    return rep("parts_sum", w < 1e-9, f"max |infit + band - f_nd| = {w:.1e}")

def t_fallback_logged(sp):
    fb = {(r["chain"], r["tag"], r["cell"], r["mx_class"]): r for r in rd(f"fallback_{sp}.csv")}; miss, n = [], 0
    for r in rd(f"scales_resolved_{sp}_nominal.csv"):
        if r["scope"] != "ALL" and (float(r["n_data_fit"]) < 100 or float(r["n_nd_raw_fit"]) < 100):
            n += 1; k = (r["chain"], "nominal", r["scope"], r["mx_class"]); x = fb.get(k)
            if x is None or abs(float(x["s_diff_used"]) - float(r["s_diff"])) > 1e-12 or abs(float(x["s_ND_used"]) - float(r["s_ND"])) > 1e-12 or r["source"] != "fallback_all": miss.append(k)
    return rep("fallback_logged", not miss and n > 0, f"{n} nominal fallback units checked, {len(miss)} missing/inconsistent {miss[:3]}")

def t_recompute(sp, chain="7091", m="MX10_20"):
    """f_nd by hand: raw payload JSON + resolved-scale CSV only."""
    p = json.load(open(O / f"payloads/{sp}_{chain}.json")); fac = p["scale_table_basis"]["exposure_factors"]; S = p["sel"]; num = den = 0.0
    sc = {r["scope"]: float(r["s_ND"]) for r in rd(f"scales_resolved_{sp}_nominal.csv") if r["chain"] == chain and r["mx_class"] == m}
    for c in ["D0", "D1", "D2"]:
        den += sum(S["data"]["templates"]["raw"][c][m]) + S["data"]["oor"][c][m]
        for lab in W.CHAINS[chain]["nd"]: num += sc[c] * fac[lab] * (sum(S["components"][lab]["templates"]["exposure1"][c][m]) + S["components"][lab]["oor"]["exposure1"][c][m])
    r = [x for x in rd("f_nd_per_mx.csv") if x["split"] == sp and x["chain"] == chain and x["mx_bin"] == m][0]
    return rep("recompute_f_nd", abs(num / den - float(r["f_nd"])) < 1e-9, f"{chain} {m}: by hand {num / den:.8f} vs table {float(r['f_nd']):.8f}")

if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("--split", choices=["even", "odd"], required=True); sp = ap.parse_args().split
    ok = [t(sp) for t in (t_g2_table, t_spread_as_syst, t_parts_sum, t_fallback_logged, t_recompute)]; sys.exit(0 if all(ok) else 1)

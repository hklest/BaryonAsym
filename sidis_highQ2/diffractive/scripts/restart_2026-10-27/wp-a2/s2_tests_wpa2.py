#!/usr/bin/env python3
"""WP-A2 S2 tests (login node, numpy on JSON; child plan S2 Tests):
  split_disjoint_complete   even/odd key overlap 0 per file (in-worker) and even+odd
                            raw counts == WP-A split=all counts per mask and D cell
  edge_localisation_reproduced   even-split baseline two-template fit (no toys) on the
                            cutremoved_c08 payload: DATA/fit > 1 in [3.15,3.35) and < 1
                            in [3.40,3.55) in D0 (summed over each window)
The empz_join_coverage and h4_control_is_inclusive numbers are printed by
s2_analyze_wpa2.py."""
import argparse, json, subprocess, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a")); sys.path.insert(0, str(HERE))
import wpa_common as W
from fit_7p1_wpa2 import fit

LABELS = W.DATA_LABELS + sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--root", required=True)
    ap.add_argument("--wpa-all", default=str(W.D / "data/restart_2026-10-27/wp-a/s1_split/partials/all"))
    a = ap.parse_args(); root = Path(a.root); res = {}
    bad = []
    for l in LABELS:
        e = json.load(open(root / "partials" / l / "even" / "partial.json")); o = json.load(open(root / "partials" / l / "odd" / "partial.json"))
        r = json.load(open(Path(a.wpa_all) / l / "partial.json"))
        if e["wpa2"]["even_odd_key_overlap"] or o["wpa2"]["even_odd_key_overlap"]:
            bad.append((l, "overlap"))
        for m, v in r["counts"].items():
            if e["counts"][m]["raw_int"] + o["counts"][m]["raw_int"] != v["raw_int"]:
                bad.append((l, m, "raw_int"))
            for c in range(3):
                if e["counts"][m]["Dcells_raw"][c] + o["counts"][m]["Dcells_raw"][c] != v["Dcells_raw"][c]:
                    bad.append((l, m, "D%d" % c))
    res["split_disjoint_complete"] = {"pass": not bad, "failures": bad[:10], "n_files": len(LABELS)}
    pl = root / "analysis" / "baseline_even_7071_payload.json"
    subprocess.run([sys.executable, str(HERE / "payload_wpa2.py"), "--root", str(root), "--split", "even", "--chain", "7071",
                    "--out", str(pl), "--check-baseline"], check=True, stdout=subprocess.DEVNULL)
    x = json.load(open(pl)); c = x["modes"]["cluster"]; fac = x["scale_table_basis"]["exposure_factors"]
    E = np.asarray(x["edges"]); lo = E[:-1]
    loc = {}
    for z in ("D0", "D1", "D2"):
        y = np.asarray(c["data"]["templates"]["raw"][z], float)
        d = sum(np.asarray(c["components"][k]["templates"]["exposure1"][z], float) * fac[k] for k in W.DIFF8)
        n = np.asarray(c["components"]["7071"]["templates"]["exposure1"][z], float) * fac["7071"]
        q, D = fit(y, d, n); pred = q[0] * d + q[1] * n
        ex = (lo >= 3.15 - 1e-9) & (lo < 3.35 - 1e-9); de = (lo >= 3.40 - 1e-9) & (lo < 3.55 - 1e-9)
        loc[z] = {"data_over_fit_3p15_3p35": float(y[ex].sum() / pred[ex].sum()), "data_over_fit_3p40_3p55": float(y[de].sum() / pred[de].sum()),
                  "deviance": D, "ndf": len(y) - 2}
    res["edge_localisation_reproduced"] = {"pass": loc["D0"]["data_over_fit_3p15_3p35"] > 1 and loc["D0"]["data_over_fit_3p40_3p55"] < 1, "cells": loc}
    json.dump(res, open(root / "analysis" / "s2_tests.json", "w"), indent=1)
    print(json.dumps({k: v["pass"] for k, v in res.items()}), json.dumps(loc))


if __name__ == "__main__":
    main()

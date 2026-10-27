#!/usr/bin/env python3
"""WP-A2 S3 (derivation split only): H1 response-branch threshold choice (D3 H1:
"for the fit, the threshold variant with the smaller edge residual, chosen on the
derivation split").  Edge residual = Poisson deviance contribution summed over
eta in [3.15, 3.55) of the full-range two-template fit (no toys), scope ALL, fine
binning, summed over the G1'(c) alternatives 7071, 7091, nd4cat.  Also reports,
for the H4 range correction (eta < 3.4 + merged [3.4, 5)), the same fit's D/ndf
per variant as context (no choice is made from it)."""
import json, subprocess, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a")); sys.path.insert(0, str(HERE))
import wpa_common as W
from fit_7p1_wpa2 import fit, dterms

ROOT = W.D / "data/restart_2026-10-27/wp-a2/s2_diag"
OUT = W.D / "data/restart_2026-10-27/wp-a2/s3_corr"
CELLS = ("D0", "D1", "D2")


def payload(chain, branch, merge):
    p = OUT / "payloads" / ("even_%s_%s%s.json" % (chain, branch[-2:], "_m34" if merge else ""))
    cmd = [sys.executable, str(HERE / "payload_wpa2.py"), "--root", str(ROOT), "--split", "even", "--chain", chain,
           "--eta-branch", branch, "--out", str(p)] + (["--eta-max-cut", "3.4", "--edge-merge"] if merge else [])
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)
    return json.load(open(p)), p


def fit_all(x, chain):
    c = x["modes"]["cluster"]; fac = x["scale_table_basis"]["exposure_factors"]; nd = W.CHAINS[chain]["nd"]
    y = np.concatenate([np.asarray(c["data"]["templates"]["raw"][z], float) for z in CELLS])
    d = np.concatenate([sum(np.asarray(c["components"][k]["templates"]["exposure1"][z], float) * fac[k] for k in W.DIFF8) for z in CELLS])
    n = np.concatenate([sum(np.asarray(c["components"][k]["templates"]["exposure1"][z], float) * fac[k] for k in nd) for z in CELLS])
    q, D = fit(y, d, n)
    return y, q[0] * d + q[1] * n, D, len(y) - 2


def main():
    res = {"split": "even", "rows": {}}
    tot = {}
    for br in ("etaMaxClus08", "etaMaxClus04"):
        tot[br] = 0.0
        for ch in ("7071", "7091", "nd4cat"):
            x, p = payload(ch, br, False)
            E = np.asarray(x["edges"]); lo = np.tile(E[:-1], 3)
            y, m, D, ndf = fit_all(x, ch)
            w = (lo >= 3.15 - 1e-9) & (lo < 3.55 - 1e-9)
            er = float(dterms(y, m)[w].sum())
            xm, pm = payload(ch, br, True); ym, mm, Dm, ndfm = fit_all(xm, ch)
            res["rows"]["%s|%s" % (br, ch)] = {"payload": str(p), "payload_sha256": W.sha256(p), "edge_residual_3p15_3p55": er,
                                              "D": D, "ndf": ndf, "merged34_D": Dm, "merged34_ndf": ndfm, "merged34_payload": str(pm)}
            tot[br] += er
    res["edge_residual_sum"] = tot
    res["chosen"] = min(tot, key=tot.get)
    (OUT / "analysis").mkdir(parents=True, exist_ok=True)
    json.dump(res, open(OUT / "analysis" / "h1_threshold_choice.json", "w"), indent=1)
    print(json.dumps({"edge_residual_sum": tot, "chosen": res["chosen"],
                      "D/ndf": {k: "%.0f/%d | merged %.0f/%d" % (v["D"], v["ndf"], v["merged34_D"], v["merged34_ndf"]) for k, v in res["rows"].items()}}, indent=1))


if __name__ == "__main__":
    main()

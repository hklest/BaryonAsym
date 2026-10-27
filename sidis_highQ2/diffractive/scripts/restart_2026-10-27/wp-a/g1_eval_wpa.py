#!/usr/bin/env python3
"""WP-A G1(a)/(b) evaluator on n1_wpa partials (stack C, signed v3 weights).

Replaces the plan's assemble_wpa/analyze_wpa forks: the t4 assembler's fixed
X1-X6 cross-checks are tied to the full frozen sample and cannot run on a
split; the metric is the registered shape_metric copied verbatim into
wpa_common.py (unit area, bins with >= 20 DATA entries, max |MC/DATA-1|).
Identity on split=all is checked against the t4 numbers (--check-t4).

G1(a): Plug N-1 (C_N1_PLUG, ePlug) max dev <= 0.10 and per D cell
       |f_fail,MC - f_fail,DATA| <= 0.02, f_fail = 1 - C_SB/C_N1_PLUG
       (DATA raw counts, MC signed sums).
G1(b): FMD (C_N1_FMD, nFmu12) and FTS (C_N1_FTS, nFts28) N-1 max dev < 0.25
       for at least one admissible cocktail: per-component multipliers on the
       signed v3 scales in {0.5,0.75,1,1.25,1.5} (grid step 0.25), the chain's
       ND labels sharing one multiplier (one ND alternative = one template);
       both observables evaluated on the SAME cocktail.
"""
import argparse, itertools, json, math
from pathlib import Path
import numpy as np
import wpa_common as W

MULTS = np.array([0.5, 0.75, 1.0, 1.25, 1.5])


def hist(p, mask, obs, w):
    return np.asarray(p["hists"][mask][obs][w], float)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--data-dir", required=True); ap.add_argument("--mc-dir", required=True)
    ap.add_argument("--chain", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--no-plug-veto", action="store_true", help="remedy 3: skip the Plug clauses")
    ap.add_argument("--check-t4", action="store_true")
    a = ap.parse_args()
    nd = W.CHAINS[a.chain]["nd"]; comps = W.DIFF8 + nd
    dp = {k: W.load_partial(a.data_dir, k) for k in W.DATA_LABELS}
    mp = {l: W.load_partial(a.mc_dir, l) for l in comps}
    # per-label weight rescale for chain-specific exposure (nd4cat 7071 -> em06)
    rs = {l: W.fac_used(l, a.chain) / W.table_fac(l) for l in comps}
    cocks = {"signal": W.SIGNAL, "plus_reg": W.DIFF8, "plus_reg_" + a.chain: comps}

    def mc(mask, obs, cl, mult=None):
        return sum(rs[l] * (1.0 if mult is None else mult.get(l, 1.0)) * hist(mp[l], mask, obs, "signed") for l in cl)

    def data(mask, obs):
        return sum(hist(dp[k], mask, obs, "raw") for k in W.DATA_LABELS)

    res = {"chain": a.chain, "components": comps, "exposure_rescale": rs,
           "inputs": {**{k: W.sha256(Path(a.data_dir) / k / "partial.json") for k in dp},
                      **{l: W.sha256(Path(a.mc_dir) / l / "partial.json") for l in mp}},
           "splits": {"data": sorted({p["wpa"]["split"] for p in dp.values()}),
                      "mc": sorted({p["wpa"]["split"] for p in mp.values()})},
           "removal_applied": {l: mp[l]["wpa"]["removal"]["applied"] for l in comps}}
    shapes = {}
    for mask, obs in (("C_N1_PLUG", "ePlug"), ("C_N1_FMD", "nFmu12"), ("C_N1_FTS", "nFts28"), ("C_SB", "ePlug")):
        for cn, cl in cocks.items():
            dv, b, nq = W.shape_metric(data(mask, obs), mc(mask, obs, cl))
            shapes["%s|%s|%s" % (mask, obs, cn)] = {"max_dev": dv, "driver_bin": b, "n_qual": nq, "verdict": W.verdict(dv)}
    res["shapes"] = shapes
    # f_fail per D cell
    ff = {}
    for c in range(3):
        dn = sum(dp[k]["counts"]["C_N1_PLUG"]["Dcells_raw"][c] for k in dp)
        dpass = sum(dp[k]["counts"]["C_SB"]["Dcells_raw"][c] for k in dp)
        fd = 1 - dpass / dn
        row = {"data": fd, "data_err": math.sqrt(fd * (1 - fd) / dn), "n_data": dn}
        for cn, cl in cocks.items():
            mn = sum(rs[l] * mp[l]["counts"]["C_N1_PLUG"]["Dcells_signed"][c] for l in cl)
            mpass = sum(rs[l] * mp[l]["counts"]["C_SB"]["Dcells_signed"][c] for l in cl)
            row[cn] = 1 - mpass / mn; row["delta_" + cn] = row[cn] - fd
        ff["D%d" % c] = row
    res["f_fail_plug"] = ff
    nom = "plus_reg_" + a.chain
    pl = shapes["C_N1_PLUG|ePlug|" + nom]["max_dev"]
    worst = max(abs(ff["D%d" % c]["delta_" + nom]) for c in range(3))
    res["G1a"] = ({"ruled": "not applicable (remedy 3, no Plug veto)"} if a.no_plug_veto else
                  {"plug_n1_max_dev": pl, "max_abs_delta_f_fail": worst,
                   "shape_pass": pl is not None and pl <= 0.10, "rate_pass": worst <= 0.02,
                   "PASS": bool(pl is not None and pl <= 0.10 and worst <= 0.02)})
    # G1(b) admissible scan
    groups = W.DIFF8 + ["ND"]
    H = {}
    for mask, obs in (("C_N1_FMD", "nFmu12"), ("C_N1_FTS", "nFts28")):
        rows = [rs[l] * hist(mp[l], mask, obs, "signed") for l in W.DIFF8] + [sum(rs[l] * hist(mp[l], mask, obs, "signed") for l in nd)]
        H[obs] = (np.vstack(rows), data(mask, obs))
    combos = np.array(list(itertools.product(MULTS, repeat=len(groups))))
    best = {"joint": (np.inf, None), "nFmu12": (np.inf, None), "nFts28": (np.inf, None)}
    for s in range(0, len(combos), 200000):
        C = combos[s:s + 200000]; devs = {}
        for obs, (Hm, dh) in H.items():
            q = dh >= W.MIN_DATA_ENTRIES; dn = dh[q] / dh[q].sum()
            M = C @ Hm[:, q]; M = M / M.sum(axis=1, keepdims=True)
            devs[obs] = np.max(np.abs(M / dn - 1), axis=1)
        j = np.maximum(devs["nFmu12"], devs["nFts28"])
        for key, arr in (("joint", j), ("nFmu12", devs["nFmu12"]), ("nFts28", devs["nFts28"])):
            i = int(np.argmin(arr))
            if arr[i] < best[key][0]:
                best[key] = (float(arr[i]), {"mults": dict(zip(groups, C[i].tolist())),
                                             "fmd": float(devs["nFmu12"][i]), "fts": float(devs["nFts28"][i])})
    # nominal (all multipliers 1) cross-check against the direct metric
    nomdev = {o: shapes["%s|%s|%s" % (m, o, nom)]["max_dev"] for m, o in (("C_N1_FMD", "nFmu12"), ("C_N1_FTS", "nFts28"))}
    res["G1b"] = {"nominal": nomdev, "scan_n_cocktails": int(len(combos)),
                  "best_joint": best["joint"][1] | {"max": best["joint"][0]},
                  "best_fmd_only": best["nFmu12"][1] | {"max": best["nFmu12"][0]},
                  "best_fts_only": best["nFts28"][1] | {"max": best["nFts28"][0]},
                  "PASS": bool(best["joint"][0] < 0.25)}
    # removal closure (A5 / test removal_rate_closure)
    rc = {}
    for l in comps:
        r = mp[l]["wpa"]["removal"]
        if not r["applied"]:
            continue
        for k, v in r["per_period"].items():
            n, h = v["n_candidates_pre"], v["n_hit_pre"]
            sig = math.sqrt(max(n * v["q"] * (1 - v["q"]), 1e-12))
            rc["%s|%s" % (l, k)] = {"q": v["q"], "n": n, "hit": h, "pull": (h - n * v["q"]) / sig if n else 0.0}
    res["removal_rate_closure"] = {"rows": rc, "PASS": all(abs(v["pull"]) <= 3 for v in rc.values())}
    if a.check_t4:
        t4 = json.load(open(W.D / "data/t4/localization_diag_2026-10-20/analysis/localization_synthesis.json"))
        ref = t4["data_fail_rates"]["C_plug"]
        res["check_t4"] = {"data_fail_ref": ref, "data_fail_got": [ff["D%d" % c]["data"] for c in range(3)],
                           "fail_fraction_recomputed": all(abs(ff["D%d" % c]["data"] - ref[c]) < 0.002 for c in range(3))}
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    json.dump(res, open(a.out, "w"), indent=1)
    print(json.dumps({"chain": a.chain, "G1a": res["G1a"], "G1b": {k: res["G1b"][k] for k in ("nominal", "PASS")},
                      "G1b_best_joint": res["G1b"]["best_joint"], "f_fail": ff,
                      "closure": res["removal_rate_closure"]["PASS"], "check_t4": res.get("check_t4")}, indent=1))


if __name__ == "__main__":
    main()

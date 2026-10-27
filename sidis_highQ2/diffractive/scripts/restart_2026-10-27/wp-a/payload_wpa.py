#!/usr/bin/env python3
"""WP-A S3: §7.1 cut-removed cluster-eta fit payload built from n1_wpa partials.

Replaces the plan's eta_builder_wpa.py + reused v3 assembler: the n1 partial's
dcell_eta["cutremoved"] block is the same basis (A_pre && selFmd && ePlug<T &&
selFts, eta cut removed, etaMaxClus08, shared 121-edge grid) and was proven
equal to assembled_v3 by the t4 assembler cross-checks X1/X2
(scripts/t4/assemble_localization_diag_2026-10-20.py:107-151); the v3
assembler cannot take corrected partials (its control cross-comparison
demands the uncorrected below-cut identity).  --compare re-proves the identity
of a payload built from split=all partials against assembled_v3.
Emits the payload subset fit_7p1_wpa.py reads; exposure_factors are the
chain's (nd4cat: 7071 -> em06 fraction)."""
import argparse, json
from pathlib import Path
import numpy as np
import wpa_common as W

CELLS = ("D0", "D1", "D2")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--data-dir", required=True); ap.add_argument("--mc-dirs", required=True,
                    help="comma list of dirs searched in order for each MC label")
    ap.add_argument("--chain", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--compare", help="assembled_v3 JSON to compare against (split=all identity)")
    a = ap.parse_args()
    comps = W.DIFF8 + W.CHAINS[a.chain]["nd"]
    dirs = a.mc_dirs.split(",")

    def find(l):
        for d in dirs:
            if (Path(d) / l / "partial.json").exists():
                return W.load_partial(d, l), str(Path(d) / l / "partial.json")
        raise SystemExit("FATAL: no partial for " + l)
    dp = {k: W.load_partial(a.data_dir, k) for k in W.DATA_LABELS}
    data = {z: sum(np.asarray(dp[k]["dcell_eta"]["cutremoved"][z]["raw"], float) for k in dp).tolist() for z in CELLS}
    comp, src, fac = {}, {}, {}
    for l in comps:
        p, path = find(l); src[l] = {"path": path, "sha256": W.sha256(path),
                                     "split": p["wpa"]["split"], "removal": p["wpa"]["removal"]["applied"]}
        t = p["dcell_eta"]["cutremoved"]
        comp[l] = {"templates": {w: {z: t[z][w] for z in CELLS} for w in ("raw", "exposure1", "signed")}}
        fac[l] = W.fac_used(l, a.chain)
    splits = {p["wpa"]["split"] for p in dp.values()} | {s["split"] for s in src.values()}
    ok = len(splits) == 1 and all(p["conservation"]["checked_in_worker"] for p in dp.values())
    edges = dp["ep0304"]["dcell_eta"]["cutremoved"]["edges"]
    out = {"schema": "wpa-fit-payload-v1", "pass": ok, "conservation": {"all_pass": ok, "note": "in-worker I1-I4 closures per partial"},
           "chain": a.chain, "split": sorted(splits), "edges": edges,
           "scale_table_basis": {"exposure_factors": fac},
           "modes": {"cluster": {"data": {"templates": {"raw": data}, "stage_B_raw_total": int(sum(map(sum, data.values())))},
                                 "components": comp}},
           "inputs": {"data": {k: W.sha256(Path(a.data_dir) / k / "partial.json") for k in dp}, "mc": src}}
    if a.compare:
        ref = json.load(open(a.compare))["modes"]["cluster"]
        cmp = {"data_raw_exact": all(np.array_equal(np.asarray(data[z]), np.asarray(ref["data"]["templates"]["raw"][z], float)) for z in CELLS)}
        for l in comps:
            if l not in ref["components"]:
                continue
            r = ref["components"][l]["templates"]
            cmp[l] = {"raw_exact": all(np.array_equal(np.asarray(comp[l]["templates"]["raw"][z], float), np.asarray(r["raw"][z], float)) for z in CELLS),
                      "exposure1_maxrel": max(float(np.max(np.abs(np.asarray(comp[l]["templates"]["exposure1"][z]) - np.asarray(r["exposure1"][z])) /
                                                         np.maximum(np.abs(np.asarray(r["exposure1"][z])), 1e-12))) for z in CELLS)}
        out["compare_assembled_v3"] = cmp
        print(json.dumps(cmp))
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    json.dump(out, open(a.out, "w"))
    print(json.dumps({"out": a.out, "pass": ok, "splits": sorted(splits), "data_total": out["modes"]["cluster"]["data"]["stage_B_raw_total"]}))


if __name__ == "__main__":
    main()

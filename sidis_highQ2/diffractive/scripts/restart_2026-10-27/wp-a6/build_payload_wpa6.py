#!/usr/bin/env python3
"""WP-A6 S1 fit payload: WP-A2 wpa-fit-payload-v1 layout on the full ec04 grid linspace(-1,5,121) (no merging),
split per M_X class (MX4_10, MX10_20, MX20_40, MXout), plus sumw2, band totals and SEL counts.
Schema: wpa6_payload_schema.md (schema_ext wpa6-mx-v1).  Weights: exposure1 = skim w / table_fac, signed = skim w,
sumw2 = exposure1**2, raw = counts; data w = 1.  Skims are read-only (F.SK)."""
import argparse, json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[0] / "wp-a3")); sys.path.insert(0, str(HERE.parents[0] / "wp-a"))
import fact_wpa3 as F
import wpa_common as W

EDGES = np.linspace(-1, 5, 121); NB = 120
CELLS = ("D0", "D1", "D2"); CLS = ("MX4_10", "MX10_20", "MX20_40", "MXout"); WK = ("raw", "exposure1", "signed", "sumw2")
MODES = ("cluster", "cluster_xip04")


def mx_class(mx):
    f = np.isfinite(mx); c = np.full(mx.size, 3)
    for i, (lo, hi) in enumerate(((4, 10), (10, 20), (20, 40.0000001))):
        c[f & (mx >= lo) & (mx < hi)] = i
    return c


def block(x, cell, cls, m, ws):
    """templates[w][cell][cls] (120 floats) and oor[w][cell][cls] for the sample m; ws = {w: array}."""
    inr = np.isfinite(x) & (x >= -1) & (x <= 5)
    T = {w: {} for w in ws}; O = {w: {} for w in ws}
    for ci, cn in enumerate(CELLS):
        for w in ws: T[w][cn] = {}; O[w][cn] = {}
        for k, kn in enumerate(CLS):
            s = m & (cell == ci) & (cls == k)
            for w, v in ws.items():
                T[w][cn][kn] = np.histogram(x[s & inr], EDGES, weights=v[s & inr])[0].tolist()
                O[w][cn][kn] = int((s & ~inr).sum()) if w == "raw" else float(v[s & ~inr].sum())
    return T, O


def close(T, O, x, cell, m, e1, tag):
    for ci, cn in enumerate(CELLS):
        s = m & (cell == ci)
        n = sum(sum(T["raw"][cn][k]) + O["raw"][cn][k] for k in CLS)
        a = sum(sum(T["exposure1"][cn][k]) + O["exposure1"][cn][k] for k in CLS) if "exposure1" in T else float(e1[s].sum()); b = float(e1[s].sum())
        if round(n) != int(s.sum()) or abs(a - b) > 1e-9 * max(abs(b), 1e-300):
            raise SystemExit("FATAL: closure %s %s: raw %s vs %d, exposure1 %.12g vs %.12g" % (tag, cn, n, s.sum(), a, b))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--chain", required=True, choices=sorted(W.CHAINS)); ap.add_argument("--split", required=True, choices=("even", "odd"))
    ap.add_argument("--out", required=True)
    a = ap.parse_args(); even = a.split == "even"
    comps = W.DIFF8 + W.CHAINS[a.chain]["nd"]
    out = {"schema": "wpa-fit-payload-v1", "schema_ext": "wpa6-mx-v1", "pass": True,
           "conservation": {"all_pass": True, "note": "per label, mode, cell: sum over classes of raw (incl. oor) == mask&cell count; exposure1 within 1e-9"},
           "chain": a.chain, "split": [a.split], "variant": "wpa6_c04_fullgrid", "edges": EDGES.tolist(),
           "mx_classes": {"MX4_10": [4, 10], "MX10_20": [10, 20], "MX20_40": [20, 40], "MXout": "mx<4 or mx>40 or non-finite"},
           "cells": list(CELLS), "scale_table_basis": {"exposure_factors": {l: W.fac_used(l, a.chain) for l in comps}},
           "modes": {m: {"data": {"templates": {"raw": {c: {k: [0.0] * NB for k in CLS} for c in CELLS}}, "oor": {c: {k: 0 for k in CLS} for c in CELLS}},
                         "components": {}} for m in MODES},
           "band_3p0_3p4": {}, "band_3p4_5": {}, "sel": {"data": {"templates": {"raw": {c: {k: [0.0] * NB for k in CLS} for c in CELLS}}, "oor": {c: {k: 0 for k in CLS} for c in CELLS}}, "components": {}},
           "outside_cells": {m: {} for m in MODES + ("sel",)},
           "diagnostics": {"ec08_sentinel_count": {}, "n_fit": {}, "n_sel": {}, "n_sel_cells": {}}, "inputs": {}, "script_sha256": W.sha256(__file__)}
    for l in W.DATA_LABELS + comps:
        out["inputs"][l] = {"path": str(F.SK / (l + ".npz")), "sha256": W.sha256(F.SK / (l + ".npz"))}

    def samples(e):
        mk = F.masks(e, even); fit = mk["FIT"]
        return {"cluster": fit, "cluster_xip04": fit & (e["xip"] < 0.04), "sel": mk["SB"] & (e["xip"] < 0.04) & (e["mx"] > 3)}, mk

    for l in W.DATA_LABELS + comps:
        e = F.load(l); S, mk = samples(e); cell = e["cell"]; cls = mx_class(e["mx"]); x = e["ec04"]
        isd = l in W.DATA_LABELS
        if isd: e1 = np.ones(x.size)
        else: e1 = e["w"] / W.table_fac(l)
        ws = {"raw": np.ones(x.size)} if isd else {"raw": np.ones(x.size), "exposure1": e1, "signed": e["w"], "sumw2": e1 ** 2}
        sent = e["ec08"] < -900
        out["diagnostics"]["ec08_sentinel_count"][l] = {"fit": int((sent & mk["FIT"]).sum()), "sel": int((sent & S["sel"]).sum())}
        out["diagnostics"]["n_fit"][l] = int(mk["FIT"].sum()); out["diagnostics"]["n_sel"][l] = int(S["sel"].sum())
        out["diagnostics"]["n_sel_cells"][l] = [int((S["sel"] & (cell == c)).sum()) for c in range(3)]
        for md, m in S.items():
            T, O = block(x, cell, cls, m, ws)
            close(T, O, x, cell, m, e1, l + "/" + md)
            out["outside_cells"][md][l] = int((m & (cell == -1)).sum())
            tgt = out[md] if md == "sel" else out["modes"][md]
            if isd:
                for c in CELLS:
                    for k in CLS:
                        tgt["data"]["templates"]["raw"][c][k] = (np.array(tgt["data"]["templates"]["raw"][c][k]) + T["raw"][c][k]).tolist()
                        tgt["data"]["oor"][c][k] += O["raw"][c][k]
            else:
                tgt["components"][l] = {"templates": T, "oor": O}
        del e
    for md in MODES:
        for nm, lo, hi in (("band_3p0_3p4", 3.0, 3.4), ("band_3p4_5", 3.4, 5.0000001)):
            d = out["modes"][md]["data"]["templates"]["raw"]; i0, i1 = int(round((lo + 1) / 0.05)), min(NB, int(round((hi + 1) / 0.05)))
            b = {"data": {c: {k: float(sum(d[c][k][i0:i1])) for k in CLS} for c in CELLS}}
            for l in comps:
                T = out["modes"][md]["components"][l]["templates"]
                b[l] = {w: {c: {k: float(sum(T[w][c][k][i0:i1])) for k in CLS} for c in CELLS} for w in ("raw", "exposure1", "sumw2")}
            out[nm][md] = b
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(out, open(a.out, "w"))
    print(json.dumps({"out": a.out, "n_fit_data": sum(out["diagnostics"]["n_fit"][l] for l in W.DATA_LABELS),
                      "n_sel_data": sum(out["diagnostics"]["n_sel"][l] for l in W.DATA_LABELS)}))


if __name__ == "__main__":
    main()

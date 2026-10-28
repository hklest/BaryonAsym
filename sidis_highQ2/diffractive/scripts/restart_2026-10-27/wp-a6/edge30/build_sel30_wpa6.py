#!/usr/bin/env python3
"""WP-A6 edge30 (user D15, 2026-10-28): event-level SEL histograms in etaMaxClus04 at the selection edge etaMaxClus08 < 3.0.
Writes, per (chain, split), the frozen-payload `sel` block (SB: ec08 < 3.3) recomputed as a closure check, and the
`sel30` block (SB30 = SB & (ec08 < 3.0 or sentinel)), both with x_IP < 0.04 and M_X > 3, in the wpa6-mx-v1 layout of
build_payload_wpa6.py (whose block/close/mx_class are reused unchanged).  Skims are read-only."""
import argparse, json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent)); sys.path.insert(0, str(HERE.parents[1] / "wp-a3")); sys.path.insert(0, str(HERE.parents[1] / "wp-a"))
import build_payload_wpa6 as B
import fact_wpa3 as F
import wpa_common as W

EDGE = 3.0
KEYS = ("sel", "sel30")


def empty():
    return {"data": {"templates": {"raw": {c: {k: [0.0] * B.NB for k in B.CLS} for c in B.CELLS}}, "oor": {c: {k: 0 for k in B.CLS} for c in B.CELLS}}, "components": {}}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--chain", required=True, choices=sorted(W.CHAINS)); ap.add_argument("--split", required=True, choices=("even", "odd"))
    ap.add_argument("--out", required=True)
    a = ap.parse_args(); even = a.split == "even"
    comps = W.DIFF8 + W.CHAINS[a.chain]["nd"] + [l for l in ("6671", "6677") if l not in W.DIFF8 + W.CHAINS[a.chain]["nd"]]
    out = {"schema_ext": "wpa6-mx-v1-edge30", "chain": a.chain, "split": a.split, "edge_ec08": EDGE, "edges": B.EDGES.tolist(),
           "scale_table_basis": {"exposure_factors": {l: W.fac_used(l, a.chain) for l in comps}},
           "sel": empty(), "sel30": empty(), "diagnostics": {"n_sel": {}, "n_sel30": {}, "n_sel30_sentinel": {}},
           "inputs": {}, "script_sha256": W.sha256(__file__), "builder_sha256": W.sha256(B.__file__)}
    for l in W.DATA_LABELS + comps:
        out["inputs"][l] = {"path": str(F.SK / (l + ".npz")), "sha256": W.sha256(F.SK / (l + ".npz"))}
    for l in W.DATA_LABELS + comps:
        e = F.load(l); mk = F.masks(e, even); cell = e["cell"]; cls = B.mx_class(e["mx"]); x = e["ec04"]; ec = e["ec08"]
        sel = mk["SB"] & (e["xip"] < 0.04) & (e["mx"] > 3)
        S = {"sel": sel, "sel30": sel & ((ec < EDGE) | (ec < -900))}
        isd = l in W.DATA_LABELS
        e1 = np.ones(x.size) if isd else e["w"] / W.table_fac(l)
        ws = {"raw": np.ones(x.size)} if isd else {"raw": np.ones(x.size), "exposure1": e1, "signed": e["w"], "sumw2": e1 ** 2}
        out["diagnostics"]["n_sel"][l] = int(S["sel"].sum()); out["diagnostics"]["n_sel30"][l] = int(S["sel30"].sum())
        out["diagnostics"]["n_sel30_sentinel"][l] = int((S["sel30"] & (ec < -900)).sum())
        for md, m in S.items():
            T, O = B.block(x, cell, cls, m, ws); B.close(T, O, x, cell, m, e1, l + "/" + md)
            tgt = out[md]
            if isd:
                for c in B.CELLS:
                    for k in B.CLS:
                        tgt["data"]["templates"]["raw"][c][k] = (np.array(tgt["data"]["templates"]["raw"][c][k]) + T["raw"][c][k]).tolist()
                        tgt["data"]["oor"][c][k] += O["raw"][c][k]
            else:
                tgt["components"][l] = {"templates": T, "oor": O}
        del e
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(out, open(a.out, "w"))
    print(json.dumps({"out": a.out, "n_sel_data": sum(out["diagnostics"]["n_sel"][l] for l in W.DATA_LABELS),
                      "n_sel30_data": sum(out["diagnostics"]["n_sel30"][l] for l in W.DATA_LABELS)}))


if __name__ == "__main__":
    main()

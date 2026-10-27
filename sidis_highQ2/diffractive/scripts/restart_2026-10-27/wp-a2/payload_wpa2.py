#!/usr/bin/env python3
"""WP-A2 §7.1 fit payload from n1_wpa2 partials (fork of wp-a/payload_wpa.py; child
plan wp-a2_plan.md D9, S3/S4).  Reads D/data/.../wp-a2/s2_diag/partials/<label>/<split>/
and emits the payload schema fit_7p1_wpa2.py reads (wpa-fit-payload-v1).

Flags (plan Outline):
  --split even|odd        which split's partials (D4)
  --empz                  35 < E-p_z < 70 (H3 ii)
  --cand                  + x_IP < 0.04 && M_X > 3 GeV (H3 iii; requires --empz)
  --eta-branch etaMaxClus08|etaMaxClus04   (H1 threshold variant)
  --eta-max-cut 3.4 --edge-merge           (H4 fit-range correction: bins below the
                          cut kept, all bins from the cut to 5 merged into one)
The template block is wpa2.dcell_eta_variants[cutremoved[_empz[_cand]]_c08|c04];
with no flags it equals dcell_eta["cutremoved"] (checked, --check-baseline)."""
import argparse, json, sys
from pathlib import Path
import numpy as np
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "wp-a"))
import wpa_common as W

CELLS = ("D0", "D1", "D2")
WK = ("raw", "exposure1", "signed")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", required=True); ap.add_argument("--split", required=True, choices=("even", "odd"))
    ap.add_argument("--chain", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--empz", action="store_true"); ap.add_argument("--cand", action="store_true")
    ap.add_argument("--eta-branch", default="etaMaxClus08", choices=("etaMaxClus08", "etaMaxClus04"))
    ap.add_argument("--eta-max-cut", type=float); ap.add_argument("--edge-merge", action="store_true")
    ap.add_argument("--check-baseline", action="store_true")
    a = ap.parse_args()
    if a.cand and not a.empz:
        raise SystemExit("FATAL: --cand requires --empz")
    if (a.eta_max_cut is None) != (not a.edge_merge):
        raise SystemExit("FATAL: --eta-max-cut and --edge-merge go together")
    vname = "cutremoved" + ("_empz" if a.empz else "") + ("_cand" if a.cand else "") + ("_c08" if a.eta_branch == "etaMaxClus08" else "_c04")
    root = Path(a.root)

    def part(l):
        p = root / "partials" / l / a.split / "partial.json"
        if not p.exists():
            raise SystemExit("FATAL: missing " + str(p))
        x = json.load(open(p))
        if x["wpa2"]["split"] != a.split:
            raise SystemExit("FATAL: split mismatch " + str(p))
        return x, p
    edges = np.linspace(-1, 5, 121)
    if a.edge_merge:
        kc = int(np.searchsorted(edges, a.eta_max_cut - 1e-9))
        if abs(edges[kc] - a.eta_max_cut) > 1e-9:
            raise SystemExit("FATAL: cut not on the grid")
        new_edges = np.concatenate([edges[:kc + 1], [edges[-1]]])
        reb = lambda h: np.concatenate([np.asarray(h, float)[:kc], [np.asarray(h, float)[kc:].sum()]])
    else:
        new_edges = edges; reb = lambda h: np.asarray(h, float)
    comps = W.DIFF8 + W.CHAINS[a.chain]["nd"]
    dp = {k: part(k) for k in W.DATA_LABELS}
    for k, (x, _) in dp.items():
        if not x["conservation"]["checked_in_worker"]:
            raise SystemExit("FATAL: conservation " + k)
    data = {z: sum(reb(dp[k][0]["wpa2"]["dcell_eta_variants"][vname][z]["raw"]) for k in dp).tolist() for z in CELLS}
    comp, src, fac, chk = {}, {}, {}, {}
    for l in comps:
        x, p = part(l)
        t = x["wpa2"]["dcell_eta_variants"][vname]
        comp[l] = {"templates": {w: {z: reb(t[z][w]).tolist() for z in CELLS} for w in WK}}
        src[l] = {"path": str(p), "sha256": W.sha256(p), "split": a.split, "checked": bool(x["conservation"]["checked_in_worker"])}
        fac[l] = W.fac_used(l, a.chain)
        if a.check_baseline:
            chk[l] = all(np.array_equal(np.asarray(t[z][w]), np.asarray(x["dcell_eta"]["cutremoved"][z][w])) for z in CELLS for w in WK)
    if a.check_baseline:
        for k in dp:
            chk[k] = all(np.array_equal(np.asarray(dp[k][0]["wpa2"]["dcell_eta_variants"]["cutremoved_c08"][z]["raw"]),
                                        np.asarray(dp[k][0]["dcell_eta"]["cutremoved"][z]["raw"])) for z in CELLS)
    ok = all(x["conservation"]["checked_in_worker"] for x, _ in dp.values()) and all(v["checked"] for v in src.values())
    out = {"schema": "wpa-fit-payload-v1", "pass": ok, "conservation": {"all_pass": ok, "note": "in-worker I1-I4 and wpa2 closures per partial (data and MC)"},
           "chain": a.chain, "split": [a.split], "variant": vname,
           "flags": {"empz": a.empz, "cand": a.cand, "eta_branch": a.eta_branch, "eta_max_cut": a.eta_max_cut, "edge_merge": a.edge_merge},
           "edges": new_edges.tolist(), "scale_table_basis": {"exposure_factors": fac},
           "modes": {"cluster": {"data": {"templates": {"raw": data}, "stage_B_raw_total": int(sum(map(sum, data.values())))}, "components": comp}},
           "inputs": {"data": {k: W.sha256(p) for k, (_, p) in dp.items()}, "mc": src}}
    if a.check_baseline:
        out["baseline_identity"] = chk
        if not all(chk.values()):
            raise SystemExit("FATAL: baseline identity failed %s" % chk)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    json.dump(out, open(a.out, "w"))
    print(json.dumps({"out": a.out, "variant": vname, "n_bins": len(new_edges) - 1, "data_total": out["modes"]["cluster"]["data"]["stage_B_raw_total"],
                      "baseline_identity": all(chk.values()) if chk else None}))


if __name__ == "__main__":
    main()

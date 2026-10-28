#!/usr/bin/env python3
"""Gate-evidence histograms (restart 2026-10-27): one Condor job over the 17 WP-A3 skims
(data/restart_2026-10-27/wp-a3/skims/*.npz, which reproduce the WP-A2 S4 / G1' stack-C N-1
samples exactly; test skim_identity_s4 in fact_wpa3.py, re-run here).

No gate is re-evaluated.  Selections, weights and the veto-state map are those of
scripts/restart_2026-10-27/wp-a3/fact_wpa3.py (imported, not copied):
  samples  F.masks(): N1_PLUG / N1_FMD / N1_FTS (A_pre && eta08<3.3 && other vetoes),
           FIT (A_pre && selFmd && ePlug<7 && selFts, no eta cut), H4 (A_pre && 35<E-pz<70, no veto),
           H4E = H4 && ePlug>20 GeV (the WP-A2 S2 H4 edge sample).
  weights  signed v3 x chain exposure rescale (fac_used/table_fac), cocktail DIFF8 + chain ND.
  map      WP-A3 run-2 joint veto-state map (fine Plug classes) on x-set XA (D cell), derived on
           CTRL even, applied per event as w' = w * r_s(cell) (for XA the period sum is trivial).
Output: one JSON of per-group (data / signal6 / reggeon2 / nd) sumw and sumw2 histograms per
sample, variable, parity, cell (D0, D1, D2, ALL = all events), chain and map state, plus the
cross-checks against fact_results_fine.json."""
import argparse, json, math, sys
from pathlib import Path
import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a3")); sys.path.insert(0, str(HERE.parent / "wp-a"))
import fact_wpa3 as F
import wpa_common as W

CHAINS = ("7071", "7094", "7091", "nd4cat")
ETA = np.round(np.linspace(-1, 5, 121), 6)
SPECS = [  # sample, variable, edges, parities, mapped too
    ("N1_PLUG", "eplug", F.G1E, ("odd",), True),
    ("N1_FMD", "nfmu12", F.IE, ("odd",), True),
    ("N1_FTS", "nfts28", F.IE, ("odd",), True),
    ("FIT", "ec04", ETA, ("odd", "even"), True),
    ("FIT", "ec08", ETA, ("odd", "even"), True),
    ("H4", "ec08", ETA, ("odd", "even"), False),
    ("H4E", "ec08", ETA, ("odd", "even"), False),
]
GROUPS = {"signal": W.SIGNAL, "reggeon": ["6671", "6677"]}


def h(x, edges, w):
    fin = np.isfinite(x)
    s = np.histogram(x[fin], edges, weights=w[fin])[0]; s2 = np.histogram(x[fin], edges, weights=(w ** 2)[fin])[0]
    over = float(w[fin & (x >= edges[-1])].sum()); nonfin = float(w[~fin].sum())
    return {"sumw": s.tolist(), "sumw2": s2.tolist(), "overflow": over, "nonfinite": nonfin}


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True); a = ap.parse_args()
    F.set_classes("fine")
    labs = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    E = {l: F.load(l) for l in W.DATA_LABELS + labs}
    MK = {(l, p): F.masks(e, p == "even") for l, e in E.items() for p in ("even", "odd")}
    for l, e in E.items():
        for p in ("even", "odd"):
            MK[(l, p)]["H4E"] = MK[(l, p)]["H4"] & np.isfinite(e["eplug"]) & (e["eplug"] > 20.)
    ST = {l: F.state(e) for l, e in E.items()}
    CI = {l: np.where(e["cell"] >= 0, e["cell"], 3).astype(int) for l, e in E.items()}
    res = {"script_sha256": W.sha256(__file__), "fact_sha256": W.sha256(F.__file__),
           "inputs": {l: W.sha256(F.SK / (l + ".npz")) for l in E}, "plug_edges_map": [float(x) for x in F.PL_E],
           "specs": [[s, v, list(map(float, ed)), list(p), m] for s, v, ed, p, m in SPECS], "hists": {}, "checks": {}}

    # skim identity vs WP-A2 S4 partials (as fact_wpa3.py)
    bad = []
    for l, e in E.items():
        kind = "data" if l in W.DATA_LABELS else "mc"; pj = json.load(open(F.S4 / kind / l / "partial.json"))
        key = "Dcells_raw" if kind == "data" else "Dcells_signed"; mk = MK[(l, "odd")]
        for mn, pm in (("N1_PLUG", "C_N1_PLUG"), ("SB", "C_SB")):
            got = [float((e["w"] if kind == "mc" else np.ones(e["w"].size))[mk[mn] & (e["cell"] == c)].sum()) for c in range(3)]
            if not np.allclose(got, pj["counts"][pm][key], rtol=1e-9, atol=1e-9): bad.append([l, mn])
    res["checks"]["skim_identity_s4"] = {"PASS": not bad, "mismatch": bad}
    if bad: raise SystemExit("FATAL: skim identity " + str(bad))

    def cellmask(l, cell):
        return np.ones(E[l]["w"].size, bool) if cell == "ALL" else (E[l]["cell"] == int(cell[1]))

    # data histograms (chain independent)
    for s, v, ed, pars, _ in SPECS:
        for p in pars:
            for cell in ("D0", "D1", "D2", "ALL"):
                acc = None
                for l in W.DATA_LABELS:
                    m = MK[(l, p)][s] & cellmask(l, cell)
                    hh = h(E[l][v][m], ed, np.ones(int(m.sum())))
                    acc = hh if acc is None else {k: (np.add(acc[k], hh[k]).tolist() if isinstance(hh[k], list) else acc[k] + hh[k]) for k in hh}
                res["hists"]["|".join((s, v, p, cell, "data", "data", "none"))] = acc

    ref = json.load(open(W.D / "data/restart_2026-10-27/wp-a3/fact/fact_results_fine.json"))
    for ch in CHAINS:
        nd = W.CHAINS[ch]["nd"]; comps = W.DIFF8 + nd
        rs = {l: W.fac_used(l, ch) / W.table_fac(l) for l in comps}
        base = {l: rs[l] * E[l]["w"] for l in comps}
        # XA map on CTRL even (fact_wpa3 tables/rmap for xs = XA)
        Nd = np.zeros((F.NS, 4)); Wm = np.zeros((F.NS, 4)); Nm = np.zeros((F.NS, 4))
        for l in W.DATA_LABELS:
            m = MK[(l, "even")]["CTRL"]; np.add.at(Nd, (ST[l][m], CI[l][m]), 1.0)
        for l in comps:
            m = MK[(l, "even")]["CTRL"]; np.add.at(Wm, (ST[l][m], CI[l][m]), base[l][m]); np.add.at(Nm, (ST[l][m], CI[l][m]), 1.0)
        Pd = Nd / np.maximum(Nd.sum(0), 1e-300); Pm = Wm / np.maximum(Wm.sum(0), 1e-300)
        R = np.where(Wm > 0, Pd / np.where(Pm > 0, Pm, 1), 1.0)
        thin = (Nd.sum(0) < 50) | (Nm.sum(0) < 200); R[:, thin] = 1.0
        mapped = {l: base[l] * R[ST[l], CI[l]] for l in comps}
        grp = dict(GROUPS, nd=nd)
        for s, v, ed, pars, domap in SPECS:
            for p in pars:
                for cell in ("D0", "D1", "D2", "ALL"):
                    for mp, WW in (("none", base), ("XA", mapped)):
                        if mp == "XA" and not domap: continue
                        for g, ls in grp.items():
                            acc = None
                            for l in ls:
                                m = MK[(l, p)][s] & cellmask(l, cell)
                                hh = h(E[l][v][m], ed, WW[l][m])
                                acc = hh if acc is None else {k: (np.add(acc[k], hh[k]).tolist() if isinstance(hh[k], list) else acc[k] + hh[k]) for k in hh}
                            acc["raw"] = int(sum(int((MK[(l, p)][s] & cellmask(l, cell)).sum()) for l in ls))
                            res["hists"]["|".join((s, v, p, cell, ch, g, mp))] = acc
        # cross-check f_fail (Plug N-1, odd) against fact_results_fine.json
        chk = {}
        for mp, WW, rk in (("none", base, ref["chains"].get(ch, {}).get("unmapped")), ("XA", mapped, ref["chains"].get(ch, {}).get("maps", {}).get("XA"))):
            for c in range(3):
                dn = sum(int((MK[(l, "odd")]["N1_PLUG"] & (E[l]["cell"] == c)).sum()) for l in W.DATA_LABELS)
                dp = sum(int((MK[(l, "odd")]["N1_PLUG"] & (E[l]["cell"] == c) & (E[l]["eplug"] < 7)).sum()) for l in W.DATA_LABELS)
                mn = sum(WW[l][MK[(l, "odd")]["N1_PLUG"] & (E[l]["cell"] == c)].sum() for l in comps)
                mpp = sum(WW[l][MK[(l, "odd")]["N1_PLUG"] & (E[l]["cell"] == c) & (E[l]["eplug"] < 7)].sum() for l in comps)
                d = (1 - mpp / mn) - (1 - dp / dn)
                r_ = rk["N1_PLUG"]["f_fail"]["D%d" % c]["delta"] if rk else None
                chk["%s_D%d" % (mp, c)] = {"delta": float(d), "ref": r_, "match": bool((r_ is None) or abs(d - r_) < 1e-9)}
        res["checks"]["ffail_" + ch] = chk
        print(ch, json.dumps({k: round(v["delta"], 4) for k, v in chk.items()}), "match", all(v["match"] for v in chk.values()), flush=True)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(res, open(a.out, "w"), default=lambda o: o.item() if hasattr(o, "item") else str(o))
    print(json.dumps({"out": a.out, "n_hists": len(res["hists"]), "skim_identity_s4": True}))


if __name__ == "__main__":
    main()

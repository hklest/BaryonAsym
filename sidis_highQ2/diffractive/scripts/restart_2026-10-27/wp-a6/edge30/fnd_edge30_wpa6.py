#!/usr/bin/env python3
"""WP-A6 edge30 (user D15): f_ND per M_X bin and G2 at the selection edge etaMaxClus08 < 3.0 (SB30), from the frozen odd and
even nominal unit fits (no refit) and the edge30 SEL payloads (build_sel30_wpa6.py).  Reuses fnd_wpa6.resolve_all / fnd /
edge unchanged; rho for delta_edge comes from the frozen payloads' `cluster` mode (the fit sample is unchanged).
Closure: the `sel` (SB, ec08 < 3.3) rows must reproduce the frozen f_nd_per_mx.csv.  Login node, JSON/CSV only.
Usage: fnd_edge30_wpa6.py"""
import csv, json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import fnd_wpa6 as FN
W = FN.W
O = FN.O; E30 = O / "edge30"
CELLS, MX, CLS, CHAINS, NIN, CEIL = FN.CELLS, FN.MX, FN.CLS, FN.CHAINS, FN.NIN, FN.CEIL
EXTRA = ["sel", "n_data_SB", "n_data_band", "frac_data_band", "diff_band_frac", "nd_band_frac", "rho_max"]


def counts(split, chain, key):
    """fnd_wpa6.counts with the SEL block taken from the edge30 payload (`key` = sel | sel30); cluster edge from the frozen payload."""
    q = json.load(open(E30 / f"payloads/{split}_{chain}_sel30.json")); p = json.load(open(O / f"payloads/{split}_{chain}.json"))
    fac = p["scale_table_basis"]["exposure_factors"]; nd, S, cl = W.CHAINS[chain]["nd"], q[key], p["modes"]["cluster"]
    arr = FN.arr

    def part(blk, lab, kind, c, m):
        t = arr(blk["components"][lab]["templates"][kind], c, m); o = blk["components"][lab]["oor"][kind][c][m]
        return t[:NIN].sum(), t[NIN:].sum() + o
    N = {}
    for c in CELLS:
        for m in CLS:
            dt = arr(S["data"]["templates"]["raw"], c, m)
            r = dict(n_data=dt.sum() + S["data"]["oor"][c][m], data_band=dt[NIN:].sum() + S["data"]["oor"][c][m],
                     nd_in=0., nd_band=0., w2_band=0., reg=0., df_in=0., df_band=0.)
            for lab in nd:
                a, b = part(S, lab, "exposure1", c, m); r["nd_in"] += a * fac[lab]; r["nd_band"] += b * fac[lab]
                r["w2_band"] += part(S, lab, "sumw2", c, m)[1] * fac[lab] ** 2
            for lab in FN.REG:
                a, b = part(S, lab, "exposure1", c, m); r["reg"] += (a + b) * fac[lab]
            for lab in W.DIFF8:
                a, b = part(S, lab, "exposure1", c, m); r["df_in"] += a * fac[lab]; r["df_band"] += b * fac[lab]
            ed = lambda lab: arr(cl["components"][lab]["templates"]["exposure1"], c, m)[NIN:NIN + 8] * fac[lab]
            r["edge_data"] = arr(cl["data"]["templates"]["raw"], c, m)[NIN:NIN + 8]
            r["edge_D"] = sum(ed(l) for l in W.DIFF8); r["edge_N"] = sum(ed(l) for l in nd)
            N[c, m] = r
    return N, np.asarray(p["edges"], float)


def rows_for(split, key, nSB):
    F, ed = {}, []
    for ch in CHAINS:
        R = FN.resolve_all(split, "nominal", ch, CELLS + ["ALL"])
        if R is None: raise SystemExit(f"FATAL: missing frozen fits {split} {ch}")
        N, edges = counts(split, ch, key)
        for m in MX:
            x = dict(FN.fnd(N, R, m), nfb=sum(R[c, m]["source"] == "fallback_all" for c in CELLS))
            rho = {c: FN.edge(N, R, edges, ch, m, c, ed) for c in CELLS}
            x["delta_edge"] = sum(N[c, m]["nd_band"] * R[c, m]["s_ND"] / x["n_data_sel"] * rho[c] for c in CELLS)
            x["rho_max"] = max(rho.values())
            db = sum(N[c, m]["data_band"] for c in CELLS); dfi = sum(R[c, m]["s_diff"] * N[c, m]["df_in"] for c in CELLS)
            dfb = sum(R[c, m]["s_diff"] * N[c, m]["df_band"] for c in CELLS)
            ndi = sum(R[c, m]["s_ND"] * N[c, m]["nd_in"] for c in CELLS); ndb = sum(R[c, m]["s_ND"] * N[c, m]["nd_band"] for c in CELLS)
            x.update(n_data_band=db, frac_data_band=db / x["n_data_sel"], diff_band_frac=dfb / (dfi + dfb) if dfi + dfb > 0 else 0.0,
                     nd_band_frac=ndb / (ndi + ndb) if ndi + ndb > 0 else 0.0)
            F[ch, m] = x
    rows = []
    for m in MX:
        spread = max(abs(F[ch, m]["f_nd"] - F["7071", m]["f_nd"]) for ch in CHAINS); big = max(CHAINS, key=lambda ch: F[ch, m]["f_nd"])
        for ch in CHAINS:
            x = F[ch, m]; g2 = (x["sig_fit"] ** 2 + spread ** 2) ** .5; e = x["delta_edge"]
            rows.append(dict(x, split=split, sel=key, mx_bin=m, chain=ch, spread=spread, sig_g2=g2, largest_chain=big, above_ceiling=x["f_nd"] > CEIL,
                             masked_G2=F[big, m]["f_nd"] > CEIL, flip_with_edge=(x["f_nd"] <= CEIL) != (x["f_nd"] + e <= CEIL),
                             g2_plus_sigma_flag=x["f_nd"] + g2 > CEIL, n_fallback_cells=x["nfb"], n_data_SB=nSB.get(m, "")))
    return rows


def main():
    frozen = {(r["split"], r["mx_bin"], r["chain"]): r for r in csv.DictReader(open(O / "f_nd_per_mx.csv"))}
    out, worst = [], 0.0
    for sp in ("even", "odd"):
        sb = rows_for(sp, "sel", {})
        for r in sb:
            f = frozen[sp, r["mx_bin"], r["chain"]]
            for k in ("f_nd", "f_nd_infit", "f_nd_band", "sig_fit", "delta_edge", "f_reg", "n_data_sel"):
                worst = max(worst, abs(float(f[k]) - float(r[k])) / max(abs(float(f[k])), 1e-12))
            if str(r["masked_G2"]) != f["masked_G2"]: raise SystemExit(f"FATAL: closure masked_G2 {sp} {r['mx_bin']} {r['chain']}")
        nSB = {r["mx_bin"]: r["n_data_sel"] for r in sb}
        out += [dict(r, n_data_SB=nSB[r["mx_bin"]]) for r in sb] + rows_for(sp, "sel30", nSB)
    if worst > 1e-9: raise SystemExit(f"FATAL: SB closure vs frozen f_nd_per_mx.csv: max rel diff {worst:.3g}")
    print(f"CLOSURE SB vs frozen f_nd_per_mx.csv: max rel diff {worst:.3g} (pass)")
    E30.mkdir(parents=True, exist_ok=True)
    FN.wcsv(E30 / "f_nd_per_mx_edge30.csv", ["split", "sel"] + FN.COLS[1:] + EXTRA[1:],
            sorted(out, key=lambda r: (r["split"], r["sel"], r["mx_bin"], r["chain"])))
    g2csv = [dict(r, split=r["split"]) for r in out if r["sel"] == "sel30"]
    FN.wcsv(E30 / "f_nd_per_mx_SB30.csv", FN.COLS, sorted(g2csv, key=lambda r: (r["split"], r["mx_bin"], r["chain"])))
    for r in out:
        if r["sel"] == "sel30":
            print(f"{r['split']:4s} {r['mx_bin']:8s} {r['chain']:7s} N30={r['n_data_sel']:.0f}/NSB={r['n_data_SB']:.0f} f_nd={r['f_nd']:.4f} in={r['f_nd_infit']:.4f} "
                  f"band={r['f_nd_band']:.4f} sig_g2={r['sig_g2']:.4f} dEdge={r['delta_edge']:.4f} dband={r['frac_data_band']:.3f} "
                  f"Dband={r['diff_band_frac']:.3f} NDband={r['nd_band_frac']:.3f} big={r['largest_chain']} masked={r['masked_G2']} flip={r['flip_with_edge']}")


if __name__ == "__main__":
    main()

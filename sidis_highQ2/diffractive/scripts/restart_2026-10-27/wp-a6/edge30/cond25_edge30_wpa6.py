#!/usr/bin/env python3
"""WP-A6 edge30 conditions 2 and 5 (critic ruling on D15), no refit, login node, JSON/CSV only.
Cond 2: f_ND per M_X bin at SB30 (ec08 < 3.0) from the frozen even-split variant fits (nobb, lo1p0, shared) + nominal,
        via fnd_edge30_wpa6.counts('sel30') + fnd_wpa6.resolve_all/fnd.  Flag largest-chain f_ND > 0.30.
Cond 5: rho re-derived on the residual band (ec08 < 3.0 and ec04 >= 3.0) from the sel30 payloads: all band bins (ec04 >= 3.0)
        with >= 20 data and pred > 0, rho = max|data/pred - 1| with nominal resolved scales (same definition as fnd_wpa6.edge);
        also the first-8-bin value.  delta_edge with per-chain rho and with chain-independent rho (max over chains, per cell).
Usage: cond25_edge30_wpa6.py"""
import sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE)); sys.path.insert(0, str(HERE.parent))
import fnd_edge30_wpa6 as E
import fnd_wpa6 as FN
W, O, E30 = FN.W, FN.O, E.E30
CELLS, MX, CHAINS, NIN, CEIL = FN.CELLS, FN.MX, FN.CHAINS, FN.NIN, FN.CEIL
import json


def band_counts(split, chain):
    q = json.load(open(E30 / f"payloads/{split}_{chain}_sel30.json")); S = q["sel30"]; fac = q["scale_table_basis"]["exposure_factors"]
    nd = W.CHAINS[chain]["nd"]; out = {}
    for c in CELLS:
        for m in MX:
            g = lambda lab: np.asarray(S["components"][lab]["templates"]["exposure1"][c][m], float)[NIN:] * fac[lab]
            out[c, m] = dict(data=np.asarray(S["data"]["templates"]["raw"][c][m], float)[NIN:], D=sum(g(l) for l in W.DIFF8), N=sum(g(l) for l in nd))
    return out, np.asarray(q["edges"], float)


def rho_of(dat, pred):
    use = (dat >= 20) & (pred > 0)
    if use.any(): return float(np.max(np.abs(dat[use] / pred[use] - 1))), int(use.sum()), False
    return (float(abs(dat.sum() / pred.sum() - 1)) if pred.sum() > 0 else 0.0), 0, True


def main():
    # ---- condition 2
    nomrows = {(r["mx_bin"], r["chain"]): r for r in FN.csv.DictReader(open(E30 / "f_nd_per_mx_SB30.csv")) if r["split"] == "even"}
    st = []
    for tag in ["nominal", "nobb", "lo1p0", "shared"]:
        F = {}
        for ch in CHAINS:
            R = FN.resolve_all("even", tag, ch, CELLS)
            N, _ = E.counts("even", ch, "sel30")
            for m in MX: F[ch, m] = dict(FN.fnd(N, R, m), nfb=sum(R[c, m]["source"] == "fallback_all" for c in CELLS))
        for m in MX:
            big = max(CHAINS, key=lambda ch: F[ch, m]["f_nd"]); nomf = {ch: float(nomrows[m, ch]["f_nd"]) for ch in CHAINS}
            for ch in CHAINS:
                x = F[ch, m]
                st.append(dict(tag=tag, mx_bin=m, chain=ch, f_nd=x["f_nd"], f_nd_infit=x["f_nd_infit"], f_nd_band=x["f_nd_band"], f_nd_nominal_SB30=nomf[ch],
                               shift=x["f_nd"] - nomf[ch], n_fallback_cells=x["nfb"], largest_chain=big, is_largest=ch == big,
                               above_ceiling=x["f_nd"] > CEIL, flag_largest_gt_0p30=(ch == big and x["f_nd"] > CEIL)))
    FN.wcsv(E30 / "stability_SB30_even.csv", list(st[0]), st)
    # ---- condition 5
    rr, dd = [], []
    for sp in ("odd", "even"):
        R = {}; BC = {}; Nc = {}; NS = {}
        for ch in CHAINS:
            R[ch] = FN.resolve_all(sp, "nominal", ch, CELLS + ["ALL"]); BC[ch], edges = band_counts(sp, ch); Nc[ch], _ = E.counts(sp, ch, "sel30")
            for m in MX: NS[ch, m] = FN.fnd(Nc[ch], R[ch], m)
        rho, cl_rho = {}, {}
        for ch in CHAINS:
            for m in MX:
                for c in CELLS:
                    b, g = BC[ch][c, m], R[ch][c, m]; pred = g["s_diff"] * b["D"] + g["s_ND"] * b["N"]; r, nb, fbk = rho_of(b["data"], pred)
                    r8, nb8, fb8 = rho_of(b["data"][:8], pred[:8]); rho[ch, m, c] = r
                    for i in range(len(pred)):
                        if b["data"][i] > 0 or pred[i] > 0:
                            rr.append(dict(split=sp, chain=ch, mx_bin=m, cell=c, lo=edges[NIN + i], hi=edges[NIN + i + 1], data=b["data"][i], prediction=pred[i],
                                           ratio=b["data"][i] / pred[i] if pred[i] > 0 else "", used_for_rho=int(b["data"][i] >= 20 and pred[i] > 0), rho_band=r, rho_first8=r8,
                                           n_bins_used=nb, class_summed_fallback=fbk))
        for m in MX:
            for ch in CHAINS:
                x = NS[ch, m]; n = x["n_data_sel"]
                rho_cl = {c: FN.edge(Nc[ch], R[ch], np.asarray(json.load(open(O / f"payloads/{sp}_{ch}.json"))["edges"], float), ch, m, c, []) for c in CELLS}
                nb = lambda c: Nc[ch][c, m]["nd_band"] * R[ch][c, m]["s_ND"] / n
                d_ch = sum(nb(c) * rho[ch, m, c] for c in CELLS); d_max = sum(nb(c) * max(rho[k, m, c] for k in CHAINS) for c in CELLS)
                d_old = sum(nb(c) * rho_cl[c] for c in CELLS)
                big = max(CHAINS, key=lambda k: NS[k, m]["f_nd"])
                dd.append(dict(split=sp, mx_bin=m, chain=ch, f_nd=x["f_nd"], f_nd_band=x["f_nd_band"], delta_edge_edge30_frozen_rho=d_old, delta_edge_band_rho_chain=d_ch,
                               delta_edge_band_rho_maxchain=d_max, rho_chain_max_over_cells=max(rho[ch, m, c] for c in CELLS), rho_maxchain_max_over_cells=max(rho[k, m, c] for k in CHAINS for c in CELLS),
                               f_plus_delta_chain=x["f_nd"] + d_ch, f_plus_delta_maxchain=x["f_nd"] + d_max, largest_chain=big, is_largest=ch == big,
                               cross_chain_rho=x["f_nd"] + d_ch > CEIL, cross_maxchain_rho=x["f_nd"] + d_max > CEIL,
                               cross_old_rho=x["f_nd"] + d_old > CEIL))
    FN.wcsv(E30 / "rho_band_SB30.csv", list(rr[0]), rr); FN.wcsv(E30 / "delta_edge_SB30.csv", list(dd[0]), dd)
    for r in st:
        if r["tag"] != "nominal" or True:
            if r["is_largest"]: print(f"S {r['tag']:8s} {r['mx_bin']:8s} big={r['chain']:7s} f={r['f_nd']:.3f} nom={r['f_nd_nominal_SB30']:.3f} flag={r['flag_largest_gt_0p30']}")
    for r in dd:
        print(f"D {r['split']:4s} {r['mx_bin']:8s} {r['chain']:7s} f={r['f_nd']:.3f} band={r['f_nd_band']:.3f} old={r['delta_edge_edge30_frozen_rho']:.3f} dch={r['delta_edge_band_rho_chain']:.3f} "
              f"dmax={r['delta_edge_band_rho_maxchain']:.3f} f+dch={r['f_plus_delta_chain']:.3f} f+dmax={r['f_plus_delta_maxchain']:.3f} rho={r['rho_chain_max_over_cells']:.2f}/{r['rho_maxchain_max_over_cells']:.2f} big={r['is_largest']} X={r['cross_chain_rho']}/{r['cross_maxchain_rho']}")


if __name__ == "__main__": main()

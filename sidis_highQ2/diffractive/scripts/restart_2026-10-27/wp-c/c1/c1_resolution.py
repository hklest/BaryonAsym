#!/usr/bin/env python3
"""WP-C C1: resolution rows (z*, p_T*^2, x_F, beta) per gen cell and the M_X projection continuity table (C1-15 i, iv).
Writes resolution_c1.csv and mx_projection_c1.csv under W.  MC only."""
import csv, json, sys
from pathlib import Path
import numpy as np
sys.path.insert(0, str(Path(__file__).resolve().parent))
import c1_lib as C
import c1_tables as TB

MXN = ("4-10", "10-20", "20-40")


def row(obs, frame, cell, d, w):
    s = C.L.wstats(d, w)
    return dict(frame=frame, observable=obs, cell=cell, n_matched_raw=int(np.isfinite(d).sum()), bias_median=s["med"], sigma_core=s["core"], rms=s["rms"], mean=s["mean"])


def main():
    np.seterr(all="ignore")
    t, ev, T, E = TB.build_mc(); tpl, _ = TB.build_templates(ev)
    gk = t["keep"] & t["gv"]; res = []
    cg = C.cell(t["G"]["hemi"], t["bbin_g"])
    for fr in "AB":
        R, G = t[fr], t["G"]
        for obs, key in (("zstar", "zs"), ("pt2", "pt2"), ("xF", "xf")):
            for ci, cn in enumerate(C.CELLS):
                m = gk & (cg == ci) & np.isfinite(R[key]) & np.isfinite(G[key])
                res.append(row(obs, fr, cn, (R[key] - G[key])[m], t["w"][m]))
            m = gk & np.isfinite(R[key]) & np.isfinite(G[key]); res.append(row(obs, fr, "all", (R[key] - G[key])[m], t["w"][m]))
    # event-level beta: reco (DiffTot) minus gen (Q2_t/(Q2_t+mirMX^2)), per gen beta bin
    k = ev["keep"] & ev["genfid"].astype(bool) & np.isfinite(ev["beta"]) & np.isfinite(ev["gbeta"]); gb = C.beta_bin(ev["gbeta"])
    for b, name in ((0, "beta_lo"), (1, "beta_hi")):
        m = k & (gb == b); res.append(row("beta", "-", name, (ev["beta"] - ev["gbeta"])[m], ev["w"][m]))
    res.append(row("beta", "-", "all", (ev["beta"] - ev["gbeta"])[k], ev["w"][k]))
    # event-level relative beta (reco/gen - 1) for the record
    with open(C.W / "resolution_c1.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(res[0])); wr.writeheader()
        for r in res: wr.writerow({a: ("%.6g" % v if isinstance(v, float) else v) for a, v in r.items()})
    # M_X projection: purity of the frozen grid bins restricted to reco M_X classes, G2 and G1 gen frames (frame A)
    grid = json.load(open(C.W / "grid_c1.json"))["cells"]; rows = []
    cr = C.cell(t["A"]["hemi"], t["bbin_r"]); tcell = C.cell(tpl["A"]["hemi"], tpl["A"]["bbin"])
    for gen in ("G2", "G1"):
        Gd = t["G"] if gen == "G2" else t["G1"]; cgn = C.cell(Gd["hemi"], t["bbin_g"])
        for axis in ("z", "pt2"):
            for ci, cn in enumerate(C.CELLS):
                gname = grid[cn]["pt2_grid"] if axis == "pt2" else "q5"; nb = C.nbins(axis, gname)
                vr = t["A"]["pt2" if axis == "pt2" else "zs"]; vg = Gd["pt2" if axis == "pt2" else "zs"]
                kr = np.where(t["keep"], C.bin_index(cr, axis, gname, vr), -1); kg = np.where(gk & np.isfinite(vg), C.bin_index(cgn, axis, gname, vg), -1)
                tk = C.bin_index(tcell, axis, gname, tpl["A"]["pt2" if axis == "pt2" else "zs"])
                for c in range(3):
                    for k in range(nb):
                        j = ci * nb + k
                        inr = (kr == j) & (t["mxcls"] == c); tin = (tk == j) & (tpl["A"]["cls"] == c)
                        nsig = t["w"][inr].sum(); ntot = nsig + tpl["A"]["w"][tin].sum(); nsame = t["w"][inr & (kg == j) & t["gv"]].sum()
                        rows.append(dict(gen=gen, axis=axis, cell=cn, bin=k, mx_bin=MXN[c], n_raw_reco=int(inr.sum()), n_reco_w=nsig,
                                         purity=nsame / ntot if ntot > 0 else float("nan")))
    with open(C.W / "mx_projection_c1.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(rows[0])); wr.writeheader()
        for r in rows: wr.writerow({a: ("%.6g" % v if isinstance(v, float) else v) for a, v in r.items()})
    print("resolution rows", len(res), "mx projection rows", len(rows))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""User-grid (2026-10-28) statistics tables: {hemisphere} x {beta bin} x {z or pT bin}, per charge.
Inputs: WP-C signal track skims s<label>.npz + WP-A5 cells/veto weights (via propagate_wpa5.sample()), data track skims
from ug_data_tracks.py.  Selection: ec08<3.0 (or sentinel), x_IP<0.03, 4<=M_X<=40, Q2 integrated.  Login node (npz only).
Usage: ug_tables.py [--out DIR]"""
import argparse, csv, json, sys
from pathlib import Path
import numpy as np

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
W = D / "scripts/restart_2026-10-27"
sys.path.insert(0, str(W / "wp-a5")); sys.path.insert(0, str(W / "wp-c/study"))
import propagate_wpa5 as P
import veto_weight as VW
import wpc_lib as L
import xframe_obs as F
X = F.X
EDGE, XIPMAX, MXLO, MXHI = 3.0, 0.03, 4.0, 40.0
THR_N, THR_P, THR_S = 50.0, 0.35, 0.35            # D7 as written in binning_record.md sec. 3 / make_blind_mask.py
EE, EP, MP = 27.6, 920.0, F.M_P
SKIMS = D / "data/restart_2026-10-27/wp-c/user_grid_2026-10-28/skims"
DATA = ["ep0304", "em0405", "em06_true", "ep0607_true"]
FIXED_PT = {5: [0, .25, .5, 1, 2, 5], 4: [0, .25, .5, 1, 5], 3: [0, .5, 1, 5]}   # GeV^2 (pt2s) or GeV (pts, ptlab)
SPLIT_FIXED = 0.2


def zstar(xf, pt2, M, m):
    with np.errstate(all="ignore"):
        return 2 * np.sqrt((xf * M / 2) ** 2 + pt2 + m ** 2) / M


def zlab_B(Q2, y, M, xf, pt2, m):
    """z_lab = P.p_h / P.q from frame-B X-frame quantities (boost invariant; P, q, p_X collinear in frame B)."""
    with np.errstate(all="ignore"):
        Pq = y * 2 * EE * EP
        xi = (Q2 + M ** 2) / (Pq + np.sqrt(Pq ** 2 + MP ** 2 * (Q2 + M ** 2)))
        EPs = (Pq + xi * MP ** 2) / M
        bP = np.sqrt(1 - MP ** 2 / EPs ** 2)
        pl = xf * M / 2
        Es = np.sqrt(pl ** 2 + pt2 + m ** 2)
        return EPs * (Es - bP * pl) / Pq


def load_mc():
    T, E, cell, ec, w0, g, emb = P.sample()
    wv = w0 * VW.weights(E["e_mx"], cell, ec, "nominal")
    keep = ((ec < EDGE) | (ec < -900)) & (E["e_xIp"] < XIPMAX) & (E["e_mx"] >= MXLO) & (E["e_mx"] <= MXHI)
    tk = keep[g]
    m = X.gen_mass(np.abs(T["t_pdg"].astype(np.int64)))
    Q2t, yt, gmx = E["e_Q2t"][g], E["e_yt"][g], T["t_gmx"]
    out = dict(
        ev_keep=keep, ev_w=wv, ev_beta=E["e_beta"], ev_mx=E["e_mx"], ev_samp=E["e_samp"], ev_Q2=E["e_Q2es"],
        ev_gbeta=E["e_Q2t"] / (E["e_Q2t"] + E["e_mirMX"] ** 2),
        t_keep=tk, t_g=g, t_w=wv[g], t_chg=T["t_charge"], t_gv=T["t_gv"], t_9012=(T["t_samp"] == 0), t_pt=T["t_pt"],
        t_Q2=E["e_Q2es"][g], t_y=E["e_Yes"][g],
        B=dict(xf=T["t_xfB"], pt2=T["t_pt2B"], M=T["t_mx"]), A=dict(xf=T["t_xfA"], pt2=T["t_pt2A"], M=T["t_mxA"]),
        G=dict(xf=T["t_xf2"], pt2=T["t_pt22"], M=gmx, m=m, pt=T["t_gpt"], Q2=Q2t, y=yt))
    for fr in "AB":
        o = out[fr]
        o["zs"] = zstar(o["xf"], o["pt2"], o["M"], F.X.M_PION)
    out["B"]["zlab"] = zlab_B(out["t_Q2"], out["t_y"], out["B"]["M"], out["B"]["xf"], out["B"]["pt2"], F.X.M_PION)
    G = out["G"]; G["zs"] = zstar(G["xf"], G["pt2"], G["M"], G["m"])
    G["zlab"] = zlab_B(G["Q2"], G["y"], G["M"], G["xf"], G["pt2"], G["m"])
    return out


def load_data():
    parts = [np.load(SKIMS / (l + ".npz")) for l in DATA]
    ev = {k: [] for k in ("mx", "mxA", "beta", "xIp", "ec08", "Q2es", "Yes")}
    tr = {k: [] for k in ("ev", "charge", "pt", "xfA", "pt2A", "xfB", "pt2B", "zlab")}
    off = 0
    for p in parts:
        for k in ev: ev[k].append(p["e_" + k])
        for k in tr: tr[k].append(p["t_" + k] + (off if k == "ev" else 0))
        off += len(p["e_mx"])
    ev = {k: np.concatenate(v) for k, v in ev.items()}; tr = {k: np.concatenate(v) for k, v in tr.items()}
    keep = ((ev["ec08"] < EDGE) | (ev["ec08"] < -900)) & (ev["xIp"] < XIPMAX) & (ev["mx"] >= MXLO) & (ev["mx"] <= MXHI)
    tk = keep[tr["ev"]]
    d = dict(ev_keep=keep, ev_beta=ev["beta"], ev_mx=ev["mx"], ev_Q2=ev["Q2es"], t_keep=tk, t_g=tr["ev"], t_chg=tr["charge"],
             t_pt=tr["pt"], t_Q2=ev["Q2es"][tr["ev"]], t_y=ev["Yes"][tr["ev"]], t_zlab_direct=tr["zlab"])
    d["B"] = dict(xf=tr["xfB"], pt2=tr["pt2B"], M=ev["mx"][tr["ev"]]); d["A"] = dict(xf=tr["xfA"], pt2=tr["pt2A"], M=ev["mxA"][tr["ev"]])
    for fr in "AB":
        o = d[fr]; o["zs"] = zstar(o["xf"], o["pt2"], o["M"], F.X.M_PION)
    d["B"]["zlab"] = zlab_B(d["t_Q2"], d["t_y"], d["B"]["M"], d["B"]["xf"], d["B"]["pt2"], F.X.M_PION)
    return d


def wq(x, qs):
    return np.quantile(x[np.isfinite(x)], qs)


def hemi(xf):
    return np.where(np.isfinite(xf) & (xf > 0), 1, np.where(np.isfinite(xf) & (xf < 0), 0, -1))


def cell_idx(hm, bb, val, edges_by_hemi, nb):
    """flat cell = ((hemi*2 + beta)*nb + k); -1 outside.  edges_by_hemi[h] = array of nb+1 lower edges (last = upper)."""
    c = np.full(len(hm), -1)
    for h in (0, 1):
        e = np.asarray(edges_by_hemi[h], float)
        k = np.searchsorted(e, val, side="right") - 1
        ok = (hm == h) & (bb >= 0) & np.isfinite(val) & (val >= e[0]) & (val < e[-1])
        c[ok] = ((h * 2 + bb[ok]) * nb + np.clip(k[ok], 0, nb - 1))
    return c


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", default=str(D / "data/restart_2026-10-27/wp-c/user_grid_2026-10-28"))
    a = ap.parse_args(); OUT = Path(a.out); OUT.mkdir(parents=True, exist_ok=True)
    np.seterr(all="ignore")
    mc, dt = load_mc(), load_data()
    meta = dict(selection="ec08<3.0|sentinel, xIp<0.03, 4<=mx<=40, Q2 integrated; WP-C study selection otherwise (see record)",
                thresholds=dict(n_sig_pred_min=THR_N, purity_min=THR_P, stability_min=THR_S))
    # ---- check z_lab invariance formula against the direct lab calculation on data
    ok = dt["t_keep"] & np.isfinite(dt["B"]["zlab"]) & np.isfinite(dt["t_zlab_direct"])
    dz = (dt["B"]["zlab"] - dt["t_zlab_direct"])[ok]
    meta["zlab_check"] = dict(n=int(ok.sum()), max_abs_diff=float(np.max(np.abs(dz))), median_abs_diff=float(np.median(np.abs(dz))))
    # ---- beta split
    bd = dt["ev_beta"][dt["ev_keep"]]; bmed = float(np.median(bd))
    bm_mc = float(np.median(mc["ev_beta"][mc["ev_keep"]]))
    meta["beta"] = dict(data_median=bmed, mc_unweighted_median=bm_mc, n_data_events=int(dt["ev_keep"].sum()))
    mxr = {}
    for b, sel in ((0, dt["ev_beta"] < bmed), (1, dt["ev_beta"] >= bmed)):
        s = dt["ev_keep"] & sel
        mxr[b] = dict(n=int(s.sum()), MX_p5_p50_p95=[float(v) for v in wq(dt["ev_mx"][s], [.05, .5, .95])],
                      Q2_p5_p50_p95=[float(v) for v in wq(dt["ev_Q2"][s], [.05, .5, .95])])
    meta["beta"]["bins"] = mxr
    ebb = lambda beta: np.where(np.isfinite(beta), (beta >= bmed).astype(int), -1)
    mc_bb = ebb(mc["ev_beta"])[mc["t_g"]]; mc_gbb = ebb(mc["ev_gbeta"])[mc["t_g"]]; dt_bb = ebb(dt["ev_beta"])[dt["t_g"]]
    # ---- event counts
    evrows = []
    for fr in "AB":
        hm_m = None
        for b in (0, 1):
            em = mc["ev_keep"] & (ebb(mc["ev_beta"]) == b); ed = dt["ev_keep"] & (ebb(dt["ev_beta"]) == b)
            evrows.append(dict(frame=fr, hemi="all", beta_bin=b, n_data=int(ed.sum()), w_diff8=float(mc["ev_w"][em].sum()),
                               raw_diff8=int(em.sum()), raw_9012=int((em & (mc["ev_samp"] == 0)).sum())))
            for h in (0, 1):
                # events with >=1 selected track in the hemisphere (any charge)
                def nev(src, evk, ew, nE, extra=None):
                    t = src["t_keep"] & (hemi(src[fr]["xf"]) == h) & (ebb(src["ev_beta"])[src["t_g"]] == b)
                    if extra is not None: t &= extra
                    return np.unique(src["t_g"][t])
                gd = nev(dt, None, None, None); gm = nev(mc, None, None, None)
                evrows.append(dict(frame=fr, hemi="P(xF>0)" if h else "G(xF<0)", beta_bin=b, n_data=len(gd),
                                   w_diff8=float(mc["ev_w"][gm].sum()), raw_diff8=len(gm), raw_9012=int((mc["ev_samp"][gm] == 0).sum())))
    # ---- grids
    rows = []; edges_rec = {}; factors = {}
    chg_sel = {"h+": lambda c: c > 0, "h-": lambda c: c < 0, "sum": lambda c: c != 0}
    for fr in "BA":
        r, g, d = mc[fr], mc["G"], dt[fr]
        hm_m, hm_d, hm_g = hemi(r["xf"]), hemi(d["xf"]), hemi(g["xf"])
        km = mc["t_keep"]; kd = dt["t_keep"]
        grids = []   # (grid, variant, binvar label, edges_by_hemi, nb, reco_vals_mc, gen_vals, data_vals)
        # z grids
        for zname, zrm, zrg, zrd in (("zstar", r["zs"], g["zs"], d["zs"]), ("zlab", r.get("zlab", mc["B"]["zlab"]), g["zlab"], d.get("zlab", dt["B"]["zlab"]))):
            med = [float(np.median(zrd[kd & (hm_d == h) & np.isfinite(zrd)])) for h in (0, 1)]
            edges_rec[(fr, zname)] = dict(data_median_G_xFneg=med[0], data_median_P_xFpos=med[1], fixed_split=SPLIT_FIXED)
            for sp, lab in (([med[0], med[1]], "med"), ([SPLIT_FIXED, SPLIT_FIXED], "0.2")):
                eb = {h: [0.0, sp[h], 1e9] for h in (0, 1)}
                grids.append(("z", "%s_%s" % (zname, lab), eb, 2, zrm, zrg, zrd))
        # pT grids
        pts = {"pt2s": (r["pt2"], g["pt2"], d["pt2"]), "pts": (np.sqrt(r["pt2"]), np.sqrt(g["pt2"]), np.sqrt(d["pt2"])),
               "ptlab": (mc["t_pt"], g["pt"], dt["t_pt"])}
        for pname, (prm, prg, prd) in pts.items():
            sel = kd & np.isfinite(prd) & (hm_d >= 0)
            for n in (5, 4, 3):
                if pname != "pts":
                    q = np.quantile(prd[sel], np.linspace(0, 1, n + 1)[1:-1]); q = np.round(q, 3)
                    e = [0.0] + [float(v) for v in q] + [1e9]
                    edges_rec[(fr, pname, "q%d" % n)] = e[:-1]
                    grids.append(("pt", "%s_q%d" % (pname, n), {0: e, 1: e}, n, prm, prg, prd))
                e = [float(v) for v in FIXED_PT[n][:-1]] + [1e9]
                edges_rec[(fr, pname, "f%d" % n)] = FIXED_PT[n]
                grids.append(("pt", "%s_f%d" % (pname, n), {0: e, 1: e}, n, prm, prg, prd))
        for gk, var, eb, nb, vm, vg, vd in grids:
            cm = cell_idx(hm_m, mc_bb, vm, eb, nb); cm = np.where(km, cm, -1)
            cg = cell_idx(hm_g, mc_gbb, vg, eb, nb); cg = np.where(km & mc["t_gv"], cg, -1)
            cd = cell_idx(hm_d, dt_bb, vd, eb, nb); cd = np.where(kd, cd, -1)
            ncell = 4 * nb
            for cn, cf in chg_sel.items():
                sm = cf(mc["t_chg"]); sd = cf(dt["t_chg"])
                hist = lambda c, w, s: np.bincount(c[(c >= 0) & s], weights=w[(c >= 0) & s], minlength=ncell)
                w = mc["t_w"]; one = np.ones_like(w); gv = mc["t_gv"]; n9 = mc["t_9012"]
                same = np.where((cm >= 0) & (cm == cg), cm, -1)
                nd = hist(cd, np.ones(len(cd)), sd)
                w_sig = hist(cm, w, sm & gv); raw_sig = hist(cm, one, sm & gv); r9_sig = hist(cm, one, sm & gv & n9)
                w_all = hist(cm, w, sm); raw_all = hist(cm, one, sm); r9_all = hist(cm, one, sm & n9)
                w_o = w_sig - hist(cm, w, sm & gv & n9)
                w_9 = hist(cm, w, sm & gv & n9)
                with np.errstate(all="ignore"):
                    pur = hist(same, w, sm) / w_all
                    stab = hist(same, w, sm) / hist(cg, w, sm)
                for c in range(ncell):
                    h, bb, k = c // (2 * nb), (c // nb) % 2, c % nb
                    lo, hi = eb[h][k], eb[h][k + 1]
                    p_ok, s_ok, n_ok = (pur[c] >= THR_P), (stab[c] >= THR_S), (w_sig[c] >= THR_N)
                    why = [t for t, bad in (("N<50", not n_ok), ("purity<0.35", not p_ok), ("stability<0.35", not s_ok)) if bad]
                    with np.errstate(all="ignore"):
                        f_w = 1.0 if n_ok else ((THR_N - w_o[c]) / w_9[c] if w_9[c] > 0 else np.inf)
                        f_w = max(1.0, f_w)
                        f_raw = max(1.0, THR_N / r9_sig[c]) if r9_sig[c] > 0 else np.inf
                    rows.append(dict(frame=fr, grid=gk, variant=var, nbins=nb, hemi="P(xF>0)" if h else "G(xF<0)", beta_bin=bb, bin=k,
                                     lo=lo, hi=("inf" if hi >= 1e8 else hi), charge=cn, n_data=int(nd[c]), w_diff8_sig=w_sig[c],
                                     raw_diff8_sig=int(raw_sig[c]), raw_9012_sig=int(r9_sig[c]), w_diff8_all=w_all[c],
                                     raw_diff8_all=int(raw_all[c]), raw_9012_all=int(r9_all[c]), purity=pur[c], stability=stab[c],
                                     pass_N=n_ok, pass_purity=p_ok, pass_stability=s_ok, D7_pass=not why, fail=";".join(why),
                                     f9012_weighted=f_w, f9012_raw50=f_raw))
    # ---- factors per (frame, variant): over per-charge bins only
    for fr in "BA":
        for var in sorted({r["variant"] for r in rows}):
            rr = [x for x in rows if x["frame"] == fr and x["variant"] == var and x["charge"] != "sum"]
            cnt_fail = [x for x in rr if not x["pass_N"]]
            ps_fail = [x for x in rr if not (x["pass_purity"] and x["pass_stability"])]
            fw = max([x["f9012_weighted"] for x in rr] + [1.0]); fr_ = max([x["f9012_raw50"] for x in rr] + [1.0])
            # the factor only fixes count failures: bins with N<50 and (purity or stability fail) are not rescued by statistics
            fw_fixable = max([x["f9012_weighted"] for x in rr if x["pass_purity"] and x["pass_stability"]] + [1.0])
            factors["%s|%s" % (fr, var)] = dict(n_bins=len(rr), n_fail_N=len(cnt_fail), n_fail_purity_or_stab=len(ps_fail),
                                                n_fail_any=sum(not x["D7_pass"] for x in rr), f9012_weighted_all=fw,
                                                f9012_weighted_purstab_ok_bins=fw_fixable, f9012_raw50_all=fr_)
    def cl(o):
        if isinstance(o, dict): return {str(k): cl(v) for k, v in o.items()}
        if isinstance(o, (list, tuple)): return [cl(v) for v in o]
        if isinstance(o, (np.floating, float)): return None if not np.isfinite(o) else float(o)
        if isinstance(o, (np.integer,)): return int(o)
        if isinstance(o, (np.bool_,)): return bool(o)
        return o
    json.dump(cl(dict(meta=meta, edges={str(k): v for k, v in edges_rec.items()}, events=evrows, factors=factors, n_rows=len(rows))),
              open(OUT / "user_grid_summary.json", "w"), indent=1)
    json.dump(cl(rows), open(OUT / "user_grid_tables.json", "w"))
    with open(OUT / "user_grid_tables.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(rows[0])); wr.writeheader()
        for x in rows: wr.writerow({k: ("%.6g" % v if isinstance(v, (float, np.floating)) else v) for k, v in x.items()})
    with open(OUT / "user_grid_events.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(evrows[0])); wr.writeheader(); wr.writerows(cl(evrows))
    print(json.dumps(cl(meta), indent=1)); print("rows", len(rows))


if __name__ == "__main__":
    main()

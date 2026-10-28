#!/usr/bin/env python3
"""WP-C C1 slice 2: per-bin tables on the frozen gen grid (840x cocktail), per-cell grid rule (C1-7), stand-in ND/Reggeon fill (C1-13).
Writes binstats_c1_A/B.csv/json, cells_c1.json, grid_c1.json(+.sha256).  Condor (c1_run.sub) -- loads ~0.6M tracks.
Usage: c1_tables.py [--out W/]   (both frames, one pass).  No data file is opened; n_data is read from user_grid_tables.csv."""
import argparse, csv, datetime, json, sys
from pathlib import Path
import numpy as np
sys.path.insert(0, str(Path(__file__).resolve().parent))
import c1_lib as C

F_ND_CSV = C.D / "data/restart_2026-10-27/wp-a6/edge30/f_nd_per_mx_edge30.csv"
SCALES_CSV = C.D / "data/restart_2026-10-27/wp-a6/scales_resolved_odd_nominal.csv"
MXC = {"MX4_10": 0, "MX10_20": 1, "MX20_40": 2}
M_PI = C.X.M_PION


def f_nd_standin():
    """largest-chain odd-split SB30 f_ND per reco M_X class (D25 'largest chain')."""
    out = {}
    for r in csv.DictReader(open(F_ND_CSV)):
        if r["split"] == "odd" and r["sel"] == "sel30" and r["chain"] == r["largest_chain"]:
            out[MXC[r["mx_bin"]]] = (float(r["f_nd"]), r["chain"], float(r["f_reg"]))
    assert sorted(out) == [0, 1, 2], out
    return out


def build_mc():
    T, E, cell_ev, ec, w0, g, emb, event, stream = C.sample_c1()
    wv = w0 * C.VW.weights(E["e_mx"], cell_ev, ec, "nominal")
    keep = ((ec < C.EC_EDGE) | (ec < -900)) & (E["e_xIp"] < C.XIPMAX) & (E["e_mx"] >= C.MXLO) & (E["e_mx"] <= C.MXHI)
    gbeta_ev = E["e_Q2t"] / (E["e_Q2t"] + E["e_mirMX"] ** 2)
    m = C.X.gen_mass(np.abs(T["t_pdg"].astype(np.int64)))
    t = dict(keep=keep[g], g=g, w=wv[g], chg=T["t_charge"], gv=T["t_gv"], samp=T["t_samp"],
             bbin_r=C.beta_bin(E["e_beta"])[g], bbin_g=C.beta_bin(gbeta_ev)[g], mxcls=emb[g])
    for fr in "AB":
        xf, pt2, M = T["t_xf" + fr], T["t_pt2" + fr], (T["t_mxA"] if fr == "A" else T["t_mx"])
        t[fr] = dict(xf=xf, pt2=pt2, zs=C.zstar(xf, pt2, M, M_PI), hemi=C.hemi(xf))
    t["G"] = dict(xf=T["t_xf2"], pt2=T["t_pt22"], zs=C.zstar(T["t_xf2"], T["t_pt22"], T["t_gmx"], m), hemi=C.hemi(T["t_xf2"]))
    t["G1"] = dict(xf=T["t_xf1"], pt2=T["t_pt21"], zs=C.zstar(T["t_xf1"], T["t_pt21"], T["t_M1"], m), hemi=C.hemi(T["t_xf1"]))
    ev = dict(keep=keep, w=wv, w0=w0, samp=E["e_samp"], event=event, stream=stream, emb=emb, beta=E["e_beta"], gbeta=gbeta_ev,
              genfid=E["e_genfid"], mx=E["e_mx"], cell=cell_ev, ec=ec)
    return t, ev, T, E


def build_templates(ev):
    """ND (b7071) and Reggeon (b6671/b6677) reco tracks at edge 3.0, WP-A6 conventions; ND scaled so its event share per reco M_X
    class equals the stand-in f_ND (C1-13).  Returns (track dict per frame, info)."""
    import purity_edge30_wpa6 as E30
    PF = E30.PF
    S = PF.read_scales(str(SCALES_CSV))
    fnd = f_nd_standin()
    # event-level sums of the cocktail per class (veto-weighted, kept events)
    k = ev["keep"] & (ev["emb"] >= 0)
    Wsig = np.bincount(ev["emb"][k], weights=ev["w"][k], minlength=3)
    blocks = []
    for kind, labels in (("nd", ["7071"]), ("reg", ["6671", "6677"])):
        for l in labels:
            d = np.load(PF.TRK + "b%s.npz" % l)
            eg = np.searchsorted(d["e_idx"], d["t_ev"]); ec = E30.ec08_for("b", l, d["e_idx"])
            kev = ((ec < C.EC_EDGE) | (ec < -900)) & (d["e_xIp"] > 0) & (d["e_xIp"] < C.XIPMAX) & (d["e_mx"] >= C.MXLO) & (d["e_mx"] <= C.MXHI)
            fac = PF.W.fac_used(l, PF.CHAIN) / PF.W.table_fac(l)
            cls = PF.mx_class(d["e_mx"])
            ew = d["e_w"] * fac
            if kind == "reg": ew = ew * PF.scale_vector(dict(t_mx=d["e_mx"], t_cell=d["e_cell"].astype(int)), "reg", S)
            blocks.append(dict(kind=kind, label=l, d=d, eg=eg, kev=kev, cls=cls, ew=ew))
    Wreg = sum(np.bincount(b["cls"][b["kev"]], weights=b["ew"][b["kev"]], minlength=4)[:3] for b in blocks if b["kind"] == "reg")
    Wnd = sum(np.bincount(b["cls"][b["kev"]], weights=b["ew"][b["kev"]], minlength=4)[:3] for b in blocks if b["kind"] == "nd")
    # f_nd = s Wnd / (Wsig + Wreg + s Wnd)
    s_nd = np.array([fnd[c][0] / (1 - fnd[c][0]) * (Wsig[c] + Wreg[c]) / Wnd[c] for c in range(3)])
    out = {fr: dict(xf=[], pt2=[], M=[], bbin=[], w=[], chg=[], kind=[], cls=[]) for fr in "AB"}
    for b in blocks:
        d, eg = b["d"], b["eg"]; tk = b["kev"][eg] & (b["cls"][eg] < 3)
        sc = np.where(b["cls"] < 3, s_nd[np.minimum(b["cls"], 2)], 0.0) if b["kind"] == "nd" else 1.0
        w = (b["ew"] * sc)[eg][tk]
        for fr in "AB":
            o = out[fr]
            o["xf"].append(d["t_xf" + fr][tk]); o["pt2"].append(d["t_pt2" + fr][tk]); o["M"].append((d["t_mxA"] if fr == "A" else d["t_mx"])[tk])
            o["bbin"].append(C.beta_bin(d["e_beta"])[eg][tk]); o["w"].append(w); o["chg"].append(d["t_charge"][tk])
            o["kind"].append(np.full(int(tk.sum()), 0 if b["kind"] == "nd" else 1)); o["cls"].append(b["cls"][eg][tk])
    for fr in "AB":
        o = {k: np.concatenate(v) for k, v in out[fr].items()}
        o["zs"] = C.zstar(o["xf"], o["pt2"], o["M"], M_PI); o["hemi"] = C.hemi(o["xf"]); out[fr] = o
    share = [float(s_nd[c] * Wnd[c] / (Wsig[c] + Wreg[c] + s_nd[c] * Wnd[c])) for c in range(3)]
    info = dict(nd_event_share_after_scaling=share, f_nd_standin={str(c): dict(f_nd=fnd[c][0], chain=fnd[c][1], f_reg_in_csv=fnd[c][2]) for c in range(3)},
                s_nd_factor=[float(x) for x in s_nd], W_sig_events=[float(x) for x in Wsig], W_reg_events=[float(x) for x in Wreg],
                W_nd_events_unscaled=[float(x) for x in Wnd], scales_file=str(SCALES_CSV), f_nd_file=str(F_ND_CSV))
    return out, info


def hist(idx, w, sel, n):
    s = sel & (idx >= 0)
    return np.bincount(idx[s], weights=None if w is None else w[s], minlength=n)


def evaluate(t, tpl, frame, charge, axis, grid, w=None, fast=False, ndata=None):
    """rows (list of dict) for one (frame, charge, axis, grid) over the 4 cells x nb bins; w = track-weight override (bootstrap)."""
    nb = C.nbins(axis, grid); NF = 4 * nb; NS = 8 * nb
    R, G = t[frame], t["G"]; vr, vg = R[axis if axis == "pt2" else "zs"], G[axis if axis == "pt2" else "zs"]
    w = t["w"] if w is None else w
    sm = (t["chg"] > 0) if charge == "h+" else (t["chg"] < 0)
    base = t["keep"] & sm
    cr = C.cell(R["hemi"], t["bbin_r"]); cgn = C.cell(G["hemi"], t["bbin_g"])
    gvk = t["keep"] & t["gv"] & np.isfinite(vg)
    kr = np.where(base, C.bin_index(cr, axis, grid, vr), -1)
    kg = np.where(gvk, C.bin_index(cgn, axis, grid, vg), -1)
    gv = t["gv"]; same = np.where((kr >= 0) & (kr == kg), kr, -1)
    nsig = hist(kr, w, base, NF); ngv = hist(kr, w, base & gv, NF); nsame = hist(same, w, base & gv, NF); ngen = hist(kg, w, base, NF)
    tc = C.cell(tpl[frame]["hemi"], tpl[frame]["bbin"]); tv = tpl[frame][axis if axis == "pt2" else "zs"]
    tsel = (tpl[frame]["chg"] > 0) if charge == "h+" else (tpl[frame]["chg"] < 0)
    tk = C.bin_index(tc, axis, grid, tv); tw = tpl[frame]["w"]
    wnd = hist(tk, tw, tsel & (tpl[frame]["kind"] == 0), NF); wreg = hist(tk, tw, tsel & (tpl[frame]["kind"] == 1), NF)
    ntot = nsig + wnd + wreg
    with np.errstate(all="ignore"):
        pur, stab, pm = nsame / ntot, nsame / ngen, nsame / ngv
        ffake, fmig, fnd_, freg = (nsig - ngv) / ntot, (ngv - nsame) / ntot, wnd / ntot, wreg / ntot
    out = dict(pur=pur, stab=stab, pm=pm, ffake=ffake, fmig=fmig, fnd=fnd_, freg=freg, ngv=ngv, ntot=ntot, ngen=ngen, nsame=nsame)
    if fast: return out
    is8 = t["samp"] < 4
    ksub = np.where(base, C.bin_index(cr, axis, grid, vr, sub=True), -1); gsub = C.bin_index(cgn, axis, grid, vg, sub=True)
    raw_sub = hist(ksub, None, base & gv, NS).reshape(NF, 2)
    out["raw_reco840"] = hist(kr, None, base & gv & is8, NF); out["raw_gen840"] = hist(kg, None, base & is8, NF)
    out["raw_subbin_min"] = raw_sub.min(axis=1)
    # response elements (reco sub-bin i, gen bin j), 840x matched tracks
    m = base & gv & is8 & (ksub >= 0) & (kg >= 0)
    flat = ksub[m] * NF + kg[m]
    rawE = np.bincount(flat, minlength=NS * NF).reshape(NS, NF).astype(float)
    wE = np.bincount(flat, weights=w[m], minlength=NS * NF).reshape(NS, NF)
    with np.errstate(all="ignore"): share = wE / wE.sum(axis=0, keepdims=True)
    used = np.nan_to_num(share, nan=0.0) >= C.THR["element_share"]
    out["raw_element_min"] = np.array([rawE[used[:, j], j].min() if used[:, j].any() else 0.0 for j in range(NF)])
    gm = gvk & base & (kg >= 0)
    frac = lambda sel: hist(kg, w, gm & sel, NF) / ngen
    with np.errstate(all="ignore"):
        out["flip_hemi"] = frac((R["hemi"] != G["hemi"])); out["flip_beta"] = frac(t["bbin_r"] != t["bbin_g"])
        out["intercell"] = frac(cr != cgn); out["xf_small"] = frac(np.abs(G["xf"]) < 0.02)
        out["diag"] = frac((cr == cgn) & (ksub >= 0) & (ksub == gsub))
    sig = np.full(NF, np.nan); bias = np.full(NF, np.nan)
    for j in range(NF):
        mm = gm & (kg == j)
        if mm.sum() >= 20:
            s = C.L.wstats((vr - vg)[mm], w[mm]); sig[j], bias[j] = s["core"], s["med"]
    out["sigma"], out["bias"] = sig, bias
    return out


def rows_from(o, frame, charge, axis, grid, ndata):
    nb = C.nbins(axis, grid); e = C.axis_edges(axis, grid); rows = []
    for j in range(4 * nb):
        c, k = j // nb, j % nb; lo, hi = e[k], e[k + 1]
        hi_eff = hi if np.isfinite(hi) else (C.Z_TOP_HI if axis == "z" else np.inf)
        width = hi_eff - lo; s, b = o["sigma"][j], o["bias"][j]
        with np.errstate(all="ignore"):
            w2 = bool(width >= C.THR["width_core"] * s); w1 = bool(width >= C.THR["width_bias"] * (s + abs(b)))
        r = dict(frame=frame, charge=charge, axis=axis, grid=grid if axis == "pt2" else "z", cell=c, cell_name=C.CELLS[c], hemi="GP"[c // 2],
                 beta_bin=c % 2, bin=k, lo=lo, hi=("inf" if not np.isfinite(hi) else hi),
                 n_data=ndata.get((frame, charge, axis, grid if axis == "pt2" else "z", c, k), ""), n_sig_pred=o["ngv"][j],
                 n_raw_840x_reco=int(o["raw_reco840"][j]), n_raw_gen_840x=int(o["raw_gen840"][j]), n_raw_subbin_min=int(o["raw_subbin_min"][j]),
                 n_raw_element_min=int(o["raw_element_min"][j]), purity=o["pur"][j], stability=o["stab"][j], p_mig=o["pm"][j],
                 f_fake=o["ffake"][j], f_mig=o["fmig"][j], f_nd=o["fnd"][j], f_reg=o["freg"][j], diag_frac=o["diag"][j],
                 intercell_mig=o["intercell"][j], flip_hemi=o["flip_hemi"][j], flip_beta=o["flip_beta"][j], xf_small_share=o["xf_small"][j],
                 sigma_core=s, bias=b, width=width, width_ok_2s=w2, width_ok_bias=w1)
        p, why = C.d7plus_row(r); r["pass_D7plus"] = p; r["reasons"] = ";".join(why); rows.append(r)
    return rows


def ug_ndata():
    """n_data per (frame A, charge, axis, grid, cell, bin) from the existing UG table (binning statistics, no new read)."""
    out = {}
    vmap = {"zstar_0.2": ("z", None), "pt2s_q5": ("pt2", "q5"), "pt2s_q4": ("pt2", "q4"), "pt2s_q3": ("pt2", "q3")}
    for r in csv.DictReader(open(C.UG / "user_grid_tables.csv")):
        if r["frame"] != "A" or r["variant"] not in vmap or r["charge"] not in ("h+", "h-"): continue
        ax, gname = vmap[r["variant"]]
        c = (0 if r["hemi"].startswith("G") else 2) + int(r["beta_bin"])
        out[("A", r["charge"], ax, gname if ax == "pt2" else "z", c, int(r["bin"]))] = int(r["n_data"])
    return out


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", default=str(C.W)); a = ap.parse_args(); OUT = Path(a.out); OUT.mkdir(parents=True, exist_ok=True)
    np.seterr(all="ignore")
    t, ev, T, E = build_mc(); tpl, tinfo = build_templates(ev); nd = ug_ndata()
    # reco 2x sub-bin split of the open top bins: weighted MC median of the frame-A reco value in the bin (cocktail, gen-valid, kept)
    gvm = t["keep"] & t["gv"]
    for name, v, lo in [("z", t["A"]["zs"], C.ZSTAR_SPLIT)] + [(q, t["A"]["pt2"], C.PT2_EDGES[q][-1]) for q in C.GRIDS]:
        m = gvm & np.isfinite(v) & (v >= lo); o = np.argsort(v[m]); cw = np.cumsum(t["w"][m][o]); C.TOP_SPLIT[name] = float(v[m][o][np.searchsorted(cw, cw[-1] / 2)])
    allrows = {"A": [], "B": []}
    for fr in "AB":
        for ch in ("h+", "h-"):
            for ax, gl in (("z", ("z",)), ("pt2", C.GRIDS)):
                for gname in gl:
                    gg = "q5" if ax == "z" else gname                      # z axis: grid name unused
                    o = evaluate(t, tpl, fr, ch, ax, gg)
                    allrows[fr] += rows_from(o, fr, ch, ax, gg, nd)
    cols = list(allrows["A"][0])
    for fr in "AB":
        with open(OUT / ("binstats_c1_%s.csv" % fr), "w", newline="") as f:
            wr = csv.DictWriter(f, fieldnames=cols); wr.writeheader()
            for r in allrows[fr]: wr.writerow({k: ("%.8g" % v if isinstance(v, (float, np.floating)) else v) for k, v in r.items()})
    grid = C.grid_rule(allrows["A"], "A"); gridB = C.grid_rule(allrows["B"], "B")
    # per-cell summary
    cells = {}
    for ci, cn in enumerate(C.CELLS):
        k = t["keep"] & (C.cell(t["A"]["hemi"], t["bbin_r"]) == ci)
        zr = t["A"]["zs"]; gvk = t["keep"] & t["gv"]
        cells[cn] = dict(n_tracks_raw_cocktail=int(k.sum()), N_pred_w=float(t["w"][k & t["gv"]].sum()),
                         reco_zstar_gt1_fraction_A=float((zr[k] > 1).sum() / max(k.sum(), 1)),
                         reco_zstar_gt1_fraction_B=float((t["B"]["zs"][t["keep"] & (C.cell(t["B"]["hemi"], t["bbin_r"]) == ci)] > 1).mean()))
    gk = t["keep"] & t["gv"] & np.isfinite(t["G"]["zs"])
    gz, gx = t["G"]["zs"][gk], t["G"]["xf"][gk]
    chk = dict(gen_zstar_min=float(gz.min()), gen_zstar_max=float(gz.max()), gen_zstar_gt1_by_1e9=int((gz > 1 + 1e-9).sum()), gen_zstar_le0=int((gz <= 0).sum()),
               gen_xf_min=float(gx.min()), gen_xf_max=float(gx.max()), n_gen_tracks=int(gk.sum()))
    part = {}
    for fr in "AB":
        for i, l in enumerate(C.LABELS_C1):
            m = t["samp"] == i; h = t[fr]["hemi"][m]
            part["%s|%d" % (fr, l)] = dict(n_all=int(m.sum()), n_G=int((h == 0).sum()), n_P=int((h == 1).sum()), n_undef=int((h < 0).sum()))
    # event-level beta purity / stability (cocktail signal events, informational C1-4)
    k = ev["keep"]; rb, gb = C.beta_bin(ev["beta"]), C.beta_bin(ev["gbeta"]); fid = ev["genfid"].astype(bool)
    bp = {}
    for b in (0, 1):
        same = k & (rb == b) & (gb == b) & fid
        bp[str(b)] = dict(purity=float(ev["w"][same].sum() / ev["w"][k & (rb == b)].sum()), stability=float(ev["w"][same].sum() / ev["w"][k & fid & (gb == b)].sum()),
                          n_events_raw=int((k & (rb == b)).sum()))
    bmed_mc = float(np.median(ev["beta"][k]))
    scan = {}
    for r in allrows["A"]:
        if r["axis"] == "pt2":
            e = scan.setdefault("%s|%s" % (r["cell_name"], r["grid"]), dict(n_bins=0, n_fail={"h+": 0, "h-": 0}, reasons={}))
            e["n_bins"] += r["charge"] == "h+"; e["n_fail"][r["charge"]] += (not r["pass_D7plus"])
            for x in [y for y in r["reasons"].split(";") if y]: e["reasons"][x] = e["reasons"].get(x, 0) + 1
    chk.update(gen_xf_gt1=int((np.abs(gx) > 1).sum()), gen_zstar_gt1=int((gz > 1).sum()), gen_zstar_gt1_by_1e9_frac=float((gz > 1 + 1e-9).mean()))
    cj = dict(grid_scan=scan, reco_subbin_top_split=C.TOP_SPLIT, hemisphere_partition=part, cells=cells, beta_bin_event=bp, definition_checks=chk, mc_unweighted_beta_median=bmed_mc, template=tinfo,
              flip_by_cell_gen_bin=None, created_utc=datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"))
    flips = {}
    for r in allrows["A"]:
        if r["grid"] in (grid[r["cell_name"]], "z"):
            flips["%s|%s|%s|%d" % (r["charge"], r["cell_name"], r["axis"], r["bin"])] = dict(
                flip_hemi=r["flip_hemi"], flip_beta=r["flip_beta"], intercell=r["intercell_mig"], xf_small_share=r["xf_small_share"], grid=r["grid"])
    cj["flip_by_cell_gen_bin"] = flips
    cj["n_flip_gt_0p30"] = sum(1 for v in flips.values() if max(v["flip_hemi"], v["flip_beta"]) > 0.30)
    json.dump(cj, open(OUT / "cells_c1.json", "w"), indent=1, default=float)
    inputs = {str(p): C.sha256(p) for p in [C.UG / "user_grid_summary.json", C.UG / "user_grid_tables.csv", C.VE, C.A5 / "veto_correction.json", F_ND_CSV, SCALES_CSV,
              C.S / "wp-a5/attach_cells.py", C.S / "wp-a2/n1_wpa2.py", C.S / "wp-a5/veto_weight.py"] + [C.W / "cells" / ("cells_%d.npz" % l) for l in (8402, 8403, 8404, 8405)]
              + [C.L.BASE + "s%d.npz" % l for l in C.LABELS_C1]}
    gc = dict(frame="A", rule="finest of q5/q4/q3 per cell with every p_T*^2 bin passing D7+ (+ MC-stat >=500/>=20, inter-cell<0.5, width>=1.5(sigma+|bias|)) in both charges; z axis fixed",
              hemisphere="P if x_F>0 else G (x_F in gamma*p/X frame, proton-positive)", beta_split=C.BETA_SPLIT, zstar_split=C.ZSTAR_SPLIT,
              pt2_edges=C.PT2_EDGES, cells={cn: dict(pt2_grid=grid[cn], zstar=C.ZSTAR_EDGES) for cn in C.CELLS}, cells_if_frame_B_rule=gridB,
              fiducial="Q2>150, 0.1<y<0.7, x_IP<0.03, M_Y<1.6, |t|<1, 4<=M_X<=40, Q2 and M_X integrated; |eta_lab|<1.6, 0.15<=pT<=15; eta_max(0.8)<3.0 is the selection",
              reco_subbin_top_split=C.TOP_SPLIT, inputs_sha256=inputs, created_utc=cj["created_utc"])
    blob = json.dumps(gc, indent=1, sort_keys=True).encode(); (OUT / "grid_c1.json").write_bytes(blob)
    import hashlib
    (OUT / "grid_c1.json.sha256").write_text("%s  %s\n" % (hashlib.sha256(blob).hexdigest(), OUT / "grid_c1.json"))
    nf = {fr: dict(n_rows=len(allrows[fr]), n_fail=sum(not r["pass_D7plus"] for r in allrows[fr] if r["grid"] in (grid[r["cell_name"]], "z"))) for fr in "AB"}
    print(json.dumps(dict(grid=grid, gridB=gridB, frames=nf, checks=chk, beta=bp), indent=1, default=float))


if __name__ == "__main__":
    main()

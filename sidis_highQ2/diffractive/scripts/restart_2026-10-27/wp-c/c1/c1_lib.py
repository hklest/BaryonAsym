#!/usr/bin/env python3
"""WP-C C1 library: frozen gen-grid definitions (D6/D16/D18-D22, plan 2026-10-28-wp-c-user-grid C1-1..C1-8), the 840x-cocktail
loader and the D7+ row rule.  Owns its label list and loaders; no shared script is edited.  Login-node safe (npz only)."""
import csv, hashlib, json, sys
from pathlib import Path
import numpy as np

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
S = D / "scripts/restart_2026-10-27"
W = D / "data/restart_2026-10-27/wp-c/c1"
UG = D / "data/restart_2026-10-27/wp-c/user_grid_2026-10-28"
for p in (S / "wp-a5", S / "wp-c/study", S / "wp-a6", S / "wp-a6/edge30", S / "wp-a"):
    sys.path.insert(0, str(p))
import wpc_lib as L
import veto_weight as VW
import xframe_obs as XF
X = XF.X                                                                 # esigma_xf: gen_mass, M_PION

LABELS_C1 = [8402, 8403, 8404, 8405, 9024, 9033, 9155, 9157, 9158]      # 9012 excluded (D23 iv)
TWIN_STREAM = {8402: 0, 8405: 0, 8403: 1, 8404: 1}                       # 8402==8405 (e+, gen 5644), 8403==8404 (e-, gen 5643)
BETA_SPLIT = 0.523                                                       # frozen at 3 decimals from data median 0.5235 (D20)
ZSTAR_SPLIT = 0.2                                                        # Q9 option (a)
ZSTAR_EDGES = [0.0, 0.2, None]                                           # z* = 2E*/M_X; last bin open
# frame-A data quantiles of p_T*^2 (GeV^2), pooled over hemispheres and beta bins (user_grid_summary.json, 3 decimals); last bin open.
PT2_EDGES = {"q5": [0.0, 0.039, 0.096, 0.205, 0.494], "q4": [0.0, 0.051, 0.141, 0.386], "q3": [0.0, 0.074, 0.268]}
GRIDS = ("q5", "q4", "q3")                                               # finest first
CELLS = ("G_lo", "G_hi", "P_lo", "P_hi")                                 # index = hemi*2 + beta_bin
Z_TOP_HI = 1.0                                                           # effective z* upper limit for widths / sub-bin split
PT2_TOP_SPLIT_FACTOR = 2.0                                               # fallback only (TOP_SPLIT unset): split of the open top p_T*^2 bin at 2 x lower edge
TOP_SPLIT = {}                                                           # reco 2x sub-bin split of the open top bins = weighted MC median (set by c1_tables.main; key 'z', 'q5', 'q4', 'q3')
# D7 and D7+ thresholds (binning_record.md section 3, D22) and the h1-unfolder additions (plan C1-7)
THR = dict(N=50.0, purity=0.35, stability=0.35, p_mig=0.40, f_fake=0.35, raw_subbin=100, raw_gen_840x=500, raw_element=20,
           intercell=0.5, width_core=2.0, width_bias=1.5, element_share=0.05, veto=0.05)
EC_EDGE, XIPMAX, MXLO, MXHI = 3.0, 0.03, 4.0, 40.0
VE = D / "data/restart_2026-10-27/gate_evidence/veto_eff.json"
A5 = D / "data/restart_2026-10-27/wp-a5"
KEEP_MID = "ec08<3.0|sentinel, x_IP<0.03, 4<=M_X<=40"


def sha256(p):
    return hashlib.sha256(open(p, "rb").read()).hexdigest()


def load_c1(prefix="s"):
    """Twin of wpc_lib.load over LABELS_C1: (T, E) with t_samp / e_samp = index into LABELS_C1 and t_eglob = global event row."""
    T = {}; E = {}
    for i, l in enumerate(LABELS_C1):
        d = np.load(L.BASE + "%s%s.npz" % (prefix, l))
        n = len(d["t_w"]); ne = len(d["e_w"])
        for k in d.files:
            if k.startswith("t_"): T.setdefault(k, []).append(d[k])
            elif k.startswith("e_"): E.setdefault(k, []).append(d[k])
        T.setdefault("t_samp", []).append(np.full(n, i)); E.setdefault("e_samp", []).append(np.full(ne, i))
        off = sum(len(x) for x in E["e_w"][:-1])
        T.setdefault("t_eglob", []).append(np.searchsorted(d["e_idx"], d["t_ev"]) + off)
    return {k: np.concatenate(v) for k, v in T.items()}, {k: np.concatenate(v) for k, v in E.items()}


def cells_file(l):
    return (W / "cells" / ("cells_%d.npz" % l)) if l in (8402, 8403, 8404, 8405) else (A5 / ("cells_%d.npz" % l))


def sample_c1():
    """-> (T, E, cell, ec, w0, g, emb, event, stream).  Mirrors propagate_wpa5.sample() (:38-49) on LABELS_C1.
    event = EventNumber (generator event; unique within a sample, shared by the twins); stream = TWIN_STREAM for 840x, 10+label else."""
    T, E = load_c1()
    cz = [np.load(cells_file(l)) for l in LABELS_C1]
    ei = np.concatenate([np.load(L.BASE + "s%d.npz" % l)["e_idx"] for l in LABELS_C1])
    if not np.array_equal(ei, np.concatenate([c["e_idx"] for c in cz])): sys.exit("FATAL: cells e_idx mismatch")
    cell = np.concatenate([c["cell"] for c in cz]).astype(int); ec = np.concatenate([c["ec08"] for c in cz]).astype(float)
    event = np.concatenate([c["event"] for c in cz]).astype(np.int64)
    stream = np.concatenate([np.full(len(c["event"]), TWIN_STREAM.get(l, 10 + l)) for l, c in zip(LABELS_C1, cz)])
    sc = json.load(open(VE))["fit_scales_fine"]["7071"]; sd = np.array([sc["D0"][0], sc["D1"][0], sc["D2"][0], 1.0])
    w0 = E["e_w"] * sd[np.where(cell >= 0, cell, 3)]
    g = T["t_eglob"]
    if not np.allclose(T["t_w"], E["e_w"][g], rtol=0, atol=0): sys.exit("FATAL: track weight != event weight")
    emb = VW.mx_index(E["e_mx"]); emb = np.where(emb < 3, emb, -1)
    return T, E, cell, ec, w0, g, emb, event, stream


def zstar(xf, pt2, M, m):
    with np.errstate(all="ignore"):
        return 2 * np.sqrt((xf * M / 2) ** 2 + pt2 + m ** 2) / M


def hemi(xf):
    """0 = G (x_F <= 0, photon side), 1 = P (x_F > 0, proton side), -1 undefined (nan).  x_F = 0 is assigned to G (C1-1)."""
    xf = np.asarray(xf, float)
    return np.where(np.isfinite(xf), (xf > 0).astype(int), -1)


def beta_bin(beta):
    beta = np.asarray(beta, float)
    return np.where(np.isfinite(beta), (beta >= BETA_SPLIT).astype(int), -1)


def cell(h, b):
    h = np.asarray(h); b = np.asarray(b)
    return np.where((h >= 0) & (b >= 0), h * 2 + b, -1)


def axis_edges(axis, grid):
    """finite lower edges + open top bin as list (last upper = inf)."""
    return [0.0, ZSTAR_SPLIT, np.inf] if axis == "z" else PT2_EDGES[grid] + [np.inf]


def nbins(axis, grid):
    return 2 if axis == "z" else len(PT2_EDGES[grid])


def halves(axis, grid):
    """2x reco edges (D22): midpoints of finite bins; open top bin split at Z_TOP_HI mid / 2 x lower edge."""
    e = axis_edges(axis, grid); out = []
    for lo, hi in zip(e[:-1], e[1:]):
        if np.isfinite(hi): mid = 0.5 * (lo + hi)
        else: mid = TOP_SPLIT.get("z" if axis == "z" else grid, 0.5 * (lo + Z_TOP_HI) if axis == "z" else PT2_TOP_SPLIT_FACTOR * lo)
        out += [lo, mid]
    return out + [np.inf]


def bin_k(edges, value):
    """bin index in an edge list with last upper inf; -1 for nan / negative."""
    v = np.asarray(value, float)
    k = np.searchsorted(np.asarray(edges, float), v, side="right") - 1
    return np.where(np.isfinite(v) & (v >= edges[0]), np.clip(k, 0, len(edges) - 2), -1)


def bin_index(c, axis, grid, value, sub=False):
    """flat index cell*nmax + k (gen layout; sub=True -> 2x reco layout); -1 outside.  One grid for all cells (grid name)."""
    e = halves(axis, grid) if sub else axis_edges(axis, grid)
    nb = len(e) - 1; k = bin_k(e, value)
    return np.where((np.asarray(c) >= 0) & (k >= 0), np.asarray(c) * nb + k, -1)


def d7plus_row(r):
    """(passes, reasons) from a binstats row dict (D7, D7+, h1-unfolder additions; C1-7).  NaN/empty fail."""
    f = lambda k: float(r[k])
    bad = [("N<50", not f("n_sig_pred") >= THR["N"]), ("purity<0.35", not f("purity") >= THR["purity"]),
           ("stability<0.35", not f("stability") >= THR["stability"]), ("p_mig<0.40", not f("p_mig") >= THR["p_mig"]),
           ("f_fake>0.35", not f("f_fake") <= THR["f_fake"]), ("raw_reco_subbin<100", not f("n_raw_subbin_min") >= THR["raw_subbin"]),
           ("raw840x_gen<500", not f("n_raw_gen_840x") >= THR["raw_gen_840x"]),
           ("raw840x_element<20", not f("n_raw_element_min") >= THR["raw_element"]),
           ("intercell>=0.5", not f("intercell_mig") < THR["intercell"]), ("width<2sigma", not str(r["width_ok_2s"]) in ("True", "1")),
           ("width<1.5(sigma+bias)", not str(r["width_ok_bias"]) in ("True", "1"))]
    why = [k for k, b in bad if b]
    return not why, why


def grid_rule(rows, frame="A"):
    """{cell_name: 'q5'|'q4'|'q3'}: per cell the finest grid with every p_T*^2 bin passing D7+ (C1-7) in both charges.
    rows = binstats dicts (axis 'pt2' used).  Falls back to q3 when none passes (C1-8: failing bins stay masked)."""
    out = {}
    for ci, cn in enumerate(CELLS):
        out[cn] = "q3"
        for gname in GRIDS:
            rr = [r for r in rows if r["frame"] == frame and r["axis"] == "pt2" and r["grid"] == gname and r["charge"] in ("h+", "h-")
                  and int(r["cell"]) == ci]
            if rr and all(str(r["pass_D7plus"]) in ("True", "1") for r in rr):
                out[cn] = gname; break
    return out


def pass_cols(r):
    p, why = d7plus_row(r)
    return p, ";".join(why)

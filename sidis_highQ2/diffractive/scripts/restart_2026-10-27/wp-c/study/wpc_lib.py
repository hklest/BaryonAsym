import numpy as np, glob
BASE = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-c/study/"
LABELS = ["9012", "9024", "9033", "9155", "9157", "9158"]
MX_EDGES = np.array([4., 10., 20., 40.])
XF_EDGES = np.array([-1, -.5, -.2, 0, .2, .5, 1.])
PT2_EDGES = np.array([0, .25, .75, 2, 10.])


def load(prefix="s"):
    T = {}; E = {}
    for i, l in enumerate(LABELS):
        d = np.load(BASE + "%s%s.npz" % (prefix, l))
        n = len(d["t_w"]); ne = len(d["e_w"])
        for k in d.files:
            if k.startswith("t_"):
                T.setdefault(k, []).append(d[k])
            elif k.startswith("e_"):
                E.setdefault(k, []).append(d[k])
        T.setdefault("t_samp", []).append(np.full(n, i)); E.setdefault("e_samp", []).append(np.full(ne, i))
        # event index within sample -> global offset
        off = sum(len(x) for x in E["e_w"][:-1])
        T.setdefault("t_eglob", []).append(np.searchsorted(d["e_idx"], d["t_ev"]) + off)
    T = {k: np.concatenate(v) for k, v in T.items()}; E = {k: np.concatenate(v) for k, v in E.items()}
    return T, E


def wstats(x, w=None):
    """returns dict n, mean, rms, iqr_sigma, core_sigma (2-sigma clipped, truncation corrected), median (weighted)"""
    x = np.asarray(x, float); w = np.ones_like(x) if w is None else np.asarray(w, float)
    m = np.isfinite(x) & np.isfinite(w) & (w > 0); x, w = x[m], w[m]
    if len(x) < 5:
        return dict(n=len(x), mean=np.nan, rms=np.nan, iqr=np.nan, core=np.nan, med=np.nan)
    mu = np.average(x, weights=w); rms = np.sqrt(np.average((x - mu) ** 2, weights=w))
    o = np.argsort(x); cw = np.cumsum(w[o]); cw /= cw[-1]
    q = lambda p: x[o][min(np.searchsorted(cw, p), len(x) - 1)]
    iqr = (q(.75) - q(.25)) / 1.349; med = q(.5)
    c, s = med, iqr if iqr > 0 else rms
    for _ in range(30):
        k = np.abs(x - c) < 2 * s
        if k.sum() < 5: break
        c = np.average(x[k], weights=w[k]); s2 = np.sqrt(np.average((x[k] - c) ** 2, weights=w[k])) / 0.8796
        if abs(s2 - s) < 1e-6 * s: s = s2; break
        s = s2
    return dict(n=len(x), mean=mu, rms=rms, iqr=iqr, core=s, med=med)


def cell_id(mx, obs, mx_edges, obs_edges_list):
    """flat cell id; -1 outside. obs_edges_list[i] edges for M_X bin i. Returns (id, mxbin, obsbin)."""
    mb = np.searchsorted(mx_edges, mx, side="right") - 1
    mb = np.where((mx >= mx_edges[0]) & (mx <= mx_edges[-1]) & np.isfinite(mx), np.minimum(mb, len(mx_edges) - 2), -1)
    ob = np.full(len(mx), -1)
    off = np.zeros(len(mx_edges), int)
    for i in range(1, len(mx_edges)):
        off[i] = off[i - 1] + len(obs_edges_list[i - 1]) - 1
    cid = np.full(len(mx), -1)
    for i in range(len(mx_edges) - 1):
        e = obs_edges_list[i]; m = (mb == i) & np.isfinite(obs) & (obs >= e[0]) & (obs <= e[-1])
        b = np.minimum(np.searchsorted(e, obs[m], side="right") - 1, len(e) - 2)
        ob[m] = b; cid[m] = off[i] + b
    return cid, mb, ob, off[-1] + len(obs_edges_list[-1]) - 1


def pur_stab(mx_r, obs_r, mx_g, obs_g, gv, w, mx_edges, edges_list):
    """weighted + raw purity/stability/fake per cell.  Reco denominators: all reco-selected tracks (any gen status)."""
    cr, _, _, nc = cell_id(mx_r, obs_r, mx_edges, edges_list)
    cg, _, _, _ = cell_id(mx_g, obs_g, mx_edges, edges_list)
    cg = np.where(gv, cg, -1)
    def hist(sel, ww):
        return np.bincount(sel[sel >= 0], weights=ww[sel >= 0], minlength=nc)
    same = np.where((cr >= 0) & (cr == cg), cr, -1)
    out = {}
    for tag, ww in (("w", w), ("raw", np.ones_like(w))):
        nr = hist(cr, ww); ng = hist(cg, ww); ns = hist(same, ww)
        nfake = hist(np.where(~gv, cr, -1), ww)
        with np.errstate(all="ignore"):
            out[tag] = dict(nreco=nr, ngen=ng, nsame=ns, purity=ns / nr, stab=ns / ng, fake=nfake / nr)
    return out

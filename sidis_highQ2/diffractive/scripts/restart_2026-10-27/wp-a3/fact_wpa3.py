#!/usr/bin/env python3
"""WP-A3 forward-veto response-map factorisation test (one Condor job over the
17 skims of skim_wpa3.py).  Written and frozen before any skim was read.

Samples (per EventNumber parity; A_pre as n1_wpa2.py; eta = etaMaxClus08 unless noted;
the frozen WP-A2 S3 set: N-1 eta cut 3.3, fit variable etaMaxClus04):
  CTRL  derivation/closure region: A_pre && 35<E-pz<70 && etaMaxClus08 >= 3.3 (finite);
        no forward veto, no gap (ND-dominated); disjoint from GAP by the eta cut.
  GAP   transfer region = G1' stack-C N-1 samples (odd): N1_PLUG = A_pre && (eta<3.3|sentinel)
        && selFmd && selFts; N1_FMD, N1_FTS analogously; SB = all three vetoes.
  FIT   cut-removed fit basis: A_pre && selFmd && ePlug<7 && selFts (no eta cut).
  H4    A_pre && 35<E-pz<70, no veto (edge test of the LAr response itself).
Map: joint veto state s = Plug class (ePlug edges 0,0.1,1,3,5,7,10,20,inf + non-finite)
  x selFmd x selFts (36 states).  r_s(x) = P_DATA(s|x) / P_MC(s|x) on CTRL even, MC = DIFF8 + ND(chain)
  with signed v3 weights and the chain exposure rescale (wpa_common), period-resolved MC
  by luminosity-share projection (s2_analyze_wpa2.proj_share).  Per-event MC weight
  w' = w * sum_k share_k r_s(x_k): it preserves the MC x-distribution (sum_s P_MC r_s = 1
  wherever DATA states are covered by MC).  x-cells with < 50 DATA or < 200 raw MC events
  fall back to the parent set; states with no MC keep r = 1 (coverage loss reported).
x-sets (Plug-independent): XA D cell; XB XA x period; XC XB x hatVtxZ{-35,-10,0,10,35} x
  E-pz{35,45,55,70} (GAP events outside the window clamped to the edge bins, unmatched to
  the middle bin; fractions reported); XD XB x (nPrt>0); XE XB x eta08 band {[3.3,3.4),
  [3.4,3.5), >=3.5}, GAP uses the [3.3,3.4) band (nearest-edge extrapolation).
Discriminants (odd split; G1 numbers, unchanged thresholds):
  T0 control closure: CTRL odd Plug N-1-like shape (G1 grid) max dev <= 0.10, |dfail| <= 0.02 per cell.
  T1 factorisation in eta: XE per-band DATA/MC Plug-fail-rate ratio per cell (reported).
  T2 transfer: (a) GAP Plug N-1 unit-area max|MC/DATA-1| (G1 0-50 GeV grid, and 0-10 GeV) <= 0.10
     and |f_fail,MC - f_fail,DATA| <= 0.02 per cell; (b) FMD/FTS N-1 max dev < 0.25 for one
     admissible cocktail (g1_eval_wpa.py grid); (ii) eta04 edge shape [2.8,3.4) 0.05 bins in FIT
     and H4: unit-area chi2 (DATA and MC stat, bins >= 20 DATA, ndf = nbins-1) p >= 0.05.
  Ruling input: "map factorises" iff for some x-set and some chain T0 and T2(a) pass.
Yield projection: per cell, signal (SIGNAL labels) Stage-B weight factor c = sum w'/sum w,
  ND Stage-B change, spread over x-sets, 20 Gaussian replicas of the even-split map tables
  (DATA Poisson, MC sumw2) for map statistics, and the odd transfer residual dfail/(1-f).
Test skim_identity_s4: unmapped odd C_N1_PLUG / C_SB per cell equal the WP-A2 S4 partials.
Variant run 2 (--plug-classes fine, pre-registered after run 1 showed the GAP f_fail closing but the
Plug shape not, before any fine-class output existed): Plug classes 0-10 GeV in 0.5 GeV steps
(first split at 0.1), then 10-20, 20-50, >50 GeV + non-finite; x-sets XA, XB, XC only.  Same
discriminants; it separates class coarseness (CTRL shape then closes) from transfer failure
(GAP shape stays > 0.10).  Driver bins and the GAP per-bin MC/DATA ratios are written."""
import argparse, itertools, json, math, sys
from pathlib import Path
import numpy as np
from scipy.stats import chi2 as CHI2
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a"))
import wpa_common as W

SK = W.D / "data/restart_2026-10-27/wp-a3/skims"
S4 = W.D / "data/restart_2026-10-27/wp-a2/s4_g1p/n1"
ALTS = ("7071", "7091", "nd4cat")
PL_E = np.array([0, .1, 1, 3, 5, 7, 10, 20, np.inf]); NPC = len(PL_E)  # 8 finite classes + 1 non-finite
NS = NPC * 4
G1E = np.linspace(0, 50, 101); E10 = np.linspace(0, 10, 21); IE = np.arange(-.5, 20.5)
EDGE = np.round(np.arange(2.8, 3.4001, .05), 4)
VZ = np.array([-35, -10, 0, 10, 35.]); EZ = np.array([35, 45, 55, 70.])
XSETS = ("XA", "XB", "XC", "XD", "XE"); PARENT = {"XB": "XA", "XC": "XB", "XD": "XB", "XE": "XB"}
CELLS = ("D0", "D1", "D2")


def set_classes(kind):
    global PL_E, NPC, NS
    if kind == "fine":
        PL_E = np.concatenate([[0, .1], np.arange(.5, 10.01, .5), [20, 50, np.inf]])
    NPC = len(PL_E); NS = NPC * 4


def load(l):
    z = np.load(SK / (l + ".npz")); o = {k: z[k] for k in z.files}
    for k in ("ec08", "ec04", "eplug", "empz", "vz", "w", "mirmy"): o[k] = o[k].astype(np.float64)
    return o


def masks(e, even):
    par = e["even"] if even else ~e["even"]
    ec = e["ec08"]; etc = (ec < 3.3) | (ec < -900); plg = e["eplug"] < 7.0
    win = np.isfinite(e["empz"]) & (e["empz"] > 35) & (e["empz"] < 70)
    return {"CTRL": par & win & np.isfinite(ec) & (ec >= 3.3), "N1_PLUG": par & etc & e["fmd"] & e["fts"],
            "N1_FMD": par & etc & plg & e["fts"], "N1_FTS": par & etc & e["fmd"] & plg, "SB": par & etc & e["fmd"] & plg & e["fts"],
            "FIT": par & e["fmd"] & plg & e["fts"], "H4": par & win}


def state(e):
    pc = np.where(np.isfinite(e["eplug"]), np.clip(np.searchsorted(PL_E, e["eplug"], side="right") - 1, 0, NPC - 2), NPC - 1)
    return pc * 4 + e["fmd"].astype(int) * 2 + e["fts"].astype(int)


def xindex(e, xs, k, gap):
    """x-cell index for period k (data: own period); returns (idx, n_x)."""
    c = np.where(e["cell"] >= 0, e["cell"], 3).astype(int)
    if xs == "XA": return c, 4
    p = np.full(c.size, k, int); b = c * 4 + p
    if xs == "XB": return b, 16
    if xs == "XC":
        iv = np.clip(np.searchsorted(VZ, e["vz"], side="right") - 1, 0, 3)
        ez = np.where(np.isfinite(e["empz"]), e["empz"], 50.0); ie = np.clip(np.searchsorted(EZ, ez, side="right") - 1, 0, 2)
        return (b * 4 + iv) * 3 + ie, 16 * 12
    if xs == "XD": return b * 2 + (e["nprt"] > 0).astype(int), 32
    if xs == "XE":
        ib = np.where(gap, 0, np.clip(np.searchsorted([3.4, 3.5], e["ec08"], side="right"), 0, 2)) if gap is not None else \
            np.clip(np.searchsorted([3.4, 3.5], e["ec08"], side="right"), 0, 2)
        return b * 3 + ib, 48


def periods_of(l, ch, kind):
    if kind == "data": return None
    rp = W.represented(l, ch); L = W.period_lumi()
    return [(W.PERIODS.index(g), L[g] / sum(L[q] for q in rp)) for g in rp]


DRV = []
def shape(dh, mh, s2=None):
    dv, b, nq = W.shape_metric(dh, mh); DRV.append(b); return dv


def edge_chi2(dh, mh, m2):
    q = dh >= 20
    if q.sum() < 2: return None
    d, m, v = dh[q], mh[q], m2[q]; D, M = d.sum(), m.sum()
    c2 = float(np.sum((d / D - m / M) ** 2 / (d / D ** 2 + v / M ** 2))); ndf = int(q.sum()) - 1
    return {"chi2": c2, "ndf": ndf, "p": float(CHI2.sf(c2, ndf)), "max_dev": float(np.max(np.abs((m / M) / (d / D) - 1)))}


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True); ap.add_argument("--replicas", type=int, default=20)
    ap.add_argument("--plug-classes", choices=("coarse", "fine"), default="coarse"); ap.add_argument("--xsets", default=",".join(XSETS))
    a = ap.parse_args(); set_classes(a.plug_classes); xsl = a.xsets.split(",")
    labs = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    E = {l: load(l) for l in W.DATA_LABELS + labs}
    res = {"plug_classes": a.plug_classes, "plug_edges": [float(x) for x in PL_E], "inputs": {l: W.sha256(SK / (l + ".npz")) for l in E}, "script_sha256": W.sha256(__file__)}
    # ---- skim_identity_s4 ----
    bad = []
    for l, e in E.items():
        kind = "data" if l in W.DATA_LABELS else "mc"; p = json.load(open(S4 / kind / l / "partial.json"))
        mk = masks(e, False); key = "Dcells_raw" if kind == "data" else "Dcells_signed"
        for mn, pm in (("N1_PLUG", "C_N1_PLUG"), ("SB", "C_SB")):
            got = [float((e["w"] if kind == "mc" else np.ones(e["w"].size))[mk[mn] & (e["cell"] == c)].sum()) for c in range(3)]
            ref = p["counts"][pm][key]
            if not np.allclose(got, ref, rtol=1e-9, atol=1e-9): bad.append([l, mn, got, ref])
    res["skim_identity_s4"] = {"PASS": not bad, "mismatch": bad}
    if bad: json.dump(res, open(a.out, "w"), indent=1); raise SystemExit("FATAL: skim_identity_s4 " + str(bad[:2]))
    # ---- per-sample precomputations ----
    ST = {l: state(e) for l, e in E.items()}
    MK = {(l, ev): masks(e, ev) for l, e in E.items() for ev in (True, False)}
    gapflag = {l: ~((E[l]["ec08"] >= 3.3) & np.isfinite(E[l]["ec08"])) for l in E}
    res["gap_empz_outside_window_frac"] = {}
    dg = sum(int(MK[(l, False)]["N1_PLUG"].sum()) for l in W.DATA_LABELS)
    do = sum(int((MK[(l, False)]["N1_PLUG"] & ~(np.isfinite(E[l]["empz"]) & (E[l]["empz"] > 35) & (E[l]["empz"] < 70))).sum()) for l in W.DATA_LABELS)
    res["gap_empz_outside_window_frac"]["data_odd_N1_PLUG"] = do / dg
    out = {}
    for ch in ALTS:
        comps = W.DIFF8 + W.CHAINS[ch]["nd"]; rs = {l: W.fac_used(l, ch) / W.table_fac(l) for l in comps}
        PER = {l: periods_of(l, ch, "mc") for l in comps}
        def xi(l, xs, k, gap=False):
            e = E[l]; return xindex(e, xs, k if k is not None else 0, gapflag[l] if gap else None)
        def tables(xs, jit=None):
            nx = xindex(E["ep0304"], xs, 0, None)[1]
            Nd = np.zeros((NS, nx)); Wm = np.zeros((NS, nx)); Vm = np.zeros((NS, nx)); Nm = np.zeros((NS, nx))
            for l in W.DATA_LABELS:
                m = MK[(l, True)]["CTRL"]; k = int(E[l]["period"][m][0]) if m.any() else 0
                ix, _ = xindex({kk: v[m] for kk, v in E[l].items()}, xs, k, None)
                np.add.at(Nd, (ST[l][m], ix), 1.0)
            for l in comps:
                m = MK[(l, True)]["CTRL"]; sub = {kk: v[m] for kk, v in E[l].items()}; w = rs[l] * E[l]["w"][m]
                for k, sh in PER[l]:
                    ix, _ = xindex(sub, xs, k, None)
                    np.add.at(Wm, (ST[l][m], ix), sh * w); np.add.at(Vm, (ST[l][m], ix), (sh * w) ** 2); np.add.at(Nm, (ST[l][m], ix), 1.0)
            return Nd, Wm, Vm, Nm
        need = set(xsl) | {PARENT[x] for x in xsl if x in PARENT} | {"XA"}
        need |= {PARENT[x] for x in list(need) if x in PARENT}
        TB = {xs: tables(xs) for xs in XSETS if xs in need or xs == "XE"}
        def rmap(xs, rng=None):
            Nd, Wm, Vm, Nm = TB[xs]
            if rng is not None:
                Nd = rng.poisson(Nd).astype(float); Wm = np.maximum(Wm + rng.normal(size=Wm.shape) * np.sqrt(Vm), 0)
            Pd = Nd / np.maximum(Nd.sum(0), 1e-300); Pm = Wm / np.maximum(Wm.sum(0), 1e-300)
            R = np.where(Wm > 0, Pd / np.where(Pm > 0, Pm, 1), 1.0)
            thin = (Nd.sum(0) < 50) | (Nm.sum(0) < 200)
            if xs in PARENT and thin.any():
                Rp = rmap(PARENT[xs], rng); n_x = R.shape[1]
                # parent cell of each child cell: XB->XA by //4; XC->XB //12; XD->XB //2; XE->XB //3
                div = {"XB": 4, "XC": 12, "XD": 2, "XE": 3}[xs]
                par = np.arange(n_x) // div
                R[:, thin] = Rp[:, par[thin]]
            elif thin.any():
                R[:, thin] = 1.0
            return R
        def wmap(l, R, xs, gap):
            e = E[l]; s = ST[l]; acc = np.zeros(e["w"].size)
            for k, sh in PER[l]:
                ix, _ = xindex(e, xs, k, gapflag[l] if gap else None); acc += sh * R[s, ix]
            return acc
        def evaluate(Wt, Wg):
            """Wt: per-label MC weight arrays for CTRL/FIT/H4 (no gap extrapolation); Wg: for GAP samples."""
            r = {}
            def dh(mn, v, e_, cell=None):
                return sum(np.histogram(E[l][v][MK[(l, False)][mn] & ((E[l]["cell"] == cell) if cell is not None else True)], e_)[0] for l in W.DATA_LABELS).astype(float)
            def mh(mn, v, e_, WW, cell=None, sq=False, cl=None):
                return sum(np.histogram(E[l][v][MK[(l, False)][mn] & ((E[l]["cell"] == cell) if cell is not None else True)], e_,
                           weights=((WW[l] ** 2) if sq else WW[l])[MK[(l, False)][mn] & ((E[l]["cell"] == cell) if cell is not None else True)])[0] for l in (cl or comps))
            def ff(mn_all, WW, cell):
                dn = sum(int((MK[(l, False)][mn_all] & (E[l]["cell"] == cell)).sum()) for l in W.DATA_LABELS)
                dp = sum(int((MK[(l, False)][mn_all] & (E[l]["cell"] == cell) & (E[l]["eplug"] < 7)).sum()) for l in W.DATA_LABELS)
                mn = sum(WW[l][MK[(l, False)][mn_all] & (E[l]["cell"] == cell)].sum() for l in comps)
                mp = sum(WW[l][MK[(l, False)][mn_all] & (E[l]["cell"] == cell) & (E[l]["eplug"] < 7)].sum() for l in comps)
                fd = 1 - dp / dn; return {"data": fd, "data_err": math.sqrt(fd * (1 - fd) / dn), "mc": 1 - mp / mn, "delta": (1 - mp / mn) - fd}
            for reg, WW in (("CTRL", Wt), ("N1_PLUG", Wg)):
                r[reg] = {"plug_shape_G1grid": shape(dh(reg, "eplug", G1E), mh(reg, "eplug", G1E, WW)),
                          "plug_shape_0_10": shape(dh(reg, "eplug", E10), mh(reg, "eplug", E10, WW)),
                          "f_fail": {"D%d" % c: ff(reg, WW, c) for c in range(3)}}
                r[reg]["driver_bins"] = DRV[-2:]
                d_ = dh(reg, "eplug", G1E); m_ = mh(reg, "eplug", G1E, WW); q = d_ >= 20
                r[reg]["G1grid_ratio_unit_area"] = [(float((m_[i] / m_[q].sum()) / (d_[i] / d_[q].sum())) if q[i] else None) for i in range(len(d_))]
            r["N1_FMD"] = shape(dh("N1_FMD", "nfmu12", IE), mh("N1_FMD", "nfmu12", IE, Wg))
            r["N1_FTS"] = shape(dh("N1_FTS", "nfts28", IE), mh("N1_FTS", "nfts28", IE, Wg))
            # admissible scan (g1_eval_wpa.py grid)
            groups = W.DIFF8 + ["ND"]; Hs = {}
            for mn, v in (("N1_FMD", "nfmu12"), ("N1_FTS", "nfts28")):
                rows = [mh(mn, v, IE, Wg, cl=[l]) for l in W.DIFF8] + [mh(mn, v, IE, Wg, cl=W.CHAINS[ch]["nd"])]
                Hs[v] = (np.vstack(rows), dh(mn, v, IE))
            combos = np.array(list(itertools.product([.5, .75, 1, 1.25, 1.5], repeat=len(groups)))); best = np.inf
            for s0 in range(0, len(combos), 200000):
                C = combos[s0:s0 + 200000]; dv = []
                for v, (Hm, d_) in Hs.items():
                    q = d_ >= 20; dn = d_[q] / d_[q].sum(); M = C @ Hm[:, q]; M = M / M.sum(1, keepdims=True); dv.append(np.max(np.abs(M / dn - 1), 1))
                best = min(best, float(np.min(np.maximum(*dv))))
            r["G1b_best_joint"] = best
            r["edge"] = {}
            for reg, WW in (("FIT", Wt), ("H4", Wt)):
                for sc in ("ALL",) + CELLS:
                    cell = None if sc == "ALL" else int(sc[1])
                    r["edge"]["%s|%s" % (reg, sc)] = edge_chi2(dh(reg, "ec04", EDGE, cell), mh(reg, "ec04", EDGE, WW, cell), mh(reg, "ec04", EDGE, WW, cell, sq=True))
            # signal / ND Stage-B factors
            r["SB"] = {}
            for c in range(3):
                z = "D%d" % c; sel = lambda l: MK[(l, False)]["SB"] & (E[l]["cell"] == c)
                r["SB"][z] = {"data": sum(int(sel(l).sum()) for l in W.DATA_LABELS),
                              "signal_mc": float(sum(Wg[l][sel(l)].sum() for l in W.SIGNAL)),
                              "nd_mc": float(sum(Wg[l][sel(l)].sum() for l in W.CHAINS[ch]["nd"])),
                              "all_mc": float(sum(Wg[l][sel(l)].sum() for l in comps))}
            return r
        base = {l: rs[l] * E[l]["w"] for l in comps}
        o = {"unmapped": evaluate(base, base), "maps": {}, "T1_band_ratio": {}}
        for xs in xsl:
            R = rmap(xs)
            Wt = {l: base[l] * wmap(l, R, xs, False) for l in comps}; Wg = {l: base[l] * wmap(l, R, xs, True) for l in comps}
            Nd, Wm, _, _ = TB[xs]; cov = float(Nd[Wm.sum(1) <= 0].sum() / Nd.sum()) if False else float(Nd[(Wm <= 0)].sum() / Nd.sum())
            ev = evaluate(Wt, Wg); ev["data_state_coverage_loss"] = cov
            if xs in ("XA", "XB", "XC"):
                rng = np.random.default_rng(20261027); reps = []
                for _ in range(a.replicas):
                    Rr = rmap(xs, rng); Wr = {l: base[l] * wmap(l, Rr, xs, True) for l in W.SIGNAL + W.CHAINS[ch]["nd"]}
                    reps.append([[float(sum(Wr[l][MK[(l, False)]["SB"] & (E[l]["cell"] == c)].sum() for l in W.SIGNAL)),
                                  float(sum(Wr[l][MK[(l, False)]["SB"] & (E[l]["cell"] == c)].sum() for l in W.CHAINS[ch]["nd"]))] for c in range(3)])
                reps = np.array(reps); ev["replica_rel_sd"] = {"signal": (reps[:, :, 0].std(0) / reps[:, :, 0].mean(0)).tolist(),
                                                               "nd": (reps[:, :, 1].std(0) / reps[:, :, 1].mean(0)).tolist()}
            o["maps"][xs] = ev
        # T1: per-band DATA/MC Plug fail ratio in CTRL even, per cell (no map)
        Nd, Wm, Vm, _ = TB["XE"]; fail = np.array([PL_E[min(s // 4, NPC - 2)] >= 7 or (s // 4) == NPC - 1 for s in range(NS)])
        for c in range(4):
            row = []
            for ib in range(3):
                cols = [((c * 4 + k) * 3 + ib) for k in range(4)]
                nd_ = Nd[:, cols].sum(1); wm_ = Wm[:, cols].sum(1)
                fd = nd_[fail].sum() / nd_.sum(); fm = wm_[fail].sum() / wm_.sum()
                row.append({"band": ["3.3-3.4", "3.4-3.5", ">=3.5"][ib], "n_data": float(nd_.sum()), "f_data": float(fd), "f_mc": float(fm), "ratio": float(fd / fm) if fm > 0 else None})
            o["T1_band_ratio"]["D%d" % c if c < 3 else "out"] = row
        out[ch] = o
        print(ch, "done", flush=True)
    res["chains"] = out
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(res, open(a.out, "w"), indent=1)
    print(json.dumps({"out": a.out, "skim_identity_s4": res["skim_identity_s4"]["PASS"]}))


if __name__ == "__main__":
    main()

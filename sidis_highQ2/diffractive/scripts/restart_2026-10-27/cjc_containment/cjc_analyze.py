#!/usr/bin/env python3
"""CJC-containment study (restart 2026-10-28): one Condor job over the 14 cjc_skim.py npz -> cjc_containment.json.

Samples (D cells only, as veto_eff_hist.py ALL; base = A_pre && (etaMaxClus08<3.3 | sentinel), from the skim):
  KIN  = base && 0 < xIp < 0.03 && 4 <= mx <= 40 (reco)
  LRG  = KIN && ePlug<7 && selFmd && selFts;   N-1: PLUG = KIN && fmd && fts, FMD = KIN && plug && fts,
  FTS = KIN && fmd && plug, COMB = KIN (fail = any veto fails).
Containment definitions (current hemisphere = x_F<0, frame B, Pomeron-positive x_F):
  TRK (nominal): >= 1 current-hemisphere track and >= 90% of its track energy at |eta| < 1.7354 (20-160 deg)
  TRKS: >= 1 current track and none outside;   PROXY: lab eta_hi of the massless current-hemisphere image < 1.7354
  categories: C (contained), N (frame defined, not contained), U (undefined: no ChargedTot row / no current track).
MC: full = s_diff(cell) DIFF8 + s_ND(cell) ND(chain) with the G1'(c) fine scales stored in gate_evidence/veto_eff.json;
  DIFF8-only = s_diff DIFF8; weights signed v3 x fac_used/table_fac (hist_ge.py basis).
Check: odd Plug N-1 f_fail per D cell without KIN (data, unit-scale MC) equals veto_eff.json / hist_ge.py."""
import argparse, json, sys
from pathlib import Path
import numpy as np

H = Path(__file__).resolve().parent
sys.path.insert(0, str(H.parent / "wp-a"))
import wpa_common as W

SK = W.D / "data/restart_2026-10-27/cjc_containment/skims"
VE = W.D / "data/restart_2026-10-27/gate_evidence/veto_eff.json"
ECJC = float(-np.log(np.tan(np.radians(10.0))))
CHAINS = ("7071", "7091")
SIGNAL, REGGEON = W.SIGNAL, ["6671", "6677"]
MXB = [4., 10., 20., 40.0001]
ETAE = np.round(3.3 - 0.25 * np.arange(18, -1, -1), 4)          # -1.2 ... 3.3 in 0.25
VARS = {"ec08": ETAE, "xip": np.linspace(0, .03, 13), "mx": np.array([4, 6, 8, 10, 13, 16, 20, 25, 30, 40.0001]),
        "beta": np.linspace(0, 1, 21), "ys": np.linspace(.1, .9, 17), "yX": np.linspace(-3, 3, 31), "eta_hi": np.linspace(-2, 4, 31)}
DEFS = ("TRK", "TRKS", "PROXY")
CELLS = ("D0", "D1", "D2", "ALL")
PAR = ("odd", "even", "both")


def load(l):
    z = np.load(SK / (l + ".npz")); e = {k: z[k] for k in z.files}
    for k in ("w", "ec08", "eplug", "xip", "mx", "beta", "ys", "eta_hi", "eta_lo", "yX", "E_cur", "E_cur_in", "E_cur_fwd", "E_cur_bwd", "Yes", "Empz"):
        e[k] = e[k].astype(np.float64)
    ec = e["ec08"]; xip, mx = e["xip"], e["mx"]
    e["incell"] = e["cell"] >= 0
    e["kin"] = e["incell"] & np.isfinite(xip) & (xip > 0) & (xip < .03) & np.isfinite(mx) & (mx >= 4) & (mx <= 40)
    e["plug"] = e["eplug"] < 7.0
    e["lrg"] = e["kin"] & e["plug"] & e["fmd"] & e["fts"]
    e["meas"] = np.isfinite(e["Yes"]) & (e["Yes"] > .1) & (e["Yes"] < .7) & np.isfinite(e["Empz"]) & (e["Empz"] > 35) & (e["Empz"] < 70)
    frame = (e["n_row"] > 0) & np.isfinite(e["eta_hi"])
    with np.errstate(all="ignore"):
        fr = e["E_cur_in"] / np.where(e["E_cur"] > 0, e["E_cur"], np.nan)
    has = frame & (e["n_cur"] > 0)
    e["cat"] = {}
    for dn, c in (("TRK", has & (fr >= 0.9)), ("TRKS", has & (e["n_cur_out"] == 0)), ("PROXY", frame & (e["eta_hi"] < ECJC))):
        und = ~has if dn != "PROXY" else ~frame
        e["cat"][dn] = {"C": c, "N": ~c & ~und, "U": und}
    if "gE_cur" in e:
        with np.errstate(all="ignore"):
            gfr = e["gE_cur_in"] / np.where(e["gE_cur"] > 0, e["gE_cur"], np.nan)
        e["gen_C"] = (e["gn_cur"] > 0) & (gfr >= 0.9); e["gen_def"] = e["gn_cur"] > 0
    return e


def sdiv(a, b):
    return a / b if b else None


def par(e, p):
    return np.ones(e["w"].size, bool) if p == "both" else (e["even"] if p == "even" else ~e["even"])


def cellm(e, c):
    return e["incell"] if c == "ALL" else (e["cell"] == int(c[1]))


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True); a = ap.parse_args()
    labs = W.DIFF8 + ["7071", "7091"]
    E = {l: load(l) for l in W.DATA_LABELS + labs}
    SC = json.load(open(VE))["fit_scales_fine"]
    WT = {}   # chain -> label -> {"full": w, "diff8": w, "unit": w}
    for ch in CHAINS:
        WT[ch] = {}
        for l in W.DIFF8 + [ch]:
            e = E[l]; b = W.fac_used(l, ch) / W.table_fac(l) * e["w"]
            s = np.ones(b.size)
            for c in range(3): s[e["cell"] == c] = SC[ch]["D%d" % c][0 if l in W.DIFF8 else 1]
            WT[ch][l] = {"full": b * s, "unit": b}
    res = {"script_sha256": W.sha256(__file__), "inputs": {l: W.sha256(SK / (l + ".npz")) for l in E}, "eta_cjc": ECJC,
           "fit_scales_fine": {c: SC[c] for c in CHAINS}, "mx_bins": MXB, "hist_edges": {k: list(map(float, v)) for k, v in VARS.items()},
           "checks": {}, "counts": {}, "fractions": {}, "hists": {}, "veto": {}, "stats": {}, "proxy": {}, "quantiles": {}}

    def dsum(mask_fn):  # data count
        return int(sum(int(mask_fn(E[l]).sum()) for l in W.DATA_LABELS))

    def msum(ch, mask_fn, which="full", ls=None):
        ls = ls or (W.DIFF8 + [ch]); s = s2 = 0.0
        for l in ls:
            m = mask_fn(E[l]); ww = WT[ch][l][which][m]; s += ww.sum(); s2 += (ww ** 2).sum()
        return float(s), float(s2)

    # ---- check vs veto_eff.json (odd Plug N-1 f_fail per cell, no KIN) ----
    VJ = json.load(open(VE))["checks"] if "checks" in json.load(open(VE)) else {}
    chk = {}
    for c in range(3):
        n1 = lambda e, c=c: (~e["even"]) & (e["cell"] == c) & e["fmd"] & e["fts"]
        dn = dsum(n1); df = dsum(lambda e: n1(e) & ~e["plug"])
        mn, _ = msum("7071", n1, "unit"); mf, _ = msum("7071", lambda e: n1(e) & ~e["plug"], "unit")
        chk["D%d" % c] = {"f_data": df / dn, "f_mc_unit_7071": mf / mn}
    ref_d = [0.2724, 0.2996, 0.3365]; ref_m = [0.062, 0.083, 0.113]
    chk["matches_veto_eff_view"] = bool(all(abs(chk["D%d" % c]["f_data"] - ref_d[c]) < 6e-5 and abs(chk["D%d" % c]["f_mc_unit_7071"] - ref_m[c]) < 6e-4 for c in range(3)))
    res["checks"]["plug_n1_odd_vs_veto_eff"] = chk; print("check", json.dumps(chk), flush=True)
    if not chk["matches_veto_eff_view"]: raise SystemExit("FATAL: preselection does not reproduce veto_eff view")

    # ---- counts / fractions of the LRG sample per definition, cell, parity ----
    for dn in DEFS:
        for c in CELLS:
            for p in PAR:
                k = "|".join((dn, c, p)); o = {}
                base = lambda e: e["lrg"] & cellm(e, c) & par(e, p)
                o["data"] = {g: dsum(lambda e, g=g: base(e) & e["cat"][dn][g]) for g in "CNU"}
                for ch in CHAINS:
                    o["mc_" + ch] = {g: msum(ch, lambda e, g=g: base(e) & e["cat"][dn][g]) for g in "CNU"}
                o["diff8"] = {g: msum("7071", lambda e, g=g: base(e) & e["cat"][dn][g], ls=W.DIFF8) for g in "CNU"}
                o["diff8_raw"] = {g: int(sum(int((base(E[l]) & E[l]["cat"][dn][g]).sum()) for l in W.DIFF8)) for g in "CNU"}
                res["counts"][k] = o
                tot = lambda d_: sum(v if not isinstance(v, (list, tuple)) else v[0] for v in d_.values())
                fC = {s: (o[s]["C"] if s in ("data", "diff8_raw") else o[s]["C"][0]) / max(tot(o[s]), 1e-300) for s in o}
                res["fractions"][k] = {"f_contained": fC,
                                       "f_contained_of_defined_data": o["data"]["C"] / max(o["data"]["C"] + o["data"]["N"], 1)}
    # eta_max fractions of contained events; converse
    for dn in DEFS:
        for p in PAR:
            o = {}
            for cut in (2.0, 2.5, 3.0):
                cm = lambda e: e["lrg"] & par(e, p) & e["cat"][dn]["C"]
                o["C_frac_eta_lt_%.1f" % cut] = {"data": dsum(lambda e: cm(e) & (e["ec08"] < cut)) / max(dsum(cm), 1),
                                                 **{"mc_" + ch: sdiv(msum(ch, lambda e: cm(e) & (e["ec08"] < cut))[0], msum(ch, cm)[0]) for ch in CHAINS}}
            o["C_frac_eta_3p0_3p3"] = {"data": dsum(lambda e: e["lrg"] & par(e, p) & e["cat"][dn]["C"] & (e["ec08"] >= 3.0)) / max(dsum(lambda e: e["lrg"] & par(e, p) & e["cat"][dn]["C"]), 1)}
            o["all_frac_eta_3p0_3p3"] = {"data": dsum(lambda e: e["lrg"] & par(e, p) & (e["ec08"] >= 3.0)) / max(dsum(lambda e: e["lrg"] & par(e, p)), 1)}
            for cut in (2.0, 2.5):
                bm = lambda e: e["lrg"] & par(e, p) & (e["ec08"] < cut)
                o["eta_lt_%.1f_frac_C" % cut] = {"data": dsum(lambda e: bm(e) & e["cat"][dn]["C"]) / max(dsum(bm), 1),
                                                 **{"mc_" + ch: sdiv(msum(ch, lambda e: bm(e) & e["cat"][dn]["C"])[0], msum(ch, bm)[0]) for ch in CHAINS},
                                                 "diff8": sdiv(msum("7071", lambda e: bm(e) & e["cat"][dn]["C"], ls=W.DIFF8)[0], msum("7071", bm, ls=W.DIFF8)[0]),
                                                 "n_data": dsum(bm)}
            res["fractions"]["eta|" + dn + "|" + p] = o
    # M_X / x_IP quantiles of contained and not-contained data (both parities)
    for dn in DEFS:
        for g in "CNU":
            v = {k: np.concatenate([E[l][k][E[l]["lrg"] & E[l]["cat"][dn][g]] for l in W.DATA_LABELS]) for k in ("mx", "xip", "ec08", "beta", "ys")}
            res["quantiles"][dn + "|" + g] = {k: (np.quantile(x, [0, .05, .5, .95, 1]).tolist() if x.size else None) for k, x in v.items()}
    # where the lost current-hemisphere energy goes for TRK not-contained data events (backward = eta <= -1.735)
    nm = {l: E[l]["lrg"] & E[l]["cat"]["TRK"]["N"] for l in W.DATA_LABELS}
    fw = np.concatenate([E[l]["E_cur_fwd"][nm[l]] for l in W.DATA_LABELS]); bw = np.concatenate([E[l]["E_cur_bwd"][nm[l]] for l in W.DATA_LABELS])
    res["quantiles"]["TRK|N|out_side"] = {"n": int(fw.size), "frac_backward_dominated": float((bw > fw).mean()) if fw.size else None,
                                          "E_out_fwd_sum": float(fw.sum()), "E_out_bwd_sum": float(bw.sum())}

    # ---- histograms (LRG, categories, cells, parity) ----
    def hist(x, ed, w):
        f = np.isfinite(x); xx = np.clip(x[f], ed[0], ed[-1] - 1e-9)   # under/overflow folded into edge bins
        return np.histogram(xx, ed, weights=w[f])[0], np.histogram(xx, ed, weights=(w ** 2)[f])[0]
    groups = lambda ch: {"signal": SIGNAL, "reggeon": REGGEON, "nd": [ch]}
    for dn in ("TRK", "PROXY"):
        for c in CELLS:
            for p in PAR:
                for g in ("C", "N", "U", "ALL"):
                    sel = lambda e: e["lrg"] & cellm(e, c) & par(e, p) & (np.ones(e["w"].size, bool) if g == "ALL" else e["cat"][dn][g])
                    for v, ed in VARS.items():
                        if v in ("yX", "eta_hi") and c != "ALL": continue
                        acc = np.zeros(len(ed) - 1)
                        for l in W.DATA_LABELS:
                            m = sel(E[l]); acc += hist(E[l][v][m], ed, np.ones(int(m.sum())))[0]
                        res["hists"]["|".join((dn, c, p, g, v, "data"))] = acc.tolist()
                        for ch in CHAINS:
                            for gn, ls in groups(ch).items():
                                s = np.zeros(len(ed) - 1); s2 = np.zeros(len(ed) - 1); raw = 0
                                for l in ls:
                                    m = sel(E[l]); h_, h2 = hist(E[l][v][m], ed, WT[ch][l]["full"][m]); s += h_; s2 += h2; raw += int(m.sum())
                                res["hists"]["|".join((dn, c, p, g, v, ch, gn))] = {"sumw": s.tolist(), "sumw2": s2.tolist(), "raw": raw}
    # 2D occupancy of contained data events (both parities): mx vs xip (log), mx vs ec08
    XL = np.logspace(-3.5, np.log10(.03), 16); ML = np.logspace(np.log10(4), np.log10(40.0001), 13); EL = ETAE
    for dn in ("TRK", "PROXY"):
        for g in ("C", "N"):
            mx = np.concatenate([E[l]["mx"][E[l]["lrg"] & E[l]["cat"][dn][g]] for l in W.DATA_LABELS])
            xi = np.concatenate([E[l]["xip"][E[l]["lrg"] & E[l]["cat"][dn][g]] for l in W.DATA_LABELS])
            ec = np.concatenate([E[l]["ec08"][E[l]["lrg"] & E[l]["cat"][dn][g]] for l in W.DATA_LABELS])
            res["hists"]["2d|%s|%s|mx_xip" % (dn, g)] = np.histogram2d(mx, np.clip(xi, XL[0], XL[-1] - 1e-12), [ML, XL])[0].tolist()
            res["hists"]["2d|%s|%s|mx_ec08" % (dn, g)] = np.histogram2d(mx, np.clip(ec, EL[0], EL[-1] - 1e-9), [ML, EL])[0].tolist()
    res["hist_edges"].update(mx_log=ML.tolist(), xip_log=XL.tolist())

    # ---- veto N-1 fail fractions: contained subset vs full KIN sample ----
    VS = {"PLUG": (lambda e: e["fmd"] & e["fts"], lambda e: ~e["plug"]), "FMD": (lambda e: e["plug"] & e["fts"], lambda e: ~e["fmd"]),
          "FTS": (lambda e: e["fmd"] & e["plug"], lambda e: ~e["fts"]), "COMB": (lambda e: np.ones(e["w"].size, bool), lambda e: ~(e["plug"] & e["fmd"] & e["fts"]))}
    for dn in DEFS:
        for sub in ("C", "ALL", "N"):
            for v, (keep, fail) in VS.items():
                for p in PAR:
                    for mb in ("all", "0", "1", "2"):
                        def smp(e):
                            m = e["kin"] & keep(e) & par(e, p)
                            if sub != "ALL": m &= e["cat"][dn][sub]
                            if mb != "all": m &= (e["mx"] >= MXB[int(mb)]) & (e["mx"] < MXB[int(mb) + 1])
                            return m
                        if dn != "TRK" and (mb != "all"): continue
                        n_ = dsum(smp); k_ = dsum(lambda e: smp(e) & fail(e)); fd = k_ / max(n_, 1)
                        o = {"n_data": n_, "f_data": fd, "err_data": float(np.sqrt(max(fd * (1 - fd), 0) / max(n_, 1)))}
                        for ch in CHAINS:
                            for which, ls in (("full", None), ("diff8", W.DIFF8)):
                                S_, S2 = msum(ch, smp, ls=ls); F_, F2 = msum(ch, lambda e: smp(e) & fail(e), ls=ls)
                                P_, P2 = S_ - F_, S2 - F2; f = F_ / S_ if S_ > 0 else np.nan
                                o["f_%s_%s" % (which, ch)] = f
                                o["err_%s_%s" % (which, ch)] = float(np.sqrt(F2 * (1 - f) ** 2 + P2 * f ** 2) / S_) if S_ > 0 else None
                                if which == "diff8": o["purity_" + ch] = sdiv(S_, msum(ch, smp)[0])
                            if o["err_full_" + ch] is not None and n_ > 0:
                                o["delta_" + ch] = fd - o["f_full_" + ch]
                                o["err_delta_" + ch] = float(np.hypot(o["err_data"], o["err_full_" + ch]))
                                o["pass_eff_ratio_" + ch] = (1 - fd) / (1 - o["f_full_" + ch])
                        res["veto"]["|".join((dn, sub, v, p, mb))] = o
    # purity of the LRG sample itself
    for dn in DEFS:
        for sub in ("C", "N", "ALL"):
            for c in CELLS:
                sm = lambda e: e["lrg"] & cellm(e, c) & (np.ones(e["w"].size, bool) if sub == "ALL" else e["cat"][dn][sub])
                res["veto"]["LRGpurity|%s|%s|%s" % (dn, sub, c)] = {ch: sdiv(msum(ch, sm, ls=W.DIFF8)[0], msum(ch, sm)[0]) for ch in CHAINS}

    # ---- statistics per M_X bin ----
    for dn in DEFS:
        for sub in ("C", "ALL"):
            for meas in (False, True):
                for p in PAR:
                    rows = []
                    for ib in range(3):
                        sm = lambda e: e["lrg"] & par(e, p) & (e["mx"] >= MXB[ib]) & (e["mx"] < MXB[ib + 1]) & \
                            (e["cat"][dn]["C"] if sub == "C" else True) & (e["meas"] if meas else True)
                        r = {"mx_bin": [MXB[ib], round(MXB[ib + 1], 3)], "n_data": dsum(sm),
                             "n_had_cur_data": int(sum(int(E[l]["n_meas_cur"][sm(E[l])].sum()) for l in W.DATA_LABELS)),
                             "n_had_all_data": int(sum(int(E[l]["n_meas"][sm(E[l])].sum()) for l in W.DATA_LABELS)),
                             "raw_9012": int(sm(E["9012"]).sum()), "raw_9012_had_cur": int(E["9012"]["n_meas_cur"][sm(E["9012"])].sum()),
                             "raw_signal6": int(sum(int(sm(E[l]).sum()) for l in SIGNAL)),
                             "raw_signal6_had_cur": int(sum(int(E[l]["n_meas_cur"][sm(E[l])].sum()) for l in SIGNAL)),
                             "raw_diff8": int(sum(int(sm(E[l]).sum()) for l in W.DIFF8)),
                             "diff8_pred_7071": msum("7071", sm, ls=W.DIFF8)[0], "full_pred_7071": msum("7071", sm)[0]}
                        rows.append(r)
                    res["stats"]["|".join((dn, sub, "meas" if meas else "lrg", p))] = rows

    # ---- proxy vs full definition; gen-level truth (DIFF8) ----
    def conf(a_, b_, w_):
        return {"CC": float(w_[a_ & b_].sum()), "CN": float(w_[a_ & ~b_].sum()), "NC": float(w_[~a_ & b_].sum()), "NN": float(w_[~a_ & ~b_].sum())}
    def defined(e, dn): return ~e["cat"][dn]["U"]
    out = {}
    for x_, y_ in (("TRK", "PROXY"), ("TRK", "TRKS")):
        m_ = lambda e: e["lrg"] & defined(e, x_) & defined(e, y_)
        acc = {"CC": 0, "CN": 0, "NC": 0, "NN": 0}
        for l in W.DATA_LABELS:
            e = E[l]; m = m_(e); cc = conf(e["cat"][x_]["C"][m], e["cat"][y_]["C"][m], np.ones(int(m.sum())))
            acc = {k: acc[k] + cc[k] for k in acc}
        out["data|%s_vs_%s" % (x_, y_)] = acc
        acc = {"CC": 0, "CN": 0, "NC": 0, "NN": 0}
        for l in W.DIFF8:
            e = E[l]; m = m_(e); cc = conf(e["cat"][x_]["C"][m], e["cat"][y_]["C"][m], WT["7071"][l]["full"][m])
            acc = {k: acc[k] + cc[k] for k in acc}
        out["diff8|%s_vs_%s" % (x_, y_)] = acc
    for dn in DEFS:   # gen truth (charged X-side hadrons, all eta, G2 frame) vs reco definition
        acc = {"CC": 0, "CN": 0, "NC": 0, "NN": 0}; acc_g = {"CC": 0, "CN": 0, "NC": 0, "NN": 0}
        for l in W.DIFF8:
            e = E[l]; m = e["lrg"] & defined(e, dn) & e["gen_def"]; ww = WT["7071"][l]["full"][m]
            cc = conf(e["cat"][dn]["C"][m], e["gen_C"][m], ww); acc = {k: acc[k] + cc[k] for k in acc}
            mg = m & np.isfinite(e["g_eta_hi"])
            cg = conf(e["g_eta_hi"][mg] < ECJC, e["gen_C"][mg], WT["7071"][l]["full"][mg]); acc_g = {k: acc_g[k] + cg[k] for k in acc_g}
        out["gen|reco_%s_vs_genTRK" % dn] = acc
        if dn == "PROXY": out["gen|genPROXY_vs_genTRK"] = acc_g
    for k, v in out.items():
        t = sum(v.values()); v["agree"] = (v["CC"] + v["NN"]) / t if t else None
        v["eff_x_given_y"] = v["CC"] / (v["CC"] + v["NC"]) if (v["CC"] + v["NC"]) else None   # x contained | y contained
        v["pur_y_given_x"] = v["CC"] / (v["CC"] + v["CN"]) if (v["CC"] + v["CN"]) else None   # y contained | x contained
    # y_X: correlation with eta_hi, and the y_X cut reproducing the PROXY decision best (data)
    yx = np.concatenate([E[l]["yX"][E[l]["lrg"]] for l in W.DATA_LABELS]); eh = np.concatenate([E[l]["eta_hi"][E[l]["lrg"]] for l in W.DATA_LABELS])
    tc = np.concatenate([E[l]["cat"]["TRK"]["C"][E[l]["lrg"]] for l in W.DATA_LABELS]); td = np.concatenate([~E[l]["cat"]["TRK"]["U"][E[l]["lrg"]] for l in W.DATA_LABELS])
    f = np.isfinite(yx) & np.isfinite(eh)
    cuts = np.round(np.arange(-1.0, 2.01, 0.05), 3)
    agp = [float(((yx[f] < c) == (eh[f] < ECJC)).mean()) for c in cuts]
    agt = [float(((yx[f & td] < c) == tc[f & td]).mean()) for c in cuts]
    out["yX"] = {"corr_yX_eta_hi": float(np.corrcoef(yx[f], eh[f])[0, 1]), "median_eta_hi_minus_yX": float(np.median(eh[f] - yx[f])),
                 "best_cut_vs_PROXY": [float(cuts[int(np.argmax(agp))]), max(agp)], "best_cut_vs_TRK": [float(cuts[int(np.argmax(agt))]), max(agt)]}
    res["proxy"] = out
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    json.dump(res, open(a.out, "w"), default=lambda o: o.item() if hasattr(o, "item") else str(o))
    print(json.dumps({"out": a.out, "n_hists": len(res["hists"]), "f_TRK_ALL_both": res["fractions"]["TRK|ALL|both"]["f_contained"]}))


if __name__ == "__main__":
    main()

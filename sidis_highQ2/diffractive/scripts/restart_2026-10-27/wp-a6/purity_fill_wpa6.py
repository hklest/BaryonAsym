#!/usr/bin/env python3
"""WP-A6 S4b: WP-C proposal purity table recomputed with ND (7071) and Reggeon (6671/6677) tracks in the denominator,
scaled by the fitted event-level scales (Decision 9). Blinded: MC only, no data read.
usage: purity_fill_wpa6.py --scales <csv> --g2 <csv> --frame B --xipcut 0.03 --rule D7plus --out <csv> [--mask-date 2026-10-28]
Re-runnable; the mask goes next to --out (mask_wpa6_<rule>_<frame>_xip<NNN>_<date>.json) via make_blind_mask.py."""
import sys, os, re, csv, json, hashlib, argparse, subprocess
import numpy as np
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive"
sys.path.insert(0, D + "/scripts/restart_2026-10-27/wp-c/study")
sys.path.insert(0, D + "/scripts/restart_2026-10-27/wp-a")
from wpc_lib import cell_id, pur_stab, MX_EDGES
import wpa_common as W
TRK = D + "/data/restart_2026-10-27/wp-a6/trk/"
WPC_CSV = D + "/data/restart_2026-10-27/wp-c/study/binstats_proposal_D7plus_xip030_frameB_G2.csv"
WPC_EDGES = D + "/data/restart_2026-10-27/wp-c/study/binstats_proposal_D7plus_xip030_frameB_G2.json"
MASKER = D + "/scripts/restart_2026-10-27/wp-c/mask/make_blind_mask.py"
SIGNAL, REGGEON, ND = W.SIGNAL, ["6671", "6677"], ["7071"]
CHAIN = "7071"
CLASSES = ["MX4_10", "MX10_20", "MX20_40", "MXout"]
MXN = ["4-10", "10-20", "20-40"]
CAVEAT = ("track build selects E-p_z 35-70 and etaMaxClus08 < 3.2 (wpc_build.py:97,100-101); the event-level f_ND uses SB "
          "(etaMaxClus08 < 3.3); harmonisation is WP-B item W2")
HDR = ["charge", "mx_bin", "axis", "bin", "lo", "hi", "n_data", "n_sig_pred", "n_mc_raw", "f_nd", "f_reg", "f_fake", "purity",
       "stability", "masked", "p_mig", "n_mc_raw_minreco", "pass_D7plus", "f_mig", "f_nd_spread", "n_raw_sig", "n_raw_reg",
       "n_raw_nd", "masked_G2", "reasons"]
MX6 = np.array([4., 7, 10, 15, 20, 30, 40])


def sha(p): return hashlib.sha256(open(p, "rb").read()).hexdigest()


def mx_class(mx):
    c = np.full(len(mx), 3)
    c[(mx >= 4) & (mx < 10)] = 0; c[(mx >= 10) & (mx < 20)] = 1; c[(mx >= 20) & (mx <= 40)] = 2
    return c


def load_tracks(labels, xipcut, prefix):
    """Concatenate tracks of the labels passing the event x_IP cut (0 < e_xIp < xipcut via the track's event)."""
    T = {}
    for l in labels:
        d = np.load("%s%s%s.npz" % (TRK, prefix, l))
        eg = np.searchsorted(d["e_idx"], d["t_ev"]); x = d["e_xIp"][eg]
        k = (x > 0) & (x < xipcut)
        for key in ("t_w", "t_charge", "t_mx", "t_xfA", "t_pt2A", "t_xfB", "t_pt2B", "t_gv", "t_gmx", "t_xf2", "t_pt22", "t_cell"):
            T.setdefault(key, []).append(d[key][k])
        T.setdefault("t_label", []).append(np.full(int(k.sum()), l))
    return {k: np.concatenate(v) for k, v in T.items()}


def read_scales(path, tag="nominal"):
    S = {}
    for r in csv.DictReader(open(path)):
        if r["chain"] == CHAIN and r.get("tag", tag) == tag:
            S[(r["scope"], r["mx_class"])] = (float(r["s_diff"]), float(r["s_ND"]))
    for sc in ("D0", "D1", "D2", "ALL"):
        for c in CLASSES:
            if (sc, c) not in S: raise SystemExit("FATAL: scales missing (%s,%s)" % (sc, c))
    return S


def scale_vector(T, kind, S):
    """s_diff (kind sig/reg) or s_ND (kind nd) per track from (event cell, reco class); cell -1 -> ALL row."""
    cls = mx_class(T["t_mx"]); cell = T["t_cell"].astype(int)
    idx = 0 if kind != "nd" else 1
    s = np.empty(len(cls))
    for ci, c in enumerate(CLASSES):
        for ce, sc in ((0, "D0"), (1, "D1"), (2, "D2"), (-1, "ALL")):
            s[(cls == ci) & (cell == ce)] = S[(sc, c)][idx]
    return s


def fac_ratio(T, chain=CHAIN):
    r = np.ones(len(T["t_w"])); dev = 0.0
    for l in np.unique(T["t_label"]):
        f = W.fac_used(str(l), chain) / W.table_fac(str(l)); dev = max(dev, abs(f - 1))
        r[T["t_label"] == l] = f
    return r, dev


def offs(el):
    o = [0]
    for e in el: o.append(o[-1] + len(e) - 1)
    return o


def halves(e): return np.sort(np.concatenate([e, (e[:-1] + e[1:]) / 2]))


def table(Ts, wS, Tr, wR, Tn, wN, edges, frame):
    """Rows (list of dicts) for both charges and axes; edges = {"xF": [3 edge arrays], "pt2": [...]}.
    wS/wR/wN are the (already scaled) weights; Tr/Tn may be None (WP-C reduction)."""
    rows = []
    gv = Ts["t_gv"] & np.isfinite(Ts["t_xf2"]) & np.isfinite(Ts["t_pt22"])
    OBS = {"xF": ("t_xf" + frame, "t_xf2"), "pt2": ("t_pt2" + frame, "t_pt22")}
    for ax in ("xF", "pt2"):
        el = [np.array(e, float) for e in edges[ax]]; rc, gc = OBS[ax]; o = offs(el); nb = o[-1]
        el6 = []
        for i in range(3): el6 += [halves(el[i])] * 2
        o6 = offs(el6)
        for c in (1, -1):
            m = Ts["t_charge"] == c; g = m & gv
            r = pur_stab(Ts["t_mx"][m], Ts[rc][m], Ts["t_gmx"][m], Ts[gc][m], gv[m], wS[m], MX_EDGES, el)
            r2 = pur_stab(Ts["t_mx"][g], Ts[rc][g], Ts["t_gmx"][g], Ts[gc][g], gv[g], wS[g], MX_EDGES, el)
            cid, _, _, nc = cell_id(Ts["t_mx"][g], Ts[rc][g], MX6, el6)
            raw6 = np.bincount(cid[cid >= 0], minlength=nc)
            mn = np.zeros(nb)
            for i in range(3):
                for j in range(len(el[i]) - 1):
                    mn[o[i] + j] = min(raw6[o6[2 * i + hh] + 2 * j + k] for hh in (0, 1) for k in (0, 1))
            def comp(T, w):
                if T is None: return np.zeros(nb), np.zeros(nb)
                mm = T["t_charge"] == c
                cc = cell_id(T["t_mx"][mm], T[rc][mm], MX_EDGES, el)[0]; k = cc >= 0
                return (np.bincount(cc[k], weights=w[mm][k], minlength=nb), np.bincount(cc[k], minlength=nb).astype(float))
            wr, nr_ = comp(Tr, wR); wn, nn_ = comp(Tn, wN)
            nsig, nsame, nsig_gv = r["w"]["nreco"][:nb], r["w"]["nsame"][:nb], r2["w"]["nreco"][:nb]  # pur_stab nc over-counts the last M_X bin
            ntot = nsig + wr + wn
            with np.errstate(all="ignore"):
                f = lambda x: np.nan_to_num(x / ntot, nan=0.0)
                fr = dict(purity=f(nsame), f_fake=f(r["w"]["fake"][:nb] * nsig), f_mig=f(nsig_gv - nsame), f_nd=f(wn), f_reg=f(wr))
            idx = 0
            for i in range(3):
                for j in range(len(el[i]) - 1):
                    rows.append(dict(charge="h+" if c > 0 else "h-", mx_bin=MXN[i], axis=ax, bin=j, lo=float(el[i][j]), hi=float(el[i][j + 1]),
                        n_sig_pred=float(nsig_gv[idx]), n_mc_raw=int(r2["raw"]["nreco"][idx]), f_nd=float(fr["f_nd"][idx]),
                        f_reg=float(fr["f_reg"][idx]), f_fake=float(fr["f_fake"][idx]), purity=float(fr["purity"][idx]),
                        stability=float(np.nan_to_num(r["w"]["stab"][idx], nan=0.0)), p_mig=float(np.nan_to_num(r2["w"]["purity"][idx], nan=0.0)),
                        n_mc_raw_minreco=int(mn[idx]), f_mig=float(fr["f_mig"][idx]), n_raw_sig=int(r["raw"]["nreco"][idx]),
                        n_raw_reg=int(nr_[idx]), n_raw_nd=int(nn_[idx])))
                    idx += 1
    return rows


def d7plus_reasons(n, p, s, pm, ff, mr):
    return [k for k, b in (("N<50", n < 50), ("purity<0.35", p < 0.35), ("stability<0.35", s < 0.35), ("p_mig<0.40", pm < 0.40),
                           ("f_fake>0.35", ff > 0.35), ("raw_reco_subbin<100", mr < 100)) if b]


def mxkey(s):
    n = re.findall(r"\d+", str(s)); return "%s-%s" % (n[0], n[1]) if len(n) >= 2 else str(s)


def read_g2(path):
    R = list(csv.DictReader(open(path)))
    splits = {r["split"] for r in R}; split = "odd" if "odd" in splits else "even"
    sel = [r for r in R if r["split"] == split and r["chain"] == CHAIN]
    tb = lambda v: str(v).strip().lower() in ("1", "true", "yes")
    g2 = {mxkey(r["mx_bin"]): tb(r["masked_G2"]) for r in sel}
    rel = {mxkey(r["mx_bin"]): (float(r["spread"]) / float(r["f_nd"]) if float(r["f_nd"]) > 0 else 0.0) for r in sel}
    for m in MXN:
        if m not in g2: raise SystemExit("FATAL: f_nd_per_mx.csv has no %s row for chain %s split %s" % (m, CHAIN, split))
    return g2, rel, split


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--scales", required=True); ap.add_argument("--g2", required=True)
    ap.add_argument("--frame", default="B"); ap.add_argument("--xipcut", type=float, default=0.03)
    ap.add_argument("--rule", default="D7plus"); ap.add_argument("--out", required=True)
    ap.add_argument("--mask-date", default="2026-10-28")
    a = ap.parse_args()
    if a.rule != "D7plus": raise SystemExit("only D7plus implemented")
    edges = json.load(open(WPC_EDGES)); S = read_scales(a.scales); g2, rel, split = read_g2(a.g2)
    Ts, Tr, Tn = load_tracks(SIGNAL, a.xipcut, "s"), load_tracks(REGGEON, a.xipcut, "b"), load_tracks(ND, a.xipcut, "b")
    fs, fr_, fn = (fac_ratio(T) for T in (Ts, Tr, Tn))
    dev = max(fs[1], fr_[1], fn[1])
    wS = Ts["t_w"] * fs[0] * scale_vector(Ts, "sig", S); wR = Tr["t_w"] * fr_[0] * scale_vector(Tr, "reg", S)
    wN = Tn["t_w"] * fn[0] * scale_vector(Tn, "nd", S)
    rows = table(Ts, wS, Tr, wR, Tn, wN, edges, a.frame)
    c1 = {k: float((T["t_cell"] == -1).mean()) for k, T in (("signal", Ts), ("reggeon", Tr), ("nd", Tn))}
    c1w = {k: float(w[T["t_cell"] == -1].sum() / w.sum()) for k, T, w in (("signal", Ts, wS), ("reggeon", Tr, wR), ("nd", Tn, wN))}
    # f_nd(7071) is per M_X bin; relative spread applied to the per-bin f_nd
    wpc = {(r["charge"], r["mx_bin"], r["axis"], int(r["bin"])): r for r in csv.DictReader(open(WPC_CSV))}
    out, nG2, nD7, nWPC = [], 0, 0, 0
    for r in rows:
        why = d7plus_reasons(r["n_sig_pred"], r["purity"], r["stability"], r["p_mig"], r["f_fake"], r["n_mc_raw_minreco"])
        gm = g2[r["mx_bin"]]; r["masked_G2"] = int(gm)
        r["f_nd_spread"] = r["f_nd"] * rel[r["mx_bin"]]
        r["pass_D7plus"] = int(not why); r["masked"] = int(bool(why) or gm)
        r["reasons"] = ";".join(why + (["G2"] if gm else [])); r["n_data"] = ""
        out.append(r)
    nb = len(out)
    nG2 = sum(r["masked_G2"] for r in out); nD7 = sum(1 for r in out if not r["pass_D7plus"]); nU = sum(r["masked"] for r in out)
    nWPC = sum(1 - int(w["pass_D7plus"]) for w in wpc.values())
    os.makedirs(os.path.dirname(a.out), exist_ok=True)
    fmt = lambda k, v: ("%.8g" % v if isinstance(v, float) and k not in ("lo", "hi") else v)
    with open(a.out, "w", newline="") as f:
        cw = csv.writer(f); cw.writerow(HDR)
        for r in out: cw.writerow([fmt(k, r[k]) for k in HDR])
    med = lambda k: float(np.median([r[k] for r in out]))
    wp_med = float(np.median([float(w["purity"]) for w in wpc.values()]))
    summary = ("bins %d | masked by G2 %d, by D7plus (new) %d, by D7plus in WP-C table %d, union %d | median purity new %.4f vs WP-C %.4f | "
               "median f_nd %.4f, median f_reg %.4f" % (nb, nG2, nD7, nWPC, nU, med("purity"), wp_med, med("f_nd"), med("f_reg")))
    stem = os.path.dirname(a.out) + "/"
    tag = "%s_%s_xip%03d" % (a.rule, a.frame, round(a.xipcut * 1000))
    mask = "%smask_wpa6_%s_%s.json" % (stem, tag, a.mask_date)
    subprocess.run([sys.executable, MASKER, a.out, mask, a.rule], check=True, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1"))
    md = json.load(open(mask))
    overlay = dict(g2_source=a.g2, g2_source_sha256=sha(a.g2), split=split, g2_masked_mx_bins=[m for m in MXN if g2[m]],
                   d7plus_mask=mask, d7plus_mask_sha256=sha(mask), source_table=a.out, source_table_sha256=sha(a.out),
                   final_masked_bins=[dict(charge=b["charge"], mx_bin=b["mx_bin"], axis=b["axis"], bin=b["bin"],
                                           reasons=b["reasons"] + (["G2"] if g2[b["mx_bin"]] else []))
                                      for b in md["bins"] if b["masked"] or g2[b["mx_bin"]]], data_read=False)
    ov = "%smask_wpa6_g2_overlay_%s.json" % (stem, a.mask_date)
    blob = json.dumps(overlay, indent=1, sort_keys=True).encode(); open(ov, "wb").write(blob)
    open(ov + ".sha256", "w").write("%s  %s\n" % (hashlib.sha256(blob).hexdigest(), ov))
    side = dict(scales_file=a.scales, scales_sha256=sha(a.scales), g2_file=a.g2, g2_sha256=sha(a.g2), g2_split=split, rule=a.rule,
                frame=a.frame, xipcut=a.xipcut, edges=edges, caveat=CAVEAT, cell_minus1_track_fraction=c1,
                cell_minus1_weight_fraction=c1w, fac_ratio_max_dev=dev, mask=mask, overlay=ov, summary=summary)
    json.dump(side, open(a.out.replace(".csv", ".json"), "w"), indent=1)
    print("cell=-1 track fractions", json.dumps(c1), "fac_ratio max |dev|", dev)
    print("SUMMARY", summary)


if __name__ == "__main__":
    main()

"""WP-A6 plots. Parts: stacks (36 post-fit etaMaxClus04 stacks), fnd (f_nd vs M_X), spectra (SEL M_X and x_IP; Condor).
Usage: plots_wpa6.py --split even [--part stacks|fnd|spectra|all] [--scales CSV]. stacks/fnd read payload JSONs + CSVs only."""
import argparse, csv, json, sys
from pathlib import Path
import numpy as np
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "gate_evidence")); sys.path.insert(0, str(HERE.parent / "wp-a")); sys.path.insert(0, str(HERE.parent / "wp-a3"))
import ge_style as G
import matplotlib.pyplot as plt
import wpa_common as W
WD_ = D / "data/restart_2026-10-27/wp-a6"
CHAINS = ("7071", "7091", "nd4cat"); CELLS = ("D0", "D1", "D2"); CLS = ("MX4_10", "MX10_20", "MX20_40", "MXout")
FIT_EDGE, SEL_EDGE = 3.0, 3.3


def outdir(split): return D / "plots/restart_2026-10-27/wp-a6" / split


def add_caps(split, caps):
    """Merge {png name: caption} into captions.md ('## name' + text blocks), keeping entries of other parts."""
    p = outdir(split) / "captions.md"; cur = {}
    if p.exists():
        for blk in p.read_text().split("\n## ")[1:]:
            k, _, v = blk.partition("\n"); cur[k.strip()] = v.strip()
    cur.update(caps)
    p.write_text("# WP-A6 figure captions (%s split)\n\n" % split + "".join("## %s\n%s\n\n" % (k, cur[k]) for k in sorted(cur)))


def read_scales(split, path=None):
    out = {}
    for r in csv.DictReader(open(path or WD_ / ("scales_resolved_%s_nominal.csv" % split))):
        out[(r["chain"], r["scope"], r["mx_class"])] = r
    return out


def stack_fig(x, sc, chain, cell, cls, split, path):
    m = x["modes"]["cluster"]; fac = x["scale_table_basis"]["exposure_factors"]; nd = W.CHAINS[chain]["nd"]
    ed = np.array(x["edges"]); g = lambda l, k: np.asarray(m["components"][l]["templates"][k][cell][cls], float)
    sd, sn = float(sc["s_diff"]), float(sc["s_ND"])
    dl = lambda l: g(l, "exposure1") * fac[l]; v2 = lambda l: g(l, "sumw2") * fac[l] ** 2
    regg = sd * (dl("6671") + dl("6677")); sig = sd * sum(dl(l) for l in W.SIGNAL); ndv = sn * sum(dl(l) for l in nd)
    var = sd ** 2 * sum(v2(l) for l in W.DIFF8) + sn ** 2 * sum(v2(l) for l in nd)
    data = np.asarray(m["data"]["templates"]["raw"][cell][cls], float); tot = regg + sig + ndv
    fig, ax = G.grid(1, w=6.5, h=4.4); a, r = ax[0][0]
    G.panel(a, r, ed, data, [("ND (s_ND x delivered)", ndv, G.C["nd"]), ("Reggeon 6671+6677 (inside DIFF8)", regg, G.C["reggeon"]),
                             ("DIFF8 signal", sig, G.C["signal"])], var, logy=True, top=60, xlabel="etaMaxClus04", rlim=(0, 2.5))
    for aa in (a, r):
        aa.axvspan(FIT_EDGE, ed[-1], color="0.5", alpha=0.12, lw=0)
    G.vline(a, r, FIT_EDGE, "fit edge 3.0"); G.vline(a, r, SEL_EDGE, "3.3", ls=":")
    f = json.load(open(D / sc["fit_file"])) if sc.get("fit_file") else {}
    G.note(a, "%s %s %s\ns_diff=%.3f  s_ND=%.3f\nsource: %s  dev/ndf=%.0f/%s\nin-fit n_data=%d" % (
        chain, cell, cls, sd, sn, sc["source"], f.get("deviance", np.nan), f.get("ndf", "?"), data[ed[:-1] < FIT_EDGE].sum()), "ul")
    a.legend(loc="upper right", fontsize=6.5); G.save(fig, path)
    cap = ("%s chain, %s split, cell %s, class %s: etaMaxClus04 (cluster mode, FIT sample, full grid [-1,5)) data vs DIFF8 (signal + Reggeon layer) and ND stacked with the resolved nominal "
           "scales s_diff=%.3f, s_ND=%.3f (%s). Hatched band and grey ratio band are MC-stat sqrt(s_d^2 W_D + s_n^2 W_N); the fit covers eta < 3.0 (dashed), "
           "3.0-3.3 and beyond (shaded) is out-of-fit extrapolation." % (chain, split, cell, cls, sd, sn, sc["source"]))
    return cap


def do_stacks(split, scales):
    sc = read_scales(split, scales); od = outdir(split) / "stacks"; od.mkdir(parents=True, exist_ok=True); caps = {}
    for ch in CHAINS:
        x = json.load(open(WD_ / ("payloads/%s_%s.json" % (split, ch))))
        for c in CELLS:
            for k in CLS:
                n = "%s_%s_%s.png" % (ch, c, k); caps["stacks/" + n] = stack_fig(x, sc[(ch, c, k)], ch, c, k, split, od / n)
    add_caps(split, caps)


def do_fnd(split):
    rows = [r for r in csv.DictReader(open(WD_ / "f_nd_per_mx.csv")) if r["split"] == split]
    fl = lambda r, k: float(r[k]) if r.get(k) not in (None, "", "nan") else np.nan
    bins = sorted(dict.fromkeys(r["mx_bin"] for r in rows), key=lambda b: (list(CLS) + [b]).index(b))
    fig, (a, b) = plt.subplots(2, 1, figsize=(7.5, 6.5), sharex=True, gridspec_kw=dict(height_ratios=[2, 1], hspace=0.05))
    for i, ch in enumerate(CHAINS):
        col = G.C[ch]; off = (i - 1) * 0.22
        for r in rows:
            if r["chain"] != ch: continue
            xx = bins.index(r["mx_bin"]) + off; f = fl(r, "f_nd"); sg = fl(r, "sig_g2")
            a.vlines(xx, f - sg, f + sg, color=col, alpha=0.3, lw=7)
            a.errorbar(xx, f, yerr=fl(r, "sig_fit"), fmt="o", color=col, ms=5, lw=1.2, label=ch if r["mx_bin"] == bins[0] else None)
            a.plot(xx, f + fl(r, "delta_edge"), "x", color=col, ms=6)
            b.plot(xx, fl(r, "f_nd_infit"), "o", color=col, mfc="white", ms=5); b.plot(xx, fl(r, "f_nd_band"), "o", color=col, ms=5)
    a.axhline(0.30, color="0.3", ls="--", lw=1); a.text(len(bins) - 0.6, 0.31, "ceiling 0.30", ha="right", fontsize=7)
    a.plot([], [], "x", color="0.3", label="f_nd + delta_edge"); a.plot([], [], lw=7, alpha=0.3, color="0.3", label="outer band sig_g2")
    a.set_ylabel("f_nd"); a.legend(fontsize=7, ncol=3); b.set_ylabel("f_nd part"); b.plot([], [], "o", mfc="white", color="0.3", label="in-fit")
    b.plot([], [], "o", color="0.3", label="band (3.0-5)"); b.legend(fontsize=7, ncol=2)
    b.set_xticks(range(len(bins))); b.set_xticklabels(bins); b.set_xlabel("M_X bin"); G.save(fig, outdir(split) / "fnd_vs_mx.png")
    add_caps(split, {"fnd_vs_mx.png": "%s split, SEL sample: ND fraction f_nd per M_X bin for the three ND chains (offset). Thin bars are the fit uncertainty sig_fit, wide pale bars the outer "
                     "G2 band sig_g2, crosses f_nd + delta_edge, dashed line the 0.30 ceiling; lower panel splits f_nd into the in-fit (hollow) and 3.0-5 band (filled) parts." % split})


def do_spectra(split):
    import fact_wpa3 as F
    sc = read_scales(split); ch = "7071"; nd = W.CHAINS[ch]["nd"]
    mxe = np.concatenate([np.linspace(3, 10, 8)[:-1], np.linspace(10, 50, 17)]); xe = np.linspace(0, 0.04, 21)
    H = {v: {k: np.zeros(len(e) - 1) for k in ("data", "nd", "regg", "sig", "var")} for v, e in (("mx", mxe), ("xip", xe))}
    def sel(l):
        e = F.load(l); s = F.masks(e, split == "even")["SB"] & (e["xip"] < 0.04) & (e["mx"] > 3); return e, s
    cl = lambda mx: np.where(mx < 4, 3, np.where(mx < 10, 0, np.where(mx < 20, 1, np.where(mx <= 40, 2, 3))))
    for l in W.DATA_LABELS:
        e, s = sel(l)
        for v, ed in (("mx", mxe), ("xip", xe)): H[v]["data"] += np.histogram(e[v][s & (e["cell"] >= 0)], ed)[0]
    for l in W.DIFF8 + nd:
        e, s = sel(l); dw = W.fac_used(l, ch) / W.table_fac(l) * e["w"]; isnd = l in nd
        for ic, c in enumerate(CELLS):
            for ik, k in enumerate(CLS):
                m = s & (e["cell"] == ic) & (cl(e["mx"]) == ik); r = sc[(ch, c, k)]; sf = float(r["s_ND"] if isnd else r["s_diff"])
                for v, ed in (("mx", mxe), ("xip", xe)):
                    lay = "nd" if isnd else ("regg" if l in ("6671", "6677") else "sig")
                    H[v][lay] += np.histogram(e[v][m], ed, weights=sf * dw[m])[0]; H[v]["var"] += np.histogram(e[v][m], ed, weights=(sf * dw[m]) ** 2)[0]
    caps = {}
    for v, ed, xl, logx in (("mx", mxe, "M_X [GeV]", True), ("xip", xe, "x_IP", False)):
        h = H[v]; fig, ax = G.grid(1, w=6.5, h=4.4); a, r = ax[0][0]
        G.panel(a, r, ed, h["data"], [("ND", h["nd"], G.C["nd"]), ("Reggeon 6671+6677", h["regg"], G.C["reggeon"]), ("DIFF8 signal", h["sig"], G.C["signal"])],
                h["var"], logy=True, top=60, xlabel=xl, rlim=(0, 2.5))
        if logx: a.set_xscale("log")
        a.legend(loc="upper right", fontsize=7); G.save(fig, outdir(split) / ("sel_%s.png" % v))
        caps["sel_%s.png" % v] = ("%s split, SEL = SB & x_IP<0.04 & M_X>3, D cells only: data vs the 7071-chain cocktail (7071 ND, Reggeon 6671+6677, DIFF8 signal) scaled with the nominal resolved "
                                  "scales per (cell, M_X class). Hatched and grey bands are MC-stat; delivered weight = fac_used/table_fac x skim w." % split)
    add_caps(split, caps)


if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("--split", required=True, choices=("even", "odd"))
    ap.add_argument("--part", default="all", choices=("stacks", "fnd", "spectra", "all")); ap.add_argument("--scales")
    a = ap.parse_args(); outdir(a.split).mkdir(parents=True, exist_ok=True)
    if a.part in ("stacks", "all"): do_stacks(a.split, a.scales)
    if a.part in ("fnd", "all"): do_fnd(a.split)
    if a.part in ("spectra", "all"): do_spectra(a.split)

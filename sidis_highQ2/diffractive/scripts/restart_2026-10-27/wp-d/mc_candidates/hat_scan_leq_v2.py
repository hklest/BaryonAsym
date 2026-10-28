#!/usr/bin/env python3
"""WP-D MC-candidate HAT scan, round 2 (h1-mc-expert, 2026-10-28): 6670 / 8221 / 7448 vs 9012.
Per HAT file: N, sum(Weight1*Weight2), weight histogram, Pt2HatGki class split, and counts / weight
sums in R = {Q2Gki>150, 0.1<YGki<0.9, XpomGki<0.03} (task definition) and R8 (same with y<0.8, the
round-1 definition), with M_X bins [4,10),[10,20),[20,40] and the finer round-1 bins, |t| and x_IP
histograms, y quantiles, and the weight mean per class (hard = Pt2HatGki>9 i.e. pt_hat>3 GeV; soft =
QPM sentinel).  Usage: hat_scan_leq_v2.py <label> <hatdir> <out.jsonl> [fileglob]
"""
import sys, os, json, glob
import numpy as np, uproot

label, hatdir, out = sys.argv[1], sys.argv[2], sys.argv[3]
pat = sys.argv[4] if len(sys.argv) > 4 else "hat.*.root"
files = sorted(glob.glob(os.path.join(hatdir, pat)))
MXB = [0, 4, 10, 20, 40, 1e9]; MXB1 = [0, 4, 8, 12, 18, 25, 40, 1e9]
WB = [0, 0.999, 1.001, 1.5, 2, 3, 4, 5, 5.999, 6.001, 10, 1e9]
Q2BINS = [(150, 200), (200, 300), (300, 700), (700, 1e9)]
def wh(w, m, bins): return [float(x) for x in np.histogram(w[m], bins=bins)[0]]
def region(rec, nm, m, b, w, hard, phys):
    y = b["YGki"][m]; xp = b["XpomGki"][m]; mx = b["MXGen"][m]
    rec[nm] = {"n": int(m.sum()), "sum_w": float(w[m].sum()), "sum_w2": float((w[m]**2).sum()),
               "n_hard": int((m & hard).sum()), "sum_w_hard": float(w[m & hard].sum()),
               "n_soft": int((m & ~hard).sum()), "sum_w_soft": float(w[m & ~hard].sum()),
               "n_pt2phys": int((m & phys).sum()),
               "mx_hist_all": [int(x) for x in np.histogram(mx, bins=MXB)[0]],
               "mx_whist_all": [float(x) for x in np.histogram(mx, bins=MXB, weights=w[m])[0]],
               "mx_hist1_all": [int(x) for x in np.histogram(mx, bins=MXB1)[0]],
               "mx_hist_hard": [int(x) for x in np.histogram(b["MXGen"][m & hard], bins=MXB)[0]],
               "mx_median": float(np.median(mx)) if m.any() else None,
               "mx_wmedian": float(np.interp(0.5, np.cumsum(w[m][np.argsort(mx)]) / w[m].sum(), np.sort(mx))) if m.any() else None,
               "y_q": [float(x) for x in np.quantile(y, [0, 0.05, 0.5, 0.95, 1])] if m.any() else None,
               "xpom_q": [float(x) for x in np.quantile(xp, [0, 0.05, 0.5, 0.95, 1])] if m.any() else None,
               "t_hist_all": [int(x) for x in np.histogram(np.abs(b["TGki"][m]), bins=[0, 0.1, 0.2, 0.4, 0.7, 1.0, 5.0])[0]],
               "xpom_hist_all": [int(x) for x in np.histogram(xp, bins=[0, 0.003, 0.01, 0.02, 0.03])[0]],
               "w_hist": wh(w, m, WB), "w_mean_hard": float(w[m & hard].mean()) if (m & hard).any() else None,
               "w_mean_soft": float(w[m & ~hard].mean()) if (m & ~hard).any() else None,
               "q2bins_all": [{"bin": list(qb), "n": int((m & (b["Q2Gki"] >= qb[0]) & (b["Q2Gki"] < qb[1])).sum()),
                               "sum_w": float(w[m & (b["Q2Gki"] >= qb[0]) & (b["Q2Gki"] < qb[1])].sum())} for qb in Q2BINS],
               "genrad_hist": {str(int(g)): int((b["GenRad"][m] == g).sum()) for g in np.unique(b["GenRad"][m])},
               "my_frac_lt1p6": float((b["MYGen"][m] < 1.6).mean()) if m.any() else None}
with open(out, "w") as fo:
    for f in files:
        t = uproot.open(f)["HAT"]
        b = {k: t[k].array(library="np").astype(np.float64) for k in
             ["Weight1", "Weight2", "Q2Gki", "YGki", "XpomGki", "TGki", "MXGen", "MYGen", "Pt2HatGki", "GenRad"]}
        w = b["Weight1"] * b["Weight2"]
        hard = b["Pt2HatGki"] > 9.0; phys = b["Pt2HatGki"] > -1000.0
        allm = np.ones(len(w), bool)
        R = (b["Q2Gki"] > 150) & (b["YGki"] > 0.1) & (b["YGki"] < 0.9) & (b["XpomGki"] < 0.03)
        R8 = R & (b["YGki"] < 0.8)
        Rfid = R & (b["MXGen"] > 4) & (b["MXGen"] < 40) & (np.abs(b["TGki"]) < 1) & (b["MYGen"] < 1.6)
        rec = {"label": label, "file": os.path.basename(f), "n": int(len(w)), "sum_w": float(w.sum()),
               "w2_ne1": int((b["Weight2"] != 1).sum()), "w1_min": float(b["Weight1"].min()), "w1_max": float(b["Weight1"].max()),
               "n_pt2phys": int(phys.sum()), "n_hard": int(hard.sum()), "sum_w_hard": float(w[hard].sum()),
               "pt2hat_min_phys": float(b["Pt2HatGki"][phys].min()) if phys.any() else None,
               "pt2hat_q_phys": [float(x) for x in np.quantile(b["Pt2HatGki"][phys], [0.5, 0.9, 0.99])] if phys.any() else None,
               "n_q2gt150": int((b["Q2Gki"] > 150).sum()), "sum_w_q2gt150": float(w[b["Q2Gki"] > 150].sum()),
               "y_q_all": [float(x) for x in np.quantile(b["YGki"], [0, 0.001, 0.5, 0.999, 1])],
               "xpom_max_all": float(b["XpomGki"].max()), "q2_min_all": float(b["Q2Gki"].min()), "q2_max_all": float(b["Q2Gki"].max()),
               "w_mean_hard_all": float(w[hard].mean()) if hard.any() else None, "w_mean_soft_all": float(w[~hard].mean()) if (~hard).any() else None,
               "w_hist_all": wh(w, allm, WB),
               "genrad_hist": {str(int(g)): int((b["GenRad"] == g).sum()) for g in np.unique(b["GenRad"])}}
        # weight vs Q2 (soft class) to identify the MCWE weighting variable
        rec["w_mean_soft_vs_q2"] = [{"bin": [lo, hi], "n": int(((~hard) & (b["Q2Gki"] >= lo) & (b["Q2Gki"] < hi)).sum()),
                                     "w_mean": float(w[(~hard) & (b["Q2Gki"] >= lo) & (b["Q2Gki"] < hi)].mean()) if ((~hard) & (b["Q2Gki"] >= lo) & (b["Q2Gki"] < hi)).any() else None}
                                    for lo, hi in [(0, 5), (5, 10), (10, 20), (20, 50), (50, 150), (150, 400), (400, 1e9)]]
        rec["w_mean_soft_vs_y"] = [{"bin": [lo, hi], "w_mean": float(w[(~hard) & (b["YGki"] >= lo) & (b["YGki"] < hi)].mean()) if ((~hard) & (b["YGki"] >= lo) & (b["YGki"] < hi)).any() else None}
                                   for lo, hi in [(0, 0.01), (0.01, 0.05), (0.05, 0.1), (0.1, 0.3), (0.3, 0.6), (0.6, 0.9), (0.9, 1)]]
        rec["w_mean_soft_vs_mx"] = [{"bin": [lo, hi], "w_mean": float(w[(~hard) & (b["MXGen"] >= lo) & (b["MXGen"] < hi)].mean()) if ((~hard) & (b["MXGen"] >= lo) & (b["MXGen"] < hi)).any() else None}
                                    for lo, hi in [(0, 2), (2, 4), (4, 10), (10, 20), (20, 40), (40, 1e9)]]
        for nm, m in (("R", R), ("R8", R8), ("Rfid", Rfid)):
            region(rec, nm, m, b, w, hard, phys)
        fo.write(json.dumps(rec) + "\n"); fo.flush()
        print(label, rec["file"], rec["n"], "R:", rec["R"]["n"], rec["R"]["n_hard"], flush=True)
print("done", label, len(files))

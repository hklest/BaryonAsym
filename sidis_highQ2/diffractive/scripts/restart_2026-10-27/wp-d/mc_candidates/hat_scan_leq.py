#!/usr/bin/env python3
"""WP-D MC-candidate HAT scan (h1-mc-expert, 2026-10-28).
Per HAT file of one ReqID: N, sum(Weight1*Weight2), and counts / weight sums in the
overlap region R = {Q2Gki>150, 0.1<YGki<0.8, XpomGki<0.03} (plus the tighter
R_fid = R & 4<MXGen<40 & |TGki|<1 & MYGen<1.6), split into the 2->2 hard class
(Pt2HatGki > 9 GeV^2, i.e. pt_hat > 3 GeV) and the rest (QPM sentinel / soft).
Used for the data-driven L_eq(8529) = sum_w_hard(8529,R) / [N_hard(9012,R)/L(9012)].
Usage: hat_scan_leq.py <label> <hatdir> <out.jsonl>
"""
import sys, os, json, glob
import numpy as np, uproot

label, hatdir, out = sys.argv[1], sys.argv[2], sys.argv[3]
files = sorted(glob.glob(os.path.join(hatdir, "hat.*.root")))
Q2BINS = [(150, 200), (200, 300), (300, 700), (700, 1e9)]
with open(out, "w") as fo:
    for f in files:
        t = uproot.open(f)["HAT"]
        b = {k: t[k].array(library="np").astype(np.float64) for k in
             ["Weight1", "Weight2", "Q2Gki", "YGki", "XpomGki", "TGki", "MXGen", "MYGen", "Pt2HatGki", "GenRad"]}
        w = b["Weight1"] * b["Weight2"]
        hard = b["Pt2HatGki"] > 9.0
        phys = b["Pt2HatGki"] > -1000.0
        R = (b["Q2Gki"] > 150) & (b["YGki"] > 0.1) & (b["YGki"] < 0.8) & (b["XpomGki"] < 0.03)
        Rfid = R & (b["MXGen"] > 4) & (b["MXGen"] < 40) & (np.abs(b["TGki"]) < 1) & (b["MYGen"] < 1.6)
        rec = {"label": label, "file": os.path.basename(f), "n": int(len(w)), "sum_w": float(w.sum()),
               "n_pt2phys": int(phys.sum()), "n_hard": int(hard.sum()), "sum_w_hard": float(w[hard].sum()),
               "pt2hat_min_phys": float(b["Pt2HatGki"][phys].min()) if phys.any() else None,
               "genrad_hist": {str(int(g)): int((b["GenRad"] == g).sum()) for g in np.unique(b["GenRad"])}}
        for nm, m in (("R", R), ("Rfid", Rfid)):
            rec[nm] = {"n": int(m.sum()), "sum_w": float(w[m].sum()),
                       "n_hard": int((m & hard).sum()), "sum_w_hard": float(w[m & hard].sum()),
                       "n_soft": int((m & ~hard).sum()), "sum_w_soft": float(w[m & ~hard].sum()),
                       "n_pt2phys": int((m & phys).sum()),
                       "q2bins_hard": [{"bin": list(qb), "n": int((m & hard & (b["Q2Gki"] >= qb[0]) & (b["Q2Gki"] < qb[1])).sum()),
                                        "sum_w": float(w[m & hard & (b["Q2Gki"] >= qb[0]) & (b["Q2Gki"] < qb[1])].sum())} for qb in Q2BINS],
                       "q2bins_all": [{"bin": list(qb), "n": int((m & (b["Q2Gki"] >= qb[0]) & (b["Q2Gki"] < qb[1])).sum()),
                                       "sum_w": float(w[m & (b["Q2Gki"] >= qb[0]) & (b["Q2Gki"] < qb[1])].sum())} for qb in Q2BINS],
                       "mx_hist_hard": [int(x) for x in np.histogram(b["MXGen"][m & hard], bins=[0, 4, 8, 12, 18, 25, 40, 1e9])[0]],
                       "mx_hist_all": [int(x) for x in np.histogram(b["MXGen"][m], bins=[0, 4, 8, 12, 18, 25, 40, 1e9])[0]],
                       "t_hist_all": [int(x) for x in np.histogram(np.abs(b["TGki"][m]), bins=[0, 0.1, 0.2, 0.4, 0.7, 1.0, 5.0])[0]],
                       "xpom_hist_all": [int(x) for x in np.histogram(b["XpomGki"][m], bins=[0, 0.003, 0.01, 0.02, 0.03])[0]]}
        fo.write(json.dumps(rec) + "\n"); fo.flush()
        print(label, rec["file"], rec["n"], "R:", rec["R"]["n"], rec["R"]["n_hard"], flush=True)
print("done", label, len(files))

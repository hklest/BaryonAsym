#!/usr/bin/env python3
"""WP-A S4 (A8): remedy-2 threshold choice from the CONTROL region only.
Primary (A8 wording, "Plug fail rates"): control Plug N-1 fail fraction
f_plug(T) = 1 - N(ctrl & FMD & FTS & ePlug<T)/N(ctrl & FMD & FTS) -- the same
definition G1(a) applies to the Stage-B N-1 sample.  Diagnostic: stack fail
rate 1 - N(ctrl & FMD & FTS & ePlug<T)/N(ctrl).  Both from
the S1 control partials' ePlug N-1 histograms (0.5 GeV bins; T in {3,5,7,10,15}
are bin edges, so the count is exact; T=7 is cross-checked against the stored
stack-pass count).  MC = chain cocktail projected per period exactly as in
noise_table_wpa.py.  Nominal T = smallest T at which data and MC agree within
2 sigma in every period.  No test-split input is read."""
import argparse, json, math
from pathlib import Path
import numpy as np
import wpa_common as W
TH = (3.0, 5.0, 7.0, 10.0, 15.0)


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--chains", default="7071,7091,7094")
    ap.add_argument("--basis", choices=("plugn1", "stack"), default="plugn1")
    ap.add_argument("--out", default=None); a = ap.parse_args()
    a.out = a.out or str(W.OUT / ("s4_thrscan/threshold_choice_%s.json" % a.basis))
    cdir = W.OUT / "s1_control/partials/control"; L = W.period_lumi(); res = {"basis": None, "thresholds": TH, "chains": {}, "inputs": {}}
    parts = {}
    for l in W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"] + W.DATA_LABELS:
        parts[l] = W.load_partial(cdir, l); res["inputs"][l] = W.sha256(cdir / l / "partial.json")
    edges = np.asarray(parts["ep0304"]["wpa"]["control_stats"]["ALL"]["n1_hists"]["ePlug"]["edges"])

    def passcnt(cs, T, w):
        h = np.asarray(cs["n1_hists"]["ePlug"][w], float); return float(h[edges[1:] <= T + 1e-9].sum())
    for l, p in parts.items():   # T=7 consistency
        cs = p["wpa"]["control_stats"]["ALL"]
        if abs(passcnt(cs, 7.0, "raw") - cs["ctrl_stack_pass"]["raw"]) > 0:
            raise SystemExit("FATAL: T=7 histogram pass count != stored stack pass for " + l)
    res["basis"] = a.basis
    for ch in a.chains.split(","):
        comps = W.DIFF8 + W.CHAINS[ch]["nd"]; rep = {l: W.represented(l, ch) for l in comps}; out = {}
        for T in TH:
            rows = {}
            for k in W.PERIODS:
                cs = parts[k]["wpa"]["control_stats"][k]; P = passcnt(cs, T, "raw")
                N = cs["ctrl"]["raw"] if a.basis == "stack" else float(np.sum(cs["n1_hists"]["ePlug"]["raw"]) + cs["n1_hists"]["ePlug"]["overflow_raw"])
                fd = 1 - P / N; ed = math.sqrt(fd * (1 - fd) / N)
                Nw = Pw = VN = VP = 0.0
                for l in comps:
                    if k not in rep[l]:
                        continue
                    c = parts[l]["wpa"]["control_stats"]["ALL"]; r = (L[k] / W.L_DATA) / W.table_fac(l)
                    if a.basis == "stack":
                        Nw += r * c["ctrl"]["signed"]; VN += r * r * c["ctrl"]["sumw2"]
                    else:
                        h = c["n1_hists"]["ePlug"]; Nw += r * (sum(h["signed"]) + h["overflow_signed"]); VN += r * r * (sum(h["sumw2"]) + h["overflow_sumw2"])
                    Pw += r * passcnt(c, T, "signed"); VP += r * r * passcnt(c, T, "sumw2")
                F = Nw - Pw; VF = max(VN - VP, 0.0); fm = F / Nw; em = math.sqrt(Pw * Pw * VF + F * F * VP) / Nw ** 2
                z = (fd - fm) / math.hypot(ed, em)
                rows[k] = {"f_data": fd, "f_data_err": ed, "f_mc": fm, "f_mc_err": em, "z": z, "agree_2sigma": abs(z) <= 2}
            out[str(T)] = {"periods": rows, "all_agree": all(r["agree_2sigma"] for r in rows.values())}
        chosen = next((T for T in TH if out[str(T)]["all_agree"]), None)
        res["chains"][ch] = {"scan": out, "chosen_threshold": chosen}
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(res, open(a.out, "w"), indent=1)
    for ch, c in res["chains"].items():
        print(ch, "chosen", c["chosen_threshold"], {T: {k: round(v["z"], 1) for k, v in c["scan"][T]["periods"].items()} for T in c["scan"]})
        print("   f_data/f_mc ep0607:", {T: (round(c["scan"][T]["periods"]["ep0607_true"]["f_data"], 3), round(c["scan"][T]["periods"]["ep0607_true"]["f_mc"], 3)) for T in c["scan"]})


if __name__ == "__main__":
    main()

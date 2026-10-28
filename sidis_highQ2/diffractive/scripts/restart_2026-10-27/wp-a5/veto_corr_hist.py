#!/usr/bin/env python3
"""WP-A5 (restart 2026-10-28) histogram job: fork of gate_evidence/veto_eff_hist.py over the 17 WP-A3 skims.

Same samples, masks (fact_wpa3.masks: N1_PLUG / N1_FMD / N1_FTS; COMB = no veto, fail = any), fail definitions and
MC weight basis (signed v3 x fac_used/table_fac, per chain 7071 / 7091 / nd4cat) as veto_eff_hist.py.  Changes:
  eta08 axis  [-1,1),[1,2),[2,2.5),[2.5,3.0),[3.0,3.2),[3.2,3.3), other (< -1 or sentinel)   (7 bins)
  M_X axis    fixed WP-C start grid: <4, [4,10), [10,20), [20,40], >40, non-finite           (6 bins; 40 itself in [20,40])
  arrays (cell4, eta7, mx6, fail2) under the same "<view>|<parity>|<chain|data>|<group>" keys.
Extra keys (diagnostics only, never used for q):  "<view>|<parity>|data|<period>" data per period, and
"...|xip04" copies restricted to 0 < x_IP < 0.04 (parent WP-A5 step (1) window, for the x_IP-cut comparison).
Invariant: summing eta and M_X reproduces veto_eff_hists.json per (cell, fail) exactly (checked here)."""
import argparse, json, sys
from pathlib import Path
import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a3")); sys.path.insert(0, str(HERE.parent / "wp-a"))
import fact_wpa3 as F
import wpa_common as W

CHAINS = ("7071", "7091", "nd4cat")
ETAE = [-1.0, 1.0, 2.0, 2.5, 3.0, 3.2, 3.3]; NE = len(ETAE)          # 6 bins + "other"
MXE = [4.0, 10.0, 20.0, 40.0]; NM = 6                                 # <4, 3 grid bins, >40, non-finite
VETOS = ("N1_PLUG", "N1_FMD", "N1_FTS", "COMB")
GROUPS = {"signal": W.SIGNAL, "reggeon": ["6671", "6677"]}
REF = W.D / "data/restart_2026-10-27/gate_evidence/veto_eff_hists.json"


def fail_of(e, v):
    if v == "COMB": return ~(e["eplug"] < 7.0) | ~e["fmd"] | ~e["fts"]
    return {"N1_PLUG": ~(e["eplug"] < 7.0), "N1_FMD": ~e["fmd"], "N1_FTS": ~e["fts"]}[v]


def ebin(e):
    x = e["ec08"]; i = np.searchsorted(ETAE, x, side="right") - 1
    return np.where(np.isfinite(x) & (x >= ETAE[0]) & (x < ETAE[-1]), i, NE - 1).astype(int)


def mbin(e):
    x = e["mx"]; i = np.searchsorted(MXE, x, side="right")            # 0:<4 1:[4,10) 2:[10,20) 3:[20,40) 4:>=40
    i = np.where(x == MXE[-1], 3, i)                                   # wpc_lib: last grid bin closed at 40
    return np.where(np.isfinite(x), i, NM - 1).astype(int)


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True); a = ap.parse_args()
    labs = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    E = {l: F.load(l) for l in W.DATA_LABELS + labs}
    MK = {(l, p): F.masks(e, p == "even") for l, e in E.items() for p in ("even", "odd")}
    for (l, p), mk in MK.items():
        e = E[l]; mk["COMB"] = (e["even"] if p == "even" else ~e["even"]) & ((e["ec08"] < 3.3) | (e["ec08"] < -900))
    XW = {l: np.isfinite(e["xip"]) & (e["xip"] > 0) & (e["xip"] < 0.04) for l, e in E.items()}
    IDX = {l: (np.where(e["cell"] >= 0, e["cell"], 3).astype(int), ebin(e), mbin(e)) for l, e in E.items()}
    shape = (4, NE, NM, 2)

    def acc(l, v, p, w, extra=None):
        c, ib, im = IDX[l]; m = MK[(l, p)][v]; f = fail_of(E[l], v).astype(int)
        if extra is not None: m = m & extra
        s = np.zeros(shape); s2 = np.zeros(shape)
        np.add.at(s, (c[m], ib[m], im[m], f[m]), w[m]); np.add.at(s2, (c[m], ib[m], im[m], f[m]), w[m] ** 2)
        return s, s2

    res = {"script_sha256": W.sha256(__file__), "fact_sha256": W.sha256(F.__file__), "wpa_common_sha256": W.sha256(W.__file__),
           "inputs": {l: W.sha256(F.SK / (l + ".npz")) for l in E}, "eta_edges": ETAE, "mx_edges": MXE,
           "axes": ["cell(D0,D1,D2,outside)", "eta(6 bins,other)", "mx(<4,[4,10),[10,20),[20,40],>40,nonfinite)", "fail(0 pass,1 fail)"],
           "arrays": {}, "checks": {}}
    for v in VETOS:
        for p in ("odd", "even"):
            for xs, ex in (("", None), ("|xip04", True)):
                per = {l: acc(l, v, p, np.ones(E[l]["w"].size), XW[l] if ex else None)[0] for l in W.DATA_LABELS}
                res["arrays"]["|".join((v, p, "data", "data")) + xs] = {"sumw": sum(per.values()).tolist()}
                if not ex:
                    for l in W.DATA_LABELS: res["arrays"]["|".join((v, p, "data", l))] = {"sumw": per[l].tolist()}
    for ch in CHAINS:
        nd = W.CHAINS[ch]["nd"]; base = {l: W.fac_used(l, ch) / W.table_fac(l) * E[l]["w"] for l in W.DIFF8 + nd}
        for v in VETOS:
            for p in ("odd", "even"):
                for g, ls in dict(GROUPS, nd=nd).items():
                    for xs, ex in (("", False), ("|xip04", True)):
                        r = [acc(l, v, p, base[l], XW[l] if ex else None) for l in ls]
                        res["arrays"]["|".join((v, p, ch, g)) + xs] = {"sumw": sum(x[0] for x in r).tolist(), "sumw2": sum(x[1] for x in r).tolist()}
    # invariant vs the gate-evidence hists: integrate eta and M_X -> (cell, fail) equal
    R = json.load(open(REF)); dev = 0.0; nk = 0
    for k, o in R["arrays"].items():
        for kk in o:
            a1 = np.asarray(res["arrays"][k][kk]).sum(axis=(1, 2)); a0 = np.asarray(o[kk]).sum(axis=(1, 2))
            dev = max(dev, float(np.max(np.abs(a1 - a0) / np.maximum(np.abs(a0), 1e-300)))); nk += 1
    res["checks"]["integrated_vs_veto_eff_hists"] = {"ref_sha256": W.sha256(REF), "n_arrays": nk, "max_rel_dev": dev, "pass_1e-9": dev < 1e-9}
    print(json.dumps(res["checks"]), flush=True)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(res, open(a.out, "w"))
    print(json.dumps({"out": a.out, "n_arrays": len(res["arrays"])}))
    if dev >= 1e-9: sys.exit(2)


if __name__ == "__main__":
    main()

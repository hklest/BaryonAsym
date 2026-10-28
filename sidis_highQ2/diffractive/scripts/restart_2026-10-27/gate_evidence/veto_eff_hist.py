#!/usr/bin/env python3
"""Veto-efficiency view (restart 2026-10-28): one Condor job over the 17 WP-A3 skims.

For each forward veto applied N-1 (F.masks: N1_PLUG / N1_FMD / N1_FTS = A_pre && eta08<3.3|sentinel
&& the other two vetoes), and the combined veto COMB (A_pre && eta08<3.3|sentinel, no veto; fail = any), it accumulates pass/fail sums in bins of
  cell  {D0, D1, D2, outside}   x  eta08 {[-1,1),[1,2),[2,2.5),[2.5,3.0),[3.0,3.3), other(<-1|sentinel)}
  x  M_X {3 bins, non-finite}   x  parity {odd, even}
for data (counts) and, per ND chain, the MC groups signal (W.SIGNAL), reggeon (6671/6677) and nd
(sumw and sumw2 with the hist_ge.py base weights: signed v3 x fac_used/table_fac, no veto map).
Fail: Plug = not (ePlug < 7) (>= 7 GeV or no measurement); FMD = not selFmd; FTS = not selFts
(checked equal to nFts28 > 0).  M_X edges: data tertiles of finite mx in the N1_PLUG sample with
eta08 < 2.5, D cells, both parities (fixed here, before any fail fraction is computed).
Cross-check: integrated odd Plug f_fail per D cell (data and unscaled MC) equals hist_ge.py."""
import argparse, json, sys
from pathlib import Path
import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a3")); sys.path.insert(0, str(HERE.parent / "wp-a"))
import fact_wpa3 as F
import wpa_common as W

CHAINS = ("7071", "7091", "nd4cat")
ETAE = [-1.0, 1.0, 2.0, 2.5, 3.0, 3.3]; NE = len(ETAE)          # 5 bins + "other"
VETOS = ("N1_PLUG", "N1_FMD", "N1_FTS", "COMB")   # COMB: no veto applied, fail = any of the three
GROUPS = {"signal": W.SIGNAL, "reggeon": ["6671", "6677"]}


def fail_of(e, v):
    if v == "COMB": return ~(e["eplug"] < 7.0) | ~e["fmd"] | ~e["fts"]
    return {"N1_PLUG": ~(e["eplug"] < 7.0), "N1_FMD": ~e["fmd"], "N1_FTS": ~e["fts"]}[v]


def ebin(e):
    x = e["ec08"]; i = np.searchsorted(ETAE, x, side="right") - 1
    return np.where(np.isfinite(x) & (x >= ETAE[0]) & (x < ETAE[-1]), i, NE - 1).astype(int)


def mbin(e, me):
    x = e["mx"]; i = np.clip(np.searchsorted(me, x, side="right") - 1, 0, 2)
    return np.where(np.isfinite(x), i, 3).astype(int)


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--out", required=True); a = ap.parse_args()
    labs = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    E = {l: F.load(l) for l in W.DATA_LABELS + labs}
    MK = {(l, p): F.masks(e, p == "even") for l, e in E.items() for p in ("even", "odd")}
    for (l, p), mk in MK.items():   # COMB base = parity & (eta08 < 3.3 | sentinel), same as the N1_* masks without any veto
        e = E[l]; mk["COMB"] = (e["even"] if p == "even" else ~e["even"]) & ((e["ec08"] < 3.3) | (e["ec08"] < -900))
    ftsbad = {l: int((e["fts"] != (e["nfts28"] == 0)).sum()) for l, e in E.items()}
    # M_X tertiles (data, N1_PLUG, eta08 < 2.5, D cells, both parities)
    mx = np.concatenate([E[l]["mx"][(MK[(l, "odd")]["N1_PLUG"] | MK[(l, "even")]["N1_PLUG"]) & (E[l]["ec08"] < 2.5)
                                    & (E[l]["ec08"] >= -1) & (E[l]["cell"] >= 0)] for l in W.DATA_LABELS])
    mx = mx[np.isfinite(mx)]; t1, t2 = np.quantile(mx, [1 / 3, 2 / 3]); me = np.array([-np.inf, t1, t2, np.inf])
    IDX = {l: (np.where(e["cell"] >= 0, e["cell"], 3).astype(int), ebin(e), mbin(e, me)) for l, e in E.items()}
    shape = (4, NE, 4, 2)

    def acc(l, v, p, w):
        c, ib, im = IDX[l]; m = MK[(l, p)][v]; f = fail_of(E[l], v).astype(int)
        s = np.zeros(shape); s2 = np.zeros(shape)
        np.add.at(s, (c[m], ib[m], im[m], f[m]), w[m]); np.add.at(s2, (c[m], ib[m], im[m], f[m]), w[m] ** 2)
        return s, s2

    res = {"script_sha256": W.sha256(__file__), "fact_sha256": W.sha256(F.__file__),
           "inputs": {l: W.sha256(F.SK / (l + ".npz")) for l in E}, "eta_edges": ETAE, "mx_edges": [float(t1), float(t2)],
           "mx_n_data_used": int(mx.size), "axes": ["cell(D0,D1,D2,outside)", "eta(5 bins,other)", "mx(3 bins,nonfinite)", "fail(0 pass,1 fail)"],
           "fts_flag_mismatch_vs_nfts28": ftsbad, "arrays": {}, "checks": {}}
    for v in VETOS:
        for p in ("odd", "even"):
            s = sum(acc(l, v, p, np.ones(E[l]["w"].size))[0] for l in W.DATA_LABELS)
            res["arrays"]["|".join((v, p, "data", "data"))] = {"sumw": s.tolist()}
    for ch in CHAINS:
        nd = W.CHAINS[ch]["nd"]; base = {l: W.fac_used(l, ch) / W.table_fac(l) * E[l]["w"] for l in W.DIFF8 + nd}
        for v in VETOS:
            for p in ("odd", "even"):
                for g, ls in dict(GROUPS, nd=nd).items():
                    r = [acc(l, v, p, base[l]) for l in ls]
                    res["arrays"]["|".join((v, p, ch, g))] = {"sumw": sum(x[0] for x in r).tolist(), "sumw2": sum(x[1] for x in r).tolist()}
    # cross-check vs hist_ge (odd Plug N-1 f_fail per D cell, data and unscaled MC)
    HG = json.load(open(W.D / "data/restart_2026-10-27/gate_evidence/hists_ge.json"))["checks"]
    d = np.asarray(res["arrays"]["N1_PLUG|odd|data|data"]["sumw"]).sum(axis=(1, 2))
    fd = d[:3, 1] / d[:3].sum(1)
    for ch in CHAINS:
        m = sum(np.asarray(res["arrays"]["|".join(("N1_PLUG", "odd", ch, g))]["sumw"]) for g in ("signal", "reggeon", "nd")).sum(axis=(1, 2))
        fm = m[:3, 1] / m[:3].sum(1)
        res["checks"][ch] = {"f_fail_data": fd.tolist(), "f_fail_mc": fm.tolist(),
                             "delta_matches_hist_ge": [bool(abs((fm[c] - fd[c]) - HG["ffail_" + ch]["none_D%d" % c]["delta"]) < 1e-9) for c in range(3)]}
        print(ch, res["checks"][ch], flush=True)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); json.dump(res, open(a.out, "w"))
    print(json.dumps({"out": a.out, "mx_edges": res["mx_edges"], "fts_mismatch": ftsbad}))


if __name__ == "__main__":
    main()

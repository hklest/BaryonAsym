#!/usr/bin/env python3
"""WP-A5 derivation of the forward-veto signal-loss correction q (child plan A5-D1..D5).  Login node, reads
data/restart_2026-10-27/wp-a5/veto_corr_hists.json only.

Per view v in {N1_PLUG, N1_FTS, N1_FMD}, cell c in {D0, D1, D2, ALL}, M_X bin m in {4-10, 10-20, 20-40, ALL}
on the deep-gap N-1 view (eta08 in [-1, 2.5)): f_data, n_data, MC "full" = s_diff(c) x DIFF8 + s_ND(c) x ND(chain)
with the fine G1'(c) scales (veto_eff.json fit_scales_fine; formulas of plot_veto_eff.py), Delta = f_data - f_MC,full,
q = Delta / (1 - f_MC,full), sigma_stat = hypot(err_data, err_mc) / (1 - f_MC,full).  Nominal chain 7071; 7091 and
nd4cat q reported.  ALL cells = D0+D1+D2; ALL M_X = every M_X (incl. <4, >40, non-finite), as veto_eff.json.
eta shape (ALL cells, all M_X): q_eta[bin] for the 7 eta bins and delta_eta = q_eta - q_deepgap.
  --split even : writes veto_correction_even.json (refuses if it exists).
  --split odd  : needs freeze_record.md holding the even sha256; writes veto_correction.json with even + odd per
                 cell, spread = |q_even - q_odd|, sigma_q = max(|q_even|/2, spread) (D14), even_sha256."""
import argparse, datetime, hashlib, json, sys
from pathlib import Path
import numpy as np

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
OD = D / "data/restart_2026-10-27/wp-a5"; HJ = OD / "veto_corr_hists.json"
EVEN = OD / "veto_correction_even.json"; FULL = OD / "veto_correction.json"
FREEZE = D / "docs/restart_2026-10-27/wp-a5/freeze_record.md"
VE = D / "data/restart_2026-10-27/gate_evidence/veto_eff.json"
CHAINS = ("7071", "7091", "nd4cat"); NOM = "7071"; CELLS = ("D0", "D1", "D2"); VIEWS = ("N1_PLUG", "N1_FTS", "N1_FMD")
MXK = {"4-10": [1], "10-20": [2], "20-40": [3], "ALL": [0, 1, 2, 3, 4, 5]}
ETAK = ["[-1,1)", "[1,2)", "[2,2.5)", "[2.5,3.0)", "[3.0,3.2)", "[3.2,3.3)", "other"]
DEEP = [0, 1, 2]


def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()


class Hists:
    def __init__(self, path=HJ, scales=None, suffix=""):
        self.H = json.load(open(path)); self.suf = suffix
        self.SC = scales if scales is not None else json.load(open(VE))["fit_scales_fine"]

    def arr(self, v, p, src, g, k="sumw"):
        return np.asarray(self.H["arrays"]["|".join((v, p, src, g)) + self.suf][k], float)

    @staticmethod
    def red(a, cells, eta, mx):
        return a[np.ix_(cells, eta, mx, [0, 1])].sum(axis=(0, 1, 2))

    def data(self, v, p, cells, eta, mx, src="data"):
        ps, fl = self.red(self.arr(v, p, "data", src), cells, eta, mx); n = ps + fl
        if n <= 0: return np.nan, np.nan, 0.0
        f = fl / n; return f, float(np.sqrt(f * (1 - f) / n)), float(n)

    def mc(self, v, p, ch, cells, eta, mx, which="full"):
        o = np.zeros(4)
        for c in cells:
            sd, sn = (1.0, 1.0) if c > 2 else self.SC[ch][CELLS[c]]
            for g, s in (("signal", sd), ("reggeon", sd), ("nd", sn)):
                if which == "diff" and g == "nd": continue
                o += np.concatenate([s * self.red(self.arr(v, p, ch, g), [c], eta, mx), s * s * self.red(self.arr(v, p, ch, g, "sumw2"), [c], eta, mx)])
        return o

    def entry(self, v, p, cells, eta, mx):
        fd, ed, n = self.data(v, p, cells, eta, mx)
        o = {"f_data": fd, "err_data": ed, "n_data": n}
        for ch in CHAINS:
            ps, fl, s2p, s2f = self.mc(v, p, ch, cells, eta, mx); N = ps + fl
            fm = fl / N; em = float(np.sqrt(max(s2f * (1 - fm) ** 2 + s2p * fm * fm, 0))) / N
            pd, fld, _, _ = self.mc(v, p, ch, cells, eta, mx, "diff")
            d = fd - fm; q = d / (1 - fm)
            o[ch] = {"f_mc_full": fm, "err_mc": em, "purity": (pd + fld) / N, "delta": d, "q": q,
                     "sigma_stat": float(np.hypot(ed, em)) / (1 - fm)}
        return o


def cidx(z): return [0, 1, 2] if z == "ALL" else [CELLS.index(z)]


def split_block(h, p):
    cells = {v: {"%s|%s" % (m, z): h.entry(v, p, cidx(z), DEEP, MXK[m]) for m in MXK for z in CELLS + ("ALL",)} for v in VIEWS}
    eta = {}
    for v in VIEWS:
        qd = h.entry(v, p, [0, 1, 2], DEEP, MXK["ALL"])[NOM]["q"]
        e = {}
        for i, k in enumerate(ETAK):
            en = h.entry(v, p, [0, 1, 2], [i], MXK["ALL"])
            e[k] = {"q": en[NOM]["q"], "delta": en[NOM]["q"] - qd, "sigma_stat": en[NOM]["sigma_stat"], "n_data": en["n_data"],
                    "purity": en[NOM]["purity"], "f_data": en["f_data"], "f_mc_full": en[NOM]["f_mc_full"]}
        eta[v] = {"q_deepgap": qd, "bins": e}
    return cells, eta


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--split", choices=("even", "odd"), required=True); a = ap.parse_args()
    h = Hists(); hx = Hists(suffix="|xip04")
    now = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    meta = {"hists": str(HJ), "hists_sha256": sha(HJ), "hist_script_sha256": h.H["script_sha256"], "skims": h.H["inputs"],
            "derive_script_sha256": sha(__file__), "scales": h.SC, "scales_source": str(VE), "mx_edges": [4, 10, 20, 40],
            "eta_edges": h.H["eta_edges"], "deep_gap": "[-1,2.5)", "nominal_chain": NOM,
            "definition": "q = (f_data - f_MC,full) / (1 - f_MC,full); weight of a passing MC event = prod_v (1 - q_v)"}
    if a.split == "even":
        if EVEN.exists(): sys.exit("FATAL: %s exists (frozen); never overwritten" % EVEN)
        cells, eta = split_block(h, "even"); cx, _ = split_block(hx, "even")
        out = {"meta": dict(meta, split="even", timestamp=now), "cells": cells, "eta_shape": eta,
               "diag_xip04_cells": {v: {k: {"q": e[NOM]["q"], "sigma_stat": e[NOM]["sigma_stat"], "n_data": e["n_data"]} for k, e in cx[v].items()} for v in VIEWS}}
        json.dump(out, open(EVEN, "w"), indent=1, default=float)
        print(json.dumps({"written": str(EVEN), "sha256": sha(EVEN)})); return
    es = sha(EVEN)
    if not FREEZE.exists() or es not in FREEZE.read_text(): sys.exit("FATAL: freeze_record.md missing or lacks the even sha256 %s" % es)
    E = json.load(open(EVEN)); co, eo = split_block(h, "odd")
    cells = {}
    for v in VIEWS:
        cells[v] = {}
        for k, ev in E["cells"][v].items():
            od = co[v][k]; qe, qo = ev[NOM]["q"], od[NOM]["q"]; sp = abs(qe - qo)
            cells[v][k] = {"q": qe, "sigma_q": max(abs(qe) / 2, sp), "spread": sp, "half_q": abs(qe) / 2,
                           "sigma_stat_even": ev[NOM]["sigma_stat"], "sigma_stat_odd": od[NOM]["sigma_stat"],
                           "q_odd": qo, "chain_q_even": {ch: ev[ch]["q"] for ch in CHAINS}, "chain_q_odd": {ch: od[ch]["q"] for ch in CHAINS},
                           "even": ev, "odd": od}
    out = {"meta": dict(meta, split="even derives, odd checks", timestamp=now, even_sha256=es, even_timestamp=E["meta"]["timestamp"]),
           "cells": cells, "eta_shape": E["eta_shape"], "eta_shape_odd": eo, "diag_xip04_cells": E["diag_xip04_cells"]}
    json.dump(out, open(FULL, "w"), indent=1, default=float)
    print(json.dumps({"written": str(FULL), "even_sha256": es}))


if __name__ == "__main__":
    main()

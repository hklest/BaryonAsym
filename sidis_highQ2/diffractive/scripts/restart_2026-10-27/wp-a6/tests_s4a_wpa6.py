#!/usr/bin/env python3
"""S4a checks (login node, small npz only). Run: PYTHONDONTWRITEBYTECODE=1 python3 tests_s4a_wpa6.py"""
import glob, json
import numpy as np

D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/"
OLD, NEW = D + "wp-c/study/", D + "wp-a6/trk/"
SIGNAL = ["9012", "9024", "9033", "9155", "9157", "9158"]
MX = np.array([4., 10., 20., 40.])


def _p(ok, msg):
    print(("PASS " if ok else "FAIL ") + msg); return ok


def wpc_identity():
    ok = True
    for l in SIGNAL:
        o, n = np.load(OLD + "s%s.npz" % l), np.load(NEW + "s%s.npz" % l)
        bad = [k for k in o.files if k not in n.files or not np.array_equal(o[k], n[k], equal_nan=True)]
        ok &= _p(not bad, "wpc_identity %s (%d keys)%s" % (l, len(o.files), " differ: %s" % bad if bad else ""))
    return ok


def wpc_identity_tol(rtol=1e-5):
    """Same keys; ints/bools/NaN pattern exact, floats within rtol (float32 pow/BLAS differ by CPU model across nodes)."""
    ok = True
    for l in SIGNAL:
        o, n = np.load(OLD + "s%s.npz" % l), np.load(NEW + "s%s.npz" % l)
        bad = [k for k in o.files if k not in n.files or (not np.array_equal(o[k], n[k]) if o[k].dtype.kind != "f" else
               not (np.array_equal(np.isnan(o[k]), np.isnan(n[k])) and np.allclose(o[k], n[k], rtol=rtol, atol=0, equal_nan=True)))]
        ok &= _p(not bad, "wpc_identity_tol %s rtol=%g%s" % (l, rtol, " differ: %s" % bad if bad else ""))
    return ok


def counts(d, xcut):
    ev = np.searchsorted(d["e_idx"], d["t_ev"]); mx = d["t_mx"]
    m = np.ones(len(mx), bool)
    if xcut:
        xi = d["e_xIp"][ev]; m = (xi > 0) & (xi < 0.03)
    tab = {}
    for c, name in ((1, "h+"), (-1, "h-")):
        tab[name] = [int((m & (d["t_charge"] == c) & (mx >= lo) & ((mx < hi) if hi < 40 else (mx <= hi))).sum()) for lo, hi in zip(MX[:-1], MX[1:])]
    return tab


def nd_tracks_present():
    ok = True
    for l in ("7071", "6671", "6677"):
        d = np.load(NEW + "b%s.npz" % l)
        for xcut in (False, True):
            tab = counts(d, xcut)
            print("  b%s xIp-cut=%s  M_X [4,10) [10,20) [20,40]: h+ %s  h- %s" % (l, xcut, tab["h+"], tab["h-"]))
            if xcut and l == "7071":
                ok &= _p(min(tab["h+"] + tab["h-"]) >= 20, "nd_tracks_present 7071 (>=20 in every charge x M_X bin, 0<xIp<0.03)")
    return ok


def cell_consistency():
    for l in ["7071", "6671", "6677"] + SIGNAL:
        d = np.load(NEW + ("b%s.npz" if l in ("7071", "6671", "6677") else "s%s.npz") % l)
        c = d["e_cell"]; ok = len(d["t_cell"]) == len(d["t_ev"]) and np.array_equal(d["t_cell"], c[np.searchsorted(d["e_idx"], d["t_ev"])])
        _p(ok, "cell_consistency %s: e_cell=-1 fraction %.4f (n_ev %d)" % (l, (c == -1).mean(), len(c)))


def merge_consistency():
    d = np.load(NEW + "b7071.npz"); j = json.load(open(NEW + "b7071.json"))
    parts = [json.load(open(p)) for p in sorted(glob.glob(NEW + "parts_7071/b7071_*.json"))]
    se = sum(p["sel_events"] for p in parts); st = sum(p["tracks_selected"] for p in parts)
    ok = len(parts) == 61 and len(d["e_idx"]) == se == j["sel_events"] and len(d["t_ev"]) == st == j["tracks_selected"] and np.all(np.diff(d["e_idx"]) > 0)
    return _p(ok, "merge_consistency: parts %d, events %d vs %d, tracks %d vs %d" % (len(parts), len(d["e_idx"]), se, len(d["t_ev"]), st))


if __name__ == "__main__":
    wpc_identity(); wpc_identity_tol(); nd_tracks_present(); cell_consistency(); merge_consistency()

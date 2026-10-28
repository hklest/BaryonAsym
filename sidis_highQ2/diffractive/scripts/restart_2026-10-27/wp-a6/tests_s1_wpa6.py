#!/usr/bin/env python3
"""WP-A6 slice-1 tests (to be merged into tests_wpa6.py).  Run: python3 tests_s1_wpa6.py"""
import json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[0] / "wp-a3")); sys.path.insert(0, str(HERE.parents[0] / "wp-a"))
import fact_wpa3 as F
import wpa_common as W

PL = W.D / "data/restart_2026-10-27/wp-a6/payloads"
REF = W.D / "data/restart_2026-10-27/wp-a2/s4_g1p/payloads"
CELLS = ("D0", "D1", "D2"); CLS = ("MX4_10", "MX10_20", "MX20_40", "MXout")
ld = lambda p: json.load(open(p))


def csum(blk, cell, w=None):
    """sum over classes of a template dict {cell: {cls: [120]}} -> array(120)"""
    return sum(np.asarray(blk[cell][k], float) for k in CLS)


def payload_identity():
    bad, det = [], []
    for ch in ("7071", "7091", "nd4cat"):
        p = ld(PL / ("odd_%s.json" % ch)); r = ld(REF / ("odd_frozen_%s.json" % ch)); pm, rm = p["modes"]["cluster"], r["modes"]["cluster"]
        nb = int(np.sum(np.asarray(p["edges"])[1:] <= 3.0 + 1e-9)); assert np.allclose(p["edges"][:nb + 1], r["edges"][:nb + 1])
        sg = True; mxe = 0.0
        for c in CELLS:
            if not np.array_equal(csum(pm["data"]["templates"]["raw"], c)[:nb], np.asarray(rm["data"]["templates"]["raw"][c])[:nb]): bad.append(ch + " data " + c)
            for l, rc in rm["components"].items():
                pc = pm["components"][l]["templates"]
                if not np.array_equal(csum(pc["raw"], c)[:nb], np.asarray(rc["templates"]["raw"][c])[:nb]): bad.append("%s %s %s raw" % (ch, l, c))
                a, b = csum(pc["exposure1"], c)[:nb], np.asarray(rc["templates"]["exposure1"][c])[:nb]
                mxe = max(mxe, float(np.max(np.abs(a - b) / np.maximum(np.abs(b), 1e-300) * (b != 0)))) if b.any() else mxe
                if abs(a.sum() - b.sum()) > 1e-6 * abs(b.sum()): bad.append("%s %s %s exposure1" % (ch, l, c))
                sg &= bool(np.allclose(csum(pc["signed"], c)[:nb], np.asarray(rc["templates"]["signed"][c])[:nb], rtol=1e-9, atol=0))
        if set(rm["components"]) != set(pm["components"]): bad.append(ch + " component set")
        det.append("%s: bins<3.0=%d, max rel exposure1 bin dev %.2e, signed_match=%s" % (ch, nb, mxe, sg))
    return "payload_identity", not bad, "; ".join(det) + (" BAD " + str(bad[:5]) if bad else "")


def mx_partition():
    p = ld(PL / "odd_7071.json"); m = p["modes"]["cluster"]; L = "9024"; e = F.load(L); mk = F.masks(e, False)["FIT"]
    x, ok = e["ec04"], True; det = []; cell = e["cell"]; E = np.asarray(p["edges"])
    inr = np.isfinite(x) & (x >= -1) & (x <= 5); mx = e["mx"]
    for c, cn in enumerate(CELLS):
        s = mk & (cell == c); tot = s.sum()
        cl = sum(sum(m["components"][L]["templates"]["raw"][cn][k]) + m["components"][L]["oor"]["raw"][cn][k] for k in CLS)
        direct = np.histogram(x[s & inr], E)[0]
        bb = csum(m["components"][L]["templates"]["raw"], cn)
        o = round(cl) == tot and np.array_equal(bb, direct); ok &= o
        det.append("%s: classes+oor=%d vs skim FIT=%d, bins equal=%s" % (cn, round(cl), tot, np.array_equal(bb, direct)))
    return "mx_partition", bool(ok), "label 9024 odd 7071; " + "; ".join(det)


def sel_count_one_file():
    p = ld(PL / "odd_7071.json"); e = F.load("ep0304")
    exp = int(np.sum(F.masks(e, False)["SB"] & (e["xip"] < 0.04) & (e["mx"] > 3) & (e["cell"] >= 0)))
    # payload sel data is summed over the 4 period skims; ep0304 alone from diagnostics is not cell-split, so rebuild from payload inputs:
    d = p["diagnostics"]["n_sel_cells"]["ep0304"]; got = sum(d)
    tot = sum(sum(sum(p["sel"]["data"]["templates"]["raw"][c][k]) + p["sel"]["data"]["oor"][c][k] for k in CLS) for c in CELLS)
    allp = sum(sum(p["diagnostics"]["n_sel_cells"][l]) for l in W.DATA_LABELS)
    return "sel_count_one_file", got == exp and round(tot) == allp, "ep0304: payload per-cell %s = %d vs direct (cell>=0) %d; payload sel data (4 skims, classes+oor) %d vs sum of per-label %d; cell -1 excluded by construction (D0-D2 blocks only)" % (d, got, exp, round(tot), allp)


def sumw2_present():
    n = 0; bad = []
    for fn in sorted(PL.glob("*_*.json")):
        p = ld(fn); blocks = [("sel", p["sel"])] + [(m, p["modes"][m]) for m in p["modes"]]
        for mn, b in blocks:
            for l, c in b["components"].items():
                t = c["templates"]
                for cn in CELLS:
                    for k in CLS:
                        n += 1
                        if "sumw2" not in t or len(t["sumw2"][cn][k]) != len(t["raw"][cn][k]) or len(t["raw"][cn][k]) != 120: bad.append((fn.name, mn, l, cn, k))
    return "sumw2_present", not bad and n > 0, "%d blocks checked, %d bad %s" % (n, len(bad), bad[:3])


if __name__ == "__main__":
    allok = True
    for t in (payload_identity, mx_partition, sel_count_one_file, sumw2_present):
        n, ok, d = t(); allok &= ok; print("%s %s: %s" % ("PASS" if ok else "FAIL", n, d))
    sys.exit(0 if allok else 1)

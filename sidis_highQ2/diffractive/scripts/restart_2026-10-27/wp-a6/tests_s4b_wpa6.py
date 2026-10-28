#!/usr/bin/env python3
"""S4b checks (login node, small npz). Run: PYTHONDONTWRITEBYTECODE=1 python3 tests_s4b_wpa6.py [<binstats csv> [<mask json>]]
Default CSV/mask are the production purity/ outputs; pass scratch paths for a dev run."""
import sys, csv, json, hashlib, importlib.util
import numpy as np
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/"
P = D + "data/restart_2026-10-27/wp-a6/purity/"
CSV = sys.argv[1] if len(sys.argv) > 1 else P + "binstats_wpa6_D7plus_B_xip030.csv"
MASK = sys.argv[2] if len(sys.argv) > 2 else P + "mask_wpa6_D7plus_B_xip030_2026-10-28.json"
WPC = D + "data/restart_2026-10-27/wp-c/"
WPC_SHA = {  # recorded 2026-10-28 before the first S4b run
    WPC + "study/binstats_proposal_D7plus_xip030_frameB_G2.csv": "207ab6ae18f48ac4e30220d0c806d3450a4c7997a4b89a91b09919e99ae03609",
    WPC + "mask/mask_D7plus_xip030_frameB_G2.json": "c89fd2b65c7062d88fc18219552286880776d14373d08bd07bccda410bcb83d1",
    WPC + "mask/mask_D7plus_xip030_frameB_G2.json.sha256": "7f537c4e83152a01ce3341bd16da414e73aa479e8a4233fc50c93d41be7c2f74"}
sha = lambda p: hashlib.sha256(open(p, "rb").read()).hexdigest()
spec = importlib.util.spec_from_file_location("pf", D + "scripts/restart_2026-10-27/wp-a6/purity_fill_wpa6.py")
pf = importlib.util.module_from_spec(spec); spec.loader.exec_module(pf)


def _p(ok, msg):
    print(("PASS " if ok else "FAIL ") + msg); return bool(ok)


def fractions_sum():
    R = list(csv.DictReader(open(CSV)))
    d = max(abs(sum(float(r[k]) for k in ("purity", "f_mig", "f_fake", "f_nd", "f_reg")) - 1) for r in R)
    return _p(d < 1e-6, "fractions_sum max|sum-1| = %.2e over %d bins" % (d, len(R)))


def purity_table():
    R = list(csv.DictReader(open(CSV))); ok = True
    for ax in ("xF", "pt2"):
        for c in ("h+", "h-"):
            for m in pf.MXN:
                ok &= any(r["axis"] == ax and r["charge"] == c and r["mx_bin"] == m for r in R)
    ok &= all(all(r[k] != "" for k in ("f_nd", "f_reg", "purity", "masked")) for r in R)
    return _p(ok, "purity_table %d rows, 2 axes x 2 charges x 3 M_X present, f_nd/f_reg/purity/masked filled" % len(R))


def wpc_untouched():
    bad = [p for p, h in WPC_SHA.items() if sha(p) != h]
    return _p(not bad, "wpc_untouched %s" % (bad or "3 files unchanged"))


def mask_sha():
    doc = open(MASK, "rb").read(); side = open(MASK + ".sha256").read().split()[0]
    j = json.loads(doc)
    return _p(hashlib.sha256(doc).hexdigest() == side and j["source_sha256"] == sha(CSV),
              "mask_sha sidecar matches, source_sha256 == csv sha")


def wpc_reduction():
    edges = json.load(open(pf.WPC_EDGES)); Ts = pf.load_tracks(pf.SIGNAL, 0.03, "s")
    rows = pf.table(Ts, Ts["t_w"], None, None, None, None, edges, "B")
    W = {(r["charge"], r["mx_bin"], r["axis"], int(r["bin"])): r for r in csv.DictReader(open(pf.WPC_CSV))}
    ok = len(rows) == len(W); worst = 0.0; badc = set()
    for r in rows:
        w = W[(r["charge"], r["mx_bin"], r["axis"], r["bin"])]
        for k in ("n_sig_pred", "purity", "stability", "p_mig", "f_fake"):
            x, y = r[k], float(w[k]); rel = abs(x - y) / max(abs(y), 1e-12) if abs(y) > 1e-3 else abs(x - y)
            worst = max(worst, rel)
            if rel > 1e-3: badc.add(k)
        for k in ("n_mc_raw", "n_mc_raw_minreco"):
            if r[k] != int(w[k]): badc.add(k)
        if r["lo"] != float(w["lo"]) or r["hi"] != float(w["hi"]): badc.add("edges")
    return _p(ok and not badc, "wpc_reduction %d bins, worst rel dev %.2e, mismatched columns %s" % (len(rows), worst, sorted(badc) or "none"))


if __name__ == "__main__":
    res = [t() for t in (fractions_sum, purity_table, wpc_untouched, mask_sha, wpc_reduction)]
    sys.exit(0 if all(res) else 1)

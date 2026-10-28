#!/usr/bin/env python3
"""WP-A5 named tests (child plan Slices 1-3).  Prints PASS/FAIL per test; exit 1 on any FAIL.
Usage: tests_wpa5.py [--slice 1|2|3|all]"""
import argparse, csv, json, re, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent; sys.path.insert(0, str(HERE)); sys.path.insert(0, str(HERE.parent / "wp-c" / "study"))
import derive_veto_corr as DV

D = DV.D; OD = DV.OD; RES = []


def rep(name, ok, msg=""):
    RES.append(ok); print("%s %s %s" % ("PASS" if ok else "FAIL", name, msg), flush=True)


def slice1():
    h = DV.Hists(); SM = json.load(open(D / "data/restart_2026-10-27/gate_evidence/summary.json")); VE = json.load(open(DV.VE))
    ok = True; msg = []
    for i, z in enumerate(DV.CELLS):
        f = h.data("N1_PLUG", "odd", [i], list(range(7)), DV.MXK["ALL"])[0]; ref = SM["fig2_7071"][z]["f_fail_data"]
        ok &= abs(f - ref) < 1e-6; msg.append("%s %.4f" % (z, f))
    rep("fig2_consistency", ok, " ".join(msg))
    e = h.entry("N1_PLUG", "odd", [0, 1, 2], DV.DEEP, DV.MXK["ALL"]); r = VE["deep_gap_eta_lt_2p5"]["N1_PLUG|odd|ALL"]
    ok = abs(e["7071"]["delta"] - r["7071"]["delta_data_minus_full"]) < 1e-6 and e["n_data"] == 5755 == r["data"]["n"]
    # extended: every view x parity x cell x chain of the deep-gap table
    dmax = 0.0; n = 0
    for v in ("N1_PLUG", "N1_FMD", "N1_FTS", "COMB"):
        for p in ("odd", "even"):
            for z in DV.CELLS + ("ALL",):
                ee = h.entry(v, p, DV.cidx(z), DV.DEEP, DV.MXK["ALL"]); rr = VE["deep_gap_eta_lt_2p5"]["|".join((v, p, z))]
                for ch in DV.CHAINS:
                    dmax = max(dmax, abs(ee[ch]["delta"] - rr[ch]["delta_data_minus_full"])); n += 1
    hj = json.load(open(DV.HJ))["checks"]["integrated_vs_veto_eff_hists"]
    rep("deep_gap_reproduced", ok and dmax < 1e-6 and hj["pass_1e-9"],
        "Plug ALL odd delta %.6f n %d; all %d deep-gap deltas max dev %.1e; hist integration max rel dev %.1e" % (e["7071"]["delta"], e["n_data"], n, dmax, hj["max_rel_dev"]))
    C = json.load(open(DV.FULL)); E = json.load(open(DV.EVEN)); fr = DV.FREEZE.read_text()
    tf = re.search(r"Frozen at \(UTC\): (\S+)", fr).group(1)
    ok = C["meta"]["even_sha256"] == DV.sha(DV.EVEN) and C["meta"]["even_sha256"] in fr and E["meta"]["timestamp"] <= tf < C["meta"]["timestamp"]
    rep("even_frozen_first", ok, "even %s <= freeze %s < odd %s" % (E["meta"]["timestamp"], tf, C["meta"]["timestamp"]))
    ok = all(e["sigma_q"] >= abs(e["q"]) / 2 - 1e-15 and e["sigma_q"] >= e["spread"] - 1e-15 for v in C["cells"].values() for e in v.values())
    rep("syst_floor", ok, "%d cells" % sum(len(v) for v in C["cells"].values()))
    hits = []
    for p in sorted(HERE.glob("*.py")):
        if p.name == Path(__file__).name: continue
        for i, l in enumerate(p.read_text().splitlines(), 1):
            if re.search(r"<\s*1\.5\b|\b1\.5\b|remedy1|qbar", l, re.I): hits.append("%s:%d" % (p.name, i))
    rep("no_control_1p5", not hits, ",".join(hits))


def slice2():
    import wpc_lib as L, veto_weight as VW
    ok = True; msg = []
    for l in L.LABELS:
        c = np.load(OD / ("cells_%s.npz" % l)); ec = c["ec08"].astype(float)
        u = np.unique(c["run"].astype(np.int64) * 10**10 + c["event"]).size == c["run"].size
        ok &= u and bool(np.all((ec < 3.2) | (ec < -900))); msg.append("%s:%d" % (l, c["run"].size))
    rep("join_keys", ok, " ".join(msg))
    T = json.load(open(OD / "g1pp_a_table.json")); rows = T["rows"]
    import propagate_wpa5 as P
    Tt, Ee, cell, ec, w0, g, emb = P.sample(); nev = {}
    chg = (Tt["t_charge"] < 0).astype(int)
    for fr in "AB":
        for ax, pre, edges in P.AX:
            _, mb, ob, _ = L.cell_id(Tt["t_mx"], Tt[pre + fr], L.MX_EDGES, [edges] * 3)
            nev[(fr, ax)] = np.where((mb >= 0) & (ob >= 0), (chg * 3 + mb) * (len(edges) - 1) + ob, -1)
    Sz = P.spectra(Tt, g, emb, w0 * VW.weights(Ee["e_mx"], cell, ec, "zero"), nev); Su = P.spectra(Tt, g, emb, w0, nev)
    rz = np.concatenate([(Sz[k][Su[k] > 0] / Su[k][Su[k] > 0] - 1) for k in Su])
    sc = json.load(open(DV.VE))["fit_scales_fine"]["7071"]; sd = np.array([sc["D0"][0], sc["D1"][0], sc["D2"][0], 1.0])
    tot = np.zeros(3)
    for l in L.LABELS:
        s = np.load(L.BASE + "s%s.npz" % l); c = np.load(OD / ("cells_%s.npz" % l)); mb = VW.mx_index(s["e_mx"])
        w = s["e_w"] * sd[np.where(c["cell"] >= 0, c["cell"], 3)]
        for i in range(3): tot[i] += w[mb == i].sum()
    ref = np.array(T["meta"]["sum_w_events_per_mx"]["skim_e_w_x_sdiff"])
    rep("zero_variant_identity", float(np.max(np.abs(rz))) == 0.0 and np.allclose(tot, ref, rtol=1e-12),
        "max|S_zero/S_unweighted-1| %.1e over %d bins; sum w per M_X %s" % (float(np.max(np.abs(rz))), len(rz), np.round(tot, 3).tolist()))
    mx = np.array([np.nan, 2.0, 50.0, 4.0, 40.0, 15.0, 25.0, 4.0]); cell = np.array([-1, 0, 1, 2, -1, 0, 2, 1])
    ec = np.array([-999.0, np.nan, 3.5, 2.7, -1.5, 0.0, 3.1, 3.25])
    vs = ["nominal", "up", "down", "eta", "eta_up", "eta_down", "zero"] + [v + "_" + d for d in VW.DETS for v in ("nominal", "up", "down", "eta")]
    ok = all(np.all(np.isfinite(VW.weights(mx, cell, ec, v))) for v in vs)
    C = VW.load()   # fallback keys: cell -1 -> "<mx>|ALL"; M_X out of range -> "ALL|<cell>"; both -> "ALL|ALL"
    exp = lambda k: np.prod([1 - C["cells"][d][k]["q"] for d in VW.DETS])
    fb = VW.weights(np.array([15.0, 50.0, np.nan, 2.0]), np.array([-1, 1, -1, 2]), np.zeros(4))
    ok &= bool(np.allclose(fb, [exp("10-20|ALL"), exp("ALL|D1"), exp("ALL|ALL"), exp("ALL|D2")], rtol=1e-14, atol=0))
    try:
        VW.weights(mx, cell, ec, "up_N1_PLUGX"); ok = False
    except ValueError:
        pass
    for l in L.LABELS:
        s = np.load(L.BASE + "s%s.npz" % l); c = np.load(OD / ("cells_%s.npz" % l))
        ok &= all(np.all(np.isfinite(VW.weights(s["e_mx"], c["cell"], c["ec08"], v))) for v in vs)
    rep("weight_total", ok, "%d variants, synthetic edge cases + six skims" % len(vs))
    with open(OD / "g1pp_a_table.csv") as f: cr = list(csv.DictReader(f))
    need = {"charge", "mx_bin", "axis", "bin", "frame", "S_nom", "R_up", "R_down", "R_eta", "syst_veto_rel", "boot_sigma", "masked_D7", "masked_veto"}
    rep("g1pp_a_table", len(cr) == 120 and need <= set(cr[0]) and len({(r["frame"], r["charge"], r["mx_bin"], r["axis"], r["bin"]) for r in cr}) == 120, "%d rows" % len(cr))


def slice3():
    G = json.load(open(OD / "diagnostics.json"))
    ok = all(len(G["period"][v][p]["f_data"]) == 4 and G["period"][v][p]["ndf"] == 3 and np.isfinite(G["period"][v][p]["chi2"])
             for v in DV.VIEWS for p in ("even", "odd"))
    rep("period_table", ok, "4 periods x 3 detectors x 2 parities")
    ok = all(np.isfinite(G["comb_closure"][p][z]["pull_corr"]) for p in ("even", "odd") for z in DV.CELLS + ("ALL",))
    rep("comb_closure", ok, " ".join("%s:%+.2f" % (p, G["comb_closure"][p]["ALL"]["pull_corr"]) for p in ("even", "odd")))
    V = (D / "docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md")
    t = V.read_text() if V.exists() else ""
    secs = ["## 1. Pre-registered gate", "## 2. Inputs and provenance", "## 3. Correction table", "## 4. Validation tests",
            "## 5. Per-bin table and mask list", "## 6. Diagnostics", "## 7. Deviations", "## 8. Ruling"]
    sec4 = t.split("## 4.")[1].split("## 5.")[0] if "## 4." in t and "## 5." in t else ""
    rows4 = [l for l in sec4.splitlines() if l.startswith("| `")]
    ok = all(s in t for s in secs) and rows4 and all(("PASS" in l) or ("FAIL" in l) for l in rows4) and \
        "Mask list, frame A" in t and "Mask list, frame B" in t and re.search(r"^Ruling:", t, re.M) is not None
    rep("verdict_sections", bool(ok), "%d test rows" % len(rows4))


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--slice", default="all"); a = ap.parse_args()
    for s, f in (("1", slice1), ("2", slice2), ("3", slice3)):
        if a.slice in (s, "all"): f()
    print("ALL PASS" if all(RES) else "SOME FAIL"); sys.exit(0 if all(RES) else 1)


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""WP-C C1 tests: plain script with named checks.  Usage: tests_c1.py slice1|slice2|slice3|slice4|all  (PYTHONDONTWRITEBYTECODE=1).
slice1 reads single small files (login node); slice2/3/4 read the C1 tables written by the Condor job."""
import csv, glob, hashlib, json, os, re, sys
from pathlib import Path
import numpy as np
sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parent))
import c1_lib as C

RES = []
LOG = C.D / "condor/restart_2026-10-27/wp-c/c1/logs"
DOC = C.D / "docs/restart_2026-10-27/wp-c"


def check(name, ok, info=""):
    RES.append((name, bool(ok))); print("%s  %-26s %s" % ("PASS" if ok else "FAIL", name, info), flush=True)


def slice1():
    # cells_840x: four check lines
    ok, last = True, {}
    for l in (8402, 8403, 8404, 8405):
        fs = sorted(glob.glob(str(LOG / ("attach_%d_*.out" % l))), key=os.path.getmtime)
        line = [x for x in open(fs[-1]) if x.startswith("{")][-1]; j = json.loads(line); last[l] = j
        ok &= j["ec08_ok"] and j["run_event_unique"] and j["mx_matches_skim"]
    check("cells_840x", ok, "events %s" % {l: j["n_events"] for l, j in last.items()})
    sc = {r["component"]: float(r["scale_to_data"]) for r in csv.DictReader(open(C.D / "data/restart_2026-10-27/wp-d2/scale_table_840x.csv"))}
    ok = True
    for l in (8402, 8403, 8404, 8405):
        ew = np.load(C.L.BASE + "s%d.npz" % l)["e_w"]; ok &= bool(np.all(np.abs(ew - sc[str(l)]) < 1e-7))
    check("weights_840x", ok, "e_w == scale_to_data %s (8402 -> %.6f)" % (sc, sc["8402"]))
    check("labels_c1", all(l in C.LABELS_C1 for l in (8402, 8403, 8404, 8405)) and 9012 not in C.LABELS_C1, str(C.LABELS_C1))
    e = json.load(open(C.UG / "user_grid_summary.json"))["edges"]
    ok = all(np.allclose(C.PT2_EDGES[q], e["('A', 'pt2s', '%s')" % q], atol=5e-4) for q in ("q5", "q4", "q3"))
    check("edges_frame_A", ok, str(C.PT2_EDGES))
    # definitions on synthetic 4-vectors through the shared xframe_obs functions (skims carry no 4-vectors)
    XF = C.XF; rng = np.random.default_rng(3); n = 10000
    q = np.stack([rng.normal(0, 8, n), rng.normal(0, 8, n), -rng.uniform(5, 25, n), rng.uniform(2, 12, n)], -1)
    pX, xi = XF.pX_collinear(q, np.full(n, 920.), rng.uniform(4, 40, n)); ok_ = np.isfinite(xi)
    res = XF.boost_to_rest(pX, pX, np.sqrt(XF.msq(pX)))
    check("boost_closure", np.abs(res[ok_, :3]).max() < 1e-6 and np.abs(res[ok_, 3] - np.sqrt(XF.msq(pX))[ok_]).max() < 1e-6,
          "max |p*| %.1e" % np.abs(res[ok_, :3]).max())
    h = np.stack([rng.normal(0, 1, n), rng.normal(0, 1, n), rng.normal(0, 3, n), np.zeros(n)], -1); h[:, 3] = np.sqrt((h[:, :3] ** 2).sum(1) + C.X.M_PION ** 2)
    xf, pt2, M = XF.frame_obs(h, pX, q); hb = XF.boost_to_rest(h, pX, M)
    zs_a = C.zstar(xf, pt2, M, C.X.M_PION); zs_b = 2 * hb[:, 3] / M; m = ok_ & np.isfinite(zs_a)
    check("zstar_identity", np.abs(zs_a - zs_b)[m].max() < 1e-6, "n=%d max|d| %.1e" % (m.sum(), np.abs(zs_a - zs_b)[m].max()))


def _csv(name):
    return list(csv.DictReader(open(C.W / name)))


def _engine():
    ENG = "/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_percharge"
    sys.path.insert(0, ENG); import crosscheck_unfold_methods_v4 as eng
    return eng


def slice2():
    ug = json.load(open(C.UG / "user_grid_summary.json"))["meta"]["beta"]["data_median"]
    cj = json.load(open(C.W / "cells_c1.json")); g = json.load(open(C.W / "grid_c1.json"))
    check("beta_split_reproduced", round(ug, 3) == C.BETA_SPLIT, "data median %.5f -> %.3f; MC unweighted median %.4f" % (ug, C.BETA_SPLIT, cj["mc_unweighted_beta_median"]))
    A = _csv("binstats_c1_A.csv")
    ok = True
    for cn, v in g["cells"].items():
        idx = C.GRIDS.index(v["pt2_grid"])
        for name in C.GRIDS[:idx]:   # every finer grid must fail somewhere
            rr = [r for r in A if r["axis"] == "pt2" and r["grid"] == name and r["cell_name"] == cn and r["charge"] in ("h+", "h-")]
            ok &= not all(r["pass_D7plus"] == "True" for r in rr)
        rr = [r for r in A if r["axis"] == "pt2" and r["grid"] == v["pt2_grid"] and r["cell_name"] == cn and r["charge"] in ("h+", "h-")]
        allpass = all(r["pass_D7plus"] == "True" for r in rr)
        if not allpass: print("   note: cell %s keeps %s with failing bins masked (C1-8)" % (cn, v["pt2_grid"]))
    check("grid_rule_applied", ok, str({k: v["pt2_grid"] for k, v in g["cells"].items()}))
    mk = sorted(glob.glob(str(C.W / "mask_c1_*_standin.json")))[-1]; m = json.load(open(mk))
    gridsets = {}
    for b in m["bins"]:
        if b["axis"] == "pt2": gridsets.setdefault((b["hemi"], b["beta_bin"]), set()).add(b["grid"])
    check("same_grid_both_charges", all(len(v) == 1 for v in gridsets.values()) and len(g["cells"]) == 4, str(gridsets))
    bad = 0; n = 0
    for fr in "AB":
        for r in _csv("binstats_c1_%s.csv" % fr):
            if float(r["f_nd"]) + float(r["f_reg"]) + float(r["purity"]) + float(r["f_mig"]) + float(r["f_fake"]) == 0 and r["purity"] == "nan": continue
            if r["purity"] in ("nan", ""): continue
            n += 1; bad += abs(float(r["purity"]) + float(r["f_mig"]) + float(r["f_fake"]) + float(r["f_nd"]) + float(r["f_reg"]) - 1) > 1e-6
    check("fractions_sum", bad == 0 and n > 0, "%d rows, %d off" % (n, bad))
    ti = cj["template"]; want = {"0": 0.040, "1": 0.243, "2": 0.221}
    ok = all(abs(ti["f_nd_standin"][k]["f_nd"] - v) < 1e-3 for k, v in want.items()) and all(abs(ti["nd_event_share_after_scaling"][int(k)] - ti["f_nd_standin"][k]["f_nd"]) < 1e-9 for k in want)
    check("purity_source", ok, "f_nd %s chains %s share %s" % ([round(ti["f_nd_standin"][k]["f_nd"], 4) for k in want], [ti["f_nd_standin"][k]["chain"] for k in want], [round(x, 4) for x in ti["nd_event_share_after_scaling"]]))
    p = cj["hemisphere_partition"]; check("hemisphere_partition", all(v["n_G"] + v["n_P"] + v["n_undef"] == v["n_all"] for v in p.values()),
                                          "undefined x_F tracks: %d of %d" % (sum(v["n_undef"] for v in p.values()), sum(v["n_all"] for v in p.values())))
    d = cj["definition_checks"]
    check("zstar_range", d["gen_zstar_le0"] == 0 and d["gen_zstar_gt1_by_1e9_frac"] < 1e-4, "gen z* in [%.4f, %.4f], %d of %d gen tracks above 1 (G2 proxy artefact, reported; not exact); reco z*>1 fraction per cell (A) %s" % (d["gen_zstar_min"], d["gen_zstar_max"], d["gen_zstar_gt1_by_1e9"], d["n_gen_tracks"], {k: round(v["reco_zstar_gt1_fraction_A"], 4) for k, v in cj["cells"].items()}))
    check("xf_range", d["gen_xf_gt1"] / d["n_gen_tracks"] < 1e-4, "gen x_F in [%.4f, %.4f], |x_F|>1 for %d of %d tracks (G2 proxy artefact, reported)" % (d["gen_xf_min"], d["gen_xf_max"], d["gen_xf_gt1"], d["n_gen_tracks"]))
    check("flip_fraction_reported", len(cj["flip_by_cell_gen_bin"]) > 0, "%d (charge, cell, axis, bin) entries; flip > 0.30: %d" % (len(cj["flip_by_cell_gen_bin"]), cj["n_flip_gt_0p30"]))
    key = {(r["charge"], r["hemi"], int(r["beta_bin"]), r["axis"], int(r["bin"])): r for r in A if r["axis"] == "z" or r["grid"] == g["cells"][r["cell_name"]]["pt2_grid"]}
    ok = all(b["masked"] or float(key[(b["charge"], b["hemi"], b["beta_bin"], b["axis"], b["bin"])]["intercell_mig"]) < C.THR["intercell"] for b in m["bins"])
    check("intercell_migration", ok, "max intercell over unmasked bins %.3f" % max([float(key[(b["charge"], b["hemi"], b["beta_bin"], b["axis"], b["bin"])]["intercell_mig"]) for b in m["bins"] if not b["masked"]] + [0]))
    sc = open(mk + ".sha256").read().split()[0]
    check("mask_hashed", sc == hashlib.sha256(open(mk, "rb").read()).hexdigest() and m["data_files_opened"] == [], "%s sha %s masked %d/%d" % (Path(mk).name, sc[:12], m["n_masked"], m["n_bins"]))
    # data files: static scan of the C1 scripts
    pat = re.compile(r"uproot\.open|ug_data|skims/|ep0304|em0405|em06_true|ep0607_true")
    hits = [f for f in glob.glob(str(Path(__file__).resolve().parent / "c1_*.py")) if pat.search(open(f).read())]
    check("no_data_open", not hits, "scripts naming a data input: %s" % hits)
    eng = _engine(); cases = {}
    ng = sum(C.nbins("z", "q5") + C.nbins("pt2", v["pt2_grid"]) for v in g["cells"].values()) * 1   # per charge, gen bins (z 2 + pt2 nb per cell)
    ng = 4 * 2 + sum(C.nbins("pt2", v["pt2_grid"]) for v in g["cells"].values()); nr = 2 * ng
    sys.path.insert(0, str(C.S / "wp-c/engine_shape")); rng = np.random.default_rng(11)
    truth = rng.uniform(500., 5000., ng); eff = rng.uniform(0.55, 0.65, ng); B = np.zeros((nr, ng))
    for j in range(ng):
        c = (j + 0.5) / ng * nr; i0 = int(np.clip(round(c - 0.5), 1, nr - 3)); wv = rng.dirichlet([5., 5., 2., 2.])
        B[i0, j] += wv[0]; B[i0 + 1, j] += wv[1]; B[i0 - 1, j] += wv[2]; B[i0 + 2, j] += wv[3]; B[:, j] *= eff[j] * truth[j]
    fake = 0.1 / 0.9 * B.sum(axis=1); M = np.zeros((nr + 1, ng + 1)); M[:nr, :ng] = B; M[nr, :ng] = truth * (1 - eff); M[:nr, ng] = fake
    Dd = B.sum(axis=1) * (1 + fake / B.sum(axis=1)); bl = eng.derive_response_blocks(M, Dd)
    r = eng.ibu_unfold(Dd * (1 - bl["f_i"]), np.zeros(nr), bl["B"], truth, [4, 50])
    clo = float(np.max(np.abs(r["N_corr"][0] / truth - 1)))
    check("engine_shape_c1", clo < 1e-3 and bool(r["valid"].all()), "gen %d reco %d (unequal blocks per cell) closure k4 %.1e" % (ng, nr, clo))


def slice3():
    import c1_stats as ST
    ev = {k: np.concatenate([np.load(C.W / "cells" / ("cells_%d.npz" % l))["event"].astype(np.int64) if k == "event" else
                             np.full(len(np.load(C.W / "cells" / ("cells_%d.npz" % l))["event"]), C.TWIN_STREAM[l] if k == "stream" else l) for l in (8402, 8403, 8404, 8405)])
          for k in ("event", "stream", "lab")}
    inv, nu = ST.unit_index(ev); lab = ev["lab"]
    def units(l): return set(inv[lab == l].tolist())
    ok = (len(units(8402) & units(8405)) > 5000 and len(units(8403) & units(8404)) > 5000 and len(units(8402) & units(8403)) == 0 and len(units(8402) & units(8404)) == 0)
    check("boot_unit", ok, "shared units 8402-8405 %d, 8403-8404 %d; cross-stream %d" % (len(units(8402) & units(8405)), len(units(8403) & units(8404)), len(units(8402) & units(8403))))
    bj = json.load(open(C.W / "boot_c1.json")); check("boot_count", bj["nboot"] == 200 and all(np.isfinite(r["sigma_purity"]) and np.isfinite(r["sigma_stability"]) for r in bj["rows"] if r["n_valid"] > 100),
                                                       "%d rows, nboot %d, max sigma_purity %.4f" % (len(bj["rows"]), bj["nboot"], max(r["sigma_purity"] for r in bj["rows"])))
    mk = sorted(glob.glob(str(C.W / "mask_c1_*_standin.json")))[-1]; m = json.load(open(mk)); g = json.load(open(C.W / "grid_c1.json")); A = _csv("binstats_c1_A.csv")
    key = {(r["charge"], r["hemi"], int(r["beta_bin"]), r["axis"], int(r["bin"])): r for r in A if r["axis"] == "z" or r["grid"] == g["cells"][r["cell_name"]]["pt2_grid"]}
    ok = all(b["masked"] or (int(key[(b["charge"], b["hemi"], b["beta_bin"], b["axis"], b["bin"])]["n_raw_gen_840x"]) >= 500 and int(key[(b["charge"], b["hemi"], b["beta_bin"], b["axis"], b["bin"])]["n_raw_element_min"]) >= 20) for b in m["bins"])
    check("mc_stat_floor", ok, "unmasked bins: raw gen min %d, element min %d" % (min([int(key[(b["charge"], b["hemi"], b["beta_bin"], b["axis"], b["bin"])]["n_raw_gen_840x"]) for b in m["bins"] if not b["masked"]]),
                                                                      min([int(key[(b["charge"], b["hemi"], b["beta_bin"], b["axis"], b["bin"])]["n_raw_element_min"]) for b in m["bins"] if not b["masked"]])))
    V = _csv("veto_syst_c1.csv"); hi = [r for r in V if r["masked_veto"] == "True"]
    inmask = all("veto_syst" in next(b for b in m["bins"] if b["charge"] == r["charge"] and b["hemi"] == "GP"[C.CELLS.index(r["cell_name"]) // 2] and b["beta_bin"] == C.CELLS.index(r["cell_name"]) % 2
                                      and b["axis"] == r["axis"] and b["bin"] == int(r["bin"]))["reasons"] for r in hi)
    check("veto_syst_table", len(V) == len(m["bins"]) and inmask, "%d rows; sigma_rel > 0.05: %d" % (len(V), len(hi)))
    P = _csv("precision_c1.csv"); nd = [float(r["delta_rel"]) for r in P if r["delta_rel"] not in ("nan", "")]
    check("precision_table", len(P) == len(m["bins"]) and len(nd) > 0 and m["data_files_opened"] == [], "delta_rel %.4f..%.4f over %d bins" % (min(nd), max(nd), len(nd)))
    figs = glob.glob(str(C.D / "plots/restart_2026-10-27/wp-c/c1/*.png")); cap = (C.D / "plots/restart_2026-10-27/wp-c/c1/captions.md").exists()
    check("plots_complete", 0 < len(figs) <= 6 and cap, "%d figures, captions %s" % (len(figs), cap))


def slice4():
    rec = DOC / "c1/grid_freeze_record.md"; stub = DOC / "user_grid_record.md"; t = rec.read_text() if rec.exists() else ""
    need = ["Definitions", "Frozen edges", "Per-cell grid", "D7+", "Mask", "Flip fractions", "beta-bin", "Resolution", "Cross-checks", "Statistics", "Re-fill", "Open items"]
    miss = [k for k in need if k.lower() not in t.lower()]
    mk = sorted(glob.glob(str(C.W / "mask_c1_*_standin.json.sha256")))
    sha = open(mk[-1]).read().split()[0] if mk else "?"
    ok = not miss and sha in t and all(str(C.PT2_EDGES[q][1]) in t for q in C.GRIDS) and stub.exists() and "grid_freeze_record.md" in stub.read_text() and len(t.splitlines()) <= 150
    check("record_complete", ok, "missing %s, lines %d, mask sha quoted %s" % (miss, len(t.splitlines()), sha in t))


if __name__ == "__main__":
    w = sys.argv[1]
    for s, f in (("slice1", slice1), ("slice2", slice2), ("slice3", slice3), ("slice4", slice4)):
        if w in (s, "all"): f()
    print("TESTS: %d passed, %d failed" % (sum(ok for _, ok in RES), sum(not ok for _, ok in RES)))
    sys.exit(0 if all(ok for _, ok in RES) else 1)

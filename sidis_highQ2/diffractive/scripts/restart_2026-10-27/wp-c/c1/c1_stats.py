#!/usr/bin/env python3
"""WP-C C1 slice 3: bootstraps by generated event (stream, EventNumber), data precision per bin, WP-A5 veto systematic per bin on the
frozen grid (C1-16).  Writes boot_c1.json, precision_c1.csv, veto_syst_c1.csv, and the final provisional mask.  No data file opened."""
import csv, datetime, json, sys
from pathlib import Path
import numpy as np
sys.path.insert(0, str(Path(__file__).resolve().parent))
import c1_lib as C
import c1_tables as TB
import c1_mask as MK

NBOOT, SEED = 200, 20261028
VARIANTS = ("nominal", "up", "down", "eta", "eta_up", "eta_down")


def frozen_bins(grid):
    """[(axis, gname, cell, k)] for the frozen grid."""
    out = []
    for ci, cn in enumerate(C.CELLS):
        g = grid["cells"][cn]["pt2_grid"]
        out += [("z", "q5", ci, k) for k in range(2)] + [("pt2", g, ci, k) for k in range(C.nbins("pt2", g))]
    return out


def unit_index(ev):
    key = (ev["stream"].astype(np.int64) << 40) | ev["event"].astype(np.int64)
    u, inv = np.unique(key, return_inverse=True)
    return inv, len(u)


def main():
    np.seterr(all="ignore")
    grid = json.load(open(C.W / "grid_c1.json")); fb = frozen_bins(grid); C.TOP_SPLIT.update(grid["reco_subbin_top_split"])
    t, ev, T, E = TB.build_mc(); tpl, _ = TB.build_templates(ev)
    binstats = {(r["charge"], r["cell_name"], r["axis"], int(r["bin"])): r for r in csv.DictReader(open(C.W / "binstats_c1_A.csv"))
                if r["axis"] == "z" or r["grid"] == grid["cells"][r["cell_name"]]["pt2_grid"]}
    # ---- veto systematic
    wv = {v: ev["w0"] * C.VW.weights(E["e_mx"], ev["cell"], ev["ec"], v) for v in VARIANTS}
    rb = C.beta_bin(ev["beta"]); kev = ev["keep"] & (rb >= 0)
    cr = C.cell(t["A"]["hemi"], t["bbin_r"]); veto = []
    names = {}
    for ch in ("h+", "h-"):
        base = t["keep"] & ((t["chg"] > 0) if ch == "h+" else (t["chg"] < 0))
        for axis, gname in sorted({(a, g) for a, g, _, _ in fb}):
            nb = C.nbins(axis, gname); NF = 4 * nb
            kr = np.where(base, C.bin_index(cr, axis, gname, t["A"]["pt2" if axis == "pt2" else "zs"]), -1)
            S = {}
            for v in VARIANTS:
                den = np.array([wv[v][kev & (rb == b)].sum() for b in (0, 1)])
                num = TB.hist(kr, wv[v][t["g"]], np.ones(len(kr), bool), NF)
                S[v] = num / den[(np.arange(NF) // nb) % 2]
            with np.errstate(all="ignore"):
                R = {v: S[v] / S["nominal"] - 1 for v in VARIANTS[1:]}
                sr = np.sqrt(np.maximum(np.abs(R["up"]), np.abs(R["down"])) ** 2 + R["eta"] ** 2)
                ea = np.maximum(np.abs(S["eta_up"] / S["eta"] - 1), np.abs(S["eta_down"] / S["eta"] - 1)); se = np.sqrt(ea ** 2 + (R["eta"] / 2) ** 2)
            for (a, g, ci, k) in fb:
                if (a, g) != (axis, gname): continue
                j = ci * nb + k
                veto.append(dict(charge=ch, cell_name=C.CELLS[ci], axis=axis, bin=k, grid=("z" if axis == "z" else gname), S_nom=S["nominal"][j], R_up=R["up"][j],
                                 R_down=R["down"][j], R_eta=R["eta"][j], sigma_rel=sr[j], sigma_rel_etaform=se[j], masked_veto=bool(not sr[j] <= C.THR["veto"])))
    with open(C.W / "veto_syst_c1.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(veto[0])); wr.writeheader()
        for r in veto: wr.writerow({a: ("%.6g" % v if isinstance(v, float) else v) for a, v in r.items()})
    # ---- precision per bin (UG frame-A n_data, no new read)
    prec = []
    for (ch, cn, ax, k), r in sorted(binstats.items()):
        n = r["n_data"]
        prec.append(dict(charge=ch, cell_name=cn, axis=ax, grid=r["grid"], bin=k, n_data=n, delta_rel=(1 / np.sqrt(float(n)) if n not in ("", "0") else float("nan")),
                         n_raw_gen_840x=r["n_raw_gen_840x"], n_raw_840x_reco=r["n_raw_840x_reco"]))
    with open(C.W / "precision_c1.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=list(prec[0])); wr.writeheader()
        for r in prec: wr.writerow({a: ("%.6g" % v if isinstance(v, float) else v) for a, v in r.items()})
    # ---- bootstrap: Poisson(1) per generated event (stream, EventNumber); twins share a draw
    inv, nu = unit_index(ev); rng = np.random.default_rng(SEED)
    keys = sorted({(a, g) for a, g, _, _ in fb}); acc = {}
    for _ in range(NBOOT):
        pw = rng.poisson(1.0, size=nu).astype(float)[inv]; wb = t["w"] * pw[t["g"]]
        for ch in ("h+", "h-"):
            for axis, gname in keys:
                o = TB.evaluate(t, tpl, "A", ch, axis, gname, w=wb, fast=True)
                nb = C.nbins(axis, gname)
                for (a, g, ci, k) in fb:
                    if (a, g) == (axis, gname):
                        j = ci * nb + k; acc.setdefault((ch, C.CELLS[ci], axis, k), []).append((o["pur"][j], o["stab"][j]))
    boot = []
    for (ch, cn, ax, k), v in sorted(acc.items()):
        v = np.array(v); boot.append(dict(charge=ch, cell_name=cn, axis=ax, bin=k, sigma_purity=float(np.nanstd(v[:, 0], ddof=1)),
                                          sigma_stability=float(np.nanstd(v[:, 1], ddof=1)), n_valid=int(np.isfinite(v[:, 0]).sum())))
    json.dump(dict(nboot=NBOOT, seed=SEED, unit="(stream, EventNumber); stream 0 = 8402+8405, 1 = 8403+8404, 10+label else; n_units=%d of %d events" % (nu, len(inv)),
                   templates_fixed=True, rows=boot, created_utc=datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")),
              open(C.W / "boot_c1.json", "w"), indent=1)
    out = MK.main(["--veto", str(C.W / "veto_syst_c1.csv")])
    nv = sum(r["masked_veto"] for r in veto)
    print("veto rows", len(veto), "veto-masked", nv, "bootstrap rows", len(boot), "mask", out)


if __name__ == "__main__":
    main()

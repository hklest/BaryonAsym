#!/usr/bin/env python3
"""WP-A6 S3a: f_ND per M_X bin, resolved scales, stability table (Decisions 3, 6, 7, 10 of the child plan).
Reads wp-a6 payloads (`sel`, `cluster`) and fit JSONs; login-node, JSON/CSV only. Usage: fnd_wpa6.py --split {even,odd}"""
import argparse, csv, json, sys
from pathlib import Path
import numpy as np
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
sys.path.insert(0, str(D / "scripts/restart_2026-10-27/wp-a")); import wpa_common as W
O = D / "data/restart_2026-10-27/wp-a6"
CELLS, MX, CLS = ["D0", "D1", "D2"], ["MX4_10", "MX10_20", "MX20_40"], ["MX4_10", "MX10_20", "MX20_40", "MXout"]
CHAINS, TAGS, CEIL, NIN = ["7071", "7091", "nd4cat"], ["nominal", "hi2p8", "lo1p0", "xip04", "nobb", "shared"], 0.30, 80
REG = ["6671", "6677"]
COLS = "split mx_bin chain f_nd f_nd_infit f_nd_band sig_fit spread sig_g2 delta_edge f_reg n_data_sel largest_chain above_ceiling masked_G2 flip_with_edge g2_plus_sigma_flag n_fallback_cells".split()
RES = "split tag chain scope mx_class s_diff sd_sig s_ND sn_sig source unit_ok n_data_fit n_nd_raw_fit fit_file".split()
FB = "chain tag cell mx_class n_data_fit n_nd_raw_fit s_diff_used s_ND_used source".split()
UT = "chain scope mx_class s_diff sd_minus sd_plus s_ND sn_minus sn_plus deviance ndf dev_per_ndf p_asym toy_p zero_mc_n_bins deviance_excl_zero_mc ndf_excl unit_ok source_used".split()
ED = "chain cell mx_class lo hi data prediction ratio used_for_rho".split()
ST = "tag mx_bin chain f_nd f_nd_nominal shift sig_g2_nominal flag_shift_gt_sig".split()

def arr(t, c, m): return np.asarray(t[c][m], float)

def counts(split, chain):
    """Per (cell, class) SEL and edge counts from the payload at delivered weights (exposure1*fac, sumw2*fac^2)."""
    p = json.load(open(O / f"payloads/{split}_{chain}.json")); fac = p["scale_table_basis"]["exposure_factors"]
    nd, S, cl = W.CHAINS[chain]["nd"], p["sel"], p["modes"]["cluster"]
    def part(blk, lab, kind, c, m):
        t = arr(blk["components"][lab]["templates"][kind], c, m); o = blk["components"][lab]["oor"][kind][c][m]
        return t[:NIN].sum(), t[NIN:].sum() + o
    N = {}
    for c in CELLS:
        for m in CLS:
            d = arr(S["data"]["templates"]["raw"], c, m).sum() + S["data"]["oor"][c][m]
            r = dict(n_data=d, nd_in=0., nd_band=0., w2_band=0., reg=0.)
            for lab in nd:
                a, b = part(S, lab, "exposure1", c, m); r["nd_in"] += a * fac[lab]; r["nd_band"] += b * fac[lab]
                r["w2_band"] += part(S, lab, "sumw2", c, m)[1] * fac[lab] ** 2
            for lab in REG:
                a, b = part(S, lab, "exposure1", c, m); r["reg"] += (a + b) * fac[lab]
            ed = lambda lab: arr(cl["components"][lab]["templates"]["exposure1"], c, m)[NIN:NIN + 8] * fac[lab]
            r["edge_data"] = arr(cl["data"]["templates"]["raw"], c, m)[NIN:NIN + 8]
            r["edge_D"] = sum(ed(l) for l in W.DIFF8); r["edge_N"] = sum(ed(l) for l in nd)
            N[c, m] = r
    return N, np.asarray(p["edges"], float)

def loadfit(split, tag, chain, scope, cls):
    f = O / f"fits/{split}/{'nominal' if tag == 'shared' else tag}/fit_{chain}_{scope}_{cls}.json"
    return (json.load(open(f)), f) if f.exists() else (None, f)

def sig(pr):
    v = [x for x in (pr.get("minus"), pr.get("plus")) if x is not None]; return max(v) if v else float("nan")

def resolve(split, tag, chain, scope, cls):
    """Decision 3: unit fit if unit_ok, else the same scope's `all` fit; tag shared: always `all`."""
    u, fu = loadfit(split, tag, chain, scope, cls) if tag != "shared" else loadfit(split, tag, chain, scope, "all")
    if u is None: return None
    a = u
    if tag == "shared": src = "shared_all"
    elif u["unit_ok"]: src = "unit"
    else:
        a, fu = loadfit(split, tag, chain, scope, "all"); src = "fallback_all"
        if a is None: return None
    return dict(split=split, tag=tag, chain=chain, scope=scope, mx_class=cls, s_diff=a["s_diffractive"], sd_sig=sig(a["profile_s_diffractive"]),
                s_ND=a["s_ND"], sn_sig=sig(a["profile_s_ND"]), source=src, unit_ok=u["unit_ok"], n_data_fit=u["n_data_fit"],
                n_nd_raw_fit=u["n_nd_raw_fit"], fit_file=str(fu.relative_to(D)))

def resolve_all(split, tag, chain, scopes=CELLS):
    R = {(s, c): resolve(split, tag, chain, s, c) for s in scopes for c in CLS}
    return None if any(v is None for v in R.values()) else R

def fnd(N, R, m):
    nd = sum(N[c, m]["n_data"] for c in CELLS); g = lambda c: R[c, m]
    fi = sum(g(c)["s_ND"] * N[c, m]["nd_in"] for c in CELLS) / nd; fb = sum(g(c)["s_ND"] * N[c, m]["nd_band"] for c in CELLS) / nd
    f = fi + fb; v = sum((g(c)["sn_sig"] * (N[c, m]["nd_in"] + N[c, m]["nd_band"])) ** 2 + g(c)["s_ND"] ** 2 * N[c, m]["w2_band"] for c in CELLS) / nd ** 2 + f * f / nd
    freg = sum(g(c)["s_diff"] * N[c, m]["reg"] for c in CELLS) / nd
    return dict(f_nd=f, f_nd_infit=fi, f_nd_band=fb, sig_fit=v ** .5, f_reg=freg, n_data_sel=nd)

def edge(N, R, edges, chain, m, c, rows):
    """rho: max |data/pred - 1| over the 8 [3.0,3.4) bins with >= 20 data (pred with the resolved scales); fallback to the class-summed histogram."""
    n, g = N[c, m], R[c, m]; pred = g["s_diff"] * n["edge_D"] + g["s_ND"] * n["edge_N"]; dat = n["edge_data"]
    use = (dat >= 20) & (pred > 0)
    if use.any(): rho = float(np.max(np.abs(dat[use] / pred[use] - 1)))
    else:
        rho = float(abs(dat.sum() / pred.sum() - 1)) if pred.sum() > 0 else 0.0; use[:] = True
        print(f"LOG rho {chain} {c} {m}: no bin with >= 20 data, class-summed 8-bin histogram used, rho={rho:.4f}")
    for i in range(8):
        rows.append(dict(chain=chain, cell=c, mx_class=m, lo=edges[NIN + i], hi=edges[NIN + i + 1], data=dat[i], prediction=pred[i],
                         ratio=dat[i] / pred[i] if pred[i] > 0 else "", used_for_rho=int(use[i])))
    return rho

def wcsv(path, cols, rows):
    with open(path, "w", newline="") as f:
        w = csv.DictWriter(f, cols, extrasaction="ignore"); w.writeheader(); w.writerows(rows)

def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--split", choices=["even", "odd"], required=True); sp = ap.parse_args().split
    C = {ch: counts(sp, ch) for ch in CHAINS}; res, fb, edrows, F, utab = [], [], [], {}, []
    tags = [t for t in TAGS if (O / f"fits/{sp}/{'nominal' if t == 'shared' else t}").is_dir()]
    for tag in tags:
        for ch in CHAINS:
            R = resolve_all(sp, tag, ch, CELLS + (["ALL"] if tag == "nominal" else []))
            if R is None: print(f"LOG missing fits for {sp} {tag} {ch}; skipped"); continue
            res += list(R.values()); fb += [r for r in R.values() if r["source"] == "fallback_all"]
            for m in MX:
                N, edges = C[ch]
                F[tag, ch, m] = dict(fnd(N, R, m), nfb=sum(R[c, m]["source"] == "fallback_all" for c in CELLS))
                if tag == "nominal": F[tag, ch, m]["delta_edge"] = sum(N[c, m]["nd_band"] * R[c, m]["s_ND"] / F[tag, ch, m]["n_data_sel"] * edge(N, R, edges, ch, m, c, edrows) for c in CELLS)
    for tag in tags:
        rr = [r for r in res if r["tag"] == tag]; wcsv(O / f"scales_resolved_{sp}_{tag}.csv", RES, rr)
    wcsv(O / f"fallback_{sp}.csv", FB, [dict(chain=r["chain"], tag=r["tag"], cell=r["scope"], mx_class=r["mx_class"], n_data_fit=r["n_data_fit"], n_nd_raw_fit=r["n_nd_raw_fit"],
         s_diff_used=r["s_diff"], s_ND_used=r["s_ND"], source=r["source"]) for r in fb])
    for r in fb: print(f"LOG fallback {r['chain']} {r['tag']} {r['scope']} {r['mx_class']}: n_data={r['n_data_fit']:.0f} n_nd_raw={r['n_nd_raw_fit']:.0f} -> all fit")
    wcsv(O / f"edge_diag_{sp}.csv", ED, edrows)
    rows = []
    for m in MX:
        ch3 = [ch for ch in CHAINS if ("nominal", ch, m) in F]
        if "7071" not in ch3: continue
        spread = max([abs(F["nominal", ch, m]["f_nd"] - F["nominal", "7071", m]["f_nd"]) for ch in ch3] or [0.0]); big = max(ch3, key=lambda ch: F["nominal", ch, m]["f_nd"])
        for ch in ch3:
            x = F["nominal", ch, m]; g2 = (x["sig_fit"] ** 2 + spread ** 2) ** .5; e = x["delta_edge"]
            rows.append(dict(x, split=sp, mx_bin=m, chain=ch, spread=spread, sig_g2=g2, delta_edge=e, largest_chain=big, above_ceiling=x["f_nd"] > CEIL,
                             masked_G2=F["nominal", big, m]["f_nd"] > CEIL, flip_with_edge=(x["f_nd"] <= CEIL) != (x["f_nd"] + e <= CEIL),
                             g2_plus_sigma_flag=x["f_nd"] + g2 > CEIL, n_fallback_cells=x["nfb"]))
    old = [r for r in csv.DictReader(open(O / "f_nd_per_mx.csv")) if r["split"] != sp] if (O / "f_nd_per_mx.csv").exists() else []
    wcsv(O / "f_nd_per_mx.csv", COLS, sorted(old + rows, key=lambda r: (r["split"], r["mx_bin"], r["chain"])))
    if sp == "even":
        st = [dict(tag=t, mx_bin=r["mx_bin"], chain=r["chain"], f_nd=F[t, r["chain"], r["mx_bin"]]["f_nd"], f_nd_nominal=r["f_nd"], shift=F[t, r["chain"], r["mx_bin"]]["f_nd"] - r["f_nd"],
                   sig_g2_nominal=r["sig_g2"], flag_shift_gt_sig=abs(F[t, r["chain"], r["mx_bin"]]["f_nd"] - r["f_nd"]) > r["sig_g2"])
              for t in tags if t != "nominal" for r in rows if (t, r["chain"], r["mx_bin"]) in F]
        wcsv(O / "stability_even.csv", ST, st)
    for ch in CHAINS:
        for scope in CELLS + ["ALL"]:
            for cls in CLS + ["all"]:
                u, _ = loadfit(sp, "nominal", ch, scope, cls)
                if u is None: continue
                pd_, pn = u["profile_s_diffractive"], u["profile_s_ND"]; z = u.get("zero_mc_bins") or {}
                src = "unit" if u["unit_ok"] or cls == "all" else "fallback_all"
                utab.append(dict(chain=ch, scope=scope, mx_class=cls, s_diff=u["s_diffractive"], sd_minus=pd_.get("minus"), sd_plus=pd_.get("plus"), s_ND=u["s_ND"], sn_minus=pn.get("minus"), sn_plus=pn.get("plus"),
                                 deviance=u["deviance"], ndf=u["ndf"], dev_per_ndf=u["deviance"] / u["ndf"] if u["ndf"] else "", p_asym=u["p_asym"], toy_p=(u.get("toy_gof") or {}).get("toy_p_ge_observed"),
                                 zero_mc_n_bins=z.get("n_bins"), deviance_excl_zero_mc=z.get("deviance_excl"), ndf_excl=z.get("ndf_excl"), unit_ok=u["unit_ok"], source_used=src))
    wcsv(O / f"unit_table_{sp}.csv", UT, utab)
    for r in rows: print(f"{r['mx_bin']:8s} {r['chain']:7s} f_nd={r['f_nd']:.4f} in={r['f_nd_infit']:.4f} band={r['f_nd_band']:.4f} sig_fit={r['sig_fit']:.4f} spread={r['spread']:.4f} dEdge={r['delta_edge']:.4f} f_reg={r['f_reg']:.4f} flip={r['flip_with_edge']} masked={r['masked_G2']}")

if __name__ == "__main__": main()

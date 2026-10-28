#!/usr/bin/env python3
"""WP-A6 Barlow-Beeston-lite two-template fitter (child plan 2026-10-28-wp-a6-leakage-refit.md D1-D3, D10).
Fork of wp-a2/fit_7p1_wpa2.py (dterms, fit multistart, prof_interval, toy_gof); one fit per (chain, scope, M_X class).
BB-lite per bin: mu=s_d*D+s_n*N, sigma^2=s_d^2*W_D+s_n^2*W_N, tau=mu^2/sigma^2; beta solves tau*beta^2+(mu-tau)*beta-y=0
(positive root, profiled); term = 2[beta*mu-y+y*ln(y/(beta*mu))] + tau*(beta-1)^2; sigma=0 -> beta=1 (plain Poisson)."""
import argparse, csv, hashlib, json, math, subprocess, sys
from datetime import datetime, timezone
from pathlib import Path
import numpy as np
from scipy.optimize import minimize, minimize_scalar
from scipy.special import gammaincc

HERE = Path(__file__).resolve().parent
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
sys.path.insert(0, str(D / "scripts/restart_2026-10-27/wp-a"))
import wpa_common as W  # noqa: E402
CELLS = ("D0", "D1", "D2"); MXC = ("MX4_10", "MX10_20", "MX20_40", "MXout"); CLASSES = MXC + ("all",)
WD = D / "data/restart_2026-10-27/wp-a6"; CND = D / "condor/restart_2026-10-27/wp-a6/fit"
THR = {"data_fit_min": 100, "nd_raw_fit_min": 100}
FALLBACK = ("Decision 3: s_diff and s_ND free per (cell, M_X class) where FIT data in [-1,3.0) >= 100 and ND raw there >= 100; "
            "otherwise the unit takes the per-cell all-M_X scales.")
BB_SPEC = ("per-bin beta_b scaling mu_b=s_d*D_b+s_n*N_b, Gaussian-constrained with sigma_b^2=s_d^2*sumw2_D,b+s_n^2*sumw2_N,b "
           "(delivered sumw2 = payload sumw2*fac^2), profiled analytically; Poisson deviance + tau*(beta-1)^2.")

def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def _bb(y, d, n, wd, wn, q, grad=False):
    """Profiled BB-lite deviance terms and beta (and the envelope-theorem gradient wrt (s_d, s_n))."""
    y = np.asarray(y, float); sd, sn = float(q[0]), float(q[1])
    mu0 = sd * d + sn * n; mu = np.maximum(mu0, 1e-12); s2 = sd * sd * wd + sn * sn * wn; pos = s2 > 0
    tau = np.where(pos, mu * mu / np.where(pos, s2, 1.0), 1.0); b = mu - tau; r = np.sqrt(b * b + 4 * tau * y)
    beta = np.where(b > 0, 2 * y / np.where(b > 0, b + r, 1.0), (-b + r) / np.where(b > 0, 1.0, 2 * tau)); beta = np.where(pos, beta, 1.0)
    bm = beta * mu; ylog = np.zeros_like(y); k = y > 0; ylog[k] = y[k] * np.log(y[k] / bm[k])
    t = 2 * (bm - y) + 2 * ylog + np.where(pos, tau * (beta - 1) ** 2, 0.0)
    if not grad: return t, beta
    dm = (d, n); ws = (sd * wd, sn * wn); dTm = 2 * (beta - y / mu); dTt = np.where(pos, (beta - 1) ** 2, 0.0)
    g = np.array([np.sum(dTm * dm[i] + dTt * np.where(pos, tau * (2 * dm[i] / mu - 2 * ws[i] / np.where(pos, s2, 1.0)), 0.0)) for i in (0, 1)])
    return float(t.sum()), g

def dev_bb(y, d, n, sw2_d, sw2_n, q): return float(_bb(y, d, n, sw2_d, sw2_n, q)[0].sum())
def dev_bb_terms(y, d, n, sw2_d, sw2_n, q): return _bb(y, d, n, sw2_d, sw2_n, q)

def _newton(fg, q):
    """Newton polish on the free coordinates (finite-difference Hessian of the analytic gradient), backtracking, q>=0."""
    q = np.array(q, float)
    for _ in range(40):
        f0, g = fg(q); fr = [i for i in (0, 1) if q[i] > 1e-12 or g[i] < 0]
        if not fr: break
        H = np.zeros((len(fr), len(fr)))
        for a, i in enumerate(fr):
            h = 1e-6 * max(1.0, q[i]); qp = q.copy(); qm = q.copy(); qp[i] += h; qm[i] = max(qm[i] - h, 0); H[:, a] = (fg(qp)[1][fr] - fg(qm)[1][fr]) / (qp[i] - qm[i])
        try: st = np.linalg.solve(H, -g[fr])
        except np.linalg.LinAlgError: break
        lam = 1.0; ok = False
        while lam > 1e-6:
            qn = q.copy(); qn[fr] = np.maximum(q[fr] + lam * st, 0)
            if fg(qn)[0] <= f0 + 1e-13 * abs(f0): ok = True; break
            lam /= 2
        if not ok: break
        done = np.max(np.abs(qn - q) / np.maximum(np.abs(q), 1e-3)) < 1e-14; q = qn
        if done: break
    return q

def fit(y, d, n, wd, wn, polish=True):
    """Non-negative two-template profiled-BB-lite MLE; WP-A2 multistart, analytic gradient, then Newton polish."""
    fg = lambda q: _bb(y, d, n, wd, wn, q, grad=True)
    sy, sd, sn = float(y.sum()), float(d.sum()), float(n.sum())
    ss = [[1, 1], [sy / sd if sd else 1, 0], [0, sy / sn if sn else 1], [sy / (2 * sd) if sd else 1, sy / (2 * sn) if sn else 1]]
    best = None
    for s in ss:
        r = minimize(fg, np.maximum(s, 0), jac=True, method="L-BFGS-B", bounds=((0, None), (0, None)),
                     options={"ftol": 1e-12, "gtol": 1e-10, "maxiter": 5000})
        if best is None or r.fun < best.fun: best = r
    q = np.maximum(best.x, 0)
    if polish:
        r = minimize(fg, q, jac=True, method="L-BFGS-B", bounds=((0, None), (0, None)), options={"ftol": 1e-15, "gtol": 1e-12, "maxiter": 5000})
        if r.fun <= fg(q)[0]: q = np.maximum(r.x, 0)
        q = _newton(fg, q)
    return q, float(fg(q)[0])

def prof_interval(devf, q, j):
    """Delta-D=1 profile interval for q[j] (other scale profiled); devf(s_d, s_n) -> profiled deviance."""
    other = 1 - j; minimum = devf(*q); target = minimum + 1
    f = lambda v, u: devf(v, u) if j == 0 else devf(u, v)
    def p(v):
        u = minimize_scalar(lambda x: f(v, x), bounds=(0, max(1, 3 * q[other] + 3)), method="bounded", options={"xatol": 1e-10}).x; hi = max(1, 3 * u + 3)
        while f(v, hi) < f(v, u) and hi < 1e7: hi *= 2
        rr = minimize_scalar(lambda x: f(v, x), bounds=(0, hi), method="bounded", options={"xatol": 1e-10}); return float(rr.fun)
    center = float(q[j]); lo = 0.0
    if center > 0 and p(0) > target:
        a, b = 0., center
        for _ in range(80):
            m = (a + b) / 2
            if p(m) > target: a = m
            else: b = m
        lo = (a + b) / 2
    a = center; b = max(1, 2 * center + 1)
    while p(b) < target and b < 1e7: b *= 2
    if p(b) >= target:
        for _ in range(80):
            m = (a + b) / 2
            if p(m) > target: b = m
            else: a = m
        hi = (a + b) / 2
    else: hi = None
    return {"minus": center - lo, "plus": None if hi is None else hi - center, "lower": lo, "upper": hi, "profile_at_zero": p(0) - minimum, "interior_minimum": bool(center > 1e-7)}

def toy_gof(y, comps, nd_labels, q, observed, seed, bb=True, n_toys=300):
    """comps = [(label, raw, delivered weighted, delivered sumw2)]; Poisson raw per component, weighted/raw and sumw2/raw fixed."""
    rng = np.random.default_rng(seed); vals = []; z = np.zeros_like(y); inv = lambda a, r: np.divide(a, r, out=np.zeros_like(a), where=r > 0)
    d0 = sum((w for l, r, w, s in comps if l not in nd_labels), z); n0 = sum((w for l, r, w, s in comps if l in nd_labels), z)
    for _ in range(n_toys):
        dd, nn, sd2, sn2 = z.copy(), z.copy(), z.copy(), z.copy()
        for lab, raw, wgt, s2 in comps:
            k = rng.poisson(raw); u = k * inv(wgt, raw); v = k * inv(s2, raw) if bb else z
            if lab in nd_labels: nn += u; sn2 += v
            else: dd += u; sd2 += v
        yy = rng.poisson(np.maximum(q[0] * d0 + q[1] * n0, 0)); vals.append(fit(yy, dd, nn, sd2, sn2, polish=False)[1])
    vals = np.asarray(vals)
    return {"method": "parametric bootstrap: Poisson DATA and per-bin raw-MC counts per component; weighted/raw and sumw2/raw fixed; both scales refit with BB-lite" if bb else "parametric bootstrap, no BB", "n_toys": n_toys,
            "observed_deviance": observed, "toy_median": float(np.median(vals)), "toy_p_ge_observed": float((np.count_nonzero(vals >= observed) + 1) / (n_toys + 1)), "p_floor": 1.0 / (n_toys + 1)}

def seed_of(*key): return int(hashlib.sha256("|".join(map(str, key)).encode()).hexdigest()[:8], 16)

def assemble(x, mode, scope, mxc, chain):
    """Full-grid arrays for one unit: scope cells concatenated, classes summed. Returns dict."""
    m = x["modes"][mode]; fac = x["scale_table_basis"]["exposure_factors"]; nd = W.CHAINS[chain]["nd"]
    cells = CELLS if scope == "ALL" else (scope,); cls = MXC if mxc == "all" else (mxc,)
    cat = lambda getter: np.concatenate([sum((np.asarray(getter(c, k), float) for k in cls), np.zeros(len(x["edges"]) - 1)) for c in cells])
    comps = []
    for lab in W.DIFF8 + nd:
        t = m["components"][lab]["templates"]; f = fac[lab]
        comps.append((lab, cat(lambda c, k: t["raw"][c][k]), cat(lambda c, k: t["exposure1"][c][k]) * f, cat(lambda c, k: t["sumw2"][c][k]) * f * f))
    z = np.zeros_like(comps[0][1]); isd = lambda l: l not in nd
    return {"y": cat(lambda c, k: m["data"]["templates"]["raw"][c][k]), "comps": comps, "nd": nd, "fac": {l: fac[l] for l in W.DIFF8 + nd},
            "D": sum((c[2] for c in comps if isd(c[0])), z), "N": sum((c[2] for c in comps if not isd(c[0])), z),
            "WD": sum((c[3] for c in comps if isd(c[0])), z), "WN": sum((c[3] for c in comps if not isd(c[0])), z),
            "nd_raw": sum((c[1] for c in comps if not isd(c[0])), z), "ncell": len(cells)}

def fit_unit(args):
    P = Path(args.payload); x = json.load(open(P)); assert x["pass"] and x["conservation"]["all_pass"]
    mode = "cluster_xip04" if args.xip_cut == "0.04" else "cluster"; bb = not args.no_bb
    edges = np.asarray(x["edges"], float); NB = len(edges) - 1; lo, hi = edges[:-1], edges[1:]
    sel1 = (lo >= args.eta_lo - 1e-9) & (hi <= args.eta_hi + 1e-9)
    if args.eta_lo == -1 and args.eta_hi == 3.0: assert sel1.sum() == 80 and np.allclose(hi[sel1] - lo[sel1], 0.05), "nominal slice must be 80 bins of 0.05"
    A = assemble(x, mode, args.scope, args.mx_class, args.chain); sel = np.tile(sel1, A["ncell"])
    y, d, n = A["y"][sel], A["D"][sel], A["N"][sel]; wd, wn = (A["WD"][sel], A["WN"][sel]) if bb else (np.zeros(sel.sum()),) * 2
    q, dv = fit(y, d, n, wd, wn); ndf = int(sel.sum()) - 2
    devf = lambda a, b: dev_bb(y, d, n, wd, wn, [a, b]); pi = [prof_interval(devf, q, j) for j in (0, 1)]
    terms, beta = _bb(y, d, n, wd, wn, q); pred = q[0] * d + q[1] * n
    ndr = float(A["nd_raw"][sel].sum()); ndat = float(y.sum())
    rec = {"schema": "wpa6-fit-v1", "argv": sys.argv, "payload": str(P), "payload_sha256": sha(P), "script_sha256": sha(__file__), "chain": args.chain, "split": x["split"][0] if isinstance(x["split"], list) else x["split"],
           "scope": args.scope, "mx_class": args.mx_class, "mode": mode, "tag": args.tag, "eta_lo": args.eta_lo, "eta_hi": args.eta_hi, "bb": bb, "n_bins": int(sel1.sum()),
           "edges": np.append(lo[sel1], hi[sel1][-1]).tolist(), "s_diffractive": float(q[0]), "s_ND": float(q[1]), "profile_s_diffractive": pi[0], "profile_s_ND": pi[1],
           "deviance": dv, "ndf": ndf, "p_asym": float(gammaincc(.5 * ndf, .5 * dv)), "data": y.tolist(), "prediction": pred.tolist(), "beta": beta.tolist(), "deviance_contribution": terms.tolist(),
           "n_data_fit": ndat, "n_nd_raw_fit": ndr, "unit_ok": bool(ndat >= THR["data_fit_min"] and ndr >= THR["nd_raw_fit_min"]), "fac_used": A["fac"]}
    zm = (d + n <= 0) & (y > 0)   # data in bins with no MC at all: constant deviance (mu floored), drives the GOF, not the scales
    rec["zero_mc_bins"] = {"n_bins": int(zm.sum()), "data": float(y[zm].sum()), "deviance": float(terms[zm].sum()),
                           "deviance_excl": float(dv - terms[zm].sum()), "ndf_excl": int(ndf - zm.sum())}
    seed = seed_of(args.chain, rec["split"], args.scope, args.mx_class, args.tag); toy_p = ""
    if args.n_toys > 0:
        comps = [(l, r[sel], w[sel], s[sel]) for l, r, w, s in A["comps"]]
        rec["toy_gof"] = toy_gof(y, comps, A["nd"], q, dv, seed, bb=bb, n_toys=args.n_toys); rec["toy_gof"]["seed"] = seed; toy_p = rec["toy_gof"]["toy_p_ge_observed"]
    ed = np.tile((lo >= 3.0 - 1e-9) & (lo < 3.4 - 1e-9), A["ncell"]); blk = np.repeat(np.arange(A["ncell"]), NB); ix = np.flatnonzero(ed)
    P_ = q[0] * A["D"] + q[1] * A["N"]; mc2 = q[0] ** 2 * A["WD"] + q[1] ** 2 * A["WN"]
    rec["edge_diagnostic"] = [{"block": int(blk[i]), "lo": float(lo[i % NB]), "hi": float(hi[i % NB]), "data": float(A["y"][i]), "prediction": float(P_[i]),
                               "ratio": float(A["y"][i] / P_[i]) if P_[i] > 0 else None, "mc_sumw2": float(mc2[i])} for i in ix]
    out = Path(args.outdir); out.mkdir(parents=True, exist_ok=True); stem = f"{args.chain}_{args.scope}_{args.mx_class}"
    json.dump(rec, open(out / f"fit_{stem}.json", "w"), indent=1)
    with open(out / f"summary_{stem}.csv", "w", newline="") as f:
        w = csv.writer(f); w.writerow("tag,chain,split,scope,mx_class,mode,eta_lo,eta_hi,bb,s_diffractive,sd_minus,sd_plus,s_ND,sn_minus,sn_plus,deviance,ndf,p_asym,toy_p,n_toys,n_data_fit,n_nd_raw_fit,unit_ok".split(","))
        w.writerow([args.tag, args.chain, rec["split"], args.scope, args.mx_class, mode, args.eta_lo, args.eta_hi, int(bb), q[0], pi[0]["minus"], pi[0]["plus"], q[1], pi[1]["minus"], pi[1]["plus"],
                    dv, ndf, rec["p_asym"], toy_p, args.n_toys, ndat, ndr, int(rec["unit_ok"])])
    print(json.dumps({"unit": stem, "s_d": q[0], "s_n": q[1], "dev": dv, "ndf": ndf, "toy_p": toy_p, "unit_ok": rec["unit_ok"]}))

def queue_rows(a):
    rows = []; cl = ("MX4_10", "MX10_20", "MX20_40", "MXout", "all"); P = lambda ch: f"{a.payload_dir}/{a.split}_{ch}.json"
    def row(ch, sc, mx, lo, hi, xip, bb, tag, nt, rt): rows.append([ch, sc, mx, lo, hi, xip, bb, P(ch), f"{a.outroot}/{a.split}/{tag}", nt, tag, rt])
    for ch in ("7071", "7091", "nd4cat"):
        if a.set == "nominal":
            for sc in ("D0", "D1", "D2", "ALL"):
                for mx in cl: row(ch, sc, mx, -1, 3.0, "none", "bb", "nominal", a.n_toys, 7200)
        else:
            assert a.split == "even", "variants run on the even split only"
            for sc in ("D0", "D1", "D2"):
                for mx in cl:
                    for tag, lo, hi, xip, bb in (("hi2p8", -1, 2.8, "none", "bb"), ("lo1p0", 1.0, 3.0, "none", "bb"), ("xip04", -1, 3.0, "0.04", "bb"), ("nobb", -1, 3.0, "none", "nobb")):
                        row(ch, sc, mx, lo, hi, xip, bb, tag, 0, 1800)
    for r in rows: print(",".join(map(str, r)))

def freeze(a):
    od = Path(a.outroot) / "odd"
    if od.exists() and any(p.is_file() for p in od.rglob("*")): print(f"REFUSE: files exist under {od}", file=sys.stderr); sys.exit(2)
    out = Path(a.out); out.parent.mkdir(parents=True, exist_ok=True)
    files = sorted(HERE.glob("*.py")) + sorted(p for p in CND.glob("*") if p.is_file() and not p.name.startswith(("submit_log", "queue")))
    md5 = lambda p: hashlib.md5(p.read_bytes()).hexdigest()
    rec = {"schema": "wpa6-range-frozen-v1", "edges": np.round(np.linspace(-1, 3.0, 81), 10).tolist(), "eta_lo": -1.0, "eta_hi": 3.0, "merged_bin": False, "mx_classes": list(MXC), "unit_thresholds": THR,
           "fallback_rule": FALLBACK, "bb_spec": BB_SPEC, "toys": {"nominal_n_toys": 1000, "variants": 0, "p": "(count+1)/(n_toys+1)"}, "script_md5": {str(p): md5(p) for p in files}, "utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")}
    out.write_text(json.dumps(rec, indent=1)); Path(str(out) + ".sha256").write_text(f"{sha(out)}  {out}\n"); print("frozen", out)

def main():
    ap = argparse.ArgumentParser(); sp = ap.add_subparsers(dest="cmd", required=True)
    f = sp.add_parser("fit"); f.add_argument("--payload", required=True); f.add_argument("--outdir", required=True); f.add_argument("--chain", required=True, choices=["7071", "7091", "nd4cat"])
    f.add_argument("--scope", required=True, choices=["D0", "D1", "D2", "ALL"]); f.add_argument("--mx-class", required=True, choices=list(CLASSES)); f.add_argument("--eta-lo", type=float, default=-1.0)
    f.add_argument("--eta-hi", type=float, default=3.0); f.add_argument("--xip-cut", default="none", choices=["none", "0.04"]); f.add_argument("--no-bb", action="store_true")
    f.add_argument("--n-toys", type=int, default=0); f.add_argument("--tag", default="nominal")
    q = sp.add_parser("queue"); q.add_argument("--split", required=True, choices=["even", "odd"]); q.add_argument("--set", required=True, choices=["nominal", "variants"])
    q.add_argument("--payload-dir", default=str(WD / "payloads")); q.add_argument("--outroot", default=str(WD / "fits")); q.add_argument("--n-toys", type=int, default=1000)
    z = sp.add_parser("freeze"); z.add_argument("--out", default=str(WD / "range_frozen.json")); z.add_argument("--outroot", default=str(WD / "fits"))
    a = ap.parse_args(); {"fit": fit_unit, "queue": queue_rows, "freeze": freeze}[a.cmd](a)

if __name__ == "__main__": main()

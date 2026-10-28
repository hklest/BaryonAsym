#!/usr/bin/env python3
"""WP-A6 S2 tests of fit_bblite_wpa6.py (login-node, single-file inputs). Usage: tests_s2_wpa6.py [name ...|all]"""
import json, sys, time
from pathlib import Path
import numpy as np
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
sys.path.insert(0, str(Path(__file__).resolve().parent)); sys.path.insert(0, str(D / "scripts/restart_2026-10-27/wp-a2"))
import fit_bblite_wpa6 as B, fit_7p1_wpa2 as A2, wpa_common as W
PL = D / "data/restart_2026-10-27/wp-a2/s4_g1p/payloads/odd_frozen_7071.json"

def arrays(cell):
    """y, D, N, sw2_D, sw2_N cut to hi<=3.0, built as fit_7p1_wpa2.main does; sw2 = exposure1^2/raw*fac^2 proxy (odd_frozen has none)."""
    x = json.load(open(PL)); e = np.asarray(x["edges"], float); k = e[1:] <= 3.0 + 1e-9; c = x["modes"]["cluster"]; fac = x["scale_table_basis"]["exposure_factors"]
    y = np.asarray(c["data"]["templates"]["raw"][cell], float)[k]; z = np.zeros(k.sum()); out = {"D": z.copy(), "N": z.copy(), "WD": z.copy(), "WN": z.copy()}
    for lab in W.DIFF8 + ["7071"]:
        t = c["components"][lab]["templates"]; w1 = np.asarray(t["exposure1"][cell], float)[k]; r = np.asarray(t["raw"][cell], float)[k]
        s2 = np.divide(w1 ** 2, r, out=np.zeros_like(w1), where=r > 0) * fac[lab] ** 2; nd = lab == "7071"
        out["N" if nd else "D"] += w1 * fac[lab]; out["WN" if nd else "WD"] += s2
    return y, out["D"], out["N"], out["WD"], out["WN"]

def pick():
    """First cell with an interior (s_ND > 0) fit, so invariance/profile tests are not at the boundary."""
    for c in B.CELLS:
        y, d, n, wd, wn = arrays(c)
        if B.fit(y, d, n, wd, wn)[0][1] > 1e-3: return c
    return "D0"

def rep(name, ok, msg): print(f"{'PASS' if ok else 'FAIL'} {name}: {msg}"); return ok

def t_reduces():
    worst = 0; ws = 0
    for cell in B.CELLS:
        y, d, n, wd, wn = arrays(cell); z = np.zeros_like(y); q1, d1 = B.fit(y, d, n, z, z); q2, d2 = A2.fit(y, d, n)
        worst = max(worst, abs(d1 - d2)); ws = max(ws, np.max(np.abs(q1 - q2)))
    return rep("bblite_reduces_to_poisson", worst < 1e-6 and ws < 1e-6, f"max|dDev|={worst:.2e} max|dScale|={ws:.2e}")

def t_closure():
    worst = 0
    for cell in B.CELLS:
        _, d, n, wd, wn = arrays(cell); y = 0.85 * d + 1.5 * n; q, dv = B.fit(y, d, n, wd, wn); worst = max(worst, abs(q[0] - 0.85), abs(q[1] - 1.5))
    return rep("closure_injected", worst < 1e-3, f"max recovery error {worst:.2e} (per-cell odd_frozen arrays, sumw2 proxy, BB on)")

def t_qinv():
    c = pick(); y, d, n, wd, wn = arrays(c); q0, _ = B.fit(y, d, n, wd, wn); q1, _ = B.fit(y, 0.8 * d, n, 0.64 * wd, wn)
    rn = abs(q1[1] - q0[1]) / q0[1]; rd = abs(q1[0] / 1.25 - q0[0]) / q0[0]
    return rep("q_invariance", rn < 1e-9 and rd < 1e-9, f"cell {c}: rel s_ND {rn:.2e}, rel s_diff(x1.25) {rd:.2e}")

def t_prof():
    y, d, n, wd, wn = arrays(pick()); q, dv = B.fit(y, d, n, wd, wn); devf = lambda a, b: B.dev_bb(y, d, n, wd, wn, [a, b]); ok = True; msg = []
    for j in (0, 1):
        p = B.prof_interval(devf, q, j); ok &= p["plus"] is not None and np.isfinite(p["plus"]) and np.isfinite(p["minus"]) and p["lower"] <= q[j] <= p["upper"] and p["plus"] > 0 and p["minus"] > 0
        msg.append(f"s[{j}]={q[j]:.4f} [{p['lower']:.4f},{p['upper'] if p['upper'] is None else round(p['upper'], 4)}]")
    return rep("prof_sanity", ok, "; ".join(msg))

def t_timing():
    y, d, n, wd, wn = arrays(pick()); q, dv = B.fit(y, d, n, wd, wn); z = np.zeros_like(y); rng = np.random.default_rng(1)
    comps = [("D", np.maximum(d / max(d.max(), 1) * 50, 1).round(), d, wd), ("7071", np.maximum(n / max(n.max(), 1) * 50, 1).round(), n, wn)]
    t0 = time.time(); devf = lambda a, b: B.dev_bb(y, d, n, wd, wn, [a, b]); [B.prof_interval(devf, q, j) for j in (0, 1)]; tp = time.time() - t0
    t0 = time.time(); B.toy_gof(y, comps, ("7071",), q, dv, 7, n_toys=50); tt = time.time() - t0
    return rep("timing", True, f"{len(y)} bins: profiles {tp:.1f}s, 50 toys {tt:.1f}s -> 1000 toys {20 * tt:.0f}s, unit total ~{tp + 20 * tt:.0f}s")

T = {"bblite_reduces_to_poisson": t_reduces, "closure_injected": t_closure, "q_invariance": t_qinv, "prof_sanity": t_prof, "timing": t_timing}
if __name__ == "__main__":
    names = [a for a in sys.argv[1:] if a != "all"] or list(T); res = [T[k]() for k in names]; sys.exit(0 if all(res) else 1)

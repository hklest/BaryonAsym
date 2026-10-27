#!/usr/bin/env python3
"""WP-A plots (JSON only): S1 data Plug N-1 fail fraction per period; S2 Plug
N-1 unit-area data vs MC before/after remedy 1; S3/S4 fine-ALL eta fit for
alt 7071 (data vs prediction) per remedy."""
import json, glob
import numpy as np
import matplotlib; matplotlib.use("Agg")
import matplotlib.pyplot as plt
import wpa_common as W
P = W.D / "plots/restart_2026-10-27/wp-a"
s1 = json.load(open(W.OUT / "s1_split/analysis/s1_checks.json"))["f_fail_plug_C"]
fig, ax = plt.subplots(figsize=(6, 4))
for i, c in enumerate(("D0", "D1", "D2")):
    xs = []; ys = []; es = []
    for j, k in enumerate(["ep0304", "em0405", "em06_true", "ep0607_true|fall-06", "ep0607_true|2007"]):
        lab = k.split("|")[0]; r = s1[lab][k][c]; xs.append(j + 0.1 * i); ys.append(r["f"]); es.append(r["err"])
    ax.errorbar(xs, ys, es, fmt="o", label=c)
ax.set_xticks(range(5)); ax.set_xticklabels(["ep0304", "em0405", "em06", "ep06", "ep07"]); ax.set_ylabel("DATA Plug N-1 fail fraction (stack C)")
ax.legend(); ax.set_title("WP-A S1: data Plug fail fraction per period"); (P / "s1").mkdir(parents=True, exist_ok=True)
fig.tight_layout(); fig.savefig(P / "s1/data_plug_failfrac_per_period.png", dpi=120)

def n1(root_d, root_m, ch):
    d = sum(np.asarray(W.load_partial(root_d, k)["hists"]["C_N1_PLUG"]["ePlug"]["raw"]) for k in W.DATA_LABELS)
    m = sum(np.asarray(W.load_partial(root_m, l)["hists"]["C_N1_PLUG"]["ePlug"]["signed"]) for l in W.DIFF8 + W.CHAINS[ch]["nd"])
    return d / d.sum(), m / m.sum()
e = np.linspace(0, 50, 101); c = 0.5 * (e[1:] + e[:-1])
d0, m0 = n1(W.OUT / "s1_split/partials/test", W.OUT / "s1_split/partials/test", "7071")
d1, m1 = n1(W.OUT / "s2_remedy1/partials/test/data", W.OUT / "s2_remedy1/partials/test/7071", "7071")
fig, ax = plt.subplots(2, 1, figsize=(7, 6), sharex=True)
ax[0].step(c, d0, where="mid", label="DATA", color="k"); ax[0].step(c, m0, where="mid", label="MC uncorrected (plus_reg+7071)")
ax[0].step(c, m1, where="mid", label="MC remedy 1 (stack-attributed)"); ax[0].set_yscale("log"); ax[0].legend(fontsize=8); ax[0].set_ylabel("unit area")
ax[0].axvline(7, ls=":", color="gray")
ax[1].plot(c, m0 / np.where(d0 > 0, d0, np.nan), "o", ms=3, label="uncorrected"); ax[1].plot(c, m1 / np.where(d1 > 0, d1, np.nan), "s", ms=3, label="remedy 1")
ax[1].axhline(1, color="k"); ax[1].set_ylim(0, 3); ax[1].set_xlabel("ePlug [GeV]"); ax[1].set_ylabel("MC/DATA"); ax[1].legend(fontsize=8)
fig.suptitle("WP-A S2: Plug N-1 (stack C, test sample)"); (P / "s2").mkdir(parents=True, exist_ok=True)
fig.tight_layout(); fig.savefig(P / "s2/plug_n1_remedy1_7071.png", dpi=120)
for rem, sub, d in (("remedy1", "s3", "s3_gof/fits"), ("remedy3", "s4", "s4_noplug/fits")):
    f = [x for x in glob.glob(str(W.OUT / d / "*7071_fine_ALL*.json"))][0]; r = json.load(open(f))["fine"]["7071"]["ALL"]
    y = np.asarray(r["data"]); p = np.asarray(r["prediction"]); x = np.arange(len(y))
    fig, ax = plt.subplots(figsize=(8, 4)); ax.step(x, y, where="mid", color="k", label="DATA"); ax.step(x, p, where="mid", label="fit (diff + ND 7071)")
    ax.set_yscale("log"); ax.set_xlabel("bin (D0 | D1 | D2, 120 cluster-eta bins each on [-1,5))"); ax.legend()
    ax.set_title("WP-A %s: §7.1 fine ALL, D/ndf %.0f/%d, toy p %.4f" % (rem, r["deviance"], r["ndf"], r["template_statistics_aware_gof"]["toy_p_ge_observed"]))
    (P / sub).mkdir(parents=True, exist_ok=True); fig.tight_layout(); fig.savefig(P / sub / ("eta_fit_fine_ALL_7071_%s.png" % rem), dpi=120)
print(sorted(str(x) for x in P.rglob("*.png")))

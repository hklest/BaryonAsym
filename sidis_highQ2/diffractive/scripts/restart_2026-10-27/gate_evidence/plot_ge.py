#!/usr/bin/env python3
"""Gate-evidence figures (restart 2026-10-27).  Login node: reads stored JSON only.
  fig1  eta_max two-free-template fit (G1'(c)): WP-A2 S4 payloads + fit results (stored).
  fig2-5  from data/restart_2026-10-27/gate_evidence/hists_ge.json (hist_ge.py, Condor).
Prints one summary line per figure (ratio ranges) to stdout and writes summary.json."""
import json, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent; sys.path.insert(0, str(HERE))
import ge_style as S

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
S4 = D / "data/restart_2026-10-27/wp-a2/s4_g1p"
HJ = D / "data/restart_2026-10-27/gate_evidence/hists_ge.json"
OUT = D / "plots/restart_2026-10-27/gate_evidence"
DIFF = ("9012", "9024", "9033", "9155", "9157", "9158", "6671", "6677"); REG = ("6671", "6677")
CELLS = ("D0", "D1", "D2")
CH_LABEL = {"7071": "ND 7071 (RAPGAP ME+PS)", "7094": "ND 7094 (RAPGAP ME+PS)", "7091": "ND 7091 (RAPGAP CDM)",
            "nd4cat": "ND nd4cat (7071+7094+7096+7097, ME+PS)"}
SUM = {}


def rng(x):
    x = np.asarray(x, float); x = x[np.isfinite(x)]
    return [float(x.min()), float(x.max())] if x.size else None


def chi2_shape(d, m, v, q):
    d, m, v = d[q], m[q], v[q]
    return float(np.sum((d - m) ** 2 / (d + v))), int(q.sum())


# ------------------------------------------------------------------ fig 1
def fig1(ch):
    P = json.load(open(S4 / "payloads" / f"odd_frozen_{ch}.json")); c = P["modes"]["cluster"]
    fac = P["scale_table_basis"]["exposure_factors"]; edges = np.asarray(P["edges"], float)
    nd = [l for l in c["components"] if l not in DIFF]
    for binning in ("fine", "coarse"):
        fig, ax = S.grid(3)
        summ = {}
        for j, z in enumerate(CELLS):
            R = json.load(open(S4 / "fits" / f"section_7_1_twofree_fit_results_{ch}_{binning}_{z}.json"))
            r = R["fine" if binning == "fine" else "coarse_ge100_data"][ch][z]
            sd, sn = r["s_diffractive"], r["s_ND"]
            y = np.asarray(c["data"]["templates"]["raw"][z], float)
            wt = {l: np.asarray(c["components"][l]["templates"]["exposure1"][z], float) * fac[l] for l in c["components"]}
            rw = {l: np.asarray(c["components"][l]["templates"]["raw"][z], float) for l in c["components"]}
            s_of = {l: (sd if l in DIFF else sn) for l in wt}
            if binning == "coarse":
                g = r["groups"]; e = np.array([edges[a] for a, _ in g] + [edges[g[-1][1]]])
                agg = lambda x: np.array([x[a:b].sum() for a, b in g])
                y = agg(y); wt = {l: agg(v) for l, v in wt.items()}; rw = {l: agg(v) for l, v in rw.items()}
            else:
                e = edges
            sig = sum(s_of[l] * wt[l] for l in DIFF if l not in REG); reg = sum(s_of[l] * wt[l] for l in REG)
            ndv = sum(s_of[l] * wt[l] for l in nd); tot = sig + reg + ndv
            var = sum(np.divide((s_of[l] * wt[l]) ** 2, rw[l], out=np.zeros_like(tot), where=rw[l] > 0) for l in wt)
            assert np.allclose(tot, r["prediction"], rtol=1e-6), (ch, binning, z)
            a, b = ax[0][j]
            S.panel(a, b, e, y, [("diffractive: RAPGAP 9012 + ODS 9024/9033/9155/9157/9158", sig, S.C["signal"]),
                                 ("Reggeon 6671/6677", reg, S.C["reggeon"]),
                                 (CH_LABEL[ch], ndv, S.C["nd"])], var,
                    density=True, logy=True, ylabel="events / unit $\\eta$", xlabel="$\\eta_{max}$ (etaMaxClus04)",
                    rlim=(0.4, 1.8), top=200, qual=y >= 10)
            S.vline(a, b, 3.4, "fit range edge 3.4", color="0.35", ls=":")
            dev = np.asarray(r["deviance_contribution"]); top3 = np.sort(dev)[-3:].sum() / dev.sum()
            gof = r["template_statistics_aware_gof"]
            q = y >= 20; rr = y[q] / tot[q]
            hi = (e[:-1] >= 2.5) & q; lo = (e[1:] <= 2.5) & q
            S.note(a, "%s  odd split   fitted s_diff = %.2f, s_ND = %.2f\nD/ndf = %.0f/%d   toy p %s\ntoy median D = %.0f\ntop-3 bins: %.0f%% of D"
                   % (z, sd, sn, r["deviance"], r["ndf"], ("= %.3f" % gof["toy_p_ge_observed"]) if gof["toy_p_ge_observed"] > gof["p_floor"] else "< %.3f (floor)" % (gof["p_floor"] * 1.001),
                      gof["toy_median"], 100 * top3), loc="ul")
            mm = lambda k: [float(np.min(y[k] / tot[k])), float(np.max(y[k] / tot[k]))] if k.any() else None
            summ[z] = {"n_bins_ge20": int(q.sum()), "ratio_eta_lt_2p5": mm(lo), "ratio_eta_ge_2p5": mm(hi),
                       "frac_bins_within_10pct": float(np.mean(np.abs(rr - 1) <= 0.10)),
                       "frac_bins_within_2sigma": float(np.mean(np.abs(y[q] - tot[q]) <= 2 * np.sqrt(y[q] + var[q]))),
                       "deviance": r["deviance"], "ndf": r["ndf"], "toy_median": gof["toy_median"], "toy_p": gof["toy_p_ge_observed"],
                       "top3_share": float(top3), "eta_of_top3": [float(e[i]) for i in np.argsort(dev)[-3:]],
                       "chi2_data_plus_mcstat": chi2_shape(y, tot, var, y > 0),
                       "chi2_data_plus_mcstat_eta_lt_3": chi2_shape(y, tot, var, (y > 0) & (e[1:] <= 3.0 + 1e-9)),
                       "ratio_eta_3p0_3p4": mm(q & (e[:-1] >= 3.0 - 1e-9) & (e[1:] <= 3.4 + 1e-9)),
                       "ratio_merged_3p4_5": float(y[-1] / tot[-1])}
            if j == 0: S.figlegend(fig, a, ncol=3)
        S.save(fig, OUT / f"fig1_etamax_fit_{ch}_{binning}.png",
               f"LRG preselection (FIT sample), post-fit two-template stack, {binning} binning, chain {ch}")
        SUM[f"fig1_{ch}_{binning}"] = summ


# ------------------------------------------------------------------ hists helpers
H = None
def g(s, v, p, cell, ch, grp, mp="none"):
    x = H["hists"]["|".join((s, v, p, cell, ch if grp != "data" else "data", grp, mp))]
    return np.asarray(x["sumw"], float), np.asarray(x["sumw2"], float), x


def stack(s, v, p, cell, ch, mp="none"):
    parts = {k: g(s, v, p, cell, ch, k, mp) for k in ("signal", "reggeon", "nd")}
    return parts, sum(x[1] for x in parts.values())


def spec_edges(s, v):
    for ss, vv, ed, _, _ in H["specs"]:
        if ss == s and vv == v: return np.asarray(ed, float)


def ffail(s, cell, ch, mp):
    """Plug fail fraction ePlug>=7 or non-finite (gate convention), data and MC."""
    e = spec_edges(s, "eplug"); k = e[:-1] >= 7
    dd, _, dx = g(s, "eplug", "odd", cell, ch, "data")
    fd = (dd[k].sum() + dx["overflow"] + dx["nonfinite"]) / (dd.sum() + dx["overflow"] + dx["nonfinite"])
    num = den = 0.
    for grp in ("signal", "reggeon", "nd"):
        m, _, mx = g(s, "eplug", "odd", cell, ch, grp, mp)
        num += m[k].sum() + mx["overflow"] + mx["nonfinite"]; den += m.sum() + mx["overflow"] + mx["nonfinite"]
    return fd, num / den


# ------------------------------------------------------------------ fig 2 Plug N-1
def fig2(ch):
    e = spec_edges("N1_PLUG", "eplug"); sel = e[1:] <= 10 + 1e-9; e10 = e[:int(sel.sum()) + 1]
    out = {}
    for logy in (False, True):
        fig, ax = S.grid(3)
        for j, z in enumerate(CELLS):
            d, _, _ = g("N1_PLUG", "eplug", "odd", z, ch, "data"); parts, var = stack("N1_PLUG", "eplug", "odd", z, ch)
            q = d >= 20; tot = sum(p[0] for p in parts.values()); k = d[q].sum() / tot[q].sum()
            n = len(e10) - 1
            a, b = ax[0][j]
            S.panel(a, b, e10, d[:n], [("diffractive 9012+ODS", k * parts["signal"][0][:n], S.C["signal"]),
                                       ("Reggeon 6671/6677", k * parts["reggeon"][0][:n], S.C["reggeon"]),
                                       (CH_LABEL[ch], k * parts["nd"][0][:n], S.C["nd"])], k * k * var[:n],
                    logy=logy, ylabel="events / 0.5 GeV", xlabel="$E_{Plug}$ [GeV]")
            S.vline(a, b, 7.0, "veto: fail if $E_{Plug}\\geq$ 7 GeV", color="#b22222")
            fd, fm = ffail("N1_PLUG", z, ch, "none")
            r = d[:n] / (k * tot[:n])
            S.note(a, "%s  odd split\nMC unit area on bins with >=20 data\nf_fail data %.3f, MC %.3f\ndata/MC [0,0.5) = %.2f\ndata/MC 1-7 GeV: %.1f-%.1f"
                   % (z, fd, fm, r[0], np.min(r[2:14]), np.max(r[2:14])), loc="ur")
            out[z] = {"f_fail_data": fd, "f_fail_mc": fm, "ratio_bin0": float(r[0]), "ratio_1_7": [float(np.min(r[2:14])), float(np.max(r[2:14]))],
                      "mc_over_data_bin0": float(1 / r[0]), "mc_over_data_1_7": [float(1 / np.max(r[2:14])), float(1 / np.min(r[2:14]))],
                      "norm_k": float(k)}
            if j == 0: S.figlegend(fig, a)
        S.save(fig, OUT / f"fig2_plug_n1_{ch}_{'log' if logy else 'lin'}.png",
               f"Plug energy N-1 (all other LRG cuts applied), chain {ch}")
    SUM[f"fig2_{ch}"] = out


# ------------------------------------------------------------------ fig 3 FMD / FTS N-1
def fig3(ch):
    fig, ax = S.grid(2, w=4.6)
    out = {}
    for j, (s, v, lab) in enumerate((("N1_FMD", "nfmu12", "FMD hit pairs, layers 1-2 (nFmu12)"), ("N1_FTS", "nfts28", "FTS hits, stations 2+8 (nFts28)"))):
        e = spec_edges(s, v); d, _, dx = g(s, v, "odd", "ALL", ch, "data"); parts, var = stack(s, v, "odd", "ALL", ch)
        q = d >= 20; tot = sum(p[0] for p in parts.values()); k = d[q].sum() / tot[q].sum()
        n = len(e) - 1 if s == "N1_FMD" else 5   # nFts28 saturates at 4 (one count per FTS layer)
        a, b = ax[0][j]
        S.panel(a, b, e[:n + 1], d[:n], [("diffractive 9012+ODS", k * parts["signal"][0][:n], S.C["signal"]),
                             ("Reggeon 6671/6677", k * parts["reggeon"][0][:n], S.C["reggeon"]),
                             (CH_LABEL[ch], k * parts["nd"][0][:n], S.C["nd"])], k * k * var[:n], logy=True,
                ylabel="events", xlabel=lab, qual=q[:n])
        if s == "N1_FMD": S.vline(a, b, 1.5, "selFmd passes only for nFmu12 <= 1", color="#b22222")
        else: S.vline(a, b, 0.5, "selFts = (nFts28 == 0)", color="#b22222")
        r = d[q] / (k * tot[q]); within = np.abs(d[q] - k * tot[q]) <= 2 * np.sqrt(d[q] + k * k * var[q])
        S.note(a, "ALL cells, odd split; MC unit area, bins >=20 data\ndata/MC range %.2f-%.2f\nbins within 2 sigma (data+MC stat): %d/%d"
               % (r.min(), r.max(), within.sum(), q.sum()), loc="ur")
        out[s] = {"ratio": [float(r.min()), float(r.max())], "ratio_bins": [round(float(x), 3) for x in r], "within2sigma": [int(within.sum()), int(q.sum())]}
        if j == 0: S.figlegend(fig, a, ncol=3)
    S.save(fig, OUT / f"fig3_fmd_fts_n1_{ch}.png", f"FMD and FTS N-1 distributions, chain {ch}")
    SUM[f"fig3_{ch}"] = out


# ------------------------------------------------------------------ fig 4 inclusive forward edge
def mc_tot(s, v, p, cell, ch, mp="none"):
    parts, var = stack(s, v, p, cell, ch, mp); return sum(x[0] for x in parts.values()), var


def fig4():
    e = spec_edges("H4", "ec08"); cen = 0.5 * (e[1:] + e[:-1])
    out = {}
    for s, tag, xr, ttl in (("H4", "fig4a_h4_inclusive_eta08", (-1, 5), "inclusive no-veto DIS (H4: A_pre, 35<E-pz<70)"),
                             ("H4E", "fig4b_h4_edge_eplug20_eta08", (2.5, 4.0), "H4 edge sample (H4 and E_Plug > 20 GeV)")):
        fig, ax = S.grid(3)
        o = {}
        for j, z in enumerate(CELLS):
            d, _, _ = g(s, "ec08", "even", z, "7071", "data")
            lines = []; oz = {}
            for ch, ls in (("7071", "-"), ("7094", "-"), ("7091", "-")):
                m, v = mc_tot(s, "ec08", "even", z, ch); k = d.sum() / m.sum()
                lines.append(("DIFF8 + " + CH_LABEL[ch], k * m, k * k * v, S.C[ch], ls))
                w = (cen >= 3.0) & (cen < 3.6) & (d >= 20)
                rr = d[w] / (k * m[w]); cw = cen[w]; lowq = (cen >= -0.5) & (cen < 2.0) & (d >= 20)
                oz[ch] = {"ratio_3p0_3p35": rng(rr[cw < 3.35]), "ratio_3p40_3p55": rng(rr[(cw >= 3.4) & (cw < 3.55)]),
                          "worst_mc_over_data_minus1": float(np.max(np.abs(1 / rr - 1))) if rr.size else None,
                          "ratio_m0p5_2": rng(d[lowq] / (k * m[lowq]))}
            i0, i1 = np.searchsorted(e, xr[0] - 1e-9), np.searchsorted(e, xr[1] - 1e-9)
            ee = e[i0:i1 + 1]
            a, b = ax[0][j]
            S.panel(a, b, ee, d[i0:i1], [], None, mc_lines=[(l, x[i0:i1], vv[i0:i1], c_, ls) for l, x, vv, c_, ls in lines],
                    logy=(s == "H4"), ylabel="events / 0.05", xlabel="$\\eta_{max}$ (etaMaxClus08)", qual=d[i0:i1] >= 20,
                    rlim=(0, 3.0))
            S.vline(a, b, 3.3, "LRG cut 3.3", color="#b22222")
            S.note(a, "%s  even split\nMC normalised to data per cell\n(full %s sample)" % (z, s), loc="ll" if s == "H4" else "ul")
            if j == 0: S.figlegend(fig, a)
            o[z] = oz
        S.save(fig, OUT / f"{tag}.png", ttl + ", data vs each ND generator")
        out[s] = o
    # fig4c: drift in the veto-passing (cut-removed) sample, normalised in 1.0 <= eta < 2.5
    fig, ax = S.grid(1, w=8.0, h=4.2)
    d, _, _ = g("FIT", "ec08", "even", "ALL", "7071", "data"); lines = []; oc = {}
    win = (e[:-1] >= 1.0 - 1e-9) & (e[1:] <= 2.5 + 1e-9)
    for ch in ("7071", "7091", "nd4cat"):
        m, v = mc_tot("FIT", "ec08", "even", "ALL", ch); k = d[win].sum() / m[win].sum()
        lines.append(("DIFF8 + " + CH_LABEL[ch], k * m, k * k * v, S.C[ch], "--" if ch == "nd4cat" else ("-" if ch == "7091" else ":")))
        nd_, _, _ = g("FIT", "ec08", "even", "ALL", ch, "nd"); knd = d[win].sum() / nd_[win].sum()
        coarse = {}
        for x0 in (1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25):
            i = np.searchsorted(e, x0 - 1e-9); coarse[x0] = [float(d[i:i + 5].sum() / (k * m[i:i + 5]).sum()), float(d[i:i + 5].sum() / (knd * nd_[i:i + 5]).sum())]
        oc[ch] = {"coarse_0p25_from_x0_DIFF8+ND_vs_NDonly": coarse,
                  "first_0p05_bin_outside_10pct": next((float(e[i]) for i in range(np.searchsorted(e, 2.5 - 1e-9), 120) if d[i] >= 20 and abs(k * m[i] / d[i] - 1) > 0.10), None)}
    i0, i1 = np.searchsorted(e, 0.0 - 1e-9), np.searchsorted(e, 3.6 - 1e-9)
    a, b = ax[0][0]
    S.panel(a, b, e[i0:i1 + 1], d[i0:i1], [], None, mc_lines=[(l, x[i0:i1], vv[i0:i1], c_, ls) for l, x, vv, c_, ls in lines],
            logy=True, ylabel="events / 0.05", xlabel="$\\eta_{max}$ (etaMaxClus08)", qual=d[i0:i1] >= 20, rlim=(0.6, 2.2))
    for x, lab, col in ((1.0, "norm. window", "0.5"), (2.5, "", "0.5"), (2.25, "2.25", "#555555"), (2.55, "2.55", "#555555"), (3.3, "LRG cut 3.3", "#b22222")):
        S.vline(a, b, x, lab, color=col, ls=":" if col != "#b22222" else "--")
    b.axhspan(0.9, 1.1, color="0.85", zorder=0)
    a.legend(loc="upper left", frameon=False, fontsize=7.5)
    S.save(fig, OUT / "fig4c_fit_eta08_drift.png", "Onset of the forward data/MC drift: veto-passing sample (selFmd, $E_{Plug}$<7 GeV, selFts), no $\\eta_{max}$ cut\n"
           "all cells, even split; DIFF8 + ND normalised to data in 1.0 <= $\\eta_{max}$ < 2.5")
    out["FIT_drift"] = oc
    SUM["fig4"] = out


# ------------------------------------------------------------------ fig 5 reweighting before/after
def fig5(ch):
    fig, ax = S.grid(2, w=4.8)
    o = {}
    e = spec_edges("N1_PLUG", "eplug"); n = int((e[1:] <= 10 + 1e-9).sum()); d, _, _ = g("N1_PLUG", "eplug", "odd", "ALL", ch, "data")
    lines = []; q = d >= 20
    for mp, ls, lab in (("none", "--", "MC, no map"), ("XA", "-", "MC, WP-A3 veto-state map (XA)")):
        m, v = mc_tot("N1_PLUG", "eplug", "odd", "ALL", ch, mp); k = d[q].sum() / m[q].sum()
        lines.append((lab, k * m[:n], k * k * v[:n], S.C[ch] if mp == "XA" else "0.35", ls))
        r = d[:n] / (k * m[:n]); o["plug_" + mp] = {"ratio_bin0": float(r[0]), "ratio_1_7": [float(r[2:14].min()), float(r[2:14].max())],
                                                    "ratio_2_2p5": float(r[4]), "ffail": {z: ffail("N1_PLUG", z, ch, mp) for z in CELLS}}
    a, b = ax[0][0]
    S.panel(a, b, e[:n + 1], d[:n], [], None, mc_lines=lines, logy=True, ylabel="events / 0.5 GeV", xlabel="$E_{Plug}$ [GeV]",
            qual=d[:n] >= 20, rlim=(0, 4.0), top=60)
    S.vline(a, b, 7.0, "veto 7 GeV", color="#b22222")
    ff = o["plug_none"]["ffail"]; fx = o["plug_XA"]["ffail"]
    S.note(a, "Plug N-1, all cells, odd split; MC unit area (bins >=20 data)\nf_fail D0/D1/D2: data %s\nMC no map %s\nMC map %s"
           % ("/".join("%.3f" % ff[z][0] for z in CELLS), "/".join("%.3f" % ff[z][1] for z in CELLS), "/".join("%.3f" % fx[z][1] for z in CELLS)), loc="ur")
    a.legend(loc="lower left", frameon=False, fontsize=7)
    e = spec_edges("FIT", "ec04"); d, _, _ = g("FIT", "ec04", "odd", "ALL", ch, "data")
    i0, i1 = np.searchsorted(e, 1.0 - 1e-9), np.searchsorted(e, 4.0 - 1e-9); lines = []
    for mp, ls, lab in (("none", "--", "MC, no map"), ("XA", "-", "MC, map (XA)")):
        m, v = mc_tot("FIT", "ec04", "odd", "ALL", ch, mp); k = d.sum() / m.sum()
        lines.append((lab, k * m[i0:i1], k * k * v[i0:i1], S.C[ch] if mp == "XA" else "0.35", ls))
        cen = 0.5 * (e[1:] + e[:-1]); w = (cen >= 2.8) & (cen < 3.4) & (d >= 20)
        rr_ = d / np.where(k * m > 0, k * m, np.nan); w15 = (cen >= 1.5) & (cen < 2.8) & (d >= 20); w30 = (cen >= 3.0) & (cen < 3.45) & (d >= 20)
        o["eta_" + mp] = {"ratio_2p8_3p4": [float((d[w] / (k * m[w])).min()), float((d[w] / (k * m[w])).max())],
                          "ratio_1p5_2p8": rng(rr_[w15]), "ratio_3p0_3p45": rng(rr_[w30])}
    a, b = ax[0][1]
    S.panel(a, b, e[i0:i1 + 1], d[i0:i1], [], None, mc_lines=lines, logy=True, ylabel="events / 0.05", xlabel="$\\eta_{max}$ (etaMaxClus04)",
            qual=d[i0:i1] >= 20, rlim=(0.4, 2.4))
    S.vline(a, b, 3.4, "fit range edge 3.4", color="0.35", ls=":")
    S.note(a, "veto-passing FIT sample, all cells, odd split\nMC normalised to data (full range)", loc="ul")
    a.legend(loc="center left", bbox_to_anchor=(0.0, 0.66), frameon=False, fontsize=7)
    S.save(fig, OUT / f"fig5_reweight_before_after_{ch}.png", f"WP-A3 veto-state map: before vs after, DIFF8 + {CH_LABEL[ch]}")
    SUM[f"fig5_{ch}"] = o


def main():
    global H
    OUT.mkdir(parents=True, exist_ok=True)
    for ch in ("7071", "7091", "nd4cat"): fig1(ch)
    if HJ.exists():
        H = json.load(open(HJ)); assert H["checks"]["skim_identity_s4"]["PASS"]
        for ch in ("7071", "7091", "nd4cat"): fig2(ch); fig3(ch); fig5(ch)
        fig4()
    json.dump(SUM, open(D / "data/restart_2026-10-27/gate_evidence/summary.json", "w"), indent=1, default=float)
    print(json.dumps(SUM, default=lambda x: round(float(x), 3))[:200])


if __name__ == "__main__":
    main()

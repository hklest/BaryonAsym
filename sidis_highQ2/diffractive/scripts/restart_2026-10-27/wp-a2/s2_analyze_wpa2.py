#!/usr/bin/env python3
"""WP-A2 S2 analysis (login node, numpy on JSON): reads the even-split n1_wpa2
partials and prints one supported / not-supported line per hypothesis H1, H2,
H4, H5, H6 with its D3 criterion value (child plan wp-a2_plan.md D3, read with
the binding readings R1-R8 of s1_definition_audit.md §6).  H3 needs fits and is
ruled in S3.  Only --split even partials size anything (D4); --split odd is
accepted for the S4 descriptive tables and never prints a ruling.

Conventions fixed here, before any S2 output was read:
- cocktail(chain) = DIFF8 + ND(chain) with signed v3 weights and the chain's
  exposure rescale (wpa_common.fac_used / table_fac), as g1_eval_wpa.py;
- H1 size (R5): MC cocktail scaled to DATA in 3.5 <= eta < 5 per scope; size of
  a window = sum_window(DATA - MC) / DATA total (full range); headline scope ALL
  (D0+D1+D2); response-driven if BOTH the 3.15-3.35 excess and the 3.40-3.55
  deficit change by >= 50 % of their 0.8 GeV size between 0.8 and 0.4 GeV, for
  both 7071 and 7091; geometry: |median(DATA) - median(9012)| of delta-eta >= 0.03,
  DATA = ep0607_true (the only period 9012 represents), all-period value reported;
- H6: 9012 reweighted to the ep0607_true hatVtxZ (2 cm bins, A_pre); supported
  iff the H1 geometry shift exists (>= 0.03) and the reweight moves it by >= 50 %;
- H4 (R1): edge = ePlug > 20 GeV H4 events, eta in [3.0, 3.6), bins with >= 20
  DATA, MC normalised to DATA per D cell over the edge sample's full eta range;
  true-detector if, for both 7071 and 7091, worst |MC/DATA-1| >= 0.15 and the sign
  pattern holds (DATA > MC summed over [3.15,3.35), DATA < MC over [3.40,3.55)),
  in at least one D cell; true-physics only if not true-detector, edge worst
  <= 0.10 in every cell and inclusive eta and ePlug shape metrics > 0.10 in the
  same direction for both; false if inclusive metrics <= 0.10 everywhere;
- H5: (i) from plug_calib.csv; (ii) reported (R7);
- H2 (R2): per period, runs ordered by run number and, separately, by per-run
  luminosity are grouped into 5 blocks of equal control count; supported if any
  period/detector has block chi2 p < 0.0027 or |slope z| > 3; per-layer: the
  DATA-MC hit-fraction excess per FMD layer / FTS station is tested for
  uniformity (chi2 p < 0.0027 = layer-specific)."""
import argparse, csv, json, math, sys
from pathlib import Path
import numpy as np
from scipy.stats import chi2 as CHI2
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "wp-a"))
import wpa_common as W

CELLS = ("D0", "D1", "D2")
E = np.linspace(-1, 5, 121)
LO = E[:-1]


def load(root, label, split):
    p = Path(root) / "partials" / label / split / "partial.json"
    if not p.exists():
        raise SystemExit("FATAL: missing " + str(p))
    return json.load(open(p))


def proj_share(label, chain, g):
    """Per-period projection of an MC component (fix after the S2 h1-mc-expert review):
    MC events carry the run numbers of their simulation period only, so per-period
    MC tables take the component's ALL-group content times L_g / L(represented
    periods) for g in the chain's represented periods (wpa_common.represented), not
    the RunNumber grouping."""
    rp = W.represented(label, chain); L = W.period_lumi()
    return L[g] / sum(L[k] for k in rp) if g in rp else 0.0


def rs(label, chain):
    return W.fac_used(label, chain) / W.table_fac(label)


def cocktail(chain):
    return W.DIFF8 + W.CHAINS[chain]["nd"]


def med(h, e):
    h = np.asarray(h, float); c = np.cumsum(h)
    if c[-1] <= 0:
        return None
    i = int(np.searchsorted(c, 0.5 * c[-1])); prev = c[i - 1] if i else 0.0
    f = (0.5 * c[-1] - prev) / h[i] if h[i] > 0 else 0.5
    return float(e[i] + f * (e[i + 1] - e[i]))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", required=True); ap.add_argument("--split", default="even", choices=("even", "odd"))
    ap.add_argument("--plug-calib", default=str(W.D / "data/restart_2026-10-27/wp-a2/s1_db/plug_calib.csv"))
    ap.add_argument("--runlist", default=str(W.D / "data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv"))
    a = ap.parse_args()
    root = Path(a.root); out = root / "analysis"; out.mkdir(parents=True, exist_ok=True)
    mcl = sorted(set(W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"]))
    P = {k: load(root, k, a.split) for k in W.DATA_LABELS + mcl}
    for k, p in P.items():
        if p["wpa2"]["split"] != a.split or p["wpa2"]["even_odd_key_overlap"]:
            raise SystemExit("FATAL: split mismatch in " + k)
    res = {"split": a.split, "inputs": {k: W.sha256(root / "partials" / k / a.split / "partial.json") for k in P}}
    lines = []

    def dsum(block, cell, key="raw", labels=W.DATA_LABELS, get=None):
        return sum(np.asarray(get(P[k])[cell][key], float) for k in labels)

    def msum(get, cell, chain, key="signed"):
        return sum(rs(l, chain) * np.asarray(get(P[l])[cell][key], float) for l in cocktail(chain))

    # ---------------- coverage tests ----------------
    cov = {k: P[k]["wpa2"]["empz_join"]["coverage_pre"] for k in P}
    res["empz_join_coverage"] = {"per_file": cov, "below_0p999": sorted(k for k, v in cov.items() if v < 0.999)}
    # ---------------- H5 ----------------
    cal = list(csv.DictReader(open(a.plug_calib)))
    enc = {float(r["encor"]) for r in cal}; thr = {float(r["thresh"]) for r in cal}
    h5i = (enc != {1.0}) or (len(thr) > 1)
    ef = np.asarray(P["ep0304"]["wpa2"]["eplug_fine"]["edges"])
    h5 = {"encor_values": sorted(enc), "thresh_values": sorted(thr), "i_true": h5i, "periods": {}}
    for g in W.PERIODS:
        d = sum(np.asarray(P[k]["wpa2"]["eplug_fine"]["eta35"].get(g, {"raw": np.zeros(len(ef) - 1)})["raw"], float) for k in W.DATA_LABELS)
        row = {"data_n": float(d.sum())}
        for ch in ("7071", "7091", "nd4cat"):
            m = sum(rs(l, ch) * proj_share(l, ch, g) * np.asarray(P[l]["wpa2"]["eplug_fine"]["eta35"]["ALL"]["signed"], float) for l in cocktail(ch))
            if d.sum() <= 0 or m.sum() <= 0:
                continue
            dn, mn = d / d.sum(), m / m.sum()
            nz = lambda h: [float(h[i] / h[0]) if h[0] > 0 else None for i in (3, 5, 10)]  # 0.3-0.4, 0.5-0.6, 1.0-1.1 GeV over the 0 bin
            w17 = (ef[:-1] >= 1.0) & (ef[:-1] < 7.0)
            row[ch] = {"zero_bin_frac_data": float(dn[0]), "zero_bin_frac_mc": float(mn[0]),
                       "ratio_bins_to_zero_data": nz(dn), "ratio_bins_to_zero_mc": nz(mn),
                       "mc_over_data_1to7": float(mn[w17].sum() / dn[w17].sum()) if dn[w17].sum() > 0 else None}
        h5["periods"][g] = row
    res["H5"] = h5
    lines.append("H5 %s: EnCor %s, Thresh %s GeV (criterion: EnCor!=1 or Thresh differs); 1-7 GeV MC/DATA (7071, unit area, eta>3.5) %s"
                 % ("supported" if h5i else "not supported", sorted(enc), sorted(thr),
                    {g: round(v.get("7071", {}).get("mc_over_data_1to7") or float("nan"), 3) for g, v in h5["periods"].items()}))
    # ---------------- H1 ----------------
    def var(p, name):
        return p["wpa2"]["dcell_eta_variants"][name]
    h1 = {}
    for thr_ in ("c08", "c04"):
        nm = "cutremoved_" + thr_
        for ch in ("7071", "7091", "7094", "nd4cat"):
            for sc in CELLS + ("ALL",):
                cl = CELLS if sc == "ALL" else (sc,)
                d = sum(dsum(None, c, "raw", get=lambda p: var(p, nm)) for c in cl)
                m = sum(msum(lambda p: var(p, nm), c, ch) for c in cl)
                nrm = (LO >= 3.5 - 1e-9) & (LO < 5.0 - 1e-9)
                m = m * (d[nrm].sum() / m[nrm].sum()) if m[nrm].sum() > 0 else m
                ex = (LO >= 3.15 - 1e-9) & (LO < 3.35 - 1e-9); de = (LO >= 3.40 - 1e-9) & (LO < 3.55 - 1e-9)
                h1["%s|%s|%s" % (thr_, ch, sc)] = {"excess": float((d[ex] - m[ex]).sum() / d.sum()), "deficit": float((d[de] - m[de]).sum() / d.sum()),
                                                  "data_over_mc_bins_3p0_3p6": [float(d[i] / m[i]) if m[i] > 0 else None for i in np.where((LO >= 3.0 - 1e-9) & (LO < 3.6 - 1e-9))[0]]}
    resp = {}
    for ch in ("7071", "7091"):
        r8, r4 = h1["c08|%s|ALL" % ch], h1["c04|%s|ALL" % ch]
        mv_ex = abs(r4["excess"] - r8["excess"]) / abs(r8["excess"]) if r8["excess"] else float("inf")
        mv_de = abs(r4["deficit"] - r8["deficit"]) / abs(r8["deficit"]) if r8["deficit"] else float("inf")
        resp[ch] = {"move_excess": mv_ex, "move_deficit": mv_de, "response_true": bool(mv_ex >= 0.5 and mv_de >= 0.5)}
    dv = {k: P[k]["wpa2"]["deta_vtx"] for k in P}
    deE = np.asarray(dv["ep0304"]["edges"])
    md_data607 = med(dv["ep0607_true"]["all"]["raw"], deE)
    md_all = med(sum(np.asarray(dv[k]["all"]["raw"], float) for k in W.DATA_LABELS), deE)
    md_9012 = med(dv["9012"]["all"]["signed"], deE)
    shift = abs(md_data607 - md_9012) if None not in (md_data607, md_9012) else None
    h1res = {"rows": h1, "response": resp, "deta_median": {"data_ep0607": md_data607, "data_all": md_all, "9012": md_9012,
             "shift_ep0607_vs_9012": shift, "n_data_ep0607": dv["ep0607_true"]["n_with_vtx_raw"], "n_9012": dv["9012"]["n_with_vtx_raw"]}}
    h1_resp = all(v["response_true"] for v in resp.values()); h1_geo = shift is not None and shift >= 0.03
    h1res["supported_response"] = h1_resp; h1res["supported_geometry"] = h1_geo
    res["H1"] = h1res
    lines.append("H1 %s: response move excess/deficit 7071 %.2f/%.2f, 7091 %.2f/%.2f (>=0.5 both, both alts); delta-eta median shift ep0607-9012 %s (>=0.03; geometry -> H6 per R5)"
                 % ("supported (response)" if h1_resp else ("geometry only -> H6" if h1_geo else "not supported"),
                    resp["7071"]["move_excess"], resp["7071"]["move_deficit"], resp["7091"]["move_excess"], resp["7091"]["move_deficit"],
                    "%.4f" % shift if shift is not None else None))
    # ---------------- H6 ----------------
    vzE = np.asarray(P["ep0304"]["wpa2"]["vtxz"]["edges"])
    dz = np.asarray(P["ep0607_true"]["wpa2"]["vtxz"]["A_pre_coarse"]["ALL"]["raw"], float)
    mz = np.asarray(P["9012"]["wpa2"]["vtxz"]["A_pre_coarse"]["ALL"]["signed"], float)
    wz = np.where(mz > 0, (dz / dz.sum()) / np.where(mz > 0, mz / mz.sum(), 1), 1.0)
    H2d = np.asarray(dv["9012"]["vz_deta_signed"], float)
    md_rw = med((H2d * wz[:, None]).sum(axis=0), deE)
    move = abs(md_rw - md_9012) / shift if (shift and md_rw is not None) else None
    vzmean = {k: float(np.average(0.5 * (vzE[1:] + vzE[:-1]), weights=np.maximum(np.asarray(P[k]["wpa2"]["vtxz"]["A_pre_coarse"]["ALL"]["raw" if k in W.DATA_LABELS else "signed"], float), 0)))
              for k in P}
    h6 = {"vtxz_mean_cm": vzmean, "weights_9012_to_ep0607": wz.tolist(), "deta_median_9012_reweighted": md_rw, "move_fraction": move,
          "supported": bool(h1_geo and move is not None and move >= 0.5)}
    res["H6"] = h6
    lines.append("H6 %s: 9012 delta-eta median %.4f -> %.4f after hatVtxZ reweight; move/shift %s (>=0.5 and H1 geometry shift>=0.03)"
                 % ("supported" if h6["supported"] else "not supported", md_9012, md_rw, "%.2f" % move if move is not None else "n/a"))
    # ---------------- H4 ----------------
    h4 = {}
    def h4b(p, nm): return p["wpa2"]["h4"][nm]
    share = {}
    for c in CELLS:
        tot = sum(rs(l, "7071") * P[l]["wpa2"]["h4"]["counts"][c]["signed"] for l in cocktail("7071"))
        nd = rs("7071", "7071") * P["7071"]["wpa2"]["h4"]["counts"][c]["signed"]
        share[c] = nd / tot if tot else None
    h4["nd_share_7071"] = share
    win = (LO >= 3.0 - 1e-9) & (LO < 3.6 - 1e-9); ex = (LO >= 3.15 - 1e-9) & (LO < 3.35 - 1e-9); de = (LO >= 3.40 - 1e-9) & (LO < 3.55 - 1e-9)
    det_alt, phys_alt, false_alt = {}, {}, {}
    for ch in ("7071", "7091", "nd4cat", "7094"):
        cellrows = {}
        for c in CELLS:
            d = dsum(None, c, get=lambda p: h4b(p, "edge_plugtag_c08")); m = msum(lambda p: h4b(p, "edge_plugtag_c08"), c, ch)
            m = m * d.sum() / m.sum() if m.sum() > 0 else m
            q = win & (d >= W.MIN_DATA_ENTRIES)
            dev = np.abs(m[q] / d[q] - 1) if q.any() else np.array([])
            worst = float(dev.max()) if dev.size else None
            pattern = bool(d[ex].sum() > m[ex].sum() and d[de].sum() < m[de].sum())
            di = dsum(None, c, get=lambda p: h4b(p, "incl_eta_c08")); mi = msum(lambda p: h4b(p, "incl_eta_c08"), c, ch)
            se, be, _ = W.shape_metric(di, mi)
            dp_ = sum(np.asarray(P[k]["wpa2"]["h4"]["incl_eplug"][c]["raw"], float) for k in W.DATA_LABELS)
            mp_ = sum(rs(l, ch) * np.asarray(P[l]["wpa2"]["h4"]["incl_eplug"][c]["signed"], float) for l in cocktail(ch))
            sp, bp, _ = W.shape_metric(dp_, mp_)
            # direction of the inclusive disagreement at its driver bin (MC above / below DATA, unit area)
            def sgn(dd, mm, b):
                if b is None: return 0
                q2 = dd >= W.MIN_DATA_ENTRIES; return int(np.sign(mm[b] / mm[q2].sum() - dd[b] / dd[q2].sum()))
            cellrows[c] = {"edge_worst": worst, "edge_pattern": pattern, "edge_n_data": float(d[win].sum()),
                           "edge_data_over_mc": [float(d[i] / m[i]) if m[i] > 0 else None for i in np.where(win)[0]],
                           "incl_eta_dev": se, "incl_eta_dir": sgn(di, mi, be), "incl_eplug_dev": sp, "incl_eplug_dir": sgn(dp_, mp_, bp)}
        h4[ch] = cellrows
        det_alt[ch] = any(r["edge_worst"] is not None and r["edge_worst"] >= 0.15 and r["edge_pattern"] for r in cellrows.values())
        phys_alt[ch] = all(r["edge_worst"] is not None and r["edge_worst"] <= 0.10 for r in cellrows.values()) and \
            all((r["incl_eta_dev"] or 0) > 0.10 and (r["incl_eplug_dev"] or 0) > 0.10 for r in cellrows.values())
        false_alt[ch] = all((r["incl_eta_dev"] or 0) <= 0.10 and (r["incl_eplug_dev"] or 0) <= 0.10 for r in cellrows.values())
    true_det = det_alt["7071"] and det_alt["7091"]
    same_dir = all(h4["7071"][c]["incl_eta_dir"] == h4["7091"][c]["incl_eta_dir"] and h4["7071"][c]["incl_eplug_dir"] == h4["7091"][c]["incl_eplug_dir"] for c in CELLS)
    true_phys = (not true_det) and phys_alt["7071"] and phys_alt["7091"] and same_dir
    reading = "true-detector" if true_det else ("true-physics" if true_phys else ("false" if (false_alt["7071"] or false_alt["7091"]) else "not supported (between)"))
    h4["reading"] = reading; h4["det_alt"] = det_alt; h4["phys_alt"] = phys_alt; h4["false_alt"] = false_alt
    res["H4"] = h4
    lines.append("H4 %s: edge worst |MC/DATA-1| D0/D1/D2 7071 %s pattern %s, 7091 %s pattern %s (>=0.15 + pattern, both); incl eta dev 7071 %s; ND share 7071 %s (>=0.90)"
                 % (reading, [None if h4["7071"][c]["edge_worst"] is None else round(h4["7071"][c]["edge_worst"], 3) for c in CELLS],
                    [h4["7071"][c]["edge_pattern"] for c in CELLS],
                    [None if h4["7091"][c]["edge_worst"] is None else round(h4["7091"][c]["edge_worst"], 3) for c in CELLS],
                    [h4["7091"][c]["edge_pattern"] for c in CELLS],
                    [None if h4["7071"][c]["incl_eta_dev"] is None else round(h4["7071"][c]["incl_eta_dev"], 3) for c in CELLS],
                    {c: None if v is None else round(v, 3) for c, v in share.items()}))
    # ---------------- H2 ----------------
    lumi = {}
    for r in csv.DictReader(open(a.runlist)):
        try:
            lumi[int(r["run"])] = float(r["lumi_per_run_nb"])
        except (ValueError, KeyError):
            pass
    h2 = {"periods": {}}
    trend = False
    for k in W.DATA_LABELS:
        pr = P[k]["wpa2"]["per_run"]; runs = np.asarray(pr["runs"]); n = np.asarray(pr["n_ctrl"], float)
        L = np.asarray([lumi.get(int(r), np.nan) for r in runs])
        lumi_src = "lumi_per_run_nb"
        if not np.isfinite(L).any():   # column empty in the certified run list: fall back to the A_pre count per run
            L = np.asarray(pr["n_pre"], float); lumi_src = "n_pre per run (proxy; lumi_per_run_nb empty)"
        h2["lumi_source"] = lumi_src
        row = {}
        for det in ("plug_fail", "fmd_fail", "fts_fail"):
            f = np.asarray(pr[det], float)
            for order_name, order in (("run", np.argsort(runs)), ("lumi", np.argsort(L, kind="stable"))):
                nn, ff = n[order], f[order]; cs = np.cumsum(nn); tot = cs[-1]
                if tot < 50:
                    continue
                b = np.clip(np.floor((cs - 0.5 * nn) / (tot / 5)), 0, 4).astype(int)  # block of each run's control midpoint
                bn = np.bincount(b, weights=nn, minlength=5); bf = np.bincount(b, weights=ff, minlength=5)
                p0 = bf.sum() / bn.sum(); rate = bf / np.maximum(bn, 1); var_ = p0 * (1 - p0) / np.maximum(bn, 1)
                ch2 = float(((rate - p0) ** 2 / np.maximum(var_, 1e-12)).sum()); pv = float(CHI2.sf(ch2, 4))
                x = np.arange(5) - 2.0; wgt = 1 / np.maximum(var_, 1e-12)
                slope = float((wgt * x * (rate - p0)).sum() / (wgt * x * x).sum()); zs = slope / math.sqrt(1 / (wgt * x * x).sum())
                row["%s|%s" % (det, order_name)] = {"block_rate": rate.tolist(), "block_n": bn.tolist(), "chi2": ch2, "p": pv, "slope_z": zs}
                trend |= (pv < 0.0027) or (abs(zs) > 3)
        h2["periods"][k] = row
    layer = {}
    layer_specific = False
    for ch in ("7071", "7091"):
        for det, key in (("fmd", "fmd_layers_ctrl"), ("fts", "fts_stations_ctrl")):
            for g in W.PERIODS:
                dd = [P[k]["wpa2"]["control"]["per_group"].get(g, {}).get(key) for k in W.DATA_LABELS]
                dd = [x for x in dd if x]
                if not dd or sum(x["den"]["raw"] for x in dd) == 0:
                    continue
                dden = sum(x["den"]["raw"] for x in dd); nl = len(dd[0]["hit"])
                dhit = np.array([sum(x["hit"][i]["raw"] for x in dd) for i in range(nl)], float)
                mm = [(rs(l, ch) * proj_share(l, ch, g), P[l]["wpa2"]["control"]["per_group"]["ALL"][key]) for l in cocktail(ch)]
                mm = [(s, x) for s, x in mm if x and s > 0]
                mden = sum(s * x["den"]["signed"] for s, x in mm)
                if mden <= 0:
                    continue
                mhit = np.array([sum(s * x["hit"][i]["signed"] for s, x in mm) for i in range(nl)])
                mw2 = np.array([sum(s_ * s_ * x["hit"][i]["sumw2"] for s_, x in mm) for i in range(nl)])
                fd, fm = dhit / dden, mhit / mden; exc = fd - fm
                err = np.sqrt(np.maximum(fd * (1 - fd) / dden + mw2 / mden ** 2, 1e-12))  # DATA binomial + MC sumw2 (layers treated as independent; they are not)
                act = fd > 0.005
                if act.sum() < 2:
                    continue
                wv = 1 / err[act] ** 2; mean = float((wv * exc[act]).sum() / wv.sum())
                ch2 = float((wv * (exc[act] - mean) ** 2).sum()); pv = float(CHI2.sf(ch2, int(act.sum()) - 1))
                z_ = exc / np.sqrt(err ** 2 + 1e-12)
                layer["%s|%s|%s" % (ch, det, g)] = {"data_frac": fd.tolist(), "mc_frac": fm.tolist(), "excess": exc.tolist(),
                                                    "n_layers_excess_gt3sigma": int((act & (z_ > 3)).sum()), "n_layers_deficit_gt3sigma": int((act & (z_ < -3)).sum()),
                                                    "mean_excess": mean, "chi2": ch2, "ndf": int(act.sum()) - 1, "p": pv}
                layer_specific |= pv < 0.0027
    h2["layers"] = layer; h2["trend"] = trend; h2["layer_specific"] = layer_specific
    h2["context_runlist_columns"] = "veto_fail_rate_PLUG7 / mean_PLUG7 (all parity): context only, size nothing (R2)"
    res["H2"] = h2
    lines.append("H2 %s: run-order/lumi trend (p<0.0027 or |z|>3) %s; layer/station-specific excess %s; min block p %s; min layer p %s"
                 % ("supported" if (trend or layer_specific) else "not supported", trend, layer_specific,
                    "%.2g" % min([v["p"] for r in h2["periods"].values() for v in r.values()] or [float("nan")]),
                    "%.2g" % min([v["p"] for v in layer.values()] or [float("nan")])))
    res["lines"] = lines
    if a.split == "odd":
        lines = ["(odd split: descriptive only, no ruling)"] + lines
    json.dump(res, open(out / ("s2_analysis_%s.json" % a.split), "w"), indent=1)
    for l in lines:
        print(l)
    print("empz_join_coverage below 0.999:", res["empz_join_coverage"]["below_0p999"])


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""WP-A5 propagation (A5-D5, A5-D6): the veto correction as event weights through the per-event-normalised
(1/N_D) dn/dx_F and (1/N_D) dn/dp_T*^2 spectra of the six WP-C signal track skims, frames A and B.  Login node.

Sample: wp-c/study/s<label>.npz (wpc_lib.load, reco selection of wpc_build.py) + wp-a5/cells_<label>.npz; event
weight w = e_w x s_diff(cell, 7071) (fine G1'(c) scale; 1 for cell -1).  Reggeon (6671/6677) has no track skim and is
omitted (deviation from parent step (4), recorded in the verdict).  Per (frame, charge, M_X bin, axis, bin):
S = sum_tracks w' / sum_events(M_X bin) w',  w' = w x veto_weight.weights(variant);  R_var = S_var / S_nom - 1.
syst_veto_rel = sqrt(max(|R_up|, |R_down|)^2 + R_eta^2); masked_veto = syst_veto_rel > 0.05 or not finite (empty bin).
Open Q-B alternative (eta-stepped nominal, half the step as systematic):
syst_veto_rel_etaform = sqrt(max(|S_eta_up/S_eta - 1|, |S_eta_down/S_eta - 1|)^2 + (R_eta/2)^2).
R_nom_vs_uncorr = S_nom / S_zero - 1 (the size of the correction itself, not a systematic).
Diagnostic syst_veto_rel_eta_quad: the per-detector eta terms R_eta_<det> added in quadrature instead of jointly
(the joint eta variant lets the Plug and FMD terms partly cancel); veto_eta_fragile flags (does not mask) a bin that passes
the gate but fails under the quadrature form or under any single-detector eta term (h1-systematics review 2026-10-28).
boot_sigma: std (NaN-skipping) of syst_veto_rel over 200 Poisson(1) event-bootstrap replicas (seed 20261028)."""
import csv, json, sys, warnings
from pathlib import Path
import numpy as np

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE)); sys.path.insert(0, str(HERE.parent / "wp-c" / "study"))
import veto_weight as VW
import wpc_lib as L

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
OD = D / "data/restart_2026-10-27/wp-a5"; MASK = D / "data/restart_2026-10-27/wp-c/mask/mask_start_frame%s_G2_D7.json"
VE = D / "data/restart_2026-10-27/gate_evidence/veto_eff.json"
MXK = ("4-10", "10-20", "20-40"); AX = (("xF", "t_xf", L.XF_EDGES), ("pt2", "t_pt2", L.PT2_EDGES)); CHG = ("h+", "h-")
DETV = [v + "_" + d for d in VW.DETS for v in ("up", "down", "eta", "nominal")]
VARS = ["nominal", "up", "down", "eta", "eta_up", "eta_down", "zero"] + DETV
NBOOT = 200; SEED = 20261028; THR = 0.05
COLS = ["charge", "mx_bin", "axis", "bin", "frame", "lo", "hi", "S_nom", "R_up", "R_down", "R_eta", "syst_veto_rel", "boot_sigma",
        "masked_D7", "masked_veto", "R_nom_vs_uncorr", "R_eta_alone_gt_0p05", "syst_veto_rel_etaform", "masked_veto_etaform", "syst_veto_rel_eta_quad", "masked_veto_eta_quad", "veto_eta_fragile", "n_tracks_raw"] + \
       ["R_%s" % v for v in DETV if not v.startswith("nominal")] + ["R_%s" % v for v in DETV if v.startswith("nominal")]


def sample():
    T, E = L.load()
    cells = [np.load(OD / ("cells_%s.npz" % l)) for l in L.LABELS]
    ei = np.concatenate([np.load(L.BASE + "s%s.npz" % l)["e_idx"] for l in L.LABELS])
    if not np.array_equal(ei, np.concatenate([c["e_idx"] for c in cells])): sys.exit("FATAL: cells_<label> e_idx mismatch")
    cell = np.concatenate([c["cell"] for c in cells]).astype(int); ec = np.concatenate([c["ec08"] for c in cells]).astype(float)
    sc = json.load(open(VE))["fit_scales_fine"]["7071"]; sd = np.array([sc["D0"][0], sc["D1"][0], sc["D2"][0], 1.0])
    w0 = E["e_w"] * sd[np.where(cell >= 0, cell, 3)]
    g = T["t_eglob"]
    if not np.allclose(T["t_w"], E["e_w"][g], rtol=0, atol=0): sys.exit("FATAL: track weight != event weight")
    emb = VW.mx_index(E["e_mx"]); emb = np.where(emb < 3, emb, -1)
    return T, E, cell, ec, w0, g, emb


def spectra(T, g, emb, wv, nevm):
    """wv: event weights (n_ev,) -> dict[(frame, axis)] -> array (2 charge, 3 mx, nb)."""
    den = np.bincount(emb[emb >= 0], weights=wv[emb >= 0], minlength=3)
    out = {}
    for fr in "AB":
        for ax, pre, edges in AX:
            k = nevm[(fr, ax)]; m = k >= 0
            num = np.bincount(k[m], weights=wv[g[m]], minlength=6 * (len(edges) - 1)).reshape(2, 3, -1)
            out[(fr, ax)] = num / den[None, :, None]
    return out


def main():
    np.seterr(invalid="ignore", divide="ignore"); warnings.filterwarnings("ignore", category=RuntimeWarning)
    T, E, cell, ec, w0, g, emb = sample()
    nevm = {}; raw = {}
    chg = (T["t_charge"] < 0).astype(int)
    for fr in "AB":
        for ax, pre, edges in AX:
            _, mb, ob, _ = L.cell_id(T["t_mx"], T[pre + fr], L.MX_EDGES, [edges] * 3)
            k = np.where((mb >= 0) & (ob >= 0), (chg * 3 + mb) * (len(edges) - 1) + ob, -1); nevm[(fr, ax)] = k
            raw[(fr, ax)] = np.bincount(k[k >= 0], minlength=6 * (len(edges) - 1)).reshape(2, 3, -1)
    WV = {v: w0 * VW.weights(E["e_mx"], cell, ec, v) for v in VARS}
    S = {v: spectra(T, g, emb, WV[v], nevm) for v in VARS}

    def syst(Sx):
        R = {v: Sx[v] / Sx["nominal"] - 1 for v in ("up", "down", "eta")}
        sr = np.sqrt(np.maximum(np.abs(R["up"]), np.abs(R["down"])) ** 2 + R["eta"] ** 2)
        return R, sr
    rng = np.random.default_rng(SEED); boot = []
    for _ in range(NBOOT):
        pw = rng.poisson(1.0, size=w0.size).astype(float)
        Sb = {v: spectra(T, g, emb, WV[v] * pw, nevm) for v in ("nominal", "up", "down", "eta")}
        boot.append({k: syst({v: Sb[v][k] for v in Sb})[1] for k in Sb["nominal"]})
    masks = {fr: {(b["charge"], b["mx_bin"], b["axis"], b["bin"]): b["masked"] for b in json.load(open(str(MASK) % fr))["bins"]} for fr in "AB"}
    rows = []
    for fr in "AB":
        for ax, pre, edges in AX:
            k = (fr, ax); Sx = {v: S[v][k] for v in VARS}; R, sr = syst(Sx)
            bs = np.nanstd(np.array([b[k] for b in boot]), axis=0, ddof=1)
            Rz = Sx["nominal"] / Sx["zero"] - 1
            ea = np.maximum(np.abs(Sx["eta_up"] / Sx["eta"] - 1), np.abs(Sx["eta_down"] / Sx["eta"] - 1)); se = np.sqrt(ea ** 2 + (R["eta"] / 2) ** 2)
            for ic, c in enumerate(CHG):
                for im, m in enumerate(MXK):
                    for ib in range(len(edges) - 1):
                        r = {"charge": c, "mx_bin": m, "axis": ax, "bin": ib, "frame": fr, "lo": float(edges[ib]), "hi": float(edges[ib + 1]),
                             "S_nom": float(Sx["nominal"][ic, im, ib]), "R_up": float(R["up"][ic, im, ib]), "R_down": float(R["down"][ic, im, ib]),
                             "R_eta": float(R["eta"][ic, im, ib]), "syst_veto_rel": float(sr[ic, im, ib]), "boot_sigma": float(bs[ic, im, ib]),
                             "masked_D7": bool(masks[fr][(c, m, ax, ib)]), "masked_veto": bool(not np.isfinite(sr[ic, im, ib]) or sr[ic, im, ib] > THR),
                             "R_eta_alone_gt_0p05": bool(abs(R["eta"][ic, im, ib]) > THR), "syst_veto_rel_etaform": float(se[ic, im, ib]),
                             "masked_veto_etaform": bool(not np.isfinite(se[ic, im, ib]) or se[ic, im, ib] > THR), "n_tracks_raw": int(raw[k][ic, im, ib]),
                             "R_nom_vs_uncorr": float(Rz[ic, im, ib])}
                        for v in DETV:
                            r["R_" + v] = float(Sx[v][ic, im, ib] / Sx["nominal"][ic, im, ib] - 1)
                        r["syst_veto_rel_eta_quad"] = float(np.sqrt(max(abs(r["R_up"]), abs(r["R_down"])) ** 2 + sum(r["R_eta_" + d] ** 2 for d in VW.DETS)))
                        r["masked_veto_eta_quad"] = bool(not np.isfinite(r["syst_veto_rel_eta_quad"]) or r["syst_veto_rel_eta_quad"] > THR)
                        r["veto_eta_fragile"] = bool(not r["masked_veto"] and (r["masked_veto_eta_quad"] or max(abs(r["R_eta_" + d]) for d in VW.DETS) > THR))
                        rows.append(r)
    with open(OD / "g1pp_a_table.csv", "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=COLS, extrasaction="ignore"); wr.writeheader()
        for r in rows: wr.writerow({k: ("%.6g" % v if isinstance(v, float) else v) for k, v in r.items()})
    den = {v: [float(x) for x in np.bincount(emb[emb >= 0], weights=WV[v][emb >= 0], minlength=3)] for v in ("nominal", "zero")}
    summ = {fr: {"n_rows": sum(r["frame"] == fr for r in rows), "masked_veto": sum(r["masked_veto"] for r in rows if r["frame"] == fr),
                 "masked_veto_not_D7": sum(r["masked_veto"] and not r["masked_D7"] for r in rows if r["frame"] == fr),
                 "masked_D7": sum(r["masked_D7"] for r in rows if r["frame"] == fr),
                 "retained_after_both": sum(not r["masked_veto"] and not r["masked_D7"] for r in rows if r["frame"] == fr),
                 "max_syst_unmaskedD7": max(r["syst_veto_rel"] for r in rows if r["frame"] == fr and not r["masked_D7"]),
                 "median_syst_unmaskedD7": float(np.median([r["syst_veto_rel"] for r in rows if r["frame"] == fr and not r["masked_D7"]])),
                 "masked_veto_etaform": sum(r["masked_veto_etaform"] for r in rows if r["frame"] == fr),
                 "masked_veto_eta_quad": sum(r["masked_veto_eta_quad"] for r in rows if r["frame"] == fr),
                 "masked_veto_eta_quad_not_D7": sum(r["masked_veto_eta_quad"] and not r["masked_D7"] for r in rows if r["frame"] == fr),
                 "veto_eta_fragile_not_D7": sum(r["veto_eta_fragile"] and not r["masked_D7"] for r in rows if r["frame"] == fr),
                 "eta_alone_gt_0p05": sum(r["R_eta_alone_gt_0p05"] for r in rows if r["frame"] == fr)} for fr in "AB"}
    meta = {"correction": str(VW.VETO_CORRECTION_JSON), "correction_sha256": __import__("hashlib").sha256(VW.VETO_CORRECTION_JSON.read_bytes()).hexdigest(),
            "n_events": int(w0.size), "n_tracks": int(g.size), "labels": L.LABELS, "nboot": NBOOT, "seed": SEED, "threshold": THR,
            "sum_w_events_per_mx": {"skim_e_w_x_sdiff": den["zero"], "nominal": den["nominal"]},
            "sum_w_events_per_mx_raw": [float((w0[emb == i]).sum()) for i in range(3)], "summary": summ}
    json.dump({"meta": meta, "rows": rows}, open(OD / "g1pp_a_table.json", "w"), indent=1)
    print(json.dumps(summ, indent=1))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""CJC-containment study (restart 2026-10-28): one merged v6dif input -> one npz event table.

Events: A_pre (skim_wpa3.py: certified runs, passRecoEnv, Q2s>150, 0.1<Ys<0.9, |hatVtxZ|<35) && (etaMaxClus08<3.3 or
sentinel) -- the COMB/N-1 base of veto_eff_hist.py, before any forward veto and any x_IP/M_X cut.
Weights: signed-v3 recipe copied from skim_wpa3.py (unchanged).
Frame: X rest frame B of the WP-C record (p_X = q_es + xi P, mass = DiffTot mx), x_F Pomeron-positive (u = -q*),
so the current (photon) hemisphere is x_F < 0.  Per event, from ChargedTot rows:
  containment tracks: Charge!=0, Pt>0.15, IsFromPrimary, central or TrackType 1/2, electron cone alpha>=0.2 rad,
    no eta cut; E = sqrt(p^2+m_pi^2).  CJC acceptance 20<theta<160 deg <=> |eta|<ETA_CJC.
  measurement tracks: the WP-C/parent release cuts (|eta|<1.6, NHitDedx>=5, RadLength>=5, |Dca/cosh eta|<2, + above).
Proxy (no hadron list): lab pseudorapidity range of the massless current-hemisphere image (equator circle of the X frame
boosted to the lab, plus whether the lab +-z directions lie in the current hemisphere); y_X = lab rapidity of p_X(B).
Gen (DIFF8 MC only): charged pi/K/p of ChargedTotGen on the X side (y* > -(mirRapUpper+mirRapLower)/2, as wpc_build.py),
frame G2 (q_true + xi P, mass mirMX); same containment and proxy quantities."""
import argparse, json, sys, time
from pathlib import Path
import numpy as np
import uproot

H = Path(__file__).resolve().parent
sys.path.insert(0, str(H.parent / "wp-a2")); sys.path.insert(0, str(H.parent / "wp-c" / "study"))
import n1_wpa2 as N
import xframe_obs as F
X = F.X
ETA_CJC = float(-np.log(np.tan(np.radians(10.0))))  # 1.7354
DIFF8 = {"9012", "9024", "9033", "9155", "9157", "9158", "6671", "6677"}
NPHI = 72


def key(r, e):
    return (np.asarray(r, np.int64).astype(np.uint64) << np.uint64(32)) | np.asarray(e, np.int64).astype(np.uint64)


def eta_of(p):
    pp = np.sqrt(np.sum(p[..., :3] ** 2, -1))
    with np.errstate(all="ignore"):
        return np.arctanh(np.clip(p[..., 2] / pp, -1 + 1e-15, 1 - 1e-15))


def proxy(pX, q, M):
    """Lab eta range [lo, hi] of massless current-hemisphere (x_F<0) directions; y_X."""
    n = pX.shape[0]
    qb = F.boost_to_rest(q, pX, M)
    with np.errstate(all="ignore"):
        u = -qb[:, :3] / np.linalg.norm(qb[:, :3], axis=1)[:, None]
        ref = np.where(np.abs(u[:, 2:3]) < 0.9, np.array([[0, 0, 1.]]), np.array([[1., 0, 0]]))
        e1 = np.cross(u, ref); e1 /= np.linalg.norm(e1, axis=1)[:, None]; e2 = np.cross(u, e1)
        ph = np.linspace(0, 2 * np.pi, NPHI, endpoint=False)
        d = np.cos(ph)[None, :, None] * e1[:, None, :] + np.sin(ph)[None, :, None] * e2[:, None, :]
        v = np.concatenate([d, np.ones((n, NPHI, 1))], -1)
        inv = np.concatenate([-pX[:, :3], pX[:, 3:4]], -1)               # boost X frame -> lab
        lab = F.boost_to_rest(v, np.broadcast_to(inv[:, None, :], v.shape), np.broadcast_to(M[:, None], v.shape[:2]))
        et = eta_of(lab); hi = np.max(et, 1); lo = np.min(et, 1)
        cur = {}
        for s in (1., -1.):
            z = np.broadcast_to(np.array([0, 0, s, 1.]), (n, 4))
            cur[s] = np.sum(F.boost_to_rest(z, pX, M)[:, :3] * u, 1) < 0
        hi = np.where(cur[1.], np.inf, hi); lo = np.where(cur[-1.], -np.inf, lo)
        yX = 0.5 * np.log((pX[:, 3] + pX[:, 2]) / (pX[:, 3] - pX[:, 2]))
    ok = np.isfinite(M) & (M > 0) & np.all(np.isfinite(pX), 1) & np.all(np.isfinite(q), 1)
    return np.where(ok, lo, np.nan), np.where(ok, hi, np.nan), np.where(ok, yX, np.nan)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--kind", choices=("data", "mc"), required=True); ap.add_argument("--label", required=True)
    ap.add_argument("--input", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--runlist"); ap.add_argument("--scale-table")
    ap.add_argument("--step", type=int, default=2_000_000)
    a = ap.parse_args(); t0 = time.time()
    b = ["RunNumber", "EventNumber", "xIp", "mx", "beta", "passRecoEnv", "Q2s", "Ys", "hatVtxZ", "etaMaxClus08",
         "selFmd", "ePlug", "selFts", "nFts28"]
    if a.kind == "mc":
        b += ["weight1", "weight2", "hatXpomGki", "mirMY", "mirMX", "mirRapUpper", "mirRapLower"] + \
             (["hatMYGen", "hatTGki", "hatMXGen"] if a.label in N.PDISS else [])
    f = uproot.open(a.input); d = f["DiffTot"].arrays(b, library="np"); n = len(d["Q2s"])
    EK = key(d["RunNumber"], d["EventNumber"])
    if np.unique(EK).size != n: raise SystemExit("FATAL: (RunNumber, EventNumber) not unique")
    acc = N.accepted(a.runlist) if a.kind == "data" else None
    runok = np.isin(d["RunNumber"], np.fromiter(acc, dtype=d["RunNumber"].dtype)) if acc is not None else np.ones(n, bool)
    ec = np.asarray(d["etaMaxClus08"], float)
    pre = runok & d["passRecoEnv"] & (d["Q2s"] > 150) & (d["Ys"] > .1) & (d["Ys"] < .9) & (np.abs(d["hatVtxZ"]) < 35) & \
        ((ec < 3.3) | (ec < -900))
    # weights: verbatim skim_wpa3.py recipe
    w = np.ones(n)
    if a.kind == "mc":
        scale, expo = N.normrow(a.scale_table, a.label); event = np.asarray(d["weight1"] * d["weight2"], float)
        if a.label == "9012": event[:] = 1.
        if a.label in N.CHARM:
            qi, yi, raw = N.g21(d["Q2s"], d["Ys"], d["hatXpomGki"]); event[:] = 0.
            for qq in range(6):
                for yy in range(3):
                    m = (qi == qq) & (yi == yy) & np.isfinite(raw) & (raw > 0)
                    if m.any(): event[m] = raw[m] / raw[m].mean()
        elif a.label in N.PDISS:
            truth = (d["hatMYGen"] < 1.6) & (np.abs(d["hatTGki"]) < 1) & (d["hatXpomGki"] < .03) & (d["hatMXGen"] > 2)
            event[truth] *= 5.3611
        w = event * scale * expo
    x = d["Q2s"] / (4 * 27.6 * 920 * d["Ys"]); cell = np.full(n, -1, np.int8)
    for i, (ql, qh, xl, xh) in enumerate(N.D): cell[(d["Q2s"] >= ql) & (d["Q2s"] < qh) & (x >= xl) & (x < xh)] = i
    ip = np.nonzero(pre)[0]; K = EK[ip]; srt = np.argsort(K); Ks = K[srt]; ne = ip.size
    mxe = np.asarray(d["mx"], float)[ip]
    # per-event accumulators
    Z = lambda dt=float: np.zeros(ne, dt)
    ev = {k: np.full(ne, np.nan) for k in ("qx", "qy", "qz", "qe", "Ep", "Q2es", "Yes", "Empz", "PVz")}
    acc_ = {k: Z() for k in ("E_cur", "E_cur_in", "E_cur_fwd", "E_cur_bwd", "E_trk")}
    cnt = {k: Z(np.int32) for k in ("n_row", "n_trk", "n_cur", "n_cur_out", "n_cur_fwd", "n_meas", "n_meas_cur")}
    CB = ["RunNumber", "EventNumber", "Pt", "Eta", "Px", "Py", "Pz", "Charge", "NHitDedx", "RadLength", "DcaPrime",
          "IsFromPrimary", "IsCentralTrk", "TrackType", "Q2es", "Yes", "Empz", "PVz", "BeamElecE", "BeamProtP",
          "scatEX", "scatEY", "scatEZ", "scatEE"]
    nrows = 0
    for C in f["ChargedTot"].iterate(CB, library="np", step_size=a.step):
        nrows += len(C["Pt"])
        kc = key(C["RunNumber"], C["EventNumber"]); j = np.clip(np.searchsorted(Ks, kc), 0, max(ne - 1, 0))
        hit = (ne > 0) & (Ks[j] == kc)
        if not hit.any(): continue
        C = {k: np.asarray(v)[hit] for k, v in C.items()}; ie = srt[j[hit]]
        np.add.at(cnt["n_row"], ie, 1)
        Eb = X._beam(C["BeamElecE"], 27.6); Ep = X._beam(C["BeamProtP"], 920.0)
        qx, qy, qz, qe = X.reco_photon(C["Q2es"], C["Yes"], C["scatEX"], C["scatEY"], C["scatEZ"], C["scatEE"], Eb)
        for k, v in (("qx", qx), ("qy", qy), ("qz", qz), ("qe", qe), ("Ep", Ep), ("Q2es", C["Q2es"]), ("Yes", C["Yes"]),
                     ("Empz", C["Empz"]), ("PVz", C["PVz"])):
            ev[k][ie] = np.asarray(v, float)
        q = np.stack([qx, qy, qz, qe], -1).astype(float)
        p3 = np.stack([C["Px"], C["Py"], C["Pz"]], -1).astype(float)
        h = np.concatenate([p3, np.sqrt(np.sum(p3 ** 2, -1) + X.M_PION ** 2)[:, None]], -1)
        pX, _ = F.pX_collinear(q, Ep, mxe[ie]); xf, _, _ = F.frame_obs(h, pX, q, mxe[ie])
        with np.errstate(all="ignore"):
            s3 = np.stack([C["scatEX"], C["scatEY"], C["scatEZ"]], -1).astype(float)
            al = np.arccos(np.clip(np.sum(p3 * s3, -1) / (np.linalg.norm(p3, axis=1) * np.linalg.norm(s3, axis=1)), -1, 1))
            eta = C["Eta"].astype(float)
            base = (C["Charge"] != 0) & (C["Pt"] > 0.15) & (C["IsFromPrimary"] == 1) & \
                ((C["IsCentralTrk"] == 1) | (C["TrackType"] == 1) | (C["TrackType"] == 2)) & np.isfinite(al) & (al >= 0.2)
            meas = base & (np.abs(eta) < 1.6) & (C["NHitDedx"] >= 5) & (C["RadLength"] >= 5.0) & \
                (np.abs(C["DcaPrime"] / np.cosh(eta)) < 2.0)
            cur = base & np.isfinite(xf) & (xf < 0); inn = np.abs(eta) < ETA_CJC
        E = h[:, 3]
        np.add.at(cnt["n_trk"], ie[base], 1); np.add.at(acc_["E_trk"], ie[base], E[base])
        np.add.at(cnt["n_cur"], ie[cur], 1); np.add.at(acc_["E_cur"], ie[cur], E[cur])
        np.add.at(acc_["E_cur_in"], ie[cur & inn], E[cur & inn])
        np.add.at(cnt["n_cur_out"], ie[cur & ~inn], 1)
        np.add.at(cnt["n_cur_fwd"], ie[cur & (eta >= ETA_CJC)], 1); np.add.at(acc_["E_cur_fwd"], ie[cur & (eta >= ETA_CJC)], E[cur & (eta >= ETA_CJC)])
        np.add.at(acc_["E_cur_bwd"], ie[cur & (eta <= -ETA_CJC)], E[cur & (eta <= -ETA_CJC)])
        np.add.at(cnt["n_meas"], ie[meas], 1); np.add.at(cnt["n_meas_cur"], ie[meas & cur], 1)
    q = np.stack([ev["qx"], ev["qy"], ev["qz"], ev["qe"]], -1)
    pX, _ = F.pX_collinear(q, np.where(np.isfinite(ev["Ep"]), ev["Ep"], 920.0), mxe)
    lo, hi, yX = proxy(pX, q, mxe)
    g = lambda k, dt=np.float32: np.asarray(d[k])[ip].astype(dt)
    out = dict(even=(np.asarray(d["EventNumber"], np.int64)[ip] % 2 == 0), cell=cell[ip], w=w[ip], eplug=g("ePlug"),
               fmd=g("selFmd", bool), fts=g("selFts", bool), nfts28=g("nFts28", np.int16), ec08=g("etaMaxClus08"),
               xip=g("xIp"), mx=g("mx"), beta=g("beta"), ys=g("Ys"), q2s=g("Q2s"), eta_lo=lo.astype(np.float32),
               eta_hi=hi.astype(np.float32), yX=yX.astype(np.float32),
               **{k: v.astype(np.float32) for k, v in ev.items() if k in ("Q2es", "Yes", "Empz", "PVz")},
               **{k: v.astype(np.float32) for k, v in acc_.items()}, **cnt)
    info = {"label": a.label, "kind": a.kind, "n_DiffTot": n, "n_pre": int(ne), "n_rows_read": nrows,
            "frac_pre_with_rows": float((cnt["n_row"] > 0).mean()) if ne else None, "eta_cjc": ETA_CJC}
    if a.kind == "mc" and a.label in DIFF8:  # gen-level containment on the X side
        GB = ["RunNumber", "EventNumber", "GenPDG", "GenPt", "GenEta", "GenPhi", "GenElecE", "GenElecTheta", "GenElecPhi",
              "GenEnPhoton", "Q2_es_gen", "Q2sGen", "GenBeamElecE", "GenBeamProtP"]
        gacc = {k: np.zeros(ne) for k in ("gE_cur", "gE_cur_in", "gE_X")}; gn = np.zeros(ne, np.int32)
        gq = np.full((ne, 4), np.nan); gEp = np.full(ne, np.nan)
        mirMX = np.asarray(d["mirMX"], float)[ip]; ymid = -(np.asarray(d["mirRapUpper"], float) + np.asarray(d["mirRapLower"], float))[ip] / 2
        for G in f["ChargedTotGen"].iterate(GB, library="np", step_size=a.step):
            kc = key(G["RunNumber"], G["EventNumber"]); j = np.clip(np.searchsorted(Ks, kc), 0, max(ne - 1, 0))
            hit = (ne > 0) & (Ks[j] == kc)
            if not hit.any(): continue
            G = {k: np.asarray(v)[hit] for k, v in G.items()}; ie = srt[j[hit]]
            gt = X.gen_truth(G, "ChargedTotGen")
            qg = np.stack([gt["qX"], gt["qY"], gt["qZ"], gt["qE"]], -1); gq[ie] = qg; gEp[ie] = gt["Ep"]
            with np.errstate(all="ignore"):
                ok = np.isfinite(G["GenEta"]) & (np.abs(G["GenEta"]) < 1e3) & (G["GenPt"] > 0)
                gh = F.hadron4(G["GenPt"], np.where(ok, G["GenEta"], 0.), G["GenPhi"], X.gen_mass(np.abs(G["GenPDG"].astype(np.int64))))
                ys = F.ystar_photon_axis(gh, qg, gt["Ep"]); inX = ok & (ys > ymid[ie])
                pXg, _ = F.pX_collinear(qg, gt["Ep"], mirMX[ie]); xf, _, _ = F.frame_obs(gh, pXg, qg, mirMX[ie])
                cur = inX & np.isfinite(xf) & (xf < 0); inn = np.abs(G["GenEta"]) < ETA_CJC
            E = gh[:, 3]
            np.add.at(gacc["gE_X"], ie[inX], E[inX]); np.add.at(gacc["gE_cur"], ie[cur], E[cur])
            np.add.at(gacc["gE_cur_in"], ie[cur & inn], E[cur & inn]); np.add.at(gn, ie[cur], 1)
        pXg, _ = F.pX_collinear(gq, np.where(np.isfinite(gEp), gEp, 920.0), mirMX)
        glo, ghi, gyX = proxy(pXg, gq, mirMX)
        out.update({k: v.astype(np.float32) for k, v in gacc.items()}, gn_cur=gn, g_eta_hi=ghi.astype(np.float32),
                   g_eta_lo=glo.astype(np.float32), mirmx=mirMX.astype(np.float32), mirmy=g("mirMY"))
        info["frac_pre_with_gen_frame"] = float(np.isfinite(ghi).mean()) if ne else None
    Path(a.out).parent.mkdir(parents=True, exist_ok=True); np.savez_compressed(a.out, **out)
    info["elapsed_s"] = round(time.time() - t0, 1); print(json.dumps(info))


if __name__ == "__main__":
    main()

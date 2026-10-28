#!/usr/bin/env python3
"""User-grid (2026-10-28) data track skim: one merged data file -> npz of selected events + selected tracks.
Selection = the WP-C study selection of wpc_build.py applied to data (certified runs, passRecoEnv, Q2es>150, 0.1<Yes<0.7,
35<Empz<70, |PVz|<35, stack C: selFmd, ePlug<7, selFts, etaMaxClus08 < 3.2 or sentinel, 0<xIp<0.04, mx>3) and the same
release track cuts.  Looser edges (ec08<3.2, xIp<0.04) are kept so the analysis stage applies D15/x_IP<0.03/4<=M_X<=40
identically to data and MC.  Frames A and B observables as in wpc_build.py; z_lab computed directly in the lab."""
import argparse, json, sys, time
import numpy as np, uproot
W = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/"
sys.path.insert(0, W + "wp-c/study"); sys.path.insert(0, W + "wp-a2")
import xframe_obs as F
import n1_wpa2 as N
X = F.X

def key(r, e):
    return (np.asarray(r, np.int64).astype(np.uint64) << np.uint64(32)) | np.asarray(e, np.int64).astype(np.uint64)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--label", required=True); ap.add_argument("--input", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--runlist", required=True); ap.add_argument("--step", type=int, default=1_000_000)
    a = ap.parse_args(); t0 = time.time()
    b = ["RunNumber", "EventNumber", "passRecoEnv", "ePlug", "selFmd", "selFts", "etaMaxClus08", "phE", "phPx", "phPy", "phPz",
         "mx", "beta", "xIp"]
    f = uproot.open(a.input); d = f["DiffTot"].arrays(b, library="np"); n = len(d["mx"])
    EK = key(d["RunNumber"], d["EventNumber"])
    if np.unique(EK).size != n: raise SystemExit("FATAL: (Run, Event) not unique")
    acc = N.accepted(a.runlist)
    ec = np.asarray(d["etaMaxClus08"], float)
    with np.errstate(invalid="ignore"):
        pre = np.isin(d["RunNumber"], np.fromiter(acc, dtype=d["RunNumber"].dtype)) & d["passRecoEnv"].astype(bool) & \
            ((ec < 3.2) | (ec < -900)) & d["selFmd"].astype(bool) & (d["ePlug"] < 7) & d["selFts"].astype(bool) & \
            (d["xIp"] > 0) & (d["xIp"] < 0.04) & (d["mx"] > 3)
    ip = np.nonzero(pre)[0]; K = EK[ip]; srt = np.argsort(K); Ks = K[srt]; ne = ip.size
    ev = {k: np.full(ne, np.nan) for k in ("Q2es", "Yes", "Empz", "PVz")}
    CB = ["RunNumber", "EventNumber", "Pt", "Eta", "Phi", "Px", "Py", "Pz", "Charge", "NHitDedx", "RadLength", "DcaPrime",
          "IsFromPrimary", "IsCentralTrk", "TrackType", "Q2es", "Yes", "Empz", "PVz", "BeamElecE", "BeamProtP",
          "scatEX", "scatEY", "scatEZ", "scatEE"]
    T = {k: [] for k in ("ev", "charge", "pt", "eta", "xfA", "pt2A", "xfB", "pt2B", "zlab")}
    mxe = np.asarray(d["mx"], float)[ip]
    for C in f["ChargedTot"].iterate(CB, library="np", step_size=a.step):
        kc = key(C["RunNumber"], C["EventNumber"]); j = np.clip(np.searchsorted(Ks, kc), 0, max(ne - 1, 0))
        hit = (ne > 0) & (Ks[j] == kc)
        if not hit.any(): continue
        C = {k: np.asarray(v)[hit] for k, v in C.items()}; ie = srt[j[hit]]
        for k in ev: ev[k][ie] = np.asarray(C[k], float)
        with np.errstate(all="ignore"):
            evok = (C["Q2es"] > 150) & (C["Yes"] > .1) & (C["Yes"] < .7) & (C["Empz"] > 35) & (C["Empz"] < 70) & (np.abs(C["PVz"]) < 35)
            p3 = np.stack([C["Px"], C["Py"], C["Pz"]], -1).astype(float); pm = np.sqrt(np.sum(p3 ** 2, -1))
            s3 = np.stack([C["scatEX"], C["scatEY"], C["scatEZ"]], -1).astype(float); sm = np.sqrt(np.sum(s3 ** 2, -1))
            alpha = np.arccos(np.clip(np.where((pm > 0) & (sm > 0), np.sum(p3 * s3, -1) / (pm * sm), np.nan), -1, 1))
            dca = np.abs(C["DcaPrime"].astype(float) / np.cosh(C["Eta"].astype(float)))
            trk = (C["Pt"] > 0.15) & (np.abs(C["Eta"]) < 1.6) & (C["NHitDedx"] >= 5) & (C["RadLength"] >= 5.0) & (dca < 2.0) & \
                (C["IsFromPrimary"] == 1) & ((C["IsCentralTrk"] == 1) | (C["TrackType"] == 1) | (C["TrackType"] == 2)) & \
                np.isfinite(alpha) & (alpha >= 0.2) & (C["Charge"] != 0) & evok
        if not trk.any(): continue
        C = {k: v[trk] for k, v in C.items()}; ie = ie[trk]; p3 = p3[trk]
        Eb = X._beam(C["BeamElecE"], 27.6); Ep = X._beam(C["BeamProtP"], 920.0)
        h = np.concatenate([p3, np.sqrt(np.sum(p3 ** 2, -1) + X.M_PION ** 2)[:, None]], -1)
        qx, qy, qz, qe = X.reco_photon(C["Q2es"], C["Yes"], C["scatEX"], C["scatEY"], C["scatEZ"], C["scatEE"], Eb)
        q = np.stack([qx, qy, qz, qe], -1).astype(float)
        pXA = np.stack([d["phPx"][ip][ie], d["phPy"][ip][ie], d["phPz"][ip][ie], d["phE"][ip][ie]], -1).astype(float)
        xfA, pt2A, _ = F.frame_obs(h, pXA, q)
        pXB, _ = F.pX_collinear(q, Ep, mxe[ie]); xfB, pt2B, _ = F.frame_obs(h, pXB, q, mxe[ie])
        P = F.proton4(Ep)
        zl = F.mdot(P, h) / F.mdot(P, q)
        for k, v in (("ev", ie), ("charge", C["Charge"]), ("pt", C["Pt"]), ("eta", C["Eta"]), ("xfA", xfA), ("pt2A", pt2A),
                     ("xfB", xfB), ("pt2B", pt2B), ("zlab", zl)):
            T[k].append(np.asarray(v))
    T = {"t_" + k: np.concatenate(v) if v else np.zeros(0) for k, v in T.items()}
    T["t_charge"] = T["t_charge"].astype(np.int8); T["t_ev"] = T["t_ev"].astype(np.int64)
    with np.errstate(invalid="ignore"):
        esel = np.isfinite(ev["Q2es"]) & (ev["Q2es"] > 150) & (ev["Yes"] > .1) & (ev["Yes"] < .7) & (ev["Empz"] > 35) & (ev["Empz"] < 70) & (np.abs(ev["PVz"]) < 35)
    # remap track event index to the selected-event table
    new = np.cumsum(esel) - 1
    assert np.all(esel[T["t_ev"]]); T["t_ev"] = new[T["t_ev"]]
    g = lambda k: np.asarray(d[k], float)[ip][esel]
    E = dict(e_mx=g("mx"), e_beta=g("beta"), e_xIp=g("xIp"), e_ec08=g("etaMaxClus08"), e_Q2es=ev["Q2es"][esel], e_Yes=ev["Yes"][esel])
    with np.errstate(invalid="ignore"):
        m2 = (d["phE"].astype(float) ** 2 - d["phPx"] ** 2 - d["phPy"] ** 2 - d["phPz"] ** 2)[ip][esel]
        E["e_mxA"] = np.where(m2 > 0, np.sqrt(np.where(m2 > 0, m2, np.nan)), np.nan)
    np.savez_compressed(a.out, **T, **E)
    print(json.dumps(dict(label=a.label, n_DiffTot=n, n_pre=int(ne), n_events=int(esel.sum()), n_tracks=int(len(T["t_pt"])),
                          elapsed_s=round(time.time() - t0, 1))))

if __name__ == "__main__":
    main()

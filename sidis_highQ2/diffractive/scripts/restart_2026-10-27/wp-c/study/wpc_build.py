#!/usr/bin/env python3
"""WP-C study per-sample builder: one merged MC file -> compact npz (event table + selected-track table).

Selection (reco):  passRecoEnv; Q2es>150, 0.1<Yes<0.7, Empz in [35,70] (flag for Empz>45 variant kept), |PVz|<35
  (ChargedTot first row of the event = parent cuts); LRG stack C (t4 build_localization_n1_mytbl_2026-10-20.py:115,
  :140-141: etaMaxClus08<3.2 or <-900, selFmd, ePlug<7, selFts); reco xIp<0.04, mx>3 (D4).
Reco track cuts = release 'zlow005_pT5_eta1p6_radlen5_nhit5' OPT-A builder
  cone_s2prime_optA_2026-10-19/build_migration_th2_per_charge_cone_s2prime_optA_esxf.py:665-669 + :1350-1360, :1423:
  Pt>0.15 (no Pt_max), |Eta|<1.6, NHitDedx>=5, RadLength>=5, |DcaPrime/cosh(Eta)|<2, IsFromPrimary==1,
  (IsCentralTrk==1 | TrackType in {1,2}), electron-alpha cone >=0.2 rad.  Truth flags are NOT used for reco selection.
Weights: composite_scale_table_v3 recipe, mirrored from t4 script (w_event * scale_to_data * exposure_factor).
"""
import argparse, csv, json, sys, time
import numpy as np
import uproot

sys.path.insert(0, "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
import xframe_obs as F
X = F.X

TABLE = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/docs/mc/composite_scale_table_v3_2026-10-20.csv"
CHARM = {"9024", "9033"}; PDISS = {"9155", "9157", "9158"}
POOL = (211, 321, 2212)

DB = ["RunNumber", "EventNumber", "passRecoEnv", "weight1", "weight2", "Q2s", "Ys", "ePlug", "selFmd", "selFts",
      "etaMaxClus08", "phE", "phPx", "phPy", "phPz", "phM2", "yh", "yes", "mx", "mx2Ye", "beta", "xIp", "hatXpom",
      "hatMXGen", "hatXpomGki", "mirFound", "mirMX", "mirMY", "mirT", "mirXip", "mirBeta", "mirRapUpper", "mirRapLower"]
CB = ["RunNumber", "EventNumber", "Pt", "Eta", "Phi", "Px", "Py", "Pz", "Charge", "NHitDedx", "RadLength", "DcaPrime",
      "IsFromPrimary", "IsCentralTrk", "TrackType", "Q2es", "Yes", "Empz", "PVz", "BeamElecE", "BeamProtP",
      "scatEX", "scatEY", "scatEZ", "scatEE", "IsMatched", "IsMatchedHadron", "IsMatchedElectron",
      "IsMatchedV0Daughter", "IsMatchedConvOrDalitz", "VetoAncestorPDG", "GenPDG", "GenPt", "GenEta", "GenPhi",
      "Q2sGen", "YsGen", "Q2_es_gen", "GenEnPhoton", "GenElecE", "GenElecTheta", "GenElecPhi"]


def key(r, e):
    return r.astype(np.int64) * 10**10 + e.astype(np.int64)


def g21(q, y, x):
    qi = np.searchsorted([150, 200, 300, 500, 700, 1000, np.inf], q, side="right") - 1
    yi = np.searchsorted([.10, .25, .45, .70], y, side="right") - 1
    return qi, yi, np.where(x > 0, x ** -1.226, 0.)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--label", required=True); ap.add_argument("--input", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--table", default=TABLE)  # WP-D2: 840x uses data/restart_2026-10-27/wp-d2/scale_table_840x.csv
    a = ap.parse_args()
    t0 = time.time()
    f = uproot.open(a.input)
    D = {k: np.asarray(v) for k, v in f["DiffTot"].arrays(DB, library="np").items()}
    C = {k: np.asarray(v) for k, v in f["ChargedTot"].arrays(CB, library="np").items()}
    nD = len(D["Q2s"]); nC = len(C["Pt"])
    rows = [r for r in csv.DictReader(open(a.table)) if r["component"] == a.label]
    assert len(rows) == 1
    scale, expo = float(rows[0]["scale_to_data"]), float(rows[0]["exposure_factor"])

    kd = key(D["RunNumber"], D["EventNumber"]); kc = key(C["RunNumber"], C["EventNumber"])
    assert len(np.unique(kd)) == nD
    srt = np.argsort(kd); pos = np.searchsorted(kd[srt], kc)
    assert np.all(kd[srt][pos] == kc), "ChargedTot event missing in DiffTot"
    ic = srt[pos]                                   # DiffTot row of each track row
    # first ChargedTot row per event -> event-level reco/gen quantities
    first = np.full(nD, -1);
    order = np.arange(nC)[::-1]; first[ic[order]] = order      # smallest row index wins
    hasC = first >= 0
    fr = np.where(hasC, first, 0)
    ev = {k: np.where(hasC, C[k][fr].astype(np.float64), np.nan) for k in
          ("Q2es", "Yes", "Empz", "PVz", "BeamElecE", "BeamProtP", "Q2sGen", "YsGen")}

    # ---- event weight (recipe mirrored from t4 script) ----
    w_ev = (D["weight1"] * D["weight2"]).astype(np.float64)
    if a.label == "9012":
        w_ev[:] = 1.
    if a.label in CHARM:
        qi, yi, raw = g21(D["Q2s"], D["Ys"], D["hatXpomGki"]); w_ev[:] = 0.
        for q in range(6):
            for y in range(3):
                m = (qi == q) & (yi == y) & np.isfinite(raw) & (raw > 0)
                if m.any():
                    w_ev[m] = raw[m] / raw[m].mean()
    if a.label in PDISS:
        with np.errstate(invalid="ignore"):
            tic = (ev["Q2sGen"] >= 150) & (ev["YsGen"] >= .1) & (ev["YsGen"] <= .7) & (D["mirMY"] < 1.6) & \
                  (np.abs(D["mirT"]) < 1) & (D["mirXip"] < .03) & (D["mirMX"] > 2)
        w_ev[tic & hasC] *= 5.3611
        n_tic_noC = int((~hasC & (D["mirMY"] < 1.6) & (np.abs(D["mirT"]) < 1) & (D["mirXip"] < .03) & (D["mirMX"] > 2)).sum())
    w_fill = w_ev * scale * expo

    # ---- reco event selection ----
    cut = {}
    s = D["passRecoEnv"].astype(bool); cut["passRecoEnv"] = int(s.sum())
    s &= hasC; cut["has_ChargedTot_rows"] = int(s.sum())
    with np.errstate(invalid="ignore"):
        s &= (ev["Q2es"] > 150) & (ev["Yes"] > .1) & (ev["Yes"] < .7); cut["+Q2es,Yes"] = int(s.sum())
        s_empz45 = s & (ev["Empz"] > 45)
        s &= (ev["Empz"] > 35) & (ev["Empz"] < 70); cut["+Empz35-70"] = int(s.sum())
        s &= np.abs(ev["PVz"]) < 35; cut["+|PVz|<35"] = int(s.sum())
        s_empz45 &= np.abs(ev["PVz"]) < 35
        ec = D["etaMaxClus08"].astype(np.float64)
        s &= (ec < 3.2) | (ec < -900); cut["+etaMaxClus08<3.2"] = int(s.sum())
        s &= D["selFmd"].astype(bool) & (D["ePlug"] < 7) & D["selFts"].astype(bool); cut["+FMD,Plug<7,FTS"] = int(s.sum())
        s &= (D["xIp"] < 0.04) & (D["xIp"] > 0); cut["+xIp<0.04"] = int(s.sum())
        s &= D["mx"] > 3; cut["+mx>3"] = int(s.sum())
    sel_ev = s

    # ---- gen event quantities (true gamma*, ISR-only) from the first row ----
    gb = {k: np.where(hasC, C[k][fr].astype(np.float64), np.nan) for k in
          ("GenElecE", "GenElecTheta", "GenElecPhi", "GenEnPhoton", "Q2_es_gen", "Q2sGen", "BeamElecE", "BeamProtP")}
    gt = X.gen_truth(gb, "ChargedTot")
    Q2t, yt = gt["Q2"], gt["y"]
    with np.errstate(invalid="ignore"):
        genfid_ev = (D["mirFound"].astype(bool) & (D["mirMY"] < 1.6) & (np.abs(D["mirT"]) < 1.0) & (D["mirXip"] < 0.03) &
                     (D["mirMX"] >= 4) & (D["mirMX"] <= 40) & (Q2t > 150) & (yt >= .1) & (yt <= .7) & hasC)

    # ---- track selection ----
    rr = np.arange(nC)
    with np.errstate(all="ignore"):
        ch = np.cosh(C["Eta"].astype(np.float64)); dca = np.abs(C["DcaPrime"].astype(np.float64) / ch)
        alpha = np.arccos(np.clip(np.where(
            (np.sqrt(C["Px"].astype(float)**2 + C["Py"]**2 + C["Pz"]**2) > 0) & (np.sqrt(C["scatEX"].astype(float)**2 + C["scatEY"]**2 + C["scatEZ"]**2) > 0),
            (C["Px"].astype(float) * C["scatEX"] + C["Py"] * C["scatEY"] + C["Pz"] * C["scatEZ"]) /
            (np.sqrt(C["Px"].astype(float)**2 + C["Py"]**2 + C["Pz"]**2) * np.sqrt(C["scatEX"].astype(float)**2 + C["scatEY"]**2 + C["scatEZ"]**2)), np.nan), -1, 1))
        trk = ((C["Pt"] > 0.15) & (np.abs(C["Eta"]) < 1.6) & (C["NHitDedx"] >= 5) & (C["RadLength"] >= 5.0) & (dca < 2.0) &
               (C["IsFromPrimary"] == 1) & ((C["IsCentralTrk"] == 1) | (C["TrackType"] == 1) | (C["TrackType"] == 2)) &
               np.isfinite(alpha) & (alpha >= 0.2) & (C["Charge"] != 0))
    cut["tracks_all_rows"] = nC
    selt = trk & sel_ev[ic]
    cut["tracks_selected"] = int(selt.sum())
    cut["tracks_selected_h+"] = int((selt & (C["Charge"] > 0)).sum()); cut["tracks_selected_h-"] = int((selt & (C["Charge"] < 0)).sum())
    cut["tracks_selected_Empz45"] = int((trk & s_empz45[ic] & sel_ev[ic]).sum())
    ti = rr[selt]; ie = ic[ti]                      # track rows, event rows

    Eb = X._beam(C["BeamElecE"][ti], 27.6); Ep = X._beam(C["BeamProtP"][ti], 920.0)
    h_reco = np.stack([C["Px"][ti], C["Py"][ti], C["Pz"][ti]], -1).astype(np.float64)
    h_reco = np.concatenate([h_reco, np.sqrt(np.sum(h_reco**2, -1) + X.M_PION**2)[:, None]], -1)
    qx, qy, qz, qe = X.reco_photon(C["Q2es"][ti], C["Yes"][ti], C["scatEX"][ti], C["scatEY"][ti], C["scatEZ"][ti], C["scatEE"][ti], Eb)
    q_rec = np.stack([qx, qy, qz, qe], -1)
    pXA = np.stack([D["phPx"][ie], D["phPy"][ie], D["phPz"][ie], D["phE"][ie]], -1).astype(np.float64)
    xfA, pt2A, MA = F.frame_obs(h_reco, pXA, q_rec)
    mxev = D["mx"][ie].astype(np.float64)
    pXB, xiB = F.pX_collinear(q_rec, Ep, mxev)
    xfB, pt2B, MB = F.frame_obs(h_reco, pXB, q_rec, mxev)

    # ---- gen side for the same tracks (matched gen hadron stored on the reco row) ----
    pdg = np.abs(C["GenPDG"][ti].astype(np.int64))
    gh = np.stack([np.zeros(len(ti))] * 4, -1)
    gpt, geta, gphi = (C[k][ti].astype(np.float64) for k in ("GenPt", "GenEta", "GenPhi"))
    gh = F.hadron4(gpt, geta, gphi, X.gen_mass(pdg))
    gbt = {k: C[k][ti].astype(np.float64) for k in ("GenElecE", "GenElecTheta", "GenElecPhi", "GenEnPhoton", "Q2_es_gen", "Q2sGen", "BeamElecE", "BeamProtP")}
    gtt = X.gen_truth(gbt, "ChargedTot")
    q_gen = np.stack([gtt["qX"], gtt["qY"], gtt["qZ"], gtt["qE"]], -1); Epg = gtt["Ep"]
    mirXip, mirMX = D["mirXip"][ie].astype(np.float64), D["mirMX"][ie].astype(np.float64)
    pX1 = F.pX_xip(q_gen, Epg, mirXip)
    xf1, pt21, M1 = F.frame_obs(gh, pX1, q_gen)
    pX2, _ = F.pX_collinear(q_gen, Epg, mirMX)
    xf2, pt22, M2 = F.frame_obs(gh, pX2, q_gen, mirMX)
    # ---- F3 diagnostics: frame-B p_T*^2 with reco/gen ingredients swapped (mx = reco event M_X in all) ----
    def pt2B_(h, qq, Epp):
        pXx, _ = F.pX_collinear(qq, Epp, mxev)
        return F.frame_obs(h, pXx, qq, mxev)[1]
    f3_base = pt2B_(h_reco, q_rec, Ep)
    f3_gphot = pt2B_(h_reco, q_gen, Epg)                                   # (i) gen photon, reco track
    f3_gtrk = pt2B_(F.hadron4(gpt, geta, gphi, X.M_PION), q_rec, Ep)       # (ii) gen momentum (pion mass), reco photon
    hm = np.concatenate([h_reco[:, :3], np.sqrt(np.sum(h_reco[:, :3]**2, -1) + X.gen_mass(pdg)**2)[:, None]], -1)
    f3_gmass = pt2B_(hm, q_rec, Ep)                                        # (iii) gen PDG mass on reco track
    f3_allgen = pt2B_(gh, q_gen, Epg)                                      # gen track+gen photon, reco mx
    ys = F.ystar_photon_axis(gh, q_gen, Epg)
    ymid = -(D["mirRapUpper"][ie].astype(np.float64) + D["mirRapLower"][ie].astype(np.float64)) / 2.0
    inX = ys > ymid
    with np.errstate(invalid="ignore"):
        is_had = (C["IsMatched"][ti] == 1) & (C["IsMatchedHadron"][ti] == 1) & (C["IsMatchedElectron"][ti] == 0) & np.isin(pdg, POOL)
        veto = (C["VetoAncestorPDG"][ti] != 0) | (C["IsMatchedV0Daughter"][ti] != 0) | (C["IsMatchedConvOrDalitz"][ti] != 0)
        ps_ok = (np.abs(geta) < 1.6) & (gpt >= 0.15) & (gpt <= 15.0)
        evfid = genfid_ev[ie]
    why = np.zeros(len(ti), np.int8)
    why[~is_had] |= 1; why[is_had & veto] |= 2; why[~ps_ok] |= 4; why[~inX] |= 8; why[~evfid] |= 16
    gv = (why == 0) & np.isfinite(xf1) & np.isfinite(pt21)

    out = dict(
        t_ev=ie.astype(np.int64), t_charge=C["Charge"][ti].astype(np.int8), t_w=w_fill[ie], t_why=why, t_gv=gv,
        t_mx=mxev, t_mxA=MA, t_xfA=xfA, t_pt2A=pt2A, t_xfB=xfB, t_pt2B=pt2B,
        t_gmx=mirMX, t_xf1=xf1, t_pt21=pt21, t_xf2=xf2, t_pt22=pt22, t_M1=M1, t_ystar=ys, t_ymid=ymid,
        t_f3_base=f3_base, t_f3_gphot=f3_gphot, t_f3_gtrk=f3_gtrk, t_f3_gmass=f3_gmass, t_f3_allgen=f3_allgen,
        t_pdg=pdg.astype(np.int32), t_gpt=gpt, t_geta=geta, t_pt=C["Pt"][ti].astype(np.float64),
        # event table: all selected events
        e_idx=np.nonzero(sel_ev)[0],
    )
    ei = out["e_idx"]
    for k in ("mx", "yh", "yes", "mx2Ye", "beta", "xIp", "phM2", "hatXpom", "hatMXGen", "mirMX", "mirMY", "mirT", "mirXip", "mirBeta"):
        out["e_" + k] = D[k][ei].astype(np.float64)
    out.update(e_w=w_fill[ei], e_genfid=genfid_ev[ei], e_Q2t=Q2t[ei], e_yt=yt[ei], e_Q2es=ev["Q2es"][ei], e_Yes=ev["Yes"][ei],
               e_empz45=s_empz45[ei])
    # all events passing gen fiducial but any reco status (for event-level acceptance): counts only
    cut["gen_fid_events_with_rows"] = int(genfid_ev.sum()); cut["sel_events"] = int(sel_ev.sum())
    cut["sel_events_genfid"] = int((sel_ev & genfid_ev).sum())
    cut["sum_w_sel_events"] = float(w_fill[sel_ev].sum())
    cut["scale_to_data"] = scale; cut["exposure_factor"] = expo; cut["n_events_DiffTot"] = nD
    cut["frac_tracks_gv"] = float(gv.mean()) if len(gv) else 0.
    if a.label in PDISS:
        cut["pdiss_truth_in_cut_without_C_rows"] = n_tic_noC
    cut["elapsed_s"] = time.time() - t0
    np.savez_compressed(a.out + ".npz", **out)
    json.dump(cut, open(a.out + ".json", "w"), indent=1)
    print(json.dumps(cut, indent=1))


if __name__ == "__main__":
    main()

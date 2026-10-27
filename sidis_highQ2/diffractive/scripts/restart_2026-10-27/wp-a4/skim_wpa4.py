#!/usr/bin/env python3
"""WP-A4 (restart 2026-10-27) fork of wp-a3/skim_wpa3.py: selection, weights, D cells, run policy
byte-identical in logic; adds per-event branches Q2s, Ys, x, etaMaxGen/hatEtaMaxGen (MC; -999 data),
etaMaxRecomp08, nMixedClus, nFmuLayer[6], nFtsLayer[16].  Original docstring follows.

WP-A3 per-event skim for the forward-veto response-map
factorisation test (docs/restart_2026-10-27/wp-a3/response_reweighting_scoping.md).

One merged v6dif input -> one compressed npz with the A_pre events (both
EventNumber parities, column `even`).  Selection, D cells, run policy and the
signed-v3 weight recipe are copied verbatim from
scripts/restart_2026-10-27/wp-a2/n1_wpa2.py (main(), no --noise-table, no
--eplug-scale, no --vtxz-weights), so stack-C counts recomputed from the skim
equal the WP-A2 S4 odd partials (test `skim_identity_s4`, run in fact_wpa3.py).
E-pz comes from the ChargedTot join on (RunNumber, EventNumber) exactly as
n1_wpa2.py --empz-join self (NaN when unmatched).  No ROOT output."""
import argparse, csv, json, sys
from pathlib import Path
import numpy as np
import uproot

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a2"))  # wp-a2 n1_wpa2 (unchanged)
import n1_wpa2 as N  # D, NOODS, PDISS, CHARM, g21, normrow, accepted, load_period_map, assign


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--kind", choices=("data", "mc"), required=True); ap.add_argument("--label", required=True)
    ap.add_argument("--input", required=True); ap.add_argument("--out", required=True)
    ap.add_argument("--runlist"); ap.add_argument("--scale-table")
    ap.add_argument("--period-map", default="/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a/period_map.csv")
    a = ap.parse_args()
    b = ["RunNumber", "EventNumber", "xIp", "mx", "passRecoEnv", "Q2s", "Ys", "hatVtxZ", "etaMaxClus08", "etaMaxClus04",
         "selFmd", "ePlug", "selFts", "nFmu12", "nFts28", "nPrt",
         "etaMaxRecomp08", "nMixedClus", "nFmuLayer", "nFtsLayer"]
    if a.kind == "mc":
        b += ["weight1", "weight2", "hatXpomGki", "mirMY", "etaMaxGen", "hatEtaMaxGen"] + (["hatMYGen", "hatTGki", "hatMXGen"] if a.label in N.PDISS else [])
    f = uproot.open(a.input); d = f["DiffTot"].arrays(b, library="np"); n = len(d["Q2s"])
    EVKEY = (np.asarray(d["RunNumber"], np.int64).astype(np.uint64) << np.uint64(32)) | np.asarray(d["EventNumber"], np.int64).astype(np.uint64)
    if np.unique(EVKEY).size != n:
        raise SystemExit("FATAL: (RunNumber, EventNumber) not unique")
    acc = N.accepted(a.runlist) if a.kind == "data" else None
    runok = np.isin(d["RunNumber"], np.fromiter(acc, dtype=d["RunNumber"].dtype)) if acc is not None else np.ones(n, bool)
    pre0 = runok & d["passRecoEnv"] & (d["Q2s"] > 150) & (d["Ys"] > .1) & (d["Ys"] < .9) & (np.abs(d["hatVtxZ"]) < 35)
    # weights: verbatim n1_wpa2.py recipe
    w_signed = np.ones(n)
    if a.kind == "mc":
        scale, expo = N.normrow(a.scale_table, a.label); event = np.asarray(d["weight1"] * d["weight2"], float)
        if a.label == "9012": event[:] = 1.
        if a.label in N.CHARM:
            qi, yi, raw = N.g21(d["Q2s"], d["Ys"], d["hatXpomGki"]); event[:] = 0.
            for q in range(6):
                for y in range(3):
                    m = (qi == q) & (yi == y) & np.isfinite(raw) & (raw > 0)
                    if m.any(): event[m] = raw[m] / raw[m].mean()
        elif a.label in N.PDISS:
            truth = (d["hatMYGen"] < 1.6) & (np.abs(d["hatTGki"]) < 1) & (d["hatXpomGki"] < .03) & (d["hatMXGen"] > 2)
            event[truth] *= 5.3611
        w_signed = event * scale * expo
    x = d["Q2s"] / (4 * 27.6 * 920 * d["Ys"]); cell = np.full(n, -1, np.int8)
    for i, (ql, qh, xl, xh) in enumerate(N.D): cell[(d["Q2s"] >= ql) & (d["Q2s"] < qh) & (x >= xl) & (x < xh)] = i
    PM = N.load_period_map(a.period_map); per, _ = N.assign(np.asarray(d["RunNumber"], np.int64), PM)
    pnames = ["ep0304", "em0405", "em06_true", "ep0607_true"]
    pidx = np.full(n, -1, np.int8)
    if a.kind == "data":
        for i, p in enumerate(pnames): pidx[per == p] = i
    # E-pz join (n1_wpa2 --empz-join self)
    t = f["ChargedTot"].arrays(["RunNumber", "EventNumber", "Empz"], library="np")
    ck = (np.asarray(t["RunNumber"], np.int64).astype(np.uint64) << np.uint64(32)) | np.asarray(t["EventNumber"], np.int64).astype(np.uint64)
    uk, ix = np.unique(ck, return_index=True); ue = np.asarray(t["Empz"], float)[ix]; del t, ck
    j = np.clip(np.searchsorted(uk, EVKEY), 0, max(uk.size - 1, 0)); hit = (uk.size > 0) & (uk[j] == EVKEY)
    empz = np.where(hit, ue[j], np.nan)
    m = pre0
    out = {"even": (np.asarray(d["EventNumber"], np.int64)[m] % 2 == 0), "cell": cell[m], "period": pidx[m],
           "w": w_signed[m], "eplug": np.asarray(d["ePlug"], np.float32)[m], "fmd": d["selFmd"].astype(bool)[m],
           "fts": d["selFts"].astype(bool)[m], "nfmu12": np.asarray(d["nFmu12"], np.int16)[m],
           "nfts28": np.asarray(d["nFts28"], np.int16)[m], "ec08": np.asarray(d["etaMaxClus08"], np.float32)[m],
           "ec04": np.asarray(d["etaMaxClus04"], np.float32)[m], "vz": np.asarray(d["hatVtxZ"], np.float32)[m],
           "empz": empz.astype(np.float32)[m], "nprt": np.asarray(d["nPrt"], np.int16)[m],
           "xip": np.asarray(d["xIp"], np.float32)[m], "mx": np.asarray(d["mx"], np.float32)[m],
           "q2s": np.asarray(d["Q2s"], np.float32)[m], "ys": np.asarray(d["Ys"], np.float32)[m], "x": x.astype(np.float32)[m],
           "etagen": (np.asarray(d["etaMaxGen"], np.float32)[m] if a.kind == "mc" else np.full(int(m.sum()), -999, np.float32)),
           "hatetagen": (np.asarray(d["hatEtaMaxGen"], np.float32)[m] if a.kind == "mc" else np.full(int(m.sum()), -999, np.float32)),
           "ecrecomp08": np.asarray(d["etaMaxRecomp08"], np.float32)[m], "nmixed": np.asarray(d["nMixedClus"], np.int16)[m],
           "nfmulayer": np.asarray(d["nFmuLayer"], np.int16)[m], "nftslayer": np.asarray(d["nFtsLayer"], np.int16)[m],
           "mirmy": (np.asarray(d["mirMY"], np.float32)[m] if a.kind == "mc" else np.full(int(m.sum()), np.nan, np.float32))}
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(a.out, **out)
    print(json.dumps({"label": a.label, "kind": a.kind, "n_events": n, "n_pre0": int(m.sum()), "n_even": int(out["even"].sum()),
                      "empz_coverage": float(np.isfinite(out["empz"]).mean()) if m.any() else None}))


if __name__ == "__main__":
    main()

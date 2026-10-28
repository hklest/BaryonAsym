#!/usr/bin/env python3
"""WP-A5: attach D cell, etaMaxClus08 and EventNumber parity to the WP-C signal track skims s<label>.npz.

Reads DiffTot of the merged input (the file wpc_build.py read), rows e_idx of s<label>.npz (wpc_build.py:187), and
assigns the D cell exactly as skim_wpa3.py:56-57 (x = Q2s / (4 * 27.6 * 920 * Ys), cells n1_wpa2.D).
Invariants (fatal): every attached etaMaxClus08 < 3.2 or < -900 (wpc_build.py:100-102), (run, event) unique,
DiffTot.mx[e_idx] == s<label>.npz e_mx."""
import argparse, json, sys
from pathlib import Path
import numpy as np
import uproot

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a2"))
import n1_wpa2 as N
WPC = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-c/study")


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--label", required=True); ap.add_argument("--input", required=True)
    ap.add_argument("--out", required=True); a = ap.parse_args()
    s = np.load(WPC / ("s%s.npz" % a.label)); ei = np.asarray(s["e_idx"], np.int64)
    d = uproot.open(a.input)["DiffTot"].arrays(["RunNumber", "EventNumber", "Q2s", "Ys", "etaMaxClus08", "mx"], library="np")
    run = np.asarray(d["RunNumber"], np.int64)[ei]; ev = np.asarray(d["EventNumber"], np.int64)[ei]
    q2 = np.asarray(d["Q2s"])[ei]; ys = np.asarray(d["Ys"])[ei]; ec = np.asarray(d["etaMaxClus08"], np.float32)[ei]
    x = q2 / (4 * 27.6 * 920 * ys); cell = np.full(ei.size, -1, np.int8)
    for i, (ql, qh, xl, xh) in enumerate(N.D): cell[(q2 >= ql) & (q2 < qh) & (x >= xl) & (x < xh)] = i
    chk = {"label": a.label, "n_events": int(ei.size),
           "ec08_ok": bool(np.all((ec.astype(np.float64) < 3.2) | (ec < -900))),
           "run_event_unique": bool(np.unique(run * 10**10 + ev).size == ei.size),
           "mx_matches_skim": bool(np.array_equal(np.asarray(d["mx"])[ei].astype(np.float64), np.asarray(s["e_mx"], np.float64))),
           "cell_counts": {str(c): int((cell == c).sum()) for c in (-1, 0, 1, 2)}}
    print(json.dumps(chk), flush=True)
    if not (chk["ec08_ok"] and chk["run_event_unique"] and chk["mx_matches_skim"]): sys.exit(2)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(a.out, e_idx=ei, run=run, event=ev, cell=cell, ec08=ec, even=(ev % 2 == 0))


if __name__ == "__main__":
    main()

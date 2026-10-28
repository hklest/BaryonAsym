#!/usr/bin/env python3
"""WP-A6 edge30 (user D15): the WP-C proposal purity table and the D7+ u G2 mask with the selection edge etaMaxClus08 < 3.0
applied to the WP-A6 track builds (built at < 3.2).  Runs purity_fill_wpa6.main unchanged, with load_tracks replaced by a
version that also drops events with 3.0 <= ec08 (sentinel kept, as in the build).  Per-event ec08: signal from
wp-a5/cells_<label>.npz (e_idx-aligned), background from edge30/ec08_attach/ (attach_ec08_wpa6.py; b7071 from the 61 parts,
offset as in wpc_build_bkg_wpa6.merge).  Blinded: MC only.  Same CLI as purity_fill_wpa6.py."""
import glob, json, os, sys
from pathlib import Path
import numpy as np
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import purity_fill_wpa6 as PF

EDGE = 3.0
D = Path(PF.D); A5 = D / "data/restart_2026-10-27/wp-a5"; AT = D / "data/restart_2026-10-27/wp-a6/edge30/ec08_attach"
STATS = {}


def ec08_for(prefix, label, e_idx):
    if prefix == "s":
        c = np.load(A5 / f"cells_{label}.npz"); ei, ec = c["e_idx"], c["ec08"]
    elif label == "7071":
        ei, ec, off = [], [], 0
        for p in sorted(glob.glob(str(D / "data/restart_2026-10-27/wp-a6/trk/parts_7071/b7071_*.npz"))):
            a = np.load(AT / os.path.basename(p)); ei.append(a["e_idx"] + off); ec.append(a["ec08"])
            off += json.load(open(p[:-4] + ".json"))["n_events_DiffTot"]
        ei, ec = np.concatenate(ei), np.concatenate(ec)
    else:
        a = np.load(AT / f"b{label}.npz"); ei, ec = a["e_idx"], a["ec08"]
    if not np.array_equal(np.asarray(ei, np.int64), np.asarray(e_idx, np.int64)): raise SystemExit(f"FATAL: ec08 e_idx mismatch {prefix}{label}")
    return np.asarray(ec, np.float64)


def load_tracks(labels, xipcut, prefix):
    """purity_fill_wpa6.load_tracks plus the event cut ec08 < EDGE or sentinel."""
    T = {}
    for l in labels:
        d = np.load("%s%s%s.npz" % (PF.TRK, prefix, l))
        eg = np.searchsorted(d["e_idx"], d["t_ev"]); x = d["e_xIp"][eg]
        ec = ec08_for(prefix, l, d["e_idx"]); keep_ev = (ec < EDGE) | (ec < -900)
        k0 = (x > 0) & (x < xipcut); k = k0 & keep_ev[eg]
        STATS[prefix + l] = dict(events=int(d["e_idx"].size), events_kept=int(keep_ev.sum()), tracks_xip=int(k0.sum()), tracks_kept=int(k.sum()))
        for key in ("t_w", "t_charge", "t_mx", "t_xfA", "t_pt2A", "t_xfB", "t_pt2B", "t_gv", "t_gmx", "t_xf2", "t_pt22", "t_cell"):
            T.setdefault(key, []).append(d[key][k])
        T.setdefault("t_label", []).append(np.full(int(k.sum()), l))
    return {k: np.concatenate(v) for k, v in T.items()}


def main():
    PF.load_tracks = load_tracks
    PF.CAVEAT = ("edge30 (user D15): event cut etaMaxClus08 < 3.0 (or sentinel) applied to the track builds (built at < 3.2); "
                 "the event-level f_ND (--g2) is evaluated on SB30 = SB & etaMaxClus08 < 3.0, so the two edges agree")
    PF.main()
    out = sys.argv[sys.argv.index("--out") + 1]; side = out.replace(".csv", ".json")
    j = json.load(open(side)); j["edge30_track_cut"] = dict(edge=EDGE, per_label=STATS); json.dump(j, open(side, "w"), indent=1)
    for p in (out, side):
        open(p + ".sha256", "w").write("%s  %s\n" % (PF.sha(p), p))
    print("EDGE30 track cut", json.dumps(STATS))


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""WP-A5 propagation re-run with the LRG selection edge etaMaxClus08 < EDGE (default 3.0; user decision D15, 2026-10-28).
Reuses the frozen propagate_wpa5.main() unchanged: sample() is wrapped to drop events with EDGE <= ec08 (sentinel
ec08 < -900 events stay, as in the original selection) and the tracks of dropped events; the frozen veto_correction.json
(q, sigma_q, eta-shape) is read as is and nothing is re-derived.  Output: <out>/g1pp_a_table_edge<NN>.csv/.json.
Usage: propagate_wpa5_edge30.py [--edge 3.0] [--out DIR]"""
import argparse, hashlib, json, sys
from pathlib import Path
import numpy as np

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
WPA5 = D / "scripts/restart_2026-10-27/wp-a5"
sys.path.insert(0, str(WPA5))
import propagate_wpa5 as P
import wpc_lib as L

ap = argparse.ArgumentParser(); ap.add_argument("--edge", type=float, default=3.0)
ap.add_argument("--out", default=str(D / "data/restart_2026-10-27/wp-a5/edge30"))
args = ap.parse_args()
EDGE = args.edge; OUT = Path(args.out); OUT.mkdir(parents=True, exist_ok=True)
FROZEN = P.OD; STAT = {}
_orig = P.sample


def sample():
    P.OD = FROZEN                      # cells_*.npz are read from the frozen directory
    T, E, cell, ec, w0, g, emb = _orig()
    n_ev, n_tr = len(w0), len(g)
    keep = (ec < EDGE) | (ec < -900)
    new = np.cumsum(keep) - 1
    tk = keep[g]
    # per-label counts (samples are concatenated in L.LABELS order; e_samp / t_samp carry the label index)
    STAT["per_label"] = {l: {"events_before": int((E["e_samp"] == i).sum()), "events_after": int(((E["e_samp"] == i) & keep).sum()),
                             "tracks_before": int((T["t_samp"] == i).sum()), "tracks_after": int(((T["t_samp"] == i) & tk).sum())}
                         for i, l in enumerate(L.LABELS)}
    for k, v in E.items(): assert len(v) == n_ev, k
    for k, v in T.items(): assert len(v) == n_tr, k
    E2 = {k: v[keep] for k, v in E.items()}; T2 = {k: v[tk] for k, v in T.items()}
    g2 = new[g[tk]]; T2["t_eglob"] = g2
    cell, ec, w0, emb = cell[keep], ec[keep], w0[keep], emb[keep]
    assert len(g2) == tk.sum() and g2.min() >= 0 and g2.max() < keep.sum()
    assert np.allclose(T2["t_w"], E2["e_w"][g2], rtol=0, atol=0)
    assert not np.any((ec >= EDGE) & (ec > -900)), "kept event with ec08 >= edge"
    assert sum(v["events_after"] for v in STAT["per_label"].values()) == keep.sum()
    assert sum(v["tracks_after"] for v in STAT["per_label"].values()) == tk.sum()
    STAT.update(n_events_before=n_ev, n_events_after=int(keep.sum()), n_tracks_before=n_tr, n_tracks_after=int(tk.sum()))
    print("edge", EDGE, json.dumps(STAT, indent=1), flush=True)
    P.OD = OUT                         # main() writes g1pp_a_table.{csv,json} to OD
    return T2, E2, cell, ec, w0, g2, emb


P.sample = sample
P.main()
tag = "edge%d" % round(EDGE * 10)
for ext in ("csv", "json"):
    (OUT / ("g1pp_a_table." + ext)).rename(OUT / ("g1pp_a_table_%s.%s" % (tag, ext)))
jp = OUT / ("g1pp_a_table_%s.json" % tag); J = json.load(open(jp))
J["meta"].update(edge=EDGE, n_events_before=STAT["n_events_before"], n_events_after=STAT["n_events_after"],
                 n_tracks_before=STAT["n_tracks_before"], n_tracks_after=STAT["n_tracks_after"], per_label=STAT["per_label"],
                 note="eta-shape bins [3.0,3.2) and [3.2,3.3) are unpopulated by construction at edge 3.0; D7 masks are the frozen "
                      "start-grid masks (mask_start_frame{A,B}_G2_D7.json), D7 not re-evaluated at this edge; q, sigma_q, eta-shape frozen "
                      "(veto_correction.json), not re-derived; n_events/n_tracks in meta fields above are the filtered sample "
                      "(top-level n_events/n_tracks in this meta equal the after counts)",
                 script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
json.dump(J, open(jp, "w"), indent=1)

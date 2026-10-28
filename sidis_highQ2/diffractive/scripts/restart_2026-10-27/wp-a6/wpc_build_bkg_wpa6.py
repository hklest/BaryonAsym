#!/usr/bin/env python3
"""WP-A6 track builder: runs the unchanged wp-c/study/wpc_build.py main() and adds the event D-cell column.

Build:  --label L --input F.root --out PREFIX --gen {on,off} -> PREFIX.npz/.json
  gen on : npz identical to wpc_build's, plus e_cell (int8, one per e_idx row) and t_cell (e_cell of each track's event).
  gen off: as above but t_gv forced False (all selected tracks count as background; t_why/t_gen*/t_x*1/2 are still
           written by wpc_build but are NOT to be used; truth never enters the selection). json: gen="off".
  Cell (copied from wp-a3/skim_wpa3.py:56-57, boxes N.D of wp-a2/n1_wpa2.py:119): x = Q2s/(4*27.6*920*Ys) from DiffTot
  Q2s/Ys; cell i if Q2 in [ql,qh) and x in [xl,xh), else -1.
Merge:  --merge 'GLOB' --out PREFIX : concatenate per-file builds (sorted by name) into one npz/json.
  Convention: concatenated event space = per-file DiffTot row spaces laid end to end; part k's e_idx and t_ev are
  shifted by O_k = sum of n_events_DiffTot of parts 0..k-1. e_idx stays sorted, so wpc_lib's
  np.searchsorted(e_idx, t_ev) maps tracks to events. Integer counters in the jsons are summed.
"""
import argparse, glob, hashlib, json, sys, os
import numpy as np
import uproot

sys.path.insert(0, "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
import wpc_build

BOXES = ((150., 300., .0021, .030), (300., 700., .0042, .069), (700., 20000., .0099, 1.5))  # N.D, n1_wpa2.py:119


def sha256(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for b in iter(lambda: f.read(1 << 24), b""):
            h.update(b)
    return h.hexdigest()


def build(a):
    sys.argv = ["wpc_build.py", "--label", a.label, "--input", a.input, "--out", a.out]
    wpc_build.main()
    d = dict(np.load(a.out + ".npz")); cut = json.load(open(a.out + ".json"))
    D = uproot.open(a.input)["DiffTot"].arrays(["RunNumber", "EventNumber", "Q2s", "Ys", "mx", "xIp"], library="np")
    q2, ys = np.asarray(D["Q2s"], float), np.asarray(D["Ys"], float)
    ei = d["e_idx"]
    assert len(q2) == cut["n_events_DiffTot"] and (len(ei) == 0 or ei.max() < len(q2)) and np.all(np.diff(ei) > 0)
    key = np.asarray(D["RunNumber"], np.int64) * 10**10 + np.asarray(D["EventNumber"], np.int64)
    assert len(np.unique(key)) == len(key), "duplicate (run,event) in DiffTot"
    assert np.array_equal(np.asarray(D["mx"], float)[ei], d["e_mx"], equal_nan=True) and \
        np.array_equal(np.asarray(D["xIp"], float)[ei], d["e_xIp"], equal_nan=True), "e_idx not aligned with DiffTot"
    with np.errstate(all="ignore"):
        x = q2 / (4 * 27.6 * 920 * ys); cell = np.full(len(q2), -1, np.int8)
        for i, (ql, qh, xl, xh) in enumerate(BOXES):
            cell[(q2 >= ql) & (q2 < qh) & (x >= xl) & (x < xh)] = i
    d["e_cell"] = cell[ei]
    d["t_cell"] = d["e_cell"][np.searchsorted(ei, d["t_ev"])]
    if a.gen == "off":
        d["t_gv"] = np.zeros(len(d["t_gv"]), bool); cut["frac_tracks_gv"] = 0.
    cut["wpa6_gen"] = a.gen
    cut["wpa6_cell_frac_selected_events"] = {str(c): float((d["e_cell"] == c).mean()) if len(ei) else 0. for c in (-1, 0, 1, 2)}
    cut["wpa6_input"] = dict(path=a.input, size=os.path.getsize(a.input), mtime=os.path.getmtime(a.input), sha256=sha256(a.input))
    np.savez_compressed(a.out + ".npz", **d)
    json.dump(cut, open(a.out + ".json", "w"), indent=1)


def merge(a):
    parts = sorted(glob.glob(a.merge)); assert parts, "no parts"
    parts = [p[:-4] for p in parts if p.endswith(".npz")]
    out, off, tot, cell_n = {}, 0, {}, np.zeros(4)
    for p in parts:
        d = np.load(p + ".npz"); j = json.load(open(p + ".json"))
        for k in d.files:
            v = d[k]
            if k in ("e_idx", "t_ev"):
                v = v + off
            out.setdefault(k, []).append(v)
        for k, v in j.items():
            if isinstance(v, (int, float)) and not isinstance(v, bool) and k not in ("scale_to_data", "exposure_factor", "frac_tracks_gv"):
                tot[k] = tot.get(k, 0) + v
        for i, c in enumerate((-1, 0, 1, 2)):
            cell_n[i] += (d["e_cell"] == c).sum()
        off += j["n_events_DiffTot"]
        scale, expo, gen = j["scale_to_data"], j["exposure_factor"], j["wpa6_gen"]
    out = {k: np.concatenate(v) for k, v in out.items()}
    assert np.all(np.diff(out["e_idx"]) > 0)
    tot.update(scale_to_data=scale, exposure_factor=expo, wpa6_gen=gen, wpa6_n_parts=len(parts), wpa6_parts=[os.path.basename(p) for p in parts],
               frac_tracks_gv=float(out["t_gv"].mean()) if len(out["t_gv"]) else 0.,
               wpa6_cell_frac_selected_events=dict(zip(("-1", "0", "1", "2"), (cell_n / max(cell_n.sum(), 1)).tolist())))
    np.savez_compressed(a.out + ".npz", **out)
    json.dump(tot, open(a.out + ".json", "w"), indent=1)


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--label"); ap.add_argument("--input"); ap.add_argument("--out", required=True)
    ap.add_argument("--gen", choices=("on", "off"), default="on"); ap.add_argument("--merge", help="glob of per-file PREFIX.npz")
    a = ap.parse_args()
    if a.merge:
        merge(a)
    else:
        assert a.label and a.input
        build(a)

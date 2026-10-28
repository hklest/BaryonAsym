#!/usr/bin/env python3
"""WP-A6 edge30: attach DiffTot etaMaxClus08 to the rows e_idx of one WP-A6 background track build part (b<label> or
parts_7071/b7071_NNNN).  Invariants (fatal): attached ec08 < 3.2 or < -900 (build cut, wpc_build.py:100-101) and
DiffTot.mx[e_idx] == e_mx of the part.  Output npz: e_idx (part-local), ec08 (float32)."""
import argparse, json, sys
from pathlib import Path
import numpy as np
import uproot


def main():
    ap = argparse.ArgumentParser(); ap.add_argument("--input", required=True); ap.add_argument("--part", required=True)
    ap.add_argument("--out", required=True); a = ap.parse_args()
    s = np.load(a.part + ".npz"); ei = np.asarray(s["e_idx"], np.int64)
    d = uproot.open(a.input)["DiffTot"].arrays(["etaMaxClus08", "mx"], library="np")
    ec = np.asarray(d["etaMaxClus08"], np.float32)[ei]
    chk = {"part": a.part, "n_events": int(ei.size),
           "ec08_ok": bool(np.all((ec.astype(np.float64) < 3.2) | (ec < -900))),
           "mx_matches": bool(np.array_equal(np.asarray(d["mx"])[ei].astype(np.float64), np.asarray(s["e_mx"], np.float64))),
           "n_ge_3p0": int((ec >= 3.0).sum())}
    print(json.dumps(chk), flush=True)
    if not (chk["ec08_ok"] and chk["mx_matches"]): sys.exit(2)
    Path(a.out).parent.mkdir(parents=True, exist_ok=True)
    np.savez_compressed(a.out, e_idx=ei, ec08=ec)


if __name__ == "__main__":
    main()

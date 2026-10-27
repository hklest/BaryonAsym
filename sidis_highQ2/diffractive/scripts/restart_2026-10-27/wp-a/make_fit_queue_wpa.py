#!/usr/bin/env python3
"""Write a fit_wpa queue: one job per (alt, binning, scope); payload per chain.
Usage: make_fit_queue_wpa.py <payload_dir> <fit_outdir> <queue_out> <n_toys> alt1 [alt2 ...]
Payloads are <payload_dir>/payload_<alt>.json (chain-specific exposure factors, no overrides)."""
import sys
import wpa_common as W
pdir, odir, qout, nt = sys.argv[1:5]
rows = []
for alt in sys.argv[5:]:
    nd = ":".join(W.CHAINS[alt]["nd"])
    for b in ("fine", "coarse"):
        for s in ("D0", "D1", "D2", "ALL"):
            rows.append(",".join([alt, nd, "-", b, s, "%s/payload_%s.json" % (pdir, alt), odir, nt, "3600"]))
open(qout, "w").write("\n".join(rows) + "\n"); print(len(rows), qout)

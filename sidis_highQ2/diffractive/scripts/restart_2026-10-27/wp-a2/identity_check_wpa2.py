#!/usr/bin/env python3
"""WP-A2 S2 test fork_default_identity: a default-flag n1_wpa2 partial equals the WP-A
partial in counts, hists, dcell_eta, my_tables, conservation and the wpa block
(period_map path excluded; keys the reference lacks, e.g. wpa.eta35_eplug added to
n1_wpa.py after WP-A S1 ran, are additive and listed).  Reports exact equality and, if not exact, the largest
relative difference of any number (float summation-order noise across hosts is
~1e-15; the test passes at max_rel <= 1e-12 with every integer count exact)."""
import json, sys
a, b = json.load(open(sys.argv[1])), json.load(open(sys.argv[2]))
st = {"n": 0, "max_rel": 0.0, "int_diff": 0, "struct": [], "extra": []}
def walk(x, y, p):
    if isinstance(x, dict):
        if not set(y) <= set(x): st["struct"].append(p); return
        if set(x) - set(y): st["extra"].append(p + ":" + ",".join(sorted(set(x) - set(y))))
        for k in y:
            if p == "/wpa" and k == "period_map": continue
            walk(x[k], y[k], p + "/" + k)
    elif isinstance(x, list):
        if len(x) != len(y): st["struct"].append(p); return
        for u, v in zip(x, y): walk(u, v, p)
    elif x != y:
        st["n"] += 1
        if isinstance(x, (int, float)) and isinstance(y, (int, float)) and not isinstance(x, bool):
            if isinstance(x, int) and isinstance(y, int): st["int_diff"] += 1
            st["max_rel"] = max(st["max_rel"], abs(x - y) / max(abs(x), abs(y), 1e-300))
        else: st["struct"].append(p)
for k in ("counts", "hists", "dcell_eta", "my_tables", "conservation", "wpa"):
    walk(a[k], b[k], "/" + k)
ok = not st["struct"] and st["int_diff"] == 0 and st["max_rel"] <= 1e-12
print(json.dumps({"exact": st["n"] == 0, "n_diff": st["n"], "max_rel": st["max_rel"], "int_diff": st["int_diff"],
                  "struct": st["struct"][:5], "extra_keys_in_candidate": st["extra"], "pass": ok, "wpa2_present": "wpa2" in a}))
sys.exit(0 if ok else 1)

#!/usr/bin/env python3
"""WP-A S1b: run-range -> period map from the frozen certified run list.

Ranges are the [min,max] run of every row (any verdict) per (true_unit,
sub_period); MC events carry real run numbers, so the same ranges map them.
Per-range certified lumi = sum of lumi_per_run_nb over ACCEPT runs
(lumi/certified_run_lumi_current_snapshot_2026-10-19.csv).
Test period_map_total: the ACCEPT lumi summed over the map must equal the
frozen total 317,212.80 nb^-1 within 0.01 nb^-1 (exit 1 otherwise).
"""
import csv, collections, json, sys
from pathlib import Path
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
RL = D / "data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv"
LU = D / "data/t2_detector/certified_runlist_2026-10-19/lumi/certified_run_lumi_current_snapshot_2026-10-19.csv"
OUT = D / "data/restart_2026-10-27/wp-a/period_map.csv"
FROZEN = 317212.80

def main():
    rows = list(csv.DictReader(open(RL)))
    lumi = {int(r["run"]): float(r["lumi_per_run_nb"]) for r in csv.DictReader(open(LU))
            if r["verdict"] == "ACCEPT" and r["lumi_per_run_nb"]}
    rng = collections.defaultdict(list)
    for r in rows:
        rng[(r["true_unit"], r["sub_period"] or r["true_unit"])].append(int(r["run"]))
    out = []
    for (per, sub), runs in sorted(rng.items(), key=lambda kv: min(kv[1])):
        lo, hi = min(runs), max(runs)
        acc = [int(r["run"]) for r in rows if r["true_unit"] == per and (r["sub_period"] or per) == sub
               and r["verdict"] == "ACCEPT"]
        out.append({"period": per, "sub_period": sub, "run_lo": lo, "run_hi": hi,
                    "n_accept_runs": len(acc), "lumi_accept_nb": round(sum(lumi.get(x, 0.0) for x in acc), 6)})
    for a, b in zip(out, out[1:]):
        if a["run_hi"] >= b["run_lo"]:
            raise SystemExit("FATAL: overlapping run ranges %s %s" % (a, b))
    tot = sum(o["lumi_accept_nb"] for o in out)
    with open(OUT, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(out[0])); w.writeheader(); w.writerows(out)
    ok = abs(tot - FROZEN) < 0.01
    print(json.dumps({"out": str(OUT), "ranges": out, "total_lumi_nb": tot,
                      "period_map_total": "PASS" if ok else "FAIL"}, indent=1))
    sys.exit(0 if ok else 1)

if __name__ == "__main__":
    main()

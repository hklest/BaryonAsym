#!/usr/bin/env python3
"""WP-A6 test entry point (child plan 2026-10-28-wp-a6-leakage-refit.md, slices 1-5).
Usage: tests_wpa6.py {slice1|slice2|slice3|slice4|slice5|all}.  Slices 1-4 run the per-slice test files beside this
one (tests_s1/s2/s3/s3b/s4a/s4b_wpa6.py) on the login node; this file adds range_frozen, freeze_before_odd and the
slice-5 checks gate_text_frozen, numbers_traceable, budget_recorded."""
import csv, glob, hashlib, json, os, re, subprocess, sys
from pathlib import Path
import numpy as np

HERE = Path(__file__).resolve().parent
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
WD = D / "data/restart_2026-10-27/wp-a6"; CF = D / "condor/restart_2026-10-27/wp-a6/fit"
PARENT = Path("/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md")
VERDICT = D / "docs/restart_2026-10-27/wp-a6/g2_verdict.md"
ENV = dict(os.environ, PYTHONDONTWRITEBYTECODE="1")


def say(name, ok, detail=""):
    print(("PASS " if ok else "FAIL ") + name + (": " + detail if detail else ""), flush=True); return bool(ok)


def run(script, *args):
    r = subprocess.run([sys.executable, str(HERE / script), *args], env=ENV, capture_output=True, text=True)
    print(r.stdout.rstrip()); print(r.stderr.rstrip(), file=sys.stderr) if r.returncode else None
    return say(script + " " + " ".join(args), r.returncode == 0)


def range_frozen():
    p = WD / "range_frozen.json"; x = json.load(open(p)); e = np.asarray(x["edges"])
    sc = (WD / "range_frozen.json.sha256").read_text().split()[0]
    ok = (len(e) == 81 and abs(e[0] + 1) < 1e-12 and abs(e[-1] - 3.0) < 1e-12 and np.allclose(np.diff(e), 0.05)
          and x["merged_bin"] is False and sc == hashlib.sha256(p.read_bytes()).hexdigest())
    return say("range_frozen", ok, "80 bins of 0.05 on [-1,3.0), no merged bin, sidecar %s" % sc[:12])


def freeze_before_odd():
    log = (CF / "submit_log_odd.txt").read_text(); fz = json.load(open(WD / "range_frozen.json"))
    sha = hashlib.sha256((WD / "range_frozen.json").read_bytes()).hexdigest()
    sub = re.search(r"utc_submit: (\S+)", log).group(1)
    ok = (sha in log) and "range_frozen check: PASS" in log and fz["utc"] < sub
    return say("freeze_before_odd", ok, "frozen %s < odd submit %s, sha quoted %s" % (fz["utc"], sub, sha in log))


def _block(text, start, stop):
    i = text.index(start); j = text.index(stop, i + len(start)); return text[i:j].strip()


def gate_text_frozen():
    v = VERDICT.read_text(); p = PARENT.read_text(); ok = True
    for start, stop in (("  (c) **Leakage (WP-A6).**", "  (d) The MC-statistics gate"), ("- **G2 (WP-A6 on the existing skims", "- **G3 (WP-G")):
        src = _block(p, start, stop)
        ok &= src in v
    return say("gate_text_frozen", ok, "G1''(c) and G2 blocks quoted verbatim from the parent")


def numbers_traceable():
    """Every number with >= 3 decimals in a verdict table row appears, at that precision, in a source table."""
    vals = set()
    srcs = [WD / "f_nd_per_mx.csv", WD / "stability_even.csv", WD / "unit_table_odd.csv", WD / "unit_table_even.csv",
            WD / "purity/binstats_wpa6_D7plus_B_xip030.csv", WD / "edge_diag_odd.csv", WD / "condor_tally.csv"]
    for s in srcs:
        if not s.exists(): continue
        for r in csv.reader(open(s)):
            for c in r:
                try: vals.add(float(c))
                except ValueError: pass
    vals = np.array(sorted(vals)); bad = []
    for line in VERDICT.read_text().splitlines():
        if not line.startswith("|") or set(line) <= set("|-: "): continue
        for m in re.finditer(r"(?<![\w.])(-?\d+\.(\d{3,}))(?![\w.])", line):
            x, nd = float(m.group(1)), len(m.group(2)); tol = 0.5 * 10 ** -nd + 1e-12
            if not np.any(np.abs(vals - x) <= tol) and not np.any(np.abs(np.abs(vals) - abs(x)) <= tol): bad.append(m.group(1))
    return say("numbers_traceable", not bad, "untraceable: %s" % bad[:10] if bad else "all table numbers found in the source CSVs")


def budget_recorded():
    rows = list(csv.DictReader(open(WD / "condor_tally.csv")))
    tot = [float(r["condor_days"]) for r in rows if r.get("item", "").upper() == "TOTAL"]
    return say("budget_recorded", bool(tot) and tot[0] <= 0.7, "total %s Condor-days" % (tot[0] if tot else None))


SL = {"slice1": lambda: [run("tests_s1_wpa6.py")],
      "slice2": lambda: [run("tests_s2_wpa6.py"), range_frozen()],
      "slice3": lambda: [freeze_before_odd(), run("tests_s3_wpa6.py", "--split", "odd"), run("tests_s3_wpa6.py", "--split", "even"),
                         run("tests_s3b_wpa6.py", "--split", "odd")],
      "slice4": lambda: [run("tests_s4a_wpa6.py"), run("tests_s4b_wpa6.py")],
      "slice5": lambda: [gate_text_frozen(), numbers_traceable(), budget_recorded()]}

if __name__ == "__main__":
    which = sys.argv[1] if len(sys.argv) > 1 else "all"
    res = sum((SL[k]() for k in (SL if which == "all" else [which])), [])
    print("ALL PASS" if all(res) else "SOME FAILED"); sys.exit(0 if all(res) else 1)

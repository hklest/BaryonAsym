#!/usr/bin/env python3
"""WP-C C1 Condor driver.  Usage: c1_run.py tables|stats|all  (stages import the c1_* modules and call main())."""
import sys, importlib
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
STAGES = {"tables": ["c1_tables", "c1_mask", "c1_resolution"], "stats": ["c1_stats", "c1_plots"]}
STAGES["t1"] = ["c1_tables"]
STAGES["all"] = STAGES["tables"] + STAGES["stats"]
if __name__ == "__main__":
    st = sys.argv[1]
    for m in STAGES[st]:
        print("=== stage module", m, flush=True)
        mod = importlib.import_module(m)
        sys.argv = [m]
        mod.main()

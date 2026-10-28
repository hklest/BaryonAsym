#!/usr/bin/env python3
"""Sum the per-file `meta` counters (D7/D17/D10) of the written hits files into wp1_counters_lam.json.
Usage (after desygroupset h1 + thish1.sh): aggregate_counters.py '<glob>' <out.json>"""
import glob, json, sys
from collections import Counter
import ROOT
T, per = Counter(), {}
for f in sorted(glob.glob(sys.argv[1])):
    m = json.loads(ROOT.TFile.Open(f).Get('meta').GetTitle())
    assert m['seen_equals_written_rejected_empty_skips'], f
    for k, v in m['counters'].items():
        T[k] += v
    per[f.split('hits_part_')[-1][:-5]] = m['counters']
json.dump({'total': dict(T), 'per_dir': per}, open(sys.argv[2], 'w'), indent=1)
print(dict(T))

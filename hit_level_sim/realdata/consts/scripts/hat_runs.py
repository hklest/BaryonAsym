#!/usr/bin/env python3
"""Distinct runs in one HAT file, all entries and after the HAT selection (only 'Q2e>X' plus '&& !(RunNumber==R && EventNumber==E)' exclusions).
usage (LCG python, not -I): hat_runs.py <hat.root> [selection]  -> JSON on stdout"""
import sys, json, re
import uproot, numpy as np
fn = sys.argv[1]; sel = sys.argv[2] if len(sys.argv) > 2 else ''
t = uproot.open(fn)['HAT']
a = t.arrays(['RunNumber', 'Q2e'], library='np')
run = a['RunNumber']
m = np.ones(len(run), bool)
mm = re.fullmatch(r'\s*Q2e\s*>\s*([0-9.eE+-]+)\s*((?:&&\s*!\(\s*RunNumber\s*==\s*\d+\s*&&\s*EventNumber\s*==\s*\d+\s*\)\s*)*)', sel)
if mm:
    m = a['Q2e'] > float(mm.group(1))
    excl = re.findall(r'RunNumber\s*==\s*(\d+)\s*&&\s*EventNumber\s*==\s*(\d+)', mm.group(2))
    if excl:   # single-event exclusions (events that crash calibprobe)
        ev = t.arrays(['EventNumber'], library='np')['EventNumber']
        for r, e in excl:
            m &= ~((run == int(r)) & (ev == int(e)))
elif sel:
    sys.exit(f'unsupported selection {sel!r}')
ra, ca = np.unique(run, return_counts=True)
rs, cs = np.unique(run[m], return_counts=True)
json.dump({'file': fn, 'selection': sel, 'entries': int(len(run)), 'selected': int(m.sum()),
           'runs_all': {int(r): int(c) for r, c in zip(ra, ca)},
           'runs_selected': {int(r): int(c) for r, c in zip(rs, cs)}}, sys.stdout)

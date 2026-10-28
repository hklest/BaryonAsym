#!/usr/bin/env python3
"""Collect one period's per-file outputs into $D/consts/<period>/consts_<run>.json (lowest file wins),
compare duplicates of runs that span files, and write runs.tsv and summary.json.
usage: python3 -I collect.py <period>"""
import sys, os, json, glob, shutil
period = sys.argv[1]
base = f'/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/consts/{period}'
files = [l.split()[0] for l in open(os.path.join(base, 'files.txt')) if l.strip()]
EVT = {('coz', 'RCJCT0'), ('coz', 'TDRMUL'), ('cjc', 'IECUR'), ('raw_ndb_banks', 'CJAC'), ('raw_ndb_banks', 'CJCC')}
def strip(j):   # drop the first-event-dependent fields (event T0 for COZ, beam current, HV monitor, CJCC)
    for a, b in EVT:
        j.get(a, {}).pop(b, None)
    return j
rows, done, missing = [], [], []
runs_all, runs_sel, hat_entries = set(), set(), 0
dup = {'identical': 0, 'event_fields_only': 0, 'different': []}
ndb = set()
for f in files:
    d = os.path.join(base, 'by_file', f)
    if not os.path.exists(os.path.join(d, 'DONE')):
        missing.append(f); continue
    done.append(f)
    h = json.load(open(os.path.join(d, 'hat_runs.json')))
    runs_all |= set(map(int, h['runs_all'])); runs_sel |= set(map(int, h['runs_selected'])); hat_entries += h['entries']
    for l in list(open(os.path.join(d, 'runs.tsv')))[1:]:
        t = l.rstrip('\n').split('\t')
        r = dict(zip(['file', 'run', 'evt', 'calrun_ier', 'crje_hits', 'json', 'ivrsqt', 'ndb'], t))
        ndb.add(r['ndb'])
        src = os.path.join(d, f'consts_{r["run"]}.json'); dst = os.path.join(base, f'consts_{r["run"]}.json')
        if r['json'] == '1' and os.path.exists(src):
            if any(x['run'] == r['run'] and x['json'] == '1' for x in rows):   # duplicate: compare, keep the first
                a, b = open(dst, 'rb').read(), open(src, 'rb').read()
                if a == b: dup['identical'] += 1
                elif strip(json.loads(a)) == strip(json.loads(b)): dup['event_fields_only'] += 1
                else: dup['different'].append(int(r['run']))
                os.remove(src); r['json'] = 'dup'
            else:
                shutil.move(src, dst)
        elif r['json'] == '1' and os.path.exists(dst):
            pass   # already collected by an earlier pass
        rows.append(r)
with open(os.path.join(base, 'runs.tsv'), 'w') as o:
    o.write('file\trun\tevt\tcalrun_ier\tcrje_hits\tjson\tivrsqt\tndb\n')
    for r in rows: o.write('\t'.join(r[k] for k in ['file', 'run', 'evt', 'calrun_ier', 'crje_hits', 'json', 'ivrsqt', 'ndb']) + '\n')
have = {int(os.path.basename(p)[7:-5]) for p in glob.glob(os.path.join(base, 'consts_*.json'))}
tried = {int(r['run']) for r in rows}
failed = sorted({int(r['run']) for r in rows if r['json'] == '0'} - have)
def split(runs):   # 2006 e-/e+ boundary (D15)
    return {'e-': sum(1 for r in runs if r < 468530), 'e+': sum(1 for r in runs if r >= 468530)} if period == '2006' else None
s = {'period': period, 'files': len(files), 'files_done': len(done), 'files_missing': missing, 'hat_entries': hat_entries,
     'runs_in_hat_all': len(runs_all), 'runs_in_hat_selected': len(runs_sel),
     'runs_attempted': len(tried), 'consts_produced': len(have), 'failed': failed,
     'selected_runs_without_attempt': sorted(runs_sel - tried)[:200], 'n_selected_runs_without_attempt': len(runs_sel - tried),
     'cjc_off_first_event': sorted(int(r['run']) for r in rows if r['crje_hits'] == '0' and r['json'] != 'dup'),
     'calrun_ier_nonzero': sorted(int(r['run']) for r in rows if r['calrun_ier'] not in ('0',)),
     'duplicates': dup, 'ivrsqt': sorted({int(r['ivrsqt']) for r in rows}),
     'ivrsqt_counts': {v: sum(1 for x in have if (3 if x >= 452777 else 2) == v) for v in (2, 3)},
     'ndb': sorted(ndb), 'run_range': [min(have), max(have)] if have else None,
     'consts_split_2006': split(have), 'note': 'consts json key cjc.IVRSQT is hard-coded 3 by make_consts.py; the true value is the ivrsqt column (calrun.F:13-15); h1hits.py does not read it'}
json.dump(s, open(os.path.join(base, 'summary.json'), 'w'), indent=1)
print(json.dumps({k: (v if not isinstance(v, list) or len(v) < 12 else f'{len(v)} items') for k, v in s.items()}))

#!/usr/bin/env python3
"""Per-run table for one calibprobe_consts job dir: parse the CONSTS lines and NDB lines of job.log.
usage: python3 -I file_summary.py <jobdir> <file>  -> TSV on stdout
columns: file run evt calrun_ier crje_hits json ivrsqt ndb"""
import sys, os, re
d, f = sys.argv[1], sys.argv[2]
cal, crje, dump = {}, {}, {}
ndb = 'none'
for line in open(os.path.join(d, 'job.log'), errors='replace'):
    t = line.split()
    if line.startswith('CONSTS CALRUN'):
        cal[int(t[3])] = (int(t[5]), int(t[7]))
    elif line.startswith('CONSTS CRJE'):
        crje[int(t[3])] = int(t[5])
    elif line.startswith('CONSTS DUMP'):
        dump[int(t[3])] = int(t[5])
    else:
        m = re.search(r"\*NDB\('/JOB'\)\*\s+(\S+)\s+ok: File opened", line)
        if m and ndb == 'none':
            ndb = m.group(1)
def ivrsqt(run):   # calrun.F:13-15 (h1rec cjrunc.F:465-467)
    return 1 if run < 281276 else (3 if run >= 452777 else 2)
print('file\trun\tevt\tcalrun_ier\tcrje_hits\tjson\tivrsqt\tndb')
for r in sorted(set(cal) | set(dump)):
    evt, ier = cal.get(r, (-1, -1))
    js = os.path.exists(os.path.join(d, f'consts_{r}.json'))
    print(f'{f}\t{r}\t{evt}\t{ier}\t{crje.get(r, -1)}\t{int(js)}\t{ivrsqt(r)}\t{ndb}')

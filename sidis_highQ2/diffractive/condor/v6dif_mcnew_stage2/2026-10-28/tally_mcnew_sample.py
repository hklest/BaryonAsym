#!/usr/bin/env python3
"""Per-sample chunk tally+gates. Usage: tally_mcnew_sample.py R  -> <camp>/tally_R.csv (run inside H1OO env, on Condor).
Columns: chunk,evls,difftot,reco_env,sum_weight1,ods_loaded,flagged,rc,wall_s,evls_rc,evls_wall_s,verdict"""
import json, os, re, subprocess, sys, datetime
R = sys.argv[1]
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive"
s = json.load(open(f"{D}/condor/v6dif_mcnew/2026-10-28/mcnew_samples.json"))["samples"][R]
O = f"{D}/output_v6dif/mc/{R}/2026-10-28"
C2 = f"{D}/condor/v6dif_mcnew_stage2/2026-10-28"
def _p(x):
    w = x.split(); return datetime.datetime.strptime(" ".join(w[:4]) + " " + w[-1], "%a %b %d %H:%M:%S %Y") if len(w) >= 5 else None
def rcwall(logf):
    t = open(logf).read() if os.path.exists(logf) else ""
    rc = re.findall(r"finished (.+?) rc=(\d+)", t); st = re.findall(r"started (.+?) on", t)
    if not rc or not st: return -1, -1
    try: return int(rc[-1][1]), int((_p(rc[-1][0]) - _p(st[-1])).total_seconds())
    except Exception: return int(rc[-1][1]), -1
rows = []
for c in range(s["chunks"]):
    sfx = f"{s['generator']}_{R}_{s['period']}_{c:04d}"
    ev = f"{O}/evls_v6dif_{sfx}.root"; st = f"{O}/sidistree_v6dif_{sfx}.root"
    rc, wall = rcwall(f"{C2}/mc{R}/logs/prod_v6dif_v6dif_{sfx}.out")
    erc, ewall = rcwall(f"{D}/condor/v6dif_mcnew_evls/2026-10-28/mc{R}/logs/prod_evls_evls_v6dif_{sfx}.out")
    p = subprocess.run(["root","-l","-b","-q",f'{C2}/verify_mcnew_chunk.C("{ev}","{st}")'], capture_output=True, text=True, cwd="/tmp")
    l = [x for x in p.stdout.splitlines() if x.startswith("VERIFY|")]
    if not l: rows.append((c,-1,-1,-1,-1,-1,"nofile",rc,wall,erc,ewall,"FAIL")); continue
    f = l[0].split("|"); ne, nd, nr, sw, no, nf, fl = int(f[1]), int(f[2]), int(f[3]), float(f[4]), int(f[5]), int(f[6]), f[7]
    ok = rc == 0 and erc == 0 and ne == nd > 0 and no == nd and nf == 0
    wmin, wmax = float(f[8]), float(f[9])
    if R in ("9022","9023"): ok = ok and abs(sw-nd) < 1e-3 and wmin == wmax == 1.0
    else: ok = ok and wmin >= 1.0 and sw > nd*0.999
    rows.append((c,ne,nd,nr,round(sw,4),no,fl if fl!="-" else "",rc,wall,erc,ewall,"PASS" if ok else "FAIL"))
with open(f"{C2}/mc{R}/tally_{R}.csv","w") as o:
    o.write("chunk,evls,difftot,reco_env,sum_weight1,ods_loaded,flagged,rc,wall_s,evls_rc,evls_wall_s,verdict\n")
    for r in rows: o.write(",".join(map(str,r))+"\n")
print(R, sum(r[-1]=="PASS" for r in rows), "/", len(rows))

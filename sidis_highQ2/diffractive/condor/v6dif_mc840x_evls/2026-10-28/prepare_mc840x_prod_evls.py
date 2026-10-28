#!/usr/bin/env python3
"""Full-production prep, stage-1 evls, all chunks. Usage: prepare_mcnew_prod_evls.py --reqid 8530
Submit: condor_submit QFILE=<camp>/queue_prod.txt <camp>/prod_evls.sub"""
import argparse, json, os, sys
R = "/data/dust/group/h1/klesthen/BaryonAsym"
DIFF = f"{R}/sidis_highQ2/diffractive"
J = f"{DIFF}/condor/v6dif_mc840x/2026-10-28/mc840x_samples.json"
BIN = f"{R}/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6"
GRLD = f"{R}/H1ChargedSidis_HighQ2/LumiFiles"
ap = argparse.ArgumentParser(); ap.add_argument("--reqid", required=True); a = ap.parse_args()
s = json.load(open(J))["samples"][a.reqid]; q = a.reqid
camp = f"{DIFF}/condor/v6dif_mc840x_evls/2026-10-28/mc{q}"
out = f"{DIFF}/output_v6dif/mc/{q}/2026-10-28"
for d in (camp, f"{camp}/logs", f"{camp}/steering", out): os.makedirs(d, exist_ok=True)
if not os.path.exists(f"{GRLD}/{s['grl']}"): sys.exit("MISSING GRL")
lines = []
for c in range(s["chunks"]):
    hat = f"{s['oo_base']}/hat.{s['file_stem']}.{c:04d}.root"; mods = f"{s['oo_base']}/mods.{s['file_stem']}.{c:04d}.root"
    for p in (hat, mods):
        if not os.path.exists(p): sys.exit(f"MISSING {p}")
    tag = f"evls_v6dif_{s['generator']}_{q}_{s['period']}_{c:04d}"
    steer = f"{camp}/steering/{tag}.steer"
    open(steer, "w").write(f'H1SteerTree() {{\n   fHatFiles  = "{hat}";\n   fModsFiles = "{mods}";\n   fLoadODS   = false;\n}}\n')
    lines.append(f"{steer},{tag},{out}/{tag}.root\n")
job = f"{camp}/prod_evls_job.sh"
open(job, "w").write(f'''#!/usr/bin/env bash
echo "=== mcnew-evls [$2] started $(date) on $(hostname) ==="
source /etc/profile.d/02-desygroupset.sh 2>&1 || true
desygroupset h1 2>&1 || true
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh 2>&1 || true
if [[ "${{ROOT_OK:-0}}" != "1" ]]; then echo "ERROR: H1OO env" >&2; exit 1; fi
mkdir -p "$(dirname "$3")"
{BIN} --stage evls --good-runs {GRLD}/{s['grl']} --mode mc --steer "$1" -o "$3"
RC=$?
echo "=== mcnew-evls [$2] finished $(date) rc=${{RC}} ==="
exit ${{RC}}
''')
os.chmod(job, 0o755)
open(f"{camp}/queue_prod.txt", "w").writelines(lines)
open(f"{camp}/prod_evls.sub", "w").write(f'''universe        = vanilla
executable      = {job}
arguments       = $(steer) $(tag) $(outroot)
output          = {camp}/logs/prod_evls_$(tag).out
error           = {camp}/logs/prod_evls_$(tag).err
log             = {camp}/logs/prod_evls_$(cluster).log
request_memory  = 6144
request_disk    = 5G
request_cpus    = 1
should_transfer_files   = IF_NEEDED
when_to_transfer_output = ON_EXIT
+MaxRuntime     = 10800
+MyProject      = "h1"
requirements    = (OpSysAndVer == "RedHat9") && (TARGET.Arch == "X86_64")
on_exit_remove  = True
notification    = Never
initialdir      = {camp}
queue steer, tag, outroot from $(QFILE)
''')
print("ok", camp, len(lines))

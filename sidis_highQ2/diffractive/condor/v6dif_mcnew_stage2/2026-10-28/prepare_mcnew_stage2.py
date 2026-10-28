#!/usr/bin/env python3
"""Thin smoke prep (chunk 0000) for stage-2 v6dif --diff-ods, mcnew samples. Needs the stage-1 smoke evls. Usage: prepare_mcnew_stage2.py --reqid 8529"""
import argparse, json, os, sys
R = "/data/dust/group/h1/klesthen/BaryonAsym"
DIFF = f"{R}/sidis_highQ2/diffractive"
J = f"{DIFF}/condor/v6dif_mcnew/2026-10-28/mcnew_samples.json"
BIN = f"{R}/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v6dif"
ap = argparse.ArgumentParser(); ap.add_argument("--reqid", required=True); a = ap.parse_args()
s = json.load(open(J))["samples"][a.reqid]; q = a.reqid
camp = f"{DIFF}/condor/v6dif_mcnew_stage2/2026-10-28/mc{q}"
out = f"{DIFF}/output_v6dif/mc/{q}/smoke_2026-10-28"
for d in (camp, f"{camp}/logs", f"{camp}/steering"): os.makedirs(d, exist_ok=True)
farm = f"{camp}/fpackfarm"; sub = f"acs/mc2/{s['generator']}/{s['pp']}/{q}"
os.makedirs(f"{farm}/{sub}", exist_ok=True)
fl = sorted(os.listdir(s["dst_dir"]))
if len(fl) != s["dst_count"]: sys.exit(f"DST count {len(fl)} != {s['dst_count']}")
for n in fl:
    l = f"{farm}/{sub}/{n}"
    if not os.path.lexists(l): os.symlink(f"{s['dst_dir']}/{n}", l)
tag = f"v6dif_{s['generator']}_{q}_{s['period']}_0000"
evls = f"{out}/evls_v6dif_{s['generator']}_{q}_{s['period']}_0000.root"
if not os.path.exists(evls): sys.exit(f"stage-1 evls missing: {evls}")
steer = f"{camp}/steering/{tag}.steer"
open(steer, "w").write(f'H1SteerTree() {{\n  fEventListFiles = "{evls}";\n}}\n')
job = f"{camp}/v6dif_mc{q}_stage2_job.sh"
open(job, "w").write(f'''#!/usr/bin/env bash
echo "=== mcnew-stage2 [$2] started $(date) on $(hostname) ==="
source /etc/profile.d/02-desygroupset.sh 2>&1 || true
desygroupset h1 2>&1 || true
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh 2>&1 || true
if [[ "${{ROOT_OK:-0}}" != "1" ]]; then echo "ERROR: H1OO env" >&2; exit 1; fi
mkdir -p "$(dirname "$3")"
{BIN} --mode mc --steer "$1" -o "$3" --diff-ods --diff-fpack-searchpath {farm}
RC=$?
echo "=== mcnew-stage2 [$2] finished $(date) rc=${{RC}} ==="
exit ${{RC}}
''')
os.chmod(job, 0o755)
open(f"{camp}/queue.txt", "w").write(f"{steer},{tag},{out}/sidistree_{tag}.root\n")
open(f"{camp}/v6dif_mc{q}_stage2.sub", "w").write(f'''universe        = vanilla
executable      = {job}
arguments       = $(steer) $(tag) $(outroot)
output          = {camp}/logs/v6dif_$(tag).out
error           = {camp}/logs/v6dif_$(tag).err
log             = {camp}/logs/v6dif_$(cluster).log
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
queue steer, tag, outroot from {camp}/queue.txt
''')
print("ok", camp)

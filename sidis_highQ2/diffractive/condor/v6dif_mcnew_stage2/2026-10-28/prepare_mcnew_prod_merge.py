#!/usr/bin/env python3
"""Build merge .sub for verified samples. Usage: prepare_mcnew_prod_merge.py R1 R2 ... (reads output_v6dif/mc/R/2026-10-28/sidistree_*.root; chunk list from mcnew_samples.json)."""
import json, os, sys
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive"
S = json.load(open(f"{D}/condor/v6dif_mcnew/2026-10-28/mcnew_samples.json"))["samples"]
camp = f"{D}/condor/v6dif_mcnew_stage2/2026-10-28/merge"; os.makedirs(f"{camp}/logs", exist_ok=True)
mdir = f"{D}/data/t2_detector/mcnew_merged_2026-10-28"; os.makedirs(mdir, exist_ok=True)
q = open(f"{camp}/queue_merge.txt", "w")
for r in sys.argv[1:]:
    s = S[r]; ins = [f"{D}/output_v6dif/mc/{r}/2026-10-28/sidistree_v6dif_{s['generator']}_{r}_{s['period']}_{c:04d}.root" for c in range(s["chunks"])]
    assert all(os.path.exists(p) for p in ins), r
    lf = f"{camp}/inputs_{r}.txt"; open(lf, "w").write("\n".join(ins) + "\n")
    q.write(f"{r},{mdir}/{r}.root,{lf}\n")
q.close()
open(f"{camp}/prod_merge.sub", "w").write(f'''universe = vanilla
executable = {D}/condor/v6dif_mcnew_stage2/2026-10-28/prod_merge_job.sh
arguments = $(r) $(out) $(ins)
output = {camp}/logs/merge_$(r).out
error = {camp}/logs/merge_$(r).err
log = {camp}/logs/merge_$(cluster).log
request_memory = 4096
request_disk = 4G
request_cpus = 1
should_transfer_files = IF_NEEDED
when_to_transfer_output = ON_EXIT
+MaxRuntime = 7200
+MyProject = "h1"
requirements = (OpSysAndVer == "RedHat9") && (TARGET.Arch == "X86_64")
on_exit_remove = True
notification = Never
initialdir = {camp}
queue r, out, ins from {camp}/queue_merge.txt
''')

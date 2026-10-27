#!/usr/bin/env bash
# WP-C study worker: wpc_build.py for one merged MC file. Usage: wpc_build_job.sh <label> <input> <outprefix>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
python3 -c "import uproot, numpy" || exit 1
python3 $BASE/scripts/restart_2026-10-27/wp-c/study/wpc_build.py --label "$1" --input "$2" --out "$3"

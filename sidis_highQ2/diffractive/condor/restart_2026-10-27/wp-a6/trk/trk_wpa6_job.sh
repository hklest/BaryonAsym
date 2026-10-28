#!/usr/bin/env bash
# WP-A6 track build worker. Usage: trk_wpa6_job.sh <label> <input> <outprefix> <gen>
export PYTHONDONTWRITEBYTECODE=1
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
RC=$BASE/condor/restart_2026-10-27/wp-a6/trk/logs/rc_$(basename "$3").txt
python3 -c "import uproot, numpy" || { echo 1 > "$RC"; exit 1; }
python3 $BASE/scripts/restart_2026-10-27/wp-a6/wpc_build_bkg_wpa6.py --label "$1" --input "$2" --out "$3" --gen "$4"
rc=$?; echo $rc > "$RC"; exit $rc

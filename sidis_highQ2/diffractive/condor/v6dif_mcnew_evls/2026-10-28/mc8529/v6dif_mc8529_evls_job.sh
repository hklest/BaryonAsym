#!/usr/bin/env bash
echo "=== mcnew-evls [$2] started $(date) on $(hostname) ==="
source /etc/profile.d/02-desygroupset.sh 2>&1 || true
desygroupset h1 2>&1 || true
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh 2>&1 || true
if [[ "${ROOT_OK:-0}" != "1" ]]; then echo "ERROR: H1OO env" >&2; exit 1; fi
mkdir -p "$(dirname "$3")"
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6 --stage evls --good-runs /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_0405.root --mode mc --steer "$1" -o "$3"
RC=$?
echo "=== mcnew-evls [$2] finished $(date) rc=${RC} ==="
exit ${RC}

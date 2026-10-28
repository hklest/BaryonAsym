#!/usr/bin/env bash
echo "=== mcnew-stage2 [$2] started $(date) on $(hostname) ==="
source /etc/profile.d/02-desygroupset.sh 2>&1 || true
desygroupset h1 2>&1 || true
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh 2>&1 || true
if [[ "${ROOT_OK:-0}" != "1" ]]; then echo "ERROR: H1OO env" >&2; exit 1; fi
mkdir -p "$(dirname "$3")"
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2_v6dif --mode mc --steer "$1" -o "$3" --diff-ods --diff-fpack-searchpath /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/condor/v6dif_mc840x_stage2/2026-10-28/mc8403/fpackfarm
RC=$?
echo "=== mcnew-stage2 [$2] finished $(date) rc=${RC} ==="
exit ${RC}

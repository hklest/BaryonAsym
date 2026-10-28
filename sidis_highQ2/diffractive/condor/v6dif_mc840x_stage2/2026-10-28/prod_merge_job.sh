#!/usr/bin/env bash
# args: reqid outfile listfile  (hadd of verified stage-2 chunks)
echo "=== mcnew-merge [$1] started $(date) on $(hostname) ==="
source /etc/profile.d/02-desygroupset.sh 2>&1 || true
desygroupset h1 2>&1 || true
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh 2>&1 || true
R=$1; OUT=$2; LST=$3
mkdir -p "$(dirname "$OUT")"
hadd -f "$OUT" $(cat "$LST")
RC=$?
echo "=== mcnew-merge [$R] finished $(date) rc=${RC} ==="
exit ${RC}

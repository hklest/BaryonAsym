#!/bin/bash
# WP-D MC-candidate Condor job, round 2 (h1-mc-expert 2026-10-28). args: <kind> <label>
#   kind=truth  : McCandidateTruthProbe on chunk 0000 (HAT+MODS, gen-level) of ReqID <label>
#   kind=hatscan: uproot HAT scan v2 over all HAT files of ReqID <label>
KIND=$1; LABEL=$2
B=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
S=$B/scripts/restart_2026-10-27/wp-d/mc_candidates
O=$B/data/restart_2026-10-27/wp-d/mc_candidates
mkdir -p $O
if [ "$KIND" = "truth" ]; then
  unset H1CURRENT H1 ROOT_OK
  source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; desygroupset h1 >/dev/null 2>&1
  source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
  cd $B
  exec $S/pkg_truth_probe/bin/x86_64-el9-gcc14-opt/McCandidateTruthProbe \
     --steer $S/pkg_truth_probe/steering/probe_${LABEL}_0000.steer --tag ${LABEL}_0000 \
     --out $O/probe_${LABEL}_0000.root --json $O/probe_${LABEL}_0000.json
elif [ "$KIND" = "hatscan" ]; then
  exec python3 $S/hat_scan_leq_v2.py $LABEL /pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/$LABEL $O/hatscan_v2_${LABEL}.jsonl
fi

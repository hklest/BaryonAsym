#!/bin/bash
# WP-D MC-candidate Condor job, round 3 (h1-mc-expert 2026-10-28): McCandidateXScan over one steering part. arg: <label>
LABEL=$1
B=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
S=$B/scripts/restart_2026-10-27/wp-d/mc_candidates
O=$B/data/restart_2026-10-27/wp-d/mc_candidates/r3
mkdir -p $O
unset H1CURRENT H1 ROOT_OK
source /etc/profile.d/02-desygroupset.sh >/dev/null 2>&1; desygroupset h1 >/dev/null 2>&1
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh >/dev/null 2>&1
cd $B
exec $S/pkg_xscan/bin/x86_64-el9-gcc14-opt/McCandidateXScan --steer $S/pkg_xscan/steering/xscan_${LABEL}.steer --tag $LABEL --out $O/xscan_${LABEL}.root

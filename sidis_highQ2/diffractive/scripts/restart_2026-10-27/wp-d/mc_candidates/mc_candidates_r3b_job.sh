#!/bin/bash
# WP-D round 3b (h1-mc-expert 2026-10-28): HAT-only full-range x_IP/M_Y scan. arg: <ReqID>
B=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive; S=$B/scripts/restart_2026-10-27/wp-d/mc_candidates; O=$B/data/restart_2026-10-27/wp-d/mc_candidates/r3; mkdir -p $O
exec python3 $S/hat_xpom_scan.py $1 /pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/$1 $O/hatxpom_$1.json

#!/bin/bash
# WP3 per-period post-processing: collect.py (consts_<run>.json, runs.tsv, summary.json) then bankdiff.py (bankdiff.txt).
# usage: consts_post.sh <period> [bankdiff]   (second arg: bank diff only, for an already collected period)
S=/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/consts/scripts
if [ "$2" != bankdiff ]; then python3 -I $S/collect.py $1 || exit 1; fi
python3 -I $S/bankdiff.py $1

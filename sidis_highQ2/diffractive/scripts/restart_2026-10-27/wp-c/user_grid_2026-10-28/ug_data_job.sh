#!/usr/bin/env bash
# user-grid data track skim wrapper. Usage: ug_data_job.sh <label> <input>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
export PYTHONDONTWRITEBYTECODE=1
python3 -B $BASE/scripts/restart_2026-10-27/wp-c/user_grid_2026-10-28/ug_data_tracks.py --label $1 --input $2 \
  --out $BASE/data/restart_2026-10-27/wp-c/user_grid_2026-10-28/skims/$1.npz \
  --runlist $BASE/data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv

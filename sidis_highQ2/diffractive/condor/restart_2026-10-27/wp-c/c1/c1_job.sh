#!/usr/bin/env bash
# WP-C C1 Condor wrapper.  Usage: c1_job.sh attach <label> | run <slice-args...>
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
S=$BASE/scripts/restart_2026-10-27; W=$BASE/data/restart_2026-10-27/wp-c/c1
export PYTHONDONTWRITEBYTECODE=1
echo "=== c1 $* start $(date -u +%FT%TZ) on $(hostname) ==="
python3 -c "import numpy, uproot; print('numpy', numpy.__version__, 'uproot', uproot.__version__)" || exit 3
case "$1" in
  attach) md5sum $S/wp-a5/attach_cells.py $S/wp-a2/n1_wpa2.py
          python3 -B $S/wp-a5/attach_cells.py --label "$2" --input $BASE/data/t2_detector/mc840x_merged_2026-10-28/$2.root --out $W/cells/cells_$2.npz; RC=$? ;;
  run)    shift; python3 -B $S/wp-c/c1/c1_run.py "$@"; RC=$? ;;
  *) echo "unknown mode $1"; exit 4 ;;
esac
echo "=== rc=$RC $(date -u +%FT%TZ) ==="; exit $RC

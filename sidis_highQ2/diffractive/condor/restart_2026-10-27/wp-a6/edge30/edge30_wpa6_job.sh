#!/usr/bin/env bash
# edge30_wpa6_job.sh - WP-A6 edge30 worker.  Usage: edge30_wpa6_job.sh sel30 <chain> <split> <out> | attach <input> <part> <out> | purity - - -
export PYTHONDONTWRITEBYTECODE=1
S=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-a6/edge30
echo "=== edge30 $* started $(date) on $(hostname) ==="
python3 -c "import numpy, uproot" >/dev/null 2>&1 || { echo "ERROR: numpy/uproot not importable" >&2; exit 1; }
case "$1" in
  sel30)  python3 $S/build_sel30_wpa6.py --chain "$2" --split "$3" --out "$4" ;;
  attach) python3 $S/attach_ec08_wpa6.py --input "$2" --part "$3" --out "$4" ;;
  purity) P=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a6
          python3 $S/purity_edge30_wpa6.py --scales $P/scales_resolved_odd_nominal.csv --g2 $P/edge30/f_nd_per_mx_SB30.csv \
            --frame B --xipcut 0.03 --rule D7plus --out $P/purity/binstats_wpa6_D7plus_B_xip030_edge30.csv --mask-date 2026-10-28_edge30 ;;
  *) echo "unknown mode $1" >&2; exit 1 ;;
esac
RC=$?; echo "=== edge30 rc=$RC $(date) ==="; exit $RC

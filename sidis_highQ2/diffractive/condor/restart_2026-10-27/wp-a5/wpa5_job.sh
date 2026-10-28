#!/usr/bin/env bash
# WP-A5 Condor wrapper (jobs of clusters 5931280/5931281 ran with the import check before md5sum; order swapped after review).  Usage: wpa5_job.sh hist            -> data/restart_2026-10-27/wp-a5/veto_corr_hists.json
#                                wpa5_job.sh attach <label>  -> data/restart_2026-10-27/wp-a5/cells_<label>.npz
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SD=$BASE/scripts/restart_2026-10-27/wp-a5; OD=$BASE/data/restart_2026-10-27/wp-a5
export PYTHONDONTWRITEBYTECODE=1
echo "=== wpa5 $1 $2 start $(date -u +%FT%TZ) on $(hostname) ==="
case "$1" in
  hist)   md5sum "$SD/veto_corr_hist.py"; python3 -c "import numpy, uproot; print('numpy', numpy.__version__, 'uproot', uproot.__version__)" || exit 3; python3 "$SD/veto_corr_hist.py" --out "$OD/veto_corr_hists.json"; RC=$? ;;
  attach) md5sum "$SD/attach_cells.py"; python3 -c "import numpy, uproot; print('numpy', numpy.__version__, 'uproot', uproot.__version__)" || exit 3
          if [ "$2" = "9012" ]; then IN=$BASE/data/t2_detector/mc9012_merged_2026-10-18/9012.root; else IN=$BASE/data/t2_detector/mc_merged_2026-10-18/$2.root; fi
          python3 "$SD/attach_cells.py" --label "$2" --input "$IN" --out "$OD/cells_$2.npz"; RC=$? ;;
  *) echo "unknown mode $1"; exit 4 ;;
esac
echo "=== rc=$RC $(date -u +%FT%TZ) ==="; exit $RC

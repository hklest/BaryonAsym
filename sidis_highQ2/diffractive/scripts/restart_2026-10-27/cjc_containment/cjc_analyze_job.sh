#!/usr/bin/env bash
# CJC-containment analysis job: 14 cjc_skim npz -> data/restart_2026-10-27/cjc_containment/cjc_containment.json
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/cjc_containment/cjc_analyze.py
OUT=$BASE/data/restart_2026-10-27/cjc_containment/cjc_containment.json
export PYTHONDONTWRITEBYTECODE=1
echo "=== cjc_analyze start $(date) on $(hostname) ==="; md5sum "$SCRIPT"
python3 -B "$SCRIPT" --out "$OUT"; RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

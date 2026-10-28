#!/usr/bin/env bash
# Gate-evidence histogram job: one job over the 17 WP-A3 skims -> data/restart_2026-10-27/gate_evidence/hists_ge.json
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/gate_evidence/hist_ge.py
OUT=$BASE/data/restart_2026-10-27/gate_evidence/hists_ge.json
echo "=== hist_ge start $(date) on $(hostname) ==="; md5sum "$SCRIPT"
python3 "$SCRIPT" --out "$OUT"; RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

#!/usr/bin/env bash
# Veto-efficiency job: one job over the 17 WP-A3 skims -> data/restart_2026-10-27/gate_evidence/veto_eff_hists.json
BASE=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive
SCRIPT=$BASE/scripts/restart_2026-10-27/gate_evidence/veto_eff_hist.py
OUT=$BASE/data/restart_2026-10-27/gate_evidence/veto_eff_hists.json
echo "=== veto_eff start $(date) on $(hostname) ==="; md5sum "$SCRIPT"
python3 "$SCRIPT" --out "$OUT"; RC=$?; echo "=== rc=$RC $(date) ==="; exit $RC

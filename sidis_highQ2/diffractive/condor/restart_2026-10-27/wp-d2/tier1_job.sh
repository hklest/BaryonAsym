#!/usr/bin/env bash
# WP-D2 tier-1 histogram stage (2 skims -> json).
python3 -B /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-d2/tier1_sw_check.py --stage hist; echo "rc=$?"

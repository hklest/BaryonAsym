#!/usr/bin/env bash
set -euo pipefail

# Build Tree*.steer files from produced event-list ROOT files.
# Uses klesthen/BaryonAsym paths only.

BASE="${BARYONASYM_BASE:-/data/dust/group/h1/klesthen/BaryonAsym}"

DATA_EV_DIR="$BASE/H1LambdaC/EventLists/2006"
DATA_STEER_DIR="$BASE/H1LambdaC/Steering/2006"

MC_EV_DIR="$BASE/H1LambdaC_MC/EventLists/RAPGAP31/2006"
MC_STEER_DIR="$BASE/H1LambdaC_MC/Steering/RAPGAP31/Rad/2006"

mkdir -p "$DATA_STEER_DIR" "$MC_STEER_DIR"

gen_data() {
  shopt -s nullglob
  local files=("$DATA_EV_DIR"/condor_06_evls_*.root)
  for f in "${files[@]}"; do
    local b idx out
    b="$(basename "$f")"
    idx="${b#condor_06_evls_}"; idx="${idx%.root}"
    out="$DATA_STEER_DIR/Tree06_${idx}.steer"
    cat > "$out" <<EOT
H1SteerTree() {fEventListFiles =  "$BASE/H1LambdaC/EventLists/2006/condor_06_evls_${idx}.root";}
EOT
    echo "Created $out"
  done
}

gen_mc() {
  shopt -s nullglob
  local files=("$MC_EV_DIR"/condor_06_evlsMC_RAPGAP31_Rad_*.root)
  for f in "${files[@]}"; do
    local b idx out
    b="$(basename "$f")"
    idx="${b#condor_06_evlsMC_RAPGAP31_Rad_}"; idx="${idx%.root}"
    out="$MC_STEER_DIR/TreeMC_RAPGAP31_06_RAD_${idx}.steer"
    cat > "$out" <<EOT
H1SteerTree() {fEventListFiles =  "$BASE/H1LambdaC_MC/EventLists/RAPGAP31/2006/condor_06_evlsMC_RAPGAP31_Rad_${idx}.root";}
EOT
    echo "Created $out"
  done
}

gen_data
gen_mc

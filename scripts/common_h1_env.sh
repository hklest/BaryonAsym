#!/usr/bin/env bash
set -euo pipefail

# Shared runtime environment for H1 jobs.
# Intentionally conservative: only sets variables if corresponding tools are available.

export SYSNAME="${SYSNAME:-amd64_rhel70}"

export ORACLE_HOME="${ORACLE_HOME:-/cvmfs/sft.cern.ch/lcg/releases/LCG_102/oracle/19.11.0.0.0/x86_64-centos7-gcc11-opt}"
export ORACLE_LIBS="${ORACLE_LIBS:--L$ORACLE_HOME/lib -lclntsh -lnnz19 -lmql1 -lclntshcore}"
export PATH="$ORACLE_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$ORACLE_HOME/lib:${LD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH="$ORACLE_HOME/lib/network:${LD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH="/cvmfs/sft.cern.ch/lcg/releases/LCG_102/libaio/0.3.110-1/x86_64-centos7-gcc11-opt/lib:${LD_LIBRARY_PATH:-}"

if command -v h1lcg >/dev/null 2>&1; then
  :
elif [[ -f /data/dust/group/h1/britzger/singularity_centos7/groupset.sh ]]; then
  # shellcheck disable=SC1091
  source /data/dust/group/h1/britzger/singularity_centos7/groupset.sh h1 || true
fi

if command -v h1lcg >/dev/null 2>&1; then
  h1lcg || true
fi

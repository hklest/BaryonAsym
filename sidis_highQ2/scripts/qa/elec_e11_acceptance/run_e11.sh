#!/bin/bash
set -e
env -u PYTHONHOME -u PYTHONPATH /usr/bin/python3 /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/qa/elec_e11_acceptance/e11_worker.py "$1" "$2"

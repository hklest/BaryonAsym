#!/usr/bin/env bash
source /etc/profile.d/02-desygroupset.sh 2>&1 || true
desygroupset h1 2>&1 || true
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh 2>&1 || true
export PYTHONDONTWRITEBYTECODE=1
python3 -B "$(dirname "$0")/tally_mc840x_sample.py" "$1"

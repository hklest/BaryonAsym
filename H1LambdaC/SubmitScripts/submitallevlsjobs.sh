#!/bin/zsh

####

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/CondorScripts

condor_submit multi_submit_create_evls_05
condor_submit multi_submit_create_evls_06
condor_submit multi_submit_create_evls_07
condor_submit multi_submit_create_evls_07low
condor_submit multi_submit_create_evls_07med


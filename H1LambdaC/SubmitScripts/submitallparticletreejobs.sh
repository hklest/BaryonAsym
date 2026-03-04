#!/bin/zsh

####

cd /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/CondorScripts

#condor_submit multi_submit_kaonTree05
#condor_submit multi_submit_kaonTree06
#condor_submit multi_submit_kaonTree07
#condor_submit multi_submit_kaonTree07low
#condor_submit multi_submit_kaonTree07med

condor_submit multi_submit_lambdaTree05
condor_submit multi_submit_lambdaTree06
condor_submit multi_submit_lambdaTree07
#condor_submit multi_submit_lambdaTree07low
#condor_submit multi_submit_lambdaTree07med

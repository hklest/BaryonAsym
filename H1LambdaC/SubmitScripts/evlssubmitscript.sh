#!/bin/zsh

####
export SYSNAME=amd64_rhel70

export ORACLE_HOME="/cvmfs/sft.cern.ch/lcg/releases/LCG_102/oracle/19.11.0.0.0/x86_64-centos7-gcc11-opt"
export ORACLE_LIBS="-L$ORACLE_HOME/lib -lclntsh -lnnz19 -lmql1 -lclntshcore"
export PATH="$ORACLE_HOME/bin:$PATH";
export LD_LIBRARY_PATH="$ORACLE_HOME/lib:$LD_LIBRARY_PATH";
export LD_LIBRARY_PATH="$ORACLE_HOME/lib/network:$LD_LIBRARY_PATH";
export LD_LIBRARY_PATH="/cvmfs/sft.cern.ch/lcg/releases/LCG_102/libaio/0.3.110-1/x86_64-centos7-gcc11-opt/lib:$LD_LIBRARY_PATH";

source /data/dust/group/h1/britzger/singularity_centos7/groupset.sh h1
h1lcg
####

echo " ------------------------------------------------------------------------ "
echo "Executable Inputs:"
echo "  STEERING:   $1"
echo "  OOUTPUT:    $2"
echo "  CHAIN:      $3"
echo " ------------------------------------------------------------------------ "
echo " "

#/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-centos7-gcc9-opt/create_evls -f /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/create_evls06.steer -o /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/test_evls.root

/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-centos7-gcc9-opt/create_evls -f $1 -o $2_$3.root

# Initialize variables
#start_number=1
#end_number=319

# Iterate through file numbers and create steer files
#for ((i = start_number; i < end_number; i++)); do
#    condor_rm 997598.$i
#done


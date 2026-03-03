#!/bin/zsh

#####

# Initialize variables
start_number=154
end_number=223

# Iterate through file numbers and create steer files
# need to edit job number in rm command by seeing job label with condor_q
for ((i = start_number; i < end_number; i++)); do
    condor_rm 19356728.$i
done


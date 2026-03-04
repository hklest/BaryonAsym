#!/bin/zsh

#####

# Initialize variables
start_number=6
end_number=9

# Iterate through file numbers and create steer files
# need to edit job number in rm command by seeing job label with condor_q
for ((i = start_number; i < end_number; i++)); do
    condor_rm 4328575.$i
done

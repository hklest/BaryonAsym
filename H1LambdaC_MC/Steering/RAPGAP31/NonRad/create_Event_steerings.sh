#!/bin/bash

# Location of the root files
#root_files_location="/pnfs/desy.de/dphep/online/h1/data/oo-4.0/05/"
#hat_files_prefix="hat.4.0.6.10dst7.c05"
#mods_files_prefix="mods.4.0.6.10dst7.c05"

# Initialize variables
start_number=0
end_number=3

# Iterate through file numbers and create steer files
for ((i = start_number; i < end_number; i++)); do
    output_file="/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/Steering/RAPGAP31/NonRad/RAPGAP31_07NORAD_${i}.steer"
        echo "H1SteerTree() {" > "$output_file"
        echo "   //2007 Data" >> "$output_file"
        echo "   fDirectoryNames=\"/nfs/dust/h1/group/britzger/norad_files/gc_out/rapgap31/\";" >> "$output_file"
        echo "   fLoadHAT  = true;" >> "$output_file"
	echo "   fLoadMODS  = true;" >> "$output_file"
	echo "   fLoadODS  = false;" >> "$output_file"
	echo "   fLoadUserTree  = false;" >> "$output_file"
        echo "}" >> "$output_file"
        echo "Created $output_file"
done

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
    output_file="/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/Steering/DJANGOH14/NonRad/DJANGOH14_07NORAD_${i}.steer"
        echo "H1SteerTree() {" > "$output_file"
        echo "   //2007 Data" >> "$output_file"
        echo "   fDirectoryNames=\"/data/dust/group/h1/britzger/norad_files/gc_out/djangoh14/\";" >> "$output_file"
	#echo "   fDirectoryNames=\"/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/djangoh14/\";" >> "$output_file"
        echo "   fLoadHAT  = true;" >> "$output_file"
	echo "   fLoadMODS  = true;" >> "$output_file"
	echo "   fLoadODS  = false;" >> "$output_file"
	echo "   fLoadUserTree  = false;" >> "$output_file"
        echo "}" >> "$output_file"
        echo "Created $output_file"
done

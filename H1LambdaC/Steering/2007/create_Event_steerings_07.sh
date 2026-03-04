#!/bin/bash

# Location of the root files
root_files_location="/pnfs/desy.de/dphep/online/h1/data/oo-4.0/07/"
hat_files_prefix="hat.4.0.6.10dst7.c07"
mods_files_prefix="mods.4.0.6.10dst7.c07"

# Initialize variables
start_number=0
end_number=112

# Iterate through file numbers and create steer files
for ((i = start_number; i < end_number; i++)); do
    file_number=$(printf "%04d" $i)
    file_name="${hat_files_prefix}$(printf "%03d" $i)"
    file_path="${root_files_location}${hat_files_prefix}${file_number}.root"
    output_file="/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Steering/2007/SelectedEvents07_${i}.steer"
    printf "${root_files_location}${hat_files_prefix}${file_number}.root \n"
    printf "$output_file\n"
    # Check if the file exists
    if [ -e "$file_path" ]; then
        echo "H1SteerTree() {" > "$output_file"
        echo "   //2007 Data" >> "$output_file"
        echo "   fHatFiles  = \"${root_files_location}${hat_files_prefix}${file_number}.root\";" >> "$output_file"
        echo "   fModsFiles  = \"${root_files_location}${mods_files_prefix}${file_number}.root\";" >> "$output_file"
        echo "}" >> "$output_file"
        echo "Created $output_file"
    fi
done

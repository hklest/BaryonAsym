#!/bin/bash

# Specify the common content for all files
line1="H1SteerTree() {fEventListFiles =  \""
line2="/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/2006/condor_06_evlsMC_DJANGOH14_Rad_"
line3=".root\";}"
# Specify the range of numbers for the file names

search_string="condor_06_evlsMC_DJANGOH14_Rad_"

# Specify the path to the directory containing files with the string
search_directory="/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/2006"

# Get the number of files with the specified string
file_count=$(grep -l "$search_string" "$search_directory"/* | wc -l)

# Check if there are files with the specified string
if [ "$file_count" -eq 0 ]; then
    echo "No files with the specified string found in $search_directory."
    exit 1
fi

# Specify the range of numbers for the file names
start_number=0
end_number=$file_count

# Loop to create files
for ((i = $start_number; i < $end_number; i++)); do
    # Generate the file name with a number
    file_name="TreeMC_DJANGOH14_06_RAD_$i.steer"
    
    # Create the file with common content and the specific number
    echo "$line1$line2$i$line3" > "$file_name"
    
    # Print a message for each created file
    echo "Created $file_name"
done

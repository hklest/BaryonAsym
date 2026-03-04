#!/bin/bash
#creating MC evls with the runs used in Daniel's low Q2 jet analysis, DJANGOH14

#8872 8873 8874 8875 8876 8877 8878 8879 8880 8881 8882 8883 8884 8885
#8886 8887 8888 8889 8890 8891 8732 8733 8734 8735 8736 8737 8738 8739
#8740 8741 8742 8743 8892 8893 8894 8895 8896 8897 8898 8899 8900 8901
#8902 8903 8904 8905 8906 8907 8908 8909 8910 8911 8912 8913 8914 8915
#8916 8917 8918 8919 8920 8921 8922 8723 8724 8725 8726 8727 8728 8729
#8730 8731  8923 8924 8925 8926 8715 8716 8717 8718 8719 8720 8721 8722
#8927 8928 8929 8709 8710 8711 8712 8713 8714

#Runs for 2006 e
#8892 (0-20 files), 8893 (0-20), 8894 (0-20), 8895 (0-20), 8896 (0-20), 8897 (0-20), 8898 (0-20), 8899 (0-20), 8900 (0-39), 8901 (0-39)
#8902 (0-39), 8903 (0-39), 8904 (0-39), 8905 (0-20), 8906 (0-20), 8907 (0-20), 8908 (0-20), 8909 (0-20), 8910 (0-38), 8911 (0-39), 8912 (0-39), 8913 (0-39), 8914 (0-39), 8915 (0-39)
#8916 (0-39), 8917 (0-20), 8918 (0-20), 8919 (0-50), 8920 (0-50), 8921 (0-26), 8922 (0-50)
#8723 (0-30), 8724 (0-30), 8725 (0-30), 8726 (0-42), 8727 (0-30), 8728 (0-30), 8729 (0-30)
#8730 (0-30), 8731 (0-30)

#Runs for 2006 p
#8923 (0-199), 8924 (0-190), 8925 (0-180), 8926 (0-140)
#8715 (1-30), 8716 (1-30), 8717 (1-30), 8718 (1-30), 8719 (1-30), 8720 (0-30), 8721 (1-30), 8722

# Location of the root files
#root_files_location="/pnfs/desy.de/dphep/online/h1/data/oo-4.0/06/"
root_files_location="/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/djangoh14/8892/"
#hat_files_prefix="hat.4.0.6.10dst7.c06"
hat_files_prefix="hat.4.0.25.DJANGOH14.DIS.NC.JOINT.LOWQ2.ELECTRON.RAD.W50.817.8892.S39600.R97905.ftt.DST." #0000.root
#mods_files_prefix="mods.4.0.6.10dst7.c06"
mods_files_prefix="mods.4.0.25.DJANGOH14.DIS.NC.JOINT.LOWQ2.ELECTRON.RAD.W50.817.8892.S39600.R97905.ftt.DST." #0000.root

# Initialize variables
start_number=0
end_number=20

# Iterate through file numbers and create steer files
for ((i = start_number; i <= end_number; i++)); do
    file_number=$(printf "%04d" $i)
    file_name="${hat_files_prefix}$(printf "%03d" $i)"
    file_path="${root_files_location}${hat_files_prefix}${file_number}.root"
    output_file="/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC_MC/Steering/DJANGOH14_06_${i}.steer"
    printf "${root_files_location}${hat_files_prefix}${file_number}.root \n"
    printf "$output_file\n"
    # Check if the file exists
    if [ -e "$file_path" ]; then
        echo "H1SteerTree() {" > "$output_file"
        echo "   //2006 Data" >> "$output_file"
        echo "   fHatFiles  = \"${root_files_location}${hat_files_prefix}${file_number}.root\";" >> "$output_file"
        echo "   fModsFiles  = \"${root_files_location}${mods_files_prefix}${file_number}.root\";" >> "$output_file"
        echo "}" >> "$output_file"
        echo "Created $output_file"
    fi
done

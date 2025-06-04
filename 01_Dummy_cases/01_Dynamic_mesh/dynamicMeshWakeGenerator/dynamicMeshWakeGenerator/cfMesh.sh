#!/bin/bash
cd ${0%/*} || exit 1    # Run from this directory

# Source tutorial run functions
. $WM_PROJECT_DIR/bin/tools/RunFunctions

# Define the array of folder names to check
folders=("1" "2")

# Loop through the array and check if each folder exists
for folder in "${folders[@]}"
do
    if [ -d "$folder" ]; then
        echo "Deleting folder \"$folder\"..."
        rm -rf "$folder"
        echo "Folder \"$folder\" deleted."
    else
        echo "Folder \"$folder\" does not exist. Continuing..."
    fi
done


# Define the pattern to check for
pattern="log.*"

# Find files matching the pattern
files=$(find . -maxdepth 1 -name "$pattern")

# Check if any files were found
if [ -z "$files" ]; then
    echo "No files matching the pattern \"$pattern\" found."
else
    echo "Deleting files matching the pattern \"$pattern\"..."
    rm -f $files
    echo "Files matching the pattern \"$pattern\" deleted."
fi


rm -r processor*

blockMesh | tee log.blockMesh
surfaceFeaturesEdges constant/triSurface/WakeGenerator.STL constant/triSurface/WakeGenerator.fms
cartesianMesh | tee log.cartesianMesh
#------------------------------------------------------------------------------

#!/bin/bash

PARENT_FOLDER=$"postProcessing/MP4/"

# Check if the given path is a directory
if [ ! -d "$PARENT_FOLDER" ]; then
    echo "Error: '$PARENT_FOLDER' is not a valid directory."
    exit 1
fi

# Loop through each subfolder
for FOLDER_PATH in "$PARENT_FOLDER"/*/; do
    if [ -d "$FOLDER_PATH" ]; then
        FOLDER_NAME=$(basename "$FOLDER_PATH")

        # Find the first file in the directory
        FILE_PATH=$(find "$FOLDER_PATH" -type f | head -n 1)

        # Check if a file exists
        if [ -z "$FILE_PATH" ]; then
            echo "Warning: No file found in '$FOLDER_PATH', skipping..."
            continue
        fi

        # Get file extension
        EXTENSION="${FILE_PATH##*.}"

        # Construct new file name
        NEW_FILE_NAME="$FOLDER_NAME.$EXTENSION"
        NEW_FILE_PATH="$FOLDER_PATH/$NEW_FILE_NAME"

        # Rename the file
        mv "$FILE_PATH" "$NEW_FILE_PATH"

        echo "Renamed '$FILE_PATH' to '$NEW_FILE_PATH'"

        # Copy the renamed file to the parent directory
        cp "$NEW_FILE_PATH" "$PARENT_FOLDER/"

        echo "Copied '$NEW_FILE_NAME' to '$PARENT_FOLDER/'"

        # Delete the subdirectory
        rm -rf "$FOLDER_PATH"
        echo "Deleted folder: '$FOLDER_PATH'"
    fi
done

echo "Batch rename, copy, and delete process completed."

#!/bin/bash

# Archive all useful files with tar

filename=servers.tar.gz
folder=servers
additionals_folders="MPI html_errors www makefile save.sh etoileBleue.conf"

# Removing the zip file if exists
if [ -f $filename ]; then
rm $filename
fi

# Create a dir container
mkdir $folder

# Copy all .c and .h file within it
cp -r sources headers $additionals_folders $folder

# Create archive file
tar -zcvf $filename $folder

# Copy archive to another dir
cp $filename ~/Git/

# Remove the folder
rm -r $folder

# echo
echo -e "\n--- SUCCESSFUL SAVED ---\n"
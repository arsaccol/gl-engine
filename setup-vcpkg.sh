#!/usr/bin/bash

echo "Setting up project for vcpkg"

# this assumes the vcpkg found by your path is a symlink
# you can remove the readlink invocation if this is not your case
vcpkg_dir_path=$(dirname $(readlink -f $(which vcpkg)))
cmake_file_path_rest="/scripts/buildsystems/vcpkg.cmake"
cmake_file_path="$vcpkg_dir_path$cmake_file_path_rest"


#echo $cmake_file_path

if [[ -f $cmake_file_path ]]; then 
    echo "File $cmake_file_path exists"
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$cmake_file_path

else
    echo "Error: file \$vcpkg_dir_path$cmake_file_path_rest not found"
    exit 1
fi




#cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=~/

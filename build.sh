#!/bin/bash

rm -rf build

if [ -z "${VCPKG_ROOT}" ]; then
  echo "Error: VCPKG_ROOT is not set." >&2
  exit 1
fi

cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DVCPKG_TARGET_TRIPLET=x64-linux

vcpkg install

cmake --build build

#!/bin/bash
if [ ! -d "build" ]; then
  mkdir build
fi
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/fuuulkrum7/1290_project/AbcUtils/build
cmake --build build -j $(nproc)
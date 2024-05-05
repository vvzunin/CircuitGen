#!/bin/bash
if [ ! -d "build" ]; then
  mkdir build
fi
cmake -S . -B build -G "Unix Makefiles"
cmake --build build -j $(nproc)
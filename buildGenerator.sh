#!/bin/bash
if [ ! -d "Generator/build" ]; then
  mkdir Generator/build
fi

cmake -S . -B Generator/build -G "Unix Makefiles"
cmake --build Generator/build -j $(nproc)

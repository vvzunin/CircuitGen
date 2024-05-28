#!/bin/bash
if [ ! -d "Generator/build" ]; then
  mkdir Generator/build
fi
 
cd Generator/build
cmake ..
cmake --build . -j $(nproc) 

cd ..

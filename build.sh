#!/bin/bash

if [ -d "build" ]; then
  rm -rf build
fi

if [ -d "bin" ]; then
  rm -rf bin
fi

mkdir bin

mkdir build && cd build

# cmake -G "MinGW Makefiles" ..
if [ "$(uname)" == "Linux" ]; then
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
else
  cmake -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
fi

make
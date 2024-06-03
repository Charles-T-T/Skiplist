#!/bin/bash

if [ -d "build" ]; then
    rm -rf build
fi

if [ -d "bin" ]; then
    rm -rf bin
fi

mkdir bin

mkdir build && cd build
cmake -G "MinGW Makefiles" ..
make
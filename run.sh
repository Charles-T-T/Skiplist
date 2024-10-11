#!/bin/bash
cd bin

if [[ " $* " == *" -t "* ]]; then
    ./stress_test
else
    ./menu
fi
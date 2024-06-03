#!/bin/bash
cd bin

if [[ " $* " == *" -t "* ]]; then
    ./stress-test
else
    ./menu
fi
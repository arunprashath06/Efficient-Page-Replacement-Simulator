#!/usr/bin/env bash
set -e
g++ src/pagereplacement.cpp -std=c++17 -O2 -o simulator
printf "7 0 1 2 0 3 0 4 2 3 0 3\n3\n5\n" | ./simulator | head -n 40

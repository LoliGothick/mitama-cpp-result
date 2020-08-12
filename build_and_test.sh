#!/bin/bash

mkdir -p build
cd build || exit
cmake .. && make -j11
./maybe-tests
./result-tests

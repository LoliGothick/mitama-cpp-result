#!/bin/bash

mkdir -p build
cd build
cmake .. \
    -DBOOST_ROOT=/opt/boost/boost_1_70_0/
make -j10 && ctest -j2

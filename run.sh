#!/bin/sh

mkdir -p build 
cd build

cmake cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. && make

clear
./sim

cd ..
rm -rf build
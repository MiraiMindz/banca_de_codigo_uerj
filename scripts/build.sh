#!/usr/bin/env bash

CDIR=$(pwd)

cd ../include/external/glew/
make GLEW_STATIC=1
cd "$CDIR"

cd ../include/external/glfw/
mkdir build
cd build
cmake ..
make
cd "$CDIR"

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build --parallel $(nproc)
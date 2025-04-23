#!/bin/bash

# Exit on any error
set -e

# Create build directory if it doesn't exist
mkdir -p build

# Clean previous build
rm -rf build/*

cd build && cmake .. && make 
# && ctest --output-on-failure -V
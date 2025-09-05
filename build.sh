#!/bin/bash

# Consolidated build script for Hello World C program
# Usage: ./build.sh [build_type] [output_name]
# Build types: basic, dev, debug, opt, strict
# Default: dev (development build with warnings)

BUILD_TYPE=${1:-dev}
OUTPUT_NAME=${2:-hello}

case $BUILD_TYPE in
    basic)
        echo "Building basic version..."
        gcc -o "$OUTPUT_NAME" hello.c
        ;;
    dev)
        echo "Building development version (with warnings)..."
        gcc -Wall -Wextra -o "$OUTPUT_NAME" hello.c
        ;;
    debug)
        echo "Building debug version..."
        gcc -g -Wall -Wextra -o "${OUTPUT_NAME}_debug" hello.c
        ;;
    opt)
        echo "Building optimized version..."
        gcc -O2 -Wall -Wextra -o "$OUTPUT_NAME" hello.c
        ;;
    strict)
        echo "Building with strict warnings..."
        gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o "$OUTPUT_NAME" hello.c
        ;;
    *)
        echo "Usage: $0 [build_type] [output_name]"
        echo "Build types:"
        echo "  basic  - Basic compilation: gcc -o hello hello.c"
        echo "  dev    - Development (default): gcc -Wall -Wextra -o hello hello.c"
        echo "  debug  - Debug build: gcc -g -Wall -Wextra -o hello_debug hello.c"
        echo "  opt    - Optimized: gcc -O2 -Wall -Wextra -o hello hello.c"
        echo "  strict - Strict warnings: gcc -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wsign-conversion -o hello hello.c"
        exit 1
        ;;
esac

if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed!"
    exit 1
fi
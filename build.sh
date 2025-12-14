#!/bin/bash
# Build script for Visual Cryptography Project (Linux/macOS)
# Reorganized folder structure

echo "==============================================="
echo "Visual Cryptography Project - Build Script"
echo "==============================================="
echo ""

# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "ERROR: g++ compiler not found!"
    exit 1
fi

echo "Compiler found:"
g++ --version | head -n 1
echo ""

# Create bin directory
mkdir -p bin

echo "Building main program..."
g++ -std=c++11 -Wall -O2 -o bin/vc_program \
    src/main.cpp src/image_utils.cpp src/vcs.cpp src/rg.cpp src/dhcod.cpp || exit 1

echo "Building analysis tool..."
g++ -std=c++11 -Wall -O2 -o bin/analyze \
    src/analyze.cpp src/image_utils.cpp src/vcs.cpp src/rg.cpp src/dhcod.cpp || exit 1

echo ""
echo "==============================================="
echo "BUILD SUCCESSFUL!"
echo "==============================================="
echo ""
echo "Executables created in bin/ directory:"
echo "  - vc_program  (Main program)"
echo "  - analyze     (Analysis tool)"
echo ""
echo "To run:"
echo "  bin/vc_program"
echo "  bin/analyze"
echo ""

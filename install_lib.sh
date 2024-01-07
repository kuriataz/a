#!/bin/bash

# Define the installation directory (change as needed)
INSTALL_DIR="~/test/"

# Create the installation directory if it doesn't exist
mkdir -p "$INSTALL_DIR"

# Build and install the library
mkdir -p build
cd build
cmake ..
make
make install DESTDIR="$INSTALL_DIR"

echo "Library installed to $INSTALL_DIR"

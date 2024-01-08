#!/bin/bash

# Check if a path is provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path>"
    exit 1
fi

# Get the provided path
install_path="$1"

# Create the directory if it doesn't exist
mkdir -p "$install_path"

# Copy necessary files to the installation directory
cp -r * "$install_path"

# Move to the installation directory
cd "$install_path" || exit

# Initialize and update submodules (assuming you have a submodule in your project)
git submodule update --init --recursive

# Build the project with Ninja
cmake -GNinja -Bbuild
ninja -Cbuild

# Install the library and executable
# sudo cmake --install .

# Display a message indicating the installation is complete
echo "Installation complete. Your allocator library is located in: $install_path"


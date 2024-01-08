#!/bin/bash

git clone https://github.com/kuriataz/allocator.git
cd allocator/dependencies
# Initialize and update submodules (assuming you have a submodule in your project)
git submodule update --init --recursive

# Build the project with Ninja
cmake -GNinja -Bbuild
ninja -Cbuild

# Install the library and executable
# sudo cmake --install .

# Display a message indicating the installation is complete
echo "Installation complete."


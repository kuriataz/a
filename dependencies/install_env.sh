#!/bin/bash

# Update system packages
sudo apt update
apt upgrade -y

# Install essential development tools
 apt install -y build-essential cmake ninja-build

# Install version control tools
 apt install -y git

# Install Valgrind for memory debugging
 apt install -y valgrind

# Install Clang for additional development tools
 apt install -y clang

# Install dependencies for Catch2 (testing framework)
 apt install -y catch

# Install clang-tidy (static analysis tool)
 apt install -y clang-tools

 apt install -y clang-tidy

 apt install libstdc++-12-dev

echo "Development environment setup complete."

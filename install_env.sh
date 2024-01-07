#!/bin/bash

# Update system packages
sudo apt update
sudo apt upgrade -y

# Install essential development tools
sudo apt install -y build-essential cmake ninja-build

# Install version control tools
sudo apt install -y git

# Install Valgrind for memory debugging
sudo apt install -y valgrind

# Install Clang for additional development tools
sudo apt install -y clang

# Install dependencies for Catch2 (testing framework)
sudo apt install -y catch

# Install clang-tidy (static analysis tool)
sudo apt install -y clang-tools

# Install additional tools (adjust as needed)
# sudo apt install -y <your_additional_tool>

echo "Development environment setup complete."

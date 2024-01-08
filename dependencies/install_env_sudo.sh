#!/bin/bash

sudo apt update
sudo apt upgrade -y

sudo apt install -y build-essential cmake ninja-build

sudo apt install -y git

sudo apt install -y valgrind

sudo apt install -y clang

sudo apt install -y catch

sudo apt install -y clang-tools

sudo apt install -y clang-tidy

sudo apt install libstdc++-12-dev

echo "Development environment setup complete."
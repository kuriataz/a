#!/bin/bash

apt update
apt upgrade -y

apt install -y build-essential cmake ninja-build

apt install -y git

apt install -y valgrind

apt install -y clang

apt install -y catch

apt install -y clang-tools

apt install -y clang-tidy

apt install libstdc++-12-dev

echo "Development environment setup complete."

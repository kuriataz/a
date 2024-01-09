# allocator

Custom Allocator Library

This is a custom allocator library designed to provide memory allocation and deallocation functionalities with additional tracking and statistics.
Features

    First Fit Algorithm: The allocator uses the First Fit algorithm to allocate memory. It searches for the first large enough free block or creates a new one if needed.

    Memory Block Tracking: Each allocated memory block is tracked, including its size, location, allocation file, and line. This tracking helps identify memory issues.

    Memory Joining: The allocator attempts to join free blocks together for efficient memory utilization.

    Statistics: The library maintains statistics about memory usage, allocation count, deallocation count, and more.


Usage

    Get started:
        1. Clone this repo: git clone https://github.com/kuriataz/allocator.git
        2. Change directory: cd allocator/
        3. Prepare environment: chmod +x install_env.sh ./install_env.sh
        4. Update submodule: git submodule update --init --recursive
        5. Build project:
            cmake -GNinja -Bbuild
            ninja -Cbuild

    Run regression: ninja -Cbuild regression
    Run tests: build/tests
    Run main: build/allocator

    Compile your own program (assumig you are in allocator/ directory):
        1. Create cpp file (for example try.cpp)
        2. Include headers: "allocator.hpp" and "allocator_utils.hpp"
        3. Compile: g++ -o try try.cpp -I./ build/liballocator_lib.a

    Example Usage:

        #include "allocator.hpp"
        #include "allocator_utils.hpp"

        int main() {
            // Allocate memory
            int *intArray = reinterpret_cast<int *>(heaven::allocate(10 * sizeof(int)));

            // Use the allocated memory

            // Check the blocks
            heaven::memory_dump();

            // Deallocate memory
            heaven::deallocate(intArray);

            // Perform final deallocation
            heaven::final_deallocate();

            return 0;
        }

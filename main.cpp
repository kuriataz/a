#include "allocator.hpp"
#include "allocator_utils.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

int main() {
  using namespace heaven;

  int *intArray1 = reinterpret_cast<int *>(allocate(10 * sizeof(int)));
  int *intArray2 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));

  if (intArray1 && intArray2) {
    for (int i = 0; i < 5; ++i) {
      intArray1[i] = i;
    }

    for (int i = 0; i < 10; ++i) {
      intArray2[i] = i;
    }

    std::cout << "intArray1: ";
    for (int i = 0; i < 5; ++i) {
      std::cout << intArray1[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "intArray2: ";
    for (int i = 0; i < 10; ++i) {
      std::cout << intArray2[i] << " ";
    }
    std::cout << std::endl;

  } else {
    std::cerr << "memory allocation failed\n";
  }
  memory_dump();
  final_deallocate();

  return 0;
}

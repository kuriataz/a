#include "allocator.hpp"
#include <iostream>

int main() {
  int *arr1 =
      static_cast<int *>(heaven::allocate(3 * sizeof(int), "try.cpp", 5));
  arr1[0] = 0;
  arr1[1] = 1;
  arr1[2] = 2;
  std::cout << arr1[0] << "\n";
  std::cout << arr1[1] << "\n";
  std::cout << arr1[2] << "\n";
  heaven::final_deallocate();
  return 0;
}
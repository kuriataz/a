#include "allocator.hpp"
#include "allocator_utils.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

// auto extract_file_name(const char *file) {
//   const char *filename = strrchr(file, '/');
// #ifdef _WIN32
//   if (!filename) {
//     filename = strrchr(path, '\\');
//   }
// #endif
//   return filename ? filename + 1 : file;
// }
// #define FILE_NAME extract_file_name(__FILE__)
// #define allocate(bytes) allocate(bytes, FILE_NAME, __LINE__)
// // #define allocate(bytes) allocate(bytes, __FILE__, __LINE__)

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

  // deallocate(intArray2);
  // int *intArray3 = reinterpret_cast<int *>(allocate(3 * sizeof(int)));
  // int *intArray4 = reinterpret_cast<int *>(allocate(1 * sizeof(int)));
  // int *intArray5 = reinterpret_cast<int *>(allocate(4 * sizeof(int)));

  // // deallocate(intArray3);
  // int *intArray6 = reinterpret_cast<int *>(allocate(2 * sizeof(int)));
  // // deallocate(intArray4);
  // int *intArray7 = reinterpret_cast<int *>(allocate(2 * sizeof(int)));
  // if (intArray3) {
  //   for (int i = 0; i < 3; ++i) {
  //     intArray1[i] = i;
  //   }

  //   std::cout << "intArray3: ";
  //   for (int i = 0; i < 3; ++i) {
  //     std::cout << intArray1[i] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  memory_dump();
  final_deallocate();

  return 0;
}

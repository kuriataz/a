#include "allocator.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>

const char *extract_name(const char *file) {
  const char *filename = strrchr(file, '/');
#ifdef _WIN32
  if (!filename) {
    filename = strrchr(path, '\\');
  }
#endif
  return filename ? filename + 1 : file;
}
#define FILE_NAME extract_name(__FILE__)
#define allocate(bytes) allocate(bytes, FILE_NAME, __LINE__)

namespace heaven {
TEST_CASE("Allocating", "[allocator]") {
  int *intArray1 = reinterpret_cast<int *>(allocate(10 * sizeof(int)));
  int *intArray2 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));
  REQUIRE(intArray1 != nullptr);
  REQUIRE(intArray2 != nullptr);
  deallocate(intArray1);
  deallocate(intArray2);
  final_deallocate();
}

TEST_CASE("Invalid Deallocation", "[allocator]") {
  std::stringstream output_stream;
  std::streambuf *old_cout_buffer = std::cerr.rdbuf(output_stream.rdbuf());

  int *intArray1 = reinterpret_cast<int *>(allocate(10 * sizeof(int)));
  deallocate(intArray1);
  deallocate(intArray1);

  deallocate(nullptr);

  int *intArray2 = reinterpret_cast<int *>(malloc(10 * sizeof(int)));

  deallocate(intArray2);
  free(intArray2);

  std::cerr.rdbuf(old_cout_buffer);

  std::string output_content = output_stream.str();

  REQUIRE(output_content.find("invalid parameters for deallocate") !=
          std::string::npos);
  REQUIRE(output_content.find("block not found") != std::string::npos);
  REQUIRE(output_content.find("memory block is already deallocated") !=
          std::string::npos);
  final_deallocate();
}

TEST_CASE("Memory Reusing", "[allocator]") {
  int *intArray1 = reinterpret_cast<int *>(allocate(10 * sizeof(int)));
  int *intArray2 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));

  deallocate(intArray1);
  int *intArray3 = reinterpret_cast<int *>(allocate(10 * sizeof(int)));

  REQUIRE(intArray1 == reinterpret_cast<int *>(intArray3));

  deallocate(intArray2);
  deallocate(intArray3);
  final_deallocate();
}
TEST_CASE("Memory Joining", "[allocator]") {
  int *intArray1 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));
  int *intArray2 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));
  int *intArray3 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));

  deallocate(intArray1);
  deallocate(intArray2);

  int *intArray4 = reinterpret_cast<int *>(
      allocate(52 * sizeof(int))); // 52 * 4 = (20 + 20) * 4 + 48
  REQUIRE(intArray2 == reinterpret_cast<int *>(intArray4));

  deallocate(intArray3);
  deallocate(intArray4);

  final_deallocate();
}

// not working
TEST_CASE("Memory Spliting", "[allocator]") {
  int *intArray1 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));
  int *intArray2 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));
  int *intArray3 = reinterpret_cast<int *>(allocate(20 * sizeof(int)));

  deallocate(intArray1);
  deallocate(intArray2);

  int *intArray4 = reinterpret_cast<int *>(allocate(10 * sizeof(int)));
  int *intArray5 = reinterpret_cast<int *>(allocate(30 * sizeof(int)));
  REQUIRE(intArray2 == reinterpret_cast<int *>(intArray4));
  memory_dump();

  deallocate(intArray3);
  deallocate(intArray4);

  final_deallocate();
}

} // namespace heaven
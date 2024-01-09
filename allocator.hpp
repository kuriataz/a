#pragma once

#include <cstddef>
#include <cstring>
#include <pthread.h>
#include <string>
#include <vector>

namespace heaven {
struct Stats {
  int alloc_num = 0;
  std::vector<std::string> alloc_files;
  std::vector<int> alloc_lines;
  int dealloc_num = 0;
  std::vector<std::string> dealloc_files;
  std::vector<int> dealloc_lines;
  size_t total_bytes = 0;
  size_t mem_peak = 0;
  size_t bytes_now = 0;
  size_t avg_bytes = 0;
  int sbrk_counter = 0;
  int damaged_blocks = 0;
  int not_freed = alloc_num - dealloc_num;

  void print_stats();
};

void *allocate(size_t size, const char *file, size_t const line);
void deallocate(void *ptr);
void final_deallocate();

void memory_dump();

} // namespace heaven

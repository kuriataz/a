#include "allocator.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define MAGIC_NUMBER 41

namespace heaven {

struct Memory_Block {
  int magic_number;       // to check if block isn't damaged
  size_t size;            // number of objects * sizeof(T)
  void *memory;           // addres to the begin of memory allocated for data
  Memory_Block *next;     // next node
  const char *file = "-"; // file where block was allocated
  int line = 0;           // line where block was allocated
  bool free = true;       // true if block is deallocated
};
struct Allocator {
private:
  Stats stats;
  pthread_mutex_t mutex;
  Memory_Block *memory_head = nullptr;

public:
  Allocator() { pthread_mutex_init(&mutex, nullptr); }

  ~Allocator() = default;

  // [[nodiscard]] pointer allocate(size_type size);
  [[nodiscard]] void *allocate(size_t size, const char *file, size_t line);
  void deallocate(void *block);
  void final_deallocate();
  void memory_dump();
  Stats get_stats();
};

Allocator allocator;
bool initialised = false;

void stats_atexit() {
  heaven::Stats stats = allocator.get_stats();
  stats.print_stats();
}
void deallocate_atexit() { allocator.final_deallocate(); }

void initialise() {
  initialised = true;

  std::atexit(stats_atexit);
  // std::atexit(deallocate_atexit);
}

void *allocate(size_t size, const char *file, size_t const line) {
  if (!initialised) {
    initialise();
  }
  return allocator.allocate(size, file, line);
}

void deallocate(void *ptr) {
  if (!initialised) {
    initialise();
  }
  allocator.deallocate(ptr);
}

void final_deallocate() {
  if (!initialised) {
    initialise();
  }
  allocator.final_deallocate();
}

void memory_dump() { allocator.memory_dump(); }
/**
  Function to allocate memory.
  @param size - bytes to allocate
  @param file - file where the function is called
  @param line - line where the function is called
  @return pointer to the addres where the memory for allocated data begins

  It works with the First_Fit algorithm - allocate the fisrt large enough block
  of memory. If there isn't large enough and free block, the new one is created
  and added to the begining of the list of blocks.
*/
void *Allocator::allocate(size_t const size, const char *file,
                          size_t const line) {
  if (size == 0) {
    std::cerr << "nothing to allocate\n";
    return nullptr;
  }

  // FIRST FIT
  pthread_mutex_lock(&mutex);
  Memory_Block *current_block = memory_head;
  while (current_block) {
    if (current_block->size >= size && current_block->free) {
      // apropirate block was found
      // if (current_block->size > sizeof(Memory_Block) + size + 10) {
      //   // block is large enough to split
      //   Memory_Block *new_block = reinterpret_cast<Memory_Block *>(
      //       static_cast<char *>(current_block->memory) + size);
      //   new_block->next = current_block->next;
      //   new_block->size = current_block->size - size - sizeof(Memory_Block);
      //   new_block->memory =
      //       reinterpret_cast<char *>(new_block) + sizeof(Memory_Block);
      //   new_block->free = true;
      //   new_block->magic_number = MAGIC_NUMBER;
      //   new_block->file = "-";
      //   new_block->line = 0;

      //   current_block->next = new_block;
      //   current_block->size = size;
      // }
      if (current_block->magic_number != MAGIC_NUMBER) {
        std::cerr << "memory block is damaged\n";
        // abort();
        ++stats.damaged_blocks;
        pthread_mutex_unlock(&mutex);
        return nullptr;
      }
      current_block->free = false;
      current_block->file = file;
      current_block->line = line;
      ++stats.alloc_num;
      stats.bytes_now += size;
      stats.mem_peak = std::max(stats.mem_peak, stats.bytes_now);

      pthread_mutex_unlock(&mutex);
      return current_block->memory;
    }
    current_block = current_block->next;
  }

  // apropirate block not found - create new one
  size_t total_size = sizeof(Memory_Block) + size;
  stats.total_bytes += total_size;
  void *raw_memory;

  raw_memory = sbrk(total_size);

  if (raw_memory == (void *)-1) {
    std::cerr << "sbrk fails\n";
    pthread_mutex_unlock(&mutex);
    return nullptr;
  }
  ++stats.sbrk_counter;

  Memory_Block *new_block = reinterpret_cast<Memory_Block *>(raw_memory);
  new_block->free = false;
  new_block->magic_number = MAGIC_NUMBER;
  new_block->size = size;
  new_block->memory =
      reinterpret_cast<char *>(new_block) + sizeof(Memory_Block);
  new_block->file = file;
  new_block->line = line;
  // it is the fisrt block now
  new_block->next = memory_head;
  memory_head = new_block;

  stats.alloc_files.push_back(file);
  stats.alloc_lines.push_back(line);
  stats.mem_peak = std::max(stats.mem_peak, stats.bytes_now);
  ++stats.alloc_num;
  stats.bytes_now += size;
  pthread_mutex_unlock(&mutex);

  return new_block->memory;
}

/**
  Function to deallocate allocated memory.
  @param block - pointer to the block that will be deallocated
  @return void

  It search for the block in lists of blocks - starting from memory_head. If the
  block wasn't found it prints the message to std::cerr.
  Deallocation is based on marking the block as free - it can be allocated
  again.
  If the block before or after the deallocating one is also free, it joins
  them together.
*/
void Allocator::deallocate(void *block) {

  if (!block) {
    std::cerr << "invalid parameters for deallocate\n";
    return;
  }

  pthread_mutex_lock(&mutex);
  Memory_Block *current_block = memory_head;
  Memory_Block *previous_block = nullptr;

  while (!(current_block == nullptr)) {
    if (current_block->memory == static_cast<void *>(block)) {
      // block to deallocate was found
      if (current_block->magic_number != MAGIC_NUMBER) {
        std::cerr << "memory block is damaged\n";
        // abort();
        ++stats.damaged_blocks;
        pthread_mutex_unlock(&mutex);
        return;
      }
      if (current_block->free) {
        std::cerr << "memory block is already deallocated\n";
        pthread_mutex_unlock(&mutex);
        return;
      }
      current_block->free = true;
      current_block->file = "-";
      current_block->line = 0;
      ++stats.dealloc_num;
      // stats.bytes_now -= size;

      // try to join with the next block
      if (current_block->next && current_block->next->free) {
        current_block->size += current_block->next->size + sizeof(Memory_Block);
        current_block->next = current_block->next->next;
      }

      // try to join with the previous block
      if (previous_block && previous_block->free) {
        previous_block->size += current_block->size + sizeof(Memory_Block);
        previous_block->next = current_block->next;
      }
      pthread_mutex_unlock(&mutex);
      return;
    }

    previous_block = current_block;
    current_block = current_block->next;
  }
  pthread_mutex_unlock(&mutex);
  std::cerr << "block not found\n";
}

/**
  Function to release all allocated memory.
  Calls brk on every block and removes it from list.
  It starts from memory_head.
*/
void Allocator::final_deallocate() {

  if (!memory_head) {
    std::cerr << "invalid parameters for deallocate\n";
    return;
  }

  pthread_mutex_lock(&mutex);
  Memory_Block *current_block = memory_head;
  Memory_Block *previous_block = nullptr;

  while (!(current_block == nullptr)) {
    if (current_block->magic_number != MAGIC_NUMBER) {
      std::cerr << "memory block is damaged\n";
      ++stats.damaged_blocks;
      pthread_mutex_unlock(&mutex);
      return;
    }
    if (!(previous_block == nullptr)) {
      previous_block->next = current_block->next;
    } else {
      memory_head = current_block->next;
    }

    if (brk(current_block->memory) == -1) {
      std::cerr << "brk fails\n";
      pthread_mutex_unlock(&mutex);
      return;
    }
    previous_block = current_block;
    current_block = current_block->next;
  }
  pthread_mutex_unlock(&mutex);
}

/**
  Function to print all the info about blocks.
  - start: addres of the block.
  - end: addres of the end of the block (i hope so).
  - size: size of allocated data, 0 if empty.
  - file: file where the block was allocated.
  - line: line where the block was allocated.
  - free: 1 if the block is free(deallocated), 0 if not
*/
void Allocator::memory_dump() {

  pthread_mutex_lock(&mutex);
  Memory_Block *current_block = memory_head;
  Memory_Block *previous_block = nullptr;

  std::cout << "MEMORY_DUMP:\n";
  int i = 0;
  while (!(current_block == nullptr)) {
    ++i;
    if (current_block->magic_number != MAGIC_NUMBER) {
      std::cerr << "memory block is damaged\n";
      ++stats.damaged_blocks;
      pthread_mutex_unlock(&mutex);
      return;
    }
    void *const block_end = reinterpret_cast<char *>(current_block) +
                            current_block->size + sizeof(Memory_Block);

    std::cout << "{Block" << i << ": start " << current_block << ", end "
              << block_end << ", size " << current_block->size << ", file "
              << current_block->file << ", line " << current_block->line
              << ", free " << current_block->free << "}\n";
    previous_block = current_block;
    current_block = current_block->next;
    pthread_mutex_unlock(&mutex);
  }
}

Stats Allocator::get_stats() {

  stats.avg_bytes = stats.total_bytes / stats.sbrk_counter;
  return stats;
}

/**
  Function to print the statistics at the end of program.
*/
void Stats::print_stats() {
  std::cout << "\nSTATISTICS:" << "\nsizeof(Memory_Block): "
            << sizeof(Memory_Block) << "\nalloc_num: " << alloc_num
            << "\ndealloc_num: " << dealloc_num
            << "\ntotal_bytes: " << total_bytes << "\nmem_peak: " << mem_peak
            << "\navg_bytes: " << avg_bytes
            << "\nsbrk_counter: " << sbrk_counter
            << "\ndamaged_blocks: " << damaged_blocks
            << "\nnot_freed: " << not_freed << '\n';

  if (!(alloc_files.empty()) && alloc_files.size() == alloc_lines.size()) {
    for (int i = 0; i != alloc_files.size(); ++i) {
      std::cout << "file: " << alloc_files[i] << " line: " << alloc_lines[i]
                << "\n";
    }
  }
}
} // namespace heaven

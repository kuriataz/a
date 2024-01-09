#pragma once

#include <cstring>

#ifdef _WIN32
#define FILE_SEPARATOR '\\'
#else
#define FILE_SEPARATOR '/'
#endif

const char *extract_name(const char *file) {
  const char *filename = strrchr(file, FILE_SEPARATOR);
  return filename ? filename + 1 : file;
}

#define FILE_NAME extract_name(__FILE__)
#define allocate(bytes) allocate(bytes, FILE_NAME, __LINE__)

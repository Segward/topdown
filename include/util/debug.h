#pragma once

#include "pch.h"

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : \
                (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

#define LOG(S, ...) \
  printf("[LOG] %s:%d " S "\n", FILENAME, __LINE__, ##__VA_ARGS__)

#define ERROR(S, ...) \
  fprintf(stderr, "[ERROR] %s:%d " S "\n", FILENAME, __LINE__, ##__VA_ARGS__)

#define ASSERT(A, B, ...) { \
  __typeof(A) _a = (A); \
  __typeof(B) _b = (B); \
  if (_a != _b) { \
    ERROR(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
  } \
}

#define ASSERT_FALSE(A, B, ...) { \
  __typeof(A) _a = (A); \
  __typeof(B) _b = (B); \
  if (_a == _b) { \
    ERROR(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
  } \
}

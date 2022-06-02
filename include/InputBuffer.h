#ifndef INCLUDE_BUFF_H
#define INCLUDE_BUFF_H
#include <stdint.h>
typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;
#endif

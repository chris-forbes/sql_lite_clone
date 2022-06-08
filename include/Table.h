#ifndef TABLE_H
#define TABLE_H
#include "Row.h"
#include "Pager.h"
#include <stdint.h>
#define u32 uint32_t

typedef struct {
  u32 num_rows;
  Pager* pager;
}Table;

#endif

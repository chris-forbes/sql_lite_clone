#ifndef DB_PAGER_H
#define DB_PAGER_H

#include <stdint.h>
#include "DataCommon.h"

#define u32 uint32_t

typedef struct {
  int file_descriptor;
  u32 file_length;
  void* pages[TABLE_MAX_PAGES];
} Pager;


void pager_flush(Pager* pager, u32 page_num, u32 size);
Pager* pager_open(const char* filename);
void* get_page(Pager* pager, const u32 page_num);

#endif 

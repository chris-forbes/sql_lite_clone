#ifndef CURSOR_H
#define CURSOR_H
#include "Table.h"
#include <stdbool.h>
#include <stdint.h>
#define u32 uint32_t

typedef struct {
  Table* table;
  u32 row_num;
  bool end_of_table;
}Cursor;

void* cursor_value(Cursor* cursor);
void cursor_advance(Cursor* cursor);
Cursor* table_end(Table* table);
Cursor* table_start(Table* table);
#endif

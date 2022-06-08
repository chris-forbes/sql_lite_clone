#include "Cursor.h"
#include "DataCommon.h"
#include "Pager.h"
#include <stdlib.h>

void* cursor_value(Cursor* cursor) {
  u32 row_num = cursor -> row_num;
  u32 page_num = row_num / ROWS_PER_PAGE;
  void* page = get_page(cursor->table->pager, page_num);
  u32 row_offset = row_num % ROWS_PER_PAGE;
  u32 byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset; 
}

void cursor_advance(Cursor* cursor) {
  cursor -> row_num += 1;
  if(cursor -> row_num >= cursor -> table -> num_rows) {
    cursor -> end_of_table = true;
  }
}

Cursor* table_end(Table* table) {
  Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
  cursor -> table = table;
  cursor -> row_num = table -> num_rows;
  cursor -> end_of_table = true;
  return cursor;
}

Cursor* table_start(Table* table) {
  Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
  cursor -> table = table;
  cursor -> row_num = 0;
  cursor -> end_of_table = (table->num_rows == 0);
  return cursor; 
}

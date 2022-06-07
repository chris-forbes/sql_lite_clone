#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "Row.h"
#include "EnumDef.h"
#include "InputBuffer.h"
#include "DataCommon.h"
#include "Pager.h"

const u32 ID_SIZE = sizeof_attr(Row, id);
const u32 USERNAME_SIZE = sizeof_attr(Row, username);
const u32 EMAIL_SIZE = sizeof_attr(Row, email);
const u32 ID_OFFSET = 0;
const u32 USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const u32 EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const u32 ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;
const u32 PAGE_SIZE = 4096;
const u32 ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const u32 TABLE_MAX_ROWS = ROWS_PER_PAGE;

typedef struct {
	StatementType type;
	Row row_to_insert;
}Statement;


typedef struct {
  u32 num_rows;
  Pager* pager;
}Table;

typedef struct {
  Table* table;
  u32 row_num;
  bool end_of_table; // indicates 1 past the last element
} Cursor;

void print_row(Row* row){
	printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

void serialize_row(Row* source, void* destination) {
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
	memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void *source, Row* destination) {
	memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

Cursor* table_start(Table* table) {
  Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
  cursor -> table = table;
  cursor -> row_num = 0;
  cursor -> end_of_table = (table->num_rows == 0);
  return cursor;
}

Cursor* table_end(Table* table) {
  Cursor* cursor = (Cursor*)malloc(sizeof(Cursor));
  cursor -> table = table;
  cursor -> row_num = table -> num_rows;
  cursor -> end_of_table = true;
  return cursor;
}

void cursor_advance(Cursor* cursor) {
  cursor -> row_num += 1;
  if(cursor -> row_num >= cursor -> table -> num_rows) {
    cursor -> end_of_table = true;
  }
}

void* cursor_value(Cursor* cursor) {
  u32 row_num = cursor -> row_num;
  u32 page_num = row_num / ROWS_PER_PAGE;
  void* page = get_page(cursor->table->pager, page_num);
  u32 row_offset = row_num % ROWS_PER_PAGE;
  u32 byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}

void db_close(Table* table) {
  Pager* pager = table->pager;
  u32 num_full_pages = table->num_rows / ROWS_PER_PAGE;

  for(u32 i = 0; i < num_full_pages; i++){
    if(pager->pages[i] == NULL){
      continue;
    }
    pager_flush(pager, i, PAGE_SIZE);
    free(pager->pages[i]);
    pager->pages[i] = NULL;
  }

  //There may be a partial page to write to the end of the file
  // This should not be needed after we switch to a b-tree
  u32 num_additional = table->num_rows % ROWS_PER_PAGE;
  if(num_additional > 0) {
    u32 page_num = num_full_pages;
    if(pager->pages[page_num] != NULL) {
      pager_flush(pager, page_num, num_additional * ROW_SIZE);
      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }

  int result = close(pager->file_descriptor);
  if(result == -1){
    printf("Error closing database file\n");
    exit(EXIT_FAILURE);
  }
  for(u32 i = 0; i < TABLE_MAX_PAGES; i++) {
    void* page = pager->pages[i];
    if(page){
      free(page);
      pager->pages[i] = NULL;
    }
  }
  free(pager);
  free(table);
}

Table* db_open(const char* filename){
  Pager* pager = pager_open(filename);
  u32 num_rows = pager -> file_length / ROW_SIZE;
  Table * table = (Table*)malloc(sizeof(Table));
  table -> pager = pager;
  table -> num_rows = num_rows;
  return table;
}

InputBuffer* new_input_buffer() {
  InputBuffer * input_buffer = malloc(sizeof(InputBuffer));
  input_buffer -> buffer = NULL;
  input_buffer -> buffer_length = 0;
  input_buffer -> input_length = 0;
  return input_buffer;
}

void print_prompt() {printf("db > ");}

void print_help() {
  printf("Meta Commands Menu\n");
  printf("===============================================\n");
  printf(".help\t\tShow this menu.\n");
  printf(".exit\t\tExit the app.\n");
  printf("Prepare Commands\n");
  printf("===============================================\n");
  printf("insert\t\tFormat: insert <number> <string> <string>\n");
  printf("select\t\tSelect everything from the table\n");
}

void read_input(InputBuffer* input_buffer) {
  ssize_t bytes_read =
    getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
  
  
  if(bytes_read <= 0) {
    printf("Error reading input\n");
		exit(EXIT_FAILURE);
  }
  
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read -1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table) {
  if(strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    db_close(table);
    exit(EXIT_SUCCESS);
  } else if(strcmp(input_buffer->buffer, ".help") == 0) {
    print_help();
    return META_COMMAND_SUCCESS;
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }

}

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement){
  statement->type = STATEMENT_INSERT;
  char* keyword = strtok(input_buffer->buffer, " ");
  char* id_string = strtok(NULL, " ");
  char* username = strtok(NULL, " ");
  char* email = strtok(NULL, " ");

  if(id_string == NULL || username == NULL || email == NULL) {
    return PREPARE_SYNTAX_ERROR;
  }

  int id = atoi(id_string);
  if(id < 0){
    return PREPARE_NEGATIVE_ID;
  }
  if(strlen(username) > COLUMN_USERNAME_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }
  if(strlen(email) > COLUMN_EMAIL_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }
  statement->row_to_insert.id = id;
  strcpy(statement->row_to_insert.username, username);
  strcpy(statement->row_to_insert.email, email);

  return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    return  prepare_insert(input_buffer, statement);
  }
  if(strncmp(input_buffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecResult execute_insert(Statement* statement, Table* table) {
  if(table->num_rows >= TABLE_MAX_ROWS) {
    return EXEC_TABLE_FULL;
  }
  Row* row_to_insert = &(statement->row_to_insert);
  Cursor* cursor = table_end(table);
  serialize_row(row_to_insert, cursor_value(cursor));
  table->num_rows += 1;
  free(cursor);
  return EXEC_SUCCESS;
}

ExecResult execute_select(Statement* statement, Table* table) {
  Row row;
  Cursor* cursor = table_start(table);
  while(!(cursor->end_of_table)){
    deserialize_row(cursor_value(cursor), &row);
    print_row(&row);
    cursor_advance(cursor);
  }
  free(cursor);
  return EXEC_SUCCESS;
}


ExecResult exec_statement(Statement* statement, Table* table) {
  switch(statement->type){
  case (STATEMENT_INSERT):
    return execute_insert(statement, table);
  case (STATEMENT_SELECT):
    return execute_select(statement, table);
  }
}

int main(int argc, char* argv[]) {

  if(argc < 2) {
    printf("Please provide a database filename \n");
    exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  Table* table = db_open(filename);
   
  /* Table* table = new_table(); */
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    
    if(input_buffer->buffer[0] == '.') {
      switch(do_meta_command(input_buffer, table)){
      case (META_COMMAND_SUCCESS):
	continue;
      case (META_COMMAND_UNRECOGNIZED_COMMAND):
	printf("Unrecognized command '%s'\n", input_buffer->buffer);
	continue;
      }
    }
    Statement statement;
    switch(prepare_statement(input_buffer, &statement)) {
    case (PREPARE_SUCCESS):
      break;
    case (PREPARE_UNRECOGNIZED_STATEMENT):
      printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
      continue;
    case(PREPARE_SYNTAX_ERROR):
      printf("Syntax Error in insert. %s\n", input_buffer->buffer);
      continue;
    case(PREPARE_STRING_TOO_LONG):
      printf("Value too long for field\n");
      continue;
    case(PREPARE_NEGATIVE_ID):
      printf("ID must be positive.\n");
      continue;
    }
    exec_statement(&statement, table);
    printf("Done..\n");
  }
}

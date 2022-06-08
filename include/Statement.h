#ifndef STATEMENT_H
#define STATEMENT_H
#include "Row.h"
#include "EnumDef.h"

typedef struct {
  StatementType type;
  Row row_to_insert;
}Statement;
#endif

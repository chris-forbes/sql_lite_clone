#ifndef DATA_COM_H
#define DATA_COM_H
#include <stdint.h>
#include "Row.h"
#include "EnumDef.h"
#define u32 uint32_t
#define TABLE_MAX_PAGES 100
#define sizeof_attr(Struct, Attrib) sizeof(((Struct*)0)->Attrib)

extern const u32 ID_SIZE;
extern const u32 USERNAME_SIZE;
extern const u32 EMAIL_SIZE;
extern const u32 ID_OFFSET;
extern const u32 USERNAME_OFFSET;
extern const u32 EMAIL_OFFSET;
extern const u32 ROW_SIZE ;

extern const u32 PAGE_SIZE;
extern const u32 ROWS_PER_PAGE;
extern const u32 TABLE_MAX_ROWS;
#endif

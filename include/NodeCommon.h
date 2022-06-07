#ifndef NODE_HEADER_H
#define NODE_HEADER_H

#include <stdint.h>
#include "EnumDef.h"
#define u32 uint32_t;
#define u8 uint8_t;
const u32 NODE_TYPE_SIZE = sizeof(uint8_t);
const u32 NODE_TYPE_OFFSET = 0;
const u32 IS_ROOT_SIZE = sizeof(uint8_t);
const u32 IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const u32 PARENT_POINTER_SIZE = sizeof(u32);
const u32 PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const u8 COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

#endif

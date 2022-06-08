#ifndef NODE_HEADER_H
#define NODE_HEADER_H

#include <stdint.h>
#include "EnumDef.h"
#define u32 uint32_t
#define u8 uint8_t

extern const uint32_t NODE_TYPE_SIZE;
extern const uint32_t NODE_TYPE_OFFSET;
extern const u32 IS_ROOT_SIZE;
extern const u32 IS_ROOT_OFFSET;
extern const u32 PARENT_POINTER_SIZE;
extern const u32 PARENT_POINTER_OFFSET;
extern const u8 COMMON_NODE_HEADER_SIZE;

#endif

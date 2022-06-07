#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include "NodeCommon.h"
#include <stdint.h>
#define u32 uint32_t;
#define u8 uint8_t;

const u32 LEAF_NODE_NUM_CELLS_SIZE = sizeof(u32);
const u32 LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const u32 LEAD_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

#endif

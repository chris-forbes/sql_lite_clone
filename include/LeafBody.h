#ifndef LEAF_BODY_H
#define LEAF_BODY_H

#include <stdint.h>
#include "LeafNode.h"
#include "NodeCommon.h"
#include "DataCommon.h"

#define u32 uint32_t
#define u8 uint8_t

const u32 LEAF_NODE_KEY_SIZE = sizeof(u32);
const u32 LEAF_NODE_KEY_OFFSET = 0;
const u32 LEAD_NODE_VALUE_SIZE = ROW_SIZE;
const u32 LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const u32 LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const u32 LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAD_NODE_HEADER_SIZE;
const u32 LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

#endif

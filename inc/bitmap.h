#pragma once

#include <stdbool.h>

#include "types.h"

typedef struct BitMap BitMap;
struct BitMap {
    u64 *chunks;
    u64 bits;
    u64 bits_last_chunk;
    u64 len;
    u64 first_chunk_with_unset;
};

BitMap BitMap_new(u64 size);
bool BitMap_get(BitMap *map, u64 index);
i64 BitMap_next_unset(BitMap *map);
void BitMap_set(BitMap *map, u64 index);
void BitMap_unset(BitMap *map, u64 index);
void BitMap_free(BitMap *map);
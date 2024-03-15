#include <malloc.h>
#include <stdbit.h>
#include <stdlib.h>

#include "inc/bitmap.h"

static inline size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

#define DEBUG 1
#define DEBUG_ASSERT(cond)
#ifdef DEBUG
#include <stdio.h>
#define DEBUG_ASSERT(cond) do { if (!(cond)) { fprintf(stderr, "DEBUG_ASSERT on line %d\n", __LINE__); __builtin_trap(); } } while(0)
#endif

BitMap BitMap_new(u64 size) {
    u64 chunks_len = (size + 63) / 64;

    u64 *chunks = calloc(chunks_len, sizeof(u64));
    if (!chunks) { exit(1); };

    BitMap map = { .chunks = chunks, .bits = size, .bits_last_chunk = size % 64, .len = chunks_len, .first_chunk_with_unset = 0 };

    return map;
}

bool BitMap_get(BitMap *map, u64 index) {
    u64 chunk = index / 64;
    u64 bit = index % 64;

    DEBUG_ASSERT(index < map->bits);

    return (map->chunks[chunk] >> bit) & 1ul;
}

i64 BitMap_next_unset(BitMap *map) {
    if (map->first_chunk_with_unset >= map->len) {
        return -1;
    }

    u64 bit = stdc_trailing_ones(map->chunks[map->first_chunk_with_unset]);
    u64 index = 64 * map->first_chunk_with_unset + bit;

    if (index > map->bits) {
        return -1;
    }

    return index;
}

void BitMap_set(BitMap *map, u64 index) {
    u64 chunk = index / 64;
    u64 bit = index % 64;

    DEBUG_ASSERT(index < map->bits);

    map->chunks[chunk] |= 1ul << bit;

    if (chunk == map->first_chunk_with_unset) {
        for (; chunk < map->len - 1; ++chunk) {
            if (stdc_count_zeros_ul(map->chunks[chunk]) > 0) {
                break;
            }
        }

        if (chunk == map->len - 1) {
            chunk += stdc_count_ones_ul(map->chunks[chunk]) == map->bits_last_chunk;
        }

        map->first_chunk_with_unset = chunk;
    }
}

void BitMap_unset(BitMap *map, u64 index) {
    u64 chunk = index / 64;
    u64 bit = index % 64;

    DEBUG_ASSERT(index < map->bits);

    map->chunks[chunk] &= UINT64_MAX ^ (1ul << bit);
    map->first_chunk_with_unset = min(chunk, map->first_chunk_with_unset);
}

void BitMap_free(BitMap *map) {
    free(map->chunks);
    *map = (BitMap) {0};
}

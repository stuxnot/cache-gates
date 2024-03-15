#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/cache_alloc.h"
#include "inc/types.h"

typedef struct CacheAllocCtx CacheAllocCtx;
struct CacheAllocCtx {
    u8 *bytes;
    size_t no_gates;
    size_t counter;
};

static CacheAllocCtx ctx = {0};

void init_cache_alloc(size_t gates) {
    ctx.bytes = calloc(sizeof(uint8_t), 4160 * gates + 4 * 4096);
    ctx.bytes = (u8*) ((intptr_t) ctx.bytes & (~0xffful)) + 4096;
    memset(ctx.bytes, 0xab, 4160 * gates + 4096);
    ctx.no_gates = gates;
    ctx.counter = 0;
    for (u64 i = 0; i < gates; ++i) {
        *(u64*) (ctx.bytes + i * 4160) = 0ul;
    }
}

void free_cache_alloc(void) {
    // free(ctx.bytes);
    ctx.bytes = NULL;
    ctx.no_gates = 0;
    ctx.counter = 0;
}

void *get_cache_line(void) {
    if (ctx.bytes == NULL) {
        fprintf(stderr, "Gate Allocator has not been initialized!");
        exit(1);
    }

    if (ctx.counter + 1 == ctx.no_gates) {
        fprintf(stderr, "Do not have enough gates!\n");
        exit(1);
    }

    return &ctx.bytes[ctx.counter++ * 4160];
}

void free_cache_line(void *line) {
    if (ctx.bytes == NULL) {
        fprintf(stderr, "Gate Allocator has not been initialized!");
        exit(1);
    }

    if (line != &ctx.bytes[(ctx.counter - 1) * 4160]) {
        fprintf(stderr, "You have not freed in the correct order :^)\n");
        exit(1);
    }

    ctx.counter--;
}

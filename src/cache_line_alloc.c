#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

#include "inc/types.h"
#include "inc/bitmap.h"
#include "inc/cache_line_alloc.h"

#define DEBUG 1
#ifdef DEBUG
#include <stdio.h>
#define DEBUG_ASSERT(cond) do { if (!(cond)) { fprintf(stderr, "DEBUG_ASSERT on line %d\n", __LINE__); __builtin_trap(); } } while(0)
#else
#define DEBUG_ASSERT(cond)
#endif

typedef struct CacheLineAllocCtx CacheLineAllocCtx;
struct CacheLineAllocCtx {
    u8 *_raw;
    u8 *raw;
    u64 lines;
    u64 stride;
    BitMap free;
};

static CacheLineAllocCtx g_ctx = {0};

void CacheLine_arena_allocator(u64 lines, u64 stride) {
    CacheLineAllocCtx ctx = {0};

    DEBUG_ASSERT(stride >= 4096 && stride < 4096 * 2 && stride % 64 == 0);

    ctx._raw = (u8*) malloc(4096 * 4 + lines * stride);
    if (!ctx._raw) { exit(1); }

    ctx.lines = lines;
    ctx.stride = stride;

    // Align to cache line
    ctx.raw = (u8*) (((intptr_t) ctx._raw & ~((intptr_t) 0xfff)) + 4096);
    memset(ctx.raw, 0xab, lines * stride + 2 * 4096);

    // For the gadgets used in the cache gates we need the bytes loaded from
    // the line to be zero.
    for (u64 line = 0; line < lines; ++line) {
        *((u64*) (ctx.raw + line * stride)) = 0ul;
    }

    ctx.free = BitMap_new(lines);

    g_ctx = ctx;
}

void *CacheLine_alloc(void) {
    if (g_ctx._raw == NULL) { return NULL; }

    i64 next = BitMap_next_unset(&g_ctx.free);
    if (next < 0) { return NULL; }

    BitMap_set(&g_ctx.free, next);

    return g_ctx.raw + next * g_ctx.stride;
}

void CacheLine_free(void *line) {
    if (g_ctx._raw == NULL) { return; }

    intptr_t base = (intptr_t) g_ctx.raw;

    ptrdiff_t byte_offset = (intptr_t) line - base;

    if (byte_offset % g_ctx.stride != 0) {
        DEBUG_ASSERT(false);
        return;
    }

    ptrdiff_t index = byte_offset / g_ctx.stride;
    BitMap_unset(&g_ctx.free, index);
}

void CacheLine_free_allocator(void) {
    free(g_ctx._raw);
    BitMap_free(&g_ctx.free);
    g_ctx = (CacheLineAllocCtx) {0};
}

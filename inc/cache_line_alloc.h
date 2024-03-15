#include "types.h"

void CacheLine_arena_allocator(u64 lines, u64 stride);
void *CacheLine_alloc(void);
void CacheLine_free(void *line);
void CacheLine_free_allocator(void);

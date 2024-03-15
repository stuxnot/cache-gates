#pragma once

#include <stddef.h>

/// Initializes the cache line allocator.
void init_cache_alloc(size_t gates);

/// Frees the memory used by the cache line allocator.
void free_cache_alloc(void);

/// Returns a cache line guaranteed to have a distance of 4160 from the previous line.
void *get_cache_line(void);

/// Returns the given cache line to the allocator to be re-used. Since the allocator
/// has no strategy implemented, the user MUST free lines in reverse order of getting them.
void free_cache_line(void *line);

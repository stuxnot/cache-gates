#include <assert.h>
#include <stdbit.h>
#include <stdio.h>
#include <stdbool.h>

#include "inc/cache_line_alloc.h"

void test_alloc(void) {
    CacheLine_arena_allocator(65, 4160);

    void *line0 = CacheLine_alloc();
    void *line1 = CacheLine_alloc();
    assert((intptr_t) line1 - (intptr_t) line0 == 4160);

    CacheLine_free(line0);
    CacheLine_free(line1);

    void *lines[65];
    for (int i = 0; i < 65; ++i) {
        lines[i] = CacheLine_alloc();
        assert(lines[i]);
    }
    assert(CacheLine_alloc() == NULL);

    CacheLine_free(lines[64]);
    assert(CacheLine_alloc() == lines[64]);

    CacheLine_free(lines[17]);
    CacheLine_free(lines[13]);
    CacheLine_free(lines[64]);
    assert(CacheLine_alloc() == lines[13]);
    assert(CacheLine_alloc() == lines[17]);
    assert(CacheLine_alloc() == lines[64]);

    CacheLine_free_allocator();
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    test_alloc();
    printf("Ok.\n");

    return 0;
}
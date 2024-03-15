#include <stdbit.h>
#include <stdio.h>
#include <assert.h>

#include "inc/bitmap.h"

void test_set_all(void) {
    const u64 bits = 316;
    BitMap map = BitMap_new(bits);

    for (u64 i = 0; i < bits; ++i) {
        assert(BitMap_next_unset(&map) == (i64) i);
        BitMap_set(&map, i);
        assert(BitMap_get(&map, i));
    }
    assert(BitMap_next_unset(&map) == -1);

    u64 count = 0;
    for (u64 c = 0; c < map.len; ++c) {
        count += stdc_count_ones_ul(map.chunks[c]);
    }
    assert(count == bits);

    BitMap_free(&map);
}

void test_next_unset(void) {
    const u64 bits = 10 * 64;
    BitMap map = BitMap_new(bits);

    for (u64 i = 0; i < bits; ++i) {
        BitMap_set(&map, i);
    }

    for (int c = 0; c < 10; ++c) {
        for (i64 offset = 0; offset < 64; ++offset) {
            BitMap_unset(&map, c * 64 + offset);
            assert(BitMap_next_unset(&map) == c * 64 + offset);
            assert(stdc_count_zeros_ul(map.chunks[c]) == 1);
            BitMap_set(&map, c * 64 + offset);
        }
    }

    for (int c = 9; c >= 0; --c) {
        for (i64 offset = 63; offset >= 0; --offset) {
            BitMap_unset(&map, c * 64 + offset);
            assert(BitMap_next_unset(&map) == c * 64 + offset);
            assert(stdc_count_zeros_ul(map.chunks[c]) == 1);
            BitMap_set(&map, c * 64 + offset);
        }
    }


    BitMap_free(&map);
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    test_set_all();
    test_next_unset();
    printf("Ok.\n");

    return 0;
}
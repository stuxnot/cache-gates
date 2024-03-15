#include "inc/eval.h"

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/cache_line_alloc.h"
#include "inc/gates.h"
#include "inc/flush.h"

#define MEASUREMENTS 10000

void evaluate(cache_gate gate_fn, gate cmp_fn) {
    void *line_in0 = CacheLine_alloc();
    void *line_in1 = CacheLine_alloc();
    void *line_out = CacheLine_alloc();

    for (int b_in = 0; b_in <= 0b11; b_in++) {
        bool in0 = b_in >> 1;
        bool in1 = b_in & 1;
        bool out = cmp_fn(in0, in1);

        uint64_t counter = 0;
        for (int m = 0; m < MEASUREMENTS; ++m) {
            set(line_in0, in0);
            set(line_in1, in1);

            set(line_out, false);

            gate_fn(line_out, line_in0, line_in1);

            bool out_cache = get(line_out);
            counter += out_cache == out;
        }

        char c0 = in0 ? 't' : 'f';
        char c1 = in1 ? 't' : 'f';

        printf("input %c%c: %5.1f (%lu)\n", c0, c1, (double)counter / ((double)MEASUREMENTS / 100), counter);
    }

    CacheLine_free(line_in0);
    CacheLine_free(line_in1);
    CacheLine_free(line_out);
}

void evaluate_restore(cache_gate gate_fn, gate cmp_fn) {
    void *line_in0 = CacheLine_alloc();
    void *line_in1 = CacheLine_alloc();
    void *line_out = CacheLine_alloc();

    for (int b_in = 0; b_in <= 0b11; b_in++) {
        bool in0 = b_in >> 1;
        bool in1 = b_in & 1;
        bool out = cmp_fn(in0, in1);

        u64 counter = 0;
        for (int m = 0; m < MEASUREMENTS; ++m) {
            set(line_in0, in0);
            set(line_in1, in1);

            set(line_out, false);

            gate_fn(line_out, line_in0, line_in1);

            bool out_cache = get(line_out);
            counter += out_cache == out && get(line_in0) == in0 && get(line_in1) == in1;
        }

        char c0 = in0 ? 't' : 'f';
        char c1 = in1 ? 't' : 'f';

        printf("input %c%c: %5.1f (%lu)\n", c0, c1, (double)counter / ((double)MEASUREMENTS / 100), counter);
    }

    CacheLine_free(line_in0);
    CacheLine_free(line_in1);
    CacheLine_free(line_out);
}

void evaluate_vec2(cache_gate_vec2 gate_fn, gate cmp_fn) {
    void *line_in0 =  CacheLine_alloc();
    void *line_in1 =  CacheLine_alloc();
    void *line_out0 = CacheLine_alloc();
    void *line_out1 = CacheLine_alloc();

    for (int b_in = 0; b_in <= 0b11; b_in++) {
        bool in0 = b_in >> 1;
        bool in1 = b_in & 1;
        bool out = cmp_fn(in0, in1);

        u64 counter = 0;
        for (int m = 0; m < MEASUREMENTS; ++m) {
            set(line_in0, in0);
            set(line_in1, in1);
            set(line_out0, false);
            set(line_out1, false);

            gate_fn(line_out0, line_out1, line_in0, line_in1);

            bool success = true;
            success &= get(line_out0) == out;
            success &= get(line_out1) == out;

            counter += success;
        }

        char c0 = in0 ? 't' : 'f';
        char c1 = in1 ? 't' : 'f';

        printf("input %c%c: %5.1f (%lu)\n", c0, c1, (double) counter / ((double)MEASUREMENTS / 100), counter);
    }

    CacheLine_free(line_in0);
    CacheLine_free(line_in1);
    CacheLine_free(line_out0);
    CacheLine_free(line_out1);
}

void evaluate_not(void) {
    void *in  = CacheLine_alloc();
    void *out = CacheLine_alloc();

    u64 counter = 0;
    for (int i = 0; i < MEASUREMENTS; ++i) {
        set(in, false);
        set(out, false);

        not_impl(out, in);

        counter += get(out);
    }
    printf("input  f: %5.1f (%lu)\n", (double) counter / ((double) MEASUREMENTS / 100), counter);

    counter = 0;
    for (int i = 0; i < MEASUREMENTS; ++i) {
        set(in, true);
        set(out, false);

        not_impl(out, in);

        counter += !get(out);
    }
    printf("input  t: %5.1f (%lu)\n", (double) counter / ((double) MEASUREMENTS / 100), counter);


    CacheLine_free(in);
    CacheLine_free(out);
}

void evaluate_not_restore(void) {
    void *in  = CacheLine_alloc();
    void *out = CacheLine_alloc();

    u64 counter = 0;
    for (int i = 0; i < MEASUREMENTS; ++i) {
        set(in, false);
        set(out, false);

        not_with_restore(out, in);

        counter += get(out) && !get(in);
    }
    printf("input  f: %5.1f (%lu)\n", (double) counter / ((double) MEASUREMENTS / 100), counter);

    counter = 0;
    for (int i = 0; i < MEASUREMENTS; ++i) {
        set(in, true);
        set(out, false);

        not_with_restore(out, in);

        counter += !get(out) && get(in);
    }
    printf("input  t: %5.1f (%lu)\n", (double) counter / ((double) MEASUREMENTS / 100), counter);


    CacheLine_free(in);
    CacheLine_free(out);
}


void evaluate_not_vec2(void) {
    void *in  = CacheLine_alloc();
    void *out1 = CacheLine_alloc();
    void *out2 = CacheLine_alloc();

    u64 counter = 0;
    for (int i = 0; i < MEASUREMENTS; ++i) {
        set(in, false);
        set(out1, false);
        set(out2, false);

        not_vec2_impl(out1, out2, in);
        
        counter += get(out1) && get(out2);
    }
    printf("input  f: %5.1f (%lu)\n", (double) counter / ((double) MEASUREMENTS / 100), counter);

    counter = 0;
    for (int i = 0; i < MEASUREMENTS; ++i) {
        set(in, true);
        set(out1, false);
        set(out2, false);

        not_vec2_impl(out1, out2, in);

        counter += !(get(out1) || get(out2));
    }
    printf("input  t: %5.1f (%lu)\n", (double) counter / ((double) MEASUREMENTS / 100), counter);

    CacheLine_free(in);
    CacheLine_free(out1);
    CacheLine_free(out2);
}

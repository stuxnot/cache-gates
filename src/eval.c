#include "inc/eval.h"

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/cache_alloc.h"
#include "inc/gates.h"

void evaluate(cache_gate gate_fn, gate cmp_fn) {
    void *line_in0 = get_cache_line();
    void *line_in1 = get_cache_line();
    void *line_out = get_cache_line();

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

    free_cache_line(line_out);
    free_cache_line(line_in1);
    free_cache_line(line_in0);
}

void evaluate_restore(cache_gate gate_fn, gate cmp_fn) {
    void *line_in0 = get_cache_line();
    void *line_in1 = get_cache_line();
    void *line_out = get_cache_line();

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
            counter += out_cache == out && get(line_in0) == in0 && get(line_in1) == in1;
        }

        char c0 = in0 ? 't' : 'f';
        char c1 = in1 ? 't' : 'f';

        printf("input %c%c: %5.1f (%lu)\n", c0, c1, (double)counter / ((double)MEASUREMENTS / 100), counter);
    }

    free_cache_line(line_out);
    free_cache_line(line_in1);
    free_cache_line(line_in0);
}

void evaluate_vec(cache_gate_vec gate_fn, gate cmp_fn, int olen) {
    void *line_in0 =  get_cache_line();
    void *line_in1 =  get_cache_line();

    void **outputs = calloc(sizeof(void*), olen);
    printf("          ");
    for (int i = 0; i < olen; ++i) {
        outputs[i] = get_cache_line();
        printf("out%d           ", i + 1);
    }
    printf("\n");

    uint64_t *counter = calloc(sizeof(uint64_t), olen);

    for (int b_in = 0; b_in <= 0b11; b_in++) {
        bool in0 = b_in >> 1;
        bool in1 = b_in & 1;
        bool out = cmp_fn(in0, in1);

        memset(counter, 0, sizeof(uint64_t) * olen);
        for (int m = 0; m < MEASUREMENTS; ++m) {
            set(line_in0, in0);
            set(line_in1, in1);

            for (int i = 0; i < olen; ++i) {
                set(outputs[i], false);
            }

            gate_fn(outputs, line_in0, line_in1);

            for (int i = 0; i < olen; ++i) {
                counter[i] += get(outputs[i]) == out;
            }
        }

        char c0 = in0 ? 't' : 'f';
        char c1 = in1 ? 't' : 'f';

        printf("input %c%c:", c0, c1);
        for (int i = 0; i < olen; ++i) {
            printf(" %5.1f (%6lu)", (double) counter[i] / ((double)MEASUREMENTS / 100), counter[i]);
        }
        printf("\n");
    }

    for (int i = olen - 1; i >= 0; i--) {
        free_cache_line(outputs[i]);
    }

    free_cache_line(line_in1);
    free_cache_line(line_in0);

    free(counter);
    free(outputs);
}


void evaluate_vec2(cache_gate_vec2 gate_fn, gate cmp_fn) {
    void *line_in0 =  get_cache_line();
    void *line_in1 =  get_cache_line();
    void *line_out0 = get_cache_line();
    void *line_out1 = get_cache_line();

    for (int b_in = 0; b_in <= 0b11; b_in++) {
        bool in0 = b_in >> 1;
        bool in1 = b_in & 1;
        bool out = cmp_fn(in0, in1);

        uint64_t counter = 0;
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

    free_cache_line(line_out1);
    free_cache_line(line_out0);
    free_cache_line(line_in1);
    free_cache_line(line_in0);
}

void evaluate_not(void) {
    void *in  = get_cache_line();
    void *out = get_cache_line();

    uint64_t counter = 0;
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


    free_cache_line(out);
    free_cache_line(in);
}

void evaluate_not_restore(void) {
    void *in  = get_cache_line();
    void *out = get_cache_line();

    uint64_t counter = 0;
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


    free_cache_line(out);
    free_cache_line(in);
}


void evaluate_not_vec2(void) {
    void *in  = get_cache_line();
    void *out1 = get_cache_line();
    void *out2 = get_cache_line();

    uint64_t counter = 0;
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

    free_cache_line(out2);
    free_cache_line(out1);
    free_cache_line(in);
}

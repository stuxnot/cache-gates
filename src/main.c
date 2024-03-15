#include <stdio.h>
#include <stdbool.h>

#include "inc/flush.h"
#include "inc/cache_alloc.h"
#include "inc/gates.h"
#include "inc/eval.h"

static bool bool_nand(bool i0, bool i1) { return !(i0 & i1); }
static bool bool_nor(bool i0, bool i1) { return !(i0 | i1); }
static bool bool_xor(bool i0, bool i1) { return i0 ^ i1; }
static bool bool_and(bool i0, bool i1) { return i0 & i1; }
static bool bool_or(bool i0, bool i1) { return  i0 | i1; }

int main(int av, char** ac) {
    (void)av;
    (void)ac;

    measure_access_times();

    init_cache_alloc(21 + 6);

    printf("NOT:\n");
    evaluate_not();

    printf("NOT (with input restore)\n");
    evaluate_not_restore();

    printf("NAND:\n");
    evaluate(nand_impl, bool_nand);

    printf("NOR:\n");
    evaluate(nor_impl, bool_nor);

    printf("AND:\n");
    evaluate(and_impl, bool_and);

    printf("AND (with input restore)\n");
    evaluate_restore(and_with_restore, bool_and);

    printf("OR:\n");
    evaluate(or_impl, bool_or);

    printf("OR (with input restore)\n");
    evaluate_restore(or_with_restore, bool_or);

    printf("XOR\n");
    evaluate(xor_impl, bool_xor);

    printf("XOR (using NAND)\n");
    evaluate(xor_impl_nand, bool_xor);

    printf("NOT (vec2):\n");
    evaluate_not_vec2();

    printf("NAND (vec2):\n");
    evaluate_vec2(nand_vec2_impl, bool_nand);

    printf("NOR (vec2):\n");
    evaluate_vec2(nor_vec2_impl, bool_nor);

    free_cache_alloc();
}

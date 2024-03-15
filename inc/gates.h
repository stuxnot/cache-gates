#pragma once

#include <stdbool.h>

void set(void*, bool);
void not_impl(void* out, void* in);
void not_vec2_impl(void *out1, void *out2, void *in);
void nor_impl(void* out, void* in1, void* in2);
void nor_vec2_impl(void *out1, void *out2, void* in1, void* in2);
void nand_impl(void* out, void* in1, void* in2);
void nand_vec2_impl(void *out1, void *out2, void* in1, void* in2);
void and_impl(void* out, void* in1, void* in2);
void xor_impl(void* out, void* in1, void* in2);
void xor_impl_nand(void* out, void* in1, void* in2);
void or_impl(void* out, void* in1, void* in2);
void not_with_restore(void *out, void *in);
void or_with_restore(void *out, void *in1, void *in2);
void and_with_restore(void *out, void *in1, void *in2);
void and_accumulate(void *out, void *in);
void or_accumulate(void *out, void *in);

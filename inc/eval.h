#pragma once

#include <stdbool.h>
#include <stddef.h>

#define MEASUREMENTS 10000

typedef void (*cache_gate)(void*, void*, void*);
typedef bool (*gate)(bool, bool);
typedef void (*cache_gate_vec)(void**, void*, void*);
typedef void (*cache_gate_vec2)(void*, void*, void*, void*);

void evaluate(cache_gate gate_fn, gate cmp_fn);
void evaluate_restore(cache_gate gate_fn, gate cmp_fn);
void evaluate_vec(cache_gate_vec gate_fn, gate cmp_fn, int olen);
void evaluate_vec2(cache_gate_vec2 gate_fn, gate cmp_fn);
void evaluate_not(void);
void evaluate_not_restore(void);
void evaluate_not_vec2(void);
void evaluate_not_vec(int olen);
void evaluate_led(void);
void evaluate_or_accumulate(void);
void evaluate_and_accumulate(void);

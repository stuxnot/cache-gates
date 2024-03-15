#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <x86intrin.h>

#include "inc/flush.h"
#include "inc/types.h"

struct RollingMean {
    double count;
    double mean;
};

static u32 limit = 0;

// checks access time to the line
bool get(const void *line)
{
    u32 measure = 0;

    // flush_reload base without flush :^)
    asm volatile
    (
        "mfence\n\t"
        "lfence\n\t"
        "rdtscp\n\t"
        "lfence\n\t"
        "mov rdi, rax\n\t"
        "mov rbx, [%1]\n\t"
        "lfence\n\t"
        "rdtscp\n\t"
        "sub rax, rdi\n\t"
        // "clflush [%1]\n\t"
        : "+a" (measure)
        : "r" (line)
        : "rbx", "rcx", "rdx", "rdi"
    );

    return measure + limit < 2 * limit;
}

static
u64 rdtscp(void)
{
    u32 low, high;
    asm volatile
    (
        "mfence\n\t"
        "lfence\n\t"
        "rdtscp\n\t"
        "lfence\n\t"
        : "=a" (low), "=d" (high)
    );

    return ((u64) high << 32) | ((u64) low);
}

void measure_access_times(void)
{
    // warm-up loop
    for (u64 i = 0; i < 10000000000; ++i)
    {
        asm volatile ("add rax, rax\n\t" : : : "rax");
    }

    u8 *_line = malloc(3 * 4096);
    if (!_line) exit(1);

    u8 *line = (u8*) ((intptr_t)_line & ~(0xffful)) + 4096;

    memset(line, 0xab, 4096);
    
    struct RollingMean mean = {0};

    for (int i = 0; i < 100000; ++i) {
        _mm_clflush(line);
        u64 before = rdtscp();
        asm volatile ("mov rax, [%0]\n\t" : : "r" (line));
        u64 diff = rdtscp() - before;

        mean.count += 1;
        float delta = diff - mean.mean;
        mean.mean += delta / mean.count; 
    }

    for (int i = 0; i < 100000; ++i) {
        u64 before = rdtscp();
        asm volatile ("mov rax, [%0]\n\t" : : "r" (line));
        u64 diff = rdtscp() - before;

        mean.count += 1;
        float delta = diff - mean.mean;
        mean.mean += delta / mean.count; 
    }

    limit = (int) mean.mean;
    printf("fr limit: %d\n", limit);

    free(_line);
}


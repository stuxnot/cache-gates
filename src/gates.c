#include "inc/gates.h"
#include "inc/cache_line_alloc.h"

#define DELAY_OPS "4"

void set(void *line, bool value) {
    asm volatile (
        "xor rax, rax                                 \n\t"
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "clflush [%0]                                 \n\t"
        "cmp %1, 0                                    \n\t"
        "je .end                                      \n\t"
        "lfence                                       \n\t"
        "mov rax, [%0]                                \n\t"
        ".end:                                        \n\t"
        "mfence                                       \n\t"
        : "+r" (line), "+r" (value)
        :
        : "rax"
    );
}


void not_impl(void *out, void *in) {
    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_not                              \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%0 + rax]                          \n\t"
        "lfence                                       \n\t"
        "gadget_not:                                  \n\t"
        "lea rbx, [rip + end_not]                     \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%1]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_not:                                     \n\t"
        "nop                                          \n\t"
        : "+r" (out), "+r" (in)
        :
        : "rax", "rbx", "r11"
    );
}

void nand_impl(void *out, void *in1, void* in2) {
    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_nand                             \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%0 + rax]                          \n\t"
        "lfence                                       \n\t"
        "gadget_nand:                                 \n\t"
        "lea rbx, [rip + end_nand]                    \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%1]                                \n\t"
        "add r11, [%2]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_nand:                                    \n\t"
        "    nop                                      \n\t"
        : "+r" (out), "+r" (in1), "+r" (in2) 
        :
        : "rax", "rbx", "r11"
    );
}

void not_vec2_impl(void *out1, void *out2, void *in) {
    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_notv2                            \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%0 + rax]                          \n\t"
        "mov r11, [%1 + rax]                          \n\t"
        "lfence                                       \n\t"
        "gadget_notv2:                                \n\t"
        "lea rbx, [rip + end_notv2]                   \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%2]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_notv2:                                   \n\t"
        "nop                                          \n\t"
        : "+r" (out1), "+r" (out2), "+r" (in) 
        :
        : "rax", "rbx", "r11"
    );
}

void nand_vec2_impl(void *out1, void *out2, void *in1, void* in2) {
    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_nandv2                           \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%0 + rax]                          \n\t"
        "mov r11, [%1 + rax]                          \n\t"
        "lfence                                       \n\t"
        "gadget_nandv2:                               \n\t"
        "lea rbx, [rip + end_nandv2]                  \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%2]                                \n\t"
        "add r11, [%3]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_nandv2:                                  \n\t"
        "nop                                          \n\t"
        : "+r" (out1), "+r" (out2), "+r" (in1), "+r" (in2)
        :
        : "rax", "rbx", "r11"
    );
}

void nor_impl(void *out, void *in1, void* in2) {
    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_nor_1                            \n\t"
        "call gadget_nor_2                            \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%0 + rax]                          \n\t"
        "lfence                                       \n\t"
        "gadget_nor_1:                                \n\t"
        "lea rbx, [rip + end_nor]                     \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%2]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "gadget_nor_2:                                \n\t"
        "lea rbx, [rip + end_nor]                     \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%1]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_nor:                                     \n\t"
        "nop                                          \n\t"
        : "+r" (out),  "+r" (in1), "+r" (in2)
        : 
        : "rax", "rbx", "r11"
    );
}

void nor_vec2_impl(void *out1, void *out2, void *in1, void* in2) {
    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_norv2_1                          \n\t"
        "call gadget_norv2_2                          \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%0 + rax]                          \n\t"
        "mov r11, [%1 + rax]                          \n\t"
        "lfence                                       \n\t"
        "gadget_norv2_1:                              \n\t"
        "lea rbx, [rip + end_norv2]                   \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%2]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "gadget_norv2_2:                              \n\t"
        "lea rbx, [rip + end_norv2]                   \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%3]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_norv2:                                   \n\t"
        "nop                                          \n\t"
        : "+r" (out1), "+r" (out2),  "+r" (in1), "+r" (in2) 
        :
        : "rax", "rbx", "r11"
    );
}

void and_impl(void *out, void *in1, void *in2) {
    void *flush_line = CacheLine_alloc();
    set(flush_line, false);

    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_and                              \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov r11, [%1 + rax]                          \n\t"
        "add r11, [%2 + rax]                          \n\t"
        "add r11, [%0 + r11]                          \n\t"
        "lfence                                       \n\t"
        "gadget_and:                                  \n\t"
        "lea rbx, [rip + end_and]                     \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%3]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_and:                                     \n\t"
        "nop                                          \n\t"
        : "+r" (out), "+r" (in1), "+r" (in2)
        : "r" (flush_line)
        : "rax", "rbx", "r11"
    );

    CacheLine_free(flush_line);
}

void or_impl(void *out, void *in1, void *in2) {
    void *flush_line = CacheLine_alloc();
    set(flush_line, false);

    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_or                               \n\t"
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        "mov rbx, [%1 + rax]                          \n\t"
        "mov rcx, [%2 + rax]                          \n\t"
        "mov r11, [%0 + rbx]                          \n\t"
        "mov r11, [%0 + rcx]                          \n\t"
        "lfence                                       \n\t"
        "gadget_or:                                   \n\t"
        "lea rbx, [rip + end_or]                      \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%3]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_or:                                      \n\t"
        "nop                                          \n\t"
        : "+r"(out), "+r" (in1), "+r" (in2)
        : "r" (flush_line)
        : "rax", "rbx", "rcx", "r11"
    );

    CacheLine_free(flush_line);
}


// Uses a hand-crafted approach instead of using nand gates,
// and results in a higher accuracy.
void xor_impl(void *out, void *in1, void *in2) {
    void *line1 = CacheLine_alloc();
    void *line2 = CacheLine_alloc();
    void *line3 = CacheLine_alloc();
    void *line4 = CacheLine_alloc();

    set(line1, false);
    set(line2, false);
    set(line3, false);
    set(line4, false);

    // create temporary copies to be able to access
    // the input lines twice.
    not_vec2_impl(line1, line3, in1);
    not_vec2_impl(line2, line4, in2);

    asm volatile (
        "lfence                                       \n\t"
        "mfence                                       \n\t"
        "call gadget_xor1                             \n\t"
        // Speculatively executed if the access to line1 (!in1) is not cached.
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        // out is loaded if r11 can be calculated before the speculation result has 
        // arrived, i.e. if line2 (!in2) is cached.
        "mov rbx, [%2 + rax]                          \n\t"
        "mov r11, [%0 + rbx]                          \n\t"
        "lfence                                       \n\t"
        // Speculation gadget for access on line1.
        "gadget_xor1:                                 \n\t"
        "lea rbx, [rip + gadget_call_xor]             \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%1]                                \n\t"
        "add [rsp], r11                               \n\t"
        // Returns to gadget_call_xor.
        "ret                                          \n\t"
        "gadget_call_xor:                             \n\t"
        "call gadget_xor2                             \n\t"
        // Speculatively executed if the access to line4 (!in2) is slow.
        "xor rax, rax                                 \n\t"
        ".rept " DELAY_OPS                           "\n\t"
        "mov rax, [rsp + rax]                         \n\t"
        "and rax, 0                                   \n\t"
        ".endr                                        \n\t"
        // out is loaded if rcx can be calculated before the speculation result has 
        // arrived, i.e. if line3 (!in2) is cached.
        "mov rcx, [%3 + rax]                          \n\t"
        "mov r11, [%0 + rcx]                          \n\t"
        "lfence                                       \n\t"
        // Speculation gadget for access on line4
        "gadget_xor2:                                 \n\t"
        "lea rbx, [rip + end_xor]                     \n\t"
        "mov [rsp], rbx                               \n\t"
        "mov r11, [%4]                                \n\t"
        "add [rsp], r11                               \n\t"
        "ret                                          \n\t"
        "end_xor:                                     \n\t"
        "nop                                          \n\t"
        : "+r" (out), "+r" (line1), "+r" (line2), "+r" (line3), "+r" (line4)
        :
        : "rax", "rbx", "rcx", "r11"
    );

    CacheLine_free(line1);
    CacheLine_free(line2);
    CacheLine_free(line3);
    CacheLine_free(line4);
}

// Here is an approach using nand:
void xor_impl_nand(void *out, void *in1, void *in2) {
    void *line1 = CacheLine_alloc();
    void *line2 = CacheLine_alloc();
    void *line3 = CacheLine_alloc();
    void *line4 = CacheLine_alloc();

    set(line1, false);
    set(line2, false);
    set(line3, false);
    set(line4, false);

    not_vec2_impl(line1, line3, in1);
    not_vec2_impl(line2, line4, in2);

    set(in1, false);
    set(in2, false);

    nand_vec2_impl(in1, in2, line1, line2);

    set(line1, false);
    set(line2, false);

    nand_impl(line1, in1, line3);
    nand_impl(line2, in2, line4);

    nand_impl(out, line1, line2);

    CacheLine_free(line1);
    CacheLine_free(line2);
    CacheLine_free(line3);
    CacheLine_free(line4);
}

void not_with_restore(void *out, void *in) {
    void *tmp1 = CacheLine_alloc();

    set(tmp1, false);
    set(out, false);

    not_vec2_impl(out, tmp1, in);

    set(in, false);
    not_impl(in, tmp1);
    
    CacheLine_free(tmp1);
}


void or_with_restore(void *out, void *in1, void *in2) {
    void *tmp1 = CacheLine_alloc();
    void *tmp2 = CacheLine_alloc();

    set(out, false);

    not_with_restore(tmp1, in1);
    not_with_restore(tmp2, in2);

    nand_impl(out, tmp1, tmp2);

    CacheLine_free(tmp1);
    CacheLine_free(tmp2);
}

void and_with_restore(void *out, void *in1, void *in2) {
    void *tmp1 = CacheLine_alloc();
    void *tmp2 = CacheLine_alloc();

    set(out, false);

    not_with_restore(tmp1, in1);
    not_with_restore(tmp2, in2);

    nor_impl(out, tmp1, tmp2);

    CacheLine_free(tmp1);
    CacheLine_free(tmp2);
}

void or_accumulate(void *acc, void *in) {
    void *tmp1 = CacheLine_alloc();
    void *tmp2 = CacheLine_alloc();

    not_with_restore(tmp1, in);

    set(tmp2, false);
    not_impl(tmp2, acc);

    set(acc, false);
    nand_impl(acc, tmp1, tmp2);

    CacheLine_free(tmp1);
    CacheLine_free(tmp2);
}

void and_accumulate(void *acc, void *in) {
    void *tmp1 = CacheLine_alloc();
    void *tmp2 = CacheLine_alloc();

    not_with_restore(tmp1, in);

    set(tmp2, false);
    not_impl(tmp2, acc);

    set(acc, false);
    nor_impl(acc, tmp1, tmp2);

    CacheLine_free(tmp1);
    CacheLine_free(tmp2);
}

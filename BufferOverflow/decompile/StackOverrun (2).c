//
// This file was generated by the Retargetable Decompiler
// Website: https://retdec.com
// Copyright (c) 2017 Retargetable Decompiler <info@retdec.com>
//

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// ------------------- Function Prototypes --------------------

int32_t bar(void);
int32_t foo(char * str2);

// ------------------------ Functions -------------------------

// Address range: 0x80484fd - 0x804855b
int32_t foo(char * str2) {
    int32_t v1 = *(int32_t *)20; // 0x804850a
    int32_t v2;
    printf("My stack looks like:\n%p\n%p\n%p\n%p\n%p\n% p\n\n", (char *)v2, str2);
    int32_t str;
    strcpy((char *)&str, str2);
    puts((char *)&str);
    printf("Now the stack looks like:\n%p\n%p\n%p\n%p\n%p\n%p\n\n", str2, str2);
    int32_t v3 = *(int32_t *)20; // 0x804854d
    if (v3 != v1) {
        // 0x8048555
        __stack_chk_fail();
        // branch -> 0x804855a
    }
    // 0x804855a
    return v3 ^ v1;
}

// Address range: 0x804855c - 0x804856f
int32_t bar(void) {
    // 0x804855c
    return puts("Augh! I've been hacked!");
}

// Address range: 0x8048570 - 0x80485ff
int main(int argc, char ** argv) {
    // 0x8048570
    printf("Address of foo = %p\n", (int32_t *)foo);
    printf("Address of bar = %p\n", (int32_t *)bar);
    int32_t * str = (int32_t *)((int32_t)argv + 4); // 0x80485a7_0
    printf("strlen of input string is: %i\n", strlen((char *)*str));
    int32_t result;
    if (argc == 2) {
        // 0x80485da
        foo((char *)*str);
        result = 0;
        // branch -> 0x80485ef
    } else {
        // 0x80485c7
        puts("Please supply a string as an argument!");
        result = -1;
        // branch -> 0x80485ef
    }
    // 0x80485ef
    return result;
}

// --------------- Dynamically Linked Functions ---------------

// void __stack_chk_fail(void);
// int printf(const char * restrict format, ...);
// int puts(const char * s);
// char * strcpy(char * restrict dest, const char * restrict src);
// size_t strlen(const char * s);

// --------------------- Meta-Information ---------------------

// Detected compiler/packer: gcc (4.8.2)
// Detected functions: 3
// Decompiler release: v2.2.1 (2016-09-07)
// Decompilation date: 2017-06-02 12:37:05

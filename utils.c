#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "error.h"


int char_to_int(char c)
{
    if (!isdigit(c)) {
        return -1;
    }
    return c - '0';
}

void panic(void)
{
    exit(EXIT_FAILURE);
}

void err(void)
{
    fprintf(stderr, "ERROR\n");
}

void ok(void)
{
    printf("OK\n");
}

uint64_t average(uint64_t a, uint64_t b)
{
    if (a > b) {
        uint64_t c = a - b;
        return c/2 + b;
    } 
    else {
        uint64_t c = b - a;
        return c/2 + a;
    }
}

uint64_t atoull(const char* number)
{
    uint64_t ret = 0;
    // Implements Horner's method.
    for (size_t i = 0; number[i]; ++i) {
        ret *= 10;
        ret += char_to_int(number[i]);
    }
    return ret;
}


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
    exit(1);
}

void free_zero(char** ptr)
{
    if (ptr == NULL) {
        return;
    }
    free(*ptr);
    *ptr = NULL;
}

void logger(const char* c, int level, FILE* fp)
{
    if (LOG_LEVEL >= level) {
        fprintf(fp, "%s\n", c);
    }
}

uint64_t average(uint64_t a, uint64_t b)
{
    if (a + b > a) {
        return (a + b)/2 - (((uint64_t)1) << 32);
    }
    return (a + b) / 2;
}

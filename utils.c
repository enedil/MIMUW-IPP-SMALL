#include <ctype.h>
#include <stdlib.h>

#include "error.h"


int char_to_int(char c)
{
    if (!isdigit(c)) {
        return -1;
    }
    return c - '0';
}

void panic()
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

void log(const char* c, size_t log_level, FILE* fp = stderr)
{
    fprintf(fp, "%s\n", c);
}

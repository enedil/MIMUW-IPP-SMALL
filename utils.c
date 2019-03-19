#include <ctype.h>

#include "error.h"

int char_to_int(char c)
{
    if (!isdigit(c)) {
        error();
    }
    return c - '0';
}

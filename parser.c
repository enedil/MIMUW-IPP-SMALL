#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"


static bool is_numeric(const char* literal)
{
    do {
        if (!isdigit(*literal)) {
            return false;
        }
    } while (*(++literal));
    return true;
} 


struct call_data parse(const char* line)
{
    struct call_data ret = {0};
    if (line == NULL) {
        goto error;
    }

    //size_t line_length = strlen(line);

    // This is the only possible form of empty line.
    // It is treated as a nop.
    if (line[0] == '\0') {
        ret.op = o_nop;
        return ret;
    }

    // Comment is treated as a nop.
    if (line[0] == '#') {
        ret.op = o_nop;
        return ret;
    }

    // Every commands accepts at least one argument, thus
    // there is at least one space present.
    char *space_pos = strchr(line, ' ');
    if (space_pos == NULL) {
        goto error;
    }

    // Every command is at most 7 characters long.
    if (space_pos - line > 7) {
        goto error;
    }


    *space_pos = '\0';
    if (strcmp(line, "DECLARE") == 0) {
        ret.op = o_declare;
    } else if (strcmp(line, "REMOVE") == 0) {
        ret.op = o_remove;
    } else if (strcmp(line, "VALID") == 0) {
        ret.op = o_valid;
    } else if (strcmp(line, "ENERGY") == 0) {
        // Count the number of occurences of space.
        int count = 0;
        for (char *p = space_pos + 1; *p != 0; ++p) {
            if (*p == ' ') {
                count++;
            }
        }

        switch (count) {
            case 1:
                ret.op = o_energy1;
                break;
            case 2:
                ret.op = o_energy2;
                break;
            default:
                goto error;
        }
    } else if (strcmp(line, "EQUAL") == 0) {
        ret.op = o_equal;
    } else {
        goto error;
    }

   
    for (size_t arg_index = 0; arg_index < MAX_ARG_LIST_SIZE - 1; ++arg_index) {
        ret.args[arg_index] = space_pos + 1;
        space_pos = strchr(space_pos + 1, ' ');
        if (space_pos == NULL) {
            break;
        }
        *space_pos = '\0';
    }

    // Something isn't processed, according to format this is en error.
    if (space_pos != NULL && strchr(space_pos + 1, ' ') != NULL) {
        goto error;
    }


    
    // Check if argument count is valid.
    switch (ret.op) {
        case o_declare:
        case o_remove:
        case o_valid:
        case o_energy1:
            if (ret.args[0] == NULL || ret.args[1] != NULL) {
                goto error;
            }
            break;
        case o_energy2:
        case o_equal:
            if (ret.args[0] == NULL || ret.args[1] == NULL) {
                goto error;
            }
            break;
        default:
            break;
    }

    // Every argument is a string of digits.
    for (size_t i = 0; i < MAX_ARG_LIST_SIZE; ++i) {
        if (ret.args[i] != NULL && !is_numeric(ret.args[i])) {
            goto error;
        }
    }

    // TODO: check for excessive arguments using line length computed at the
    // beginning

    

    // second argument should be in range [1, 2^64 - 1]
    if (ret.op == o_energy1) {
        const char max_uint64_t[] = "18446744073709551615";
        if (strlen(ret.args[1]) > 20) {
            goto error;
        }
        if (strcmp(ret.args[1], max_uint64_t) > 0) {
            goto error;
        }
        if (*ret.args[1] == '0' || *ret.args[1] == '\0') {
            goto error;
        }
    }


    return ret;


error:
    ret.op = o_error;
    return ret;
}

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"


static bool is_numeric(const char* literal)
{
    do {
        if (!isdigit(*literal)) {
            return false;
        }
    } while (*(++literal));
    return true;
} 


static bool is_valid_history(const char* hist)
{
    do {
        int n = char_to_int(*hist);
        if (n < 0 || n >= QUANTUM_STATE_COUNT) {
            return false;
        }
    } while (*(++hist));
    return true;
}


struct call_data parse(char* line)
{
    struct call_data ret = {0};
    if (line == NULL) {
        goto error;
    }


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


    size_t line_length = strlen(line);
    if (line[line_length - 1] != '\n') {
        goto error;
    }

    line[line_length - 1] = '\0';

    if (line_length == 1) {
        ret.op = o_nop;
        return ret;
    }

    int space_count = 0;
    for (size_t i = 0; i < line_length - 1; ++i) {
        if (line[i] == ' ') {
            space_count++;
        }
    }
    if (space_count > 2) {
        goto error;
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
        char *c = space_pos + 1;
        while (*c) {
            if (*c == ' ') {
                count++;
            }
            c++;
        }

        switch (count) {
            case 0:
                ret.op = o_energy1;
                break;
            case 1:
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

   
    for (size_t arg_index = 0; arg_index < MAX_ARG_LIST_SIZE; ++arg_index) {
        ret.args[arg_index] = space_pos + 1;
        space_pos = strchr(space_pos + 1, ' ');
        if (space_pos == NULL) {
            break;
        }
        *space_pos = '\0';
    }

    //
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


    // first arguments of ENERGY1 should be a valid history
    // second argument should be in range [1, 2^64 - 1]
    if (ret.op == o_energy2) {
        char max_uint64_t[] = "18446744073709551615";
        size_t max_len = strlen(max_uint64_t);
//        if (ret.args[1] == NULL) {
  //          goto error;
    //    }
        size_t len = strlen(ret.args[1]);
        if (len > max_len) {
            goto error;
        }
        if (len == max_len && strcmp(ret.args[1], max_uint64_t) > 0) {
            goto error;
        }
        if (*ret.args[1] == '0' || *ret.args[1] == '\0') {
            goto error;
        }

        if (!is_valid_history(ret.args[0])) {
            goto error;
        }
    } else {
        // every other command takes quantum history strings, which can consist
        // of numbers from 0 to QUANTUM_STATE_COUNT - 1
        for (size_t i = 0; i < MAX_ARG_LIST_SIZE; ++i) {
            if (ret.args[i] != NULL && !is_valid_history(ret.args[i])) {
                goto error;
            }
        }
    }


    return ret;


error:
    ret.op = o_error;
    return ret;
}

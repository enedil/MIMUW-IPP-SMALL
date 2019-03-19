#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

struct call_data parse(const char* line)
{
    struct call_data ret = {0};
    if (line == NULL) {
        ret.op = o_error; 
        return ret;
    }

    size_t line_length = strlen(line);

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

    // every commands accepts at least one argument, thus
    // there is at least one space present
    char *space_pos = strchr(line, ' ');
    if (space_pos == NULL) {
        ret.op = o_error;
        return ret;
    }

    // Every command is at most 7 characters long.
    if (space_pos - line > 7) {
        ret.op = o_error;
        return ret;
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
                ret.op = o_error;
                return ret;
        }
    } else if (strcmp(line, "EQUAL") == 0) {
        ret.op = o_equal;
    } else {
        ret.op = o_error;
        return ret;
    }

   
    for (size_t arg_index = 0; arg_index < MAX_ARG_LIST_SIZE - 1; ++arg_index) {
        ret.args[arg_index] = space_pos + 1;
        space_pos = strchr(space_pos + 1, ' ');
        if (space_pos == NULL) {
            break;
        }
        *space_pos = '\0';
    }

    // something isn't processed, according to format this is en error
    if (strchr(space_pos + 1, ' ') != NULL) {
        ret.op = o_error;
        return ret;
    }
    
    // check if argument count is valid
    switch (ret.op) {
        case o_declare:
        case o_remove:
        case o_valid:
        case o_energy1:
            if (ret.args[0] == NULL || ret.args[1] != NULL) {
                ret.op = o_error;
                return ret;
            }
            break;
        case o_energy2:
        case o_equal:
            if (ret.args[0] == NULL || ret.args[1] == NULL) {
                ret.op = o_error;
                return ret;
            }
    }
 

    return ret;
}

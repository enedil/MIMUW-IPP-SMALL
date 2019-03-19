#include <stdint.h>
#include <stdlib.h>

#include "constants.h"
#include "error.h"



struct tokenization
{
    char *tokens[MAX_TOKEN_COUNT];
    size_t len;
};


struct tokenization tokenize(const char* line)
{
    size_t length = strlen(line);
    for (size_t i = 0; i < length; ++i) {

    } 

    
    char *line_copy = strdup(line);

    if (line_copy == NULL) {
        panic();
    }
    struct tokenization tokens = {0};
    tokens.tokens[0] = line_copy;
    if (line_copy[0] == ' ') {
        error();
    }
    for (char* p = line_copy; *p != '\0'; p++) {
        if (*p == ' ') {
            tokens.len += 1; 
            if (tokens.len >= 3) {
                error();
            }
            tokens.tokens[tokens.len] = p+1;
            *p = 0;
        }
    }

    // make it more clean and less "hacky"
        //    if (tokens.len < 2 && tokens.tokens[tokens.len] != NULL) {
        tokens.len += 1;
        //    }

    return tokens;
}

void free_tokenization(struct tokenization tokens)
{
    free(tokens.tokens[0]);
}
/*
void exec(struct tokenization t)
{
    if (t.len < 2) {
        error();
    }
    int found = 0;
    if (t.len == 2) {
        char commands[][] = {"DECLARE", "REMOVE", "VALID", "ENERGY"};
        int length = sizeof(commands) / sizeof(commands[0]);
        for (int i = 0; i < length; ++i) {
            if (*(int*)(commands[i]) == *(int*)(t.tokens[0])) {
                if (strcmp(commands[i], t.tokens[0])) {
                    found = 1;

                }
            }
        }

    } else if (t.len == 3) {
        char commands[][] = {"ENERGY", "EQUAL"};
    }

}
*/

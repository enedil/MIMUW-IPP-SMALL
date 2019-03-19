#include <stdint.h>


#include "constants.h"

struct tokenization
{
    const char *tokens[MAX_TOKEN_COUNT];
    uint8_t len;
};


struct tokenization tokenize(const char*);
void free_tokenization(struct tokenization);

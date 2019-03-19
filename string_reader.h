#include <stdint.h>

struct buffer
{
    char* buffer;
    size_t len;
    size_t alloc_size;
};

// skip rest of the line after seeing '#'
struct buffer* read_line();
void free_buffer(struct buffer**);

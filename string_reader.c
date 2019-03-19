#include <stdio.h>
#include <stdlib.h>


#include "string_reader.h"


static const int BUF_INIT_SIZE_ = 0x100;

static struct buffer* init_buffer()
{
    struct buffer* buf = calloc(1, sizeof(struct buffer));
    if (buf == NULL) {
        return NULL;
    }
    buf->buffer = calloc(BUF_INIT_SIZE_, sizeof(char));
    if (buf->buffer == NULL) {
        free(buf);
        return NULL;
    }
    buf->alloc_size = BUF_INIT_SIZE_;
    return buf;
}


struct buffer* read_line()
{
    struct buffer* buf = init_buffer();
    if (buf == NULL) {
        return NULL;
    }
    char to_read;
    do {
        // zero byte at the end
        if (buf->len >= buf->alloc_size + 1) {
            buf->alloc_size *= 2;
            buf->buffer = realloc(buf->buffer, buf->alloc_size);
            if (buf->buffer == NULL) {
                free_buffer(&buf);
                return NULL;
            }
        }
        to_read = getchar(); 
        buf->buffer[buf->len] = to_read;
        buf->len += 1;
    } while (to_read != '\n' && to_read != '#' && to_read != EOF);

    if (to_read == '#') {
        while (to_read != '\n' && to_read != EOF) {
            to_read = getchar();
        }
    }

    buf->buffer[buf->len - 1] = 0;
    return buf;
}

void free_buffer(struct buffer** buf)
{
    if (buf == NULL || *buf == NULL) {
        return;
    }

    free((*buf)->buffer);
    free(*buf);
    *buf = NULL;
}

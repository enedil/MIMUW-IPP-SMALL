#include <stdio.h>
#include <stdlib.h>


//#include "parser.h"
//#include "history_manager.h"

#include "string_reader.h"

int main()
{
    struct buffer *buf = read_line();
    printf("%s\n", buf->buffer);
    free_buffer(&buf);
    buf = read_line();
    printf("%s\n", buf->buffer);
    free_buffer(&buf);
}
/*    char sss[1000];
    for (int i = 0; i < 10; ++i) {
        fgets(sss, 999, stdin);
        struct tokenization s = tokenize(sss);
        printf("%d\n", s.len);
        free_tokenization(s);
    }
    */
    /*
    struct history h;
    history_init(&h, 0);
    history_declare(&h, "0123");
printf("%d\n", history_valid(&h, "0"));
printf("%d\n", history_valid(&h, "01"));
printf("%d\n", history_valid(&h, "012"));
printf("%d\n", history_valid(&h, "0123"));
printf("%d\n", history_valid(&h, "1"));
printf("%d\n", history_valid(&h, "01232"));
printf("---\n");
history_declare(&h, "0123210");
printf("%d\n", history_valid(&h, "01232"));
printf("%d\n", history_valid(&h, "012321"));
printf("%d\n", history_valid(&h, "0123210"));
printf("---\n");
history_remove(&h, "012321");
printf("---\n");
printf("%d\n", history_valid(&h, "0"));
printf("%d\n", history_valid(&h, "01"));
printf("%d\n", history_valid(&h, "012"));
printf("%d\n", history_valid(&h, "0123"));
printf("%d\n", history_valid(&h, "01232"));
printf("%d\n", history_valid(&h, "012321"));
printf("%d\n", history_valid(&h, "0123210"));


    history_remove(&h, "");
    printf("---\n");
printf("%d\n", history_valid(&h, "01232"));
printf("%d\n", history_valid(&h, "012321"));
printf("%d\n", history_valid(&h, "0123210"));
}*/

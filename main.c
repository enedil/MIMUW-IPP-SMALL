#include <errno.h>

#include <stdio.h>
#include <stdlib.h>


//#include "history_manager.h"
#include "parser.h"
#include "utils.h"


int main()
{
    char *line = NULL;
    size_t line_len;
    while (getline(&line, &line_len, stdin) != -1) {
        if (errno == ENOMEM) {
            panic();
        }
        struct call_data cmd = parse(line);
        //printf("operation number: %u", cmd.op);





        free_zero(&line);
    }
}

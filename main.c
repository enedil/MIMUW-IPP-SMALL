#define _XOPEN_SOURCE 700

#include <errno.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "energy_manager.h"
#include "history_manager.h"
#include "parser.h"
#include "utils.h"


int main()
{
    struct energy_class begin, end;
    energy_init(&begin, &end);
    energy_insert_begin(&begin, 432);
    energy_insert_begin(&begin, 515);
    energy_delete(energy_insert_begin(&begin, 123));
    energy_insert_begin(&begin, 4);
    energy_delete_all(&begin);


    struct history hist;
    history_init(&hist);


    char *line = NULL;
    size_t line_len = 0;
    while (getline(&line, &line_len, stdin) != -1) {
        if (errno == ENOMEM) {
            panic();
        }
        struct call_data cmd = parse(line);
        
        switch (cmd.op) {
            case o_error:
                fprintf(stderr, "ERROR\n");
                break;
            case o_nop:
                break;
            case o_remove:
                history_remove(&hist, cmd.args[0]);
                printf("OK\n");
                break;
            case o_declare:
                history_declare(&hist, cmd.args[0]);
                printf("OK\n");
                break;
            case o_valid:
                if (history_valid(&hist, cmd.args[0])) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
                break;
            case o_energy1:
                {
                    uint64_t e = history_energy1(&hist, cmd.args[0]);
                    if (e == 0) {
                        fprintf(stderr, "ERROR\n");
                    } else {
                        printf("%lu\n", e);
                    }
                }
                break;
            case o_energy2:
            case o_equal:
                logger("not implemented", l_critical, stderr);
                break;
        }


        free(line);
        line = NULL;
        line_len = 0;
    }
    
    // remove whole tree
    history_remove(&hist, "");
    free(line);
}

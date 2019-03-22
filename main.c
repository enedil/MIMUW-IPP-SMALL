#define _XOPEN_SOURCE 700

#include <errno.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "energy_manager.h"
#include "history_manager.h"
#include "parser.h"
#include "utils.h"
/*
void dddd(struct energy_class* begin, struct energy_class* end)
{
    printf("\n\nBEGIN===============\n");
    struct energy_class* ptr = begin;
    while (ptr != end) {
        printf("next: %p prev: %p energy: %20lu ref_count: % 10d succ: %p\n",
                (void*)ptr->next, (void*)ptr->prev, ptr->energy, ptr->ref_count, (void*)ptr->successor);
        ptr = ptr->next;
    }
}*/

int main()
{
    struct energy_class begin, end;
    energy_init(&begin, &end);


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
                err();
                break;
            case o_nop:
                break;
            case o_remove:
                history_remove(&hist, cmd.args[0]);
                ok();
                break;
            case o_declare:
                history_declare(&hist, cmd.args[0]);
                ok();
                break;
            case o_valid:
                if (history_from_str(&hist, cmd.args[0]) != NULL) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
                break;
            case o_energy1:
                {
                    uint64_t e = history_energy_get(&hist, cmd.args[0]);
                    if (e == 0) {
                        err();
                    } else {
                        printf("%lu\n", e);
                    }
                }
                break;
            case o_energy2:
                {
                    uint64_t energy = atoull(cmd.args[1]);
                    if (energy_set(&begin, &hist, cmd.args[0], energy)) {
                        ok();
                    } else {
                        err();
                    }
                }
                break;

            case o_equal:
                if (energy_merge(&hist, cmd.args[0], cmd.args[1])) {
                    ok();
                } else {
                    err();
                }
                break;
        }


    }
    
    energy_delete_all(&begin);
    // remove whole tree
    history_remove(&hist, "");
    free(line);
}

#include <stdint.h>
#include <stdlib.h>


#include "energy_manager.h"
#include "utils.h"


typedef struct energy_class ecls;


static inline bool is_terminal(ecls* node)
{
    return node->ref_count == -1 && node->energy == 0;
}


void energy_init(ecls *start, ecls *end)
{
    start->prev = NULL;
    start->next = end;

    end->prev = start;
    end->next = NULL;

    start->successor = end->successor = NULL;
    
    start->energy = end->energy = 0;
    start->ref_count = end->ref_count = -1;
}

ecls* energy_insert_begin(ecls *start, uint64_t energy)
{
    ecls* ret = calloc(sizeof(ecls), 1);
    if (ret == NULL) {
        panic();
    }
    
    ret->energy = energy;
    ret->ref_count = 1;

    ret->next = start->next;
    start->next = ret;

    ret->next->prev = ret;
    ret->prev = start;

    return ret;
}

void energy_delete_all(ecls* begin)
{
    ecls* current = begin->next;
    while (!is_terminal(current)) {
        current = current->next;
        free(current->prev);
    }
}

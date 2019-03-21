#include <stdint.h>
#include <stdlib.h>


#include "energy_manager.h"
#include "history_manager.h"
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

// prevents from deleting begin and end, since they are not heap-allocated 
// (and are required for subsequent correct functioning of data structures
void energy_delete(ecls* node)
{
    if (is_terminal(node)) {
        return;
    }

    ecls* prev = node->prev;
    ecls* next = node->next;

    prev->next = next;
    next->prev = prev;

    free(node);
}


static inline void compress_energy_path(struct history *hist)
{
    if (hist->cls->successor == NULL) {
        return;
    }

    hist->cls->ref_count--;
    if (hist->cls->ref_count == 0) {
        energy_delete(hist->cls);
    } else {
        hist->cls = hist->cls->successor;
    }
    compress_energy_path(hist);
}

// TODO: what if input is NULL?

// erroneus output is 0
uint64_t energy_of_molecule(struct history *hist)
{
    if (hist->cls == NULL) {
        return 0;
    }
    compress_energy_path(hist);
    return hist->cls->energy;
}


// assigns energy for a molecule
void energy_set(ecls *begin, struct history *hist, uint64_t energy)
{
    if (hist->cls == NULL) {
        hist->cls = energy_insert_begin();
    }
    compress_energy_path(hist);
    hist->cls->energy = energy;
}

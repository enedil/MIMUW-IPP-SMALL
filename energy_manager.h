#ifndef __ENERGY_MANAGER_H
#define __ENERGY_MANAGER_H



#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "history_manager.h"

struct energy_class
{
    struct energy_class *next;
    struct energy_class *prev;
    uint64_t energy;
    int32_t ref_count;

    // defer questions for class duiuhduhfhd idush (improve wording)
    struct energy_class *successor;
};


void energy_init(struct energy_class*, struct energy_class*);
struct energy_class* energy_insert_begin(struct energy_class*, uint64_t);
void energy_delete_all(struct energy_class*);
void energy_delete(struct energy_class*);
uint64_t energy_of_molecule(struct history*);

#endif

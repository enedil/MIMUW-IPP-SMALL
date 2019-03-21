#ifndef __HISTORY_MANAGER_H
#define __HISTORY_MANAGER_H


#include <stdbool.h>
#include <stdint.h>

#include "utils.h"


struct history
{
    struct history *next[QUANTUM_STATE_COUNT];
    struct energy_class *cls;
};


void history_declare(struct history*, const char*);
void history_init(struct history* hist);
void history_remove(struct history*, const char*);
bool history_valid(const struct history*, const char*);
uint64_t history_energy1(const struct history*, const char*);

#endif

#include <stdbool.h>
#include <stdint.h>

#include "utils.h"


struct energy_class
{
    struct energy_class *next;
    uint64_t energy;
};


struct history
{
    struct history *next[QUANTUM_STATE_COUNT];
    struct energy_class *cls;
};


void history_declare(struct history*, const char*);
void history_init(struct history* hist);
void history_remove(struct history*, const char*);
bool history_valid(const struct history*, const char*);
uint64_t energy1(const struct history*, const char*);

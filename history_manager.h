#include <stdint.h>

#define QUANTUM_STATE_COUNT

struct history
{
    (struct history *next)[QUANTUM_STATE_COUNT];
    uint64_t energy;
};


void history_declare(struct history*, const char*);
void history_init(struct history* hist, uint64_t energy);
void history_remove(struct history*, const char*);
int history_valid(const struct history*, const char*);
uint64_t energy1(const struct history*, const char*);

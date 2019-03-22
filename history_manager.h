#ifndef __HISTORY_MANAGER_H
#define __HISTORY_MANAGER_H


#include <stdbool.h>
#include <stdint.h>

#include "utils.h"

// Trie holding valid molecule histories.
struct history {
    // Each node can have up to QUANTUM_STATE_COUNT children.
    struct history *next[QUANTUM_STATE_COUNT];
    // Associated energy equivalence class.
    struct energy_class *cls;
};

// Initialize history structure. First argument sould be a pointer to a history 
// element initialized by user (for instance, on stack).
void history_init(struct history* hist);

// Declare history represented by string as valid. Requires the root of the trie.
void history_declare(struct history*, const char*);

// Invalidate history represented by string. Requires the root of the trie.
void history_remove(struct history*, const char*);

// Given history string, return pointer representing associated node in the
// trie.
struct history* history_from_str(struct history*, const char*);

// Get energy of a molecule with given history string. 
// If history is invalid, returns 0.
uint64_t history_energy_get(struct history*, const char*);

#endif

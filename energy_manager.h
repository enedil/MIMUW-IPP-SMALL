#ifndef __ENERGY_MANAGER_H
#define __ENERGY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "history_manager.h"

struct energy_class
{
    // Previous element in the linked list.
    struct energy_class *next;
    // Next element in the linked list.
    struct energy_class *prev;
    // Associated energy of the equivalence class.
    uint64_t energy;
    // Count of incoming edges (cardinality of equivalence class).
    int32_t ref_count;

    // Defer questions about equivalence class to successor.
    struct energy_class *successor;
};


// Initialize the "triply"-linked list structure - 
// the elements form a doubly-linked list with additional pointer, which if set,
// tells where should we defer lookup, in case one equivalence class got merged.
// Arguments represent sentinel values for the begin and the end of the list and
// should be allocated on stack.
void energy_init(struct energy_class*, struct energy_class*);

// Insert new element with energy given into the beginning of the list.
// Note that it isn't needed to implement general-purpose insert operation.
struct energy_class* energy_insert_begin(struct energy_class*, uint64_t);

// Frees all the heap-allocated memory of list with given beginning.
void energy_delete_all(struct energy_class*);

// Free memory allocated for one node of the list.
void energy_delete(struct energy_class*);

// Get energy associated with given molecule history.
uint64_t energy_get(struct history*);

// Set energy of a molecule. If it wasn't valid, insert another element into the
// linked list.
bool energy_set(struct energy_class*, struct history*, const char*, uint64_t);

// Merrge energy classes which are represented by history strings.
bool energy_merge(struct history*, const char*, const char*);

#endif

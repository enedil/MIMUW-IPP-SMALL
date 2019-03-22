#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "energy_manager.h"
#include "history_manager.h"
#include "utils.h"

typedef struct energy_class ecls;


// Checks if node indeed represents valid equivalence class of energy.
static inline bool is_sentinel(ecls *node)
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

void energy_delete_all(ecls *begin)
{
    ecls* current = begin->next;
    while (!is_sentinel(current)) {
        current = current->next;
        free(current->prev);
    }
}

// Prevents from deleting begin and end, since they are not heap-allocated 
// (and are required for subsequent correct functioning of data structure).
void energy_delete(ecls *node)
{
    if (is_sentinel(node)) {
        return;
    }

    ecls* prev = node->prev;
    ecls* next = node->next;

    prev->next = next;
    next->prev = prev;

    free(node);
}

// Straighten the path leading from hist node into its' equivalence class of
// energy.
static inline void compress_energy_path(struct history *hist)
{
    if (hist->cls->successor == NULL) {
        return;
    }

    hist->cls->ref_count--;
    if (hist->cls->ref_count == 0) {
        energy_delete(hist->cls);
    }
    else {
        hist->cls->successor->ref_count++;
        hist->cls = hist->cls->successor;
    }
    compress_energy_path(hist);
}

// Erroneus output is 0, as it is an illegal level of energy.
uint64_t energy_get(struct history *hist)
{
    if (hist->cls == NULL) {
        return 0;
    }
    compress_energy_path(hist);
    return hist->cls->energy;
}

bool energy_set(ecls *begin, struct history *root, 
                const char *hist_string, uint64_t energy)
{
    struct history *hist = history_from_str(root, hist_string);
    if (hist == NULL) {
        return false;
    }
    if (hist->cls == NULL) {
        hist->cls = energy_insert_begin(begin, energy);
    }
    compress_energy_path(hist);
    hist->cls->energy = energy;

    return true;
}

// True if merge is successfull, false otherwise.
bool energy_merge(struct history *root, const char *h1, const char *h2)
{
    struct history *hist1 = history_from_str(root, h1);
    struct history *hist2 = history_from_str(root, h2);
    if (hist1 == NULL || hist2 == NULL) {
        return false;
    }

    if (hist1 == hist2) {
        return true;
    }

    // Path compression is a side effect, thus it is done before the next
    // conditional.
    uint64_t energy_1 = energy_get(hist1), energy_2 = energy_get(hist2);
    if (energy_1 == 0 && energy_2 == 0) {
        return false;
    }

    // According to specification, equalizing same molecules is a nop.
    if (hist1->cls == hist2->cls) {
        return true;
    }

    if (energy_1 == 0) {
        hist1->cls = hist2->cls;
        hist1->cls->ref_count++;
    } 
    else if (energy_2 == 0) {
        hist2->cls = hist1->cls;
        hist2->cls->ref_count++;
    } 
    else {
        // Wire smaller equivalence class to bigger.
        if (hist1->cls->ref_count > hist2->cls->ref_count) {
            hist1->cls->energy = average(energy_1, energy_2);
            hist2->cls->energy = 0;
            hist2->cls->successor = hist1->cls;
        } 
        else {
            hist2->cls->energy = average(energy_1, energy_2);
            hist1->cls->energy = 0;
            hist1->cls->successor = hist2->cls;
        }
    }

    return true;
}

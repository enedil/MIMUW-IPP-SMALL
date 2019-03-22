#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


#include "energy_manager.h"
#include "history_manager.h"
#include "utils.h"


typedef struct energy_class ecls;

/*
static bool energy_list_invariant_check(ecls *begin, ecls *end)
{
    if (begin->ref_count != -1 || end->ref_count != -1) {
        return false;
    }
    if (begin->energy != 0 || end->energy != 0) {
        return false;
    }
    if (begin->prev != NULL || end->next != NULL) {
        return false;
    }

    ecls *ptr = begin;
    while (ptr != end) {
        if (ptr->next->prev != ptr) {
            return false;
        }
    }
}
*/

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
        hist->cls->successor->ref_count++;
        hist->cls = hist->cls->successor;
    }
    compress_energy_path(hist);
}

// TODO: what if input is NULL?

// erroneus output is 0
uint64_t energy_get(struct history *hist)
{
    if (hist->cls == NULL) {
        return 0;
    }
    compress_energy_path(hist);
    return hist->cls->energy;
}


// assigns energy for a molecule
bool energy_set(ecls *begin, struct history* root, const char *hist_string, uint64_t energy)
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


// true if success, false if failure
bool energy_merge(struct history *root, const char *h1, const char *h2)
{
    struct history *hist1 = history_from_str(root, h1);
    struct history *hist2 = history_from_str(root, h2);
    if (hist1 == NULL || hist2 == NULL) {
        return false;
    }

    // side effect is compressing paths, thus it is done above the next
    // conditional
    uint64_t energy_1 = energy_get(hist1), energy_2 = energy_get(hist2);

    // according to specification, equalizing same molecules is a nop
    if (hist1->cls == hist2->cls) {
        return true;
    }

    if (energy_1 == 0 && energy_2 == 0) {
        false;
    }

    if (energy_1 == 0) {
        hist1->cls = hist2->cls;
        hist1->cls->ref_count++;
    } else if (energy_2 == 0) {
        hist2->cls = hist1->cls;
        hist2->cls->ref_count++;
    } else {
        if (hist1->cls->ref_count > hist2->cls->ref_count) {
            hist1->cls->energy = average(energy_1, energy_2);
            hist2->cls->energy = 0;
            hist2->cls->successor = hist1->cls;
        } else {
            hist2->cls->energy = average(energy_1, energy_2);
            hist1->cls->energy = 0;
            hist1->cls->successor = hist2->cls;
        }
    }

    return true;
}
